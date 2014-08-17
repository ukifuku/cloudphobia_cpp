#include "SpriteEXC.h"

void TSpriteEXCombiner::SetFColor( const D3DCOLOR _col )
{
	// �S�Ă̓o�^�σX�v���C�g�ɂ��ď���
	for( u32 i  =  0 ; i < FSprList.size(); i++ )
		((TSpriteEX*)FSprList[i])->FColor  =  _col;

}

TDGTexture* TSpriteEXCombiner::GetFTex()
{
	return  ((TSpriteEX*)FSprList[0])->GetFTex();
}

void TSpriteEXCombiner::SetFBM( TBlendMode _bm )
{
	// �S�Ă̓o�^�σX�v���C�g�ɂ��ď���
	for( u32 i  =  0 ; i < FSprList.size(); i++ )
		((TSpriteEX*)FSprList[i])->FBM  =  _bm;

}

TSpriteEXCombiner::TSpriteEXCombiner()
{
	FR  =  PI_ZERO;
	FColor  =  0xFFFFFFFF;
	FZX  =  1.0f;
	FZY  =  1.0f;
	FSprList.clear();

}

TSpriteEXCombiner::~TSpriteEXCombiner()
{
	FSprList.clear();
}

void TSpriteEXCombiner::AddSprite( TSpriteEX* _spr )
{
	FSprList.push_back(_spr);

}

void TSpriteEXCombiner::SetColor( u8 _val, TColorType _type )
{
	// �S�Ă̓o�^�σX�v���C�g�ɂ��ď���
	for( u32 i  =  0 ; i < FSprList.size(); i++ )
		((TSpriteEX*)FSprList[i])->SetColor(_val, _type);

}

void TSpriteEXCombiner::MoveR( s16 _x, s16 _y )
{
	// �S�Ă̓o�^�σX�v���C�g�ɂ��ď���
	for( u32 i  =  0 ; i < FSprList.size(); i++ )
		((TSpriteEX*)FSprList[i])->MoveR(_x, _y);

	// ���W�̍X�V
	FX  =  FX + _x;
	FY  =  FY + _y;
	FCX  =  FCX + _x;
	FCY  =  FCY + _y;

}

void TSpriteEXCombiner::MoveR( f32 _x, f32 _y )
{
	// �S�Ă̓o�^�σX�v���C�g�ɂ��ď���
	for( u32 i  =  0 ; i < FSprList.size(); i++ )
		((TSpriteEX*)FSprList[i])->MoveR(_x, _y);

	// ���W�̍X�V
	FX  =  FX + _x;
	FY  =  FY + _y;
	FCX  =  FCX + _x;
	FCY  =  FCY + _y;

}

void TSpriteEXCombiner::Move( f32 _x, f32 _y )
{
	MoveR(_x - FX, _y - FY);

}

void TSpriteEXCombiner::SlideMoveR( s16 _x, s16 _y )
{
	SlideMoveR((f32)_x + 0.0f, (f32)_y + 0.0f);

}

void TSpriteEXCombiner::SlideMoveR( f32 _x, f32 _y )
{
	exRotate(&_x, &_y, 0, 0, _x, _y, FR);
	MoveR(_x, _y);

}

void TSpriteEXCombiner::RotateR( s16 _r )
{
	_r  =  _r & 4095;  		   	// 0..4095�͈͓̔��Ɏ��߂�
	FR += _r;            		// FR�̓��e���X�V
	FR  =  FR & 4095;				// 0..4095�͈͓̔��Ɏ��߂�

	// �X�v���C�g�̉�]�����F
	// �S�Ă̓o�^�σX�v���C�g�ɂ��ď���
	for( u32 i  =  0 ; i < FSprList.size(); i++ ) {
		TSpriteEX* spr = ((TSpriteEX*)FSprList[i]);
		// �X�v���C�g�̑S���_�ɂ��ď���
		for( u32 j  =  0 ; j <= 3; j++ ) {
			// ��]����
			exRotate(&spr->FTLVs[j].sx, &spr->FTLVs[j].sy, FCX, FCY, spr->FTLVs[j].sx, spr->FTLVs[j].sy, _r);
		}
		spr->FR  =  (spr->FR + _r) & 4095;
		exRotate(&spr->FCX, &spr->FCY,
			FCX, FCY, spr->FCX, spr->FCY, _r);

		spr->FCalcedBB  =  false;
	}

}

void TSpriteEXCombiner::Rotate( s16 _r )
{
	RotateR((_r - FR) & 0x0FFF);

}

void TSpriteEXCombiner::ZoomR( f32 _zx, f32 _zy )
{
	// �X�P�[�����O�F
	// �S�Ă̓o�^�σX�v���C�g�ɂ��ď���
	for( u32 i  =  0 ; i< FSprList.size(); i++) {
		TSpriteEX* spr = ((TSpriteEX*)FSprList[i]);
		// �X�P�[�����O
		for( u32 j  =  0 ; j <= 3; j++ ) {
			exZoom(&spr->FTLVs[j].sx, &spr->FTLVs[j].sy, FCX, FCY, spr->FTLVs[j].sx, spr->FTLVs[j].sy, _zx, _zy);
		}
		// ����W�̍X�V
		exZoom(&spr->FCX, &spr->FCY,
			FCX, FCY,
			spr->FCX, spr->FCY,
			_zx, _zy);
		// ZX, ZY�l�̍X�V
		spr->FZX  =  spr->FZX * _zx;
		spr->FZY  =  spr->FZY * _zy;

		spr->FCalcedBB  =  false;
	}

	// �g�嗦�̍X�V
	FZX  =  FZX * _zx;
	FZY  =  FZY * _zy;

}

void TSpriteEXCombiner::Zoom( f32 _zx, f32 _zy )
{
	ZoomR(1 / FZX, 1 / FZY);			// ���̑傫���ɖ߂���
	ZoomR(_zx, _zy);                    // �X�P�[�����O

}

bool TSpriteEXCombiner::CollisionCheck( TSpriteBase* _spr )
{
	bool Result  =  false;
	if( (! _spr->FHit) || (! FHit) ) {
		return false;
	}

	if( _spr->GetClassType() == SPR_CLASS_EX ) {
		// TSpriteEX�������Ƃ��ēn���ꂽ�ꍇ
		// TSpriteEX��TSpriteEXC�̏Փ˔���
		Result  =  ((TSpriteEX*)_spr)->CollisionCheck(this);
	}
	else if( _spr->GetClassType() == SPR_CLASS_COMBINER ) {
		// TSpriteEXCombiner�������Ƃ��ēn���ꂽ�ꍇ
		// TSpriteEXC��TSpriteEXC�̏Փ˔���
		TSpriteEXCombiner* sprc  = (TSpriteEXCombiner*)_spr;
		for( u32 i  =  0 ; i < sprc->FSprList.size(); i++) {
			for( u32 j  =  0 ; j < FSprList.size(); j++ ) {
				if( ((TSpriteEX*)sprc->FSprList[i])->CollisionCheckSPREX((TSpriteEX*)FSprList[j]) ) {
					return true;
				}
			}
		}
	}
	return Result;

}

void TSpriteEXCombiner::ChangeZLevel( s16 _z )
{
	// �S�Ă̓o�^�σX�v���C�g�ɂ��ď���
	for( u32 i  =  0 ; i < FSprList.size(); i++) {
		s16 z  =  (s16)((TSpriteEX*)FSprList[i])->FZ + _z;		// �P���ɑ���
		if( z < 0 )			z  =  0;  			// 0..255�͈͓̔��Ɏ��߂�
		else if( z > 255 )	z  =  255;
		((TSpriteEX*)FSprList[i])->FZ  =  z;             // Z�l�̍X�V
	}

}

void TSpriteEXCombiner::SetFVisible( bool _flg )
{
	// �S�Ă̓o�^�σX�v���C�g�ɂ��ď���
	for( u32 i  =  0 ; i < FSprList.size(); i++) 
		((TSpriteEX*)FSprList[i])->FVisible  =  _flg;

	FVisible  =  _flg;
}

