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
    s32 ShootFlg;             //����t���O(-1:���g�p 0:�m�[�}���V���b�g)
	bool MisOK;                //�~�T�C���g�p��
	s32 BlinkTime,LastBlink;  //�_�Ŏ��ԁA�Ō�ɓ_�ł�������
	s32 LastMis;              //�Ō�Ƀ~�T�C��������������
	bool MissileMuteki;        //�~�T�C�����G��
	s32 BoostTime;            //�u�[�X�g����
	s32 PrevX,PrevY;          //1�t���[���O�̍��W
	s32 NokeTime;             //�̂����蓮��J�n����
	s32 CrushR;               //�Ԃ���������Ƃ̊p�x
	s32 BoostR;               //�����������
	s32 ActFlg[100];     //�s���t���O
	s32 ActCount[100];   //�s���J�E���^
	s32 ShowSpr;

	s32 SlideX,SlideY;
	bool AcceptMoveBody;       //�ړ����̃A�j���[�V�����������邩
	TVaporControl* Vapor;
	virtual void MakeVapor(s32 _num, s32 _sprite, s32 _x, s32 _y);
public:
	s32 Speed;                //���@�X�s�[�h
	f32 ZoomRate;            //�g�嗦
	TJikiHitArea* HitArea;
	TJikiBase(TOBJList* owner, s32 _x, s32 _y);
	~TJikiBase();
	void Move(); 
	virtual void Input();                                 //�L�[����
	void OnDamaged(TOBJ* target);
	void Die();
	virtual void Slide(f32 _x, f32 _y);                   //�ړ����̏���
	virtual void BodyControl(s32 Bo, s32 L2, s32 L3, s32 R2, s32 R3);   //���Ԑڐ���
	virtual void BFControl(s32 _R, f32 _zoom);   //�o�b�N�p�b�N����
	virtual void HeroJet();  
	virtual void Nokezori();
	virtual void MonoRotate(s32 _Sprnum, s32 _dist, s32 _speed);
	virtual void StartEvent();                      //�o�����C�x���g
	virtual void Flash(bool _state);
	virtual void BlinkControl();    //�_��
	virtual void BoostSoon(bool _boost);    //�����Ȃ�u�[�X�g��Ԃɂ���(�f���p)
	virtual void BodyControlSoon(s32 Bo, s32 L2, s32 L3, s32 R2, s32 R3);   //���Ԑڐ���i�A�j���[�V���������j
	virtual void ResetBeforeStart();
	virtual void BoostByOther(bool _boost);
	virtual void Hide();
};

//���@
class TJiki : public TJikiBase {
protected:
public:
    TJiki(TOBJList* owner, s32 _x, s32 _y);
    ~TJiki();
    void Move(); 
    void Input();                                 //�L�[����
    void MakeShot();                              //�e������
    void OnDamaged(TOBJ* target);
    void Nokezori();
    void Flash(bool _state);
    void BoostSoon(bool _boost);    //�����Ȃ�u�[�X�g��Ԃɂ���(�f���p)
    void DoStartEvent();
    void ResetBeforeStart();
    void Hide();
};

//�e
class TShot : public TOBJ{
public:
    TShot(TOBJList* owner, s32 sx, s32 sy, s32 R, f32 _zoom);
    void Move(); 
    void OnDamaged(TOBJ* target); 
};

//���ˉ�
class TBFire : public TOBJ{
private:
    s32 Rdeg;
    s32 DisX,DisY;//���@���S����̋���
    f32 ZoomRate;
    TOBJ* OwnerOBJ;
public:
    TBFire(TOBJList* owner, s32 sx, s32 sy, s32 _R, s32 _LV, f32 _zoom, TOBJ* _obj);
    void Move(); 
};

//���i�ԍ�
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
static const s32       HSheath=16;   //��
static const s32       HBlade=17;    //��
static const s32       HFlag=16;    //�t���b�O
   

//�s���ԍ�
static const s32       HAStart = 0;
static const s32       HANokezori = 1;
static const s32       HASlash = 2;

static const s32     BodyTable[] = {256,128,128,3840,3840,3968,3968,256,0};
static const s32     LLegTable2[] = {400,3052,3052,3188,3188,3492,3492,200,3396};
static const s32     LLegTable3[] = {88,4060,4060,3132,3132,3732,3732,88,100};
static const s32     RLegTable2[] = {500,3472,3472,3368,3368,3728,3728,200,3992};
static const s32     RLegTable3[] = {88,4080,4080,3272,3272,3576,3576,88,3992};
