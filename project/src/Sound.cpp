#include "Sound.h"
#include "Define.h"
#include "DDSD.h"

TSoundEffect::TSoundEffect()
{
	SE.push_back(new TDDSDWaveData(SD,"./Data/sound8.BIN","graze.wav"));                 //ch0
	SE.push_back(new TDDSDWaveData(SD,"./Data/sound8.BIN","L_damage2.wav"));              //ch1
	SE.push_back(new TDDSDWaveData(SD,"./Data/sound8.BIN","L_damage.wav"));              //ch1
	SE.push_back(new TDDSDWaveData(SD,"./Data/sound8.BIN","gunshot.wav"));               //ch2
	SE.push_back(new TDDSDWaveData(SD,"./Data/sound8.BIN","Missile.wav"));               //ch4
	SE.push_back(new TDDSDWaveData(SD,"./Data/sound8.BIN","exp_s.wav"));                 //ch3
	SE.push_back(new TDDSDWaveData(SD,"./Data/sound8.BIN","exp_m.wav"));                 //ch3
	SE.push_back(new TDDSDWaveData(SD,"./Data/sound8.BIN","EDamage0.wav"));              //ch7
	SE.push_back(new TDDSDWaveData(SD,"./Data/sound8.BIN","exp_L.wav"));                 //ch5
	SE.push_back(new TDDSDWaveData(SD,"./Data/sound8.BIN","lock.wav"));                  //ch1
	SE.push_back(new TDDSDWaveData(SD,"./Data/sound8.BIN","warning.wav"));              //ch6
	SE.push_back(new TDDSDWaveData(SD,"./Data/sound8.BIN","VScroll.wav"));              //ch5
	SE.push_back(new TDDSDWaveData(SD,"./Data/sound8.BIN","zako5appear.wav"));          //ch5
	SE.push_back(new TDDSDWaveData(SD,"./Data/sound8.BIN","BossExp.wav"));              //ch0
	SE.push_back(new TDDSDWaveData(SD,"./Data/sound8.BIN","Boost.wav"));                //ch1
	SE.push_back(new TDDSDWaveData(SD,"./Data/sound8.BIN","Boost2.wav"));               //ch6
	SE.push_back(new TDDSDWaveData(SD,"./Data/sound8.BIN","Charge.wav"));               //ch8
	SE.push_back(new TDDSDWaveData(SD,"./Data/sound8.BIN","Beam.wav"));                 //ch9
	SE.push_back(new TDDSDWaveData(SD,"./Data/sound8.BIN","Edamage1.wav"));             //ch7
	SE.push_back(new TDDSDWaveData(SD,"./Data/sound8.BIN","FunMove.wav"));              //ch6
	SE.push_back(new TDDSDWaveData(SD,"./Data/sound8.BIN","FunAppear.wav"));            //ch6
	SE.push_back(new TDDSDWaveData(SD,"./Data/sound8.BIN","L_beam.wav"));               //ch10
	SE.push_back(new TDDSDWaveData(SD,"./Data/sound8.BIN","homing.wav"));               //ch10
	SE.push_back(new TDDSDWaveData(SD,"./Data/sound8.BIN","homing2.wav"));              //ch11
	SE.push_back(new TDDSDWaveData(SD,"./Data/sound8.BIN","bossshot.wav"));             //ch9
	SE.push_back(new TDDSDWaveData(SD,"./Data/sound8.BIN","trans.wav"));                //ch10
	SE.push_back(new TDDSDWaveData(SD,"./Data/sound8.BIN","Start.wav"));                //ch0
	SE.push_back(new TDDSDWaveData(SD,"./Data/sound8.BIN","zako4appear.wav"));          //ch11
	SE.push_back(new TDDSDWaveData(SD,"./Data/sound8.BIN","heart.wav"));                //ch14
	SE.push_back(new TDDSDWaveData(SD,"./Data/sound8.BIN","deside.wav"));                //ch0
	SE.push_back(new TDDSDWaveData(SD,"./Data/sound8.BIN","select.wav"));                //ch1
	SE.push_back(new TDDSDWaveData(SD,"./Data/sound8.BIN","bonus2.wav"));                //ch13
	SE.push_back(new TDDSDWaveData(SD,"./Data/sound8.BIN","boot.wav"));                //ch0
	SE.push_back(new TDDSDWaveData(SD,"./Data/sound8.BIN","slash_a_2.wav"));                //
	SE.push_back(new TDDSDWaveData(SD,"./Data/sound8.BIN","slash_b.wav"));                //
	SE.push_back(new TDDSDWaveData(SD,"./Data/sound8.BIN","slash_c.wav"));                // 
	SE.push_back(new TDDSDWaveData(SD,"./Data/sound8.BIN","swing_a.wav"));                // ch2
	SE.push_back(new TDDSDWaveData(SD,"./Data/sound8.BIN","swing_b.wav"));                // ch2

	SE.push_back(new TDDSDWaveData(SD, "./Data/sound2.BIN", "alarm.wav"));
	SE.push_back(new TDDSDWaveData(SD, "./Data/sound2.BIN", "base_exp.wav"));                                      // ch10
	SE.push_back(new TDDSDWaveData(SD, "./Data/sound2.BIN", "engine.wav"));                                        // ch0
	SE.push_back(new TDDSDWaveData(SD, "./Data/sound2.BIN", "flash.wav"));                                        // ch0
	SE.push_back(new TDDSDWaveData(SD, "./Data/sound2.BIN", "shutter_close.wav"));                                // ch11
	SE.push_back(new TDDSDWaveData(SD, "./Data/sound2.BIN", "shutter_fullopen.wav"));                             // ch0
	SE.push_back(new TDDSDWaveData(SD, "./Data/sound2.BIN", "shutter_open.wav"));                                 // ch0
	SE.push_back(new TDDSDWaveData(SD, "./Data/sound2.BIN", "shutter_unlock.wav"));                                // ch1
	SE.push_back(new TDDSDWaveData(SD, "./Data/sound2.BIN", "spidereye.wav"));                                    // ch11
	SE.push_back(new TDDSDWaveData(SD, "./Data/sound2.BIN", "swing_enemy.wav"));                                  // ch8
	SE.push_back(new TDDSDWaveData(SD, "./Data/sound2.BIN", "tankdash.wav"));                                     // ch10
	SE.push_back(new TDDSDWaveData(SD, "./Data/sound2.BIN", "tankjump.wav"));                                      // ch10
	SE.push_back(new TDDSDWaveData(SD, "./Data/sound2.BIN", "tankshot.wav"));                                     // ch9
	SE.push_back(new TDDSDWaveData(SD, "./Data/sound2.BIN", "thunder.wav"));                                      // ch10
	SE.push_back(new TDDSDWaveData(SD, "./Data/sound2.BIN", "warp.wav"));                                  // ch8
	SE.push_back(new TDDSDWaveData(SD, "./Data/sound2.BIN", "warpout.wav"));                               // ch8
	SE.push_back(new TDDSDWaveData(SD, "./Data/sound2.BIN", "warp2.wav"));                                  // ch8
	SE.push_back(new TDDSDWaveData(SD, "./Data/sound2.BIN", "shipcharge.wav"));                                  // ch10
	SE.push_back(new TDDSDWaveData(SD, "./Data/sound2.BIN", "submarine.wav"));                                  // ch0
	SE.push_back(new TDDSDWaveData(SD, "./Data/sound2.BIN", "Ship_exp.wav"));                                  // ch0
	SE.push_back(new TDDSDWaveData(SD, "./Data/sound2.BIN", "boss2laser.wav"));                                  // ch9
	SE.push_back(new TDDSDWaveData(SD, "./Data/sound2.BIN", "ShipDamage.wav"));                                  // ch1

}

TSoundEffect::~TSoundEffect()
{
	for( s32 i = 0 ; i < SE.size(); i++ )
		SAFE_DELETE( SE[i]);

	SE.clear();
}

void TSoundEffect::WavPlay( s32 _WavNum, s32 _ch )
{
	if( SEVol>0 ) {
		SD->GetChannels(_ch)->setWaveData(SE[_WavNum]);
	SD->GetChannels(_ch)->setVolume(-500-(10-SEVol)*200);
	SD->GetChannels(_ch)->Stop();
	SD->GetChannels(_ch)->Play();
	}

}

void TSoundEffect::WavPlay2( s32 _WavNum, s32 _ch, s32 _vol )
{
	if( SEVol>0 ) {
		s32 factor = RoundOff(SEVol * _vol/100);
		SD->GetChannels(_ch)->setWaveData(SE[_WavNum]);
		SD->GetChannels(_ch)->setVolume(-500-(10-factor)*200);
		SD->GetChannels(_ch)->Stop();
		SD->GetChannels(_ch)->Play();
	}

}

void TSoundEffect::WavStop( s32 _Ch )
{
	SD->GetChannels(_Ch)->Stop();

}

void TSoundEffect::AllStop()
{
	for( s32 i = 0 ; i <= 14; i++ )
		SD->GetChannels(i)->Stop();                                                        
}


TMusicPlayer::TMusicPlayer()
{
	//バッファ長
	//  Buff = 1024*96*2;
	s32 Buff  =  KPLAYER_BUFFERLENGTH;

	//OggDLL読み込み
	_VorbisfileInitialize("./Data/wuvorbis.dll");

	s32 bgmdatasize = sizeof(BGMData)/sizeof(TOggData*);
	BGMPlayer = new TOggPlayer(Buff, bgmdatasize );
	BGMPlayer->Init();

	for( s32 i = 0 ; i < bgmdatasize; i++ ) {
		BGMData[i] = new TOggData(Buff);
		BGMData[i]->Init();
	}

	// ストリーミング再生用
	MuStream = new TDDSDWaveData(SD, 44100, 16, Buff, true, false);
	MuStream->OnUpdate = UpdateWav;
	SD->GetChannels(BGMCH)->setWaveData(MuStream);
	SD->GetChannels(BGMCH)->LoopPlay();

	BGMData[0].LoadOgg("./bgm/MurdDes96.AO");
	BGMPlayer->Load(0,BGMData[0]);

	BGMData[2].LoadOgg("./bgm/FallAp.AO");
	BGMPlayer->Load(2,BGMData[2]);

	for( s32 i = 0 ; i < sizeof(Playing)/sizeof(bool); i++ )
		Playing[i] = false;

	Muted = false;

	Stop();

}

TMusicPlayer::~TMusicPlayer()
{
	// ストリーミングサウンドの解放
	SD->GetChannels(BGMCH)->Stop();
	SD->GetChannels(BGMCH)->setWaveData(NULL);
	SAFE_DELETE(MuStream);

	// Ogg関連を解放
	s32 bgmdatasize = sizeof(BGMData)/sizeof(TOggData*);
	for( s32 i = 0 ; i < bgmdatasize; i++ )
		SAFE_DELETE(BGMData[i]);

	SAFE_DELETE(BGMPlayer);

	// DLLの解放
	_VorbisfileFinalize();

}

void TMusicPlayer::UpdateWav( TDDSDGenWave* Sender, TDDSDChannel* Player, u32 ofs, u32 len )
{
	//OggDataにレンダリング命令を発効する
	BGMPlayer->Rendering();
	//ストリーミングサウンドにレンダリングした結果を転送
	MuStream->BlockCopy(ofs, BGMPlayer->Offset, len);

}

void TMusicPlayer::Play( s32 _ch )
{
	if( (! Playing[_ch]) && (bgmVol>0) ) {
		BGMPlayer->Stop(_ch);
		// 再生
		if( TrackNum == 2 )
			BGMPlayer->SetChannelVolume(_ch,0.7f);
		else
			BGMPlayer->SetChannelVolume(_ch,1.0f);

		BGMPlayer->SetMasterVolume(bgmVol*0.1f);
		BGMPlayer->SetVolume(_ch,1.0f);

		//再生後すぐに音が鳴るようにプライマリバッファにデータを格納しておく
		BGMPlayer->Rendering();
		MuStream.BlockCopy(0,BGMPlayer->Offset,KPLAYER_BUFFERLENGTH / 2);
		MuStream.Buffer.SetCurrentPosition(0);


		BGMPlayer->Play(_ch,TRUE);

		Playing[_ch] = true;
	}

}

void TMusicPlayer::Stop()
{
	// 停止
	BGMPlayer->Stop;

	//現在再生中のバッファを空白にする(普通にStopさせるとタイムラグが生じる)
	u8 Buffer[KPLAYER_BUFFERLENGTH];
	for( s32 i = 0 ; i < sizeof(Buffer); i++ )
		Buffer[i] = 0;

	MuStream.BlockCopy(0, Buffer, KPLAYER_BUFFERLENGTH);

	for( s32 i = 0 ; i < sizeof(Playing); i++ )
		Playing[i] = false;

}

void TMusicPlayer::FadeOut( s32 _ch, f32 _vol, f32 _sec )
{
	if( BGMPlayer->GetOutputTime(_ch)!=0 ) 
		BGMPlayer->SetVolume(_ch,_vol,_sec);

	Playing[_ch] = false;

}

void TMusicPlayer::BGMChange( s32 _ch, s32 _track )
{
	BGMData[_ch].LoadOgg(BGMFile[_track]);

	BGMPlayer->UnLoad(_ch);
	BGMPlayer->Load(_ch,BGMData[_ch]);

	s32 TrackNum = _track;
}

void TMusicPlayer::Mute( bool _onoff )
{
	if( (! Muted) && (_onoff) ) {
		BGMPlayer->SetMasterVolume(0.0f);
		Muted = true;
	}
	else if( (Muted) && (! _onoff) ) {
		BGMPlayer->SetMasterVolume(bgmVol*0.1f);
		Muted = false;
	}

}

void TMusicPlayer::FadeIn( s32 _ch, f32 _vol, f32 _sec )
{
	if( (! Playing[_ch]) && (bgmVol>0) ) {
		BGMPlayer->Stop(_ch);

		// 再生
		BGMPlayer->SetMasterVolume(bgmVol*0.1f);
		BGMPlayer->SetVolume(_ch,0,0);

		BGMPlayer->Play(_ch,true);

		BGMPlayer->SetVolume(_ch,_vol, _sec);

		Playing[_ch] = true;
	}

}

void TMusicPlayer::SetVolume( s32 _vol )
{
	BGMPlayer->SetMasterVolume(_vol*0.1f);

}

void TMusicPlayer::PlayOnce( s32 _ch )
{
	if( (! Playing[_ch]) && (bgmVol>0) ) {
		BGMPlayer->Stop(_ch);

		// 再生
		BGMPlayer->SetMasterVolume(bgmVol*0.1f);
		BGMPlayer->SetVolume(_ch,1.0f);

		//再生後すぐに音が鳴るようにプライマリバッファにデータを格納しておく
		BGMPlayer->Rendering;
		MuStream.BlockCopy(0,BGMPlayer->Offset,KPLAYER_BUFFERLENGTH / 2);
		MuStream.Buffer.SetCurrentPosition(0);

		//ループしない
		BGMPlayer->Play(_ch,false);

		Playing[_ch] = true;
	}

}
