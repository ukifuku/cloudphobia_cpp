#include "PolyJoint.h"
#include "SXLib9.h"

TJoint::TJoint( const TSpriteEXBLT& _blt )
{
	Spr =  new TSpriteEX(_blt);         // �X�v���C�g�̐���
	Spr->FVisible  =  true;
	Rdegree = 0;
	RelationID = -1;
	Sticky = false;
}

TJoint::~TJoint()
{
	m_children.clear();
	SAFE_DELETE(Spr);
}

void TJoint::Move( f32 _x, f32 _y )
{
	f32 tempX = _x - Spr->FCX;
	f32 tempY = _y - Spr->FCY;
	
	Spr->Move(_x,_y);
	for( u32 i = 0; i < m_children.size(); i++ ) {
		if( m_children[i] != NULL )
			m_children[i]->MoveR( tempX, tempY );
	}
}

void TJoint::MoveR( f32 _x, f32 _y )
{
	Spr->MoveR(_x,_y);
	for( u32 i = 0; i < m_children.size(); i++ ) {
		if( m_children[i] != NULL )
			m_children[i]->MoveR(_x, _y);
	}
}

void TJoint::Rotate( s32 _r )
{
	RotateR( _r - Rdegree );
}

void TJoint::RotateR( s32 _r )
{
	_r  =  _r & 0x0FFF;
	Rdegree += _r;
	Rdegree  =  Rdegree & 0x0FFF;

	for( u32 i = 0; i < m_children.size(); i++ ) {
		if( m_children[i] != NULL ) {
			f32 tempX = m_children[i]->Spr->FCX;    //���̍��W����������i�[
			f32 tempY = m_children[i]->Spr->FCY;
			m_children[i]->Spr->FCX = Spr->FCX;   //�q�X�v���C�g�Ɏ����̉�]���S���L�^
			m_children[i]->Spr->FCY = Spr->FCY;
			m_children[i]->RotateR(_r);       //������FCX,FCY�𒆐S�ɉ�]

			//�q��FCX,FCY����]��̍��W�ɖ߂�
			tempX  =  tempX - Spr->FCX;    		    // ��U��_�Ɏ����Ă�����
			tempY  =  tempY - Spr->FCY;
			f32 ax  =  tempX * cost[_r] - tempY * sint[_r];				// ��]����
			f32 ay  =  tempX * sint[_r] + tempY * cost[_r];
			m_children[i]->Spr->FCX  =  ax + Spr->FCX;                              // ���ɖ߂�
			m_children[i]->Spr->FCY  =  ay + Spr->FCY;
		}
	}
	Spr->RotateR(_r);
}

void TJoint::SlideMoveR( f32 _x, f32 _y )
{
	Spr->SlideMoveR( _x, _y );
	for( u32 i = 0; i < m_children.size(); i++ ){
		if( m_children[i] != NULL ){
			m_children[i]->MoveR(_x*cost[Rdegree]-_y*sint[Rdegree],_x*sint[Rdegree]+_y*cost[Rdegree]);
		}
	}
}

void TJoint::Zoom( f32 _x, f32 _y )
{
	ZoomR(1 / Spr->FZX, 1 / Spr->FZY);
	ZoomR(_x, _y);
}

void TJoint::ZoomR( f32 _x, f32 _y )
{
	Spr->ZoomR(_x, _y);

	for( u32 i = 0; i < m_children.size(); i++ ) {
		if( m_children[i] != NULL ){
			m_children[i]->MoveR((m_children[i]->Spr->FCX - Spr->FCX)*(_x-1), (m_children[i]->Spr->FCY - Spr->FCY)*(_y-1));
		}
	}
}

void TJoint::SetDest( s32 _x, s32 _y, s32 _speed )
{
	//�ړI�n����
	DestX  =  _x;
	DestY  =  _y;

	SpeedFactor  =  _speed;
}

void TJoint::SetDestR( s32 _x, s32 _y )
{
	DestX += _x;
	DestY += _y;
}

void TJoint::AutoSlide()
{
	f32 distance  =  DestX - Spr->FCX;
	if( distance != 0 ) {
		//�ړ��X�s�[�h������
		XSpeed  =  distance / SpeedFactor;
		//    if( (Abs(XSpeed) < 1) ) XSpeed  =  distance / Abs(distance);
		MoveR(XSpeed,0);
	}

	distance  =  DestY - Spr->FCY;
	if( distance != 0 ) {
		YSpeed  =  distance / SpeedFactor;
		//    if( (Abs(YSpeed) < 1) ) YSpeed  =  distance / Abs(distance);
		MoveR(0,YSpeed);
	}


}

void TJoint::MonoRotateR( s32 _r )
{
	_r  =  _r & 0x0FFF;
	Rdegree += _r;
	Rdegree  =  Rdegree & 0x0FFF;

	Spr->RotateR(_r);
}

void TJoint::MonoRotate( s32 _r )
{
	MonoRotateR(_r-Rdegree);

}

void TJoint::RotateKeep( s32 _r )
{
	RotateKeepR(_r-Rdegree);

}

void TJoint::RotateKeepR( s32 _r )
{
	_r  =  _r & 0x0FFF;
	Rdegree += _r;
	Rdegree  =  Rdegree & 0x0FFF;

	for( u32 i = 0; i < m_children.size(); i++ ) {
		if( m_children[i] != NULL ) {
			f32 tempX = m_children[i]->Spr->FCX;    //���̍��W����������i�[
			f32 tempY = m_children[i]->Spr->FCY;
			m_children[i]->Spr->FCX = Spr->FCX;   //�q�X�v���C�g�Ɏ����̉�]���S���L�^
			m_children[i]->Spr->FCY = Spr->FCY;
			s32 tempr = m_children[i]->Rdegree;
			m_children[i]->RotateR(_r);       //������FCX,FCY�𒆐S�ɉ�]

			//�q��FCX,FCY����]��̍��W�ɖ߂�
			tempX  =  tempX - Spr->FCX;    		    // ��U��_�Ɏ����Ă�����
			tempY  =  tempY - Spr->FCY;
			f32 ax  =  tempX * cost[_r] - tempY * sint[_r];// ��]����
			f32 ay  =  tempX * sint[_r] + tempY * cost[_r];
			m_children[i]->Spr->FCX  =  ax + Spr->FCX;                              // ���ɖ߂�
			m_children[i]->Spr->FCY  =  ay + Spr->FCY;
			if( ! m_children[i]->Sticky ) m_children[i]->Rotate(tempr);
		}
	}
	Spr->RotateR(_r);
}

u32 TJoint::Add( TJoint* _child )
{
	if( _child )
	{
		m_children.push_back(_child);
		return m_children.size()-1;
	}

	return 0;
}

void TJoint::Delete( TJoint* _child )
{
	for( JointArray::iterator itr = m_children.begin(); itr != m_children.end(); ++itr ) {
		if( *itr != NULL ) {
			m_children.erase(itr);
			break;
		}
	}
}


