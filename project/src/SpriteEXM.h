#include "SpriteEXU.h"
#include "SpriteEX.h"
#include "SpriteEXC.h"
#include "SXLib9.h"

// スプライトマネージャ
class TSpriteEXManager{
protected:
	TDGCarad* FDDDD;                    // DDDD（へのポインタ）
	TBlendMode FBMCHFlg;   			// BlendModeの変更の検知に使用
	bool FSmoothCHFlg;      		    // Smoothフラグの変更の検知に使用
	TDGTexture* FTexCHFlg;         	// Textureの変更の検知に使用

	s16 	FCX; 				// カメラの基準点
	s16	FCY;                 //       〃
	f32	FZX;                   // カメラの拡大率
	f32	FZY;                   //       〃
	u16	FR;                     // カメラの回転角
	Rect	FClearRect;     				// 画面消去範囲
	vector<TSpriteBase*> FSprList;					// 登録スプライトのリスト
	TDGVertexBuffer* FVertBuff;	           	// レンダリング用頂点バッファ
	D3DTLVERTEX2* FSysVertBuff;                 // レンダリング用頂点バッファ(no VB)
	TDGTexture*	FBGTex,*FBGTex2,*FTempTex1,*FTempTex2,*FBlurTex1,*FBlurTex2, *FAlphaEffectTex, *FAddEffectTex;
	IDirect3DSurface9*	FRendTarget, *FRendTarget2, *FRendTarget3,*FTmpRendTarget1,*FTmpRendTarget2,*FBlurRendTarget1,*FBlurRendTarget2, *FAlphaRendTarget, *FAddRendTarget;
	IDirect3DSurface9*	FSSSurface; //スクリーンショット用サーフェス
	bool	FCreateBump;
	bool	FCreateVB;

	f32 	FaspX,FaspY;                       //アスペクト比
	bool	FSaveSS;                     //画面をセーブするか
	string	FSaveFileName;
	u16	FBGTexSize;                         //背景テクスチャの一辺のサイズ
	u16	FBumpCHFlg;
	bool	FCopied,FCopied2,FCopied3;
	f32 	FBUMax,FBVMax;                      //背景テクスチャのUV上限値
	bool	FChangeCHFlg;                     //前のスプライトと描画条件が変化したか
	u32	FTotalDraw,FTotalSpr;
	f32	FChangeBLVFlg;
	u32	FBloomLevel;                      //ブルーミング作成時の輝度レベル
	s32	FBloomBlendCnt;                      //ブルーミング作成時の重ねあわせ回数
	bool	FCreateBloom;
	bool	FMakeBlurFlag;
	s32	FMaxLuminance;       //画面中の最大輝度

	TSpriteEX* GetFSprList();
	void SetFColorKey(bool _flg);
	void MakeBlurInternal();
	void DrawSameSpr( u32 _poly, s32* _start);
	u32 GetToneMappedCol(u32 _col, f32 _factor);
public:
	bool 	FVisible;   		   		// 可視フラグ
	bool	FSmooth	;  				// スムージングフラグ
	bool	FColorKey;
	D3DCOLOR	FClearColor;					// 画面消去の色
	f32 	FX;					// カメラ座標
	f32 	FY;                 	//     〃
	//property CX			: Smallint		read FCX		write FCX;
	//property CY			: Smallint		read FCY     	write FCY;
	//property R			: WORD			read FR;
	//property ZX			: Single		read FZX;
	//property ZY			: Single		read FZY;
	//property Visible	: Boolean		read FVisible	write FVisible;
	//property Smooth		: Boolean		read FSmooth	write FSmooth;
	//property ClearRect 	: TRect			read FClearRect	write FClearRect;
	//property ClearColor	: D3DCOLOR		read FClearColor write FClearColor;
	//property SprList	: TList			read FSprList;
	//property ColorKey	: Boolean		read FColorKey	write SetFColorKey;
	//              property TotalDraw      : Cardinal              read FTotalDraw;
	//              property TotalSpr       : Cardinal              read FTotalSpr;
	//              property BGTex          : TDGTexture            read FBGTex;
	//              property BloomTex1       : TDGTexture            read FTempTex1;
	//              property BloomTex2       : TDGTexture            read FTempTex2;
	//              property BlurTex1       : TDGTexture            read FBlurTex1;
	//              property BlurTex2       : TDGTexture            read FBlurTex2;
	TDGTexture*		GetAlphaEffectTex() { return FAlphaEffectTex; }
	TDGTexture*		GetAddEffectTex() { return FAddEffectTex; }
	TDGTexture*		GetBloomTex1() { return FTempTex1; }
	TDGTexture*		GetBloomTex2() { return FTempTex2; }
	TDGTexture*		GetBlurTex1() { return FBlurTex1; }
	TDGTexture*		GetBlurTex2() { return FBlurTex2; }
	TSpriteEXManager(TDGCarad& _dd, bool _bump, bool _bloom);       			// コンストラクタ
	~TSpriteEXManager();       		 	// デストラクタ
	void AddSprite(TSpriteBase* _spr); 	// スプライト登録
	void RemoveSprite(TSpriteBase* _spr); 		// スプライトを解除
	void Render();								// 登録スプライトをまとめて描画
	void Sort(); 								// スプライトリストを最適化
	void SortOnBrendMode();
	void SortOnTexture();
	void SortOnZ();
	void ClearSprite();							// スプライトリストをクリア
	void ClearScreen(); 							// 画面消去
	void MoveR( s32 _x, s32 _y );    			// カメラを移動（相対指定）
	void MoveR(f32 _x, f32 _y ); 
	void Move( s32 _x, s32 _y );     			//      〃     （絶対指定）
	void Move(f32 _x, f32 _y );     			//      〃     （絶対指定）
	void SlideMoveR( s32 _x, s32 _y ); 			//      〃     （相対指定、カメラ向きを考慮）
	void SlideMoveR(f32 _x, f32 _y); 
	void RotateR(s16 _r );				// カメラを回転（相対指定）
	void Rotate(s16 _r); 				//      〃     （絶対指定）
	void ZoomR(f32 _zx, f32 _zy);				// 拡縮（相対指定）
	void Zoom(f32 _zx, f32 _zy );         		//  〃 （絶対指定）
	void GeometryProcess();  					// レンダリング用頂点バッファを生成
	void RenderHitArea();
	void PointTransformToSCR(f32* _x, f32* _y );	// 頂点のトランスフォーム
	void CalcScreenBB(Rect* _rt );     		// スクリーンのBBを求める
	void SetAspect(f32 _ax, f32 _ay);
	void SaveScreenShot(const string& _filename);
	void SavingProcedure();
	void ReleaseBeforeReset();
	void InitAfterReset();
	void MakeBlooming();
	void SetBloomLevel(u32 _level);
	void SetBloomLevelRed(u32 _level);
	void SetBloomLevelGreen(u32 _level);
	void SetBloomLevelBlue(u32 _level);
	void SetBloomBlend(s32 _cnt);
	void MakeBlur();
	void SetMaxLuminance(s32 _L);
	void SetExtraEffect(bool _exeffect, bool _bloom);
};

