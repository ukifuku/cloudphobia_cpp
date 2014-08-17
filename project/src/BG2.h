#pragma once
#include "TOBJ.h"
#include "TOBJList.h"

//朝焼け空
class TDawn : public TOBJ
{
  protected:
//    MeshFrame:array [0..15] of TFrameManager;
//    rx,ry,rz:array [0..15] of Integer;
//    s:Single;
public:
    TDawn(TOBJList* _owner);
    ~TDawn();
    void Move();  
    s32 OnDraw(); 
};

//海
class TSea : public TOBJ
{
  protected:
    TFrameManager* MeshFrame; //メッシュを格納するフレーム
    f32 	PosX;
    f32		PosZ;
  public:
    TSea(TOBJList* _owner, s32 _x, s32 _z);
    ~TSea();
    void Move();
};

//海（2層目）
class TUnderSea : public TOBJ
{
protected:
   TFrameManager* MeshFrame[3]; //メッシュを格納するフレーム
public:
    TUnderSea(TOBJList* _owner);
    ~TUnderSea(); 
    void Move();
};

//朝焼け雲
class TDawncloud : public TOBJ
{
  protected:
 TFrameManager*   MeshFrame; //メッシュを格納するフレーム
  public:
    TDawncloud(TOBJList* _owner);
    ~TDawncloud(); 
    void Move();    
};

//フォグ
class TFog : public TOBJ
{
  protected:
    f32 Zoomrate;
  public:
    TFog(TOBJList* _owner);
    void Move();    
    s32 OnDraw();
};

//太陽
class TDawnSun : public TOBJ
{
  protected:
    f32 PosX;
	f32 PosY;
public:
    TDawnSun(TOBJList* _owner);
    void Move();    
    s32 OnDraw();
};

//太陽の反射
class TDawnSunReflection : public TOBJ
{
  protected:
	    public:
    TDawnSunReflection(TOBJList* _owner);
    void Move();    
    s32 OnDraw();
};

//ビル（基本）
class TBuildingBase : public TOBJ
{
  protected:
    TFrameManager* MeshFrame;
    f32 Speed;
  public:
    TBuildingBase(TOBJList* _owner, f32 _Z, s32 _rx, s32 _ry, s32 _rz, f32 _zoom );
    ~TBuildingBase(); 
    void Move();    
};
                                   
//ビル影
class TBuildingShadow : public TOBJ
{
  protected:
    TFrameManager* MeshFrame;
    f32 Speed;
  public:
    TBuildingShadow(TOBJList* _owner, f32 _Z, s32 _rx, s32 _ry, s32 _rz, f32 _zoom );
    ~TBuildingShadow();
    void Move();    
};

//ビルメーカー
class TBuildingMaker : public TOBJ
{
  protected:
   f32 Z;
   s32 RX;
  s32 RY;
   s32 RZ;
   f32 Zoom;
   s32 interval;
  public:
    TBuildingMaker(TOBJList* _owner);
    void Move();
};

//岩
class TRockBase : public TOBJ
{
  protected:
    TFrameManager* MeshFrame;
    f32 Speed;
  public:
    TRockBase(TOBJList* _owner, f32 _Z, s32 _rx, s32 _ry, s32 _rz, f32 _zoom );
    ~TRockBase(); 
    void Move();    
};

class TRockMaker : public TOBJ
{
  protected:
    f32 Z;
    s32 RX;
    s32 RY;
    s32 RZ;
    f32 Zoom;
    s32 interval;
  public:
    TRockMaker(TOBJList* _owner);
    void Move();    
};


//青空
class TSky_new : public TOBJ
{
  protected:
  public:
    TSky_new(TOBJList* _owner);
    void Move();    
    s32 OnDraw(); 
};

//雲海
class TCloud_new : public TOBJ
{
  protected:
    TFrameManager* MeshFrame; //メッシュを格納するフレーム
    TFrameManager* SonMesh[8];
public:
    TCloud_new(TOBJList* _owner);
    ~TCloud_new();
    void Move();    
};

//ビル遠景1
class TBuilding_Far1 : public TOBJ
{
  protected:
    s32 BGType;
  public:
    TBuilding_Far1(TOBJList* _owner, s32 _x, s32 _type);
    void Move();    
    s32 OnDraw();
};

//ガラスキューブ
class TGlassCubeBase : public TOBJ
{
  protected:
    s32 RX;
    s32 RY;
    s32 RZ;
    bool Appeared;
  public:
	  TFrameManager* MeshFrame;
	  TGlassCubeBase(TOBJList* _owner, TFrameManager* _parent, f32 _X, f32 _Y, f32 _Z, s32 _rx, s32 _ry, s32 _rz, f32 _zoom );
    ~TGlassCubeBase();
    void Move();    
    void Die(); 
};

class TGlassCubeMaker : public TOBJ
{
  protected:
//    Z:Single;
    s32 RX;
    s32 RY;
    s32 RZ;
    f32 fZoom;
    TFrameManager* MeshFrame;
    TFrameManager* CenterFrame;
    vector<TGlassCubeBase*> Cubes;
    s32 Speed;
    bool EndFlg;
    void Input();
  public:
    TGlassCubeMaker(TOBJList* _owner, s32 _pat);
    ~TGlassCubeMaker(); 
    void Move();    
    void Die(); 
    void EndProc();
};

