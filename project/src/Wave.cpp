#include "Wave.h"
#include "Define.h"
#include "QDArc.h"

void EWaveReset( WaveSet* WS )
{
	WS->EventCount = 0;
	WS->EventMaxCount = 0;
	for( s32 i = 0 ; i < 256; i++ ) {
		WS->Relative[i] = 0;
		WS->EventDistance[i] = 0;
		WS->EnemyType[i] = -1;
		WS->X[i] = 0;
		WS->Y[i] = 0;
		WS->OPT1[i] = 0;
		WS->OPT2[i] = 0;
		WS->OPT3[i] = 0;
		WS->FLG1[i] = false;
		WS->FLG2[i] = false;
	}

}

void EWaveTrans( WaveSet* WS, s32 _num, const string& _line )
{
	//���Βl���l��
	WS->Relative[_num] = stoi(ReadSegment(_line,0));
	//���Βl��0�ȉ��Ȃ�A�����l�͂��̂܂܋L�^
	if( WS->Relative[_num] <= 0 )
		WS->EventDistance[_num] = stoi(ReadSegment(_line,1));
	//���Βl���v���X�Ȃ�A�O�̍s�̋������玩���̋������v�Z����  
	else if( WS->Relative[_num] > 0 )
		WS->EventDistance[_num] = WS->EventDistance[_num-WS->Relative[_num]]+stoi(ReadSegment(_line,1));

	//���ߌ�ɉ����Ď��ʃR�[�h���l������
	string WCode = ReadSegment(_line,2);
	if( WCode == "warning" ) {
		WS->EnemyType[_num] = 1;
		WS->X[_num] =  stoi(ReadSegment(_line,3));
		WS->Y[_num] =  stoi(ReadSegment(_line,4));
		WS->OPT1[_num] = stoi(ReadSegment(_line,5));
		WS->OPT2[_num] = stoi(ReadSegment(_line,6));
	}
	else if( WCode == "s1zako1" ) {
		WS->EnemyType[_num] = 2;
		WS->X[_num] =  stoi(ReadSegment(_line,3));
		WS->Y[_num] =  stoi(ReadSegment(_line,4));
		WS->OPT1[_num] = stoi(ReadSegment(_line,5));
		WS->FLG1[_num] = ReadSegment(_line,6) == "true";
	}
	else if( WCode == "s1zako2" ) {
		WS->EnemyType[_num] = 3;
		WS->X[_num] =  stoi(ReadSegment(_line,3));
		WS->Y[_num] =  stoi(ReadSegment(_line,4));
		WS->OPT1[_num] = stoi(ReadSegment(_line,5));
	}
	else if( WCode == "s1zako3" ) {
		WS->EnemyType[_num] = 4;
		WS->X[_num] =  stoi(ReadSegment(_line,3));
		WS->Y[_num] =  stoi(ReadSegment(_line,4));
		WS->OPT1[_num] = stoi(ReadSegment(_line,5));
	}
	else if( WCode == "s1zako4" ) {
		WS->EnemyType[_num] = 5;
		WS->OPT1[_num] = stoi(ReadSegment(_line,3));
		WS->OPT2[_num] = stoi(ReadSegment(_line,4));
	}
	else if( WCode == "s1z4war" ) {
		WS->EnemyType[_num] = 6;
		WS->X[_num] =  stoi(ReadSegment(_line,3));
		WS->Y[_num] =  stoi(ReadSegment(_line,4));
		WS->FLG1[_num] = (ReadSegment(_line,5)) == "true";
	}
	else if( WCode == "s1zako5" ) {
		WS->EnemyType[_num] = 7;
		WS->X[_num] =  stoi(ReadSegment(_line,3));
		WS->Y[_num] =  stoi(ReadSegment(_line,4));
		WS->OPT1[_num] = stoi(ReadSegment(_line,5));
		WS->OPT2[_num] = stoi(ReadSegment(_line,6));
		WS->FLG1[_num] = (ReadSegment(_line,7)) == "true";
	}
	else if( WCode == "s1vup" ) {
		WS->EnemyType[_num] = 8;
	}
	else if( WCode == "s1vdown" ) {
		WS->EnemyType[_num] = 9;
	}
	else if( WCode == "s1boss" ) {
		WS->EnemyType[_num] = 10;
		WS->X[_num] =  stoi(ReadSegment(_line,3));
		WS->Y[_num] =  stoi(ReadSegment(_line,4));
	}

	//�C�x���g���ő�l���C���N�������g
	WS->EventMaxCount++;

}

void EWaveSort( WaveSet* WS )
{
	for( s32 i = WS->EventMaxCount ; i >= 0; i-- ) {
		//���Βl���}�C�i�X�̂Ƃ��A���ȍ~�̍s�̋������玩���̋������v�Z����
		if( WS->Relative[i] < 0 ) {
			WS->EventDistance[i] = WS->EventDistance[i-WS->Relative[i]]-WS->EventDistance[i];
		}
	}

}

void EWaveLoad( WaveSet* WS, const string& _file )
{
	s32 k = 0;
	EWaveReset(WS);

	std::string line;
	if( DebugMode ) {
		std::ifstream ifs(_file);

		// �e�L�X�g�t�@�C���ǂݍ���
		while (std::getline(ifs, line)) {
			EWaveTrans(WS, k++, line);
		}
		ifs.close();
	}
	else {
		//���k�t�@�C������ǂݍ���
		std::stringstream stream;
		ExtractFromQDAFile(stream, "./Data/script.BIN", _file);
		while (std::getline(stream, line)) {
			EWaveTrans(WS, k++, line);
		}
		stream.str("");
	}

	EWaveSort(WS);
}

