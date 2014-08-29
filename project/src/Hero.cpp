#include "Hero.h"
#include "Effect.h"
#include "TOBJList.h"
#include "Gauge.h"
#include "Missile.h"
#include "Effect.h"
#include "Sound.h"


void TJikiBase::MakeVapor( s32 _num, s32 _sprite, s32 _x, s32 _y ) 
{
	if( FOwner->NowStage == 3 ) return;

	f32 XX = (s32)(Sprite[_sprite]->GetSpr()->FCX + (f32)_x*ZoomRate);
	f32 YY = (s32)(Sprite[_sprite]->GetSpr()->FCY + (f32)_y*ZoomRate);
	s32 RR = Sprite[_sprite]->Rdegree & 0x0FFF;
	PosRoll( XX, YY,Sprite[_sprite]->GetSpr()->FCX, Sprite[_sprite]->GetSpr()->FCY, RR);
	Vapor->MakeVapor((s32)(XX),(s32)(YY),_num);

}

TJikiBase::TJikiBase( TOBJList* owner, s32 _x, s32 _y ) : TOBJ(owner)
{

}

TJikiBase::~TJikiBase()
{

}

void TJikiBase::Move()
{
	TOBJ::Move();
}

void TJikiBase::Input()
{

}

void TJikiBase::Slide( f32 _x, f32 _y )
{
	s32 YLimit;
	if( FOwner->IsSequence() )
		YLimit = 4;
	else
		YLimit = 32;

	Sprite[HBody]->MoveR(_x,_y);

	//画面端で止まる
	if( Sprite[HBody]->GetSpr()->FCX < YLimit ){
		Sprite[HBody]->GetSpr()->Move((f32)YLimit, Sprite[HBody]->GetSpr()->FCY);
		SlideX=0;
		SlideY=0;
	}
	else if( Sprite[HBody]->GetSpr()->FCX > 640-YLimit ){
		Sprite[HBody]->GetSpr()->Move((f32)(640-YLimit), Sprite[HBody]->GetSpr()->FCY);
		SlideX=0;
		SlideY=0;
	}

	if( Sprite[HBody]->GetSpr()->FCY < YLimit ){
		Sprite[HBody]->GetSpr()->Move(Sprite[HBody]->GetSpr()->FCX,(f32)YLimit);
		SlideX=0;
		SlideY=0;
	}
	else if( Sprite[HBody]->GetSpr()->FCY > 480-YLimit ){
		Sprite[HBody]->GetSpr()->Move(Sprite[HBody]->GetSpr()->FCX, (f32)(480-YLimit));
		SlideX=0;
		SlideY=0;
	}

}

void TJikiBase::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);
}

void TJikiBase::Die()
{
	FOwner->Add(new TBoxContrl(FOwner,5));
	FOwner->Add(new TExplarge(FOwner,X,Y,1,true));//爆発
	FOwner->HeroDie = true;
	HitArea->Die();
	Vapor->Die();
	TOBJ::Die();

}

void TJikiBase::BodyControl( s32 Bo, s32 L2, s32 L3, s32 R2, s32 R3 )
{
	s32 Rdiff = Bo-Sprite[HBody]->Rdegree;
	Rdiff = Rdiff & 0x0FFF;
	if( Rdiff>=2047 ) {
		Sprite[HBody]->RotateR(-ceil((f32)(4095-Rdiff)/8.f));
		Sprite[HRArm]->RotateR(ceil((f32)(4095-Rdiff)/8.f));
		//Sprite[HLArm]->RotateR(ceil((4095-Rdiff)/8));
		Sprite[HLLeg2]->RotateR(ceil((f32)(4095-Rdiff)/8.f));
		Sprite[HRCover]->RotateR(ceil((f32)(4095-Rdiff)/8.f));
		Sprite[HLCover]->RotateR(ceil((f32)(4095-Rdiff)/8.f));
		Sprite[HRWing]->RotateR(ceil((f32)(4095-Rdiff)/8.f));
		Sprite[HLWing]->RotateR(ceil((f32)(4095-Rdiff)/8.f));
	}
	else if( (Rdiff<2047) & (Rdiff>0) ) {
		Sprite[HBody]->RotateR(ceil((f32)(Rdiff)/8.f));
		Sprite[HRArm]->RotateR(-ceil((f32)(Rdiff)/8.f));
		//Sprite[HLArm]->RotateR(-ceil((Rdiff)/8));
		Sprite[HLLeg2]->RotateR(-ceil((f32)(Rdiff)/8.f));
		Sprite[HLLeg2]->RotateR(-ceil((f32)(Rdiff)/8.f));
		Sprite[HRCover]->RotateR(-ceil((f32)(Rdiff)/8.f));
		Sprite[HLCover]->RotateR(-ceil((f32)(Rdiff)/8.f));
		Sprite[HRWing]->RotateR(-ceil((f32)(Rdiff)/8.f));
		Sprite[HLWing]->RotateR(-ceil((f32)(Rdiff)/8.f));
	}

	Rdiff = L2-Sprite[HLLeg2]->Rdegree;
	Rdiff = Rdiff & 0x0FFF;
	if( Rdiff>=2047 ) { Sprite[HLLeg2]->RotateR(-ceil((f32)(4095-Rdiff)/8.f)); Sprite[HLLeg3]->RotateR(ceil((f32)(4095-Rdiff)/8.f)); }
	else if( (Rdiff<2047) && (Rdiff>0) ) { Sprite[HLLeg2]->RotateR(ceil((f32)(Rdiff)/8.f)); Sprite[HLLeg3]->RotateR(-ceil((f32)(Rdiff)/8.f)); }

	Rdiff = L3-Sprite[HLLeg3]->Rdegree;
	Rdiff = Rdiff & 0x0FFF;
	if( Rdiff>=2047 ) Sprite[HLLeg3]->RotateR(-ceil((f32)(4095-Rdiff)/8.f));
	else if( (Rdiff<2047) && (Rdiff>0) ) Sprite[HLLeg3]->RotateR(ceil((f32)(Rdiff)/8.f));

	Rdiff = R2-Sprite[HRLeg2]->Rdegree;
	Rdiff = Rdiff & 0x0FFF;
	if( Rdiff>=2047 ) { Sprite[HRLeg2]->RotateR(-ceil((f32)(4095-Rdiff)/8.f)); Sprite[HRLeg3]->RotateR(ceil((f32)(4095-Rdiff)/8.f)); }
	else if( (Rdiff<2047) && (Rdiff>0) ) { Sprite[HRLeg2]->RotateR(ceil((f32)(Rdiff)/8.f)); Sprite[HRLeg3]->RotateR(-ceil((f32)(Rdiff)/8.f)); }

	Rdiff = R3-Sprite[HRLeg3]->Rdegree;
	Rdiff = Rdiff & 0x0FFF;
	if( Rdiff>=2047 ) Sprite[HRLeg3]->RotateR(-ceil((f32)(4095-Rdiff)/8.f));
	else if( (Rdiff<2047) && (Rdiff>0) ) Sprite[HRLeg3]->RotateR(ceil((f32)(Rdiff)/8.f));


}

void TJikiBase::BFControl( s32 _R, f32 _zoom )
{
	s32 Rdiff = _R-Sprite[HRCover]->Rdegree;
	Rdiff = Rdiff & 0x0FFF;
	if( Rdiff>=2047 ) Sprite[HRCover]->RotateR(-ceil((f32)(4095-Rdiff)/4.f));
	else if( (Rdiff<2047) && (Rdiff>0) ) Sprite[HRCover]->RotateR(ceil((f32)(Rdiff)/4.f));

	Rdiff = _R-Sprite[HLCover]->Rdegree;
	Rdiff = Rdiff & 0x0FFF;
	if( Rdiff>=2047 ) Sprite[HLCover]->RotateR(-ceil((f32)(4095-Rdiff)/4.f));
	else if( (Rdiff<2047) && (Rdiff>0) ) Sprite[HLCover]->RotateR(ceil((f32)(Rdiff)/4.f));

	Rdiff = _R-Sprite[HRWing]->Rdegree;
	Rdiff = Rdiff & 0x0FFF;
	if( Rdiff>=2047 ) Sprite[HRWing]->RotateR(-ceil((f32)(4095-Rdiff)/4.f));
	else if( (Rdiff<2047) && (Rdiff>0) ) Sprite[HRWing]->RotateR(ceil((f32)(Rdiff)/4.f));

	Rdiff = _R-Sprite[HLWing]->Rdegree;
	Rdiff = Rdiff & 0x0FFF;
	if( Rdiff>=2047 ) Sprite[HLWing]->RotateR(-ceil((f32)(4095-Rdiff)/4.f));
	else if( (Rdiff<2047) && (Rdiff>0) ) Sprite[HLWing]->RotateR(ceil((f32)(Rdiff)/4.f));

	f32 ZRate = Sprite[HRWing]->GetSpr()->FZX;
	if( ZRate>_zoom*ZoomRate ) ZRate = ZRate-0.1f*ZoomRate;
	if( ZRate<_zoom*ZoomRate ) ZRate = ZRate+0.1f*ZoomRate;
	Sprite[HRWing]->Zoom(ZRate,ZRate);
	Sprite[HLWing]->Zoom(ZRate,ZRate);

}

void TJikiBase::HeroJet()
{
	s32 RR;
	f32 XX,YY;
	//バーニア
	switch( BoostLV ){
	case 1:{ BFControl(3200+Sprite[HBody]->Rdegree,0.3f); Speed = 5; } break;
	case 2:{
		Speed = 10;
		BFControl(0+Sprite[HBody]->Rdegree,1.0f);

		//画面を揺らす、光らす、輪っか生成
		if( (Age-BoostTime) == 5 ) {
			FOwner->Quake(4,30);
			RR = Sprite[HBody]->Rdegree & 0x0FFF;
			XX = Sprite[HBody]->GetSpr()->FCX-23.f*ZoomRate;
			YY = Sprite[HBody]->GetSpr()->FCY-16.f*ZoomRate;
			PosRoll(XX,YY,X,Y,RR);
			FOwner->Add(new TBoxContrl(FOwner,4));
			FOwner->Add(new TSRControl(FOwner,(s32)(XX)+32,(s32)(YY)-32,BoostR));
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WABoost,1);
			Vapor->Clear();
		}

		if( (Age-BoostTime)>5 ) {
			//噴射
			RR = Sprite[HRWing]->Rdegree & 0x0FFF;
			XX = (s32)(Sprite[HRWing]->GetSpr()->FCX-9.f*ZoomRate);
			YY = (s32)(Sprite[HRWing]->GetSpr()->FCY+6.f*ZoomRate);
			PosRoll(XX,YY,Sprite[HRWing]->GetSpr()->FCX,Sprite[HRWing]->GetSpr()->FCY,RR);
			FOwner->Add(new TBLight(FOwner,XX + SlideX,YY + SlideY,ZoomRate,9500));
			if( (Age-BoostTime)<=21 ) {
				//反動
				s32 reaction = 21-(Age-BoostTime);
				Slide(reaction*cost[BoostR],reaction*sint[BoostR]);
				//火花
				FOwner->Add(new TParticle2(FOwner,(s32)(XX),(s32)(YY)-32,0,2048+(random(600)-300),5+random(10),5+random(10)));
				FOwner->Add(new TParticle2(FOwner,(s32)(XX),(s32)(YY)-32,0,2048+(random(600)-300),5+random(10),5+random(10)));
			}
			if( Age % 4 == 0 ) {
				XX = Sprite[HRWing]->GetSpr()->FCX-9.f*ZoomRate;
				YY = Sprite[HRWing]->GetSpr()->FCY-8.f*ZoomRate;
				PosRoll(XX,YY,Sprite[HRWing]->GetSpr()->FCX,Sprite[HRWing]->GetSpr()->FCY,RR);
				FOwner->Add(new TBFire(FOwner,(s32)(XX),(s32)(YY),(RR+412) & 0x0fff,BoostLV,ZoomRate,this));
				XX = Sprite[HRWing]->GetSpr()->FCX-7*ZoomRate;
				YY = Sprite[HRWing]->GetSpr()->FCY-4*ZoomRate;
				PosRoll(XX,YY,Sprite[HRWing]->GetSpr()->FCX,Sprite[HRWing]->GetSpr()->FCY,RR);
				FOwner->Add(new TBFire(FOwner,(s32)(XX),(s32)(YY),(RR+200) & 0x0fff,BoostLV,ZoomRate,this));
				XX = Sprite[HRWing]->GetSpr()->FCX-5*ZoomRate;
				YY = Sprite[HRWing]->GetSpr()->FCY+2*ZoomRate;
				PosRoll(XX,YY,Sprite[HRWing]->GetSpr()->FCX,Sprite[HRWing]->GetSpr()->FCY,RR);
				FOwner->Add(new TBFire(FOwner,(s32)(XX),(s32)(YY),(RR) & 0x0fff,BoostLV,ZoomRate,this));
			}
		}
		if( (Age-BoostTime)>21 ) BoostLV = 3;
		   }break;
	case 3:{
		Speed = 8;
		BFControl(0+Sprite[HBody]->Rdegree,1.0f);
		//ヴェイパートレイル
		MakeVapor(0,HBody,0,0);
		MakeVapor(1,HRCover,-15,-16);
		MakeVapor(2,HLCover,-15,-12);
		MakeVapor(3,HRLeg3,-37,35);
		MakeVapor(4,HLLeg3,-37,40);

		//噴射
		RR = Sprite[HRWing]->Rdegree & 0x0FFF;
		XX = (s32)(Sprite[HRWing]->GetSpr()->FCX-9.f*ZoomRate);
		YY = (s32)(Sprite[HRWing]->GetSpr()->FCY-4.f*ZoomRate);
		PosRoll(XX,YY,Sprite[HRWing]->GetSpr()->FCX,Sprite[HRWing]->GetSpr()->FCY,RR);
		FOwner->Add(new TBLight(FOwner,XX + SlideX,YY + SlideY,ZoomRate,9500));
		if( Age % 4 == 0 ) {
			XX = Sprite[HRWing]->GetSpr()->FCX-9.f*ZoomRate;
			YY = Sprite[HRWing]->GetSpr()->FCY-8.f*ZoomRate;
			PosRoll(XX,YY,Sprite[HRWing]->GetSpr()->FCX,Sprite[HRWing]->GetSpr()->FCY,RR);
			FOwner->Add(new TBFire(FOwner,(s32)(XX),(s32)(YY),(RR+412) & 0x0fff,BoostLV,ZoomRate,this));
			XX = Sprite[HRWing]->GetSpr()->FCX-7.f*ZoomRate;
			YY = Sprite[HRWing]->GetSpr()->FCY-4.f*ZoomRate;
			PosRoll(XX,YY,Sprite[HRWing]->GetSpr()->FCX,Sprite[HRWing]->GetSpr()->FCY,RR);
			FOwner->Add(new TBFire(FOwner,(s32)(XX),(s32)(YY),(RR+200) & 0x0fff,BoostLV,ZoomRate,this));
			XX = Sprite[HRWing]->GetSpr()->FCX-5*ZoomRate;
			YY = Sprite[HRWing]->GetSpr()->FCY+2*ZoomRate;
			PosRoll(XX,YY,Sprite[HRWing]->GetSpr()->FCX,Sprite[HRWing]->GetSpr()->FCY,RR);
			FOwner->Add(new TBFire(FOwner,(s32)(XX),(s32)(YY),(RR) & 0x0fff,BoostLV,ZoomRate,this));
		}
		   }break;
	case 4:{
		Speed = 5;
		BFControl(3200+Sprite[HBody]->Rdegree,0.3f);

		if( (Age-BoostTime)<21 ) {
			//ヴェイパートレイル
			MakeVapor(0,HBody,0,0);
			MakeVapor(1,HRCover,-15,-16);
			MakeVapor(2,HLCover,-15,-12);
			MakeVapor(3,HRLeg3,-37,35);
			MakeVapor(4,HLLeg3,-37,40);
		}

		if( (Age-BoostTime)>10 ) {
			//噴射
			XX = (s32)(Sprite[HRLeg3]->GetSpr()->FCX-27.f*ZoomRate);
			YY = (s32)(Sprite[HRLeg3]->GetSpr()->FCY+25.f*ZoomRate);
			RR = Sprite[HRLeg3]->Rdegree & 0x0FFF;
			PosRoll(XX,YY,Sprite[HRLeg3]->GetSpr()->FCX,Sprite[HRLeg3]->GetSpr()->FCY,RR);
			FOwner->Add(new TBFire(FOwner,(s32)(XX),(s32)(YY),(RR-400) & 0x0fff,BoostLV,ZoomRate,this));
			FOwner->Add(new TBLight(FOwner,XX,YY,ZoomRate,9000));
			XX = (s32)(Sprite[HLLeg3]->GetSpr()->FCX-27.f*ZoomRate);
			YY = (s32)(Sprite[HLLeg3]->GetSpr()->FCY+25.f*ZoomRate);
			RR = Sprite[HLLeg3]->Rdegree & 0x0FFF;
			PosRoll(XX,YY,Sprite[HLLeg3]->GetSpr()->FCX,Sprite[HLLeg3]->GetSpr()->FCY,RR);
			FOwner->Add(new TBFire(FOwner,(s32)(XX),(s32)(YY),(RR-400) & 0x0fff,BoostLV,ZoomRate,this));
			FOwner->Add(new TBLight(FOwner,XX,YY,ZoomRate,9500));
		}
		if( (Age-BoostTime)>30 ) BoostLV = 1;
		   }break;
	}

	RR = Sprite[HBody]->Rdegree & 0x0FFF;
	XX = Sprite[HBody]->GetSpr()->FCX-23.f*ZoomRate;
	YY = Sprite[HBody]->GetSpr()->FCY-16.f*ZoomRate;
	PosRoll(XX,YY,X,Y,RR);
	FOwner->Add(new TBLight(FOwner,XX + SlideX,YY + SlideY,ZoomRate,9500));

	if( Age % 4 == 0 )
		FOwner->Add(new TBFire(FOwner,(s32)(XX),(s32)(YY),RR-384,BoostLV,ZoomRate,this));


}

void TJikiBase::Nokezori()
{
	s32 NokeSpeed;//のけぞりスピード
	s32 Interval;
	if( (NokeTime-Age+20)>0 ) NokeSpeed = (NokeTime-Age+20) / 2;
	else NokeSpeed = 1;

	Slide((s32)(-cost[CrushR]*NokeSpeed),(s32)(-sint[CrushR]*NokeSpeed));
	BodyControl(4240,3188,3132,3368,3272);

	if( FOwner->IsSequence() )
		Interval  =  60;
	else
		Interval  =  40;

	if( Age-NokeTime>Interval ) {
		ActFlg[HANokezori] = 0;
		FOwner->Weapon = true;
	}
}

void TJikiBase::MonoRotate( s32 _Sprnum, s32 _dist, s32 _speed )
{
	_dist = _dist & 0x0fff;
	s32 Rdiff = _dist-Sprite[_Sprnum]->Rdegree;
	{
		if( abs(Rdiff)>=2048 ) {
			Sprite[_Sprnum]->RotateR(-Sign(Rdiff)*ceil((4095-abs(Rdiff))/_speed)-Sign(Rdiff)*1);
			for( u32 i = 0 ; i < Sprite[_Sprnum]->GetSize(); i++ ){
				if( Sprite[_Sprnum]->Objs(i)!=NULL )
					Sprite[_Sprnum]->Objs(i)->RotateR(Sign(Rdiff)*ceil((4095-abs(Rdiff))/_speed)+Sign(Rdiff)*1);
			}
		}
		else if( abs(Rdiff)<2048 ) {
			Sprite[_Sprnum]->RotateR(ceil((Rdiff)/_speed)+Sign(Rdiff)*1);
			for( u32 i = 0 ; i < Sprite[_Sprnum]->GetSize(); i++ ){
				if( Sprite[_Sprnum]->Objs(i)!=NULL )
					Sprite[_Sprnum]->Objs(i)->RotateR(-ceil((Rdiff)/_speed)-Sign(Rdiff)*1);
			}
		}
	}

}

void TJikiBase::StartEvent()
{
	switch( ActFlg[HAStart] ){
	case 1:{
		if( CreateSE ) FOwner->SoundEffect->WavPlay(WAStart,0);
		Inc(ActFlg[HAStart]);
		ActCount[HAStart] = -1;
		   }break;
	case 2:{
		BodyControl(256,400,88,500,88);
		Sprite[HBody]->MoveR(15.f- (f32)ActCount[HAStart]/3.f, 2.f);

		if( ActCount[HAStart]>50 ) {
			Inc(ActFlg[HAStart]);
			ActCount[HAStart] = -1;
		}
		   }break;
	case 3:{
		if( ActCount[HAStart]>0 ) {
			ActFlg[HAStart] = 0;
			ActCount[HAStart] = -1;
		}
		   }break;
	}
	Inc(ActCount[HAStart]);

}

void TJikiBase::Flash( bool _state )
{

}

void TJikiBase::BlinkControl()
{
	if( FOwner->Muteki ) {
		//点滅
		if( ((Age-LastBlink)<BlinkTime) ) {
			if( Age % (FrameRate+1) == 0 ) {
				for( s32 i = 1 ; i < numSpr; i++ )
					Sprite[i]->GetSpr()->FVisible = ! Sprite[i]->GetSpr()->FVisible;
			}
		}
		else {
			for( s32 i = 1 ; i < numSpr; i++ )
				Sprite[i]->GetSpr()->FVisible = true;
			if( ! MissileMuteki ) FOwner->Muteki = false;
		}
	}

	if( (MissileMuteki) ) {
		if( Age % 4 == 0 ) Flash(true);
		else Flash(false);

		if( (Age-LastMis>90) ) {
			FOwner->Muteki = false;
			MissileMuteki = false;
			Flash(false);
		}
	}

}

void TJikiBase::BoostSoon( bool _boost )
{
}

void TJikiBase::BodyControlSoon( s32 Bo, s32 L2, s32 L3, s32 R2, s32 R3 )
{
	s32 Rdiff = Bo-Sprite[HBody]->Rdegree;
	Rdiff = Rdiff & 0x0FFF;
	if( Rdiff>=2047 ) {
		Sprite[HBody]->RotateR(-(4095-Rdiff));
		Sprite[HRArm]->RotateR((4095-Rdiff));
		Sprite[HLLeg2]->RotateR((4095-Rdiff));
		Sprite[HRCover]->RotateR((4095-Rdiff));
		Sprite[HLCover]->RotateR((4095-Rdiff));
		Sprite[HRWing]->RotateR((4095-Rdiff));
		Sprite[HLWing]->RotateR((4095-Rdiff));
	}
	else if( (Rdiff<2047) && (Rdiff>0) ) {
		Sprite[HBody]->RotateR((Rdiff));
		Sprite[HRArm]->RotateR(-(Rdiff));
		Sprite[HLLeg2]->RotateR(-(Rdiff));
		Sprite[HLLeg2]->RotateR(-(Rdiff));
		Sprite[HRCover]->RotateR(-(Rdiff));
		Sprite[HLCover]->RotateR(-(Rdiff));
		Sprite[HRWing]->RotateR(-(Rdiff));
		Sprite[HLWing]->RotateR(-(Rdiff));
	}

	Rdiff = L2-Sprite[HLLeg2]->Rdegree;
	Rdiff = Rdiff & 0x0FFF;
	if( Rdiff>=2047 ) { Sprite[HLLeg2]->RotateR(-(4095-Rdiff)); Sprite[HLLeg3]->RotateR((4095-Rdiff)); }
	else if( (Rdiff<2047) && (Rdiff>0) ) { Sprite[HLLeg2]->RotateR((Rdiff)); Sprite[HLLeg3]->RotateR(-(Rdiff)); }

	Rdiff = L3-Sprite[HLLeg3]->Rdegree;
	Rdiff = Rdiff & 0x0FFF;
	if( Rdiff>=2047 ) Sprite[HLLeg3]->RotateR(-(4095-Rdiff));
	else if( (Rdiff<2047) && (Rdiff>0) ) Sprite[HLLeg3]->RotateR((Rdiff));

	Rdiff = R2-Sprite[HRLeg2]->Rdegree;
	Rdiff = Rdiff & 0x0FFF;
	if( Rdiff>=2047 ) { Sprite[HRLeg2]->RotateR(-(4095-Rdiff)); Sprite[HRLeg3]->RotateR((4095-Rdiff)); }
	else if( (Rdiff<2047) && (Rdiff>0) ) { Sprite[HRLeg2]->RotateR((Rdiff)); Sprite[HRLeg3]->RotateR(-(Rdiff)); }

	Rdiff = R3-Sprite[HRLeg3]->Rdegree;
	Rdiff = Rdiff & 0x0FFF;
	if( Rdiff>=2047 ) Sprite[HRLeg3]->RotateR(-(4095-Rdiff));
	else if( (Rdiff<2047) && (Rdiff>0) ) Sprite[HRLeg3]->RotateR((Rdiff));


}

void TJikiBase::ResetBeforeStart()
{
	
}

void TJikiBase::BoostByOther( bool _boost )
{
	if( _boost )
		BoostLV = 2;
	else
		BoostLV = 4;

	BoostTime = Age;

}

void TJikiBase::Hide()
{

}

//////////////////////////////////////////////////////////
static const s32       HConstForeground=15; 

TJiki::TJiki( TOBJList* owner, s32 _x, s32 _y ) : TJikiBase(owner, _x, _y)
{
	static const s32 ZP = 152;   //これを足せば陽炎の影響を受ける最低Z値になる
	static const s32 ZP2 = 72;
	FObjectName = "TJiki";
	X = _x;
	Y = _y;

	s16 tempX = X;
	s16 tempY = Y;
	HP = FOwner->HeroHP;
	Speed = 5;
	FKind = kHero;
	PrevShot = 0;
	MisOK = true;
	BlinkTime = 30;
	LastBlink = 0;
	BoostLV = 1;
	BoostTime = 0;
	PrevX = 0;
	PrevY = 0;
	NokeTime = 0;
	CrushR = 0;
	BoostR = 0;
	LastMis = 0;
	MissileMuteki = false;

	ShowSpr = 0;
	ZoomRate = 1;
	SlideX = 0;
	SlideY = 0;
	AcceptMoveBody = true;
	FOwner->HeroWeapon  =  0;

	s32 TYCorrect;
	if( FOwner->NowStage == 1 )
		TYCorrect  =  128;
	else
		TYCorrect  =  0;

	for( u32 i = 0; i <= 99; i++ ) {
		ActFlg[i] = 0;
		ActCount[i] = 0;
	}

	//IDの付与
	FID = NewID;
	Inc(NewID);

	for( u32 i = 0; i <= 7; i++ )
		ButtonFlg[i] = false;

	Vapor = new TVaporControl(FOwner,5);
	FOwner->Add(Vapor);

	//  _num, TX, TY, TW, TH,    Z, HX, HY, HW, HH,_cx,_cy
	MakeSprite(FOwner->GetDG(), JikiTex, HBody,  2,1+TYCorrect, 46, 79,27+ZP, 29, 39,  2,  2, 30, 40,false);
	Sprite[HBody]->GetSpr()->Move(tempX+30,tempY+40);

	MakeSprite(FOwner->GetDG(), JikiTex, HRArm, 49,1+TYCorrect, 31, 31,21+ZP,  0,  0,  0,  0, 22, 14,false);
	Sprite[HRArm]->GetSpr()->Move(tempX+31,tempY+25);

	MakeSprite(FOwner->GetDG(), JikiTex, HRHand ,  1,81+TYCorrect, 95, 31,20+ZP,  0,  0,  0,  0, 24,  7,false);
	Sprite[HRHand]->GetSpr()->Move(tempX+12,tempY+38);

	MakeSprite(FOwner->GetDG(), JikiTex, HLArm,129,1+TYCorrect, 15, 31,34+ZP,  0,  0,  0,  0,  5, 11,false);
	Sprite[HLArm]->GetSpr()->Move(tempX+37,tempY+26);

	Sprite[HLArm]->GetSpr()->SetColor(196,ctRed);
	Sprite[HLArm]->GetSpr()->SetColor(196,ctGreen);
	Sprite[HLArm]->GetSpr()->SetColor(196,ctBlue);

	MakeSprite(FOwner->GetDG(), JikiTex, HRLeg1, 81,1+TYCorrect, 31, 31,22+ZP,  0,  0,  0,  0, 17, 15,false);
	Sprite[HRLeg1]->GetSpr()->Move(tempX+25,tempY+44);

	MakeSprite(FOwner->GetDG(), JikiTex, HLLeg1, 81,1+TYCorrect, 31, 31,30+ZP,  0,  0,  0,  0, 17, 15,false);
	Sprite[HLLeg1]->GetSpr()->Move(tempX+31,tempY+44);

	MakeSprite(FOwner->GetDG(), JikiTex, HRLeg2, 113,1+TYCorrect, 15, 31,24+ZP,  0,  0,  0,  0, 10,  9,false);
	Sprite[HRLeg2]->GetSpr()->Move(tempX+24,tempY+47);

	MakeSprite(FOwner->GetDG(), JikiTex, HLLeg2, 113,1+TYCorrect, 15, 31,32+ZP,  0,  0,  0,  0, 10,  9,false);
	Sprite[HLLeg2]->GetSpr()->Move(tempX+30,tempY+47);

	MakeSprite(FOwner->GetDG(), JikiTex, HRLeg3, 49,33+TYCorrect, 63, 47,23+ZP,  0,  0,  0,  0, 38,  9,false);
	Sprite[HRLeg3]->GetSpr()->Move(tempX+18,tempY+62);

	MakeSprite(FOwner->GetDG(), JikiTex, HLLeg3, 49,33+TYCorrect, 63, 47,31+ZP,  0,  0,  0,  0, 38,  9,false);
	Sprite[HLLeg3]->GetSpr()->Move(tempX+24,tempY+62);

	MakeSprite(FOwner->GetDG(), JikiTex, HBuzz,  0,  0+TYCorrect, 64, 64,20+ZP,  0,  0,  63,  63, 32, 32,false);
	Sprite[HBuzz]->GetSpr()->Move(tempX+30,tempY+40);
	Sprite[HBuzz]->GetSpr()->FVisible  =  false;

	MakeSprite(FOwner->GetDG(), JikiTex, HRCover,113,33+TYCorrect, 30, 30,25+ZP,  0,  0,  0,  0, 20, 24,false);
	Sprite[HRCover]->GetSpr()->Move(tempX+21,tempY+15);

	MakeSprite(FOwner->GetDG(), JikiTex, HLCover,113,33+TYCorrect, 30, 30,28+ZP,  0,  0,  0,  0, 20, 24,false);
	Sprite[HLCover]->GetSpr()->Move(tempX+27,tempY+15);

	MakeSprite(FOwner->GetDG(), JikiTex, HRWing,145,33+TYCorrect, 15, 31,26+ZP,  0,  0,  0,  0, 16, 24,false);
	Sprite[HRWing]->GetSpr()->Move(tempX+21,tempY+15);

	MakeSprite(FOwner->GetDG(), JikiTex, HLWing,145,33+TYCorrect, 15, 31,29+ZP,  0,  0,  0,  0, 16, 24,false);
	Sprite[HLWing]->GetSpr()->Move(tempX+27,tempY+15);

	MakeSprite(FOwner->GetDG(), JikiTex, HLHand,113,81+TYCorrect, 47, 15,33+ZP,  0,  0,  0,  0, 11,  4,false);
	Sprite[HLHand]->GetSpr()->Move(tempX+43,tempY+40);

	////////////////////////////////////////////////////////////////////////////////////////////////
	//  _num, TX, TY, TW, TH,    Z, HX, HY, HW, HH,_cx,_cy
	MakeSprite(FOwner->GetDG(), JikiTex, HBody+HConstForeground,  2,1+TYCorrect, 46, 79,27+ZP2,  0,  0,  0,  0, 30, 40,false);
	Sprite[HBody+HConstForeground]->GetSpr()->Move(tempX+30,tempY+40);

	MakeSprite(FOwner->GetDG(), JikiTex, HRArm+HConstForeground, 49,1+TYCorrect, 31, 31,21+ZP2,  0,  0,  0,  0, 22, 14,false);
	Sprite[HRArm+HConstForeground]->GetSpr()->Move(tempX+31,tempY+25);

	MakeSprite(FOwner->GetDG(), JikiTex, HRHand+HConstForeground , 1,81+TYCorrect, 95, 31,20+ZP2,  0,  0,  0,  0, 24,  7,false);
	Sprite[HRHand+HConstForeground]->GetSpr()->Move(tempX+14,tempY+38);

	MakeSprite(FOwner->GetDG(), JikiTex, HLArm+HConstForeground,129,1+TYCorrect, 15, 31,34+ZP2,  0,  0,  0,  0,  5, 11,false);
	Sprite[HLArm+HConstForeground]->GetSpr()->Move(tempX+37,tempY+26);

	Sprite[HLArm+HConstForeground]->GetSpr()->SetColor(196,ctRed);
	Sprite[HLArm+HConstForeground]->GetSpr()->SetColor(196,ctGreen);
	Sprite[HLArm+HConstForeground]->GetSpr()->SetColor(196,ctBlue);

	MakeSprite(FOwner->GetDG(), JikiTex, HRLeg1+HConstForeground, 81,1+TYCorrect, 31, 31,22+ZP2,  0,  0,  0,  0, 17, 15,false);
	Sprite[HRLeg1+HConstForeground]->GetSpr()->Move(tempX+25,tempY+44);

	MakeSprite(FOwner->GetDG(), JikiTex, HLLeg1+HConstForeground, 81,1+TYCorrect, 31, 31,30+ZP2,  0,  0,  0,  0, 17, 15,false);
	Sprite[HLLeg1+HConstForeground]->GetSpr()->Move(tempX+31,tempY+44);

	MakeSprite(FOwner->GetDG(), JikiTex, HRLeg2+HConstForeground, 113,1+TYCorrect, 15, 31,24+ZP2,  0,  0,  0,  0, 10,  9,false);
	Sprite[HRLeg2+HConstForeground]->GetSpr()->Move(tempX+24,tempY+47);

	MakeSprite(FOwner->GetDG(), JikiTex, HLLeg2+HConstForeground, 113,1+TYCorrect, 15, 31,32+ZP2,  0,  0,  0,  0, 10,  9,false);
	Sprite[HLLeg2+HConstForeground]->GetSpr()->Move(tempX+30,tempY+47);

	MakeSprite(FOwner->GetDG(), JikiTex, HRLeg3+HConstForeground, 49,33+TYCorrect, 63, 47,23+ZP2,  0,  0,  0,  0, 38,  9,false);
	Sprite[HRLeg3+HConstForeground]->GetSpr()->Move(tempX+18,tempY+62);

	MakeSprite(FOwner->GetDG(), JikiTex, HLLeg3+HConstForeground, 49,33+TYCorrect, 63, 47,31+ZP2,  0,  0,  0,  0, 38,  9,false);
	Sprite[HLLeg3+HConstForeground]->GetSpr()->Move(tempX+24,tempY+62);

	MakeSprite(FOwner->GetDG(), JikiTex, HRCover+HConstForeground,113,33+TYCorrect, 30, 30,25+ZP2,  0,  0,  0,  0, 20, 24,false);
	Sprite[HRCover+HConstForeground]->GetSpr()->Move(tempX+21,tempY+15);

	MakeSprite(FOwner->GetDG(), JikiTex, HLCover+HConstForeground,113,33+TYCorrect, 30, 30,28+ZP2,  0,  0,  0,  0, 20, 24,false);
	Sprite[HLCover+HConstForeground]->GetSpr()->Move(tempX+27,tempY+15);

	//ダミー
	MakeSprite(FOwner->GetDG(), JikiTex, HRWing+HConstForeground,0,0, 0, 0,0,  0,  0,  0,  0, 0, 0,false);
	Sprite[HRWing+HConstForeground]->GetSpr()->Move(-50,-50);

	//ダミー
	MakeSprite(FOwner->GetDG(), JikiTex, HLWing+HConstForeground,0,0, 0, 0,0,  0,  0,  0,  0, 0, 0,false);
	Sprite[HLWing+HConstForeground]->GetSpr()->Move(-50,-50);

	MakeSprite(FOwner->GetDG(), JikiTex, HLHand+HConstForeground,  113,81+TYCorrect, 47, 15,33+ZP2,  0,  0,  0,  0, 11,  4,false);
	Sprite[HLHand+HConstForeground]->GetSpr()->Move(tempX+43,tempY+40);

	if( FOwner->NowStage != 1 ) {
		for( s32 i = HBody ; i < numSpr; i++ ) {
			switch( i )
			{
			case HLLeg1:
			case HLLeg2:
			case HLLeg3:
			case HLCover:
			case HLWing:
			case HLHand:
			case HLLeg1+HConstForeground:
			case HLLeg2+HConstForeground:
			case HLLeg3+HConstForeground:
			case HLCover+HConstForeground:
			case HLHand+HConstForeground:
				{
					Sprite[i]->GetSpr()->SetColor(196,ctRed);
					Sprite[i]->GetSpr()->SetColor(196,ctGreen);
					Sprite[i]->GetSpr()->SetColor(196,ctBlue);
				}break;
			}
		}
	}

	//  0     1     2     3    4      5      6      7       8      9    10
	//HBody,HRArm,HLArm,HGun,HRLeg1,HRLeg2,HRLeg3,HLLeg1,HLLeg2,HLLeg3,HBuzz

	Sprite[HBody]->Add(Sprite[HRArm]);
	Sprite[HBody]->Add(Sprite[HLArm]);
	Sprite[HRArm]->Add(Sprite[HRHand]);
	Sprite[HBody]->Add(Sprite[HRLeg1]);
	Sprite[HBody]->Add(Sprite[HLLeg1]);
	Sprite[HBody]->Add(Sprite[HRLeg2]);
	Sprite[HBody]->Add(Sprite[HLLeg2]);
	Sprite[HRLeg2]->Add(Sprite[HRLeg3]);
	Sprite[HLLeg2]->Add(Sprite[HLLeg3]);
	Sprite[HBody]->Add(Sprite[HBuzz]);
	Sprite[HBody]->Add(Sprite[HRCover]);
	Sprite[HBody]->Add(Sprite[HLCover]);
	Sprite[HBody]->Add(Sprite[HRWing]);
	Sprite[HBody]->Add(Sprite[HLWing]);
	Sprite[HLArm]->Add(Sprite[HLHand]);

	for( u32 i = HBody ; i <= HLHand ; i++ )
		Sprite[i]->Add(Sprite[i+HConstForeground]);

	Sprite[HLLeg2]->Rotate(-700);
	Sprite[HLLeg3]->RotateR(800);

	Sprite[HRCover]->Rotate(3200);
	Sprite[HLCover]->Rotate(3200);
	Sprite[HRWing]->Rotate(3200);
	Sprite[HLWing]->Rotate(3200);
	Sprite[HLArm]->Rotate(480);
	Sprite[HLHand]->Rotate(208);


	Sprite[HRWing]->GetSpr()->Zoom(0.3f,0.3f);
	Sprite[HLWing]->GetSpr()->Zoom(0.3f,0.3f);


	Sprite[HRArm]->Rotate(256);
	Sprite[HRHand]->Rotate(1000);
	Sprite[HLArm]->Rotate(768);
	Sprite[HLHand]->Rotate(1400);

	for( s32 i = HBody ; i < numSpr; i++ ) {
		Sprite[i]->ZoomR(ZoomRate,ZoomRate);
	}

	Sprite[HBody]->Move(X,Y);


	HitArea = new TJikiHitArea(FOwner, this);
	FOwner->Add(HitArea);

}

TJiki::~TJiki()
{

}

void TJiki::Move()
{
	static const s32 RandomTable[] = {-1,-1,0,1,1,0,-1,0,1,-1};

	ShootFlg = -1;
	if( FOwner->Muteki )
		Sprite[HBody]->MoveR(0,ResilienceY * Speed*2);
	ResilienceX = 0;
	ResilienceY = 0;
	if( ActFlg[HAStart]>0 ) StartEvent();
	else if( ActFlg[HANokezori]>0  ) Nokezori();
	else {
		for( u32 i = 0; i <= 1; i++ ) {
			//両腕を常に定位置に保つ
			MonoRotate(HRArm+HConstForeground*i,0,8);
			MonoRotate(HRHand+HConstForeground*i,0,8);
			MonoRotate(HLArm+HConstForeground*i,Sprite[HBody]->Rdegree+480,8);
			MonoRotate(HLHand+HConstForeground*i,Sprite[HBody]->Rdegree+208,8);
		}
		Input();
	}
	//  PartsMove();


	X = (s32)(Sprite[HBody]->GetSpr()->FCX);
	Y = (s32)(Sprite[HBody]->GetSpr()->FCY);
	FOwner->HeroPos.X = X;
	FOwner->HeroPos.Y = Y;

	//ダメージ無敵やミサイル無敵時の点滅制御
	BlinkControl();

	//各種兵器
	if( FOwner->Weapon ) {
		switch( ShootFlg ){
			case 0:MakeShot(); break;
			case 1:if( ! FOwner->MissileON ) FOwner->Add(new TMakeMissile2(FOwner,MissileStock-FOwner->RestMis)); break;
		}
	}

	//リコイル
	switch( (Age-PrevShot) ){
		case 0:{ Sprite[HRHand]->MoveR(-3,0); Sprite[HRArm]->MoveR(-1,0); Sprite[HBody]->MoveR(-1,0); }	break;
		case 2:{ Sprite[HRHand]->MoveR(3,0);  Sprite[HRArm]->MoveR(1,0);  Sprite[HBody]->MoveR(1,0); }	break;
	}

	//機体を揺らしてスピード感をアピール
	if( (BoostLV == 3) ) Sprite[HBody]->MoveR(RandomTable[Age % 9],RandomTable[9-(Age % 9)]);

	//死亡
	if( FOwner->HeroHP <= 0 ) Die();

	if( (FOwner->LimitTime>0) && (ShieldInfinite) && (! FOwner->ReplayON)  ) HP = HPMax;

	HeroJet();

	PrevX = X - KazeSpeed;
	PrevY = Y;

	HitArea->Place(Sprite[HBody]->GetSpr()->FCX, Sprite[HBody]->GetSpr()->FCY);

	TOBJ::Move();
}

void TJiki::Input()
{
	bool DummyButton[3];
	s32 DummyBType;
	s32 BooltoInt[3];

	static const s32 StickTableX[] = { 1,1,0,-1,-1,-1, 0, 1 };
	static const s32 StickTableY[] = { 0,1,1, 1, 0,-1,-1,-1 };
	static const s32 RdegTable[] = { 0,512,1024,1536,2048,2560,3072,3584 };

	ShootFlg = -1;

	s8 DummyStickY = 0;
	s8 DummyStickX = 0;
	for( u32 i = 0; i <= 2; i++ ) {
		DummyButton[i] = false;
		BooltoInt[i] = 0;
	}

	if( FOwner->ReplayON || FOwner->DemoON ) {
		if( ! FOwner->BossDie ) {
			DummyStickY = FOwner->Replayer->NowStickY;
			DummyStickX = FOwner->Replayer->NowStickX;
			for( u32 i = 0; i <= 2; i++ ) {
				DummyButton[i] = FOwner->Replayer->NowButton[i];
				if( DummyButton[i] ) BooltoInt[i] = 1;
				else BooltoInt[i] = 0;
			}
		}

		if( FOwner->DemoON )
			DummyBType = 0;
		else
			DummyBType = FOwner->Replayer->RepBType;
	}
	else {
		if( ! FOwner->BossDie ) {
			FOwner->Replayer->NowStickY = FOwner->Joy.StickY();
			FOwner->Replayer->NowStickX = FOwner->Joy.StickX();
			DummyStickY = FOwner->Joy.StickY();
			DummyStickX = FOwner->Joy.StickX();

			if( ButtonON[5] ) {
				ButtonON[0] = true;
				ButtonON[2] = true;
			}

			for( u32 i = 0; i <= 2; i++ ) {
				DummyButton[i] = ButtonON[i];
				FOwner->Replayer->NowButton[i] = ButtonON[i];
				if( DummyButton[i] ) BooltoInt[i] = 1;
				else BooltoInt[i] = 0;
			}

		}
		DummyBType = BoostMode;
	}
	SlideX = 0;
	SlideY = 0;

	if( ! FDead ) {

		//移動
		for( u32 i = 0; i <= 7; i++ ) {
			if( (DummyStickX == StickTableX[i]) && (DummyStickY == StickTableY[i]) ) {
				if( AcceptMoveBody ) {
					switch( BoostLV ){
					case 1:
						case 3:BodyControl(BodyTable[i],LLegTable2[i],LLegTable3[i],RLegTable2[i],RLegTable3[i]);	break;
						//逆噴射
						case 4:BodyControl(BodyTable[4]-200,LLegTable2[4]-500,LLegTable3[4]-600,RLegTable2[4]-500,RLegTable3[4]-600);	break;
						//前傾姿勢
						case 2:{
							if( (i==0) || (i==1) || (i==7) ) BodyControl(BodyTable[0],LLegTable2[0],LLegTable3[0],RLegTable2[0],RLegTable3[0]);
							else BodyControl(BodyTable[i],LLegTable2[i],LLegTable3[i],RLegTable2[i],RLegTable3[i]);
						}	break;
					}
				}
				SlideX = RoundOff(abs(DummyStickX)*(cost[RdegTable[i]]*Speed));
				SlideY = RoundOff(abs(DummyStickY)*(sint[RdegTable[i]]*Speed));
				Slide(SlideX,SlideY);
				//if( FOwner->ReplayON ) FOwner->PlayData[age] = RdegTable[i]
				//else FOwner->RecData[age] = RdegTable[i];
				break;
			}
		}

		//ニュートラル時
		if( (DummyStickX == 0) && (DummyStickY == 0) ) {
			if( AcceptMoveBody ) {
				switch( BoostLV ){
					case 1:BodyControl(BodyTable[8],LLegTable2[8],LLegTable3[8],RLegTable2[8],RLegTable3[8]);	break;
					//逆噴射
					case 4:BodyControl(BodyTable[4]-200,LLegTable2[4]-500,LLegTable3[4]-600,RLegTable2[4]-500,RLegTable3[4]-600);	break;
					//前傾姿勢
					case 2:
					case 3:BodyControl(BodyTable[0],LLegTable2[0],LLegTable3[0],RLegTable2[0],RLegTable3[0]);	break;
				}
			}
		}

		if( ! FOwner->BossDie ) {
			for( u32 i = 0; i <= 2; i++ )
				if( ! DummyButton[i] ) ButtonFlg[i] = false;
			//ショット
			if( DummyButton[0] ) {
				if( (FOwner->GameMode == gPlay)) ShootFlg = 0;
				ButtonFlg[0] = true;
			}

			//ショット連射制限
			if( (ShootFlg == 0) && (Age-PrevShot<4) ) ShootFlg = -1;

			//ミサイル
			if( (! ButtonFlg[1]) && DummyButton[1] && (FOwner->RestMis > 0) ) {
				if( (FOwner->GameMode == gPlay) ) {
					ShootFlg = 1;
					FOwner->Muteki = true;
					LastMis = Age;
					MissileMuteki = true;
				}
				ButtonFlg[1] = true;
			}

			//ブーストON/OFF
			switch( DummyBType ){
				case 0:{        //alternate
					if( (! ButtonFlg[2]) && DummyButton[2] ) {
						if( (FOwner->GameMode == gPlay)  ) {
							if( (BoostLV == 1) || (BoostLV == 4) ) {
								BoostLV = 2;
								for( u32 i = 0; i <= 7; i++ ) {
									if( (DummyStickX == StickTableX[i]) && (DummyStickY == StickTableY[i]) ) {
										BoostR = RdegTable[i];
										break;
									}
									else BoostR = 0;
								}
							}
							else BoostLV = 4;
							BoostTime = Age;
						}
						ButtonFlg[2] = true;
					}
				}break;
				case 1:{     //momentary
					if( (! FOwner->DontBoost) && DummyButton[2] ) {
						if( (FOwner->GameMode == gPlay)  ) {
							if( (BoostLV == 1) || (BoostLV == 4) ) {
								BoostLV = 2;
								for( u32 i = 0; i <= 7; i++ ) {
									if( (DummyStickX == StickTableX[i]) && (DummyStickY == StickTableY[i]) ) {
										BoostR = RdegTable[i];
										break;
									}
									else BoostR = 0;
								}
								BoostTime = Age;
							}
						}
					}
					else {
						if( ! ((BoostLV == 1) || (BoostLV == 4)) ) {
							if( (FOwner->GameMode == gPlay)  ) {
								BoostLV = 4;
								BoostTime = Age;
							}
						}
						if( ! DummyButton[2] ) FOwner->DontBoost = false;
					}
				}break;
			}
		}
	}

}

void TJiki::MakeShot()
{
	FOwner->Add(new TShot(FOwner,X+96,Y-12,0,1.0));
	//FOwner->Add(new TBullet(FOwner,X-4,Y+10,1.0));
	FOwner->Add(new TShotFire(FOwner,X+46,Y-3,1024,1.0));
	PrevShot = Age;
	if( CreateSE ) FOwner->SoundEffect->WavPlay(WAGunshot,2);

}

void TJiki::OnDamaged( TOBJ* target )
{
	if( Hit[HBuzz] )
		HitArea->Show();

	if( Hit[HBody] && (! FOwner->Muteki) && (CollisionON || FOwner->ReplayON) ) {
		if( target->STR() > 0 ) {

			Dec(FOwner->HeroHP, target->STR());
			FOwner->Muteki = true;
			LastBlink = Age;
			BlinkTime = 30;
			FOwner->Add(new TBoxContrl(FOwner,4));

			if( (target->Kind() == kEnemy) || (target->STR() >= 300) || (FOwner->IsSequence()) ) {
				BoostLV = 1;
				ActFlg[HANokezori] = 1;
				NokeTime = Age;
				if( target->Kind() == kColBG )
					BlinkTime = 80;
				else
					BlinkTime = 110;
				CrushR = (s32)(atan2(target->X-X, target->Y-Y)/(f32)M_PI*2048.f) & 0x0fff;
				FOwner->Weapon = false;
				if( CreateSE ) FOwner->SoundEffect->WavPlay(WAHdamage,1);
				//      FOwner->TempCount[1] = Age;
				//      FOwner->TempCount[2] = X;
				//      FOwner->TempCount[3] = Y;
				//      FOwner->TempCount[4] = target->X;
				//      FOwner->TempCount[5] = target->Y;
				//      FOwner->TempCount[6] = CrushR;
			}
			else if( CreateSE ) FOwner->SoundEffect->WavPlay(WALdamage,1);
		}

	}
	TOBJ::OnDamaged(target);

}

void TJiki::Nokezori()
{
	s32 NokeSpeed;//のけぞりスピード
	s32 Interval;
	if( (NokeTime-Age+20)>0 ) NokeSpeed = (NokeTime-Age+20) / 2;
	else NokeSpeed = 1;

	Slide((s32)(-cost[CrushR]*NokeSpeed),(s32)(-sint[CrushR]*NokeSpeed));
	BodyControl(4240,3188,3132,3368,3272);

	if( FOwner->IsSequence() )
		Interval  =  60;
	else
		Interval  =  40;

	if( Age-NokeTime>Interval ) {
		ActFlg[HANokezori] = 0;
		FOwner->Weapon = true;
	}
}

void TJiki::Flash( bool _state )
{
	static const s32 GrayArray[] = { HLArm,HLLeg1,HLLeg2,HLLeg3,HLCover,HLHand };
	s32 numGrayArray = sizeof(GrayArray) / sizeof(s32);
	if( _state ){
		for( s32 i = HBody ; i < numSpr; i++ ) {
			Sprite[i]->GetSpr()->SetColor(0,ctRed);
			Sprite[i]->GetSpr()->SetColor(128,ctGreen);
			Sprite[i]->GetSpr()->SetColor(255,ctBlue);
		}
	}
	else{
		//いったん全部白にする
		for( s32 i = HBody ; i < numSpr; i++ ) {
			Sprite[i]->GetSpr()->SetColor(255,ctRed);
			Sprite[i]->GetSpr()->SetColor(255,ctGreen);
			Sprite[i]->GetSpr()->SetColor(255,ctBlue);
		}
		//必要なパーツだけグレーに
		if( FOwner->NowStage != 1 ) {
			for( u32 k = 0; k <= 1; k++ ) {
				for( s32 i = 0 ; i < numGrayArray; i++ ) {
					Sprite[GrayArray[i]+HConstForeground*k]->GetSpr()->SetColor(196,ctRed);
					Sprite[GrayArray[i]+HConstForeground*k]->GetSpr()->SetColor(196,ctGreen);
					Sprite[GrayArray[i]+HConstForeground*k]->GetSpr()->SetColor(196,ctBlue);
				}
			}
		}
	}

}

void TJiki::BoostSoon( bool _boost )
{
	for( u32 i = 0; i <= 1; i++ ) {
		Sprite[HRArm+HConstForeground*i]->Rotate(0);
		Sprite[HRHand+HConstForeground*i]->Rotate(0);
		Sprite[HLArm+HConstForeground*i]->Rotate(Sprite[HBody]->Rdegree+480);
		Sprite[HLHand+HConstForeground*i]->Rotate(Sprite[HBody]->Rdegree+208);
	}

	if( _boost ) {
		BoostLV = 3;
		BodyControlSoon(BodyTable[0],LLegTable2[0],LLegTable3[0],RLegTable2[0],RLegTable3[0]);
	}
	else {
		BoostLV = 1;
		BodyControlSoon(BodyTable[8],LLegTable2[8],LLegTable3[8],RLegTable2[8],RLegTable3[8]);
	}

}

void TJiki::DoStartEvent()
{
	ActFlg[HAStart] = 1;

}

void TJiki::ResetBeforeStart()
{
	Age = 0;
	FDead = false;
	distance = 0;
	LastSlashed = 0;
	Speed = 5;
	FKind = kHero;
	PrevShot = -4;
	MisOK = true;
	BlinkTime = 30;
	LastBlink = 0;
	if( FOwner->NowStage!=3 )
		BoostLV = 1;
	BoostTime = 0;
	PrevX = 0;
	PrevY = 0;
	NokeTime = 0;
	CrushR = 0;
	BoostR = 0;
	LastMis = 0;
	MissileMuteki = false;
	ShowSpr = 0;
	ZoomRate = 1;
	SlideX = 0;
	SlideY = 0;
	AcceptMoveBody = true;
	FOwner->HeroWeapon  =  0;

	for( u32 i = 0; i <= 99; i++ ) {
		ActFlg[i] = 0;
		ActCount[i] = 0;
	}

}

void TJiki::Hide()
{

}

TShot::TShot( TOBJList* owner, s32 sx, s32 sy, s32 R, f32 _zoom ) : TOBJ(owner)
{
	FObjectName = "TShot";
	X = sx;
	Y = sy;

	FSTR = KSTR_SHOT;

	if( _zoom>=1.0 ) {
		FKind = kHShot;
		TSpriteEXBLT sblt;
		sblt.RBM      =  bmSubAdd; // αブレンドモード
		sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
		sblt.RDDDD    =  FOwner->GetDG();     // TDDDDの参照
		sblt.RHeight  =  16;        // 幅
		sblt.RWidth   =  16;        // 高さ
		sblt.RX       =  X;        // X座標
		sblt.RY       =  Y;        // Y座標
		sblt.RZ       =  3000;       // Z値

		Sprite[0] =  new TJoint(sblt);         // スプライトの生成
		Inc(numSpr,1);
		Sprite[0]->GetSpr()->AddTexture(Eff1Tex,1,1,15,15);    // テクスチャを登録
		Sprite[0]->GetSpr()->FVisible  =  true;
		Sprite[0]->GetSpr()->AddHitArea(TPoint(0, -15), TPoint(0, 31), TPoint(15, 31), TPoint(15, -15), htSky);
		Sprite[0]->GetSpr()->FHit      =  true;
		Sprite[0]->GetSpr()->FCX += 16;
		Sprite[0]->GetSpr()->FCY += 20;
	}
	else {
		FKind = kEffect;
		TSpriteEXBLT sblt;
		sblt.RBM      =  bmSubAdd; // αブレンドモード
		sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
		sblt.RDDDD    =  FOwner->GetDG();     // TDDDDの参照
		sblt.RHeight  =  16;        // 幅
		sblt.RWidth   =  16;        // 高さ
		sblt.RX       =  0;        // X座標
		sblt.RY       =  0;        // Y座標
		sblt.RZ       =  20000;       // Z値

		Sprite[0] =  new TJoint(sblt);         // スプライトの生成
		Inc(numSpr,1);
		Sprite[0]->GetSpr()->AddTexture(Eff1Tex,0, 0, 15, 15);    // テクスチャを登録
		Sprite[0]->GetSpr()->FVisible  =  false;
		Sprite[0]->GetSpr()->FCX += 8;
		Sprite[0]->GetSpr()->FCY += 8;
		Sprite[0]->GetSpr()->Move(X,Y);
	}

	Sprite[0]->GetSpr()->Zoom(7.f*_zoom,0.8f*_zoom);
	Sprite[0]->Rotate(R);
	Sprite[0]->GetSpr()->SetColor(64,ctAlpha);

}

void TShot::Move()
{
	X = (s32)(Sprite[0]->GetSpr()->FCX);
	Y = (s32)(Sprite[0]->GetSpr()->FCY);
	Sprite[0]->SlideMoveR(50,0);  //弾の移動

	//if( FOwner->BGSLide!=0 ) Spr.MoveR(0,FOwner->BGSLide);

	//  if( age=1 ) Sprite[0]->GetSpr()->Visible = true;

	if( (X > 640+32) || (Y > 480+32) || (Y <0-32) ) Die();//弾が画面外に消えたら消す

	TOBJ::Move();

}

void TShot::OnDamaged( TOBJ* target )
{
	TOBJ::OnDamaged(target);
	X = (s32)(Sprite[0]->GetSpr()->FCX);
	Y = (s32)(Sprite[0]->GetSpr()->FCY);
	FOwner->Add(new TExpSS(FOwner,X,Y,0,-10));
	//FOwner->Add(new TExpSmall(FOwner,X,Y,0,0));//爆発
	Die();

}

TBFire::TBFire( TOBJList* owner, s32 sx, s32 sy, s32 _R, s32 _LV, f32 _zoom, TOBJ* _obj ) : TOBJ(owner)
{
	FObjectName = "TBFire";
	X = sx;
	Y = sy;
	OwnerOBJ = _obj;
	DisX = OwnerOBJ->X-X;
	DisY = OwnerOBJ->Y-Y;
	Rdeg = _R & 0x0fff;
	FKind = kEffect2;
	ZoomRate  =  _zoom;


	TSpriteEXBLT sblt;
	sblt.RBM      =  bmSubAdd; // αブレンドモード
	sblt.RColor   =  0xFFFFFFFF; // ポリゴン色
	sblt.RDDDD    =  FOwner->GetDG();     // TDDDDの参照
	sblt.RHeight  =  32;        // 幅
	sblt.RWidth   =  32;        // 高さ
	sblt.RX       =  0;        // X座標
	sblt.RY       =  0;        // Y座標
	if( _zoom >= 1.0 )
		sblt.RZ       =  16000;       // Z値
	else
		sblt.RZ       =  22000;

	Sprite[0] =  new TJoint(sblt);         // スプライトの生成
	Inc(numSpr,1);

		for( u32 k = 0; k <= 3; k++ )
			Sprite[0]->GetSpr()->AddTexture(Eff2Tex, k*63, 0, 63+k*64, 63);    // テクスチャを登録
		for( u32 k = 0; k <= 3; k++ )
			Sprite[0]->GetSpr()->AddTexture(Eff2Tex, k*63, 64, 63+k*64, 127);    // テクスチャを登録
		for( u32 k = 0; k <= 3; k++ )
			Sprite[0]->GetSpr()->AddTexture(Eff2Tex, k*63, 128, 63+k*64, 191);    // テクスチャを登録
		Sprite[0]->GetSpr()->FAnim = true;
		Sprite[0]->GetSpr()->FAnimBgnIdx = 5;
		Sprite[0]->GetSpr()->FAnimEndIdx = 11;
		Sprite[0]->GetSpr()->FAnimItv = 3;
		Sprite[0]->GetSpr()->FCX += 20;
		Sprite[0]->GetSpr()->FCY += 16;
		Sprite[0]->GetSpr()->Move(X,Y);
		switch( _LV ){
			case 1:Sprite[0]->GetSpr()->Zoom(1.f,0.7f);	break;
			case 2:Sprite[0]->GetSpr()->Zoom(3.f,1.2f);	break;
			case 3:Sprite[0]->GetSpr()->Zoom(2.f,1.0f);	break;
			case 4:Sprite[0]->GetSpr()->Zoom(3.f,1.0f);	break;
		}
		Sprite[0]->GetSpr()->ZoomR(ZoomRate,ZoomRate);
		Sprite[0]->GetSpr()->Rotate(Rdeg);
		Sprite[0]->GetSpr()->SetColor(96, ctRed);
		Sprite[0]->GetSpr()->SetColor(96, ctBlue);
		Sprite[0]->GetSpr()->SetColor(255, ctGreen);
		Sprite[0]->GetSpr()->SetColor(32,ctAlpha);
		Sprite[0]->GetSpr()->FVisible = true;


}

void TBFire::Move()
{
	if( !OwnerOBJ || OwnerOBJ->Dead() ) {
		Die();
		return;
	}

	Sprite[0]->GetSpr()->AnimNext();
	Sprite[0]->GetSpr()->AnimNext();
	Sprite[0]->Rotate(0);
	Sprite[0]->ZoomR(1.f,0.8f);
	Sprite[0]->Rotate(Rdeg);
	//Sprite[0]->Move((s32)(FOwner->HeroPos.X+cost[Rdeg]*2*age-DisX),(s32)(FOwner->HeroPos.Y+sint[Rdeg]*2*age)-DisY);
	X = OwnerOBJ->X;
	Y = OwnerOBJ->Y;
	Sprite[0]->Move(X-DisX,Y-DisY);
	Sprite[0]->SlideMoveR(-2.f*(f32)Age*ZoomRate,0);

	if( Age>=8 ) {
		Die();
	}
	TOBJ::Move();

}
