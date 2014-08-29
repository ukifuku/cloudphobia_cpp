#include "BG3D.h"
#include <iostream>
#include <fstream>
#include <typeinfo>
#include "glm/gtx/compatibility.hpp"
#include "QDArc.h"
#include "TOBJList.h"
#include "Effect2.h"
#include "Effect3.h"

void TPolyLine::InitVertices()
{
	//ライン頂点の初期化
	for( u32 i = 0; i < Points.size(); i++ ){
		Points[i]  =  glm::vec3( (f32)i,0,0);
	}

	f32 uvwidth = (uv.u2 - uv.u1) / (Points.size() - 1);
	//ストリップ頂点の初期化
	u8* pV = (u8*)Mesh->VBuf()->Lock(0,0,0);

	TSXVertex v[6];
	for( u32 i = 0; i < Points.size(); i++ ) {
		v[0]  =  SXVertex(Points[i].x,  Points[i].y  ,Points[i].z  ,0, 1, 0,0xFFFFFFFF,0xFFFFFFFF,uv.u1 + uvwidth*i,uv.v1);
		v[1]  =  SXVertex(Points[i].x,  Points[i].y  ,Points[i].z+1,0, 1, 0,0xFFFFFFFF,0xFFFFFFFF,uv.u1 + uvwidth*i,uv.v2);
		v[2]  =  SXVertex(Points[i+1].x,Points[i+1].y,Points[i+1].z,0, 1, 0,0xFFFFFFFF,0xFFFFFFFF,uv.u1 + uvwidth*(i+1),uv.v1);
		v[3]  =  v[2];
		v[4]  =  v[1];
		v[5]  =  SXVertex(Points[i+1].x,Points[i+1].y,Points[i+1].z+1,0, 1, 0,0xFFFFFFFF,0xFFFFFFFF,uv.u1 + uvwidth*(i+1),uv.v2);

		memcpy( v, pV, Mesh->VBuf()->GetVertexSize()*6 );
		pV += Mesh->VBuf()->GetVertexSize()*6;
	}
	Mesh->VBuf()->Unlock();

	Mesh->IBuf()->Lock(0,0,0);
	//  for i  =  0 to Points.size() - 2 do {
	//    Mesh->IBuf()->Indices[i  ] = i*4 + 0;
	//    Mesh->IBuf()->Indices[i+1] = i*4 + 1;
	//    Mesh->IBuf()->Indices[i+2] = i*4 + 2;
	//    Mesh->IBuf()->Indices[i+3] = i*4 + 2;
	//    Mesh->IBuf()->Indices[i+4] = i*4 + 1;
	//    Mesh->IBuf()->Indices[i+5] = i*4 + 3;
	//  }

	for( u32 i = 0; i < Mesh->IBuf()->GetCount(); i++ ){
		Mesh->IBuf()->SetIndices(i, i);
	}

	Mesh->IBuf()->Unlock();

	Mesh->Compact();
	Mesh->Optimize();

}

TPolyLine::TPolyLine( TSceneManager* _owner, TFrameManager* _parent, s32 _segment, f32 _width, TDGTexture* _tex, UVRect& _uv, TSXBlendMode _blend )
{
	FOwner  =  _owner;
	FSegment = _segment;
	Points.resize(FSegment+1);
	Width = _width;
	IsUpdated = false;
	uv  =  _uv;
	ZoomRate = 1.f;

	Mesh = new TSXMesh(QD);
	Mesh->SetupBuffers((FSegment*2)*3, (FSegment*2)*3, FVF_SXVERTEX, sizeof(TSXVertex), D3DPOOL_MANAGED );
	InitVertices();

	FrameMng  = new TFrameManager(_owner, _parent, Mesh, _tex, _blend, false, false, false);
	//高速化のためカリング判定させない（そのかわり常に表示される）
	FrameMng->Frame->SetEnableBounds(false);
	FrameMng->Frame->SetEnableMeshTransform(false);
}

TPolyLine::~TPolyLine()
{
	SAFE_DELETE(Mesh);
	SAFE_DELETE(FrameMng);
}

void TPolyLine::SetPoint( s32 _num, glm::vec3& _vec )
{
	if( _num < Points.size() ) {
		Points[_num]  =  _vec;
		IsUpdated = true;
	}
}

void TPolyLine::SetZoomrate( f32 _zoom )
{
	ZoomRate = _zoom;

}

void TPolyLine::SetWidth( f32 _w )
{
	Width  =  _w;
}

void TPolyLine::CalcVartices()
{
	f32 tmpZoom = 1.f;
	u8* pV  =  (u8*)Mesh->VBuf()->Lock(0,0,0);
	for( u32 i = 0 ; i < Points.size(); i++ ) {
		// 点からカメラへの単位ベクトルを求める
		//親フレームの回転・移動を考慮して、カメラベクトルにフレームの逆行列をかけている。
		glm::mat4 tmpMat  = FrameMng->Frame->GetParent()->GetMatrix() * FrameMng->Frame->GetMatrix();
		glm::vec3 vec  =  TransformVec3(FOwner->CameraStand->GetTranslation(FOwner->Root), InvMatrix(tmpMat)) - Points[i];

		glm::vec3 tangent;
		// 点の単位接線ベクトルを求める
		if( i = 0 ){
			tangent  =  Points[i + 1] - Points[i];
		}
		else if( i = (Points.size() - 1) ){
			tangent  =  Points[i] - Points[i - 1];
		}
		else{
			tangent  =  Points[i + 1] - Points[i - 1];
		}

		//両ベクトルの外積を求める    
		glm::vec3 cross  =  glm::normalize(glm::cross(tangent, vec));

		//ポリゴン頂点を求める
		glm::vec3 v1  =  Points[i] - cross * (Width*tmpZoom);
		memcpy(pV, &v1, sizeof(glm::vec3));
		pV += Mesh->VBuf()->GetVertexSize();

		glm::vec3 v2  =  Points[i] + cross * (Width*tmpZoom);
		memcpy(pV, &v2, sizeof(glm::vec3));
		pV += Mesh->VBuf()->GetVertexSize();

		tmpZoom = tmpZoom*ZoomRate;
	}
	Mesh->VBuf()->Unlock();

	if( IsUpdated ) {
		//    centerNum  =  Points.size() div 2;
		//    Mesh->UpdateBounds;
		//    Mesh->Sphere.Center  =  Points[centerNum];
		//    Mesh->Sphere.R  =  NowDistance(Points[centerNum], Points[High(Points)]);
		IsUpdated = false;
	}

}


void TBG3DManager::CalcCameraAngle( s32 _dest )
{
	if( _dest>0 ) {
		for( u32 i  =  0; i < ViewList.size(); i++ ) {
			if( _dest <= ViewList[i]->Distance ) {
				s32 diffDist  =  ViewList[i]->Distance - ViewList[i-1]->Distance;
				s32 DistFromPrev  =  _dest - ViewList[i-1]->Distance;
				s32 PrevRX  =  ViewList[i-1]->RX;
				s32 PrevRY  =  ViewList[i-1]->RY;
				s32 PrevRZ  =  ViewList[i-1]->RZ;

				s32 diffRX  =  GetDiffR(PrevRX , ViewList[i]->RX);
				s32 diffRY  =  GetDiffR(PrevRY , ViewList[i]->RY);
				s32 diffRZ  =  GetDiffR(PrevRZ , ViewList[i]->RZ);

				if( diffDist > 0 ) {
					glm::vec3 CurrPos  =  glm::vec3(ViewList[i]->X , ViewList[i]->Y, ViewList[i]->Z);
					glm::vec3 PrevPos  =  glm::vec3(ViewList[i-1]->X , ViewList[i-1]->Y, ViewList[i-1]->Z);


					f32 DiffLength  =  glm::distance(CurrPos, PrevPos);

					f32 PrevSpeed  =  ViewList[i-1]->Speed;
					f32 CurrentSpeed  =  ViewList[i]->Speed;

					f32 NewSpeed  =   glm::lerp(PrevSpeed, CurrentSpeed, (f32)DistFromPrev / (f32)diffDist);
					f32 NewLength  =  (PrevSpeed+NewSpeed) * (f32)DistFromPrev / (f32)KSCROLLSPEED_NORMAL / 2.f;

					if( ViewList[i]->Spline ) {
						//スプライン曲線に沿った移動
						glm::vec3 NextPos;
						if( i < ViewList.size() - 1 )
							NextPos  =  glm::vec3(ViewList[i+1]->X , ViewList[i+1]->Y, ViewList[i+1]->Z);
						else
							NextPos  =  CurrPos;

						glm::vec3 PrevPrevPos;
						if( i>1 )
							PrevPrevPos  =  glm::vec3(ViewList[i-2]->X , ViewList[i-2]->Y, ViewList[i-2]->Z);
						else
							PrevPrevPos  =  PrevPos;

						glm::vec3 FinalPos;
						FinalPos.x = CalcCatmullRom( NewLength / DiffLength, PrevPrevPos.x , PrevPos.x, CurrPos.x, NextPos.x);
						FinalPos.y = CalcCatmullRom( NewLength / DiffLength, PrevPrevPos.y , PrevPos.y, CurrPos.y, NextPos.y);
						FinalPos.z = CalcCatmullRom( NewLength / DiffLength, PrevPrevPos.z , PrevPos.z, CurrPos.z, NextPos.z);

						FOwner->GetScene()->CameraMove(FinalPos.x,FinalPos.y,FinalPos.z);
					}
					else {
						//直線移動
						FOwner->GetScene()->CameraMove(ViewList[i-1]->X , ViewList[i-1]->Y, ViewList[i-1]->Z);

						glm::vec3 FinalPos  =  glm::normalize(CurrPos - PrevPos) * NewLength;

						FOwner->GetScene()->CameraMoveR(FinalPos.x,FinalPos.y,FinalPos.z);
					}

					//オフセット設定されている場合はさらにカメラをずらす
					FOwner->GetScene()->CameraMoveR(CameraOffset.x,CameraOffset.y,CameraOffset.z);


					//カメラ回転
					s32 RX, RY, RZ;
					if( (s32)((PrevSpeed - CurrentSpeed)*1000.f) == 0 ) {
						//等速移動の場合は、cosを使用してなめらかに視点移動
						s32 CurrentRad  =  RoundOff((f32)DistFromPrev / (f32)diffDist * 2048.f);

						RX  =  PrevRX + RoundOff((f32)diffRX/2.f - (f32)diffRX * costable[CurrentRad]/2.f);
						RY  =  PrevRY + RoundOff((f32)diffRY/2.f - (f32)diffRY * costable[CurrentRad]/2.f);
						RZ  =  PrevRZ + RoundOff((f32)diffRZ/2.f - (f32)diffRZ * costable[CurrentRad]/2.f);
					}
					else {
						//等速でない場合は、視点回転スピードを移動スピードに合わせる
						RX  =  PrevRX + RoundOff((f32)diffRX * NewLength / DiffLength);
						RY  =  PrevRY + RoundOff((f32)diffRY * NewLength / DiffLength);
						RZ  =  PrevRZ + RoundOff((f32)diffRZ * NewLength / DiffLength);
					}
					FOwner->GetScene()->CameraRotate(RX,RY,RZ);
				}

				break;
			}
		}
	}


}

void TBG3DManager::LoadMesh()
{
	SAFE_DELETE_VECTOR( MeshList );
	SAFE_DELETE_VECTOR( BBList );
	SAFE_DELETE_VECTOR( MaterialList );
	SAFE_DELETE_VECTOR( EffectList );

	if( DebugMode ) {
		std::string line;
		std::ifstream ifs(DEBUG_GRAPH_PATH + "meshlist.txt");

		// テキストファイル読み込み
		while (std::getline(ifs, line)) {
			SetUpMesh(line);
		}
		ifs.close();

		ifs.open( DEBUG_GRAPH_PATH + "billboardlist.txt" );
		while (std::getline(ifs, line)) {
			SetUpBB(line);
		}
		ifs.close();

		ifs.open( DEBUG_GRAPH_PATH + "effectlist.txt" );
		while (std::getline(ifs, line)) {
			SetUpEffect(line);
		}
		ifs.close();
	}
	else {
		std::string line;
		std::stringstream stream;
		ExtractFromQDAFile(stream, "./Data/map.bin", "meshlist.txt");
		while (std::getline(stream, line)) {
			SetUpMesh(line);
		}
		stream.str("");


		ExtractFromQDAFile(stream, "./Data/map.bin", "billboardlist.txt");
		while (std::getline(stream, line)) {
			SetUpBB(line);
		}
		stream.str("");


		ExtractFromQDAFile(stream, "./Data/map.bin", "effectlist.txt");
		while (std::getline(stream, line)) {
			SetUpEffect(line);
		}
		stream.str("");
	}

}

f32 TBG3DManager::CalcCatmullRom( f32 _t, f32 _x0, f32 _x1, f32 _x2, f32 _x3 )
{
	f32 t2 = _t*_t;
	f32 t3 = t2*_t;

	glm::mat4 mat;
	mat[0][0] = -1.0f/2.0f; mat[1][0] =  3.0f/2.0f; mat[2][0] = -3.0f/2.0f; mat[3][0] =  1.0f/2.0f;
	mat[0][1] =  2.0f/2.0f; mat[1][1] = -5.0f/2.0f; mat[2][1] =  4.0f/2.0f; mat[3][1] = -1.0f/2.0f;
	mat[0][2] = -1.0f/2.0f; mat[1][2] =  0.0f/2.0f; mat[2][2] =  1.0f/2.0f; mat[3][2] =  0.0f/2.0f;
	mat[0][3] =  0.0f/2.0f; mat[1][3] =  2.0f/2.0f; mat[2][3] =  0.0f/2.0f; mat[3][3] =  0.0f/2.0f;

	glm::vec4 vec(t3, t2, _t, 1.f);

	vec = mat * vec;

	return _x0 * vec.x + _x1 * vec.y + _x2 * vec.z + _x3 * vec.w;
}

void TBG3DManager::LoadTextureFromMeshID( s32 _id )
{
	s32 texid = -1;
	if( _id < MeshList.size() ) {
		if( MeshList[_id]->TextureID >= 0 ){
			texid  =  MeshList[_id]->TextureID;
		}
	}
	else if( _id < KIDOFFSET_EFFECT ) {
		texid  =  BBList[_id - KIDOFFSET_BILLBOARD]->TextureID ;
	}
	else {
		texid  =  BBList[_id - KIDOFFSET_BILLBOARD]->TextureID ;
		if( EffectList[_id - KIDOFFSET_EFFECT]->TextureID >= 0 ){
			texid  =  EffectList[_id - KIDOFFSET_EFFECT]->TextureID;
		}
	}

	if( (texid >= Textures.size() ) || (texid < 0) ) return;

	if( Textures[texid] == NULL ) {
		Textures[texid] = new TDGTexture(QD, DGFMT_ARGB, 0, D3DPOOL_MANAGED);
		if( DebugMode )
			Textures[texid]->LoadFromFile(DEBUG_GRAPH_PATH + TextureFileName[texid]);
		else
			Textures[texid]->LoadFromFile(TextureFileName[texid], "./Data/map.bin");
	}
}

void TBG3DManager::LoadMeshFromMeshID( s32 _id )
{
	//メッシュを読み込む
	if( _id < MeshList.size() ) {
		if( MeshList[_id]->Mesh == NULL ) {
			MeshList[_id]->Mesh = new TSXMesh(QD);
			MeshList[_id]->Mesh->LoadFromFile(MeshList[_id]->FileName, "./Data/map.bin");
			MeshList[_id]->Mesh->Compact();
			MeshList[_id]->Mesh->Optimize();
		}
	}
	else if( _id < KIDOFFSET_EFFECT ) {

	}
	else {
		s32 effectid = -1;
		switch(_id - KIDOFFSET_EFFECT){
		case  4:effectid  =  26; break;
		case 14:effectid  =  38; break;
		case 15:effectid  =  39; break;
		case 16:effectid  =  41; break;
		case 17:effectid  =  44; break;
		case 18:effectid  =  45; break;
		case 19:effectid  =  46; break;
		case 20:effectid  =  47; break;
		case 21:effectid  =  48; break;
		case 22:effectid  =  49; break;
		case 23:effectid  =  50; break;
		case 24:effectid  =  51; break;
		case 25:effectid  =  53; break;
		case 26:effectid  =  56; break;
		case 28:effectid  =  59; break;
		case 29:effectid  =  60; break;
		case 37:effectid  =  37; break;
		case 38:effectid  =  38; break;
		case 39:effectid  =  39; break;
		}

		if( (effectid < 0) || (effectid >= MeshList.size() )) return;

		if( MeshList[effectid]->Mesh=NULL ) {
			MeshList[effectid]->Mesh = new TSXMesh(QD);
			MeshList[effectid]->Mesh->LoadFromFile(MeshList[effectid]->FileName, "./Data/map.bin");
			MeshList[effectid]->Mesh->Compact();
			MeshList[effectid]->Mesh->Optimize();
		}
	}
}

void TBG3DManager::CreatePolyline( s32 _id )
{
	s32 amount = 0;
	TDGTexture* tex = NULL;
	s32 seg = 0;
	f32 width = 0;
	TSXBlendMode blend = sxbDefault;
	s32 plkind = 0;

	UVRect uv;
	switch( _id ){
	case 0:{
		uv.u1  =  0;
		uv.u2  =  2.0f;
		uv.v1  =  0;
		uv.v2  =  0.25f;
		plkind = _id;
		seg = 20;
		width = 1.f;
		tex  =  Textures[EffectList[_id]->TextureID];
		blend = sxbAdd;
		amount = 6;
		   }
		   break;
	case 2:{
		uv.u1  =  0;
		uv.u2  =  2.0f;
		uv.v1  =  0;
		uv.v2  =  0.25f;
		plkind = _id;
		seg = 10;
		width = 0.6f;
		tex  =  Textures[EffectList[_id]->TextureID];
		blend = sxbAdd;
		amount = 2;
		   }
		   break;
	case 3:{
		uv.u1  =  0;
		uv.u2  =  2.0f;
		uv.v1  =  0.25f;
		uv.v2  =  0.5f;
		plkind = _id;
		seg = 20;
		width = 1;
		tex  =  Textures[EffectList[_id]->TextureID];
		blend = sxbAdd;
		amount = 6;
		   }
		   break;
	case 8:
	case 9:{
		uv.u1  =  0;
		uv.u2  =  2.f;
		uv.v1  =  0;
		uv.v2  =  1.f;
		plkind = KPOLYLINE_MISSILESMOKE;
		seg = 48;
		width = 0.8f;
		tex  =  Eff6Tex;
		blend = sxbAlpha;
		amount = 1;
		   }
		   break;
	case 10:{
		uv.u1  =  0;
		uv.u2  =  2.0f;
		uv.v1  =  1.f;
		uv.v2  =  0;
		plkind = KPOLYLINE_BSHIPSMOKE;
		seg = 10;
		width = 1.f;
		tex  =  Eff6Tex;
		blend = sxbAlpha;
		amount = 1;
			}
			break;
	case 12:{
		uv.u1  =  0;
		uv.u2  =  0.5f;
		uv.v1  =  0;
		uv.v2  =  0.25f;
		plkind = KPOLYLINE_STRAIGHTBEAM1;
		seg = 1;
		width = 1;
		tex  =  Textures[EffectList[_id]->TextureID];
		blend = sxbAdd;
		amount = 1;
			}
			break;
	case 13:{
		uv.u1  =  0;
		uv.u2  =  0.5f;
		uv.v1  =  0.25f;
		uv.v2  =  0.5f;
		plkind = KPOLYLINE_STRAIGHTBEAM2;
		seg = 1;
		width = 1.f;
		tex  =  Textures[EffectList[_id]->TextureID];
		blend = sxbAdd;
		amount = 1;
			}
			break;
	case 27:{
		uv.u1  =  0;
		uv.u2  =  2.0f;
		uv.v1  =  0;
		uv.v2  =  1.f;
		plkind = KPOLYLINE_BUILDINGSMOKE;
		seg = 10;
		width = 1;
		tex  =  Eff6Tex;
		blend = sxbAlpha;
		amount = 2;
			}
			break;
	case 30:{
		uv.u1  =  0;
		uv.u2  =  0.5f;
		uv.v1  =  0;
		uv.v2  =  0.25f;
		plkind = KPOLYLINE_STRAIGHTBEAMCTRL1;
		seg = 1;
		width = 1.f;
		tex  =  Textures[EffectList[_id]->TextureID];
		blend = sxbAdd;
		amount = 8;
			}
			break;
	case 31:{
		uv.u1  =  0;
		uv.u2  =  0.5f;
		uv.v1  =  0.25f;
		uv.v2  =  0.5f;
		plkind = KPOLYLINE_STRAIGHTBEAMCTRL2;
		seg = 1;
		width = 1;
		tex  =  Textures[EffectList[_id]->TextureID];
		blend = sxbAdd;
		amount = 8;
			}  
			break;
	case 32:{
		uv.u1  =  0;
		uv.u2  =  1.f;
		uv.v1  =  0.5f;
		uv.v2  =  0.75f;
		plkind = KPOLYLINE_HOMINGLASER;
		seg = 48;
		width = 1.f;
		tex  =  Textures[EffectList[_id]->TextureID];
		blend = sxbAdd;
		amount = 18;
			}
			break;
	}

	if( amount > 0 ) {
		for( s32 i = 0; i < amount; i++ ) {
			TPolyLineInfo info;
			info.pl = new TPolyLine(FOwner->GetScene(), NULL, seg, width, tex,uv, blend);
			info.pl->FrameMng->Frame->SetVisibility(sxvHide);
			info.Kind = plkind;
			info.Used = false;

			PolyLinePool.push_back(info);
		}
	}
}

TBG3DManager::TBG3DManager( TOBJList* _owner )
{
	FOwner = _owner;
	ScrollSpeed = 0.1f;
	Clear();

	FTotalObj = 0;
	CameraOffset = glm::vec3(0,0,0);
	LoadMesh();
	MakeTextures();
}

TBG3DManager::~TBG3DManager()
{
	Clear();
	SAFE_DELETE_VECTOR(BBList);
	SAFE_DELETE_VECTOR(EffectList);
	SAFE_DELETE_VECTOR(Textures);
	SAFE_DELETE_VECTOR(MeshList);

	// ★★★　PolyLinePoolの開放も
}

void TBG3DManager::SetUpMesh( const string& _line )
{
	//コメント行は飛ばす
	if( _line.find_first_of("//") == 0 ) return;

	MeshData* meshdata = new MeshData();
	//名前
	meshdata->Name = ReadSegment(_line,0);

	//ファイル名
	meshdata->FileName = ReadSegment(_line,1);

	//テクスチャＩＤ
	meshdata->TextureID = std::stoi(ReadSegment(_line,2));

	//ライティング
	meshdata->Lighting = std::stoi(ReadSegment(_line,3)) != 0;

	//ブレンドモード
	meshdata->BlendMode  =  std::stoi(ReadSegment(_line,4));

	//メッシュを読み込む
	meshdata->Mesh = new TSXMesh(QD);
	if( DebugMode ){
		meshdata->Mesh->LoadFromFile(DEBUG_GRAPH_PATH + meshdata->FileName);
	}
	else{
		meshdata->Mesh->LoadFromFile(meshdata->FileName, "./Data/map.bin");
	}
	meshdata->Mesh->Compact();
	meshdata->Mesh->Optimize();

	MeshList.push_back(meshdata);

	MaterialData* matdata = new MaterialData();
	matdata->Diffuse = 1;
	matdata->Specular = 1;
	matdata->Ambient = 1;
	matdata->Emissive = 0;
	matdata->Power = 10;
	matdata->u = 0;
	matdata->v = 0;
	MaterialList.push_back( matdata );

}

void TBG3DManager::SetUpBB( const string& _line )
{
	//コメント行は飛ばす
	if( _line.find_first_of("//") == 0 ) return;

	BBData* bbdata = new BBData();

	//名前
	bbdata->Name = ReadSegment(_line,0);

	//UV情報
	bbdata->u  =  std::stoi(ReadSegment(_line,1));
	bbdata->v  =  std::stoi(ReadSegment(_line,2));
	bbdata->width  =  std::stoi(ReadSegment(_line,3));
	bbdata->height  =  std::stoi(ReadSegment(_line,4));

	//テクスチャＩＤ
	bbdata->TextureID = std::stoi(ReadSegment(_line,5));

	//ブレンドモード
	bbdata->BlendMode  =  std::stoi(ReadSegment(_line,6));

	BBList.push_back(bbdata);
}

void TBG3DManager::SetUpEffect( const string& _line )
{
	//コメント行は飛ばす
	if( _line.find_first_of("//") == 0 ) return;

	EffectData* effdata = new EffectData();

	//名前
	effdata->Name = ReadSegment(_line,0);

	//テクスチャＩＤ
	effdata->TextureID = std::stoi(ReadSegment(_line,1));

	//ライティング
	effdata->Lighting = std::stoi(ReadSegment(_line,2)) != 0;

	//ブレンドモード
	effdata->BlendMode  =  std::stoi(ReadSegment(_line,3));

	//パラメタ項目数
	effdata->numParam  =  std::stoi(ReadSegment(_line,4));

	for( s32 i = 0; i < effdata->numParam; i++ ) {
		effdata->ParamName.push_back( ReadSegment(_line,5+i*2) );
		effdata->ParamVal.push_back( std::stof(ReadSegment(_line,6+i*2)) );
	}

	EffectList.push_back( effdata );

}

void TBG3DManager::Scroll( s32 _distance )
{
	s32 frame =  RoundOff((f32)_distance / (f32)KSCROLLSPEED_NORMAL);

	//カメラの移動

	if( ViewList.size() > 0 ){
		CalcCameraAngle(_distance);
	}
	else {
		f32 CX,CY,CZ;
		FOwner->GetScene()->GetCameraPos(CX,CY,CZ);
		FOwner->GetScene()->CameraMove(frame * ScrollSpeed,CY,CZ);
	}

	for( u32 i = 0; i < FrameList.size(); i++ ) {
		//メッシュを出現させるか判定
		if( (FrameList[i]->AppearDistance <= _distance) && (! FrameList[i]->IsAppeared) ) {
			if( (FrameList[i]->DisappearDistance < 0) || (FrameList[i]->DisappearDistance > _distance) ) {
				MakeMeshFrame(i);
				FTotalObj++;
			}
			FrameList[i]->IsAppeared  =  true;
		}

		if( FrameList[i]->FrameMng && FrameList[i]->FrameMng->Frame ) {
			//画面に表示されたかどうか
			if( (! FrameList[i]->IsRendered) && (FrameList[i]->FrameMng->Frame->Rendered()) ) {          
				FrameList[i]->IsRendered  =  true;
			}

			//フリーズ設定されているなら、位置をカメラ前に固定
			//      if( (FrameList[i]->Freeze) && (FrameList[i]->IsAppeared) ) {
			//          FrameList[i]->FrameMng->Move(CX,FrameList[i]->FrameMng->GetY,FrameList[i]->FrameMng->GetZ);
			//      }

			//画面外に消えたらメッシュ解放
			//      if( (FrameList[i]->IsRendered) && (! FrameList[i]->FrameMng->Frame.Rendered) ) {
			//          FrameList[i]->FrameMng->Frame.Free;
			//          FrameList[i]->FrameMng  =  NULL;
			//          Dec(FTotalObj);
			//      }


			//Hideするかチェック

			if( (FrameList[i]->HideDistance > 0) && (FrameList[i]->HideDistance > _distance) && (FrameList[i]->FrameMng->Frame->GetVisibility() == sxvHide) )
				FrameList[i]->FrameMng->Frame->SetVisibility(sxvShow);

			if( (FrameList[i]->HideDistance > 0) && (FrameList[i]->HideDistance <= _distance) && (FrameList[i]->FrameMng->Frame->GetVisibility() == sxvShow) )
				FrameList[i]->FrameMng->Frame->SetVisibility(sxvHide);

			if( (FrameList[i]->DisappearDistance > 0) && (FrameList[i]->DisappearDistance <= _distance)
				&& (FrameList[i]->IsAppeared ) && (! FrameList[i]->IsDeleted) ) {

					if( FrameList[i]->MeshID < KIDOFFSET_EFFECT ) {
						SAFE_DELETE( FrameList[i]->FrameMng );
					}
					else {
						//          if( FrameList[i]->EffectPtr!=NULL ) TOBJ(FrameList[i]->EffectPtr).Die;
						//          FrameList[i]->EffectPtr = NULL;
					}


					FTotalObj--;
					FrameList[i]->IsDeleted = true;
			}
		}
	}
}

void TBG3DManager::Load( const string& _file )
{
	Clear();

	if( DebugMode )
		Load_debug(_file);
	else {
		//圧縮ファイルから読み込み
		std::stringstream stream;
		ExtractFromQDAFile(stream, "./Data/map.BIN", _file );

		//MaxDistance
		MaxDistance  =  std::stoi(GetLine(stream));
		//FOV
		FOwner->GetScene()->SetFOV(std::stoi(GetLine(stream))); 

		//光源
		f32 difR  =  std::stof(GetLine(stream));   
		f32 difG  =  std::stof(GetLine(stream));   
		f32 difB  =  std::stof(GetLine(stream));   

		f32 specR  =  std::stof(GetLine(stream));  
		f32 specG  =  std::stof(GetLine(stream));  
		f32 specB  =  std::stof(GetLine(stream));  

		f32 ambR  =  std::stof(GetLine(stream));   
		f32 ambG  =  std::stof(GetLine(stream));   
		f32 ambB  =  std::stof(GetLine(stream));   

		f32 dirX  =  std::stof(GetLine(stream));   
		f32 dirY  =  std::stof(GetLine(stream));   
		f32 dirZ  =  std::stof(GetLine(stream));   

		FOwner->GetScene()->GlobalLight->SetupDiffuse(difR, difG, difB);
		FOwner->GetScene()->GlobalLight->SetupSpecular(specR, specG, specB);
		FOwner->GetScene()->GlobalLight->SetupAmbient(ambR, ambG, ambB);
		FOwner->GetScene()->GlobalLight->SetupDirectional(glm::vec3(dirX, dirY, dirZ));

		//マテリアル
		s32 NumMaterial = std::stoi(GetLine(stream));    
		for( s32 i = 0; i < NumMaterial; i++ ) {
			MaterialList[i]->Diffuse  =  std::stof(GetLine(stream));  
			MaterialList[i]->Specular  =  std::stof(GetLine(stream)); 
			MaterialList[i]->Ambient  =  std::stof(GetLine(stream));  
			MaterialList[i]->Emissive  =  std::stof(GetLine(stream)); 
			MaterialList[i]->Power  =  std::stof(GetLine(stream));    
			MaterialList[i]->u  =  std::stof(GetLine(stream));        
			MaterialList[i]->v  =  std::stof(GetLine(stream));        
		}

		//マップ
		s32 NumFrame  =  std::stoi(GetLine(stream));        
		for( s32 i  =  0; i < NumFrame; i++ ){
			FrameData* frmdata = new FrameData();

			frmdata->MeshID  =  std::stoi(GetLine(stream));      
			//必要なテクスチャだけ読み込む
			LoadTextureFromMeshID(frmdata->MeshID);
			//必要なメッシュだけ読み込む
			//      LoadMeshFromMeshID(MeshID);            //やっぱ最初に全部読み込んでおく

			//ポリラインをあらかじめ作成
			//        if( (MeshID>=KIDOFFSET_EFFECT) )
			//          CreatePolyLine(MeshID - KIDOFFSET_EFFECT);

			frmdata->AppearDistance  =  std::stoi(GetLine(stream));         
			if( (frmdata->MeshID >= KIDOFFSET_BILLBOARD) && (frmdata->MeshID < KIDOFFSET_EFFECT) )
				frmdata->AppearDistance  =  0;

			frmdata->DisappearDistance  =  std::stoi(GetLine(stream));        
			if( frmdata->AppearDistance < 0 ) frmdata->DisappearDistance = -1;
			if( (frmdata->MeshID >= KIDOFFSET_BILLBOARD) && (frmdata->MeshID < KIDOFFSET_EFFECT) )
				frmdata->DisappearDistance = -1;

			frmdata->HideDistance  =  std::stoi(GetLine(stream));         
			//HideDistanceが有効になっている場合はそれをDisAppearDistanceとしてつかう
			if( frmdata->HideDistance >= 0 ) frmdata->DisappearDistance = frmdata->HideDistance;

			frmdata->Freeze  = std::stoi(GetLine(stream)) != 0;    
			frmdata->X  =  std::stof(GetLine(stream));          
			frmdata->Y  =  std::stof(GetLine(stream));          
			frmdata->Z  =  std::stof(GetLine(stream));          
			frmdata->RX  =  std::stoi(GetLine(stream));           
			frmdata->RY  =  std::stoi(GetLine(stream));           
			frmdata->RZ  =  std::stoi(GetLine(stream));           
			frmdata->ZX  =  std::stof(GetLine(stream));         
			frmdata->ZY  =  std::stof(GetLine(stream));         
			frmdata->ZZ  =  std::stof(GetLine(stream));         

			//パラメタ
			frmdata->numParam = std::stoi(GetLine(stream));      
			for( s32 k = 0; k < frmdata->numParam; k++ ){
				frmdata->ParamVal.push_back( std::stof(GetLine(stream)) );   
			}

			frmdata->IsAppeared = false;
			frmdata->IsRendered = false;
			frmdata->IsDeleted = false;

			FrameList.push_back(frmdata);

		}

		//カメラ
		s32 NumView  =  std::stoi(GetLine(stream));      
		for( s32 i = 0; i < NumView; i++ ) {
			ViewData* viewdat = new ViewData();
			viewdat->Distance  =  std::stoi(GetLine(stream));  

			viewdat->X  =  std::stof(GetLine(stream));     
			viewdat->Y  =  std::stof(GetLine(stream));        
			viewdat->Z  =  std::stof(GetLine(stream));    

			viewdat->RX  =  std::stoi(GetLine(stream));        
			viewdat->RY  =  std::stoi(GetLine(stream));        
			viewdat->RZ  =  std::stoi(GetLine(stream));        

			viewdat->Speed  =  std::stof(GetLine(stream));      
			viewdat->Spline  =  std::stoi(GetLine(stream)) != 0;      
			ViewList.push_back(viewdat);
		}
	}

	PrevPos = 0;

	MoveTo();

}

void TBG3DManager::Load_debug( const string& _file )
{
	Clear();

	//ファイルを開く
	std::ifstream ifs( DEBUG_GRAPH_PATH + _file );

	//MaxDistance
	MaxDistance  =  std::stoi(GetLine(ifs));

	//FOV
	FOwner->GetScene()->SetFOV(std::stoi(GetLine(ifs)));

	//光源
	f32 difR  =  std::stof(GetLine(ifs));
	f32 difG  =  std::stof(GetLine(ifs));
	f32 difB  =  std::stof(GetLine(ifs));

	f32 specR  =  std::stof(GetLine(ifs));
	f32 specG  =  std::stof(GetLine(ifs));
	f32 specB  =  std::stof(GetLine(ifs));

	f32 ambR  =  std::stof(GetLine(ifs));
	f32 ambG  =  std::stof(GetLine(ifs));
	f32 ambB  =  std::stof(GetLine(ifs));

	f32 dirX  =  std::stof(GetLine(ifs));
	f32 dirY  =  std::stof(GetLine(ifs));
	f32 dirZ  =  std::stof(GetLine(ifs));

	FOwner->GetScene()->GlobalLight->SetupDiffuse(difR, difG, difB);
	FOwner->GetScene()->GlobalLight->SetupSpecular(specR, specG, specB);
	FOwner->GetScene()->GlobalLight->SetupAmbient(ambR, ambG, ambB);
	FOwner->GetScene()->GlobalLight->SetupDirectional(glm::vec3(dirX, dirY, dirZ));

	//マテリアル
	s32 NumMaterial = std::stoi(GetLine(ifs));
	for( s32 i = 0; i < NumMaterial; i++ ) {
		MaterialList[i]->Diffuse  =  std::stof(GetLine(ifs));
		MaterialList[i]->Specular  =  std::stof(GetLine(ifs));
		MaterialList[i]->Ambient  =  std::stof(GetLine(ifs));
		MaterialList[i]->Emissive  =  std::stof(GetLine(ifs));
		MaterialList[i]->Power  =  std::stof(GetLine(ifs));
		MaterialList[i]->u  =  std::stof(GetLine(ifs));
		MaterialList[i]->v  =  std::stof(GetLine(ifs));
	}

	//マップ
	s32 NumFrame  =  std::stoi(GetLine(ifs));
	for( s32 i = 0; i < NumFrame; i++ ){
		FrameData* framedat = new FrameData();
		framedat->MeshID  =  std::stoi(GetLine(ifs));
		//必要なテクスチャだけ読み込む
		LoadTextureFromMeshID(framedat->MeshID);
		//必要なメッシュだけ読み込む
		//      LoadMeshFromMeshID(MeshID);            //やっぱ最初に全部読み込んでおく

		//ポリラインをあらかじめ作成
		//        if( (MeshID>=KIDOFFSET_EFFECT) )
		//          CreatePolyLine(MeshID - KIDOFFSET_EFFECT);

		framedat->AppearDistance  =  std::stoi(GetLine(ifs));
		if( (framedat->MeshID >= KIDOFFSET_BILLBOARD) && (framedat->MeshID < KIDOFFSET_EFFECT) )
			framedat->AppearDistance  =  0;

		framedat->DisappearDistance  =  std::stoi(GetLine(ifs));
		if( framedat->AppearDistance < 0 ) framedat->DisappearDistance = -1;
		if( (framedat->MeshID >= KIDOFFSET_BILLBOARD) && (framedat->MeshID < KIDOFFSET_EFFECT) )
			framedat->DisappearDistance = -1;
		framedat->HideDistance  =  std::stoi(GetLine(ifs));
		//HideDistanceが有効になっている場合はそれをDisAppearDistanceとしてつかう
		if( framedat->HideDistance >= 0 ) framedat->DisappearDistance = framedat->HideDistance;

		framedat->Freeze  =  std::stoi(GetLine(ifs)) != 0;

		framedat->X  =  std::stof(GetLine(ifs));

		framedat->Y  =  std::stof(GetLine(ifs));

		framedat->Z  =  std::stof(GetLine(ifs));

		framedat->RX  =  std::stoi(GetLine(ifs));

		framedat->RY  =  std::stoi(GetLine(ifs));

		framedat->RZ  =  std::stoi(GetLine(ifs));

		framedat->ZX  =  std::stof(GetLine(ifs));

		framedat->ZY  =  std::stof(GetLine(ifs));

		framedat->ZZ  =  std::stof(GetLine(ifs));

		//パラメタ
		framedat->numParam = std::stoi(GetLine(ifs));

		for( s32 k = 0; k < framedat->numParam; k++ ){
			framedat->ParamVal.push_back( std::stof(GetLine(ifs)) );
		}

		framedat->IsAppeared = false;
		framedat->IsRendered = false;
		framedat->IsDeleted = false;

		FrameList.push_back(framedat);

	}

	//カメラ
	s32 NumView  =  std::stoi(GetLine(ifs));
	for( s32 i = 0; i < NumView; i++ ) {
		ViewData* viewdat = new ViewData();

		viewdat->Distance  =  std::stoi(GetLine(ifs));

		viewdat->X  =  std::stof(GetLine(ifs));
		viewdat->Y  =  std::stof(GetLine(ifs));
		viewdat->Z  =  std::stof(GetLine(ifs));

		viewdat->RX  =  std::stoi(GetLine(ifs));
		viewdat->RY  =  std::stoi(GetLine(ifs));
		viewdat->RZ  =  std::stoi(GetLine(ifs));

		viewdat->Speed  =  std::stof(GetLine(ifs));
		viewdat->Spline  =  std::stoi(GetLine(ifs)) != 0;

		ViewList.push_back(viewdat);
	}
	ifs.close();
}

void TBG3DManager::Clear()
{
	//フレームデータを解放
	for( s32 i = 0; i < FrameList.size(); i++ ){
		if( FrameList[i]->MeshID < KIDOFFSET_EFFECT ) {
			SAFE_DELETE( FrameList[i]->FrameMng );
		}
		//    else
		//      if( FrameList[i]->EffectPtr!=NULL ) TOBJ(FrameList[i]->EffectPtr).Die;
	}
	SAFE_DELETE_VECTOR( FrameList );

	//テクスチャを開放
	SAFE_DELETE_VECTOR( Textures );

	//ポリラインを解放
	for( s32 i = 0; i < PolyLinePool.size(); i++ ) {
		SAFE_DELETE( PolyLinePool[i].pl );
	}
	PolyLinePool.clear();

	//  for i  =  0 to Length(MeshList) - 1 do {
	//    if( MeshList[i].Mesh!=NULL ) {
	//      MeshList[i].Mesh->Free;
	//      MeshList[i].Mesh = NULL;
	//    }
	//  }

	SAFE_DELETE_VECTOR( ViewList );
}

void TBG3DManager::MakeMeshFrame( s32 _id )
{
	FrameData* frmdat = FrameList[_id];

	//  SetLength(FrameList, Length(FrameList)+1);
	TDGTexture* tex;
	if( FrameList[_id]->MeshID < MeshList.size() ) {

		if( MeshList[FrameList[_id]->MeshID]->TextureID >= 0 )
			tex  =  Textures[MeshList[FrameList[_id]->MeshID]->TextureID];
		else
			tex  =  NULL;

		frmdat->FrameMng  = new TFrameManager( FOwner->GetScene(), NULL, MeshList[frmdat->MeshID]->Mesh, tex, (TSXBlendMode)MeshList[frmdat->MeshID]->BlendMode, MeshList[frmdat->MeshID]->Lighting, MeshList[frmdat->MeshID]->Lighting, false);
		frmdat->EffectPtr = NULL;

		//マテリアル
		MaterialData* mat = MaterialList[frmdat->MeshID];
		D3DMATERIAL9 matdata;
		matdata.Diffuse.r  =  mat->Diffuse;
		matdata.Diffuse.g  =  mat->Diffuse;
		matdata.Diffuse.b  =  mat->Diffuse;

		matdata.Specular.r  =  mat->Specular;
		matdata.Specular.g  =  mat->Specular;
		matdata.Specular.b  =  mat->Specular;

		matdata.Ambient.r  =  mat->Ambient;
		matdata.Ambient.g  =  mat->Ambient;
		matdata.Ambient.b  =  mat->Ambient;

		matdata.Emissive.r  =  mat->Emissive;
		matdata.Emissive.g  =  mat->Emissive;
		matdata.Emissive.b  =  mat->Emissive;

		matdata.Power  =  mat->Power;

		frmdat->FrameMng->Frame->SetMaterialRcv(matdata);
	}
	else if( frmdat->MeshID < KIDOFFSET_EFFECT ) {
		//ビルボード用フレーム
		tex  =  Textures[BBList[frmdat->MeshID - KIDOFFSET_BILLBOARD]->TextureID] ;
		frmdat->FrameMng  = new TFrameManager(FOwner->GetScene(),NULL,NULL,tex,sxbDefault,false,false,false);
		//string meshname = BBList[frmdat->MeshID - KIDOFFSET_BILLBOARD]->Name;
		frmdat->AppearDistance  =  0;  //可視性判定が出来ないのでとりあえず常に表示させる（TBD）
		frmdat->EffectPtr = NULL;
	}
	else {
		//特殊エフェクト用フレーム
		if( EffectList[frmdat->MeshID - KIDOFFSET_EFFECT]->TextureID>=0 )
			tex  =  Textures[EffectList[frmdat->MeshID - KIDOFFSET_EFFECT]->TextureID];
		else
			tex  =  NULL;

		//パラメタIDに対応したオブジェクトを生成
		switch( frmdat->MeshID - KIDOFFSET_EFFECT ){
		case 0:{
			frmdat->EffectPtr = new TBattleShipBeam(OBJList, tex, frmdat->ParamVal[1], frmdat->ParamVal[4], 0);
			frmdat->FrameMng  =  ((TBattleShipBeam*)frmdat->EffectPtr)->Frame;
			   }
			   break;
		case 1:{
			frmdat->EffectPtr = new TBattleShipBeam(OBJList, tex, frmdat->ParamVal[1], frmdat->ParamVal[4], 1);
			frmdat->FrameMng  =  ((TBattleShipBeam*)frmdat->EffectPtr)->Frame;
			   }
			   break;
		case 2:{
			frmdat->EffectPtr = new TBattleShipBeam(OBJList, tex, frmdat->ParamVal[1], frmdat->ParamVal[4],2);
			frmdat->FrameMng  =  ((TBattleShipBeam*)frmdat->EffectPtr)->Frame;
			   }
			   break;
		case 3:{
			frmdat->EffectPtr = new TBattleShipBeam(OBJList, tex, frmdat->ParamVal[1], frmdat->ParamVal[4],3);
			frmdat->FrameMng  =  ((TBattleShipBeam*)frmdat->EffectPtr)->Frame;
			   }
			   break;
		case 4:{
			frmdat->EffectPtr = new TBattleShip3(OBJList, MeshList[26].Mesh, tex, MaterialList[26], RoundOff(frmdat->ParamVal[0]), RoundOff(frmdat->ParamVal[1]), frmdat->ParamVal[2]);
			frmdat->FrameMng  =  ((TBattleShip3*)frmdat->EffectPtr)->Frame;
			   }
			   break;
		case 5:{
			UVRect uv;
			uv.u1  =  0;
			uv.v1  =  0;
			uv.u2  =  0.5f;
			uv.v2  =  0.5f;
			frmdat->EffectPtr = new TBoardControl(OBJList,tex, uv, frmdat->ParamVal[0], frmdat->ParamVal[1], frmdat->ParamVal[2], 128, frmdat->ParamVal[3], frmdat->ParamVal[4]);
			frmdat->FrameMng  = ((TBoardControl*)frmdat->EffectPtr)->Frame;
			   }
			   break;
		case 6:{
			UVRect uv;
			uv.u1  =  0.5f;
			uv.v1  =  0;
			uv.u2  =  1.f;
			uv.v2  =  0.5f;
			frmdat->EffectPtr = new TBoardControl(OBJList,tex, uv, frmdat->ParamVal[0], frmdat->ParamVal[1], frmdat->ParamVal[2], 128, frmdat->ParamVal[3], frmdat->ParamVal[4]);
			frmdat->FrameMng  =  ((TBoardControl*)frmdat->EffectPtr)->Frame;
			   }
			   break;
		case 7:{
			UVRect uv;
			uv.u1  =  0;
			uv.v1  =  0.5f;
			uv.u2  =  0.5f;
			uv.v2  =  1.f;
			frmdat->EffectPtr = new TBoardControl(OBJList,tex, uv, frmdat->ParamVal[0], frmdat->ParamVal[1], frmdat->ParamVal[2], 255, frmdat->ParamVal[3], frmdat->ParamVal[4]);
			frmdat->FrameMng  = ((TBoardControl*)frmdat->EffectPtr)->Frame;
			   }
			   break;
		case 8:{
			frmdat->EffectPtr = new TMissile3DGen(OBJList, 1, frmdat->ParamVal[0], frmdat->ParamVal[1], frmdat->ParamVal[2], frmdat->ParamVal[3], frmdat->ParamVal[4]);
			frmdat->FrameMng  = ((TMissile3DGen*)frmdat->EffectPtr)->Frame;
			   }
			   break;
		case 9:{
			frmdat->EffectPtr = new TMissile3DGen(OBJList, 0,  frmdat->ParamVal[0],  frmdat->ParamVal[1],  frmdat->ParamVal[2], frmdat->ParamVal[3], frmdat->ParamVal[4]);
			frmdat->FrameMng  = ((TMissile3DGen*)frmdat->EffectPtr)->Frame;
			   }
			   break;
		case 10:{
			frmdat->EffectPtr = new TBattleShipSmoke(OBJList, NULL, frmdat->ParamVal[0], frmdat->ParamVal[3]);
			frmdat->FrameMng  = ((TBattleShipSmoke*)frmdat->EffectPtr)->Frame;
				}
				break;
		case 11:{
			frmdat->EffectPtr = new TBattleShipFire(OBJList, tex, frmdat->ParamVal[0], frmdat->ParamVal[1], frmdat->ParamVal[2], frmdat->ParamVal[3]);
			frmdat->FrameMng  = ((TBattleShipFire*)frmdat->EffectPtr)->Frame;
				}
				break;
		case 12:{
			frmdat->EffectPtr = new TStraightBeam(OBJList, tex,0, frmdat->ParamVal[0], frmdat->ParamVal[1], frmdat->ParamVal[2]);
			frmdat->FrameMng  = ((TStraightBeam*)frmdat->EffectPtr)->Frame;
				}
				break;
		case 13:{
			frmdat->EffectPtr = new TStraightBeam(OBJList, tex,1,  frmdat->ParamVal[0],  frmdat->ParamVal[1],  frmdat->ParamVal[2]);
			frmdat->FrameMng  = ((TStraightBeam*)frmdat->EffectPtr)->Frame;
				}
				break;
		case 14:{
			frmdat->EffectPtr = new TRotateMesh(OBJList, MeshList[38]->Mesh, tex);
			frmdat->FrameMng  = ((TRotateMesh*)frmdat->EffectPtr)->Frame;
				}
				break;
		case 15:{
			frmdat->EffectPtr = new TRotateMesh(OBJList, MeshList[39]->Mesh, tex);
			frmdat->FrameMng  = ((TRotateMesh*)frmdat->EffectPtr)->Frame;
				}
				break;
		case 16:{
			frmdat->EffectPtr = new TShutter(OBJList, MeshList[41]->Mesh, tex, frmdat->ParamVal[0], frmdat->ParamVal[1], frmdat->ParamVal[2]);
			frmdat->FrameMng  = ((TShutter*)frmdat->EffectPtr)->Frame;
				}
				break;
		case 17:{
			frmdat->EffectPtr = new TLandscape(OBJList, MeshList[44]->Mesh, tex, 0);
			frmdat->FrameMng  = ((TLandscape*)frmdat->EffectPtr)->Frame;
				}
				break;
		case 18:{
			frmdat->EffectPtr = new TLandscape(OBJList, MeshList[45]->Mesh, tex, 2);
			frmdat->FrameMng  = ((TLandscape*)frmdat->EffectPtr)->Frame;
				}
				break;
		case 19:{
			frmdat->EffectPtr = new TLandscape(OBJList, MeshList[46]->Mesh, tex, 1);
			frmdat->FrameMng  = ((TLandscape*)frmdat->EffectPtr)->Frame;
				}
				break;
		case 20:{
			frmdat->EffectPtr = new TLandscape(OBJList, MeshList[47]->Mesh, tex, 0);
			frmdat->FrameMng  = ((TLandscape*)frmdat->EffectPtr)->Frame;
				}
				break;
		case 21:{
			frmdat->EffectPtr = new TLandscape(OBJList, MeshList[48]->Mesh, tex, 0);
			frmdat->FrameMng  = ((TLandscape*)frmdat->EffectPtr)->Frame;
				}
				break;
		case 22:{
			frmdat->EffectPtr = new TLandscape(OBJList, MeshList[49]->Mesh, tex, 0);
			frmdat->FrameMng  = ((TLandscape*)frmdat->EffectPtr)->Frame;
				}
				break;
		case 23:{
			frmdat->EffectPtr = new TLandscape(OBJList, MeshList[50]->Mesh, tex, 2);
			frmdat->FrameMng  = ((TLandscape*)frmdat->EffectPtr)->Frame;
				}
				break;
		case 24:{
			frmdat->EffectPtr = new TLandscape(OBJList, MeshList[51]->Mesh, tex, 1);
			frmdat->FrameMng  = ((TLandscape*)frmdat->EffectPtr)->Frame;
				}
				break;
		case 25:{
			frmdat->EffectPtr = new TElevator(OBJList, MeshList[53]->Mesh, tex, frmdat->ParamVal[4], frmdat->ParamVal[5], frmdat->ParamVal[6]);
			frmdat->FrameMng  = ((TElevator*)frmdat->EffectPtr)->Frame;
				}
				break;
		case 26:{
			frmdat->EffectPtr = new TBuildingMove(OBJList, MeshList[56]->Mesh, tex, MaterialList[56], frmdat->ParamVal[0], frmdat->ParamVal[1], frmdat->ParamVal[2], frmdat->ParamVal[3], frmdat->ParamVal[4], frmdat->ParamVal[5]);
			frmdat->FrameMng  = ((TBuildingMove*)frmdat->EffectPtr)->Frame;
				}
				break;
		case 27:{
			frmdat->EffectPtr = new TBuildingSmoke(OBJList, frmdat->ParamVal[0], frmdat->ParamVal[1], frmdat->ParamVal[2], frmdat->ParamVal[3], frmdat->ParamVal[4], frmdat->ParamVal[5]);
			frmdat->FrameMng  = ((TBuildingSmoke*)frmdat->EffectPtr)->Frame;
				}
				break;
		case 28:{
			frmdat->EffectPtr = new TRotateCloud(OBJList, MeshList[59]->Mesh, tex, frmdat->ParamVal[0], frmdat->ParamVal[1], frmdat->ParamVal[2], frmdat->ParamVal[3], frmdat->ParamVal[4]);
			frmdat->FrameMng  = ((TRotateCloud*)frmdat->EffectPtr)->Frame;
				}
				break;
		case 29:{
			frmdat->EffectPtr = new TBuildingMove(OBJList, MeshList[60]->Mesh, tex, MaterialList[60], frmdat->ParamVal[0], frmdat->ParamVal[1], frmdat->ParamVal[2], frmdat->ParamVal[3], frmdat->ParamVal[4], frmdat->ParamVal[5]);
			frmdat->FrameMng  = ((TBuildingMove*)frmdat->EffectPtr)->Frame;
				}
				break;
		case 30:{
			frmdat->EffectPtr = new TStraightBeamControl(OBJList, tex, 0);
			frmdat->FrameMng  = ((TStraightBeamControl*)frmdat->EffectPtr)->Frame;
				}
				break;
		case 31:{
			frmdat->EffectPtr = new TStraightBeamControl(OBJList, tex, 1);
			frmdat->FrameMng  = ((TStraightBeamControl*)frmdat->EffectPtr)->Frame;
				}
				break;
		case 32:{
			frmdat->EffectPtr = new THomingLaserGen(OBJList, tex, frmdat->ParamVal[0], frmdat->ParamVal[1], frmdat->ParamVal[2], frmdat->ParamVal[3], frmdat->ParamVal[4], frmdat->ParamVal[5], frmdat->ParamVal[6], frmdat->ParamVal[7]);
			frmdat->FrameMng  = ((THomingLaserGen*)frmdat->EffectPtr)->Frame;
				}
				break;
		case 33:{
			UVRect uv;
			uv.u1  =  0;
			uv.v1  =  0;
			uv.u2  =  0.5f;
			uv.v2  =  0.5f;
			frmdat->EffectPtr = new TBoardControl(OBJList, tex, uv, frmdat->ParamVal[0], frmdat->ParamVal[1], frmdat->ParamVal[2], 128, frmdat->ParamVal[3], frmdat->ParamVal[4]);
			frmdat->FrameMng  = ((TBoardControl*)frmdat->EffectPtr)->Frame;
				}
				break;
		case 34:{
			UVRect uv;
			uv.u1  =  0.5f;
			uv.v1  =  0.01f;
			uv.u2  =  1.f;
			uv.v2  =  0.49f;
			frmdat->EffectPtr = new TBoardControl(OBJList, tex, uv, frmdat->ParamVal[0], frmdat->ParamVal[1], frmdat->ParamVal[2], 128, frmdat->ParamVal[3], frmdat->ParamVal[4]);
			frmdat->FrameMng  = ((TBoardControl*)frmdat->EffectPtr)->Frame;
				}
				break;
		case 35:{
			UVRect uv;
			uv.u1  =  0.005f;
			uv.v1  =  0.5f;
			uv.u2  =  0.495f;
			uv.v2  =  1.f;
			frmdat->EffectPtr = new TBoardControl(OBJList, tex, uv, frmdat->ParamVal[0], frmdat->ParamVal[1], frmdat->ParamVal[2], 255, frmdat->ParamVal[3], frmdat->ParamVal[4]);
			frmdat->FrameMng  = ((TBoardControl*)frmdat->EffectPtr)->Frame;
				}
				break;
		case 36:{
			frmdat->EffectPtr = new TMissile3DGen(OBJList, 2, frmdat->ParamVal[0], frmdat->ParamVal[1], frmdat->ParamVal[2], frmdat->ParamVal[3], frmdat->ParamVal[4]);
			frmdat->FrameMng  = ((TMissile3DGen*)frmdat->EffectPtr)->Frame;
				}
				break;
		case 37:{
			frmdat->EffectPtr = new TRoopSlideMesh(OBJList, MeshList[37]->Mesh, MaterialList[37], tex ,MeshList[37]->BlendMode );
			frmdat->FrameMng  = ((TRoopSlideMesh*)frmdat->EffectPtr)->Frame;
				}
				break;
		case 38:{
			frmdat->EffectPtr = new TRoopSlideMesh(OBJList, MeshList[38]->Mesh, MaterialList[38], tex , MeshList[38]->BlendMode );
			frmdat->FrameMng  = ((TRoopSlideMesh*)frmdat->EffectPtr)->Frame;
				}
				break;
		case 39:{
			frmdat->EffectPtr = new TRoopSlideMesh(OBJList, MeshList[39]->Mesh, MaterialList[39], tex , MeshList[39]->BlendMode );
			frmdat->FrameMng  = ((TRoopSlideMesh*)frmdat->EffectPtr)->Frame;
				}
				break;
		}
		ReflectParam(frmdat);
		if( frmdat->EffectPtr != NULL ) OBJList.Add( frmdat->EffectPtr );
	}

	if( frmdat->FrameMng != NULL ) {
		frmdat->FrameMng->Zoom(frmdat->ZX, frmdat->ZY, frmdat->ZZ);
		frmdat->FrameMng->Rotate(frmdat->RX, frmdat->RY, frmdat->RZ);

		if( frmdat->Freeze ) {
			frmdat->FrameMng->Frame->SetParent(FOwner->GetScene()->CameraStand);
			f32 CX, CY, CZ;
			FOwner->GetScene()->GetCameraPos(CX, CY, CZ);
			frmdat->FrameMng->Move( CX+frmdat->X, CY+frmdat->Y, CZ+frmdat->Z);
		}
		else {
			//        FrameMng->Frame.Parent  =  FOwner->GetScene()->Root;
			frmdat->FrameMng->Move( frmdat->X, frmdat->Y, frmdat->Z );
		}
	}

}

void TBG3DManager::MakeTextures()
{
	SAFE_DELETE_VECTOR(Textures);
	SAFE_DELETE_VECTOR(TextureFileName);

	if( DebugMode ) {
		std::string line;
		std::ifstream ifs(DEBUG_GRAPH_PATH + "texturelist.txt");

		while( std::getline(ifs, line) ) {
			Textures.push_back(NULL);
			TextureFileName.push_back(line);
		}
		ifs.close();

	}
	else {
		std::string line;
		std::stringstream stream;
		ExtractFromQDAFile(stream, "./Data/map.bin", "texturelist.txt");

		while (std::getline(stream, line)) {
			Textures.push_back(NULL);
			TextureFileName.push_back(line);
		}
		stream.str("");
	}
}

void TBG3DManager::MoveTo()
{
	if( ViewList.size() <= 0 ) return;

	ViewData* tmpData = ViewList[0];

	FOwner->GetScene()->CameraRotate(tmpData->RX, tmpData->RY, tmpData->RZ);
	FOwner->GetScene()->CameraMove(tmpData->X, tmpData->Y, tmpData->Z);
}

void TBG3DManager::ReflectParam( const FrameData& _framedata )
{
	switch(_framedata.MeshID - KIDOFFSET_EFFECT) {
		case 0:
		case 1:
		case 2:
		case 3:{
			((TBattleShipBeam*)_framedata.EffectPtr)->SetParam(_framedata.ParamVal[0],_framedata.ParamVal[1],_framedata.ParamVal[2],_framedata.ParamVal[3],_framedata.ParamVal[4]);
		}
			   break;
		case 4:{
			((TBattleShip3*)_framedata.EffectPtr)->SetParam(_framedata.ParamVal[0],_framedata.ParamVal[1],_framedata.ParamVal[2]);
		}
			   break;
		case 5:
		case 6:
		case 7:{
			((TBoardControl*)_framedata.EffectPtr)->SetParam(_framedata.ParamVal[0],_framedata.ParamVal[1],_framedata.ParamVal[2],_framedata.ParamVal[3],_framedata.ParamVal[4]);
		}
			   break;
		case 8:
		case 9:
		case 36:{
			((TMissile3DGen*)_framedata.EffectPtr)->SetParam(_framedata.ParamVal[0],_framedata.ParamVal[1],_framedata.ParamVal[2],_framedata.ParamVal[3],_framedata.ParamVal[4]);
		}
				break;
		case 10:{
			((TBattleShipSmoke*)_framedata.EffectPtr)->SetParam(_framedata.ParamVal[0],_framedata.ParamVal[1],_framedata.ParamVal[2],_framedata.ParamVal[3]);
		}
				break;
		case 11:{
			((TBattleShipFire*)_framedata.EffectPtr)->SetParam(_framedata.ParamVal[0],_framedata.ParamVal[1],_framedata.ParamVal[2],_framedata.ParamVal[3]);
		}
				break;
		case 12:
		case 13:{
			((TStraightBeam*)_framedata.EffectPtr)->SetParam(_framedata.ParamVal[0],_framedata.ParamVal[1],_framedata.ParamVal[2]);
		}
				break;
		case 14:
		case 15:{
			((TRotateMesh*)_framedata.EffectPtr)->SetParam(_framedata.ParamVal[0],_framedata.ParamVal[1],_framedata.ParamVal[2]);
		}
				break;
		case 16:{
			((TShutter*)_framedata.EffectPtr)->SetParam(_framedata.ParamVal[0],_framedata.ParamVal[1],_framedata.ParamVal[2]);
		}
				break;
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		case 22:
		case 23:
		case 24:{
			((TLandscape*)_framedata.EffectPtr)->SetParam(_framedata.ParamVal[0],_framedata.ParamVal[1],_framedata.ParamVal[2],_framedata.ParamVal[3]);
		}
				break;
		case 25:{
			((TElevator*)_framedata.EffectPtr)->SetParam(_framedata.ParamVal[0],_framedata.ParamVal[1],_framedata.ParamVal[2],_framedata.ParamVal[3],_framedata.ParamVal[4],_framedata.ParamVal[5],_framedata.ParamVal[6]);
		}
				break;
		case 26:{
			((TBuildingMove*)_framedata.EffectPtr)->SetParam(_framedata.ParamVal[0],_framedata.ParamVal[1],_framedata.ParamVal[2],_framedata.ParamVal[3],_framedata.ParamVal[4],_framedata.ParamVal[5]);
		}
				break;
		case 27:{
			((TBuildingSmoke*)_framedata.EffectPtr)->SetParam(_framedata.ParamVal[0],_framedata.ParamVal[1],_framedata.ParamVal[2],_framedata.ParamVal[3],_framedata.ParamVal[4],_framedata.ParamVal[5]);
		}
				break;
		case 28:{
			((TRotateCloud*)_framedata.EffectPtr)->SetParam(_framedata.ParamVal[0],_framedata.ParamVal[1],_framedata.ParamVal[2],_framedata.ParamVal[3], _framedata.ParamVal[4]);
		}
				break;
		case 29:{
			((TBuildingMove*)_framedata.EffectPtr)->SetParam(_framedata.ParamVal[0],_framedata.ParamVal[1],_framedata.ParamVal[2],_framedata.ParamVal[3],_framedata.ParamVal[4],_framedata.ParamVal[5]);
		}
				break;
		case 30:
		case 31:{
			((TStraightBeamControl*)_framedata.EffectPtr)->SetParam(_framedata.ParamVal[0],_framedata.ParamVal[1],_framedata.ParamVal[2],_framedata.ParamVal[3],_framedata.ParamVal[4],_framedata.ParamVal[5],_framedata.ParamVal[6]);
		}
				break;
		case 32:{
			((THomingLaserGen*)_framedata.EffectPtr)->SetParam(_framedata.ParamVal[0],_framedata.ParamVal[1],_framedata.ParamVal[2],_framedata.ParamVal[3],_framedata.ParamVal[4],_framedata.ParamVal[5],_framedata.ParamVal[6],_framedata.ParamVal[7]);
		}
				break;
		case 33:
		case 34:
		case 35:{
			((TBoardControl*)_framedata.EffectPtr)->SetParam(_framedata.ParamVal[0],_framedata.ParamVal[1],_framedata.ParamVal[2],_framedata.ParamVal[3],_framedata.ParamVal[4]);
		}
				break;
		case 37:
		case 38:
		case 39:{
			((TRoopSlideMesh*)_framedata.EffectPtr)->SetParam(_framedata.ParamVal[0],_framedata.ParamVal[1],_framedata.ParamVal[2],_framedata.ParamVal[3],_framedata.ParamVal[4],_framedata.ParamVal[5],_framedata.ParamVal[6]);
		}
				break;
	}

}

void TBG3DManager::RenderBB()
{
	for( s32 i = 0; i < FrameList.size(); i++ ) {
		if( FrameList[i]->FrameMng != NULL ) {
			if( (FrameList[i]->MeshID >= KIDOFFSET_BILLBOARD) &&
				(FrameList[i]->MeshID < KIDOFFSET_EFFECT) &&
				(FrameList[i]->FrameMng->Frame.Visibility == sxvShow) &&
				(FrameList[i]->IsAppeared) && (! FrameList[i]->IsDeleted )
				) {
					s32 BBID  =  FrameList[i]->MeshID - KIDOFFSET_BILLBOARD;
					
					UVRect uv;
					uv.u1  =  BBList[BBID]->u / Textures[BBList[BBID]->TextureID].Width;
					uv.v1  =  BBList[BBID]->v / Textures[BBList[BBID]->TextureID].Height;
					uv.u2  =  (BBList[BBID]->u + BBList[BBID]->width) / Textures[BBList[BBID]->TextureID].Width;
					uv.v2  =  (BBList[BBID]->v + BBList[BBID]->height) / Textures[BBList[BBID]->TextureID].Height;

					FOwner->GetScene()->SetBillBoardSX2(FrameList[i]->FrameMng->Frame, 0, 0, 0,
						RoundOff(BBList[BBID]->width * FrameList[i]->FrameMng->getZoomX()),
						RoundOff(BBList[BBID]->height * FrameList[i]->FrameMng->getZoomY()),
						uv,
						255,
						Textures[BBList[BBID]->TextureID],
						BBList[BBID]->BlendMode,
						true,
						false);
				}
		}
	}

}

void TBG3DManager::FrameControl()
{
	//UVアニメ、フレームの回転、移動などを行う
	for( s32 k = 0; k < MaterialList.size(); k++ ) {
		for( s32 i = 0; i < FrameList.size(); i++ ) {
			if( (FrameList[i]->MeshID == k) && (FrameList[i]->FrameMng != NULL) ) {
				FrameList[i]->FrameMng->SlideUV(MaterialList[k]->u, MaterialList[k]->v);
				break;
			}
		}
	}
}

void TBG3DManager::ResetFrameList()
{
	//フレームデータを解放
	for( s32 i = 0; i < FrameList.size(); i++ ) {
		if( FrameList[i]->MeshID < KIDOFFSET_EFFECT ) {
			if( (FrameList[i]->FrameMng != NULL) ) {
				SAFE_DELETE(FrameList[i]->FrameMng);
			}
		}
		//    else
		//      if( FrameList[i]->EffectPtr!=NULL ) TOBJ(FrameList[i]->EffectPtr).Die;

		FrameList[i]->IsAppeared = false;
		FrameList[i]->IsRendered = false;
		FrameList[i]->IsDeleted = false;
	}
	FTotalObj = 0;

}

void TBG3DManager::SetCameraOffset( f32 _x, f32 _y, f32 _z )
{
	CameraOffset = glm::vec3(_x, _y, _z);

}

TPolyLine* TBG3DManager::GetPolyLine( s32 _kind, TFrameManager* _parent )
{
	TPolyLine* result = NULL;
	for( s32 i = 0; i < PolyLinePool.size(); i++ ){
		if( (PolyLinePool[i]->Kind == _kind) && (! PolyLinePool[i]->Used) ) {
			PolyLinePool[i]->Used  =  true;
			result  =  PolyLinePool[i]->pl;
			PolyLinePool[i]->pl->FrameMng->Frame.Parent  =  _parent.Frame;
			PolyLinePool[i]->pl->FrameMng->Frame.Visibility  =  sxvShow;
			break;
		}
	}

	return result;
}

std::string TBG3DManager::GetLine( std::istream& _stream )
{
	std::string line;
	std::getline(_stream, line);

	return line;
}
