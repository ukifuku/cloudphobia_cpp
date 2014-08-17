//------------------------------------------------------------------------------
// �t�@�C����	: SpriteEX.pas
// ����			: �X�v���C�g���Ǘ�����N���X"TSpriteEX"���`
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

// �X�v���C�g�̊�{�N���X
class TSpriteBase {
protected:
public:
	u16			FR;    		   	// ��]�p
	f32			FZX;			// X�g�嗦
	f32			FZY;		   	// Y�g�嗦
	f32			FCX;  	 		// ����W�i���[���h���W���)
	f32			FCY;        	//    �V   �i���[���h���W����j
	bool		FHit;			// �����蔻��ON/OFF
	bool		FSmooth;			// �X���[�Y�t�B���^��ON/OFF
	u16			FZ;        		// Z�l
	TBlendMode	FBM;       // �u�����h���[�h
	D3DCOLOR	FColor;			// ���_�J���[
	bool		FVisible;          // ���t���O
	bool		FTransform;          // �g�����X�t�H�[���t���O

	virtual		void 	SetFColor(const D3DCOLOR _col ) = 0;				// �F�w��i���l�j
	virtual 	TDGTexture*		GetFTex() = 0;

	virtual void 	Zoom(f32 _zx, f32 _zy) = 0;			        // �g�k�i��Ύw��j
	virtual void    ZoomR(f32 _zx, f32 _zy )= 0;         			// �g�k�i���Ύw��j
	virtual void 	Move(f32 _x, f32 _y )= 0;                   // ���s�ړ��i��Ύw��j
	virtual void 	MoveR(s32 _x, s32 _y ) = 0;                  // ���s�ړ��i���Ύw��j
	virtual void 	MoveR(f32 _x, f32 _y ) = 0;                  // ���s�ړ��i���Ύw��j
	virtual void 	SlideMoveR(s32 _x, s32 _y ) = 0; 				// �X���C�h�ړ�
	virtual void 	SlideMoveR(f32 _x, f32 _y ) = 0; 				// �X���C�h�ړ�
	virtual void 	Rotate(s16 _r ) = 0;      				// ��]�i��Ύw��j
	virtual void 	RotateR(s16 _r )= 0;     				// ��]�i���Ύw��j
	virtual void 	SetColor(u8 _val, TColorType _type ) = 0;	// �F�w��ia, r, g, b �ʁj
	virtual bool CollisionCheck(TSpriteBase* _spr ) = 0;// �����蔻����Ƃ�
	virtual SprClassType	GetClassType() const{ return SPR_CLASS_BASE; }	
};


// �X�v���C�g�Ǘ��N���X
class TSpriteEX : public TSpriteBase{
protected:

public:
	TDGCarad*		FDDDD;   	 		// DDDD�i�ւ̃|�C���^�j
	D3DTLVERTEX2	FTLVs[6];           	// �|���S���f�[�^
	vector<TTexAnim> FTexAnimAry;		// �e�N�X�`���A�j���[�V�����z��
	u16				FAnimCnt;             // �A�j���J�E���^
	bool			FAnim;          // �A�j����ON/OFF�iTrue:ON�j
	u16				FAnimIdx;				// �e�N�X�`���A�j���z��̃C���f�b�N�X
	u16				FAnimBgnIdx;             // �A�j���J�n�C���f�b�N�X
	u16				FAnimEndIdx;             // �A�j���I���C���f�b�N�X
	u16				FAnimItv;				// �A�j���X�s�[�h�i�ؑ֊Ԋu�A�t���[���x�[�X�j
	vector<THitArea>	FHitAreaAry;		// �����蔻��z��

	u8				FEffectNum;
	f32				FBumpSize;         //�e�N�X�`�������炷�x����
	s16				FBumpSlideX;         //�e�N�X�`�������炷�x�����i���̂Q�j
	s16				FBumpSlideY;
	bool			FDistorte;
	f32				FDistortRate;
	f32				FRHW;
	TDGTexture*		FBumpTex;
	f32				FBumpLevel;       //�o���v�s��ɓ����l
	glm::vec2		FPrevPos;
	glm::vec2		FCurrPos;
	glm::vec2		FNextPos;
	s32				FLuminance;      //�P�x
	bool			FToneMapping;       //�g�[���}�b�s���O�̉e�����󂯂邩

	Rect			FBB;
	bool			FCalcedBB;


	void 	SetFColor(const D3DCOLOR _col);				// �F�w��i���l�j
	TDGTexture* 	GetFTex();
	void 	SetFAnim(bool _flg);					// �A�j��ON/OFF�t���O�̎w��
	void 	SetFAnimBgnIdx(u16 _idx);				// �A�j���J�n�C���f�b�N�X�w��
	void	SetFAnimEndIdx(u16 _idx);                // �A�j���I���C���f�b�N�X�w��
	void	SetFAnimItv(u16 _itv);   				// �A�j���Ԋu�̎w��
	vector<TTexAnim>&        GetFTexAnimAry(){ return FTexAnimAry; }               // �e�N�X�`���A�j���z��̃|�C���^�𓾂�
	vector<THitArea>& 	GetFHitAreaAry() { return FHitAreaAry; }				// �����蔻��z��̃|�C���^�𓾂�

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

	TSpriteEX(const TSpriteEXBLT& _blt );    			// �R���X�g���N�^
	~TSpriteEX();                          // �f�X�g���N�^
	void 	Zoom( f32 _zx, f32 _zy );			        // �g�k�i��Ύw��j
	void    ZoomR( f32 _zx, f32 _zy );         			// �g�k�i���Ύw��j
	void 	Move(s32 _x, s32 _y ) { Move( (f32)_x, (f32)_y ); }                   // ���s�ړ��i��Ύw��j
	void 	Move(f32 _x, f32 _y );                   // ���s�ړ��i��Ύw��j
	void 	MoveR(s32 _x, s32 _y);                  // ���s�ړ��i���Ύw��j
	void 	MoveR(f32 _x, f32 _y );                  // ���s�ړ��i���Ύw��j
	void 	Rotate(s16 _r);      				// ��]�i��Ύw��j
	void 	RotateR(s16 _r );     				// ��]�i���Ύw��j
	void 	SlideMoveR(s32 _x, s32 _y ); // �X���C�h�ړ�
	void 	SlideMoveR(f32 _x, f32 _y ); // �X���C�h�ړ�
	void 	SetColor(u8 _val, TColorType _type );
	u8 		GetColor(TColorType _type); 	// �F�擾�ia, r, g, b �ʁj
	void 	SetColorR(s32 _val, TColorType _type );
	bool CollisionCheck(TSpriteBase* _spr );// �����蔻����Ƃ�
	bool CollisionCheckSPREX(TSpriteEX* _spr);
	//		void	RenderHitArea(_col : TColor);				// �����蔻��G���A��`��
	// �F�w��ia, r, g, b �ʁj
	void 	SetTexRect(const Rect& _rt, const u16 _no = 0);  	// �e�N�X�`���͈͂̕ύX
	void 	AddTexture( TDGTexture* _tex, const Rect& _rt );
	void	AddTexture( TDGTexture* _tex, u16 _left, u16 _top, u16 _right, u16 _bottom);
	// �e�N�X�`����z��ɒǉ�
	void 	VertexUVUpdate();								// D3DTLVERTEX��UV���X�V
	void 	Animation();                                  // �A�j��
	void 	AnimNext(); 									// �A�j����1�R�}�i�߂�
	void 	AnimPrev();                                   //       �V     �߂�
	void 	AnimReinitialize();							// �e�N�X�`���A�j��������ď�����
	void	AddHitArea(  const glm::vec2& _pt1, const glm::vec2& _pt2, const glm::vec2& _pt3, const glm::vec2& _pt4, const THitType _type );
	void	AddHitArea( const TPoint& _pt1,  const TPoint& _pt2,  const  TPoint& _pt3,  const  TPoint& _pt4,  THitType _type );
	void 	HitAreaReinitialize();						// �����蔻�������ď�����
	void	CalcBoundingBox( Rect& _rt);		   		// �o�E���f�B���O�{�b�N�X��`�̌v�Z
	void	HitAreaTransform( vector<THitArea>& _haa);       // �����蔻��̃g�����X�t�H�[��
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

