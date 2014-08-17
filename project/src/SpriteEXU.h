//------------------------------------------------------------------------------
// ファイル名	: SpriteEXU.pas
// 説明			: SpriteEXのヘルパーユニット
//------------------------------------------------------------------------------
#pragma once

#include "types.h"
#include <string>
#include <sstream>
#include <d3d9.h>
#include <d3d9types.h>
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "DGCarad9.h"
#include "SXLib9.h"

// 型の定義
	enum TBlendMode {
		bmNone,
		bmNormal,
		bmSubAdd, 
		bmAdd,
		bmDec ,
		bmMax, 
		bmMul
	}; 	// ブレンドモード

	enum TColorType	{
		ctAlpha,
		ctRed,
		ctGreen, 
		ctBlue
	};    // 色の種類（A,R,G,B）

	enum THitType {
		htGround,
		htSky,
		htAll
	};	 	// 当たり判定の種別

	// 浮動小数点座標
	struct TSPoint {
		f32 x;
		f32 y;
	};

	// テクスチャアニメーション用構造体
	struct TTexAnim {
		TDGTexture* 	RTex;		// テクスチャへのポインタ
		f32 RU1;           // 左上UV座標
		f32 RV1;           //     〃
		f32 RU2;           // 右下UV座標
		f32 RV2;           //     〃
	};

	// テクスチャアニメ配列
//	TTexAnimAry		= array of TTexAnim;
//	PTexAnimAry		= ^TTexAnimAry;

	// 当たり判定配列用構造体
	struct THitArea {
		TSPoint RPointAry[4];
		THitType RType;
	};
//	PHitAreaAry		= ^THitAreaAry;
//	THitAreaAry		= array of THitArea;

	// TSpriteEX生成構造体
//	PSpriteEXBLT 	= ^TSpriteEXBLT;
	struct TSpriteEXBLT {
		TDGCarad* 	RDDDD;
		s16		RX;
		s16 		RY;
		u16		RWidth;
		u16		RHeight;
		u32		RColor;
		TBlendMode	RBM;
		u16		RZ;
	};

// 代表的な角度を定義（4096度系）
#define	PI_ZERO			 0
#define	PI_DIV_2		 1024
#define	PI_ONE			 2048
#define	PI_3_DIV_2               3072
#define	PI_MUL_2		 4095
#define	PI_DIV_4		 ((PI_ZERO + PI_DIV_2) >> 1)
#define	PI_3_DIV_4		 ((PI_DIV_2 + PI_ONE) >> 1)
#define	PI_5_DIV_4		 ((PI_ONE + PI_3_DIV_2) >> 1)
#define	PI_7_DIV_4		 ((PI_3_DIV_2 + PI_MUL_2) >> 1)

//FVF
const u32 SprFVF = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX2);

//ヘルパールーチン
void exRotate( f32* pzx, f32* pzy, const f32 ox, const f32 oy, f32 fx, f32 fy, s16 rad);
																// 座標回転
f32 PTPRad(const f32 fx, const f32 fy, const f32 tx, const f32 ty);  		// 座標から座標への角度を算出
f32 PTPDistance(const f32 fx, const f32 fy, const f32 tx, const f32 ty);	// 2点間の距離を求める
bool NearNum(const f32 _n1, const f32 _n2 );   			// 浮動小数点の比較(近似値ならTrue)
void SpriteEXInitialize();                          	 		// SpriteEX使用リソースの初期化
TSPoint SPoint(const f32 _x, const f32 _y );		 		// TSPoint型を作る
u16 RadToDeg4096(const f32 _rad );     			// ラジアン→4096度系変換


// SpriteEX内部処理用ルーチン
D3DTLVERTEX2 MakeTLV(f32 _x, f32 _y, f32 _z, f32 _rhw, u32 _col, u32 _sp,
	f32 _tu, f32 _tv , f32 _bu, f32 _bv);     				// D3DTLVERTEXへ値をまとめて入れる
bool CheckLineCross( const TSPoint* _pt );
																// 線分交差判定
bool CheckLineCross(const TSPoint& _a, const TSPoint& _b, const TSPoint& _c, const TSPoint& _d );
																// 線分交差判定
bool PointInArea(const TSPoint* _pgn, const TSPoint& _pt);
																// 点の領域内判定
bool PointInArea(const Rect& _rt, const glm::vec2& _pt );
bool CheckRectCross(const Rect& _rt1, const Rect& _rt2 );	// 2矩形の接触判定
void exZoom(f32* _ax, f32* _ay, f32 _ox, f32 _oy, f32 _fx, f32 _fy, f32 _zx, f32 _zy);
void exWriteDebugText(string const _txt); 				// デバッグテキストの出力
s32 exShowModalMessage(string const );   	// メッセージボックスの表示（モーダル）
u32 SingleToDW( f32 f);
u32 MakeCol(u32 _a, u32 _r, u32 _g, u32 _b);
