#pragma once

using namespace std;

#include "types.h"
#include "TOBJ.h"
#include <vector>

class TEnemyGauge2;
class TVaporControl;

// ジェネレータ
class TTransGene : public TOBJ {
private:
	s32 Count;
public:
	TTransGene(TOBJList* owner);
	void Move(); 
};

//変形くん
class TTransformer : public TOBJ {
private:
	bool Buzzed;//Buzzったかどうか
	s32 LastBuzz;//最後にBuzzった時間
	s32 Pattern; //行動パターン
	TEnemyGauge2* HPGauge;
	s32 GaugeTime;
	s32 TotalFlight;//飛行距離
	s32 ActFlg;     //行動フラグ
	s32 ActCount;   //行動カウンタ
	s32 LastFlash;
	bool BeamON;     //ビーム使用中
	s32 BeamCount;  //ビーム発射動作用カウンタ
	s32 GunR;   //ビーム砲角度
	s32 EnemyDeg; //敵と自分の相対角度
	bool Boost;    //ブースト
	s32 BoostCount;
	f32 BSpeed;
	s32 NowSpr;
	bool TransState; //T:戦闘機　F:人型
	bool TransForm;
	s32 TransCount;//変形カウンタ
	s32 XCycle,YCycle;
	f32 XSpeed,YSpeed;
	s32 Xdist,Ydist; //目的地座標
	f32 IniDis;       //目的地までの距離(指定直後)
	f32 CurDis;       //目的地までの現在の距離
	s32 DestDeg;     //目的地までの角度
	s32 TransTiming; //変形するまでの時間
	s32 PrevX,PrevY;
	s32 Retreat; //退却する距離
	s32 TimCount;    //タイミング取りカウンタ
	f32 AccelElement;
	s32 DecelElement;
	TPoint HPosList[4];
	TPoint PPosList[4]; //移動先リスト
	TVaporControl* Vapor;

	//変形くん部品番号
	enum{
	THip     = 0,
	THead    = 1,
	TLShoulder= 2,
	TLArm     = 3,
	TBody    = 4,
	TRLeg1   = 5,
	TRLeg2   = 6,
	TLLeg1   = 7,
	TLLeg2   = 8,
	TNose    = 9,
	TCWing   =10,
	TRWing   =11,
	TLWing   =12,
	TGun1    =13,
	TGun2    =14,
	TRShoulder=15,
	TRArm     =16,


	THipR     = 17,
	THeadR    = 18,
	TLShoulderR= 19,
	TLArmR     = 20,
	TBodyR    = 21,
	TRLeg1R   = 22,
	TRLeg2R   = 23,
	TLLeg1R   = 24,
	TLLeg2R   = 25,
	TNoseR    = 26,
	TCWingR   =27,
	TLWingR   =28,
	TGun1R    =29,
	TGun2R    =30,
	};

public:
	TTransformer(TOBJList* owner, s32 _x, s32 _y, s32 _r, s32 _pat, bool _red );
	void Move(); 
	void OnDamaged(TOBJ* target);
	void Die();
	void BeamShot();
	void Control();
	void Trans(bool _state);
	void PartsRotate(s32 _sprnum, s32 _dist, s32 _pn, s32 _speed);
	void BodyRotate(s32 _dist, s32 _speed);
	void MonoRotate(s32 _Sprnum, s32 _dist, s32 _speed );
	void SlideBody(s32 _pat);
	void Accel();
	void OnBroken(); 
};

//ビーム素片
class TTransBeamPart : public TOBJ {
private:
	s32 Rdeg;
	s32 Speed;
	s32 Life;
public:
	TTransBeamPart(TOBJList* owner, s32 sx, s32 sy, s32 R, s32 S, s32 L, f32 _zoom);
	void Move(); 
};

//ビーム本体
class TTransBeamCore : public TOBJ {
private:
	s32 Rdeg;
	bool Buzzed;//Buzzったかどうか
	s32 LastBuzz;//最後にBuzzった時間
	s32 Pattern;
public:
	TTransBeamCore(TOBJList* owner, s32 sx, s32 sy, s32 R, s32 _pat);
	void Move(); 
	void OnDamaged(TOBJ* target);
};

//噴射炎
class TTransBFire : public TOBJ {
public:
	TTransBFire(TOBJList* owner, s32 sx, s32 sy);
	void Move(); 
};

