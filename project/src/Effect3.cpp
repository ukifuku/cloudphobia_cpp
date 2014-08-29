#include "Effect3.h"
#include "Effect.h"
#include "TOBJList.h"
#include "SpriteEXM.h"
#include "SXScene.h"

TBladeTrace::TBladeTrace( TOBJList* owner, TJoint* _obj, s32 _Width, s32 _Length, s32 _R ) : TOBJ(owner)
{
	FObjectName = "TBladeTrace";

	DivLevel = 16;
	BladeOwner = _obj;
	BladeWidth = _Width;
	X = RoundOff(BladeOwner->GetSpr()->FCX);
	Y = RoundOff(BladeOwner->GetSpr()->FCY);
	FKind = kEffect2;
	DiffR = _R;

	MAXElement  =  _Length;
	MAXElement_ORG  =  MAXElement / DivLevel;

		TSpriteEXBLT sblt;
		sblt.RBM      =  bmNormal; // αブレンドモード
		sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
		sblt.RDDDD    =  QD;     // TDDDDの参照
		sblt.RHeight  =  BladeWidth;        // 高さ
		sblt.RWidth   =  5;        //  幅
		sblt.RX       =  X;        // X座標
		sblt.RY       =  Y;        // Y座標
		sblt.RZ       =  15100;       // Z値

	for( u32 i = 0 ; i < MAXElement; i++ ) {
		PointX1.push_back(X);
		PointY1.push_back(Y);
		sblt.RZ = 15100+i;
		TSpriteEX* spr = new TSpriteEX(sblt);         // スプライトの生成
			for( u32 k = 1 ; k <= MAXElement_ORG ; k++)
				spr->AddTexture(FontTex, 128+(s32)(i*64/(k*DivLevel)), 128, 128+(s32)((i+1)*64/(k*DivLevel)) , 191);

			spr->FAnim  =  true;
			if( i <= MAXElement-DivLevel )
				spr->FVisible  =  true;

			if( ! ExtraEffects ) {
				spr->SetColor(35,ctRed);
				spr->SetColor(74,ctBlue);
				spr->SetColor(46,ctGreen);
			}

			spr->SetColor(196,ctAlpha);

			f32 UFactor  =  X/640.f;
			f32 VFactor  =  Y/480.f/2.f + 0.05f;
			spr->BumpUVUpdate(i/MAXElement/2.f + UFactor, 0 +VFactor, (i+1)/MAXElement/2.f + UFactor, 0.4f +VFactor);
			spr->FEffectNum = 3.f;
			Element.push_back(spr);
	}

	s32 RR = (BladeOwner->Rdegree+DiffR) & 0x0fff;
	PrevRR = RR;

	for( u32 i = 0 ; i < MAXElement; i++ ) {
		PointX2.push_back( PointX1[i]+BladeWidth*cost[RR] );
		PointY2.push_back( PointY1[i]+BladeWidth*sint[RR] );
		Element[i]->SetVertex(0,PointX1[0],PointY1[0]);
		Element[i]->SetVertex(1,PointX1[0],PointY1[0]);
		Element[i]->SetVertex(2,PointX2[0],PointY2[0]);
		Element[i]->SetVertex(3,PointX2[0],PointY2[0]);
	}

}

TBladeTrace::~TBladeTrace()
{
	for( u32 i = 0 ; i < MAXElement; i++ ) {
		SAFE_DELETE(Element[i]);
	}
}

void TBladeTrace::Move()
{
	if( FOwner->Weapon == false ) Die();

	//前フレームの頂点の位置をコピー
	for( u32 i = MAXElement-1; i >= DivLevel-1; i-- ) {
		PointX1[i] = PointX1[i-DivLevel+1];
		PointY1[i] = PointY1[i-DivLevel+1];
		PointX2[i] = PointX2[i-DivLevel+1];
		PointY2[i] = PointY2[i-DivLevel+1];

		if( i<=MAXElement-2 ) {
			Element[i]->SetVertex(0,PointX1[i],PointY1[i]);
			Element[i]->SetVertex(1,PointX1[i+1],PointY1[i+1]);
			Element[i]->SetVertex(2,PointX2[i],PointY2[i]);
			Element[i]->SetVertex(3,PointX2[i+1],PointY2[i+1]);
		}
	}

	//先頭の残像の根元の座標を算出
	PointX1[0] = RoundOff(BladeOwner->GetSpr()->FCX);
	PointY1[0] = RoundOff(BladeOwner->GetSpr()->FCY);

	//刀の角度をゲット
	s32 RR = (BladeOwner->Rdegree+DiffR) & 0x0fff;

	//先頭の残像の先端の座標を算出
	PointX2[0] = PointX1[0]+BladeWidth*cost[RR];
	PointY2[0] = PointY1[0]+BladeWidth*sint[RR];

	//先頭の残像をDIVTRACE個に分割
	for( u32 i = (DivLevel -2) ; i >= 1; i-- ) {
		//根元の座標を分割
		PointX1[i] = PointX1[DivLevel-1]+(PointX1[0]-PointX1[DivLevel-1])*(DivLevel-i)/DivLevel;
		PointY1[i] = PointY1[DivLevel-1]+(PointY1[0]-PointY1[DivLevel-1])*(DivLevel-i)/DivLevel;

		//角度も分割
		s32 DivRR  =  RoundOff(PrevRR-(GetDiffR(RR, PrevRR)*(DivLevel-i) / (f32)DivLevel)) & 0x0fff;

		//分割された角度を元に先端の座標を算出
		PointX2[i] = PointX1[i]+BladeWidth*cost[DivRR];
		PointY2[i] = PointY1[i]+BladeWidth*sint[DivRR];

		//座標セット
		if( i<=MAXElement-2 ) {
			Element[i]->SetVertex(0,PointX1[i],PointY1[i]);
			Element[i]->SetVertex(1,PointX1[i+1],PointY1[i+1]);
			Element[i]->SetVertex(2,PointX2[i],PointY2[i]);
			Element[i]->SetVertex(3,PointX2[i+1],PointY2[i+1]);
		}
	}

	//前フレームの刀の角度として保存
	PrevRR  =  RR;

	//先頭スプライトの頂点セット
	Element[0]->SetVertex(0,PointX1[0],PointY1[0]);
	Element[0]->SetVertex(1,PointX1[1],PointY1[1]);
	Element[0]->SetVertex(2,PointX2[0],PointY2[0]);
	Element[0]->SetVertex(3,PointX2[1],PointY2[1]);

	if( (Age > 0) && (Age < (MAXElement / DivLevel-1)) ) {
		for( u32 i = 0 ; i < MAXElement; i++ ) {
			Element[i]->AnimNext();
		}
	}

	X = RoundOff(BladeOwner->GetSpr()->FCX);
	Y = RoundOff(BladeOwner->GetSpr()->FCY);
	for( u32 i = 0 ; i < MAXElement; i++ ) {
		//環境バンプマップ作成のため、軌跡の座標を元にUVを作成する
		f32 UFactor  =  (X-256)/640.f;
		f32 VFactor  =  Y/480.f/2.f + 0.05f;
		Element[i]->BumpUVUpdate((f32)i/MAXElement/4.f + UFactor, 0 +VFactor, (f32)(i+1)/MAXElement/4.f + UFactor, 0.4f +VFactor);
	}


	//先頭と尻尾が重なったら消滅
	{  if( (Age > 0) &&
		(PointX1[MAXElement-1]==PointX1[0]) &&
		(PointY1[MAXElement-1]==PointY1[0]) &&
		(PointX2[MAXElement-1]==PointX2[0]) &&
		(PointY2[MAXElement-1]==PointY2[0])
		) Die();
	}
	TOBJ::Move();

}

s32 TBladeTrace::OnDraw()
{
	for( u32 i = 0 ; i < MAXElement; i++ )
		FOwner->GetSprMan()->AddSprite(Element[i]);

	return 0;
}

THomingTrace::THomingTrace( TOBJList* owner, TJoint* _obj, s32 _Width, s32 _Length ) : TOBJ(owner)
{
	FObjectName = "THomingTrace";

	FSTR = 1000;

	IsTerminated = false;
	BeamOwner = _obj;
	BeamWidth = _Width;
	X = RoundOff(BeamOwner->GetSpr()->FCX);
	Y = RoundOff(BeamOwner->GetSpr()->FCY);
	FKind = kEffect;

	MAXElement  =  _Length;

	//ビーム表示用スプライト
	TSpriteEXBLT sblt;
	sblt.RBM      =  bmAdd; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  QD;     // TDDDDの参照
	sblt.RHeight  =  BeamWidth;        // 高さ
	sblt.RWidth   =  BeamWidth;        //  幅
	sblt.RX       =  X;        // X座標
	sblt.RY       =  Y;        // Y座標
	sblt.RZ       =  15100;       // Z値

	for( u32 i = 0 ; i < MAXElement; i++ ) {
		PointX1.push_back(X);
		PointY1.push_back(Y);
		sblt.RZ = 15100+i;
		TSpriteEX* spr =  new TSpriteEX(sblt);         // スプライトの生成
		spr->AddTexture(Eff8Tex, 129 +(s32)(i*128.f/MAXElement), 1, 129 + (s32)((i+1)*128.f/MAXElement), 127);
		spr->FVisible = true;

		Element.push_back(spr);
	}

	s32 RR = (BeamOwner->Rdegree + 1024) & 0x0fff;
	PrevRR = RR;

	for( u32 i = 0 ; i < MAXElement; i++ ) {
		PointX2.push_back( PointX1[i]+BeamWidth*cost[RR] );
		PointY2.push_back( PointY1[i]+BeamWidth*sint[RR] );
		Element[i]->SetVertex(0,PointX1[0],PointY1[0]);
		Element[i]->SetVertex(1,PointX1[0],PointY1[0]);
		Element[i]->SetVertex(2,PointX2[0],PointY2[0]);
		Element[i]->SetVertex(3,PointX2[0],PointY2[0]);
	}
}

THomingTrace::~THomingTrace()
{
	for( u32 i = 0 ; i < MAXElement; i++ ) {
		SAFE_DELETE(Element[i]);
	}

}

void THomingTrace::Move()
{
	//前フレームの頂点の位置をコピー
	for( u32 i = MAXElement-1 ; i >= 1 ; i--) {
		PointX1[i] = PointX1[i-1];
		PointY1[i] = PointY1[i-1];
		PointX2[i] = PointX2[i-1];
		PointY2[i] = PointY2[i-1];

		if( i<=MAXElement-2 ) {
			Element[i]->SetVertex(0,PointX1[i],PointY1[i]);
			Element[i]->SetVertex(1,PointX1[i+1],PointY1[i+1]);
			Element[i]->SetVertex(2,PointX2[i],PointY2[i]);
			Element[i]->SetVertex(3,PointX2[i+1],PointY2[i+1]);
		}
	}

	//先頭の残像の根元の座標を算出
	PointX1[0] = RoundOff(BeamOwner->GetSpr()->FCX);
	PointY1[0] = RoundOff(BeamOwner->GetSpr()->FCY);

	//オーナーの角度をゲット
	s32 RR = (BeamOwner->Rdegree+1024) & 0x0fff;

	//先頭の残像の先端の座標を算出
	PointX2[0] = PointX1[0]+BeamWidth*cost[RR];
	PointY2[0] = PointY1[0]+BeamWidth*sint[RR];

	//前フレームのオーナーの角度として保存
	PrevRR  =  RR;

	//先頭スプライトの頂点セット
	Element[0]->SetVertex(0,PointX1[0],PointY1[0]);
	Element[0]->SetVertex(1,PointX1[1],PointY1[1]);
	Element[0]->SetVertex(2,PointX2[0],PointY2[0]);
	Element[0]->SetVertex(3,PointX2[1],PointY2[1]);

	//先頭と尻尾が重なったら消滅
	if( (Age > 0) &&
		(PointX1[MAXElement-1]==PointX1[0]) &&
		(PointY1[MAXElement-1]==PointY1[0]) &&
		(PointX2[MAXElement-1]==PointX2[0]) &&
		(PointY2[MAXElement-1]==PointY2[0])
		) IsTerminated = true;

	TOBJ::Move();

}

s32 THomingTrace::OnDraw()
{
	for( u32 i = 0 ; i < MAXElement; i++ )
		FOwner->GetSprMan()->AddSprite(Element[i]);

	for( u32 i = 0 ; i < numSpr; i++ )
		if( Sprite[i]!=NULL ) {
			FOwner->GetSprMan()->AddSprite(Sprite[i]->GetSpr());
		}

	return 0;
}

THomingTrace2::THomingTrace2( TOBJList* owner, TJoint* _obj, s32 _Width, s32 _Length ) : THomingTrace(owner, _obj, _Width, _Length)
{
	Age = 0;
	FOwner = owner;
	FDead = false;
	numSpr = 0;
	distance = 0;
	TempHP = 0;
	FDef = 0;
	LastSlashed = 0;

	FObjectName = "THomingTrace2";

	FSTR = 1000;

	IsTerminated = false;
	BeamOwner = _obj;
	BeamWidth = _Width;
	X = RoundOff(BeamOwner->GetSpr()->FCX);
	Y = RoundOff(BeamOwner->GetSpr()->FCY);
	FKind = kEffect2;
	MAXElement  =  _Length;

	//ビーム表示用スプライト
	TSpriteEXBLT sblt;
	sblt.RBM      =  bmAdd; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  QD;     // TDDDDの参照
	sblt.RHeight  =  BeamWidth;        // 高さ
	sblt.RWidth   =  BeamWidth;        //  幅
	sblt.RX       =  X;        // X座標
	sblt.RY       =  Y;        // Y座標
	sblt.RZ       =  15100;       // Z値

	for( u32 i = 0 ; i < MAXElement; i++ ) {
		PointX1.push_back(X);
		PointY1.push_back(Y);
		sblt.RZ = 15100+i;
		TSpriteEX* spr =  new TSpriteEX(sblt);         // スプライトの生成
		spr->AddTexture(Eff8Tex, 16 +(s32)(i*16.f/MAXElement), 128, 16 + (s32)((i+1)*16.f/MAXElement), 160);
		spr->FVisible = true;
		spr->SetColor(128, ctRed);
		spr->SetColor(128, ctGreen);
		Element.push_back(spr);
	}

	s32 RR = (BeamOwner->Rdegree + 1024) & 0x0fff;
	PrevRR = RR;

	for( u32 i = 0 ; i < MAXElement; i++ ) {
		PointX2.push_back( PointX1[i]+BeamWidth*cost[RR] );
		PointY2.push_back( PointY1[i]+BeamWidth*sint[RR] );
		Element[i]->SetVertex(0,PointX1[0],PointY1[0]);
		Element[i]->SetVertex(1,PointX1[0],PointY1[0]);
		Element[i]->SetVertex(2,PointX2[0],PointY2[0]);
		Element[i]->SetVertex(3,PointX2[0],PointY2[0]);
	}

}

TEnemyBladeTrace::TEnemyBladeTrace( TOBJList* owner, TJoint* _obj, s32 _Width, s32 _Length ) : TOBJ(owner)
{
	FObjectName = "TEnemyBladeTrace";

	DivLevel = 8;
	BladeOwner = _obj;
	BladeWidth = _Width;
	X = RoundOff(BladeOwner->GetSpr()->FCX);
	Y = RoundOff(BladeOwner->GetSpr()->FCY);
	FKind = kEffect2;
	MAXElement  =  _Length;
	MAXElement_ORG  =  MAXElement / DivLevel;

	TSpriteEXBLT sblt;
	sblt.RBM      =  bmNormal; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  QD;     // TDDDDの参照
	sblt.RHeight  =  BladeWidth;        // 高さ
	sblt.RWidth   =  5;        //  幅
	sblt.RX       =  X;        // X座標
	sblt.RY       =  Y;        // Y座標
	sblt.RZ       =  25100;       // Z値

	for( u32 i = 0 ; i < MAXElement; i++ ) {
		PointX1.push_back(X);
		PointY1.push_back(Y);
		sblt.RZ = 25100+i;
		TSpriteEX* spr =  new TSpriteEX(sblt);         // スプライトの生成
			for( u32 k = 1 ; k <= MAXElement_ORG; k++ )
				spr->AddTexture(Enemy4Tex, 144+(s32)(i*184.f/(k*DivLevel)), 511, 144+(s32)((i+1)*184.f/(k*DivLevel)) , 224);

			spr->FAnim  =  true;
			if( i <= MAXElement-DivLevel )
				spr->FVisible  =  true;

			spr->SetColor(196,ctAlpha);

		Element.push_back(spr);
	}

	s32 RR = (BladeOwner->Rdegree+1024) & 0x0fff;
	PrevRR = RR;

	for( u32 i = 0 ; i < MAXElement; i++ ) {
		PointX2.push_back( PointX1[i]+BladeWidth*cost[RR] );
		PointY2.push_back( PointY1[i]+BladeWidth*sint[RR] );
		Element[i]->SetVertex(0,PointX1[0],PointY1[0]);
		Element[i]->SetVertex(1,PointX1[0],PointY1[0]);
		Element[i]->SetVertex(2,PointX2[0],PointY2[0]);
		Element[i]->SetVertex(3,PointX2[0],PointY2[0]);
	}
}

TEnemyBladeTrace::~TEnemyBladeTrace()
{
	for( u32 i = 0 ; i < MAXElement; i++ ) {
		SAFE_DELETE(Element[i]);
	}
}

void TEnemyBladeTrace::Move()
{
	TOBJ::Move();
	//前フレームの頂点の位置をコピー
	for( u32 i = MAXElement-1 ; i >= DivLevel-1; i-- ) {
		PointX1[i] = PointX1[i-DivLevel+1];
		PointY1[i] = PointY1[i-DivLevel+1];
		PointX2[i] = PointX2[i-DivLevel+1];
		PointY2[i] = PointY2[i-DivLevel+1];

		if( i<=MAXElement-2 ) {
			Element[i]->SetVertex(0,PointX1[i],PointY1[i]);
			Element[i]->SetVertex(1,PointX1[i+1],PointY1[i+1]);
			Element[i]->SetVertex(2,PointX2[i],PointY2[i]);
			Element[i]->SetVertex(3,PointX2[i+1],PointY2[i+1]);
		}
	}

	//先頭の残像の根元の座標を算出
	PointX1[0] = RoundOff(BladeOwner->GetSpr()->FCX);
	PointY1[0] = RoundOff(BladeOwner->GetSpr()->FCY);

	//刀の角度をゲット
	s32 RR = (BladeOwner->Rdegree-163) & 0x0fff;

	//先頭の残像の先端の座標を算出
	PointX2[0] = PointX1[0]+BladeWidth*cost[RR];
	PointY2[0] = PointY1[0]+BladeWidth*sint[RR];

	//先頭の残像をDIVTRACE個に分割
	for( u32 i = (DivLevel -2) ; i >= 1; i-- ) {
		//根元の座標を分割
		PointX1[i] = PointX1[DivLevel-1]+(PointX1[0]-PointX1[DivLevel-1])*(DivLevel-i)/(f32)DivLevel;
		PointY1[i] = PointY1[DivLevel-1]+(PointY1[0]-PointY1[DivLevel-1])*(DivLevel-i)/(f32)DivLevel;

		//角度も分割
		s32 DivRR  =  RoundOff(PrevRR-(GetDiffR(RR, PrevRR)*(DivLevel-i) / (f32)DivLevel)) & 0x0fff;

		//分割された角度を元に先端の座標を算出
		PointX2[i] = PointX1[i]+BladeWidth*cost[DivRR];
		PointY2[i] = PointY1[i]+BladeWidth*sint[DivRR];

		//座標セット
		if( i<=MAXElement-2 ) {
			Element[i]->SetVertex(0,PointX1[i],PointY1[i]);
			Element[i]->SetVertex(1,PointX1[i+1],PointY1[i+1]);
			Element[i]->SetVertex(2,PointX2[i],PointY2[i]);
			Element[i]->SetVertex(3,PointX2[i+1],PointY2[i+1]);
		}
	}

	//前フレームの刀の角度として保存
	PrevRR  =  RR;

	//先頭スプライトの頂点セット
	Element[0]->SetVertex(0,PointX1[0],PointY1[0]);
	Element[0]->SetVertex(1,PointX1[1],PointY1[1]);
	Element[0]->SetVertex(2,PointX2[0],PointY2[0]);
	Element[0]->SetVertex(3,PointX2[1],PointY2[1]);

	if( (Age > 0) && (Age < (MAXElement / DivLevel-1)) ) {
		for( u32 i = 0 ; i < MAXElement; i++ ) {
			Element[i]->AnimNext();
		}
	}

	TOBJ::Move();

}

s32 TEnemyBladeTrace::OnDraw()
{
	for( u32 i = 0 ; i < MAXElement; i++ )
		FOwner->GetSprMan()->AddSprite(Element[i]);

	return 0;

}

TEnemyBladeTrace2::TEnemyBladeTrace2( TOBJList* owner, TJoint* _obj, s32 _Width, s32 _Length, s32 _R ) : TOBJ(owner)
{
	FObjectName = "TEnemyBladeTrace2";

	DivLevel = 8;
	BladeOwner = _obj;
	BladeWidth = _Width;
	X = RoundOff(BladeOwner->GetSpr()->FCX);
	Y = RoundOff(BladeOwner->GetSpr()->FCY);
	FKind = kEffect2;
	DiffR = _R;

	MAXElement  =  _Length;
	MAXElement_ORG  =  MAXElement / DivLevel;

	TSpriteEXBLT sblt;
	sblt.RBM      =  bmNormal; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  QD;     // TDDDDの参照
	sblt.RHeight  =  BladeWidth;        // 高さ
	sblt.RWidth   =  5;        //  幅
	sblt.RX       =  X;        // X座標
	sblt.RY       =  Y;        // Y座標
	sblt.RZ       =  20000;       // Z値

	for( u32 i = 0 ; i < MAXElement; i++ ) {
		PointX1.push_back(X);
		PointY1.push_back(Y);
		sblt.RZ = 20000+i;
		TSpriteEX* spr =  new TSpriteEX(sblt);         // スプライトの生成
			for( u32 k = 1 ; k <= MAXElement_ORG; k++ )
				spr->AddTexture(FontTex, 128+(s32)(i*64.f/(k*DivLevel)), 128, 128+(s32)((i+1)*64.f/(k*DivLevel)) , 191);

			spr->FAnim  =  true;
			if( i <= MAXElement-DivLevel )
				spr->FVisible  =  true;

			spr->SetColor(196,ctAlpha);

			if( ! ExtraEffects ) {
				spr->SetColor(35,ctRed);
				spr->SetColor(74,ctBlue);
				spr->SetColor(46,ctGreen);
			}

			f32 UFactor  =  X/640.f;
			f32 VFactor  =  Y/480.f/2.f + 0.05f;
			spr->BumpUVUpdate((f32)i/MAXElement/2 + UFactor, 0 +VFactor, (f32)(i+1)/MAXElement/2 + UFactor, 0.4f +VFactor);
			spr->FEffectNum = 3;
		Element.push_back(spr);
	}

	s32 RR = (BladeOwner->Rdegree+DiffR) & 0x0fff;
	PrevRR = RR;

	for( u32 i = 0 ; i < MAXElement; i++ ) {
		PointX2.push_back( PointX1[i]+BladeWidth*cost[RR] );
		PointY2.push_back( PointY1[i]+BladeWidth*sint[RR] );
		Element[i]->SetVertex(0,PointX1[0],PointY1[0]);
		Element[i]->SetVertex(1,PointX1[0],PointY1[0]);
		Element[i]->SetVertex(2,PointX2[0],PointY2[0]);
		Element[i]->SetVertex(3,PointX2[0],PointY2[0]);
	}

}

TEnemyBladeTrace2::~TEnemyBladeTrace2()
{
	for( u32 i = 0 ; i < MAXElement; i++ ) {
		SAFE_DELETE( Element[i] );
	}
}

void TEnemyBladeTrace2::Move()
{
	//前フレームの頂点の位置をコピー
	for( u32 i = MAXElement-1 ; i >= DivLevel-1; i-- ) {
		PointX1[i] = PointX1[i-DivLevel+1];
		PointY1[i] = PointY1[i-DivLevel+1];
		PointX2[i] = PointX2[i-DivLevel+1];
		PointY2[i] = PointY2[i-DivLevel+1];

		if( i<=MAXElement-2 ) {
			Element[i]->SetVertex(0,PointX1[i],PointY1[i]);
			Element[i]->SetVertex(1,PointX1[i+1],PointY1[i+1]);
			Element[i]->SetVertex(2,PointX2[i],PointY2[i]);
			Element[i]->SetVertex(3,PointX2[i+1],PointY2[i+1]);
		}
	}

	//先頭の残像の根元の座標を算出
	PointX1[0] = RoundOff(BladeOwner->GetSpr()->FCX);
	PointY1[0] = RoundOff(BladeOwner->GetSpr()->FCY);

	//刀の角度をゲット
	s32 RR = (BladeOwner->Rdegree+DiffR) & 0x0fff;

	//先頭の残像の先端の座標を算出
	PointX2[0] = PointX1[0]+BladeWidth*cost[RR];
	PointY2[0] = PointY1[0]+BladeWidth*sint[RR];

	//先頭の残像をDIVTRACE個に分割
	for( u32 i = (DivLevel -2) ; i >= 1; i-- ) {
		//根元の座標を分割
		PointX1[i] = PointX1[DivLevel-1]+(PointX1[0]-PointX1[DivLevel-1])*(DivLevel-i)/(f32)DivLevel;
		PointY1[i] = PointY1[DivLevel-1]+(PointY1[0]-PointY1[DivLevel-1])*(DivLevel-i)/(f32)DivLevel;

		//角度も分割
		s32 DivRR  =  RoundOff(PrevRR-(GetDiffR(RR, PrevRR)*(DivLevel-i) / (f32)DivLevel)) & 0x0fff;

		//分割された角度を元に先端の座標を算出
		PointX2[i] = PointX1[i]+BladeWidth*cost[DivRR];
		PointY2[i] = PointY1[i]+BladeWidth*sint[DivRR];

		//座標セット
		if( i<=MAXElement-2 ) {
			Element[i]->SetVertex(0,PointX1[i],PointY1[i]);
			Element[i]->SetVertex(1,PointX1[i+1],PointY1[i+1]);
			Element[i]->SetVertex(2,PointX2[i],PointY2[i]);
			Element[i]->SetVertex(3,PointX2[i+1],PointY2[i+1]);
		}
	}

	//前フレームの刀の角度として保存
	PrevRR  =  RR;

	//先頭スプライトの頂点セット
	Element[0]->SetVertex(0,PointX1[0],PointY1[0]);
	Element[0]->SetVertex(1,PointX1[1],PointY1[1]);
	Element[0]->SetVertex(2,PointX2[0],PointY2[0]);
	Element[0]->SetVertex(3,PointX2[1],PointY2[1]);

	if( (Age > 0) && (Age < (MAXElement / DivLevel-1)) ) {
		for( u32 i = 0 ; i < MAXElement; i++ ) {
			Element[i]->AnimNext();
		}
	}

	X = RoundOff(BladeOwner->GetSpr()->FCX);
	Y = RoundOff(BladeOwner->GetSpr()->FCY);
	for( u32 i = 0 ; i < MAXElement; i++ ) {
		//環境バンプマップ作成のため、軌跡の座標を元にUVを作成する
		f32 UFactor  =  (X-256)/640.f;
		f32 VFactor  =  Y/480.f/2.f + 0.05f;
		Element[i]->BumpUVUpdate((f32)i/MAXElement/4 + UFactor, 0 +VFactor, (f32)(i+1)/MAXElement/4 + UFactor, 0.4f +VFactor);
	}

	TOBJ::Move();
}

s32 TEnemyBladeTrace2::OnDraw()
{
	for( u32 i = 0 ; i < MAXElement; i++ )
		FOwner->GetSprMan()->AddSprite(Element[i]);

	return 0;

}

TBoss2BeamTrace::TBoss2BeamTrace( TOBJList* owner, s32 _Width, s32 _Length ) : TOBJ(owner)
{
	FObjectName = "THomingTrace";

	FSTR = 1000;
	DiffColor = 0;
	X = 0;
	Y = 0;

	BeamWidth = _Width;
	FKind = kEShot;

	PointX1.resize(_Length);
	PointY1.resize(_Length);
	PointX2.resize(_Length);
	PointY2.resize(_Length);
	RR.resize(_Length);


	for( u32 i = 0 ; i < _Length; i++ ) {
		PointX1[i] = X;
		PointY1[i] = Y;
		MakeSpriteLite(i, X,Y,BeamWidth,BeamWidth,15100+i,bmAdd);
		Sprite[i]->GetSpr()->AddTexture(Eff8Tex, 129 +(s32)(i*128.f/_Length), 1, 129 + (s32)((i+1)*128.f/_Length), 127);
		Sprite[i]->GetSpr()->FVisible = true;
		Sprite[i]->GetSpr()->AddHitArea(TPoint(0, 0), TPoint(0, BeamWidth*2), TPoint(BeamWidth, BeamWidth*2),TPoint(BeamWidth, 0), htSky);
		Sprite[i]->GetSpr()->FHit        =  false;
		Sprite[i]->GetSpr()->SetColor(0,ctAlpha);
	}
}

void TBoss2BeamTrace::Move()
{
	TOBJ::Move();
	for( u32 i = 0 ; i < numSpr; i++ ) {
		if( DiffColor!=0 ) {
			Sprite[i]->GetSpr()->SetColorR(DiffColor, ctAlpha);
			if( Sprite[i]->GetSpr()->GetColor(ctAlpha) == 255 )
				Sprite[i]->GetSpr()->FHit = true;
			else
				Sprite[i]->GetSpr()->FHit = false;
		}
	}
}

void TBoss2BeamTrace::SetOwnerPos( s32 _x, s32 _y, s32 _r )
{
	//前フレームの頂点の位置をコピー
	for( u32 i = numSpr-1 ; i >= 1; i-- ) {
		PointX1[i] = PointX1[i-1];
		PointY1[i] = PointY1[i-1];
		PointX2[i] = PointX2[i-1];
		PointY2[i] = PointY2[i-1];
		RR[i] = RR[i-1];

		if( i<=numSpr-2 ) {
			Sprite[i]->Rotate(RR[i]);
			Sprite[i]->GetSpr()->SetVertex(0,PointX1[i],PointY1[i]);
			Sprite[i]->GetSpr()->SetVertex(1,PointX1[i+1],PointY1[i+1]);
			Sprite[i]->GetSpr()->SetVertex(2,PointX2[i],PointY2[i]);
			Sprite[i]->GetSpr()->SetVertex(3,PointX2[i+1],PointY2[i+1]);
		}
	}

	//先頭の残像の根元の座標を算出
	PointX1[0] = _x;
	PointY1[0] = _y;

	//オーナーの角度をゲット
	RR[0] = (_r + 1024) & 0x0fff;

	//先頭の残像の先端の座標を算出
	PointX2[0] = PointX1[0]+BeamWidth*cost[RR[0]];
	PointY2[0] = PointY1[0]+BeamWidth*sint[RR[0]];

	//先頭スプライトの頂点セット
	Sprite[0]->GetSpr()->SetVertex(0,PointX1[0],PointY1[0]);
	Sprite[0]->GetSpr()->SetVertex(1,PointX1[1],PointY1[1]);
	Sprite[0]->GetSpr()->SetVertex(2,PointX2[0],PointY2[0]);
	Sprite[0]->GetSpr()->SetVertex(3,PointX2[1],PointY2[1]);

}

void TBoss2BeamTrace::Reset( s32 _x, s32 _y, s32 _r )
{
	for( u32 i = 0 ; i < numSpr; i++ ) {
		RR[i] = 0;
		PointX1[i] = _x;
		PointY1[i] = _y;
		PointX2[i] = PointX1[i]+BeamWidth*cost[_r];
		PointY2[i] = PointY1[i]+BeamWidth*sint[_r];
		Sprite[i]->GetSpr()->SetVertex(0,PointX1[0],PointY1[0]);
		Sprite[i]->GetSpr()->SetVertex(1,PointX1[0],PointY1[0]);
		Sprite[i]->GetSpr()->SetVertex(2,PointX2[0],PointY2[0]);
		Sprite[i]->GetSpr()->SetVertex(3,PointX2[0],PointY2[0]);
		Sprite[i]->Rotate(RR[0]);
	}

}

void TBoss2BeamTrace::Show( bool _show )
{
	if( _show )
		DiffColor  =  4;
	else
		DiffColor  =  -16;

}

void TBoss2BeamTrace::Slide( s32 _x, s32 _y )
{
	for( u32 i  =  0 ; i < numSpr; i++ )    
		Sprite[i]->MoveR(_x, _y);

}

TPolyLine2D::TPolyLine2D( TOBJList* owner, s32 _segment, s32 _width, TDGTexture* _tex, Rect& _uv, TBlendMode _blend ) : TOBJ(owner)
{
	FObjectName = "TPolyLine2D";
	Segment = _segment;
	Points.resize(Segment+1);
	Vertex1.resize(Segment+1);
	Vertex2.resize(Segment+1);
	Element.resize(Segment);
	Width = _width;
	IsUpdated = false;
	uv  =  _uv;
	ZoomRate = 1;
	FKind = kEffect2;

	s32 texwidth  =  _uv.Right - _uv.Left;
	for( u32 i = 0 ; i < Segment; i++ ) {
			TSpriteEXBLT sblt;
			sblt.RBM      =  _blend; // αブレンドモード
			sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
			sblt.RDDDD    =  QD;     // TDDDDの参照
			sblt.RHeight  =  Width;        // 高さ
			sblt.RWidth   =  16;        //  幅
			sblt.RX       =  -100;        // X座標
			sblt.RY       =  0;        // Y座標
			sblt.RZ       =  6000+i;       // Z値

		Element[i] =  new TSpriteEX(sblt);         // スプライトの生成
		Element[i]->AddTexture(_tex, _uv.Left + (s32)((f32)i*texwidth/Segment), _uv.Top, _uv.Left + (s32)((f32)(i+1)*texwidth/Segment), _uv.Bottom);
		Element[i]->FVisible = true;
	}

}

TPolyLine2D::~TPolyLine2D()
{
	for( u32 i = 0 ; i < Segment; i++ ) {
		SAFE_DELETE(Element[i]);
	}
}

void TPolyLine2D::SetPoint( s32 _num, glm::vec2& _vec )
{
	if( _num<Points.size() ) {
		Points[_num]  =  _vec;
		IsUpdated = true;
	}
}

void TPolyLine2D::CalcVartices()
{
	glm::vec2 tan;
	glm::vec2 cross;
	for( u32 i = 0 ; i < Points.size(); i++ ) {
		// 点の単位接線ベクトルを求める
		if( i == 0 )
			tan  =  Points[i + 1] - Points[i];
		else if( i == (Points.size() - 1) )
			tan  =  Points[i] - Points[i - 1];
		else
			tan  =  Points[i + 1] - Points[i - 1];

		cross  =  glm::normalize(Rot2D(tan , 1024));

		Vertex1[i]  =  Points[i] + cross * (f32)Width;
		Vertex2[i]  =  Points[i] - cross * (f32)Width;
	}

	for( u32 i = 0 ; i < Segment; i++ ) {
		Element[i]->SetVertex(0,Vertex1[i].x,Vertex1[i].y);
		Element[i]->SetVertex(1,Vertex1[i+1].x,Vertex1[i+1].y);
		Element[i]->SetVertex(2,Vertex2[i].x,Vertex2[i].y);
		Element[i]->SetVertex(3,Vertex2[i+1].x,Vertex2[i+1].y);
	}
}

void TPolyLine2D::SetZoomrate( f32 _zoom )
{

}

void TPolyLine2D::SetWidth( s32 _w )
{
	Width  =  _w;

}

void TPolyLine2D::Move()
{
	TOBJ::Move();
	if( IsUpdated ) {
		CalcVartices();
		IsUpdated = false;
	}

}

s32 TPolyLine2D::OnDraw()
{
	for( u32 i = 0 ; i < Segment; i++ )
		FOwner->GetSprMan()->AddSprite(Element[i]);

	return 0;

}

void TPolyLine2D::SetAlpha( s32 _a )
{
	for( u32 i = 0 ; i < Segment; i++ ) {
		Element[i]->SetColor(_a, ctAlpha);
	}

}

void TPolyLine2D::SetAlphaR( s32 _a )
{
	for( u32 i = 0 ; i < Segment; i++ ) {
		Element[i]->SetColorR(_a, ctAlpha);
	}

}

void TPolyLine2D::SetPointR( s32 _num, glm::vec2& _vec )
{
	if( _num < Points.size() ) {
		Points[_num]  =  Points[_num] + _vec;
		IsUpdated = true;
	}

}

void TPolyLine2D::Show( s32 _num, bool _bool )
{
	if( _num< Element.size() ) {
		Element[_num]->FVisible = _bool;
	}

}

TCollisionSpr::TCollisionSpr( TOBJList* owner, TOBJ* _parent, s32 _x, s32 _y, s32 w, s32 h ) : TOBJ(owner)
{
	FObjectName = "TCollisionSpr";

	X = _x;
	Y = _y;
	FKind = kEShot;
	Parent = _parent;
	FSTR  =  Parent->STR();
	FIsParentExists  =  true;

	MakeSpriteLite(0, _x,_y,w,h,5000,bmNormal);
		Sprite[0]->GetSpr()->AddTexture(Eff8Tex, 1, 1, 2, 2);
		Sprite[0]->GetSpr()->AddHitArea(TPoint(0, 0), TPoint(0, h), TPoint(w, h),TPoint(w, 0), htSky);
		Sprite[0]->GetSpr()->FHit        =  true;
		Sprite[0]->GetSpr()->FCX += w/2.f;
		Sprite[0]->GetSpr()->FCY += h/2.f;
		Sprite[0]->GetSpr()->Move(_x,_y);

}

void TCollisionSpr::Move()
{
	TOBJ::Move();

}

void TCollisionSpr::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);
	Parent->OnDamaged(target);

}

void TLightning::CalcPos()
{
	f32 SX = X;
	f32 SY = Y;
	s32 Rdeg = 2048;

	glm::vec2 finalpos(SX, SY);
	s32 steering = 16;
	pl->SetPoint(0, finalpos);
	Points[0]  =  finalpos;
	for( u32 i  =  1 ; i < Points.size(); i++ ) {
		f32 RX = DestX-finalpos.x;             //自分と敵との距離を計算
		f32 RY = DestY-finalpos.y;
		s32 EnemyDeg = RoundOff(atan2(RY,RX)/PI*2048.f) & 0x0fff;

		s32 Rdiff = GetDiffR( EnemyDeg, Rdeg);

		if( abs(Rdiff)>=2048 ) Inc(Rdeg, (s32)(-Sign(Rdiff)*ceil((f32)(4095-abs(Rdiff))/steering)-Sign(Rdiff)));
		else if( abs(Rdiff)<2048 ) Dec(Rdeg, (s32)ceil((f32)(Rdiff)/steering)+Sign(Rdiff));

		glm::vec2 velocity(10, 0);
		velocity  =  Rot2D(velocity, Rdeg);

		finalpos = finalpos + velocity;

		if( (i % 4 == 0) && (i<Collision.size()) ) {
			Collision[i]->Sprite[0]->Move(finalpos.x, finalpos.y);
		}
		else if( (i<Collision.size()) )
			Collision[i]->Sprite[0]->GetSpr()->FHit = false;   

		Points[i]  =  finalpos;
		pl->SetPoint(i, finalpos);
	}

}

TLightning::TLightning( TOBJList* owner, s32 _x, s32 _y, s32 dx, s32 dy ) : TOBJ(owner)
{
	FObjectName = "TLightning";
	Points.resize(64);
	Collision.resize(Points.size());
	RandVel.resize(Points.size());
	FKind = kEffect2;
	FSTR = 2000;
	X = _x;
	Y = _y;
	DestX = dx;
	DestY = dy;
	LastShown = -1;

	Rect uv(224,432,512,512);
	pl = new TPolyLine2D(FOwner, Points.size()-1, 64, MechaTex, uv, bmAdd);
	FOwner->Add(pl);

	for( u32 i  =  0 ; i < Collision.size()-1; i++ ) {
		Collision[i] = new TCollisionSpr(FOwner, this, X,Y,48,48);
		FOwner->Add(Collision[i]);
	}

	//先端の光
	MakeSpriteLite(0, X,Y,80,80,12000,bmAdd);
	Sprite[0]->GetSpr()->AddTexture(MechaTex, 288 , 256, 368, 336);
	Sprite[0]->GetSpr()->FVisible = true;
	Sprite[0]->GetSpr()->FCX += 40;
	Sprite[0]->GetSpr()->FCY += 40;
	Sprite[0]->GetSpr()->Zoom(5,1);

	for( u32 i  =  0 ; i < RandVel.size(); i++ ) {
		RandVel[i] = Random2D(0.1f);
	}

	CalcPos();

}

TLightning::~TLightning()
{

}

void TLightning::Move()
{
	TOBJ::Move();
	if( Age == 1 ) {
		for( u32 i  =  0 ; i < Points.size(); i++ ) {
			pl->Show(i, false);
		}
	}

	if( LastShown < Points.size()-1 ) {
		for( u32 i = 0; i <= 7; i++ ) {
			Inc(LastShown);
			pl->Show(LastShown, true);
		}
		Sprite[0]->Move(Points[LastShown].x, Points[LastShown].y);
	}
	else {
		pl->SetAlphaR(-4);
		if( Sprite[0]->GetSpr()->FVisible ) Sprite[0]->GetSpr()->FVisible = false;
	}

	for( u32 i  =  0 ; i < Collision.size(); i++ ) {
		Collision[i]->Sprite[0]->MoveR(FOwner->Speed - KSCROLLSPEED_NORMAL,0);
	}

	if( Age>60 ) Die();


}

void TLightning::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);

}

void TLightning::Die()
{
	pl->Die();

	for( u32 i  =  0 ; i < Collision.size(); i++ ) {
		Collision[i]->Die();
	}

	TOBJ::Die();

}

TThunderSword::TThunderSword( TOBJList* owner, s32 _x, s32 _y ) : TOBJ(owner)
{
	FObjectName = "TThunderSword";

	X = _x;
	Y = _y;
	FKind = kEShot;
	FSTR  =  2000;
	Buzzed = false;
	LastBuzz = 0;

	MakeSpriteLite(0, _x,_y,224,64,12000,bmAdd);
		Sprite[0]->GetSpr()->AddTexture(MechaTex, 96 , 192, 320, 256);
		Sprite[0]->GetSpr()->AddHitArea(TPoint(80, 16), TPoint(80, 48), TPoint(224, 48),TPoint(224, 16), htSky);
		Sprite[0]->GetSpr()->FHit        =  true;
		Sprite[0]->GetSpr()->FCX += 200;
		Sprite[0]->GetSpr()->FCY += 24;
		Sprite[0]->GetSpr()->Move(_x,_y);
		if( FOwner->Difficulty == KDIFFICULTY_HARD )
			Sprite[0]->GetSpr()->Zoom(4, 1);

}

void TThunderSword::Move()
{
	TOBJ::Move();
	if( Age % 5 == 0 ) FOwner->Add(new TThunderSwordPart(FOwner, X, Y));

	if( (Age-LastBuzz)>=40 ) Buzzed = false;

	if( Age>60 ) Die();

	Sprite[0]->MoveR(FOwner->Speed-KSCROLLSPEED_NORMAL,0);


}

void TThunderSword::OnDamaged( TOBJ* target )
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

TThunderSwordPart::TThunderSwordPart( TOBJList* owner, s32 _x, s32 _y ) : TOBJ(owner)
{
	FObjectName = "TThunderSwordPart";

	X = _x;
	Y = _y;
	FKind = kEffect2;

	MakeSpriteLite(0, _x,_y,224,64,12000,bmAdd);
		Sprite[0]->GetSpr()->AddTexture(MechaTex, 96 , 192, 320, 256);
		Sprite[0]->GetSpr()->FCX += 200;
		Sprite[0]->GetSpr()->FCY += 24;
		Sprite[0]->GetSpr()->Move(_x,_y);
		if( FOwner->Difficulty == KDIFFICULTY_HARD )
			Sprite[0]->GetSpr()->Zoom(1.5f, 4.f);
		else
			Sprite[0]->GetSpr()->Zoom(0.5f, 4.f);

}

void TThunderSwordPart::Move()
{
	TOBJ::Move();
	Sprite[0]->ZoomR(1.2f, 0.9f);

	if( Sprite[0]->GetSpr()->FZX>2 ) Die();

}

TBitLight::TBitLight( TOBJList* owner, TOBJ* _parent ) : TOBJ(owner)
{
	FObjectName = "TBitLight";
	Parent = _parent;
	Visible = false;

	FKind = kEffect2;

	for( u32 i = 0; i <= 1; i++ ) {
		MakeSpriteLite(i, 0,0,64,64,22000,bmAdd);
			Sprite[0]->GetSpr()->AddTexture(MechaTex, 0 , 176, 80, 256);
			Sprite[0]->GetSpr()->FVisible = true;
			Sprite[0]->GetSpr()->FCX += 32;
			Sprite[0]->GetSpr()->FCY += 32;
			Sprite[0]->GetSpr()->SetColor(128,ctAlpha);
			Sprite[0]->GetSpr()->Zoom(2,2);
			Sprite[0]->GetSpr()->FVisible = false;
	}

	MakeSpriteLite(numSpr, 0,0,64,64,22000,bmAdd);
	Sprite[numSpr-1]->GetSpr()->AddTexture(MechaTex, 0 , 176, 80, 256);
	Sprite[numSpr-1]->GetSpr()->FCX += 32;
	Sprite[numSpr-1]->GetSpr()->FCY += 32;
	Sprite[numSpr-1]->GetSpr()->SetColor(128,ctAlpha);
	Sprite[numSpr-1]->GetSpr()->Zoom(2,2);
	Sprite[numSpr-1]->GetSpr()->FVisible = false;
}

void TBitLight::Move()
{
	TOBJ::Move();
	for( u32 i  =  0 ; i < numSpr; i++ ) {
		Sprite[i]->Move(Parent->X, Parent->Y);
		if( Visible && (Sprite[i]->GetSpr()->GetColor(ctAlpha)<128) )
			Sprite[i]->GetSpr()->SetColorR(4,ctAlpha);
	}
	Sprite[0]->RotateR(32);
	Sprite[1]->RotateR(-48);

}

void TBitLight::Show()
{
	Visible = true;
	for( u32 i  =  0 ; i < numSpr; i++ ) {
		Sprite[i]->GetSpr()->SetColor(0,ctAlpha);
		Sprite[i]->GetSpr()->FVisible = true;
	}

}

void TSpreadLaser::CalcPos()
{
	s32 posx = Parent->X + X;
	s32 posy = Parent->Y + Y;
	for( u32 i  =  0 ; i <= pl->GetSize(); i++ ) {
		f32 beamX  =  posx - i*10.f;
		f32 beamY  =  posy + pow(i*10.f, 0.5f)*Width*0.1f;
		pl->SetPoint(i, glm::vec2(beamX, beamY));

		if( (i<Collision.size()) ) {
			Collision[i]->Sprite[0]->Move(RoundOff(beamX), RoundOff(beamY));
			if( (Alpha == 255) )
				Collision[i]->Sprite[0]->GetSpr()->FHit = true;
			else
				Collision[i]->Sprite[0]->GetSpr()->FHit = false;
		}
	}

}

TSpreadLaser::TSpreadLaser( TOBJList* owner, TOBJ* _parent, s32 _x, s32 _y, s32 _width, s32 _life ) : TOBJ(owner)
{
	FObjectName = "TSpreadLaser";
	Points.resize(64);
	Collision.resize(Points.size());
	FKind = kEffect2;
	FSTR = 1000;
	X = _x;
	Y = _y;
	Parent = _parent;
	Width = _width;
	Alpha = 0;
	Life = _life;

	Rect uv(24, 145, 48, 208);
	pl = new TPolyLine2D(FOwner, Points.size()-1, 16, MechaTex, uv, bmAdd);
	FOwner->Add(pl);
	pl->SetAlpha(0);

	for( u32 i  =  0 ; i < Collision.size(); i++ ) {
		Collision[i] = new TCollisionSpr(FOwner, this, X,Y,16,16);
		FOwner->Add(Collision[i]);
	}
}

void TSpreadLaser::Move()
{
	TOBJ::Move();

	if( Alpha>0 )
		CalcPos();

	if( Age<=Life ) {
		pl->SetAlphaR(8);
		Inc(Alpha,8);
		if( Alpha>255 ) Alpha = 255;
	}
	else {
		pl->SetAlphaR(-8);
		Inc(Alpha,-8);
		if( Alpha<0 ) Die();
	}

	for( u32 i  =  0 ; i < Collision.size(); i++ ) {
		Collision[i]->Sprite[0]->MoveR(FOwner->Speed - KSCROLLSPEED_NORMAL,0);
	}
}

void TSpreadLaser::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);

}

void TSpreadLaser::Die()
{
	pl->Die();

	for( u32 i  =  0 ; i < Collision.size(); i++ ) {
		Collision[i]->Die();
	}
	TOBJ::Die();
}
