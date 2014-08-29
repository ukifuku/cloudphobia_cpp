#pragma once

using namespace std;

#include "types.h"
#include "TOBJ.h"
#include <vector>

class TEnemyGauge2;
class TVaporControl;

// �W�F�l���[�^
class TTransGene : public TOBJ {
private:
	s32 Count;
public:
	TTransGene(TOBJList* owner);
	void Move(); 
};

//�ό`����
class TTransformer : public TOBJ {
private:
	bool Buzzed;//Buzz�������ǂ���
	s32 LastBuzz;//�Ō��Buzz��������
	s32 Pattern; //�s���p�^�[��
	TEnemyGauge2* HPGauge;
	s32 GaugeTime;
	s32 TotalFlight;//��s����
	s32 ActFlg;     //�s���t���O
	s32 ActCount;   //�s���J�E���^
	s32 LastFlash;
	bool BeamON;     //�r�[���g�p��
	s32 BeamCount;  //�r�[�����˓���p�J�E���^
	s32 GunR;   //�r�[���C�p�x
	s32 EnemyDeg; //�G�Ǝ����̑��Ίp�x
	bool Boost;    //�u�[�X�g
	s32 BoostCount;
	f32 BSpeed;
	s32 NowSpr;
	bool TransState; //T:�퓬�@�@F:�l�^
	bool TransForm;
	s32 TransCount;//�ό`�J�E���^
	s32 XCycle,YCycle;
	f32 XSpeed,YSpeed;
	s32 Xdist,Ydist; //�ړI�n���W
	f32 IniDis;       //�ړI�n�܂ł̋���(�w�蒼��)
	f32 CurDis;       //�ړI�n�܂ł̌��݂̋���
	s32 DestDeg;     //�ړI�n�܂ł̊p�x
	s32 TransTiming; //�ό`����܂ł̎���
	s32 PrevX,PrevY;
	s32 Retreat; //�ދp���鋗��
	s32 TimCount;    //�^�C�~���O���J�E���^
	f32 AccelElement;
	s32 DecelElement;
	TPoint HPosList[4];
	TPoint PPosList[4]; //�ړ��惊�X�g
	TVaporControl* Vapor;

	//�ό`���񕔕i�ԍ�
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

//�r�[���f��
class TTransBeamPart : public TOBJ {
private:
	s32 Rdeg;
	s32 Speed;
	s32 Life;
public:
	TTransBeamPart(TOBJList* owner, s32 sx, s32 sy, s32 R, s32 S, s32 L, f32 _zoom);
	void Move(); 
};

//�r�[���{��
class TTransBeamCore : public TOBJ {
private:
	s32 Rdeg;
	bool Buzzed;//Buzz�������ǂ���
	s32 LastBuzz;//�Ō��Buzz��������
	s32 Pattern;
public:
	TTransBeamCore(TOBJList* owner, s32 sx, s32 sy, s32 R, s32 _pat);
	void Move(); 
	void OnDamaged(TOBJ* target);
};

//���ˉ�
class TTransBFire : public TOBJ {
public:
	TTransBFire(TOBJList* owner, s32 sx, s32 sy);
	void Move(); 
};

