#include "Friend.h"
#include "Hero.h"
#include "Effect.h"
#include "TOBJList.h"
#include "Gauge.h"
#include "Missile.h"
#include "Effect.h"
#include "Sound.h"


TFriend::TFriend( TOBJList* owner, s32 _x, s32 _y, f32 _zoom, s32 _type )	: TJiki(owner, _x, _y)
{
	const s32 ZP = 180;
	Age = 0;
	FOwner = owner;
	//LockNum = -1;
	FDead = false;
	numSpr = 0;
	distance = 0;
	TempHP = 0;
	FDef = 0;
	for( u32 i = 0 ; i < MaxJoint; i++ ) {
		Sprite[i] = NULL;
		Hit[i] = false;
	}
	BonusRate = 1;

	for( u32 i = 0; i <= 7; i++ )
		TempValue[i] = 0;

	FID = -1;

	X = _x;
	Y = _y;
	s16 tempX = X;
	s16 tempY = Y;
	HP = 0;
	Speed = 0;
	FKind = kEffect2;
	PrevShot = 0;
	MisOK = true;
	BlinkTime = 30;
	LastBlink = 0;
	BoostLV_friend = 1;
	BoostTime = 0;
	PrevX = 0;
	PrevY = 0;
	NokeTime = 0;
	CrushR = 0;
	BoostR = 0;
	LastMis = 0;
	MissileMuteki = false;
	ShowSpr = 0;
	ZoomRate = _zoom;
	ActType  =  _type;
	SlideX = 0;
	SlideY = 0;
	ShootFlg  =  -1;
	OwnerSprite = NULL;
	ActionID = 0;

	//IDの付与
	FID = NewID;
	Inc(NewID);

	s32 TYCorrect;
	if( FOwner->NowStage == 1 )
		TYCorrect  =  128;
	else
		TYCorrect  =  0;

	for( u32 i = 0; i <= 99; i++ ) {
		ActFlg[i] = 0;
		ActCount[i] = 0;
	}


	//  _num, TX, TY, TW, TH,    Z, HX, HY, HW, HH,_cx,_cy
	MakeSprite(QD, JikiTex, HBody,  2,1+TYCorrect, 46, 79,27+ZP, 29, 39,  2,  2, 30, 40,false);
	Sprite[HBody]->GetSpr()->Move(tempX+30,tempY+40);

	MakeSprite(QD, JikiTex, HRArm, 49,1+TYCorrect, 31, 31,21+ZP,  0,  0,  0,  0, 22, 14,false);
	Sprite[HRArm]->GetSpr()->Move(tempX+31,tempY+25);

	if( _type == 0 )
		MakeSprite(QD, JikiTex, HRHand,  161,33+TYCorrect, 63, 31,20+ZP,  0,  0,  0,  0, 24,  7,false);
	else
		MakeSprite(QD, JikiTex, HRHand ,  1,81+TYCorrect, 95, 31,20+ZP,  0,  0,  0,  0, 24,  7,false);

	Sprite[HRHand]->GetSpr()->Move(tempX+12,tempY+38);

	MakeSprite(QD, JikiTex, HLArm,129,1+TYCorrect, 15, 31,34+ZP,  0,  0,  0,  0,  5, 11,false);
	Sprite[HLArm]->GetSpr()->Move(tempX+37,tempY+26);

	Sprite[HLArm]->GetSpr()->SetColor(196,ctRed);
	Sprite[HLArm]->GetSpr()->SetColor(196,ctGreen);
	Sprite[HLArm]->GetSpr()->SetColor(196,ctBlue);

	MakeSprite(QD, JikiTex, HRLeg1, 81,1+TYCorrect, 31, 31,22+ZP,  0,  0,  0,  0, 17, 15,false);
	Sprite[HRLeg1]->GetSpr()->Move(tempX+25,tempY+44);

	MakeSprite(QD, JikiTex, HLLeg1, 81,1+TYCorrect, 31, 31,30+ZP,  0,  0,  0,  0, 17, 15,false);
	Sprite[HLLeg1]->GetSpr()->Move(tempX+31,tempY+44);

	MakeSprite(QD, JikiTex, HRLeg2, 113,1+TYCorrect, 15, 31,24+ZP,  0,  0,  0,  0, 10,  9,false);
	Sprite[HRLeg2]->GetSpr()->Move(tempX+24,tempY+47);

	MakeSprite(QD, JikiTex, HLLeg2, 113,1+TYCorrect, 15, 31,32+ZP,  0,  0,  0,  0, 10,  9,false);
	Sprite[HLLeg2]->GetSpr()->Move(tempX+30,tempY+47);

	MakeSprite(QD, JikiTex, HRLeg3, 49,33+TYCorrect, 63, 47,23+ZP,  0,  0,  0,  0, 38,  9,false);
	Sprite[HRLeg3]->GetSpr()->Move(tempX+18,tempY+62);

	MakeSprite(QD, JikiTex, HLLeg3, 49,33+TYCorrect, 63, 47,31+ZP,  0,  0,  0,  0, 38,  9,false);
	Sprite[HLLeg3]->GetSpr()->Move(tempX+24,tempY+62);

	MakeSprite(QD, JikiTex, HBuzz,  0,  0, 64, 64,20+ZP,  0,  0,  63,  63, 32, 32,false);
	Sprite[HBuzz]->GetSpr()->Move(tempX+34,tempY+28);

	Sprite[HBuzz]->GetSpr()->FVisible  =  false;

	MakeSprite(QD, JikiTex, HRCover,113,33+TYCorrect, 30, 30,25+ZP,  0,  0,  0,  0, 20, 24,false);
	Sprite[HRCover]->GetSpr()->Move(tempX+21,tempY+15);

	MakeSprite(QD, JikiTex, HLCover,113,33+TYCorrect, 30, 30,28+ZP,  0,  0,  0,  0, 20, 24,false);
	Sprite[HLCover]->GetSpr()->Move(tempX+27,tempY+15);

	MakeSprite(QD, JikiTex, HRWing,145,33+TYCorrect, 15, 31,26+ZP,  0,  0,  0,  0, 16, 24,false);
	Sprite[HRWing]->GetSpr()->Move(tempX+21,tempY+15);

	MakeSprite(QD, JikiTex, HLWing,145,33+TYCorrect, 15, 31,29+ZP,  0,  0,  0,  0, 16, 24,false);
	Sprite[HLWing]->GetSpr()->Move(tempX+27,tempY+15);

	MakeSprite(QD, JikiTex, HLHand,113,81+TYCorrect, 47, 15,33+ZP,  0,  0,  0,  0, 11,  4,false);
	Sprite[HLHand]->GetSpr()->Move(tempX+43,tempY+40);

	if( _type == 5 ) {
		MakeSprite(QD, JikiTex, HFlag,112,96+TYCorrect, 79, 31,26+ZP,  0,  0,  0,  0, 73,  12,false);
		Sprite[HFlag]->GetSpr()->Move(tempX+22,tempY+19);
		Sprite[HBody]->Add(Sprite[HFlag]);
		Sprite[HFlag]->Rotate(-600);
	}


	for( s32 i = 0 ; i < numSpr; i++ ) {
		// switch( i ){
		//   HLLeg1,HLLeg2,HLLeg3,HLCover,HLWing,HLShield:{
		Sprite[i]->GetSpr()->SetColor(240,ctRed);
		Sprite[i]->GetSpr()->SetColor(240,ctGreen);
		//          SetColor(144,ctBlue);
		Sprite[i]->GetSpr()->SetColor(196,ctAlpha);
		//   }
		// }
	}

	Sprite[HBody]->Add(Sprite[HRArm]);
	Sprite[HBody]->Add(Sprite[HLArm]);
	Sprite[HRArm]->Add(Sprite[HRHand]);
	Sprite[HBody]->Add(Sprite[HRLeg1]);
	Sprite[HBody]->Add(Sprite[HLLeg1]);
	Sprite[HBody]->Add(Sprite[HRLeg2]);
	Sprite[HBody]->Add(Sprite[HLLeg2]);
	Sprite[HRLeg2]->Add(Sprite[HRLeg3]);
	Sprite[HLLeg2]->Add(Sprite[HLLeg3]);
	Sprite[HBody]->Add(Sprite[HBuzz]);
	Sprite[HBody]->Add(Sprite[HRCover]);
	Sprite[HBody]->Add(Sprite[HLCover]);
	Sprite[HBody]->Add(Sprite[HRWing]);
	Sprite[HBody]->Add(Sprite[HLWing]);
	Sprite[HLArm]->Add(Sprite[HLHand]);

	Sprite[HLLeg2]->Rotate(-700);
	Sprite[HLLeg3]->RotateR(800);

	Sprite[HRCover]->Rotate(3200);
	Sprite[HLCover]->Rotate(3200);
	Sprite[HRWing]->Rotate(3200);
	Sprite[HLWing]->Rotate(3200);
	//Sprite[HLArm]->Rotate(480);
	//Sprite[HLShield]->Rotate(208);


	Sprite[HRWing]->GetSpr()->Zoom(0.3f,0.3f);
	Sprite[HLWing]->GetSpr()->Zoom(0.3f,0.3f);


	Sprite[HRArm]->Rotate(128);
	if( _type == 3 ) 
		Sprite[HRHand]->Rotate(256+random(512));
	else
		Sprite[HRHand]->Rotate(768);

	Sprite[HLArm]->Rotate(768);
	//Sprite[HLShield]->Rotate(1400);

	for( u32 i = 0; i <= 15; i++ ) {
		Sprite[i]->ZoomR(ZoomRate, ZoomRate);

		if( _type == 4 )
			Sprite[i]->GetSpr()->FZ -= 10000;
	}

}

void TFriend::Move()
{
	const s32 RandomTable[] = { -1,-1,0,1,1,0,-1,0,1,-1 };

	f32 tmpSpeed;
	switch( ActType ){
		case 0:{
			BodyControl(128,400,88,500,88);
			//BodyControl(256,3396+256,100+256,3992+256,3992+256);
			Sprite[HBody]->MoveR(-1.f*((f32)(FOwner->Speed-FOwner->Speed)*ZoomRate/1.5f),0.f);
		}break;
		case 1:{
			if( Age == 0 ) {
				Sprite[HRArm]->Rotate(256);
				Sprite[HBody]->Rotate(256);
				Sprite[HLLeg2]->Rotate(400);
				Sprite[HLLeg3]->Rotate(88);
				Sprite[HRLeg2]->Rotate(500);
				Sprite[HRLeg3]->Rotate(88);
				Sprite[HRHand]->Rotate(512);
			}

			BoostLV_friend = 3;
			Sprite[HBody]->MoveR((10.f+(f32)Age/2.f)*ZoomRate,0);
		}break;
		case 2:{
			switch( ActFlg[0] ){
				case 0:{
					Sprite[HBody]->Rotate(3840);
					Sprite[HLLeg2]->Rotate(3188);
					Sprite[HLLeg3]->Rotate(3132);
					Sprite[HRLeg2]->Rotate(3368);
					Sprite[HRLeg3]->Rotate(3272);
					Sprite[HRArm]->Rotate(0);
					Sprite[HRHand]->Rotate(0);

					Sprite[HBody]->Rotate(3300);
					ActFlg[0] = 1;
					ShootFlg = 0;
				}break;
				case 1:{
					tmpSpeed = (16.f - (f32)ActCount[0]/2.f)*cost[512];
					Sprite[HBody]->MoveR(-tmpSpeed,tmpSpeed);
					Inc(ActCount[0]);

					if( tmpSpeed<5 ) BodyControl(256,400,88,500,88);

					if( tmpSpeed<0 ) {
						ActCount[0] = 0;
						ActFlg[0] = 2;
						ShootFlg = -1;
						FOwner->Add(new TBeamControl(FOwner,440,-60,0));
					}
				}break;
				case 2:{
					BodyControl(256,400,88,500,88);
					MonoRotate(HRArm,256,16);
					MonoRotate(HRHand,512,16);
					MonoRotate(HLArm,Sprite[HBody]->Rdegree+480,8);
					MonoRotate(HLHand,Sprite[HBody]->Rdegree+208,8);
					if( ActCount[0] > 1 ) {
						ActCount[0] = 0;
						ActFlg[0] = 3;
						BoostLV_friend = 2;
						BoostTime = Age;
					}

					Inc(ActCount[0]);
				}break;
				case 3:{
					BodyControl(256,400,88,500,88);
					MonoRotate(HRArm,256,16);
					MonoRotate(HRHand,512,16);
					MonoRotate(HLArm,Sprite[HBody]->Rdegree+480,8);
					MonoRotate(HLHand,Sprite[HBody]->Rdegree+208,8);
					if( ActCount[0]>5 ) Sprite[HBody]->MoveR(((f32)(Speed+3+ActCount[0])/2.f)*ZoomRate,0);

					if( ActCount[0]>=14 ) {
						NokeTime = Age;
						CrushR = 3072;
						ActCount[0] = 0;
						ActFlg[0] = 4;
						BoostLV_friend = 1;
						FOwner->Add(new TExpSmall(FOwner,X,Y,0,0,0));//爆発
					}

					Inc(ActCount[0]);
				}break;
				case 4:{
					Sprite[HBody]->MoveR((f32)(Speed)*ZoomRate,0);
					Nokezori();
					if( ActCount[0]>15 ) Die();
					Inc(ActCount[0]);
				}break;
			}
		}break;
		//１面開始デモ
		case 3:{

			BodyControl(256,3396+256,100+256,3992+256,3992+256);
			Sprite[HBody]->MoveR(-(f32)(FOwner->Speed - Speed)*ZoomRate,0);
			Sprite[HBody]->MoveR(0.3f*ZoomRate,0);
		}break;
		case 4:{
			BodyControl(4240,3188,3132,3368,3272);

		}break;
		//隊長機
		case 5:{
			switch( ActFlg[0] ){
				case 0:{
					Sprite[HBody]->MoveR(-(f32)(FOwner->Speed - Speed)*ZoomRate,0);
				}break;
				case 1:{
					Sprite[HFlag]->RotateR(64);
					if( ActCount[0] == 20 ) {
						Inc(ActFlg[0]);
						ActCount[0]  =  0;
					}
				}break;
				case 2:{
					if( Age % 3 == 0 ) {
						if( ActCount[0]<10 )
							FOwner->Add(new TBeamFlag(FOwner,X,Y,1.5, Sprite[HFlag]));
						else
							FOwner->Add(new TBeamFlag(FOwner,X,Y,1.0, Sprite[HFlag]));
					}

					if( ActCount[0] == 60 ) {
						Inc(ActFlg[0]);
						ActCount[0]  =  0;
						BoostLV_friend = 2;
						BoostTime = Age;
					}
				}break;
				case 3:{
					BodyControl(256,400,88,500,88);
					MonoRotate(HRArm,256,16);
					MonoRotate(HRHand,512,16);
					MonoRotate(HLArm,Sprite[HBody]->Rdegree+480,8);
					MonoRotate(HLHand,Sprite[HBody]->Rdegree+208,8);
					if( Age % 3 == 0 ) FOwner->Add(new TBeamFlag(FOwner,X,Y,1, Sprite[HFlag]));
					if( ActCount[0] >4 ) Sprite[HBody]->MoveR(((f32)(Speed + ActCount[0])/2.f)*ZoomRate,0);

					if( X > 700 ) {
						Inc(ActFlg[0]);
						ActCount[0]  =  0;
					}
				}break;
				case 4:{
					if( ActCount[0] == 60 ) Disappear();
				}
			}
			Inc(ActCount[0]);
		}break;
		case 6:{
			//      BodyControl(128,400,88,500,88);

			switch( ActFlg[0] ){
				case 0:{
					Sprite[HBody]->MoveR(-(f32)(FOwner->Speed - Speed)*ZoomRate,0);
				}break;
				case 1:{
					if( ActCount[0] >4 ) Sprite[HBody]->MoveR((f32)(Speed + ActCount[0])*ZoomRate,0);
					if( X > 700 ) {
						Disappear();
					}
				}break;
			}
			Inc(ActCount[0]);
		}break;
		case 7:{

			switch( ActFlg[0] ){
				case 0:{

				}break;
				case 1:{
					if( ActCount[0] >4 ) Sprite[HBody]->MoveR((f32)(Speed/2 + ActCount[0])*ZoomRate,-(f32)(Speed/2 + ActCount[0])*ZoomRate);
				}break;
			}
			Inc(ActCount[0]);
		}break;
		case 8:{
			switch( ActFlg[0] ){
				case 0:{
					DoAction(KACTION_FRIEND_BOOST);
				}break;
				case 1:{
					if( ActCount[0] >4 ) Sprite[HBody]->MoveR((f32)(Speed + ActCount[0])*ZoomRate,0);
					if( X > 700 ) {
						Disappear();
					}
				}break;
			}
			Inc(ActCount[0]);
		}break;
	}

	//  Sprite[HBody]->MoveR(FOwner->Speed-FOwner->Speed*ZoomRate/1.5,0);

	if( (ShootFlg == 0) && (Age-PrevShot>4) ) MakeShot();
	//  Input;
	//リコイル
	switch( (Age-PrevShot) ){
		case 0:{ Sprite[HRHand]->SlideMoveR(-3,0); Sprite[HRArm]->MoveR(-1,0); Sprite[HBody]->MoveR(-1,0); }
		case 2:{ Sprite[HRHand]->SlideMoveR(3,0);  Sprite[HRArm]->MoveR(1,0);  Sprite[HBody]->MoveR(1,0); }
	}


	//機体を揺らしてスピード感をアピール
	//  if( (BoostLV_friend=3) ) Sprite[HBody]->MoveR(RandomTable[Age % 9]*ZoomRate,RandomTable[9-(Age % 9)]*ZoomRate);

	X = (s32)(Sprite[HBody]->GetSpr()->FCX);
	Y = (s32)(Sprite[HBody]->GetSpr()->FCY);

	HeroJet();

	PrevX = X-KazeSpeed;
	PrevY = Y;

	Inc(Age);

}

void TFriend::Die()
{
	FOwner->Add(new TExplarge(FOwner,X,Y,0,true));//爆発
	FDead = true;

	if( OwnerSprite!=NULL ) {
		OwnerSprite->Delete(Sprite[HBody]);       //親スプライトの多関節リストから自分を削除
	}

}

void TFriend::MakeShot()
{
	f32 XX = Sprite[HRHand]->GetSpr()->FCX-8.f*ZoomRate;
	f32 YY = Sprite[HRHand]->GetSpr()->FCY+0*ZoomRate;
	PosRoll(XX, YY,Sprite[HRHand]->GetSpr()->FCX,Sprite[HRHand]->GetSpr()->FCY,Sprite[HRHand]->Rdegree);
	FOwner->Add(new TShot(FOwner,(s32)(XX),(s32)(YY),Sprite[HRHand]->Rdegree,ZoomRate));
	XX = Sprite[HRHand]->GetSpr()->FCX+64.f*ZoomRate;
	YY = Sprite[HRHand]->GetSpr()->FCY-3.f*ZoomRate;
	PosRoll(XX, YY,Sprite[HRHand]->GetSpr()->FCX,Sprite[HRHand]->GetSpr()->FCY,Sprite[HRHand]->Rdegree);
	FOwner->Add(new TShotFire(FOwner,(s32)(XX),(s32)(YY),1024+Sprite[HRHand]->Rdegree,ZoomRate));
	PrevShot = Age;
	if( CreateSE ) FOwner->SoundEffect->WavPlay(WAGunshot,2);

}

void TFriend::HeroJet()
{
	s32 RR;
	f32 XX, YY;
	s32 reaction;
	const s32 LIGHT_Z = 18000;
	//バーニア
	switch( BoostLV_friend ){
		case 1:{ BFControl(3200+Sprite[HBody]->Rdegree,0.3f); Speed = 5; }	break;
		case 2:{
			Speed = 10;
			BFControl(0+Sprite[HBody]->Rdegree,1.0f);

			//輪っか生成
			if( (Age-BoostTime) == 5 ) {
				RR = Sprite[HBody]->Rdegree & 0x0FFF;
				XX = Sprite[HBody]->GetSpr()->FCX-23.f*ZoomRate;
				YY = Sprite[HBody]->GetSpr()->FCY-16.f*ZoomRate;
				PosRoll(XX, YY,X,Y,RR);
				FOwner->Add(new TSRControl(FOwner,(s32)(XX)+32,(s32)(YY)-32,BoostR));
				if( CreateSE ) FOwner->SoundEffect->WavPlay(WABoost,1);
			}

			if( (Age-BoostTime)>5 ) {
				if( (Age-BoostTime)<=21 ) {
					RR = Sprite[HRWing]->Rdegree & 0x0FFF;
					XX = (s32)(Sprite[HRWing]->GetSpr()->FCX-9.f*ZoomRate);
					YY = (s32)(Sprite[HRWing]->GetSpr()->FCY+6.f*ZoomRate);
					PosRoll(XX, YY,Sprite[HRWing]->GetSpr()->FCX,Sprite[HRWing]->GetSpr()->FCY,RR);
					//反動
					reaction = 21-(Age-BoostTime);
					Slide((f32)reaction*cost[BoostR], (f32)reaction*sint[BoostR]);
					//火花
					FOwner->Add(new TParticle2(FOwner,(s32)(XX),(s32)(YY)-32,0,2048+(random(600)-300),5+random(10),5+random(10)));
					FOwner->Add(new TParticle2(FOwner,(s32)(XX),(s32)(YY)-32,0,2048+(random(600)-300),5+random(10),5+random(10)));
				}
				//噴射
				RR = Sprite[HRWing]->Rdegree & 0x0FFF;
				XX = (s32)(Sprite[HRWing]->GetSpr()->FCX-9.f*ZoomRate);
				YY = (s32)(Sprite[HRWing]->GetSpr()->FCY-4.f*ZoomRate);
				PosRoll(XX, YY,Sprite[HRWing]->GetSpr()->FCX,Sprite[HRWing]->GetSpr()->FCY,RR);
				FOwner->Add(new TBLight(FOwner,XX + SlideX,YY + SlideY,ZoomRate,LIGHT_Z));
				if( Age % 4 == 0 ) {
					XX = Sprite[HRWing]->GetSpr()->FCX-9.f*ZoomRate;
					YY = Sprite[HRWing]->GetSpr()->FCY-8.f*ZoomRate;
					PosRoll(XX,YY,Sprite[HRWing]->GetSpr()->FCX,Sprite[HRWing]->GetSpr()->FCY,RR);
					FOwner->Add(new TBFire(FOwner,(s32)(XX),(s32)(YY),(RR+412) & 0x0fff,BoostLV_friend,ZoomRate,this));
					XX = Sprite[HRWing]->GetSpr()->FCX-7*ZoomRate;
					YY = Sprite[HRWing]->GetSpr()->FCY-4*ZoomRate;
					PosRoll(XX, YY,Sprite[HRWing]->GetSpr()->FCX,Sprite[HRWing]->GetSpr()->FCY,RR);
					FOwner->Add(new TBFire(FOwner,(s32)(XX),(s32)(YY),(RR+200) & 0x0fff,BoostLV_friend,ZoomRate,this));
					XX = Sprite[HRWing]->GetSpr()->FCX-5*ZoomRate;
					YY = Sprite[HRWing]->GetSpr()->FCY+2*ZoomRate;
					PosRoll(XX, YY,Sprite[HRWing]->GetSpr()->FCX,Sprite[HRWing]->GetSpr()->FCY,RR);
					FOwner->Add(new TBFire(FOwner,(s32)(XX),(s32)(YY),(RR) & 0x0fff,BoostLV_friend,ZoomRate,this));
				}
			}
			if( (Age-BoostTime)>21 ) BoostLV_friend = 3;
		}break;
		case 3:{
			Speed = 10;
			BFControl(0+Sprite[HBody]->Rdegree,1.0);
			
			//噴射
			RR = Sprite[HRWing]->Rdegree & 0x0FFF;
			XX = (s32)(Sprite[HRWing]->GetSpr()->FCX-9.f*ZoomRate);
			YY = (s32)(Sprite[HRWing]->GetSpr()->FCY-4.f*ZoomRate);
			PosRoll(XX, YY,Sprite[HRWing]->GetSpr()->FCX,Sprite[HRWing]->GetSpr()->FCY,RR);
			FOwner->Add(new TBLight(FOwner,XX + SlideX,YY + SlideY,ZoomRate,LIGHT_Z));
			if( Age % 4 == 0 ) {
				XX = Sprite[HRWing]->GetSpr()->FCX-9.f*ZoomRate;
				YY = Sprite[HRWing]->GetSpr()->FCY-8.f*ZoomRate;
				PosRoll(XX, YY,Sprite[HRWing]->GetSpr()->FCX,Sprite[HRWing]->GetSpr()->FCY,RR);
				FOwner->Add(new TBFire(FOwner,(s32)(XX),(s32)(YY),(RR+412) & 0x0fff,BoostLV_friend,ZoomRate,this));
				XX = Sprite[HRWing]->GetSpr()->FCX-7.f*ZoomRate;
				YY = Sprite[HRWing]->GetSpr()->FCY-4.f*ZoomRate;
				PosRoll(XX, YY,Sprite[HRWing]->GetSpr()->FCX,Sprite[HRWing]->GetSpr()->FCY,RR);
				FOwner->Add(new TBFire(FOwner,(s32)(XX),(s32)(YY),(RR+200) & 0x0fff,BoostLV_friend,ZoomRate,this));
				XX = Sprite[HRWing]->GetSpr()->FCX-5.f*ZoomRate;
				YY = Sprite[HRWing]->GetSpr()->FCY+2.f*ZoomRate;
				PosRoll(XX, YY,Sprite[HRWing]->GetSpr()->FCX,Sprite[HRWing]->GetSpr()->FCY,RR);
				FOwner->Add(new TBFire(FOwner,(s32)(XX),(s32)(YY),(RR) & 0x0fff,BoostLV_friend,ZoomRate,this));
			}
		}break;
		case 4:{
			Speed = 5;
			BFControl(3200+Sprite[HBody]->Rdegree,0.3f);

			if( (Age-BoostTime)>10 ) {
				//噴射
				XX = (s32)(Sprite[HRLeg3]->GetSpr()->FCX-27.f*ZoomRate);
				YY = (s32)(Sprite[HRLeg3]->GetSpr()->FCY+25.f*ZoomRate);
				RR = Sprite[HRLeg3]->Rdegree & 0x0FFF;
				PosRoll(XX, YY,Sprite[HRLeg3]->GetSpr()->FCX,Sprite[HRLeg3]->GetSpr()->FCY,RR);
				FOwner->Add(new TBFire(FOwner,(s32)(XX),(s32)(YY),(RR-400) & 0x0fff,BoostLV_friend,ZoomRate,this));
				FOwner->Add(new TBLight(FOwner,XX,YY,ZoomRate,LIGHT_Z));
				XX = (s32)(Sprite[HLLeg3]->GetSpr()->FCX-27.f*ZoomRate);
				YY = (s32)(Sprite[HLLeg3]->GetSpr()->FCY+25.f*ZoomRate);
				RR = Sprite[HLLeg3]->Rdegree & 0x0FFF;
				PosRoll(XX, YY,Sprite[HLLeg3]->GetSpr()->FCX,Sprite[HLLeg3]->GetSpr()->FCY,RR);
				FOwner->Add(new TBFire(FOwner,(s32)(XX),(s32)(YY),(RR-400) & 0x0fff,BoostLV_friend,ZoomRate,this));
				FOwner->Add(new TBLight(FOwner,XX,YY,ZoomRate,LIGHT_Z));
			}
			if( (Age-BoostTime)>30 ) BoostLV_friend = 1;
		}
	}

	RR = Sprite[HBody]->Rdegree & 0x0FFF;
	XX = Sprite[HBody]->GetSpr()->FCX-23.f*ZoomRate;
	YY = Sprite[HBody]->GetSpr()->FCY-16.f*ZoomRate;
	PosRoll(XX, YY,X,Y,RR);
	FOwner->Add(new TBLight(FOwner,XX + SlideX,YY + SlideY,ZoomRate,LIGHT_Z));

	if( Age % 4 == 0 )
		FOwner->Add(new TBFire(FOwner,(s32)(XX),(s32)(YY),RR-384,BoostLV_friend,ZoomRate,this));
}

void TFriend::Slide( f32 _x, f32 _y )
{
	Sprite[HBody]->MoveR(_x*ZoomRate,_y*ZoomRate);
}

void TFriend::BoostSoon( bool _boost )
{
	BoostLV_friend  =  3;

	Sprite[HRArm]->Rotate(0);
	Sprite[HRHand]->Rotate(0);
	Sprite[HLArm]->Rotate(Sprite[HBody]->Rdegree+480);
	Sprite[HLHand]->Rotate(Sprite[HBody]->Rdegree+208);

}

void TFriend::Disappear()
{
	FDead = true;
}

void TFriend::SetOwnerSprite( TJoint* _spr )
{
	_spr->Add(Sprite[HBody]);
	OwnerSprite = _spr;

}

void TFriend::DoAction( s32 _num )
{
	switch( _num ){
	case KACTION_FRIEND_BOOST:
		{
			ActFlg[0] = 1;
			ActCount[0]  =  0;
			BoostLV_friend = 2;
			BoostTime = Age;
		}break;
	case KACTION_FRIEND_FLAG:
		{
			if( ActType == 5 ) {
				ActFlg[0] = 1;
				ActCount[0]  =  0;
			}
		}break;
	}
}

TBeamFlag::TBeamFlag( TOBJList* owner, s32 _x, s32 _y, f32 _zoom, TJoint* _spr ) : TOBJ(owner)
{
	X = _x;
	Y = _y;
	FObjectName = "TBeamFlag";
	OwnerSprite = _spr;
	FKind = kEffect;

	MakeSpriteLite(0, 0,0,160,80,30000,bmAdd);
	Sprite[0]->GetSpr()->AddTexture(GaugeTex, 289, 145, 448, 224);   // テクスチャを登録
	Sprite[0]->GetSpr()->FCX += 144;
	Sprite[0]->GetSpr()->FCY += 67;
	Sprite[0]->GetSpr()->Zoom(_zoom, _zoom*2);
	Sprite[0]->GetSpr()->Move(X -30, Y -40);
	Sprite[0]->GetSpr()->SetColor(128, ctAlpha);

	_spr->Add(Sprite[0]);
}

void TBeamFlag::Move()
{
	TOBJ::Move();
	Sprite[0]->ZoomR(1.0f,0.9f);
	Sprite[0]->GetSpr()->SetColorR(-8, ctAlpha);

	if( Sprite[0]->GetSpr()->GetColor(ctAlpha) == 0 )
		Die();

}

void TBeamFlag::Die()
{
	TOBJ::Die();
	if( OwnerSprite!=NULL ) {
		OwnerSprite->Delete(0);       //親スプライトの多関節リストから自分を削除
	}
}

TYarareZako::TYarareZako( TOBJList* owner, s32 _x, s32 _y, f32 _zoom ) : TOBJ(owner)
{
	X = _x;
	Y = _y;
	ActCount = 0;
	ActFlg = 0;
	FKind = kEffect2;

	MakeSprite(FOwner->GetDG(), MechaTex,  0,1,65,192, 80,160,  0,  0,  0,  0, 128, 40,false);
	Sprite[0]->GetSpr()->Move(_x,_y);
	Sprite[0]->ZoomR(_zoom,_zoom);
	Sprite[0]->SetDest(450,200,32);

}

void TYarareZako::Move()
{
	TOBJ::Move();
	X = (s32)(Sprite[0]->GetSpr()->FCX);
	Y = (s32)(Sprite[0]->GetSpr()->FCY);

	Sprite[0]->AutoSlide();

}

void TYarareZako::Die()
{
	TOBJ::Die();
	FOwner->Add(new TExplarge(FOwner,X,Y,0,true));//爆発

}

TFriend2::TFriend2( TOBJList* owner, s32 _x, s32 _y, f32 _zoom, s32 _type )	: TOBJ(owner)
{
	X = _x;
	Y = _y;
	HP = 0;
	FKind = kEffect2;
	BoostLV_friend = 1;
	ActType  =  _type;
	ActionID = 0;
	ZoomRate = _zoom;

	for( u32 i = 0; i <= 99; i++ ) {
		ActFlg[i] = 0;
		ActCount[i] = 0;
	}

	MakeSprFromID(FOwner->GetDG(), KSPRDATA_FRIEND, JikiTex, 24000, false);
	MotionControlSoon(KMOTION_FRIEND_SWING1);

	for( s32 i = 0 ; i < numSpr; i++ ) {
		Sprite[i]->GetSpr()->SetColor(240,ctRed);
		Sprite[i]->GetSpr()->SetColor(240,ctGreen);
		Sprite[i]->GetSpr()->SetColor(196,ctAlpha);
	}

	for( s32 i = 0 ; i < numSpr; i++ ) {
		Sprite[i]->ZoomR(ZoomRate, ZoomRate);
	}

	ActFlg[KACTION_BLADE] = 1;

	zako  =   new TYarareZako(FOwner,700,200,0.85f);
	FOwner->Add(zako);

}

void TFriend2::Move()
{
	TOBJ::Move();
	if( ActFlg[KACTION_BLADE]>0 ) BladeMotion();

}

void TFriend2::Die()
{
	TOBJ::Die();
}

void TFriend2::Slide( f32 _x, f32 _y )
{
	Sprite[HBody]->MoveR(_x*ZoomRate,_y*ZoomRate);
}

void TFriend2::Disappear()
{

}

void TFriend2::DoAction( s32 _num )
{

}

void TFriend2::BladeMotion()
{
	switch( ActFlg[KACTION_BLADE] ){
		//
		case 1:{
			if( ActCount[KACTION_BLADE] > 30 ) {
				Inc(ActFlg[KACTION_BLADE]);
				ActCount[KACTION_BLADE]  =  -1;

				Sprite[0]->SetDest(450,160,16);
			}
		}break;
		//
		case 2:{
			Sprite[0]->AutoSlide();
			if( ActCount[KACTION_BLADE] > 25 )
				MotionControl(KMOTION_FRIEND_SWING2);

			if( ActCount[KACTION_BLADE] == 30 ) {
				if( CreateSE ) FOwner->SoundEffect->WavPlay2(WASlash_A,12,80);
				FOwner->Add(new TSlashedEffect(FOwner, 450,200, -512,1));
			}

			if( ActCount[KACTION_BLADE] > 70 ) {
				Inc(ActFlg[KACTION_BLADE]);
				ActCount[KACTION_BLADE]  =  -1;

				Sprite[0]->SetDest(640,-100,16);

				if( CreateSE ) FOwner->SoundEffect->WavPlay2(WASlash_A,12,80);
				FOwner->Add(new TSlashedEffect(FOwner, 450,200, 512,1));
			}
		}break;
		case 3:{
			Sprite[0]->AutoSlide();
			MotionControl(KMOTION_FRIEND_SWING3);

			if( ActCount[KACTION_BLADE] == 10 )
				zako->Die();

			if( ActCount[KACTION_BLADE] > 60 ) {
				Inc(ActFlg[KACTION_BLADE]);
				ActCount[KACTION_BLADE]  =  -1;
			}
		}break;
		case 4:{
			if( ActCount[KACTION_BLADE] == 0 )
				FOwner->Add(new TFriend(FOwner, -100,150, 0.75f,8));

			if( ActCount[KACTION_BLADE] == 30 )
				FOwner->Add(new TFriend(FOwner, -100,300, 0.95f,8));

			if( ActCount[KACTION_BLADE] == 31 )
				Die();
		}
	}

	Inc(ActCount[KACTION_BLADE]);

}

