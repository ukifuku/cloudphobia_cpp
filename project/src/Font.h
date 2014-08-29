#pragma once

using namespace std;

#include "types.h"
#include "TOBJ.h"
#include <vector>

//フォント(一文字分)
class TMoji : public TOBJ {
private:
	s32 pat;  //動作パターン
public:
	TMoji(TOBJList* owner, s32 moji, s32 fx, s32 fy, s32 _cx, s32 _cy, s32 _pat);
	void Move();
};

//フォント2(グレア)
class TMoji_blight : public TOBJ {
private:
	bool Blink;
public:
	TMoji_blight(TOBJList* owner, s32 moji, s32 fx, s32 fy, bool _blink);
	void Move();
};

//フォント3(太ゴシック)
class TMoji_gothic : public TOBJ {
private:
public:
	TMoji_gothic(TOBJList* owner, s32 moji, s32 fx, s32 fy, s32 z);
	void Move();
	void MakeUnit(s32 _moji);
};

//フォント4(数値が頻繁に変わるカウンタ用)
class TMoji_counter : public TOBJ {
private:
public:
	TMoji_counter(TOBJList* owner, s32 fx, s32 fy, s32 z);
	void Move();
	void Update(s32 _val);
};

//フォント5(オレンジ)
class TMoji_orange : public TOBJ {
private:
public:
	TMoji_orange(TOBJList* owner, s32 moji, s32 fx, s32 fy, s32 z);
	void Move();
};


//移動文字列
class TMoveSentence : public TOBJ {
private:
	s32 ActFlg;
	s32 pattern;  //動作パターン
	s32 RDeg;
	s32 IniDis;  //目的地までの距離
	s32 DestX,DestY;//目的地
	string letter;  //表示する文章
	s32 life;   //寿命
public:
	TMoveSentence(TOBJList* owner, const string& moji, s32 sx, s32 sy, s32 dx, s32 dy, s32 _life, s32 _pat);
	void Move();
};

//スプライトのように扱える文字列
class TSprSentence : public TOBJ {
private:
	vector<TOBJ*> MojiList;
	s32 FontKind;
public:
	TSprSentence(TOBJList* owner, const string& moji, s32  sx, s32 sy, s32 brank, s32 font);
	~TSprSentence();
	void Move();
	void Die();
	void SetAlpha(s32 _alpha);
	void SetColor(s32 _red, s32 _green,s32 _blue);
	void SetZ(u16 _rz);
};

class TMojiAndGlassCursor : public TOBJ {
private:
	vector<TOBJ*> SentenceList;
	s32 Pattern;
	s32 XSpeed,YSpeed;   //カーソル移動スピード
	s32 NowX,NowY;   //カーソル現在位置
	s32 DestX,DestY;  //目的地座標
	s32 XSpeedFactor,YSpeedFactor;  //カーソルスピード決定用変数
	s32 DelayTime;          //移動開始を遅らせる時間
	s32 CurrentAlpha,DestAlpha;              //Alpha値
	s32 AlphaSpeed;         //Alpha値変更スピード
	s32 AlphaDelayTime;     //Alpha値変更開始を遅らせる時間
	TPoint DeathPoint;          //この地点に移動すると消滅
	bool Blink;          //点滅
	s32 MaxSprOfCursor;   //カーソル表示に使用しているスプライト数
	s32 SlideDelayCount;
	s32 AlphaDelayCount;
	void Slide();
	void ModulateAlpha();
public:
	TMojiAndGlassCursor(TOBJList* owner, s32 _x, s32 _y, s32 _long, s32 _pat);
	~TMojiAndGlassCursor();
	void Move();
	void Die();
	void AddSentence(const string& _moji, s32 _x, s32 _brank, s32 _red, s32 _green, s32 _blue, s32 _font);
	void AddSentenceRight(const string& _moji, s32 _x, s32 _brank, s32 _red, s32 _green, s32 _blue, s32 _font);
	void SetAlpha(s32 _alpha);
	void SetDest(s32 _x, s32 _y,s32 _delay);
	void SetColor(s32 _red, s32 _green, s32 _blue);
	void SetSpeed(s32 _x, s32 _y);
	void SetDestAlpha(s32 _dest, s32 _speed, s32 _delay);
	void SetDeathPoint(s32 _x, s32 _y);
	void SetBlink(bool _state);
	void BlinkControl();
	void AddLine(s32 _x);
	void SetRank(s32 _rank);
};

//倍率表示
class TMultiple : public TOBJ {
private:
public:
	TMultiple(TOBJList* owner, s32 _mul, s32 _x, s32 _y);
	void Move();
};

//敵破壊スコア表示
class TEnemyScore : public TOBJ {
private:
	TSprSentence*	sentence;
public:
	TEnemyScore(TOBJList* owner, s32 _x, s32 _y, s32 _score);
	void Move();
	void Die();
};

//ヒット数
class THitCount : public TOBJ {
private:
s32 CurrentHit;
public:
	THitCount(TOBJList* owner, s32 _x, s32 _y);
	void Move();
	void Die();
};

//コマンダーボーナス
//class TComBonus : public TOBJ {
//private:
//	TMojiAndGlassCursor* sentence[2];
//public:
//	TComBonus(TOBJList* owner, s32 _score);
//	~TComBonus();
//	void Move();
//	void Die();
//};

//カーソル管理クラス
class TCursorList {
private:
	vector<TMojiAndGlassCursor*>	m_array;
	TOBJList* FOwner;
	s32 	OrgX,OrgY;
public:
	TCursorList(TOBJList* owner);
	~TCursorList();
	void MakeCursor(const string& _caption, s32 _x, s32 _y, s32 _blank, s32 _width, s32 _num, s32 _font, s32 _pat, s32 _pos);
	void MakeCursor_Base(const string& _caption, s32 _x, s32 _y, s32 _blank, s32 _width,  s32 _num, s32 _font, s32 _pat, s32 _pos,  s32 _red,  s32 _green,  s32 _blue,  s32 _alpha);
	void ClearCursors( s32 _x, s32 _y);
	void SetAppearPoint( s32 _x, s32 _y);
	u32	GetSize() { return m_array.size(); }
	TOBJ*	GetItem(s32 _idx) { return m_array[_idx]; }
};

//数値が頻繁に変わるカウンタ用_ゴシック
class TGothic_counter : public TOBJ {
private:
	bool ShowZero ;	//ゼロサプレスするか
	s32 Desit;
public:
	TGothic_counter(TOBJList* owner, s32 fx, s32 fy, s32 z, s32 _desit, bool _showzero);
	void Move();
	void Update(s32 _val);
};

//コマンダーボーナスリメイク
class TComBonus2 : public TOBJ {
private:
	TSprSentence* sentence[2];
public:
	TComBonus2(TOBJList* owner, s32 _x, s32 _y, s32 _base, s32 _mul);
	void Move();
	void Die();
};

//アルファ値
#define		GLASSCURSOR_BLIND_ALPHA		 128
#define		GLASSCURSOR_GLASS_ALPHA		 140
#define		GLASSCURSOR_REFLEX_ALPHA	 180
#define		GLASSCURSOR_PARTITION_ALPHA  180
#define		GLASSCURSOR_FONT_ALPHA		 255

//カーソル表示パターン
#define		GLASSCURSOR_PATTERN_NONE	  0
#define		GLASSCURSOR_PATTERN_BLIND	  1
#define		GLASSCURSOR_PATTERN_GLASS	 2
#define		GLASSCURSOR_PATTERN_LINE	 3
#define		GLASSCURSOR_PATTERN_BLIGHT   4
#define		GLASSCURSOR_PATTERN_BLACK_LEFT   5
#define		GLASSCURSOR_PATTERN_BLACK_RIGHT  6
#define		GLASSCURSOR_PATTERN_BLACK_CENTER  7
#define		GLASSCURSOR_PATTERN_SHADOW   8

//カーソル色
#define		GLASSCURSOR_BLIGHT_RED   190
#define		GLASSCURSOR_BLIGHT_GREEN   90

//フォント種別
#define		KFONTKIND_NORMAL   0
#define		KFONTKIND_BLIGHT   1
#define		KFONTKIND_GOTHIC   2
#define		KFONTKIND_BLINK    3
#define		KFONTKIND_ORANGE   4
#define		KFONTKIND_BLINK2   5

//カーソル表示位置
#define		KCURSOR_POS_LEFT     0
#define		KCURSOR_POS_CENTER   1
#define		KCURSOR_POS_RIGHT    2


static const s8 FONTSET[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e',
			'f','g','h','i','j','k','l','m','n','o','p','q','r','s','t',
			'u','v','w','x','y','z','.',',',':',';','"','+','-','*','/',
			'!','?',' ','^','~','|','_','{'};  //48番目以降は特殊文字指定用のダミー文字

static const s8 FONTSET_ORANGE[] = {'0','1','2','3','4','5','6','7','8','9',
				   'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p',
				   'q','r','s','t','u','v','w','x','y','z','.',',',':',';','\'','+',
				   '-','*','/','!','?',' ','A','B','C','D','E','F','G','H','I','J',
				   'K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
				   '<','>','"','#','$','%','&','(',')','=','~','\\','@'};



#define		KHEROPOS_TOPGAUGE_HIDED  150
#define		KHEROPOS_BOTTOMGAUGE_HIDED  330
#define		KGAUGE_FADESPEED  24
#define		KGAUGE_SHIFTDISTANCE  56
#define		KGAUGE_SHIFTSPEED  4
