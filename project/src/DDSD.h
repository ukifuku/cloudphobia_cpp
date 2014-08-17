#pragma once

#include <string>
#include <dsound.h>
#include <vector>
#include "types.h"

using namespace std;

bool	DDSD_HaltOnError;       // {エラーが起きたらすぐにHaltする}
bool	DDSD_DebugMessage;      // {デバッグメッセージをファイルに書き出す}
bool	DDSD_ExceptOnError;     // {エラーが起きたら例外を発生する}

//デバッグオプション
// TDDSDDebugOption = set of (dsoHaltOnError, dsoDebugMessage, dsoExceptOnError);


//3D音源の減衰開始・減衰完了距離
struct TDDSDDistance 
{
	f32	Min;
	f32	Max;
};

//コーン角度
struct TDDSDConeAngle 
{
	u32	Inside;
	u32	Outside;
};

class TDDSDGenWave;
class TDDSDChannel;
class TDDSDCapture;
class TDDSD;

//ストリーム更新イベント
typedef void (*TDDSDUpdateEvent)(TDDSDGenWave*, TDDSDChannel*, u32, u32);

//オーディオストリーム更新用イベント監視スレッド
class TDDSDNotifyThread
{
private:
	s32 FPositionFlag;

    TDDSDGenWave* FOwner; //このスレッドを作り出したTDDSDGenWaveオブジェクト
	bool _GetPlay();
	TPoint _GetHalfPosition();
	void _SyncFunc();
public:
    bool IsTop;     
	bool Terminated;
	//現在、バッファの先頭に再生位置があるか
	TDDSDNotifyThread(TDDSDGenWave* owner);
	~TDDSDNotifyThread();
    void Execute();

};


//WAVファイルをカプセル化した物
class TDDSDGenWave
{
private:
    IDirectSoundBuffer8* FSoundBuffer;
    WAVEFORMATEX FWaveFormat;
    u32 FLength;                  //バッファの長さ
    TDDSD* FOwner;                   //オーナーとなるDDSDオブジェクト
    bool FIsStream;                //ストリームかそうでないか
    bool FUseFX;                   //FX対応か
    u32 FHalfPos;                   //真中のオフセット
	IDirectSoundNotify8* FDSNotify;    //Notify用オブジェクト
    HANDLE FEvent[3];// of THandle;    //イベントオブジェクトへのハンドル
    TDDSDNotifyThread* FNotifyThread;  //ストリームの場合、イベントの監視スレッド
    TDDSDUpdateEvent FOnUpdate;       //「バッファを更新しなさい」イベント
	HANDLE	m_threadId;
public:
	TDDSDChannel* FPlayer;             //このWaveDataを再生しているTDDSDChannelオブジェクト…ストリームの場合のみ有効

	TDDSDGenWave( TDDSD* OWner, const wstring& fname, bool is3D, bool useFX ); //WAVEファイルを読み込む
    TDDSDGenWave( TDDSD* OWner, const string& QDAname, const string& id, bool is3D, bool useFX); //QDAからWAVEファイルを読み込む
    TDDSDGenWave( TDDSD* OWner, u32 freq, u32 bps, bool isStereo, u32 length, bool is3D, bool useFX, bool isStream );

    ~TDDSDGenWave();
    void BlockCopy( u32 offset, void* src, u32 length);
	IDirectSoundBuffer8* Buffer() { return FSoundBuffer; }
	bool IsStream()	{ return FIsStream; }
	bool UseFX() { return FUseFX; }
	LPWAVEFORMATEX WaveFormat() { return &FWaveFormat; }
	u32 Length() { return FLength; }
	u32 GetHalfPos() {return FHalfPos; }              
	void Sync();                   //NotifyThreadから呼ばれて、ストリーム更新イベントを呼ぶルーチン…ほんとはSynchronizeずべきなんだけど…

};

//WAVファイル(普通の)
class TDDSDWaveData : public TDDSDGenWave
{
public:
    //共通コンストラクタ
    TDDSDWaveData( TDDSD* OWner, const wstring& fname, bool useFX = false);
    TDDSDWaveData( TDDSD* OWner, const string& QDAname, const string& id, bool useFX = false); //QDAからWAVEファイルを読み込む

	TDDSDWaveData( TDDSD* OWner, u32 freq, u32 bps, bool isStereo, u32 length, bool isStream, bool useFX = false);

    ~TDDSDWaveData();
};


//二次バッファをカプセル化した物
class TDDSDChannel 
{
private:
    IDirectSoundBuffer8* FSoundBuffer;
    IDirectSound3DBuffer* F3DBuffer;

    bool FIs3D;  //3DサウンドデータがWaveDataに指定されている
    TDDSD* FOwner; //オーナーとなるTDDSDコンポーネント
    s32 FPan;     //パンポット
    u32 FFreq;    //再生周波数
    u32 FSrcFreq; //元のWAVの周波数
    f64 FFreqRate; //元の周波数に対する再生周波数の割合
    s32 FVolume;  //ボリューム(1/100dB単位)
    TDDSDGenWave* FWaveData;
    //FParams3D:TDDSDParams3D;
    bool FIsStream; //ストリーミング対応WaveDataを載せているか
    //3D専用
    //void setParams3D(v:TDDSDParams3D);
public:
    TDDSDChannel(TDDSD* _owner);
    ~TDDSDChannel();
    void Play();
    void LoopPlay();
    void PlayAt(u32 ofs);    //バッファの任意のオフセットから再生
    void LoopPlayAt(u32 ofs);
    void Stop();
	IDirectSoundBuffer8* Buffer() { return FSoundBuffer; }
    //非3D用
	s32 GetPan() { return FPan; }  //パンニングは、3D化している場合無効
    //3D用
    //property Params3D:TDDSDParams3D read FParams3D write setParams3D;  //3Dパラメータ
    //両用
	u32 getStatus();
	void setFreqRate(f64 v);
	void setVolume(s32 v);
	void setPan(s32 v);
	void setWaveData(TDDSDGenWave* wav);
	TDDSDGenWave* GetWaveData(){ return FWaveData; }
	f64 GetFreqRate() { return FFreqRate; }
	s32 GetVolume() { return FVolume; }
	u32 SrcFreq() { return FSrcFreq; }
	bool Is3D() { return FIs3D; }
	bool IsStream() { return FIsStream; }
	u32 getPlayPosition();   //バッファの何バイト目を現在演奏中か
	void setPlayPosition(u32 pos);  //再生位置の指定
};

typedef vector<TDDSDChannel*> TDDSDChannelArray;

//DDSD本体
class TDDSD
{
private:
    IDirectSound8* FDSound;
    IDirectSoundBuffer* FPrimaryBuffer;
    //IDirectSound3DListener8* F3DListener;
    TDDSDChannelArray FChannels;          //各チャンネル
    bool FInitialized;                 //「サウンドカードが無い」フラグ
    bool FUse3D;                       //「3Dサウンドを使う」フラグ
    //TDDSDListener* FListener;
    //FDebugOption: TDDSDDebugOption;                 //ほかのアプリに制御が移っても、再生できるように
    //3D専用
    //void setListener(const v:TDDSDListener);
    //void SetDebugOption(const Value: TDDSDDebugOption);
public:
	u32 FChannelCount;                  //チャンネル数
	bool FStickyFocus;

	TDDSD(HWND _handle);
    ~TDDSD();
    bool forceInitialize(HWND _handle);     //DirectSoundの再初期化
	IDirectSound8* DSound() { return FDSound; }
	IDirectSoundBuffer* PrimaryBuffer() { return FPrimaryBuffer; }
	TDDSDChannel* GetChannels(s32 iCh);
	bool Initialized() { return FInitialized; }
    //property Listener:TDDSDListener read FListener write setListener;
    void SetPrimaryBufferFotmat(u32 freq, u32 bps, bool isStereo );

    //property DebugOption:TDDSDDebugOption read FDebugOption write SetDebugOption;
    //property Use3D:Boolean read FUse3D write FUse3D default False;
};
