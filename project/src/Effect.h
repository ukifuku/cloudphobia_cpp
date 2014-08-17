#pragma once

using namespace std;

#include "types.h"
#include "TOBJ.h"
#include <vector>

class TGlassCubeMaker;

//基本爆発
class TExpBasic : public TOBJ {
  private:
    s32 Rdeg;//回転角
    f32 Speed;//スピード
  public:
    TExpBasic(TOBJList* owner, s32 sx, s32 sy, s32 R, f32 S, f32 _zoom, u16 _flg);
    void Move(); 
};

//小小爆発
class TExpSS : public TOBJ {
  private:
    s32 Rdeg;//回転角
    s32 Speed;//スピード
  public:
    TExpSS(TOBJList* owner, s32 sx, s32 sy, s32 R, s32 S);
    void Move(); 
};

//火花素片
class TParticle : public TOBJ {
  private:
    s32 Life;  //寿命
    s32 Speed;
    s32 Rdeg;
  public:
    TParticle(TOBJList* owner, s32 sx, s32 sy, s32 R, s32 S, s32 L);
    void Move(); 
};

//火花その2
class TParticle2 : public TOBJ {
  private:
    s32 Life;  //寿命
    s32 Speed;
    s32 Rdeg;
    s32 pattern;
  public:
    TParticle2(TOBJList* owner, s32 sx, s32 sy, s32 _pat, s32 R, s32 S, s32 L);
    void Move(); 
};

//火花制御クラス(カスリ)
class TFF : public TOBJ {
  private:
    s32 Rdeg;
  public:
    TFF(TOBJList* owner, s32 sx, s32 sy, s32 R);
    void Move(); 
};

//火花制御その2（爆発）
class TFF2 : public TOBJ {
  private:
    s32 alpha;
  public:
    TFF2(TOBJList* owner, s32 sx, s32 sy);
    void Move(); 
};


//薬莢
class TBullet : public TOBJ {
  private:
    s32 IncR;
  public:
    TBullet(TOBJList* owner, s32 sx, s32 sy);
    void Move(); 
};

//ショット時の火花
class TShotFire : public TOBJ {
  public:
    TShotFire(TOBJList* owner, s32 _x, s32 _y, s32 Rdeg, f32 _zoom);
    void Move(); 
};

//爆発の輪っか
class TFCircle : public TOBJ {
  private:
    s32 Pattern;
    f32 zoomrate; //拡大率
    s32 Rdeg;
    s32 Speed;
  public:
    TFCircle(TOBJList* owner, s32 _x, s32 _y, s32 _pat, s32 _R, s32 _S);
    void Move(); 
};

//四角
class TBox : public TOBJ {
  private:
    s32 BoxCol;//色(0:黒　1:白)
    u8 BValue;
  public:
    TBox(TOBJList* owner, s32 _x, s32 _y, s32 _xx, s32 _yy, s32 _col, u16 _z, u8 _val);
    void Move(); 
};

//四角制御
class TBoxContrl : public TOBJ {
  private:
    s32 Pattern;
  public:
    TBoxContrl(TOBJList* owner, s32 _pat);
    void Move(); 
};

//ロックオンマーカー
class TMarker : public TOBJ {
  private:
    f32 ZoomRate;
    s32 R;
  public:
    TMarker(TOBJList* owner, s32 num);
    void Move(); 
};

//バーニア光
class TBLight : public TOBJ {
  public:
    TBLight(TOBJList* owner, s32 sx, s32 sy, f32 _zoom, u16 _z);
    void Move(); 
};

//ダミー
class TDummy : public TOBJ {
  public:
    TDummy(TOBJList* owner, u16 _z);
    void Move(); 
};

//煙リング素片
class TSmokeRing : public TOBJ {
  public:
    TSmokeRing(TOBJList* owner, s32 sx, s32 sy, s32 _R, f32 _Zoom, s32 _age);
    void Move(); 
};

//噴射煙リング
class TSRControl : public TOBJ {
  private:
    OBJParts* RP[17];
    s32 Rdeg;
    s32 Speed;
  public:
    TSRControl(TOBJList* owner, s32 sx, s32 sy, s32 _R);
    void Move(); 
};

//雲（エフェクト用）素片
class TCloudPart : public TOBJ {
  private:
    s32 Rdeg;//回転角
    s32 Speed;//スピード
  public:
    TCloudPart(TOBJList* owner, s32 sx, s32 sy, s32 R, s32 S, s32 Z, f32 _zoom);
    void Move(); 
};

//雲爆発
class TCloudExp : public TOBJ {
  private:
    s32 Rdeg;//回転角
    s32 Speed;//スピード
    u16 Zflg;
    u16 SetZ;
  public:
    TCloudExp(TOBJList* owner, s32 sx, s32 sy, s32 R, s32 S, u16 _flg);
    void Move(); 
};

//風素片
class TKazeParts : public TOBJ {
  public:
    TKazeParts(TOBJList* owner, s32 sx, s32 sy, s32 _a, f32 _zoom);
    void Move(); 
};

//風
class TKaze : public TOBJ {
  public:
    TKaze(TOBJList* owner, s32 sx, s32 sy, s32 _a);
    void Move(); 
};

//ヴェイパー
class TVapor : public TOBJ {
  private:
    s32 AValue;
    f32 kazeSpeed;
  public:
    TVapor(TOBJList* owner, s32 sx, s32 sy, s32 px, s32 py, s32 _a, f32 _zoom);
    void Move(); 
};

//ビーム照準
class TBeamSite : public TOBJ {
  public:
    TBeamSite(TOBJList* owner, s32 sx, s32 sy, s32 _r, s32 _a);
    void Move(); 
};

//ミサイル発射
class TMissileFlash : public TOBJ {
  public:
    TMissileFlash(TOBJList* owner, s32 sx, s32 sy);
    void Move(); 
};

//まぶしい光球
class TBlightBall : public TOBJ {
  public:
    TBlightBall(TOBJList* owner, s32 sx, s32 sy, s32 _R, f32 _a);
    void Move(); 
};

//ファンネル残像
class TFunLight : public TOBJ {
  private:
    s32 Life;
  public:
    TFunLight(TOBJList* owner, s32 sx, s32 sy, f32 _zoom, bool _Slide);
    void Move(); 
};

//普通の光球
class TLight : public TOBJ {
  public:
    TLight(TOBJList* owner, s32 sx, s32 sy, f32 _zoom);
    void Move(); 
};

//ボス弱点表示
class TWeakPoint : public TOBJ {
  private:
    s32 RDeg;
    s32 BossNumber;
    f32 ZoomRate;
  public:
    TWeakPoint(TOBJList* owner, s32 _bossnum);
    void Move(); 
};

//ミラー
class TMirror : public TOBJ {
  private:
  public:
    TMirror(TOBJList* owner);
    void Move(); 
};

class TSplashBasic : public TOBJ {
  private:
    f32 ZoomRateX,ZoomRateY; 
    f32 MaxZoom;
    s32 CurrentAngle;
  public:
    TSplashBasic(TOBJList* owner, s32 sx, s32 sy, s32 _R, s32 _black, f32 _zoomx, f32 _zoomy);
    void Move(); 
};

class TSplash : public TOBJ {
  private:
    f32 Level;
  public:
    TSplash(TOBJList* owner, s32 sx, f32 _level);
    void Move(); 
};

class TTestSprite : public TOBJ {
  private:
  public:
    TTestSprite(TOBJList* owner);
    void Move(); 
};

class TCursorBack : public TOBJ {
  private:
  public:
    TCursorBack(TOBJList* owner, s32 _x, s32 _y, s32 _long, s32 _alpha);
    void Move(); 
};

class TGlassCursor : public TOBJ {
  private:
    s32 XSpeed,YSpeed;   //カーソル移動スピード
    s32 NowX,NowY;   //カーソル現在位置
    s32 DestX,DestY;  //目的地座標
    vector<TPoint> MovePoint;
    s32 CurrentNum;
    void Slide();
    void SetDest(s32 _x, s32 _y);
  public:
    TGlassCursor(TOBJList* owner, s32 _x, s32 _y, s32 _long);
    ~TGlassCursor();
    void Move(); 
    void Show();
    void Hide();
    void Die();  
    void NextPoint(s32 _n);
    void SetMovePoint(s32 _x, s32 _y);
    void ClearPoints();
    s32 GetCurrentPos();
    void MoveToHome();
    void MoveToEnd();
};

class TSlashedEffectBasic : public TOBJ {
  private:
    s32 Alpha;
    s32 Rdeg;
  public:
    TSlashedEffectBasic(TOBJList* owner, s32 sx, s32 sy, s32 _R, f32 _zoomx, f32 _zoomy);
    void Move(); 
};

class TSlashedEffect : public TOBJ {
  private:
    s32 Alpha;
    s32 Level;
    s32 Rdeg;
  public:
    TSlashedEffect(TOBJList* owner, s32 sx, s32 sy, s32 _R, s32 _level);
    void Move(); 
};

//火花その3
class TParticle3 : public TOBJ {
  private:
    s32 Life;  //寿命
    s32 Speed;
    s32 Rdeg;
  public:
    TParticle3(TOBJList* owner, s32 _x, s32 _y, s32 _R, s32 _speed, s32 _life, s32 _red, s32 _green, s32 _blue);
    void Move(); 
};

//火花制御その3
class TFF3 : public TOBJ {
  private:
    s32 alpha;
    s32 Rdeg;
    s32 Red,Green,Blue;
  public:
    TFF3(TOBJList* owner, s32 sx, s32 sy, s32 _R, s32 _red, s32 _green, s32 _blue);
    void Move(); 
};

//基本爆発リメイク
class TExpBasic2 : public TOBJ {
  private:
    s32 Rdeg;//回転角
    f32 Speed;//スピード
    s32 PersonalR;
    s32 IncR;
    f32 ZoomRateX,ZoomRateY;
  public:
    TExpBasic2(TOBJList* owner, s32 _x, s32 _y, s32 _r, f32 _speed, f32 _zx, f32 _zy);
    void Move(); 
};

//基本爆発リメイク(スピードと拡大率が変化)
class TExpBasic3 : public TOBJ {
  private:
    s32 Rdeg;//回転角
    f32 Speed;//スピード
    s32 PersonalR;
    s32 IncR;
    f32 ZoomRateX,ZoomRateY;
    f32 Deccel;
  public:
    TExpBasic3(TOBJList* owner, s32 _x, s32 _y, s32 _r, s32 _distance, f32 _speed, f32 _deccel, f32 _zoom, u16 _zflg);
    void Move(); 
};

//爆発煙リメイク
class TExpSmoke : public TOBJ {
  private:
    s32 Rdeg;//回転角
    f32 Speed;//スピード
    s32 PersonalR;
    s32 IncR;
    f32 ZoomRateX,ZoomRateY;
  public:
    TExpSmoke(TOBJList* owner, s32 _x, s32 _y, s32 _r, f32 _speed, f32 _zx, f32 _zy);
    void Move(); 
};

//爆発煙リメイク(スピードと拡大率が変化)
class TExpSmoke2 : public TOBJ {
  private:
    s32 Rdeg;//回転角
    f32 Speed;//スピード
    s32 PersonalR;
    s32 IncR;
    f32 ZoomRateX,ZoomRateY;
    f32 Deccel;
  public:
    TExpSmoke2(TOBJList* owner, s32 _x, s32 _y, s32 _r, s32 _distance, f32 _speed, f32 _deccel, f32 _zoom, u16 _zflg);
    void Move(); 
};

//中爆発リメイク
class TExpmiddle : public TOBJ {
  private:
    s32 Rdeg;//回転角
    s32 Speed;//スピード
    u16 ZFlg;
  public:
    TExpmiddle(TOBJList* owner, s32 sx, s32 sy, s32 R, s32 S, u16 _flg);
    void Move(); 
};

//大爆発リメイク
class TExplarge : public TOBJ {
  private:
    u16 ZFlg;
    bool Parts;
  public:
    TExplarge(TOBJList* owner, s32 sx, s32 sy, u16 _flg, bool _parts);
    void Move(); 
};

//小爆発リメイク
class TExpSmall : public TOBJ {
  private:
    s32 Rdeg;//回転角
    s32 Speed;//スピード
    u16 ZFlg;
  public:
    TExpSmall(TOBJList* owner, s32 sx, s32 sy, s32 R, s32 S, u16 _flg);
    void Move(); 
};

//ブレードやられ爆発
class TExpSlashed : public TOBJ {
  private:
    s32 Rdeg;//回転角
    s32 Speed;//スピード
  public:
    TExpSlashed(TOBJList* owner, s32 sx, s32 sy, s32 R, s32 S, u16 _flg);
    void Move(); 
};

//破片
class TBrokenPiece : public TOBJ {
  private:
    s32 Rdeg;//回転角
    s32 Speed;//スピード
    s32 IncR;
  public:
    TBrokenPiece(TOBJList* owner, s32 sx, s32 sy, s32 R, s32 S, u16 _flg);
    void Move(); 
};

//ボス弱点表示リメイク
class TWeakPoint2 : public TOBJ {
  private:
    TJoint* BossObj;
  public:
    TWeakPoint2(TOBJList* owner, TJoint* _boss);
    void Move(); 
};

//ボス弱点素片
class TWeakPointPart : public TOBJ {
  private:
    TJoint* BossObj;
  public:
    TWeakPointPart(TOBJList* owner, TJoint* _boss, s32 _r);
    void Move(); 
};

//バストショット
class TBustShot : public TOBJ {
  private:
    s32 WeaponType;
    s32 CurrentBumpAlpha;
    s32 DestBumpAlpha;
    f32 DestBumpLevel;
    f32 FirstBumpLevel;
  public:
    TBustShot(TOBJList* owner, s32 _x, s32 _y, s32 _destx, s32 _desty, s32 _type, bool _lr);
    void Move(); 
    void Show();
    void Hide();
    void ChangeColor(s32 _pat);
};

//バストショット用　眼光
class TEyeLight : public TOBJ {
  private:
  public:
    TEyeLight(TOBJList* owner, s32 _x, s32 _y);
    void Move(); 
};

//矢印
class TArrow : public TOBJ {
  private:
  public:
    TArrow(TOBJList* owner, s32 _x, s32 _y, s32 _pat);
    void Move(); 
    void Hide();
    void Show();
    void SetPlace(s32 _x, s32 _y);
};

//ヴェイパーリメイク
class TVapor2 : public TOBJ {
  private:
    f32 ZoomRate;
    s32 PersonalR;
    s32 IncR;
    TVapor2* PrevSmoke;    //直前に生成された煙
    bool MotherIsDead;  //自分を生成した本体が死んだ
  public:
    TVapor2* NextSmoke;   //直後に生成された煙
    TVapor2(TOBJList* owner, s32 sx, s32 sy, TVapor2* _prev);
    void Move(); 
    void TellMomDead();
    void Die(); 
};

//ヴェイパー管理クラス
class TVaporControl : public TOBJ {
  private:
    vector<TVapor2*> PrevSmoke;   //前回出した煙オブジェクト
  public:
    TVaporControl(TOBJList* owner, s32 _maxvapor);
    void MakeVapor(s32 _x, s32 _y, s32 _num);
    void Move(); 
    void Die(); 
    void Clear();
};

//目くらまし
class TSunShine : public TOBJ {
  private:
  public:
    TSunShine(TOBJList* owner);
    void Move(); 
};

//火花その4
class TParticle4 : public TOBJ {
  private:
    s32 Life;  //寿命
    s32 Speed;
    s32 Rdeg;
    f32 CurrX,CurrY;
    f32 PrevX,PrevY;
  public:
    TParticle4(TOBJList* owner, s32 _x, s32 _y, s32 _R, s32 _speed, s32 _life, s32 _red, s32 _green, s32 _blue);
    void Move(); 
};

//ガラスキューブエフェクトフィルタ
class TGlassCubeFilter : public TOBJ {
  private:
    TGlassCubeMaker* Cubes;
    bool EndFlg;
  public:
    TGlassCubeFilter(TOBJList* owner, s32 _pat);
    void Move(); 
    void Die(); 
    void EndProc();
    void OnDeviceLost(); 
    void OnDeviceReset(); 
};

//水滴エフェクトフィルタ
class TRainFilter : public TOBJ {
  private:
  public:
    TRainFilter(TOBJList* owner);
    void Move(); 
    void OnDeviceReset(); 
};

//アルファエフェクトフィルタ
class TAlphaEffectFilter : public TOBJ {
  private:
  public:
    TAlphaEffectFilter(TOBJList* owner);
    void Move(); 
    void OnDeviceReset(); 
};

//加算エフェクトフィルタ
class TAddEffectFilter : public TOBJ {
  private:
  public:
    TAddEffectFilter(TOBJList* owner);
    void Move(); 
    void OnDeviceReset(); 
};

//母艦
class TMotherShip : public TOBJ {
  private:
    bool SmokeON;
  public:
    TMotherShip(TOBJList* owner, s32 _x, s32 _y);
    void Move(); 
    void MakeSmoke();
};

//母艦から出る煙
class TMotherShipSmoke : public TOBJ {
  private:
    f32 XSpeed, YSpeed;
  public:
    TMotherShipSmoke(TOBJList* owner, s32 _x, s32 _y, f32 _xspeed, f32 _yspeed, f32 _zoomX, f32 _zoomY);
    void Move(); 
};

//母艦から出る炎
class TMotherShipFire : public TOBJ {
  private:
    f32 XSpeed,YSpeed;
  public:
    TMotherShipFire(TOBJList* owner, s32 _x, s32 _y, f32 _xspeed, f32 _yspeed, f32 _zoomX, f32 _zoomY);
    void Move(); 
};

//イレイザー（デモ版）
class TDemoEraser : public TOBJ {
  private:
  public:
    TDemoEraser(TOBJList* owner);
    void Move(); 
    void ShowEraserParts(bool _show);
};

//ボス死亡時カットイン
class TBossDyingCutin : public TOBJ {
  private:
    s32 Life;
  public:
    TBossDyingCutin(TOBJList* owner, s32 _life);
    void Move(); 
};

//ボス眼光
class TBossEyeLight : public TOBJ {
  private:
    s32 Life;
  public:
    TBossEyeLight(TOBJList* owner, s32 _x, s32 _y, s32 _life);             
    void Move(); 
};

//火花（エネルギーチャージ）
class TParticle_charge : public TOBJ {
  private:
    s32 Life;  //寿命
    s32 Speed;
    s32 Rdeg;
  public:
    TParticle_charge(TOBJList* owner, s32 _x, s32 _y, s32 _R, s32 _speed, s32 _life, s32 _red, s32 _green, s32 _blue);
    void Move(); 
};

//任意の領域にパーティクルをばらまく
class TParticleSpreader : public TOBJ {
  private:
    s32 Life;  //寿命
    s32 Rdeg;
    s32 Width;
    s32 Red,Green,Blue;
  public:
    TParticleSpreader(TOBJList* owner, s32 _x, s32 _y, s32 _width, s32 _R, s32 _life, s32 _red, s32 _green, s32 _blue);
    void Move(); 
};

//火花その5
class TParticle5 : public TOBJ {
  private:
    s32 Gain;  
    s32 Speed;
  public:
    TParticle5(TOBJList* owner, s32 _x, s32 _y, s32 _r, s32 _speed, s32 _life, s32 _red, s32 _green, s32 _blue, f32 _size);
    void Move(); 
};

//光の輪っか
class TLightCircle : public TOBJ {
  private:
    f32 ZoomRate;
  public:
    TLightCircle(TOBJList* owner, s32 _x, s32 _y, s32 _red, s32 _green, s32 _blue, f32 _size);
    void Move(); 
};

//血
class TBlood : public TOBJ {
  private:
    s32 Rdeg;
  public:
    TBlood(TOBJList* owner, s32 _x, s32 _y, s32 _R);
    void Move(); 
};

//オーラ
class TAura : public TOBJ {
  private:
    s32 RSpeed;
  public:
    TAura(TOBJList* owner, s32 _x, s32 _y);
    void Move(); 
};

//ワープ演出(2D)
class TWarp2D : public TOBJ {
  private:
    s32 ActFlg,ActCount;
    f32 Zoomrate;
    s32 Pattern;
    void MakeParticle();
  public:
    TWarp2D(TOBJList* owner, s32 _x, s32 _y, s32 _pat);
    void Move(); 
};

//ワープ演出(2D) パーツ
class TWarp2DPart : public TOBJ {
  private:
    s32 ActFlg,ActCount;
    f32 Zoomrate;
  public:
    TWarp2DPart(TOBJList* owner, s32 _x, s32 _y, f32 _Zoom, s32 _r);
    void Move(); 
};


//ワープ演出(2D) 輪っか
class TWarp2DCircle : public TOBJ {
  private:
  public:
    TWarp2DCircle(TOBJList* owner, s32 _x, s32 _y);
    void Move(); 
};

//ショット時の火花 (大)
class TShotFireLarge : public TOBJ {
  public:
    TShotFireLarge(TOBJList* owner, s32 _x, s32 _y, f32 _zoom);
    void Move(); 
};

//ショット時の火花 (煙つき)
class TShotFireWithSmoke : public TOBJ {
  private:
    s32 Rdeg;
    f32 ZoomRate;
  public:
    TShotFireWithSmoke(TOBJList* owner, s32 _x, s32 _y, s32 _r, f32 _zoom);
    void Move(); 
};

//細ビーム発射エフェクト
class TBeamShotEffect : public TOBJ {
  private:
    f32 ZoomRate;
    s32 Life;
    TJoint* Parent;
    s32 ActFlg;
  public:
    TBeamShotEffect(TOBJList* owner, TJoint* _parent, s32 _x, s32 _y, s32 _life, f32 _zoom);
    void Move(); 
};

//ボス爆風
class TBossExpWind : public TDemoEraser {
  private:
  public:
    TBossExpWind(TOBJList* owner);
    void Move(); 
};

//影
class TBossShadow : public TOBJ {
  private:
    s32 MaxHeight,MinHeight;
    TJoint* Parent;
    s32 DX;
  public:
    TBossShadow(TOBJList* owner, TJoint* _parent, s32 _x, s32 _y, s32 _high, s32 _low);
    void Move(); 
};

//ガレキ
class TDebris : public TOBJ {
  private:
    s32 Rdeg;//回転角
    s32 Speed;//スピード
    s32 IncR;
  public:
    TDebris(TOBJList* owner, s32 sx, s32 sy, s32 R, s32 S, u16 _flg);
    void Move(); 
};

//ガレキまき散らし
class TSplashDebris : public TOBJ {
  private:
  public:
    TSplashDebris(TOBJList* owner, s32 sx, s32 sy);
    void Move(); 
};

//敵ロックオンマーカー
class TEnemyLockMarker : public TOBJ {
  private:
    f32 ZoomRate;
    f32 CurrentZoom;
  public:
    TEnemyLockMarker(TOBJList* owner, s32 _x, s32 _y, f32 _zoom);
    void Move(); 
};
