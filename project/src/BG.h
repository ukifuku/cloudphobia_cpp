#pragma once

#include "TOBJ.h"


class TRainFilter;

//空（システムメモリに生成）
class TSky : public TOBJ
{
protected:
	Rect	m_BltRect;
public:
	TSky(TOBJList* _owner);
};

//空2（ビデオボードのVRAMのみを使用）
class TSky2 : public TOBJ
{
public:
	TSky2(TOBJList* _owner);
	void Move();
};

//雲
class TCloud : public TOBJ
{
protected:
	s32	ZValue;
public:
	TCloud(TOBJList* _owner, s32 _x, s32 _y, s32 _z );
	void Move();
};

//雲2
class TCloud2 : public TOBJ
{
protected:
	s32	Pattern;
	s32	Speed;
	u8	Alpha;
public:
	TCloud2(TOBJList* _owner, s32 _x, s32 _y, s32 _pat, u8 _alpha, s32 _x1, s32 _y1, s32 _x2, s32 _y2);
	void Move(); 
};

//雲3
class TCloud3 : public TOBJ
{	
protected:
	s32	Pattern;
	s32	Speed;
	u8	Alpha;
	TOBJ*	OwnerOBJ;
public:
	TCloud3( TOBJList* _owner, s32 _x, s32 _y, s32 _pat, u8 _alpha, s32 x1, s32 y1, s32 x2, s32 y2, TOBJ* _obj);
	void Move(); 
};

//雲生成
class TCloudMaker : public TOBJ
{
public:
	f32	Cdistance[6];
	s32	CloudYPos[6];
public:
	TCloudMaker( TOBJList* _owner, s32 _y );
	void Move();   
};

//雲生成（画面全体を覆う）
class TCloudMaker2 : public TOBJ
{
protected:
	Rect	TexRect[96];
	s32	Cdistance[2];
	s32	TotalDist;
	s32	CloudFlg[2];
	s32	EndDistance;
	u8	Alpha;
public:
	TCloudMaker2( TOBJList* _owner );
	void	Move();
};

//雲生成（部分雲）
class	TCloudMaker3 : public TOBJ
{
public:
	TCloudMaker3(TOBJList* _owner, s32 _x, s32 _y);
	void	Move();    
};

//雲生成（前半の背景のアッパーバージョン）
class	TCloudMaker4 : public TOBJ
{
protected:
	s32 Cdistance[9];
	s32	TotalDist;
	Rect	TexRect[96];
	s32	CloudFlg[9];
	u16	Alpha;
public:
	TCloudMaker4(TOBJList* _owner, s32 _y);
	void	Move();   
};

//太陽
class TSun : TOBJ
{
protected:
	s32	ActFlg;
	s32	ActCount;
	s32	EndDistance;
public:
	TSun(TOBJList* _owner, s32 _x, s32 _y);
	void 	Move();   
	void 	OnDamaged(TOBJ& _target);
};

//月
class TMoon : public TOBJ
{
protected:
	s32	ActFlg;
	s32	ActCount;
public:
	TMoon(TOBJList* _owner, s32 _x, s32 _y);
	void Move();
};

//レンズフレア
class TFlare : public TOBJ
{
public:
	TFlare(TOBJList* _owner, s32 _x, s32 _y, f32 _a);
	void 	Move();
};


//縦スクロールイベント(上昇)
class TVScroll : public TOBJ
{
protected:
	s32	ActFlg;
	s32	ActCount;
	s32	TotalFlight;
	b8	ClSet[4];
public:
	TVScroll(TOBJList* _owner);
	void Move();  
};

//縦スクロールイベント（下降）
class	TDownScroll : public TOBJ
{
private:
	s32	ActFlg;
	s32	ActCount;
	s32	TotalFlight;
public:
	TDownScroll(TOBJList* _owner);
	void	Move();
};

//太陽リメイク
class	TSun2 : public TOBJ
{
private: 
	s32	ActFlg;
	s32	ActCount;
	TRainFilter*	RainFilter;
public:
	TSun2(TOBJList* _owner, s32 _x, s32 _y);
	void Move();
	void Die();
};

//雲（高速）
class	TCloudFast : public TOBJ
{
private:
	s32	PersonalR;
	s32	IncR;
	f32	ZoomRate;
public:
	TCloudFast(TOBJList* _owner, s32 _x, s32 _y);
	void Move();
};

//砂嵐
class	TSandStorm : public TOBJ
{
public:
	TSandStorm(TOBJList* _owner, s32 _x, s32 _y);
	void Move();    
};

//空（ステージセレクト画面用）
class	TSky_StageSelect : public TOBJ
{
public:
	TSky_StageSelect(TOBJList* _owner);
	void Move();    
};

//ステージ5音楽制御
class	TStage5MusicControl : public TOBJ
{
public:
	TStage5MusicControl(TOBJList* _owner);
	void Move();    
};



