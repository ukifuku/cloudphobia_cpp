#include "BG.h"
#include "SXLib9.h"
#include "TOBJList.h"
#include "Define.h"
#include "Sound.h"

const s32	CloudSize[6] = { 196,160,128,96,96,96 };
const s32	CloudSpeed[6] = { 10,10,7,5,3,1 };
const s32	Cloud2Speed[6] = { 10,20,10,10,5,1 };
const s32	Cloud3Speed[6] = { 13,5,10,10,5,1 };
const u8	ClSpeedRate = 5;

const s32	KDISTANCE_FLAREOK = 47300;

const s32	KSKY_HIGHT_MAX = 1024;

static s32 ZCoordinate = 0;


//    空　　////////////////////////////////////////////////////////////
TSky::TSky(TOBJList* _owner) : TOBJ( _owner )
{
	FObjectName = "TSky";
	m_BltRect.Top = 768;
	m_BltRect.Left = 0;
	m_BltRect.Right = 511;
	m_BltRect.Bottom = 1279;
}

//    空2　　////////////////////////////////////////////////////////////
TSky2::TSky2(TOBJList* _owner) : TOBJ( _owner )
{
	FObjectName = "TSky2";

	X = -16;
	Y = -16;
	FKind = kBG;

	TSpriteEXBLT sblt; 
	sblt.RBM     = bmNormal; 	// αブレンドモード
	sblt.RColor  = 0xFFFFFFFF; 	// ポリゴン色
	sblt.RDDDD   = QD;  		// TDDDDの参照
	sblt.RHeight = 512;        	// 高さ
	sblt.RWidth  = 672;        	// 幅
	sblt.RX      = X;        	// X座標
	sblt.RY      = Y;        	// Y座標
	sblt.RZ      = 25500;       	// Z値

	Sprite[0] = new TJoint(sblt);         // 背景画像一枚目
	numSpr++;

	Sprite[1] = new TJoint(sblt);         // 背景画像二枚目
	numSpr++;

	Sprite[0]->GetSpr()->AddTexture(BGTex2, 1, 1, 256, 256);    // テクスチャを登録
	Sprite[0]->GetSpr()->FVisible = true;
	Sprite[0]->GetSpr()->SetColor(240, ctRed);
	Sprite[0]->GetSpr()->SetColor(240, ctGreen);
	Sprite[0]->GetSpr()->SetColor(240, ctBlue);

	Sprite[1]->GetSpr()->AddTexture(BGTex1, 1, 1, 255, 255);    // テクスチャを登録
	Sprite[1]->GetSpr()->FVisible = false;
	Sprite[1]->GetSpr()->FCY += 512;
	Sprite[1]->GetSpr()->Zoom(1.0f,2.0f);
	Sprite[1]->GetSpr()->Move(X, Y);
	Sprite[1]->GetSpr()->SetColor(240, ctRed);
	Sprite[1]->GetSpr()->SetColor(240, ctGreen);
	Sprite[1]->GetSpr()->SetColor(240, ctBlue);

	Sprite[0]->Add(Sprite[1]);
}

void TSky2::Move()
{
	Sprite[0]->Move(X, Y);

	//背景横移動の影響は受けない
	Sprite[0]->MoveR(FOwner->BGSlide.X, 0);

	if(FOwner->BGSlide.Y != 0)
	{
		Y += FOwner->BGSlide.Y;
	} 

	if(Y < -16) Y = -16;
	if(Y > KSKY_HIGHT_MAX ) Y = KSKY_HIGHT_MAX;

	if((Y > -16) && (Sprite[1]->GetSpr()->FVisible == false)) Sprite[1]->GetSpr()->FVisible = true;
	if((Y <= -16) && (Sprite[1]->GetSpr()->FVisible)) Sprite[1]->GetSpr()->FVisible =false;

	if((Y >= 512) && (Sprite[0]->GetSpr()->FVisible)) Sprite[0]->GetSpr()->FVisible = false;
	if((Y < 512) && (Sprite[0]->GetSpr()->FVisible == false)) Sprite[0]->GetSpr()->FVisible = true;

	TOBJ::Move();
}

//    雲　　////////////////////////////////////////////////////////////
TCloud::TCloud(TOBJList* _owner, s32 _x, s32 _y, s32 _z) : TOBJ(_owner)
{
	f32 zoomrate;
	FObjectName = "TCloud";
	X = _x;
	Y = _y;
	ZValue = _z;
	zoomrate = 1;
	FKind = kBG;

	TSpriteEXBLT sblt;
	sblt.RBM     = bmNormal; // αブレンドモード
	sblt.RColor  = 0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD   = QD;     // TDDDDの参照
	sblt.RHeight = 64;        // 高さ
	sblt.RWidth  = 64;        // 幅
	sblt.RX      = 0;        // X座標
	sblt.RY      = 0;        // Y座標

	switch( ZValue ) 
	{
	case 0:{
		zoomrate     = 1.7f;
		sblt.RZ      = 20000+ZCoordinate;       // Z値
		   }
		   break;
	case 1:{
		zoomrate     = 1.5f;
		sblt.RZ      = 21000+ZCoordinate;       // Z値
		   }
		   break;
	case 2:{
		zoomrate     = 1.3f;
		sblt.RZ      = 22000+ZCoordinate;       // Z値
		   }
		   break;
	case 3:{
		zoomrate     = 1.f;
		sblt.RZ      = 24600+ZCoordinate;       // Z値
		   }
		   break;
	case 4:{
		zoomrate     = 0.9f;
		sblt.RZ      = 24900+ZCoordinate;       // Z値
		   }
		   break;
	case 5:{
		zoomrate     = 0.9f;
		sblt.RZ      = 25200+ZCoordinate;       // Z値
		   }
	}

	if( ZCoordinate >= 200 )
		ZCoordinate = 0;
	else 
		ZCoordinate++;

	Sprite[0] = new TJoint(sblt);         // スプライトの生成
	numSpr++;

	Sprite[0]->GetSpr()->AddTexture(Eff8Tex, 33, 129, 63, 159);  //
	Sprite[0]->GetSpr()->FCX += 32;
	Sprite[0]->GetSpr()->FCY += 32;
	Sprite[0]->GetSpr()->Move(X,Y);
	Sprite[0]->GetSpr()->Rotate(random(4096));
	Sprite[0]->GetSpr()->Zoom(zoomrate*3.f, zoomrate);
	Sprite[0]->GetSpr()->FVisible = false;
	Sprite[0]->GetSpr()->SetColor(240, ctGreen);

	if( ZValue>=4 ) {
		Sprite[0]->GetSpr()->SetColor(200, ctAlpha);
	}
}

void TCloud::Move()
{

	f32 XSpeed;
	s32 deadPointX;

	X = RoundOff(Sprite[0]->GetSpr()->FCX);
	Y = RoundOff(Sprite[0]->GetSpr()->FCY);
	Sprite[0]->GetSpr()->SetGradation(2500, 0x884400, 2);

	XSpeed =  (-CloudSpeed[0]-(FOwner->Speed-5)*ClSpeedRate)*(1-0.19f*ZValue);
	Sprite[0]->MoveR(XSpeed, 0);

	if( XSpeed<0 ) {
		deadPointX =  -CloudSize[ZValue] / 2;
		if(X<deadPointX) Die();
	}	
	else {
		deadPointX =  640+CloudSize[ZValue] / 2;
		if(X>deadPointX) Die();
	}

	if(EffLevel>0 ) {
		if((Y>550) || (!CloudON) )
			Sprite[0]->GetSpr()->FVisible = false;
		else 
			Sprite[0]->GetSpr()->FVisible = true;
	}
	else {
		Sprite[0]->GetSpr()->FVisible = false;
	}

	if(FOwner->NowStage == 99 ) {
		if((TStageSelectScreenMng*)(FOwner->StageSelect)->GetCurrentStage() == 0 )
			Sprite[0]->GetSpr()->FVisible = true;
		else
			Sprite[0]->GetSpr()->FVisible = false;
	}

	TOBJ::Move();
}


//    雲2　　////////////////////////////////////////////////////////////
TCloud2::TCloud2(TOBJList* _owner, s32 _x, s32 _y, s32 _pat, u8 _alpha, s32 x1, s32 y1, s32 x2, s32 y2) : TOBJ(_owner)
{
	TSpriteEXBLT sblt;
	u16 _z = 250;
	const s32 MaxAlpha[] = { 160,240,144 };

	FObjectName = "TCloud2";
	X = _x;
	Y = _y;
	FKind = kBG;
	Pattern = _pat;
	Alpha = _alpha;

	if( Alpha > MaxAlpha[Pattern] ) 
		Alpha = MaxAlpha[Pattern];

	switch( Pattern ) 
	{
	case 0:{ _z = 25000;    Speed = Cloud2Speed[0]; } break;
	case 1:{ _z = 25100;    Speed = Cloud2Speed[1]; } break;
	case 2:{ _z = 25200;    Speed = Cloud2Speed[2]; } break;
	}

	sblt.RBM      =  bmNormal; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  QD;     // TDDDDの参照
	sblt.RHeight  =  64;        // 高さ
	sblt.RWidth   =  64;        // 幅
	sblt.RX       =  0;        // X座標
	sblt.RY       =  0;        // Y座標
	sblt.RZ       =  _z;       // Z値

	Sprite[0] = new TJoint(sblt);         // スプライトの生成
	numSpr++;

	Sprite[0]->GetSpr()->AddTexture(MechaTex, x1, y1, x2, y2);    // テクスチャを登録
	Sprite[0]->GetSpr()->SetColor(200,ctRed);
	Sprite[0]->GetSpr()->SetColor(200,ctGreen);
	Sprite[0]->GetSpr()->FLuminance = 80;
	Sprite[0]->GetSpr()->FToneMapping  =  true;

	Sprite[0]->GetSpr()->SetColor(Alpha, ctAlpha);
	Sprite[0]->GetSpr()->Move(X,Y);

	if(Pattern==1 )
		Sprite[0]->GetSpr()->Zoom(2,2);

	if( !((EffLevel == 0) && (Pattern == 0)) ){
		Sprite[0]->GetSpr()->FVisible = true;
	}
	else{
		Sprite[0]->GetSpr()->FVisible = false;
	}

	//IDの付与
	FID = NewID;
	NewID++;
}

void TCloud2::Move()
{
	const s32 MaxAlpha[] = { 160, 240, 144 };
	X = RoundOff(Sprite[0]->GetSpr()->FCX);
	Y = RoundOff(Sprite[0]->GetSpr()->FCY);

	if(Alpha < MaxAlpha[Pattern] )
		Alpha = (Alpha + FOwner->Speed / 5);

	Sprite[0]->GetSpr()->SetColor(Alpha, ctAlpha);

	Sprite[0]->MoveR(-Speed-FOwner->Speed+5, 0);

	switch( Pattern )
	{
	case 0: if((X < -70) || (Y > 544)) Die(); break;
	case 1: if((X < -130) || (Y > 544)) Die(); break;
	}

	TOBJ::Move();
}

//    雲3　　////////////////////////////////////////////////////////////
TCloud3::TCloud3(TOBJList* _owner, s32 _x, s32 _y, s32 _pat, u8 _alpha, s32 x1, s32 y1, s32 x2, s32 y2, TOBJ* _obj) : TOBJ(_owner)
{
	TSpriteEXBLT sblt;
	u16 _z = 250;

	FObjectName = "TCloud3";
	X = _x;
	Y = _y;
	FKind = kBG;
	Pattern = _pat;
	Alpha = _alpha;
	OwnerOBJ = _obj;

	switch( Pattern ) 
	{
	case 0:{ _z = 19800;    Speed = Cloud3Speed[0];} break;
	case 1:{ _z = 19900;    Speed = Cloud3Speed[1];} break;
	case 2:{ _z = 20000;    Speed = Cloud3Speed[2];} break;
	}

	sblt.RBM      =  bmNormal; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  QD;     // TDDDDの参照
	sblt.RHeight  =  140+16*Pattern;        // 高さ
	sblt.RWidth   =  128;        // 幅
	sblt.RX       =  0;        // X座標
	sblt.RY       =  0;        // Y座標
	sblt.RZ       =  _z;       // Z値

	Sprite[0] = new TJoint(sblt);         // スプライトの生成
	numSpr++;

	Sprite[0]->GetSpr()->AddTexture(MechaTex, x1, y1, x2, y2);    // テクスチャを登録
	Sprite[0]->GetSpr()->SetColor(Alpha, ctAlpha);
	Sprite[0]->GetSpr()->Move(X,Y);
	Sprite[0]->GetSpr()->FDistorte = true;
	Sprite[0]->GetSpr()->FDistortRate = 2;
}

void TCloud3::Move()
{
	f32 XSpeed;
	s32 deadPointX;

	//雲生成器と高さをあわせる
	Sprite[0]->Move(X, OwnerOBJ->Y+350+10);

	Sprite[0]->GetSpr()->SetColor(Alpha,ctAlpha);

	XSpeed = (f32)( -Speed-(FOwner->Speed-5)*ClSpeedRate );
	Sprite[0]->MoveR(XSpeed,0);

	X = RoundOff(Sprite[0]->GetSpr()->FCX);
	Y = RoundOff(Sprite[0]->GetSpr()->FCY);

	if(XSpeed<0 ) {
		deadPointX =  -350;
		if( X<deadPointX )
			Die();
	}	
	else {
		deadPointX =  640+384+128;
		if( X>deadPointX ) Die();
	}

	if( Y<350 ) Sprite[0]->Move(X,350);

	if((Y>550) || !CloudON ) 
		Sprite[0]->GetSpr()->FVisible = false;
	else 
		Sprite[0]->GetSpr()->FVisible = true;

	if( FOwner->NowStage == 99 ) {

		if((TStageSelectScreenMng*)(FOwner->StageSelect)->GetCurrentStage() == 0 )
			Sprite[0]->GetSpr()->FVisible = true;
		else
			Sprite[0]->GetSpr()->FVisible = false;
	}

	TOBJ::Move();
}


//    雲生成　　////////////////////////////////////////////////////////////
TCloudMaker::TCloudMaker(TOBJList* _owner, s32 _y) : TOBJ(_owner)
{
	FObjectName = "TCloudMaker";
	FKind = kBG;
	Y = _y;

	CloudYPos[0]  =  480;
	CloudYPos[1]  =  448;
	CloudYPos[2]  =  416;
	CloudYPos[3]  =  384;
	CloudYPos[4]  =  368;
	CloudYPos[5]  =  352;


	for( s32 i = 0; i < 6; i++ ) {
		Cdistance[i] = 0;

		for( s32 k = 0; k <= (640 / CloudSize[i])+1; k++ ){
			FOwner->Add( new TCloud(FOwner, k*CloudSize[i], CloudYPos[i]+random(16)-8+Y, i));
		}
	}

}

void TCloudMaker::Move()
{
	s32 appearX;
	Y += FOwner->BGSlide.Y;

	if( Y > KSKY_HIGHT_MAX )
		Y = KSKY_HIGHT_MAX;

	if( Y < 0 )
		Y = 0;

	for( s32 i = 0; i < 6; i++ ) {
		Cdistance[i] = Cdistance[i] + (CloudSpeed[0] + abs(FOwner->Speed-5) * ClSpeedRate) * ( 1-0.19f*i);

		if(Cdistance[i]>(CloudSize[i]) ) {
			if(FOwner->Speed>=0 )
				appearX = 640+(CloudSize[i] / 2);
			else
				appearX = -(CloudSize[i] / 2);

			FOwner->Add(new TCloud(FOwner,appearX,CloudYPos[i]+random(16)-8+Y,i));
			Cdistance[i] = 0;
		}
	}
	TOBJ::Move();
}

//    雲生成2　　////////////////////////////////////////////////////////////
TCloudMaker2::TCloudMaker2(TOBJList* _owner) : TOBJ(_owner)
{
	const s32 TexSize = 30;
	const s32 tempX[] = { 130,  2,258,  2,130,258 };
	const s32 tempY[] = { 386,386,386,386,386,386 };
	const s32 StartAlpha[] = {0,0};

	FObjectName = "TCloudMaker2";
	FKind = kBG;
	Alpha = 0;

	for( s32 i = 0; i<2; i++ ) {
		Cdistance[i] = 0;
		CloudFlg[i] = 1;
	}

	if((FOwner->Difficulty <= KDIFFICULTY_NORMAL) )
		EndDistance  =  KDISTANCE_FLAREOK - 5000;
	else
		EndDistance  =  KDISTANCE_FLAREOK;

	TotalDist = 0;

	s32 nRect = 0;

	for( s32 l = 0; l<4; l++ ) {
		for( s32 i = 0; i<4; i++ ) {
			for( s32 k = 0; k<4; k++ ) {
				TexRect[nRect] = Rect(tempX[l]+i*TexSize,tempY[l]+k*TexSize,tempX[l]+i*TexSize+TexSize,tempY[l]+k*TexSize+TexSize);
				nRect++;
			}
		}
	}

	for( s32 l = 4; l<6; l++ ) {
		for( s32 i = 3; i>=0; i-- ) {
			for( s32 k = 0; k<4; k++ ) {
				TexRect[nRect] = Rect(tempX[l]+i*TexSize+TexSize,tempY[l]+k*TexSize,tempX[l]+i*TexSize,tempY[l]+k*TexSize+TexSize);
				nRect++;
			}
		}
	}

	for( s32 i = 0; i<=1; i++ ) {
		for( s32 k = 0; k<=3; k++ ){
			FOwner->Add( new TCloud2(FOwner,640+64,k*64*(i+1),i,StartAlpha[i],TexRect[k].Left,TexRect[k].Top,TexRect[k].Right,TexRect[k].Bottom));
		}
	}

	for( s32 k = 0; k <= 3; k++ ) {
		FOwner->Add( new TCloud2(FOwner,640+64,256+k*64,0,StartAlpha[0],TexRect[k+32].Left,TexRect[k+32].Top,TexRect[k+32].Right,TexRect[k+32].Bottom));
	}
}

void TCloudMaker2::Move()
{
	const s32 CloudSize[] = {64,128};
	s32 StartAlpha[] = {0,0};
	TotalDist += 1+FOwner->Speed-5;
	Alpha += FOwner->Speed / 5;

	for( s32 i = 0; i<= 1; i++ ) {
		Cdistance[i] += Cloud2Speed[i]+FOwner->Speed-5;
		if(Cdistance[i]>CloudSize[i]-1 ) {
			if(CloudFlg[i]<8 ) {

				for( s32 k = 0; k<= 3; k++ ) {
					FOwner->Add( new TCloud2(FOwner,640+64-(Cdistance[i]-CloudSize[i]),k*CloudSize[i],i,StartAlpha[i]+Alpha,
						TexRect[k+4*CloudFlg[i]].Left,TexRect[k+4*CloudFlg[i]].Top,
						TexRect[k+4*CloudFlg[i]].Right,TexRect[k+4*CloudFlg[i]].Bottom));
				}

				if( i==0 ) {
					for( s32 k = 0; k <= 3; k++ ) {
						FOwner->Add( new TCloud2(FOwner,640+64-(Cdistance[i]-CloudSize[i]),256+k*CloudSize[i],i,StartAlpha[i]+Alpha,
							TexRect[k+4*CloudFlg[i]+32].Left,TexRect[k+4*CloudFlg[i]+32].Top,
							TexRect[k+4*CloudFlg[i]+32].Right,TexRect[k+4*CloudFlg[i]+32].Bottom));
					}	
				}

				CloudFlg[i]++;

				if((CloudFlg[0]==7) && (FOwner->FlareOK) ) 
					FOwner->FlareOK = false;

				if((FOwner->Distance>EndDistance) && (CloudFlg[i]>7) )
					CloudFlg[i] = 16;
				else if((CloudFlg[i]>7) ) 
					CloudFlg[i] = 4;
			}
			else if(CloudFlg[i]<20 ) {
				for( s32 k = 0; k <= 3; k++ ){
					FOwner->Add( new TCloud2(FOwner,640+64-(Cdistance[i]-CloudSize[i]),k*CloudSize[i],i,StartAlpha[i]+Alpha,
						TexRect[k+4*CloudFlg[i]].Left,TexRect[k+4*CloudFlg[i]].Top,
						TexRect[k+4*CloudFlg[i]].Right,TexRect[k+4*CloudFlg[i]].Bottom));
				}

				if(i==0 ){
					for( s32 k = 0; k<= 3; k++ ){
						FOwner->Add( new TCloud2(FOwner,640+64-(Cdistance[i]-CloudSize[i]),256+k*CloudSize[i],i,StartAlpha[i]+Alpha,
							TexRect[k+4*CloudFlg[i]+16].Left,TexRect[k+4*CloudFlg[i]+16].Top,
							TexRect[k+4*CloudFlg[i]+16].Right,TexRect[k+4*CloudFlg[i]+16].Bottom));
					}
				}

				CloudFlg[i]++;
			}

			if((CloudFlg[0]>19) && (CloudFlg[1]>19) ) {
				FOwner->Add( new TBoxContrl(FOwner,5));
				FOwner->FlareOK = true;
				Die();
			}
			Cdistance[i] = Cdistance[i]-CloudSize[i];
		}
	}

	if( Age % (random(30)+20) == 0 ) {
		FOwner->Add(new TCloudFast(FOwner,640,random(400)));
	}

	//雨
	if((Age % 3 == 0) && (Age>60) ) {
		for( s32 i = 0; i<=3; i++ ){
			FOwner->Add( new TRain(FOwner,random(800)+100,0));
		}
	}

	if(ExtraEffects ) {
		//水滴
		if((Age % 6 == 0) && (Age>60) ) {
			FOwner->Add(new TWaterControl(FOwner,random(640),random(480)));
		}
	}
	TOBJ::Move();
}

//    雲生成3　　////////////////////////////////////////////////////////////
TCloudMaker3::TCloudMaker3(TOBJList* _owner, s32 _x, s32 _y) : TOBJ(_owner)
{
	s32 CNum;

	FObjectName = "TCloudMaker3";
	FKind = kBG;
	Y = _y;
	CNum = random(3)+2;

	for( s32 i = 0; i<= CNum; i++ ){
		FOwner->Add(new TCloud(FOwner, _x+i*(CloudSize[0] / 2), _y+random(48)-24,0));
	}
}

void TCloudMaker3::Move()
{
	Die();

	TOBJ::Move();
}

//    雲生成4　　////////////////////////////////////////////////////////////
TCloudMaker4::TCloudMaker4(TOBJList* _owner, s32 _y) : TOBJ(_owner)
{
	const s32 TexSize = 32;
	const s32 StartAlpha[] = {50,100,100};    //150,120,255

	FObjectName = "TCloudMaker4";
	FKind = kBG;
	Alpha = 0;

	Y = _y;

	for( s32 i = 0; i<= 8; i++) {
		Cdistance[i] = 0;
		CloudFlg[i] = 0;
	}

	TotalDist = 0;

	//IDの付与
	FID = NewID;
	NewID++;

	for( s32 i = 0; i<= 3; i++ ) {
		TexRect[i] = Rect(385+i*TexSize,385,385+i*TexSize+TexSize-2,512);
	}


	for( s32 k = 0; k<= 2 ; k++ ) {
		for( s32 i = 0; i<= 3; i++ ) {
			FOwner->Add(new TCloud3(FOwner,-448+512*k+i*128,Y+350,0,StartAlpha[0],TexRect[i].Left,TexRect[i].Top,TexRect[i].Right,TexRect[i].Bottom,this));
			FOwner->Add(new TCloud3(FOwner,-448+512*k+i*128,Y+350,1,StartAlpha[1],TexRect[i].Left,TexRect[i].Top,TexRect[i].Right,TexRect[i].Bottom,this));
		}
	}

}

void TCloudMaker4::Move()
{
	s32 appearX;
	const s32 Cloud2Size[] = {128,128,128};
	const u32 StartAlpha[] = {50,100,255};
	TotalDist += 1+FOwner->Speed-5;

	Y += FOwner->BGSlide.Y;
	if(Y>KSKY_HIGHT_MAX )
		Y = KSKY_HIGHT_MAX;

	if(Y<0 )
		Y = 0;

	for( s32 i = 0; i<= 1; i++ ) {
		Cdistance[i] += Cloud3Speed[i]+(FOwner->Speed-5)*ClSpeedRate;
		if(abs(Cdistance[i])>=(Cloud2Size[i]) ) {
			if((Cdistance[i])>=0 ) {
				appearX = 640+320-(Cdistance[i]-Cloud2Size[i]);
				Cdistance[i] = Cdistance[i]-Cloud2Size[i];

				FOwner->Add(new TCloud3(FOwner,appearX,Y+350,i,StartAlpha[i],
					TexRect[CloudFlg[i]].Left,TexRect[CloudFlg[i]].Top,
					TexRect[CloudFlg[i]].Right,TexRect[CloudFlg[i]].Bottom,this));

				CloudFlg[i]++;

				if((CloudFlg[i]>3) ) CloudFlg[i] = 0;
			}
			else {
				appearX = -448-(Cdistance[i]+Cloud2Size[i]);
				Cdistance[i] = (Cdistance[i]+Cloud2Size[i]);

				FOwner->Add(new TCloud3(FOwner,appearX,Y+350,i,StartAlpha[i],
					TexRect[CloudFlg[i]].Left,TexRect[CloudFlg[i]].Top,
					TexRect[CloudFlg[i]].Right,TexRect[CloudFlg[i]].Bottom,this));

				CloudFlg[i]--;

				if((CloudFlg[i]<0) ) CloudFlg[i] = 3;
			}


		}
	}

	TOBJ::Move();
}


//  太陽　　////////////////////////////////////////////////////////////////////////
TSun::TSun(TOBJList* _owner, s32 _x, s32 _y) : TOBJ(_owner)
{
	X = _x;
	Y = _y;
	ActFlg = 3;
	ActCount = 0;
	FKind = kBG;

	if((FOwner->Difficulty == KDIFFICULTY_EASY) )
		EndDistance  =  KDISTANCE_FLAREOK - 3000;
	else
		EndDistance  =  KDISTANCE_FLAREOK;

	TSpriteEXBLT sblt;
	sblt.RBM      =  bmSubAdd; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  QD;     // TDDDDの参照
	sblt.RHeight  =  64;        // 高さ
	sblt.RWidth   =  64;        // 幅
	sblt.RX       =  0;        // X座標
	sblt.RY       =  0;        // Y座標
	sblt.RZ       =  25400;       // Z値

	Sprite[0] = new TJoint(sblt);         // スプライトの生成
	numSpr++;

	Sprite[0]->GetSpr()->AddTexture(Eff1Tex,  0, 16, 175, 191);    // テクスチャを登録

	Sprite[0]->GetSpr()->SetColor(128,ctBlue);
	Sprite[0]->GetSpr()->SetColor(64,ctRed);
	Sprite[0]->GetSpr()->SetColor(64,ctGreen);
	//SetColor(200, ctAlpha);

	Sprite[0]->GetSpr()->FCX += 32;
	Sprite[0]->GetSpr()->FCY += 32;
	Sprite[0]->GetSpr()->AddHitArea(TPoint(-50, 29), TPoint(35, 29), TPoint(35, 35),TPoint(-50, 35), htSky);
	Sprite[0]->GetSpr()->FHit        =  true;
	Sprite[0]->GetSpr()->Move(X,Y);
	Sprite[0]->GetSpr()->FVisible = true;

	sblt.RBM      =  bmAdd; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  QD;     // TDDDDの参照
	sblt.RHeight  =  256;        // 高さ
	sblt.RWidth   =  256;        // 幅
	sblt.RX       =  0;        // X座標
	sblt.RY       =  0;        // Y座標
	sblt.RZ       =  25300;       // Z値

	Sprite[1] =  new TJoint(sblt);         // スプライトの生成
	numSpr++;

	Sprite[1]->GetSpr()->AddTexture(Eff5Tex,  0, 0, 255, 255);    // テクスチャを登録
	Sprite[1]->GetSpr()->FCX += 128;
	Sprite[1]->GetSpr()->FCY += 128;
	Sprite[1]->GetSpr()->FVisible = true;
	Sprite[1]->GetSpr()->Move(X,Y);

	Sprite[0]->Add(Sprite[1]);

	//IDの付与
	FID = NewID;
	NewID++;
}

void TSun::Move()
{

	f32 Alpha;
	X = RoundOff(Sprite[0]->GetSpr()->FCX);
	Y = RoundOff(Sprite[0]->GetSpr()->FCY);
	//FlareOK = true;
	switch (ActFlg) 
	{ 
	case 0:{
		Sprite[0]->MoveR(-4*cost[ActCount*16],8*sint[1024+ActCount*16]);
		ActCount++;
		if(ActCount*16>=1024 ) {
			ActFlg = 1;
			ActCount = 0;
			FOwner->Add(new TBoxContrl(FOwner,5));
		}
		   }
		   break;
	case 1:{
		ActCount++;
		//FOwner->TempCount[0] = X;
		if(ActCount>=10 ) {
			ActFlg = 2;
			ActCount = 0;
		}
		   }
		   break;
	case 2:{
		Sprite[0]->MoveR(4*cost[1024+ActCount*16],8*sint[ActCount*16]);
		ActCount++;
		if(ActCount*16>=1024 ) {
			Die();
		}
		   } 
		   break;
	case 3:{
		//出現
		Alpha = ActCount*0.02f;
		if(Alpha>1 ) Alpha = 1;
		//if((FOwner->Distance<22820) and (Age mod (2-FrameRate) = 0) ) FOwner->Add(TFlare.Create(FOwner,X,Y,Alpha));
		if((FOwner->FlareOK) && (Age % (2-FrameRate) == 0) ) 
			FOwner->Add(new TFlare(FOwner,X,Y,Alpha));

		Sprite[0]->MoveR(-4*cost[ActCount*12],0);
		ActCount++;

		if(ActCount*12>=1024 ) {
			ActFlg = 4;
			ActCount = 0;
		}
		   }
		   break;
	case 4:{
		//何もしない
		if((Age % (2-FrameRate) == 0) ) 
			FOwner->Add( new TFlare(FOwner,X,Y,1));

		if(! FOwner->FlareOK ) {
			ActFlg = 5;
			ActCount = 0;
		}
		   }
		   break;
	case 5:{
		//雲に隠れる
		if(ActCount<=20 ) {
			Alpha = 1.f - (f32)ActCount*0.05f;
			if((Age % (2-FrameRate) == 0) ) FOwner->Add( new TFlare(FOwner,X,Y,Alpha));
		}

		ActCount++;

		if(FOwner->FlareOK ) {
			ActFlg = 6;
			ActCount = 0;
		}
		   }
		   break;
	case 6:{
		//雲が過ぎ去る
		Alpha = (f32)ActCount*0.1f;
		if(Alpha > 1.f ) Alpha = 1.f;

		if((Age % (2-FrameRate) == 0) ) FOwner->Add(new TFlare(FOwner,X,Y,Alpha));

		ActCount++;

		if(FOwner->Distance>EndDistance + 1000 ) {
			ActFlg = 7;
			ActCount = 0;
		}
		   }
		   break;
	case 7:{
		Sprite[0]->MoveR(-4*sint[ActCount*12],-8*sint[ActCount*12]);
		ActCount++;
		if((Age % (2-FrameRate) == 0) ) 
			FOwner->Add( new TFlare(FOwner,X,Y,1-ActCount*0.01f));
		if(ActCount*12>=1024 ) {
			Die();
		}
		   }
	}
	/// 330
	TOBJ::Move();
}


//  月　　////////////////////////////////////////////////////////////////////////
TMoon::TMoon(TOBJList* _owner, s32 _x, s32 _y) :TOBJ(_owner)
{
	X = _x;
	Y = _y;
	ActFlg = 0;
	ActCount = 0;
	FKind = kBG;

	TSpriteEXBLT sblt;
	sblt.RBM      =  bmNormal; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  QD;     // TDDDDの参照
	sblt.RHeight  =  64;        // 高さ
	sblt.RWidth   =  64;        // 幅
	sblt.RX       =  0;        // X座標
	sblt.RY       =  0;        // Y座標
	sblt.RZ       =  25400;       // Z値

	Sprite[0] = new TJoint(sblt);         // スプライトの生成
	numSpr++;
	Sprite[0]->GetSpr()->AddTexture(Eff1Tex,  0, 16, 175, 191);    // テクスチャを登録
	Sprite[0]->GetSpr()->Move(X,Y);
	Sprite[0]->GetSpr()->FCX += 32;
	Sprite[0]->GetSpr()->FCY += 32;
	Sprite[0]->GetSpr()->FVisible = true;
}

void TMoon::Move()
{
	X = RoundOff(Sprite[0]->GetSpr()->FCX);
	Y = RoundOff(Sprite[0]->GetSpr()->FCY);

	switch( ActFlg )
	{
	case 0:{
		Sprite[0]->MoveR(-4,0);
		if(Y>500 ) {
			Die();
			//FOwner->TempCount[0] = FOwner->Distance;
		}
		   }
		   break;
	case 1:{

		   }
		   break;
	case 2:{

		   }
		   break;
	}

	TOBJ::Move();
}

//  レンズフレア　　////////////////////////////////////////////////////////////////////////
TFlare::TFlare(TOBJList* _owner, s32 _x, s32 _y, f32 _a) : TOBJ(_owner)
{
	FObjectName = "TFlare";
	X = _x;
	Y = _y;
	FKind = kBG;
	if(_a<0 ) _a = 0;
	if(_a>1 ) _a = 1;

	_a = _a*0.5f;

	//近い順に
	TSpriteEXBLT sblt;
	sblt.RBM      =  bmAdd; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  QD;     // TDDDDの参照
	sblt.RHeight  =  256;        // 高さ
	sblt.RWidth   =  256;        // 幅
	sblt.RX       =  0;        // X座標
	sblt.RY       =  0;        // Y座標
	sblt.RZ       =  2300;       // Z値

	Sprite[0] = new TJoint(sblt);         // スプライトの生成
	numSpr++;
	Sprite[0]->GetSpr()->AddTexture(Eff5Tex,  0, 0, 255, 255);    // テクスチャを登録
	Sprite[0]->GetSpr()->FCX += 128;
	Sprite[0]->GetSpr()->FCY += 128;
	Sprite[0]->GetSpr()->FVisible = false;  //太陽本体のほうに移したので表示しない
	Sprite[0]->GetSpr()->Move(X,Y);

	sblt.RBM      =  bmAdd; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  QD;     // TDDDDの参照
	sblt.RHeight  =  128;        // 高さ
	sblt.RWidth   =  128;        // 幅
	sblt.RX       =  0;        // X座標
	sblt.RY       =  0;        // Y座標
	sblt.RZ       =  2200;       // Z値

	Sprite[1] = new TJoint(sblt);         // スプライトの生成
	numSpr++;
	Sprite[1]->GetSpr()->AddTexture(Eff4Tex,  0, 127, 127, 255);    // テクスチャを登録
	Sprite[1]->GetSpr()->FCX += 64;
	Sprite[1]->GetSpr()->FCY += 64;
	Sprite[1]->GetSpr()->SetColor(RoundOff(128*_a), ctRed);
	Sprite[1]->GetSpr()->SetColor(RoundOff(48*_a), ctBlue);
	Sprite[1]->GetSpr()->SetColor(RoundOff(48*_a), ctGreen);
	Sprite[1]->GetSpr()->Zoom(2,2);
	Sprite[1]->GetSpr()->Move(320+(s32)((f32)(X-320)*1.8f),240+(s32)((f32)(Y-240)*1.8f));
	Sprite[1]->GetSpr()->FVisible = true;

	sblt.RBM      =  bmAdd; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  QD;     // TDDDDの参照
	sblt.RHeight  =  128;        // 高さ
	sblt.RWidth   =  128;        // 幅
	sblt.RX       =  0;        // X座標
	sblt.RY       =  0;        // Y座標
	sblt.RZ       =  2100;       // Z値

	Sprite[2] = new TJoint(sblt);         // スプライトの生成
	numSpr++;
	Sprite[2]->GetSpr()->AddTexture(Eff4Tex,  129, 128, 255, 255);    // テクスチャを登録
	Sprite[2]->GetSpr()->FCX += 64;
	Sprite[2]->GetSpr()->FCY += 64;
	Sprite[2]->GetSpr()->SetColor(RoundOff(96*_a), ctRed);
	Sprite[2]->GetSpr()->SetColor(RoundOff(48*_a), ctBlue);
	Sprite[2]->GetSpr()->SetColor(RoundOff(48*_a), ctGreen);
	Sprite[2]->GetSpr()->Zoom(0.5,0.5);
	Sprite[2]->GetSpr()->Move(320+(s32)((f32)(X-320)*1.5f),240+(s32)((f32)(Y-240)*1.5f));
	Sprite[2]->GetSpr()->FVisible = true;

	sblt.RBM      =  bmAdd; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  QD;     // TDDDDの参照
	sblt.RHeight  =  128;        // 高さ
	sblt.RWidth   =  128;        // 幅
	sblt.RX       =  0;        // X座標
	sblt.RY       =  0;        // Y座標
	sblt.RZ       =  2000;       // Z値

	Sprite[3] = new TJoint(sblt);         // スプライトの生成
	numSpr++;
	Sprite[3]->GetSpr()->AddTexture(Eff4Tex,  0, 128, 127, 255);    // テクスチャを登録
	Sprite[3]->GetSpr()->FCX += 64;
	Sprite[3]->GetSpr()->FCY += 64;
	Sprite[3]->GetSpr()->SetColor(RoundOff(128*_a), ctRed);
	Sprite[3]->GetSpr()->SetColor(RoundOff(64*_a), ctBlue);
	Sprite[3]->GetSpr()->SetColor(RoundOff(64*_a), ctGreen);
	Sprite[3]->GetSpr()->Zoom(0.2f,0.2f);
	Sprite[3]->GetSpr()->Move(320+(s32)((f32)(X-320)*0.6f),240+(s32)((f32)(Y-240)*0.6f));
	Sprite[3]->GetSpr()->FVisible = true;

	sblt.RBM      =  bmAdd; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  QD;     // TDDDDの参照
	sblt.RHeight  =  128;        // 高さ
	sblt.RWidth   =  128;        // 幅
	sblt.RX       =  0;        // X座標
	sblt.RY       =  0;        // Y座標
	sblt.RZ       =  1900;       // Z値

	Sprite[4] = new TJoint(sblt);         // スプライトの生成
	numSpr++;
	Sprite[4]->GetSpr()->AddTexture(Eff4Tex,  0, 128, 127, 255);    // テクスチャを登録
	Sprite[4]->GetSpr()->FCX += 64;
	Sprite[4]->GetSpr()->FCY += 64;
	Sprite[4]->GetSpr()->SetColor(RoundOff(96*_a), ctRed);
	Sprite[4]->GetSpr()->SetColor(RoundOff(48*_a), ctBlue);
	Sprite[4]->GetSpr()->SetColor(RoundOff(96*_a), ctGreen);
	Sprite[4]->GetSpr()->Zoom(0.3f,0.3f);
	Sprite[4]->GetSpr()->Move(320+(s32)((f32)(X-320)*0.2f),240+(s32)((f32)(Y-240)*0.2f));
	Sprite[4]->GetSpr()->FVisible = true;

	sblt.RBM      =  bmAdd; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  QD;     // TDDDDの参照
	sblt.RHeight  =  128;        // 高さ
	sblt.RWidth   =  128;        // 幅
	sblt.RX       =  0;        // X座標
	sblt.RY       =  0;        // Y座標
	sblt.RZ       =  1800;       // Z値

	Sprite[5] = new TJoint(sblt);         // スプライトの生成
	numSpr++;
	Sprite[5]->GetSpr()->AddTexture(Eff4Tex,  0, 128, 127, 255);    // テクスチャを登録
	Sprite[5]->GetSpr()->FCX += 64;
	Sprite[5]->GetSpr()->FCY += 64;
	Sprite[5]->GetSpr()->SetColor(RoundOff(255*_a), ctRed);
	Sprite[5]->GetSpr()->SetColor(RoundOff(255*_a), ctBlue);
	Sprite[5]->GetSpr()->SetColor(RoundOff(255*_a), ctGreen);
	Sprite[5]->GetSpr()->Zoom(0.9f,0.9f);
	Sprite[5]->GetSpr()->Move(320-(s32)((f32)(X-320)*0.3f),240-(s32)((f32)(Y-240)*0.3f));
	Sprite[5]->GetSpr()->FVisible = true;

	sblt.RBM      =  bmAdd; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  QD;     // TDDDDの参照
	sblt.RHeight  =  128;        // 高さ
	sblt.RWidth   =  128;        // 幅
	sblt.RX       =  0;        // X座標
	sblt.RY       =  0;        // Y座標
	sblt.RZ       =  1700;       // Z値

	Sprite[6] = new TJoint(sblt);         // スプライトの生成
	numSpr++;
	Sprite[6]->GetSpr()->AddTexture(Eff4Tex,  129, 128, 255, 255);    // テクスチャを登録
	Sprite[6]->GetSpr()->FCX += 64;
	Sprite[6]->GetSpr()->FCY += 64;
	Sprite[6]->GetSpr()->SetColor(RoundOff(48*_a), ctRed);
	Sprite[6]->GetSpr()->SetColor(RoundOff(32*_a), ctBlue);
	Sprite[6]->GetSpr()->SetColor(RoundOff(48*_a), ctGreen);
	Sprite[6]->GetSpr()->FVisible = true;
	Sprite[6]->GetSpr()->Zoom(0.3f,0.3f);
	Sprite[6]->GetSpr()->Move(320-(s32)((f32)(X-320)*0.5f),240-(s32)((f32)(Y-240)*0.5f));

	sblt.RBM      =  bmAdd; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  QD;     // TDDDDの参照
	sblt.RHeight  =  128;        // 高さ
	sblt.RWidth   =  128;        // 幅
	sblt.RX       =  0;        // X座標
	sblt.RY       =  0;        // Y座標
	sblt.RZ       =  1600;       // Z値

	Sprite[7] = new TJoint(sblt);         // スプライトの生成
	numSpr++;
	Sprite[7]->GetSpr()->AddTexture(Eff4Tex,  0, 128, 127, 255);    // テクスチャを登録
	Sprite[7]->GetSpr()->FCX += 64;
	Sprite[7]->GetSpr()->FCY += 64;
	Sprite[7]->GetSpr()->SetColor(RoundOff(96*_a), ctRed);
	Sprite[7]->GetSpr()->SetColor(RoundOff(96*_a), ctBlue);
	Sprite[7]->GetSpr()->SetColor(RoundOff(255*_a), ctGreen);
	Sprite[7]->GetSpr()->FVisible = true;
	Sprite[7]->GetSpr()->Zoom(0.4f,0.4f);
	Sprite[7]->GetSpr()->Move(320-(s32)((f32)(X-320)*0.9f),240-(s32)((f32)(Y-240)*0.9f));

	sblt.RBM      =  bmAdd; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  QD;     // TDDDDの参照
	sblt.RHeight  =  128;        // 高さ
	sblt.RWidth   =  128;        // 幅
	sblt.RX       =  0;        // X座標
	sblt.RY       =  0;        // Y座標
	sblt.RZ       =  1500;       // Z値

	Sprite[8] = new TJoint(sblt);         // スプライトの生成
	numSpr++;
	Sprite[8]->GetSpr()->AddTexture(Eff4Tex,  129, 128, 255, 255);    // テクスチャを登録
	Sprite[8]->GetSpr()->FCX += 64;
	Sprite[8]->GetSpr()->FCY += 64;
	Sprite[8]->GetSpr()->SetColor(RoundOff(160*_a), ctRed);
	Sprite[8]->GetSpr()->SetColor(RoundOff(96*_a), ctBlue);
	Sprite[8]->GetSpr()->SetColor(RoundOff(96*_a), ctGreen);
	Sprite[8]->GetSpr()->Zoom(1.5f,1.5f);
	Sprite[8]->GetSpr()->FVisible = true;
	Sprite[8]->GetSpr()->Move(640-X,480-Y);

	sblt.RBM      =  bmAdd; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  QD;     // TDDDDの参照
	sblt.RHeight  =  256;        // 高さ
	sblt.RWidth   =  256;        // 幅
	sblt.RX       =  0;        // X座標
	sblt.RY       =  0;        // Y座標
	sblt.RZ       =  1400;       // Z値

	Sprite[9] = new TJoint(sblt);         // スプライトの生成
	numSpr++;
	Sprite[9]->GetSpr()->AddTexture(Eff4Tex,  129, 128, 255, 255);    // テクスチャを登録
	Sprite[9]->GetSpr()->FCX += 128;
	Sprite[9]->GetSpr()->FCY += 128;
	Sprite[9]->GetSpr()->SetColor(RoundOff(128*_a), ctRed);
	Sprite[9]->GetSpr()->SetColor(RoundOff(196*_a), ctBlue);
	Sprite[9]->GetSpr()->SetColor(RoundOff(128*_a), ctGreen);
	Sprite[9]->GetSpr()->FVisible = true;
	Sprite[9]->GetSpr()->Zoom(1.5f,1.5f);
	Sprite[9]->GetSpr()->Move(320-(s32)((f32)(X-320)*2),240-(s32)((f32)(Y-240)*2));
}

void TFlare::Move()
{
	Die();
	TOBJ::Move();
}

//  縦スクロール　　////////////////////////////////////////////////////////////////////////
TVScroll::TVScroll(TOBJList* _owner) : TOBJ(_owner)
{
	FObjectName = "TVScroll";
	ActFlg = 0;
	ActCount = 0;
	TotalFlight = 0;
	FKind = kBG;
	for( s32 i = 0; i <= 3; i++ ){
		ClSet[i] = false;
	}
}

void TVScroll::Move()
{
	s32 tempRad,prevRad;
	const s32 KDISTANCE_TOPTOBOTTOM = 1000;
	tempRad = 0;
	//空の高さ＝円の直径
	//角度＝TotalFlight　として、１フレームあたりのY軸移動量を計算
	//TotalFlightがKDISTANCE_TOPTOBOTTOMに達したとき、円は半周して直径KSKY_HIGHT_MAXを示す
	if(ActFlg<=2 ) {
		tempRad =  RoundOff(2048.f / (f32)(KDISTANCE_TOPTOBOTTOM*TotalFlight));
		prevRad =  RoundOff(2048.f / (f32)(KDISTANCE_TOPTOBOTTOM*(TotalFlight-FOwner->Speed)));
		if(prevRad>0 ) FOwner->BGSlide.Y = -RoundOff(KSKY_HIGHT_MAX/2*(cost[tempRad]-cost[prevRad]));
	}

	switch( ActFlg )
	{
	case 0:{
		if(TotalFlight>=100 ) {
			ActFlg = 1;
			if(CreateSE ) FOwner->SoundEffect->WavPlay(WAVScroll,5);
		}
		   }
		   break;
	case 1:{
		if(tempRad>=1024 ) {
			ActFlg = 2;
			FOwner->Add( new TSunShine(FOwner));
			FOwner->Add( new TSun2(FOwner,540,-220));
			FOwner->Add( new TTransGene(FOwner));
		}
		   }
		   break;
	case 2:{
		if(tempRad>=2048 ) {
			FOwner->BGSlide.Y = 0;
			ActFlg = 3;
		}
		   }
		   break;
	case 3:{
		if(TotalFlight>2000 ) {
			FOwner->Add( new TCloudMaker2(FOwner));
			FOwner->BGSlide.Y = 0;
			Die();
		}
		   }
		   break;
	}
	TotalFlight += FOwner->Speed;
	TOBJ::Move();
}

//  縦スクロール(下降)　　////////////////////////////////////////////////////////////////////////
TDownScroll::TDownScroll(TOBJList* _owner) : TOBJ(_owner)
{
	FObjectName = "TDownScroll";
	ActFlg = 0;
	ActCount = 0;
	TotalFlight = 0;
	FKind = kBG;
	Y = KSKY_HIGHT_MAX;
}

void TDownScroll::Move()
{
	const s32 KDISTANCE_NEXTACT = 500;
	const s32 KDISTANCE_TOPTOBOTTOM = 1000;
	s32 tempRad,prevRad;
	switch( ActFlg ) {
	case 0:{
		if(TotalFlight>=KDISTANCE_NEXTACT ) {
			ActFlg = 1;
			//          PutDebugLog(FObjectName +" TotalFlight = " + IntToStr(TotalFlight));
			if(CreateSE ) FOwner->SoundEffect->WavPlay(WAStart,0);
		}
		   }
		   break;
	case 1:{
		tempRad =  RoundOff(2048.f / (f32)(KDISTANCE_TOPTOBOTTOM*(TotalFlight-KDISTANCE_NEXTACT)));
		prevRad =  RoundOff(2048.f / (f32)(KDISTANCE_TOPTOBOTTOM*(TotalFlight-FOwner->Speed-KDISTANCE_NEXTACT)));
		if((tempRad>=0) && (prevRad>=0) ) FOwner->BGSlide.Y = RoundOff((KSKY_HIGHT_MAX/2)*(cost[tempRad]-cost[prevRad]));
		if(tempRad>=2048 ) {
			FOwner->BGSlide.Y = 0;
			Die();
		}
		   }
		   break;
	}
	TotalFlight += FOwner->Speed;
	TOBJ::Move();
}

TSun2::TSun2(TOBJList* _owner, s32 _x, s32 _y) : TOBJ(_owner)
{
	FObjectName = "TSun2";
	X = _x;
	Y = _y;
	ActFlg = 0;
	ActCount = 0;
	FKind = kBG;
	//FlareOK = true;

	MakeSpriteLite(QD, numSpr, X, Y, 320, 320, 25400, bmNormal);
	Sprite[numSpr-1]->GetSpr()->AddTexture(SunTex,  128, 1, 255, 127);    // テクスチャを登録
	Sprite[numSpr-1]->GetSpr()->SetColor(128, ctAlpha);
	Sprite[numSpr-1]->GetSpr()->SetColor(160, ctRed);
	Sprite[numSpr-1]->GetSpr()->SetColor(160, ctGreen);
	Sprite[numSpr-1]->GetSpr()->SetColor(160, ctBlue);
	Sprite[numSpr-1]->GetSpr()->FCX += 160;
	Sprite[numSpr-1]->GetSpr()->FCY += 160;
	Sprite[numSpr-1]->GetSpr()->Move(X,Y);

	MakeSpriteLite(QD, numSpr,X, Y, 1218,1218,25399,bmAdd);
	Sprite[numSpr-1]->GetSpr()->AddTexture(SunTex,  1, 1, 127, 127);    // テクスチャを登録
	Sprite[numSpr-1]->GetSpr()->SetColor(255, ctAlpha);
	Sprite[numSpr-1]->GetSpr()->FCX += 609;
	Sprite[numSpr-1]->GetSpr()->FCY += 609;
	Sprite[numSpr-1]->GetSpr()->Move(X,Y);

	MakeSpriteLite(QD, numSpr,X, Y, 190,190,25398,bmAdd);
	Sprite[numSpr-1]->GetSpr()->AddTexture(SunTex,  1, 1, 127, 127);    // テクスチャを登録
	Sprite[numSpr-1]->GetSpr()->SetColor(192, ctAlpha);
	Sprite[numSpr-1]->GetSpr()->SetColor(160, ctRed);
	Sprite[numSpr-1]->GetSpr()->FCX += 95;
	Sprite[numSpr-1]->GetSpr()->FCY += 95;
	Sprite[numSpr-1]->GetSpr()->Move(X,Y);

	MakeSpriteLite(QD, numSpr,X, Y, 160,160,25397,bmAdd);
	Sprite[numSpr-1]->GetSpr()->AddTexture(SunTex,  1, 128, 127, 255);    // テクスチャを登録
	Sprite[numSpr-1]->GetSpr()->SetColor(64, ctAlpha);
	Sprite[numSpr-1]->GetSpr()->FCX += 80;
	Sprite[numSpr-1]->GetSpr()->FCY += 80;
	Sprite[numSpr-1]->GetSpr()->Move(X,Y);

	for( s32 i = 1; i < numSpr; i++ ) {
		Sprite[0]->Add(Sprite[i]);
	}

	if( ExtraEffects ) {
		RainFilter = new TRainFilter(FOwner);
		FOwner->Add(RainFilter);
	}
}

void TSun2::Die()
{
	TOBJ::Die();
	if(ExtraEffects ) {
		RainFilter->Die();
	}
}

void TSun2::Move()
{

	f32 Alpha;
	X = RoundOff(Sprite[0]->GetSpr()->FCX);
	Y = RoundOff(Sprite[0]->GetSpr()->FCY);

	switch( ActFlg ) {
	case 0:{
		//出現
		Alpha = ActCount*0.02f;
		if( Alpha>1 ) Alpha = 1;
		if( FOwner->FlareOK )
			FOwner->Add( new TFlare(FOwner,X,Y,Alpha));

		Sprite[0]->MoveR(-(f32)FOwner->BGSlide.Y * 0.5f ,0);    //横方向の移動スピードは縦方向の背景移動スピードに準拠
		ActCount++;
		if(FOwner->BGSlide.Y==0 ) {
			ActFlg++;
			ActCount = 0;
		}

		for( s32 i = 0; i < numSpr; i++ ) {
			if(Sprite[i]->GetSpr()->FLuminance < 500 )
				Sprite[i]->GetSpr()->FLuminance += 16;
		}

		   }
		   break;
	case 1:{
		//何もしない
		FOwner->Add( new TFlare(FOwner,X,Y,1));
		if(! FOwner->FlareOK ) {
			ActFlg++;
			ActCount = 0;
		}
		   }
	case 2:{
		//雲に隠れる
		if(ActCount<=20 ) {
			Alpha = 1-ActCount*0.05f;
			FOwner->Add( new TFlare(FOwner,X,Y,Alpha));
		}

		ActCount++;

		if(FOwner->FlareOK ) {
			ActFlg++;
			ActCount = 0;
			FOwner->Add( new TSunShine(FOwner));
		}

		for( s32 i = 0; i < numSpr; i++ ) {
			if(Sprite[i]->GetSpr()->FLuminance >= 100 )
				Sprite[i]->GetSpr()->FLuminance = Sprite[i]->GetSpr()->FLuminance-4;

		}
		break;
		   }
	case 3:{
		//雲が過ぎ去る
		Alpha = ActCount*0.1f;
		if(Alpha>1 ) Alpha = 1;

		FOwner->Add( new TFlare(FOwner,X,Y,Alpha));

		ActCount++;

		for( s32 i = 0; i < numSpr ) {
			Sprite[i]->GetSpr()->FLuminance  = 500;
		}
		if(FOwner->BGSlide.Y != 0 ) {
			ActFlg++;
			ActCount = 0;
		}
		   }
		   break;
	case 4:{
		//下降
		Sprite[0]->MoveR(FOwner->BGSlide.Y/2.f ,0);    //横方向の移動スピードは縦方向の背景移動スピードに準拠
		ActCount++;
		FOwner->Add( new TFlare(FOwner,X,Y,1-ActCount*0.01f));

		for( s32 i = 0; i < numSpr; i++ ) {
			if(Sprite[i]->GetSpr()->FLuminance >= 100 )
				Sprite[i]->GetSpr()->FLuminance -= 4;
		}

		if(Sprite[0]->GetSpr()->FLuminance <= 100 ) Die();
		   }
	}
	TOBJ::Move();
}

TCloudFast::TCloudFast(TOBJList* _owner, s32 _x, s32 _y) : TOBJ(_owner)
{
	FObjectName = "TCloudFast";
	ZoomRate = random(80)/10+12;
	PersonalR = random(4095);
	IncR = random(64)-32;
	FKind = kEffect;


	MakeSpriteLite(QD, numSpr,_x,_y,64,64,2000,bmNormal);
		Sprite[numSpr-1]->GetSpr()->AddTexture(Eff8Tex, 33, 129, 63, 159);
		Sprite[numSpr-1]->GetSpr()->FCX += 32;
		Sprite[numSpr-1]->GetSpr()->FCY += 32;
		Sprite[numSpr-1]->GetSpr()->Rotate(PersonalR);
		Sprite[numSpr-1]->GetSpr()->Zoom(ZoomRate,ZoomRate/3.f);
		Sprite[numSpr-1]->GetSpr()->MoveR(_x + Sprite[numSpr-1]->GetSpr()->FCX, 0.f);

		//    SetColor(240, ctRed);
		Sprite[numSpr-1]->GetSpr()->SetColor(240, ctGreen);
		Sprite[numSpr-1]->GetSpr()->SetColor(160, ctAlpha);
		Sprite[numSpr-1]->GetSpr()->FToneMapping = true;
		//    FLuminance = 80;
}

void TCloudFast::Move()
{
	TOBJ::Move();
	Sprite[0]->MoveR(-40-FOwner->Speed*2,0);

	X = RoundOff(Sprite[0]->GetSpr()->FCX);
	if(X<-600 ) Die();

	Sprite[0]->GetSpr()->Rotate(PersonalR);
	Sprite[0]->GetSpr()->Zoom(1,1);
	PersonalR += IncR;
	Sprite[0]->GetSpr()->Rotate(PersonalR);
	Sprite[0]->GetSpr()->Zoom(ZoomRate,ZoomRate/3.f);
	Sprite[0]->GetSpr()->Rotate((PersonalR) & 0x0fff);
	Sprite[0]->GetSpr()->SetGradation(2500, 0xaa8800,1);
}

TSandStorm::TSandStorm(TOBJList* _owner, s32 _x, s32 _y) : TOBJ(_owner)
{
	FObjectName = "TSandStorm";
	FKind = kEffect2;

	for( s32 i = 0; i <= 7; i++ ) {
		MakeSpriteLite(QD, numSpr,160*i,0,160,480,18000,bmMul);


		if(FOwner->NowStage==4 )
			Sprite[numSpr-1]->GetSpr()->AddTexture(Eff7Tex,  32*i, 0, 31+32*i, 255);
		else
			Sprite[numSpr-1]->GetSpr()->AddTexture(ST2Tex1,  32*i, 0, 31+32*i, 255);

		Sprite[numSpr-1]->GetSpr()->SetColor(144, ctRed);
		Sprite[numSpr-1]->GetSpr()->SetColor(160, ctBlue);
		Sprite[numSpr-1]->GetSpr()->SetColor(160, ctGreen);
	}

	for( s32 i = 0; i <= 7; i++ ) {
		MakeSpriteLite(QD, numSpr,96*i,0,96,480,8001,bmMul);

		if(FOwner->NowStage==4 )
			Sprite[numSpr-1]->GetSpr()->AddTexture(Eff7Tex,   32*i, 64, 31+32*i, 192);
		else
			Sprite[numSpr-1]->GetSpr()->AddTexture(ST2Tex1,   32*i, 64, 31+32*i, 192);
	}
}

void TSandStorm::Move()
{
	TOBJ::Move();

	if((FOwner->NowStage==4) || ( (TStageSelectScreenMng*)(FOwner->StageSelect)->GetCurrentStage()==4) ) {

		for( s32 i = 0; i <= 7; i++ ) {
			Sprite[i]->MoveR(-32,0);
			if(Sprite[i]->GetSpr()->FCX<-160 )
				Sprite[i]->GetSpr()->MoveR(160*8,0);

			if(Sprite[i]->GetSpr()->FCX > 640 )
				Sprite[i]->GetSpr()->FVisible = false;
			else
				Sprite[i]->GetSpr()->FVisible = true;
		}

		for( s32 i = 8; i <= 15; i++ ) {
			Sprite[i]->MoveR(-16,0);
			if(Sprite[i]->GetSpr()->FCX<-96 )
				Sprite[i]->GetSpr()->MoveR(96*8,0);

			if(Sprite[i]->GetSpr()->FCX > 640 )
				Sprite[i]->GetSpr()->FVisible = false;
			else
				Sprite[i]->GetSpr()->FVisible = true;
		}

		if(FOwner->Distance>=39200 ) {
			//      for i  =  0 to numSpr-1 do {
			//        Sprite[i]->GetSpr()->SetColorR(8, ctRed);
			//        Sprite[i]->GetSpr()->SetColorR(8, ctBlue);
			//        Sprite[i]->GetSpr()->SetColorR(8, ctGreen);
			//      }
			FOwner->Add( new TBoxContrl(FOwner,5));
			if(CreateBGM ) FOwner->MusicPlayer->Stop();
			FOwner->Add( new TStage5MusicControl(FOwner));
			Die();
		}
	}
	else {
		for( s32 i = 0; i < numSpr; i++ ) {
			Sprite[i]->GetSpr()->FVisible = false;
		}
	}

}

TSky_StageSelect::TSky_StageSelect(TOBJList* _owner) : TOBJ(_owner)
{
	FObjectName = "TSky_StageSelect";

	X = -16;
	Y = -16;
	FKind = kBG;

	TSpriteEXBLT sblt;
	sblt.RBM      =  bmNormal; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  QD;     // TDDDDの参照
	sblt.RHeight  =  512;        // 高さ
	sblt.RWidth   =  672;        // 幅
	sblt.RX       =  X;        // X座標
	sblt.RY       =  Y;        // Y座標
	sblt.RZ       =  25500;       // Z値

	Sprite[0] = new TJoint(sblt);         // 背景画像一枚目
	numSpr++;

	Sprite[0]->GetSpr()->AddTexture(BGTex1, 1, 1, 256, 256);    // テクスチャを登録
	Sprite[0]->GetSpr()->SetColor(240, ctRed);
	Sprite[0]->GetSpr()->SetColor(240, ctGreen);
	Sprite[0]->GetSpr()->SetColor(240, ctBlue);
	Sprite[0]->GetSpr()->FVisible = true;
	Sprite[0]->GetSpr()->Move(X,Y);
}

void TSky_StageSelect::Move()
{
	TOBJ::Move();

	Sprite[0]->Move(X,Y);
	if(FOwner->NowStage==99 ) {
		if((TStageSelectScreenMng*)(FOwner->StageSelect).GetCurrentStage()==0 )
			Sprite[0]->GetSpr()->FVisible = true;
		else
			Sprite[0]->GetSpr()->FVisible = false;
	}
}

TStage5MusicControl::TStage5MusicControl(TOBJList* _owner) : TOBJ(_owner)
{
	FObjectName = "TStage5MusicControl";
	FKind = kEffect2;
}

void TStage5MusicControl::Move()
{
	TOBJ::Move();
	//3秒後にボス音楽をならすだけ
	if(Age = 180 ) {
		if(CreateBGM ) FOwner->MusicPlayer->Play(1);
		Die();
	}
}

