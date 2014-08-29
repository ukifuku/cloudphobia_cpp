#pragma once

#include "types.h"
#include <string>
#include <sstream>
#include <d3d9.h>
#include <d3d9types.h>
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "DGCarad9.h"

/* 新SX形式について

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


DXVersionは将来のDirectXアップグレードへの布石
*/


/* SX形式について(古い)

name   | ofs          | length         | comment
----------------------------------------------------------------------------
Signature |            0 |             16 | 'Simplified_X00  '
nVextices |           16 |              2 | number of vertices
nIndices  |           18 |              2 | number of indices for( s32 vertices

(padding)

Vertices  |          256 | 40 * nVertices | vertices(TSXVertex)
Indices   | 256+Vertices |  2 * nIndices  | indices for( s32 vertices


　SX01形式は、未公開バージョン、サポートしない
 */

using namespace std;

//SXファイル識別子
const char SX00_SIGNATURE[] = "Simplified_X00  ";  //古いSX
const char SX02_SIGNATURE[] = "Simplified_X02  ";  //新SX、フレキシブル頂点フォーマット(DX8)対応

const char DXVER_FOR_FVF[] = "0800";                //FVF内容識別用のDirectXのバージョン


//想定するテクスチャステージ・サンプラステージの上限
#define	SX_MAX_TSS  8
#define	SX_MAX_SSS  8


//TSXVertex用、FVFフラグ
#define	FVF_SXVERTEX  (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1)

//TSXVertex、ワンスキン用、FVFフラグ(スペキュラ無し)
#define	FVF_SXV_ONESKIN  (D3DFVF_XYZB1 | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//TSXVertex、テクスチャ２枚用(スペキュラ無し)
#define	FVF_SXVERTEX2T  (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX2)

#define D3DFVF_TLVERTEX	(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//定義済みブレンドモード
enum TSXBlendMode {
	sxbDefault = 0,     //普通、透けない
	sxbAdd,         //加算合成
	sxbAlpha,       //半透明
	sxbMulNeg,      //エセ減算合成(反転してからMul)
	sxbMul,         //乗算合成
	sxbColorKey    //αテストだけ、透けない
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

// 初期化
void SXInit();

//SX形式のための頂点データ
struct TSXVertex 
{
	union{
		struct{
			f32 x,y,z;     //頂点
			f32 nx,ny,nz;  //法線
			u32 diffuse;   //ディフューズ
			u32 specular;  //スペキュラ
			f32 tu,tv;       //テクスチャ座標
		};
		struct{
			glm::vec4 pos;
			glm::vec4 normal;
		};
	};
};

//マルチテクスチャ対応SXVertex
struct TSXVertex2T
{
	union{
		struct{
			f32 x,y,z;     //頂点
			f32 nx,ny,nz;  //法線
			u32 diffuse;   //ディフューズ
			f32 tu0,tv0;       //テクスチャ座標
			f32 tu1,tv1;       //テクスチャ座標(2)
		};
		struct{
			glm::vec4 pos;
			glm::vec4 normal;
		};
	};
};

//ワンスキン用頂点データ
struct TSXVertexOneSkin 
{
	union{
		struct{
			f32 x,y,z;     //頂点
			f32 b;         //ベータ
			f32 nx,ny,nz;  //法線
			u32 diffuse;   //ディフューズ
			f32 tu,tv;       //テクスチャ座標
		};
		struct{
			glm::vec4 pos;
			f32 beta;
			glm::vec4 normal;
		};
	};
};


//ビルボードのための頂点情報
struct TSXVertexBB 
{
	//union{
	//struct{
	f32 dx,dy;     //中心からの相対座標
	u32 color;   //色
	u32 specular;
	f32 tu,tv;     //テクスチャ座標
	//	};
	//	glm::vec2 size;
	//};
};

//スプライトのための頂点情報
struct TSXVertexSP 
{
	f32 dx,dy;         //中心からの相対座標(スクリーン座標系)
	u32 color;       //色
	u32 specular;
	f32 tu,tv;         //テクスチャ座標
};


//ブレンドモード定義情報
struct TSXBlendInfo 
{
	bool Sort;               //Zソートする必要がある
	//以下、レンダリングステート設定用
	bool AlphaTestEnable;    //カラーキー処理用、αテスト、falseならAlphaFuncは無視
	D3DCMPFUNC AlphaFunc;          //αテスト用比較関数

	bool	AlphaBlendEnable;   //もし、このメンバがfalseなら、以下の設定はスキップ
	D3DBLEND SrcBlend;
	D3DBLEND DestBlend;
	D3DBLENDOP BlendOp;              //BlendOpが設定できるビデオカードのみ
};

struct D3DTLVERTEX //DirectX9.0ではサポートしてないので定義しなおし
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

//可視属性
enum TSXVisibility{
	sxvShow,        //見える
	sxvHide,        //隠れる。子フレームも含めて
	sxvHideMyself   //隠れる。子フレームは隠れない。
};

//メッシュ描画のソート時に何を基準にするか
enum TSXSortKey {
	sxsMeshCenter,  //メッシュの中心
	sxsMeshFar,     //メッシュの遠い所
	sxsMeshNear     //メッシュの近い所
};

//座標変換についてのポリシー
enum TSXFrameStyle { 
	sxfsDefault,      //ふつう
	sxfsMeshContainer //親フレームと常に同じ変換行列を取り、純粋にメッシュ格納庫としての役割を果たす
};


//イベントハンドラ
typedef void (*TSXRenderEvent)(TSXFrame&, TSXScene&, TDGCarad& );
typedef void (*TSXQueryZEvent)(TSXFrame*, TSXScene*, TDGCarad*, f32& ZValue );
typedef void (*TSXQueryVisibilityEvent)(TSXFrame*, TSXScene*, TDGCarad*, TSXVisibility& );

//クラス群

//BOX(AABB)
struct TBox3D
{
	glm::vec3 Min,Max;
};

//球
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

//線分…点A,Bを始点・終点とする線分
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


//円筒… Axisを軸にする、半径Radiusの円筒
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


//メッシュ用クラス
class TSXMesh
{
	friend class TSXScene;
protected:
	TDGCarad* FOwner;                //色々お世話になるDDDDオブジェクト

	TDGVertexBuffer*  FVBuf;          //VertexBuffer
	TDGIndexBuffer*  FIBuf;           //IndexBuffer
	string FVBufName,FIBufName;     //デバッグログに吐き出される、VertexBufferとIndexBufferの名前

	u32 FFaceCount;                //面の数、常にインデクス数/3

	TBox3D FBox;                    //AABB
	TSphere3D FSphere;              //外接球
	TCapsule3D FCapsule;            //外接カプセル

	u32 MaxPrims;                 //一度に描画できる最大のプリミティブ数
	u32 MaxIndex;                 //頂点インデクスの最大値
	bool IsSameVs(u8* pV, u32 v1, u32 v2);

	void GetFace(u16* dest, u16 faceIdx, vector<u16>& Indices)
	{
		*dest = Indices[faceIdx*3];
		dest++;
		*dest = Indices[faceIdx*3+1];
		dest++;
		*dest = Indices[faceIdx*3+2];
	}

	//その面はキャッシュヒットしてる？(最近nCacheEntryまでのインデクスと同じ？)
	//面のうちキャッシュヒットしてる頂点の数を返す
	s32 HitCount(u16 faceIdx, s32 nCacheEntry, s32 nOrdered, vector<u16>& Indices)
	{
		s32 result = 0;

		for( u32 i = 0; i <= 2; i++ ) {
			//面を構成するそれぞれの点について頂点キャッシュをさらう
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
			//面を構成するそれぞれの点について頂点キャッシュをさらう
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

	void Draw();                                                 //DrawIndexedPrimitiveで全部描画
	void DrawPartial( u32 start, u32 primCount );            //DrawIndexedPrimitiveで部分描画

	void LoadFromStream( istream& ss );
	void LoadFromFile( string fileName, string qdaName = "");
	void SaveToFile( string filename );
	void SaveToStream( ostream& ss );

	void SetupBuffers( u32 newVertexCount, u32 newIndexCount, u32 FVF,
		u32 VertexSize, D3DPOOL Pool );

	void Compact();            //同じ頂点をまとめる
	void Optimize();           //VCacheが良く利くように最適化する
	void UpdateBounds();       //メッシュのAABBと、外接球の半径を計算
	void Assign(TSXMesh& source); //メッシュの内容をコピー

	TDGVertexBuffer* VBuf() { return FVBuf; }
	TDGIndexBuffer*  IBuf() { return FIBuf; }
	u32 FaceCount() { return FFaceCount; }

	TBox3D& GetBox() { return FBox; }
	TSphere3D& GetSphere() { return FSphere; }
	TCapsule3D& GetCapsule() { return FCapsule; }

	string VBufName() { return FVBufName; }
	string IBufName() { return FIBufName; }
};


//メッシュのリスト
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


//フレーム
class TSXFrame 
{
friend class TSXScene;
friend class TSXRenderingQueue;
protected:
	glm::mat4 FMatrix;            //変換行列(自分の座標系→親座標系)
	glm::mat4 FMeshMatrix;        //メッシュ描画時の変換行列…メッシュ座標→自分座標
	TDGTexture* FTextures[SX_MAX_TSS]; //テクスチャ

	TSXFrameList* FAncestors;  //祖先フレーム達
	TSXFrameList* FChildren;   //子フレーム達
	TSXFrame* FParent;         //親フレーム(Ancestors[Ancestors.Count-1]と同)
	TSXVisibility FVisibility;   //可視属性

	bool FEnableMeshT;         //ローカル座標→メッシュ座標への変換を行う(MeshMatrix有効)
	TSXFrameStyle FStyle;         //座標変換についてのポリシー

	bool FRendered;            //最後のRenderメソッド実行時に、カリングされていない
	bool FValidRenderedMatrix; //RenderedMatrixは、最後のRenderメソッド実行時のシーンに一致している

	glm::mat4 RenderedMatrix;     //最後にレンダリングされたときのワールド行列(ビルボードの描画に使用)
	glm::mat4 RenderedMeshMatrix; //最後にレンダリングされたときのメッシュ行列
	bool BBAttached;           //ビルボードがくっついてる？
	bool FEnableBS;

	void RebuildAncestors( TSXFrame* me );
public:
	//中身
	TSXMesh* Mesh;                       //書き込むメッシュ
	D3DMATERIAL9 Material;               //マテリアル


	void SetTexture(TDGTexture* Value );
	void SetTextures( s32 idx, TDGTexture* Value );
	void SetParent(TSXFrame* Value);
	TDGTexture* GetTextures( s32 idx );


	bool isBBAttached() {return BBAttached;}
	//レンダリングステートの制御
	TSXBlendMode BlendMode;              //描画時のブレンドモード
	bool Lighting;                       //true:ライティングをする, false:頂点の色をそのまま使う
	bool Specular;                       //スペキュラハイライトをつける
	u32 TextureWrap;                     //ラップの方法
	D3DFILLMODE FillMode;                //塗り方(ポイント/ライン/ポリゴン)

	//イベントハンドラ
	TSXRenderEvent OnRender;                    //オーナードロウ
	TSXQueryZEvent OnQueryZ;                    //オーナードロウで描かれる物体のZ値、Nilの場合、メッシュのSortKey位置でソート
	TSXQueryVisibilityEvent OnQueryVisibility;  //カリング判定、Nilの場合、メッシュのバウンディング球でカリング

	//いろいろ
	TSphere3D Bounds;                       //フレームの外接球、可視判定用
	s32 Tag;                            //整数値、なんにでも使って
	s32 RenderAttr;                     //整数値、なんにでも使っての２
	void* UserData;                       //何かポインタ

	//ソート制御
	TSXSortKey SortKey;                     //何を基準にしてソートするか

	//コンストラクタ・デストラクタ
	TSXFrame(TSXFrame* parentFrame);
	~TSXFrame(); 

	//親子関係
	TSXFrameList* GetAncestors() { return FAncestors; }
	TSXFrameList* GetChildren() { return FChildren; }
	TSXFrame* GetParent() { return FParent; }

	TSXFrame* Ancestors(u32 _idx);
	TSXFrame* Children(u32 _idx);
	void ClearChildren();

	//可視属性
	TSXVisibility GetVisibility() { return FVisibility; }
	void	SetVisibility(TSXVisibility _vis) { FVisibility = _vis; }

	//フレームの外接球で、下位フレームまでカリングする
	bool GetEnableBounds() { return FEnableBS; }
	void	SetEnableBounds(bool _val) { FEnableBS = _val; }

	//可動部分ナシとして、現在の下位フレームまでの状態からバウンディング球を計算
	//onlyMyselfがfalseの時は、下位フレームのバウンディング情報も更新
	void UpdateBounds( bool onlyMyself );
	void ActivateBounds();     //下位フレームも含めて全部のフレームのBSをEnableに
	void DeactivateBounds();   //下位フレームも含めて全部のフレームのBSをDisableに

	//座標変換関係
	glm::mat4& GetMatrix() { return FMatrix; }
	void	SetMatrix(glm::mat4& _mat) { FMatrix = _mat; }
	glm::mat4& MatrixOnRender() { return RenderedMatrix; }               //OnRenderイベント内などで利用する、実際に描画されたときのフレーム→ワールド変換行列 
	glm::mat4& GetMeshMatrix() { return FMeshMatrix; }
	void SetMeshMatrix(glm::mat4& _mat) { FMeshMatrix = _mat;}    //メッシュ描画時の変換行列…メッシュ座標→自分座標
	glm::mat4& GetMeshMatrixOnRender() { return RenderedMeshMatrix; }
	bool GetEnableMeshTransform() { return FEnableMeshT; }
	void SetEnableMeshTransform( bool _val ) { FEnableMeshT = _val;} //メッシュ座標→自分座標の変換を行う
	TSXFrameStyle GetStyle() { return FStyle; }
	void SetStyle(TSXFrameStyle _style ){ FStyle = _style; }              //座標変換についてのポリシー
	bool Rendered() { return FRendered; }
	void SetRendered(bool val) { FRendered = val; }
	bool ValidMatrixOnRender() { return FValidRenderedMatrix; }
	void SetTexture( u32 idx, TDGTexture* _tex ) { FTextures[idx] = _tex; }
	TDGTexture* GetTexture();

	void SetTransform(TSXFrame* ref, const glm::mat4& mat);
	glm::mat4  GetTransform(TSXFrame* ref);

	glm::vec3 LocalToWorld(const glm::vec3& vec);           //フレーム内での座標を、ワールド座標に変換する
	glm::vec3 WorldToLocal(glm::vec3& vec);           //ワールド座標をフレーム内の座標に変換する
	glm::vec3 LocalToWorldRot(glm::vec3& rotvec);     //フレーム内で向きベクトルを、ワールド座標での向きベクトルに変換
	glm::vec3 WorldToLocalRot(glm::vec3& rotvec);     //ワールド座標内の向きベクトルをフレーム内の向きベクトルに変換する

	void CalcMatrixOnRender();                             //最後のRender時に即した変換行列を求める。カリングされたフレームの変換行列を知りたい時などに用いる

	//位置
	void SetTranslation(TSXFrame* ref, const glm::vec3& pos);
	glm::vec3 GetTranslation(TSXFrame* ref);

	//姿勢
	//…軸ベクトルで設定・取得
	void SetOrientation(TSXFrame* ref, const glm::vec3& vecZ, const glm::vec3& vecY);
	void GetOrientation(TSXFrame* ref, glm::vec3& vecZ, glm::vec3& vecY);

	//…マトリクスを与えて姿勢だけ設定
	void SetOrientationMatrix(TSXFrame* ref, const glm::mat4& mat);
	//…クォータニオンを与えて姿勢だけ設定
	void SetOrientationQuat(TSXFrame* ref, const glm::quat& quat);


	//お便利
	//…Z軸をtargetフレーム内の座標posを向ける。Y軸はworldフレーム内のY軸に向け。Y軸の傾きをbank度とする
	void LookAt(TSXFrame* target, const glm::vec3& pos, TSXFrame* world, const s32 bank);
	glm::mat4 ViewMatrix();  //このフレームをカメラにした場合のビュー行列を生成

	void SetBlendModeRcv(TSXBlendMode Value); //再帰的にブレンドモードをセットする
	void SetLightingRcv( bool Value );
	void SetSpecularRcv(bool Value);
	void SetTextureWrapRcv(u32 Value);
	void SetFillModeRcv(D3DFILLMODE Value);
	void SetMaterialRcv(D3DMATERIAL9& Value);
	void SetTexturesRcv(s32 idx, TDGTexture* Value);

	void DumpFrameStats(); //フレームの状態をデバッグログに出力

	glm::mat4 GetWorldMatrix();
	glm::mat4 GetWorldMatrixRot();
};

//フレームのリスト
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




//シーン全体
class TSXScene : public TDGCommonResource
{
protected:
	TSXStateRestorer* FStateRestorer;  //レンダリングステートお任せクン
	TSXFrame* FCamera;
	TDGCarad* DG;
	u32 ResID;                     //DGCaradに管理されるときのリソースID
	string FDebugName;              //デバッグログに出力される名前

	glm::mat4 FProjectionMatrix;
	glm::mat4 FScreenMatrix;
	D3DVIEWPORT9 FVP;
	f32 FFovH,FFovV,FAspectRatio,FNearZ,FFarZ;    //射影行列のパラメータ


	bool FBlendOpEnable;             //この環境でBLENDOPの設定ができるか？
	bool FSortAlpha;                 //半透明体のソートを行う
	vector<TSXBlendInfo> FBlendInfos;  //ブレンド情報格納用

	void InspectDevCaps();          //DEVCAPSに関係する情報の更新
public:
	TSXRenderingQueue* TopQueue;    //常に一番上に描画される物体のためのキュー、半透明体のソート等はしない
	TSXRenderingQueue* Queue;       //ソートしない(一般に、不透明体用のキュー)
	TSXRenderingQueue* SortQueue;   //ソートする(一般に透明体用のキュー)

	//コンストラクタ・デストラクタ
	TSXScene(TDGCarad* ADG, string debugName = "");
	~TSXScene(); 

	//描画
	void Render(TSXFrame* rootFrame, bool flushBBSP = true);        //rootFrame以下を描画
	void Clear( u32 dwFlags, u32 color, f32 z, u32 stencil );  //バックバッファのクリア
	void FlushBBSP();  //ビルボード・スプライトのためのキューをフラッシュする

	//一般設定
	bool GetSortAlpha() { return FSortAlpha; }
	void SetSortAlpha(bool val) { FSortAlpha = val; }
	TSXBlendMode RegisterBlendMode(TSXBlendInfo& info);  //新しいブレンドモードを追加する

	//変換周りの設定
	void SetProjection( f32 fov, f32 aspect, f32 nearZ, f32 farZ, bool setHorizontalFov = true);    //投影変換の設定
	void SetViewPort(u32 left, u32 top, u32 right, u32 bottom);        //ビューポート

	//↑の設定によって決定されるプロパティ
	f32 GetFarZ() { return FFarZ; }                           //SetProjectionで決定される値、以下４つもそう。
	f32 GetNearZ() { return FNearZ; }
	f32 GetFovH() { return FFovH; }                             //Fovの単位はラジアン
	f32 GetFovV() { return FFovV; }                            //Fovの単位はラジアン
	f32 GetAspectRatio() { return FAspectRatio; }
	D3DVIEWPORT9& GetFVP() { return FVP; }
	glm::mat4& GetProjectionMatrix() { return FProjectionMatrix; }
	glm::mat4& GetScreenMatrix() { return FScreenMatrix; } //スクリーン変換行列

	//ビルボード・スプライト
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
	//↑二つのメソッドでは、座標の指定が面倒いので、座標指定の簡単な代わり、
	//回転とかが出来ないもの
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


	//可視判定ユーティリティ
	//refフレーム内にある、球は見えるか、但し、距離がdepth以上なら見えないものとする
	bool SphereVisibility(TSXFrame* ref, TSphere3D& sphere, f32 depth);
	//↑と同じ、ただし、ref→ワールド座標までのマトリクスが求まっているとする
	bool SphereVisibility(glm::mat4& worldM, TSphere3D& sphere, f32 depth);

	//レンダリングステート記憶装置
	TSXStateRestorer* GetStateRestorer() { return FStateRestorer; }

	//その他
	bool BlendOpEnable() { return FBlendOpEnable; } //ブレンドモードを追加する際の指針
	void QueingFrame( TSXFrame* frame, const glm::mat4& mat);


	TSXBlendInfo& GetBlendInfos(u32 idx) {return FBlendInfos[idx];}  //ブレンド情報格納用

	void SetCamera( TSXFrame* _cam ) { FCamera = _cam; }
	TSXFrame* GetCamera() { return FCamera; }

	TDGCarad* GetDG() { return DG; }

	void Recover();              //ビューポートなどの状態をリストア
	void Cleanup(){};  
	void Reset(){};                             
	void AfterReset(){};   
};


//ライト
class TSXLight 
{
protected:
	TDGCarad* DG;
	u32 FIndex;       //SetLightに渡す、インデクス
	bool FEnabled;
	bool FUpdate;    //パラメータの変更を、即反映
public:
	D3DLIGHT9 Params;

	TSXLight(TDGCarad* DDCompo, u32 index);
	~TSXLight();

	void BeginUpdate();   //Paramsの内容を、Direct3Dに伝えなくする
	void EndUpdate();     //Paramsの内容を、Direct3Dに伝えなくした状態を解除

	void SetEnabled(bool Value);
	bool GetEnabled() { return FEnabled; }

	//お便利ルーチン
	void SetupDiffuse (f32 _R, f32 _G, f32 _B);
	void SetupSpecular(f32 _R, f32 _G, f32 _B);
	void SetupAmbient(f32 _R, f32 _G, f32 _B);

	void SetupColors(f32 difR, f32 difG, f32 difB, f32 specR, f32 specG, f32 specB, f32 ambR, f32 ambG, f32 ambB);
	void SetupRanges(f32 range, f32 att0, f32 att1, f32 att2);

	void SetupDirectional(glm::vec3& dir);
	void SetupPoint(glm::vec3& pos);
	void SetupSpot(glm::vec3& pos, glm::vec3& dir, f32 theta, f32 phi, f32 falloff);

	void FitFrame(TSXFrame* target);  //フレームの位置・向きにセットする
};


//複数のライトを管理するオブジェクト
class TSXLightGroup : public TDGCommonResource
{
protected:
	vector<TSXLight*> FLights;
	TDGCarad* DG;
	u32 ResID;             //DGCaradに管理される時のリソースID
	string FDebugName;      //デバッグログに出力される名前
	u32 FCapacity;        //管理するライトの数
	TSXLight* GetLights(u32 idx);
	TSXLight* GetUnusedLight();
public:
	TSXLightGroup(TDGCarad* ADG, u32 capacity, string debugName = "");
	~TSXLightGroup();

	TSXLight* operator[](u32 i) { return FLights[i];}
	void Recover();      //解像度を変えた後などのリカバリ
	void EnableAll();    //全部点ける
	void DisableAll();   //全部消す
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

	void BeginUpdate();   //Paramsの内容を、Direct3Dに伝えなくする
	void EndUpdate();     //Paramsの内容を、Direct3Dに伝えなくした状態を解除

	void SetupDiffuse (f32 _R, f32 _G, f32 _B);
	void SetupSpecular(f32 _R, f32 _G, f32 _B);
	void SetupAmbient (f32 _R, f32 _G, f32 _B);
	void SetupEmissive (f32 _R, f32 _G, f32 _B);
	void SetupSpecularPower(f32 pow);

	void SetupColors( f32 difR, f32 difG, f32 difB, f32 specR, f32 specG, f32 specB, f32 ambR, f32 ambG, f32 ambB, f32 emsR, f32 emsG, f32 emsB, f32 pow);
};



//メッシュ描画キューに入れる情報
struct TSXMeshInfo 
{
	TSXFrame* frame;     //そのメッシュを描こうとしたフレームはどれ？
};

//ビルボード描画キューに入れる情報
struct TSXBillboardInfo 
{
	bool scaling;      //視点からの距離でスケーリングする？
	bool filter;       //フィルタリングする？
	D3DTEXTUREADDRESS addrU,addrV;  //テクスチャアドレッシングモード
	TSXBlendMode blend;
	TSXFrame* ref;         //基準フレーム
	glm::vec4 pos;        //基準フレーム内での位置(計算用にも使う)
	TDGTexture *tex;       //テクスチャ
	TSXVertexBB pts[4];  //頂点データ
};

//2Dスプライトキューに入れる情報
struct TSXSpriteInfo 
{
	bool filter;       //フィルタリングする？
	D3DTEXTUREADDRESS addrU,addrV; //テクスチャアドレッシングモード
	TSXBlendMode blend;
	TDGTexture *tex;
	glm::vec3 pos;                    //中心点(x,yは画面座標、zは視点座標系のz)
	TSXVertexSP pts[4];  //頂点データ
};

//レンダリングの順序を保つためのバッファ、透明体かそうでないかで順序を変える。
class TSXRenderingQueue 
{
private:
	TDGCarad *FOwner;   //色々お世話になるDGCaradオブジェクト
	TSXScene* Scene;

	vector<TSXMeshInfo> MeshQueue;
	vector<TSXBillboardInfo> BillboardQueue;
	vector<TSXSpriteInfo>	SpriteQueue;

	vector<s32> OrderTable;        //描画順序テーブル
	vector<f32> Zs;                 //Z座標ソート用

	s32 FMeshCapacity;              //メッシュ情報を蓄える容量
	s32 FBillboardCapacity;
	s32 FSpriteCapacity;

	s32 FMeshCount;                 //キューに入ってる数
	s32 FBillboardCount;
	s32 FSpriteCount;

	s32 FVisibleBillboardCount;     //画面に入ってるビルボードの数(ソート時に決定)
	s32 FVisibleSpriteCount;        //画面に入ってるスプライトの数(ソート時に決定)


	TDGTexture* prevTex;   //前回使ったテクスチャへの参照
	bool prevSpec;     //前回描画したものは、スペキュラを計算したか
	bool prevLit;      //前回描画したものは、ライティングの計算をしたか
	TSXFrame* prevFrame;   //直前に描画したビルボードの入ってたフレーム
	TSXBlendMode prevBlend;
	u32 prevWrap;       //テクスチャラップ
	D3DFILLMODE prevFill;      //フィルモード
	bool prevFilter;
	bool prevDrawnIsMesh;

	D3DTEXTUREADDRESS prevAddrU, prevAddrV;
	D3DTEXTUREADDRESS storedAddrU, storedAddrV;
	D3DTEXTUREFILTERTYPE storedFilterMag, storedFilterMin, storedFilterMip;

	glm::vec4 center;
	D3DTLVERTEX pts[4];
	glm::mat4 m;
	glm::mat4 projM, screenM, viewProjScreenM;

	f32 zF, zN; //遠近クリッピング面までの距離(スプライトのZバッファへの書き込み値算出用)
	f32 zMax, zMin;

	f32 ratio_dx, ratio_dy; //ビューポートの半分のサイズ、ビルボード描画時の補正値として


	void Sort(D3DVIEWPORT9& viewParam);   //レンダリング前ソート
	void SetBillBoardCapacity(s32 Value);
	void SetMeshCapacity(s32 Value);
	void SetSpriteCapacity(s32 Value);
	void QSort(s32 left, s32 right );
	f32 TransformedZ( glm::mat4& viewProjScreenM, glm::vec3& v, glm::mat4& worldM);
public:
	s32 AlphaRef;   //カラーキーの時のαREF。αがこの値以下なら抜かれる
	TSXRenderingQueue(TDGCarad* AOwner, TSXScene* _Scene);
	~TSXRenderingQueue(); 
	void PushMesh(TSXFrame* frame); //フレームにつっついたメッシュ //(blendMode:TSXBlendMode; mesh:TSXMesh; frame:TSXFrame; mat:D3DMatrix; tex:TDGTexture; mtrl:D3DMATERIAL8;  event:TSXRenderEvent; ZEvent:TSXQueryZEvent);
	void PushBillboard(bool scaling, bool filter, D3DTEXTUREADDRESS addrU, D3DTEXTUREADDRESS addrV, TSXBlendMode blendMode, TSXFrame* ref, glm::vec3& pos, TDGTexture* tex, TSXVertexBB pts[] ); //ビルボード
	void PushSprite(bool filter, D3DTEXTUREADDRESS addrU, D3DTEXTUREADDRESS addrV, TSXBlendMode blendMode, glm::vec3& pos, TDGTexture* tex, TSXVertexSP pts[] ); //スプライト

	void Render(bool needSort, glm::mat4& viewMat, D3DVIEWPORT9& viewParam, bool doFlush = true); //レンダリング
	void Flush();  //レンダリングせず、バッファの中を空に
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




//レンダリングステート記憶装置
class TSXStateRestorer 
{
private:
	TDGCarad* DG;

	u32 SavedRS[256];  //レンダリングステートバッファ
	u32 RSFlag;         //各レンダリングステートが更新されたかのフラグ
	D3DRENDERSTATETYPE RSList[256]; //更新されたレンダリングステートリスト
	s32 RSCount;     //更新されたレンダリングステートの種類のカウント

	u32 SavedTSS[SX_MAX_TSS][64];  //TSSバッファ
	u32 TSSFlag[SX_MAX_TSS];    //TSSが更新されたかどうか
	D3DTEXTURESTAGESTATETYPE TSSList[SX_MAX_TSS][64];
	s32 TSSCount[SX_MAX_TSS];

	u32 SavedSSS[SX_MAX_SSS][64];
	u32 SSSFlag[SX_MAX_SSS];    //SSSが更新されたかどうか
	D3DSAMPLERSTATETYPE SSSList[SX_MAX_SSS][64];
	u32 SSSCount[SX_MAX_SSS];
public:
	TSXStateRestorer(TDGCarad* ADG);
	HRESULT SetRS(D3DRENDERSTATETYPE RSType, u32 Value);
	HRESULT SetTSS(u32 Stage, D3DTEXTURESTAGESTATETYPE TSSType, u32 Value);
	HRESULT SetSSS(u32 Stage, D3DSAMPLERSTATETYPE SSSType, u32 Value);
	void Memory();   //今のステートを覚えておく
	void Restore();  //最後に覚えたステートに戻す
};


//SXVertex作成
TSXVertex SXVertex(f32 x, f32 y, f32 z, f32 nx, f32 ny, f32 nz, u32 dif, u32 spec, f32 tu, f32 tv);
TSXVertex2T SXVertex2T(f32 x, f32 y, f32 z, f32 nx, f32 ny, f32 nz, u32 dif, f32 tu0, f32 tv0, f32 tu1, f32 tv1);

//SXVertexBB作成
TSXVertexBB SXVertexBB(f32 dx, f32 dy, u32 col, f32 tu, f32 tv, u32 spec=0x00000000);

//SXVertexSP作成
TSXVertexSP SXVertexSP(f32 dx, f32 dy, u32 col, f32 tu, f32 tv, u32 spec=0x00000000);

//マテリアル設定の便宜を図る関数
D3DMATERIAL9 MakeMaterial(f32 difR, f32 difG, f32 difB, f32 specR, f32 specG, f32 specB, f32 ambR, f32 ambG, f32 ambB, f32 emsR, f32 emsG, f32 emsB, f32 pow);
D3DMATERIAL9 MakeMaterial(u32 dif, u32 spec, u32 amb, u32 ems, f32 pow); //α指定可、u32版

//ブレンドモード情報の代入
TSXBlendInfo BlendInfo(bool _sort, bool _alphaTest, D3DCMPFUNC _alphaFunc, bool _alphaBlend, D3DBLEND _srcBlend, D3DBLEND _destBlend, D3DBLENDOP _blendOp);

glm::mat3x3 Matrix44To33(const glm::mat4& m)
{
	glm::mat3x3 result;

	result[0][0] = m[0][0];   result[1][0] = m[1][0];   result[2][0] = m[2][0];
	result[0][1] = m[0][1];   result[1][1] = m[1][1];   result[2][1] = m[2][1];
	result[0][2] = m[0][2];   result[1][2] = m[1][2];   result[2][2] = m[2][2];

	return result;
}

//回転部分だけ抜き出し
glm::mat4 ExtractRotation(const glm::mat4& m)
{
	glm::mat4 result;

	result[0][0] = m[0][0];   result[1][0] = m[1][0];   result[2][0] = m[2][0];   result[3][0] = 0;
	result[0][1] = m[0][1];   result[1][1] = m[1][1];   result[2][1] = m[2][1];   result[3][1] = 0;
	result[0][2] = m[0][2];   result[1][2] = m[1][2];   result[2][2] = m[2][2];   result[3][2] = 0;
	result[0][3] = 0;		  result[1][3] = 0;		    result[2][3] = 0;		  result[3][3] = 1.f;

	return result;
}

//逆行列(変換行列用の、手抜き計算)
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

//逆行列(回転だけを格納していると決めウチ)
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

//正規化除算(x,y,z,w) → (x/w, y/w, z/w, 1/w)
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