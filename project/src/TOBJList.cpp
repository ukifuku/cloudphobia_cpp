#include "TOBJList.h"
#include "TOBJ.h"
#include "SpriteEX.h"
#include "SpriteEXM.h"
#include "SpriteEXU.h"
#include "SpriteEXC.h"
#include "BG.h"
#include "BG2.h"
#include "QDArc.h"
#include <algorithm>
#include <direct.h>
#include "Sound.h"
#include "SXScene.h"
#include "BG3D.h"

#define ITERATE  for( objlist_itr itr = m_objList.begin(); itr != m_objList.end(); ++itr ) 

void TOBJList::Event()
{
	if( (WS.EventCount<=WS.EventMaxCount) && (Distance>=WS.EventDistance[WS.EventCount]) ) {
		switch( WS.EnemyType[WS.EventCount] ) {
		case 1:Add( new TWarningGauge2(this,WS.X[WS.EventCount],WS.Y[WS.EventCount],WS.OPT1[WS.EventCount],WS.OPT2[WS.EventCount]));  break;//Warning�Q�[�W
		case 2:Add( new TZ01Gene(this,WS.X[WS.EventCount],WS.Y[WS.EventCount],WS.OPT1[WS.EventCount],WS.FLG1[WS.EventCount]));   break;      //�U�R�P
		case 3:Add( new TZ02Gene(this,WS.X[WS.EventCount],WS.Y[WS.EventCount],WS.OPT1[WS.EventCount]));                          break;      //�U�R�Q
		case 4:Add( new TZ03Gene(this,WS.X[WS.EventCount],WS.Y[WS.EventCount],WS.OPT1[WS.EventCount]));                          break;     //�U�R�R
		case 5:Add( new TZ04Gene(this,WS.OPT1[WS.EventCount],WS.OPT2[WS.EventCount]));                                          break;   //�U�R�S
		case 6:Add( new TZ04Warn(this,WS.X[WS.EventCount],WS.Y[WS.EventCount],WS.FLG1[WS.EventCount]));                         break;    //�U�R�S�x��
		case 7:Add( new TZako05(this,WS.X[WS.EventCount],WS.Y[WS.EventCount],WS.OPT1[WS.EventCount],WS.OPT2[WS.EventCount],WS.FLG1[WS.EventCount]));   break; //�U�R�T
		case 8:Add( new TVScroll(this));                                                                                        break;       //���ړ�
		case 9:Add( new TDownScroll(this));                                                                                     break;    //���~
		case 10:Add( new TBoss(this,WS.X[WS.EventCount],WS.Y[WS.EventCount]));                                                  break;      //�{�X
		}
		WS.EventCount++;
	}
}

void TOBJList::EventReload()
{
	EWaveLoad(&WS_Seq[0], "s1wave_e.txt");
	EWaveLoad(&WS_Seq[1], "s1wave_n.txt");
	EWaveLoad(&WS_Seq[2], "s1wave_h.txt");
	EWaveLoad(&WS_Seq[3], "s1wave.txt");

	WS  =  WS_Seq[Difficulty];

}

void TOBJList::Setup()
{
	EWaveReset(&WS);
	for( s32 i = 0 ; i <= 3 ; i++)
		EWaveReset(&WS_Seq[i]);

	//�R���t�B�O�f�[�^�ǂݍ���
	ConfigLoad();

	//�O���t�B�b�N�f�[�^�ǂݍ���
	string FName  =  "./Data/Ich.BIN";

	Eff1Tex->LoadFromFile("Effects1.bmp", FName);
	Eff2Tex->LoadFromFile("Effects2.bmp", FName);
	Eff4Tex->LoadFromFile("Effects4.bmp", FName);
	Eff5Tex->LoadFromFile("Effects5.bmp", FName);
	Eff8Tex->LoadFromFile("Effects9.bmp", FName);
	Eff9Tex->LoadFromFile("Effects10.bmp", FName);
	JikiTex->LoadFromFile("jiki.bmp", FName);
	FontTex->LoadFromFile("font.bmp", FName);
	FontTex_Gothic->LoadFromFile("font_gothic.bmp", FName);
	GaugeTex->LoadFromFile("gauge.bmp", FName);

	//�X�v���C�g�ݒ�f�[�^�ǂݍ���
	SpriteDataList.clear();
	MotionDataList.clear();
	LoadSpriteData("stage2boss1.spr");
	LoadMotionData("stage2boss1.mot");

	LoadSpriteData("stage2guard.spr");
	LoadMotionData("stage2guard.mot");

	LoadSpriteData("guardbws.spr");
	LoadSpriteData("stage2boss2.spr");

	LoadSpriteData("Spider.spr");
	LoadMotionData("Spider.mot");

	LoadSpriteData("stage3boss.spr");
	LoadMotionData("stage3boss.mot");

	LoadSpriteData("stage4boss2.spr");
	LoadMotionData("stage4boss2.mot");

	LoadSpriteData("stage4guard.spr");

	LoadSpriteData("stage5boss1.spr");
	LoadMotionData("stage5boss1.mot");

	LoadSpriteData("stage5boss2.spr");
	LoadMotionData("stage5boss2.mot");

	LoadSpriteData("stage5boss3.spr");
	LoadMotionData("stage5boss3.mot");

	LoadSpriteData("friend.spr");
	LoadMotionData("friend.mot");

	LoadSpriteData("stage5boss2_2.spr");
	LoadMotionData("stage5boss2_2.mot");

	m_SprMan->FSmooth  =  FilterON;         // �X���[�X�t�B���^
	m_SprMan->FVisible  =  true;
	m_SprMan->FClearColor  =  0xFF0014A5;
	//m_SprMan->ClearColor  =  0xFF00FF00;

	//�X�R�A�f�[�^�ǂݍ���
	ifstream ifs("Score_029.BIN");
	if( ifs ) {
		s8		readversion;
		ifs.read(  (char*)&readversion, sizeof(s8) );

		if( readversion == 29 ) {
			for( s32 l = 0 ; l < MAXSTAGE_SEQ; l++ ) {
				for( s32 i = 0 ; i <= 9; i++ ) {
					ifs.read(  (char*)&ScoreRanking_Seq_local[l][i], sizeof(NetRankingData) );
				}
				for( s32 i = 0 ; i <= 9; i++ ) {
					ifs.read(  (char*)&TimeRanking_Seq_local[l][i], sizeof(NetRankingData) );
				}
				for( s32 i = 0 ; i <= 9; i++ ) {
					ifs.read(  (char*)&ScoreRanking_Split_local[l][i], sizeof(NetRankingData) );
				}
				for( s32 i = 0 ; i <= 9; i++ ) {
					ifs.read(  (char*)&TimeRanking_Split_local[l][i], sizeof(NetRankingData) );
				}

				for( s32 i = 0 ; i <= 9; i++ )
					ifs.read(  (char*)&IsCleared[l][i], sizeof(bool) );
			}
		}

		ifs.close();
	}

	//BGM�Z�b�g
	if( CreateBGM ) {
		MusicPlayer = new TMusicPlayer();
		MusicPlayer->BGMChange(0,BGMNumber);
	}

	//SE�Z�b�g
	//if( CreateSE ) {
	SoundEffect = new TSoundEffect();
	//}

	//�X�e�[�W�ǂݍ���
	if( DebugMode && (NowStage != 99))
		LoadStage(NowStage, MPlayStart);
	else
		LoadStage(NowStage, MOpeningMovie);

}

void TOBJList::Hantei()
{
	vector<TOBJ*> objHero,objShot,objEnemy,objEShot,objMissile,objColBG;

	//�����蔻��
	ITERATE {
		switch( (*itr)->Kind() ) {
		case kHero: { objHero.push_back(*itr);   }	break;
		case kHShot: { objShot.push_back(*itr);   }	break;
		case kEnemy:
		case kEMissile: { objEnemy.push_back(*itr);  }	break;
		case kEShot: { objEShot.push_back(*itr);  }	break;
		case kMissile: { objMissile.push_back(*itr);  }	break;
		case kColBG: { objColBG.push_back(*itr); }	break;
		}
	}

	TOBJ*	xMatrix;
	TOBJ*	yMatrix;

	//�V���b�g�ƓG
	for( s32 i = 0 ; i < objShot.size(); i++ ) {
		xMatrix = objShot[i];
		for( s32 j = 0 ; j < objEnemy.size(); j++ ) {
			yMatrix = objEnemy[j];
			if( yMatrix->FDead )
				continue;

			if( xMatrix->CheckTouch(yMatrix) ) {
				xMatrix->OnDamaged(yMatrix);
				yMatrix->OnDamaged(xMatrix);
			}

			//if( xMatrix->FDead )           //�d�Ȃ����G���ꔭ�œ|����悤��
			//  break;
		}
	}


	//���@�ƓG
	// if( (! Muteki) and (CollisionON) ) {
	for( s32 i = 0 ; i < objHero.size(); i++ ) {
		xMatrix = objHero[i];
		for( s32 j = 0 ; j < objEnemy.size(); j++ ) {
			yMatrix = objEnemy[j];
			if( yMatrix->FDead )
				continue;

			if( xMatrix->CheckTouch(yMatrix) ) {
				xMatrix->OnDamaged(yMatrix);
				yMatrix->OnDamaged(xMatrix);
			}

			if( xMatrix->FDead )
				break;
		}
		// }
	}

	//���@�ƓG�e
	if( (! Muteki) && (CollisionON || ReplayON) ) {
		for( s32 i = 0 ; i < objHero.size(); i++ ) {
			xMatrix = objHero[i];
			for( s32 j = 0 ; j < objEShot.size(); j++) {
				yMatrix = objEShot[j];
				if( yMatrix->FDead )
					continue;

				if( xMatrix->CheckTouch(yMatrix) ) {
					xMatrix->OnDamaged(yMatrix);
					yMatrix->OnDamaged(xMatrix);
				}

				if( xMatrix->FDead )
					break;
			}
		}
	}

	//�~�T�C���ƓG
	for( s32 i = 0 ; i < objMissile.size(); i++ ) {
		xMatrix = objMissile[i];
		for( s32 j = 0 ; j < objEnemy.size(); j++ ) {
			yMatrix = objEnemy[j];
			if( yMatrix->FDead )
				continue;

			if( xMatrix->CheckTouch(yMatrix) && (LockID[xMatrix->Locknum]==yMatrix->ID()) ) {
				xMatrix->OnDamaged(yMatrix);
				yMatrix->OnDamaged(xMatrix);
			}

			if( xMatrix->FDead )
				break;
		}
	}


	//�����蔻����������w�i�Ǝ��@
	for( s32 i = 0 ; i < objHero.size(); i++) {
		xMatrix = objHero[i];
		for( s32 j = 0 ; j < objColBG.size(); j++ ) {
			yMatrix = objColBG[j];
			if( yMatrix->FDead )
				continue;

			if( xMatrix->CheckTouch(yMatrix) ) {
				xMatrix->OnDamaged(yMatrix);
				yMatrix->OnDamaged(xMatrix);
			}

			if( xMatrix->FDead )
				break;
		}
	}

	//�����蔻����������w�i�ƃV���b�g
	for( s32 i = 0 ; i < objShot.size(); i++ ) {
		xMatrix = objShot[i];
		for( s32 j = 0 ; j < objColBG.size(); j++ ) {
			yMatrix = objColBG[j];
			if( yMatrix->FDead )
				continue;

			if( xMatrix->CheckTouch(yMatrix) ) {
				xMatrix->OnDamaged(yMatrix);
				yMatrix->OnDamaged(xMatrix);
			}

			if( xMatrix->FDead )
				break;
		}
	}

	//�����蔻����������w�i�ƓG�V���b�g
	for( s32 i = 0 ; i < objEShot.size(); i++) {
		xMatrix = objEShot[i];
		for( s32 j = 0 ; j < objColBG.size(); j++ ) {
			yMatrix = objColBG[j];
			if( yMatrix->FDead )
				continue;

			if( xMatrix->CheckTouch(yMatrix) ) {
				xMatrix->OnDamaged(yMatrix);
				yMatrix->OnDamaged(xMatrix);
			}

			if( xMatrix->FDead )
				break;
		}
	}

	//�����蔻����������w�i�ƓG
	for( s32 i = 0 ; i < objEnemy.size(); i++ ) {
		xMatrix = objEnemy[i];
		for( s32 j = 0 ; j < objColBG.size(); j++ ) {
			yMatrix = objColBG[j];
			if( yMatrix->FDead )
				continue;

			if( xMatrix->CheckTouch(yMatrix) ) {
				xMatrix->OnDamaged(yMatrix);
				yMatrix->OnDamaged(xMatrix);
			}

			if( xMatrix->FDead )
				break;
		}
	}


	//���b�N�I���^�[�Q�b�g�ߑ�
	for( s32 i = 0 ; i < MaxLock*MissileStock; i++ ) {
		for( s32 j = 0 ; j < objEnemy.size(); j++ ) {
			yMatrix = objEnemy[j];
			if( (yMatrix->ID() == LockID[i]) ) {
				if( (yMatrix->FDead) || (yMatrix->HP<=0) )
					UnLock[i] = true;
				else 
				{
					BeLocked[i].X = yMatrix->X;
					BeLocked[i].Y = yMatrix->Y;
				}
			}
		}
	}

}

void TOBJList::SetStage( s8 StageNum )
{
	if( Blooming ) {
		BloomingFilter = new TBloomingBasic(this,0,0,640,480,1100);
		Add(BloomingFilter);
	}
	Add( new TAlphaEffectFilter(this));
	Add( new TAddEffectFilter(this));

	Add( new TCountDown(this, -50,100,0));
	Add( new TCountDown(this, -50,100,1));
	Add( new TCountDown(this, -50,100,2));

	switch(StageNum ) {
	case 0:{
		Add(new TSky2(this));  //�w�i
		Add(new TCloudMaker(this,0));
		Add(new TCloudMaker4(this,0));
		SAFE_DELETE(Enemy4Tex);
		SAFE_DELETE(BossBump);

		//�ʂ��ʃ��[�h�p�ɃX�R�A�EHP��������
		//1�ʊJ�n���̂ݏ��������āA�ȍ~�̖ʂ͈����p��
		InitPlayerGauge();
		   }break;
	case 1:{
		Add(new TSea(this,0,3));  //�C
		Add(new TUnderSea(this));  //
		Add(new TDawnCloud(this));  //�_
		CameraReset();
		m_Scene->SetFog(0x00ee3390, 5, 20);

		BG3DMan->ResetFrameList();

		Script->UnDone(KSCRIPT_STAGE2ENEMY);                                              
		Script->UnDone(KSCRIPT_STAGE2ENEMY_EASY);
		Script->UnDone(KSCRIPT_STAGE2ENEMY_NORMAL);
		Script->UnDone(KSCRIPT_STAGE2ENEMY_HARD);
		Script->UnDone(KSCRIPT_NIGHTVISION);
		SetBloomingColor(255,0,60,47,47);

		if( AfterLoopEvent!=MRestart ) {
			DemoMan  =  new TDemoManager(this, 0);
			Add(DemoMan);
			((TDemoManager*)DemoMan)->DemoStart();
			Script->UnDone(KSCRIPT_STAGE2DEMO);
			if( CreateBGM ) {
				MusicPlayer->Play(0);
			}
		}

		switch(Difficulty ) {
		case KDIFFICULTY_SINGLE: StageScriptID = KSCRIPT_STAGE2ENEMY;			break;
		case KDIFFICULTY_EASY  : StageScriptID = KSCRIPT_STAGE2ENEMY_EASY;		break;
		case KDIFFICULTY_NORMAL: StageScriptID = KSCRIPT_STAGE2ENEMY_NORMAL;	break;
		case KDIFFICULTY_HARD  : StageScriptID = KSCRIPT_STAGE2ENEMY_HARD;		break;
		}

		if( (DebugMode && (SequenceTotalTime == 0)) || IsPractice ) InitPlayerGauge();

		   }break;
	case 2:{
		BG3DMan->ResetFrameList();
		Script->UnDone(KSCRIPT_STAGE3ENEMY);
		Script->UnDone(KSCRIPT_STAGE3ENEMY_EASY);
		Script->UnDone(KSCRIPT_STAGE3ENEMY_NORMAL);
		Script->UnDone(KSCRIPT_STAGE3ENEMY_HARD);
		switch(Difficulty ) {
		case KDIFFICULTY_SINGLE: StageScriptID = KSCRIPT_STAGE3ENEMY;			break;
		case KDIFFICULTY_EASY  : StageScriptID = KSCRIPT_STAGE3ENEMY_EASY;		break;
		case KDIFFICULTY_NORMAL: StageScriptID = KSCRIPT_STAGE3ENEMY_NORMAL;	break;
		case KDIFFICULTY_HARD  : StageScriptID = KSCRIPT_STAGE3ENEMY_HARD;		break;
		}

		if( (DebugMode && (SequenceTotalTime == 0)) || IsPractice ) InitPlayerGauge();
		   }break;
	case 3:{
		BG3DMan->ResetFrameList;
		Script->UnDone(KSCRIPT_STAGE4ENEMY);
		Script->UnDone(KSCRIPT_STAGE4ENEMY_EASY);
		Script->UnDone(KSCRIPT_STAGE4ENEMY_NORMAL);
		Script->UnDone(KSCRIPT_STAGE4ENEMY_HARD);
		SetBloomingColor(255,0,47,47,47);

		switch(Difficulty ) {
		case KDIFFICULTY_SINGLE: StageScriptID = KSCRIPT_STAGE4ENEMY;			break;
		case KDIFFICULTY_EASY  : StageScriptID = KSCRIPT_STAGE4ENEMY_EASY;		break;
		case KDIFFICULTY_NORMAL: StageScriptID = KSCRIPT_STAGE4ENEMY_NORMAL;	break;
		case KDIFFICULTY_HARD  : StageScriptID = KSCRIPT_STAGE4ENEMY_HARD;		break;
		}

		if( (DebugMode && (SequenceTotalTime == 0)) || IsPractice ) InitPlayerGauge();
		   }break;
	case 4:{
		BG3DMan->ResetFrameList;
		Script->UnDone(KSCRIPT_STAGE5ENEMY);
		Add(new TSandStorm(this,0,0));
		m_Scene->SetFog(0xff9f4242, 2, 10);

		switch(Difficulty ) {
		case KDIFFICULTY_SINGLE: StageScriptID = KSCRIPT_STAGE5ENEMY;			break;
		case KDIFFICULTY_EASY  : StageScriptID = KSCRIPT_STAGE5ENEMY;			break;
		case KDIFFICULTY_NORMAL: StageScriptID = KSCRIPT_STAGE5ENEMY;			break;
		case KDIFFICULTY_HARD  : StageScriptID = KSCRIPT_STAGE5ENEMY;			break;
		}

		if( (DebugMode && (SequenceTotalTime == 0)) || IsPractice ) InitPlayerGauge();
		   }break;

	case 5:{ //�f�o�b�O�p�X�e�[�W
		//        Add(TSea.Create(this,0,3));  //�C
		//        Add(TUnderSea.Create(this));  //
		//        Add(TDawnCloud.Create(this));  //�_
		//        CameraReset;
		//        m_Scene->SetFog(0x00ee3390, 5, 20);

		//        BG3DMan->ResetFrameList;
		//        SetBloomingColor(255,0,60,47,47);
		//        SetBloomingColor(255,0,47,47,47);

		   }break;
	case 98:{
		DemoMan  = new TDemoManager(this, 1);
		Add(DemoMan);
		((TDemoManager*)DemoMan)->DemoStart;
		Script->UnDone(KSCRIPT_STAGE5DEMO);
		if( CreateBGM ) {
			MusicPlayer->PlayOnce(0);
		}
		InitPlayerGauge();
			}break;
	case 99:{
		BG3DMan->ResetFrameList;
		Add(new TSky_StageSelect(this));  //�w�i
		Add(new TCloudMaker(this,0));
		Add(new TCloudMaker4(this,0));
		Add(new TSandStorm(this,0,0));
		BG3DMan->Scroll(0);

		StageSelect = new TStageSelectScreenMng(this);
		Add(StageSelect);

		InitPlayerGauge();

		//        Add(TGlassCubeFilter.Create(this,0));
		//        AfterLoopEvent  =  MWeaponSelect;
			}break;
	}

	DontRender = true;  //�e�N�X�`�����J���������X�v���C�g�͓o�^����Ă���̂ŁA���̂܂ܕ`�悷��ƃG���[�ɂȂ�B
	//���̃t���[���͕`�悳���Ȃ��悤�ɂ���B

}

void TOBJList::LoadStage( s8 StageNum, TMainLoopEvent _event )
{
	AfterLoadingEvent = _event;

	//���񂾃I�u�W�F�N�g������
	ITERATE {
		if( (*itr)->FDead ) {
			SAFE_DELETE(*itr);
			m_objList.erase(itr);
			continue;
		}

		++itr;
	}

	if( NowStage == 99 )
		if( CreateBGM ) MusicPlayer->FadeOut(3,0.0,0.5);

	//�ǂݍ��ޑO�Ɍ��݂̃X�e�[�W�f�[�^���J��
	ReleaseStage();

	DontRender = true;

	DataLoaded[StageNum] = true;

	LoadingScreen = new TLoadingScreen(this);
	Add(LoadingScreen);

	//  LoadThread = TLoadStageThread.Create(this,StageNum);
	//  LoadThread.OnTerminate  =  OnLoaded;

}

void TOBJList::ReleaseStage()
{
	//���݂̃X�e�[�W�̃f�[�^�̂݊J������
	switch(NowStage ) {
	case 0:{
		//�e�N�X�`���̊J��
		SAFE_DELETE(Enemy4Tex);
		SAFE_DELETE(BossBump);
		SAFE_DELETE(BGTex1);
		SAFE_DELETE(BGTex2);
		SAFE_DELETE(MechaTex);
		SAFE_DELETE(BumpTex);
		SAFE_DELETE(Eff6Tex);
		SAFE_DELETE(Eff7Tex);
		SAFE_DELETE(SunTex);
		SAFE_DELETE(DemoTex);
		SAFE_DELETE(Eff6Tex);
		SAFE_DELETE(Enemy5Tex);
		SAFE_DELETE(BossBump);
		   }break;
	case 1:{
		//�e�N�X�`���̊J��
		SAFE_DELETE(BGTex1);
		SAFE_DELETE(BGTex2);
		SAFE_DELETE(ST2Tex1);
		SAFE_DELETE(ST2Tex2);
		SAFE_DELETE(MechaTex);
		SAFE_DELETE(Eff3Tex);
		SAFE_DELETE(Eff6Tex);
		SAFE_DELETE(Eff7Tex);
		SAFE_DELETE(Board);
		SAFE_DELETE(CloudMesh);
		SAFE_DELETE(Undersea);
		SAFE_DELETE(Enemy4Tex);
		SAFE_DELETE(DemoTex);

		   }break;
	case 2:{
		SAFE_DELETE(Eff6Tex);
		SAFE_DELETE(MissileTex);
		SAFE_DELETE(MissileMesh);
		SAFE_DELETE(MechaTex);
		   }break;
	case 3:{
		SAFE_DELETE(Eff3Tex);
		SAFE_DELETE(MechaTex);
		SAFE_DELETE(Enemy4Tex);
		   }break;
	case 4:{
		SAFE_DELETE(Eff6Tex);
		SAFE_DELETE(Eff7Tex);
		SAFE_DELETE(MechaTex);
		SAFE_DELETE(Enemy4Tex);
		SAFE_DELETE(Enemy5Tex);
		   }break;
	case 99:{
		SAFE_DELETE(BGTex1);
		SAFE_DELETE(BGTex2);
		SAFE_DELETE(MechaTex);
		SAFE_DELETE(BShotTex1);
		SAFE_DELETE(BShotTex2);
		SAFE_DELETE(BShotBumpTex1);
		SAFE_DELETE(BShotBumpTex2);
		SAFE_DELETE(Eff6Tex);
		SAFE_DELETE(Eff7Tex);
		SAFE_DELETE(Board);
		SAFE_DELETE(CloudMesh);
		SAFE_DELETE(Undersea);
		SAFE_DELETE(ST2Tex1);
		SAFE_DELETE(DemoTex);

		for( s32 i  =  0 ; i < NCube; i++ ) {
			SAFE_DELETE(GCube[i]);
		}
			}break;
	}

	if( BG3DMan != NULL )
		BG3DMan->Clear();

	DataLoaded[NowStage] = false;

}

void TOBJList::QuakeManager()
{
	s32 RandomTable[] = {-1,-1,0,1,1,0,-1,0,1,-1};
	if( QuakeFlg ) {
		m_SprMan->MoveR(RandomTable[FTick % 9] * QuakePower, RandomTable[9-(FTick % 9)] * QuakePower);
		if( ! TimeStop ){
			m_Scene->CameraRotateR(RandomTable[FTick % 9] * QuakePower,
				RandomTable[FTick % 9] * QuakePower,
				0);
		}
	}

	//��ʗh�炵�ŋ��������W��␳
	if( ((FTick-QuakeTime)>QuakeLife) && QuakeFlg ) {
		QuakeFlg = false;
		m_SprMan->Move(0,0);
		//    CameraAngleReset;
	}

	//�|�[�Y���͌��̍��W�ɖ߂�
	if( TimeStop ) {
		m_SprMan->Move(0,0);
		//    CameraAngleReset;
	}

}

void TOBJList::CameraReset()
{
	switch( NowStage ) {
	case 1:{
		m_Scene->CameraRotate(0,0,0);
		m_Scene->CameraMove(0,0,0);
		//    m_Scene->CameraRotate(570,0,0);
		//    m_Scene->CameraMove(0,-0.8,2.3);
		   }break;
	}

}

void TOBJList::CameraAngleReset()
{ 
	switch(NowStage ) {
	case 1:{
		m_Scene->CameraRotate(-128,0,0);
		   }
	}


}

void TOBJList::LoadSpriteData( const string& _file )
{
	TSpriteDataList data;
	std::string line;
	// �e�L�X�g�t�@�C���ǂݍ���
	if( DebugMode ) {
		std::ifstream ifs(DEBUG_GRAPH_PATH + _file);

		std::getline(ifs, line);
		data.TotalSpr  =  stoi(line);
		std::getline(ifs, data.BmpFile);

		for( s32 i = 0 ; i< data.TotalSpr; i++ ) {
			TSpriteData newSpr;

			std::getline(ifs, line);
			newSpr.Name  =  line;
			std::getline(ifs, line);
			newSpr.ParentID  =  stoi(line);
			std::getline(ifs, line);
			newSpr.TexRect.Left = stoi(line);
			std::getline(ifs, line);
			newSpr.TexRect.Top = stoi(line);
			std::getline(ifs, line);
			newSpr.TexRect.Right = stoi(line);
			std::getline(ifs, line);
			newSpr.TexRect.Bottom = stoi(line);
			for( s32 l  =  0 ; l <= 3; l++ ) {
				std::getline(ifs, line);
				newSpr.HitRect[l].X = stoi(line);
				std::getline(ifs, line);
				newSpr.HitRect[l].Y = stoi(line);
			}
			std::getline(ifs, line);
			newSpr.CX       =  stoi(line);
			std::getline(ifs, line);
			newSpr.CY       =  stoi(line);
			std::getline(ifs, line);
			newSpr.X        =  stoi(line);
			std::getline(ifs, line);
			newSpr.Y        =  stoi(line);
			std::getline(ifs, line);
			newSpr.Rdeg     =  stoi(line);
			std::getline(ifs, line);
			newSpr.Z     =  stoi(line);

			data.Sprites.push_back(newSpr);
		}
		SpriteDataList.push_back(data);
	}
	else {
		//���k�t�@�C������ǂݍ���
		std::stringstream stream;
		ExtractFromQDAFile(stream, "./Data/script.BIN", _file);

		std::getline(stream, line);
		data.TotalSpr  =  stoi(line);
		std::getline(stream, data.BmpFile);

		for( s32 i = 0 ; i< data.TotalSpr; i++ ) {
			TSpriteData newSpr;

			std::getline(stream, line);
			newSpr.Name  =  line;
			std::getline(stream, line);
			newSpr.ParentID  =  stoi(line);
			std::getline(stream, line);
			newSpr.TexRect.Left = stoi(line);
			std::getline(stream, line);
			newSpr.TexRect.Top = stoi(line);
			std::getline(stream, line);
			newSpr.TexRect.Right = stoi(line);
			std::getline(stream, line);
			newSpr.TexRect.Bottom = stoi(line);
			for( s32 l  =  0 ; l <= 3; l++ ) {
				std::getline(stream, line);
				newSpr.HitRect[l].X = stoi(line);
				std::getline(stream, line);
				newSpr.HitRect[l].Y = stoi(line);
			}
			std::getline(stream, line);
			newSpr.CX       =  stoi(line);
			std::getline(stream, line);
			newSpr.CY       =  stoi(line);
			std::getline(stream, line);
			newSpr.X        =  stoi(line);
			std::getline(stream, line);
			newSpr.Y        =  stoi(line);
			std::getline(stream, line);
			newSpr.Rdeg     =  stoi(line);
			std::getline(stream, line);
			newSpr.Z     =  stoi(line);

			data.Sprites.push_back(newSpr);
		}
		SpriteDataList.push_back(data);
		stream.str("");
	}


}

void TOBJList::LoadMotionData( const string& _file )
{
	s32 PrevMotionMax  =  MotionDataList.size();
	std::string line;
	if( DebugMode ) {
		// �e�L�X�g�t�@�C���ǂݍ���
		std::ifstream ifs(DEBUG_GRAPH_PATH + _file);

		std::getline(ifs, line);
		std::getline(ifs, line);
		s32 numData = stoi(line);

		for( s32 k  =  PrevMotionMax; k < numData; k++ ) {
			TMotionData data;

			std::getline(ifs, line);
			data.TotalSpr  =  stoi(line);

			std::getline(ifs, data.Name);

			for( s32 i = 0 ; i < data.TotalSpr; i++ ) {
				TPartsMotionData newSpr;
				std::getline(ifs, line);
				newSpr.Name  =  line;

				std::getline(ifs, line);
				newSpr.X        =  stoi(line);
				std::getline(ifs, line);
				newSpr.Y        =  stoi(line);
				std::getline(ifs, line);
				newSpr.Rdeg     =  stoi(line);
				std::getline(ifs, line);
				newSpr.Speed     =  stoi(line);
				std::getline(ifs, line);
				newSpr.Direction  =  stoi(line);

				data.Sprites.push_back(newSpr);
			}
			MotionDataList.push_back(data);
		}

		ifs.close();
	}
	else {

		//���k�t�@�C������ǂݍ���
		std::stringstream stream;
		ExtractFromQDAFile(stream, "./Data/sprites.BIN", _file);

		std::getline(stream, line);
		std::getline(stream, line);
		s32 numData = stoi(line);

		for( s32 k  =  PrevMotionMax; k < numData; k++ ) {
			TMotionData data;

			std::getline(stream, line);
			data.TotalSpr  =  stoi(line);

			std::getline(stream, data.Name);

			for( s32 i = 0 ; i < data.TotalSpr; i++ ) {
				TPartsMotionData newSpr;
				std::getline(stream, line);
				newSpr.Name  =  line;

				std::getline(stream, line);
				newSpr.X        =  stoi(line);
				std::getline(stream, line);
				newSpr.Y        =  stoi(line);
				std::getline(stream, line);
				newSpr.Rdeg     =  stoi(line);
				std::getline(stream, line);
				newSpr.Speed     =  stoi(line);
				std::getline(stream, line);
				newSpr.Direction  =  stoi(line);

				data.Sprites.push_back(newSpr);
			}
			MotionDataList.push_back(data);
		}
	}

}

void TOBJList::CreateRestart()
{
	switch( NowStage ) {
	case 0:{
		if( ! DebugMode ) {
			Add( new TReady(this,3000));
			GameMode = gReady;
		}

		InitBeforePlayStart();
		   }break;
	case 1:{
		GameMode = gReady;
		Add(new TStage2StartingDemo(this));
		   }break;
	case 2:{
		BG3DMan->ResetFrameList();
		GameMode = gReady;
		Add(new TStage3StartingDemo(this));

		   }break;
	case 3:{
		BG3DMan->ResetFrameList();
		Add(new TStage4StartingDemo(this));
		GameMode = gReady;
		   }break;
	case 4:{
		BG3DMan->ResetFrameList();
		GameMode = gReady;
		Add(new TStage5StartingDemo(this));
		   }break;
	case 99:{
		BG3DMan->ResetFrameList();
			}break;
	}

}

void TOBJList::InitPlayerGauge()
{
	switch(Difficulty ) {
	case KDIFFICULTY_SINGLE:{
		HPMax  =  HPMax_Single;
		MSHPMax = MSHPMax_Single;
		//�V���O�����[�h�̂ݖ���HP������
		MShipHP = MSHPMax;
		HeroHP = HPMax;
		RestMis = MissileStock;
							}break;
	case KDIFFICULTY_EASY:{
		HPMax  =  HPMax_Easy;
		MSHPMax = MSHPMax_Easy;
						  }break;
	case KDIFFICULTY_NORMAL:{
		HPMax  =  HPMax_Normal;
		MSHPMax = MSHPMax_Normal;
							}break;
	case KDIFFICULTY_HARD:{
		HPMax  =  HPMax_Hard;
		MSHPMax = MSHPMax_Hard;
						  }break;
	}

	TotalScore = 0;
	SequenceTotalTime = 0;
	MShipHP = MSHPMax;
	HeroHP = HPMax;
	RestMis = MissileStock;

}

void TOBJList::DeleteBullets()
{
	ITERATE {
		if( ((*itr)->Kind() == kEShot) && (! (*itr)->IsParentExists()) )
			(*itr)->Die();
	}

	IsRequestedDelBullets = false;

}

void TOBJList::OnAfterLoop()
{
	s32 nextstage;

	switch(AfterLoopEvent ) {
	case MLoadBeforePlay:
		{
			GameReset();
			TimeStop = false;
			if( (Difficulty == KDIFFICULTY_SINGLE) || (IsPractice) ) {
				nextstage  = ((TStageSelectScreenMng*)StageSelect)->GetCurrentStage();
			}
			else {
				switch(NowStage ) {
				case 99:nextstage = 0;     break; //��
				default:
					nextstage = NowStage + 1;
				}

				if( ReplayON ) Replayer->LoadTempReplay(nextstage);
			}
			LoadStage(nextstage, MLoadBeforePlay);
			NowStage = nextstage;
		}break;
	case MPlayStart:
		{
			if( CreateBGM ) MusicPlayer->Stop;
			GameStart();
			PlayStart();
			Add(new TBoxContrl(this,7));

			if( DebugMode ) {
				Replayer->debugload();
				TotalScore  =  Replayer->SplitData.SplitScore[2];
			}


			switch(NowStage ) {
			case 0:Add(new TStage1StartingDemo(this));		break;
				//        1:Add(TStage2StartingDemo.Create(this));
			case 2:Add(new TStage3StartingDemo(this));		break;
			case 3:Add(new TStage4StartingDemo(this));		break;
			case 4:Add(new TStage5StartingDemo(this));		break;
			}

		}break;
	case MRestart:
		{
			TimeStop = false;
			GameReset();
			GameStart();
			PlayStart();
			Add(new TBoxContrl(this,7));
			CreateRestart();
		}break;
	case MRestart_Seq:
		{
			TimeStop = false;
			GameReset();
			LoadStage(0, MRestart_Seq);
			NowStage = 0;
			if( ReplayON ) Replayer->LoadTempReplay(0);
		}break;
	case MTitle:
		{
			TimeStop = false;
			GameReset();
			LoadStage(99, MTitle);
			NowStage = 99;
		}break;
	case MPause:
		{
			TimeStop = true;
			GameMode = gPause;
			Add(new TPause2(this));
		}break;
	case MMove:
		{
			TimeStop = false;
			GameMode = gPlay;
		}break;
	case MRepView:
		{
			TimeStop = false;
			ReplayON = false;
			GameReset();

			LoadStage(99, MRepView);
			NowStage = 99;
		}break;
	case MWeaponSelect:
		{
			GameReset();
			GameStart();
			Add(new TDifficultySelect(this));
		}break;
	case MReady:
		{
			GameMode = gReady;
			InitBeforePlayStart();
			Add(new TReady(this,3000));
			DemoON = false;
		}break;
	case MOpeningMovie:
		{
			TimeStop = false;
			GameReset();
			LoadStage(99, MOpeningMovie);
			NowStage = 99;
		}break;
	case MEndingMovie:
		{
			TimeStop = false;
			ReplayON = false;
			GameReset();

			LoadStage(98, MEndingMovie);
			NowStage = 98;
		}break;
	}
	AfterLoopEvent = MNone;

}

void TOBJList::HUDVisibleControl()
{
	if( HUDHideFlg ) {
		//�L��������ʉ����ɂƂǂ܂��Ă���Ԃ̓J�E���^���Z�A��u�ł��オ��΃J�E���^���Z�b�g
		if( HeroPos.Y >= KHEROPOS_TOPGAUGE_HIDED )
			HUDHideCount++;
		else
			HUDHideCount = 0;

		//�J�E���^�����l�ȏ�ɂȂ�΃Q�[�W�o��
		if( HUDHideCount > 30 ) {
			HUDHideFlg  =  false;
			HUDHideCount = 0;
		}

	}
	else {
		//�L��������ʏ㕔�ɂƂǂ܂��Ă���Ԃ̓J�E���^���Z�A��u�ł�������΃J�E���^���Z�b�g
		if( HeroPos.Y < KHEROPOS_TOPGAUGE_HIDED )
			HUDHideCount++;
		else
			HUDHideCount = 0;

		//�J�E���^�����l�ȏ�ɂȂ�΃Q�[�W�B��
		if( HUDHideCount > 30 ) {
			HUDHideFlg  =  true;
			HUDHideCount = 0;
		}
	}

}

TOBJList::TOBJList( TDGCarad* _dddd )
{
	m_dddd = _dddd;

	for( s32 i = 0; i <= 4095 ; i++)
		OBJIDList[i] = 0;

	D3DCAPS9 d3dCaps;
	QD->Get3DDevice()->GetDeviceCaps( &d3dCaps );

	//�o���v�}�b�v���T�|�[�g���Ă��邩
	if( d3dCaps.TextureOpCaps && ( D3DTEXOPCAPS_BUMPENVMAP || D3DTEXOPCAPS_BUMPENVMAPLUMINANCE ) == 0 ) 
		ExtraEffects  =  false;
	//�e�N�X�`���X�e�[�W��3���ȏ゠�邩
	if( d3dCaps.MaxSimultaneousTextures < 3 )
		ExtraEffects  =  false;

	SpriteEXInitialize();

	Scene = new TSceneManager(QD);

	// �X�v���C�g�}�l�[�W���̐���
	if( ExtraEffects )
		Blooming = true;
	else
		Blooming = false;

	m_SprMan  = new TSpriteEXManager(QD,ExtraEffects, Blooming);
	m_SprMan->SetAspect( (f32)QD->GetBackBufferWidth() / 640.f, (f32)QD->GetBackBufferHeight() / 480.f);

	// �e�N�X�`���̐���
	JikiTex = new TDGTexture(QD, DGFMT_ARGB,0,D3DPOOL_MANAGED);     //���@
	FontTex = new TDGTexture(QD, DGFMT_ARGB,0,D3DPOOL_MANAGED);     //�t�H���g
	FontTex_Gothic = new TDGTexture(QD, DGFMT_ARGB,0,D3DPOOL_MANAGED);     //�t�H���g
	Eff1Tex = new TDGTexture(QD, DGFMT_ARGB,0,D3DPOOL_MANAGED); //�G�t�F�N�g
	Eff2Tex = new TDGTexture(QD, DGFMT_ARGB,0,D3DPOOL_MANAGED); //
	Eff4Tex = new TDGTexture(QD, DGFMT_RGB,0,D3DPOOL_MANAGED);    //
	Eff5Tex = new TDGTexture(QD, DGFMT_RGB,0,D3DPOOL_MANAGED);      //
	Eff8Tex = new TDGTexture(QD, DGFMT_ARGB,0,D3DPOOL_MANAGED);
	Eff9Tex = new TDGTexture(QD, DGFMT_ARGB,0,D3DPOOL_MANAGED);
	GaugeTex = new TDGTexture(QD, DGFMT_ARGB,0,D3DPOOL_MANAGED);

	BG3DMan = NULL;

	FTick = 0;
	TimeStop = false;

	GameMode = gTitle;
	DontRender = false;

	CDownTick = 0;

	Replayer = new TReplay();
	REplayON = false;

	Network = new TIdHTTP();
	NetworkID = "";

	//  lastdeleted  =  "";

	for( s32 i = 0 ; i <= 7 ; i++)
		PlayerName[i] = 47;

	AfterLoopEvent = MNone;

	for( s32 l  =  0 ; l < MAXSTAGE; l++ ) {
		for( s32 i = 0 ; i <= 9; i++ ) {
			ScoreRanking_local[l][i].Score = 0;
			for( s32 k = 0 ; k <= 7; k++ )
				ScoreRanking_local[l][i].Name[k] = 47;
			ScoreRanking_local[l][i].Time = 0;
			ScoreRanking_local[l][i].BoostType = 0;
			ScoreRanking_local[l][i].Rank = 1;
			ScoreRanking_local[l][i].WeaponType = 0;
			ScoreRanking_local[l][i].FrameSkipRate = 0;
			ScoreRanking_local[l][i].Year = 0;
			ScoreRanking_local[l][i].Month = 0;
			ScoreRanking_local[l][i].Day = 0;

			TimeRanking_local[l][i].Score = 0;
			for( s32 k = 0 ; k <= 7; k++ )
				TimeRanking_local[l][i].Name[k] = 47;
			TimeRanking_local[l][i].Time = 0;
			TimeRanking_local[l][i].BoostType = 0;
			TimeRanking_local[l][i].Rank = 1;
			TimeRanking_local[l][i].WeaponType = 0;
			TimeRanking_local[l][i].FrameSkipRate = 0;
			TimeRanking_local[l][i].Year = 0;
			TimeRanking_local[l][i].Month = 0;
			TimeRanking_local[l][i].Day = 0;
		}
	}

	for( s32 l  =  0 ; l < MAXSTAGE_SEQ; l++ ) {
		for( s32 i = 0 ; i <= 9; i++ ) {
			ScoreRanking_Seq_local[l][i].Score = 0;
			for( s32 k = 0 ; k <= 7; k++ )
				ScoreRanking_Seq_local[l][i].Name[k] = 47;
			ScoreRanking_Seq_local[l][i].Time = 0;
			ScoreRanking_Seq_local[l][i].BoostType = 0;
			ScoreRanking_Seq_local[l][i].Rank = 1;
			ScoreRanking_Seq_local[l][i].WeaponType = 0;
			ScoreRanking_Seq_local[l][i].FrameSkipRate = 0;
			ScoreRanking_Seq_local[l][i].Year = 0;
			ScoreRanking_Seq_local[l][i].Month = 0;
			ScoreRanking_Seq_local[l][i].Day = 0;

			TimeRanking_Seq_local[l][i].Score = 0;
			for( s32 k = 0 ; k <= 7; k++ )
				TimeRanking_Seq_local[l][i].Name[k] = 47;
			TimeRanking_Seq_local[l][i].Time = 0;
			TimeRanking_Seq_local[l][i].BoostType = 0;
			TimeRanking_Seq_local[l][i].Rank = 1;
			TimeRanking_Seq_local[l][i].WeaponType = 0;
			TimeRanking_Seq_local[l][i].FrameSkipRate = 0;
			TimeRanking_Seq_local[l][i].Year = 0;
			TimeRanking_Seq_local[l][i].Month = 0;
			TimeRanking_Seq_local[l][i].Day = 0;

			for( s32 k = 0 ; k <= 4; k++ ) {
				ScoreRanking_Split_local[l][i].SplitScore[k] = 0;
				ScoreRanking_Split_local[l][i].SplitTime[k] = 0;
				ScoreRanking_Split_local[l][i].Life[k] = 0;
				ScoreRanking_Split_local[l][i].MShip[k] = 0;
				ScoreRanking_Split_local[l][i].Missile[k] = 0;
			}

			for( s32 k = 0 ; k <= 4; k++ ) {
				TimeRanking_Split_local[l][i].SplitScore[k] = 0;
				TimeRanking_Split_local[l][i].SplitTime[k] = 0;
				TimeRanking_Split_local[l][i].Life[k] = 0;
				TimeRanking_Split_local[l][i].MShip[k] = 0;
				TimeRanking_Split_local[l][i].Missile[k] = 0;
			}
		}

		for( s32 i = 0 ; i <= 1; i++ )
			IsCleared[l][i] = false;
	}

	for( s32 i = 0 ; i <= 3; i++ )
		AssinedKey[i] = i;

	BoostMode = 0;
	CloudON = true;
	FilterON = true;
	BGMNumber = 0;
	EffLevel = 2;
	ShieldInfinite = false;
	MShipInfinite = false;
	CollisionON = true;
	TimeInfinite = false;
	SlowON = false;
	TexCol = 1;
	FrameRate = 0;
	SEVol = 8;
	bgmVol = 10;

	LuminanceMap = false;
	ConnectNet = false;
	HeroWeapon = 0;

	KPUTLOG = false;

	TotalRendCount  =  0;
	TotalFrameCount  =  0;

	CheatON = false;

	DemoON = false;
	DemoNumber = -1;

	JikiObj = NULL;

	ScreenshotFlg = false;

	NetReplayState = -1;

	QuakeLife = 0;

	RondomSeed = 1;

	LoadingScreen = NULL;

	GameSpeed = sNormal;

	StageScriptID = 0;

	IsRequestedDelBullets = false;
	IsPractice = false;

	HPMax  =  HPMax_Single;
	MSHPMax = MSHPMax_Single;
	MShipHP = MSHPMax;
	HeroHP = HPMax;

	SequenceTotalTime = 0;

	NowStage = 99;

	if( DebugMode ) {
		NowStage = 0;
		Difficulty  =  KDIFFICULTY_NORMAL;
	}


	for( s32 i = 0 ; i <= 99; i++ ) {
		Z4AppX[i] = 0;
		Z4AppY[i] = 0;
	}

	for( s32 i = 0 ; i <= 7; i++ ) {
		RandZYDist[i] = 0;
		RandZCount[i] = 0;
	}

	for( s32 i = 0 ; i <= 255 ; i++) {
		FunXDist[i] = 0;
		FunXDist[i] = 0;
	}

	for( s32 i = 0 ; i <= 7; i++ )
		DataLoaded[i] = false;

	Setup();

}

TOBJList::~TOBJList()
{
	//�X�R�A�f�[�^
	// �o�C�i���t�@�C����������

	std::ofstream ofs("Score_029.BIN", ios::trunc);

	ofs.write((char*)(&ScoreVersion), sizeof(s8));

	for( s32 l = 0 ; l < MAXSTAGE_SEQ; l++ ) {
		for( s32 i = 0 ; i <= 9; i++ ) {
			ofs.write((char*)(&ScoreRanking_Seq_local[l][i]), sizeof(NetRankingData));
		}
		for( s32 i = 0 ; i <= 9; i++ ) {
			ofs.write((char*)(&TimeRanking_Seq_local[l][i]), sizeof(NetRankingData));
		}
		for( s32 i = 0 ; i <= 9; i++ ) {
			ofs.write((char*)(&ScoreRanking_Split_local[l][i]), sizeof(NetRankingData_Split));
		}
		for( s32 i = 0 ; i <= 9; i++ ) {
			ofs.write((char*)(&TimeRanking_Split_local[l][i]), sizeof(NetRankingData_Split));
		}

		for( s32 i = 0 ; i <= 1; i++ )
			ofs.write((char*)(&IsCleared[l][i]), sizeof(bool));
	}
	ofs.close();

	ConfigSave();

	//Replayer->Save;

	//�I�u�W�F�N�g�J��
	for( objlist_itr itr = m_objList.begin(); itr != m_objList.end(); ++itr ) {
		SAFE_DELETE(*itr);
	}

	QD->Get3DDevice()->SetTexture(0, NULL);     // �e�N�X�`�������O��
	QD->Get3DDevice()->SetTexture(1, NULL);     // �e�N�X�`�������O��
	QD->Get3DDevice()->SetTexture(2, NULL);     // �e�N�X�`�������O��
	SAFE_DELETE(m_SprMan);
	SAFE_DELETE(JikiTex);
	SAFE_DELETE(Eff1Tex);
	SAFE_DELETE(Eff2Tex);
	SAFE_DELETE(Eff4Tex);
	SAFE_DELETE(Eff5Tex);
	SAFE_DELETE(Eff8Tex);
	SAFE_DELETE(Eff9Tex);
	SAFE_DELETE(FontTex);
	SAFE_DELETE(FontTex_Gothic);
	SAFE_DELETE(GaugeTex);

	///////////////////////////////////////////////////
	ReleaseStage();

	SAFE_DELETE(BG3DMan);

	SAFE_DELETE(Replayer);

	SAFE_DELETE(m_Scene);
	SAFE_DELETE(Network);

	SAFE_DELETE(MusicPlayer);
	SAFE_DELETE(SoundEffect);
}

//TOBJ* TOBJList::getOBJFromID( s32 id )
//{
//	if( id<4096 ) result = Items[OBJIDList[id]];
//	else result = Items[0];
//
//}

bool TOBJList::ConfigSave()
{
	//�R���t�B�O�f�[�^�L�^
	std::ofstream ofs("Config_029.BIN", ios::trunc);

	ofs.write((char*)(&ConfigVersion), sizeof(s8));

	for( s32 i = 0 ; i <= 7; i++ )
		ofs.write((char*)(&PlayerName[i]), sizeof(s32));

	for( s32 i = 0 ; i <= 3; i++ )
		ofs.write((char*)(&AssinedKey[i]), sizeof(s32));

	ofs.write((char*)(&BoostMode), sizeof(s32));
	ofs.write((char*)(&CloudON), sizeof(bool));
	ofs.write((char*)(&FilterON), sizeof(bool));
	ofs.write((char*)(&ScreenMode), sizeof(bool));
	ofs.write((char*)(&BGMNumber), sizeof(s32));

	ofs.write((char*)(&ShieldInfinite), sizeof(bool));
	ofs.write((char*)(&MShipInfinite), sizeof(bool));
	ofs.write((char*)(&CollisionON), sizeof(bool));
	ofs.write((char*)(&TimeInfinite), sizeof(bool));
	ofs.write((char*)(&SlowON), sizeof(bool));

	ofs.write((char*)(&EffLevel), sizeof(s32));
	ofs.write((char*)(&TexCol), sizeof(s32));
	ofs.write((char*)(&FrameRate), sizeof(s32));
	ofs.write((char*)(&SEVol), sizeof(s32));
	ofs.write((char*)(&bgmVol), sizeof(s32));
	ofs.write((char*)(&CreateBGM), sizeof(bool));
	ofs.write((char*)(&ScreenResolution), sizeof(s32));
	ofs.write((char*)(&CreateSE), sizeof(bool));
	ofs.write((char*)(&ExtraEffects), sizeof(bool));
	ofs.write((char*)(&ConnectNet), sizeof(bool));
	for( s32 i = 1 ; i <= 12; i++ )
		ofs.write((char*)(&NetworkID[i]), sizeof(char));

	return true;
}

bool TOBJList::ConfigLoad()
{
	//�R���t�B�O�f�[�^�ǂݍ���
	std::ifstream ifs("Config_029.BIN");
	if( ifs )
	{
		s8 readversion;
		ifs.read((char*)&readversion, sizeof(s8));
		if( readversion >= 26 ) {
			for( s32 i = 0 ; i <= 7; i++ )
				ifs.read((char*)&PlayerName[i], sizeof(s32));

			for( s32 i = 0 ; i <= 3; i++ )
				ifs.read((char*)&AssinedKey[i], sizeof(s32));

			ifs.read((char*)&BoostMode, sizeof(s32));
			ifs.read((char*)&CloudON, sizeof(bool));
			ifs.read((char*)&ScreenMode, sizeof(bool));
			ifs.read((char*)&BGMNumber, sizeof(s32));

			ifs.read((char*)&ShieldInfinite, sizeof(bool));
			ifs.read((char*)&MShipInfinite, sizeof(bool));
			ifs.read((char*)&CollisionON, sizeof(bool));
			ifs.read((char*)&TimeInfinite, sizeof(bool));
			ifs.read((char*)&SlowON, sizeof(bool));

			ifs.read((char*)&EffLevel, sizeof(s32));
			ifs.read((char*)&TexCol, sizeof(s32));
			ifs.read((char*)&FrameRate, sizeof(s32));
			ifs.read((char*)&SEVol, sizeof(s32));
			ifs.read((char*)&bgmVol, sizeof(s32));
			ifs.read((char*)&CreateBGM, sizeof(bool));
			ifs.read((char*)&ScreenResolution, sizeof(s32));
			ifs.read((char*)&CreateSE, sizeof(bool));
			ifs.read((char*)&ExtraEffects, sizeof(bool));
			ifs.read((char*)&ConnectNet, sizeof(bool));


			if( readversion == 27 ) {
				for( s32 i = 0 ; i <= 11 ; i++) {
					s8 tmpchar;
					ifs.read((char*)&tmpchar, sizeof(s8));
					NetworkID  =  NetworkID + tmpchar;
				}
			}
			else{
				NetworkID = GetMacAddress();
			}

			return true;
		}
	}
	//else if( ifs.open("Config.BIN") ) {
	//	lbf  =  TFileStream.Create( "Config.BIN", fmOpenRead );
	//	lbf.Read(readversion,sizeof(ShortInt));    //�o�[�W�����m�F
	//	if( readversion >= 26 ) {
	//		for( s32 i = 0 to 7 )
	//			lbf.Read(PlayerName[i],sizeof(Integer));

	//		for( s32 i = 0 to 3 )
	//			lbf.Read( AssinedKey[i], sizeof(Integer) );

	//		lbf.Read( BoostMode, sizeof(Integer) );
	//		lbf.Read( CloudON, sizeof(Boolean) );
	//		lbf.Read( FilterON, sizeof(Boolean) );
	//		lbf.Read( ScreenMode, sizeof(Boolean) );
	//		lbf.Read( BGMNumber, sizeof(Integer) );

	//		lbf.Read(ShieldInfinite, sizeof(Boolean));
	//		lbf.Read(MShipInfinite, sizeof(Boolean));
	//		lbf.Read(CollisionON, sizeof(Boolean));
	//		lbf.Read(TimeInfinite, sizeof(Boolean));
	//		lbf.Read(SlowON, sizeof(Boolean));

	//		lbf.Read(EffLevel, sizeof(Integer));
	//		lbf.Read(TexCol, sizeof(Integer));
	//		lbf.Read(FrameRate, sizeof(Integer));
	//		lbf.Read(SEVol, sizeof(Integer));
	//		lbf.Read(bgmVol, sizeof(Integer));
	//		lbf.Read(CreateBGM, sizeof(Boolean));
	//		lbf.Read(ScreenResolution, sizeof(Integer));
	//		lbf.Read(CreateSE, sizeof(Boolean));
	//		lbf.Read(ExtraEffects, sizeof(Boolean));
	//		lbf.Read(ConnectNet, sizeof(Boolean));

	//		NetworkID = GetMacAddress;

	//		result = true;
	//	}
	//	lbf.Free;
	//}
	//else {
	//	NetworkID = GetMacAddress;

	//}

	ShieldInfinite = false;
	MShipInfinite = false;
	CollisionON = true;
	TimeInfinite = false;
	SlowON = false;

}

std::string TOBJList::ScoreToStr( s32 _score )
{
	s8 c[9];
	sprintf_s( c, "%9d", _score);

	string result(c);

	return result;
}

std::string TOBJList::PlayerNameToStr( const s32 _name[], s32 _size )
{
	string tempname  =  "";

	for( s32 i = 0 ; i < _size; i++ )
		tempname  =  tempname + FONTSET[_name[i]];

	return tempname;
}

std::string TOBJList::DateToStr( s32 _year, s32 _month, s32 _day )
{
	s8 c[10];
	sprintf_s( c, "%2d/%2d/%4d", _month, _day, _year);

	string result(c);

	return result;
}

std::string TOBJList::TimeToStr( s32 _time )
{

	s8 c[10];
	sprintf_s( c, "%d.%d%d", _time/1000, (_time / 100)-(_time / 1000)*10, (_time / 10)-(_time / 100)*10);

	string result(c);

	return result;
}

std::string TOBJList::FrameSkipRateToStr( s32 _fsr )
{
	s8 c[10];
	sprintf_s( c, "%d.%d", _fsr/1000, _fsr-(_fsr / 1000)*1000);

	string result(c);

	return result;
}

s32 TOBJList::GetRand( s32 _max )
{
	//���������߂�
	RondomSeed  =  RondomSeed * (JikiObj->X+1) * (JikiObj->Y+1)  * (FTick+1) * 214013 + 2531011;
	RondomSeed  =  RondomSeed >> 16;
	return  glm::min(_max, (s32)(RondomSeed * ((_max+1)/(65536))));

}

void TOBJList::Move()
{
	m_SprMan->ClearSprite();

	DontRender = false;

	if( /*Form1.*/ActiveFlg ) {
		ScanKey();
		Joy.Scan(PKey+PJoy1);
	}

	GameSpeed = sNormal;

	//Mosaic(this,200,200,200,200,20);


	if( (SlowON) && (! ReplayON) ) GameSpeed = sSlow;
	if( GameMode=gPlay ) {
			//    TempCount[4] = m_SprMan->TotalSpr;
			//    TempCount[5] = m_SprMan->TotalDraw;
			//    TempCount[6] = m_SprMan->TotalSpr-m_SprMan->TotalDraw;

			TotalFrameCount++;

			HUDVisibleControl();

			if( (! BossDie) && (! DemoON) ) Input();

			//�u�[�X�g���x���ɉ����ăX�N���[���X�s�[�h�𒲐�
			if( AcceptChangeSpeed ) {
				switch(BoostLV ) {
				case 1:
				case 4:{
						if( (FTick % 5 == 0) && (Speed>5) )
							Speed--;
					}break;
				case 2:
				case 3:{
						if( (FTick % 10 == 0) && (Speed<11) )
							Speed--;
					}break;
				}
			}
			else {
				Speed = RequestedSpeedFromChild;
			}
			//TempCount[0] = Speed;

			//�C�x���g����
			if( (! DemoON) ) {
				switch(NowStage ) {
					case 0:Event();
					default:
						Script->ExecuteProc(StageScriptID,NULL);
				}
			}

			//�n�C�X�R�A�X�V
			if( TotalScore>HiScore ) HiScore = TotalScore;

			//�Q�[���I�[�o�[
			if( HeroDie ) {
				GameMode = gOver;
				if( LimitTime>0 ) Add( new TGameOver(this,0));
				else Add( new TGameOver(this,2));
			}

			if( MShipHP <= 0 ) {
				TimeStop = true;
				GameMode = gOver;
				if( CreateSE ) SoundEffect->WavPlay(WABossExp,0);
				if( LimitTime>0 ) Add(new TGameOver(this,1));
				else Add(new TGameOver(this,2));
			}

			//�q������
			if( (! DemoON) ) Distance += Speed;

			//�c�莞��
			if( (LimitTime>0) &&
				(! TimeStop) &&
				(! BossDie) &&
			((! TimeInfinite) || (ReplayON)) )
				LimitTime -= GainTime;

			if( LimitTime>10000 ) CDownTick = FTick;

			if( (LimitTime<=10000) && ((FTick-CDownTick) % 60==0) && CreateSE ) SoundEffect->WavPlay(WAHeart,14);

			if( LimitTime<=0 ) LimitTime = 0;

			if( BossDie && (FTick-BDieTime==180) && (! ResultExists) ) {
				Add(new TResult2(this));
			}

			if( (LimitTime>0) && (MShipInfinite) && (! ReplayON) ) MShipHP = MSHPMax;

			//Muteki = true;
	}

	//3D�w�i�X�N���[������
	if( (GameMode == gPlay) || (GameMode == gReady) || (GameMode == gOther)) {
		BG3DMan->Scroll(Distance);
	}

	//��ʗh�炵����
	QuakeManager();

	//�����蔻��
	if( (! BossDie) ) Hantei();

	//���񂾃I�u�W�F�N�g������
	for ( objlist_itr itr = m_objList.begin(); itr != m_objList.end();  ) {
		if( (*itr)->FDead ) {
			SAFE_DELETE(*itr);
			m_objList.erase(itr);
			continue;
		}

		++itr;
	}

	//ID��OBJList���̔ԍ��Ƃ��֘A�t��
	u32 i=0;
	for ( objlist_itr itr = m_objList.begin(); itr != m_objList.end(); ++i, ++itr ) {
		if( ((*itr)->ID < 4096) && ((*itr)->ID >= 0) )
			OBJIDList[(*itr)->ID] = i;
	}


	if( ReplayON && ((GameMode == gPlay) || (GameMode == gReady)) && (! DemoON) ) {
		Replayer->Play(FTick);
		DontRender  =  ! Replayer->Rendered[FTick];
	}

	// BGSlide.Y = 10;
	//�I�u�W�F�N�g�𓮂���
	ITERATE {
			if( ! (*itr)->FDead ) {
			if( TimeStop ) {
				if( ((*itr)->Kind==kGauge) ) (*itr)->Move();
			}
			else {
				//���@�X�s�[�h�ɉ����đ��Α��x�ω�
				if( ((*itr)->Kind == kEnemy) || ((*itr)->Kind == kEShot) || ((*itr)->Kind == kMissile)  || ((*itr)->Kind == kEffect) || ((*itr)->Kind == kFunnel) || ((*itr)->Kind == kEffect4) || ((*itr)->Kind == kColBG) ) {
					if( ((*itr)->Sprite[(*itr)->Sprite_Center] != NULL) && (Speed>0) )
						(*itr)->Sprite[(*itr)->Sprite_Center]->MoveR(-Speed + KSCROLLSPEED_NORMAL, 0);
				}

				//�w�i�ɂ��Ĉړ�
				if( ((*itr)->Kind == kEnemy) || ((*itr)->Kind == kEShot) || ((*itr)->Kind == kEffect) ||
					((*itr)->Kind == kBG) || ((*itr)->Kind == kColBG) || ((*itr)->Kind == kMissile) || ((*itr)->Kind == kEffect4) ) {
						if( ((*itr)->Sprite[(*itr)->Sprite_Center] != NULL) )
							(*itr)->Sprite[(*itr)->Sprite_Center]->MoveR(BGSlide.X, BGSlide.Y);
				}
				(*itr)->Move();
			}

		}
	}


	if( IsRequestedDelBullets ) DeleteBullets();


	//�e����\��
	if( (GameMode = gPlay) && (! DemoON)  ) Console();

	if( (! ReplayON) && ((GameMode == gPlay) || (GameMode == gReady))  && (! DemoON) )
		Replayer->Rec(FTick);


	if( DebugMode ) {
		//    for( s32 i = 0 to 7 )
		//      Sentence(IntToStr(TempCount[i]),0,200+i*16,0,0);
	}
	//TempCount[7] = FTick;

	if( GameMode != gPause ) {
		FTick++;
	}

	if( ScreenshotFlg ) ScreenShot();

	OnAfterLoop();

}

void TOBJList::Render()
{
	if( ExtraEffects ) Add(new TDummy(this,9100));    //�����Y�G�t�F�N�g�p�Ƀ_�~�[�X�v���C�g��ǉ�
	if( ExtraEffects ) Add(new TDummy(this,17100));
	{if( Blooming ) }Add(new TDummy(this,2100));
	Add(new TDummy(this,300));
	Add(new TDummy(this,51200));
	Add(new TDummy(this,25601));

	TotalObj = 0;
	ITERATE {
		if( ! (*itr)->FDead ) {
			TotalObj += (*itr)->OnDraw();
		}
	}

	if( BG3DMan!=NULL ) {
		BG3DMan->FrameControl();
		BG3DMan->RenderBB();
	}

	// �`��J�n��錾
	QD->Get3DDevice()->BeginScene();

	m_SprMan->FSmooth  =  FilterON;
	m_SprMan->Sort();            // �X�v���C�g�̃\�[�g
	m_SprMan->GeometryProcess(); // ���_�o�b�t�@�̐���
	m_Scene->Clear();
	m_Scene->Render();

	ITERATE {
		if( ! (*itr)->FDead ) {
			TotalObj += (*itr)->OnExRender();
		}
	}
	m_Scene->RenderEffect();
	m_SprMan->Render();          // �X�v���C�g��`��

	if( ShowHit ) m_SprMan->RenderHitArea();
	//�`��I���̐錾
	QD->Get3DDevice()->EndScene();
	QD->Get3DDevice()->Present(NULL,NULL,0,NULL); // �t���b�v

	if(GameMode == gPlay ) TotalRendCount++;

}

void TOBJList::GameStart()
{
	//Add(TBoxContrl.Create(this,3));
	JikiObj  =  NULL;
	NewID = 0;
	FTick = 0;
	SetStage(NowStage);
	//�V���O�����[�h�̂ݖʂ��ƂɃX�R�A���N���A
	if( Difficulty == KDIFFICULTY_SINGLE )
		TotalScore = 0;

	switch(Difficulty ) {
		case KDIFFICULTY_SINGLE:{
		HPMax  =  HPMax_Single;
		MSHPMax = MSHPMax_Single;
		//�V���O�����[�h�̂ݖ���HP������
		MShipHP = MSHPMax;
		HeroHP = HPMax;
		RestMis = MissileStock;
		}break;
	}


	Muteki = false;
	HeroDie = false;
	Weapon = true;
	LimitTime = 180000;
	//LimitTime = 10000;
	TotalLock = 0;
	TotalHit = 0;

	for( s32 i = 0 ; i < MaxLock*MissileStock; i++ ) {
		LockID[i] = -1;
		BeLocked[i].X = 0;
		BeLocked[i].Y = 0;
		UnLock[i] = true;
	}

	for( s32 i = 0 ; i <= 9 ; i++)
		TempCount[i] = 0;

	GameMode = gTitle;
	MissileON = false;
	TimeStop = false;
	Speed = 5;
	Distance = 0;
	StartDistance = 0;
	QuakeFlg = false;
	QuakeTime = 0;
	BGSlide.X = 0;
	BGSlide.Y = 0;
	WS.EventCount = 0;
	ShowHit = false;
	FunSignal = fNone;
	FunSync = 0;
	FunPattern = 0;
	BossDie = false;
	BDieTime = 0;
	DontBoost = false;
	if( Difficulty == KDIFFICULTY_SINGLE )
		HiScore = ScoreRanking_local[NowStage][0].Score;
	else
		HiScore = ScoreRanking_Seq_local[Difficulty][0].Score;

	BossRate = 1;
	InitSprZ();
	TotalKage = 0;
	ResultExists = false;
	FlareOK = true;
	AcceptChangeSpeed = true;
	RequestedSpeedFromChild = 0;
	ResultSkipped = false;

	TotalRendCount  =  0;
	TotalFrameCount  =  0;

	ComBonusCount  =  0;

	IsRecovering = false;

	HUDHideFlg  =  false;
	HUDHideCount = 0; 

	m_SprMan->Move(0,0);

	m_Scene->SetClipNear(0.01f);

	BG3DMan->SetCameraOffset(0,0,0);

	if( TrialMode )
		ConnectNet = false;

	if( ShieldInfinite || MShipInfinite || (! CollisionON) || TimeInfinite || SlowON ) CheatON = true;
	else CheatON = false;


}

void TOBJList::GameReset()
{
	ITERATE {
		(*itr)->FDead = true;
	}

}

void TOBJList::Sentence( string moji, s32 fx, s32 fy, s32 brank, s32 _pat )
{
	s32 cx = (s32)((moji.size()*16 + (moji.size()-1)*brank) / 2);
	SentenceCenter(moji,fx+cx,fy,brank,_pat);

}

void TOBJList::SentenceCenter( const string& moji, s32 fx, s32 fy, s32 brank, s32 _pat )
{
	s32 Fnum = 0;
	for( s32 i = 1 ; i < moji.size(); i++ ) {
		for( s32 k = 0 ; k <= 47 ; k++){
			if( moji[i] == FONTSET[k] ) { Fnum = k; break; }
		}
		s32 cx = (s32)((moji.size()*16+(moji.size()-1)*brank)/2-((i-1)*16+(i-1)*brank));
		s32 cy = 16;
		Add(new TMoji(this,Fnum,fx,fy+16,cx,cy,_pat));
	}

}

void TOBJList::SentenceRight( const string& moji, s32 fx, s32 fy, s32 brank, s32 _pat )
{
	s32 cx = (s32)((moji.size()*16 + (moji.size()-1)*brank) / 2);
	SentenceCenter(moji,fx-cx,fy,brank,_pat);

}

void TOBJList::ShowScore( s32 score,s32 x, s32 y, bool _red )
{
	s32 Pattern;
	if( _red ) Pattern = 4;
	else Pattern = 3;

	for( s32 i = 8 ; i >= 0; i-- ) {
		s32 dig = pow(10,i);
		for( s32 k = 0 ; k <= 9; k++ ) {
			if( score / dig == k ) {
				Add(new TMoji(this,k,x+(8-i)*16,y,0,0, Pattern));
				break;
			}
		}
		score -= (score / dig) * dig;
	}

}

void TOBJList::ShowTime( s32 _x, s32 _y, s32 _time, bool _red )
{
	s32 FontCol;
	if( _red ) FontCol = 4;
	else FontCol = 3;

	//�c�莞�ԕ\��
	SentenceRight(InttoStr(_time / 1000),_x+16*3,_y,0,FontCol);
	Sentence(".",_x+16*3,_y,0,FontCol);
	Sentence(InttoStr((_time / 100)-(_time / 1000)*10),_x+16*4,_y,0,FontCol);
	Sentence(InttoStr((_time / 10)-(_time / 100)*10),_x+16*5,_y,0,FontCol);

}

void TOBJList::Scrach( TOBJ* target )
{
	s32 DX = RoundOff(HeroPos.X-target->X);             //���@�ƓG�Ƃ̋������v�Z
	s32 DY = RoundOff(HeroPos.Y-target->Y);
	s32 Rdeg = RoundOff( atan2((f32)DY, (f32)DX)/ (f32)M_PI*2048.f) && 0xfff;
	Add(new TFF(this,HeroPos.X,HeroPos.Y,Rdeg));
	TotalScore += 500;

	if( CreateSE ) SoundEffect->WavPlay(WAGraze,0);

}

class DistSortCompare{
public:
	bool operator()(const void* _item1, const void* _item2 ) const {
		return ( ((TOBJ*)_item1)->distance < ((TOBJ*)_item2)->distance );
	}
};

void TOBJList::LockOn( s32 setNumber )
{
	vector<TOBJ*> TargetList;

	//�S�I�u�W�F�N�g����G�I�u�W�F�N�g�����𒊏o
	ITERATE{
		if( (*itr)->Kind == kEnemy ) {
			TargetList.push_back(*itr);
			s32 DX = HeroPos.X-(*itr)->X;
			s32 DY = HeroPos.Y-(*itr)->Y;
			(*itr)->distance = pow( (f32)(DX*DX + DY*DY) , 0.5f);      //���@�Ƃ̋������v�Z
		}
	}

	//���@����߂����Ƀ\�[�g
	sort(TargetList.begin(), TargetList.end(), DistSortCompare());

	//�߂���HP�̍������̂���D��I�Ƀ��b�N�I��
	s32 Lnumber = 0;
	if( TargetList.size()>0 ) {
		for( s32 i = 0 ; i < TargetList.size(); i++) {
			TOBJ* Target = TargetList[i];
			s32 TempHP = Target->HP;
			while( TempHP > 0 ) {
				if( Lnumber >= MaxLock ) break;
				LockID[Lnumber+MaxLock*setNumber] = Target->ID;
				BeLocked[Lnumber+MaxLock*setNumber].X = Target->X;
				BeLocked[Lnumber+MaxLock*setNumber].Y = Target->Y;
				TempHP -= MissileSTR-Target->Def;
				Add(new TMarker(this,Lnumber+MaxLock*setNumber));//�}�[�J�[�o��
				UnLock[Lnumber+MaxLock*setNumber] = false;
				Lnumber++;
			}
		}
	}

	if( Lnumber>0 ) {
		Add(new TMakeMissile(this,setNumber,Lnumber));
		RestMis--;
	}
}

void TOBJList::LockOn2( s32 setNumber, s32 MisNumber )
{
	vector<TOBJ*> TargetList;

	//�S�I�u�W�F�N�g����G�I�u�W�F�N�g�����𒊏o
	ITERATE{
		if( (*itr)->Kind == kEnemy ) {
			TargetList.push_back(*itr);
			s32 DX = HeroPos.X-(*itr)->X;
			s32 DY = HeroPos.Y-(*itr)->Y;
			(*itr)->distance = pow( (f32)(DX*DX + DY*DY) , 0.5f);      //���@�Ƃ̋������v�Z
		}
	}

	//���@����߂����Ƀ\�[�g
	sort(TargetList.begin(), TargetList.end(), DistSortCompare());

	//�߂���HP�̍������̂���D��I�Ƀ��b�N�I��
	s32 Lnumber = 0;
	if( TargetList.size()>0 ) {
		for( s32 i = 0 ; i < TargetList.size(); i++) {
			TOBJ* Target = TargetList[i];
			if( Target->TempHP > 0 ) {
				LockID[MisNumber+MaxLock*setNumber] = Target->ID;
				BeLocked[MisNumber+MaxLock*setNumber].X = Target->X;
				BeLocked[MisNumber+MaxLock*setNumber].Y = Target->Y;
				Target->TempHP -= MissileSTR-Target->Def;
				Add(new TMarker(this,MisNumber+MaxLock*setNumber));//�}�[�J�[�o��
				UnLock[MisNumber+MaxLock*setNumber] = false;
				break;
			}
		}
	}

}

void TOBJList::Input()
{
	for( s32 i = 3 ; i <= 15 ; i++)
		if( ! ButtonON[i] ) ButtonFlg[i] = false;

	//�|�[�Y
	if( (! ButtonFlg[3]) && ButtonON[3] ) {
		AfterLoopEvent = MPause;
		ButtonFlg[3] = true;
	}

	if( DebugMode ) {
		if( (! ButtonFlg[6]) && ButtonON[6] ) {

			ButtonFlg[6] = true;
		}

		if( (! ButtonFlg[7]) && ButtonON[7] ) {

			GameReset();
			GameStart();
			EventReload();
			/*Form1.*/ReloadScript();
			PlayStart();

			ButtonFlg[7] = true;
		}

		if( (! ButtonFlg[8]) && ButtonON[8] ) {

			LimitTime = 0;
			ButtonFlg[8] = true;
		}         

	}

	if( ReplayON ) {
		if( ButtonON[0] ) GameSpeed = sSlow;
		if( ButtonON[1] ) GameSpeed = sFast;

	}

}

void TOBJList::Quake( s32 _power, s32 _time )
{
	QuakeFlg = true;
	QuakeTime = FTick;
	QuakePower  =  _power;
	QuakeLife = _time;

}

void TOBJList::Console()
{
	if( ! HUDHideFlg ) {
		//���_��\��
		Sentence("hiscore",360,8,0,0);
		Sentence("score",392,32,0,0);
		ShowScore(HiScore,488,8,false);
		ShowScore(TotalScore,488,32,false);
	}

}

void TOBJList::PlayStart()
{

	GameMode = gPlay;

	Replayer->InitCurrentData();

	if( ReplayON ) {
		HeroWeapon  =  Replayer->RepWType;

		HeroHP = Replayer->SplitData.Life[NowStage];
		MShipHP = Replayer->SplitData.Mship[NowStage];
		RestMis = Replayer->SplitData.Missile[NowStage];
	}
	else {
		Replayer->SplitData.Life[NowStage]  =  HeroHP;
		Replayer->SplitData.MShip[NowStage]  =  MShipHP;
		Replayer->SplitData.Missile[NowStage]  =  RestMis;
	}

	if( DebugMode )
		HeroWeapon = 1;

	if( HeroWeapon == 0 )
		JikiObj  = new TJiki(this,320,240);  //���@�̐���
	else
		JikiObj  = new TJiki2(this,320,240);

	Add(JikiObj);

	if( ExtraEffects ) {

	}
	ButtonFlg[0]  =  true;

}

void TOBJList::GoTitle()
{
	if( CreateBGM ) MusicPlayer->FadeOut(0,0.0,0.5);
	if( CreateBGM ) MusicPlayer->FadeOut(1,0.0,0.5);
	if( CreateBGM ) MusicPlayer->FadeOut(2,0.0,0.5);
	ReplayON = false;
	Replayer->Init();
	Replayer->InitSplitData();
	GameStart();
	Add(new TTitle2(this));
	IsPractice  =  false;

}

void TOBJList::ScanKey()
{
	for( s32 i = 0 ; i <= 15; i++ )
		ButtonON[i] = false;

	Joy.Scan(PKey);

	for( s32 i = 0 ; i <= 15; i++ )
		if( Joy.getbutton(i) ) ButtonON[i] = true;

	Joy.Scan(PJoy1);

	for( s32 i = 0 ; i <= 3; i++ )
		if( Joy.getbutton(AssinedKey[i])) ButtonON[i] = true;


}

void TOBJList::ReleaseBeforeReset()
{
	// if( BGTex!=NULL ) BGTex.Free;

}

void TOBJList::InitAfterReset()
{
	//BGTex = new TDGTexture(QD, DGFMT_RGB,0,D3DPOOL_DEFAULT);

}

void TOBJList::CulcFrameSkipRate()
{
	//�S�����_�����O�񐔂�S�t���[�����Ŋ���A�����1��������ƃt���[���X�L�b�v�̊������o��B
	//�����_3���܂ł𐮐��l�Ƃ��ĕۑ����邽�߁A1000�{���������l�̌ܓ����Ă���B
	if( TotalFrameCount>0 )
		FrameSkipRate  =  RoundOff((1.f - (f32)TotalRendCount / (f32)TotalFrameCount) * 1000);

}

void TOBJList::ScreenShot()
{
	//�f�B���N�g���쐬
	_mkdir("Screenshots");


	//�������O�̃t�@�C�����Ȃ����`�F�b�N
	for(s32 i=0; i<100000; i++){
		s8 c[5];
		sprintf_s(c, "%5d", i );
		string FileNum(c);

		string filename = "Screenshots/" + "CP" + FileNum + ".BMP";
		FILE *fp;
		if ( (fp = fopen(filename.c_str(),"r")) != NULL ){
			fclose( fp );
		}
		else{
			m_SprMan->SaveScreenShot(filename);
			break;
		}
	}

	ScreenshotFlg  =  false;
}

void TOBJList::OnFrameSkipped()
{
	if( (! ReplayON) && ((GameMode == gPlay) || (GameMode == gReady)) ) Replayer->Rendered[FTick-1] = false;
}

TDGTexture* TOBJList::GetExTexture()
{
	return m_Scene->GetExTexture();

}

TDGTexture* TOBJList::GetAlphaTexture()
{
	return m_SprMan->GetAlphaEffectTex();

}

TDGTexture* TOBJList::GetAddTexture()
{
	return m_SprMan->GetAddEffectTex();

}

void TOBJList::PrepareBeforeDevLost()
{
	//�S�I�u�W�F�N�g�Ƀf�o�C�X���X�g��ʒB
	ITERATE {
		if( (*itr)->FDead == false ) {
			(*itr)->OnDeviceLost();
		}
	}

	//�`��G���W���ɂ��ʒB
	m_SprMan->ReleaseBeforeReset();
	m_Scene->ReleaseBeforeReset();

}

void TOBJList::ResetAfterDevLost()
{
	//�`��G���W���Ƀf�o�C�X���Z�b�g��ʒB
	m_SprMan->InitAfterReset();
	m_Scene->InitAfterReset();

	//�S�I�u�W�F�N�g�ɒʒB
	ITERATE {
			if( (*itr)->FDead == false ) {
			(*itr)->OnDeviceReset();
		}
	}

}

void TOBJList::CreateHUD()
{
	Add(new THPGauge2(this,8,8,0));
	Add(new TMSGauge2(this,8,32,0));
	Add(new TMiSGauge2(this,8,56,0));
	Add(new TLimitTime(this,216,450));
	Add(new TMojiBack(this,344,18,296));   //�n�C�X�R�A�w�i
	Add(new TMojiBack(this,376,42,296));   //�X�R�A�w�i
	Add(new TFPSIndicator(this,0,454));

	if( IsPractice ) Add(new TPracticeTitle(this));

}

void TOBJList::InitBeforePlayStart()
{
	FTick = 0;
	DemoON = false;
	TotalRendCount  =  0;
	TotalFrameCount  =  0;
	LimitTime = 180000;
	Distance = StartDistance;
	RondomSeed = 1;
	if( HeroWeapon == 0 )
		((TJiki*)JikiObj)->ResetBeforeStart();
	else
		((TJiki2)JikiObj)->ResetBeforeStart();

}

void TOBJList::TimeOver()
{
	TimeStop = true;
	GameMode = gOver;
	Add(new TGameOver(this,2));

}

void TOBJList::StageEnd()
{
	TMainLoopEvent NextEvent, AfterReplay;
	if( (Difficulty == KDIFFICULTY_SINGLE) || (HeroHP <= 0) || (MShipHP <=0) || (LimitTime <= 0) || (IsPractice) ) {
		NextEvent  =  MTitle;
		AfterReplay  =  MRepView;
	}
	else {
		NextEvent  =  MLoadBeforePlay;
		AfterReplay  =  MLoadBeforePlay;
	}

	switch(NowStage ) {
	case 0:
	case 2:
	case 3:{
			if( ReplayON )
				AfterLoopEvent = AfterReplay;
			else {
				TimeStop = false;
				AfterLoopEvent = NextEvent;
			}
		}break;
	case 1:{
			if( BossDie && (! ResultSkipped) )
				Add(new TSubmarineDemo(this));
			else {
				if( ReplayON )
					AfterLoopEvent = AfterReplay;
				else {
					TimeStop = false;
					if( TrialMode )
						AfterLoopEvent = MTitle;
					else
						AfterLoopEvent = NextEvent;
				}
			}
		}break;
	case 4:{
			if( ReplayON )
				AfterLoopEvent = MRepView;
			else if( BossDie && (! IsPractice) ) {
				Add(new TEndingStarter(this));
			}
			else {
				TimeStop = false;
				AfterLoopEvent = MTitle;
			}
		}break;
	}

}

void TOBJList::SetBloomingColor( s32 _add, s32 _alpha, s32 _r, s32 _g, s32 _b )
{
	if( ! ExtraEffects ) return;

	((TBloomingBasic*)BloomingFilter)->SetColor(_add,_alpha,_r,_g,_b);

}

void TOBJList::SetBloomingColorSoon( s32 _add, s32 _alpha, s32 _r, s32 _g, s32 _b )
{
	if( ! ExtraEffects ) return;

	((TBloomingBasic*)BloomingFilter)->SetColorSoon(_add,_alpha,_r,_g,_b);

}

void TOBJList::OnLoaded( void* Sender )
{
	if( LoadingScreen != NULL ) {
		LoadingScreen->Die();
		LoadingScreen = NULL;
	}

	//���[�h�������̏������L�q
	switch(AfterLoadingEvent ) {
	case MLoadBeforePlay:
	case MPlayStart:
		{
			AfterLoopEvent = MPlayStart;
		}break;
	case MTitle:
		{
			GoTitle();
		}break;
	case MOpeningMovie:
		{
			Add(new TMovieManager(this,0));
			FTick  =  0;   //�t���[���X�L�b�v����̂���
		}break;
	case MEndingMovie:
		{
			if( CreateBGM ) MusicPlayer->Stop();
			GameStart();
		}break;
	case MRepView:
		{
			GameStart();
			Add(new TBoxContrl(this,7));

			if( CreateBGM ) MusicPlayer->FadeIn(3, 1.0, 2.0);

			if( NetReplayState<0 )
				Add(new TLocalReplayViewer(this));
			else
				Add(new TNetRankingReplayViewer(this));
		}break;
	case MRestart_Seq:
		{
			AfterLoopEvent = MRestart;
			if( CreateBGM ) MusicPlayer->Play(0);
		}break;
	}
	AfterLoadingEvent = MNone;

}

void TOBJList::LoadInternal()
{
//FName: String;
//lbf: TFileStream;
//i:Integer;
//readversion:ShortInt;
//TmpBmp:array [0..12]  TBitmap;
//

	//3D�w�i�Ǘ��N���X�쐬     �iLoading��ʕ\�����Ƀ��b�V����ǂݍ��܂��������߁A���̈ʒu�j
	if( BG3DMan == NULL )
		BG3DMan = new TBG3DManager();

	switch(NowStage ) {
		case 0:{
			// �e�N�X�`���̐���
			MechaTex = new TDGTexture(QD, DGFMT_ARGB,0,D3DPOOL_MANAGED);   //�U�R
			BGTex1 = new TDGTexture(QD, DGFMT_RGB,0,D3DPOOL_MANAGED);      //�w�i
			BGTex2 = new TDGTexture(QD, DGFMT_RGB,0,D3DPOOL_MANAGED);     //�w�i
			BumpTex = new TDGTexture(QD,DGFMT_ARGB ,0,D3DPOOL_MANAGED);     //�o���v�e�N�X�`��               DGFMT_Bump
			SunTex = new TDGTexture(QD, DGFMT_ARGB,0,D3DPOOL_MANAGED);
			DemoTex = new TDGTexture(QD, DGFMT_RGB,0,D3DPOOL_MANAGED);
			Enemy5Tex = new TDGTexture(QD, DGFMT_ARGB,0,D3DPOOL_MANAGED);	// �{�X
			BossBump = new TDGTexture(QD, DGFMT_ARGB,0,D3DPOOL_MANAGED);	// �{�X�o���v

			//�O���t�B�b�N�f�[�^�ǂݍ���
			std::ifstream ifs("./Data/Stage1.BIN");
			if( ifs ) {
				s8 readversion;
				ifs.read((char*)&readversion, sizeof(s8));
				if( readversion == Stage1Version ) {
					//�e�N�X�`���̓ǂݍ���
					MechaTex->LoadFromStream(ifs);	break;
					BGTex1->LoadFromStream(ifs);	break;
					BGTex2->LoadFromStream(ifs);	break;
					BumpTex->LoadFromStream(ifs);	break;
					Enemy5Tex->LoadFromStream(ifs);	break;
					BossBump->LoadFromStream(ifs);	break;
					SunTex->LoadFromStream(ifs);	break;
					DemoTex->LoadFromStream(ifs);	break;

					if( !ExtraEffects ) {
						SAFE_DELETE(BumpTex);
						SAFE_DELETE(BossBump);
					}

					for( s32 i = 0 ; i <= 99 ; i++) {
						ifs.read( (char*)&Z4AppX[i], sizeof(s32) );
						ifs.read( (char*)&Z4AppY[i], sizeof(s32) );
					}

					for( s32 i = 0 ; i <= 7; i++ ) {
						ifs.read( (char*)&RandZYDist[i], sizeof(s32) );
						ifs.read( (char*)&RandZCount[i], sizeof(s32) );
					}

					for( s32 i = 0 ; i <= 255 ; i++) {
						ifs.read( (char*)&FunXDist[i], sizeof(s32) );
						ifs.read( (char*)&FunYDist[i], sizeof(s32) );
					}

					// �G�z�u�ǂݍ���
					ifs.read( (char*)&WS, sizeof(WS) );
				}
				ifs.close();
			}

			EventReload();

			//BGM���[�h
			if( CreateBGM ) {
				MusicPlayer->BGMChange(0, KBGM_STAGE1_A);
			}
		}break;
		case 1:{
			// �e�N�X�`���̐���
			Eff6Tex = new TDGTexture(QD, DGFMT_ARGB,0,D3DPOOL_MANAGED);      //�_
			Eff7Tex = new TDGTexture(QD, DGFMT_ARGB,0,D3DPOOL_MANAGED);      //�C
			BGTex2 = new TDGTexture(QD, DGFMT_RGB,0,D3DPOOL_MANAGED);     //�w�i
			MechaTex = new TDGTexture(QD, DGFMT_ARGB,0,D3DPOOL_MANAGED);   //�U�R
			Enemy4Tex = new TDGTexture(QD, DGFMT_ARGB,0,D3DPOOL_MANAGED);  //���{�X
			ST2Tex2 = new TDGTexture(QD, DGFMT_ARGB,0,D3DPOOL_MANAGED);  //�g���l��
			DemoTex = new TDGTexture(QD, DGFMT_ARGB,0,D3DPOOL_MANAGED);  //������
			Eff3Tex = new TDGTexture(QD, DGFMT_ARGB,0,D3DPOOL_MANAGED);  //���[�v

			//�O���t�B�b�N�f�[�^�ǂݍ���
			string FName  =  "./Data/Stage2.BIN";

			Eff6Tex->LoadFromFile("Dawncloud.bmp", FName);
			Eff7Tex->LoadFromFile("sea4.bmp", FName);
			BGTex2->LoadFromFile("dawnsky1_2.bmp", FName);
			MechaTex->LoadFromFile("Zako.bmp", FName);
			Enemy4Tex->LoadFromFile("boss_stage2_1.bmp", FName);
			ST2Tex2->LoadFromFile("tunnel.bmp", FName);
			DemoTex->LoadFromFile("submarine.bmp", FName);
			Eff3Tex->LoadFromFile("warp.bmp", FName);

			//���b�V���̃��[�h
			Board = new TSXMesh(QD);
			CloudMesh = new TSXMesh(QD);
			Undersea = new TSXMesh(QD);
			Undersea2 = new TSXMesh(QD);

			Board->LoadFromFile("sea.sx", FName);
			CloudMesh->LoadFromFile("cloud.sx", FName);
			Undersea->LoadFromFile("Undersea.sx", FName);
			Undersea2->LoadFromFile("Undersea2.sx", FName);


			BG3DMan->Load("stage2.stg");

			//BGM���[�h
			if( CreateBGM ) {
				MusicPlayer->BGMChange(0, KBGM_STAGE2_A);
				MusicPlayer->BGMChange(1, KBGM_STAGE2_B);
			}
		}break;
		case 2:{
			string FName  =  "./Data/Stage3.BIN";
			MissileMesh = new TSXMesh(QD);
			Eff6Tex = new TDGTexture(QD, DGFMT_ARGB,0,D3DPOOL_MANAGED);
			MissileTex = new TDGTexture(QD, DGFMT_ARGB,0,D3DPOOL_MANAGED);
			MechaTex = new TDGTexture(QD, DGFMT_ARGB,0,D3DPOOL_MANAGED);

			MissileMesh->LoadFromFile("Missile.sx", FName);
			Eff6Tex->LoadFromFile("cloud0.bmp", FName);
			MissileTex->LoadFromFile("missiletex.bmp", FName);
			MechaTex->LoadFromFile("stage3enemy.bmp", FName);
			//      MechaTex.LoadFromFile(DEBUG_GRAPH_PATH+"stage3enemy.bmp");

			BG3DMan->Load("stage3.stg");

			//BGM���[�h
			if( CreateBGM ) {
				MusicPlayer->BGMChange(0,KBGM_STAGE3);
			}
		}break;
		case 3:{
			string FName  =  "./Data/Stage4.BIN";
			Eff3Tex = new TDGTexture(QD, DGFMT_ARGB,0,D3DPOOL_MANAGED);      //���[�v
			MechaTex = new TDGTexture(QD, DGFMT_ARGB,0,D3DPOOL_MANAGED);   //�U�R
			Enemy4Tex = new TDGTexture(QD, DGFMT_ARGB,0,D3DPOOL_MANAGED);  //���{�X

			Eff3Tex->LoadFromFile("warp.bmp", FName);
			MechaTex->LoadFromFile("stage4enemy.bmp", FName);
			Enemy4Tex->LoadFromFile("stage4boss1.bmp", FName);

			switch(Difficulty ) {
			case KDIFFICULTY_SINGLE:BG3DMan->Load("stage4.stg");	break;
			case KDIFFICULTY_EASY:BG3DMan->Load("stage4_e.stg");	break;
			case KDIFFICULTY_NORMAL:BG3DMan->Load("stage4_n.stg");	break;
			case KDIFFICULTY_HARD:BG3DMan->Load("stage4_h.stg");	break;
			}


			//BGM���[�h
			if( CreateBGM ) {
				MusicPlayer->BGMChange(0,KBGM_STAGE4_A);
				MusicPlayer->BGMChange(1,KBGM_STAGE4_B);
			}
		}break;
		case 4:{
			FName  =  "./Data/Stage5.BIN";
			Eff6Tex = new TDGTexture(QD, DGFMT_ARGB,0,D3DPOOL_MANAGED);
			Eff7Tex = new TDGTexture(QD, DGFMT_ARGB,0,D3DPOOL_MANAGED);
			MechaTex = new TDGTexture(QD, DGFMT_ARGB,0,D3DPOOL_MANAGED);   //�U�R
			Enemy4Tex = new TDGTexture(QD, DGFMT_ARGB,0,D3DPOOL_MANAGED);  //���{�X
			Enemy5Tex = new TDGTexture(QD, DGFMT_ARGB,0,D3DPOOL_MANAGED);  //�{�X

			Eff6Tex->LoadFromFile("cloud0.bmp", FName);
			Eff7Tex->LoadFromFile("stage5storm.bmp", FName);
			MechaTex->LoadFromFile("stage5enemy.bmp", FName);
			Enemy4Tex->LoadFromFile("stage5boss1.bmp", FName);
			Enemy5Tex->LoadFromFile("stage5boss3.bmp", FName);

			BG3DMan->Load("stage5.stg");

			//BGM���[�h
			if( CreateBGM ) {
				MusicPlayer->BGMChange(0,KBGM_STAGE5_A);
				MusicPlayer->BGMChange(1,KBGM_STAGE5_B);
			}
		}break;

		case 5:{ //�f�o�b�O�p�X�e�[�W
			//      BG3DMan->Load(NowStage);
			//BGM���[�h

		}break;
		//�G���f�B���O
		case 98:{
			if( CreateBGM ) {
				MusicPlayer->BGMChange(0,KBGM_ENDING);
			}
		}break;
		//�^�C�g�����
		case 99:{
			// �e�N�X�`���̐���
			MechaTex = new TDGTexture(QD, DGFMT_ARGB,0,D3DPOOL_MANAGED);   //
			BGTex1 = new TDGTexture(QD, DGFMT_RGB,0,D3DPOOL_MANAGED);      //�w�i
			BGTex2 = new TDGTexture(QD, DGFMT_RGB,0,D3DPOOL_MANAGED);     //�w�i
			BShotTex1 = new TDGTexture(QD, DGFMT_ARGB,0,D3DPOOL_MANAGED);
			BShotTex2 = new TDGTexture(QD, DGFMT_ARGB,0,D3DPOOL_MANAGED);
			BShotBumpTex1 = new TDGTexture(QD, DGFMT_ARGB,0,D3DPOOL_MANAGED);
			BShotBumpTex2 = new TDGTexture(QD, DGFMT_ARGB,0,D3DPOOL_MANAGED);
			Eff6Tex = new TDGTexture(QD, DGFMT_ARGB,0,D3DPOOL_MANAGED);      //�_
			Eff7Tex = new TDGTexture(QD, DGFMT_ARGB,0,D3DPOOL_MANAGED);      //�C
			ST2Tex1 = new TDGTexture(QD, DGFMT_ARGB,0,D3DPOOL_MANAGED);  //����
			DemoTex = new TDGTexture(QD, DGFMT_ARGB,0,D3DPOOL_MANAGED);  //�J�[�\��


			string FName  =  "./Data/Stage100.BIN";
			MechaTex->LoadFromFile("Machines.bmp", FName);
			BGTex1->LoadFromFile("sky2_s.bmp", FName);
			BGTex2->LoadFromFile("dawnsky1_2.bmp", FName);
			BShotTex1->LoadFromFile("BShot_rifle.bmp", FName);
			BShotTex2->LoadFromFile("BShot_blade.bmp", FName);
			BShotBumpTex1->LoadFromFile("BShot_rifle_bump.bmp", FName);
			BShotBumpTex2->LoadFromFile("BShot_blade_bump.bmp", FName);
			Eff6Tex->LoadFromFile("DawnCloud.bmp", FName);
			Eff7Tex->LoadFromFile("sea4.bmp", FName);
			ST2Tex1->LoadFromFile("stage5storm.bmp", FName);
			DemoTex->LoadFromFile("stageselect.bmp", FName);

			//���b�V���̃��[�h
			Board = new TSXMesh(QD);
			CloudMesh = new TSXMesh(QD);
			Undersea = new TSXMesh(QD);
			Undersea2 = new TSXMesh(QD);

			Board->LoadFromFile("sea.sx", FName);
			CloudMesh->LoadFromFile("cloud.sx", FName);
			Undersea->LoadFromFile("Undersea.sx", FName);
			Undersea2->LoadFromFile("Undersea2.sx", FName);

			for( s32 i = 0 ; i < NCube; i++ ) {
				GCube[i] = new TSXMesh(QD);
				GCube[i]->LoadFromFile("cube.sx", "./Data/Misc.BIN");
			}

			BG3DMan->Load("stage100.stg");

			//BGM���[�h
			if( CreateBGM ) {
				MusicPlayer->BGMChange(3,KBGM_MENU);
			}
		}break;
	}

	OnLoaded(NULL);

}

void TOBJList::SetRankInHeader()
{
	s32 TempTime;
	if( BossDie ){
		if( (Difficulty < KDIFFICULTY_SINGLE) )
			TempTime = SequenceTotalTime;
		else
			TempTime = LimitTime;
	}
	else{
		TempTime = 0;
	}

	for( s32 i  =  0 ; i <= 7; i++ )
		RankinHeaderData.PlayerName[i] = PlayerName[i];

	RankinHeaderData.Score = TotalScore;
	RankinHeaderData.Time = TempTime;
	RankinHeaderData.BType = BoostMode;
	RankinHeaderData.WType = HeroWeapon;
	RankinHeaderData.FrameSkipRate = FrameSkipRate;
	RankinHeaderData.Stage = NowStage;
	RankinHeaderData.Difficulty = Difficulty;

	if( ! BossDie ) {
		RankinHeaderData.OriginalScore = TotalScore;
		RankinHeaderData.DestBonusRate = 0;
		RankinHeaderData.Life = 0;
		RankinHeaderData.NoDamageBonus = 0;
		RankinHeaderData.TotalHit = 0;
		RankinHeaderData.ClearBonus = 0;
	}
}

bool TOBJList::IsSequence()
{
	if( Difficulty == KDIFFICULTY_SINGLE )
		return false;
	else
		return true;

}

void TOBJList::RequestDelBullets()
{
	IsRequestedDelBullets = true;

}

bool TOBJList::IsBossDefeated( s32 _stage )
{
	for( s32 i  =  0 ; i <= 9; i++ ) {
		if( (TimeRanking_local[_stage][i].Time > 0) || (ScoreRanking_Split_local[Difficulty][i].SplitTime[_stage] > 0) ) {
			return true;
		}
	}
	return false;
}

bool TOBJList::IsBossDefeated_Seq( s32 _stage )
{
	for( s32 i  =  0 ; i <= 9; i++ ) {
		if( (ScoreRanking_Split_local[Difficulty][i].SplitTime[_stage] > 0) ) {
			return true;
		}
	}

	return false;
}

void TOBJList::Add( TOBJ* obj )
{
	m_objList.push_back(obj);
}

void TOBJList::AddSprite( TSpriteBase* _spr )
{
	m_SprMan->AddSprite(_spr);
}

