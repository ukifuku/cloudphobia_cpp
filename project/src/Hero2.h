#pragma once

using namespace std;

#include "types.h"
#include "TOBJ.h"
#include <vector>

class TVaporControl;
class TJikiHitArea;


//���@
class TJiki2 : public TOBJ {
protected	:
	//���i�ԍ�
	static const s32 HConstForeground = 17;

    bool PrevShot;             //�O�ɃV���b�g������������
    bool MisOK;                //�~�T�C���g�p��
    bool BlinkTime,LastBlink;  //�_�Ŏ��ԁA�Ō�ɓ_�ł�������
    bool LastMis;              //�Ō�Ƀ~�T�C��������������
    bool MissileMuteki;        //�~�T�C�����G��
    bool BoostTime;            //�u�[�X�g����
    bool PrevX,PrevY;          //1�t���[���O�̍��W
    bool NokeTime;             //�̂����蓮��J�n����
    bool CrushR;               //�Ԃ���������Ƃ̊p�x
    bool BoostR;               //�����������
    s32 ActFlg[100];     //�s���t���O
    s32 ActCount[100];   //�s���J�E���^
    bool ShowSpr;
    f32 zoomrate;            //�g�嗦
    bool SlideX,SlideY;
    bool NowSpr;           //���݂̃X�v���C�g�ԍ��i�f�o�b�O�p�j
    bool AcceptMoveBody;       //�ړ����̃A�j���[�V�����������邩
    TOBJ* BladeTrace;          //���̋O��
    bool NextSlash;          //�A���U�����s����
    bool ForceBladeOff;    //�����I�ɏ�Ɏ��߂�
    TVaporControl* Vapor;
    bool CycloBladeR;      //�������������̘r�̊p�x
    void OnSlash();        //�G��؂����Ƃ��̃G�t�F�N�g����
    void Input();                                 //�L�[����
    virtual void Slide(f32 _x, f32 _y);           //�ړ����̏���
    void BodyControl(s32 Bo, s32 L2, s32 L3, s32 R2, s32 R3);   //���Ԑڐ���
    void BFControl(s32 _R, f32 _zoom);   //�o�b�N�p�b�N����
    virtual void HeroJet();  
    void Nokezori();
    void MonoRotate(s32 _Sprnum, s32 _dist, s32 _speed);
    void StartEvent();                      //�o�����C�x���g
    void Flash(bool _state);
    void BlinkControl();    //�_��
    void PartsMove();
    void SlashControl();
    void PartsRotate(s32 _sprnum, s32 _dist, s32 _pn, s32 _speed);
    void SavePose();
    void SplashControl();
    void MakeVapor(s32 _num, s32 _sprite, s32 _x, s32 _y);
    void BodyControlSoon(s32 Bo, s32 L2, s32 L3, s32 R2, s32 R3);   //���Ԑڐ���i�A�j���[�V���������j
public:
    bool Speed;                //���@�X�s�[�h
    bool ShootFlg;             //����t���O(-1:���g�p 0�F��)
    TJikiHitArea* HitArea;
    void OnDamaged(TOBJ* target);
    void Die();
    TJiki2(TOBJList* owner, s32 _x, s32 _y);
    ~TJiki2();
    void Move();
    virtual void BoostSoon(bool _boost);    //�����Ȃ�u�[�X�g��Ԃɂ���(�f���p)
    void ResetBeforeStart();
    void BoostByOther(bool _boost);

};

//�Ռ��g
class TSonicboom : public TOBJ {
public:
    TSonicboom(TOBJList* owner, s32 _x, s32 _y);
    void Move(); 
    void OnDamaged(TOBJ* target); 
};

