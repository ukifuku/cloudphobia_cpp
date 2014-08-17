#include "Demo.h"
#include "QDArc.h"
#include "TOBJ.h"
#include "TOBJList.h"
#include "Sound.h"
#include "PolyJoint.h"
#include "SpriteEX.h"
#include "SpriteEXM.h"
#include <fstream>

void TDemoManager::LoadTexture()
{
	string filename,qdaname;

	switch( DemoID )
	{
		case 0:{
			filename = "stage2demotex.txt";
			qdaname  =  "./Data/stage2.bin";
		}break;
		case 1:{
			filename = "stage5demotex.txt";
			qdaname  =  "./Data/stage5.bin";
		}
	}

	SAFE_DELETE_VECTOR(Textures);

	if( DebugMode ) {
		ifstream ifs( DEBUG_GRAPH_PATH + filename );
		string readline;
		while( getline(ifs, readline) )  {
			TDGTexture* tex = new TDGTexture(FOwner->GetDG(), DGFMT_ARGB,0,D3DPOOL_MANAGED);
			tex->LoadFromFile(DEBUG_GRAPH_PATH + readline);
			Textures.push_back(tex);
		}
		
		ifs.close();
	}
	else {
		stringstream tempstring;
		ExtractFromQDAFile(tempstring, qdaname, filename);

		string readline;
		while( getline(tempstring, readline) )  {
			TDGTexture* tex = new TDGTexture(FOwner->GetDG(), DGFMT_ARGB,0,D3DPOOL_MANAGED);
			tex->LoadFromFile(readline, qdaname);
			Textures.push_back(tex);
		}
	}
}

void TDemoManager::Input()
{
	if( ButtonON[0] ) {
		FOwner->SoundEffect->AllStop();
		Die();
	}
}

TDemoManager::TDemoManager( TOBJList* owner, s32 _id ) : TOBJ(owner)
{
	DemoID = _id;
	FKind  =  kEffect2;

	switch( DemoID )
	{
		case 0:{
			ScriptID = KSCRIPT_STAGE2DEMO;
			   }break;
		case 1:{
			ScriptID = KSCRIPT_STAGE5DEMO;
			   }break;
	}

	LoadTexture();

	MakeSpriteLite(FOwner->GetDG(), numSpr,0,0,640,60,10,bmNormal);
	Sprite[numSpr-1]->GetSpr()->AddTexture(Eff1Tex, 17, 1, 31, 15);
	Sprite[numSpr-1]->GetSpr()->SetColor(0, ctRed);
	Sprite[numSpr-1]->GetSpr()->SetColor(0, ctBlue);
	Sprite[numSpr-1]->GetSpr()->SetColor(0, ctGreen);

	MakeSpriteLite(FOwner->GetDG(), numSpr,0,420,640,60,10,bmNormal);
	Sprite[numSpr-1]->GetSpr()->AddTexture(Eff1Tex, 17, 1, 31, 15);
	Sprite[numSpr-1]->GetSpr()->SetColor(0, ctRed);
	Sprite[numSpr-1]->GetSpr()->SetColor(0, ctBlue);
	Sprite[numSpr-1]->GetSpr()->SetColor(0, ctGreen);
}

TDemoManager::~TDemoManager()
{
	SAFE_DELETE_VECTOR(SpriteList);
	SAFE_DELETE_VECTOR(Textures);
}

void TDemoManager::Die()
{
	TOBJ::Die();
	switch(DemoID){
		case 0:{
			FOwner->Add(new TStage2StartingDemo(FOwner));
		}break;
		case 1:{
			FOwner->AfterLoopEvent = MTitle;
		}break;
	}
}

void TDemoManager::Move()
{
	TOBJ::Move();

	if( (Age == 1) && (FOwner->JikiObj != NULL) ) {
		FOwner->JikiObj->Sprite[1].Move(-230, 240);
	}

	if( Start ) {
		Script->ExecuteProc(ScriptID, this);
		Input();
	}
}

void TDemoManager::MakeSprite( s32 _num, s32 _tex, s32 _x, s32 _y, s32 _z, s32 _w, s32 _h, s32 _tx, s32 _ty, s32 _tw, s32 _th, s32 _cx, s32 _cy, s32 _blend )
{

	TBlendMode Blend;
	switch( _blend ){
		case 0:Blend = bmNormal;	break;
		default:
			Blend = bmAdd;
	}

	TSpriteEXBLT sblt;
	sblt.RBM      =  Blend;    // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  QD;        // TDDDDの参照
	sblt.RWidth   =  _w;        // 幅
	sblt.RHeight  =  _h;        // 高さ
	sblt.RX       =  0;         // X座標
	sblt.RY       =  0;         // Y座標
	sblt.RZ       =  _z ;        // Z値

	TSpriteEX* newSpr = new TSpriteEX(sblt);

	newSpr->AddTexture(Textures[_tex], _tx, _ty, _tx+_tw, _ty+_th);
	newSpr->FCX += _cx;
	newSpr->FCY += _cy;
	newSpr->Move(_x,_y);
	newSpr->FVisible = true;
	SpriteList.push_back(newSpr);
}

void TDemoManager::SprMove( s32 _id, f32 _x, f32 _y )
{
	SpriteList[_id]->Move(_x,_y);
}

void TDemoManager::SprMoveR( s32 _id, f32 _x, f32 _y )
{
	SpriteList[_id]->MoveR(_x,_y);
}

void TDemoManager::SprSlideMoveR( s32 _id, f32 _x, f32 _y )
{
	SpriteList[_id]->SlideMoveR(_x,_y);
}

void TDemoManager::SprRotate( s32 _id, s32 _r )
{
	SpriteList[_id]->Rotate(_r);
}

void TDemoManager::SprRotateR( s32 _id, s32 _r )
{
	SpriteList[_id]->RotateR(_r);
}

void TDemoManager::SprZoom( s32 _id, f32 _x, f32 _y )
{
	SpriteList[_id]->Zoom(_x,_y);
}

void TDemoManager::SprZoomR( s32 _id, f32 _x, f32 _y )
{
	SpriteList[_id]->ZoomR(_x,_y);
}

void TDemoManager::SprColor( s32 _id, s32 _col )
{
	SpriteList[_id]->SetColor(_col,ctRed);
	SpriteList[_id]->SetColor(_col,ctGreen);
	SpriteList[_id]->SetColor(_col,ctBlue);
}

void TDemoManager::SprColorR( s32 _id, s32 _col )
{
	SpriteList[_id]->SetColorR(_col,ctRed);
	SpriteList[_id]->SetColorR(_col,ctGreen);
	SpriteList[_id]->SetColorR(_col,ctBlue);
}

void TDemoManager::SprAlpha( s32 _id, s32 _col )
{
	SpriteList[_id]->SetColor(_col,ctAlpha);
}

void TDemoManager::SprAlphaR( s32 _id, s32 _col )
{
	SpriteList[_id]->SetColorR(_col,ctAlpha);
}

void TDemoManager::DemoStart()
{
	Start = true;
	FOwner->DemoON = true;
}

void TDemoManager::SprRelease( s32 _id )
{
	SAFE_DELETE(SpriteList[_id]);
}

s32 TDemoManager::OnDraw()
{
	for( u32 i = 0 ; i < SpriteList.size(); i++ ) {
		if( SpriteList[i] != NULL ) FOwner->GetSprMan()->AddSprite(SpriteList[i]);
	}

	for( s32 i = 0 ; i < numSpr; i++ ){
		if( Sprite[i] != NULL ) {
			FOwner->GetSprMan()->AddSprite(Sprite[i]->GetSpr());
		}
	}

	return SpriteList.size();
}


void TMovieManager::Input()
{
	if( ButtonON[0] ) {
		FOwner->SoundEffect->AllStop();
		Die();
	}
}

TMovieManager::TMovieManager( TOBJList* owner, s32 _id ) : TOBJ(owner)
{
	MovieID = _id;
	FKind  =  kGauge;
	StopRequested = false;

	//黒背景
	MakeSpriteLite(FOwner->GetDG(), numSpr,0,0,640,480,0,bmNormal);
	Sprite[numSpr-1]->GetSpr()->AddTexture(Eff1Tex, 17, 1, 31, 15);
	Sprite[numSpr-1]->GetSpr()->SetColor(0, ctRed);
	Sprite[numSpr-1]->GetSpr()->SetColor(0, ctBlue);
	Sprite[numSpr-1]->GetSpr()->SetColor(0, ctGreen);
}

TMovieManager::~TMovieManager()
{
}

void TMovieManager::Move()
{
	TOBJ::Move();

	FOwner->DontRender  =  true;

	if( Age == 2 ) {
		try{
			Form1.OpenMovie(MovieID);		// ★★★
			Form1.PlayMovie(MovieID);
			Form1.MoviePlaying = true;
		}
		catch(int e){
			//ムービーを再生せずタイトル画面へ
			FOwner->AfterLoopEvent = MTitle;
			Form1.MoviePlaying  =  false;
			Die();
		}
	}


	if( Age > 2 ) {
		if( ! StopRequested ) Input();

		if( ! Form1.MoviePlaying ) {		// ★★★
			FOwner->AfterLoopEvent = MTitle;
			Die();
		}
	}
}

TEndingStarter::TEndingStarter( TOBJList* owner ) : TOBJ(owner)
{
	FKind  =  kEffect2;
	FOwner->GameMode = gOther;

	FOwner->Add(new TBoxContrl(FOwner, 9));

	if( CreateBGM ) {
		FOwner->MusicPlayer->FadeOut(1,0.0,0.5);
		FOwner->MusicPlayer->FadeOut(2,0.0,0.5);
	}
}

void TEndingStarter::Move()
{
	TOBJ::Move();
	if( Age > 240 ) {
		FOwner->AfterLoopEvent = MEndingMovie;
		//    FOwner.AfterLoopEvent = MTitle;
		Die();
	}
}
