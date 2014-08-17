//�A�[�J�C�u���j�b�g
#pragma once

#include "types.h"
#include <vector>
#include <iostream>
#include <string>

using namespace std;
//QDA�̃t�H�[�}�b�g�c
/*

 OFS  |   SIZE   |  TYPE         | NAME      |   COMMENT
 ------------------------------------------------------------------
  +0    4           Char * 4       Signature    'QDA0'
  +4    4           DWORD          Compress?    0:No Comp.  1:Compressed
  +8    4           DWORD          DataCount    Number of Data
  +12   244         BYTE           Reserved     Reserved

  +256  268*nData   QDADataHeader  dataHeader   informations of each data
  ???   ??? *nData  ???            ---          DataBlock
*/

class EQDAError;

struct TQDAFileHeader
{
  u32 Compress;                   //���k���Ă�H 0=No 1=Yes
  s8 Signature[4];		    //�V�O�l�`�� 'QDA0'
  u32 DataCount;                  //�f�[�^��
  u8 Reserved[244];		// 1..244
};

struct TQDADataHeader 
{
  u32 Offset;                   //�t�@�C���́u�擪�v����̃I�t�Z�b�g
  u32 Length;                   //�f�[�^�̒���(�i�[��)
  u32 RestoredLength;           //�f�[�^�̒���(�𓀌�)
  s8 ID[256];		       //ID
};


//�R�[���o�b�N�֐�
//typedef void (*TQDANotifyEvent)(s32 ,s32 , string&);

//�A�[�J�C�u�̍쐬
//void CreateQDAFile(string FileName, vector<string>& SourceFile, vector<string>& IDs, bool doCompress, TQDANotifyEvent callback);

//�A�[�J�C�u����̒��o
bool ExtractFromQDAFile( stringstream& _out, string FileName, string ID );
bool ExtractFromQDAStream( stringstream& _out, istream& Source, string ID);

//�A�[�J�C�u�ɓ����Ă���f�[�^�̏��𓾂�
//TQDADataHeader GetQDADataInfo(string FileName, u32 index);

//�A�[�J�C�u�̏��𓾂�
//TQDAFileHeader GetQDAFileInfo(string FileName);

