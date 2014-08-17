#include "TOBJ.h"
#include "TOBJList.h"

TOBJ::TOBJ( TOBJList* _owner )
{
	deleted = false;
	Age = 0;
	FOwner = _owner;
	//LockNum = -1;
	FDead = false;
	numSpr = 0;
	distance = 0;
	TempHP = 0;
	FDef = 0;
	FIsParentExists = false;
	LastSlashed = 0;
	ResilienceX = 0;
	ResilienceY = 0;
	for( s32 i = 0; i < MaxJoint; i++ ) {
		Sprite[i] = NULL;
		Hit[i] = false;
	}
	BonusRate = 1;
	Sprite_Center = 0;

	for( s32 i = 0; i <= 7; i++ ){
		TempValue[i] = 0;
	}

	FID = -1;
}

TOBJ::~TOBJ()
{
	for( s32 i = 0; i < numSpr; i++ ) {
		SAFE_DELETE( Sprite[i] );
	}
	deleted = true;
}

void TOBJ::MakeSprite( TDGCarad& _dddd, TDGTexture& _Tex, s32 _num, s32 TX, s32 TY, s32 TW, s32 TH, s32 Z, s32 HX, s32 HY, s32 HW, s32 HH, s32 _cx, s32 _cy, bool com )
{
	TSpriteEXBLT sblt;
	sblt.RBM      =  bmNormal;    // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  &_dddd;        // TDDDDの参照
	sblt.RWidth   =  abs(TW);        // 幅
	sblt.RHeight  =  abs(TH);        // 高さ
	sblt.RX       =  0;         // X座標
	sblt.RY       =  0;         // Y座標
	sblt.RZ       =  Z * 100;        // Z値   //★★★　Z最大値拡張のためゲタ履き。

	//if( (_Tex = BumpTex) or (_Tex = BossBump) ) sblt.RBM = bmAdd;

	Sprite[_num] = new TJoint(sblt);
	numSpr++;

	TSpriteEX* spr = Sprite[_num]->GetSpr();
	spr->AddTexture(&_Tex, TX, TY, TX+TW, TY+TH);    // テクスチャを登録
	spr->FVisible  =  true;
	if( HW+HH>0 ) {
		spr->AddHitArea(glm::vec2(HX, HY), glm::vec2(HX, HY+HH), glm::vec2(HX+HW, HY+HH), glm::vec2(HX+HW, HY), htSky);
		spr->FHit      =  true;
	}
	spr->FCX += _cx;
	spr->FCY += _cy;
	spr->FToneMapping  =  true;
	if( com ) {
		spr->SetColor(255, ctRed);
		spr->SetColor(64, ctBlue);
		spr->SetColor(64, ctGreen);
	}

}

void TOBJ::MakeSpriteLite( TDGCarad* _dddd, s32 _num, s32 _x, s32 _y, s32 _w, s32 _h, s32 _z, TBlendMode _blend )
{
	TSpriteEXBLT sblt;
	sblt.RBM      =  _blend;    // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  _dddd;        // TDDDDの参照
	sblt.RWidth   =  abs(_w);        // 幅
	sblt.RHeight  =  abs(_h);        // 高さ
	sblt.RX       =  0;         // X座標
	sblt.RY       =  0;         // Y座標
	sblt.RZ       =  _z ;        // Z値

	Sprite[_num] = new TJoint(sblt);
	numSpr++;

	Sprite[_num]->GetSpr()->FVisible  =  true;
	Sprite[_num]->GetSpr()->Move((f32)_x,(f32)_y);

}

bool TOBJ::CheckTouch( TOBJ* Target )
{
	bool result  =  false;
	if( !Target->FDead ) {
		for( s32 i = 0; i < numSpr; i++ ){
			Hit[i] = false;
		}

		for( s32  j = 0; j < Target->numSpr; j++ )
		{
			Target->Hit[j] = false;
		}

		for( s32 i = 0; i < numSpr; i++ ) {
			//      FHit[i] = false;
			for( s32 j = 0; j < Target->numSpr; j++ ){
				//        Target.FHit[j] = false;
				if( Sprite[i]->GetSpr()->CollisionCheck(Target->Sprite[j]->GetSpr()) ) {
					Hit[i] = true;
					Target->Hit[j] = true;
					result  =  true;
				}
			}
		}
	}

	return result;
}

void TOBJ::Move()
{
	Age++;
}

void TOBJ::OnDamaged( TOBJ* Target )
{

}

void TOBJ::Die()
{
	FDead = true;
}

s32 TOBJ::OnDraw()
{
	s32 SPCount = 0;
	//スプライトを登録
	for( s32 i = 0; i < numSpr; i++ )
	{
		if( Sprite[i] ) {
			FOwner->AddSprite(Sprite[i]->GetSpr());
			SPCount++;
		}
	}

	return SPCount;
}

void TOBJ::OnBroken()
{
	CalcBonus();

	Die();
}

void TOBJ::OnResponse( stringstream& _res )
{

}

s32 TOBJ::OnExRender()
{
	return 0;

}

void TOBJ::OnDeviceLost()
{

}

void TOBJ::OnDeviceReset()
{

}

s32 TOBJ::GetAge()
{
	return Age;

}

void TOBJ::MakeSprFromID( TDGCarad& _dddd, s32 _id, TDGTexture& _tex, s32 _z, bool _extra )
{
	s32 TWidth;
	s32 THeight;

	TSpriteData data;
	for( s32 i = 0; i < FOwner->SpriteDataList[_id].TotalSpr; i++ ) {
		data = FOwner->SpriteDataList[_id].Sprites[i];
		TWidth = data.TexRect.Right - data.TexRect.Left;
		THeight = data.TexRect.Bottom - data.TexRect.Top;

		MakeSpriteLite(_dddd, numSpr,0,0,TWidth,THeight, _z + data.Z*2, bmNormal);
		FOwner->SpriteDataList[_id].Sprites[i].Spr = Sprite[numSpr-1];

		TJoint* tmp = Sprite[numSpr-1];
		tmp->GetSpr()->AddTexture(&_tex, data.TexRect.Left, data.TexRect.Top, data.TexRect.Right-1,data.TexRect.Bottom-1);
		if( (data.HitRect[0].X - data.HitRect[3].X) != 0 ) {
			tmp->GetSpr()->AddHitArea(data.HitRect[0], data.HitRect[1], data.HitRect[2], data.HitRect[3], htSky);
			tmp->GetSpr()->FHit      =  true;
		}
		tmp->GetSpr()->FCX += data.CX;
		tmp->GetSpr()->FCY += data.CY;
		tmp->GetSpr()->Move((f32)X, (f32)Y);
		tmp->GetSpr()->MoveR((f32)data.X, (f32)data.Y);
		tmp->MonoRotate(data.Rdeg);
	}

	//親スプライトをセット
	for( s32 i = 0; i < FOwner->SpriteDataList[_id].TotalSpr; i++ ) {
		data  =  FOwner->SpriteDataList[_id].Sprites[i];
		if( data.ParentID != -1 ) {
			FOwner->SpriteDataList[_id].Sprites[data.ParentID].Spr->Add(data.Spr);
		}
	}

	//映り込みスプライト
	if( _extra ) {
		for( s32 i = 0; i < FOwner->SpriteDataList[_id].TotalSpr; i++ ){
			data  =  FOwner->SpriteDataList[_id].Sprites[i];
			TWidth = data.TexRect.Right - data.TexRect.Left;
			THeight = data.TexRect.Bottom - data.TexRect.Top;

			MakeSpriteLite(_dddd, numSpr,0,0,TWidth,THeight,_z + data.Z*2 -1,bmNormal);

			TJoint* tmp = Sprite[numSpr-1];
			tmp->GetSpr()->AddTexture(&_tex, data.TexRect.Left, data.TexRect.Top, data.TexRect.Right-1,data.TexRect.Bottom-1);

			tmp->GetSpr()->FCX += data.CX;
			tmp->GetSpr()->FCY += data.CY;
			tmp->GetSpr()->Move((f32)X, (f32)Y);
			tmp->GetSpr()->MoveR((f32)data.X, (f32)data.Y);
			tmp->Sticky = true;
			tmp->MonoRotate(data.Rdeg);
			tmp->GetSpr()->SetColor(160,ctAlpha);
			tmp->GetSpr()->FEffectNum = 1;
			tmp->GetSpr()->FBumpSize = 2.9f;
			tmp->GetSpr()->FBumpTex = &_tex;
			tmp->GetSpr()->FBumpLevel = 2.0f;
			Sprite[i]->Add(Sprite[numSpr-1]);
		}

	}
}

void TOBJ::MotionControl( s32 _id )
{
	for( s32 i = 0; i < FOwner->MotionDataList[_id].TotalSpr; i++ ) {

		if( Sprite[i]->Rdegree != FOwner->MotionDataList[_id].Sprites[i].Rdeg ) {
			TPartsMotionData& tmp = FOwner->MotionDataList[_id].Sprites[i];
			s32 Rdiff = GetDiffR(Sprite[i]->Rdegree, tmp.Rdeg);

			switch( tmp.Direction )
			{
			case -1:{
				Sprite[i]->RotateKeepR(-(s32)(abs(Rdiff)/tmp.Speed)+Sign(Rdiff));
					}
					break;
			case 0 :{
				if( abs(Rdiff)>=2048 ) Sprite[i]->RotateKeepR(-Sign(Rdiff) * (s32)ceil((f32)(4095-abs(Rdiff))/(f32)tmp.Speed)-Sign(Rdiff));
				else if( abs(Rdiff)<2048 ) Sprite[i]->RotateKeepR((s32)ceil((f32)(Rdiff)/(f32)tmp.Speed)+Sign(Rdiff));
					}
					break;
			case 1:{
				Sprite[i]->RotateKeepR((s32)(abs(Rdiff)/tmp.Speed)+Sign(Rdiff));
				   }
				   break;
			}
		}
	}
}

void TOBJ::MotionControlSoon( s32 _id )
{
	for( s32 i = 0; i < FOwner->MotionDataList[_id].TotalSpr; i++ ) {
		Sprite[i]->RotateKeepR(FOwner->MotionDataList[_id].Sprites[i].Rdeg);
	}
}

void TOBJ::CollisionReaction( f32 _x, f32 _y )
{
	glm::vec2 vec = glm::normalize(glm::vec2(ResilienceX+_x , ResilienceY+_y));
	ResilienceX = vec.x;
	ResilienceY = vec.y;
}

void TOBJ::CalcBonus()
{
	FOwner->TotalScore += Score*BonusRate;
	FOwner->TotalHit++;
	if( X>640-64 ) X = 640-64;
	if( X<64 ) X = 64;

	if( Y<64 ) Y = 64;
	if( Y>480-32 ) Y = 480-32;

	FOwner->Add( new TEnemyScore(FOwner,X,Y,Score*BonusRate) );
	if( BonusRate>1 )
		FOwner->Add( new TMultiple(FOwner,BonusRate,X,Y-48) );
	FOwner->Add( new THitCount(FOwner,640,64) );
	if( CommandRed ) {
		if( BonusRate>1 ) ComBonus = ComBonus*2;                  //ミサイルで倒せば得点２倍
		s32 Cbonus = FOwner->TotalHit*ComBonus;                          //赤色ならボーナス
		FOwner->TotalScore += Cbonus;
		//    FOwner->SentenceCenter('commander bonus',X,Y+16,0,2);
		//    FOwner->SentenceCenter(IntToStr(Cbonus),X,Y+32,0,2);
		//    FOwner->Add(TComBonus.Create(FOwner,Cbonus));

		FOwner->Add( new TComBonus2( FOwner,X,Y+24,ComBonus, FOwner->TotalHit));

		if( CreateSE ) FOwner->SoundEffect->WavPlay2(WABonus,13,100);
		//    FOwner->Quake(4);
	}
}

