#include "Zako01.h"
#include "Gauge.h"
#include "TOBJList.h"
#include "Effect.h"
#include "Effect2.h"
#include "Effect3.h"
#include "Sound.h"
#include "SXScene.h"
#include "Demo.h"
#include "Font.h"
#include "Command.h"
#include "Missile.h"

TZ01Gene::TZ01Gene( TOBJList* owner, s32 _x, s32 _y, s32 _pat, bool _red ) : TOBJ(owner)
{
	FObjectName = "TZ01Gene";
	X = _x;
	Y = _y;
	FKind = kGene;
	Pattern = _pat;
	Count = 0;
	TotalFlight = 0;
	YDist = 0;

	RanCount = 4;
	ReplayCount = 1;

	CommandRed = _red;
}

void TZ01Gene::Move()
{
	const s32 YPos[] = { 48,144,240,336,432 };
	switch( Pattern ){
		//前方から自機追尾
	case 0:{
		if( (TotalFlight>=80*Count) ) {
			if( Count<=4 )
				FOwner->Add(new TZako01(FOwner,X,Y,0,-6,1,false)); //敵を出す
			else if( Count>=5 ) {
				if( CommandRed ) FOwner->Add(new TZako01(FOwner,X,Y,0,-6,1,true));
				else FOwner->Add(new TZako01(FOwner,X,Y,0,-6,1,false));
				Die();
			}
			Inc(Count);
		}
		Inc(TotalFlight,FOwner->Speed+1); //機体のスピード分足す(+1は敵のスピード-自機のスピード)
		   }break;
		//画面中心で円を描く

		//上下から
	case 1:{
		if( Age % 10 == 0 ) {
			switch( Count ){
			case 0:{
				FOwner->Add(new TZako01(FOwner,X+Age,0,3072,-10,2,true)); //敵を出す
				FOwner->Add(new TZako01(FOwner,X+Age-32,480,1024,-10,3,false)); //敵を出す
				   }break;
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:{
				FOwner->Add(new TZako01(FOwner,X+Age,0,3072,-10,2,false)); //敵を出す
				FOwner->Add(new TZako01(FOwner,X+Age-32,480,1024,-10,3,false)); //敵を出す
				   }break;
			case 9:{
				FOwner->Add(new TZako01(FOwner,X+Age,0,3072,-10,2,false)); //敵を出す
				FOwner->Add(new TZako01(FOwner,X+Age-32,480,1024,-10,3,false)); //敵を出す
				Die();
				   }break;
			}
			Inc(Count);
		}
		Dec(X,FOwner->Speed-5);
		   }break;
		//横一列に延々と
	case 2:{
		if( (TotalFlight>=80*Count) ) {
			FOwner->Add(new TZako01(FOwner,X,Y,0,-3,0,false));
			Inc(Count);
			if( Count>34 ) {
				Die();
			}
		}
		Inc(TotalFlight,FOwner->Speed-2);
		   }break;
		//前からランダム
	case 3:{
		if( (TotalFlight>=80*Count) ) {
			FOwner->Add(new TZako01(FOwner,X,Y,0,-6,0,false));
			if( Y == YDist ) Dec(RanCount);
			else if( Y>YDist ) Dec(Y,24);
			else if( Y<YDist ) Inc(Y,24);

			Inc(Count);
			if( RanCount<=0 ) {


				YDist = YPos[FOwner->RandZYDist[ReplayCount % 8]];
				RanCount = FOwner->RandZCount[ReplayCount % 8];
				Inc(ReplayCount);
			}

			if( Count>79 ) {
				Die();
				//FOwner->TempCount[0] = FOwner->distance;
			}
		}
		Inc(TotalFlight,FOwner->Speed+1); //機体のスピード分足す
		   }break;
		//2列で
	case 4:{
		if( (TotalFlight>=80*Count) ) {
			if( Count<=5 ) {
				FOwner->Add(new TZako01(FOwner,X,Y,0,-6,1,false)); //敵を出す
				FOwner->Add(new TZako01(FOwner,X,Y+96,0,-6,1,false));
			}
			else if( Count>=6 ) {
				FOwner->Add(new TZako01(FOwner,X,Y,0,-6,1,false)); //敵を出す
				FOwner->Add(new TZako01(FOwner,X,Y+96,0,-6,1,true));
				Die();
			}

			Inc(Count);
		}
		Inc(TotalFlight,FOwner->Speed+1); //機体のスピード分足す
		   }break;
		//上から左へ
	case 5:{
		if( Age % 10 == 0 ) {
			switch( Count ){
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:FOwner->Add(new TZako01(FOwner,X+Age,0,3072,-10,4,false));	//敵を出す
				break;
			case 6:{
				if( CommandRed ) FOwner->Add(new TZako01(FOwner,X+Age,0,3072,-10,4,true));
				else FOwner->Add(new TZako01(FOwner,X+Age,0,3072,-10,4,false));
				Die();
				   }break;
			}
			Inc(Count);
		}
		Dec(X,FOwner->Speed-5);
		   }break;
		//下から左へ
	case 6:{
		if( Age % 10 == 0 ) {
			switch( Count ){
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:FOwner->Add(new TZako01(FOwner,X+Age,480,1024,-10,5,false)); //敵を出す
				break;
			case 6:{
				if( CommandRed ) FOwner->Add(new TZako01(FOwner,X+Age,480,1024,-10,5,true));
				else FOwner->Add(new TZako01(FOwner,X+Age,480,1024,-10,5,false));
				Die();
				   }
			}
			Inc(Count);
		}
		Dec(X,FOwner->Speed-5);
		   }break;
		//右から一直線
	case 7:{
		if( (TotalFlight>=70*Count) ) {
			if( Count<=6 ) {
				FOwner->Add(new TZako01(FOwner,X,Y,0,-8,0,false)); //敵を出す
			}
			else if( Count>=7 ) {
				FOwner->Add(new TZako01(FOwner,X,Y,0,-8,0,true));
				Die();
			}

			Inc(Count);
		}
		Inc(TotalFlight,FOwner->Speed+3); //機体のスピード分足す
		   }break;
		//左から一直線
	case 8:{
		if( (TotalFlight>=70*Count) ) {
			if( Count<=6 ) {
				FOwner->Add(new TZako01(FOwner,X-32,Y,0,7,7,false)); //敵を出す
			}
			else if( Count>=7 ) {
				FOwner->Add(new TZako01(FOwner,X-32,Y,0,7,7,true));
				Die();
			}

			Inc(Count);
		}
		Inc(TotalFlight,12-FOwner->Speed); //機体のスピード分足す
		Inc(Y,FOwner->BGSlide.Y);
		if( Y>480 ) Die();
		   }break;
	}

	TOBJ::Move();

}

TZako01::TZako01( TOBJList* owner, s32 _x, s32 _y, s32 _R, s32 _S, s32 _pat, bool _red ) : TOBJ(owner)
{
	FObjectName = "TZako01";

	X = _x;
	Y = _y;
	MaxHP = KSTR_SHOT;
	HP = MaxHP;
	TempHP = HP;
	if( FOwner->IsSequence() )
		FSTR = 250;
	else
		FSTR = 500;
	FKind = kEnemy;
	Score = 300;
	Buzzed = false;
	LastBuzz = 0;
	Rdeg = _R;
	Speed = _S;
	Pattern = _pat;
	CommandRed = _red;
	GaugeOK = false;
	GaugeTime = 0;
	TotalFlight = 0;
	ComBonus = 5000;

	//IDの付与
	FID = NewID;
	Inc(NewID);

	TSpriteEXBLT sblt;
	sblt.RBM      =  bmNormal; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  FOwner->GetDG();     // TDDDDの参照
	sblt.RHeight  =  64;        // 幅
	sblt.RWidth   =  64;        // 高さ
	sblt.RX       =  X;        // X座標
	sblt.RY       =  Y;        // Y座標
	sblt.RZ       =  11000;       // Z値

	Sprite[0] =  new TJoint(sblt);         // スプライトの生成
	Inc(numSpr,1);
	switch( Pattern ){
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
		Sprite[0]->GetSpr()->AddTexture(MechaTex, 0+256, 0+224, 64+256, 64+224);    // テクスチャを登録
		break;
	case 7:
		Sprite[0]->GetSpr()->AddTexture(MechaTex, 64+256, 0+224, 0+256, 64+224);
		break;
	}
	Sprite[0]->GetSpr()->FVisible  =  true;
	Sprite[0]->GetSpr()->AddHitArea(TPoint(9, 5), TPoint(9, 60), TPoint(63, 60),TPoint(63, 5), htSky);
	Sprite[0]->GetSpr()->FHit      =  true;
	Sprite[0]->GetSpr()->FCX += 32;
	Sprite[0]->GetSpr()->FCY += 32;
	//Bump = true;
	//BumpSize = 200;
	if( CommandRed ) {
		Sprite[0]->GetSpr()->SetColor(255, ctRed);
		Sprite[0]->GetSpr()->SetColor(64, ctBlue);
		Sprite[0]->GetSpr()->SetColor(64, ctGreen);
	}

}

void TZako01::Move()
{
	TOBJ::Move();
	Sprite[0]->GetSpr()->SetColor(255,ctRed);          //色を戻す

	X = (s32)(Sprite[0]->GetSpr()->FCX);
	Y = (s32)(Sprite[0]->GetSpr()->FCY);

	switch( Pattern ){
		//直進型
	case 0:{
		//if( Age % 100 =0  ) MakeShot;    //100フレームごとに弾を吐く
		   }break;
		//自機追尾
	case 1:{
		if( FOwner->HeroPos.Y>Y )
			Dec(Rdeg,3);
		else if( FOwner->HeroPos.Y<Y )
			Inc(Rdeg,6);
		   }break;
		//円を描く(時計回り)
	case 2:{
		if( (TotalFlight >= 214) && (Age <= 200) ) Inc(Rdeg,31);   //1フレームあたりの角度=4096/(円の直径*π/Speed)

		if( (Age>200) ) Inc(Rdeg,32-(Age-200));

		if( (Age == 20) && (FOwner->Difficulty == KDIFFICULTY_HARD)  ) 
			MakeShot();
		   }break;
		//円を描く(反時計回り)
	case 3:{
		if( (TotalFlight>=272) && (Age<=170) ) Dec(Rdeg,37);

		if( (Age>170) ) Dec(Rdeg,32-(Age-170));

		if( (Age == 20) && (FOwner->Difficulty == KDIFFICULTY_HARD)  ) 
			MakeShot();
		   }break;
		//上から左
	case 4:{
		Inc(Rdeg,15);   //1フレームあたりの角度=4096/(円の直径*π/Speed)
		if( (Age == 20) || (Age == 150)  ) 
			MakeShot();   //弾を吐く
		   }break;
		//下から左
	case 5:{
		Dec(Rdeg,15);   //1フレームあたりの角度=4096/(円の直径*π/Speed)
		if( (Age == 20) || (Age == 150)  ) 
			MakeShot();   //弾を吐く
		   }break;
	case 7:{

		   }break;
	}
	Rdeg = Rdeg & 0x0fff;
	Sprite[0]->SlideMoveR(Speed,0);              //移動
	Inc(TotalFlight,abs(Speed));
	Sprite[0]->Rotate(Rdeg);

	X = (s32)(Sprite[0]->GetSpr()->FCX);
	Y = (s32)(Sprite[0]->GetSpr()->FCY);

	if( Age % 3 == 0 )
		FOwner->Add(new TZ01BFire(FOwner,X,Y,Rdeg));

	if( Pattern!=7 )
		FOwner->Add(new TBLight(FOwner,(s32)((f32)X+cost[Rdeg]*3.f),(s32)((f32)Y+sint[Rdeg]*3.f),1,2000));
	else
		FOwner->Add(new TBLight(FOwner,(s32)((f32)X-cost[Rdeg]*3.f),(s32)((f32)Y+sint[Rdeg]*3.f),1,2000));



	if( (Age-LastBuzz)>=40 ) Buzzed = false;

	switch( Pattern ){
	case 0:
	case 1:
	case 4:
	case 5:
	case 7:
		{
			if( (X > 640+64) || (Y > 480+64) || (Y <0-64) || (X < 0-64) ) {
				Die();//画面外に消えたら消す & 母艦にダメージ
				Dec(FOwner->MShipHP,FSTR / 2);
				FOwner->Add(new TMotherShipDamaged(FOwner, X, Y));
			}
		}break;
	case 2:
	case 3:
		{
			if( (X > 640+64) || (Y > 480+64) || (Y <0-64) || (X < 0-256) ) {
				Die();//画面外に消えたら消す & 母艦にダメージ
				Dec(FOwner->MShipHP,FSTR / 2);
				FOwner->Add(new TMotherShipDamaged(FOwner, X, Y));
			}
		}break;
	}
	if( HP<=0 ) OnBroken();                        //HPが無くなったら死ぬ

	if( (Age-GaugeTime)>40 ) GaugeOK = false;

}

void TZako01::MakeShot()
{
	if( FOwner->Difficulty == KDIFFICULTY_EASY ) 
		return; 

	if( FOwner->IsSequence() ) {
		s32 DX = RoundOff(FOwner->HeroPos.X-X);             //自機と敵との距離を計算
		s32 DY = RoundOff(FOwner->HeroPos.Y-Y);
		s32 GunR = RoundOff(atan2(DY,DX)/(f32)M_PI*2048.f) & 0xfff;
		FOwner->Add(new TZ01Shot4(FOwner,X,Y,GunR,4,0,0,500));
	}
	else
		FOwner->Add(new TZ01Shot(FOwner,X,Y));

	FOwner->Add(new TFCircle(FOwner,X,Y,1,Rdeg,Speed));
	if( CreateSE ) FOwner->SoundEffect->WavPlay(WABShot,9);

}

void TZako01::OnDamaged( TOBJ* target )
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
		if( (target->Hit[HitBlade]) ) {
			s32 DX = (s32)(target->X-X);
			s32 DY = (s32)(target->Y-Y);
			s32 RR = (s32)(atan2(DY,DX)/(f32)M_PI*2048.f) & 0xfff;
			Dec(HP,target->STR());
			Sprite[0]->GetSpr()->SetColor(0,ctRed);           //色を変える
			Sprite[0]->MoveR(-(s32)(cost[RR]*(f32)target->STR()),-(s32)(sint[RR]*(f32)target->STR()));                  //のけぞる
			GaugeOK = true;
			GaugeTime = Age;
		}
	}

	if( target->Kind() == kHShot ) {
		Dec(HP,target->STR());
		Sprite[0]->GetSpr()->SetColor(0,ctRed);           //色を変える
		Sprite[0]->MoveR(target->STR(),0);                  //のけぞる
		GaugeOK = true;
		GaugeTime = Age;
	}

	if( target->Kind() == kMissile ) {
		s32 DX = (s32)(target->X-X);
		s32 DY = (s32)(target->Y-Y);
		s32 RR = (s32)(atan2(DY,DX)/(f32)M_PI*2048.f) & 0xfff;
		Dec(HP,target->STR());
		Sprite[0]->GetSpr()->SetColor(0,ctRed);           //色を変える
		Sprite[0]->MoveR(-(s32)(cost[RR]*(f32)target->STR()),-(s32)(sint[RR]*(f32)target->STR()));                  //のけぞる
		GaugeOK = true;
		GaugeTime = Age;
		if( HP<=0 ) BonusRate = target->BonusRate;
	}
	if( HP<TempHP ) TempHP = HP;

}

void TZako01::Die()
{
	TOBJ::Die();

}

void TZako01::OnBroken()
{
	TOBJ::OnBroken();
	FOwner->Add(new TExpSmall(FOwner,X,Y,Rdeg,Speed,0));//爆発

}

TZ01Shot::TZ01Shot( TOBJList* owner, s32 sx, s32 sy ) : TOBJ(owner)
{
	FObjectName = "TZ01Shot";
	X = sx;
	Y = sy;
	FSTR = 250;
	FKind = kEShot;
	Buzzed = false;
	LastBuzz = 0;
	Anim = false;
	Rdeg = 0;

	TSpriteEXBLT sblt;
	sblt.RBM      =  bmNormal; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  FOwner->GetDG();     // TDDDDの参照
	sblt.RHeight  =  64;        // 幅
	sblt.RWidth   =  64;        // 高さ
	sblt.RX       =  X;        // X座標
	sblt.RY       =  Y;        // Y座標
	sblt.RZ       =  TamaZ;       // Z値

	Inc(TamaZ);
	if( TamaZ>1099 ) TamaZ = 1000;

	Sprite[0] =  new TJoint(sblt);         // スプライトの生成
	Inc(numSpr,1);
	//    AddTexture(Eff1Tex, 176, 113, 176+31, 113+31);    // テクスチャを登録
	switch( FOwner->NowStage ){
	case 0:
	case 2:
	case 3: 
		Sprite[0]->GetSpr()->AddTexture(Eff8Tex, 32, 224, 32+31, 224+31);
		break;
	default:
		Sprite[0]->GetSpr()->AddTexture(Eff8Tex, 64, 224, 64+31, 224+31);
	}
	Sprite[0]->GetSpr()->FVisible  =  true;
	if( FOwner->IsSequence() )
		Sprite[0]->GetSpr()->AddHitArea(TPoint(26, 28), TPoint(26, 36), TPoint(36, 36),TPoint(36, 28), htSky);
	else
		Sprite[0]->GetSpr()->AddHitArea(TPoint(24, 24), TPoint(24, 40), TPoint(40, 40),TPoint(40, 24), htSky);
	Sprite[0]->GetSpr()->FHit      =  true;
	Sprite[0]->GetSpr()->FCX += 32;
	Sprite[0]->GetSpr()->FCY += 32;

	s32 DX = RoundOff(FOwner->HeroPos.X-X);             //自機と敵との距離を計算
	s32 DY = RoundOff(FOwner->HeroPos.Y-Y);
	s32 Rdeg = RoundOff(atan2(DY,DX)/(f32)M_PI*2048.f) & 0xfff;

	Sprite[0]->GetSpr()->Move(X,Y); //弾の初期位置を設定
	Sprite[0]->GetSpr()->Rotate(Rdeg); //弾の方角を決定

}

void TZ01Shot::Move()
{
	TOBJ::Move();
	X = (s32)(Sprite[0]->GetSpr()->FCX);
	Y = (s32)(Sprite[0]->GetSpr()->FCY);

	Sprite[0]->SlideMoveR(4,0); //弾の移動

	if( Age % 3 == 0 ) Anim = ! Anim;

	Sprite[0]->Rotate(0);
	if( Anim ) Sprite[0]->GetSpr()->Zoom(1.f, 0.6f);
	else Sprite[0]->GetSpr()->Zoom(1.f, 1.1f);
	Sprite[0]->Rotate(Rdeg);

	if( (Age-LastBuzz)>=40 ) Buzzed = false;

	if( Age>=240 ) {
		FOwner->Add(new TFCircle(FOwner,X,Y,1,Rdeg,4));
		Die();
	}

	if( (X > 640+32) || (Y > 480+32) || (Y <0-32) || (X < 0-32) ) 
		Die();//弾が画面外に消えたら消す
}

void TZ01Shot::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);
	if( target->Kind() == kHero ) {
		if( (! Buzzed) && (target->Hit[HitBuzz]) ) {
			FOwner->Scrach(this);
			Buzzed = true;
			LastBuzz = Age;
		}

		if( target->Hit[HitBody] ) {
			Die();
			FOwner->Add(new TExpSS(FOwner,X,Y,0,-10));
		}
	}
}

TZ01Shot2::TZ01Shot2( TOBJList* owner, s32 sx, s32 sy, s32 _R, s32 _S, f32 _IniXS ) : TOBJ(owner)
{
	FObjectName = "TZ01Shot2";
	X = sx;
	Y = sy;
	FSTR = 250;
	FKind = kEShot;
	Buzzed = false;
	LastBuzz = 0;
	Anim = false;
	Rdeg = _R;
	Speed = _S;
	IniXSpeed = _IniXS;

	TSpriteEXBLT sblt;
	sblt.RBM      =  bmNormal; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  FOwner->GetDG();     // TDDDDの参照
	sblt.RHeight  =  64;        // 高さ
	sblt.RWidth   =  64;        // 幅
	sblt.RX       =  X;        // X座標
	sblt.RY       =  Y;        // Y座標
	sblt.RZ       =  TamaZ;       // Z値

	Inc(TamaZ);
	if( TamaZ>1099 ) TamaZ = 1000;

	Sprite[0] =  new TJoint(sblt);         // スプライトの生成
	Inc(numSpr,1);

	switch( FOwner->NowStage )
	{
	case 0:
	case 2:
	case 3: 
		Sprite[0]->GetSpr()->AddTexture(Eff8Tex, 32, 224, 32+31, 224+31);
		break;
	default:
		Sprite[0]->GetSpr()->AddTexture(Eff8Tex, 64, 224, 64+31, 224+31);
	}

	Sprite[0]->GetSpr()->FVisible  =  true;
	if( FOwner->IsSequence() )
		Sprite[0]->GetSpr()->AddHitArea(TPoint(26, 28), TPoint(26, 36), TPoint(36, 36),TPoint(36, 28), htSky);
	else
		Sprite[0]->GetSpr()->AddHitArea(TPoint(24, 24), TPoint(24, 40), TPoint(40, 40),TPoint(40, 24), htSky);
	Sprite[0]->GetSpr()->FHit      =  true;
	Sprite[0]->GetSpr()->FCX += 32;
	Sprite[0]->GetSpr()->FCY += 32;
	Sprite[0]->GetSpr()->Zoom(1,1);

	Sprite[0]->GetSpr()->Move(X,Y); //弾の初期位置を設定
	Sprite[0]->GetSpr()->Rotate(Rdeg); //弾の方角を決定

}

void TZ01Shot2::Move()
{
	TOBJ::Move();
	X = (s32)(Sprite[0]->GetSpr()->FCX);
	Y = (s32)(Sprite[0]->GetSpr()->FCY);

	Sprite[0]->SlideMoveR(Speed,0); //弾の移動
	Sprite[0]->MoveR(IniXSpeed,0);  //撃ったオブジェクトのスピードを足す

	if( Age % 3 == 0 ) Anim = ! Anim;

	Sprite[0]->Rotate(0);
	if( Anim ) Sprite[0]->GetSpr()->Zoom(1.f, 0.6f);
	else Sprite[0]->GetSpr()->Zoom(1.f, 1.1f);
	Sprite[0]->Rotate(Rdeg);

	if( Age == 30 )
		Sprite[0]->GetSpr()->FZ  =  Sprite[0]->GetSpr()->FZ - 150;

	if( (Age-LastBuzz) >= 40 ) Buzzed = false;

	if( Age >= 240 ) {
		FOwner->Add(new TFCircle(FOwner,X,Y,1,Rdeg,Speed));
		Die();
	}

	if( (X > 640+32) || (Y > 480+32) || (Y <0-32) || (X < 0-32) ) 
		Die();//弾が画面外に消えたら消す

}

void TZ01Shot2::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);
	if( target->Kind() == kHero ) {
		if( (! Buzzed) && (target->Hit[HitBuzz]) ) {
			FOwner->Scrach(this);
			Buzzed = true;
			LastBuzz = Age;
		}

		if( target->Hit[HitBody] ) {
			Die();
			FOwner->Add(new TExpSS(FOwner,X,Y,0,-10));
		}
	}
}

TZ01BFire::TZ01BFire( TOBJList* owner, s32 sx, s32 sy, s32 _R ) : TOBJ(owner)
{
	FObjectName = "TZ01BFire";
	X = sx;
	Y = sy;
	Rdeg = _R;
	FKind = kEffect;

	TSpriteEXBLT sblt;
	sblt.RBM      =  bmSubAdd; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  FOwner->GetDG();     // TDDDDの参照
	sblt.RHeight  =  32;        // 幅
	sblt.RWidth   =  32;        // 高さ
	sblt.RX       =  0;        // X座標
	sblt.RY       =  0;        // Y座標
	sblt.RZ       =  20100;       // Z値

	Sprite[0] =  new TJoint(sblt);         // スプライトの生成
	Inc(numSpr,1);
	for( u32 k = 0; k <= 3; k++ )
		Sprite[0]->GetSpr()->AddTexture(Eff2Tex, k*63, 0, 63+k*64, 63);    // テクスチャを登録
	for( u32 k = 0; k <= 3; k++ )
		Sprite[0]->GetSpr()->AddTexture(Eff2Tex, k*63, 64, 63+k*64, 127);    // テクスチャを登録
	for( u32 k = 0; k <= 3; k++ )
		Sprite[0]->GetSpr()->AddTexture(Eff2Tex, k*63, 128, 63+k*64, 191);    // テクスチャを登録
	Sprite[0]->GetSpr()->FAnim = true;
	Sprite[0]->GetSpr()->FAnimBgnIdx = 5;
	Sprite[0]->GetSpr()->FAnimEndIdx = 11;
	Sprite[0]->GetSpr()->FAnimItv = 3;
	Sprite[0]->GetSpr()->FCX += 16;
	Sprite[0]->GetSpr()->FCY += 16;
	Sprite[0]->GetSpr()->Move(X,Y);
	Sprite[0]->GetSpr()->Zoom(1.5f, 0.9f);
	Sprite[0]->GetSpr()->Rotate(Rdeg);
	Sprite[0]->GetSpr()->SetColor(255, ctRed);
	Sprite[0]->GetSpr()->SetColor(96, ctBlue);
	Sprite[0]->GetSpr()->SetColor(96, ctGreen);
	Sprite[0]->GetSpr()->SetColor(96,ctAlpha);
	Sprite[0]->GetSpr()->FVisible = true;
}

void TZ01BFire::Move()
{
	if( Age % 1 == 0 ) {
		Sprite[0]->GetSpr()->AnimNext();
		Sprite[0]->GetSpr()->AnimNext();
		Sprite[0]->GetSpr()->Rotate(0);
		Sprite[0]->GetSpr()->ZoomR(1.f, 0.8f);
		Sprite[0]->GetSpr()->Rotate(Rdeg);
	}
	Sprite[0]->SlideMoveR(2,0);

	if( Age>=4 ) Die();
	TOBJ::Move();

}

TZ02Gene::TZ02Gene( TOBJList* owner, s32 _x, s32 _y, s32 _pat ) : TOBJ(owner)
{
	X = _x;
	Y = _y;
	FKind = kGene;
	Pattern = _pat;
	Count = 0;
	TotalFlight = 0;
	FObjectName = "TZ02Gene";
}

void TZ02Gene::Move()
{
	const s32 ZakoY[] = { 0,-48,48 };
	switch( Pattern ){
		//3機ずつ前から
	case 0:{
		if( (TotalFlight>=100*Count) ) {
			FOwner->Add(new TZako02(FOwner,X,Y+ZakoY[Count],0,-8,0,false)); //敵を出す
			Inc(Count);
			if( Count>2 ) Die();
		}
		Inc(TotalFlight,FOwner->Speed+3); //機体のスピード分足す
		   }break;
		//上下から
	case 1:{
		if( (TotalFlight>=100*Count) ) {
			switch( Count ){
			case 0:FOwner->Add(new TZako02(FOwner,X,80,0,-8,0,false)); break; //敵を出す
			case 1:FOwner->Add(new TZako02(FOwner,X,380,0,-8,0,false)); break;
			case 2:FOwner->Add(new TZako02(FOwner,X,80+48,0,-8,0,false)); break;
			case 3:FOwner->Add(new TZako02(FOwner,X,380-48,0,-8,0,false)); break;
			case 4:FOwner->Add(new TZako02(FOwner,X,80-48,0,-8,0,true)); break;
			case 5:FOwner->Add(new TZako02(FOwner,X,380+48,0,-8,0,true)); break;
			}
			Inc(Count);
			if( Count>5 ) Die();
		}
		Inc(TotalFlight,FOwner->Speed+3); //機体のスピード分足す
		   }break;
	}

	TOBJ::Move();
}

TZako02::TZako02( TOBJList* owner, s32 _x, s32 _y, s32 _R, s32 _S, s32 _pat, bool _red ) : TOBJ(owner)
{
	FObjectName = "TZako02";

	X = _x;
	Y = _y;
	MaxHP = KSTR_SHOT*3;
	HP = MaxHP;
	TempHP = HP;
	FSTR = 1000;
	FKind = kEnemy;
	Score = 5000;
	Buzzed = false;
	LastBuzz = 0;
	Rdeg = _R;
	Speed = _S;
	Pattern = _pat;
	CommandRed = _red;
	GaugeTime = 0;
	TotalFlight = 0;
	ActFlg = 0;
	ActCount = 0;
	ComBonus = 5000;

	//IDの付与
	FID = NewID;
	Inc(NewID);


	s32 tempX = X;
	s32 tempY = Y;

	TSpriteEXBLT sblt;
	sblt.RBM      =  bmNormal;    // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  FOwner->GetDG();        // TDDDDの参照
	sblt.RWidth   =  64;        // 幅
	sblt.RHeight  =  48;        // 高さ
	sblt.RX       =  X;         // X座標
	sblt.RY       =  Y;         // Y座標
	sblt.RZ       = 11000;        // Z値

	Sprite[Z2Body] =  new TJoint(sblt);         // Body
	Inc(numSpr,1);

	sblt.RBM      =  bmNormal;    // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  FOwner->GetDG();        // TDDDDの参照
	sblt.RWidth   =  48;        // 幅
	sblt.RHeight  =  32;        // 高さ
	sblt.RX       =  X;         // X座標
	sblt.RY       =  Y;         // Y座標
	sblt.RZ       =  11100;        // Z値

	Sprite[Z2Wing] =  new TJoint(sblt);         // Wing
	Inc(numSpr,1);

	sblt.RBM      =  bmNormal;    // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  FOwner->GetDG();        // TDDDDの参照
	sblt.RWidth   =  64;        // 幅
	sblt.RHeight  =  32;        // 高さ
	sblt.RX       =  X;         // X座標
	sblt.RY       =  Y;         // Y座標
	sblt.RZ       =  11100;        // Z値

	Sprite[Z2Tail] =  new TJoint(sblt);         // Tail
	Inc(numSpr,1);

	sblt.RBM      =  bmNormal;    // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  FOwner->GetDG();        // TDDDDの参照
	sblt.RWidth   =  80;        // 幅
	sblt.RHeight  =  80;        // 高さ
	sblt.RX       =  X;         // X座標
	sblt.RY       =  Y;         // Y座標
	sblt.RZ       =  4900;        // Z値         //49

	Sprite[Z2Sonic] =  new TJoint(sblt);         // Sonic
	Inc(numSpr,1);

	switch( Pattern ){
	case 0:Sprite[Z2Body]->GetSpr()->AddTexture(MechaTex, 0+176, 288, 63+176, 47+288);   break; // テクスチャを登録
	case 1:Sprite[Z2Body]->GetSpr()->AddTexture(MechaTex, 63+176, 288, 0+176, 47+288);	break;
	}
	Sprite[Z2Body]->GetSpr()->FVisible  =  true;
	Sprite[Z2Body]->GetSpr()->AddHitArea(TPoint(0, 0), TPoint(0, 63), TPoint(40, 63),TPoint(40, 0), htSky);
	Sprite[Z2Body]->GetSpr()->FHit      =  true;
	Sprite[Z2Body]->GetSpr()->FCX += 20;
	Sprite[Z2Body]->GetSpr()->FCY += 19;
	if( CommandRed ) {
		Sprite[Z2Body]->GetSpr()->SetColor(255, ctRed);
		Sprite[Z2Body]->GetSpr()->SetColor(64, ctBlue);
		Sprite[Z2Body]->GetSpr()->SetColor(64, ctGreen);
	}
	//Bump = true;
	Sprite[Z2Body]->GetSpr()->FBumpSize = 200;

	switch( Pattern ){
	case 0:Sprite[Z2Wing]->GetSpr()->AddTexture(MechaTex, 64+176, 288, 111+176, 31+288);   break; // テクスチャを登録
	case 1:Sprite[Z2Wing]->GetSpr()->AddTexture(MechaTex, 111+176, 288, 64+176, 31+288);		break;
	}
	Sprite[Z2Wing]->GetSpr()->FVisible  =  true;
	Sprite[Z2Wing]->GetSpr()->FCX += 8;
	Sprite[Z2Wing]->GetSpr()->FCY += 27;
	if( CommandRed ) {
		Sprite[Z2Wing]->GetSpr()->SetColor(255, ctRed);
		Sprite[Z2Wing]->GetSpr()->SetColor(64, ctBlue);
		Sprite[Z2Wing]->GetSpr()->SetColor(64, ctGreen);
	}

	//Bump = true;
	Sprite[Z2Wing]->GetSpr()->FBumpSize = 200;
	Sprite[Z2Wing]->GetSpr()->Move(tempX+17,tempY+10);

	switch( Pattern ){
	case 0:Sprite[Z2Tail]->GetSpr()->AddTexture(MechaTex, 64+176, 32+288, 127+176, 63+288);  break; // テクスチャを登録
	case 1:Sprite[Z2Tail]->GetSpr()->AddTexture(MechaTex, 127+176, 32+288, 64+176, 63+288);	break;
	}
	Sprite[Z2Tail]->GetSpr()->FVisible  =  true;
	Sprite[Z2Tail]->GetSpr()->FCX += 4;
	Sprite[Z2Tail]->GetSpr()->FCY += 4;
	if( CommandRed ) {
		Sprite[Z2Tail]->GetSpr()->SetColor(255, ctRed);
		Sprite[Z2Tail]->GetSpr()->SetColor(64, ctBlue);
		Sprite[Z2Tail]->GetSpr()->SetColor(64, ctGreen);
	}
	//Bump = true;
	Sprite[Z2Tail]->GetSpr()->FBumpSize = 200;
	Sprite[Z2Tail]->GetSpr()->Move(tempX+40,tempY+29);

	switch( Pattern ){
	case 0:Sprite[Z2Sonic]->GetSpr()->AddTexture(Eff1Tex, 176, 32, 176+79, 32+79);    break;// テクスチャを登録
	case 1:Sprite[Z2Sonic]->GetSpr()->AddTexture(Eff1Tex, 79+176, 32, 176, 32+79);    break;
	}
	Sprite[Z2Sonic]->GetSpr()->FVisible  =  true;
	Sprite[Z2Sonic]->GetSpr()->FCX += 38;
	Sprite[Z2Sonic]->GetSpr()->FCY += 40;
	Sprite[Z2Sonic]->GetSpr()->SetColor(0,ctAlpha);
	Sprite[Z2Sonic]->GetSpr()->Move(tempX+20,tempY+19);
	//Bump = true;
	Sprite[Z2Sonic]->GetSpr()->FBumpSize = 200;

	//間接ドッキング
	Sprite[Z2Body]->Add(Sprite[Z2Wing]);
	Sprite[Z2Body]->Add(Sprite[Z2Tail]);
	Sprite[Z2Body]->Add(Sprite[Z2Sonic]);

	Sprite[Z2Wing]->RotateR(-400);
	Sprite[Z2Tail]->RotateR(1000);

	HPGauge  =  new TEnemyGauge2(FOwner,this,0,32);
	FOwner->Add(HPGauge);
}

void TZako02::Move()
{
	TOBJ::Move();
	Sprite[Z2Body]->GetSpr()->SetColor(255,ctRed);          //色を戻す

	f32 DX = FOwner->HeroPos.X-X;
	f32 DY = FOwner->HeroPos.Y-Y;

	s32 HeroDeg = RoundOff(atan2(DY,DX)/(f32)M_PI*2048.f) + 2048;
	HeroDeg = HeroDeg & 0x0fff;
	Rdeg = Rdeg & 0x0fff;
	switch( Pattern ){
		//直進型
	case 0:{
		switch( ActFlg ){
		case 0:{
			if( ActCount % 4 == 0 ) Inc(Speed,1);
			if( Speed >= 0 ) {
				ActFlg = 1;
				ActCount = -1;
				MakeShot();
			}

			if( abs(Rdeg-HeroDeg)>=2048 ) Inc(Rdeg,-RoundOff((f32)(4096-Rdeg-HeroDeg)/16.f));
			if( abs(Rdeg-HeroDeg)<2048 ) Inc(Rdeg,-RoundOff((f32)(Rdeg-HeroDeg)/16.f));
			   }break;

		case 1:{
			Speed = 0;
			if( Sprite[Z2Wing]->Rdegree != Rdeg ) Sprite[Z2Wing]->RotateR(10);
			if( Sprite[Z2Tail]->Rdegree != Rdeg ) Sprite[Z2Tail]->RotateR(-25);

			s32 nextcount;
			if( FOwner->Difficulty <= KDIFFICULTY_NORMAL )
				nextcount  =  180;
			else
				nextcount  =  60;

			if( ActCount > nextcount ) {
				ActFlg = 2;
				ActCount = -1;
				if( CreateSE ) FOwner->SoundEffect->WavPlay(WAEBoost,13);
			}

			if( FOwner->IsSequence() ) {
				if( abs(Rdeg-HeroDeg)>=2048 ) Inc(Rdeg,Sign(Rdeg-HeroDeg)*RoundOff((f32)(4095-abs(Rdeg-HeroDeg))/16.f));
				else if( abs(Rdeg-HeroDeg)<2048 ) Dec(Rdeg,RoundOff((f32)(Rdeg-HeroDeg)/16.f));
			}
			else {
				if( abs(Rdeg-HeroDeg)>=2048 ) Inc(Rdeg,-RoundOff((f32)(4096-Rdeg-HeroDeg)/16.f));
				if( abs(Rdeg-HeroDeg)<2048 ) Inc(Rdeg,-RoundOff((f32)(Rdeg-HeroDeg)/16.f));
			}
			   }break;
		case 2:{
			if( ActCount == 0 ) FOwner->Add(new TSRControl(FOwner,X,Y,Rdeg+2048));
			if( Sprite[Z2Wing]->Rdegree != Rdeg ) Sprite[Z2Wing]->RotateR(20);
			if( Sprite[Z2Tail]->Rdegree != Rdeg ) Sprite[Z2Tail]->RotateR(-50);
			if( ActCount <= 15 ) Sprite[Z2Sonic]->GetSpr()->SetColor(ActCount*17,ctAlpha);
			Dec(Speed);

			//噴射
			if( Age % 3 == 0 )
				FOwner->Add(new TZ01BFire(FOwner,X+Speed,Y,Rdeg));
			//火花
			FOwner->Add(new TParticle2(FOwner,X+Speed+32,Y,0,2048+(random(600)-300),5+random(10),5+random(10)));
			   }
		}
		   }
	}

	Sprite[Z2Body]->Rotate(Rdeg);

	Sprite[Z2Body]->SlideMoveR(Speed,0);
	Inc(TotalFlight,abs(Speed));

	X = (s32)(Sprite[Z2Body]->GetSpr()->FCX);
	Y = (s32)(Sprite[Z2Body]->GetSpr()->FCY);

	if( (Age-LastBuzz)>=40 ) Buzzed = false;

	if( (X > 640+64) || (Y > 480+64) || (Y <0-64) || (X < 0-64) ) {
		Die();//画面外に消えたら消す & 母艦にダメージ
		Dec(FOwner->MShipHP,FSTR / 2);
		FOwner->Add(new TMotherShipDamaged(FOwner, X, Y));
	}
	if( HP<=0 ) OnBroken();                        //HPが無くなったら死ぬ

	//ゲージ表示
	//  if( (GaugeOK and (FDead=false)) )
	//    FOwner->Add(new TEnemyGauge(FOWner,X-40,Y+32,HP,MaxHP));

	if( (Age-GaugeTime)>120 ) HPGauge->Hide();

	Inc(ActCount);
}

void TZako02::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);
	X = (s32)(Sprite[Z2Body]->GetSpr()->FCX);
	Y = (s32)(Sprite[Z2Body]->GetSpr()->FCY);
	if( target->Kind() == kHero ) {
		if( (! Buzzed) && (target->Hit[HitBuzz]) ) {
			FOwner->Scrach(this);
			Buzzed = true;
			LastBuzz = Age;
		}
		if( (target->Hit[HitBlade]) ) {
			s32 DX = (s32)(target->X-X);
			s32 DY = (s32)(target->Y-Y);
			s32 RR = (s32)(atan2(DY,DX)/(f32)M_PI*2048.f) & 0xfff;
			Dec(HP,target->STR());
			Sprite[0]->GetSpr()->SetColor(0,ctRed);           //色を変える
			Sprite[0]->MoveR(-(s32)(cost[RR]*(f32)target->STR()),-(s32)(sint[RR]*(f32)target->STR()));                  //のけぞる
			HPGauge->Show();
			GaugeTime = Age;
		}
	}

	if( target->Kind() == kHShot ) {
		Dec(HP,target->STR());
		Sprite[Z2Body]->GetSpr()->SetColor(0,ctRed);           //色を変える
		Sprite[Z2Body]->MoveR(target->STR(),0);                  //のけぞる
		HPGauge->Show();
		GaugeTime = Age;
		if( CreateSE ) FOwner->SoundEffect->WavPlay(WAEdamage0,1);
	}

	if( target->Kind() == kMissile ) {
		s32 DX = (s32)(target->X-X);
		s32 DY = (s32)(target->Y-Y);
		s32 RR = (s32)(atan2(DY,DX)/(f32)M_PI*2048.f) & 0xfff;
		Dec(HP,target->STR() );
		Sprite[Z2Body]->GetSpr()->SetColor(0,ctRed);           //色を変える
		Sprite[Z2Body]->MoveR(-(s32)(cost[RR]*(f32)target->STR()),-(s32)(sint[RR]*(f32)target->STR()));                  //のけぞる
		HPGauge->Show();
		GaugeTime = Age;
		if( CreateSE ) FOwner->SoundEffect->WavPlay(WAEdamage0,7);
		if( HP<=0 ) BonusRate = target->BonusRate;
	}
	if( HP<TempHP ) TempHP = HP;

}

void TZako02::Die()
{
	HPGauge->Die();
	TOBJ::Die();
}

void TZako02::MakeShot()
{
	for( u32 i = 0; i <= 2; i++ ) {
		if( FOwner->IsSequence() ) {
			FOwner->Add(new TZ01Shot4(FOwner,X,Y,Rdeg+1792,6+i,0,0,500));
			FOwner->Add(new TZ01Shot4(FOwner,X,Y,Rdeg+2304,6+i,0,0,500));
		}
		else {
			FOwner->Add(new TZ01Shot2(FOwner,X,Y,Rdeg+1792,6+i,0));
			FOwner->Add(new TZ01Shot2(FOwner,X,Y,Rdeg+2304,6+i,0));
		}
		FOwner->Add(new TFCircle(FOwner,X,Y,1,Rdeg,RoundOff(Speed)));
		if( CreateSE ) FOwner->SoundEffect->WavPlay(WABShot,9);
	}

}

void TZako02::OnBroken()
{
	TOBJ::OnBroken();
	FOwner->Add(new TExpmiddle(FOwner,X,Y,Rdeg,Speed,0));//爆発
}

TZ03Gene::TZ03Gene( TOBJList* owner, s32 _x, s32 _y, s32 _pat ) : TOBJ(owner)
{
	FObjectName = "TZ03Gene";
	X = _x;
	Y = _y;
	FKind = kGene;
	Pattern = _pat;
	Count = 0;
	TotalFlight = 0;

}

void TZ03Gene::Move()
{
	const s32 ZakoY[] = { 0,-48,48,-96,96 };
	const s32 RandomY[] = { 0,2,-1,-4,3,1,-5,6,-3 };
	switch( Pattern ){
		//前方から5機
	case 0:{      //赤が一番下
		if( (TotalFlight >= 100*Count) ) {
			if( Count<4 ) 
				FOwner->Add(new TZako03(FOwner,640+32,Y+ZakoY[Count],0,-10,0,false));
			else {
				FOwner->Add(new TZako03(FOwner,640+32,Y+ZakoY[Count],0,-10,0,true));
				Die();
			}
			Inc(Count);
		}
		Inc(TotalFlight,FOwner->Speed+15);
		   }break;
	case 1:{      //赤が一番上
		if( (TotalFlight >= 100*Count) ) {
			if( Count<4 )
				FOwner->Add(new TZako03(FOwner,640+32,Y-ZakoY[Count],0,-10,0,false));
			else {
				FOwner->Add(new TZako03(FOwner,640+32,Y-ZakoY[Count],0,-10,0,true));
				Die();
			}
			Inc(Count);
		}
		Inc(TotalFlight,FOwner->Speed+15);
		   }break;
		//一列に並んで
	case 2:{      //上から
		if( (TotalFlight >= 100*Count) ) {
			if( Count<9 ) 
				FOwner->Add(new TZako03(FOwner,640+32,24+48*Count,0,-10,4,false));
			else {
				//FOwner->Add(new TZako03(FOwner,640+32,Y-ZakoY[Count],0,-20,0,true));
				Die();
			}
			Inc(Count);
		}
		Inc(TotalFlight,FOwner->Speed+15);
		   }break;
	case 3:{      //下から
		if( (TotalFlight >= 100*Count) ) {
			if( Count<8 ) 
				FOwner->Add(new TZako03(FOwner,640+32,408-48*Count,0,-10,4,false));
			else {
				FOwner->Add(new TZako03(FOwner,640+32,24,0,-10,0,true));
				Die();
			}
			Inc(Count);
		}
		Inc(TotalFlight,FOwner->Speed+15);
		   }break;
	case 4:{      //前からワラワラと
		if( (TotalFlight >= 100*Count) ) {
			if( Count<8 ) 
				FOwner->Add(new TZako03(FOwner,640+32,Y+RandomY[Count]*15,0,-10,0,false));
			else {
				//FOwner->Add(new TZako03(FOwner,640+32,24,0,-20,0,true));
				Die();
			}
			Inc(Count);
		}
		Inc(TotalFlight,FOwner->Speed+15);
		   }break;
	case 5:{      //上下から同時
		if( Age % 20 == 0 ) {
			switch( Count ){
			case 0:
			case 1:
			case 2:{
				FOwner->Add(new TZako03(FOwner,X-48,0-48,3072,-14,1,false));
				FOwner->Add(new TZako03(FOwner,X-48,480,1024,-14,1,false));
				   }break;
			case 3:{
				FOwner->Add(new TZako03(FOwner,X-48,0-48,3072,-14,1,false));
				FOwner->Add(new TZako03(FOwner,X-48,480,1024,-14,1,true));
				   }break;
			}
			if( Count>3 ) Die();
			Inc(Count);
		}
		Inc(TotalFlight,FOwner->Speed);
		Dec(X,FOwner->Speed);
		   }break;
	case 6:{    //上下から出て右へ
		if( Age % 10 == 0 ) {
			switch( Count ){
			case 0:
			case 1:
			case 2:
			case 3:{
				FOwner->Add(new TZako03(FOwner,X-96,0,2048,-8,2,false));
				FOwner->Add(new TZako03(FOwner,X-96,480,2048,-8,3,false));
				   }break;
			case 4:{
				FOwner->Add(new TZako03(FOwner,X-96,0,2048,-8,2,true));
				FOwner->Add(new TZako03(FOwner,X-96,480,2048,-8,3,false));
				   }break;
			}
			if( Count>4 ) Die();
			Inc(Count);
		}
		Inc(TotalFlight,FOwner->Speed);
		Dec(X,FOwner->Speed);
		   }break;
	}

	TOBJ::Move();

}

TZako03::TZako03( TOBJList* owner, s32 _x, s32 _y, s32 _R, s32 _S, s32 _pat, bool _red ) : TOBJ(owner)
{
	FObjectName = "TZako03";

	X = _x;
	Y = _y;
	MaxHP = KSTR_SHOT;
	HP = MaxHP;
	TempHP = HP;
	FSTR = 500;
	FKind = kEnemy;
	Score = 500;
	Buzzed = false;
	LastBuzz = 0;
	Rdeg = _R;
	Speed = _S;
	Pattern = _pat;
	CommandRed = _red;
	GaugeOK = false;
	GaugeTime = 0;
	TotalFlight = 0;
	PrevX = X;
	PrevY = Y;
	ComBonus = 3000;
	ShotNum = 0;

	//IDの付与
	FID = NewID;
	Inc(NewID);

	TSpriteEXBLT sblt;
	sblt.RBM      =  bmNormal; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  FOwner->GetDG();     // TDDDDの参照
	sblt.RHeight  =  48;        // 幅
	sblt.RWidth   =  48;        // 高さ
	sblt.RX       =  X;        // X座標
	sblt.RY       =  Y;        // Y座標
	sblt.RZ       =  11000;       // Z値

	Sprite[0] =  new TJoint(sblt);         // スプライトの生成
	Inc(numSpr,1);
	for( u32 k = 0; k <= 2; k++ )
		Sprite[0]->GetSpr()->AddTexture(MechaTex, 0+k*48+320, 224, 47+k*48+320, 47+224);
	for( u32 k = 0; k <= 1; k++ )
		Sprite[0]->GetSpr()->AddTexture(MechaTex, 95-k*48+320, 224, 48-k*48+320, 47+224);

	Sprite[0]->GetSpr()->FAnim = true;
	Sprite[0]->GetSpr()->FAnimBgnIdx = 0;
	Sprite[0]->GetSpr()->FAnimEndIdx = 4;
	Sprite[0]->GetSpr()->FAnimItv = 3;
	Sprite[0]->GetSpr()->FVisible  =  true;
	Sprite[0]->GetSpr()->AddHitArea(TPoint(0, 0), TPoint(0, 47), TPoint(47, 47),TPoint(47, 0), htSky);
	Sprite[0]->GetSpr()->FHit      =  true;
	Sprite[0]->GetSpr()->FCX += 24;
	Sprite[0]->GetSpr()->FCY += 24;
	if( CommandRed ) {
		Sprite[0]->GetSpr()->SetColor(255, ctRed);
		Sprite[0]->GetSpr()->SetColor(64, ctBlue);
		Sprite[0]->GetSpr()->SetColor(64, ctGreen);
	}

}

void TZako03::Move()
{
	TOBJ::Move();
	Sprite[0]->GetSpr()->SetColor(255,ctRed);          //色を戻す

	X = RoundOff(Sprite[0]->GetSpr()->FCX);
	Y = RoundOff(Sprite[0]->GetSpr()->FCY);

	PrevX = X;
	PrevY = Y;

	switch( Pattern ){
		//Uターン
	case 0:
	case 1:
	case 4:{
		Speed = Speed+0.2f;
		s32 sp = RoundOff(Speed*10.f);
		if( -20 <= sp && sp <= -10 )
			Sprite[0]->GetSpr()->FAnimIdx = 1;
		else if( -5 <= sp && sp <= 5 )
			Sprite[0]->GetSpr()->FAnimIdx = 2;
		else if( 10 <= sp && sp <= 20 )
			Sprite[0]->GetSpr()->FAnimIdx = 3;
		else if( 25 <= sp && sp <= 500 )
			Sprite[0]->GetSpr()->FAnimIdx = 4;

		s32 DX = FOwner->HeroPos.X-X;
		s32 DY = FOwner->HeroPos.Y-Y;
		f32 TargetDis = sqrt(DX*DX+DY*DY);

		if( (Pattern == 0) && (Age == 10) && (TargetDis>120) ) 
			MakeShot();

		if( (Pattern == 4) ) {
			if( (Speed>0) && (X>540) && (ShotNum == 0) && (TargetDis>120) ) 
				MakeShot();
		}

		if( ((Pattern == 1) || (Pattern == 4)) && (FOwner->Difficulty == KDIFFICULTY_HARD) ) {
			if( (Age == 10) && (TargetDis>120) ) 
				MakeShot();
		}

		   }break;
	case 2:{
		//Speed = Speed-0.3;
		Sprite[0]->MoveR(1.0f, Age*0.2f);

		//DX = FOwner->HeroPos.X-X;
		//DY = FOwner->HeroPos.Y-Y;
		// TargetDis = sqrt(DX*DX+DY*DY);
		if( (Age % 30 == 0)) 
			MakeShot();
		   }break;
	case 3:{
		//Speed = Speed-0.3;
		Sprite[0]->MoveR(1.0f, -Age*0.2f);
		if( (Age % 40 == 0) ) 
			MakeShot();
		   }break;
	}

	Rdeg = Rdeg & 0x0fff;
	Sprite[0]->SlideMoveR(Speed,0);              //移動
	//Inc(TotalFlight,RoundOff(Speed));
	Sprite[0]->Rotate(Rdeg);

	X = RoundOff(Sprite[0]->GetSpr()->FCX);
	Y = RoundOff(Sprite[0]->GetSpr()->FCY);


	//軌跡
	f32 PrevDis = pow(pow(X-PrevX,2)+pow(Y-PrevY,2),0.5f);
	s32 trace = 0;
	switch( EffLevel ){
	case 0:trace = RoundOff(PrevDis/8.f );		break;
	case 1:
	case 2:trace = RoundOff(PrevDis/4.f);		break;
	}
	for( u32 i = 0 ; i <= trace ; i++) {
		if( PrevDis>1 )
			FOwner->Add(new TFunLight(FOwner,PrevX+RoundOff((f32)(X-PrevX)/(f32)(trace+1)*(f32)(i+1)),
			PrevY+RoundOff((f32)(Y-PrevY)/(f32)(trace+1)*(f32)(i+1)),
			0.7f,true));
	}


	if( (Age-LastBuzz)>=40 ) Buzzed = false;

	if( (X > 640+64) || (Y > 480+64) || (Y <0-64) ) {
		Die();//画面外に消えたら消す & 母艦にダメージ
		Dec(FOwner->MShipHP,FSTR / 2);
		FOwner->Add(new TMotherShipDamaged(FOwner, X, Y));
	}
	if( HP<=0 ) 
		OnBroken();                        //HPが無くなったら死ぬ

	//ゲージ表示
	//if( (GaugeOK and (FDead=false)) )
	//FOwner->Add(new TEnemyGauge(FOWner,X-40,Y+32,HP,MaxHP));

	if( (Age-GaugeTime)>40 ) GaugeOK = false;

}

void TZako03::MakeShot()
{
	Inc(ShotNum);

	if( FOwner->Difficulty < KDIFFICULTY_NORMAL )  return; 

	if( FOwner->IsSequence() ) {
		s32 DX = RoundOff(FOwner->HeroPos.X-X);             //自機と敵との距離を計算
		s32 DY = RoundOff(FOwner->HeroPos.Y-Y);
		s32 GunR = RoundOff(atan2(DY,DX)/(f32)M_PI*2048) & 0xfff;
		FOwner->Add(new TZ01Shot4(FOwner,X,Y,GunR,4,0,0,500));
	}
	else
		FOwner->Add(new TZ01Shot(FOwner,X,Y));
	FOwner->Add(new TFCircle(FOwner,X,Y,1,Rdeg,RoundOff(Speed)));
	if( CreateSE ) FOwner->SoundEffect->WavPlay(WABShot,9);

}

void TZako03::OnDamaged( TOBJ* target )
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
		if( (target->Hit[HitBlade]) ) {
			s32 DX = (s32)(target->X-X);
			s32 DY = (s32)(target->Y-Y);
			s32 RR = (s32)(atan2(DY,DX)/(f32)M_PI*2048.f) & 0xfff;
			Dec(HP,target->STR());
			Sprite[0]->GetSpr()->SetColor(0,ctRed);           //色を変える
			Sprite[0]->MoveR(-(s32)(cost[RR]*(f32)target->STR()),-(s32)(sint[RR]*(f32)target->STR()));                  //のけぞる
			GaugeOK = true;
			GaugeTime = Age;
		}
	}

	if( target->Kind() == kHShot ) {
		Dec(HP,target->STR());
		Sprite[0]->GetSpr()->SetColor(0,ctRed);           //色を変える
		Sprite[0]->MoveR(target->STR(),0);                  //のけぞる
		GaugeOK = true;
		GaugeTime = Age;
	}

	if( target->Kind() == kMissile ) {
		s32 DX = RoundOff(target->X-X);
		s32 DY = RoundOff(target->Y-Y);
		s32 RR = RoundOff(atan2(DY,DX)/(f32)M_PI*2048.f) & 0xfff;
		Dec(HP,target->STR() );
		Sprite[0]->GetSpr()->SetColor(0,ctRed);           //色を変える
		Sprite[0]->MoveR(-(s32)(cost[RR]*(f32)target->STR()),-(s32)(sint[RR]*(f32)target->STR()));                  //のけぞる
		GaugeOK = true;
		GaugeTime = Age;
		if( HP<=0 ) BonusRate = target->BonusRate;
	}
	if( HP<TempHP ) TempHP = HP;

}

void TZako03::Die()
{
	TOBJ::Die();

}

void TZako03::OnBroken()
{
	TOBJ::OnBroken();
	FOwner->Add(new TExpSmall(FOwner,X,Y,0,0,0));//爆発

}

TZ04Gene::TZ04Gene( TOBJList* owner, s32 _pat, s32 _life ) :  TOBJ(owner)
{
	FObjectName = "TZ04Gene";
	X = 0;
	Y = 0;
	FKind = kGene;
	Pattern = _pat;
	Count = 0;
	TotalFlight = 0;
	Life = _life;
	PosCount = 0;

}

void TZ04Gene::Move()
{
	switch( Pattern ){
	case 0:{
		if( TotalFlight>(100*Count) ) {
			X = FOwner->Z4AppX[PosCount];
			Y = FOwner->Z4AppY[PosCount];
			f32 DX = FOwner->HeroPos.X-X;
			f32 DY = FOwner->HeroPos.Y-Y;
			Inc(PosCount);
			if( PosCount>99 ) PosCount = 0;

			if( sqrt(DX*DX+DY*DY)<240 ) {     //自機のすぐそばには出現させない
				if( FOwner->HeroPos.X>X ) X = (X-200+640) % 640 ;
				else X = (X+200) % 640;
			}

			FOwner->Add(new TZ04Warn(FOwner,X,Y,false));
			Inc(Count,1);
		}
		Inc(TotalFlight,FOwner->Speed);

		if( Count>=Life ) {
			Die();
			//FOwner->Add(new TBoxContrl(FOwner,4));
		}

		   }
	}

	TOBJ::Move();

}

TZ04Warn::TZ04Warn( TOBJList* owner, s32 _x, s32 _y, bool _red ) : TOBJ(owner)
{
	FObjectName = "TZ04Warn";
	X = _x;
	Y = _y;
	FKind = kEffect2;
	TotalFlight = 0;
	ZoomRate = 2.f;
	CommandRed = _red;
	MakeSpriteLite(numSpr, X,Y,128,128,1500,bmNormal);
	Sprite[numSpr-1]->GetSpr()->AddTexture(GaugeTex, 385, 385, 512, 512);
	Sprite[numSpr-1]->GetSpr()->FCX += 64;
	Sprite[numSpr-1]->GetSpr()->FCY += 64;
	Sprite[numSpr-1]->GetSpr()->SetColor(0, ctBlue);
	Sprite[numSpr-1]->GetSpr()->SetColor(0, ctGreen);
	Sprite[numSpr-1]->GetSpr()->Move(X+32, Y+32);
	Sprite[numSpr-1]->GetSpr()->Zoom(ZoomRate, ZoomRate);
}

void TZ04Warn::Move()
{
	if( ZoomRate>1 ) {
		Sprite[0]->GetSpr()->Zoom(ZoomRate,ZoomRate);
		//    Sprite[1]->GetSpr()->Zoom(ZoomRate,ZoomRate);
		ZoomRate = ZoomRate-0.1f;
	}

	Sprite[0]->RotateR(64);


	//  Sprite[1]->Rotate(-RoundOff(2048*TotalFlight/300));

	if( TotalFlight>300 ) {
		if( CommandRed ) FOwner->Add(new TZako04(FOwner,X,Y,2048,-10,0,true));
		else FOwner->Add(new TZako04(FOwner,X,Y,0,0,0,false));
		if( CreateSE ) FOwner->SoundEffect->WavPlay(WAZ4app,11);
		Die();
	}

	Inc(TotalFlight,FOwner->Speed);
	TOBJ::Move();

}

TZako04::TZako04( TOBJList* owner, s32 _x, s32 _y, s32 _R, s32 _S, s32 _pat, bool _red ) : TOBJ(owner)
{
	FObjectName = "TZako04";

	X = _x;
	Y = _y;
	MaxHP = KSTR_SHOT;
	HP = MaxHP;
	TempHP = HP;
	FSTR = 100;
	FKind = kEnemy;
	Score = 1000;
	Buzzed = false;
	LastBuzz = 0;
	Rdeg = _R;
	Speed = _S;
	Pattern = _pat;
	CommandRed = _red;
	GaugeOK = false;
	GaugeTime = 0;
	TotalFlight = 0;
	ActFlg = 0;
	ActCount = 0;
	ComBonus = 1000;

	//IDの付与
	FID = NewID;
	Inc(NewID);


	s32 tempX = X;
	s32 tempY = Y;

	TSpriteEXBLT sblt;
	sblt.RBM      =  bmNormal;    // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  FOwner->GetDG();        // TDDDDの参照
	sblt.RWidth   =  32;        // 幅
	sblt.RHeight  =  32;        // 高さ
	sblt.RX       =  X;         // X座標
	sblt.RY       =  Y;         // Y座標
	sblt.RZ       =  11000;        // Z値

	Sprite[Z4Body] =  new TJoint(sblt);         // Body
	Inc(numSpr,1);

	sblt.RBM      =  bmNormal;    // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  FOwner->GetDG();        // TDDDDの参照
	sblt.RWidth   =  48;        // 幅
	sblt.RHeight  =  16;        // 高さ
	sblt.RX       =  X;         // X座標
	sblt.RY       =  Y;         // Y座標
	sblt.RZ       =  11100;        // Z値

	Sprite[Z4UpperWing] =  new TJoint(sblt);         // upWing
	Inc(numSpr,1);

	sblt.RBM      =  bmNormal;    // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  FOwner->GetDG();        // TDDDDの参照
	sblt.RWidth   =  48;        // 幅
	sblt.RHeight  =  16;        // 高さ
	sblt.RX       =  X;         // X座標
	sblt.RY       =  Y;         // Y座標
	sblt.RZ       =  11100;        // Z値

	Sprite[Z4lowerWing] =  new TJoint(sblt);         // lowWing
	Inc(numSpr,1);

	sblt.RBM      =  bmSubAdd; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  FOwner->GetDG();     // TDDDDの参照
	sblt.RHeight  =  20;        // 幅
	sblt.RWidth   =  20;        // 高さ
	sblt.RX       =  0;        // X座標
	sblt.RY       =  0;        // Y座標
	sblt.RZ       =  4000;       // Z値

	Sprite[Z4Fire] =  new TJoint(sblt);         // backfire
	Inc(numSpr,1);

	Sprite[Z4Body]->GetSpr()->AddTexture(MechaTex, 0+320, 272, 31+320, 31+272);    // テクスチャを登録
	Sprite[Z4Body]->GetSpr()->FVisible  =  true;
	Sprite[Z4Body]->GetSpr()->AddHitArea(TPoint(0, 0), TPoint(0, 32), TPoint(32, 32),TPoint(32, 0), htSky);
	Sprite[Z4Body]->GetSpr()->FHit      =  true;
	Sprite[Z4Body]->GetSpr()->FCX += 11;
	Sprite[Z4Body]->GetSpr()->FCY += 16;
	if( CommandRed ) {
		Sprite[Z4Body]->GetSpr()->SetColor(255, ctRed);
		Sprite[Z4Body]->GetSpr()->SetColor(64, ctBlue);
		Sprite[Z4Body]->GetSpr()->SetColor(64, ctGreen);
	}

	Sprite[Z4UpperWing]->GetSpr()->AddTexture(MechaTex, 32+320, 272, 79+320, 15+272);    // テクスチャを登録
	Sprite[Z4UpperWing]->GetSpr()->FVisible  =  true;
	Sprite[Z4UpperWing]->GetSpr()->FCX += 4;
	Sprite[Z4UpperWing]->GetSpr()->FCY += 11;
	if( CommandRed ) {
		Sprite[Z4UpperWing]->GetSpr()->SetColor(255, ctRed);
		Sprite[Z4UpperWing]->GetSpr()->SetColor(64, ctBlue);
		Sprite[Z4UpperWing]->GetSpr()->SetColor(64, ctGreen);
	}
	Sprite[Z4UpperWing]->GetSpr()->Move(tempX+6,tempY+12);

		Sprite[Z4lowerWing]->GetSpr()->AddTexture(MechaTex, 80+320, 16+272, 127+320, 31+272);    // テクスチャを登録
		Sprite[Z4lowerWing]->GetSpr()->FVisible  =  true;
		Sprite[Z4lowerWing]->GetSpr()->FCX += 4;
		Sprite[Z4lowerWing]->GetSpr()->FCY += 4;
		if( CommandRed ) {
			Sprite[Z4lowerWing]->GetSpr()->SetColor(255, ctRed);
			Sprite[Z4lowerWing]->GetSpr()->SetColor(64, ctBlue);
			Sprite[Z4lowerWing]->GetSpr()->SetColor(64, ctGreen);
		}
		Sprite[Z4lowerWing]->GetSpr()->Move(tempX+6,tempY+21);

		Sprite[Z4Fire]->GetSpr()->AddTexture(Eff1Tex, 0, 192, 64, 256);    // テクスチャを登録
		Sprite[Z4Fire]->GetSpr()->FVisible  =  true;
		Sprite[Z4Fire]->GetSpr()->FCX += 10;
		Sprite[Z4Fire]->GetSpr()->FCY += 10;
		Sprite[Z4Fire]->GetSpr()->SetColor(128, ctRed);
		Sprite[Z4Fire]->GetSpr()->SetColor(128, ctBlue);
		Sprite[Z4Fire]->GetSpr()->SetColor(128, ctGreen);
		Sprite[Z4Fire]->GetSpr()->SetColor(32,ctAlpha);
		Sprite[Z4Fire]->GetSpr()->Zoom(1,1);
		Sprite[Z4Fire]->GetSpr()->Move(X+28,Y+16);

	//間接ドッキング
	Sprite[Z4Body]->Add(Sprite[Z4UpperWing]);
	Sprite[Z4Body]->Add(Sprite[Z4lowerWing]);
	Sprite[Z4Body]->Add(Sprite[Z4Fire]);

	Sprite[Z4Body]->Move(X+32,Y+32);

	Sprite[Z4UpperWing]->RotateR(-400);
	Sprite[Z4lowerWing]->RotateR(400);
	//自機の方向を向く
	f32 DX = FOwner->HeroPos.X-X;
	f32 DY = FOwner->HeroPos.Y-Y;

	if( ! _red ) {
		s32 HeroDeg = RoundOff(atan2(DY,DX)/(f32)M_PI*2048.f)+2048;
		HeroDeg = HeroDeg & 0x0fff;
		Rdeg = HeroDeg;
	}
	Sprite[Z4Body]->Rotate(Rdeg);

	//雲を掻き分けながら出現
	FOwner->Add(new TCloudExp(FOwner,X+64,Y+32,0,0,0));

}

void TZako04::Move()
{
	TOBJ::Move();
	Sprite[Z4Body]->GetSpr()->SetColor(255,ctRed);          //色を戻す
	switch( Pattern ){
		//直進型
		case 0:{
			switch( ActFlg ){
				case 0:{
					if( Age>8 ) ActFlg = 1;
				}break;
				case 1:{
					if( Sprite[Z4UpperWing]->Rdegree != Rdeg ) Sprite[Z4UpperWing]->RotateR(20);
					if( Sprite[Z4lowerWing]->Rdegree != Rdeg ) Sprite[Z4lowerWing]->RotateR(-20);
					if( (CommandRed) ) {
						if( (Age % 4 == 0) ) Dec(Speed);
					}
					else {
						if( (Age % 6 == 0) ) Dec(Speed);
					}
				}break;
			}
		}break;
	}

	Sprite[Z4Body]->SlideMoveR(Speed,0);
	Inc(TotalFlight,abs(Speed));

	X = RoundOff(Sprite[Z4Body]->GetSpr()->FCX);
	Y = RoundOff(Sprite[Z4Body]->GetSpr()->FCY);

	if( (Age-LastBuzz)>=40 ) Buzzed = false;

	if( (X > 640+64) || (Y > 480+64) || (Y <0-64) || (X < 0-64) ) {
		Die();//画面外に消えたら消す & 母艦にダメージ
		if( FOwner->Difficulty=KDIFFICULTY_HARD )
			Dec(FOwner->MShipHP,FSTR * 2);
		else
			Dec(FOwner->MShipHP,FSTR / 10);
		FOwner->Add(new TMotherShipDamaged(FOwner, X, Y));
	}
	if( HP<=0 ) OnBroken();                        //HPが無くなったら死ぬ

	//ゲージ表示
	// if( (GaugeOK and (FDead=false)) )
	// FOwner->Add(new TEnemyGauge(FOWner,X-40,Y+32,HP,MaxHP));

	if( (Age-GaugeTime)>40 ) GaugeOK = false;

	Inc(ActCount);

}

void TZako04::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);
	X = (s32)(Sprite[Z4Body]->GetSpr()->FCX);
	Y = (s32)(Sprite[Z4Body]->GetSpr()->FCY);
	if( target->Kind() == kHero ) {
		if( (! Buzzed) && (target->Hit[HitBuzz]) ) {
			FOwner->Scrach(this);
			Buzzed = true;
			LastBuzz = Age;
		}
		if( (target->Hit[HitBlade]) ) {
			s32 DX = (s32)(target->X-X);
			s32 DY = (s32)(target->Y-Y);
			s32 RR = (s32)(atan2(DY,DX)/(f32)M_PI*2048.f) & 0xfff;
			Dec(HP,target->STR());
			Sprite[0]->GetSpr()->SetColor(0,ctRed);           //色を変える
			Sprite[0]->MoveR(-(s32)(cost[RR]*(f32)target->STR()),-(s32)(sint[RR]*(f32)target->STR()));                  //のけぞる
			GaugeOK = true;
			GaugeTime = Age;
		}
	}

	if( target->Kind() == kHShot ) {
		Dec(HP,target->STR());
		Sprite[Z4Body]->GetSpr()->SetColor(0,ctRed);           //色を変える
		Sprite[Z4Body]->MoveR(target->STR(),0);                  //のけぞる
		GaugeOK = true;
		GaugeTime = Age;
	}

	if( target->Kind() == kMissile ) {
		s32 DX = (s32)(target->X-X);
		s32 DY = (s32)(target->Y-Y);
		s32 RR = (s32)(atan2(DY,DX)/(f32)M_PI*2048.f) && 0xfff;
		Dec(HP,target->STR());
		Sprite[Z4Body]->GetSpr()->SetColor(0,ctRed);           //色を変える
		Sprite[Z4Body]->MoveR(-(s32)(cost[RR]*target->STR()),-(s32)(sint[RR]*target->STR()));                  //のけぞる
		GaugeOK = true;
		GaugeTime = Age;
		if( HP<=0 ) BonusRate = target->BonusRate;
	}
	if( HP<TempHP ) TempHP = HP;

}

void TZako04::Die()
{
	TOBJ::Die();

}

void TZako04::OnBroken()
{
	FOwner->Add(new TExpSmall(FOwner,X,Y,0,0,0));//爆発
}

void TZako05::MissileShot()
{
	if( 0 <= MisCount && MisCount <= 13 )
		Sprite[Z5Hatch]->MoveR(-1.f,-0.7f);
	else if( MisCount == 20 )
	{ FOwner->Add(new TZ05Missile(FOwner,X+95,Y-49,0+UnitNum*5)); FOwner->Add(new TMissileFlash(FOwner,X+95,Y-49));if( CreateSE )  FOwner->SoundEffect->WavPlay(WAMissile,4);}
	else if( MisCount == 30 )
	{ FOwner->Add(new TZ05Missile(FOwner,X+123,Y-43,1+UnitNum*5)); FOwner->Add(new TMissileFlash(FOwner,X+123,Y-43));if( CreateSE )  FOwner->SoundEffect->WavPlay(WAMissile,4);}
	else if( MisCount == 40 )
	{ FOwner->Add(new TZ05Missile(FOwner,X+137,Y-33,2+UnitNum*5));  FOwner->Add(new TMissileFlash(FOwner,X+137,Y-33));if( CreateSE ) FOwner->SoundEffect->WavPlay(WAMissile,4); }
	else if( 75 <= MisCount && MisCount <= 87 )
		Sprite[Z5Hatch]->MoveR(1.f,0.7f);
	else if( MisCount == 88 )
	{
		Sprite[Z5Hatch]->MoveR(1.f,0.7f);
		MisCount = -1;
		MissileON = false;
	}
	Inc(MisCount,1);
}

void TZako05::BeamShot( s32 EnemyDeg )
{
	const s32 RandomTable[] = { 4,2,5,1,3,0 };
	s32 GunR = GunR & 0x0fff;
	s32 GX = X+RoundOff(30.f*cost[GunR]);
	s32 GY = Y+RoundOff(30.f*sint[GunR]);
	s32 ParticleR = random(4095);
		if( 0 <= BeamCount && BeamCount <= 90 ){
			if( (BeamCount == 40) && CreateSE ) FOwner->SoundEffect->WavPlay(WACharge,8);
			if( (GunR>=1328) && (GunR-EnemyDeg>7) ) Inc(GunR,-12);
			else if( (GunR<=2408) && (EnemyDeg-GunR>7) ) Inc(GunR,12);

			FOwner->Add(new TBeamSite(FOwner,GX,GY,GunR,BeamCount));

			if( BeamCount % 2 == 0 ) {
				FOwner->Add(new TBLight(FOwner,GX,GY,(f32)BeamCount*0.06f,2000));
				if( ExtraEffects ) FOwner->Add(new TBallKage(FOwner,GX,GY,0,0,10,0, (f32)BeamCount*0.06f));
				if( BeamCount<70 )
					FOwner->Add(new TParticle2(FOwner,GX+RoundOff(60.f*cost[ParticleR]),GY+RoundOff(60.f*sint[ParticleR]),1,ParticleR,-1,30));
			}
			else {
				FOwner->Add(new TBLight(FOwner,GX,GY,(f32)BeamCount*0.05f,2000));
				if( ExtraEffects ) FOwner->Add(new TBallKage(FOwner,GX,GY,0,0,10,0,(f32)BeamCount*0.05f));
			}
		}
		else if( BeamCount == 90 ){
			FOwner->Add(new TBoxContrl(FOwner,4));
			FOwner->Add(new TSRControl(FOwner,GX,GY,GunR+2048));
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WALBeam,10);
		}
		else if( 92 <= BeamCount && BeamCount <= 100 ){
			if( BeamCount % 3 == 0 )
				FOwner->Add(new TZBeamPart(FOwner,X+RoundOff(80.f*cost[GunR]),Y+RoundOff(80.f*sint[GunR]),GunR,1.2f+(f32)BeamCount*0.009f+(f32)RandomTable[Age % 6]*0.05f,false));
			FOwner->Add(new TBLight(FOwner,GX,GY,4,2000));
			if( ExtraEffects ) FOwner->Add(new TBallKage(FOwner,GX,GY,0,0,10,0,4));
		}
		else if( 101 <= BeamCount && BeamCount <= 140 ){
			if( BeamCount % 3 == 0 )
				FOwner->Add(new TZBeamPart(FOwner,X+RoundOff(80.f*cost[GunR]),Y+RoundOff(80.f*sint[GunR]),GunR,1+(f32)RandomTable[Age % 6]*0.05f,true));
			if( BeamCount % 2 == 0 ) {
				FOwner->Add(new TBLight(FOwner,GX,GY,6.f+(f32)RandomTable[Age % 6]*0.1f,2000));
				if( ExtraEffects ) FOwner->Add(new TBallKage(FOwner,GX,GY,0,0,10,0,6.f+(f32)RandomTable[Age % 6]*0.1f));
			}

		}
		else if( 141 <= BeamCount && BeamCount <= 156 )
		{
			if( BeamCount % 3 == 0 )
				FOwner->Add(new TZBeamPart(FOwner,X+RoundOff(80.f*cost[GunR]),Y+RoundOff(80.f*sint[GunR]),GunR,1.0f-(f32)(BeamCount-141)*0.057f+(f32)RandomTable[Age % 6]*0.02f,false));
			if( BeamCount % 2 == 0 ) {
				FOwner->Add(new TBLight(FOwner,GX,GY,6.f-(f32)(BeamCount-141)*0.4f,2000));
				if( ExtraEffects ) FOwner->Add(new TBallKage(FOwner,GX,GY,0,0,10,0,6.f-(f32)(BeamCount-141)*0.4f));
			}
		}
		else if( BeamCount == 157 ){
			BeamCount = -1;
			BeamON = false;
		}

	Sprite[Z5Gun]->Rotate(GunR-2048);
	Inc(BeamCount,1);

}

void TZako05::Accel()
{
	switch( BoostCount ){
		case 0    : {
			FOwner->Add(new TSRControl(FOwner,X-50,Y,0));
			BSpeed = 0;
		}break;
	}

	if( (X<350) && (BSpeed<8) ) BSpeed = BSpeed+0.1f;

	if( (X>400) && (BSpeed>5) ) BSpeed = BSpeed-0.2f;

	if( (X>300) && (FOwner->Speed == 5) ) {
		BoostCount = -1;
		Boost = false;
	}

	Inc(BoostCount);

}

void TZako05::SerialShot()
{
	switch( ActFlg[1] ){
		case 1:{
			if( 0 <= ActCount[1] && ActCount[1] <= 13 )
				Sprite[Z5Hatch]->MoveR(-1,-0.7);
			else if( 14 == ActCount[1]  ){
				ActCount[1] = -1;
				Inc(ActFlg[1]);
				ShotCount = 0;
			}      
		}break;
		case 2:{
			if( (ActCount[1] % 10 == 0) ) {
				s32 GunX = X+95;
				s32 GunY = Y-49;
				ShotR  =  GetRfromArcTan(GunX,GunY,FOwner->HeroPos.X, FOwner->HeroPos.Y);

				for( u32 i = 0; i <= 7; i++ ) {
					FOwner->Add(new TBoss3Shot(FOwner,GunX,GunY,ShotR + i*512,10,500, BSpeed, 1,1,0.5f));
				}
				FOwner->Add(new TFCircle(FOwner,GunX,GunY,1,0,0));
				if( CreateSE ) FOwner->SoundEffect->WavPlay2(WABShot,9, 150);
				Inc(ShotCount);
			}

			if( ShotCount>=3 ) {
				Inc(ActFlg[1]);
				ActCount[1] = -1;
			}
		}break;
		case 3:{
				if( 0 <= ActCount[1] && ActCount[1] <= 12 )
					Sprite[Z5Hatch]->MoveR(1.f,0.7f);
				else if( 13 == ActCount[1] ){
					Sprite[Z5Hatch]->MoveR(1.f,0.7f);
					ActCount[1] = -1;
					ActFlg[1] = 0;
				}      
			}break;
		}
	Inc(ActCount[1]);

}

void TZako05::KnockBack( s32 _impact )
{
	for( u32 i  =  0 ; i < numSpr; i++ )
		Sprite[i]->GetSpr()->FHit  =  false;

	if( FOwner->HeroPos.Y > Y )
		KnockBackSpeed  =  _impact;
	else
		KnockBackSpeed  =  -1*_impact;

	ActCount[2] = 0;

}

void TZako05::KnockBackAction()
{
	if( ActCount[2] == 4 ) FOwner->Add(new TExpSlashed(FOwner,X+80,Y-32,0,64,0));

	if( ActCount[2] > 2 ) {
		Sprite[Z5Body]->RotateR(KnockBackSpeed);
		Sprite[Z5Body]->MoveR(abs((f32)KnockBackSpeed)/7.f,0);
		KnockBackSpeed  =  (s32)((f32)KnockBackSpeed*0.6f);
		//    Dec(KnockBackSpeed,10);
	}
	Inc(ActCount[2]);

}

TZako05::TZako05( TOBJList* owner, s32 _x, s32 _y, s32 _pat, s32 _num, bool _red ) : TOBJ(owner)
{
	FObjectName = "TZako05";

	X = _x;
	Y = _y;
	MaxHP = KSTR_SHOT*48;
	HP = MaxHP;
	TempHP = HP;
	FSTR = 100;
	FKind = kEnemy;
	Score = 50000;
	Buzzed = false;
	LastBuzz = 0;
	Rdeg = 0;
	XSpeed = 1;
	YSpeed = -3;
	Pattern = _pat;
	CommandRed = _red;
	GaugeTime = 0;
	TotalFlight = 0;
	LastFlash = 0;
	MissileON = false;
	MisCount = 0;
	BeamON = false;
	BeamCount = 0;
	GunR = 2048;
	EnemyDeg = 0;
	Boost = false;
	BoostCount = 0;
	BSpeed = 0;
	UnitNum = _num;
	ShotR = 0;
	ShotCount = 0;
	KnockBackSpeed = 0;
	ComBonus = 5000;

	//IDの付与
	FID = NewID;
	Inc(NewID);

	for( u32 i = 0; i <= 9; i++ ) {
		ActFlg[i] = 0;
		ActCount[i] = 0;
	}


	s32 tempX = X;
	s32 tempY = Y;
	//  _num, TX, TY, TW, TH,  Z, HX, HY, HW, HH,_cx,_cy
	MakeSprite(FOwner->GetDG(), MechaTex, Z5Body,  1, 1 ,463,158, 154,232, 48,160, 60,273, 97,_red);                //84
	Sprite[Z5Body]->GetSpr()->Move(tempX+273,tempY+97);

	MakeSprite(FOwner->GetDG(), MechaTex, Z5Wing,  1,161,207, 62, 150,  0,  0,  0,  0, 88, 16,_red);                //80
	Sprite[Z5Wing]->GetSpr()->Move(tempX+294,tempY+75);

	MakeSprite(FOwner->GetDG(), MechaTex,Z5Hatch,435,161, 79, 31, 152,  0,  0,  0,  0, 28, 13,_red);                //82
	Sprite[Z5Hatch]->GetSpr()->Move(tempX+368,tempY+51);

	MakeSprite(FOwner->GetDG(), MechaTex,  Z5Gun,241,161,191, 62, 152,  0,  0,  0,  0,165, 33,_red);                //82
	Sprite[Z5Gun]->GetSpr()->Move(tempX+273,tempY+97);

	//間接ドッキング
	Sprite[Z5Body]->Add(Sprite[Z5Wing]);
	Sprite[Z5Body]->Add(Sprite[Z5Hatch]);
	Sprite[Z5Body]->Add(Sprite[Z5Gun]);

	if( ExtraEffects ) {
		//反射
		MakeSprite(FOwner->GetDG(), MechaTex, Z5BodyR,  1, 1 ,463,158,153,  0,  0,  0,  0,273, 97,false);
		Sprite[Z5BodyR]->GetSpr()->Move(tempX+273,tempY+97);

		MakeSprite(FOwner->GetDG(), MechaTex, Z5WingR,  1,161,207, 62,149,  0,  0,  0,  0, 88, 16,false);
		Sprite[Z5WingR]->GetSpr()->Move(tempX+294,tempY+75);

		MakeSprite(FOwner->GetDG(), MechaTex,Z5HatchR,435,161, 79, 31,151,  0,  0,  0,  0, 28, 13,false);
		Sprite[Z5HatchR]->GetSpr()->Move(tempX+368,tempY+51);

		MakeSprite(FOwner->GetDG(), MechaTex,  Z5GunR,241,161,191, 62,151,  0,  0,  0,  0,165, 33,false);
		Sprite[Z5GunR]->GetSpr()->Move(tempX+273,tempY+97);

		//OriginColor = 224;
		OriginColor = 255;
		for( u32 i = 0; i <= 3; i++ ) {
			if( _red ) {
				Sprite[i]->GetSpr()->SetColor(255,ctRed);
				Sprite[i]->GetSpr()->SetColor(0,ctBlue);
				Sprite[i]->GetSpr()->SetColor(0,ctGreen);
			}
			else {
				Sprite[i]->GetSpr()->SetColor(OriginColor,ctRed);
				Sprite[i]->GetSpr()->SetColor(OriginColor,ctBlue);
				Sprite[i]->GetSpr()->SetColor(OriginColor,ctGreen);
			}
		}

		for( u32 i = 4; i <= 7; i++ ) {
			Sprite[i]->GetSpr()->SetColor(80,ctAlpha);
			Sprite[i]->GetSpr()->FEffectNum = 1;
			Sprite[i]->GetSpr()->FBumpSize = 2.9f;
			Sprite[i]->GetSpr()->FBumpTex = MechaTex;
			Sprite[i]->GetSpr()->FBumpLevel = 2.0f;

			Sprite[i]->GetSpr()->FVisible = true;
		}

		Sprite[Z5Body]->Add(Sprite[Z5BodyR]);
		Sprite[Z5Wing]->Add(Sprite[Z5WingR]);
		Sprite[Z5Gun]->Add(Sprite[Z5GunR]);
		Sprite[Z5Hatch]->Add(Sprite[Z5HatchR]);
	}

	Sprite[Z5Body]->Move(X,Y);

	HPGauge  =  new TEnemyGauge2(FOwner,this,-40,32);
	FOwner->Add(HPGauge);

}

void TZako05::Move()
{
	TOBJ::Move();
	switch( Pattern ){
		case 0:{  //下から
			switch( ActFlg[0] ){
				case 0:{
					XSpeed = 2;
					if( Y>400 ) Sprite[Z5Body]->MoveR(2,-3);
					if( Y<=400 ) {
						Sprite[Z5Body]->MoveR(2*cost[ActCount[0]*8]+BSpeed,YSpeed*sint[1024+ActCount[0]*8]);
						Inc(ActCount[0]);
					}

					if( (ActCount[0] == 1) && CreateSE ) FOwner->SoundEffect->WavPlay(WAZ5app,5);

					if( (1024+ActCount[0]*8>=2048) ) {
						ActFlg[0] = 1;
						ActCount[0] = 0;
					}
					if( (FOwner->Speed>9) && (X<200) ) Boost = true;
				}break;
				case 1:{
					Sprite[Z5Body]->MoveR(0.5f*cost[1024+ActCount[0]*16]+BSpeed,-0.5f*sint[2048+ActCount[0]*16]);
					Inc(ActCount[0]);
					if( (2048+ActCount[0]*16>=4096) ) {
						ActFlg[0] = 2;
						ActCount[0] = 0;
					}
					if( TotalFlight>4500 ) {
						ActFlg[0] = 3;
						ActCount[0] = 0;
					}
					if( (X<300) ) Boost = true;
				}break;
				case 2:{
					Sprite[Z5Body]->MoveR(-0.5f*cost[1024+ActCount[0]*16]+BSpeed,0.5f*sint[2048+ActCount[0]*16]);
					Inc(ActCount[0]);

					if( (2048+ActCount[0]*16>=4096) ) {
						ActFlg[0] = 1;
						ActCount[0] = 0;
					}
					if( (X<300) ) Boost = true;
					if( TotalFlight>4500 ) {
						ActFlg[0] = 3;
						ActCount[0] = 0;
					}
					//Sprite[Z5Body]->Move(400,(s32)(DData.FCURRENT[4]));
				}break;
				case 3:{
					if( (X<300) ) Boost = true;
					if( ActCount[0]<1024 ) {
						Sprite[Z5Body]->MoveR(BSpeed,-YSpeed*sint[ActCount[0]*4]);
						Inc(ActCount[0]);
					}
					else Sprite[Z5Body]->MoveR(BSpeed,-YSpeed);
				}break;
			}
		}break;
		case 1:{    //上から
			switch( ActFlg[0] ){
				case 0:{
					XSpeed = 2;
					if( Y<100 ) Sprite[Z5Body]->MoveR(3,4);
					if( Y>=100 ) {
						Sprite[Z5Body]->MoveR(3.f*cost[ActCount[0]*8]+BSpeed,-YSpeed*sint[1024+ActCount[0]*8]);
						Inc(ActCount[0]);
					}

					if( (ActCount[0] == 1) && CreateSE ) FOwner->SoundEffect->WavPlay(WAZ5app,5);


					if( (1024+ActCount[0]*8>=2048) ) {
						ActFlg[0] = 1;
						ActCount[0] = 0;
					}
					if( (FOwner->Speed>9) && (X<200) ) Boost = true;
				}break;
				case 1:{
					Sprite[Z5Body]->MoveR(0.5f*cost[1024+ActCount[0]*16]+BSpeed,0.5f*sint[2048+ActCount[0]*16]);
					Inc(ActCount[0]);
					if( (2048+ActCount[0]*16>=4096) ) {
						ActFlg[0] = 2;
						ActCount[0] = 0;
					}
					if( TotalFlight>4500 ) {
						ActFlg[0] = 3;
						ActCount[0] = 0;
					}
					if( (X<300) ) Boost = true;
				}break;
				case 2:{
					Sprite[Z5Body]->MoveR(-0.5f*cost[1024+ActCount[0]*16]+BSpeed,-0.5f*sint[2048+ActCount[0]*16]);
					Inc(ActCount[0]);
					if( (2048+ActCount[0]*16>=4096) ) {
						ActFlg[0] = 1;
						ActCount[0] = 0;
					}
					if( (X<300) ) Boost = true;
					if( TotalFlight>4500 ) {
						ActFlg[0] = 3;
						ActCount[0] = 0;
					}
				}break;
				case 3:{
					if( (X<300) ) Boost = true;
					if( ActCount[0]<1024 ) {
						Sprite[Z5Body]->MoveR(BSpeed,-YSpeed*sint[ActCount[0]*4]);
						Inc(ActCount[0]);
					}
					else Sprite[Z5Body]->MoveR(BSpeed,-YSpeed);
				}break;
			}
		}
	}

	X = RoundOff(Sprite[Z5Body]->GetSpr()->FCX);
	Y = RoundOff(Sprite[Z5Body]->GetSpr()->FCY);

	if( (! Boost) ) {
		if( (BSpeed>0) || (X>550) ) BSpeed = BSpeed-0.2f;
		if( (X<500) ) BSpeed = BSpeed+0.1f;
	}

	if( FOwner->Difficulty=KDIFFICULTY_HARD )
		if( (Age==128) || ((Age>128) && ((Age-128) % 200==0)) && (ActFlg[1]==0)) ActFlg[1] = 1;

	//タイマーにあわせてミサイルを撃つ
	//  if( ! Debugmode ) {
	if( (UnitNum>-1) && (! FOwner->IsSequence()) )
		if( (Age == 128) || ((Age > 128) && ((Age-128) % 200 == 0)) ) MissileON = true;

	//タイマーにあわせてビームを撃つ
	if( ((Age == 180) || ((Age>180) && ((Age-180) % 180 == 0))) && (Y<450) ) {
		s32 AX = X-RoundOff(30.f*cost[GunR & 0xfff]);
		s32 AY = Y-RoundOff(30.f*sint[GunR & 0xfff]);

		s32 DX = FOwner->HeroPos.X-AX;             //自分と敵との距離を計算
		s32 DY = FOwner->HeroPos.Y-AY;

		EnemyDeg = RoundOff(atan2(DY,DX)/(f32)M_PI*2048.f);
		EnemyDeg = EnemyDeg & 0x0fff;
		BeamON = true;
	}
	//  }
	//タイマーにあわせてショットを打つ
	//if( (Age % 180 = 0) and (Y<450) ) ActFlg[1] = 1;

	if( HP>0 ) {
		if( Boost ) Accel();
		if( MissileON ) MissileShot();
		if( BeamON ) BeamShot(EnemyDeg);
		if( ActFlg[1] > 0 ) SerialShot();
	}
	else
		KnockBackAction();

	//雲を掻き分ける
	if( (Y>420) && (Age % 8 == 0) && (FOwner->Distance<20800) ) FOwner->Add(new TCloudExp(FOwner,X+200,Y-64,1024,0,1));

	if( (Age-LastBuzz)>=40 ) Buzzed = false;

	if( (Y > 480+200) || (Y <0-200) ) {
		Die();//画面外に消えたら消す & 母艦にダメージ
		Dec(FOwner->MShipHP, FSTR*10);
		FOwner->Add(new TMotherShipDamaged(FOwner, X, Y));
	}
	if( (HP<=0) && (KnockBackSpeed == 0) ) OnBroken();                        //HPが無くなったら死ぬ

	if( (Age-GaugeTime)>120 ) HPGauge->Hide();

	if( (Age-LastFlash == 2) ) {
		if( CommandRed || (! ExtraEffects) ) {
			Sprite[Z5Body]->GetSpr()->SetColor(255,ctRed);          //色を戻す
			Sprite[Z5Wing]->GetSpr()->SetColor(255,ctRed);
		}
		else {
			Sprite[Z5Body]->GetSpr()->SetColor(OriginColor,ctRed);          //色を戻す
			Sprite[Z5Wing]->GetSpr()->SetColor(OriginColor,ctRed);
		}
		Sprite[Z5Body]->MoveR(-3,0);                  //のけぞり戻し
	}

	Inc(TotalFlight,FOwner->Speed);

}

void TZako05::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);
	X = RoundOff(Sprite[Z5Body]->GetSpr()->FCX);
	Y = RoundOff(Sprite[Z5Body]->GetSpr()->FCY);
	if( target->Kind() == kHero ) {
		if( (! Buzzed) && (target->Hit[HitBuzz]) ) {
			FOwner->Scrach(this);
			Buzzed = true;
			LastBuzz = Age;
		}
		if( (target->Hit[HitBlade]) && (target->LastSlashed > LastSlashed) ) {
			Dec(HP,target->STR() );
			Sprite[Z5Body]->GetSpr()->SetColor(0,ctRed);           //色を変える
			Sprite[Z5Wing]->GetSpr()->SetColor(0,ctRed);           //色を変える
			LastFlash = Age;
			Sprite[Z5Body]->MoveR(3,0);                  //のけぞる
			HPGauge->Show();
			GaugeTime = Age;
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WAEdamage0,7);
			LastSlashed = target->LastSlashed;
			if( HP <= 0 ) KnockBack(target->STR()*4);
		}
	}

	if( (target->Kind() == kHShot) || (target->Kind() == kMissile) ) {
		Dec(HP,target->STR());
		Sprite[Z5Body]->GetSpr()->SetColor(0,ctRed);           //色を変える
		Sprite[Z5Wing]->GetSpr()->SetColor(0,ctRed);           //色を変える
		LastFlash = Age;
		Sprite[Z5Body]->MoveR(3,0);                  //のけぞる
		HPGauge->Show();
		GaugeTime = Age;
		if( CreateSE ) FOwner->SoundEffect->WavPlay(WAEdamage0,7);
		if( HP<=0 ) BonusRate = target->BonusRate;
	}

	if( HP<TempHP ) TempHP = HP;

}

void TZako05::Die()
{
	HPGauge->Die();
	TOBJ::Die();

}

void TZako05::OnBroken()
{
	TOBJ::OnBroken();
	FOwner->Add(new TExplarge(FOwner,X,Y,0,true));//爆発
}

TZ05Missile::TZ05Missile( TOBJList* owner, s32 sx, s32 sy, s32 _misNum ) : TOBJ(owner)
{
	const s32 ShotAngle[] = { -100,25,-75,75,-25,100,-50,0 };
	const s32 RandomY[] = { 4,-3,2,-1,0,-2,3,-4 };
	FObjectName = "TZ05Missile";

	X = sx;
	Y = sy;
	Buzzed = false;
	LastBuzz = 0;
	FSTR = 300;
	FKind = kEMissile;
	//YMove = random(4)-2;
	YMove = RandomY[_misNum];
	//Rdeg = 3*(Random(200)-100);
	Rdeg = 5*(ShotAngle[_misNum]);

	TSpriteEXBLT sblt;
	sblt.RBM      =  bmNormal; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  FOwner->GetDG();     // TDDDDの参照
	sblt.RHeight  =  32;        // 幅
	sblt.RWidth   =  32;        // 高さ
	sblt.RX       =  X;        // X座標
	sblt.RY       =  Y;        // Y座標
	sblt.RZ       =  5000;       // Z値

	Sprite[0] =  new TJoint(sblt);         // スプライトの生成
	Inc(numSpr,1);
	Sprite[0]->GetSpr()->AddTexture(JikiTex, 144, 0, 144+31, 31);    // テクスチャを登録
	Sprite[0]->GetSpr()->FVisible  =  true;
	Sprite[0]->GetSpr()->AddHitArea(TPoint(0, 8), TPoint(0, 24), TPoint(31, 24),TPoint(31, 8), htSky);
	Sprite[0]->GetSpr()->FHit        =  true;
	Sprite[0]->GetSpr()->FCX += 16;
	Sprite[0]->GetSpr()->FCY += 16;
	Sprite[0]->GetSpr()->SetColor(160,ctRed);
	Sprite[0]->GetSpr()->SetColor(160,ctGreen);
	Sprite[0]->GetSpr()->SetColor(160,ctBlue);

	sblt.RBM      =  bmSubAdd; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  FOwner->GetDG();     // TDDDDの参照
	sblt.RHeight  =  20;        // 幅
	sblt.RWidth   =  20;        // 高さ
	sblt.RX       =  0;        // X座標
	sblt.RY       =  0;        // Y座標
	sblt.RZ       =  4000;       // Z値

	Sprite[1] =  new TJoint(sblt);         // backfire
	Inc(numSpr,1);

	Sprite[1]->GetSpr()->AddTexture(Eff1Tex, 0, 192, 64, 256);
	Sprite[1]->GetSpr()->FVisible  =  true;
	Sprite[1]->GetSpr()->FCX += 10;
	Sprite[1]->GetSpr()->FCY += 10;
	Sprite[1]->GetSpr()->SetColor(128, ctRed);
	Sprite[1]->GetSpr()->SetColor(128, ctBlue);
	Sprite[1]->GetSpr()->SetColor(128, ctGreen);
	Sprite[1]->GetSpr()->SetColor(32,ctAlpha);
	Sprite[1]->GetSpr()->Zoom(1.2f,0.8f);
	Sprite[1]->GetSpr()->Move(X,Y+16);

	Sprite[0]->Add(Sprite[1]);

	Sprite[0]->Rotate(Rdeg);

}

void TZ05Missile::Move()
{
	s32 Steering;
	s32 SlideSpeed;
	Rdeg  =  Rdeg & 0x0fff;
	PrevR = Rdeg;
	if( FOwner->Difficulty == KDIFFICULTY_SINGLE ) {
		if( Age<20 ) Steering = 5;
		else Steering = Age*3-55;

		SlideSpeed = 5+Age / 5;
	}
	else {
		if( Age<20 ) Steering = 10;
		else Steering = Age - 10;

		SlideSpeed = 5+Age / 5;
	}

	s32 DX = FOwner->HeroPos.X-X;             //自分と敵との距離を計算
	s32 DY = FOwner->HeroPos.Y-Y;

	s32 EnemyDeg = RoundOff(atan2(DY,DX)/(f32)M_PI*2048.f);
	EnemyDeg  =  EnemyDeg & 0x0fff;

	if( abs(Rdeg-EnemyDeg)>=2048 ) Inc(Rdeg,Sign(Rdeg-EnemyDeg)*RoundOff((f32)(4095-abs(Rdeg-EnemyDeg))/(f32)Steering));
	else if( abs(Rdeg-EnemyDeg)<2048 ) Dec(Rdeg,RoundOff((f32)(Rdeg-EnemyDeg)/(f32)Steering));

	Sprite[0]->Rotate(Rdeg);
	Sprite[0]->SlideMoveR(SlideSpeed,0);

	X = RoundOff(Sprite[0]->GetSpr()->FCX);
	Y = RoundOff(Sprite[0]->GetSpr()->FCY);

	if( Age == 0 ) {
		PrevSmoke  =  new TSmoke(FOwner,X,Y,Rdeg,NULL);
		FOwner->Add(PrevSmoke);
	}


	//煙と煙の間を煙で埋める
	s32 trace = 1;
	Inc(PrevSmoke->Y,FOwner->BGSlide.Y);    //前の煙の座標に、背景スクロール移動量を考慮
	Inc(PrevSmoke->X,RoundOff(-8*cost[Rdeg & 0x0fff]));
	Inc(PrevSmoke->Y,RoundOff(-8*sint[Rdeg & 0x0fff]));
	switch( EffLevel ){
	case 0:
	case 1:trace = 1; 
		break;
	case 2:{
			trace = RoundOff(pow(pow(X-PrevSmoke->X,2)+pow(Y-PrevSmoke->Y,2),0.5f)/16.f);
			if( abs(GetDiffR(Rdeg,PrevR))>150 ) Inc(trace,1);
		}break;
	}
	//trace = 2;
	if( trace > 0 ) {
		s32 DivX = RoundOff((X-PrevSmoke->X)/trace);
		s32 DivY = RoundOff((Y-PrevSmoke->Y)/trace);

		//煙
		for( u32 i = 0 ; i < trace; i++ ) {
			TSmoke* tmpObj  =  new TSmoke(FOwner,PrevSmoke->X+DivX,
				PrevSmoke->Y+DivY,
				PrevR+(GetDiffR(PrevR,Rdeg) / trace)*(i+1),
				PrevSmoke);

			PrevSmoke->NextSmoke  =  tmpObj;
			PrevSmoke  =  tmpObj;
			FOwner->Add(tmpObj);
		}
	}

	if( ((X > 710) || (X <-70) || (Y > 480+70) || (Y <0-70)) ) Die();//弾が画面外に消えたら消す
	TOBJ::Move();

}

void TZ05Missile::Die()
{
	TOBJ::Die();
	if( PrevSmoke!=NULL )
		PrevSmoke->TellMomDead();

}

void TZ05Missile::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);
	if( target->Kind() == kHero ) {
		if( (! Buzzed) && (target->Hit[HitBuzz]) ) {
			FOwner->Scrach(this);
			Buzzed = true;
			LastBuzz = Age;
		}
		else if( (target->Hit[HitBody]) ) {
			FOwner->Add(new TExpSmall(FOwner,X,Y,Rdeg,0,0));//爆発
			Die();
		}
		if( (target->Hit[HitBlade]) ) {
			FOwner->Add(new TExpSmall(FOwner,X,Y,Rdeg,0,0));//爆発
			Die();
		}
	}
	if( target->Kind() == kHShot ) {
		FOwner->Add(new TExpSmall(FOwner,X,Y,Rdeg,0,0));//爆発
		Die();
	}

}

TZBeamPart::TZBeamPart( TOBJList* owner, s32 sx, s32 sy, s32 R, f32 _zoom, bool _hit ) : TOBJ(owner)
{
	FObjectName = "TZBeamPart";
	X = sx;
	Y = sy;
	FSTR = 1000;
	FKind = kEShot;
	Buzzed = false;
	LastBuzz = 0;
	Rdeg = R;

	TSpriteEXBLT sblt;
	sblt.RBM      =  bmSubAdd; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  FOwner->GetDG();     // TDDDDの参照
	sblt.RHeight  =  64;        // 幅
	sblt.RWidth   =  64;        // 高さ
	sblt.RX       =  X;        // X座標
	sblt.RY       =  Y;        // Y座標
	sblt.RZ       =  1100;       // Z値

	Sprite[0] =  new TJoint(sblt);         // スプライトの生成
	Inc(numSpr,1);
		Sprite[0]->GetSpr()->AddTexture(Eff2Tex,0, 128, 63, 191);    // テクスチャを登録
		Sprite[0]->GetSpr()->FVisible  =  true;
		Sprite[0]->GetSpr()->AddHitArea(TPoint(0, 20), TPoint(0, 44), TPoint(63, 44),TPoint(63, 20), htSky);
		Sprite[0]->GetSpr()->FHit      =  _hit;
		Sprite[0]->GetSpr()->FCX += 32;
		Sprite[0]->GetSpr()->FCY += 32;
		Sprite[0]->GetSpr()->Zoom(4,_zoom);
		Sprite[0]->GetSpr()->SetColor(150,ctGreen);
		Sprite[0]->GetSpr()->SetColor(150,ctBlue);
		Sprite[0]->GetSpr()->SetColor(100,ctAlpha);

	Sprite[0]->Move(X,Y); //弾の初期位置を設定
	Sprite[0]->Rotate(Rdeg);

}

void TZBeamPart::Move()
{
	TOBJ::Move();
	X = RoundOff(Sprite[0]->GetSpr()->FCX);
	Y = RoundOff(Sprite[0]->GetSpr()->FCY);

	Sprite[0]->SlideMoveR(30,0); //弾の移動

	if( (Age-LastBuzz)>=40 ) Buzzed = false;

	if( (X > 640+70) || (Y > 480+70) || (Y <0-70) || (X < 0-70) ) Die();//弾が画面外に消えたら消す

}

void TZBeamPart::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);
	if( target->Kind() == kHero ) {
		if( (! Buzzed) && (target->Hit[HitBuzz]) ) {
			FOwner->Scrach(this);
			Buzzed = true;
			LastBuzz = Age;
		}
	}

}

TFunBeamPart::TFunBeamPart( TOBJList* owner, s32 sx, s32 sy, s32 R, s32 S, s32 L, f32 _zoom ) : TOBJ(owner)
{
	FObjectName = "TFunBeamPart";
	X = sx;
	Y = sy;
	FKind = kEffect;
	Rdeg = R;
	Speed = S;
	Life = L;

	TSpriteEXBLT sblt;
	sblt.RBM      =  bmSubAdd; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  FOwner->GetDG();     // TDDDDの参照
	sblt.RHeight  =  10;        // 高さ
	sblt.RWidth   =  20;        // 幅
	sblt.RX       =  X;        // X座標
	sblt.RY       =  Y;        // Y座標
	sblt.RZ       =  BeamZ;       // Z値

	Inc(BeamZ);
	if( BeamZ>4900 ) BeamZ = 4000;

	Sprite[0] =  new TJoint(sblt);         // スプライトの生成
	Inc(numSpr,1);
	if( FOwner->NowStage == 1 ) {
		Sprite[0]->GetSpr()->AddTexture(Eff2Tex,0, 192, 63, 255);
		Sprite[0]->GetSpr()->SetColor(48, ctRed);
		Sprite[0]->GetSpr()->SetColor(128,ctGreen);
		Sprite[0]->GetSpr()->SetColor(64,ctBlue);
		Sprite[0]->GetSpr()->SetColor(128,ctAlpha);
	}
	else {
		Sprite[0]->GetSpr()->AddTexture(Eff2Tex,0, 128, 63, 191);    // テクスチャを登録
		Sprite[0]->GetSpr()->SetColor(128, ctRed);
		Sprite[0]->GetSpr()->SetColor(75,ctGreen);
		Sprite[0]->GetSpr()->SetColor(75,ctBlue);
		Sprite[0]->GetSpr()->SetColor(75,ctAlpha);
	}
	Sprite[0]->GetSpr()->FVisible  =  true;
	Sprite[0]->GetSpr()->FCX += 20;
	Sprite[0]->GetSpr()->FCY += 5;
	Sprite[0]->GetSpr()->Zoom(_zoom*2.f,_zoom);
	Sprite[0]->GetSpr()->Move(X,Y); //弾の初期位置を設定
	Sprite[0]->GetSpr()->Rotate(Rdeg);
}

void TFunBeamPart::Move()
{
	TOBJ::Move();
	X = RoundOff(Sprite[0]->GetSpr()->FCX);
	Y = RoundOff(Sprite[0]->GetSpr()->FCY);

	Sprite[0]->SlideMoveR(Speed,0); //弾の移動

	Sprite[0]->GetSpr()->Rotate(0);
	Sprite[0]->GetSpr()->ZoomR(0.99f,0.96f);
	Sprite[0]->GetSpr()->Rotate(Rdeg);
	if( FOwner->NowStage == 1 ) {
		Sprite[0]->GetSpr()->SetColorR(-4,ctAlpha);
	}
	else {
		Sprite[0]->GetSpr()->SetColor(90-Life, ctRed);
		Sprite[0]->GetSpr()->SetColor(60-Life,ctGreen);
		Sprite[0]->GetSpr()->SetColor(60-Life,ctBlue);
		Sprite[0]->GetSpr()->SetColor(50-Life,ctAlpha);
	}

	if( Age>Life ) Die();


	if( (X > 640+70) || (Y > 480+70) || (Y <0-70) || (X < 0-70) ) Die();//画面外に消えたら消す

}

TFunBeamCore::TFunBeamCore( TOBJList* owner, s32 sx, s32 sy, s32 R, s32 _pat ) : TOBJ(owner)
{
	FObjectName = "TFunBeamCore";
	X = sx;
	Y = sy;
	FSTR = 500;
	FKind = kEShot;
	Buzzed = false;
	LastBuzz = 0;
	Pattern = _pat;
	Rdeg = R & 0x0fff;

	TSpriteEXBLT sblt;
	sblt.RBM      =  bmNone; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  FOwner->GetDG();     // TDDDDの参照
	sblt.RHeight  =  16;        // 高さ
	sblt.RWidth   =  64;        // 幅
	sblt.RX       =  X;        // X座標
	sblt.RY       =  Y;        // Y座標
	sblt.RZ       =  5000;       // Z値

	Sprite[0] =  new TJoint(sblt);         // スプライトの生成
	Inc(numSpr,1);

	Sprite[0]->GetSpr()->AddTexture(Eff2Tex,0, 128, 63, 191);    // テクスチャを登録
	Sprite[0]->GetSpr()->FVisible  =  false;
	Sprite[0]->GetSpr()->AddHitArea(TPoint(0, 2), TPoint(0, 14), TPoint(64, 14), TPoint(64, 2), htSky);
	if( Pattern!=3 ) Sprite[0]->GetSpr()->FHit      =  true;
	Sprite[0]->GetSpr()->FCX += 64;
	Sprite[0]->GetSpr()->FCY += 8;
	Sprite[0]->Move(X,Y); //弾の初期位置を設定
	Sprite[0]->Rotate(Rdeg);

}

void TFunBeamCore::Move()
{
	s32 PatInterval = 2;
	s32 N_particle = 0;

	TOBJ::Move();

	switch( EffLevel ){
		case 0:N_particle = 5;	break;
		case 1:N_particle = 8;	break;
		case 2:N_particle = 10;	break;
	}

	switch( Pattern ){
		case 0:{ Sprite[0]->SlideMoveR(20+FOwner->Speed,0); PatInterval = 2; }		break;
		case 1:{ Sprite[0]->SlideMoveR(7+FOwner->Speed,0);  PatInterval = 3;  }		break;
		case 2:{ Sprite[0]->SlideMoveR(12+FOwner->Speed,0); PatInterval = 2; }		break;
		case 3:{ Sprite[0]->SlideMoveR(30+FOwner->Speed,0); PatInterval = 2; }		break;
		case 4:{ Sprite[0]->SlideMoveR(5+FOwner->GetRand(5),0); PatInterval = 3; }		break;
	}

	X = RoundOff(Sprite[0]->GetSpr()->FCX);
	Y = RoundOff(Sprite[0]->GetSpr()->FCY);
	if( Age % PatInterval == 0 ) {
		for( u32 i = 0 ; i <= N_particle ; i++) {
			s32 PartsX = random(64)+i;
			//PartsY = random(20-i)-(20-i) / 2;
			s32 PartsY = 0;
			FOwner->Add(new TFunBeamPart(FOwner,X+RoundOff((f32)PartsX*cost[Rdeg]+(f32)PartsY*sint[Rdeg]),
				Y+RoundOff((f32)PartsY*cost[Rdeg]+(f32)PartsX*sint[Rdeg]),
				Rdeg,
				random(5)+1,
				5+i*2,
				2.3f-(f32)i*0.05f));

		}
	}

	if( (Age-LastBuzz)>=40 ) Buzzed = false;

	if( (Y > 440) && (FOwner->NowStage == 1) ) {
		FOwner->Add(new TSplash(FOwner, X,5));
		Die();
	}

	if( (X > 640+70) || (Y > 480+70) || (Y <0-70) || (X < 0-70) ) {
		Die();//弾が画面外に消えたら消す
	}
}

void TFunBeamCore::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);
	if( target->Kind() == kHero ) {
		if( (! Buzzed) && (target->Hit[HitBuzz]) ) {
			FOwner->Scrach(this);
			Buzzed = true;
			LastBuzz = Age;
		}
	}

}

TZakoHomingCore::TZakoHomingCore( TOBJList* owner, s32 sx, s32 sy, s32 dx, s32 dy, s32 _r, s32 _level ) : TOBJ(owner)
{
	FObjectName = "TZakoHomingCore";

	X = sx;
	Y = sy;
	Buzzed = false;
	LastBuzz = 0;
	FKind = kEShot;
	Rdeg = _r & 0x0fff;
	HomingLevel  =  _level;
	DestX = dx;
	DestY = dy;
	ArriveCount = 0;
	//本体
	MakeSpriteLite(0,X,Y,28,28,5000,bmAdd);
	Sprite[0]->GetSpr()->AddTexture(Eff8Tex, 129, 1, 255, 127);
	Sprite[0]->GetSpr()->FVisible = false;

	//根本の光
	MakeSpriteLite(1,X,Y,64,64,12000,bmAdd);
		Sprite[1]->GetSpr()->AddTexture(Eff8Tex, 192, 192, 255, 255);
		Sprite[1]->GetSpr()->FCX += 32;
		Sprite[1]->GetSpr()->FCY += 32;
		Sprite[1]->GetSpr()->SetColor(128,ctAlpha);
		Sprite[1]->GetSpr()->Move(X,Y);

	Trace  =  new THomingTrace(FOwner, Sprite[0], HomingLevel*8 + 4, HomingLevel*16);
	FOwner->Add(Trace);

	Sprite[0]->Rotate(Rdeg);

	Marker = new TEnemyLockMarker(FOwner, DestX, DestY, HomingLevel*0.33f);
	FOwner->Add(Marker);

}

TZakoHomingCore::~TZakoHomingCore()
{

}

void TZakoHomingCore::Move()
{
	Rdeg  =  Rdeg & 0x0fff;
	PrevR = Rdeg;

	//  if( Age<20 ) Steering = 5 * HomingLevel
	//  else Steering = (Age*3-55)*HomingLevel;

	s32 Steering =  20;
	if( Age>20 ) Steering =  2;


	s32 DX = DestX-X;             //自分と敵との距離を計算
	s32 DY = DestY-Y;

	s32 DestDistance = RoundOff(pow(DX*DX+DY*DY, 0.5f));

	if( (DestDistance>48) || (Age<30) ) {

		s32 EnemyDeg = RoundOff(atan2(DY,DX)/(f32)M_PI*2048.f) & 0x0fff;

		s32 Rdiff = GetDiffR( EnemyDeg, Rdeg);

		if( abs(Rdiff)>=2048 ) Inc(Rdeg, (s32)(-Sign(Rdiff)*ceil((f32)(4095-abs(Rdiff))/(f32)Steering)-Sign(Rdiff)));
		else if( abs(Rdiff)<2048 ) Dec(Rdeg, (s32)ceil((f32)(Rdiff)/(f32)Steering)+Sign(Rdiff));

		s32 MinSpeed = 2;
		s32 MaxSpeed = 20+HomingLevel*10;
		s32 Accel = 2;
		s32 Speed;
		if( Age<=(MaxSpeed / Accel) )
			Speed = MinSpeed + MaxSpeed -Age*Accel;
		else
			Speed = MinSpeed - MaxSpeed +Age*Accel;

		Sprite[0]->Rotate(Rdeg);
		Sprite[0]->SlideMoveR(Speed ,0);

		X = RoundOff(Sprite[0]->GetSpr()->FCX);
		Y = RoundOff(Sprite[0]->GetSpr()->FCY);
	}
	else {
		Inc(ArriveCount);
		if( ArriveCount == 1 ) {
			Marker->Die();
			Sprite[0]->Move(DestX,DestY);
		}

		if( (! FOwner->IsSequence() ) || (HomingLevel == 3) )  {
			//着弾して5フレーム後に爆発発生
			if( ArriveCount == 5 ) {
				FOwner->Add(new TEnergyBall(FOwner, DestX,DestY,HomingLevel*40, HomingLevel));
				FOwner->Add(new TLightCircle(FOwner,DestX,DestY,255,128,128,5+HomingLevel));
			}
		}
		else {
			if( ArriveCount == 1 ) {
				FOwner->Add(new TEnergyBall(FOwner, DestX,DestY,HomingLevel*240, 1.5f));
				FOwner->Add(new TLightCircle(FOwner,DestX,DestY,255,128,128,5+HomingLevel*2));
			}
		}
	}


	Sprite[1]->GetSpr()->SetColorR(-4,ctAlpha);

	if( Trace->IsTerminated || (Age>300) ) Die();

	TOBJ::Move();

}

void TZakoHomingCore::Die()
{
	TOBJ::Die();
	if( ArriveCount<1 ) {
		Marker->Die();
	}
	Trace->Die();

}

void TZakoHomingCore::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);

}

TEyeLaser::TEyeLaser( TOBJList* owner, s32 sx, s32 sy, s32 _r ) : TOBJ(owner)
{
	FObjectName = "TEyeLaser";

	X = sx;
	Y = sy;

	Rdeg = _r;

	MakeSpriteLite(0, X,Y,48,48,15000,bmAdd);
	Sprite[0]->GetSpr()->AddTexture(Eff8Tex, 216, 128, 239, 175);
	//    FCX += 8;
	Sprite[0]->GetSpr()->FCY += 24;
	Sprite[0]->GetSpr()->Zoom(1.f,2.5f);
	Sprite[0]->GetSpr()->Move(X,Y);
	Sprite[0]->GetSpr()->SetColor(128, ctAlpha);
	Sprite[0]->GetSpr()->SetColor(255, ctRed);
	Sprite[0]->GetSpr()->SetColor(128, ctBlue);
	Sprite[0]->GetSpr()->SetColor(255, ctGreen);

	MakeSpriteLite(1, X,Y,48,48,15000,bmAdd);
	Sprite[1]->GetSpr()->AddTexture(Eff8Tex, 192, 128, 215, 175);
	Sprite[1]->GetSpr()->FCX += 48;
	Sprite[1]->GetSpr()->FCY += 24;
	Sprite[1]->GetSpr()->Zoom(1,2.5);
	Sprite[1]->GetSpr()->Move(X,Y);
	Sprite[1]->GetSpr()->SetColor(128, ctAlpha);
	Sprite[1]->GetSpr()->SetColor(255, ctRed);
	Sprite[1]->GetSpr()->SetColor(128, ctBlue);
	Sprite[1]->GetSpr()->SetColor(255, ctGreen);

	Sprite[0]->Rotate(Rdeg);
	Sprite[1]->Rotate(Rdeg+2048);

	FOwner->Add(new TSRControl(FOwner,X,Y,Rdeg+2048));

}

void TEyeLaser::Move()
{
	TOBJ::Move();
	if( (Age<=10) && (Age % 2 == 0) )
		FOwner->Add(new TEyeLaserParts(FOwner,X,Y,Rdeg));

	Sprite[0]->GetSpr()->Rotate(0);
	if( Sprite[0]->GetSpr()->FZX<30 ) Sprite[0]->GetSpr()->ZoomR(1.4f, 1.0f);     //あまり拡大しすぎると表示が乱れるようなので、拡大率に制限
	Sprite[0]->GetSpr()->ZoomR(1.0f, 0.95f);
	Sprite[0]->GetSpr()->Rotate(Rdeg);
	Sprite[0]->GetSpr()->SetColorR(-2, ctAlpha);
	if( Sprite[0]->GetSpr()->GetColor(ctAlpha) == 0 ) Die();

	Sprite[1]->GetSpr()->Rotate(0);
	Sprite[1]->GetSpr()->ZoomR(1.0f, 0.95f);
	Sprite[1]->GetSpr()->Rotate(Rdeg);
	Sprite[1]->GetSpr()->SetColorR(-2, ctAlpha);

}

TEyeLaserParts::TEyeLaserParts( TOBJList* owner, s32 sx, s32 sy, s32 _r ) : TOBJ(owner)
{
	FObjectName = "TEyeLaserParts";
	X = sx;
	Y = sy;
	Buzzed = false;
	LastBuzz = 0;
	FKind = kEShot;
	if( FOwner->IsSequence() )
		FSTR = 1000;
	else
		FSTR = 2000;
	Rdeg = _r && 0x0fff;

	MakeSpriteLite(0, X,Y,48,48,15000,bmAdd);
	Sprite[0]->GetSpr()->AddTexture(Eff8Tex, 136, 1, 255, 127);
	Sprite[0]->GetSpr()->AddHitArea(TPoint(4, 4), TPoint(4, 44), TPoint(64, 44),TPoint(64, 4), htSky);
	Sprite[0]->GetSpr()->FHit        =  true;
	//    FCX += 8;
	Sprite[0]->GetSpr()->FCY += 24;
	Sprite[0]->GetSpr()->Zoom(1,2);
	Sprite[0]->GetSpr()->Move(X,Y);

	MakeSpriteLite(1, X,Y,48,48,15000,bmAdd);
	Sprite[1]->GetSpr()->AddTexture(Eff8Tex, 128, 1, 135, 127);
	Sprite[1]->GetSpr()->FCX += 48;
	Sprite[1]->GetSpr()->FCY += 24;
	Sprite[1]->GetSpr()->Zoom(1,2);
	Sprite[1]->GetSpr()->Move(X,Y);

	Sprite[0]->Rotate(Rdeg);
	Sprite[1]->Rotate(Rdeg+2048);

}

void TEyeLaserParts::Move()
{
	TOBJ::Move();
	Sprite[0]->GetSpr()->Rotate(0);
	if( Sprite[0]->GetSpr()->FZX<30 ) Sprite[0]->GetSpr()->ZoomR(1.4f, 1.0f);     //あまり拡大しすぎると表示が乱れるようなので、拡大率に制限
	Sprite[0]->GetSpr()->ZoomR(1.0f, 0.95f);
	Sprite[0]->GetSpr()->Rotate(Rdeg);
	Sprite[0]->GetSpr()->SetColorR(-4, ctAlpha);
	if( Sprite[0]->GetSpr()->GetColor(ctAlpha) == 0 ) Die();

	if( Age == 30 ) Sprite[0]->GetSpr()->FHit = false;

	Sprite[1]->GetSpr()->Rotate(0);
	Sprite[1]->GetSpr()->ZoomR(1.0f, 0.95f);
	Sprite[1]->GetSpr()->Rotate(Rdeg);
	Sprite[1]->GetSpr()->SetColorR(-4, ctAlpha);

	if( (Age-LastBuzz)>=40 ) Buzzed = false;

}

void TEyeLaserParts::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);
	if( target->Kind() == kHero ) {
		if( (! Buzzed) && (target->Hit[HitBuzz]) ) {
			FOwner->Scrach(this);
			Buzzed = true;
			LastBuzz = Age;
		}
	}
}

TZakoHomingCore2::TZakoHomingCore2( TOBJList* owner, s32 sx, s32 sy, s32 _r, s32 _level ) : TOBJ(owner)
{
	FObjectName = "TZakoHomingCore2";

	X = sx;
	Y = sy;
	Buzzed = false;
	LastBuzz = 0;
	FKind = kEShot;
	FSTR = 500;
	Rdeg = _r & 0x0fff;
	HomingLevel  =  _level;

	MakeSpriteLite(0, X, Y,64,64,5000,bmAdd);
	Sprite[0]->GetSpr()->AddTexture(Eff8Tex, 192, 128, 239, 175);
	Sprite[0]->GetSpr()->AddHitArea(TPoint(16, 16), TPoint(16, 48), TPoint(48, 48),TPoint(48, 16), htSky);
	Sprite[0]->GetSpr()->FHit        =  true;
	Sprite[0]->GetSpr()->FCX += 32;
	Sprite[0]->GetSpr()->FCY += 16;
	Sprite[0]->GetSpr()->Move(X,Y);

	Trace  =  new THomingTrace2(FOwner, Sprite[0], 32, 32);
	FOwner->Add(Trace);

	Sprite[0]->Rotate(Rdeg);

}

TZakoHomingCore2::~TZakoHomingCore2()
{

}

void TZakoHomingCore2::Move()
{
	Rdeg  =  Rdeg & 0x0fff;
	PrevR = Rdeg;

	s32 Steering =  5+ HomingLevel;
	if( Age>20 ) Inc(Steering, Age / 2);


	s32 DX = FOwner->HeroPos.X-X;             //自分と敵との距離を計算
	s32 DY = FOwner->HeroPos.Y-Y;

	s32 EnemyDeg = RoundOff(atan2(DY,DX)/(f32)M_PI*2048.f) & 0x0fff;

	s32 Rdiff = GetDiffR( EnemyDeg, Rdeg);

	if( abs(Rdiff)>=2048 ) Inc(Rdeg, (s32)(-Sign(Rdiff)*ceil((f32)(4095-abs(Rdiff))/(f32)Steering)-Sign(Rdiff)));
	else if( abs(Rdiff)<2048 ) Dec(Rdeg, (s32)ceil((f32)(Rdiff)/(f32)Steering)+Sign(Rdiff));

	if( !((X > 640+70) || (Y > 480+70) || (Y <0-70) || (X < 0-70)) ) {
		s32 Speed;
		if( Age<15 )
			Speed = HomingLevel + 20 - Age;
		else
			Speed = HomingLevel+5;

		Sprite[0]->Rotate(Rdeg);
		Sprite[0]->SlideMoveR(Speed ,0);

		Sprite[0]->MoveR(FOwner->Speed - KSCROLLSPEED_NORMAL,0);

		X = RoundOff(Sprite[0]->GetSpr()->FCX);
		Y = RoundOff(Sprite[0]->GetSpr()->FCY);
	}

	if( Age % 4 == 0 )
		Sprite[0]->GetSpr()->Zoom(0.9f,0.9f);
	else
		Sprite[0]->GetSpr()->Zoom(1.0f,1.0f);

	if( Trace->IsTerminated ) Die();
	TOBJ::Move();

}

void TZakoHomingCore2::Die()
{
	TOBJ::Die();
	Trace->Die();

}

void TZakoHomingCore2::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);
	if( target->Kind() == kHero ) {
		if( (! Buzzed) && (target->Hit[HitBuzz]) ) {
			FOwner->Scrach(this);
			Buzzed = true;
			LastBuzz = Age;
		}
	}

}

TEnergyBall::TEnergyBall( TOBJList* owner, s32 sx, s32 sy, s32 _life, f32 _zoom ) : TOBJ(owner)
{
	FObjectName = "TEnergyBall";

	X = sx;
	Y = sy;
	Buzzed = false;
	LastBuzz = 0;
	FKind = kEShot;
	FSTR = 500;
	Life = _life;
	ZoomRate = _zoom;

	if( FOwner->Difficulty == KDIFFICULTY_HARD )
		Life = (s32)(Life*1.5f);

	MakeSpriteLite(0, X,Y,64,64,15000,bmAdd);
	Sprite[0]->GetSpr()->AddTexture(Eff8Tex, 1, 128, 31, 160);
	Sprite[0]->GetSpr()->AddHitArea(TPoint(16, 16), TPoint(16, 48), TPoint(48, 48),TPoint(48, 16), htSky);
	if( FOwner->IsSequence() )
		Sprite[0]->GetSpr()->FHit        =  true;
	else
		Sprite[0]->GetSpr()->FHit        =  false;
	Sprite[0]->GetSpr()->FCX += 32;
	Sprite[0]->GetSpr()->FCY += 32;
	Sprite[0]->GetSpr()->Move(X,Y);
	//    SetColor(128, ctBlue);
	//    SetColor(128, ctGreen);
	Sprite[0]->GetSpr()->Zoom(0.5f, 0.5f);

	//着弾時の閃光
	MakeSpriteLite(1, X,Y,64,64,15000,bmAdd);
		//    AddTexture(Eff1Tex, 1, 17, 176, 192);
		Sprite[1]->GetSpr()->AddTexture(Eff8Tex, 192, 128, 239, 175);
		Sprite[1]->GetSpr()->FCX += 32;
		Sprite[1]->GetSpr()->FCY += 32;
		Sprite[1]->GetSpr()->Move(X,Y);
		//    SetColor(128, ctBlue);
		//    SetColor(128, ctGreen);
		Sprite[1]->GetSpr()->Zoom(ZoomRate*2.f, ZoomRate*2.f);

}

void TEnergyBall::Move()
{
	TOBJ::Move();

	if( Age == 2 ) Sprite[1]->GetSpr()->FVisible = false;

	//光球本体の制御
	if( Age > 10 ) {
		if( Age < Life ) {
			if( Sprite[0]->GetSpr()->FHit == false ) {
				if( Sprite[0]->GetSpr()->FZX < ZoomRate ) {
					Sprite[0]->GetSpr()->ZoomR(1.1f, 1.1f);
				}
				else {
					Sprite[0]->GetSpr()->FHit = true;   //最大まで拡大しきったら当たり判定を出す
				}
			}
			else {
				if( Age % 4 == 0 )
					Sprite[0]->GetSpr()->ZoomR(0.9f, 0.9f);
				else
					Sprite[0]->GetSpr()->Zoom(ZoomRate,ZoomRate);
			}
		}
		else {
			Sprite[0]->GetSpr()->FHit = false;    //縮小時には当たり判定は消す
			if( Sprite[0]->GetSpr()->FZX > 0.1f ) {
				Sprite[0]->GetSpr()->ZoomR(0.9f, 0.9f);
				Sprite[0]->GetSpr()->SetColorR(-16,ctAlpha);
			}
			else
				Die();
		}

	}

}

void TEnergyBall::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);
	if( target->Kind() == kHero ) {
		if( (! Buzzed) && (target->Hit[HitBuzz]) ) {
			FOwner->Scrach(this);
			Buzzed = true;
			LastBuzz = Age;
		}
	}

}

TZ01Shot3::TZ01Shot3( TOBJList* owner, s32 sx, s32 sy, s32 _R, s32 _S, s32 _str, f32 _IniXS, f32 _zoom ) : TOBJ(owner)
{
	FObjectName = "TZ01Shot2";
	X = sx;
	Y = sy;
	FSTR = _str;
	FKind = kEShot;
	Buzzed = false;
	LastBuzz = 0;
	Anim = false;
	Rdeg = _R;
	Speed = _S;
	IniXSpeed = _IniXS;
	OriginXZoom = _zoom;

	TSpriteEXBLT sblt;
		sblt.RBM      =  bmNormal; // αブレンドモード
		sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
		sblt.RDDDD    =  FOwner->GetDG();     // TDDDDの参照
		sblt.RHeight  =  64;        // 高さ
		sblt.RWidth   =  64;        // 幅
		sblt.RX       =  X;        // X座標
		sblt.RY       =  Y;        // Y座標
		sblt.RZ       =  TamaZ;       // Z値

	Inc(TamaZ);
	if( TamaZ>1099 ) TamaZ = 1000;

	Sprite[0] =  new TJoint(sblt);         // スプライトの生成
	Inc(numSpr,1);
	Sprite[0]->GetSpr()->AddTexture(Eff1Tex, 176, 113, 176+31, 113+31);    // テクスチャを登録          // 177, 145, 177+47, 145+31
	Sprite[0]->GetSpr()->FVisible  =  false;
	Sprite[0]->GetSpr()->AddHitArea(TPoint(24, 24), TPoint(24, 40), TPoint(40, 40),TPoint(40, 24), htSky);
	Sprite[0]->GetSpr()->FHit      =  true;
	Sprite[0]->GetSpr()->FCX += 32;
	Sprite[0]->GetSpr()->FCY += 32;
	Sprite[0]->GetSpr()->Zoom(OriginXZoom,1);

	Sprite[0]->Move(X,Y); //弾の初期位置を設定
	Sprite[0]->Rotate(Rdeg); //弾の方角を決定


}

void TZ01Shot3::Move()
{
	TOBJ::Move();
	X = (s32)(Sprite[0]->GetSpr()->FCX);
	Y = (s32)(Sprite[0]->GetSpr()->FCY);

	Sprite[0]->SlideMoveR(Speed,0); //弾の移動
	Sprite[0]->MoveR(IniXSpeed,0);  //撃ったオブジェクトのスピードを足す

	if( Age % 3 == 0 ) Anim = ! Anim;

	if( Age == 2 ) Sprite[0]->GetSpr()->FVisible = true;


	Sprite[0]->Rotate(0);
	if( Anim ) Sprite[0]->GetSpr()->Zoom(OriginXZoom, 0.6f);
	else Sprite[0]->GetSpr()->Zoom(OriginXZoom, 1.1f);
	Sprite[0]->Rotate(Rdeg);

	if( (Age-LastBuzz)>=40 ) Buzzed = false;

	if( Age>=240 ) {
		FOwner->Add(new TFCircle(FOwner,X,Y,1,Rdeg,Speed));
		Die();
	}

	if( (Age > 60) &&
		((X > 640+32) || (Y > 480+32) || (Y <0-32) || (X < 0-32)) ) Die();//弾が画面外に消えたら消す

}

void TZ01Shot3::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);
	if( target->Kind() == kHero ) {
		if( (! Buzzed) && (target->Hit[HitBuzz]) ) {
			FOwner->Scrach(this);
			Buzzed = true;
			LastBuzz = Age;
		}

		if( target->Hit[HitBody] ) {
			Die();
			FOwner->Add(new TExpSS(FOwner,X,Y,0,-10));
		}
	}

}

TS2Boss2LaserParts::TS2Boss2LaserParts( TOBJList* owner, s32 sx, s32 sy, s32 _r ) : TOBJ(owner)
{
	FObjectName = "TS2Boss2LaserParts";
	X = sx;
	Y = sy;
	Buzzed = false;
	LastBuzz = 0;
	FKind = kEShot;
	FSTR = 2000;
	Rdeg = _r & 0x0fff;

	MakeSpriteLite(0, X,Y,48,48,15000,bmAdd);
	Sprite[0]->GetSpr()->AddTexture(Eff8Tex, 136, 1, 255, 127);
	Sprite[0]->GetSpr()->AddHitArea(TPoint(4, 4), TPoint(4, 44), TPoint(64, 44),TPoint(64, 4), htSky);
	Sprite[0]->GetSpr()->FHit        =  true;
	//    FCX += 8;
	Sprite[0]->GetSpr()->FCY += 24;
	Sprite[0]->GetSpr()->Zoom(1,1);
	Sprite[0]->GetSpr()->Move(X,Y);

	Sprite[0]->Rotate(Rdeg);

}

void TS2Boss2LaserParts::Move()
{
	TOBJ::Move();
	Sprite[0]->GetSpr()->Rotate(0);
	if( Sprite[0]->GetSpr()->FZX<30 ) Sprite[0]->GetSpr()->ZoomR(1.4f, 1.0f);     //あまり拡大しすぎると表示が乱れるようなので、拡大率に制限
	Sprite[0]->GetSpr()->ZoomR(1.0f, 0.95f);
	Sprite[0]->GetSpr()->Rotate(Rdeg);
	Sprite[0]->GetSpr()->SetColorR(-8, ctAlpha);
	if( Sprite[0]->GetSpr()->GetColor(ctAlpha) == 0 ) Die();
	if( Age == 10 ) Sprite[0]->GetSpr()->FHit = false;

	if( (Age-LastBuzz)>=40 ) Buzzed = false;

}

void TS2Boss2LaserParts::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);
	if( target->Kind() == kHero ) {
		if( (! Buzzed) && (target->Hit[HitBuzz]) ) {
			FOwner->Scrach(this);
			Buzzed = true;
			LastBuzz = Age;
		}
	}

}

TZBeamPart2::TZBeamPart2( TOBJList* owner, s32 sx, s32 sy, f32 _xspeed, f32 _yspeed, s32 R, f32 _zoom, bool _hit ) : TOBJ(owner)
{
	FObjectName = "TZBeamPart";
	X = sx;
	Y = sy;
	XSpeed = _xspeed;
	YSpeed = _yspeed;
	FSTR = 1000;
	FKind = kEShot;
	Buzzed = false;
	LastBuzz = 0;
	Rdeg = R;

	TSpriteEXBLT sblt;
	sblt.RBM      =  bmSubAdd; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  FOwner->GetDG();     // TDDDDの参照
	sblt.RHeight  =  64;        // 幅
	sblt.RWidth   =  64;        // 高さ
	sblt.RX       =  X;        // X座標
	sblt.RY       =  Y;        // Y座標
	sblt.RZ       =  1100;       // Z値

	Sprite[0] =  new TJoint(sblt);         // スプライトの生成
	Inc(numSpr,1);
		Sprite[0]->GetSpr()->AddTexture(Eff2Tex,0, 128, 63, 191);    // テクスチャを登録
		Sprite[0]->GetSpr()->FVisible  =  true;
		Sprite[0]->GetSpr()->AddHitArea(TPoint(0, 20), TPoint(0, 44), TPoint(63, 44),TPoint(63, 20), htSky);
		Sprite[0]->GetSpr()->FHit      =  _hit;
		Sprite[0]->GetSpr()->FCX += 32;
		Sprite[0]->GetSpr()->FCY += 32;
		Sprite[0]->GetSpr()->Zoom(4.f,_zoom);
		Sprite[0]->GetSpr()->SetColor(150,ctGreen);
		Sprite[0]->GetSpr()->SetColor(150,ctBlue);
		Sprite[0]->GetSpr()->SetColor(100,ctAlpha);

	Sprite[0]->Move(X,Y); //弾の初期位置を設定
	Sprite[0]->Rotate(Rdeg);


}

void TZBeamPart2::Move()
{
	TOBJ::Move();
	X = RoundOff(Sprite[0]->GetSpr()->FCX);
	Y = RoundOff(Sprite[0]->GetSpr()->FCY);

	Sprite[0]->SlideMoveR(30,0); //弾の移動
	Sprite[0]->MoveR(XSpeed,YSpeed);

	if( (Age-LastBuzz)>=40 ) Buzzed = false;

	if( (X > 640+70) || (Y > 480+70) || (Y <0-70) || (X < 0-70) ) Die();//弾が画面外に消えたら消す
}

void TZBeamPart2::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);
	if( target->Kind() == kHero ) {
		if( (! Buzzed) && (target->Hit[HitBuzz]) ) {
			FOwner->Scrach(this);
			Buzzed = true;
			LastBuzz = Age;
		}
	}

}

TBeamControl::TBeamControl( TOBJList* owner, s32 sx, s32 sy, s32 _pat ) : TOBJ(owner)
{
	FObjectName = "TBeamControl";

	X = sx;
	Y = sy;
	Pattern  =  _pat;

}

void TBeamControl::Move()
{
	switch( Pattern ){
		//4連射　2-1ボス登場時演出
		case 0:{
			switch( Age ){
				case 0:{ FOwner->Add(new TFunBeamCore(FOwner,X,Y,1700,3)); if( CreateSE ) FOwner->SoundEffect->WavPlay(WABeam,9); }		break;
				case 10:{ FOwner->Add(new TFunBeamCore(FOwner,X,Y,1300,3)); if( CreateSE ) FOwner->SoundEffect->WavPlay(WABeam,9); }	break;
				case 20:{ FOwner->Add(new TFunBeamCore(FOwner,X,Y,900,3)); if( CreateSE ) FOwner->SoundEffect->WavPlay(WABeam,9); }		break;
				case 30:{ FOwner->Add(new TFunBeamCore(FOwner,X,Y,500,3)); if( CreateSE ) FOwner->SoundEffect->WavPlay(WABeam,9); }		break;
				case 31:Die();
			}
		}break;
	}
	TOBJ::Move();

}

void TBoss3Shot::ExAction()
{
	switch( ActFlg ){
		case 0:{
			Sprite[0]->SlideMoveR(Speed,0);
			Speed = Speed-Accel;

			if( Speed<1 ) {
				Inc(ActFlg);
				ActCount = -1;
			}
		}break;
		case 1:{
			Sprite[0]->SlideMoveR(Speed,0);
			Speed = Speed+Accel;
		}break;
	}

	Inc(ActCount);

}

TBoss3Shot::TBoss3Shot( TOBJList* owner, s32 sx, s32 sy, s32 _R, s32 _S, s32 _str, f32 _IniXS, f32 _zoom, s32 _pat, f32 _accel ) : TOBJ(owner)
{
	FObjectName = "TBoss3Shot";
	X = sx;
	Y = sy;
	FSTR = _str;
	FKind = kEShot;
	Buzzed = false;
	LastBuzz = 0;
	Rdeg = _R;
	Speed = _S;
	IniXSpeed = _IniXS;
	OriginXZoom = _zoom;
	Anim = false;
	Pattern = _pat;
	ActFlg = 0;
	ActCount = 0;
	Accel = _accel;

	MakeSpriteLite(0, X,Y,64,32,TamaZ,bmNormal);
	switch( FOwner->NowStage ){
	case 0:
	case 3:Sprite[0]->GetSpr()->AddTexture(Eff8Tex, 95, 192, 32, 223);	break;
	default:
		Sprite[0]->GetSpr()->AddTexture(Eff8Tex, 159, 192, 96, 223);
	}
	//    AddTexture(MechaTex, 223, 256, 160, 287);
	Sprite[0]->GetSpr()->AddHitArea(TPoint(4, 4), TPoint(4, 28), TPoint(60, 28),TPoint(60, 4), htSky);
	Sprite[0]->GetSpr()->FHit        =  true;
	Sprite[0]->GetSpr()->FCX += 16;
	Sprite[0]->GetSpr()->FCY += 16;
	Sprite[0]->GetSpr()->Zoom(1,1);
	Sprite[0]->GetSpr()->Move(X,Y);

	Inc(TamaZ);
	if( TamaZ>1099 ) TamaZ = 1000;

	Sprite[0]->Move(X,Y); //弾の初期位置を設定
	Sprite[0]->Rotate(Rdeg); //弾の方角を決定

}

void TBoss3Shot::Move()
{
	TOBJ::Move();
	X = (s32)(Sprite[0]->GetSpr()->FCX);
	Y = (s32)(Sprite[0]->GetSpr()->FCY);

	switch( Pattern ){
		case 0:Sprite[0]->SlideMoveR(Speed,0);	break;  //弾の移動
		case 1:ExAction();	break;
	}


	Sprite[0]->MoveR(IniXSpeed,0);  //撃ったオブジェクトのスピードを足す

	if( Age % 3 == 0 ) Anim = ! Anim;

	Sprite[0]->Rotate(0);
	if( Anim ) Sprite[0]->GetSpr()->Zoom(OriginXZoom,0.6f);
	else Sprite[0]->GetSpr()->Zoom(OriginXZoom,1.0f);
	Sprite[0]->Rotate(Rdeg);

	if( (Age-LastBuzz)>=40 ) Buzzed = false;

	if( (X > 640+32) || (Y > 480+32) || (Y <0-32) || (X < 0-32) ) Die();//弾が画面外に消えたら消す

}

void TBoss3Shot::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);
	if( target->Kind() == kHero ) {
		if( (! Buzzed) && (target->Hit[HitBuzz]) ) {
			FOwner->Scrach(this);
			Buzzed = true;
			LastBuzz = Age;
		}

		if( target->Hit[HitBody] ) {
			Die();
			FOwner->Add(new TExpSS(FOwner,X,Y,0,-10));
		}
	}

}

TBoss3Beam::TBoss3Beam( TOBJList* owner, s32 sx, s32 sy, s32 dx, s32 dy ) : TOBJ(owner)
{
	FObjectName = "TBoss3Beam";
	X = sx;
	Y = sy;
	FKind = kEffect2;
	Buzzed = false;
	LastBuzz = 0;
	DestX = dx;
	DestY = dy;

	f32 diffX = RoundOff(dx-X);
	f32 diffY = RoundOff(dy-Y);
	Rdeg = (RoundOff(atan2(diffY,diffX)/(f32)M_PI*2048.f) + 2048) & 0xfff;

	MakeSpriteLite(0, X,Y,32,32,TamaZ,bmNormal);
	Sprite[0]->GetSpr()->AddTexture(MechaTex, 223, 256, 160, 287);
	Sprite[0]->GetSpr()->FCX += 16;
	//    FCY += 16;
	Sprite[0]->GetSpr()->FVisible  =  false;
	Sprite[0]->GetSpr()->Zoom(1,1);
	Sprite[0]->GetSpr()->Move(X,Y);

	Inc(TamaZ);
	if( TamaZ>1099 ) TamaZ = 1000;

	Trace  =  new THomingTrace(FOwner, Sprite[0], 16, 32);
	FOwner->Add(Trace);

	Sprite[0]->Move(X,Y); //弾の初期位置を設定
	Sprite[0]->Rotate(Rdeg); //弾の方角を決定

}

TBoss3Beam::~TBoss3Beam()
{

}

void TBoss3Beam::Move()
{
	TOBJ::Move();
	if( (Age-LastBuzz)>=40 ) Buzzed = false;

	X = RoundOff(Sprite[0]->GetSpr()->FCX);
	Y = RoundOff(Sprite[0]->GetSpr()->FCY);

	if( Age=1 ) Sprite[0]->Move(DestX, DestY);

	if( Age=10 ) {
		FOwner->Add(new TEnergyBall(FOwner, DestX,DestY,15, 2));
		FOwner->Add(new TLightCircle(FOwner,DestX,DestY,255,128,128,7));

		FOwner->Add(new T8WayBeam(FOwner, DestX,DestY));
	}

	if( Trace->IsTerminated ) Die();

}

void TBoss3Beam::Die()
{
	TOBJ::Die();
	Trace->Die();

}

void TBoss3Beam::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);

}

TLongBeam::TLongBeam( TOBJList* owner, s32 sx, s32 sy, s32 R, f32 _zoom, f32 _speed ) : TOBJ(owner)
{
	FObjectName = "TLongBeam";
	X = sx;
	Y = sy;
	FKind = kEShot;
	FSTR = 1000;
	Buzzed = false;
	LastBuzz = 0;

	Rdeg = R;
	MaxZoom = _zoom;
	ZoomSpeed = _speed;

	if( ! FOwner->IsSequence() ) {
		MakeSpriteLite(0, X,Y,32,32,TamaZ,bmAdd);
		Sprite[0]->GetSpr()->AddTexture(MechaTex, 223, 256, 160, 287);
		Sprite[0]->GetSpr()->AddHitArea(TPoint(12, 4), TPoint(12, 28), TPoint(28, 28),TPoint(28, 4), htSky);
		Sprite[0]->GetSpr()->FHit        =  true;
		Sprite[0]->GetSpr()->FCY += 16;
		Sprite[0]->GetSpr()->Move(X,Y);
		Sprite[0]->GetSpr()->Rotate(Rdeg);
	}
	else {
		MakeSpriteLite(0, X,Y,32,32,TamaZ,bmAdd);
		MakeSpriteLite(1, X,Y,32,32,TamaZ+1,bmNormal);
		for( u32 i = 0; i <= 1; i++ ) {
			Sprite[i]->GetSpr()->AddTexture(Eff1Tex, 1, 193, 63, 255);
			Sprite[i]->GetSpr()->FCY += 16;
			Sprite[i]->GetSpr()->Move(X,Y);
			Sprite[i]->GetSpr()->Rotate(Rdeg);
		}
		Sprite[0]->GetSpr()->AddHitArea(TPoint(4, 4), TPoint(4, 28), TPoint(28, 28),TPoint(28, 4), htSky);
		Sprite[0]->GetSpr()->FHit        =  true;

		Sprite[0]->Add(Sprite[1]);
	}

	Inc(TamaZ);
	if( TamaZ>1099 ) TamaZ = 1000;


}

TLongBeam::~TLongBeam()
{

}

void TLongBeam::Move()
{
	TOBJ::Move();
	f32 ZoomRate  =  ZoomSpeed * (f32)Age / 32.f;
	for( u32 i = 0; i <= 1; i++ ) {
		if( ZoomRate<MaxZoom ) {
			Sprite[i]->GetSpr()->Rotate(0);
			Sprite[i]->GetSpr()->Zoom(ZoomRate,0.5f);
			Sprite[i]->GetSpr()->Rotate(Rdeg);
		}
		else {
			Sprite[i]->GetSpr()->SlideMoveR(ZoomSpeed, 0.f);
		}
	}

}

void TLongBeam::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);
	if( target->Kind() == kHero ) {
		if( (! Buzzed) && (target->Hit[HitBuzz]) ) {
			FOwner->Scrach(this);
			Buzzed = true;
			LastBuzz = Age;
		}
	}

}

T8WayBeam::T8WayBeam( TOBJList* owner, s32 sx, s32 sy ) : TOBJ(owner)
{
	FObjectName = "T8WayBeam";
	X = sx;
	Y = sy;
	FKind = kEffect2;
	ShotCount = 0;

	s32 DX = RoundOff(FOwner->HeroPos.X-X);
	s32 DY = RoundOff(FOwner->HeroPos.Y-Y);
	Rdeg = (RoundOff(atan2(DY,DX)/(f32)M_PI*2048.f)) & 0xfff;

}

void T8WayBeam::Move()
{
	if( Age % 5 == 0 ) {
		switch( FOwner->Difficulty ){
		case KDIFFICULTY_SINGLE:FOwner->Add(new TLongBeam(FOwner, X,Y,Rdeg + 512*ShotCount, 20, 5));	break;
		case KDIFFICULTY_HARD:FOwner->Add(new TLongBeam(FOwner, X,Y,Rdeg + 512*ShotCount, 23, 5));	break;
		default:
		FOwner->Add(new TLongBeam(FOwner, X,Y,Rdeg + 512*ShotCount, 13, 5));
		}

		if( CreateSE ) FOwner->SoundEffect->WavPlay(WABeam,10);
		Inc(ShotCount);
	}

	if( ShotCount>=8 ) Die();
	TOBJ::Move();

}

TOrthLaserPart::TOrthLaserPart( TOBJList* owner, s32 sx, s32 sy, s32 _life, bool _col ) : TOBJ(owner)
{
	FObjectName = "TOrthLaserPart";
	X = sx;
	Y = sy;
	FKind = kEShot;
	FSTR = 1000;
	Buzzed = false;
	LastBuzz = 0;
	Life = _life;

	MakeSpriteLite(0, X,Y,32,32,42000,bmAdd);
	Sprite[0]->GetSpr()->AddTexture(MechaTex, 224, 256, 287, 319);
	if( _col ) {
		Sprite[0]->GetSpr()->AddHitArea(TPoint(4, 4), TPoint(4, 28), TPoint(28, 28),TPoint(28, 4), htSky);
		Sprite[0]->GetSpr()->FHit        =  true;
	}
	Sprite[0]->GetSpr()->FCX += 16;
	Sprite[0]->GetSpr()->FCY += 16;
	Sprite[0]->GetSpr()->Move(X,Y);

}

void TOrthLaserPart::Move()
{
	TOBJ::Move();
	X = (s32)(Sprite[0]->GetSpr()->FCX);
	Y = (s32)(Sprite[0]->GetSpr()->FCY);

	if( Age>=Life ) Die();
	//  Sprite[0]->GetSpr()->SetColorR(-8,ctAlpha);
	//  if( Sprite[0]->GetSpr()->GetColor(ctAlpha)=0 )  Die();


	if( (Age-LastBuzz)>=40 ) Buzzed = false;

	if( (X > 640+32) || (Y > 480+32) || (Y <0-32) || (X < 0-32) ) Die();//弾が画面外に消えたら消す

}

void TOrthLaserPart::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);
	if( target->Kind() == kHero ) {
		if( (! Buzzed) && (target->Hit[HitBuzz]) ) {
			FOwner->Scrach(this);
			Buzzed = true;
			LastBuzz = Age;
		}
	}

}

TOrthLaser::TOrthLaser( TOBJList* owner, s32 sx, s32 sy, s32 _speed, s32 _life ) : TOBJ(owner)
{
	FObjectName = "TOrthLaser";
	X = sx;
	Y = sy;
	FKind = kEffect;
	Speed = _speed;
	Life = _life;
	TurnCount = 0;

	//先導用透明スプライト
	MakeSpriteLite(0, X,Y,32,32,42000,bmAdd);
	Sprite[0]->GetSpr()->AddTexture(MechaTex, 224, 256, 287, 319);
	Sprite[0]->GetSpr()->FVisible = false;
	Sprite[0]->GetSpr()->FCX += 16;
	Sprite[0]->GetSpr()->FCY += 16;
	Sprite[0]->GetSpr()->Move(X,Y);
}

void TOrthLaser::Move()
{
	TOBJ::Move();
	bool Collision;
	if( Age % 2 == 0 ) {
		if( Age % 6 == 0 )
			Collision = true;
		else
			Collision = false;
		FOwner->Add(new TOrthLaserPart(FOwner, X, Y, Life, Collision));
	}

	X = (s32)(Sprite[0]->GetSpr()->FCX);
	Y = (s32)(Sprite[0]->GetSpr()->FCY);

	if( (TurnCount == 0) && (X < FOwner->HeroPos.X) ) {
		if( Y < FOwner->HeroPos.Y )
			Sprite[0]->Rotate(-1024);
		else
			Sprite[0]->Rotate(1024);
		TurnCount = 1;
	}

	Sprite[0]->GetSpr()->SlideMoveR(Speed,0);

	if( (X > 640+32) || (Y > 480+32) || (Y <0-32) || (X < 0-32) ) Die();//弾が画面外に消えたら消す  

}

TCurvedLaser::TCurvedLaser( TOBJList* owner, s32 _x, s32 _y, s32 _r, s32 _rspeed, f32 _speed, f32 _accel, f32 _vel, s32 _pat ) : TOBJ(owner)
{
	FObjectName = "TCurvedLaser";
	X = _x;
	Y = _y;
	FKind = kEShot;
	Buzzed = false;
	LastBuzz = 0;
	FSTR = 500;

	const s32 MaxPoint = 16;

	ActFlg = 0;
	Rdeg = _r & 0xfff;
	Accel = _accel;
	Velocity = _vel;
	RSpeed = _rspeed;
	Speed = Velocity;
	Pattern = _pat;

	//本体
	MakeSpriteLite(0, X,Y,16,16,16000,bmAdd);
	Sprite[0]->GetSpr()->AddTexture(MechaTex, 223, 256, 160, 287);
	Sprite[0]->GetSpr()->FCX += 8;
	Sprite[0]->GetSpr()->FCY += 8;
	Sprite[0]->GetSpr()->Move(X,Y);
	Sprite[0]->GetSpr()->FVisible = false;

	Rect uv(32, 192, 95, 223);
	TBlendMode blend;
	if( FOwner->NowStage=4 )
		blend  =  bmNormal;
	else
		blend  =  bmAdd;
	pl  =  new TPolyLine2D(FOwner, MaxPoint-2, 8, Eff8Tex, uv, blend);
	FOwner->Add(pl);
	for( u32 i  =  0 ; i < MaxPoint; i++ ) {
		glm::vec2 pos(X, Y);
		pl->SetPoint(i, pos);
		Points.push_back( pos );
	}

	for( u32 i  =  0 ; i < MaxPoint; i++ ) {
		TCollisionSpr* col  = new TCollisionSpr(FOwner, this, X,Y,16,16);
		FOwner->Add(col);
		Collision.push_back(col);
	}

	Sprite[0]->Move(X,Y); //弾の初期位置を設定
	Sprite[0]->Rotate(Rdeg); //弾の方角を決定

}

TCurvedLaser::~TCurvedLaser()
{

}

void TCurvedLaser::Move()
{
	TOBJ::Move();
	switch( Pattern ){
		case 0:{
			switch( ActFlg ){
				case 0:{
					Sprite[0]->SlideMoveR(Speed, 0);
					Speed = Speed-Velocity/40.f;

					if( Speed<=0.5f ) ActFlg = 1;

				}break;
				case 1:{
					Sprite[0]->SlideMoveR(Speed, 0);
					Speed = Speed+Accel;
					Sprite[0]->RotateR(RSpeed);
				}break;
			}
		}
		case 1:{
			switch( ActFlg ){
				case 0:{
					Sprite[0]->SlideMoveR(Speed, 0);
					Speed = Speed+Accel;
					Sprite[0]->RotateR(RSpeed);
				}break;
			}
		}break;
	}


	X = (s32)(Sprite[0]->GetSpr()->FCX);
	Y = (s32)(Sprite[0]->GetSpr()->FCY);

	//  if( Age % 2 = 0 ) {
	for( u32 i  =  Points.size()-1; i >= 1; i-- ) {
		Points[i]  =  Points[i-1];
		pl->SetPoint(i, Points[i]);
		Collision[i]->Sprite[0]->Move(Points[i].x, Points[i].y);
	}
	//  }
	Points[0].x  =  X;
	Points[0].y  =  Y;
	pl->SetPoint(0, Points[0]);

	u32 last = Points.size() - 1;
	if( (Points[last].x<-32) || (Points[last].x>640+32) || (Points[last].y<-32) || (Points[last].y>480+32)) Die();
	if( (Age-LastBuzz)>=40 ) Buzzed = false;

}

void TCurvedLaser::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);
	if( target->Kind() == kHero ) {
		if( (! Buzzed) && (target->Hit[HitBuzz]) ) {
			FOwner->Scrach(this);
			Buzzed = true;
			LastBuzz = Age;
		}
	}

}

void TCurvedLaser::Die()
{
	pl->Die();

	for( u32 i  =  0 ; i < Collision.size()-1; i++ ) {
		Collision[i]->Die();
	}
	TOBJ::Die();

}

TSpreadBeam::TSpreadBeam( TOBJList* owner, s32 _x, s32 _y, s32 _dx, s32 _dy, f32 _speed ) : TOBJ(owner)
{
	FObjectName = "TSpreadBeam";
	X = _x;
	Y = _y;
	DestX = _dx;
	DestY = _dy;
	FKind = kEShot;
	Buzzed = false;
	LastBuzz = 0;
	FSTR = 500;
	Speed = _speed;
	ActFlg = 0;
	ActCount = 0;

	//本体
	MakeSpriteLite(0, X,Y,32,32,16000,bmAdd);
	Sprite[0]->GetSpr()->AddTexture(MechaTex, 224, 256, 287, 319);
	Sprite[0]->GetSpr()->AddHitArea(TPoint(8, 8), TPoint(8, 24), TPoint(24, 24),TPoint(24, 8), htSky);
	Sprite[0]->GetSpr()->FHit        =  false;
	Sprite[0]->GetSpr()->FCX += 16;
	Sprite[0]->GetSpr()->FCY += 16;
	Sprite[0]->GetSpr()->Move(X,Y);
	Sprite[0]->GetSpr()->Rotate(GetRfromArcTan(_x,_y,_dx,_dy));
}

void TSpreadBeam::Move()
{
	TOBJ::Move();
	X = (s32)(Sprite[0]->GetSpr()->FCX);
	Y = (s32)(Sprite[0]->GetSpr()->FCY);

	switch( ActFlg ){
		case 0:{
			Sprite[0]->SlideMoveR(Speed,0);
			if( CheckDistance(X,Y,X,DestY,16) ) {
				//      if( ActCount>5 ) {
				ActFlg = 1;
				ActCount = -1;
				Sprite[0]->GetSpr()->FHit = true;
				Sprite[0]->Zoom(5,5);
			}
		}break;
		case 1:{
			if( ActCount % 3 == 0 )
				Sprite[0]->Zoom(3,3);
			else
				Sprite[0]->Zoom(5,5);

			if( ActCount>60 ) {
				Sprite[0]->GetSpr()->FHit = false;
				ActFlg = 2;
				ActCount = -1;
			}
		}break;
		case 2:{
			Sprite[0]->ZoomR(0.98,0.98);
			Sprite[0]->GetSpr()->SetColorR(-8,ctAlpha);

			if( Sprite[0]->GetSpr()->GetColor(ctAlpha) == 0 ) Die();

		}break;
	}
	Inc(ActCount);
	if( (Age-LastBuzz)>=40 ) Buzzed = false;

}

void TSpreadBeam::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);
	if( target->Kind() == kHero ) {
		if( (! Buzzed) && (target->Hit[HitBuzz]) ) {
			FOwner->Scrach(this);
			Buzzed = true;
			LastBuzz = Age;
		}
	}

}

TSpreadBeam2::TSpreadBeam2( TOBJList* owner, s32 _x, s32 _y, s32 _R, f32 _speed ) : TOBJ(owner)
{
	FObjectName = "TSpreadBeam2";
	X = _x;
	Y = _y;
	FKind = kEffect;
	Speed = _speed;
	ActFlg = 0;
	ActCount = 0;

	//本体
	MakeSpriteLite(0, X,Y,32,32,5000,bmAdd);
	Sprite[0]->GetSpr()->AddTexture(Eff1Tex, 1, 1, 15, 16);
	//    FCX += 16;
	Sprite[0]->GetSpr()->FCY += 16;
	Sprite[0]->GetSpr()->Move(X,Y);
	Sprite[0]->GetSpr()->Zoom(3,1);
	Sprite[0]->GetSpr()->Rotate(_R);
}

void TSpreadBeam2::Move()
{
	TOBJ::Move();
	X = (s32)(Sprite[0]->GetSpr()->FCX);
	Y = (s32)(Sprite[0]->GetSpr()->FCY);

	switch( ActFlg ){
		case 0:{
			Sprite[0]->SlideMoveR(-Speed,0);
			if( (X < 32) || (Y>450) || (Y<32) ) {
				ActFlg = 1;
				ActCount = -1;
			}
		}break;
		case 1:{
			FOwner->Add(new TMissileFlash(FOwner,X,Y));

			s32 firstR = FOwner->GetRand(4095);

			if( FOwner->Difficulty == KDIFFICULTY_HARD ) {
				for( u32 i = 0; i <= 31; i++ )
					FOwner->Add(new TCurvedLaser(FOwner,X,Y,firstR+i*256,32, 5, 0.2f,5,1));
			}
			else if( FOwner->Difficulty == KDIFFICULTY_NORMAL ) {
				for( u32 i = 0; i <= 31; i++ )
					FOwner->Add(new TCurvedLaser(FOwner,X,Y,firstR+i*256,16, 5, 0.2f,5,1));
			}
			else {
				for( u32 i = 0; i <= 15; i++ )
					FOwner->Add(new TCurvedLaser(FOwner,X,Y,firstR+i*512,16, 5, 0.2f,5,1));
			}

			if( CreateSE ) FOwner->SoundEffect->WavPlay(WABeam,9);

			Die();
		}break;
	}
	Inc(ActCount);

}

TBeamRain::TBeamRain( TOBJList* owner, s32 _x, s32 _y, s32 _life, f32 _speed ) : TOBJ(owner)
{
	FObjectName = "TBeamRain";
	X = _x;
	Y = _y;
	FKind = kEffect2;
	FallSpeed = _speed;
	Life = _life;

}

void TBeamRain::Move()
{
	TOBJ::Move();

	s32 beaminterval;
	switch( FOwner->Difficulty ){
		case KDIFFICULTY_EASY:beaminterval = 12;		break;
		case KDIFFICULTY_HARD:beaminterval = 6;		break;
		default: beaminterval = 8;
	}

	if( Age % beaminterval == 0 ) {
		FOwner->Add(new TFunBeamCore(FOwner,FOwner->GetRand(36)*16,-32,1024,4));
		if( CreateSE ) FOwner->SoundEffect->WavPlay(WABeam, 9);
	}

	if( Age>=Life ) Die();
}

TZakoHomingCore3::TZakoHomingCore3( TOBJList* owner, s32 _x, s32 _y, s32 _r, f32 _speed, f32 _accel, s32 _pat ) : TOBJ(owner)
{
	FObjectName = "TZakoHomingCore3";
	X = _x;
	Y = _y;
	FKind = kEShot;
	Buzzed = false;
	LastBuzz = 0;
	FSTR = 500;
	const s32 MaxPoint = 16;

	ActFlg = 0;
	Rdeg = _r & 0xfff;
	Accel = _accel;
	Speed = _speed;
	Pattern = _pat;

	//本体
	MakeSpriteLite(0, X,Y,16,16,16000,bmAdd);
	Sprite[0]->GetSpr()->AddTexture(MechaTex, 223, 256, 160, 287);
	Sprite[0]->GetSpr()->FCX += 8;
	Sprite[0]->GetSpr()->FCY += 8;
	Sprite[0]->GetSpr()->Move(X,Y);
	Sprite[0]->GetSpr()->FVisible = false;

	Rect uv(256, 320, 319, 351);
	pl  =  new TPolyLine2D(FOwner, MaxPoint-2, 8, MechaTex,uv,bmAdd);
	FOwner->Add(pl);
	for( u32 i  =  0 ; i < MaxPoint; i++ ) {
		glm::vec2 pos(X, Y);
		pl->SetPoint(i, pos);
		Points.push_back( pos );
	}

	for( u32 i  =  0 ; i < MaxPoint; i++ ) {
		TCollisionSpr* col  = new TCollisionSpr(FOwner, this, X,Y,16,16);
		FOwner->Add(col);
		Collision.push_back(col);
	}

	Sprite[0]->Move(X,Y); //弾の初期位置を設定
	Sprite[0]->Rotate(Rdeg); //弾の方角を決定


}

TZakoHomingCore3::~TZakoHomingCore3()
{

}

void TZakoHomingCore3::Move()
{
	TOBJ::Move();

	X = (s32)(Sprite[0]->GetSpr()->FCX);
	Y = (s32)(Sprite[0]->GetSpr()->FCY);

	s32 Steering =  8+(Age / 2);

	s32 DX = FOwner->HeroPos.X-X;             //自分と敵との距離を計算
	s32 DY = FOwner->HeroPos.Y-Y;

	s32 EnemyDeg = RoundOff(atan2(DY,DX)/(f32)M_PI*2048.f) & 0x0fff;

	s32 Rdiff = GetDiffR( EnemyDeg, Rdeg);

	if( abs(Rdiff)>=2048 ) Inc(Rdeg, (s32)(-Sign(Rdiff)*ceil((f32)(4095-abs(Rdiff))/(f32)Steering)-Sign(Rdiff)));
	else if( abs(Rdiff)<2048 ) Dec(Rdeg, (s32)ceil((f32)(Rdiff)/(f32)Steering)+Sign(Rdiff));

	switch( Pattern ){
		case 0:{
			Sprite[0]->Rotate(Rdeg);
			Sprite[0]->SlideMoveR(Speed, 0);
			Speed = Speed+Accel;
		}break;
		case 1:{
			Sprite[0]->Rotate(Rdeg);
			Sprite[0]->SlideMoveR(Speed, 0);
			switch( ActFlg ){
				case 0:{
					Speed = Speed-Accel;

					if( Speed<1 )
						ActFlg = 1;
				}break;
				case 1:{
					Speed = Speed+Accel;
				}break;
			}break;
		}
	}

	//  if( Age % 2 = 0 ) {
	for( u32 i  =  Points.size()-1; i >= 1 ; i-- ) {
		Points[i]  =  Points[i-1];
		pl->SetPoint(i, Points[i]);
		Collision[i]->Sprite[0]->Move(Points[i].x, Points[i].y);
	}
	//  }
	Points[0].x  =  X;
	Points[0].y  =  Y;
	pl->SetPoint(0, Points[0]);

	u32 last = Points.size() - 1;
	if( (Points[last].x<-32) || (Points[last].x>640+32) || (Points[last].y<-32) || (Points[last].y>480+32)) Die();
	if( (Age-LastBuzz)>=40 ) Buzzed = false;

}

void TZakoHomingCore3::Die()
{
	pl->Die();

	for( u32 i  =  0 ; Collision.size(); i++) {
		Collision[i]->Die();
	}
	TOBJ::Die();

}

void TZakoHomingCore3::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);
	if( target->Kind() == kHero ) {
		if( (! Buzzed) && (target->Hit[HitBuzz]) ) {
			FOwner->Scrach(this);
			Buzzed = true;
			LastBuzz = Age;
		}
	}

}

TSlowBeam::TSlowBeam( TOBJList* owner, s32 _x, s32 _y, s32 _R, s32 _L, f32 _speed, f32 _zoom ) : TOBJ(owner)
{
	TBlendMode blend;
	FObjectName = "TSlowBeam";
	X = _x;
	Y = _y;
	FKind = kEShot;
	FSTR = 500;
	Buzzed = false;
	LastBuzz = 0;
	Life = _L;
	ZoomRate = _zoom;
	Speed = _speed;

	if( FOwner->NowStage == 0 )
		blend = bmNormal;
	else
		blend = bmAdd;

	MakeSpriteLite(0, X,Y,32,32,5000,blend);
	if( FOwner->NowStage == 0 )
		Sprite[0]->GetSpr()->AddTexture(Eff1Tex, 1, 193, 63, 255);
	else
		Sprite[0]->GetSpr()->AddTexture(MechaTex, 224, 256, 287, 319);
	Sprite[0]->GetSpr()->AddHitArea(TPoint(4, 4), TPoint(4, 28), TPoint(28, 28),TPoint(28, 4), htSky);
	Sprite[0]->GetSpr()->FHit = true;
	Sprite[0]->GetSpr()->FCX += 16;
	Sprite[0]->GetSpr()->FCY += 16;
	Sprite[0]->GetSpr()->Move(X,Y);
	Sprite[0]->GetSpr()->Zoom(ZoomRate,ZoomRate);
	Sprite[0]->GetSpr()->Rotate(_R);
}

void TSlowBeam::Move()
{
	TOBJ::Move();
	Sprite[0]->SlideMoveR(Speed,0);
	if( Age % 3 == 0 )
		Sprite[0]->GetSpr()->ZoomR(0.6f,0.6f);
	else
		Sprite[0]->GetSpr()->Zoom(ZoomRate,ZoomRate);

	if( (X > 640+32) || (Y > 480+32) || (Y <0-32) || (X < 0-32) ) Die();
	if( Age>Life ) Die();

	if( FOwner->BossDie ) Die();

}

void TSlowBeam::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);
	if( target->Kind() == kHero ) {
		if( (! Buzzed) && (target->Hit[HitBuzz]) ) {
			FOwner->Scrach(this);
			Buzzed = true;
			LastBuzz = Age;
		}
	}

}

TEraserCore::TEraserCore( TOBJList* owner, s32 sx, s32 sy ) : TOBJ(owner)
{
	FObjectName = "TEraserCore";
	X = sx;
	Y = sy;
	FSTR = 0;
	FKind = kEffect2;

	TSpriteEXBLT sblt;
		sblt.RBM      =  bmNone; // αブレンドモード
		sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
		sblt.RDDDD    =  FOwner->GetDG();     // TDDDDの参照
		sblt.RHeight  =  32;        // 高さ
		sblt.RWidth   =  32;        // 幅
		sblt.RX       =  X;        // X座標
		sblt.RY       =  Y;        // Y座標
		sblt.RZ       =  5000;       // Z値

	Sprite[0] =  new TJoint(sblt);         // スプライトの生成
	Inc(numSpr,1);

	Sprite[0]->GetSpr()->AddTexture(Eff2Tex,0, 128, 63, 191);    // テクスチャを登録
	Sprite[0]->GetSpr()->FVisible  =  false;
	Sprite[0]->GetSpr()->FCX += 16;
	Sprite[0]->GetSpr()->FCY += 16;
	Sprite[0]->Move(X,Y); //弾の初期位置を設定

}

void TEraserCore::Move()
{
	TOBJ::Move();

	if( Age<15 )
		Sprite[0]->MoveR(15-Age,0);

	X = RoundOff(Sprite[0]->GetSpr()->FCX);
	Y = RoundOff(Sprite[0]->GetSpr()->FCY);


	if( Age % 2 == 0 ) {
		FOwner->Add(new TBLight(FOwner,X-16,Y,8,2000));
	}
	else FOwner->Add(new TBLight(FOwner,X-16,Y,5,2000));

	if( Age % 40 == 0 ) FOwner->Add(new TSRControl(FOwner,X-16,Y,0));

	for( u32 i = 0; i <= 20; i++ )
		FOwner->Add(new TLight(FOwner,X-16*i,Y,i));

	for( u32 i = 0; i <= 30; i++ )
		FOwner->Add(new TLight(FOwner,X-16*(21+i),Y,20));

	if( Age % 4 == 0 ) {
		FOwner->Add(new TEraserPart(FOwner,X,Y));
	}

}

TEraserPart::TEraserPart( TOBJList* owner, s32 sx, s32 sy ) : TOBJ(owner)
{
	FObjectName = "TEraserPart";
	X = sx;
	Y = sy;
	FSTR = 100000;
	FKind = kEShot;

		TSpriteEXBLT sblt;
		sblt.RBM      =  bmSubAdd; // αブレンドモード
		sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
		sblt.RDDDD    =  FOwner->GetDG();     // TDDDDの参照
		sblt.RHeight  =  32;        // 幅
		sblt.RWidth   =  32;        // 高さ
		sblt.RX       =  X;        // X座標
		sblt.RY       =  Y;        // Y座標
		sblt.RZ       =  1000;       // Z値

	Sprite[0] =  new TJoint(sblt);         // スプライトの生成
	Inc(numSpr,1);
		Sprite[0]->GetSpr()->AddTexture(Eff2Tex,0, 128, 63, 191);    // テクスチャを登録
		Sprite[0]->GetSpr()->FVisible  =  true;
		Sprite[0]->GetSpr()->AddHitArea(TPoint(0, 0), TPoint(0, 32), TPoint(32, 32),TPoint(32, 0), htSky);
		Sprite[0]->GetSpr()->FHit      =  true;
		Sprite[0]->GetSpr()->FCX += 16;
		Sprite[0]->GetSpr()->FCY += 16;
		Sprite[0]->GetSpr()->Rotate(random(4095));
		Sprite[0]->GetSpr()->Zoom(2,1);
		Sprite[0]->GetSpr()->SetColor(128, ctRed);
		Sprite[0]->GetSpr()->SetColor(75,ctGreen);
		Sprite[0]->GetSpr()->SetColor(75,ctBlue);
		Sprite[0]->GetSpr()->SetColor(110,ctAlpha);
	Sprite[0]->Move(X,Y); //弾の初期位置を設定

}

void TEraserPart::Move()
{
	TOBJ::Move();
	Sprite[0]->MoveR(-40,0);
	if( Sprite[0]->GetSpr()->FZY<14.f )  Sprite[0]->GetSpr()->ZoomR(1.f, 1.5f);
	if( Sprite[0]->GetSpr()->FZX<15.f )  Sprite[0]->GetSpr()->ZoomR(1.5f, 1.f);

	X = RoundOff(Sprite[0]->GetSpr()->FCX);
	Y = RoundOff(Sprite[0]->GetSpr()->FCY);

	if( (X < 0-70) ) {
		Die();//弾が画面外に消えたら消す
		Dec(FOwner->MShipHP,5000);
	}

}

TEnergyBallBurst::TEnergyBallBurst( TOBJList* owner, s32 sx, s32 sy, s32 _amount, s32 _life, f32 _zoom ) : TOBJ(owner)
{
	FObjectName = "TEnergyBallBurst";
	X = sx;
	Y = sy;
	FKind = kEffect;
	Amount = _amount;
	Life = _life;
	ZoomRate = _zoom;

}

void TEnergyBallBurst::Move()
{
	TOBJ::Move();
	if( Age % 5 == 0 ) {
		s32 tx  =  RoundOff((f32)X + (f32)(FOwner->GetRand(64)-32)*ZoomRate);
		s32 ty  =  RoundOff((f32)Y + (f32)(FOwner->GetRand(64)-32)*ZoomRate);
		FOwner->Add(new TEnergyBall(FOwner, tx,ty,Life, ZoomRate));
		Dec(Amount);

		if( Amount<=0 ) Die();

	}

}

TZBeam2::TZBeam2( TOBJList* owner, TJoint* _parent, s32 sx, s32 sy, f32 _zoom ) : TOBJ(owner)
{
	FObjectName = "TZBeam2";
	X = sx;
	Y = sy;
	FKind = kEShot;
	FSTR = 1000;
	Buzzed = false;
	LastBuzz = 0;
	Parent = _parent;
	FIsParentExists  =  true;

	//ビーム根本
	MakeSpriteLite(numSpr, 0,0,64,64,12000,bmAdd);
	Sprite[numSpr-1]->GetSpr()->AddTexture(MechaTex, 1, 145, 64, 208);
	Sprite[numSpr-1]->GetSpr()->FCX += 32;
	Sprite[numSpr-1]->GetSpr()->FCY += 32;
	Sprite[numSpr-1]->GetSpr()->Move(X, Y);
	Sprite[numSpr-1]->GetSpr()->Zoom(4,4);
	Sprite[numSpr-1]->GetSpr()->SetColor(0,ctAlpha);
	_parent->Add(Sprite[numSpr-1]);

	//beam
	MakeSpriteLite(numSpr, 0,0,64,64,12000,bmAdd);
	Sprite[numSpr-1]->GetSpr()->AddTexture(MechaTex, 30, 145, 34, 208);
	Sprite[numSpr-1]->GetSpr()->AddHitArea(TPoint(0, 16), TPoint(0, 48), TPoint(64, 48),TPoint(64, 16), htSky);
	Sprite[numSpr-1]->GetSpr()->FHit      =  true;
	Sprite[numSpr-1]->GetSpr()->FCX += 64;
	Sprite[numSpr-1]->GetSpr()->FCY += 32;
	Sprite[numSpr-1]->GetSpr()->Move(X, Y);
	Sprite[numSpr-1]->GetSpr()->Zoom(10,_zoom);
	Sprite[numSpr-1]->GetSpr()->SetColor(0,ctAlpha);
	Sprite[0]->Add(Sprite[numSpr-1]);

}

void TZBeam2::Move()
{
	TOBJ::Move();

	if( Age % 3 == 0 )
		Sprite[0]->GetSpr()->ZoomR(0.9f, 0.9f);
	else
		Sprite[0]->GetSpr()->Zoom(4.f, 4.f);

	if( Sprite[0]->GetSpr()->GetColor(ctAlpha)<255 ) {
		Sprite[0]->GetSpr()->SetColorR(16,ctAlpha);
		Sprite[1]->GetSpr()->SetColorR(16,ctAlpha);
	}

	Sprite[0]->MoveR(FOwner->Speed - KSCROLLSPEED_NORMAL,0);

}

void TZBeam2::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);
	if( target->Kind() == kHero ) {
		if( (! Buzzed) && (target->Hit[HitBuzz]) ) {
			FOwner->Scrach(this);
			Buzzed = true;
			LastBuzz = Age;
		}
	}

}

void TZBeam2::Die()
{
	TOBJ::Die();
	if( Parent ) {
		Parent->Delete(Sprite[0]);       //親スプライトの多関節リストから自分を削除
	}

}

TZ01Shot4::TZ01Shot4( TOBJList* owner, s32 sx, s32 sy, s32 _R, s32 _S, f32 _IniXS, s32 _col, s32 _str ) : TOBJ(owner)
{
	FObjectName = "TZ01Shot4";
	X = sx;
	Y = sy;
	FSTR = _str;
	FKind = kEShot;
	Buzzed = false;
	LastBuzz = 0;
	Anim = false;
	Rdeg = _R;
	Speed = _S;
	IniXSpeed = _IniXS;
	BulletColor = _col;

	if( BulletColor == 0 ) {
		switch( FOwner->NowStage ){
		case 0:
		case 3:
			BulletColor = 1;
			break;
		default:
			BulletColor = 2;
		}
	}

	TSpriteEXBLT sblt;
		sblt.RBM      =  bmNormal; // αブレンドモード
		sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
		sblt.RDDDD    =  FOwner->GetDG();     // TDDDDの参照
		sblt.RHeight  =  32;        // 高さ
		sblt.RWidth   =  64;        // 幅
		sblt.RX       =  X;        // X座標
		sblt.RY       =  Y;        // Y座標
		sblt.RZ       =  TamaZ;       // Z値

	Inc(TamaZ);
	if( TamaZ>1099 ) TamaZ = 1000;

	Sprite[0] =  new TJoint(sblt);         // スプライトの生成
	Inc(numSpr,1);
	if( BulletColor == 1 )
		Sprite[0]->GetSpr()->AddTexture(Eff8Tex, 95, 192, 32, 223);
	else
		Sprite[0]->GetSpr()->AddTexture(Eff8Tex, 159, 192, 96, 223);

	Sprite[0]->GetSpr()->FVisible  =  true;
	Sprite[0]->GetSpr()->AddHitArea(TPoint(36, 10), TPoint(36, 22), TPoint(52, 22),TPoint(52, 10), htSky);
	Sprite[0]->GetSpr()->FHit      =  false;
	Sprite[0]->GetSpr()->FCX += 44;
	Sprite[0]->GetSpr()->FCY += 16;
	Sprite[0]->GetSpr()->Zoom(1,3);

	Sprite[0]->Move(X,Y); //弾の初期位置を設定
	Sprite[0]->Rotate(Rdeg); //弾の方角を決定


}

void TZ01Shot4::Move()
{
	TOBJ::Move();
	X = (s32)(Sprite[0]->GetSpr()->FCX);
	Y = (s32)(Sprite[0]->GetSpr()->FCY);

	Sprite[0]->SlideMoveR(Speed,0); //弾の移動
	Sprite[0]->MoveR(IniXSpeed,0);  //撃ったオブジェクトのスピードを足す

	if( Age % 3 == 0 ) Anim = ! Anim;

	if( Age == 3 ) Sprite[0]->GetSpr()->FHit = true;

	if( Age > 3 ) {
		Sprite[0]->Rotate(0);
		if( Anim ) Sprite[0]->GetSpr()->Zoom(1.f, 0.7f);
		else Sprite[0]->GetSpr()->Zoom(1.f, 1.0f);
		Sprite[0]->Rotate(Rdeg);
	}

	if( (Age-LastBuzz)>=40 ) Buzzed = false;

	if( Age>=240 ) {
		FOwner->Add(new TFCircle(FOwner,X,Y,1,Rdeg,Speed));
		Die();
	}

	if( (X > 640+128) || (Y > 480+128) || (Y <0-128) || (X < 0-128) ) Die();//弾が画面外に消えたら消す

}

void TZ01Shot4::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);
	if( target->Kind() == kHero ) {
		if( (! Buzzed) && (target->Hit[HitBuzz]) ) {
			FOwner->Scrach(this);
			Buzzed = true;
			LastBuzz = Age;
		}

		if( target->Hit[HitBody] ) {
			Die();
			FOwner->Add(new TExpSS(FOwner,X,Y,0,-10));
		}
	}

}
