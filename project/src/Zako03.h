#pragma once

using namespace std;

#include "types.h"
#include "TOBJ.h"
#include <vector>

class TEnemyGauge2;
class TVaporControl;
class TZBeam2;

// �G��2-1
class TZako2_01 : public TOBJ {
  protected:
    bool Buzzed;//Buzz�������ǂ���
    s32 LastBuzz;//�Ō��Buzz��������
    s32 Rdeg;    //�p�x
    s32 Speed;                //���@�X�s�[�h
    s32 Pattern; //�s���p�^�[��
    s32 GaugeTime;
    s32 TotalFlight;//��s����
    u8 OriginColor;
    s32 LastFlash;
    TEnemyGauge2* HPGauge;
    s32 DestX,DestY;
    void Action();
    void MakeShot(s32 _pat);
    void MakeMissile();
  public:
    TZako2_01(TOBJList* owner, s32 _x, s32 _y, s32 _destx, s32 _desty, s32 _R, s32 _S, s32 _pat, bool _red );
    void Move(); 
    void OnDamaged(TOBJ* target);
    void Die();
    void OnBroken();
};

// �G��2-2�i�o�������j
class TZako2_02 : public TOBJ {
  protected:
    bool Buzzed;//Buzz�������ǂ���
    s32 LastBuzz;//�Ō��Buzz��������
    s32 Rdeg;    //�p�x
    s32 Speed;                //���@�X�s�[�h
    s32 Pattern; //�s���p�^�[��
    TEnemyGauge2* HPGauge;
    s32 GaugeTime;
    s32 TotalFlight;//��s����
    u8 OriginColor;
    s32 LastFlash;
    s32 ActFlg[100];     //�s���t���O
    s32 ActCount[100];   //�s���J�E���^
    s32 DestX,DestY;
    f32 BSpeed;
    s32 RetreatTime;
    void Action();
    void Accel();
    void MakeShot();
  public:
    TZako2_02(TOBJList* owner, s32 _x, s32 _y, s32 dx, s32 dy, s32 _R, s32 _S, s32 _pat, bool _red );
    void Move(); 
    void OnDamaged(TOBJ* target);
    void Die();
    void OnBroken();
  
    //�o���T�����񕔕i�ԍ�
	enum{
		Z2_2Body       = 0,
		Z2_2Rotor      = 1,
		Z2_2Wing1      = 2,
		Z2_2Wing2      = 3,
		Z2_2Wing3      = 4,
	};

	enum{ KACTION_S2Z2_ACT1  = 0 };
};

// �G��2-3�i��]�o�[�j
class TZako2_03 : public TOBJ {
  protected:
    bool Buzzed;//Buzz�������ǂ���
    s32 LastBuzz;//�Ō��Buzz��������
    s32 Rdeg;    //�p�x
    s32 Pattern; //�s���p�^�[��
    s32 GaugeTime;
    s32 TotalFlight;//��s����
    u8 OriginColor;
    s32 LastFlash;
    void MakeBar(s32 _angle);
  public:
    s32 Speed;                //���@�X�s�[�h
    TZako2_03(TOBJList* owner, s32 _x, s32 _y, s32 _R, s32 _S, s32 _pat, bool _red );
    void Move(); 
    void OnDamaged(TOBJ* target);
    void Die();
};

// �G��2-4�i�X�p�C�_�[�j
class TZako2_04 : public TOBJ {
  protected:
    bool Buzzed;//Buzz�������ǂ���
    s32 LastBuzz;//�Ō��Buzz��������
    s32 Rdeg;    //�p�x
    s32 Speed;                //���@�X�s�[�h
    s32 Pattern; //�s���p�^�[��
    s32 GaugeTime;
    s32 TotalFlight;//��s����
    u8 OriginColor;
    s32 LastFlash;
    TEnemyGauge2* HPGauge;
    s32 EnemyDeg; //�G�Ǝ����̑��Ίp�x
    s32 CurrentMotion;
    f32 BSpeed;
	s32 ActFlg[100];     //�s���t���O
	s32 ActCount[100];   //�s���J�E���^
    f32 IniDis;       //�ړI�n�܂ł̋���
    s32 DestDeg;     //�ړI�n�܂ł̊p�x
    s32 DestX,DestY;
    s32 HomePosX;
    void MakeShot();
    void Accel();
    void Transfer();
    void Appear();
    void Escape();
    void Action();
    void ChargeMotion();
    void ShotMotion();
  public:
    TZako2_04(TOBJList* owner, s32 _x, s32 _y, s32 _pat, bool _red );
    void Move(); 
    void OnDamaged(TOBJ* target);
    void Die();
    void OnBroken(); 
    
	//�s���ԍ�
	enum{
		KACTION_S2Z4_DEFAULT  = 0,
		KACTION_S2Z4_TRANSFER = 1,
		KACTION_S2Z4_APPEAR = 2,
		KACTION_S2Z4_CHARGE = 3,
		KACTION_S2Z4_SHOT = 4,
	};
};

// ������
class TSubmarine : public TOBJ {
  protected:

  public:
    TSubmarine(TOBJList* owner, s32 _x, s32 _y);
    void Move(); 
};

// �G��2-5�i�r�[���C�j
class TZako2_05 : public TOBJ {
  protected:
    bool Buzzed;//Buzz�������ǂ���
    s32 LastBuzz;//�Ō��Buzz��������
    s32 Rdeg;    //�p�x
    s32 Speed;                //���@�X�s�[�h
    s32 Pattern; //�s���p�^�[��
    s32 GaugeTime;
    s32 TotalFlight;//��s����
    u8 OriginColor;
    s32 LastFlash;
    TEnemyGauge2* HPGauge;
    f32 BSpeed;
	s32 ActFlg[100];     //�s���t���O
	s32 ActCount[100];   //�s���J�E���^
    s32 DestX,DestY;
    void MakeShot();
    void Accel();
    void Action();
    void BeamMotion();
    void MakeMissile();
    void BeamMotion2();
  public:
    TZako2_05(TOBJList* owner, s32 _x, s32 _y, s32 _dx, s32 _dy, s32 _pat, bool _red );
    void Move(); 
    void OnDamaged(TOBJ* target);
    void Die();
    void OnBroken(); 

    //�s���ԍ�
	enum{
		KACTION_S2Z5_BEAM  = 0,
		KACTION_S2Z5_ACT1  = 1,
	};
};

// �G��2-6�i���^�@�j
class TZako2_06 : public TOBJ {
  protected:
    bool Buzzed;//Buzz�������ǂ���
    s32 LastBuzz;//�Ō��Buzz��������
    s32 Rdeg;    //�p�x
    s32 Speed;                //���@�X�s�[�h
    s32 Pattern; //�s���p�^�[��
    s32 TotalFlight;//��s����
    s32 DiffDistance;
    u8 OriginColor;
    s32 LastFlash;
    f32 BSpeed;
    s32 RSpeed;
    s32 DisappearPos;
	s32 ActFlg[100];     //�s���t���O
	s32 ActCount[100];   //�s���J�E���^
    s32 ShotCount;
    void MakeShot();
    void Action();
  public:
    TZako2_06(TOBJList* owner, s32 _x, s32 _y, s32 _r, s32 _s, s32 _pat, bool _red );
    void Move(); 
    void OnDamaged(TOBJ* target);
    void OnBroken(); 

    //�s���ԍ�
	enum{
		KACTION_S2Z6_ACT1  = 0
	};
};

// ��]�o�[�f��
class TRevBarBasic : public TOBJ {
  protected:
    s32 Rdeg;    //�p�x
    f32 Speed;                //���@�X�s�[�h
    s32 Life;
    TOBJ* Parent;
    s32 PrevR;
    f32 ParentSpeed;
  public:
    TRevBarBasic(TOBJList* owner, TOBJ* _parent, s32 _x, s32 _y, s32 _R, f32 _S, f32 _parentspeed, s32 _L);
    void Move(); 
};

// �G���W�F�l���[�^2-1
class TZ02_1Gene : public TOBJ {
  private:
    s32 Pattern;
    s32 Count;
    s32 TotalFlight;
    s32 OriginX;
  public:
    TZ02_1Gene(TOBJList* owner, s32 _x, s32 _y, s32 _pat);
    void Move(); 
};

// �G���W�F�l���[�^2-5
class TZ02_5Gene : public TOBJ {  
  private:
    s32 Pattern;
    s32 Count;
    s32 TotalFlight;
    s32 CommanderNum;
    s32 diffX;
  public:
    TZ02_5Gene(TOBJList* owner, s32 _x, s32 _y, s32 _pat, s32 _com);
    void Move(); 
};

// �G���W�F�l���[�^2-6
class TZ02_6Gene : public TOBJ {  
  private:
    s32 Pattern;
    s32 Count;
    s32 TotalFlight;
    s32 diffX;
  public:
    TZ02_6Gene(TOBJList* owner, s32 _x, s32 _y, s32 _pat);
    void Move(); 
};

// �G��3-1�i��^�@�j
class TZako3_01 : public TOBJ {
  protected:
    bool Buzzed;//Buzz�������ǂ���
    s32 LastBuzz;//�Ō��Buzz��������
    s32 Speed;                //���@�X�s�[�h
    s32 Pattern; //�s���p�^�[��
    s32 TotalFlight;//��s����
    u8 OriginColor;
    s32 LastFlash;
    f32 BSpeed;
    TEnemyGauge2* HPGauge;
    s32 GaugeTime;
    s32 KnockBackSpeed;
    s32 DestX,DestY;
	s32 ActFlg[100];     //�s���t���O
	s32 ActCount[100];   //�s���J�E���^
    s32 RoopCount;
    void MakeShot(s32 _x, s32 _y, s32 _r);
    void MakeHoming(s32 _x, s32 _y, s32 _r);
    void Action();
    void Accel();
    void KnockBack(s32 _impact);
    void KnockBackAction();
  public:
    TZako3_01(TOBJList* owner, s32 _x, s32 _y, s32 dx, s32 dy, s32 _pat, bool _red );
    void Move(); 
    void OnDamaged(TOBJ* target);
    void OnBroken(); 
    void Die(); 

    //�s���ԍ�
    enum{
		KACTION_ACT1  = 0,
		KACTION_KNOCKBACK = 1,
	};
};

// �G��2-5-2�i�r�[���C_5�{�X�j
class TZako2_05_2 : public TOBJ {
  protected:
    bool Buzzed;//Buzz�������ǂ���
    s32 LastBuzz;//�Ō��Buzz��������
    s32 Rdeg;    //�p�x
    s32 Pattern; //�s���p�^�[��
    s32 GaugeTime;
    s32 TotalFlight;//��s����
    u8 OriginColor;
    s32 LastFlash;
    TEnemyGauge2* HPGauge;
    f32 BSpeed;
    s32 ActFlg;     //�s���t���O
    s32 ActCount;   //�s���J�E���^
    s32 DestX,DestY;
    TJoint* Parent;
    s32 RSpeed;
    s32 DistY;
    s32 CurrentDist;
    TZBeam2* Beam;
    void Action();
  public:
    TZako2_05_2(TOBJList* owner, TJoint* _parent, s32 _x, s32 _y, s32 _r, s32 _distance, s32 _Rspeed, s32 _pat, bool _red );
    void Move(); 
    void OnDamaged(TOBJ* target);
    void Die(); 
    void OnBroken();
};

// �G���W�F�l���[�^2-3
class TZ02_3Gene : public TOBJ {
  private:
    s32 Pattern;
    s32 Count;
    s32 TotalFlight;
    s32 diffX;
    s32 Interval;
  public:
    TZ02_3Gene(TOBJList* owner, s32 _pat);
    void Move(); 
};

