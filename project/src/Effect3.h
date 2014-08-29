#pragma once

using namespace std;

#include "types.h"
#include "TOBJ.h"
#include <vector>

class TSpriteEX;
//
class TBladeTrace : public TOBJ {
  private:
    vector<TSpriteEX*> Element;
	vector<f32> PointX1;
	vector<f32> PointY1;
	vector<f32> PointX2;
	vector<f32> PointY2;
    s32 BladeWidth;
    TJoint* BladeOwner;
    s32 PrevRR;
    s32 MAXElement;
    s32 MAXElement_ORG;
    s32 DiffR;
    s32 DivLevel;
  public:
    TBladeTrace(TOBJList* owner, TJoint* _obj, s32 _Width, s32 _Length, s32 _R);
    ~TBladeTrace(); 
    void Move(); 
    s32 OnDraw(); 
};

class THomingTrace : public TOBJ {
protected:
	vector<TSpriteEX*> Element;
	vector<f32> PointX1;
	vector<f32> PointY1;
	vector<f32> PointX2;
	vector<f32> PointY2;
	s32 BeamWidth;
	TJoint* BeamOwner;
	s32 PrevRR;
	s32 MAXElement;
public:
	bool IsTerminated;   //消滅判定に使う
	THomingTrace(TOBJList* owner, TJoint* _obj, s32 _Width, s32 _Length );
	~THomingTrace(); 
	void Move(); 
	s32 OnDraw(); 
};

class THomingTrace2 : public THomingTrace {
  private:
  public:
    bool IsTerminated;   //消滅判定に使う
    THomingTrace2(TOBJList* owner, TJoint* _obj, s32 _Width, s32 _Length );
};

//
class TEnemyBladeTrace : public TOBJ {
private:
	vector<TSpriteEX*> Element;
	vector<f32> PointX1;
	vector<f32> PointY1;
	vector<f32> PointX2;
	vector<f32> PointY2;
	s32 BladeWidth;
	TJoint* BladeOwner;
	s32 PrevRR;
	s32 MAXElement;
	s32 MAXElement_ORG;
	s32 DivLevel;
public:
	TEnemyBladeTrace(TOBJList* owner, TJoint* _obj, s32 _Width, s32 _Length);
	~TEnemyBladeTrace(); 
	void Move(); 
	s32 OnDraw(); 
};

class TEnemyBladeTrace2 : public TOBJ {
private:
	vector<TSpriteEX*> Element;
	vector<f32> PointX1;
	vector<f32> PointY1;
	vector<f32> PointX2;
	vector<f32> PointY2;
	s32 BladeWidth;
	TJoint* BladeOwner;
	s32 PrevRR;
	s32 MAXElement;
	s32 MAXElement_ORG;
	s32 DiffR;
	s32 DivLevel;
public:
	TEnemyBladeTrace2(TOBJList* owner, TJoint* _obj, s32 _Width, s32 _Length, s32 _R);
	~TEnemyBladeTrace2(); 
	void Move(); 
	s32 OnDraw(); 
};

class TBoss2BeamTrace : public TOBJ {
  protected:
	  vector<f32> PointX1;
	  vector<f32> PointY1;
	  vector<f32> PointX2;
	  vector<f32> PointY2;
    s32 BeamWidth;
    vector<s32> RR;
    s32 DiffColor;
  public:
    TBoss2BeamTrace(TOBJList* owner, s32 _Width, s32 _Length);
    void Move(); 
    void SetOwnerPos(s32 _x, s32 _y, s32 _r);
    void Reset(s32 _x, s32 _y, s32 _r);
    void Show(bool _show);
    void Slide(s32 _x, s32 _y);
};

//ポリライン2D
class TPolyLine2D : public TOBJ {
  private:
	  vector<TSpriteEX*> Element;
	  s32 Segment;
	  vector<glm::vec2> Points;
	  vector<glm::vec2> Vertex1;
	  vector<glm::vec2> Vertex2;
    s32 Width;
    bool IsUpdated;
    Rect uv;
    f32 ZoomRate;
  public:
    TPolyLine2D(TOBJList* owner, s32 _segment, s32 _width, TDGTexture* _tex, Rect& _uv, TBlendMode _blend);
    ~TPolyLine2D();  
    void SetPoint(s32 _num, glm::vec2& _vec);
    void CalcVartices();
    void SetZoomrate(f32 _zoom);
    void SetWidth(s32 _w);
    void Move(); 
    s32 OnDraw(); 
    void SetAlpha(s32 _a);
    void SetAlphaR(s32 _a);
    void SetPointR(s32 _num, glm::vec2& _vec);
    void Show(s32 _num, bool _bool);
	s32 GetSize() { return Segment; } 
};

//当たり判定専用スプライト
class TCollisionSpr : public TOBJ {
  private:
    TOBJ* Parent;
  public:
    TCollisionSpr(TOBJList* owner, TOBJ* _parent, s32 _x, s32 _y, s32 w, s32 h);
    void Move(); 
    void OnDamaged(TOBJ* target); 
};

//電撃
class TLightning : public TOBJ {
  private:
    TPolyLine2D* pl;
    s32 DestX,DestY;
	vector<glm::vec2> Points;
	vector<glm::vec2> RandVel;
	vector<TCollisionSpr*> Collision;
    s32 LastShown;
    void CalcPos();
  public:
    TLightning(TOBJList* owner, s32 _x, s32 _y, s32 dx, s32 dy);
    ~TLightning();  
    void Move(); 
    void OnDamaged(TOBJ* target); 
    void Die(); 
};

//サンダーソード
class TThunderSword : public TOBJ {
  private:
    bool Buzzed;//Buzzったかどうか
    s32 LastBuzz;//最後にBuzzった時間
  public:
    TThunderSword(TOBJList* owner, s32 _x, s32 _y);
    void Move(); 
    void OnDamaged(TOBJ* target); 
};

//サンダーソード素片
class TThunderSwordPart : public TOBJ {
  private:
  public:
    TThunderSwordPart(TOBJList* owner, s32 _x, s32 _y);
    void Move(); 
};

//ビットの光球
class TBitLight : public TOBJ {
  private:
    TOBJ* Parent;
    bool Visible;
  public:
    TBitLight(TOBJList* owner, TOBJ* _parent);
    void Move(); 
    void Show();
};

//拡散レーザー
class TSpreadLaser : public TOBJ {
  private:
    TPolyLine2D* pl;
	vector<glm::vec2> Points;
	vector<TCollisionSpr*> Collision;
    s32 Width;
    TOBJ* Parent;
    s32 Alpha;
    s32 Life;
    void CalcPos();
  public:
    TSpreadLaser(TOBJList* owner, TOBJ* _parent, s32 _x, s32 _y, s32 _width, s32 _life);
    void Move(); 
    void OnDamaged(TOBJ* target); 
    void Die(); 
};

