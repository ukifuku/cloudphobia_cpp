#pragma once

using namespace std;

#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include <vector>
#include "types.h"
#include "glm/glm.hpp"
#include "SXLib9.h"

#define MissileSTR  30 //�~�T�C���U����
#define MaxLock		8     //�ő働�b�N��(2�̔{��)
#define MissileStock  8    //�~�T�C���Z�b�g�ő及����
//NSmoke =512;     //���̗�
#define KSTR_SHOT	5    //�V���b�g�U����
#define KSTR_BLADE  90  //�u���[�h�U����

#define KazeSpeed	70    //���f�ЃX�s�[�h

#define MaxJoint	49    //�ő�Ԑڐ�

#define GainTime	17     //1�t���[��������̎��Ԍ�����

#define HPMax_Single	7000          //���@�̍ő�HP     7000
#define MSHPMax_Single  12000       //��͂̍ő�HP
#define HPMax_Easy		8000
#define MSHPMax_Easy	 12000
#define HPMax_Normal	 6000
#define MSHPMax_Normal	 8000
#define HPMax_Hard		 5000
#define MSHPMax_Hard	 7000
#define BossMaxHP		 3000     //�{�X�̍ő�HP     3000
#define BossMaxHP_Short  2000      //�{�X�̍ő�HP     3000
#define MaxFrame		 12000      //�ő�t���[�����i���v���C�ŕۑ��ł��钷���j

#define MAXSTAGE		 8
#define MAXSTAGE_SEQ	 3

const s8 Version = 38;          //�Q�[���̃o�[�W�����i�Ƃ����������[�X�ԍ��j
const s8 ScoreVersion = 29;          //�X�R�A�̃o�[�W����
const s8 RepVersion = 30;          //���v���C�̃o�[�W����
const s8 ConfigVersion = 27;       //�R���t�B�O�t�@�C���̃o�[�W����
const s8 IchVersion = 28;       //�摜�t�@�C���̃o�[�W����
const s8 Stage1Version = 0;     //�X�e�[�W1�f�[�^�̃o�[�W����
const s8 Stage2Version = 0;     //�X�e�[�W2�f�[�^�̃o�[�W����
const s8 Stage99Version = 0;     //�X�e�[�W99�f�[�^�̃o�[�W����

const string BGMTitle[] = {"murderous desire","murderous desire ; fyu-kosay mix","murderous desire ; at-nen mix"} ;

const string DEBUG_GRAPH_PATH = "c:/data/graphics/gameproject/";

const string NETRANKING_CGI_PATH = "http://www.dwahan.net/uki/cloudphobia/ranking/v028/";
const string NETRANKING_SEQUENCE_CGI_PATH = "http://www.dwahan.net/uki/cloudphobia/ranking/v029/";
//const string NETRANKING_CGI_PATH = "http://localhost/";

const bool DebugMode = false;
const bool TrialMode = false;

const f32 PI = (f32)M_PI;

#define MAXVALUES  8

//���ʉ�
#define WAGraze		  0
#define WALdamage	  1
#define WAHdamage	  2
#define WAGunshot	  3
#define WAMissile	  4
#define WAExp_S		  5
#define WAExp_M		  6
#define WAEdamage0	  7
#define WAExp_L		  8
#define WALock		  9
#define WAWArning	 10
#define WAVScroll	 11
#define WAZ5app		 12
#define WABossExp	 13
#define WABoost		 14
#define WAEBoost	 15
#define WACharge	 16
#define WABeam		 17
#define WAEdamage1	 18
#define WAFunMove	 19
#define WAFunAppear	20
#define WALBeam		 21
#define WAHoming	 22
#define WAHoming2	 23
#define WABShot		 24
#define WATrans		 25
#define WAStart		 26
#define WAZ4app		 27
#define WAheart		 28
#define WADeside	 29
#define WASelect	 30
#define WABonus		 31
#define WABoot		 32
#define WASlash_A	 33
#define WASlash_B	 34
#define WASlash_C	 35
#define WASwing_A	 36
#define WASwing_B	 37
#define WAAlarm		 38
#define WABaseExp	 39
#define WAEngine	 40
#define WAFlash		 41
#define WAShutter_close		42
#define WAShutter_fullopen  43
#define WAShutter_open		44
#define WAShutter_unlock	45
#define WASpydereye  46
#define WASwing_E	 47
#define WATank_dash  48
#define WATank_jump  49
#define WATank_shot  50
#define WAThunder		51
#define WAWarp			52
#define WAWarpout		53
#define WAWarp2			54
#define WAShipCharge    55
#define WASubmarine     56
#define WAShipExp		57
#define WABoss2Laser    58
#define WAShipDamaged	59

#define NCube		30

#define HitBody  1
#define HitBuzz  0
#define HitBlade  17

//�f�o�b�O���ݒ���
#define KDEBUGINFO_FAKEHDR		 0
#define KDEBUGINFO_ENVMAP		 1
#define KDEBUGINFO_EXPLOSION	 2

//�f���ԍ�
#define KDEMONUM_TUTORIAL		 0		//�`���[�g���A��
#define KDEMONUM_STAGE1START	 1		//�X�e�[�W�P�J���f��
#define KDEMONUM_STAGE2START	 2		//�X�e�[�W2�J���f��
#define KDEMONUM_STAGE3START	 3		//�X�e�[�W3�J���f��
#define KDEMONUM_STAGE4START	 4
#define KDEMONUM_STAGE2END		 5


//�X�v���C�g�f�[�^
#define KSPRDATA_STAGE2BOSS1     0
#define KSPRDATA_STAGE2GUARD     1
#define KSPRDATA_S2GUARDBWS     2
#define KSPRDATA_STAGE2BOSS2     3
#define KSPRDATA_STAGE2ZAKO4     4
#define KSPRDATA_STAGE3BOSS1     5
#define KSPRDATA_STAGE4BOSS2     6
#define KSPRDATA_STAGE4GUARD     7
#define KSPRDATA_STAGE5BOSS1     8
#define KSPRDATA_STAGE5BOSS2     9
#define KSPRDATA_STAGE5BOSS3    10
#define KSPRDATA_FRIEND    11
#define KSPRDATA_STAGE5BOSS2_2     12

//���[�V����
#define KMOTION_STAGE2BOSS1_DEFAULT    0
#define KMOTION_STAGE2BOSS1_APPEAR1    1
#define KMOTION_STAGE2BOSS1_APPEAR2    2
#define KMOTION_STAGE2BOSS1_FOWARD    3
#define KMOTION_STAGE2BOSS1_BACK      4
#define KMOTION_STAGE2BOSS1_CHARGE    5
#define KMOTION_STAGE2BOSS1_BERSERK    6
#define KMOTION_STAGE2BOSS1_BLADEPREPARE1    7
#define KMOTION_STAGE2BOSS1_BLADEPREPARE2    8
#define KMOTION_STAGE2BOSS1_SLASH    9
#define KMOTION_STAGE2BOSS1_BEAM    10
#define KMOTION_STAGE2BOSS1_BERSERK2    11
#define KMOTION_STAGE2BOSS1_FALL    12

#define KMOTION_STAGE2GUARD_DEFAULT    13
#define KMOTION_STAGE2GUARD_APPEAR1    14
#define KMOTION_STAGE2GUARD_APPEAR2    15
#define KMOTION_STAGE2GUARD_FOWARD    16
#define KMOTION_STAGE2GUARD_BACK      17
#define KMOTION_STAGE2GUARD_SHOT      18
#define KMOTION_STAGE2GUARD_SLASH1    19
#define KMOTION_STAGE2GUARD_SLASH2    20
#define KMOTION_STAGE2GUARD_SLASH1TO2    21

#define KMOTION_STAGE2ZAKO4_DEFAULT   22
#define KMOTION_STAGE2ZAKO4_FOWARD    23
#define KMOTION_STAGE2ZAKO4_BACK      24
#define KMOTION_STAGE2ZAKO4_LEFT      25
#define KMOTION_STAGE2ZAKO4_RIGHT     26

#define KMOTION_STAGE3BOSS_DEFAULT   27
#define KMOTION_STAGE3BOSS_FOWARD    28
#define KMOTION_STAGE3BOSS_BACK      29
#define KMOTION_STAGE3BOSS_SHOT1      30
#define KMOTION_STAGE3BOSS_SHOT2      31
#define KMOTION_STAGE3BOSS_FALL      32

#define KMOTION_STAGE4BOSS_DEFAULT   33
#define KMOTION_STAGE4BOSS_FOWARD    34
#define KMOTION_STAGE4BOSS_BACK      35
#define KMOTION_STAGE4BOSS_SHOT      36
#define KMOTION_STAGE4BOSS_CUTTER1      37
#define KMOTION_STAGE4BOSS_CUTTER2      38
#define KMOTION_STAGE4BOSS_CUTTER3      39

#define KMOTION_STAGE5BOSS1_DEFAULT   40
#define KMOTION_STAGE5BOSS1_SHOT1    41
#define KMOTION_STAGE5BOSS1_SHOT2      42
#define KMOTION_STAGE5BOSS1_MOVE      43
#define KMOTION_STAGE5BOSS1_SITDOWN      44
#define KMOTION_STAGE5BOSS1_JUMP      45
#define KMOTION_STAGE5BOSS1_SHOT3      46
#define KMOTION_STAGE5BOSS1_FALL      47

#define KMOTION_STAGE5BOSS2_DEFAULT   48
#define KMOTION_STAGE5BOSS2_FOWARD      49
#define KMOTION_STAGE5BOSS2_BACK      50
#define KMOTION_STAGE5BOSS2_SLASHL1      51
#define KMOTION_STAGE5BOSS2_SLASHL2      52
#define KMOTION_STAGE5BOSS2_SLASHL1_2      53
#define KMOTION_STAGE5BOSS2_SLASHR1      54
#define KMOTION_STAGE5BOSS2_SLASHR1_2      55
#define KMOTION_STAGE5BOSS2_SLASHR2      56
#define KMOTION_STAGE5BOSS2_LEAD      57
#define KMOTION_STAGE5BOSS2_FALL1      58
#define KMOTION_STAGE5BOSS2_FALL2      59

#define KMOTION_STAGE5BOSS3_DEFAULT   60
#define KMOTION_STAGE5BOSS3_FOWARD      61
#define KMOTION_STAGE5BOSS3_BACK      62
#define KMOTION_STAGE5BOSS3_CHARGE      63
#define KMOTION_STAGE5BOSS3_BEAM      64
#define KMOTION_STAGE5BOSS3_BEAM2      65

#define KMOTION_FRIEND_SWING1      66
#define KMOTION_FRIEND_SWING2      67
#define KMOTION_FRIEND_SWING3      68

#define KMOTION_STAGE5BOSS2_2_DEFAULT   69
#define KMOTION_STAGE5BOSS2_2_FOWARD      70
#define KMOTION_STAGE5BOSS2_2_BACK      71
#define KMOTION_STAGE5BOSS2_2_SLASHL1      72
#define KMOTION_STAGE5BOSS2_2_SLASHL2      73
#define KMOTION_STAGE5BOSS2_2_SLASHL1_2      74
#define KMOTION_STAGE5BOSS2_2_SLASHR1      75
#define KMOTION_STAGE5BOSS2_2_SLASHR1_2      76
#define KMOTION_STAGE5BOSS2_2_SLASHR2      77
#define KMOTION_STAGE5BOSS2_2_LEAD      78
#define KMOTION_STAGE5BOSS2_2_FALL1      79
#define KMOTION_STAGE5BOSS2_2_FALL2      80

//�X�N���[���X�s�[�h
#define KSCROLLSPEED_NORMAL    5

//�X�N���v�g
#define KSCRIPT_TUTORIAL      0
#define KSCRIPT_STAGE2ENEMY      1
#define KSCRIPT_NIGHTVISION      2
#define KSCRIPT_STAGE3ENEMY      3
#define KSCRIPT_STAGE4ENEMY      4
#define KSCRIPT_STAGE5ENEMY      5
#define KSCRIPT_STAGE2DEMO      6
#define KSCRIPT_STAGE5DEMO      7
#define KSCRIPT_STAGE2ENEMY_EASY      8
#define KSCRIPT_STAGE3ENEMY_EASY      9
#define KSCRIPT_STAGE4ENEMY_EASY     10
#define KSCRIPT_STAGE2ENEMY_NORMAL      11
#define KSCRIPT_STAGE3ENEMY_NORMAL      12
#define KSCRIPT_STAGE4ENEMY_NORMAL     13
#define KSCRIPT_STAGE2ENEMY_HARD      14
#define KSCRIPT_STAGE3ENEMY_HARD      15
#define KSCRIPT_STAGE4ENEMY_HARD      16

#define KIDOFFSET_BILLBOARD     256    //�r���{�[�h�Ɋ���U�郁�b�V���h�c�̏����l
#define KIDOFFSET_EFFECT     512    //����G�t�F�N�g�Ɋ���U�郁�b�V���h�c�̏����l

//��Փx
 #define  KDIFFICULTY_EASY    0
 #define  KDIFFICULTY_NORMAL  1
 #define  KDIFFICULTY_HARD    2
 #define  KDIFFICULTY_SINGLE  3

//�I�u�W�F�N�g�̎��
enum TOBJKind {
	kHero,			// ���@ 
	kHShot,			// ���e 
	kEnemy,			// �G
	kEShot,			// �G�e
	kEffect,		// �G�t�F�N�g
	kMissile,		// �~�T�C��
	kEMissile,		// �G�~�T�C��
	kGauge ,		// �v���
	kBG ,			// �w�i
	kGene  ,		// �G����
	kEffect2,		// �X�N���[���̉e�����󂯂Ȃ��G�t�F�N�g
	kEffect3,       // �X�N���[���̉e���͎󂯂Ȃ����U���̉e���͎󂯂�        
	kEffect4,       // �U���̉e���͎󂯂Ȃ����X�N���[���̉e�����󂯂�              
	kFunnel,		// �t�@���l��
	kColBG			// �����蔻����������w�i
};

//�Q�[���̏�� 
enum TGameMode {
	gTitle  ,	// �^�C�g��
	gPlay,		// �{�� 
	gPause ,	// �|�[�Y
	gOver ,     // �Q�[���I�[�o�[
	gReady   ,  // �J�n�O���o
	gOther		// ���̑�
};

//���C�����[�v�ɃC�x���g�v��    �@ �@                                
enum TMainLoopEvent {
	MNone,			// �Ȃ� �@
	MPlayStart,		// �Q�[���J�n
	MRestart  ,		// ���X�^�[�g
	MTitle,         // �^�C�g���ɖ߂� 
	MPause,			// �X�g�b�v
	MMove ,			// �X�g�b�v����
	MRepView,		// ���v���C�ꗗ��
	MWeaponSelect,  // ����I����� 
	MReady  ,		// Ready���
	MLoadBeforePlay,    // �X�e�[�W�J�n�O�̓ǂݍ���     
	MOpeningMovie,	 // �I�[�v�j���O���[�r�[
	MEndingMovie,	 // �G���f�B���O���[�r�[ 
	MRestart_Seq	//���X�^�[�g�i�ʂ��ʁj
};

//�t�@���l���Ƃ̘A�g    �@�@           
enum TFunSignal {
	fNone,			// �w���Ȃ�
	fDisperse,		// ���U
	fRoll,			// ��]
	fNarrow,		// ���������� 
	fSelfDes		// ����
};

//�Q�[���X�s�[�h    
enum TGameSpeed {
	sSlow,  
	sNormal,
	sFast
};


class TJoint;
class TFrameManager;
class CEffectBase;
class TDDSD;
class TBitmap;
class TSpriteEXManager;
class TSceneManager;
struct THitArea;

struct OBJParts //���낢��f�Зp���R�[�h
{
	s32 X;
	s32 Y;
	s32 Rdeg;
};

struct DebugData 
{
  string FCAPTION[MAXVALUES];
  f32 FMAX[MAXVALUES];
  f32 FMIN[MAXVALUES];
  f32 FCURRENT[MAXVALUES];
};

#define MAX_NAME_LENGTH		8
struct NetRankingData 
{
  s32 Rank;
  s32 Name[MAX_NAME_LENGTH];
  s32 Score;
  s32 Time;
  s32 BoostType;
  s32 WeaponType;
  s32 FrameSkipRate;
  s32 Year;
  s32 Month;
  s32 Day;
  s32 FileNumber;
};

struct NetRankingData_Split 
{
  s32 SplitScore[5];
  s32 SplitTime[5];

  s32 Life[5];
  s32 MShip[5];
  s32 Missile[5];
};

struct TSpriteData //�X�v���C�g�f�[�^
{
	TJoint* Spr;      //�X�v���C�g�̃|�C���^�i��ʕ\���p�Ɏg���B�t�@�C���ۑ��͂��Ȃ��j
	s32 ParentID;   //�e�X�v���C�gID
	string Name;        //���O
	Rect TexRect;      //�e�N�X�`�����W
	TPoint HitRect[4];     //�����蔻��
	s32 CX;
	s32 CY;      //��]���S���W
	s32 X;
	s32 Y;        //�ʒu
	s32 Rdeg;       //��]�p
	s32 Z;          //�y�l
};

struct TSpriteDataList //�X�v���C�g�f�[�^���X�g
{
	s32		 TotalSpr;   //�S�X�v���C�g��
	string	 BmpFile;     //�r�b�g�}�b�v�t�@�C��
	vector<TSpriteData> Sprites;
};

struct TPartsMotionData //���[�V�����f�[�^
{
	TJoint* Spr;      //�X�v���C�g�̃|�C���^�i��ʕ\���p�Ɏg���B�t�@�C���ۑ��͂��Ȃ��j
	string Name;        //���O
	s32 X;
	s32 Y;        //�ʒu
	s32 Rdeg;       //��]�p
	s32 Speed;          //�X�s�[�h
	s32 Direction;    //��]����
};

struct TMotionData //���[�V�����f�[�^
{
	s32 TotalSpr;   //�S�X�v���C�g��
	string Name;        //���[�V�����̖��O
	vector<TPartsMotionData> Sprites;
};

struct MeshData //���b�V���f�[�^
{
	string	Name;     //���ʖ�
	string	FileName;  //�t�@�C����
	s32		TextureID; //�e�N�X�`���h�c
	TSXMesh*	Mesh; //���b�V��
	s32		BlendMode;  //�u�����h���[�h
	bool	Lighting; //���C�e�B���O�L����
};

struct BBData //�r���{�[�h�f�[�^
{
	string Name;     //���ʖ�
	s32 TextureID; //�e�N�X�`���h�c
	s32 u;
	s32 v;
	s32 width;
	s32 height;
	s32 BlendMode;  //�u�����h���[�h
};

struct FrameData //�t���[���f�[�^
{
	s32		MeshID; //���b�V���h�c
	TFrameManager*	FrameMng; //�t���[��
	s32		AppearDistance;  //�o���ʒu
	s32		DisappearDistance;  //���ňʒu
	bool	Freeze;			//�J�����ʒu�ɌŒ肷�邩
	CEffectBase*	EffectPtr;		//����G�t�F�N�g���g���ꍇ�A���̃C���X�^���X�ւ̃|�C���^
	s32		HideDistance;   //�G�f�B�^��Ŕ�\���ɂ���ʒu
	bool	IsAppeared;
	bool	IsRendered;
	bool	IsDeleted;
	f32		ZX;
	f32		ZY;
	f32		ZZ;
	f32		X;
	f32		Y;
	f32		Z;
	s32		RX;
	s32		RY;
	s32		RZ;

	s32		numParam;   //�p�����^���ڐ�
	vector<string>	ParamName;
	vector<f32>		ParamVal;
};

struct ViewData //���_�f�[�^
{
	s32		Distance;
	//�J����
	f32		X;
	f32		Y;
	f32		Z;       //�ʒu
	s32		RX;
	s32		RY;
	s32		RZ;   //�p�x
	//  SpeedX,SpeedY,SpeedZ:Single;                //�ړ��X�s�[�h
	f32		Speed;
	bool	Spline;   //�X�v���C���Ȑ����[�h
};

struct MaterialData //�}�e���A���f�[�^
{
	f32		Diffuse;
	f32		Specular;
	f32		Ambient;
	f32		Emissive;
	f32		Power;
	f32		u;
	f32		v;
};

struct EffectData //���b�V���f�[�^
{
	string	Name;     //���ʖ�
	s32		TextureID; //�e�N�X�`���h�c
	bool	Lighting; //���C�e�B���O�L����
	s32		BlendMode;  //�u�����h���[�h
	s32		numParam;   //�p�����^���ڐ�
	
	vector<string>	ParamName;
	vector<f32>		ParamVal;
};


//TAstat = packed record
//  adapt: TAdapterStatus;
//  NameBuff: array [0 .. 29] of TNameBuffer;
//end;
//


s32		NewID = 0;
u32		VFPSCount;

//�C���^�[�t�F�[�X
TDGCarad*	QD;
TDDSD*		SD;

//�e�N�X�`��
//GaugeTex:TDDDDTexture;    //�Q�[�W
TDGTexture		*JikiTex;          //���@
TDGTexture		*FontTex, *FontTex_Gothic;            //�t�H���g
TDGTexture		*Eff1Tex, *Eff2Tex, *Eff3Tex, *Eff4Tex, *Eff5Tex, *Eff6Tex, *Eff7Tex, *Eff8Tex, *Eff9Tex;//�G�t�F�N�g
TDGTexture		*MechaTex;     //�G
TDGTexture		*Enemy4Tex, *Enemy5Tex;
//BGSource:TDGTexture;
//BGTex:TDGTexture;
TDGTexture		*BGTex1, *BGTex2;     //�w�i
TDGTexture		*ST2Tex1, *ST2Tex2;     //�X�e�[�W�Q�e�N�X�`��
TDGTexture*		BumpTex;
TDGTexture*		BossBump;
TDGTexture*		GaugeTex;
TDGTexture*		SunTex;
TDGTexture*		DemoTex;   //�f���p�e�N�X�`��
TDGTexture*		MissileTex;

TDGTexture*		BShotTex1, *BShotTex2; //�o�X�g�V���b�g�p�e�N�X�`��
TDGTexture*		BShotBumpTex1, *BShotBumpTex2; //�o�X�g�V���b�g�p�e�N�X�`���i�o���v�j

//���b�V��
TSXMesh* Board;        //��
TSXMesh* CloudMesh;       //
TSXMesh* Undersea, *Undersea2;     //
TSXMesh* Building[NCube];       //�r��
TSXMesh* Rock[NCube];       //��
TSXMesh* GCube[NCube];       //�K���X�L���[�u
TSXMesh* MissileMesh;

//�w�i�p�T�[�t�F�X
//BGSurface:TDDDDSurface;

//�{�X�e�N�X�`���ۑ��T�[�t�F�X
//TBitmap*		BossBmp;
//TBitmap*		BossBumpBmp;


// �X�v���C�g�}�l�[�W��
//TSpriteEXManager*	SprMan;

//�|���S���}�l�[�W��
//TSceneManager*		Scene;

//�t���X�N���[���ɂ��Ȃ����ׂ���
bool	FullScreenOK;

bool	ButtonFlg[16];           //�{�^���������ꂽ��
s32		AssinedKey[16];         //�L�[�A�T�C���ύX��̃{�^���̔ԍ�
bool	ButtonON[16];          //Joy.Buttons�̑��

#define				LOCAL_RANKING_SIZE		10

NetRankingData		ScoreRanking_local[MAXSTAGE][LOCAL_RANKING_SIZE];
NetRankingData		TimeRanking_local[MAXSTAGE][LOCAL_RANKING_SIZE];
NetRankingData		ScoreRanking_Seq_local[MAXSTAGE_SEQ][LOCAL_RANKING_SIZE];
NetRankingData		TimeRanking_Seq_local[MAXSTAGE_SEQ][LOCAL_RANKING_SIZE];
NetRankingData_Split	ScoreRanking_Split_local[MAXSTAGE_SEQ][LOCAL_RANKING_SIZE];
NetRankingData_Split	TimeRanking_Split_local[MAXSTAGE_SEQ][LOCAL_RANKING_SIZE];

s32		BoostLV;              //�u�[�X�g���x��

//�R���t�B�O���
s8		BoostMode;            //�u�[�X�g���[�h(0:�I���^�l�C�g�@1:���[�����^���i�����Ă���Ԃ����j)
bool	CloudON;              //�_��\�����邩
bool	FilterON;             //�t�B���^ON/OFF
bool	ScreenMode;           //�X�N���[�����[�h(T:�t���X�N���[���@F:�E�B���h�E)
s32		BGMNumber;            //BGM�ԍ�
bool	ShieldInfinite,MShipInfinite,CollisionON,TimeInfinite,SlowON;
s32		EffLevel,TexCol,FrameRate;
s32		SEVol,bgmVol;
bool	CreateBGM;            //BGM�Đ��N���X�𐶐����邩
bool	CreateSE;            //SE�Đ��N���X�𐶐����邩
s32		ScreenResolution;     //��ʉ𑜓x
bool	ExtraEffects;         //�z���Ȃǂ̃G�t�F�N�g
bool	Blooming;
bool	LuminanceMap;
bool	ConnectNet;

//���O��ON/OFF
bool	KPUTLOG;


//�X�v���C�g�`�揇������p�ϐ�
s32		KageExpZ;
s32		BeamZ;
s32		SmokeZ;
s32		ExpZ;
s32		ClExpZ;
s32		TamaZ;
s32		SplashZ;

//�f�o�b�O�p���
vector<DebugData>	DData;

//�f�o�b�O���O�i�[�z��
vector<string>		DebugLogArray;
string	LastMovedOBJ;

s32		HPMax;          //���@�̍ő�HP     7000
s32		MSHPMax;       //��͂̍ő�HP

glm::vec2 Rot2D( glm::vec2& _vec, s32 _r );

void PosRoll( f32& _x, f32& _y, f32 _cx, f32 _cy, s32 _R );
string InttoStr( s32 _val );
string TimeToStr( s32 _time );
bool Equal( s32 _int, s32 _num );
void InitSprZ();
string ReadSegment( string _s, s32 _n );
s32 GetDiffR( s32 _r1, s32 _r2 );
void PutDebugLog( string _msg );
void WriteDebugLog();
void InitDebugLog();
bool CheckDistance( s32 X1, s32 Y1, s32 X2, s32 Y2, s32 dist );
s32 GetRfromArcTan( f32 _x, f32 _y, f32 _dx, f32 _dy );
glm::vec2 GetReflectionVector( glm::vec2& _vec, THitArea* _hitarea );
string GetMacAddress();
string TotalTimeToStr( s32 _time );
s32	Sign(s32 val);
u32 random(u32 _max);

inline DWORD F2DW( FLOAT f ) { return *((DWORD*)&f); }

void GetDate( s32& _year, s32& _mon, s32& _day );
s32 BoolToInt(bool _val);
bool IntToBool(s32 _val);
glm::vec2 Random2D(f32 k);


