#include "BG2.h"
#include "SXScene.h"

TDawn::TDawn( TOBJList* _owner ) : TOBJ(_owner)
{
	X = -16;
	Y = -110;
	//Y = -16;
	FKind = kBG;
}

TDawn::~TDawn()
{
}

s32 TDawn::OnDraw()
{
	UVRect uv;

	uv.u1 = 0;
	uv.u2 = 1;
	uv.v1 = 0;
	uv.v2 = 1;
	FOwner->GetScene()->SetSpriteSX(X,Y,640+32,480+32,1000,uv,255,BGTex1,sxbAlpha);
	FOwner->GetScene()->SetSpriteSX(X,Y-480-31,640+32,480+32,1000,uv,255,BGTex2,sxbAlpha);
	//  FOwner->GetScene()->SetEffectSprite(X,Y,640+32,480+32,uv,255,BGTex1,sxbAlpha);

	return 1;
}


TSea::TSea( TOBJList* _owner, s32 _x, s32 _z ) : TOBJ(_owner)
{
	FKind = kBG;
	PosX = _x;
	PosZ = _z;

	MeshFrame = new TFrameManager(FOwner->GetScene(), NULL,Board,Eff7Tex,sxbMul,false,false,false);
	MeshFrame->Frame->SetParent(FOwner->GetScene()->FogFrame);
	MeshFrame->SetColor(128,96,96,96);
	MeshFrame->Move(_x,-0.8f,_z);
	MeshFrame->ZoomR(10,1,4);
}

TSea::~TSea()
{
	SAFE_DELETE(MeshFrame);
}

void TSea::Move()
{

	f32 cx;
	f32 cy;
	f32 cz;
	//MeshFrame->RotateR(0,20,0);
	MeshFrame->SlideUV(0.023f*(FOwner->Speed / 10.f),0.00f);
	FOwner->GetScene()->GetCameraPos(cx,cy,cz);
	MeshFrame->Move(cx+PosX,-0.8f,PosZ);

	TOBJ::Move();
}

TUnderSea::TUnderSea( TOBJList* _owner ) : TOBJ(_owner)
{

	MeshFrame[0] = new TFrameManager(FOwner->GetScene(),NULL,Undersea,BGTex2,sxbDefault,false,false,false);
	//  MeshFrame[0]->Frame.Parent  =  FOwner->GetScene()->FogFrame;
	MeshFrame[1] = new TFrameManager(FOwner->GetScene(),MeshFrame[0],Undersea2,BGTex2,sxbDefault,false,false,false);
	MeshFrame[2] = new TFrameManager(FOwner->GetScene(),MeshFrame[0],Undersea2,BGTex2,sxbDefault,false,false,false);

	MeshFrame[0]->Zoom(5,1,16);
	for( s32 i = 1; i<= 2; i++) {
		MeshFrame[i]->Zoom(9,1,16);
	}
	MeshFrame[0]->Move(0,-0.81f,14);

	MeshFrame[1]->MoveR(-14,0,0);
	MeshFrame[2]->MoveR(14,0,0);
}

TUnderSea::~TUnderSea()
{
	for( s32 i = 1; i <= 2; i++ ){
		SAFE_DELETE( MeshFrame[i] );
	}

	SAFE_DELETE( MeshFrame[0] );
}

void TUnderSea::Move()
{
	f32 cx;
	f32 cy;
	f32 cz;

	TOBJ::Move();
	FOwner->GetScene()->GetCameraPos(cx,cy,cz);
	MeshFrame[0]->Move(cx,-0.81f,14);
}

TDawncloud::TDawncloud( TOBJList* _owner ) : TOBJ(_owner)
{
	MeshFrame = new TFrameManager(FOwner->GetScene(),NULL,CloudMesh,Eff6Tex,sxbAlpha,false,false,false);

	MeshFrame->SetColor(196,255,255,255);
	MeshFrame->Zoom(100,150,20);
	MeshFrame->RotateR(3176,0,0);
	MeshFrame->Move(0.0,-150, 1000);
}

TDawncloud::~TDawncloud()
{
	SAFE_DELETE( MeshFrame );
}

void TDawncloud::Move()
{
	f32 cx;
	f32 cy;
	f32 cz;
		MeshFrame->SlideUV(0.0002f,-0.0005f);

	FOwner->GetScene()->GetCameraPos(cx,cy,cz);
	MeshFrame->Move(cx,-150,800);
}

TFog::TFog( TOBJList* _owner ) : TOBJ(_owner)
{
	X = 0;
	Y = 239;
	FKind = kBG;
	Zoomrate = 1.0f;
}

void TFog::Move()
{
	TOBJ::Move();
}

s32 TFog::OnDraw()
{
	UVRect uv;
	uv.u1 = 80.f / 255.f;
	uv.u2 = 112.f / 255.f;
	uv.v1 = 0;
	uv.v2 = 32.f / 255.f;
	FOwner->GetScene()->SetSpriteSX(0-16,Y,640+32,(s32)(16.f*Zoomrate),9,uv,255,ST2Tex1,sxbAlpha);

	return 1;
}

TDawnSun::TDawnSun( TOBJList* _owner ) : TOBJ(_owner)
{
	X = 320-55;
	Y = 170;
	FKind = kBG; 
}

void TDawnSun::Move()
{
	f32 cx;
	f32 cy;
	f32 cz;
	FOwner->GetScene()->GetCameraPos(cx,cy,cz);
	PosX  =  cx;
	PosY  =  2;

	TOBJ::Move();
}

s32 TDawnSun::OnDraw()
{
	UVRect uv;
	uv.u1 = 0;
	uv.u2 = 79.f / 255.f;
	uv.v1 = 0;
	uv.v2 = 79.f / 255.f;
	FOwner->GetScene()->SetBillBoardSX(PosX,PosY,25,120,120,uv,160,ST2Tex1,sxbAdd, false);

	return 1;
}

TDawnSunReflection::TDawnSunReflection( TOBJList* _owner ) : TOBJ(_owner)
{
	X = 320-100;
	Y = 50;
	FKind = kBG;
}

void TDawnSunReflection::Move()
{
	TOBJ::Move();
}

s32 TDawnSunReflection::OnDraw()
{
	UVRect uv;
	uv.u1 = 0;
	uv.u2 = 79.f / 255.f;
	uv.v1 = 0;
	uv.v2 = 79.f / 255.f;
	FOwner->GetScene()->SetSpriteSX(X,Y,200,300,1,uv,20,ST2Tex1,sxbAdd);

	return 1;
}

TBuildingBase::TBuildingBase( TOBJList* _owner, f32 _Z, s32 _rx, s32 _ry, s32 _rz, f32 _zoom ) : TOBJ(_owner)
{
	FKind = kBG;
	Speed =  0.125;

	MeshFrame = new TFrameManager(FOwner->GetScene(),NULL,Building[0],NULL,sxbDefault ,false,false,false);

	MeshFrame->Move(8,0,_Z);
	MeshFrame->Rotate(_rx,_ry,_rz);
	MeshFrame->Zoom(_zoom*(random(10)*0.1f+0.1f),_zoom*(random(20)*0.1f+0.1f),_zoom*(random(20)*0.1f+0.1f));
	MeshFrame->SetColor(255,0,0,0);
	//FOwner->Add(TBuildingShadow.Create(FOwner,_Z,_rx,_ry,_rz,_zoom));
}

TBuildingBase::~TBuildingBase()
{
	SAFE_DELETE( MeshFrame );
}

void TBuildingBase::Move()
{
	//Speed = DData.FCURRENT[5];
	MeshFrame->MoveR(-Speed*(FOwner->Speed/10),0,0);

	if( MeshFrame->GetX() < -8 ) 
		Die();

	TOBJ::Move();
}

TBuildingShadow::TBuildingShadow( TOBJList* _owner, f32 _Z, s32 _rx, s32 _ry, s32 _rz, f32 _zoom ) : TOBJ(_owner)
{
	FKind = kBG;
	Speed = 0.125f;

	MeshFrame = new TFrameManager(FOwner->GetScene(),NULL,Building[0],NULL,sxbMul ,false,false,false);

	MeshFrame->Move(8,-0.2f,_Z);
	MeshFrame->Zoom(_zoom*(random(10)*0.05f+0.8f),_zoom*(random(20)*0.1f+1.f),_zoom*(random(20)*0.1f+1.f));
	MeshFrame->Rotate(_rx,_ry,_rz);
	MeshFrame->RotateR(0,1024,0);
	MeshFrame->ZoomR(1,1,0);
	MeshFrame->SetColor(255,128,128,128);
}

TBuildingShadow::~TBuildingShadow()
{
	SAFE_DELETE( MeshFrame );
}

void TBuildingShadow::Move()
{
	//Speed = DData.FCURRENT[5];
	MeshFrame->MoveR(-Speed*(FOwner->Speed/10),0,0);

	if( MeshFrame->GetX()<-8 ) Die();

	TOBJ::Move();
}

TBuildingMaker::TBuildingMaker( TOBJList* _owner ) : TOBJ(_owner)
{
	FKind = kBG;
	Z = 0.1f;
	RX = 0;
	RY = 0;
	RZ = 0;
	Zoom = 1;

	interval = random(100)+200;
}

void TBuildingMaker::Move()
{
	if( FOwner->Distance >= interval ) {

		Z = (f32)random(600) / 100.f+2.f;
		RX = random(256)-128;
		RY = random(256)-128;
		RZ = 1024+random(256)-128;
		Zoom = (f32)(random(40)/100)+0.5f;
		FOwner->Add( new TBuildingBase(FOwner,Z,RX,RY,RZ,Zoom));
		//FOwner->Add(TBuildingShadow.Create(FOwner,Z,RX,RY,RZ,Zoom));
		interval += random(200)+200;
	}
	TOBJ::Move();
}

TRockBase::TRockBase( TOBJList* _owner, f32 _Z, s32 _rx, s32 _ry, s32 _rz, f32 _zoom ) : TOBJ(_owner)
{
	FKind = kBG;
	Speed =  0.125f;

	MeshFrame = new TFrameManager(FOwner->GetScene(),NULL,Rock[0],NULL,sxbDefault ,false,false,false);

	MeshFrame->Move(8.f,-(f32)random(10)*0.1f+0.3f,_Z);
	MeshFrame->Rotate(_rx,_ry,_rz);
	MeshFrame->Zoom(_zoom,_zoom,_zoom);
	MeshFrame->SetColor(255,0,0,0);
}

TRockBase::~TRockBase()
{
	SAFE_DELETE( MeshFrame );
}

void TRockBase::Move()
{
	//Speed = DData.FCURRENT[5];
	MeshFrame->MoveR(-Speed*(FOwner->Speed/10),0,0);

	if( MeshFrame->GetX()<-8 ) Die();

	TOBJ::Move();
}

TRockMaker::TRockMaker( TOBJList* _owner ) : TOBJ(_owner)
{
	FKind = kBG;
	Z = 0.1f;
	RX = 0;
	RY = 0;
	RZ = 0;
	Zoom = 1.f;

	interval = random(100)+200;
}

void TRockMaker::Move()
{
	if(FOwner->Distance >= interval ) {
		Z = (f32)random(600)/100.f+2.f;
		RX = random(1024)-512;
		RY = random(2048)-1024;
		RZ = random(256)-128+random(2)*2048;
		Zoom = (f32)random(4)/100.f+0.2f;
		FOwner->Add( new TRockBase(FOwner,Z,RX,RY,RZ,Zoom));
		interval += random(200)+200;
	}
	TOBJ::Move();
}

TSky_new::TSky_new( TOBJList* _owner ) : TOBJ(_owner)
{
	X = -16;
	Y = -16;
	FKind = kBG;

}

void TSky_new::Move()
{
	TOBJ::Move();
}

s32 TSky_new::OnDraw()
{
	UVRect uv;
	uv.u1 = 0;
	uv.u2 = 1.f;
	uv.v1 = 0;
	uv.v2 = 1.f;
	FOwner->GetScene()->SetSpriteSX(0-16,0-16,640+16,480+16,1000,uv,255,BGTex2,sxbAlpha);

	return 1;
}

TCloud_new::TCloud_new( TOBJList* _owner ) : TOBJ(_owner)
{
	FKind = kBG;

	MeshFrame = new TFrameManager(FOwner->GetScene(),NULL,Board,MechaTex,sxbAlpha,false,false,false);                //sxbMul

	for( s32 i = 0; i <= 5; i++ ) {
		SonMesh[i] = new TFrameManager(FOwner->GetScene(),MeshFrame,Board,MechaTex,sxbAlpha,false,false,false);
	}
	SonMesh[0]->MoveR(0,0,-2);
	SonMesh[1]->MoveR(-2,0,0);
	SonMesh[2]->MoveR(2,0,0);
	SonMesh[3]->MoveR(-2,0,2);
	SonMesh[4]->MoveR(0,0,2);
	SonMesh[5]->MoveR(2,0,2);

	//MeshFrame->SetColor(128,128,128,128);
	//MeshFrame->Move(0,-0.04,1.5);
	MeshFrame->Move(0,-0.25f,5.8f);
	MeshFrame->Zoom(3.f,1.f,1.8f);
	//MeshFrame->Zoom(2,1,1.5);
}

TCloud_new::~TCloud_new()
{
	for( s32 i = 0; i <= 5; i++ ) {
		SAFE_DELETE( SonMesh[i] );
	}

	SAFE_DELETE( MeshFrame );
}

void TCloud_new::Move()
{
	//MeshFrame->RotateR(0,20,0);
	MeshFrame->SlideUV(0.023f*(FOwner->Speed/10.f),0.00f);
	TOBJ::Move();
}

TBuilding_Far1::TBuilding_Far1( TOBJList* _owner, s32 _x, s32 _type ) : TOBJ(_owner)
{
	BGType = _type;
	X = _x;
	Y = 224;
	FKind = kBG; 
}

void TBuilding_Far1::Move()
{
	TOBJ::Move();
}

s32 TBuilding_Far1::OnDraw()
{
	UVRect uv;
	switch( BGType )
	{
		case 0:{
			uv.u1 = 0;
			uv.u2 = 239.f / 255.f;
			uv.v1 = 80.f / 255.f;
			uv.v2 = 111.f / 255.f;
		}
		break;
		case 1:{
			uv.u1 = 0;
			uv.u2 = 239.f / 255.f;
			uv.v1 = 112.f / 255.f;
			uv.v2 = 143.f / 255.f;
		}
		break;
		case 2:{
			uv.u1 = 0;
			uv.u2 = 239.f / 255.f;
			uv.v1 = 144.f / 255.f;
			uv.v2 = 175.f / 255.f;
		}
		break;
	}
	
	FOwner->GetScene()->SetSpriteSX( (f32)X - (f32)Age*0.1f, Y, 240, 32, 8.99f,uv,255,ST2Tex1,sxbAlpha);

	return 1;

}

TGlassCubeBase::TGlassCubeBase( TOBJList* _owner, TFrameManager* _parent, f32 _X, f32 _Y, f32 _Z, s32 _rx, s32 _ry, s32 _rz, f32 _zoom ) : TOBJ(_owner)
{
	FKind = kBG;
	Appeared = false;
	RX = random(16)-8;
	RY = random(16)-8;
	RZ = random(16)-8;

	MeshFrame = new TFrameManager(FOwner->GetScene(),_parent,GCube[0],NULL,sxbDefault ,true,true,true);

	MeshFrame->Move(_X,_Y,_Z);
	//MeshFrame->Move(2,0,6);
	MeshFrame->Rotate(_rx,_ry,_rz);
	MeshFrame->Zoom(_zoom,_zoom,_zoom);
	MeshFrame->SetColor(240,255,255,255);
	//FOwner->Add(TBuildingShadow.Create(FOwner,_Z,_rx,_ry,_rz,_zoom));
}

TGlassCubeBase::~TGlassCubeBase()
{
	MeshFrame->Frame->SetParent(NULL);    //親フレームとは独立して解放する（親が解放されるとき、子も含めて開放しようとするため）
	SAFE_DELETE( MeshFrame );
}

void TGlassCubeBase::Move()
{
	MeshFrame->RotateR(RX,RY,RZ);
	if( !Appeared && MeshFrame->Rendered() ) 
		Appeared  =  true;

	if(Appeared && !MeshFrame->Rendered() ) {
		//    MeshFrame->Move(random(1000)/100+6,random(1000)/100+6,random(100)/100+9);
		Appeared  =  false;
	}


}

void TGlassCubeMaker::Input()
{
	if((FOwner->Joy.StickY() == -1) ) {
		CenterFrame->MoveR(0,-0.1f,0);
	}

	if((FOwner->Joy.StickY() == 1) ) {
		CenterFrame->MoveR(0,0.1f,0);
	}

	if((FOwner->Joy.StickX() == -1) ) {
		CenterFrame->MoveR(0.1f,0,0);
	}

	if((FOwner->Joy.StickX() == 1) ) {
		CenterFrame->MoveR(-0.1f,0,0);
	}

	if(ButtonON[0] ) {
		CenterFrame->MoveR(0,0,0.1f);
	}
	if(ButtonON[1] ) {
		CenterFrame->MoveR(0,0,-0.1f);
	}
	if(ButtonON[2] ) {
		CenterFrame->RotateR(0,0,16);
		RZ += 16;
	}
	if(ButtonON[4] ) {
		CenterFrame->RotateR(0,0,-16);
		RZ -= 16;
	}

	if(ButtonON[5] ) {
		CenterFrame->RotateR(0,16,0);
		RY += 16;
	}

	if(ButtonON[6] ) {
		CenterFrame->RotateR(0,-16,0);
		RY -= 16;
	}

	if(ButtonON[7] ) {
		CenterFrame->RotateR(16,0,0);
		RX += 16;
	}

	if(ButtonON[8] ) {
		CenterFrame->RotateR(-16,0,0);
		RX -= 16;
	}
}

TGlassCubeMaker::TGlassCubeMaker( TOBJList* _owner, s32 _pat ) : TOBJ(_owner)
{
	f32 SX;
	f32 SY;
	f32 SZ;
	FKind = kGauge;
	Speed = 20;
	EndFlg = false;

	CenterFrame  = new TFrameManager(FOwner->GetScene(),NULL,NULL,NULL,sxbDefault ,true,true,true);
	MeshFrame = new TFrameManager(FOwner->GetScene(),CenterFrame,NULL,NULL,sxbDefault ,true,true,true);
	Cubes.clear();
	switch( _pat )
	{
		case 0:{
			for( s32 k = 0; k <= 2; k++ ) {
				for( s32 i = 0; i <= 29; i++ ) {
					SX = (f32)random(1000)/100.f +(f32)k*7.f;
					SY = sint[(i*140+70*k) & 0xfff]*(17+random(1000)/100);
					SZ = cost[(i*140+70*k) & 0xfff]*(17+random(1000)/100);
					RX = random(256)-128;
					RY = random(256)-128;
					RZ = 1024+random(256)-128;
					fZoom = (f32)random(4)/100.f+1.0f;
					TGlassCubeBase* newcube = new TGlassCubeBase(FOwner, MeshFrame,SX,SY,SZ,RX,RY,RZ,fZoom);
					Cubes.push_back( newcube );
					FOwner->Add( newcube );
				}
			}
			CenterFrame->Rotate(0,384,0);
			CenterFrame->Move(-20002.9f,10.2f,36.9f);
		}
		break;
		case 1:{
			for( s32 i = 0; i <= 59; i++ ) {
				SX = (f32)random(300)/100.f;
				SY = sint[(i*70) & 0xfff]*20;
				SZ = cost[(i*70) & 0xfff]*20;
				RX = random(256)-128;
				RY = random(256)-128;
				RZ = 1024+random(256)-128;
				fZoom = (f32)random(4)/100.f+0.5f;
				TGlassCubeBase* newcube = new TGlassCubeBase(FOwner, MeshFrame,SX,SY,SZ,RX,RY,RZ,fZoom);
				Cubes.push_back( newcube );
				FOwner->Add( newcube );
			}
			CenterFrame->Rotate(0,-80,-432);
			CenterFrame->Move(-19996.f,7.5f,22.9f);
		}
		break;
	}
}


TGlassCubeMaker::~TGlassCubeMaker()
{
	if(MeshFrame->Frame->GetChildren()->GetSize() > 0 ) {
		for( u32 i = 0; i < Cubes.size(); i++ ) {
			Cubes[i]->MeshFrame->Frame->SetParent(NULL);   //親フレームとは独立して解放する（親が解放されるとき、子も含めて開放しようとするため）
			Cubes[i]->Die();
		}
	}

	Cubes.clear();

	SAFE_DELETE( MeshFrame );
	SAFE_DELETE( CenterFrame );
}

void TGlassCubeMaker::Move()
{
	TOBJ::Move();
	MeshFrame->RotateR2(CenterFrame,Speed,0,0);

	if( EndFlg ) {
		if((Speed<30) && (Age % 3 == 0) ) Speed++;

		if(Speed == 30 ) {
			Die();
		}
	}
	else {
		if((Speed>1) && (Age % 3 == 0) ) Speed--;
	}
}

void TGlassCubeMaker::Die()
{
	TOBJ::Die();
}

void TGlassCubeMaker::EndProc()
{
	EndFlg = true;
}
