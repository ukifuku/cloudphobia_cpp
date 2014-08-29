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

// �G���W�F�l���[�^1
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


// �G��1
class TZako01 : public TOBJ {
protected:
	bool Buzzed;//Buzz�������ǂ���
	s32 LastBuzz;//�Ō��Buzz��������
	s32 Rdeg;    //�p�x
	s32 Speed;                //���@�X�s�[�h
	s32 Pattern; //�s���p�^�[��
	bool GaugeOK;
	s32 GaugeTime;
	s32 TotalFlight;//��s����
public:
	TZako01(TOBJList* owner, s32 _x, s32 _y, s32 _R, s32 _S, s32 _pat, bool _red );
	void Move(); 
	void MakeShot();                              //�e������
	void OnDamaged(TOBJ* target);
	void Die();
	void OnBroken();
};

//�e
class TZ01Shot : public TOBJ {
private:
	bool Buzzed;//Buzz�������ǂ���
	s32 LastBuzz;//�Ō��Buzz��������
	bool Anim;//�A�j���[�V�����p�ϐ�
	s32 Rdeg;
public:
	TZ01Shot(TOBJList* owner, s32 sx, s32 sy);
	void Move(); 
	void OnDamaged(TOBJ* target); 
};


//�e2(�X�s�[�h�Ɗp�x�𒲐߉\)
class TZ01Shot2 : public TOBJ {
private:
	bool Buzzed;//Buzz�������ǂ���
	s32 LastBuzz;//�Ō��Buzz��������
	bool Anim;//�A�j���[�V�����p�ϐ�
	s32 Rdeg;
	s32 Speed;
	f32 IniXSpeed;//�X�N���[���X�s�[�h�̉e��
public:
	TZ01Shot2(TOBJList* owner, s32 sx, s32 sy, s32 _R, s32 _S, f32 _IniXS);
	void Move(); 
	void OnDamaged(TOBJ* target); 
};

//���ˉ�
class TZ01BFire : public TOBJ {
private:
	s32 Rdeg;
public:
	TZ01BFire(TOBJList* owner, s32 sx, s32 sy, s32 _R);
	void Move(); 
};

// �G���W�F�l���[�^2
class TZ02Gene : public TOBJ {
private:
	s32 Pattern;
	s32 Count;
	s32 TotalFlight;
public:
	TZ02Gene(TOBJList* owner, s32 _x, s32 _y, s32 _pat);
	void Move(); 
};

//�G��2�i�ˌ�����j
class TZako02 : public TOBJ {
private:
	bool Buzzed;//Buzz�������ǂ���
	s32 LastBuzz;//�Ō��Buzz��������
	s32 Rdeg;    //�p�x
	s32 Speed;                //���@�X�s�[�h
	s32 Pattern; //�s���p�^�[��
	TEnemyGauge2* HPGauge;
	s32 GaugeTime;
	s32 TotalFlight;//��s����
	s32 ActFlg;     //�s���t���O
	s32 ActCount;   //�s���J�E���^
public:
	TZako02(TOBJList* owner, s32 _x, s32 _y, s32 _R, s32 _S, s32 _pat, bool _red);
	void Move(); 
	void OnDamaged(TOBJ* target);
	void Die();
	void MakeShot();
	void OnBroken();
};


// �G���W�F�l���[�^3
class TZ03Gene : public TOBJ {
private:
	s32 Pattern;
	s32 Count;
	s32 TotalFlight;
public:
	TZ03Gene(TOBJList* owner, s32 _x, s32 _y, s32 _pat);
	void Move(); 
};

//�G��3�i�g�r�E�I�j
class TZako03 : public TOBJ {
private:
	bool Buzzed;//Buzz�������ǂ���
	s32 LastBuzz;//�Ō��Buzz��������
	s32 Rdeg;    //�p�x
	f32 Speed;                //���@�X�s�[�h
	s32 Pattern; //�s���p�^�[��
	bool GaugeOK;
	s32 GaugeTime;
	s32 TotalFlight;//��s����
	s32 PrevX,PrevY; //�O�t���[���ł̈ʒu
	s32 ShotNum;    //�V���b�g��
public:
	TZako03(TOBJList* owner, s32 _x, s32 _y, s32 _R, s32 _S, s32 _pat, bool _red );
	void Move(); 
	void MakeShot();
	void OnDamaged(TOBJ* target);
	void Die();
	void OnBroken();
};

// �G���W�F�l���[�^4
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

//�G��4�o���\��
class TZ04Warn : public TOBJ {
private:
	s32 TotalFlight;
	f32 ZoomRate;
public:
	TZ04Warn(TOBJList* owner, s32 _x, s32 _y, bool _red);
	void Move(); 
};

//�G��4�i���^�@�j
class TZako04 : public TOBJ {
private:
	bool Buzzed;//Buzz�������ǂ���
	s32 LastBuzz;//�Ō��Buzz��������
	s32 Rdeg;    //�p�x
	s32 Speed;                //���@�X�s�[�h
	s32 Pattern; //�s���p�^�[��
	bool GaugeOK;
	s32 GaugeTime;
	s32 TotalFlight;//��s����
	s32 ActFlg;     //�s���t���O
	s32 ActCount;   //�s���J�E���^
public:
	TZako04(TOBJList* owner, s32 _x, s32 _y, s32 _R, s32 _S, s32 _pat, bool _red );
	void Move(); 
	void OnDamaged(TOBJ* target);
	void Die();
	void OnBroken();
};

//�G��5�i��^�@�j
class TZako05 : public TOBJ {
private:
	bool Buzzed;//Buzz�������ǂ���
	s32 LastBuzz;//�Ō��Buzz��������
	s32 Rdeg;    //�p�x
	f32 XSpeed,YSpeed;                //���@�X�s�[�h
	s32 Pattern; //�s���p�^�[��
	TEnemyGauge2* HPGauge;
	s32 GaugeTime;
	s32 TotalFlight;//��s����
	s32 ActFlg[10];     //�s���t���O
	s32 ActCount[10];   //�s���J�E���^
	s32 LastFlash;
	bool MissileON;  //�~�T�C���g�p��
	s32 MisCount;   //�~�T�C�����˓���p�J�E���^
	bool BeamON;     //�r�[���g�p��
	s32 BeamCount;  //�r�[�����˓���p�J�E���^
	s32 GunR;   //�r�[���C�p�x
	s32 EnemyDeg; //�G�Ǝ����̑��Ίp�x
	bool Boost;    //�u�[�X�g
	s32 BoostCount;
	f32 BSpeed;
	s32 UnitNum; //�@�̔ԍ��i�~�T�C�����ˎ��ɕK�v�j
	s32 ShotR;    //�V���b�g�ˊp
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

//�G�~�T�C��
class TZ05Missile : public TOBJ {
private:
	bool Buzzed;//Buzz�������ǂ���
	s32 LastBuzz;//�Ō��Buzz��������
	s32 YMove; //   Y�����̓���
	s32 Rdeg;
	TSmoke* PrevSmoke;   //�O��o�������I�u�W�F�N�g
	s32 PrevR;   //�O�t���[���ł̊p�x
public:
	TZ05Missile(TOBJList* owner, s32 sx, s32 sy, s32 _misNum);
	void Move(); 
	void Die(); 
	void OnDamaged(TOBJ* target); 
};

//�r�[���f��(��)
class TZBeamPart : public TOBJ {
private:
	bool Buzzed;//Buzz�������ǂ���
	s32 LastBuzz;//�Ō��Buzz��������
	s32 Rdeg;
public:
	TZBeamPart(TOBJList* owner, s32 sx, s32 sy, s32 R, f32 _zoom, bool _hit);
	void Move(); 
	void OnDamaged(TOBJ* target);
};


//�r�[���f��(��)
class TFunBeamPart : public TOBJ {
private:
	s32 Rdeg;
	s32 Speed;
	s32 Life;
public:
	TFunBeamPart(TOBJList* owner, s32 sx, s32 sy, s32 R, s32 S, s32 L, f32 _zoom);
	void Move(); 
};

//�r�[���{��
class TFunBeamCore : public TOBJ {
private:
	s32 Rdeg;
	bool Buzzed;//Buzz�������ǂ���
	s32 LastBuzz;//�Ō��Buzz��������
	s32 Pattern;
public:
	TFunBeamCore(TOBJList* owner, s32 sx, s32 sy, s32 R, s32 _pat);
	void Move(); 
	void OnDamaged(TOBJ* target);
};


//�z�[�~���O�r�[���{��
class TZakoHomingCore : public TOBJ {
private:
	bool Buzzed;//Buzz�������ǂ���
	s32 LastBuzz;//�Ō��Buzz��������
	s32 Rdeg;
	s32 PrevR;   //�O�t���[���ł̊p�x
	s32 HomingLevel;    //�ǔ��̋Ȃ���₷��
	THomingTrace* Trace;   //�r�[���̋O��
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

//�ڂ��烌�[�U�[
class TEyeLaser : public TOBJ {
private:
	s32 Rdeg;
public:
	TEyeLaser(TOBJList* owner, s32 sx, s32 sy, s32 _r);
	void Move(); 
};

//�ڂ��烌�[�U�[���i
class TEyeLaserParts : public TOBJ {
private:
	bool Buzzed;//Buzz�������ǂ���
	s32 LastBuzz;//�Ō��Buzz��������
	s32 Rdeg;
public:
	TEyeLaserParts(TOBJList* owner, s32 sx, s32 sy, s32 _r);
	void Move(); 
	void OnDamaged(TOBJ* target); 
};

//�z�[�~���O�r�[���{�̂��̂Q
class TZakoHomingCore2 : public TOBJ {
private:
	bool Buzzed;//Buzz�������ǂ���
	s32 LastBuzz;//�Ō��Buzz��������
	s32 Rdeg;
	s32 PrevR;   //�O�t���[���ł̊p�x
	s32 HomingLevel;    //�ǔ��̋Ȃ���₷��
	THomingTrace* Trace;   //�r�[���̋O��
public:
	TZakoHomingCore2(TOBJList* owner, s32 sx, s32 sy, s32 _r, s32 _level);
	~TZakoHomingCore2();
	void Move(); 
	void Die(); 
	void OnDamaged(TOBJ* target); 
};

//�����蔻��̂������
class TEnergyBall : public TOBJ {
private:
	bool Buzzed;//Buzz�������ǂ���
	s32 LastBuzz;//�Ō��Buzz��������
	f32 ZoomRate;
	s32 Life;

public:
	TEnergyBall(TOBJList* owner, s32 sx, s32 sy, s32 _life, f32 _zoom);
	void Move(); 
	void OnDamaged(TOBJ* target); 
};

//�e3(�X�s�[�h�A�p�x�A�З́A�g�嗦���߉\)
class TZ01Shot3 : public TOBJ {
private:
	bool Buzzed;//Buzz�������ǂ���
	s32 LastBuzz;//�Ō��Buzz��������
	bool Anim;//�A�j���[�V�����p�ϐ�
	s32 Rdeg;
	s32 Speed;
	f32 OriginXZoom;
	f32 IniXSpeed;//�X�N���[���X�s�[�h�̉e��
public:
	TZ01Shot3(TOBJList* owner, s32 sx, s32 sy, s32 _R, s32 _S, s32 _str, f32 _IniXS, f32 _zoom);
	void Move(); 
	void OnDamaged(TOBJ* target); 
};

//2�{�X���[�U�[���i
class TS2Boss2LaserParts : public TOBJ {
private:
	bool Buzzed;//Buzz�������ǂ���
	s32 LastBuzz;//�Ō��Buzz��������
	s32 Rdeg;
public:
	TS2Boss2LaserParts(TOBJList* owner, s32 sx, s32 sy, s32 _r);
	void Move(); 
	void OnDamaged(TOBJ* target); 
};

//�r�[���f��(��) �i�C��̈ړ��ɒǏ]�j
class TZBeamPart2 : public TOBJ {
private:
	bool Buzzed;//Buzz�������ǂ���
	s32 LastBuzz;//�Ō��Buzz��������
	s32 Rdeg;
	f32 XSpeed,YSpeed;
public:
	TZBeamPart2(TOBJList* owner, s32 sx, s32 sy, f32 _xspeed, f32 _yspeed, s32 R, f32 _zoom, bool _hit);
	void Move(); 
	void OnDamaged(TOBJ* target);
};

//�r�[�����ˉ��o
class TBeamControl : public TOBJ {
private:
	s32 Rdeg;
	s32 Pattern;
public:
	TBeamControl(TOBJList* owner, s32 sx, s32 sy, s32 _pat);
	void Move(); 
};

//�R�{�X�V���b�g
class TBoss3Shot : public TOBJ {
private:
	bool Buzzed;//Buzz�������ǂ���
	s32 LastBuzz;//�Ō��Buzz��������
	bool Anim;//�A�j���[�V�����p�ϐ�
	s32 Rdeg;
	f32 Speed;
	f32 OriginXZoom;
	f32 IniXSpeed;//�X�N���[���X�s�[�h�̉e��
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

//�R�{�X�r�[��
class TBoss3Beam : public TOBJ {
private:
	bool Buzzed;//Buzz�������ǂ���
	s32 LastBuzz;//�Ō��Buzz��������
	s32 Rdeg;
	s32 DestX, DestY;
	THomingTrace* Trace;   //�r�[���̋O��
public:
	TBoss3Beam(TOBJList* owner, s32 sx, s32 sy, s32 dx, s32 dy);
	~TBoss3Beam(); 
	void Move(); 
	void Die(); 
	void OnDamaged(TOBJ* target); 
};

//�����蔻��̒����r�[��
class TLongBeam : public TOBJ {
private:
	bool Buzzed;//Buzz�������ǂ���
	s32 LastBuzz;//�Ō��Buzz��������
	s32 Rdeg;
	f32 MaxZoom;
	f32 ZoomSpeed;
public:
	TLongBeam(TOBJList* owner, s32 sx, s32 sy, s32 R, f32 _zoom, f32 _speed);
	~TLongBeam(); 
	void Move(); 
	void OnDamaged(TOBJ* target); 
};

//�r�[������
class T8WayBeam : public TOBJ {
private:
	s32 Rdeg;
	s32 ShotCount;
public:
	T8WayBeam(TOBJList* owner, s32 sx, s32 sy);
	void Move(); 
};

//���p���[�U�[�f��
class TOrthLaserPart : public TOBJ {
private:
	bool Buzzed;//Buzz�������ǂ���
	s32 LastBuzz;//�Ō��Buzz��������
	s32 Life;
public:
	TOrthLaserPart(TOBJList* owner, s32 sx, s32 sy, s32 _life, bool _col);
	void Move(); 
	void OnDamaged(TOBJ* target); 
};

//���p���[�U�[
class TOrthLaser : public TOBJ {
private:
	s32 Speed;
	s32 Life;
	s32 TurnCount;
public:
	TOrthLaser(TOBJList* owner, s32 sx, s32 sy, s32 _speed, s32 _life);
	void Move(); 
};

//�Ȃ��郌�[�U�[
class TCurvedLaser : public TOBJ {
private:
	TPolyLine2D* pl;
	vector<glm::vec2> Points;
	vector<TCollisionSpr*> Collision;
	f32 Speed,Accel,Velocity;
	bool Buzzed;//Buzz�������ǂ���
	s32 LastBuzz;//�Ō��Buzz��������
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

//�X�v���b�h�r�[��
class TSpreadBeam : public TOBJ {
private:
	bool Buzzed;//Buzz�������ǂ���
	s32 LastBuzz;//�Ō��Buzz��������
	s32 Rdeg;
	s32 DestX,DestY;
	f32 Speed;
	s32 ActFlg,ActCount;
public:
	TSpreadBeam(TOBJList* owner, s32 _x, s32 _y, s32 _dx, s32 _dy, f32 _speed);
	void Move(); 
	void OnDamaged(TOBJ* target); 
};

//�X�v���b�h2
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

//�r�[���̉J
class TBeamRain : public TOBJ {
private:
	s32 Life;
	f32 FallSpeed;
public:
	TBeamRain(TOBJList* owner, s32 _x, s32 _y, s32 _life, f32 _speed);
	void Move(); 
};

//�z�[�~���O���[�U�[(��)
class TZakoHomingCore3 : public TOBJ {
private:
	TPolyLine2D* pl;
	vector<glm::vec2> Points;
	vector<TCollisionSpr*> Collision;
	f32 Speed,Accel,Velocity;
	bool Buzzed;//Buzz�������ǂ���
	s32 LastBuzz;//�Ō��Buzz��������
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

//�X���[�r�[��
class TSlowBeam : public TOBJ {
private:
	f32 Speed;
	s32 Life;
	f32 ZoomRate;
	bool Buzzed;//Buzz�������ǂ���
	s32 LastBuzz;//�Ō��Buzz��������
public:
	TSlowBeam(TOBJList* owner, s32 _x, s32 _y, s32 _R, s32 _L, f32 _speed, f32 _zoom);
	void Move(); 
	void OnDamaged(TOBJ* target); 
};


//�C���C�U�[�{��
class TEraserCore : public TOBJ {
private:
public:
	TEraserCore(TOBJList* owner, s32 sx, s32 sy);
	void Move(); 
};

//�C���C�U�[�f��
class TEraserPart : public TOBJ {
private:
public:
	TEraserPart(TOBJList* owner, s32 sx, s32 sy);
	void Move(); 
};

//�����蔻��̂������ �o�[�X�g
class TEnergyBallBurst : public TOBJ {
private:
	f32 ZoomRate;
	s32 Life;
	s32 Amount;
public:
	TEnergyBallBurst(TOBJList* owner, s32 sx, s32 sy, s32 _amount, s32 _life, f32 _zoom);
	void Move(); 
};

//�r�[��2
class TZBeam2 : public TOBJ {
private:
	bool Buzzed;//Buzz�������ǂ���
	s32 LastBuzz;//�Ō��Buzz��������
	TJoint* Parent;
public:
	TZBeam2(TOBJList* owner, TJoint* _parent, s32 sx, s32 sy, f32 _zoom);
	void Move(); 
	void OnDamaged(TOBJ* target); 
	void Die(); 
};

//�e4�i�e�Q�̃O���t�B�b�N�ύX�j
class TZ01Shot4 : public TOBJ {
private:
	bool Buzzed;//Buzz�������ǂ���
	s32 LastBuzz;//�Ō��Buzz��������
	bool Anim;//�A�j���[�V�����p�ϐ�
	s32 Rdeg;
	s32 Speed;
	f32 IniXSpeed;//�X�N���[���X�s�[�h�̉e��
	s32 BulletColor;  //�e�̐F�i0:�X�e�[�W���ƂɍœK���@1:�ԁ@2:�j
public:
	TZ01Shot4(TOBJList* owner, s32 sx, s32 sy, s32 _R, s32 _S, f32 _IniXS, s32 _col, s32 _str);
	void Move(); 
	void OnDamaged(TOBJ* target); 
};

enum{
	//�ˌ����񕔕i�ԍ�
	Z2Body		= 0,
	Z2Wing		= 1,
	Z2Tail		= 2,
	Z2Sonic		= 3,
};

//���^���񕔕i�ԍ�
enum{
	Z4Body      = 0,
	Z4UpperWing = 1,
	Z4lowerWing = 2,
	Z4Fire      = 3,
};

//��^���񕔕i�ԍ�
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

