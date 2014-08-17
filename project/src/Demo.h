#pragma once

#include "types.h"
#include "TOBJ.h"
#include <vector>

class TDGTexture;
class TSpriteEX;
class TOBJList;

//デモ管理クラス
class TDemoManager : public TOBJ
{
private:
	vector<TDGTexture*> Textures;
	vector<TSpriteEX*> SpriteList;
    s32 DemoID;
    s32 ScriptID;
    bool Start;
    void LoadTexture();
    void Input();
public:
    TDemoManager(TOBJList* owner, s32 _id);
    ~TDemoManager();
    void Die(); 
    void Move();    
    void MakeSprite(s32 _num, s32 _tex, s32 _x, s32 _y, s32 _z, s32 _w, s32 _h, s32 _tx, s32 _ty, s32 _tw, s32 _th, s32 _cx, s32 _cy, s32 _blend);
    void SprMove(s32 _id, f32 _x, f32 _y );
    void SprMoveR(s32 _id, f32 _x, f32 _y);
    void SprSlideMoveR(s32 _id, f32 _x, f32 _y);
    void SprRotate(s32 _id, s32 _r);
    void SprRotateR(s32 _id, s32 _r);
    void SprZoom(s32 _id, f32 _x, f32 _y);
    void SprZoomR(s32 _id, f32 _x, f32 _y);
    void SprColor(s32 _id, s32 _col);
    void SprColorR(s32 _id, s32 _col);
    void SprAlpha(s32 _id, s32 _col);
    void SprAlphaR(s32 _id, s32 _col);
    void DemoStart();
    void SprRelease(s32 _id);
    s32 OnDraw(); 
};


//ムービー管理クラス
class TMovieManager : public TOBJ
{
private:
    s32 MovieID;
    bool StopRequested;
    void Input();
public:
    TMovieManager(TOBJList* owner, s32 _id);
    ~TMovieManager();
    void Move();    
};

//エンディング開始処理
class TEndingStarter : public TOBJ
{
private:

public:
    TEndingStarter(TOBJList* owner);
    void Move();    
};
  
