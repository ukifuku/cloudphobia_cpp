#pragma once

#ifndef _MARS_TYPES_H_
#define _MARS_TYPES_H_

#define SAFE_RELEASE(x)  { if(x) { (x)->Release(); (x)=NULL; } }	// âï˙É}ÉNÉç
#define SAFE_DELETE(x)  if( x != NULL ){ delete x;  x = NULL; }
#define SAFE_DELETE_ARRAY(x)  if( x != NULL ){ delete[] x;  x = NULL; }
#define SAFE_FREE(x)  if( x != NULL ){ free( x );  x = NULL; }
#define SAFE_DELETE_VECTOR(x)  { for( u32 iii=0; iii<x.size(); iii++ ){ if( x[iii] != NULL ){ delete x[iii]; } } x.clear(); }

typedef char			s8;
typedef unsigned char	u8;

typedef short			s16;
typedef unsigned short	u16;

typedef int				s32;
typedef unsigned int	u32;

typedef __int64			s64;
typedef unsigned __int64	u64;

typedef bool			b8;

typedef char			c8;

typedef float			f32;
typedef double			f64;

template<typename T>
void Inc(T x) { x++; }

template<typename T>
void Dec(T x) { x--; }

template<typename T>
void Inc(T x, T y) { x += y; }

template<typename T>
void Dec(T x, T y) { x -= y; }

struct Rect
{
	s32 Left;
	s32 Top;
	s32 Right;
	s32 Bottom;
	Rect(){}
	Rect( s32 _l, s32 _t, s32 _r, s32 _b ){
		Left = _l;
		Top = _t;
		Right = _r;
		Bottom = _b;
	}
};

struct TPoint
{
	s32 X;
	s32 Y;

	TPoint(){}
	TPoint(s32 _x, s32 _y){ X = _x; Y = _y;}
};

#endif	//_MARS_TYPES_H_