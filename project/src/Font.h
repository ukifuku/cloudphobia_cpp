#pragma once

using namespace std;

#include "types.h"
#include "TOBJ.h"
#include <vector>

//�t�H���g(�ꕶ����)
class TMoji : public TOBJ {
private:
	s32 pat;  //����p�^�[��
public:
	TMoji(TOBJList* owner, s32 moji, s32 fx, s32 fy, s32 _cx, s32 _cy, s32 _pat);
	void Move();
};

//�t�H���g2(�O���A)
class TMoji_blight : public TOBJ {
private:
	bool Blink;
public:
	TMoji_blight(TOBJList* owner, s32 moji, s32 fx, s32 fy, bool _blink);
	void Move();
};

//�t�H���g3(���S�V�b�N)
class TMoji_gothic : public TOBJ {
private:
public:
	TMoji_gothic(TOBJList* owner, s32 moji, s32 fx, s32 fy, s32 z);
	void Move();
	void MakeUnit(s32 _moji);
};

//�t�H���g4(���l���p�ɂɕς��J�E���^�p)
class TMoji_counter : public TOBJ {
private:
public:
	TMoji_counter(TOBJList* owner, s32 fx, s32 fy, s32 z);
	void Move();
	void Update(s32 _val);
};

//�t�H���g5(�I�����W)
class TMoji_orange : public TOBJ {
private:
public:
	TMoji_orange(TOBJList* owner, s32 moji, s32 fx, s32 fy, s32 z);
	void Move();
};


//�ړ�������
class TMoveSentence : public TOBJ {
private:
	s32 ActFlg;
	s32 pattern;  //����p�^�[��
	s32 RDeg;
	s32 IniDis;  //�ړI�n�܂ł̋���
	s32 DestX,DestY;//�ړI�n
	string letter;  //�\�����镶��
	s32 life;   //����
public:
	TMoveSentence(TOBJList* owner, const string& moji, s32 sx, s32 sy, s32 dx, s32 dy, s32 _life, s32 _pat);
	void Move();
};

//�X�v���C�g�̂悤�Ɉ����镶����
class TSprSentence : public TOBJ {
private:
	vector<TOBJ*> MojiList;
	s32 FontKind;
public:
	TSprSentence(TOBJList* owner, const string& moji, s32  sx, s32 sy, s32 brank, s32 font);
	~TSprSentence();
	void Move();
	void Die();
	void SetAlpha(s32 _alpha);
	void SetColor(s32 _red, s32 _green,s32 _blue);
	void SetZ(u16 _rz);
};

class TMojiAndGlassCursor : public TOBJ {
private:
	vector<TOBJ*> SentenceList;
	s32 Pattern;
	s32 XSpeed,YSpeed;   //�J�[�\���ړ��X�s�[�h
	s32 NowX,NowY;   //�J�[�\�����݈ʒu
	s32 DestX,DestY;  //�ړI�n���W
	s32 XSpeedFactor,YSpeedFactor;  //�J�[�\���X�s�[�h����p�ϐ�
	s32 DelayTime;          //�ړ��J�n��x�点�鎞��
	s32 CurrentAlpha,DestAlpha;              //Alpha�l
	s32 AlphaSpeed;         //Alpha�l�ύX�X�s�[�h
	s32 AlphaDelayTime;     //Alpha�l�ύX�J�n��x�点�鎞��
	TPoint DeathPoint;          //���̒n�_�Ɉړ�����Ə���
	bool Blink;          //�_��
	s32 MaxSprOfCursor;   //�J�[�\���\���Ɏg�p���Ă���X�v���C�g��
	s32 SlideDelayCount;
	s32 AlphaDelayCount;
	void Slide();
	void ModulateAlpha();
public:
	TMojiAndGlassCursor(TOBJList* owner, s32 _x, s32 _y, s32 _long, s32 _pat);
	~TMojiAndGlassCursor();
	void Move();
	void Die();
	void AddSentence(const string& _moji, s32 _x, s32 _brank, s32 _red, s32 _green, s32 _blue, s32 _font);
	void AddSentenceRight(const string& _moji, s32 _x, s32 _brank, s32 _red, s32 _green, s32 _blue, s32 _font);
	void SetAlpha(s32 _alpha);
	void SetDest(s32 _x, s32 _y,s32 _delay);
	void SetColor(s32 _red, s32 _green, s32 _blue);
	void SetSpeed(s32 _x, s32 _y);
	void SetDestAlpha(s32 _dest, s32 _speed, s32 _delay);
	void SetDeathPoint(s32 _x, s32 _y);
	void SetBlink(bool _state);
	void BlinkControl();
	void AddLine(s32 _x);
	void SetRank(s32 _rank);
};

//�{���\��
class TMultiple : public TOBJ {
private:
public:
	TMultiple(TOBJList* owner, s32 _mul, s32 _x, s32 _y);
	void Move();
};

//�G�j��X�R�A�\��
class TEnemyScore : public TOBJ {
private:
	TSprSentence*	sentence;
public:
	TEnemyScore(TOBJList* owner, s32 _x, s32 _y, s32 _score);
	void Move();
	void Die();
};

//�q�b�g��
class THitCount : public TOBJ {
private:
s32 CurrentHit;
public:
	THitCount(TOBJList* owner, s32 _x, s32 _y);
	void Move();
	void Die();
};

//�R�}���_�[�{�[�i�X
//class TComBonus : public TOBJ {
//private:
//	TMojiAndGlassCursor* sentence[2];
//public:
//	TComBonus(TOBJList* owner, s32 _score);
//	~TComBonus();
//	void Move();
//	void Die();
//};

//�J�[�\���Ǘ��N���X
class TCursorList {
private:
	vector<TMojiAndGlassCursor*>	m_array;
	TOBJList* FOwner;
	s32 	OrgX,OrgY;
public:
	TCursorList(TOBJList* owner);
	~TCursorList();
	void MakeCursor(const string& _caption, s32 _x, s32 _y, s32 _blank, s32 _width, s32 _num, s32 _font, s32 _pat, s32 _pos);
	void MakeCursor_Base(const string& _caption, s32 _x, s32 _y, s32 _blank, s32 _width,  s32 _num, s32 _font, s32 _pat, s32 _pos,  s32 _red,  s32 _green,  s32 _blue,  s32 _alpha);
	void ClearCursors( s32 _x, s32 _y);
	void SetAppearPoint( s32 _x, s32 _y);
	u32	GetSize() { return m_array.size(); }
	TOBJ*	GetItem(s32 _idx) { return m_array[_idx]; }
};

//���l���p�ɂɕς��J�E���^�p_�S�V�b�N
class TGothic_counter : public TOBJ {
private:
	bool ShowZero ;	//�[���T�v���X���邩
	s32 Desit;
public:
	TGothic_counter(TOBJList* owner, s32 fx, s32 fy, s32 z, s32 _desit, bool _showzero);
	void Move();
	void Update(s32 _val);
};

//�R�}���_�[�{�[�i�X�����C�N
class TComBonus2 : public TOBJ {
private:
	TSprSentence* sentence[2];
public:
	TComBonus2(TOBJList* owner, s32 _x, s32 _y, s32 _base, s32 _mul);
	void Move();
	void Die();
};

//�A���t�@�l
#define		GLASSCURSOR_BLIND_ALPHA		 128
#define		GLASSCURSOR_GLASS_ALPHA		 140
#define		GLASSCURSOR_REFLEX_ALPHA	 180
#define		GLASSCURSOR_PARTITION_ALPHA  180
#define		GLASSCURSOR_FONT_ALPHA		 255

//�J�[�\���\���p�^�[��
#define		GLASSCURSOR_PATTERN_NONE	  0
#define		GLASSCURSOR_PATTERN_BLIND	  1
#define		GLASSCURSOR_PATTERN_GLASS	 2
#define		GLASSCURSOR_PATTERN_LINE	 3
#define		GLASSCURSOR_PATTERN_BLIGHT   4
#define		GLASSCURSOR_PATTERN_BLACK_LEFT   5
#define		GLASSCURSOR_PATTERN_BLACK_RIGHT  6
#define		GLASSCURSOR_PATTERN_BLACK_CENTER  7
#define		GLASSCURSOR_PATTERN_SHADOW   8

//�J�[�\���F
#define		GLASSCURSOR_BLIGHT_RED   190
#define		GLASSCURSOR_BLIGHT_GREEN   90

//�t�H���g���
#define		KFONTKIND_NORMAL   0
#define		KFONTKIND_BLIGHT   1
#define		KFONTKIND_GOTHIC   2
#define		KFONTKIND_BLINK    3
#define		KFONTKIND_ORANGE   4
#define		KFONTKIND_BLINK2   5

//�J�[�\���\���ʒu
#define		KCURSOR_POS_LEFT     0
#define		KCURSOR_POS_CENTER   1
#define		KCURSOR_POS_RIGHT    2


static const s8 FONTSET[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e',
			'f','g','h','i','j','k','l','m','n','o','p','q','r','s','t',
			'u','v','w','x','y','z','.',',',':',';','"','+','-','*','/',
			'!','?',' ','^','~','|','_','{'};  //48�Ԗڈȍ~�͓��ꕶ���w��p�̃_�~�[����

static const s8 FONTSET_ORANGE[] = {'0','1','2','3','4','5','6','7','8','9',
				   'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p',
				   'q','r','s','t','u','v','w','x','y','z','.',',',':',';','\'','+',
				   '-','*','/','!','?',' ','A','B','C','D','E','F','G','H','I','J',
				   'K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
				   '<','>','"','#','$','%','&','(',')','=','~','\\','@'};



#define		KHEROPOS_TOPGAUGE_HIDED  150
#define		KHEROPOS_BOTTOMGAUGE_HIDED  330
#define		KGAUGE_FADESPEED  24
#define		KGAUGE_SHIFTDISTANCE  56
#define		KGAUGE_SHIFTSPEED  4
