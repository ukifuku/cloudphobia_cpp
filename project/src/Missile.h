#pragma once

using namespace std;

#include "types.h"
#include "TOBJ.h"
#include <vector>

class TSmoke;

//ミサイル発射管理クラス
class TMakeMissile : public TOBJ {
  private:
    s32 SetNum;   //何セット目のミサイルか
	s32 ShotInt;  //ショット間隔
    s32 TotalLock;
    s32 MissileNum;
  public:
    TMakeMissile(TOBJList* owner, s32 _setnum, s32 _locknum);
    void Move(); 
};

//ミサイル発射管理クラス2
class TMakeMissile2 : public TOBJ {
  private:
    s32 SetNum;   //何セット目のミサイルか
    s32 ShotInt;  //ショット間隔
    s32 MissileNum;
  public:
    TMakeMissile2(TOBJList* owner, s32 _setnum);
    void Move(); 
};

//煙
class TSmoke : public TOBJ {
  private:
    s32 Speed;
    f32 ZoomRate;
    s32 PersonalR;
    s32 IncR;
    TSmoke* PrevSmoke;    //直前に生成された煙
    bool MotherIsDead;  //自分を生成したミサイル本体が死んだ
    s32 OriginalBGSpeed;  //生成当時の背景スピード
  public:
    s32 Rdeg;
    TSmoke* NextSmoke;   //直後に生成された煙
    TSmoke(TOBJList* owner, s32 sx, s32 sy, s32 _R, TSmoke* _prev);
    void Move(); 
    void TellMomDead();    //ミサイル本体が死んだことをお隣の煙にお知らせ
    void Die(); 
};

//ミサイル本体
class TMissile : public TOBJ {
  private:
    s32 YMove; //   Y方向の動き
    s32 Rdeg;
    s32 SetNum; //何セット目のミサイルか
    s32 PrevR;   //前フレームでの角度
    TSmoke* PrevSmoke;   //前回出した煙オブジェクト
  public:
    TMissile(TOBJList* owner, s32 sx, s32 sy, s32 _setnum, s32 _misnum);
    void Move(); 
    void Die(); 
    void OnDamaged(TOBJ* target); 
};

//煙2
class TSmoke2 : public TOBJ {
  private:
    s32 Rdeg;
    s32 Speed;
    TOBJ* PrevObj;
    s32 MissileID;
    f32 ZoomRate;
  public:
    TSmoke2(TOBJList* owner, s32 sx, s32 sy, s32 _R, TOBJ* _PrevObj, s32 _misID);
    void Move(); 
};

//バックファイア
class TMisBFire : public TOBJ {
  private:
    s32 Rdeg;
  public:
    TMisBFire(TOBJList* owner, s32 sx, s32 sy, s32 _R);
    void Move(); 
};
