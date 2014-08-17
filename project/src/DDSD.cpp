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
	//位置取得
	TPoint pos  =  _GetHalfPosition();
	if( ((pos.X == 0) && (pos.Y == 0))) return;
	if( ((pos.X < 0) || (pos.Y < 0))) return;

	//再生位置が前半に来た
	if( (FPositionFlag == 0)){
		if( ((pos.X < (s32)(FOwner->GetHalfPos())) &&
			(pos.Y < (s32)(FOwner->GetHalfPos()))))
		{
			//後半を更新
			IsTop = true;
			if( _GetPlay()) FOwner->Sync();
			FPositionFlag  =  1;
			return;
		}
	}

	//再生位置が後半に来た
	if( (FPositionFlag == 1)){
		if( ((pos.X > (s32)(FOwner->GetHalfPos())) &&
			(pos.Y > (s32)(FOwner->GetHalfPos()))))
		{
			//前半を更新
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

	//サウンドカードが無いなら、何もしない
	if( ! FOwner->Initialized() )
		return;

	HMMIO hm  =  mmioOpen((LPWSTR)fname.c_str(), NULL, MMIO_READ | MMIO_ALLOCBUF);

	if( hm == 0 ) {
		//PutDebugMessage(fname + 'が、見つかりません');
		return;
	}

	//WAVEに入る
	MMCKINFO mckP;    //親チャンクの情報
	mckP.fccType = MakeFourCC('W','A','V','E');
	if( (mmioDescend(hm, &mckP, NULL, MMIO_FINDRIFF)) != 0 ) {
		mmioClose(hm, 0);
		//PutDebugMessage(fname + 'は、WAVEファイルではありません');
		return;
	}

	//fmtチャンクに入る
	MMCKINFO mckC;    //子チャンクの情報
	mckC.ckid = MakeFourCC('f','m','t',' ');
	if( mmioDescend(hm, &mckC, &mckP, MMIO_FINDCHUNK) != 0 ) {
		mmioClose(hm, 0);
		//PutDebugMessage(fname + 'には、fmtチャンクが有りません');
		return;
	}

	//fmtチャンクを読み取る
	u32 fmtSize  =  mckC.cksize;
	if( mmioRead(hm, (HPSTR)&FWaveFormat, fmtSize) != fmtSize ) {
		mmioClose(hm, 0);
		//PutDebugMessage(fname + 'のfmtチャンクが不正です');
		return;
	}

	//fmtチャンクを抜け、dataチャンクに入る
	mmioAscend(hm, &mckC, 0);
	mckC.ckid = MakeFourCC('d','a','t','a');
	if( mmioDescend(hm, &mckC, &mckP, MMIO_FINDCHUNK) != 0 ) {
		mmioClose(hm, 0);
		//PutDebugMessage(fname + 'には、dataチャンクが有りません');
		return;
	}

	u32 dataSize = mckC.cksize;

	//二次バッファの作成
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


	//二次バッファのロック
	LPVOID lpBuf1, lpBuf2;
	DWORD dwBuf1, dwBuf2;
	FSoundBuffer->Lock(0,dataSize, &lpBuf1, &dwBuf1, &lpBuf2, &dwBuf2, 0);

	//音データのロード(dataチャンクを読み取る)
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

	//テンポラリファイルの作成
	GetTempPath(MAX_PATH, TempPath);
	GetTempFileName(TempPath, L"tmp", 0, TempName);

	size_t fileSize = (size_t)ms.seekg(0, std::ios::end).tellg();
	ms.seekg(0, std::ios::beg); 

	char* buff = (char*)malloc(fileSize);

	ofstream fs(TempName);
	fs.write(buff, (u32)fileSize);

	SAFE_FREE(buff);

	//テンポラリから読みこむ
	TDDSDGenWave::TDDSDGenWave(OWner, wstring(TempName), is3D, useFX);

	//テンポラリの削除
	DeleteFile(TempName);

}

TDDSDGenWave::TDDSDGenWave( TDDSD* OWner, u32 freq, u32 bps, bool isStereo, u32 length, bool is3D, bool useFX, bool isStream )
{
	FIsStream = isStream;
	FOwner = OWner;
	FLength = length;
	FOnUpdate = NULL;
	FUseFX = useFX;

	//サウンドカードが無いなら、何もしない
	if( ! FOwner->Initialized() )
		return;

	//WaveFormatを記述
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

	//二次バッファの作成
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

		//イベントオブジェクトの生成
		FEvent[0] = CreateEvent(NULL,false,false,NULL);
		FEvent[1] = CreateEvent(NULL,false,false,NULL);
		FEvent[2] = CreateEvent(NULL,false,false,NULL);

		//イベントの設定
		DSBPOSITIONNOTIFY pn[3];
		pn[0].dwOffset = 0;  //先頭
		pn[0].hEventNotify = FEvent[0];

		pn[1].dwOffset = FHalfPos;  //真中
		pn[1].hEventNotify = FEvent[1];

		pn[2].dwOffset = DSBPN_OFFSETSTOP;  //再生停止
		pn[2].hEventNotify = FEvent[2];

		//ノーティファイオブジェクトの作成
		FSoundBuffer->QueryInterface(IID_IDirectSoundNotify8, (LPVOID*)&FDSNotify);
		FDSNotify->SetNotificationPositions(3, pn);

		//イベント通知用スレッドの作成
		FNotifyThread = new TDDSDNotifyThread(this);

		HANDLE hMutex = CreateMutex(NULL,FALSE,NULL);	//ミューテックス生成
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
			HRESULT hr = FSoundBuffer->Release(); //返り値がエラーコードでない場合、AddRefによって参照カウントが増えてるだけ
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
	//サウンドカードが無いなら、何もしない
	if( ! FOwner->Initialized() )
		return;

	//ロック
	DWORD len1,len2;
	LPVOID pBuf1,pBuf2;
	FSoundBuffer->Lock(offset,length, &pBuf1, &len1, &pBuf2, &len2,0);

	//Buf1へコピー
	memmove(pBuf1, src, len1);

	//Buf2へコピー
	if( len1 < length ) {
		u8* src2 = (u8*)src;
		src2 += len1;
		memmove(pBuf2, src2, len2);
	}

	//アンロック
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
	//サウンドカード無しでは、何も出来ん
	if( ! FOwner->Initialized() )
		return;

	//既存のサウンドバッファの解放
	if( ! IsStream() ) {
		SAFE_RELEASE(F3DBuffer);
		SAFE_RELEASE(FSoundBuffer);
		
	} else {
		//いま乗せているのと同じオブジェクトなら、何もしない
		if( wav == FWaveData )
			return;

		//いままでの再生を止めさせる
		Stop();
	}

	if( wav == NULL ) {
		FWaveData = NULL;
		return;
	}

	//ストリームならば、単純に代入。そうでないならDuplicateSoundBufferして多重再生できるようにする
	//FX対応の場合、DUplicateしたくても出来ないDSoundの仕様があるので、泣く泣く代入だけ
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


	//3D再生できるようにする
	//if( wav is TDDSDWave3D ) {
	//	FIs3D = true;

	//	TRYDS(FSoundBuffer->QueryInterface(IID_IDirectSound3DBuffer,F3DBuffer),'setWaveData 3Dサウンドバッファの作成に失敗しました');
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
	//デフォルト値の設定
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
	//	TRYDS(F3DListener.Release,'DirectSound3D Listnerの解放に失敗しました');

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

	//DirectSoundの初期化
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

	//一次バッファの初期化
	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd,sizeof(dsbd));

	dsbd.dwSize = sizeof(dsbd);
	if( FUse3D )
		dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRL3D;
	else
		dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER;

	FDSound->CreateSoundBuffer(&dsbd, &FPrimaryBuffer, NULL);

	//3D対応にする
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
	//サウンドカードの無い環境なら、何もしない
	if( ! FInitialized )
		return;

	//プライマリ･バッファのフォーマットを設定する
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
