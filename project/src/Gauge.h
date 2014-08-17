#pragma once

using namespace std;

#include "types.h"
#include "TOBJ.h"
#include <vector>

class TMoji_counter;
class TCursorList;
class TSprSentence;
class TGlassCursor;

//自機ゲージ
class THPGauge : public TOBJ {
  private:

  public:
    THPGauge(TOBJList* owner, s32 _x, s32 _y, s32 _now, s32 _max);
    void Move(); 
};

//母艦ゲージ
class TMSGauge : public TOBJ {
  private:

  public:
    TMSGauge(TOBJList* owner, s32 _x, s32 _y, s32 _now, s32 _max);
    void Move(); 
};

//敵ゲージ
class TEnemyGauge : public TOBJ {
  private:

  public:
    TEnemyGauge(TOBJList* owner, s32 _x, s32 _y, s32 _now, s32 _max);
    void Move(); 
};

//ボスゲージ
class TBossGauge : public TOBJ {
  private:

  public:
    TBossGauge(TOBJList* owner, s32 _x, s32 _y, s32 _now, s32 _max);
    void Move(); 
};

//敵接近ゲージ
class TWarningGauge : public TOBJ {
  private:
    s32 TotalFlight,Max;
  public:
    TWarningGauge(TOBJList* owner, s32 _x, s32 _y, s32 _distance, s32 _pat);
    void Move(); 
};

//ミサイルゲージ
class TMiSGauge : public TOBJ {
  private:

  public:
    TMiSGauge(TOBJList* owner, s32 _x, s32 _y, s32 _now);
    void Move(); 
};

//音量ゲージ
class TSoundGauge : public TOBJ {
  private:

  public:
    TSoundGauge(TOBJList* owner, s32 _x, s32 _y, s32 _now);
    void Move(); 
};

//自機ゲージリメイク
class THPGauge2 : public TOBJ {
  private:
    s32 Pattern;
    s32 TotalShift;
  public:
    THPGauge2(TOBJList* owner, s32 _x, s32 _y, s32 _pat);
    void Move(); 
};

//母艦ゲージリメイク
class TMSGauge2 : public TOBJ {
  private:
    s32 Pattern;
    s32 TotalShift;
  public:
    TMSGauge2(TOBJList* owner, s32 _x, s32 _y, s32 _pat);
    void Move(); 
};

//ミサイルゲージリメイク
class TMiSGauge2 : public TOBJ {
  private:
    s32 Pattern;
    s32 TotalShift;
  public:
    TMiSGauge2(TOBJList* owner, s32 _x, s32 _y, s32 _pat);
    void Move(); 
};

//ミサイルゲージ素片
class TMiSGaugeParts : public TOBJ {
  private:
    s32 order;
    s32 pattern;
    s32 TotalShift;
  public:
    TMiSGaugeParts(TOBJList* owner, s32 _x, s32 _y, s32 _num, s32 _pat);
    void Move(); 
};

//敵ゲージリメイク
class TEnemyGauge2 : public TOBJ {
  private:
    TOBJ* Parent;    //このゲージの所有者
    s32 OffsetX,OffsetY;
  public:
    TEnemyGauge2(TOBJList* owner, TOBJ* _object, s32 _ox, s32 _oy);
    void Move(); 
    void Show();
    void Hide();
    s32 OnDraw(); 
};

//ボスゲージリメイク
class TBossGauge2 : public TOBJ {
  private:
    TOBJ* Parent;    //このゲージの所有者
    s32 OffsetX,OffsetY;
  public:
    TBossGauge2(TOBJList* owner, TOBJ* _object, s32 _ox, s32 _oy);
    void Move(); 
    void Show();
    void Hide();
};

//残り時間
class TLimitTime : public TOBJ {
  private:
    TMoji_counter* moji[6];
  public:
    TLimitTime(TOBJList* owner, s32 _x, s32 _y);
    void Move(); 
    void Die(); 
};

//文字の背景
class TMojiBack : public TOBJ {
  private:
  public:
    TMojiBack(TOBJList* owner, s32 _x, s32 _y, s32 _long);
    void Move(); 
};

//FPS表示
class TFPSIndicator : public TOBJ {
  private:
    TMoji_counter* moji[3];
  public:
    TFPSIndicator(TOBJList* owner, s32 _x, s32 _y);
    void Move(); 
    void Die(); 
};

//敵接近ゲージリメイク
class TWarningGauge2 : public TOBJ {
  private:
    s32 TotalFlight,Max;
    s32 SignalCount;
  public:
    TWarningGauge2(TOBJList* owner, s32 _x, s32 _y, s32 _distance, s32 _pat);
    void Move(); 
};

//敵接近ゲージ素片
class TWarningGaugePart : public TOBJ {
  private:
    s32 Speed;
    s32 Alpha;
  public:
    TWarningGaugePart(TOBJList* owner, s32 _x, s32 _y, s32 _pat);
    void Move(); 
};

//敵接近ゲージ(サークル)
class TWarningGaugeCircle : public TOBJ {
  private:

  public:
    TWarningGaugeCircle(TOBJList* owner, s32 _x, s32 _y);
    void Move(); 
};

//ネット接続中ゲージ
class TConnectingGauge : public TOBJ {
  private:
    TCursorList* CursorList;
    s32 Alpha;
  public:
    TConnectingGauge(TOBJList* owner);
    ~TConnectingGauge();
    void Move(); 
    void Die(); 
};

//チュートリアル用キーボード
class TKeyBoard : public TOBJ {
  private:
    TCursorList* CursorList;
    vector<TGlassCursor*>  GlareCursor;
    void MakeButton(const string& _moji, s32 _x, s32 _y);
  public:
    TKeyBoard(TOBJList* owner);
    ~TKeyBoard();
    void Move(); 
    void Imput(s32 _x, s32 _y, bool _b1, bool _b2, bool _b3);
};

//注目カーソル素片
class TAttractionCursorPart : public TOBJ {
  private:

  public:
    TAttractionCursorPart(TOBJList* owner, s32 _x, s32 _y, s32 _long);
    void Move(); 
    void SetColor(s32 _r, s32 _g, s32 _b);
};

//注目カーソル
class TAttractionCursor : public TOBJ {
  private:
    s32 Long;
    s32 Life;
    vector<TAttractionCursorPart*>	CursorPart;
  public:
    TAttractionCursor(TOBJList* owner, s32 _x, s32 _y, s32 _long, s32 _life);
    ~TAttractionCursor();
    void Move(); 
};

//BGMタイトル
class TBGMTitle : public TOBJ {
  private:
    TCursorList* CursorList;
  public:
    TBGMTitle(TOBJList* owner, const string& _title);
    ~TBGMTitle();
    void Move(); 
};

//母艦（チュートリアル用）
class TMothership_Tutorial : public TOBJ {
  private:
  public:
    TMothership_Tutorial(TOBJList* owner, s32 _x, s32 _y);
    void Move(); 
};

//警報
class TDangerAlarm : public TOBJ {
  private:
    bool Visible;
  public:
    TDangerAlarm(TOBJList* owner, s32 _x);
    void Move(); 
    void Place(s32 _x);
    void Show(bool _visible);
};

//面セレクトカーソル
class TStSelCursor : public TOBJ {
  private:
    s32 CurrentNum;
    vector<TSprSentence*> Sentence;           //stage title
    s32 Alpha;
    s32 MaxStage,MaxIcon;
    s32 StarIconID[2];
    void MakeCursor(const string& _caption, s32 _x, s32 _y);
    void SetStageTitle(s32 _num);
  public:
    TStSelCursor(TOBJList* owner);
    void Move(); 
    void Up();
    void Down();
    void Die(); 
    void ShowStar(bool _tf);
};

//ローディング画面
class TLoadingScreen : public TOBJ {
  private:
    TCursorList* CursorList;

  public:
    TLoadingScreen(TOBJList* owner);
    ~TLoadingScreen();
    void Move(); 
    void Die(); 
};

//タイムリミットカウントダウン
class TCountDown : public TOBJ {
  private:
    s32 ActFlg;
    s32 Pattern;
  public:
    TCountDown(TOBJList* owner, s32 _x, s32 _y, s32 _pat);
    void Move(); 
};

//母艦ダメージ
class TMotherShipDamaged : public TOBJ {
  private:
    s32 Pattern;
  public:
    TMotherShipDamaged(TOBJList* owner, s32 _x, s32 _y);
    void Move(); 
};

//自機当たり判定
class TJikiHitArea : public TOBJ {
  private:
    s32 ActCount;
    TOBJ* Parent;
    bool Visible;
  public:
    TJikiHitArea(TOBJList* owner, TOBJ* _parent);
    void Move(); 
    void Show();
    void Place(f32 _x, f32 _y);
};

//プラクティスモード用タイトル
class TPracticeTitle : public TOBJ {
  private:
    TCursorList* CursorList;
  public:
    TPracticeTitle(TOBJList* owner);
    ~TPracticeTitle();
    void Move(); 
};
