#include "Missile.h"
#include "TOBJList.h"
#include "Effect.h"
#include "Sound.h"

TMakeMissile::TMakeMissile( TOBJList* owner, s32 _setnum, s32 _locknum )	: TOBJ(owner)
{
	FObjectName = "TMakeMissile";
	FSTR = 0;
	FKind = kEffect;
	SetNum = _setnum;
	TotalLock = _locknum;
	ShotInt = 8;
	MissileNum = 0;

}

void TMakeMissile::Move()
{
	if( Age % ShotInt == 0 ) {
		FOwner->Add(new TMissile(FOwner,FOwner->HeroPos.X,FOwner->HeroPos.Y,SetNum,MissileNum));
		Inc(MissileNum);
		FOwner->Add(new TMissile(FOwner,FOwner->HeroPos.X,FOwner->HeroPos.Y,SetNum,MissileNum));
		Inc(MissileNum);
	}

	if( Age >= (ShotInt*((MaxLock / 2) - 1)) ) {
		Die();
	}
	TOBJ::Move();
}

TMakeMissile2::TMakeMissile2( TOBJList* owner, s32 _setnum )	: TOBJ(owner)
{
	FObjectName = "TMakeMissile2";
	FSTR = 0;
	FKind = kEffect;
	SetNum = _setnum;
	ShotInt = 8;
	MissileNum = 0;
	FOwner->MissileON = true;
}

void TMakeMissile2::Move()
{
	if( Age % ShotInt == 0 ) {
		for( u32 i = 0; i <= 1; i++ ) {                
			FOwner->LockOn2(SetNum,MissileNum);
			FOwner->Add(new TMissile(FOwner,FOwner->HeroPos.X,FOwner->HeroPos.Y,SetNum,MissileNum));
			Inc(MissileNum);
		}
		FOwner->Add(new TMissileFlash(FOwner,FOwner->HeroPos.X+16,FOwner->HeroPos.Y+8));
		if( CreateSE ) FOwner->SoundEffect->WavPlay(WAMissile,4);
	}

	if( Age >= (ShotInt*((MaxLock / 2) - 1)) ) {
		Dec(FOwner->RestMis,1);
		Die();
		FOwner->MissileON = false;
	}
	TOBJ::Move();

}

TSmoke::TSmoke( TOBJList* owner, s32 sx, s32 sy, s32 _R, TSmoke* _prev )	: TOBJ(owner)
{
	FObjectName = "TSmoke";
	X = sx;
	Y = sy;
	FKind = kEffect;
	Speed = 13;
	Rdeg = _R+2048 & 0x0fff;
	ZoomRate = 0.2f + (f32)random(10)*0.01f;
	PersonalR = random(4095);
	IncR = random(64)-32;
	PrevSmoke = _prev;
	MotherIsDead = false;
	OriginalBGSpeed = FOwner->Speed;
	NextSmoke = NULL;

	TSpriteEXBLT sblt;
	sblt.RBM      =  bmNormal; // ���u�����h���[�h
	sblt.RColor   =  0xFFFFFFFF; // �|���S���F
	sblt.RDDDD    =  QD;     // TDDDD�̎Q��
	sblt.RHeight  =  32;        // ��
	sblt.RWidth   =  32;        // ����
	sblt.RX       =  0;        // X���W
	sblt.RY       =  0;        // Y���W
	sblt.RZ       =  SmokeZ;       // Z�l

	Inc(SmokeZ);
	if( SmokeZ > 6000 ) SmokeZ = 3000;

	Sprite[0] =  new TJoint(sblt);         // �X�v���C�g�̐���
	Inc(numSpr,1);
	Sprite[0]->GetSpr()->AddTexture(Eff8Tex, 33, 129, 64, 160);  //��
	Sprite[0]->GetSpr()->FCX += 16;
	Sprite[0]->GetSpr()->FCY += 16;
	Sprite[0]->GetSpr()->Move(X,Y);
	Sprite[0]->GetSpr()->Rotate(0);
	Sprite[0]->GetSpr()->FVisible = false;

}

void TSmoke::Move()
{
	s32 RR = Rdeg;
	s32 SmokeLife = 60;  
	s32 FadeSpeed = 4;
	switch( EffLevel ){
	case 0:{  SmokeLife = 6;  FadeSpeed = 40;  }	break;
	case 1:{  SmokeLife = 30;  FadeSpeed = 8;  }	break;
	case 2:{  SmokeLife = 70;  FadeSpeed = 4;  }	break;
	}

	X = (s32)(Sprite[0]->GetSpr()->FCX);
	Y = (s32)(Sprite[0]->GetSpr()->FCY);

	//�������������~�T�C���{�̂����ł����玀��
	if( MotherIsDead && (Age >= SmokeLife) ) {
		Die();
		return;
	}

	//�ׂ̉��Ƃ̋�������X�v���C�g�̊g�嗦�Ɖ�]�p���v�Z
	if( (PrevSmoke != NULL) ) {
		s32 DX = X-PrevSmoke->X;
		s32 DY = Y-PrevSmoke->Y;
		distance = RoundOff(pow(DX*DX + DY*DY , 0.5f))+1;
		RR = RoundOff(atan2(DY, DX) / (f32)M_PI*2048.f) & 0xfff;
	}
	else {
		Sprite[0]->GetSpr()->FVisible = false;
		distance = 1;
	}

	if( (Age >7) && (Age % 2 == 0) ) {
		Sprite[0]->GetSpr()->SetColor(255-(Age-7)*FadeSpeed,ctAlpha);
	}

	if( Age == 0 ) Sprite[0]->GetSpr()->FVisible = true;

	Sprite[0]->GetSpr()->Rotate(PersonalR);
	Sprite[0]->GetSpr()->Zoom(1.f, 1.f);
	Inc(PersonalR, IncR);
	Sprite[0]->GetSpr()->Rotate(PersonalR);
	f32 tempZX = distance / 32.f * 2.f;
	if( (tempZX < ZoomRate*2.0f) ) tempZX = ZoomRate*2.0f;
	Sprite[0]->GetSpr()->Zoom(tempZX,ZoomRate);
	Sprite[0]->GetSpr()->Rotate(Rdeg);      //���˕����ɉ�]  }
	Sprite[0]->GetSpr()->SlideMoveR(Speed,0);
	Sprite[0]->GetSpr()->Rotate((PersonalR+RR) & 0x0fff);        //�O�̉��̕����ɉ�]
	if( Age > 0 ) {
		switch( FOwner->NowStage ){
		case 0:
		case 99:Sprite[0]->GetSpr()->SetGradation(2500, 0xaa8800,1);	break;
		case 1:Sprite[0]->GetSpr()->SetGradation(2500, 0x88ff22,1);		break;
		case 2:
		case 3:Sprite[0]->GetSpr()->SetGradation(2500, 0xfffdbe,1);		break;
		case 4:Sprite[0]->GetSpr()->SetGradation(2500, 0x95d1d8,1);		break;
		}

		Sprite[0]->GetSpr()->MoveR(OriginalBGSpeed-5,0);
	}
	ZoomRate = ZoomRate+0.01f + (f32)random(10)*0.001f;

	if( Age % 3 == 0 ) Dec(Speed,1);
	if( Speed<2 ) Speed = 2;

	if( ((X > 710) || (X <-70) || (Y > 480+70) || (Y <0-70)) ) Sprite[0]->GetSpr()->FVisible = false;//��ʊO�ɏ����������

	//�������������\���ɂ���B
	//���ݎ��̂̓~�T�C���{�̂����ʂ܂ł���Â���
	if( Age >= SmokeLife ) Sprite[0]->GetSpr()->FVisible = false;

	TOBJ::Move();

}

void TSmoke::TellMomDead()
{
	MotherIsDead = true;
	if( PrevSmoke != NULL )
		PrevSmoke->TellMomDead();

}

void TSmoke::Die()
{
	TOBJ::Die();
	if( NextSmoke != NULL ) {
		NextSmoke->PrevSmoke = NULL;
	}

}

TMissile::TMissile( TOBJList* owner, s32 sx, s32 sy, s32 _setnum, s32 _misnum )	: TOBJ(owner)
{
	static const s32 ShotAngle[] = {-100,25,-75,75,-25,100,-50,0};
	static const s32 RandomY[] = {2,-2,1,-1,0,-2,2,-1};
	FObjectName = "TMissile";
	s32 RandomIndex = (_misnum+_setnum) & 7;

	X = sx;
	Y = sy;
	FSTR = 30;
	FKind = kMissile;
	YMove = RandomY[_misnum];
	Rdeg = 2*(ShotAngle[RandomIndex]);
	SetNum = _setnum;
	Locknum = _misnum+MaxLock*SetNum;
	BonusRate = RoundOff(pow( 2.f, RoundOff((f32)_misnum+1.f)/2.f));
	PrevSmoke = NULL;
	
	TSpriteEXBLT sblt;
	sblt.RBM      =  bmNormal; // ���u�����h���[�h
	sblt.RColor   =  0xFFFFFFFF; // �|���S���F
	sblt.RDDDD    =  QD;     // TDDDD�̎Q��
	sblt.RHeight  =  32;        // ��
	sblt.RWidth   =  32;        // ����
	sblt.RX       =  X;        // X���W
	sblt.RY       =  Y;        // Y���W
	sblt.RZ       =  20000;       // Z�l

	Sprite[0] =  new TJoint(sblt);         // �X�v���C�g�̐���
	Inc(numSpr,1);
	Sprite[0]->GetSpr()->AddTexture(JikiTex, 144, 0, 144+31, 31);    // �e�N�X�`����o�^
	Sprite[0]->GetSpr()->FVisible  =  true;
	Sprite[0]->GetSpr()->AddHitArea(TPoint(0, 0), TPoint(0, 31), TPoint(31, 31),TPoint(31, 0), htSky);
	Sprite[0]->GetSpr()->FHit        =  true;
	Sprite[0]->GetSpr()->FCX += 16;
	Sprite[0]->GetSpr()->FCY += 16;

	//�o�b�N�t�@�C�A
	sblt.RBM      =  bmAdd; // ���u�����h���[�h
	sblt.RColor   =  0xFFFFFFFF; // �|���S���F
	sblt.RDDDD    =  QD;     // TDDDD�̎Q��
	sblt.RHeight  =  20;        // ��
	sblt.RWidth   =  20;        // ����
	sblt.RX       =  0;        // X���W
	sblt.RY       =  0;        // Y���W
	sblt.RZ       =  4000;       // Z�l
	
	for( u32 i = 1; i <= 1; i++ ) {
		Sprite[i] =  new TJoint(sblt);         // backfire
		Inc(numSpr,1);

		Sprite[i]->GetSpr()->AddTexture(Eff1Tex, 1, 193, 63, 255);
		Sprite[i]->GetSpr()->FVisible  =  true;
		Sprite[i]->GetSpr()->FCX += 10;
		Sprite[i]->GetSpr()->FCY += 10;

		Sprite[i]->GetSpr()->Zoom(1.8f, 0.8f);
		Sprite[i]->GetSpr()->Move(X, Y+16);
	}

	Sprite[0]->Add(Sprite[1]);

	Sprite[0]->Rotate(Rdeg);

	//ID�̕t�^
	FID = NewID;
	Inc(NewID);

}

void TMissile::Move()
{
	s32 RandomAngle[] = { 2,4,-2,6,-6,-8,0,8,5,-1,-3,1,7,-5,0,3 };
	Rdeg  =  Rdeg & 0x0fff;
	PrevR = Rdeg;

	s32 Steering;
	if( Age <= 20 ) Steering = 6;
	else Steering = 1;

	X = RoundOff(Sprite[0]->GetSpr()->FCX);
	Y = RoundOff(Sprite[0]->GetSpr()->FCY);

	if( (! FOwner->UnLock[Locknum]) && (! FOwner->BossDie) ) {
		s32 DX = FOwner->BeLocked[Locknum].X-X;             //�����ƓG�Ƃ̋������v�Z
		s32 DY = FOwner->BeLocked[Locknum].Y-Y;

		s32 EnemyDeg = RoundOff(atan2(DY,DX) / (f32)M_PI*2048.f);
		EnemyDeg  =  EnemyDeg & 0x0fff;

		if( abs(Rdeg-EnemyDeg)>=2048 ) Inc(Rdeg, Sign(Rdeg-EnemyDeg)*RoundOff((f32)(4095-abs(Rdeg-EnemyDeg))/(f32)Steering));
		else if( abs(Rdeg-EnemyDeg)<2048 ) Dec(Rdeg,RoundOff((f32)(Rdeg-EnemyDeg)/(f32)Steering));
	}

	Inc(Rdeg, 2*RandomAngle[Age % 16]);
	Sprite[0]->Rotate(Rdeg);

	Sprite[0]->SlideMoveR(Age,YMove);

	X = RoundOff(Sprite[0]->GetSpr()->FCX);
	Y = RoundOff(Sprite[0]->GetSpr()->FCY);

	if( Age == 0 ) {
		PrevSmoke =  new TSmoke(FOwner,X,Y,Rdeg,NULL);
		FOwner->Add(PrevSmoke);
	}

	//���Ɖ��̊Ԃ����Ŗ��߂�
	s32 trace = 1;
	Inc(PrevSmoke->Y, FOwner->BGSlide.Y);    //�O�̉��̍��W�ɁA�w�i�X�N���[���ړ��ʂ��l��
	Inc(PrevSmoke->X, RoundOff(-8.f*cost[Rdeg & 0x0fff]));
	Inc(PrevSmoke->Y, RoundOff(-8.f*sint[Rdeg & 0x0fff]));
	switch( EffLevel )
	{
	case 0:
	case 1:trace = 1;
	case 2:
		{
			trace = RoundOff(pow(pow(X-PrevSmoke->X, 2)+pow(Y-PrevSmoke->Y, 2), 0.5f)/16.f);
			if( abs(GetDiffR(Rdeg,PrevR))>150 ) Inc(trace,1);
		}
	}
	if( trace > 0 ) {
		s32 divX = RoundOff((f32)(X-PrevSmoke->X)/(f32)trace);
		s32 divY = RoundOff((f32)(Y-PrevSmoke->Y)/(f32)trace);

		//��
		for( s32 i = 0 ; i < trace; i++ ) {
			TSmoke* tmpObj  =  new TSmoke(FOwner,PrevSmoke->X+divX,
				PrevSmoke->Y+divY,
				PrevR+(GetDiffR(PrevR,Rdeg) / trace)*(i+1),
				PrevSmoke);

			PrevSmoke->NextSmoke  =  tmpObj;
			PrevSmoke =  tmpObj;
			FOwner->Add(tmpObj);
		}
	}

	if( (FOwner->UnLock[Locknum] || FOwner->BossDie) && ((X > 710) || (X <-70) || (Y > 480+70) || (Y <0-70)) ) Die();//�e����ʊO�ɏ����������
	TOBJ::Move();

}

void TMissile::Die()
{
	TOBJ::Die();
	if( PrevSmoke == NULL )
		PrevSmoke->TellMomDead();
}

void TMissile::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);
	FOwner->Add(new TExpSmall(FOwner,X,Y,Rdeg,0,0));//����
	FOwner->UnLock[Locknum] = true;      //���b�N�I������
	Die();
}

TSmoke2::TSmoke2( TOBJList* owner, s32 sx, s32 sy, s32 _R, TOBJ* _PrevObj, s32 _misID ) : TOBJ(owner)
{
	FObjectName = "TSmoke2";
	X = sx;
	Y = sy;
	FKind = kEffect2;
	Speed = 15;
	PrevObj = _PrevObj;
	MissileID = _misID;
	Rdeg = (_R+2048) & 0x0fff;
	ZoomRate = 0.2f;

	TSpriteEXBLT sblt;
	sblt.RBM      =  bmSubAdd; // ���u�����h���[�h
	sblt.RColor   =  0xFFFFFFFF; // �|���S���F
	sblt.RDDDD    =  QD;     // TDDDD�̎Q��
	sblt.RHeight  =  32;        // ��
	sblt.RWidth   =  32;        // ����
	sblt.RX       =  0;        // X���W
	sblt.RY       =  0;        // Y���W
	sblt.RZ       =  3000;       // Z�l

	Sprite[0] =  new TJoint(sblt);         // �X�v���C�g�̐���
	Inc(numSpr,1);
	Sprite[0]->GetSpr()->AddTexture(Eff2Tex, 1, 193, 63, 255);    // �e�N�X�`����o�^
	Sprite[0]->GetSpr()->FCX += 16;
	Sprite[0]->GetSpr()->FCY += 16;
	Sprite[0]->GetSpr()->Move(X,Y);
	Sprite[0]->GetSpr()->Rotate(0);
	Sprite[0]->GetSpr()->SetColor(255,ctBlue);
	Sprite[0]->GetSpr()->SetColor(255,ctRed);
	Sprite[0]->GetSpr()->SetColor(255,ctGreen);
	Sprite[0]->GetSpr()->SetColor(235, ctAlpha);
	Sprite[0]->GetSpr()->FVisible = false;

	//ID�̕t�^
	FID = NewID;
	Inc(NewID);


}

void TSmoke2::Move()
{
	s32 RR = Rdeg;
	s32 SmokeLife = 60;  s32 FadeSpeed = 4;
	switch( EffLevel ){
		case 0:{  SmokeLife = 6;  FadeSpeed = 40;  }	break;
		case 1:{  SmokeLife = 30;  FadeSpeed = 8;  }	break;
		case 2:{  SmokeLife = 60;  FadeSpeed = 4;  }	break;
	}

	X = (s32)(Sprite[0]->GetSpr()->FCX);
	Y = (s32)(Sprite[0]->GetSpr()->FCY);

	if( PrevObj != NULL && !PrevObj->Dead()) {
		Sprite[0]->GetSpr()->FVisible = true;
		s32 DY = PrevObj->Y;
		s32 DX = PrevObj->X;
		distance = (s32)(pow(DX*DX+DY*DY, 0.5f));
		s32 RR = (s32)(atan2(DY, DX) / (f32)M_PI*2048.f) & 0xfff;
	}
	else Sprite[0]->GetSpr()->FVisible = false;

	if( Age % 2 == 0 ) {
		Sprite[0]->GetSpr()->SetColor(255-Age*FadeSpeed,ctBlue);
		Sprite[0]->GetSpr()->SetColor(255-Age*FadeSpeed,ctRed);
		Sprite[0]->GetSpr()->SetColor(255-Age*FadeSpeed,ctGreen);
		Sprite[0]->GetSpr()->SetColor(255-Age*FadeSpeed,ctAlpha);
	}

	Sprite[0]->GetSpr()->Rotate(0);
	Sprite[0]->GetSpr()->Zoom((f32)distance/32.f*3.f,ZoomRate);
	Sprite[0]->GetSpr()->Rotate(Rdeg);
	Sprite[0]->GetSpr()->SlideMoveR(Speed,0);
	Sprite[0]->GetSpr()->Rotate(RR);
	ZoomRate = ZoomRate+0.015f;

	if( Age % 3 == 0 ) Dec(Speed,1);
	if( Speed < 2 ) Speed = 2;

	if( ((X > 710) || (X <-70) || (Y > 480+70) || (Y <0-70)) ) Die();//��ʊO�ɏ����������
	if( Age >= SmokeLife ) Die();
	TOBJ::Move();

}

TMisBFire::TMisBFire( TOBJList* owner, s32 sx, s32 sy, s32 _R )	: TOBJ(owner)
{
	FObjectName = "TMisBFire";
	X = sx;
	Y = sy;
	Rdeg = (_R+2048) && 0x0fff;
	FKind = kEffect;

	TSpriteEXBLT sblt;
	sblt.RBM      =  bmSubAdd; // ���u�����h���[�h
	sblt.RColor   =  0xFFFFFFFF; // �|���S���F
	sblt.RDDDD    =  QD;     // TDDDD�̎Q��
	sblt.RHeight  =  20;        // ��
	sblt.RWidth   =  20;        // ����
	sblt.RZ       =  19900;       // Z�l

	Sprite[0] =  new TJoint(sblt);         // �X�v���C�g�̐���
	Inc(numSpr,1);
	Sprite[0]->GetSpr()->AddTexture(Eff1Tex, 0, 0, 15, 15);    // �e�N�X�`����o�^
	Sprite[0]->GetSpr()->FVisible  =  true;
	Sprite[0]->GetSpr()->FCX += 10;
	Sprite[0]->GetSpr()->FCY += 10;
	Sprite[0]->GetSpr()->Move(X,Y);
	Sprite[0]->GetSpr()->SetColor(255, ctRed);
	Sprite[0]->GetSpr()->SetColor(128, ctGreen);
	Sprite[0]->GetSpr()->SetColor(64,ctAlpha);
	Sprite[0]->GetSpr()->Zoom(10,0.8);
	Sprite[0]->Move(X,Y);

}

void TMisBFire::Move()
{
	TOBJ::Move();
	Die();

}
