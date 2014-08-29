#pragma once

using namespace std;

#include "types.h"
#include "TOBJ.h"
#include <vector>

class TEnemyGauge2;
class TSmoke;
class THomingTrace;
class TEnemyLockMarker;
class TPolyLine2D;
class TCollisionSpr;

// 雑魚ジェネレータ1
class TZ01Gene : public TOBJ {
private:
	s32 Pattern;
	s32 Count;
	s32 TotalFlight;
	s32 YDist;
	s32 RanCount;
	s32 ReplayCount;
public:
	TZ01Gene(TOBJList* owner, s32 _x, s32 _y, s32 _pat, bool _red);
	void Move(); 
};


// 雑魚1
class TZako01 : public TOBJ {
protected:
	bool Buzzed;//Buzzったかどうか
	s32 LastBuzz;//最後にBuzzった時間
	s32 Rdeg;    //角度
	s32 Speed;                //自機スピード
	s32 Pattern; //行動パターン
	bool GaugeOK;
	s32 GaugeTime;
	s32 TotalFlight;//飛行距離
public:
	TZako01(TOBJList* owner, s32 _x, s32 _y, s32 _R, s32 _S, s32 _pat, bool _red );
	void Move(); 
	void MakeShot();                              //弾を撃つ
	void OnDamaged(TOBJ* target);
	void Die();
	void OnBroken();
};

//弾
class TZ01Shot : public TOBJ {
private:
	bool Buzzed;//Buzzったかどうか
	s32 LastBuzz;//最後にBuzzった時間
	bool Anim;//アニメーション用変数
	s32 Rdeg;
public:
	TZ01Shot(TOBJList* owner, s32 sx, s32 sy);
	void Move(); 
	void OnDamaged(TOBJ* target); 
};


//弾2(スピードと角度を調節可能)
class TZ01Shot2 : public TOBJ {
private:
	bool Buzzed;//Buzzったかどうか
	s32 LastBuzz;//最後にBuzzった時間
	bool Anim;//アニメーション用変数
	s32 Rdeg;
	s32 Speed;
	f32 IniXSpeed;//スクロールスピードの影響
public:
	TZ01Shot2(TOBJList* owner, s32 sx, s32 sy, s32 _R, s32 _S, f32 _IniXS);
	void Move(); 
	void OnDamaged(TOBJ* target); 
};

//噴射炎
class TZ01BFire : public TOBJ {
private:
	s32 Rdeg;
public:
	TZ01BFire(TOBJList* owner, s32 sx, s32 sy, s32 _R);
	void Move(); 
};

// 雑魚ジェネレータ2
class TZ02Gene : public TOBJ {
private:
	s32 Pattern;
	s32 Count;
	s32 TotalFlight;
public:
	TZ02Gene(TOBJList* owner, s32 _x, s32 _y, s32 _pat);
	void Move(); 
};

//雑魚2（突撃くん）
class TZako02 : public TOBJ {
private:
	bool Buzzed;//Buzzったかどうか
	s32 LastBuzz;//最後にBuzzった時間
	s32 Rdeg;    //角度
	s32 Speed;                //自機スピード
	s32 Pattern; //行動パターン
	TEnemyGauge2* HPGauge;
	s32 GaugeTime;
	s32 TotalFlight;//飛行距離
	s32 ActFlg;     //行動フラグ
	s32 ActCount;   //行動カウンタ
public:
	TZako02(TOBJList* owner, s32 _x, s32 _y, s32 _R, s32 _S, s32 _pat, bool _red);
	void Move(); 
	void OnDamaged(TOBJ* target);
	void Die();
	void MakeShot();
	void OnBroken();
};


// 雑魚ジェネレータ3
class TZ03Gene : public TOBJ {
private:
	s32 Pattern;
	s32 Count;
	s32 TotalFlight;
public:
	TZ03Gene(TOBJList* owner, s32 _x, s32 _y, s32 _pat);
	void Move(); 
};

//雑魚3（トビウオ）
class TZako03 : public TOBJ {
private:
	bool Buzzed;//Buzzったかどうか
	s32 LastBuzz;//最後にBuzzった時間
	s32 Rdeg;    //角度
	f32 Speed;                //自機スピード
	s32 Pattern; //行動パターン
	bool GaugeOK;
	s32 GaugeTime;
	s32 TotalFlight;//飛行距離
	s32 PrevX,PrevY; //前フレームでの位置
	s32 ShotNum;    //ショット回数
public:
	TZako03(TOBJList* owner, s32 _x, s32 _y, s32 _R, s32 _S, s32 _pat, bool _red );
	void Move(); 
	void MakeShot();
	void OnDamaged(TOBJ* target);
	void Die();
	void OnBroken();
};

// 雑魚ジェネレータ4
class TZ04Gene : public TOBJ {
private:
	s32 Pattern;
	s32 Count;
	s32 TotalFlight;
	s32 Life;
	s32 PosCount;
public:
	TZ04Gene(TOBJList* owner, s32 _pat, s32 _life);
	void Move(); 
};

//雑魚4出現予告
class TZ04Warn : public TOBJ {
private:
	s32 TotalFlight;
	f32 ZoomRate;
public:
	TZ04Warn(TOBJList* owner, s32 _x, s32 _y, bool _red);
	void Move(); 
};

//雑魚4（小型機）
class TZako04 : public TOBJ {
private:
	bool Buzzed;//Buzzったかどうか
	s32 LastBuzz;//最後にBuzzった時間
	s32 Rdeg;    //角度
	s32 Speed;                //自機スピード
	s32 Pattern; //行動パターン
	bool GaugeOK;
	s32 GaugeTime;
	s32 TotalFlight;//飛行距離
	s32 ActFlg;     //行動フラグ
	s32 ActCount;   //行動カウンタ
public:
	TZako04(TOBJList* owner, s32 _x, s32 _y, s32 _R, s32 _S, s32 _pat, bool _red );
	void Move(); 
	void OnDamaged(TOBJ* target);
	void Die();
	void OnBroken();
};

//雑魚5（大型機）
class TZako05 : public TOBJ {
private:
	bool Buzzed;//Buzzったかどうか
	s32 LastBuzz;//最後にBuzzった時間
	s32 Rdeg;    //角度
	f32 XSpeed,YSpeed;                //自機スピード
	s32 Pattern; //行動パターン
	TEnemyGauge2* HPGauge;
	s32 GaugeTime;
	s32 TotalFlight;//飛行距離
	s32 ActFlg[10];     //行動フラグ
	s32 ActCount[10];   //行動カウンタ
	s32 LastFlash;
	bool MissileON;  //ミサイル使用中
	s32 MisCount;   //ミサイル発射動作用カウンタ
	bool BeamON;     //ビーム使用中
	s32 BeamCount;  //ビーム発射動作用カウンタ
	s32 GunR;   //ビーム砲角度
	s32 EnemyDeg; //敵と自分の相対角度
	bool Boost;    //ブースト
	s32 BoostCount;
	f32 BSpeed;
	s32 UnitNum; //機体番号（ミサイル発射時に必要）
	s32 ShotR;    //ショット射角
	s32 ShotCount;
	u8 OriginColor;
	s32 KnockBackSpeed;
	void MissileShot();
	void BeamShot(s32 EnemyDeg);
	void Accel();
	void SerialShot();
	void KnockBack(s32 _impact);
	void KnockBackAction();
public:
	TZako05(TOBJList* owner, s32 _x, s32 _y, s32 _pat, s32 _num, bool _red );
	void Move(); 
	void OnDamaged(TOBJ* target);
	void Die();
	void OnBroken();
};

//敵ミサイル
class TZ05Missile : public TOBJ {
private:
	bool Buzzed;//Buzzったかどうか
	s32 LastBuzz;//最後にBuzzった時間
	s32 YMove; //   Y方向の動き
	s32 Rdeg;
	TSmoke* PrevSmoke;   //前回出した煙オブジェクト
	s32 PrevR;   //前フレームでの角度
public:
	TZ05Missile(TOBJList* owner, s32 sx, s32 sy, s32 _misNum);
	void Move(); 
	void Die(); 
	void OnDamaged(TOBJ* target); 
};

//ビーム素片(太)
class TZBeamPart : public TOBJ {
private:
	bool Buzzed;//Buzzったかどうか
	s32 LastBuzz;//最後にBuzzった時間
	s32 Rdeg;
public:
	TZBeamPart(TOBJList* owner, s32 sx, s32 sy, s32 R, f32 _zoom, bool _hit);
	void Move(); 
	void OnDamaged(TOBJ* target);
};


//ビーム素片(細)
class TFunBeamPart : public TOBJ {
private:
	s32 Rdeg;
	s32 Speed;
	s32 Life;
public:
	TFunBeamPart(TOBJList* owner, s32 sx, s32 sy, s32 R, s32 S, s32 L, f32 _zoom);
	void Move(); 
};

//ビーム本体
class TFunBeamCore : public TOBJ {
private:
	s32 Rdeg;
	bool Buzzed;//Buzzったかどうか
	s32 LastBuzz;//最後にBuzzった時間
	s32 Pattern;
public:
	TFunBeamCore(TOBJList* owner, s32 sx, s32 sy, s32 R, s32 _pat);
	void Move(); 
	void OnDamaged(TOBJ* target);
};


//ホーミングビーム本体
class TZakoHomingCore : public TOBJ {
private:
	bool Buzzed;//Buzzったかどうか
	s32 LastBuzz;//最後にBuzzった時間
	s32 Rdeg;
	s32 PrevR;   //前フレームでの角度
	s32 HomingLevel;    //追尾の曲がりやすさ
	THomingTrace* Trace;   //ビームの軌跡
	s32 DestX,DestY;
	s32 ArriveCount;
	TEnemyLockMarker* Marker;
public:
	TZakoHomingCore(TOBJList* owner, s32 sx, s32 sy, s32 dx, s32 dy, s32 _r, s32 _level);
	~TZakoHomingCore();
	void Move(); 
	void Die(); 
	void OnDamaged(TOBJ* target); 
};

//目からレーザー
class TEyeLaser : public TOBJ {
private:
	s32 Rdeg;
public:
	TEyeLaser(TOBJList* owner, s32 sx, s32 sy, s32 _r);
	void Move(); 
};

//目からレーザー部品
class TEyeLaserParts : public TOBJ {
private:
	bool Buzzed;//Buzzったかどうか
	s32 LastBuzz;//最後にBuzzった時間
	s32 Rdeg;
public:
	TEyeLaserParts(TOBJList* owner, s32 sx, s32 sy, s32 _r);
	void Move(); 
	void OnDamaged(TOBJ* target); 
};

//ホーミングビーム本体その２
class TZakoHomingCore2 : public TOBJ {
private:
	bool Buzzed;//Buzzったかどうか
	s32 LastBuzz;//最後にBuzzった時間
	s32 Rdeg;
	s32 PrevR;   //前フレームでの角度
	s32 HomingLevel;    //追尾の曲がりやすさ
	THomingTrace* Trace;   //ビームの軌跡
public:
	TZakoHomingCore2(TOBJList* owner, s32 sx, s32 sy, s32 _r, s32 _level);
	~TZakoHomingCore2();
	void Move(); 
	void Die(); 
	void OnDamaged(TOBJ* target); 
};

//当たり判定のある光球
class TEnergyBall : public TOBJ {
private:
	bool Buzzed;//Buzzったかどうか
	s32 LastBuzz;//最後にBuzzった時間
	f32 ZoomRate;
	s32 Life;

public:
	TEnergyBall(TOBJList* owner, s32 sx, s32 sy, s32 _life, f32 _zoom);
	void Move(); 
	void OnDamaged(TOBJ* target); 
};

//弾3(スピード、角度、威力、拡大率調節可能)
class TZ01Shot3 : public TOBJ {
private:
	bool Buzzed;//Buzzったかどうか
	s32 LastBuzz;//最後にBuzzった時間
	bool Anim;//アニメーション用変数
	s32 Rdeg;
	s32 Speed;
	f32 OriginXZoom;
	f32 IniXSpeed;//スクロールスピードの影響
public:
	TZ01Shot3(TOBJList* owner, s32 sx, s32 sy, s32 _R, s32 _S, s32 _str, f32 _IniXS, f32 _zoom);
	void Move(); 
	void OnDamaged(TOBJ* target); 
};

//2ボスレーザー部品
class TS2Boss2LaserParts : public TOBJ {
private:
	bool Buzzed;//Buzzったかどうか
	s32 LastBuzz;//最後にBuzzった時間
	s32 Rdeg;
public:
	TS2Boss2LaserParts(TOBJList* owner, s32 sx, s32 sy, s32 _r);
	void Move(); 
	void OnDamaged(TOBJ* target); 
};

//ビーム素片(太) （砲台の移動に追従）
class TZBeamPart2 : public TOBJ {
private:
	bool Buzzed;//Buzzったかどうか
	s32 LastBuzz;//最後にBuzzった時間
	s32 Rdeg;
	f32 XSpeed,YSpeed;
public:
	TZBeamPart2(TOBJList* owner, s32 sx, s32 sy, f32 _xspeed, f32 _yspeed, s32 R, f32 _zoom, bool _hit);
	void Move(); 
	void OnDamaged(TOBJ* target);
};

//ビーム発射演出
class TBeamControl : public TOBJ {
private:
	s32 Rdeg;
	s32 Pattern;
public:
	TBeamControl(TOBJList* owner, s32 sx, s32 sy, s32 _pat);
	void Move(); 
};

//３ボスショット
class TBoss3Shot : public TOBJ {
private:
	bool Buzzed;//Buzzったかどうか
	s32 LastBuzz;//最後にBuzzった時間
	bool Anim;//アニメーション用変数
	s32 Rdeg;
	f32 Speed;
	f32 OriginXZoom;
	f32 IniXSpeed;//スクロールスピードの影響
	s32 Pattern;
	s32 ActFlg;
	s32 ActCount;
	f32 Accel;
	void ExAction();
public:
	TBoss3Shot(TOBJList* owner, s32 sx, s32 sy, s32 _R, s32 _S, s32 _str, f32 _IniXS, f32 _zoom, s32 _pat, f32 _accel);
	void Move(); 
	void OnDamaged(TOBJ* target); 
};

//３ボスビーム
class TBoss3Beam : public TOBJ {
private:
	bool Buzzed;//Buzzったかどうか
	s32 LastBuzz;//最後にBuzzった時間
	s32 Rdeg;
	s32 DestX, DestY;
	THomingTrace* Trace;   //ビームの軌跡
public:
	TBoss3Beam(TOBJList* owner, s32 sx, s32 sy, s32 dx, s32 dy);
	~TBoss3Beam(); 
	void Move(); 
	void Die(); 
	void OnDamaged(TOBJ* target); 
};

//あたり判定の長いビーム
class TLongBeam : public TOBJ {
private:
	bool Buzzed;//Buzzったかどうか
	s32 LastBuzz;//最後にBuzzった時間
	s32 Rdeg;
	f32 MaxZoom;
	f32 ZoomSpeed;
public:
	TLongBeam(TOBJList* owner, s32 sx, s32 sy, s32 R, f32 _zoom, f32 _speed);
	~TLongBeam(); 
	void Move(); 
	void OnDamaged(TOBJ* target); 
};

//ビーム放射
class T8WayBeam : public TOBJ {
private:
	s32 Rdeg;
	s32 ShotCount;
public:
	T8WayBeam(TOBJList* owner, s32 sx, s32 sy);
	void Move(); 
};

//直角レーザー素片
class TOrthLaserPart : public TOBJ {
private:
	bool Buzzed;//Buzzったかどうか
	s32 LastBuzz;//最後にBuzzった時間
	s32 Life;
public:
	TOrthLaserPart(TOBJList* owner, s32 sx, s32 sy, s32 _life, bool _col);
	void Move(); 
	void OnDamaged(TOBJ* target); 
};

//直角レーザー
class TOrthLaser : public TOBJ {
private:
	s32 Speed;
	s32 Life;
	s32 TurnCount;
public:
	TOrthLaser(TOBJList* owner, s32 sx, s32 sy, s32 _speed, s32 _life);
	void Move(); 
};

//曲がるレーザー
class TCurvedLaser : public TOBJ {
private:
	TPolyLine2D* pl;
	vector<glm::vec2> Points;
	vector<TCollisionSpr*> Collision;
	f32 Speed,Accel,Velocity;
	bool Buzzed;//Buzzったかどうか
	s32 LastBuzz;//最後にBuzzった時間
	s32 Rdeg;
	s32 RSpeed;
	s32 ActFlg;
	s32 Pattern;
public:
	TCurvedLaser(TOBJList* owner, s32 _x, s32 _y, s32 _r, s32 _rspeed, f32 _speed, f32 _accel, f32 _vel, s32 _pat);
	~TCurvedLaser();
	void Move(); 
	void OnDamaged(TOBJ* target); 
	void Die(); 
};

//スプレッドビーム
class TSpreadBeam : public TOBJ {
private:
	bool Buzzed;//Buzzったかどうか
	s32 LastBuzz;//最後にBuzzった時間
	s32 Rdeg;
	s32 DestX,DestY;
	f32 Speed;
	s32 ActFlg,ActCount;
public:
	TSpreadBeam(TOBJList* owner, s32 _x, s32 _y, s32 _dx, s32 _dy, f32 _speed);
	void Move(); 
	void OnDamaged(TOBJ* target); 
};

//スプレッド2
class TSpreadBeam2 : public TOBJ {
private:
	s32 Rdeg;
	s32 DestX,DestY;
	f32 Speed;
	s32 ActFlg,ActCount;
public:
	TSpreadBeam2(TOBJList* owner, s32 _x, s32 _y, s32 _R, f32 _speed);
	void Move(); 
};

//ビームの雨
class TBeamRain : public TOBJ {
private:
	s32 Life;
	f32 FallSpeed;
public:
	TBeamRain(TOBJList* owner, s32 _x, s32 _y, s32 _life, f32 _speed);
	void Move(); 
};

//ホーミングレーザー(細)
class TZakoHomingCore3 : public TOBJ {
private:
	TPolyLine2D* pl;
	vector<glm::vec2> Points;
	vector<TCollisionSpr*> Collision;
	f32 Speed,Accel,Velocity;
	bool Buzzed;//Buzzったかどうか
	s32 LastBuzz;//最後にBuzzった時間
	s32 Rdeg;
	s32 RSpeed;
	s32 ActFlg;
	s32 Pattern;
public:
	TZakoHomingCore3(TOBJList* owner, s32 _x, s32 _y, s32 _r, f32 _speed, f32 _accel, s32 _pat);
	~TZakoHomingCore3(); 
	void Move(); 
	void Die(); 
	void OnDamaged(TOBJ* target); 
};

//スロービーム
class TSlowBeam : public TOBJ {
private:
	f32 Speed;
	s32 Life;
	f32 ZoomRate;
	bool Buzzed;//Buzzったかどうか
	s32 LastBuzz;//最後にBuzzった時間
public:
	TSlowBeam(TOBJList* owner, s32 _x, s32 _y, s32 _R, s32 _L, f32 _speed, f32 _zoom);
	void Move(); 
	void OnDamaged(TOBJ* target); 
};


//イレイザー本体
class TEraserCore : public TOBJ {
private:
public:
	TEraserCore(TOBJList* owner, s32 sx, s32 sy);
	void Move(); 
};

//イレイザー素片
class TEraserPart : public TOBJ {
private:
public:
	TEraserPart(TOBJList* owner, s32 sx, s32 sy);
	void Move(); 
};

//当たり判定のある光球 バースト
class TEnergyBallBurst : public TOBJ {
private:
	f32 ZoomRate;
	s32 Life;
	s32 Amount;
public:
	TEnergyBallBurst(TOBJList* owner, s32 sx, s32 sy, s32 _amount, s32 _life, f32 _zoom);
	void Move(); 
};

//ビーム2
class TZBeam2 : public TOBJ {
private:
	bool Buzzed;//Buzzったかどうか
	s32 LastBuzz;//最後にBuzzった時間
	TJoint* Parent;
public:
	TZBeam2(TOBJList* owner, TJoint* _parent, s32 sx, s32 sy, f32 _zoom);
	void Move(); 
	void OnDamaged(TOBJ* target); 
	void Die(); 
};

//弾4（弾２のグラフィック変更）
class TZ01Shot4 : public TOBJ {
private:
	bool Buzzed;//Buzzったかどうか
	s32 LastBuzz;//最後にBuzzった時間
	bool Anim;//アニメーション用変数
	s32 Rdeg;
	s32 Speed;
	f32 IniXSpeed;//スクロールスピードの影響
	s32 BulletColor;  //弾の色（0:ステージごとに最適化　1:赤　2:青）
public:
	TZ01Shot4(TOBJList* owner, s32 sx, s32 sy, s32 _R, s32 _S, f32 _IniXS, s32 _col, s32 _str);
	void Move(); 
	void OnDamaged(TOBJ* target); 
};

enum{
	//突撃くん部品番号
	Z2Body		= 0,
	Z2Wing		= 1,
	Z2Tail		= 2,
	Z2Sonic		= 3,
};

//小型くん部品番号
enum{
	Z4Body      = 0,
	Z4UpperWing = 1,
	Z4lowerWing = 2,
	Z4Fire      = 3,
};

//大型くん部品番号
enum{
	Z5Body		= 0,
	Z5Wing		= 1,
	Z5Hatch		= 2,
	Z5Gun		= 3,
	Z5BodyR		= 4,
	Z5WingR		= 5,
	Z5HatchR	= 6,
	Z5GunR		= 7,
};

