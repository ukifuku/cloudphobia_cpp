#pragma once

#include "types.h"
#include <d3d9.h>
#include <d3d9types.h>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

using namespace std;

/****************************************************************************
*
* �f�o�b�O���b�Z�[�W�o�͗p
*
****************************************************************************/
 bool DGCarad_HaltOnError = true;        //��O���N�����炷����Halt����
 bool DGCarad_DebugMessage = true;       //�f�o�b�O���b�Z�[�W���t�@�C���ɏ����o��
 bool DGCarad_DetailedLog =  true;       //���O�̏o��(AllowDebugMessage:True�łȂ���Ώo�͂���Ȃ�)
 bool DGCarad_debugIndent = 0;            //�f�o�b�O���O�̃C���f���g



/****************************************************************************
*
* �R���|�[�l���g�̍��q
*
****************************************************************************/


class  TDGCarad;
class  TDGTexture;
class  TDGVertexBuffer;
class  TDGIndexBuffer;

typedef void (*TDGNotifyEvent)(void);

//�f�o�b�O�I�v�V����
enum TDGDebugOptionEnum {
	dgoHaltOnError	=	1,
	dgoDebugMessage =	2,
	dgoDetailedLog	=	4
};

typedef u32 TDGDebugOption;

struct TRGBQUAD {
	u8 rgbBlue;
	u8 rgbGreen;
	u8 rgbRed;
	u8 rgbReserved;
};

struct WindowInfo{
	HWND	m_handle;
	u32		m_left;
	u32		m_top;
	u32		m_width;
	u32		m_height;
	u32		m_clientWidth;
	u32		m_clientHeight;
};

// ���\�[�X���ʃN���X
class TDGCommonResource 
{
public:
	virtual void Cleanup() = 0;  
	virtual void Recover() = 0;                       
	virtual void Reset() = 0;                             
	virtual void AfterReset() = 0;                             
};

//���\�[�X�̏��c
//�����Ō����u���\�[�X�v�Ƃ����̂́AD3DDevice�̃��X�g�Ƃ��ɔ����Ė����Ȃ�\���̂���I�u�W�F�N�g�̎�
struct TDGResourceDesc
{
	u32 id;							//���\�[�X���ʎq
	string debugName;             //ID���ƕ�����Â炢�̂ŁA���\�[�X�����w�肳���āA�f�o�b�N���O�ɓf����悤�ɂ���
	TDGCommonResource* resource;  //�̐S�̃��\�[�X
};


//���̃��j�b�g�̍��q
class TDGCarad
{
private:
	WindowInfo	m_owner;
	IDirect3D9* FD3D;
	IDirect3DDevice9* FD3DDevice;
	vector<IDirect3DSurface9*> FBackBuffers;
	IDirect3DSurface9* FDepthStencilSurface;
	u32 FBackBufferCount;
	TDGDebugOption FDebugOption;

	bool FUseREFDevice;          //HEL��D��I�Ɏg��
	bool FUseSoftwareVertexProcessing; //SoftVP��D��I�Ɏg��

	bool FWindowed;              //�E�B���h�E���Ŏ��s���Ă�H
	D3DFORMAT FBackBufferFormat;    //�o�b�N�o�b�t�@���
	u32 FBackBufferWidth;
	u32 FBackBufferHeight;

	D3DPRESENT_PARAMETERS FLastPP;  //�Ō��CreateDevice�Ƃ�Reset�Ƃ��������̃p�����[�^
	u32 FAdapter;                  //���ݎg���Ă���A�_�v�^�ԍ�
	D3DDEVTYPE FDeviceType;         //HAL? REF?

	u32 NextResourceID;            //���\�[�X�Ǘ�
	vector<TDGResourceDesc*> Resources;
	void SetDebugOption(TDGDebugOption Value);
protected:
	void SetDisplayMode(u32 Adapter, bool windowed, D3DDISPLAYMODE mode,
		D3DMULTISAMPLE_TYPE multiSampleType, vector<D3DFORMAT>& depthStencilFmt,
		D3DSWAPEFFECT swapEffect, u32 presentationInterval);  //��ʃ��[�h�̂��܂����ݒ�

	void CleanupResources();                            //�e�N�X�`���EVB�EIB�̑S����
	void RecoverResources();                            //�e�N�X�`���EVB�EIB�̍Đ���
	void Loaded();
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	TDGCarad( WindowInfo& _owner );
	~TDGCarad();

	const WindowInfo& GetWindowInfo() { return m_owner; }
	IDirect3DSurface9* GetBackBuffers( s32 idx );

	//���\�[�X�Ǘ�
	u32 RegisterResouce(TDGCommonResource* res, string debugName = ""); //���\�[�X�̓o�^
	void UnregisterResource( u32 id );  //���\�[�X�̓o�^����

	//�f�o�C�X�ݒ�
	void CreateDevice(u32 Adapter, D3DDEVTYPE DeviceType,
		HWND hFocusWindow, u32 BehaviorFlags,
		D3DPRESENT_PARAMETERS& presentationParameters); //D3DDevice�̍쐬�Ɖ�ʂ̏�����
	void ReleaseDevice();  //D3DDevice�Ƃ��̉��
	void WindowMode(u32 width, u32 height, 
		vector<D3DFORMAT>& depthStencilFmt,
		u32 presentationInterval = D3DPRESENT_INTERVAL_DEFAULT,
		D3DSWAPEFFECT swapEffect = D3DSWAPEFFECT_DISCARD,
		D3DMULTISAMPLE_TYPE multiSampleType = D3DMULTISAMPLE_NONE,
		u32 Adapter = D3DADAPTER_DEFAULT);     //�K���ɏ�����
	void FullscreenMode(u32 width, u32 height, D3DFORMAT format,
		vector<D3DFORMAT>& depthStencilFmt,
		u32 refreshRate = 0,
		u32 presentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE,
		D3DSWAPEFFECT swapEffect = D3DSWAPEFFECT_DISCARD,
		D3DMULTISAMPLE_TYPE multiSampleType = D3DMULTISAMPLE_NONE,
		u32 Adapter = D3DADAPTER_DEFAULT);
	void Reset();  //Direct3DDevice�̃��Z�b�g�A�Ō�ɐݒ肵���v���[���e�[�V�����p�����[�^���Đݒ�

	//��ʃ��[�h�ɂ��Ă̏��
	D3DDISPLAYMODE GetCurrentDisplayMode(u32 Adapter);  //�w�肳�ꂽ�A�_�v�^�ł̌��݂̃f�B�X�v���C���[�h�𓾂�
	bool IsAvailableDisplayMode(u32 Adapter, u32 screenWidth, u32 screenHeight, u32 freq, D3DFORMAT fmt); //�w�肳�ꂽ�A�_�v�^�ŁA���̉�ʃ��[�h��������̂��`�F�b�N
	D3DFORMAT BestZSFormat( D3DDEVTYPE devType, vector<D3DFORMAT>& depthStencilFmt, D3DPRESENT_PARAMETERS& prep, u32 Adapter );
	//��ʃ��[�h
	u32 GetAdapter() { return FAdapter; }
	D3DDEVTYPE GetDeviceType() { return FDeviceType; }
	//    property Windowed:Boolean read FWindowed;

	u32	GetBackBufferWidth() {return FBackBufferWidth;}
	u32	GetBackBufferHeight() {return FBackBufferHeight;}
	D3DFORMAT	GetBackBufferFormat() {return FBackBufferFormat;}
	// property BackBufferFormat:D3DFORMAT read FBackBufferFormat;
	// property PresentationParameters:D3DPRESENT_PARAMETERS read FLastPP;

	//�o�b�t�@
	//      property BackBuffers[idx:Integer]:IDirect3DSurface9 read GetBackBuffers;
	//     property BackBufferCount:UINT read FBackBufferCount;
	//     property DepthStencilSurface:IDirect3DSurface9 read FDepthStencilSurface;

	//�@�\�g���c�Ƃ������K�{(^^;)
	IDirect3D9* GetD3D() { return FD3D; }
	IDirect3DDevice9* Get3DDevice() { return FD3DDevice; }

	//�e��ݒ�
	//      property UseREFDevice:Boolean read FUseREFDevice write FUseREFDevice;
	//     property UseSoftwareVertexProcessing:Boolean read FUseSoftwareVertexProcessing write FUseSoftwareVertexProcessing;
	//    property DebugOption:TDGDebugOption read FDebugOption write SetDebugOption;
};


//Keep/Recover�̕��@ ... Keep���Ȃ��A�t�@�C������AQDA����A�����̃X�g���[������ǂݒ����A
enum TDGKeepMethod {
	kmDontKeep,
	kmFromFile,
	kmFromQDA, 
	kmFromStream
};

//�s�N�Z�����Ƃ̏�����o�^���邽�߂̊֐�
typedef u32 (*TDGPerPixelOp)(TDGTexture*, u32, s32, s32, void*);
void LoadBMPStreamToTexture(IDirect3DTexture9* texture, istream& source );
void LoadBMPStreamToLockedRect(D3DLOCKED_RECT& LockedRect, Rect& lockedRegion, D3DSURFACE_DESC ddsd, istream& source);
u32 GetPixelBits( D3DFORMAT fmt );
bool IsARGBFormat(D3DFORMAT fmt);
D3DCOLOR PhysicalToARGB( u32 pcol, D3DFORMAT fmt);
u32 GainBit( u32 c, u32 nBit );
u32 ARGBToPhysical( u32 a, u32 r, u32 g, u32 b, D3DFORMAT fmt);
u32 D3DColorToPhysical( D3DCOLOR col, D3DFORMAT fmt);
//�e�N�X�`��
class TDGTexture : public TDGCommonResource
{
private:
	u32 ID;                        //DGCarad���Ǘ����邽�߂�ID
	string FDebugName;              //�f�o�b�O���O�ɓf���o�����Ƃ��̖��O
	TDGCarad* FOwner;                //�ʓ|�����Ă����DGCarad�I�u�W�F�N�g

	D3DFORMAT FFormat;              //�ӂ��[�܂���
	vector<D3DFORMAT> FormatList;  //�������ꂽ�Ƃ��̃t�H�[�}�b�g���X�g�B�F���ύX���ɍĐ��������Ƃ��Ɏg����
	u32 FWidth,FHeight;            //�ǂݍ��񂾉摜�̃T�C�Y
	u32 FTotalWidth,FTotalHeight;  //�e�N�X�`���S�̂̃T�C�Y
	f32 FU,FV;                   //�摜�̉E��������uv���W
	IDirect3DTexture9* FTexture;     //�J�u�Z�������Ă���e�N�X�`��
	u32 FUsage;                    //���̃e�N�X�`���̎g����
	D3DPOOL FPool;                  //���̃e�N�X�`���̃v�[���̂����
	D3DCOLOR FBorderColor;          //LoadFromFile�Ȃǂœǂݍ��ލۂ́A�摜�̌��Ԃ𖄂߂邽�߂̐F

	bool FLocked;                //���b�N���Ă�H
	D3DLOCKED_RECT FLockedRect;     //LOCKEDRECT���
	Rect FLockedRegion;            //���b�N�����͈�
	u32 FLockedLevel;              //���b�N�������x��
	u32 FPixelSize;                //1�s�N�Z���̃o�C�g��
	u32 FMipLevel;                 //�������ׂ�(����)�~�b�v�}�b�v�̃��x��

	bool FAutoKeep;              //������������H
	TDGKeepMethod KeepMethod;       //�ǂݍ��ݕ��@
	string KeepFilename;
	string KeepQDAName;
	string KeepQDAID;
	stringstream KeepStream;
	bool Recovering;
	u32 GetTexels(u32 level, s32 x, s32 y);
	void SetTexels(u32 level, s32 x, s32 y, u32 Value);             //���ݕ�����ƒ��H

protected:
	D3DFORMAT SearchBestFormat();             //FormatList�����o����A�g���钆�ōł��ǂ��t�H�[�}�b�g�𓾂�

	void Keep();                                   //���݂̃e�N�X�`���̓��e��ێ�����
	void CreateTexture(u32 w, u32 h);                //�e�N�X�`�������ۂɐ�������
	void Cleanup();                                //�e�N�X�`�����������(�𑜓x�ύX�O�ɌĂ�)
	void Recover();                                //�e�N�X�`�����Đ������āA�摜��ǂݒ���
	void Reset();                                  //�f�o�C�X�̃��Z�b�g�ɐ旧���āAPOOL_DEFAULT�ȃe�N�X�`�������
	void AfterReset();                             //�f�o�C�X�̃��Z�b�g��APOOL_DEFAULT�ȃe�N�X�`�����Đ���
public:
	TDGTexture(TDGCarad* AOwner, const D3DFORMAT formats[], u32 _usage = 0, D3DPOOL _pool = D3DPOOL_MANAGED, u32 mipLevel = 0, string debugName = ""); //�t�H�[�}�b�g�����]������ׂăe�N�X�`������
	~TDGTexture(); 

	//�t�@�C��I/O
	void LoadFromFile(string filename, string qdaName = "");   //BMP�ǂݍ���
	void LoadFromStream(istream& source);                       //BMP�̓������X�g���[������ǂݍ���
	//     void LoadFromTBitmap(TBitmap* source);                      //TBitmap�I�u�W�F�N�g����ǂݍ���

	void SaveToStream(ostream& dest);                        //�X�g���[���Ƀe�N�X�`���̓��e��32�r�b�gBMP�ŕۑ�

	//�e�N�Z������
	void PerPixelOperation(Rect& region, TDGPerPixelOp op, void* data); //�S���̃s�N�Z���ɁAop�Ŏw�肵���֐��ɂ��I�y���[�V�������s��
	void SetColorKey(Rect& region, D3DCOLOR col);      //col�Ɠ����F����=0�ɂ���
	void BlueToAlpha(Rect& region);                           //B�l�����l�ɃR�s�[����
	void Fill(Rect& region, D3DCOLOR col, bool maskA);     //�w�肳�ꂽ�F�œh��Ԃ��AmaskA=True�̎��A���`�����l���͓h��Ȃ�

	//�T�C�Y����
	void SetSize(u32 w, u32 h);        //�T�C�Y�̕ύX

	//���b�N
	bool LockRect(D3DLOCKED_RECT* _out, u32 level, Rect& rect, u32 flags);
	void UnlockRect();

	//�T�C�Y���
	u32	GetWidth() { return FWidth; }
	u32	GetHeight() { return FHeight; }
	D3DFORMAT GetFormat() { return FFormat; }
	//      property TotalWidth:UINT read FTotalWidth;
	//      property TotalHeight:UINT read FTotalHeight;
	f32	GetU() { return FU; }
	f32	GetV() { return FV; }
	//      property U:Single read FU;
	//      property V:Single read FV;

	//�s�N�Z��
	//     property Texels[level:UINT; x,y:Integer]:DWord read GetTexels write SetTexels;

	//�ݒ�
	//      property AutoKeep:Boolean read FAutoKeep write FAutoKeep;

	//���̑�
	IDirect3DTexture9* GetTexture() { return FTexture; }
	//     property Pool:D3DPOOL read FPool;
	//   property Usage:UINT read FUsage;
	//  property BorderColor:D3DColor read FBorderColor write FBorderColor;
};


class  TDGVertexBuffer : public TDGCommonResource
{
private:
	TDGCarad* FOwner;
	IDirect3DVertexBuffer9* FVBuf;

	bool FLocked;
	void* PLocked;    //�Ō�Ƀ��b�N�����Ƃ��ɃQ�b�g�����|�C���^
	u32 FVertexSize;   //�꒸�_������̃T�C�Y
	u32 FVertexCount;  //�i�[����Ă��钸�_��
	u32 FLockStart,FLockEnd; //���b�N���ꂽ�͈�

	u32 ID;            //DGCarad�ɂ���ĊǗ������ۂ�ID
	string FDebugName;  //�f�o�b�O���O�ɏo�͂���閼�O
	void* KeepBuf;    //Keep����ۂ̃o�b�t�@

	D3DPOOL FPool;
	u32 FUsage;
	u32 FFVF;

	bool FAutoKeep;   //True�̎��ACleanup���Ɏ����I��Keep����
protected:
	void Cleanup();                               //VertexBuffer���������
	void Recover();                               //VertexBuffer�𐶐����Ȃ���
	void Reset();                                 //�f�o�C�X�̃��Z�b�g�ɔ�����
	void AfterReset();                            //�f�o�C�X�����Z�b�g����I�����
	void Keep();                         //���݂̏�Ԃ�����Recover���ɕ�������
public:
	TDGVertexBuffer(TDGCarad* AOwner, u32 _vertexCount, u32 _vertexSize,
		u32 _Usage, u32 _FVF, D3DPOOL _Pool, string debugName = "");
	~TDGVertexBuffer();

	//���b�N
	void* Lock(u32 start, u32 count, u32 Flags);
	void Unlock();

	//���_�̓ǂݏ���
	void GetVertex(u32 idx, void* v);   //���_�͌^��������Ȃ��̂Ńv���p�e�B�ɏo���Ȃ���
	void SetVertex(u32 idx, void* v);

	//���_�f�[�^�ɂ��Ă̏��
	u32 GetFVF(){ return FFVF; }
	u32 GetVertexSize() { return FVertexSize; }
	u32 GetVertexCount(){ return FVertexCount; }
	void SetVertexCount(u32 Value);

	//�ݒ�
	//     property AutoKeep:Boolean read FAutoKeep write FAutoKeep;

	//���̑�
	IDirect3DVertexBuffer9* GetVertexBuffer() { return FVBuf; }
	D3DPOOL GetPool() { return FPool; }
	//   property Usage:DWORD read FUsage;
};


//IndexBuffer(2�o�C�g�C���f�N�X�̂ݑΉ�)
class TDGIndexBuffer : public TDGCommonResource
{
private:
	TDGCarad* FOwner;
	IDirect3DIndexBuffer9* FIBuf;

	u32 FCount;        //�i�[���Ă�C���f�N�X�̐�

	bool FLocked;
	void* PLocked;    //�Ō�Ƀ��b�N�����Ƃ��ɃQ�b�g�����|�C���^
	u32 FLockStart,FLockEnd; //���b�N���ꂽ�͈�

	u32 ID;            //DGCarad�ɂ���ĊǗ������ۂ�ID
	string FDebugName;  //�f�o�b�O���O�ɏo�͂���閼�O
	void* KeepBuf;    //Keep����ۂ̃o�b�t�@

	D3DPOOL FPool;
	u32 FUsage;

	bool FAutoKeep;   //True�̎��ACleanup���Ɏ����I��Keep����
	void SetCount(u32 Value);
protected:
	void Cleanup();                               //IndexBuffer���������
	void Recover();                               //IndexBuffer�𐶐����Ȃ���
	void Reset();                                 //�f�o�C�X�̃��Z�b�g�ɔ�����
	void AfterReset();                            //�f�o�C�X�����Z�b�g����I�����
	void Keep();                         //���݂̏�Ԃ�����Recover���ɕ�������
public:
	TDGIndexBuffer(TDGCarad* AOwner, u32 _indexCount, u32 _Usage,
		D3DPOOL _Pool, string debugName = "");
	~TDGIndexBuffer();

	//���b�N
	u16* Lock(u32 start, u32 count, u32 Flags);
	void Unlock();

	//�C���f�N�X�ɂ��Ă̏��
	u32 GetCount() { return FCount; }
	//      property Indices[idx:UINT]:Word read GetIndices write SetIndices;

	//�ݒ�
	void SetIndices(u32 idx, u16 Value);
	u16 GetIndices(u32 idx);
	//      property AutoKeep:Boolean read FAutoKeep write FAutoKeep;

	//���̑�
	IDirect3DIndexBuffer9* GetIndexBuffer() { return FIBuf; }
	//      property Pool:D3DPOOL read FPool;
	//      property Usage:DWORD read FUsage;
};

//TDGVertexShader = class
//  private
//    FOwner:TDGCarad;
//    FToken:Pointer;       //VS�̃g�[�N����
//    FAutoKeep: Boolean;
//    FVS: IDirect3DVertexShader9;
//    FDebugName:String;                               //�f�o�b�O���O�ɏo�͂���閼�O
//    ID:Integer;
//  void SetToken(const Value: Pointer);                                      //DGCarad���\�[�XID
//  protected
//    void Cleanup;                               //VS���������
//    void Recover;                               //VS�𐶐����Ȃ���
//    void Reset;                                 //�f�o�C�X�̃��Z�b�g�ɔ�����
//    void AfterReset;                            //�f�o�C�X�����Z�b�g����I�����
//    void Keep;                         //���݂̏�Ԃ�����Recover���ɕ�������
//  public
//    constructor Create(AOwner:TDGCarad; DebugName:String = '');
//    destructor Destroy; override;
//    //�t�@�C��I/O
//    void LoadFromFile(filename:String; qdaName:String = '');
//    void LoadFromStream(source:TStream; bytes:DWord);
//    //�ݒ�
//    property AutoKeep:Boolean read FAutoKeep write FAutoKeep;
//    //���g
//    property Token:Pointer read FToken write SetToken;
//    property VS:IDirect3DVertexShader9 read FVS;
//end;

//TDGPixelShader = class
//  private
//    FOwner:TDGCarad;
//    FToken:Pointer;       //VS�̃g�[�N����
//    FAutoKeep: Boolean;
//    FPS: IDirect3DPixelShader9;
//    FDebugName:String;                               //�f�o�b�O���O�ɏo�͂���閼�O
//    ID:Integer;
//  void SetToken(const Value: Pointer);                                      //DGCarad���\�[�XID
//  protected
//    void Cleanup;                               //PS���������
//    void Recover;                               //PS�𐶐����Ȃ���
//    void Reset;                                 //�f�o�C�X�̃��Z�b�g�ɔ�����
//    void AfterReset;                            //�f�o�C�X�����Z�b�g����I�����
//    void Keep;                         //���݂̏�Ԃ�����Recover���ɕ�������
//  public
//    constructor Create(AOwner:TDGCarad; DebugName:String = '');
//    destructor Destroy; override;
//    //�t�@�C��I/O
//    void LoadFromFile(filename:String; qdaName:String = '');
//    void LoadFromStream(source:TStream; bytes:DWord);
//    //�ݒ�
//    property AutoKeep:Boolean read FAutoKeep write FAutoKeep;
//    //���g
//    property Token:Pointer read FToken write SetToken;
//    property PS:IDirect3DPixelShader9 read FPS;
//end;

////VertexDeclaration�I�u�W�F�N�g
////���g�̎����L�[�v�͏��on
//TDGVertexDeclaration = class
//  private
//    FOwner:TDGCarad;
//    FElements:Array of D3DVERTEXELEMENT9;
//    FVDecl: IDirect3DVertexDeclaration9;
//    FDebugName:String;                               //�f�o�b�O���O�ɏo�͂���閼�O
//    ID:Integer;
//    FCount: Integer;
//    function GetElements(idx: Integer): D3DVERTEXELEMENT9;
//  protected
//    void Cleanup;                               //IndexBuffer���������
//    void Recover;                               //IndexBuffer�𐶐����Ȃ���
//    void Reset;                                 //�f�o�C�X�̃��Z�b�g�ɔ�����
//    void AfterReset;                            //�f�o�C�X�����Z�b�g����I�����
//    void Keep;                         //���݂̏�Ԃ�����Recover���ɕ�������
//  public
//    constructor Create(AOwner:TDGCarad; elem: array of D3DVERTEXELEMENT9; DebugName:String = '');
//    destructor Destroy; override;
//    property VDecl:IDirect3DVertexDeclaration9 read FVDecl;
//    property Elements[idx:Integer]:D3DVERTEXELEMENT9 read GetElements;
//    property Count:Integer read FCount;
//end;

/****************************************************************************
*
* ���֗��֐��Ƃ�
*
****************************************************************************/

//void DGCarad_PutDebugMessage(string mes);  //�f�o�b�O���O�ɏo��
//
////�w���p�[�֐�
//string D3DFORMATToString(D3DFORMAT fmt);               //�e�N�X�`���t�H�[�}�b�g�𕶎���ɂ���
//u32 D3DColorToPhysical(D3DCOLOR col, D3DFORMAT fmt); //�e�N�X�`���t�H�[�}�b�g�ɂ��������ĐF��ϊ�
//u32 ARGBToPhysical(u32 a, u32 r, u32 g, u32 b, D3DFORMAT fmt);    //���Ɠ���
//D3DCOLOR PhysicalToARGB(u32 pcol, D3DFORMAT fmt);    //�e�N�X�`���̐F�����AARGB�`���ɕϊ�
//bool IsARGBFormat(D3DFORMAT fmt);                   //�F�����܂ރe�N�X�`���t�H�[�}�b�g���ǂ������f
//u32 GetPixelBits(D3DFORMAT fmt);                      //�e�N�X�`���̂P�s�N�Z��������̃r�b�g���𓾂�
//
////�e�N�X�`���ɃX�g���[������BMP��ǂݍ���
//void LoadBMPStreamToTexture(IDirect3DTexture9* texture, stringstream& source);
//
////���b�N�����͈͂�BMP��ǂݍ���
//void LoadBMPStreamToLockedRect(D3DLOCKED_RECT LockedRect, Rect& lockedRegion, D3DSURFACE_DESC ddsd, stringstream& source);
//

/****************************************************************************
*
* ���֗��萔�Ƃ�
*
****************************************************************************/

/////�e�N�X�`���t�H�[�}�b�g
//�Ȃ�ł������̂œK���Ȃ́A���͂����
const D3DFORMAT DGFMT_RGB[] = {D3DFMT_X8R8G8B8, D3DFMT_R8G8B8, D3DFMT_R5G6B5, D3DFMT_X1R5G5B5};
//�Ȃ�ǂ������̂Ń��t��
const D3DFORMAT DGFMT_ARGB[] = {D3DFMT_A8R8G8B8, D3DFMT_A4R4G4B4, D3DFMT_A1R5G5B5};
//16�r�b�g�̂ŁA���i�V
const D3DFORMAT DGFMT_HighColor[] = {D3DFMT_R5G6B5, D3DFMT_X1R5G5B5};
//16�r�b�g�̂ŁA���A��
const D3DFORMAT DGFMT_HighColorA[] = {D3DFMT_A4R4G4B4, D3DFMT_A1R5G5B5};
//�Ȃ�ł���������TrueColor�A���͂����
const D3DFORMAT DGFMT_TrueColor[] = {D3DFMT_X8R8G8B8, D3DFMT_R8G8B8};

/////�f�v�X�X�e���V���o�b�t�@�t�H�[�}�b�g
//�Ȃ�ł�����������オ���
const D3DFORMAT DGFMT_ZCheap[] = {D3DFMT_D16, D3DFMT_D16_LOCKABLE, D3DFMT_D32};
//�ł����32
const D3DFORMAT DGFMT_ZDetail[] = {D3DFMT_D32, D3DFMT_D24X8, D3DFMT_D16_LOCKABLE, D3DFMT_D16};
//�X�e���V���~����
const D3DFORMAT DGFMT_ZStencil[]  = {D3DFMT_D24S8, D3DFMT_D24X4S4, D3DFMT_D15S1};

/////�o���v�}�b�v�̃t�H�[�}�b�g
//�P�Ȃ�o���v�A���~�i���X�t���ł�������
const D3DFORMAT DGFMT_Bump[] =  {D3DFMT_V8U8, D3DFMT_X8L8V8U8, D3DFMT_L6V5U5};
//���i�~���X�t���o���v
const D3DFORMAT DGFMT_BumpL[] = {D3DFMT_X8L8V8U8, D3DFMT_L6V5U5};

