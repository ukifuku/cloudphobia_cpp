#include "DDSD.h"
#pragma comment ( lib, "winmm.lib" )
#include <mmsystem.h>
#include <Windows.h>
#include <sstream>
#include <fstream>
#include "QDArc.h"

void ExecThread( TDDSDNotifyThread* _obj )
{
	_obj->Execute();
}

void TerminateThread( TDDSDNotifyThread* _obj )
{
	_obj->Terminated = true;
}

TDDSDNotifyThread::TDDSDNotifyThread( TDDSDGenWave* owner )
{
	FOwner = owner;
	Terminated = false;
	IsTop = false;
}

TDDSDNotifyThread::~TDDSDNotifyThread()
{
	Terminated = true;
}


bool TDDSDNotifyThread::_GetPlay()
{
	u32 a  =  0;
	bool RESULT  =  false;
	if( (FOwner->Buffer() != NULL))
		FOwner->Buffer()->GetStatus((LPDWORD)&a);

	if( (a && DSBSTATUS_PLAYING != 0)) 
		RESULT  =  true;

	return RESULT;
}



TPoint TDDSDNotifyThread::_GetHalfPosition()
{
	TPoint RESULT;
	if( (FOwner->Buffer() == NULL))
	{
		RESULT.X  =  0;
		RESULT.Y  =  0;
	}

	s32 w, r;
	FOwner->Buffer()->GetCurrentPosition((LPDWORD)&r, (LPDWORD)&w);
	RESULT.X  =  r;
	RESULT.Y  =  w;

	return RESULT;
}


void TDDSDNotifyThread::_SyncFunc()
{
	//�ʒu�擾
	TPoint pos  =  _GetHalfPosition();
	if( ((pos.X == 0) && (pos.Y == 0))) return;
	if( ((pos.X < 0) || (pos.Y < 0))) return;

	//�Đ��ʒu���O���ɗ���
	if( (FPositionFlag == 0)){
		if( ((pos.X < (s32)(FOwner->GetHalfPos())) &&
			(pos.Y < (s32)(FOwner->GetHalfPos()))))
		{
			//�㔼���X�V
			IsTop = true;
			if( _GetPlay()) FOwner->Sync();
			FPositionFlag  =  1;
			return;
		}
	}

	//�Đ��ʒu���㔼�ɗ���
	if( (FPositionFlag == 1)){
		if( ((pos.X > (s32)(FOwner->GetHalfPos())) &&
			(pos.Y > (s32)(FOwner->GetHalfPos()))))
		{
			//�O�����X�V
			IsTop = false;
			if( _GetPlay()) FOwner->Sync();
			FPositionFlag  =  0;
			return;
		}
	}
}

void TDDSDNotifyThread::Execute()
{
	FPositionFlag  =  0;
	while( ! Terminated )
	{
		Sleep(10);
		if( Terminated ) break;
		
		_SyncFunc();
	}

}


void TDDSDGenWave::Sync()
{
	if( FOnUpdate ) {
		if( FNotifyThread->IsTop )
			FOnUpdate(this, FPlayer, FHalfPos, FLength - FHalfPos);
		else
			FOnUpdate(this, FPlayer,0,FHalfPos);
	}
}


u32 MakeFourCC(s8 ch0, s8 ch1, s8 ch2, s8 ch3)
{
	return (s32)(ch0) + ((s32)(ch1) << 8) +
		((s32)(ch2) << 16) + ((s32)(ch3) << 24);
}

TDDSDGenWave::TDDSDGenWave( TDDSD* OWner, const wstring& fname, bool is3D, bool useFX )
{
	FIsStream = false;
	FOwner = OWner;
	FUseFX = useFX;

	//�T�E���h�J�[�h�������Ȃ�A�������Ȃ�
	if( ! FOwner->Initialized() )
		return;

	HMMIO hm  =  mmioOpen((LPWSTR)fname.c_str(), NULL, MMIO_READ | MMIO_ALLOCBUF);

	if( hm == 0 ) {
		//PutDebugMessage(fname + '���A������܂���');
		return;
	}

	//WAVE�ɓ���
	MMCKINFO mckP;    //�e�`�����N�̏��
	mckP.fccType = MakeFourCC('W','A','V','E');
	if( (mmioDescend(hm, &mckP, NULL, MMIO_FINDRIFF)) != 0 ) {
		mmioClose(hm, 0);
		//PutDebugMessage(fname + '�́AWAVE�t�@�C���ł͂���܂���');
		return;
	}

	//fmt�`�����N�ɓ���
	MMCKINFO mckC;    //�q�`�����N�̏��
	mckC.ckid = MakeFourCC('f','m','t',' ');
	if( mmioDescend(hm, &mckC, &mckP, MMIO_FINDCHUNK) != 0 ) {
		mmioClose(hm, 0);
		//PutDebugMessage(fname + '�ɂ́Afmt�`�����N���L��܂���');
		return;
	}

	//fmt�`�����N��ǂݎ��
	u32 fmtSize  =  mckC.cksize;
	if( mmioRead(hm, (HPSTR)&FWaveFormat, fmtSize) != fmtSize ) {
		mmioClose(hm, 0);
		//PutDebugMessage(fname + '��fmt�`�����N���s���ł�');
		return;
	}

	//fmt�`�����N�𔲂��Adata�`�����N�ɓ���
	mmioAscend(hm, &mckC, 0);
	mckC.ckid = MakeFourCC('d','a','t','a');
	if( mmioDescend(hm, &mckC, &mckP, MMIO_FINDCHUNK) != 0 ) {
		mmioClose(hm, 0);
		//PutDebugMessage(fname + '�ɂ́Adata�`�����N���L��܂���');
		return;
	}

	u32 dataSize = mckC.cksize;

	//�񎟃o�b�t�@�̍쐬
	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof(dsbd));
	dsbd.dwSize = sizeof(dsbd);
	if( is3D )
		dsbd.dwFlags = DSBCAPS_CTRLVOLUME + DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRL3D;
	else
		dsbd.dwFlags = DSBCAPS_CTRLPAN + DSBCAPS_CTRLVOLUME + DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPAN;

	if( FUseFX )
		dsbd.dwFlags = DSBCAPS_CTRLPAN + DSBCAPS_CTRLVOLUME + DSBCAPS_CTRLFREQUENCY + DSBCAPS_CTRLFX;


	if( FOwner->FStickyFocus )
		dsbd.dwFlags = dsbd.dwFlags | DSBCAPS_STICKYFOCUS;

	dsbd.dwBufferBytes = dataSize;
	dsbd.lpwfxFormat = &FWaveFormat;

	FLength = dataSize;

	IDirectSoundBuffer* dsb;
	FOwner->DSound()->CreateSoundBuffer(&dsbd, &dsb, NULL);
	dsb->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&FSoundBuffer);
	dsb->Release(); 


	//�񎟃o�b�t�@�̃��b�N
	LPVOID lpBuf1, lpBuf2;
	DWORD dwBuf1, dwBuf2;
	FSoundBuffer->Lock(0,dataSize, &lpBuf1, &dwBuf1, &lpBuf2, &dwBuf2, 0);

	//���f�[�^�̃��[�h(data�`�����N��ǂݎ��)
	mmioRead(hm, (HPSTR)lpBuf1, dwBuf1);
	if( dwBuf2 != 0 ) {
		mmioRead(hm, (HPSTR)lpBuf2, dwBuf2);
	}

	FSoundBuffer->Unlock(lpBuf1,dwBuf1,lpBuf2,dwBuf2);
	FSoundBuffer->SetFrequency(FWaveFormat.nSamplesPerSec);
	mmioClose(hm, 0);


}

TDDSDGenWave::TDDSDGenWave( TDDSD* OWner, const string& QDAname, const string& id, bool is3D, bool useFX )
{
	wchar_t TempPath[MAX_PATH];
	wchar_t TempName[MAX_PATH];

	stringstream ms;
	ExtractFromQDAFile(ms, QDAname, id);

	//�e���|�����t�@�C���̍쐬
	GetTempPath(MAX_PATH, TempPath);
	GetTempFileName(TempPath, L"tmp", 0, TempName);

	size_t fileSize = (size_t)ms.seekg(0, std::ios::end).tellg();
	ms.seekg(0, std::ios::beg); 

	char* buff = (char*)malloc(fileSize);

	ofstream fs(TempName);
	fs.write(buff, (u32)fileSize);

	SAFE_FREE(buff);

	//�e���|��������ǂ݂���
	TDDSDGenWave::TDDSDGenWave(OWner, wstring(TempName), is3D, useFX);

	//�e���|�����̍폜
	DeleteFile(TempName);

}

TDDSDGenWave::TDDSDGenWave( TDDSD* OWner, u32 freq, u32 bps, bool isStereo, u32 length, bool is3D, bool useFX, bool isStream )
{
	FIsStream = isStream;
	FOwner = OWner;
	FLength = length;
	FOnUpdate = NULL;
	FUseFX = useFX;

	//�T�E���h�J�[�h�������Ȃ�A�������Ȃ�
	if( ! FOwner->Initialized() )
		return;

	//WaveFormat���L�q
	FWaveFormat.wFormatTag = WAVE_FORMAT_PCM;
	if( isStereo )
		FWaveFormat.nChannels = 2;
	else
		FWaveFormat.nChannels = 1;
	FWaveFormat.nSamplesPerSec = freq;
	FWaveFormat.nBlockAlign = FWaveFormat.nChannels * bps / 8;
	FWaveFormat.nAvgBytesPerSec = freq * FWaveFormat.nBlockAlign;
	FWaveFormat.wBitsPerSample = bps;
	FWaveFormat.cbSize = 0;

	//�񎟃o�b�t�@�̍쐬
	u32 flagBase;
	if( isStream )
		flagBase = DSBCAPS_CTRLPAN + DSBCAPS_CTRLVOLUME + DSBCAPS_CTRLFREQUENCY + DSBCAPS_CTRLPOSITIONNOTIFY + DSBCAPS_GETCURRENTPOSITION2;
	else
		flagBase = DSBCAPS_CTRLPAN + DSBCAPS_CTRLVOLUME + DSBCAPS_CTRLFREQUENCY;

	if( useFX )
		flagBase = DSBCAPS_CTRLPAN + DSBCAPS_CTRLVOLUME + DSBCAPS_CTRLFREQUENCY + DSBCAPS_CTRLFX;

	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd,sizeof(dsbd));

	dsbd.dwSize = sizeof(dsbd);
	if( is3D )
		dsbd.dwFlags = flagBase + DSBCAPS_CTRL3D - DSBCAPS_CTRLPAN;
	else
		dsbd.dwFlags = flagBase;

	if( FOwner->FStickyFocus )
		dsbd.dwFlags = dsbd.dwFlags | DSBCAPS_STICKYFOCUS;

	dsbd.dwBufferBytes = length;
	dsbd.lpwfxFormat = &FWaveFormat;


	LPDIRECTSOUNDBUFFER dsb;
	FOwner->DSound()->CreateSoundBuffer(&dsbd, &dsb, NULL);
	dsb->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&FSoundBuffer);
	dsb->Release();


	if( isStream ) {

		FHalfPos = length >> 1;

		//�C�x���g�I�u�W�F�N�g�̐���
		FEvent[0] = CreateEvent(NULL,false,false,NULL);
		FEvent[1] = CreateEvent(NULL,false,false,NULL);
		FEvent[2] = CreateEvent(NULL,false,false,NULL);

		//�C�x���g�̐ݒ�
		DSBPOSITIONNOTIFY pn[3];
		pn[0].dwOffset = 0;  //�擪
		pn[0].hEventNotify = FEvent[0];

		pn[1].dwOffset = FHalfPos;  //�^��
		pn[1].hEventNotify = FEvent[1];

		pn[2].dwOffset = DSBPN_OFFSETSTOP;  //�Đ���~
		pn[2].hEventNotify = FEvent[2];

		//�m�[�e�B�t�@�C�I�u�W�F�N�g�̍쐬
		FSoundBuffer->QueryInterface(IID_IDirectSoundNotify8, (LPVOID*)&FDSNotify);
		FDSNotify->SetNotificationPositions(3, pn);

		//�C�x���g�ʒm�p�X���b�h�̍쐬
		FNotifyThread = new TDDSDNotifyThread(this);

		HANDLE hMutex = CreateMutex(NULL,FALSE,NULL);	//�~���[�e�b�N�X����
		m_threadId = (HANDLE)CreateThread(NULL, 0,
			(LPTHREAD_START_ROUTINE)ExecThread,
			(LPVOID)FNotifyThread, 0, NULL);

		//FnotifyThread.Priority = tpHigher;
	}

}

TDDSDGenWave::~TDDSDGenWave()
{
	if( FOwner->Initialized() ) {

		if( FSoundBuffer ) {
			HRESULT hr = FSoundBuffer->Release(); //�Ԃ�l���G���[�R�[�h�łȂ��ꍇ�AAddRef�ɂ���ĎQ�ƃJ�E���g�������Ă邾��
			if( (u32)(hr) > 0x80000000 )
			{
				;
			}

			if( FIsStream && FNotifyThread ) {
				TerminateThread( FNotifyThread );
				WaitForSingleObject( m_threadId, INFINITE );
				SAFE_DELETE(FNotifyThread);
				CloseHandle(FEvent[0]);
				CloseHandle(FEvent[1]);
				CloseHandle(FEvent[2]);
			}
		}
	}
}

void TDDSDGenWave::BlockCopy( u32 offset, void* src, u32 length )
{
	//�T�E���h�J�[�h�������Ȃ�A�������Ȃ�
	if( ! FOwner->Initialized() )
		return;

	//���b�N
	DWORD len1,len2;
	LPVOID pBuf1,pBuf2;
	FSoundBuffer->Lock(offset,length, &pBuf1, &len1, &pBuf2, &len2,0);

	//Buf1�փR�s�[
	memmove(pBuf1, src, len1);

	//Buf2�փR�s�[
	if( len1 < length ) {
		u8* src2 = (u8*)src;
		src2 += len1;
		memmove(pBuf2, src2, len2);
	}

	//�A�����b�N
	FSoundBuffer->Unlock(pBuf1,len1,pBuf2,len2);

}

TDDSDWaveData::TDDSDWaveData( TDDSD* OWner, const wstring& fname, bool useFX /*= false*/ ) : TDDSDGenWave(OWner,fname,false,useFX)
{
}

TDDSDWaveData::TDDSDWaveData( TDDSD* OWner, const string& QDAname, const string& id, bool useFX /*= false*/ ) : TDDSDGenWave(OWner, QDAname,id, false, useFX)
{
}

TDDSDWaveData::TDDSDWaveData( TDDSD* OWner, u32 freq, u32 bps, bool isStereo, u32 length, bool isStream, bool useFX /*= false*/ ) : TDDSDGenWave(OWner,freq,bps,isStereo,length,false, useFX, isStream)
{
}

TDDSDWaveData::~TDDSDWaveData()
{

}

TDDSDChannel::TDDSDChannel( TDDSD* _owner )
{
	FOwner = _owner;
	FSoundBuffer = NULL;
	F3DBuffer = NULL;
	FWaveData = NULL;
	FIs3D = false;
	FIsStream = false;

	FPan = 0;
	FFreqRate = 1.0f;
	FVolume = 0;

	//FParams3D = TDDSDParams3D.Create(this);

}

TDDSDChannel::~TDDSDChannel()
{
	SAFE_RELEASE(F3DBuffer);
	SAFE_RELEASE(FSoundBuffer);

	//if( Assigned(FParams3D) )
	//	FParams3D.Free;
}

void TDDSDChannel::Play()
{
	if( ! FSoundBuffer )
		return;

	FSoundBuffer->SetCurrentPosition(0);
	FSoundBuffer->Play(0,0,0);

}

void TDDSDChannel::LoopPlay()
{
	if( ! FSoundBuffer )
		return;

	FSoundBuffer->SetCurrentPosition(0);
	FSoundBuffer->Play(0,0,DSBPLAY_LOOPING);

}

void TDDSDChannel::PlayAt( u32 ofs )
{
	if( ! FSoundBuffer )
		return;

	FSoundBuffer->SetCurrentPosition(ofs);
	FSoundBuffer->Play(0,0,0);

}

void TDDSDChannel::LoopPlayAt( u32 ofs )
{
	if( ! FSoundBuffer )
		return;

	FSoundBuffer->SetCurrentPosition(ofs);
	FSoundBuffer->Play(0,0,DSBPLAY_LOOPING);

}

void TDDSDChannel::Stop()
{
	if( ! FSoundBuffer ) {
		return;
	}

	FSoundBuffer->Stop();

}

u32 TDDSDChannel::getStatus()
{
	if( ! FSoundBuffer ) {
		return 0;
	}

	DWORD ret;
	FSoundBuffer->GetStatus(&ret);
	return ret;

}

void TDDSDChannel::setFreqRate( f64 v )
{
	if( ! !FSoundBuffer )
		return;

	FFreq = (u32)(v * (f64)FSrcFreq);
	FSoundBuffer->SetFrequency(FFreq);

}

void TDDSDChannel::setVolume( s32 v )
{
	if( ! !FSoundBuffer )
		return;
	FVolume = v;
	FSoundBuffer->SetVolume(FVolume);

}

void TDDSDChannel::setPan( s32 v )
{
	if( ! !FSoundBuffer )
		return;

	if( ! Is3D() ) {
		FPan = v;
		FSoundBuffer->SetPan(FPan);
	}

}

void TDDSDChannel::setWaveData( TDDSDGenWave* wav )
{
	//�T�E���h�J�[�h�����ł́A�����o����
	if( ! FOwner->Initialized() )
		return;

	//�����̃T�E���h�o�b�t�@�̉��
	if( ! IsStream() ) {
		SAFE_RELEASE(F3DBuffer);
		SAFE_RELEASE(FSoundBuffer);
		
	} else {
		//���܏悹�Ă���̂Ɠ����I�u�W�F�N�g�Ȃ�A�������Ȃ�
		if( wav == FWaveData )
			return;

		//���܂܂ł̍Đ����~�߂�����
		Stop();
	}

	if( wav == NULL ) {
		FWaveData = NULL;
		return;
	}

	//�X�g���[���Ȃ�΁A�P���ɑ���B�����łȂ��Ȃ�DuplicateSoundBuffer���đ��d�Đ��ł���悤�ɂ���
	//FX�Ή��̏ꍇ�ADUplicate�������Ă��o���Ȃ�DSound�̎d�l������̂ŁA���������������
	if( wav->IsStream() || wav->UseFX() ) {
		FSoundBuffer = wav->Buffer();
		FSoundBuffer->AddRef();
		wav->FPlayer = this;
	} else {
		LPDIRECTSOUNDBUFFER dsb;
		FOwner->DSound()->DuplicateSoundBuffer(wav->Buffer(), &dsb);
		dsb->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&FSoundBuffer);
		dsb->Release();
	}

	FIsStream = wav->IsStream();


	//3D�Đ��ł���悤�ɂ���
	//if( wav is TDDSDWave3D ) {
	//	FIs3D = true;

	//	TRYDS(FSoundBuffer->QueryInterface(IID_IDirectSound3DBuffer,F3DBuffer),'setWaveData 3D�T�E���h�o�b�t�@�̍쐬�Ɏ��s���܂���');
	//	Params3D = TDDSDWave3D(wav).Params3D;
	//} else {
		FIs3D = false;
	//}

	FSrcFreq = wav->WaveFormat()->nSamplesPerSec;
	FWaveData = wav;

}

u32 TDDSDChannel::getPlayPosition()
{
	if( ! !FSoundBuffer ) {
		return 0;
	}

	DWORD pos;
	FSoundBuffer->GetCurrentPosition(&pos, NULL);
	return pos;

}

void TDDSDChannel::setPlayPosition( u32 pos )
{
	if( ! !FSoundBuffer ) {
		return;
	}

	FSoundBuffer->SetCurrentPosition(pos);

}

TDDSD::TDDSD(HWND _handle)
{
	//�f�t�H���g�l�̐ݒ�
	FChannelCount = 8;
	FStickyFocus = true;
	FUse3D = false;
	//DebugOption = [dsoHaltOnError];

	forceInitialize(_handle);
}

TDDSD::~TDDSD()
{
	for( u32 i = 0 ; i < FChannels.size(); i++ )
		SAFE_DELETE(FChannels[i]);

	//if( Assigned(FListener) )
	//	FListener.Free;

	//if( Assigned(F3DListener) )
	//	TRYDS(F3DListener.Release,'DirectSound3D Listner�̉���Ɏ��s���܂���');

	SAFE_RELEASE(FPrimaryBuffer);
	SAFE_RELEASE(FDSound);

	FChannels.clear();
}

bool TDDSD::forceInitialize( HWND _handle )
{
	//dsb:IDirectSoundBuffer;

	if( !FChannels.empty() ) {
		for( u32 i = 0 ; i < FChannels.size(); i++ ) {
			SAFE_DELETE(FChannels[i]);
		}
		FChannels.clear();
	}
	FChannels.resize(FChannelCount);

	for( u32 i = 0 ; i < FChannelCount; i++ ) {
		FChannels[i] = new TDDSDChannel(this);
	}

	//DirectSound�̏�����
	FInitialized = false;
	if( CoCreateInstance(CLSID_DirectSound8,NULL,CLSCTX_INPROC_SERVER, IID_IDirectSound8, (LPVOID*)&FDSound) != 0 ) {
		return false;
	} else {
		if( FDSound->Initialize(NULL) != 0 ) {
			return false;
		}
	}

	FInitialized = true;

	FDSound->SetCooperativeLevel(_handle, DSSCL_PRIORITY);

	//�ꎟ�o�b�t�@�̏�����
	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd,sizeof(dsbd));

	dsbd.dwSize = sizeof(dsbd);
	if( FUse3D )
		dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRL3D;
	else
		dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER;

	FDSound->CreateSoundBuffer(&dsbd, &FPrimaryBuffer, NULL);

	//3D�Ή��ɂ���
	//if( FUse3D ) {
	//	FPrimaryBuffer->QueryInterface(IID_IDirectSound3DListener8, (LPVOID*)&F3DListener);
	//	if( FListener != NULL )
	//		FListener.Free;
	//	FListener = TDDSDListener.Create(this);
	//}

	return true;
}

TDDSDChannel* TDDSD::GetChannels( s32 iCh )
{
	if( iCh > (s32)FChannelCount ) {
		return NULL;
	}
	return FChannels[iCh];
}

void TDDSD::SetPrimaryBufferFotmat( u32 freq, u32 bps, bool isStereo )
{
	//�T�E���h�J�[�h�̖������Ȃ�A�������Ȃ�
	if( ! FInitialized )
		return;

	//�v���C�}����o�b�t�@�̃t�H�[�}�b�g��ݒ肷��
	WAVEFORMATEX format;
	format.wFormatTag  =  WAVE_FORMAT_PCM;

	if( isStereo )
		format.nChannels  =  2;
	else
		format.nChannels  =  1;

	format.nSamplesPerSec  =  freq;
	format.wBitsPerSample  =  bps;
	format.nBlockAlign  =  format.wBitsPerSample / 8 * format.nChannels;
	format.nAvgBytesPerSec  =  format.nSamplesPerSec * format.nBlockAlign;
	format.cbSize = 0;

	PrimaryBuffer()->SetFormat(&format);

}
