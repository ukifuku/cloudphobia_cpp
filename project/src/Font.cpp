#include "Font.h"
#include "TOBJList.h"
#include "SpriteEXM.h"
#include "Effect.h"


TMoji::TMoji( TOBJList* owner, s32 moji, s32 fx, s32 fy, s32 _cx, s32 _cy, s32 _pat ) : TOBJ(owner)
{
	FObjectName = "TMoji";
	X = fx;
	Y = fy;
	pat = _pat;

	TPoint FPoint[48];
	for( u32 i = 0; i <= 9; i++ ) {
		FPoint[i].X = i*16;
		FPoint[i].Y = 0;
	}

	for( u32 i = 0; i <= 1; i++ ){
		for( u32 k = 0; k <= 12; k++ ) {
			FPoint[10+k+(i*13)].X = k*16;
			FPoint[10+k+(i*13)].Y = 32+32*i;
		}
	}

	for( u32 i = 0; i <= 11; i++ ) {
		FPoint[36+i].X = i*16;
		FPoint[36+i].Y = 96;
	}

	TSpriteEXBLT sblt;
	sblt.RBM      =  bmNormal; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  FOwner->GetDG();     // TDDDDの参照
	sblt.RHeight  =  32;        // 高さ
	if( moji <= 47 )
		sblt.RWidth   =  16;        // 幅
	else
		sblt.RWidth   =  32;        // 幅
	sblt.RX       =  X;        // X座標
	sblt.RY       =  Y;        // Y座標

	switch( pat ){
	case 0:{
		FKind = kGauge;
		sblt.RZ = 100;
		   }break;
	case 1:{
		FKind = kEffect2;        //エフェクト扱い
		sblt.RZ = 500;
		   }break;
	case 2:
	case 8:{
		FKind = kEffect2;        //エフェクト扱い
		sblt.RZ = 500;
		   }break;
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:{
		FKind = kGauge;
		sblt.RZ = 100;
		   }break;
	case 9:{
		FKind = kGauge;
		sblt.RZ = 180;
		   }break;
	case 10:{
		FKind = kEffect2;
		sblt.RZ = 510;
			}break;
	}

	Sprite[0] = new TJoint(sblt);
	Inc(numSpr,1);
	if( moji <= 47 )
		Sprite[0]->GetSpr()->AddTexture(FontTex, FPoint[moji].X+1, FPoint[moji].Y, FPoint[moji].X+16, FPoint[moji].Y+32);	// テクスチャを登録
	else {
		Sprite[0]->GetSpr()->AddTexture(GaugeTex, 112+32*(moji-48), 305, 143+32*(moji-48), 336);
		Sprite[0]->GetSpr()->FCX += 8;
	}

	Sprite[0]->GetSpr()->FCX += _cx;
	Sprite[0]->GetSpr()->FCY += _cy;
	Sprite[0]->GetSpr()->FVisible  =  true;
	Sprite[0]->GetSpr()->Move(X,Y);
	if( (ScreenResolution < 2) ) Sprite[0]->GetSpr()->FSmooth = false;
	else Sprite[0]->GetSpr()->FSmooth = true;

	switch( pat ){
	case 4:{
		Sprite[0]->GetSpr()->SetColor(0, ctBlue);
		Sprite[0]->GetSpr()->SetColor(0, ctGreen);
		   }break;
	case 5:{
		Sprite[0]->GetSpr()->Zoom(0.7f,0.7f);
		Sprite[0]->GetSpr()->FSmooth = true;
		   }break;
	case 6:
	case 8:{
		Sprite[0]->GetSpr()->Zoom(2.f, 2.f);
		   }break;
	case 7:{
		Sprite[0]->GetSpr()->Zoom(2.f, 2.f);
		Sprite[0]->GetSpr()->SetColor(0, ctBlue);
		Sprite[0]->GetSpr()->SetColor(0, ctGreen);
		   }break;
	}
}

void TMoji::Move()
{
	switch( pat ){
	case 0:
		Die();
		break;
	case 1:{
		Sprite[0]->MoveR(0.f, -0.5f);
		if( (Age >= 50) ) Die();
		   }break;
	case 2:{
		Sprite[0]->GetSpr()->SetColor(0, ctBlue);
		Sprite[0]->GetSpr()->SetColor(0, ctGreen);
		Sprite[0]->MoveR(0.f, -0.5f);
		if( Age % 4 == 0 ) {
			Sprite[0]->GetSpr()->SetColor(255, ctBlue);
			Sprite[0]->GetSpr()->SetColor(255, ctGreen);
		}

		//Sprite[0]->GetSpr()->SetColor(255-Age*5, ctAlpha);
		if( (Age >= 50) ) Die();
		   }break;
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
		Die();
		break;
	case 8:{
		Sprite[0]->GetSpr()->SetColor(0, ctBlue);
		Sprite[0]->GetSpr()->SetColor(0, ctGreen);
		Sprite[0]->MoveR(0.f, -0.5f);
		if( Age % 4 == 0 ) {
			Sprite[0]->GetSpr()->SetColor(255, ctBlue);
			Sprite[0]->GetSpr()->SetColor(255, ctGreen);
		}

		//Sprite[0]->GetSpr()->SetColor(255-Age*5, ctAlpha);
		if( (Age >= 50) ) Die();
		   }break;
	case 10:{
		Sprite[0]->GetSpr()->SetColor(0, ctBlue);
		Sprite[0]->GetSpr()->SetColor(0, ctGreen);
		if( Age % 4 == 0 ) {
			Sprite[0]->GetSpr()->SetColor(255, ctBlue);
			Sprite[0]->GetSpr()->SetColor(255, ctGreen);
		}
			}break;
	}
	TOBJ::Move();

}

TMoji_blight::TMoji_blight( TOBJList* owner, s32 moji, s32 fx, s32 fy, bool _blink ) : TOBJ(owner)
{
	FObjectName = "TMoji_blight";
	X = fx;
	Y = fy;
	Blink = _blink;
	FKind = kGauge;

	s32 n  =  0;
	TPoint FPoint[48];
	for( u32 i = 0; i <= 9; i++ ) {
		FPoint[n].X = i*32;
		FPoint[n].Y = 176;
		Inc(n);
	}

	for( u32 i = 0; i <= 1; i++ ){
		for( u32 k = 0; k <= 12; k++ ) {
			FPoint[n].X = k*32;
			FPoint[n].Y = 208+32*i;
			Inc(n);
		}
	}

	for( u32 i = 0; i <= 11; i++ ) {
		FPoint[n].X = i*32;
		FPoint[n].Y = 272;
		Inc(n);
	}

	TSpriteEXBLT sblt;
	sblt.RBM      =  bmNormal; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  FOwner->GetDG();     // TDDDDの参照
	sblt.RHeight  =  32;        // 高さ
	sblt.RWidth   =  32;        // 幅
	sblt.RX       =  X;        // X座標
	sblt.RY       =  Y;        // Y座標
	sblt.RZ       =  100;

	Sprite[0] = new TJoint(sblt);
	Inc(numSpr,1);
	Sprite[0]->GetSpr()->AddTexture(FontTex_Gothic, FPoint[moji].X, FPoint[moji].Y, FPoint[moji].X+32, FPoint[moji].Y+32);// テクスチャを登録
	Sprite[0]->GetSpr()->FVisible  =  true;
	Sprite[0]->GetSpr()->Move(X,Y);
	Sprite[0]->GetSpr()->FSmooth = true;
}

void TMoji_blight::Move()
{
	TOBJ::Move();
	if( Blink && (FOwner->GameMode == gPlay) ) {
		Sprite[0]->GetSpr()->SetColor(0, ctBlue);
		Sprite[0]->GetSpr()->SetColor(0, ctGreen);
		if( Age % 3 == 0 ) {
			Sprite[0]->GetSpr()->SetColor(255, ctBlue);
			Sprite[0]->GetSpr()->SetColor(255, ctGreen);
		}
	}
}

TMoji_gothic::TMoji_gothic( TOBJList* owner, s32 moji, s32 fx, s32 fy, s32 z ) : TOBJ(owner)
{
	FObjectName = "TMoji_gothic";
	X = fx;
	Y = fy;

	FKind = kGauge;
	s32 n  =  0;

	if( moji > 9 ) {
		MakeUnit(moji);
	}
	else {
		TPoint FPoint[10];
		for( u32 i = 0; i <= 1; i++ ){
			for( u32 k = 0; k <= 4; k++ ) {
				FPoint[n].X = k*80;
				FPoint[n].Y = 64*i;
				Inc(n);
			}
		}

		TSpriteEXBLT sblt;
		sblt.RBM      =  bmNormal; // αブレンドモード
		sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
		sblt.RDDDD    =  FOwner->GetDG();     // TDDDDの参照
		sblt.RHeight  =  64;        // 高さ
		sblt.RWidth   =  80;        // 幅
		sblt.RX       =  X;        // X座標
		sblt.RY       =  Y;        // Y座標
		sblt.RZ       =  z;

		Sprite[0] = new TJoint(sblt);
		Inc(numSpr,1);
		Sprite[0]->GetSpr()->AddTexture(FontTex_Gothic, FPoint[moji].X, FPoint[moji].Y, FPoint[moji].X+80, FPoint[moji].Y+64);// テクスチャを登録
		Sprite[0]->GetSpr()->FVisible  =  true;
		Sprite[0]->GetSpr()->Move(X,Y);
		Sprite[0]->GetSpr()->FSmooth = true;
	}

}

void TMoji_gothic::Move()
{
	TOBJ::Move();
}

void TMoji_gothic::MakeUnit( s32 _moji )
{
	TPoint FPoint;
	if( FONTSET[_moji] == 's' ) {
		FPoint.X = 0;
		FPoint.Y = 128;
	}
	else if( FONTSET[_moji] == 'n' ) {
		FPoint.X = 96;
		FPoint.Y = 128;
	}
	else if( FONTSET[_moji] == 'r' ) {
		FPoint.X = 192;
		FPoint.Y = 128;
	}
	else {
		FPoint.X = 288;
		FPoint.Y = 128;
	}

	TSpriteEXBLT sblt;
	sblt.RBM      =  bmNormal; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  FOwner->GetDG();     // TDDDDの参照
	sblt.RHeight  =  48;        // 高さ
	sblt.RWidth   =  96;        // 幅
	sblt.RX       =  X;        // X座標
	sblt.RY       =  Y;        // Y座標
	sblt.RZ       =  141;

	Sprite[0] = new TJoint(sblt);
	Inc(numSpr,1);
	Sprite[0]->GetSpr()->AddTexture(FontTex_Gothic, FPoint.X, FPoint.Y, FPoint.X+96, FPoint.Y+48);// テクスチャを登録
	Sprite[0]->GetSpr()->FVisible  =  true;
	Sprite[0]->GetSpr()->Move(X+8,Y+16);
	Sprite[0]->GetSpr()->FSmooth = true;
}

TMoji_counter::TMoji_counter( TOBJList* owner, s32 fx, s32 fy, s32 z ) : TOBJ(owner)
{
	FObjectName = "TMoji_counter";
	X = fx;
	Y = fy;
	FKind = kGauge;

	TPoint FPoint[12];
	for( u32 i = 0; i <= 11; i++ ) {
		FPoint[i].X = i*32;
		FPoint[i].Y = 304;
	}

	TSpriteEXBLT sblt;
	sblt.RBM      =  bmAdd; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  FOwner->GetDG();     // TDDDDの参照
	sblt.RHeight  =  32;        // 高さ
	sblt.RWidth   =  32;        // 幅
	sblt.RX       =  X;        // X座標
	sblt.RY       =  Y;        // Y座標
	sblt.RZ       =  z;

	Sprite[0] = new TJoint(sblt);
	Inc(numSpr,1);
	for( u32 i = 0; i <= 11; i++ )
		Sprite[0]->GetSpr()->AddTexture(FontTex_Gothic, FPoint[i].X, FPoint[i].Y, FPoint[i].X+31, FPoint[i].Y+31);// テクスチャを登録
	Sprite[0]->GetSpr()->FAnim = true;
	Sprite[0]->GetSpr()->FAnimBgnIdx = 0;
	Sprite[0]->GetSpr()->FAnimEndIdx = 11;
	Sprite[0]->GetSpr()->FVisible  =  true;
	Sprite[0]->GetSpr()->Move(X,Y);
	Sprite[0]->GetSpr()->FSmooth = false;
}

void TMoji_counter::Move()
{
	TOBJ::Move();

}

void TMoji_counter::Update( s32 _val )
{
	Sprite[0]->GetSpr()->FAnimIdx = _val;

}

TMoji_orange::TMoji_orange( TOBJList* owner, s32 moji, s32 fx, s32 fy, s32 z ) : TOBJ(owner)
{
	FObjectName = "TMoji_orange";
	X = fx;
	Y = fy;

	FKind = kGauge;

	s32 n = 0;

	TPoint FPoint[88];
	for( u32 i = 0; i <= 9; i++ ) {
		FPoint[n].X = i*32;
		FPoint[n].Y = 304;
		Inc(n);
	}

	for( u32 i = 0; i <= 3; i++ ){
		for( u32 k = 0; k <= 15; k++ ) {
			FPoint[n].X = k*32;
			FPoint[n].Y = 336+32*i;
			Inc(n);
		}
	}

	for( u32 i = 0; i <= 12; i++ ) {
		FPoint[n].X = i*32;
		FPoint[n].Y = 464;
		Inc(n);
	}

	MakeSpriteLite(0, X, Y,32,32,z,bmAdd);
	MakeSpriteLite(1, X, Y,32,32,z+1,bmNormal);

	Sprite[0]->GetSpr()->AddTexture(FontTex_Gothic, FPoint[moji].X, FPoint[moji].Y, FPoint[moji].X+31, FPoint[moji].Y+31);// テクスチャを登録
	Sprite[0]->GetSpr()->FVisible  =  true;
	Sprite[0]->GetSpr()->Move(X,Y);
	Sprite[0]->GetSpr()->FSmooth = true;

	Sprite[1]->GetSpr()->AddTexture(FontTex_Gothic, FPoint[moji].X, FPoint[moji].Y, FPoint[moji].X+31, FPoint[moji].Y+31);// テクスチャを登録
	Sprite[1]->GetSpr()->FVisible  =  true;
	Sprite[1]->GetSpr()->Move(X,Y);
	Sprite[1]->GetSpr()->FSmooth = true;
	Sprite[1]->GetSpr()->SetColor(96,ctRed);
	Sprite[1]->GetSpr()->SetColor(96,ctGreen);
	Sprite[1]->GetSpr()->SetColor(96,ctBlue);
	Sprite[1]->GetSpr()->SetColor(160,ctAlpha);

	Sprite[0]->Add(Sprite[1]);
}

void TMoji_orange::Move()
{
	TOBJ::Move();

}

TMoveSentence::TMoveSentence( TOBJList* owner, const string& moji, s32 sx, s32 sy, s32 dx, s32 dy, s32 _life, s32 _pat ) : TOBJ(owner)
{
	FObjectName = "TMoveSentence";
	ActFlg = 0;

	pattern = _pat;
	X = sx;
	Y = sy;
	DestX = dx;
	DestY = dy;
	letter = moji;
	life = _life;

	IniDis = RoundOff(sqrt((DestX-X)*(DestX-X)+(DestY-Y)*(DestY-Y)));

	RDeg = RoundOff(atan2(DestY-Y, DestX-X)/PI*2048.f) & 0xfff;

}

void TMoveSentence::Move()
{
	FOwner->SentenceRight(letter,X,Y,0,3);
	FOwner->Add(new TBox(FOwner,X-16-letter.size()*16,Y+12,X+16,Y+20,0,200,255));

	f32 TempDis = RoundOff(sqrt((DestX-X)*(DestX-X)+(DestY-Y)*(DestY-Y)));

	if( TempDis>8 ) {
		X = X+RoundOff(TempDis*cost[RDeg]/8);
		Y = Y+RoundOff(TempDis*sint[RDeg]/8);
	}
	else {
		switch( ActFlg ){
		case 0:Dec(life);	break;
		case 1:Die();		break;
		}
	}

	if( (life<=0) && (ActFlg == 0) ) {
		DestX = X-RoundOff(IniDis*cost[RDeg]);
		DestY = Y-RoundOff(IniDis*sint[RDeg]);
		RDeg = RoundOff(atan2(DestY-Y,DestX-X)/PI*2048.f) & 0xfff;
		ActFlg = 1;
	}

}

TSprSentence::TSprSentence( TOBJList* owner, const string& moji, s32 sx, s32 sy, s32 brank, s32 font ) : TOBJ(owner)
{
	FObjectName = "TSprSentence";
	X = sx;
	Y = sy;
	FKind = kGauge;
	FontKind = font;

	TSpriteEXBLT sblt;
	sblt.RBM      =  bmNormal; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  FOwner->GetDG();     // TDDDDの参照
	sblt.RHeight  =  32;        // 高さ
	sblt.RWidth   =  16;        // 幅
	sblt.RX       =  0;        // X座標
	sblt.RY       =  0;        // Y座標
	sblt.RZ       =  10000;

	Sprite[0] = new TJoint(sblt);
	Inc(numSpr,1);
	Sprite[0]->GetSpr()->AddTexture(FontTex, 0, 0, 1, 1);  //非表示だが一応テクスチャ登録（登録してないと落ちるため）
	Sprite[0]->GetSpr()->FVisible  =  false;

	s32 Fnum = 0;
	for( u32 i = 1 ; i < moji.size(); i++ ) {
		if( font = KFONTKIND_ORANGE ) {
			for( u32 k = 0 ; k < sizeof(FONTSET_ORANGE); k++ )
				if( moji[i] == FONTSET_ORANGE[k] ) { Fnum = k; break; }
		}
		else {
			for( u32 k = 0 ; k < sizeof(FONTSET); k++ )
				if( moji[i] == FONTSET[k] ) { Fnum = k; break; }
		}

		TOBJ* TmpObj;
		s32 fx;
		switch( font ){
		case KFONTKIND_NORMAL : 
			{
				fx  =  (i-1)*16+brank*(i-1);
				TmpObj  =  new TMoji(owner,Fnum,fx,0,0,0,9);
			}break;
		case KFONTKIND_BLIGHT : 
			{
				fx  =  (i-1)*18+brank*(i-1);
				TmpObj  =  new TMoji_blight(owner,Fnum,fx,0,false);
			}break;
		case KFONTKIND_BLINK : 
			{
				fx  =  (i-1)*18+brank*(i-1);
				TmpObj  =  new TMoji_blight(owner,Fnum,fx,0,true);
			}break;
		case KFONTKIND_ORANGE : 
			{
				fx  =  (i-1)*18+brank*(i-1);
				TmpObj  =  new TMoji_orange(owner,Fnum,fx,0,141);
			}break;
		case KFONTKIND_BLINK2 : 
			{
				fx  =  (i-1)*16+brank*(i-1);
				TmpObj  =  new TMoji(owner,Fnum,fx,0,0,0,10);
			}break;
		default:
			{
				fx  =  (i-1)*48+brank*(i-1);
				TmpObj  =  new TMoji_gothic(owner,Fnum,fx,0,140+i);
			}
		}

		MojiList.push_back(TmpObj);
		FOwner->Add(TmpObj);
		Sprite[0]->Add(TmpObj->Sprite[0]);
	}

	Sprite[0]->Move(X,Y);
}

TSprSentence::~TSprSentence()
{
	MojiList.clear();
}

void TSprSentence::Move()
{
	TOBJ::Move();
}

void TSprSentence::Die()
{
	if( ! MojiList.empty() ) {
		for( u32 i = 0 ; i < MojiList.size(); i++ ) {
			MojiList[i]->Die();
		}
	}
	TOBJ::Die();

}

void TSprSentence::SetAlpha( s32 _alpha )
{
	for( u32 i = 0 ; i < MojiList.size(); i++ ) {
		MojiList[i]->Sprite[0]->GetSpr()->SetColor(_alpha,ctAlpha);
	}
}

void TSprSentence::SetColor( s32 _red, s32 _green,s32 _blue )
{
	TOBJ* TmpObj;
	for( u32 i = 0 ; i < MojiList.size(); i++ ) {
		TmpObj  =  MojiList[i];
		TmpObj->Sprite[0]->GetSpr()->SetColor(_red,ctRed);
		TmpObj->Sprite[0]->GetSpr()->SetColor(_green,ctGreen);
		TmpObj->Sprite[0]->GetSpr()->SetColor(_blue,ctBlue);
	}
}

void TSprSentence::SetZ( u16 _rz )
{
	for( u32 i = 0 ; i < MojiList.size(); i++ ) {
		MojiList[i]->Sprite[0]->GetSpr()->FZ = _rz;
	}
}

void TMojiAndGlassCursor::Slide()
{
	s32 distance  =  DestX - NowX;
	if( distance != 0 ) {
		//移動スピードを決定
		XSpeed  =  distance / XSpeedFactor;
		if( (XSpeed == 0) ) XSpeed  =  distance / abs(distance);
		Inc(NowX, XSpeed);
		Sprite[0]->MoveR(XSpeed,0);
	}

	distance  =  DestY - NowY;
	if( distance != 0 ) {
		YSpeed  =  distance / YSpeedFactor;
		if( (YSpeed == 0) ) YSpeed  =  distance / abs(distance);
		Inc(NowY, YSpeed);
		Sprite[0]->MoveR(0,YSpeed);
	}
}

void TMojiAndGlassCursor::ModulateAlpha()
{
	s32 tempAlpha  =  CurrentAlpha + AlphaSpeed;

	SetAlpha(tempAlpha);

	if( (abs(DestAlpha - tempAlpha) < abs(AlphaSpeed)) )
		AlphaSpeed  =  DestAlpha - tempAlpha;


}

TMojiAndGlassCursor::TMojiAndGlassCursor( TOBJList* owner, s32 _x, s32 _y, s32 _long, s32 _pat ) : TOBJ(owner)
{
	const s32 BLIGHT_YCORRECT = -7;
	FObjectName = "TMojiAndGlassCursor";

	XSpeed  =  0;   //カーソル移動スピード
	YSpeed  =  0;
	DestX  =  _x;
	DestY  =  _y;  //目的地座標
	NowX   =  _x;
	NowY   =  _y;
	X = _x;
	Y = _y;
	FKind = kGauge;
	Pattern  =  _pat;
	XSpeedFactor  =  3;
	YSpeedFactor  =  3;
	CurrentAlpha  =  255;
	DestAlpha  =  255;
	AlphaSpeed  =  0;
	AlphaDelayTime  =  0;
	Blink  =  false;
	SlideDelayCount  =  0;
	AlphaDelayCount  =  0;

	DeathPoint  =  TPoint(0,0);

	TSpriteEXBLT sblt;
	sblt.RBM      =  bmNormal; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  FOwner->GetDG();     // TDDDDの参照
	sblt.RHeight  =  32;        // 高さ
	sblt.RWidth   =  16;        // 幅
	sblt.RX       =  _x;        // X座標
	sblt.RY       =  _y;        // Y座標
	sblt.RZ       =  200;       // Z値

	//左端、真ん中、右端の部分にわけて作成
	for( u32 i = 0; i <= 2; i++ ) {
		Sprite[i] =  new TJoint(sblt);         // スプライトの生成
		Inc(numSpr,1);
		Sprite[i]->GetSpr()->SetColor(GLASSCURSOR_BLIND_ALPHA, ctAlpha);
		Sprite[i]->GetSpr()->SetColor(0, ctRed);
		Sprite[i]->GetSpr()->SetColor(0, ctGreen);
		Sprite[i]->GetSpr()->SetColor(0, ctBlue);
		Sprite[i]->GetSpr()->FSmooth = false;
		if( ! (Pattern == GLASSCURSOR_PATTERN_BLIND) )
			Sprite[i]->GetSpr()->FVisible = false;
	}

	Sprite[0]->GetSpr()->AddTexture(Eff8Tex, 0, 48, 16, 80);
	Sprite[1]->GetSpr()->AddTexture(Eff8Tex, 16, 48, 32, 80);
	Sprite[2]->GetSpr()->AddTexture(Eff8Tex, 32, 48, 48, 80);

	//指定された長さに合わせて真ん中のパーツを拡大
	Sprite[1]->GetSpr()->Zoom(_long/16.f, 1.0f);
	Sprite[1]->GetSpr()->MoveR(16,0);

	//右端パーツの位置を合わせる
	Sprite[2]->GetSpr()->MoveR(16+_long,0);

	Sprite[0]->Add(Sprite[1]);
	Sprite[0]->Add(Sprite[2]);

	if( Pattern == GLASSCURSOR_PATTERN_LINE ) {

		//区切り線
		sblt.RBM      =  bmNormal; // αブレンドモード
		sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
		sblt.RDDDD    =  FOwner->GetDG();     // TDDDDの参照
		sblt.RHeight  =  16;        // 高さ
		sblt.RWidth   =  16;        // 幅
		sblt.RX       =  _x;        // X座標
		sblt.RY       =  _y;        // Y座標
		sblt.RZ       =  200;       // Z値

		Sprite[3] =  new TJoint(sblt);         // スプライトの生成
		Inc(numSpr,1);
		Sprite[3]->GetSpr()->FSmooth = false;

		Sprite[3]->GetSpr()->AddTexture(Eff8Tex, 1, 97, 16, 112);

		//指定された長さに合わせてパーツを拡大
		Sprite[3]->GetSpr()->ZoomR(_long/16.f, 1.0f);

		Sprite[0]->Add(Sprite[3]);
	}

	if( (Pattern == GLASSCURSOR_PATTERN_GLASS) || (Pattern == GLASSCURSOR_PATTERN_BLIGHT)  ) {

		//ガラス部分
		sblt.RBM      =  bmNormal; // αブレンドモード
		sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
		sblt.RDDDD    =  FOwner->GetDG();     // TDDDDの参照
		sblt.RHeight  =  48;        // 高さ
		sblt.RWidth   =  16;        // 幅
		sblt.RX       =  _x;        // X座標
		sblt.RY       =  _y;        // Y座標
		sblt.RZ       =  190;       // Z値
		//左端、真ん中、右端の部分にわけて作成
		for( u32 i = 3; i <= 5; i++ ) {
			Sprite[i] =  new TJoint(sblt);         // スプライトの生成
			Inc(numSpr,1);
			Sprite[i]->GetSpr()->SetColor(GLASSCURSOR_GLASS_ALPHA, ctAlpha);
			Sprite[i]->GetSpr()->FSmooth = false;
		}

		Sprite[3]->GetSpr()->AddTexture(Eff8Tex, 0, 1, 16, 48);
		Sprite[4]->GetSpr()->AddTexture(Eff8Tex, 16, 1, 32, 48);
		Sprite[5]->GetSpr()->AddTexture(Eff8Tex, 32, 1, 48, 48);

		//指定された長さに合わせて真ん中のパーツを拡大
		Sprite[4]->GetSpr()->ZoomR(_long/16.f, 1.0f);
		Sprite[4]->GetSpr()->MoveR(16,0);

		//右端パーツの位置を合わせる
		Sprite[5]->GetSpr()->MoveR(16+_long,0);

		//光沢部分
		sblt.RBM      =  bmNormal; // αブレンドモード
		sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
		sblt.RDDDD    =  FOwner->GetDG();     // TDDDDの参照
		sblt.RHeight  =  16;        // 高さ
		sblt.RWidth   =  16;        // 幅
		sblt.RX       =  _x;        // X座標
		sblt.RY       =  _y;        // Y座標
		sblt.RZ       =  170;       // Z値

		//左端、真ん中、右端の部分にわけて作成
		for( u32 i = 6; i <= 8; i++ ) {
			Sprite[i] =  new TJoint(sblt);         // スプライトの生成
			Inc(numSpr,1);
			Sprite[i]->GetSpr()->SetColor(GLASSCURSOR_REFLEX_ALPHA, ctAlpha);
			Sprite[i]->GetSpr()->FSmooth = true;
		}

		Sprite[6]->GetSpr()->AddTexture(Eff8Tex, 0, 81, 16, 96);
		Sprite[7]->GetSpr()->AddTexture(Eff8Tex, 16, 81, 32, 96);
		Sprite[8]->GetSpr()->AddTexture(Eff8Tex, 32, 81, 48, 96);

		//指定された長さに合わせて真ん中のパーツを拡大
		Sprite[7]->GetSpr()->ZoomR(_long/16.f, 1.0f);
		Sprite[7]->GetSpr()->MoveR(16,0);

		//右端パーツの位置を合わせる
		Sprite[8]->GetSpr()->MoveR(16+_long,0);

		for( u32 i = 3; i <= 8; i++ )
			Sprite[0]->Add(Sprite[i]);
	}

	switch( Pattern ){
	case GLASSCURSOR_PATTERN_BLIGHT :
		{

			//グレア光
			sblt.RBM      =  bmAdd; // αブレンドモード
			sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
			sblt.RDDDD    =  FOwner->GetDG();     // TDDDDの参照
			sblt.RHeight  =  48;        // 高さ
			sblt.RWidth   =  16;        // 幅
			sblt.RX       =  _x;        // X座標
			sblt.RY       =  _y;        // Y座標
			sblt.RZ       =  189;       // Z値
			//左端、真ん中、右端の部分にわけて作成
			for( u32 i = 9; i <= 11; i++ ) {
				Sprite[i] =  new TJoint(sblt);         // スプライトの生成
				Inc(numSpr,1);
				Sprite[i]->GetSpr()->SetColor(GLASSCURSOR_BLIGHT_RED, ctRed);
				Sprite[i]->GetSpr()->SetColor(GLASSCURSOR_BLIGHT_GREEN, ctGreen);
				Sprite[i]->GetSpr()->SetColor(0  , ctBlue);
				Sprite[i]->GetSpr()->FSmooth = true;
			}

			Sprite[9]->GetSpr()->AddTexture(Eff8Tex, 48, 1, 64, 48);
			Sprite[10]->GetSpr()->AddTexture(Eff8Tex, 64, 1, 80, 48);
			Sprite[11]->GetSpr()->AddTexture(Eff8Tex, 80, 1, 96, 48);

			//指定された長さに合わせて真ん中のパーツを拡大
			Sprite[10]->GetSpr()->ZoomR(_long/16.f, 1.0f);
			Sprite[10]->GetSpr()->MoveR(16,BLIGHT_YCORRECT);

			//右端パーツの位置を合わせる
			Sprite[11]->GetSpr()->MoveR(16+_long,BLIGHT_YCORRECT);

			Sprite[9]->GetSpr()->MoveR(0,BLIGHT_YCORRECT);

			for( u32 i = 9; i <= 11; i++ )
				Sprite[0]->Add(Sprite[i]);
		}break;

	case GLASSCURSOR_PATTERN_BLACK_LEFT : 
		{

			//黒帯
			MakeSpriteLite(3,X,Y+10,_long,32,200,bmNormal);

			Sprite[3]->GetSpr()->AddTexture(GaugeTex, 120, 192, 239, 223);    // テクスチャを登録
			Sprite[3]->GetSpr()->FSmooth = false;
			Sprite[3]->GetSpr()->SetColor(196,ctAlpha);
			Sprite[0]->Add(Sprite[3]);
		}break;

	case GLASSCURSOR_PATTERN_BLACK_RIGHT : 
		{

			//黒帯
			MakeSpriteLite(3,X,Y+10,_long,32,200,bmNormal);

			Sprite[3]->GetSpr()->AddTexture(GaugeTex, 1, 192, 111, 223);    // テクスチャを登録
			Sprite[3]->GetSpr()->FSmooth = false;
			Sprite[3]->GetSpr()->SetColor(196,ctAlpha);

			Sprite[0]->Add(Sprite[3]);
		}break;

	case GLASSCURSOR_PATTERN_BLACK_CENTER : 
		{

			//黒帯
			MakeSpriteLite(3,X,Y+10,_long,32,200,bmNormal);

			Sprite[3]->GetSpr()->AddTexture(GaugeTex, 1, 192, 231, 223);    // テクスチャを登録
			Sprite[3]->GetSpr()->FSmooth = false;
			Sprite[3]->GetSpr()->SetColor(196,ctAlpha);
			Sprite[0]->Add(Sprite[3]);
		}break;
	case GLASSCURSOR_PATTERN_SHADOW  :
		{
			//黒帯
			MakeSpriteLite(3,X,Y+2,_long,32,200,bmNormal);

			Sprite[3]->GetSpr()->AddTexture(GaugeTex, 161, 449, 336, 480);    // テクスチャを登録
			Sprite[3]->GetSpr()->SetColor(196,ctAlpha);
			Sprite[0]->Add(Sprite[3]);
		}break;
	}

	MaxSprOfCursor  =  numSpr;

}

TMojiAndGlassCursor::~TMojiAndGlassCursor()
{
	SentenceList.clear();
}

void TMojiAndGlassCursor::Move()
{
	if( (X == DeathPoint.X) && (Y == DeathPoint.Y) ) 
		Die();

	if( SlideDelayCount >= DelayTime ) Slide();
	if( (AlphaSpeed != 0) && (AlphaDelayCount >= AlphaDelayTime) ) 
		ModulateAlpha();

	X = RoundOff(Sprite[0]->GetSpr()->FCX);
	Y = RoundOff(Sprite[0]->GetSpr()->FCY);

	//点滅処理
	if( Blink ) BlinkControl();

	Inc(SlideDelayCount);
	Inc(AlphaDelayCount);

	TOBJ::Move();

}

void TMojiAndGlassCursor::Die()
{
	if( ! SentenceList.empty() ) {
		for( u32 i = 0 ; i < SentenceList.size(); i++ ) {
			SentenceList[i]->Die();
		}
	}
	TOBJ::Die();
}

void TMojiAndGlassCursor::AddSentence( const string& _moji, s32 _x, s32 _brank, s32 _red, s32 _green, s32 _blue, s32 _font )
{
	TSprSentence* TmpObj  =  new TSprSentence(FOwner,_moji,X+_x,Y,_brank,_font);
	TmpObj->SetColor(_red,_green,_blue);
	SentenceList.push_back(TmpObj);
	FOwner->Add(TmpObj);
	Sprite[0]->Add(TmpObj->Sprite[0]);
}

void TMojiAndGlassCursor::AddSentenceRight( const string& _moji, s32 _x, s32 _brank, s32 _red, s32 _green, s32 _blue, s32 _font )
{
	//出力する文字列の幅を求める
	s32 width  =  (_moji.size() * 16) + ((_moji.size()-1) * _brank);

	//文字列を右寄せで表示する(表示位置から文字列の幅を引く)
	AddSentence(_moji, _x-width,_brank,_red,_green,_blue,_font);

}

void TMojiAndGlassCursor::SetAlpha( s32 _alpha )
{
	CurrentAlpha  =  _alpha;
	for( u32 i = 0; i <= 2; i++ ) {
		Sprite[i]->GetSpr()->SetColor((s32)(_alpha * GLASSCURSOR_BLIND_ALPHA / 255),ctAlpha);

		if( (Pattern == GLASSCURSOR_PATTERN_GLASS) || (Pattern == GLASSCURSOR_PATTERN_BLIGHT) ) {
			Sprite[i+3]->GetSpr()->SetColor((s32)(_alpha * GLASSCURSOR_GLASS_ALPHA / 255),ctAlpha);
			Sprite[i+6]->GetSpr()->SetColor((s32)(_alpha * GLASSCURSOR_REFLEX_ALPHA / 255),ctAlpha);
		}

		if( (Pattern == GLASSCURSOR_PATTERN_BLIGHT) ) {
			Sprite[i+9]->GetSpr()->SetColor((s32)(_alpha * GLASSCURSOR_BLIGHT_RED / 255),ctRed);
			Sprite[i+9]->GetSpr()->SetColor((s32)(_alpha * GLASSCURSOR_BLIGHT_GREEN / 255),ctGreen);
		}
	}

	if( (Pattern == GLASSCURSOR_PATTERN_LINE) || (Pattern == GLASSCURSOR_PATTERN_BLACK_LEFT) || (Pattern == GLASSCURSOR_PATTERN_BLACK_RIGHT)) {
		Sprite[3]->GetSpr()->SetColor(_alpha,ctAlpha);
	}

	for( u32 i = MaxSprOfCursor ; i < numSpr; i++ ) {
		Sprite[i]->GetSpr()->SetColor((s32)(_alpha * GLASSCURSOR_PARTITION_ALPHA / 255),ctAlpha);
	}

	for( u32 i = 0 ; i < SentenceList.size(); i++ ) {
		((TSprSentence*)SentenceList[i])->SetAlpha((s32)(_alpha * GLASSCURSOR_FONT_ALPHA / 255));
	}
}

void TMojiAndGlassCursor::SetDest( s32 _x, s32 _y,s32 _delay )
{
	//目的地決定
	DestX  =  _x;
	DestY  =  _y;

	//ディレイ時間決定
	DelayTime  =  _delay;

	SlideDelayCount  =  0;
}

void TMojiAndGlassCursor::SetColor( s32 _red, s32 _green, s32 _blue )
{
	for( u32 i = 0 ; i < SentenceList.size(); i++ ) {
		((TSprSentence*)SentenceList[i])->SetColor(_red,_green,_blue);
	}

}

void TMojiAndGlassCursor::SetSpeed( s32 _x, s32 _y )
{
	XSpeedFactor = _x;
	YSpeedFactor = _y;

}

void TMojiAndGlassCursor::SetDestAlpha( s32 _dest, s32 _speed, s32 _delay )
{
	DestAlpha  =  _dest;
	AlphaDelayTime  =  _delay;
	s32 diff  =  DestAlpha - CurrentAlpha;
	AlphaSpeed  =  diff / _speed;

	AlphaDelayCount  =  0;

}

void TMojiAndGlassCursor::SetDeathPoint( s32 _x, s32 _y )
{
	DeathPoint.X  =  _x;
	DeathPoint.Y  =  _y;

}

void TMojiAndGlassCursor::SetBlink( bool _state )
{
	Blink  =  _state;

}

void TMojiAndGlassCursor::BlinkControl()
{
	if( Age % 4 == 0 )
		SetColor(255,0,0);
	else
		SetColor(255,255,255);
}

void TMojiAndGlassCursor::AddLine( s32 _x )
{
	//区切り線
	TSpriteEXBLT sblt;
	sblt.RBM      =  bmNormal; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  FOwner->GetDG();     // TDDDDの参照
	sblt.RHeight  =  16;        // 高さ
	sblt.RWidth   =  16;        // 幅
	sblt.RX       =  X+_x;        // X座標
	sblt.RY       =  Y+3;        // Y座標
	sblt.RZ       =  169;       // Z値

	Sprite[numSpr] =  new TJoint(sblt);         // スプライトの生成
	Sprite[numSpr]->GetSpr()->FSmooth = false;
	Sprite[numSpr]->GetSpr()->AddTexture(Eff8Tex, 1, 97, 16, 112);
	Sprite[numSpr]->GetSpr()->SetColor(GLASSCURSOR_PARTITION_ALPHA, ctAlpha);
	Sprite[numSpr]->GetSpr()->Zoom(1.6f, 1.0f);
	Sprite[numSpr]->GetSpr()->Rotate(1024);

	Sprite[0]->Add(Sprite[numSpr]);
	Inc(numSpr,1);

}

void TMojiAndGlassCursor::SetRank( s32 _rank )
{
	string ordinal;
	_rank = _rank % 100;
	switch( _rank ){
	case 1: case 21: case 31: case 41: case 51: case 61: case 71: case 81: case 91:ordinal = "s";	break;
	case 2: case 22: case 32: case 42: case 52: case 62: case 72: case 82: case 92:ordinal = "n";	break;
	case 3: case 23: case 33: case 43: case 53: case 63: case 73: case 83: case 93:ordinal = "r";	break;
	default:
		ordinal = "t";
	}

	TSprSentence* TmpObj  =  new TSprSentence(FOwner,InttoStr(_rank) + ordinal,X,Y,0,KFONTKIND_GOTHIC);
	SentenceList.push_back(TmpObj);
	FOwner->Add(TmpObj);
	Sprite[0]->Add(TmpObj->Sprite[0]);
}

TMultiple::TMultiple( TOBJList* owner, s32 _mul, s32 _x, s32 _y ) : TOBJ(owner)
{
	FObjectName = "TMultiple";
	X = _x;
	Y = _y;

	if( X>640-64 ) X = 640-64;
	if( X<64 ) X = 64;

	if( Y<64 ) Y = 64;
	if( Y>480-32 ) Y = 480-32;

	FKind = kEffect2;

	MakeSpriteLite(0, X,Y,48,64,500,bmNormal);       //"x"

		Sprite[0]->GetSpr()->AddTexture(FontTex_Gothic, 401, 1, 448, 64);    // テクスチャを登録
		Sprite[0]->GetSpr()->FSmooth = false;

	TPoint FPoint[10];
	//倍率をstringに変換
	string mul_str  =  InttoStr(_mul);
	s32 n = 0;
	for( u32 i = 0; i <= 1; i++ ) {
		for( u32 k = 0; k <= 4; k++ ) {
			FPoint[n].X = k*80;
			FPoint[n].Y = 64*i;
			Inc(n);
		}
	}

	//一桁ずつスプライト作成
	for( u32 i  =  0 ; i < mul_str.size(); i++ ) {
		MakeSpriteLite(i+1,X+32+i*48,Y,80,64,500,bmNormal);
		s32 moji  =  mul_str[i+1];
		Sprite[i+1]->GetSpr()->AddTexture(FontTex_Gothic, FPoint[moji].X, FPoint[moji].Y, FPoint[moji].X+80, FPoint[moji].Y+64);// テクスチャを登録
		Sprite[i+1]->GetSpr()->FSmooth = false;
		Sprite[0]->Add(Sprite[i+1]);
	}

	//中心位置調整
	s32 OffsetX  =  ((48+mul_str.size()*48) / 2);
	Sprite[0]->MoveR(-OffsetX,0);

}

void TMultiple::Move()
{
	TOBJ::Move();
	Sprite[0]->MoveR(0.f,-0.5f);

	for( u32 i = 0 ; i < numSpr; i++) {
		Sprite[i]->GetSpr()->SetColor(0, ctBlue);
		Sprite[i]->GetSpr()->SetColor(0, ctGreen);

		if( Age % 4 == 0 ) {
			Sprite[i]->GetSpr()->SetColor(255, ctBlue);
			Sprite[i]->GetSpr()->SetColor(255, ctGreen);
		}
	}

	if( (Age >= 50) ) Die();

}

TEnemyScore::TEnemyScore( TOBJList* owner, s32 _x, s32 _y, s32 _score ) : TOBJ(owner)
{
	FObjectName = "TEnemyScore";
	X = _x;
	Y = _y;
	FKind = kEffect2;

	string score_str  =  InttoStr(_score);

	//中心位置調整
	s32 OffsetX  =  ((score_str.size()*18) / 2);
	sentence  =  new TSprSentence(FOwner,score_str,_x-OffsetX,_y,0,KFONTKIND_BLIGHT);
	sentence->SetZ(501);
	FOwner->Add(sentence);

}

void TEnemyScore::Move()
{
	TOBJ::Move();
	sentence->Sprite[0]->MoveR(0.f,-0.5f);
	if( (Age >= 50) ) Die();

}

void TEnemyScore::Die()
{
	TOBJ::Die();
	sentence->Die();
}

THitCount::THitCount( TOBJList* owner, s32 _x, s32 _y ) : TOBJ(owner)
{
	FObjectName = "THitCount";
	CurrentHit  =  FOwner->TotalHit;
	X = _x;
	Y = _y;

	FKind = kEffect2;

	//hitをstringに変換
	string hit_str  =  InttoStr(CurrentHit);

	//出現位置が画面右端になるように座標を再設定
	X  =  X-80-hit_str.size()*56;

	MakeSpriteLite(0, X,Y,64,48,500,bmNormal);       //"hit"

		Sprite[0]->GetSpr()->AddTexture(FontTex_Gothic, 401, 65, 464, 112);    // テクスチャを登録
		Sprite[0]->GetSpr()->FSmooth = false;

	s32 n = 0;
	TPoint FPoint[10];
	for( u32 i = 0; i <= 1; i++ ) {
		for( u32 k = 0; k <= 4; k++ ) {
			FPoint[n].X = k*80;
			FPoint[n].Y = 64*i;
			Inc(n);
		}
	}

	//一桁ずつスプライト作成
	for( u32 i  =  0 ; i < hit_str.size(); i++ ) {
		MakeSpriteLite(i+1,X+48+i*56,Y,80,64,500,bmNormal);
		s32 moji  =  hit_str[i+1];
		Sprite[i+1]->GetSpr()->AddTexture(FontTex_Gothic, FPoint[moji].X, FPoint[moji].Y, FPoint[moji].X+80, FPoint[moji].Y+64);// テクスチャを登録
		Sprite[i+1]->GetSpr()->FSmooth = false;
		Sprite[0]->Add(Sprite[i+1]);
	}  

	if( (FOwner->HeroPos.Y < KHEROPOS_TOPGAUGE_HIDED)  ) {
		for( u32 i  =  0 ; i < numSpr; i++ ) {
			Sprite[i]->GetSpr()->SetColor(48,ctAlpha);
		}
	}
}

void THitCount::Move()
{
	TOBJ::Move();
	if( Age > 120 ) {
		for( u32 i  =  0 ; i < numSpr; i++ ) {
			Sprite[i]->GetSpr()->SetColorR(-8,ctAlpha);
			Sprite[i]->GetSpr()->MoveR(32,0);
		}
	}
	else if( (FOwner->HeroPos.Y < KHEROPOS_TOPGAUGE_HIDED)  ) {
		for( u32 i  =  0 ; i < numSpr; i++ ) {
			Sprite[i]->GetSpr()->SetColor(48,ctAlpha);
		}
	}
	else {
		for( u32 i  =  0 ; i < numSpr; i++ ) {
			Sprite[i]->GetSpr()->SetColor(255,ctAlpha);
		}
	}

	//hit数が更新されたら消滅
	if( CurrentHit != FOwner->TotalHit  ) Die();

	if( (Age >= 150) ) Die();

}

void THitCount::Die()
{
	TOBJ::Die();
}

TCursorList::TCursorList( TOBJList* owner )
{
	FOwner  =  owner;
}

TCursorList::~TCursorList()
{
	if( ! m_array.empty() ) {
		for( u32 i = 0 ; i < m_array.size(); i++) {
			m_array[i]->Die();
		}
	}
}

void TCursorList::MakeCursor( const string& _caption, s32 _x, s32 _y, s32 _blank, s32 _width, s32 _num, s32 _font, s32 _pat, s32 _pos )
{
	MakeCursor_Base(_caption, _x, _y, _blank, _width, _num, _font, _pat, _pos, 255, 255, 255, 255);

}

void TCursorList::MakeCursor_Base( const string& _caption, s32 _x, s32 _y, s32 _blank, s32 _width, s32 _num, s32 _font, s32 _pat, s32 _pos, s32 _red, s32 _green, s32 _blue, s32 _alpha )
{
	TMojiAndGlassCursor* tmpObj  =  new TMojiAndGlassCursor(FOwner,_x+OrgX,_y+OrgY,_width,_pat);
	FOwner->Add(tmpObj);
	m_array.push_back(tmpObj);

	s32 fontsize;
	if( _caption.size() > 0 ) {
		switch( _font ){
		case KFONTKIND_NORMAL:fontsize  =  16;	break;
		case KFONTKIND_BLIGHT:fontsize  =  18;	break;
		case KFONTKIND_ORANGE:fontsize  =  18;	break;
		case KFONTKIND_GOTHIC:fontsize  =  48;	break;
		default: fontsize  =  16;
		}

		s32 textXPos;
		switch( _pos ){
		case KCURSOR_POS_LEFT:textXPos  =  fontsize;	break;
		case KCURSOR_POS_CENTER:textXPos  =  320 - (_caption.size()*fontsize + (_caption.size()-1)*_blank) / 2 - _x;	break;
		case KCURSOR_POS_RIGHT:textXPos  =  _width - _caption.size() * fontsize - (_caption.size()-1) * _blank - fontsize*2;	break;
		default:
			textXPos  =  0;
		}

		tmpObj->AddSentence(_caption, textXPos, _blank, _red, _green, _blue,_font);
	}

	if( _num>=0 ) {
		tmpObj->SetSpeed(5, 5);
		tmpObj->SetDest(_x,_y,_num*5);
		tmpObj->SetAlpha(0);
		tmpObj->SetDestAlpha(255,10,_num*7);
	}
}

void TCursorList::ClearCursors( s32 _x, s32 _y )
{
	for( u32 i = 0 ; i < m_array.size(); i++ ) {
		TMojiAndGlassCursor* tmpObj =  m_array[i];
		tmpObj->SetSpeed(5, 5);
		tmpObj->SetDest(tmpObj->X+_x,tmpObj->Y+_y,0);
		tmpObj->SetDestAlpha(0,15,0);
		tmpObj->SetDeathPoint(tmpObj->X+_x,tmpObj->Y+_y);
	}

	m_array.clear();
}

void TCursorList::SetAppearPoint( s32 _x, s32 _y )
{
	OrgX  =  _x;
	OrgY  =  _y;
}

TGothic_counter::TGothic_counter( TOBJList* owner, s32 fx, s32 fy, s32 z, s32 _desit, bool _showzero ) : TOBJ(owner)
{
	FObjectName = "TGothic_counter";
	X = fx;
	Y = fy;
	Desit = _desit;
	ShowZero = _showzero;
	FKind = kGauge;

	s32 n = 0;
	TPoint FPoint[12];
	for( u32 i = 0; i <= 1; i++ ) {
		for( u32 k = 0; k <= 4; k++ ) {
			FPoint[n].X = k*80;
			FPoint[n].Y = 64*i;
			Inc(n);
		}
	}

	for( u32 k  =  0 ; k < Desit; k++ )  {
		MakeSpriteLite(k, X, Y,80,64,z+k,bmNormal);
		Inc(numSpr,1);
		for( u32 i = 0; i <= 11; i++ )
			Sprite[k]->GetSpr()->AddTexture(FontTex_Gothic, FPoint[i].X, FPoint[i].Y, FPoint[i].X+80, FPoint[i].Y+64);// テクスチャを登録
		Sprite[k]->GetSpr()->FAnim = true;
		Sprite[k]->GetSpr()->FAnimBgnIdx = 0;
		Sprite[k]->GetSpr()->FAnimEndIdx = 11;
		Sprite[k]->GetSpr()->FVisible  =  true;
		Sprite[k]->GetSpr()->Move(X+56*k,Y);
		Sprite[k]->GetSpr()->FSmooth = false;
	}

}

void TGothic_counter::Move()
{
	TOBJ::Move();

}

void TGothic_counter::Update( s32 _val )
{
	if( Desit<=0 ) return;

	string tempStr  =  InttoStr(_val);
	s32 numZero  =  Desit - tempStr.size();
	if( numZero<0 ) return;

	for( u32 i  =  0 ; i < Desit; i++ ) {
		if( i<numZero ) {
			if( ShowZero )
				Sprite[i]->GetSpr()->FAnimIdx = 0;   //ゼロサプレス
			else
				Sprite[i]->GetSpr()->FVisible = false;
		}
		else {
			Sprite[i]->GetSpr()->FVisible = true;
			Sprite[i]->GetSpr()->FAnimIdx = tempStr[i-numZero+1];
		}
	}

}

TComBonus2::TComBonus2( TOBJList* owner, s32 _x, s32 _y, s32 _base, s32 _mul ) : TOBJ(owner)
{
	FObjectName = "TComBonus2";
	X = _x;
	Y = _y;
	FKind = kEffect2;

	string bonusstr[2];
	bonusstr[0] = "bonus";
	bonusstr[1] = InttoStr(_base*_mul);

	for( u32 i = 0; i <= 1; i++ ) {
		s32 OffsetX  =  (bonusstr[i].size() * 18) / 2;
		sentence[i]  =  new TSprSentence(FOwner,bonusstr[i],_x-OffsetX,_y+i*24,0,KFONTKIND_BLINK);
		FOwner->Add(sentence[i]);
		sentence[i]->SetZ(500);
	}
}

void TComBonus2::Move()
{
	TOBJ::Move();
	for( u32 i = 0; i <= 1; i++ )
		sentence[i]->Sprite[0]->MoveR(0.f,-0.5f);
	if( (Age >= 50) ) Die();

}

void TComBonus2::Die()
{
	TOBJ::Die();
	for( u32 i = 0; i <= 1; i++ )
		sentence[i]->Die();

}
