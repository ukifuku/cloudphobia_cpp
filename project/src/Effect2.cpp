// (.) = (\d) to (\d)
// $1 = $2; $1 <= $3; $1++

#include "Effect2.h"
#include "Effect.h"
#include "TOBJList.h"
#include "SpriteEXM.h"
#include "SXScene.h"


TKagerouBasic::TKagerouBasic( TOBJList* owner, s32 sx, s32 sy, s32 R, s32 S, s32 _lens, s32 _pat, f32 _zoom ) : TOBJ(owner)
{
	FObjectName = "TKagerouBasic";
	X = sx;
	Y = sy;
	Rdeg = R;
	Speed = S;
	FKind = kEffect;
	Pattern = _pat;
	TotalKage++;

	TSpriteEXBLT sblt;
		sblt.RBM      =  bmNormal; // αブレンドモード
		sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
		sblt.RDDDD    =  QD;     // TDDDDの参照
		sblt.RHeight  =  48;        // 幅
		sblt.RWidth   =  48;        // 高さ
		sblt.RX       =  0;        // X座標
		sblt.RY       =  0;        // Y座標
		sblt.RZ       =  KageExpZ;       // Z値

	KageExpZ++;
	if( KageExpZ > 7000 ) KageExpZ = 5000;

	Sprite[0] =  new TJoint(sblt);         // スプライトの生成
	numSpr++;
	for( u32 k = 0; k <= 3; k++ )
		Sprite[0]->GetSpr()->AddTexture(FontTex, k*32, 128, 31+k*32, 159);    // テクスチャを登録
	for( u32 k = 0; k <= 3; k++ )
		Sprite[0]->GetSpr()->AddTexture(FontTex, k*32, 160, 31+k*32, 191);    // テクスチャを登録
	for( u32 k = 0; k <= 3; k++ )
		Sprite[0]->GetSpr()->AddTexture(FontTex, k*32, 192, 31+k*32, 223);    // テクスチャを登録
	Sprite[0]->GetSpr()->FAnim = true;
	Sprite[0]->GetSpr()->FAnimBgnIdx = 0;
	Sprite[0]->GetSpr()->FAnimEndIdx = 11;
	Sprite[0]->GetSpr()->FAnimItv = 3;
	Sprite[0]->GetSpr()->FCX += 24;
	Sprite[0]->GetSpr()->FCY += 24;
	Sprite[0]->GetSpr()->Move(X,Y);
	Sprite[0]->GetSpr()->Zoom(_zoom,_zoom);
	Sprite[0]->GetSpr()->FBumpSize = 0.9f;//-_lens*0.01;
	Sprite[0]->GetSpr()->FEffectNum = 2;
	Sprite[0]->GetSpr()->FBumpLevel = -1.76f;
	Sprite[0]->GetSpr()->FBumpTex = FontTex;
	Sprite[0]->GetSpr()->FVisible = true;
}

void TKagerouBasic::Move()
{
	Sprite[0]->MoveR(cost[Rdeg]*Speed,sint[Rdeg]*Speed-0.5f);

	// if( Age>20 )
	Sprite[0]->GetSpr()->SetColorR(-12, ctAlpha);

	switch( Pattern ){
	case 0:
		{
			if( Age % 4 == 0 )
				Sprite[0]->GetSpr()->AnimNext();

			if( Age >= 44 ) Die();
		}break;
	case 1:
		{
			if( Age % 2 == 0 )
				Sprite[0]->GetSpr()->AnimNext();

			if( Age >= 22 ) Die();
		}break;
	}

	TOBJ::Move();

}

void TKagerouBasic::Die()
{
	if( TotalKage>0 ) TotalKage--;  

	TOBJ::Die();
}

TKageExp::TKageExp( TOBJList* owner, s32 sx, s32 sy, u16 _level )	: TOBJ(owner)
{
	FObjectName = "TKageExp";
	X = sx;
	Y = sy;
	FSTR = 2;
	FKind = kEffect;
	Level = _level;
	switch( _level ){
		case 0:Life = 5;	break;
		case 1:Life = 11;	break;
		case 2:Life = 21;	break;
		case 3:Life = 6;	break;
	}

}

void TKageExp::Move()
{
	switch( Level ) {
	case 0:
		{
			if( (Age % 3 == 0) && (TotalKage <= MaxKage ) ) FOwner->Add(new TKagerouBasic2(FOwner,X-30+random(60),Y-30+random(60),random(15)*256,random(1),random(20)+60,0,(f32)random(10)*0.1f+3.f));  //中心の爆炎
		}break;
	case 1:
		{
			if( (Age % 2 == 0) && (TotalKage <= MaxKage ) ) FOwner->Add(new TKagerouBasic2(FOwner,X-65+random(130),Y-65+random(130),random(15)*256,random(1),random(20)+60,0,(f32)random(10)*0.1f+3.f));  //中心の爆炎
		}break;
	case 2:
		{
			if( (Age % 2 == 0) && (TotalKage <= MaxKage ) ) FOwner->Add(new TKagerouBasic2(FOwner,X-70+random(140),Y-70+random(140),random(15)*256,random(1),random(20)+60,0,(f32)random(10)*0.15f+3.f));  //中心の爆炎
			if( (Age % 2 == 0) && (TotalKage <= MaxKage ) ) FOwner->Add(new TKagerouBasic2(FOwner,X-100+random(200),Y-100+random(200),random(15)*256,random(1),random(20)+60,0,(f32)random(10)*0.1f+3.f));
		}break;
	case 3:
		{
			for( u32 i = 0; i <= 8; i++ ) {
				//        if( (Age % 2 = 0) && (TotalKage <= MaxKage ) ) FOwner->Add(TKagerouBasic.Create(FOwner,640-random(14)*32,480-random(10)*32,random(15)*256,random(2),random(20)+60,0,random(10)*0.2+1));
			}
		}break;
	}
	//  テスト用
	// if( Age=1 ) FOwner->Add(TKagerouBasic2.Create(FOwner,X-100+random(200),Y-100+random(200),random(15)*256,random(1),random(20)+60,0,random(10)*0.1+3));

	if( Age>Life ) Die();

	TOBJ::Move();

}

TBallKage::TBallKage( TOBJList* owner, s32 sx, s32 sy, s32 R, s32 S, s32 _lens, s32 _pat, f32 _zoom ) : TOBJ(owner)
{
	FObjectName = "TBallKage";
	X = sx;
	Y = sy;
	Rdeg = R;
	Speed = S;
	FKind = kEffect;
	Pattern = _pat;
	Life = 10;

	TSpriteEXBLT sblt;
	sblt.RBM      =  bmNormal; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  QD;     // TDDDDの参照
	sblt.RHeight  =  30;        // 幅
	sblt.RWidth   =  30;        // 高さ
	sblt.RX       =  0;        // X座標
	sblt.RY       =  0;        // Y座標
	sblt.RZ       =  KageExpZ;       // Z値

	KageExpZ++;
	if( KageExpZ > 7000 ) KageExpZ = 5000;

	Sprite[0] = new TJoint(sblt);         // スプライトの生成
	numSpr++;
	Sprite[0]->GetSpr()->AddTexture(BumpTex, 161, 417, 161+63, 417+63);    // テクスチャを登録

	Sprite[0]->GetSpr()->FCX += 15;
	Sprite[0]->GetSpr()->FCY += 15;
	Sprite[0]->GetSpr()->Move(X,Y);
	Sprite[0]->GetSpr()->Zoom(_zoom,_zoom);
	Sprite[0]->GetSpr()->FEffectNum = 1;
	Sprite[0]->GetSpr()->FBumpSize = (f32)_lens;
	Sprite[0]->GetSpr()->FVisible = true;
	Sprite[0]->GetSpr()->FBumpTex = BumpTex;

}

void TBallKage::Move()
{
	if( Pattern == 0 )
		Die();

	TOBJ::Move();
}

TMosaicBasic::TMosaicBasic( TOBJList* owner, s32 sx, s32 sy, s32 _lens, s32 _width ) : TOBJ(owner)
{
	X = sx;
	Y = sy;
	FKind = kGauge;

	TSpriteEXBLT sblt;
	sblt.RBM      =  bmNormal; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  QD;     // TDDDDの参照
	sblt.RHeight  =  _width;        // 幅
	sblt.RWidth   =  _width;        // 高さ
	sblt.RX       =  0;        // X座標
	sblt.RY       =  0;        // Y座標
	sblt.RZ       =  300;       // Z値

	Sprite[0] = new TJoint(sblt);         // スプライトの生成
	numSpr++;
	Sprite[0]->GetSpr()->AddTexture(Eff1Tex, 17, 1, 31, 15);    // テクスチャを登録
	Sprite[0]->GetSpr()->Move(X,Y);
	Sprite[0]->GetSpr()->FEffectNum = 1;
	Sprite[0]->GetSpr()->FBumpSize = (f32)_lens;
	Sprite[0]->GetSpr()->FVisible = true;
}

void TMosaicBasic::Move()
{
	Die();
	TOBJ::Move();
}

TBloomingBasic::TBloomingBasic( TOBJList* owner, s32 _x, s32 _y, s32 _width, s32 _height, s32 _z ) : TOBJ(owner)
{
	X = _x;
	Y = _y;
	FKind = kGauge;
	Height = _height;
	Width = _width;
	Z = _z;

	TSpriteEXBLT sblt;
	sblt.RBM      =  bmSubAdd; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  QD;     // TDDDDの参照
	sblt.RHeight  =  _height;        // 高さ
	sblt.RWidth   =  _width;        //  幅
	sblt.RX       =  0;        // X座標
	sblt.RY       =  0;        // Y座標
	sblt.RZ       =  _z;       // Z値

	Sprite[0] = new TJoint(sblt);         // スプライトの生成
	numSpr++;
	Sprite[0]->GetSpr()->AddTexture(FOwner->GetSprMan()->GetBloomTex2(), 1, 1, FOwner->GetSprMan()->GetBloomTex2()->GetWidth(), FOwner->GetSprMan()->GetBloomTex2()->GetHeight());
	Sprite[0]->GetSpr()->Move(X,Y);
	Sprite[0]->GetSpr()->FVisible = false;

	sblt.RBM      =  bmNormal; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  QD;     // TDDDDの参照
	sblt.RHeight  =  _height / 3;        // 高さ
	sblt.RWidth   =  _width / 3;        //  幅
	sblt.RX       =  0;        // X座標
	sblt.RY       =  0;        // Y座標
	sblt.RZ       =  _z-1;       // Z値

	Sprite[1] = new TJoint(sblt);         // スプライトの生成
	numSpr++;
	Sprite[0]->GetSpr()->AddTexture(FOwner->GetSprMan()->GetBloomTex2(), 1, 1, FOwner->GetSprMan()->GetBloomTex2()->GetWidth(), FOwner->GetSprMan()->GetBloomTex2()->GetHeight());
	Sprite[1]->GetSpr()->Move(X,Y);
	Sprite[1]->GetSpr()->FVisible = false;

	Sprite[0]->GetSpr()->SetColor(255, ctRed);
	Sprite[0]->GetSpr()->SetColor(255, ctBlue);
	Sprite[0]->GetSpr()->SetColor(255, ctGreen);
	Sprite[0]->GetSpr()->SetColor(0, ctAlpha);

	FOwner->GetSprMan()->SetBloomBlend( 5);
	FOwner->GetSprMan()->SetBloomLevelRed(36);
	FOwner->GetSprMan()->SetBloomLevelGreen(23);
	FOwner->GetSprMan()->SetBloomLevelBlue(20);

	for( u32 i = 0; i <= 4; i++ )
		Current[i] = 0;

	SetColor(255,0,36,23,20);

}

void TBloomingBasic::Move()
{
	TOBJ::Move();
	Sprite[0]->GetSpr()->FVisible = Blooming;
	Sprite[1]->GetSpr()->FVisible = LuminanceMap;

	//画面振動時にズレるのを防ぐ
	Sprite[0]->Move(FOwner->GetSprMan()->FX,FOwner->GetSprMan()->FY);

	s32 FinalCol[5];
	for( u32 i = 0; i <= 4; i++ ) {
		if( Speed[i]>0 ) {
			if( (DestCol[i] - Current[i]) >= Speed[i] ) {
				Current[i] = Current[i]+Speed[i];
			}
		}
		else {
			if( (DestCol[i] - Current[i]) <= Speed[i] ) {
				Current[i] = Current[i]+Speed[i];
			}
		}

		FinalCol[i] = RoundOff(Current[i]);

		FinalCol[i]  =  glm::max(0, glm::min(255, FinalCol[i]));
	}

		Sprite[0]->GetSpr()->SetColor(FinalCol[0], ctRed);
		Sprite[0]->GetSpr()->SetColor(FinalCol[0], ctBlue);
		Sprite[0]->GetSpr()->SetColor(FinalCol[0], ctGreen);
		Sprite[0]->GetSpr()->SetColor(FinalCol[1], ctAlpha);
	FOwner->GetSprMan()->SetBloomLevelRed( FinalCol[2]);
	FOwner->GetSprMan()->SetBloomLevelGreen( FinalCol[3]);
	FOwner->GetSprMan()->SetBloomLevelBlue( FinalCol[4]);

}

void TBloomingBasic::Show()
{

}

void TBloomingBasic::SetColor( s32 _add, s32 _alpha, s32 _r, s32 _g, s32 _b )
{
	DestCol[0] = (f32)_add;
	DestCol[1] = (f32)_alpha;
	DestCol[2] = (f32)_r;
	DestCol[3] = (f32)_g;
	DestCol[4] = (f32)_b;

	for( u32 i = 0; i <= 4; i++ ) {
		Speed[i]  =  (DestCol[i] - Current[i]) / 120.f;
	}

}

void TBloomingBasic::OnDeviceReset()
{
	TOBJ::OnDeviceReset();
	//テクスチャが解放されているので一旦スプライトを破棄
	if( Sprite[0] != NULL ) {
		SAFE_DELETE(Sprite[0]);
		SAFE_DELETE(Sprite[1]);
	}

	//フィルターを作成し直し
	MakeSpriteLite(0, 0,0,Width,Height,Z,bmSubAdd);
	Sprite[0]->GetSpr()->AddTexture(FOwner->GetSprMan()->GetBloomTex2(), 1, 1, FOwner->GetSprMan()->GetBloomTex2()->GetWidth(), FOwner->GetSprMan()->GetBloomTex2()->GetHeight());
	Sprite[0]->GetSpr()->Move(X,Y);

	MakeSpriteLite(1, 0,0,Width / 3,Height / 3 ,Z-1,bmNormal);
	Sprite[1]->GetSpr()->AddTexture(FOwner->GetSprMan()->GetBloomTex2(), 1, 1, FOwner->GetSprMan()->GetBloomTex2()->GetWidth(), FOwner->GetSprMan()->GetBloomTex2()->GetHeight());
	Sprite[1]->GetSpr()->Move(X,Y);

}

void TBloomingBasic::SetColorSoon( s32 _add, s32 _alpha, s32 _r, s32 _g, s32 _b )
{
	DestCol[0] = (f32)_add;
	DestCol[1] = (f32)_alpha;
	DestCol[2] = (f32)_r;
	DestCol[3] = (f32)_g;
	DestCol[4] = (f32)_b;

	for( u32 i = 0; i <= 4; i++ ) {
		Speed[i]  =  (DestCol[i] - Current[i]);
	}

}

TBlur::TBlur( TOBJList* owner ) : TOBJ(owner)
{
	FKind = kGauge;
	Alpha = 0;
	IsResume = false;
	TSpriteEXBLT sblt;
	sblt.RBM      =  bmNormal; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  QD;     // TDDDDの参照
	sblt.RHeight  =  480;        // 高さ
	sblt.RWidth   =  640;        //  幅
	sblt.RX       =  0;        // X座標
	sblt.RY       =  0;        // Y座標
	sblt.RZ       =  250;       // Z値

	Sprite[0] = new TJoint(sblt);         // スプライトの生成
	numSpr++;
	//この時点ではまだぼかし用のテクスチャは作成されていない
	Sprite[0]->GetSpr()->AddTexture(FOwner->GetSprMan()->GetBlurTex2(), 0, 0, FOwner->GetSprMan()->GetBlurTex2()->GetWidth()-1, FOwner->GetSprMan()->GetBlurTex2()->GetHeight()-1);    // テクスチャを登録
	Sprite[0]->GetSpr()->FVisible = true;              // ここをfalseにすればポーズ中もボケなくなる
	Sprite[0]->GetSpr()->SetColor(0, ctAlpha);
}

void TBlur::Move()
{
	TOBJ::Move();
	if(IsResume ) {
		FOwner->Add(new TBox(FOwner,0,0,640,31,0,200,255));
		FOwner->SentenceCenter("pause",320,0,32,3);

		FOwner->Add(new TBox(FOwner,0,448,640,480,0,200,255));
		FOwner->SentenceCenter("cloudphobia",320,448,32,3);
	}

	if( Age == 1 ) FOwner->GetSprMan()->MakeBlur();    //ぼかしテクスチャ作成（実際にできるのは1フレーム後）
	if( Age < 2 ) return;    //ぼかしテクスチャが完成するまで何もしない

	if( Age <= 32 ) Fadein();

	if( (Age > 32) && (IsResume) ) Fadeout();

}

void TBlur::Fadein()
{
	if( Alpha < 255 ) {
		Alpha += 8;
		if( Alpha > 255 ) Alpha = 255;
		Sprite[0]->GetSpr()->SetColor(Alpha, ctAlpha);
	}

}

void TBlur::Fadeout()
{
	if( Alpha > 0 ) {
		Alpha -= 8;
		if( Alpha < 0 ) Alpha = 0;
		Sprite[0]->GetSpr()->SetColor(Alpha, ctAlpha);
	}

	//完全に消えたらポーズ解除
	if( Alpha == 0 ) {
		FOwner->AfterLoopEvent = MMove;
		Die();
	}

}

void TBlur::Hide()
{
	Sprite[0]->GetSpr()->FVisible = false;

}

void TBlur::Show()
{
	Sprite[0]->GetSpr()->FVisible = true;

}

void TBlur::Resume()
{
	IsResume = true;

}

void TBlur::OnDeviceReset()
{
	TOBJ::OnDeviceReset();
	Age  =  0;

	if( Sprite[0] != NULL )
		SAFE_DELETE(Sprite[0]);

	MakeSpriteLite(0, 0,0,640,480,250,bmNormal);
	//この時点ではまだぼかし用のテクスチャは作成されていない
	Sprite[0]->GetSpr()->AddTexture(FOwner->GetSprMan()->GetBlurTex2(), 0, 0, FOwner->GetSprMan()->GetBlurTex2()->GetWidth()-1, FOwner->GetSprMan()->GetBlurTex2()->GetHeight()-1);    // テクスチャを登録
	Sprite[0]->GetSpr()->FVisible = true;              // ここをfalseにすればポーズ中もボケなくなる
	Sprite[0]->GetSpr()->SetColor(0, ctAlpha);
}

TKagerouBasic2::TKagerouBasic2( TOBJList* owner, s32 sx, s32 sy, s32 R, s32 S, s32 _lens, s32 _pat, f32 _zoom ) : TOBJ(owner)
{
	FObjectName = "TKagerouBasic2";
	X = sx;
	Y = sy;
	FKind = kEffect;
	Rdeg = R && 0x0fff;
	Speed = S;
	TotalKage++;
	Pattern = _pat;
	ActFlg = 0;

	TSpriteEXBLT sblt;
	sblt.RBM      =  bmNormal; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  QD;     // TDDDDの参照
	sblt.RHeight  =  48;        // 幅
	sblt.RWidth   =  48;        // 高さ
	sblt.RX       =  0;        // X座標
	sblt.RY       =  0;        // Y座標
	sblt.RZ       =  KageExpZ;       // Z値

	KageExpZ++;
	if( KageExpZ>7000 ) KageExpZ = 5000;

	Sprite[0] = new TJoint(sblt);         // スプライトの生成
	numSpr++;
		Sprite[0]->GetSpr()->AddTexture(Eff8Tex, 33, 129, 64, 160);
		Sprite[0]->GetSpr()->FCX += 24;
		Sprite[0]->GetSpr()->FCY += 24;
		Sprite[0]->GetSpr()->Move(X,Y);
		Sprite[0]->GetSpr()->SetColor(0, ctAlpha);

		Sprite[0]->GetSpr()->Zoom(_zoom,_zoom);
		Sprite[0]->GetSpr()->FBumpSize = 1.7f-_lens*0.01f;
		Sprite[0]->GetSpr()->FEffectNum = 2;

}

void TKagerouBasic2::Move()
{
	Sprite[0]->GetSpr()->MoveR(cost[Rdeg]*Speed,sint[Rdeg]*Speed-0.5f);
	Sprite[0]->GetSpr()->RotateR(16);

	switch( ActFlg ){
	case 0:{
		Sprite[0]->GetSpr()->SetColorR(8, ctAlpha);
		if( Sprite[0]->GetSpr()->GetColor(ctAlpha)>=255 ) ActFlg = 1;
		   }break;
	case 1:{
		Sprite[0]->GetSpr()->SetColorR(-8, ctAlpha);
		if( Sprite[0]->GetSpr()->GetColor(ctAlpha) == 0 ) Die();
		   }break;
	}

	TOBJ::Move();

}

void TKagerouBasic2::Die()
{
	if( TotalKage>0 ) TotalKage--;

	TOBJ::Die();

}

TWaterDrop::TWaterDrop( TOBJList* owner, s32 sx, s32 sy, s32 _size ) : TOBJ(owner)
{
	FObjectName = "TWaterDrop";
	X = sx;
	Y = sy;
	FKind = kEffect2;
	Alpha = random(64)+144;
	Size = _size;

}

void TWaterDrop::Move()
{
	TOBJ::Move();
	VX = (f32)X+(f32)(random(3)-1)/2.f;
	VY = (f32)Y+(f32)(random(3)-1)/2.f;
	Alpha--;
	if( Alpha<=0 ) Die();
	X -= random(FOwner->Speed / 2);

}

s32 TWaterDrop::OnExRender()
{
	UVRect uv;

	uv.u1 = 16.f / 255.f;
	uv.u2 = 32.f / 255.f;
	uv.v1 = 112.f / 255.f;
	uv.v2 = 127.f / 255.f;
	FOwner->GetScene()->SetEffectSprite(VX-Size/2,VY-Size/2,Size,Size,uv,Alpha,Eff8Tex,sxbAdd);
	//  FOwner->GetScene()->SetSpriteSX(VX-Size/2,VY-Size/2,Size,Size,9,uv,Alpha,Eff8Tex,sxbAdd);
	//  FOwner->GetScene()->SetEffectSprite(320,240,Size,Size,uv,Alpha,Eff8Tex,sxbAdd);

	return 1;
}

TWaterControl::TWaterControl( TOBJList* owner, s32 sx, s32 sy ) : TOBJ(owner)
{
	FObjectName = "TWaterControl";
	X = sx;
	Y = sy;
	FKind = kEffect2;
	Life = random(3)+5;

}

void TWaterControl::Move()
{
	TOBJ::Move();

	s32 size;
	if( Age <= 2 )
		size = 16+random(32);
	else
		size = 14+random(20);


	FOwner->Add(new TWaterDrop(FOwner,X,Y,size));
	Y += random(5)-1;
	X -= FOwner->Speed+random(2);

	if( Age > Life ) Die();
}

TRain::TRain( TOBJList* owner, s32 sx, s32 sy ) : TOBJ(owner)
{
	FObjectName = "TRain";
	X = sx;
	Y = sy;
	FKind = kEffect;
	s32 randomCol = random(80)+160;

	MakeSpriteLite(0, 0,0,8,8,1000,bmNormal);
	Sprite[0]->GetSpr()->AddTexture(Eff8Tex, 33, 129, 64, 160);
	Sprite[0]->GetSpr()->FVisible  =  true;
	Sprite[0]->GetSpr()->FCX += 4;
	Sprite[0]->GetSpr()->FCY += 4;
	Sprite[0]->GetSpr()->Move(X,Y);
	Sprite[0]->GetSpr()->FVisible = true;
	Sprite[0]->GetSpr()->SetColor(randomCol / 2,ctAlpha);
	Sprite[0]->GetSpr()->SetColor(randomCol,ctRed);
	Sprite[0]->GetSpr()->SetColor(randomCol,ctGreen);
	Sprite[0]->GetSpr()->SetColor(randomCol,ctBlue);
	Sprite[0]->GetSpr()->Zoom((f32)random(5) / 10.f + 0.5f, 20.f + (f32)random(10));
	Sprite[0]->GetSpr()->Rotate(256+random(512));
}

void TRain::Move()
{
	TOBJ::Move();
	Sprite[0]->GetSpr()->SlideMoveR(0,64);
	if( Sprite[0]->GetSpr()->FCY > 700 ) Die();
}

TNightVision::TNightVision( TOBJList* owner ) : TOBJ(owner)
{
	FObjectName = "TNightVision";
	FKind = kEffect2;
	whiteout = false;

	//赤フィルタ
	for( u32 i = 0; i <= 5; i++ ) {
		MakeSpriteLite(i, -32,i*96,672,96,900,bmMul);
		Sprite[i]->GetSpr()->AddTexture(ST2Tex2, 416, 144, 511, 240);
		Sprite[i]->GetSpr()->FSmooth  =  false;
		Sprite[i]->GetSpr()->FVisible = false;
	}

	//緑フィルタ
	s32 num = 6;
	for( s32 i = 0; i <= 5; i++ ) {
		for( s32 k = 0; k <= 6; k++ ) {
			MakeSpriteLite(num, 0,0,96,96,910,bmAdd);
			Sprite[num]->GetSpr()->AddTexture(ST2Tex2, 321, 145, 416, 241);
			Sprite[num]->GetSpr()->FSmooth  =  false;
			Sprite[num]->GetSpr()->Move(k*96 - 32, i*96 - 32);
			Sprite[num]->GetSpr()->FVisible = false;
			if( num > 6 ) Sprite[6]->Add(Sprite[num]);

			num++;
		}
	}

	black  =  num;

	//黒フィルタ
	MakeSpriteLite(black, 0,0,640,480,10501,bmNormal);
	Sprite[black]->GetSpr()->AddTexture(ST2Tex2, 416, 144, 511, 240);
	Sprite[black]->GetSpr()->FSmooth  =  false;
	Sprite[black]->GetSpr()->SetColor(0,ctRed);
	Sprite[black]->GetSpr()->SetColor(0,ctGreen);
	Sprite[black]->GetSpr()->SetColor(0,ctBlue);
	Sprite[black]->GetSpr()->SetColor(0,ctAlpha);
	num++;

	white = num;
	//白フィルタ
	MakeSpriteLite(white, 0,0,640,480,10600,bmNormal);
	Sprite[white]->GetSpr()->AddTexture(ST2Tex2, 416, 144, 511, 240);
	Sprite[white]->GetSpr()->FSmooth  =  false;
	Sprite[white]->GetSpr()->SetColor(0,ctAlpha);

	actflg = 0;
}

void TNightVision::Move()
{
	TOBJ::Move();
	//暗転
	if( Age < 60 )
		Sprite[black]->GetSpr()->SetColorR(16,ctAlpha);

	//暗闇解除
	if( Age == 120 ) Show(true);

	//明転
	if( Age > 120 )
		Sprite[black]->GetSpr()->SetColorR(-4,ctAlpha);

	if( whiteout )
		Sprite[white]->GetSpr()->SetColorR(16,ctAlpha);

	//ノイズ表現
	if( Age % 4 == 0 ) {
		switch( actflg ){
		case 0:Sprite[6]->MoveR(32,0);		break;
		case 1:Sprite[6]->MoveR(0,32);		break;
		case 2:Sprite[6]->MoveR(-32,0);		break;
		case 3:Sprite[6]->MoveR(0,-32);		break;
		}
		actflg++;

		if( actflg>3 ) actflg = 0;
	}

	Form1.Script.ExecuteProc(KSCRIPT_NIGHTVISION,this);

}

void TNightVision::Show( bool _show )
{
	if( _show ) {
		Sprite[black]->GetSpr()->FZ  =  600;
		Sprite[black]->GetSpr()->SetColor(255,ctAlpha);
		for( s32 i  =  0 ; i < black; i++ ) {
			Sprite[i]->GetSpr()->FVisible = true;
		}
	}
	else {
		Sprite[white]->GetSpr()->FVisible = false;
		Sprite[black]->GetSpr()->SetColor(255,ctAlpha);
		for( s32 i  =  0 ; i < black; i++ ) {
			Sprite[i]->GetSpr()->FVisible = false;
		}
	}

}

void TNightVision::MakeWhite()
{
	whiteout = true;

}

TStage2ScrollBlur::TStage2ScrollBlur( TOBJList* owner ) : TOBJ(owner)
{
	FObjectName = "TStage2ScrollBlur";
	FKind = kEffect2;
	ShowState = true;

	for( u32 i = 0; i <= 7; i++ ) {
		MakeSpriteLite(numSpr, 160*i,0,160,480,600,bmNormal);

		Sprite[numSpr-1]->GetSpr()->AddTexture(ST2Tex2,  257+32*i, 257, 287+32*i, 512);
	}

}

void TStage2ScrollBlur::Move()
{
	TOBJ::Move();
	for( u32 i = 0; i <= 7; i++ ) {
		if( ShowState )
			Sprite[i]->GetSpr()->SetColorR(16,ctAlpha);
		else
			Sprite[i]->GetSpr()->SetColorR(-16,ctAlpha);

		Sprite[i]->MoveR(64,0);
		if( Sprite[i]->GetSpr()->FCX>640 )
			Sprite[i]->GetSpr()->MoveR(-160*8,0);

		if( Sprite[i]->GetSpr()->FCX <-160 )
			Sprite[i]->GetSpr()->FVisible = false;
		else
			Sprite[i]->GetSpr()->FVisible = true;
	}

}

u16 TStage2ScrollBlur::Show( bool _show )
{
	ShowState = _show;

}

void Mosaic( TOBJList* owner, s32 sx, s32 sy, s32 _width, s32 _height, s32 _level )
{

}
