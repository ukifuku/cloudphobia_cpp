#include "Effect4.h"
#include "TOBJList.h"
#include "SXScene.h"
#include "BG3D.h"
#include "Zako01.h"
#include "Sound.h"
#include "SpriteEXM.h"
#include "Effect.h"
#include "glm/gtx/compatibility.hpp"

TBoard::TBoard( f32 u1, f32 v1, f32 u2, f32 v2 )
{
	TSXVertex v[6];

	Mesh  =  new TSXMesh(QD);
	Mesh->SetupBuffers(6, 6, FVF_SXVERTEX, sizeof(TSXVertex), D3DPOOL_DEFAULT );

	u8* pV = (u8*)Mesh->VBuf()->Lock(0,0,0);
	Mesh->IBuf()->Lock(0,0,0);

	v[0]  =  SXVertex(-0.5,0.5, 0 ,0, 1, 0,0xFFFFFFFF,0xFFFFFFFF,u1,v1);
	v[1]  =  SXVertex(0.5, 0.5, 0, 0, 1, 0,0xFFFFFFFF,0xFFFFFFFF,u1,v2);
	v[2]  =  SXVertex(-0.5, -0.5, 0, 0, 1, 0,0xFFFFFFFF,0xFFFFFFFF,u2,v1);
	v[3]  =  v[2];
	v[4]  =  v[1];
	v[5]  =  SXVertex(0.5,  -0.5, 0, 0, 1, 0,0xFFFFFFFF,0xFFFFFFFF,u2,v2);

	memmove( pV, v, Mesh->VBuf()->GetVertexSize()*6);

	Mesh->IBuf()->SetIndices(0, 0);
	Mesh->IBuf()->SetIndices(1, 1);
	Mesh->IBuf()->SetIndices(2, 2);
	Mesh->IBuf()->SetIndices(3, 2);
	Mesh->IBuf()->SetIndices(4, 1);
	Mesh->IBuf()->SetIndices(5, 3);

	for( u32 i  =  0 ; i < Mesh->IBuf()->GetCount(); i++ ) {
		Mesh->IBuf()->SetIndices(i, i);
	}

	Mesh->VBuf()->Unlock();
	Mesh->IBuf()->Unlock();

	Mesh->Compact();
	Mesh->Optimize();
	Mesh->UpdateBounds();

}

TBoard::~TBoard()
{
	SAFE_DELETE( Mesh );
}

TBattleShipBeam::TBattleShipBeam( TOBJList* owner, TDGTexture* _tex, f32 _speed, f32 _space, s32 _pat ) : TOBJ(owner)
{
	UVSlideSpeed  =  _speed;
	Offset  =  0;
	Pattern  =  _pat;
	XSpace  =  _space;
	FKind  =  kEffect2;
	Texture = _tex;
	//親フレーム生成
	Frame  =  new TFrameManager(FOwner->GetScene(),NULL,NULL,NULL,sxbAdd,false,false,false);

	//ビームの色
	UVRect uv;
	switch( Pattern ){
	case 0:
	case 1:
	case 2:{
		uv.u1  =  0;
		uv.u2  =  2.0;
		uv.v1  =  0;
		uv.v2  =  0.25;
		   }break;
	case 3:
	case 4:{
		uv.u1  =  0;
		uv.u2  =  2.0;
		uv.v1  =  0.25;
		uv.v2  =  0.5;
		   }break;
	}

	switch( Pattern ){
	case 0:
	case 3:{
		for( u32 i = 0; i <= 5; i++ ) {
			TPolyLine* newline = new TPolyLine(FOwner->GetScene(), Frame, 20, 1, _tex,uv, sxbAdd);
			//        pl[i] = FOwner->BG3DMan.GetPolyLine(KPOLYLINE_BSHIPBEAM1 + Pattern, Frame);
			newline->FrameMng->SlideUV(0.16f*i,0.f);

			pl.push_back(newline);
		}

		pl[1]->FrameMng->RotateR(0,0,2048);
		pl[2]->FrameMng->RotateR(0,0,-512);
		pl[3]->FrameMng->RotateR(0,0,2560);
		pl[4]->FrameMng->RotateR(0,0,512);
		pl[5]->FrameMng->RotateR(0,0,1536);

		SetParam(Offset, UVSlideSpeed, 1,1,XSpace);
		   }break;
	case 1:{
		for( u32 i = 0; i <= 2; i++ ) {
			TPolyLine* newline = new TPolyLine(FOwner->GetScene(), Frame, 20, 1, _tex,uv, sxbAdd);
			newline->FrameMng->SlideUV(0.30f*i, 0.f);
			pl.push_back(newline);
		}
		pl[0]->FrameMng->RotateR(0,-1024,1024);
		pl[1]->FrameMng->RotateR(0,-1024,1024);
		pl[2]->FrameMng->RotateR(0,-1024,1024);

		SetParam(Offset, UVSlideSpeed, 1,1,XSpace);
		   }break;
	case 2:{
		for( u32 i = 0; i <= 1; i++ ) {
			TPolyLine* newline = new TPolyLine(FOwner->GetScene(), Frame, 10, 0.6f, _tex,uv, sxbAdd);
			newline->FrameMng->SlideUV(0.4f*i,0.f);
			pl.push_back(newline);
		}
		pl[0]->FrameMng->MoveR(2,0,0.3f);
		pl[1]->FrameMng->MoveR(-2,0,0.3f);

		SetParam(Offset, UVSlideSpeed, 2,1,XSpace);
		   }
	}

	f32 tempZoom = 1.0f;
	for( u32 i  =  0 ; i < pl.size() ; i++ ) {
		zoomrate.push_back( tempZoom );
		tempZoom = tempZoom*0.5f;
	}

}

TBattleShipBeam::~TBattleShipBeam()
{
	for( u32 i  =  0 ; i < pl.size(); i++ ) {
		SAFE_DELETE( pl[i] );
	}

	SAFE_DELETE( Frame );
}

void TBattleShipBeam::Move()
{
	TOBJ::Move();
	if( (Pattern == 3) && (FOwner->Distance < 14820) && (FOwner->NowStage == 2)) 
		return;     //苦し紛れ

	if( Frame->Frame->GetVisibility() == sxvShow ) {
		for( u32 i  =  0 ; i < pl.size(); i++ ) {
			zoomrate[i] = zoomrate[i]*0.9f;
			if( zoomrate[i]<0.05f ) zoomrate[i] = 1.0f;
			pl[i]->FrameMng->SlideUV(UVSlideSpeed,0);
		}
	}

}

s32 TBattleShipBeam::OnDraw()
{
	if( (Pattern == 3) && (FOwner->Distance < 14820) && (FOwner->NowStage == 2)) {
		return 1;     //苦し紛れ
	}

	//根本エフェクト
	UVRect UV;
	if( Frame->Frame->GetVisibility() == sxvShow ) {
		switch( Pattern ){
		case 0:
		case 1:
		case 2:{
			UV.u1  =  1/256.f;
			UV.v1  =  1/256.f;
			UV.u2  =  128/256.f;
			UV.v2  =  64/256.f;
			   }break;
		case 3:
		case 4:{
			UV.u1  =  1/256.f;
			UV.v1  =  65/256.f;
			UV.u2  =  128/256.f;
			UV.v2  =  128/256.f;
			   }break;
		}

		for( u32 i  =  0 ; i < pl.size(); i++ ) {
			f32 size = zoomrate[i] * 10.f;

			if( zoomrate[i]>0.5f ) {
				glm::vec3 pos = pl[i]->FrameMng->Frame->GetTranslation(FOwner->GetScene()->Root);
				pos = pos + TransformVec3(glm::vec3(1.5f, 0.f, 0.5f), pl[i]->FrameMng->Frame->GetWorldMatrixRot());
				FOwner->GetScene()->SetBillBoardSX2(FOwner->GetScene()->Root, pos.x, pl[i]->FrameMng->Frame->GetTranslation(FOwner->GetScene()->Root).y, pos.z,
					size*2,
					size,
					UV,
					255,
					Texture,
					sxbAdd,
					true,
					true);
			}
			pl[i]->CalcVartices();
		}

	}
	return 1;
}

void TBattleShipBeam::SetParam( f32 uvoffset, f32 speed, f32 zx, f32 zy, f32 space )
{
	f32 PrevOffset  =  Offset;

	UVSlideSpeed  =  speed;
	Offset  =  uvoffset;
	XSpace  =  space;

	s32 diffOffest  =  RoundOff((Offset - PrevOffset) * 10000.f);

	if( diffOffest != 0 ) {
		for( u32 i  =  0 ; i < pl.size(); i++ )
			pl[i]->FrameMng->SlideUV(diffOffest/10000.f, 0.f);
	}


	switch( Pattern ){
	case 0:
	case 1:
	case 3:{
		for( u32 k  =  0 ; k < pl.size(); k++) {
			for( u32 i  =  0 ; i <= pl[k]->Segment(); i++)
				pl[k]->SetPoint(i, glm::vec3(sint[i*(1024 / pl[k]->Segment())]*20.f*zy, 0.f, i*i * zx));

			pl[k]->FrameMng->Frame->SetTranslation(Frame->Frame, glm::vec3(0,0,-k*space));
		}
		   }break;
	case 2:{
		for( u32 k  =  0 ; k < pl.size(); k++) {
			for( u32 i  =  0 ; i <= pl[k]->Segment(); i++)
				pl[k]->SetPoint(i, glm::vec3(0.f, 0.f, i*i * zx));
		}
		   }
	}

}

TMissileSmoke3D::TMissileSmoke3D( TOBJList * owner, s32 _x, s32 _y, s32 _segment ) : TOBJ(owner)
{
	X = _x;
	Y = _y;
	Fade = false;
	Alpha = 255;
	FKind  =  kEffect2;
	PointAvailable = false;
	Segment = _segment;

	PosList.resize(Segment);
	VelocityList.resize(Segment);

	FirstID = 0;

	//親フレーム生成
	Frame  =  new TFrameManager(FOwner->GetScene(),NULL,NULL,NULL,sxbDefault,false,false,false);

	UVRect uv;
	uv.u1  =  0.f;
	uv.u2  =  2.f;
	uv.v1  =  0.f;
	uv.v2  =  1.f;

	pl = new TPolyLine(FOwner->GetScene(), Frame, Segment-1, 0.8f, Eff6Tex, uv, sxbAlpha);

	pl->FrameMng->Frame->SetTranslation(FOwner->GetScene()->Root, glm::vec3(0,0,0));
	Frame->Frame->SetVisibility(sxvHide);

}

TMissileSmoke3D::~TMissileSmoke3D()
{
	SAFE_DELETE(pl);

	SAFE_DELETE( Frame );

}

void TMissileSmoke3D::Move()
{
	TOBJ::Move();
	if( ! PointAvailable ) return;

	s32 lastID = FirstID-1;
	if( lastID<0 ) lastID = Segment-1;


	PosList[lastID]  =  CurrentPos;
	VelocityList[lastID]  =  CurrentVel;
	s32 id = FirstID;

	for( u32 i  =  0 ; i < Segment; i++ ) {
		if( (Age % 3 == 0) || (i == Segment-1) ) {
			pl->SetPoint(i, PosList[id]);
		}
		id  =  (id + 1) % Segment;
	}

	if( (Age % 3 == 0) ) FirstID  =  (FirstID + 1) % (Segment);

	pl->FrameMng->SlideUV(0.04f,0.f);

	if( Fade ) {
		pl->FrameMng->SetColor(Alpha, 255,255,255);
		Dec(Alpha,4);
		if( Alpha<=0 ) Die();
	}


}

s32 TMissileSmoke3D::OnDraw()
{
	if( PointAvailable )
		pl->CalcVartices();

	return pl->Segment();
}

void TMissileSmoke3D::SetPoint( const glm::vec3& _vec, const glm::vec3& _vel )
{
	CurrentPos  =  _vec;
	CurrentVel  =  _vel;

	if( (! PointAvailable) ) {
		PointAvailable = true;
		Frame->Frame->SetVisibility(sxvShow);
	}
}

void TMissileSmoke3D::FadeStart()
{
	Fade = true;

}

void TMissileSmoke3D::InitPos( s32 _x, s32 _y )
{
	CurrentPos = FOwner->GetScene()->Get3DPosFrom2D(X,Y);
	CurrentVel = glm::vec3(0,0,0);

	for( u32 i  =  0 ; i < Segment; i++ ) {
		PosList[i]  =  CurrentPos;
		VelocityList[i] = glm::vec3(0,0,0);
	}

}

void TMissile3D::Chase()
{
	glm::vec2 Target(FOwner->HeroPos.X + cost[Age*Rspeed & 4095] * rad, FOwner->HeroPos.Y+sint[Age*Rspeed & 4095] * rad);

	//    Velocity  =  NowAdd2D(Velocity, glm::vec2(-1,0));
	glm::vec2 Dest;
	glm::vec3 Dest3D;
	//規定の距離に達するまでは追い続ける
	if( FOwner->Distance <= EndDistance ) {
		glm::vec2 Diff  =  Target - PrevPos;

		//最初は勢いよく登場させるためにホーミング性能高めで発射し、
		//一定時間たったらゆっくりにする
		if( Age == 5  ) {
			switch( FOwner->Difficulty ){
			case KDIFFICULTY_SINGLE:
			case KDIFFICULTY_EASY:
				Homing = Homing/10;
				break;
			case KDIFFICULTY_NORMAL:
			case KDIFFICULTY_HARD:
				Homing = Homing/5;
				break;
			}
		}

		Velocity  =  glm::lerp(Velocity, Diff, Homing);

		//攻撃力発生
		if( FOwner->IsSequence() )
			if( Age == 60 ) 
				FSTR = 1000;
			else if( Age == 30 )
				FSTR = 1000;


		if( Age>30 ) {
			if( Velocity.length() > 5 )
				Velocity = glm::normalize(Velocity) * 5.f;  //速度を一定に保つ
		}

		Dest  =  PrevPos + Velocity;

		Dest3D = FOwner->GetScene()->Get3DPosFrom2D(Dest.x, Dest.y);

		Velocity3D  =  Dest3D - PrevPos3D;
	}
	else {
		FOwner->Add(new TEnergyBallBurst(FOwner,X,Y,4,120,3));
		Die();
		if( CreateSE ) FOwner->SoundEffect->WavPlay(WABeam,9);
	}


	smoke->SetPoint(Dest3D, Velocity3D);

	//ミサイルのメッシュを置く
	Frame->Move(PrevPos3D.x, PrevPos3D.y, PrevPos3D.z);

	//あたり判定スプライトを置く
	Sprite[0]->Move(Dest.x, Dest.y);
	X = RoundOff(Dest.x);
	Y = RoundOff(Dest.y);

	//ミサイルの向き
	Frame->Frame->LookAt(FOwner->GetScene()->Root, Dest3D + Velocity3D, FOwner->GetScene()->Root, Age*32 );

	PrevPos  =  Dest;
	PrevPos3D  =  Dest3D;

}

void TMissile3D::DummyMove()
{
	glm::vec3 Target  =  FOwner->GetScene()->Get3DPosFrom2D(320 + cost[(Age*Rspeed+3072) & 4095] * rad,240+sint[(Age*Rspeed+3072) & 4095] * rad);
	f32 Boost = FOwner->Speed / (f32)KSCROLLSPEED_NORMAL;

	//規定の距離に達するまでは追い続ける
	f32 OriginSpeed;
	glm::vec3 Dest;
	glm::vec3 Diff;
	if( FOwner->Distance <= EndDistance ) {
		Diff  =  Target - PrevPos3D;
		if( Age>1 )
			OriginSpeed  =  Velocity3D.length();      //初速
		else
			OriginSpeed  =  Diff.length() / 10.f;

		Velocity3D  =  glm::lerp(Velocity3D, Diff, Homing);

		if( Boost<1.1f )
			Velocity3D = glm::normalize(Velocity3D) * OriginSpeed;  //速度を一定に保つ
		Dest  =  PrevPos3D + Velocity3D;
	}
	else {
		//追いかけ疲れたら画面外へ逃げる

		if( Velocity3D.length() < 1.f )
			Velocity3D = Velocity3D * 1.05f;
		Dest = PrevPos3D + Velocity3D;
	}


	smoke->SetPoint(Dest,Velocity3D);

	//ミサイルのメッシュを置く
	Frame->Move(PrevPos3D.x, PrevPos3D.y, PrevPos3D.z);

	//ミサイルの向き
	Frame->Frame->LookAt(FOwner->GetScene()->Root, Dest + Velocity3D,FOwner->GetScene()->Root, Age*32 );

	PrevPos3D  =  Dest;

}

TMissile3D::TMissile3D( TOBJList* owner, TMissileSmoke3D* _smoke, s32 _x, s32 _y, f32 _homingLV, s32 _pat, s32 _distance ) : TOBJ(owner)
{
	X = _x;
	Y = _y;
	FKind  =  kEffect2;
	switch( FOwner->Difficulty ){
	case KDIFFICULTY_EASY:
		MaxHP = KSTR_SHOT*3;
		break;
	default:
		MaxHP = KSTR_SHOT*6;
	}
	HP = MaxHP;
	TempHP = HP;
	FSTR = 0;
	Score = 10000;
	Buzzed = false;
	LastBuzz = 0;
	Pattern  =  _pat;
	if( FOwner->ReplayON && (FOwner->Replayer->ReadRepVer == 29) ) {
		rad  =  FOwner->GetRand(100)+50;
		Rspeed = FOwner->GetRand(32)+16;
	}
	else {
		if( Pattern == 0 ) {
			rad  =  FOwner->GetRand(100)+50;
			Rspeed = FOwner->GetRand(32)+16;
		}
		else {
			rad  =  random(100)+50;
			Rspeed = random(32)+16;
		}
	}

	smoke = _smoke;

	Velocity  =  glm::vec2(0.f, 0.f);
	Velocity3D  =  glm::vec3(0.f,0.f,0.f);
	Rdeg = 0;
	Homing =  _homingLV;
	StartDistance  =  FOwner->Distance;
	EndDistance  =  _distance;             //ホーミング終了距離
	AliveDistance  =  (random(1)+1)*5*60;  //ホーミング終了から自爆までの距離。1～2秒

	//IDの付与
	FID = NewID;
	Inc(NewID);

	if( Pattern == 0 ) {
		FKind  =  kEnemy;
		//あたり判定用透明スプライト
		MakeSpriteLite(0, _x,_y,48,48,2000,bmNormal);
		Sprite[0]->GetSpr()->AddTexture(MechaTex, 192, 48, 239, 95);   // テクスチャを登録
		//      AddTexture(JikiTex, 1, 1, 46, 79);

		Sprite[0]->GetSpr()->FCX += 24;
		Sprite[0]->GetSpr()->FCY += 24;
		Sprite[0]->GetSpr()->AddHitArea(TPoint(0, 0), TPoint(0, 48), TPoint(48, 48),TPoint(48, 0), htSky);
		Sprite[0]->GetSpr()->FHit      =  true;
		Sprite[0]->GetSpr()->FVisible = true;
		Sprite[0]->GetSpr()->Move(X,Y);
		Sprite[0]->GetSpr()->Rotate(512);
	}

	//親フレーム生成
	Frame  =  new TFrameManager(FOwner->GetScene(),NULL,MissileMesh,MissileTex,sxbDefault,true,false,false);

	FOwner->Add(smoke);

	PrevPos  =  glm::vec2(X,Y);
	PrevPos3D  =  FOwner->GetScene()->Get3DPosFrom2D(X,Y);

	smoke->InitPos(X,Y);

	if( CreateSE ) FOwner->SoundEffect->WavPlay(WAMissile,4);

}

TMissile3D::~TMissile3D()
{
	SAFE_DELETE( Frame );
	smoke->FadeStart();

}

void TMissile3D::Move()
{
	TOBJ::Move();

	switch( Pattern ){
	case 0:{
		Chase();
		   }break;
	case 1:{
		DummyMove();
		   }break;
	}

	if( (Age-LastBuzz)>=40 ) Buzzed = false;
	if( numSpr>0 )
		if( (Sprite[0]->GetSpr()->FVisible) && (HP<=0) ) OnBroken();                        //HPが無くなったら死ぬ

	if( FOwner->Distance < StartDistance ) Die();    //巻き戻し時は消す
	if( FOwner->Distance >= (EndDistance+AliveDistance) ) {
		Die();
	}

}

void TMissile3D::Die()
{
	FOwner->Add(new TFireBall(FOwner, PrevPos3D));

}

void TMissile3D::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);
	if( target->Kind() == kHero ) {
		if( (! Buzzed) && (target->Hit[HitBuzz]) ) {
			FOwner->Scrach(this);
			Buzzed = true;
			LastBuzz = Age;
		}
		if( (target->Hit[HitBlade]) && (target->LastSlashed > LastSlashed) ) {
			Dec(HP,target->STR());
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WAEdamage0,7);
			LastSlashed = target->LastSlashed;
		}

		if( (target->Hit[HitBody]) && (FSTR>0) ) Die();

	}

	if( (target->Kind() == kHShot) || (target->Kind() == kMissile) ) {
		Dec(HP,target->STR());
		if( CreateSE ) FOwner->SoundEffect->WavPlay(WAEdamage0,7);
		if( HP<=0 ) BonusRate = target->BonusRate;
	}

	if( HP<TempHP ) TempHP = HP;

}

void TMissile3D::OnBroken()
{
	TOBJ::OnBroken();
	FOwner->Add(new TExpmiddle(FOwner,X,Y,0,0,0));

}

s32 TMissile3D::OnDraw()
{
	UVRect UV;
	//バックファイア
	if( ! FDead ) {
		UV.u1  =  0;
		UV.v1  =  192/256.f;
		UV.u2  =  64/256.f;
		UV.v2  =  1;

		FOwner->GetScene()->SetBillBoardSX2(FOwner->GetScene()->Root,Frame->GetX(), Frame->GetY(), Frame->GetZ(),
			1,
			1,
			UV,
			255,
			Eff1Tex,
			sxbAdd,
			true,
			true);
	}

	//スプライトを登録
	if( Sprite[0]!=NULL ) {
		FOwner->GetSprMan()->AddSprite(Sprite[0]->GetSpr());
	}

	return 1;
}

TBattleShip3::TBattleShip3( TOBJList* owner, TSXMesh* _mesh, TDGTexture* _tex, MaterialData* _mat, s32 _appear, s32 _stop, f32 _speed ) : TOBJ(owner)
{
	AppearDistance = _appear;
	StopDistance = _stop;
	Speed = _speed;
	FKind  =  kEffect2;
	//親フレーム生成
	Frame  =  new TFrameManager(FOwner->GetScene(),NULL,NULL,NULL,sxbDefault,false,false,false);
	ShipFrame  =  new TFrameManager(FOwner->GetScene(),Frame,_mesh,_tex,sxbDefault,true,true,false);
	ShipFrame->Frame->SetVisibility(sxvHide);
	ShipFrame->Zoom(4.f,4.f,4.f);

	//マテリアル
	D3DMATERIAL9 matdata;
	matdata.Diffuse.r  =  _mat->Diffuse;
	matdata.Diffuse.g  =  _mat->Diffuse;
	matdata.Diffuse.b  =  _mat->Diffuse;

	matdata.Specular.r  =  _mat->Specular;
	matdata.Specular.g  =  _mat->Specular;
	matdata.Specular.b  =  _mat->Specular;

	matdata.Ambient.r  =  _mat->Ambient;
	matdata.Ambient.g  =  _mat->Ambient;
	matdata.Ambient.b  =  _mat->Ambient;

	matdata.Emissive.r  =  _mat->Emissive;
	matdata.Emissive.g  =  _mat->Emissive;
	matdata.Emissive.b  =  _mat->Emissive;

	matdata.Power  =  _mat->Power;

	ShipFrame->Frame->SetMaterialRcv(matdata);
}

TBattleShip3::~TBattleShip3()
{
	SAFE_DELETE(Frame);
}

void TBattleShip3::Move()
{
	TOBJ::Move();
	if( (FOwner->Distance>=AppearDistance) && (ShipFrame->Frame->GetVisibility() == sxvHide) )
		ShipFrame->Frame->SetVisibility(sxvShow);

	if( (FOwner->Distance<AppearDistance) && (ShipFrame->Frame->GetVisibility() == sxvShow) )
		ShipFrame->Frame->SetVisibility(sxvHide);

	if( (FOwner->Distance>=AppearDistance) && (FOwner->Distance<=(AppearDistance + StopDistance)) ) {
		s32 DiffDistance  =  FOwner->Distance - AppearDistance;
		f32 FinalSpeed  =  glm::lerp(Speed, 0.f, (f32)DiffDistance / (f32)StopDistance);

		f32 FinalPos  =  (Speed + FinalSpeed) * DiffDistance / 2.f;

		ShipFrame->Frame->SetTranslation(Frame->Frame, glm::vec3(0.f, 0.f,FinalPos));
	}

}

void TBattleShip3::SetParam( f32 _appear, f32 _stop, f32 _speed )
{
	AppearDistance = RoundOff(_appear);
	StopDistance = RoundOff(_stop);
	Speed = _speed;

}

TBoardControl::TBoardControl( TOBJList* owner, TDGTexture* _tex, UVRect& _uv, f32 _appear, f32 _disappear, f32 _rot, f32 _alpha, f32 _alphaspeed, f32 _zoomrate ) : TOBJ(owner)
{
	AppearDistance = RoundOff(_appear);
	DisappearDistance = RoundOff(_disappear);
	RotSpeed  =  RoundOff(_rot);
	Alpha  =  RoundOff(_alpha);
	AlphaSpeed  =  RoundOff(_alphaspeed);
	ZoomRate  =  _zoomrate;
	FKind  =  kEffect2;

	PrevR = RotSpeed;
	PrevZoom = ZoomRate;

	Board  =  new TBoard(_uv.u1, _uv.v1, _uv.u2, _uv.v2);

	//親フレーム生成
	Frame  =  new TFrameManager(FOwner->GetScene(),NULL,Board->Mesh,_tex,sxbAdd,false,false,false);
	Frame->Frame->SetVisibility(sxvHide);

}

TBoardControl::~TBoardControl()
{
	SAFE_DELETE(Frame);
	SAFE_DELETE(Board);
}

void TBoardControl::Move()
{
	TOBJ::Move();
	if( (FOwner->Distance>=AppearDistance) && (FOwner->Distance<DisappearDistance) && (Frame->Frame->GetVisibility() == sxvHide) )
		Frame->Frame->SetVisibility(sxvShow);

	if( ((FOwner->Distance<AppearDistance) || (FOwner->Distance>=DisappearDistance)) && (Frame->Frame->GetVisibility() == sxvShow) )
		Frame->Frame->SetVisibility(sxvHide);

	s32 DiffDistance  =  RoundOff((FOwner->Distance - AppearDistance)/5.f);
	if( DiffDistance<0 ) DiffDistance = 0;


	if( DiffDistance>=0 ) {
		s32 FinalAlpha  =  Alpha + AlphaSpeed * DiffDistance;
		if( FinalAlpha>255 ) FinalAlpha = 255;
		if( FinalAlpha<0 ) FinalAlpha = 0;

		Frame->SetColor(FinalAlpha, 255,255,255);

		s32 FinalRot = RotSpeed * DiffDistance;

		Frame->RotateR(0,0,-PrevR);
		Frame->RotateR(0,0,FinalRot);
		PrevR  =  FinalRot;
		f32 FinalZoom  =  pow(ZoomRate, DiffDistance);       //実機では重過ぎるので変える（TBD）
		Frame->ZoomR(1/PrevZoom, 1/PrevZoom,1/PrevZoom);
		Frame->ZoomR(FinalZoom, FinalZoom,FinalZoom);
		PrevZoom  =  FinalZoom;
	}


}

void TBoardControl::SetParam( f32 _appear, f32 _disappear, f32 _rot, f32 _alphaspeed, f32 _zoomrate )
{
	AppearDistance = RoundOff(_appear);
	DisappearDistance = RoundOff(_disappear);
	RotSpeed  =  RoundOff(_rot);
	AlphaSpeed  =  RoundOff(_alphaspeed);
	ZoomRate =  _zoomrate;
}

TMissile3DGen::TMissile3DGen( TOBJList* owner, f32 _pat, f32 _start, f32 _end, f32 _homingLV, f32 _x, f32 _y ) : TOBJ(owner)
{
	X = RoundOff(_x);
	Y = RoundOff(_y);
	Homing = _homingLV;
	StartDistance = RoundOff(_start);
	EndDistance = RoundOff(_end);
	Launched = false;
	Pattern = RoundOff(_pat);
	FKind  =  kEffect2;

	s32 Seg;
	if( Pattern == 2 ) {
		Pattern = 1;
		Seg = 128;
	}
	else
		Seg = 48;

	Frame  =  new TFrameManager(FOwner->GetScene(),NULL,NULL,NULL,sxbDefault,false,false,false);

	smoke.resize(2);
	smoke[0] = new TMissileSmoke3D(FOwner, X, Y,Seg);
	smoke[1] = new TMissileSmoke3D(FOwner, X, Y,Seg);

}

TMissile3DGen::~TMissile3DGen()
{
	SAFE_DELETE(Frame);

	//ミサイルを発射しないうちはSmokeはこいつの所有物なので責任もって消す。
	//ミサイル発車後はSmokeは自分で消えるかリセット時にFOwnerが消してくれる。
	if( ! Launched ) {
		for( u32 i  =  0 ; i < smoke.size(); i++ )
			smoke[i]->Die();
	}

}

void TMissile3DGen::Move()
{
	TOBJ::Move();
	if( (FOwner->Distance >= StartDistance) && (Launched == false) ) {
		FOwner->Add(new TMissile3D(FOwner,smoke[0], X, Y, Homing, Pattern, EndDistance));

		if( (Pattern == 0) && (FOwner->Difficulty == KDIFFICULTY_HARD) )
			FOwner->Add(new TMissile3D(FOwner,smoke[1], X+700, Y, Homing, Pattern, EndDistance));

		if( (Pattern = 1) && (EffLevel=2) )
			FOwner->Add(new TMissile3D(FOwner,smoke[1], X+1400, Y, Homing/2, Pattern, EndDistance));

		Launched  =  true;
	}

	if( FOwner->Distance < StartDistance ) Launched = false;

}

void TMissile3DGen::SetParam( f32 _start, f32 _end, f32 _homingLV, f32 _x, f32 _y )
{
	StartDistance = RoundOff(_start);
	EndDistance = RoundOff(_end);
	X  =  RoundOff(_x);
	Y  =  RoundOff(_y);
	Homing  =  _homingLV;

}

TBattleShipSmoke::TBattleShipSmoke( TOBJList* owner, TDGTexture* _tex, f32 _speed, f32 _zoom ) : TOBJ(owner)
{
	UVSlideSpeed  =  _speed;
	FKind  =  kEffect2;
	//親フレーム生成
	Frame  =  new TFrameManager(FOwner->GetScene(),NULL,NULL,NULL,sxbDefault,false,false,false);

	UVRect uv;
	uv.u1  =  0;
	uv.u2  =  2.0f;
	uv.v1  =  1;
	uv.v2  =  0;

	pl = new TPolyLine(FOwner->GetScene(), Frame, 10, 1, Eff6Tex,uv, sxbAlpha);

	SetParam(UVSlideSpeed, 1,1,_zoom);
	pl->FrameMng->SetColor(255,64,64,64);

}

TBattleShipSmoke::~TBattleShipSmoke()
{
	SAFE_DELETE( pl );

	SAFE_DELETE(Frame);

}

void TBattleShipSmoke::Move()
{
	TOBJ::Move();
	if( Frame->Frame->GetVisibility() == sxvShow ) {
		pl->FrameMng->SlideUV(UVSlideSpeed,0.f);
	}

}

s32 TBattleShipSmoke::OnDraw()
{
	if( Frame->Frame->GetVisibility() == sxvShow ) {
		pl->CalcVartices();
	}
	return pl->Segment();

}

void TBattleShipSmoke::SetParam( f32 speed, f32 zx, f32 zy, f32 _zoomrate )
{
	UVSlideSpeed  =  speed;

	for( u32 i  =  0 ; i < pl->Segment(); i++ )
		pl->SetPoint(i, glm::vec3(0.f, 0.f, i * zx));

	pl->SetZoomrate(_zoomrate);

}

TFire3D::TFire3D( TOBJList* owner, TFrameManager* _parent, TDGTexture* _tex, UVRect& _uv, glm::vec3& _pos, f32 _speed, f32 _zoom, f32 _zoomrate, f32 _alpha ) : TOBJ(owner)
{
	Speed = _speed;
	ZoomRate = _zoomrate;
	Zoom = _zoom;
	AlphaSpeed = RoundOff(_alpha);
	Rdeg = random(4095);
	RSpeed = random(64)-32;
	Parent = _parent;
	Texture = _tex;
	UV  =  _uv;
	Pos = _pos;
	Alpha = 255;
	FKind  =  kEffect2;
}

TFire3D::~TFire3D()
{

}

void TFire3D::Move()
{
	TOBJ::Move();
	Pos  =  Pos + glm::vec3(Speed, 0.f, 0.f);
	Inc(Rdeg, RSpeed);
	Zoom  =  Zoom*ZoomRate;
	Inc(Alpha, AlphaSpeed);
	if( Alpha<0 ) Die();

	if( Parent->Frame->GetVisibility() == sxvHide ) Die();

}

s32 TFire3D::OnDraw()
{
	if( FDead ) {
		return 0;
	}

	s32 width  =  (s32)((UV.u2 - UV.u1) * Texture->GetWidth());
	s32 height  =  (s32)((UV.v2 - UV.v1) * Texture->GetHeight());

	FOwner->GetScene()->SetBillBoardSX2(Parent->Frame, Pos.x, Pos.y, 0,
		RoundOff(width * Zoom),
		RoundOff(height * Zoom),
		UV,
		Alpha,
		Texture,
		sxbAdd,
		true,
		false);

	return 1;

}

TBattleShipFire::TBattleShipFire( TOBJList* owner, TDGTexture* _tex, f32 _speed, f32 _zoom, f32 _alpha, f32 _spread ) : TOBJ(owner)
{
	Speed = _speed;
	ZoomRate = _zoom;
	AlphaSpeed = RoundOff(_alpha);
	Spread  =  _spread;
	Texture = _tex;
	FKind  =  kEffect2;

	Frame  =  new TFrameManager(FOwner->GetScene(),NULL,NULL,NULL,sxbDefault,false,false,false);
}

TBattleShipFire::~TBattleShipFire()
{
	SAFE_DELETE(Frame);
}

void TBattleShipFire::Move()
{
	TOBJ::Move();
	if( (Age % 2 == 0) && (Frame->Frame->GetVisibility() == sxvShow) ) {
		UVRect UV;

		UV.u1  =  0.5f;
		UV.v1  =  0;
		UV.u2  =  1.0f;
		UV.v2  =  0.5f;

		f32 rnd  =  random(RoundOff(Spread*10.f))/10.f;
		glm::vec3 Pos(rnd,rnd,0);

		FOwner->Add(new TFire3D(FOwner, Frame,Texture,UV, Pos, Speed, Frame->GetZoomX(), ZoomRate, AlphaSpeed));
	}

}

void TBattleShipFire::SetParam( f32 _speed, f32 _zoom, f32 _alpha, f32 _spread )
{
	Speed = _speed;
	ZoomRate = _zoom;
	AlphaSpeed = RoundOff(_alpha);
	Spread  =  _spread;

}

void TBattleShipFire::Die()
{
	TOBJ::Die();
	Frame->Frame->SetVisibility(sxvHide);

}

TStraightBeam::TStraightBeam( TOBJList* owner, TDGTexture* _tex, s32 _pat, f32 _start, f32 _speed, f32 _life ) : TOBJ(owner)
{
	Speed  =  _speed;
	Life = RoundOff(_life);
	StartDistance = RoundOff(_start);
	FKind  =  kEffect2;
	Frame  =  new TFrameManager(FOwner->GetScene(),NULL,NULL,NULL,sxbDefault,false,false,false);

	UVRect uv;
	if( _pat == 0 ) {
		uv.u1  =  0;
		uv.u2  =  0.5f;
		uv.v1  =  0;
		uv.v2  =  0.25f;
	}
	else {
		uv.u1  =  0;
		uv.u2  =  0.5f;
		uv.v1  =  0.25f;
		uv.v2  =  0.5f;
	}

	pl = new TPolyLine(FOwner->GetScene(), Frame, 1, 1, _tex,uv, sxbAdd);

	for( u32 i  =  0 ; i < pl->Segment(); i++ )
		pl->SetPoint(i, glm::vec3(i*5.f, 0.f, 0.f));

	pl->FrameMng->Frame->SetTranslation(Frame->Frame, glm::vec3(0,0,0));
}

TStraightBeam::~TStraightBeam()
{
	SAFE_DELETE(pl);

	SAFE_DELETE(Frame);

}

void TStraightBeam::Move()
{
	TOBJ::Move();
	s32 DiffDistance  = (FOwner->Distance - StartDistance);

	if( (DiffDistance>=0) && (DiffDistance < Life) ) {
		if( Frame->Frame->GetVisibility() == sxvHide )
			Frame->Frame->SetVisibility(sxvShow);

		f32 FinalPos = DiffDistance/5.f * Speed;
		pl->SetPoint(1, glm::vec3(FinalPos*Frame->GetZoomX(),0.f,0.f));
		pl->SetWidth(Frame->GetZoomY());
	}
	else if( Frame->Frame->GetVisibility() == sxvShow )
		Frame->Frame->SetVisibility(sxvHide);


	if( DiffDistance >= Life  )
		Frame->Frame->SetVisibility(sxvHide);

}

void TStraightBeam::SetParam( f32 _start, f32 _speed, f32 _life )
{
	Speed  =  _speed;
	Life = RoundOff(_life);
	StartDistance = RoundOff(_start);

}

s32 TStraightBeam::OnDraw()
{
	if( Frame->Frame->GetVisibility() == sxvShow )
		pl->CalcVartices();

	return 1;
}

TFireBall::TFireBall( TOBJList* owner, const glm::vec3& _pos ) : TOBJ(owner)
{
	Alpha = 255;
	Pos = _pos;
	FKind  =  kEffect2;

}

void TFireBall::Move()
{
	TOBJ::Move();
	Dec(Alpha,2);

	if( Alpha<=0 ) Die();

}

s32 TFireBall::OnDraw()
{
	if( FDead ) {
		return 0;
	}

	//バックファイア
	UVRect UV;
	UV.u1  =  0;
	UV.v1  =  192/256.f;
	UV.u2  =  64/256.f;
	UV.v2  =  1;
	FOwner->GetScene()->SetBillBoardSX2(FOwner->GetScene()->Root, Pos.x, Pos.y, Pos.z,
		8,
		8,
		UV,
		Alpha,
		Eff1Tex,
		sxbAdd,
		true,
		true);
	return 1;
}

TLandscape::TLandscape( TOBJList* owner, TSXMesh* _mesh, TDGTexture* _tex, s32 pattern ) : TOBJ(owner)
{
	X = 2000;
	Y = 0;
	FSTR = 1000;
	FKind = kColBG;

	FCX = 0;
	FCY = 0;
	Width = 64;
	Height = 64;

	//    FKind  =  kEnemy;
	//あたり判定用透明スプライト
	MakeSpriteLite(0, 0,0,64,64,12000,bmNormal);
	Sprite[0]->GetSpr()->AddTexture(FontTex, 0, 0, 1, 1);   // テクスチャを登録

	FCX +=32;
	FCY +=32;
	switch( pattern ){
	case 0:Sprite[0]->GetSpr()->AddHitArea(TPoint(0, 0), TPoint(0, 64), TPoint(64, 64),TPoint(64, 0), htSky);			break;
	case 1:Sprite[0]->GetSpr()->AddHitArea(TPoint(0, 0), TPoint(0, 200), TPoint(96, 200),TPoint(96, 96), htSky);			break;
	case 2:Sprite[0]->GetSpr()->AddHitArea(TPoint(0, -200), TPoint(0, 80), TPoint(96, -16),TPoint(96, -200), htSky);		break;
	}

	Sprite[0]->GetSpr()->FHit      =  true;
	Sprite[0]->GetSpr()->FVisible = true;
	Sprite[0]->GetSpr()->Move(X,Y);

	Frame  =  new TFrameManager(FOwner->GetScene(),NULL,_mesh,_tex,sxbDefault,true,false,false);
}

TLandscape::~TLandscape()
{
	SAFE_DELETE(Frame);

}

void TLandscape::Move()
{
	TOBJ::Move();
	TPoint pos = FOwner->GetScene()->Get2DPosFrom3D(Frame->Frame->GetTranslation(FOwner->GetScene()->Root));

	Sprite[0]->Move(pos.X + FCX, pos.Y + FCY);
	Sprite[0]->Rotate(-Frame->GetRotZ());

	if( (pos.X < -Width) ) Die();

}

void TLandscape::SetParam( f32 _x, f32 _y, f32 _width, f32 _height )
{
	FCX = RoundOff(_x);
	FCY = RoundOff(_y);

	Sprite[0]->ZoomR(_width/Width, _height/Height);
	Width = RoundOff(_width);
	Height = RoundOff(_height);

}

void TLandscape::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);
	vector<THitArea> hitarea1, hitarea2;
	if( target->Kind() == kHero ) {
		target->Sprite[1]->GetSpr()->HitAreaTransform(hitarea1);
		Sprite[0]->GetSpr()->HitAreaTransform(hitarea2);
		glm::vec2 vec = GetReflectionVector(glm::vec2(hitarea1[0].RPointAry[0].x,
			hitarea1[0].RPointAry[0].y),
			&hitarea2[0]);

		target->CollisionReaction(vec.x, vec.y);
	}

}

TRotateMesh::TRotateMesh( TOBJList* owner, TSXMesh* _mesh, TDGTexture* _tex ) : TOBJ(owner)
{
	Frame  =  new TFrameManager(FOwner->GetScene(),NULL,_mesh,_tex,sxbDefault,true,true,false);

}

TRotateMesh::~TRotateMesh()
{
	SAFE_DELETE(Frame);

}

void TRotateMesh::Move()
{
	TOBJ::Move();
	Frame->RotateR(RX,RY,RZ);

}

void TRotateMesh::SetParam( f32 _rx, f32 _ry, f32 _rz )
{
	RX = RoundOff(_rx);
	RY = RoundOff(_ry);
	RZ = RoundOff(_rz);

}

TShutter::TShutter( TOBJList* owner, TSXMesh* _mesh, TDGTexture* _tex, f32 _start, f32 _speed, f32 _endy ) : TOBJ(owner)
{
	X = 800;
	Y = 0;
	FSTR = 1000;
	FKind = kColBG;
	FCX = 48;
	FCY = -814;
	Width = 96;
	Height = 480;
	Closing = false;
	YPos = 0;
	StartDistance = RoundOff(_start);
	Speed = _speed;
	EndY = _endy;

	//あたり判定用透明スプライト
	MakeSpriteLite(0, 0,0,64,64,12000,bmNormal);
	Sprite[0]->GetSpr()->AddTexture(FontTex, 0, 0, 1, 1);   // テクスチャを登録

	Sprite[0]->GetSpr()->FCX +=32;
	Sprite[0]->GetSpr()->FCY +=32;
	Sprite[0]->GetSpr()->AddHitArea(TPoint(0, 0), TPoint(0, 64), TPoint(64, 64), TPoint(64, 0), htSky);
	Sprite[0]->GetSpr()->FHit =  false;          //通し面では当たり判定無し
	Sprite[0]->GetSpr()->FVisible = true;
	Sprite[0]->GetSpr()->Move(X,Y);
	Sprite[0]->GetSpr()->ZoomR(Width/64, Height/64);

	Frame  =  new TFrameManager(FOwner->GetScene(),NULL,_mesh,_tex,sxbDefault,true,false,false);
}

TShutter::~TShutter()
{
	SAFE_DELETE(Frame);
}

void TShutter::Move()
{
	TOBJ::Move();
	if( (FOwner->Distance >= 35000)  ) {
		if( (FOwner->Distance >= StartDistance) ) {
			if( (! Closing) ) {
				Closing = true;
				if( CreateSE ) FOwner->SoundEffect->WavPlay(WAShutter_close, 11);
			}
			else {
				if( (YPos>EndY) ) {
					Frame->MoveR(0, Speed, 0);
					YPos = YPos+Speed;
				}
			}
		}
		else if( Closing ) {
			Closing = false;
			Frame->MoveR(0,-YPos,0);
			YPos = 0;
		}
	}

	TPoint pos = FOwner->GetScene()->Get2DPosFrom3D(Frame->Frame->GetTranslation(FOwner->GetScene()->Root) + glm::vec3(0.f, 0.f, -2.f));

	Sprite[0]->Move(pos.X + FCX, pos.Y + FCY);

	if( (FOwner->Distance>=(StartDistance+900)) ) Die();

}

void TShutter::SetParam( f32 _start, f32 _speed, f32 _endy )
{
	StartDistance = RoundOff(_start);
	Speed = _speed;
	EndY = _endy;

}

TElevator::TElevator( TOBJList* owner, TSXMesh* _mesh, TDGTexture* _tex, f32 _distance, f32 _speed, f32 _start ) : TOBJ(owner)
{
	X = 800;
	Y = 0;
	FSTR = 1000;
	FKind = kColBG;
	FCX = 0;
	FCY = 0;
	Width = 64;
	Height = 64;
	StartPos = _start;
	Speed = _speed;
	StartDistance = (s32)(_distance);

	//    FKind  =  kEnemy;
	//あたり判定用透明スプライト
	MakeSpriteLite(0, 0,0,64,64,12000,bmNormal);
	Sprite[0]->GetSpr()->AddTexture(FontTex, 0, 0, 1, 1);   // テクスチャを登録

	Sprite[0]->GetSpr()->FCX +=32;
	Sprite[0]->GetSpr()->FCY +=32;
	Sprite[0]->GetSpr()->AddHitArea(TPoint(0, 0), TPoint(0, 64), TPoint(64, 64),TPoint(64, 0), htSky);
	Sprite[0]->GetSpr()->FHit      =  true;
	Sprite[0]->GetSpr()->FVisible = true;
	Sprite[0]->GetSpr()->Move(X,Y);

	Frame  =  new TFrameManager(FOwner->GetScene(),NULL,_mesh,_tex,sxbDefault,true,false,false);

}

TElevator::~TElevator()
{
	SAFE_DELETE(Frame);

}

void TElevator::Move()
{
	TOBJ::Move();
	if( (FOwner->Distance>=StartDistance) ) {
		Frame->MoveR(0,Speed,0);
	}
	else {
		Frame->Move(Frame->GetX(), StartPos,Frame->GetZ());
	}

	TPoint pos = FOwner->GetScene()->Get2DPosFrom3D(Frame->Frame->GetTranslation(FOwner->GetScene()->Root));

	Sprite[0]->Move(pos.Y + FCX, pos.Y + FCY);

	if( (FOwner->Distance>=(StartDistance+900)) ) Die();

}

void TElevator::SetParam( f32 _x, f32 _y, f32 _width, f32 _height, f32 _distance, f32 _speed, f32 _start )
{
	StartPos = _start;
	Speed = _speed;
	StartDistance = (s32)(_distance);

	FCX = RoundOff(_x);
	FCY = RoundOff(_y);

	Sprite[0]->ZoomR(_width/Width, _height/Height);
	Width = RoundOff(_width);
	Height = RoundOff(_height);

}

TBuildingMove::TBuildingMove( TOBJList* owner, TSXMesh* _mesh, TDGTexture* _tex, MaterialData* _mat, f32 _farx, f32 _farz, f32 _movex, f32 _movez, f32 _start, f32 _end ) : TOBJ(owner)
{
	FKind  =  kEffect2;
	//マテリアル
	D3DMATERIAL9 matdata;
	matdata.Diffuse.r  =  _mat->Diffuse;
	matdata.Diffuse.g  =  _mat->Diffuse;
	matdata.Diffuse.b  =  _mat->Diffuse;

	matdata.Specular.r  =  _mat->Specular;
	matdata.Specular.g  =  _mat->Specular;
	matdata.Specular.b  =  _mat->Specular;

	matdata.Ambient.r  =  _mat->Ambient;
	matdata.Ambient.g  =  _mat->Ambient;
	matdata.Ambient.b  =  _mat->Ambient;

	matdata.Emissive.r  =  _mat->Emissive;
	matdata.Emissive.g  =  _mat->Emissive;
	matdata.Emissive.b  =  _mat->Emissive;

	matdata.Power  =  _mat->Power;

	FarX = _farx;
	FarZ = _farz;
	MoveX = _movex;
	MoveZ = _movez;
	StartDistance = (s32)(_start);
	EndDistance = (s32)(_end);

	Frame  =  new TFrameManager(FOwner->GetScene(),NULL,_mesh,_tex,sxbColorKey,true,true,false);
	Frame->Frame->SetParent( FOwner->GetScene()->FogFrame );

}

TBuildingMove::~TBuildingMove()
{
	SAFE_DELETE(Frame);
}

void TBuildingMove::Move()
{
	TOBJ::Move();

	if( (FOwner->Distance >= StartDistance) && (FOwner->Distance < EndDistance)) {
		f32 cx,cy,cz;
		FOwner->GetScene()->GetCameraPos(cx,cy,cz);
		if( FarX > 1 ) {
			while( cx>(Frame->GetX() + FarX) ) {
				Frame->MoveR(MoveX,0,0);
			}

			while( cx<(Frame->GetX() - FarX) ) {
				Frame->MoveR(-MoveX,0,0);
			}
		}

		if( FarZ>1 ) {
			while( cz>(Frame->GetZ() + FarZ) ) {
				Frame->MoveR(0,0,MoveZ);
			}

			while( cz<(Frame->GetZ() - FarZ) ) {
				Frame->MoveR(0,0,-MoveZ);
			}
		}
	}

}

void TBuildingMove::SetParam( f32 _farx, f32 _farz, f32 _movex, f32 _movez, f32 _start, f32 _end )
{
	Frame->Frame->SetParent( FOwner->GetScene()->FogFrame );
	FarX = _farx;
	FarZ = _farz;
	MoveX = _movex;
	MoveZ = _movez;
	StartDistance = (s32)(_start);
	EndDistance = (s32)(_end);
}

TBuildingSmoke::TBuildingSmoke( TOBJList* owner, f32 _farx, f32 _farz, f32 _movex, f32 _movez, f32 _start, f32 _end ) : TOBJ(owner)
{
	FKind  =  kEffect2;
	UVSlideSpeed  =  -0.01f;
	//親フレーム生成
	Frame  =  new TFrameManager(FOwner->GetScene(),NULL,NULL,NULL,sxbDefault,false,false,false);
	Frame->Frame->SetParent( FOwner->GetScene()->FogFrame );
	UVRect uv;
	uv.u1  =  0;
	uv.u2  =  2.0f;
	uv.v1  =  0;
	uv.v2  =  1.f;

	for( u32 i = 0; i <= 1; i++ ) {
		pl[i] = new TPolyLine(FOwner->GetScene(), Frame, 10, 1, Eff6Tex,uv, sxbAlpha);

		pl[i]->FrameMng->SetColor(255,0,0,0);

		for( u32 k  =  0 ; k <= pl[i]->Segment(); k++ )
			pl[i]->SetPoint(k, glm::vec3(0.f, 0.f, k));
	}

	pl[0]->SetZoomrate(1.08f);
	pl[1]->SetZoomrate(1.1f);

	FarX = _farx;
	FarZ = _farz;
	MoveX = _movex;
	MoveZ = _movez;
	StartDistance = (s32)(_start);
	EndDistance = (s32)(_end);
}

TBuildingSmoke::~TBuildingSmoke()
{
	for( u32 i = 0; i <= 1; i++ )
		SAFE_DELETE( pl[i] );

	SAFE_DELETE(Frame);

}

void TBuildingSmoke::Move()
{
	TOBJ::Move();

	f32 cx,cy,cz;

	pl[0]->FrameMng->SlideUV(UVSlideSpeed,0);
	pl[1]->FrameMng->SlideUV(UVSlideSpeed*0.7,0);

	if( (FOwner->Distance >= StartDistance) && (FOwner->Distance < EndDistance)) {
		FOwner->GetScene()->GetCameraPos(cx,cy,cz);
		if( FarX>1 ) {
			while( cx>(Frame->GetX()+FarX) ) {
				Frame->MoveR(MoveX, 0.f, 0.f);
			}

			while( cx<(Frame->GetX()-FarX) ) {
				Frame->MoveR(-MoveX, 0.f, 0.f);
			}
		}

		if( FarZ>1 ) {
			while( cz>(Frame->GetZ()+FarZ) ) {
				Frame->MoveR(0.f, 0.f, MoveZ);
			}

			while( cz<(Frame->GetZ()-FarZ) ) {
				Frame->MoveR(0,0,-MoveZ);
			}
		}
	}

	if( (FOwner->Distance>=39000) ) {
		Die();
	}

}

s32 TBuildingSmoke::OnDraw()
{
	for( u32 i = 0; i <= 1; i++ )
		pl[i]->CalcVartices();
	return pl[0]->Segment();
}

void TBuildingSmoke::SetParam( f32 _farx, f32 _farz, f32 _movex, f32 _movez, f32 _start, f32 _end )
{
	Frame->Frame->SetParent( FOwner->GetScene()->FogFrame );
	FarX = _farx;
	FarZ = _farz;
	MoveX = _movex;
	MoveZ = _movez;
	StartDistance = (s32)(_start);
	EndDistance = (s32)(_end);

}

TRotateCloud::TRotateCloud( TOBJList* owner, TSXMesh* _mesh, TDGTexture* _tex, f32 _uspeed, f32 _vspeed, f32 _start, f32 _end, f32 _deccel ) : TOBJ(owner)
{
	StartDistance = RoundOff(_start);
	USpeed = _uspeed;
	VSpeed = _vspeed;
	EndV = _end;
	Deccel = _deccel;

	VSpeed_org =  VSpeed;
	FKind  =  kEffect2;
	CurrentV = 0;

	Frame  =  new TFrameManager(FOwner->GetScene(),NULL,_mesh,_tex,sxbAlpha,false,false,false);

}

TRotateCloud::~TRotateCloud()
{
	if( (CurrentV>0)) {
		Frame->SlideUV(0,CurrentV);
	}
	SAFE_DELETE(Frame);
}

void TRotateCloud::Move()
{
	TOBJ::Move();
	Frame->SlideUV(USpeed,0);

	if( (FOwner->Distance >= StartDistance) && (CurrentV < EndV)) {
		Frame->SlideUV(0,-VSpeed);
		CurrentV = CurrentV+VSpeed;

		if( VSpeed>Deccel )
			VSpeed = VSpeed - Deccel;
	}

	if( (FOwner->Distance<StartDistance) && (CurrentV>0)) {
		Frame->SlideUV(0,CurrentV);
		CurrentV = 0;
		VSpeed =  VSpeed_org;
	}

}

void TRotateCloud::SetParam( f32 _uspeed, f32 _vspeed, f32 _start, f32 _end, f32 _deccel )
{
	StartDistance = RoundOff(_start);
	USpeed = _uspeed;
	VSpeed = _vspeed;
	EndV = _end;
	Deccel = _deccel;
	VSpeed_org =  VSpeed;

}

TStraightBeamControl::TStraightBeamControl( TOBJList* owner, TDGTexture* _tex, s32 _pat ) : TOBJ(owner)
{
	Speed  =  1;
	Life = 30;
	StartDistance = 100000;
	EndDistance = StartDistance + 10000;
	Width = 30;
	Height = 30;
	Interval = 500;
	NextDistance =  StartDistance;
	CurrentBeamNum = 0;
	FKind  =  kEffect2;

	Frame  =  new TFrameManager(FOwner->GetScene(),NULL,NULL,NULL,sxbDefault,false,false,false);

	for( u32 i  =  0 ; i < 8 ; i++ ) {
		TStraightBeam* newbeam = new TStraightBeam(FOwner, _tex, _pat, StartDistance, Speed, Life);
		FOwner->Add(newbeam);
		newbeam->pl->FrameMng->Frame->SetParent( Frame->Frame );

		beam.push_back(newbeam);
	}

}

TStraightBeamControl::~TStraightBeamControl()
{
	for( u32 i  =  0 ; i < beam.size(); i++ ) {
		beam[i]->Die();
	}

	Frame->Frame->ClearChildren();
	SAFE_DELETE(Frame);

}

void TStraightBeamControl::Move()
{
	TOBJ::Move();
	s32 DiffDistance  = (FOwner->Distance - StartDistance);

	if( DiffDistance>=0 ) {
		if( Frame->Frame->GetVisibility() == sxvHide )
			Frame->Frame->SetVisibility(sxvShow);

		if( (FOwner->Distance>=NextDistance) ) {
			beam[CurrentBeamNum]->SetParam(NextDistance, Speed, Life);
			f32 randX =  (random(RoundOff(Width*100)) - Width*100/2) / 100.f;
			f32 randY =  (random(RoundOff(Height*100)) - Height*100/2) / 100.f;
			beam[CurrentBeamNum]->pl->FrameMng->Move(Frame->GetX() + randX, Frame->GetY() + randY, Frame->GetZ());


			Inc(NextDistance, Interval + (s32)random(RoundOff(Interval * 0.3f)));
			Inc(CurrentBeamNum);
			if( CurrentBeamNum>=beam.size() ) CurrentBeamNum = 0;

		}
	}
	else if( Frame->Frame->GetVisibility() == sxvShow )
		Frame->Frame->SetVisibility(sxvHide);

	if( FOwner->Distance>=EndDistance  )
		Frame->Frame->SetVisibility(sxvHide);

}

void TStraightBeamControl::SetParam( f32 _start, f32 _end, f32 _speed, f32 _life, f32 _width, f32 _height, f32 _interval )
{
	Speed  =  _speed;
	Life = RoundOff(_life);
	StartDistance = RoundOff(_start);
	EndDistance = RoundOff(_end);
	Width = _width;
	Height = _height;
	Interval = RoundOff(_interval);

	NextDistance =  StartDistance;

	for( u32 i = 0 ; i < beam.size(); i++ )
		beam[i]->Frame->Zoom(Frame->GetZoomX(), Frame->GetZoomY(), Frame->GetZoomY());

}

void TButtleShipHomingLaser::Homing()
{
	f32 Boost = FOwner->Speed / 5.f;

	glm::vec3 Diff  =  Target - PrevPos3D;
	if( Age>1 ) {
		f32 OriginSpeed  =  Velocity3D.length();      //初速

		Velocity3D  =  glm::lerp(Velocity3D, Diff, HomingLevel);

		if( Boost<2 )
			Velocity3D = glm::normalize(Velocity3D) * OriginSpeed;  //速度を一定に保つ

	}
	glm::vec3 Dest  =  PrevPos3D + Velocity3D;

	SetPoint(Dest);

	PrevPos3D  =  Dest;
}

TButtleShipHomingLaser::TButtleShipHomingLaser( TOBJList* owner, TPolyLine* _pl, TFrameManager* _parent, TDGTexture* _tex, glm::vec3& _vec, glm::vec3& _target, f32 _homingLV, f32 _vel, f32 _r ) : TOBJ(owner)
{
	Fade = false;
	Alpha = 0;
	HomePos = _vec;
	Target = _target;
	Texture =  _tex;
	Velocity3D  =  glm::vec3(_vel,0,0);
	HomingLevel =  _homingLV;
	PrevPos3D = HomePos;
	FKind  =  kEffect2;
	pl = _pl;

	FirstID = 0;

	//親フレーム生成
	Frame  =  new TFrameManager(FOwner->GetScene(),NULL,NULL,NULL,sxbDefault,false,false,false);

	CurrentPos = FOwner->GetScene()->Get3DPosFrom2D(X,Y);

	for( u32 i  =  0 ; i < pl->Segment(); i++ ) {
		PosList.push_back( HomePos );
	}

	pl->FrameMng->Frame->SetTranslation(FOwner->GetScene()->Root, glm::vec3(0,0,0));
	pl->FrameMng->Frame->SetVisibility(sxvShow);

	Velocity3D  =  TransformVec3(Velocity3D, ExtractRotation(_parent->Frame->GetMatrix()));
	Velocity3D  =  TransformVec3(Velocity3D, RotZMatrix((s32)(_r) & 0xfff));
}

TButtleShipHomingLaser::~TButtleShipHomingLaser()
{
	SAFE_DELETE(Frame);
}

void TButtleShipHomingLaser::Move()
{
	Homing();

	if( (Age == 30) && CreateSE ) FOwner->SoundEffect->WavPlay(WABossExp, 8); 


	s32 lastID = FirstID-1;
	if( lastID<0 ) lastID = pl->Segment()-1;

	PosList[lastID]  =  CurrentPos;
	s32 id = FirstID;

	for( u32 i  =  0 ; i < pl->Segment(); i++ ) {
		if( (Age % 2 == 0) || (i == pl->Segment()-1) ) {
			pl->SetPoint(i, PosList[id]);
		}
		id  =  (id + 1) % pl->Segment();
	}

	if( (Age % 2 == 0) ) FirstID  =  (FirstID + 1) % (pl->Segment());

	if( Fade ) {
		pl->FrameMng->SetColor(Alpha, 255,255,255);
		Dec(Alpha,4);
		if( Alpha<=0 ) Die();
	}
	else {
		Inc(Alpha,4);
		if( Alpha>255 ) Alpha = 255;
	}

	if( Age>300 ) FadeStart();

	if( Age == 240 )
		FOwner->Add(new TBattleShipExplosion(FOwner, Texture, Target, 40));

	TOBJ::Move();
}

s32 TButtleShipHomingLaser::OnDraw()
{
	//根本エフェクト
	if( Frame->Frame->GetVisibility() == sxvShow ) {
		UVRect uv;
		uv.u1  =  0;
		uv.u2  =  0.5f;
		uv.v1  =  0.5f;
		uv.v2  =  0.75f;

		FOwner->GetScene()->SetBillBoardSX2(FOwner->GetScene()->Root,HomePos.x, HomePos.y, HomePos.z,
			4,
			4,
			uv,
			Alpha,
			Texture,
			sxbAdd,
			true,
			false);

	}
	pl->CalcVartices();
	return pl->Segment();

}

void TButtleShipHomingLaser::SetPoint( const glm::vec3& _vec )
{
	CurrentPos  =  _vec;

}

void TButtleShipHomingLaser::FadeStart()
{
	Fade = true;

}

THomingLaserGen::THomingLaserGen( TOBJList* owner, TDGTexture* _tex, f32 _start, f32 _homingLV, f32 _x, f32 _y, f32 _z, f32 _vel, f32 _xspace, f32 _zspace ) : TOBJ(owner)
{
	Target = glm::vec3(_x,_y,_z);
	Homing = _homingLV;
	StartDistance = RoundOff(_start);
	Launched = false;
	Texture = _tex;
	Velocity = _vel;
	XSpace = _xspace;
	ZSpace = _zspace;
	ActFlg = 0;
	ActCount = 0;
	FKind  =  kEffect2;

	Frame  =  new TFrameManager(FOwner->GetScene(),NULL,NULL,NULL,sxbDefault,false,false,false);

	UVRect uv;
	uv.u1  =  0;
	uv.u2  =  1.f;
	uv.v1  =  0.5f;
	uv.v2  =  0.75f;

	for( u32 i  =  0 ; i < HOMINGLASER_MAX; i++ ) {
		pl[i] = new TPolyLine(FOwner->GetScene(), NULL, KSEGMENT_MAX, 1, Texture, uv, sxbAdd);
		pl[i]->FrameMng->Frame->SetVisibility(sxvHide);
	}

}

THomingLaserGen::~THomingLaserGen()
{
	SAFE_DELETE(Frame);

	for( u32 i  =  0 ; i < HOMINGLASER_MAX; i++ )
		SAFE_DELETE( pl[i] );

}

void THomingLaserGen::Move()
{
	TOBJ::Move();
	if( (FOwner->Distance >= StartDistance) ) {
		if( (ActCount % 10 == 0) && (ActFlg < 9) ) {
			glm::vec3 pos1 =  Frame->Frame->GetTranslation(FOwner->GetScene()->Root);
			glm::vec3 pos2 =  Frame->Frame->GetTranslation(FOwner->GetScene()->Root);

			pos1  =  pos1 + TransformVec3(glm::vec3(XSpace,0,ActFlg * ZSpace), ExtractRotation(Frame->Frame->GetMatrix()));
			pos2  =  pos2 + TransformVec3(glm::vec3(-XSpace,0,ActFlg * ZSpace), ExtractRotation(Frame->Frame->GetMatrix()));
			glm::vec3 rantarget =   Target + glm::vec3(random(100),random(100),random(100));
			FOwner->Add(new TButtleShipHomingLaser(FOwner,pl[ActFlg*2  ], Frame,Texture,pos1, rantarget, Homing, Velocity, -800 + ActFlg*200));
			FOwner->Add(new TButtleShipHomingLaser(FOwner,pl[ActFlg*2+1], Frame,Texture,pos2, rantarget, Homing, -Velocity, 800 - ActFlg*200));
			Inc(ActFlg);
		}
		Inc(ActCount);
	}

	if( FOwner->Distance < StartDistance ) {
		ActFlg = 0;
		ActCount = 0;
	}

}

void THomingLaserGen::SetParam( f32 _start, f32 _homingLV, f32 _x, f32 _y, f32 _z, f32 _vel, f32 _xspace, f32 _zspace )
{
	StartDistance = RoundOff(_start);
	Target = glm::vec3(_x,_y,_z);
	Velocity = _vel;
	Homing  =  _homingLV;
	XSpace = _xspace;
	ZSpace = _zspace;

}

TBattleShipExplosion::TBattleShipExplosion( TOBJList* owner, TDGTexture* _tex, glm::vec3 _pos, f32 _size ) : TOBJ(owner)
{
	Position = _pos;
	Texture = _tex;
	Size = _size;
	Alpha = 255;
	FKind  =  kEffect2;

}

TBattleShipExplosion::~TBattleShipExplosion()
{

}

void TBattleShipExplosion::Move()
{
	TOBJ::Move();
	Dec(Alpha, 2);
	if( Alpha<=0 ) Die();

}

s32 TBattleShipExplosion::OnDraw()
{
	UVRect uv;
	uv.u1  =  0;
	uv.u2  =  0.25f;
	uv.v1  =  0.75f;
	uv.v2  =  1.f;

	FOwner->GetScene()->SetBillBoardSX2(FOwner->GetScene()->Root,Position.x, Position.y, Position.z,
		Size,
		Size,
		uv,
		Alpha,
		Texture,
		sxbAdd,
		true,
		false);

	return 1;
}

TRoopSlideMesh::TRoopSlideMesh( TOBJList* owner, TSXMesh* _mesh, MaterialData* _mat, TDGTexture* _tex, TSXBlendMode _blend ) : TOBJ(owner)
{
	FKind  =  kEffect2;
	//マテリアル
	D3DMATERIAL9 matdata;
	matdata.Diffuse.r  =  _mat->Diffuse;
	matdata.Diffuse.g  =  _mat->Diffuse;
	matdata.Diffuse.b  =  _mat->Diffuse;

	matdata.Specular.r  =  _mat->Specular;
	matdata.Specular.g  =  _mat->Specular;
	matdata.Specular.b  =  _mat->Specular;

	matdata.Ambient.r  =  _mat->Ambient;
	matdata.Ambient.g  =  _mat->Ambient;
	matdata.Ambient.b  =  _mat->Ambient;

	matdata.Emissive.r  =  _mat->Emissive;
	matdata.Emissive.g  =  _mat->Emissive;
	matdata.Emissive.b  =  _mat->Emissive;

	matdata.Power  =  _mat->Power;

	Frame  =  new TFrameManager(FOwner->GetScene(),NULL,_mesh,_tex,_blend,true,true,false);
	Frame->Frame->SetMaterialRcv(matdata);

}

TRoopSlideMesh::~TRoopSlideMesh()
{
	SAFE_DELETE(Frame);

}

void TRoopSlideMesh::Move()
{
	TOBJ::Move();

	if( Age == 1 )
		Frame->Move(FirstX, Frame->GetY(), Frame->GetZ());

	Frame->MoveR(Speed,0,0);

	if( Speed>0 ) {
		if( Frame->GetX() >= FarX ) {
			Frame->MoveR(-ReturnX,0,0);
		}
	}
	else {
		if( Frame->GetX() <= FarX ) {
			Frame->MoveR(ReturnX,0,0);
		}
	}

	Frame->RotateR(RX,RY,RZ);

}

void TRoopSlideMesh::SetParam( f32 _FirstX, f32 _FarX, f32 _ReturnX, f32 _Speed, f32 _rx, f32 _ry, f32 _rz )
{
	FirstX = _FirstX;
	FarX = _FarX;
	ReturnX = _ReturnX;
	Speed = _Speed;

	RX = RoundOff(_rx);
	RY = RoundOff(_ry);
	RZ = RoundOff(_rz);

	Frame->Move(FirstX, Frame->GetY(), Frame->GetZ());

}
