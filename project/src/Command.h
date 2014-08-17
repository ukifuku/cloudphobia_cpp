//^( +)([^\(\n]+):Boolean;
//$1bool $2


#pragma once

using namespace std;

#include "types.h"
#include "TOBJ.h"
#include <vector>

class TMojiAndGlassCursor;
class TBlur;
class TMSGauge2;
class TCursorList;
class TArrow;
class TConnectingGauge;
class TGlassCursor;
class TBustShot;
class TSprSentence;
class TGothic_counter;
class TSea;  //海
class TUnderSea;  //
class TDawncloud;  //雲
class TFriend;
class TStage2ScrollBlur;
class TMotherShip;
class TDemoEraser;
class TGlassCubeFilter;
class TStSelCursor;
class TKeyBoard;

struct ConfigCursor //コンフィグカーソル管理構造体
{
	bool Horizontal;       //左右選択式か
	s32 Current;          //現在の選択値
	vector<string> Caption;           //現在の設定値名称
	TMojiAndGlassCursor* Owner;    //親カーソル
	TMojiAndGlassCursor* Spr;           //対応するスプライト
};

//ゲームオーバー画面クラス
class TGameOver : public TOBJ
{
private:
	s32 DeadPattern;  //死因
	TBlur* Blur;
	s32 ActFlg;
	TCursorList* CursorList;
	TMSGauge2* MSGauge;
public:
	TGameOver(TOBJList* owner, s32 _dp);
	~TGameOver();
	void Move();
	void Input();
	void Die();
};

//Ready画面クラス
class TReady : public TOBJ{
private:
	s32 ReadyTime;
	TCursorList* Cursor;
	s32 bandWidth;
public:
	TReady(TOBJList* owner, s32 _time);
	~TReady();
	void Move();
};

//ダイイングメッセージ
class TDying : public TOBJ{
private:
public:
	TDying(TOBJList* owner);
	void Move();
	void Input();
};

//ランキング画面クラス
class TRanking : public TOBJ{
private:
	s32 Choice;
	s32 PlayerScore;
	bool StickFlgX,StickFlgY; //スティックが押されているか
	s32 ActCount[10];
	s32 ActFlg[10];
	TPoint BarPos[100];
	s32 BlinkNum;    //点滅させる順位
public:
	TRanking(TOBJList* owner, s32 _score);
	void Move();
	void Input();
	void RankIn();
};

//ネットランキング画面クラス
class TNetRanking : public TOBJ{
private:
	s32 Choice;
	bool StickFlgX,StickFlgY; //スティックが押されているか
	s32 ActCount[10];
	s32 ActFlg[10];
	s32 BlinkNum;    //点滅させる順位
	vector<NetRankingData> ScoreRanking;
	vector<NetRankingData> TimeRanking;
	s32 TotalScoreData;   //総スコアデータ数
	s32 TotalTimeData;    //総タイムデータ数
	s32 ConnectResult;    //接続結果
	s32 OrgX,OrgY;    //表示位置
	s32 DestX,DestY;  //移動先位置
	s32 XSpeed,YSpeed;   //カーソル移動スピード
	s32 ScoreRankLine,TimeRankLine;   //ランキング位置
	s32 SameID_Score,SameID_Time;		//重複IDがあった場合、その位置
	vector<TMojiAndGlassCursor*> CursorList;           //項目リスト
	//    Blur:TBlur;
	s32 TotalScoreData_local;   //総スコアデータ数
	s32 TotalTimeData_local;    //総タイムデータ数
	s32 ScoreRankLine_local,TimeRankLine_local;   //ランキング位置
	TArrow* Arrow_Left;
	TArrow* Arrow_Right;
	TConnectingGauge* ConnectGauge;
	bool IsConnecting;
	s32 ErrorNo;
	void MakeRankingItems(const NetRankingData _rankdata[], s32 _totaldata, s32 _rankline, s32 _sameid);
	void MakePartition(TMojiAndGlassCursor* _cursor);
public:
	TNetRanking(TOBJList* owner);
	~TNetRanking();
	void Move();
	void Input();
	void RankIn();
	void RankIn_local();
	void RankIn_local2(NetRankingData scorerank[], NetRankingData timerank[], s32 size);
	void TestRankIn();
	void Die();  
	void MakeItemName();
	void MakeSentence(const string& _caption, s32 _x, s32 _y, s32 _delay);
	void SetNextState();
	void OnResponse( stringstream& _res );
};

//ネットランキングリプレイ閲覧クラス
class TNetRankingReplayViewer : public TOBJ{
private:
	s32 Choice;
	bool StickFlgX,StickFlgY; //スティックが押されているか
	s32 ActCount[10];
	s32 ActFlg[10];
	vector<NetRankingData> RankingData;
	s32 TotalData;   //総データ数
	s32 ConnectResult;    //接続結果
	TGlassCursor* Cursor;  //カーソル
	s32 OrgX,OrgY;    //表示位置
	s32 DestX,DestY;  //移動先位置
	s32 SlideTime;  //移動するフレーム数
	s32 XSpeed,YSpeed;   //カーソル移動スピード
	s32 KeyUpCount,KeyDownCount;  //キー押下フレーム数
	vector<TMojiAndGlassCursor*> CursorList;           //項目リスト
	vector<TMojiAndGlassCursor*> BGItemList;           //背景オブジェクト（項目名、バーなど）リスト
	s32 CurrentItems;          //現在表示している項目数(ページ切り替え時に利用)
	s32 CurrentItemNumber;  //現在表示している項目番号(ページ切り替え時に利用)
	s32 CurrentStatus;      //現在の画面遷移
	s32 RankingMode;        //スコア／タイムランキングの区別
	TBustShot* BustShot_Rifle;
	TBustShot* BustShot_Blade;
	TConnectingGauge* ConnectGauge;
	bool IsConnecting;
	s32 CurrentStage;
	void AddNextItems();
	void Input();
	void GetData();
	s32 GetReplay(s32 _choice);
	void ShowDetail();
	void MakeRightItems();
	void MakeLeftItems();
	void SetNextStatus(s32 _next);
	void ClearScreen();
	void UpdateCurrentItems();
	void MakePartition(TMojiAndGlassCursor* _cursor);
	void MakeItemName();
	void MakeSentence(const string& _caption, s32 _x, s32 _y, s32 _delay);
	void ClearBGItems();
	void MakeCursor(const string& _caption, s32 _x, s32 _y, s32 _long, s32 _pat, s32 _font, s32 _delay);
	void UpdateBustShot();
	void ReceiveRanking(stringstream& _res);
	void ReceiveReplay(stringstream& _res);
	void CopyToLocal(s32 _choice);
	void DeleteData();
public:
	TNetRankingReplayViewer(TOBJList* owner);
	~TNetRankingReplayViewer();
	void Move();
	void Die(); 
	void OnResponse(stringstream& _res);
};

//コンフィグ画面リメイク
class TConfig2 : public TOBJ{
private:
	s32 Choice;   //何番目のカーソルが選ばれている？
	bool StickFlgX,StickFlgY; //スティックが押されているか
	s32 ActCount[10];
	s32 ActFlg[10];
	s32 TempKey[4];  //キーアサイン用
	vector<ConfigCursor>	SettingList;
	TGlassCursor* Cursor;  //カーソル
	s32 OrgX,OrgY;    //表示位置
	s32 KeyUpCount,KeyDownCount;  //キー押下フレーム数
	vector<TMojiAndGlassCursor*> CursorList;           //項目リスト
	s32 CurrentItems;          //現在表示している項目数
	s32 CurrentStatus;      //現在の画面遷移
	bool ButtonFlg_KeyConfig[16];
	TArrow* Arrow_Left;
	TArrow* Arrow_Right;
	TGlassCubeFilter* GlassCubes;
	s32 DecidedTime;
	bool tmpExEffects;
	TCursorList* Caution;      //チート設定時の警告
	void Input();
	void Endprocedure();
	void ClearToRight();
	void ClearToLeft();
	void SetNextStatus(s32 _next);
	void MakeCursor(const string& _caption, s32 _x, s32 _y, s32 _long, s32 _pat, bool _horizontal);
	void AddSetting(s32 _num, const string& _caption);
	void UpdateSetting(s32 _rx, s32 _ry, s32 _num, s32 _val, s32 _delay);
	void PrevStatus();
	void OnDecided();
	void IncorporateSettings();
	void KeyConfig();
	void MakeSentence(const string& _caption, s32 _x, s32 _y, s32 _delay);
	void ArrowControl();
	void CheckCheat();
public:
	TConfig2(TOBJList* owner);
	~TConfig2();
	void Move();
	void Die(); 
};

//ポーズ画面リメイク
class TPause2 : public TOBJ{
private:
	s32 Choice;   //何番目のカーソルが選ばれている？
	bool StickFlg; //スティックが押されているか
	s32 ActCount[10];
	s32 ActFlg[10];
	TGlassCursor* Cursor;  //カーソル
	s32 KeyUpCount,KeyDownCount;  //キー押下フレーム数
	void Input();
	void OnDecided();
protected:
	TBlur* Blur;
	s32 OrgX,OrgY;    //表示位置
	vector<TMojiAndGlassCursor*> CursorList;           //項目リスト
	s32 CurrentItems;          //現在表示している項目数
	void MakeCursor(const string& _caption, s32 _x, s32 _y, s32 _num);
public:
	TPause2(TOBJList* owner);
	~TPause2();
	void Move();
	void Die();
};

//武器選択画面
class TWeaponSelect : public TOBJ{
private:
	s32 Choice;   //何番目のカーソルが選ばれている？
	bool StickFlgX, StickFlgY; //スティックが押されているか
	s32 ActCount[10];
	s32 ActFlg[10];
	s32 OrgX,OrgY;    //表示位置
	s32 KeyUpCount,KeyDownCount;  //キー押下フレーム数
	TSprSentence* Sentence[8];           //項目リスト
	s32 CurrentItems;          //現在表示している項目数
	TBustShot* BustShot_Rifle;
	TBustShot* BustShot_Blade;
	s32 DecidedTime;
	s32 CurrentStage;
	TStSelCursor* StSel;
	s32 maxstage;
	void Input();
	void MakeCursor(const string& _caption, s32 _x, s32 _y, s32 _num);
	void OnDecided();
	void UpdateBustShot();
	void Slide();
public:
	TWeaponSelect(TOBJList* owner);
	void Move();
	void Die();
	bool IsSkipRequied();
};

//リザルト画面リメイク
class TResult2 : public TOBJ{
private:
	//    Blur:TBlur;
	s32 destbonus,timebonus,lifebonus,ndbonus,hitbonus,total;
	s32 LifeRate; //残りライフ割合
	TCursorList* CursorList;
	void MakeItem(const string& _caption, const string& _mul, s32  _score, s32 _num);
public:
	TResult2(TOBJList* owner);
	~TResult2();
	void Move();
	void Die();
	void Input();
};

//ネット接続確認画面
class TConnectAsk : public TOBJ{
private:
	TCursorList* CursorList;
	TGlassCursor* Cursor;
	bool StickFlg;
	void OnDecided();
	void Input();
public:
	TConnectAsk(TOBJList* owner);
	~TConnectAsk();
	void Move();
	void Die();
};

//ローカルランキング画面クラス
class TLocalRanking : public TOBJ{
private:
	bool StickFlgX,StickFlgY; //スティックが押されているか
	s32 ActCount[10];
	s32 ActFlg[10];
	vector<TMojiAndGlassCursor*> CursorList;           //項目リスト
	s32 OrgX,OrgY;    //表示位置
	s32 DestX,DestY;  //移動先位置
	s32 XSpeed,YSpeed;   //カーソル移動スピード
	s32 TotalScoreData_local[MAXSTAGE];   //総スコアデータ数
	s32 TotalTimeData_local[MAXSTAGE];    //総タイムデータ数
	TArrow* Arrow_Left;
	TArrow* Arrow_Right;
	s32 CurrentStage;
	s32 CurrentItems;
	void MakeRankingItems(NetRankingData _rankdata[], s32 _totaldata);
	void MakeItemName();
	void MakeSentence(const string& _caption, s32 _x, s32 _y, s32 _delay);
	void Input();
	void ChangeOrder(s32 _order);
public:
	TLocalRanking(TOBJList* owner);
	~TLocalRanking();
	void Move();
	void Die();  
};

//ネームエントリーリメイク
class TNameEntry2 : public TOBJ{
private:
	bool StickFlgX,StickFlgY; //スティックが押されているか
	s32 ActCount[10];
	s32 ActFlg[10];
	TCursorList* CursorList;           //項目リスト
	s32 SelectName[8];
	bool PlayFlg; //ネームエントリー後にゲームスタートするか
	s32 KeyUpCount,KeyDownCount;  //キー押下フレーム数
	s32 KeyLeftCount,KeyRightCount;  //キー押下フレーム数
	TGlassCursor* Cursor;
	TCursorList* CursorList_Name; //名前表示用カーソルリスト
	s32 DecidedTime;
	TGlassCursor* Cursor_Name;
	TGlassCubeFilter* GlassCubes;
	void Input();
	void OnDecided();
	void MakeChar(s32 _moji, s32 _x, s32 _y, s32 _num);
	void UpdateName(bool _blink);
	void Back();
	void EndProcedure();
public:
	TNameEntry2(TOBJList* owner, bool _play);
	~TNameEntry2();
	void Move();
	void Die();
};

//リプレイ保存確認画面リメイク
class TReplayAsk2 : public TOBJ{
private:
	TCursorList* CursorList;
	TGlassCursor* Cursor;
	bool StickFlg;
	void OnDecided();
	void Input();
public:
	TReplayAsk2(TOBJList* owner);
	~TReplayAsk2();
	void Move();
	void Die();
};

//ローカルリプレイ閲覧画面リメイク
class TLocalReplayViewer : public TOBJ{
private:
	s32 Choice;
	bool StickFlgX,StickFlgY; //スティックが押されているか
	s32 ActCount[10];
	s32 ActFlg[10];
	s32 TotalData;   //総データ数
	TGlassCursor* Cursor;
	s32 OrgX,OrgY;    //表示位置
	s32 DestX,DestY;  //移動先位置
	s32 SlideTime;  //移動するフレーム数
	s32 XSpeed,YSpeed;   //カーソル移動スピード
	s32 KeyUpCount,KeyDownCount;  //キー押下フレーム数
	vector<TMojiAndGlassCursor*> CursorList;           //項目リスト
	vector<TMojiAndGlassCursor*> BGItemList;           //背景オブジェクト（項目名、バーなど）リスト
	s32 CurrentItems;          //現在表示している項目数(ページ切り替え時に利用)
	s32 CurrentItemNumber;  //現在表示している項目番号(ページ切り替え時に利用)
	s32 CurrentStatus;      //現在の画面遷移
	TBustShot* BustShot_Rifle;
	TBustShot* BustShot_Blade;
	void AddNextItems();
	void Input();
	s32 GetReplay(s32 _choice);
	void ShowDetail();
	void MakeRightItems();
	void MakeLeftItems();
	void SetNextStatus(s32 _next);
	void ClearScreen();
	void UpdateCurrentItems();
	void MakeItemName();
	void MakeSentence(const string& _caption, s32 _x, s32 _y, s32 _delay);
	void ClearBGItems();
	void MakeCursor(const string& _caption, s32 _x, s32 _y, s32 _long, s32 _pat, s32 _font, s32 _delay);
	void UpdateBustShot();
	void GetData();
public:
	TLocalReplayViewer(TOBJList* owner);
	~TLocalReplayViewer();
	void Move();
	void Die(); 
};

//チュートリアル
class TTutorial : public TOBJ{
private:
	TCursorList* CursorList;
	bool ButtonFlg_local;
	void OnDecided();
	void Input();
public:
	TKeyBoard* KeyBoard;
	TTutorial(TOBJList* owner);
	~TTutorial();
	void Move();
	void Die();
	void UpdateMessage(const string& _msg, s32 _x, s32 _y, s32 _blank, s32 _width, s32 _num);
};

//ステージ１開始前デモ
class TStage1StartingDemo : public TOBJ{
private:
	TFriend* friends[10];
	TMotherShip* mother;
	TDemoEraser* eraser;
	bool ButtonFlg_local;
	void OnEraser();
	void Input();
	void Skip();
	void SetNextState();
	void Makefriends();
public:
	TStage1StartingDemo(TOBJList* owner);
	~TStage1StartingDemo();
	void Move();
};

//ステージ2開始前デモ
class TStage2StartingDemo : public TOBJ{
private:
	TFriend* friends[10];
	TFriend* captain;
	bool ButtonFlg_local;
	TStage2ScrollBlur* Blur;
	s32 boostedcount;
	void Input();
	void Skip();
	void SetNextState();
	void Makefriends();
	void ReseTFriendPos();
public:
	TStage2StartingDemo(TOBJList* owner);
	~TStage2StartingDemo();
	void Move();
};

//ステージ3開始前デモ
class TStage3StartingDemo : public TOBJ{
private:
	TFriend* friends[5];
	bool ButtonFlg_local;
	s32 boostedcount;
	void Input();
	void Skip();
	void SetNextState();
	void MakeFriends();
	void ResetFriendPos();
public:
	TStage3StartingDemo(TOBJList* owner);
	~TStage3StartingDemo();
	void Move();
};

//ステージ4開始前デモ
class TStage4StartingDemo : public TOBJ{
private:
	TFriend* friends[3];
	bool ButtonFlg_local;
	s32 boostedcount;
	void Input();
	void Skip();
	void SetNextState();
	void Makefriends();
	void ResetFriendPos();
public:
	TStage4StartingDemo(TOBJList* owner);
	~TStage4StartingDemo();
	void Move();
};


//潜水艦デモ
class TSubmarineDemo : public TOBJ{
private:
	bool ButtonFlg_local;
	void Input();
	void Skip();
public:
	TSubmarineDemo(TOBJList* owner);
	void Move();
	void Die();
};

//ステージ4終了デモ
class TStage4EndingDemo : public TOBJ{
private:
	bool ButtonFlg_local;
	s32 ActFlg;
	void SetNextState();
public:
	TStage4EndingDemo(TOBJList* owner);
	~TStage4EndingDemo();
	void Move();
};

//ステージ5開始前デモ
class TStage5StartingDemo : public TOBJ{
private:
	TFriend* friends[5];
	TFriend* captain;
	bool ButtonFlg_local;
	s32 boostedcount;
	void Input();
	void Skip();
	void SetNextState();
	void MakeFriends();
	void ResetFriendPos();
public:
	TStage5StartingDemo(TOBJList* owner);
	~TStage5StartingDemo();
	void Move();
};

//ステージセレクト画面制御
class TStageSelectScreenMng : public TOBJ{
private:
	TSea* Stage2Sea;  //海
	TUnderSea* Stage2UnderSea;  //
	TDawncloud* Stage2Cloud;  //雲
	s32 CurrentStage;
	f32 TotalSlide;
public:
	TStageSelectScreenMng(TOBJList* owner);
	~TStageSelectScreenMng();
	void Move();
	void ChangeStage(s32 _stage);
	s32 GetCurrentStage();
};

//タイトル画面リメイク
class TTitle2 : public TOBJ{
private:
	s32 Choice;   //何番目のカーソルが選ばれている？
	bool StickFlgX,StickFlgY; //スティックが押されているか
	s32 ActCount[10];
	s32 ActFlg[10];
	s32 OrgX,OrgY;    //表示位置
	s32 KeyUpCount,KeyDownCount;  //キー押下フレーム数
	TCursorList* CursorList;           //項目リスト
	s32 CursorSpr,ShadowSpr;
	f32 CursorZoom, ShadowZoom;
	TSprSentence* versionSpr;
	TSprSentence* copyright;
	s32 ButtonUpCount;    //ボタンを押してない時間
	void Input();
	void OnDecided();
	void MakeItems();
	void CursorMove(s32 _num);
public:
	TTitle2(TOBJList* owner);
	~TTitle2();
	void Move();
	void Die(); 

	static const s32 FirstY = 224;
	static const s32 Ybrank = 28;
};

//回復画面
class TRecover : public TOBJ{
private:
	TCursorList* CursorList;
	s32 tempScore;
	TGothic_counter* ScoreSprite;
	s32 Deposit;
	s32 HPNorma, MSNorma, MisNorma;
	s32 HPNext, MSNext, MisNext;
	void CalcRecover(s32 _val);
	void MakeItem(const string& _caption, const string& _mul, const string& _unit, s32 _score, s32 _num);
public:
	TRecover(TOBJList* owner);
	~TRecover();
	void Move();
	void Die();
	void Input();
};

//フェードアウト→次の面へ
class TFadeOuttoNextStage : public TOBJ{
private:
public:
	TFadeOuttoNextStage(TOBJList* owner);
	void Move();
};

//難易度選択画面
class TDifficultySelect : public TOBJ{
private:
	TCursorList* CursorList;
	TGlassCursor* Cursor;
	bool StickFlg;
	void OnDecided();
	void Input();
public:
	TDifficultySelect(TOBJList* owner);
	~TDifficultySelect();
	void Move();
	void Die();
};

#define		PSSMode    0

//title
#define		TIni       0

//BarPos
#define		PTitle     0

//config
#define		CKey0      0
#define		CKey1      1
#define		CKey2      2
#define		CKey3      3
#define		CBoost     4
#define		CFScreen   5
#define		CFilter    6
#define		CCloud     7  //いらんかも
#define		CExit      7

//接続結果
#define		KCONNECT_SUCCESS         0  //成功
#define		KCONNECT_NONE            1  //未接続
#define		KCONNECT_ERROR           -1  //接続エラー
#define		KCONNECT_NOTFOUND        -2  //ファイルなし

#define		FirstYPos				104        // カーソル初期位置
#define		YSPACE					 40            // カーソル間隔
#define		YSPACE_HALF				 (YSPACE / 2);            // カーソル間隔
#define		FirstYPos_Ranking		 104        // ランキング　カーソル初期位置
#define		FirstYPos_LocalRanking	 56        // ローカルランキング　カーソル初期位置


//ネットランキング全般
#define		KNETRANK_COMMOM_MAXRANK         100   //クライアント側で表示できる最大順位

//ネットランキング(プレイ終了時)項目数
#define		KNETRANK_PLAY_MAX				 8   //最大表示数
#define		KNETRANK_PLAY_RANKIN             4    //ランクインした場合、何番目に表示するか

//ネットランキング(リプレイ閲覧)項目数
#define		KNETRANK_REP_MAX             8   //最大表示数

//ネットランキング(リプレイ閲覧)画面遷移
#define		KNETRANK_SCR_STAGE           0        //ステージ選択
#define		KNETRANK_SCR_ORDER           1        //スコア順／タイム順選択
#define		KNETRANK_SCR_RANKING         2        //ランキング画面
#define		KNETRANK_SCR_DETAIL          3        //詳細画面
#define		KNETRANK_SCR_DETAIL_RANK     4        //詳細→ランキングへ戻る
#define		KNETRANK_SCR_ERROR           5        //接続エラー
#define		KNETRANK_SCR_NOTFOUND        6        //ファイルなし

//コンフィグ画面遷移
#define		KCONFIG_SCR_TOP             0        //
#define		KCONFIG_SCR_GAMESETTING     1        //
#define		KCONFIG_SCR_CONTROL         2
#define		KCONFIG_SCR_GRAPHIC         3        //
#define		KCONFIG_SCR_SOUND           4        //
#define		KCONFIG_SCR_NETWORK         5        //

//ウェポンセレクト用定数
#define		KWSELECT_CURSOR_TOP         240        //
#define		KWSELECT_CURSOR_WIDTH       180        //
#define		KWSELECT_CURSOR_LEFT        214

#define		MAX_FRIENDS					(sizeof(friends)/sizeof(TFriend*))