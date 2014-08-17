//アーカイブユニット
#pragma once

#include "types.h"
#include <vector>
#include <iostream>
#include <string>

using namespace std;
//QDAのフォーマット…
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
  u32 Compress;                   //圧縮してる？ 0=No 1=Yes
  s8 Signature[4];		    //シグネチャ 'QDA0'
  u32 DataCount;                  //データ数
  u8 Reserved[244];		// 1..244
};

struct TQDADataHeader 
{
  u32 Offset;                   //ファイルの「先頭」からのオフセット
  u32 Length;                   //データの長さ(格納後)
  u32 RestoredLength;           //データの長さ(解凍後)
  s8 ID[256];		       //ID
};


//コールバック関数
//typedef void (*TQDANotifyEvent)(s32 ,s32 , string&);

//アーカイブの作成
//void CreateQDAFile(string FileName, vector<string>& SourceFile, vector<string>& IDs, bool doCompress, TQDANotifyEvent callback);

//アーカイブからの抽出
bool ExtractFromQDAFile( stringstream& _out, string FileName, string ID );
bool ExtractFromQDAStream( stringstream& _out, istream& Source, string ID);

//アーカイブに入っているデータの情報を得る
//TQDADataHeader GetQDADataInfo(string FileName, u32 index);

//アーカイブの情報を得る
//TQDAFileHeader GetQDAFileInfo(string FileName);

