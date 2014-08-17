#include "QDArc.h"
#include <sstream>
#include <fstream>
#include <algorithm>

typedef vector<TQDADataHeader> TQDADataHeaderArray;

//展開
stringstream Expand( istream& Src, u32 restoredLength)
{
	u8 left[256], right[256];
	u8 count;
	u32 c,size;
	stringstream dest;

	//srcの終わりまで、各ブロックを展開
	while( ! Src.eof() ) {
		Src.read((char*)&count, 1);

		for( s32 i = 0 ; i <= 255 ; i++ )
			left[i] = i;

		//ペア表の読みこみ
		c = 0;
		while( true ) {
			//7ビット目が立っているなら、表の格納位置をスキップ
			if( count > 127 ) {
				c += count-127;
				count = 0;
			}
			if( (c == 256) )
				break;

			//そうでないなら、その個数だけペアを読む
			for( s32 i = 0 ; i <= count ; i++) {
				Src.read((char*)&left[c], 1);
				if( c != left[c] )
					Src.read((char*)&right[c], 1);
				c++;
			}
			if( (c == 256) )
				break;

			Src.read((char*)&count, 1);
		}


		//圧縮されたデータブロックのサイズを求める
		Src.read((char*)&size, 4);

		//データブロックを展開
		s32 i = 0;
		u8 b;
		u8 stack[256];
		while( true ) {
			//スタックかSrcから1バイト読む
			if( i > 0 ) {
				i--;
				c = stack[i];
			} else {
				if( size == 0 )
					break;
				size--;
				Src.read((char*)&b,1);
				c = b;
			}

			//1バイト出力するか、スタックにプッシュ
			if( c == left[c] ) {
				b = c;
				dest.write((char*)&b, 1);
			}
			else {
				stack[i] = right[c];
				i++;
				stack[i] = left[c];
				i++;
			}

		}
	}

	dest.seekg(0);
	return dest;
}

//QDAFileHeaderのチェック
bool CheckFileHeader(const TQDAFileHeader& qfh)
{
	return 
		(qfh.Signature[0]=='Q') &&
		(qfh.Signature[1]=='D') &&
		(qfh.Signature[2]=='A') &&
		(qfh.Signature[3]=='0');
}



bool ExtractFromQDAFile( stringstream& _out, string FileName, string ID )
{
	ifstream fs;

	try{
		fs.open(FileName);
	}
	catch(ifstream::failure e){
		return false;
	};

	return ExtractFromQDAStream(_out, fs, ID);
}

bool ExtractFromQDAStream( stringstream& _out, istream& Source, string ID )
{
	TQDAFileHeader qfh;
	TQDADataHeader qdh;
	TQDADataHeader* qdhs = NULL;
	try{
		//まず、ヘッダを読む
		Source.read((char*)&qfh, sizeof(qfh));

		if( ! CheckFileHeader(qfh) ) {
			//ex = EQDAError.Create('Quadruple D Archiveの扱えるデータではありません');
			return false;
		}

		//データヘッダの読みこみ
		qdhs = (TQDADataHeader*)malloc( sizeof(qdh) * qfh.DataCount );
		Source.read((char*)qdhs, sizeof(qdh)*qfh.DataCount);

		//IDの検索
		Source.seekg(0);
		for( u32 i = 0 ; i < qfh.DataCount; i++) {
			string lower1(string(qdhs[i].ID));
			string lower2 = ID;
			transform(lower1.begin(), lower1.end(), lower1.begin(), tolower);
			transform(lower2.begin(), lower2.end(), lower2.begin(), tolower);
			if( lower1 == lower2 ) {
				qdh = qdhs[i];
				Source.seekg(qdhs[i].Offset);
				break;
			}
		}

		//見つからない
		if( Source.seekg(0) ) {
			//ex = EQDAError.CreateFmt('アーカイブ内には指定されたID ''%s'' を持つデータが見つかりません',[ID]);
			return false;
		}

		//データの読みこみ
		if( qfh.Compress == 1 ) {
			_out = Expand( Source, qdh.RestoredLength);
		} else {
			char* buff = (char*)malloc(qdh.RestoredLength);
			Source.read(buff, qdh.RestoredLength);

			_out.write(buff, qdh.RestoredLength);

			SAFE_FREE(buff);
		}

		SAFE_FREE(qdhs);
	}
	catch(istream::failure e)
	{
		if( qdhs )
			SAFE_FREE(qdhs);

		return false;
	}

	return true;
}
