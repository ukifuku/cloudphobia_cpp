#include "Zako02.h"
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

#define		KCOLOR_ORG  240


TTransGene::TTransGene( TOBJList* owner ) : TOBJ(owner)
{
	FObjectName = "TTransGene";
	FKind = kGene;
	Count = 0;
}

void TTransGene::Move()
{
	if( FOwner->Difficulty == KDIFFICULTY_EASY ) {
		FOwner->Add(new TTransformer(FOwner,550,0,0,1,false));
		FOwner->Add(new TTransformer(FOwner,400,-200,0,3,false));
	}
	else {
		FOwner->Add(new TTransformer(FOwner,50,0,0,0,false));
		FOwner->Add(new TTransformer(FOwner,550,0,0,1,false));
		FOwner->Add(new TTransformer(FOwner,200,-200,0,2,false));
		FOwner->Add(new TTransformer(FOwner,400,-200,0,3,true));
	}

	Die();
	TOBJ::Move();

}

TTransformer::TTransformer( TOBJList* owner, s32 _x, s32 _y, s32 _r, s32 _pat, bool _red ) : TOBJ(owner)
{
	FObjectName = "TTransformer";

	X = _x;
	Y = _y;
	MaxHP = KSTR_SHOT*48;
	if( _red ) MaxHP = 480;
	HP = MaxHP;
	TempHP = HP;
	if( (FOwner->IsSequence()) )
		FSTR = 500;
	else
		FSTR = 100;
	FKind = kEnemy;
	Score = 40000;
	Buzzed = false;
	LastBuzz = 0;
	Pattern = _pat;
	CommandRed = _red;
	GaugeTime = 0;
	TotalFlight = 0;
	ActFlg = 11;
	ActCount = 0;
	LastFlash = 0;
	BeamON = false;
	BeamCount = 0;
	GunR = 2048;
	EnemyDeg = 0;
	Boost = false;
	BoostCount = 0;
	BSpeed = 0;
	NowSpr = 0;
	TransState = true;
	TransCount = 0;
	TransForm = false;
	XCycle = 0;
	YCycle = 1024;
	XSpeed = 0;
	YSpeed = 8;
	Xdist = 0;
	Ydist = 0;
	IniDis = 0;
	DestDeg = 0;
	ComBonus = 5000;

	//IDの付与
	FID = NewID;
	Inc(NewID);

	Vapor = new TVaporControl(FOwner,1);

	TransTiming = _pat+1;
	switch( _pat ){
		case 0:{
			HPosList[0] = TPoint(512,320);  PPosList[0] = TPoint(160,160);
			HPosList[1] = TPoint(592,160);  PPosList[1] = TPoint(320,400);
			HPosList[2] = TPoint(512,320);  PPosList[2] = TPoint(160,160);
			HPosList[3] = TPoint(592,160);  PPosList[3] = TPoint(320,400);
		}break;
		case 1:{
			HPosList[0] = TPoint(592,240);  PPosList[0] = TPoint(160,320);
			HPosList[1] = TPoint(512,400);  PPosList[1] = TPoint(480,80);
			HPosList[2] = TPoint(592,240);  PPosList[2] = TPoint(160,320);
			HPosList[3] = TPoint(512,400);  PPosList[3] = TPoint(480,80);
		}break;
		case 2:{
			HPosList[0] = TPoint(576,120);  PPosList[0] = TPoint(480,240);
			HPosList[1] = TPoint(544,40);  PPosList[1] = TPoint(240,240);
			HPosList[2] = TPoint(576,120);  PPosList[2] = TPoint(480,240);
			HPosList[3] = TPoint(544,40);  PPosList[3] = TPoint(240,240);
		}break;
		case 3:{
			HPosList[0] = TPoint(512,96);  PPosList[0] = TPoint(128,96);
			HPosList[1] = TPoint(576,288);  PPosList[1] = TPoint(64,288);
			HPosList[2] = TPoint(544,192);  PPosList[2] = TPoint(64,192);
			HPosList[3] = TPoint(512,384);  PPosList[3] = TPoint(128,384);
		}break;
	}

	PrevX = 0;
	PrevY = 0;
	if( _red ){
		if( FOwner->Difficulty <= KDIFFICULTY_NORMAL )
			Retreat = 43000;
		else
			Retreat = 47800;
	}
	else{
		if( FOwner->Difficulty <= KDIFFICULTY_NORMAL )
			Retreat = 29000;
		else
			Retreat = 26200;
	}

	TimCount = 0;

	s32 tempX = X+14;
	s32 tempY = Y+2;
	// _num,TX,TY,TW,TH,Z,HX,HY,HW,HH,_cx,_cy
	MakeSprite(FOwner->GetDG(), MechaTex,THip, 0,32+224,31,31,154,0,0,25,27,14,2,_red);
	Sprite[THip]->GetSpr()->Move(tempX,tempY);

	MakeSprite(FOwner->GetDG(), MechaTex,THead, 0,0+224,31,31,160,0,0,16,16,18,15,_red);
	Sprite[THead]->GetSpr()->Move(tempX,tempY-32);

	MakeSprite(FOwner->GetDG(), MechaTex,TBody, 32,0+224,31,47,158,0,0,22,33,13,34,_red);
	Sprite[TBody]->GetSpr()->Move(tempX,tempY);

	MakeSprite(FOwner->GetDG(), MechaTex,TLShoulder, 64,0+224,31,47,152,0,0,0,0,6,12,_red);
	Sprite[TLShoulder]->GetSpr()->Move(tempX+1,tempY-26);

	MakeSprite(FOwner->GetDG(), MechaTex,TLArm, 32,48+224,47,15,142,0,0,0,0,43,6,_red);
	Sprite[TLArm]->GetSpr()->Move(tempX+1,tempY-3);

	MakeSprite(FOwner->GetDG(), MechaTex,TRShoulder, 64,0,31,47,154,0,0,0,0,6,12,_red);
	Sprite[TRShoulder]->GetSpr()->Move(tempX+1,tempY-26);
	Sprite[TRShoulder]->GetSpr()->FVisible = false;


	MakeSprite(FOwner->GetDG(), MechaTex,TRArm, 32,48,47,15,155,0,0,0,0,43,6,_red);
	Sprite[TRArm]->GetSpr()->Move(tempX+1,tempY-3);
	Sprite[TRArm]->GetSpr()->FVisible = false;

	MakeSprite(FOwner->GetDG(), MechaTex,TRLeg1, 96,0+224,31,47,164,1,0,20,34,6,7,_red);
	Sprite[TRLeg1]->GetSpr()->Move(tempX,tempY+16);

	MakeSprite(FOwner->GetDG(), MechaTex,TRLeg2, 128,0+224,31,95,166,5,0,20,56,3,3,_red);
	Sprite[TRLeg2]->GetSpr()->Move(tempX+10,tempY+40);

	MakeSprite(FOwner->GetDG(), MechaTex,TLLeg1, 96,0+224,31,47,148,1,0,20,34,6,7,_red);
	Sprite[TLLeg1]->GetSpr()->Move(tempX,tempY+16);

	MakeSprite(FOwner->GetDG(), MechaTex,TLLeg2, 128,0+224,31,95,150,5,0,20,56,3,3,_red);
	Sprite[TLLeg2]->GetSpr()->Move(tempX+10,tempY+40);

	MakeSprite(FOwner->GetDG(), MechaTex,TCWing, 160,0+224,79,31,162,0,0,0,0,3,31,_red);
	Sprite[TCWing]->GetSpr()->Move(tempX+7,tempY-27);

	MakeSprite(FOwner->GetDG(), MechaTex,TRWing, 160,32+224,95,31,167,0,0,0,0,21,14,_red);
	Sprite[TRWing]->GetSpr()->Move(tempX-1,tempY+18);


	MakeSprite(FOwner->GetDG(), MechaTex,TLWing, 160,32+224,95,31,140,0,0,0,0,21,14,_red);
	Sprite[TLWing]->GetSpr()->Move(tempX-1,tempY+18);
	//Sprite[TLWing]->GetSpr()->FVisible = false;

	MakeSprite(FOwner->GetDG(), MechaTex,TGun1, 0,96+224,95,31,144,0,0,0,0,65,1,_red);
	Sprite[TGun1]->GetSpr()->Move(tempX-31,tempY-1);

	MakeSprite(FOwner->GetDG(), MechaTex,TGun2, 96,96+224,79,31,146,0,0,0,0,0,0,_red);
	Sprite[TGun2]->GetSpr()->Move(tempX-31,tempY-1);

	MakeSprite(FOwner->GetDG(), MechaTex,TNose, 0,64+224,95,31,156,0,0,0,0,3,13,_red);
	Sprite[TNose]->GetSpr()->Move(tempX+1,tempY-28);

	if( ExtraEffects ) {

		// _num,TX,TY,TW,TH,Z,HX,HY,HW,HH,_cx,_cy
		MakeSprite(FOwner->GetDG(), BumpTex,THipR, 0,32+224,31,31,153,0,0,25,27,14,2,false);
		Sprite[THipR]->GetSpr()->Move(tempX,tempY);

		MakeSprite(FOwner->GetDG(), BumpTex,THeadR, 0,0+224,31,31,159,0,0,16,16,18,15,false);
		Sprite[THeadR]->GetSpr()->Move(tempX,tempY-32);

		MakeSprite(FOwner->GetDG(), BumpTex,TBodyR, 32,0+224,31,47,157,0,0,22,33,13,34,false);
		Sprite[TBodyR]->GetSpr()->Move(tempX,tempY);

		MakeSprite(FOwner->GetDG(), BumpTex,TLShoulderR, 64,0+224,31,47,151,0,0,0,0,6,12,false);
		Sprite[TLShoulderR]->GetSpr()->Move(tempX+1,tempY-26);

		MakeSprite(FOwner->GetDG(), BumpTex,TLArmR, 32,48+224,47,15,141,0,0,0,0,43,6,false);
		Sprite[TLArmR]->GetSpr()->Move(tempX+1,tempY-3);

		MakeSprite(FOwner->GetDG(), BumpTex,TRLeg1R, 96,0+224,31,47,163,1,0,20,34,6,7,false);
		Sprite[TRLeg1R]->GetSpr()->Move(tempX,tempY+16);

		MakeSprite(FOwner->GetDG(), BumpTex,TRLeg2R, 128,0+224,31,95,165,5,0,20,56,3,3,false);
		Sprite[TRLeg2R]->GetSpr()->Move(tempX+10,tempY+40);

		MakeSprite(FOwner->GetDG(), BumpTex,TLLeg1R, 96,0+224,31,47,147,1,0,20,34,6,7,false);
		Sprite[TLLeg1R]->GetSpr()->Move(tempX,tempY+16);

		MakeSprite(FOwner->GetDG(), BumpTex,TLLeg2R, 128,0+224,31,95,149,5,0,20,56,3,3,false);
		Sprite[TLLeg2R]->GetSpr()->Move(tempX+10,tempY+40);

		MakeSprite(FOwner->GetDG(), BumpTex,TCWingR, 160,0+224,79,31,161,0,0,0,0,3,31,false);
		Sprite[TCWingR]->GetSpr()->Move(tempX+7,tempY-27);

		MakeSprite(FOwner->GetDG(), BumpTex,TLWingR, 160,32+224,95,31,139,0,0,0,0,21,14,false);
		Sprite[TLWingR]->GetSpr()->Move(tempX-1,tempY+18);

		MakeSprite(FOwner->GetDG(), BumpTex,TGun1R, 0,96+224,95,31,143,0,0,0,0,65,1,false);
		Sprite[TGun1R]->GetSpr()->Move(tempX-31,tempY-1);

		MakeSprite(FOwner->GetDG(), BumpTex,TGun2R, 96,96+224,79,31,145,0,0,0,0,0,0,false);
		Sprite[TGun2R]->GetSpr()->Move(tempX-31,tempY-1);

		MakeSprite(FOwner->GetDG(), BumpTex,TNoseR, 0,64+224,95,31,155,0,0,0,0,3,13,false);
		Sprite[TNoseR]->GetSpr()->Move(tempX+1,tempY-28);

		for( u32 i = 0; i <= 16; i++ ) {
			if( _red ) {
				Sprite[i]->GetSpr()->SetColor(255,ctRed);
				Sprite[i]->GetSpr()->SetColor(0,ctBlue);
				Sprite[i]->GetSpr()->SetColor(0,ctGreen);
			}
			else {
				Sprite[i]->GetSpr()->SetColor(KCOLOR_ORG,ctRed);
				Sprite[i]->GetSpr()->SetColor(KCOLOR_ORG,ctBlue);
				Sprite[i]->GetSpr()->SetColor(KCOLOR_ORG,ctGreen);
			}
			//FVisible = false;
		}

		for( u32 i = 17; i <= 30; i++ ) {
			Sprite[i]->GetSpr()->FEffectNum = 1;
			Sprite[i]->GetSpr()->FBumpSize = 10;
			Sprite[i]->GetSpr()->FBumpTex = BumpTex;
			Sprite[i]->GetSpr()->FBumpLevel = 4;

			Sprite[i]->GetSpr()->SetColor(192,ctAlpha);
		}

		Sprite[THip]->Add(Sprite[THipR]);
		Sprite[THead]->Add(Sprite[THeadR]);
		Sprite[TLShoulder]->Add(Sprite[TLShoulderR]);
		Sprite[TLArm]->Add(Sprite[TLArmR]);
		Sprite[TBody]->Add(Sprite[TBodyR]);
		Sprite[TRLeg1]->Add(Sprite[TRLeg1R]);
		Sprite[TRLeg2]->Add(Sprite[TRLeg2R]);
		Sprite[TLLeg1]->Add(Sprite[TLLeg1R]);
		Sprite[TLLeg2]->Add(Sprite[TLLeg2R]);
		Sprite[TNose]->Add(Sprite[TNoseR]);
		Sprite[TCWing]->Add(Sprite[TCWingR]);
		Sprite[TLWing]->Add(Sprite[TLWingR]);
		Sprite[TGun1]->Add(Sprite[TGun1R]);
		Sprite[TGun2]->Add(Sprite[TGun2R]);
	}

	//間接ドッキング
	Sprite[THip]->Add(Sprite[TBody]);
	Sprite[THip]->Add(Sprite[TRLeg1]);
	Sprite[THip]->Add(Sprite[TLLeg1]);

	Sprite[TBody]->Add(Sprite[THead]);
	Sprite[TBody]->Add(Sprite[TCWing]);
	Sprite[TBody]->Add(Sprite[TNose]);
	Sprite[TBody]->Add(Sprite[TLShoulder]);
	Sprite[TBody]->Add(Sprite[TRShoulder]);

	Sprite[TLShoulder]->Add(Sprite[TLArm]);

	Sprite[TRShoulder]->Add(Sprite[TRArm]);

	Sprite[TLArm]->Add(Sprite[TGun1]);

	Sprite[TGun1]->Add(Sprite[TGun2]);

	Sprite[TRLeg1]->Add(Sprite[TRLeg2]);
	Sprite[TRLeg1]->Add(Sprite[TRWing]);

	Sprite[TLLeg1]->Add(Sprite[TLLeg2]);
	Sprite[TLLeg1]->Add(Sprite[TLWing]);

	Sprite[TGun2]->SlideMoveR(-22,0);
	Sprite[THead]->SlideMoveR(0,14);
	Sprite[TBody]->SlideMoveR(0,8);

	Sprite[THip]->Rotate(1024);

	Sprite[TLShoulder]->Rotate(552);
	Sprite[TLArm]->Rotate(2232);
	Sprite[TGun1]->Rotate(2048);

	Sprite[TLLeg1]->Rotate(3072);
	Sprite[TRLeg1]->Rotate(3072);
	Sprite[TLLeg2]->Rotate(1400);
	Sprite[TRLeg2]->Rotate(1400);

	Sprite[TLWing]->Rotate(0);
	Sprite[TRWing]->Rotate(0);

	Sprite[TCWing]->Rotate(2096);
	Sprite[TNose]->Rotate(0);

	Sprite[THip]->Rotate(1024);

	HPGauge  =  new TEnemyGauge2(FOwner,this,0,32);
	FOwner->Add(HPGauge);

}

void TTransformer::Move()
{
	TOBJ::Move();

	switch( ActFlg ){
		case 0:{
			X = RoundOff(Sprite[THip]->GetSpr()->FCX);
			Y = RoundOff(Sprite[THip]->GetSpr()->FCY);

			Xdist = HPosList[TimCount].X;
			Ydist = HPosList[TimCount].Y;
			s32 DX = Xdist-X;
			s32 DY = Ydist-Y;
			IniDis = RoundOff(sqrt(DX*DX+DY*DY));

			Inc(TimCount);
			if( TimCount>3 ) TimCount = 0;
			CurDis = IniDis;
			DestDeg = RoundOff(atan2(DY,DX)/(f32)M_PI*2048.f);
			DestDeg  =  DestDeg & 0x0fff;
			XSpeed = 0;
			ActFlg = 1;
		}break;
		case 1:{
			if( (FOwner->Speed>=10) ) Boost = true;
			X = RoundOff(Sprite[THip]->GetSpr()->FCX);
			Y = RoundOff(Sprite[THip]->GetSpr()->FCY);

			CurDis = CurDis-XSpeed;
			XSpeed = XSpeed+0.2f;

			if( IniDis/CurDis > 2.f ) {
				ActFlg = 2;
			}

			if( (! TransState) && (! TransForm) ) {
				if( (0 <= DestDeg && DestDeg <= 512) || (3072 <= DestDeg && DestDeg <= 4095) )
					SlideBody(2);
				else if( 513 <= DestDeg && DestDeg <= 3071)
					SlideBody(1);
			}

			if( ((TransTiming<=0) || (FOwner->Speed>=10) || (FOwner->Distance>=Retreat)) && (! TransState) && (! BeamON) ) TransForm = true;

			if( (TransCount>=30) && (! TransState) ) {
				ActFlg = 5;
				TransTiming = 3-Pattern;
				TimCount = 0;
			}

			Boost = false;
			Sprite[THip]->MoveR(XSpeed*cost[DestDeg]+BSpeed,XSpeed*sint[DestDeg]);
		}break;
		case 2:{
			if( (FOwner->Speed>=10) ) Boost = true;
			X = RoundOff(Sprite[THip]->GetSpr()->FCX);
			Y = RoundOff(Sprite[THip]->GetSpr()->FCY);

			CurDis = CurDis-XSpeed;
			XSpeed = XSpeed-(XSpeed/20.f+0.01f);

			if( (! TransForm) && (! TransState) && (CurDis<100) ) BeamON = true;

			if( (BeamCount>=80) ) {
				ActFlg = 3;
				XSpeed = 0;
			}

			if( (CurDis>100) && (! TransState) && (! TransForm) ) {
				if( (0 <= DestDeg && DestDeg <= 1023) || (3072 <= DestDeg && DestDeg <= 4095) )
					SlideBody(2);
				else if( 1024 <= DestDeg && DestDeg <= 3071)
					SlideBody(1);
			}

			if( (CurDis<=100) && (! TransState) && (! TransForm) ) SlideBody(0);

			if( ((TransTiming<=0) || (FOwner->Speed>=10) || (FOwner->Distance>=Retreat)) && (! TransState) && (! BeamON) ) TransForm = true;

			if( (TransCount>=30) && (! TransState) ) {
				ActFlg = 5;
				TransTiming = Pattern+1;
				TimCount = 0;
				if( (FOwner->Distance>=Retreat) ) ActFlg = 8;
			}

			Boost = false;

			Sprite[THip]->MoveR(XSpeed*cost[DestDeg]+BSpeed,XSpeed*sint[DestDeg]);
		}break;
		case 3:{
			if( (FOwner->Speed>=10) ) Boost = true;
			if( (! TransState) && (! TransForm) && (BeamCount>90)) SlideBody(0);
			if( (BeamCount>80) || (! BeamON) ) {
				ActFlg = 0;
			}
		}break;
		case 5:{
			X = RoundOff(Sprite[THip]->GetSpr()->FCX);
			Y = RoundOff(Sprite[THip]->GetSpr()->FCY);

			Xdist = PPosList[TimCount].X;
			Ydist = PPosList[TimCount].Y;
			s32 DX = Xdist-X;
			s32 DY = Ydist-Y;
			IniDis = RoundOff(sqrt(DX*DX+DY*DY));

			Inc(TimCount);
			if( TimCount>3 ) TimCount = 0;
			CurDis = IniDis;
			DestDeg = RoundOff(atan2(DY,DX)/(f32)M_PI*2048);
			DestDeg  =  DestDeg & 0x0fff;
			XSpeed = 0;
			ActFlg = 6;
			Sprite[THip]->MoveR(BSpeed,0);

			AccelElement = 0.05f;
			DecelElement = 40;
		}break;
		case 6:{
			if( (FOwner->Speed>=10) ) Boost = true;

			if( (TransCount>=30) && (TransState) ) {
				ActFlg = 0;

				TransTiming = 4-Pattern;
				TimCount = 0;
			}
			X = RoundOff(Sprite[THip]->GetSpr()->FCX);
			Y = RoundOff(Sprite[THip]->GetSpr()->FCY);
			s32 DX = Xdist-X;
			s32 DY = Ydist-Y;
			//CurDis = RoundOff(sqrt(DX*DX+DY*DY));

			CurDis = CurDis-XSpeed;
			XSpeed = XSpeed+AccelElement;


			if( (TransTiming<=0) && (! BeamON) && (! Boost) ) {
				TransForm = true;
				ActCount = 0;
			}

			if( ( IniDis/CurDis>=1.25f) && (! BeamON) && (! TransForm) && (TransState) ) BeamON = true;


			//if( CurDis>20 ) {
			DestDeg = RoundOff(atan2(DY,DX)/(f32)M_PI*2048.f);
			DestDeg  =  DestDeg & 0x0fff;
			//}

			if( IniDis/CurDis>2.f ) {
				ActFlg = 7;
			}

			Sprite[THip]->MoveR(XSpeed*cost[DestDeg]+BSpeed,XSpeed*sint[DestDeg]-FOwner->BGSlide.Y);
		}break;
		case 7:{
			if( (FOwner->Speed>=10) ) Boost = true;
			else if( CommandRed && (Age>120) ) TransTiming = 0;

			CurDis = CurDis-XSpeed;
			XSpeed = XSpeed-(XSpeed/(f32)DecelElement+0.01f);

			if( (XSpeed<=0) && ((BeamCount>72) || (BeamCount == 0)) ) {
				ActFlg = 5;
				XSpeed = 0;
				if( (FOwner->Distance>=Retreat) ) ActFlg = 8;
			}

			if( (TransCount>=30) && (TransState) ) {
				ActFlg = 0;
				BeamON = false;

				TransTiming = 4-Pattern;
				TimCount = 0;
			}

			Sprite[THip]->MoveR(XSpeed*cost[DestDeg]+BSpeed,XSpeed*sint[DestDeg]-FOwner->BGSlide.Y);
		}break;
		case 8:{
			XSpeed = XSpeed+0.5f;
			Sprite[THip]->MoveR(XSpeed,0);
		}break;
		case 11:{
			Inc(ActCount);
			//if( ActCount=80 ) {
			switch( Pattern ){
				case 0:{ Xdist = 50; Ydist = 400; }		break;
				case 1:{ Xdist = 550; Ydist = 350; }		break;
				case 2:{ Xdist = 150; Ydist = 200; }		break;
				case 3:{ Xdist = 400; Ydist = 150; }		break;
			}
			Sprite[THip]->Move(Xdist,-64);
			X = RoundOff(Sprite[THip]->GetSpr()->FCX);
			Y = RoundOff(Sprite[THip]->GetSpr()->FCY);
			s32 DX = Xdist-X;
			s32 DY = Ydist-Y;
			IniDis = RoundOff(sqrt(DX*DX+DY*DY));
			CurDis = IniDis;
			DestDeg = RoundOff(atan2(DY,DX)/(f32)M_PI*2048);
			DestDeg  =  DestDeg & 0x0fff;

			XSpeed = 0;
			ActFlg = 6;
			ActCount = 0;
			AccelElement = 0.2f;
			DecelElement = 20;
		}break;
		//}
	}

	X = RoundOff(Sprite[THip]->GetSpr()->FCX);
	Y = RoundOff(Sprite[THip]->GetSpr()->FCY);

	if( Boost ) Accel();
	if( (! Boost) ) {
		if( (BSpeed>0) ) BSpeed = BSpeed-0.1f;
	}

	if( TransForm ) Trans(TransState);


	if( BeamON ) BeamShot();

	if( (Age-LastBuzz)>=40 ) Buzzed = false;

	if( (X > 640+400) ) {
		Die();//画面外に消えたら消す & 母艦にダメージ

		if( (FOwner->IsSequence()) )
			Dec(FOwner->MShipHP,FSTR*2);
		else
			Dec(FOwner->MShipHP,FSTR*10);
		FOwner->Add(new TMotherShipDamaged(FOwner, X, Y));
	}
	if( HP<=0 ) OnBroken();                        //HPが無くなったら死ぬ

	if( (Age-GaugeTime)>120 ) HPGauge->Hide();


	if( (Age-LastFlash == 2) ) {
		for( u32 i = 0 ; i < numSpr; i++) {
			if( CommandRed || (! ExtraEffects))
				Sprite[i]->GetSpr()->SetColor(255,ctRed);         //色を戻す
			else
				Sprite[i]->GetSpr()->SetColor(KCOLOR_ORG,ctRed);          //色を戻す
		}
		Sprite[THip]->MoveR(-3,0);                  //のけぞり戻し
	}

	PrevX = X-KazeSpeed;
	PrevY = Y;

}

void TTransformer::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);
	X = RoundOff(Sprite[THip]->GetSpr()->FCX);
	Y = RoundOff(Sprite[THip]->GetSpr()->FCY);
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
			Sprite[THip]->MoveR(3,0);                  //のけぞる
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
		Sprite[THip]->MoveR(3,0);                  //のけぞる
		HPGauge->Show();
		GaugeTime = Age;
		if( CreateSE ) FOwner->SoundEffect->WavPlay(WAEdamage0,7);
		if( HP<=0 ) BonusRate = target->BonusRate;
	}

	if( HP<TempHP ) TempHP = HP;

}

void TTransformer::Die()
{
	HPGauge->Die();
	TOBJ::Die();

}

void TTransformer::BeamShot()
{
	GunR = Sprite[TGun1]->Rdegree;
	GunR = GunR & 0x0fff;

	s32 AX = X;
	s32 AY = Y;

	s32 GX = RoundOff(Sprite[TGun1]->GetSpr()->FCX-68.f*cost[GunR]+8.f*sint[GunR]);
	s32 GY = RoundOff(Sprite[TGun1]->GetSpr()->FCY+8.f*cost[GunR]-68.f*sint[GunR]);

	s32 DX = FOwner->HeroPos.X-AX;
	s32 DY = FOwner->HeroPos.Y-AY;

	EnemyDeg = RoundOff(atan2(DY,DX)/(f32)M_PI*2048.f);
	EnemyDeg = EnemyDeg & 0x0fff;

	s32 ParticleR = random(4095);
	if( !TransState ){
		if( 0 <= BeamCount && BeamCount <= 59 ){
			if( (BeamCount == 0) && CreateSE ) FOwner->SoundEffect->WavPlay(WACharge,8);
			MonoRotate(TLShoulder,400,8);
			MonoRotate(TLShoulderR,400,8);
			if( 0 <= EnemyDeg && EnemyDeg <= 1479 )	PartsRotate(TLArm,1479+2048,0,8);
			else if( 1480 <= EnemyDeg && EnemyDeg <= 2700 )	PartsRotate(TLArm,EnemyDeg+2048,0,8);
			else if( 2701 <= EnemyDeg && EnemyDeg <= 4095 )	PartsRotate(TLArm,2700+2048,0,8);

			if( BeamCount % 2 == 0 ) {
				FOwner->Add(new TBLight(FOwner,GX,GY,(f32)BeamCount*0.04f,2000));
				FOwner->Add(new TParticle2(FOwner,GX+RoundOff(60.f*cost[ParticleR]),GY+RoundOff(60.f*sint[ParticleR]),1,ParticleR,-1,30));
			}
			else {
				FOwner->Add(new TBLight(FOwner,GX,GY,(f32)BeamCount*0.03f,2000));
			}
			if( ! CommandRed ) FOwner->Add(new TBeamSite(FOwner,GX,GY,GunR+2048,BeamCount));
		}
		else if( 60 <= BeamCount && BeamCount <= 70 ){
			if( BeamCount % 2 == 0 ) {
				FOwner->Add(new TBLight(FOwner,GX,GY,(f32)BeamCount*0.04f,2000));
			}
			else {
				FOwner->Add(new TBLight(FOwner,GX,GY,(f32)BeamCount*0.03f,2000));
			}

			if( ! CommandRed ) FOwner->Add(new TBeamSite(FOwner,GX,GY,GunR+2048,BeamCount));
		}
		else if( 71 == BeamCount ){
			FOwner->Add(new TSRControl(FOwner,GX,GY,GunR+2048));
			if( CommandRed ){
				FOwner->Add(new TTransBeamCore(FOwner,GX+RoundOff(80.f*cost[GunR]),GY+RoundOff(80.f*sint[GunR]),GunR+2048+512,1));
				FOwner->Add(new TTransBeamCore(FOwner,GX+RoundOff(80.f*cost[GunR]),GY+RoundOff(80.f*sint[GunR]),GunR+2048+128,1));
				FOwner->Add(new TTransBeamCore(FOwner,GX+RoundOff(80.f*cost[GunR]),GY+RoundOff(80.f*sint[GunR]),GunR+2048-128,1));
				FOwner->Add(new TTransBeamCore(FOwner,GX+RoundOff(80.f*cost[GunR]),GY+RoundOff(80.f*sint[GunR]),GunR+2048-512,1));
			}
			else {
				FOwner->Add(new TTransBeamCore(FOwner,GX+RoundOff(80.f*cost[GunR]),GY+RoundOff(80.f*sint[GunR]),GunR+2048,0));
				Dec(TransTiming);
			}
			FOwner->Add(new TMissileFlash(FOwner,GX,GY));
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WABeam,9);
			for( u32 i = 0; i <= 20; i++ )
				FOwner->Add(new TParticle2(FOwner,GX,GY,1,GunR+random(768*2)-768,random(15)+5,random(5)+1));
		}
		else if( 72 <= BeamCount && BeamCount <= 77 ){
			s32 BC = (BeamCount-70);
			SlideBody(2);
			MonoRotate(TLShoulder,-600,2);
			MonoRotate(TLShoulderR,-600,2);
			Sprite[THip]->MoveR((f32)(20-BC)*cost[GunR],(f32)(20-BC)*sint[GunR]);
		}
		else if( 78 <= BeamCount && BeamCount <= 90 ){
			s32 BC = (BeamCount-70);
			MonoRotate(TLShoulder,0,20);
			MonoRotate(TLShoulderR,0,20);
			SlideBody(2);
			if( (BeamCount-78)<=5 )
				Sprite[THip]->MoveR((f32)(20-BC)*cost[GunR],(f32)(20-BC)*sint[GunR]);
		}
		else if( 110 == BeamCount ){
			BeamCount = -1;
			BeamON = false;
		}
	}
	else{
		if( 0 <= BeamCount && BeamCount <= 59 ){
			if( (BeamCount == 0) && CreateSE ) FOwner->SoundEffect->WavPlay(WACharge,8);
			if( BeamCount % 2 == 0 ) {
				FOwner->Add(new TBLight(FOwner,GX,GY,BeamCount*0.04f,2000));
				FOwner->Add(new TParticle2(FOwner,GX+RoundOff(60.f*cost[ParticleR]),GY+RoundOff(60.f*sint[ParticleR]),1,ParticleR,-1,30));
			}
			else {
				FOwner->Add(new TBLight(FOwner,GX,GY,BeamCount*0.03f,2000));
			}
			if( ! CommandRed ) FOwner->Add(new TBeamSite(FOwner,GX,GY,GunR+2048,BeamCount));
		}
		else if( 60 <= BeamCount && BeamCount <= 70 ){
			if( BeamCount % 2 == 0 ) {
				FOwner->Add(new TBLight(FOwner,GX,GY,BeamCount*0.04f,2000));
			}
			else {
				FOwner->Add(new TBLight(FOwner,GX,GY,BeamCount*0.03f,2000));
			}

			if( ! CommandRed ) FOwner->Add(new TBeamSite(FOwner,GX,GY,GunR+2048,BeamCount));
		}
		else if( 71 == BeamCount ){
			FOwner->Add(new TSRControl(FOwner,GX,GY,GunR+2048));
			if( CommandRed ){
				FOwner->Add(new TTransBeamCore(FOwner,GX-48,GY,GunR+2048+512,1));
				FOwner->Add(new TTransBeamCore(FOwner,GX-48,GY,GunR+2048+128,1));
				FOwner->Add(new TTransBeamCore(FOwner,GX-48,GY,GunR+2048-128,1));
				FOwner->Add(new TTransBeamCore(FOwner,GX-48,GY,GunR+2048-512,1));
			}
			else{
				FOwner->Add(new TTransBeamCore(FOwner,GX-48,GY,GunR+2048,0));
			}
			Dec(TransTiming);
			if( CommandRed ) TransTiming = 0;
			if( BeamCount<77 ) Sprite[TGun1]->SlideMoveR(-2,0);
			if( (BeamCount>=77) && (BeamCount<83) ) Sprite[TGun1]->SlideMoveR(2,0);
			FOwner->Add(new TMissileFlash(FOwner,GX,GY));
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WABeam,9);
			for( u32 i = 0; i <= 20; i++ )
				FOwner->Add(new TParticle2(FOwner,GX,GY,1,GunR+random(768*2)-768,random(15)+5,random(5)+1));
		}
		else if( 72 <= BeamCount && BeamCount <= 74 ){
			Sprite[TGun1]->SlideMoveR(7,0);
		}
		else if( 75 <= BeamCount && BeamCount <= 93 ){
			Sprite[TGun1]->SlideMoveR(-1,0);
		}
		else if( 110 == BeamCount ){
			BeamCount = -1;
			BeamON = false;
		}
	}
	Inc(BeamCount,1);
}

void TTransformer::Control()
{

}

void TTransformer::Trans( bool _state )
{
	if( !_state ){
		if( TransCount<=10 ) {
			PartsRotate(TLShoulder,2048,1,4);
			Sprite[TGun2]->SlideMoveR(-2,0);
		}
		if( (TransCount>=0) && (TransCount<=6) ) Sprite[THead]->SlideMoveR(0,2);
		if( (TransCount>=0) && (TransCount<=7) ) Sprite[TBody]->SlideMoveR(0,1);

		if( TransCount>10 ) {
			BodyRotate(1024,6);

			PartsRotate(TLWing,0,0,6);
			PartsRotate(TRWing,0,0,6);
			PartsRotate(TGun1,2048,0,6);
			PartsRotate(TLShoulder,552,1,12);
			PartsRotate(TLArm,2232,0,4);
		}

		if( TransCount>15 ) {
			PartsRotate(TLLeg1,3072,1,6);
			PartsRotate(TRLeg1,3072,1,6);
			PartsRotate(TLLeg2,1400,0,6);
			PartsRotate(TRLeg2,1400,0,6);
		}

		if( TransCount>20 ) {
			PartsRotate(TCWing,2096,0,6);
			PartsRotate(TNose,0,0,6);
		}

		if( TransCount>50 ) {
			TransState = true;
			TransForm = false;
			TransCount = -1;
		}
	}
	else{
		if( TransCount<=25 ) {
			PartsRotate(TLShoulder,2048,-1,6);
			PartsRotate(TLArm,1024,0,6);
		}

		if( TransCount>10 ) {
			BodyRotate(0,6);
			PartsRotate(TLWing,300,0,6);
			PartsRotate(TRWing,300,0,6);

		}

		if( (TransCount>10) && (TransCount<=20) ) {
			PartsRotate(TLLeg1,1024,-1,6);
			PartsRotate(TRLeg1,1024,-1,6);
			PartsRotate(TLLeg2,1400,0,4);
			PartsRotate(TRLeg2,1400,0,4);
		}

		if( TransCount>20 ) {
			PartsRotate(TLLeg1,600,0,12);
			PartsRotate(TRLeg1,4000,0,12);
			PartsRotate(TLLeg2,3784,0,4);
			PartsRotate(TRLeg2,3920,0,4);
		}

		if( TransCount>25 ) {
			PartsRotate(TLShoulder,3664,-1,4);
			PartsRotate(TLArm,3848,0,4);
		}

		if( TransCount>40 ) PartsRotate(TGun1,3848,0,4);

		if( TransCount>30 ) {
			PartsRotate(TCWing,0,0,6);
			if( (TransCount<=37) ) Sprite[THead]->SlideMoveR(0,-2);
			if( (TransCount<=37) ) Sprite[TBody]->SlideMoveR(0,-1);
			PartsRotate(TNose,400,0,6);
		}

		if( (TransCount>50) && (TransCount<=61) ) Sprite[TGun2]->SlideMoveR(2,0);

		if( TransCount == 35 ) {
			FOwner->Add(new TFF(FOwner,X,Y-48,-200));
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WATrans,10);
		}

		if( TransCount > 60 ) {
			TransState = false;
			TransForm = false;
			TransCount = -1;
		}
	}

	Inc(TransCount);

}

void TTransformer::PartsRotate( s32 _sprnum, s32 _dist, s32 _pn, s32 _speed )
{
	s32 Rdiff = _dist-Sprite[_sprnum]->Rdegree;
	switch( _pn ){
		case -1:{
			Sprite[_sprnum]->RotateR(-ceil(abs(Rdiff)/(f32)_speed)+Sign(Rdiff)*1);
		}break;
		case 0 :{
			if( abs(Rdiff)>=2048 ) Sprite[_sprnum]->RotateR(-Sign(Rdiff)*ceil((f32)(4095-abs(Rdiff))/(f32)_speed)-Sign(Rdiff)*1);
			else if( abs(Rdiff)<2048 ) Sprite[_sprnum]->RotateR(ceil((f32)(Rdiff)/(f32)_speed)+Sign(Rdiff)*1);
		}break;
		case 1:{
			Sprite[_sprnum]->RotateR(ceil((f32)abs(Rdiff)/(f32)_speed)+Sign(Rdiff)*1);
		}break;
	}

}

void TTransformer::BodyRotate( s32 _dist, s32 _speed )
{
	s32 Rdiff = _dist-Sprite[THip]->Rdegree;
	if( abs(Rdiff)>=2048 ) {
		Sprite[THip]->RotateR(-Sign(Rdiff)*ceil((f32)(4095-abs(Rdiff))/(f32)_speed)-Sign(Rdiff)*1);
		Sprite[TLLeg1]->RotateR(Sign(Rdiff)*ceil((f32)(4095-abs(Rdiff))/(f32)_speed)+Sign(Rdiff)*1);
		Sprite[TRLeg1]->RotateR(Sign(Rdiff)*ceil((f32)(4095-abs(Rdiff))/(f32)_speed)+Sign(Rdiff)*1);
		Sprite[TLShoulder]->RotateR(Sign(Rdiff)*ceil((f32)(4095-abs(Rdiff))/(f32)_speed)+Sign(Rdiff)*1);
	}
	else if( abs(Rdiff)<2048 ) {
		Sprite[THip]->RotateR(ceil((f32)(Rdiff)/(f32)_speed)+Sign(Rdiff)*1);
		Sprite[TLLeg1]->RotateR(-ceil((f32)(Rdiff)/(f32)_speed)-Sign(Rdiff)*1);
		Sprite[TRLeg1]->RotateR(-ceil((f32)(Rdiff)/(f32)_speed)-Sign(Rdiff)*1);
		Sprite[TLShoulder]->RotateR(-ceil((f32)(Rdiff)/(f32)_speed)-Sign(Rdiff)*1);
	}
}

void TTransformer::MonoRotate( s32 _Sprnum, s32 _dist, s32 _speed )
{
	if( (_Sprnum>16) && (! ExtraEffects) ) return;
	s32 Rdiff = _dist-Sprite[_Sprnum]->Rdegree;
	if( abs(Rdiff)>=2048 ) {
		Sprite[_Sprnum]->RotateR(-Sign(Rdiff)*ceil((f32)(4095-abs(Rdiff))/(f32)_speed)-Sign(Rdiff)*1);
		for( u32 i = 0 ; i < Sprite[_Sprnum]->GetSize(); i++ ){
			if( (Sprite[_Sprnum]->Objs(i) != NULL) )
				Sprite[_Sprnum]->Objs(i)->RotateR(Sign(Rdiff)*ceil((f32)(4095-abs(Rdiff))/(f32)_speed)+Sign(Rdiff)*1);
		}
	}
	else if( abs(Rdiff)<2048 ) {
		Sprite[_Sprnum]->RotateR(ceil((f32)(Rdiff)/(f32)_speed)+Sign(Rdiff)*1);
		for( u32 i = 0 ; i < Sprite[_Sprnum]->GetSize(); i++ ){
			if( (Sprite[_Sprnum]->Objs(i)!=NULL) )
				Sprite[_Sprnum]->Objs(i)->RotateR(-ceil((f32)(Rdiff)/(f32)_speed)-Sign(Rdiff)*1);
		}
	}
}

void TTransformer::SlideBody( s32 _pat )
{
	switch( _pat ){
		case 0:{
			BodyRotate(0,8);
			if( ! BeamON ) {
				MonoRotate(TLShoulder,3664,8);
				MonoRotate(TLShoulderR,3664,8);
				PartsRotate(TLArm,3848,0,8);
				//MonoRotate(TRShoulder,488,8);
				//PartsRotate(TRArm,3888,0,8);
			}
			MonoRotate(TLLeg1,600,8);
			MonoRotate(TRLeg1,4000,8);
			MonoRotate(TLLeg1R,600,8);
			MonoRotate(TRLeg1R,4000,8);
			PartsRotate(TLLeg2,3784,0,8);
			PartsRotate(TRLeg2,3920,0,8);
		}break;
		case 1:{
			BodyRotate(3832,8);
			if( ! BeamON ) {
				MonoRotate(TLShoulder,3664,8);
				MonoRotate(TLShoulderR,3664,8);
				PartsRotate(TLArm,3848,0,8);
			}
			MonoRotate(TLLeg1,3832,8);
			MonoRotate(TRLeg1,3792,8);
			MonoRotate(TLLeg1R,3832,8);
			MonoRotate(TRLeg1R,3792,8);
			PartsRotate(TLLeg2,3432,0,8);
			PartsRotate(TRLeg2,3560,0,8);
		}break;
		case 2:{
			BodyRotate(224,8);
			if( ! BeamON ) {
				MonoRotate(TLShoulder,3664,8);
				MonoRotate(TLShoulderR,3664,8);
				PartsRotate(TLArm,3848,0,8);
			}
			MonoRotate(TLLeg1,736,8);
			MonoRotate(TRLeg1,848,8);
			MonoRotate(TLLeg1R,736,8);
			MonoRotate(TRLeg1R,848,8);
			PartsRotate(TLLeg2,688,0,8);
			PartsRotate(TRLeg2,416,0,8);
		}break;
	}

}

void TTransformer::Accel()
{
	switch( BoostCount ){
		case 0:{
			BSpeed = 0;
			Vapor->Clear();
		}break;
	}

	if( TransState ) {
		Vapor->MakeVapor(X+48,Y,0);
	}

	if( BSpeed<5.6f ) BSpeed = BSpeed+0.2f;

	if( (FOwner->Speed == 5) ) {
		BoostCount = -1;
		Boost = false;
	}

	Inc(BoostCount);

}

void TTransformer::OnBroken()
{
	TOBJ::OnBroken();
	FOwner->Add(new TExplarge(FOwner,X,Y,0,true));//爆発

}

TTransBeamPart::TTransBeamPart( TOBJList* owner, s32 sx, s32 sy, s32 R, s32 S, s32 L, f32 _zoom ): TOBJ(owner)
{
	FObjectName = "TTransBeamPart";
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
	sblt.RHeight  =  20;        // 幅
	sblt.RWidth   =  20;        // 高さ
	sblt.RX       =  X;        // X座標
	sblt.RY       =  Y;        // Y座標
	sblt.RZ       =  BeamZ;       // Z値

	Inc(BeamZ);
	if( BeamZ>4900 ) BeamZ = 2200;

	Sprite[0] =  new TJoint(sblt);         // スプライトの生成
	Inc(numSpr,1);

	Sprite[0]->GetSpr()->AddTexture(Eff2Tex,0, 192, 63, 255);
	Sprite[0]->GetSpr()->FVisible  =  true;
	Sprite[0]->GetSpr()->FCX += 10;
	Sprite[0]->GetSpr()->FCY += 10;
	Sprite[0]->GetSpr()->Zoom(_zoom*2.f,_zoom);
	Sprite[0]->GetSpr()->SetColor(48, ctRed);
	Sprite[0]->GetSpr()->SetColor(128,ctGreen);
	Sprite[0]->GetSpr()->SetColor(64,ctBlue);
	Sprite[0]->GetSpr()->SetColor(128,ctAlpha);
	Sprite[0]->Move(X,Y); //弾の初期位置を設定
	Sprite[0]->Rotate(Rdeg);

}

void TTransBeamPart::Move()
{
	TOBJ::Move();
	X = RoundOff(Sprite[0]->GetSpr()->FCX);
	Y = RoundOff(Sprite[0]->GetSpr()->FCY);

	Sprite[0]->SlideMoveR(Speed,0); //弾の移動

	Sprite[0]->GetSpr()->Rotate(0);
	Sprite[0]->GetSpr()->ZoomR(0.99f,0.95f);
	Sprite[0]->GetSpr()->Rotate(Rdeg);
	Sprite[0]->GetSpr()->SetColorR(-4,ctAlpha);

	if( Age>Life ) Die();


	if( (X > 640+70) || (Y > 480+70) || (Y <0-70) || (X < 0-70) ) Die();//画面外に消えたら消す

}

TTransBeamCore::TTransBeamCore( TOBJList* owner, s32 sx, s32 sy, s32 R, s32 _pat ): TOBJ(owner)
{
	FObjectName = "TTransBeamCore";
	X = sx;
	Y = sy;
	FSTR = 1000;
	FKind = kEShot;
	Buzzed = false;
	LastBuzz = 0;
	Pattern = _pat;
	Rdeg = R & 0x0fff;

	TSpriteEXBLT sblt;
	sblt.RBM      =  bmNormal; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  FOwner->GetDG();     // TDDDDの参照
	sblt.RHeight  =  32;        // 幅
	sblt.RWidth   =  64;        // 高さ
	sblt.RX       =  X;        // X座標
	sblt.RY       =  Y;        // Y座標
	sblt.RZ       =  5000;       // Z値

	Sprite[0] =  new TJoint(sblt);         // スプライトの生成
	Inc(numSpr,1);

	Sprite[0]->GetSpr()->AddTexture(Eff2Tex,0, 128, 63, 191);    // テクスチャを登録
	Sprite[0]->GetSpr()->FVisible  =  false;
	Sprite[0]->GetSpr()->AddHitArea(TPoint(16, 8), TPoint(16, 24), TPoint(48, 24),TPoint(48, 8), htSky);
	Sprite[0]->GetSpr()->FHit      =  true;
	Sprite[0]->GetSpr()->FCY +=16;
	Sprite[0]->Move(X,Y); //弾の初期位置を設定
	Sprite[0]->Rotate(Rdeg);

}

void TTransBeamCore::Move()
{
	TOBJ::Move();
	s32 N_particle = 0;
	s32 PatInterval = 0;

	switch( Pattern ){
		case 0:{
			Sprite[0]->SlideMoveR(35+FOwner->Speed,0); //弾の移動
			switch( EffLevel ){
				case 0:N_particle = 10;		break;
				case 1:N_particle = 14;		break;
				case 2:N_particle = 20;		break;
			}
			PatInterval = 2;
		}break;
		case 1:{
			Sprite[0]->SlideMoveR(7+FOwner->Speed,0);
			switch( EffLevel ){
				case 0:N_particle = 7;		break;
				case 1:N_particle = 10;		break;
				case 2:N_particle = 14;		break;
			}
			PatInterval = 3;
		}break;
	}

	X = RoundOff(Sprite[0]->GetSpr()->FCX);
	Y = RoundOff(Sprite[0]->GetSpr()->FCY);
	if( Age % PatInterval == 0 ) {
		for( u32 i = 0 ; i <= N_particle ; i++ ) {
			s32 PartsX = random(64)+i*2+32;
			s32 PartsY = random(40-i*2)-(40-i*2) / 2;
			FOwner->Add(new TTransBeamPart(FOwner,X+RoundOff((f32)PartsX*cost[Rdeg]+(f32)PartsY*sint[Rdeg]),
				Y+RoundOff(sint[Rdeg]*cost[Rdeg]+(f32)PartsX*sint[Rdeg]),
				Rdeg,
				random(5)+1,
				7+i*2,
				2.3f-(f32)i*0.07f));
		}
	}

	if( (Age-LastBuzz)>=40 ) Buzzed = false;

	if( (X > 640+70) || (Y > 480+70) || (Y <0-70) || (X < 0-70) ) Die();//弾が画面外に消えたら消す

}

void TTransBeamCore::OnDamaged( TOBJ* target )
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

TTransBFire::TTransBFire( TOBJList* owner, s32 sx, s32 sy ): TOBJ(owner)
{
	FObjectName = "TTransBFire";
	X = sx;
	Y = sy;
	FKind = kEffect;

	TSpriteEXBLT sblt;
	sblt.RBM      =  bmSubAdd; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  FOwner->GetDG();     // TDDDDの参照
	sblt.RHeight  =  32;        // 幅
	sblt.RWidth   =  32;        // 高さ
	sblt.RX       =  0;        // X座標
	sblt.RY       =  0;        // Y座標
	sblt.RZ       =  4100;       // Z値

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
	Sprite[0]->GetSpr()->FCX +=16;
	//FCY +=16;
	Sprite[0]->GetSpr()->Move(X,Y);
	Sprite[0]->GetSpr()->Zoom(0.9f, 4.f);
	Sprite[0]->GetSpr()->SetColor(96, ctRed);
	Sprite[0]->GetSpr()->SetColor(96, ctBlue);
	Sprite[0]->GetSpr()->SetColor(255, ctGreen);
	Sprite[0]->GetSpr()->SetColor(32,ctAlpha);
	Sprite[0]->GetSpr()->FVisible = true;
}

void TTransBFire::Move()
{
	if( Age % 1 == 0 ) {
		Sprite[0]->GetSpr()->AnimNext();
		Sprite[0]->GetSpr()->AnimNext();
		Sprite[0]->GetSpr()->ZoomR(0.8f, 1.f);
	}
	Sprite[0]->SlideMoveR(0,10);

	if( Age>=4 ) Die();
	TOBJ::Move();

}
