#include "Hero2.h"
#include "Effect.h"
#include "TOBJList.h"
#include "Gauge.h"
#include "Missile.h"
#include "Effect.h"
#include "Effect2.h"
#include "Sound.h"

//部品番号
static const s32 HConstForeground = 17;

void TJiki2::OnSlash()
{
	s32 RR  =  Sprite[HBlade]->Rdegree+1024;
	s32 BladeR  =  RR & 0x0fff;
	s32 SX = RoundOff(Sprite[HBlade]->GetSpr()->FCX+76*cost[BladeR]);
	s32 SY = RoundOff(Sprite[HBlade]->GetSpr()->FCY+76*sint[BladeR]);

	s32 level;
	if( ActFlg[HASlash] == 9 ) {
		level  =  0;
		if( CreateSE ) FOwner->SoundEffect->WavPlay2(WASlash_B,12,50);
	}
	else {
		level  =  1;
		if( CreateSE ) FOwner->SoundEffect->WavPlay2(WASlash_A,12,80);
	}

	FOwner->Add(new TSlashedEffect(FOwner, SX,SY, RR,level));

}

void TJiki2::Input()
{
	static const s32 StickTableX[] = { 1,1,0,-1,-1,-1, 0, 1 };
	static const s32 StickTableY[] = { 0,1,1, 1, 0,-1,-1,-1 };
	static const s32 RdegTable[] = { 0,512,1024,1536,2048,2560,3072,3584 };

	static const s32 BodyTable[] = { 184,128,128,3840,3840,3968,3968,256,0 };
	static const s32 LLegTable2[] = { 2996,3052,3052,3188,3188,3492,3492,200,3396 };
	static const s32 LLegTable3[] = { 308,4060,4060,3132,3132,3732,3732,88,100 };
	static const s32 RLegTable2[] = { 560,3472,3472,3368,3368,3728,3728,200,3992 };
	static const s32 RLegTable3[] = { 0,4080,4080,3272,3272,3576,3576,88,3992 };
	static const s32 BladeTable[] = {3100,3600,3600,3600,3100,2600,2600,2600 };
	//BodyControl(184,2996,308,560,0
	ShootFlg = -1;

	s8 DummyStickY = 0;
	s8 DummyStickX = 0;
	bool DummyButton[3];
	s32 DummyBType;
	s32 BooltoInt[3];
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
					case 3:BodyControl(BodyTable[i],LLegTable2[i],LLegTable3[i],RLegTable2[i],RLegTable3[i]);		break;
						//逆噴射
					case 4:BodyControl(BodyTable[4]-200,LLegTable2[4]-500,LLegTable3[4]-600,RLegTable2[4]-500,RLegTable3[4]-600);		break;
						//前傾姿勢
					case 2:{
						if( (i == 0) || (i == 1) || (i == 7) ) BodyControl(184,2996,308,560,0);//BodyControl(BodyTable[0],LLegTable2[0],LLegTable3[0],RLegTable2[0],RLegTable3[0])
						else BodyControl(BodyTable[i],LLegTable2[i],LLegTable3[i],RLegTable2[i],RLegTable3[i]);
						   }break;
					}
				}
				SlideX = RoundOff(abs(DummyStickX)*(cost[RdegTable[i]]*Speed));
				SlideY = RoundOff(abs(DummyStickY)*(sint[RdegTable[i]]*Speed));
				Slide(SlideX,SlideY);
				CycloBladeR = BladeTable[i];
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
				case 3:BodyControl(184,2996,308,560,0);	break;//BodyControl(BodyTable[0],LLegTable2[0],LLegTable3[0],RLegTable2[0],RLegTable3[0]);
				}
			}
			CycloBladeR = 3100;
		}

		if( ! FOwner->BossDie ) {
			for( u32 i = 0; i <= 2; i++ )
				if( ! DummyButton[i] ) ButtonFlg[i] = false;

			//ブレード
			if( DummyButton[0] && (FOwner->GameMode == gPlay)  ) ShootFlg = 0;

			if( (! ButtonFlg[0]) && DummyButton[0] ) {
				ButtonFlg[0] = true;
			}

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
					if( (FOwner->GameMode == gPlay) ) {
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
					if( (FOwner->GameMode == gPlay) ) {
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
				   }
			}
		}
	}

}

void TJiki2::Nokezori()
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
		ForceBladeOff = true;
	}

	if( BladeTrace != NULL ) {
		BladeTrace->Die();
		BladeTrace  =  NULL;
	}

}

void TJiki2::Flash( bool _state )
{
	static const s32 GrayArray[] = { HLLeg1,HLLeg2,HLLeg3,HLCover };
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

void TJiki2::PartsMove()
{
	//for( u32 i = 0; i <= 7; i++ )
	//if( ! ButtonON[i] ) ButtonFlg[i] = false;

	//if( (! ButtonFlg[4]) and ButtonON[4] ) {

	//	Dec(NowSpr);
	//if( NowSpr<1 ) NowSpr = HBlade;

	//ButtonFlg[4] = true;
	//}

	//if( (! ButtonFlg[5]) and ButtonON[5] ) {

	//	Inc(NowSpr);
	//if( NowSpr>HBlade ) NowSpr = 1;

	//ButtonFlg[5] = true;
	//}

	//if( ButtonON[6] ) {
	//	Sprite[NowSpr]->RotateR(8);
	//}

	//if( ButtonON[7] ) {
	//	Sprite[NowSpr]->RotateR(-8);
	//}

	//if( (! ButtonFlg[1]) and ButtonON[1] ) {
	//	SavePose;
	//ButtonFlg[1] = true;
	//}

	//for( u32 i = HBody to numSpr-1 )
	//Sprite[i]->GetSpr()->SetColor(255,ctRed);          //色を戻す

	////  FOwner->TempCount[0] = NowSpr;
	////  FOwner->TempCount[1] = Sprite[NowSpr]->Rdegree;
	//Sprite[NowSpr+HConstForeground]->GetSpr()->SetColor(0,ctRed);


}

void TJiki2::SlashControl()
{
	const s32 BladeSpeed = 2;

	if( BoostLV == 4 ) AcceptMoveBody = true;

	switch( ActFlg[HASlash] ){
	case 0:{
		//通常状態
		PartsRotate(HRArm,Sprite[HBody]->Rdegree+328,0,BladeSpeed);
		PartsRotate(HRHand,Sprite[HBody]->Rdegree+328,0,8);
		PartsRotate(HLArm,Sprite[HBody]->Rdegree+3840,0,8);
		PartsRotate(HLHand,Sprite[HLArm]->Rdegree-2496,0,8);
		PartsRotate(HSheath,Sprite[HBody]->Rdegree+1280,0,8);
		PartsRotate(HBlade,Sprite[HLArm]->Rdegree-2544,0,4);
		ActCount[HASlash] = -1;
		   }break;
	case 1:{
		//        BodyControl(1200,2996+300,308+300,560+300,0+300);
		//抜刀開始
		//FOwner->Add(new TBladeTrace(FOwner,Sprite[HBlade],76));        
		if( (ActCount[HASlash]>=0)) {
			NextSlash = false;
			AcceptMoveBody = false;
			Inc(ActFlg[HASlash]);
			ActCount[HASlash] = -1;
			FSTR  =  KSTR_BLADE;
			Inc(LastSlashed);
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WASwing_A,2);
		}

		if( ForceBladeOff ) {
			ForceBladeOff = false;
			AcceptMoveBody = true;
			ActFlg[HASlash] = 0;
			ActCount[HASlash] = -1;
		}
		   }break;
	case 2:{
		//抜刀中　
		if( BoostLV!=4 ) BodyControl(184,2996,308,560,0);
		PartsRotate(HRHand,Sprite[HBody]->Rdegree+600,0,BladeSpeed*2);
		PartsRotate(HRArm,Sprite[HBody]->Rdegree+800,0,BladeSpeed*2);
		PartsRotate(HLHand,Sprite[HLArm]->Rdegree+632,0,BladeSpeed);
		PartsRotate(HSheath,Sprite[HBody]->Rdegree+1456+128,0,BladeSpeed*2);

		if( (ActCount[HASlash]>=1)) {

			//          if(Sprite[HLArm].Rdegree > (Sprite[HBody]->Rdegree-1416) and 0x0fff) )
			//            Sprite[HLArm]->RotateR(-300);
			PartsRotate(HLArm,Sprite[HBody]->Rdegree-1416,0,BladeSpeed);
			PartsRotate(HBlade,Sprite[HLArm]->Rdegree+24,0,BladeSpeed);
		}

		//左腕のZ値を変更する（描画が頭に重ならないようにする）
		if( ActCount[HASlash] > 3 ) {
			Sprite[HLHand + HConstForeground]->GetSpr()->FZ  =  3300 + 7300;
			Sprite[HBlade + HConstForeground]->GetSpr()->FZ  =  3400 + 7300;
		}

		//剣の軌跡を表示
		if( ActCount[HASlash] == 0 ) {
			BladeTrace  =  new TBladeTrace(FOwner,Sprite[HBlade],76,192,1024);
			FOwner->Add(BladeTrace);
			Sprite[HBlade]->GetSpr()->FHit   =  true;
		}

		//一定時間後orブーストＯＦＦ時に剣の軌跡を消す
		if( ActCount[HASlash] == 18 ) {
			if( BladeTrace != NULL ) BladeTrace->Die();
			BladeTrace  =  NULL;
			Sprite[HBlade]->GetSpr()->FHit   =  false;
		}

		//特定期間内にボタンが押されていれば連続攻撃フラグを立てる
		if( (ActCount[HASlash] >= 7) && (ShootFlg == 0) ) {
			if( BladeTrace != NULL ) BladeTrace->Die();
			BladeTrace  =  NULL;
			NextSlash = true;
			Sprite[HBlade]->GetSpr()->FHit   =  false;
		}

		//連続攻撃フラグが立っていれば2撃目へ
		if( (ActCount[HASlash]>10) && (NextSlash == true) ) {
			NextSlash = false;
			ActFlg[HASlash] = 6;
			ActCount[HASlash] = -1;
			if( BladeTrace != NULL ) { 
				BladeTrace->Die();
				BladeTrace  =  NULL;
				Sprite[HBlade]->GetSpr()->FHit   =  false;
			}
			//2撃目　振りかぶり
			Sprite[HLHand]->GetSpr()->AnimNext();
			Sprite[HLHand + HConstForeground]->GetSpr()->AnimNext();
			Sprite[HBlade]->GetSpr()->AnimNext();
			Sprite[HBlade + HConstForeground]->GetSpr()->AnimNext();
		}

		//行動終了
		if( (ActCount[HASlash]>30) || ForceBladeOff ) {
			if( BladeTrace != NULL ) BladeTrace->Die();
			BladeTrace  =  NULL;
			Sprite[HBlade]->GetSpr()->FHit   =  false;
			ForceBladeOff = false;
			AcceptMoveBody = true;
			Inc(ActFlg[HASlash]);
			ActCount[HASlash] = -1;
		}
		   }break;
	case 3:{
		//剣を収める
		PartsRotate(HRArm,Sprite[HBody]->Rdegree+328,0,BladeSpeed);
		PartsRotate(HRHand,Sprite[HBody]->Rdegree+984,0,4);
		PartsRotate(HLArm,Sprite[HBody]->Rdegree+3376,0,4);
		PartsRotate(HLHand,Sprite[HLArm]->Rdegree-3116,0,4);
		PartsRotate(HSheath,Sprite[HBody]->Rdegree+1040,0,2);
		PartsRotate(HBlade,Sprite[HLHand]->Rdegree+824,0,2);

		if( ActCount[HASlash] == 0 ) {
			Sprite[HLHand]->GetSpr()->FAnimIdx  =  0;
			Sprite[HLHand + HConstForeground]->GetSpr()->FAnimIdx  =  0;
			Sprite[HBlade]->GetSpr()->FAnimIdx  =  0;
			Sprite[HBlade + HConstForeground]->GetSpr()->FAnimIdx  =  0;
		}

		if( ActCount[HASlash]>5 ) {
			Sprite[HLHand + HConstForeground]->GetSpr()->FZ  =  2000 + 7300;
			Sprite[HBlade + HConstForeground]->GetSpr()->FZ  =  2100 + 7300;
			Inc(ActFlg[HASlash]);
			ActCount[HASlash] = -1;
		}
		   }break;
	case 4:{
		//剣を収める
		PartsRotate(HRArm,Sprite[HBody]->Rdegree+328,0,BladeSpeed);
		PartsRotate(HRHand,Sprite[HBody]->Rdegree+328,0,4);
		PartsRotate(HLArm,Sprite[HBody]->Rdegree+3840,0,4);
		PartsRotate(HLHand,Sprite[HLArm]->Rdegree-2496,0,4);
		PartsRotate(HSheath,Sprite[HBody]->Rdegree+1280,0,1);
		PartsRotate(HBlade,Sprite[HLArm]->Rdegree-2544,0,2);

		if( ActCount[HASlash]>10 ) {
			Inc(ActFlg[HASlash]);
			ActCount[HASlash] = -1;
		}
		   }break;
	case 5:{
		ActFlg[HASlash] = 0;
		ActCount[HASlash] = -1;
		   }break;
	case 6:{
		//振り上げポーズ
		//BodyControl(0,2996,308,560,0);
		if( BoostLV!=4 ) BodyControl(-500,3604,308,208,208);
		PartsRotate(HRArm,Sprite[HBody]->Rdegree+100,0,2);
		PartsRotate(HRHand,Sprite[HBody]->Rdegree,0,16);
		PartsRotate(HLArm,Sprite[HBody]->Rdegree+2200,0,2);
		PartsRotate(HLHand,Sprite[HLArm]->Rdegree-100,0,2);
		PartsRotate(HSheath,Sprite[HBody]->Rdegree+952,0,16);
		PartsRotate(HBlade,Sprite[HLHand]->Rdegree-1664,0,2);

		if( ActCount[HASlash]>2 ) {
			Inc(ActFlg[HASlash]);
			ActCount[HASlash] = -1;
			Inc(LastSlashed);
			if( CreateSE ) FOwner->SoundEffect->WavPlay(WASwing_A,2);
		}
		   }break;
	case 7:{
		//振りおろしポーズ
		if( BoostLV!=4 ) BodyControl(208,2996,308,560,0);
		PartsRotate(HRArm,Sprite[HBody]->Rdegree+600,0,BladeSpeed);
		PartsRotate(HRHand,Sprite[HRArm]->Rdegree-100,0,BladeSpeed);
		PartsRotate(HLArm,Sprite[HBody]->Rdegree+56,0,2);
		PartsRotate(HLHand,Sprite[HLArm]->Rdegree+400,0,2);
		PartsRotate(HSheath,Sprite[HBody]->Rdegree+1500,0,BladeSpeed);
		PartsRotate(HBlade,Sprite[HLHand]->Rdegree+2600,0,BladeSpeed);

		//剣の軌跡を表示
		if( ActCount[HASlash] == 0 ) {
			BladeTrace  =  new TBladeTrace(FOwner,Sprite[HBlade],76,192,1024);
			FOwner->Add(BladeTrace);
			Sprite[HBlade]->GetSpr()->FHit   =  true;
		}

		//一定時間後orブーストＯＦＦ時に剣の軌跡を消す
		if( ActCount[HASlash] == 18 ) {
			if( BladeTrace != NULL ) BladeTrace->Die();
			BladeTrace  =  NULL;
			Sprite[HBlade]->GetSpr()->FHit   =  false;
		}

		//特定期間内にボタンが押されていれば連続攻撃フラグを立てる
		if( (ActCount[HASlash] >= 7) && (ShootFlg == 0) ) {
			if( BladeTrace != NULL ) BladeTrace->Die();
			BladeTrace  =  NULL;
			NextSlash = true;
			Sprite[HBlade]->GetSpr()->FHit   =  false;
		}

		//連続攻撃フラグが立っていれば3撃目へ
		if( (ActCount[HASlash]>10) && (NextSlash == true) ) {
			NextSlash = false;
			Inc(ActFlg[HASlash]);
			ActCount[HASlash] = -1;
			AcceptMoveBody = true;
			if( BladeTrace != NULL ) { 
				BladeTrace->Die();
				BladeTrace  =  NULL;
				Sprite[HBlade]->GetSpr()->FHit   =  false;
			}

			//3撃目　振りかぶり
			Sprite[HLHand]->GetSpr()->FAnimIdx  =  0;
			Sprite[HLHand + HConstForeground]->GetSpr()->FAnimIdx  =  0;
			Sprite[HBlade]->GetSpr()->FAnimIdx  =  0;
			Sprite[HBlade + HConstForeground]->GetSpr()->FAnimIdx  =  0;
		}

		if( (ActCount[HASlash]>20) || (ForceBladeOff) ) {
			if( BladeTrace != NULL ) BladeTrace->Die();
			BladeTrace  =  NULL;
			Sprite[HBlade]->GetSpr()->FHit   =  false;
			ForceBladeOff = false;
			AcceptMoveBody = true;
			ActFlg[HASlash] = 10;
			ActCount[HASlash] = -1;
		}
		   }break;
	case 8:{
		//旋風剣準備
		if( BoostLV!=4 ) BodyControl(1200,2996+300,308+300,560+300,0+300);
		PartsRotate(HLHand,Sprite[HLArm]->Rdegree+800,0,4);
		PartsRotate(HLHand,Sprite[HLArm]->Rdegree+1000,0,4);
		PartsRotate(HBlade,Sprite[HLHand]->Rdegree,0,4);

		if( ActCount[HASlash]>2 ) {
			//剣の軌跡作り直し
			BladeTrace  =  new TBladeTrace(FOwner,Sprite[HBlade],76,64,1024);

			Sprite[HBlade]->GetSpr()->FHit   =  true;  
			FOwner->Add(BladeTrace);
			Inc(ActFlg[HASlash]);
			ActCount[HASlash] = -1;
			FSTR  =  5;
			Inc(LastSlashed);
		}
		   }break;
	case 9:{
		//旋風剣
		//この状態のみAcceptMoveBody = true
		PartsRotate(HRArm,Sprite[HBody]->Rdegree+328,0,2);
		PartsRotate(HRHand,Sprite[HBody]->Rdegree+600-184,0,2);
		PartsRotate(HLArm,Sprite[HBody]->Rdegree+2800,0,2);
		//        PartsRotate(HLArm,Sprite[HBody]->Rdegree+CycloBladeR,0,4);
		PartsRotate(HLHand,Sprite[HLArm]->Rdegree+600,0,4);
		PartsRotate(HSheath,Sprite[HBody]->Rdegree+1456,0,2);
		Sprite[HBlade]->RotateR(-600);

		//刀が一周するごとに連続ヒット防止カウンタをインクリメント
		if( ActCount[HASlash] % 7 == 0 ) {
			Inc(LastSlashed);
			if( CreateSE ) FOwner->SoundEffect->WavPlay2(WASwing_B,2,50);
		}


		//特定期間内にボタンが押されていれば連続攻撃フラグを立てる
		if( (ActCount[HASlash] >= 5) && (ShootFlg == 0) ) {
			NextSlash = true;
		}

		if( (ActCount[HASlash]>20) && (NextSlash == true) ) {
			NextSlash = false;
			ActCount[HASlash] = 15;
		}


		if( ((ActCount[HASlash]>22) && (NextSlash == false)) || (ForceBladeOff)  ) {
			ForceBladeOff = false;
			AcceptMoveBody = true;
			ActFlg[HASlash] = 11;
			ActCount[HASlash] = -1;
			if( BladeTrace != NULL ) { 
				BladeTrace->Die();
				BladeTrace  =  NULL;
				Sprite[HBlade]->GetSpr()->FHit   =  false;
			}
		}
		   }break;
	case 10:{
		//剣を納める（その２）

		PartsRotate(HRHand,Sprite[HBody]->Rdegree+600,0,4);
		PartsRotate(HRArm,Sprite[HBody]->Rdegree+600,0,4);   
		PartsRotate(HLHand,Sprite[HLArm]->Rdegree+632,0,2);
		PartsRotate(HLArm,Sprite[HBody]->Rdegree-1416,0,4);
		PartsRotate(HSheath,1456,0,2);
		PartsRotate(HBlade,Sprite[HLArm]->Rdegree+24,0,4);

		if( ActCount[HASlash] == 0 ) {
			Sprite[HLHand]->GetSpr()->FAnimIdx  =  0;
			Sprite[HLHand + HConstForeground]->GetSpr()->FAnimIdx  =  0;
			Sprite[HBlade]->GetSpr()->FAnimIdx  =  0;
			Sprite[HBlade + HConstForeground]->GetSpr()->FAnimIdx  =  0;
		}

		if( ActCount[HASlash]>2 ) {
			Sprite[HLHand + HConstForeground]->GetSpr()->FZ  =  2000 + 7300;
			Sprite[HBlade + HConstForeground]->GetSpr()->FZ  =  2100 + 7300;
			ActFlg[HASlash] = 3;
			ActCount[HASlash] = -1;
		}
			}break;
	case 11:{
		//剣を納める（その２）
		if( BoostLV!=4 ) BodyControl(500,2996+300,308+300,560+300,0+300);
		PartsRotate(HRHand,Sprite[HBody]->Rdegree+600,0,4);
		PartsRotate(HRArm,Sprite[HBody]->Rdegree+600,0,4);   
		PartsRotate(HLHand,Sprite[HLArm]->Rdegree+632,0,2);
		PartsRotate(HLArm,Sprite[HBody]->Rdegree-1416,0,4);
		PartsRotate(HSheath,1456,0,2);
		PartsRotate(HBlade,Sprite[HLArm]->Rdegree+24,0,4);

		if( ActCount[HASlash] == 0 ) {
			Sprite[HLHand]->GetSpr()->FAnimIdx  =  0;
			Sprite[HLHand + HConstForeground]->GetSpr()->FAnimIdx  =  0;
			Sprite[HBlade]->GetSpr()->FAnimIdx  =  0;
			Sprite[HBlade + HConstForeground]->GetSpr()->FAnimIdx  =  0;
		}

		if( ActCount[HASlash]>2 ) {
			Sprite[HLHand + HConstForeground]->GetSpr()->FZ  =  2000 + 7300;
			Sprite[HBlade + HConstForeground]->GetSpr()->FZ  =  2100 + 7300;
			ActFlg[HASlash] = 12;
			ActCount[HASlash] = -1;
		}
			}break;
	case 12:{
		//剣を収める（その３）
		PartsRotate(HRArm,Sprite[HBody]->Rdegree+328,0,2);
		PartsRotate(HRHand,Sprite[HBody]->Rdegree+984,0,2);
		PartsRotate(HLArm,Sprite[HBody]->Rdegree+3376,0,2);
		PartsRotate(HLHand,Sprite[HLArm]->Rdegree-3116,0,2);
		PartsRotate(HSheath,Sprite[HBody]->Rdegree+1040,0,1);
		PartsRotate(HBlade,Sprite[HLHand]->Rdegree+824,0,1);

		if( ActCount[HASlash]>2 ) {
			Inc(ActFlg[HASlash]);
			ActCount[HASlash] = -1;
		}
			}break;
	case 13:{
		//剣を収める （その３）
		PartsRotate(HRArm,Sprite[HBody]->Rdegree+328,0,2);
		PartsRotate(HRHand,Sprite[HBody]->Rdegree+328,0,4);
		PartsRotate(HLArm,Sprite[HBody]->Rdegree+3840,0,2);
		PartsRotate(HLHand,Sprite[HLArm]->Rdegree-2496,0,4);
		PartsRotate(HSheath,Sprite[HBody]->Rdegree+1280,0,1);
		PartsRotate(HBlade,Sprite[HLArm]->Rdegree-2544,0,1);                                                
		if( ActCount[HASlash]>15 ) {
			ActFlg[HASlash] = 5;
			ActCount[HASlash] = -1;
		}
			}
	}
	Inc(ActCount[HASlash]);
}

void TJiki2::PartsRotate( s32 _sprnum, s32 _dist, s32 _pn, s32 _speed )
{
	s32 Rdiff = _dist-Sprite[_sprnum]->Rdegree;
	switch( _pn ){
	case -1:{
		Sprite[_sprnum]->RotateR(-ceil(abs(Rdiff)/_speed)+Sign(Rdiff)*1);
			}break;
	case 0 :{
		if( abs(Rdiff)>=2048 ) Sprite[_sprnum]->RotateR(-Sign(Rdiff)*ceil((4095-abs(Rdiff))/_speed)-Sign(Rdiff)*1);
		else if( abs(Rdiff)<2048 ) Sprite[_sprnum]->RotateR(ceil((Rdiff)/_speed)+Sign(Rdiff)*1);
			}break;
	case 1:{
		Sprite[_sprnum]->RotateR(ceil(abs(Rdiff)/_speed)+Sign(Rdiff)*1);
		   }break;
	}
}

void TJiki2::SavePose()
{
	//FName: String;
	//WriteLine: String;
	//WriteFile: TextFile;
	//FName  =  "jikipose.txt";
	//
	//	// 姿勢データ書き込み
	//	AssignFile( WriteFile, FName );
	//	Rewrite( WriteFile );
	//	//全パーツの現在角度を出力
	//	for( u32 i = HBody to HBlade ) { 
	//WriteLine  =  IntToStr(i) + " , " + IntToStr(Sprite[i]->Rdegree);
	//
	//	Writeln( WriteFile, WriteLine );
	//	}
	//
	//	CloseFile( WriteFile );

}

void TJiki2::SplashControl()
{

}

void TJiki2::OnDamaged( TOBJ* target )
{
	if( Hit[HBuzz] )
		HitArea->Show();

	if( Hit[HBody] && (! FOwner->Muteki) && (CollisionON || FOwner->ReplayON) ) {
		if( target->STR() > 0 ) {
			Dec(FOwner->HeroHP,target->STR());
			//FOwner->TotalHit = 0;
			FOwner->Muteki = true;
			LastBlink = Age;
			BlinkTime = 30;
			FOwner->Add(new TBoxContrl(FOwner,4));

			if( (target->Kind() == kEnemy) || (target->STR() >= 300) || (FOwner->IsSequence())  ) {
				BoostLV = 1;
				ActFlg[HANokezori] = 1;
				NokeTime = Age;
				if( target->Kind() == kColBG )
					BlinkTime = 80;
				else
					BlinkTime = 110;
				Sprite[HBlade]->GetSpr()->FHit   =  false;
				CrushR = (s32)(atan2(target->X-X,target->Y-Y)/(f32)M_PI*2048.f) & 0x0fff;
				FOwner->Weapon = false;
				if( CreateSE ) FOwner->SoundEffect->WavPlay(WAHdamage,1);
			}
			else if( CreateSE ) FOwner->SoundEffect->WavPlay(WALdamage,1);
		}
	}

	//剣ヒット時
	if( Hit[HitBlade] ) {
		if( ((target->Kind() == kEnemy) || (target->Kind() == kEMissile)) && (target->LastSlashed < LastSlashed) ) OnSlash();
	}
	TJikiBase::OnDamaged(target);

}

void TJiki2::Die()
{
	if( BladeTrace != NULL ) BladeTrace->Die();
	BladeTrace = NULL;
	TJikiBase::Die();
}

TJiki2::TJiki2( TOBJList* owner, s32 _x, s32 _y ) : TJikiBase(owner, _x, _y)
{
	const s32 ZP = 152;   //これを足せば陽炎の影響を受ける最低Z値になる
	const s32 ZP2= 73;
	FObjectName = "TJiki2";

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
	ZoomRate = 1.f;
	SlideX = 0;
	SlideY = 0;
	NowSpr = 0;
	AcceptMoveBody = true;
	FSTR  =  KSTR_BLADE;     //刀の攻撃力として使用
	ForceBladeOff = false;
	FOwner->HeroWeapon  =  1;

	s32 TYCorrect;
	if( FOwner->NowStage == 1 )
		TYCorrect  =  128;
	else
		TYCorrect  =  0;

	for( u32 i = 0; i <= 99; i++ ) {
		ActFlg[i] = 0;
		ActCount[i] = 0;
	}

	Vapor = new TVaporControl(FOwner,5);
	FOwner->Add(Vapor);

	//IDの付与
	FID = NewID;
	Inc(NewID);

	for( u32 i = 0; i <= 7; i++ )
		ButtonFlg[i] = false;


	//  _num, TX, TY, TW, TH,    Z, HX, HY, HW, HH,_cx,_cy
	MakeSprite(QD, JikiTex, HBody,  2,1+TYCorrect, 46, 79,27+ZP, 29, 39,  2,  2, 30, 40,false);
	Sprite[HBody]->GetSpr()->Move(tempX+30,tempY+40);

	MakeSprite(QD, JikiTex, HRArm, 49,1+TYCorrect, 31, 31,21+ZP,  0,  0,  0,  0, 22, 14,false);
	Sprite[HRArm]->GetSpr()->Move(tempX+31,tempY+25);

	MakeSprite(QD, JikiTex, HRHand ,  161,33+TYCorrect, 63, 31,20+ZP,  0,  0,  0,  0, 24,  7,false);
	Sprite[HRHand]->GetSpr()->Move(tempX+14,tempY+38);

	MakeSprite(QD, JikiTex, HLArm,129,1+TYCorrect, 15, 31,34+ZP,  0,  0,  0,  0,  5, 11,false);
	Sprite[HLArm]->GetSpr()->Move(tempX+39,tempY+24);

	Sprite[HLArm]->GetSpr()->SetColor(196,ctRed);
	Sprite[HLArm]->GetSpr()->SetColor(196,ctGreen);
	Sprite[HLArm]->GetSpr()->SetColor(196,ctBlue);

	MakeSprite(QD, JikiTex, HRLeg1, 81,1+TYCorrect, 31, 31,22+ZP,  0,  0,  0,  0, 17, 15,false);
	Sprite[HRLeg1]->GetSpr()->Move(tempX+25,tempY+44);

	MakeSprite(QD, JikiTex, HLLeg1, 81,1+TYCorrect, 31, 31,30+ZP,  0,  0,  0,  0, 17, 15,false);
	Sprite[HLLeg1]->GetSpr()->Move(tempX+31,tempY+44);

	MakeSprite(QD, JikiTex, HRLeg2, 113,1+TYCorrect, 15, 31,24+ZP,  0,  0,  0,  0, 10,  9,false);
	Sprite[HRLeg2]->GetSpr()->Move(tempX+24,tempY+47);

	MakeSprite(QD, JikiTex, HLLeg2, 113,1+TYCorrect, 15, 31,32+ZP,  0,  0,  0,  0, 10,  9,false);
	Sprite[HLLeg2]->GetSpr()->Move(tempX+30,tempY+47);

	MakeSprite(QD, JikiTex, HRLeg3, 49,33+TYCorrect, 63, 47,23+ZP,  0,  0,  0,  0, 38,  9,false);
	Sprite[HRLeg3]->GetSpr()->Move(tempX+18,tempY+62);

	MakeSprite(QD, JikiTex, HLLeg3, 49,33+TYCorrect, 63, 47,31+ZP,  0,  0,  0,  0, 38,  9,false);
	Sprite[HLLeg3]->GetSpr()->Move(tempX+24,tempY+62);

	MakeSprite(QD, JikiTex, HBuzz,  0,  0, 64, 64,20+ZP,  0,  0,  63,  63, 32, 32,false);
	Sprite[HBuzz]->GetSpr()->Move(tempX+30,tempY+40);

	Sprite[HBuzz]->GetSpr()->FVisible  =  false;

	MakeSprite(QD, JikiTex, HRCover,113,33+TYCorrect, 30, 30,25+ZP,  0,  0,  0,  0, 20, 24,false);
	Sprite[HRCover]->GetSpr()->Move(tempX+21,tempY+15);

	MakeSprite(QD, JikiTex, HLCover,113,33+TYCorrect, 30, 30,28+ZP,  0,  0,  0,  0, 20, 24,false);
	Sprite[HLCover]->GetSpr()->Move(tempX+27,tempY+15);

	MakeSprite(QD, JikiTex, HRWing,145,33+TYCorrect, 15, 31,26+ZP,  0,  0,  0,  0, 16, 24,false);
	Sprite[HRWing]->GetSpr()->Move(tempX+21,tempY+15);

	MakeSprite(QD, JikiTex, HLWing,145,33+TYCorrect, 15, 31,29+ZP,  0,  0,  0,  0, 16, 24,false);
	Sprite[HLWing]->GetSpr()->Move(tempX+27,tempY+15);

	MakeSprite(QD, JikiTex, HLHand,  161,33+TYCorrect, 63, 31,33+ZP,  0,  0,  0,  0, 24,  7,false);
	Sprite[HLHand]->GetSpr()->AddTexture(JikiTex, 161,33+32+TYCorrect, 161+63, 33+TYCorrect+32+31);        //振り下ろし時の腕パターン
	Sprite[HLHand]->GetSpr()->Move(tempX+45,tempY+38);
	Sprite[HLHand]->GetSpr()->FAnim = true;

	MakeSprite(QD, JikiTex, HSheath,  241,17+TYCorrect, 15, 95,21+ZP,  0,  0,  0,  0, 7,  22,false);
	Sprite[HSheath]->GetSpr()->Move(tempX+32,tempY+43);

	//  MakeSprite(QD, JikiTex, HBlade,  225,1+TYCorrect, 15, 111,34+ZP,  -48,  32,  64,  96, 7,  24,false);
	MakeSprite(QD, JikiTex, HBlade,  225,1+TYCorrect, 15, 111,34+ZP,  0,  0,  0,  0, 7,  24,false);
	Sprite[HBlade]->GetSpr()->AddHitArea( TPoint(-96, 32), TPoint(-48, 32+80), TPoint(-48+64, 32+80),TPoint(-48+64, 32), htSky);
	Sprite[HBlade]->GetSpr()->FHit   =  false;
	Sprite[HBlade]->GetSpr()->AddTexture(JikiTex, 239,1+TYCorrect, 225, 111+TYCorrect);        //反転パターン
	Sprite[HBlade]->GetSpr()->Move(tempX+63,tempY+46);
	Sprite[HBlade]->GetSpr()->FAnim = true;

	////////////////////////////////////////////////////////////////////////////////////////////////
	//  _num, TX, TY, TW, TH,    Z, HX, HY, HW, HH,_cx,_cy
	MakeSprite(QD, JikiTex, HBody+HConstForeground,  2,1+TYCorrect, 46, 79,27+ZP2,  0,  0,  0,  0, 30, 40,false);
	Sprite[HBody+HConstForeground]->GetSpr()->Move(tempX+30,tempY+40);

	MakeSprite(QD, JikiTex, HRArm+HConstForeground, 49,1+TYCorrect, 31, 31,21+ZP2,  0,  0,  0,  0, 22, 14,false);
	Sprite[HRArm+HConstForeground]->GetSpr()->Move(tempX+31,tempY+25);

	MakeSprite(QD, JikiTex, HRHand+HConstForeground ,  161,33+TYCorrect, 63, 31,19+ZP2,  0,  0,  0,  0, 24,  7,false);
	Sprite[HRHand+HConstForeground]->GetSpr()->Move(tempX+14,tempY+38);

	MakeSprite(QD, JikiTex, HLArm+HConstForeground,129,1+TYCorrect, 15, 31,34+ZP2,  0,  0,  0,  0,  5, 11,false);
	Sprite[HLArm+HConstForeground]->GetSpr()->Move(tempX+39,tempY+24);

	Sprite[HLArm+HConstForeground]->GetSpr()->SetColor(196,ctRed);
	Sprite[HLArm+HConstForeground]->GetSpr()->SetColor(196,ctGreen);
	Sprite[HLArm+HConstForeground]->GetSpr()->SetColor(196,ctBlue);

	MakeSprite(QD, JikiTex, HRLeg1+HConstForeground, 81,1+TYCorrect, 31, 31,22+ZP2,  0,  0,  0,  0, 17, 15,false);
	Sprite[HRLeg1+HConstForeground]->GetSpr()->Move(tempX+25,tempY+44);

	MakeSprite(QD, JikiTex, HLLeg1+HConstForeground, 81,1+TYCorrect, 31, 31,30+ZP2,  0,  0,  0,  0, 17, 15,false);
	Sprite[HLLeg1+HConstForeground]->GetSpr()->Move(tempX+31,tempY+44);

	MakeSprite(QD, JikiTex, HRLeg2+HConstForeground, 113,1+TYCorrect, 15, 31,24+ZP2,  0,  0,  0,  0, 10,  9,false);
	Sprite[HRLeg2+HConstForeground]->GetSpr()->Move(tempX+24,tempY+47);

	MakeSprite(QD, JikiTex, HLLeg2+HConstForeground, 113,1+TYCorrect, 15, 31,32+ZP2,  0,  0,  0,  0, 10,  9,false);
	Sprite[HLLeg2+HConstForeground]->GetSpr()->Move(tempX+30,tempY+47);

	MakeSprite(QD, JikiTex, HRLeg3+HConstForeground, 49,33+TYCorrect, 63, 47,23+ZP2,  0,  0,  0,  0, 38,  9,false);
	Sprite[HRLeg3+HConstForeground]->GetSpr()->Move(tempX+18,tempY+62);

	MakeSprite(QD, JikiTex, HLLeg3+HConstForeground, 49,33+TYCorrect, 63, 47,31+ZP2,  0,  0,  0,  0, 38,  9,false);
	Sprite[HLLeg3+HConstForeground]->GetSpr()->Move(tempX+24,tempY+62);

	MakeSprite(QD, JikiTex, HRCover+HConstForeground,113,33+TYCorrect, 30, 30,25+ZP2,  0,  0,  0,  0, 20, 24,false);
	Sprite[HRCover+HConstForeground]->GetSpr()->Move(tempX+21,tempY+15);

	MakeSprite(QD, JikiTex, HLCover+HConstForeground,113,33+TYCorrect, 30, 30,28+ZP2,  0,  0,  0,  0, 20, 24,false);
	Sprite[HLCover+HConstForeground]->GetSpr()->Move(tempX+27,tempY+15);

	//ダミー
	MakeSprite(QD, JikiTex, HRWing+HConstForeground,0,0, 0, 0,0,  0,  0,  0,  0, 0, 0,false);
	Sprite[HRWing+HConstForeground]->GetSpr()->Move(-50,-50);

	//ダミー
	MakeSprite(QD, JikiTex, HLWing+HConstForeground,0,0, 0, 0,0,  0,  0,  0,  0, 0, 0,false);
	Sprite[HLWing+HConstForeground]->GetSpr()->Move(-50,-50);

	MakeSprite(QD, JikiTex, HLHand+HConstForeground,  161,33+TYCorrect, 63, 31,20+ZP2,  0,  0,  0,  0, 24,  7,false);
	//MakeSprite(QD, JikiTex, HLHand+HConstForeground,  161,33+TYCorrect+32, 63, 31,33+ZP2,  0,  0,  0,  0, 24,  7,false);
	Sprite[HLHand+HConstForeground]->GetSpr()->AddTexture(JikiTex, 161,33+TYCorrect+32, 161+63, 33+TYCorrect+32+31);        //振り下ろし時の腕パターン
	Sprite[HLHand+HConstForeground]->GetSpr()->Move(tempX+45,tempY+38);
	Sprite[HLHand+HConstForeground]->GetSpr()->FAnim = true;

	MakeSprite(QD, JikiTex, HSheath+HConstForeground,  241,17+TYCorrect, 15, 95,20+ZP2,  0,  0,  0,  0, 7,  22,false);
	Sprite[HSheath+HConstForeground]->GetSpr()->Move(tempX+32,tempY+43); 

	MakeSprite(QD, JikiTex, HBlade+HConstForeground,  225,1+TYCorrect, 15, 111,21+ZP2,  0,  0,  0,  0, 7,  24,false);
	Sprite[HBlade+HConstForeground]->GetSpr()->AddTexture(JikiTex, 239,1+TYCorrect, 225, 111+TYCorrect);        //反転パターン
	Sprite[HBlade+HConstForeground]->GetSpr()->Move(tempX+63,tempY+46);
	Sprite[HBlade+HConstForeground]->GetSpr()->FAnim = true;

	//if( FOwner->NowStage != 1 ) {
	for( u32 i = HBody ; i < numSpr; i++) {
		switch( i ){
		case HLLeg1:
		case HLLeg2:
		case HLLeg3:
		case HLCover:
		case HLWing:
		case HLLeg1+HConstForeground:
		case HLLeg2+HConstForeground:
		case HLLeg3+HConstForeground:
		case HLCover+HConstForeground:
			{
				Sprite[i]->GetSpr()->SetColor(196,ctRed);
				Sprite[i]->GetSpr()->SetColor(196,ctGreen);
				Sprite[i]->GetSpr()->SetColor(196,ctBlue);
			}break;
		}
	}
	//}

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
	Sprite[HRHand]->Add(Sprite[HSheath]);
	Sprite[HLHand]->Add(Sprite[HBlade]);

	for( u32 i = HBody ; i <= HBlade; i++ )
		Sprite[i]->Add(Sprite[i+HConstForeground]);

	Sprite[HLLeg2]->Rotate(-700);
	Sprite[HLLeg3]->RotateR(800);

	Sprite[HRCover]->Rotate(3200);
	Sprite[HLCover]->Rotate(3200);
	Sprite[HRWing]->Rotate(3200);
	Sprite[HLWing]->Rotate(3200);
	Sprite[HLArm]->Rotate(480);
	Sprite[HLHand]->Rotate(208);


	Sprite[HRWing]->GetSpr()->Zoom(0.3,0.3);
	Sprite[HLWing]->GetSpr()->Zoom(0.3,0.3);


	Sprite[HRArm]->Rotate(328);
	Sprite[HRHand]->Rotate(328);
	Sprite[HLArm]->Rotate(3840);
	Sprite[HLHand]->Rotate(1344);
	Sprite[HSheath]->Rotate(1280);
	Sprite[HBlade]->Rotate(1296);

	for( s32 i = HBody ; i < numSpr; i++ ) {
			Sprite[i]->ZoomR(ZoomRate,ZoomRate);
	}

	Sprite[HBody]->Move(X,Y);

	HitArea = new TJikiHitArea( FOwner, this);
	FOwner->Add( HitArea );

}

TJiki2::~TJiki2()
{

}

void TJiki2::Move()
{
	static const s32 RandomTable[] = { -1,-1,0,1,1,0,-1,0,1,-1 };

	ShootFlg = -1;
	if( FOwner->Muteki ) Sprite[HBody]->MoveR(0,ResilienceY * Speed*2);
	ResilienceX = 0;
	ResilienceY = 0;
	if( ActFlg[HAStart] > 0 ) StartEvent();
	else if( ActFlg[HANokezori] > 0  ) Nokezori();
	else {
		Input();
		SlashControl();
	}

	X = (s32)(Sprite[HBody]->GetSpr()->FCX);
	Y = (s32)(Sprite[HBody]->GetSpr()->FCY);
	FOwner->HeroPos.X = X;
	FOwner->HeroPos.Y = Y;

	//ダメージ無敵やミサイル無敵時の点滅制御
	BlinkControl();

	//海ステージだったら水しぶき制御
	//  if( FOwner->NowStage = 1 ) SplashControl;

	//各種兵器
	if( FOwner->Weapon ) {
		switch( ShootFlg ){
			case 0:{
				if( (ActFlg[HASlash] == 0) ) ActFlg[HASlash] = 1;
			}break;
			//1:FOwner->LockOn(MissileStock-FOwner->RestMis);
			case 1:if( ! FOwner->MissileON ) FOwner->Add(new TMakeMissile2(FOwner,MissileStock-FOwner->RestMis)); break;
		}
	}


	//機体を揺らしてスピード感をアピール
	if( (BoostLV == 3) ) Sprite[HBody]->MoveR(RandomTable[Age % 9],RandomTable[9-(Age % 9)]);
	//if( BoostLV=3 ) Sprite[HBody]->MoveR(random(3)-1,random(3)-1);

	//死亡
	if( FOwner->HeroHP<=0 ) Die();

	if( (FOwner->LimitTime>0) && (ShieldInfinite) && (! FOwner->ReplayON) ) HP = HPMax;

	HeroJet();

	//FOwner->TempCount[2] = BoostLV;
	PrevX = X-KazeSpeed;
	PrevY = Y;

	HitArea->Place(Sprite[HBody]->GetSpr()->FCX, Sprite[HBody]->GetSpr()->FCY);

	TJikiBase::Move();

}

void TJiki2::BoostSoon( bool _boost )
{
	if( _boost ) {
		BoostLV = 3;
		BodyControlSoon(BodyTable[0],LLegTable2[0],LLegTable3[0],RLegTable2[0],RLegTable3[0]);
	}
	else {
		BoostLV = 1;
		BodyControlSoon(BodyTable[8],LLegTable2[8],LLegTable3[8],RLegTable2[8],RLegTable3[8]);
	}
}

void TJiki2::ResetBeforeStart()
{
	Age = 0;
	FDead = false;
	distance = 0;
	LastSlashed = 0;
	Speed = 5;
	FKind = kHero;
	PrevShot = 0;
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
	NowSpr = 0;
	AcceptMoveBody = true;
	FSTR  =  KSTR_BLADE;     //刀の攻撃力として使用
	ForceBladeOff = false;
	FOwner->HeroWeapon  =  1;

	for( u32 i = 0; i <= 99; i++ ) {
		ActFlg[i] = 0;
		ActCount[i] = 0;
	}

}

