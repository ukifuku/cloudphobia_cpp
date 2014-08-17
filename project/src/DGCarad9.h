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
* デバッグメッセージ出力用
*
****************************************************************************/
 bool DGCarad_HaltOnError = true;        //例外が起きたらすぐにHaltする
 bool DGCarad_DebugMessage = true;       //デバッグメッセージをファイルに書き出す
 bool DGCarad_DetailedLog =  true;       //ログの出力(AllowDebugMessage:Trueでなければ出力されない)
 bool DGCarad_debugIndent = 0;            //デバッグログのインデント



/****************************************************************************
*
* コンポーネントの骨子
*
****************************************************************************/


class  TDGCarad;
class  TDGTexture;
class  TDGVertexBuffer;
class  TDGIndexBuffer;

typedef void (*TDGNotifyEvent)(void);

//デバッグオプション
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

// リソース共通クラス
class TDGCommonResource 
{
public:
	virtual void Cleanup() = 0;  
	virtual void Recover() = 0;                       
	virtual void Reset() = 0;                             
	virtual void AfterReset() = 0;                             
};

//リソースの情報…
//ここで言う「リソース」というのは、D3DDeviceのロストとかに伴って無くなる可能性のあるオブジェクトの事
struct TDGResourceDesc
{
	u32 id;							//リソース識別子
	string debugName;             //IDだと分かりづらいので、リソース名を指定させて、デバックログに吐けるようにする
	TDGCommonResource* resource;  //肝心のリソース
};


//このユニットの骨子
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

	bool FUseREFDevice;          //HELを優先的に使う
	bool FUseSoftwareVertexProcessing; //SoftVPを優先的に使う

	bool FWindowed;              //ウィンドウ内で実行してる？
	D3DFORMAT FBackBufferFormat;    //バックバッファ情報
	u32 FBackBufferWidth;
	u32 FBackBufferHeight;

	D3DPRESENT_PARAMETERS FLastPP;  //最後にCreateDeviceとかResetとかした時のパラメータ
	u32 FAdapter;                  //現在使っているアダプタ番号
	D3DDEVTYPE FDeviceType;         //HAL? REF?

	u32 NextResourceID;            //リソース管理
	vector<TDGResourceDesc*> Resources;
	void SetDebugOption(TDGDebugOption Value);
protected:
	void SetDisplayMode(u32 Adapter, bool windowed, D3DDISPLAYMODE mode,
		D3DMULTISAMPLE_TYPE multiSampleType, vector<D3DFORMAT>& depthStencilFmt,
		D3DSWAPEFFECT swapEffect, u32 presentationInterval);  //画面モードのおまかせ設定

	void CleanupResources();                            //テクスチャ・VB・IBの全消去
	void RecoverResources();                            //テクスチャ・VB・IBの再生成
	void Loaded();
public:
	//コンストラクタ・デストラクタ
	TDGCarad( WindowInfo& _owner );
	~TDGCarad();

	const WindowInfo& GetWindowInfo() { return m_owner; }
	IDirect3DSurface9* GetBackBuffers( s32 idx );

	//リソース管理
	u32 RegisterResouce(TDGCommonResource* res, string debugName = ""); //リソースの登録
	void UnregisterResource( u32 id );  //リソースの登録解除

	//デバイス設定
	void CreateDevice(u32 Adapter, D3DDEVTYPE DeviceType,
		HWND hFocusWindow, u32 BehaviorFlags,
		D3DPRESENT_PARAMETERS& presentationParameters); //D3DDeviceの作成と画面の初期化
	void ReleaseDevice();  //D3DDeviceとかの解放
	void WindowMode(u32 width, u32 height, 
		vector<D3DFORMAT>& depthStencilFmt,
		u32 presentationInterval = D3DPRESENT_INTERVAL_DEFAULT,
		D3DSWAPEFFECT swapEffect = D3DSWAPEFFECT_DISCARD,
		D3DMULTISAMPLE_TYPE multiSampleType = D3DMULTISAMPLE_NONE,
		u32 Adapter = D3DADAPTER_DEFAULT);     //適当に初期化
	void FullscreenMode(u32 width, u32 height, D3DFORMAT format,
		vector<D3DFORMAT>& depthStencilFmt,
		u32 refreshRate = 0,
		u32 presentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE,
		D3DSWAPEFFECT swapEffect = D3DSWAPEFFECT_DISCARD,
		D3DMULTISAMPLE_TYPE multiSampleType = D3DMULTISAMPLE_NONE,
		u32 Adapter = D3DADAPTER_DEFAULT);
	void Reset();  //Direct3DDeviceのリセット、最後に設定したプレゼンテーションパラメータを再設定

	//画面モードについての情報
	D3DDISPLAYMODE GetCurrentDisplayMode(u32 Adapter);  //指定されたアダプタでの現在のディスプレイモードを得る
	bool IsAvailableDisplayMode(u32 Adapter, u32 screenWidth, u32 screenHeight, u32 freq, D3DFORMAT fmt); //指定されたアダプタで、その画面モードがつかえるのかチェック
	D3DFORMAT BestZSFormat( D3DDEVTYPE devType, vector<D3DFORMAT>& depthStencilFmt, D3DPRESENT_PARAMETERS& prep, u32 Adapter );
	//画面モード
	u32 GetAdapter() { return FAdapter; }
	D3DDEVTYPE GetDeviceType() { return FDeviceType; }
	//    property Windowed:Boolean read FWindowed;

	u32	GetBackBufferWidth() {return FBackBufferWidth;}
	u32	GetBackBufferHeight() {return FBackBufferHeight;}
	D3DFORMAT	GetBackBufferFormat() {return FBackBufferFormat;}
	// property BackBufferFormat:D3DFORMAT read FBackBufferFormat;
	// property PresentationParameters:D3DPRESENT_PARAMETERS read FLastPP;

	//バッファ
	//      property BackBuffers[idx:Integer]:IDirect3DSurface9 read GetBackBuffers;
	//     property BackBufferCount:UINT read FBackBufferCount;
	//     property DepthStencilSurface:IDirect3DSurface9 read FDepthStencilSurface;

	//機能拡張…というより必須(^^;)
	IDirect3D9* GetD3D() { return FD3D; }
	IDirect3DDevice9* Get3DDevice() { return FD3DDevice; }

	//各種設定
	//      property UseREFDevice:Boolean read FUseREFDevice write FUseREFDevice;
	//     property UseSoftwareVertexProcessing:Boolean read FUseSoftwareVertexProcessing write FUseSoftwareVertexProcessing;
	//    property DebugOption:TDGDebugOption read FDebugOption write SetDebugOption;
};


//Keep/Recoverの方法 ... Keepしない、ファイルから、QDAから、内蔵のストリームから読み直す、
enum TDGKeepMethod {
	kmDontKeep,
	kmFromFile,
	kmFromQDA, 
	kmFromStream
};

//ピクセルごとの処理を登録するための関数
typedef u32 (*TDGPerPixelOp)(TDGTexture*, u32, s32, s32, void*);
void LoadBMPStreamToTexture(IDirect3DTexture9* texture, istream& source );
void LoadBMPStreamToLockedRect(D3DLOCKED_RECT& LockedRect, Rect& lockedRegion, D3DSURFACE_DESC ddsd, istream& source);
u32 GetPixelBits( D3DFORMAT fmt );
bool IsARGBFormat(D3DFORMAT fmt);
D3DCOLOR PhysicalToARGB( u32 pcol, D3DFORMAT fmt);
u32 GainBit( u32 c, u32 nBit );
u32 ARGBToPhysical( u32 a, u32 r, u32 g, u32 b, D3DFORMAT fmt);
u32 D3DColorToPhysical( D3DCOLOR col, D3DFORMAT fmt);
//テクスチャ
class TDGTexture : public TDGCommonResource
{
private:
	u32 ID;                        //DGCaradが管理するためのID
	string FDebugName;              //デバッグログに吐き出されるときの名前
	TDGCarad* FOwner;                //面倒を見てくれるDGCaradオブジェクト

	D3DFORMAT FFormat;              //ふぉーまっと
	vector<D3DFORMAT> FormatList;  //生成されたときのフォーマットリスト。色数変更時に再生成されるときに使われる
	u32 FWidth,FHeight;            //読み込んだ画像のサイズ
	u32 FTotalWidth,FTotalHeight;  //テクスチャ全体のサイズ
	f32 FU,FV;                   //画像の右下を示すuv座標
	IDirect3DTexture9* FTexture;     //カブセル化しているテクスチャ
	u32 FUsage;                    //このテクスチャの使い道
	D3DPOOL FPool;                  //このテクスチャのプールのされ方
	D3DCOLOR FBorderColor;          //LoadFromFileなどで読み込む際の、画像の隙間を埋めるための色

	bool FLocked;                //ロックしてる？
	D3DLOCKED_RECT FLockedRect;     //LOCKEDRECT情報
	Rect FLockedRegion;            //ロックした範囲
	u32 FLockedLevel;              //ロックしたレベル
	u32 FPixelSize;                //1ピクセルのバイト数
	u32 FMipLevel;                 //生成すべき(した)ミップマップのレベル

	bool FAutoKeep;              //自動復元する？
	TDGKeepMethod KeepMethod;       //読み込み方法
	string KeepFilename;
	string KeepQDAName;
	string KeepQDAID;
	stringstream KeepStream;
	bool Recovering;
	u32 GetTexels(u32 level, s32 x, s32 y);
	void SetTexels(u32 level, s32 x, s32 y, u32 Value);             //現在復元作業中？

protected:
	D3DFORMAT SearchBestFormat();             //FormatListメンバから、使える中で最も良いフォーマットを得る

	void Keep();                                   //現在のテクスチャの内容を保持する
	void CreateTexture(u32 w, u32 h);                //テクスチャを実際に生成する
	void Cleanup();                                //テクスチャを解放する(解像度変更前に呼ぶ)
	void Recover();                                //テクスチャを再生成して、画像を読み直す
	void Reset();                                  //デバイスのリセットに先立って、POOL_DEFAULTなテクスチャを解放
	void AfterReset();                             //デバイスのリセット後、POOL_DEFAULTなテクスチャを再生成
public:
	TDGTexture(TDGCarad* AOwner, const D3DFORMAT formats[], u32 _usage = 0, D3DPOOL _pool = D3DPOOL_MANAGED, u32 mipLevel = 0, string debugName = ""); //フォーマットを第一希望から並べてテクスチャ生成
	~TDGTexture(); 

	//ファイルI/O
	void LoadFromFile(string filename, string qdaName = "");   //BMP読み込み
	void LoadFromStream(istream& source);                       //BMPの入ったストリームから読み込み
	//     void LoadFromTBitmap(TBitmap* source);                      //TBitmapオブジェクトから読み込み

	void SaveToStream(ostream& dest);                        //ストリームにテクスチャの内容を32ビットBMPで保存

	//テクセル操作
	void PerPixelOperation(Rect& region, TDGPerPixelOp op, void* data); //全部のピクセルに、opで指定した関数によるオペレーションを行う
	void SetColorKey(Rect& region, D3DCOLOR col);      //colと同じ色をα=0にする
	void BlueToAlpha(Rect& region);                           //B値をα値にコピーする
	void Fill(Rect& region, D3DCOLOR col, bool maskA);     //指定された色で塗りつぶす、maskA=Trueの時、αチャンネルは塗らない

	//サイズ操作
	void SetSize(u32 w, u32 h);        //サイズの変更

	//ロック
	bool LockRect(D3DLOCKED_RECT* _out, u32 level, Rect& rect, u32 flags);
	void UnlockRect();

	//サイズ情報
	u32	GetWidth() { return FWidth; }
	u32	GetHeight() { return FHeight; }
	D3DFORMAT GetFormat() { return FFormat; }
	//      property TotalWidth:UINT read FTotalWidth;
	//      property TotalHeight:UINT read FTotalHeight;
	f32	GetU() { return FU; }
	f32	GetV() { return FV; }
	//      property U:Single read FU;
	//      property V:Single read FV;

	//ピクセル
	//     property Texels[level:UINT; x,y:Integer]:DWord read GetTexels write SetTexels;

	//設定
	//      property AutoKeep:Boolean read FAutoKeep write FAutoKeep;

	//その他
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
	void* PLocked;    //最後にロックしたときにゲットしたポインタ
	u32 FVertexSize;   //一頂点当たりのサイズ
	u32 FVertexCount;  //格納されている頂点数
	u32 FLockStart,FLockEnd; //ロックされた範囲

	u32 ID;            //DGCaradによって管理される際のID
	string FDebugName;  //デバッグログに出力される名前
	void* KeepBuf;    //Keepする際のバッファ

	D3DPOOL FPool;
	u32 FUsage;
	u32 FFVF;

	bool FAutoKeep;   //Trueの時、Cleanup時に自動的にKeepする
protected:
	void Cleanup();                               //VertexBufferを解放する
	void Recover();                               //VertexBufferを生成しなおす
	void Reset();                                 //デバイスのリセットに備える
	void AfterReset();                            //デバイスがリセットされ終わった
	void Keep();                         //現在の状態を次回Recover時に復元する
public:
	TDGVertexBuffer(TDGCarad* AOwner, u32 _vertexCount, u32 _vertexSize,
		u32 _Usage, u32 _FVF, D3DPOOL _Pool, string debugName = "");
	~TDGVertexBuffer();

	//ロック
	void* Lock(u32 start, u32 count, u32 Flags);
	void Unlock();

	//頂点の読み書き
	void GetVertex(u32 idx, void* v);   //頂点は型が分かんないのでプロパティに出来ないの
	void SetVertex(u32 idx, void* v);

	//頂点データについての情報
	u32 GetFVF(){ return FFVF; }
	u32 GetVertexSize() { return FVertexSize; }
	u32 GetVertexCount(){ return FVertexCount; }
	void SetVertexCount(u32 Value);

	//設定
	//     property AutoKeep:Boolean read FAutoKeep write FAutoKeep;

	//その他
	IDirect3DVertexBuffer9* GetVertexBuffer() { return FVBuf; }
	D3DPOOL GetPool() { return FPool; }
	//   property Usage:DWORD read FUsage;
};


//IndexBuffer(2バイトインデクスのみ対応)
class TDGIndexBuffer : public TDGCommonResource
{
private:
	TDGCarad* FOwner;
	IDirect3DIndexBuffer9* FIBuf;

	u32 FCount;        //格納してるインデクスの数

	bool FLocked;
	void* PLocked;    //最後にロックしたときにゲットしたポインタ
	u32 FLockStart,FLockEnd; //ロックされた範囲

	u32 ID;            //DGCaradによって管理される際のID
	string FDebugName;  //デバッグログに出力される名前
	void* KeepBuf;    //Keepする際のバッファ

	D3DPOOL FPool;
	u32 FUsage;

	bool FAutoKeep;   //Trueの時、Cleanup時に自動的にKeepする
	void SetCount(u32 Value);
protected:
	void Cleanup();                               //IndexBufferを解放する
	void Recover();                               //IndexBufferを生成しなおす
	void Reset();                                 //デバイスのリセットに備える
	void AfterReset();                            //デバイスがリセットされ終わった
	void Keep();                         //現在の状態を次回Recover時に復元する
public:
	TDGIndexBuffer(TDGCarad* AOwner, u32 _indexCount, u32 _Usage,
		D3DPOOL _Pool, string debugName = "");
	~TDGIndexBuffer();

	//ロック
	u16* Lock(u32 start, u32 count, u32 Flags);
	void Unlock();

	//インデクスについての情報
	u32 GetCount() { return FCount; }
	//      property Indices[idx:UINT]:Word read GetIndices write SetIndices;

	//設定
	void SetIndices(u32 idx, u16 Value);
	u16 GetIndices(u32 idx);
	//      property AutoKeep:Boolean read FAutoKeep write FAutoKeep;

	//その他
	IDirect3DIndexBuffer9* GetIndexBuffer() { return FIBuf; }
	//      property Pool:D3DPOOL read FPool;
	//      property Usage:DWORD read FUsage;
};

//TDGVertexShader = class
//  private
//    FOwner:TDGCarad;
//    FToken:Pointer;       //VSのトークン列
//    FAutoKeep: Boolean;
//    FVS: IDirect3DVertexShader9;
//    FDebugName:String;                               //デバッグログに出力される名前
//    ID:Integer;
//  void SetToken(const Value: Pointer);                                      //DGCaradリソースID
//  protected
//    void Cleanup;                               //VSを解放する
//    void Recover;                               //VSを生成しなおす
//    void Reset;                                 //デバイスのリセットに備える
//    void AfterReset;                            //デバイスがリセットされ終わった
//    void Keep;                         //現在の状態を次回Recover時に復元する
//  public
//    constructor Create(AOwner:TDGCarad; DebugName:String = '');
//    destructor Destroy; override;
//    //ファイルI/O
//    void LoadFromFile(filename:String; qdaName:String = '');
//    void LoadFromStream(source:TStream; bytes:DWord);
//    //設定
//    property AutoKeep:Boolean read FAutoKeep write FAutoKeep;
//    //中身
//    property Token:Pointer read FToken write SetToken;
//    property VS:IDirect3DVertexShader9 read FVS;
//end;

//TDGPixelShader = class
//  private
//    FOwner:TDGCarad;
//    FToken:Pointer;       //VSのトークン列
//    FAutoKeep: Boolean;
//    FPS: IDirect3DPixelShader9;
//    FDebugName:String;                               //デバッグログに出力される名前
//    ID:Integer;
//  void SetToken(const Value: Pointer);                                      //DGCaradリソースID
//  protected
//    void Cleanup;                               //PSを解放する
//    void Recover;                               //PSを生成しなおす
//    void Reset;                                 //デバイスのリセットに備える
//    void AfterReset;                            //デバイスがリセットされ終わった
//    void Keep;                         //現在の状態を次回Recover時に復元する
//  public
//    constructor Create(AOwner:TDGCarad; DebugName:String = '');
//    destructor Destroy; override;
//    //ファイルI/O
//    void LoadFromFile(filename:String; qdaName:String = '');
//    void LoadFromStream(source:TStream; bytes:DWord);
//    //設定
//    property AutoKeep:Boolean read FAutoKeep write FAutoKeep;
//    //中身
//    property Token:Pointer read FToken write SetToken;
//    property PS:IDirect3DPixelShader9 read FPS;
//end;

////VertexDeclarationオブジェクト
////中身の自動キープは常にon
//TDGVertexDeclaration = class
//  private
//    FOwner:TDGCarad;
//    FElements:Array of D3DVERTEXELEMENT9;
//    FVDecl: IDirect3DVertexDeclaration9;
//    FDebugName:String;                               //デバッグログに出力される名前
//    ID:Integer;
//    FCount: Integer;
//    function GetElements(idx: Integer): D3DVERTEXELEMENT9;
//  protected
//    void Cleanup;                               //IndexBufferを解放する
//    void Recover;                               //IndexBufferを生成しなおす
//    void Reset;                                 //デバイスのリセットに備える
//    void AfterReset;                            //デバイスがリセットされ終わった
//    void Keep;                         //現在の状態を次回Recover時に復元する
//  public
//    constructor Create(AOwner:TDGCarad; elem: array of D3DVERTEXELEMENT9; DebugName:String = '');
//    destructor Destroy; override;
//    property VDecl:IDirect3DVertexDeclaration9 read FVDecl;
//    property Elements[idx:Integer]:D3DVERTEXELEMENT9 read GetElements;
//    property Count:Integer read FCount;
//end;

/****************************************************************************
*
* お便利関数とか
*
****************************************************************************/

//void DGCarad_PutDebugMessage(string mes);  //デバッグログに出力
//
////ヘルパー関数
//string D3DFORMATToString(D3DFORMAT fmt);               //テクスチャフォーマットを文字列にする
//u32 D3DColorToPhysical(D3DCOLOR col, D3DFORMAT fmt); //テクスチャフォーマットにしたがって色を変換
//u32 ARGBToPhysical(u32 a, u32 r, u32 g, u32 b, D3DFORMAT fmt);    //↑と同じ
//D3DCOLOR PhysicalToARGB(u32 pcol, D3DFORMAT fmt);    //テクスチャの色情報を、ARGB形式に変換
//bool IsARGBFormat(D3DFORMAT fmt);                   //色情報を含むテクスチャフォーマットかどうか判断
//u32 GetPixelBits(D3DFORMAT fmt);                      //テクスチャの１ピクセルあたりのビット数を得る
//
////テクスチャにストリームからBMPを読み込む
//void LoadBMPStreamToTexture(IDirect3DTexture9* texture, stringstream& source);
//
////ロックした範囲にBMPを読み込む
//void LoadBMPStreamToLockedRect(D3DLOCKED_RECT LockedRect, Rect& lockedRegion, D3DSURFACE_DESC ddsd, stringstream& source);
//

/****************************************************************************
*
* お便利定数とか
*
****************************************************************************/

/////テクスチャフォーマット
//なんでもいいので適当なの、αはいらん
const D3DFORMAT DGFMT_RGB[] = {D3DFMT_X8R8G8B8, D3DFMT_R8G8B8, D3DFMT_R5G6B5, D3DFMT_X1R5G5B5};
//なんどもいいのでα付き
const D3DFORMAT DGFMT_ARGB[] = {D3DFMT_A8R8G8B8, D3DFMT_A4R4G4B4, D3DFMT_A1R5G5B5};
//16ビットので、αナシ
const D3DFORMAT DGFMT_HighColor[] = {D3DFMT_R5G6B5, D3DFMT_X1R5G5B5};
//16ビットので、αアリ
const D3DFORMAT DGFMT_HighColorA[] = {D3DFMT_A4R4G4B4, D3DFMT_A1R5G5B5};
//なんでもいいからTrueColor、αはいらん
const D3DFORMAT DGFMT_TrueColor[] = {D3DFMT_X8R8G8B8, D3DFMT_R8G8B8};

/////デプスステンシルバッファフォーマット
//なんでもいいから安上がりの
const D3DFORMAT DGFMT_ZCheap[] = {D3DFMT_D16, D3DFMT_D16_LOCKABLE, D3DFMT_D32};
//できれば32
const D3DFORMAT DGFMT_ZDetail[] = {D3DFMT_D32, D3DFMT_D24X8, D3DFMT_D16_LOCKABLE, D3DFMT_D16};
//ステンシル欲しい
const D3DFORMAT DGFMT_ZStencil[]  = {D3DFMT_D24S8, D3DFMT_D24X4S4, D3DFMT_D15S1};

/////バンプマップのフォーマット
//単なるバンプ、ルミナンス付きでもいいや
const D3DFORMAT DGFMT_Bump[] =  {D3DFMT_V8U8, D3DFMT_X8L8V8U8, D3DFMT_L6V5U5};
//ルナミンス付きバンプ
const D3DFORMAT DGFMT_BumpL[] = {D3DFMT_X8L8V8U8, D3DFMT_L6V5U5};

