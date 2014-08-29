#pragma once

using namespace std;

#include "types.h"
#include "TOBJ.h"
#include <vector>

class TEnemyGauge2;
class TVaporControl;
class TZBeam2;

// 雑魚2-1
class TZako2_01 : public TOBJ {
  protected:
    bool Buzzed;//Buzzったかどうか
    s32 LastBuzz;//最後にBuzzった時間
    s32 Rdeg;    //角度
    s32 Speed;                //自機スピード
    s32 Pattern; //行動パターン
    s32 GaugeTime;
    s32 TotalFlight;//飛行距離
    u8 OriginColor;
    s32 LastFlash;
    TEnemyGauge2* HPGauge;
    s32 DestX,DestY;
    void Action();
    void MakeShot(s32 _pat);
    void MakeMissile();
  public:
    TZako2_01(TOBJList* owner, s32 _x, s32 _y, s32 _destx, s32 _desty, s32 _R, s32 _S, s32 _pat, bool _red );
    void Move(); 
    void OnDamaged(TOBJ* target);
    void Die();
    void OnBroken();
};

// 雑魚2-2（バラ捲き）
class TZako2_02 : public TOBJ {
  protected:
    bool Buzzed;//Buzzったかどうか
    s32 LastBuzz;//最後にBuzzった時間
    s32 Rdeg;    //角度
    s32 Speed;                //自機スピード
    s32 Pattern; //行動パターン
    TEnemyGauge2* HPGauge;
    s32 GaugeTime;
    s32 TotalFlight;//飛行距離
    u8 OriginColor;
    s32 LastFlash;
    s32 ActFlg[100];     //行動フラグ
    s32 ActCount[100];   //行動カウンタ
    s32 DestX,DestY;
    f32 BSpeed;
    s32 RetreatTime;
    void Action();
    void Accel();
    void MakeShot();
  public:
    TZako2_02(TOBJList* owner, s32 _x, s32 _y, s32 dx, s32 dy, s32 _R, s32 _S, s32 _pat, bool _red );
    void Move(); 
    void OnDamaged(TOBJ* target);
    void Die();
    void OnBroken();
  
    //バラ撒きくん部品番号
	enum{
		Z2_2Body       = 0,
		Z2_2Rotor      = 1,
		Z2_2Wing1      = 2,
		Z2_2Wing2      = 3,
		Z2_2Wing3      = 4,
	};

	enum{ KACTION_S2Z2_ACT1  = 0 };
};

// 雑魚2-3（回転バー）
class TZako2_03 : public TOBJ {
  protected:
    bool Buzzed;//Buzzったかどうか
    s32 LastBuzz;//最後にBuzzった時間
    s32 Rdeg;    //角度
    s32 Pattern; //行動パターン
    s32 GaugeTime;
    s32 TotalFlight;//飛行距離
    u8 OriginColor;
    s32 LastFlash;
    void MakeBar(s32 _angle);
  public:
    s32 Speed;                //自機スピード
    TZako2_03(TOBJList* owner, s32 _x, s32 _y, s32 _R, s32 _S, s32 _pat, bool _red );
    void Move(); 
    void OnDamaged(TOBJ* target);
    void Die();
};

// 雑魚2-4（スパイダー）
class TZako2_04 : public TOBJ {
  protected:
    bool Buzzed;//Buzzったかどうか
    s32 LastBuzz;//最後にBuzzった時間
    s32 Rdeg;    //角度
    s32 Speed;                //自機スピード
    s32 Pattern; //行動パターン
    s32 GaugeTime;
    s32 TotalFlight;//飛行距離
    u8 OriginColor;
    s32 LastFlash;
    TEnemyGauge2* HPGauge;
    s32 EnemyDeg; //敵と自分の相対角度
    s32 CurrentMotion;
    f32 BSpeed;
	s32 ActFlg[100];     //行動フラグ
	s32 ActCount[100];   //行動カウンタ
    f32 IniDis;       //目的地までの距離
    s32 DestDeg;     //目的地までの角度
    s32 DestX,DestY;
    s32 HomePosX;
    void MakeShot();
    void Accel();
    void Transfer();
    void Appear();
    void Escape();
    void Action();
    void ChargeMotion();
    void ShotMotion();
  public:
    TZako2_04(TOBJList* owner, s32 _x, s32 _y, s32 _pat, bool _red );
    void Move(); 
    void OnDamaged(TOBJ* target);
    void Die();
    void OnBroken(); 
    
	//行動番号
	enum{
		KACTION_S2Z4_DEFAULT  = 0,
		KACTION_S2Z4_TRANSFER = 1,
		KACTION_S2Z4_APPEAR = 2,
		KACTION_S2Z4_CHARGE = 3,
		KACTION_S2Z4_SHOT = 4,
	};
};

// 潜水艦
class TSubmarine : public TOBJ {
  protected:

  public:
    TSubmarine(TOBJList* owner, s32 _x, s32 _y);
    void Move(); 
};

// 雑魚2-5（ビーム砲）
class TZako2_05 : public TOBJ {
  protected:
    bool Buzzed;//Buzzったかどうか
    s32 LastBuzz;//最後にBuzzった時間
    s32 Rdeg;    //角度
    s32 Speed;                //自機スピード
    s32 Pattern; //行動パターン
    s32 GaugeTime;
    s32 TotalFlight;//飛行距離
    u8 OriginColor;
    s32 LastFlash;
    TEnemyGauge2* HPGauge;
    f32 BSpeed;
	s32 ActFlg[100];     //行動フラグ
	s32 ActCount[100];   //行動カウンタ
    s32 DestX,DestY;
    void MakeShot();
    void Accel();
    void Action();
    void BeamMotion();
    void MakeMissile();
    void BeamMotion2();
  public:
    TZako2_05(TOBJList* owner, s32 _x, s32 _y, s32 _dx, s32 _dy, s32 _pat, bool _red );
    void Move(); 
    void OnDamaged(TOBJ* target);
    void Die();
    void OnBroken(); 

    //行動番号
	enum{
		KACTION_S2Z5_BEAM  = 0,
		KACTION_S2Z5_ACT1  = 1,
	};
};

// 雑魚2-6（小型機）
class TZako2_06 : public TOBJ {
  protected:
    bool Buzzed;//Buzzったかどうか
    s32 LastBuzz;//最後にBuzzった時間
    s32 Rdeg;    //角度
    s32 Speed;                //自機スピード
    s32 Pattern; //行動パターン
    s32 TotalFlight;//飛行距離
    s32 DiffDistance;
    u8 OriginColor;
    s32 LastFlash;
    f32 BSpeed;
    s32 RSpeed;
    s32 DisappearPos;
	s32 ActFlg[100];     //行動フラグ
	s32 ActCount[100];   //行動カウンタ
    s32 ShotCount;
    void MakeShot();
    void Action();
  public:
    TZako2_06(TOBJList* owner, s32 _x, s32 _y, s32 _r, s32 _s, s32 _pat, bool _red );
    void Move(); 
    void OnDamaged(TOBJ* target);
    void OnBroken(); 

    //行動番号
	enum{
		KACTION_S2Z6_ACT1  = 0
	};
};

// 回転バー素片
class TRevBarBasic : public TOBJ {
  protected:
    s32 Rdeg;    //角度
    f32 Speed;                //自機スピード
    s32 Life;
    TOBJ* Parent;
    s32 PrevR;
    f32 ParentSpeed;
  public:
    TRevBarBasic(TOBJList* owner, TOBJ* _parent, s32 _x, s32 _y, s32 _R, f32 _S, f32 _parentspeed, s32 _L);
    void Move(); 
};

// 雑魚ジェネレータ2-1
class TZ02_1Gene : public TOBJ {
  private:
    s32 Pattern;
    s32 Count;
    s32 TotalFlight;
    s32 OriginX;
  public:
    TZ02_1Gene(TOBJList* owner, s32 _x, s32 _y, s32 _pat);
    void Move(); 
};

// 雑魚ジェネレータ2-5
class TZ02_5Gene : public TOBJ {  
  private:
    s32 Pattern;
    s32 Count;
    s32 TotalFlight;
    s32 CommanderNum;
    s32 diffX;
  public:
    TZ02_5Gene(TOBJList* owner, s32 _x, s32 _y, s32 _pat, s32 _com);
    void Move(); 
};

// 雑魚ジェネレータ2-6
class TZ02_6Gene : public TOBJ {  
  private:
    s32 Pattern;
    s32 Count;
    s32 TotalFlight;
    s32 diffX;
  public:
    TZ02_6Gene(TOBJList* owner, s32 _x, s32 _y, s32 _pat);
    void Move(); 
};

// 雑魚3-1（大型機）
class TZako3_01 : public TOBJ {
  protected:
    bool Buzzed;//Buzzったかどうか
    s32 LastBuzz;//最後にBuzzった時間
    s32 Speed;                //自機スピード
    s32 Pattern; //行動パターン
    s32 TotalFlight;//飛行距離
    u8 OriginColor;
    s32 LastFlash;
    f32 BSpeed;
    TEnemyGauge2* HPGauge;
    s32 GaugeTime;
    s32 KnockBackSpeed;
    s32 DestX,DestY;
	s32 ActFlg[100];     //行動フラグ
	s32 ActCount[100];   //行動カウンタ
    s32 RoopCount;
    void MakeShot(s32 _x, s32 _y, s32 _r);
    void MakeHoming(s32 _x, s32 _y, s32 _r);
    void Action();
    void Accel();
    void KnockBack(s32 _impact);
    void KnockBackAction();
  public:
    TZako3_01(TOBJList* owner, s32 _x, s32 _y, s32 dx, s32 dy, s32 _pat, bool _red );
    void Move(); 
    void OnDamaged(TOBJ* target);
    void OnBroken(); 
    void Die(); 

    //行動番号
    enum{
		KACTION_ACT1  = 0,
		KACTION_KNOCKBACK = 1,
	};
};

// 雑魚2-5-2（ビーム砲_5ボス）
class TZako2_05_2 : public TOBJ {
  protected:
    bool Buzzed;//Buzzったかどうか
    s32 LastBuzz;//最後にBuzzった時間
    s32 Rdeg;    //角度
    s32 Pattern; //行動パターン
    s32 GaugeTime;
    s32 TotalFlight;//飛行距離
    u8 OriginColor;
    s32 LastFlash;
    TEnemyGauge2* HPGauge;
    f32 BSpeed;
    s32 ActFlg;     //行動フラグ
    s32 ActCount;   //行動カウンタ
    s32 DestX,DestY;
    TJoint* Parent;
    s32 RSpeed;
    s32 DistY;
    s32 CurrentDist;
    TZBeam2* Beam;
    void Action();
  public:
    TZako2_05_2(TOBJList* owner, TJoint* _parent, s32 _x, s32 _y, s32 _r, s32 _distance, s32 _Rspeed, s32 _pat, bool _red );
    void Move(); 
    void OnDamaged(TOBJ* target);
    void Die(); 
    void OnBroken();
};

// 雑魚ジェネレータ2-3
class TZ02_3Gene : public TOBJ {
  private:
    s32 Pattern;
    s32 Count;
    s32 TotalFlight;
    s32 diffX;
    s32 Interval;
  public:
    TZ02_3Gene(TOBJList* owner, s32 _pat);
    void Move(); 
};

