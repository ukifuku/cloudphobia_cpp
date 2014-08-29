#pragma once

#include "types.h"
#include "SpriteEX.h"
#include "SpriteEXU.h"
#include <vector>

//���Ԑڐ���N���X
class TJoint 
{
	typedef std::vector<TJoint*> JointArray;

protected:
	JointArray	m_children;
    f32 XSpeed;
	f32	YSpeed;   //�����X���C�h�ړ��X�s�[�h
    s32 DestX;
	s32 DestY;  //�ړI�n���W
    s32 SpeedFactor;  //�����X���C�h�ړ��X�s�[�h����p�ϐ�
	TSpriteEX* Spr;      //���g�̃X�v���C�g
public:
    s32 X;
    s32 Y;        //���W
    s32 Rdegree;
    s32 RelationID;
    bool Sticky;

public:
    TJoint* Objs( u32 _id ) { return _id < m_children.size() ? m_children[_id] : NULL; } 
	TSpriteEX* GetSpr() { return Spr; }
	s32 GetSize() { return m_children.size(); } 

    TJoint( const TSpriteEXBLT& _blt );
    virtual ~TJoint();
	virtual void Move( s32 _x, s32 _y ) { Move( (f32)_x, (f32)_y );}
	virtual void Move( f32 _x, f32 _y );
	//virtual void MoveR( s32 _x, s32 _y ){ MoveR( (f32)_x, (f32)_y );}
	virtual void MoveR( f32 _x, f32 _y );
    virtual void Rotate( s32 _r );
    virtual void RotateR( s32 _r );
    virtual void SlideMoveR( f32 _x, f32 _y );
    virtual void Zoom( f32 _x, f32 _y );
    virtual void ZoomR( f32 _x, f32 _y );
    virtual void SetDest( s32 _x, s32 _y, s32 _speed );
    virtual void SetDestR( s32 _x, s32 _y );
    virtual void AutoSlide();
    virtual void MonoRotateR( s32 _r );
    virtual void MonoRotate( s32 _r );
    virtual void RotateKeep( s32 _r );
    virtual void RotateKeepR( s32 _r );
	virtual u32 Add( TJoint* _child );
	virtual void Delete( TJoint* _child );
};


