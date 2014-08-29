#pragma once

using namespace std;

#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include <vector>
#include "types.h"
#include "glm/glm.hpp"
#include "SXLib9.h"

#define MissileSTR  30 //ミサイル攻撃力
#define MaxLock		8     //最大ロック数(2の倍数)
#define MissileStock  8    //ミサイルセット最大所持数
//NSmoke =512;     //煙の量
#define KSTR_SHOT	5    //ショット攻撃力
#define KSTR_BLADE  90  //ブレード攻撃力

#define KazeSpeed	70    //風素片スピード

#define MaxJoint	49    //最大間接数

#define GainTime	17     //1フレームあたりの時間減少量

#define HPMax_Single	7000          //自機の最大HP     7000
#define MSHPMax_Single  12000       //母艦の最大HP
#define HPMax_Easy		8000
#define MSHPMax_Easy	 12000
#define HPMax_Normal	 6000
#define MSHPMax_Normal	 8000
#define HPMax_Hard		 5000
#define MSHPMax_Hard	 7000
#define BossMaxHP		 3000     //ボスの最大HP     3000
#define BossMaxHP_Short  2000      //ボスの最大HP     3000
#define MaxFrame		 12000      //最大フレーム数（リプレイで保存できる長さ）

#define MAXSTAGE		 8
#define MAXSTAGE_SEQ	 3

const s8 Version = 38;          //ゲームのバージョン（というかリリース番号）
const s8 ScoreVersion = 29;          //スコアのバージョン
const s8 RepVersion = 30;          //リプレイのバージョン
const s8 ConfigVersion = 27;       //コンフィグファイルのバージョン
const s8 IchVersion = 28;       //画像ファイルのバージョン
const s8 Stage1Version = 0;     //ステージ1データのバージョン
const s8 Stage2Version = 0;     //ステージ2データのバージョン
const s8 Stage99Version = 0;     //ステージ99データのバージョン

const string BGMTitle[] = {"murderous desire","murderous desire ; fyu-kosay mix","murderous desire ; at-nen mix"} ;

const string DEBUG_GRAPH_PATH = "c:/data/graphics/gameproject/";

const string NETRANKING_CGI_PATH = "http://www.dwahan.net/uki/cloudphobia/ranking/v028/";
const string NETRANKING_SEQUENCE_CGI_PATH = "http://www.dwahan.net/uki/cloudphobia/ranking/v029/";
//const string NETRANKING_CGI_PATH = "http://localhost/";

const bool DebugMode = false;
const bool TrialMode = false;

const f32 PI = (f32)M_PI;

#define MAXVALUES  8

//効果音
#define WAGraze		  0
#define WALdamage	  1
#define WAHdamage	  2
#define WAGunshot	  3
#define WAMissile	  4
#define WAExp_S		  5
#define WAExp_M		  6
#define WAEdamage0	  7
#define WAExp_L		  8
#define WALock		  9
#define WAWArning	 10
#define WAVScroll	 11
#define WAZ5app		 12
#define WABossExp	 13
#define WABoost		 14
#define WAEBoost	 15
#define WACharge	 16
#define WABeam		 17
#define WAEdamage1	 18
#define WAFunMove	 19
#define WAFunAppear	20
#define WALBeam		 21
#define WAHoming	 22
#define WAHoming2	 23
#define WABShot		 24
#define WATrans		 25
#define WAStart		 26
#define WAZ4app		 27
#define WAheart		 28
#define WADeside	 29
#define WASelect	 30
#define WABonus		 31
#define WABoot		 32
#define WASlash_A	 33
#define WASlash_B	 34
#define WASlash_C	 35
#define WASwing_A	 36
#define WASwing_B	 37
#define WAAlarm		 38
#define WABaseExp	 39
#define WAEngine	 40
#define WAFlash		 41
#define WAShutter_close		42
#define WAShutter_fullopen  43
#define WAShutter_open		44
#define WAShutter_unlock	45
#define WASpydereye  46
#define WASwing_E	 47
#define WATank_dash  48
#define WATank_jump  49
#define WATank_shot  50
#define WAThunder		51
#define WAWarp			52
#define WAWarpout		53
#define WAWarp2			54
#define WAShipCharge    55
#define WASubmarine     56
#define WAShipExp		57
#define WABoss2Laser    58
#define WAShipDamaged	59

#define NCube		30

#define HitBody  1
#define HitBuzz  0
#define HitBlade  17

//デバッグ情報設定種別
#define KDEBUGINFO_FAKEHDR		 0
#define KDEBUGINFO_ENVMAP		 1
#define KDEBUGINFO_EXPLOSION	 2

//デモ番号
#define KDEMONUM_TUTORIAL		 0		//チュートリアル
#define KDEMONUM_STAGE1START	 1		//ステージ１開幕デモ
#define KDEMONUM_STAGE2START	 2		//ステージ2開幕デモ
#define KDEMONUM_STAGE3START	 3		//ステージ3開幕デモ
#define KDEMONUM_STAGE4START	 4
#define KDEMONUM_STAGE2END		 5


//スプライトデータ
#define KSPRDATA_STAGE2BOSS1     0
#define KSPRDATA_STAGE2GUARD     1
#define KSPRDATA_S2GUARDBWS     2
#define KSPRDATA_STAGE2BOSS2     3
#define KSPRDATA_STAGE2ZAKO4     4
#define KSPRDATA_STAGE3BOSS1     5
#define KSPRDATA_STAGE4BOSS2     6
#define KSPRDATA_STAGE4GUARD     7
#define KSPRDATA_STAGE5BOSS1     8
#define KSPRDATA_STAGE5BOSS2     9
#define KSPRDATA_STAGE5BOSS3    10
#define KSPRDATA_FRIEND    11
#define KSPRDATA_STAGE5BOSS2_2     12

//モーション
#define KMOTION_STAGE2BOSS1_DEFAULT    0
#define KMOTION_STAGE2BOSS1_APPEAR1    1
#define KMOTION_STAGE2BOSS1_APPEAR2    2
#define KMOTION_STAGE2BOSS1_FOWARD    3
#define KMOTION_STAGE2BOSS1_BACK      4
#define KMOTION_STAGE2BOSS1_CHARGE    5
#define KMOTION_STAGE2BOSS1_BERSERK    6
#define KMOTION_STAGE2BOSS1_BLADEPREPARE1    7
#define KMOTION_STAGE2BOSS1_BLADEPREPARE2    8
#define KMOTION_STAGE2BOSS1_SLASH    9
#define KMOTION_STAGE2BOSS1_BEAM    10
#define KMOTION_STAGE2BOSS1_BERSERK2    11
#define KMOTION_STAGE2BOSS1_FALL    12

#define KMOTION_STAGE2GUARD_DEFAULT    13
#define KMOTION_STAGE2GUARD_APPEAR1    14
#define KMOTION_STAGE2GUARD_APPEAR2    15
#define KMOTION_STAGE2GUARD_FOWARD    16
#define KMOTION_STAGE2GUARD_BACK      17
#define KMOTION_STAGE2GUARD_SHOT      18
#define KMOTION_STAGE2GUARD_SLASH1    19
#define KMOTION_STAGE2GUARD_SLASH2    20
#define KMOTION_STAGE2GUARD_SLASH1TO2    21

#define KMOTION_STAGE2ZAKO4_DEFAULT   22
#define KMOTION_STAGE2ZAKO4_FOWARD    23
#define KMOTION_STAGE2ZAKO4_BACK      24
#define KMOTION_STAGE2ZAKO4_LEFT      25
#define KMOTION_STAGE2ZAKO4_RIGHT     26

#define KMOTION_STAGE3BOSS_DEFAULT   27
#define KMOTION_STAGE3BOSS_FOWARD    28
#define KMOTION_STAGE3BOSS_BACK      29
#define KMOTION_STAGE3BOSS_SHOT1      30
#define KMOTION_STAGE3BOSS_SHOT2      31
#define KMOTION_STAGE3BOSS_FALL      32

#define KMOTION_STAGE4BOSS_DEFAULT   33
#define KMOTION_STAGE4BOSS_FOWARD    34
#define KMOTION_STAGE4BOSS_BACK      35
#define KMOTION_STAGE4BOSS_SHOT      36
#define KMOTION_STAGE4BOSS_CUTTER1      37
#define KMOTION_STAGE4BOSS_CUTTER2      38
#define KMOTION_STAGE4BOSS_CUTTER3      39

#define KMOTION_STAGE5BOSS1_DEFAULT   40
#define KMOTION_STAGE5BOSS1_SHOT1    41
#define KMOTION_STAGE5BOSS1_SHOT2      42
#define KMOTION_STAGE5BOSS1_MOVE      43
#define KMOTION_STAGE5BOSS1_SITDOWN      44
#define KMOTION_STAGE5BOSS1_JUMP      45
#define KMOTION_STAGE5BOSS1_SHOT3      46
#define KMOTION_STAGE5BOSS1_FALL      47

#define KMOTION_STAGE5BOSS2_DEFAULT   48
#define KMOTION_STAGE5BOSS2_FOWARD      49
#define KMOTION_STAGE5BOSS2_BACK      50
#define KMOTION_STAGE5BOSS2_SLASHL1      51
#define KMOTION_STAGE5BOSS2_SLASHL2      52
#define KMOTION_STAGE5BOSS2_SLASHL1_2      53
#define KMOTION_STAGE5BOSS2_SLASHR1      54
#define KMOTION_STAGE5BOSS2_SLASHR1_2      55
#define KMOTION_STAGE5BOSS2_SLASHR2      56
#define KMOTION_STAGE5BOSS2_LEAD      57
#define KMOTION_STAGE5BOSS2_FALL1      58
#define KMOTION_STAGE5BOSS2_FALL2      59

#define KMOTION_STAGE5BOSS3_DEFAULT   60
#define KMOTION_STAGE5BOSS3_FOWARD      61
#define KMOTION_STAGE5BOSS3_BACK      62
#define KMOTION_STAGE5BOSS3_CHARGE      63
#define KMOTION_STAGE5BOSS3_BEAM      64
#define KMOTION_STAGE5BOSS3_BEAM2      65

#define KMOTION_FRIEND_SWING1      66
#define KMOTION_FRIEND_SWING2      67
#define KMOTION_FRIEND_SWING3      68

#define KMOTION_STAGE5BOSS2_2_DEFAULT   69
#define KMOTION_STAGE5BOSS2_2_FOWARD      70
#define KMOTION_STAGE5BOSS2_2_BACK      71
#define KMOTION_STAGE5BOSS2_2_SLASHL1      72
#define KMOTION_STAGE5BOSS2_2_SLASHL2      73
#define KMOTION_STAGE5BOSS2_2_SLASHL1_2      74
#define KMOTION_STAGE5BOSS2_2_SLASHR1      75
#define KMOTION_STAGE5BOSS2_2_SLASHR1_2      76
#define KMOTION_STAGE5BOSS2_2_SLASHR2      77
#define KMOTION_STAGE5BOSS2_2_LEAD      78
#define KMOTION_STAGE5BOSS2_2_FALL1      79
#define KMOTION_STAGE5BOSS2_2_FALL2      80

//スクロールスピード
#define KSCROLLSPEED_NORMAL    5

//スクリプト
#define KSCRIPT_TUTORIAL      0
#define KSCRIPT_STAGE2ENEMY      1
#define KSCRIPT_NIGHTVISION      2
#define KSCRIPT_STAGE3ENEMY      3
#define KSCRIPT_STAGE4ENEMY      4
#define KSCRIPT_STAGE5ENEMY      5
#define KSCRIPT_STAGE2DEMO      6
#define KSCRIPT_STAGE5DEMO      7
#define KSCRIPT_STAGE2ENEMY_EASY      8
#define KSCRIPT_STAGE3ENEMY_EASY      9
#define KSCRIPT_STAGE4ENEMY_EASY     10
#define KSCRIPT_STAGE2ENEMY_NORMAL      11
#define KSCRIPT_STAGE3ENEMY_NORMAL      12
#define KSCRIPT_STAGE4ENEMY_NORMAL     13
#define KSCRIPT_STAGE2ENEMY_HARD      14
#define KSCRIPT_STAGE3ENEMY_HARD      15
#define KSCRIPT_STAGE4ENEMY_HARD      16

#define KIDOFFSET_BILLBOARD     256    //ビルボードに割り振るメッシュＩＤの初期値
#define KIDOFFSET_EFFECT     512    //特殊エフェクトに割り振るメッシュＩＤの初期値

//難易度
 #define  KDIFFICULTY_EASY    0
 #define  KDIFFICULTY_NORMAL  1
 #define  KDIFFICULTY_HARD    2
 #define  KDIFFICULTY_SINGLE  3

//オブジェクトの種類
enum TOBJKind {
	kHero,			// 自機 
	kHShot,			// 自弾 
	kEnemy,			// 敵
	kEShot,			// 敵弾
	kEffect,		// エフェクト
	kMissile,		// ミサイル
	kEMissile,		// 敵ミサイル
	kGauge ,		// 計器類
	kBG ,			// 背景
	kGene  ,		// 敵生成
	kEffect2,		// スクロールの影響を受けないエフェクト
	kEffect3,       // スクロールの影響は受けないが振動の影響は受ける        
	kEffect4,       // 振動の影響は受けないがスクロールの影響を受ける              
	kFunnel,		// ファンネル
	kColBG			// 当たり判定を持った背景
};

//ゲームの状態 
enum TGameMode {
	gTitle  ,	// タイトル
	gPlay,		// 本編 
	gPause ,	// ポーズ
	gOver ,     // ゲームオーバー
	gReady   ,  // 開始前演出
	gOther		// その他
};

//メインループにイベント要求    　 　                                
enum TMainLoopEvent {
	MNone,			// なし 　
	MPlayStart,		// ゲーム開始
	MRestart  ,		// リスタート
	MTitle,         // タイトルに戻る 
	MPause,			// ストップ
	MMove ,			// ストップ解除
	MRepView,		// リプレイ一覧へ
	MWeaponSelect,  // 武器選択画面 
	MReady  ,		// Ready画面
	MLoadBeforePlay,    // ステージ開始前の読み込み     
	MOpeningMovie,	 // オープニングムービー
	MEndingMovie,	 // エンディングムービー 
	MRestart_Seq	//リスタート（通し面）
};

//ファンネルとの連携    　　           
enum TFunSignal {
	fNone,			// 指示なし
	fDisperse,		// 解散
	fRoll,			// 回転
	fNarrow,		// 狭い鳥かご 
	fSelfDes		// 自爆
};

//ゲームスピード    
enum TGameSpeed {
	sSlow,  
	sNormal,
	sFast
};


class TJoint;
class TFrameManager;
class CEffectBase;
class TDDSD;
class TBitmap;
class TSpriteEXManager;
class TSceneManager;
struct THitArea;

struct OBJParts //いろいろ素片用レコード
{
	s32 X;
	s32 Y;
	s32 Rdeg;
};

struct DebugData 
{
  string FCAPTION[MAXVALUES];
  f32 FMAX[MAXVALUES];
  f32 FMIN[MAXVALUES];
  f32 FCURRENT[MAXVALUES];
};

#define MAX_NAME_LENGTH		8
struct NetRankingData 
{
  s32 Rank;
  s32 Name[MAX_NAME_LENGTH];
  s32 Score;
  s32 Time;
  s32 BoostType;
  s32 WeaponType;
  s32 FrameSkipRate;
  s32 Year;
  s32 Month;
  s32 Day;
  s32 FileNumber;
};

struct NetRankingData_Split 
{
  s32 SplitScore[5];
  s32 SplitTime[5];

  s32 Life[5];
  s32 MShip[5];
  s32 Missile[5];
};

struct TSpriteData //スプライトデータ
{
	TJoint* Spr;      //スプライトのポインタ（画面表示用に使う。ファイル保存はしない）
	s32 ParentID;   //親スプライトID
	string Name;        //名前
	Rect TexRect;      //テクスチャ座標
	TPoint HitRect[4];     //当たり判定
	s32 CX;
	s32 CY;      //回転中心座標
	s32 X;
	s32 Y;        //位置
	s32 Rdeg;       //回転角
	s32 Z;          //Ｚ値
};

struct TSpriteDataList //スプライトデータリスト
{
	s32		 TotalSpr;   //全スプライト数
	string	 BmpFile;     //ビットマップファイル
	vector<TSpriteData> Sprites;
};

struct TPartsMotionData //モーションデータ
{
	TJoint* Spr;      //スプライトのポインタ（画面表示用に使う。ファイル保存はしない）
	string Name;        //名前
	s32 X;
	s32 Y;        //位置
	s32 Rdeg;       //回転角
	s32 Speed;          //スピード
	s32 Direction;    //回転方向
};

struct TMotionData //モーションデータ
{
	s32 TotalSpr;   //全スプライト数
	string Name;        //モーションの名前
	vector<TPartsMotionData> Sprites;
};

struct MeshData //メッシュデータ
{
	string	Name;     //識別名
	string	FileName;  //ファイル名
	s32		TextureID; //テクスチャＩＤ
	TSXMesh*	Mesh; //メッシュ
	s32		BlendMode;  //ブレンドモード
	bool	Lighting; //ライティング有効か
};

struct BBData //ビルボードデータ
{
	string Name;     //識別名
	s32 TextureID; //テクスチャＩＤ
	s32 u;
	s32 v;
	s32 width;
	s32 height;
	s32 BlendMode;  //ブレンドモード
};

struct FrameData //フレームデータ
{
	s32		MeshID; //メッシュＩＤ
	TFrameManager*	FrameMng; //フレーム
	s32		AppearDistance;  //出現位置
	s32		DisappearDistance;  //消滅位置
	bool	Freeze;			//カメラ位置に固定するか
	CEffectBase*	EffectPtr;		//特殊エフェクトを使う場合、そのインスタンスへのポインタ
	s32		HideDistance;   //エディタ上で非表示にする位置
	bool	IsAppeared;
	bool	IsRendered;
	bool	IsDeleted;
	f32		ZX;
	f32		ZY;
	f32		ZZ;
	f32		X;
	f32		Y;
	f32		Z;
	s32		RX;
	s32		RY;
	s32		RZ;

	s32		numParam;   //パラメタ項目数
	vector<string>	ParamName;
	vector<f32>		ParamVal;
};

struct ViewData //視点データ
{
	s32		Distance;
	//カメラ
	f32		X;
	f32		Y;
	f32		Z;       //位置
	s32		RX;
	s32		RY;
	s32		RZ;   //角度
	//  SpeedX,SpeedY,SpeedZ:Single;                //移動スピード
	f32		Speed;
	bool	Spline;   //スプライン曲線モード
};

struct MaterialData //マテリアルデータ
{
	f32		Diffuse;
	f32		Specular;
	f32		Ambient;
	f32		Emissive;
	f32		Power;
	f32		u;
	f32		v;
};

struct EffectData //メッシュデータ
{
	string	Name;     //識別名
	s32		TextureID; //テクスチャＩＤ
	bool	Lighting; //ライティング有効か
	s32		BlendMode;  //ブレンドモード
	s32		numParam;   //パラメタ項目数
	
	vector<string>	ParamName;
	vector<f32>		ParamVal;
};


//TAstat = packed record
//  adapt: TAdapterStatus;
//  NameBuff: array [0 .. 29] of TNameBuffer;
//end;
//


s32		NewID = 0;
u32		VFPSCount;

//インターフェース
TDGCarad*	QD;
TDDSD*		SD;

//テクスチャ
//GaugeTex:TDDDDTexture;    //ゲージ
TDGTexture		*JikiTex;          //自機
TDGTexture		*FontTex, *FontTex_Gothic;            //フォント
TDGTexture		*Eff1Tex, *Eff2Tex, *Eff3Tex, *Eff4Tex, *Eff5Tex, *Eff6Tex, *Eff7Tex, *Eff8Tex, *Eff9Tex;//エフェクト
TDGTexture		*MechaTex;     //敵
TDGTexture		*Enemy4Tex, *Enemy5Tex;
//BGSource:TDGTexture;
//BGTex:TDGTexture;
TDGTexture		*BGTex1, *BGTex2;     //背景
TDGTexture		*ST2Tex1, *ST2Tex2;     //ステージ２テクスチャ
TDGTexture*		BumpTex;
TDGTexture*		BossBump;
TDGTexture*		GaugeTex;
TDGTexture*		SunTex;
TDGTexture*		DemoTex;   //デモ用テクスチャ
TDGTexture*		MissileTex;

TDGTexture*		BShotTex1, *BShotTex2; //バストショット用テクスチャ
TDGTexture*		BShotBumpTex1, *BShotBumpTex2; //バストショット用テクスチャ（バンプ）

//メッシュ
TSXMesh* Board;        //板
TSXMesh* CloudMesh;       //
TSXMesh* Undersea, *Undersea2;     //
TSXMesh* Building[NCube];       //ビル
TSXMesh* Rock[NCube];       //岩
TSXMesh* GCube[NCube];       //ガラスキューブ
TSXMesh* MissileMesh;

//背景用サーフェス
//BGSurface:TDDDDSurface;

//ボステクスチャ保存サーフェス
//TBitmap*		BossBmp;
//TBitmap*		BossBumpBmp;


// スプライトマネージャ
//TSpriteEXManager*	SprMan;

//ポリゴンマネージャ
//TSceneManager*		Scene;

//フルスクリーンにしなおすべきか
bool	FullScreenOK;

bool	ButtonFlg[16];           //ボタンが押されたか
s32		AssinedKey[16];         //キーアサイン変更後のボタンの番号
bool	ButtonON[16];          //Joy.Buttonsの代替

#define				LOCAL_RANKING_SIZE		10

NetRankingData		ScoreRanking_local[MAXSTAGE][LOCAL_RANKING_SIZE];
NetRankingData		TimeRanking_local[MAXSTAGE][LOCAL_RANKING_SIZE];
NetRankingData		ScoreRanking_Seq_local[MAXSTAGE_SEQ][LOCAL_RANKING_SIZE];
NetRankingData		TimeRanking_Seq_local[MAXSTAGE_SEQ][LOCAL_RANKING_SIZE];
NetRankingData_Split	ScoreRanking_Split_local[MAXSTAGE_SEQ][LOCAL_RANKING_SIZE];
NetRankingData_Split	TimeRanking_Split_local[MAXSTAGE_SEQ][LOCAL_RANKING_SIZE];

s32		BoostLV;              //ブーストレベル

//コンフィグ情報
s8		BoostMode;            //ブーストモード(0:オルタネイト　1:モーメンタリ（押している間だけ）)
bool	CloudON;              //雲を表示するか
bool	FilterON;             //フィルタON/OFF
bool	ScreenMode;           //スクリーンモード(T:フルスクリーン　F:ウィンドウ)
s32		BGMNumber;            //BGM番号
bool	ShieldInfinite,MShipInfinite,CollisionON,TimeInfinite,SlowON;
s32		EffLevel,TexCol,FrameRate;
s32		SEVol,bgmVol;
bool	CreateBGM;            //BGM再生クラスを生成するか
bool	CreateSE;            //SE再生クラスを生成するか
s32		ScreenResolution;     //画面解像度
bool	ExtraEffects;         //陽炎などのエフェクト
bool	Blooming;
bool	LuminanceMap;
bool	ConnectNet;

//ログのON/OFF
bool	KPUTLOG;


//スプライト描画順序制御用変数
s32		KageExpZ;
s32		BeamZ;
s32		SmokeZ;
s32		ExpZ;
s32		ClExpZ;
s32		TamaZ;
s32		SplashZ;

//デバッグ用情報
vector<DebugData>	DData;

//デバッグログ格納配列
vector<string>		DebugLogArray;
string	LastMovedOBJ;

s32		HPMax;          //自機の最大HP     7000
s32		MSHPMax;       //母艦の最大HP

glm::vec2 Rot2D( glm::vec2& _vec, s32 _r );

void PosRoll( f32& _x, f32& _y, f32 _cx, f32 _cy, s32 _R );
string InttoStr( s32 _val );
string TimeToStr( s32 _time );
bool Equal( s32 _int, s32 _num );
void InitSprZ();
string ReadSegment( string _s, s32 _n );
s32 GetDiffR( s32 _r1, s32 _r2 );
void PutDebugLog( string _msg );
void WriteDebugLog();
void InitDebugLog();
bool CheckDistance( s32 X1, s32 Y1, s32 X2, s32 Y2, s32 dist );
s32 GetRfromArcTan( f32 _x, f32 _y, f32 _dx, f32 _dy );
glm::vec2 GetReflectionVector( glm::vec2& _vec, THitArea* _hitarea );
string GetMacAddress();
string TotalTimeToStr( s32 _time );
s32	Sign(s32 val);
u32 random(u32 _max);

inline DWORD F2DW( FLOAT f ) { return *((DWORD*)&f); }

void GetDate( s32& _year, s32& _mon, s32& _day );
s32 BoolToInt(bool _val);
bool IntToBool(s32 _val);
glm::vec2 Random2D(f32 k);


