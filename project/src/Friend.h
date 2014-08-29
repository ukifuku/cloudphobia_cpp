#pragma once

using namespace std;

#include "types.h"
#include "TOBJ.h"
#include "Hero.h"

//寮機
class TFriend : public TJiki {
  protected:
    s32 ActType;
    s32 ActionID;
    s32 BoostLV_friend;
    TJoint* OwnerSprite;     //他のスプライトの多関節に組み込む場合の、親スプライト
  public:
    TFriend(TOBJList* owner, s32 _x, s32 _y, f32 _zoom, s32 _type);
    void Move(); 
    void Die();
    void MakeShot(); 
    void HeroJet(); 
    void Slide(f32 _x, f32 _y); 
    void BoostSoon(bool _boost); 
    void Disappear();
    void SetOwnerSprite(TJoint* _spr);
    void DoAction(s32 _num);
  };

//ビームフラッグ
class TBeamFlag : public TOBJ {
  protected:
    TJoint* OwnerSprite;     //他のスプライトの多関節に組み込む場合の、親スプライト
  public:
    TBeamFlag(TOBJList* owner, s32 _x, s32 _y, f32 _zoom, TJoint* _spr );
    void Move(); 
    void Die();
  };

//やられザコ
class TYarareZako : public TOBJ {
  protected:
    s32 ActFlg;
    s32 ActCount;
    f32 ZoomRate;
  public:
    TYarareZako(TOBJList* owner, s32 _x, s32 _y, f32 _zoom );
    void Move(); 
    void Die();
  };

//寮機2
class TFriend2 : public TOBJ {
  protected:
    s32 ActType;
    s32 ActionID;
    s32 ActFlg[100];
    s32 ActCount[100];
    s32 BoostLV_friend;
    f32 ZoomRate;

    TYarareZako* zako;
  public:
    TFriend2(TOBJList* owner, s32 _x, s32 _y, f32 _zoom, s32 _type);
    void Move(); 
    void Die();
    void Slide(f32 _x, f32 _y);
    void Disappear();
    void DoAction(s32 _num);
    void BladeMotion();

  };

  //行動番号
#define		KACTION_FRIEND_BOOST	1
#define		KACTION_FRIEND_FLAG		2
//行動番号
#define		KACTION_BLADE			0