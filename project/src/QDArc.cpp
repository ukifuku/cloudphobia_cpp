#include "QDArc.h"
#include <sstream>
#include <fstream>
#include <algorithm>

typedef vector<TQDADataHeader> TQDADataHeaderArray;

//�W�J
stringstream Expand( istream& Src, u32 restoredLength)
{
	u8 left[256], right[256];
	u8 count;
	u32 c,size;
	stringstream dest;

	//src�̏I���܂ŁA�e�u���b�N��W�J
	while( ! Src.eof() ) {
		Src.read((char*)&count, 1);

		for( s32 i = 0 ; i <= 255 ; i++ )
			left[i] = i;

		//�y�A�\�̓ǂ݂���
		c = 0;
		while( true ) {
			//7�r�b�g�ڂ������Ă���Ȃ�A�\�̊i�[�ʒu���X�L�b�v
			if( count > 127 ) {
				c += count-127;
				count = 0;
			}
			if( (c == 256) )
				break;

			//�����łȂ��Ȃ�A���̌������y�A��ǂ�
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


		//���k���ꂽ�f�[�^�u���b�N�̃T�C�Y�����߂�
		Src.read((char*)&size, 4);

		//�f�[�^�u���b�N��W�J
		s32 i = 0;
		u8 b;
		u8 stack[256];
		while( true ) {
			//�X�^�b�N��Src����1�o�C�g�ǂ�
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

			//1�o�C�g�o�͂��邩�A�X�^�b�N�Ƀv�b�V��
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

//QDAFileHeader�̃`�F�b�N
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
		//�܂��A�w�b�_��ǂ�
		Source.read((char*)&qfh, sizeof(qfh));

		if( ! CheckFileHeader(qfh) ) {
			//ex = EQDAError.Create('Quadruple D Archive�̈�����f�[�^�ł͂���܂���');
			return false;
		}

		//�f�[�^�w�b�_�̓ǂ݂���
		qdhs = (TQDADataHeader*)malloc( sizeof(qdh) * qfh.DataCount );
		Source.read((char*)qdhs, sizeof(qdh)*qfh.DataCount);

		//ID�̌���
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

		//������Ȃ�
		if( Source.seekg(0) ) {
			//ex = EQDAError.CreateFmt('�A�[�J�C�u���ɂ͎w�肳�ꂽID ''%s'' �����f�[�^��������܂���',[ID]);
			return false;
		}

		//�f�[�^�̓ǂ݂���
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
