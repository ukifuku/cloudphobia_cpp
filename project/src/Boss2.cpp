#include "Boss2.h"
#include "Gauge.h"
#include "TOBJList.h"
#include "Friend.h"
#include "Effect.h"
#include "Effect3.h"
#include "Sound.h"
#include "SXScene.h"
#include "Demo.h"
#include "Font.h"
#include "Zako01.h"
#include "Zako02.h"
#include "Zako03.h"
#include "Command.h"

void TBossBase::Transfer( s32 _x, s32 _y, s32 _speed )
{
	switch( ActFlg[KACTION_TRANSFER] ){
	case 2:{    //移動（加速ポーズ）
		if( ActCount[KACTION_TRANSFER] <= 1500 ) {
			if( (0 <= DestDeg && DestDeg <= 1024) || (3072 <= DestDeg && DestDeg <= 4095) ){
				MotionControl(Motion_Back);
			}
			else if( 1025 <= DestDeg && DestDeg <= 3071){
				MotionControl(Motion_Foward);
			}
		}
		else {
			MotionControl(Motion_Default);
		}
		   }break;
	}
	Transfer_NoMotion(_x, _y, _speed);
}

void TBossBase::Transfer_NoMotion( s32 _x, s32 _y, s32 _speed )
{
	s32 angularVel = _speed;
	s32 TransSpeed = 0;
	s32 currentRad =  ActCount[KACTION_TRANSFER];
	s32 prevRad =  ActCount[KACTION_TRANSFER]-angularVel;
	if( prevRad>0 ) TransSpeed = -RoundOff(IniDis/2.f*(cost[currentRad]-cost[prevRad]));

	switch( ActFlg[KACTION_TRANSFER] ){
	case 1:{    //位置決定
		s32 DX = _x-X;
		s32 DY = _y-Y;
		IniDis = RoundOff(sqrt(DX*DX+DY*DY));
		DestDeg = RoundOff(atan2(DY,DX)/(f32)M_PI*2048.f) & 0x0fff;
		Inc(ActFlg[KACTION_TRANSFER]);
		ActCount[KACTION_TRANSFER] = 0;
		   }break;
	case 2:{    //移動
		Sprite[Sprite_Center]->MoveR(TransSpeed * cost[DestDeg], TransSpeed * sint[DestDeg]);

		if( ActCount[KACTION_TRANSFER] >= 2048 ) {
			ActFlg[KACTION_TRANSFER] = 0;
			ActCount[KACTION_TRANSFER] = 0;
		}
		   }break;
	}
	Inc(ActCount[KACTION_TRANSFER], angularVel);

}

void TBossBase::Accel()
{
	if( (FOwner->Speed>=10) )
		BSpeed = BSpeed+0.4f;
	else if( BSpeed>0 )
		BSpeed = BSpeed-0.4f;

	if( BSpeed>6 )
		BSpeed = 6;

	if( BSpeed<0 )
		BSpeed = 0;

	Sprite[Sprite_Center]->MoveR(BSpeed,0);

}

void TBossBase::SetNextState( s32 _current )
{

}

void TBossBase::KeepPosition( s32 _x, s32 _y )
{
	//スクロールスピードが10以上、または現在位置より100ずれたら移動する
	if( ((FOwner->Speed>10) || (abs(X-_x)>100))
		&& (ActFlg[KACTION_TRANSFER] == 0) ) {
			ActFlg[KACTION_TRANSFER] = 1;
			ActCount[KACTION_TRANSFER] = 0;
	}
	Transfer_NoMotion(_x,_y,32);

}

void TBossBase::KnockBack()
{
	for( u32 i = 0 ; i < ConstForeground; i++ )
		Sprite[i]->GetSpr()->SetColor(0,ctRed);

	LastFlash = Age;
	HPGauge->Show();
	GaugeTime = Age;
	Sprite[Sprite_Center]->MoveR(1,0);
	Inc(KnockBackVel, 1);
}

TBossBase::TBossBase( TOBJList* owner )	: TOBJ(owner)
{
	FObjectName = "TBossBase";
}

TBossBase::~TBossBase()
{

}

void TBossBase::Move()
{
	TOBJ::Move();
	X = RoundOff(Sprite[Sprite_Center]->GetSpr()->FCX);
	Y = RoundOff(Sprite[Sprite_Center]->GetSpr()->FCY);
	if( HP>0 ) {
		Accel();
	}

	if( (Age-LastBuzz)>=40 ) Buzzed = false;
	if( (Age-GaugeTime)>120 ) HPGauge->Hide();
	if( (Age-LastFlash == 2) ) {
		for( u32 i = 0 ; i < ConstForeground; i++ )
			Sprite[i]->GetSpr()->SetColor(OriginColor,ctRed);          //色を戻す

		Sprite[Sprite_Center]->MoveR(-KnockBackVel,0);                  //のけぞり戻し
		KnockBackVel = 0;
	}
}

void TBossBase::OnDamaged( TOBJ* target )
{
	X = RoundOff(Sprite[Sprite_Center]->GetSpr()->FCX);
	Y = RoundOff(Sprite[Sprite_Center]->GetSpr()->FCY);
	if( target->Kind() == kHero ) {
		if( (! Buzzed) && (target->Hit[HitBuzz]) ) {
			FOwner->Scrach(this);
			Buzzed = true;
			LastBuzz = Age;
		}
		if( (target->Hit[HitBlade]) && (target->LastSlashed > LastSlashed) ) {
			switch( FOwner->Difficulty ){
			case KDIFFICULTY_SINGLE:
				{
					Dec(HP, target->STR());
				}break;
			default: {
				if( target->STR() >FDef )
					Dec(HP,target->STR()-FDef);
				else
					Dec(HP,target->STR());
					 }
			}
			KnockBack();
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WAEdamage0,7);
			LastSlashed = target->LastSlashed;
		}
	}

	if( (target->Kind() == kHShot) ) {
		Dec(HP,target->STR());
		if( CreateSE ) FOwner->SoundEffect->WavPlay(WAEdamage0,7);

		KnockBack();
		if( HP<=0 ) BonusRate = target->BonusRate;
	}

	if( (target->Kind() == kMissile) ) {
		Dec(HP,target->STR()-FDef);
		if( CreateSE ) FOwner->SoundEffect->WavPlay(WAEdamage0,7);
		KnockBack();
		if( HP<=0 ) BonusRate = target->BonusRate;
	}

	if( HP<TempHP ) TempHP = HP;
	TOBJ::OnDamaged(target);
}

void TBossBase::Die()
{
	TOBJ::Die();
	HPGauge->Die();

}

void TBossBase::OnBroken()
{
	TOBJ::OnBroken();
	FOwner->Add(new TExplarge(FOwner,X,Y,0,true));//爆発

}

void TBossBase::Init( s32 _x, s32 _y, s32 _hp, s32 _def )
{
	X = _x;
	Y = _y;
	MaxHP = _hp;
	//  MaxHP = 1;
	HP = MaxHP;
	TempHP = HP;
	FSTR = 100;
	FDef = _def;
	FKind = kEnemy;
	Score = 40000;
	Buzzed = false;
	LastBuzz = 0;
	GaugeTime = 0;
	TotalFlight = 0;
	for( u32 i = 0 ; i <= 99; i++ ) {
		ActFlg[i] = 0;
		ActCount[i] = 0;
	}
	LastFlash = 0;
	EnemyDeg = 0;
	BSpeed = 0;
	IniDis = 0;
	DestDeg = 0;
	ShotCount = 0;
	ShotCount_Spread = 0;
	ConstForeground = 0;

	OriginColor = 255;

	KnockBackVel = 0;

	//IDの付与
	FID = NewID;
	Inc(NewID);

	HPGauge  =  new TBossGauge2(FOwner,this,-128,32);
	FOwner->Add(HPGauge);
}

void TStage2Boss1::MakeHoming()
{
	s32 MaxBeam;
	s32 BeamXSpace;
	switch( FOwner->Difficulty ){
	case KDIFFICULTY_EASY:{ MaxBeam = 3;  BeamXSpace = 100;   }	break;
	case KDIFFICULTY_HARD:{ MaxBeam = 9;    BeamXSpace = 30;   } break;
	default: 
		{
			MaxBeam = 5;
			BeamXSpace = 60;
		}
	}

	s32 BCount = 0;
	while( BCount<MaxBeam ) {
		s32 TargetX = BCount*BeamXSpace;
		s32 TargetY = FOwner->GetRand(10)*40;
		//目標が自機の近くだったら、当たらないように避けてあげる
		if( CheckDistance(TargetX, TargetY, FOwner->JikiObj->X, FOwner->JikiObj->Y, 80) ) {
			continue;
		}
		f32 GX = Sprite[BChest]->GetSpr()->FCX +20 + 4*BCount;
		f32 GY = Sprite[BChest]->GetSpr()->FCY -28 + 4*BCount;
		PosRoll(GX,GY,Sprite[BChest]->GetSpr()->FCX,Sprite[BChest]->GetSpr()->FCY,Sprite[BChest]->Rdegree);
		FOwner->Add(new TZakoHomingCore(FOwner,(s32)(GX),(s32)(GY),TargetX,TargetY, 228*(BCount+1), 3));
		Inc(BCount);
	}

	BCount = 0;
	while( BCount<MaxBeam ) {
		s32 TargetX = (BCount+MaxBeam)*BeamXSpace;
		s32 TargetY = FOwner->GetRand(10)*40+ 40;
		if( CheckDistance(TargetX, TargetY, FOwner->JikiObj->X, FOwner->JikiObj->Y, 80) ) {
			continue;
		}
		f32 GX = Sprite[BChest]->GetSpr()->FCX +20- 4*BCount;
		f32 GY = Sprite[BChest]->GetSpr()->FCY -28 - 4*BCount;
		PosRoll(GX,GY,Sprite[BChest]->GetSpr()->FCX,Sprite[BChest]->GetSpr()->FCY,Sprite[BChest]->Rdegree);
		FOwner->Add(new TZakoHomingCore(FOwner,(s32)(GX),(s32)(GY),TargetX,TargetY, -228*(BCount+1), 3));
		Inc(BCount);
	}

	if( CreateSE ) FOwner->SoundEffect->WavPlay(WABeam,9);

}

void TStage2Boss1::Transfer( s32 _x, s32 _y, s32 _speed )
{
	switch( ActFlg[KACTION_S2B1_TRANSFER] ){
	case 2:{    //移動（加速ポーズ）
		if( ActCount[KACTION_S2B1_TRANSFER] <= 1500 ) {
			if( (0 <= DestDeg && DestDeg <= 1024) || (3072 <= DestDeg && DestDeg <= 4095) ){
				MotionControl(KMOTION_STAGE2BOSS1_BACK);
			}
			else if( 1025 <= DestDeg && DestDeg <= 3071){
				MotionControl(KMOTION_STAGE2BOSS1_FOWARD);
			}
		}
		else {
			MotionControl(KMOTION_STAGE2BOSS1_DEFAULT);
		}
		   }break;
	}
	Transfer_NoMotion(_x, _y, _speed);
}

void TStage2Boss1::Transfer_NoMotion( s32 _x, s32 _y, s32 _speed )
{
	s32 TransSpeed;
	s32 angularVel = _speed;
	TransSpeed = 0;
	s32 currentRad =  ActCount[KACTION_S2B1_TRANSFER];
	s32 prevRad =  ActCount[KACTION_S2B1_TRANSFER]-angularVel;
	if( prevRad>0 ) TransSpeed = -RoundOff(IniDis/2.f*(cost[currentRad]-cost[prevRad]));

	switch( ActFlg[KACTION_S2B1_TRANSFER] ){
	case 1:
		{    //位置決定
			s32 DX = _x-X;
			s32 DY = _y-Y;
			IniDis = RoundOff(sqrt(DX*DX+DY*DY));
			DestDeg = RoundOff(atan2(DY,DX)/(f32)M_PI*2048.f) & 0x0fff;
			Inc(ActFlg[KACTION_S2B1_TRANSFER]);
		}break;
	case 2:
		{    //移動
			Sprite[BHip]->MoveR(TransSpeed * cost[DestDeg], TransSpeed * sint[DestDeg]);

			if( ActCount[KACTION_S2B1_TRANSFER] >= 2048 ) {
				ActFlg[KACTION_S2B1_TRANSFER] = 0;
				ActCount[KACTION_S2B1_TRANSFER] = 0;
			}
		}break;
	}
	if( ActFlg[KACTION_S2B1_TRANSFER]>0 ) Inc(ActCount[KACTION_S2B1_TRANSFER], angularVel);

}

void TStage2Boss1::Accel()
{
	if( (FOwner->Speed>=10) )
		BSpeed = BSpeed+0.4f;
	else if( BSpeed>0 )
		BSpeed = BSpeed-0.4f;

	if( BSpeed>6 )
		BSpeed = 6;

	if( BSpeed<0 )
		BSpeed = 0;

	Sprite[BHip]->MoveR(BSpeed,0);
}

void TStage2Boss1::Fall()
{
	switch( ActFlg[KACTION_S2B1_FALL] ){
		//
	case 1:
		{
			Inc(ActFlg[KACTION_S2B1_FALL]);
			ActCount[KACTION_S2B1_FALL]  =  -1;
			s32 GX = (s32)(Sprite[BHead]->GetSpr()->FCX);
			s32 GY = (s32)(Sprite[BHead]->GetSpr()->FCY);
			FOwner->Add(new TExplarge(FOwner,GX,GY,1,true));
			if( FOwner->IsSequence() ) FOwner->RequestDelBullets();

			//頭を消す
			HideHead();

			for( u32 i  =  0 ; i < numSpr; i++) {
				//あたり判定を消す
				Sprite[i]->GetSpr()->FHit = false;
			}
			HP = 0;
			TempHP = 0;
			HPGauge->Hide();

			CalcBonus();
		}break;
		//
	case 2:{
		MotionControl(KMOTION_STAGE2BOSS1_FALL);

		if( (ActCount[KACTION_S2B1_FALL]>30) ) {
			Sprite[BHip]->MoveR(0.2f, (f32)(ActCount[KACTION_S2B1_FALL]-30)/100.f);
			s32 GX = (s32)(Sprite[BChest]->GetSpr()->FCX);
			s32 GY = (s32)(Sprite[BChest]->GetSpr()->FCY);

			FOwner->Add(new TBlood(FOwner, GX,GY,Sprite[BChest]->Rdegree + 2048));
		}

		if( Y>400 ) {
			Inc(ActFlg[KACTION_S2B1_FALL]);
			ActCount[KACTION_S2B1_FALL]  =  -1;
			FOwner->Quake(3,120);
			FOwner->Add(new TBoxContrl(FOwner,5));
			for( u32 i  =  0 ; i < numSpr; i++) {
				//水しぶきに隠れるように、Z値を変更する
				Sprite[i]->GetSpr()->FZ = Sprite[i]->GetSpr()->FZ+8000;
			}
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WAShipExp,0);
		}

		   }break;
	case 3:{
		Sprite[BHip]->MoveR(0.2f, (f32)(ActCount[KACTION_S2B1_FALL]-30)/100.f);
		//水しぶき発生
		if( (Age % 2 == 0) ) {
			FOwner->Add(new TSplash(FOwner, X+300,4));
		}

		if( Y>600 ) {
			Die();
		}
		   }
	}
	Inc(ActCount[KACTION_S2B1_FALL]);

}

void TStage2Boss1::MakeShot( s32 _pat )
{
	s32 GX = RoundOff(Sprite[BHead]->GetSpr()->FCX);
	s32 GY = RoundOff(Sprite[BHead]->GetSpr()->FCY);

	s32 DX = RoundOff(FOwner->HeroPos.X-GX);
	s32 DY = RoundOff(FOwner->HeroPos.Y-GY);
	s32 GunR = RoundOff(atan2(DY,DX)/(f32)M_PI*2048.f) & 0xfff;
	switch( _pat ){
	case 0:{
		FOwner->Add(new TZ01Shot2(FOwner,GX,GY,GunR,5,0));
		FOwner->Add(new TZ01Shot2(FOwner,GX,GY,GunR+340,5,0));
		FOwner->Add(new TZ01Shot2(FOwner,GX,GY,GunR-340,5,0));
		FOwner->Add(new TFCircle(FOwner,GX,GY,1,GunR,0));
		   }break;
	case 1:{
		FOwner->Add(new TZ01Shot2(FOwner,GX,GY,GunR,15,0));
		FOwner->Add(new TZ01Shot2(FOwner,GX,GY,GunR+256,15,0));
		FOwner->Add(new TZ01Shot2(FOwner,GX,GY,GunR-256,15,0));
		FOwner->Add(new TZ01Shot2(FOwner,GX,GY,GunR+512,15,0));
		FOwner->Add(new TZ01Shot2(FOwner,GX,GY,GunR-512,15,0));
		FOwner->Add(new TFCircle(FOwner,GX,GY,1,GunR,0));
		   }break;
	}

	if( CreateSE ) FOwner->SoundEffect->WavPlay(WABShot,9);

}

void TStage2Boss1::AppearEvent()
{
	switch( ActFlg[KACTION_S2B1_APPEAR] ){
		//初期化
	case 1:{
		Inc(ActFlg[KACTION_S2B1_APPEAR]);
		ActCount[KACTION_S2B1_APPEAR]  =  -1;
		ActFlg[KACTION_S2B1_TRANSFER]  =  1;
		   }break;
		//下降
	case 2:{
		Transfer(450,250,32);
		if( ActFlg[KACTION_S2B1_TRANSFER] == 0 ) {
			Inc(ActFlg[KACTION_S2B1_APPEAR]);
			ActCount[KACTION_S2B1_APPEAR]  =  -1;
		}
		   }break;
		//待機
	case 3:{
		MotionControl(KMOTION_STAGE2BOSS1_DEFAULT);
		if( ActCount[KACTION_S2B1_APPEAR]>30 ) {
			Inc(ActFlg[KACTION_S2B1_APPEAR]);
			ActCount[KACTION_S2B1_APPEAR]  =  -1;
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WACharge,8);
		}
		   }break;
		//ビーム構え
	case 4:{
		MotionControl(KMOTION_STAGE2BOSS1_APPEAR1);
		MakeChargeEffect(KFIXEDANGLE_EYELASER);

		if( ActCount[KACTION_S2B1_APPEAR]>60 ) {
			Inc(ActFlg[KACTION_S2B1_APPEAR]);
			ActCount[KACTION_S2B1_APPEAR]  =  -1;
			Sprite[BHip]->SetDest(520,250,4);
			MakeEyeLaser();
			Victim->Die();
		}
		   }break;
		//ビーム発射
	case 5:{
		Sprite[BHip]->AutoSlide();
		MotionControl(KMOTION_STAGE2BOSS1_APPEAR2);
		if( ActCount[KACTION_S2B1_APPEAR]>30 ) {
			Inc(ActFlg[KACTION_S2B1_APPEAR]);
			ActCount[KACTION_S2B1_APPEAR]  =  -1;
		}
		   }break;
		//待機
	case 6:{
		MotionControl(KMOTION_STAGE2BOSS1_DEFAULT);
		if( ActCount[KACTION_S2B1_APPEAR]>60 ) {
			SetNextState(KACTION_S2B1_APPEAR);
			FOwner->Add(new TWeakPoint2(FOwner,Sprite[BHead]));
		}
		   }break;
	}
	Inc(ActCount[KACTION_S2B1_APPEAR]);

}

void TStage2Boss1::MakeBeam( s32 _r )
{
	s32 GX = RoundOff(Sprite[BChest]->GetSpr()->FCX);
	s32 GY = RoundOff(Sprite[BChest]->GetSpr()->FCY);

	if( CreateSE ) FOwner->SoundEffect->WavPlay(WABeam,9);
	if( EffLevel>0 ) {
		for( u32 i = 0; i <= 20; i++ )
			FOwner->Add(new TParticle2(FOwner,GX,GY,1,random(768*2)-768,random(15)+5,random(5)+1));
	}

	FOwner->Add(new TFunBeamCore(FOwner,GX,GY,_r,2));
	FOwner->Add(new TMissileFlash(FOwner,GX,GY));

}

void TStage2Boss1::SpreadBeam()
{
	s32 beamCount;
	switch( ActFlg[KACTION_S2B1_BEAM] ){
		//初期化
	case 1:{
		Inc(ActFlg[KACTION_S2B1_BEAM]);
		ActCount[KACTION_S2B1_BEAM]  =  -1;
		ActFlg[KACTION_S2B1_TRANSFER]  =  1;
		ActCount[KACTION_S2B1_TRANSFER]  =  -1;
		   }break;
		//第一の移動先決定
	case 2:{
		switch( FOwner->Difficulty ){
		case KDIFFICULTY_SINGLE: DestX  =  320 + FOwner->GetRand(3) * 64;	break;
		default:
			DestX  =  544 + FOwner->GetRand(3) * 32;
		}

		DestY  =  128 + FOwner->GetRand(2) * 128;
		Transfer(DestX,DestY,25);

		if( ActCount[KACTION_S2B1_BEAM]>20 ) {
			Inc(ActFlg[KACTION_S2B1_BEAM]);
			ActCount[KACTION_S2B1_BEAM]  =  -1;
		}
		   }break;
		//移動&ビーム発射（上から）
	case 3:{
		Transfer(DestX,DestY,25);

		beamCount = ActCount[KACTION_S2B1_BEAM] / 10;

		switch( FOwner->Difficulty ){
		case KDIFFICULTY_EASY:if( (beamCount<=2) && (ActCount[KACTION_S2B1_BEAM] % 10 == 0) ) MakeBeam(2730 - beamCount*682);	break;
		case KDIFFICULTY_HARD:if( (beamCount<=6) && (ActCount[KACTION_S2B1_BEAM] % 10 == 0) ) MakeBeam(2816 - beamCount*256);	break;
		default:
			if( (beamCount<=5) && (ActCount[KACTION_S2B1_BEAM] % 10 == 0) ) MakeBeam(2730 - beamCount*341);
		}

		if( ActFlg[KACTION_S2B1_TRANSFER] == 0 ) {
			Inc(ActFlg[KACTION_S2B1_BEAM]);
			ActCount[KACTION_S2B1_BEAM]  =  -1;
			ActFlg[KACTION_S2B1_TRANSFER]  =  1;
			//退却判定
			CheckRetreat();
		}
		   }break;
		//第二の移動先決定
	case 4:{
		switch( FOwner->Difficulty ){
		case KDIFFICULTY_SINGLE: DestX  =  320 + FOwner->GetRand(3) * 64;	break;
		default:
			DestX  =  544 + FOwner->GetRand(3) * 32;
		}
		DestY  =  96 + FOwner->GetRand(3) * 96;
		Transfer(DestX,DestY,25);

		if( ActCount[KACTION_S2B1_BEAM]>20 ) {
			Inc(ActFlg[KACTION_S2B1_BEAM]);
			ActCount[KACTION_S2B1_BEAM]  =  -1;
		}
		   }break;
		//移動＆ビーム発射（下から）
	case 5:{
		Transfer(DestX,DestY,25);

		beamCount = ActCount[KACTION_S2B1_BEAM] / 10;
		switch( FOwner->Difficulty ){
		case KDIFFICULTY_EASY:if( (beamCount<=2) && (ActCount[KACTION_S2B1_BEAM] % 10 == 0) ) MakeBeam(2730 - beamCount*682);	break;
		case KDIFFICULTY_HARD:if( (beamCount<=6) && (ActCount[KACTION_S2B1_BEAM] % 10 == 0) ) MakeBeam(2816 - beamCount*256);	break;
		default:
			if( (beamCount<=5) && (ActCount[KACTION_S2B1_BEAM] % 10 == 0) ) MakeBeam(2730 - beamCount*341);
		}

		if( ActFlg[KACTION_S2B1_TRANSFER] == 0 ) {
			Inc(ActFlg[KACTION_S2B1_BEAM]);
			ActCount[KACTION_S2B1_BEAM]  =  -1;
			ActFlg[KACTION_S2B1_TRANSFER]  =  1;
			//退却判定
			CheckRetreat();
		}
		   }break;
		//元の位置へ
	case 6:{
		Transfer(450,250,32);
		switch( FOwner->Difficulty ){
		case KDIFFICULTY_NORMAL:
		case KDIFFICULTY_SINGLE:
			{
				if( (ActCount[KACTION_S2B1_BEAM] % 10 == 0) ) 
					MakeShot(0);	
			}
			break;
		case KDIFFICULTY_HARD:
			{
				if( (ActCount[KACTION_S2B1_BEAM] > 30) && (ActCount[KACTION_S2B1_BEAM] % 5 == 0) ) 
					MakeShot(1);
			}break;
		}
		if( ActFlg[KACTION_S2B1_TRANSFER] == 0 ) {
			Inc(ActFlg[KACTION_S2B1_BEAM]);
			ActCount[KACTION_S2B1_BEAM]  =  -1;
		}
		   }break;
		//待機＆弾吐き
	case 7:{
		MotionControl(KMOTION_STAGE2BOSS1_DEFAULT);

		switch( FOwner->Difficulty ){
		case KDIFFICULTY_NORMAL:
		case KDIFFICULTY_SINGLE:
			{
				if( (ActCount[KACTION_S2B1_BEAM] % 10 == 0) ) 
					MakeShot(0);
			}break;

		case KDIFFICULTY_HARD:
			{
				if( (ActCount[KACTION_S2B1_BEAM] % 5 == 0) )
					MakeShot(1);
			}break;
		}

		if( ActCount[KACTION_S2B1_BEAM]>30 ) {
			ActFlg[KACTION_S2B1_BEAM] = 0;
			ActCount[KACTION_S2B1_BEAM]  =  -1;
			//ホーミングへ遷移
			ActFlg[KACTION_S2B1_HOMING] = 1;
			//退却判定
			CheckRetreat();
		}
		   }break;
	}
	Inc(ActCount[KACTION_S2B1_BEAM]);

}

void TStage2Boss1::HomingMotion()
{
	switch( ActFlg[KACTION_S2B1_HOMING] ){
		//
	case 1:{
		MotionControl(KMOTION_STAGE2BOSS1_CHARGE);
		if( ActCount[KACTION_S2B1_HOMING]>20 ) {
			Inc(ActFlg[KACTION_S2B1_HOMING]);
			ActCount[KACTION_S2B1_HOMING]  =  -1;
		}
		   }break;
		//
	case 2:{
		MotionControl(KMOTION_STAGE2BOSS1_BERSERK);

		if( ActCount[KACTION_S2B1_HOMING] == 10 )  MakeHoming();

		if( ActCount[KACTION_S2B1_HOMING]>30 ) {
			Inc(ActFlg[KACTION_S2B1_HOMING]);
			ActCount[KACTION_S2B1_HOMING]  =  -1;
		}
		   }break;
	case 3:{
		MotionControl(KMOTION_STAGE2BOSS1_DEFAULT);
		if( ActCount[KACTION_S2B1_HOMING]>40 ) {
			SetNextState(KACTION_S2B1_HOMING);
			ActFlg[KACTION_S2B1_TRANSFER]  = 1;
			ActCount[KACTION_S2B1_TRANSFER]  =  -1;
			//        ActFlg[KACTION_S2B1_HOMING]  = 1;
		}
		   }break;
	}
	Inc(ActCount[KACTION_S2B1_HOMING]);

}

void TStage2Boss1::BladeMotion()
{
	switch( ActFlg[KACTION_S2B1_BLADE] ){
		//
	case 1:{
		MotionControl(KMOTION_STAGE2BOSS1_BLADEPREPARE1);
		Transfer_NoMotion(550,240,32);
		if( ActCount[KACTION_S2B1_BLADE]>10 ) {
			Inc(ActFlg[KACTION_S2B1_BLADE]);
			ActCount[KACTION_S2B1_BLADE]  =  -1;
		}
		   }break;
		//
	case 2:{
		MotionControl(KMOTION_STAGE2BOSS1_BLADEPREPARE2);
		Transfer_NoMotion(550,240,32);

		if( ActCount[KACTION_S2B1_BLADE]>60 ) {
			Inc(ActFlg[KACTION_S2B1_BLADE]);
			ActCount[KACTION_S2B1_BLADE]  =  -1;
			ActFlg[KACTION_S2B1_TRANSFER]  = 1;
			ActCount[KACTION_S2B1_TRANSFER]  =  -1;
		}
		   }break;
	case 3:{
		MotionControl(KMOTION_STAGE2BOSS1_BLADEPREPARE2);
		Transfer_NoMotion(370,240,80);
		if( ActCount[KACTION_S2B1_BLADE]>15 ) {
			Inc(ActFlg[KACTION_S2B1_BLADE]);
			ActCount[KACTION_S2B1_BLADE]  =  -1;
			BladeTrace  =  new TEnemyBladeTrace(FOwner, Sprite[BBlade], 240, 256);
			FOwner->Add(BladeTrace);
			Sprite[BBlade]->GetSpr()->FHit  =  true;
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WASwing_E, 9);
		}
		   }break;
	case 4:{
		MotionControl(KMOTION_STAGE2BOSS1_SLASH);
		Transfer_NoMotion(370,240,80);
		if( ActCount[KACTION_S2B1_BLADE]>60 ) {
			Inc(ActFlg[KACTION_S2B1_BLADE]);
			ActCount[KACTION_S2B1_BLADE]  =  -1;

			ActFlg[KACTION_S2B1_TRANSFER]  = 1;
			ActCount[KACTION_S2B1_TRANSFER]  =  -1;

			BladeTrace->Die();
			BladeTrace = NULL;
			Sprite[BBlade]->GetSpr()->FHit  =  false;
		}
		   }break;
	case 5:{
		Transfer(500,240,32);
		if( ActFlg[KACTION_S2B1_TRANSFER]=0 ) {
			Inc(ActFlg[KACTION_S2B1_BLADE]);
			ActCount[KACTION_S2B1_BLADE]  =  -1;
		}
		   }break;
	case 6:{
		MotionControl(KMOTION_STAGE2BOSS1_DEFAULT);
		if( ActCount[KACTION_S2B1_BLADE]>30 ) {
			SetNextState(KACTION_S2B1_BLADE);
		}
		   }break;
	}
	Inc(ActCount[KACTION_S2B1_BLADE]);

}

void TStage2Boss1::EyeLaserMotion()
{
	switch( ActFlg[KACTION_S2B1_EYELASER] ){
		//
	case 1:{
		Inc(ActFlg[KACTION_S2B1_EYELASER]);
		ActCount[KACTION_S2B1_EYELASER]  =  -1;
		ActFlg[KACTION_S2B1_TRANSFER]  =  1;
		ActCount[KACTION_S2B1_TRANSFER]  =  -1;

		if( CreateSE ) FOwner->SoundEffect->WavPlay(WACharge,8);
		//初期の頭の角度を記録
		OriginHeadR = FOwner->MotionDataList[KMOTION_STAGE2BOSS1_DEFAULT].Sprites[BHead].Rdeg;
		   }break;
		//
	case 2:{
		Search();
		MotionControl(KMOTION_STAGE2BOSS1_DEFAULT);
		Transfer_NoMotion(500,240,24);

		MakeHoming2();

		if( ActCount[KACTION_S2B1_EYELASER]>120 ) {
			Inc(ActFlg[KACTION_S2B1_EYELASER]);
			ActCount[KACTION_S2B1_EYELASER]  =  -1;
			MakeEyeLaser();
			Inc(EyeLaserCount);
		}
		   }break;
	case 3:{
		FOwner->MotionDataList[KMOTION_STAGE2BOSS1_DEFAULT].Sprites[BHead].Rdeg  =  OriginHeadR;
		MotionControl(KMOTION_STAGE2BOSS1_BEAM);
		Sprite[BHip]->AutoSlide();

		if( ActCount[KACTION_S2B1_EYELASER]>30 ) {
			Inc(ActFlg[KACTION_S2B1_EYELASER]);
			ActCount[KACTION_S2B1_EYELASER]  =  -1;

			ActFlg[KACTION_S2B1_TRANSFER]  =  1;
			ActCount[KACTION_S2B1_TRANSFER]  =  -1;
		}
		   }break;
	case 4:{
		MotionControl(KMOTION_STAGE2BOSS1_DEFAULT);
		Transfer_NoMotion(500,240,24);
		if( ActCount[KACTION_S2B1_EYELASER]>60 ) {
			SetNextState(KACTION_S2B1_EYELASER);
		}
		   }break;
	}
	Inc(ActCount[KACTION_S2B1_EYELASER]);

}

void TStage2Boss1::MakeEyeLaser()
{
	f32 GX = Sprite[BHead]->GetSpr()->FCX+32;
	f32 GY = Sprite[BHead]->GetSpr()->FCY-16;

	s32 GunR = (Sprite[BHead]->Rdegree + 2048 - KFIXEDANGLE_EYELASER) & 0xfff;

	PosRoll(GX, GY,Sprite[BHead]->GetSpr()->FCX,Sprite[BHead]->GetSpr()->FCY,GunR);

	FOwner->Add(new TEyeLaser(FOwner,RoundOff(GX),RoundOff(GY),GunR));
	if( CreateSE ) FOwner->SoundEffect->WavPlay(WALBeam,10);

	FOwner->Add(new TParticleSpreader(FOwner,RoundOff(GX),RoundOff(GY),48,GunR, 60, 255,128,64));


	//反動
	Sprite[BHip]->SetDest(X - RoundOff(50*cost[GunR]), RoundOff(Y - 50*sint[GunR]),8);

}

void TStage2Boss1::Search()
{
	s32 GX = RoundOff(Sprite[BHead]->GetSpr()->FCX);
	s32 GY = RoundOff(Sprite[BHead]->GetSpr()->FCY);

	s32 DX = RoundOff(FOwner->HeroPos.X-GX);
	s32 DY = RoundOff(FOwner->HeroPos.Y-GY);
	s32 GunR = (RoundOff(atan2(DY,DX)/(f32)M_PI*2048.f) + 2048 + KFIXEDANGLE_EYELASER) & 0xfff;

	if( (GunR <= 3328) && (GunR >= 768) ) GunR  =  0;

	FOwner->MotionDataList[KMOTION_STAGE2BOSS1_DEFAULT].Sprites[BHead].Rdeg  =  GunR;

	MakeChargeEffect(GunR);

}

void TStage2Boss1::MakeChargeEffect( s32 _headR )
{
	f32 GX = Sprite[BHead]->GetSpr()->FCX-32;
	f32 GY = Sprite[BHead]->GetSpr()->FCY+32;

	PosRoll(GX, GY,Sprite[BHead]->GetSpr()->FCX,Sprite[BHead]->GetSpr()->FCY,_headR+KFIXEDANGLE_EYELASER);

	s32 ParticleR = random(4095);
	FOwner->Add(new TBLight(FOwner,RoundOff(GX),RoundOff(GY),ActCount[KACTION_S2B1_EYELASER]*0.04,2000));
	FOwner->Add(new TParticle2(FOwner,RoundOff(GX)+RoundOff(40*cost[ParticleR]),RoundOff(GY)+RoundOff(40*sint[ParticleR]),1,ParticleR,-1,30));

	ParticleR = random(4095);
	FOwner->Add(new TParticle_charge(FOwner, RoundOff(GX),RoundOff(GY),ParticleR, 8, 8, 255,192,128));

}

void TStage2Boss1::MakeHoming2()
{
	s32 homingcount = ActCount[KACTION_S2B1_EYELASER] / 30;
	if( (homingcount<3) && (ActCount[KACTION_S2B1_EYELASER] % 30 == 0) ) {

		f32 GX = Sprite[BLShoulder]->GetSpr()->FCX+80;
		f32 GY = Sprite[BLShoulder]->GetSpr()->FCY-57;

		PosRoll( GX, GY,Sprite[BLShoulder]->GetSpr()->FCX,Sprite[BLShoulder]->GetSpr()->FCY,Sprite[BLShoulder]->Rdegree);

		if( CreateSE ) FOwner->SoundEffect->WavPlay(WAHoming,10);
		FOwner->Add(new TMissileFlash(FOwner,(s32)(GX),(s32)(GY)));
		FOwner->Add(new TZakoHomingCore2(FOwner,(s32)(GX),(s32)(GY), FOwner->GetRand(1024), 0));
	}
}

void TStage2Boss1::MakeEyeLight()
{
	if( HP>0 ) {
		f32 GX = Sprite[BHead]->GetSpr()->FCX-10;
		f32 GY = Sprite[BHead]->GetSpr()->FCY+8;

		PosRoll( GX, GY,Sprite[BHead]->GetSpr()->FCX,Sprite[BHead]->GetSpr()->FCY,Sprite[BHead]->Rdegree);

		FOwner->Add(new TBossEyeLight(FOwner,(s32)(GX),(s32)(GY),10));
	}
}

void TStage2Boss1::BerserkMotion()
{
	s32 BeamInterval;
	switch( ActFlg[KACTION_S2B1_BERSERK] ){
		//
	case 1:{
		Inc(ActFlg[KACTION_S2B1_BERSERK]);
		ActCount[KACTION_S2B1_BERSERK]  =  -1;
		   }break;
	case 2:{
		MotionControl(KMOTION_STAGE2BOSS1_CHARGE);
		if( ActCount[KACTION_S2B1_BERSERK]>60 ) {
			Inc(ActFlg[KACTION_S2B1_BERSERK]);
			ActCount[KACTION_S2B1_BERSERK]  =  -1;
		}
		   }break;
		//
	case 3:{
		MotionControl(KMOTION_STAGE2BOSS1_BERSERK);

		if( (ActCount[KACTION_S2B1_BERSERK] >10) ) {
			//羽を揺らす
			if( (ActCount[KACTION_S2B1_BERSERK] % 4 == 0) ) 
				MotionControl(KMOTION_STAGE2BOSS1_BERSERK2);

			//体全体を振動
			if( (ActCount[KACTION_S2B1_BERSERK] % 4 == 0) ) 
				Sprite[BHip]->RotateR(16);

			//ビーム出す
			switch( FOwner->Difficulty ){
			case KDIFFICULTY_EASY:
				BeamInterval = 15;		
				break;
			case KDIFFICULTY_NORMAL:
			case KDIFFICULTY_HARD:
				BeamInterval = 10;
				break;
			default:
				BeamInterval = 10;
			}
			if( (ActCount[KACTION_S2B1_BERSERK] % BeamInterval == 0) ) MakeBerserkBeam();
		}

		KeepPosition(500,240);

		CheckRetreat();
		   }break;
	case 4:{
		MotionControl(KMOTION_STAGE2BOSS1_DEFAULT);
		if( ActCount[KACTION_S2B1_BERSERK]>40 ) {
			ActFlg[KACTION_S2B1_BERSERK] = 0;
			ActCount[KACTION_S2B1_BERSERK]  =  -1;

			ActFlg[KACTION_S2B1_BLADE]  = 1;
			ActFlg[KACTION_S2B1_TRANSFER]  = 1;
			ActCount[KACTION_S2B1_TRANSFER]  =  -1;
		}
		   }
	}
	Inc(ActCount[KACTION_S2B1_BERSERK]);

}

void TStage2Boss1::MakeBerserkBeam()
{
	f32 GX = Sprite[BChest]->GetSpr()->FCX +32;
	f32 GY = Sprite[BChest]->GetSpr()->FCY -32;
	PosRoll(GX, GY,Sprite[BChest]->GetSpr()->FCX,Sprite[BChest]->GetSpr()->FCY,Sprite[BChest]->Rdegree);

	s32 TargetX = FOwner->JikiObj->X;
	s32 TargetY = FOwner->JikiObj->Y;
	s32 Angle = -1024+FOwner->GetRand(8)*228;
	FOwner->Add(new TZakoHomingCore(FOwner,(s32)(GX),(s32)(GY),TargetX,TargetY, Angle, 1));

	if( ! FOwner->IsSequence() )  {
		for( u32 i = 0; i <= 1; i++ ) {
			GX = Sprite[BChest]->GetSpr()->FCX +16 + 32*i;
			GY = Sprite[BChest]->GetSpr()->FCY -16 + 32*i;
			PosRoll( GX, GY,Sprite[BChest]->GetSpr()->FCX,Sprite[BChest]->GetSpr()->FCY,Sprite[BChest]->Rdegree);
			TargetX = FOwner->GetRand(15)*40;
			TargetY = FOwner->GetRand(15)*40;
			Angle = -1024+FOwner->GetRand(8)*228;
			FOwner->Add(new TZakoHomingCore(FOwner,(s32)(GX),(s32)(GY),TargetX,TargetY, Angle, 1));
		}
	}

	if( CreateSE ) FOwner->SoundEffect->WavPlay(WABeam,9);

}

void TStage2Boss1::HideHead()
{
	Sprite[BHead]->GetSpr()->FVisible = false;
	Sprite[BNeck]->GetSpr()->FVisible = false;

	if( ExtraEffects ) {
		Sprite[BHead + BConstForeground]->GetSpr()->FVisible = false;
		Sprite[BNeck + BConstForeground]->GetSpr()->FVisible = false;
	}

}

void TStage2Boss1::SetNextState( s32 _current )
{
	ActFlg[_current] = 0;
	ActCount[_current] = -1;

	switch( _current ){
	case KACTION_S2B1_APPEAR   : 
		ActFlg[KACTION_S2B1_BEAM] = 1;
		break;
	case KACTION_S2B1_BEAM     : 
		ActFlg[KACTION_S2B1_HOMING] = 1;
		break;
	case KACTION_S2B1_HOMING   : 
		ActFlg[KACTION_S2B1_BLADE] = 1;
		break;
	case KACTION_S2B1_BLADE    : 
		ActFlg[KACTION_S2B1_EYELASER] = 1;
		break;
	case KACTION_S2B1_EYELASER :
		{
			//目レーザーは３セット繰り返す
			if( EyeLaserCount>=3 ) {
				EyeLaserCount = 0;
				ActFlg[KACTION_S2B1_BEAM] = 1;
			}
			else {
				ActFlg[KACTION_S2B1_EYELASER] = 1;
			}
			ActCount[KACTION_S2B1_EYELASER]  =  -1;
		}break;
	}

	//発狂モード判定
	if( (HP <= MaxHP/3) && (ActFlg[KACTION_S2B1_BERSERK] == 0) ) {
		ActFlg[KACTION_S2B1_BERSERK] = 1;
		ActFlg[KACTION_S2B1_APPEAR] = 0;
		ActFlg[KACTION_S2B1_BEAM] = 0;
		ActFlg[KACTION_S2B1_HOMING] = 0;
		ActFlg[KACTION_S2B1_BLADE] = 0;
		ActFlg[KACTION_S2B1_EYELASER] = 0;
	}

	//退却判定
	CheckRetreat();

}

void TStage2Boss1::KeepPosition( s32 _x, s32 _y )
{
	//スクロールスピードが10以上、または現在位置より100ずれたら移動する
	if( ((FOwner->Speed>10) || (abs(X-_x)>100))
		&& (ActFlg[KACTION_S2B1_TRANSFER] == 0) ) {
			ActFlg[KACTION_S2B1_TRANSFER] = 1;
			ActCount[KACTION_S2B1_TRANSFER] = 0;
	}
	Transfer_NoMotion(_x,_y,32);

}

void TStage2Boss1::RetreatMotion()
{
	switch( ActFlg[KACTION_S2B1_RETREAT] ){
		//
	case 1:{
		MotionControl(KMOTION_STAGE2BOSS1_DEFAULT);
		if( ActCount[KACTION_S2B1_RETREAT]>30 ) {
			Inc(ActFlg[KACTION_S2B1_RETREAT]);
			ActCount[KACTION_S2B1_RETREAT]  =  -1;
			ActFlg[KACTION_S2B1_TRANSFER]  =  1;
			ActCount[KACTION_S2B1_TRANSFER]  =  -1;
		}
		   }break;
		//
	case 2:{
		Transfer(320,-240,32);
		if( ActFlg[KACTION_S2B1_TRANSFER] == 0 ) {
			Die();
			Dec(FOwner->MShipHP, MSHPMax / 2);
			FOwner->Add(new TMotherShipDamaged(FOwner, X, Y));
		}
		   }break;
	}
	Inc(ActCount[KACTION_S2B1_RETREAT]);

}

void TStage2Boss1::CheckRetreat()
{
	//退却判定
	if( (FOwner->Distance>=44895) && (ActFlg[KACTION_S2B1_RETREAT] == 0) ) {
		ActFlg[KACTION_S2B1_RETREAT] = 1;
		ActFlg[KACTION_S2B1_APPEAR] = 0;
		ActFlg[KACTION_S2B1_BEAM] = 0;
		ActFlg[KACTION_S2B1_HOMING] = 0;
		ActFlg[KACTION_S2B1_BLADE] = 0;
		ActFlg[KACTION_S2B1_EYELASER] = 0;
		ActFlg[KACTION_S2B1_BERSERK] = 0;
	}

}

TStage2Boss1::TStage2Boss1( TOBJList* owner, s32 _x, s32 _y ) : TOBJ(owner)
{
	FObjectName = "TStage2Boss1";

	X = _x;
	Y = _y;
	switch( FOwner->Difficulty ){
	case KDIFFICULTY_EASY: 
		{
			MaxHP = 700;
			FDef = 20;
		}break;
	case KDIFFICULTY_NORMAL: 
		{
			MaxHP = 1000;
			FDef = 20;
		}break;
	case KDIFFICULTY_HARD: 
		{
			MaxHP = 1000;
			FDef = 20;
		}break;
	default: 
		{
			MaxHP = 1000;
			FDef = 10;
		}
	}
	HP = MaxHP;
	TempHP = HP;
	FSTR = 100;
	FDef = 20;
	FKind = kEnemy;
	Score = 100000;
	ComBonus = 10000;
	Buzzed = false;
	LastBuzz = 0;
	GaugeTime = 0;
	TotalFlight = 0;
	for( u32 i = 0; i <= 99; i++ ) {
		ActFlg[i] = 0;
		ActCount[i] = 0;
	}
	LastFlash = 0;
	EnemyDeg = 0;
	BSpeed = 0;
	IniDis = 0;
	DestDeg = 0;
	CommandRed = true;

	FOwner->BossR = 0;
	s32 OriginZ = 16000;
	OriginColor = 255;

	EyeLaserCount = 0;

	BladeTrace = NULL;

	//IDの付与
	FID = NewID;
	Inc(NewID);

	MakeSprFromID(FOwner->GetDG(), KSPRDATA_STAGE2BOSS1, Enemy4Tex, OriginZ, ExtraEffects);
	Sprite[BBlade]->GetSpr()->FHit  =  false;

	HPGauge  =  new TBossGauge2(FOwner,this,-128,32);
	FOwner->Add(HPGauge);

	ActFlg[KACTION_S2B1_APPEAR] = 1;

	Victim = new TFriend(FOwner,X-100,Y,0.99f,4);
	Victim->SetOwnerSprite(Sprite[BRArm]);
	FOwner->Add(Victim);

	CommandRed = true;

}

TStage2Boss1::~TStage2Boss1()
{
	if( BladeTrace!=NULL )
		BladeTrace->Die();

}

void TStage2Boss1::Move()
{
	TOBJ::Move();
	X = RoundOff(Sprite[BHip]->GetSpr()->FCX);
	Y = RoundOff(Sprite[BHip]->GetSpr()->FCY);
	if( HP>0 ) {
		if( ActFlg[KACTION_S2B1_APPEAR]>0 ) AppearEvent();
		if( ActFlg[KACTION_S2B1_BEAM]>0 ) SpreadBeam();
		if( ActFlg[KACTION_S2B1_HOMING]>0 ) HomingMotion();
		if( ActFlg[KACTION_S2B1_BLADE]>0 ) BladeMotion();
		if( ActFlg[KACTION_S2B1_EYELASER]>0 ) EyeLaserMotion();
		if( ActFlg[KACTION_S2B1_BERSERK]>0 ) BerserkMotion();
		if( ActFlg[KACTION_S2B1_RETREAT]>0 ) RetreatMotion();

		MakeEyeLight();
		Accel();
	}

	if( (Age-LastBuzz)>=40 ) Buzzed = false;
	if( (Age-GaugeTime)>120 ) HPGauge->Hide();
	if( (Age-LastFlash == 2) ) {
		for( u32 i = 0; i <= 20; i++ )
			Sprite[i]->GetSpr()->SetColor(OriginColor,ctRed);          //色を戻す

		Sprite[BHip]->MoveR(-1,0);                  //のけぞり戻し
	}

	//死亡判定
	if( (HP<=0) && (ActFlg[KACTION_S2B1_FALL] == 0) )
		ActFlg[KACTION_S2B1_FALL] = 1;

	if( ActFlg[KACTION_S2B1_FALL]>0 ) Fall();

	FOwner->BossR = Sprite[BHip]->Rdegree;

}

void TStage2Boss1::OnDamaged( TOBJ* target )
{
	X = RoundOff(Sprite[BHip]->GetSpr()->FCX);
	Y = RoundOff(Sprite[BHip]->GetSpr()->FCY);
	if( target->Kind() == kHero ) {
		if( (! Buzzed) && (target->Hit[HitBuzz]) ) {
			FOwner->Scrach(this);
			Buzzed = true;
			LastBuzz = Age;
		}
		if( (target->Hit[HitBlade]) && (target->LastSlashed > LastSlashed) ) {
			if( Hit[BHead] ) {
				if( target->STR() > FDef )
					Dec(HP,target->STR()-FDef);
				else
					Dec(HP,target->STR());

				for( u32 i = 0; i <= 20; i++ )
					Sprite[i]->GetSpr()->SetColor(0,ctRed);
				LastFlash = Age;
			}
			else {
				Dec(HP,target->STR() / 2);
				if( CreateSE ) FOwner->SoundEffect->WavPlay2(WASlash_C,12,60);
			}
			HPGauge->Show();
			GaugeTime = Age;
			Sprite[BHip]->MoveR(1,0);                  //のけぞる
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WAEdamage0,7);
			LastSlashed = target->LastSlashed;
		}
	}


	if( (target->Kind() == kHShot) ) {
		if( Hit[BHead] ) {
			Dec(HP,target->STR());
			for( u32 i = 0; i <= 20; i++ )
				Sprite[i]->GetSpr()->SetColor(0,ctRed);
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WAEdamage0,7);
		}
		else {
			//      Dec(HP,target->STR / 2);
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WAEdamage1,7);
		}

		LastFlash = Age;
		Sprite[BHip]->MoveR(1,0);                  //のけぞる
		HPGauge->Show();
		GaugeTime = Age;
	}

	if( (target->Kind() == kMissile) ) {
		Dec(HP,target->STR()-FDef);
		for( u32 i = 0; i <= 20; i++ )
			Sprite[i]->GetSpr()->SetColor(0,ctRed);
		LastFlash = Age;
		Sprite[BHip]->MoveR(1,0);                  //のけぞる
		HPGauge->Show();
		GaugeTime = Age;
		if( HP<=0 ) BonusRate = target->BonusRate;
	}

	if( HP<TempHP ) TempHP = HP;
	TOBJ::OnDamaged(target);

}

void TStage2Boss1::Die()
{
	TOBJ::Die();
	HPGauge->Die();

}

void TStage2Boss1::OnBroken()
{
	TOBJ::OnBroken();

}



void TStage2Boss2::AppearEvent()
{
	switch( ActFlg[KACTION_S2B2_APPEAR] ){
		//初期化
	case 1:{
		Inc(ActFlg[KACTION_S2B2_APPEAR]);
		ActCount[KACTION_S2B2_APPEAR]  =  -1;
		DestX = 500;
		DestY = Y;
		ActFlg[KACTION_S2B2_TRANSFER] = 1;
		   }break;
		//
	case 2:{
		Transfer(DestX,DestY,16);
		if( ActFlg[KACTION_S2B2_TRANSFER] == 0 ) {
			SetNextState(KACTION_S2B2_APPEAR);
		}
		   }break;
	}
	Inc(ActCount[KACTION_S2B2_APPEAR]);

}

void TStage2Boss2::Transfer( s32 _x, s32 _y, s32 _speed )
{
	switch( ActFlg[KACTION_S2B2_TRANSFER] ){
	case 1:{    //位置決定
		s32 DX = _x-X;
		s32 DY = _y-Y;

		if( (DX==0) && (DY==0) ) return;


		IniDis = RoundOff(sqrt(DX*DX+DY*DY));
		DestDeg = RoundOff(atan2(DY,DX)/(f32)M_PI*2048.f) & 0x0fff;
		Inc(ActFlg[KACTION_S2B2_TRANSFER]);
		ActCount[KACTION_S2B2_TRANSFER] = 0;
		   }break;
	case 2:{    //移動
		s32 angularVel = _speed;
		s32 TransSpeed = 0;
		s32 currentRad =  ActCount[KACTION_S2B2_TRANSFER];
		s32 prevRad =  ActCount[KACTION_S2B2_TRANSFER]-angularVel;
		if( prevRad>0 ) TransSpeed = -RoundOff(IniDis/2.f*(cost[currentRad]-cost[prevRad]));

		Sprite[BBody]->MoveR(TransSpeed * cost[DestDeg], TransSpeed * sint[DestDeg]);

		for( u32 i = 0; i <= 7; i++ )
			TailBeam[i]->SetTargetR(RoundOff(TransSpeed * cost[DestDeg]), RoundOff(TransSpeed * sint[DestDeg]));

		if( ActCount[KACTION_S2B2_TRANSFER] >= 2048 ) {
			ActFlg[KACTION_S2B2_TRANSFER] = 0;
			ActCount[KACTION_S2B2_TRANSFER] = 0;
		}

		Inc(ActCount[KACTION_S2B2_TRANSFER], angularVel);
		   }
	}


}

void TStage2Boss2::Accel()
{
	if( (FOwner->Speed>=10) )
		BSpeed = BSpeed+0.4f;
	else if( BSpeed>0 )
		BSpeed = BSpeed-0.4f;

	if( BSpeed>6 )
		BSpeed = 6;

	if( BSpeed<0 )
		BSpeed = 0;

	Sprite[BBody]->MoveR(BSpeed,0);

}

void TStage2Boss2::MakeShot( s32 _pat )
{
	s32 firstR;
	switch( _pat ){
	case 0:{
		if( CreateSE ) FOwner->SoundEffect->WavPlay(WAHoming,10);
		FOwner->Add(new TMissileFlash(FOwner,X,Y));
		FOwner->Add(new TZakoHomingCore2(FOwner,X,Y, 512, 0));
		FOwner->Add(new TZakoHomingCore2(FOwner,X,Y, -512, 0));
		   }break;
	case 1:{
		firstR = FOwner->GetRand(4095);

		FOwner->Add(new TMissileFlash(FOwner,X,Y));
		for( u32 i = 0; i <= 15; i++ )
			FOwner->Add(new TCurvedLaser(FOwner,X,Y,firstR+i*512,16, 3, 0.1,5,1));

		if( CreateSE ) FOwner->SoundEffect->WavPlay(WABeam,9);
		   }break;
	case 2:{
		FOwner->Add(new TMissileFlash(FOwner,X,Y));
		firstR = FOwner->GetRand(4095);
		for( u32 i = 0; i <= 15; i++ )
			FOwner->Add(new TZ01Shot2(FOwner,X,Y,firstR + i*256,FOwner->GetRand(2)+4,BSpeed));

		if( CreateSE ) FOwner->SoundEffect->WavPlay(WABShot,9);
		   }
	}

}

void TStage2Boss2::MakeTail()
{
	TailBeam[0]->CalcTrajectory(X,Y,1300);
	TailBeam[1]->CalcTrajectory(X,Y,2700);
	TailBeam[2]->CalcTrajectory(X,Y,1700);
	TailBeam[3]->CalcTrajectory(X,Y,2300);
	TailBeam[4]->CalcTrajectory(X,Y,500);
	TailBeam[5]->CalcTrajectory(X,Y,-500);
	TailBeam[6]->CalcTrajectory(X,Y,800);
	TailBeam[7]->CalcTrajectory(X,Y,-800);

}

void TStage2Boss2::BeamMotion()
{
	switch( ActFlg[KACTION_S2B2_BEAM] ){
		//チャージ
	case 1:{
		if( ActCount[KACTION_S2B2_BEAM] == 60 ) {
			switch( FOwner->Difficulty ){
			case KDIFFICULTY_SINGLE:
				Inc(ActFlg[KACTION_S2B2_BEAM]);
				break;
			case KDIFFICULTY_NORMAL:
				ActFlg[KACTION_S2B2_BEAM] = 6;
				break;
			default:
				Inc(ActFlg[KACTION_S2B2_BEAM]);
			}
			ActCount[KACTION_S2B2_BEAM]  =  -1;
		}
		   }break;
		//目標設定
	case 2:{
		Inc(ActFlg[KACTION_S2B2_BEAM]);
		ActCount[KACTION_S2B2_BEAM]  =  -1;
		for( u32 i = 0; i <= 7; i++ )
			TailBeam[i]->SetTarget(FOwner->JikiObj->X, FOwner->JikiObj->Y);
		MakeTail();
		   }break;
		//ビーム発生
	case 3:{
		SlideTail();
		if( (ActCount[KACTION_S2B2_BEAM] % 5 == 0) && (ActCount[KACTION_S2B2_BEAM] <= 35) ) {
			TailBeam[ActCount[KACTION_S2B2_BEAM] / 5]->Show(true);
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WABoss2Laser,10);
		}

		if( ActCount[KACTION_S2B2_BEAM] > 60 ) {
			Inc(ActFlg[KACTION_S2B2_BEAM]);
			ActCount[KACTION_S2B2_BEAM]  =  -1;

			ActFlg[KACTION_S2B2_TRANSFER] = 1;
			DestX = 500;
			if( Y<100 )
				DestY = 240;
			else if( Y>380 )
				DestY = 240;
			else {
				if( FOwner->GetRand(1) == 0 )
					DestY = 40;
				else
					DestY = 440;
			}
		}
		   }break;
		//移動
	case 4:{
		SlideTail();
		Transfer(DestX,DestY,16);

		if( ActFlg[KACTION_S2B2_TRANSFER] == 0 ) {
			Inc(ActFlg[KACTION_S2B2_BEAM]);
			ActCount[KACTION_S2B2_BEAM]  =  -1;
			for( u32 i = 0; i <= 7; i++ )
				TailBeam[i]->Show(false);
		}
		   }break;
		//ビーム消去
	case 5:{
		SlideTail();
		if( ActCount[KACTION_S2B2_BEAM] > 30 ) {
			switch( FOwner->Difficulty ){
			case KDIFFICULTY_SINGLE:ActFlg[KACTION_S2B2_BEAM] = 2;	break;
			case KDIFFICULTY_NORMAL:ActFlg[KACTION_S2B2_BEAM] = 6;	break;
			default:
				ActFlg[KACTION_S2B2_BEAM] = 2;
			}
			ActCount[KACTION_S2B2_BEAM]  =  -1;
		}
		   }break;
		//弾ばらまき
	case 6:{
		if( (ActCount[KACTION_S2B2_BEAM] % 20 == 0) && (ActCount[KACTION_S2B2_BEAM] <= 80) )
			MakeShot(2);

		if( ActCount[KACTION_S2B2_BEAM] == 80 ) {
			ActFlg[KACTION_S2B2_BEAM] = 2;
			ActCount[KACTION_S2B2_BEAM]  =  -1;
		}
		   }break;
	}
	Inc(ActCount[KACTION_S2B2_BEAM]);


	if( (Age % 30 == 0) && (FOwner->Difficulty=KDIFFICULTY_HARD) ) MakeShot(2);

}

void TStage2Boss2::SetNextState( s32 _current )
{
	ActFlg[_current] = 0;
	ActCount[_current] = -1;

	switch( _current ){
	case KACTION_S2B2_APPEAR   : ActFlg[KACTION_S2B2_ESCAPE] = 1;	break;
	case KACTION_S2B2_BEAM     : ActFlg[KACTION_S2B2_BEAM] = 1;		break;
	case KACTION_S2B2_ESCAPE   : ActFlg[KACTION_S2B2_ESCAPE] = 1;	break;
	}

	CheckServantDead();

}

void TStage2Boss2::EscapeMotion()
{
	switch( ActFlg[KACTION_S2B2_ESCAPE] ){
		//初期化
	case 1:{
		Inc(ActFlg[KACTION_S2B2_ESCAPE]);
		ActCount[KACTION_S2B2_ESCAPE]  =  -1;

		ActFlg[KACTION_S2B2_TRANSFER] = 1;
		DestX = 500;
		DestY = 240;
		   }break;
		//自機との距離をチェック
	case 2:{
		CheckServantDead();

		Transfer(DestX,DestY,16);

		KeepPosition(DestX,DestY);

		s32 DX = FOwner->JikiObj->X-X;         
		s32 DY = FOwner->JikiObj->Y-Y;
		s32 DestDistance = RoundOff(pow(pow(DX,2)+pow(DY,2),0.5f));

		//自機がある程度近づいてきたら逃げる
		if( DestDistance <160 ) {
			MakeShot(0);
			ActFlg[KACTION_S2B2_ESCAPE] = 3;      //Inc()を使わないのは、CheckServantDeadでActFlgが0にされる可能性があるため
			ActCount[KACTION_S2B2_ESCAPE]  =  -1;

			ActFlg[KACTION_S2B2_TRANSFER] = 1;
			DestX = 500;
			DestY = Y;
			while( DestY == Y ) {
				DestY = FOwner->GetRand(4)*100 + 40;
			}
		}
		   }break;
		//移動
	case 3:{
		Transfer(DestX,DestY,32);

		if( ActFlg[KACTION_S2B2_TRANSFER] == 0 ) {
			Inc(ActFlg[KACTION_S2B2_ESCAPE]);
			ActCount[KACTION_S2B2_ESCAPE]  =  -1;
		}
		   }break;
	case 4:{

		if( ActCount[KACTION_S2B2_ESCAPE] >10 ) {
			SetNextState(KACTION_S2B2_ESCAPE);
		}
		   }break;
	}
	Inc(ActCount[KACTION_S2B2_ESCAPE]);

}

void TStage2Boss2::Fall()
{
	switch( ActFlg[KACTION_S2B2_FALL] ){
		//初期化
	case 1:{
		Inc(ActFlg[KACTION_S2B2_FALL]);
		ActCount[KACTION_S2B2_FALL]  =  -1;
		FOwner->Add(new TExplarge(FOwner,X,Y,1,true));

		if( CreateBGM ) FOwner->MusicPlayer->Stop();
		if( BonusRate>1 ) {
			FOwner->Add(new TMultiple(FOwner,BonusRate,X,Y));
			FOwner->BossRate = BonusRate;
		}

		for( u32 i  =  0 ; i < numSpr; i++) {
			//あたり判定を消す
			Sprite[i]->GetSpr()->FHit = false;
		}

		//ビーム消す
		for( u32 i = 0; i <= 7; i++ )
			TailBeam[i]->Show(false);

		HP = 0;
		TempHP = 0;
		HPGauge->Hide();
		FOwner->BossDie = true;
		for( u32 i  =  0 ; i < numSpr; i++ ) {
			//水しぶきに隠れるように、Z値を変更する
			Sprite[i]->GetSpr()->FZ = Sprite[i]->GetSpr()->FZ+8000;
		}
		   }break;
		//
	case 2:{
		MakeTail();
		//振動
		if( Age % 2 == 0 )
			Sprite[BBody]->MoveR(0,-2);
		else
			Sprite[BBody]->MoveR(0,2);

		//炎上
		FOwner->Add(new TMotherShipSmoke(FOwner,X,Y,-18,-0.2, 4, 2));
		FOwner->Add(new TMotherShipSmoke(FOwner,X-32,Y-16,-15,-0.2, 4,2));
		FOwner->Add(new TMotherShipFire(FOwner,X,Y,-18,-0.2, 4, 2.0));

		if( ActCount[KACTION_S2B2_FALL] >120 ) {
			f32 YSpeed = (f32)(ActCount[KACTION_S2B2_FALL]-120)*0.01f;
			Sprite[BBody]->MoveR(-0.5f,YSpeed);
		}

		//水しぶき発生
		if( (Y>400) && (Age % 2 == 0) ) {
			FOwner->Add(new TSplash(FOwner, X+200,4));
		}

		if( Y>480 ) {
			Inc(ActFlg[KACTION_S2B2_FALL]);
			ActCount[KACTION_S2B2_FALL]  =  -1;
			FOwner->Quake(3,60);
		}
		   }break;
		//移動
	case 3:{
		//振動
		if( Age % 2 == 0 )
			Sprite[BBody]->MoveR(0,-2);
		else
			Sprite[BBody]->MoveR(0,2);

		//炎上
		FOwner->Add(new TMotherShipSmoke(FOwner,X,Y,-18,-0.2, 4, 2));
		FOwner->Add(new TMotherShipSmoke(FOwner,X-32,Y-16,-15,-0.2, 4,2));
		FOwner->Add(new TMotherShipFire(FOwner,X,Y,-18,-0.2, 4, 2.0));

		//水しぶき発生
		if( (Y>400) && (Age % 2 == 0) ) {
			FOwner->Add(new TSplash(FOwner, X+200,4));
		}

		//バウンド
		f32 YSpeed = -5.f+(f32)(ActCount[KACTION_S2B2_FALL])*0.05f;
		Sprite[BBody]->MoveR(-1,YSpeed);

		Sprite[BBody]->RotateR(16);

		if( (ActCount[KACTION_S2B2_FALL] > 30) && (Y>480) ) {
			Inc(ActFlg[KACTION_S2B2_FALL]);
			ActCount[KACTION_S2B2_FALL]  =  -1;
			FOwner->Quake(3,120);
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WABossExp,0);
			FOwner->Add(new TBoxContrl(FOwner,3));
		}
		   }break;
	case 4:{
		//水しぶき発生
		if( (Age % 2 == 0) ) {
			FOwner->Add(new TSplash(FOwner, X+200,4));
		}

		Sprite[BBody]->MoveR(5,5);

		if( (ActCount[KACTION_S2B2_FALL] > 60) ) {
			Die();
			FOwner->BDieTime = FOwner->GetTick();
		}
		   }
	}
	Inc(ActCount[KACTION_S2B2_FALL]);

	//死にイベント早送り用
	if( ! ButtonON[0] ) ButtonFlg[0] = false;

	if( (! ButtonFlg[0]) && ButtonON[0] && (! FOwner->ResultExists) && (FOwner->IsBossDefeated(1)) ) {
		FOwner->Add(new TResult2(FOwner));
		ButtonFlg[0] = true;
		FOwner->ResultSkipped = true;
	}

}

void TStage2Boss2::CheckServantDead()
{
	//発狂モード判定
	if( (ServantDead >= 2) && (ActFlg[KACTION_S2B2_BEAM] == 0) ) {
		ActFlg[KACTION_S2B2_BEAM] = 1;
		ActFlg[KACTION_S2B2_APPEAR] = 0;
		ActFlg[KACTION_S2B2_ESCAPE] = 0;
	}

	if( (ActFlg[KACTION_S2B2_BEAM] > 0) && (ActFlg[KACTION_S2B2_ESCAPE] > 0) )
		ActFlg[KACTION_S2B2_ESCAPE] = 0;
}

void TStage2Boss2::StartingDemo()
{
	Sprite[BBody]->MoveR(5.3f, 0);

	Sprite[0]->MoveR(-FOwner->Speed+KSCROLLSPEED_NORMAL, 0);

	if( X>700 ) Die();

}

void TStage2Boss2::WarpMotion()
{
	switch( ActFlg[KACTION_S2B2_WARP] ){
	case 1:{
		FOwner->Add(new TWarp2D(FOwner,600,240,1));

		ActFlg[KACTION_S2B2_TRANSFER] = 1;
		DestX = 400;
		DestY = 240;

		Inc(ActFlg[KACTION_S2B2_WARP]);
		ActCount[KACTION_S2B2_WARP]  =  -1;
		   }break;
	case 2:{
		Transfer(DestX,DestY,16);
		if( ActFlg[KACTION_S2B2_TRANSFER] == 0 ) {
			Inc(ActFlg[KACTION_S2B2_WARP]);
			ActCount[KACTION_S2B2_WARP]  =  -1;
		}
		   }break;
	case 3:{
		if( FOwner->LimitTime <= 250 ) {
			ActFlg[KACTION_S2B2_TRANSFER] = 1;
			DestX = 800;
			DestY = 240;

			for( u32 i  =  0 ; i < numSpr ; i++) {
				//あたり判定を消す
				Sprite[i]->GetSpr()->FHit = false;
			}

			HPGauge->Hide();

			FKind = kEffect2;   //ロック対象から外す

			Inc(ActFlg[KACTION_S2B2_WARP]);
			ActCount[KACTION_S2B2_WARP]  =  -1;
		}
		   }break;
	case 4:{
		Transfer(DestX,DestY, 48);
		if( X>=600 ) {
			for( u32 i  =  0 ; i < numSpr; i++) {
				Sprite[i]->GetSpr()->FVisible = false;
			}
			Inc(ActFlg[KACTION_S2B2_WARP]);
			ActCount[KACTION_S2B2_WARP]  =  -1;
		}
		   }break;
	case 5:{
		if( ActCount[KACTION_S2B2_WARP]>=20 ) {
			Die();
			FOwner->TimeOver();
		}     
		   }break;
	}
	Inc(ActCount[KACTION_S2B2_WARP]);

}

void TStage2Boss2::KeepPosition( s32 _x, s32 _y )
{
	//スクロールスピードが10以上、または現在位置より100ずれたら移動する
	if( ((FOwner->Speed>10) || (abs(X-_x)>100))
		&& (ActFlg[KACTION_S2B2_TRANSFER] == 0) ) {
			ActFlg[KACTION_S2B2_TRANSFER] = 1;
			ActCount[KACTION_S2B2_TRANSFER] = 0;
	}
	Transfer(_x, _y, 32);

}

void TStage2Boss2::SlideTail()
{
	for( u32 i = 0; i <= 7; i++ )
		TailBeam[i]->Place(X,Y);

}

TStage2Boss2::TStage2Boss2( TOBJList* owner, s32 _x, s32 _y ) : TOBJ(owner)
{
	FObjectName = "TStage2Boss2";

	X = _x;
	Y = _y;

	switch( FOwner->Difficulty ){
	case KDIFFICULTY_EASY: 
		{
			MaxHP = 1000;
			FDef = 20;
			FSTR = 1000;
		}break;
	case KDIFFICULTY_NORMAL: 
		{
			MaxHP = BossMaxHP / 2;
			FDef = 20;
			FSTR = 1000;
		}break;
	default: 
		{
			MaxHP = BossMaxHP / 2;
			FDef = 20;
			FSTR = 100;
		}
	}

	HP = MaxHP;
	TempHP = HP;

	FKind = kEnemy;
	Score = 40000;
	Buzzed = false;
	LastBuzz = 0;
	GaugeTime = 0;
	TotalFlight = 0;
	for( u32 i = 0; i <= 99; i++ ) {
		ActFlg[i] = 0;
		ActCount[i] = 0;
	}
	LastFlash = 0;
	EnemyDeg = 0;
	BSpeed = 0;
	IniDis = 0;
	DestDeg = 0;
	ShotCount = 0;
	ServantDead = 0;

	s32 OriginZ = 13000;
	OriginColor = 255;

	//IDの付与
	FID = NewID;
	Inc(NewID);

	MakeSprFromID( FOwner->GetDG(), KSPRDATA_STAGE2BOSS2, MechaTex, OriginZ, ExtraEffects);

	HPGauge  =  new TBossGauge2(FOwner,this,-128,32);
	FOwner->Add(HPGauge);


	for( u32 i = 0; i <= 7; i++ ) {
		TailBeam[i] = new TBoss2BeamCore(FOwner);
		FOwner->Add(TailBeam[i]);
	}

	//護衛を生成
	FOwner->Add(new TStage2Guard(FOwner,X-100,150, this));
	FOwner->Add(new TStage2Guard(FOwner,X-100,350, this));

	if( FOwner->DemoON )
		ActFlg[KACTION_S2B2_STARTDEMO] = 1;
	else
		ActFlg[KACTION_S2B2_APPEAR] = 1;

}

TStage2Boss2::~TStage2Boss2()
{
	for( u32 i = 0; i <= 7; i++ )
		TailBeam[i]->Die();

}

void TStage2Boss2::Move()
{
	TOBJ::Move();

	X = RoundOff(Sprite[BBody]->GetSpr()->FCX);
	Y = RoundOff(Sprite[BBody]->GetSpr()->FCY);

	Accel();

	if( HP>0 ) {
		if( ActFlg[KACTION_S2B2_WARP] == 0 ) {
			if( ActFlg[KACTION_S2B2_APPEAR]>0 ) AppearEvent();
			if( ActFlg[KACTION_S2B2_BEAM]>0 ) BeamMotion();
			if( ActFlg[KACTION_S2B2_ESCAPE]>0 ) EscapeMotion();
			if( ActFlg[KACTION_S2B2_STARTDEMO]>0 ) StartingDemo();
		}
		else WarpMotion();

		if( (FOwner->LimitTime <= 3000) && (ActFlg[KACTION_S2B2_WARP] == 0) ) ActFlg[KACTION_S2B2_WARP] = 1;

	}

	if( (Age-LastBuzz)>=40 ) Buzzed = false;
	if( (Age-GaugeTime)>120 ) HPGauge->Hide();
	if( (Age-LastFlash == 2) ) {
		for( u32 i = 0 ; i < BConstForeground; i++ )
			Sprite[i]->GetSpr()->SetColor(OriginColor,ctRed);          //色を戻す

		Sprite[BBody]->MoveR(-1,0);                  //のけぞり戻し
	}

	//死亡判定
	if( (HP<=0) && (ActFlg[KACTION_S2B2_FALL] == 0) )
		ActFlg[KACTION_S2B2_FALL] = 1;

	if( ActFlg[KACTION_S2B2_FALL]>0 ) Fall();

	if(FOwner->Distance>69000 )  FOwner->Distance = 60000;
}

void TStage2Boss2::OnDamaged( TOBJ* target )
{
	X = RoundOff(Sprite[BBody]->GetSpr()->FCX);
	Y = RoundOff(Sprite[BBody]->GetSpr()->FCY);
	if( target->Kind() == kHero ) {
		if( (! Buzzed) && (target->Hit[HitBuzz]) ) {
			FOwner->Scrach(this);
			Buzzed = true;
			LastBuzz = Age;
		}
		if( (target->Hit[HitBlade]) && (target->LastSlashed > LastSlashed) ) {
			if( target->STR() > FDef )
				Dec(HP,target->STR()-FDef);
			else
				Dec(HP,target->STR());
			for( u32 i = 0 ; i < BConstForeground; i++ )
				Sprite[i]->GetSpr()->SetColor(0,ctRed);
			LastFlash = Age;
			HPGauge->Show();
			GaugeTime = Age;
			Sprite[BBody]->MoveR(1,0);                  //のけぞる
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WAEdamage0,7);
			LastSlashed = target->LastSlashed;
		}
	}

	if( (target->Kind() == kHShot) ) {
		Dec(HP,target->STR());
		for( u32 i = 0 ; i < BConstForeground; i++ )
			Sprite[i]->GetSpr()->SetColor(0,ctRed);
		if( CreateSE ) FOwner->SoundEffect->WavPlay(WAEdamage0,7);

		LastFlash = Age;
		Sprite[BBody]->MoveR(1,0);                  //のけぞる
		HPGauge->Show();
		GaugeTime = Age;
	}

	if( (target->Kind() == kMissile) ) {
		Dec(HP,target->STR()-FDef);
		for( u32 i = 0 ; i < BConstForeground; i++ )
			Sprite[i]->GetSpr()->SetColor(0,ctRed);
		LastFlash = Age;
		Sprite[BBody]->MoveR(1,0);                  //のけぞる
		HPGauge->Show();
		GaugeTime = Age;
		if( HP<=0 ) BonusRate = target->BonusRate;
	}

	if( HP<TempHP ) TempHP = HP;
	TOBJ::OnDamaged(target);
}

void TStage2Boss2::Die()
{
	TOBJ::Die();
	HPGauge->Die();

}

void TStage2Boss2::OnBroken()
{
	TOBJ::OnBroken();
	FOwner->Add(new TExplarge(FOwner,X,Y,0,true));//爆発

}

void TStage2Boss2::TellImDead()
{
	Inc(ServantDead);

}

TStage2GuardBWS::TStage2GuardBWS( TOBJList* owner, s32 _x, s32 _y ) : TOBJ(owner)
{
	FObjectName = "TStage2GuardBWS";
	X = _x;
	Y = _y;
	FKind = kEffect2;
	for( u32 i = 0; i <= 2; i++ ) {
		Purged[i] = false;
		YSpeed[i] = -random(5)-3;
	}


	MakeSprFromID( FOwner->GetDG(), KSPRDATA_S2GUARDBWS, MechaTex, 15000, ExtraEffects);

	Sprite[0]->Move(X+65,Y+5);
	Sprite[0]->Sticky = true;

	Sprite[1]->Move(X+32,Y+15);
	Sprite[1]->Rotate(1024);
	Sprite[1]->Sticky = true;

	Sprite[2]->Move(X+32,Y+15);
	Sprite[2]->Rotate(1024);
	Sprite[2]->Sticky = true;
	Sprite[2]->GetSpr()->SetColor(0,ctRed);
	Sprite[2]->GetSpr()->SetColor(0,ctGreen);
	Sprite[2]->GetSpr()->SetColor(0,ctBlue);
}

TStage2GuardBWS::~TStage2GuardBWS()
{

}

void TStage2GuardBWS::Move()
{
	TOBJ::Move();
	for( u32 i = 0; i <= 2; i++ ) {
		if( Purged[i] )  {
			Sprite[i]->MoveR(-30,YSpeed[i]);
			YSpeed[i] = YSpeed[i]+0.2;
			Sprite[i]->RotateR(-100);
		}
	}

	if( (! FOwner->DemoON) && (Sprite[2]->GetSpr()->FCX<-100) ) Die();

}

void TStage2GuardBWS::Die()
{
	TOBJ::Die();

}

void TStage2GuardBWS::SetOwnerSprite( TJoint* _spr )
{
	_spr->Add(Sprite[0]);
	_spr->Add(Sprite[1]);
	_spr->Add(Sprite[2]);
	OwnerSprite = _spr;

}

void TStage2GuardBWS::Purge( s32 _id )
{
	Purged[_id] = true;

	if( OwnerSprite!=NULL ) {
		OwnerSprite->Delete(Sprite[0]);       //親スプライトの多関節リストから自分を削除
	}

}

void TStage2Guard::AppearEvent()
{
	switch( ActFlg[KACTION_S2GR_APPEAR] ){
		//初期化
	case 1:{
		Inc(ActFlg[KACTION_S2GR_APPEAR]);
		ActCount[KACTION_S2GR_APPEAR]  =  -1;
		DestX = 400;
		DestY = Y;
		ActFlg[KACTION_S2GR_TRANSFER] = 1;
		   }break;
		//
	case 2:{
		Transfer_NoMotion(DestX,DestY,16);
		if( ActCount[KACTION_S2GR_APPEAR] == 120 ) {
			Inc(ActFlg[KACTION_S2GR_APPEAR]);
			ActCount[KACTION_S2GR_APPEAR]  =  -1;
		}
		   }break;
	case 3:{
		MotionControl(KMOTION_STAGE2GUARD_APPEAR2);

		if( ActCount[KACTION_S2GR_APPEAR] == 20 ) {
			Bws->Purge(0);

		}

		if( ActCount[KACTION_S2GR_APPEAR] == 40 ) {
			Inc(ActFlg[KACTION_S2GR_APPEAR]);
			ActCount[KACTION_S2GR_APPEAR]  =  -1;
			ActFlg[KACTION_S2GR_TRANSFER] = 1;
			DestX = FOwner->GetRand(3)*50 + 450;
			DestY = FOwner->GetRand(4)*25 + Y-50;
		}
		   }break;
	case 4:{
		Transfer(DestX,DestY,32);

		if( ActCount[KACTION_S2GR_APPEAR] == 0 ) {
			Bws->Purge(1);
		}

		if( ActCount[KACTION_S2GR_APPEAR] == 10 ) {
			Bws->Purge(2);
			BWSPurged = true;
		}

		if( ActFlg[KACTION_S2GR_TRANSFER] == 0 ) {
			SetNextState(KACTION_S2GR_APPEAR);
		}
		   }
	}
	Inc(ActCount[KACTION_S2GR_APPEAR]);

}

void TStage2Guard::Transfer( s32 _x, s32 _y, s32 _speed )
{
	switch( ActFlg[KACTION_S2GR_TRANSFER] ){
	case 2:{    //移動（加速ポーズ）
		if( ActCount[KACTION_S2GR_TRANSFER] <= 1500 ) {
			if( (0 <= DestDeg && DestDeg <= 1024) || (3072 <= DestDeg && DestDeg <= 4095) ){
				MotionControl(KMOTION_STAGE2GUARD_BACK);
			}
			else if( 1025 <= DestDeg && DestDeg <= 3071){
				MotionControl(KMOTION_STAGE2GUARD_FOWARD);
			}
		}
		else {
			MotionControl(KMOTION_STAGE2GUARD_DEFAULT);
		}
		   }
	}
	Transfer_NoMotion(_x, _y, _speed);

}

void TStage2Guard::Transfer_NoMotion( s32 _x, s32 _y, s32 _speed )
{
	s32 angularVel = _speed;
	s32 TransSpeed = 0;
	s32 currentRad =  ActCount[KACTION_S2GR_TRANSFER];
	s32 prevRad =  ActCount[KACTION_S2GR_TRANSFER]-angularVel;
	if( prevRad>0 ) TransSpeed = -RoundOff(IniDis/2.f*(cost[currentRad]-cost[prevRad]));

	switch( ActFlg[KACTION_S2GR_TRANSFER] ){
	case 1:{    //位置決定
		s32 DX = _x-X;
		s32 DY = _y-Y;
		IniDis = RoundOff(sqrt(DX*DX+DY*DY));
		DestDeg = RoundOff(atan2(DY,DX)/(f32)M_PI*2048.f) & 0x0fff;
		Inc(ActFlg[KACTION_S2GR_TRANSFER]);
		ActCount[KACTION_S2GR_TRANSFER] = 0;
		   }break;
	case 2:{    //移動
		Sprite[BHip]->MoveR(TransSpeed * cost[DestDeg], TransSpeed * sint[DestDeg]);

		if( ActCount[KACTION_S2GR_TRANSFER] >= 2048 ) {
			ActFlg[KACTION_S2GR_TRANSFER] = 0;
			ActCount[KACTION_S2GR_TRANSFER] = 0;
		}
		   }break;
	}
	Inc(ActCount[KACTION_S2GR_TRANSFER], angularVel);


}

void TStage2Guard::Accel()
{
	if( (FOwner->Speed>=10) )
		BSpeed = BSpeed+0.4f;
	else if( BSpeed>0 )
		BSpeed = BSpeed-0.4f;

	if( BSpeed>6 )
		BSpeed = 6;

	if( BSpeed<0 )
		BSpeed = 0;

	Sprite[BHip]->MoveR(BSpeed,0);

}

void TStage2Guard::MakeShot( s32 _pat )
{
	s32 GunR = (Sprite[BRArm]->Rdegree) & 0xfff;
	f32 GX = Sprite[BRArm]->GetSpr()->FCX -70;
	f32 GY = Sprite[BRArm]->GetSpr()->FCY -11;
	PosRoll( GX, GY,Sprite[BRArm]->GetSpr()->FCX,Sprite[BRArm]->GetSpr()->FCY,GunR);

	FOwner->Add(new TZ01Shot3(FOwner,(s32)(GX),(s32)(GY),GunR+2048,30, 500,BSpeed,4));

	FOwner->Add(new TShotFireLarge(FOwner,(s32)(GX),(s32)(GY),1));

	if( CreateSE ) FOwner->SoundEffect->WavPlay(WABShot,9);

}

void TStage2Guard::BladeMotion()
{
	switch( ActFlg[KACTION_S2GR_BLADE] ){
		//初期化
	case 1:{
		Inc(ActFlg[KACTION_S2GR_BLADE]);
		ActCount[KACTION_S2GR_BLADE]  =  -1;
		SetNextPos();
		   }break;
		//位置につく
	case 2:{
		Transfer(DestX,DestY,TransferSpeed);

		if( ActCount[KACTION_S2GR_TRANSFER] > 1000 ) {
			Inc(ActFlg[KACTION_S2GR_BLADE]);
			ActCount[KACTION_S2GR_BLADE]  =  -1;
		}
		   }break;
		//振りかぶり
	case 3:{
		MotionControl(KMOTION_STAGE2GUARD_SLASH1);
		Transfer_NoMotion(DestX,DestY,TransferSpeed);
		if( ActFlg[KACTION_S2GR_TRANSFER] == 0 ) {
			Inc(ActFlg[KACTION_S2GR_BLADE]);
			ActCount[KACTION_S2GR_BLADE]  =  -1;
			ActFlg[KACTION_S2GR_TRANSFER] = 1;
			if( FOwner->Difficulty=KDIFFICULTY_HARD )
				DestX = FOwner->HeroPos.X + 48;
			else
				DestX = FOwner->HeroPos.X + 128;
			DestY = FOwner->HeroPos.Y;
		}
		   }break;
		//接近
	case 4:{
		Transfer_NoMotion(DestX,DestY,48);

		if( ActCount[KACTION_S2GR_TRANSFER] > 1000 ) {
			Inc(ActFlg[KACTION_S2GR_BLADE]);
			ActCount[KACTION_S2GR_BLADE]  =  -1;

			Sprite[BBlade]->GetSpr()->FHit = true;
			BladeTrace = new TEnemyBladeTrace2(FOwner,Sprite[BBlade],160,192,-1024);
			FOwner->Add(BladeTrace);
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WASwing_E, 8);
		}
		   }break;
		//振り下ろし
	case 5:{
		Transfer_NoMotion(DestX,DestY,48);

		if( ActCount[KACTION_S2GR_BLADE] < 5 ) {
			MotionControl(KMOTION_STAGE2GUARD_SLASH1TO2);
		}
		else {
			MotionControl(KMOTION_STAGE2GUARD_SLASH2);
		}

		if( ActFlg[KACTION_S2GR_TRANSFER] == 0 ) {
			Inc(ActFlg[KACTION_S2GR_BLADE]);
			ActCount[KACTION_S2GR_BLADE]  =  -1;
			Sprite[BBlade]->GetSpr()->FHit = false;
		}
		   }break;
		//余韻
	case 6:{
		MotionControl(KMOTION_STAGE2GUARD_SLASH2);

		if( ActCount[KACTION_S2GR_BLADE] == 10 ) {
			BladeTrace->Die();
			BladeTrace = NULL;
			SetNextState(KACTION_S2GR_BLADE);
		}
		   }break;
	}
	Inc(ActCount[KACTION_S2GR_BLADE]);

}

void TStage2Guard::ShotMotion()
{
	switch( ActFlg[KACTION_S2GR_BEAM] ){
		//初期化
	case 1:{
		Inc(ActFlg[KACTION_S2GR_BEAM]);
		ActCount[KACTION_S2GR_BEAM]  =  -1;
		SetNextPos();
		   }break;
		//
	case 2:{
		Transfer(DestX,DestY,TransferSpeed);

		if( ActCount[KACTION_S2GR_TRANSFER] > 1000 ) {
			Inc(ActFlg[KACTION_S2GR_BEAM]);
			ActCount[KACTION_S2GR_BEAM]  =  -1;
		}
		   }break;
	case 3:{
		Search();
		MotionControl(KMOTION_STAGE2GUARD_SHOT);
		Transfer_NoMotion(DestX,DestY,TransferSpeed);
		if( ActFlg[KACTION_S2GR_TRANSFER] == 0 ) {
			Inc(ActFlg[KACTION_S2GR_BEAM]);
			ActCount[KACTION_S2GR_BEAM]  =  -1;
		}
		   }break;
	case 4:{
		Search();
		MotionControl(KMOTION_STAGE2GUARD_SHOT);
		if( ActCount[KACTION_S2GR_BEAM] % 5 == 0 ) MakeShot(0);
		if( ActCount[KACTION_S2GR_BEAM] >20 ) {
			SetNextState(KACTION_S2GR_BEAM);
		}
		   }break;
	}
	Inc(ActCount[KACTION_S2GR_BEAM]);

}

void TStage2Guard::SetNextState( s32 _current )
{
	ActFlg[_current] = 0;
	ActCount[_current] = -1;

	switch( _current ){
	case KACTION_S2GR_APPEAR   : 
		ActFlg[KACTION_S2GR_BEAM] = 1;
		break;
	case KACTION_S2GR_BEAM     : 
		{
			Inc(ShotCount);

			if( ShotCount<FOwner->GetRand(5) ) {
				ActFlg[KACTION_S2GR_BEAM] = 1;
			}
			else {
				ActFlg[KACTION_S2GR_BLADE] = 1;
				ShotCount = 0;
			}
		}break;
	case KACTION_S2GR_BLADE     : 
		ActFlg[KACTION_S2GR_BEAM] = 1;
		break;
	}

	if( FOwner->BossDie ) {
		ActFlg[KACTION_S2GR_RETREAT] = 1;
		ActFlg[KACTION_S2GR_BEAM] = 0;
		ActFlg[KACTION_S2GR_BLADE] = 0;
	}

}

void TStage2Guard::Search()
{
	f32 GX = Sprite[BRArm]->GetSpr()->FCX -71;
	f32 GY = Sprite[BRArm]->GetSpr()->FCY -11;
	PosRoll(GX,GY,Sprite[BRArm]->GetSpr()->FCX,Sprite[BRArm]->GetSpr()->FCY,Sprite[BRArm]->Rdegree);

	s32 DX = RoundOff(FOwner->HeroPos.X-GX);
	s32 DY = RoundOff(FOwner->HeroPos.Y-GY);
	s32 GunR = (RoundOff(atan2(DY,DX)/(f32)M_PI*2048.f) + 2048) & 0xfff;

	if( (GunR <= 3072) && (GunR >= 1024) ) GunR  =  0;

	FOwner->MotionDataList[KMOTION_STAGE2GUARD_SHOT].Sprites[BRArm].Rdeg  =  GunR;
}

void TStage2Guard::RetreatMotion()
{
	switch( ActFlg[KACTION_S2GR_RETREAT] ){
		//初期化
	case 1:{
		Inc(ActFlg[KACTION_S2GR_RETREAT]);
		ActCount[KACTION_S2GR_RETREAT]  =  -1;
		   }break;
		//
	case 2:{
		MotionControl(KMOTION_STAGE2GUARD_DEFAULT);
		if( ActCount[KACTION_S2GR_RETREAT] == 80 ) {
			Inc(ActFlg[KACTION_S2GR_RETREAT]);
			ActCount[KACTION_S2GR_RETREAT]  =  -1;

			ActFlg[KACTION_S2GR_TRANSFER] = 1;
			DestX = 700;
			DestY = -300;
		}
		   }break;
	case 3:{
		Transfer(DestX,DestY,24);
		if( ActCount[KACTION_S2GR_TRANSFER] == 0 ) {
			Die();
		}
		   }break;
	}
	Inc(ActCount[KACTION_S2GR_RETREAT]);

}

void TStage2Guard::StartingDemo()
{
	Sprite[BHip]->MoveR(5.3f,0);

	Sprite[0]->MoveR(-FOwner->Speed+KSCROLLSPEED_NORMAL,0);

	if( X>700 ) Die();

}

void TStage2Guard::SetNextPos()
{
	ActFlg[KACTION_S2GR_TRANSFER] = 1;
	DestX = FOwner->GetRand(3)*50 + 400;

	if( Pattern == 0 )
		DestY = FOwner->GetRand(4)*50 + 50;
	else
		DestY = FOwner->GetRand(4)*50 + 230;

}

TStage2Guard::TStage2Guard( TOBJList* owner, s32 _x,s32 _y, TStage2Boss2* _master ) : TOBJ(owner)
{
	if( _master )
	{
		FObjectName = "TStage2Guard";

		X = _x;
		Y = _y;
		switch( FOwner->Difficulty ){
		case KDIFFICULTY_EASY:
		case KDIFFICULTY_NORMAL:
			MaxHP = KSTR_SHOT*48;
			break;
		default:
			MaxHP = KSTR_SHOT*96;
		}

		if( FOwner->Difficulty == KDIFFICULTY_HARD )
			TransferSpeed = 48;
		else
			TransferSpeed = 20;
		HP = MaxHP;
		TempHP = HP;
		FSTR = 100;
		FKind = kEnemy;
		Score = 50000;
		ComBonus = 5000;
		Buzzed = false;
		LastBuzz = 0;
		GaugeTime = 0;
		TotalFlight = 0;
		for( u32 i = 0; i <= 99; i++ ) {
			ActFlg[i] = 0;
			ActCount[i] = 0;
		}
		LastFlash = 0;
		EnemyDeg = 0;
		BSpeed = 0;
		IniDis = 0;
		DestDeg = 0;
		ShotCount = 0;
		Master = _master;

		if( _y<240 )
			Pattern  =  0;
		else
			Pattern  =  1;

		s32 OriginZ = 16000;
		OriginColor = 255;

		BladeTrace = NULL;

		BWSPurged = false;

		//IDの付与
		FID = NewID;
		Inc(NewID);

		MakeSprFromID( FOwner->GetDG(), KSPRDATA_STAGE2GUARD, MechaTex, OriginZ, ExtraEffects);

		Sprite[BBlade]->GetSpr()->FHit = false;

		HPGauge  =  new TEnemyGauge2(FOwner,this,-128,32);
		FOwner->Add(HPGauge);

		MotionControlSoon(KMOTION_STAGE2GUARD_APPEAR1);

		Bws = new TStage2GuardBWS(FOwner,X,Y);
		FOwner->Add(Bws);

		Bws->SetOwnerSprite(Sprite[BBody]);

		if( ! FOwner->DemoON )
			ActFlg[KACTION_S2GR_APPEAR] = 1;
		else
			ActFlg[KACTION_S2GR_STARTDEMO] =  1;

	}

}

TStage2Guard::~TStage2Guard()
{
	if( BladeTrace!=NULL )
		BladeTrace->Die();

}

void TStage2Guard::Move()
{
	TOBJ::Move();
	X = RoundOff(Sprite[BHip]->GetSpr()->FCX);
	Y = RoundOff(Sprite[BHip]->GetSpr()->FCY);
	if( HP>0 ) {
		if( ActFlg[KACTION_S2GR_APPEAR]>0 ) AppearEvent();
		if( ActFlg[KACTION_S2GR_BEAM]>0 ) ShotMotion();
		if( ActFlg[KACTION_S2GR_BLADE]>0 ) BladeMotion();
		if( ActFlg[KACTION_S2GR_RETREAT]>0 ) RetreatMotion();
		if( ActFlg[KACTION_S2GR_STARTDEMO]>0 ) StartingDemo();

		Accel();
	}

	if( (Age-LastBuzz)>=40 ) Buzzed = false;
	if( (Age-GaugeTime)>120 ) HPGauge->Hide();
	if( (Age-LastFlash == 2) ) {
		for( u32 i = 0 ; i < BConstForeground; i++ )
			Sprite[i]->GetSpr()->SetColor(OriginColor,ctRed);          //色を戻す

		Sprite[BHip]->MoveR(-1,0);                  //のけぞり戻し
	}

	if( HP<=0 ) {
		OnBroken();                        //HPが無くなったら死ぬ
		Master->TellImDead();    //自分が死んだことをボスに報告
	}

}

void TStage2Guard::OnDamaged( TOBJ* target )
{
	X = RoundOff(Sprite[BHip]->GetSpr()->FCX);
	Y = RoundOff(Sprite[BHip]->GetSpr()->FCY);
	if( target->Kind() == kHero ) {
		if( (! Buzzed) && (target->Hit[HitBuzz]) ) {
			FOwner->Scrach(this);
			Buzzed = true;
			LastBuzz = Age;
		}
		if( (target->Hit[HitBlade]) && (target->LastSlashed > LastSlashed) ) {
			Dec(HP,target->STR());
			for( u32 i = 0 ; i < BConstForeground; i++ )
				Sprite[i]->GetSpr()->SetColor(0,ctRed);
			LastFlash = Age;
			HPGauge->Show();
			GaugeTime = Age;
			Sprite[BHip]->MoveR(1,0);                  //のけぞる
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WAEdamage0,7);
			LastSlashed = target->LastSlashed;
		}
	}

	if( (target->Kind() == kHShot) || (target->Kind() == kMissile) ) {
		Dec(HP,target->STR());
		for( u32 i = 0 ; i < BConstForeground; i++ )
			Sprite[i]->GetSpr()->SetColor(0,ctRed);
		if( CreateSE ) FOwner->SoundEffect->WavPlay(WAEdamage0,7);

		LastFlash = Age;
		Sprite[BHip]->MoveR(1,0);                  //のけぞる
		HPGauge->Show();
		GaugeTime = Age;
		if( HP<=0 ) BonusRate = target->BonusRate;
	}

	if( HP<TempHP ) TempHP = HP;
	TOBJ::OnDamaged(target);

}

void TStage2Guard::Die()
{
	TOBJ::Die();
	HPGauge->Die();
	if( ! BWSPurged )
		Bws->Die();

}

void TStage2Guard::OnBroken()
{
	TOBJ::OnBroken();
	FOwner->Add(new TExplarge(FOwner,X,Y,0,true));//爆発
}

TStage3BossBit::TStage3BossBit( TOBJList* owner, s32 _x, s32 _y ) : TOBJ(owner)
{
	FObjectName = "TStage3BossBit";

	X = _x;
	Y = _y;
	MaxHP = 0;
	HP = MaxHP;
	TempHP = HP;
	FSTR = 100;
	FKind = kColBG;
	Score = 0;
	for( u32 i = 0; i <= 99; i++ ) {
		ActFlg[i] = 0;
		ActCount[i] = 0;
	}
	EnemyDeg = 0;
	IniDis = 0;
	DestDeg = 0;
	ShotCount = 0;

	MakeSpriteLite( numSpr, X,Y,48,32,16500,bmNormal);
	Sprite[numSpr-1]->GetSpr()->AddTexture(MechaTex, 96, 384, 143, 415);
	Sprite[numSpr-1]->GetSpr()->FCX += 24;
	Sprite[numSpr-1]->GetSpr()->FCY += 16;
	Sprite[numSpr-1]->GetSpr()->Move(X,Y);

	if( ExtraEffects ) {
		MakeSpriteLite( numSpr, X,Y,48,32,16000,bmAdd);
		Sprite[numSpr-1]->GetSpr()->AddTexture(MechaTex, 96, 384, 143, 415);
		Sprite[numSpr-1]->GetSpr()->FCX += 24;
		Sprite[numSpr-1]->GetSpr()->FCY += 16;
		Sprite[numSpr-1]->GetSpr()->Move(X,Y);
		Sprite[numSpr-1]->GetSpr()->SetColor(160,ctAlpha);
		Sprite[numSpr-1]->GetSpr()->FEffectNum = 1;
		Sprite[numSpr-1]->GetSpr()->FBumpSize = 2.9f;
		Sprite[numSpr-1]->GetSpr()->FBumpTex = MechaTex;
		Sprite[numSpr-1]->GetSpr()->FBumpLevel = 2.0f;

		Sprite[0]->Add( Sprite[numSpr-1] );
	}

	//バックファイア
	MakeSpriteLite(numSpr, X,Y,48,48,20000,bmAdd);
	Sprite[numSpr-1]->GetSpr()->AddTexture(MechaTex, 224, 256, 287, 319);
	Sprite[numSpr-1]->GetSpr()->FCX += 24;
	Sprite[numSpr-1]->GetSpr()->FCY += 24;
	Sprite[numSpr-1]->GetSpr()->Move(X+16,Y);
	Sprite[0]->Add(Sprite[numSpr-1]);

	//目くらまし光球
	MakeSpriteLite(numSpr, X,Y,48,48,50000,bmAdd);
	Sprite[numSpr-1]->GetSpr()->AddTexture(MechaTex, 224, 256, 287, 319);
	Sprite[numSpr-1]->GetSpr()->FCX += 24;
	Sprite[numSpr-1]->GetSpr()->FCY += 24;
	Sprite[numSpr-1]->GetSpr()->Move(X,Y);
	Sprite[numSpr-1]->GetSpr()->FVisible = false;

	Sprite[0]->SetDest(X,Y,2);

	Sprite[numSpr-1]->Move(X,Y);
	Sprite[0]->Add(Sprite[numSpr-1]);

	BitLight  =  new TBitLight(FOwner, this);
	FOwner->Add(BitLight);

}

void TStage3BossBit::Move()
{
	TOBJ::Move();
	X = RoundOff(Sprite[0]->GetSpr()->FCX);
	Y = RoundOff(Sprite[0]->GetSpr()->FCY);
	Sprite[0]->AutoSlide();

	Sprite[0]->MoveR(FOwner->Speed - KSCROLLSPEED_NORMAL,0);

	if( ActFlg[KACTION_SELFDEST]>0  )  SelfDestMotion();
	if( ActFlg[KACTION_LIGHTNING]>0  )  LightningMotion();
	if( ActFlg[KACTION_BEAM]>0  )  BeamMotion();
	if( ActFlg[KACTION_HOMING]>0  )  homingMotion();

}

void TStage3BossBit::SetNextState( s32 _current )
{

}

void TStage3BossBit::Place( f32 _x, f32 _y, f32 _speed )
{
	Sprite[0]->SetDest(RoundOff(_x),RoundOff(_y),RoundOff(_speed));

}

void TStage3BossBit::MakeShot( s32 _pat )
{
	s32 GX = RoundOff(Sprite[0]->GetSpr()->FCX);
	s32 GY = RoundOff(Sprite[0]->GetSpr()->FCY);

	s32 GunR = (Sprite[0]->GetSpr()->FR + 2048) & 0xfff;
	switch( _pat ){
	case -1:{
		ActFlg[KACTION_BEAM] = 0;
		ActCount[KACTION_BEAM] = -1;
			}break;
	case 0:{
		FOwner->Add(new TZ01Shot3(FOwner,GX,GY,GunR,20,1000,FOwner->Speed-KSCROLLSPEED_NORMAL,0.6f));
		if( FOwner->Difficulty=KDIFFICULTY_HARD ) {
			for( u32 i = 1; i <= 5; i++ )
				FOwner->Add(new TZ01Shot3(FOwner,GX,GY,GunR+i*683,20,1000,FOwner->Speed-KSCROLLSPEED_NORMAL,0.6f));
		}

		FOwner->Add(new TFCircle(FOwner,GX,GY,1,GunR,0));
		   }break;
	case 1:{
		FOwner->Add(new TZ01Shot3(FOwner,GX,GY,GunR,2,500,FOwner->Speed-KSCROLLSPEED_NORMAL,1));
		FOwner->Add(new TFCircle(FOwner,GX,GY,1,GunR,0));
		Inc(ShotCount);
		   }break;
	case 2:{
		if( EffLevel>0 ) {
			for( u32 i = 0; i <= 20; i++ )
				FOwner->Add(new TParticle2(FOwner,GX,GY,1,GunR+random(768*2)-768,random(15)+5,random(5)+1));
		}
		FOwner->Add(new TFunBeamCore(FOwner,GX,GY,GunR,1));
		FOwner->Add(new TMissileFlash(FOwner,GX,GY));
		   }break;
	case 3:{
		ActFlg[KACTION_BEAM] = 1;
		ActCount[KACTION_BEAM] = -1;
		   }break;
	case 4:{
		ActFlg[KACTION_HOMING] = 1;
		ActCount[KACTION_HOMING] = -1;
		   }break;
	case 5:{
		FOwner->Add(new TSlowBeam(FOwner,GX,GY,GunR,600,1,1));
		FOwner->Add(new TMissileFlash(FOwner,GX,GY));
		   }break;
	}

	if( CreateSE && (_pat>=0)) FOwner->SoundEffect->WavPlay(WABShot,9);

}

void TStage3BossBit::Search()
{
	f32 GX = Sprite[0]->GetSpr()->FCX;
	f32 GY = Sprite[0]->GetSpr()->FCY;
	PosRoll( GX, GY,Sprite[0]->GetSpr()->FCX,Sprite[0]->GetSpr()->FCY,Sprite[0]->Rdegree);

	s32 DX = RoundOff(FOwner->HeroPos.X-GX);
	s32 DY = RoundOff(FOwner->HeroPos.Y-GY);
	s32 GunR = (RoundOff(atan2(DY,DX)/(f32)M_PI*2048.f) + 2048) & 0xfff;

	Sprite[0]->Rotate(GunR);


}

void TStage3BossBit::MakeLaser()
{
	FOwner->Add(new TOrthLaser(FOwner,X,Y,-5,50));
	FOwner->Add(new TMissileFlash(FOwner,X,Y));
	if( CreateSE ) FOwner->SoundEffect->WavPlay(WAHoming,10);

}

void TStage3BossBit::BeamMotion()
{
	s32 GX = RoundOff(Sprite[0]->GetSpr()->FCX);
	s32 GY = RoundOff(Sprite[0]->GetSpr()->FCY);

	s32 GunR = (Sprite[0]->GetSpr()->FR + 2048) & 0xfff;

	switch( ActFlg[KACTION_BEAM] ){
		//
	case 1:{
		if( (ActCount[KACTION_BEAM] == 0) && CreateSE ) FOwner->SoundEffect->WavPlay(WACharge,8);
		if( (ActCount[KACTION_BEAM]>0) ) FOwner->Add(new TBeamSite(FOwner,GX,GY,GunR,ActCount[KACTION_BEAM]*2));
		s32 ParticleR = random(4095);
		FOwner->Add(new TBLight(FOwner,GX,GY,(f32)ActCount[KACTION_BEAM]*0.06f,2000));
		FOwner->Add(new TParticle2(FOwner,GX+RoundOff(60*cost[ParticleR]),GY+RoundOff(60*sint[ParticleR]),1,ParticleR,-1,30));
		if( ActCount[KACTION_BEAM]>60 ) {
			Inc(ActFlg[KACTION_BEAM]);
			ActCount[KACTION_BEAM] = -1;
		}
		   }break;
	case 2:{
		Inc(ActFlg[KACTION_BEAM]);
		ActCount[KACTION_BEAM] = -1;
		FOwner->Add(new TSRControl(FOwner,GX,GY,GunR+2048));
		if( CreateSE ) FOwner->SoundEffect->WavPlay(WALBeam,10);
		   }break;
		//
	case 3:{
		if( ActCount[KACTION_BEAM] % 3 == 0 )
			FOwner->Add(new TZBeamPart(FOwner,X+RoundOff(120*cost[GunR]),Y+RoundOff(120*sint[GunR]),GunR,0.5f,true));

		FOwner->Add(new TBLight(FOwner,GX,GY,4,2000));

		if( ActCount[KACTION_BEAM]>100 ) {
			ActFlg[KACTION_BEAM] = 0;
			ActCount[KACTION_BEAM] = -1;
		}
		   }break;
	}
	Inc(ActCount[KACTION_BEAM]);

}

void TStage3BossBit::SelfDestruction()
{
	Place(320,240,32);
	ActFlg[KACTION_SELFDEST]  =  1;

}

void TStage3BossBit::SelfDestMotion()
{
	switch( ActFlg[KACTION_SELFDEST] ){
		//初期化
	case 1:{
		Inc(ActFlg[KACTION_SELFDEST]);
		ActCount[KACTION_SELFDEST]  =  -1;
		Sprite[numSpr-1]->GetSpr()->FVisible = true;
		if( CreateSE ) FOwner->SoundEffect->WavPlay2(WAFlash, 0, 150);
		   }break;
		//
	case 2:{
		Sprite[numSpr-1]->ZoomR(1.05f,1.05f);

		if( ActCount[KACTION_SELFDEST]>60 ) {
			Inc(ActFlg[KACTION_SELFDEST]);
			ActCount[KACTION_SELFDEST]  =  -1;
			FOwner->Add(new TBoxContrl(FOwner,3));
		}
		   }break;
	case 3:{
		Die();
		FOwner->BDieTime = FOwner->GetTick();
		   }break;
	}
	Inc(ActCount[KACTION_SELFDEST]);

}

void TStage3BossBit::LightningMotion()
{
	switch( ActFlg[KACTION_LIGHTNING] ){
		//
	case 1:{
		Sprite[0]->RotateR(32);
		if( ActCount[KACTION_LIGHTNING] % 6 == 0 ) Dec(ShotCount);

		if( ShotCount<=0 ) ActFlg[KACTION_LIGHTNING] = 3;

		   }break;
		//
	case 2:{
		Sprite[0]->RotateR(-32);
		if( ActCount[KACTION_LIGHTNING] % 6 == 0 ) Dec(ShotCount);

		if( ShotCount<=0 ) ActFlg[KACTION_LIGHTNING] = 4;
		   }break;
	case 3:{
		Sprite[0]->RotateR(32);
		if( ActCount[KACTION_LIGHTNING] % 3 == 0 ) MakeShot(1);

		if( ShotCount>=3 ) ActFlg[KACTION_LIGHTNING] = 1;
		   }break;
	case 4:{
		Sprite[0]->RotateR(-32);
		if( ActCount[KACTION_LIGHTNING] % 3 == 0 ) MakeShot(1);

		if( ShotCount>=3 ) ActFlg[KACTION_LIGHTNING] = 2;
		   }break;
	case 5:{
		BitLight->Show();
		   }
	}
	Inc(ActCount[KACTION_LIGHTNING]);

}

void TStage3BossBit::homingMotion()
{
	s32 GX = RoundOff(Sprite[0]->GetSpr()->FCX);
	s32 GY = RoundOff(Sprite[0]->GetSpr()->FCY);

	s32 GunR = (Sprite[0]->GetSpr()->FR + 2048) & 0xfff;

	switch( ActFlg[KACTION_HOMING] ){
	case 1:{
		if( ActCount[KACTION_HOMING] % 60 == 0 ) {
			if( EffLevel>0 ) {
				for( u32 i = 0; i <= 20; i++ )
					FOwner->Add(new TParticle2(FOwner,GX,GY,1,GunR+random(768*2)-768,random(15)+5,random(5)+1));
			}
			FOwner->Add(new TZakoHomingCore3(FOwner,GX,GY,GunR,5,0.2f,0));
			FOwner->Add(new TMissileFlash(FOwner,GX,GY));
		}

		if( ActCount[KACTION_HOMING]>300 ) {
			ActFlg[KACTION_HOMING] = 0;
			ActCount[KACTION_HOMING] = -1;
		}
		   }break;
	}
	Inc(ActCount[KACTION_HOMING]);


}

void TStage3BossBit::SetLightningState( s32 _state )
{
	ActFlg[KACTION_LIGHTNING] = _state;
	ActCount[KACTION_LIGHTNING]  =  -1;

}

void TStage3BossBit::Die()
{
	TOBJ::Die();
	BitLight->Die();

}

void TStage3BossBit::DieWithExplosion()
{
	FOwner->Add(new TExpmiddle(FOwner,X,Y,0,0,0));//爆発
	Die();

}

void TStage3Boss::SetNextState( s32 _current )
{
	TBossBase::SetNextState(_current);
	ActFlg[_current] = 0;
	ActCount[_current] = -1;

	switch( _current ){
	case KACTION_APPEAR     : 
		{
			ActFlg[KACTION_SHOT] = 1;
		}break;
	case KACTION_SHOT     : 
		{
			Inc(ShotCount);

			if( ShotCount<5 ) {
				ActFlg[KACTION_SHOT] = 1;
			}
			else {
				ShotCount = 0;
				ActFlg[KACTION_BEAM] = 1;
			}
		}break;
	case KACTION_BEAM     : 
		{
			Inc(ShotCount);

			if( ShotCount<4 ) {
				ActFlg[KACTION_BEAM] = 1;
			}
			else {
				ShotCount = 0;
				ActFlg[KACTION_LASER] = 1;
			}

		}break;
	case KACTION_LASER     : 
		{
			ActFlg[KACTION_SHOT] = 1;
		}break;
	}

	//発狂モード判定
	if( (HP<=MaxHP/3) && (ActFlg[KACTION_LIGHTNING] == 0) && (FOwner->Difficulty != KDIFFICULTY_EASY)) {
		ActFlg[KACTION_LIGHTNING] = 1;
		ActFlg[KACTION_SHOT] = 0;
		ActFlg[KACTION_BEAM] = 0;
		ActFlg[KACTION_LASER] = 0;
	}
}

void TStage3Boss::AppearEvent()
{
	switch( ActFlg[KACTION_APPEAR] ){
		//初期化
	case 1:{
		if( ActCount[KACTION_APPEAR] == 0 ) {
			FOwner->Add(new TLightning(FOwner, 600,-50, 600, 500));
			if( CreateSE ) FOwner->SoundEffect->WavPlay2(WAThunder, 10, 130);
		}

		if( ActCount[KACTION_APPEAR] == 5 )
			FOwner->Add(new TLightning(FOwner, 640,-50, 400, 500));

		if( ActCount[KACTION_APPEAR] > 60 ) {
			Inc(ActFlg[KACTION_APPEAR]);
			ActCount[KACTION_APPEAR]  =  -1;
		}

		   }break;
		//
	case 2:{
		DestX = 550;
		DestY = 240;

		Inc(ActFlg[KACTION_APPEAR]);
		ActCount[KACTION_APPEAR]  =  -1;
		ActFlg[KACTION_TRANSFER] = 1;
		   }break;
	case 3:{
		Transfer(DestX,DestY,32);

		if( ActCount[KACTION_TRANSFER] > 1000 ) {
			Inc(ActFlg[KACTION_APPEAR]);
			ActCount[KACTION_APPEAR]  =  -1;
		}
		   }break;
	case 4:{
		MotionControl(KMOTION_STAGE3BOSS_DEFAULT);
		Transfer_NoMotion(DestX,DestY,32);
		if( ActFlg[KACTION_TRANSFER] == 0 ) {
			SetNextState(KACTION_APPEAR);
		}
		   }break;
	case 5:{

		   }break;
	}
	Inc(ActCount[KACTION_APPEAR]);

}

void TStage3Boss::MakeShot( s32 _pat )
{
	s32 GX = RoundOff(Sprite[BGun]->GetSpr()->FCX);
	s32 GY = RoundOff(Sprite[BGun]->GetSpr()->FCY);

	s32 GunR = (Sprite[BGun]->GetSpr()->FR + 2048 + FOwner->GetRand(128)-64) & 0xfff ;
	switch( _pat ){
	case 0:{
		FOwner->Add(new TBoss3Shot(FOwner,GX,GY,GunR,15,1000,BSpeed,1,0,1));
		FOwner->Add(new TFCircle(FOwner,GX,GY,1,GunR,0));
		   }break;
	case 1:{

		   }break;
	}

	if( CreateSE ) FOwner->SoundEffect->WavPlay(WABShot,9);

}

void TStage3Boss::ShotMotion()
{
	switch( ActFlg[KACTION_SHOT] ){
		//初期化
	case 1:{
		Inc(ActFlg[KACTION_SHOT]);
		ActCount[KACTION_SHOT]  =  -1;
		ActFlg[KACTION_TRANSFER] = 1;
		DestX = FOwner->GetRand(3)*50 + 450;

		if( ShotCount == 0 ) {
			if( FOwner->HeroPos.Y < 240 )
				DestY = 100;
			else
				DestY = 380;
		}
		else {
			if( Y > 240 )
				DestY = 100;
			else
				DestY = 380;
		}
		   }break;
		//
	case 2:{
		Transfer(DestX,DestY,32);

		if( ActCount[KACTION_TRANSFER] > 1000 ) {
			Inc(ActFlg[KACTION_SHOT]);
			ActCount[KACTION_SHOT]  =  -1;
		}
		   }break;
	case 3:{
		Search();
		MotionControl(KMOTION_STAGE3BOSS_SHOT2);
		Transfer_NoMotion(DestX,DestY,32);
		if( ActFlg[KACTION_TRANSFER] == 0 ) {
			Inc(ActFlg[KACTION_SHOT]);
			ActCount[KACTION_SHOT]  =  -1;
		}
		   }break;
	case 4:{
		//      Search;
		MotionControl(KMOTION_STAGE3BOSS_SHOT2);
		if( ActCount[KACTION_SHOT] % 4 == 0 ) {
			MakeShot(0);
			Bit[0]->MakeShot(0);
			Bit[1]->MakeShot(0);
		}
		if( ActCount[KACTION_SHOT] >40 ) {
			SetNextState(KACTION_SHOT);
		}
		   }break;
	}
	Inc(ActCount[KACTION_SHOT]);

	Bit[0]->Place(X, Y-120,2);
	Bit[1]->Place(X, Y+120,2);

}

void TStage3Boss::RetreatMotion()
{
	switch( ActFlg[KACTION_RETREAT] ){
		//初期化
	case 0:
		{
			for( u32 i  =  0 ; i < numSpr; i++) {
				//あたり判定を消す
				Sprite[i]->GetSpr()->FHit = false;
			}

			HPGauge->Hide();

			Inc(ActFlg[KACTION_RETREAT]);
			ActCount[KACTION_RETREAT]  =  -1;

			ActFlg[KACTION_TRANSFER] = 1;
			DestX = 1000;
			DestY = Y;

			FKind = kEffect;   //ロック対象から外す
		}break;
		//
	case 1:{
		Transfer(DestX,DestY,24);

		if( X>700 ) {
			Die();
			FOwner->Add(new TBSBeamGen_St3(FOwner));
		}
		   }break;
	}
	Inc(ActCount[KACTION_RETREAT]);
	Bit[0]->Place(X, Y-120,2);
	Bit[1]->Place(X, Y+120,2);

}

void TStage3Boss::Fall()
{
	Accel();

	switch( ActFlg[KACTION_FALL] ){
		//初期化
	case 1:{

		for( u32 i  =  0 ; i < numSpr; i++) {
			//あたり判定を消す
			Sprite[i]->GetSpr()->FHit = false;
		}
		FOwner->BossDie = true;
		if( CreateBGM ) FOwner->MusicPlayer->Stop();
		HPGauge->Hide();
		FOwner->Add(new TExplarge(FOwner,X,Y,1,true));
		if( BonusRate>1 ) {
			FOwner->Add(new TMultiple(FOwner,BonusRate,X,Y));

			//ミサイル倍率がスコアに反映されなかったバグの修正
			//シングル面のほうは今から直すとランキングに影響するので、通し面のみ修正
			if( FOwner->Difficulty != KDIFFICULTY_SINGLE )
				FOwner->BossRate = BonusRate;
		}

		if( FOwner->IsSequence() ) FOwner->RequestDelBullets();

		Bit[0]->SetLightningState(0);
		Bit[1]->SetLightningState(0);

		Sprite[BLShoulder]->GetSpr()->FVisible  =  false;
		Sprite[BLArm]->GetSpr()->FVisible  =  false;
		Sprite[BGun]->GetSpr()->FVisible  =  false;
		if( ExtraEffects ) {
			Sprite[BLShoulder + ConstForeground]->GetSpr()->FVisible  =  false;
			Sprite[BLArm + ConstForeground]->GetSpr()->FVisible  =  false;
			Sprite[BGun + ConstForeground]->GetSpr()->FVisible  =  false;
		}

		Inc(ActFlg[KACTION_FALL]);
		ActCount[KACTION_FALL]  =  -1;
		   }break;
		//
	case 2:{
		MotionControl(KMOTION_STAGE3BOSS_FALL);

		if( ActCount[KACTION_FALL]>180 ) {
			Inc(ActFlg[KACTION_FALL]);
			ActCount[KACTION_FALL]  =  -1;
		}
		   }break;
	case 3:{
		DestX = 800;
		DestY = 0;

		Inc(ActFlg[KACTION_FALL]);
		ActCount[KACTION_FALL]  =  -1;
		ActFlg[KACTION_TRANSFER] = 1;
		   }break;
	case 4:{
		Transfer(DestX,DestY,32);

		if( ActCount[KACTION_FALL]=30 ) {
			Bit[0]->SelfDestruction();
			Bit[1]->SelfDestruction();
		}

		if( ActFlg[KACTION_TRANSFER] == 0 ) {
			Die();
		}
		   }
	}

	//炎上
	if( Age % 2 == 0 ) {
		s32 expX = RoundOff(Sprite[BLShoulder]->GetSpr()->FCX);
		s32 expY = RoundOff(Sprite[BLShoulder]->GetSpr()->FCY);
		FOwner->Add(new TMotherShipSmoke(FOwner,expX,expY,-18,-0.2, 8, 4));
		FOwner->Add(new TMotherShipFire(FOwner,expX,expY,-18,-0.2, 8, 4));
	}

	//振動
	if( Age % 2 == 0 )
		Sprite[BHip]->MoveR(0,-2);
	else
		Sprite[BHip]->MoveR(0,2);

	Inc(ActCount[KACTION_FALL]);

	//死にイベント早送り用
	if( ! ButtonON[0] ) ButtonFlg[0] = false;

	if( (! ButtonFlg[0]) && ButtonON[0] && (! FOwner->ResultExists)  && (FOwner->IsBossDefeated(2)) ) {
		FOwner->Add(new TResult2(FOwner));
		ButtonFlg[0] = true;
	}
}

void TStage3Boss::Search()
{
	f32 GX = Sprite[BGun]->GetSpr()->FCX;
	f32 GY = Sprite[BGun]->GetSpr()->FCY;
	PosRoll(GX, GY,Sprite[BGun]->GetSpr()->FCX,Sprite[BGun]->GetSpr()->FCY,Sprite[BGun]->Rdegree);

	s32 DX = RoundOff(FOwner->HeroPos.X-GX);
	s32 DY = RoundOff(FOwner->HeroPos.Y-GY);
	s32 GunR = (RoundOff(atan2(DY,DX)/(f32)M_PI*2048.f) + 2048) && 0xfff;

	if( (GunR <= 3072) && (GunR >= 1024) ) GunR  =  0;

	FOwner->MotionDataList[KMOTION_STAGE3BOSS_SHOT2].Sprites[BGun].Rdeg  =  GunR;
	FOwner->MotionDataList[KMOTION_STAGE3BOSS_SHOT2].Sprites[BLArm].Rdeg  =  GunR + 1024;
	EnemyDeg = GunR;

	if( FOwner->Difficulty!=KDIFFICULTY_HARD ) {
		Bit[0]->Search();
		Bit[1]->Search();
	}
	else {
		Bit[0]->Sprite[0]->Rotate(GunR);
		Bit[1]->Sprite[0]->Rotate(GunR);
	}

}

void TStage3Boss::BeamMotion()
{
	switch( ActFlg[KACTION_BEAM] ){
		//初期化
	case 1:{
		Beamcount = 0;
		Inc(ActFlg[KACTION_BEAM]);
		ActCount[KACTION_BEAM]  =  -1;
		DestX = FOwner->GetRand(6)*50 + 50;
		DestY = FOwner->GetRand(3)*30 + 50;
		if( FOwner->Speed>KSCROLLSPEED_NORMAL )
			DestX = DestX+150;
		Bit[0]->Place(DestX, DestY,24);

		DestX = FOwner->GetRand(6)*50 + 50;
		DestY = FOwner->GetRand(3)*30 + 340;
		if( FOwner->Speed>KSCROLLSPEED_NORMAL )
			DestX = DestX+100;
		Bit[1]->Place(DestX, DestY,24);
		   }break;
		//
	case 2:{
		DestX = FOwner->GetRand(3)*50 + 450;
		DestY = FOwner->GetRand(3)*50 + 150;

		Inc(ActFlg[KACTION_BEAM]);
		ActCount[KACTION_BEAM]  =  -1;
		ActFlg[KACTION_TRANSFER] = 1;
		   }break;
	case 3:{
		Aim(Bit[Beamcount]->X, Bit[Beamcount]->Y);
		Transfer(DestX,DestY,32);

		if( ActCount[KACTION_TRANSFER] > 1000 ) {
			Inc(ActFlg[KACTION_BEAM]);
			ActCount[KACTION_BEAM]  =  -1;
		}
		   }break;
	case 4:{
		Aim(Bit[Beamcount]->X, Bit[Beamcount]->Y);
		MotionControl(KMOTION_STAGE3BOSS_SHOT2);
		Transfer_NoMotion(DestX,DestY,32);
		if( ActFlg[KACTION_TRANSFER] = 0 ) {
			MakeBeam(Bit[Beamcount]->X, Bit[Beamcount]->Y);
			Inc(Beamcount);
			Inc(ActFlg[KACTION_BEAM]);
			ActCount[KACTION_BEAM]  =  -1;
		}
		   }break;
	case 5:{
		MotionControl(KMOTION_STAGE3BOSS_SHOT2);
		if( ActCount[KACTION_BEAM] >20 ) {
			if( Beamcount<2 ) {
				ActFlg[KACTION_BEAM] = 2;
				ActCount[KACTION_BEAM]  =  -1;
			}
			else
				SetNextState(KACTION_BEAM);
		}
		   }break;
	}
	Inc(ActCount[KACTION_BEAM]);

	Bit[0]->Search();
	Bit[1]->Search();

}

void TStage3Boss::Aim( s32 _x, s32 _y )
{
	f32 GX = Sprite[BGun]->GetSpr()->FCX;
	f32 GY = Sprite[BGun]->GetSpr()->FCY;
	PosRoll(GX, GY,Sprite[BGun]->GetSpr()->FCX,Sprite[BGun]->GetSpr()->FCY,Sprite[BGun]->Rdegree);

	s32 DX = RoundOff(_x-GX);
	s32 DY = RoundOff(_y-GY);
	s32 GunR = (RoundOff(atan2(DY,DX)/(f32)M_PI*2048.f) + 2048) & 0xfff;

	FOwner->MotionDataList[KMOTION_STAGE3BOSS_SHOT2].Sprites[BGun].Rdeg  =  GunR;
	FOwner->MotionDataList[KMOTION_STAGE3BOSS_SHOT2].Sprites[BLArm].Rdeg  =  GunR + 1024;
	EnemyDeg = GunR;

}

void TStage3Boss::MakeBeam( s32 dx, s32 dy )
{
	s32 GX = RoundOff(Sprite[BGun]->GetSpr()->FCX);
	s32 GY = RoundOff(Sprite[BGun]->GetSpr()->FCY);
	FOwner->Add(new TBoss3Beam(FOwner,GX,GY,dx,dy));

}

void TStage3Boss::LaserMotion()
{
	switch( ActFlg[KACTION_LASER] ){
		//初期化
	case 1:{
		Inc(ActFlg[KACTION_LASER]);
		ActCount[KACTION_LASER]  =  -1;
		Bit[0]->Place(550, 240,24);
		Bit[1]->Place(550, 240,24);
		Bit[0]->Sprite[0]->Rotate(0);
		Bit[1]->Sprite[0]->Rotate(0);
		   }break;
		//
	case 2:{
		DestX = 550;
		DestY = 240;

		Inc(ActFlg[KACTION_LASER]);
		ActCount[KACTION_LASER]  =  -1;
		ActFlg[KACTION_TRANSFER] = 1;
		   }break;
	case 3:{
		Transfer(DestX,DestY,24);

		if( ActCount[KACTION_TRANSFER] > 1000 ) {
			Inc(ActFlg[KACTION_LASER]);
			ActCount[KACTION_LASER]  =  -1;
		}
		   }break;
	case 4:{
		MotionControl(KMOTION_STAGE3BOSS_DEFAULT);
		Transfer_NoMotion(DestX,DestY,24);
		if( ActFlg[KACTION_TRANSFER] == 0 ) {
			Inc(ActFlg[KACTION_LASER]);
			ActCount[KACTION_LASER]  =  -1;
		}
		   }break;
	case 5:{
		MotionControl(KMOTION_STAGE3BOSS_DEFAULT);
		KeepPosition(DestX,DestY);

		//ビット回転
		f32 RotPosX  =  cost[ActCount[KACTION_LASER]*24 & 0xfff]*32;
		f32 RotPosY  =  sint[ActCount[KACTION_LASER]*24 & 0xfff]*200;
		Bit[0]->Place(550 + RotPosX, 240 +RotPosY,16);

		RotPosX  =  cost[(ActCount[KACTION_LASER]*24+2048) & 0xfff]*32;
		RotPosY  =  sint[(ActCount[KACTION_LASER]*24+2048) & 0xfff]*200;
		Bit[1]->Place(550 + RotPosX, 240 +RotPosY,16);

		s32 EndTime;
		s32 LaserInterval;
		s32 LaserEndTime;
		switch( FOwner->Difficulty ){
		case KDIFFICULTY_EASY:
			{
				EndTime = 720;
				LaserEndTime = 600;
				LaserInterval  =  120;
			}break;
		case KDIFFICULTY_HARD:
			{
				EndTime = 900;
				LaserEndTime = 840;
				LaserInterval  =  60;
			}break;
		default: 
			{
				EndTime = 900;
				LaserEndTime = 900;
				LaserInterval  =  90;
			}
		}

		if( (ActCount[KACTION_LASER]>30) && (ActCount[KACTION_LASER] <= LaserEndTime)) {
			if( ActCount[KACTION_LASER] % LaserInterval == 0 )
				Bit[0]->MakeLaser();

			if( (ActCount[KACTION_LASER]+30) % LaserInterval == 0 )
				Bit[1]->MakeLaser();
		}

		if( ActCount[KACTION_LASER]>EndTime )
			SetNextState(KACTION_LASER);
		   }
	}
	Inc(ActCount[KACTION_LASER]);

}

void TStage3Boss::LightningMotion()
{
	switch( ActFlg[KACTION_LIGHTNING] ){
		//初期化
	case 1:{
		Inc(ActFlg[KACTION_LIGHTNING]);
		ActCount[KACTION_LIGHTNING]  =  -1;
		Bit[0]->SetLightningState(5);
		Bit[1]->SetLightningState(5);
		   }break;
	case 2:{
		Inc(ActFlg[KACTION_LIGHTNING]);
		ActCount[KACTION_LIGHTNING]  =  -1;
		DestX = FOwner->GetRand(6)*50 + 50;
		DestY = FOwner->GetRand(3)*30 + 50;
		Bit[0]->Place(DestX, DestY,32);
		//      Bit[0]->SetLightningState(1);

		DestX = FOwner->GetRand(6)*50 + 50;
		DestY = FOwner->GetRand(3)*30 + 340;
		Bit[1]->Place(DestX, DestY,32);
		//      Bit[1]->SetLightningState(2);
		   }break;
		//
	case 3:{
		DestX = FOwner->GetRand(3)*50 + 500;
		DestY = FOwner->GetRand(3)*50 + 150;

		Inc(ActFlg[KACTION_LIGHTNING]);
		ActCount[KACTION_LIGHTNING]  =  -1;
		ActFlg[KACTION_TRANSFER] = 1;
		   }break;
	case 4:{
		Transfer(DestX,DestY,32);

		if( ActCount[KACTION_TRANSFER] > 1000 ) {
			Inc(ActFlg[KACTION_LIGHTNING]);
			ActCount[KACTION_LIGHTNING]  =  -1;

		}
		   }break;
	case 5:{
		KeepPosition(DestX,DestY);
		MotionControl(KMOTION_STAGE3BOSS_SHOT1);
		Transfer_NoMotion(DestX,DestY,32);
		if( ActFlg[KACTION_TRANSFER] == 0 ) {
			Inc(ActFlg[KACTION_LIGHTNING]);
			ActCount[KACTION_LIGHTNING]  =  -1;
			Bit[0]->SetLightningState(3);
			Bit[1]->SetLightningState(4);
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WACharge,8);
		}
		   }break;
	case 6:{
		KeepPosition(DestX,DestY);
		MotionControl(KMOTION_STAGE3BOSS_SHOT1);
		MakeChargeEffect();
		if( ActCount[KACTION_LIGHTNING]>60 ) {
			Inc(ActFlg[KACTION_LIGHTNING]);
			//        ActFlg[KACTION_LIGHTNING] = 1;
			ActCount[KACTION_LIGHTNING]  =  -1;
			MakeLightning();
		}
		   }break;
	case 7:{
		KeepPosition(DestX,DestY);
		MotionControl(KMOTION_STAGE3BOSS_SHOT1);
		if( ActCount[KACTION_LIGHTNING]>60 ) {
			ActFlg[KACTION_LIGHTNING] = 2;
			ActCount[KACTION_LIGHTNING]  =  -1;        
		}
		   }break;
	}
	Inc(ActCount[KACTION_LIGHTNING]);

}

void TStage3Boss::MakeChargeEffect()
{
	f32 GX = Sprite[BGun]->GetSpr()->FCX;
	f32 GY = Sprite[BGun]->GetSpr()->FCY;

	s32 ParticleR = random(4095);
	FOwner->Add(new TBLight(FOwner,RoundOff(GX),RoundOff(GY),ActCount[KACTION_LIGHTNING]*0.04f,2000));
	FOwner->Add(new TParticle2(FOwner,RoundOff(GX)+RoundOff(40.f*cost[ParticleR]),RoundOff(GY)+RoundOff(40.f*sint[ParticleR]),1,ParticleR,-1,30));

	ParticleR = random(4095);
	FOwner->Add(new TParticle_charge(FOwner, RoundOff(GX),RoundOff(GY),ParticleR, 8, 8, 255,192,128));


}

void TStage3Boss::MakeLightning()
{
	s32 GX = RoundOff(Sprite[BGun]->GetSpr()->FCX);
	s32 GY = RoundOff(Sprite[BGun]->GetSpr()->FCY);

	FOwner->Add(new TLightning(FOwner, GX,GY, Bit[0]->X, Bit[0]->Y));
	FOwner->Add(new TLightning(FOwner, GX,GY, Bit[1]->X, Bit[1]->Y));

	FOwner->Add(new TThunderSword(FOwner, GX,GY));
	if( CreateSE ) FOwner->SoundEffect->WavPlay2(WAThunder, 10, 150);

}

TStage3Boss::TStage3Boss( TOBJList* owner, s32 _x, s32 _y ) : TBossBase(owner)
{
	s32 hp;
	s32 def;
	switch( owner->Difficulty ){
	case KDIFFICULTY_SINGLE:
		{
			hp = BossMaxHP;
			def = 10;
		}break;
	case KDIFFICULTY_EASY:
	case KDIFFICULTY_NORMAL:
		{
			hp = BossMaxHP_Short;
			def = 20;
		}break;
	default: 
		{
			hp = BossMaxHP;
			def = 20;
		}
	}

	TBossBase::Init(_x, _y, hp, def);
	ConstForeground = 13;
	Motion_Default = KMOTION_STAGE3BOSS_DEFAULT;
	Motion_Foward = KMOTION_STAGE3BOSS_FOWARD;
	Motion_Back = KMOTION_STAGE3BOSS_BACK;

	MakeSprFromID( FOwner->GetDG(), KSPRDATA_STAGE3BOSS1, MechaTex, 16000, ExtraEffects);
	MotionControlSoon(KMOTION_STAGE3BOSS_DEFAULT);

	for( u32 i = 0; i <= 1; i++ ) {
		Bit[i] =  new TStage3BossBit(FOwner,X,Y);
		FOwner->Add(Bit[i]);
	}

	ActFlg[KACTION_APPEAR] = 1;

}

void TStage3Boss::Move()
{
	TBossBase::Move();
	if( (HP>0) && (FOwner->LimitTime>0) ) {
		if( ActFlg[KACTION_SHOT]>0 ) ShotMotion();
		if( ActFlg[KACTION_BEAM]>0 ) BeamMotion();
		if( ActFlg[KACTION_LASER]>0 ) LaserMotion();
		if( ActFlg[KACTION_LIGHTNING]>0 ) LightningMotion();
		if( ActFlg[KACTION_APPEAR]>0 ) AppearEvent();
	}
	else if( (HP>0) && (FOwner->LimitTime == 0) ) RetreatMotion();

	if( FOwner->IsSequence() ) {
		//発狂モード判定
		if( (HP<=MaxHP/3) && (ActFlg[KACTION_LIGHTNING] == 0) && (FOwner->Difficulty != KDIFFICULTY_EASY)) {
			ActFlg[KACTION_LIGHTNING] = 1;
			ActFlg[KACTION_SHOT] = 0;
			ActFlg[KACTION_BEAM] = 0;
			ActFlg[KACTION_LASER] = 0;
		}
	}

	//死亡判定
	if( (HP<=0) && (ActFlg[KACTION_FALL] == 0) )
		ActFlg[KACTION_FALL] = 1;

	if( (ActFlg[KACTION_FALL]>0) && (FOwner->LimitTime>0) ) Fall();

}

void TStage3Boss::Die()
{
	TBossBase::Die();

}

void TStage4Boss1::SetNextState( s32 _current )
{
	TBossBase::SetNextState(_current);

}

void TStage4Boss1::MakeShot( s32 _pat )
{
	s32 numBeam;
	s32 Rinterval;
	switch( FOwner->Difficulty ){
	case KDIFFICULTY_SINGLE:
		{
			numBeam = 15;
			Rinterval = 256;
		}break;
	case KDIFFICULTY_EASY:
		{
			numBeam = 12;
			Rinterval = 341;
		}break;
	default: {
		numBeam = 15;
		Rinterval = 256;
			 }
	}


	switch( _pat ){
	case 0:{
		s32 firstR = FOwner->GetRand(4095);

		for( u32 i  =  0 ; i <= numBeam; i++)
			FOwner->Add(new TCurvedLaser(FOwner,X,Y,firstR+i*Rinterval,32, 0, 1,10,0));

		for( u32 i  =  0 ; i <= numBeam; i++)
			FOwner->Add(new TCurvedLaser(FOwner,X,Y,firstR+i*Rinterval,-32, 0, 1,10,0));

		if( CreateSE ) FOwner->SoundEffect->WavPlay(WABeam,9);  
		Inc(ShotCount);
		   }break;
	}

}

void TStage4Boss1::ShotMotion()
{
	if( FOwner->Distance<47915 ) return;


	switch( ActFlg[KACTION_SHOT] ){
	case 1:{
		if( Age % 8 == 0 )
			FOwner->Add(new TAura(FOwner, X, Y));

		if( FOwner->Difficulty=KDIFFICULTY_HARD ) {
			if( (ActCount[KACTION_SHOT] % 120 == 0) || (ActCount[KACTION_SHOT] % 120 == 30) ) {
				MakeShot(0);
			}
		}
		else {
			if( ActCount[KACTION_SHOT] % 120 == 0 ) {
				MakeShot(0);
			}
		}
		   }break;
	}
	Inc(ActCount[KACTION_SHOT]);

}

void TStage4Boss1::Fall()
{
	switch( ActFlg[KACTION_FALL] ){
		//初期化
	case 1:
		{
			HPGauge->Hide();
			if( HP<=0 ) {
				FOwner->Add(new TExplarge(FOwner,X,Y,1,true));
				CalcBonus();
			}
			Sprite[0]->GetSpr()->FHit = false;
			Inc(ActFlg[KACTION_FALL]);
			ActCount[KACTION_FALL]  =  -1;
			FOwner->Quake(4,300);

			if( FOwner->IsSequence() ) FOwner->RequestDelBullets();

			if( CreateBGM ) FOwner->MusicPlayer->Stop();
		}break;
		//
	case 2:
		{
			Sprite[0]->ZoomR(1.01f,1.01f);
			Sprite[1]->ZoomR(1.01f,1.01f);

			s32 ParticleR = random(4095);
			FOwner->Add(new TParticle2(FOwner,X+RoundOff((f32)(80+ActCount[KACTION_FALL])*cost[ParticleR]),
				Y+RoundOff((f32)(80+ActCount[KACTION_FALL])*sint[ParticleR]),
				3,ParticleR,-2,50));

			if( ActCount[KACTION_FALL]>120 ) {
				//スクロール再開
				FOwner->Speed = KSCROLLSPEED_NORMAL;
				Inc(ActFlg[KACTION_FALL]);
				ActCount[KACTION_FALL]  =  -1;
			}
		}break;
	case 3:{
		Sprite[0]->ZoomR(1.01,1.01);
		Sprite[1]->ZoomR(1.01,1.01);

		if( X<200 ) {
			Inc(ActFlg[KACTION_FALL]);
			ActCount[KACTION_FALL]  =  -1;

			Sprite[2]->GetSpr()->SetColor(255,ctAlpha);
		}
		   }
	case 4:{
		Sprite[0]->ZoomR(1.01f,1.01f);
		Sprite[1]->ZoomR(1.01f,1.01f);
		Sprite[2]->ZoomR(1.2f,1.2f);
		//      Sprite[2]->GetSpr()->SetColorR(-2,ctAlpha);

		if( ActCount[KACTION_FALL]>20 ) {
			FOwner->Add(new TBoxContrl(FOwner,3));
			FOwner->Add(new TStage4Boss1Exp(FOwner));
			Inc(ActFlg[KACTION_FALL]);
			ActCount[KACTION_FALL]  =  -1;
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WABossExp,0);
		}
		   }break;
	case 5:{

		if( ActFlg[KACTION_TRANSFER] == 0 ) {
			Die();
		}
		   }break;
	}

	Inc(ActCount[KACTION_FALL]);

}

void TStage4Boss1::MakeChargeEffect()
{

}

TStage4Boss1::TStage4Boss1( TOBJList* owner, s32 _x, s32 _y ) : TBossBase(owner)
{
	s32 def;
	switch( owner->Difficulty ){
	case KDIFFICULTY_SINGLE:
		{
			def = 10;
		}break;
	default: 
		{
			def = 20;
		}
	}

	TBossBase::Init(_x, _y, KSTR_SHOT*192, def);

	ShotCount = 0;
	Score = 100000;
	CommandRed = true;
	ComBonus = 5000;

	//本体
	MakeSpriteLite( numSpr, X,Y,128,128,16000,bmAdd);
	Sprite[numSpr-1]->GetSpr()->AddTexture(Enemy4Tex, 1, 1, 128, 128);
	Sprite[numSpr-1]->GetSpr()->AddHitArea(TPoint(0, 0), TPoint(0, 128), TPoint(128, 128),TPoint(128, 0), htSky);
	Sprite[numSpr-1]->GetSpr()->FHit      =  true;
	Sprite[numSpr-1]->GetSpr()->FCX += 64;
	Sprite[numSpr-1]->GetSpr()->FCY += 64;
	Sprite[numSpr-1]->GetSpr()->Zoom(0.5f,0.5f);

	//グレア
	MakeSpriteLite( numSpr, X,Y,128,128,16000,bmAdd);
	Sprite[numSpr-1]->GetSpr()->AddTexture(Enemy4Tex, 129, 1, 256, 128);
	Sprite[numSpr-1]->GetSpr()->FCX += 64;
	Sprite[numSpr-1]->GetSpr()->FCY += 64;
	Sprite[numSpr-1]->GetSpr()->Zoom(1.2f,1.2f);

	//爆発用
	MakeSpriteLite( numSpr, X,Y,128,128,16000,bmAdd);
	Sprite[numSpr-1]->GetSpr()->AddTexture(Enemy4Tex, 129, 1, 256, 128);
	Sprite[numSpr-1]->GetSpr()->FCX += 64;
	Sprite[numSpr-1]->GetSpr()->FCY += 64;
	Sprite[numSpr-1]->GetSpr()->SetColor(0,ctAlpha);

	Sprite[0]->Add(Sprite[1]);
	Sprite[0]->Add(Sprite[2]);

	ActFlg[KACTION_SHOT] = 1;

}

void TStage4Boss1::Move()
{
	TBossBase::Move();
	TPoint pos = FOwner->GetScene()->Get2DPosFrom3D(glm::vec3(730.278f, -5.f, 18.1f));

	X = pos.X;
	Y = pos.Y;

	Sprite[0]->Move(pos.X, pos.Y);

	if( ActFlg[KACTION_FALL] == 0 ) {
		if( ActFlg[KACTION_SHOT]>0 ) ShotMotion();
		//スクロールを止める
		if( (FOwner->Distance>=47915) ) FOwner->Speed = 0;
	}

	//死亡判定
	if( ((HP<=0) || (FOwner->LimitTime<=10000)) && (ActFlg[KACTION_FALL]==0) )
		ActFlg[KACTION_FALL] = 1;

	if( ActFlg[KACTION_FALL]>0 ) Fall();

}

void TStage4Boss1::Die()
{
	TBossBase::Die();

}

TStage4Boss1Exp::TStage4Boss1Exp( TOBJList* owner ) : TOBJ(owner)
{
	FObjectName = "TStage4Boss1Exp";
	ActFlg = 0;
	ActCount = 0;

	X = 0;
	Y = 240;
	MaxHP = 0;
	HP = MaxHP;
	TempHP = HP;
	FSTR = 10000;
	FKind = kColBG;
	Score = 0;
	switch( FOwner->Difficulty ){
	case KDIFFICULTY_EASY:Speed = 2; break;
	case KDIFFICULTY_HARD:Speed = 4; break;
	default:
		Speed = 3;
	}

	MakeSpriteLite( numSpr, X,Y,64,128,16000,bmAdd);
	Sprite[numSpr-1]->GetSpr()->AddTexture(Enemy4Tex, 65, 1, 128, 128);
	//    FCX = FCX+64;
	Sprite[numSpr-1]->GetSpr()->FCY += 64;
	Sprite[numSpr-1]->GetSpr()->Zoom(3.f,6.f);
	Sprite[numSpr-1]->GetSpr()->Move(X,Y);

	MakeSpriteLite( numSpr, X,Y,64,128,16000,bmAdd);
	Sprite[numSpr-1]->GetSpr()->AddTexture(Enemy4Tex, 193, 1, 256, 128);
	//    FCX = FCX+64;
	Sprite[numSpr-1]->GetSpr()->FCY += 64;
	Sprite[numSpr-1]->GetSpr()->Zoom(4,7);
	Sprite[numSpr-1]->GetSpr()->Move(X,Y);

	MakeSpriteLite( numSpr, X,Y,128,128,16000,bmAdd);
	Sprite[numSpr-1]->GetSpr()->AddTexture(Enemy4Tex, 64, 1, 65, 128);
	Sprite[numSpr-1]->GetSpr()->AddHitArea(TPoint(0, 0), TPoint(0, 128), TPoint(128, 128),TPoint(128, 0), htSky);
	Sprite[numSpr-1]->GetSpr()->FHit      =  true;
	Sprite[numSpr-1]->GetSpr()->FCX += 128;
	Sprite[numSpr-1]->GetSpr()->FCY += 64;
	Sprite[numSpr-1]->GetSpr()->Zoom(6.f,6.f);
	Sprite[numSpr-1]->GetSpr()->Move(X,Y);

	MakeSpriteLite( numSpr, X,Y,128,128,16000,bmAdd);
	Sprite[numSpr-1]->GetSpr()->AddTexture(Enemy4Tex, 192, 1, 193, 128);
	Sprite[numSpr-1]->GetSpr()->FCX += 128;
	Sprite[numSpr-1]->GetSpr()->FCY += 64;
	Sprite[numSpr-1]->GetSpr()->Zoom(6,7);
	Sprite[numSpr-1]->GetSpr()->Move(X,Y);

	Sprite[0]->Add(Sprite[1]);
	Sprite[0]->Add(Sprite[2]);
	Sprite[0]->Add(Sprite[3]);
	Sprite[0]->Move(0,240);

	Alarm = new TDangerAlarm(FOwner, X);
	FOwner->Add(Alarm);
	Alarm->Show(true);

	if( CreateSE ) FOwner->SoundEffect->WavPlay2(WABaseExp, 10, 150);


}

TStage4Boss1Exp::~TStage4Boss1Exp()
{
	if( CreateSE && (! FOwner->BossDie)) FOwner->SoundEffect->WavStop(10);

}

void TStage4Boss1Exp::Move()
{
	TOBJ::Move();
	X = RoundOff(Sprite[0]->GetSpr()->FCX);

	switch( ActFlg ){
	case 0:{
		if( (Age==30) && CreateSE ) FOwner->SoundEffect->WavPlay2(WAAlarm, 3, 120);

		TPoint pos = FOwner->GetScene()->Get2DPosFrom3D(glm::vec3(730.278f, -5.f, 18.1f));
		Y = pos.Y;
		if( X<-320 ) X = -320;
		if( X>640 ) X = 640;

		Alarm->Place(X);
		if( Age == 300 ) Alarm->Show(false);     

		Sprite[0]->Move(X, Y);
		if( (! TimeInfinite) || (FOwner->ReplayON) ) Sprite[0]->MoveR(Speed,0);

		if( FOwner->LimitTime<=5000 )
			Sprite[0]->MoveR(12,0);

		if( (FOwner->LimitTime<=0) && (X>600) ) FOwner->HeroHP  =  0;


		if( Age % 60 == 0 )
			FOwner->Quake(4,60);

		if( FOwner->Distance>63600 ) {
			ActFlg = 1;
			Sprite[2]->GetSpr()->FHit = false;
			FOwner->Add(new TStage4EndingDemo(FOwner));
			FOwner->BossDie = true;
		}
		   }break;
	case 1:{
		Sprite[0]->MoveR(-10,0);
		if( X<-320 ) Die();   
		   }break;
	}

	if( Age % 2 == 0 ) {
		f32 Zoom = random(3)+3;
		FOwner->Add(new TExpBasic2(FOwner,X+random(120)+120,Y+random(480)-240,0,-15,Zoom,Zoom));
	}

	if( Age % 600 == 0 )
		if( CreateSE ) FOwner->SoundEffect->WavPlay2(WABaseExp, 10, 150);

	if( Age == 60 )
		if( CreateBGM ) FOwner->MusicPlayer->Play(1);   

}

void TStage4Boss1Exp::Die()
{
	TOBJ::Die();
	Alarm->Die();

}

void TStage4Boss2::SetNextState( s32 _current )
{
	TBossBase::SetNextState(_current);
	ActFlg[_current] = 0;
	ActCount[_current] = -1;

	switch( _current ){
	case KACTION_APPEAR     : 
		{
			ActFlg[KACTION_SHOT] = 1;
		}break;
	case KACTION_SHOT     : 
		{
			if( CheckDistance(X,Y,FOwner->HeroPos.X,FOwner->HeroPos.Y,240) )
				ActFlg[KACTION_CUTTER] = 1;
			else
				ActFlg[KACTION_SHOT] = 1;
		}break;
	case KACTION_CUTTER     : 
		{
			ActFlg[KACTION_SHOT] = 1;
		}break;
	case KACTION_LASER     : 
		{
			ActFlg[KACTION_SHOT] = 1;
		}break;
	}

	//
	if( ((FOwner->LimitTime<=5000) || (FOwner->BossDie)) && (ActFlg[KACTION_RETREAT] == 0) ) {
		ActFlg[KACTION_RETREAT] = 1;
		ActFlg[KACTION_SHOT] = 0;
		ActFlg[KACTION_LASER] = 0;
		ActFlg[KACTION_CUTTER] = 0;
	}

}

void TStage4Boss2::AppearEvent()
{
	switch( ActFlg[KACTION_APPEAR] ){
		//初期化
	case 1:{
		Sprite[BHip]->SetDest(550,Y,16);
		Inc(ActFlg[KACTION_APPEAR]);
		ActCount[KACTION_APPEAR]  =  -1;
		   }break;
		//
	case 2:{
		Sprite[BHip]->AutoSlide();
		MotionControl(KMOTION_STAGE3BOSS_BACK);
		if( ActCount[KACTION_APPEAR]>30 ) {
			Inc(ActFlg[KACTION_APPEAR]);
			ActCount[KACTION_APPEAR]  =  -1;
		}
		   }break;
	case 3:{
		Sprite[BHip]->AutoSlide();
		MotionControl(KMOTION_STAGE3BOSS_DEFAULT);
		if( ActCount[KACTION_APPEAR]>50 ) {
			SetNextState(KACTION_APPEAR);
		}
		   }break;
	}
	Inc(ActCount[KACTION_APPEAR]);

}

void TStage4Boss2::MakeShot( s32 _r )
{
	FOwner->Add(new TBoss3Shot(FOwner,X,Y,_r,10,500, BSpeed, 1,1,1));
	FOwner->Add(new TFCircle(FOwner,X,Y,1,_r,0));
	if( CreateSE ) FOwner->SoundEffect->WavPlay2(WABShot,9, 150);

}

void TStage4Boss2::ShotMotion()
{
	switch( ActFlg[KACTION_SHOT] ){
	case 1:{
		ShotCount = 0;
		ShotCount_Spread = 0;
		Inc(ActFlg[KACTION_SHOT]);
		ActCount[KACTION_SHOT]  =  -1;
		ActFlg[KACTION_TRANSFER] = 1;
		DestX = FOwner->GetRand(3)*50 + 450;

		if( Y > 240 )
			DestY = 100;
		else
			DestY = 380;
		   }break;
		//
	case 2:{
		Transfer(DestX,DestY,32);

		if( (ActCount[KACTION_SHOT] % 2 == 0) && (ActCount[KACTION_SHOT] <= 40) ) {
			s32 ShotR  =  ShotCount * 384 + 1024;
			if( FOwner->Difficulty != KDIFFICULTY_EASY ) MakeShot(ShotR);
			Inc(ShotCount);
		}

		if( ActCount[KACTION_TRANSFER] > 1000 ) {
			Inc(ActFlg[KACTION_SHOT]);
			ActCount[KACTION_SHOT]  =  -1;
		}
		   }break;
	case 3:{
		TargetX = FOwner->GetRand(3)*50 + 240;
		if( DestY <240 )
			TargetY = 400 - ShotCount_Spread*80;
		else
			TargetY = 80 + ShotCount_Spread*80;

		Inc(ActFlg[KACTION_SHOT]);
		ActCount[KACTION_SHOT]  =  -1;
		   }break;
	case 4:{
		Aim(TargetX,TargetY);
		MotionControl(KMOTION_STAGE4BOSS_SHOT);
		Transfer_NoMotion(DestX,DestY,32);

		if( FOwner->Difficulty == KDIFFICULTY_HARD ) {
			if( (ActCount[KACTION_SHOT] % 2 == 0) && (ActCount[KACTION_SHOT] <= 40) ) {
				s32 ShotR  =  ShotCount * 384 + 1024;
				MakeShot(ShotR);
				Inc(ShotCount);
			}
		}

		if( ActCount[KACTION_SHOT] > 20 ) {
			MakeSpread();
			Inc(ShotCount_Spread);
			if( ShotCount_Spread>3 ) {
				Inc(ActFlg[KACTION_SHOT]);
				ActCount[KACTION_SHOT]  =  -1;
			}
			else {
				ActFlg[KACTION_SHOT] = 3;
				ActCount[KACTION_SHOT]  =  -1;
			}
		}
		   }break;
	case 5:{
		MotionControl(KMOTION_STAGE4BOSS_DEFAULT);
		if( ActCount[KACTION_SHOT] > 30 ) {
			SetNextState(KACTION_SHOT);
			ShotCount = 0;
			ShotCount_Spread = 0;
		}
		   }break;
	}
	Inc(ActCount[KACTION_SHOT]);

}

void TStage4Boss2::RetreatMotion()
{
	switch( ActFlg[KACTION_RETREAT] ){
		//初期化
	case 1:{
		for( u32 i  =  0 ; i < numSpr; i++ ) {
			//あたり判定を消す
			Sprite[i]->GetSpr()->FHit = false;
		}

		HPGauge->Hide();

		Inc(ActFlg[KACTION_RETREAT]);
		ActCount[KACTION_RETREAT]  =  -1;

		ActFlg[KACTION_TRANSFER] = 1;
		DestX = 1000;
		DestY = Y;

		FKind = kEffect;   //ロック対象から外す
		   }break;
		//
	case 2:{
		Transfer(DestX,DestY,32);

		if( X>800 ) {
			Die();
		}
		   }break;
	}
	Inc(ActCount[KACTION_RETREAT]);


}

void TStage4Boss2::CutterMotion()
{
	switch( ActFlg[KACTION_CUTTER] ){
	case 1:{
		Inc(ActFlg[KACTION_CUTTER]);
		ActCount[KACTION_CUTTER]  =  -1;
		ActFlg[KACTION_TRANSFER] = 1;

		DestX = 600;
		DestY = Y;
		   }break;
	case 2:{
		Transfer_NoMotion(DestX,DestY,32);
		MotionControl(KMOTION_STAGE4BOSS_CUTTER1);

		if( ActCount[KACTION_CUTTER] > CutterSpeed ) {
			DestX = FOwner->HeroPos.X;
			DestY = FOwner->HeroPos.Y;

			if( DestX<300 ) DestX = 300;

			Sprite[BHip]->SetDest(DestX, DestY, 16);
			ActFlg[KACTION_TRANSFER] = 1;
			Inc(ActFlg[KACTION_CUTTER]);
			ActCount[KACTION_CUTTER]  =  -1;
		}
		   }break;
	case 3:{
		Sprite[BHip]->AutoSlide();
		MotionControl(KMOTION_STAGE4BOSS_CUTTER2);
		if( ActCount[KACTION_CUTTER] > 5 ) {
			Inc(ActFlg[KACTION_CUTTER]);
			ActCount[KACTION_CUTTER]  =  -1;
		}
		   }break;
		//
	case 4:{
		Sprite[BHip]->AutoSlide();
		MotionControl(KMOTION_STAGE4BOSS_CUTTER3);
		if( ActCount[KACTION_CUTTER] > 60 ) {
			Inc(ActFlg[KACTION_CUTTER]);
			ActCount[KACTION_CUTTER]  =  -1;
		}
		   }break;
	case 5:{
		MotionControl(KMOTION_STAGE4BOSS_CUTTER2);
		if( ActCount[KACTION_CUTTER] > 5 ) {
			Inc(ActFlg[KACTION_CUTTER]);
			ActCount[KACTION_CUTTER]  =  -1;
		}
		   }break;
	case 6:{
		Sprite[BHip]->AutoSlide();
		MotionControl(KMOTION_STAGE4BOSS_DEFAULT);
		if( ActCount[KACTION_CUTTER] > 30 ) {
			SetNextState(KACTION_CUTTER);
		}
		   }break;
	}
	Inc(ActCount[KACTION_CUTTER]);

}

void TStage4Boss2::Fall()
{
	switch( ActFlg[KACTION_FALL] ){
		//初期化
	case 1:{

		for( u32 i  =  0 ; i < numSpr; i++ ) {
			//あたり判定を消す
			Sprite[i]->GetSpr()->FHit = false;
		}

		HPGauge->Hide();
		FOwner->Add(new TExplarge(FOwner,X,Y,1,true));

		if( BonusRate>1 ) {
			FOwner->Add(new TMultiple(FOwner,BonusRate,X,Y));
			FOwner->BossRate = BonusRate;
		}
		else
			FOwner->BossRate = 1;

		Inc(ActFlg[KACTION_FALL]);
		ActCount[KACTION_FALL]  =  -1;
		   }break;
		//
	case 2:{
		MotionControl(KMOTION_STAGE3BOSS_FALL);

		Sprite[BHip]->MoveR(-5,0);

		if( X<-100 ) {
			Die();
		}
		   }break;
	}

	//炎上
	if( Age % 2 == 0 ) {
		FOwner->Add(new TMotherShipSmoke(FOwner,X,Y,-18,-0.2f, 8, 4));
		FOwner->Add(new TMotherShipFire(FOwner,X,Y,-18,-0.2f, 8, 4));
	}

	//振動
	if( Age % 2 == 0 )
		Sprite[BHip]->MoveR(0,-2);
	else
		Sprite[BHip]->MoveR(0,2);

	Inc(ActCount[KACTION_FALL]);

}

void TStage4Boss2::Search()
{
	f32 GX = Sprite[BGun]->GetSpr()->FCX;
	f32 GY = Sprite[BGun]->GetSpr()->FCY;
	PosRoll(GX, GY,Sprite[BGun]->GetSpr()->FCX,Sprite[BGun]->GetSpr()->FCY,Sprite[BGun]->Rdegree);

	s32 DX = RoundOff(FOwner->HeroPos.X-GX);
	s32 DY = RoundOff(FOwner->HeroPos.Y-GY);
	s32 GunR = (RoundOff(atan2(DY,DX)/(f32)M_PI*2048) + 2048) & 0xfff;

	if( (GunR <= 3072) && (GunR >= 1024) ) GunR  =  0;

	FOwner->MotionDataList[KMOTION_STAGE4BOSS_SHOT].Sprites[BGun].Rdeg  =  GunR;
	FOwner->MotionDataList[KMOTION_STAGE4BOSS_SHOT].Sprites[BRArm].Rdeg  =  GunR+1024;
	EnemyDeg = GunR;


}

void TStage4Boss2::Aim( s32 _x, s32 _y )
{
	f32 GX = Sprite[BGun]->GetSpr()->FCX;
	f32 GY = Sprite[BGun]->GetSpr()->FCY;
	PosRoll( GX, GY,Sprite[BGun]->GetSpr()->FCX,Sprite[BGun]->GetSpr()->FCY,Sprite[BGun]->Rdegree);

	s32 DX = RoundOff(_x-GX);
	s32 DY = RoundOff(_y-GY);
	s32 GunR = (RoundOff(atan2(DY,DX)/(f32)M_PI*2048) + 2048) & 0xfff;

	FOwner->MotionDataList[KMOTION_STAGE4BOSS_SHOT].Sprites[BGun].Rdeg  =  GunR;
	FOwner->MotionDataList[KMOTION_STAGE4BOSS_SHOT].Sprites[BRArm].Rdeg  =  GunR + 1024;
	EnemyDeg = GunR;

}

void TStage4Boss2::MakeSpread()
{
	f32 GX = Sprite[BGun]->GetSpr()->FCX - 79;
	f32 GY = Sprite[BGun]->GetSpr()->FCY - 1;
	PosRoll(GX, GY,Sprite[BGun]->GetSpr()->FCX,Sprite[BGun]->GetSpr()->FCY,Sprite[BGun]->Rdegree);

	FOwner->Add(new TMissileFlash(FOwner,RoundOff(GX), RoundOff(GY)));
	FOwner->Add(new TSpreadBeam(FOwner, RoundOff(GX), RoundOff(GY), TargetX, TargetY, 15));

	if( CreateSE ) FOwner->SoundEffect->WavPlay2(WATank_shot, 11, 100);
}

TStage4Boss2::TStage4Boss2( TOBJList* owner, s32 _x, s32 _y ) : TBossBase(owner)
{
	s32 def;
	switch( owner->Difficulty ){
	case KDIFFICULTY_SINGLE:
		{
			def = 10;
		}break;
	default: 
		{
			def = 20;
		}
	}

	MaxHP = KSTR_SHOT*192;
	TBossBase::Init(_x, _y, MaxHP, def);

	TargetX = 0;
	TargetY = 0;
	ConstForeground = 18;

	FOwner->BossRate = 0;   //このボスを撃破できなければボーナスは0としたいため、ここで倍率0を設定

	if( FOwner->Difficulty == KDIFFICULTY_HARD )
		CutterSpeed = 10;
	else
		CutterSpeed = 30;

	Motion_Default = KMOTION_STAGE4BOSS_DEFAULT;
	Motion_Foward = KMOTION_STAGE4BOSS_FOWARD;
	Motion_Back = KMOTION_STAGE4BOSS_BACK;

	MakeSprFromID( FOwner->GetDG(), KSPRDATA_STAGE4BOSS2, MechaTex, 16000, ExtraEffects);
	MotionControlSoon(KMOTION_STAGE4BOSS_DEFAULT);

	ActFlg[KACTION_APPEAR] = 1;

}

void TStage4Boss2::Move()
{
	TBossBase::Move();
	if( HP>0 ) {
		if( ActFlg[KACTION_SHOT]>0 ) ShotMotion();
		if( ActFlg[KACTION_APPEAR]>0 ) AppearEvent();
		if( ActFlg[KACTION_CUTTER]>0 ) CutterMotion();
		if( ActFlg[KACTION_RETREAT]>0 ) RetreatMotion();
	}

	//死亡判定
	if( (HP<=0) && (ActFlg[KACTION_FALL] == 0) )
		ActFlg[KACTION_FALL] = 1;

	if( ActFlg[KACTION_FALL]>0 ) Fall();

}

void TStage4Boss2::Die()
{
	TBossBase::Die();

}

TStage4Boss2Gene::TStage4Boss2Gene( TOBJList* owner, s32 _x, s32 _y ) : TOBJ(owner)
{
	FObjectName = "TStage4Boss2Gene";
	X = _x;
	Y = _y;
	FKind = kEffect2;

}

void TStage4Boss2Gene::Move()
{
	TOBJ::Move();
	switch( Age ){
	case 1:{
		if( FOwner->LimitTime>=10000 )
			FOwner->Add(new TWarp2D(FOwner,X,Y,0));
		else
			Die();
		   }break;
	case 130:{
		FOwner->Add(new TStage4Boss2(FOwner,X,Y));
		Die();
			 }
	}

}

void TStage4Guard::AppearEvent()
{
	switch( ActFlg[KACTION_S2GR_APPEAR] ){
		//初期化
	case 1:{
		Inc(ActFlg[KACTION_S2GR_APPEAR]);
		ActCount[KACTION_S2GR_APPEAR]  =  -1;
		Sprite[BHip]->SetDest(X-400, Y, 24);
		   }break;
	case 2:{
		Sprite[BHip]->AutoSlide();
		MotionControl(KMOTION_STAGE2GUARD_DEFAULT);
		if( ActCount[KACTION_S2GR_APPEAR]>=30 ) {
			SetNextState(KACTION_S2GR_APPEAR);
		}
		   }break;
	}
	Inc(ActCount[KACTION_S2GR_APPEAR]);

}

void TStage4Guard::RetreatMotion()
{
	switch( ActFlg[KACTION_S2GR_RETREAT] ){
		//初期化
	case 1:{
		Inc(ActFlg[KACTION_S2GR_RETREAT]);
		ActCount[KACTION_S2GR_RETREAT]  =  -1;

		ActFlg[KACTION_S2GR_TRANSFER] = 1;
		DestX = -400;
		DestY = Y;
		   }break;
		//
	case 2:{
		Transfer(DestX,DestY,24);

		if( X<-200 ) {
			Die();
			Dec(FOwner->MShipHP,FSTR*5);
			FOwner->Add(new TMotherShipDamaged(FOwner, X, Y));
		}
		   }break;
	}
	Inc(ActCount[KACTION_S2GR_RETREAT]);

}

void TStage4Guard::SetNextState( s32 _current )
{
	TStage2Guard::SetNextState(_current);
	//退却判定
	if( (FOwner->Distance>=46000) && (ActFlg[KACTION_S2GR_RETREAT] == 0) ) {
		ActFlg[KACTION_S2GR_RETREAT] = 1;
		ActFlg[KACTION_S2GR_APPEAR] = 0;
		ActFlg[KACTION_S2GR_BEAM] = 0;
		ActFlg[KACTION_S2GR_BLADE] = 0;;
	}

}

TStage4Guard::TStage4Guard( TOBJList* owner, s32 _x, s32 _y ) : TStage2Guard(owner, _x, _y, NULL)
{
	Age = 0;
	FOwner = owner;
	FDead = false;
	numSpr = 0;
	distance = 0;
	TempHP = 0;
	FDef = 0;
	LastSlashed = 0;
	ResilienceX = 0;
	ResilienceY = 0;

	FObjectName = "TStage4Guard";

	X = _x;
	Y = _y;

	if( FOwner->Difficulty=KDIFFICULTY_HARD )
		TransferSpeed = 48;
	else
		TransferSpeed = 20;

	MaxHP = KSTR_SHOT*48;
	HP = MaxHP;
	TempHP = HP;
	FSTR = 100;
	FKind = kEnemy;
	Score = 40000;
	Buzzed = false;
	LastBuzz = 0;
	GaugeTime = 0;
	TotalFlight = 0;
	for( u32 i = 0; i <= 99; i++ ) {
		ActFlg[i] = 0;
		ActCount[i] = 0;
	}
	LastFlash = 0;
	EnemyDeg = 0;
	BSpeed = 0;
	IniDis = 0;
	DestDeg = 0;
	ShotCount = 0;
	OriginColor = 255;
	CommandRed = true;
	ComBonus = 5000;

	BladeTrace = NULL;

	BWSPurged = true;

	if( _y<240 )
		Pattern  =  0;
	else
		Pattern  =  1;

	//IDの付与
	FID = NewID;
	Inc(NewID);

	MakeSprFromID( FOwner->GetDG(), KSPRDATA_STAGE4GUARD, MechaTex, 16000, ExtraEffects);

	for( u32 i = 0 ; i < numSpr; i++ ) {
		Sprite[i]->GetSpr()->SetColor(80,ctBlue);
		Sprite[i]->GetSpr()->SetColor(64,ctGreen);
	}

	if( ExtraEffects ) {
		for( u32 i  =  BConstForeground ; i < numSpr; i++ ) {
			Sprite[i]->GetSpr()->SetColor(64,ctAlpha);
		}
	}


	Sprite[BBlade]->GetSpr()->FHit = false;

	HPGauge  =  new TEnemyGauge2(FOwner,this,-128,32);
	FOwner->Add(HPGauge);

	MotionControlSoon(KMOTION_STAGE2GUARD_DEFAULT);

	ActFlg[KACTION_S2GR_APPEAR] = 1;

}

void TStage4Guard::Move()
{
	Inc(Age);
	X = RoundOff(Sprite[BHip]->GetSpr()->FCX);
	Y = RoundOff(Sprite[BHip]->GetSpr()->FCY);
	if( HP>0 ) {
		if( ActFlg[KACTION_S2GR_APPEAR]>0 ) AppearEvent();
		if( ActFlg[KACTION_S2GR_BEAM]>0 ) ShotMotion();
		if( ActFlg[KACTION_S2GR_BLADE]>0 ) BladeMotion();
		if( ActFlg[KACTION_S2GR_RETREAT]>0 ) RetreatMotion();

		Accel();
	}

	if( (Age-LastBuzz)>=40 ) Buzzed = false;
	if( (Age-GaugeTime)>120 ) HPGauge->Hide();
	if( (Age-LastFlash == 2) ) {
		for( u32 i = 0 ; i < BConstForeground; i++ )
			Sprite[i]->GetSpr()->SetColor(OriginColor,ctRed);          //色を戻す

		Sprite[BHip]->MoveR(-1,0);                  //のけぞり戻し
	}

	if( HP<=0 ) {
		OnBroken();                        //HPが無くなったら死ぬ
	}

}

void TStage5Boss1::SetNextState( s32 _current )
{
	TBossBase::SetNextState(_current);
	ActFlg[_current] = 0;
	ActCount[_current] = -1;

	if( FOwner->Distance>=RetreatDistance ) {
		ActFlg[KACTION_RETREAT] = 1;
	}
	else {
		switch( _current ){
		case KACTION_APPEAR     : 
			{
				ActFlg[KACTION_SHOT] = 1;
			}break;
		case KACTION_SHOT     : 
			{
				ActFlg[KACTION_SPREAD] = 1;
			}break;
		case KACTION_SPREAD     : 
			{
				ActFlg[KACTION_BODYATTACK] = 1;
			}break;
		case KACTION_BODYATTACK     : 
			{
				ActFlg[KACTION_SHOT2] = 1;
			}break;
		case KACTION_SHOT2     : 
			{
				ActFlg[KACTION_JUMP] = 1;
			}break;
		case KACTION_JUMP     : 
			{
				ActFlg[KACTION_RAIN] = 1;
				//        ActFlg[KACTION_JUMP] = 1;
			}break;
		case KACTION_RAIN     : 
			{
				ActFlg[KACTION_SHOT] = 1;
			}break;
		}
	}
}

void TStage5Boss1::AppearEvent()
{
	switch( ActFlg[KACTION_APPEAR] ){
	case 1:{
		Inc(ActFlg[KACTION_APPEAR]);
		ActCount[KACTION_APPEAR]  =  -1;
		JumpSpeed = 0;
		   }break;
		//
	case 2:{
		Sprite[Sprite_Center]->MoveR(0,JumpSpeed);
		JumpSpeed = JumpSpeed+1;
		MotionControl(KMOTION_STAGE5BOSS1_JUMP);

		if( (Y>=70) ) {
			Inc(ActFlg[KACTION_APPEAR]);
			ActCount[KACTION_APPEAR]  =  -1;
			FOwner->Add(new TSplashDebris(FOwner, X-176, 470));
			FOwner->Add(new TSplashDebris(FOwner, X+176, 470));
			FOwner->Quake(4,60);
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WATank_jump, 10);
		}
		   }break;
	case 3:{
		MotionControl(KMOTION_STAGE5BOSS1_SITDOWN);

		if( ActCount[KACTION_APPEAR] > 20 ) {
			Inc(ActFlg[KACTION_APPEAR]);
			ActCount[KACTION_APPEAR]  =  -1;
		}
		   }break;
	case 4:{
		MotionControl(KMOTION_STAGE5BOSS1_DEFAULT);

		if( ActCount[KACTION_APPEAR] > 30 ) {
			SetNextState(KACTION_APPEAR);
			FOwner->Add(new TWeakPoint2(FOwner,Sprite[BHead]));
		}
		   }break;
	}
	Inc(ActCount[KACTION_APPEAR]);

}

void TStage5Boss1::MakeShot( s32 _pat )
{
	f32 GX,GY;   //発射する座標
	s32 GunR;   //発射角度
	switch( _pat ){
	case 0:
		{
			GunR = Sprite[BLArm]->Rdegree;
			GX = Sprite[BLArm]->GetSpr()->FCX -190 ;
			GY = Sprite[BLArm]->GetSpr()->FCY + 36;
			PosRoll(GX, GY,Sprite[BLArm]->GetSpr()->FCX,Sprite[BLArm]->GetSpr()->FCY,Sprite[BLArm]->Rdegree);
			FOwner->Add(new TBoss3Shot(FOwner,RoundOff(GX),RoundOff(GY),GunR + 2048,25,500, BSpeed, 1,0,1));
			FOwner->Add(new TShotFireLarge(FOwner,RoundOff(GX),RoundOff(GY),2));
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WATank_shot, 9);
		}break;
	case 1:
		{
			GunR = Sprite[BRArm]->Rdegree;
			GX = Sprite[BRArm]->GetSpr()->FCX -190 ;
			GY = Sprite[BRArm]->GetSpr()->FCY + 36;
			PosRoll(GX, GY,Sprite[BRArm]->GetSpr()->FCX,Sprite[BRArm]->GetSpr()->FCY,GunR);
			FOwner->Add(new TSpreadBeam2(FOwner,RoundOff(GX),RoundOff(GY),GunR,15));
			FOwner->Add(new TShotFireWithSmoke(FOwner,RoundOff(GX),RoundOff(GY),GunR,3));

			Sprite[BRShoulder]->RotateR(-512);
			Sprite[BRArm]->RotateR(512);

			if( CreateSE ) FOwner->SoundEffect->WavPlay(WATank_shot, 9);
		}break;
	case 2:
		{
			GunR = Sprite[BLArm]->Rdegree;
			GX = Sprite[BLArm]->GetSpr()->FCX -190 ;
			GY = Sprite[BLArm]->GetSpr()->FCY + 36;
			PosRoll(GX, GY,Sprite[BLArm]->GetSpr()->FCX,Sprite[BLArm]->GetSpr()->FCY,Sprite[BLArm]->Rdegree);
			FOwner->Add(new TBoss3Shot(FOwner,RoundOff(GX),RoundOff(GY),GunR + 2048,10,500, BSpeed, 1,1,1));
			FOwner->Add(new TShotFireLarge(FOwner,RoundOff(GX),RoundOff(GY),2));
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WATank_shot, 9);
		}break;
	case 3:
		{
			GunR = Sprite[BRArm]->Rdegree;
			GX = Sprite[BRArm]->GetSpr()->FCX -190 ;
			GY = Sprite[BRArm]->GetSpr()->FCY + 36;
			PosRoll(GX, GY,Sprite[BRArm]->GetSpr()->FCX,Sprite[BRArm]->GetSpr()->FCY,GunR);
			FOwner->Add(new TShotFireWithSmoke(FOwner,RoundOff(GX),RoundOff(GY),GunR,3));
		}break;
	case 4:
		{
			GunR = Sprite[BRArm]->Rdegree;
			GX = Sprite[BRArm]->GetSpr()->FCX -190 ;
			GY = Sprite[BRArm]->GetSpr()->FCY + 36;
			PosRoll(GX, GY,Sprite[BRArm]->GetSpr()->FCX,Sprite[BRArm]->GetSpr()->FCY,GunR);
			FOwner->Add(new TShotFireLarge(FOwner,RoundOff(GX),RoundOff(GY),2));

			GunR = Sprite[BLArm]->Rdegree;
			GX = Sprite[BLArm]->GetSpr()->FCX -190 ;
			GY = Sprite[BLArm]->GetSpr()->FCY + 36;
			PosRoll(GX, GY,Sprite[BRArm]->GetSpr()->FCX,Sprite[BRArm]->GetSpr()->FCY,GunR);
			FOwner->Add(new TShotFireLarge(FOwner,RoundOff(GX),RoundOff(GY),2));
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WATank_shot, 9);
		}break;
	case 5:
		{
			GX = Sprite[BHip]->GetSpr()->FCX + 48;
			GY = Sprite[BHip]->GetSpr()->FCY - 48;
			FOwner->Add(new TZakoHomingCore3(FOwner,RoundOff(GX),RoundOff(GY),-1024,5,0.5,1));
			FOwner->Add(new TMissileFlash(FOwner,RoundOff(GX),RoundOff(GY)));
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WABeam,9);
		}break;
	}
}

void TStage5Boss1::ShotMotion()
{
	switch( ActFlg[KACTION_SHOT] ){
	case 1:
		{
			Inc(ActFlg[KACTION_SHOT]);
			ActCount[KACTION_SHOT]  =  -1;
			if( X<500 )
				ActFlg[KACTION_TRANSFER] = 1;
			DestX = 550;
			DestY = Y;
		}break;
		//
	case 2:
		{
			Transfer(DestX,DestY,32);

			if( ActCount[KACTION_TRANSFER] > 1000 ) {

				Inc(ActFlg[KACTION_SHOT]);
				ActCount[KACTION_SHOT]  =  -1;
			}
		}break;
	case 3:
		{
			Aim(FOwner->HeroPos.X,FOwner->HeroPos.Y, 0);
			Transfer_NoMotion(DestX,DestY,32);
			MotionControl(KMOTION_STAGE5BOSS1_SHOT1);

			if( (ActCount[KACTION_SHOT] % 40 == 0) && (FOwner->Difficulty != KDIFFICULTY_EASY) ) MakeShot(5);

			if( ActCount[KACTION_SHOT] > 30 ) {
				Inc(ActFlg[KACTION_SHOT]);
				ActCount[KACTION_SHOT]  =  -1;
			}
		}break;
	case 4:
		{
			Aim(FOwner->HeroPos.X,FOwner->HeroPos.Y,0);
			MotionControl(KMOTION_STAGE5BOSS1_SHOT1);

			if( ActCount[KACTION_SHOT] % 4 == 0 ) MakeShot(0);
			if( (ActCount[KACTION_SHOT] % 40 == 0) && (FOwner->Difficulty != KDIFFICULTY_EASY) ) MakeShot(5);

			if( ActCount[KACTION_SHOT] > ShotTime ) {
				Inc(ActFlg[KACTION_SHOT]);
				ActCount[KACTION_SHOT]  =  -1;
			}
		}break;
	case 5:
		{
			MotionControl(KMOTION_STAGE5BOSS1_SHOT1);
			if( ActCount[KACTION_SHOT] > 20 ) {
				Inc(ActFlg[KACTION_SHOT]);
				ActCount[KACTION_SHOT]  =  -1;
			}
		}break;
	case 6:
		{
			MotionControl(KMOTION_STAGE5BOSS1_DEFAULT);
			if( ActCount[KACTION_SHOT] > 20 ) {
				Inc(ShotCount);

				if( ShotCount<3 ) {
					ActFlg[KACTION_SHOT] = 1;
					ActCount[KACTION_SHOT]  =  -1;
				}
				else {
					ShotCount = 0;
					SetNextState(KACTION_SHOT);
				}
			}
		}break;
	}
	Inc(ActCount[KACTION_SHOT]);

}

void TStage5Boss1::SpreadMotion()
{
	switch( ActFlg[KACTION_SPREAD] ){
	case 1:
		{
			Inc(ActFlg[KACTION_SPREAD]);
			ActCount[KACTION_SPREAD]  =  -1;
			if( X<500 )
				ActFlg[KACTION_TRANSFER] = 1;
			DestX = 550;
			DestY = Y;
		}break;
		//
	case 2:
		{
			Transfer(DestX,DestY,32);

			if( ActCount[KACTION_TRANSFER] > 1000 ) {
				AimR(FOwner->GetRand(512)-256, 1);
				//        AimR(-512, 1);
				Inc(ActFlg[KACTION_SPREAD]);
				ActCount[KACTION_SPREAD]  =  -1;
			}
		}break;
	case 3:
		{
			Transfer_NoMotion(DestX,DestY,32);
			MotionControl(KMOTION_STAGE5BOSS1_SHOT2);

			if( ActFlg[KACTION_TRANSFER] == 0 ) {

				Inc(ActFlg[KACTION_SPREAD]);
				ActCount[KACTION_SPREAD]  =  -1;
			}
		}break;
	case 4:
		{
			MotionControl(KMOTION_STAGE5BOSS1_SHOT2);

			if( ActCount[KACTION_SPREAD] > 30 ) {
				MakeShot(1);
				Inc(ActFlg[KACTION_SPREAD]);
				ActCount[KACTION_SPREAD]  =  -1;
			}
		}break;
	case 5:
		{
			MotionControl(KMOTION_STAGE5BOSS1_SHOT2);
			if( ActCount[KACTION_SPREAD] > 20 ) {
				Inc(ActFlg[KACTION_SPREAD]);
				ActCount[KACTION_SPREAD]  =  -1;
			}
		}break;
	case 6:
		{
			MotionControl(KMOTION_STAGE5BOSS1_DEFAULT);
			if( ActCount[KACTION_SPREAD] > 40 ) {
				Inc(ShotCount);

				if( ShotCount<3 ) {
					ActFlg[KACTION_SPREAD] = 1;
					ActCount[KACTION_SPREAD]  =  -1;
				}
				else
					SetNextState(KACTION_SPREAD);
			}
		}break;
	}
	Inc(ActCount[KACTION_SPREAD]);

}

void TStage5Boss1::RetreatMotion()
{
	switch( ActFlg[KACTION_RETREAT] ){
		case 1:
			{
			MotionControl(KMOTION_STAGE5BOSS1_MOVE);
			if( ActCount[KACTION_RETREAT] > 20 ) {
				Inc(ActFlg[KACTION_RETREAT]);
				ActCount[KACTION_RETREAT]  =  -1;
				ActFlg[KACTION_TRANSFER] = 1;
				DestX = -640;
				DestY = Y;
			}

		}break;
		//
		case 2:
			{
			Transfer(DestX,DestY,32);

			if( X <-200 ) {
				Inc(ActFlg[KACTION_RETREAT]);
				ActCount[KACTION_RETREAT]  =  -1;
			}
		}break;
		case 3:
			{
			Die();
			Dec(FOwner->MShipHP,MSHPMax / 2);
			FOwner->Add(new TStage5Boss2(FOwner,400,-200));
			FOwner->Add(new TMotherShipDamaged(FOwner, X, Y));
		}break;
	}
	Inc(ActCount[KACTION_RETREAT]);

}

void TStage5Boss1::Fall()
{
	switch( ActFlg[KACTION_FALL] ){
		//初期化
		case 1:
			{
			FallSpeed = 0;
			for( u32 i  =  0 ; i < numSpr; i++) {
				//あたり判定を消す
				Sprite[i]->GetSpr()->FHit = false;
			}

			HPGauge->Hide();
			FOwner->Add(new TExplarge(FOwner,X,Y,1,true));
			if( FOwner->IsSequence() ) FOwner->RequestDelBullets();
			CalcBonus();

			Inc(ActFlg[KACTION_FALL]);
			ActCount[KACTION_FALL]  =  -1;
		}break;
		//
		case 2:{
			if( ActFlg[KACTION_JUMP]<=1 ) {
				MotionControl(KMOTION_STAGE5BOSS1_SITDOWN);
				Sprite[Sprite_Center]->MoveR(FallSpeed,0);
				FallSpeed = FallSpeed-0.2f;
			}

			if( ActCount[KACTION_FALL] % 5 == 0 )
				FOwner->Add(new TExpSmall(FOwner,X+random(256)-128,Y+random(256)-128,0,0,1));//爆発

			if( X<-200 ) {
				Die();
				FOwner->Add(new TStage5Boss2(FOwner,400,-200));
			}

		}break;
	}

	Inc(ActCount[KACTION_FALL]);

}

void TStage5Boss1::Search()
{

}

void TStage5Boss1::Aim( s32 _x, s32 _y, s32 _pat )
{
	f32 GX,GY;   //発射する座標
	s32 DX,DY;  //自機との角度計算に使用
	s32 GunR;   //発射角度
	switch( _pat ){
		case 0:
			{
			if( FOwner->IsSequence() )
				GX = Sprite[BLArm]->GetSpr()->FCX;      //自機が近寄りすぎると方針がブルブルする問題に対処
			else
				GX = Sprite[BRArm]->GetSpr()->FCX - 190;

			GY = Sprite[BLArm]->GetSpr()->FCY + 36;
			PosRoll(GX, GY,Sprite[BLArm]->GetSpr()->FCX,Sprite[BLArm]->GetSpr()->FCY,Sprite[BLArm]->Rdegree);

			DX = RoundOff(_x-GX);
			DY = RoundOff(_y-GY);
			GunR = (RoundOff(atan2(DY,DX)/(f32)M_PI*2048) + 2048) & 0xfff;

			if( (GunR <= 3328) && (GunR >= 1024) ) GunR  =  1024;

			FOwner->MotionDataList[KMOTION_STAGE5BOSS1_SHOT1].Sprites[BLArm].Rdeg  =  GunR;
			FOwner->MotionDataList[KMOTION_STAGE5BOSS1_SHOT1].Sprites[BLShoulder].Rdeg  =  GunR+512;
			EnemyDeg = GunR;
		}break;
		case 1:
			{
			TargetX = _x;
			TargetY = _y;

			GX = Sprite[BRArm]->GetSpr()->FCX - 190;
			GY = Sprite[BRArm]->GetSpr()->FCY + 36;
			PosRoll(GX, GY,Sprite[BRArm]->GetSpr()->FCX,Sprite[BRArm]->GetSpr()->FCY,Sprite[BRArm]->Rdegree);

			DX = RoundOff(_x-GX);
			DY = RoundOff(_y-GY);
			GunR = (RoundOff(atan2(DY,DX)/(f32)M_PI*2048) + 2048) & 0xfff;

			FOwner->MotionDataList[KMOTION_STAGE5BOSS1_SHOT2].Sprites[BRArm].Rdeg  =  GunR;
			FOwner->MotionDataList[KMOTION_STAGE5BOSS1_SHOT2].Sprites[BRShoulder].Rdeg  =  GunR+512;
			EnemyDeg = GunR;
		}break;
	}

}

void TStage5Boss1::AimR( s32 _r, s32 _pat )
{
	_r  =  _r & 0xfff;
	switch( _pat ){
	case 0:
		{
			FOwner->MotionDataList[KMOTION_STAGE5BOSS1_SHOT1].Sprites[BLArm].Rdeg  =  _r;
			FOwner->MotionDataList[KMOTION_STAGE5BOSS1_SHOT1].Sprites[BLShoulder].Rdeg  =  _r;
			EnemyDeg = _r;
		}break;
	case 1:
		{
			FOwner->MotionDataList[KMOTION_STAGE5BOSS1_SHOT2].Sprites[BRArm].Rdeg  =  _r;
			FOwner->MotionDataList[KMOTION_STAGE5BOSS1_SHOT2].Sprites[BRShoulder].Rdeg  =  _r+512;
			EnemyDeg = _r;
		}break;
	}

}

void TStage5Boss1::BodyAttack()
{
	switch( ActFlg[KACTION_BODYATTACK] ){
		case 1:
			{
			MotionControl(KMOTION_STAGE5BOSS1_MOVE);
			if( ActCount[KACTION_BODYATTACK] > 40 ) {
				Inc(ActFlg[KACTION_BODYATTACK]);
				ActCount[KACTION_BODYATTACK]  =  -1;
				ActFlg[KACTION_TRANSFER] = 1;
				DestX = 200;
				DestY = Y;
				FOwner->Quake(4,60);
			}

		}break;
		//
		case 2:
			{
			Transfer(DestX,DestY,64);

			if( ActCount[KACTION_BODYATTACK] % 10 == 0 )
				FOwner->Add(new TSplashDebris(FOwner, X-176, 470));

			if( ActCount[KACTION_BODYATTACK] % 20 == 0 )  
				if( CreateSE ) FOwner->SoundEffect->WavPlay(WATank_dash, 10);

			if( ActFlg[KACTION_TRANSFER] == 0 ) {
				Inc(ActFlg[KACTION_BODYATTACK]);
				ActCount[KACTION_BODYATTACK]  =  -1;
			}
		}break;
		case 3:
			{
			MotionControl(KMOTION_STAGE5BOSS1_DEFAULT);

			if( ActCount[KACTION_BODYATTACK] > 50 ) {
				SetNextState(KACTION_BODYATTACK);
			}
		}
	}
	Inc(ActCount[KACTION_BODYATTACK]);

}

void TStage5Boss1::ShotMotion2()
{
	switch( ActFlg[KACTION_SHOT2] ){
		case 1:
			{
			Inc(ActFlg[KACTION_SHOT2]);
			ActCount[KACTION_SHOT2]  =  -1;
			ShotCount = 0;
			ActFlg[KACTION_TRANSFER] = 1;
			DestX = 550;
			DestY = Y;
		}break;
		//
		case 2:
			{
			Transfer(DestX,DestY,24);

			if( ActCount[KACTION_TRANSFER] > 500 ) {
				EnemyDeg = -512;
				AimR(EnemyDeg,0);
				Inc(ActFlg[KACTION_SHOT2]);
				ActCount[KACTION_SHOT2]  =  -1;
			}
		}break;
		case 3:
			{
			MotionControl(KMOTION_STAGE5BOSS1_SHOT1);
			Transfer_NoMotion(DestX,DestY,24);

			if( (ActCount[KACTION_SHOT2] % 40 == 0) && (FOwner->Difficulty == KDIFFICULTY_HARD) ) MakeShot(5);

			if( ActCount[KACTION_SHOT2] % 10 == 0 ) {
				MakeShot(2);
				Inc(EnemyDeg,128);
				Inc(ShotCount);
				AimR(EnemyDeg,0);
			}

			if( ShotCount > 8 ) {
				ShotCount = 0;
				Inc(ActFlg[KACTION_SHOT2]);
				ActCount[KACTION_SHOT2]  =  -1;
			}
		}break;
		case 4:
			{
			MotionControl(KMOTION_STAGE5BOSS1_SHOT1);
			Transfer_NoMotion(DestX,DestY,24);

			if( (ActCount[KACTION_SHOT2] % 40 == 0) && (FOwner->Difficulty == KDIFFICULTY_HARD) ) MakeShot(5);

			if( ActCount[KACTION_SHOT2] % 10 == 0 ) {
				MakeShot(2);
				Dec(EnemyDeg,128);
				Inc(ShotCount);
				AimR(EnemyDeg,0);
			}

			if( ShotCount > 8 ) {
				ShotCount = 0;
				Inc(ActFlg[KACTION_SHOT2]);
				ActCount[KACTION_SHOT2]  =  -1;
			}
		}break;
		case 5:
			{
			if( ActCount[KACTION_SHOT2] > 20 ) {
				Inc(ActFlg[KACTION_SHOT2]);
				ActCount[KACTION_SHOT2]  =  -1;
			}
		}break;
		case 6:
			{
			MotionControl(KMOTION_STAGE5BOSS1_DEFAULT);
			if( ActCount[KACTION_SHOT2] > 20 ) {
				ShotCount = 0;
				SetNextState(KACTION_SHOT2);
			}
		}break;
	}
	Inc(ActCount[KACTION_SHOT2]);

}

void TStage5Boss1::JumpMotion()
{
	switch( ActFlg[KACTION_JUMP] ){
		case 1:
			{
			MotionControl(KMOTION_STAGE5BOSS1_SITDOWN);
			if( ActCount[KACTION_JUMP] > 20 ) {
				if( ActFlg[KACTION_FALL]>1 ) {
					ActFlg[KACTION_JUMP] = 0;
					ActCount[KACTION_JUMP]  =  -1;
				}
				else {
					Inc(ActFlg[KACTION_JUMP]);
					ActCount[KACTION_JUMP]  =  -1;
					JumpSpeed = -30;
				}

				//        FirstY = Y;
			}

		}break;
		//
		case 2:
			{
			Sprite[Sprite_Center]->MoveR(-6,JumpSpeed);
			JumpSpeed = JumpSpeed+1;
			MotionControl(KMOTION_STAGE5BOSS1_JUMP);

			if( (ActCount[KACTION_JUMP] > 5) && (JumpSpeed>30) ) {
				Inc(ActFlg[KACTION_JUMP]);
				ActCount[KACTION_JUMP]  =  -1;
				FOwner->Add(new TSplashDebris(FOwner, X-176, 470));
				FOwner->Add(new TSplashDebris(FOwner, X+176, 470));
				FOwner->Quake(4,60);
				if( CreateSE ) FOwner->SoundEffect->WavPlay(WATank_jump, 10);
			}
		}break;
		case 3:
			{
			MotionControl(KMOTION_STAGE5BOSS1_SITDOWN);

			if( ActCount[KACTION_JUMP] > 50 ) {
				if( ActFlg[KACTION_FALL]>1 ) {
					ActFlg[KACTION_JUMP] = 0;
					ActCount[KACTION_JUMP]  =  -1;
				}
				else {
					Inc(ActFlg[KACTION_JUMP]);
					ActCount[KACTION_JUMP]  =  -1;
					ActFlg[KACTION_TRANSFER] = 1;
					DestX = 550;
					DestY = Y;
				}
			}
		}break;
		case 4:
			{
			Transfer(DestX,DestY,64);

			if( ActFlg[KACTION_TRANSFER] == 0 ) {
				Inc(ActFlg[KACTION_JUMP]);
				ActCount[KACTION_JUMP]  =  -1;
			}
		}break;
		case 5:
			{
			MotionControl(KMOTION_STAGE5BOSS1_DEFAULT);

			if( ActCount[KACTION_JUMP] > 50 ) {
				SetNextState(KACTION_JUMP);
			}
		}break;
	}
	Inc(ActCount[KACTION_JUMP]);

}

void TStage5Boss1::RainMotion()
{
	switch( ActFlg[KACTION_RAIN] ){
		case 1:
			{
			MotionControl(KMOTION_STAGE5BOSS1_SHOT3);

			if( ActCount[KACTION_RAIN] > 40 ) {
				Inc(ActFlg[KACTION_RAIN]);
				ActCount[KACTION_RAIN]  =  -1;
			}
		}break;
		//
		case 2:
			{
			MotionControl(KMOTION_STAGE5BOSS1_SHOT3);

			if( ActCount[KACTION_RAIN] % 3 == 0 ) {
				Sprite[BLShoulder]->RotateR(-32);
				Sprite[BRShoulder]->RotateR(-32);
				MakeShot(4);
			}

			if( ActCount[KACTION_RAIN] == 60 )
				FOwner->Add(new TBeamRain(FOwner, 0,0,240, 5));

			if( ActCount[KACTION_RAIN] > 180 ) {
				Inc(ActFlg[KACTION_RAIN]);
				ActCount[KACTION_RAIN]  =  -1;
			}
		}break;
		case 3:
			{
			MotionControl(KMOTION_STAGE5BOSS1_DEFAULT);
			if( ActCount[KACTION_RAIN] > 120 ) {
				SetNextState(KACTION_RAIN);
			}
		}break;
	}
	Inc(ActCount[KACTION_RAIN]);

}

TStage5Boss1::TStage5Boss1( TOBJList* owner, s32 _x, s32 _y ) : TBossBase(owner)
{
	s32 def;
	switch( owner->Difficulty ){
	case KDIFFICULTY_SINGLE:
		{
			def = 10;
		}break;
	default: {
		def = 20;
	}
	}

	TBossBase::Init(_x, _y, KSTR_SHOT*256, def);

	FirstY = _y;
	ConstForeground = 16;
	Sprite_Center = BLLeg2;
	CommandRed = true;
	Score = 100000;
	ComBonus = 5000;
	RetreatDistance = 18000;
	if( FOwner->Difficulty=KDIFFICULTY_HARD )
		ShotTime = 60;
	else
		ShotTime = 40;

	Motion_Default = KMOTION_STAGE5BOSS1_DEFAULT;
	Motion_Foward = KMOTION_STAGE5BOSS1_MOVE;
	Motion_Back = KMOTION_STAGE5BOSS1_MOVE;

	MakeSprFromID(FOwner->GetDG(), KSPRDATA_STAGE5BOSS1, Enemy4Tex, 16000, ExtraEffects);
	if( ExtraEffects ) {
		for( u32 i  =  ConstForeground ; i < numSpr; i++) {
			Sprite[i]->GetSpr()->SetColor(16,ctAlpha);
		}
	}

	MotionControlSoon(KMOTION_STAGE5BOSS1_DEFAULT);

	ActFlg[KACTION_APPEAR] = 1;

	Shadow  =  new TBossShadow(FOwner, Sprite[BHip], X+64, 480, -200, 300);
	FOwner->Add(Shadow);

}

void TStage5Boss1::Move()
{
	TBossBase::Move();
	X = RoundOff(Sprite[BWaist]->GetSpr()->FCX);
	Y = RoundOff(Sprite[BWaist]->GetSpr()->FCY);
	if( HP>0 ) {
		if( ActFlg[KACTION_SHOT]>0 ) ShotMotion();
		if( ActFlg[KACTION_SHOT2]>0 ) ShotMotion2();
		if( ActFlg[KACTION_SPREAD]>0 ) SpreadMotion();
		if( ActFlg[KACTION_APPEAR]>0 ) AppearEvent();
		if( ActFlg[KACTION_BODYATTACK]>0 ) BodyAttack();
		if( ActFlg[KACTION_RAIN]>0 ) RainMotion();
		if( ActFlg[KACTION_RETREAT]>0 ) RetreatMotion();
	}
	if( ActFlg[KACTION_JUMP]>0 ) JumpMotion();

	//死亡判定
	if( (HP<=0) && (ActFlg[KACTION_FALL]==0) )
		ActFlg[KACTION_FALL] = 1;

	if( ActFlg[KACTION_FALL]>0 ) Fall();

}

void TStage5Boss1::Die()
{
	TBossBase::Die();
	Shadow->Die();

}

void TStage5Boss1::OnDamaged( TOBJ* target )
{
	X = RoundOff(Sprite[BWaist]->GetSpr()->FCX);
	Y = RoundOff(Sprite[BWaist]->GetSpr()->FCY);

	if( target->Kind() == kHero ) {
		if( (! Buzzed) && (target->Hit[HitBuzz]) ) {
			FOwner->Scrach(this);
			Buzzed = true;
			LastBuzz = Age;
		}
		if( (target->Hit[HitBlade]) && (target->LastSlashed > LastSlashed) ) {
			if( Hit[BHead] ) {
				switch( FOwner->Difficulty ){
				case KDIFFICULTY_SINGLE:Dec(HP,target->STR());		break;
				default: {
					if( target->STR()>FDef )
						Dec(HP,target->STR()-FDef);
					else
						Dec(HP,target->STR());
						 }
				}

				for( u32 i = 0 ; i < ConstForeground; i++ )
					Sprite[i]->GetSpr()->SetColor(0,ctRed);
				LastFlash = Age;
			}
			else {
				Dec(HP,target->STR() / 2);
				if( CreateSE ) FOwner->SoundEffect->WavPlay2(WASlash_C,12,60);
			}
			HPGauge->Show();
			GaugeTime = Age;
			Sprite[Sprite_Center]->MoveR(1,0);                  //のけぞる
			Inc(KnockBackVel, 1);
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WAEdamage0,7);
			LastSlashed = target->LastSlashed;
		}
	}


	if( (target->Kind() == kHShot) ) {
		if( Hit[BHead] ) {
			Dec(HP,target->STR());
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WAEdamage0,7);

			KnockBack();
		}
		else {
			//      Dec(HP,target->STR / 2);
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WAEdamage1,7);
		}

	}

	if( (target->Kind() == kMissile) ) {
		Dec(HP,target->STR()-FDef);
		KnockBack();
		if( HP<=0 ) BonusRate = target->BonusRate;
	}

	if( HP<TempHP ) TempHP = HP;

}

void TStage5Boss2::SetNextState( s32 _current )
{
	TBossBase::SetNextState(_current);
	ActFlg[_current] = 0;
	ActCount[_current] = -1;

	switch( _current ){
	case KACTION_APPEAR     : 
		{
			ActFlg[KACTION_SHOT] = 1;
		}break;
	case KACTION_SHOT     : 
		{
			ActFlg[KACTION_HOMING] = 1;
		}break;
	case KACTION_HOMING     : 
		{
			ActFlg[KACTION_LASER] = 1;
		}break;
	case KACTION_LASER     : 
		{
			ActFlg[KACTION_CALL] = 1;
		}break;
	case KACTION_CALL     : 
		{
			ActFlg[KACTION_SHOT] = 1;
		}break;
	}

}

void TStage5Boss2::AppearEvent()
{
	switch( ActFlg[KACTION_APPEAR] ){
		//初期化
	case 1:
		{
			Inc(ActFlg[KACTION_APPEAR]);
			ActCount[KACTION_APPEAR]  =  -1;
		}break;
		//
	case 2:
		{
			DestX = 550;
			DestY = 240;

			Inc(ActFlg[KACTION_APPEAR]);
			ActCount[KACTION_APPEAR]  =  -1;
			ActFlg[KACTION_TRANSFER] = 1;
		}break;
	case 3:
		{
			Transfer(DestX,DestY,32);

			if( ActCount[KACTION_TRANSFER] > 1000 ) {
				Inc(ActFlg[KACTION_APPEAR]);
				ActCount[KACTION_APPEAR]  =  -1;
			}
		}break;
	case 4:
		{
			MotionControl(KMOTION_STAGE5BOSS2_DEFAULT);
			Transfer_NoMotion(DestX,DestY,32);
			if( ActFlg[KACTION_TRANSFER] == 0 ) {
				SetNextState(KACTION_APPEAR);
			}
		}break;
	case 5:
		{

		}break;
	}
	Inc(ActCount[KACTION_APPEAR]);

	Bit[0]->Place(X+30, Y-180,2);
	Bit[1]->Place(X-60, Y-120,2);
	Bit[2]->Place(X-60, Y+120,2);
	Bit[3]->Place(X+30, Y+180,2);

}

void TStage5Boss2::MakeShot( s32 _r )
{
	if( FOwner->Difficulty=KDIFFICULTY_NORMAL )
		FOwner->Add(new TBoss3Shot(FOwner,X,Y,_r,FOwner->GetRand(2)+3,500, BSpeed, 1,0,1));
	else
		FOwner->Add(new TBoss3Shot(FOwner,X,Y,_r,20,500, BSpeed, 1,1,1));
	FOwner->Add(new TFCircle(FOwner,X,Y,1,_r,0));
	if( CreateSE ) FOwner->SoundEffect->WavPlay(WABShot,9);

}

void TStage5Boss2::ShotMotion()
{
	switch( ActFlg[KACTION_SHOT] ){
		case 1:
			{
			for( u32 i = 0; i <= 3; i++ ) {
				s32 bitX = FOwner->GetRand(12)*48;
				s32 bitY = FOwner->GetRand(10)*48;
				Bit[i]->Place(bitX, bitY,24);
			}

			Inc(ActFlg[KACTION_SHOT]);
			ActCount[KACTION_SHOT]  =  -1;
			ActFlg[KACTION_TRANSFER] = 1;
			DestX = 550;
			DestY = Y;
		}break;
		//
		case 2:
			{
			Transfer(DestX,DestY,32);

			for( u32 i = 0; i <= 3; i++ ) {
				Bit[i]->Search();
			}

			if( ActCount[KACTION_SHOT] > 60 ) {
				Inc(ActFlg[KACTION_SHOT]);
				ActCount[KACTION_SHOT]  =  -1;
			}
		}break;
		case 3:
			{
			switch( ActCount[KACTION_SHOT] ){
				case 0:Bit[0]->MakeShot(3);		break;
				case 10:Bit[1]->MakeShot(3);	break;
				case 20:Bit[2]->MakeShot(3);	break;
				case 30:Bit[3]->MakeShot(3);	break;
			}
			if( ActCount[KACTION_SHOT] > 60 ) {
				Inc(ActFlg[KACTION_SHOT]);
				ActCount[KACTION_SHOT]  =  -1;

				if( FOwner->IsSequence() ) {
					DestX = FOwner->HeroPos.X;
					DestY = FOwner->HeroPos.Y;
				}
			}
		}break;
		case 4:
			{
			MotionControl(KMOTION_STAGE5BOSS2_SLASHL1);

			if( ActCount[KACTION_SHOT] > 30 ) {
				Inc(ActFlg[KACTION_SHOT]);
				ActCount[KACTION_SHOT]  =  -1;
				ActFlg[KACTION_TRANSFER] = 1;
				if( (! FOwner->IsSequence()) || (FOwner->Difficulty == KDIFFICULTY_HARD) ) {
					DestX = FOwner->HeroPos.X;
					DestY = FOwner->HeroPos.Y;
				}
			}
		}break;
		case 5:
			{
			Transfer_NoMotion(DestX,DestY,40);

			if( ActCount[KACTION_TRANSFER] > 900 ) {
				Inc(ActFlg[KACTION_SHOT]);
				ActCount[KACTION_SHOT]  =  -1;

				Sprite[BBlade1]->GetSpr()->FHit = true;
				BladeTrace[0] = new TEnemyBladeTrace2(FOwner,Sprite[BBlade1],160,192,2048);
				FOwner->Add(BladeTrace[0]);
				if( CreateSE ) FOwner->SoundEffect->WavPlay(WASwing_E, 8);
			}
		}break;
		case 6:
			{
			Transfer_NoMotion(DestX,DestY,40);

			if( ActCount[KACTION_SHOT] < 5 ) {
				MotionControl(KMOTION_STAGE5BOSS2_SLASHL1_2);
			}
			else {
				MotionControl(KMOTION_STAGE5BOSS2_SLASHL2);
			}

			if( ActFlg[KACTION_TRANSFER] == 0 ) {
				Inc(ActFlg[KACTION_SHOT]);
				ActCount[KACTION_SHOT]  =  -1;
				Sprite[BBlade1]->GetSpr()->FHit = false;

				if( FOwner->IsSequence() ) {
					DestX = FOwner->HeroPos.X;
					DestY = FOwner->HeroPos.Y;
				}
			}
		}break;
		case 7:
			{
			MotionControl(KMOTION_STAGE5BOSS2_SLASHL2);

			//EASYならここで終了NORMAL以上なら2撃目へ
			if( FOwner->Difficulty == KDIFFICULTY_EASY ) {
				if( ActCount[KACTION_SHOT] == 40 ) {
					BladeTrace[0]->Die();
					BladeTrace[0] = NULL;

					for( u32 i = 0; i <= 3; i++ )
						Bit[i]->MakeShot(-1);

					Inc(ShotCount);
					if( ShotCount>2 ) {
						SetNextState(KACTION_SHOT);
						ShotCount = 0;
					}
					else {
						ActFlg[KACTION_SHOT] = 1;
						ActCount[KACTION_SHOT]  =  -1;
					}
				}
			}
			else {
				if( ActCount[KACTION_SHOT] == 10 ) {
					BladeTrace[0]->Die();
					BladeTrace[0] = NULL;
					Inc(ActFlg[KACTION_SHOT]);
					ActCount[KACTION_SHOT]  =  -1;
				}
			}
		}break;
		case 8:
			{
			MotionControl(KMOTION_STAGE5BOSS2_SLASHR1);

			if( ActCount[KACTION_SHOT] > 5 ) {
				Inc(ActFlg[KACTION_SHOT]);
				ActCount[KACTION_SHOT]  =  -1;
				ActFlg[KACTION_TRANSFER] = 1;
				if( (! FOwner->IsSequence()) || (FOwner->Difficulty == KDIFFICULTY_HARD) ) {
					DestX = FOwner->HeroPos.X;
					DestY = FOwner->HeroPos.Y;
				}
			}
		}break;
		case 9:
			{
			Transfer_NoMotion(DestX,DestY,40);

			if( ActCount[KACTION_TRANSFER] > 1100 ) {
				Inc(ActFlg[KACTION_SHOT]);
				ActCount[KACTION_SHOT]  =  -1;

				Sprite[BBlade2]->GetSpr()->FHit = true;
				BladeTrace[1] = new TEnemyBladeTrace2(FOwner,Sprite[BBlade2],160,192,2048);
				FOwner->Add(BladeTrace[1]);
				if( CreateSE ) FOwner->SoundEffect->WavPlay(WASwing_E, 8);
			}
		}break;
		case 10:{
			Transfer_NoMotion(DestX,DestY,40);

			if( ActCount[KACTION_SHOT] < 5 ) {
				MotionControl(motionid_sla1);
			}
			else {
				MotionControl(motionid_sla2);
			}

			if( ActFlg[KACTION_TRANSFER] == 0 ) {
				Inc(ActFlg[KACTION_SHOT]);
				ActCount[KACTION_SHOT]  =  -1;
				Sprite[BBlade2]->GetSpr()->FHit = false;
			}
		}break;
		case 11:{
			MotionControl(motionid_sla2);

			if( ActCount[KACTION_SHOT] == 10 ) {
				BladeTrace[1]->Die();
				BladeTrace[1] = NULL;

				Inc(ShotCount);
				if( ShotCount>2 ) {
					SetNextState(KACTION_SHOT);
					ShotCount = 0;
				}
				else {
					ActFlg[KACTION_SHOT] = 1;
					ActCount[KACTION_SHOT]  =  -1;
				}
			}
		}break;
	}
	Inc(ActCount[KACTION_SHOT]);

}

void TStage5Boss2::RetreatMotion()
{
	switch( ActFlg[KACTION_RETREAT] ){
		//初期化
		case 0:
			{

			Inc(ActFlg[KACTION_RETREAT]);
			ActCount[KACTION_RETREAT]  =  -1;

			ActFlg[KACTION_TRANSFER] = 1;
			DestX = -400;
			DestY = Y;
		}break;
		//
		case 1:
			{
			Transfer(DestX,DestY,16);

			if( X<-200 ) {
				Die();
				Dec(FOwner->MShipHP,MSHPMax / 2);
				FOwner->Add(new TMotherShipDamaged(FOwner, X, Y));
			}
		}break;
	}
	Inc(ActCount[KACTION_RETREAT]);
	Bit[0]->Place(X+30, Y-180,2);
	Bit[1]->Place(X-60, Y-120,2);
	Bit[2]->Place(X-60, Y+120,2);
	Bit[3]->Place(X+30, Y+180,2);

}

void TStage5Boss2::Fall()
{
	switch( ActFlg[KACTION_FALL] ){
		//初期化
		case 1:
			{

			for( u32 i  =  0 ; i < numSpr; i++) {
				//あたり判定を消す
				Sprite[i]->GetSpr()->FHit = false;
			}

			HPGauge->Hide();
			FOwner->Add(new TExplarge(FOwner,X,Y,1,true));

			CalcBonus();

			for( u32 i = 0; i <= 3; i++ ) {
				Bit[i]->DieWithExplosion();
				Bit[i] = NULL;
			}
			if( FOwner->IsSequence() ) FOwner->RequestDelBullets();
			Inc(ActFlg[KACTION_FALL]);
			ActCount[KACTION_FALL]  =  -1;
		}break;
		//
		case 2:
			{
			MotionControl(KMOTION_STAGE5BOSS2_FALL1);

			if( ActCount[KACTION_FALL]>120 ) {
				Inc(ActFlg[KACTION_FALL]);
				ActCount[KACTION_FALL]  =  -1;
			}
		}break;
		case 3:
			{
			MotionControl(KMOTION_STAGE5BOSS2_FALL2);

			if( ActCount[KACTION_FALL]>180 ) {
				FOwner->Add(new TExplarge(FOwner,X,Y,1,true));
				Die();

				if( FOwner->Distance<38000 ) FOwner->Add(new TZ02_3Gene(FOwner, 0));
			}
		}break;
	}

	//炎上
	if( Age % 2 == 0 ) {
		FOwner->Add(new TMotherShipSmoke(FOwner,X,Y,-18,-0.2, 8, 4));
		FOwner->Add(new TMotherShipFire(FOwner,X,Y,-18,-0.2, 8, 4));
	}

	//振動
	if( Age % 2 == 0 )
		Sprite[BHip]->MoveR(0,-4);
	else
		Sprite[BHip]->MoveR(0,4);

	Inc(ActCount[KACTION_FALL]);


}

void TStage5Boss2::Search()
{

}

void TStage5Boss2::Aim( s32 _x, s32 _y )
{

}

void TStage5Boss2::MakeSpread()
{

}

void TStage5Boss2::HomingMotion()
{
	switch( ActFlg[KACTION_HOMING] ){
		case 1:
			{
			Bit[0]->Place(480, 48,24);
			Bit[1]->Place(480, 144,24);
			Bit[2]->Place(480, 336,24);
			Bit[3]->Place(480, 432,24);

			for( u32 i = 0; i <= 3; i++ )
				Bit[i]->Sprite[0]->Rotate(0);

			Inc(ActFlg[KACTION_HOMING]);
			ActCount[KACTION_HOMING]  =  -1;
			ActFlg[KACTION_TRANSFER] = 1;
			DestX = 550;
			DestY = 240;
			ShotCount = 0;
			ZakoCount = 0;
			RepeatCount = 0;
		}break;
		//
		case 2:
			{
			Transfer(DestX,DestY,32);

			if( ActCount[KACTION_HOMING] > 120 ) {
				Inc(ActFlg[KACTION_HOMING]);
				ActCount[KACTION_HOMING]  =  -1;
				for( u32 i = 0; i <= 3; i++ )
					Bit[i]->MakeShot(4);
			}
		}break;
		case 3:
			{
			Inc(ActFlg[KACTION_HOMING]);
			ActCount[KACTION_HOMING]  =  -1;
			ActFlg[KACTION_TRANSFER] = 1;
			DestX = FOwner->GetRand(3)*50 + 450;

			if( Y > 240 )
				DestY = 100;
			else
				DestY = 380;

			TargetDeg  = GetRfromArcTan(X,Y,FOwner->HeroPos.X,FOwner->HeroPos.Y);
		}break;
		//
		case 4:
			{
				Transfer(DestX,DestY,32);

				s32 maxshot;
				switch( FOwner->Difficulty ){
				case KDIFFICULTY_NORMAL:maxshot = 4;		break;
				default:
					maxshot = 5;
				}

			if( (ActCount[KACTION_HOMING] % 10 == 0) && (ShotCount <= maxshot-1) ) {
				if( FOwner->Difficulty != KDIFFICULTY_EASY )  MakeShot(TargetDeg);
				Inc(ShotCount);
			}

			if( ActFlg[KACTION_TRANSFER] == 0 ) {
				Inc(ActFlg[KACTION_HOMING]);
				ActCount[KACTION_HOMING]  =  -1;
				ShotCount = 0;
			}
		}break;
		case 5:
			{
			if( ActCount[KACTION_HOMING] > 30 ) {
				Inc(RepeatCount);
				if( RepeatCount>3 ) {
					SetNextState(KACTION_HOMING);
					RepeatCount = 0;
				}
				else {
					ActFlg[KACTION_HOMING] = 3;
					ActCount[KACTION_HOMING]  =  -1;
				}
			}
		}break;
	}
	Inc(ActCount[KACTION_HOMING]);

	if( (Age % 15 == 0) && (ActFlg[KACTION_HOMING] >= 3) ) {
		if( ZakoCount<24 ) {
			FOwner->Add(new TZako2_06(FOwner,320,530,1024,8,0,false));
			FOwner->Add(new TZako2_06(FOwner,270,-30,-1024,8,0,false));
		}
		else if( ZakoCount=24 ) {
			FOwner->Add(new TZako2_06(FOwner,320,530,1024,8,0,true));
			FOwner->Add(new TZako2_06(FOwner,270,-30,-1024,8,0,true));
		}
		Inc(ZakoCount);
	}

}

void TStage5Boss2::LaserMotion()
{
	s32 waittime;
	s32 swingtime;
	s32 shotamount;
	s32 shotinterval;
	switch( FOwner->Difficulty ){
	case KDIFFICULTY_EASY:{ waittime = 40;  swingtime = 900; shotamount = 1; shotinterval = 40; }	break;
	case KDIFFICULTY_HARD:{ waittime = 10;  swingtime = 1100; shotamount = 3; shotinterval = 30; }	break;
	default:
		{
			waittime = 10;
			swingtime = 1100;
			shotamount = 3;
			shotinterval = 40;
		}
	}


	switch( ActFlg[KACTION_LASER] ){
		case 1:
			{
			Inc(ActFlg[KACTION_LASER]);
			ActCount[KACTION_LASER]  =  -1;
			ActFlg[KACTION_TRANSFER] = 1;
			DestX = 550;
			DestY = 240;
		}break;
		//
		case 2:
			{
			Transfer(DestX,DestY,32);

			for( u32 i = 0; i <= 3; i++ ) {
				s32 bitX =  RoundOff((f32)X - 120.f * cost[i*1024]);
				s32 bitY =  RoundOff((f32)Y - 120.f * sint[i*1024]);
				Bit[i]->Place(bitX, bitY,12);
				Bit[i]->Sprite[0]->Rotate(i*1024);
			}

			if( ActCount[KACTION_LASER] > 60 ) {
				Inc(ActFlg[KACTION_LASER]);
				ActCount[KACTION_LASER]  =  -1;
			}
		}break;
		case 3:
			{

			if( ActCount[KACTION_LASER] > 120 ) {
				Inc(ActFlg[KACTION_LASER]);
				ActCount[KACTION_LASER]  =  -1;
			}
		}break;
		case 4:
			{
			MotionControl(KMOTION_STAGE5BOSS2_SLASHL1);

			if( FOwner->IsSequence() && (ActCount[KACTION_LASER] == 0) ) {
				DestX = FOwner->HeroPos.X;
				DestY = FOwner->HeroPos.Y;
			}

			if( ActCount[KACTION_LASER] > 30 ) {
				Inc(ActFlg[KACTION_LASER]);
				ActCount[KACTION_LASER]  =  -1;
				ActFlg[KACTION_TRANSFER] = 1;
				if( (! FOwner->IsSequence()) || (FOwner->Difficulty == KDIFFICULTY_HARD) ) {
					DestX = FOwner->HeroPos.X;
					DestY = FOwner->HeroPos.Y;
				}
			}
		}break;
		case 5:
			{
			Transfer_NoMotion(DestX,DestY,40);

			if( ActCount[KACTION_TRANSFER] > 900 ) {
				Inc(ActFlg[KACTION_LASER]);
				ActCount[KACTION_LASER]  =  -1;

				Sprite[BBlade1]->GetSpr()->FHit = true;
				BladeTrace[0] = new TEnemyBladeTrace2(FOwner,Sprite[BBlade1],160,192,2048);
				FOwner->Add(BladeTrace[0]);
				if( CreateSE ) FOwner->SoundEffect->WavPlay(WASwing_E, 8);
			}
		}break;
		case 6:
			{
			Transfer_NoMotion(DestX,DestY,40);

			if( ActCount[KACTION_LASER] < 5 ) {
				MotionControl(KMOTION_STAGE5BOSS2_SLASHL1_2);
			}
			else {
				MotionControl(KMOTION_STAGE5BOSS2_SLASHL2);
			}

			if( ActFlg[KACTION_TRANSFER] == 0 ) {
				Inc(ActFlg[KACTION_LASER]);
				ActCount[KACTION_LASER]  =  -1;
				Sprite[BBlade1]->GetSpr()->FHit = false;
				if( FOwner->IsSequence() ) {
					DestX = FOwner->HeroPos.X;
					DestY = FOwner->HeroPos.Y;
				}
			}
		}break;
		case 7:
			{
			MotionControl(KMOTION_STAGE5BOSS2_SLASHL2);

			if( ActCount[KACTION_LASER] == waittime ) {
				BladeTrace[0]->Die();
				BladeTrace[0] = NULL;
				Inc(ActFlg[KACTION_LASER]);
				ActCount[KACTION_LASER]  =  -1;
			}
		}break;
		case 8:
			{
			MotionControl(KMOTION_STAGE5BOSS2_SLASHR1);

			if( ActCount[KACTION_LASER] > 5 ) {
				Inc(ActFlg[KACTION_LASER]);
				ActCount[KACTION_LASER]  =  -1;
				ActFlg[KACTION_TRANSFER] = 1;
				if( (! FOwner->IsSequence()) || (FOwner->Difficulty == KDIFFICULTY_HARD) ) {
					DestX = FOwner->HeroPos.X;
					DestY = FOwner->HeroPos.Y;
				}
			}
		}break;
		case 9:
			{
			Transfer_NoMotion(DestX,DestY,40);

			if( ActCount[KACTION_TRANSFER] > swingtime ) {
				Inc(ActFlg[KACTION_LASER]);
				ActCount[KACTION_LASER]  =  -1;

				Sprite[BBlade2]->GetSpr()->FHit = true;
				BladeTrace[1] = new TEnemyBladeTrace2(FOwner,Sprite[BBlade2],160,192,2048);
				FOwner->Add(BladeTrace[1]);
				if( CreateSE ) FOwner->SoundEffect->WavPlay(WASwing_E, 8);
			}
		}break;
		case 10:
			{
			Transfer_NoMotion(DestX,DestY,40);

			if( ActCount[KACTION_LASER] < 5 ) {
				MotionControl(motionid_sla1);
			}
			else {
				MotionControl(motionid_sla2);
			}

			if( ActFlg[KACTION_TRANSFER] = 0 ) {
				Inc(ActFlg[KACTION_LASER]);
				ActCount[KACTION_LASER]  =  -1;
				Sprite[BBlade2]->GetSpr()->FHit = false;
			}
		}break;
		case 11:
			{
			MotionControl(motionid_sla2);

			if( ActCount[KACTION_LASER] = waittime ) {
				BladeTrace[1]->Die();
				BladeTrace[1] = NULL;

				Inc(ShotCount);
				if( ShotCount>3 ) {
					SetNextState(KACTION_LASER);
					ShotCount = 0;
				}
				else {
					ActFlg[KACTION_LASER] = 4;
					ActCount[KACTION_LASER]  =  -1;
				}
			}
		}break;
	}
	Inc(ActCount[KACTION_LASER]);

	if( ActFlg[KACTION_LASER] >=3 ) {
		RotateBit();
		if( Age % shotinterval == 0 ) {
			for( u32 i  =  0 ; i <= shotamount; i++ ) {
				Bit[i]->MakeShot(5);
			}
		}
	}

}

void TStage5Boss2::RotateBit()
{
	for( u32 i = 0; i <= 3; i++ ) {
		Bit[i]->Sprite[0]->RotateR(-8);
		s32 bitX =  RoundOff((f32)X - 120.f * cost[Bit[i]->Sprite[0]->Rdegree]);
		s32 bitY =  RoundOff((f32)Y - 120.f * sint[Bit[i]->Sprite[0]->Rdegree]);
		Bit[i]->Place(bitX, bitY,2);
	}

}

void TStage5Boss2::CallZako()
{
	switch( ActFlg[KACTION_CALL] ){
		case 1:{
			Inc(ActFlg[KACTION_CALL]);
			ActCount[KACTION_CALL]  =  -1;
			ActFlg[KACTION_TRANSFER] = 1;
			DestX = 550;
			DestY = 240;
			ZakoCount = 0;
		}break;
		//
		case 2:{
			Transfer(DestX,DestY,32);

			if( ActCount[KACTION_CALL] > 120 ) {
				Inc(ActFlg[KACTION_CALL]);
				ActCount[KACTION_CALL]  =  -1;
			}
		}break;
		case 3:{
			Transfer(DestX,DestY,32);

			if( ActFlg[KACTION_TRANSFER] == 0 )
				KeepPosition(DestX,DestY);

			if( ActCount[KACTION_CALL] % 10 == 0 ) {
				if( ZakoCount<29 )
					FOwner->Add(new TZako2_06(FOwner,700,FOwner->GetRand(480),0,5,12,false));
				else if( ZakoCount == 29 ) {
					FOwner->Add(new TZako2_06(FOwner,700,120,0,5,12,true));
					FOwner->Add(new TZako2_06(FOwner,700,360,0,5,12,true));
				}
				Inc(ZakoCount);
			}

			if( ActCount[KACTION_CALL] > 300 ) {
				SetNextState(KACTION_CALL);
			}
		}break;
	}
	Inc(ActCount[KACTION_CALL]);

	RotateBit();

}

TStage5Boss2::TStage5Boss2( TOBJList* owner, s32 _x, s32 _y ) : TBossBase(owner)
{
	ConstForeground = 14;
	ShotCount = 0;
	ZakoCount = 0;
	CommandRed = true;
	Score = 100000;
	ComBonus = 5000;
	RetreatDistance = 38000;

	TBossBase::Init(_x, _y, KSTR_SHOT*400, 20);


	if( (FOwner->IsSequence()) )
		FSTR = 500;
	else
		FSTR = 1000;
	Motion_Default = KMOTION_STAGE5BOSS2_DEFAULT;
	Motion_Foward = KMOTION_STAGE5BOSS2_FOWARD;
	Motion_Back = KMOTION_STAGE5BOSS2_BACK;

	if( FOwner->Difficulty <= KDIFFICULTY_NORMAL ) {
		motionid_sla1 =  KMOTION_STAGE5BOSS2_2_SLASHR1_2;
		motionid_sla2 =  KMOTION_STAGE5BOSS2_2_SLASHR2;
		MakeSprFromID(FOwner->GetDG(), KSPRDATA_STAGE5BOSS2_2, MechaTex, 16000, ExtraEffects);
	}
	else {
		motionid_sla1 =  KMOTION_STAGE5BOSS2_SLASHR1_2;
		motionid_sla2 =  KMOTION_STAGE5BOSS2_SLASHR2;
		MakeSprFromID(FOwner->GetDG(), KSPRDATA_STAGE5BOSS2, MechaTex, 16000, ExtraEffects);
	}

	if( ExtraEffects ) {
		for( u32 i  =  ConstForeground ; i < numSpr; i++ ) {
			Sprite[i]->GetSpr()->SetColor(96,ctAlpha);
		}
	}
	MotionControlSoon(KMOTION_STAGE3BOSS_DEFAULT);

	for( u32 i = 0; i <= 3; i++ ) {
		Bit[i] =  new TStage3BossBit(FOwner,X,Y);
		FOwner->Add(Bit[i]);
	}

	Sprite[BBlade1]->GetSpr()->FHit = false;
	Sprite[BBlade2]->GetSpr()->FHit = false;

	ActFlg[KACTION_APPEAR] = 1;

}

void TStage5Boss2::Move()
{
	TBossBase::Move();
	if( (HP>0) && (FOwner->Distance<RetreatDistance) ) {
		if( ActFlg[KACTION_SHOT]>0 ) ShotMotion();
		if( ActFlg[KACTION_HOMING]>0 ) HomingMotion();
		if( ActFlg[KACTION_APPEAR]>0 ) AppearEvent();
		if( ActFlg[KACTION_LASER]>0 ) LaserMotion();
		if( ActFlg[KACTION_CALL]>0 ) CallZako();
	}
	else if( (HP>0) && (FOwner->Distance>=RetreatDistance) ) RetreatMotion();

	//死亡判定
	if( (HP<=0) && (ActFlg[KACTION_FALL]==0) )
		ActFlg[KACTION_FALL] = 1;

	if( ActFlg[KACTION_FALL]>0 ) Fall();

}

void TStage5Boss2::Die()
{
	TBossBase::Die();
	for( u32 i = 0; i <= 1; i++ )
		if( BladeTrace[i]!=NULL )
			BladeTrace[i]->Die();

	for( u32 i = 0; i <= 3; i++ ) {
		if( Bit[i]!=NULL ) Bit[i]->Die();
	}

}

TStage5Boss3Hair::TStage5Boss3Hair( TOBJList* owner, TJoint* _spr ) : TOBJ(owner)
{
	FObjectName = "TStage5Boss3Hair";
	FKind = kEffect2;

	RSpeedORG = random(12)-6;
	cnt = 0;
	cycle = random(32)+24;

	Element = _spr;

}

void TStage5Boss3Hair::Move()
{
	TOBJ::Move();
	Element->RotateR(RSpeed);

	RSpeed = RoundOff(cost[cnt & 0xfff]*RSpeedORG);
	Inc(cnt,cycle);

	if( cnt>4095 ) {
		//    RSpeedORG = random(16)-8;
		cnt = cnt-4095;
	}

}

TStage5Boss3HairControler::TStage5Boss3HairControler( TOBJList* owner, TJoint* _parent, s32 _x, s32 _y, s32 _r, s32 _segment, s32 _z, s32 _pat, s32 _col ) : TOBJ(owner)
{
	FObjectName = "TStage5Boss3HairControler";
	FKind = kEffect2;
	X = _x;
	Y = _y;
	OwnerSprite = _parent;

	for( u32 i  =  0 ; i < _segment; i++ ) {
		MakeSpriteLite(i, 0,0,32,16,_z+i,bmNormal);
		Sprite[i]->GetSpr()->AddTexture(Enemy5Tex, 352, 1 + 16*_pat, 383, 15 + 16*_pat);
		//      FCX = FCX+16;
		Sprite[i]->GetSpr()->FCY += 8;
		Sprite[i]->GetSpr()->Move(X + 28*i,Y);
		Sprite[i]->GetSpr()->SetColor(_col, ctRed);
		Sprite[i]->GetSpr()->SetColor(_col, ctGreen);
		Sprite[i]->GetSpr()->SetColor(_col, ctBlue);
		Sprite[i]->GetSpr()->Zoom(1.f,0.5f);

		TStage5Boss3Hair* part  =  new TStage5Boss3Hair(FOwner, Sprite[i]);
		FOwner->Add(part);

		if( i>0 )
			Sprite[i-1]->Add(Sprite[i]);

		Parts.push_back(part);
	}

	Sprite[0]->Rotate(_r);

	OwnerSprite->Add(Sprite[0]);

}

TStage5Boss3HairControler::~TStage5Boss3HairControler()
{

}

void TStage5Boss3HairControler::Move()
{
	TOBJ::Move();

}

void TStage5Boss3HairControler::Die()
{
	TOBJ::Die();
	if( OwnerSprite!=NULL ) {
		OwnerSprite->Delete(Sprite[0]);       //親スプライトの多関節リストから自分を削除
	}
	for( u32 i  =  0 ; i < Parts.size(); i++ )
		Parts[i]->Die();

}

TStage5Boss3BGFilter::TStage5Boss3BGFilter( TOBJList* owner ) : TOBJ(owner)
{
	FObjectName = "TStage5Boss3BGFilter";
	FKind = kEffect2;

	MakeSpriteLite(0, -32,-32,704,544,25000,bmNormal);
	Sprite[0]->GetSpr()->AddTexture(Eff7Tex, 1 , 1, 255, 255);
	Sprite[0]->GetSpr()->SetColor(64,ctRed);
	Sprite[0]->GetSpr()->SetColor(64,ctGreen);
	Sprite[0]->GetSpr()->SetColor(64,ctBlue);
	Sprite[0]->GetSpr()->SetColor(0,ctAlpha);
}

void TStage5Boss3BGFilter::Move()
{
	TOBJ::Move();
	if( (Sprite[0]->GetSpr()->GetColor(ctAlpha)<64) && (Age % 3 == 0) )
		Sprite[0]->GetSpr()->SetColorR(1,ctAlpha);

}

void TStage5Boss3::SetNextState( s32 _current )
{
	TBossBase::SetNextState(_current);
	ActFlg[_current] = 0;
	ActCount[_current] = -1;

	switch( _current ){
	case KACTION_APPEAR     : 
		{
			ActFlg[KACTION_SHOT] = 1;
			//ActFlg[KACTION_HOMING2] = 1;
			//ActFlg[KACTION_CALL2] = 1;
		}break;
	case KACTION_SHOT     :
		{
			ActFlg[KACTION_CALL1] = 1;
		}break;
	case KACTION_CALL1     :
		{
			ActFlg[KACTION_HOMING] = 1;
		}break;
	case KACTION_HOMING     : 
		{
			ActFlg[KACTION_HOMING2] = 1;
		}break;
	case KACTION_HOMING2     : 
		{
			ActFlg[KACTION_CALL2] = 1;
		}break;
	case KACTION_CALL2     : 
		{
			ActFlg[KACTION_CALL3] = 1;
			//      ActFlg[KACTION_SHOT] = 1;
		}break;
	case KACTION_CALL3     : 
		{
			ActFlg[KACTION_SHOT] = 1;
		}break;
	}

}

void TStage5Boss3::AppearEvent()
{
	switch( ActFlg[KACTION_APPEAR] ){
		//初期化
		case 1:
			{
			Inc(ActFlg[KACTION_APPEAR]);
			ActCount[KACTION_APPEAR]  =  -1;

		}break;
		//
		case 2:
			{
			DestX = 450;
			DestY = 230;

			Inc(ActFlg[KACTION_APPEAR]);
			ActCount[KACTION_APPEAR]  =  -1;
			ActFlg[KACTION_TRANSFER] = 1;
		}break;
		case 3:
			{
			Transfer(DestX,DestY,8);

			if( ActFlg[KACTION_TRANSFER] == 0 ) {
				Inc(ActFlg[KACTION_APPEAR]);
				ActCount[KACTION_APPEAR]  =  -1;
				FOwner->Add(new TWeakPoint2(FOwner,Sprite[BHead]));
			}
		}break;
		case 4:{
			KeepPosition(DestX,DestY);
			if( ActCount[KACTION_APPEAR] >60 ) {
				SetNextState(KACTION_APPEAR);

			}
		}break;
	}
	Inc(ActCount[KACTION_APPEAR]);

}

void TStage5Boss3::MakeShot( s32 _num, s32 _r )
{
	s32 GX = RoundOff(Sprite[_num+6]->GetSpr()->FCX);
	s32 GY = RoundOff(Sprite[_num+6]->GetSpr()->FCY);
	if( FOwner->IsSequence() )
		FOwner->Add(new TCurvedLaser(FOwner,GX,GY,_r+2048,0, 10, 0.5,10,0));
	else
		FOwner->Add(new TCurvedLaser(FOwner,GX,GY,_r+2048,0, 10, 2,10,0));
	if( CreateSE ) FOwner->SoundEffect->WavPlay(WABeam,9);

}

void TStage5Boss3::ShotMotion()
{
	switch( ActFlg[KACTION_SHOT] ){
		case 1:
			{
			for( u32 i = 0; i <= 8; i++ ) {
				FOwner->Add(new TBeamShotEffect(FOwner, Sprite[BCanon1+i], 0,0,690,1));
			}

			Inc(ActFlg[KACTION_SHOT]);
			ActCount[KACTION_SHOT]  =  -1;
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WACharge,8);
		}break;
		//
		case 2:{
			KeepPosition(DestX,DestY);
			if( ActCount[KACTION_SHOT] > 60 ) {
				Inc(ActFlg[KACTION_SHOT]);
				ActCount[KACTION_SHOT]  =  -1;
			}
		}break;
		case 3:
			{
			KeepPosition(DestX,DestY);
			s32 shotinterval;

			switch( FOwner->Difficulty ){
			case KDIFFICULTY_EASY:shotinterval = 15;		break;
			case KDIFFICULTY_HARD:shotinterval = 6;		break;
			default: 
				shotinterval = 10;
			}

			if( ActCount[KACTION_SHOT] % shotinterval == 0 ) {
				MakeShot(ShotCount,FOwner->GetRand(1024)-512);
				Inc(ShotCount);
				if( ShotCount>9 ) ShotCount = 0;

			}

			if( ActCount[KACTION_SHOT] > 600 ) {
				Inc(ActFlg[KACTION_SHOT]);
				ActCount[KACTION_SHOT]  =  -1;
			}
		}break;
		case 4:{
			KeepPosition(DestX,DestY);
			if( ActCount[KACTION_SHOT] > 60 ) {
				SetNextState(KACTION_SHOT);
			}
		}break;
	}
	Inc(ActCount[KACTION_SHOT]);

}

void TStage5Boss3::HomingMotion()
{
	s32 hominginterval;
	switch( FOwner->Difficulty ){
	case KDIFFICULTY_EASY:hominginterval = 80;	break;
	case KDIFFICULTY_HARD:hominginterval = 30;	break;
	default: 
		hominginterval = 40;
	}

	switch( ActFlg[KACTION_HOMING] ){
		case 1:{
			if( ActCount[KACTION_HOMING]>120 ) {
				DestX = 450;
				DestY = 80;

				ActFlg[KACTION_TRANSFER] = 1;

				Inc(ActFlg[KACTION_HOMING]);
				ActCount[KACTION_HOMING]  =  -1;
			}
		}break;
		case 2:{
			Transfer(DestX,DestY,32);

			if( Age % hominginterval == 0 ) {
				MakeHoming();
			}

			if( ActFlg[KACTION_TRANSFER] == 0 ) {
				DestX = 350;
				DestY = 400;

				ActFlg[KACTION_TRANSFER] = 1;

				Inc(ActFlg[KACTION_HOMING]);
				ActCount[KACTION_HOMING]  =  -1;
			}
		}break;
		case 3:{
			Transfer(DestX,DestY,32);

			if( Age % hominginterval == 0 ) {
				MakeHoming();
			}

			if( ActFlg[KACTION_TRANSFER] == 0 ) {
				Inc(ActFlg[KACTION_HOMING]);
				ActCount[KACTION_HOMING]  =  -1;

				DestX = 280;
				DestY = 240;
				ActFlg[KACTION_TRANSFER] = 1;
			}
		}break;
		case 4:{
			Transfer(DestX,DestY,32);

			if( Age % hominginterval == 0 ) {
				MakeHoming();
			}

			if( ActFlg[KACTION_TRANSFER] == 0 ) {
				Inc(ActFlg[KACTION_HOMING]);
				ActCount[KACTION_HOMING]  =  -1;
			}
		}break;
		case 5:
			{
			KeepPosition(DestX,DestY);
			if( ActCount[KACTION_HOMING]>30 ) {
				SetNextState(KACTION_HOMING);
			}
		}break;
	}
	Inc(ActCount[KACTION_HOMING]);

}

void TStage5Boss3::HomingMotion2()
{
	const s32 Lnum[] = { 3,6,1,4,0,5,2,7 };
	switch( ActFlg[KACTION_HOMING2] ){
		case 1:{
			MotionControl(KMOTION_STAGE5BOSS3_BEAM2);
			if( ActCount[KACTION_HOMING2] > 30 ) {
				MakeHoming2();
				Inc(ActFlg[KACTION_HOMING2]);
				ActCount[KACTION_HOMING2]  =  -1;
			}
		}break;
		case 2:{
			if( ActCount[KACTION_HOMING2] > 30 ) {
				DestX = 500;
				DestY = 230;

				Inc(ActFlg[KACTION_HOMING2]);
				ActCount[KACTION_HOMING2]  =  -1;
				ActFlg[KACTION_TRANSFER] = 1;
				FOwner->Add(new TBeamShotEffect(FOwner, Sprite[BHip], -66,36,120,2));
			}
		}break;
		case 3:{
			Transfer(DestX,DestY,24);

			if( ActFlg[KACTION_TRANSFER] == 0 ) {
				Inc(ActFlg[KACTION_HOMING2]);
				ActCount[KACTION_HOMING2]  =  -1;
				ShotCount = 0;
			}
		}break;
		case 4:{
			MotionControl(KMOTION_STAGE5BOSS3_BEAM);
			KeepPosition(DestX,DestY);

			if( (ActCount[KACTION_HOMING2] % 5 == 0) && (ShotCount<8) ) {
				FOwner->Add(new TSpreadLaser(FOwner, this, -66,36,256-64*Lnum[ShotCount],100));
				if( FOwner->Difficulty == KDIFFICULTY_HARD )        
					FOwner->Add(new TSpreadLaser(FOwner, this, -66,36,(282-64*Lnum[7-ShotCount]),100));
				if( CreateSE ) FOwner->SoundEffect->WavPlay(WALBeam, 9);
				Inc(ShotCount);
			}

			if( ActCount[KACTION_HOMING2] > 120 ) {
				SetNextState(KACTION_HOMING2);
			}
		}break;
	}
	Inc(ActCount[KACTION_HOMING2]);

}

void TStage5Boss3::Fall()
{
	switch( ActFlg[KACTION_FALL] ){
		//初期化
		case 1:
			{
			FOwner->BossDie = true;
			if( CreateBGM ) FOwner->MusicPlayer->Stop();
			FOwner->Quake(8,30);

			if( BonusRate>1 ) {
				FOwner->Add(new TMultiple(FOwner,BonusRate,X,Y));
				FOwner->BossRate = BonusRate;
			}

			for( u32 i  =  0 ; i < numSpr; i++ ) {
				//あたり判定を消す
				Sprite[i]->GetSpr()->FHit = false;
			}

			HPGauge->Hide();
			FOwner->Add(new TExplarge(FOwner,X,Y,1,true));
			FOwner->Add(new TExplarge(FOwner,(s32)(Sprite[BHead]->GetSpr()->FCX),(s32)(Sprite[BHead]->GetSpr()->FCY),1,true));



			Sprite[BMask]->GetSpr()->FVisible = false;
			if( ExtraEffects ) {
				Sprite[BMask + ConstForeground]->GetSpr()->FVisible = false;
			}
			Inc(ActFlg[KACTION_FALL]);
			ActCount[KACTION_FALL]  =  -1;
		}break;
		//
		case 2:
			{
			MotionControl(KMOTION_STAGE5BOSS3_BEAM);

			if( ActCount[KACTION_FALL] > 120 ) {
				Inc(ActFlg[KACTION_FALL]);
				ActCount[KACTION_FALL]  =  -1;

				FOwner->SetBloomingColor(255,0,47,47,47);
			}
		}break;
		case 3:
			{
			Sprite[BHip]->RotateR(1);
			for( u32 i  =  0 ; i <= numSpr-2; i++ ) {
				Sprite[i]->GetSpr()->SetColorR(-1,ctRed);
				Sprite[i]->GetSpr()->SetColorR(-1,ctGreen);
				Sprite[i]->GetSpr()->SetColorR(-1,ctBlue);
			}

			Sprite[BHip]->MoveR(0.5f, 0.2f);

			Sprite[numSpr-1]->GetSpr()->SetColorR(8, ctAlpha);
			Sprite[numSpr-1]->ZoomR(1.005f, 1.005f);

			s32 ParticleR = random(4095);
			if( (ActCount[KACTION_FALL]>60) && (ActCount[KACTION_FALL] % 2 == 0) ) {
				FOwner->Add(new TParticle2(FOwner,X+RoundOff((f32)(160+ActCount[KACTION_FALL])*cost[ParticleR]),
					Y+RoundOff((f32)(160+ActCount[KACTION_FALL])*sint[ParticleR]),
					2,ParticleR,-1,50));
			}

			if( ActCount[KACTION_FALL]>300 ) {
				Inc(ActFlg[KACTION_FALL]);
				ActCount[KACTION_FALL]  =  -1;
			}
		}break;
		case 4:
			{
			Sprite[BHip]->RotateR(1);
			Sprite[BHip]->MoveR(0.5f,0.2f);

			Sprite[numSpr-1]->ZoomR(0.95f,0.95f);

			if( ActCount[KACTION_FALL]>30 ) {
				Inc(ActFlg[KACTION_FALL]);
				ActCount[KACTION_FALL]  =  -1;
			}
		}break;
		case 5:
			{
			FOwner->Add(new TBossExpWind(FOwner));
			Filter->Die();
			Die();
			FOwner->BDieTime = FOwner->GetTick();

		}
	}

	//振動
	if( Age % 2 == 0 )
		Sprite[BHip]->MoveR(0,-2);
	else
		Sprite[BHip]->MoveR(0,2);

	Inc(ActCount[KACTION_FALL]);

	//死にイベント早送り用
	if( ! ButtonON[0] ) ButtonFlg[0] = false;

	if( (! ButtonFlg[0]) && ButtonON[0] && (! FOwner->ResultExists)  && (FOwner->IsBossDefeated(4)) ) {
		FOwner->Add(new TResult2(FOwner));
		ButtonFlg[0] = true;
	}

}

void TStage5Boss3::Search()
{

}

void TStage5Boss3::CallZako( s32 _pat )
{
	switch( _pat ){
		case 0:{
			s32 sx = FOwner->GetRand(200)+320;
			s32 sy = FOwner->GetRand(300)+50;
			FOwner->Add(new TZako2_01(FOwner,sx, 540, sx, sy, 0,0,4,false));
		}break;
		case 1:{
			FOwner->Add(new TZako2_06(FOwner,550, -50, 0,0,9,false));
			FOwner->Add(new TZako2_06(FOwner,700, 50, 0,0,7,false));
			FOwner->Add(new TZako2_06(FOwner,700, 450, 0,0,6,false));
			FOwner->Add(new TZako2_06(FOwner,550, 500, 0,0,8,false));
		}break;
		case 2:{
			for( u32 i = 0; i <= 7; i++ )
				FOwner->Add(new TZako2_05_2(FOwner,Sprite[BHip],-800, 240,i*512, 120,6,0,false));

			FOwner->FunSync = 8;
		}break;
		case 3:{
			FOwner->Add(new TZako3_01(FOwner,450, -140,450, 120,1,false));
			FOwner->Add(new TZako3_01(FOwner,450, 480+140,450, 360,1,true));
		}break;
	}

}

void TStage5Boss3::CallMotion1()
{
	s32 interval;
	switch( FOwner->Difficulty ){
	case KDIFFICULTY_EASY:interval = 30;		break;
	case KDIFFICULTY_HARD:interval = 15;		break;
	default:
		interval = 20;
	}

	switch( ActFlg[KACTION_CALL1] ){
		case 1:{
			DestX = 550;
			DestY = 240;

			ActFlg[KACTION_TRANSFER] = 1;

			Inc(ActFlg[KACTION_CALL1]);
			ActCount[KACTION_CALL1]  =  -1;
		}break;
		case 2:{
			Transfer(DestX,DestY,24);

			if( ActCount[KACTION_CALL1] % interval == 0 ) {
				CallZako(0);
			}

			if( ActFlg[KACTION_TRANSFER] = 0 ) {
				Inc(ActFlg[KACTION_CALL1]);
				ActCount[KACTION_CALL1]  =  -1;
			}
		}break;
		case 3:{
			MotionControl(KMOTION_STAGE5BOSS3_DEFAULT);
			KeepPosition(DestX,DestY);
			if( ActCount[KACTION_CALL1] % interval == 0 ) {
				CallZako(0);
			}

			if( ActCount[KACTION_CALL1] > 300 ) {
				SetNextState(KACTION_CALL1);
			}
		}break;
	}
	Inc(ActCount[KACTION_CALL1]);

}

void TStage5Boss3::CallMotion2()
{
	switch( ActFlg[KACTION_CALL2] ){
		case 1:{
			MotionControl(KMOTION_STAGE5BOSS3_DEFAULT);
			KeepPosition(DestX,DestY);
			if( ActCount[KACTION_CALL2]>60 ) {
				CallZako(2);

				Inc(ActFlg[KACTION_CALL2]);
				ActCount[KACTION_CALL2]  =  -1;
			}
		}break;
		case 2:{
			MotionControl(KMOTION_STAGE5BOSS3_DEFAULT);
			KeepPosition(DestX,DestY);

			if( ActCount[KACTION_CALL2]>120 ) {
				DestX = 320;
				DestY = 340;

				ActFlg[KACTION_TRANSFER] = 1;

				Inc(ActFlg[KACTION_CALL2]);
				ActCount[KACTION_CALL2]  =  -1;
			}
		}break;
		case 3:{
			Transfer(DestX,DestY,12);

			if( ActFlg[KACTION_TRANSFER] == 0 ) {
				Inc(ActFlg[KACTION_CALL2]);
				ActCount[KACTION_CALL2]  =  -1;

				DestX = 140;
				DestY = 80;
				ActFlg[KACTION_TRANSFER] = 1;
			}
		}break;
		case 4:{
			Transfer(DestX,DestY,12);

			if( ActFlg[KACTION_TRANSFER] == 0 ) {
				Inc(ActFlg[KACTION_CALL2]);
				ActCount[KACTION_CALL2]  =  -1;

				DestX = 450;
				DestY = 100;
				ActFlg[KACTION_TRANSFER] = 1;
			}
		}break;
		case 5:{
			Transfer(DestX,DestY,12);

			if( ActFlg[KACTION_TRANSFER] == 0 ) {
				ActFlg[KACTION_CALL2] = 3;
				ActCount[KACTION_CALL2]  =  -1;

				DestX = 320;
				DestY = 340;
				ActFlg[KACTION_TRANSFER] = 1;
			}
		}break;
	}
	Inc(ActCount[KACTION_CALL2]);

	if( (ActFlg[KACTION_CALL2]>=3) && (FOwner->FunSync == 0) )
		SetNextState(KACTION_CALL2);

}

void TStage5Boss3::CallMotion3()
{
	switch( ActFlg[KACTION_CALL3] ){
		case 1:{
			DestX = 450;
			DestY = 230;

			ActFlg[KACTION_TRANSFER] = 1;

			Inc(ActFlg[KACTION_CALL3]);
			ActCount[KACTION_CALL3]  =  -1;
		}break;
		case 2:{
			Transfer(DestX,DestY,24);

			if( ActFlg[KACTION_TRANSFER] == 0 ) {
				//        Inc(ActFlg[KACTION_CALL3]);
				//        ActCount[KACTION_CALL3]  =  -1;
				SetNextState(KACTION_CALL3);
					//        CallZako(3);
			}
		}break;
		case 3:{
			MotionControl(KMOTION_STAGE5BOSS3_BEAM);
			KeepPosition(DestX,DestY);
			if( ActCount[KACTION_CALL3] >120 ) {
				Inc(ActFlg[KACTION_CALL3]);
				ActCount[KACTION_CALL3]  =  -1;
			}
		}break;
		case 4:{
			MotionControl(KMOTION_STAGE5BOSS3_DEFAULT);
			KeepPosition(DestX,DestY);
			if( ActCount[KACTION_CALL3] >120 ) {
				SetNextState(KACTION_CALL3);
			}
		}break;
	}
	Inc(ActCount[KACTION_CALL3]);

}

void TStage5Boss3::MakeHoming()
{
	for( u32 i = 0; i <= 1; i++ ) {
		s32 GX = RoundOff(Sprite[BCanon10+i]->GetSpr()->FCX);
		s32 GY = RoundOff(Sprite[BCanon10+i]->GetSpr()->FCY);
		FOwner->Add(new TZakoHomingCore3(FOwner,GX,GY,-512 + 1024*i,15,0.5,1));
		FOwner->Add(new TMissileFlash(FOwner,GX,GY));
	}
	if( CreateSE ) FOwner->SoundEffect->WavPlay(WABeam,9);

}

void TStage5Boss3::BeamMotion()
{
	switch( ActFlg[KACTION_BEAM] ){
		//初期化
		case 1:{
			DestX = 450;
			DestY = 230;

			Inc(ActFlg[KACTION_BEAM]);
			ActCount[KACTION_BEAM]  =  -1;
			ActFlg[KACTION_TRANSFER] = 1;
		}break;
		//
		case 2:{
			Transfer(DestX,DestY,24);

			if( ActFlg[KACTION_TRANSFER] == 0 ) {
				Inc(ActFlg[KACTION_BEAM]);
				ActCount[KACTION_BEAM]  =  -1;
			}
		}break;
		case 3:{
			MotionControl(KMOTION_STAGE5BOSS3_CHARGE);

			KeepPosition(DestX,DestY);

			if( (ActCount[KACTION_BEAM] == 30) && CreateSE ) FOwner->SoundEffect->WavPlay(WACharge,8);

			s32 ParticleR = random(4095);
			if( ActCount[KACTION_BEAM] % 2 == 0 ) {
				FOwner->Add(new TBLight(FOwner,RoundOff(Sprite[BHip]->GetSpr()->FCX-50),RoundOff(Sprite[BHip]->GetSpr()->FCY+48), (f32)ActCount[KACTION_BEAM]*0.08f+2,2000));
				if( ActCount[KACTION_BEAM]<150 )
					FOwner->Add(new TParticle2(FOwner,RoundOff(Sprite[BHip]->GetSpr()->FCX-50)+RoundOff(128*cost[ParticleR]),
					RoundOff(Sprite[BHip]->GetSpr()->FCY+48)+RoundOff(128*sint[ParticleR]),
					1,ParticleR,-1,30));
			}
			else FOwner->Add(new TBLight(FOwner,RoundOff(Sprite[BHip]->GetSpr()->FCX-50),RoundOff(Sprite[BHip]->GetSpr()->FCY+48), (f32)ActCount[KACTION_BEAM]*0.06f+2,2000));

			if( FOwner->LimitTime<=0 ) {
				Inc(ActFlg[KACTION_BEAM]);
				ActCount[KACTION_BEAM] = -1;
				FOwner->Add(new TEraserCore(FOwner,RoundOff(Sprite[BHip]->GetSpr()->FCX-70) ,RoundOff(Sprite[BHip]->GetSpr()->FCY+40)));
				FOwner->Add(new TBoxContrl(FOwner,5));
				FOwner->Add(new TSRControl(FOwner,RoundOff(Sprite[BHip]->GetSpr()->FCX-70),RoundOff(Sprite[BHip]->GetSpr()->FCY+40),2048));
				if( CreateSE ) FOwner->SoundEffect->WavPlay(WALBeam,10);

				for( u32 i  =  0 ; i < numSpr; i++ ) {
					//あたり判定を消す
					Sprite[i]->GetSpr()->FHit = false;
				}

				HPGauge->Hide();
			}

		}break;
		case 4:{
			MotionControl(KMOTION_STAGE5BOSS3_BEAM);
			KeepPosition(DestX,DestY);
			if( ActCount[KACTION_BEAM]<15 )
				Sprite[BHip]->MoveR(15-ActCount[KACTION_BEAM],0);
		}break;
	}
	Inc(ActCount[KACTION_BEAM]);

}

void TStage5Boss3::MakeHoming2()
{
	f32 GX, GY;   //発射する座標
	s32 BCount;
	s32 MaxBeam;
	s32 BeamXSpace;
	switch( FOwner->Difficulty ){
	case KDIFFICULTY_EASY:{ MaxBeam = 3;  BeamXSpace = 100;   }	break;
		//    KDIFFICULTY_HARD:{ MaxBeam = 9;    BeamXSpace = 30;   }
	default: {
		MaxBeam = 5;
		BeamXSpace = 60;
			 }
	}

	BCount = 0;
	while( BCount<MaxBeam ) {
		TargetX = BCount*BeamXSpace;
		TargetY = FOwner->GetRand(10)*40;
		//目標が自機の近くだったら、当たらないように避けてあげる
		if( CheckDistance(TargetX, TargetY, FOwner->JikiObj->X, FOwner->JikiObj->Y, 80) ) {
			continue;
		}
		GX = Sprite[BBody]->GetSpr()->FCX +76 + 4*BCount;
		GY = Sprite[BBody]->GetSpr()->FCY -16 + 4*BCount;
		PosRoll(GX, GY,Sprite[BBody]->GetSpr()->FCX,Sprite[BBody]->GetSpr()->FCY,Sprite[BBody]->Rdegree);
		FOwner->Add(new TZakoHomingCore(FOwner,(s32)(GX),(s32)(GY),TargetX,TargetY, 228*(BCount+1), 3));
		Inc(BCount);
	}

	BCount = 0;
	while( BCount<MaxBeam ) {
		TargetX = (BCount+5)*BeamXSpace;
		TargetY = FOwner->GetRand(10)*40+ 40;
		if( CheckDistance(TargetX, TargetY, FOwner->JikiObj->X, FOwner->JikiObj->Y, 80) ) {
			continue;
		}
		GX = Sprite[BBody]->GetSpr()->FCX +76- 4*BCount;
		GY = Sprite[BBody]->GetSpr()->FCY -16 - 4*BCount;
		PosRoll(GX,GY,Sprite[BBody]->GetSpr()->FCX,Sprite[BBody]->GetSpr()->FCY,Sprite[BBody]->Rdegree);
		FOwner->Add(new TZakoHomingCore(FOwner,(s32)(GX),(s32)(GY),TargetX,TargetY, -228*(BCount+1), 3));
		Inc(BCount);
	}

	if( CreateSE ) FOwner->SoundEffect->WavPlay(WABeam,9);


}

TStage5Boss3::TStage5Boss3( TOBJList* owner, s32 _x, s32 _y ) : TBossBase(owner)
{
	s32 hp;
	switch( owner->Difficulty ){
	case KDIFFICULTY_EASY:hp = KSTR_SHOT*384;	break;
	default: hp = KSTR_SHOT*512;
	}

	TBossBase::Init(_x, _y, hp, 20);

	ConstForeground = 19;
	ShotCount = 0;
	Motion_Default = KMOTION_STAGE5BOSS3_DEFAULT;
	Motion_Foward = KMOTION_STAGE5BOSS3_FOWARD;
	Motion_Back = KMOTION_STAGE5BOSS3_BACK;

	MakeSprFromID(FOwner->GetDG(), KSPRDATA_STAGE5BOSS3, Enemy5Tex, 16000, ExtraEffects);
	//  MotionControlSoon(KMOTION_STAGE3BOSS_DEFAULT);
	Sprite[BRShoulder]->GetSpr()->FZ = Sprite[BRShoulder]->GetSpr()->FZ+300;
	if( ExtraEffects )
		Sprite[BRShoulder + ConstForeground]->GetSpr()->FZ = Sprite[BRShoulder+ConstForeground]->GetSpr()->FZ+300;


	s32 headX = RoundOff(Sprite[BHead]->GetSpr()->FCX);
	s32 headY = RoundOff(Sprite[BHead]->GetSpr()->FCY);
	for( u32 i = 0; i <= 3; i++ ) {
		TStage5Boss3HairControler* part = new TStage5Boss3HairControler(FOwner,Sprite[BHead],headX-7,headY-24+i*4, -400 + 80*i, 20,16001 + i*10,0,160-i*32);
		FOwner->Add(part);
		Hair.push_back(part);
	}
	for( u32 i = 0; i <= 3; i++ ) {
		TStage5Boss3HairControler* part = new TStage5Boss3HairControler(FOwner,Sprite[BHead],headX-7,headY-22+i*6, -340 + 60*i, 20,16101 + i*10,1,255-i*32);
		FOwner->Add(part);
		Hair.push_back(part);
	}
	for( u32 i = 0; i <= 3; i++ ) {
		TStage5Boss3HairControler* part = new TStage5Boss3HairControler(FOwner,Sprite[BHead],headX-7,headY-22+i*8, -340 + 60*i, 20,16201 + i*10,2,255-i*32);
		FOwner->Add(part);
		Hair.push_back(part);
	}

	//爆発光
	MakeSpriteLite(numSpr, 0,0,64,64,17000,bmAdd);
	Sprite[numSpr-1]->GetSpr()->AddTexture(MechaTex, 1, 145, 64, 208);
	Sprite[numSpr-1]->GetSpr()->FCX += 32;
	Sprite[numSpr-1]->GetSpr()->FCY += 32;
	Sprite[numSpr-1]->GetSpr()->Move(X, Y);
	Sprite[numSpr-1]->GetSpr()->Zoom(10,10);
	Sprite[numSpr-1]->GetSpr()->SetColor(0,ctAlpha);
	Sprite[BHip]->Add(Sprite[numSpr-1]);

	ActFlg[KACTION_APPEAR] = 1;

	Filter = new TStage5Boss3BGFilter(FOwner);
	FOwner->Add(Filter);
}

void TStage5Boss3::Move()
{
	TBossBase::Move();
	if( HP>0 ) {
		if( ActFlg[KACTION_APPEAR]>0 ) AppearEvent();
		if( ActFlg[KACTION_SHOT]>0 ) ShotMotion();
		if( ActFlg[KACTION_CALL1]>0 ) CallMotion1();
		if( ActFlg[KACTION_CALL2]>0 ) CallMotion2();
		if( ActFlg[KACTION_CALL3]>0 ) CallMotion3();
		if( ActFlg[KACTION_HOMING]>0 ) HomingMotion();
		if( ActFlg[KACTION_HOMING2]>0 ) HomingMotion2();
		if( ActFlg[KACTION_BEAM]>0 ) BeamMotion();
	}

	//死亡判定
	if( (HP<=0) && (ActFlg[KACTION_FALL] == 0) )
		ActFlg[KACTION_FALL] = 1;

	if( ActFlg[KACTION_FALL]>0 ) Fall();

	if( (FOwner->LimitTime<=7000) && (ActFlg[KACTION_BEAM] == 0) ) {
		ActFlg[KACTION_BEAM] = 1;

		ActFlg[KACTION_SHOT] = 0;
		ActFlg[KACTION_CALL1] = 0;
		ActFlg[KACTION_CALL2] = 0;
		ActFlg[KACTION_CALL3] = 0;
		ActFlg[KACTION_HOMING] = 0;
		ActFlg[KACTION_HOMING2] = 0;
	}
}

void TStage5Boss3::Die()
{
	TBossBase::Die();
	for( u32 i  =  Hair.size()-1; i >= 0; i-- )
		Hair[i]->Die();
}

void TStage5Boss3::OnDamaged( TOBJ* target )
{
	X = RoundOff(Sprite[BHip]->GetSpr()->FCX);
	Y = RoundOff(Sprite[BHip]->GetSpr()->FCY);

	if( target->Kind() == kHero ) {
		if( (! Buzzed) && (target->Hit[HitBuzz]) ) {
			FOwner->Scrach(this);
			Buzzed = true;
			LastBuzz = Age;
		}
		if( (target->Hit[HitBlade]) && (target->LastSlashed > LastSlashed) ) {
			if( Hit[BHead] ) {
				if( target->STR() > FDef )
					Dec(HP,target->STR() - FDef);
				else
					Dec(HP,target->STR() );
				for( u32 i = 0 ; i < ConstForeground; i++ )
					Sprite[i]->GetSpr()->SetColor(0,ctRed);
				LastFlash = Age;
			}
			else {
				if( CreateSE ) FOwner->SoundEffect->WavPlay2(WASlash_C,12,60);
			}
			HPGauge->Show();
			GaugeTime = Age;
			Sprite[Sprite_Center]->MoveR(1,0);                  //のけぞる
			Inc(KnockBackVel, 1);
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WAEdamage0,7);
			LastSlashed = target->LastSlashed;
		}
	}


	if( (target->Kind() == kHShot) ) {
		if( Hit[BHead] ) {
			Dec(HP,target->STR() );
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WAEdamage0,7);

			KnockBack();
		}
		else {
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WAEdamage1,7);
		}
	}

	if( (target->Kind() == kMissile) ) {
		Dec(HP,target->STR() - FDef);
		KnockBack();
		if( HP<=0 ) BonusRate = target->BonusRate;
	}

	if( HP<TempHP ) TempHP = HP;

}

TBSBeam_St3::TBSBeam_St3( TOBJList* owner, s32 _x, s32 _y, s32 _r ) : TOBJ(owner)
{
	FObjectName = "TBSBeam_St3";
	FKind = kEffect2;
	X = _x;
	Y = _y;
	Rdeg = _r;

	MakeSpriteLite(0, 0,0,256,256,5000,bmAdd);
	Sprite[0]->GetSpr()->AddTexture(MechaTex, 96 , 192, 288, 256);
	Sprite[0]->GetSpr()->FCX += 256;
	Sprite[0]->GetSpr()->FCY += 128;
	Sprite[0]->GetSpr()->Move(X,Y);
	Sprite[0]->GetSpr()->Rotate(Rdeg);

	if( CreateSE ) FOwner->SoundEffect->WavPlay(WALBeam,10);

}

void TBSBeam_St3::Move()
{
	TOBJ::Move();

	if( Age<60 ) {
		Sprite[0]->GetSpr()->Rotate(0);
		Sprite[0]->GetSpr()->ZoomR(1.2f, 1.f);
		Sprite[0]->GetSpr()->Rotate(Rdeg);
	}
}

TBSBeamGen_St3::TBSBeamGen_St3( TOBJList* owner ):TOBJ(owner)
{
	FObjectName = "TBSBeamGen_St3";
	FKind = kEffect2;

}

void TBSBeamGen_St3::Move()
{
	TOBJ::Move();
	switch( Age ){
		case 1:FOwner->Add(new TBSBeam_St3(FOwner, 100, -100, -700));		break;
		case 10:FOwner->Add(new TBSBeam_St3(FOwner, 250, -100, -800));		break;
		case 20:FOwner->Add(new TBSBeam_St3(FOwner, 550, -100, -600));		break;
		case 30:FOwner->Add(new TBSBeam_St3(FOwner, 700, -100, -700));		break;
		case 40:FOwner->Add(new TBSBeam_St3(FOwner, 800, -100, -800));		break;
		case 60:Die();
	}

	if( Age == 30 ) FOwner->HeroHP = 0;

}

TBoss2BeamCore::TBoss2BeamCore( TOBJList* owner ) : TOBJ(owner)
{
	FObjectName = "TBoss2BeamCore";

	Buzzed = false;
	LastBuzz = 0;
	FKind = kEffect;
	HomingLevel  =  1;
	ArriveCount = 0;

	MakeSpriteLite(0, X,Y,28,28,5000,bmAdd);
	Sprite[0]->GetSpr()->AddTexture(Eff8Tex, 129, 1, 255, 127);
	Sprite[0]->GetSpr()->FVisible = false;

	Trace  =  new TBoss2BeamTrace(FOwner, 20, 48);
	FOwner->Add(Trace);

	Sprite[0]->Rotate(Rdeg);

}

TBoss2BeamCore::~TBoss2BeamCore()
{
	Trace->Die();

}

void TBoss2BeamCore::Move()
{
	TOBJ::Move();

}

void TBoss2BeamCore::Die()
{
	TOBJ::Die();

}

void TBoss2BeamCore::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);

}

void TBoss2BeamCore::CalcTrajectory( s32 sx, s32 sy, s32 _r )
{
	X = sx;
	Y = sy;
	PrevX = X;
	PrevY = Y;
	Rdeg = _r & 0x0fff;

	s32 Steering =  20;

	Sprite[0]->Move(X,Y);
	Sprite[0]->Rotate(Rdeg);
	Trace->Reset(X,Y,Rdeg);

	s32 Count = 0;

	//ホーミング弾の位置を40フレーム分計算し、それぞれの位置をTraceに渡す
	while( Count<40 ) {

		s32 RX = DestX-X;             //自分と敵との距離を計算
		s32 RY = DestY-Y;
		s32 EnemyDeg = RoundOff(atan2(RY,RX)/(f32)M_PI*2048.f) & 0x0fff;

		s32 Rdiff = GetDiffR( EnemyDeg, Rdeg);

		if( abs(Rdiff)>=2048 ) Inc(Rdeg, -Sign(Rdiff)*(s32)(ceil((f32)(4095-abs(Rdiff))/Steering)/-Sign(Rdiff)));
		else if( abs(Rdiff)<2048 ) Dec(Rdeg, (s32)(ceil((f32)(Rdiff)/Steering)+Sign(Rdiff)));

		s32 MinSpeed = 10;
		s32 MaxSpeed = 20;
		s32 Accel = 2;
		s32 Speed;
		if( Count<=(MaxSpeed / Accel) )
			Speed = MinSpeed + MaxSpeed -Count*Accel;
		else
			Speed = MinSpeed - MaxSpeed +Count*Accel;

		Sprite[0]->Rotate(Rdeg);
		Sprite[0]->SlideMoveR(Speed ,0);

		X = RoundOff(Sprite[0]->GetSpr()->FCX);
		Y = RoundOff(Sprite[0]->GetSpr()->FCY);

		Trace->SetOwnerPos(X,Y,Rdeg);

		Inc(Count);
	}


}

void TBoss2BeamCore::Show( bool _show )
{
	Trace->Show(_show);

}

void TBoss2BeamCore::SetTarget( s32 _x, s32 _y )
{
	DestX = _x;
	DestY = _y;

}

void TBoss2BeamCore::SetTargetR( s32 _x, s32 _y )
{
	Inc(DestX,_x);
	Inc(DestY,_y);

}

void TBoss2BeamCore::Place( s32 _x, s32 _y )
{
	Trace->Slide(_x - PrevX, _y - PrevY);
	PrevX = _x;
	PrevY = _y;

}
