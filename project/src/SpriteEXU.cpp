#define _USE_MATH_DEFINES
#include <math.h>

#include "SpriteEXU.h"


void exRotate( f32* pzx, f32* pzy, const f32 ox, const f32 oy, f32 fx, f32 fy, s16 rad )
{
	rad  =  rad & 4095;

	// 角度がついてないなら計算の必要なし
	if( rad == PI_ZERO ) {
		*pzx  =  fx;
		*pzy  =  fy;
		return;
	}

	// 点の回転
	fx  =  fx - ox;
	fy  =  fy - oy;
	*pzx  =  fx * cost[rad] - fy * sint[rad];
	*pzy  =  fx * sint[rad] + fy * cost[rad];
	*pzx  =  *pzx + ox;
	*pzy  =  *pzy + oy;

}

f32 PTPRad( const f32 fx, const f32 fy, const f32 tx, const f32 ty )
{
	f32 dx, dy, Rad;
	dx  =  tx - fx;
	dy  =  ty - fy;
	if( NearNum(dx, 0.0f) )
		Rad  =  (f32)M_PI * 0.5f;
	else
		Rad  =  atan(dy / dx);
	if( ((dx <= 0.0f) && (dy <= 0.0f)) || ((dx < 0.0f) && (dy > 0.0f)) )
		Rad  =  Rad + (f32)M_PI;
	return Rad;

}

f32 PTPDistance( const f32 fx, const f32 fy, const f32 tx, const f32 ty )
{
	return sqrt((fx - tx)*(fx - tx) + (fy - ty)*(fy - ty));
}

bool NearNum( const f32 _n1, const f32 _n2 )
{
	return ((_n1 - _n2) > -0.00001f) && ((_n1 - _n2) < 0.00001f);
}

void SpriteEXInitialize()
{

}

TSPoint SPoint( const f32 _x, const f32 _y )
{
	TSPoint result;
	result.x  =  _x;
	result.y  =  _y;

	return result;
}

u16 RadToDeg4096( const f32 _rad )
{
	return RoundOff(_rad * 2048.f / (f32)M_PI) & 0x0fff;

}

D3DTLVERTEX2 MakeTLV( f32 _x, f32 _y, f32 _z, f32 _rhw, u32 _col, u32 _sp, f32 _tu, f32 _tv , f32 _bu, f32 _bv )
{
	D3DTLVERTEX2 result;
	result.sx		 =  _x;
	result.sy		 =  _y;
	result.sz		 =  _z;
	result.rhw 	 =  _rhw;
	result.color	 =  _col;
	result.specular =  _sp;
	result.tu		 =  _tu;
	result.tv		 =  _tv;
	result.bu        =  _bu;
	result.bv        =  _bv;

	return result;
}

bool CheckLineCross( const TSPoint* _pt )
{
	f32 f1  =  (_pt[0].y - _pt[2].y) * (_pt[3].x - _pt[2].x) -
		(_pt[3].y - _pt[2].y) * (_pt[0].x - _pt[2].x);
	f32 f2  =  (_pt[1].y - _pt[2].y) * (_pt[3].x - _pt[2].x) -
		(_pt[3].y - _pt[2].y) * (_pt[1].x - _pt[2].x);
	if( f1 * f2 < 0 ) {
		f1  =  (_pt[2].y - _pt[0].y) * (_pt[1].x - _pt[0].x) -
			(_pt[1].y - _pt[0].y) * (_pt[2].x - _pt[0].x);
		f2  =  (_pt[3].y - _pt[0].y) * (_pt[1].x - _pt[0].x) -
			(_pt[1].y - _pt[0].y) * (_pt[3].x - _pt[0].x);
		return (f1 * f2 < 0);
	}
	else
		return false;

}

bool CheckLineCross( const TSPoint& _a, const TSPoint& _b, const TSPoint& _c, const TSPoint& _d )
{
	f32 f1  =  (_a.y - _c.y) * (_d.x - _c.x) - (_d.y - _c.y) * (_a.x - _c.x);
	f32 f2  =  (_b.y - _c.y) * (_d.x - _c.x) - (_d.y - _c.y) * (_b.x - _c.x);

	if( f1 * f2 < 0 ) {
		f1  =  (_c.y - _a.y) * (_b.x - _a.x) - (_b.y - _a.y) * (_c.x - _a.x);
		f2  =  (_d.y - _a.y) * (_b.x - _a.x) - (_b.y - _a.y) * (_d.x - _a.x);
		if( f1 * f2 < 0 ) 	return true;
		else				return false;
	}
	else
		return false;

}

bool PointInArea( const TSPoint* _pgn, const TSPoint& _pt )
{
	TSPoint v1;		// 四角形の頂点から対象頂点へのベクトル
	TSPoint v2;      // 四角形の辺のベクトル
	for( u32 i  =  0 ; i <= 3; i++ ) {
		v1  =  SPoint(_pt.x - _pgn[i].x, _pt.y - _pgn[i].y);
		v2  =  SPoint(_pgn[(i + 1) & 3].x - _pgn[i].x,
			_pgn[(i + 1) & 3].y - _pgn[i].y);
		if( (v1.x * v2.y - v1.y * v2.x) < 0 ) {  	// 外積をとって領域内チェック
			return false;
		}
	}

	return true;

}

bool PointInArea( const Rect& _rt, const glm::vec2& _pt )
{
	return (_pt.x > _rt.Left) & (_pt.x < _rt.Right) &
		(_pt.y > _rt.Top) & (_pt.y < _rt.Bottom);

}

bool CheckRectCross( const Rect& _rt1, const Rect& _rt2 )
{
	return (_rt1.Right > _rt2.Left) & (_rt1.Left < _rt2.Right) &
		(_rt1.Top < _rt2.Bottom) & (_rt1.Bottom > _rt2.Top);

}

void exZoom( f32* _ax, f32* _ay, f32 _ox, f32 _oy, f32 _fx, f32 _fy, f32 _zx, f32 _zy )
{
	*_ax  =  (_fx - _ox) * _zx + _ox;
	*_ay  =  (_fy - _oy) * _zy + _oy;

}

void exWriteDebugText( string const _txt )
{

}

s32 exShowModalMessage( string const )
{
	//return Application.MessageBox(PChar(_msg), 'SpriteEX メッセージ', MB_OK);
	return 0;
}

u32 SingleToDW( f32 f )
{
	return *((u32*)&f);
}

u32 MakeCol( u32 _a, u32 _r, u32 _g, u32 _b )
{
	return (_a << 24) | (_r << 16) | (_g << 8) | _b;

}
