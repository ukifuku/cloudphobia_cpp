#pragma once

using namespace std;

#include "types.h"
#include "TOBJ.h"
#include "Hero.h"
#include <vector>

class TVaporControl;
class TJikiHitArea;


//���@
class TJiki2 : public TJikiBase {
protected	:
    bool AcceptMoveBody;       //�ړ����̃A�j���[�V�����������邩
    TOBJ* BladeTrace;          //���̋O��
    bool NextSlash;          //�A���U�����s����
    bool ForceBladeOff;    //�����I�ɏ�Ɏ��߂�
    s32 CycloBladeR;      //�������������̘r�̊p�x
	s32 NowSpr;           //���݂̃X�v���C�g�ԍ��i�f�o�b�O�p�j
    void OnSlash();        //�G��؂����Ƃ��̃G�t�F�N�g����
    void Input();                                 //�L�[����
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
    virtual void BoostSoon(bool _boost);    //�����Ȃ�u�[�X�g��Ԃɂ���(�f���p)
    void ResetBeforeStart();
};

