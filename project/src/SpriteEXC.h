#pragma once

#include "SpriteEX.h"

// �X�v���C�g�R���o�C�i�[�N���X
class TSpriteEXCombiner : public TSpriteBase
{
protected:
	f32 FX;     		// ���W�i���[���h���W����j
	f32 FY;             //           �V
	vector<TSpriteBase*> FSprList;                // �X�v���C�g���X�g
	void	SetFColor(const D3DCOLOR _col );   	// �J���[�l�̐ݒ�i���l�j
	TDGTexture*	GetFTex(); 
	void	SetFBM(TBlendMode _bm);       // �u�����h���[�h�̐ݒ�
public:

	TSpriteEXCombiner();      			   			// �R���X�g���N�^
	~TSpriteEXCombiner();                   // �f�X�g���N�^
	void	AddSprite(TSpriteEX* _spr);        // �X�v���C�g��o�^
	void	SetColor(u8 _val, TColorType _type ); // �J���[�l�̐ݒ�iA,R,G,B�w��j
	void 	MoveR(s16 _x, s16 _y);           // ���s�ړ��i���Ύw��j
	void 	MoveR(f32 _x, f32 _y);	
	void 	Move(f32 _x, f32 _y );            // ���s�ړ��i��Ύw��j
	void 	SlideMoveR(s16 _x, s16 _y );       // �X���C�h�ړ��i���Ύw��j
	void 	SlideMoveR(f32 _x, f32 _y);       // �X���C�h�ړ��i���Ύw��j
	void 	RotateR(s16 _r );             // ��]�i���Ύw��j
	void	Rotate(s16  _r);              // ��]�i��Ύw��j
	void	ZoomR(f32 _zx, f32 _zy ); 			// �g�k�i���Ύw��j
	void 	Zoom(f32 _zx, f32 _zy );            // �g�k�i��Ύw��j
	bool	CollisionCheck(TSpriteBase* _spr); // �����蔻����Ƃ�
	void ChangeZLevel(s16 _z );       // Z�l�̕ύX
	void 	SetFVisible(bool _flg );   // ���t���O�̐ݒ�
	vector<TSpriteBase*>& GetList() {return FSprList;}
	TSpriteBase*	GetItem(u16 idx) { return idx < FSprList.size() ? FSprList[idx] : NULL; }
};

