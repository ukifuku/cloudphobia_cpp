#include "Replay.h"
#include <direct.h>
#include <time.h>

TReplay::TReplay()
{
	Init();
	RepBType = 0;
	for( s32 i  =  0 ; i < MAX_TMP_REP ; i++ )
		TempReplay[i] =  stringstream("");

	for( s32 i = 0 ; i < 100; i++ ) {
		RepFile[i].FileName = L"";
		for( s32 k = 0 ; i < 8; i++ )
			RepFile[i].PlayerName[k] = 47;
		RepFile[i].Score = 0;
		RepFile[i].Time = 0;
		RepFile[i].BType = 0;
		RepFile[i].FrameSkipRate = 0;
		RepFile[i].Stage = 0;
	}
	TotalData = 0;
	ReadRepVer = 0;
}

TReplay::~TReplay()
{
}

void TReplay::Rec( s32 _frame )
{
	if( _frame<MaxFrame ) {
		HeroStickY[_frame] = NowStickY;
		HeroStickX[_frame] = NowStickX;
		for( s32 i = 0 ; i <= 2; i++ )
			HeroButton[i][_frame] = NowButton[i];
	}
}

void TReplay::Play( s32 _frame )
{
	if( _frame<MaxFrame ) {
		NowStickY = HeroStickY[_frame];
		NowStickX = HeroStickX[_frame];
		for( s32 i = 0 ; i <= 2; i++ )
			NowButton[i] = HeroButton[i][_frame];
	}
}

void TReplay::Init()
{
	for( s32 i = 0 ; i < MaxFrame; i++ ) {
		HeroStickY[i] = 0;
		HeroStickX[i] = 0;
		for( s32 k = 0 ; k <= 2; k++ )
			HeroButton[k][i] = false;
		Rendered[i] = true;
	}
	NowStickY = 0;
	NowStickX = 0;
	for( s32 i = 0 ; i <= 2; i++ )
		NowButton[i] = false;
}

void TReplay::Save( const TRepFileData& _data )
{
	_mkdir("Replay");

	string filename;
	for(s32 i=0; i<100000; i++){
		s8 c[5];
		sprintf_s(c, "%5d", i );
		string FileNum(c);

		filename = string("Replay/") + string("CPRep") + FileNum + string(".cpr");
		FILE *fp;
		if ( (fopen_s(&fp, filename.c_str(),"r")) != EINVAL ){
			fclose( fp );

			if(i>=9999) return;
		}
		else{
			break;
		}
	}

	// バイナリファイル書き込み
	ofstream ofs(filename);
	if( ofs )
	{
		s8 tempver  =  28;
		ofs.write((char*)&tempver, sizeof(s8));

		//プレイヤーデータ
		for( s32 i = 0 ; i <= 7; i++ )
			ofs.write((char*)&_data.PlayerName[i], sizeof(s32));

		ofs.write((char*)&_data.Score, sizeof(s32));
		ofs.write((char*)&_data.Time, sizeof(s32));
		ofs.write((char*)&_data.BType, sizeof(s8));
		ofs.write((char*)&_data.WType, sizeof(s8));
		ofs.write((char*)&_data.FrameSkipRate, sizeof(s32));

		//日付
		time_t timer;
		struct tm local;
		timer = time(NULL);

		localtime_s(&local, &timer); 

		s32 Year = local.tm_year + 1900;
		s32 Month = local.tm_mon + 1;
		s32 Day = local.tm_mday;
		ofs.write((char*)&Year, sizeof(s32));
		ofs.write((char*)&Month, sizeof(s32));
		ofs.write((char*)&Day, sizeof(s32));

		ofs.write((char*)&_data.Stage, sizeof(s8));

		//圧縮しつつ保存　　圧縮はランレングス式
		ofs.write((char*)&HeroStickY[0], sizeof(s8));
		s16 SameNum = 1;
		for( s32 i = 1 ; i < MaxFrame; i++ ) {
			if( HeroStickY[i]==HeroStickY[i-1] ) {
				SameNum++;
			}
			else {
				ofs.write((char*)&SameNum, sizeof(s16));
				ofs.write((char*)&HeroStickY[i], sizeof(s8));
				SameNum = 1;
			}
			if( i == MaxFrame-1 )
				ofs.write((char*)&SameNum, sizeof(s16));
		}

		ofs.write((char*)&HeroStickX[0], sizeof(s8));
		SameNum = 1;
		for( s32 i = 1 ; i < MaxFrame; i++ ) {
			if( HeroStickX[i] == HeroStickX[i-1] ) {
				SameNum++;
			}
			else {
				ofs.write((char*)&SameNum, sizeof(s16));
				ofs.write((char*)&HeroStickX[i], sizeof(s8));
				SameNum = 1;
			}
			if( i == MaxFrame-1 )
				ofs.write((char*)&SameNum, sizeof(s16));
		}

		for( s32 k = 0 ; k <= 2; k++ ) {
			ofs.write((char*)&HeroButton[k][0], sizeof(bool));
			SameNum = 1;
			for( s32 i = 1 ; i < MaxFrame; i++) {
				if( HeroButton[k][i] == HeroButton[k][i-1] ) {
					SameNum++;
				}
				else {
					ofs.write((char*)&SameNum, sizeof(s16));
					ofs.write((char*)&HeroButton[k][i], sizeof(bool));
					SameNum = 1;
				}
				if( i=MaxFrame-1 )
					ofs.write((char*)&SameNum, sizeof(s16));
			}
		}

		ofs.write((char*)&Rendered[0], sizeof(bool));
		SameNum = 1;
		for( s32 i = 1 ; i < MaxFrame; i++ ) {
			if( Rendered[i] == Rendered[i-1] ) {
				SameNum++;
			}
			else {
				ofs.write((char*)&SameNum, sizeof(s16));
				ofs.write((char*)&Rendered[i], sizeof(bool));
				SameNum = 1;
			}
			if( i == MaxFrame-1 )
				ofs.write((char*)&SameNum, sizeof(s16));
		}

		ofs.close();
	}
}

void TReplay::SaveToStream( stringstream& F, const TRepFileData& _data )
{
	F.write((char*)&RepVersion, sizeof(s8));

	//プレイヤーデータ
	for( s32 i = 0 ; i <= 7; i++ )
		F.write((char*)&_data.PlayerName[i],sizeof(s32));

	F.write((char*)&_data.Score,sizeof(s32));
	F.write((char*)&_data.Time,sizeof(s32));
	F.write((char*)&_data.BType,sizeof(s8));
	F.write((char*)&_data.WType,sizeof(s8));
	F.write((char*)&_data.FrameSkipRate,sizeof(s32));

	F.write((char*)&_data.Stage,sizeof(s8));

	F.write((char*)&_data.OriginalScore,sizeof(s32));
	F.write((char*)&_data.DestBonusRate,sizeof(s32));
	F.write((char*)&_data.Life,sizeof(s32));
	F.write((char*)&_data.NoDamageBonus,sizeof(s32));
	F.write((char*)&_data.TotalHit,sizeof(s32));
	F.write((char*)&_data.ClearBonus,sizeof(s32));

	//圧縮しつつ保存　　圧縮はランレングス式
	F.write((char*)&HeroStickY[0],sizeof(s8));

	s16 SameNum = 1;
	for( s32 i = 1 ; i < MaxFrame; i++ ) {
		if( HeroStickY[i] == HeroStickY[i-1] ) {
			SameNum++;
		}
		else {
			F.write((char*)&SameNum, sizeof(s16));
			F.write((char*)&HeroStickY[i], sizeof(s8));
			SameNum = 1;
		}
		if( i == MaxFrame-1 )
			F.write((char*)&SameNum,sizeof(s16));
	}

	F.write((char*)&HeroStickX[0], sizeof(s8));
	SameNum = 1;
	for( s32 i = 1 ; i < MaxFrame; i++ ) {
		if( HeroStickX[i] == HeroStickX[i-1] ) {
			SameNum++;
		}
		else {
			F.write((char*)&SameNum, sizeof(s16));
			F.write((char*)&HeroStickX[i], sizeof(s8));
			SameNum = 1;
		}
		if( i == MaxFrame-1 )
			F.write((char*)&SameNum, sizeof(s16));
	}

	for( s32 k = 0 ; k <= 2; k++ ) {
		F.write((char*)&HeroButton[k][0],sizeof(bool));
		SameNum = 1;
		for( s32 i = 1 ; i < MaxFrame; i++ ) {
			if( HeroButton[k][i] == HeroButton[k][i-1] ) {
				SameNum++;
			}
			else {
				F.write((char*)&SameNum, sizeof(s16));
				F.write((char*)&HeroButton[k][i], sizeof(bool));
				SameNum = 1;
			}
			if( i == MaxFrame-1 )
				F.write((char*)&SameNum, sizeof(s16));
		}
	}

	F.write((char*)&Rendered[0], sizeof(bool));
	SameNum = 1;
	for( s32 i = 1 ; i < MaxFrame; i++ ) {
		if( Rendered[i] == Rendered[i-1] ) {
			SameNum++;
		}
		else {
			F.write((char*)&SameNum, sizeof(s16));
			F.write((char*)&Rendered[i], sizeof(bool));
			SameNum = 1;
		}
		if( i == MaxFrame-1 )
			F.write((char*)&SameNum,sizeof(s16));
	}

	//   F.Position  =  0;
	//    _network.Post("http://localhost/test.pl", F, _res);

}

void TReplay::Load( const wstring& _filename )
{
	// バイナリファイル読み込み
	ifstream ifs( wstring(L"Replay/") + _filename);

	if( ifs )
	{
		LoadFromStream(ifs, true);
	}

}

void TReplay::ListUp()
{
//SR:TSearchRec;
//F:TFileStream;
//ReadVersion:s8;
	//リスト初期化
	for( s32 i = 0 ; i < 100; i++ ) {
		RepFile[i].FileName = L"";
		for( s32 k = 0 ; k <= 7; k++ )
			RepFile[i].PlayerName[k] = 47;
		RepFile[i].Score = 0;
		RepFile[i].Time = 0;
		RepFile[i].BType = 0;
		RepFile[i].WType = 0;
		RepFile[i].FrameSkipRate = 0;
		RepFile[i].Year = 0;
		RepFile[i].Month = 0;
		RepFile[i].Day = 0;
		RepFile[i].Stage = 0;
		RepFile[i].Difficulty = 3;
	}
	TotalData = 0;

	s32 ReadCount = 0;
	
	//////////////////////////////
	// 宣言
	vector<wstring> fileList;
	HANDLE hFind;
	WIN32_FIND_DATA fd;

	// ファイル名検索のためにワイルドカード追加
	// 例 : "D:\\Users\\Pictures\\*.*"
	wstringstream ss;
	wstring folder(L"./Replay/*.cpr");
	ss << folder;

	// ファイル探索
	// FindFirstFile(ファイル名, &fd);
	hFind = FindFirstFile(ss.str().c_str(), &fd);

	// 検索失敗
	if(hFind == INVALID_HANDLE_VALUE){
		return;
	}

	// ファイル名をリストに格納するためのループ
	do{
		// フォルダは除く
		if(!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			&& !(fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
		{
			//ファイル名をリストに格納
			WCHAR *file = fd.cFileName;
			wstring filename = file;
			// バイナリファイル読み込み
			ifstream ifs(filename);
			s8 ReadVersion;
			ifs.read((char*)&ReadVersion, sizeof(s8));
			if( ReadVersion>=(28) ) {  //バージョンが違ってたら読まない
				RepFile[ReadCount].FileName = filename;
				for( s32 i = 0 ; i <= 7; i++ )
					ifs.read((char*)&RepFile[ReadCount].PlayerName[i],sizeof(s32));
				ifs.read((char*)&RepFile[ReadCount].Score,sizeof(s32));
				ifs.read((char*)&RepFile[ReadCount].Time,sizeof(s32));
				ifs.read((char*)&RepFile[ReadCount].BType,sizeof(s8));
				ifs.read((char*)&RepFile[ReadCount].WType,sizeof(s8));
				ifs.read((char*)&RepFile[ReadCount].FrameSkipRate,sizeof(s32));
				ifs.read((char*)&RepFile[ReadCount].Year,sizeof(s32));
				ifs.read((char*)&RepFile[ReadCount].Month,sizeof(s32));
				ifs.read((char*)&RepFile[ReadCount].Day,sizeof(s32));
				ifs.read((char*)&RepFile[ReadCount].Stage,sizeof(s8));
				if( ReadVersion>=29 )
					ifs.read((char*)&RepFile[ReadCount].Difficulty,sizeof(s32));
				TotalData++;
				ReadCount++;
			}
			ifs.close();
			if( ReadCount>99 ) break;
		}
	}while(FindNextFile(hFind, &fd)); //次のファイルを探索

	// hFindのクローズ
	FindClose(hFind); 

}

void TReplay::LoadFromStream( istream& _stream, bool _fromfile )
{
	ReadRepVer = 0;
	s8 ReadStage;
	_stream.read((char*)&ReadRepVer,sizeof(s8));

	s32 DummyRead;
	if( (ReadRepVer>=27) && (ReadRepVer<=28) ) {  //バージョンが違ってたら読まない
		for( s32 i = 0 ; i <= 9 ; i++)
			_stream.read((char*)&DummyRead,sizeof(s32));

		_stream.read((char*)&RepBType,sizeof(s8));
		_stream.read((char*)&RepWType,sizeof(s8));
		_stream.read((char*)&DummyRead,sizeof(s32));

		if( _fromfile ) {
			//ファイルから読み込む場合は日付データが入っているので飛ばす
			_stream.read((char*)&DummyRead,sizeof(s32));
			_stream.read((char*)&DummyRead,sizeof(s32));
			_stream.read((char*)&DummyRead,sizeof(s32));
		}

		_stream.read((char*)&ReadStage,sizeof(s8));

		RepDifficulty = KDIFFICULTY_SINGLE;

		Decode(_stream);
	}
	else if( ReadRepVer >= 29 ) {
		for( s32 i = 0 ; i <= 9; i++ )
			_stream.read((char*)&DummyRead,sizeof(s32));

		_stream.read((char*)&RepBType,sizeof(s8));
		_stream.read((char*)&RepWType,sizeof(s8));
		_stream.read((char*)&DummyRead,sizeof(s32));

		if( _fromfile ) {
			//ファイルから読み込む場合は日付データが入っているので飛ばす
			_stream.read((char*)&DummyRead,sizeof(s32));
			_stream.read((char*)&DummyRead,sizeof(s32));
			_stream.read((char*)&DummyRead,sizeof(s32));
		}

		_stream.read((char*)&ReadStage,sizeof(s8));
		_stream.read((char*)&RepDifficulty,sizeof(s32));

		_stream.read((char*)&SplitData,sizeof(NetRankingData_Split));

		s64 size;
		for( s32 i  =  0 ; i <= ReadStage ; i++) {
			if( _fromfile )
				_stream.read((char*)&size,sizeof(s64));
			else
				_stream.read((char*)&size,sizeof(s32));
			size  =  size & 0xffff;
			char* buff = (char*)malloc(size);
			_stream.read( buff, size );
			TempReplay[i].str("");
			TempReplay[i].write(buff, size);

			SAFE_FREE(buff);
		}
	}

}

void TReplay::OutPutText( const string& _filename )
{
	// テキストファイル書き込み
	ofstream ofs(_filename);
	if( ofs ){
		for( s32 i = 0 ; i < MaxFrame; i++ ){
			ofs <<	i  << "\t"
				<< HeroStickX[i] << "\t"
				<< HeroStickY[i] << "\t"		
				<< HeroButton[0][i] << "\t"		
				<< HeroButton[1][i] << "\t"		
				<< HeroButton[2][i] << "\t"		
				<< (Rendered[i] ? 1 : 0) << "\t"		
				<< std::flush;
		}
		ofs.close();
	}
}

void TReplay::CopyToLocal()
{

}

void TReplay::InitCurrentData()
{
	NowStickY = 0;
	NowStickX = 0;
	for( s32 i = 0 ; i <= 2; i++ )
		NowButton[i] = false;

}

void TReplay::SaveTempReplay( s32 _stage, const TRepFileData& _data )
{
	if( _stage >= MAX_TMP_REP )  return;

	TempReplay[_stage].str("");
	SaveToStream(TempReplay[_stage], _data);

}

void TReplay::SaveSequenceToStream( s32 _difficulty, string& _id, stringstream& F, const TRepFileData& _data, const NetRankingData_Split& _split )
{
	F.write((char*)&RepVersion,sizeof(s8));

	//プレイヤーデータ
	for( s32 i = 0 ; i <= 7; i++ )
		F.write((char*)&_data.PlayerName[i],sizeof(s32));

	F.write((char*)&_data.Score,sizeof(s32));
	F.write((char*)&_data.Time,sizeof(s32));
	F.write((char*)&_data.BType,sizeof(s8));
	F.write((char*)&_data.WType,sizeof(s8));
	F.write((char*)&_data.FrameSkipRate,sizeof(s32));

	F.write((char*)&_data.Stage,sizeof(s8));
	F.write((char*)&_difficulty,sizeof(s32));

	F.write((char*)&_data.OriginalScore,sizeof(s32));
	F.write((char*)&_data.DestBonusRate,sizeof(s32));
	F.write((char*)&_data.Life,sizeof(s32));
	F.write((char*)&_data.NoDamageBonus,sizeof(s32));
	F.write((char*)&_data.TotalHit,sizeof(s32));
	F.write((char*)&_data.ClearBonus,sizeof(s32));

	F.write((char*)&_split, sizeof(NetRankingData_Split));

	for( s32 i = 1 ; i <= 12; i++ )
		F.write((char*)&_id[i], sizeof(s8));

	//ステージ数分のリプレイデータをコピー
	for( s32 i  =  0 ; i <= _data.Stage; i++ ) {
		TempReplay[i].seekg(0, ios::end);
		s64 size = (s64)TempReplay[i].tellg();
		TempReplay[i].seekg(0, ios::beg);

		F.write((char*)&size, sizeof(s64));

		char* buff = (char*)malloc(size);
		TempReplay[i].read( buff, size );
		F.write( buff, size );

		SAFE_FREE(buff);
	}

}

void TReplay::SaveSequence( s32 _difficulty, const TRepFileData& _data, const NetRankingData_Split& _split )
{
	_mkdir("Replay");

	string filename;
	for(s32 i=0; i<100000; i++){
		s8 c[5];
		sprintf_s(c, "%5d", i );
		string FileNum(c);

		filename = string("Replay/") + string("CPRep") + FileNum + string(".cpr");
		FILE *fp;
		if ( (fopen_s(&fp, filename.c_str(),"r")) != EINVAL ){
			fclose( fp );

			if(i>=9999) return;
		}
		else{
			break;
		}
	}

	// バイナリファイル書き込み
	ofstream ofs(filename);
	if( ofs )
	{
		ofs.write((char*)&RepVersion, sizeof(s8));

		//プレイヤーデータ
		for( s32 i = 0 ; i <= 7; i++ )
			ofs.write((char*)&_data.PlayerName[i], sizeof(s32));

		ofs.write((char*)&_data.Score, sizeof(s32));
		ofs.write((char*)&_data.Time, sizeof(s32));
		ofs.write((char*)&_data.BType, sizeof(s8));
		ofs.write((char*)&_data.WType, sizeof(s8));
		ofs.write((char*)&_data.FrameSkipRate, sizeof(s32));

		//日付
		time_t timer;
		struct tm local;
		timer = time(NULL);

		localtime_s(&local, &timer); 

		s32 Year = local.tm_year + 1900;
		s32 Month = local.tm_mon + 1;
		s32 Day = local.tm_mday;
		ofs.write((char*)&Year, sizeof(s32));
		ofs.write((char*)&Month, sizeof(s32));
		ofs.write((char*)&Day, sizeof(s32));

		ofs.write((char*)&_data.Stage, sizeof(s8));
		ofs.write((char*)&_difficulty, sizeof(s32));

		ofs.write((char*)&_split, sizeof(NetRankingData_Split));

		for( s32 i  =  0 ; i <= _data.Stage; i++ ) {
			TempReplay[i].seekg(0, ios::end);
			s64 size = (s64)TempReplay[i].tellg();
			TempReplay[i].seekg(0, ios::beg);

			ofs.write((char*)&size, sizeof(s64));

			char* buff = (char*)malloc(size);
			TempReplay[i].read( buff, size );
			ofs.write( buff, size );

			SAFE_FREE(buff);
		}

		ofs.close();
	}
}

void TReplay::Decode( istream& _stream )
{
	s32 ReadCount = 0;      //読み込んだフレーム総数
	s16 TempCount = 0;      //同じデータの数
	Init();
	//解凍しつつロード
	while( ReadCount <= MaxFrame-1 ) {
		_stream.read((char*)&HeroStickY[ReadCount],sizeof(s8));
		_stream.read((char*)&TempCount,sizeof(s16));
		if( TempCount>1 ) {
			for( s32 i = 1 ; i < TempCount; i++ )
				HeroStickY[ReadCount+i] = HeroStickY[ReadCount];
		}
		ReadCount += TempCount;
	}

	TempCount = 0;
	ReadCount = 0;
	while( ReadCount <= MaxFrame-1 ) {
		_stream.read((char*)&HeroStickX[ReadCount],sizeof(s8));
		_stream.read((char*)&TempCount,sizeof(s16));
		if( TempCount>1 ) {
			for( s32 i = 1 ; i < TempCount; i++ )
				HeroStickX[ReadCount+i] = HeroStickX[ReadCount];
		}
		ReadCount += TempCount;
	}

	for( s32 k = 0 ; k <= 2; k++ ) {
		TempCount = 0;
		ReadCount = 0;
		while( ReadCount <= MaxFrame-1 ) {
			_stream.read((char*)&HeroButton[k,ReadCount],sizeof(bool));
			_stream.read((char*)&TempCount,sizeof(s16));
			if( TempCount>1 ) {
				for( s32 i = 1 ; i < TempCount; i++ )
					HeroButton[k][ReadCount+i] = HeroButton[k][ReadCount];
			}
			ReadCount += TempCount;
		}
	}

	TempCount = 0;
	ReadCount = 0;
	while( ReadCount <= MaxFrame-1 ) {
		_stream.read((char*)&Rendered[ReadCount],sizeof(bool));
		_stream.read((char*)&TempCount,sizeof(s16));
		if( TempCount>1 ) {
			for( s32 i = 1 ; i < TempCount; i++ )
				Rendered[ReadCount+i] = Rendered[ReadCount];
		}
		ReadCount += TempCount;
	}

}

void TReplay::LoadTempReplay( s32 _stage )
{
	TempReplay[_stage].seekg(0, ios::end);
	s32 size = (s32)TempReplay[_stage].tellg();

	if( size == 0 ) return;

	TempReplay[_stage].seekg(0, ios::beg);

	TempReplay[_stage].ignore(sizeof(s8));

	for( s32 i = 0 ; i <= 9; i++ )
		TempReplay[_stage].ignore(sizeof(s32));

	TempReplay[_stage].ignore(sizeof(s8));
	TempReplay[_stage].ignore(sizeof(s8));
	TempReplay[_stage].ignore(sizeof(s32));

	TempReplay[_stage].ignore(sizeof(s8));

	for( s32 i = 0 ; i <= 5; i++ )
		TempReplay[_stage].ignore(sizeof(s32));

	TempReplay[_stage].ignore(sizeof(s8));

	Decode(TempReplay[_stage]);

}

void TReplay::InitSplitData()
{
	for( s32 i = 0 ; i <= 4; i++ ) {
		SplitData.SplitScore[i] = 0;
		SplitData.SplitTime[i] = 0;
		SplitData.Life[i] = 0;
		SplitData.MShip[i] = 0;
		SplitData.Missile[i] = 0;
	}
}

void TReplay::debugload()
{
//ReadStage:s8;
//DummyRead:s32;
//size:Int64;
//F:TFileStream;
//	SetCurrentDir("Replay");
//
//	// バイナリファイル読み込み
//	F  =  TFileStream.Create( "CPRep22.cpr", fmOpenRead );
//
//	SetCurrentDir(ExtractFilePath(Application.ExeName));
//	F.Read(ReadRepVer,sizeof(s8));
//
//
//	for( s32 i = 0 to 9 )
//		F.Read(DummyRead,sizeof(s32));
//
//	F.Read(RepBType,sizeof(s8));
//	F.Read(RepWType,sizeof(s8));
//	F.Read(DummyRead,sizeof(s32));
//
//	F.Read(DummyRead,sizeof(s32));
//	F.Read(DummyRead,sizeof(s32));
//	F.Read(DummyRead,sizeof(s32));
//
//	F.Read(ReadStage,sizeof(s8));
//	F.Read(RepDifficulty,sizeof(s32));
//
//	F.Read(SplitData,sizeof(NetRankingData_Split));
//
//	for( s32 i  =  0 to 3 ) {
//		F.Read(size,sizeof(Int64));
//		TempReplay[i].Clear;
//		TempReplay[i].CopyFrom(F, size);
//	}
//
//	F.Free;

}

