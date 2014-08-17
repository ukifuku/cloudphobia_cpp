#pragma once

using namespace std;

#include <string>
#include <vector>
#include <list>
#include "types.h"
#include "SXLib9.h"
#include "Define.h"
#include "Wave.h"
#include "JoyScan.h"
#include "Replay.h"

class TOBJ;
class TThread;
class TBG3DManager;
class TSoundEffect;
class TMusicPlayer;
class TReplay;
class TSpriteBase;

//オブジェクト管理クラス
class	TOBJList {
private:
	TDGCarad*	m_dddd;
	list<TOBJ*> m_objList;
	TSpriteEXManager*	m_SprMan;
	TSceneManager*		m_Scene;
	TBG3DManager*		BG3DMan;

	s32 FTick;      //ゲーム開始から何フレーム経ったか？
	s32 CDownTick;       //カウントダウンをはじめる時間
	//ボステクスチャ保存サーフェス
	// BossBmp:TBitmap;
	//BossBumpBmp:TBitmap;

	bool	DataLoaded[100];
	s32		OBJIDList[4096];     //IDリスト
	s32		QuakePower;   //画面揺らし量
	u32		TotalRendCount,TotalFrameCount;    //平均FPS算出用
	bool	QuakeFlg;   //オブジェクトを揺らす
	s32		QuakeTime; //揺らし始めた時間
	s32 	QuakeLife; //揺れる時間
	u32		RondomSeed;
	TMainLoopEvent   AfterLoadingEvent;     //ロード完了時のイベント種別
	TOBJ*	LoadingScreen;
	TThread*	LoadThread;

	s32		StageScriptID;

	bool	IsRequestedDelBullets;

	s32		HUDHideCount;

	void Event();
	void EventReload();
	void Setup();
	void Hantei();
	void SetStage( s8 StageNum );
	void LoadStage( s8 StageNum, TMainLoopEvent _event );
	void ReleaseStage();
	void QuakeManager();
	void CameraReset();
	void CameraAngleReset();
	void LoadSpriteData(const string& _file);
	void LoadMotionData(const string& _file);
	void CreateRestart();
	void InitPlayerGauge();
	void DeleteBullets();
	void OnAfterLoop();
	void HUDVisibleControl();
public:
	TOBJ*	DemoMan;
	s32		HiScore;   //ハイスコア
	TPoint	HeroPos;    //自機の位置
	bool	HeroDie;   //自機が死んだ
	s32		HeroHP;    //自機HP
	s32		TotalScore;//スコア
	bool	Muteki;    //無敵フラグ
	bool	Weapon;    //武器使用の可否
	WaveSet WS;
	WaveSet	WS_Seq[4];
	s32		TotalLock; //ロックオンしている数
	s32		LockID[MaxLock*MissileStock];   //ロックしたオブジェクトのID
	TPoint	BeLocked[MaxLock*MissileStock];   //ロックオンされたオブジェクトの位置
	bool	UnLock[MaxLock*MissileStock];     //ロックオンがはずれた
	bool	MissileON; //ミサイル使用中
	s32		RestMis;   //残りミサイル
	s32		LimitTime; //制限時間
	s32		TotalObj;    //総オブジェクト数
	s32		TotalHit;  //総ヒット数
	bool	TimeStop;   //時よ止まれ
	TGameMode 	GameMode; //ゲームの状態-
	s32		Speed;      //スクロールスピード
	s32		Distance;   //飛行距離
	TPoint	BGSlide;    //1フレームあたりの背景スクロール量
	s32		MShipHP;   //母艦の体力
	bool	ShowHit;   //当たり判定を表示するか
	TFunSignal	FunSignal; //ファンネルへの指示
	s8		FunPattern;  //ファンネルの攻撃パターン
	s32		FunSync;      //ファンネル同期用変数
	bool	BossDie;      //ボスが死んだ
	s32		BossRate;     //ボス得点倍率
	s32		BDieTime;     //ボスが死んだ時間
	s32		BossR;        //ボスの角度
	bool	DontBoost;    //ブースト禁止
	TJoyPad	Joy;
	s32		PlayerName[8];  //スコアネーム
	TMainLoopEvent AfterLoopEvent;
	s32		Z4AppX[100];  //雲ザコ出現位置
	s32		Z4AppY[100];  //雲ザコ出現位置
	s32		RandZYDist[8];  //ランダムザコ出現位置、カウント
	s32		RandZCount[8];  //ランダムザコ出現位置、カウント
	s32		FunXDist[256];    //ファンネル位置
	s32		FunYDist[256];    //ファンネル位置
	bool	DontRender;        //レンダリングしてもよいか
	bool	ResultExists;      //リザルト画面が出たか
	bool	FlareOK;       //レンズフレアを出してよい
	s8		HeroWeapon;  //自機ウェポンタイプ

	TReplay*	Replayer;  //リプレイクラス
	bool	ReplayON;  //リプレイ中か？
	s32		NetReplayState;  //どの面のネットリプレイを再生中か(リプレイ終了後に一覧画面に戻るため)

	bool	CheatON;   //チート（無敵とかスローとか）使用中か
	s8		NowStage;    //現在のステージ

	TSoundEffect*	SoundEffect;
	TMusicPlayer*	MusicPlayer;
	//TIdHTTP*	Network;
	s32		FrameSkipRate;

	bool	ScreenshotFlg;

	s32		ComBonusCount;    // 画面上にコマンダーボーナスが何個表示されているか

	s32		TempCount[10];  //雑用変数

	TGameSpeed 	GameSpeed;

	bool	DemoON;
	s32		DemoNumber;

	TOBJ*	JikiObj;     //自機オブジェクトのポインタ

	bool	AcceptChangeSpeed;    //スクロールスピード変更をMain側で行うか
	s32		RequestedSpeedFromChild;    //外部からスピード変更依頼を受ける場合の値

	vector<TSpriteDataList> 	SpriteDataList;
	vector<TMotionData> 	MotionDataList;

	s32		StartDistance;  //スタート時の飛行距離

	bool	ResultSkipped;

	TOBJ*	BloomingFilter;

	TOBJ*	StageSelect;

	TRepFileData 	RankinHeaderData;

	s32		Difficulty;
	s32		SequenceTotalTime;

	bool	IsRecovering;

	bool	IsPractice;
	bool	IsCleared[MAXSTAGE_SEQ][2];    //クリアー情報
	string	NetworkID;

	bool	HUDHideFlg;

	TOBJList( TDGCarad* _dddd );
	~TOBJList();
	//TOBJ* getOBJFromID(s32 id);		// ★★★廃止すること
	bool ConfigSave();
	bool ConfigLoad();
	string ScoreToStr(s32 _score);
	string PlayerNameToStr(const s32 _name[], s32 _size);
	string DateToStr(s32 _year, s32 _month, s32 _day);
	string TimeToStr(s32 _time);
	string FrameSkipRateToStr(s32 _fsr);
	s32 GetRand(s32 _max);
	void Move();
	void Render();
	void GameStart();
	void GameReset();
	void Sentence(string moji, s32 fx, s32 fy, s32 brank, s32 _pat);
	void SentenceCenter(const string& moji, s32 fx, s32 fy, s32 brank, s32 _pat);
	void SentenceRight(const string& moji, s32 fx, s32 fy, s32 brank, s32 _pat);
	void ShowScore(s32 score,s32 x, s32 y, bool _red);
	void ShowTime( s32 _x, s32 _y, s32 _time, bool _red);
	void Scrach(TOBJ* target);
	void LockOn(s32 setNumber);   //一括ロックオン
	void LockOn2(s32 setNumber, s32 MisNumber);   //逐次ロックオン
	void Input();
	void Quake(s32 _power, s32 _time);
	void Console();
	void PlayStart();
	void GoTitle();
	void ScanKey();
	void ReleaseBeforeReset();
	void InitAfterReset();
	void CulcFrameSkipRate();
	//    property Objs[id:Integer]:TOBJ read getOBJ;
	s32 GetTick() { return FTick; }
	void ScreenShot();
	void OnFrameSkipped();
	TDGTexture* GetExTexture();
	TDGTexture* GetAlphaTexture();
	TDGTexture* GetAddTexture();
	void PrepareBeforeDevLost();
	void ResetAfterDevLost();
	void CreateHUD();
	void InitBeforePlayStart();
	void TimeOver();
	void StageEnd();
	void SetBloomingColor(s32 _add, s32 _alpha, s32 _r, s32 _g, s32 _b);
	void SetBloomingColorSoon(s32 _add, s32 _alpha, s32 _r, s32 _g, s32 _b);
	void OnLoaded( void* Sender);
	void LoadInternal();
	void SetRankInHeader();
	bool IsSequence();
	void RequestDelBullets();
	bool IsNormalCleared;
	bool IsBossDefeated(s32 _stage);
	bool IsBossDefeated_Seq(s32 _stage);
	void Add(TOBJ* obj);
	void AddSprite(TSpriteBase* _spr);
	TSceneManager* GetScene() { return m_Scene; }

	TDGCarad* GetDG() { return m_dddd; }
	TSpriteEXManager*	GetSprMan() { return m_SprMan; }
	TBG3DManager*	GetBG3DMan() { return BG3DMan; }
};

typedef list<TOBJ*>::iterator	objlist_itr;

