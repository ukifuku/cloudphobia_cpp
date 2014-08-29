#pragma once

#include "types.h"
#include <string>
#include <sstream>
#include <d3d9.h>
#include <d3d9types.h>
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "DGCarad9.h"

/* �VSX�`���ɂ���

name   | ofs          | length                  | comment
----------------------------------------------------------------------------------------------------
Signature |            0 |                      16 | 'Simplified_X02  '
nVextices |           16 |                       2 | number of vertices
nIndices  |           18 |                       2 | number of indices for( s32 vertices
dwFVF     |           20 |                       4 | flags for( s32 flexible vertex format
VertexSize|           24 |                       4 | required bytes per vertex
DXVersion |           28 |                       4 | DirectXVersion(for( s32 Identification of FVF)

(padding)

Vertices  |          256 |  VertexSize * nVertices | vertices
Indices   | 256+Vertices |           2 * nIndices  | indices for( s32 vertices


DXVersion�͏�����DirectX�A�b�v�O���[�h�ւ̕z��
*/


/* SX�`���ɂ���(�Â�)

name   | ofs          | length         | comment
----------------------------------------------------------------------------
Signature |            0 |             16 | 'Simplified_X00  '
nVextices |           16 |              2 | number of vertices
nIndices  |           18 |              2 | number of indices for( s32 vertices

(padding)

Vertices  |          256 | 40 * nVertices | vertices(TSXVertex)
Indices   | 256+Vertices |  2 * nIndices  | indices for( s32 vertices


�@SX01�`���́A�����J�o�[�W�����A�T�|�[�g���Ȃ�
 */

using namespace std;

//SX�t�@�C�����ʎq
const char SX00_SIGNATURE[] = "Simplified_X00  ";  //�Â�SX
const char SX02_SIGNATURE[] = "Simplified_X02  ";  //�VSX�A�t���L�V�u�����_�t�H�[�}�b�g(DX8)�Ή�

const char DXVER_FOR_FVF[] = "0800";                //FVF���e���ʗp��DirectX�̃o�[�W����


//�z�肷��e�N�X�`���X�e�[�W�E�T���v���X�e�[�W�̏��
#define	SX_MAX_TSS  8
#define	SX_MAX_SSS  8


//TSXVertex�p�AFVF�t���O
#define	FVF_SXVERTEX  (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1)

//TSXVertex�A�����X�L���p�AFVF�t���O(�X�y�L��������)
#define	FVF_SXV_ONESKIN  (D3DFVF_XYZB1 | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//TSXVertex�A�e�N�X�`���Q���p(�X�y�L��������)
#define	FVF_SXVERTEX2T  (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX2)

#define D3DFVF_TLVERTEX	(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//��`�ς݃u�����h���[�h
enum TSXBlendMode {
	sxbDefault = 0,     //���ʁA�����Ȃ�
	sxbAdd,         //���Z����
	sxbAlpha,       //������
	sxbMulNeg,      //�G�Z���Z����(���]���Ă���Mul)
	sxbMul,         //��Z����
	sxbColorKey    //���e�X�g�����A�����Ȃ�
};


class TSXMesh;
class TSXFrame;
class TSXFrameList;
class TSXScene;
class TSXRenderingQueue;
class TSXTextWriter;
class TSXStateRestorer;

f32 sint[4096];
f32 cost[4096];

// ������
void SXInit();

//SX�`���̂��߂̒��_�f�[�^
struct TSXVertex 
{
	union{
		struct{
			f32 x,y,z;     //���_
			f32 nx,ny,nz;  //�@��
			u32 diffuse;   //�f�B�t���[�Y
			u32 specular;  //�X�y�L����
			f32 tu,tv;       //�e�N�X�`�����W
		};
		struct{
			glm::vec4 pos;
			glm::vec4 normal;
		};
	};
};

//�}���`�e�N�X�`���Ή�SXVertex
struct TSXVertex2T
{
	union{
		struct{
			f32 x,y,z;     //���_
			f32 nx,ny,nz;  //�@��
			u32 diffuse;   //�f�B�t���[�Y
			f32 tu0,tv0;       //�e�N�X�`�����W
			f32 tu1,tv1;       //�e�N�X�`�����W(2)
		};
		struct{
			glm::vec4 pos;
			glm::vec4 normal;
		};
	};
};

//�����X�L���p���_�f�[�^
struct TSXVertexOneSkin 
{
	union{
		struct{
			f32 x,y,z;     //���_
			f32 b;         //�x�[�^
			f32 nx,ny,nz;  //�@��
			u32 diffuse;   //�f�B�t���[�Y
			f32 tu,tv;       //�e�N�X�`�����W
		};
		struct{
			glm::vec4 pos;
			f32 beta;
			glm::vec4 normal;
		};
	};
};


//�r���{�[�h�̂��߂̒��_���
struct TSXVertexBB 
{
	//union{
	//struct{
	f32 dx,dy;     //���S����̑��΍��W
	u32 color;   //�F
	u32 specular;
	f32 tu,tv;     //�e�N�X�`�����W
	//	};
	//	glm::vec2 size;
	//};
};

//�X�v���C�g�̂��߂̒��_���
struct TSXVertexSP 
{
	f32 dx,dy;         //���S����̑��΍��W(�X�N���[�����W�n)
	u32 color;       //�F
	u32 specular;
	f32 tu,tv;         //�e�N�X�`�����W
};


//�u�����h���[�h��`���
struct TSXBlendInfo 
{
	bool Sort;               //Z�\�[�g����K�v������
	//�ȉ��A�����_�����O�X�e�[�g�ݒ�p
	bool AlphaTestEnable;    //�J���[�L�[�����p�A���e�X�g�Afalse�Ȃ�AlphaFunc�͖���
	D3DCMPFUNC AlphaFunc;          //���e�X�g�p��r�֐�

	bool	AlphaBlendEnable;   //�����A���̃����o��false�Ȃ�A�ȉ��̐ݒ�̓X�L�b�v
	D3DBLEND SrcBlend;
	D3DBLEND DestBlend;
	D3DBLENDOP BlendOp;              //BlendOp���ݒ�ł���r�f�I�J�[�h�̂�
};

struct D3DTLVERTEX //DirectX9.0�ł̓T�|�[�g���ĂȂ��̂Œ�`���Ȃ���
{
	f32 sx;
	f32 sy;
	f32 sz;
	f32 rhw;
	u32 color;
	f32 tu;
	f32 tv;
};

struct D3DTLVERTEX2 
{
	f32 sx;
	f32 sy;
	f32 sz;
	f32 rhw;
	u32 color;
	u32 specular;
	f32 tu;
	f32 tv;
	f32 bu;
	f32 bv;
};

//������
enum TSXVisibility{
	sxvShow,        //������
	sxvHide,        //�B���B�q�t���[�����܂߂�
	sxvHideMyself   //�B���B�q�t���[���͉B��Ȃ��B
};

//���b�V���`��̃\�[�g���ɉ�����ɂ��邩
enum TSXSortKey {
	sxsMeshCenter,  //���b�V���̒��S
	sxsMeshFar,     //���b�V���̉�����
	sxsMeshNear     //���b�V���̋߂���
};

//���W�ϊ��ɂ��Ẵ|���V�[
enum TSXFrameStyle { 
	sxfsDefault,      //�ӂ�
	sxfsMeshContainer //�e�t���[���Ə�ɓ����ϊ��s������A�����Ƀ��b�V���i�[�ɂƂ��Ă̖������ʂ���
};


//�C�x���g�n���h��
typedef void (*TSXRenderEvent)(TSXFrame&, TSXScene&, TDGCarad& );
typedef void (*TSXQueryZEvent)(TSXFrame*, TSXScene*, TDGCarad*, f32& ZValue );
typedef void (*TSXQueryVisibilityEvent)(TSXFrame*, TSXScene*, TDGCarad*, TSXVisibility& );

//�N���X�Q

//BOX(AABB)
struct TBox3D
{
	glm::vec3 Min,Max;
};

//��
struct TSphere3D 
{
	union 
	{
		struct{
			f32 X,Y,Z,R;
		};
		struct{
			glm::vec3 Center;
			f32 Radius;
		};
	};
};

//�����c�_A,B���n�_�E�I�_�Ƃ������
struct TLineSeg3D 
{
	union 
	{
		struct{
			glm::vec3 A,B;
		};
		struct{
			glm::vec3 _Start,_End;
		};
	};
};


//�~���c Axis�����ɂ���A���aRadius�̉~��
struct TCapsule3D 
{
	union 
	{
		struct{
			TLineSeg3D Axis;
			f32 Radius;
		};
		struct{
			glm::vec3 A,B;
			f32 R;
		};
	};
};


//���b�V���p�N���X
class TSXMesh
{
	friend class TSXScene;
protected:
	TDGCarad* FOwner;                //�F�X�����b�ɂȂ�DDDD�I�u�W�F�N�g

	TDGVertexBuffer*  FVBuf;          //VertexBuffer
	TDGIndexBuffer*  FIBuf;           //IndexBuffer
	string FVBufName,FIBufName;     //�f�o�b�O���O�ɓf���o�����AVertexBuffer��IndexBuffer�̖��O

	u32 FFaceCount;                //�ʂ̐��A��ɃC���f�N�X��/3

	TBox3D FBox;                    //AABB
	TSphere3D FSphere;              //�O�ڋ�
	TCapsule3D FCapsule;            //�O�ڃJ�v�Z��

	u32 MaxPrims;                 //��x�ɕ`��ł���ő�̃v���~�e�B�u��
	u32 MaxIndex;                 //���_�C���f�N�X�̍ő�l
	bool IsSameVs(u8* pV, u32 v1, u32 v2);

	void GetFace(u16* dest, u16 faceIdx, vector<u16>& Indices)
	{
		*dest = Indices[faceIdx*3];
		dest++;
		*dest = Indices[faceIdx*3+1];
		dest++;
		*dest = Indices[faceIdx*3+2];
	}

	//���̖ʂ̓L���b�V���q�b�g���Ă�H(�ŋ�nCacheEntry�܂ł̃C���f�N�X�Ɠ����H)
	//�ʂ̂����L���b�V���q�b�g���Ă钸�_�̐���Ԃ�
	s32 HitCount(u16 faceIdx, s32 nCacheEntry, s32 nOrdered, vector<u16>& Indices)
	{
		s32 result = 0;

		for( u32 i = 0; i <= 2; i++ ) {
			//�ʂ��\�����邻�ꂼ��̓_�ɂ��Ē��_�L���b�V�������炤
			for( s32 j = 1; j <= nCacheEntry; j++ ) {
				if( nOrdered < j )
					break;
				if( Indices[nOrdered-j] == Indices[faceIdx*3+i] ) {
					result++;
					break;
				}
			}
		}
		return result;
	}

	s32 TotalHitCount( u16 faceIdx, s32 nCacheEntry, s32 nOrdered, vector<u16>& Indices )
	{
		s32 result = 0;

		for( s32 i = 0; i <= 2; i++ ) {
			//�ʂ��\�����邻�ꂼ��̓_�ɂ��Ē��_�L���b�V�������炤
			for( s32 j = 1 ; j <= nCacheEntry-i ; j++) {
				if( nOrdered < j )
					break;
				if( Indices[nOrdered-j] == Indices[faceIdx*3+i] ) {
					result++;
				}
			}
		}

	}

	void SwapFace( u16 a, u16 b, vector<u16>& Indices )
	{
		for( u32 i = 0 ; i <= 2 ; i++ ) {
			u16 tmp = Indices[a*3+i];
			Indices[a*3+i] = Indices[b*3+i];
			Indices[b*3+i] = tmp;
		}
	}
public:

	TSXMesh(TDGCarad* AOwner, string VBufName = "", string IBufName = "");
	~TSXMesh();

	void Draw();                                                 //DrawIndexedPrimitive�őS���`��
	void DrawPartial( u32 start, u32 primCount );            //DrawIndexedPrimitive�ŕ����`��

	void LoadFromStream( istream& ss );
	void LoadFromFile( string fileName, string qdaName = "");
	void SaveToFile( string filename );
	void SaveToStream( ostream& ss );

	void SetupBuffers( u32 newVertexCount, u32 newIndexCount, u32 FVF,
		u32 VertexSize, D3DPOOL Pool );

	void Compact();            //�������_���܂Ƃ߂�
	void Optimize();           //VCache���ǂ������悤�ɍœK������
	void UpdateBounds();       //���b�V����AABB�ƁA�O�ڋ��̔��a���v�Z
	void Assign(TSXMesh& source); //���b�V���̓��e���R�s�[

	TDGVertexBuffer* VBuf() { return FVBuf; }
	TDGIndexBuffer*  IBuf() { return FIBuf; }
	u32 FaceCount() { return FFaceCount; }

	TBox3D& GetBox() { return FBox; }
	TSphere3D& GetSphere() { return FSphere; }
	TCapsule3D& GetCapsule() { return FCapsule; }

	string VBufName() { return FVBufName; }
	string IBufName() { return FIBufName; }
};


//���b�V���̃��X�g
class TSXMeshList 
{
private:
	vector<TSXMesh*> m_meshes;
	TDGCarad* DG;
	TSXMesh* GetMesh( s32 idx );
	void SetMesh( s32 idx, TSXMesh* Value );
public:
	TSXMeshList(TDGCarad& ADG);
	~TSXMeshList(); 
	void Delete( s32 idx );
	void Clear(); 
	void LoadFromStream( istream& Source );
	void LoadFromFile( string filename, string qdaName = "");
	void SaveToStream(ostream& Dest);
	void SaveToFile(string filename);
	void Add(TSXMesh* _mesh);
};


//�t���[��
class TSXFrame 
{
friend class TSXScene;
friend class TSXRenderingQueue;
protected:
	glm::mat4 FMatrix;            //�ϊ��s��(�����̍��W�n���e���W�n)
	glm::mat4 FMeshMatrix;        //���b�V���`�掞�̕ϊ��s��c���b�V�����W���������W
	TDGTexture* FTextures[SX_MAX_TSS]; //�e�N�X�`��

	TSXFrameList* FAncestors;  //�c��t���[���B
	TSXFrameList* FChildren;   //�q�t���[���B
	TSXFrame* FParent;         //�e�t���[��(Ancestors[Ancestors.Count-1]�Ɠ�)
	TSXVisibility FVisibility;   //������

	bool FEnableMeshT;         //���[�J�����W�����b�V�����W�ւ̕ϊ����s��(MeshMatrix�L��)
	TSXFrameStyle FStyle;         //���W�ϊ��ɂ��Ẵ|���V�[

	bool FRendered;            //�Ō��Render���\�b�h���s���ɁA�J�����O����Ă��Ȃ�
	bool FValidRenderedMatrix; //RenderedMatrix�́A�Ō��Render���\�b�h���s���̃V�[���Ɉ�v���Ă���

	glm::mat4 RenderedMatrix;     //�Ō�Ƀ����_�����O���ꂽ�Ƃ��̃��[���h�s��(�r���{�[�h�̕`��Ɏg�p)
	glm::mat4 RenderedMeshMatrix; //�Ō�Ƀ����_�����O���ꂽ�Ƃ��̃��b�V���s��
	bool BBAttached;           //�r���{�[�h���������Ă�H
	bool FEnableBS;

	void RebuildAncestors( TSXFrame* me );
public:
	//���g
	TSXMesh* Mesh;                       //�������ރ��b�V��
	D3DMATERIAL9 Material;               //�}�e���A��


	void SetTexture(TDGTexture* Value );
	void SetTextures( s32 idx, TDGTexture* Value );
	void SetParent(TSXFrame* Value);
	TDGTexture* GetTextures( s32 idx );


	bool isBBAttached() {return BBAttached;}
	//�����_�����O�X�e�[�g�̐���
	TSXBlendMode BlendMode;              //�`�掞�̃u�����h���[�h
	bool Lighting;                       //true:���C�e�B���O������, false:���_�̐F�����̂܂܎g��
	bool Specular;                       //�X�y�L�����n�C���C�g������
	u32 TextureWrap;                     //���b�v�̕��@
	D3DFILLMODE FillMode;                //�h���(�|�C���g/���C��/�|���S��)

	//�C�x���g�n���h��
	TSXRenderEvent OnRender;                    //�I�[�i�[�h���E
	TSXQueryZEvent OnQueryZ;                    //�I�[�i�[�h���E�ŕ`����镨�̂�Z�l�ANil�̏ꍇ�A���b�V����SortKey�ʒu�Ń\�[�g
	TSXQueryVisibilityEvent OnQueryVisibility;  //�J�����O����ANil�̏ꍇ�A���b�V���̃o�E���f�B���O���ŃJ�����O

	//���낢��
	TSphere3D Bounds;                       //�t���[���̊O�ڋ��A������p
	s32 Tag;                            //�����l�A�Ȃ�ɂł��g����
	s32 RenderAttr;                     //�����l�A�Ȃ�ɂł��g���Ă̂Q
	void* UserData;                       //�����|�C���^

	//�\�[�g����
	TSXSortKey SortKey;                     //������ɂ��ă\�[�g���邩

	//�R���X�g���N�^�E�f�X�g���N�^
	TSXFrame(TSXFrame* parentFrame);
	~TSXFrame(); 

	//�e�q�֌W
	TSXFrameList* GetAncestors() { return FAncestors; }
	TSXFrameList* GetChildren() { return FChildren; }
	TSXFrame* GetParent() { return FParent; }

	TSXFrame* Ancestors(u32 _idx);
	TSXFrame* Children(u32 _idx);
	void ClearChildren();

	//������
	TSXVisibility GetVisibility() { return FVisibility; }
	void	SetVisibility(TSXVisibility _vis) { FVisibility = _vis; }

	//�t���[���̊O�ڋ��ŁA���ʃt���[���܂ŃJ�����O����
	bool GetEnableBounds() { return FEnableBS; }
	void	SetEnableBounds(bool _val) { FEnableBS = _val; }

	//�������i�V�Ƃ��āA���݂̉��ʃt���[���܂ł̏�Ԃ���o�E���f�B���O�����v�Z
	//onlyMyself��false�̎��́A���ʃt���[���̃o�E���f�B���O�����X�V
	void UpdateBounds( bool onlyMyself );
	void ActivateBounds();     //���ʃt���[�����܂߂đS���̃t���[����BS��Enable��
	void DeactivateBounds();   //���ʃt���[�����܂߂đS���̃t���[����BS��Disable��

	//���W�ϊ��֌W
	glm::mat4& GetMatrix() { return FMatrix; }
	void	SetMatrix(glm::mat4& _mat) { FMatrix = _mat; }
	glm::mat4& MatrixOnRender() { return RenderedMatrix; }               //OnRender�C�x���g���Ȃǂŗ��p����A���ۂɕ`�悳�ꂽ�Ƃ��̃t���[�������[���h�ϊ��s�� 
	glm::mat4& GetMeshMatrix() { return FMeshMatrix; }
	void SetMeshMatrix(glm::mat4& _mat) { FMeshMatrix = _mat;}    //���b�V���`�掞�̕ϊ��s��c���b�V�����W���������W
	glm::mat4& GetMeshMatrixOnRender() { return RenderedMeshMatrix; }
	bool GetEnableMeshTransform() { return FEnableMeshT; }
	void SetEnableMeshTransform( bool _val ) { FEnableMeshT = _val;} //���b�V�����W���������W�̕ϊ����s��
	TSXFrameStyle GetStyle() { return FStyle; }
	void SetStyle(TSXFrameStyle _style ){ FStyle = _style; }              //���W�ϊ��ɂ��Ẵ|���V�[
	bool Rendered() { return FRendered; }
	void SetRendered(bool val) { FRendered = val; }
	bool ValidMatrixOnRender() { return FValidRenderedMatrix; }
	void SetTexture( u32 idx, TDGTexture* _tex ) { FTextures[idx] = _tex; }
	TDGTexture* GetTexture();

	void SetTransform(TSXFrame* ref, const glm::mat4& mat);
	glm::mat4  GetTransform(TSXFrame* ref);

	glm::vec3 LocalToWorld(const glm::vec3& vec);           //�t���[�����ł̍��W���A���[���h���W�ɕϊ�����
	glm::vec3 WorldToLocal(glm::vec3& vec);           //���[���h���W���t���[�����̍��W�ɕϊ�����
	glm::vec3 LocalToWorldRot(glm::vec3& rotvec);     //�t���[�����Ō����x�N�g�����A���[���h���W�ł̌����x�N�g���ɕϊ�
	glm::vec3 WorldToLocalRot(glm::vec3& rotvec);     //���[���h���W���̌����x�N�g�����t���[�����̌����x�N�g���ɕϊ�����

	void CalcMatrixOnRender();                             //�Ō��Render���ɑ������ϊ��s������߂�B�J�����O���ꂽ�t���[���̕ϊ��s���m�肽�����Ȃǂɗp����

	//�ʒu
	void SetTranslation(TSXFrame* ref, const glm::vec3& pos);
	glm::vec3 GetTranslation(TSXFrame* ref);

	//�p��
	//�c���x�N�g���Őݒ�E�擾
	void SetOrientation(TSXFrame* ref, const glm::vec3& vecZ, const glm::vec3& vecY);
	void GetOrientation(TSXFrame* ref, glm::vec3& vecZ, glm::vec3& vecY);

	//�c�}�g���N�X��^���Ďp�������ݒ�
	void SetOrientationMatrix(TSXFrame* ref, const glm::mat4& mat);
	//�c�N�H�[�^�j�I����^���Ďp�������ݒ�
	void SetOrientationQuat(TSXFrame* ref, const glm::quat& quat);


	//���֗�
	//�cZ����target�t���[�����̍��Wpos��������BY����world�t���[������Y���Ɍ����BY���̌X����bank�x�Ƃ���
	void LookAt(TSXFrame* target, const glm::vec3& pos, TSXFrame* world, const s32 bank);
	glm::mat4 ViewMatrix();  //���̃t���[�����J�����ɂ����ꍇ�̃r���[�s��𐶐�

	void SetBlendModeRcv(TSXBlendMode Value); //�ċA�I�Ƀu�����h���[�h���Z�b�g����
	void SetLightingRcv( bool Value );
	void SetSpecularRcv(bool Value);
	void SetTextureWrapRcv(u32 Value);
	void SetFillModeRcv(D3DFILLMODE Value);
	void SetMaterialRcv(D3DMATERIAL9& Value);
	void SetTexturesRcv(s32 idx, TDGTexture* Value);

	void DumpFrameStats(); //�t���[���̏�Ԃ��f�o�b�O���O�ɏo��

	glm::mat4 GetWorldMatrix();
	glm::mat4 GetWorldMatrixRot();
};

//�t���[���̃��X�g
class TSXFrameList 
{
private:
	vector<TSXFrame*> m_frame;
	TSXFrame* GetFrame( s32 idx );
	void SetFrame( s32 idx, TSXFrame* Value );
public:
	TSXFrameList();
	~TSXFrameList(); 
	void Assign(TSXFrameList* source);
	u32 GetSize() { return m_frame.size(); }
	void Add( TSXFrame* _item );
	void Delete(TSXFrame* _item);
	void Clear();
	TSXFrame* GetItem(u32 _idx) { return _idx < m_frame.size() ? m_frame[_idx] : NULL; }
};




//�V�[���S��
class TSXScene : public TDGCommonResource
{
protected:
	TSXStateRestorer* FStateRestorer;  //�����_�����O�X�e�[�g���C���N��
	TSXFrame* FCamera;
	TDGCarad* DG;
	u32 ResID;                     //DGCarad�ɊǗ������Ƃ��̃��\�[�XID
	string FDebugName;              //�f�o�b�O���O�ɏo�͂���閼�O

	glm::mat4 FProjectionMatrix;
	glm::mat4 FScreenMatrix;
	D3DVIEWPORT9 FVP;
	f32 FFovH,FFovV,FAspectRatio,FNearZ,FFarZ;    //�ˉe�s��̃p�����[�^


	bool FBlendOpEnable;             //���̊���BLENDOP�̐ݒ肪�ł��邩�H
	bool FSortAlpha;                 //�������̂̃\�[�g���s��
	vector<TSXBlendInfo> FBlendInfos;  //�u�����h���i�[�p

	void InspectDevCaps();          //DEVCAPS�Ɋ֌W������̍X�V
public:
	TSXRenderingQueue* TopQueue;    //��Ɉ�ԏ�ɕ`�悳��镨�̂̂��߂̃L���[�A�������̂̃\�[�g���͂��Ȃ�
	TSXRenderingQueue* Queue;       //�\�[�g���Ȃ�(��ʂɁA�s�����̗p�̃L���[)
	TSXRenderingQueue* SortQueue;   //�\�[�g����(��ʂɓ����̗p�̃L���[)

	//�R���X�g���N�^�E�f�X�g���N�^
	TSXScene(TDGCarad* ADG, string debugName = "");
	~TSXScene(); 

	//�`��
	void Render(TSXFrame* rootFrame, bool flushBBSP = true);        //rootFrame�ȉ���`��
	void Clear( u32 dwFlags, u32 color, f32 z, u32 stencil );  //�o�b�N�o�b�t�@�̃N���A
	void FlushBBSP();  //�r���{�[�h�E�X�v���C�g�̂��߂̃L���[���t���b�V������

	//��ʐݒ�
	bool GetSortAlpha() { return FSortAlpha; }
	void SetSortAlpha(bool val) { FSortAlpha = val; }
	TSXBlendMode RegisterBlendMode(TSXBlendInfo& info);  //�V�����u�����h���[�h��ǉ�����

	//�ϊ�����̐ݒ�
	void SetProjection( f32 fov, f32 aspect, f32 nearZ, f32 farZ, bool setHorizontalFov = true);    //���e�ϊ��̐ݒ�
	void SetViewPort(u32 left, u32 top, u32 right, u32 bottom);        //�r���[�|�[�g

	//���̐ݒ�ɂ���Č��肳���v���p�e�B
	f32 GetFarZ() { return FFarZ; }                           //SetProjection�Ō��肳���l�A�ȉ��S�������B
	f32 GetNearZ() { return FNearZ; }
	f32 GetFovH() { return FFovH; }                             //Fov�̒P�ʂ̓��W�A��
	f32 GetFovV() { return FFovV; }                            //Fov�̒P�ʂ̓��W�A��
	f32 GetAspectRatio() { return FAspectRatio; }
	D3DVIEWPORT9& GetFVP() { return FVP; }
	glm::mat4& GetProjectionMatrix() { return FProjectionMatrix; }
	glm::mat4& GetScreenMatrix() { return FScreenMatrix; } //�X�N���[���ϊ��s��

	//�r���{�[�h�E�X�v���C�g
	void PushBillboard (TSXFrame* ref, glm::vec3& pos, 
		TSXVertexBB points[], TDGTexture* tex,
		TSXBlendMode blendMode = sxbColorKey, bool scaling = true,
		bool filter = true, bool topMost = false,
		D3DTEXTUREADDRESS addrU = D3DTADDRESS_CLAMP,
		D3DTEXTUREADDRESS addrV = D3DTADDRESS_CLAMP);
	void PushSprite(glm::vec2& pos, TSXVertexSP points[],
		TDGTexture* tex, TSXBlendMode blendMode = sxbColorKey,
		bool filter = true, bool topMost = false,
		f32 rhw = 0.0f,
		D3DTEXTUREADDRESS addrU = D3DTADDRESS_CLAMP,
		D3DTEXTUREADDRESS addrV = D3DTADDRESS_CLAMP);
	//����̃��\�b�h�ł́A���W�̎w�肪�ʓ|���̂ŁA���W�w��̊ȒP�ȑ���A
	//��]�Ƃ����o���Ȃ�����
	void PushBillboardLite (TSXFrame* ref, glm::vec3& pos, u32 color,
		f32 size, TDGTexture* tex, TSXBlendMode blendMode = sxbColorKey,
		bool scaling = true, bool filter = true, bool topMost = false,
		u32 specular = 0x00000000,
		D3DTEXTUREADDRESS addrU = D3DTADDRESS_CLAMP,
		D3DTEXTUREADDRESS addrV = D3DTADDRESS_CLAMP);
	void PushSpriteLite(glm::vec2 pos, u32 color,
		TDGTexture* tex, TSXBlendMode blendMode = sxbColorKey,
		bool filter = true, bool topMost = false, f32 rhw = 0.0f,
		u32 specular = 0x00000000,
		D3DTEXTUREADDRESS addrU = D3DTADDRESS_CLAMP,
		D3DTEXTUREADDRESS addrV = D3DTADDRESS_CLAMP);


	//�����胆�[�e�B���e�B
	//ref�t���[�����ɂ���A���͌����邩�A�A���A������depth�ȏ�Ȃ猩���Ȃ����̂Ƃ���
	bool SphereVisibility(TSXFrame* ref, TSphere3D& sphere, f32 depth);
	//���Ɠ����A�������Aref�����[���h���W�܂ł̃}�g���N�X�����܂��Ă���Ƃ���
	bool SphereVisibility(glm::mat4& worldM, TSphere3D& sphere, f32 depth);

	//�����_�����O�X�e�[�g�L�����u
	TSXStateRestorer* GetStateRestorer() { return FStateRestorer; }

	//���̑�
	bool BlendOpEnable() { return FBlendOpEnable; } //�u�����h���[�h��ǉ�����ۂ̎w�j
	void QueingFrame( TSXFrame* frame, const glm::mat4& mat);


	TSXBlendInfo& GetBlendInfos(u32 idx) {return FBlendInfos[idx];}  //�u�����h���i�[�p

	void SetCamera( TSXFrame* _cam ) { FCamera = _cam; }
	TSXFrame* GetCamera() { return FCamera; }

	TDGCarad* GetDG() { return DG; }

	void Recover();              //�r���[�|�[�g�Ȃǂ̏�Ԃ����X�g�A
	void Cleanup(){};  
	void Reset(){};                             
	void AfterReset(){};   
};


//���C�g
class TSXLight 
{
protected:
	TDGCarad* DG;
	u32 FIndex;       //SetLight�ɓn���A�C���f�N�X
	bool FEnabled;
	bool FUpdate;    //�p�����[�^�̕ύX���A�����f
public:
	D3DLIGHT9 Params;

	TSXLight(TDGCarad* DDCompo, u32 index);
	~TSXLight();

	void BeginUpdate();   //Params�̓��e���ADirect3D�ɓ`���Ȃ�����
	void EndUpdate();     //Params�̓��e���ADirect3D�ɓ`���Ȃ�������Ԃ�����

	void SetEnabled(bool Value);
	bool GetEnabled() { return FEnabled; }

	//���֗����[�`��
	void SetupDiffuse (f32 _R, f32 _G, f32 _B);
	void SetupSpecular(f32 _R, f32 _G, f32 _B);
	void SetupAmbient(f32 _R, f32 _G, f32 _B);

	void SetupColors(f32 difR, f32 difG, f32 difB, f32 specR, f32 specG, f32 specB, f32 ambR, f32 ambG, f32 ambB);
	void SetupRanges(f32 range, f32 att0, f32 att1, f32 att2);

	void SetupDirectional(glm::vec3& dir);
	void SetupPoint(glm::vec3& pos);
	void SetupSpot(glm::vec3& pos, glm::vec3& dir, f32 theta, f32 phi, f32 falloff);

	void FitFrame(TSXFrame* target);  //�t���[���̈ʒu�E�����ɃZ�b�g����
};


//�����̃��C�g���Ǘ�����I�u�W�F�N�g
class TSXLightGroup : public TDGCommonResource
{
protected:
	vector<TSXLight*> FLights;
	TDGCarad* DG;
	u32 ResID;             //DGCarad�ɊǗ�����鎞�̃��\�[�XID
	string FDebugName;      //�f�o�b�O���O�ɏo�͂���閼�O
	u32 FCapacity;        //�Ǘ����郉�C�g�̐�
	TSXLight* GetLights(u32 idx);
	TSXLight* GetUnusedLight();
public:
	TSXLightGroup(TDGCarad* ADG, u32 capacity, string debugName = "");
	~TSXLightGroup();

	TSXLight* operator[](u32 i) { return FLights[i];}
	void Recover();      //�𑜓x��ς�����Ȃǂ̃��J�o��
	void EnableAll();    //�S���_����
	void DisableAll();   //�S������
	void Cleanup(){};  
	void Reset(){};                             
	void AfterReset(){};   
};



class TSXMaterial 
{
private:
	TDGCarad* DG;
	bool FUpdate;
public:
	D3DMATERIAL9 Params;
	TSXMaterial(TDGCarad* ADG);

	void BeginUpdate();   //Params�̓��e���ADirect3D�ɓ`���Ȃ�����
	void EndUpdate();     //Params�̓��e���ADirect3D�ɓ`���Ȃ�������Ԃ�����

	void SetupDiffuse (f32 _R, f32 _G, f32 _B);
	void SetupSpecular(f32 _R, f32 _G, f32 _B);
	void SetupAmbient (f32 _R, f32 _G, f32 _B);
	void SetupEmissive (f32 _R, f32 _G, f32 _B);
	void SetupSpecularPower(f32 pow);

	void SetupColors( f32 difR, f32 difG, f32 difB, f32 specR, f32 specG, f32 specB, f32 ambR, f32 ambG, f32 ambB, f32 emsR, f32 emsG, f32 emsB, f32 pow);
};



//���b�V���`��L���[�ɓ������
struct TSXMeshInfo 
{
	TSXFrame* frame;     //���̃��b�V����`�����Ƃ����t���[���͂ǂ�H
};

//�r���{�[�h�`��L���[�ɓ������
struct TSXBillboardInfo 
{
	bool scaling;      //���_����̋����ŃX�P�[�����O����H
	bool filter;       //�t�B���^�����O����H
	D3DTEXTUREADDRESS addrU,addrV;  //�e�N�X�`���A�h���b�V���O���[�h
	TSXBlendMode blend;
	TSXFrame* ref;         //��t���[��
	glm::vec4 pos;        //��t���[�����ł̈ʒu(�v�Z�p�ɂ��g��)
	TDGTexture *tex;       //�e�N�X�`��
	TSXVertexBB pts[4];  //���_�f�[�^
};

//2D�X�v���C�g�L���[�ɓ������
struct TSXSpriteInfo 
{
	bool filter;       //�t�B���^�����O����H
	D3DTEXTUREADDRESS addrU,addrV; //�e�N�X�`���A�h���b�V���O���[�h
	TSXBlendMode blend;
	TDGTexture *tex;
	glm::vec3 pos;                    //���S�_(x,y�͉�ʍ��W�Az�͎��_���W�n��z)
	TSXVertexSP pts[4];  //���_�f�[�^
};

//�����_�����O�̏�����ۂ��߂̃o�b�t�@�A�����̂������łȂ����ŏ�����ς���B
class TSXRenderingQueue 
{
private:
	TDGCarad *FOwner;   //�F�X�����b�ɂȂ�DGCarad�I�u�W�F�N�g
	TSXScene* Scene;

	vector<TSXMeshInfo> MeshQueue;
	vector<TSXBillboardInfo> BillboardQueue;
	vector<TSXSpriteInfo>	SpriteQueue;

	vector<s32> OrderTable;        //�`�揇���e�[�u��
	vector<f32> Zs;                 //Z���W�\�[�g�p

	s32 FMeshCapacity;              //���b�V������~����e��
	s32 FBillboardCapacity;
	s32 FSpriteCapacity;

	s32 FMeshCount;                 //�L���[�ɓ����Ă鐔
	s32 FBillboardCount;
	s32 FSpriteCount;

	s32 FVisibleBillboardCount;     //��ʂɓ����Ă�r���{�[�h�̐�(�\�[�g���Ɍ���)
	s32 FVisibleSpriteCount;        //��ʂɓ����Ă�X�v���C�g�̐�(�\�[�g���Ɍ���)


	TDGTexture* prevTex;   //�O��g�����e�N�X�`���ւ̎Q��
	bool prevSpec;     //�O��`�悵�����̂́A�X�y�L�������v�Z������
	bool prevLit;      //�O��`�悵�����̂́A���C�e�B���O�̌v�Z��������
	TSXFrame* prevFrame;   //���O�ɕ`�悵���r���{�[�h�̓����Ă��t���[��
	TSXBlendMode prevBlend;
	u32 prevWrap;       //�e�N�X�`�����b�v
	D3DFILLMODE prevFill;      //�t�B�����[�h
	bool prevFilter;
	bool prevDrawnIsMesh;

	D3DTEXTUREADDRESS prevAddrU, prevAddrV;
	D3DTEXTUREADDRESS storedAddrU, storedAddrV;
	D3DTEXTUREFILTERTYPE storedFilterMag, storedFilterMin, storedFilterMip;

	glm::vec4 center;
	D3DTLVERTEX pts[4];
	glm::mat4 m;
	glm::mat4 projM, screenM, viewProjScreenM;

	f32 zF, zN; //���߃N���b�s���O�ʂ܂ł̋���(�X�v���C�g��Z�o�b�t�@�ւ̏������ݒl�Z�o�p)
	f32 zMax, zMin;

	f32 ratio_dx, ratio_dy; //�r���[�|�[�g�̔����̃T�C�Y�A�r���{�[�h�`�掞�̕␳�l�Ƃ���


	void Sort(D3DVIEWPORT9& viewParam);   //�����_�����O�O�\�[�g
	void SetBillBoardCapacity(s32 Value);
	void SetMeshCapacity(s32 Value);
	void SetSpriteCapacity(s32 Value);
	void QSort(s32 left, s32 right );
	f32 TransformedZ( glm::mat4& viewProjScreenM, glm::vec3& v, glm::mat4& worldM);
public:
	s32 AlphaRef;   //�J���[�L�[�̎��̃�REF�B�������̒l�ȉ��Ȃ甲�����
	TSXRenderingQueue(TDGCarad* AOwner, TSXScene* _Scene);
	~TSXRenderingQueue(); 
	void PushMesh(TSXFrame* frame); //�t���[���ɂ��������b�V�� //(blendMode:TSXBlendMode; mesh:TSXMesh; frame:TSXFrame; mat:D3DMatrix; tex:TDGTexture; mtrl:D3DMATERIAL8;  event:TSXRenderEvent; ZEvent:TSXQueryZEvent);
	void PushBillboard(bool scaling, bool filter, D3DTEXTUREADDRESS addrU, D3DTEXTUREADDRESS addrV, TSXBlendMode blendMode, TSXFrame* ref, glm::vec3& pos, TDGTexture* tex, TSXVertexBB pts[] ); //�r���{�[�h
	void PushSprite(bool filter, D3DTEXTUREADDRESS addrU, D3DTEXTUREADDRESS addrV, TSXBlendMode blendMode, glm::vec3& pos, TDGTexture* tex, TSXVertexSP pts[] ); //�X�v���C�g

	void Render(bool needSort, glm::mat4& viewMat, D3DVIEWPORT9& viewParam, bool doFlush = true); //�����_�����O
	void Flush();  //�����_�����O�����A�o�b�t�@�̒������
	void SetBlendMode(TSXBlendMode bm);
	void SetFilterAddrBBSP(bool enable, D3DTEXTUREADDRESS addrU, D3DTEXTUREADDRESS addrV);
	void RestoreFilterAddr();
	void SetDevs(TSXBlendMode blend, TDGTexture* tex, bool spec, bool lit, u32 wrap, D3DFILLMODE fill);
	void DrawMesh( u32 idx);
	void DrawBB( bool needSort, u32 idx );
	void DrawSP( u32 idx );
	//property MeshCapacity:Integer read FMeshCapacity write SetMeshCapacity;
	//property BillboardCapacity:Integer read FBillboardCapacity write SetBillBoardCapacity;
	//property SpriteCapacity:Integer read FSpriteCapacity write SetSpriteCapacity;
	//property MeshCount:Integer read FMeshCount;
	//property BillboardCount:Integer read FBillboardCount;
	//property SpriteCount:Integer read FSpriteCount;
};




//�����_�����O�X�e�[�g�L�����u
class TSXStateRestorer 
{
private:
	TDGCarad* DG;

	u32 SavedRS[256];  //�����_�����O�X�e�[�g�o�b�t�@
	u32 RSFlag;         //�e�����_�����O�X�e�[�g���X�V���ꂽ���̃t���O
	D3DRENDERSTATETYPE RSList[256]; //�X�V���ꂽ�����_�����O�X�e�[�g���X�g
	s32 RSCount;     //�X�V���ꂽ�����_�����O�X�e�[�g�̎�ނ̃J�E���g

	u32 SavedTSS[SX_MAX_TSS][64];  //TSS�o�b�t�@
	u32 TSSFlag[SX_MAX_TSS];    //TSS���X�V���ꂽ���ǂ���
	D3DTEXTURESTAGESTATETYPE TSSList[SX_MAX_TSS][64];
	s32 TSSCount[SX_MAX_TSS];

	u32 SavedSSS[SX_MAX_SSS][64];
	u32 SSSFlag[SX_MAX_SSS];    //SSS���X�V���ꂽ���ǂ���
	D3DSAMPLERSTATETYPE SSSList[SX_MAX_SSS][64];
	u32 SSSCount[SX_MAX_SSS];
public:
	TSXStateRestorer(TDGCarad* ADG);
	HRESULT SetRS(D3DRENDERSTATETYPE RSType, u32 Value);
	HRESULT SetTSS(u32 Stage, D3DTEXTURESTAGESTATETYPE TSSType, u32 Value);
	HRESULT SetSSS(u32 Stage, D3DSAMPLERSTATETYPE SSSType, u32 Value);
	void Memory();   //���̃X�e�[�g���o���Ă���
	void Restore();  //�Ō�Ɋo�����X�e�[�g�ɖ߂�
};


//SXVertex�쐬
TSXVertex SXVertex(f32 x, f32 y, f32 z, f32 nx, f32 ny, f32 nz, u32 dif, u32 spec, f32 tu, f32 tv);
TSXVertex2T SXVertex2T(f32 x, f32 y, f32 z, f32 nx, f32 ny, f32 nz, u32 dif, f32 tu0, f32 tv0, f32 tu1, f32 tv1);

//SXVertexBB�쐬
TSXVertexBB SXVertexBB(f32 dx, f32 dy, u32 col, f32 tu, f32 tv, u32 spec=0x00000000);

//SXVertexSP�쐬
TSXVertexSP SXVertexSP(f32 dx, f32 dy, u32 col, f32 tu, f32 tv, u32 spec=0x00000000);

//�}�e���A���ݒ�̕֋X��}��֐�
D3DMATERIAL9 MakeMaterial(f32 difR, f32 difG, f32 difB, f32 specR, f32 specG, f32 specB, f32 ambR, f32 ambG, f32 ambB, f32 emsR, f32 emsG, f32 emsB, f32 pow);
D3DMATERIAL9 MakeMaterial(u32 dif, u32 spec, u32 amb, u32 ems, f32 pow); //���w��Au32��

//�u�����h���[�h���̑��
TSXBlendInfo BlendInfo(bool _sort, bool _alphaTest, D3DCMPFUNC _alphaFunc, bool _alphaBlend, D3DBLEND _srcBlend, D3DBLEND _destBlend, D3DBLENDOP _blendOp);

glm::mat3x3 Matrix44To33(const glm::mat4& m)
{
	glm::mat3x3 result;

	result[0][0] = m[0][0];   result[1][0] = m[1][0];   result[2][0] = m[2][0];
	result[0][1] = m[0][1];   result[1][1] = m[1][1];   result[2][1] = m[2][1];
	result[0][2] = m[0][2];   result[1][2] = m[1][2];   result[2][2] = m[2][2];

	return result;
}

//��]�������������o��
glm::mat4 ExtractRotation(const glm::mat4& m)
{
	glm::mat4 result;

	result[0][0] = m[0][0];   result[1][0] = m[1][0];   result[2][0] = m[2][0];   result[3][0] = 0;
	result[0][1] = m[0][1];   result[1][1] = m[1][1];   result[2][1] = m[2][1];   result[3][1] = 0;
	result[0][2] = m[0][2];   result[1][2] = m[1][2];   result[2][2] = m[2][2];   result[3][2] = 0;
	result[0][3] = 0;		  result[1][3] = 0;		    result[2][3] = 0;		  result[3][3] = 1.f;

	return result;
}

//�t�s��(�ϊ��s��p�́A�蔲���v�Z)
glm::mat4 InvMatrix(const glm::mat4& m)
{
	glm::mat4 result;
	result[0][0] = m[0][0];   result[1][0] = m[0][1];   result[2][0] = m[0][2];   result[3][0] = 0;
	result[0][1] = m[1][0];   result[1][1] = m[1][1];   result[2][1] = m[1][2];   result[3][1] = 0;
	result[0][2] = m[2][0];   result[1][2] = m[2][1];   result[2][2] = m[2][2];   result[3][2] = 0;

	result[0][3] =  - (m[0][0]*m[0][3] + m[1][0]*m[1][3] + m[2][0]*m[2][3]);
	result[1][3] =  - (m[0][1]*m[0][3] + m[1][1]*m[1][3] + m[2][1]*m[2][3]);
	result[2][3] =  - (m[0][2]*m[0][4] + m[1][2]*m[1][3] + m[2][2]*m[2][3]);
	result[3][3] =  1.f;
	return result;
}

//�t�s��(��]�������i�[���Ă���ƌ��߃E�`)
glm::mat4 InvMatrixRot(const glm::mat4& m)
{
	glm::mat4 result;
	result[0][0] = m[0][0];   result[1][0] = m[0][1];   result[2][0] = m[0][2];   result[3][0] = 0;
	result[0][1] = m[1][0];   result[1][1] = m[1][1];   result[2][1] = m[1][2];   result[3][1] = 0;
	result[0][2] = m[2][0];   result[1][2] = m[2][1];   result[2][2] = m[2][2];   result[3][2] = 0;

	result[0][3] = 0;
	result[1][3] = 0;
	result[2][3] = 0;
	result[3][3] = 1.f;
	return result;
}

glm::mat4 RotXMatrix(const s32 t)
{
	s32 t2 = t & 4095;
	glm::mat4 result;

	result[1][1] = cost[t2];   result[2][1] = sint[t2];
	result[1][2] = -sint[t2];  result[2][2] = cost[t2];

	return result;
}

glm::mat4 RotYMatrix(const s32 t)
{
	s32 t2 = t & 4095;
	glm::mat4 result;

	result[0][0] = cost[t2];   result[2][0] = -sint[t2];
	result[0][2] = sint[t2];  result[2][2] = cost[t2];

	return result;
}

glm::mat4 RotZMatrix(const s32 t)
{
	s32 t2 = t & 4095;
	glm::mat4 result;

	result[0][0] = cost[t2];   result[1][0] = sint[t2];
	result[0][1] = -sint[t2];  result[1][1] = cost[t2];

	return result;
}

glm::mat4 ScaleMatrix(f32 _x, f32 _y, f32 _z)
{
	glm::mat4 result;
	result[0][0] = _x;
	result[1][1] = _y;
	result[2][2] = _z;

	return result;
}


D3DMATRIX mat4toD3DMATRIX( const glm::mat4& mat ){
	D3DMATRIX d3dmat;
	glm::mat4 trans = glm::transpose(mat);
	memcpy( &d3dmat, &trans, sizeof(f32)*16);

	return d3dmat;
}

glm::mat4 D3DMATRIXtomat4( const D3DMATRIX& mat ){
	glm::mat4 dest;
	memcpy( &dest, &mat, sizeof(f32)*16);
	glm::mat4 trans = glm::transpose(dest);

	return trans;
}

glm::vec4 Homoginize( const glm::vec3& v )
{
	return glm::vec4( v.x, v.y, v.z, 1.f );
}

//���K�����Z(x,y,z,w) �� (x/w, y/w, z/w, 1/w)
glm::vec4 ViewFrustumToScreen(const glm::vec4& a)
{
	return a / a.w;
}

s32 RoundOff( f32 X )
{
	if ( X > 0.0f ) {
		return (s32)floor(X + 0.5f);
	} else {
		return (s32)(-1.0f * floor(fabs(X) + 0.5f));
	}
}

glm::vec3 TransformVec3(const glm::vec3& a, const glm::mat4& m)
{
	glm::vec3 result;

	result.x = a.x * m[0][0] + a.y * m[0][1] + a.z * m[0][2] + m[0][3];
	result.y = a.x * m[1][0] + a.y * m[1][1] + a.z * m[1][2] + m[1][3];
	result.z = a.x * m[2][0] + a.y * m[2][1] + a.z * m[2][2] + m[2][3];

	return result;
}