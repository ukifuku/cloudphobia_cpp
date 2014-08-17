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

	//デバイスの能力ゲット
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

	//primCountずつ分割して描画
	u32 ofs = 0;
	while( ofs < (FIBuf->GetCount()-2) ) {  //なんで-2してるかというと、１プリミティブは最低３頂点だから

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


	//primCountずつ分割して描画

	//開始頂点インデクスの位置と、終了頂点インデクスの位置を確定
	u32 ofs = start;
	u32 endOfs = start + primCount*3;
	if( endOfs > FIBuf->GetCount() )
		endOfs = FIBuf->GetCount();

	//終了頂点インデクスまで、MaxPrimsずつちまちま描く
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

	//シグネチャの確認
	sign[16] = 0;
	ss.read(sign, 16);

	if( strcmp(sign, SX00_SIGNATURE) == 0) {
		//SX形式
		FVF = FVF_SXVERTEX;
		VSize = sizeof(TSXVertex);
		//頂点数・頂点インデクス数の設定
		ss.read(reinterpret_cast<char*>(&nVertices), sizeof(u16));
		ss.read(reinterpret_cast<char*>(&nIndices),  sizeof(u16));
	} else if( strcmp(sign, SX02_SIGNATURE) == 0 ) {
		//頂点数・頂点インデクス数の設定
		ss.read(reinterpret_cast<char*>(&nVertices), sizeof(u16));
		ss.read(reinterpret_cast<char*>(&nIndices),  sizeof(u16));
		//SXFlexible形式
		ss.read(reinterpret_cast<char*>(&FVF), sizeof(u32));
		ss.read(reinterpret_cast<char*>(&VSize), sizeof(u32));
	} else {
		//DGCarad_PutDebugMessage('SXLib: 不正なSXファイルです');
		return;
	}

	SetupBuffers(nVertices, nIndices, FVF, VSize, D3DPOOL_MANAGED);

	//ヘッダのスキップ
	ss.seekg(StartPos + 256);

	try{
		//頂点のロード
		pLock = (u8*)FVBuf->Lock( 0, 0, 0);
		ss.read(reinterpret_cast<char*>(pLock), VSize * nVertices);
		FVBuf->Unlock();

		//頂点インデクスのロード
		pLock = (u8*)FIBuf->Lock( 0, 0, 0);
		ss.read(reinterpret_cast<char*>(pLock), sizeof(u16) * nIndices);
		FIBuf->Unlock();
	}
	catch(std::istream::failure e){
		//DGCarad_PutDebugMessage('SXLib: ストリームからの読み込み中にエラーが発生しました');
		return;
	}

	//バウンディングボリューム情報更新
	UpdateBounds();

}

void TSXMesh::LoadFromFile( string fileName, string qdaName /*= ""*/ )
{
	ifstream fs;
	if( qdaName.empty()) {
		//ファイルから読み込み
		try{
			fs.open( fileName );
		}
		catch(std::istream::failure e){
			//DGCarad_PutDebugMessage('SXLib: ' + filename + ' が、開けません');
			return;
		}

		LoadFromStream(fs);
		fs.close();
	} else {
		//アーカイブから読み込み
		stringstream ms;
		try{
			ExtractFromQDAFile( ms, qdaName, fileName);
		}
		catch(std::istream::failure e){
			//DGCarad_PutDebugMessage('SXLib: ' + QDAname + ' 内の、' + filename + 'が抽出できません');
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
	//{ヘッダ}

	//シグネチャ
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

	//{本体}

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

	//まず、既存のVertexBufferの解放
	SAFE_DELETE(FVBuf);
	SAFE_DELETE(FIBuf);

	if( newVertexCount > MaxIndex ) {
		//DGCarad_PutDebugMessage(Format('SXLib : このハードウェアでは、最大 %d までしか頂点インデクスを振る事ができません',[maxIndex]));
	}

	//VertexBufferの生成
	FVBuf = new TDGVertexBuffer(FOwner, newVertexCount, VertexSize, 0, FVF, Pool, FVBufName);

	//IndexBufferの作成
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


	vector<u32> tbl; //頂点番号変換テーブル
	tbl.resize(FVBuf->GetVertexCount());

	u8* pV = (u8*)FVBuf->Lock(0,0,0);

	//まず、コンパクション後の頂点が、コンパクション前のどの頂点にあたるのかを調べる
	//コンパクション後のi番の頂点は、コンパクション前のtbl[i]番の頂点に相当する
	u32 nCompacted = 0;	//今までまとめられた点の数
	tbl[0] = 0;
	for( u32 i = 1; i < FVBuf->GetVertexCount(); ++i ) {
		bool compacted = false;	//着目してた頂点は、まとめの対象になったか
		for( u32 j = 0; j < i; ++j ) {
			//以前に探索した点と同じ座標、同じ色なら、まとめる
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


	//{コンパクション}

	//パッファ作成
	u8* copyBuf = (u8*)malloc(newVertexCount * FVBuf->GetVertexSize());

	//上書きしていく
	for( u32 i = 0; i < FVBuf->GetVertexCount(); ++i ) {
		//Vertices[tbl[i]] = Vertices[i];
		u8* pV1 = pV;  pV1 += tbl[i] * FVBuf->GetVertexSize();
		u8* pV2 = pV;  pV2 += i * FVBuf->GetVertexSize();

		memmove(pV1, pV2, FVBuf->GetVertexSize());
	}

	//バッファに退避
	memmove(copyBuf, pV, newVertexCount * FVBuf->GetVertexSize());


	//VBの再構築
	FVBuf->Unlock();
	FVBuf->SetVertexCount(newVertexCount);

	//コピー
	pV = (u8*)FVBuf->Lock(0,0,0);
	memmove(pV, copyBuf, newVertexCount * FVBuf->GetVertexSize());
	FVBuf->Unlock();


	//インデクスバッファの中身を書き換え
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

	//GeForce1/2にフィットするように、キャッシュのエントリは16としておく
	s32 nCacheEntry = 16;

	//インデクスバッファをコピー
	vector<u16> Indices;
	Indices.resize( FIBuf->GetCount());
	FIBuf->Lock(0,0,0);
	for( u32 i = 0; i < FIBuf->GetCount(); i++ )
		Indices[i] = FIBuf->GetIndices(i);
	FIBuf->Unlock();

	//最初の一個の面はそのまま
	s32 nOrdered = 3;		 //並び替えの終わった頂点インデクスの数

	//面ごとにインデクスをソートしてく
	for( u32 i = 1 ; i < FFaceCount; i++ ) {
		s32 goodFace = -1;	//探索中に見つかった、１ヒット、2ヒット面のインデクス
		s32 betterFace = -1;

		//できれば３つともキャッシュヒットしてる頂点を持ってくる
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

		//無かったら、２つか１つキャッシュヒットしてる面を持ってくる
		if( ! swapped ) {
			if( betterFace != -1 )
				SwapFace(i, betterFace, Indices);
			else if( goodFace != -1 )
				SwapFace(i, goodFace, Indices);
		}

		nOrdered += 3;
	}


	//コピー戻し
	FIBuf->Lock(0,0,0);
	for( u32 i = 0 ; i < FIBuf->GetCount(); ++i ){
		FIBuf->SetIndices(i, Indices[i]);
	}
	FIBuf->Unlock();

	Indices.clear();
}

void TSXMesh::UpdateBounds()
{

	//読み取り専用でロック
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

	//結果の格納
	FBox.Min = min;
	FBox.Max = max;

	glm::vec3 center = (FBox.Min + FBox.Max) * 0.5f;
	glm::vec3 dif = FBox.Max - FBox.Min;

	//まだいいかげんながら、カプセルの計算

	if( (dif.x > dif.y) && (dif.x > dif.z) ) {
		//軸をX方向に取る
		FCapsule.R = glm::sqrt( (dif.y*dif.y) + (dif.z*dif.z) ) * 0.5f;
		FCapsule.A = glm::vec3(FBox.Min.x + FCapsule.R, center.y, center.z);
		FCapsule.B = glm::vec3(FBox.Max.x - FCapsule.R, center.y, center.z);
	} else if( (dif.y > dif.x) && (dif.y > dif.z) ) {
		//軸をY方向に取る
		FCapsule.A = glm::vec3(center.x, FBox.Min.y + FCapsule.R, center.z);
		FCapsule.B = glm::vec3(center.x, FBox.Max.y - FCapsule.R, center.z);
		FCapsule.R = glm::sqrt( (dif.x*dif.x) + (dif.z*dif.z) ) * 0.5f;
	} else {
		//軸をZ方向に取る
		FCapsule.A = glm::vec3(center.x, center.y, FBox.Min.z + FCapsule.R);
		FCapsule.B = glm::vec3(center.x, center.y, FBox.Max.z - FCapsule.R);
		FCapsule.R = glm::sqrt( (dif.x*dif.x) + (dif.y*dif.y) ) * 0.5f;
	}

	//半径をちょっと補正(インチキ版)
	//FCapsule.R = FCapsule.R * 0.5;


	FSphere.Center = center;
	FSphere.Radius = glm::length(dif) * 0.5f;
}

void TSXMesh::Assign( TSXMesh& source )
{
	SetupBuffers(source.FVBuf->GetVertexCount(), source.FIBuf->GetCount(), source.FVBuf->GetFVF(),
		source.FVBuf->GetVertexSize(), source.FVBuf->GetPool());

	//中身のコピー
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
	//まず中身を消す
	Clear();


	//項目数
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
		//ファイルから読み込み
		ifstream src(filename);
		LoadFromStream(src);
		src.close();
	} else {
		//アーカイブ読み込み
		stringstream ms;
		ExtractFromQDAFile(ms, qdaName, filename);
		LoadFromStream(ms);
	}

}

void TSXMeshList::SaveToStream( ostream& Dest )
{
	//項目数
	s32 cnt = m_meshes.size();
	Dest.write(reinterpret_cast<char*>(&cnt), sizeof(s32));

	//メッシュの保存、もし、メッシュがNULLなら、バイト値 0 を1バイト入れる
	//そうじゃないなら、バイト値1を入れる
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

	//祖先のマトリクスを 親→祖父… の順に乗じていく。
	//Ancestors[0] はNULLであり、途中にNULLは入らないという前提の下に計算
	for( u32 i = FAncestors->GetSize()-1 ; i >= 1; i-- ) {
		//MeshContainerの場合、マトリクスは常にIdentity
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

	//祖先のマトリクスを 親→祖父… の順に乗じていく。
	//Ancestors[0] はNULLであり、途中にNULLは入らないという前提の下に計算
	for( u32 i = FAncestors->GetSize()-1; i >=1; i-- ) {
		//MeshContainerの場合、マトリクスは常にIdentity
		if( Ancestors(i)->GetStyle() != sxfsMeshContainer ){
			glm::mat4 ancmat = ExtractRotation(Ancestors(i)->FMatrix);
			mat = ancmat * mat;
		}
	}

	return mat;
}

//先祖リストを再帰的に修正する
void TSXFrame::RebuildAncestors( TSXFrame* me )											
{																
	for( u32 i = 0; i < me->GetChildren()->GetSize(); i++ ) {	
		/*ヲレの先祖を先祖と崇めなさい*/						
		me->Children(i)->GetAncestors()->Assign(me->GetAncestors());
		/*ヲレを親と崇めなさい*/									
		me->Children(i)->GetAncestors()->Add(me);		
		/*子々孫々に渡ってそうしたまへ	*/							
		RebuildAncestors(me->Children(i));			
	}															
}

void TSXFrame::SetParent( TSXFrame* Value )
{
	//元の親のリストから外す
	if( FParent != NULL )
		FParent->GetChildren()->Delete(this);


	if( Value != NULL ) {
		//新しい親のリストに加える
		Value->GetChildren()->Add(this);
		//先祖リストを変える
		FAncestors->Assign(Value->FAncestors);
		FAncestors->Add(Value);
	} else {
		FAncestors->Clear();
	}

	//自分に子供がいるなら、それらの先祖リストも変える
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
	//必須オブジェクトの生成

	//親子関係の初期化
	FAncestors = new TSXFrameList();
	if( parentFrame != NULL ) {
		FAncestors->Assign(parentFrame->FAncestors);   //先祖リストのコピー
		FAncestors->Add(parentFrame);                //先祖リストの末尾に、親を入れる

		parentFrame->GetChildren()->Add(this);                 //親の子リストに自分をつける
	}

	FChildren = new TSXFrameList();
	FParent = parentFrame;

	//FBinds = [];

	FStyle = sxfsDefault;
	FEnableMeshT = true;

	SortKey = sxsMeshCenter;


	//変数とかの初期化
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
		delete (Children(0));   //これで、再帰的に子フレームがFreeされる
	}

	//リストオブジェクトの解放
	SAFE_DELETE(FChildren);
	SAFE_DELETE(FAncestors);

	//親のリストから自分をはずす
	if( FParent != NULL )
		FParent->GetChildren()->Delete(this);

}

TSphere3D MergeBS(TSphere3D& a, TSphere3D& b)
{
	glm::vec3 dir;
	TSphere3D result;
	f32 d = glm::distance(a.Center, b.Center);

	if( (d + b.R <= a.R) || (b.R <= 0) ) {
		//aがbを完全に包んでいるか、bは存在しない
		result = a;
	} else if( (d + a.R <= b.R) || (a.R <= 0) ) {
		//bがaを完全に包んでいるか、aは存在しない
		result = b;
	} else {
		//↑2つの条件は、d=0 の場合を含んでいる
		//以下、どちらも含んだり含まれたりしてない場合

		//半径は簡単に求まる
		result.R = (d + a.R + b.R) / 2.0f;

		//a.Center から、b.Centerに対して、a.Rの分戻ってから半径分進んだところが新しい中心
		dir = glm::normalize(b.Center - a.Center);
		result.Center = a.Center + dir * (result.R - a.R);
	}

	//DGCarad_PutDebugMessage(Format('球の合成…C1(%f,%f,%f),R1=%f & C2(%f,%f,%f),R2=%f --> C(%f,%f,%f),R=%f',
	//	[a.X, a.Y, a.Z, a.R, b.x, b.Y, b.Z, b.R, result.X, result.Y, result.Z, result.R]));

	return result;
}

//自フレームのバウンディング球と子孫フレームのバウンディング球をマージ
TSphere3D CalcBS(TSXFrame* me, glm::mat4& worldM, bool onlyMyself)
{
	glm::mat4 meToWorldM,meshM;
	glm::mat3x3 m33;
	TSphere3D result;
	if( me->GetVisibility() == sxvHide ) {
		//子孫も含めてHideになってるなら、このフレーム以下は考慮しない
		result.R = -1.f;
		result.Center = glm::vec3(0,0,0);
	} else {
		if( me->GetStyle() == sxfsMeshContainer )
			meToWorldM = worldM;
		else
			meToWorldM = worldM * me->GetMatrix();

		if( (me->GetVisibility() == sxvHideMyself) ) {
			//自分だけ見えない
			result.R = -1.f;
			result.Center = glm::vec3(0,0,0);
			for( u32 i = 0; i < me->GetChildren()->GetSize(); i++ ) {
				result = MergeBS(CalcBS(me->Children(i), meToWorldM, onlyMyself), result);
			}
		} else {
			//見える
			if( me->GetEnableBounds() ) {
				//バウンディング球が指定されているなら、それを返す
				result.Center = TransformVec3( me->Bounds.Center, meToWorldM ); 
				result.R = me->Bounds.R;
			} else {
				//メッシュのバウンディング球と子孫フレームのバウンディング球をマージして返す
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

	//ゲットしてきたのは、ワールド座標表記なので、翻訳する
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
		//自分への変換行列
		FMatrix = FMatrix * mat;

	} else if( ref == FParent ) {
		//親への変換行列
		FMatrix = mat;
	} else {
		//自分→ref→ワールド→親
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
		//自分への変換を得る…単位行列に決まってる。
		return glm::mat4();

	} else if( ref == FParent ) {

		//親へのオリエンテーションを得る
		return FMatrix;

	} else {

		//自分→ワールド→ref
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
	//変換済みの親フレームの行列から、frameの行列を算出
#define CalcMatrix(frame)										\
	{															\
	RenderedMatrix = FParent->MatrixOnRender() * FMatrix;	\
	if( FEnableMeshT )										\
	RenderedMeshMatrix = RenderedMatrix * FMeshMatrix;	\
		else													\
		RenderedMeshMatrix = RenderedMatrix;				\
		/*検査完了サインを入れる*/								\
		FValidRenderedMatrix = true;							\
}

	//変換済みのフレームがないか、先祖をさかのぼって調べる
	s32 StartG = 1; //何世代前から変換を開始すればいいか？
	for( u32 i = FAncestors->GetSize(); i >= 0; i-- ){
		if( Ancestors(i)->ValidMatrixOnRender() ){
			StartG = i+1;
		}
	}

	//先祖→…→親までの変換行列を計算する
	for( u32 i = StartG; i < FAncestors->GetSize(); i++ ){
		CalcMatrix(FAncestors[i]);
	}

	//自分の変換行列の計算
	CalcMatrix(this);

}

void TSXFrame::SetTranslation( TSXFrame* ref, const glm::vec3& pos )
{
	if( ref == FParent ) {
		//親へのトランスレーションを作るだけなら、簡単
		FMatrix[0][3] = pos.x;   FMatrix[1][3] = pos.y;   FMatrix[2][3] = pos.z;

	} else {

		glm::mat4 origMat = FMatrix;	 //姿勢保持用
		glm::mat4 tmpMat = FMatrix;		 //refフレームとの相対変換行列

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

		//親への姿勢を作るだけなら、簡単
		FMatrix[0][0] = vecX.x;  FMatrix[1][0] = vecX.y;  FMatrix[2][0] = vecX.z;
		FMatrix[0][1] = vecY.x;  FMatrix[1][1] = vecY.y;  FMatrix[2][1] = vecY.z;
		FMatrix[0][2] = vecZ.x;  FMatrix[1][2] = vecZ.y;  FMatrix[2][2] = vecZ.z;

	} else {
		f32 vTrans[4];			 //平行移動成分の保存用

		//平行移動成分の保存
		vTrans[0] = FMatrix[0][3];
		vTrans[1] = FMatrix[1][3];
		vTrans[2] = FMatrix[2][3];

		//変換
		glm::mat4 tmpMat;             //refフレームとの相対変換行列
		tmpMat[0][0] = vecX.x;  tmpMat[1][0] = vecX.y;  tmpMat[2][0] = vecX.z;  tmpMat[3][0] = 0;
		tmpMat[0][1] = vecY.x;  tmpMat[1][1] = vecY.y;  tmpMat[2][1] = vecY.z;  tmpMat[3][1] = 0;
		tmpMat[0][2] = vecZ.x;  tmpMat[1][2] = vecZ.y;  tmpMat[2][2] = vecZ.z;  tmpMat[3][2] = 0;
		tmpMat[0][3] = vTrans[0]; tmpMat[1][3] = vTrans[1]; tmpMat[2][3] = vTrans[2]; tmpMat[3][3] = 1.f;

		SetTransform(ref, tmpMat);

		//平行移動成分の復元
		FMatrix[0][3] = vTrans[0]; FMatrix[1][3] = vTrans[1]; FMatrix[2][3] = vTrans[2];
	}
}

void TSXFrame::GetOrientation( TSXFrame* ref, glm::vec3& vecZ, glm::vec3& vecY )
{
	glm::mat4 tmpMat = GetTransform(ref);	//結果算出用バッファ

	vecY.x = tmpMat[0][1]; vecY.y = tmpMat[1][1]; vecY.z = tmpMat[2][1];
	vecZ.x = tmpMat[0][2]; vecZ.y = tmpMat[1][2]; vecZ.z = tmpMat[2][2];
}

void TSXFrame::SetOrientationMatrix( TSXFrame* ref, const glm::mat4& mat )
{
	//保存
	glm::vec3 pushedV;  //平行移動分保存用

	pushedV.x = FMatrix[0][3];  pushedV.y = FMatrix[1][3];  pushedV.z = FMatrix[2][3];

	if( ref == this ) {
		//自分への変換行列
		FMatrix = FMatrix * mat;

	} else if( ref == FParent ) {
		//親への変換行列
		FMatrix = mat;

	} else {

		//自分→ref→ワールド→親
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

	//復元
	FMatrix[0][3] = pushedV.x;  FMatrix[1][3] = pushedV.y;  FMatrix[2][3] = pushedV.z;

}

void TSXFrame::SetOrientationQuat( TSXFrame* ref, const glm::quat& quat )
{
	SetOrientationMatrix(ref, glm::mat4_cast(quat));
}

void TSXFrame::LookAt( TSXFrame* target, const glm::vec3& pos, TSXFrame* world, const s32 bank )
{
	//targetのposは、自分の座標系でどこなのか
	glm::vec3 vRel = WorldToLocal(target->LocalToWorld(pos));     //Targetとの相対座標

	//向き = Z軸ベクトル
	glm::vec3 vecZ = glm::normalize(vRel);

	//天井の向き = worldのY軸
	glm::vec3 vecCeil = WorldToLocal(world->LocalToWorld(glm::vec3(0,1.f,0))) - WorldToLocal(world->LocalToWorld(glm::vec3(0,0,0)));
	vecCeil = glm::normalize(vecCeil);

	//天井とZで外積を取って、Xの向きとする
	glm::vec3 vecX = glm::cross(vecCeil, vecZ);

	//天井とZが平行な場合、右耳の方向をworldのX軸にする
	f32 normX = glm::length(vecX);
	if( normX < 0.0001f )
		vecX = WorldToLocal(world->LocalToWorld(glm::vec3(1.f,0,0))) - WorldToLocal(world->LocalToWorld(glm::vec3(0,0,0)));

	vecX = glm::normalize(vecX);

	//XとZで外積を取って、Yの向きとする
	glm::vec3 vecY = glm::normalize(glm::cross(vecZ, vecX));



	//以上で姿勢の計算完了～セット
	SetOrientation(this, vecZ,vecY);

	//バンク
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

	//解像度とかドライバ変更にともなって、デバイスCapsが変わったかもしれんので、再調査
	InspectDevCaps();
}

TSXScene::TSXScene( TDGCarad* ADG, string debugName /*= ""*/ )
{
	Queue = new TSXRenderingQueue(ADG, this);
	SortQueue = new TSXRenderingQueue(ADG, this);
	TopQueue = new TSXRenderingQueue(ADG, this);

	FStateRestorer = new TSXStateRestorer(ADG);

	//DGCaradにリソースとして登録
	ResID = ADG->RegisterResouce(this, debugName);
	FDebugName = debugName;

	//ブレンドモードの登録

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

	//可視性のチェック
	if( frame->OnQueryVisibility ) {
		//イベントが設定してあるなら、それに任せる
		frame->OnQueryVisibility(*frame, *this, *DG, visibility);
	} else {
		//デフォルトでは、Visibilityプロパティに従う
		visibility = frame->FVisibility;

		//ただし、BoundingSphereを使うようなら、それに従う
		if( visibility != sxvHide ) {
			if( frame->FEnableBS ) {
				if( ! SphereVisibility(mat * frame->FMatrix, frame->Bounds, FFarZ) ) {
					visibility = sxvHide;
				}
			}
		}
	}

	//sxvHide(子孫も含めて隠す)なら何もしない
	if( visibility = sxvHide )
		return;

	//子フレーム無し、かつ、描画する必要なしなら、何もしない
	if( frame->GetChildren()->GetSize() == 0 ){
		if( ((frame->Mesh == NULL) && (! frame->OnRender)) || (visibility = sxvHideMyself) ){
			return;
		}
	}

	//変換行列の計算
	if( frame->GetStyle() == sxfsMeshContainer )
		frame->RenderedMatrix = mat;
	else
		frame->RenderedMatrix = mat * frame->FMatrix;

	if( frame->FEnableMeshT )
		frame->RenderedMeshMatrix = frame->RenderedMatrix * frame->FMeshMatrix;
	else
		frame->RenderedMeshMatrix = frame->RenderedMatrix;

	frame->FValidRenderedMatrix = true;

	//キューに放り込む

	if( visibility == sxvShow ) {

		if( (frame->Mesh != NULL) && (! frame->OnQueryVisibility) ) {
			//Meshが付いている場合

			//Meshのバウンディング球でカリングを試みる
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
			//メッシュが付いていなくても、オーナードロウの場合
			//メッシュが付いていても、QueryVisibilityで判定した場合
			//カリングしない(しようが無い)で、そのままGo!

			//ただし、オーナードロウなのにメッシュが付いてなくて、QueryZも無いなら前後関係の判定のしようがないので、何も描かない
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

	//子供の描画
	for( u32 i = 0; i < frame->GetChildren()->GetSize(); i++ ) {
		QueingFrame(frame->Children(i), frame->RenderedMatrix);
	}

}

void TSXScene::Render( TSXFrame* rootFrame, bool flushBBSP /*= true*/ )
{
	glm::mat4 viewMat;

	//ビュー行列の作成…(ワールド座標系でのカメラフレームの姿勢)の逆
	if( FCamera != NULL )
		viewMat  =  FCamera->ViewMatrix();
	else
		viewMat  =  glm::mat4();


	DG->Get3DDevice()->SetTransform(D3DTS_VIEW, &mat4toD3DMATRIX(viewMat));


	//再帰的にキューに入れる
	//ルートフレームがない場合は、スプライトだけ描く
	if( rootFrame != NULL ) {
		if( rootFrame->FParent == NULL )
			QueingFrame(rootFrame, glm::mat4());
		else
			QueingFrame(rootFrame, rootFrame->FParent->GetWorldMatrix());
	}

	//{ キューに入ってる物体を描く }
	Queue->Render(false, viewMat, FVP, flushBBSP);
	SortQueue->Render(FSortAlpha, viewMat, FVP, flushBBSP);

	//どういうわけかRadeon9600ではZENABLE切っても効かず、ZFUNCを設定しないとTopMostにならない
	DG->Get3DDevice()->SetRenderState(D3DRS_ZENABLE, u32(false));
	DG->Get3DDevice()->SetRenderState(D3DRS_ZWRITEENABLE, u32(false));
	DG->Get3DDevice()->SetRenderState(D3DRS_ZFUNC, u32(D3DCMP_ALWAYS));

	TopQueue->Render(false,viewMat, FVP, flushBBSP);  //最前面に表示されるスプライト用

	DG->Get3DDevice()->SetRenderState(D3DRS_ZENABLE, u32(true));
	DG->Get3DDevice()->SetRenderState(D3DRS_ZWRITEENABLE, u32(true));
	DG->Get3DDevice()->SetRenderState(D3DRS_ZFUNC, u32(D3DCMP_LESSEQUAL));


	//テクスチャに何も指定しない状態で、一連のレンダリングを終える
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
		//水平画角が設定された
		h = cos(fov*0.5f)/sin(fov*0.5f);
		v = h / aspect;
		FFovH = fov;
		FFovV = atan(1.0f/v)*2.f;
	} else {
		//垂直画角が設定された
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
	//フラグを立てる
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
	//フラグを立てる
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

	//球の半径が０以下なら、見えるはずがない
	if( sphere.R <=0 ) {
		return false;
	}

	//p = 視野座標系での球の中心
	glm::mat4 viewM = FCamera->ViewMatrix() * worldM;
	glm::vec3 p = TransformVec3(sphere.Center, viewM);

	//視界の後方や、depthより遠くにあるってのは論外
	if( (p.z < -sphere.R) || (p.z > depth+sphere.R) ) {
		//デバッグ
		return false;
	}

	//縦・横の画角を求める
	//DG->Get3DDevice()->GetTransform(D3DTS_PROJECTION, projM);
	f32 fH = FFovH * 0.5f;
	f32 fV = FFovV * 0.5f;

	//画角のSin,Cosを出しておく
	f32 SinFovH = sin(fH);   f32 CosFovH = cos(fH);
	f32 SinFovV = sin(fV);   f32 CosFovV = cos(fV);

	//XZ平面での判定…画角分だけ球の中心を回転させた時、コーンの中心から球の中心までが、球の半径の値以下になるかを見る
	//左手側の面
	f32 sideValue = p.x * CosFovH + p.z * SinFovH;
	if( sideValue < -sphere.R )
		return false;

	//右手側の面
	sideValue = -p.x * CosFovH + p.z * SinFovH;
	if( sideValue < -sphere.R )
		return false;

	//上側の面
	sideValue = -p.y * CosFovV + p.z * SinFovV;
	if( sideValue < -sphere.R )
		return false;

	//下側の面
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

	//デフォ値の設定
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

//vを変換して、Z値だけ返す
f32 TSXRenderingQueue::TransformedZ( glm::mat4& viewProjScreenM, glm::vec3& v, glm::mat4& worldM)
{
	//射影行列まで通すので変換した際に、カメラからのZ値がWに返る
	return ( (viewProjScreenM * worldM) * glm::vec4(v.x, v.y, v.z, 1.f)).w;
}

//くいっくそーと
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
	glm::mat4 viewM,projM,screenM,viewProjScreenM;  //ビュー行列・射影行列・スクリーン行列←３つの合成
	f32 z;
	f32 zC,zMax,zMin;         //メッシュの遠方・近方での比較用
	TSXFrame* prevBBFrame;	 //前回かかれたビルボードの収まっていたフレーム 

	// メッシュの分だけ、それ以外は各カテゴリのZ座標計算と一緒に作る
	for( s32 i = 0 ; i < FMeshCount; i++)
		OrderTable[i] = i;


	//{ まず、全部のメッシュ・BBのZ値を計算}
	D3DMATRIX projd3d, viewd3d;
	FOwner->Get3DDevice()->GetTransform(D3DTS_PROJECTION, &projd3d);
	FOwner->Get3DDevice()->GetTransform(D3DTS_VIEW, &viewd3d);
	projM = D3DMATRIXtomat4(projd3d);
	viewM = D3DMATRIXtomat4(viewd3d);

	screenM = Scene->GetScreenMatrix();

	viewProjScreenM = screenM * projM * viewM;

	//メッシュのソート用Z値を求める
	for( s32 i = 0 ; i < FMeshCount; i++ ) {

		if( MeshQueue[i].frame->OnQueryZ ) {
			//カスタムZ計算におまかせ
			MeshQueue[i].frame->OnQueryZ(*MeshQueue[i].frame, *Scene, *FOwner, Zs[i]);

		} else {

			//SortKeyを頼りに求める
			z = 0;
			switch( MeshQueue[i].frame->SortKey ){

				//メッシュの中心でソート
			case sxsMeshCenter:
				z = TransformedZ( viewProjScreenM, MeshQueue[i].frame->Mesh->GetSphere().Center, MeshQueue[i].frame->GetMeshMatrixOnRender());
				break;
				//メッシュの遠いほうでソート
			case sxsMeshFar: {
				mtrx = viewProjScreenM * MeshQueue[i].frame->GetMeshMatrixOnRender();
				zC = ( mtrx * Homoginize(MeshQueue[i].frame->Mesh->GetSphere().Center)).w;
				zMax = ( mtrx * Homoginize(MeshQueue[i].frame->Mesh->GetBox().Max)).w;
				zMin = ( mtrx * Homoginize(MeshQueue[i].frame->Mesh->GetBox().Min)).w;

				z = zC + abs(zMax-zMin) * 0.5f;
							 }
							 break;
							 //メッシュの近いところでソート
			case sxsMeshNear: {
				mtrx = viewProjScreenM * MeshQueue[i].frame->GetMeshMatrixOnRender();
				//中心の変換後の位置と、AABBのエッジのZ方向の長さを求める
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


	//ビルボードのZ値を求める
	prevBBFrame = NULL;
	FVisibleBillboardCount = 0;
	for( s32 i = 0 ; i < FBillboardCount; i++ ) {
		//mtrxに変換行列を代入
		if( prevBBFrame != BillboardQueue[i].ref ) {
			if( ! BillboardQueue[i].ref->ValidMatrixOnRender() )
				BillboardQueue[i].ref->CalcMatrixOnRender();

			mtrx = (viewProjScreenM * BillboardQueue[i].ref->MatrixOnRender());
			prevBBFrame = BillboardQueue[i].ref;
		}

		BillboardQueue[i].pos = mtrx * BillboardQueue[i].pos;

		//W<0なら、描画しない…ソートの対象が減って軽くなる
		if( BillboardQueue[i].pos.w > 0 ) {
			Zs[FVisibleBillboardCount + FMeshCount] = BillboardQueue[i].pos.w;
			OrderTable[FVisibleBillboardCount+FMeshCount] = i + FMeshCount;
			FVisibleBillboardCount++;
		}
	}

	//スプライトのZ値を求める
	FVisibleSpriteCount = 0;
	for( s32 i = 0 ; i < FSpriteCount; i++ ) {
		//if( SpriteQueue[i].pos.z > 0 ) {  //rhwではなく、カメラフレーム座標指定の場合
		if( SpriteQueue[i].pos.z >= 0 ) {
			Zs[FMeshCount+FVisibleBillboardCount+FVisibleSpriteCount] = SpriteQueue[i].pos.z;
			OrderTable[FMeshCount+FVisibleBillboardCount+FVisibleSpriteCount] = i + FMeshCount + FBillboardCount;
			FVisibleSpriteCount++;
		}
	}


	//クイックソート…ヘタにハッシュとかするより、あまりビルボードとかが多くないうちはこちらの方がトク
	QSort(0, FMeshCount + FVisibleBillboardCount + FVisibleSpriteCount - 1);


	//以上でOrderTable作成完了

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

	//デフォ値の代入
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
	//必要なら拡張する
	if( FMeshCount >= FMeshCapacity )
		FMeshCapacity = FMeshCapacity * 2;

	MeshQueue[FMeshCount].frame = frame;

	FMeshCount++;

}

void TSXRenderingQueue::PushBillboard( bool scaling, bool filter, D3DTEXTUREADDRESS addrU, D3DTEXTUREADDRESS addrV, TSXBlendMode blendMode, TSXFrame* ref, glm::vec3& pos, TDGTexture* tex, TSXVertexBB pts[] )
{
	//必要なら拡張する
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
	memmove(&BillboardQueue[FBillboardCount].pts[0], &pts[0], sizeof(TSXVertexBB)*4);  //4頂点分コピー

	FBillboardCount++;

}

void TSXRenderingQueue::PushSprite( bool filter, D3DTEXTUREADDRESS addrU, D3DTEXTUREADDRESS addrV, TSXBlendMode blendMode, glm::vec3& pos, TDGTexture* tex, TSXVertexSP pts[] )
{
	//必要なら拡張する
	if( FSpriteCount >= FSpriteCapacity )
		FSpriteCapacity = FSpriteCapacity * 2;

	SpriteQueue[FSpriteCount].filter = filter;
	SpriteQueue[FSpriteCount].addrU = addrU;
	SpriteQueue[FSpriteCount].addrV = addrV;
	SpriteQueue[FSpriteCount].blend = blendMode;
	SpriteQueue[FSpriteCount].tex = tex;
	SpriteQueue[FSpriteCount].pos = pos;
	memmove(&SpriteQueue[FSpriteCount].pts[0], &pts[0], sizeof(TSXVertexSP)*4);  //4頂点分コピー

	FSpriteCount++;

}


//ブレンドモードの変更
void TSXRenderingQueue::SetBlendMode(TSXBlendMode bm)																
{																					
	/*今までのと同じだったら何もしなくて良い*/										
	if( prevBlend == bm )															
		return;																		
	if( Scene->GetBlendInfos(bm).AlphaBlendEnable ) {													
		/*αブレンドあり*/														
		FOwner->Get3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, (true));						
		/*Z書き込みなし(比較は有り)*/											
		FOwner->Get3DDevice()->SetRenderState(D3DRS_ZWRITEENABLE, (false));							
		FOwner->Get3DDevice()->SetRenderState(D3DRS_SRCBLEND, (Scene->GetBlendInfos(bm).SrcBlend));							
		FOwner->Get3DDevice()->SetRenderState(D3DRS_DESTBLEND, (Scene->GetBlendInfos(bm).DestBlend));						
		if( Scene->BlendOpEnable() )												
			FOwner->Get3DDevice()->SetRenderState(D3DRS_BLENDOP, (Scene->GetBlendInfos(bm).BlendOp));						
	} else {																	
		/*αブレンドなし*/														
		FOwner->Get3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, (false));						
		/*Z書き込みあり	*/														
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

//フィルタの設定(BB,スプライト用)
void TSXRenderingQueue::SetFilterAddrBBSP(bool enable, D3DTEXTUREADDRESS addrU, D3DTEXTUREADDRESS addrV)														
{																									
	if( prevDrawnIsMesh ) {																		
		/*前にメッシュ描いてたなら、フィルタ状態保存*/											
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


//レンダリングステート周りの設定
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
	/*フィルタ*/																						
	if( ! prevDrawnIsMesh ) {																			
		RestoreFilterAddr();																			
		prevDrawnIsMesh = true;																			
	}																									
	SetDevs(MeshQueue[idx].frame->BlendMode, MeshQueue[idx].frame->GetTexture(), MeshQueue[idx].frame->Specular, MeshQueue[idx].frame->Lighting, MeshQueue[idx].frame->TextureWrap, MeshQueue[idx].frame->FillMode);		
	/*行列の設定*/																						
	FOwner->Get3DDevice()->SetTransform(D3DTS_WORLD, &mat4toD3DMATRIX(MeshQueue[idx].frame->GetMeshMatrixOnRender()));	
	/*マテリアルの設定*/																				
	FOwner->Get3DDevice()->SetMaterial(&MeshQueue[idx].frame->Material);									
	/*オーナードロウなら、イベントハンドラを呼ぶ*/														
	if( MeshQueue[idx].frame->OnRender ) {																
		/*イベントを呼んでオーナードロウ*/																
		MeshQueue[idx].frame->OnRender(*MeshQueue[idx].frame, *Scene, *FOwner);							
		/*テクスチャ戻す*/																				
		if( prevTex != NULL )																			
			FOwner->Get3DDevice()->SetTexture(0, prevTex->GetTexture());										
		else																							
			FOwner->Get3DDevice()->SetTexture(0, NULL);													
	} else {																							
		/*描画*/																						
		MeshQueue[idx].frame->Mesh->Draw();																
	}																									
}

//ビルボードの描画
void TSXRenderingQueue::DrawBB(  bool needSort, u32 idx )																								
{																												
	/*フィルタ*/																								
	SetFilterAddrBBSP(BillboardQueue[idx].filter, BillboardQueue[idx].addrU,BillboardQueue[idx].addrV);			
	/*レンダリングステートとかの設定*/																			
	SetDevs(BillboardQueue[idx].blend, BillboardQueue[idx].tex, true, false, 0, D3DFILL_SOLID);										
	/*ビルボードの中心の視野座標を求める*/																		
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
		/*ソート済みの場合は、ソートの段階で変換されている*/													
		center = BillboardQueue[idx].pos;																		
	}																											
	/*W値が正なら描画*/																							
	if( center.w > 0.f ) {																						
		/*ビルボードの中心の画面での座標を求める*/																
		center = ViewFrustumToScreen(center);																
		if( BillboardQueue[idx].scaling ) {																		
			/*スケーリングあり*/																				
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
			/*スケーリングなし*/																				
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
	/*参照フレームに付いたフラグを消す*/																	
	if( BillboardQueue[idx].ref != NULL )																	
		BillboardQueue[idx].ref->BBAttached = false;														
}

//スプライトの描画
void TSXRenderingQueue::DrawSP( u32 idx )																									
{																													
	/*フィルタ*/																										
	SetFilterAddrBBSP(SpriteQueue[idx].filter, SpriteQueue[idx].addrU, SpriteQueue[idx].addrV);						
	SetDevs(SpriteQueue[idx].blend, SpriteQueue[idx].tex, true, false, 0, D3DFILL_SOLID);											
	/*Z値が正なら描画*/																								
	if( SpriteQueue[idx].pos.z >= 0.f ) {																			
		/*Z値をそのままZバッファに描く…他のオブジェクトとのZに関する整合性が崩れるが、スプライトだからいいか*/		
		f32 bz = SpriteQueue[idx].pos.z;																				
		for( u32 i = 0 ; i <= 3 ; i++ ) {																			
			pts[i].sx = SpriteQueue[idx].pos.x + SpriteQueue[idx].pts[i].dx;								
			pts[i].sy = SpriteQueue[idx].pos.y + SpriteQueue[idx].pts[i].dy;								
			pts[i].sz = bz;																					
			pts[i].rhw = 0.5f;        /*てきとー	*/														
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
	//ビルボード用、視野座標→画面 の変換行列の計算
	D3DMATRIX tmpMat;
	FOwner->Get3DDevice()->GetTransform(D3DTS_PROJECTION, &tmpMat);
	projM = D3DMATRIXtomat4(tmpMat);
	screenM = Scene->GetScreenMatrix();

	viewProjScreenM = projM * viewMat;
	viewProjScreenM = screenM * viewProjScreenM;

	//スプライト用、視点座標系Z→Zバッファ変換用係数の計算
	zN = Scene->GetNearZ();
	zF = Scene->GetFarZ();
	zMin = Scene->GetFVP().MinZ;
	zMax = Scene->GetFVP().MaxZ;

	//ビルボード用、ビューポートのサイズ
	//W = 1のビルボードは、このサイズで表示される
	ratio_dx = viewParam.Width / 2.0f * projM[0][0];
	ratio_dy = viewParam.Height / 2.0f * projM[1][1];

	//変数の初期化
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

	//不透明体のαブレンド設定
	prevBlend = sxbDefault;
	FOwner->Get3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, u32(false));  //ブレンドしない
	FOwner->Get3DDevice()->SetRenderState(D3DRS_ZENABLE, u32(true));            //Z比較あり
	FOwner->Get3DDevice()->SetRenderState(D3DRS_ZWRITEENABLE, u32(true));       //Z書き込みあり

	//デフォルトのテクスチャブレンド設定
	FOwner->Get3DDevice()->SetTextureStageState(0, D3DTSS_COLORARG1, u32(D3DTA_TEXTURE));
	FOwner->Get3DDevice()->SetTextureStageState(0, D3DTSS_COLORARG2, u32(D3DTA_DIFFUSE));
	FOwner->Get3DDevice()->SetTextureStageState(0, D3DTSS_COLOROP,   u32(D3DTOP_MODULATE));

	FOwner->Get3DDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, u32(D3DTA_TEXTURE));
	FOwner->Get3DDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG2, u32(D3DTA_DIFFUSE));
	FOwner->Get3DDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP,   u32(D3DTOP_MODULATE));


	//フィルタの設定…はしなくていい(ビルボード・スプライトは個々に持っていて、メッシュはRenderStateに準じるから)

	//テクスチャアドレスモードの指定も同様に、せずともよい
	//FOwner->Get3DDevice()->SetSamplerState(0, D3DSAMP_ADDRESSU, u32(D3DTADDRESS_CLAMP));
	//FOwner->Get3DDevice()->SetSamplerState(0, D3DSAMP_ADDRESSV, u32(D3DTADDRESS_CLAMP));


	if( ! needSort ) {
		//メッシュ描画
		for( s32 i = 0 ; i < FMeshCount; i++ ) {
			DrawMesh(i);
		}
		//ビルボード描画
		for( s32 i = 0 ; i < FBillboardCount; i++ ) {
			DrawBB(needSort, i);
		}
		//スプライト描画
		for( s32 i = 0 ; i< FSpriteCount; i++ ) {
			DrawSP(i);
		}
	} else {
		//ソートして、オーダリングテーブルに書き込まれた順で描画
		Sort(viewParam);

		//ソートの段階でビルボードについては可視・不可視が判定済み
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

	//最後に書かれたのがスプライトかビルボなら、フィルタ状態を戻す
	if( ! prevDrawnIsMesh )
		RestoreFilterAddr();

	//キューを空に
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
	Memory(); //バッファクリア

}

HRESULT TSXStateRestorer::SetRS( D3DRENDERSTATETYPE RSType, u32 Value )
{
	//手をつけられてないフラグなら、保存しておく
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
	//手をつけられてないフラグなら、保存しておく
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
	//手をつけられてないフラグなら、保存しておく
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
	//フラグ消去、だけ
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
	//範囲チェック
	if( idx >= FCapacity ) {
		//DGCarad_PutDebugMessage('TSXLightGroup & SXLib: 範囲外のライトを参照しようとしました');
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
