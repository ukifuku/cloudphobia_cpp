//^( +)([^\(\n]+):Boolean;
//$1bool $2


#pragma once

using namespace std;

#include "types.h"
#include "TOBJ.h"
#include <vector>

class TMojiAndGlassCursor;
class TBlur;
class TMSGauge2;
class TCursorList;
class TArrow;
class TConnectingGauge;
class TGlassCursor;
class TBustShot;
class TSprSentence;
class TGothic_counter;
class TSea;  //�C
class TUnderSea;  //
class TDawncloud;  //�_
class TFriend;
class TStage2ScrollBlur;
class TMotherShip;
class TDemoEraser;
class TGlassCubeFilter;
class TStSelCursor;
class TKeyBoard;

struct ConfigCursor //�R���t�B�O�J�[�\���Ǘ��\����
{
	bool Horizontal;       //���E�I������
	s32 Current;          //���݂̑I��l
	vector<string> Caption;           //���݂̐ݒ�l����
	TMojiAndGlassCursor* Owner;    //�e�J�[�\��
	TMojiAndGlassCursor* Spr;           //�Ή�����X�v���C�g
};

//�Q�[���I�[�o�[��ʃN���X
class TGameOver : public TOBJ
{
private:
	s32 DeadPattern;  //����
	TBlur* Blur;
	s32 ActFlg;
	TCursorList* CursorList;
	TMSGauge2* MSGauge;
public:
	TGameOver(TOBJList* owner, s32 _dp);
	~TGameOver();
	void Move();
	void Input();
	void Die();
};

//Ready��ʃN���X
class TReady : public TOBJ{
private:
	s32 ReadyTime;
	TCursorList* Cursor;
	s32 bandWidth;
public:
	TReady(TOBJList* owner, s32 _time);
	~TReady();
	void Move();
};

//�_�C�C���O���b�Z�[�W
class TDying : public TOBJ{
private:
public:
	TDying(TOBJList* owner);
	void Move();
	void Input();
};

//�����L���O��ʃN���X
class TRanking : public TOBJ{
private:
	s32 Choice;
	s32 PlayerScore;
	bool StickFlgX,StickFlgY; //�X�e�B�b�N��������Ă��邩
	s32 ActCount[10];
	s32 ActFlg[10];
	TPoint BarPos[100];
	s32 BlinkNum;    //�_�ł����鏇��
public:
	TRanking(TOBJList* owner, s32 _score);
	void Move();
	void Input();
	void RankIn();
};

//�l�b�g�����L���O��ʃN���X
class TNetRanking : public TOBJ{
private:
	s32 Choice;
	bool StickFlgX,StickFlgY; //�X�e�B�b�N��������Ă��邩
	s32 ActCount[10];
	s32 ActFlg[10];
	s32 BlinkNum;    //�_�ł����鏇��
	vector<NetRankingData> ScoreRanking;
	vector<NetRankingData> TimeRanking;
	s32 TotalScoreData;   //���X�R�A�f�[�^��
	s32 TotalTimeData;    //���^�C���f�[�^��
	s32 ConnectResult;    //�ڑ�����
	s32 OrgX,OrgY;    //�\���ʒu
	s32 DestX,DestY;  //�ړ���ʒu
	s32 XSpeed,YSpeed;   //�J�[�\���ړ��X�s�[�h
	s32 ScoreRankLine,TimeRankLine;   //�����L���O�ʒu
	s32 SameID_Score,SameID_Time;		//�d��ID���������ꍇ�A���̈ʒu
	vector<TMojiAndGlassCursor*> CursorList;           //���ڃ��X�g
	//    Blur:TBlur;
	s32 TotalScoreData_local;   //���X�R�A�f�[�^��
	s32 TotalTimeData_local;    //���^�C���f�[�^��
	s32 ScoreRankLine_local,TimeRankLine_local;   //�����L���O�ʒu
	TArrow* Arrow_Left;
	TArrow* Arrow_Right;
	TConnectingGauge* ConnectGauge;
	bool IsConnecting;
	s32 ErrorNo;
	void MakeRankingItems(const NetRankingData _rankdata[], s32 _totaldata, s32 _rankline, s32 _sameid);
	void MakePartition(TMojiAndGlassCursor* _cursor);
public:
	TNetRanking(TOBJList* owner);
	~TNetRanking();
	void Move();
	void Input();
	void RankIn();
	void RankIn_local();
	void RankIn_local2(NetRankingData scorerank[], NetRankingData timerank[], s32 size);
	void TestRankIn();
	void Die();  
	void MakeItemName();
	void MakeSentence(const string& _caption, s32 _x, s32 _y, s32 _delay);
	void SetNextState();
	void OnResponse( stringstream& _res );
};

//�l�b�g�����L���O���v���C�{���N���X
class TNetRankingReplayViewer : public TOBJ{
private:
	s32 Choice;
	bool StickFlgX,StickFlgY; //�X�e�B�b�N��������Ă��邩
	s32 ActCount[10];
	s32 ActFlg[10];
	vector<NetRankingData> RankingData;
	s32 TotalData;   //���f�[�^��
	s32 ConnectResult;    //�ڑ�����
	TGlassCursor* Cursor;  //�J�[�\��
	s32 OrgX,OrgY;    //�\���ʒu
	s32 DestX,DestY;  //�ړ���ʒu
	s32 SlideTime;  //�ړ�����t���[����
	s32 XSpeed,YSpeed;   //�J�[�\���ړ��X�s�[�h
	s32 KeyUpCount,KeyDownCount;  //�L�[�����t���[����
	vector<TMojiAndGlassCursor*> CursorList;           //���ڃ��X�g
	vector<TMojiAndGlassCursor*> BGItemList;           //�w�i�I�u�W�F�N�g�i���ږ��A�o�[�Ȃǁj���X�g
	s32 CurrentItems;          //���ݕ\�����Ă��鍀�ڐ�(�y�[�W�؂�ւ����ɗ��p)
	s32 CurrentItemNumber;  //���ݕ\�����Ă��鍀�ڔԍ�(�y�[�W�؂�ւ����ɗ��p)
	s32 CurrentStatus;      //���݂̉�ʑJ��
	s32 RankingMode;        //�X�R�A�^�^�C�������L���O�̋��
	TBustShot* BustShot_Rifle;
	TBustShot* BustShot_Blade;
	TConnectingGauge* ConnectGauge;
	bool IsConnecting;
	s32 CurrentStage;
	void AddNextItems();
	void Input();
	void GetData();
	s32 GetReplay(s32 _choice);
	void ShowDetail();
	void MakeRightItems();
	void MakeLeftItems();
	void SetNextStatus(s32 _next);
	void ClearScreen();
	void UpdateCurrentItems();
	void MakePartition(TMojiAndGlassCursor* _cursor);
	void MakeItemName();
	void MakeSentence(const string& _caption, s32 _x, s32 _y, s32 _delay);
	void ClearBGItems();
	void MakeCursor(const string& _caption, s32 _x, s32 _y, s32 _long, s32 _pat, s32 _font, s32 _delay);
	void UpdateBustShot();
	void ReceiveRanking(stringstream& _res);
	void ReceiveReplay(stringstream& _res);
	void CopyToLocal(s32 _choice);
	void DeleteData();
public:
	TNetRankingReplayViewer(TOBJList* owner);
	~TNetRankingReplayViewer();
	void Move();
	void Die(); 
	void OnResponse(stringstream& _res);
};

//�R���t�B�O��ʃ����C�N
class TConfig2 : public TOBJ{
private:
	s32 Choice;   //���Ԗڂ̃J�[�\�����I�΂�Ă���H
	bool StickFlgX,StickFlgY; //�X�e�B�b�N��������Ă��邩
	s32 ActCount[10];
	s32 ActFlg[10];
	s32 TempKey[4];  //�L�[�A�T�C���p
	vector<ConfigCursor>	SettingList;
	TGlassCursor* Cursor;  //�J�[�\��
	s32 OrgX,OrgY;    //�\���ʒu
	s32 KeyUpCount,KeyDownCount;  //�L�[�����t���[����
	vector<TMojiAndGlassCursor*> CursorList;           //���ڃ��X�g
	s32 CurrentItems;          //���ݕ\�����Ă��鍀�ڐ�
	s32 CurrentStatus;      //���݂̉�ʑJ��
	bool ButtonFlg_KeyConfig[16];
	TArrow* Arrow_Left;
	TArrow* Arrow_Right;
	TGlassCubeFilter* GlassCubes;
	s32 DecidedTime;
	bool tmpExEffects;
	TCursorList* Caution;      //�`�[�g�ݒ莞�̌x��
	void Input();
	void Endprocedure();
	void ClearToRight();
	void ClearToLeft();
	void SetNextStatus(s32 _next);
	void MakeCursor(const string& _caption, s32 _x, s32 _y, s32 _long, s32 _pat, bool _horizontal);
	void AddSetting(s32 _num, const string& _caption);
	void UpdateSetting(s32 _rx, s32 _ry, s32 _num, s32 _val, s32 _delay);
	void PrevStatus();
	void OnDecided();
	void IncorporateSettings();
	void KeyConfig();
	void MakeSentence(const string& _caption, s32 _x, s32 _y, s32 _delay);
	void ArrowControl();
	void CheckCheat();
public:
	TConfig2(TOBJList* owner);
	~TConfig2();
	void Move();
	void Die(); 
};

//�|�[�Y��ʃ����C�N
class TPause2 : public TOBJ{
private:
	s32 Choice;   //���Ԗڂ̃J�[�\�����I�΂�Ă���H
	bool StickFlg; //�X�e�B�b�N��������Ă��邩
	s32 ActCount[10];
	s32 ActFlg[10];
	TGlassCursor* Cursor;  //�J�[�\��
	s32 KeyUpCount,KeyDownCount;  //�L�[�����t���[����
	void Input();
	void OnDecided();
protected:
	TBlur* Blur;
	s32 OrgX,OrgY;    //�\���ʒu
	vector<TMojiAndGlassCursor*> CursorList;           //���ڃ��X�g
	s32 CurrentItems;          //���ݕ\�����Ă��鍀�ڐ�
	void MakeCursor(const string& _caption, s32 _x, s32 _y, s32 _num);
public:
	TPause2(TOBJList* owner);
	~TPause2();
	void Move();
	void Die();
};

//����I�����
class TWeaponSelect : public TOBJ{
private:
	s32 Choice;   //���Ԗڂ̃J�[�\�����I�΂�Ă���H
	bool StickFlgX, StickFlgY; //�X�e�B�b�N��������Ă��邩
	s32 ActCount[10];
	s32 ActFlg[10];
	s32 OrgX,OrgY;    //�\���ʒu
	s32 KeyUpCount,KeyDownCount;  //�L�[�����t���[����
	TSprSentence* Sentence[8];           //���ڃ��X�g
	s32 CurrentItems;          //���ݕ\�����Ă��鍀�ڐ�
	TBustShot* BustShot_Rifle;
	TBustShot* BustShot_Blade;
	s32 DecidedTime;
	s32 CurrentStage;
	TStSelCursor* StSel;
	s32 maxstage;
	void Input();
	void MakeCursor(const string& _caption, s32 _x, s32 _y, s32 _num);
	void OnDecided();
	void UpdateBustShot();
	void Slide();
public:
	TWeaponSelect(TOBJList* owner);
	void Move();
	void Die();
	bool IsSkipRequied();
};

//���U���g��ʃ����C�N
class TResult2 : public TOBJ{
private:
	//    Blur:TBlur;
	s32 destbonus,timebonus,lifebonus,ndbonus,hitbonus,total;
	s32 LifeRate; //�c�胉�C�t����
	TCursorList* CursorList;
	void MakeItem(const string& _caption, const string& _mul, s32  _score, s32 _num);
public:
	TResult2(TOBJList* owner);
	~TResult2();
	void Move();
	void Die();
	void Input();
};

//�l�b�g�ڑ��m�F���
class TConnectAsk : public TOBJ{
private:
	TCursorList* CursorList;
	TGlassCursor* Cursor;
	bool StickFlg;
	void OnDecided();
	void Input();
public:
	TConnectAsk(TOBJList* owner);
	~TConnectAsk();
	void Move();
	void Die();
};

//���[�J�������L���O��ʃN���X
class TLocalRanking : public TOBJ{
private:
	bool StickFlgX,StickFlgY; //�X�e�B�b�N��������Ă��邩
	s32 ActCount[10];
	s32 ActFlg[10];
	vector<TMojiAndGlassCursor*> CursorList;           //���ڃ��X�g
	s32 OrgX,OrgY;    //�\���ʒu
	s32 DestX,DestY;  //�ړ���ʒu
	s32 XSpeed,YSpeed;   //�J�[�\���ړ��X�s�[�h
	s32 TotalScoreData_local[MAXSTAGE];   //���X�R�A�f�[�^��
	s32 TotalTimeData_local[MAXSTAGE];    //���^�C���f�[�^��
	TArrow* Arrow_Left;
	TArrow* Arrow_Right;
	s32 CurrentStage;
	s32 CurrentItems;
	void MakeRankingItems(NetRankingData _rankdata[], s32 _totaldata);
	void MakeItemName();
	void MakeSentence(const string& _caption, s32 _x, s32 _y, s32 _delay);
	void Input();
	void ChangeOrder(s32 _order);
public:
	TLocalRanking(TOBJList* owner);
	~TLocalRanking();
	void Move();
	void Die();  
};

//�l�[���G���g���[�����C�N
class TNameEntry2 : public TOBJ{
private:
	bool StickFlgX,StickFlgY; //�X�e�B�b�N��������Ă��邩
	s32 ActCount[10];
	s32 ActFlg[10];
	TCursorList* CursorList;           //���ڃ��X�g
	s32 SelectName[8];
	bool PlayFlg; //�l�[���G���g���[��ɃQ�[���X�^�[�g���邩
	s32 KeyUpCount,KeyDownCount;  //�L�[�����t���[����
	s32 KeyLeftCount,KeyRightCount;  //�L�[�����t���[����
	TGlassCursor* Cursor;
	TCursorList* CursorList_Name; //���O�\���p�J�[�\�����X�g
	s32 DecidedTime;
	TGlassCursor* Cursor_Name;
	TGlassCubeFilter* GlassCubes;
	void Input();
	void OnDecided();
	void MakeChar(s32 _moji, s32 _x, s32 _y, s32 _num);
	void UpdateName(bool _blink);
	void Back();
	void EndProcedure();
public:
	TNameEntry2(TOBJList* owner, bool _play);
	~TNameEntry2();
	void Move();
	void Die();
};

//���v���C�ۑ��m�F��ʃ����C�N
class TReplayAsk2 : public TOBJ{
private:
	TCursorList* CursorList;
	TGlassCursor* Cursor;
	bool StickFlg;
	void OnDecided();
	void Input();
public:
	TReplayAsk2(TOBJList* owner);
	~TReplayAsk2();
	void Move();
	void Die();
};

//���[�J�����v���C�{����ʃ����C�N
class TLocalReplayViewer : public TOBJ{
private:
	s32 Choice;
	bool StickFlgX,StickFlgY; //�X�e�B�b�N��������Ă��邩
	s32 ActCount[10];
	s32 ActFlg[10];
	s32 TotalData;   //���f�[�^��
	TGlassCursor* Cursor;
	s32 OrgX,OrgY;    //�\���ʒu
	s32 DestX,DestY;  //�ړ���ʒu
	s32 SlideTime;  //�ړ�����t���[����
	s32 XSpeed,YSpeed;   //�J�[�\���ړ��X�s�[�h
	s32 KeyUpCount,KeyDownCount;  //�L�[�����t���[����
	vector<TMojiAndGlassCursor*> CursorList;           //���ڃ��X�g
	vector<TMojiAndGlassCursor*> BGItemList;           //�w�i�I�u�W�F�N�g�i���ږ��A�o�[�Ȃǁj���X�g
	s32 CurrentItems;          //���ݕ\�����Ă��鍀�ڐ�(�y�[�W�؂�ւ����ɗ��p)
	s32 CurrentItemNumber;  //���ݕ\�����Ă��鍀�ڔԍ�(�y�[�W�؂�ւ����ɗ��p)
	s32 CurrentStatus;      //���݂̉�ʑJ��
	TBustShot* BustShot_Rifle;
	TBustShot* BustShot_Blade;
	void AddNextItems();
	void Input();
	s32 GetReplay(s32 _choice);
	void ShowDetail();
	void MakeRightItems();
	void MakeLeftItems();
	void SetNextStatus(s32 _next);
	void ClearScreen();
	void UpdateCurrentItems();
	void MakeItemName();
	void MakeSentence(const string& _caption, s32 _x, s32 _y, s32 _delay);
	void ClearBGItems();
	void MakeCursor(const string& _caption, s32 _x, s32 _y, s32 _long, s32 _pat, s32 _font, s32 _delay);
	void UpdateBustShot();
	void GetData();
public:
	TLocalReplayViewer(TOBJList* owner);
	~TLocalReplayViewer();
	void Move();
	void Die(); 
};

//�`���[�g���A��
class TTutorial : public TOBJ{
private:
	TCursorList* CursorList;
	bool ButtonFlg_local;
	void OnDecided();
	void Input();
public:
	TKeyBoard* KeyBoard;
	TTutorial(TOBJList* owner);
	~TTutorial();
	void Move();
	void Die();
	void UpdateMessage(const string& _msg, s32 _x, s32 _y, s32 _blank, s32 _width, s32 _num);
};

//�X�e�[�W�P�J�n�O�f��
class TStage1StartingDemo : public TOBJ{
private:
	TFriend* friends[10];
	TMotherShip* mother;
	TDemoEraser* eraser;
	bool ButtonFlg_local;
	void OnEraser();
	void Input();
	void Skip();
	void SetNextState();
	void Makefriends();
public:
	TStage1StartingDemo(TOBJList* owner);
	~TStage1StartingDemo();
	void Move();
};

//�X�e�[�W2�J�n�O�f��
class TStage2StartingDemo : public TOBJ{
private:
	TFriend* friends[10];
	TFriend* captain;
	bool ButtonFlg_local;
	TStage2ScrollBlur* Blur;
	s32 boostedcount;
	void Input();
	void Skip();
	void SetNextState();
	void Makefriends();
	void ReseTFriendPos();
public:
	TStage2StartingDemo(TOBJList* owner);
	~TStage2StartingDemo();
	void Move();
};

//�X�e�[�W3�J�n�O�f��
class TStage3StartingDemo : public TOBJ{
private:
	TFriend* friends[5];
	bool ButtonFlg_local;
	s32 boostedcount;
	void Input();
	void Skip();
	void SetNextState();
	void MakeFriends();
	void ResetFriendPos();
public:
	TStage3StartingDemo(TOBJList* owner);
	~TStage3StartingDemo();
	void Move();
};

//�X�e�[�W4�J�n�O�f��
class TStage4StartingDemo : public TOBJ{
private:
	TFriend* friends[3];
	bool ButtonFlg_local;
	s32 boostedcount;
	void Input();
	void Skip();
	void SetNextState();
	void Makefriends();
	void ResetFriendPos();
public:
	TStage4StartingDemo(TOBJList* owner);
	~TStage4StartingDemo();
	void Move();
};


//�����̓f��
class TSubmarineDemo : public TOBJ{
private:
	bool ButtonFlg_local;
	void Input();
	void Skip();
public:
	TSubmarineDemo(TOBJList* owner);
	void Move();
	void Die();
};

//�X�e�[�W4�I���f��
class TStage4EndingDemo : public TOBJ{
private:
	bool ButtonFlg_local;
	s32 ActFlg;
	void SetNextState();
public:
	TStage4EndingDemo(TOBJList* owner);
	~TStage4EndingDemo();
	void Move();
};

//�X�e�[�W5�J�n�O�f��
class TStage5StartingDemo : public TOBJ{
private:
	TFriend* friends[5];
	TFriend* captain;
	bool ButtonFlg_local;
	s32 boostedcount;
	void Input();
	void Skip();
	void SetNextState();
	void MakeFriends();
	void ResetFriendPos();
public:
	TStage5StartingDemo(TOBJList* owner);
	~TStage5StartingDemo();
	void Move();
};

//�X�e�[�W�Z���N�g��ʐ���
class TStageSelectScreenMng : public TOBJ{
private:
	TSea* Stage2Sea;  //�C
	TUnderSea* Stage2UnderSea;  //
	TDawncloud* Stage2Cloud;  //�_
	s32 CurrentStage;
	f32 TotalSlide;
public:
	TStageSelectScreenMng(TOBJList* owner);
	~TStageSelectScreenMng();
	void Move();
	void ChangeStage(s32 _stage);
	s32 GetCurrentStage();
};

//�^�C�g����ʃ����C�N
class TTitle2 : public TOBJ{
private:
	s32 Choice;   //���Ԗڂ̃J�[�\�����I�΂�Ă���H
	bool StickFlgX,StickFlgY; //�X�e�B�b�N��������Ă��邩
	s32 ActCount[10];
	s32 ActFlg[10];
	s32 OrgX,OrgY;    //�\���ʒu
	s32 KeyUpCount,KeyDownCount;  //�L�[�����t���[����
	TCursorList* CursorList;           //���ڃ��X�g
	s32 CursorSpr,ShadowSpr;
	f32 CursorZoom, ShadowZoom;
	TSprSentence* versionSpr;
	TSprSentence* copyright;
	s32 ButtonUpCount;    //�{�^���������ĂȂ�����
	void Input();
	void OnDecided();
	void MakeItems();
	void CursorMove(s32 _num);
public:
	TTitle2(TOBJList* owner);
	~TTitle2();
	void Move();
	void Die(); 

	static const s32 FirstY = 224;
	static const s32 Ybrank = 28;
};

//�񕜉��
class TRecover : public TOBJ{
private:
	TCursorList* CursorList;
	s32 tempScore;
	TGothic_counter* ScoreSprite;
	s32 Deposit;
	s32 HPNorma, MSNorma, MisNorma;
	s32 HPNext, MSNext, MisNext;
	void CalcRecover(s32 _val);
	void MakeItem(const string& _caption, const string& _mul, const string& _unit, s32 _score, s32 _num);
public:
	TRecover(TOBJList* owner);
	~TRecover();
	void Move();
	void Die();
	void Input();
};

//�t�F�[�h�A�E�g�����̖ʂ�
class TFadeOuttoNextStage : public TOBJ{
private:
public:
	TFadeOuttoNextStage(TOBJList* owner);
	void Move();
};

//��Փx�I�����
class TDifficultySelect : public TOBJ{
private:
	TCursorList* CursorList;
	TGlassCursor* Cursor;
	bool StickFlg;
	void OnDecided();
	void Input();
public:
	TDifficultySelect(TOBJList* owner);
	~TDifficultySelect();
	void Move();
	void Die();
};

#define		PSSMode    0

//title
#define		TIni       0

//BarPos
#define		PTitle     0

//config
#define		CKey0      0
#define		CKey1      1
#define		CKey2      2
#define		CKey3      3
#define		CBoost     4
#define		CFScreen   5
#define		CFilter    6
#define		CCloud     7  //����񂩂�
#define		CExit      7

//�ڑ�����
#define		KCONNECT_SUCCESS         0  //����
#define		KCONNECT_NONE            1  //���ڑ�
#define		KCONNECT_ERROR           -1  //�ڑ��G���[
#define		KCONNECT_NOTFOUND        -2  //�t�@�C���Ȃ�

#define		FirstYPos				104        // �J�[�\�������ʒu
#define		YSPACE					 40            // �J�[�\���Ԋu
#define		YSPACE_HALF				 (YSPACE / 2);            // �J�[�\���Ԋu
#define		FirstYPos_Ranking		 104        // �����L���O�@�J�[�\�������ʒu
#define		FirstYPos_LocalRanking	 56        // ���[�J�������L���O�@�J�[�\�������ʒu


//�l�b�g�����L���O�S��
#define		KNETRANK_COMMOM_MAXRANK         100   //�N���C�A���g���ŕ\���ł���ő叇��

//�l�b�g�����L���O(�v���C�I����)���ڐ�
#define		KNETRANK_PLAY_MAX				 8   //�ő�\����
#define		KNETRANK_PLAY_RANKIN             4    //�����N�C�������ꍇ�A���Ԗڂɕ\�����邩

//�l�b�g�����L���O(���v���C�{��)���ڐ�
#define		KNETRANK_REP_MAX             8   //�ő�\����

//�l�b�g�����L���O(���v���C�{��)��ʑJ��
#define		KNETRANK_SCR_STAGE           0        //�X�e�[�W�I��
#define		KNETRANK_SCR_ORDER           1        //�X�R�A���^�^�C�����I��
#define		KNETRANK_SCR_RANKING         2        //�����L���O���
#define		KNETRANK_SCR_DETAIL          3        //�ڍ׉��
#define		KNETRANK_SCR_DETAIL_RANK     4        //�ڍׁ������L���O�֖߂�
#define		KNETRANK_SCR_ERROR           5        //�ڑ��G���[
#define		KNETRANK_SCR_NOTFOUND        6        //�t�@�C���Ȃ�

//�R���t�B�O��ʑJ��
#define		KCONFIG_SCR_TOP             0        //
#define		KCONFIG_SCR_GAMESETTING     1        //
#define		KCONFIG_SCR_CONTROL         2
#define		KCONFIG_SCR_GRAPHIC         3        //
#define		KCONFIG_SCR_SOUND           4        //
#define		KCONFIG_SCR_NETWORK         5        //

//�E�F�|���Z���N�g�p�萔
#define		KWSELECT_CURSOR_TOP         240        //
#define		KWSELECT_CURSOR_WIDTH       180        //
#define		KWSELECT_CURSOR_LEFT        214

#define		MAX_FRIENDS					(sizeof(friends)/sizeof(TFriend*))