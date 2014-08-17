#pragma once
#include "TOBJ.h"
#include "TOBJList.h"

//���Ă���
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

//�C
class TSea : public TOBJ
{
  protected:
    TFrameManager* MeshFrame; //���b�V�����i�[����t���[��
    f32 	PosX;
    f32		PosZ;
  public:
    TSea(TOBJList* _owner, s32 _x, s32 _z);
    ~TSea();
    void Move();
};

//�C�i2�w�ځj
class TUnderSea : public TOBJ
{
protected:
   TFrameManager* MeshFrame[3]; //���b�V�����i�[����t���[��
public:
    TUnderSea(TOBJList* _owner);
    ~TUnderSea(); 
    void Move();
};

//���Ă��_
class TDawncloud : public TOBJ
{
  protected:
 TFrameManager*   MeshFrame; //���b�V�����i�[����t���[��
  public:
    TDawncloud(TOBJList* _owner);
    ~TDawncloud(); 
    void Move();    
};

//�t�H�O
class TFog : public TOBJ
{
  protected:
    f32 Zoomrate;
  public:
    TFog(TOBJList* _owner);
    void Move();    
    s32 OnDraw();
};

//���z
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

//���z�̔���
class TDawnSunReflection : public TOBJ
{
  protected:
	    public:
    TDawnSunReflection(TOBJList* _owner);
    void Move();    
    s32 OnDraw();
};

//�r���i��{�j
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
                                   
//�r���e
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

//�r�����[�J�[
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

//��
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


//��
class TSky_new : public TOBJ
{
  protected:
  public:
    TSky_new(TOBJList* _owner);
    void Move();    
    s32 OnDraw(); 
};

//�_�C
class TCloud_new : public TOBJ
{
  protected:
    TFrameManager* MeshFrame; //���b�V�����i�[����t���[��
    TFrameManager* SonMesh[8];
public:
    TCloud_new(TOBJList* _owner);
    ~TCloud_new();
    void Move();    
};

//�r�����i1
class TBuilding_Far1 : public TOBJ
{
  protected:
    s32 BGType;
  public:
    TBuilding_Far1(TOBJList* _owner, s32 _x, s32 _type);
    void Move();    
    s32 OnDraw();
};

//�K���X�L���[�u
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

