//------------------------------------------------------------------------------
// ファイル名	: SpriteEX.pas
// 説明			: スプライトを管理するクラス"TSpriteEX"を定義
//------------------------------------------------------------------------------
#pragma once

#include "SpriteEXU.h"
#include "Define.h"

enum SprClassType
{
	SPR_CLASS_BASE,
	SPR_CLASS_EX,
	SPR_CLASS_COMBINER,
};

// スプライトの基本クラス
class TSpriteBase {
protected:
public:
	u16			FR;    		   	// 回転角
	f32			FZX;			// X拡大率
	f32			FZY;		   	// Y拡大率
	f32			FCX;  	 		// 基準座標（ワールド座標を基準)
	f32			FCY;        	//    〃   （ワールド座標を基準）
	bool		FHit;			// 当たり判定ON/OFF
	bool		FSmooth;			// スムーズフィルタのON/OFF
	u16			FZ;        		// Z値
	TBlendMode	FBM;       // ブレンドモード
	D3DCOLOR	FColor;			// 頂点カラー
	bool		FVisible;          // 可視フラグ
	bool		FTransform;          // トランスフォームフラグ

	virtual		void 	SetFColor(const D3DCOLOR _col ) = 0;				// 色指定（即値）
	virtual 	TDGTexture*		GetFTex() = 0;

	virtual void 	Zoom(f32 _zx, f32 _zy) = 0;			        // 拡縮（絶対指定）
	virtual void    ZoomR(f32 _zx, f32 _zy )= 0;         			// 拡縮（相対指定）
	virtual void 	Move(f32 _x, f32 _y )= 0;                   // 平行移動（絶対指定）
	virtual void 	MoveR(s32 _x, s32 _y ) = 0;                  // 平行移動（相対指定）
	virtual void 	MoveR(f32 _x, f32 _y ) = 0;                  // 平行移動（相対指定）
	virtual void 	SlideMoveR(s32 _x, s32 _y ) = 0; 				// スライド移動
	virtual void 	SlideMoveR(f32 _x, f32 _y ) = 0; 				// スライド移動
	virtual void 	Rotate(s16 _r ) = 0;      				// 回転（絶対指定）
	virtual void 	RotateR(s16 _r )= 0;     				// 回転（相対指定）
	virtual void 	SetColor(u8 _val, TColorType _type ) = 0;	// 色指定（a, r, g, b 個別）
	virtual bool CollisionCheck(TSpriteBase* _spr ) = 0;// 当たり判定をとる
	virtual SprClassType	GetClassType() const{ return SPR_CLASS_BASE; }	
};


// スプライト管理クラス
class TSpriteEX : public TSpriteBase{
protected:

public:
	TDGCarad*		FDDDD;   	 		// DDDD（へのポインタ）
	D3DTLVERTEX2	FTLVs[6];           	// ポリゴンデータ
	vector<TTexAnim> FTexAnimAry;		// テクスチャアニメーション配列
	u16				FAnimCnt;             // アニメカウンタ
	bool			FAnim;          // アニメのON/OFF（True:ON）
	u16				FAnimIdx;				// テクスチャアニメ配列のインデックス
	u16				FAnimBgnIdx;             // アニメ開始インデックス
	u16				FAnimEndIdx;             // アニメ終了インデックス
	u16				FAnimItv;				// アニメスピード（切替間隔、フレームベース）
	vector<THitArea>	FHitAreaAry;		// 当たり判定配列

	u8				FEffectNum;
	f32				FBumpSize;         //テクスチャをずらす度合い
	s16				FBumpSlideX;         //テクスチャをずらす度合い（その２）
	s16				FBumpSlideY;
	bool			FDistorte;
	f32				FDistortRate;
	f32				FRHW;
	TDGTexture*		FBumpTex;
	f32				FBumpLevel;       //バンプ行列に入れる値
	glm::vec2		FPrevPos;
	glm::vec2		FCurrPos;
	glm::vec2		FNextPos;
	s32				FLuminance;      //輝度
	bool			FToneMapping;       //トーンマッピングの影響を受けるか

	Rect			FBB;
	bool			FCalcedBB;


	void 	SetFColor(const D3DCOLOR _col);				// 色指定（即値）
	TDGTexture* 	GetFTex();
	void 	SetFAnim(bool _flg);					// アニメON/OFFフラグの指定
	void 	SetFAnimBgnIdx(u16 _idx);				// アニメ開始インデックス指定
	void	SetFAnimEndIdx(u16 _idx);                // アニメ終了インデックス指定
	void	SetFAnimItv(u16 _itv);   				// アニメ間隔の指定
	vector<TTexAnim>&        GetFTexAnimAry(){ return FTexAnimAry; }               // テクスチャアニメ配列のポインタを得る
	vector<THitArea>& 	GetFHitAreaAry() { return FHitAreaAry; }				// 当たり判定配列のポインタを得る

	//property	TLVs		: TTLVs		read FTLVs;
	//property 	Anim		: Boolean	read FAnim		write SetFAnim;
	//property 	AnimIdx		: WORD		read FAnimIdx   	write FAnimIdx;
	//property	AnimBgnIdx	: WORD		read FAnimBgnIdx        write SetFAnimBgnIdx;
	//property	AnimEndIdx	: WORD		read FAnimEndIdx        write SetFAnimEndIdx;
	//property 	AnimItv		: WORD	   	read FAnimItv 	        write SetFAnimItv;
	//property 	TexAnimAry 	: TTexAnimAry   read FTexAnimAry;
	//property        HitAreaAry 	: PHitAreaAry   read GetFHitAreaAry;
	//property 	Tex		: TDGTexture    read GetFTex;
	//property	CalcedBB	: Boolean 	read FCalcedBB	        write FCalcedBB;
	//  property        EffectNum    	: Byte  	read FEffectNum	        write FEffectNum;
	//  property        BumpSize        : Single      read FBumpSize	        write FBumpSize;
	//  property        BumpSlideX      : SmallInt      read FBumpSlideX        write FBumpSlideX;
	//  property        BumpSlideY      : SmallInt      read FBumpSlideY        write FBumpSlideY;
	//  property        Distorte    	: Boolean	read FDistorte  	write FDistorte;
	//  property        DistortRate     : Single        read FDistortRate	write FDistortRate;
	//  property        EXBumpTex         : TDGTexture    read FBumptex         write FBumptex;
	//  property        BumpLevel       : Single        read FBumpLevel         write FbumpLevel;
	//  property        Luminance       :Integer    read FLuminance     write FLuminance;
	//  property        ToneMapping     :Boolean    read FToneMapping   write FToneMapping;

	TSpriteEX(const TSpriteEXBLT& _blt );    			// コンストラクタ
	~TSpriteEX();                          // デストラクタ
	void 	Zoom( f32 _zx, f32 _zy );			        // 拡縮（絶対指定）
	void    ZoomR( f32 _zx, f32 _zy );         			// 拡縮（相対指定）
	void 	Move(s32 _x, s32 _y ) { Move( (f32)_x, (f32)_y ); }                   // 平行移動（絶対指定）
	void 	Move(f32 _x, f32 _y );                   // 平行移動（絶対指定）
	void 	MoveR(s32 _x, s32 _y);                  // 平行移動（相対指定）
	void 	MoveR(f32 _x, f32 _y );                  // 平行移動（相対指定）
	void 	Rotate(s16 _r);      				// 回転（絶対指定）
	void 	RotateR(s16 _r );     				// 回転（相対指定）
	void 	SlideMoveR(s32 _x, s32 _y ); // スライド移動
	void 	SlideMoveR(f32 _x, f32 _y ); // スライド移動
	void 	SetColor(u8 _val, TColorType _type );
	u8 		GetColor(TColorType _type); 	// 色取得（a, r, g, b 個別）
	void 	SetColorR(s32 _val, TColorType _type );
	bool CollisionCheck(TSpriteBase* _spr );// 当たり判定をとる
	bool CollisionCheckSPREX(TSpriteEX* _spr);
	//		void	RenderHitArea(_col : TColor);				// 当たり判定エリアを描画
	// 色指定（a, r, g, b 個別）
	void 	SetTexRect(const Rect& _rt, const u16 _no = 0);  	// テクスチャ範囲の変更
	void 	AddTexture( TDGTexture* _tex, const Rect& _rt );
	void	AddTexture( TDGTexture* _tex, u16 _left, u16 _top, u16 _right, u16 _bottom);
	// テクスチャを配列に追加
	void 	VertexUVUpdate();								// D3DTLVERTEXのUVを更新
	void 	Animation();                                  // アニメ
	void 	AnimNext(); 									// アニメを1コマ進める
	void 	AnimPrev();                                   //       〃     戻す
	void 	AnimReinitialize();							// テクスチャアニメ周りを再初期化
	void	AddHitArea(  const glm::vec2& _pt1, const glm::vec2& _pt2, const glm::vec2& _pt3, const glm::vec2& _pt4, const THitType _type );
	void	AddHitArea( const TPoint& _pt1,  const TPoint& _pt2,  const  TPoint& _pt3,  const  TPoint& _pt4,  THitType _type );
	void 	HitAreaReinitialize();						// 当たり判定周りを再初期化
	void	CalcBoundingBox( Rect& _rt);		   		// バウンディングボックス矩形の計算
	void	HitAreaTransform( vector<THitArea>& _haa);       // 当たり判定のトランスフォーム
	//void       SetBumpUV(_x1,_y1,_x2,_y2);
	void       BumpUVUpdate(f32 _u1, f32 _v1, f32 _u2, f32 _v2);
	void       SetVertex( s32 _ind, f32 _x, f32 _y );
	void       SetUV( s32 _ind, f32 _u, f32 _v );
	void       SetVertexColor( s32 _ind, u32 _col );
	void       SetGradation( s32 _deg, u32 _col, f32 _factor );
	u32       GetSubColor( u32 _col1, u32 _col2, f32 _factor );
	Rect*	GetBB();
	virtual SprClassType	GetClassType() const { return SPR_CLASS_EX; }	

};

