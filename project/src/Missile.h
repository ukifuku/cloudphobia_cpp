#pragma once

using namespace std;

#include "types.h"
#include "TOBJ.h"
#include <vector>

class TSmoke;

//�~�T�C�����ˊǗ��N���X
class TMakeMissile : public TOBJ {
  private:
    s32 SetNum;   //���Z�b�g�ڂ̃~�T�C����
	s32 ShotInt;  //�V���b�g�Ԋu
    s32 TotalLock;
    s32 MissileNum;
  public:
    TMakeMissile(TOBJList* owner, s32 _setnum, s32 _locknum);
    void Move(); 
};

//�~�T�C�����ˊǗ��N���X2
class TMakeMissile2 : public TOBJ {
  private:
    s32 SetNum;   //���Z�b�g�ڂ̃~�T�C����
    s32 ShotInt;  //�V���b�g�Ԋu
    s32 MissileNum;
  public:
    TMakeMissile2(TOBJList* owner, s32 _setnum);
    void Move(); 
};

//��
class TSmoke : public TOBJ {
  private:
    s32 Speed;
    f32 ZoomRate;
    s32 PersonalR;
    s32 IncR;
    TSmoke* PrevSmoke;    //���O�ɐ������ꂽ��
    bool MotherIsDead;  //�����𐶐������~�T�C���{�̂�����
    s32 OriginalBGSpeed;  //���������̔w�i�X�s�[�h
  public:
    s32 Rdeg;
    TSmoke* NextSmoke;   //����ɐ������ꂽ��
    TSmoke(TOBJList* owner, s32 sx, s32 sy, s32 _R, TSmoke* _prev);
    void Move(); 
    void TellMomDead();    //�~�T�C���{�̂����񂾂��Ƃ����ׂ̉��ɂ��m�点
    void Die(); 
};

//�~�T�C���{��
class TMissile : public TOBJ {
  private:
    s32 YMove; //   Y�����̓���
    s32 Rdeg;
    s32 SetNum; //���Z�b�g�ڂ̃~�T�C����
    s32 PrevR;   //�O�t���[���ł̊p�x
    TSmoke* PrevSmoke;   //�O��o�������I�u�W�F�N�g
  public:
    TMissile(TOBJList* owner, s32 sx, s32 sy, s32 _setnum, s32 _misnum);
    void Move(); 
    void Die(); 
    void OnDamaged(TOBJ* target); 
};

//��2
class TSmoke2 : public TOBJ {
  private:
    s32 Rdeg;
    s32 Speed;
    TOBJ* PrevObj;
    s32 MissileID;
    f32 ZoomRate;
  public:
    TSmoke2(TOBJList* owner, s32 sx, s32 sy, s32 _R, TOBJ* _PrevObj, s32 _misID);
    void Move(); 
};

//�o�b�N�t�@�C�A
class TMisBFire : public TOBJ {
  private:
    s32 Rdeg;
  public:
    TMisBFire(TOBJList* owner, s32 sx, s32 sy, s32 _R);
    void Move(); 
};
