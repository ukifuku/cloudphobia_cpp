#pragma once

using namespace std;

#include "types.h"
#include "TOBJ.h"
#include <vector>

class TGlassCubeMaker;

//��{����
class TExpBasic : public TOBJ {
  private:
    s32 Rdeg;//��]�p
    f32 Speed;//�X�s�[�h
  public:
    TExpBasic(TOBJList* owner, s32 sx, s32 sy, s32 R, f32 S, f32 _zoom, u16 _flg);
    void Move(); 
};

//��������
class TExpSS : public TOBJ {
  private:
    s32 Rdeg;//��]�p
    s32 Speed;//�X�s�[�h
  public:
    TExpSS(TOBJList* owner, s32 sx, s32 sy, s32 R, s32 S);
    void Move(); 
};

//�Ήԑf��
class TParticle : public TOBJ {
  private:
    s32 Life;  //����
    s32 Speed;
    s32 Rdeg;
  public:
    TParticle(TOBJList* owner, s32 sx, s32 sy, s32 R, s32 S, s32 L);
    void Move(); 
};

//�ΉԂ���2
class TParticle2 : public TOBJ {
  private:
    s32 Life;  //����
    s32 Speed;
    s32 Rdeg;
    s32 pattern;
  public:
    TParticle2(TOBJList* owner, s32 sx, s32 sy, s32 _pat, s32 R, s32 S, s32 L);
    void Move(); 
};

//�ΉԐ���N���X(�J�X��)
class TFF : public TOBJ {
  private:
    s32 Rdeg;
  public:
    TFF(TOBJList* owner, s32 sx, s32 sy, s32 R);
    void Move(); 
};

//�ΉԐ��䂻��2�i�����j
class TFF2 : public TOBJ {
  private:
    s32 alpha;
  public:
    TFF2(TOBJList* owner, s32 sx, s32 sy);
    void Move(); 
};


//���
class TBullet : public TOBJ {
  private:
    s32 IncR;
  public:
    TBullet(TOBJList* owner, s32 sx, s32 sy);
    void Move(); 
};

//�V���b�g���̉Ή�
class TShotFire : public TOBJ {
  public:
    TShotFire(TOBJList* owner, s32 _x, s32 _y, s32 Rdeg, f32 _zoom);
    void Move(); 
};

//�����̗ւ���
class TFCircle : public TOBJ {
  private:
    s32 Pattern;
    f32 zoomrate; //�g�嗦
    s32 Rdeg;
    s32 Speed;
  public:
    TFCircle(TOBJList* owner, s32 _x, s32 _y, s32 _pat, s32 _R, s32 _S);
    void Move(); 
};

//�l�p
class TBox : public TOBJ {
  private:
    s32 BoxCol;//�F(0:���@1:��)
    u8 BValue;
  public:
    TBox(TOBJList* owner, s32 _x, s32 _y, s32 _xx, s32 _yy, s32 _col, u16 _z, u8 _val);
    void Move(); 
};

//�l�p����
class TBoxContrl : public TOBJ {
  private:
    s32 Pattern;
  public:
    TBoxContrl(TOBJList* owner, s32 _pat);
    void Move(); 
};

//���b�N�I���}�[�J�[
class TMarker : public TOBJ {
  private:
    f32 ZoomRate;
    s32 R;
  public:
    TMarker(TOBJList* owner, s32 num);
    void Move(); 
};

//�o�[�j�A��
class TBLight : public TOBJ {
  public:
    TBLight(TOBJList* owner, s32 sx, s32 sy, f32 _zoom, u16 _z);
    void Move(); 
};

//�_�~�[
class TDummy : public TOBJ {
  public:
    TDummy(TOBJList* owner, u16 _z);
    void Move(); 
};

//�������O�f��
class TSmokeRing : public TOBJ {
  public:
    TSmokeRing(TOBJList* owner, s32 sx, s32 sy, s32 _R, f32 _Zoom, s32 _age);
    void Move(); 
};

//���ˉ������O
class TSRControl : public TOBJ {
  private:
    OBJParts* RP[17];
    s32 Rdeg;
    s32 Speed;
  public:
    TSRControl(TOBJList* owner, s32 sx, s32 sy, s32 _R);
    void Move(); 
};

//�_�i�G�t�F�N�g�p�j�f��
class TCloudPart : public TOBJ {
  private:
    s32 Rdeg;//��]�p
    s32 Speed;//�X�s�[�h
  public:
    TCloudPart(TOBJList* owner, s32 sx, s32 sy, s32 R, s32 S, s32 Z, f32 _zoom);
    void Move(); 
};

//�_����
class TCloudExp : public TOBJ {
  private:
    s32 Rdeg;//��]�p
    s32 Speed;//�X�s�[�h
    u16 Zflg;
    u16 SetZ;
  public:
    TCloudExp(TOBJList* owner, s32 sx, s32 sy, s32 R, s32 S, u16 _flg);
    void Move(); 
};

//���f��
class TKazeParts : public TOBJ {
  public:
    TKazeParts(TOBJList* owner, s32 sx, s32 sy, s32 _a, f32 _zoom);
    void Move(); 
};

//��
class TKaze : public TOBJ {
  public:
    TKaze(TOBJList* owner, s32 sx, s32 sy, s32 _a);
    void Move(); 
};

//���F�C�p�[
class TVapor : public TOBJ {
  private:
    s32 AValue;
    f32 kazeSpeed;
  public:
    TVapor(TOBJList* owner, s32 sx, s32 sy, s32 px, s32 py, s32 _a, f32 _zoom);
    void Move(); 
};

//�r�[���Ə�
class TBeamSite : public TOBJ {
  public:
    TBeamSite(TOBJList* owner, s32 sx, s32 sy, s32 _r, s32 _a);
    void Move(); 
};

//�~�T�C������
class TMissileFlash : public TOBJ {
  public:
    TMissileFlash(TOBJList* owner, s32 sx, s32 sy);
    void Move(); 
};

//�܂Ԃ�������
class TBlightBall : public TOBJ {
  public:
    TBlightBall(TOBJList* owner, s32 sx, s32 sy, s32 _R, f32 _a);
    void Move(); 
};

//�t�@���l���c��
class TFunLight : public TOBJ {
  private:
    s32 Life;
  public:
    TFunLight(TOBJList* owner, s32 sx, s32 sy, f32 _zoom, bool _Slide);
    void Move(); 
};

//���ʂ̌���
class TLight : public TOBJ {
  public:
    TLight(TOBJList* owner, s32 sx, s32 sy, f32 _zoom);
    void Move(); 
};

//�{�X��_�\��
class TWeakPoint : public TOBJ {
  private:
    s32 RDeg;
    s32 BossNumber;
    f32 ZoomRate;
  public:
    TWeakPoint(TOBJList* owner, s32 _bossnum);
    void Move(); 
};

//�~���[
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
    s32 XSpeed,YSpeed;   //�J�[�\���ړ��X�s�[�h
    s32 NowX,NowY;   //�J�[�\�����݈ʒu
    s32 DestX,DestY;  //�ړI�n���W
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

//�ΉԂ���3
class TParticle3 : public TOBJ {
  private:
    s32 Life;  //����
    s32 Speed;
    s32 Rdeg;
  public:
    TParticle3(TOBJList* owner, s32 _x, s32 _y, s32 _R, s32 _speed, s32 _life, s32 _red, s32 _green, s32 _blue);
    void Move(); 
};

//�ΉԐ��䂻��3
class TFF3 : public TOBJ {
  private:
    s32 alpha;
    s32 Rdeg;
    s32 Red,Green,Blue;
  public:
    TFF3(TOBJList* owner, s32 sx, s32 sy, s32 _R, s32 _red, s32 _green, s32 _blue);
    void Move(); 
};

//��{���������C�N
class TExpBasic2 : public TOBJ {
  private:
    s32 Rdeg;//��]�p
    f32 Speed;//�X�s�[�h
    s32 PersonalR;
    s32 IncR;
    f32 ZoomRateX,ZoomRateY;
  public:
    TExpBasic2(TOBJList* owner, s32 _x, s32 _y, s32 _r, f32 _speed, f32 _zx, f32 _zy);
    void Move(); 
};

//��{���������C�N(�X�s�[�h�Ɗg�嗦���ω�)
class TExpBasic3 : public TOBJ {
  private:
    s32 Rdeg;//��]�p
    f32 Speed;//�X�s�[�h
    s32 PersonalR;
    s32 IncR;
    f32 ZoomRateX,ZoomRateY;
    f32 Deccel;
  public:
    TExpBasic3(TOBJList* owner, s32 _x, s32 _y, s32 _r, s32 _distance, f32 _speed, f32 _deccel, f32 _zoom, u16 _zflg);
    void Move(); 
};

//�����������C�N
class TExpSmoke : public TOBJ {
  private:
    s32 Rdeg;//��]�p
    f32 Speed;//�X�s�[�h
    s32 PersonalR;
    s32 IncR;
    f32 ZoomRateX,ZoomRateY;
  public:
    TExpSmoke(TOBJList* owner, s32 _x, s32 _y, s32 _r, f32 _speed, f32 _zx, f32 _zy);
    void Move(); 
};

//�����������C�N(�X�s�[�h�Ɗg�嗦���ω�)
class TExpSmoke2 : public TOBJ {
  private:
    s32 Rdeg;//��]�p
    f32 Speed;//�X�s�[�h
    s32 PersonalR;
    s32 IncR;
    f32 ZoomRateX,ZoomRateY;
    f32 Deccel;
  public:
    TExpSmoke2(TOBJList* owner, s32 _x, s32 _y, s32 _r, s32 _distance, f32 _speed, f32 _deccel, f32 _zoom, u16 _zflg);
    void Move(); 
};

//�����������C�N
class TExpmiddle : public TOBJ {
  private:
    s32 Rdeg;//��]�p
    s32 Speed;//�X�s�[�h
    u16 ZFlg;
  public:
    TExpmiddle(TOBJList* owner, s32 sx, s32 sy, s32 R, s32 S, u16 _flg);
    void Move(); 
};

//�唚�������C�N
class TExplarge : public TOBJ {
  private:
    u16 ZFlg;
    bool Parts;
  public:
    TExplarge(TOBJList* owner, s32 sx, s32 sy, u16 _flg, bool _parts);
    void Move(); 
};

//�����������C�N
class TExpSmall : public TOBJ {
  private:
    s32 Rdeg;//��]�p
    s32 Speed;//�X�s�[�h
    u16 ZFlg;
  public:
    TExpSmall(TOBJList* owner, s32 sx, s32 sy, s32 R, s32 S, u16 _flg);
    void Move(); 
};

//�u���[�h���ꔚ��
class TExpSlashed : public TOBJ {
  private:
    s32 Rdeg;//��]�p
    s32 Speed;//�X�s�[�h
  public:
    TExpSlashed(TOBJList* owner, s32 sx, s32 sy, s32 R, s32 S, u16 _flg);
    void Move(); 
};

//�j��
class TBrokenPiece : public TOBJ {
  private:
    s32 Rdeg;//��]�p
    s32 Speed;//�X�s�[�h
    s32 IncR;
  public:
    TBrokenPiece(TOBJList* owner, s32 sx, s32 sy, s32 R, s32 S, u16 _flg);
    void Move(); 
};

//�{�X��_�\�������C�N
class TWeakPoint2 : public TOBJ {
  private:
    TJoint* BossObj;
  public:
    TWeakPoint2(TOBJList* owner, TJoint* _boss);
    void Move(); 
};

//�{�X��_�f��
class TWeakPointPart : public TOBJ {
  private:
    TJoint* BossObj;
  public:
    TWeakPointPart(TOBJList* owner, TJoint* _boss, s32 _r);
    void Move(); 
};

//�o�X�g�V���b�g
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

//�o�X�g�V���b�g�p�@���
class TEyeLight : public TOBJ {
  private:
  public:
    TEyeLight(TOBJList* owner, s32 _x, s32 _y);
    void Move(); 
};

//���
class TArrow : public TOBJ {
  private:
  public:
    TArrow(TOBJList* owner, s32 _x, s32 _y, s32 _pat);
    void Move(); 
    void Hide();
    void Show();
    void SetPlace(s32 _x, s32 _y);
};

//���F�C�p�[�����C�N
class TVapor2 : public TOBJ {
  private:
    f32 ZoomRate;
    s32 PersonalR;
    s32 IncR;
    TVapor2* PrevSmoke;    //���O�ɐ������ꂽ��
    bool MotherIsDead;  //�����𐶐������{�̂�����
  public:
    TVapor2* NextSmoke;   //����ɐ������ꂽ��
    TVapor2(TOBJList* owner, s32 sx, s32 sy, TVapor2* _prev);
    void Move(); 
    void TellMomDead();
    void Die(); 
};

//���F�C�p�[�Ǘ��N���X
class TVaporControl : public TOBJ {
  private:
    vector<TVapor2*> PrevSmoke;   //�O��o�������I�u�W�F�N�g
  public:
    TVaporControl(TOBJList* owner, s32 _maxvapor);
    void MakeVapor(s32 _x, s32 _y, s32 _num);
    void Move(); 
    void Die(); 
    void Clear();
};

//�ڂ���܂�
class TSunShine : public TOBJ {
  private:
  public:
    TSunShine(TOBJList* owner);
    void Move(); 
};

//�ΉԂ���4
class TParticle4 : public TOBJ {
  private:
    s32 Life;  //����
    s32 Speed;
    s32 Rdeg;
    f32 CurrX,CurrY;
    f32 PrevX,PrevY;
  public:
    TParticle4(TOBJList* owner, s32 _x, s32 _y, s32 _R, s32 _speed, s32 _life, s32 _red, s32 _green, s32 _blue);
    void Move(); 
};

//�K���X�L���[�u�G�t�F�N�g�t�B���^
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

//���H�G�t�F�N�g�t�B���^
class TRainFilter : public TOBJ {
  private:
  public:
    TRainFilter(TOBJList* owner);
    void Move(); 
    void OnDeviceReset(); 
};

//�A���t�@�G�t�F�N�g�t�B���^
class TAlphaEffectFilter : public TOBJ {
  private:
  public:
    TAlphaEffectFilter(TOBJList* owner);
    void Move(); 
    void OnDeviceReset(); 
};

//���Z�G�t�F�N�g�t�B���^
class TAddEffectFilter : public TOBJ {
  private:
  public:
    TAddEffectFilter(TOBJList* owner);
    void Move(); 
    void OnDeviceReset(); 
};

//���
class TMotherShip : public TOBJ {
  private:
    bool SmokeON;
  public:
    TMotherShip(TOBJList* owner, s32 _x, s32 _y);
    void Move(); 
    void MakeSmoke();
};

//��͂���o�鉌
class TMotherShipSmoke : public TOBJ {
  private:
    f32 XSpeed, YSpeed;
  public:
    TMotherShipSmoke(TOBJList* owner, s32 _x, s32 _y, f32 _xspeed, f32 _yspeed, f32 _zoomX, f32 _zoomY);
    void Move(); 
};

//��͂���o�鉊
class TMotherShipFire : public TOBJ {
  private:
    f32 XSpeed,YSpeed;
  public:
    TMotherShipFire(TOBJList* owner, s32 _x, s32 _y, f32 _xspeed, f32 _yspeed, f32 _zoomX, f32 _zoomY);
    void Move(); 
};

//�C���C�U�[�i�f���Łj
class TDemoEraser : public TOBJ {
  private:
  public:
    TDemoEraser(TOBJList* owner);
    void Move(); 
    void ShowEraserParts(bool _show);
};

//�{�X���S���J�b�g�C��
class TBossDyingCutin : public TOBJ {
  private:
    s32 Life;
  public:
    TBossDyingCutin(TOBJList* owner, s32 _life);
    void Move(); 
};

//�{�X���
class TBossEyeLight : public TOBJ {
  private:
    s32 Life;
  public:
    TBossEyeLight(TOBJList* owner, s32 _x, s32 _y, s32 _life);             
    void Move(); 
};

//�Ήԁi�G�l���M�[�`���[�W�j
class TParticle_charge : public TOBJ {
  private:
    s32 Life;  //����
    s32 Speed;
    s32 Rdeg;
  public:
    TParticle_charge(TOBJList* owner, s32 _x, s32 _y, s32 _R, s32 _speed, s32 _life, s32 _red, s32 _green, s32 _blue);
    void Move(); 
};

//�C�ӂ̗̈�Ƀp�[�e�B�N�����΂�܂�
class TParticleSpreader : public TOBJ {
  private:
    s32 Life;  //����
    s32 Rdeg;
    s32 Width;
    s32 Red,Green,Blue;
  public:
    TParticleSpreader(TOBJList* owner, s32 _x, s32 _y, s32 _width, s32 _R, s32 _life, s32 _red, s32 _green, s32 _blue);
    void Move(); 
};

//�ΉԂ���5
class TParticle5 : public TOBJ {
  private:
    s32 Gain;  
    s32 Speed;
  public:
    TParticle5(TOBJList* owner, s32 _x, s32 _y, s32 _r, s32 _speed, s32 _life, s32 _red, s32 _green, s32 _blue, f32 _size);
    void Move(); 
};

//���̗ւ���
class TLightCircle : public TOBJ {
  private:
    f32 ZoomRate;
  public:
    TLightCircle(TOBJList* owner, s32 _x, s32 _y, s32 _red, s32 _green, s32 _blue, f32 _size);
    void Move(); 
};

//��
class TBlood : public TOBJ {
  private:
    s32 Rdeg;
  public:
    TBlood(TOBJList* owner, s32 _x, s32 _y, s32 _R);
    void Move(); 
};

//�I�[��
class TAura : public TOBJ {
  private:
    s32 RSpeed;
  public:
    TAura(TOBJList* owner, s32 _x, s32 _y);
    void Move(); 
};

//���[�v���o(2D)
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

//���[�v���o(2D) �p�[�c
class TWarp2DPart : public TOBJ {
  private:
    s32 ActFlg,ActCount;
    f32 Zoomrate;
  public:
    TWarp2DPart(TOBJList* owner, s32 _x, s32 _y, f32 _Zoom, s32 _r);
    void Move(); 
};


//���[�v���o(2D) �ւ���
class TWarp2DCircle : public TOBJ {
  private:
  public:
    TWarp2DCircle(TOBJList* owner, s32 _x, s32 _y);
    void Move(); 
};

//�V���b�g���̉Ή� (��)
class TShotFireLarge : public TOBJ {
  public:
    TShotFireLarge(TOBJList* owner, s32 _x, s32 _y, f32 _zoom);
    void Move(); 
};

//�V���b�g���̉Ή� (����)
class TShotFireWithSmoke : public TOBJ {
  private:
    s32 Rdeg;
    f32 ZoomRate;
  public:
    TShotFireWithSmoke(TOBJList* owner, s32 _x, s32 _y, s32 _r, f32 _zoom);
    void Move(); 
};

//�׃r�[�����˃G�t�F�N�g
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

//�{�X����
class TBossExpWind : public TDemoEraser {
  private:
  public:
    TBossExpWind(TOBJList* owner);
    void Move(); 
};

//�e
class TBossShadow : public TOBJ {
  private:
    s32 MaxHeight,MinHeight;
    TJoint* Parent;
    s32 DX;
  public:
    TBossShadow(TOBJList* owner, TJoint* _parent, s32 _x, s32 _y, s32 _high, s32 _low);
    void Move(); 
};

//�K���L
class TDebris : public TOBJ {
  private:
    s32 Rdeg;//��]�p
    s32 Speed;//�X�s�[�h
    s32 IncR;
  public:
    TDebris(TOBJList* owner, s32 sx, s32 sy, s32 R, s32 S, u16 _flg);
    void Move(); 
};

//�K���L�܂��U�炵
class TSplashDebris : public TOBJ {
  private:
  public:
    TSplashDebris(TOBJList* owner, s32 sx, s32 sy);
    void Move(); 
};

//�G���b�N�I���}�[�J�[
class TEnemyLockMarker : public TOBJ {
  private:
    f32 ZoomRate;
    f32 CurrentZoom;
  public:
    TEnemyLockMarker(TOBJList* owner, s32 _x, s32 _y, f32 _zoom);
    void Move(); 
};
