#pragma once

using namespace std;

#include "types.h"
#include "TOBJ.h"
#include "Hero.h"
#include <vector>

class TVaporControl;
class TJikiHitArea;


//自機
class TJiki2 : public TJikiBase {
protected	:
    bool AcceptMoveBody;       //移動時のアニメーションを許可するか
    TOBJ* BladeTrace;          //剣の軌跡
    bool NextSlash;          //連続攻撃を行うか
    bool ForceBladeOff;    //強制的に鞘に収める
    s32 CycloBladeR;      //旋風剣発生時の腕の角度
	s32 NowSpr;           //現在のスプライト番号（デバッグ用）
    void OnSlash();        //敵を切ったときのエフェクト処理
    void Input();                                 //キー入力
    void Nokezori();
    void Flash(bool _state);
    void PartsMove();
    void SlashControl();
    void PartsRotate(s32 _sprnum, s32 _dist, s32 _pn, s32 _speed);
    void SavePose();
    void SplashControl();
public:
    void OnDamaged(TOBJ* target);
    void Die();
    TJiki2(TOBJList* owner, s32 _x, s32 _y);
    ~TJiki2();
    void Move();
    virtual void BoostSoon(bool _boost);    //いきなりブースト状態にする(デモ用)
    void ResetBeforeStart();
};

