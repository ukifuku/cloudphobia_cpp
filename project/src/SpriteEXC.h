#pragma once

#include "SpriteEX.h"

// スプライトコンバイナークラス
class TSpriteEXCombiner : public TSpriteBase
{
protected:
	f32 FX;     		// 座標（ワールド座標を基準）
	f32 FY;             //           〃
	vector<TSpriteBase*> FSprList;                // スプライトリスト
	void	SetFColor(const D3DCOLOR _col );   	// カラー値の設定（即値）
	TDGTexture*	GetFTex(); 
	void	SetFBM(TBlendMode _bm);       // ブレンドモードの設定
public:

	TSpriteEXCombiner();      			   			// コンストラクタ
	~TSpriteEXCombiner();                   // デストラクタ
	void	AddSprite(TSpriteEX* _spr);        // スプライトを登録
	void	SetColor(u8 _val, TColorType _type ); // カラー値の設定（A,R,G,B指定）
	void 	MoveR(s16 _x, s16 _y);           // 平行移動（相対指定）
	void 	MoveR(f32 _x, f32 _y);	
	void 	Move(f32 _x, f32 _y );            // 平行移動（絶対指定）
	void 	SlideMoveR(s16 _x, s16 _y );       // スライド移動（相対指定）
	void 	SlideMoveR(f32 _x, f32 _y);       // スライド移動（相対指定）
	void 	RotateR(s16 _r );             // 回転（相対指定）
	void	Rotate(s16  _r);              // 回転（絶対指定）
	void	ZoomR(f32 _zx, f32 _zy ); 			// 拡縮（相対指定）
	void 	Zoom(f32 _zx, f32 _zy );            // 拡縮（絶対指定）
	bool	CollisionCheck(TSpriteBase* _spr); // 当たり判定をとる
	void ChangeZLevel(s16 _z );       // Z値の変更
	void 	SetFVisible(bool _flg );   // 可視フラグの設定
	vector<TSpriteBase*>& GetList() {return FSprList;}
	TSpriteBase*	GetItem(u16 idx) { return idx < FSprList.size() ? FSprList[idx] : NULL; }
};

