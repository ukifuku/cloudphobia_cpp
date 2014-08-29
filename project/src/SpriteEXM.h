#include "SpriteEXU.h"
#include "SpriteEX.h"
#include "SpriteEXC.h"
#include "SXLib9.h"

// �X�v���C�g�}�l�[�W��
class TSpriteEXManager{
protected:
	TDGCarad* FDDDD;                    // DDDD�i�ւ̃|�C���^�j
	TBlendMode FBMCHFlg;   			// BlendMode�̕ύX�̌��m�Ɏg�p
	bool FSmoothCHFlg;      		    // Smooth�t���O�̕ύX�̌��m�Ɏg�p
	TDGTexture* FTexCHFlg;         	// Texture�̕ύX�̌��m�Ɏg�p

	s16 	FCX; 				// �J�����̊�_
	s16	FCY;                 //       �V
	f32	FZX;                   // �J�����̊g�嗦
	f32	FZY;                   //       �V
	u16	FR;                     // �J�����̉�]�p
	Rect	FClearRect;     				// ��ʏ����͈�
	vector<TSpriteBase*> FSprList;					// �o�^�X�v���C�g�̃��X�g
	TDGVertexBuffer* FVertBuff;	           	// �����_�����O�p���_�o�b�t�@
	D3DTLVERTEX2* FSysVertBuff;                 // �����_�����O�p���_�o�b�t�@(no VB)
	TDGTexture*	FBGTex,*FBGTex2,*FTempTex1,*FTempTex2,*FBlurTex1,*FBlurTex2, *FAlphaEffectTex, *FAddEffectTex;
	IDirect3DSurface9*	FRendTarget, *FRendTarget2, *FRendTarget3,*FTmpRendTarget1,*FTmpRendTarget2,*FBlurRendTarget1,*FBlurRendTarget2, *FAlphaRendTarget, *FAddRendTarget;
	IDirect3DSurface9*	FSSSurface; //�X�N���[���V���b�g�p�T�[�t�F�X
	bool	FCreateBump;
	bool	FCreateVB;

	f32 	FaspX,FaspY;                       //�A�X�y�N�g��
	bool	FSaveSS;                     //��ʂ��Z�[�u���邩
	string	FSaveFileName;
	u16	FBGTexSize;                         //�w�i�e�N�X�`���̈�ӂ̃T�C�Y
	u16	FBumpCHFlg;
	bool	FCopied,FCopied2,FCopied3;
	f32 	FBUMax,FBVMax;                      //�w�i�e�N�X�`����UV����l
	bool	FChangeCHFlg;                     //�O�̃X�v���C�g�ƕ`��������ω�������
	u32	FTotalDraw,FTotalSpr;
	f32	FChangeBLVFlg;
	u32	FBloomLevel;                      //�u���[�~���O�쐬���̋P�x���x��
	s32	FBloomBlendCnt;                      //�u���[�~���O�쐬���̏d�˂��킹��
	bool	FCreateBloom;
	bool	FMakeBlurFlag;
	s32	FMaxLuminance;       //��ʒ��̍ő�P�x

	TSpriteEX* GetFSprList();
	void SetFColorKey(bool _flg);
	void MakeBlurInternal();
	void DrawSameSpr( u32 _poly, s32* _start);
	u32 GetToneMappedCol(u32 _col, f32 _factor);
public:
	bool 	FVisible;   		   		// ���t���O
	bool	FSmooth	;  				// �X���[�W���O�t���O
	bool	FColorKey;
	D3DCOLOR	FClearColor;					// ��ʏ����̐F
	f32 	FX;					// �J�������W
	f32 	FY;                 	//     �V
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
	TSpriteEXManager(TDGCarad& _dd, bool _bump, bool _bloom);       			// �R���X�g���N�^
	~TSpriteEXManager();       		 	// �f�X�g���N�^
	void AddSprite(TSpriteBase* _spr); 	// �X�v���C�g�o�^
	void RemoveSprite(TSpriteBase* _spr); 		// �X�v���C�g������
	void Render();								// �o�^�X�v���C�g���܂Ƃ߂ĕ`��
	void Sort(); 								// �X�v���C�g���X�g���œK��
	void SortOnBrendMode();
	void SortOnTexture();
	void SortOnZ();
	void ClearSprite();							// �X�v���C�g���X�g���N���A
	void ClearScreen(); 							// ��ʏ���
	void MoveR( s32 _x, s32 _y );    			// �J�������ړ��i���Ύw��j
	void MoveR(f32 _x, f32 _y ); 
	void Move( s32 _x, s32 _y );     			//      �V     �i��Ύw��j
	void Move(f32 _x, f32 _y );     			//      �V     �i��Ύw��j
	void SlideMoveR( s32 _x, s32 _y ); 			//      �V     �i���Ύw��A�J�����������l���j
	void SlideMoveR(f32 _x, f32 _y); 
	void RotateR(s16 _r );				// �J��������]�i���Ύw��j
	void Rotate(s16 _r); 				//      �V     �i��Ύw��j
	void ZoomR(f32 _zx, f32 _zy);				// �g�k�i���Ύw��j
	void Zoom(f32 _zx, f32 _zy );         		//  �V �i��Ύw��j
	void GeometryProcess();  					// �����_�����O�p���_�o�b�t�@�𐶐�
	void RenderHitArea();
	void PointTransformToSCR(f32* _x, f32* _y );	// ���_�̃g�����X�t�H�[��
	void CalcScreenBB(Rect* _rt );     		// �X�N���[����BB�����߂�
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

