#include "Zako03.h"
#include "Zako01.h"
#include "Gauge.h"
#include "Effect.h"
#include "TOBJList.h"
#include "Sound.h"
#include "Missile.h"
#include "Hero.h"


void TZako2_01::Action()
{
	f32 escapespeed;
	switch( Pattern ){
	case 0:{
		Sprite[0]->MoveR(Speed,0);
		if( Age == 60 ) MakeShot(0);
		   }break;
	case 1:{
		if( FOwner->HeroPos.Y>Y )
			Sprite[0]->RotateR(-3);
		else if( FOwner->HeroPos.Y<Y )
			Sprite[0]->RotateR(3);

		Sprite[0]->SlideMoveR(Speed,0);

		if( Age == 60 ) MakeShot(0);

		   }break;
	case 2:{
		if( Age == 1 )
			Sprite[0]->SetDest(DestX,DestY,16);
		else if( 2 <= Age && Age <= 179 )
			Sprite[0]->AutoSlide();
		else if( 180 <= Age && Age <= 999 )
		{
			escapespeed = (f32)(Age-180)/2.f;
			Sprite[0]->MoveR(escapespeed,0);
		}

		if( Age == 30 ) MakeShot(0);
		   }break;
	case 3:{
		switch( Age ){
			if( Age == 1 )
				Sprite[0]->SetDest(DestX,DestY,32);
			else if( 2 <= Age && Age <= 240 )
				Sprite[0]->AutoSlide();
			else if( 241 <= Age && Age <= 999 )
			{
				escapespeed = (f32)(Age-240)/2.f;
				Sprite[0]->MoveR(escapespeed,0);
			}
		}

		switch( Age ){
		case 60:
		case 120:
		case 180:
		case 240:
			MakeMissile();
		}
		   }break;
	case 4:{
		switch( Age ){
			if( Age == 1 )
				Sprite[0]->SetDest(DestX,DestY,16);
			else if( 2 <= Age && Age <= 120 ){
				Sprite[0]->AutoSlide();
				s32 DestR = GetRfromArcTan(X,Y,FOwner->HeroPos.X,FOwner->HeroPos.Y);
				Sprite[0]->Rotate(DestR+2048);
			}
			else if( Age == 121 )
				MakeShot(0);
			else if( 122 <= Age && Age <= 999 ){
				escapespeed = (f32)(Age-120)/2.f;
				Sprite[0]->SlideMoveR(-escapespeed,0);
			}
		}
		   }
	}
}

void TZako2_01::MakeShot( s32 _pat )
{
	s32 DX = RoundOff(FOwner->HeroPos.X-X);
	s32 DY = RoundOff(FOwner->HeroPos.Y-Y);
	s32 GunR = RoundOff(atan2(DY,DX)/(f32)M_PI*2048.f) & 0xfff;
	for( u32 i = 0; i <= 2; i++ ) {
		if( FOwner->IsSequence() ) {
			FOwner->Add(new TZ01Shot4(FOwner,X,Y,GunR,6+i,0,0,500));
			if( (FOwner->Difficulty == KDIFFICULTY_HARD) && (Pattern == 2) ) {
				FOwner->Add(new TZ01Shot4(FOwner,X,Y,GunR+340,6+i,0,0,500));
				FOwner->Add(new TZ01Shot4(FOwner,X,Y,GunR-340,6+i,0,0,500));
			}
		}
		else
			FOwner->Add(new TZ01Shot2(FOwner,X,Y,GunR,6+i,0));

	}
	FOwner->Add(new TFCircle(FOwner,X,Y,1,Rdeg,RoundOff(Speed)));
	if( CreateSE ) FOwner->SoundEffect->WavPlay(WABShot,9);

}

void TZako2_01::MakeMissile()
{
	FOwner->Add(new TZ05Missile(FOwner,X,Y,FOwner->GetRand(7)));
	FOwner->Add(new TMissileFlash(FOwner,X,Y));
	if( CreateSE )  FOwner->SoundEffect->WavPlay(WAMissile,4);

}

TZako2_01::TZako2_01( TOBJList* owner, s32 _x, s32 _y, s32 _destx, s32 _desty, s32 _R, s32 _S, s32 _pat, bool _red ) : TOBJ(owner)
{
	X = _x;
	Y = _y;
	DestX = _destx;
	DestY = _desty;
	MaxHP = KSTR_SHOT*4;
	HP = MaxHP;
	TempHP = HP;
	FSTR = 500;
	FKind = kEnemy;
	Score = 1000;
	Buzzed = false;
	LastBuzz = 0;
	Rdeg = _R;
	Speed = _S;
	Pattern = _pat;
	CommandRed = _red;
	GaugeTime = 0;
	TotalFlight = 0;
	LastFlash = 0;
	OriginColor  =  255;
	ComBonus = 5000;
	//IDの付与
	FID = NewID;
	Inc(NewID);

	s32 texX;
	s32 texWidth;
	switch( _pat ){
	case 0:
	case 2:
	case 3:{
		texX = 1;
		texWidth = 127;
		   }break;
	default: {
		texX = 128;
		texWidth = -127;
			 }
	}


	MakeSprite(QD, MechaTex,  0,texX,1,texWidth, 63,111,  20,  21,  68,  31,64, 32,CommandRed);
	Sprite[0]->GetSpr()->Move(_x,_y);
	Sprite[0]->GetSpr()->Zoom(1.2f,1.2f);

	if( ExtraEffects ) {
		MakeSprite(QD, MechaTex,  1,texX,1,texWidth, 63,110,  0,  0,  0,  0,64, 32,false);
		Sprite[1]->GetSpr()->Move(_x,_y);
		Sprite[1]->GetSpr()->FEffectNum = 1;
		Sprite[1]->GetSpr()->FBumpSize = 3.5f;
		Sprite[1]->GetSpr()->FBumpTex = MechaTex;
		Sprite[1]->GetSpr()->FBumpLevel = -0.2f;

		if( FOwner->NowStage == 2 )
			Sprite[1]->GetSpr()->SetColor(64,ctAlpha);
		else
			Sprite[1]->GetSpr()->SetColor(128,ctAlpha);
		Sprite[1]->GetSpr()->Zoom(1.2f, 1.2f);
		Sprite[0]->Add(Sprite[1]);
	}

	HPGauge  =  new TEnemyGauge2(FOwner,this,0,32);
	FOwner->Add(HPGauge);

}

void TZako2_01::Move()
{
	TOBJ::Move();

	X = (s32)(Sprite[0]->GetSpr()->FCX);
	Y = (s32)(Sprite[0]->GetSpr()->FCY);

	Action();

	if( (X > 700+64) || (Y > 480+64) || (X < 0-64) ) {
		Die();//画面外に消えたら消す & 母艦にダメージ
		Dec(FOwner->MShipHP,FSTR);
		FOwner->Add(new TMotherShipDamaged(FOwner, X, Y));
	}

	if( (Age - LastFlash == 2) ) {
		if( CommandRed || (! ExtraEffects) ) {
			Sprite[0]->GetSpr()->SetColor(255,ctRed);          //色を戻す
		}
		else {
			Sprite[0]->GetSpr()->SetColor(OriginColor,ctRed);          //色を戻す
		}
		Sprite[0]->MoveR(-3,0);                  //のけぞり戻し
	}

	if( (Age-LastBuzz)>=40 ) Buzzed = false;
	if( HP<=0 ) OnBroken();                        //HPが無くなったら死ぬ

	if( (Age-GaugeTime)>120 ) HPGauge->Hide();

	if( FOwner->BossDie ) {
		FOwner->Add(new TExpmiddle(FOwner,X,Y,Rdeg,Speed,0));//爆発
		Die();
	}

}

void TZako2_01::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);
	X = RoundOff(Sprite[0]->GetSpr()->FCX);
	Y = RoundOff(Sprite[0]->GetSpr()->FCY);
	if( target->Kind() == kHero ) {
		if( (! Buzzed) && (target->Hit[HitBuzz]) ) {
			FOwner->Scrach(this);
			Buzzed = true;
			LastBuzz = Age;
		}
		if( (target->Hit[HitBlade]) && (target->LastSlashed > LastSlashed) ) {
			Dec(HP,target->STR());
			Sprite[0]->GetSpr()->SetColor(0,ctRed);
			LastFlash = Age;
			Sprite[0]->MoveR(3,0);                  //のけぞる
			HPGauge->Show();
			GaugeTime = Age;
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WAEdamage0,7);
			LastSlashed = target->LastSlashed;
		}
	}

	if( (target->Kind() == kHShot) || (target->Kind() == kMissile) ) {
		Dec(HP,target->STR());
		Sprite[0]->GetSpr()->SetColor(0,ctRed);
		LastFlash = Age;
		Sprite[0]->MoveR(3,0);                  //のけぞる
		HPGauge->Show();
		GaugeTime = Age;
		if( CreateSE ) FOwner->SoundEffect->WavPlay(WAEdamage0,7);
		if( HP<=0 ) BonusRate = target->BonusRate;
	}

	if( target->Kind() == kColBG ) {
		FOwner->Add(new TExpmiddle(FOwner,X,Y,Rdeg,Speed,0));//爆発
		Die();
	}

	if( HP<TempHP ) TempHP = HP;

}

void TZako2_01::Die()
{
	HPGauge->Die();
	TOBJ::Die();

}

void TZako2_01::OnBroken()
{
	TOBJ::OnBroken();
	FOwner->Add(new TExpmiddle(FOwner,X,Y,Rdeg,Speed,0));//爆発

}

void TZako2_02::Action()
{
	Sprite[Z2_2Body]->RotateR(96);
	Sprite[Z2_2Rotor]->RotateR(-172);
	Sprite[Z2_2Wing1]->RotateR(60);
	Sprite[Z2_2Wing2]->RotateR(60);
	Sprite[Z2_2Wing3]->RotateR(60);

	switch( Pattern ){
	case 0:{
		switch( ActFlg[KACTION_S2Z2_ACT1] ){
		case 0:{
			Sprite[0]->SetDest(DestX, DestY, 16);
			Inc(ActFlg[KACTION_S2Z2_ACT1]);
			ActCount[KACTION_S2Z2_ACT1] = 0;
			   }break;
		case 1:{
			Sprite[0]->AutoSlide();

			if( ActCount[KACTION_S2Z2_ACT1] == 60 ) {
				Inc(ActFlg[KACTION_S2Z2_ACT1]);
				ActCount[KACTION_S2Z2_ACT1] = 0;
			}
			   }break;
		case 2:{
			if( ActCount[KACTION_S2Z2_ACT1] % 3 == 0 ) MakeShot();

			if( ActCount[KACTION_S2Z2_ACT1] == RetreatTime ) {
				Inc(ActFlg[KACTION_S2Z2_ACT1]);
				ActCount[KACTION_S2Z2_ACT1] = 0;
			}
			   }break;
		case 3:{
			if( ActCount[KACTION_S2Z2_ACT1] == 60 ) {
				Inc(ActFlg[KACTION_S2Z2_ACT1]);
				ActCount[KACTION_S2Z2_ACT1] = 0;
			}
			   }break;
		case 4:{
			f32 escapespeed = (f32)ActCount[KACTION_S2Z2_ACT1] / 4.f;
			Sprite[0]->MoveR(-escapespeed, 0);
			   }break;
		}
		   }break;
	}

	Inc(ActCount[KACTION_S2Z2_ACT1]);

}

void TZako2_02::Accel()
{
	if( (FOwner->Speed>=10) )
		BSpeed = BSpeed+0.4f;
	else if( BSpeed>0 )
		BSpeed = BSpeed-0.4f;

	if( BSpeed>6 )
		BSpeed = 6;

	if( BSpeed<0 )
		BSpeed = 0;

	Sprite[0]->MoveR(BSpeed,0);

}

void TZako2_02::MakeShot()
{
	s32 ShotAngle  =  FOwner->GetRand(128)*32;
	FOwner->Add(new TZ01Shot2(FOwner,X,Y,ShotAngle,FOwner->GetRand(3)+2,BSpeed));
	if( CreateSE ) FOwner->SoundEffect->WavPlay(WABShot,9);
}

TZako2_02::TZako2_02( TOBJList* owner, s32 _x, s32 _y, s32 dx, s32 dy, s32 _R, s32 _S, s32 _pat, bool _red ) : TOBJ(owner)
{
	X = _x;
	Y = _y;
	DestX = dx;
	DestY = dy;
	MaxHP = KSTR_SHOT*40;
	HP = MaxHP;
	TempHP = HP;
	FSTR = 500;
	FKind = kEnemy;
	Score = 10000;
	Buzzed = false;
	LastBuzz = 0;
	Rdeg = _R;
	Speed = _S;
	Pattern = _pat;
	CommandRed = _red;
	GaugeTime = 0;
	TotalFlight = 0;
	LastFlash = 0;
	OriginColor  =  255;
	ActFlg[KACTION_S2Z2_ACT1] = 0;
	ActCount[KACTION_S2Z2_ACT1] = 0;
	BSpeed = 0;

	switch( FOwner->Difficulty ){
	case KDIFFICULTY_SINGLE:
	case KDIFFICULTY_HARD:
		RetreatTime = 300;
		break;
	default:
		RetreatTime = 360;
	}

	//IDの付与
	FID = NewID;
	Inc(NewID);

	MakeSprite(QD, MechaTex,  Z2_2Body, 129,  1, 31, 31, 150,  1,  1,  31,  31,15, 15,false);
	Sprite[Z2_2Body]->GetSpr()->Move(X, Y);

	MakeSprite(QD, MechaTex,  Z2_2Rotor,129, 33, 31, 31, 151,  1,  1,  31,  31,16, 16,false);
	Sprite[Z2_2Rotor]->GetSpr()->Move(X, Y);

	MakeSprite(QD, MechaTex,  Z2_2Wing1,160,  1, 15, 47, 152,  1,  1,  15,  47, 9, 33,false);
	Sprite[Z2_2Wing1]->GetSpr()->Move(X, Y);

	MakeSprite(QD, MechaTex,  Z2_2Wing2,160,  1, 15, 47, 153,  1,  1,  15,  47, 9, 33,false);
	Sprite[Z2_2Wing2]->GetSpr()->Move(X, Y);

	MakeSprite(QD, MechaTex,  Z2_2Wing3,160,  1, 15, 47, 154,  1,  1,  15,  47, 9, 33,false);
	Sprite[Z2_2Wing3]->GetSpr()->Move(X, Y);

	Sprite[Z2_2Body]->Add(Sprite[Z2_2Rotor]);
	Sprite[Z2_2Body]->Add(Sprite[Z2_2Wing1]);
	Sprite[Z2_2Body]->Add(Sprite[Z2_2Wing2]);
	Sprite[Z2_2Body]->Add(Sprite[Z2_2Wing3]);

	Sprite[Z2_2Wing2]->Rotate(1365);
	Sprite[Z2_2Wing3]->Rotate(-1365);

	Sprite[Z2_2Body]->GetSpr()->FSmooth = false;

	HPGauge  =  new TEnemyGauge2(FOwner,this,-96,32);
	FOwner->Add(HPGauge);

}

void TZako2_02::Move()
{
	TOBJ::Move();

	X = (s32)(Sprite[0]->GetSpr()->FCX);
	Y = (s32)(Sprite[0]->GetSpr()->FCY);
	Accel();
	Action();
	if( (X < -64) || (Y > 480+64) ) {
		Die();//画面外に消えたら消す & 母艦にダメージ
		Dec(FOwner->MShipHP, FSTR*3);
		FOwner->Add(new TMotherShipDamaged(FOwner, X, Y));
	}

	if( (Age-LastFlash == 2) ) {
		if( CommandRed || (! ExtraEffects) ) {
			for( u32 i = 0 ; i < numSpr; i++ )
				Sprite[i]->GetSpr()->SetColor(255,ctRed);
		}
		else {
			for( u32 i = 0 ; i < numSpr; i++ )
				Sprite[i]->GetSpr()->SetColor(OriginColor,ctRed);          //色を戻す
		}
		Sprite[0]->MoveR(-3,0);                  //のけぞり戻し
	}

	if( (Age-LastBuzz)>=40 ) Buzzed = false;
	if( HP<=0 ) OnBroken();                        //HPが無くなったら死ぬ

	if( (Age-GaugeTime)>120 ) HPGauge->Hide();

}

void TZako2_02::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);
	X = RoundOff(Sprite[0]->GetSpr()->FCX);
	Y = RoundOff(Sprite[0]->GetSpr()->FCY);
	if( target->Kind() == kHero ) {
		if( (! Buzzed) && (target->Hit[HitBuzz]) ) {
			FOwner->Scrach(this);
			Buzzed = true;
			LastBuzz = Age;
		}
		if( (target->Hit[HitBlade]) && (target->LastSlashed > LastSlashed) ) {
			Dec(HP,target->STR());
			for( u32 i = 0 ; i < numSpr; i++ )
				Sprite[i]->GetSpr()->SetColor(0,ctRed);
			LastFlash = Age;
			Sprite[0]->MoveR(3,0);                  //のけぞる
			HPGauge->Show();
			GaugeTime = Age;
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WAEdamage0,7);
			LastSlashed = target->LastSlashed;
		}
	}

	if( (target->Kind() == kHShot) || (target->Kind() == kMissile) ) {
		Dec(HP,target->STR());
		for( u32 i = 0 ; i < numSpr; i++ )
			Sprite[i]->GetSpr()->SetColor(0,ctRed);
		LastFlash = Age;
		Sprite[0]->MoveR(3,0);                  //のけぞる
		HPGauge->Show();
		GaugeTime = Age;
		if( CreateSE ) FOwner->SoundEffect->WavPlay(WAEdamage0,7);
		if( HP<=0 ) BonusRate = target->BonusRate;
	}

	if( HP<TempHP ) TempHP = HP;

}

void TZako2_02::Die()
{
	HPGauge->Die();
	TOBJ::Die();

}

void TZako2_02::OnBroken()
{
	TOBJ::OnBroken();
	FOwner->Add(new TExpmiddle(FOwner,X,Y,Rdeg,Speed,0));//爆発

}

void TZako2_03::MakeBar( s32 _angle )
{
	FOwner->Add(new TRevBarBasic(FOwner,this,X,Y,_angle,20,-Speed,1));
	FOwner->Add(new TRevBarBasic(FOwner,this,X+(random(10)-5)*2,Y+(random(10)-5)*3,_angle,random(5)+1,-Speed,random(10)+10));

}

TZako2_03::TZako2_03( TOBJList* owner, s32 _x, s32 _y, s32 _R, s32 _S, s32 _pat, bool _red ) : TOBJ(owner)
{
	X = _x;
	Y = _y;
	MaxHP = KSTR_SHOT*40;
	HP = MaxHP;
	TempHP = HP;
	FSTR = 1000;
	FKind = kColBG;
	Buzzed = false;
	LastBuzz = 0;
	Rdeg = _R;
	Speed = _S;
	Pattern = _pat;
	CommandRed = _red;
	GaugeTime = 0;
	TotalFlight = 0;
	LastFlash = 0;
	OriginColor  =  255;

	s32 TX,TY;
	if( FOwner->NowStage == 1 ) {
		TX = 273;
		TY = 1;
	}
	else {
		TX = 193;
		TY = 49;
	}
	MakeSprite(QD, MechaTex,  0, TX,  TY, 95, 95, 150,  0,  0,  0,  0,47, 47,false);
	Sprite[0]->GetSpr()->Move(_x,_y);

	s32 maxloop;
	if( Pattern<4 )
		maxloop = 2;
	else
		maxloop = 1;

	//当たり判定用
	for( u32 i  =  1 ; i <= maxloop ; i++) {
		MakeSpriteLite(i, 0,0,470,32,16000,bmAdd);
		if( FOwner->NowStage == 1 )
			Sprite[i]->GetSpr()->AddTexture(MechaTex, 369, 145, 384, 176);
		else
			Sprite[i]->GetSpr()->AddTexture(MechaTex, 289, 113, 304, 144);

		Sprite[i]->GetSpr()->AddHitArea(TPoint(0, 4), TPoint(0, 28), TPoint(470, 28),TPoint(470, 4), htSky);
		Sprite[i]->GetSpr()->FHit      =  true;
		Sprite[i]->GetSpr()->FCX += 235;
		Sprite[i]->GetSpr()->FCY += 16;
		Sprite[i]->GetSpr()->Move(_x,_y);
		Sprite[0]->Add(Sprite[i]);
	}


	if( Pattern<4 )
		maxloop = 6;
	else
		maxloop = 3;
	//先端
	for( u32 i  =  numSpr ; i <= maxloop; i++ ) {
		MakeSpriteLite(i, 0,0,16,32,16000,bmAdd);
		if( FOwner->NowStage == 1 )
			Sprite[i]->GetSpr()->AddTexture(MechaTex, 385, 145, 400, 176);
		else
			Sprite[i]->GetSpr()->AddTexture(MechaTex, 305, 113, 320, 144);

		Sprite[i]->GetSpr()->FCY += 16;

		if( i % 2 == 0 ) {
			Sprite[i]->GetSpr()->Move(_x-235,_y);
			Sprite[i]->GetSpr()->Rotate(2048);
		}
		else {
			Sprite[i]->GetSpr()->Move(_x+235,_y);
		}
	}

	if( Pattern<4 ) {
		Sprite[1]->Add(Sprite[3]);
		Sprite[1]->Add(Sprite[4]);
		Sprite[2]->Add(Sprite[5]);
		Sprite[2]->Add(Sprite[6]);

		Sprite[2]->Rotate(1024);
	}
	else {
		Sprite[1]->Add(Sprite[2]);
		Sprite[1]->Add(Sprite[3]);
	}
	Sprite[0]->Rotate(Rdeg);

}

void TZako2_03::Move()
{
	TOBJ::Move();

	X = (s32)(Sprite[0]->GetSpr()->FCX);
	Y = (s32)(Sprite[0]->GetSpr()->FCY);

	s32 rotspeed;
	switch( Pattern ){
	case 0:rotspeed = -8;	break;
	case 1:rotspeed = 8;	break;
	case 2:rotspeed = -24;	break;
	case 3:rotspeed = 24;	break;
	case 4:rotspeed = 16;	break;
	case 5:rotspeed = 0;	break;
	}

	if( FOwner->Difficulty == KDIFFICULTY_HARD )
		rotspeed = rotspeed*2;

	Sprite[0]->RotateR(rotspeed);
	Sprite[0]->MoveR(-Speed,0);

	if( (X < 0-250) ) {
		Die();//画面外に消えたら消す
	}
	else if( (X < 900) ) {
		if( Pattern<4 ) {
			MakeBar(0);
			MakeBar(1024);
			MakeBar(2048);
			MakeBar(3072);
		}
		else {
			MakeBar(0);
			MakeBar(2048);
		}
	}

	if( (Age-LastBuzz)>=40 ) Buzzed = false;

}

void TZako2_03::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);
	X = (s32)(Sprite[0]->GetSpr()->FCX);
	Y = (s32)(Sprite[0]->GetSpr()->FCY);
	if( target->Kind() == kHero ) {
		if( (! Buzzed) && (target->Hit[HitBuzz]) ) {
			FOwner->Scrach(this);
			Buzzed = true;
			LastBuzz = Age;
		}

		s32 HitNum;
		if( Sprite[1]->GetSpr()->FHit )
			HitNum = 1;
		else 
			HitNum = 2;   

		vector<THitArea> hitarea1,hitarea2;
		target->Sprite[1]->GetSpr()->HitAreaTransform(hitarea1);
		Sprite[HitNum]->GetSpr()->HitAreaTransform(hitarea2);
		glm::vec2 vec = GetReflectionVector(glm::vec2(hitarea1[0].RPointAry[0].x, hitarea1[0].RPointAry[0].y), &hitarea2[0]);

		target->CollisionReaction(vec.x, vec.y);

	}

	if( (target->Kind() == kHShot) || (target->Kind() == kMissile) ) {
		if( CreateSE ) FOwner->SoundEffect->WavPlay(WAEdamage1,7);
	}
	if( HP<TempHP ) TempHP = HP;

}

void TZako2_03::Die()
{
	TOBJ::Die();
}

void TZako2_04::MakeShot()
{
	f32 GX = Sprite[0]->GetSpr()->FCX;
	f32 GY = Sprite[0]->GetSpr()->FCY;
	PosRoll(GX, GY, Sprite[0]->GetSpr()->FCX,Sprite[0]->GetSpr()->FCY,Sprite[0]->Rdegree);

	s32 GunR = Sprite[0]->Rdegree + 1024 & 0x0fff;

	switch( FOwner->Difficulty ){
	case KDIFFICULTY_SINGLE:
		{
			if( EffLevel>0 ) {
				for( u32 i = 0; i <= 20; i++ )
					FOwner->Add(new TParticle2(FOwner,RoundOff(GX),RoundOff(GY),1,GunR+random(768*2)-768,random(15)+5,random(5)+1));
			}

			FOwner->Add(new TFunBeamCore(FOwner,RoundOff(GX),RoundOff(GY),GunR,2));

			if( CreateSE ) FOwner->SoundEffect->WavPlay(WABeam,9);
		}break;
	default:
		{
			if( FOwner->Difficulty != KDIFFICULTY_HARD ) {
				if( FOwner->Difficulty >= KDIFFICULTY_NORMAL ) {
					FOwner->Add(new TZ01Shot4(FOwner,X,Y,GunR+384,3,BSpeed,0,500));
					FOwner->Add(new TZ01Shot4(FOwner,X,Y,GunR-384,3,BSpeed,0,500));
				}
				FOwner->Add(new TZ01Shot4(FOwner,X,Y,GunR    ,3,BSpeed,0,500));
			}
			else {
				FOwner->Add(new TZ01Shot4(FOwner,X,Y,GunR+512,8,BSpeed,0,500));
				FOwner->Add(new TZ01Shot4(FOwner,X,Y,GunR-512,8,BSpeed,0,500));
				FOwner->Add(new TZ01Shot4(FOwner,X,Y,GunR+256,8,BSpeed,0,500));
				FOwner->Add(new TZ01Shot4(FOwner,X,Y,GunR-256,8,BSpeed,0,500));
				FOwner->Add(new TZ01Shot4(FOwner,X,Y,GunR    ,8,BSpeed,0,500));
			}

			if( CreateSE ) FOwner->SoundEffect->WavPlay(WABShot,9);
		}
	}
	FOwner->Add(new TMissileFlash(FOwner,RoundOff(GX),RoundOff(GY)));

}

void TZako2_04::Accel()
{
	f32 maxspeed,minspeed;
	if( (FOwner->Speed >= 8) )
		BSpeed = BSpeed+0.4f;
	else 
		BSpeed = BSpeed-0.4f;

	switch( FOwner->Difficulty ){
	case KDIFFICULTY_SINGLE:{ maxspeed = 3; minspeed = -1; }		break;
	default: {
		maxspeed = 5; minspeed = 0;
			 }
	}

	if( BSpeed>maxspeed )
		BSpeed = maxspeed;

	if( BSpeed<minspeed )
		BSpeed = minspeed;

	Sprite[0]->MoveR(BSpeed,0);

}

void TZako2_04::Transfer()
{
	s32 angularVel;
	if( FOwner->IsSequence() )
		angularVel = 32;
	else
		angularVel = 16;
	s32 TransSpeed = 0;
	s32 currentRad =  ActCount[KACTION_S2Z4_TRANSFER];
	s32 prevRad =  ActCount[KACTION_S2Z4_TRANSFER]-angularVel;
	if( prevRad>0 ) TransSpeed = -RoundOff(IniDis/2.f*(cost[currentRad]-cost[prevRad]));

	switch( ActFlg[KACTION_S2Z4_TRANSFER] ){
		case 1:{    //位置決定
			s32 DX = DestX-X;
			s32 DY = DestY-Y;
			IniDis = RoundOff(sqrt(DX*DX+DY*DY));
			DestDeg = RoundOff(atan2(DY,DX)/(f32)M_PI*2048.f) & 0x0fff;
			Inc(ActFlg[KACTION_S2Z4_TRANSFER]);
		}break;
		case 2:{    //移動
			Sprite[0]->MoveR((f32)TransSpeed * cost[DestDeg], (f32)TransSpeed * sint[DestDeg]);

			if( ActCount[KACTION_S2Z4_TRANSFER] >= 2048 ) {
				ActFlg[KACTION_S2Z4_TRANSFER] = 0;
				ActCount[KACTION_S2Z4_TRANSFER] = 0;
			}
		}break;
	}
	Inc(ActCount[KACTION_S2Z4_TRANSFER], angularVel);

}

void TZako2_04::Appear()
{
	switch( ActFlg[KACTION_S2Z4_APPEAR] ){
		case 1:{
			DestY = Y;
			DestX = X-200;

			ActFlg[KACTION_S2Z4_TRANSFER] = 1;
			ActCount[KACTION_S2Z4_TRANSFER] = 0;
			ActFlg[KACTION_S2Z4_APPEAR] = 0;
		}break;
	}

}

void TZako2_04::Escape()
{
	if( ActFlg[KACTION_S2Z4_TRANSFER]=0 ) {
		s32 DY  =  abs(FOwner->JikiObj->Y - Y);

		if( DY<80 ) {
			while( DY<80 ) {
				DestY = FOwner->GetRand(3)*100 + 50;
				DY  =  abs(DestY - Y);
			}
			DestX = X;

			ActFlg[KACTION_S2Z4_TRANSFER] = 1;
			ActCount[KACTION_S2Z4_TRANSFER] = 0;
		}
	}

}

void TZako2_04::Action()
{
	if( HP>0 ) {
		if( ActFlg[KACTION_S2Z4_CHARGE]>0 ) ChargeMotion();
	}

	switch( Pattern ){
		case 0:{
			Accel();
			Escape();

			Sprite[numSpr-1]->GetSpr()->SetColorR(-16,ctAlpha);    //眼光を徐々に消す
			if( (Age>180) && (Age % 120 == 0) ) ActFlg[KACTION_S2Z4_CHARGE] = 1;
		}
		case 1:{
			Accel();
			if( Age == 1 )
				ActFlg[KACTION_S2Z4_APPEAR] = 1;

			if( (Age % 120 == 0) ) ActFlg[KACTION_S2Z4_CHARGE] = 1;

			if( (Age>1) && (ActFlg[KACTION_S2Z4_APPEAR] == 0) ) {
				Escape();
			}
		}break;
		case 2:{
			if( FOwner->Distance < 51500 )
				Accel();
			else
				Sprite[0]->MoveR(-5,0);

			ShotMotion();
		}break;
	}

}

void TZako2_04::ChargeMotion()
{
	switch( ActFlg[KACTION_S2Z4_CHARGE] ){
		case 1:{
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WACharge,8);

			Inc(ActFlg[KACTION_S2Z4_CHARGE]);
			ActCount[KACTION_S2Z4_CHARGE] = -1;
		}break;
		case 2:{
			s32 ParticleR = random(4095);
			if( ActCount[KACTION_S2Z4_CHARGE] % 2 == 0 ) {
				FOwner->Add(new TBLight(FOwner,X,Y,(f32)ActCount[KACTION_S2Z4_CHARGE]*0.1f,12000));
				FOwner->Add(new TParticle2(FOwner,X+RoundOff(60.f*cost[ParticleR]),Y+RoundOff(60.f*sint[ParticleR]),1,ParticleR,-1,30));
			}
			else {
				FOwner->Add(new TBLight(FOwner,X,Y,(f32)ActCount[KACTION_S2Z4_CHARGE]*0.09f,12000));
			}

			s32 GunR = Sprite[0]->Rdegree + 1024 & 0x0fff;

			switch( FOwner->Difficulty ){
			case KDIFFICULTY_SINGLE:
				FOwner->Add(new TBeamSite(FOwner,X,Y,GunR,ActCount[KACTION_S2Z4_CHARGE]*2));
			}


			if( ActCount[KACTION_S2Z4_CHARGE] == 60 ) {
				Inc(ActFlg[KACTION_S2Z4_CHARGE]);
				ActCount[KACTION_S2Z4_CHARGE] = 0;
			}
		}break;
		case 3:{
			MakeShot();

			ActFlg[KACTION_S2Z4_CHARGE] = 0;
			ActCount[KACTION_S2Z4_CHARGE] = -1;
		}
	}
	Inc(ActCount[KACTION_S2Z4_CHARGE]);

}

void TZako2_04::ShotMotion()
{
	switch( ActFlg[KACTION_S2Z4_SHOT] ){
		//登場
		case 0:{
			Sprite[numSpr-1]->GetSpr()->SetColorR(-16,ctAlpha);  //眼光を徐々に消す

			if( ActCount[KACTION_S2Z4_SHOT] == 60 ) {
				DestX = HomePosX+FOwner->GetRand(128)-64;
				DestY = FOwner->GetRand(7)*50 + 50;
				ActFlg[KACTION_S2Z4_TRANSFER] = 1;
				ActCount[KACTION_S2Z4_TRANSFER] = 0;

				Inc(ActFlg[KACTION_S2Z4_SHOT]);
				ActCount[KACTION_S2Z4_SHOT] = 0;
			}
		}break;
		//持ち場に移動～射撃
		case 1:{

			if( ActFlg[KACTION_S2Z4_TRANSFER] == 0 ) {
				Inc(ActFlg[KACTION_S2Z4_SHOT]);
				ActCount[KACTION_S2Z4_SHOT] = 0;

				if( (Age>180) ) ActFlg[KACTION_S2Z4_CHARGE] = 1;
			}
		}break;
		//射撃完了～移動
		case 2:{
			if( ActFlg[KACTION_S2Z4_CHARGE] == 0 ) {
				ActFlg[KACTION_S2Z4_SHOT] = 1;
				ActCount[KACTION_S2Z4_SHOT] = 0;

				DestX = HomePosX+FOwner->GetRand(128)-64;
				DestY = FOwner->GetRand(7)*50 + 50;
				ActFlg[KACTION_S2Z4_TRANSFER] = 1;
				ActCount[KACTION_S2Z4_TRANSFER] = 0;
			}
		}break;
	}
	Inc(ActCount[KACTION_S2Z4_SHOT]);

}

TZako2_04::TZako2_04( TOBJList* owner, s32 _x, s32 _y, s32 _pat, bool _red ) : TOBJ(owner)
{
	FObjectName = "TZako2_04";

	X = _x;
	Y = _y;
	HomePosX = X;
	switch( FOwner->Difficulty ){
	case KDIFFICULTY_SINGLE:
	case KDIFFICULTY_EASY: 
		MaxHP = 40;
		break;
	case KDIFFICULTY_NORMAL:
	case KDIFFICULTY_HARD: 
		{
			if( _red )
				MaxHP = KSTR_SHOT*48;
			else
				MaxHP = 60;
		}break;
	}

	HP = MaxHP;
	TempHP = HP;
	FSTR = 0;      //本体にあたってもノーダメージ
	FKind = kEnemy;
	Score = 5000;
	ComBonus = 5000;
	Buzzed = false;
	LastBuzz = 0;
	GaugeTime = 0;
	TotalFlight = 0;
	LastFlash = 0;
	OriginColor = 255;
	BSpeed = 0;
	IniDis = 0;
	DestDeg = 0;
	Pattern = _pat;
	CommandRed = _red;

	for( u32 i = 0; i <= 99; i++ ) {
		ActFlg[i] = 0;
		ActCount[i] = 0;
	}

	CurrentMotion = KMOTION_STAGE2ZAKO4_DEFAULT;

	//IDの付与
	FID = NewID;
	Inc(NewID);

	MakeSprFromID(FOwner->GetDG(), KSPRDATA_STAGE2ZAKO4, MechaTex, 11000, ExtraEffects);

	if( CommandRed ) {
		for( u32 i = 0 ; i < numSpr; i++ ) {
			Sprite[i]->GetSpr()->SetColor(64, ctRed);
			Sprite[i]->GetSpr()->SetColor(0, ctBlue);
			Sprite[i]->GetSpr()->SetColor(0, ctGreen);
		}
	}

	//目
	MakeSpriteLite(numSpr, 0,0,80,80,10500,bmAdd);
	Sprite[numSpr-1]->GetSpr()->AddTexture(MechaTex, 289, 97, 368, 176);
	Sprite[numSpr-1]->GetSpr()->FCX  += 40;
	Sprite[numSpr-1]->GetSpr()->FCY  += 40;
	Sprite[numSpr-1]->GetSpr()->Move(X-3, Y+8);
	Sprite[numSpr-1]->Sticky = true;
	Sprite[0]->Add(Sprite[numSpr-1]);

	//眼光
	if( (Pattern == 0) || (Pattern == 2) ) {
		MakeSpriteLite(numSpr, 0,0,176,160,10500,bmAdd);
		Sprite[numSpr-1]->GetSpr()->AddTexture(MechaTex, 338, 178, 511, 353);
		Sprite[numSpr-1]->GetSpr()->FCX  += 88;
		Sprite[numSpr-1]->GetSpr()->FCY  += 80;
		Sprite[numSpr-1]->GetSpr()->Move(X-3, Y+8);
		Sprite[numSpr-1]->Sticky = true;
		Sprite[0]->Add(Sprite[numSpr-1]);
	}

	HPGauge  =  new TEnemyGauge2(FOwner,this,-128,32);
	FOwner->Add(HPGauge);

}

void TZako2_04::Move()
{
	TOBJ::Move();

	X = RoundOff(Sprite[0]->GetSpr()->FCX);
	Y = RoundOff(Sprite[0]->GetSpr()->FCY);

	f32 DX = FOwner->JikiObj->X-X;             //自分と敵との距離を計算
	f32 DY = FOwner->JikiObj->Y-Y;

	EnemyDeg = RoundOff(atan2(DY,DX)/(f32)M_PI*2048.f);
	EnemyDeg  =  EnemyDeg - 1024 & 0x0fff;

	//現在の頭の角度からポーズを決める
	if( 0 <= Sprite[0]->Rdegree && Sprite[0]->Rdegree <= 512 )
		CurrentMotion  =  KMOTION_STAGE2ZAKO4_LEFT;
	else if(513 <= Sprite[0]->Rdegree && Sprite[0]->Rdegree <= 1500)
		CurrentMotion  =  KMOTION_STAGE2ZAKO4_BACK;
	else if(1501 <= Sprite[0]->Rdegree && Sprite[0]->Rdegree <= 2500)
		CurrentMotion  =  KMOTION_STAGE2ZAKO4_RIGHT;
	else if(2501 <= Sprite[0]->Rdegree && Sprite[0]->Rdegree <= 3500)
		CurrentMotion  =  KMOTION_STAGE2ZAKO4_FOWARD;
	else if(3501 <= Sprite[0]->Rdegree && Sprite[0]->Rdegree <= 4095)
		CurrentMotion  =  KMOTION_STAGE2ZAKO4_LEFT;

	//ポーズの目的角度に敵の角度を足す
	for( u32 i  =  0 ; i < FOwner->MotionDataList[CurrentMotion].TotalSpr; i++) {
		Inc(FOwner->MotionDataList[CurrentMotion].Sprites[i].Rdeg, EnemyDeg);
	}

	MotionControl(CurrentMotion);

	//ポーズの目的角度をデフォルトに戻す
	for( u32 i  =  0 ; i < FOwner->MotionDataList[CurrentMotion].TotalSpr; i++ ) {
		Dec(FOwner->MotionDataList[CurrentMotion].Sprites[i].Rdeg, EnemyDeg);
	}

	Transfer();
	Action();
	Appear();

	if( (X < -200) ) {
		Die();//画面外に消えたら消す & 母艦にダメージ
		if( FOwner->IsSequence() )
			Dec(FOwner->MShipHP, 500);
		else
			Dec(FOwner->MShipHP,FSTR);
		FOwner->Add(new TMotherShipDamaged(FOwner, X, Y));
	}
	if( HP<=0 ) OnBroken();                        //HPが無くなったら死ぬ

	if( (Age-GaugeTime)>120 ) HPGauge->Hide();


	if( (Age-LastFlash == 2) ) {
		for( u32 i = 0 ; i <= numSpr-3; i++ ) {
			if( CommandRed || (! ExtraEffects))
				Sprite[i]->GetSpr()->SetColor(64,ctRed);          //色を戻す
			else
				Sprite[i]->GetSpr()->SetColor(255,ctRed);          //色を戻す
		}
		Sprite[0]->MoveR(-3,0);                  //のけぞり戻し
	}

}

void TZako2_04::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);
	X = RoundOff(Sprite[0]->GetSpr()->FCX);
	Y = RoundOff(Sprite[0]->GetSpr()->FCY);
	if( target->Kind() == kHero ) {
		if( (! Buzzed) && (target->Hit[HitBuzz]) ) {
			FOwner->Scrach(this);
			Buzzed = true;
			LastBuzz = Age;
		}
		if( (target->Hit[HitBlade]) && (target->LastSlashed > LastSlashed) ) {
			Dec(HP,target->STR());
			for( u32 i = 0 ; i < numSpr; i++ )
				Sprite[i]->GetSpr()->SetColor(0,ctRed);
			LastFlash = Age;
			if( (! FOwner->IsSequence()) || (X<640) ) Sprite[0]->MoveR(3,0);                  //のけぞる
			HPGauge->Show();
			GaugeTime = Age;
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WAEdamage0,7);
			LastSlashed = target->LastSlashed;
		}
	}

	if( (target->Kind() == kHShot) || (target->Kind() == kMissile) ) {
		Dec(HP,target->STR());
		for( u32 i = 0 ; i <= numSpr-3; i++ )
			Sprite[i]->GetSpr()->SetColor(0,ctRed);
		LastFlash = Age;
		if( (! FOwner->IsSequence()) || (X<640) ) Sprite[0]->MoveR(3,0);                  //のけぞる
		HPGauge->Show();
		GaugeTime = Age;
		if( CreateSE ) FOwner->SoundEffect->WavPlay(WAEdamage0,7);
		if( HP<=0 ) BonusRate = target->BonusRate;
	}

	if( HP<TempHP ) TempHP = HP;

}

void TZako2_04::Die()
{
	TOBJ::Die();
	HPGauge->Die();

}

void TZako2_04::OnBroken()
{
	TOBJ::OnBroken();
	FOwner->Add(new TExpmiddle(FOwner,X,Y,0,0,0));//爆発

}

TSubmarine::TSubmarine( TOBJList* owner, s32 _x, s32 _y ) : TOBJ(owner)
{
	FObjectName = "TSubmarine";

	X = _x;
	Y = _y;
	FKind = kEffect;

	MakeSpriteLite(numSpr, X,Y,700,192,25001,bmNormal);
	Sprite[numSpr-1]->GetSpr()->AddTexture(DemoTex, 1, 321, 512, 512);

	MakeSpriteLite(numSpr, 0,Y,1200,100,25000,bmMul);
	Sprite[numSpr-1]->GetSpr()->AddTexture(DemoTex, 1, 257, 512, 320);

	MakeSpriteLite(numSpr, X,Y,700,192,15000,bmNormal);
		Sprite[numSpr-1]->GetSpr()->AddTexture(DemoTex, 1, 321, 512, 512);
		Sprite[numSpr-1]->GetSpr()->SetColor(140,ctAlpha);
		Sprite[numSpr-1]->GetSpr()->FEffectNum = 1;
		Sprite[numSpr-1]->GetSpr()->FBumpSize = 1.f;
		Sprite[numSpr-1]->GetSpr()->FBumpTex = DemoTex;
		Sprite[numSpr-1]->GetSpr()->FBumpLevel = 2.0f;
	Sprite[0]->Add(Sprite[numSpr-1]);
}

void TSubmarine::Move()
{
	TOBJ::Move();
	Sprite[0]->MoveR(2, 0);
	Sprite[1]->MoveR(-1, 0);

}

void TZako2_05::MakeShot()
{
	for( u32 i = 0; i <= 2; i++ ) {
		if( FOwner->IsSequence() ) {
			FOwner->Add(new TZ01Shot4(FOwner,X,Y,Rdeg+1792,5+i,0,0,500));
			FOwner->Add(new TZ01Shot4(FOwner,X,Y,Rdeg+2304,5+i,0,0,500));
		}
		else {
			FOwner->Add(new TZ01Shot2(FOwner,X,Y,Rdeg+1792,5+i,0));
			FOwner->Add(new TZ01Shot2(FOwner,X,Y,Rdeg+2304,5+i,0));
		}
		FOwner->Add(new TFCircle(FOwner,X,Y,1,Rdeg,RoundOff(Speed)));
		if( CreateSE ) FOwner->SoundEffect->WavPlay(WABShot,9);
	}
}

void TZako2_05::Accel()
{

}

void TZako2_05::Action()
{
	if( ActFlg[KACTION_S2Z5_BEAM]>0 ) BeamMotion();

	switch( Pattern ){
		case 0:{
			switch( ActFlg[KACTION_S2Z5_ACT1] ){
				case 0:{
					Sprite[0]->SetDest(DestX, DestY, 32);
					Inc(ActFlg[KACTION_S2Z5_ACT1]);
					ActCount[KACTION_S2Z5_ACT1] = 0;
				}break;
				case 1:{
					Sprite[0]->SetDestR(KSCROLLSPEED_NORMAL - FOwner->Speed, 0);
					Sprite[0]->AutoSlide();
					//ビームチャージ開始
					if( (! FOwner->IsSequence()) && (ActCount[KACTION_S2Z5_ACT1] == 20) ) ActFlg[KACTION_S2Z5_BEAM] = 1;

					if( ActCount[KACTION_S2Z5_ACT1] == 60 ) {
						if( FOwner->IsSequence() ) MakeShot();

						Inc(ActFlg[KACTION_S2Z5_ACT1]);
						ActCount[KACTION_S2Z5_ACT1] = 0;
					}
				}break;
				case 2:{
					Sprite[0]->SetDestR(KSCROLLSPEED_NORMAL - FOwner->Speed, 0);
					Sprite[0]->AutoSlide();

					if( (FOwner->IsSequence()) ) {
						if( (ActCount[KACTION_S2Z5_ACT1] == 60) ) ActFlg[KACTION_S2Z5_BEAM] = 1;

						if( (ActCount[KACTION_S2Z5_ACT1]>60) && (ActFlg[KACTION_S2Z5_BEAM] == 0) ) {
							Inc(ActFlg[KACTION_S2Z5_ACT1]);
							ActCount[KACTION_S2Z5_ACT1] = 0;
						}
					}
					else {
						if( ActFlg[KACTION_S2Z5_BEAM] == 0 ) {
							Inc(ActFlg[KACTION_S2Z5_ACT1]);
							ActCount[KACTION_S2Z5_ACT1] = 0;
						}
					}
				}break;
				case 3:{
					switch( FOwner->Difficulty ){
					case KDIFFICULTY_SINGLE: {
						switch( ActCount[KACTION_S2Z5_ACT1] ){
						case 20:
						case 50:
						case 80:MakeMissile();	break;
						}
					}break;
					case KDIFFICULTY_NORMAL: {
					switch( ActCount[KACTION_S2Z5_ACT1] ){
						case 50:MakeMissile();	break;
					}
					}break;
				case KDIFFICULTY_HARD: {
					switch( ActCount[KACTION_S2Z5_ACT1] ){
					case 20:
					case 50:
					case 80:MakeMissile();
						break;
					}
				  }break;
					}


					if( ActCount[KACTION_S2Z5_ACT1] == 100 ) {
						Inc(ActFlg[KACTION_S2Z5_ACT1]);
						ActCount[KACTION_S2Z5_ACT1] = 0;
					}
				}break;
				case 4:{
					f32 escapespeed = (f32)ActCount[KACTION_S2Z5_ACT1]/4.f;
					if( escapespeed>3 ) escapespeed = 3;

					Sprite[0]->MoveR(-escapespeed,0);
				}break;
			}

		}break;
		case 1:{
			switch( ActFlg[KACTION_S2Z5_ACT1] ){
				case 0:{
					Rdeg = 1024;
					Sprite[0]->Rotate(Rdeg);
					if( CreateSE ) FOwner->SoundEffect->WavPlay(WALBeam,10);
					Inc(ActFlg[KACTION_S2Z5_ACT1]);
					ActCount[KACTION_S2Z5_ACT1] = 0;
				}break;
				case 1:{
					Sprite[0]->MoveR(-5,0);
					BeamMotion2();
				}break;
			}
		}break;
		case 2:{
			switch( ActFlg[KACTION_S2Z5_ACT1] ){
				case 0:{
					Rdeg = 3072;
					Sprite[0]->Rotate(Rdeg);
					if( CreateSE ) FOwner->SoundEffect->WavPlay(WALBeam,10);
					Inc(ActFlg[KACTION_S2Z5_ACT1]);
					ActCount[KACTION_S2Z5_ACT1] = 0;
				}break;
				case 1:{
					Sprite[0]->MoveR(-5,0);
					BeamMotion2();
				}break;
			}
		}break;
		case 3:{

		}break;
	}

	Inc(ActCount[KACTION_S2Z5_ACT1]);

}

void TZako2_05::BeamMotion()
{
	const s32 RandomTable[] = { 4,2,5,1,3,0 };
	s32 GunR = (Rdeg+2048) & 0x0fff;
	s32 GX = X+RoundOff(128.f*cost[GunR]);
	s32 GY = Y;

	s32 ParticleR = random(4095);
	switch( ActFlg[KACTION_S2Z5_BEAM] ){
		case 1:{
			//初期化
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WACharge,8);
			Inc(ActFlg[KACTION_S2Z5_BEAM]);
			ActCount[KACTION_S2Z5_BEAM] = 0;
		}break;
		case 2:{
			//チャージ
			if( ActCount[KACTION_S2Z5_BEAM] % 2 == 0 ) {
				FOwner->Add(new TBLight(FOwner,GX,GY,(f32)ActCount[KACTION_S2Z5_BEAM]*0.06f,12000));
				FOwner->Add(new TParticle2(FOwner,GX+RoundOff(60.f*cost[ParticleR]),GY+RoundOff(60.f*sint[ParticleR]),1,ParticleR,-1,30));
			}
			else {
				FOwner->Add(new TBLight(FOwner,GX,GY,(f32)ActCount[KACTION_S2Z5_BEAM]*0.05f,12000));
			}

			if( ActCount[KACTION_S2Z5_BEAM] == 70 ) {
				Inc(ActFlg[KACTION_S2Z5_BEAM]);
				ActCount[KACTION_S2Z5_BEAM] = 0;
				FOwner->Add(new TSRControl(FOwner,GX,GY,GunR+2048));
				if( CreateSE ) FOwner->SoundEffect->WavPlay(WALBeam,10);
			}
		}break;
		case 3:{
			//発射開始
			if( ActCount[KACTION_S2Z5_BEAM] % 3 == 0 )
				FOwner->Add(new TZBeamPart(FOwner,GX+RoundOff(80.f*cost[GunR]),GY+RoundOff(80.f*sint[GunR]),GunR,1.2f+(f32)ActCount[KACTION_S2Z5_BEAM]*0.009f+(f32)RandomTable[Age % 6]*0.05f,false));

			FOwner->Add(new TBLight(FOwner,GX,GY,4,12000));
			if( ActCount[KACTION_S2Z5_BEAM] == 8 ) {
				Inc(ActFlg[KACTION_S2Z5_BEAM]);
				ActCount[KACTION_S2Z5_BEAM] = 0;
			}
		}break;
		case 4:{
			//発射継続
			if( ActCount[KACTION_S2Z5_BEAM] % 3 == 0 )
				FOwner->Add(new TZBeamPart(FOwner,GX+RoundOff(80.f*cost[GunR]),GY+RoundOff(80.f*sint[GunR]),GunR,1.f+(f32)RandomTable[Age % 6]*0.05f,true));
			if( ActCount[KACTION_S2Z5_BEAM] % 2 == 0 ) {
				FOwner->Add(new TBLight(FOwner,GX,GY,6.f+(f32)RandomTable[Age % 6]*0.1f,12000));
			}
			if( ActCount[KACTION_S2Z5_BEAM] == 40 ) {
				Inc(ActFlg[KACTION_S2Z5_BEAM]);
				ActCount[KACTION_S2Z5_BEAM] = 0;
			}

		}break;
		case 5:{
			//収束
			if( ActCount[KACTION_S2Z5_BEAM] % 3 == 0 )
				FOwner->Add(new TZBeamPart(FOwner,GX+RoundOff(80.f*cost[GunR]),GY+RoundOff(80.f*sint[GunR]),GunR,1.0f-(f32)(ActCount[KACTION_S2Z5_BEAM])*0.057f+(f32)RandomTable[Age % 6]*0.02f,false));
			if( ActCount[KACTION_S2Z5_BEAM] % 2 == 0 ) {
				FOwner->Add(new TBLight(FOwner,GX,GY,6.f-(f32)(ActCount[KACTION_S2Z5_BEAM])*0.4f,12000));
			}
			if( ActCount[KACTION_S2Z5_BEAM] == 15 ) {
				ActFlg[KACTION_S2Z5_BEAM] = 0;
				ActCount[KACTION_S2Z5_BEAM] = 0;
			}
		}
	}
	Inc(ActCount[KACTION_S2Z5_BEAM]);
}

void TZako2_05::MakeMissile()
{
	FOwner->Add(new TZ05Missile(FOwner,X,Y,FOwner->GetRand(7)));
	FOwner->Add(new TMissileFlash(FOwner,X,Y));
	if( CreateSE )  FOwner->SoundEffect->WavPlay(WAMissile,4);

}

void TZako2_05::BeamMotion2()
{
	const s32 RandomTable[] = { 4,2,5,1,3,0 };
	s32 GunR = (Rdeg+2048) & 0x0fff;
	s32 GX = X+RoundOff(128.f*cost[GunR]);
	s32 GY = Y+RoundOff(128.f*sint[GunR]);
	if( ActCount[KACTION_S2Z5_ACT1] % 3 == 0 )
		FOwner->Add(new TZBeamPart2(FOwner,GX+RoundOff(80.f*cost[GunR]),GY+RoundOff(80.f*sint[GunR]),-5,0,GunR,1.f+(f32)RandomTable[Age % 6]*0.05f,true));
	if( ActCount[KACTION_S2Z5_ACT1] % 2 == 0 ) {
		FOwner->Add(new TBLight(FOwner,GX,GY,6.f+(f32)RandomTable[Age % 6]*0.1f,12000));
	}
}

TZako2_05::TZako2_05( TOBJList* owner, s32 _x, s32 _y, s32 _dx, s32 _dy, s32 _pat, bool _red ) : TOBJ(owner)
{
	X = _x;
	Y = _y;
	DestX = _dx;
	DestY = _dy;
	MaxHP = KSTR_SHOT*12;
	HP = MaxHP;
	TempHP = HP;
	FSTR = 500;
	FKind = kEnemy;
	Score = 10000;
	Buzzed = false;
	LastBuzz = 0;
	Pattern = _pat;
	CommandRed = _red;
	GaugeTime = 0;
	TotalFlight = 0;
	LastFlash = 0;
	OriginColor  =  255;
	ComBonus = 5000;
	//IDの付与
	FID = NewID;
	Inc(NewID);

	MakeSprite(QD, MechaTex,  0,1,65,192, 80,115,  0,  0,  192,  80, 128, 40,CommandRed);
	Sprite[0]->GetSpr()->Move(_x,_y);

	if( ExtraEffects ) {
		MakeSprite(QD, MechaTex,  1,1,65,192, 80,114,  0,  0,  0,  0, 128, 40,CommandRed);
			Sprite[1]->GetSpr()->Move(_x,_y);
			Sprite[1]->GetSpr()->FEffectNum = 1;
			Sprite[1]->GetSpr()->FBumpSize = 3.5f;
			Sprite[1]->GetSpr()->FBumpTex = MechaTex;
			Sprite[1]->GetSpr()->FBumpLevel = -0.2f;
			Sprite[1]->GetSpr()->SetColor(128,ctAlpha);
		Sprite[0]->Add(Sprite[1]);
	}

	HPGauge  =  new TEnemyGauge2(FOwner,this,-128,32);
	FOwner->Add(HPGauge);

}

void TZako2_05::Move()
{
	TOBJ::Move();
	X = (s32)(Sprite[0]->GetSpr()->FCX);
	Y = (s32)(Sprite[0]->GetSpr()->FCY);

	Action();

	if( (Y > 480+64) || (X < 0-64) ) {
		Die();//画面外に消えたら消す & 母艦にダメージ
		Dec(FOwner->MShipHP,FSTR*2);
		FOwner->Add(new TMotherShipDamaged(FOwner, X, Y));
	}

	if( (Age-LastFlash == 2) ) {
		if( CommandRed || (! ExtraEffects) ) {
			Sprite[0]->GetSpr()->SetColor(255,ctRed);          //色を戻す
		}
		else {
			Sprite[0]->GetSpr()->SetColor(OriginColor,ctRed);          //色を戻す
		}
		Sprite[0]->MoveR(-3,0);                  //のけぞり戻し
	}

	if( (Age-LastBuzz)>=40 ) Buzzed = false;
	if( HP<=0 ) OnBroken();                        //HPが無くなったら死ぬ

	if( (Age-GaugeTime)>120 ) HPGauge->Hide();

}

void TZako2_05::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);
	X = RoundOff(Sprite[0]->GetSpr()->FCX);
	Y = RoundOff(Sprite[0]->GetSpr()->FCY);
	if( target->Kind() == kHero ) {
		if( (! Buzzed) && (target->Hit[HitBuzz]) ) {
			FOwner->Scrach(this);
			Buzzed = true;
			LastBuzz = Age;
		}
		if( (target->Hit[HitBlade]) && (target->LastSlashed > LastSlashed) ) {
			Dec(HP,target->STR());
			Sprite[0]->GetSpr()->SetColor(0,ctRed);
			LastFlash = Age;
			Sprite[0]->MoveR(3,0);                  //のけぞる
			HPGauge->Show();
			GaugeTime = Age;
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WAEdamage0,7);
			LastSlashed = target->LastSlashed;
		}
	}

	if( (target->Kind() == kHShot) || (target->Kind() == kMissile) ) {
		Dec(HP,target->STR());
		Sprite[0]->GetSpr()->SetColor(0,ctRed);
		LastFlash = Age;
		Sprite[0]->MoveR(3,0);                  //のけぞる
		HPGauge->Show();
		GaugeTime = Age;
		if( CreateSE ) FOwner->SoundEffect->WavPlay(WAEdamage0,7);
		if( HP<=0 ) BonusRate = target->BonusRate;
	}

	if( HP<TempHP ) TempHP = HP;

}

void TZako2_05::Die()
{
	TOBJ::Die();
	HPGauge->Die();

}

void TZako2_05::OnBroken()
{
	TOBJ::OnBroken();
	FOwner->Add(new TExplarge(FOwner,X,Y,0,true));//爆発

}

void TZako2_06::MakeShot()
{
	if( FOwner->IsSequence() ) {
		s32 DX = RoundOff(FOwner->HeroPos.X-X);             //自機と敵との距離を計算
		s32 DY = RoundOff(FOwner->HeroPos.Y-Y);
		s32 GunR = RoundOff(atan2(DY,DX)/(f32)M_PI*2048.f) & 0xfff;
		f32 TargetDis = sqrt(DX*DX+DY*DY);
		if( (TargetDis>150) ) FOwner->Add(new TZ01Shot4(FOwner,X,Y,GunR,4,0,0,500));
	}
	else
		FOwner->Add(new TZ01Shot(FOwner,X,Y));
	FOwner->Add(new TFCircle(FOwner,X,Y,1,Rdeg,Speed));
	if( CreateSE ) FOwner->SoundEffect->WavPlay(WABShot,9);

	Inc(ShotCount);


}

void TZako2_06::Action()
{
	switch( Pattern ){
		case 0:{
			//直線
			Sprite[0]->SlideMoveR(-Speed,0);
		}break;
		case 1:{
			//ゆるい自機追尾
			if( (X<640) ) {
				if( FOwner->HeroPos.Y>Y )
					Sprite[0]->RotateR(-6);
				else if( FOwner->HeroPos.Y<Y )
					Sprite[0]->RotateR(6);
			}

			Sprite[0]->SlideMoveR(-Speed,0);

			if( (ActFlg[KACTION_S2Z6_ACT1] == 0) && (X<600) ) {
				MakeShot();
				ActFlg[KACTION_S2Z6_ACT1] = 1;
			}
		}break;
		//上から
		case 2:{
			if( Y>0 ) Sprite[0]->RotateR(15);
			Sprite[0]->SlideMoveR(-Speed,0);
			if( (Age == 20) ) MakeShot();
		}break;
		case 3:{
			//下から
			if( Y<480 ) Sprite[0]->RotateR(-15);
			Sprite[0]->SlideMoveR(-Speed,0);
			if( (Age == 20) ) MakeShot();
		}break;
		case 4:{
			//蛇行
			s32 rotatespeed = 32;

			switch( ActFlg[KACTION_S2Z6_ACT1] ){
				case 0:{
					if( Sprite[0]->Rdegree!=1024 )
						Sprite[0]->RotateR(rotatespeed);
					else {
						ActFlg[KACTION_S2Z6_ACT1] = 1;
						if( FOwner->Difficulty == KDIFFICULTY_HARD ) MakeShot();

					}
				}break;
				case 1:{
					if( Sprite[0]->Rdegree!=3072 )
						Sprite[0]->RotateR(-rotatespeed);
					else {
						ActFlg[KACTION_S2Z6_ACT1] = 0;
					}
				}break;
			}

			Sprite[0]->MoveR(-1.f,-5.f*sint[Sprite[0]->Rdegree]);

		}break;
		case 5:{
			//蛇行2
			s32 rotatespeed = 32;

			switch( ActFlg[KACTION_S2Z6_ACT1] ){
				case 0:{
					if( Sprite[0]->Rdegree!=3072 )
						Sprite[0]->RotateR(-rotatespeed);
					else {
						ActFlg[KACTION_S2Z6_ACT1] = 1;
						if( FOwner->Difficulty == KDIFFICULTY_HARD ) MakeShot();
					}
				}break;
				case 1:{
					if( Sprite[0]->Rdegree!=1024 )
						Sprite[0]->RotateR(rotatespeed);
					else {
						ActFlg[KACTION_S2Z6_ACT1] = 0;
					}
				}break;
			}

			Sprite[0]->MoveR(-1.f,-5.f*sint[Sprite[0]->Rdegree]);

		}break;
		case 6:{
			//旋回1
			if( (Age == 20) ) MakeShot();
			
			switch( ActFlg[KACTION_S2Z6_ACT1] ){
				case 0:{
					s32 rad = abs(Y-240);    //回転半径
					RSpeed  =  RoundOff(2048.f * (f32)Speed / PI / (f32)rad);   //１フレームあたりの回転角度を求める
					Inc(ActFlg[KACTION_S2Z6_ACT1]);
					DiffDistance  =  0;
					DisappearPos = 192;
				}break;
				case 1:{
					Inc(DiffDistance, FOwner->Speed - KSCROLLSPEED_NORMAL);
					if( (TotalFlight - DiffDistance)>=350 ) {
						Inc(ActFlg[KACTION_S2Z6_ACT1]);
						ActCount[KACTION_S2Z6_ACT1] = -1;
					}
				}break;
				case 2:{
					if( ActCount[KACTION_S2Z6_ACT1]<420 )
						Sprite[0]->RotateR(RSpeed);
					else {
						Inc(ActFlg[KACTION_S2Z6_ACT1]);
					}
				}break;
				case 3:{

				}break;
			}

			Sprite[0]->SlideMoveR(-Speed,0);
		}break;
		case 7:{
			//旋回2
			if( (Age == 20) ) MakeShot();

			switch( ActFlg[KACTION_S2Z6_ACT1] ){
				case 0:{
					s32 rad = abs(Y-240);    //回転半径
					RSpeed  =  RoundOff(2048.f * (f32)Speed / PI / (f32)rad);   //１フレームあたりの回転角度を求める
					Inc(ActFlg[KACTION_S2Z6_ACT1]);
					DiffDistance  =  0;
					DisappearPos = 192;
				}break;
				case 1:{
					Inc(DiffDistance, FOwner->Speed - KSCROLLSPEED_NORMAL);
					if( (TotalFlight - DiffDistance)>=350 ) {
						Inc(ActFlg[KACTION_S2Z6_ACT1]);
						ActCount[KACTION_S2Z6_ACT1] = -1;
					}
				}break;
				case 2:{
					if( ActCount[KACTION_S2Z6_ACT1]<420 )
						Sprite[0]->RotateR(-RSpeed);
					else {
						Inc(ActFlg[KACTION_S2Z6_ACT1]);
					}
				}break;
				case 3:{

				}break;
			}

			Sprite[0]->SlideMoveR(-Speed,0);
			//      Inc(TotalFlight, FOwner->Speed);
		}break;
		case 8:{
			//旋回3
			if( (Age == 20) ) MakeShot();
			
			switch( ActFlg[KACTION_S2Z6_ACT1] ){
				case 0:{
					s32 rad = abs(X-320);    //回転半径
					RSpeed  =  RoundOff(2048.f * (f32)Speed / PI / (f32)rad);   //１フレームあたりの回転角度を求める
					Sprite[0]->Rotate(1024);
					Inc(ActFlg[KACTION_S2Z6_ACT1]);
					ActCount[KACTION_S2Z6_ACT1] = 0;
					DisappearPos = 192;
				}break;
				case 1:{
					if( Y<=240 ) {
						Inc(ActFlg[KACTION_S2Z6_ACT1]);
						ActCount[KACTION_S2Z6_ACT1] = 0;
					}
				}break;
				case 2:{
					if( ActCount[KACTION_S2Z6_ACT1]<900 )
						Sprite[0]->RotateR(-RSpeed);
					else {
						Inc(ActFlg[KACTION_S2Z6_ACT1]);
					}
				}break;
				case 3:{

				}break;
			}

			Sprite[0]->SlideMoveR(-Speed,0);
		}break;
		case 9:{
			//旋回4
			s32 rotatespeed = 32;
			if( (Age == 20) ) MakeShot();
			
			switch( ActFlg[KACTION_S2Z6_ACT1] ){
				case 0:{
					s32 rad = abs(X-320);    //回転半径
					RSpeed  =  RoundOff(2048.f * (f32)Speed / PI / (f32)rad);   //１フレームあたりの回転角度を求める
					Sprite[0]->Rotate(3072);
					Inc(ActFlg[KACTION_S2Z6_ACT1]);
					ActCount[KACTION_S2Z6_ACT1] = 0;
					DisappearPos = 192;
				}break;
				case 1:{
					if( Y>=240 ) {
						Inc(ActFlg[KACTION_S2Z6_ACT1]);
						ActCount[KACTION_S2Z6_ACT1] = 0;
					}
				}break;
				case 2:{
					if( ActCount[KACTION_S2Z6_ACT1]<900 )
						Sprite[0]->RotateR(RSpeed);
					else {
						Inc(ActFlg[KACTION_S2Z6_ACT1]);
					}
				}break;
				case 3:{

				}break;
			}

			Sprite[0]->SlideMoveR(-Speed,0);
			//      Inc(TotalFlight, FOwner->Speed);
		}break;
		case 10:{
			//一旦ブレーキしてから加速
			switch( ActFlg[KACTION_S2Z6_ACT1] ){
				case 0:{
					Sprite[0]->SlideMoveR(-Speed,0);
					if( ActCount[KACTION_S2Z6_ACT1] % 2 == 0 )
						Dec(Speed);

					if( Speed == 0 ) {
						MakeShot();
						Inc(ActFlg[KACTION_S2Z6_ACT1]);
						ActCount[KACTION_S2Z6_ACT1] = 0;
					}
				}break;
				case 1:{
					Sprite[0]->SlideMoveR(-Speed,0);
					if( ActCount[KACTION_S2Z6_ACT1] % 2 == 0 )
						Inc(Speed);
				}break;
			}
		}break;
		case 11:{
			//垂直方向に自機がいたら曲がる
			switch( ActFlg[KACTION_S2Z6_ACT1] ){
				case 0:{
					Sprite[0]->SlideMoveR(-Speed,0);

					if( abs(X-FOwner->HeroPos.X) < 64 ) {
						ActCount[KACTION_S2Z6_ACT1] = 0;

						if( FOwner->HeroPos.Y>=Y )
							ActFlg[KACTION_S2Z6_ACT1] = 1;
						else
							ActFlg[KACTION_S2Z6_ACT1] = 2;
					}
				}break;
				case 1:{
					s32 rotatespeed = -128;

					Sprite[0]->RotateR(rotatespeed);
					if( abs(rotatespeed * ActCount[KACTION_S2Z6_ACT1]) >=1024 ) {
						ActFlg[KACTION_S2Z6_ACT1] = 3;
						ActCount[KACTION_S2Z6_ACT1] = 0;
					}
				}break;
				case 2:{
					s32 rotatespeed = 128;

					Sprite[0]->RotateR(rotatespeed);
					if( abs(rotatespeed * ActCount[KACTION_S2Z6_ACT1]) >=1024 ) {
						ActFlg[KACTION_S2Z6_ACT1] = 3;
						ActCount[KACTION_S2Z6_ACT1] = 0;
					}
				}break;
				case 3:{
					Sprite[0]->SlideMoveR(-Speed*2,0);
				}break;
			}
		}break;
		case 12:{
			//一旦ブレーキしてから加速(追尾)
			switch( ActFlg[KACTION_S2Z6_ACT1] ){
				case 0:{
					Sprite[0]->MoveR(-Speed,0);
					if( ActCount[KACTION_S2Z6_ACT1] % 2 == 0 )
						Dec(Speed);

					Sprite[0]->Rotate(GetRfromArcTan(X,Y,FOwner->HeroPos.X,FOwner->HeroPos.Y) + 2048);

					if( Speed == 0 ) {
						if( FOwner->Difficulty!=KDIFFICULTY_EASY ) MakeShot();
						Inc(ActFlg[KACTION_S2Z6_ACT1]);
						ActCount[KACTION_S2Z6_ACT1] = 0;
					}
				}break;
				case 1:{
					Sprite[0]->SlideMoveR(-Speed,0);
					if( ActCount[KACTION_S2Z6_ACT1] % 2 == 0 )
						Inc(Speed);
				}break;
			}
		}break;
		case 13:{
			//垂直方向に自機がいたら曲がる(gyaku)
			switch( ActFlg[KACTION_S2Z6_ACT1] ){
				case 0:{
					Sprite[0]->SlideMoveR(Speed,0);

					if( abs(FOwner->HeroPos.X-X) < 64 ) {
						ActCount[KACTION_S2Z6_ACT1] = 0;

						if( FOwner->HeroPos.Y>=Y )
							ActFlg[KACTION_S2Z6_ACT1] = 1;
						else
							ActFlg[KACTION_S2Z6_ACT1] = 2;
					}
				}break;
				case 1:{
					s32 rotatespeed = 64;

					Sprite[0]->RotateR(rotatespeed);
					if( abs(rotatespeed * ActCount[KACTION_S2Z6_ACT1]) >=1024 ) {
						ActFlg[KACTION_S2Z6_ACT1] = 3;
						ActCount[KACTION_S2Z6_ACT1] = 0;
					}
				}break;
				case 2:{
					s32 rotatespeed = -64;

					Sprite[0]->RotateR(rotatespeed);
					if( abs(rotatespeed * ActCount[KACTION_S2Z6_ACT1]) >=1024 ) {
						ActFlg[KACTION_S2Z6_ACT1] = 3;
						ActCount[KACTION_S2Z6_ACT1] = 0;
					}
				}break;
				case 3:{
					Sprite[0]->SlideMoveR(Speed,0);
				}break;
			}
		}break;
		case 14:{
			//Uターン(下から)
			switch( ActFlg[KACTION_S2Z6_ACT1] ){
				case 0:{
					s32 rad = abs(Y-240);    //回転半径
					RSpeed  =  RoundOff(2048.f * (f32)Speed / PI / (f32)rad);   //１フレームあたりの回転角度を求める
					Inc(ActFlg[KACTION_S2Z6_ACT1]);
					DiffDistance  =  0;
					DisappearPos = 256;
				}break;
				case 1:{
					Sprite[0]->MoveR(-Speed,0);
					Inc(DiffDistance, FOwner->Speed - KSCROLLSPEED_NORMAL);
					if( (TotalFlight - DiffDistance)>=300 ) {
						Inc(ActFlg[KACTION_S2Z6_ACT1]);
						ActCount[KACTION_S2Z6_ACT1] = -1;
						//            putdebuglog(IntToStr( Age));
					}
				}break;
				case 2:{
					Sprite[0]->Rotate(Rdeg);
					Sprite[0]->SlideMoveR(-Speed,0);
					Inc(Rdeg, RSpeed);

					if( Rdeg>=2048 ) {
						MakeShot();
						Sprite[0]->Rotate(2048);
						Inc(ActFlg[KACTION_S2Z6_ACT1]);
						ActCount[KACTION_S2Z6_ACT1] = -1;
					}
				}break;
				case 3:{
					Sprite[0]->SlideMoveR(-Speed,0);
				}
			}
		}break;
		case 15:{
			//Uターン(上から)
			switch( ActFlg[KACTION_S2Z6_ACT1] ){
				case 0:{
					s32 rad = abs(Y-240);    //回転半径
					RSpeed  =  RoundOff(2048.f * (f32)Speed / PI / (f32)rad);   //１フレームあたりの回転角度を求める
					Inc(ActFlg[KACTION_S2Z6_ACT1]);
					DiffDistance  =  0;
					DisappearPos = 256;
				}break;
				case 1:{
					Sprite[0]->MoveR(-Speed,0);
					Inc(DiffDistance, FOwner->Speed - KSCROLLSPEED_NORMAL);
					if( (TotalFlight - DiffDistance)>=300 ) {
						Inc(ActFlg[KACTION_S2Z6_ACT1]);
						ActCount[KACTION_S2Z6_ACT1] = -1;
						//            putdebuglog(IntToStr( Age));
					}
				}break;
				case 2:{
					Sprite[0]->Rotate(Rdeg);
					Sprite[0]->SlideMoveR(-Speed,0);
					Dec(Rdeg, RSpeed);

					if( Rdeg<=-2048 ) {
						MakeShot();
						Sprite[0]->Rotate(2048);
						Inc(ActFlg[KACTION_S2Z6_ACT1]);
						ActCount[KACTION_S2Z6_ACT1] = -1;
					}
				}break;
				case 3:{
					Sprite[0]->SlideMoveR(-Speed,0);
				}
			}
		}break;
		case 16:{
			//垂直方向に自機がいたら曲がる(スロー)
			switch( ActFlg[KACTION_S2Z6_ACT1] ){
				case 0:{
					Sprite[0]->SlideMoveR(-Speed,0);
					if( (TotalFlight >=200) && (ShotCount == 0) ) MakeShot();
					if( cost[Rdeg]>0 ) {
						if( (X-FOwner->HeroPos.X) <= 0 ) {
							ActCount[KACTION_S2Z6_ACT1] = 0;

							if( FOwner->HeroPos.Y>=Y )
								ActFlg[KACTION_S2Z6_ACT1] = 1;
							else
								ActFlg[KACTION_S2Z6_ACT1] = 2;
						}
					}
					else {
						if( (FOwner->HeroPos.X-X) <= 0 ) {
							ActCount[KACTION_S2Z6_ACT1] = 0;

							if( FOwner->HeroPos.Y>=Y )
								ActFlg[KACTION_S2Z6_ACT1] = 1;
							else
								ActFlg[KACTION_S2Z6_ACT1] = 2;
						}
					}
				}break;
				case 1:{
					s32 rotatespeed = -48;

					Sprite[0]->RotateR(rotatespeed);
					if( abs(rotatespeed * ActCount[KACTION_S2Z6_ACT1]) >=1024 ) {
						ActFlg[KACTION_S2Z6_ACT1] = 3;
						ActCount[KACTION_S2Z6_ACT1] = 0;
					}
				}break;
				case 2:{
					s32 rotatespeed = 48;

					Sprite[0]->RotateR(rotatespeed);
					if( abs(rotatespeed * ActCount[KACTION_S2Z6_ACT1]) >=1024 ) {
						ActFlg[KACTION_S2Z6_ACT1] = 3;
						ActCount[KACTION_S2Z6_ACT1] = 0;
					}
				}break;
				case 3:{
					Sprite[0]->SlideMoveR(-Speed,0);
				}
			}
		}break;
		case 17:{
			//一旦ブレーキしてから加速(slow)
			switch( ActFlg[KACTION_S2Z6_ACT1] ){
				case 0:{
					Sprite[0]->SlideMoveR(-Speed,0);
					if( ActCount[KACTION_S2Z6_ACT1] % 2 == 0 )
						Dec(Speed);

					if( Speed == 0 ) {
						MakeShot();
						Inc(ActFlg[KACTION_S2Z6_ACT1]);
						ActCount[KACTION_S2Z6_ACT1] = 0;
					}
				}break;
				case 1:{
					Sprite[0]->SlideMoveR(-Speed,0);
					if( ActCount[KACTION_S2Z6_ACT1] % 4 == 0 )
						Inc(Speed);
				}
			}
		}break;
	}

	Inc(ActCount[KACTION_S2Z6_ACT1]);

}

TZako2_06::TZako2_06( TOBJList* owner, s32 _x, s32 _y, s32 _r, s32 _s, s32 _pat, bool _red ) : TOBJ(owner)
{
	X = _x;
	Y = _y;
	MaxHP = KSTR_SHOT;
	HP = MaxHP;
	TempHP = HP;

	if( FOwner->IsSequence() )
		FSTR = 250;
	else
		FSTR = 500;

	FKind = kEffect;   //画面外にいるうちはエフェクト扱い（ロックオンされない）
	Score = 300;
	Buzzed = false;
	LastBuzz = 0;
	Pattern = _pat;
	CommandRed = _red;
	TotalFlight = 0;
	LastFlash = 0;
	OriginColor  =  255;
	ComBonus = 5000;
	Rdeg = _r;
	Speed = _s;
	RSpeed = 0;
	DiffDistance = 0;
	DisappearPos = 64;
	//IDの付与
	FID = NewID;
	Inc(NewID);
	ActFlg[KACTION_S2Z6_ACT1] = 0;
	ActCount[KACTION_S2Z6_ACT1] = 0;
	ShotCount = 0;

	s32 texX,texWidth;
	if( (0 <= _pat && _pat <= 12) || (14 <= _pat && _pat <=17 ) )
	{
		texX = 177;
		texWidth = 96;
	}
	else
	{
		texX = 272;
		texWidth = -96;
	}

	MakeSprite(QD, MechaTex,  0,texX,1,texWidth, 48,110,  0,  0,  96, 48, 56, 30,CommandRed);
	Sprite[0]->GetSpr()->Move(_x,_y);
	Sprite[0]->GetSpr()->FHit = false;

	//コマンダーの色がわかりにくいので下地に一枚敷いて輪郭を強調
	if( CommandRed ) {
		MakeSpriteLite(numSpr, X,Y,96,48,11500,bmAdd);
		Sprite[numSpr-1]->GetSpr()->AddTexture(MechaTex, texX, 1, texX + texWidth, 48);
		Sprite[numSpr-1]->GetSpr()->FCX += 56;
		Sprite[numSpr-1]->GetSpr()->FCY += 30;
		Sprite[numSpr-1]->GetSpr()->Move(_x,_y);
		Sprite[numSpr-1]->GetSpr()->Zoom(1.2f, 1.2f);
		Sprite[numSpr-1]->GetSpr()->SetColor(0,ctBlue);
		Sprite[numSpr-1]->GetSpr()->SetColor(0,ctGreen);
		Sprite[0]->Add(Sprite[numSpr-1]);
	}

	Sprite[0]->Rotate(Rdeg);
}

void TZako2_06::Move()
{
	TOBJ::Move();
	X = (s32)(Sprite[0]->GetSpr()->FCX);
	Y = (s32)(Sprite[0]->GetSpr()->FCY);

	Action();

	if( (X > 640+64) || (Y > 480+64) || (Y <0-64) || (X < 0-DisappearPos) ) {
		if( FKind == kEnemy ) {
			Die();//画面外に消えたら消す & 母艦にダメージ
			Dec(FOwner->MShipHP,FSTR / 2);
			FOwner->Add(new TMotherShipDamaged(FOwner, X, Y));
		}
	}
	else {
		if( FKind == kEffect ) { //画面にフレームインすると同時に当たり判定有効化＆ロックオン対象
			FKind = kEnemy;
			Sprite[0]->GetSpr()->FHit = true;
		}
		Inc(TotalFlight, FOwner->Speed +Speed- KSCROLLSPEED_NORMAL);
	}

	if( (Age-LastFlash == 2) ) {
		if( CommandRed || (! ExtraEffects) ) {
			Sprite[0]->GetSpr()->SetColor(255,ctRed);          //色を戻す
		}
		else {
			Sprite[0]->GetSpr()->SetColor(OriginColor,ctRed);          //色を戻す
		}
		Sprite[0]->MoveR(-3,0);                  //のけぞり戻し
	}

	if( (Age-LastBuzz)>=40 ) Buzzed = false;
	if( HP<=0 ) OnBroken();                        //HPが無くなったら死ぬ

	if( FOwner->BossDie ) {
		FOwner->Add(new TExpSmall(FOwner,X,Y,0,0,0));//爆発
		Die();
	}

}

void TZako2_06::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);
	X = RoundOff(Sprite[0]->GetSpr()->FCX);
	Y = RoundOff(Sprite[0]->GetSpr()->FCY);
	if( target->Kind() == kHero ) {
		if( (! Buzzed) && (target->Hit[HitBuzz]) ) {
			FOwner->Scrach(this);
			Buzzed = true;
			LastBuzz = Age;
		}
		if( (target->Hit[HitBlade]) && (target->LastSlashed > LastSlashed) ) {
			Dec(HP,target->STR());
			Sprite[0]->GetSpr()->SetColor(0,ctRed);
			LastFlash = Age;
			Sprite[0]->MoveR(3,0);                  //のけぞる
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WAEdamage0,7);
			LastSlashed = target->LastSlashed;
		}
	}

	if( (target->Kind() == kHShot) || (target->Kind() == kMissile) ) {
		Dec(HP,target->STR());
		Sprite[0]->GetSpr()->SetColor(0,ctRed);
		LastFlash = Age;
		Sprite[0]->MoveR(3,0);                  //のけぞる
		if( CreateSE ) FOwner->SoundEffect->WavPlay(WAEdamage0,7);
		if( HP<=0 ) BonusRate = target->BonusRate;
	}

	if( target->Kind() == kColBG ) {
		FOwner->Add(new TExpSmall(FOwner,X,Y,0,0,0));//爆発
		Die();
	}

	if( HP<TempHP ) TempHP = HP;

}

void TZako2_06::OnBroken()
{
	TOBJ::OnBroken();
	FOwner->Add(new TExpSmall(FOwner,X,Y,0,0,0));//爆発

	if( (FOwner->Difficulty == KDIFFICULTY_HARD) && (FOwner->NowStage>=3) ) MakeShot();   //打ち返し弾

}

TRevBarBasic::TRevBarBasic( TOBJList* owner, TOBJ* _parent, s32 _x, s32 _y, s32 _R, f32 _S, f32 _parentspeed, s32 _L ) : TOBJ(owner)
{
	X = _x;
	Y = _y;
	Speed = _S;
	ParentSpeed = _parentspeed;
	Rdeg = _R;
	Parent = _parent;
	Life = _L;
	FKind = kEffect;
	PrevR = Rdeg;

	MakeSpriteLite(0, X,Y,32,32,16000,bmAdd);
	if( FOwner->NowStage == 1 )
		Sprite[0]->GetSpr()->AddTexture(MechaTex, 369, 113, 400, 144);
	else
		Sprite[0]->GetSpr()->AddTexture(MechaTex, 289, 81, 320, 112);
	Sprite[0]->GetSpr()->FCX += 8;
	Sprite[0]->GetSpr()->FCY += 16;
	Sprite[0]->GetSpr()->Move(_x,_y);
	Sprite[0]->GetSpr()->SetColor(128,ctAlpha);
	Sprite[0]->GetSpr()->Zoom((f32)random(3)+3.f, 1.f+(f32)random(10)*0.02f);
	Sprite[0]->GetSpr()->ZoomR(Speed*0.1f,1);
	Sprite[0]->GetSpr()->Rotate(Parent->Sprite[0]->Rdegree + Rdeg);
}

void TRevBarBasic::Move()
{
	TOBJ::Move();
	if( Parent->Sprite[0] ) {
		if( ! Parent->Dead() ) {
			s32 newR  = (Parent->Sprite[0]->Rdegree + Rdeg) && 0x0fff;
			f32 newX = Sprite[0]->GetSpr()->FCX;
			f32 newY = Sprite[0]->GetSpr()->FCY;
			PosRoll(newX,newY,Parent->X,Parent->Y, PrevR - newR);
			Sprite[0]->GetSpr()->SlideMoveR(Speed,0.f);
			Sprite[0]->GetSpr()->Rotate(0);
			Sprite[0]->GetSpr()->ZoomR(1.05f, 0.98f);
			Sprite[0]->GetSpr()->Rotate(newR);
			Sprite[0]->GetSpr()->Move(newX, newY);
			PrevR = newR;

			Sprite[0]->MoveR(ParentSpeed,0);
		}
		else Die();
	}
	else Die();

	if( Age>=Life )
		Sprite[0]->GetSpr()->SetColorR(-16,ctAlpha);

	if( Sprite[0]->GetSpr()->GetColor(ctAlpha) == 0 ) Die();

}

TZ02_1Gene::TZ02_1Gene( TOBJList* owner, s32 _x, s32 _y, s32 _pat ) : TOBJ(owner)
{
	FObjectName = "TZ02_1Gene";
	X = _x;
	Y = _y;
	FKind = kGene;
	Pattern = _pat;
	Count = 0;
	TotalFlight = 0;
	OriginX = _x;

}

void TZ02_1Gene::Move()
{
	TOBJ::Move(); 
	s32 Speed = 0;
	switch( Pattern ){
		//前方から3機直進
	case 0:{
		Speed = 2;
		if( (TotalFlight>=80*Count) ) {
			switch( Count ){
			case 0:{
				FOwner->Add(new TZako2_01(FOwner,X,Y-70,0,0,0,-Speed,0,false));
				FOwner->Add(new TZako2_01(FOwner,X,Y+70,0,0,0,-Speed,0,false));
				   }break;
			case 1:{
				FOwner->Add(new TZako2_01(FOwner,X,Y,0,0,0,-Speed,0,true));
				Die();
				   }break;
			}
			Inc(Count);
		}
		   }break;
		//前方から3機直進2
	case 1:{
		Speed = 2;
		if( (TotalFlight>=80*Count) ) {
			switch( Count ){
			case 0:{
				FOwner->Add(new TZako2_01(FOwner,X,Y,0,0,0,-Speed,0,false));
				   }break;
			case 1:{
				FOwner->Add(new TZako2_01(FOwner,X,Y+70,0,0,0,-Speed,0,false));
				   }break;
			case 2:{
				FOwner->Add(new TZako2_01(FOwner,X,Y+140,0,0,0,-Speed,0,true));
				Die();
				   }break;
			}
			Inc(Count);
		}
		   }break;   
		//前方からゆるい追尾
	case 2:{
		Speed = 6;
		FOwner->Add(new TZako2_01(FOwner,X,Y,0,0,0,-Speed,1,false));
		Die();
		   } break;
		//上から4機
	case 3:{
		if( Age % 10 == 0 ) {
			X = OriginX-Count*120;
			switch( Count ){
			case 0:
			case 2:{
				FOwner->Add(new TZako2_01(FOwner,X,-60,X,Y,0,-Speed,2,false));
				   }break;
			case 1:{
				FOwner->Add(new TZako2_01(FOwner,X,-60,X,Y+150,0,-Speed,2,false));
				   }break;
			case 3:{
				FOwner->Add(new TZako2_01(FOwner,X,-60,X,Y+150,0,-Speed,2,true));
				Die();
				   }break;
			}
			Inc(Count);
		}
		   }break;   
		//上から4機 ミサイル
	case 4:{
		if( Age % 10 == 0 ) {
			switch( Count ){
			case 0:
			case 1:
			case 2:{
				FOwner->Add(new TZako2_01(FOwner,X,-60,X,Y,0,-Speed,3,false));
				   }break;
			case 3:{
				FOwner->Add(new TZako2_01(FOwner,X,-60,X,Y,0,-Speed,3,true));
				Die();
				   }break;
			}
			Inc(Count);
			Y = Y-100;
		}
		   }break;
		//上から3機
	case 5:{
		FOwner->Add(new TZako2_01(FOwner,X-60,-60,X-80,Y-70 ,0,-Speed,2,true));
		FOwner->Add(new TZako2_01(FOwner,X-60,-60,X   ,Y-140,0,-Speed,2,false));
		FOwner->Add(new TZako2_01(FOwner,X-60,-60,X-80,Y-210,0,-Speed,2,false));

		Die();
		   }break;
		//下から3機
	case 6:{
		FOwner->Add(new TZako2_01(FOwner,X-60,540,X-80,Y+70 ,0,-Speed,2,true));
		FOwner->Add(new TZako2_01(FOwner,X-60,540,X   ,Y+140,0,-Speed,2,false));
		FOwner->Add(new TZako2_01(FOwner,X-60,540,X-80,Y+210,0,-Speed,2,false));

		Die();
		   }break;
		//前方から3機直進、ランダム赤
	case 7:{
		Speed = 2;
		FOwner->Add(new TZako2_01(FOwner,X   ,Y-70,0,0,0,-Speed,0,false));
		FOwner->Add(new TZako2_01(FOwner,X+40,Y   ,0,0,0,-Speed,0,false));
		FOwner->Add(new TZako2_01(FOwner,X   ,Y+70,0,0,0,-Speed,0,true));

		Die();

		   }break;
	}
	Inc(TotalFlight,FOwner->Speed+(Speed-KSCROLLSPEED_NORMAL)); //機体のスピード分足す

}

TZ02_5Gene::TZ02_5Gene( TOBJList* owner, s32 _x, s32 _y, s32 _pat, s32 _com ) : TOBJ(owner)
{
	FObjectName = "TZ02_5Gene";
	X = _x;
	Y = _y;
	FKind = kGene;
	Pattern = _pat;
	Count = 0;
	TotalFlight = 0;
	CommanderNum = _com;
	diffX = 0;

}

void TZ02_5Gene::Move()
{
	TOBJ::Move();

	bool Red = false;
	switch( Pattern ){
		//前方から2機
	case 0:{
		if( (TotalFlight>=80*Count) ) {
			if( Count == CommanderNum ) Red = true;

			switch( Count ){
			case 0:{
				FOwner->Add(new TZako2_05(FOwner,X,Y,X-300,Y,0,Red));
				   }break;
			case 1:{
				FOwner->Add(new TZako2_05(FOwner,X,Y+80,X-300-diffX,Y+80,0,Red));
				Die();
				   }break;
			}
			Inc(Count);
		}
		   } break;
		//前方から4機
	case 1:{
		if( (TotalFlight>=80*Count) ) {
			if( Count == CommanderNum ) Red = true;

			switch( Count ){
			case 0:
			case 1:
			case 2:
			case 3:{
				FOwner->Add(new TZako2_05(FOwner,X,Y,X-300-diffX,Y,0,Red));
				   }break;
			case 4:
				Die();
				break;
			}
			Inc(Count);
			Y = Y+100;
		}
		   }break;
		//前方から2機 間隔あり
	case 2:{
		if( (TotalFlight>=80*Count) ) {
			if( Count == CommanderNum ) Red = true;

			switch( Count ){
			case 0:{
				FOwner->Add(new TZako2_05(FOwner,X,Y,X-300,Y,0,Red));
				   }break;
			case 1:{
				FOwner->Add(new TZako2_05(FOwner,X,Y+160,X-300-diffX,Y+160,0,Red));
				Die();
				   }break;
			}
			Inc(Count);
		}
		   }break;
		//前方から1機
	case 3:{
		if( Count == CommanderNum ) Red = true;
		FOwner->Add(new TZako2_05(FOwner,X,Y,X-300,Y,0,Red));
		Die();
		   }break;
	}
	Inc(TotalFlight,FOwner->Speed+(KSCROLLSPEED_NORMAL)); //機体のスピード分足す
	Inc(diffX,FOwner->Speed-KSCROLLSPEED_NORMAL);

}

TZ02_6Gene::TZ02_6Gene( TOBJList* owner, s32 _x, s32 _y, s32 _pat ) : TOBJ(owner)
{
	FObjectName = "TZ02_6Gene";
	X = _x;
	Y = _y;
	FKind = kGene;
	Pattern = _pat;
	Count = 0;
	TotalFlight = 0;
	diffX = 0;

	if( Pattern == 3 ) {
		Pattern  =  Pattern+FOwner->GetRand(1);
	}

}

void TZ02_6Gene::Move()
{
	TOBJ::Move();
	switch( Pattern ){
		//前方から直進
	case 0:{
		for( u32 i = 0; i <= 5; i++ ) {
			FOwner->Add(new TZako2_06(FOwner,X+i*80,Y,0,5,0,false));
		}
		FOwner->Add(new TZako2_06(FOwner,X+6*80,Y,0,5,0,true));
		Die();
		   } break;
		//前方から追尾
	case 1:{
		for( u32 i = 0; i <= 4; i++ ) {
			FOwner->Add(new TZako2_06(FOwner,X+i*80,Y,0,5,1,false));
		}
		FOwner->Add(new TZako2_06(FOwner,X+5*80,Y,0,5,1,true));
		Die();
		   } break;
		//蛇行
	case 2:{
		for( u32 i = 0; i <= 5; i++ ) {
			if( i % 2 == 0 ) {
				FOwner->Add(new TZako2_06(FOwner,X+i*80,Y,3072,0,4,true));
				FOwner->Add(new TZako2_06(FOwner,X+i*80,Y,1024,0,5,false));              
			}
			else {
				FOwner->Add(new TZako2_06(FOwner,X+i*80,Y+105,0,0,4,false));
				FOwner->Add(new TZako2_06(FOwner,X+i*80,Y-105,0,0,5,false));
			}
		}
		Die();
		   }break;
		//前方から直進 with bar
	case 3:{
		Y = 368;

		if( (TotalFlight == 0) ) {
			FOwner->Add(new TZako2_03(FOwner,X+320,16,0,9,2,false));
			FOwner->Add(new TZako2_03(FOwner,X+960,16,0,9,2,false));
			FOwner->Add(new TZako2_03(FOwner,X+1600,16,0,9,2,false));
		}

		if( (TotalFlight>=120*Count) ) {
			if( 0 <= Count && Count <= 13){
				FOwner->Add(new TZako2_06(FOwner,X,Y,0,8,0,false));
			}
			else if( 14 == Count ){
				FOwner->Add(new TZako2_06(FOwner,X,Y,0,8,0,true));
				Die();
			}
			Inc(Count);
		}
		   }break;
	case 4:{
		Y = 120;

		if( (TotalFlight == 0) ) {
			FOwner->Add(new TZako2_03(FOwner,X+320,464,0,9,3,false));
			FOwner->Add(new TZako2_03(FOwner,X+960,464,0,9,3,false));
			FOwner->Add(new TZako2_03(FOwner,X+1600,464,0,9,3,false));
		}

		if( (TotalFlight>=120*Count) ) {
			if( 0 <= Count && Count <= 13){
				FOwner->Add(new TZako2_06(FOwner,X,Y,0,8,0,false));
			}
			else if( 14 == Count ){
				FOwner->Add(new TZako2_06(FOwner,X,Y,0,8,0,true));
				Die();
			}
			Inc(Count);
		}
		   }break;
		//画面の外側から中心に向かって
	case 5:{

		if( (TotalFlight>=150*Count) ) {
			s32 tmpR = Count*512;
			X = 320 + (s32)( 500*cost[tmpR & 0xfff]);
			Y = 240 + (s32)( 500*sint[tmpR & 0xfff]);
			switch( Count ){
			case 15:{
				FOwner->Add(new TZako2_06(FOwner,X,Y,tmpR,17,17,true));
				Die();
					}break;
			default:
				{
					FOwner->Add(new TZako2_06(FOwner,X,Y,tmpR,17,17,false));
				}
			}
			Inc(Count);
		}
		   }break;
		//画面の外側から中心に向かって (同時)
	case 6:{
		for( u32 i = 0; i <= 7; i++ ) {
			s32 tmpR = i*512;
			X = 320 + (s32)( 500*cost[tmpR & 0xfff]);
			Y = 240 + (s32)( 500*sint[tmpR & 0xfff]);
			FOwner->Add(new TZako2_06(FOwner,X,Y,tmpR,17,17,false));
		}
		Die();
		   }break;
	}
	Inc(diffX,FOwner->Speed-KSCROLLSPEED_NORMAL);
	Inc(TotalFlight,FOwner->Speed+(KSCROLLSPEED_NORMAL)); //機体のスピード分足す

}

void TZako3_01::MakeShot( s32 _x, s32 _y, s32 _r )
{
	FOwner->Add(new TFCircle(FOwner,_x,_y,1,_r,Speed));
	if( CreateSE ) FOwner->SoundEffect->WavPlay(WABShot,9);

	if( FOwner->Difficulty == KDIFFICULTY_HARD ) {
		for( u32 i = 0; i <= 4; i++ )
			FOwner->Add(new TZ01Shot4(FOwner,_x,_y,_r,2+i,BSpeed,0,500));
	}
	else
		FOwner->Add(new TBoss3Shot(FOwner,_x,_y,_r,10,500, BSpeed, 1,1,1));

}

void TZako3_01::MakeHoming( s32 _x, s32 _y, s32 _r )
{
	if( CreateSE ) FOwner->SoundEffect->WavPlay(WAHoming,10);
	FOwner->Add(new TMissileFlash(FOwner,_x,_y));
	FOwner->Add(new TZakoHomingCore2(FOwner,_x,_y, _r, 3));

}

void TZako3_01::Action()
{
	switch( Pattern ){
	case 0:{
		switch( ActFlg[KACTION_ACT1] ){
			//初期化
		case 1:{
			Sprite[0]->SetDest(DestX,DestY,48);

			Inc(ActFlg[KACTION_ACT1]);
			ActCount[KACTION_ACT1]  =  -1;
			   }break;
			//
		case 2:{
			Sprite[0]->AutoSlide();
			if( ActCount[KACTION_ACT1] > 60 ) {
				Inc(ActFlg[KACTION_ACT1]);
				ActCount[KACTION_ACT1]  =  -1;
			}
			   }break;
		case 3:{
			Sprite[0]->AutoSlide();
			switch( ActCount[KACTION_ACT1] ){
			case 5:{
				MakeShot(X, Y-32, -512);
				MakeShot(X, Y+32,  512);
				   }break;
			case 15:{
				MakeShot(X-16, Y-32, -1024);
				MakeShot(X-16, Y+32,  1024);
					}break;
			case 25:{
				MakeShot(X-32, Y-32, -1536);
				MakeShot(X-32, Y+32,  1536);
					}break;
			case 35:{
				MakeShot(X-48, Y-32, 2048);
				MakeShot(X-48, Y+32,  2048);
					}break;
			case 120:{
				Inc(ActFlg[KACTION_ACT1]);
				ActCount[KACTION_ACT1]  =  -1;
					 }break;
			}
			   }break;
		case 4:{
			switch( ActCount[KACTION_ACT1] ){
			case 0:{
				MakeHoming(X+32, Y-32, 0);
				MakeHoming(X+32, Y+32, 0);
				   }break;
			case 60:{
				if( (FOwner->Difficulty == KDIFFICULTY_SINGLE) || (FOwner->Difficulty == KDIFFICULTY_HARD) ) {
					MakeHoming(X+56, Y-32, -512);
					MakeHoming(X+56, Y+32, 512);
				}
					}break;
			case 120:{
				if( (FOwner->Difficulty != KDIFFICULTY_EASY) ) {
					MakeHoming(X+80, Y-32, -1024);
					MakeHoming(X+80, Y+32, 1024);
				}
					 }break;
			case 180:{
				Inc(RoopCount);
				if( RoopCount<2 ) {
					ActFlg[KACTION_ACT1] = 3;
					ActCount[KACTION_ACT1]  =  -1;
				}
				else {
					Inc(ActFlg[KACTION_ACT1]);
					ActCount[KACTION_ACT1]  =  -1;
				}
					 }break;
			}
			   }break;
		case 5:{
			Sprite[0]->MoveR(-0.1f*(f32)ActCount[KACTION_ACT1],0.f);
			if( (X < 0-144) && (! FOwner->IsSequence())) {
				Die();//画面外に消えたら消す & 母艦にダメージ
				Dec(FOwner->MShipHP,FSTR*10);
				FOwner->Add(new TMotherShipDamaged(FOwner, X, Y));
			}
			   }
		}

		//通し面バージョンではすべてのステートで画面外判定
		if( (X < 0-144) && (FOwner->IsSequence())) {
			Die();
			Dec(FOwner->MShipHP,FSTR);
			FOwner->Add(new TMotherShipDamaged(FOwner, X, Y));
		}
		   }break;
	case 1:{
		switch( ActFlg[KACTION_ACT1] ){
			//初期化
		case 1:{
			Sprite[0]->SetDest(DestX,DestY,32);

			Inc(ActFlg[KACTION_ACT1]);
			ActCount[KACTION_ACT1]  =  -1;
			   }break;
			//
		case 2:{
			Sprite[0]->AutoSlide();
			s32 DestR = GetRfromArcTan(X,Y,FOwner->HeroPos.X,FOwner->HeroPos.Y);
			Sprite[0]->Rotate(DestR+1024 +2048);

			if( ActCount[KACTION_ACT1] > 180 ) {
				Inc(ActFlg[KACTION_ACT1]);
				ActCount[KACTION_ACT1]  =  -1;
			}
			   }break;
		case 3:{
			Sprite[0]->SlideMoveR(0.f, (f32)ActCount[KACTION_ACT1]*0.1f);
			if( (X < 0-144) || (X > 640+144) || (Y < 0-144) || (Y >480+144) ) {
				Die();//画面外に消えたら消す & 母艦にダメージ
				Dec(FOwner->MShipHP,FSTR);
				FOwner->Add(new TMotherShipDamaged(FOwner, X, Y));
			}
			   }break;
		}
		   }
	}
	Inc(ActCount[KACTION_ACT1]);
}

void TZako3_01::Accel()
{
	if( (FOwner->Speed>=10) )
		BSpeed = BSpeed+0.4f;
	else if( BSpeed>0 )
		BSpeed = BSpeed-0.4f;

	if( BSpeed>5 )
		BSpeed = 5;

	if( BSpeed<0 )
		BSpeed = 0;

	Sprite[0]->MoveR(BSpeed,0);

}

void TZako3_01::KnockBack( s32 _impact )
{
	for( u32 i  =  0 ; i < numSpr; i++)
		Sprite[i]->GetSpr()->FHit  =  false;

	if( FOwner->HeroPos.Y > Y )
		KnockBackSpeed  =  _impact;
	else
		KnockBackSpeed  =  -1*_impact;

	ActCount[KACTION_KNOCKBACK] = 0;

}

void TZako3_01::KnockBackAction()
{
	if( ActCount[KACTION_KNOCKBACK] == 4 ) FOwner->Add(new TExpSlashed(FOwner,X+80,Y-32,0,64,0));

	if( ActCount[KACTION_KNOCKBACK] > 2 ) {
		Sprite[0]->RotateR(KnockBackSpeed);
		Sprite[0]->MoveR((f32)abs(KnockBackSpeed)/7.f,0.f);
		KnockBackSpeed  =  (s32)((f32)KnockBackSpeed*0.6f);
	}
	Inc(ActCount[KACTION_KNOCKBACK]);

}

TZako3_01::TZako3_01( TOBJList* owner, s32 _x, s32 _y, s32 dx, s32 dy, s32 _pat, bool _red ) : TOBJ(owner)
{
	FObjectName = "TZako3_01";

	X = _x;
	Y = _y;
	DestX = dx;
	DestY = dy;
	MaxHP = KSTR_SHOT*48;
	HP = MaxHP;
	TempHP = HP;
	switch( FOwner->Difficulty ){
	case KDIFFICULTY_EASY:
		FSTR = 500;
		break;
	case KDIFFICULTY_NORMAL:
	case KDIFFICULTY_HARD:
		FSTR = 1000;
		break;
	default:
		FSTR = 100;
	}
	FKind = kEnemy;
	Score = 50000;
	Buzzed = false;
	LastBuzz = 0;
	Pattern = _pat;
	CommandRed = _red;
	GaugeTime = 0;
	TotalFlight = 0;
	LastFlash = 0;
	BSpeed = 0;
	KnockBackSpeed = 0;
	ComBonus = 5000;
	OriginColor  =  255;
	RoopCount = 0;

	//IDの付与
	FID = NewID;
	Inc(NewID);

	for( u32 i = 0; i <= 99; i++ ) {
		ActFlg[i] = 0;
		ActCount[i] = 0;
	}

	MakeSpriteLite(numSpr, X,Y,160,384,16500,bmNormal);
	Sprite[numSpr-1]->GetSpr()->AddTexture(MechaTex, 352, 0, 512, 384);
	Sprite[numSpr-1]->GetSpr()->AddHitArea(TPoint(32, 48), TPoint(32, 352), TPoint(128, 352),TPoint(128, 48), htSky);
	Sprite[numSpr-1]->GetSpr()->FHit      =  true;
	Sprite[numSpr-1]->GetSpr()->FCX += 80;
	Sprite[numSpr-1]->GetSpr()->FCY += 128;
	if( CommandRed ) {
		Sprite[numSpr-1]->GetSpr()->SetColor(255, ctRed);
		Sprite[numSpr-1]->GetSpr()->SetColor(64, ctBlue);
		Sprite[numSpr-1]->GetSpr()->SetColor(64, ctGreen);
	}

	if( ExtraEffects ) {
		MakeSpriteLite(numSpr, X,Y,160,384,16000,bmAdd);
		Sprite[numSpr-1]->GetSpr()->AddTexture(MechaTex, 352, 0, 512, 384);
		Sprite[numSpr-1]->GetSpr()->FCX += 80;
		Sprite[numSpr-1]->GetSpr()->FCY += 128;
		Sprite[numSpr-1]->GetSpr()->SetColor(160,ctAlpha);
		Sprite[numSpr-1]->GetSpr()->FEffectNum = 1;
		Sprite[numSpr-1]->GetSpr()->FBumpSize = 2.9f;
		Sprite[numSpr-1]->GetSpr()->FBumpTex = MechaTex;
		Sprite[numSpr-1]->GetSpr()->FBumpLevel = 2.0f;
		if( CommandRed ) {
			Sprite[numSpr-1]->GetSpr()->SetColor(255, ctRed);
			Sprite[numSpr-1]->GetSpr()->SetColor(64, ctBlue);
			Sprite[numSpr-1]->GetSpr()->SetColor(64, ctGreen);
		}

		Sprite[0]->Add( Sprite[numSpr-1] );
	}

	Sprite[0]->Move(X,Y);
	Sprite[0]->Rotate(1024);

	HPGauge  =  new TEnemyGauge2(FOwner,this,-128,32);
	FOwner->Add(HPGauge);

	ActFlg[KACTION_ACT1] = 1;

}

void TZako3_01::Move()
{
	X = (s32)(Sprite[0]->GetSpr()->FCX);
	Y = (s32)(Sprite[0]->GetSpr()->FCY);

	if( HP>0 ) {
		Action();
	}
	else
		KnockBackAction();

	if( (HP<=0) && (KnockBackSpeed == 0) ) OnBroken();


	if( (Age-LastFlash == 2) ) {
		if( CommandRed || (! ExtraEffects) ) {
			Sprite[0]->GetSpr()->SetColor(255,ctRed);          //色を戻す
		}
		else {
			Sprite[0]->GetSpr()->SetColor(OriginColor,ctRed);          //色を戻す
		}
		Sprite[0]->MoveR(-3,0);                  //のけぞり戻し
	}

	if( (Age-LastBuzz)>=40 ) Buzzed = false;

	if( (Age-GaugeTime)>120 ) HPGauge->Hide();

	if( FOwner->BossDie ) {
		FOwner->Add(new TExplarge(FOwner,X,Y,0,true));//爆発
		Die();
	}

}

void TZako3_01::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);
	X = RoundOff(Sprite[0]->GetSpr()->FCX);
	Y = RoundOff(Sprite[0]->GetSpr()->FCY);
	if( target->Kind() == kHero ) {
		if( (! Buzzed) && (target->Hit[HitBuzz]) ) {
			FOwner->Scrach(this);
			Buzzed = true;
			LastBuzz = Age;
		}
		if( (target->Hit[HitBlade]) && (target->LastSlashed > LastSlashed) ) {
			Dec(HP,target->STR());
			Sprite[0]->GetSpr()->SetColor(0,ctRed);
			LastFlash = Age;
			Sprite[0]->MoveR(3,0);                  //のけぞる
			HPGauge->Show();
			GaugeTime = Age;
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WAEdamage0,7);
			LastSlashed = target->LastSlashed;

			if( HP <= 0 ) KnockBack(target->STR()*4);
		}
	}

	if( (target->Kind() == kHShot) || (target->Kind() == kMissile) ) {
		Dec(HP,target->STR());
		Sprite[0]->GetSpr()->SetColor(0,ctRed);
		LastFlash = Age;
		Sprite[0]->MoveR(3,0);                  //のけぞる
		HPGauge->Show();
		GaugeTime = Age;
		if( CreateSE ) FOwner->SoundEffect->WavPlay(WAEdamage0,7);
		if( HP<=0 ) BonusRate = target->BonusRate;
	}

	if( HP<TempHP ) TempHP = HP;
}

void TZako3_01::OnBroken()
{
	TOBJ::OnBroken();
	FOwner->Add(new TExplarge(FOwner,X,Y,0,true));//爆発

}

void TZako3_01::Die()
{
	TOBJ::Die();
	HPGauge->Die();

}

void TZako2_05_2::Action()
{
	switch( ActFlg ){
	case 1:{
		if( CreateSE ) FOwner->SoundEffect->WavPlay(WACharge,8);
		Inc(ActFlg);
		ActCount  =  -1;
		   }break;
	case 2:{

		   }break;
	}
	Inc(ActCount);

}

TZako2_05_2::TZako2_05_2( TOBJList* owner, TJoint* _parent, s32 _x, s32 _y, s32 _r, s32 _distance, s32 _Rspeed, s32 _pat, bool _red ) : TOBJ(owner)
{
	X = _x;
	Y = _y;
	MaxHP = KSTR_SHOT*12;
	HP = MaxHP;
	TempHP = HP;
	FSTR = 500;
	FKind = kEnemy;
	Score = 50000;
	Buzzed = false;
	LastBuzz = 0;
	Pattern = _pat;
	CommandRed = _red;
	GaugeTime = 0;
	TotalFlight = 0;
	LastFlash = 0;
	OriginColor  =  255;
	ComBonus = 5000;
	RSpeed = _Rspeed;
	DistY = _distance;
	Parent = _parent;
	Rdeg = _r;
	ActFlg = 1;
	ActCount = 0;
	CurrentDist = 700;

	//IDの付与
	FID = NewID;
	Inc(NewID);

	MakeSprite(QD, MechaTex,  numSpr,1,65,192, 80,150,  0,  0,  192,  80, 128, 40,CommandRed);
	Sprite[numSpr-1]->GetSpr()->Move(_x,_y);

	if( ExtraEffects ) {
		MakeSprite(QD, MechaTex,  1,1,65,192, 80,149,  0,  0,  0,  0, 128, 40,CommandRed);
		Sprite[numSpr-1]->GetSpr()->Move(_x,_y);
		Sprite[numSpr-1]->GetSpr()->FEffectNum = 1;
		Sprite[numSpr-1]->GetSpr()->FBumpSize = 3.5f;
		Sprite[numSpr-1]->GetSpr()->FBumpTex = MechaTex;
		Sprite[numSpr-1]->GetSpr()->FBumpLevel = -0.2f;
		Sprite[numSpr-1]->GetSpr()->SetColor(128,ctAlpha);
		Sprite[0]->Add(Sprite[numSpr-1]);
	}

	Beam = new TZBeam2(FOwner, Sprite[0], X-120, Y, 1);
	FOwner->Add(Beam);

	Sprite[0]->Rotate(Rdeg);

	HPGauge  =  new TEnemyGauge2(FOwner,this,-128,32);
	FOwner->Add(HPGauge);

}

void TZako2_05_2::Move()
{
	TOBJ::Move();
	if( ! FOwner->BossDie ) {
		s32 DX = RoundOff(Parent->GetSpr()->FCX + CurrentDist*cost[Rdeg]);
		s32 DY = RoundOff(Parent->GetSpr()->FCY + CurrentDist*sint[Rdeg]);
		Sprite[0]->Move(DX,DY);
		Sprite[0]->Rotate(Rdeg+2048);
	}
	else {
		FOwner->Add(new TExpmiddle(FOwner,X,Y,0,0,0));//爆発
		Die();
		return;
	}

	Rdeg = (Rdeg+RSpeed) & 0xfff;
	if( CurrentDist>DistY )
		Dec(CurrentDist,2);

	X = (s32)(Sprite[0]->GetSpr()->FCX);
	Y = (s32)(Sprite[0]->GetSpr()->FCY);

	Action();

	if( (Age-LastFlash == 2) ) {
		if( CommandRed || (! ExtraEffects) ) {
			Sprite[0]->GetSpr()->SetColor(255,ctRed);          //色を戻す
		}
		else {
			Sprite[0]->GetSpr()->SetColor(OriginColor,ctRed);          //色を戻す
		}
		Sprite[0]->MoveR(-3,0);                  //のけぞり戻し
	}

	if( (Age-LastBuzz)>=40 ) Buzzed = false;
	if( HP<=0 ) {
		OnBroken();                        //HPが無くなったら死ぬ
		return;
	}

	if( (Age-GaugeTime)>120 ) HPGauge->Hide();


	if( Age > 1320 ) {
		FOwner->Add(new TExpmiddle(FOwner,X,Y,Rdeg,0,0));
		Die();
	}

}

void TZako2_05_2::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);
	X = RoundOff(Sprite[0]->GetSpr()->FCX);
	Y = RoundOff(Sprite[0]->GetSpr()->FCY);
	if( target->Kind() == kHero ) {
		if( (! Buzzed) && (target->Hit[HitBuzz]) ) {
			FOwner->Scrach(this);
			Buzzed = true;
			LastBuzz = Age;
		}
		if( (target->Hit[HitBlade]) && (target->LastSlashed > LastSlashed) ) {
			Dec(HP,target->STR());
			Sprite[0]->GetSpr()->SetColor(0,ctRed);
			LastFlash = Age;
			Sprite[0]->MoveR(3,0);                  //のけぞる
			HPGauge->Show();
			GaugeTime = Age;
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WAEdamage0,7);
			LastSlashed = target->LastSlashed;
		}
	}

	if( (target->Kind() == kHShot) || (target->Kind() == kMissile) ) {
		Dec(HP,target->STR());
		Sprite[0]->GetSpr()->SetColor(0,ctRed);
		LastFlash = Age;
		Sprite[0]->MoveR(3,0);                  //のけぞる
		HPGauge->Show();
		GaugeTime = Age;
		if( CreateSE ) FOwner->SoundEffect->WavPlay(WAEdamage0,7);
		if( HP<=0 ) BonusRate = target->BonusRate;
	}

	if( HP<TempHP ) TempHP = HP;

}

void TZako2_05_2::Die()
{
	TOBJ::Die();
	HPGauge->Die();
	Beam->Die();
	Dec(FOwner->FunSync);

}

void TZako2_05_2::OnBroken()
{
	TOBJ::OnBroken();
	FOwner->Add(new TExplarge(FOwner,X,Y,0,true));//爆発

}

TZ02_3Gene::TZ02_3Gene( TOBJList* owner, s32 _pat ) : TOBJ(owner)
{
	FObjectName = "TZ02_3Gene";
	FKind = kGene;
	Pattern = _pat;
	Count = 0;
	TotalFlight = 0;
	diffX = 0;

	switch( FOwner->Difficulty ){
	case KDIFFICULTY_EASY:Interval = 1200;	break;
	case KDIFFICULTY_HARD:Interval = 600;	break;
	default:
		Interval = 800;
	}

}

void TZ02_3Gene::Move()
{
	TOBJ::Move();
	if( (TotalFlight>=Interval*Count) ) {
		FOwner->Add(new TZako2_03(FOwner, 800, FOwner->HeroPos.Y, FOwner->GetRand(4095), 2, 4, false));
		Inc(Count);
	}

	Inc(TotalFlight,FOwner->Speed);

	if( FOwner->Distance>=39000 ) Die();

}
