#pragma once

using namespace std;

#include "types.h"
#include "TOBJ.h"
#include <vector>

class TBossGauge2;
class TEnemyGauge2;
class TEnemyBladeTrace;
class TEnemyBladeTrace2;
class TFriend;
class TOBJList;
class TBoss2BeamTrace;
class TJoint;
class TBitLight;
class TDangerAlarm;
class TBossShadow;

//�{�X��{�N���X
class TBossBase : public TOBJ {
protected:
	bool Buzzed;//Buzz�������ǂ���
	s32 LastBuzz;//�Ō��Buzz��������
	TBossGauge2* HPGauge;
	s32 GaugeTime;
	s32 TotalFlight;//��s����
	s32 ActFlg[100];     //�s���t���O
	s32 ActCount[100];   //�s���J�E���^
	s32 LastFlash;
	s32 EnemyDeg; //�G�Ǝ����̑��Ίp�x
	f32 BSpeed;
	f32 IniDis;       //�ړI�n�܂ł̋���
	s32 DestDeg;     //�ړI�n�܂ł̊p�x
	s32 DestX,DestY;
	u8 OriginColor;
	s32 ShotCount,ShotCount_Spread;
	s32 Motion_Default, Motion_Foward, Motion_Back;
	s32 ConstForeground;
	s32 KnockBackVel;
	virtual void Transfer(s32 _x, s32 _y, s32 _speed);
	virtual void Transfer_NoMotion(s32 _x, s32 _y, s32 _speed);
	virtual void Accel();      
	virtual void SetNextState(s32 _current);
	virtual void KeepPosition(s32 _x, s32 _y);
	virtual void KnockBack();
public:
	TBossBase(TOBJList* owner);
	void Init(s32 _x, s32 _y, s32 _hp, s32 _def);
	virtual ~TBossBase();
	void Move(); 
	void OnDamaged(TOBJ* target);
	void Die(); 
	void OnBroken();  

	//�s���ԍ�
	enum { KACTION_TRANSFER = 0};
};


//�X�e�[�W�Q���{�X
class TStage2Boss1 : public TOBJ {
private:
	bool Buzzed;//Buzz�������ǂ���
	s32 LastBuzz;//�Ō��Buzz��������
	TBossGauge2* HPGauge;
	s32 GaugeTime;
	s32 TotalFlight;//��s����
	s32 ActFlg[100];     //�s���t���O
	s32 ActCount[100];   //�s���J�E���^
	s32 LastFlash;
	s32 EnemyDeg; //�G�Ǝ����̑��Ίp�x
	f32 BSpeed;
	f32 IniDis;       //�ړI�n�܂ł̋���
	s32 DestDeg;     //�ړI�n�܂ł̊p�x
	s32 DestX,DestY;
	u8 OriginColor;
	s32 OriginHeadR;    //���p�[�c�̏����p�x
	TEnemyBladeTrace* BladeTrace;
	TFriend* Victim;
	s32 EyeLaserCount;
	void MakeHoming();
	void Transfer(s32 _x, s32 _y, s32 _speed);
	void Transfer_NoMotion(s32 _x, s32 _y, s32 _speed);
	void Accel();
	void Fall();
	void MakeShot(s32 _pat);
	void AppearEvent();
	void MakeBeam(s32 _r);
	void SpreadBeam();
	void HomingMotion();
	void BladeMotion();
	void EyeLaserMotion();
	void MakeEyeLaser();
	void Search();
	void MakeChargeEffect(s32 _headR);
	void MakeHoming2();
	void MakeEyeLight();
	void BerserkMotion();
	void MakeBerserkBeam();
	void HideHead();
	void SetNextState(s32 _current);
	void KeepPosition(s32 _x, s32 _y);
	void RetreatMotion();
	void CheckRetreat();
public:
	TStage2Boss1(TOBJList* owner, s32 _x, s32 _y);
	~TStage2Boss1(); 
	void Move(); 
	void OnDamaged(TOBJ* target);
	void Die(); 
	void OnBroken(); 

	//���i�ԍ�
	enum {
		BHip        = 0,
		BHead       = 1,
		BNeck       = 2,
		BChest      = 3,
		BWaist1     = 4,
		BWaist2     = 5,
		BWaist3     = 6,
		BWaist4     = 7,
		BLShoulder  = 8,
		BRShoulder  = 9,
		BLArm       =10,
		BRArm       =11,
		BBlade      =12,
		BLThigh     =13,
		BRThigh     =14,
		BLLeg       =15,
		BRLeg       =16,
		BLegCover   =17,
		BWing1      =18,
		BWing2      =19,
		BWing3      =20,

		BConstForeground=21,
	};

	//�s���ԍ�
	enum{
		KACTION_S2B1_APPEAR  = 0,
		KACTION_S2B1_TRANSFER = 1,
		KACTION_S2B1_BEAM = 2,
		KACTION_S2B1_HOMING = 3,
		KACTION_S2B1_BLADE = 4,
		KACTION_S2B1_EYELASER = 5,
		KACTION_S2B1_BERSERK = 6,
		KACTION_S2B1_FALL = 7,
		KACTION_S2B1_RETREAT = 8,

		KFIXEDANGLE_EYELASER  = 261,    //EyeLaser�̔��ˊp�x�␳
	};
};


//�{�X�Q�r�[���{��
class TBoss2BeamCore : public TOBJ {
private:
	bool Buzzed;//Buzz�������ǂ���
	s32 LastBuzz;//�Ō��Buzz��������
	s32 Rdeg;
	s32 HomingLevel;    //�ǔ��̋Ȃ���₷��
	TBoss2BeamTrace* Trace;   //�r�[���̋O��
	s32 DestX,DestY;
	s32 ArriveCount;
	s32 PrevX, PrevY;
public:
	TBoss2BeamCore(TOBJList* owner);
	~TBoss2BeamCore();
	void Move(); 
	void Die(); 
	void OnDamaged(TOBJ* target); 
	void CalcTrajectory(s32 sx, s32 sy, s32 _r);
	void Show(bool _show);
	void SetTarget(s32 _x, s32 _y);
	void SetTargetR(s32 _x, s32 _y);
	void Place(s32 _x, s32 _y);
};

//�X�e�[�W�Q�{�X2
class TStage2Boss2 : public TOBJ {
private:
	bool Buzzed;//Buzz�������ǂ���
	s32 LastBuzz;//�Ō��Buzz��������
	TBossGauge2* HPGauge;
	s32 GaugeTime;
	s32 TotalFlight;//��s����
	s32 ActFlg[100];     //�s���t���O
	s32 ActCount[100];   //�s���J�E���^
	s32 LastFlash;
	s32 EnemyDeg; //�G�Ǝ����̑��Ίp�x
	f32 BSpeed;
	f32 IniDis;       //�ړI�n�܂ł̋���
	s32 DestDeg;     //�ړI�n�܂ł̊p�x
	s32 DestX,DestY;
	u8 OriginColor;
	s32 ShotCount;
	TBoss2BeamCore* TailBeam[8];
	s32 ServantDead;
	void AppearEvent();
	void Transfer(s32 _x, s32 _y, s32 _speed);
	void Accel();
	void MakeShot(s32 _pat);
	void MakeTail();
	void BeamMotion();
	void SetNextState(s32 _current);
	void EscapeMotion();
	void Fall();
	void CheckServantDead();
	void StartingDemo();
	void WarpMotion();
	void KeepPosition(s32 _x, s32 _y);
	void SlideTail();
public:
	TStage2Boss2(TOBJList* owner, s32 _x, s32 _y);
	~TStage2Boss2();
	void Move(); 
	void OnDamaged(TOBJ* target);
	void Die(); 
	void OnBroken();  
	void TellImDead();

	//���i�ԍ�
	enum{ BBody        = 0 ,
		BConstForeground = 1,
	};
	//�s���ԍ�
	enum{
		KACTION_S2B2_APPEAR  = 0,
		KACTION_S2B2_TRANSFER = 1,
		KACTION_S2B2_BEAM = 2,
		KACTION_S2B2_ESCAPE = 3,
		KACTION_S2B2_FALL = 4,
		KACTION_S2B2_STARTDEMO = 5,
		KACTION_S2B2_WARP = 6
	};
};

//�X�e�[�W�Q�{�X��qBWS
class TStage2GuardBWS : public TOBJ {
private:
	TJoint* OwnerSprite;     //���̃X�v���C�g�̑��֐߂ɑg�ݍ��ޏꍇ�́A�e�X�v���C�g
	bool Purged[3];
	f32 YSpeed[3];
public:
	TStage2GuardBWS(TOBJList* owner, s32 _x, s32 _y);
	~TStage2GuardBWS(); 
	void Move(); 
	void Die(); 
	void SetOwnerSprite(TJoint* _spr);
	void Purge(s32 _id);
};

//�X�e�[�W�Q�{�X��q
class TStage2Guard : public TOBJ {
protected:
	bool Buzzed;//Buzz�������ǂ���
	s32 LastBuzz;//�Ō��Buzz��������
	TEnemyGauge2* HPGauge;
	s32 GaugeTime;
	s32 TotalFlight;//��s����
	s32 ActFlg[100];     //�s���t���O
	s32 ActCount[100];   //�s���J�E���^
	s32 LastFlash;
	s32 EnemyDeg; //�G�Ǝ����̑��Ίp�x
	f32 BSpeed;
	f32 IniDis;       //�ړI�n�܂ł̋���
	s32 DestDeg;     //�ړI�n�܂ł̊p�x
	s32 DestX,DestY;
	u8 OriginColor;
	TEnemyBladeTrace2* BladeTrace;
	TStage2GuardBWS* Bws;
	s32 ShotCount;
	TStage2Boss2* Master;
	bool BWSPurged;
	s32 Pattern;
	s32 TransferSpeed;
	virtual void AppearEvent();
	void Transfer(s32 _x, s32 _y, s32 _speed);
	void Transfer_NoMotion(s32 _x, s32 _y, s32 _speed);
	void Accel();
	void MakeShot(s32 _pat);
	void BladeMotion();
	void ShotMotion();
	virtual void SetNextState(s32 _current); 
	void Search();
	virtual void RetreatMotion();
	void StartingDemo();
	void SetNextPos();
public:
	TStage2Guard(TOBJList* owner, s32 _x,s32 _y, TStage2Boss2* _master);
	~TStage2Guard();
	void Move(); 
	void OnDamaged(TOBJ* target);
	void Die(); 
	void OnBroken();  

	//���i�ԍ�
	enum {
		BHip        = 0,
		BBody       = 1,
		BHead       = 2,
		BLShoulder  = 3,
		BRShoulder  = 4,
		BLArm       = 5,
		BRArm       = 6,
		BLThigh     = 7,
		BRThigh     = 8,
		BLLeg       = 9,
		BRLeg       =10,
		BLegCover   =11,
		BBlade      =12,

		BConstForeground=13,
	};

	//�s���ԍ�
	enum{
		KACTION_S2GR_APPEAR  = 0,
		KACTION_S2GR_TRANSFER = 1,
		KACTION_S2GR_BEAM = 2,
		KACTION_S2GR_BLADE = 3,
		KACTION_S2GR_RETREAT = 4,
		KACTION_S2GR_STARTDEMO = 5,
	};
};

//�X�e�[�W�R�{�X bit
class TStage3BossBit : public TOBJ {
protected:
	s32 ActFlg[100];     //�s���t���O
	s32 ActCount[100];   //�s���J�E���^
	s32 EnemyDeg; //�G�Ǝ����̑��Ίp�x
	f32 IniDis;       //�ړI�n�܂ł̋���
	s32 DestDeg;     //�ړI�n�܂ł̊p�x
	s32 DestX,DestY;
	s32 ShotCount;
	TBitLight* BitLight;
public:
	TStage3BossBit(TOBJList* owner, s32 _x, s32 _y);
	void Move(); 
	void SetNextState(s32 _current);
	void Place(f32 _x, f32 _y, f32 _speed);
	void MakeShot(s32 _pat);
	void Search();
	void MakeLaser();
	void BeamMotion();
	void SelfDestruction();
	void SelfDestMotion();
	void LightningMotion();
	void homingMotion();
	void SetLightningState(s32 _state);
	void Die(); 
	void DieWithExplosion();

	//�s���ԍ�
	enum{
		KACTION_SELFDEST = 0,
		KACTION_LIGHTNING = 1,
		KACTION_BEAM    = 2,
		KACTION_HOMING    = 3,
	};
};

//�X�e�[�W�R�{�X
class TStage3Boss : public TBossBase {
protected:
	s32 Beamcount;
	TStage3BossBit* Bit[2];
	void SetNextState(s32 _current);  
	void AppearEvent();
	void MakeShot(s32 _pat);
	void ShotMotion();
	void RetreatMotion();
	void Fall();
	void Search();
	void BeamMotion();
	void Aim(s32 _x, s32 _y);
	void MakeBeam(s32 dx, s32 dy);
	void LaserMotion();
	void LightningMotion();
	void MakeChargeEffect();
	void MakeLightning();
public:
	TStage3Boss(TOBJList* owner, s32 _x, s32 _y);
	void Move(); 
	void Die(); 

	//���i�ԍ�
	enum{
		BHip        = 0,
		BBody       = 1,
		BHead       = 2,
		BLShoulder  = 3,
		BRShoulder  = 4,
		BLArm       = 5,
		BRArm       = 6,
		BSkirt      = 7,
		BLThigh     = 8,
		BRThigh     = 9,
		BLLeg       = 10,
		BRLeg       = 11,
		BGun        = 12,
	};

	//�s���ԍ�
	enum{
		KACTION_TRANSFER = 0,
		KACTION_SHOT     = 1,
		KACTION_BEAM     = 2,
		KACTION_LASER    = 3,
		KACTION_LIGHTNING= 4,
		KACTION_FALL     = 5,
		KACTION_APPEAR   = 6,
		KACTION_RETREAT   = 7,
	};
};

//�X�e�[�W�S�{�X�P
class TStage4Boss1 : public TBossBase {
protected:
	s32 ShotCount;
	void SetNextState(s32 _current);  
	void MakeShot(s32 _pat);
	void ShotMotion();
	void Fall();
	void MakeChargeEffect();
public:
	TStage4Boss1(TOBJList* owner, s32 _x, s32 _y);
	void Move(); 
	void Die(); 

	//���i�ԍ�
	enum{
		BHip  = 0,
		BConstForeground=1,
	};

	//�s���ԍ�
	enum{
		KACTION_TRANSFER = 0,
		KACTION_SHOT     = 1,
		KACTION_BEAM     = 2,
		KACTION_LASER    = 3,
		KACTION_LIGHTNING= 4,
		KACTION_FALL     = 5,
		KACTION_APPEAR   = 6,
	};
};

//�X�e�[�W�S�{�X�P �j��㔚��
class TStage4Boss1Exp : public TOBJ {
protected:
	s32 ActFlg,ActCount;
	TDangerAlarm* Alarm;
	f32 Speed;
public:
	TStage4Boss1Exp(TOBJList* owner);
	~TStage4Boss1Exp();
	void Move(); 
	void Die(); 
};

//�X�e�[�W�S�{�X�Q
class TStage4Boss2 : public TBossBase {
protected:
	s32 Beamcount;
	s32 TargetX,TargetY;
	s32 CutterSpeed;
	void SetNextState(s32 _current);  
	void AppearEvent();
	void MakeShot(s32 _r);
	void ShotMotion();
	void RetreatMotion();
	void CutterMotion();
	void Fall();
	void Search();
	void Aim(s32 _x, s32 _y);
	void MakeSpread();
public:
	TStage4Boss2(TOBJList* owner, s32 _x, s32 _y);
	void Move(); 
	void Die(); 

	//���i�ԍ�
	enum{
		BHip        = 0,
		BBody       = 1,
		BHead       = 2,
		BLShoulder  = 3,
		BRShoulder  = 4,
		BLArm       = 5,
		BRArm       = 6,
		BSkirt      = 7,
		BLThigh     = 8,
		BRThigh     = 9,
		BLLeg       = 10,
		BRLeg       = 11,
		BGun        = 12,
		BShield     = 13,
		BHidedArm1  = 14,
		BHidedArm2  = 15,
		BCutter1    = 16,
		BCutter2    = 17,
	};


	//�s���ԍ�
	enum{
		KACTION_TRANSFER = 0,
		KACTION_SHOT     = 1,
		KACTION_BEAM     = 2,
		KACTION_LASER    = 3,
		KACTION_LIGHTNING= 4,
		KACTION_FALL     = 5,
		KACTION_APPEAR   = 6,
		KACTION_CUTTER   = 7,
		KACTION_RETREAT  = 8,
	};
};

//�X�e�[�W�S�{�X2
class TStage4Boss2Gene : public TOBJ {
protected:
public:
	TStage4Boss2Gene(TOBJList* owner, s32 _x, s32 _y);
	void Move(); 
};

//�X�e�[�W�S�K�[�h
class TStage4Guard : public TStage2Guard {
protected:
	void AppearEvent(); 
	void RetreatMotion(); 
	void SetNextState(s32 _current); 
public:
	TStage4Guard(TOBJList* owner, s32 _x, s32 _y);
	void Move(); 
};


//�X�e�[�W5�{�X1
class TStage5Boss1 : public TBossBase {
protected:
	s32 Beamcount;
	s32 TargetX,TargetY;
	f32 JumpSpeed;
	s32 FirstY;
	f32 FallSpeed;
	s32 RetreatDistance;
	TBossShadow* Shadow;
	s32 ShotTime;
	void SetNextState(s32 _current);  
	void AppearEvent();
	void MakeShot(s32 _pat);
	void ShotMotion();
	void SpreadMotion();
	void RetreatMotion();
	void Fall();
	void Search();
	void Aim(s32 _x, s32 _y, s32 _pat);
	void AimR(s32 _r, s32 _pat);
	void BodyAttack();
	void ShotMotion2();
	void JumpMotion();
	void RainMotion();
public:
	TStage5Boss1(TOBJList* owner, s32 _x, s32 _y);
	void Move(); 
	void Die(); 
	void OnDamaged(TOBJ* target);

	//���i�ԍ�
	enum{
		BHip        = 0,
		BBody       = 1,
		BWaist      = 2,
		BLShoulder  = 3,
		BRShoulder  = 4,
		BLArm       = 5,
		BRArm       = 6,
		BLThigh     = 7,
		BRThigh     = 8,
		BLThigh2     = 9,
		BRThigh2     = 10,
		BLLeg       = 11,
		BRLeg       = 12,
		BLLeg2       = 13,
		BRLeg2       = 14,
		BHead       = 15,
	};

	//�s���ԍ�
	enum{
		KACTION_TRANSFER = 0,
		KACTION_SHOT     = 1,
		KACTION_SPREAD     = 2,
		KACTION_BODYATTACK = 3,
		KACTION_SHOT2  = 4,
		KACTION_FALL     = 5,
		KACTION_APPEAR   = 6,
		KACTION_JUMP   = 7,
		KACTION_RAIN   = 8,
		KACTION_RETREAT   = 9,
	};
};

//�X�e�[�W5�{�X�Q
class TStage5Boss2 : public TBossBase {
protected:
	s32 Beamcount;
	s32 TargetX,TargetY;
	TStage3BossBit* Bit[4];
	s32 RetreatDistance;
	TEnemyBladeTrace2* BladeTrace[2];
	s32 RepeatCount;
	s32 ZakoCount;
	s32 TargetDeg;
	s32 motionid_sla1,motionid_sla2;
	void SetNextState(s32 _current);  
	void AppearEvent();
	void MakeShot(s32 _r);
	void ShotMotion();
	void RetreatMotion();
	void Fall();
	void Search();
	void Aim(s32 _x, s32 _y);
	void MakeSpread();
	void HomingMotion();
	void LaserMotion();
	void RotateBit();
	void CallZako();
public:
	TStage5Boss2(TOBJList* owner, s32 _x, s32 _y);
	void Move(); 
	void Die(); 

	//���i�ԍ�
	enum{
		BHip        = 0,
		BBody       = 1,
		BHead       = 2,
		BLShoulder  = 3,
		BRShoulder  = 4,
		BLArm       = 5,
		BRArm       = 6,
		BSkirt      = 7,
		BLThigh     = 8,
		BRThigh     = 9,
		BLLeg       = 10,
		BRLeg       = 11,
		BBlade1     = 12,
		BBlade2     = 13,
	};


	//�s���ԍ�
	enum{
		KACTION_TRANSFER = 0,
		KACTION_SHOT     = 1,
		KACTION_HOMING   = 2,
		KACTION_LASER    = 3,
		KACTION_CALL     = 4,
		KACTION_FALL     = 5,
		KACTION_APPEAR   = 6,
		KACTION_SLASH    = 7,
		KACTION_RETREAT  = 8,
	};
};

//�X�e�[�W5�{�X3���̖�
class TStage5Boss3Hair : public TOBJ {
protected:
	s32 RSpeed,RSpeedORG;
	TJoint* Element;
	s32 cnt;
	s32 cycle;
public:
	TStage5Boss3Hair(TOBJList* owner, TJoint* _spr);
	void Move(); 
};

//�X�e�[�W5�{�X3���̖ъǗ��N���X
class TStage5Boss3HairControler : public TOBJ {
protected:
	TJoint* OwnerSprite;     //���̃X�v���C�g�̑��֐߂ɑg�ݍ��ޏꍇ�́A�e�X�v���C�g
	vector<TStage5Boss3Hair*> Parts;
public:
	TStage5Boss3HairControler(TOBJList* owner, TJoint* _parent, s32 _x, s32 _y, s32 _r, s32 _segment, s32 _z, s32 _pat, s32 _col);
	~TStage5Boss3HairControler(); 
	void Move(); 
	void Die(); 
};


//5�ʃ{�X3�p�w�i�t�B���^�[
class TStage5Boss3BGFilter : public TOBJ {
private:
public:
	TStage5Boss3BGFilter(TOBJList* owner);
	void Move(); 
};

//�X�e�[�W5�{�X3
class TStage5Boss3 : public TBossBase {
protected:
	s32 Beamcount;
	s32 TargetX,TargetY;
	s32 RepeatCount;
	vector<TStage5Boss3HairControler*> Hair;
	TStage5Boss3BGFilter* Filter;
	void SetNextState(s32 _current);  
	void AppearEvent();
	void MakeShot(s32 _num, s32 _r);
	void ShotMotion();
	void HomingMotion();
	void HomingMotion2();
	void Fall();
	void Search();
	void CallZako(s32 _pat);
	void CallMotion1();
	void CallMotion2();
	void CallMotion3();
	void MakeHoming();
	void BeamMotion();
	void MakeHoming2();
public:
	TStage5Boss3(TOBJList* owner, s32 _x, s32 _y);
	void Move(); 
	void Die(); 
	void OnDamaged(TOBJ* target);

	//���i�ԍ�
	enum{
		BHip        = 0,
		BBody       = 1,
		BHead       = 2,
		BLShoulder  = 3,
		BRShoulder  = 4,
		BMask       = 5,
		BCanon1     = 6,
		BCanon2     = 7,
		BCanon3     = 8,
		BCanon4     = 9,
		BCanon5     = 10,
		BCanon6     = 11,
		BCanon7     = 12,
		BCanon8     = 13,
		BCanon9     = 14,
		BCanon10     = 15,
		BCanon11     = 16,
		BCanon12     = 17,
		BCanon13     = 18,
	};

	//�s���ԍ�
	enum{
		KACTION_TRANSFER = 0,
		KACTION_SHOT     = 1,
		KACTION_HOMING     = 2,
		KACTION_LASER    = 3,
		KACTION_FALL     = 4,
		KACTION_APPEAR   = 5,
		KACTION_CALL1   = 6,
		KACTION_CALL2   = 7,
		KACTION_CALL3   = 8,
		KACTION_BEAM   = 9,
		KACTION_HOMING2     = 10,
	};
};


//��̓r�[���i�X�e�[�W�R�^�C���I�[�o�[�j
class TBSBeam_St3 : public TOBJ {
private:
	s32 Rdeg;
public:
	TBSBeam_St3(TOBJList* owner, s32 _x, s32 _y, s32 _r);
	void Move(); 
};


//��̓r�[���W�F�l���[�^�i�X�e�[�W�R�^�C���I�[�o�[�j
class TBSBeamGen_St3 : public TOBJ {
private:
public:
	TBSBeamGen_St3(TOBJList* owner);
	void Move(); 
};

