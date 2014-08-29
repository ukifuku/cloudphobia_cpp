#pragma once

#include "types.h"
#include "Define.h"
#include "PolyJoint.h"
class TOBJList;
class TJoint;

#define MaxJoint 49     //�ő�Ԑڐ�

//�I�u�W�F�N�g��N���X
class TOBJ
{
friend TOBJList;
protected:
	string FObjectName;   //�I�u�W�F�N�g�̖��O�i�f�o�b�O���O�p�j
	s32 Age;			  //��������Ă���̎���
	s32 FID;
	TOBJKind FKind;     //�G�̎��
	TOBJList* FOwner;    //�I�u�U�[�o�[�ƂȂ�TOBJList�̃C���X�^���X
	bool FDead;      //���Ƀt���O
	s32 FSTR;       //���������Ƃ��̍U����
	s32 FDef;       //�h���
	s32 Score;      //���_
	s32 Locknum;    //���b�N�I���ԍ�
	s32 numSpr;                //�I�u�W�F�N�g�������̃X�v���C�g��
	bool CommandRed;
	s32 ComBonus;
	f32 ResilienceX;
	f32 ResilienceY;   //�����́i�����蔻���̏Փˉ����Ŏg�p�j
	bool FIsParentExists;     //�e�����邩�i���Z�b�g���A���ŏ�����e�ɔC���Ă��邩�j

public:
	s32 Sprite_Center;
	s32 LastSlashed;			 //�u���[�h�A���q�b�g�h�~�J�E���^
	TJoint* Sprite[MaxJoint];   //�X�v���C�g
	s32 X;
	s32 Y;        //���W
	s32 HP;         //�q�b�g�|�C���g
	s32 TempHP;     //���b�N�I�������pHP
	bool Hit[MaxJoint];  //������t���O
	s32 distance;   //����I�u�W�F�N�g�Ԃ̋���
	// TempSlide:TPoint;   //�U�������Ƃ��̈ړ���
	s32 BonusRate; //�{�[�i�X���[�g
	s32 TempValue[8];
	s32 MaxHP;
	bool deleted;

public:
	TOBJ( TOBJList* _owner );
	virtual ~TOBJ();
	virtual void MakeSprite( TDGCarad* _dddd, TDGTexture* _Tex, s32 _num, s32 TX, s32 TY, s32 TW, s32 TH, s32 Z, s32 HX, s32 HY, s32 HW, s32 HH, s32 _cx, s32 _cy, bool com );
	virtual void MakeSpriteLite( s32 _num, s32 _x, s32 _y, s32 _w, s32 _h, s32 _z, TBlendMode _blend );
	virtual bool CheckTouch( TOBJ* Target );
	virtual void Move();
	virtual void OnDamaged( TOBJ* Target );
	virtual void Die();
	virtual s32 OnDraw();
	virtual TOBJKind Kind() { return FKind; }
	virtual s32 STR() { return FSTR; }
	virtual bool Dead() { return FDead; }
	virtual s32 ID() { return FID; }
	virtual s32 Def() { return FDef; }
	virtual bool IsParentExists() { return FIsParentExists; }
	virtual void OnBroken(); 
	virtual void OnResponse(stringstream& _res);
	virtual s32 OnExRender();
	virtual void OnDeviceLost(); 
	virtual void OnDeviceReset(); 
	virtual s32 GetAge();
	virtual void MakeSprFromID( TDGCarad* _dddd, s32 _id, TDGTexture* _tex, s32 _z, bool _extra );
	virtual void MotionControl( s32 _id );
	virtual void MotionControlSoon( s32 _id );
	virtual void CollisionReaction( f32 _x, f32 _y );

protected:
	virtual void CalcBonus();
};