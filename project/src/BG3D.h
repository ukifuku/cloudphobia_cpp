#pragma once

#include <vector>
#include <string>
#include <sstream>
#include "types.h"
#include "DGCarad9.h"
#include "SXLib9.h"
#include "SXScene.h"

class TOBJList;

//ポリライン
class TPolyLine 
{
protected:
    TSceneManager* FOwner;
    s32 FSegment;
    std::vector<glm::vec3> Points;
    f32 Width;
    UVRect uv;
    f32 ZoomRate;
    void InitVertices();
    bool IsUpdated;
public:
    TSXMesh* Mesh;
    TFrameManager* FrameMng;
    TPolyLine(TSceneManager* _owner, TFrameManager* _parent, s32 _segment, f32 _width, TDGTexture* _tex, UVRect& _uv, TSXBlendMode _blend );
    ~TPolyLine();  
    void SetPoint( s32 _num, glm::vec3& _vec );
    void CalcVartices();
    void SetZoomrate( f32 _zoom );
    void SetWidth( f32 _w );
    s32 Segment() { return FSegment; }
};

//ポリライン格納用
struct TPolyLineInfo{
  TPolyLine* pl;
  s32 Kind;   //種別
  bool Used;   //使用中か
};

//3D背景管理クラス
class TBG3DManager 
{
private:
	TOBJList* FOwner;
    std::vector<FrameData*> FrameList;
    std::vector<MeshData*> MeshList;
    std::vector<ViewData*> ViewList;
    std::vector<BBData*> BBList;
    std::vector<MaterialData*> MaterialList;
    std::vector<EffectData*> EffectList;
    std::vector<string> TextureFileName;
    std::vector<TDGTexture*> Textures;

    f32 ScrollSpeed;
    std::vector<TPolyLineInfo> PolyLinePool;
    s32 FTotalObj;
    s32 PrevPos;
    s32 MaxDistance;
    glm::vec3 CameraOffset;
    void CalcCameraAngle( s32 _dest );
    void LoadMesh();
    f32 CalcCatmullRom( f32 _t, f32 _x0, f32 _x1, f32 _x2, f32 _x3 );
    void LoadTextureFromMeshID( s32 _id );
    void LoadMeshFromMeshID( s32 _id );
    void CreatePolyline( s32 _id );
	std::string GetLine(std::istream& _stream);
public:
    TBG3DManager( TOBJList* _owner );
    ~TBG3DManager(); 
    void SetUpMesh( const string& _line );
    void SetUpBB( const string& _line );
    void SetUpEffect( const string& _line );
    void Scroll( s32 _distance );
    void Load( const string& _file );
    void Load_debug( const string& _file );
    void Clear();
    void MakeMeshFrame( s32 _id );
    void MakeTextures();
    void MoveTo();
    void ReflectParam(const FrameData& _framedata);
    void RenderBB();
    void FrameControl();
    void ResetFrameList();
    void SetCameraOffset( f32 _x, f32 _y, f32 _z );

    s32 TotalObj() const { return FTotalObj; }
    TPolyLine* GetPolyLine( s32 _kind, TFrameManager* _parent );
};

#define KPOLYLINE_BSHIPBEAM1   0
#define KPOLYLINE_BSHIPBEAM2   1
#define KPOLYLINE_BSHIPBEAM3   2
#define KPOLYLINE_BSHIPBEAM4   3
#define KPOLYLINE_MISSILESMOKE   4
#define KPOLYLINE_BSHIPSMOKE   5
#define KPOLYLINE_STRAIGHTBEAM1   6
#define KPOLYLINE_STRAIGHTBEAM2   7
#define KPOLYLINE_BUILDINGSMOKE   8
#define KPOLYLINE_STRAIGHTBEAMCTRL1   9
#define KPOLYLINE_STRAIGHTBEAMCTRL2   10
#define KPOLYLINE_HOMINGLASER   11

