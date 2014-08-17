//------------------------------------------------------------------------------
// �t�@�C����	: SpriteEXU.pas
// ����			: SpriteEX�̃w���p�[���j�b�g
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

// �^�̒�`
	enum TBlendMode {
		bmNone,
		bmNormal,
		bmSubAdd, 
		bmAdd,
		bmDec ,
		bmMax, 
		bmMul
	}; 	// �u�����h���[�h

	enum TColorType	{
		ctAlpha,
		ctRed,
		ctGreen, 
		ctBlue
	};    // �F�̎�ށiA,R,G,B�j

	enum THitType {
		htGround,
		htSky,
		htAll
	};	 	// �����蔻��̎��

	// ���������_���W
	struct TSPoint {
		f32 x;
		f32 y;
	};

	// �e�N�X�`���A�j���[�V�����p�\����
	struct TTexAnim {
		TDGTexture* 	RTex;		// �e�N�X�`���ւ̃|�C���^
		f32 RU1;           // ����UV���W
		f32 RV1;           //     �V
		f32 RU2;           // �E��UV���W
		f32 RV2;           //     �V
	};

	// �e�N�X�`���A�j���z��
//	TTexAnimAry		= array of TTexAnim;
//	PTexAnimAry		= ^TTexAnimAry;

	// �����蔻��z��p�\����
	struct THitArea {
		TSPoint RPointAry[4];
		THitType RType;
	};
//	PHitAreaAry		= ^THitAreaAry;
//	THitAreaAry		= array of THitArea;

	// TSpriteEX�����\����
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

// ��\�I�Ȋp�x���`�i4096�x�n�j
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

//�w���p�[���[�`��
void exRotate( f32* pzx, f32* pzy, const f32 ox, const f32 oy, f32 fx, f32 fy, s16 rad);
																// ���W��]
f32 PTPRad(const f32 fx, const f32 fy, const f32 tx, const f32 ty);  		// ���W������W�ւ̊p�x���Z�o
f32 PTPDistance(const f32 fx, const f32 fy, const f32 tx, const f32 ty);	// 2�_�Ԃ̋��������߂�
bool NearNum(const f32 _n1, const f32 _n2 );   			// ���������_�̔�r(�ߎ��l�Ȃ�True)
void SpriteEXInitialize();                          	 		// SpriteEX�g�p���\�[�X�̏�����
TSPoint SPoint(const f32 _x, const f32 _y );		 		// TSPoint�^�����
u16 RadToDeg4096(const f32 _rad );     			// ���W�A����4096�x�n�ϊ�


// SpriteEX���������p���[�`��
D3DTLVERTEX2 MakeTLV(f32 _x, f32 _y, f32 _z, f32 _rhw, u32 _col, u32 _sp,
	f32 _tu, f32 _tv , f32 _bu, f32 _bv);     				// D3DTLVERTEX�֒l���܂Ƃ߂ē����
bool CheckLineCross( const TSPoint* _pt );
																// ������������
bool CheckLineCross(const TSPoint& _a, const TSPoint& _b, const TSPoint& _c, const TSPoint& _d );
																// ������������
bool PointInArea(const TSPoint* _pgn, const TSPoint& _pt);
																// �_�̗̈������
bool PointInArea(const Rect& _rt, const glm::vec2& _pt );
bool CheckRectCross(const Rect& _rt1, const Rect& _rt2 );	// 2��`�̐ڐG����
void exZoom(f32* _ax, f32* _ay, f32 _ox, f32 _oy, f32 _fx, f32 _fy, f32 _zx, f32 _zy);
void exWriteDebugText(string const _txt); 				// �f�o�b�O�e�L�X�g�̏o��
s32 exShowModalMessage(string const );   	// ���b�Z�[�W�{�b�N�X�̕\���i���[�_���j
u32 SingleToDW( f32 f);
u32 MakeCol(u32 _a, u32 _r, u32 _g, u32 _b);
