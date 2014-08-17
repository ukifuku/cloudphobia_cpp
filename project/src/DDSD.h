#pragma once

#include <string>
#include <dsound.h>
#include <vector>
#include "types.h"

using namespace std;

bool	DDSD_HaltOnError;       // {�G���[���N�����炷����Halt����}
bool	DDSD_DebugMessage;      // {�f�o�b�O���b�Z�[�W���t�@�C���ɏ����o��}
bool	DDSD_ExceptOnError;     // {�G���[���N�������O�𔭐�����}

//�f�o�b�O�I�v�V����
// TDDSDDebugOption = set of (dsoHaltOnError, dsoDebugMessage, dsoExceptOnError);


//3D�����̌����J�n�E������������
struct TDDSDDistance 
{
	f32	Min;
	f32	Max;
};

//�R�[���p�x
struct TDDSDConeAngle 
{
	u32	Inside;
	u32	Outside;
};

class TDDSDGenWave;
class TDDSDChannel;
class TDDSDCapture;
class TDDSD;

//�X�g���[���X�V�C�x���g
typedef void (*TDDSDUpdateEvent)(TDDSDGenWave*, TDDSDChannel*, u32, u32);

//�I�[�f�B�I�X�g���[���X�V�p�C�x���g�Ď��X���b�h
class TDDSDNotifyThread
{
private:
	s32 FPositionFlag;

    TDDSDGenWave* FOwner; //���̃X���b�h�����o����TDDSDGenWave�I�u�W�F�N�g
	bool _GetPlay();
	TPoint _GetHalfPosition();
	void _SyncFunc();
public:
    bool IsTop;     
	bool Terminated;
	//���݁A�o�b�t�@�̐擪�ɍĐ��ʒu�����邩
	TDDSDNotifyThread(TDDSDGenWave* owner);
	~TDDSDNotifyThread();
    void Execute();

};


//WAV�t�@�C�����J�v�Z����������
class TDDSDGenWave
{
private:
    IDirectSoundBuffer8* FSoundBuffer;
    WAVEFORMATEX FWaveFormat;
    u32 FLength;                  //�o�b�t�@�̒���
    TDDSD* FOwner;                   //�I�[�i�[�ƂȂ�DDSD�I�u�W�F�N�g
    bool FIsStream;                //�X�g���[���������łȂ���
    bool FUseFX;                   //FX�Ή���
    u32 FHalfPos;                   //�^���̃I�t�Z�b�g
	IDirectSoundNotify8* FDSNotify;    //Notify�p�I�u�W�F�N�g
    HANDLE FEvent[3];// of THandle;    //�C�x���g�I�u�W�F�N�g�ւ̃n���h��
    TDDSDNotifyThread* FNotifyThread;  //�X�g���[���̏ꍇ�A�C�x���g�̊Ď��X���b�h
    TDDSDUpdateEvent FOnUpdate;       //�u�o�b�t�@���X�V���Ȃ����v�C�x���g
	HANDLE	m_threadId;
public:
	TDDSDChannel* FPlayer;             //����WaveData���Đ����Ă���TDDSDChannel�I�u�W�F�N�g�c�X�g���[���̏ꍇ�̂ݗL��

	TDDSDGenWave( TDDSD* OWner, const wstring& fname, bool is3D, bool useFX ); //WAVE�t�@�C����ǂݍ���
    TDDSDGenWave( TDDSD* OWner, const string& QDAname, const string& id, bool is3D, bool useFX); //QDA����WAVE�t�@�C����ǂݍ���
    TDDSDGenWave( TDDSD* OWner, u32 freq, u32 bps, bool isStereo, u32 length, bool is3D, bool useFX, bool isStream );

    ~TDDSDGenWave();
    void BlockCopy( u32 offset, void* src, u32 length);
	IDirectSoundBuffer8* Buffer() { return FSoundBuffer; }
	bool IsStream()	{ return FIsStream; }
	bool UseFX() { return FUseFX; }
	LPWAVEFORMATEX WaveFormat() { return &FWaveFormat; }
	u32 Length() { return FLength; }
	u32 GetHalfPos() {return FHalfPos; }              
	void Sync();                   //NotifyThread����Ă΂�āA�X�g���[���X�V�C�x���g���Ăԃ��[�`���c�ق�Ƃ�Synchronize���ׂ��Ȃ񂾂��ǁc

};

//WAV�t�@�C��(���ʂ�)
class TDDSDWaveData : public TDDSDGenWave
{
public:
    //���ʃR���X�g���N�^
    TDDSDWaveData( TDDSD* OWner, const wstring& fname, bool useFX = false);
    TDDSDWaveData( TDDSD* OWner, const string& QDAname, const string& id, bool useFX = false); //QDA����WAVE�t�@�C����ǂݍ���

	TDDSDWaveData( TDDSD* OWner, u32 freq, u32 bps, bool isStereo, u32 length, bool isStream, bool useFX = false);

    ~TDDSDWaveData();
};


//�񎟃o�b�t�@���J�v�Z����������
class TDDSDChannel 
{
private:
    IDirectSoundBuffer8* FSoundBuffer;
    IDirectSound3DBuffer* F3DBuffer;

    bool FIs3D;  //3D�T�E���h�f�[�^��WaveData�Ɏw�肳��Ă���
    TDDSD* FOwner; //�I�[�i�[�ƂȂ�TDDSD�R���|�[�l���g
    s32 FPan;     //�p���|�b�g
    u32 FFreq;    //�Đ����g��
    u32 FSrcFreq; //����WAV�̎��g��
    f64 FFreqRate; //���̎��g���ɑ΂���Đ����g���̊���
    s32 FVolume;  //�{�����[��(1/100dB�P��)
    TDDSDGenWave* FWaveData;
    //FParams3D:TDDSDParams3D;
    bool FIsStream; //�X�g���[�~���O�Ή�WaveData���ڂ��Ă��邩
    //3D��p
    //void setParams3D(v:TDDSDParams3D);
public:
    TDDSDChannel(TDDSD* _owner);
    ~TDDSDChannel();
    void Play();
    void LoopPlay();
    void PlayAt(u32 ofs);    //�o�b�t�@�̔C�ӂ̃I�t�Z�b�g����Đ�
    void LoopPlayAt(u32 ofs);
    void Stop();
	IDirectSoundBuffer8* Buffer() { return FSoundBuffer; }
    //��3D�p
	s32 GetPan() { return FPan; }  //�p���j���O�́A3D�����Ă���ꍇ����
    //3D�p
    //property Params3D:TDDSDParams3D read FParams3D write setParams3D;  //3D�p�����[�^
    //���p
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
	u32 getPlayPosition();   //�o�b�t�@�̉��o�C�g�ڂ����݉��t����
	void setPlayPosition(u32 pos);  //�Đ��ʒu�̎w��
};

typedef vector<TDDSDChannel*> TDDSDChannelArray;

//DDSD�{��
class TDDSD
{
private:
    IDirectSound8* FDSound;
    IDirectSoundBuffer* FPrimaryBuffer;
    //IDirectSound3DListener8* F3DListener;
    TDDSDChannelArray FChannels;          //�e�`�����l��
    bool FInitialized;                 //�u�T�E���h�J�[�h�������v�t���O
    bool FUse3D;                       //�u3D�T�E���h���g���v�t���O
    //TDDSDListener* FListener;
    //FDebugOption: TDDSDDebugOption;                 //�ق��̃A�v���ɐ��䂪�ڂ��Ă��A�Đ��ł���悤��
    //3D��p
    //void setListener(const v:TDDSDListener);
    //void SetDebugOption(const Value: TDDSDDebugOption);
public:
	u32 FChannelCount;                  //�`�����l����
	bool FStickyFocus;

	TDDSD(HWND _handle);
    ~TDDSD();
    bool forceInitialize(HWND _handle);     //DirectSound�̍ď�����
	IDirectSound8* DSound() { return FDSound; }
	IDirectSoundBuffer* PrimaryBuffer() { return FPrimaryBuffer; }
	TDDSDChannel* GetChannels(s32 iCh);
	bool Initialized() { return FInitialized; }
    //property Listener:TDDSDListener read FListener write setListener;
    void SetPrimaryBufferFotmat(u32 freq, u32 bps, bool isStereo );

    //property DebugOption:TDDSDDebugOption read FDebugOption write SetDebugOption;
    //property Use3D:Boolean read FUse3D write FUse3D default False;
};
