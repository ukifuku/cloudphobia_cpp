#pragma once

using namespace std;

#include "types.h"
#include "TOBJ.h"
#include <vector>

class TVaporControl;
class TJikiHitArea;

class TJikiBase: public TOBJ {
protected:
	s32 PrevShot;
    s32 ShootFlg;             //武器フラグ(-1:未使用 0:ノーマルショット)
	bool MisOK;                //ミサイル使用可
	s32 BlinkTime,LastBlink;  //点滅時間、最後に点滅した時間
	s32 LastMis;              //最後にミサイルを撃った時間
	bool MissileMuteki;        //ミサイル無敵中
	s32 BoostTime;            //ブースト時間
	s32 PrevX,PrevY;          //1フレーム前の座標
	s32 NokeTime;             //のけぞり動作開始時間
	s32 CrushR;               //ぶつかった相手との角度
	s32 BoostR;               //加速する方向
	s32 ActFlg[100];     //行動フラグ
	s32 ActCount[100];   //行動カウンタ
	s32 ShowSpr;

	s32 SlideX,SlideY;
	bool AcceptMoveBody;       //移動時のアニメーションを許可するか
	TVaporControl* Vapor;
	virtual void MakeVapor(s32 _num, s32 _sprite, s32 _x, s32 _y);
public:
	s32 Speed;                //自機スピード
	f32 ZoomRate;            //拡大率
	TJikiHitArea* HitArea;
	TJikiBase(TOBJList* owner, s32 _x, s32 _y);
	~TJikiBase();
	void Move(); 
	virtual void Input();                                 //キー入力
	void OnDamaged(TOBJ* target);
	void Die();
	virtual void Slide(f32 _x, f32 _y);                   //移動時の処理
	virtual void BodyControl(s32 Bo, s32 L2, s32 L3, s32 R2, s32 R3);   //多間接制御
	virtual void BFControl(s32 _R, f32 _zoom);   //バックパック制御
	virtual void HeroJet();  
	virtual void Nokezori();
	virtual void MonoRotate(s32 _Sprnum, s32 _dist, s32 _speed);
	virtual void StartEvent();                      //出現時イベント
	virtual void Flash(bool _state);
	virtual void BlinkControl();    //点滅
	virtual void BoostSoon(bool _boost);    //いきなりブースト状態にする(デモ用)
	virtual void BodyControlSoon(s32 Bo, s32 L2, s32 L3, s32 R2, s32 R3);   //多間接制御（アニメーション無し）
	virtual void ResetBeforeStart();
	virtual void BoostByOther(bool _boost);
	virtual void Hide();
};

//自機
class TJiki : public TJikiBase {
protected:
public:
    TJiki(TOBJList* owner, s32 _x, s32 _y);
    ~TJiki();
    void Move(); 
    void Input();                                 //キー入力
    void MakeShot();                              //弾を撃つ
    void OnDamaged(TOBJ* target);
    void Nokezori();
    void Flash(bool _state);
    void BoostSoon(bool _boost);    //いきなりブースト状態にする(デモ用)
    void DoStartEvent();
    void ResetBeforeStart();
    void Hide();
};

//弾
class TShot : public TOBJ{
public:
    TShot(TOBJList* owner, s32 sx, s32 sy, s32 R, f32 _zoom);
    void Move(); 
    void OnDamaged(TOBJ* target); 
};

//噴射炎
class TBFire : public TOBJ{
private:
    s32 Rdeg;
    s32 DisX,DisY;//自機中心からの距離
    f32 ZoomRate;
    TOBJ* OwnerOBJ;
public:
    TBFire(TOBJList* owner, s32 sx, s32 sy, s32 _R, s32 _LV, f32 _zoom, TOBJ* _obj);
    void Move(); 
};

//部品番号
static const s32		HBuzz = 0;
static const s32       HBody=1;
static const s32       HRArm=2;
static const s32       HLArm=3;
static const s32       HRHand=4;
static const s32       HRLeg1=5;
static const s32       HRLeg2=6;
static const s32       HRLeg3=7;
static const s32       HLLeg1=8;
static const s32       HLLeg2=9;
static const s32       HLLeg3=10;
static const s32       HRCover=11;
static const s32       HLCover=12;
static const s32       HLHand=13;
static const s32       HRWing=14;
static const s32       HLWing=15;
static const s32       HSheath=16;   //鞘
static const s32       HBlade=17;    //刀
static const s32       HFlag=16;    //フラッグ
   

//行動番号
static const s32       HAStart = 0;
static const s32       HANokezori = 1;
static const s32       HASlash = 2;

static const s32     BodyTable[] = {256,128,128,3840,3840,3968,3968,256,0};
static const s32     LLegTable2[] = {400,3052,3052,3188,3188,3492,3492,200,3396};
static const s32     LLegTable3[] = {88,4060,4060,3132,3132,3732,3732,88,100};
static const s32     RLegTable2[] = {500,3472,3472,3368,3368,3728,3728,200,3992};
static const s32     RLegTable3[] = {88,4080,4080,3272,3272,3576,3576,88,3992};
