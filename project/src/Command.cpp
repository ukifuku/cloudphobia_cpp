#include "Command.h"
#include "TOBJList.h"
#include "Sound.h"
#include "Define.h"
#include "Font.h"
#include "Gauge.h"
#include "Effect.h"
#include "Effect2.h"
#include "Replay.h"
#include "JoyScan.h"
#include "SpriteEXM.h"
#include "Hero.h"
#include "Hero2.h"
#include "Friend.h"
#include "BG2.h"
#include "BG3D.h"

TGameOver::TGameOver( TOBJList* owner, s32 _dp ) : TOBJ(owner)
{
	FObjectName = "TGameOver";
	FKind = kGauge;
	DeadPattern = _dp;
	MSGauge = NULL;

	Blur  =  NULL;
	ActFlg = 0;

	if( CreateBGM ) FOwner->MusicPlayer->Stop();

	CursorList =  new TCursorList(FOwner);

	//母艦ダメージアイコン
	MakeSpriteLite(FOwner->GetDG(), numSpr,0,0,112,80,100,bmNormal);
	Sprite[numSpr-1]->GetSpr()->AddTexture(GaugeTex, 289, 225, 400, 304);
	Sprite[numSpr-1]->GetSpr()->FCX += 56;
	Sprite[numSpr-1]->GetSpr()->FCY += 40;
	Sprite[numSpr-1]->GetSpr()->Move(320, 128);
	Sprite[numSpr-1]->GetSpr()->FVisible = false;

	if( CreateSE ) FOwner->SoundEffect->WavStop(10);     //4面の要塞爆発音を消す
}

TGameOver::~TGameOver()
{
	SAFE_DELETE(CursorList);
}

void TGameOver::Move()
{
	if( ActFlg == 0 ) {
		if( Age == 30 ) {
			Blur = new TBlur(FOwner);
			FOwner->Add(Blur);
		}


		FOwner->Add(new TBox(FOwner,0,0,640,31,0,200,255));
		switch( DeadPattern ){
		case 0:{
			FOwner->SentenceCenter("you died",320,0,32,3);
			if( Age>30 ) FOwner->TimeStop = true;
			   }break;
		case 1:{
			FOwner->SentenceCenter("the ship is sunk",320,0,16,3);
			if( Age == 40 ) {
				//母艦撃沈の場合は説明文を追加

				MSGauge  = new TMSGauge2(FOwner,208,192,2);
				FOwner->Add(MSGauge);

				Sprite[0]->GetSpr()->FVisible = true;

				CursorList->MakeCursor("The mothership was destroyed",0,240,-6,640,0,KFONTKIND_ORANGE, GLASSCURSOR_PATTERN_BLACK_CENTER, KCURSOR_POS_CENTER);
				CursorList->MakeCursor("because you let too many enemies pass by.",0,272,-6,640,1,KFONTKIND_ORANGE, GLASSCURSOR_PATTERN_BLACK_CENTER, KCURSOR_POS_CENTER);
				CursorList->MakeCursor("watch out for( u32 the mothership's health gauge.",0,304,-6,640,2,KFONTKIND_ORANGE, GLASSCURSOR_PATTERN_BLACK_CENTER, KCURSOR_POS_CENTER);
			}
			   }break;
		case 2:{
			FOwner->SentenceCenter("time over",320,0,16,3);
			if( Age>30 ) FOwner->TimeStop = true;
			   }break;
		}

		FOwner->Add(new TBox(FOwner,0,448,640,480,0,200,255));
		FOwner->SentenceCenter("game over",320,448,32,3);

		if( Age>60 ) Input();
	}

	TOBJ::Move();
}

void TGameOver::Input()
{
	for( u32 i = 0 ; i <= 7; i++ )
		if( ! ButtonON[i] ) ButtonFlg[i] = false;

	if( (! ButtonFlg[0]) && ButtonON[0] ) {
		ButtonFlg[0] = true;

		CursorList->ClearCursors(-640,0);
		Sprite[0]->GetSpr()->FVisible = false;
		if( MSGauge != NULL ) {
			MSGauge->Die();
			MSGauge = NULL;
		}

		if( (! FOwner->ReplayON) && (! FOwner->CheatON) && (! FOwner->IsPractice) ) {
			FOwner->Add(new TNetRanking(FOwner));
			ActFlg = 1;
			//Blurを次の画面で使い回すため、このケースだけDieしない
			//最終的にはタイトル画面に戻るため、そのときリセットで自動的に死ぬことになる
		}
		else if( FOwner->ReplayON ) {
			FOwner->AfterLoopEvent = MRepView;
			Die();
		}
		else {
			FOwner->AfterLoopEvent = MTitle;
			FOwner->TimeStop = false;
			Die();
		}
	}
}

void TGameOver::Die()
{
	TOBJ::Die();
	if( Blur != NULL ) Blur->Die();

	if( MSGauge != NULL ) MSGauge->Die();
}

TReady::TReady( TOBJList* owner, s32 _time ) : TOBJ(owner)
{
	FObjectName = "TReady";
	FKind = kGauge;
	ReadyTime = _time;
	bandWidth = 0;
	Cursor = new TCursorList(FOwner);

	//画面作成
	string title  =  "";
	switch(FOwner->NowStage){
	case 0:title  =  "she's lost control";	break;
	case 1:title  =  "in a lonely place";	break;
	case 2:title  =  "disorder";	break;
	case 3:title  =  "isolation";	break;
	case 4:title  =  "ceremony";	break;
	}

	Cursor->SetAppearPoint(-640,0);
	Cursor->MakeCursor(title,  0, 1, 16, 640, 0, KFONTKIND_NORMAL, GLASSCURSOR_PATTERN_NONE, KCURSOR_POS_CENTER);
	Cursor->SetAppearPoint(640,0);
	Cursor->MakeCursor("get ready",0,448,32,640,1,KFONTKIND_NORMAL, GLASSCURSOR_PATTERN_NONE, KCURSOR_POS_CENTER);
}

TReady::~TReady()
{
	SAFE_DELETE(Cursor);
}

void TReady::Move()
{
	bool DummyButton = FOwner->Replayer->NowButton[2];

	FOwner->Add(new TBox(FOwner,0,0,640,31,0,200,255));
	FOwner->Add(new TBox(FOwner,0,448,640,480,0,200,255));


	if( Age>30 ) {
		if( (bandWidth<16) )
			bandWidth++;

		FOwner->Add(new TBox(FOwner,0,240-bandWidth,640,240+bandWidth,0,200,128));
		FOwner->SentenceRight(InttoStr(ReadyTime / 1000),304,224,8,3);             //180000
		FOwner->Sentence(".",304,224,8,3);
		FOwner->Sentence(InttoStr((ReadyTime / 100)-(ReadyTime / 1000)*10),320,224,8,3);
		FOwner->Sentence(InttoStr((ReadyTime / 10)-(ReadyTime / 100)*10),336,224,8,3);

		ReadyTime -= GainTime;
	}

	if( FOwner->ReplayON ) {
		FOwner->SentenceRight("button1:slow",640,352,0,0);
		FOwner->SentenceRight("button2:skip",640,384,0,0);
	}

	if( FOwner->NowStage != 0 ) {
		//ステージ２以降はReady画面でのスクロールをここで肩代わりする。
		//（Ready中はメインルーチンでスクロールできないため）
		FOwner->Distance += FOwner->Speed;
	}

	if( ReadyTime<=0 ) {
		Die();
		FOwner->Add(new TBoxContrl(FOwner,4));
		FOwner->GameMode = gPlay;
		FOwner->CreateHUD();
	}

	if( DummyButton ) FOwner->DontBoost = true;
	else FOwner->DontBoost = false;


	TOBJ::Move();
}

TDying::TDying( TOBJList* owner ) : TOBJ(owner)
{
	FObjectName = "TDying";
	FKind = kGauge;
	FOwner->TimeStop = true;
}

void TDying::Move()
{
	FOwner->Add( new TBox(FOwner,0,224,640,255,0,200,255));
	FOwner->SentenceCenter("i just wanted to be free...",320,224,0,3);

	if( Age>10 ) Input();

	if( Age>120 ) {
		FOwner->TimeStop = false;
		Die();
	}

	TOBJ::Move();
}

void TDying::Input()
{
	if( ! ButtonON[0] ) ButtonFlg[0] = false;

	if( (! ButtonFlg[0]) && ButtonON[0] && (TimeRanking_local[0][0].Time>0) ) {
		FOwner->TimeStop = false;
		Die();
		FOwner->Add( new TResult2(FOwner));
		ButtonFlg[0] = true;
	}
}

TRanking::TRanking( TOBJList* owner, s32 _score ) : TOBJ(owner)
{
	FKind = kGauge;
	FOwner->GameMode = gOther;
	Choice = 0;
	BlinkNum = 99;
	PlayerScore = _score;

	if( (PlayerScore>0) && (! FOwner->ReplayON) && (! FOwner->CheatON) && (! FOwner->IsPractice) )
		RankIn();

	StickFlgX = true;
	StickFlgY = true;

	for( u32 i = 0 ; i <= 9; i++ ) {
		ActFlg[i] = 0;
		ActCount[i] = 0;
	}

	for( u32 i = 0 ; i <= 99; i++ )
		BarPos[i] = TPoint(0,0);
}

void TRanking::Move()
{
	TOBJ::Move();
}

void TRanking::Input()
{
	for( u32 i = 0 ; i <= 7; i++ )
		if( ! ButtonON[i] ) ButtonFlg[i] = false;

	for( u32 i = 0 ; i <= 7; i++ ) {
		if( (! ButtonFlg[i]) && ButtonON[i] ) {
			ButtonFlg[i] = true;
			Die();
			//FOwner->SoundEffect->WavPlay(WADeside,1);
			if( (PlayerScore=0) || FOwner->ReplayON || FOwner->CheatON || FOwner->IsPractice ) {
				FOwner->AfterLoopEvent = MTitle;
				FOwner->TimeStop = false;
			}
			else{
				//FOwner->Add( new TReplayAsk(FOwner));
				FOwner->Add( new TNetRanking(FOwner));
			}
		}
	}
}

void TRanking::RankIn()
{

}

void TNetRanking::MakeRankingItems( const NetRankingData _rankdata[], s32 _totaldata, s32 _rankline, s32 _sameid )
{
	//tmpObj:TMojiAndGlassCursor;
	//offset:s32;
	//maxline:s32;
	//CurrentTime:TDateTime;
	//	Year, Month, Day: Word;
	//glasspattern:s32;
	//TempTime:s32;

	//全カーソルをクリア
	if(CursorList.size() > 0 ) {
		for( u32 i = 0 ; i < CursorList.size(); i++ ) {
			CursorList[i]->Die();
		}
		CursorList.clear();
	}

	MakeItemName();

	s32 offset  =  0;
	s32 maxline = glm::min(KNETRANK_PLAY_MAX, _totaldata);

	if( _rankline >= KNETRANK_COMMOM_MAXRANK ) {
		//ランク外の場合、下2行分空ける
		maxline  =  glm::min(KNETRANK_PLAY_MAX - 2, _totaldata);
		offset  =  _totaldata - maxline;        

		//自分のスコアを表示
		TMojiAndGlassCursor* tmpObj  = new TMojiAndGlassCursor(FOwner, OrgX+16,FirstYPos_Ranking+(KNETRANK_PLAY_MAX-1)*YSPACE+OrgY,576,GLASSCURSOR_PATTERN_BLIGHT);
		FOwner->Add(tmpObj);
		CursorList.push_back(tmpObj);

		//ランク
		tmpObj->AddSentenceRight("--", 64,0,255,255,255, KFONTKIND_NORMAL);

		//名前
		tmpObj->AddSentence(FOwner->PlayerNameToStr(FOwner->PlayerName, MAX_NAME_LENGTH), 88,0,255,255,255,KFONTKIND_NORMAL);

		switch( ActFlg[0] ){
		case 0:
		case 2:{
			//スコア
			tmpObj->AddSentence(FOwner->ScoreToStr(FOwner->TotalScore), 256,0,255,255,255,KFONTKIND_NORMAL);
			   }break;
		case 1:
		case 3:{
			//タイム
			s32 TempTime;
			if( FOwner->BossDie ) TempTime = FOwner->SequenceTotalTime;
			else TempTime = 0;

			if( FOwner->IsSequence() )
				tmpObj->AddSentenceRight(TotalTimeToStr(TempTime), 400,0,255,255,255,KFONTKIND_NORMAL);
			else
				tmpObj->AddSentenceRight(FOwner->TimeToStr(TempTime), 368,0,255,255,255,KFONTKIND_NORMAL);
			   }break;
		}

		//日付

		s32 Year, Month, Day;
		GetDate( Year, Month, Day );
		tmpObj->AddSentenceRight(FOwner->DateToStr(Year,Month,Day),592,0,255,255,255,KFONTKIND_NORMAL);

		//tmpObj->SetColor(255,128,0);
		MakePartition(tmpObj);
		tmpObj->SetSpeed(5, 5);
		tmpObj->SetDest(16,FirstYPos_Ranking+(KNETRANK_PLAY_MAX-1)*YSPACE,KNETRANK_PLAY_MAX*5);
		tmpObj->SetAlpha(0);
		tmpObj->SetDestAlpha(255,15,(KNETRANK_PLAY_MAX-1)*7);


		//区切り線生成
		tmpObj  = new TMojiAndGlassCursor(FOwner,OrgX,FirstYPos_Ranking+(KNETRANK_PLAY_MAX-1)*YSPACE-32 + OrgY,608,GLASSCURSOR_PATTERN_LINE);
		FOwner->Add(tmpObj);
		CursorList.push_back(tmpObj);
		tmpObj->SetSpeed(5, 5);
		tmpObj->SetDest(16,FirstYPos_Ranking+(KNETRANK_PLAY_MAX-1)*YSPACE-32,KNETRANK_PLAY_MAX*5);
		tmpObj->SetAlpha(0);
		tmpObj->SetDestAlpha(200,15,(KNETRANK_PLAY_MAX-1)*7);

		//通し面モードでかつ前回プレイ時のスコアのほうが上だった場合、前回のスコアの位置を求める
		if( FOwner->IsSequence() && (ActFlg[0]>=2) && (_sameid>=0) ) {
			offset  =  _sameid - KNETRANK_PLAY_RANKIN + 2;

			if( ((offset + maxline) > _totaldata) ) {
				offset  =  _totaldata - maxline;
			}
		}

	}
	else if( _rankline >= 0 ) {
		//ランクインした項目を画面の真ん中に持っていくため、一行目の順位を求める
		offset  =  _rankline - KNETRANK_PLAY_RANKIN;

		if( ((offset + maxline) > _totaldata) ) {
			offset  =  _totaldata - maxline;
		}
	}

	if( offset < 0 ) {
		offset  =  0;
	}


	if( _totaldata > 0 ) {

		for( s32 i = 0 ; i < maxline; i++ ) {
			s32 glasspattern;
			//ランクインしたら色を変える
			if( ((offset+i) == _rankline) ) {
				glasspattern  =  GLASSCURSOR_PATTERN_BLIGHT;
				MakeSentence("new!", OrgX, FirstYPos_Ranking+i*YSPACE-12, i);
			}
			else if( (_rankline >= KNETRANK_COMMOM_MAXRANK) && ((offset+i) == _sameid) ) {
				glasspattern  =  GLASSCURSOR_PATTERN_BLIGHT;
			}
			else
				glasspattern  =  GLASSCURSOR_PATTERN_GLASS;

			TMojiAndGlassCursor* tmpObj  = new TMojiAndGlassCursor(FOwner,OrgX+16,OrgY+FirstYPos_Ranking+i*YSPACE,576,glasspattern);
			FOwner->Add(tmpObj);
			CursorList.push_back(tmpObj);

			//if( ((offset+i) = _rankline) ) tmpObj->SetBlink(true);

			//ランク
			tmpObj->AddSentenceRight(InttoStr(_rankdata[offset+i].Rank), 64,0,255,255,255,KFONTKIND_NORMAL);

			//名前
			tmpObj->AddSentence(FOwner->PlayerNameToStr(_rankdata[offset+i].Name, MAX_NAME_LENGTH), 88,0,255,255,255,KFONTKIND_NORMAL);

			switch(ActFlg[0]) {
			case 0:
			case 2:{
				//スコア
				tmpObj->AddSentence(FOwner->ScoreToStr(_rankdata[offset+i].Score), 256,0,255,255,255,KFONTKIND_NORMAL);
				   }break;
			case 1:
			case 3:{
				//タイム
				if( FOwner->IsSequence() )
					tmpObj->AddSentenceRight(TotalTimeToStr(_rankdata[offset+i].Time), 400,0,255,255,255,KFONTKIND_NORMAL);
				else
					tmpObj->AddSentenceRight(FOwner->TimeToStr(_rankdata[offset+i].Time), 368,0,255,255,255,KFONTKIND_NORMAL);
				   }break;
			}

			//日付
			tmpObj->AddSentenceRight(FOwner->DateToStr(_rankdata[offset+i].Year,
				_rankdata[offset+i].Month,
				_rankdata[offset+i].Day),
				592,0,255,255,255,KFONTKIND_NORMAL);

			MakePartition(tmpObj);
			tmpObj->SetSpeed(5, 5);
			tmpObj->SetDest(16,FirstYPos_Ranking+i*YSPACE,i*5);
			tmpObj->SetAlpha(0);
			tmpObj->SetDestAlpha(255,15,i*7);
		}
	}
}

void TNetRanking::MakePartition( TMojiAndGlassCursor* _cursor )
{

}

TNetRanking::TNetRanking( TOBJList* owner ) : TOBJ(owner)
{
	FObjectName = "TNetRanking";
	FKind = kGauge;
	FOwner->GameMode = gOther;
	Choice = 0;
	BlinkNum = 99;
	ErrorNo = 0;

	TotalScoreData = 0;
	TotalTimeData = 0;

	TotalScoreData_local = 0;
	TotalTimeData_local = 0;

	SameID_Score = -1;
	SameID_Time = -1;

	ConnectResult = 0;
	IsConnecting = false;

	OrgX  =  0;
	OrgY  =  480;
	DestX  =  0;
	DestY  =  0;

	XSpeed  =  0;
	YSpeed  =  0;

	FOwner->CulcFrameSkipRate();

	//  if( (FOwner->TotalScore>0) && (! FOwner->ReplayON) && (! FOwner->CheatON) ) RankIN_local;
	if( (FOwner->TotalScore>0) && (! FOwner->ReplayON) && (! FOwner->CheatON) && (! FOwner->IsPractice) ) {
		if( FOwner->Difficulty == KDIFFICULTY_SINGLE )
			RankIn_local2(ScoreRanking_local[FOwner->NowStage], TimeRanking_local[FOwner->NowStage], LOCAL_RANKING_SIZE);
		else
			RankIn_local2(ScoreRanking_Seq_local[FOwner->Difficulty], TimeRanking_Seq_local[FOwner->Difficulty], LOCAL_RANKING_SIZE);
	}

	StickFlgX = true;
	StickFlgY = true;

	for( u32 i = 0 ; i <= 9; i++ ) {
		ActFlg[i] = 0;
		ActCount[i] = 0;
	}

	//  Blur  = new TBlur(FOwner);
	//  FOwner->Add(Blur);

	Arrow_Left  = new TArrow(FOwner,128,48,2);
	Arrow_Left->Hide();
	FOwner->Add(Arrow_Left);
	Arrow_Right  = new TArrow(FOwner,512,48,0);
	Arrow_Right->Show();
	FOwner->Add(Arrow_Right);
}

TNetRanking::~TNetRanking()
{
	if( !CursorList.empty() ) {
		for( u32 i = 0 ; i < CursorList.size(); i++ ) {
			CursorList[i]->Die();
		}
	}

	CursorList.clear();

	//  Blur.Die();

	Arrow_Right->Die();
	Arrow_Left->Die();

	if( ConnectGauge != NULL ) ConnectGauge->Die();
}

void TNetRanking::Move()
{
	TOBJ::Move();
	if( ! IsConnecting ) Input();

	//タイトル表示
	FOwner->Add( new TBox(FOwner,0,0,640,32,0,200,255));
	FOwner->Add( new TBox(FOwner,0,448,640,480,0,200,255));

	if(ConnectResult  !=  KCONNECT_SUCCESS ) {
		FOwner->Add( new TBox(FOwner,0,224,640,256,0,200,128));
		FOwner->SentenceCenter(string("connection failed:")+ InttoStr(ErrorNo),320,224,0,3);
		return;   
	}

	switch(ActFlg[0]){
	case 0:
	case 1:{ //ローカルランキング表示
		//タイトル表示
		FOwner->SentenceCenter("local ranking",320,0,8,3);
		   }break;
	case 2:
	case 3:{  //ネットランキング
		//タイトル表示
		FOwner->SentenceCenter("network ranking",320,0,8,3);
		   }break;
	}
}

void TNetRanking::Input()
{
	for( u32 i = 0 ; i <= 7; i++ ){
		if( ! ButtonON[i] ) ButtonFlg[i] = false;
	}

	if( DebugMode && (! ButtonFlg[7]) && ButtonON[7] ) {
		ButtonFlg[7] = true;
		ActFlg[0]  =  0;
		TestRankIn();
	}

	if( FOwner->Joy.StickX() == 0 ) StickFlgX = false;

	if( (StickFlgX == false) && (FOwner->Joy.StickX() == 1) ) {
		StickFlgX = true;
		OrgX = 640;
		OrgY = 0;
		switch( ActFlg[0]){
		case 0:{
			//タイムランキングに移行
			ActFlg[0] = 1;
			if( FOwner->IsSequence() )
				MakeRankingItems(TimeRanking_Seq_local[FOwner->Difficulty], TotalTimeData_local, TimeRankLine_local, -1);
			else
				MakeRankingItems(TimeRanking_local[FOwner->NowStage], TotalTimeData_local, TimeRankLine_local, -1);
			   }break;
		case 2:{
			//タイムランキングに移行
			ActFlg[0] = 3;
			MakeRankingItems(&TimeRanking[0], TotalTimeData, TimeRankLine, SameID_Time);
			   }break;
		}
		Arrow_Right->Hide();
		Arrow_Left->Show();
		if( CreateSE ) FOwner->SoundEffect->WavPlay(WASelect,0);
	}

	if( (StickFlgX == false) && (FOwner->Joy.StickX() == -1) ) {
		StickFlgX = true;
		OrgX = -640;
		OrgY = 0;
		switch( ActFlg[0] ){
		case 1:{
			//スコアランキングに移行
			ActFlg[0]  =  0;
			if( FOwner->IsSequence() )
				MakeRankingItems(ScoreRanking_Seq_local[FOwner->Difficulty], TotalScoreData_local, ScoreRankLine_local, -1);
			else
				MakeRankingItems(ScoreRanking_local[FOwner->NowStage], TotalScoreData_local, ScoreRankLine_local, -1);
			   }break;
		case 3:{
			//スコアランキングに移行
			ActFlg[0]  =  2;
			MakeRankingItems(&ScoreRanking[0], TotalScoreData, ScoreRankLine, SameID_Score);
			   }break;
		}
		Arrow_Right->Show();
		Arrow_Left->Hide();
		if( CreateSE ) FOwner->SoundEffect->WavPlay(WASelect,0);
	}

	for( u32 i = 0 ; i <= 7; i++ ) {
		if( (! ButtonFlg[i]) && ButtonON[i] ) {
			ButtonFlg[i] = true;
			switch( ActFlg[0]) {
			case 0:
			case 1:{
				//ネットランキングに移行
				if( ConnectNet )
					RankIn();
				else
					SetNextState();
				   }break;
			case 2:
			case 3:
			case 4:{
				SetNextState();
				   }break;
			}
		}
	}
}

void TNetRanking::RankIn()
{
	//全カーソルをクリア
	if( !CursorList.empty() ) {
		for( u32 i = 0 ; i < CursorList.size(); i++ ) {
			CursorList[i]->Die();
		}
		CursorList.clear();
	}

	//ランキング用ヘッダ作成
	FOwner->SetRankInHeader();

	IsConnecting = true;
	ConnectGauge  = new TConnectingGauge(FOwner);
	FOwner->Add(ConnectGauge);

	stringstream memStream;

	if( (FOwner->Difficulty < KDIFFICULTY_SINGLE) ) {
		//通し面モードならこのステージのリプレイを一時データに保存
		FOwner->Replayer->SaveTempReplay(FOwner->NowStage, FOwner->RankinHeaderData);
		//全ステージのリプレイをマージ
		FOwner->Replayer->SaveSequenceToStream(FOwner->Difficulty, FOwner->NetworkID, memStream,FOwner->RankinHeaderData, FOwner->Replayer->SplitData);

		TPostThread(this, NETRANKING_SEQUENCE_CGI_PATH + "rankingup.cgi", memStream);
	}
	else {
		FOwner->Replayer->SaveToStream( memStream, FOwner->RankinHeaderData);
		TPostThread(this, NETRANKING_CGI_PATH + "rankingup.cgi", memStream);
	}
}

void TNetRanking::RankIn_local()
{
	ScoreRankLine_local =  99;
	TimeRankLine_local =  99;

	//スコアランキング
	if( FOwner->TotalScore >= ScoreRanking_local[FOwner->NowStage][9].Score ) {       //ランキング10位の得点より上なら、入れ替え

		s32 Year, Month, Day;
		GetDate( Year, Month, Day );

		ScoreRanking_local[FOwner->NowStage][9].Score = FOwner->TotalScore;
		for( u32 i = 0 ; i <= 7; i++ )
			ScoreRanking_local[FOwner->NowStage][9].Name[i] = FOwner->PlayerName[i];

		if( FOwner->BossDie )
			ScoreRanking_local[FOwner->NowStage][9].Time = FOwner->LimitTime;
		else 
			ScoreRanking_local[FOwner->NowStage][9].Time = 0;

		ScoreRanking_local[FOwner->NowStage][9].BoostType = BoostMode;
		ScoreRanking_local[FOwner->NowStage][9].Rank = 10;
		ScoreRanking_local[FOwner->NowStage][9].WeaponType = FOwner->HeroWeapon;
		ScoreRanking_local[FOwner->NowStage][9].FrameSkipRate = FOwner->FrameSkipRate;
		ScoreRanking_local[FOwner->NowStage][9].Year = Year;
		ScoreRanking_local[FOwner->NowStage][9].Month = Month;
		ScoreRanking_local[FOwner->NowStage][9].Day = Day;

		ScoreRankLine_local = 9;


		for( u32 i = 8 ; i >= 0; i-- ) {                              //下の順位から順番に入れ替えていく
			if( ScoreRanking_local[FOwner->NowStage][i+1].Score >= ScoreRanking_local[FOwner->NowStage][i].Score ) {
				NetRankingData dummyrank = ScoreRanking_local[FOwner->NowStage][i];
				ScoreRanking_local[FOwner->NowStage][i]  =  ScoreRanking_local[FOwner->NowStage][i+1];
				ScoreRanking_local[FOwner->NowStage][i+1]  =  dummyrank;

				ScoreRanking_local[FOwner->NowStage][i].Rank--;

				if( ScoreRanking_local[FOwner->NowStage][i].Score > ScoreRanking_local[FOwner->NowStage][i+1].Score )
					ScoreRanking_local[FOwner->NowStage][i+1].Rank++;

				ScoreRankLine_local = i;
			}
			else 
				break;
		}
	}
	//項目数をカウントする
	TotalScoreData_local = 0;
	for( u32 i = 0 ; i <= 9; i++ ) {
		if( ScoreRanking_local[FOwner->NowStage][i].Score != 0 ) {
			TotalScoreData_local++;
		}
		else 
			break;
	}

	//タイムランキング
	if( FOwner->BossDie && (FOwner->LimitTime >= TimeRanking_local[FOwner->NowStage][9].Time) ) {       //ランキング10位の得点より上なら、入れ替え
		s32 Year, Month, Day;
		GetDate( Year, Month, Day );

		TimeRanking_local[FOwner->NowStage][9].Score = FOwner->TotalScore;
		for( u32 i = 0 ; i <= 7; i++ )
			TimeRanking_local[FOwner->NowStage][9].Name[i] = FOwner->PlayerName[i];
		TimeRanking_local[FOwner->NowStage][9].Time = FOwner->LimitTime;
		TimeRanking_local[FOwner->NowStage][9].BoostType = BoostMode;
		TimeRanking_local[FOwner->NowStage][9].Rank = 10;
		TimeRanking_local[FOwner->NowStage][9].WeaponType = FOwner->HeroWeapon;
		TimeRanking_local[FOwner->NowStage][9].FrameSkipRate = FOwner->FrameSkipRate;
		TimeRanking_local[FOwner->NowStage][9].Year = Year;
		TimeRanking_local[FOwner->NowStage][9].Month = Month;
		TimeRanking_local[FOwner->NowStage][9].Day = Day;

		TimeRankLine_local = 9;


		for( u32 i = 8 ; i >= 0; i-- ) {                              //下の順位から順番に入れ替えていく
			if( TimeRanking_local[FOwner->NowStage][i+1].Time >= TimeRanking_local[FOwner->NowStage][i].Time ) {
				NetRankingData dummyrank = TimeRanking_local[FOwner->NowStage][i];
				TimeRanking_local[FOwner->NowStage][i]  =  TimeRanking_local[FOwner->NowStage][i+1];
				TimeRanking_local[FOwner->NowStage][i+1]  =  dummyrank;

				TimeRanking_local[FOwner->NowStage][i].Rank++;

				if( TimeRanking_local[FOwner->NowStage][i].Time > TimeRanking_local[FOwner->NowStage][i+1].Time )
					TimeRanking_local[FOwner->NowStage][i+1].Rank++;

				TimeRankLine_local = i;
			}
			else 
				break;
		}
	}

	//項目数をカウントする
	TotalTimeData_local = 0;
	for( u32 i = 0 ; i <= 9; i++ ) {
		if( TimeRanking_local[FOwner->NowStage][i].Time != 0 ) {
			TotalTimeData_local++;
		}
		else 
			break;
	}

	MakeRankingItems(ScoreRanking_local[FOwner->NowStage], TotalScoreData_local, ScoreRankLine_local, -1);
}

void TNetRanking::RankIn_local2( NetRankingData scorerank[], NetRankingData timerank[], s32 size )
{
	ScoreRankLine_local =  99;
	TimeRankLine_local =  99;

	//スコアランキング
	if( FOwner->TotalScore >= scorerank[9].Score ) {       //ランキング10位の得点より上なら、入れ替え
		s32 Year, Month, Day;
		GetDate( Year, Month, Day );

		scorerank[9].Score = FOwner->TotalScore;
		for( u32 i = 0 ; i <= 7; i++ )
			scorerank[9].Name[i] = FOwner->PlayerName[i];

		if( FOwner->Difficulty == KDIFFICULTY_SINGLE ) {
			if( FOwner->BossDie )
				scorerank[9].Time = FOwner->LimitTime;
			else 
				scorerank[9].Time = 0;
		}
		else {
			if( FOwner->BossDie && (FOwner->NowStage == 4) ) 
				scorerank[9].Time = FOwner->SequenceTotalTime;
			else 
				scorerank[9].Time = 0;
		}

		scorerank[9].BoostType = BoostMode;
		scorerank[9].Rank = 10;
		scorerank[9].WeaponType = FOwner->HeroWeapon;
		scorerank[9].FrameSkipRate = FOwner->FrameSkipRate;
		scorerank[9].Year = Year;
		scorerank[9].Month = Month;
		scorerank[9].Day = Day;

		ScoreRankLine_local = 9;


		for( u32 i = 8 ; i >= 0; i-- ) {                              //下の順位から順番に入れ替えていく
			if( scorerank[i+1].Score >= scorerank[i].Score ) {
				NetRankingData dummyrank = scorerank[i];
				scorerank[i]  =  scorerank[i+1];
				scorerank[i+1]  =  dummyrank;

				scorerank[i].Rank--;

				if( scorerank[i].Score>scorerank[i+1].Score )
					scorerank[i+1].Rank++;

				ScoreRankLine_local = i;
			}
			else 
				break;
		}

		if( FOwner->IsSequence() ) {
			ScoreRanking_Split_local[FOwner->Difficulty][ScoreRankLine_local]  =  FOwner->Replayer->SplitData;
		}
	}
	//項目数をカウントする
	TotalScoreData_local = 0;
	for( u32 i = 0 ; i <= 9; i++ ) {
		if( scorerank[i].Score != 0 ) {
			TotalScoreData_local++;
		}
		else
			break;
	}

	//タイムランキング
	if( FOwner->BossDie && (FOwner->SequenceTotalTime >= timerank[9].Time) ) {       //ランキング10位の得点より上なら、入れ替え
		s32 Year, Month, Day;
		GetDate( Year, Month, Day );

		timerank[9].Score = FOwner->TotalScore;
		for( u32 i = 0 ; i <= 7; i++ )
			timerank[9].Name[i] = FOwner->PlayerName[i];

		if( FOwner->Difficulty == KDIFFICULTY_SINGLE ) {
			timerank[9].Time = FOwner->LimitTime;
		}
		else if( FOwner->NowStage == 4 ) {
			timerank[9].Time = FOwner->SequenceTotalTime;
		}
		timerank[9].BoostType = BoostMode;
		timerank[9].Rank = 10;
		timerank[9].WeaponType = FOwner->HeroWeapon;
		timerank[9].FrameSkipRate = FOwner->FrameSkipRate;
		timerank[9].Year = Year;
		timerank[9].Month = Month;
		timerank[9].Day = Day;

		TimeRankLine_local = 9;


		for( u32 i = 8 ; i >= 0; i-- ) {                              //下の順位から順番に入れ替えていく
			if( timerank[i+1].Time>=timerank[i].Time ) {
				NetRankingData dummyrank = timerank[i];
				timerank[i]  =  timerank[i+1];
				timerank[i+1]  =  dummyrank;

				timerank[i].Rank--;

				if( timerank[i].Time>timerank[i+1].Time )
					timerank[i+1].Rank++;

				TimeRankLine_local = i;
			}
			else
				break;
		}

		if( FOwner->IsSequence() ) {
			TimeRanking_Split_local[FOwner->Difficulty][TimeRankLine_local]  =  FOwner->Replayer->SplitData;
		}
	}

	//項目数をカウントする
	TotalTimeData_local = 0;
	for( u32 i = 0 ; i <= 9; i++ ) {
		if( timerank[i].Time != 0 ) {
			TotalTimeData_local++;
		}
		else 
			break;
	}

	MakeRankingItems(scorerank, TotalScoreData_local, ScoreRankLine_local, -1);
}

void TNetRanking::TestRankIn()
{
	FOwner->TimeStop = true;

	//ダミーデータセット
	for( u32 i = 0 ; i <= 7; i++ )
		FOwner->PlayerName[i]  =  random(47);

	FOwner->TotalScore  =  random(100000000);
	FOwner->LimitTime  =  random(180000);
	FOwner->BossDie  =  true;
	BoostMode =  random(1);
	FOwner->HeroWeapon  =  random(1);
	FOwner->FrameSkipRate  =  random(10000);

	FOwner->Replayer->Init();

	//このデータで送信
	RankIn();
}

void TNetRanking::Die()
{
	TOBJ::Die();
}

void TNetRanking::MakeItemName()
{
	//区切り線
	TMojiAndGlassCursor* tmpObj  = new TMojiAndGlassCursor(FOwner,16+OrgX,FirstYPos_Ranking - 24 + OrgY,608,GLASSCURSOR_PATTERN_LINE);
	FOwner->Add(tmpObj);
	CursorList.push_back(tmpObj);
	tmpObj->SetSpeed(5, 5);
	tmpObj->SetDest(16,FirstYPos_Ranking - 24,0);
	tmpObj->SetAlpha(0);
	tmpObj->SetDestAlpha(200,15,0);

	//名前
	MakeSentence("name", 120, FirstYPos_Ranking - 48, 0);

	switch(ActFlg[0]){
	case 0:
	case 2:{
		//スコア
		MakeSentence("score", 296, FirstYPos_Ranking - 48, 1);

		   }break;
	case 1:
	case 3:{
		//タイム
		MakeSentence("time", 296, FirstYPos_Ranking - 48, 2);
		   }break;
	}

	//日付
	MakeSentence("date", 488, FirstYPos_Ranking - 48, 3);

	string order;
	switch( ActFlg[0] ){
	case 0:
	case 2:{ //スコアランク表示
		order = "ordered by score";
		   }break;
	case 1:
	case 3:{  //タイムランク表示
		order = "ordered by time";
		   }break;
	}

	s32 textXPos  =  320 - (order.size()*18 / 2) - 160;
	tmpObj  = new TMojiAndGlassCursor(FOwner,160-640,FirstYPos_Ranking - 72,320,GLASSCURSOR_PATTERN_NONE);
	FOwner->Add(tmpObj);
	CursorList.push_back(tmpObj);
	tmpObj->SetSpeed(5, 5);
	tmpObj->SetDest(160,FirstYPos_Ranking - 72,0);
	tmpObj->SetAlpha(0);
	tmpObj->SetDestAlpha(200,15,0);
	tmpObj->AddSentence(order, textXPos,0,255,255,255,KFONTKIND_ORANGE);
}

void TNetRanking::MakeSentence( const string& _caption, s32 _x, s32 _y, s32 _delay )
{
	TMojiAndGlassCursor* tmpObj  = new TMojiAndGlassCursor(FOwner,_x-640,_y,48,GLASSCURSOR_PATTERN_NONE);
	FOwner->Add(tmpObj);
	CursorList.push_back(tmpObj);
	tmpObj->AddSentence(_caption, 0,0,255,255,255,KFONTKIND_BLIGHT);
	tmpObj->SetSpeed(5, 5);
	tmpObj->SetDest(_x,_y,_delay*5);
	tmpObj->SetAlpha(0);
	tmpObj->SetDestAlpha(255,15,_delay*7);
}

void TNetRanking::SetNextState()
{
	//消滅
	Die();
	//FOwner->SoundEffect->WavPlay(WADeside,1);
	if( (FOwner->TotalScore=0) || FOwner->ReplayON || FOwner->CheatON || FOwner->IsPractice || TrialMode ) {
		FOwner->StageEnd();
	}
	else
		FOwner->Add( new TReplayAsk2(FOwner));
}

void TNetRanking::OnResponse( stringstream& _res )
{
	TOBJ::OnResponse(_res);

	IsConnecting = false;
	ConnectGauge->Die();
	ConnectGauge = NULL;


	ScoreRankLine  =  -1;
	TimeRankLine   =  -1;

	SameID_Score = -1;
	SameID_Time = -1;


	_res.seekp(0, ios::beg);
	stringstream::pos_type offset = _res.tellp();
	_res.seekp(0);
	ScoreRanking.clear();
	TimeRanking.clear();

	if( (offset > 0) ) {

		s32 Result;
		//応答結果取得
		_res.read((char*)&Result,sizeof(s32));

		//問題なければ以下を実施
		if(Result == 0 ) {
			ConnectResult  =  KCONNECT_SUCCESS;

			//スコアランクイン結果を取得
			_res.read((char*)&ScoreRankLine,sizeof(s32));

			//タイムランクイン結果を取得
			_res.read((char*)&TimeRankLine,sizeof(s32));

			if( FOwner->IsSequence() ) {
				//
				_res.read((char*)&SameID_Score,sizeof(s32));
				_res.read((char*)&SameID_Time,sizeof(s32));
			}

			//スコアランクのサイズを取得
			u32 ReadEndSize;
			_res.read((char*)&ReadEndSize,sizeof(u32));

			//ランクのサイズから、最終的に読み込むサイズを算出
			ReadEndSize  =  ReadEndSize + (u32)_res.tellp();

			//取得したデータをそれぞれのデータに格納
			TotalScoreData  =  0;
			while(_res.tellp() < ReadEndSize )
			{
				NetRankingData newData;

				_res.read((char*)&newData.Rank,sizeof(s32));
				for( u32 i = 0 ; i <= 7; i++ )
					_res.read((char*)&newData.Name[i],sizeof(s32));

				_res.read((char*)&newData.Score,sizeof(s32));
				_res.read((char*)&newData.Time,sizeof(s32));
				_res.read((char*)&newData.BoostType,sizeof(s32));
				_res.read((char*)&newData.WeaponType,sizeof(s32));
				_res.read((char*)&newData.FrameSkipRate,sizeof(s32));
				_res.read((char*)&newData.Year,sizeof(s32));
				_res.read((char*)&newData.Month,sizeof(s32));
				_res.read((char*)&newData.Day,sizeof(s32));
				_res.read((char*)&newData.FileNumber,sizeof(s32));

				if( newData.Rank <= KNETRANK_COMMOM_MAXRANK )
					TotalScoreData++;

				ScoreRanking.push_back(newData);
			}

			//タイムランクのサイズを取得
			_res.read((char*)&ReadEndSize,sizeof(u32));

			//ランクのサイズから、最終的に読み込むサイズを算出
			ReadEndSize  =  ReadEndSize + (u32)_res.tellp();

			//タイムデータを取得
			TotalTimeData  =  0;
			while( _res.tellp() < ReadEndSize )
			{
				NetRankingData newData;
				_res.read((char*)&newData.Rank,sizeof(s32));
				for( u32 i = 0 ; i <= 7; i++ )
					_res.read((char*)&newData.Name[i],sizeof(s32));

				_res.read((char*)&newData.Score,sizeof(s32));
				_res.read((char*)&newData.Time,sizeof(s32));
				_res.read((char*)&newData.BoostType,sizeof(s32));
				_res.read((char*)&newData.WeaponType,sizeof(s32));
				_res.read((char*)&newData.FrameSkipRate,sizeof(s32));
				_res.read((char*)&newData.Year,sizeof(s32));
				_res.read((char*)&newData.Month,sizeof(s32));
				_res.read((char*)&newData.Day,sizeof(s32));
				_res.read((char*)&newData.FileNumber,sizeof(s32));

				if( newData.Rank <= KNETRANK_COMMOM_MAXRANK )
					TotalTimeData++;

				ScoreRanking.push_back(newData);

			}
			//ランク外の場合はランキングMAXより大きい値にする
			if( ScoreRankLine < 0 ) ScoreRankLine  =  glm::max(TotalScoreData, KNETRANK_COMMOM_MAXRANK) + 1;
			if( TimeRankLine < 0 ) TimeRankLine  =  glm::max(TotalTimeData, KNETRANK_COMMOM_MAXRANK) + 1;

			if( (TotalScoreData > 0) ) {
				ActFlg[0]  =  2;
				MakeRankingItems(&ScoreRanking[0], TotalScoreData, ScoreRankLine, SameID_Score);
			}
			else if( (TotalTimeData>0) ) {
				ActFlg[0]  =  3;
				MakeRankingItems(&TimeRanking[0], TotalTimeData, TimeRankLine, SameID_Time);
			}
		}
		else {  //応答結果異常の場合
			ActFlg[0]  =  4;
			ErrorNo = Result;
			ConnectResult  =  KCONNECT_ERROR;
			Arrow_Right->Hide();
			Arrow_Left->Hide();
		}
	}
	else {
		ActFlg[0]  =  4;
		ConnectResult  =  KCONNECT_ERROR;
		Arrow_Right->Hide();
		Arrow_Left->Hide();
	}
}

void TNetRankingReplayViewer::AddNextItems()
{
	if( TotalData <= 0 ) return;

	s32 offset  =  CurrentItemNumber;

	//画面中の最大表示数を求める
	UpdateCurrentItems();

	for( s32 i = 0 ; i < CurrentItems; i++ ) {
		TMojiAndGlassCursor* tmpObj  = new TMojiAndGlassCursor(FOwner,OrgX+16,OrgY+FirstYPos_Ranking+i*YSPACE,576,GLASSCURSOR_PATTERN_GLASS);
		FOwner->Add(tmpObj);
		CursorList.push_back(tmpObj);

		Cursor->SetMovePoint(16,FirstYPos_Ranking+i*YSPACE);

		//ランク
		tmpObj->AddSentenceRight(InttoStr(RankingData[offset+i].Rank), 64,0,255,255,255,KFONTKIND_NORMAL);

		//名前
		tmpObj->AddSentence(FOwner->PlayerNameToStr(RankingData[offset+i].Name, MAX_NAME_LENGTH), 88,0,255,255,255,KFONTKIND_NORMAL);

		switch( RankingMode ){
		case 0:{
			//スコア
			tmpObj->AddSentence(FOwner->ScoreToStr(RankingData[offset+i].Score), 256,0,255,255,255,KFONTKIND_NORMAL);
			   }break;
		case 1:{
			//タイム
			if( CurrentStage <= 4 )
				tmpObj->AddSentenceRight(FOwner->TimeToStr(RankingData[offset+i].Time), 368,0,255,255,255,KFONTKIND_NORMAL);
			else
				tmpObj->AddSentenceRight(TotalTimeToStr(RankingData[offset+i].Time), 400,0,255,255,255,KFONTKIND_NORMAL);
			   }break;
		}

		//日付
		tmpObj->AddSentenceRight(FOwner->DateToStr(RankingData[offset+i].Year,
			RankingData[offset+i].Month,
			RankingData[offset+i].Day),
			592,0,255,255,255,KFONTKIND_NORMAL);

		MakePartition(tmpObj);

		if( (Age == 0) ) {
			tmpObj->SetSpeed(7, 7);
			tmpObj->SetDest(16,FirstYPos_Ranking+i*YSPACE,i*5);
			tmpObj->SetAlpha(0);
			tmpObj->SetDestAlpha(255,15,i*7);
		}
		else {
			tmpObj->SetSpeed(5, 5);
			tmpObj->SetDest(16,FirstYPos_Ranking+i*YSPACE,0);
			tmpObj->SetAlpha(0);
			tmpObj->SetDestAlpha(255,15,0);
		}
	}
}

void TNetRankingReplayViewer::Input()
{
	for( u32 i = 0 ; i <= 7; i++ ){
		if( ! ButtonON[i] ) ButtonFlg[i] = false;
	}

	if( (! ButtonFlg[0]) && ButtonON[0] ) {
		ButtonFlg[0] = true;
		switch( CurrentStatus ){
		case KNETRANK_SCR_STAGE:{
			CurrentStage  =  Choice + 5;
			SetNextStatus(KNETRANK_SCR_ORDER);
								}break;
		case KNETRANK_SCR_ORDER:{
			RankingMode  =  Choice;
			SetNextStatus(KNETRANK_SCR_RANKING);
								}break;
		case KNETRANK_SCR_RANKING:{
			if( ConnectResult = KCONNECT_SUCCESS ) {
				if( TotalData>0 ) SetNextStatus(KNETRANK_SCR_DETAIL);
			}
			else {
				//接続エラー時は前画面に戻す
				ConnectResult  =  KCONNECT_SUCCESS;
				SetNextStatus(KNETRANK_SCR_ORDER);
				BustShot_Rifle->Hide();
				BustShot_Blade->Hide();
			}
								  }break;
		case KNETRANK_SCR_DETAIL:{
			if( ConnectResult = KCONNECT_SUCCESS ) {
				if( TotalData>0 ) {
					GetReplay(Choice + CurrentItemNumber);
					if( CreateSE ) FOwner->SoundEffect->WavPlay(WADeside,1);
				}
			}
			else {
				//接続エラー時は前画面に戻す
				ConnectResult  =  KCONNECT_SUCCESS;
				SetNextStatus(KNETRANK_SCR_DETAIL_RANK);
			}

								 }break;
		}
	}

	if( (! ButtonFlg[1]) && ButtonON[1] ) {
		ButtonFlg[1] = true;
		//前の画面へ
		switch( CurrentStatus ){
		case KNETRANK_SCR_STAGE:{
			Die();
			FOwner->NetReplayState = -1;
			FOwner->AfterLoopEvent = MTitle;
								}break;
		case KNETRANK_SCR_ORDER:{
			SetNextStatus(KNETRANK_SCR_STAGE);
								}break;
		case KNETRANK_SCR_RANKING:{
			ConnectResult  =  KCONNECT_SUCCESS;
			SetNextStatus(KNETRANK_SCR_ORDER);
			BustShot_Rifle->Hide();
			BustShot_Blade->Hide();
			FOwner->NetReplayState = -1;
								  }break;
		case KNETRANK_SCR_DETAIL:{
			ConnectResult  =  KCONNECT_SUCCESS;
			SetNextStatus(KNETRANK_SCR_DETAIL_RANK);
								 }break;
		}
	}

	if( FOwner->Joy.StickX() == 0 ) StickFlgX = false;
	if( FOwner->Joy.StickY() == 0 ) StickFlgY = false;

	if( ConnectResult == KCONNECT_SUCCESS ) {

		if( (FOwner->Joy.StickY() == 1) ) {
			KeyDownCount++;
			if( (StickFlgY == false) || (KeyDownCount > 30) ) {    //新規に押下するか、60フレーム押し続ければ処理を有効
				if( (KeyDownCount > 30) ) KeyDownCount  =  25;  //キー押し続けの場合、カーソル移動が早くなりすぎるのを防ぐ
				if( Cursor  !=  NULL ) {
					Cursor->NextPoint(1);
				}
				if( CurrentItems > 0 ) {
					Choice++;
					if( (Choice >= CurrentItems) ) {
						Choice = 0;
						if( (CurrentItems != TotalData) &&
							((CurrentStatus == KNETRANK_SCR_RANKING) || (CurrentStatus == KNETRANK_SCR_DETAIL)) ) 
							MakeRightItems();
					}

					if( CurrentStatus == KNETRANK_SCR_DETAIL ) ShowDetail();
					if( CurrentStatus == KNETRANK_SCR_STAGE ) {
						//              TStageSelectScreenMng(FOwner->StageSelect).ChangeStage(Choice);
						BustShot_Rifle->ChangeColor(Choice);
						BustShot_Blade->ChangeColor(Choice);
					}

					UpdateBustShot();


					if( CreateSE ) FOwner->SoundEffect->WavPlay(WASelect,0);
				}
				StickFlgY = true;
			}
		}
		else {
			KeyDownCount  =  0;
		}

		if( (FOwner->Joy.StickY() == -1) ) {
			KeyUpCount++;
			if( (StickFlgY == false) || (KeyUpCount > 30)  ) {
				if( (KeyUpCount > 30) ) KeyUpCount  =  25;
				if( CurrentItems > 0 ) {
					Choice--;
					if( (Choice < 0) ) {
						if( (CurrentItems != TotalData) &&
							((CurrentStatus == KNETRANK_SCR_RANKING) || (CurrentStatus == KNETRANK_SCR_DETAIL)) ) MakeLeftItems();
						Choice = CurrentItems - 1;
					}


					if( CurrentStatus == KNETRANK_SCR_DETAIL ) ShowDetail();
					if( CurrentStatus == KNETRANK_SCR_STAGE ) {
						//              TStageSelectScreenMng(FOwner->StageSelect).ChangeStage(Choice);
						BustShot_Rifle->ChangeColor(Choice);
						BustShot_Blade->ChangeColor(Choice);
					}
					UpdateBustShot();

					if( CreateSE ) FOwner->SoundEffect->WavPlay(WASelect,0);
				}
				if( Cursor  !=  NULL ) {
					Cursor->NextPoint(-1);
				}
				StickFlgY = true;
			}
		}
		else {
			KeyUpCount  =  0;
		}

		if( CurrentStatus == KNETRANK_SCR_RANKING ) {
			if( (StickFlgX == false) && (FOwner->Joy.StickX() == 1) ) {
				if( (CurrentItems != TotalData) ) {
					MakeRightItems();
					if( (Choice >= CurrentItems) ) {
						Choice = CurrentItems - 1;
						Cursor->MoveToEnd();
					}
					else {
						Cursor->NextPoint(Choice);
					}
				}
				StickFlgX = true;
				UpdateBustShot();
				if( CreateSE ) FOwner->SoundEffect->WavPlay(WASelect,0);
			}

			if( (StickFlgX == false) && (FOwner->Joy.StickX() == -1) ) {
				if( CurrentItems != TotalData ) {
					MakeLeftItems();
					if( (Choice >= CurrentItems) ) {
						Choice = CurrentItems - 1;
						Cursor->MoveToEnd();
					}
					else {
						Cursor->NextPoint(Choice);
					}
				}
				StickFlgX = true;
				UpdateBustShot();
				if( CreateSE ) FOwner->SoundEffect->WavPlay(WASelect,0);
			}
		}
	}
}

void TNetRankingReplayViewer::GetData()
{

	//前回のランキング項目を消去
	if( CursorList.size() > 0 ) {
		for( u32 i = 0 ; i < CursorList.size(); i++ ) {
			CursorList[i]->Die();
		}
	}
	CursorList.clear();
	CurrentItemNumber  =  0;

	//リクエストデータ作成
	s32 Request;
	if( FOwner->NetReplayState >= 0 ) {
		RankingMode  =  FOwner->NetReplayState % 2;
		Request      =  FOwner->NetReplayState / 2;
	}
	else {
		Request = CurrentStage;
	}

	FOwner->NetReplayState = Request*2 + RankingMode;  //リプレイ終了後、戻ってくる画面を記憶する(偶数：スコア、奇数：タイムランクとなる)

	//ステージ追加によるリクエストデータ補正
	string url, cgipath;
	if( Request > 4 ) {
		Request = Request-5;
		cgipath  =  NETRANKING_SEQUENCE_CGI_PATH;
	}
	else {
		cgipath  =  NETRANKING_CGI_PATH;
	}

	//リクエストURL決定
	switch( RankingMode) {
	case 0:url  =  cgipath + "returnscore.cgi";	break;
	case 1:url  =  cgipath + "returntime.cgi";	break;
	}

	//接続中ゲージ生成
	IsConnecting = true;
	ConnectGauge  = new TConnectingGauge(FOwner);
	FOwner->Add(ConnectGauge);

	stringstream memStream;
	memStream.write((char*)Request, sizeof(s32));
	memStream.seekg(0);
	TPostThread(this, url, memStream);
}

s32 TNetRankingReplayViewer::GetReplay( s32 _choice )
{
	s32 Ret  =  0;
	//選択カーソル位置から再生ファイル番号を取得
	string filename  =  InttoStr(RankingData[_choice].FileNumber);

	//ステージ追加によるリクエストデータ補正
	string cgipath;
	if( (FOwner->NetReplayState / 2) > 4 ) {
		cgipath  =  NETRANKING_SEQUENCE_CGI_PATH;
	}
	else {
		cgipath  =  NETRANKING_CGI_PATH;
	}

	ClearScreen();
	IsConnecting = true;
	ConnectGauge  = new TConnectingGauge(FOwner);
	FOwner->Add(ConnectGauge);

	TGetThread(this,cgipath +"data/" + filename);

	return Ret;
}

void TNetRankingReplayViewer::ShowDetail()
{
	if( CursorList.size() > 0 ) {
		for( u32 i = 0 ; i < CursorList.size(); i++ ) {
			CursorList[i]->Die();
		}
	}
	CursorList.clear();


	s32 itemnum  =  Choice + CurrentItemNumber;

	string btype;
	string wtype;
	if(RankingData[itemnum].BoostType == 0)
		btype  =  "alternate";
	else
		btype  =  "momentary";

	if(RankingData[itemnum].WeaponType == 0 )
		wtype  =  "rifle";
	else
		wtype  =  "blade";

	OrgX = -640;
	OrgY = 0;
	//名前
	MakeCursor(FOwner->PlayerNameToStr(RankingData[itemnum].Name, MAX_NAME_LENGTH), 384,112, 48, GLASSCURSOR_PATTERN_NONE, KFONTKIND_BLIGHT, 0);

	//難易度
	string diff;
	switch( CurrentStage ){
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:diff  =  "single";	break;
	case 5:diff  =  "easy";		break;
	case 6:diff  =  "normal";	break;
	case 7:diff  =  "hard";		break;
	}
	MakeCursor("difficulty",  16,128+1*YSPACE, 576, GLASSCURSOR_PATTERN_BLIND, KFONTKIND_NORMAL, 1);
	CursorList[1]->AddSentenceRight(diff, 576,0,255,255,255,KFONTKIND_NORMAL);

	//スコア
	MakeCursor("score",  16,128+2*YSPACE, 576, GLASSCURSOR_PATTERN_BLIND, KFONTKIND_NORMAL, 1);
	CursorList[2]->AddSentenceRight(FOwner->ScoreToStr(RankingData[itemnum].Score), 576,0,255,255,255,KFONTKIND_NORMAL);

	//タイム
	MakeCursor("time",  16,128+3*YSPACE, 576, GLASSCURSOR_PATTERN_BLIND, KFONTKIND_NORMAL, 1);
	if( CurrentStage <= 4 )
		CursorList[3]->AddSentenceRight(FOwner->TimeToStr(RankingData[itemnum].Time), 576,0,255,255,255,KFONTKIND_NORMAL);
	else
		CursorList[3]->AddSentenceRight(TotalTimeToStr(RankingData[itemnum].Time), 576,0,255,255,255,KFONTKIND_NORMAL);

	//ブーストタイプ
	MakeCursor("boost type",  16,128+4*YSPACE, 576, GLASSCURSOR_PATTERN_BLIND, KFONTKIND_NORMAL, 1);
	CursorList[4]->AddSentenceRight(btype, 576,0,255,255,255,KFONTKIND_NORMAL);

	//ウェポンタイプ
	MakeCursor("weapon type",  16,128+5*YSPACE, 576, GLASSCURSOR_PATTERN_BLIND, KFONTKIND_NORMAL, 1);
	CursorList[5]->AddSentenceRight(wtype, 576,0,255,255,255,KFONTKIND_NORMAL);

	//時間
	MakeCursor("date",  16,128+6*YSPACE, 576, GLASSCURSOR_PATTERN_BLIND, KFONTKIND_NORMAL, 1);
	CursorList[6]->AddSentenceRight(FOwner->DateToStr(RankingData[itemnum].Year,
		RankingData[itemnum].Month,
		RankingData[itemnum].Day), 576,0,255,255,255,KFONTKIND_NORMAL);

	//フレームスキップ
	MakeCursor("frameskip", 16,128+7*YSPACE, 576, GLASSCURSOR_PATTERN_BLIND, KFONTKIND_NORMAL, 1);
	CursorList[7]->AddSentenceRight(FOwner->FrameSkipRateToStr(RankingData[itemnum].FrameSkipRate), 576,0,255,255,255,KFONTKIND_NORMAL);

	OrgX = 16;
	OrgY = 128;

	//順位
	TMojiAndGlassCursor* tmpObj  = new TMojiAndGlassCursor(FOwner,OrgX-640,64,48,GLASSCURSOR_PATTERN_NONE);
	FOwner->Add(tmpObj);
	CursorList.push_back(tmpObj);
	tmpObj->SetRank(RankingData[itemnum].Rank);
	tmpObj->SetSpeed(5, 5);
	tmpObj->SetDest(OrgX,64,0);
	tmpObj->SetAlpha(0);
	tmpObj->SetDestAlpha(255,10,0);

	//区切り線
	tmpObj  = new TMojiAndGlassCursor(FOwner,16-640,136,608,GLASSCURSOR_PATTERN_LINE);
	FOwner->Add(tmpObj);
	CursorList.push_back(tmpObj);
	tmpObj->SetSpeed(5, 5);
	tmpObj->SetDest(16,136,0);
	tmpObj->SetAlpha(0);
	tmpObj->SetDestAlpha(200,15,0);
}

void TNetRankingReplayViewer::MakeRightItems()
{
	CurrentItemNumber += KNETRANK_REP_MAX;

	if( CurrentItemNumber >= TotalData ) CurrentItemNumber  =  0;

	if( CurrentStatus == KNETRANK_SCR_RANKING ) {
		//項目生成位置
		OrgX  =  640;
		OrgY  =  0;

		//現在表示中の項目を左へ流す
		for( u32 i = 0 ; i < CursorList.size(); i++ ) {
			TMojiAndGlassCursor* tmpObj =  CursorList[i];
			tmpObj->SetSpeed(5, 5);
			tmpObj->SetDest(-640+16,tmpObj->Y,0);
			tmpObj->SetDestAlpha(0,15,0);
			tmpObj->SetDeathPoint(-640+16,tmpObj->Y);
		}

		CursorList.clear();

		if( Cursor  !=  NULL ) Cursor->ClearPoints();

		AddNextItems();
	}
	else 
		UpdateCurrentItems();

}

void TNetRankingReplayViewer::MakeLeftItems()
{
	CurrentItemNumber -= KNETRANK_REP_MAX;

	if( (CurrentItemNumber < 0) )
		CurrentItemNumber  =  ((TotalData-1) / KNETRANK_REP_MAX)*KNETRANK_REP_MAX;

	if( CurrentStatus == KNETRANK_SCR_RANKING ) {
		//項目生成位置
		OrgX  =  -640+16;
		OrgY  =  0;

		//現在表示中の項目を右へ流す
		for( u32 i = 0 ; i < CursorList.size(); i++ ) {
			TMojiAndGlassCursor* tmpObj =  CursorList[i];
			tmpObj->SetSpeed(5, 5);
			tmpObj->SetDest(640+16,tmpObj->Y,0);
			tmpObj->SetDestAlpha(0,15,0);
			tmpObj->SetDeathPoint(640+16,tmpObj->Y);
		}

		CursorList.clear();

		if( Cursor  !=  NULL ) Cursor->ClearPoints();
		AddNextItems();
	}
	else UpdateCurrentItems();
}

void TNetRankingReplayViewer::SetNextStatus( s32 _next )
{
	string caption[8];

	ClearScreen();
	if( Cursor  !=  NULL ) Cursor->ClearPoints();
	switch(_next){
	case KNETRANK_SCR_STAGE:{
		caption[0]  =  "sequence  easy";
		caption[1]  =  "sequence  normal";
		caption[2]  =  "sequence  hard";
		caption[3]  =  "episode 1";
		caption[4]  =  "episode 2";
		caption[5]  =  "episode 3";
		caption[6]  =  "episode 4";
		caption[7]  =  "episode 5";

		OrgX = -640;
		OrgY = 0;
		//名前
		MakeCursor("select a difficulty", 0,112-48, 48, GLASSCURSOR_PATTERN_NONE, KFONTKIND_BLIGHT, 0);

		//区切り線
		TMojiAndGlassCursor* tmpObj  = new TMojiAndGlassCursor(FOwner,16-640,112-24,608,GLASSCURSOR_PATTERN_LINE);
		FOwner->Add(tmpObj);
		CursorList.push_back(tmpObj);
		tmpObj->SetSpeed(5, 5);
		tmpObj->SetDest(16,112-24,0);
		tmpObj->SetAlpha(0);
		tmpObj->SetDestAlpha(200,15,0);

		OrgX = 16;
		OrgY = 112;

		//カーソル生成
		Cursor  = new TGlassCursor(FOwner,OrgX, OrgY, 576);
		FOwner->Add(Cursor);
		Choice  =  0;

		if( CurrentStatus == KNETRANK_SCR_ORDER ) ClearBGItems();

		CurrentItems = 0;
		for( u32 i = 0 ; i <= 2; i++ ) {
			if( (i == 2) && (! FOwner->IsNormalCleared) ) continue;    //ノーマル未クリアならHardは表示しない

			tmpObj  = new TMojiAndGlassCursor(FOwner,OrgX-320,OrgY+i*YSPACE,576,GLASSCURSOR_PATTERN_GLASS);
			FOwner->Add(tmpObj);
			CursorList.push_back(tmpObj);
			tmpObj->AddSentence(caption[i], 64,0,255,255,255,KFONTKIND_NORMAL);
			Cursor->SetMovePoint(OrgX,OrgY+i*YSPACE);

			tmpObj->SetSpeed(5, 5);
			tmpObj->SetDest(OrgX,OrgY+i*YSPACE,i*5);
			tmpObj->SetAlpha(0);
			tmpObj->SetDestAlpha(255,10,i*7);

			CurrentItems++;
		}
							}break;
	case KNETRANK_SCR_ORDER:{
		caption[0]  =  "ordered by score";
		caption[1]  =  "ordered by time";

		OrgX = -640;
		OrgY = 0;
		//名前
		MakeCursor("select sorting order", 0,128-48, 48, GLASSCURSOR_PATTERN_NONE, KFONTKIND_BLIGHT, 0);

		//区切り線
		TMojiAndGlassCursor* tmpObj  = new TMojiAndGlassCursor(FOwner,16-640,128-24,608,GLASSCURSOR_PATTERN_LINE);
		FOwner->Add(tmpObj);
		CursorList.push_back(tmpObj);
		tmpObj->SetSpeed(5, 5);
		tmpObj->SetDest(16,128-24,0);
		tmpObj->SetAlpha(0);
		tmpObj->SetDestAlpha(200,15,0);

		OrgX = 16;
		OrgY = 128;

		//カーソル生成
		Cursor  = new TGlassCursor(FOwner,OrgX, OrgY, 576);
		FOwner->Add(Cursor);
		Choice  =  0;

		if( CurrentStatus = KNETRANK_SCR_RANKING ) ClearBGItems();

		for( u32 i = 0 ; i <= 1; i++ ) {
			TMojiAndGlassCursor* tmpObj  = new TMojiAndGlassCursor(FOwner,OrgX-320,OrgY+i*YSPACE,576,GLASSCURSOR_PATTERN_GLASS);
			FOwner->Add(tmpObj);
			CursorList.push_back(tmpObj);
			tmpObj->AddSentence(caption[i], 64,0,255,255,255,KFONTKIND_NORMAL);
			Cursor->SetMovePoint(OrgX,OrgY+i*YSPACE);

			tmpObj->SetSpeed(5, 5);
			tmpObj->SetDest(OrgX,OrgY+i*YSPACE,i*5);
			tmpObj->SetAlpha(0);
			tmpObj->SetDestAlpha(255,10,i*7);
		}

		CurrentItems  =  2;

							}break;
	case KNETRANK_SCR_RANKING:{
		//カーソル生成
		Cursor  = new TGlassCursor(FOwner,16, FirstYPos_Ranking, 576);
		FOwner->Add(Cursor);
		Cursor->Hide();    //最初は非表示
		Choice  =  0;
		Age  =  0;
		OrgX = 0;
		OrgY = 480;
		GetData();
							  }break;
	case KNETRANK_SCR_DETAIL:{
		OrgX = 16;
		OrgY = 128;
		ClearBGItems();
		ShowDetail();
							 }break;
	case KNETRANK_SCR_DETAIL_RANK:{
		//カーソル生成
		Cursor  = new TGlassCursor(FOwner,16, FirstYPos_Ranking+Choice*YSPACE, 576);
		FOwner->Add(Cursor);
		OrgX = 640+16;
		OrgY = 0;
		AddNextItems();
		Cursor->NextPoint(Choice);
		MakeItemName();
		_next  =  KNETRANK_SCR_RANKING;
								  }break;
	}

	CurrentStatus  =  _next;
}

void TNetRankingReplayViewer::ClearScreen()
{
	if( Cursor  !=  NULL ) {
		Cursor->Die();
		Cursor  =  NULL;
	}

	switch( CurrentStatus ){
	case KNETRANK_SCR_STAGE:
	case KNETRANK_SCR_ORDER:
	case KNETRANK_SCR_RANKING:{
		//現在表示中の項目を右へ流す
		for( u32 i = 0 ; i < CursorList.size(); i++ ) {
			TMojiAndGlassCursor* tmpObj =  CursorList[i];
			tmpObj->SetSpeed(5, 5);
			tmpObj->SetDest(640+16,tmpObj->Y,0);
			tmpObj->SetDestAlpha(0,15,0);
			tmpObj->SetDeathPoint(640+16,tmpObj->Y);
		}
							  }break;
	case KNETRANK_SCR_DETAIL:{
		//現在表示中の項目を左へ流す
		for( u32 i = 0 ; i < CursorList.size(); i++ ) {
			TMojiAndGlassCursor* tmpObj =  CursorList[i];
			tmpObj->SetSpeed(5, 5);
			tmpObj->SetDest(-640+16,tmpObj->Y,0);
			tmpObj->SetDestAlpha(0,15,0);
			tmpObj->SetDeathPoint(-640+16,tmpObj->Y);
		}
							 }break;
	}

	CursorList.clear();
}

void TNetRankingReplayViewer::UpdateCurrentItems()
{
	//画面中の最大表示数を求める
	if( (TotalData - CurrentItemNumber) < KNETRANK_REP_MAX )
		CurrentItems  =  (TotalData - CurrentItemNumber);
	else
		CurrentItems  =  KNETRANK_REP_MAX;
}

void TNetRankingReplayViewer::MakePartition( TMojiAndGlassCursor* _cursor )
{

}

void TNetRankingReplayViewer::MakeItemName()
{
	//区切り線
	TMojiAndGlassCursor* tmpObj  = new TMojiAndGlassCursor(FOwner,16-640,FirstYPos_Ranking - 24,608,GLASSCURSOR_PATTERN_LINE);
	FOwner->Add(tmpObj);
	BGItemList.push_back(tmpObj);
	tmpObj->SetSpeed(5, 5);
	tmpObj->SetDest(16,FirstYPos_Ranking - 24,0);
	tmpObj->SetAlpha(0);
	tmpObj->SetDestAlpha(200,15,0);

	//名前
	MakeSentence("name", 120, FirstYPos_Ranking - 48, 0);

	switch( RankingMode ){
	case 0:{
		//スコア
		MakeSentence("score", 296, FirstYPos_Ranking - 48, 1);

		   }break;
	case 1:{
		//タイム
		MakeSentence("time", 296, FirstYPos_Ranking - 48, 2);
		   }break;
	}

	//日付
	MakeSentence("date", 488, FirstYPos_Ranking - 48, 3);
}

void TNetRankingReplayViewer::MakeSentence( const string& _caption, s32 _x, s32 _y, s32 _delay )
{
	TMojiAndGlassCursor* tmpObj  = new TMojiAndGlassCursor(FOwner,_x-640,_y,48,GLASSCURSOR_PATTERN_NONE);
	FOwner->Add(tmpObj);
	BGItemList.push_back(tmpObj);
	tmpObj->AddSentence(_caption, 0,4,255,255,255,KFONTKIND_BLIGHT);
	tmpObj->SetSpeed(5, 5);
	tmpObj->SetDest(_x,_y,_delay*5);
	tmpObj->SetAlpha(0);
	tmpObj->SetDestAlpha(255,15,_delay*7);
}

void TNetRankingReplayViewer::ClearBGItems()
{
	//現在表示中の項目を左へ流す
	for( u32 i = 0 ; i < BGItemList.size(); i++ ) {
		TMojiAndGlassCursor* tmpObj =  BGItemList[i];
		tmpObj->SetSpeed(5, 5);
		tmpObj->SetDest(-640,tmpObj->Y,0);
		tmpObj->SetDestAlpha(0,15,0);
		tmpObj->SetDeathPoint(-640,tmpObj->Y);
	}

	BGItemList.clear();
}

void TNetRankingReplayViewer::MakeCursor( const string& _caption, s32 _x, s32 _y, s32 _long, s32 _pat, s32 _font, s32 _delay )
{
	TMojiAndGlassCursor* tmpObj  = new TMojiAndGlassCursor(FOwner,OrgX+_x,OrgY+_y,_long,_pat);
	FOwner->Add(tmpObj);
	CursorList.push_back(tmpObj);

	tmpObj->AddSentence(_caption, 32, 4, 255, 255, 255,_font);

	tmpObj->SetSpeed(5, 5);
	tmpObj->SetDest(_x,_y,_delay*2);
	tmpObj->SetAlpha(0);
	tmpObj->SetDestAlpha(255,10,_delay*2);
}

void TNetRankingReplayViewer::UpdateBustShot()
{
	if( !((CurrentStatus == KNETRANK_SCR_RANKING) || (CurrentStatus == KNETRANK_SCR_DETAIL)) ) return;
	if( RankingData.size()>0 ) {

		if(RankingData[Choice + CurrentItemNumber].WeaponType == 0 ) {
			BustShot_Rifle->Show();
			BustShot_Blade->Hide();
		}
		else {
			BustShot_Rifle->Hide();
			BustShot_Blade->Show();
		}
	}
}

void TNetRankingReplayViewer::ReceiveRanking( stringstream& _res )
{
	//スコアデータを取得
	RankingData.clear();
	TotalData  =  0;

	_res.seekp(0, ios::beg);
	stringstream::pos_type offset = _res.tellp();
	_res.seekp(0);
	if( offset > 0 ) {
		//取得したデータをそれぞれのデータに格納
		while(_res.tellp() < offset )
		{
			NetRankingData newData;
			_res.read((char*)&newData.Rank,sizeof(s32));
			for( u32 i = 0 ; i <= 7; i++ )
				_res.read((char*)&newData.Name[i],sizeof(s32));

			_res.read((char*)&newData.Score,sizeof(s32));
			_res.read((char*)&newData.Time,sizeof(s32));
			_res.read((char*)&newData.BoostType,sizeof(s32));
			_res.read((char*)&newData.WeaponType,sizeof(s32));
			_res.read((char*)&newData.FrameSkipRate,sizeof(s32));
			_res.read((char*)&newData.Year,sizeof(s32));
			_res.read((char*)&newData.Month,sizeof(s32));
			_res.read((char*)&newData.Day,sizeof(s32));
			_res.read((char*)&newData.FileNumber,sizeof(s32));

			if( newData.Rank <= KNETRANK_COMMOM_MAXRANK )
				TotalData++;

			RankingData.push_back(newData);
		}

		ConnectResult  =  KCONNECT_SUCCESS;

		//データがあればカーソルを表示
		if( TotalData > 0 ) {
			Cursor->Show();
			Choice = 0;
		}
		AddNextItems();
		MakeItemName();
		UpdateBustShot();
	}
	else {
		ConnectResult  =  KCONNECT_SUCCESS;

		MakeItemName();
	}
}

void TNetRankingReplayViewer::ReceiveReplay( stringstream& _res )
{
	//再生
	_res.seekp(0, ios::beg);
	stringstream::pos_type offset = _res.tellp();
	_res.seekp(0);
	if( offset>0 ) {
		ConnectResult  =  KCONNECT_SUCCESS;
		FOwner->Replayer->LoadFromStream(_res,false);

		Die();
		FOwner->ReplayON = true;
		FOwner->Difficulty  =  FOwner->Replayer->RepDifficulty;
		FOwner->AfterLoopEvent = MLoadBeforePlay;
	}
	else {
		//エラー処理
		ConnectResult  =  KCONNECT_NOTFOUND;
	}
}

void TNetRankingReplayViewer::CopyToLocal( s32 _choice )
{

}

void TNetRankingReplayViewer::DeleteData()
{
	//選択カーソル位置から再生ファイル番号を取得
	s32 Request  =  RankingData[Choice + CurrentItemNumber].FileNumber;

	s32 stage = (FOwner->NetReplayState / 2);

	//ステージ追加によるリクエストデータ補正
	string cgipath;
	if( stage > 4 ) {
		stage = stage-5;
		cgipath  =  NETRANKING_SEQUENCE_CGI_PATH + "delete.cgi";;
	}
	else {
		cgipath  =  NETRANKING_CGI_PATH + "delete.cgi";
	}

	ClearScreen();
	IsConnecting = true;
	ConnectGauge  = new TConnectingGauge(FOwner);
	FOwner->Add(ConnectGauge);

	stringstream memStream;
	memStream.write((char*)&Request, sizeof(s32));
	memStream.write((char*)&stage, sizeof(s32));
	memStream.seekg(0);
	TPostThread(this,cgipath,memStream);


	//前回のランキング項目を消去
	if(CursorList.size() > 0 ) {
		for( u32 i = 0 ; i < CursorList.size(); i++ ) {
			CursorList[i]->Die();
		}
	}
	CursorList.clear();
	CurrentItemNumber  =  0;
}

TNetRankingReplayViewer::TNetRankingReplayViewer( TOBJList* owner ) : TOBJ(owner)
{
	FObjectName = "TNetRankingReplayViewer";
	FKind = kGauge;
	FOwner->GameMode = gTitle;
	Choice = 0;
	CurrentStage = 5;

	StickFlgX = true;
	StickFlgY = true;

	TotalData = 0;

	ConnectResult = KCONNECT_SUCCESS;
	IsConnecting = false;

	OrgX  =  0;
	OrgY  =  480;
	DestX  =  0;
	DestY  =  0;

	SlideTime  =  0;
	XSpeed  =  0;
	YSpeed  =  0;

	KeyUpCount  =  0;
	KeyDownCount  =  0;

	Cursor  =  NULL;

	CurrentItems  =  0;
	CurrentItemNumber  =  0;

	CurrentStatus  =  KNETRANK_SCR_ORDER;

	RankingMode  =  0;

	for( u32 i = 0 ; i <= 9; i++ ) {
		ActFlg[i] = 0;
		ActCount[i] = 0;
	}

	//音楽ストップ
	if( CreateBGM ) FOwner->MusicPlayer->FadeOut(2,0.0,0.5);

	BustShot_Rifle  = new TBustShot(FOwner,640,0,160,0,0,false);
	BustShot_Blade  = new TBustShot(FOwner,640,0,160,0,1,false);
	FOwner->Add(BustShot_Rifle);
	FOwner->Add(BustShot_Blade);
	BustShot_Rifle->Hide();
	BustShot_Blade->Hide();

	if( FOwner->NetReplayState<0 )
		SetNextStatus(KNETRANK_SCR_STAGE);
	else {
		SetNextStatus(KNETRANK_SCR_RANKING);
		CurrentStage = FOwner->NetReplayState / 2;

		((TStageSelectScreenMng*)FOwner->StageSelect)->ChangeStage(CurrentStage);
		BustShot_Rifle->ChangeColor(CurrentStage);
		BustShot_Blade->ChangeColor(CurrentStage);
	}
}

TNetRankingReplayViewer::~TNetRankingReplayViewer()
{
	if(CursorList.size() > 0 ) {
		for( u32 i = 0 ; i < CursorList.size(); i++ ) {
			CursorList[i]->Die();
		}
	}
	CursorList.clear();

	if(BGItemList.size() > 0 ) {
		for( u32 i = 0 ; i < BGItemList.size(); i++ ) {
			BGItemList[i]->Die();
		}
	}

	BGItemList.clear();

	if( Cursor  !=  NULL ) Cursor->Die();

	BustShot_Rifle->Die();
	BustShot_Blade->Die();

}

void TNetRankingReplayViewer::Move()
{
	TOBJ::Move();

	if( ! IsConnecting ) Input();

	//タイトル表示
	FOwner->Add( new TBox(FOwner,0,0,640,32,0,200,255));
	FOwner->SentenceCenter("network ranking",320,0,16,3);
	FOwner->Add( new TBox(FOwner,0,448,640,480,0,200,255));

	if(ConnectResult == KCONNECT_ERROR ) {
		FOwner->Add( new TBox(FOwner,0,224,640,256,0,200,128));
		FOwner->SentenceCenter("connection failed",320,224,0,3);
		return;   
	}

	if(ConnectResult == KCONNECT_NOTFOUND ) {
		FOwner->Add( new TBox(FOwner,0,224,640,256,0,200,128));
		FOwner->SentenceCenter("file ! found",320,224,0,3);
		return;   
	}
}

void TNetRankingReplayViewer::Die()
{
	TOBJ::Die();
}

void TNetRankingReplayViewer::OnResponse( stringstream& _res )
{
	TOBJ::OnResponse(_res);
	ConnectGauge->Die();
	IsConnecting = false;
	switch( CurrentStatus ){
	case KNETRANK_SCR_RANKING:ReceiveRanking(_res);	break;
	case KNETRANK_SCR_DETAIL:ReceiveReplay(_res);	break;
	}
}

void TConfig2::Input()
{

	for( u32 i = 0 ; i <= 15 ; i++)
		if( ! ButtonON[i] ) ButtonFlg[i] = false;

	if( CurrentStatus == KCONFIG_SCR_CONTROL ) KeyConfig();

	if( (! ButtonFlg[0]) && ButtonON[0] ) {
		ButtonFlg[0] = true;
		OnDecided();
	}

	if( (! ButtonFlg[1]) && ButtonON[1] ) {
		ButtonFlg[1] = true;
		PrevStatus();
	}

	if( FOwner->Joy.StickX() == 0 ) StickFlgX = false;
	if( FOwner->Joy.StickY() == 0 ) StickFlgY = false;

	if( (FOwner->Joy.StickY() == 1) ) {
		KeyDownCount++;
		if( (StickFlgY == false) || (KeyDownCount > 30) ) {    //新規に押下するか、60フレーム押し続ければ処理を有効
			if( KeyDownCount > 30 ) KeyDownCount  =  25;  //キー押し続けの場合、カーソル移動が早くなりすぎるのを防ぐ
			if( Cursor != NULL ) Cursor->NextPoint(1);
			Choice++;
			if( (Choice >= CurrentItems) ) {
				Choice = 0;
			}
			ArrowControl();
			StickFlgY = true;
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WASelect,0);
		}
	}
	else {
		KeyDownCount  =  0;
	}

	if( (FOwner->Joy.StickY() == -1) ) {
		KeyUpCount++;
		if( (StickFlgY == false) || (KeyUpCount > 30)  ) {
			if( (KeyUpCount > 30) ) KeyUpCount  =  25;
			Choice--;
			if( Choice < 0 ) {
				Choice = CurrentItems - 1;
			}
			if( Cursor != NULL ) Cursor->NextPoint(-1);
			ArrowControl();
			StickFlgY = true;
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WASelect,0);
		}
	}
	else {
		KeyUpCount  =  0;
	}


	if( (StickFlgX == false) && (FOwner->Joy.StickX() == 1) ) {
		StickFlgX = true;
		SettingList[Choice].Current++;
		UpdateSetting(64, 0, Choice, SettingList[Choice].Current,0);
	}

	if( (StickFlgX == false) && (FOwner->Joy.StickX() == -1) ) {
		StickFlgX = true;
		SettingList[Choice].Current--;
		UpdateSetting(-64, 0, Choice, SettingList[Choice].Current,0);
	}
}

void TConfig2::ClearToRight()
{
	//現在表示中の項目を右へ流す
	for( u32 i = 0 ; i < CursorList.size(); i++ ) {
		TMojiAndGlassCursor* tmpObj =  CursorList[i];
		tmpObj->SetSpeed(5, 5);
		tmpObj->SetDest(640+16,tmpObj->Y,0);
		tmpObj->SetDestAlpha(0,15,0);
		tmpObj->SetDeathPoint(640+16,tmpObj->Y);
	}

	CursorList.clear();
	SettingList.clear();

	if( Cursor  !=  NULL ) {
		Cursor->Die();
		Cursor  =  NULL;
	}
}

void TConfig2::ClearToLeft()
{
	//現在表示中の項目を左へ流す
	for( u32 i = 0 ; i < CursorList.size(); i++ ) {
		TMojiAndGlassCursor* tmpObj =  CursorList[i];
		tmpObj->SetSpeed(5, 5);
		tmpObj->SetDest(-640+16,tmpObj->Y,0);
		tmpObj->SetDestAlpha(0,15,0);
		tmpObj->SetDeathPoint(-640+16,tmpObj->Y);
	}

	CursorList.clear();
	SettingList.clear();

	if( Cursor  !=  NULL ) {
		Cursor->Die();
		Cursor  =  NULL;
	}
}

void TConfig2::SetNextStatus( s32 _next )
{
	CurrentItems  =  0;
	switch( _next ){
	case KCONFIG_SCR_TOP:{
		//カーソル生成
		Cursor  = new TGlassCursor(FOwner,16, FirstYPos, 296);
		FOwner->Add(Cursor);
		if( Age == 0 )
			OrgX = -640;
		else
			OrgX = 640;

		OrgY = 0;
		Choice  =  0;

		//タイトル
		MakeSentence("top menu", 16, FirstYPos -48, 0);
		MakeCursor("", 16, FirstYPos - 24, 608, GLASSCURSOR_PATTERN_LINE, false);

		//各項目
		//      MakeCursor("game settings", 16, FirstYPos + YSPACE * 0, 296, GLASSCURSOR_PATTERN_GLASS, false);
		MakeCursor("control settings", 16, FirstYPos + YSPACE * 0, 296, GLASSCURSOR_PATTERN_GLASS, false);
		MakeCursor("graphic settings", 16, FirstYPos + YSPACE * 1, 296, GLASSCURSOR_PATTERN_GLASS, false);
		MakeCursor("sound settings", 16, FirstYPos + YSPACE * 2, 296, GLASSCURSOR_PATTERN_GLASS, false);
		MakeCursor("network settings", 16, FirstYPos + YSPACE * 3, 296, GLASSCURSOR_PATTERN_GLASS, false);
		MakeCursor("return", 16, FirstYPos + YSPACE * 4, 296, GLASSCURSOR_PATTERN_GLASS, false);

		Caution->ClearCursors(0,0);
						 }break;
	case KCONFIG_SCR_GAMESETTING:{
		//カーソル生成
		Cursor  = new TGlassCursor(FOwner,16, FirstYPos, 576);
		FOwner->Add(Cursor);
		OrgX = -640;
		OrgY = 0;

		Choice  =  0;

		//タイトル
		MakeSentence("game settings", 16, FirstYPos -48, 0);
		MakeCursor("", 16, FirstYPos - 24, 608, GLASSCURSOR_PATTERN_LINE, false);

		//各設定項目作成
		//shield
		MakeCursor("shield", 16, FirstYPos + YSPACE * 0, 576, GLASSCURSOR_PATTERN_GLASS,true);
		AddSetting(0, "finite");
		AddSetting(0, "infinite");
		UpdateSetting(OrgX, OrgY, 0, BoolToInt(ShieldInfinite), 0);
		//mothership
		MakeCursor("mothership", 16, FirstYPos + YSPACE * 1, 576, GLASSCURSOR_PATTERN_GLASS,true);
		AddSetting(1, "finite");
		AddSetting(1, "infinite");
		UpdateSetting(OrgX, OrgY, 1, BoolToInt(MShipInfinite), 1);
		//collision
		MakeCursor("collision", 16, FirstYPos + YSPACE * 2, 576, GLASSCURSOR_PATTERN_GLASS,true);
		AddSetting(2, "off");
		AddSetting(2, "on");
		UpdateSetting(OrgX, OrgY, 2, BoolToInt(CollisionON), 2);
		//time
		MakeCursor("time", 16, FirstYPos + YSPACE * 3, 576, GLASSCURSOR_PATTERN_GLASS,true);
		AddSetting(3, "finite");
		AddSetting(3, "infinite");
		UpdateSetting(OrgX, OrgY, 3, BoolToInt(TimeInfinite), 3);
		//gamespeed
		MakeCursor("game speed", 16, FirstYPos + YSPACE * 4, 576, GLASSCURSOR_PATTERN_GLASS,true);
		AddSetting(4, "normal");
		AddSetting(4, "slow");
		UpdateSetting(OrgX, OrgY, 4, BoolToInt(SlowON), 4);

		MakeCursor("return", 16, FirstYPos + YSPACE * 6, 576, GLASSCURSOR_PATTERN_GLASS, false);

		CheckCheat();
								 }break;
	case KCONFIG_SCR_CONTROL:{
		//カーソル生成
		Cursor  = new TGlassCursor(FOwner,16, FirstYPos, 576);
		FOwner->Add(Cursor);
		OrgX = -640;
		OrgY = 0;

		Choice  =  0;

		//タイトル
		MakeSentence("key assign", 16, FirstYPos -48, 0);
		MakeCursor("", 16, FirstYPos - 24, 608, GLASSCURSOR_PATTERN_LINE, false);

		MakeCursor("shot", 16, FirstYPos + YSPACE * 0, 576, GLASSCURSOR_PATTERN_GLASS,false);
		for( u32 i = 0 ; i <= 15; i++ )
			AddSetting(0, "button "+ InttoStr(i));
		UpdateSetting(OrgX, OrgY, 0, TempKey[0], 0);

		MakeCursor("missile", 16, FirstYPos + YSPACE * 1, 576, GLASSCURSOR_PATTERN_GLASS,false);
		for( u32 i = 0 ; i <= 15; i++ )
			AddSetting(1, "button "+ InttoStr(i));
		UpdateSetting(OrgX, OrgY, 1, TempKey[1], 1);

		MakeCursor("boost", 16, FirstYPos + YSPACE * 2, 576, GLASSCURSOR_PATTERN_GLASS,false);
		for( u32 i = 0 ; i <= 15; i++ )
			AddSetting(2, "button "+ InttoStr(i));
		UpdateSetting(OrgX, OrgY, 2, TempKey[2], 2);

		MakeCursor("pause", 16, FirstYPos + YSPACE * 3, 576, GLASSCURSOR_PATTERN_GLASS,false);
		for( u32 i = 0 ; i <= 15; i++ )
			AddSetting(3, "button "+ InttoStr(i));
		UpdateSetting(OrgX, OrgY, 3, TempKey[3], 3);

		//タイトル
		MakeSentence("select boost type", 16, FirstYPos + YSPACE * 5 -32, 0);
		MakeCursor("", 16, FirstYPos + YSPACE * 5 - 8, 608, GLASSCURSOR_PATTERN_LINE, false);

		MakeCursor("boost type", 16, FirstYPos + YSPACE * 5 + 16, 576, GLASSCURSOR_PATTERN_GLASS,true);
		AddSetting(4, "alternate");
		AddSetting(4, "momentary");
		UpdateSetting(OrgX, OrgY, 4, BoostMode, 4);

		MakeCursor("return", 16, FirstYPos + YSPACE * 7, 576, GLASSCURSOR_PATTERN_GLASS, false);
							 }break;
	case KCONFIG_SCR_GRAPHIC:{
		//カーソル生成
		Cursor  = new TGlassCursor(FOwner,16, FirstYPos, 576);
		FOwner->Add(Cursor);
		OrgX = -640;
		OrgY = 0;

		Choice  =  0;

		//タイトル
		MakeSentence("graphic settings", 16, FirstYPos -48, 0);
		MakeCursor("", 16, FirstYPos - 24, 608, GLASSCURSOR_PATTERN_LINE, false);

		//各設定項目作成
		//effect level
		MakeCursor("effect level", 16, FirstYPos + YSPACE * 0, 576, GLASSCURSOR_PATTERN_GLASS,true);
		AddSetting(0, "low");
		AddSetting(0, "middle");
		AddSetting(0, "high");
		UpdateSetting(OrgX, OrgY, 0, EffLevel, 0);
		//color mode
		MakeCursor("color mode", 16, FirstYPos + YSPACE * 1, 576, GLASSCURSOR_PATTERN_GLASS,true);
		AddSetting(1, "16bit");
		AddSetting(1, "32bit");
		UpdateSetting(OrgX, OrgY, 1, TexCol, 1);
		//screen mode
		MakeCursor("screen mode", 16, FirstYPos + YSPACE * 2, 576, GLASSCURSOR_PATTERN_GLASS,true);
		AddSetting(2, "window");
		AddSetting(2, "fullscreen");
		UpdateSetting(OrgX, OrgY, 2, BoolToInt(ScreenMode), 2);
		//filter
		MakeCursor("smoothing", 16, FirstYPos + YSPACE * 3, 576, GLASSCURSOR_PATTERN_GLASS,true);
		AddSetting(3, "off");
		AddSetting(3, "on");
		UpdateSetting(OrgX, OrgY, 3, BoolToInt(FilterON), 3);
		//resolution
		MakeCursor("resolution", 16, FirstYPos + YSPACE * 4, 576, GLASSCURSOR_PATTERN_GLASS,true);
		AddSetting(4, "320 x 240");
		AddSetting(4, "640 x 480");
		AddSetting(4, "1024 x 768");
		AddSetting(4, "1280 x 960");
		UpdateSetting(OrgX, OrgY, 4, ScreenResolution, 4);
		//ex-effects
		MakeCursor("ex-effects", 16, FirstYPos + YSPACE * 5, 576, GLASSCURSOR_PATTERN_GLASS,true);
		AddSetting(5, "off");
		AddSetting(5, "on");
		UpdateSetting(OrgX, OrgY, 5, BoolToInt(ExtraEffects), 5);

		MakeCursor("return", 16, FirstYPos + YSPACE * 7, 576, GLASSCURSOR_PATTERN_GLASS, false);
							 }break;
	case KCONFIG_SCR_SOUND:{
		//カーソル生成
		Cursor  = new TGlassCursor(FOwner,16, FirstYPos, 576);
		FOwner->Add(Cursor);
		OrgX = -640;
		OrgY = 0;

		Choice  =  0;

		//タイトル
		MakeSentence("sound settings", 16, FirstYPos -48, 0);
		MakeCursor("", 16, FirstYPos - 24, 608, GLASSCURSOR_PATTERN_LINE, false);

		//se volume
		MakeCursor("se volume", 16, FirstYPos + YSPACE * 0, 576, GLASSCURSOR_PATTERN_GLASS,true);
		for( u32 i = 0 ; i <= 10; i++ )
			AddSetting(0, InttoStr(i*10));
		UpdateSetting(OrgX, OrgY, 0, SEVol, 0);

		//bgm volume
		MakeCursor("bgm volume", 16, FirstYPos + YSPACE * 1, 576, GLASSCURSOR_PATTERN_GLASS,true);
		for( u32 i = 0 ; i <= 10; i++ )
			AddSetting(1, InttoStr(i*10));
		UpdateSetting(OrgX, OrgY, 1, bgmVol, 1);

		MakeCursor("return", 16, FirstYPos + YSPACE * 3, 576, GLASSCURSOR_PATTERN_GLASS, false);
						   }break;
	case KCONFIG_SCR_NETWORK:{
		//カーソル生成
		Cursor  = new TGlassCursor(FOwner,16, FirstYPos, 576);
		FOwner->Add(Cursor);
		OrgX = -640;
		OrgY = 0;

		Choice  =  0;

		//タイトル
		MakeSentence("network settings", 16, FirstYPos -48, 0);
		MakeCursor("", 16, FirstYPos - 24, 608, GLASSCURSOR_PATTERN_LINE, false);

		//各設定項目作成
		//ネット接続
		MakeCursor("auto-upload", 16, FirstYPos + YSPACE * 0, 576, GLASSCURSOR_PATTERN_GLASS,true);
		AddSetting(0, "no");
		AddSetting(0, "yes");
		UpdateSetting(OrgX, OrgY, 0, BoolToInt(ConnectNet), 0);

		MakeCursor("return", 16, FirstYPos + YSPACE * 2, 576, GLASSCURSOR_PATTERN_GLASS, false);
							 }break;
	}

	CurrentStatus  =  _next;
	ArrowControl();
}

void TConfig2::MakeCursor( const string& _caption, s32 _x, s32 _y, s32 _long, s32 _pat, bool _horizontal )
{
	TMojiAndGlassCursor* tmpObj  = new TMojiAndGlassCursor(FOwner,OrgX+_x,OrgY+_y,_long,_pat);
	FOwner->Add(tmpObj);
	CursorList.push_back(tmpObj);

	tmpObj->AddSentence(_caption, 32, 0, 255, 255, 255,KFONTKIND_NORMAL);

	if( _pat == GLASSCURSOR_PATTERN_GLASS ) {
		ConfigCursor newCursor;
		Cursor->SetMovePoint(_x,_y);
		newCursor.Current  =  0;
		newCursor.Owner  =  tmpObj;
		newCursor.Spr  =  NULL;
		if( _horizontal ) tmpObj->AddLine(240);

		CurrentItems++;

		SettingList.push_back(newCursor);
	}

	tmpObj->SetSpeed(5, 5);
	tmpObj->SetDest(_x, _y, (SettingList.size()-1)*5);
	tmpObj->SetAlpha(0);
	tmpObj->SetDestAlpha(255, 10, (SettingList.size()-1)*7);

}

void TConfig2::AddSetting( s32 _num, const string& _caption )
{
	SettingList[_num].Horizontal  =  true;
	SettingList[_num].Caption.push_back(_caption);
}

void TConfig2::UpdateSetting( s32 _rx, s32 _ry, s32 _num, s32 _val, s32 _delay )
{
	if( ! SettingList[_num].Horizontal ) return;

	s32 maxsetting  =  SettingList[_num].Caption.size();

	if( maxsetting <= _val ) _val  =  0;
	if( _val < 0 ) _val  =  maxsetting - 1;

	//現在表示中の項目を消去
	if( SettingList[_num].Spr  !=  NULL ) {
		TMojiAndGlassCursor* tmpObj  =  SettingList[_num].Spr;
		tmpObj->SetSpeed(5, 5);
		tmpObj->SetDest(tmpObj->X-_rx,tmpObj->Y-_ry,0);
		tmpObj->SetDestAlpha(0,5,0);
		tmpObj->SetDeathPoint(tmpObj->X-_rx,tmpObj->Y-_ry);

		for(vector<TMojiAndGlassCursor*>::iterator itr = CursorList.begin(); itr != CursorList.end(); ++itr )
		{
			if( *itr == tmpObj )
				CursorList.erase(itr);
		}
	}

	TMojiAndGlassCursor* owner  =  SettingList[_num].Owner;
	string caption  =  SettingList[_num].Caption[_val];
	s32 textXPos  =  408 - (caption.size()*16 / 2);
	OrgX  =  _rx;
	OrgY  =  _ry;

	TMojiAndGlassCursor* tmpObj  = new TMojiAndGlassCursor(FOwner,OrgX+textXPos,OrgY+owner->Y,48,GLASSCURSOR_PATTERN_NONE);
	FOwner->Add(tmpObj);
	CursorList.push_back(tmpObj);
	tmpObj->AddSentence(caption, 32, 0, 128, 255, 128,KFONTKIND_NORMAL);

	SettingList[_num].Current  =  _val;
	SettingList[_num].Spr  =  tmpObj;

	//出現アニメーション設定
	tmpObj->SetSpeed(5, 5);
	tmpObj->SetDest(textXPos, owner->Y,_delay*5);
	tmpObj->SetAlpha(0);
	tmpObj->SetDestAlpha(255,10,_delay*7);


	//Game Settings時、設定が変更されたら警告文を表示
	if( CurrentStatus == KCONFIG_SCR_GAMESETTING ) {
		CheckCheat();
	}

	if( (CurrentStatus == KCONFIG_SCR_SOUND) && (Choice == 1) ) {
		FOwner->MusicPlayer->SetVolume(SettingList[1].Current);
	}

	if( CreateSE ) FOwner->SoundEffect->WavPlay(WASelect,0);
}

void TConfig2::PrevStatus()
{
	switch( CurrentStatus ){
	case KCONFIG_SCR_CONTROL:
		{
			if( Choice >= 4 ) {
				IncorporateSettings();
				ClearToLeft();
				SetNextStatus(KCONFIG_SCR_TOP);
			}
		}break;
	case KCONFIG_SCR_GAMESETTING:
	case KCONFIG_SCR_GRAPHIC:
	case KCONFIG_SCR_SOUND:
	case KCONFIG_SCR_NETWORK:
		{
			IncorporateSettings();
			ClearToLeft();
			SetNextStatus(KCONFIG_SCR_TOP);
		}break;
	case KCONFIG_SCR_TOP:
		{
			Endprocedure();
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WADeside,1);
		}break;
	}
}

void TConfig2::OnDecided()
{
	switch( CurrentStatus ){
	case KCONFIG_SCR_GAMESETTING:
		{
			if( Choice == 5 ) {
				PrevStatus();
				if( CreateSE ) FOwner->SoundEffect->WavPlay(WADeside,1);
			}
		}break;
	case KCONFIG_SCR_CONTROL:
		{
			if( Choice == 5 ) {
				PrevStatus();
				if( CreateSE ) FOwner->SoundEffect->WavPlay(WADeside,1);
			}
		}break;
	case KCONFIG_SCR_GRAPHIC:
		{
			if( Choice == 6 ) {
				PrevStatus();
				if( CreateSE ) FOwner->SoundEffect->WavPlay(WADeside,1);
			}
		}break;
	case KCONFIG_SCR_SOUND:
		{
			if( Choice == 2 ) {
				PrevStatus();
				if( CreateSE ) FOwner->SoundEffect->WavPlay(WADeside,1);
			}
		}break;
	case KCONFIG_SCR_NETWORK:
		{
			if( Choice == 1 ) {
				PrevStatus();
				if( CreateSE ) FOwner->SoundEffect->WavPlay(WADeside,1);
			}
		}break;
	case KCONFIG_SCR_TOP:
		{
			ClearToRight();
			switch( Choice ){
			case 0:SetNextStatus(KCONFIG_SCR_CONTROL);	break;
			case 1:SetNextStatus(KCONFIG_SCR_GRAPHIC);	break;
			case 2:SetNextStatus(KCONFIG_SCR_SOUND);	break;
			case 3:SetNextStatus(KCONFIG_SCR_NETWORK);	break;
			case 4:Endprocedure();	break;
			}
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WADeside,1);
		}break;
	}
}

void TConfig2::IncorporateSettings()
{
	switch( CurrentStatus ){
	case KCONFIG_SCR_GAMESETTING:
		{
			ShieldInfinite = IntToBool(SettingList[0].Current);
			MShipInfinite = IntToBool(SettingList[1].Current);
			CollisionON = IntToBool(SettingList[2].Current);
			TimeInfinite = IntToBool(SettingList[3].Current);
			SlowON = IntToBool(SettingList[4].Current);
		}break;
	case KCONFIG_SCR_CONTROL:
		{
			TempKey[0] = SettingList[0].Current;
			TempKey[1] = SettingList[1].Current;
			TempKey[2] = SettingList[2].Current;
			TempKey[3] = SettingList[3].Current;
			BoostMode  =  SettingList[4].Current;

			for( u32 i = 0 ; i <= 3; i++ )
				AssinedKey[i] = TempKey[i];
		}break;
	case KCONFIG_SCR_GRAPHIC:
		{
			Form1.ChangeFullScreenMode(SettingList[4].Current,IntToBool(SettingList[2].Current), SettingList[1].Current);
			EffLevel = SettingList[0].Current;
			TexCol = SettingList[1].Current;
			ScreenMode = IntToBool(SettingList[2].Current);
			FilterON = IntToBool(SettingList[3].Current);
			ScreenResolution = SettingList[4].Current;

			tmpExEffects = IntToBool(SettingList[5].Current);
		}break;
	case KCONFIG_SCR_SOUND:
		{
			SEVol = SettingList[0].Current;
			bgmVol = SettingList[1].Current;
		}break;
	case KCONFIG_SCR_NETWORK:
		{
			ConnectNet = IntToBool(SettingList[0].Current);
		}break;
	}
}

void TConfig2::KeyConfig()
{
	FOwner->Joy.Scan(PJoy1);

	for( u32 i = 0 ; i <= 15; i++ )
		if( (! FOwner->Joy.getbutton(i)) && (! ButtonON[i]) ) ButtonFlg_KeyConfig[i] = false;

	for( u32 k = 0 ; k <= 15; k++ ){
		if( (! ButtonFlg_KeyConfig[k]) && FOwner->Joy.getbutton(k) ) {
			ButtonFlg_KeyConfig[k]  =  true;
			for( u32 i = 0 ; i <= 3; i++ ) {
				if( (Choice == i) ) {
					TempKey[i] = k;
					UpdateSetting(0, 0, i, k, 0);
				}
			}
			break;
		}
	}

	FOwner->Joy.Scan(PKey + PJoy1);
}

void TConfig2::MakeSentence( const string& _caption, s32 _x, s32 _y, s32 _delay )
{
	TMojiAndGlassCursor* tmpObj  = new TMojiAndGlassCursor(FOwner,OrgX+_x,OrgY+_y,48,GLASSCURSOR_PATTERN_NONE);
	FOwner->Add(tmpObj);
	CursorList.push_back(tmpObj);
	tmpObj->AddSentence(_caption, 0,0,255,255,255,KFONTKIND_BLIGHT);
	tmpObj->SetSpeed(5, 5);
	tmpObj->SetDest(_x,_y,_delay*5);
	tmpObj->SetAlpha(0);
	tmpObj->SetDestAlpha(255,15,_delay*7);
}

void TConfig2::ArrowControl()
{
	if( SettingList[Choice].Horizontal ) {
		Arrow_Left->Show();
		Arrow_Right->Show();
		Arrow_Left->SetPlace(304,SettingList[Choice].Owner->Y+16);
		Arrow_Right->SetPlace(576,SettingList[Choice].Owner->Y+16);
	}
	else {
		Arrow_Left->Hide();
		Arrow_Right->Hide();
	}
}

void TConfig2::CheckCheat()
{
	if( ((SettingList[0].Current + SettingList[1].Current + SettingList[3].Current + SettingList[4].Current)  !=  0) || (SettingList[2].Current  !=  1) ) {
		if( Caution->GetSize() == 0 ) {
			Caution->MakeCursor("Caution : Your score && replay data",0,384,-6,640,0,KFONTKIND_ORANGE, GLASSCURSOR_PATTERN_BLACK_CENTER, KCURSOR_POS_CENTER);
			Caution->MakeCursor("aren't to be saved under this setting.",0,408,-6,640,0,KFONTKIND_ORANGE, GLASSCURSOR_PATTERN_BLACK_CENTER, KCURSOR_POS_CENTER);
		}
	}
	else {
		Caution->ClearCursors(0,0);
	}
}

TConfig2::TConfig2( TOBJList* owner ) : TOBJ(owner)
{
	FObjectName = "TConfig2";

	FKind = kGauge;

	tmpExEffects = ExtraEffects;

	Choice = 0;
	for( u32 i = 0 ; i <= 3; i++ )
		TempKey[i] = AssinedKey[i];

	CurrentItems = 0;
	CurrentStatus = 0;

	Caution = new TCursorList(FOwner);

	StickFlgX = true;
	StickFlgY = true;

	for( u32 i = 0 ; i <= 9; i++ ) {
		ActFlg[i] = 0;
		ActCount[i] = 0;
	}

	OrgX = 0;
	OrgY = 0;

	DecidedTime = 0;

	Arrow_Left  = new TArrow(FOwner,0,0,2);
	Arrow_Left->Hide();
	FOwner->Add(Arrow_Left);
	Arrow_Right  = new TArrow(FOwner,0,0,0);
	Arrow_Right->Hide();
	FOwner->Add(Arrow_Right);

	SetNextStatus(KCONFIG_SCR_TOP);

	GlassCubes = NULL;
	if( ExtraEffects ) {
		GlassCubes = new TGlassCubeFilter(FOwner,1);
		FOwner->Add(GlassCubes);
	}
}

TConfig2::~TConfig2()
{
	if(CursorList.size() > 0 ) {
		for( u32 i = 0 ; i < CursorList.size(); i++ ) {
			CursorList[i]->Die();
		}
	}

	CursorList.clear();
	SettingList.clear();

	if( Cursor  !=  NULL ) {
		Cursor->Die();
		Cursor  =  NULL;
	}

	Arrow_Left->Die();
	Arrow_Right->Die();

	FOwner->AfterLoopEvent = MTitle;

	SAFE_DELETE(Caution);
}

void TConfig2::Move()
{
	TOBJ::Move();

	if( DecidedTime == 0 ) Input();

	//タイトル表示
	FOwner->Add( new TBox(FOwner,0,0,640,32,0,200,255));
	FOwner->SentenceCenter("config",320,0,32,3);
	FOwner->Add( new TBox(FOwner,0,448,640,480,0,200,255));

	if( (DecidedTime > 0) && (Age - DecidedTime > 60) ) Die();
}

void TConfig2::Die()
{
	TOBJ::Die();

	//extraeffectsに変更があればここで反映
	if( ExtraEffects != tmpExEffects )
		FOwner->GetSprMan()->SetExtraEffect(tmpExEffects, tmpExEffects);

	ExtraEffects  =  tmpExEffects;
	Blooming = ExtraEffects;
}

void TPause2::Input()
{
	for( u32 i = 0 ; i <= 7; i++ )
		if( ! ButtonON[i] ) ButtonFlg[i] = false;

	if( (! ButtonFlg[0]) && ButtonON[0] ) {
		ButtonFlg[0] = true;
		OnDecided();
	}

	if( (! ButtonFlg[3]) && ButtonON[3] ) {    //プレイ画面に戻る
		Die();
		Blur->Resume();
		ButtonFlg[3] = true;
	}

	if( FOwner->Joy.StickY() == 0 ) StickFlg = false;

	if( FOwner->Joy.StickY() == 1 ) {
		KeyDownCount++;
		if( (StickFlg == false) || (KeyDownCount > 30) ) {    //新規に押下するか、60フレーム押し続ければ処理を有効
			if( KeyDownCount > 30 ) KeyDownCount  =  25;  //キー押し続けの場合、カーソル移動が早くなりすぎるのを防ぐ
			if( Cursor != NULL ) Cursor->NextPoint(1);
			Choice++;
			if( (Choice>=CurrentItems) ) {
				Choice = 0;
			}
			StickFlg = true;
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WASelect,0);
		}
	}
	else {
		KeyDownCount  =  0;
	}

	if( (FOwner->Joy.StickY() == -1) ) {
		KeyUpCount++;
		if( (StickFlg == false) || (KeyUpCount > 30)  ) {
			if( (KeyUpCount > 30) ) KeyUpCount  =  25;
			Choice--;
			if( (Choice<0) ) {
				Choice = CurrentItems - 1;
			}
			if( Cursor != NULL ) Cursor->NextPoint(-1);
			StickFlg = true;
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WASelect,0);
		}
	}
	else {
		KeyUpCount  =  0;
	}
}

void TPause2::OnDecided()
{
	switch( Choice ){
	case 0:
		{                               //return
			Die();
			Blur->Resume();
		}break;
	case 1:
		{                             //restart
			Die();
			if( ! FOwner->ReplayON ) {
				FOwner->Replayer->Init();
				FOwner->Replayer->InitSplitData();
				FOwner->ReplayON = false;
			}

			if( FOwner->IsSequence() && (! FOwner->IsPractice) ) {
				FOwner->AfterLoopEvent = MRestart_Seq;
				if( CreateBGM ) FOwner->MusicPlayer->Stop();
			}
			else
				FOwner->AfterLoopEvent = MRestart;
		}break;
	case 2:
		{                             //title
			Die();
			if( FOwner->ReplayON )
				FOwner->AfterLoopEvent = MRepView;
			else
				FOwner->AfterLoopEvent = MTitle;
			if( CreateBGM ) 
				FOwner->MusicPlayer->Stop();
		}break;
	}

	if( CreateSE ) FOwner->SoundEffect->WavPlay(WADeside,1);
}

void TPause2::MakeCursor( const string& _caption, s32 _x, s32 _y, s32 _num )
{
	TMojiAndGlassCursor* tmpObj = new TMojiAndGlassCursor(FOwner,OrgX+_x,OrgY+_y,336,GLASSCURSOR_PATTERN_GLASS);
	FOwner->Add(tmpObj);
	CursorList.push_back(tmpObj);

	s32 textXPos = 320 - ((_caption.size()*16 + (_caption.size()-1)*16) / 2);
	tmpObj->AddSentence(_caption, textXPos-_x, 16, 255, 255, 255,KFONTKIND_NORMAL);

	Cursor->SetMovePoint(_x,_y);

	tmpObj->SetSpeed(5, 5);
	tmpObj->SetDest(_x,_y,_num*5);
	tmpObj->SetAlpha(0);
	tmpObj->SetDestAlpha(255,10,_num*7);
	CurrentItems++;
}

TPause2::TPause2( TOBJList* owner ) : TOBJ(owner)
{
	static const s32 TPOS_PAUSE_TOP = 160;
	FObjectName = "TPause2";
	FKind = kGauge;
	Choice = 0;
	StickFlg = true;
	for( u32 i = 0 ; i <= 9; i++ ) {
		ActFlg[i] = 0;
		ActCount[i] = 0;
	}
	Blur  = new TBlur(FOwner);
	FOwner->Add(Blur);

	Choice = 0;

	CurrentItems = 0;

	OrgX = -640;
	OrgY = 0;
	//カーソル生成
	Cursor  = new TGlassCursor(FOwner,136, TPOS_PAUSE_TOP, 336);
	FOwner->Add(Cursor);

	MakeCursor("continue", 136, TPOS_PAUSE_TOP + YSPACE * 0, 0);
	MakeCursor("restart", 136, TPOS_PAUSE_TOP + YSPACE * 1, 1);
	if( FOwner->ReplayON ) MakeCursor("return", 136, TPOS_PAUSE_TOP + YSPACE * 2, 2);
	else MakeCursor("title", 136, TPOS_PAUSE_TOP + YSPACE * 2, 2);
}

TPause2::~TPause2()
{
	if(CursorList.size() > 0) {
		for( u32 i = 0 ; i < CursorList.size(); i++ ) {
			TMojiAndGlassCursor* tmpObj  =  CursorList[i];
			tmpObj->SetDest(540,tmpObj->Y,i*5);
			tmpObj->SetDestAlpha(0,10,i*5);
			tmpObj->SetDeathPoint(540,tmpObj->Y);
		}
	}

	CursorList.clear();

	if( Cursor  !=  NULL ) {
		Cursor->Die();
		Cursor  =  NULL;
	}
}

void TPause2::Move()
{
	TOBJ::Move();

	//タイトル表示
	FOwner->Add( new TBox(FOwner,0,0,640,31,0,200,255));
	FOwner->SentenceCenter("pause",320,0,32,3);
	FOwner->Add( new TBox(FOwner,0,448,640,480,0,200,255));
	FOwner->SentenceCenter("cloudphobia",320,448,32,3);

	if( (Application.Active) && (Age >= 5) ) Input();
}

void TPause2::Die()
{
	TOBJ::Die();
}

bool TWeaponSelect::IsSkipRequied()
{
	bool result  =  false;
	if( (FOwner->IsSequence()) && (CurrentStage > 0) && (CurrentStage < 5) && (! FOwner->IsBossDefeated_Seq(CurrentStage)) )
		result = true;

	if( TrialMode ) {
		if( (CurrentStage > 1) && (CurrentStage < 5) ) result  =  true;
	}

	return result;
}


void TWeaponSelect::Input()
{
	for( u32 i = 0 ; i <= 7; i++ )
		if( ! ButtonON[i] ) ButtonFlg[i] = false;

	if( (! ButtonFlg[0]) && ButtonON[0] ) {
		ButtonFlg[0] = true;
		//未クリア面は選択不可
		if( (FOwner->IsSequence()) && (CurrentStage > 0) && (CurrentStage < 5) && (! FOwner->IsBossDefeated_Seq(CurrentStage)) ) return;

		if( (TrialMode) && (CurrentStage > 1) && (CurrentStage < 5) ) return;

		OnDecided();
	}

	if( (! ButtonFlg[1]) && ButtonON[1] ) {
		ButtonFlg[1] = true;
		FOwner->AfterLoopEvent = MWeaponSelect;
		Die();
	}

	if( FOwner->Joy.StickX() == 0 ) StickFlgX = false;
	if( FOwner->Joy.StickY() == 0 ) StickFlgY = false;

	if( (FOwner->Joy.StickX() == 1) ) {
		KeyDownCount++;
		if( (StickFlgX == false) || (KeyDownCount > 30) ) {    //新規に押下するか、60フレーム押し続ければ処理を有効
			if( (KeyDownCount > 30) ) KeyDownCount  =  25;  //キー押し続けの場合、カーソル移動が早くなりすぎるのを防ぐ
			Choice++;
			if( (Choice >= CurrentItems) ) {
				Choice = 0;
			}
			StickFlgX = true;
			UpdateBustShot();
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WASelect,0);
		}
	}
	else {
		KeyDownCount  =  0;
	}

	if( (FOwner->Joy.StickX() == -1) ) {
		KeyUpCount++;
		if( (StickFlgX == false) || (KeyUpCount > 30)  ) {
			if( (KeyUpCount > 30) ) KeyUpCount  =  25;
			Choice--;
			if( (Choice<0) ) {
				Choice = CurrentItems - 1;
			}
			StickFlgX = true;
			UpdateBustShot();
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WASelect,0);
		}
	}
	else {
		KeyUpCount  =  0;
	}

	if( (FOwner->Joy.StickY() == 1) ) {
		KeyDownCount++;
		if( (StickFlgY == false) || (KeyDownCount > 30) ) {    //新規に押下するか、60フレーム押し続ければ処理を有効
			if( (KeyDownCount > 30) ) KeyDownCount  =  25;  //キー押し続けの場合、カーソル移動が早くなりすぎるのを防ぐ
			CurrentStage++;

			if( CurrentStage>maxstage ) CurrentStage = 0;
			StSel->Up();

			//未クリア面はスキップ
			while( IsSkipRequied() ) {
				CurrentStage++;

				if( CurrentStage>maxstage ) CurrentStage = 0;
				StSel->Up();
			}

			((TStageSelectScreenMng*)FOwner->StageSelect)->ChangeStage(CurrentStage);
			BustShot_Rifle->ChangeColor(CurrentStage);
			BustShot_Blade->ChangeColor(CurrentStage);
			StickFlgY = true;
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WASelect,0);
		}
	}
	else {
		KeyDownCount  =  0;
	}

	if( (FOwner->Joy.StickY() == -1) ) {
		KeyUpCount++;
		if( (StickFlgY == false) || (KeyUpCount > 30) ) {    //新規に押下するか、60フレーム押し続ければ処理を有効
			if( (KeyUpCount > 30) ) KeyUpCount  =  25;  //キー押し続けの場合、カーソル移動が早くなりすぎるのを防ぐ

			CurrentStage--;
			if( CurrentStage<0 ) CurrentStage = maxstage;
			StSel->Down();

			//未クリア面はスキップ
			while( IsSkipRequied() ) {
				CurrentStage--;
				if( CurrentStage<0 ) CurrentStage = maxstage;

				StSel->Down();
			}

			((TStageSelectScreenMng*)FOwner->StageSelect)->ChangeStage(CurrentStage);
			BustShot_Rifle->ChangeColor(CurrentStage);
			BustShot_Blade->ChangeColor(CurrentStage);
			StickFlgY = true;
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WASelect,0);
		}
	}
	else {
		KeyUpCount  =  0;
	}
}

void TWeaponSelect::MakeCursor( const string& _caption, s32 _x, s32 _y, s32 _num )
{
	Sentence[_num]  = new TSprSentence(FOwner,_caption,OrgX+_x,OrgY+_y,-4,KFONTKIND_ORANGE);
	FOwner->Add(Sentence[_num]);

	Sprite[0]->Add(Sentence[_num]->Sprite[0]);
}

void TWeaponSelect::OnDecided()
{
	DecidedTime  =  Age;
	FOwner->HeroWeapon  =  Choice;
	FOwner->Add( new TBoxContrl(FOwner,5));
	if(Choice == 0 ) {
		BustShot_Rifle->Sprite[0]->Move(160,0);
		BustShot_Blade->Sprite[0]->Move(640,0);
		Sprite[0]->Move(256,56);
		Sprite[1]->Move(640,56);
		FOwner->Add( new TEyeLight(FOwner,450,162));
	}
	else {
		BustShot_Rifle->Sprite[0]->Move(640,0);
		BustShot_Blade->Sprite[0]->Move(160,0);
		Sprite[0]->Move(640,56);
		Sprite[1]->Move(256,56);
		FOwner->Add( new TEyeLight(FOwner,374,162));
	}
	if( CreateSE ) FOwner->SoundEffect->WavPlay(WABoot,0);
	//  if( CreateSE ) FOwner->SoundEffect->WavPlay(WADeside,1);

	if( FOwner->IsSequence() && (CurrentStage<5) )
		FOwner->IsPractice  =  true;
	else
		FOwner->IsPractice  =  false;
}

void TWeaponSelect::UpdateBustShot()
{
	if(Choice == 0 ) {
		BustShot_Rifle->Show();
		BustShot_Blade->Hide();

		Sprite[0]->SetDest(256,56,5);
		Sprite[1]->SetDest(640,56,5);
	}
	else {
		BustShot_Rifle->Hide();
		BustShot_Blade->Show();

		Sprite[0]->SetDest(640,56,5);
		Sprite[1]->SetDest(256,56,5);
	}

	if( FOwner->IsSequence() )  
		StSel->ShowStar(FOwner->IsCleared[FOwner->Difficulty][Choice]);
}

void TWeaponSelect::Slide()
{
	Sprite[0]->AutoSlide();
	Sprite[1]->AutoSlide();
}

TWeaponSelect::TWeaponSelect( TOBJList* owner ) : TOBJ(owner)
{
	FObjectName = "TWeaponSelect";
	FKind = kGauge;
	Choice = 0;
	FOwner->GameMode = gTitle;
	StickFlgX = true;
	StickFlgY = true;
	for( u32 i = 0 ; i <= 9; i++ ) {
		ActFlg[i] = 0;
		ActCount[i] = 0;
	}

	Choice = 0;
	DecidedTime = -1;
	CurrentItems = 2;

	OrgX = 0;
	OrgY = 0;

	switch( FOwner->Difficulty ){
	case KDIFFICULTY_SINGLE:
		{
			CurrentStage = 0;
			maxstage = 4;
		}break;
	default:
		{
			CurrentStage = 5;
			maxstage = 5;
		}
	}

	BustShot_Rifle  = new TBustShot(FOwner,640,0,160,0,0,false);
	BustShot_Blade  = new TBustShot(FOwner,640,0,160,0,1,false);
	FOwner->Add(BustShot_Rifle);
	FOwner->Add(BustShot_Blade);

	MakeSpriteLite(FOwner->GetDG(), 0,640,56,384,80,200,bmNormal);     //ライフルの名前
	Sprite[0]->GetSpr()->AddTexture(BShotTex1, 128, 1, 511, 79);
	Sprite[0]->GetSpr()->FSmooth  =  false;
	MakeSpriteLite(FOwner->GetDG(),1,640,56,384,80,200,bmNormal);     //刀の名前
	Sprite[1]->GetSpr()->AddTexture(BShotTex2, 128, 1, 511, 79);
	Sprite[1]->GetSpr()->FSmooth  =  false;

	StSel =new TStSelCursor(FOwner);
	FOwner->Add(StSel);

	UpdateBustShot();

	ButtonFlg[0] = true;
}

void TWeaponSelect::Move()
{
	TOBJ::Move();

	//タイトル表示
	FOwner->Add( new TBox(FOwner,0,0,640,31,0,200,255));
	FOwner->SentenceCenter("select your weapon",320,0,4,3);
	FOwner->Add( new TBox(FOwner,0,448,640,480,0,200,255));

	if( DecidedTime<0 ) {
		Input();
		Slide();
	}

	if( (DecidedTime > 0) && ((Age-DecidedTime)>120) ) {
		FOwner->AfterLoopEvent = MLoadBeforePlay;
		Die();
	}
}

void TWeaponSelect::Die()
{
	BustShot_Rifle->Die();
	BustShot_Blade->Die();

	StSel->Die();

	TOBJ::Die();
}

void TResult2::MakeItem( const string& _caption, const string& _mul, s32 _score, s32 _num )
{
	static const s32 Pos_Top = 64;
	static const s32 Pos_YBlank = 26;
	static const s32 Pos_YBlank_half = 24;
	static const s32 Cursor_Width = 424;
	static const s32 Pos_Left = 0;
	static const s32 Pos_Left_Point = 16;

	CursorList->MakeCursor(_caption, Pos_Left, Pos_Top + Pos_YBlank*_num*2, -4, Cursor_Width, _num, KFONTKIND_ORANGE, GLASSCURSOR_PATTERN_NONE, KCURSOR_POS_LEFT);
	CursorList->MakeCursor(_mul, Pos_Left, Pos_Top + Pos_YBlank*_num*2, -4, Cursor_Width, _num, KFONTKIND_ORANGE, GLASSCURSOR_PATTERN_BLACK_LEFT, KCURSOR_POS_RIGHT);
	CursorList->MakeCursor(InttoStr(_score), Pos_Left_Point, Pos_Top + Pos_YBlank*_num*2+Pos_YBlank_half, 2, Cursor_Width, _num, KFONTKIND_BLIGHT, GLASSCURSOR_PATTERN_NONE, KCURSOR_POS_LEFT);

}

TResult2::TResult2( TOBJList* owner ) : TOBJ(owner)
{
	FObjectName = "TResult2";
	//  FOwner->TimeStop = true;
	FOwner->GameMode = gOther;
	FOwner->ResultExists = true;
	FKind = kGauge;
	//  Blur  = new TBlur(FOwner);
	//  FOwner->Add(Blur);

	CursorList = new TCursorList(FOwner);

	//ボーナス計算
	destbonus = 200000*FOwner->BossRate;

	timebonus = (FOwner->LimitTime / 10)*300;
	lifebonus = RoundOff((f32)FOwner->HeroHP / (f32)HPMax*1000000.f);
	if( FOwner->HeroHP == HPMax ) ndbonus = 1000000;
	else ndbonus = 0;
	hitbonus = FOwner->TotalHit*5000;
	total = destbonus+timebonus+lifebonus+ndbonus+hitbonus;
	FOwner->TotalScore += total;

	if( (FOwner->TotalScore>ScoreRanking_local[FOwner->NowStage][0].Score) && (! FOwner->ReplayON) && (! FOwner->CheatON) && (! FOwner->IsPractice) ) {
		FOwner->HiScore = FOwner->TotalScore;
	}

	for( u32 i = 0 ; i <= 7; i++ )
		ButtonFlg[i] = true;

	LifeRate = RoundOff((f32)FOwner->HeroHP / (f32)HPMax * 1000.f);

	//累積タイムを加算
	FOwner->SequenceTotalTime += FOwner->LimitTime;

	//ランキング用ヘッダにクリアボーナス情報を登録
	FOwner->RankinHeaderData.OriginalScore = FOwner->TotalScore - total;
	FOwner->RankinHeaderData.DestBonusRate = FOwner->BossRate;
	FOwner->RankinHeaderData.Life = lifebonus;
	FOwner->RankinHeaderData.NoDamageBonus = ndbonus;
	FOwner->RankinHeaderData.TotalHit = FOwner->TotalHit;
	FOwner->RankinHeaderData.ClearBonus = total;

	FOwner->Replayer->SplitData.SplitScore[FOwner->NowStage]  =  FOwner->TotalScore;
	FOwner->Replayer->SplitData.SplitTime[FOwner->NowStage]  =  FOwner->LimitTime;

	//画面作成
	CursorList->SetAppearPoint(-640,0);
	CursorList->MakeCursor("result", 176, 12, 16, 240, 0, KFONTKIND_NORMAL, GLASSCURSOR_PATTERN_GLASS, KCURSOR_POS_CENTER);

	MakeItem("destruction bonus", "x"+InttoStr(FOwner->BossRate), destbonus,0);

	MakeItem("time bonus", TimeToStr(FOwner->LimitTime) + "sec", timebonus,1);

	MakeItem("shield bonus", "x"+TimeToStr(LifeRate), lifebonus,2);

	MakeItem("no damage bonus", "", ndbonus,3);

	MakeItem("hit bonus", InttoStr(FOwner->TotalHit) + "hits", hitbonus,4);

	MakeItem("total", "", total,5);

	CursorList->SetAppearPoint(640,0);    //右から出す
	//  MakeCursor("total", 320, 324, -4, 320, 5, KFONTKIND_ORANGE, GLASSCURSOR_PATTERN_BLACK_RIGHT, KCURSOR_POS_RIGHT);
	//  MakeCursor(InttoStr(total), 0, 348, 8, 640, 5, KFONTKIND_BLIGHT, GLASSCURSOR_PATTERN_NONE, KCURSOR_POS_RIGHT);

	CursorList->MakeCursor("your score", 120, 380, -4, 520, 7, KFONTKIND_ORANGE, GLASSCURSOR_PATTERN_BLACK_RIGHT, KCURSOR_POS_RIGHT);
	CursorList->MakeCursor(InttoStr(FOwner->TotalScore), 48, 406, 8, 640, 7, KFONTKIND_GOTHIC, GLASSCURSOR_PATTERN_NONE, KCURSOR_POS_RIGHT);

	//最終面ならクリアー実績を記録
	if( (FOwner->IsSequence()) && (FOwner->NowStage == 4) && (! FOwner->ReplayON) && (! FOwner->CheatON) && (! FOwner->IsPractice) )
		FOwner->IsCleared[FOwner->Difficulty][FOwner->HeroWeapon]  =  true;
}

TResult2::~TResult2()
{
	SAFE_DELETE(CursorList);
}

void TResult2::Move()
{
	TOBJ::Move();
	if( Age>30 ) Input();

	if( (Age == 5) && CreateBGM ) FOwner->MusicPlayer->Play(2);
}

void TResult2::Die()
{
	TOBJ::Die();
}

void TResult2::Input()
{
	for( u32 i = 0 ; i <= 7; i++ )
		if( ! ButtonON[i] ) ButtonFlg[i] = false;

	for( u32 i = 0 ; i <= 7; i++ ){
		if( (! ButtonFlg[i]) && ButtonON[i] ) {
			ButtonFlg[i] = true;
			Die();
			if( (! FOwner->ReplayON) && (! FOwner->CheatON) && (! FOwner->IsPractice) ) {
				if( (FOwner->Difficulty < KDIFFICULTY_SINGLE) && (FOwner->NowStage < 4) ) {
					FOwner->SetRankInHeader();
					//このステージのリプレイデータを一時的に保存
					FOwner->Replayer->SaveTempReplay(FOwner->NowStage, FOwner->RankinHeaderData);
					if( TrialMode && (FOwner->NowStage == 1) )     //体験版は2面で終了させるため、ここでスコア登録
						FOwner->Add( new TNetRanking(FOwner));
					else
						FOwner->Add( new TRecover(FOwner));//回復画面へ
					//          FOwner->StageEnd;  //通し面モードならすぐ次のステージへ（最終面の場合はランキングへ）
				}
				else {
					FOwner->Add( new TNetRanking(FOwner));
				}
			}
			else {
				if( (FOwner->Difficulty < KDIFFICULTY_SINGLE) && (FOwner->NowStage < 4) && (! FOwner->IsPractice) )
					FOwner->Add( new TRecover(FOwner)); //回復画面へ
				else
					FOwner->StageEnd();
			}
			break;
		}
	}
}

void TConnectAsk::OnDecided()
{
	switch( Cursor->GetCurrentPos() ){
	case 1:
		{
			ConnectNet = true;
		}break;
	case 0:
		{
			ConnectNet = false;
		}  
	}

	//ネームエントリー画面へ
	FOwner->Add( new TNameEntry2(FOwner,true));
	if( CreateSE ) FOwner->SoundEffect->WavPlay(WADeside,1);

	Die();
}

void TConnectAsk::Input()
{
	for( u32 i = 0 ; i <= 7; i++ )
		if( ! ButtonON[i] ) ButtonFlg[i] = false;

	if( FOwner->Joy.StickY() == 0 ) StickFlg = false;

	if( (! ButtonFlg[0]) && ButtonON[0] ) {
		ButtonFlg[0] = true;
		OnDecided();
	}

	if( (! ButtonFlg[1]) && ButtonON[1] ) {
		ButtonFlg[1] = true;
		FOwner->AfterLoopEvent = MTitle;
		Die();
	}

	if( (StickFlg == false) && (FOwner->Joy.StickY() == 1) ) {
		if( Cursor  !=  NULL ) Cursor->NextPoint(1);
		StickFlg = true;
		if( CreateSE ) FOwner->SoundEffect->WavPlay(WASelect,0);
	}

	if( (StickFlg == false) && (FOwner->Joy.StickY() == -1) ) {
		if( Cursor  !=  NULL ) Cursor->NextPoint(-1);
		StickFlg = true;
		if( CreateSE ) FOwner->SoundEffect->WavPlay(WASelect,0);
	}
}

TConnectAsk::TConnectAsk( TOBJList* owner ) : TOBJ(owner)
{
	FObjectName = "TConnectAsk";
	FOwner->GameMode = gTitle;
	FKind = kGauge;
	StickFlg = false;

	CursorList = new TCursorList(FOwner);

	//カーソル生成
	Cursor  = new TGlassCursor(FOwner,224, 256, 160);
	FOwner->Add(Cursor);

	//画面作成
	CursorList->SetAppearPoint(-640,0);

	CursorList->MakeCursor("would you connect to network ?",  0, 120, 1, 640, 0, KFONTKIND_BLIGHT, GLASSCURSOR_PATTERN_NONE, KCURSOR_POS_CENTER);
	CursorList->MakeCursor("",  32, 152, 1, 576, 1, KFONTKIND_NORMAL, GLASSCURSOR_PATTERN_LINE, KCURSOR_POS_CENTER);

	CursorList->MakeCursor("no",  224, 256, 16, 160, 3, KFONTKIND_NORMAL, GLASSCURSOR_PATTERN_GLASS, KCURSOR_POS_CENTER);
	Cursor->SetMovePoint(224,256);
	CursorList->MakeCursor("yes", 224, 192, 16, 160, 2, KFONTKIND_NORMAL, GLASSCURSOR_PATTERN_GLASS, KCURSOR_POS_CENTER);
	Cursor->SetMovePoint(224,192);
}

TConnectAsk::~TConnectAsk()
{
	SAFE_DELETE(CursorList);
	Cursor->Die();
}

void TConnectAsk::Move()
{
	TOBJ::Move();
	// 帯表示
	FOwner->Add( new TBox(FOwner,0,0,640,32,0,200,255));
	FOwner->Add( new TBox(FOwner,0,448,640,480,0,200,255));

	Input();
}

void TConnectAsk::Die()
{
	TOBJ::Die();

}

void TLocalRanking::MakeRankingItems( NetRankingData _rankdata[], s32 _totaldata )
{

	//全カーソルをクリア
	if(CursorList.size() > 0 ) {
		for( u32 i = 0 ; i < CursorList.size(); i++ ) {
			CursorList[i]->Die();
		}
	}

	MakeItemName();

	s32 maxline  =  _totaldata;

	if( _totaldata>0 ) {

		for( s32 i = 0 ; i < maxline; i++ ) {
			TMojiAndGlassCursor* tmpObj  = new TMojiAndGlassCursor(FOwner,OrgX+16,OrgY+FirstYPos_LocalRanking+i*YSPACE,576,GLASSCURSOR_PATTERN_GLASS);
			FOwner->Add(tmpObj);
			CursorList.push_back(tmpObj);

			//ランク
			tmpObj->AddSentenceRight(InttoStr(_rankdata[i].Rank), 64,0,255,255,255,KFONTKIND_NORMAL);

			//名前
			tmpObj->AddSentence(FOwner->PlayerNameToStr(_rankdata[i].Name, MAX_NAME_LENGTH), 88,0,255,255,255,KFONTKIND_NORMAL);

			switch( ActFlg[0] ){
			case 0:
				{
					//スコア
					tmpObj->AddSentence(FOwner->ScoreToStr(_rankdata[i].Score), 256,0,255,255,255,KFONTKIND_NORMAL);
				}break;
			case 1:
				{
					//タイム
					if( FOwner->IsSequence() )
						tmpObj->AddSentenceRight(TotalTimeToStr(_rankdata[i].Time), 400,0,255,255,255,KFONTKIND_NORMAL);
					else
						tmpObj->AddSentenceRight(FOwner->TimeToStr(_rankdata[i].Time), 368,0,255,255,255,KFONTKIND_NORMAL);
				}break;
			}

			//日付
			tmpObj->AddSentenceRight(FOwner->DateToStr(_rankdata[i].Year,
				_rankdata[i].Month,
				_rankdata[i].Day),
				592,0,255,255,255,KFONTKIND_NORMAL);

			tmpObj->SetSpeed(5, 5);
			tmpObj->SetDest(16,FirstYPos_LocalRanking+i*YSPACE,i*5);
			tmpObj->SetAlpha(0);
			tmpObj->SetDestAlpha(255,15,i*7);
		}
	}
}

void TLocalRanking::MakeItemName()
{
	//名前
	MakeSentence("name", 120, FirstYPos_LocalRanking-32, 0);

	switch( ActFlg[0] ){
	case 0:
	case 2:
		{
			//スコア
			MakeSentence("score", 296, FirstYPos_LocalRanking-32, 1);

		}break;
	case 1:
	case 3:
		{
			//タイム
			MakeSentence("time", 296, FirstYPos_LocalRanking-32, 2);
		}break;
	}

	//日付
	MakeSentence("date", 488, FirstYPos_LocalRanking-32, 3);

	string order;
	switch( ActFlg[0] ){
	case 0:
	case 2:
		{ //スコアランク表示
			order = "ordered by score";
		}break;
	case 1:
	case 3:
		{  //タイムランク表示
			order = "ordered by time";
		}break;
	}
	//ソート名
	s32 textXPos  =  320 - (order.size()*18 / 2) - 160;
	TMojiAndGlassCursor* tmpObj  = new TMojiAndGlassCursor(FOwner,160-640,FirstYPos_LocalRanking - 56,320,GLASSCURSOR_PATTERN_NONE);
	FOwner->Add(tmpObj);
	CursorList.push_back(tmpObj);
	tmpObj->SetSpeed(5, 5);
	tmpObj->SetDest(160,FirstYPos_LocalRanking - 56,0);
	tmpObj->SetAlpha(0);
	tmpObj->SetDestAlpha(200,15,0);
	tmpObj->AddSentence(order, textXPos,0,255,255,255,KFONTKIND_ORANGE);

	//ステージ
	string stagename;
	switch(CurrentStage){
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:stagename  =  "Episode " + InttoStr(CurrentStage+1) + " (old)";		break;
	case 5:stagename  =  "Sequence : easy";			break;
	case 6:stagename  =  "Sequence : normal";		break;
	case 7:stagename  =  "Sequence : hard";			break;
	}
	textXPos  =  320 - (stagename.size()*18 / 2) - 160;
	tmpObj  = new TMojiAndGlassCursor(FOwner,160-640,480 - 32,320,GLASSCURSOR_PATTERN_NONE);
	FOwner->Add(tmpObj);
	CursorList.push_back(tmpObj);
	tmpObj->SetSpeed(5, 5);
	tmpObj->SetDest(160,480 - 32,0);
	tmpObj->SetAlpha(0);
	tmpObj->SetDestAlpha(200,15,0);
	tmpObj->AddSentence(stagename, textXPos,0,255,255,255,KFONTKIND_ORANGE);
}

void TLocalRanking::MakeSentence( const string& _caption, s32 _x, s32 _y, s32 _delay )
{
	TMojiAndGlassCursor* tmpObj  = new TMojiAndGlassCursor(FOwner,_x-640,_y,48,GLASSCURSOR_PATTERN_NONE);
	FOwner->Add(tmpObj);
	CursorList.push_back(tmpObj);
	tmpObj->AddSentence(_caption, 0,0,255,255,255,KFONTKIND_BLIGHT);
	tmpObj->SetSpeed(5, 5);
	tmpObj->SetDest(_x,_y,_delay*5);
	tmpObj->SetAlpha(0);
	tmpObj->SetDestAlpha(255,15,_delay*7);
}

void TLocalRanking::Input()
{
	for( u32 i = 0 ; i <= 7; i++ )
		if( ! ButtonON[i] ) ButtonFlg[i] = false;

	if( FOwner->Joy.StickX() == 0 ) StickFlgX = false;
	if( FOwner->Joy.StickY() == 0 ) StickFlgY = false;

	if( (StickFlgX == false) && (FOwner->Joy.StickX() == 1) ) {
		StickFlgX = true;
		OrgX = 640;
		OrgY = 0;
		switch( ActFlg[0] ){
		case 0:{
			//タイムランキングに移行
			ChangeOrder(1);
			   }break;
		}
		if( CreateSE ) FOwner->SoundEffect->WavPlay(WASelect,0);
	}

	if( (StickFlgX == false) && (FOwner->Joy.StickX() == -1) ) {
		StickFlgX = true;
		OrgX = -640;
		OrgY = 0;
		switch( ActFlg[0] ){
		case 1:
			{
				//スコアランキングに移行
				ChangeOrder(0);
			}break;
		}
		if( CreateSE ) FOwner->SoundEffect->WavPlay(WASelect,0);
	}

	if( (StickFlgY == false) && (FOwner->Joy.StickY() == 1) ) {
		StickFlgY = true;
		OrgX = 0;
		OrgY = 480;

		CurrentStage++;
		if( CurrentStage > CurrentItems ) CurrentStage = 5;
		//    TStageSelectScreenMng(FOwner->StageSelect).ChangeStage(CurrentStage);

		ChangeOrder(ActFlg[0]);

		if( CreateSE ) FOwner->SoundEffect->WavPlay(WASelect,0);
	}

	if( (StickFlgY == false) && (FOwner->Joy.StickY() == -1) ) {
		StickFlgY = true;
		OrgX = 0;
		OrgY = 480;

		CurrentStage--;
		if( CurrentStage < 5 ) CurrentStage = CurrentItems;
		//    TStageSelectScreenMng(FOwner->StageSelect).ChangeStage(CurrentStage);

		ChangeOrder(ActFlg[0]);

		if( CreateSE ) FOwner->SoundEffect->WavPlay(WASelect,0);
	}

	for( u32 i = 0 ; i <= 7; i++ ) {
		if( (! ButtonFlg[i]) && ButtonON[i] ) {
			ButtonFlg[i] = true;
			Die();
			FOwner->AfterLoopEvent = MTitle;
		}
	}
}

void TLocalRanking::ChangeOrder( s32 _order )
{
	switch( _order ){
	case 0:
		{
			//スコアランキングに移行
			ActFlg[0]  =  0;
			if( CurrentStage < 5 )
				MakeRankingItems(ScoreRanking_local[CurrentStage], TotalScoreData_local[CurrentStage]);
			else
				MakeRankingItems(ScoreRanking_Seq_local[CurrentStage-5], TotalScoreData_local[CurrentStage]);
			Arrow_Right->Show();
			Arrow_Left->Hide();
		}break;
	case 1:
		{
			//タイムランキングに移行
			ActFlg[0]  =  1;
			if( CurrentStage < 5 )
				MakeRankingItems(TimeRanking_local[CurrentStage], TotalTimeData_local[CurrentStage]);
			else
				MakeRankingItems(TimeRanking_Seq_local[CurrentStage-5], TotalTimeData_local[CurrentStage]);
			Arrow_Right->Hide();
			Arrow_Left->Show();
		}break;
	}
}

TLocalRanking::TLocalRanking( TOBJList* owner ) : TOBJ(owner)
{
	FObjectName = "TLocalRanking";
	FKind = kGauge;
	FOwner->GameMode = gTitle;
	CurrentStage = 5;

	OrgX  =  0;
	OrgY  =  480;
	DestX  =  0;
	DestY  =  0;

	XSpeed  =  0;
	YSpeed  =  0;

	StickFlgX = true;
	StickFlgY = true;

	for( u32 i = 0 ; i <= 9; i++ ) {
		ActFlg[i] = 0;
		ActCount[i] = 0;
	}

	Arrow_Left  = new TArrow(FOwner,128,16,2);
	Arrow_Left->Hide();
	FOwner->Add(Arrow_Left);
	Arrow_Right  = new TArrow(FOwner,512,16,0);
	Arrow_Right->Show();
	FOwner->Add(Arrow_Right);


	//項目数をカウントする
	for( u32 k = 0; k <= 4; k++ ) {
		TotalScoreData_local[k] = 0;
		TotalTimeData_local[k] = 0;
		for( u32 i = 0; i <= 9; i++ ) {
			if( ScoreRanking_local[k][i].Score != 0 ) {
				TotalScoreData_local[k]++;
			}
			else break;
		}

		for( u32 i = 0; i <= 9; i++ ) {
			if( TimeRanking_local[k][i].Time != 0 ) {
				TotalTimeData_local[k]++;
			}
			else break;
		}
	}

	for( u32 k  =  5 ; k <= 7; k++ ) {
		TotalScoreData_local[k] = 0;
		TotalTimeData_local[k] = 0;
		for( u32 i = 0; i <= 9; i++ ) {
			if( ScoreRanking_Seq_local[k-5][i].Score != 0 ) {
				TotalScoreData_local[k]++;
			}
			else break;
		}

		for( u32 i = 0; i <= 9; i++ ) {
			if( TimeRanking_Seq_local[k-5][i].Time != 0 ) {
				TotalTimeData_local[k]++;
			}
			else break;
		}
	}

	if( (! FOwner->IsNormalCleared) )
		CurrentItems = 6;    //ノーマル未クリアならHardは表示しない
	else
		CurrentItems = 7;

	ChangeOrder(0);
}

TLocalRanking::~TLocalRanking()
{
	for( u32 i = 0 ; i < CursorList.size(); i++ ) {
		CursorList[i]->Die();
	}

	CursorList.clear();

	Arrow_Left->Die();
	Arrow_Right->Die();
}

void TLocalRanking::Move()
{
	TOBJ::Move();
	Input();
}

void TLocalRanking::Die()
{
	TOBJ::Die();
}

void TNameEntry2::Input()
{
	for( u32 i = 0 ; i <= 7; i++ )
		if( ! ButtonON[i] ) ButtonFlg[i] = false;

	if( (! ButtonFlg[0]) && ButtonON[0] ) {
		ButtonFlg[0] = true;
		OnDecided();
	}

	if( (! ButtonFlg[1]) && ButtonON[1] ) {
		ButtonFlg[1] = true;
		Back();
	}

	if( FOwner->Joy.StickX() == 0 ) StickFlgX = false;
	if( FOwner->Joy.StickY() == 0 ) StickFlgY = false;

	if( (FOwner->Joy.StickY() == 1) ) {
		KeyDownCount++;
		if( (StickFlgY == false) || (KeyDownCount > 30) ) {    //新規に押下するか、60フレーム押し続ければ処理を有効
			if( (KeyDownCount > 30) ) KeyDownCount  =  25;  //キー押し続けの場合、カーソル移動が早くなりすぎるのを防ぐ
			if( Cursor != NULL ) Cursor->NextPoint(10);
			StickFlgY = true;
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WASelect,0);
		}
	}
	else {
		KeyDownCount  =  0;
	}

	if( (FOwner->Joy.StickY() == -1) ) {
		KeyUpCount++;
		if( (StickFlgY == false) || (KeyUpCount > 30)  ) {
			if( (KeyUpCount > 30) ) KeyUpCount  =  25;
			if( Cursor != NULL ) Cursor->NextPoint(-10);
			StickFlgY = true;
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WASelect,0);
		}
	}
	else {
		KeyUpCount  =  0;
	}


	if( (FOwner->Joy.StickX() == 1) ) {
		KeyRightCount++;
		if( (StickFlgX == false) || (KeyRightCount > 30) ) {    //新規に押下するか、60フレーム押し続ければ処理を有効
			if( (KeyRightCount > 30) ) KeyRightCount  =  25;  //キー押し続けの場合、カーソル移動が早くなりすぎるのを防ぐ
			if( Cursor != NULL ) {
				Cursor->NextPoint(1);
				if( Cursor->GetCurrentPos() % 10 == 0 )
					Cursor->NextPoint(-10);
			}
			StickFlgX = true;
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WASelect,0);
		}
	}
	else {
		KeyRightCount  =  0;
	}

	if( (FOwner->Joy.StickX() == -1) ) {
		KeyLeftCount++;
		if( (StickFlgX == false) || (KeyLeftCount > 30)  ) {
			if( (KeyLeftCount > 30) ) KeyLeftCount  =  25;
			if( Cursor != NULL ) {
				Cursor->NextPoint(-1);
				if( (Cursor->GetCurrentPos() % 10 == 9) )
					Cursor->NextPoint(10);
			}
			StickFlgX = true;
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WASelect,0);
		}
	}
	else {
		KeyLeftCount  =  0;
	}

}

void TNameEntry2::OnDecided()
{
	if( Cursor->GetCurrentPos() <= 46 ) {
		SelectName[Cursor_Name->GetCurrentPos()]  =  Cursor->GetCurrentPos();
		Cursor_Name->NextPoint(1);
	}
	else {
		switch( Cursor->GetCurrentPos() ){
		case 47:Cursor_Name->NextPoint(1);	break;
		case 48:Back();						break;
		case 49:EndProcedure();				break;
		}
	}

	if( (Cursor->GetCurrentPos() <= 47) ) {
		if( (Cursor_Name->GetCurrentPos() == 0) )
			EndProcedure();
		else
			UpdateName(false);
	}

	if( CreateSE ) FOwner->SoundEffect->WavPlay(WADeside,1);
}

void TNameEntry2::MakeChar( s32 _moji, s32 _x, s32 _y, s32 _num )
{
	s8 c[2];
	c[0] = FONTSET[_moji];
	c[1] = '\0';
	const string str = c;
	CursorList->MakeCursor(str, _x, _y, 0, 16, _num, KFONTKIND_NORMAL, GLASSCURSOR_PATTERN_GLASS, KCURSOR_POS_LEFT);
	Cursor->SetMovePoint(_x,_y);
}

void TNameEntry2::UpdateName( bool _blink )
{
	string tempName = "";

	s32 Blink;
	if( _blink )
		Blink = KFONTKIND_BLINK2;
	else
		Blink = KFONTKIND_NORMAL;

	CursorList_Name->ClearCursors(0,0);

	//名前を文字列データに変換
	for( u32 i = 0; i <= 7; i++ )
		tempName = tempName + FONTSET[SelectName[i]];

	//名前を表示  
	CursorList_Name->MakeCursor(tempName, 208, 96, 8, 640, -1, Blink, GLASSCURSOR_PATTERN_NONE, KCURSOR_POS_LEFT);
}

void TNameEntry2::Back()
{
	SelectName[Cursor_Name->GetCurrentPos()]  =  47;
	if( Cursor_Name->GetCurrentPos() > 0 ) Cursor_Name->NextPoint(-1);
	UpdateName(false);
}

void TNameEntry2::EndProcedure()
{
	UpdateName(true);

	DecidedTime = Age;

	for( u32 i = 0; i <= 7; i++ )
		FOwner->PlayerName[i] = SelectName[i];

	Cursor_Name->Hide();

	if( ExtraEffects ) {
		GlassCubes->EndProc();
	}
}

TNameEntry2::TNameEntry2( TOBJList* owner, bool _play ) : TOBJ(owner)
{
	FObjectName = "TNameEntry2";
	FKind = kGauge;
	FOwner->GameMode = gTitle;
	PlayFlg = _play;

	StickFlgX = true;
	StickFlgY = true;
	KeyLeftCount = 0;
	KeyRightCount = 0;
	KeyUpCount = 0;
	KeyDownCount = 0;
	DecidedTime = 0;

	for( u32 i = 0; i <= 9; i++ ) {
		ActFlg[i] = 0;            //ActFlg[0]を「SelectNameのn文字目」と定義
		ActCount[i] = 0;
	}

	for( u32 i = 0; i <= 7; i++ ) {
		SelectName[i] = FOwner->PlayerName[i];
	}

	CursorList = new TCursorList(FOwner);
	CursorList_Name = new TCursorList(FOwner);

	//カーソル生成
	Cursor  = new TGlassCursor(FOwner,80, 184, 16);
	FOwner->Add(Cursor);

	Cursor_Name  = new TGlassCursor(FOwner,208, 96, 16);
	FOwner->Add(Cursor_Name);
	for( u32 i = 0; i <= 7; i++ )
		Cursor_Name->SetMovePoint(208+i*24,96);

	//名前カーソルの初期位置を決定
	s32 cursor_offset = 7;
	for( u32 i  =  7 ; i >= 0; i-- ) {
		if( SelectName[i]=47 )
			cursor_offset--;
		else
			break;
	}
	if( cursor_offset>0 ) Cursor_Name->NextPoint(cursor_offset);

	//画面作成
	CursorList->SetAppearPoint(-640,0);
	CursorList->MakeCursor("", 176, 96, 16, 256, 0, KFONTKIND_NORMAL, GLASSCURSOR_PATTERN_GLASS, KCURSOR_POS_CENTER);
	CursorList->MakeCursor("",  32, 144, 1, 576, 1, KFONTKIND_NORMAL, GLASSCURSOR_PATTERN_LINE, KCURSOR_POS_CENTER);


	CursorList->SetAppearPoint(640,0);
	for( u32 i = 0; i <= 9; i++ )
		MakeChar(i, 80+i*48,184, 2);

	CursorList->SetAppearPoint(-640,0);
	for( u32 i = 0; i <= 9; i++ )
		MakeChar(10+i, 80+i*48,232, 3);

	CursorList->SetAppearPoint(640,0);
	for( u32 i = 0; i <= 9; i++ )
		MakeChar(20+i, 80+i*48,280, 4);

	CursorList->SetAppearPoint(-640,0);
	for( u32 i = 0; i <= 9; i++ )
		MakeChar(30+i, 80+i*48,328, 5);

	CursorList->SetAppearPoint(640,0);
	for( u32 i = 0; i <= 6; i++ )
		MakeChar(40+i, 80+i*48,376, 6);

	MakeChar(48, 416,376, 6);
	MakeChar(49, 464,376, 6);
	MakeChar(50, 512,376, 6);

	UpdateName(false);
	if( ExtraEffects ) {
		GlassCubes =new TGlassCubeFilter(FOwner,0);
		FOwner->Add(GlassCubes);
	}
}

TNameEntry2::~TNameEntry2()
{
	if( PlayFlg ) {
		FOwner->Add( new TTutorial(FOwner));
	}
	else FOwner->AfterLoopEvent = MTitle;

	SAFE_DELETE(CursorList);
	SAFE_DELETE(CursorList_Name);
	Cursor->Die();
	Cursor_Name->Die();
}

void TNameEntry2::Move()
{
	TOBJ::Move();
	// 帯表示
	FOwner->SentenceCenter("enter your name",320,0,16,3);
	FOwner->Add( new TBox(FOwner,0,0,640,32,0,200,255));
	FOwner->Add( new TBox(FOwner,0,448,640,480,0,200,255));

	if( (DecidedTime == 0) ) Input();

	if( (DecidedTime > 0) && (Age-DecidedTime > 90) ) Die();
}

void TNameEntry2::Die()
{
	TOBJ::Die();
}

void TReplayAsk2::OnDecided()
{
	switch( Cursor->GetCurrentPos() ){
	case 0:
		{
			FOwner->SetRankInHeader();

			if( FOwner->Difficulty < KDIFFICULTY_SINGLE ) {
				FOwner->Replayer->SaveTempReplay(FOwner->NowStage, FOwner->RankinHeaderData);
				FOwner->Replayer->SaveSequence(FOwner->Difficulty,  FOwner->RankinHeaderData, FOwner->Replayer->SplitData);
			}
			else
				FOwner->Replayer->Save(FOwner->RankinHeaderData);

			FOwner->StageEnd();
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WADeside,1);
		}break;
	case 1:
		{
			FOwner->StageEnd();
		}
	}
	Die();
}

void TReplayAsk2::Input()
{
	for( u32 i = 0 ; i <= 7; i++ )
		if( ! ButtonON[i] ) ButtonFlg[i] = false;

	if( FOwner->Joy.StickY() == 0 ) StickFlg = false;

	if( (! ButtonFlg[0]) && ButtonON[0] ) {
		ButtonFlg[0] = true;
		OnDecided();
	}

	if( (! ButtonFlg[1]) && ButtonON[1] ) {
		FOwner->StageEnd();
		Die();
	}

	if( (StickFlg=false) && (FOwner->Joy.StickY() == 1) ) {
		if( Cursor != NULL ) Cursor->NextPoint(1);
		StickFlg = true;
		if( CreateSE ) FOwner->SoundEffect->WavPlay(WASelect,0);
	}

	if( (StickFlg=false) && (FOwner->Joy.StickY() == -1) ) {
		if( Cursor != NULL ) Cursor->NextPoint(-1);
		StickFlg = true;
		if( CreateSE ) FOwner->SoundEffect->WavPlay(WASelect,0);
	}
}

TReplayAsk2::TReplayAsk2( TOBJList* owner ) : TOBJ(owner)
{
	FObjectName = "TReplayAsk2";
	FOwner->GameMode = gOther;
	FKind = kGauge;
	StickFlg = false;

	CursorList = new TCursorList(FOwner);

	//カーソル生成
	Cursor  = new TGlassCursor(FOwner,224, 192, 160);
	FOwner->Add(Cursor);

	//画面作成
	CursorList->SetAppearPoint(-640,0);

	CursorList->MakeCursor("save replay ?",  0, 120, 1, 640, 0, KFONTKIND_BLIGHT, GLASSCURSOR_PATTERN_NONE, KCURSOR_POS_CENTER);
	CursorList->MakeCursor("",  32, 152, 1, 576, 1, KFONTKIND_NORMAL, GLASSCURSOR_PATTERN_LINE, KCURSOR_POS_CENTER);

	CursorList->MakeCursor("yes", 224, 192, 16, 160, 2, KFONTKIND_NORMAL, GLASSCURSOR_PATTERN_GLASS, KCURSOR_POS_CENTER);
	Cursor->SetMovePoint(224,192);
	CursorList->MakeCursor("no",  224, 256, 16, 160, 3, KFONTKIND_NORMAL, GLASSCURSOR_PATTERN_GLASS, KCURSOR_POS_CENTER);
	Cursor->SetMovePoint(224,256);

}

TReplayAsk2::~TReplayAsk2()
{
	SAFE_DELETE(CursorList);
	Cursor->Die();
}

void TReplayAsk2::Move()
{
	TOBJ::Move();
	// 帯表示
	FOwner->Add( new TBox(FOwner,0,0,640,32,0,200,255));
	FOwner->Add( new TBox(FOwner,0,448,640,480,0,200,255));

	Input();
}

void TReplayAsk2::Die()
{
	TOBJ::Die();
}

void TLocalReplayViewer::AddNextItems()
{
	if( TotalData <= 0 ) return;

	s32 offset  =  CurrentItemNumber;

	//画面中の最大表示数を求める
	UpdateCurrentItems();

	for( s32 i = 0 ; i < CurrentItems; i++ ) {
		TMojiAndGlassCursor* tmpObj  = new TMojiAndGlassCursor(FOwner,OrgX+16,OrgY+FirstYPos_Ranking+i*YSPACE,576,GLASSCURSOR_PATTERN_GLASS);
		FOwner->Add(tmpObj);
		CursorList.push_back(tmpObj);

		Cursor->SetMovePoint(16,FirstYPos_Ranking+i*YSPACE);

		//No.
		tmpObj->AddSentenceRight(InttoStr(offset+i), 64,0,255,255,255,KFONTKIND_NORMAL);

		//名前
		tmpObj->AddSentence(FOwner->PlayerNameToStr(FOwner->Replayer->RepFile[offset+i].PlayerName, MAX_NAME_LENGTH), 88,0,255,255,255,KFONTKIND_NORMAL);

		//スコア
		tmpObj->AddSentence(FOwner->ScoreToStr(FOwner->Replayer->RepFile[offset+i].Score), 256,0,255,255,255,KFONTKIND_NORMAL);


		//日付
		tmpObj->AddSentenceRight(FOwner->DateToStr(FOwner->Replayer->RepFile[offset+i].Year,
			FOwner->Replayer->RepFile[offset+i].Month,
			FOwner->Replayer->RepFile[offset+i].Day),
			592,0,255,255,255,KFONTKIND_NORMAL);


		if( (Age == 0) ) {
			tmpObj->SetSpeed(7, 7);
			tmpObj->SetDest(16,FirstYPos_Ranking+i*YSPACE,i*5);
			tmpObj->SetAlpha(0);
			tmpObj->SetDestAlpha(255,15,i*7);
		}
		else {
			tmpObj->SetSpeed(5, 5);
			tmpObj->SetDest(16,FirstYPos_Ranking+i*YSPACE,0);
			tmpObj->SetAlpha(0);
			tmpObj->SetDestAlpha(255,15,0);
		}
	}

}

void TLocalReplayViewer::Input()
{
	for( u32 i = 0; i <= 7; i++ )
		if( ! ButtonON[i] ) ButtonFlg[i] = false;

	if( (! ButtonFlg[0]) && ButtonON[0] ) {
		ButtonFlg[0] = true;
		switch( CurrentStatus ){
		case KNETRANK_SCR_RANKING:
			{
				if( CurrentItems>0 ) SetNextStatus(KNETRANK_SCR_DETAIL);
			}break;
		case KNETRANK_SCR_DETAIL:
			{
				if( TotalData>0 ) {
					GetReplay(Choice + CurrentItemNumber);
					Die();
					FOwner->ReplayON = true;
					FOwner->Difficulty  =  FOwner->Replayer->RepDifficulty;
					FOwner->AfterLoopEvent = MLoadBeforePlay;
					if( CreateSE ) FOwner->SoundEffect->WavPlay(WADeside,1);
				}

			}break;
		}
	}

	if( (! ButtonFlg[1]) && ButtonON[1] ) {
		ButtonFlg[1] = true;
		//前の画面へ
		switch( CurrentStatus ){
		case KNETRANK_SCR_RANKING:
			{
				Die();
				FOwner->NetReplayState = -1;
				FOwner->AfterLoopEvent = MTitle;
			}break;
		case KNETRANK_SCR_DETAIL:
			{
				SetNextStatus(KNETRANK_SCR_DETAIL_RANK);
			}
		}
	}

	if( FOwner->Joy.StickX() == 0 ) StickFlgX = false;
	if( FOwner->Joy.StickY() == 0 ) StickFlgY = false;

	if( TotalData > 0 ) {

		if( (FOwner->Joy.StickY() == 1) ) {
			KeyDownCount++;
			if( (StickFlgY == false) || (KeyDownCount > 30) ) {    //新規に押下するか、60フレーム押し続ければ処理を有効
				if( (KeyDownCount > 30) ) KeyDownCount = 25;  //キー押し続けの場合、カーソル移動が早くなりすぎるのを防ぐ
				if( Cursor  !=  NULL ) {
					Cursor->NextPoint(1);
				}
				if( CurrentItems > 0 ) {
					Choice++;
					if( (Choice >= CurrentItems) ) {
						Choice = 0;
						if( (CurrentItems != TotalData) &&
							((CurrentStatus == KNETRANK_SCR_RANKING) || (CurrentStatus == KNETRANK_SCR_DETAIL)) ) MakeRightItems();
					}

					if( CurrentStatus == KNETRANK_SCR_DETAIL ) ShowDetail();

					UpdateBustShot();


					if( CreateSE ) FOwner->SoundEffect->WavPlay(WASelect,0);
				}
				StickFlgY = true;
			}
		}
		else {
			KeyDownCount  =  0;
		}

		if( (FOwner->Joy.StickY() == -1) ) {
			KeyUpCount++;
			if( (StickFlgY == false) || (KeyUpCount > 30)  ) {
				if( (KeyUpCount > 30) ) KeyUpCount  =  25;
				if( CurrentItems > 0 ) {
					Choice--;
					if( (Choice < 0) ) {
						if( (CurrentItems != TotalData) &&
							((CurrentStatus == KNETRANK_SCR_RANKING) || (CurrentStatus == KNETRANK_SCR_DETAIL)) ) MakeLeftItems();
						Choice = CurrentItems - 1;
					}


					if( CurrentStatus == KNETRANK_SCR_DETAIL ) ShowDetail();
					UpdateBustShot();

					if( CreateSE ) FOwner->SoundEffect->WavPlay(WASelect,0);
				}
				if( Cursor  !=  NULL ) {
					Cursor->NextPoint(-1);
				}
				StickFlgY = true;
			}
		}
		else {
			KeyUpCount  =  0;
		}

		if( CurrentStatus == KNETRANK_SCR_RANKING ) {
			if( (StickFlgX == false) && (FOwner->Joy.StickX() == 1) ) {
				if( (CurrentItems != TotalData) ) {
					MakeRightItems();
					if( (Choice >= CurrentItems) ) {
						Choice = CurrentItems - 1;
						Cursor->MoveToEnd();
					}
					else {
						Cursor->NextPoint(Choice);
					}
				}
				UpdateBustShot();
				StickFlgX = true;
				if( CreateSE ) FOwner->SoundEffect->WavPlay(WASelect,0);
			}

			if( (StickFlgX == false) && (FOwner->Joy.StickX() == -1) ) {
				if( CurrentItems != TotalData ) {
					MakeLeftItems();
					if( (Choice >= CurrentItems) ) {
						Choice = CurrentItems - 1;
						Cursor->MoveToEnd();
					}
					else {
						Cursor->NextPoint(Choice);
					}
				}
				UpdateBustShot();
				StickFlgX = true;
				if( CreateSE ) FOwner->SoundEffect->WavPlay(WASelect,0);
			}
		}
	}
}

s32 TLocalReplayViewer::GetReplay( s32 _choice )
{
	FOwner->Replayer->Load(FOwner->Replayer->RepFile[_choice].FileName);
	return 0;
}

void TLocalReplayViewer::ShowDetail()
{
	for( u32 i = 0 ; i < CursorList.size(); i++ ) {
		CursorList[i]->Die();
	}
	CursorList.clear();


	s32 itemnum  =  Choice + CurrentItemNumber;

	string btype;
	string wtype;
	if(FOwner->Replayer->RepFile[itemnum].BType == 0 )
		btype  =  "alternate";
	else
		btype  =  "momentary";

	if(FOwner->Replayer->RepFile[itemnum].WType == 0 )
		wtype  =  "rifle";
	else
		wtype  =  "blade";

	OrgX = -640;
	OrgY = 0;
	//名前
	MakeCursor(FOwner->PlayerNameToStr(FOwner->Replayer->RepFile[itemnum].PlayerName, MAX_NAME_LENGTH), 384,112, 48, GLASSCURSOR_PATTERN_NONE, KFONTKIND_BLIGHT, 0);

	//難易度
	string diff;
	switch( FOwner->Replayer->RepFile[itemnum].Difficulty ){
	case 0:diff = "easy";		break;
	case 1:diff = "normal";		break;
	case 2:diff = "hard";		break;
	default:
		diff  =  "single";		break;
	}

	MakeCursor("difficulty",  16,128+1*YSPACE, 576, GLASSCURSOR_PATTERN_BLIND, KFONTKIND_NORMAL, 1);
	CursorList[1]->AddSentenceRight(diff, 576,0,255,255,255,KFONTKIND_NORMAL);

	//スコア
	MakeCursor("score",  16,128+2*YSPACE, 576, GLASSCURSOR_PATTERN_BLIND, KFONTKIND_NORMAL, 1);
	CursorList[2]->AddSentenceRight(FOwner->ScoreToStr(FOwner->Replayer->RepFile[itemnum].Score), 576,0,255,255,255,KFONTKIND_NORMAL);

	//タイム
	MakeCursor("time",  16,128+3*YSPACE, 576, GLASSCURSOR_PATTERN_BLIND, KFONTKIND_NORMAL, 1);

	if( FOwner->Replayer->RepFile[itemnum].Difficulty <= 2 )
		CursorList[3]->AddSentenceRight(TotalTimeToStr(FOwner->Replayer->RepFile[itemnum].Time), 576,0,255,255,255,KFONTKIND_NORMAL);
	else
		CursorList[3]->AddSentenceRight(FOwner->TimeToStr(FOwner->Replayer->RepFile[itemnum].Time), 576,0,255,255,255,KFONTKIND_NORMAL);

	//ブーストタイプ
	MakeCursor("boost type",  16,128+4*YSPACE, 576, GLASSCURSOR_PATTERN_BLIND, KFONTKIND_NORMAL, 1);
	CursorList[4]->AddSentenceRight(btype, 576,0,255,255,255,KFONTKIND_NORMAL);

	//ウェポンタイプ
	MakeCursor("weapon type",  16,128+5*YSPACE, 576, GLASSCURSOR_PATTERN_BLIND, KFONTKIND_NORMAL, 1);
	CursorList[5]->AddSentenceRight(wtype, 576,0,255,255,255,KFONTKIND_NORMAL);

	//時間
	MakeCursor("date",  16,128+6*YSPACE, 576, GLASSCURSOR_PATTERN_BLIND, KFONTKIND_NORMAL, 1);
	CursorList[6]->AddSentenceRight(FOwner->DateToStr(FOwner->Replayer->RepFile[itemnum].Year,
		FOwner->Replayer->RepFile[itemnum].Month,
		FOwner->Replayer->RepFile[itemnum].Day), 576,0,255,255,255,KFONTKIND_NORMAL);

	//フレームスキップ
	MakeCursor("frameskip", 16,128+7*YSPACE, 576, GLASSCURSOR_PATTERN_BLIND, KFONTKIND_NORMAL, 1);
	CursorList[7]->AddSentenceRight(FOwner->FrameSkipRateToStr(FOwner->Replayer->RepFile[itemnum].FrameSkipRate), 576,0,255,255,255,KFONTKIND_NORMAL);

	OrgX = 16;
	OrgY = 128;

	//区切り線
	TMojiAndGlassCursor* tmpObj  = new TMojiAndGlassCursor(FOwner,16-640,136,608,GLASSCURSOR_PATTERN_LINE);
	FOwner->Add(tmpObj);
	CursorList.push_back(tmpObj);
	tmpObj->SetSpeed(5, 5);
	tmpObj->SetDest(16,136,0);
	tmpObj->SetAlpha(0);
	tmpObj->SetDestAlpha(200,15,0);

}

void TLocalReplayViewer::MakeRightItems()
{
	CurrentItemNumber += KNETRANK_REP_MAX;

	if( (CurrentItemNumber >= TotalData) ) CurrentItemNumber  =  0;

	if( CurrentStatus == KNETRANK_SCR_RANKING ) {
		//項目生成位置
		OrgX  =  640;
		OrgY  =  0;

		//現在表示中の項目を左へ流す
		for( u32 i = 0 ; i < CursorList.size(); i++ ) {
			TMojiAndGlassCursor* tmpObj = CursorList[i];
			tmpObj->SetSpeed(5, 5);
			tmpObj->SetDest(-640+16,tmpObj->Y,0);
			tmpObj->SetDestAlpha(0,15,0);
			tmpObj->SetDeathPoint(-640+16,tmpObj->Y);
		}

		CursorList.clear();

		if( Cursor  !=  NULL ) Cursor->ClearPoints();

		AddNextItems();
	}
	else 
		UpdateCurrentItems();

}

void TLocalReplayViewer::MakeLeftItems()
{
	CurrentItemNumber -= KNETRANK_REP_MAX;

	if( (CurrentItemNumber < 0) )
		CurrentItemNumber  =  ((TotalData-1) / KNETRANK_REP_MAX)*KNETRANK_REP_MAX;

	if( CurrentStatus == KNETRANK_SCR_RANKING ) {
		//項目生成位置
		OrgX  =  -640+16;
		OrgY  =  0;

		//現在表示中の項目を右へ流す
		for( u32 i = 0 ; i < CursorList.size(); i++ ) {
			TMojiAndGlassCursor* tmpObj =  CursorList[i];
			tmpObj->SetSpeed(5, 5);
			tmpObj->SetDest(640+16,tmpObj->Y,0);
			tmpObj->SetDestAlpha(0,15,0);
			tmpObj->SetDeathPoint(640+16,tmpObj->Y);
		}

		CursorList.clear();

		if( Cursor  !=  NULL ) Cursor->ClearPoints();
		AddNextItems();
	}
	else 
		UpdateCurrentItems();
}

void TLocalReplayViewer::SetNextStatus( s32 _next )
{
	ClearScreen();
	if( Cursor  !=  NULL ) Cursor->ClearPoints();
	switch( _next ){
	case KNETRANK_SCR_STAGE:
		{

		}break;
	case KNETRANK_SCR_RANKING:
		{
			//カーソル生成
			Cursor  = new TGlassCursor(FOwner,16, FirstYPos_Ranking, 576);
			FOwner->Add(Cursor);
			Cursor->Hide();    //最初は非表示
			Choice  =  0;
			Age  =  0;
			OrgX = 0;
			OrgY = 480;
			GetData();
		}break;
	case KNETRANK_SCR_DETAIL:
		{
			OrgX = 16;
			OrgY = 128;
			ClearBGItems();
			ShowDetail();
		}break;
	case KNETRANK_SCR_DETAIL_RANK:
		{
			//カーソル生成
			Cursor  = new TGlassCursor(FOwner,16, FirstYPos_Ranking+Choice*YSPACE, 576);
			FOwner->Add(Cursor);
			OrgX = 640+16;
			OrgY = 0;
			AddNextItems();
			Cursor->NextPoint(Choice);
			MakeItemName();
			_next  =  KNETRANK_SCR_RANKING;
		}break;
	}

	CurrentStatus  =  _next;

}

void TLocalReplayViewer::ClearScreen()
{
	if( Cursor  !=  NULL ) {
		Cursor->Die();
		Cursor  =  NULL;
	}

	switch( CurrentStatus ){
	case KNETRANK_SCR_STAGE:
		{

		}break;
	case KNETRANK_SCR_RANKING:
		{
			//現在表示中の項目を右へ流す
			for( u32 i = 0 ; i < CursorList.size(); i++ ) {
				TMojiAndGlassCursor* tmpObj =  CursorList[i];
				tmpObj->SetSpeed(5, 5);
				tmpObj->SetDest(640+16,tmpObj->Y,0);
				tmpObj->SetDestAlpha(0,15,0);
				tmpObj->SetDeathPoint(640+16,tmpObj->Y);
			}
		}break;
	case KNETRANK_SCR_DETAIL:
		{
			//現在表示中の項目を左へ流す
			for( u32 i = 0 ; i < CursorList.size(); i++ ) {
				TMojiAndGlassCursor* tmpObj =  CursorList[i];
				tmpObj->SetSpeed(5, 5);
				tmpObj->SetDest(-640+16,tmpObj->Y,0);
				tmpObj->SetDestAlpha(0,15,0);
				tmpObj->SetDeathPoint(-640+16,tmpObj->Y);
			}
		}break;
	}

	CursorList.clear();
}

void TLocalReplayViewer::UpdateCurrentItems()
{
	//画面中の最大表示数を求める
	if( (TotalData - CurrentItemNumber) < KNETRANK_REP_MAX )
		CurrentItems  =  (TotalData - CurrentItemNumber);
	else
		CurrentItems  =  KNETRANK_REP_MAX;
}

void TLocalReplayViewer::MakeItemName()
{
	//区切り線
	TMojiAndGlassCursor* tmpObj  = new TMojiAndGlassCursor(FOwner,16-640,FirstYPos_Ranking - 24,608,GLASSCURSOR_PATTERN_LINE);
	FOwner->Add(tmpObj);
	BGItemList.push_back(tmpObj);
	tmpObj->SetSpeed(5, 5);
	tmpObj->SetDest(16,FirstYPos_Ranking - 24,0);
	tmpObj->SetAlpha(0);
	tmpObj->SetDestAlpha(200,15,0);

	//名前
	MakeSentence("name", 120, FirstYPos_Ranking - 48, 0);

	//スコア
	MakeSentence("score", 296, FirstYPos_Ranking - 48, 1);

	//日付
	MakeSentence("date", 488, FirstYPos_Ranking - 48, 3);
}

void TLocalReplayViewer::MakeSentence( const string& _caption, s32 _x, s32 _y, s32 _delay )
{
	TMojiAndGlassCursor* tmpObj  = new TMojiAndGlassCursor(FOwner,_x-640,_y,48,GLASSCURSOR_PATTERN_NONE);
	FOwner->Add(tmpObj);
	BGItemList.push_back(tmpObj);
	tmpObj->AddSentence(_caption, 0,4,255,255,255,KFONTKIND_BLIGHT);
	tmpObj->SetSpeed(5, 5);
	tmpObj->SetDest(_x,_y,_delay*5);
	tmpObj->SetAlpha(0);
	tmpObj->SetDestAlpha(255,15,_delay*7);
}

void TLocalReplayViewer::ClearBGItems()
{
	//現在表示中の項目を左へ流す
	for( u32 i = 0 ; i < BGItemList.size(); i++ ) {
		TMojiAndGlassCursor* tmpObj =  BGItemList[i];
		tmpObj->SetSpeed(5, 5);
		tmpObj->SetDest(-640,tmpObj->Y,0);
		tmpObj->SetDestAlpha(0,15,0);
		tmpObj->SetDeathPoint(-640,tmpObj->Y);
	}

	BGItemList.clear();
}

void TLocalReplayViewer::MakeCursor( const string& _caption, s32 _x, s32 _y, s32 _long, s32 _pat, s32 _font, s32 _delay )
{
	TMojiAndGlassCursor* tmpObj  = new TMojiAndGlassCursor(FOwner,OrgX+_x,OrgY+_y,_long,_pat);
	FOwner->Add(tmpObj);
	CursorList.push_back(tmpObj);

	tmpObj->AddSentence(_caption, 32, 4, 255, 255, 255,_font);

	tmpObj->SetSpeed(5, 5);
	tmpObj->SetDest(_x,_y,_delay*2);
	tmpObj->SetAlpha(0);
	tmpObj->SetDestAlpha(255,10,_delay*2);
}

void TLocalReplayViewer::UpdateBustShot()
{
	if( TotalData>0 ) {

		if(FOwner->Replayer->RepFile[Choice + CurrentItemNumber].WType == 0 ) {
			BustShot_Rifle->Show();
			BustShot_Blade->Hide();
		}
		else {
			BustShot_Rifle->Hide();
			BustShot_Blade->Show();
		}

		((TStageSelectScreenMng*)FOwner->StageSelect)->ChangeStage(FOwner->Replayer->RepFile[Choice + CurrentItemNumber].Stage);
		BustShot_Rifle->ChangeColor(FOwner->Replayer->RepFile[Choice + CurrentItemNumber].Stage);
		BustShot_Blade->ChangeColor(FOwner->Replayer->RepFile[Choice + CurrentItemNumber].Stage);
	}
}

void TLocalReplayViewer::GetData()
{
	//データを取得
	FOwner->Replayer->ListUp();
	TotalData  =  0;
	for( u32 i = 0; i <= 9; i++ ) {
		if( FOwner->Replayer->RepFile[i].Score>0 ) {
			TotalData++;
		}
	}

	//データがあればカーソルを表示
	if( TotalData > 0 ) {
		Cursor->Show();
		AddNextItems();
		UpdateBustShot();
	}

	MakeItemName();
}

TLocalReplayViewer::TLocalReplayViewer( TOBJList* owner ) : TOBJ(owner)
{
	FObjectName = "TLocalReplayViewer";
	FKind = kGauge;
	//  FOwner->GameMode = gOther;
	Choice = 0;

	StickFlgX = true;
	StickFlgY = true;

	TotalData = 0;

	OrgX  =  0;
	OrgY  =  480;
	DestX  =  0;
	DestY  =  0;

	SlideTime  =  0;
	XSpeed  =  0;
	YSpeed  =  0;

	KeyUpCount  =  0;
	KeyDownCount  =  0;

	Cursor  =  NULL;

	CurrentItems  =  0;
	CurrentItemNumber  =  0;

	CurrentStatus  =  KNETRANK_SCR_ORDER;

	for( u32 i = 0; i <= 9; i++ ) {
		ActFlg[i] = 0;
		ActCount[i] = 0;
	}

	//音楽ストップ
	if( CreateBGM ) FOwner->MusicPlayer->FadeOut(2,0.0,0.5);

	BustShot_Rifle  = new TBustShot(FOwner,640,0,160,0,0,false);
	BustShot_Blade  = new TBustShot(FOwner,640,0,160,0,1,false);
	FOwner->Add(BustShot_Rifle);
	FOwner->Add(BustShot_Blade);
	BustShot_Rifle->Hide();
	BustShot_Blade->Hide();

	SetNextStatus(KNETRANK_SCR_RANKING);
}

TLocalReplayViewer::~TLocalReplayViewer()
{
	for( u32 i = 0 ; CursorList.size(); i++ ) {
		CursorList[i]->Die();
	}

	CursorList.clear();

	for( u32 i = 0 ; BGItemList.size(); i++ ) {
		BGItemList[i]->Die();
	}

	BGItemList.clear();

	if( Cursor  !=  NULL ) Cursor->Die();

	BustShot_Rifle->Die();
	BustShot_Blade->Die();
}

void TLocalReplayViewer::Move()
{
	TOBJ::Move();
	FOwner->SentenceCenter("replay",320,0,32,3);
	FOwner->Add( new TBox(FOwner,0,0,640,32,0,200,255));
	FOwner->Add( new TBox(FOwner,0,448,640,480,0,200,255));

	Input();
}

void TLocalReplayViewer::Die()
{
	TOBJ::Die();
}

void TTutorial::OnDecided()
{

}

void TTutorial::Input()
{
	if( ! ButtonON[0] ) ButtonFlg_local = false;

	if( (! ButtonFlg_local) && ButtonON[0] ) {
		ButtonFlg_local = true;
		Die();
	}
}

TTutorial::TTutorial( TOBJList* owner ) : TOBJ(owner)
{
	FObjectName = "TTutorial";
	FOwner->DemoON = true;
	FOwner->DemoNumber  =  KDEMONUM_TUTORIAL;
	FOwner->HiScore = 0;
	FKind = kGauge;
	FOwner->GameMode = gPlay;

	ButtonFlg_local = true;

	KeyBoard  = new TKeyBoard(FOwner);
	FOwner->Add(KeyBoard);

	CursorList = new TCursorList(FOwner);

	TJiki* tmpObj = new TJiki(FOwner,-32,144);
	tmpObj->DoStartEvent();
	FOwner->Add(tmpObj);

	//画面作成
	CursorList->SetAppearPoint(640,0);

	FOwner->CreateHUD();

	Form1.Script.UnDone(0);
}

TTutorial::~TTutorial()
{
	SAFE_DELETE(CursorList);
	KeyBoard->Die();
}

void TTutorial::Move()
{
	TOBJ::Move();
	Form1.Script.ExecuteProc(KSCRIPT_TUTORIAL,this);

	Input();
}

void TTutorial::Die()
{
	TOBJ::Die();
	FOwner->AfterLoopEvent = MWeaponSelect;
}

void TTutorial::UpdateMessage( const string& _msg, s32 _x, s32 _y, s32 _blank, s32 _width, s32 _num )
{
	if( _num == 0 ) CursorList->ClearCursors(-640, 0);

	CursorList->MakeCursor(_msg,_x,_y,_blank,_width,_num,KFONTKIND_ORANGE, GLASSCURSOR_PATTERN_BLACK_CENTER, KCURSOR_POS_CENTER);

}

void TStage1StartingDemo::OnEraser()
{
	for( u32 i = 0 ; i < MAX_FRIENDS ; i++) {
		if( friends[i] != NULL ) {
			FOwner->Add( new TExpSmall(FOwner, friends[i]->X, friends[i]->Y, 0, 5, 0));
			friends[i]->Disappear();
			friends[i] = NULL;
		}
	}

	mother->MakeSmoke();

	FOwner->Replayer->NowButton[2] = true;
}

void TStage1StartingDemo::Input()
{
	if( ! ButtonON[0] ) ButtonFlg_local = false;

	if( (! ButtonFlg_local) && ButtonON[0] ) {
		ButtonFlg_local = true;
		Skip();
	}
}

void TStage1StartingDemo::Skip()
{
	FOwner->SoundEffect->AllStop();
	FOwner->AfterLoopEvent = MRestart;
	FOwner->DemoON = false;
	Die();
}

void TStage1StartingDemo::SetNextState()
{
	FOwner->AfterLoopEvent = MReady;
	Die();
}

void TStage1StartingDemo::Makefriends()
{
	mother =new TMotherShip(FOwner,450-550,250);
	FOwner->Add(mother);

	friends[0] =new TFriend(FOwner,200,200,0.75f,3);
	friends[1] =new TFriend(FOwner,400,250,0.5f,3);
	friends[2] =new TFriend(FOwner,150,150,0.65f,3);
	friends[3] =new TFriend(FOwner,400,100,0.75f,3);
	friends[4] =new TFriend(FOwner,150,300,0.65f,3);
	friends[5] =new TFriend(FOwner,340,200,0.55f,3);
	friends[6] =new TFriend(FOwner,87,295,0.85f,3);
	friends[7] =new TFriend(FOwner,35,80,0.75f,3);
	friends[8] =new TFriend(FOwner,00,330,0.65f,3);
	friends[9] =new TFriend(FOwner,180,55,0.55f,3);

	for( u32 i  =  0 ; i < MAX_FRIENDS ; i++ ) {
		FOwner->Add(friends[i]);
		friends[i]->BoostSoon(true);
		friends[i]->Sprite[1]->MoveR(-550,0);
	}
}

TStage1StartingDemo::TStage1StartingDemo( TOBJList* owner ) : TOBJ(owner)
{
	FObjectName = "TStage1StartingDemo";
	FOwner->DemoON = true;
	FOwner->DemoNumber  =  KDEMONUM_STAGE1START;
	FKind = kGauge;
	eraser = NULL;
	mother = NULL;
	FOwner->AcceptChangeSpeed = false;

	for( u32 i  =  0 ; i < MAX_FRIENDS; i++ ) {
		friends[i] = NULL;
	}

	if( FOwner->HeroWeapon == 0 )
		((TJiki*)FOwner->JikiObj)->BoostSoon(true);
	else
		((TJiki2*)FOwner->JikiObj)->BoostSoon(true);

	FOwner->JikiObj->Sprite[1]->MoveR(-496,0);

	ButtonFlg_local = true;
	FOwner->RequestedSpeedFromChild = -20;

	if( CreateSE ) FOwner->SoundEffect->WavPlay(WABoost,1);

	if( CreateBGM ) FOwner->MusicPlayer->Play(0);
}

TStage1StartingDemo::~TStage1StartingDemo()
{
	for( u32 i  =  0 ; i < MAX_FRIENDS; i++ ) {
		if( friends[i] != NULL )
			friends[i]->Disappear();
	}
	FOwner->AcceptChangeSpeed = true;
}

void TStage1StartingDemo::Move()
{
	TOBJ::Move();

	FOwner->Add( new TBox(FOwner,0,0,640,32,0,200,255));
	FOwner->Add( new TBox(FOwner,0,448,640,480,0,200,255));

	if( Age == 40 ){
		Makefriends();
	}
	else if( Age >= 60 && Age <= 89 ){
		FOwner->JikiObj->Sprite[1]->MoveR(-FOwner->Speed+11,0);
		FOwner->RequestedSpeedFromChild  =  FOwner->Speed+1;
	}
	else if( Age == 90 ){
		FOwner->AcceptChangeSpeed = true;
		FOwner->JikiObj->Sprite[1]->Move(320,240);
	}
	else if( Age == 150 ){
		eraser = new TDemoEraser(FOwner);
		FOwner->Add(eraser);
	}
	else if( Age == 240 ){
		OnEraser();
	}
	else if( Age == 430 ){
		SetNextState();
	}

	Input();
}

void TStage2StartingDemo::Input()
{
	if( ! ButtonON[0] ) ButtonFlg_local = false;

	if( (! ButtonFlg_local) && ButtonON[0] ) {
		ButtonFlg_local = true;
		Skip();
	}
}

void TStage2StartingDemo::Skip()
{
	Age  =  154;
	FOwner->SoundEffect->AllStop();

	//ボスを画面から消すため、一時的にスピードを変更する。
	FOwner->RequestedSpeedFromChild = -1000;

	FOwner->GetBG3DMan()->SetCameraOffset(0,0,0);

	Blur->Die();
	Blur = NULL;

	if( friends[0] == NULL ) Makefriends();

}

void TStage2StartingDemo::SetNextState()
{
	FOwner->AfterLoopEvent = MReady;
	Die();
}

void TStage2StartingDemo::Makefriends()
{
	friends[0] =new TFriend(FOwner,0,0,0.65f,6);
	friends[1] =new TFriend(FOwner,0,0,0.65f,6);
	friends[2] =new TFriend(FOwner,0,0,0.5f,6);
	friends[3] =new TFriend(FOwner,0,0,0.75f,6);
	friends[4] =new TFriend(FOwner,0,0,0.55f,6);
	friends[5] =new TFriend(FOwner,0,0,0.55f,6);
	friends[6] =new TFriend(FOwner,0,0,0.85f,6);
	friends[7] =new TFriend(FOwner,0,0,0.75f,6);
	friends[8] =new TFriend(FOwner,0,0,0.65f,6);
	friends[9] =new TFriend(FOwner,0,0,0.75f,6);

	captain =new TFriend(FOwner,0,0,0.95f,5);

	ReseTFriendPos();

	for( u32 i  =  0 ; i < MAX_FRIENDS; i++ ) {
		FOwner->Add(friends[i]);
		friends[i]->Sprite[1]->MoveR(-550*friends[i]->zoomrate,0);
	}

	FOwner->Add(captain);
	captain->Sprite[1]->MoveR(-550*captain->zoomrate,0);

	boostedcount = 0;
}

void TStage2StartingDemo::ReseTFriendPos()
{
	friends[0]->Sprite[1]->Move(550,200);
	friends[1]->Sprite[1]->Move(500,380);
	friends[2]->Sprite[1]->Move(400,300);
	friends[3]->Sprite[1]->Move(400,150);
	friends[4]->Sprite[1]->Move(380,105);
	friends[5]->Sprite[1]->Move(340,250);
	friends[6]->Sprite[1]->Move(287,355);
	friends[7]->Sprite[1]->Move(200,250);
	friends[8]->Sprite[1]->Move(150,350);
	friends[9]->Sprite[1]->Move(135,130);
	captain->Sprite[1]->Move(500,240);
}

TStage2StartingDemo::TStage2StartingDemo( TOBJList* owner ) : TOBJ(owner)
{
	FObjectName = "TStage2StartingDemo";
	FOwner->DemoON = true;
	FOwner->DemoNumber  =  KDEMONUM_STAGE2START;
	FKind = kGauge;
	FOwner->AcceptChangeSpeed = false;

	Blur =new TStage2ScrollBlur(FOwner);
	FOwner->Add(Blur);

	for( u32 i  =  0 ; i < MAX_FRIENDS ; i++) {
		friends[i] = NULL;
	}
	captain = NULL;

	//  FOwner->JikiObj.Sprite[1].MoveR(-550,0);        //Demoで移動されるので不要

	ButtonFlg_local = true;
	FOwner->RequestedSpeedFromChild = -20;

	FOwner->SetBloomingColorSoon(255,0,60,47,47);

	//リスタート時はgReadyになっているので、最初から演出をスキップさせる
	if( FOwner->GameMode == gReady ) {
		Skip(); 
	}
	else {
		FOwner->Distance = 2000;
		FOwner->GetBG3DMan()->SetCameraOffset(1400,0,0);   //ボスのいる位置までカメラをずらす

		if( CreateSE ) FOwner->SoundEffect->WavPlay(WABoost,1);
		FOwner->Add( new TStage2Boss2(FOwner,-100,240));
		//    Blur.Show(false);
		if( CreateBGM ) {
			FOwner->MusicPlayer->Stop();  //ムービーシーンの音楽を止める
			FOwner->MusicPlayer->Play(1);
		}
	}
}

TStage2StartingDemo::~TStage2StartingDemo()
{
	if( Blur != NULL ) Blur->Die();



	FOwner->AcceptChangeSpeed = true;

}

void TStage2StartingDemo::Move()
{
	TOBJ::Move();

	if( 0 <= Age && Age <= 30 ){ 
		FOwner->RequestedSpeedFromChild  =  FOwner->Speed+1;
	}
	else if( Age == 80 ){ 
		FOwner->RequestedSpeedFromChild  =  -20;
	}
	else if( Age == 90 ){ 
		Blur->Show(true);
	}
	else if( Age == 120 ){ 
		Makefriends();
	}
	else if( Age == 128 ){ 
		FOwner->Distance = 2355;
		FOwner->GetBG3DMan()->SetCameraOffset(0,0,0);
	}
	else if( Age == 129 ){ 
		Blur->Show(false);
	}
	else if( 130 <= Age && Age <= 154 ){ 
		FOwner->RequestedSpeedFromChild  =  FOwner->Speed+1;
	}
	else if( Age == 155 ){ 
		FOwner->StartDistance = 2115;

		ReseTFriendPos();
		FOwner->Speed = 5;
		//      FOwner->DemoON = false;    ←ready画面生成時にOFFになるのでいらない
		FOwner->AfterLoopEvent = MReady;
		FOwner->AcceptChangeSpeed = true;
		FOwner->JikiObj->Sprite[1]->Move(320,240);

		captain->DoAction(KACTION_FRIEND_FLAG);
	}
	else if( 255. <= Age && Age <= 600 ){ 
		if( (boostedcount<=9) ) {
			for( u32 i = 0; i <= 9; i++ ) {
				s32 timing  =  RoundOff((1.f - (f32)friends[i]->X / 640.f ) * 80.f);
				if( ((Age - 255) >= timing) && (i >= boostedcount) ) {
					friends[i]->DoAction(KACTION_FRIEND_BOOST);
					boostedcount++;
				}
			}
		}
	}
	else if( Age == 601 ){ 
		Die();
	}

	if( Age == 10 ) Blur->Show(false);

	if( (Age >= 121) && (Age < 156) )
		FOwner->JikiObj->Sprite[1]->MoveR(-FOwner->Speed+5,0);


	if( FOwner->DemoON ) {
		FOwner->Distance += FOwner->Speed;
		Input();
		FOwner->Add( new TBox(FOwner,0,0,640,32,0,200,255));
		FOwner->Add( new TBox(FOwner,0,448,640,480,0,200,255));
	}
}

void TStage3StartingDemo::Input()
{
	if( ! ButtonON[0] ) ButtonFlg_local = false;

	if( (! ButtonFlg_local) && ButtonON[0] ) {
		ButtonFlg_local = true;
		Skip();
	}
}

void TStage3StartingDemo::Skip()
{
	Age  =  519;
	FOwner->SoundEffect->AllStop();
}

void TStage3StartingDemo::SetNextState()
{
	FOwner->AfterLoopEvent = MReady;
	Die();
}

void TStage3StartingDemo::MakeFriends()
{
	friends[0] =new TFriend(FOwner,0,0,0.65f,7);
	friends[1] =new TFriend(FOwner,0,0,0.85f,7);
	friends[2] =new TFriend(FOwner,0,0,0.75f,7);
	friends[3] =new TFriend(FOwner,0,0,0.65f,7);
	friends[4] =new TFriend(FOwner,0,0,0.75f,7);

	ResetFriendPos();

	for( u32 i  =  0; i < MAX_FRIENDS; i++ ) {
		FOwner->Add(friends[i]);
	}

	boostedcount = 0;

}

void TStage3StartingDemo::ResetFriendPos()
{
	friends[0]->Sprite[1]->Move(550,200);
	friends[1]->Sprite[1]->Move(200,380);
	friends[2]->Sprite[1]->Move(0,550);
	friends[3]->Sprite[1]->Move(100,550);
	friends[4]->Sprite[1]->Move(-50,350);
}

TStage3StartingDemo::TStage3StartingDemo( TOBJList* owner ) : TOBJ(owner)
{
	FObjectName = "TStage3StartingDemo";
	FOwner->DemoON = true;
	FOwner->DemoNumber  =  KDEMONUM_STAGE3START;
	FKind = kGauge;

	for( u32 i  =  0; i < MAX_FRIENDS; i++ ) {
		friends[i] = NULL;
	}

	ButtonFlg_local = true;
	FOwner->Distance = 0;

	FOwner->JikiObj->Sprite[1]->Move(320,240);
	MakeFriends();

	if( CreateBGM ) FOwner->MusicPlayer->Play(0);

	//リスタート時はgReadyになっているので、最初から演出をスキップさせる
	if( FOwner->GameMode == gReady ) {
		Skip(); 
	}
	else {

	}
}

TStage3StartingDemo::~TStage3StartingDemo()
{

}

void TStage3StartingDemo::Move()
{
	TOBJ::Move();

	if( Age == 1 ){
		if( CreateSE ) FOwner->SoundEffect->WavPlay(WAShipExp,0);
	}
	else if( Age == 350 ){
		friends[0]->DoAction(KACTION_FRIEND_BOOST);
	}
	else if( Age == 380 ){
		friends[1]->DoAction(KACTION_FRIEND_BOOST);
	}
	else if( Age == 420 ){
		friends[2]->DoAction(KACTION_FRIEND_BOOST);
	}
	else if( Age == 450 ){
		friends[3]->DoAction(KACTION_FRIEND_BOOST);
	}
	else if( Age == 470 ){
		friends[4]->DoAction(KACTION_FRIEND_BOOST);
	}
	else if( Age == 520 ){
			FOwner->StartDistance = 2600;
			FOwner->Speed = 5;
			//      FOwner->DemoON = false;    ←ready画面生成時にOFFになるのでいらない
			FOwner->AfterLoopEvent = MReady;
			FOwner->AcceptChangeSpeed = true;
			for( u32 i  =  0; i < MAX_FRIENDS; i++ ) {
				friends[i]->Disappear();
			}
		}
	else if( 521 <= Age && Age <= 700 ){
			//      if( (boostedcount<=9) ) {
			//        for( u32 i = 0; i <= 9; i++ ) {
			//          timing  =  Roundoff((1- friends[i].X/640) * 80);
			//          if( ((Age-521) >= timing) && (i>=boostedcount) ) {
			//            friends[i].DoAction(KACTION_FRIEND_BOOST);
			//            Inc(boostedcount);
			//          }
			//        }
			//
			//      }
		}
	else if( Age == 701 ){
		Die();
	}

	if( FOwner->DemoON ) {
		FOwner->Distance += FOwner->Speed;
		Input();
		FOwner->Add( new TBox(FOwner,0,0,640,32,0,200,255));
		FOwner->Add( new TBox(FOwner,0,448,640,480,0,200,255));
	}
}

void TStage4StartingDemo::Input()
{

}

void TStage4StartingDemo::Skip()
{

}

void TStage4StartingDemo::SetNextState()
{

}

void TStage4StartingDemo::Makefriends()
{
	friends[0] =new TFriend(FOwner,0,0,0.95f,8);
	friends[1] =new TFriend(FOwner,0,0,0.85f,8);
	friends[2] =new TFriend(FOwner,0,0,0.75f,8);

	ResetFriendPos();

	for( u32 i  =  0; i < MAX_FRIENDS; i++ ) {
		FOwner->Add(friends[i]);
		friends[i]->BoostSoon(true);
	}

	boostedcount = 0;
}

void TStage4StartingDemo::ResetFriendPos()
{
	friends[0]->Sprite[1]->Move(550,340);
	friends[1]->Sprite[1]->Move(200,380);
	friends[2]->Sprite[1]->Move(280,150);
}

TStage4StartingDemo::TStage4StartingDemo( TOBJList* owner ) : TOBJ(owner)
{
	FObjectName = "TStage4StartingDemo";
	FKind = kGauge;

	for( u32 i  =  0; i < MAX_FRIENDS; i++ ) {
		friends[i] = NULL;
	}

	ButtonFlg_local = true;
	FOwner->Distance = 520;
	FOwner->StartDistance = 520;
	FOwner->Speed = 5;

	FOwner->JikiObj->Sprite[1]->Move(320,240);

	if( CreateBGM ) FOwner->MusicPlayer->Play(0);
	if( CreateBGM ) FOwner->MusicPlayer->FadeOut(1,0.0,0.5);
}

TStage4StartingDemo::~TStage4StartingDemo()
{

}

void TStage4StartingDemo::Move()
{
	TOBJ::Move();
	switch( Age ){
	case 1:
		{
			FOwner->AfterLoopEvent = MReady;
			if( FOwner->HeroWeapon == 0 )
				((TJiki*)FOwner->JikiObj)->BoostSoon(true);
			else
				((TJiki2*)FOwner->JikiObj)->BoostSoon(true);
		}break;
	case 150:
		{
			if( FOwner->HeroWeapon == 0 )
				((TJiki*)FOwner->JikiObj)->BoostByOther(false);
			else
				((TJiki2*)FOwner->JikiObj)->BoostByOther(false);
		}break;
	case 151:
		Die();
		break;
	}
}

void TSubmarineDemo::Input()
{

}

void TSubmarineDemo::Skip()
{
	FOwner->SoundEffect->AllStop();
}

TSubmarineDemo::TSubmarineDemo( TOBJList* owner ) : TOBJ(owner)
{
	FObjectName = "TSubmarineDemo";
	FOwner->DemoON = true;
	FKind = kGauge;
	FOwner->AcceptChangeSpeed = false;

	ButtonFlg_local = true;
	FOwner->Distance = 70000;

	if( CreateBGM ) FOwner->MusicPlayer->FadeOut(2,0.0f,0.5f);
}

void TSubmarineDemo::Move()
{
	TOBJ::Move();
	FOwner->JikiObj->Sprite[1]->MoveR(0,-10);
	switch( Age ){
	case 170:if( CreateSE ) FOwner->SoundEffect->WavPlay2(WASubmarine, 0, 100);		break;
	case 180:FOwner->Add( new TSubmarine(FOwner,-700,300));				break;
	case 390:
		{
			FOwner->Add( new TBoxContrl(FOwner,0));
		}break;
	case 510:
		{
			Die();
		}
	}

	FOwner->Distance += 5;
}

void TSubmarineDemo::Die()
{
	TOBJ::Die();

	if( (FOwner->ReplayON) && (FOwner->Difficulty == KDIFFICULTY_SINGLE) )
		FOwner->AfterLoopEvent = MRepView;
	else {
		if( (FOwner->Difficulty == KDIFFICULTY_SINGLE) || (TrialMode) || (FOwner->IsPractice))
			FOwner->AfterLoopEvent = MTitle;
		else
			FOwner->AfterLoopEvent = MLoadBeforePlay;
	}
}

void TStage4EndingDemo::SetNextState()
{

}

TStage4EndingDemo::TStage4EndingDemo( TOBJList* owner ) : TOBJ(owner)
{
	FObjectName = "TStage4EndingDemo";
	//  FOwner->DemoON = true;
	//  FOwner->DemoNumber  =  KDEMONUM_STAGE4START;
	FKind = kGauge;
	FOwner->AcceptChangeSpeed = false;
	ActFlg = 0;

	ButtonFlg_local = true;
}

TStage4EndingDemo::~TStage4EndingDemo()
{
	if( CreateSE ) FOwner->SoundEffect->WavStop(10);
}

void TStage4EndingDemo::Move()
{
	switch( ActFlg ){
	case 0:
		{
			if( Age % 600 == 0 )
				if( CreateSE ) FOwner->SoundEffect->WavPlay2(WABaseExp, 10, 150);
			if( FOwner->Distance >= 64300 ) {
				if( FOwner->HeroWeapon == 0 )
					((TJiki*)FOwner->JikiObj)->BoostByOther(false);
				else
					((TJiki2*)FOwner->JikiObj)->BoostByOther(false);

				ActFlg++;
			}
		}break;
	case 1:
		{
			if( Age % 600 == 0 )
				if( CreateSE ) FOwner->SoundEffect->WavPlay2(WABaseExp, 10, 150);
			if( FOwner->Distance >= 67500 ) {
				FOwner->Add( new TBoxContrl(FOwner,5));
				if( CreateSE ) FOwner->SoundEffect->WavPlay(WABossExp, 0);
				if( CreateSE ) FOwner->SoundEffect->WavStop(10);
				ActFlg++;
				if( CreateBGM && (! FOwner->ResultExists) ) FOwner->MusicPlayer->Stop();
			}
		}break;
	case 2:
		{
			//      FOwner->AcceptChangeSpeed = true;
			FOwner->BDieTime = FOwner->GetTick();
			ActFlg++;
		}break;
	}

	if( (FOwner->Distance < 72000) ) FOwner->Distance += 5;


	//イベント早送り用
	if( ! ButtonON[0] ) ButtonFlg[0] = false;

	if( (! ButtonFlg[0]) && ButtonON[0] && (! FOwner->ResultExists) && (FOwner->IsBossDefeated(3)) ) {
		FOwner->Add( new TResult2(FOwner));
		ButtonFlg[0] = true;
		if( CreateBGM ) FOwner->MusicPlayer->Stop();
	}

	TOBJ::Move();
}

void TStage5StartingDemo::Input()
{
	if( ! ButtonON[0] ) ButtonFlg_local = false;

	if( (! ButtonFlg_local) && ButtonON[0] ) {
		ButtonFlg_local = true;
		Skip();
	}
}

void TStage5StartingDemo::Skip()
{
	Age  =  59;

	if( friends[0] == NULL ) MakeFriends();
}

void TStage5StartingDemo::SetNextState()
{
	FOwner->AfterLoopEvent = MReady;
	Die();
}

void TStage5StartingDemo::MakeFriends()
{
	friends[0] =new TFriend(FOwner,0,0,0.85f,6);
	friends[1] =new TFriend(FOwner,0,0,0.85f,6);
	friends[2] =new TFriend(FOwner,0,0,0.7f,6);
	friends[3] =new TFriend(FOwner,0,0,0.75f,6);
	friends[4] =new TFriend(FOwner,0,0,0.75f,6);

	captain =new TFriend(FOwner,0,0,0.95f,5);

	ResetFriendPos();

	for( u32 i  =  0 ; i < MAX_FRIENDS; i++ ) {
		FOwner->Add(friends[i]);
	}

	FOwner->Add(captain);

	boostedcount = 0;
}

void TStage5StartingDemo::ResetFriendPos()
{
	friends[0]->Sprite[1]->Move(550,120);
	friends[1]->Sprite[1]->Move(400,380);
	friends[2]->Sprite[1]->Move(287,300);
	friends[3]->Sprite[1]->Move(150,150);
	friends[4]->Sprite[1]->Move(135,350);
	captain->Sprite[1]->Move(500,240);
}

TStage5StartingDemo::TStage5StartingDemo( TOBJList* owner ) : TOBJ(owner)
{
	FObjectName = "TStage5StartingDemo";
	FOwner->DemoON = true;
	FKind = kGauge;
	FOwner->StartDistance = 0;
	for( u32 i  =  0 ; i < MAX_FRIENDS; i++ ) {
		friends[i] = NULL;
	}
	captain = NULL;

	ButtonFlg_local = true;

	FOwner->JikiObj->Sprite[1]->Move(320,240);
	//リスタート時はgReadyになっているので、最初から演出をスキップさせる
	if( FOwner->GameMode == gReady ) {
		Skip();
	}

	if( CreateBGM ) FOwner->MusicPlayer->Play(0);
	if( CreateBGM ) FOwner->MusicPlayer->FadeOut(1, 0.0f, 0.5f);

}

TStage5StartingDemo::~TStage5StartingDemo()
{

}

void TStage5StartingDemo::Move()
{
	TOBJ::Move();

	if( Age == 1 ){
		MakeFriends();
	}
	else if( Age == 60 ){
		FOwner->StartDistance = 300;

		FOwner->AfterLoopEvent = MReady;
		FOwner->JikiObj->Sprite[1]->Move(320,240);

		captain->DoAction(KACTION_FRIEND_FLAG);
	}
	else if( 160 <= Age && Age <= 600 ){
		if( (boostedcount <= 4) ) {
			for( u32 i = 0; i <= 4; i++ ) {
				s32 timing  =  RoundOff((1.f - (f32)friends[i]->X / 640.f) * 80.f);
				if( ((Age-160) >= timing) && (i >= boostedcount) ) {
					friends[i]->DoAction(KACTION_FRIEND_BOOST);
					boostedcount++;
				}
			}
		}
	}
	else if( Age == 601 ){
		Die();
	}

	if( FOwner->DemoON ) {
		FOwner->Distance += FOwner->Speed;
		Input();
		FOwner->Add( new TBox(FOwner,0,0,640,32,0,200,255));
		FOwner->Add( new TBox(FOwner,0,448,640,480,0,200,255));
	}
}

TStageSelectScreenMng::TStageSelectScreenMng( TOBJList* owner ) : TOBJ(owner)
{
	FObjectName = "TStageSelectScreenMng";
	FKind = kGauge;
	CurrentStage = 0;

	if( ExtraEffects ) {
		MakeSpriteLite(FOwner->GetDG(), 0,0,0,700,540,15000,bmNormal);     //反射
		//反射スプライト設定
		//      AddTexture(Eff1Tex, 17, 1, 31, 15);
		Sprite[0]->GetSpr()->AddTexture(Eff1Tex, 65, 81, 112, 128);
		//      AddTexture(BGTex1, 1, 1, 256, 256);
		Sprite[0]->GetSpr()->SetColor(192,ctRed);
		Sprite[0]->GetSpr()->SetColor(192,ctBlue);
		Sprite[0]->GetSpr()->SetColor(192,ctGreen);
		//      SetColor(0,ctAlpha);
		Sprite[0]->GetSpr()->FEffectNum = 1;
		Sprite[0]->GetSpr()->FBumpSize = 1.0f;
		Sprite[0]->GetSpr()->FBumpTex = Eff7Tex;
		Sprite[0]->GetSpr()->FBumpLevel = 1;
		Sprite[0]->GetSpr()->Move(0,0);
	}

	ChangeStage(0);
}

TStageSelectScreenMng::~TStageSelectScreenMng()
{
	if( CurrentStage == 1 ) {
		Stage2Sea->Die();
		Stage2UnderSea->Die();
		Stage2Cloud->Die();
	}
}

void TStageSelectScreenMng::Move()
{
	TOBJ::Move();
	if( ExtraEffects && (Sprite[0] != NULL) ) {
		//          SetColor(trunc(DData.FCURRENT[0]),ctAlpha);
		//          BumpLevel = DData.FCURRENT[1];
		if( Sprite[0]->GetSpr()->FBumpLevel > 0 ) Sprite[0]->GetSpr()->FBumpLevel -= -0.033f;
		//          BumpSize = DData.FCURRENT[2];
		Sprite[0]->GetSpr()->SetColorR(-6,ctAlpha);
		Sprite[0]->GetSpr()->MoveR(-1,-1);
	}


	if( CurrentStage == 4 ){
		FOwner->GetScene()->CameraMoveR(0.05f, 0, 0);
		TotalSlide = TotalSlide+0.05f;
		if( TotalSlide >= 10 ) {
			FOwner->GetScene()->CameraMoveR(-TotalSlide, 0, 0);
			TotalSlide = TotalSlide - 10;
		}
	}
}

void TStageSelectScreenMng::ChangeStage( s32 _stage )
{

	if( (CurrentStage == 1) && (_stage != 1) ) {
		Stage2Sea->Die();
		Stage2UnderSea->Die();
		Stage2Cloud->Die();
	}

	switch( _stage ) {
	case 0:
	case 5:
		{
			FOwner->GetScene()->CameraMove(-20000,0,0);
			FOwner->GetScene()->CameraRotate(0,0,0);
			FOwner->SetBloomingColor(255,0,36,23,20);
		}break;
	case 1:
		{
			if( (CurrentStage != 1) ) {
				Stage2Sea  = new TSea(FOwner,0,3);
				Stage2UnderSea  =new TUnderSea(FOwner);
				Stage2Cloud  = new TDawncloud(FOwner);
				FOwner->Add(Stage2Sea);
				FOwner->Add(Stage2UnderSea);
				FOwner->Add(Stage2Cloud);
			}

			FOwner->GetScene()->SetFog(0x00ee3390, 5, 20);

			FOwner->SetBloomingColor(255,0,60,47,47);

			FOwner->GetScene()->CameraMove(0,0.3f,3);
			FOwner->GetScene()->CameraRotate(3968,0,0);
			FOwner->GetScene()->SetFOV(90);
			FOwner->GetScene()->GlobalLight->SetupDirectional(glm::vec3(0, -0.1f, 1.f));
		}break;
	case 2:
		{
			FOwner->GetScene()->CameraMove(20000,0,0);
			FOwner->GetScene()->CameraRotate(320,0,0);
			FOwner->GetScene()->SetFOV(90);
			FOwner->GetScene()->GlobalLight->SetupDirectional(glm::vec3(1.f, -0.1f, 0));
			FOwner->SetBloomingColor(255,0,47,47,47);
		}break;
	case 3:
		{
			FOwner->GetScene()->CameraMove(30000,-5,8.7f);
			FOwner->GetScene()->CameraRotate(0,0,0);
			FOwner->GetScene()->SetFOV(45);
			FOwner->GetScene()->GlobalLight->SetupDirectional(glm::vec3(0, 0, 1.f));
			FOwner->SetBloomingColor(255,0,47,47,47);
		}break;
	case 4:
		{
			FOwner->GetScene()->CameraMove(40000,6.184f,-4.09f);
			FOwner->GetScene()->CameraRotate(576,384,0);
			FOwner->GetScene()->SetFOV(45);
			FOwner->GetScene()->GlobalLight->SetupDirectional(glm::vec3(1.f, -0.5f, 1.f));
		}break;
	}

	if( (CurrentStage != _stage) && ExtraEffects ) {
		Sprite[0]->GetSpr()->FBumpLevel = 1;
		Sprite[0]->GetSpr()->SetColor(255,ctAlpha);
		Sprite[0]->GetSpr()->Move(0,0);
	}
	//  FOwner->Add( new TBoxContrl(FOwner,8));

	TotalSlide = 0;
	CurrentStage = _stage;

	if( _stage >= 5 )
		CurrentStage = 0;
}

s32 TStageSelectScreenMng::GetCurrentStage()
{
	return CurrentStage;
}

void TTitle2::Input()
{
	for( u32 i = 0; i <= 7; i++ ){
		if( ! ButtonON[i] )
			ButtonFlg[i] = false;
		else
			ButtonUpCount = 0;
	}

	if( (! ButtonFlg[0]) && ButtonON[0] ) {
		ButtonFlg[0] = true;
		OnDecided();
	}

	if( FOwner->Joy.StickX() == 0 ) StickFlgX = false;
	if( FOwner->Joy.StickY() == 0 ) StickFlgY = false;

	if( ActFlg[0] == 1 ) {

		if( (FOwner->Joy.StickY() == 1) ) {
			KeyDownCount++;
			if( (StickFlgY == false) || (KeyDownCount > 30) ) {    //新規に押下するか、60フレーム押し続ければ処理を有効
				if( (KeyDownCount > 30) ) KeyDownCount  =  25;  //キー押し続けの場合、カーソル移動が早くなりすぎるのを防ぐ

				Choice++;
				if( Choice >= CursorList->GetSize() ) Choice  =  0;

				CursorMove(Choice);
				if( CreateSE ) FOwner->SoundEffect->WavPlay(WASelect,0);
				StickFlgY = true;
			}
		}
		else {
			KeyDownCount  =  0;
		}

		if( (FOwner->Joy.StickY() == -1) ) {
			KeyUpCount++;
			if( (StickFlgY == false) || (KeyUpCount > 30)  ) {
				if( (KeyUpCount > 30) ) KeyUpCount  =  25;

				Choice--;
				if( Choice<0 ) Choice  =  CursorList->GetSize()-1;

				CursorMove(Choice);
				if( CreateSE ) FOwner->SoundEffect->WavPlay(WASelect,0);
				StickFlgY = true;
			}
		}
		else {
			KeyUpCount  =  0;
		}
	}
}

void TTitle2::OnDecided()
{
	s32 NameSum = 0;
	switch( ActFlg[0] ){
	case 0:
		{
			MakeItems();
			ActFlg[0]++;
			Sprite[0]->SetDest(0,-48,16);
			Sprite[1]->SetDest(0,448,16);

			copyright->Sprite[0]->Move(700,0);
			versionSpr->Sprite[0]->Move(700,0);

			if( CreateBGM ) FOwner->MusicPlayer->FadeIn(3, 1.0, 2.0);
		}break;
	case 1:
		{
			switch( Choice ){
			case 0:{//ゲームスタート
				for( u32 i = 0; i <= 7; i++ )
					NameSum += FOwner->PlayerName[i];
				//ネット接続確認画面
				if( (NameSum=47*8) && (! TrialMode) ) 
					FOwner->Add( new TConnectAsk(FOwner));
				else {
					FOwner->Add( new TTutorial(FOwner));
				}
				if( CreateSE ) FOwner->SoundEffect->WavPlay(WADeside,1);
				   }break;
			case 1:{//リプレイ
				if( TrialMode ) return;   //体験版は使用不可

				FOwner->Add( new TLocalReplayViewer(FOwner));
				if( CreateSE ) FOwner->SoundEffect->WavPlay(WADeside,1);
				   }break;
			case 2:{  //コンフィグ
				FOwner->Add( new TConfig2(FOwner));
				if( CreateSE ) FOwner->SoundEffect->WavPlay(WADeside,1);
				   }break;
			case 3:{  //ランキング
				FOwner->Add( new TLocalRanking(FOwner));
				if( CreateSE ) FOwner->SoundEffect->WavPlay(WADeside,1);
				   }break;
			case 4:{  //ネットランキング
				if( TrialMode ) return;   //体験版は使用不可

				FOwner->Add( new TNetRankingReplayViewer(FOwner));
				if( CreateSE ) FOwner->SoundEffect->WavPlay(WADeside,1);
				   }break;
			case 5:{  //ネームエントリー
				FOwner->Add( new TNameEntry2(FOwner,false));
				if( CreateSE ) FOwner->SoundEffect->WavPlay(WADeside,1);
				   }break;
			case 6:{  //終了
				Application.Terminate;
				   }break;
			}
			ActFlg[0]++;
			CursorList->ClearCursors(-320,FirstY);
			Sprite[0]->SetDest(0,-800,16);
			Sprite[1]->SetDest(0,600,16);
			Sprite[CursorSpr]->GetSpr()->FVisible = false;
		}
	}
}

void TTitle2::MakeItems()
{
	s32 Col;
	if( TrialMode )
		Col  =  32;
	else
		Col  =  255;

	//選択項目
	CursorList->SetAppearPoint(-320,0);
	CursorList->MakeCursor("Game Start", -16, FirstY, -2, 224, 0, KFONTKIND_ORANGE, GLASSCURSOR_PATTERN_SHADOW, KCURSOR_POS_LEFT);
	CursorList->MakeCursor_Base("Replay", -16, FirstY + Ybrank, -2, 224, 1, KFONTKIND_ORANGE, GLASSCURSOR_PATTERN_SHADOW, KCURSOR_POS_LEFT, Col, Col, Col, 255);
	CursorList->MakeCursor("Config", -16, FirstY + Ybrank*2, -2, 224, 2, KFONTKIND_ORANGE, GLASSCURSOR_PATTERN_SHADOW, KCURSOR_POS_LEFT);
	CursorList->MakeCursor("Ranking", -16, FirstY + Ybrank*3, -2, 224, 3, KFONTKIND_ORANGE, GLASSCURSOR_PATTERN_SHADOW, KCURSOR_POS_LEFT);
	CursorList->MakeCursor_Base("Netranking", -16, FirstY + Ybrank*4, -2, 224, 4, KFONTKIND_ORANGE, GLASSCURSOR_PATTERN_SHADOW, KCURSOR_POS_LEFT, Col, Col, Col, 255);
	CursorList->MakeCursor("Name entry", -16, FirstY + Ybrank*5, -2, 224, 5, KFONTKIND_ORANGE, GLASSCURSOR_PATTERN_SHADOW, KCURSOR_POS_LEFT);
	CursorList->MakeCursor("Quit", -16, FirstY + Ybrank*6, -2, 224, 6, KFONTKIND_ORANGE, GLASSCURSOR_PATTERN_SHADOW, KCURSOR_POS_LEFT);

}

void TTitle2::CursorMove( s32 _num )
{
	Sprite[CursorSpr]->Move(-16,FirstY + Ybrank * _num);
	CursorZoom = 0.01f;
}

TTitle2::TTitle2( TOBJList* owner ) : TOBJ(owner)
{
	FObjectName = "TTitle2";
	FKind = kGauge;
	Choice = 0;
	StickFlgX = true;
	StickFlgY = true;
	for( u32 i = 0; i <= 9; i++ ) {
		ActFlg[i] = 0;
		ActCount[i] = 0;
	}
	ButtonUpCount = 0;

	CursorList = new TCursorList(FOwner);

	//黒シート（上）
	MakeSpriteLite(FOwner->GetDG(), numSpr,0,0,640,256,150,bmNormal);
	Sprite[numSpr-1]->GetSpr()->AddTexture(Eff1Tex, 17, 1, 31, 15);
	Sprite[numSpr-1]->GetSpr()->SetColor(0,ctRed);
	Sprite[numSpr-1]->GetSpr()->SetColor(0,ctBlue);
	Sprite[numSpr-1]->GetSpr()->SetColor(0,ctGreen);

	//黒シート（下）
	MakeSpriteLite(FOwner->GetDG(), numSpr,0,432,640,64,150,bmNormal);
	Sprite[numSpr-1]->GetSpr()->AddTexture(Eff1Tex, 17, 1, 31, 15);
	Sprite[numSpr-1]->GetSpr()->SetColor(0,ctRed);
	Sprite[numSpr-1]->GetSpr()->SetColor(0,ctBlue);
	Sprite[numSpr-1]->GetSpr()->SetColor(0,ctGreen);

	//タイトル
	MakeSpriteLite(FOwner->GetDG(), numSpr,0,224,240,32,140,bmNormal);
	Sprite[numSpr-1]->GetSpr()->AddTexture(GaugeTex, 129, 337, 368, 368);
	Sprite[0]->Add(Sprite[numSpr-1]);

	//タイトル2
	MakeSpriteLite(FOwner->GetDG(), numSpr,240,224,224,32,140,bmNormal);
	Sprite[numSpr-1]->GetSpr()->AddTexture(GaugeTex, 129, 369, 351, 400);
	Sprite[0]->Add(Sprite[numSpr-1]);

	//選択項目の背景
	MakeSpriteLite(FOwner->GetDG(), numSpr,189,245,370,320,210,bmNormal);
	Sprite[numSpr-1]->GetSpr()->AddTexture(DemoTex, 81, 129, 255, 255);
	Sprite[numSpr-1]->GetSpr()->Rotate(1024);
	Sprite[numSpr-1]->GetSpr()->SetColor(160,ctAlpha);
	ShadowZoom = 0.01f;
	Sprite[numSpr-1]->GetSpr()->Zoom(1,ShadowZoom);
	ShadowSpr = numSpr-1;
	Sprite[0]->Add(Sprite[numSpr-1]);

	//カーソル
	MakeSpriteLite(FOwner->GetDG(), numSpr,-16,224,320,32,205,bmNormal);
	Sprite[numSpr-1]->GetSpr()->AddTexture(GaugeTex, 161, 417, 320, 448);
	CursorZoom = 0.01f;
	Sprite[numSpr-1]->GetSpr()->Zoom(CursorZoom,1);
	CursorSpr = numSpr-1;

	copyright =new TSprSentence(FOwner, "(c)2008-2010 Marsbound",276, 432, -2, KFONTKIND_ORANGE);
	FOwner->Add(copyright);

	if( TrialMode )
		versionSpr =new TSprSentence(FOwner, "Trial version",420, 448, -2, KFONTKIND_ORANGE);
	else
		versionSpr =new TSprSentence(FOwner, "Release no."+ InttoStr(Version),422, 448, -2, KFONTKIND_ORANGE);

	FOwner->Add(versionSpr);

	FOwner->Add( new TBoxContrl(FOwner,3));
}

TTitle2::~TTitle2()
{
	SAFE_DELETE(CursorList);
}

void TTitle2::Move()
{
	TOBJ::Move();
	if( ActFlg[0]<3 ) Input();

	switch( ActFlg[0] ){
		//初期画面
	case 0:
		{
			//      Inc(ButtonUpCount);
			//
			//      if( ButtonUpCount>1200 ) {
			//        ActFlg[0] = 3;
			//        ActCount[0] = 0;
			//        if( CreateBGM ) {
			//          FOwner->MusicPlayer->FadeOut(3,0.0,0.5);
			//          FOwner->MusicPlayer->FadeOut(2,0.0,0.5);
			//        }
			//      }
		}break;
		//メニュー出現中
	case 1:
		{
			Sprite[0]->AutoSlide();
			Sprite[1]->AutoSlide();

			if( ShadowZoom < 1 ) {
				ShadowZoom = ShadowZoom+0.08f;
				Sprite[ShadowSpr]->Zoom(1.f, ShadowZoom);
			}

			if( CursorZoom < 1 ) {
				CursorZoom = CursorZoom+0.08f;
				Sprite[CursorSpr]->Zoom(CursorZoom, 1.f);
			}
		}break;
		//終了処理
	case 2:
		{
			Sprite[0]->AutoSlide();
			Sprite[1]->AutoSlide();
			if( ActCount[0] > 30 ) Die();

			ActCount[0]++;
		}break;
		//ムービーへ
	case 3:
		{
			FOwner->Add( new TBoxContrl(FOwner,2));

			if( ActCount[0]>90 ) {
				FOwner->AfterLoopEvent = MOpeningMovie;
				Die();
			}

			ActCount[0]++;

		}break;
	}
}

void TTitle2::Die()
{
	TOBJ::Die();
	copyright->Die();
	versionSpr->Die();
}

void TRecover::CalcRecover( s32 _val )
{
	Deposit += _val;

	while (Deposit>=HPNext ) {
		FOwner->HeroHP += HPMax / 100;
		FOwner->HeroHP  =  glm::min(HPMax, FOwner->HeroHP);

		HPNext = HPNext + HPNorma;
	}

	while (Deposit>=MSNext ) {
		FOwner->MShipHP += MSHPMax / 100;
		FOwner->MShipHP  =  glm::min(MSHPMax, FOwner->MShipHP);
		MSNext = MSNext + MSNorma;
	}

	while (Deposit>=MisNext ) {
		FOwner->RestMis++;
		FOwner->RestMis  =  glm::min(MissileStock, FOwner->RestMis);
		MisNext = MisNext + MisNorma;
	}
}

void TRecover::MakeItem( const string& _caption, const string& _mul, const string& _unit, s32 _score, s32 _num )
{
	static const s32 Pos_Top = 168;
	static const s32 Pos_YBlank = 26;
	static const s32 Pos_YBlank_half = 24;
	static const s32 Cursor_Width = 392;
	static const s32 Pos_Left = 0;
	static const s32 Pos_Left_Point = 16;
	CursorList->MakeCursor(_caption, Pos_Left, Pos_Top + Pos_YBlank*_num*2, -4, Cursor_Width, _num, KFONTKIND_ORANGE, GLASSCURSOR_PATTERN_NONE, KCURSOR_POS_LEFT);
	CursorList->MakeCursor(_mul, Pos_Left, Pos_Top + Pos_YBlank*_num*2, -4, Cursor_Width, _num, KFONTKIND_ORANGE, GLASSCURSOR_PATTERN_BLACK_LEFT, KCURSOR_POS_RIGHT);
	CursorList->MakeCursor(InttoStr(_score) + _unit, Pos_Left_Point, Pos_Top + Pos_YBlank*_num*2+Pos_YBlank_half, 2, Cursor_Width, _num, KFONTKIND_BLIGHT, GLASSCURSOR_PATTERN_NONE, KCURSOR_POS_LEFT);

}

TRecover::TRecover( TOBJList* owner ) : TOBJ(owner)
{
	static const s32 hparray[3][4] = {{200000, 400000, 700000, 1000000}, {300000, 600000, 1000000, 1500000}, {600000, 1000000, 1500000, 2000000}};
	static const s32 msarray[3][4] = {{200000, 400000, 700000, 1000000}, {300000, 600000, 1000000, 1500000}, {600000, 1000000, 1500000, 2000000}};
	static const s32 misarray[3][4] = {{1500000, 3000000, 5000000, 7000000}, {2500000, 5000000, 8000000, 10000000}, {3000000, 6000000, 9000000, 12000000}};

	FObjectName = "TRecover";
	FOwner->GameMode = gOther;
	FOwner->ResultExists = true;
	FOwner->IsRecovering = true;
	FKind = kGauge;
	tempScore = FOwner->TotalScore;
	Deposit = 0;
	HPNorma = hparray[FOwner->Difficulty][FOwner->NowStage];
	MSNorma = msarray[FOwner->Difficulty][FOwner->NowStage];
	MisNorma = misarray[FOwner->Difficulty][FOwner->NowStage];
	HPNext = HPNorma;
	MSNext = MSNorma;
	MisNext = MisNorma;

	CursorList = new TCursorList(FOwner);

	for( u32 i = 0; i <= 7; i++ )
		ButtonFlg[i] = true;

	CursorList->SetAppearPoint(640,0);
	CursorList->MakeCursor("recovery", 168, 92, 8, 256, 0, KFONTKIND_NORMAL, GLASSCURSOR_PATTERN_GLASS, KCURSOR_POS_CENTER);

	CursorList->SetAppearPoint(0,0);   
	CursorList->MakeCursor("your score", 120, 380, -4, 520, 0, KFONTKIND_ORANGE, GLASSCURSOR_PATTERN_BLACK_RIGHT, KCURSOR_POS_RIGHT);
	((TMojiAndGlassCursor*)CursorList->GetItem(1))->SetAlpha(254);

	CursorList->SetAppearPoint(-640,0);
	MakeItem("shield", InttoStr(HPNorma) + "/ %", " percent", glm::min(50, FOwner->TotalScore / HPNorma),0);
	MakeItem("mothership", InttoStr(MSNorma) + " / %", " percent",  glm::min(50, FOwner->TotalScore / MSNorma),1);
	MakeItem("missile", InttoStr(MisNorma) + " / set", " set",  glm::min(MissileStock, FOwner->TotalScore / MisNorma),2);

	FOwner->Add( new THPGauge2(FOwner,392,174,1));
	FOwner->Add( new TMSGauge2(FOwner,392,228,1));
	FOwner->Add( new TMiSGauge2(FOwner,392,276,1));

	ScoreSprite  = new TGothic_counter(FOwner, 112,406,1000,9,false);
	ScoreSprite->Update(FOwner->TotalScore);
	FOwner->Add(ScoreSprite);
}

TRecover::~TRecover()
{
	SAFE_DELETE(CursorList);
	FOwner->IsRecovering = false;
}

void TRecover::Move()
{
	TOBJ::Move();
	Input();

	//数値カウントダウン
	if( tempScore > 0 ) {
		s32 count = 16666;
		if( count > tempScore ) count = tempScore;
		tempScore -= count;

		CalcRecover(count);
	}

	//表示数値更新
	ScoreSprite->Update(tempScore);
}

void TRecover::Die()
{
	TOBJ::Die();
	ScoreSprite->Die();
}

void TRecover::Input()
{
	for( u32 i = 0; i <= 7; i++ )
		if( ! ButtonON[i] ) ButtonFlg[i] = false;

	for( u32 i = 0; i <= 7; i++ ) {
		if( (! ButtonFlg[i]) && ButtonON[i] ) {
			ButtonFlg[i] = true;
			if( tempScore>0 ) {
				//カウントダウンスキップ処理
				CalcRecover(tempScore);
				tempScore = 0;
			}
			else {
				Die();
				//フェードアウト処理
				//2面だけは直後に潜水艦デモがあるのでそのまま終了
				if( FOwner->NowStage=1 )
					FOwner->StageEnd();
				else
					FOwner->Add( new TFadeOuttoNextStage(FOwner));
			}
			break;
		}
	}
}

TFadeOuttoNextStage::TFadeOuttoNextStage( TOBJList* owner ) : TOBJ(owner)
{
	FObjectName = "TFadeOuttoNextStage";
	FKind = kGauge;
	//ブラックアウト
	MakeSpriteLite(FOwner->GetDG(), 0,0,0,640,480,0,bmNormal);
	Sprite[0]->GetSpr()->AddTexture(Eff1Tex, 17, 1, 31, 15);
	Sprite[0]->GetSpr()->SetColor(0, ctRed);
	Sprite[0]->GetSpr()->SetColor(0, ctBlue);
	Sprite[0]->GetSpr()->SetColor(0, ctGreen);
	Sprite[0]->GetSpr()->SetColor(0, ctAlpha);
}

void TFadeOuttoNextStage::Move()
{
	TOBJ::Move();
	Sprite[0]->GetSpr()->SetColorR(2,ctAlpha);

	//真っ暗になるまで何もしない
	if( Age == 180 )  {
		FOwner->StageEnd();
	}
}

void TDifficultySelect::OnDecided()
{
	switch( Cursor->GetCurrentPos() ){
	case 0:FOwner->Difficulty = KDIFFICULTY_EASY;		break;
	case 1:FOwner->Difficulty = KDIFFICULTY_NORMAL;		break;
	case 2:{
		if( (FOwner->IsNormalCleared) )
			FOwner->Difficulty = KDIFFICULTY_HARD;
			//      else
			//        FOwner->Difficulty = KDIFFICULTY_SINGLE;
		   }		break;
		//    3:FOwner->Difficulty = KDIFFICULTY_SINGLE;
	}

	FOwner->Add( new TBoxContrl(FOwner,7));
	FOwner->Add( new TWeaponSelect(FOwner));
	if( CreateSE ) FOwner->SoundEffect->WavPlay(WADeside,1);

	Die();
}

void TDifficultySelect::Input()
{
	for( u32 i = 0; i <= 7; i++ )
		if( ! ButtonON[i] ) ButtonFlg[i] = false;

	if( FOwner->Joy.StickY() == 0 ) StickFlg = false;

	if( (! ButtonFlg[0]) && ButtonON[0] ) {
		ButtonFlg[0] = true;
		OnDecided();
	}

	if( (! ButtonFlg[1]) && ButtonON[1] ) {
		ButtonFlg[1] = true;
		FOwner->AfterLoopEvent = MTitle;
		Die();
	}

	if( (StickFlg == false) && (FOwner->Joy.StickY() == 1) ) {
		if( Cursor  !=  NULL ) Cursor->NextPoint(1);
		StickFlg = true;
		if( CreateSE ) FOwner->SoundEffect->WavPlay(WASelect,0);
	}

	if( (StickFlg == false) && (FOwner->Joy.StickY() == -1) ) {
		if( Cursor  !=  NULL ) Cursor->NextPoint(-1);
		StickFlg = true;
		if( CreateSE ) FOwner->SoundEffect->WavPlay(WASelect,0);
	}
}

TDifficultySelect::TDifficultySelect( TOBJList* owner ) : TOBJ(owner)
{
	FObjectName = "TDifficultySelect";
	FOwner->GameMode = gTitle;
	FKind = kGauge;
	StickFlg = false;

	CursorList = new TCursorList(FOwner);

	//カーソル生成
	Cursor  = new TGlassCursor(FOwner,160, 192, 288);
	FOwner->Add(Cursor);

	//画面作成
	CursorList->SetAppearPoint(-640,0);

	CursorList->MakeCursor("select difficulty",  0, 120, 1, 640, 0, KFONTKIND_BLIGHT, GLASSCURSOR_PATTERN_NONE, KCURSOR_POS_CENTER);
	CursorList->MakeCursor("",  32, 152, 1, 576, 1, KFONTKIND_NORMAL, GLASSCURSOR_PATTERN_LINE, KCURSOR_POS_CENTER);

	CursorList->MakeCursor("easy",  160, 192, 16, 288, 3, KFONTKIND_NORMAL, GLASSCURSOR_PATTERN_GLASS, KCURSOR_POS_CENTER);
	Cursor->SetMovePoint(160,192);
	CursorList->MakeCursor("normal", 160, 240, 16, 288, 2, KFONTKIND_NORMAL, GLASSCURSOR_PATTERN_GLASS, KCURSOR_POS_CENTER);
	Cursor->SetMovePoint(160,240);
	if( (FOwner->IsNormalCleared) ) {
		CursorList->MakeCursor("hard", 160, 288, 16, 288, 2, KFONTKIND_NORMAL, GLASSCURSOR_PATTERN_GLASS, KCURSOR_POS_CENTER);
		Cursor->SetMovePoint(160,288);
	}
	//  CursorList->MakeCursor("single level / old", 160, 336, -3, 288, 2, KFONTKIND_NORMAL, GLASSCURSOR_PATTERN_GLASS, KCURSOR_POS_CENTER);
	//  Cursor->SetMovePoint(160,336);

	//デフォルトはNormal
	Cursor->NextPoint(1);

	//スター
	for( u32 i = 0; i <= 2; i++ ) {
		if( FOwner->IsCleared[i][0] || FOwner->IsCleared[i][1] ) {
			MakeSpriteLite(FOwner->GetDG(), numSpr,0,0,48,48,99,bmNormal);
			if( FOwner->IsCleared[i, 0] && FOwner->IsCleared[i, 1] )
				Sprite[numSpr-1]->GetSpr()->AddTexture(DemoTex, 257, 1, 304, 48);
			else if( FOwner->IsCleared[i][0] )
				Sprite[numSpr-1]->GetSpr()->AddTexture(DemoTex, 257, 49, 304, 96);
			else if( FOwner->IsCleared[i][1] )
				Sprite[numSpr-1]->GetSpr()->AddTexture(DemoTex, 257, 97, 304, 144);
			Sprite[numSpr-1]->GetSpr()->FCX += 24;
			Sprite[numSpr-1]->GetSpr()->FCY += 24;
			Sprite[numSpr-1]->GetSpr()->Move(444, 208+48*i);
			Sprite[numSpr-1]->GetSpr()->SetColor(0, ctAlpha);
		}
	}
}

TDifficultySelect::~TDifficultySelect()
{
	SAFE_DELETE(CursorList);
	Cursor->Die();
}

void TDifficultySelect::Move()
{
	TOBJ::Move();
	// 帯表示
	FOwner->Add( new TBox(FOwner,0,0,640,32,0,200,255));
	FOwner->Add( new TBox(FOwner,0,448,640,480,0,200,255));

	for( s32 i  =  0 ; i < numSpr; i++ )
		Sprite[i]->GetSpr()->SetColorR(8, ctAlpha);

	if( Age>30 ) Input();
}

void TDifficultySelect::Die()
{
	TOBJ::Die();

}
