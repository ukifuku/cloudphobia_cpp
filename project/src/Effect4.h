#pragma once

using namespace std;

#include "types.h"
#include "SXScene.h"
#include "TOBJ.h"
#include <vector>

class TPolyLine;

//板ポリゴンを生成
class TBoard{
private:
public:
	TSXMesh* Mesh;
	TBoard(f32 u1, f32 v1, f32 u2, f32 v2);
	~TBoard(); 
};

//戦艦ビーム
class TBattleShipBeam : public TOBJ {
private:
	f32 UVSlideSpeed;
	f32 Offset;
	f32 XSpace;
	s32 Pattern;
	vector<TPolyLine*> pl;
	TDGTexture* Texture;
	vector<f32> zoomrate;
public:
	TFrameManager* Frame;
	TBattleShipBeam(TOBJList* owner, TDGTexture* _tex, f32 _speed, f32 _space, s32 _pat);
	~TBattleShipBeam();
	void Move();    
	s32 OnDraw(); 
	void SetParam(f32 uvoffset, f32 speed, f32 zx, f32 zy, f32 space);
};

//3Dミサイル煙
class TMissileSmoke3D : public TOBJ {
private:
	TPolyLine* pl;
	vector<glm::vec3> PosList;
	vector<glm::vec3> VelocityList;
	s32 FirstID;
	glm::vec3 CurrentPos;
	glm::vec3 CurrentVel;
	bool Fade;
	s32 Alpha;
	bool PointAvailable;
	s32 Segment;
public:
	TFrameManager* Frame;
	TMissileSmoke3D(TOBJList * owner, s32 _x, s32 _y, s32 _segment);
	~TMissileSmoke3D();
	void Move();    
	s32 OnDraw(); 
	void SetPoint(const glm::vec3& _vec, const glm::vec3& _vel);
	void FadeStart();
	void InitPos(s32 _x, s32 _y);
};

//3Dミサイル本体
class TMissile3D : public TOBJ {
private:
	bool Buzzed;//Buzzったかどうか
	s32 LastBuzz;//最後にBuzzった時間
	TMissileSmoke3D* smoke;
	s32 Rdeg;
	glm::vec2 Velocity;
	glm::vec3 Velocity3D;
	glm::vec2 PrevPos;
	glm::vec3 PrevPos3D;
	f32 Homing;
	s32 Pattern;
	s32 StartDistance, EndDistance;
	s32 AliveDistance;
	s32 rad,Rspeed;
	void Chase();
	void DummyMove();
public:
	TFrameManager* Frame;
	TMissile3D(TOBJList* owner, TMissileSmoke3D* _smoke, s32 _x, s32 _y, f32 _homingLV, s32 _pat, s32 _distance);
	~TMissile3D();
	void Move();    
	void Die(); 
	void OnDamaged(TOBJ* target);
	void OnBroken();
	s32 OnDraw(); 
};

//ワープアウトする戦艦
class TBattleShip3 : public TOBJ {
private:
	s32 AppearDistance;
	s32 StopDistance;
	f32 Speed;
public:
	TFrameManager* Frame;
	TFrameManager* ShipFrame;
	TBattleShip3(TOBJList* owner, TSXMesh* _mesh, TDGTexture* _tex, MaterialData* _mat, s32 _appear, s32 _stop, f32 _speed);
	~TBattleShip3();
	void Move();    
	void SetParam(f32 _appear, f32 _stop, f32 _speed);
};

//板ポリゴン制御
class TBoardControl : public TOBJ {
private:
	s32 AppearDistance;
	s32 DisappearDistance;
	s32 RotSpeed;
	s32 AlphaSpeed;
	s32 Alpha;
	TBoard* Board;
	f32 ZoomRate;
	s32 PrevR;
	f32 PrevZoom;
public:
	TFrameManager* Frame;
	TBoardControl(TOBJList* owner, TDGTexture* _tex, UVRect& _uv, f32 _appear, f32 _disappear, f32 _rot, f32 _alpha, f32 _alphaspeed, f32 _zoomrate);
	~TBoardControl(); 
	void Move();    
	void SetParam(f32 _appear, f32 _disappear, f32 _rot,  f32 _alphaspeed, f32 _zoomrate);
};

//3Dミサイルジェネレータ
class TMissile3DGen : public TOBJ {
private:
	f32 Homing;
	s32 StartDistance, EndDistance;
	bool Launched;
	s32 Pattern;
	vector<TMissileSmoke3D*> smoke;
public:
	TFrameManager* Frame;
	TMissile3DGen(TOBJList* owner, f32 _pat, f32 _start, f32 _end, f32 _homingLV, f32 _x, f32 _y);
	~TMissile3DGen();
	void Move();    
	void SetParam(f32 _start, f32 _end, f32 _homingLV, f32 _x, f32 _y);
};

//戦艦煙
class TBattleShipSmoke : public TOBJ {
private:
	f32 UVSlideSpeed;
	TPolyLine* pl;
public:
	TFrameManager* Frame;
	TBattleShipSmoke(TOBJList* owner, TDGTexture* _tex, f32 _speed, f32 _zoom);
	~TBattleShipSmoke(); 
	void Move();    
	s32 OnDraw(); 
	void SetParam(f32 speed, f32 zx, f32 zy, f32 _zoomrate);
};

//3D炎
class TFire3D : public TOBJ {
private:
	f32 Speed;
	f32 ZoomRate;
	f32 Zoom;
	s32 AlphaSpeed;
	s32 Alpha;
	s32 Rdeg,RSpeed;
	TFrameManager* Parent;
	TDGTexture* Texture;
	UVRect UV;
	glm::vec3 Pos;
public:
	TFire3D(TOBJList* owner, TFrameManager* _parent, TDGTexture* _tex, UVRect& _uv, glm::vec3& _pos, f32 _speed, f32 _zoom, f32 _zoomrate, f32 _alpha);
	~TFire3D(); 
	void Move();    
	s32 OnDraw(); 
};

//戦艦炎
class TBattleShipFire : public TOBJ {
private:
	f32 Speed;
	f32 ZoomRate;
	s32 AlphaSpeed;
	TDGTexture* Texture;
	f32 Spread;
public:
	TFrameManager* Frame;
	TBattleShipFire(TOBJList* owner, TDGTexture* _tex, f32 _speed, f32 _zoom, f32 _alpha, f32 _spread);
	~TBattleShipFire(); 
	void Move();    
	void SetParam(f32 _speed, f32 _zoom, f32 _alpha, f32 _spread);
	void Die(); 
};

//単発直線ビーム
class TStraightBeam : public TOBJ {
private:
	f32 Speed;
	s32 Life;
	s32 StartDistance;
public:
	TPolyLine* pl;
	TFrameManager* Frame;
	TStraightBeam(TOBJList* owner, TDGTexture* _tex, s32 _pat, f32 _start, f32 _speed, f32 _life);
	~TStraightBeam(); 
	void Move();    
	void SetParam(f32 _start, f32 _speed, f32 _life);
	s32 OnDraw(); 
};

//ミサイル着弾後の火球
class TFireBall : public TOBJ {
private:
	glm::vec3 Pos;
	s32 Alpha;
public:
	TFireBall(TOBJList* owner, const glm::vec3& _pos);
	void Move();    
	s32 OnDraw(); 
};

//地形
class TLandscape : public TOBJ {
private:
	s32 FCX,FCY;
	s32 Width,Height;
public:
	TFrameManager* Frame;
	TLandscape(TOBJList* owner, TSXMesh* _mesh, TDGTexture* _tex, s32 pattern);
	~TLandscape();
	void Move();    
	void SetParam(f32 _x, f32 _y, f32 _width, f32 _height);
	void OnDamaged(TOBJ* target); 
};

//回転するメッシュ
class TRotateMesh : public TOBJ {
private:
	s32 RX,RY,RZ;
public:
TFrameManager* Frame;
	TRotateMesh(TOBJList* owner, TSXMesh* _mesh, TDGTexture* _tex);
	~TRotateMesh();
	void Move();    
	void SetParam(f32 _rx, f32 _ry, f32 _rz);
};

//シャッター
class TShutter : public TOBJ {
private:
	s32 StartDistance;
	f32 Speed;
	f32 EndY;
	bool Closing;
	f32 YPos;
	s32 FCX,FCY;
	s32 Width,Height;
public:
TFrameManager* Frame;
	TShutter(TOBJList* owner, TSXMesh* _mesh, TDGTexture* _tex, f32 _start, f32 _speed, f32 _endy);
	~TShutter();
	void Move();    
	void SetParam(f32 _start, f32 _speed, f32 _endy);
};

//エレベーター
class TElevator : public TOBJ {
private:
	f32 StartPos;
	s32 StartDistance;
	f32 Speed;
	s32 FCX,FCY;
	s32 Width,Height;
public:
TFrameManager* Frame;
	TElevator(TOBJList* owner, TSXMesh* _mesh, TDGTexture* _tex, f32 _distance, f32 _speed, f32 _start);
	~TElevator();
	void Move();    
	void SetParam(f32 _x, f32 _y, f32 _width, f32 _height, f32 _distance, f32 _speed, f32 _start);
};


//カメラにあわせて動くビル
class TBuildingMove : public TOBJ {
private:
	s32 StartDistance,EndDistance;
	f32 FarX,FarZ;
	f32 MoveX, MoveZ;
public:
TFrameManager* Frame;
	TBuildingMove(TOBJList* owner, TSXMesh* _mesh, TDGTexture* _tex, MaterialData* _mat, f32 _farx, f32 _farz, f32 _movex, f32 _movez, f32 _start, f32 _end);
	~TBuildingMove();
	void Move();    
	void SetParam(f32 _farx,  f32 _farz, f32 _movex, f32 _movez, f32 _start, f32 _end);
};

//ビル煙
class TBuildingSmoke : public TOBJ {
private:
	f32 UVSlideSpeed;
	TPolyLine* pl[2];
	s32 StartDistance,EndDistance;
	f32 FarX,FarZ;
	f32 MoveX, MoveZ;
public:
	TFrameManager* Frame;
	TBuildingSmoke(TOBJList* owner, f32 _farx, f32 _farz, f32 _movex, f32 _movez, f32 _start, f32 _end);
	~TBuildingSmoke(); 
	void Move();    
	s32 OnDraw(); 
	void SetParam(f32 _farx, f32 _farz, f32 _movex, f32 _movez, f32 _start, f32 _end);
};

//回転雲
class TRotateCloud : public TOBJ {
private:
	f32 USpeed,VSpeed,VSpeed_org;
	s32 StartDistance;
	f32 EndV;
	f32 CurrentV;
	f32 Deccel;
public:
TFrameManager* Frame;
	TRotateCloud(TOBJList* owner, TSXMesh* _mesh, TDGTexture* _tex, f32 _uspeed, f32 _vspeed, f32 _start, f32 _end, f32 _deccel);
	~TRotateCloud();
	void Move();    
	void SetParam(f32 _uspeed, f32 _vspeed, f32 _start, f32 _end, f32 _deccel);
};

//直線ビーム管理
class TStraightBeamControl : public TOBJ {
private:
	f32 Speed;
	s32 Life;
	s32 StartDistance,NextDistance,EndDistance;
	s32 Interval;
	f32 Width,Height;
	vector<TStraightBeam*> beam;
	s32 CurrentBeamNum;
public:
	TFrameManager* Frame;
	TStraightBeamControl(TOBJList* owner, TDGTexture* _tex, s32 _pat);
	~TStraightBeamControl(); 
	void Move();    
	void SetParam(f32 _start, f32 _end, f32 _speed, f32 _life, f32 _width, f32 _height,  f32 _interval);
};

//巨大戦艦レーザー
class TButtleShipHomingLaser : public TOBJ {
private:
	TPolyLine* pl;
	vector<glm::vec3> PosList;
	s32 FirstID;
	glm::vec3 CurrentPos;
	bool Fade;
	s32 Alpha;
	glm::vec3 HomePos,Target;
	TDGTexture* Texture;
	glm::vec3 Velocity3D;
	glm::vec3 PrevPos3D;
	f32 HomingLevel;
	void Homing();
public:
	TFrameManager* Frame;
	TButtleShipHomingLaser(TOBJList* owner, TPolyLine* _pl, TFrameManager* _parent, TDGTexture* _tex, glm::vec3& _vec, glm::vec3& _target, f32 _homingLV, f32 _vel, f32 _r);
	~TButtleShipHomingLaser(); 
	void Move();    
	s32 OnDraw(); 
	void SetPoint(const glm::vec3& _vec);
	void FadeStart();
};

//戦艦レーザージェネレータ
#define HOMINGLASER_MAX 18
class THomingLaserGen : public TOBJ {
private:
	TPolyLine* pl[HOMINGLASER_MAX];
	f32 Homing;
	s32 StartDistance;
	glm::vec3 Target;
	f32 Velocity;
	bool Launched;
	TDGTexture* Texture;
	f32 XSpace,ZSpace;
	s32 ActFlg,ActCount;
public:
	TFrameManager* Frame;
	THomingLaserGen(TOBJList* owner, TDGTexture* _tex, f32 _start, f32 _homingLV, f32 _x, f32 _y, f32 _z, f32 _vel, f32 _xspace, f32 _zspace);
	~THomingLaserGen();
	void Move();    
	void SetParam(f32 _start, f32 _homingLV,  f32 _x, f32 _y, f32 _z, f32 _vel, f32 _xspace, f32 _zspace);
	
	enum{ KSEGMENT_MAX = 48 };
};

//戦艦簡易爆発
class TBattleShipExplosion : public TOBJ {
private:
	TDGTexture* Texture;
	f32 Size;
	glm::vec3 Position;
	s32 Alpha;
public:
	TBattleShipExplosion(TOBJList* owner, TDGTexture* _tex, glm::vec3 _pos, f32 _size);
	~TBattleShipExplosion(); 
	void Move();    
	s32 OnDraw(); 
};

//ループ移動するメッシュ
class TRoopSlideMesh : public TOBJ {
private:
	f32 FirstX,FarX,ReturnX,Speed;
	s32 RX,RY,RZ;
public:
	TFrameManager* Frame;
	TRoopSlideMesh(TOBJList* owner, TSXMesh* _mesh, MaterialData* _mat, TDGTexture* _tex, TSXBlendMode _blend);
	~TRoopSlideMesh(); 
	void Move();    
	void SetParam(f32 _FirstX, f32 _FarX, f32 _ReturnX, f32 _Speed, f32 _rx, f32 _ry, f32 _rz);
};

