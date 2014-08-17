#pragma once

using namespace std;

#include "types.h"
#include "TOBJ.h"
#include <vector>

class TVaporControl;
class TJikiHitArea;


//自機
class TJiki2 : public TOBJ {
protected	:
	//部品番号
	static const s32 HConstForeground = 17;

    bool PrevShot;             //前にショットを撃った時間
    bool MisOK;                //ミサイル使用可
    bool BlinkTime,LastBlink;  //点滅時間、最後に点滅した時間
    bool LastMis;              //最後にミサイルを撃った時間
    bool MissileMuteki;        //ミサイル無敵中
    bool BoostTime;            //ブースト時間
    bool PrevX,PrevY;          //1フレーム前の座標
    bool NokeTime;             //のけぞり動作開始時間
    bool CrushR;               //ぶつかった相手との角度
    bool BoostR;               //加速する方向
    s32 ActFlg[100];     //行動フラグ
    s32 ActCount[100];   //行動カウンタ
    bool ShowSpr;
    f32 zoomrate;            //拡大率
    bool SlideX,SlideY;
    bool NowSpr;           //現在のスプライト番号（デバッグ用）
    bool AcceptMoveBody;       //移動時のアニメーションを許可するか
    TOBJ* BladeTrace;          //剣の軌跡
    bool NextSlash;          //連続攻撃を行うか
    bool ForceBladeOff;    //強制的に鞘に収める
    TVaporControl* Vapor;
    bool CycloBladeR;      //旋風剣発生時の腕の角度
    void OnSlash();        //敵を切ったときのエフェクト処理
    void Input();                                 //キー入力
    virtual void Slide(f32 _x, f32 _y);           //移動時の処理
    void BodyControl(s32 Bo, s32 L2, s32 L3, s32 R2, s32 R3);   //多間接制御
    void BFControl(s32 _R, f32 _zoom);   //バックパック制御
    virtual void HeroJet();  
    void Nokezori();
    void MonoRotate(s32 _Sprnum, s32 _dist, s32 _speed);
    void StartEvent();                      //出現時イベント
    void Flash(bool _state);
    void BlinkControl();    //点滅
    void PartsMove();
    void SlashControl();
    void PartsRotate(s32 _sprnum, s32 _dist, s32 _pn, s32 _speed);
    void SavePose();
    void SplashControl();
    void MakeVapor(s32 _num, s32 _sprite, s32 _x, s32 _y);
    void BodyControlSoon(s32 Bo, s32 L2, s32 L3, s32 R2, s32 R3);   //多間接制御（アニメーション無し）
public:
    bool Speed;                //自機スピード
    bool ShootFlg;             //武器フラグ(-1:未使用 0：剣)
    TJikiHitArea* HitArea;
    void OnDamaged(TOBJ* target);
    void Die();
    TJiki2(TOBJList* owner, s32 _x, s32 _y);
    ~TJiki2();
    void Move();
    virtual void BoostSoon(bool _boost);    //いきなりブースト状態にする(デモ用)
    void ResetBeforeStart();
    void BoostByOther(bool _boost);

};

//衝撃波
class TSonicboom : public TOBJ {
public:
    TSonicboom(TOBJList* owner, s32 _x, s32 _y);
    void Move(); 
    void OnDamaged(TOBJ* target); 
};

