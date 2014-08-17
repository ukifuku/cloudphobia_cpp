#include "types.h"
#include <vector>

using namespace std;


string BGMFile[] = {"MurdDes.AO",
			"MD_fyu_Mix96.AO",
			"MD_atnen_mix96.AO",
			"MDSlow.AO",
			"FallAp.AO",
			"Stage2a.AO",
			"Stage2b.AO",
			"Stage4a.AO",
			"Stage4b.AO",
			"Stage5a.AO",
			"Stage5b.AO",
			"Stage3.AO",
			"ending.AO"};

class TOggPlayer;
class TOggData;
class TDDSDWaveData;
class TDDSDGenWave;
class TDDSDChannel;

//効果音クラス
class TSoundEffect
{
private:
    vector<TDDSDWaveData*> SE;
public:
    TSoundEffect();
    ~TSoundEffect();
    void WavPlay(s32 _WavNum, s32 _ch);
    void WavPlay2( s32 _WavNum, s32 _ch, s32 _vol);
    void WavStop(s32 _Ch);
    void AllStop();
  };

  
//音楽再生クラス
class TMusicPlayer 
{
private:
    TOggPlayer*		BGMPlayer;
    TOggData*		BGMData[4];
    TDDSDWaveData*	MuStream;
    s32			TrackNum;     //曲番号
public:
    bool	Playing[4];
    bool	Muted;
    TMusicPlayer();
    ~TMusicPlayer();
    void UpdateWav(TDDSDGenWave* Sender, TDDSDChannel* Player, u32 ofs, u32 len);
    void Play(s32 _ch);
    void Stop();
    void FadeOut(s32 _ch, f32 _vol, f32 _sec);
    void BGMChange(s32 _ch, s32 _track);
    void Mute(bool _onoff);
    void FadeIn(s32 _ch, f32 _vol, f32 _sec);
    void SetVolume(s32 _vol);
    void PlayOnce(s32 _ch);
};

#define		BGMCH  15

#define		KBGM_STAGE1_A     0
#define		KBGM_STAGE1_B     1
#define		KBGM_STAGE1_C     2
#define		KBGM_MENU       3
#define		KBGM_CLEAR      4
#define		KBGM_STAGE2_A     5
#define		KBGM_STAGE2_B     6
#define		KBGM_STAGE4_A     7
#define		KBGM_STAGE4_B     8
#define		KBGM_STAGE5_A     9
#define		KBGM_STAGE5_B    10
#define		KBGM_STAGE3    11
#define		KBGM_ENDING    12

#define		KPLAYER_BUFFERLENGTH   44100*2


