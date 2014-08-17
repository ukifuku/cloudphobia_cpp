#include "SpriteEXC.h"

void TSpriteEXCombiner::SetFColor( const D3DCOLOR _col )
{
	// 全ての登録済スプライトについて処理
	for( u32 i  =  0 ; i < FSprList.size(); i++ )
		((TSpriteEX*)FSprList[i])->FColor  =  _col;

}

TDGTexture* TSpriteEXCombiner::GetFTex()
{
	return  ((TSpriteEX*)FSprList[0])->GetFTex();
}

void TSpriteEXCombiner::SetFBM( TBlendMode _bm )
{
	// 全ての登録済スプライトについて処理
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
	// 全ての登録済スプライトについて処理
	for( u32 i  =  0 ; i < FSprList.size(); i++ )
		((TSpriteEX*)FSprList[i])->SetColor(_val, _type);

}

void TSpriteEXCombiner::MoveR( s16 _x, s16 _y )
{
	// 全ての登録済スプライトについて処理
	for( u32 i  =  0 ; i < FSprList.size(); i++ )
		((TSpriteEX*)FSprList[i])->MoveR(_x, _y);

	// 座標の更新
	FX  =  FX + _x;
	FY  =  FY + _y;
	FCX  =  FCX + _x;
	FCY  =  FCY + _y;

}

void TSpriteEXCombiner::MoveR( f32 _x, f32 _y )
{
	// 全ての登録済スプライトについて処理
	for( u32 i  =  0 ; i < FSprList.size(); i++ )
		((TSpriteEX*)FSprList[i])->MoveR(_x, _y);

	// 座標の更新
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
	_r  =  _r & 4095;  		   	// 0..4095の範囲内に収める
	FR += _r;            		// FRの内容を更新
	FR  =  FR & 4095;				// 0..4095の範囲内に収める

	// スプライトの回転処理：
	// 全ての登録済スプライトについて処理
	for( u32 i  =  0 ; i < FSprList.size(); i++ ) {
		TSpriteEX* spr = ((TSpriteEX*)FSprList[i]);
		// スプライトの全頂点について処理
		for( u32 j  =  0 ; j <= 3; j++ ) {
			// 回転処理
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
	// スケーリング：
	// 全ての登録済スプライトについて処理
	for( u32 i  =  0 ; i< FSprList.size(); i++) {
		TSpriteEX* spr = ((TSpriteEX*)FSprList[i]);
		// スケーリング
		for( u32 j  =  0 ; j <= 3; j++ ) {
			exZoom(&spr->FTLVs[j].sx, &spr->FTLVs[j].sy, FCX, FCY, spr->FTLVs[j].sx, spr->FTLVs[j].sy, _zx, _zy);
		}
		// 基準座標の更新
		exZoom(&spr->FCX, &spr->FCY,
			FCX, FCY,
			spr->FCX, spr->FCY,
			_zx, _zy);
		// ZX, ZY値の更新
		spr->FZX  =  spr->FZX * _zx;
		spr->FZY  =  spr->FZY * _zy;

		spr->FCalcedBB  =  false;
	}

	// 拡大率の更新
	FZX  =  FZX * _zx;
	FZY  =  FZY * _zy;

}

void TSpriteEXCombiner::Zoom( f32 _zx, f32 _zy )
{
	ZoomR(1 / FZX, 1 / FZY);			// 元の大きさに戻して
	ZoomR(_zx, _zy);                    // スケーリング

}

bool TSpriteEXCombiner::CollisionCheck( TSpriteBase* _spr )
{
	bool Result  =  false;
	if( (! _spr->FHit) || (! FHit) ) {
		return false;
	}

	if( _spr->GetClassType() == SPR_CLASS_EX ) {
		// TSpriteEXが引数として渡された場合
		// TSpriteEXとTSpriteEXCの衝突判定
		Result  =  ((TSpriteEX*)_spr)->CollisionCheck(this);
	}
	else if( _spr->GetClassType() == SPR_CLASS_COMBINER ) {
		// TSpriteEXCombinerが引数として渡された場合
		// TSpriteEXCとTSpriteEXCの衝突判定
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
	// 全ての登録済スプライトについて処理
	for( u32 i  =  0 ; i < FSprList.size(); i++) {
		s16 z  =  (s16)((TSpriteEX*)FSprList[i])->FZ + _z;		// 単純に足す
		if( z < 0 )			z  =  0;  			// 0..255の範囲内に収める
		else if( z > 255 )	z  =  255;
		((TSpriteEX*)FSprList[i])->FZ  =  z;             // Z値の更新
	}

}

void TSpriteEXCombiner::SetFVisible( bool _flg )
{
	// 全ての登録済スプライトについて処理
	for( u32 i  =  0 ; i < FSprList.size(); i++) 
		((TSpriteEX*)FSprList[i])->FVisible  =  _flg;

	FVisible  =  _flg;
}

