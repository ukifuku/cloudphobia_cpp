#pragma once

using namespace std;

#include <string>
#include <sstream>
#include <iostream>
#include "types.h"
#include "Define.h"

//���v���C�f�[�^���
struct TRepFileData
{
	wstring FileName;
	s32 PlayerName[8];
	s32 Score;
	s32 Time;
	s8 BType;
	s8 WType;
	s32 FrameSkipRate;
	s32 Year;
	s32 Month;
	s32 Day;
	s8 Stage;

	s32 Difficulty;
	s32 OriginalScore;
	s32 DestBonusRate;
	s32 Life;
	s32 NoDamageBonus;
	s32 TotalHit;
	s32 ClearBonus;
};

#define MAX_TMP_REP	5
//���v���C�N���X
class TReplay
{
private:
	s8 HeroStickY[MaxFrame];
	s8 HeroStickX[MaxFrame];     //�W���C�p�b�h�̕����L�[
	bool HeroButton[3][MaxFrame];           //�{�^��
	stringstream TempReplay[MAX_TMP_REP];
public:
	s8 NowStickY,NowStickX;
	bool NowButton[3];
	bool Rendered[MaxFrame];    //���̃t���[���Ń����_�����O���ꂽ���i���������Č��p�j
	s8 RepBType;                                         //���v���C���̃u�[�X�g�^�C�v
	s8 RepWType;                                         //���v���C���̕���^�C�v
	TRepFileData RepFile[100];                    //���v���C�f�[�^���̍\����
	s32 TotalData;                                        //���v���C�f�[�^����
	s8 ReadRepVer;                                          //�ǂݍ��񂾃��v���C�f�[�^�̃o�[�W����
	s32 RepDifficulty;
	NetRankingData_Split SplitData;
	TReplay();
	~TReplay();
	void Rec(s32 _frame);
	void Play(s32 _frame);
	void Init();
	void Save(const TRepFileData& _data);
	void SaveToStream(stringstream& F, const TRepFileData& _data);
	void Load(const wstring& _filename);
	void ListUp();
	void LoadFromStream(istream& _stream, bool _fromfile);
	void OutPutText(const string& _filename);
	void CopyToLocal();
	void InitCurrentData();
	void SaveTempReplay(s32 _stage, const TRepFileData& _data);
	void SaveSequenceToStream(s32 _difficulty, string& _id, stringstream& F, const TRepFileData& _data, const NetRankingData_Split& _split);
	void SaveSequence(s32 _difficulty, const TRepFileData& _data, const NetRankingData_Split& _split);
	void Decode( istream& _stream);
	void LoadTempReplay(s32 _stage);
	void InitSplitData();
	void debugload();
};


