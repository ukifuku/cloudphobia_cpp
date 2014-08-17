#pragma once

#include "types.h"
#include "glm/glm.hpp"
#include "SXLib9.h"
#include "Define.h"

struct 	UVRect  //uvレコード
{
  f32 u1,v1,u2,v2;
};


//3Dシーンクラス
class TSceneManager {
private:
    TSXFrame*	Effector;  //エフェクト用フレーム
    TSXScene*	Scene;
    TSXLightGroup*	Lights; //ライト管理オブジェクト
    s32	RX,RY,RZ;                 //カメラ角度
    s32	FScreenX,FScreenY;  //バックバッファのサイズ
    bool	FScaleCHFlg;        //スケーリング行列が使われたか
    TDGTexture*	FExEffectTexture;  //特殊エフェクト用テクスチャ
    IDirect3DSurface9*	FExRendTarget; //特殊エフェクト用レンダリングターゲット
    f32		Ratio2Dto3D_X,Ratio2Dto3D_Y;
    f32		Zval2Dto3D;
    f32		FOV;
    f32		ClipNear;
public:
    TSXFrame*	Root;      //全てのフレームの親になるフレーム
    TDGCarad*	FOwner;
    TSXFrame*	Camera;    //カメラの位置・姿勢を示すフレーム
    TSXFrame*	CameraStand;   //カメラスタンド（カメラの親フレーム）
    TSXFrame*	FogFrame;    //  フォグをきかせたいときはこのフレームに納める
    TSXLight*	GlobalLight;
    TSceneManager(TDGCarad* owner);
    ~TSceneManager();
    TSXFrame* AddFrame(bool _effect);
	TSXScene* GetScene() { return Scene; }
    void Move(TSXFrame* frame, f32 _x, f32 _y, f32 _z);
    void MoveR(TSXFrame* frame, f32 _x, f32 _y, f32 _z);
    void RotateX(TSXFrame* frame, s32 _R);
    void RotateY(TSXFrame* frame, s32 _R);
    void RotateZ(TSXFrame* frame, s32 _R);
    void ZoomR(TSXFrame* frame, f32 _x, f32 _y, f32 _z);
    void CameraRotate(s32 _x, s32 _y, s32 _z);
    void CameraRotateR(s32 _x, s32 _y, s32 _z);
    void CameraMove(f32 _x, f32 _y, f32 _z);
    void CameraMoveR(f32 _x, f32 _y, f32 _z);
    void SetSpriteSX(f32 _x, f32 _y, s32 _width, s32 _height, f32 _z, UVRect& _uv, u8 _alpha, TDGTexture* _tex, TSXBlendMode blend);
    void SetBillBoardSX(f32 _x, f32 _y, f32 _z, s32 _width, s32 _height, UVRect& _uv, u8 _alpha, TDGTexture* _tex, TSXBlendMode blend, bool _scale);
    void SetBillBoardSX2(TSXFrame* _frame, f32 _x, f32 _y, f32 _z, f32 _width, f32 _height, UVRect& _uv, u8 _alpha, TDGTexture* _tex, TSXBlendMode blend, bool _scale, bool _top);
    void Render();
    void RenderEffect();
    void Clear();
    TDGTexture* GetExTexture();
    void SetEffectSprite(f32 _x, f32 _y, s32 _width, s32 _height, UVRect& _uv, u8 _alpha, TDGTexture* _tex, TSXBlendMode blend);

    void ReleaseBeforeReset();
    void InitAfterReset();
    void ResetProjection();
    void SetFog(u32 _col, f32 _start, f32 _end);
    void FogEnable(bool _enable);
    void GetCameraAngle(s32& _rx, s32& _ry, s32& _rz);
    void GetCameraPos(f32& _rx, f32& _ry, f32& _rz);
    bool CheckVisible(TSXFrame* _frame);
    glm::vec3 Get3DPosFrom2D(f32 _x, f32 _y);
    void Calc2Dto3DRatio(f32 _z);
    void SetFOV(s32 _r);
    s32 GetFOV();
    TPoint Get2DPosFrom3D(const glm::vec3& _vec);
    void SetClipNear(f32 _near);
	void GetScreenSize(u32& _X, u32& _y);
};

//フレームクラス
class TFrameManager 
{
private:
    TSceneManager* FOwner;
    s32 RX,RY,RZ;
    f32 ZX,ZY,ZZ;
    u32 clAlpha,clRed,clGreen,clBlue;
public:
    TSXFrame* Frame;
    TFrameManager(TSceneManager* owner, TFrameManager* _parent, TSXMesh* _mesh, TDGTexture* _tex, TSXBlendMode _blend, bool _light, bool _spec, bool _effect);
    ~TFrameManager();
    void Move(f32 _x, f32 _y, f32 _z);
    void MoveR(f32 _x, f32 _y, f32 _z);
    void MoveOnCamera(f32 _x, f32 _y, f32 _z);
    void MoveOnCameraR(f32 _x, f32 _y, f32 _z);
    void Rotate(s32 _RX, s32 _RY, s32 _RZ);
    void RotateR(s32 _RX, s32 _RY, s32 _RZ);
    void Zoom(f32 _x,f32 _y, f32 _z);
    void ZoomR(f32 _x,f32 _y, f32 _z);
    void SetColor(u32 _a, u32 _r, u32 _g, u32 _b);
    void SlideUV(f32 _u, f32 _v);
    void Refract(f32 _s);
    void Rotate2(TFrameManager* _frame, s32 _RX, s32 _RY, s32 _RZ);
    void RotateR2(TFrameManager* _frame, s32 _RX, s32 _RY, s32 _RZ);
    f32 GetX();
    f32 GetY();
    f32 GetZ();
    f32 GetZoomX();
    f32 GetZoomY();
    f32 GetZoomZ();
    s32 GetRotX();
    s32 GetRotY();
    s32 GetRotZ();
    bool Rendered();
};

void QueryVisibility(TSXFrame* Sender, TSXScene* Scene, TDGCarad* DG, TSXVisibility& Visibility);
void QueryZ(TSXFrame* Sender, TSXScene* Scene, TDGCarad* DG, f32& ZValue);

