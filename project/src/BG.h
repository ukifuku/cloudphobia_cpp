#pragma once

#include "TOBJ.h"


class TRainFilter;

//��i�V�X�e���������ɐ����j
class TSky : public TOBJ
{
protected:
	Rect	m_BltRect;
public:
	TSky(TOBJList* _owner);
};

//��2�i�r�f�I�{�[�h��VRAM�݂̂��g�p�j
class TSky2 : public TOBJ
{
public:
	TSky2(TOBJList* _owner);
	void Move();
};

//�_
class TCloud : public TOBJ
{
protected:
	s32	ZValue;
public:
	TCloud(TOBJList* _owner, s32 _x, s32 _y, s32 _z );
	void Move();
};

//�_2
class TCloud2 : public TOBJ
{
protected:
	s32	Pattern;
	s32	Speed;
	u8	Alpha;
public:
	TCloud2(TOBJList* _owner, s32 _x, s32 _y, s32 _pat, u8 _alpha, s32 _x1, s32 _y1, s32 _x2, s32 _y2);
	void Move(); 
};

//�_3
class TCloud3 : public TOBJ
{	
protected:
	s32	Pattern;
	s32	Speed;
	u8	Alpha;
	TOBJ*	OwnerOBJ;
public:
	TCloud3( TOBJList* _owner, s32 _x, s32 _y, s32 _pat, u8 _alpha, s32 x1, s32 y1, s32 x2, s32 y2, TOBJ* _obj);
	void Move(); 
};

//�_����
class TCloudMaker : public TOBJ
{
public:
	f32	Cdistance[6];
	s32	CloudYPos[6];
public:
	TCloudMaker( TOBJList* _owner, s32 _y );
	void Move();   
};

//�_�����i��ʑS�̂𕢂��j
class TCloudMaker2 : public TOBJ
{
protected:
	Rect	TexRect[96];
	s32	Cdistance[2];
	s32	TotalDist;
	s32	CloudFlg[2];
	s32	EndDistance;
	u8	Alpha;
public:
	TCloudMaker2( TOBJList* _owner );
	void	Move();
};

//�_�����i�����_�j
class	TCloudMaker3 : public TOBJ
{
public:
	TCloudMaker3(TOBJList* _owner, s32 _x, s32 _y);
	void	Move();    
};

//�_�����i�O���̔w�i�̃A�b�p�[�o�[�W�����j
class	TCloudMaker4 : public TOBJ
{
protected:
	s32 Cdistance[9];
	s32	TotalDist;
	Rect	TexRect[96];
	s32	CloudFlg[9];
	u16	Alpha;
public:
	TCloudMaker4(TOBJList* _owner, s32 _y);
	void	Move();   
};

//���z
class TSun : TOBJ
{
protected:
	s32	ActFlg;
	s32	ActCount;
	s32	EndDistance;
public:
	TSun(TOBJList* _owner, s32 _x, s32 _y);
	void 	Move();   
	void 	OnDamaged(TOBJ& _target);
};

//��
class TMoon : public TOBJ
{
protected:
	s32	ActFlg;
	s32	ActCount;
public:
	TMoon(TOBJList* _owner, s32 _x, s32 _y);
	void Move();
};

//�����Y�t���A
class TFlare : public TOBJ
{
public:
	TFlare(TOBJList* _owner, s32 _x, s32 _y, f32 _a);
	void 	Move();
};


//�c�X�N���[���C�x���g(�㏸)
class TVScroll : public TOBJ
{
protected:
	s32	ActFlg;
	s32	ActCount;
	s32	TotalFlight;
	b8	ClSet[4];
public:
	TVScroll(TOBJList* _owner);
	void Move();  
};

//�c�X�N���[���C�x���g�i���~�j
class	TDownScroll : public TOBJ
{
private:
	s32	ActFlg;
	s32	ActCount;
	s32	TotalFlight;
public:
	TDownScroll(TOBJList* _owner);
	void	Move();
};

//���z�����C�N
class	TSun2 : public TOBJ
{
private: 
	s32	ActFlg;
	s32	ActCount;
	TRainFilter*	RainFilter;
public:
	TSun2(TOBJList* _owner, s32 _x, s32 _y);
	void Move();
	void Die();
};

//�_�i�����j
class	TCloudFast : public TOBJ
{
private:
	s32	PersonalR;
	s32	IncR;
	f32	ZoomRate;
public:
	TCloudFast(TOBJList* _owner, s32 _x, s32 _y);
	void Move();
};

//����
class	TSandStorm : public TOBJ
{
public:
	TSandStorm(TOBJList* _owner, s32 _x, s32 _y);
	void Move();    
};

//��i�X�e�[�W�Z���N�g��ʗp�j
class	TSky_StageSelect : public TOBJ
{
public:
	TSky_StageSelect(TOBJList* _owner);
	void Move();    
};

//�X�e�[�W5���y����
class	TStage5MusicControl : public TOBJ
{
public:
	TStage5MusicControl(TOBJList* _owner);
	void Move();    
};



