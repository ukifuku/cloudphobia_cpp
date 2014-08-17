#include "SpriteEX.h"
#include "SpriteEXC.h"
#include "SXLib9.h"


void TSpriteEX::SetFColor( const D3DCOLOR _col )
{
	FColor  =  _col;
	for( u32 i  =  0 ; i <= 5; i++ ){
		FTLVs[i].color  =  FColor;
	}
}

TDGTexture* TSpriteEX::GetFTex()
{
	return FTexAnimAry[FAnimIdx].RTex;
}

void TSpriteEX::SetFAnim( bool _flg )
{
	FAnim  =  _flg;
}

void TSpriteEX::SetFAnimBgnIdx( u16 _idx )
{
	FAnimBgnIdx  =  _idx;

}

void TSpriteEX::SetFAnimEndIdx( u16 _idx )
{
	FAnimEndIdx  =  _idx;

}

void TSpriteEX::SetFAnimItv( u16 _itv )
{
	FAnimItv  =  _itv;

}

TSpriteEX::TSpriteEX( const TSpriteEXBLT& _blt )
{
	FDDDD 		 =  _blt.RDDDD;
	FColor 		 =  _blt.RColor;
	FCX 		 =  0.f;
	FCY 		 =  0.f;
	FZX			 =  1.0f;
	FZY			 =  1.0f;
	//	FOWidth	 	 =  _blt.RWidth;
	//	FOHeight	 =  _blt.RHeight;
	FZ 			 =  _blt.RZ;
	FR 			 =  PI_ZERO;
	FBM 		 =  _blt.RBM;
	FAnimItv 	 =  1;

	FTLVs[0] = MakeTLV(          0, 		   0,   1, 1, FColor, 0,   0,	0, 0, 0);
	FTLVs[1] = MakeTLV(_blt.RWidth, 	           0,  	1, 1, FColor, 0,   0,   0, 0, 0);
	FTLVs[2] = MakeTLV(          0, 	_blt.RHeight, 	1, 1, FColor, 0,   0,   0, 0, 0);
	FTLVs[3] = MakeTLV(_blt.RWidth,	           0, 	1, 1, FColor, 0,   0,   0, 0, 0);
	FTLVs[4] = MakeTLV(          0,	_blt.RHeight, 	1, 1, FColor, 0,   0,   0, 0, 0);
	FTLVs[5] = MakeTLV(_blt.RWidth,	_blt.RHeight, 	1, 1, FColor, 0,   0,   0, 0, 0);
	MoveR(_blt.RX, _blt.RY);
	//	VertexUVUpdate;
	FSmooth = true;
	FTransform = true;
	FEffectNum = 0;
	FBumpSize = 0;
	FBumpSlideX = 0;
	FBumpSlideY = 0;
	FDistorte = false;
	FDistortRate = 1;
	FRHW  =  0.5;
	FBumpTex = NULL;
	FBumpLevel = 0;
	FLuminance = 100;
	FToneMapping = false;

}

TSpriteEX::~TSpriteEX()
{

}

void TSpriteEX::Zoom( f32 _zx, f32 _zy )
{
	ZoomR(1 / FZX, 1 / FZY);
	ZoomR(_zx, _zy);

}

void TSpriteEX::ZoomR( f32 _zx, f32 _zy )
{
	for( u32 i  =  0 ; i <= 5; i++ ) {
		FTLVs[i].sx  =  _zx * FTLVs[i].sx - (_zx - 1) * FCX;
		FTLVs[i].sy  =  _zy * FTLVs[i].sy - (_zy - 1) * FCY;
	}

	FCalcedBB = false;

	FZX  =  FZX * _zx;
	FZY  =  FZY * _zy;

}

void TSpriteEX::Move( f32 _x, f32 _y )
{
	MoveR(_x - FCX, _y - FCY);

}

void TSpriteEX::MoveR( s32 _x, s32 _y )
{
	FCX  =  FCX + _x;
	FCY  =  FCY + _y;

	for( u32 i  =  0 ; i <= 5; i++ ) {
		FTLVs[i].sx  =  FTLVs[i].sx + _x;
		FTLVs[i].sy  =  FTLVs[i].sy + _y;
	}
	FCalcedBB = false;

}

void TSpriteEX::MoveR( f32 _x, f32 _y )
{
	FCX  =  FCX + _x;
	FCY  =  FCY + _y;

	for( u32 i  =  0 ; i <= 5; i++ ) {
		FTLVs[i].sx  =  FTLVs[i].sx + _x;
		FTLVs[i].sy  =  FTLVs[i].sy + _y;
	}
	FCalcedBB = false;

}

void TSpriteEX::Rotate( s16 _r )
{
	RotateR((_r - FR) & 0x0FFF);

}

void TSpriteEX::RotateR( s16 _r )
{
	_r  =  _r & 0x0FFF;  		   	// 0..4095の範囲内に収める
	FR += _r;            		// FRの内容を更新
	FR  =  FR & 0x0FFF;				// 0..4095の範囲内に収める
	for( u32 i  =  0 ; i <= 5 ; i++ ) { 	   	// 回転処理
		FTLVs[i].sx  =  FTLVs[i].sx - FCX;    					// 一旦基準点に持っていって
		FTLVs[i].sy  =  FTLVs[i].sy - FCY;
		f32 ax  =  FTLVs[i].sx * cost[_r] - FTLVs[i].sy * sint[_r];	// 回転して
		f32 ay  =  FTLVs[i].sx * sint[_r] + FTLVs[i].sy * cost[_r];
		FTLVs[i].sx  =  ax + FCX;                           		// 元に戻す
		FTLVs[i].sy  =  ay + FCY;
	}
	FCalcedBB = false;

}

void TSpriteEX::SlideMoveR( s32 _x, s32 _y )
{
	SlideMoveR(_x + 0.0f, _y + 0.0f);
}

void TSpriteEX::SlideMoveR( f32 _x, f32 _y )
{
	exRotate(&_x, &_y, 0, 0, _x, _y, FR);
	for( u32 i  =  0 ; i <= 5; i++ ) {
		FTLVs[i].sx  =  FTLVs[i].sx + _x;
		FTLVs[i].sy  =  FTLVs[i].sy + _y;
	}
	FCX  =  FCX + _x;
	FCY  =  FCY + _y;
	FCalcedBB = false;

}

void TSpriteEX::SetColor( u8 _val, TColorType _type )
{
	u32 mask;
	// 変更対象の色値を0にする
	mask  =  0xFF << ((3 - (s32)_type) << 3);
	FColor  =  (FColor | mask) & ( ~mask);

	// 新たな色値を代入
	mask  =  _val << ((3 - (s32)_type) << 3);
	FColor  =  FColor | mask;

	for( u32 i  =  0 ; i <= 5; i++ ){
		FTLVs[i].color  =  FColor;
	}
}

u8 TSpriteEX::GetColor( TColorType _type )
{
	return (u8)( FColor >> ((3 - (s32)_type) << 3) );
}

void TSpriteEX::SetColorR( s32 _val, TColorType _type )
{
	s32 tempCol = GetColor(_type);
	tempCol  =  glm::max(0, glm::min(255, tempCol + _val));
	SetColor(tempCol, _type);
}

bool TSpriteEX::CollisionCheck( TSpriteBase* _spr )
{
	TSpriteEXCombiner* sprc;
	if( (! _spr->FHit) || (! FHit) ) {
		// 双方のスプライトのうちどちらかのヒットフラグが立っていない時
		return false;
	}

	bool Result  =  false;

	if( _spr->GetClassType() == SPR_CLASS_EX ) {
		// TSpriteEXが引数として渡された時
		return CollisionCheckSPREX((TSpriteEX*)_spr);
	}
	else if( _spr->GetClassType() == SPR_CLASS_COMBINER ) {
		// TSpriteEXCombinerが引数として渡された時
		sprc  =  reinterpret_cast<TSpriteEXCombiner*>(_spr);
		//		sprc.SprList.Pack;
		for( u32 i  =  0 ; i < sprc->GetList().size(); i++ ) {
			if( CollisionCheckSPREX((TSpriteEX*)sprc->GetItem(i)) ) {
				return true;
			}
		}
	}

	return Result;
}

bool TSpriteEX::CollisionCheckSPREX( TSpriteEX* _spr )
{
	vector<THitArea> haa1, haa2;      // 当たり判定エリア配列を格納
	TSPoint	pt[4];       // 2つの線分を格納
	// バウンディングボックスによるプリチェック
	const Rect* bb1  =  GetBB();
	const Rect* bb2  =  _spr->GetBB();
	if( ! CheckRectCross(*bb1, *bb2) ) {
		return false;
	}


	// 当たり判定エリア配列のトランスフォーム
	haa1.resize(FHitAreaAry.size());
	haa2.resize(_spr->FHitAreaAry.size());
	HitAreaTransform(haa1);
	_spr->HitAreaTransform(haa2);

	// 全ての当たり判定エリアを判定
	for( u32 i  =  0 ; i < haa1.size(); i++ ) {
		// 対象スプライトの全ての当たり判定エリアを判定
		for( u32 k  =  0 ; k < haa2.size(); k++ ) {
			// タイプによる前判定
			switch( haa1[i].RType ){
			case htGround: 	
				if( haa2[k].RType = htSky ) continue;
				break;
			case htSky:		
				if( haa2[k].RType = htGround ) continue;
				break;
			}

			// 線分交差判定法を用いて判定：
			// 当たり判定エリアの全ての辺を判定
			for( u32 j  =  0 ; j <= 3; j++ ) {
				// スプライトの線分セッティング
				pt[0]  =  haa1[i].RPointAry[j];
				pt[1]  =  haa1[i].RPointAry[(j + 1) & 3];

				// 対象スプライトの当たり判定エリアの全ての辺を判定
				for( u32 l  =  0 ; l <= 3 ; l++) {
					// 対象スプライトの線分セッティング
					pt[2]  =  haa2[k].RPointAry[l];
					pt[3]  =  haa2[k].RPointAry[(l + 1) & 3];
					// 当たり判定
					if( CheckLineCross(pt) ) {
						return true;
					}
				}
			}

			// 頂点領域内判定法を用いて判定
			for( u32 j  =  0 ; j <= 3; j++ ) {
				// こちら→対象スプライトに対して調べる
				if( PointInArea(haa1[i].RPointAry,
					TSPoint(haa2[k].RPointAry[j])) ) {
						return true;
				}
				// 対象スプライト→こちらに対して調べる
				else if( PointInArea(haa2[k].RPointAry,
					TSPoint(haa1[i].RPointAry[j])) ) {
						return true;
				}
			}
		}
	}

	return false;				// 全ての当たり判定エリアを調べても見つからない→非接触

}

void TSpriteEX::SetTexRect( const Rect& _rt, const u16 _no /*= 0*/ )
{
	FTexAnimAry[_no].RU1  =  (f32)_rt.Left / (f32)FTexAnimAry[_no].RTex->GetWidth();
	FTexAnimAry[_no].RV1  =  (f32)_rt.Top / (f32)FTexAnimAry[_no].RTex->GetHeight();
	FTexAnimAry[_no].RU2  =  (f32)_rt.Right / (f32)FTexAnimAry[_no].RTex->GetWidth();
	FTexAnimAry[_no].RV2  =  (f32)_rt.Bottom / (f32)FTexAnimAry[_no].RTex->GetHeight();
}

void TSpriteEX::AddTexture( TDGTexture* _tex, const Rect& _rt )
{
	TTexAnim newAnim;
	if( _tex != NULL ) {
		newAnim.RTex 	 =  _tex;
		newAnim.RU1 	 =  (f32)_rt.Left / (f32)_tex->GetWidth();
		newAnim.RV1 	 =  (f32)_rt.Top / (f32)_tex->GetHeight();
		newAnim.RU2 	 =  (f32)_rt.Right / (f32)_tex->GetWidth();
		newAnim.RV2 	 = (f32) _rt.Bottom / (f32)_tex->GetHeight();
	}
	else {
		newAnim.RTex 	 =  NULL;
		newAnim.RU1	 =  0;
		newAnim.RV1	 =  0;
		newAnim.RU2  =  1.0f;
		newAnim.RV2	 =  1.0f;
	}
	FTexAnimAry.push_back( newAnim );
	VertexUVUpdate();

}

void TSpriteEX::AddTexture( TDGTexture* _tex, u16 _left, u16 _top, u16 _right, u16 _bottom )
{
	TTexAnim newAnim;
	if( _tex != NULL ) {
		newAnim.RTex 	 =  _tex;
		newAnim.RU1 	 =  (f32)_left / (f32)_tex->GetWidth();
		newAnim.RV1 	 =  (f32)_top / (f32)_tex->GetHeight();
		newAnim.RU2 	 =  (f32)_right / (f32)_tex->GetWidth();
		newAnim.RV2 	 =  (f32)_bottom / (f32)_tex->GetHeight();
	}
	else {
		newAnim.RTex 	 =  NULL;
		newAnim.RU1		 =  0;
		newAnim.RV1		 =  0;
		newAnim.RU2 	 =  1.0f;
		newAnim.RV2		 =  1.0f;
	}
	FTexAnimAry.push_back( newAnim );
	VertexUVUpdate();

}

void TSpriteEX::VertexUVUpdate()
{
	FTLVs[0].tu  =  FTexAnimAry[FAnimIdx].RU1;
	FTLVs[0].tv  =  FTexAnimAry[FAnimIdx].RV1;
	FTLVs[1].tu  =  FTexAnimAry[FAnimIdx].RU2;
	FTLVs[1].tv  =  FTexAnimAry[FAnimIdx].RV1;
	FTLVs[2].tu  =  FTexAnimAry[FAnimIdx].RU1;
	FTLVs[2].tv  =  FTexAnimAry[FAnimIdx].RV2;
	FTLVs[3].tu  =  FTexAnimAry[FAnimIdx].RU2;
	FTLVs[3].tv  =  FTexAnimAry[FAnimIdx].RV1;
	FTLVs[4].tu  =  FTexAnimAry[FAnimIdx].RU1;
	FTLVs[4].tv  =  FTexAnimAry[FAnimIdx].RV2;
	FTLVs[5].tu  =  FTexAnimAry[FAnimIdx].RU2;
	FTLVs[5].tv  =  FTexAnimAry[FAnimIdx].RV2;
}

void TSpriteEX::Animation()
{
	if( FAnim ) {							// アニメフラグがONなら
		if( FAnimCnt >= FAnimItv ) {      // 指定フレーム経過していたら
			FAnimCnt  =  0;
			if( FAnimIdx < FAnimEndIdx )      // インデックスを超えていなければ
				AnimNext();                       // アニメーション
			else
				FAnimIdx  =  FAnimBgnIdx;        // インデックスを最初に戻す
		}
		else
			FAnimCnt++;                      // アニメカウンタを+1
	}

}

void TSpriteEX::AnimNext()
{
	if( FAnim ) {
		if( FAnimIdx < FTexAnimAry.size()-1 )
			FAnimIdx++;
		else
			FAnimIdx  =  0;
	}

}

void TSpriteEX::AnimPrev()
{
	if( FAnim ) {
		if( FAnimIdx > 0 )
			FAnimIdx--;
		else
			FAnimIdx  =  FTexAnimAry.size()-1;
	}

}

void TSpriteEX::AnimReinitialize()
{
	FAnimCnt  =  0;
	FAnim  =  false;
	FAnimIdx  =  0;
	FAnimBgnIdx  =  0;
	FAnimEndIdx  =  0;
	FAnimItv  =  1;
	FTexAnimAry.clear();

}

void TSpriteEX::AddHitArea( const glm::vec2& _pt1, const glm::vec2& _pt2, const glm::vec2& _pt3, const glm::vec2& _pt4, THitType _type )
{
	THitArea newArea;
	newArea.RPointAry[0].x  =  _pt1.x;
	newArea.RPointAry[0].y  =  _pt1.y;
	newArea.RPointAry[1].x  =  _pt2.x;
	newArea.RPointAry[1].y  =  _pt2.y;
	newArea.RPointAry[2].x  =  _pt3.x;
	newArea.RPointAry[2].y  =  _pt3.y;
	newArea.RPointAry[3].x  =  _pt4.x;
	newArea.RPointAry[3].y  =  _pt4.y;
	newArea.RType  =  _type;
	FHitAreaAry.push_back(newArea);
}

void TSpriteEX::AddHitArea( const TPoint& _pt1, const TPoint& _pt2, const TPoint& _pt3, const TPoint& _pt4, THitType _type )
{
	AddHitArea( glm::vec2(_pt1.X, _pt1.Y), glm::vec2(_pt2.X, _pt2.Y), glm::vec2(_pt3.X, _pt3.Y), glm::vec2(_pt4.X, _pt4.Y),  _type );
}

void TSpriteEX::HitAreaReinitialize()
{
	FHit  =  false;
	FHitAreaAry.clear();

}

void TSpriteEX::CalcBoundingBox( Rect& _rt )
{
	// BBの初期化
	_rt.Left	 =  0x7FFFFFFF;
	_rt.Top		 =  _rt.Left;
	_rt.Right 	 =  0;
	_rt.Bottom	 =  _rt.Right;

	for( u32 i  =  0 ; i <= 3 ; i++ ) {
		if( _rt.Left > (s32)FTLVs[i].sx )	_rt.Left  =  (s32)(FTLVs[i].sx);
		if( _rt.Right < (s32)FTLVs[i].sx ) 	_rt.Right  =  (s32)(FTLVs[i].sx);
		if( _rt.Top > (s32)FTLVs[i].sy )		_rt.Top  =  (s32)(FTLVs[i].sy);
		if( _rt.Bottom < (s32)FTLVs[i].sy )	_rt.Bottom  =  (s32)(FTLVs[i].sy);
	}

}

void TSpriteEX::HitAreaTransform( vector<THitArea>& _haa )
{
	_haa.resize(FHitAreaAry.size());
	for( u32 i  =  0 ; i < _haa.size(); i++ ) {
		memcpy( _haa[i].RPointAry, FHitAreaAry[i].RPointAry, sizeof(TSPoint)*4 );
		_haa[i].RType  =  FHitAreaAry[i].RType;
	}

	for( u32 i  =  0 ; i < _haa.size(); i++ ) {
		for( u32 j  =  0 ; i <= 3; i++ ) {
			//				spt  =  SPoint(FCX - FTLVs[0].sx, FCY - FTLVs[0].sy);
			//				exRotate(&spt.x, &spt.y, 0, 0, spt.x, spt.y, -FR);

			// 基準座標中心の座標系に変換
			//				RPointAry[j].x  =  RPointAry[j].x - spt.x;
			//				RPointAry[j].y  =  RPointAry[j].y - spt.y;

			// スケーリング
			_haa[i].RPointAry[j].x  =  FZX * _haa[i].RPointAry[j].x;
			_haa[i].RPointAry[j].y  =  FZY * _haa[i].RPointAry[j].y;

			// 回転
			if( FR != PI_ZERO ) {
				f32 ax  =  _haa[i].RPointAry[j].x * cost[FR] - _haa[i].RPointAry[j].y * sint[FR];
				f32 ay  =  _haa[i].RPointAry[j].x * sint[FR] + _haa[i].RPointAry[j].y * cost[FR];
				_haa[i].RPointAry[j].x  =  ax;
				_haa[i].RPointAry[j].y  =  ay;
			}

			// 元の座標系に戻す
			//				RPointAry[j].x  =  RPointAry[j].x + spt.x;
			//				RPointAry[j].y  =  RPointAry[j].y + spt.y;

			// 平行移動
			_haa[i].RPointAry[j].x  =  _haa[i].RPointAry[j].x + FTLVs[0].sx;
			_haa[i].RPointAry[j].y  =  _haa[i].RPointAry[j].y + FTLVs[0].sy;
		}
	}

}

void TSpriteEX::BumpUVUpdate( f32 _u1, f32 _v1, f32 _u2, f32 _v2 )
{
	FTLVs[0].bu = _u1;
	FTLVs[0].bv = _v1;
	FTLVs[1].bu = _u2;
	FTLVs[1].bv = _v1;
	FTLVs[2].bu = _u1;
	FTLVs[2].bv = _v2;
	FTLVs[3].bu = _u2;
	FTLVs[3].bv = _v1;
	FTLVs[4].bu = _u1;
	FTLVs[4].bv = _v2;
	FTLVs[5].bu = _u2;
	FTLVs[5].bv = _v2;


}

void TSpriteEX::SetVertex( s32 _ind, f32 _x, f32 _y )
{
	switch(_ind){
		case 0:{
			FTLVs[0].sx = _x;
			FTLVs[0].sy = _y;
		}break;
		case 1:{
			FTLVs[1].sx = _x;
			FTLVs[1].sy = _y;
			FTLVs[3].sx = _x;
			FTLVs[3].sy = _y;
		}break;
		case 2:{
			FTLVs[2].sx = _x;
			FTLVs[2].sy = _y;
			FTLVs[4].sx = _x;
			FTLVs[4].sy = _y;
		}break;
		case 3:{
			FTLVs[5].sx = _x;
			FTLVs[5].sy = _y;
		}break;
	}

}

void TSpriteEX::SetUV( s32 _ind, f32 _u, f32 _v )
{
	switch(_ind){
		case 0:{
			FTLVs[0].tu = _u;
			FTLVs[0].tv = _v;
		}break;
		case 1:{
			FTLVs[1].tu = _u;
			FTLVs[1].tv = _v;
			FTLVs[3].tu = _u;
			FTLVs[3].tv = _v;
		}break;
		case 2:{
			FTLVs[2].tu = _u;
			FTLVs[2].tv = _v;
			FTLVs[4].tu = _u;
			FTLVs[4].tv = _v;
		}break;
		case 3:{
			FTLVs[5].tu = _u;
			FTLVs[5].tv = _v;
		}break;
	}
}

void TSpriteEX::SetVertexColor( s32 _ind, u32 _col )
{
	switch(_ind){
	case 0:{
		FTLVs[0].color = _col;
		   }break;
	case 1:{
		FTLVs[1].color = _col;
		FTLVs[3].color = _col;
		   }break;
	case 2:{
		FTLVs[2].color = _col;
		FTLVs[4].color = _col;
		   }break;
	case 3:{
		FTLVs[5].color = _col;
		   }break;
	}

}

void TSpriteEX::SetGradation( s32 _deg, u32 _col, f32 _factor )
{
	//現在の角度と光源の角度との差をとる
	s32 diff  =  (FR - _deg) & 0xfff;

	//すべての頂点の色を現在の色で初期化
	for( u32 i  =  0 ; i <= 5; i++ )
		FTLVs[i].color  =  FColor;

	//角度_degで示された方向に向かって暗くする（左上が初期位置）
	SetVertexColor(0,GetSubColor(FColor,_col, pow(((cost[diff]+1.f)*0.5f),2.f)*_factor));
	SetVertexColor(1,GetSubColor(FColor,_col, pow(((cost[(diff+1024) & 4095]+1.f)*0.5f), 2.f)*_factor));
	SetVertexColor(2,GetSubColor(FColor,_col, pow(((cost[(diff-1024) & 4095]+1.f)*0.5f), 2.f)*_factor));
	SetVertexColor(3,GetSubColor(FColor,_col, pow(((cost[(diff+2048) & 4095]+1.f)*0.5f), 2.f)*_factor));

}

u32 TSpriteEX::GetSubColor( u32 _col1, u32 _col2, f32 _factor )
{
	u32 red,green,blue;
	//Red
	u32 term1  =  ((_col1 & 0xff0000) >> 16);
	u32 term2  =  (u32)((f32)((_col2 & 0xff0000) >> 16) * _factor);
	if( term2 > term1 )
		red  =  0;
	else
		red  =  term1 - term2;

	//Green
	term1  =  ((_col1 & 0xff00) >> 8);
	term2  =  (u32)((f32)((_col2 & 0xff00) >> 8) * _factor);
	if( term2 > term1 )
		green  =  0;
	else
		green  =  term1 -term2;

	//Blue
	term1  =  (_col1 & 0xff);
	term2  =  (u32)((f32)(_col2 & 0xff) * _factor);
	if( term2 > term1 )
		blue  =  0;
	else
		blue  =  term1 -term2;

	return (FColor & 0xff000000) | (red << 16) | (green << 8) | (blue);

}

Rect* TSpriteEX::GetBB()
{
	if( FCalcedBB ){
		return &FBB;
	}

	// BBの初期化
	FBB.Left	= 1000000;
	FBB.Top		= FBB.Left;
	FBB.Right 	= -1000000;
	FBB.Bottom	= FBB.Right;

	for( u32 i = 0 ; i <= 5 ; i++ ){
		if( FBB.Left > (s32)FTLVs[i].sx )		FBB.Left = (s32)(FTLVs[i].sx);
		if( FBB.Right < (s32)FTLVs[i].sx ) 	FBB.Right = (s32)(FTLVs[i].sx);
		if( FBB.Top > (s32)FTLVs[i].sy )		FBB.Top = (s32)(FTLVs[i].sy);
		if( FBB.Bottom < (s32)FTLVs[i].sy )	FBB.Bottom = (s32)(FTLVs[i].sy);
	}

	FCalcedBB = true;
	return &FBB;

}
