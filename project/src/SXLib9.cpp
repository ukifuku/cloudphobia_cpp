#define _USE_MATH_DEFINES
#include <math.h>
#include "SXLib9.h"
#include "QDArc.h"

void SXInit()
{
	for( u32 i = 0; i < 4096; i++ ) {
		f32 real_i = (f32)i;
		f32 s = sin(real_i / 2048.0f * (f32)M_PI);
		f32 c = cos(real_i / 2048.0f * (f32)M_PI);

		sint[i] = s;
		cost[i] = c;
	}

}

TSXMesh::TSXMesh( TDGCarad* AOwner, string VBufName /*= ""*/, string IBufName /*= ""*/ )
{

	FOwner = AOwner;
	FVBuf = NULL;
	FIBuf = NULL;

	FVBufName = VBufName;
	FIBufName = IBufName;

	//�f�o�C�X�̔\�̓Q�b�g
	D3DCAPS9 caps;
	FOwner->GetD3D()->GetDeviceCaps(FOwner->GetAdapter(), FOwner->GetDeviceType(), &caps);
	MaxPrims = caps.MaxPrimitiveCount;
	MaxIndex = caps.MaxVertexIndex;
}

TSXMesh::~TSXMesh()
{
	SAFE_DELETE(FVBuf);
	SAFE_DELETE(FIBuf);
}

void TSXMesh::Draw()
{
	//FOwner->Get3DDevice()->SetVertexShader(FVBuf->FVF);
	FOwner->Get3DDevice()->SetFVF(FVBuf->GetFVF());
	FOwner->Get3DDevice()->SetIndices(FIBuf->GetIndexBuffer());
	FOwner->Get3DDevice()->SetStreamSource(0, FVBuf->GetVertexBuffer(), 0, FVBuf->GetVertexSize());

	//primCount���������ĕ`��
	u32 ofs = 0;
	while( ofs < (FIBuf->GetCount()-2) ) {  //�Ȃ��-2���Ă邩�Ƃ����ƁA�P�v���~�e�B�u�͍Œ�R���_������

		u32 primCount  =  (FIBuf->GetCount() - ofs) / 3;
		if( primCount > MaxPrims )
			primCount = MaxPrims;

		FOwner->Get3DDevice()->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST, 0,0, FVBuf->GetVertexCount(), ofs, primCount);

		ofs += primCount*3;
	}

}

void TSXMesh::DrawPartial( u32 start, u32 primCount )
{
	//FOwner->Get3DDevice()->SetVertexShader(FVBuf->FVF);
	FOwner->Get3DDevice()->SetFVF(FVBuf->GetFVF());
	FOwner->Get3DDevice()->SetIndices(FIBuf->GetIndexBuffer());
	FOwner->Get3DDevice()->SetStreamSource(0, FVBuf->GetVertexBuffer(), 0, FVBuf->GetVertexSize());


	//primCount���������ĕ`��

	//�J�n���_�C���f�N�X�̈ʒu�ƁA�I�����_�C���f�N�X�̈ʒu���m��
	u32 ofs = start;
	u32 endOfs = start + primCount*3;
	if( endOfs > FIBuf->GetCount() )
		endOfs = FIBuf->GetCount();

	//�I�����_�C���f�N�X�܂ŁAMaxPrims�����܂��ܕ`��
	while( ofs < endOfs-2 ) {

		u32 curPrims  =  (endOfs - ofs) / 3;
		if( curPrims > MaxPrims )
			curPrims = MaxPrims;

		FOwner->Get3DDevice()->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST, 0,0, FVBuf->GetVertexCount(), ofs, curPrims);

		ofs += curPrims*3;
	}
}

void TSXMesh::LoadFromStream( istream& ss )
{
	s8 sign[17];
	u16 nVertices,nIndices;
	u32 FVF;
	u32 VSize;
	u8*	pLock;

	std::streamoff StartPos = ss.tellg();

	//�V�O�l�`���̊m�F
	sign[16] = 0;
	ss.read(sign, 16);

	if( strcmp(sign, SX00_SIGNATURE) == 0) {
		//SX�`��
		FVF = FVF_SXVERTEX;
		VSize = sizeof(TSXVertex);
		//���_���E���_�C���f�N�X���̐ݒ�
		ss.read(reinterpret_cast<char*>(&nVertices), sizeof(u16));
		ss.read(reinterpret_cast<char*>(&nIndices),  sizeof(u16));
	} else if( strcmp(sign, SX02_SIGNATURE) == 0 ) {
		//���_���E���_�C���f�N�X���̐ݒ�
		ss.read(reinterpret_cast<char*>(&nVertices), sizeof(u16));
		ss.read(reinterpret_cast<char*>(&nIndices),  sizeof(u16));
		//SXFlexible�`��
		ss.read(reinterpret_cast<char*>(&FVF), sizeof(u32));
		ss.read(reinterpret_cast<char*>(&VSize), sizeof(u32));
	} else {
		//DGCarad_PutDebugMessage('SXLib: �s����SX�t�@�C���ł�');
		return;
	}

	SetupBuffers(nVertices, nIndices, FVF, VSize, D3DPOOL_MANAGED);

	//�w�b�_�̃X�L�b�v
	ss.seekg(StartPos + 256);

	try{
		//���_�̃��[�h
		pLock = (u8*)FVBuf->Lock( 0, 0, 0);
		ss.read(reinterpret_cast<char*>(pLock), VSize * nVertices);
		FVBuf->Unlock();

		//���_�C���f�N�X�̃��[�h
		pLock = (u8*)FIBuf->Lock( 0, 0, 0);
		ss.read(reinterpret_cast<char*>(pLock), sizeof(u16) * nIndices);
		FIBuf->Unlock();
	}
	catch(std::istream::failure e){
		//DGCarad_PutDebugMessage('SXLib: �X�g���[������̓ǂݍ��ݒ��ɃG���[���������܂���');
		return;
	}

	//�o�E���f�B���O�{�����[�����X�V
	UpdateBounds();

}

void TSXMesh::LoadFromFile( string fileName, string qdaName /*= ""*/ )
{
	ifstream fs;
	if( qdaName.empty()) {
		//�t�@�C������ǂݍ���
		try{
			fs.open( fileName );
		}
		catch(std::istream::failure e){
			//DGCarad_PutDebugMessage('SXLib: ' + filename + ' ���A�J���܂���');
			return;
		}

		LoadFromStream(fs);
		fs.close();
	} else {
		//�A�[�J�C�u����ǂݍ���
		stringstream ms;
		try{
			ExtractFromQDAFile( ms, qdaName, fileName);
		}
		catch(std::istream::failure e){
			//DGCarad_PutDebugMessage('SXLib: ' + QDAname + ' ���́A' + filename + '�����o�ł��܂���');
			return;
		}

		LoadFromStream(ms);
	}
}

void TSXMesh::SaveToFile( string filename )
{
	ofstream src(filename);
	SaveToStream(src);
	src.close();

}

void TSXMesh::SaveToStream( ostream& ss )
{
	s8 sign[17];
	u8 pad[224];	//1..224
	s32 DXVer[5];
	u8* p;

	std::streamoff headPos = ss.tellp();
	//{�w�b�_}

	//�V�O�l�`��
	memcpy( sign, SX02_SIGNATURE, 17 );
	ss.write(sign, 16);

	u16 vtxCnt = FVBuf->GetVertexCount();
	ss.write(reinterpret_cast<char*>(&vtxCnt), 2);
	u16 idxCnt = FIBuf->GetCount();
	ss.write(reinterpret_cast<char*>(&idxCnt), 2);

	u32 fvf = FVBuf->GetFVF();
	ss.write(reinterpret_cast<char*>(&fvf), 4);
	u32 vtxSize = FVBuf->GetVertexSize();
	ss.write(reinterpret_cast<char*>(&vtxSize), 4);
	memcpy( DXVer, DXVER_FOR_FVF, 5 );
	ss.write(reinterpret_cast<char*>(DXVer), 4);
	ZeroMemory(&pad, sizeof(pad));
	ss.write(reinterpret_cast<char*>(pad), sizeof(pad));

	ss.seekp(headPos + 256);

	//{�{��}

	//VBuf
	p = (u8*)FVBuf->Lock(0,0,0);
	ss.write(reinterpret_cast<char*>(p), FVBuf->GetVertexSize() * FVBuf->GetVertexCount());
	FVBuf->Unlock();

	//FIBuf
	p = (u8*)FIBuf->Lock(0,0,0);
	ss.write(reinterpret_cast<char*>(p), 2 * FIBuf->GetCount());
	FIBuf->Unlock();

}

void TSXMesh::SetupBuffers( u32 newVertexCount, u32 newIndexCount, u32 FVF, u32 VertexSize, D3DPOOL Pool )
{
	FFaceCount = newIndexCount / 3;

	//�܂��A������VertexBuffer�̉��
	SAFE_DELETE(FVBuf);
	SAFE_DELETE(FIBuf);

	if( newVertexCount > MaxIndex ) {
		//DGCarad_PutDebugMessage(Format('SXLib : ���̃n�[�h�E�F�A�ł́A�ő� %d �܂ł������_�C���f�N�X��U�鎖���ł��܂���',[maxIndex]));
	}

	//VertexBuffer�̐���
	FVBuf = new TDGVertexBuffer(FOwner, newVertexCount, VertexSize, 0, FVF, Pool, FVBufName);

	//IndexBuffer�̍쐬
	FIBuf = new TDGIndexBuffer(FOwner, newIndexCount, 0, Pool, FIBufName);


}


bool TSXMesh::IsSameVs(u8* pV, u32 v1, u32 v2)
{
	u8* pV1;
	u8* pV2;
	pV1 = pV;  pV1 += FVBuf->GetVertexSize()*v1;
	pV2 = pV;  pV2 += FVBuf->GetVertexSize()*v2;

	return memcmp(pV1, pV2, FVBuf->GetVertexSize()) == 0;
}

void TSXMesh::Compact()
{


	vector<u32> tbl; //���_�ԍ��ϊ��e�[�u��
	tbl.resize(FVBuf->GetVertexCount());

	u8* pV = (u8*)FVBuf->Lock(0,0,0);

	//�܂��A�R���p�N�V������̒��_���A�R���p�N�V�����O�̂ǂ̒��_�ɂ�����̂��𒲂ׂ�
	//�R���p�N�V�������i�Ԃ̒��_�́A�R���p�N�V�����O��tbl[i]�Ԃ̒��_�ɑ�������
	u32 nCompacted = 0;	//���܂ł܂Ƃ߂�ꂽ�_�̐�
	tbl[0] = 0;
	for( u32 i = 1; i < FVBuf->GetVertexCount(); ++i ) {
		bool compacted = false;	//���ڂ��Ă����_�́A�܂Ƃ߂̑ΏۂɂȂ�����
		for( u32 j = 0; j < i; ++j ) {
			//�ȑO�ɒT�������_�Ɠ������W�A�����F�Ȃ�A�܂Ƃ߂�
			if( IsSameVs(pV, i, j) ) {
				compacted = true;
				tbl[i] = tbl[j];
				nCompacted++;
				break;
			}
		}

		if( ! compacted )
			tbl[i] = i - nCompacted;

	}
	u32 newVertexCount = FVBuf->GetVertexCount() - nCompacted;


	//{�R���p�N�V����}

	//�p�b�t�@�쐬
	u8* copyBuf = (u8*)malloc(newVertexCount * FVBuf->GetVertexSize());

	//�㏑�����Ă���
	for( u32 i = 0; i < FVBuf->GetVertexCount(); ++i ) {
		//Vertices[tbl[i]] = Vertices[i];
		u8* pV1 = pV;  pV1 += tbl[i] * FVBuf->GetVertexSize();
		u8* pV2 = pV;  pV2 += i * FVBuf->GetVertexSize();

		memmove(pV1, pV2, FVBuf->GetVertexSize());
	}

	//�o�b�t�@�ɑޔ�
	memmove(copyBuf, pV, newVertexCount * FVBuf->GetVertexSize());


	//VB�̍č\�z
	FVBuf->Unlock();
	FVBuf->SetVertexCount(newVertexCount);

	//�R�s�[
	pV = (u8*)FVBuf->Lock(0,0,0);
	memmove(pV, copyBuf, newVertexCount * FVBuf->GetVertexSize());
	FVBuf->Unlock();


	//�C���f�N�X�o�b�t�@�̒��g����������
	FIBuf->Lock(0,0,0);
	for( u32 i = 0; i < FIBuf->GetCount(); ++i ) {
		FIBuf->SetIndices(i, tbl[FIBuf->GetIndices(i)]);
	}
	FIBuf->Unlock();

	SAFE_FREE(copyBuf);
	tbl.clear();
}

void TSXMesh::Optimize()
{
	if( FFaceCount == 0 )
		return;

	//GeForce1/2�Ƀt�B�b�g����悤�ɁA�L���b�V���̃G���g����16�Ƃ��Ă���
	s32 nCacheEntry = 16;

	//�C���f�N�X�o�b�t�@���R�s�[
	vector<u16> Indices;
	Indices.resize( FIBuf->GetCount());
	FIBuf->Lock(0,0,0);
	for( u32 i = 0; i < FIBuf->GetCount(); i++ )
		Indices[i] = FIBuf->GetIndices(i);
	FIBuf->Unlock();

	//�ŏ��̈�̖ʂ͂��̂܂�
	s32 nOrdered = 3;		 //���ёւ��̏I��������_�C���f�N�X�̐�

	//�ʂ��ƂɃC���f�N�X���\�[�g���Ă�
	for( u32 i = 1 ; i < FFaceCount; i++ ) {
		s32 goodFace = -1;	//�T�����Ɍ��������A�P�q�b�g�A2�q�b�g�ʂ̃C���f�N�X
		s32 betterFace = -1;

		//�ł���΂R�Ƃ��L���b�V���q�b�g���Ă钸�_�������Ă���
		bool swapped = false;
		for( u32 j = i+1; j < FFaceCount; j++ ) {
			switch( HitCount(j, nCacheEntry, nOrdered, Indices ) ){
			case 1: if( goodFace == -1 ) goodFace = j;	break;
			case 2: if( betterFace == -1 ) betterFace = j;	break;
			case 3: {
				SwapFace(i, j, Indices );
				swapped = true;
				break;
					}
			}
		}

		//����������A�Q���P�L���b�V���q�b�g���Ă�ʂ������Ă���
		if( ! swapped ) {
			if( betterFace != -1 )
				SwapFace(i, betterFace, Indices);
			else if( goodFace != -1 )
				SwapFace(i, goodFace, Indices);
		}

		nOrdered += 3;
	}


	//�R�s�[�߂�
	FIBuf->Lock(0,0,0);
	for( u32 i = 0 ; i < FIBuf->GetCount(); ++i ){
		FIBuf->SetIndices(i, Indices[i]);
	}
	FIBuf->Unlock();

	Indices.clear();
}

void TSXMesh::UpdateBounds()
{

	//�ǂݎ���p�Ń��b�N
	u8* pV = (u8*)FVBuf->Lock(0,0,D3DLOCK_READONLY);

	glm::vec3* pP = (glm::vec3*)pV;

	glm::vec3 min;
	glm::vec3 max;
	min.x = (*pP).x;
	min.y = (*pP).y;
	min.z = (*pP).z;
	max = min;

	pV += FVBuf->GetVertexSize();

	for( u32 i = 1; i < FVBuf->GetVertexCount(); ++i ) {
		pP = (glm::vec3*)pV;

		if( min.x > (*pP).x )
			min.x = (*pP).x;
		else if( max.x < (*pP).x )
			max.x = (*pP).x;

		if( min.y > (*pP).y )
			min.y = (*pP).y;
		else if( max.y < (*pP).y )
			max.y = (*pP).y;

		if( min.z > (*pP).z )
			min.z = (*pP).z;
		else if( max.z < (*pP).z )
			max.z = (*pP).z;


		pV += FVBuf->GetVertexSize();
	}

	FVBuf->Unlock();

	//���ʂ̊i�[
	FBox.Min = min;
	FBox.Max = max;

	glm::vec3 center = (FBox.Min + FBox.Max) * 0.5f;
	glm::vec3 dif = FBox.Max - FBox.Min;

	//�܂�����������Ȃ���A�J�v�Z���̌v�Z

	if( (dif.x > dif.y) && (dif.x > dif.z) ) {
		//����X�����Ɏ��
		FCapsule.R = glm::sqrt( (dif.y*dif.y) + (dif.z*dif.z) ) * 0.5f;
		FCapsule.A = glm::vec3(FBox.Min.x + FCapsule.R, center.y, center.z);
		FCapsule.B = glm::vec3(FBox.Max.x - FCapsule.R, center.y, center.z);
	} else if( (dif.y > dif.x) && (dif.y > dif.z) ) {
		//����Y�����Ɏ��
		FCapsule.A = glm::vec3(center.x, FBox.Min.y + FCapsule.R, center.z);
		FCapsule.B = glm::vec3(center.x, FBox.Max.y - FCapsule.R, center.z);
		FCapsule.R = glm::sqrt( (dif.x*dif.x) + (dif.z*dif.z) ) * 0.5f;
	} else {
		//����Z�����Ɏ��
		FCapsule.A = glm::vec3(center.x, center.y, FBox.Min.z + FCapsule.R);
		FCapsule.B = glm::vec3(center.x, center.y, FBox.Max.z - FCapsule.R);
		FCapsule.R = glm::sqrt( (dif.x*dif.x) + (dif.y*dif.y) ) * 0.5f;
	}

	//���a��������ƕ␳(�C���`�L��)
	//FCapsule.R = FCapsule.R * 0.5;


	FSphere.Center = center;
	FSphere.Radius = glm::length(dif) * 0.5f;
}

void TSXMesh::Assign( TSXMesh& source )
{
	SetupBuffers(source.FVBuf->GetVertexCount(), source.FIBuf->GetCount(), source.FVBuf->GetFVF(),
		source.FVBuf->GetVertexSize(), source.FVBuf->GetPool());

	//���g�̃R�s�[
	u8* pVDest = (u8*)FVBuf->Lock(0, FVBuf->GetVertexCount(), 0);
	u8* pVSrc = (u8*)source.FVBuf->Lock(0, source.FVBuf->GetVertexCount(), D3DLOCK_READONLY);
	memmove(pVDest, pVSrc, FVBuf->GetVertexSize() * FVBuf->GetVertexCount());
	FVBuf->Unlock();
	source.FVBuf->Unlock();

	u16* pIDest = FIBuf->Lock(0, FIBuf->GetCount(), 0);
	u16* pISrc = source.FIBuf->Lock(0, FIBuf->GetCount(), D3DLOCK_READONLY);
	memmove(pIDest, pISrc, FIBuf->GetCount() * sizeof(u16));
	FIBuf->Unlock();
	source.FIBuf->Unlock();

}


TSXMesh* TSXMeshList::GetMesh( s32 idx )
{
	return m_meshes[idx];
}

void TSXMeshList::SetMesh( s32 idx, TSXMesh* Value )
{
	m_meshes[idx] = Value;

}

TSXMeshList::TSXMeshList( TDGCarad& ADG )
{
	DG = &ADG;
}

TSXMeshList::~TSXMeshList()
{
	Clear();
}

void TSXMeshList::Delete( s32 idx )
{
	SAFE_DELETE(m_meshes[idx]);
	m_meshes.erase(m_meshes.begin() + idx);
}

void TSXMeshList::LoadFromStream( istream& Source )
{
	//�܂����g������
	Clear();


	//���ڐ�
	u32 cnt;
	Source.read(reinterpret_cast<char*>(&cnt), 4);

	for( u32 i = 0 ; i < cnt; i++ ) {
		u8 flag = 0;
		Source.read(reinterpret_cast<char*>(flag), 1);
		if( flag == 0 ) {
			Add(NULL);
		} else {
			TSXMesh* newMesh = new TSXMesh(DG);
			newMesh->LoadFromStream(Source);
			Add(newMesh);
		}
	}

}

void TSXMeshList::LoadFromFile( string filename, string qdaName /*= ""*/ )
{
	if( qdaName.empty() ) {
		//�t�@�C������ǂݍ���
		ifstream src(filename);
		LoadFromStream(src);
		src.close();
	} else {
		//�A�[�J�C�u�ǂݍ���
		stringstream ms;
		ExtractFromQDAFile(ms, qdaName, filename);
		LoadFromStream(ms);
	}

}

void TSXMeshList::SaveToStream( ostream& Dest )
{
	//���ڐ�
	s32 cnt = m_meshes.size();
	Dest.write(reinterpret_cast<char*>(&cnt), sizeof(s32));

	//���b�V���̕ۑ��A�����A���b�V����NULL�Ȃ�A�o�C�g�l 0 ��1�o�C�g�����
	//��������Ȃ��Ȃ�A�o�C�g�l1������
	u8 zero = 0;
	u8 one = 1;
	for( u32 i = 0; i < m_meshes.size(); i++ ) {
		if( m_meshes[i] == NULL ) {
			Dest.write(reinterpret_cast<char*>(&zero), 1);
		} else {
			Dest.write(reinterpret_cast<char*>(&one), 1);
			m_meshes[i]->SaveToStream(Dest);
		}
	}

}

void TSXMeshList::SaveToFile( string filename )
{
	ofstream dst(filename);
	SaveToStream(dst);
	dst.close();
}

void TSXMeshList::Add( TSXMesh* _mesh )
{
	m_meshes.push_back(_mesh);
}

void TSXMeshList::Clear()
{
	for( u32 i = 0; i < m_meshes.size(); i++ ) {
		SAFE_DELETE(m_meshes[i]);
	}
	m_meshes.clear();
}

glm::mat4 TSXFrame::GetWorldMatrix()
{
	glm::mat4 mat;
	if( FStyle != sxfsMeshContainer )
		mat = FMatrix;

	//�c��̃}�g���N�X�� �e���c���c �̏��ɏ悶�Ă����B
	//Ancestors[0] ��NULL�ł���A�r����NULL�͓���Ȃ��Ƃ����O��̉��Ɍv�Z
	for( u32 i = FAncestors->GetSize()-1 ; i >= 1; i-- ) {
		//MeshContainer�̏ꍇ�A�}�g���N�X�͏��Identity
		if( Ancestors(i)->GetStyle() != sxfsMeshContainer )
			mat = Ancestors(i)->FMatrix * mat;
	}

	return mat;

}

glm::mat4 TSXFrame::GetWorldMatrixRot()
{
	glm::mat4 mat;
	if( FStyle != sxfsMeshContainer )
		mat = ExtractRotation(FMatrix);

	//�c��̃}�g���N�X�� �e���c���c �̏��ɏ悶�Ă����B
	//Ancestors[0] ��NULL�ł���A�r����NULL�͓���Ȃ��Ƃ����O��̉��Ɍv�Z
	for( u32 i = FAncestors->GetSize()-1; i >=1; i-- ) {
		//MeshContainer�̏ꍇ�A�}�g���N�X�͏��Identity
		if( Ancestors(i)->GetStyle() != sxfsMeshContainer ){
			glm::mat4 ancmat = ExtractRotation(Ancestors(i)->FMatrix);
			mat = ancmat * mat;
		}
	}

	return mat;
}

//��c���X�g���ċA�I�ɏC������
void TSXFrame::RebuildAncestors( TSXFrame* me )											
{																
	for( u32 i = 0; i < me->GetChildren()->GetSize(); i++ ) {	
		/*�����̐�c���c�Ɛ��߂Ȃ���*/						
		me->Children(i)->GetAncestors()->Assign(me->GetAncestors());
		/*������e�Ɛ��߂Ȃ���*/									
		me->Children(i)->GetAncestors()->Add(me);		
		/*�q�X���X�ɓn���Ă��������܂�	*/							
		RebuildAncestors(me->Children(i));			
	}															
}

void TSXFrame::SetParent( TSXFrame* Value )
{
	//���̐e�̃��X�g����O��
	if( FParent != NULL )
		FParent->GetChildren()->Delete(this);


	if( Value != NULL ) {
		//�V�����e�̃��X�g�ɉ�����
		Value->GetChildren()->Add(this);
		//��c���X�g��ς���
		FAncestors->Assign(Value->FAncestors);
		FAncestors->Add(Value);
	} else {
		FAncestors->Clear();
	}

	//�����Ɏq��������Ȃ�A�����̐�c���X�g���ς���
	RebuildAncestors(this);

	FParent  =  Value;

}

TDGTexture* TSXFrame::GetTexture()
{
	return FTextures[0];

}

TDGTexture* TSXFrame::GetTextures( s32 idx )
{
	return FTextures[idx];

}

void TSXFrame::SetTexture( TDGTexture* Value )
{
	FTextures[0] = Value;

}

void TSXFrame::SetTextures( s32 idx, TDGTexture* Value )
{
	FTextures[idx] = Value;

}

TSXFrame::TSXFrame( TSXFrame* parentFrame )
{
	//�K�{�I�u�W�F�N�g�̐���

	//�e�q�֌W�̏�����
	FAncestors = new TSXFrameList();
	if( parentFrame != NULL ) {
		FAncestors->Assign(parentFrame->FAncestors);   //��c���X�g�̃R�s�[
		FAncestors->Add(parentFrame);                //��c���X�g�̖����ɁA�e������

		parentFrame->GetChildren()->Add(this);                 //�e�̎q���X�g�Ɏ���������
	}

	FChildren = new TSXFrameList();
	FParent = parentFrame;

	//FBinds = [];

	FStyle = sxfsDefault;
	FEnableMeshT = true;

	SortKey = sxsMeshCenter;


	//�ϐ��Ƃ��̏�����
	FMatrix = glm::mat4();
	FMeshMatrix = glm::mat4();

	BlendMode = sxbDefault;
	Material = MakeMaterial(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 10.0f);
	Lighting = true;
	Specular = true;
	FillMode = D3DFILL_SOLID;

	FVisibility = sxvShow;
	FEnableBS = false;
	Bounds.Center = glm::vec3(0,0,0);
	Bounds.Radius = 1.0f;

	//OnRender = NULL;
	//OnQueryZ = NULL;
	//OnQueryVisibility = NULL;

	Mesh = NULL;

	for( u32 i = 0 ; i < SX_MAX_TSS; i++ )
		FTextures[i] = NULL;

}

TSXFrame::~TSXFrame()
{
	while( FChildren->GetSize() > 0 ) {
		delete (Children(0));   //����ŁA�ċA�I�Ɏq�t���[����Free�����
	}

	//���X�g�I�u�W�F�N�g�̉��
	SAFE_DELETE(FChildren);
	SAFE_DELETE(FAncestors);

	//�e�̃��X�g���玩�����͂���
	if( FParent != NULL )
		FParent->GetChildren()->Delete(this);

}

TSphere3D MergeBS(TSphere3D& a, TSphere3D& b)
{
	glm::vec3 dir;
	TSphere3D result;
	f32 d = glm::distance(a.Center, b.Center);

	if( (d + b.R <= a.R) || (b.R <= 0) ) {
		//a��b�����S�ɕ��ł��邩�Ab�͑��݂��Ȃ�
		result = a;
	} else if( (d + a.R <= b.R) || (a.R <= 0) ) {
		//b��a�����S�ɕ��ł��邩�Aa�͑��݂��Ȃ�
		result = b;
	} else {
		//��2�̏����́Ad=0 �̏ꍇ���܂�ł���
		//�ȉ��A�ǂ�����܂񂾂�܂܂ꂽ�肵�ĂȂ��ꍇ

		//���a�͊ȒP�ɋ��܂�
		result.R = (d + a.R + b.R) / 2.0f;

		//a.Center ����Ab.Center�ɑ΂��āAa.R�̕��߂��Ă��甼�a���i�񂾂Ƃ��낪�V�������S
		dir = glm::normalize(b.Center - a.Center);
		result.Center = a.Center + dir * (result.R - a.R);
	}

	//DGCarad_PutDebugMessage(Format('���̍����cC1(%f,%f,%f),R1=%f & C2(%f,%f,%f),R2=%f --> C(%f,%f,%f),R=%f',
	//	[a.X, a.Y, a.Z, a.R, b.x, b.Y, b.Z, b.R, result.X, result.Y, result.Z, result.R]));

	return result;
}

//���t���[���̃o�E���f�B���O���Ǝq���t���[���̃o�E���f�B���O�����}�[�W
TSphere3D CalcBS(TSXFrame* me, glm::mat4& worldM, bool onlyMyself)
{
	glm::mat4 meToWorldM,meshM;
	glm::mat3x3 m33;
	TSphere3D result;
	if( me->GetVisibility() == sxvHide ) {
		//�q�����܂߂�Hide�ɂȂ��Ă�Ȃ�A���̃t���[���ȉ��͍l�����Ȃ�
		result.R = -1.f;
		result.Center = glm::vec3(0,0,0);
	} else {
		if( me->GetStyle() == sxfsMeshContainer )
			meToWorldM = worldM;
		else
			meToWorldM = worldM * me->GetMatrix();

		if( (me->GetVisibility() == sxvHideMyself) ) {
			//�������������Ȃ�
			result.R = -1.f;
			result.Center = glm::vec3(0,0,0);
			for( u32 i = 0; i < me->GetChildren()->GetSize(); i++ ) {
				result = MergeBS(CalcBS(me->Children(i), meToWorldM, onlyMyself), result);
			}
		} else {
			//������
			if( me->GetEnableBounds() ) {
				//�o�E���f�B���O�����w�肳��Ă���Ȃ�A�����Ԃ�
				result.Center = TransformVec3( me->Bounds.Center, meToWorldM ); 
				result.R = me->Bounds.R;
			} else {
				//���b�V���̃o�E���f�B���O���Ǝq���t���[���̃o�E���f�B���O�����}�[�W���ĕԂ�
				if( me->Mesh != NULL ) {
					if( me->GetEnableMeshTransform() ) {
						meshM = meToWorldM * me->GetMeshMatrix();
						m33 = Matrix44To33(meshM);
						result.Center = TransformVec3(me->Mesh->GetSphere().Center, meshM);
						result.R = glm::determinant(m33) * me->Mesh->GetSphere().R;
					} else {
						result.Center = TransformVec3(me->Mesh->GetSphere().Center, meToWorldM);
						result.R = me->Mesh->GetSphere().R;
					}
				} else {
					result.Center = glm::vec3(0,0,0);
					result.R = -1.f;
				}

				for( u32 i = 0; i < me->GetChildren()->GetSize(); i++ ) {
					result = MergeBS(CalcBS(me->Children(i), meToWorldM, onlyMyself), result);
				}
			}
		}

	}

	if( ! onlyMyself ) {
		me->Bounds = result;
		me->Bounds.Center = TransformVec3(me->Bounds.Center, InvMatrix(meToWorldM));
		// me->WorldToLocal(me->Bounds.Center);
	}

	return result;

}

void TSXFrame::UpdateBounds( bool onlyMyself )
{
	if( FParent = NULL )
		Bounds = CalcBS(this, glm::mat4(), onlyMyself);
	else
		Bounds = CalcBS(this, FParent->GetWorldMatrix(), onlyMyself);

	//�Q�b�g���Ă����̂́A���[���h���W�\�L�Ȃ̂ŁA�|�󂷂�
	Bounds.Center = WorldToLocal(Bounds.Center);
}


void ActivateBS(TSXFrame* me)
{
	me->SetEnableBounds(true);
	for( u32 i = 0; i < me->GetChildren()->GetSize(); i++ ) {
		ActivateBS(me->Children(i));
	}
}

void TSXFrame::ActivateBounds()
{
	ActivateBS(this);
}


void DeactivateBS(TSXFrame* me)
{
	me->SetEnableBounds(false);
	for( u32 i = 0 ; i < me->GetChildren()->GetSize(); i++ ) {
		DeactivateBS(me->Children(i));
	}
}
void TSXFrame::DeactivateBounds()
{
	DeactivateBS(this);
}

void TSXFrame::SetTransform( TSXFrame* ref, const glm::mat4& mat )
{

	if( ref == this ) {
		//�����ւ̕ϊ��s��
		FMatrix = FMatrix * mat;

	} else if( ref == FParent ) {
		//�e�ւ̕ϊ��s��
		FMatrix = mat;
	} else {
		//������ref�����[���h���e
		if( FParent != NULL ) {
			if( ref != NULL )
				FMatrix = InvMatrix(FParent->GetWorldMatrix()) * ref->GetWorldMatrix() * mat;
			else
				FMatrix = InvMatrix(FParent->GetWorldMatrix()) * mat;
		} else {
			if( ref != NULL )
				FMatrix = ref->GetWorldMatrix() * mat;
			else
				FMatrix = mat;
		}
	}
}

glm::mat4 TSXFrame::GetTransform( TSXFrame* ref )
{
	if( ref == this ) {
		//�����ւ̕ϊ��𓾂�c�P�ʍs��Ɍ��܂��Ă�B
		return glm::mat4();

	} else if( ref == FParent ) {

		//�e�ւ̃I���G���e�[�V�����𓾂�
		return FMatrix;

	} else {

		//���������[���h��ref
		if( ref != NULL )
			return InvMatrix(ref->GetWorldMatrix()) * GetWorldMatrix();
		else
			return GetWorldMatrix();
	}


}

glm::vec3 TSXFrame::LocalToWorld( const glm::vec3& vec )
{
	return TransformVec3(vec, GetWorldMatrix());

}

glm::vec3 TSXFrame::WorldToLocal( glm::vec3& vec )
{
	return TransformVec3(vec, InvMatrix(GetWorldMatrix()));

}

glm::vec3 TSXFrame::LocalToWorldRot( glm::vec3& rotvec )
{
	return TransformVec3(rotvec, GetWorldMatrixRot());

}

glm::vec3 TSXFrame::WorldToLocalRot( glm::vec3& rotvec )
{
	return TransformVec3(rotvec, InvMatrixRot(GetWorldMatrixRot()));

}

void TSXFrame::CalcMatrixOnRender()
{
	//�ϊ��ς݂̐e�t���[���̍s�񂩂�Aframe�̍s����Z�o
#define CalcMatrix(frame)										\
	{															\
	RenderedMatrix = FParent->MatrixOnRender() * FMatrix;	\
	if( FEnableMeshT )										\
	RenderedMeshMatrix = RenderedMatrix * FMeshMatrix;	\
		else													\
		RenderedMeshMatrix = RenderedMatrix;				\
		/*���������T�C��������*/								\
		FValidRenderedMatrix = true;							\
}

	//�ϊ��ς݂̃t���[�����Ȃ����A��c�������̂ڂ��Ē��ׂ�
	s32 StartG = 1; //������O����ϊ����J�n����΂������H
	for( u32 i = FAncestors->GetSize(); i >= 0; i-- ){
		if( Ancestors(i)->ValidMatrixOnRender() ){
			StartG = i+1;
		}
	}

	//��c���c���e�܂ł̕ϊ��s����v�Z����
	for( u32 i = StartG; i < FAncestors->GetSize(); i++ ){
		CalcMatrix(FAncestors[i]);
	}

	//�����̕ϊ��s��̌v�Z
	CalcMatrix(this);

}

void TSXFrame::SetTranslation( TSXFrame* ref, const glm::vec3& pos )
{
	if( ref == FParent ) {
		//�e�ւ̃g�����X���[�V��������邾���Ȃ�A�ȒP
		FMatrix[0][3] = pos.x;   FMatrix[1][3] = pos.y;   FMatrix[2][3] = pos.z;

	} else {

		glm::mat4 origMat = FMatrix;	 //�p���ێ��p
		glm::mat4 tmpMat = FMatrix;		 //ref�t���[���Ƃ̑��Εϊ��s��

		tmpMat[0][3] = pos.x;   tmpMat[1][3] = pos.y;   tmpMat[2][3] = pos.z;

		SetTransform(ref, tmpMat);

		FMatrix[0][0] = origMat[0][0];  FMatrix[1][0] = origMat[1][0];  FMatrix[2][0] = origMat[2][0];
		FMatrix[0][1] = origMat[0][1];  FMatrix[1][1] = origMat[1][1];  FMatrix[2][1] = origMat[2][1];
		FMatrix[0][2] = origMat[0][2];  FMatrix[1][2] = origMat[1][2];  FMatrix[2][2] = origMat[2][2];
	}
}

glm::vec3 TSXFrame::GetTranslation( TSXFrame* ref )
{
	glm::vec3 result;
	if( ref == FParent ) {
		result.x = FMatrix[0][3];
		result.y = FMatrix[1][3];
		result.z = FMatrix[2][3];
	} else {
		glm::mat4 tmpMat = GetTransform(ref);
		result.x = tmpMat[0][3];
		result.y = tmpMat[1][3];
		result.z = tmpMat[2][3];
	}

	return result;
}

void TSXFrame::SetOrientation( TSXFrame* ref, const glm::vec3& vecZ, const glm::vec3& vecY )
{
	glm::vec3 vecX = glm::normalize(glm::cross(vecY, vecZ));

	if( ref = FParent ) {

		//�e�ւ̎p������邾���Ȃ�A�ȒP
		FMatrix[0][0] = vecX.x;  FMatrix[1][0] = vecX.y;  FMatrix[2][0] = vecX.z;
		FMatrix[0][1] = vecY.x;  FMatrix[1][1] = vecY.y;  FMatrix[2][1] = vecY.z;
		FMatrix[0][2] = vecZ.x;  FMatrix[1][2] = vecZ.y;  FMatrix[2][2] = vecZ.z;

	} else {
		f32 vTrans[4];			 //���s�ړ������̕ۑ��p

		//���s�ړ������̕ۑ�
		vTrans[0] = FMatrix[0][3];
		vTrans[1] = FMatrix[1][3];
		vTrans[2] = FMatrix[2][3];

		//�ϊ�
		glm::mat4 tmpMat;             //ref�t���[���Ƃ̑��Εϊ��s��
		tmpMat[0][0] = vecX.x;  tmpMat[1][0] = vecX.y;  tmpMat[2][0] = vecX.z;  tmpMat[3][0] = 0;
		tmpMat[0][1] = vecY.x;  tmpMat[1][1] = vecY.y;  tmpMat[2][1] = vecY.z;  tmpMat[3][1] = 0;
		tmpMat[0][2] = vecZ.x;  tmpMat[1][2] = vecZ.y;  tmpMat[2][2] = vecZ.z;  tmpMat[3][2] = 0;
		tmpMat[0][3] = vTrans[0]; tmpMat[1][3] = vTrans[1]; tmpMat[2][3] = vTrans[2]; tmpMat[3][3] = 1.f;

		SetTransform(ref, tmpMat);

		//���s�ړ������̕���
		FMatrix[0][3] = vTrans[0]; FMatrix[1][3] = vTrans[1]; FMatrix[2][3] = vTrans[2];
	}
}

void TSXFrame::GetOrientation( TSXFrame* ref, glm::vec3& vecZ, glm::vec3& vecY )
{
	glm::mat4 tmpMat = GetTransform(ref);	//���ʎZ�o�p�o�b�t�@

	vecY.x = tmpMat[0][1]; vecY.y = tmpMat[1][1]; vecY.z = tmpMat[2][1];
	vecZ.x = tmpMat[0][2]; vecZ.y = tmpMat[1][2]; vecZ.z = tmpMat[2][2];
}

void TSXFrame::SetOrientationMatrix( TSXFrame* ref, const glm::mat4& mat )
{
	//�ۑ�
	glm::vec3 pushedV;  //���s�ړ����ۑ��p

	pushedV.x = FMatrix[0][3];  pushedV.y = FMatrix[1][3];  pushedV.z = FMatrix[2][3];

	if( ref == this ) {
		//�����ւ̕ϊ��s��
		FMatrix = FMatrix * mat;

	} else if( ref == FParent ) {
		//�e�ւ̕ϊ��s��
		FMatrix = mat;

	} else {

		//������ref�����[���h���e
		if( FParent != NULL ) {
			if( ref != NULL )
				FMatrix = InvMatrix(FParent->GetWorldMatrix()) * ref->GetWorldMatrix() * mat;
			else
				FMatrix = InvMatrix(FParent->GetWorldMatrix()) * mat;
		} else {
			if( ref != NULL )
				FMatrix = ref->GetWorldMatrix() * mat;
			else
				FMatrix = mat;
		}
	}

	//����
	FMatrix[0][3] = pushedV.x;  FMatrix[1][3] = pushedV.y;  FMatrix[2][3] = pushedV.z;

}

void TSXFrame::SetOrientationQuat( TSXFrame* ref, const glm::quat& quat )
{
	SetOrientationMatrix(ref, glm::mat4_cast(quat));
}

void TSXFrame::LookAt( TSXFrame* target, const glm::vec3& pos, TSXFrame* world, const s32 bank )
{
	//target��pos�́A�����̍��W�n�łǂ��Ȃ̂�
	glm::vec3 vRel = WorldToLocal(target->LocalToWorld(pos));     //Target�Ƃ̑��΍��W

	//���� = Z���x�N�g��
	glm::vec3 vecZ = glm::normalize(vRel);

	//�V��̌��� = world��Y��
	glm::vec3 vecCeil = WorldToLocal(world->LocalToWorld(glm::vec3(0,1.f,0))) - WorldToLocal(world->LocalToWorld(glm::vec3(0,0,0)));
	vecCeil = glm::normalize(vecCeil);

	//�V���Z�ŊO�ς�����āAX�̌����Ƃ���
	glm::vec3 vecX = glm::cross(vecCeil, vecZ);

	//�V���Z�����s�ȏꍇ�A�E���̕�����world��X���ɂ���
	f32 normX = glm::length(vecX);
	if( normX < 0.0001f )
		vecX = WorldToLocal(world->LocalToWorld(glm::vec3(1.f,0,0))) - WorldToLocal(world->LocalToWorld(glm::vec3(0,0,0)));

	vecX = glm::normalize(vecX);

	//X��Z�ŊO�ς�����āAY�̌����Ƃ���
	glm::vec3 vecY = glm::normalize(glm::cross(vecZ, vecX));



	//�ȏ�Ŏp���̌v�Z�����`�Z�b�g
	SetOrientation(this, vecZ,vecY);

	//�o���N
	if( bank != 0 )
		SetTransform(this, RotZMatrix(bank));


}

glm::mat4 TSXFrame::ViewMatrix()
{
	return InvMatrix(GetWorldMatrix());
}


void traverse_SetBlendModeRcv( TSXFrame* frame, TSXBlendMode Value )
{
	frame->BlendMode = Value;
	for( u32 i = 0; i < frame->GetChildren()->GetSize(); i++ ){
		traverse_SetBlendModeRcv(frame->Children(i), Value);
	}
}

void TSXFrame::SetBlendModeRcv( TSXBlendMode Value )
{
	traverse_SetBlendModeRcv(this, Value);  
}

void traverse_SetLightingRcv( TSXFrame* frame, bool Value )
{
	frame->Lighting = Value;
	for( u32 i = 0; i < frame->GetChildren()->GetSize(); i++ ){
		traverse_SetLightingRcv(frame->Children(i), Value);
	}
}

void TSXFrame::SetLightingRcv( bool Value )
{
	traverse_SetLightingRcv(this, Value);
}


void traverse_SetSpecularRcv( TSXFrame* frame, bool Value )
{
	frame->Specular = Value;
	for( u32 i = 0; i < frame->GetChildren()->GetSize(); i++ ){
		traverse_SetSpecularRcv(frame->Children(i), Value);
	}
}
void TSXFrame::SetSpecularRcv( bool Value )
{
	traverse_SetSpecularRcv(this, Value);
}


void traverse_SetTextureWrapRcv( TSXFrame* frame, u32 Value )
{
	frame->TextureWrap = Value;
	for( u32 i = 0; i < frame->GetChildren()->GetSize(); i++ ){
		traverse_SetTextureWrapRcv(frame->Children(i), Value);
	}
}
void TSXFrame::SetTextureWrapRcv( u32 Value )
{
	traverse_SetTextureWrapRcv(this, Value);
}


void traverse_SetFillModeRcv( TSXFrame* frame, D3DFILLMODE Value )
{
	frame->FillMode = Value;
	for( u32 i = 0; i < frame->GetChildren()->GetSize(); i++ ){
		traverse_SetFillModeRcv(frame->Children(i), Value);
	}
}
void TSXFrame::SetFillModeRcv( D3DFILLMODE Value )
{
	traverse_SetFillModeRcv(this, Value);
}


void traverse_SetMaterialRcv( TSXFrame* frame, D3DMATERIAL9& Value )
{
	frame->Material = Value;
	for( u32 i = 0; i < frame->GetChildren()->GetSize(); i++ ){
		traverse_SetMaterialRcv(frame->Children(i), Value);
	}
}
void TSXFrame::SetMaterialRcv( D3DMATERIAL9& Value )
{
	traverse_SetMaterialRcv(this, Value);
}


void traverse_SetTexturesRcv( TSXFrame* frame, s32 idx, TDGTexture* Value )
{
	frame->SetTexture(idx, Value);
	for( u32 i = 0; i < frame->GetChildren()->GetSize(); i++ ){
		traverse_SetTexturesRcv(frame->Children(i), idx, Value);
	}
}
void TSXFrame::SetTexturesRcv( s32 idx, TDGTexture* Value )
{
	traverse_SetTexturesRcv(this, idx, Value);
}

void TSXFrame::DumpFrameStats()
{

}

TSXFrame* TSXFrame::Ancestors( u32 _idx )
{
	return FAncestors->GetItem(_idx);
}

TSXFrame* TSXFrame::Children( u32 _idx )
{
	return FChildren->GetItem(_idx);
}

TSXFrame* TSXFrameList::GetFrame( s32 idx )
{
	return m_frame[idx];

}

void TSXFrameList::SetFrame( s32 idx, TSXFrame* Value )
{
	m_frame[idx] = Value;
}

TSXFrameList::TSXFrameList()
{

}

TSXFrameList::~TSXFrameList()
{
	Clear();
}

void TSXFrameList::Assign( TSXFrameList* source )
{
	Clear();

	for( u32 i = 0; i < source->GetSize(); i++ ) {
		Add(source->GetFrame(i));
	}

}

void TSXFrameList::Add( TSXFrame* _item )
{
	m_frame.push_back(_item);
}

void TSXFrameList::Delete( TSXFrame* _item )
{
	for( u32 i = 0; i < m_frame.size(); i++ ) {
		if( _item == m_frame[i] )
		{
			m_frame.erase(m_frame.begin() + i );
			break;
		}
	}
}

void TSXFrameList::Clear()
{
	for( u32 i = 0; i < m_frame.size(); i++ ) {
		SAFE_DELETE(m_frame[i]);
	}
	m_frame.clear();
}

void TSXScene::InspectDevCaps()
{
	D3DCAPS9 caps;
	DG->GetD3D()->GetDeviceCaps(DG->GetAdapter(), DG->GetDeviceType(), &caps);

	FBlendOpEnable = (caps.PrimitiveMiscCaps && D3DPMISCCAPS_BLENDOP) != 0;

}

void TSXScene::Recover()
{
	DG->Get3DDevice()->SetViewport(&FVP);

	DG->Get3DDevice()->SetTransform( D3DTS_PROJECTION, &mat4toD3DMATRIX(FProjectionMatrix) );

	//�𑜓x�Ƃ��h���C�o�ύX�ɂƂ��Ȃ��āA�f�o�C�XCaps���ς�������������̂ŁA�Ē���
	InspectDevCaps();
}

TSXScene::TSXScene( TDGCarad* ADG, string debugName /*= ""*/ )
{
	Queue = new TSXRenderingQueue(ADG, this);
	SortQueue = new TSXRenderingQueue(ADG, this);
	TopQueue = new TSXRenderingQueue(ADG, this);

	FStateRestorer = new TSXStateRestorer(ADG);

	//DGCarad�Ƀ��\�[�X�Ƃ��ēo�^
	ResID = ADG->RegisterResouce(this, debugName);
	FDebugName = debugName;

	//�u�����h���[�h�̓o�^

	RegisterBlendMode(BlendInfo(false, false, D3DCMP_ALWAYS, false, D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA, D3DBLENDOP_ADD));
	RegisterBlendMode(BlendInfo(true, false, D3DCMP_ALWAYS, true, D3DBLEND_SRCALPHA, D3DBLEND_ONE, D3DBLENDOP_ADD));
	RegisterBlendMode(BlendInfo(true, false, D3DCMP_ALWAYS, true, D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA, D3DBLENDOP_ADD));
	RegisterBlendMode(BlendInfo(true, false, D3DCMP_ALWAYS, true, D3DBLEND_ZERO, D3DBLEND_INVSRCCOLOR, D3DBLENDOP_ADD));
	RegisterBlendMode(BlendInfo(true, false, D3DCMP_ALWAYS, true, D3DBLEND_ZERO, D3DBLEND_SRCCOLOR, D3DBLENDOP_ADD));
	RegisterBlendMode(BlendInfo(false, true, D3DCMP_GREATER, false, D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA, D3DBLENDOP_ADD));

	FCamera = NULL;
	FSortAlpha = true;
	DG = ADG;

	SetViewPort(0,0, ADG->GetWindowInfo().m_clientWidth, ADG->GetWindowInfo().m_clientHeight);
	SetProjection((f32)M_PI*0.5f, (f32)ADG->GetWindowInfo().m_clientHeight / (f32)ADG->GetWindowInfo().m_clientWidth, 0.01f, 10000.0f);

	InspectDevCaps();

}

TSXScene::~TSXScene()
{
	DG->UnregisterResource(ResID);
	SAFE_DELETE(FStateRestorer);

	SAFE_DELETE(SortQueue);
	SAFE_DELETE(Queue);
	SAFE_DELETE(TopQueue);

}


void TSXScene::QueingFrame( TSXFrame* frame, const glm::mat4& mat)
{
	TSphere3D sphere;

	TSXVisibility visibility;
	frame->SetRendered(false);
	frame->FValidRenderedMatrix = false;

	//�����̃`�F�b�N
	if( frame->OnQueryVisibility ) {
		//�C�x���g���ݒ肵�Ă���Ȃ�A����ɔC����
		frame->OnQueryVisibility(*frame, *this, *DG, visibility);
	} else {
		//�f�t�H���g�ł́AVisibility�v���p�e�B�ɏ]��
		visibility = frame->FVisibility;

		//�������ABoundingSphere���g���悤�Ȃ�A����ɏ]��
		if( visibility != sxvHide ) {
			if( frame->FEnableBS ) {
				if( ! SphereVisibility(mat * frame->FMatrix, frame->Bounds, FFarZ) ) {
					visibility = sxvHide;
				}
			}
		}
	}

	//sxvHide(�q�����܂߂ĉB��)�Ȃ牽�����Ȃ�
	if( visibility = sxvHide )
		return;

	//�q�t���[�������A���A�`�悷��K�v�Ȃ��Ȃ�A�������Ȃ�
	if( frame->GetChildren()->GetSize() == 0 ){
		if( ((frame->Mesh == NULL) && (! frame->OnRender)) || (visibility = sxvHideMyself) ){
			return;
		}
	}

	//�ϊ��s��̌v�Z
	if( frame->GetStyle() == sxfsMeshContainer )
		frame->RenderedMatrix = mat;
	else
		frame->RenderedMatrix = mat * frame->FMatrix;

	if( frame->FEnableMeshT )
		frame->RenderedMeshMatrix = frame->RenderedMatrix * frame->FMeshMatrix;
	else
		frame->RenderedMeshMatrix = frame->RenderedMatrix;

	frame->FValidRenderedMatrix = true;

	//�L���[�ɕ��荞��

	if( visibility == sxvShow ) {

		if( (frame->Mesh != NULL) && (! frame->OnQueryVisibility) ) {
			//Mesh���t���Ă���ꍇ

			//Mesh�̃o�E���f�B���O���ŃJ�����O�����݂�
			if( frame->FEnableMeshT ) {
				sphere.Center = frame->Mesh->FSphere.Center + glm::vec3(frame->FMeshMatrix[0][3], frame->FMeshMatrix[1][3], frame->FMeshMatrix[2][3]);
				sphere.R = glm::determinant(Matrix44To33(frame->FMeshMatrix)) * frame->Mesh->FSphere.R;
			} else {
				sphere = frame->Mesh->FSphere;
			}

			if( SphereVisibility(frame->RenderedMatrix, sphere,FFarZ) ) {

				if( FBlendInfos[frame->BlendMode].Sort ) {
					SortQueue->PushMesh(frame); //(frame->BlendMode, frame->Mesh, frame, meshM, curTexture, curMaterial, frame->OnRender);
				} else {
					Queue->PushMesh(frame);//(frame->BlendMode, frame->Mesh, frame, meshM, curTexture, curMaterial, frame->OnRender);
				}
			}

			frame->FRendered = true;
		} else if( frame->OnRender || frame->OnQueryVisibility ) {  
			//���b�V�����t���Ă��Ȃ��Ă��A�I�[�i�[�h���E�̏ꍇ
			//���b�V�����t���Ă��Ă��AQueryVisibility�Ŕ��肵���ꍇ
			//�J�����O���Ȃ�(���悤������)�ŁA���̂܂�Go!

			//�������A�I�[�i�[�h���E�Ȃ̂Ƀ��b�V�����t���ĂȂ��āAQueryZ�������Ȃ�O��֌W�̔���̂��悤���Ȃ��̂ŁA�����`���Ȃ�
			if( (frame->Mesh != NULL) || (frame->OnQueryZ) ) {

				if( FBlendInfos[frame->BlendMode].Sort ) {
					SortQueue->PushMesh(frame);//(frame->BlendMode, frame->Mesh, frame, meshM, curTexture, curMaterial, frame->OnRender);
				} else {
					Queue->PushMesh(frame); //(frame->BlendMode, frame->Mesh, frame, meshM, curTexture, curMaterial, frame->OnRender);
				}
			}

			frame->FRendered = true;
		}
	}

	//�q���̕`��
	for( u32 i = 0; i < frame->GetChildren()->GetSize(); i++ ) {
		QueingFrame(frame->Children(i), frame->RenderedMatrix);
	}

}

void TSXScene::Render( TSXFrame* rootFrame, bool flushBBSP /*= true*/ )
{
	glm::mat4 viewMat;

	//�r���[�s��̍쐬�c(���[���h���W�n�ł̃J�����t���[���̎p��)�̋t
	if( FCamera != NULL )
		viewMat  =  FCamera->ViewMatrix();
	else
		viewMat  =  glm::mat4();


	DG->Get3DDevice()->SetTransform(D3DTS_VIEW, &mat4toD3DMATRIX(viewMat));


	//�ċA�I�ɃL���[�ɓ����
	//���[�g�t���[�����Ȃ��ꍇ�́A�X�v���C�g�����`��
	if( rootFrame != NULL ) {
		if( rootFrame->FParent == NULL )
			QueingFrame(rootFrame, glm::mat4());
		else
			QueingFrame(rootFrame, rootFrame->FParent->GetWorldMatrix());
	}

	//{ �L���[�ɓ����Ă镨�̂�`�� }
	Queue->Render(false, viewMat, FVP, flushBBSP);
	SortQueue->Render(FSortAlpha, viewMat, FVP, flushBBSP);

	//�ǂ������킯��Radeon9600�ł�ZENABLE�؂��Ă��������AZFUNC��ݒ肵�Ȃ���TopMost�ɂȂ�Ȃ�
	DG->Get3DDevice()->SetRenderState(D3DRS_ZENABLE, u32(false));
	DG->Get3DDevice()->SetRenderState(D3DRS_ZWRITEENABLE, u32(false));
	DG->Get3DDevice()->SetRenderState(D3DRS_ZFUNC, u32(D3DCMP_ALWAYS));

	TopQueue->Render(false,viewMat, FVP, flushBBSP);  //�őO�ʂɕ\�������X�v���C�g�p

	DG->Get3DDevice()->SetRenderState(D3DRS_ZENABLE, u32(true));
	DG->Get3DDevice()->SetRenderState(D3DRS_ZWRITEENABLE, u32(true));
	DG->Get3DDevice()->SetRenderState(D3DRS_ZFUNC, u32(D3DCMP_LESSEQUAL));


	//�e�N�X�`���ɉ����w�肵�Ȃ���ԂŁA��A�̃����_�����O���I����
	DG->Get3DDevice()->SetTexture(0, NULL);

}

void TSXScene::Clear( u32 dwFlags, u32 color, f32 z, u32 stencil )
{
	DG->Get3DDevice()->Clear(0, NULL, dwFlags,color,z,stencil);

}

void TSXScene::FlushBBSP()
{
	TopQueue->Flush();
	SortQueue->Flush();
	Queue->Flush();

}

TSXBlendMode TSXScene::RegisterBlendMode( TSXBlendInfo& info )
{
	FBlendInfos.push_back(info);
	return (TSXBlendMode)(FBlendInfos.size()-1);
}

void TSXScene::SetProjection( f32 fov, f32 aspect, f32 nearZ, f32 farZ, bool setHorizontalFov /*= true*/ )
{
	f32 q,v,h;

	if( setHorizontalFov ) {
		//������p���ݒ肳�ꂽ
		h = cos(fov*0.5f)/sin(fov*0.5f);
		v = h / aspect;
		FFovH = fov;
		FFovV = atan(1.0f/v)*2.f;
	} else {
		//������p���ݒ肳�ꂽ
		v = cos(fov*0.5f)/sin(fov*0.5f);
		h = v * aspect;
		FFovH = atan(1.0f/h)*2.f;
		FFovV = fov;
	}

	q = farZ / (farZ-nearZ);

	FProjectionMatrix = glm::mat4();

	FProjectionMatrix[0][0]  =   h;                              //
	FProjectionMatrix[1][1]  =   v;                              //
	FProjectionMatrix[2][2]  =   q;                              //   Q = Zf / (Zf-Zn)
	FProjectionMatrix[3][2]  =   1.0f;
	FProjectionMatrix[2][3]  =  -q*nearZ;                        //   -QZn
	FProjectionMatrix[3][3]  =   0.0f;

	DG->Get3DDevice()->SetTransform( D3DTS_PROJECTION, &mat4toD3DMATRIX(FProjectionMatrix));

	FFarZ = farZ;
	FNearZ = nearZ;

	FAspectRatio = aspect;

}

void TSXScene::SetViewPort( u32 left, u32 top, u32 right, u32 bottom )
{
	ZeroMemory(&FVP, sizeof(FVP));

	FVP.X = left;
	FVP.Y = top;
	FVP.Width = right - left;
	FVP.Height = bottom - top;
	FVP.MinZ = 0;
	FVP.MaxZ = 1.0;

	FScreenMatrix = glm::mat4();
	FScreenMatrix[0][0] = FVP.Width/2.0f;
	FScreenMatrix[1][1] = -(FVP.Height/2.0f);
	FScreenMatrix[2][2] = FVP.MaxZ-FVP.MinZ;
	FScreenMatrix[0][3] = FVP.X + FVP.Width/2.0f;
	FScreenMatrix[1][3] = FVP.Y + FVP.Height/2.0f;
	FScreenMatrix[2][3] = FVP.MinZ;


	DG->Get3DDevice()->SetViewport(&FVP);

}

void TSXScene::PushBillboard( TSXFrame* ref, glm::vec3& pos, TSXVertexBB points[], TDGTexture* tex, TSXBlendMode blendMode /*= sxbColorKey*/, bool scaling /*= true*/, bool filter /*= true*/, bool topMost /*= false*/, D3DTEXTUREADDRESS addrU /*= D3DTADDRESS_CLAMP*/, D3DTEXTUREADDRESS addrV /*= D3DTADDRESS_CLAMP*/ )
{
	if( topMost ) {
		TopQueue->PushBillboard(scaling, filter, addrU, addrV, blendMode, ref, pos, tex, points);
	} else {
		if( FBlendInfos[blendMode].Sort ) {
			SortQueue->PushBillboard(scaling, filter, addrU, addrV, blendMode, ref, pos, tex, points);
		} else {
			Queue->PushBillboard(scaling, filter, addrU,addrV, blendMode, ref, pos, tex, points);
		}
	}
	//�t���O�𗧂Ă�
	ref->BBAttached = true;

}

void TSXScene::PushSprite( glm::vec2& pos, TSXVertexSP points[], TDGTexture* tex, TSXBlendMode blendMode /*= sxbColorKey*/, bool filter /*= true*/, bool topMost /*= false*/, f32 rhw /*= 0.0f*/, D3DTEXTUREADDRESS addrU /*= D3DTADDRESS_CLAMP*/, D3DTEXTUREADDRESS addrV /*= D3DTADDRESS_CLAMP*/ )
{
	glm::vec3 ppos;
	ppos.x = pos.x;
	ppos.y = pos.y;

	if( topMost ) {
		ppos.z = 0;
		TopQueue->PushSprite(filter, addrU,addrV, blendMode, ppos, tex, points);
	} else {
		ppos.z = rhw;
		if( FBlendInfos[blendMode].Sort ) {
			SortQueue->PushSprite(filter, addrU,addrV, blendMode, ppos, tex, points);
		} else {
			Queue->PushSprite(filter, addrU,addrV, blendMode, ppos, tex, points);
		}
	}


}

void TSXScene::PushBillboardLite( TSXFrame* ref, glm::vec3& pos, u32 color, f32 size, TDGTexture* tex, TSXBlendMode blendMode /*= sxbColorKey*/, bool scaling /*= true*/, bool filter /*= true*/, bool topMost /*= false*/, u32 specular /*= 0x00000000*/, D3DTEXTUREADDRESS addrU /*= D3DTADDRESS_CLAMP*/, D3DTEXTUREADDRESS addrV /*= D3DTADDRESS_CLAMP*/ )
{
	TSXVertexBB points[4];
	f32 half_dx = size / 2.0f;
	f32 half_dy = size / 2.0f * tex->GetHeight() / tex->GetWidth();
	points[0] = SXVertexBB(-half_dx,-half_dy, color, 0.0f, 0.0f, specular);
	points[1] = SXVertexBB(+half_dx,-half_dy, color, tex->GetU(), 0.0, specular);
	points[2] = SXVertexBB(-half_dx,+half_dy, color, 0.0, tex->GetV(), specular);
	points[3] = SXVertexBB(+half_dx,+half_dy, color, tex->GetU(), tex->GetV(), specular);

	if( topMost ) {
		TopQueue->PushBillboard(scaling, filter, addrU,addrV, blendMode, ref, pos, tex, points);
	} else {
		if( FBlendInfos[blendMode].Sort ) {
			SortQueue->PushBillboard(scaling, filter, addrU,addrV, blendMode, ref, pos, tex, points);
		} else {
			Queue->PushBillboard(scaling, filter, addrU,addrV, blendMode, ref, pos, tex, points);
		}
	}
	//�t���O�𗧂Ă�
	ref->BBAttached = true;


}

void TSXScene::PushSpriteLite( glm::vec2 pos, u32 color, TDGTexture* tex, TSXBlendMode blendMode /*= sxbColorKey*/, bool filter /*= true*/, bool topMost /*= false*/, f32 rhw /*= 0.0f*/, u32 specular /*= 0x00000000*/, D3DTEXTUREADDRESS addrU /*= D3DTADDRESS_CLAMP*/, D3DTEXTUREADDRESS addrV /*= D3DTADDRESS_CLAMP*/ )
{
	glm::vec3 ppos;
	ppos.x = pos.x;
	ppos.y = pos.y;

	TSXVertexSP points[4];
	points[0] = SXVertexSP(0,0, color, 0,0, specular);
	points[1] = SXVertexSP((f32)(tex->GetWidth()-1),0, color, tex->GetU(),0, specular);
	points[2] = SXVertexSP(0,(f32)(tex->GetHeight()-1), color, 0,tex->GetV(), specular);
	points[3] = SXVertexSP((f32)(tex->GetWidth()-1),(f32)(tex->GetHeight()-1), color, tex->GetU(),tex->GetV(), specular);

	if( topMost ) {
		ppos.z = 0;
		TopQueue->PushSprite(filter, addrU,addrV, blendMode, ppos, tex, points);
	} else {
		ppos.z = rhw;
		if( FBlendInfos[blendMode].Sort ) {
			SortQueue->PushSprite(filter, addrU,addrV, blendMode, ppos, tex, points);
		} else {
			Queue->PushSprite(filter, addrU,addrV, blendMode, ppos, tex, points);
		}
	}

}

bool TSXScene::SphereVisibility( TSXFrame* ref, TSphere3D& sphere, f32 depth )
{
	return SphereVisibility(ref->GetWorldMatrix(),  sphere, depth);

}

bool TSXScene::SphereVisibility( glm::mat4& worldM, TSphere3D& sphere, f32 depth )
{

	//���̔��a���O�ȉ��Ȃ�A������͂����Ȃ�
	if( sphere.R <=0 ) {
		return false;
	}

	//p = ������W�n�ł̋��̒��S
	glm::mat4 viewM = FCamera->ViewMatrix() * worldM;
	glm::vec3 p = TransformVec3(sphere.Center, viewM);

	//���E�̌����Adepth��艓���ɂ�����Ă̂͘_�O
	if( (p.z < -sphere.R) || (p.z > depth+sphere.R) ) {
		//�f�o�b�O
		return false;
	}

	//�c�E���̉�p�����߂�
	//DG->Get3DDevice()->GetTransform(D3DTS_PROJECTION, projM);
	f32 fH = FFovH * 0.5f;
	f32 fV = FFovV * 0.5f;

	//��p��Sin,Cos���o���Ă���
	f32 SinFovH = sin(fH);   f32 CosFovH = cos(fH);
	f32 SinFovV = sin(fV);   f32 CosFovV = cos(fV);

	//XZ���ʂł̔���c��p���������̒��S����]���������A�R�[���̒��S���狅�̒��S�܂ł��A���̔��a�̒l�ȉ��ɂȂ邩������
	//���葤�̖�
	f32 sideValue = p.x * CosFovH + p.z * SinFovH;
	if( sideValue < -sphere.R )
		return false;

	//�E�葤�̖�
	sideValue = -p.x * CosFovH + p.z * SinFovH;
	if( sideValue < -sphere.R )
		return false;

	//�㑤�̖�
	sideValue = -p.y * CosFovV + p.z * SinFovV;
	if( sideValue < -sphere.R )
		return false;

	//�����̖�
	sideValue = p.y * CosFovV + p.z * SinFovV;
	if( sideValue < -sphere.R )
		return false;

	return true;
}

TSXLight::TSXLight( TDGCarad* DDCompo, u32 index )
{
	DG = DDCompo;
	FIndex = index;
	FEnabled = false;
	FUpdate = true;

	//�f�t�H�l�̐ݒ�
	ZeroMemory(&Params, sizeof(Params));
	Params.Type = D3DLIGHT_DIRECTIONAL;

	Params.Diffuse.r = 1.0;
	Params.Diffuse.g = 1.0;
	Params.Diffuse.b = 1.0;

	Params.Direction.x = 0.f;
	Params.Direction.y = 0.f;
	Params.Direction.z = 1.f;
}

TSXLight::~TSXLight()
{

}

void TSXLight::BeginUpdate()
{
	FUpdate = false;

}

void TSXLight::EndUpdate()
{
	FUpdate = true;
	DG->Get3DDevice()->SetLight(FIndex, &Params);

}

void TSXLight::SetEnabled( bool Value )
{
	FEnabled  =  Value;

	DG->Get3DDevice()->SetLight(FIndex, &Params);
	DG->Get3DDevice()->LightEnable(FIndex, FEnabled);

}

void TSXLight::SetupDiffuse( f32 _R, f32 _G, f32 _B )
{
	Params.Diffuse.a = 1.0f;
	Params.Diffuse.r = _R;
	Params.Diffuse.g = _G;
	Params.Diffuse.b = _B;

	if( FUpdate )
		DG->Get3DDevice()->SetLight(FIndex, &Params);

}

void TSXLight::SetupSpecular( f32 _R, f32 _G, f32 _B )
{
	Params.Specular.a = 1.0f;
	Params.Specular.r = _R;
	Params.Specular.g = _G;
	Params.Specular.b = _B;

	if( FUpdate )
		DG->Get3DDevice()->SetLight(FIndex, &Params);

}

void TSXLight::SetupAmbient( f32 _R, f32 _G, f32 _B )
{
	Params.Ambient.a = 1.0f;
	Params.Ambient.r = _R;
	Params.Ambient.g = _G;
	Params.Ambient.b = _B;

	if( FUpdate )
		DG->Get3DDevice()->SetLight(FIndex, &Params);

}

void TSXLight::SetupColors( f32 difR, f32 difG, f32 difB, f32 specR, f32 specG, f32 specB, f32 ambR, f32 ambG, f32 ambB )
{
	Params.Diffuse.a = 1.0f;
	Params.Diffuse.r = difR;
	Params.Diffuse.g = difG;
	Params.Diffuse.b = difB;

	Params.Specular.a = 1.0f;
	Params.Specular.r = specR;
	Params.Specular.g = specG;
	Params.Specular.b = specB;

	Params.Ambient.a = 1.0f;
	Params.Ambient.r = ambR;
	Params.Ambient.g = ambG;
	Params.Ambient.b = ambB;

	if( FUpdate )
		DG->Get3DDevice()->SetLight(FIndex, &Params);

}

void TSXLight::SetupRanges( f32 range, f32 att0, f32 att1, f32 att2 )
{
	Params.Range = range;
	Params.Attenuation0 = att0;
	Params.Attenuation1 = att1;
	Params.Attenuation2 = att2;

	if( FUpdate )
		DG->Get3DDevice()->SetLight(FIndex, &Params);

}

void TSXLight::SetupDirectional( glm::vec3& dir )
{
	Params.Type = D3DLIGHT_DIRECTIONAL;
	memcpy( &Params.Direction, &dir, sizeof(glm::vec3));

	if( FUpdate )
		DG->Get3DDevice()->SetLight(FIndex, &Params);

}

void TSXLight::SetupPoint( glm::vec3& pos )
{
	Params.Type = D3DLIGHT_POINT;
	memcpy( &Params.Position, &pos, sizeof(glm::vec3));

	if( FUpdate )
		DG->Get3DDevice()->SetLight(FIndex, &Params);

}

void TSXLight::SetupSpot( glm::vec3& pos, glm::vec3& dir, f32 theta, f32 phi, f32 falloff )
{
	Params.Type = D3DLIGHT_SPOT;
	memcpy( &Params.Direction, &dir, sizeof(glm::vec3));

	Params.Theta = theta;
	Params.Phi = phi;
	Params.Falloff = falloff;

	if( FUpdate )
		DG->Get3DDevice()->SetLight(FIndex, &Params);

}

void TSXLight::FitFrame( TSXFrame* target )
{
	glm::mat4 mat;
	if( target != NULL )
		mat = target->GetWorldMatrix();

	Params.Direction.x = mat[0][2];
	Params.Direction.y = mat[1][2]; 
	Params.Direction.z = mat[2][2];

	Params.Position.x = mat[0][3];
	Params.Position.y = mat[1][3];
	Params.Position.z = mat[2][3];

	if( FUpdate )
		DG->Get3DDevice()->SetLight(FIndex, &Params);

}


TSXVertex SXVertex( f32 x, f32 y, f32 z, f32 nx, f32 ny, f32 nz, u32 dif, u32 spec, f32 tu, f32 tv )
{
	TSXVertex result;
	result.x  = x;
	result.y  = y;
	result.z  = z;
	result.nx = nx;
	result.ny = ny;
	result.nz = nz;

	result.diffuse = dif;
	result.specular = spec;

	result.tu = tu;
	result.tv = tv;

	return result;
}

TSXVertex2T SXVertex2T( f32 x, f32 y, f32 z, f32 nx, f32 ny, f32 nz, u32 dif, f32 tu0, f32 tv0, f32 tu1, f32 tv1 )
{
	TSXVertex2T result;
	result.x  = x;
	result.y  = y;
	result.z  = z;

	result.nx = nx;
	result.ny = ny;
	result.nz = nz;

	result.diffuse = dif;

	result.tu0 = tu0;
	result.tv0 = tv0;

	result.tu1 = tu1;
	result.tv1 = tv1;

	return result;
}

TSXVertexBB SXVertexBB( f32 dx, f32 dy, u32 col, f32 tu, f32 tv, u32 spec/*=0x00000000*/ )
{
	TSXVertexBB result;

	result.dx  = dx;
	result.dy  = dy;

	result.color = col;

	result.tu = tu;
	result.tv = tv;

	result.specular = spec;

	return result;
}

TSXVertexSP SXVertexSP( f32 dx, f32 dy, u32 col, f32 tu, f32 tv, u32 spec/*=0x00000000*/ )
{
	TSXVertexSP result;

	result.dx  = dx;
	result.dy  = dy;

	result.color = col;

	result.tu = tu;
	result.tv = tv;

	result.specular = spec;

	return result;
}

D3DMATERIAL9 MakeMaterial(f32 difR, f32 difG, f32 difB, f32 specR, f32 specG, f32 specB, f32 ambR, f32 ambG, f32 ambB, f32 emsR, f32 emsG, f32 emsB, f32 pow)
{
	D3DMATERIAL9 result;

	result.Diffuse.r = difR;
	result.Diffuse.g = difG;
	result.Diffuse.b = difB;
	result.Diffuse.a = 1.0f;

	result.Specular.r = specR;
	result.Specular.g = specG;
	result.Specular.b = specB;
	result.Specular.a = 1.0f;

	result.Ambient.r = ambR;
	result.Ambient.g = ambG;
	result.Ambient.b = ambB;
	result.Ambient.a = 1.0f;

	result.Emissive.r = emsR;
	result.Emissive.g = emsG;
	result.Emissive.b = emsB;
	result.Emissive.a = 1.0f;

	result.Power = pow;

	return result;
}

D3DMATERIAL9 MakeMaterial(u32 dif, u32 spec, u32 amb, u32 ems, f32 pow)
{
	D3DMATERIAL9 Result;

	Result.Diffuse.a = (f32)(dif >> 24) / 255.0f;
	Result.Diffuse.r = (f32)(dif >> 16 & 0xFF) / 255.0f;
	Result.Diffuse.g = (f32)(dif >> 8 & 0xFF) / 255.0f;
	Result.Diffuse.b = (f32)(dif & 255) / 255.0f;

	Result.Specular.a = (f32)(spec >> 24) / 255.0f;
	Result.Specular.r = (f32)(spec >> 16 & 0xFF) / 255.0f;
	Result.Specular.g = (f32)(spec >> 8 & 0xFF) / 255.0f;
	Result.Specular.b = (f32)(spec & 255) / 255.0f;

	Result.Ambient.a = (f32)(amb >> 24) / 255.0f;
	Result.Ambient.r = (f32)(amb >> 16 & 0xFF) / 255.0f;
	Result.Ambient.g = (f32)(amb >> 8 & 0xFF) / 255.0f;
	Result.Ambient.b = (f32)(amb & 255) / 255.0f;

	Result.Emissive.a = (f32)(ems >> 24) / 255.0f;
	Result.Emissive.r = (f32)(ems >> 16 & 0xFF) / 255.0f;
	Result.Emissive.g = (f32)(ems >> 8 & 0xFF) / 255.0f;
	Result.Emissive.b = (f32)(ems & 255) / 255.0f;

	Result.Power = pow;

	return Result;

}

TSXBlendInfo BlendInfo( bool _sort, bool _alphaTest, D3DCMPFUNC _alphaFunc, bool _alphaBlend, D3DBLEND _srcBlend, D3DBLEND _destBlend, D3DBLENDOP _blendOp )
{
	TSXBlendInfo result;
	result.Sort = _sort;
	result.AlphaTestEnable = _alphaTest;
	result.AlphaFunc = _alphaFunc;
	result.AlphaBlendEnable = _alphaBlend;
	result.SrcBlend = _srcBlend;
	result.DestBlend = _destBlend;
	result.BlendOp = _blendOp;

	return result;
}

TSXMaterial::TSXMaterial( TDGCarad* ADG )
{
	ZeroMemory(&Params, sizeof(Params));
	DG = ADG;
	FUpdate = true;

}

void TSXMaterial::BeginUpdate()
{
	FUpdate = false;

}

void TSXMaterial::EndUpdate()
{
	FUpdate = true;
	DG->Get3DDevice()->SetMaterial(&Params);

}

void TSXMaterial::SetupDiffuse( f32 _R, f32 _G, f32 _B )
{
	Params.Diffuse.r = _R;  Params.Diffuse.g = _G;  Params.Diffuse.b = _B;

	if( FUpdate )
		DG->Get3DDevice()->SetMaterial(&Params);

}

void TSXMaterial::SetupSpecular( f32 _R, f32 _G, f32 _B )
{
	Params.Specular.r = _R;  Params.Specular.g = _G;  Params.Specular.b = _B;

	if( FUpdate )
		DG->Get3DDevice()->SetMaterial(&Params);

}

void TSXMaterial::SetupAmbient( f32 _R, f32 _G, f32 _B )
{
	Params.Ambient.r = _R;  Params.Ambient.g = _G;  Params.Ambient.b = _B;

	if( FUpdate )
		DG->Get3DDevice()->SetMaterial(&Params);

}

void TSXMaterial::SetupEmissive( f32 _R, f32 _G, f32 _B )
{
	Params.Emissive.r = _R;  Params.Emissive.g = _G;  Params.Emissive.b = _B;

	if( FUpdate )
		DG->Get3DDevice()->SetMaterial(&Params);

}

void TSXMaterial::SetupSpecularPower( f32 pow )
{
	Params.Power = pow;

	if( FUpdate )
		DG->Get3DDevice()->SetMaterial(&Params);

}

void TSXMaterial::SetupColors( f32 difR, f32 difG, f32 difB, f32 specR, f32 specG, f32 specB, f32 ambR, f32 ambG, f32 ambB, f32 emsR, f32 emsG, f32 emsB, f32 pow)
{
	Params.Diffuse.r = difR;
	Params.Diffuse.g = difG;
	Params.Diffuse.b = difB;

	Params.Specular.r = specR;
	Params.Specular.g = specG;
	Params.Specular.b = specB;

	Params.Ambient.r = ambR;
	Params.Ambient.g = ambG;
	Params.Ambient.b = ambB;

	Params.Emissive.r = emsR;
	Params.Emissive.g = emsG;
	Params.Emissive.b = emsB;

	Params.Power = pow;

	if( FUpdate )
		DG->Get3DDevice()->SetMaterial(&Params);


}

//v��ϊ����āAZ�l�����Ԃ�
f32 TSXRenderingQueue::TransformedZ( glm::mat4& viewProjScreenM, glm::vec3& v, glm::mat4& worldM)
{
	//�ˉe�s��܂Œʂ��̂ŕϊ������ۂɁA�J���������Z�l��W�ɕԂ�
	return ( (viewProjScreenM * worldM) * glm::vec4(v.x, v.y, v.z, 1.f)).w;
}

//�����������[��
void TSXRenderingQueue::QSort(s32 left, s32 right )
{
	s32 i,j,tmp;
	f32 s,t;

	if( (left<right) ) {
		s = Zs[(left+right) >> 1];
		i = left-1;
		j = right+1;
		while( true ) {

			do{
				i++;
			}
			while(Zs[i]<=s);

			do{
				j--;
			}
			while(Zs[j]>=s);

			if( (i>=j) )
				break;

			t = Zs[i];
			Zs[i] = Zs[j];
			Zs[j] = t;

			tmp = OrderTable[i];
			OrderTable[i] = OrderTable[j];
			OrderTable[j] = tmp;
		}
		QSort(left,i-1);
		QSort(j+1,right);
	}
}

void TSXRenderingQueue::Sort( D3DVIEWPORT9& viewParam )
{
	glm::mat4 mtrx;
	glm::mat4 viewM,projM,screenM,viewProjScreenM;  //�r���[�s��E�ˉe�s��E�X�N���[���s�񁩂R�̍���
	f32 z;
	f32 zC,zMax,zMin;         //���b�V���̉����E�ߕ��ł̔�r�p
	TSXFrame* prevBBFrame;	 //�O�񂩂��ꂽ�r���{�[�h�̎��܂��Ă����t���[�� 

	// ���b�V���̕������A����ȊO�͊e�J�e�S����Z���W�v�Z�ƈꏏ�ɍ��
	for( s32 i = 0 ; i < FMeshCount; i++)
		OrderTable[i] = i;


	//{ �܂��A�S���̃��b�V���EBB��Z�l���v�Z}
	D3DMATRIX projd3d, viewd3d;
	FOwner->Get3DDevice()->GetTransform(D3DTS_PROJECTION, &projd3d);
	FOwner->Get3DDevice()->GetTransform(D3DTS_VIEW, &viewd3d);
	projM = D3DMATRIXtomat4(projd3d);
	viewM = D3DMATRIXtomat4(viewd3d);

	screenM = Scene->GetScreenMatrix();

	viewProjScreenM = screenM * projM * viewM;

	//���b�V���̃\�[�g�pZ�l�����߂�
	for( s32 i = 0 ; i < FMeshCount; i++ ) {

		if( MeshQueue[i].frame->OnQueryZ ) {
			//�J�X�^��Z�v�Z�ɂ��܂���
			MeshQueue[i].frame->OnQueryZ(*MeshQueue[i].frame, *Scene, *FOwner, Zs[i]);

		} else {

			//SortKey�𗊂�ɋ��߂�
			z = 0;
			switch( MeshQueue[i].frame->SortKey ){

				//���b�V���̒��S�Ń\�[�g
			case sxsMeshCenter:
				z = TransformedZ( viewProjScreenM, MeshQueue[i].frame->Mesh->GetSphere().Center, MeshQueue[i].frame->GetMeshMatrixOnRender());
				break;
				//���b�V���̉����ق��Ń\�[�g
			case sxsMeshFar: {
				mtrx = viewProjScreenM * MeshQueue[i].frame->GetMeshMatrixOnRender();
				zC = ( mtrx * Homoginize(MeshQueue[i].frame->Mesh->GetSphere().Center)).w;
				zMax = ( mtrx * Homoginize(MeshQueue[i].frame->Mesh->GetBox().Max)).w;
				zMin = ( mtrx * Homoginize(MeshQueue[i].frame->Mesh->GetBox().Min)).w;

				z = zC + abs(zMax-zMin) * 0.5f;
							 }
							 break;
							 //���b�V���̋߂��Ƃ���Ń\�[�g
			case sxsMeshNear: {
				mtrx = viewProjScreenM * MeshQueue[i].frame->GetMeshMatrixOnRender();
				//���S�̕ϊ���̈ʒu�ƁAAABB�̃G�b�W��Z�����̒��������߂�
				zC = ( mtrx * Homoginize(MeshQueue[i].frame->Mesh->GetSphere().Center)).w;
				zMax = ( mtrx * Homoginize(MeshQueue[i].frame->Mesh->GetBox().Max)).w;
				zMin = ( mtrx * Homoginize(MeshQueue[i].frame->Mesh->GetBox().Min)).w;

				z = zC - abs(zMax-zMin) * 0.5f;
							  }
							  break;
			}

			Zs[i] = z;
		}

	}


	//�r���{�[�h��Z�l�����߂�
	prevBBFrame = NULL;
	FVisibleBillboardCount = 0;
	for( s32 i = 0 ; i < FBillboardCount; i++ ) {
		//mtrx�ɕϊ��s�����
		if( prevBBFrame != BillboardQueue[i].ref ) {
			if( ! BillboardQueue[i].ref->ValidMatrixOnRender() )
				BillboardQueue[i].ref->CalcMatrixOnRender();

			mtrx = (viewProjScreenM * BillboardQueue[i].ref->MatrixOnRender());
			prevBBFrame = BillboardQueue[i].ref;
		}

		BillboardQueue[i].pos = mtrx * BillboardQueue[i].pos;

		//W<0�Ȃ�A�`�悵�Ȃ��c�\�[�g�̑Ώۂ������Čy���Ȃ�
		if( BillboardQueue[i].pos.w > 0 ) {
			Zs[FVisibleBillboardCount + FMeshCount] = BillboardQueue[i].pos.w;
			OrderTable[FVisibleBillboardCount+FMeshCount] = i + FMeshCount;
			FVisibleBillboardCount++;
		}
	}

	//�X�v���C�g��Z�l�����߂�
	FVisibleSpriteCount = 0;
	for( s32 i = 0 ; i < FSpriteCount; i++ ) {
		//if( SpriteQueue[i].pos.z > 0 ) {  //rhw�ł͂Ȃ��A�J�����t���[�����W�w��̏ꍇ
		if( SpriteQueue[i].pos.z >= 0 ) {
			Zs[FMeshCount+FVisibleBillboardCount+FVisibleSpriteCount] = SpriteQueue[i].pos.z;
			OrderTable[FMeshCount+FVisibleBillboardCount+FVisibleSpriteCount] = i + FMeshCount + FBillboardCount;
			FVisibleSpriteCount++;
		}
	}


	//�N�C�b�N�\�[�g�c�w�^�Ƀn�b�V���Ƃ�������A���܂�r���{�[�h�Ƃ��������Ȃ������͂�����̕����g�N
	QSort(0, FMeshCount + FVisibleBillboardCount + FVisibleSpriteCount - 1);


	//�ȏ��OrderTable�쐬����

}

void TSXRenderingQueue::SetBillBoardCapacity( s32 Value )
{
	FBillboardCapacity  =  Value;
	BillboardQueue.resize(Value);

	OrderTable.resize(FBillboardCapacity + FMeshCapacity + FSpriteCapacity);
	Zs.resize( FBillboardCapacity + FMeshCapacity + FSpriteCapacity);

}

void TSXRenderingQueue::SetMeshCapacity( s32 Value )
{
	FMeshCapacity  =  Value;
	MeshQueue.resize(Value);

	OrderTable.resize( FBillboardCapacity + FMeshCapacity + FSpriteCapacity);
	Zs.resize( FBillboardCapacity + FMeshCapacity + FSpriteCapacity);

}

void TSXRenderingQueue::SetSpriteCapacity( s32 Value )
{
	FSpriteCapacity  =  Value;
	SpriteQueue.resize( Value);

	OrderTable.resize(FBillboardCapacity + FMeshCapacity + FSpriteCapacity);
	Zs.resize( FBillboardCapacity + FMeshCapacity + FSpriteCapacity);

}

TSXRenderingQueue::TSXRenderingQueue( TDGCarad* AOwner, TSXScene* _Scene )
{
	FOwner = AOwner;
	Scene = _Scene;

	//�f�t�H�l�̑��
	FBillboardCapacity = 32;
	FMeshCapacity = 32;
	FSpriteCapacity = 32;

	FBillboardCount = 0;
	FMeshCount = 0;
	FSpriteCount = 0;

	AlphaRef = 128;

}

TSXRenderingQueue::~TSXRenderingQueue()
{
	BillboardQueue.clear();
	MeshQueue.clear();
	SpriteQueue.clear();

	OrderTable.clear();
	Zs.clear();

}

void TSXRenderingQueue::PushMesh( TSXFrame* frame )
{
	//�K�v�Ȃ�g������
	if( FMeshCount >= FMeshCapacity )
		FMeshCapacity = FMeshCapacity * 2;

	MeshQueue[FMeshCount].frame = frame;

	FMeshCount++;

}

void TSXRenderingQueue::PushBillboard( bool scaling, bool filter, D3DTEXTUREADDRESS addrU, D3DTEXTUREADDRESS addrV, TSXBlendMode blendMode, TSXFrame* ref, glm::vec3& pos, TDGTexture* tex, TSXVertexBB pts[] )
{
	//�K�v�Ȃ�g������
	if( FBillboardCount >= FBillboardCapacity )
		FBillboardCapacity = FBillboardCapacity * 2;


	BillboardQueue[FBillboardCount].scaling = scaling;
	BillboardQueue[FBillboardCount].filter = filter;
	BillboardQueue[FBillboardCount].addrU = addrU;
	BillboardQueue[FBillboardCount].addrV = addrV;
	BillboardQueue[FBillboardCount].blend = blendMode;
	BillboardQueue[FBillboardCount].tex = tex;
	BillboardQueue[FBillboardCount].ref = ref;
	BillboardQueue[FBillboardCount].pos = Homoginize(pos);
	memmove(&BillboardQueue[FBillboardCount].pts[0], &pts[0], sizeof(TSXVertexBB)*4);  //4���_���R�s�[

	FBillboardCount++;

}

void TSXRenderingQueue::PushSprite( bool filter, D3DTEXTUREADDRESS addrU, D3DTEXTUREADDRESS addrV, TSXBlendMode blendMode, glm::vec3& pos, TDGTexture* tex, TSXVertexSP pts[] )
{
	//�K�v�Ȃ�g������
	if( FSpriteCount >= FSpriteCapacity )
		FSpriteCapacity = FSpriteCapacity * 2;

	SpriteQueue[FSpriteCount].filter = filter;
	SpriteQueue[FSpriteCount].addrU = addrU;
	SpriteQueue[FSpriteCount].addrV = addrV;
	SpriteQueue[FSpriteCount].blend = blendMode;
	SpriteQueue[FSpriteCount].tex = tex;
	SpriteQueue[FSpriteCount].pos = pos;
	memmove(&SpriteQueue[FSpriteCount].pts[0], &pts[0], sizeof(TSXVertexSP)*4);  //4���_���R�s�[

	FSpriteCount++;

}


//�u�����h���[�h�̕ύX
void TSXRenderingQueue::SetBlendMode(TSXBlendMode bm)																
{																					
	/*���܂ł̂Ɠ����������牽�����Ȃ��ėǂ�*/										
	if( prevBlend == bm )															
		return;																		
	if( Scene->GetBlendInfos(bm).AlphaBlendEnable ) {													
		/*���u�����h����*/														
		FOwner->Get3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, (true));						
		/*Z�������݂Ȃ�(��r�͗L��)*/											
		FOwner->Get3DDevice()->SetRenderState(D3DRS_ZWRITEENABLE, (false));							
		FOwner->Get3DDevice()->SetRenderState(D3DRS_SRCBLEND, (Scene->GetBlendInfos(bm).SrcBlend));							
		FOwner->Get3DDevice()->SetRenderState(D3DRS_DESTBLEND, (Scene->GetBlendInfos(bm).DestBlend));						
		if( Scene->BlendOpEnable() )												
			FOwner->Get3DDevice()->SetRenderState(D3DRS_BLENDOP, (Scene->GetBlendInfos(bm).BlendOp));						
	} else {																	
		/*���u�����h�Ȃ�*/														
		FOwner->Get3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, (false));						
		/*Z�������݂���	*/														
		FOwner->Get3DDevice()->SetRenderState(D3DRS_ZWRITEENABLE, (true));	
	}																			
	if( Scene->GetBlendInfos(bm).AlphaTestEnable ) {														
		FOwner->Get3DDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, (Scene->GetBlendInfos(bm).AlphaTestEnable));			
		FOwner->Get3DDevice()->SetRenderState(D3DRS_ALPHAFUNC, (Scene->GetBlendInfos(bm).AlphaFunc));						
		FOwner->Get3DDevice()->SetRenderState(D3DRS_ALPHAREF, (AlphaRef));							
	} else {																	
		FOwner->Get3DDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, (Scene->GetBlendInfos(bm).AlphaTestEnable));			
	}																			
	prevBlend = bm;																	
}

//�t�B���^�̐ݒ�(BB,�X�v���C�g�p)
void TSXRenderingQueue::SetFilterAddrBBSP(bool enable, D3DTEXTUREADDRESS addrU, D3DTEXTUREADDRESS addrV)														
{																									
	if( prevDrawnIsMesh ) {																		
		/*�O�Ƀ��b�V���`���Ă��Ȃ�A�t�B���^��ԕۑ�*/											
		FOwner->Get3DDevice()->GetSamplerState(0, D3DSAMP_MIPFILTER, (DWORD*)(&storedFilterMip));		
		FOwner->Get3DDevice()->GetSamplerState(0, D3DSAMP_MAGFILTER, (DWORD*)(&storedFilterMag));		
		FOwner->Get3DDevice()->GetSamplerState(0, D3DSAMP_MINFILTER, (DWORD*)(&storedFilterMin));		
		FOwner->Get3DDevice()->GetSamplerState(0, D3DSAMP_ADDRESSU, (DWORD*)(&storedAddrU));			
		FOwner->Get3DDevice()->GetSamplerState(0, D3DSAMP_ADDRESSU, (DWORD*)(&storedAddrV));			
	}																							
	if( (enable != prevFilter) || prevDrawnIsMesh ) {											
		if( enable ) {																			
			FOwner->Get3DDevice()->SetSamplerState(0, D3DSAMP_MIPFILTER, (D3DTEXF_NONE));		
			FOwner->Get3DDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, (D3DTEXF_LINEAR));		
			FOwner->Get3DDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, (D3DTEXF_LINEAR));		
		} else {																				
			FOwner->Get3DDevice()->SetSamplerState(0, D3DSAMP_MIPFILTER, (D3DTEXF_NONE));		
			FOwner->Get3DDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, (D3DTEXF_POINT));		
			FOwner->Get3DDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, (D3DTEXF_POINT));		
		}																						
		prevFilter = enable;																	
	}																							
	if( (addrU != prevAddrU) || prevDrawnIsMesh ) {												
		FOwner->Get3DDevice()->SetSamplerState(0, D3DSAMP_ADDRESSU, (addrU));				
		prevAddrU = addrU;																		
	}																							
	if( (addrV != prevAddrV) || prevDrawnIsMesh ) {												
		FOwner->Get3DDevice()->SetSamplerState(0, D3DSAMP_ADDRESSV, (addrV));				
		prevAddrV = addrV;																		
	}																							
	prevDrawnIsMesh = false;																	
}


void TSXRenderingQueue::RestoreFilterAddr()																		
{																							
	FOwner->Get3DDevice()->SetSamplerState(0, D3DSAMP_MIPFILTER, (storedFilterMip));		
	FOwner->Get3DDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, (storedFilterMag));		
	FOwner->Get3DDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, (storedFilterMin));		
	FOwner->Get3DDevice()->SetSamplerState(0, D3DSAMP_ADDRESSU, (storedAddrU));			
	FOwner->Get3DDevice()->SetSamplerState(0, D3DSAMP_ADDRESSV, (storedAddrV));			
}


//�����_�����O�X�e�[�g����̐ݒ�
void TSXRenderingQueue::SetDevs(TSXBlendMode blend, TDGTexture* tex, bool spec, bool lit, u32 wrap, D3DFILLMODE fill)								
{																			
	SetBlendMode(blend);													
	if( prevTex != tex ) {													
		if( tex = NULL )													
			FOwner->Get3DDevice()->SetTexture(0, NULL);						
		else																
			FOwner->Get3DDevice()->SetTexture(0, tex->GetTexture());				
		prevTex = tex;														
	}																		
	if( prevSpec != spec ) {												
		FOwner->Get3DDevice()->SetRenderState(D3DRS_SPECULARENABLE, spec);	
		prevSpec = spec;													
	}																		
	if( prevLit != lit ) {													
		FOwner->Get3DDevice()->SetRenderState(D3DRS_LIGHTING, lit);			
		prevLit = lit;														
	}																		
	if( prevWrap != wrap ) {												
		FOwner->Get3DDevice()->SetRenderState(D3DRS_WRAP0, wrap);			
		prevWrap = wrap;													
	}																		
	if( prevFill != fill ) {												
		FOwner->Get3DDevice()->SetRenderState(D3DRS_FILLMODE, fill);		
		prevFill = fill;													
	}																		
}

void TSXRenderingQueue::DrawMesh( u32 idx)																						
{																										
	/*�t�B���^*/																						
	if( ! prevDrawnIsMesh ) {																			
		RestoreFilterAddr();																			
		prevDrawnIsMesh = true;																			
	}																									
	SetDevs(MeshQueue[idx].frame->BlendMode, MeshQueue[idx].frame->GetTexture(), MeshQueue[idx].frame->Specular, MeshQueue[idx].frame->Lighting, MeshQueue[idx].frame->TextureWrap, MeshQueue[idx].frame->FillMode);		
	/*�s��̐ݒ�*/																						
	FOwner->Get3DDevice()->SetTransform(D3DTS_WORLD, &mat4toD3DMATRIX(MeshQueue[idx].frame->GetMeshMatrixOnRender()));	
	/*�}�e���A���̐ݒ�*/																				
	FOwner->Get3DDevice()->SetMaterial(&MeshQueue[idx].frame->Material);									
	/*�I�[�i�[�h���E�Ȃ�A�C�x���g�n���h�����Ă�*/														
	if( MeshQueue[idx].frame->OnRender ) {																
		/*�C�x���g���Ă�ŃI�[�i�[�h���E*/																
		MeshQueue[idx].frame->OnRender(*MeshQueue[idx].frame, *Scene, *FOwner);							
		/*�e�N�X�`���߂�*/																				
		if( prevTex != NULL )																			
			FOwner->Get3DDevice()->SetTexture(0, prevTex->GetTexture());										
		else																							
			FOwner->Get3DDevice()->SetTexture(0, NULL);													
	} else {																							
		/*�`��*/																						
		MeshQueue[idx].frame->Mesh->Draw();																
	}																									
}

//�r���{�[�h�̕`��
void TSXRenderingQueue::DrawBB(  bool needSort, u32 idx )																								
{																												
	/*�t�B���^*/																								
	SetFilterAddrBBSP(BillboardQueue[idx].filter, BillboardQueue[idx].addrU,BillboardQueue[idx].addrV);			
	/*�����_�����O�X�e�[�g�Ƃ��̐ݒ�*/																			
	SetDevs(BillboardQueue[idx].blend, BillboardQueue[idx].tex, true, false, 0, D3DFILL_SOLID);										
	/*�r���{�[�h�̒��S�̎�����W�����߂�*/																		
	if( ! needSort ) {																							
		if( prevFrame != BillboardQueue[idx].ref ) {															
			if( BillboardQueue[idx].ref == NULL )																
				m = viewProjScreenM;																			
			else {																								
				if( ! BillboardQueue[idx].ref->ValidMatrixOnRender() )												
					BillboardQueue[idx].ref->CalcMatrixOnRender();												
				m = viewProjScreenM * BillboardQueue[idx].ref->MatrixOnRender();									
			}																									
			prevFrame = BillboardQueue[idx].ref;																	
		}																										
		center = BillboardQueue[idx].pos;																		
		center = m * center;																					
	} else {																									
		/*�\�[�g�ς݂̏ꍇ�́A�\�[�g�̒i�K�ŕϊ�����Ă���*/													
		center = BillboardQueue[idx].pos;																		
	}																											
	/*W�l�����Ȃ�`��*/																							
	if( center.w > 0.f ) {																						
		/*�r���{�[�h�̒��S�̉�ʂł̍��W�����߂�*/																
		center = ViewFrustumToScreen(center);																
		if( BillboardQueue[idx].scaling ) {																		
			/*�X�P�[�����O����*/																				
			for( u32 j = 0 ; j <= 3; j++ ) {																	
				pts[j].sx = center.x + BillboardQueue[idx].pts[j].dx * ratio_dx * center.w;	
				pts[j].sy = center.y + BillboardQueue[idx].pts[j].dy * ratio_dy * center.w;	
				pts[j].sz = center.z;														
				pts[j].rhw = center.w;														
				pts[j].color = BillboardQueue[idx].pts[j].color;														
				pts[j].tu = BillboardQueue[idx].pts[j].tu;																
				pts[j].tv = BillboardQueue[idx].pts[j].tv;																
			}																									
		} else {																								
			/*�X�P�[�����O�Ȃ�*/																				
			for( u32 j = 0 ; j <= 3; j++ ) {																	
				pts[j].sx = center.x + BillboardQueue[idx].pts[j].dx;						
				pts[j].sy = center.y + BillboardQueue[idx].pts[j].dy;					
				pts[j].sz = center.z;													
				pts[j].rhw = center.w;													
				pts[j].color = BillboardQueue[idx].pts[j].color;													
				pts[j].tu = BillboardQueue[idx].pts[j].tu;															
				pts[j].tv = BillboardQueue[idx].pts[j].tv;															
			}																								
		}																									
		FOwner->Get3DDevice()->SetFVF(D3DFVF_TLVERTEX);														
		FOwner->Get3DDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &pts, sizeof(D3DTLVERTEX));			
	}																										
	/*�Q�ƃt���[���ɕt�����t���O������*/																	
	if( BillboardQueue[idx].ref != NULL )																	
		BillboardQueue[idx].ref->BBAttached = false;														
}

//�X�v���C�g�̕`��
void TSXRenderingQueue::DrawSP( u32 idx )																									
{																													
	/*�t�B���^*/																										
	SetFilterAddrBBSP(SpriteQueue[idx].filter, SpriteQueue[idx].addrU, SpriteQueue[idx].addrV);						
	SetDevs(SpriteQueue[idx].blend, SpriteQueue[idx].tex, true, false, 0, D3DFILL_SOLID);											
	/*Z�l�����Ȃ�`��*/																								
	if( SpriteQueue[idx].pos.z >= 0.f ) {																			
		/*Z�l�����̂܂�Z�o�b�t�@�ɕ`���c���̃I�u�W�F�N�g�Ƃ�Z�Ɋւ��鐮����������邪�A�X�v���C�g�����炢����*/		
		f32 bz = SpriteQueue[idx].pos.z;																				
		for( u32 i = 0 ; i <= 3 ; i++ ) {																			
			pts[i].sx = SpriteQueue[idx].pos.x + SpriteQueue[idx].pts[i].dx;								
			pts[i].sy = SpriteQueue[idx].pos.y + SpriteQueue[idx].pts[i].dy;								
			pts[i].sz = bz;																					
			pts[i].rhw = 0.5f;        /*�Ă��Ɓ[	*/														
			pts[i].color = SpriteQueue[idx].pts[i].color;													
			pts[i].tu = SpriteQueue[idx].pts[i].tu;															
			pts[i].tv = SpriteQueue[idx].pts[i].tv;															
		}																									
		FOwner->Get3DDevice()->SetFVF(D3DFVF_TLVERTEX);														
		FOwner->Get3DDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &pts, sizeof(D3DTLVERTEX));			
	}																										
}

void TSXRenderingQueue::Render( bool needSort, glm::mat4& viewMat, D3DVIEWPORT9& viewParam, bool doFlush /*= true*/ )
{
	//�r���{�[�h�p�A������W����� �̕ϊ��s��̌v�Z
	D3DMATRIX tmpMat;
	FOwner->Get3DDevice()->GetTransform(D3DTS_PROJECTION, &tmpMat);
	projM = D3DMATRIXtomat4(tmpMat);
	screenM = Scene->GetScreenMatrix();

	viewProjScreenM = projM * viewMat;
	viewProjScreenM = screenM * viewProjScreenM;

	//�X�v���C�g�p�A���_���W�nZ��Z�o�b�t�@�ϊ��p�W���̌v�Z
	zN = Scene->GetNearZ();
	zF = Scene->GetFarZ();
	zMin = Scene->GetFVP().MinZ;
	zMax = Scene->GetFVP().MaxZ;

	//�r���{�[�h�p�A�r���[�|�[�g�̃T�C�Y
	//W = 1�̃r���{�[�h�́A���̃T�C�Y�ŕ\�������
	ratio_dx = viewParam.Width / 2.0f * projM[0][0];
	ratio_dy = viewParam.Height / 2.0f * projM[1][1];

	//�ϐ��̏�����
	prevFrame = NULL;

	prevTex = NULL;
	FOwner->Get3DDevice()->SetTexture(0,NULL);

	prevSpec = true;
	FOwner->Get3DDevice()->SetRenderState(D3DRS_SPECULARENABLE, u32(true));

	prevLit = true;
	FOwner->Get3DDevice()->SetRenderState(D3DRS_LIGHTING, u32(true));

	prevFill = D3DFILL_SOLID;
	FOwner->Get3DDevice()->SetRenderState(D3DRS_FILLMODE, (D3DFILL_SOLID));

	prevWrap = 0;
	FOwner->Get3DDevice()->SetRenderState(D3DRS_WRAP0, 0);

	prevDrawnIsMesh = true;

	//�s�����̂̃��u�����h�ݒ�
	prevBlend = sxbDefault;
	FOwner->Get3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, u32(false));  //�u�����h���Ȃ�
	FOwner->Get3DDevice()->SetRenderState(D3DRS_ZENABLE, u32(true));            //Z��r����
	FOwner->Get3DDevice()->SetRenderState(D3DRS_ZWRITEENABLE, u32(true));       //Z�������݂���

	//�f�t�H���g�̃e�N�X�`���u�����h�ݒ�
	FOwner->Get3DDevice()->SetTextureStageState(0, D3DTSS_COLORARG1, u32(D3DTA_TEXTURE));
	FOwner->Get3DDevice()->SetTextureStageState(0, D3DTSS_COLORARG2, u32(D3DTA_DIFFUSE));
	FOwner->Get3DDevice()->SetTextureStageState(0, D3DTSS_COLOROP,   u32(D3DTOP_MODULATE));

	FOwner->Get3DDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, u32(D3DTA_TEXTURE));
	FOwner->Get3DDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG2, u32(D3DTA_DIFFUSE));
	FOwner->Get3DDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP,   u32(D3DTOP_MODULATE));


	//�t�B���^�̐ݒ�c�͂��Ȃ��Ă���(�r���{�[�h�E�X�v���C�g�͌X�Ɏ����Ă��āA���b�V����RenderState�ɏ����邩��)

	//�e�N�X�`���A�h���X���[�h�̎w������l�ɁA�����Ƃ��悢
	//FOwner->Get3DDevice()->SetSamplerState(0, D3DSAMP_ADDRESSU, u32(D3DTADDRESS_CLAMP));
	//FOwner->Get3DDevice()->SetSamplerState(0, D3DSAMP_ADDRESSV, u32(D3DTADDRESS_CLAMP));


	if( ! needSort ) {
		//���b�V���`��
		for( s32 i = 0 ; i < FMeshCount; i++ ) {
			DrawMesh(i);
		}
		//�r���{�[�h�`��
		for( s32 i = 0 ; i < FBillboardCount; i++ ) {
			DrawBB(needSort, i);
		}
		//�X�v���C�g�`��
		for( s32 i = 0 ; i< FSpriteCount; i++ ) {
			DrawSP(i);
		}
	} else {
		//�\�[�g���āA�I�[�_�����O�e�[�u���ɏ������܂ꂽ���ŕ`��
		Sort(viewParam);

		//�\�[�g�̒i�K�Ńr���{�[�h�ɂ��Ă͉��E�s��������ς�
		for( s32 i = 0 ; i < FMeshCount + FVisibleBillboardCount + FVisibleSpriteCount; i++ ) {
			if( OrderTable[i] < FMeshCount ) {
				DrawMesh(OrderTable[i]);
			} else if( OrderTable[i] < FMeshCount + FBillboardCount ) {
				DrawBB(needSort,  OrderTable[i] - FMeshCount);
			} else {
				DrawSP(OrderTable[i] - FMeshCount - FBillboardCount);
			}
		}
	}

	//�Ō�ɏ����ꂽ�̂��X�v���C�g���r���{�Ȃ�A�t�B���^��Ԃ�߂�
	if( ! prevDrawnIsMesh )
		RestoreFilterAddr();

	//�L���[�����
	if( doFlush )
		Flush();

}

void TSXRenderingQueue::Flush()
{
	FBillboardCount = 0;
	FMeshCount = 0;  
	FSpriteCount = 0;

}

TSXStateRestorer::TSXStateRestorer( TDGCarad* ADG )
{
	DG = ADG;
	Memory(); //�o�b�t�@�N���A

}

HRESULT TSXStateRestorer::SetRS( D3DRENDERSTATETYPE RSType, u32 Value )
{
	//��������ĂȂ��t���O�Ȃ�A�ۑ����Ă���
	if( (RSType & RSFlag) == 0 ) {
		RSFlag = RSFlag + RSType;
		DG->Get3DDevice()->GetRenderState(RSType, (DWORD*)&SavedRS[RSType]);
		RSList[RSCount] = RSType;
		RSCount++;
	}

	return DG->Get3DDevice()->SetRenderState(RSType, u32(Value));

}

HRESULT TSXStateRestorer::SetTSS( u32 Stage, D3DTEXTURESTAGESTATETYPE TSSType, u32 Value )
{
	//��������ĂȂ��t���O�Ȃ�A�ۑ����Ă���
	if( (TSSType & TSSFlag[Stage])==0 ) {
		TSSFlag[Stage] = TSSFlag[Stage] + TSSType;
		DG->Get3DDevice()->GetTextureStageState(Stage, TSSType, (DWORD*)&SavedTSS[Stage][TSSType]);
		TSSList[Stage][TSSCount[Stage]] = TSSType;
		TSSCount[Stage]++;
	}

	return DG->Get3DDevice()->SetTextureStageState(Stage, TSSType, u32(Value));


}

HRESULT TSXStateRestorer::SetSSS( u32 Stage, D3DSAMPLERSTATETYPE SSSType, u32 Value )
{
	//��������ĂȂ��t���O�Ȃ�A�ۑ����Ă���
	if( (SSSType & SSSFlag[Stage])==0 ) {
		SSSFlag[Stage] = SSSFlag[Stage] + SSSType;
		DG->Get3DDevice()->GetSamplerState(Stage, SSSType, (DWORD*)&SavedSSS[Stage][SSSType]);
		SSSList[Stage][SSSCount[Stage]] = SSSType;
		SSSCount[Stage]++;
	}

	return DG->Get3DDevice()->SetSamplerState(Stage, SSSType, u32(Value));

}

void TSXStateRestorer::Memory()
{
	//�t���O�����A����
	RSFlag = 0;
	RSCount = 0;

	for( u32 i = 0 ; i < SX_MAX_TSS; i++ ) {
		TSSFlag[i] = 0;
		TSSCount[i] = 0;
	}

}

void TSXStateRestorer::Restore()
{
	for( s32 i = 0 ; i < RSCount; i++ ) {
		DG->Get3DDevice()->SetRenderState(RSList[i], SavedRS[RSList[i]]);
	}

	for( u32 i = 0 ; i < SX_MAX_TSS; i++ ) {
		for( s32 j = 0 ; j < TSSCount[i]; j++ ) {
			DG->Get3DDevice()->SetTextureStageState(i, TSSList[i][j], SavedTSS[i][TSSList[i][j]]);
		}
	}

}

TSXLight* TSXLightGroup::GetLights( u32 idx )
{
	//�͈̓`�F�b�N
	if( idx >= FCapacity ) {
		//DGCarad_PutDebugMessage('TSXLightGroup & SXLib: �͈͊O�̃��C�g���Q�Ƃ��悤�Ƃ��܂���');
		return NULL;
	}

	return FLights[idx];

}

TSXLight* TSXLightGroup::GetUnusedLight()
{
	for( u32 i = 0 ; i < FCapacity; i++ ) {
		if( ! FLights[i]->GetEnabled() ) {
			return FLights[i];
		}
	}

	return NULL;
}

void TSXLightGroup::Recover()
{
	for( u32 i = 0 ; i < FCapacity; i++ ) {
		if( FLights[i]->GetEnabled() )
			FLights[i]->SetEnabled(true);
	}

}

TSXLightGroup::TSXLightGroup( TDGCarad* ADG, u32 capacity, string debugName /*= ""*/ )
{
	DG = ADG;
	ResID = DG->RegisterResouce(this, debugName);
	FDebugName = debugName;

	FCapacity = capacity;
	FLights.resize(capacity);

	for( u32 i = 0 ; i < capacity; i++ ) {
		FLights[i] = new TSXLight(DG, i);
	}

}

TSXLightGroup::~TSXLightGroup()
{
	DG->UnregisterResource(ResID);

	for( u32 i = 0 ; i < FCapacity; i++ ) {
		SAFE_DELETE(FLights[i]);
	}
	FLights.clear();
}

void TSXLightGroup::EnableAll()
{
	for( u32 i = 0 ; i < FCapacity; i++ ) {
		FLights[i]->SetEnabled(true);
	}
}

void TSXLightGroup::DisableAll()
{
	for( u32 i = 0 ; i < FCapacity; i++ ) {
			FLights[i]->SetEnabled(false);
	}

}
