#pragma once

using namespace std;

#include <string>
#include <vector>
#include <list>
#include "types.h"
#include "SXLib9.h"
#include "Define.h"
#include "Wave.h"
#include "JoyScan.h"
#include "Replay.h"

class TOBJ;
class TThread;
class TBG3DManager;
class TSoundEffect;
class TMusicPlayer;
class TReplay;
class TSpriteBase;

//�I�u�W�F�N�g�Ǘ��N���X
class	TOBJList {
private:
	TDGCarad*	m_dddd;
	list<TOBJ*> m_objList;
	TSpriteEXManager*	m_SprMan;
	TSceneManager*		m_Scene;
	TBG3DManager*		BG3DMan;

	s32 FTick;      //�Q�[���J�n���牽�t���[���o�������H
	s32 CDownTick;       //�J�E���g�_�E�����͂��߂鎞��
	//�{�X�e�N�X�`���ۑ��T�[�t�F�X
	// BossBmp:TBitmap;
	//BossBumpBmp:TBitmap;

	bool	DataLoaded[100];
	s32		OBJIDList[4096];     //ID���X�g
	s32		QuakePower;   //��ʗh�炵��
	u32		TotalRendCount,TotalFrameCount;    //����FPS�Z�o�p
	bool	QuakeFlg;   //�I�u�W�F�N�g��h�炷
	s32		QuakeTime; //�h�炵�n�߂�����
	s32 	QuakeLife; //�h��鎞��
	u32		RondomSeed;
	TMainLoopEvent   AfterLoadingEvent;     //���[�h�������̃C�x���g���
	TOBJ*	LoadingScreen;
	TThread*	LoadThread;

	s32		StageScriptID;

	bool	IsRequestedDelBullets;

	s32		HUDHideCount;

	void Event();
	void EventReload();
	void Setup();
	void Hantei();
	void SetStage( s8 StageNum );
	void LoadStage( s8 StageNum, TMainLoopEvent _event );
	void ReleaseStage();
	void QuakeManager();
	void CameraReset();
	void CameraAngleReset();
	void LoadSpriteData(const string& _file);
	void LoadMotionData(const string& _file);
	void CreateRestart();
	void InitPlayerGauge();
	void DeleteBullets();
	void OnAfterLoop();
	void HUDVisibleControl();
public:
	TOBJ*	DemoMan;
	s32		HiScore;   //�n�C�X�R�A
	TPoint	HeroPos;    //���@�̈ʒu
	bool	HeroDie;   //���@������
	s32		HeroHP;    //���@HP
	s32		TotalScore;//�X�R�A
	bool	Muteki;    //���G�t���O
	bool	Weapon;    //����g�p�̉�
	WaveSet WS;
	WaveSet	WS_Seq[4];
	s32		TotalLock; //���b�N�I�����Ă��鐔
	s32		LockID[MaxLock*MissileStock];   //���b�N�����I�u�W�F�N�g��ID
	TPoint	BeLocked[MaxLock*MissileStock];   //���b�N�I�����ꂽ�I�u�W�F�N�g�̈ʒu
	bool	UnLock[MaxLock*MissileStock];     //���b�N�I�����͂��ꂽ
	bool	MissileON; //�~�T�C���g�p��
	s32		RestMis;   //�c��~�T�C��
	s32		LimitTime; //��������
	s32		TotalObj;    //���I�u�W�F�N�g��
	s32		TotalHit;  //���q�b�g��
	bool	TimeStop;   //����~�܂�
	TGameMode 	GameMode; //�Q�[���̏��-
	s32		Speed;      //�X�N���[���X�s�[�h
	s32		Distance;   //��s����
	TPoint	BGSlide;    //1�t���[��������̔w�i�X�N���[����
	s32		MShipHP;   //��̗͂̑�
	bool	ShowHit;   //�����蔻���\�����邩
	TFunSignal	FunSignal; //�t�@���l���ւ̎w��
	s8		FunPattern;  //�t�@���l���̍U���p�^�[��
	s32		FunSync;      //�t�@���l�������p�ϐ�
	bool	BossDie;      //�{�X������
	s32		BossRate;     //�{�X���_�{��
	s32		BDieTime;     //�{�X�����񂾎���
	s32		BossR;        //�{�X�̊p�x
	bool	DontBoost;    //�u�[�X�g�֎~
	TJoyPad	Joy;
	s32		PlayerName[8];  //�X�R�A�l�[��
	TMainLoopEvent AfterLoopEvent;
	s32		Z4AppX[100];  //�_�U�R�o���ʒu
	s32		Z4AppY[100];  //�_�U�R�o���ʒu
	s32		RandZYDist[8];  //�����_���U�R�o���ʒu�A�J�E���g
	s32		RandZCount[8];  //�����_���U�R�o���ʒu�A�J�E���g
	s32		FunXDist[256];    //�t�@���l���ʒu
	s32		FunYDist[256];    //�t�@���l���ʒu
	bool	DontRender;        //�����_�����O���Ă��悢��
	bool	ResultExists;      //���U���g��ʂ��o����
	bool	FlareOK;       //�����Y�t���A���o���Ă悢
	s8		HeroWeapon;  //���@�E�F�|���^�C�v

	TReplay*	Replayer;  //���v���C�N���X
	bool	ReplayON;  //���v���C�����H
	s32		NetReplayState;  //�ǂ̖ʂ̃l�b�g���v���C���Đ�����(���v���C�I����Ɉꗗ��ʂɖ߂邽��)

	bool	CheatON;   //�`�[�g�i���G�Ƃ��X���[�Ƃ��j�g�p����
	s8		NowStage;    //���݂̃X�e�[�W

	TSoundEffect*	SoundEffect;
	TMusicPlayer*	MusicPlayer;
	//TIdHTTP*	Network;
	s32		FrameSkipRate;

	bool	ScreenshotFlg;

	s32		ComBonusCount;    // ��ʏ�ɃR�}���_�[�{�[�i�X�����\������Ă��邩

	s32		TempCount[10];  //�G�p�ϐ�

	TGameSpeed 	GameSpeed;

	bool	DemoON;
	s32		DemoNumber;

	TOBJ*	JikiObj;     //���@�I�u�W�F�N�g�̃|�C���^

	bool	AcceptChangeSpeed;    //�X�N���[���X�s�[�h�ύX��Main���ōs����
	s32		RequestedSpeedFromChild;    //�O������X�s�[�h�ύX�˗����󂯂�ꍇ�̒l

	vector<TSpriteDataList> 	SpriteDataList;
	vector<TMotionData> 	MotionDataList;

	s32		StartDistance;  //�X�^�[�g���̔�s����

	bool	ResultSkipped;

	TOBJ*	BloomingFilter;

	TOBJ*	StageSelect;

	TRepFileData 	RankinHeaderData;

	s32		Difficulty;
	s32		SequenceTotalTime;

	bool	IsRecovering;

	bool	IsPractice;
	bool	IsCleared[MAXSTAGE_SEQ][2];    //�N���A�[���
	string	NetworkID;

	bool	HUDHideFlg;

	TOBJList( TDGCarad* _dddd );
	~TOBJList();
	//TOBJ* getOBJFromID(s32 id);		// �������p�~���邱��
	bool ConfigSave();
	bool ConfigLoad();
	string ScoreToStr(s32 _score);
	string PlayerNameToStr(const s32 _name[], s32 _size);
	string DateToStr(s32 _year, s32 _month, s32 _day);
	string TimeToStr(s32 _time);
	string FrameSkipRateToStr(s32 _fsr);
	s32 GetRand(s32 _max);
	void Move();
	void Render();
	void GameStart();
	void GameReset();
	void Sentence(string moji, s32 fx, s32 fy, s32 brank, s32 _pat);
	void SentenceCenter(const string& moji, s32 fx, s32 fy, s32 brank, s32 _pat);
	void SentenceRight(const string& moji, s32 fx, s32 fy, s32 brank, s32 _pat);
	void ShowScore(s32 score,s32 x, s32 y, bool _red);
	void ShowTime( s32 _x, s32 _y, s32 _time, bool _red);
	void Scrach(TOBJ* target);
	void LockOn(s32 setNumber);   //�ꊇ���b�N�I��
	void LockOn2(s32 setNumber, s32 MisNumber);   //�������b�N�I��
	void Input();
	void Quake(s32 _power, s32 _time);
	void Console();
	void PlayStart();
	void GoTitle();
	void ScanKey();
	void ReleaseBeforeReset();
	void InitAfterReset();
	void CulcFrameSkipRate();
	//    property Objs[id:Integer]:TOBJ read getOBJ;
	s32 GetTick() { return FTick; }
	void ScreenShot();
	void OnFrameSkipped();
	TDGTexture* GetExTexture();
	TDGTexture* GetAlphaTexture();
	TDGTexture* GetAddTexture();
	void PrepareBeforeDevLost();
	void ResetAfterDevLost();
	void CreateHUD();
	void InitBeforePlayStart();
	void TimeOver();
	void StageEnd();
	void SetBloomingColor(s32 _add, s32 _alpha, s32 _r, s32 _g, s32 _b);
	void SetBloomingColorSoon(s32 _add, s32 _alpha, s32 _r, s32 _g, s32 _b);
	void OnLoaded( void* Sender);
	void LoadInternal();
	void SetRankInHeader();
	bool IsSequence();
	void RequestDelBullets();
	bool IsNormalCleared;
	bool IsBossDefeated(s32 _stage);
	bool IsBossDefeated_Seq(s32 _stage);
	void Add(TOBJ* obj);
	void AddSprite(TSpriteBase* _spr);
	TSceneManager* GetScene() { return m_Scene; }

	TDGCarad* GetDG() { return m_dddd; }
	TSpriteEXManager*	GetSprMan() { return m_SprMan; }
	TBG3DManager*	GetBG3DMan() { return BG3DMan; }
};

typedef list<TOBJ*>::iterator	objlist_itr;

