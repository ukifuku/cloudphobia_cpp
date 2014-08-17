#include "SpriteEXM.h"
#include <algorithm>

TSpriteEX* TSpriteEXManager::GetFSprList()
{
	return (TSpriteEX*)FSprList[0];
}

void TSpriteEXManager::SetFColorKey( bool _flg )
{
	FColorKey  =  _flg;
}

void TSpriteEXManager::MakeBlurInternal()
{
	f32 UVFactor  =  1.0f;
	//BltRect  =  Rect(0, 0, 256, 127);
	//作業用スプライト頂点作成
	D3DTLVERTEX2 TmpTLV[6];
	TmpTLV[0] = MakeTLV(          0.f, 		   0.f,   1.f, 1.f, 0x80FFFFFF, 0,   0.f,	0.f, 0.f, 0.f);
	TmpTLV[1] = MakeTLV((f32)FBlurTex1->GetWidth(), 	           0.f,  	1.f, 1.f, 0x80FFFFFF, 0,   UVFactor,   0.f, 0.f, 0.f);
	TmpTLV[2] = MakeTLV(          0, 	(f32)FBlurTex1->GetHeight(), 	1.f, 1.f, 0x80FFFFFF, 0,   0.f,   UVFactor, 0.f, 0.f);
	TmpTLV[3] = MakeTLV((f32)FBlurTex1->GetWidth(),	           0.f, 	1.f, 1.f, 0x80FFFFFF, 0,   UVFactor,   0.f, 0.f, 0.f);
	TmpTLV[4] = MakeTLV(          0,	        (f32)FBlurTex1->GetHeight(), 	1.f, 1.f, 0x80FFFFFF, 0,   0.f,   UVFactor, 0.f, 0.f);
	TmpTLV[5] = MakeTLV((f32)FBlurTex1->GetWidth(),	(f32)FBlurTex1->GetHeight(), 	1.f, 1.f, 0x80FFFFFF, 0,   UVFactor,   UVFactor, 0.f, 0.f);

	//バックバッファの内容をコピー
	FDDDD->Get3DDevice()->StretchRect(FDDDD->GetBackBuffers(0),NULL,FBlurRendTarget1,NULL,D3DTEXF_NONE);
	FDDDD->Get3DDevice()->StretchRect(FDDDD->GetBackBuffers(0),NULL,FBlurRendTarget2,NULL,D3DTEXF_NONE);

	//頂点バッファ作成
	TDGVertexBuffer* vertBuff  = new TDGVertexBuffer(FDDDD,6,sizeof(D3DTLVERTEX2),D3DUSAGE_DYNAMIC,SprFVF,D3DPOOL_DEFAULT,"");

	D3DTLVERTEX2* pV = (D3DTLVERTEX2*)vertBuff->Lock(0,0,D3DLOCK_NOOVERWRITE);

	// 頂点のトランスフォーム
	for( u32 i  =  0 ; i <= 5 ; i++) {
		// 平行移動
		D3DTLVERTEX2 SetTLV  =  TmpTLV[i];
		SetTLV.sx  =  TmpTLV[i].sx / FaspX - FX * FaspX;
		SetTLV.sy  =  TmpTLV[i].sy / FaspY - FY * FaspY;

		// 回転（カメラ向きと逆方向に回転させる）＆スケーリング
		u16 r  =  (PI_ZERO - FR) & 0x0FFF;
		exRotate(&SetTLV.sx, &SetTLV.sy, FCX, FCY, SetTLV.sx, SetTLV.sy, r);
		exZoom(&SetTLV.sx, &SetTLV.sy, FCX, FCY, SetTLV.sx, SetTLV.sy, FZX * FaspX, FZY * FaspY);

		memmove(pV, &SetTLV, vertBuff->GetVertexSize());
		pV++;
	}

	vertBuff->Unlock();


	FDDDD->Get3DDevice()->SetStreamSource(0,vertBuff->GetVertexBuffer(),0,vertBuff->GetVertexSize());
	FDDDD->Get3DDevice()->SetFVF(vertBuff->GetFVF());

	//ブレンドモード設定
	FDDDD->Get3DDevice()->SetRenderState(D3DRS_BLENDOP,UINT(D3DBLENDOP_ADD));
	//通常合成
	FDDDD->Get3DDevice()->SetRenderState(D3DRS_SRCBLEND,UINT(D3DBLEND_SRCALPHA));
	FDDDD->Get3DDevice()->SetRenderState(D3DRS_DESTBLEND,UINT(D3DBLEND_INVSRCALPHA));

	//テクスチャサンプラー設定
	FDDDD->Get3DDevice()->SetSamplerState(0,D3DSAMP_ADDRESSU,UINT(D3DTADDRESS_MIRROR));
	FDDDD->Get3DDevice()->SetSamplerState(0,D3DSAMP_ADDRESSV,UINT(D3DTADDRESS_MIRROR));

	//レンダリングターゲット変更
	FDDDD->Get3DDevice()->SetRenderTarget(0, FBlurRendTarget2);
	//FDDDD->Get3DDevice()->Clear(0, NULL, D3DCLEAR_TARGET, 0x00000000, 1.0, 0);

	//テクスチャ設定
	FDDDD->Get3DDevice()->SetTexture(0, FBlurTex1->GetTexture());

	FDDDD->Get3DDevice()->DrawPrimitive(D3DPT_TRIANGLELIST,0, 2);

	for( u32 i = 0 ; i <= 5 ; i++) {
		FDDDD->Get3DDevice()->StretchRect(FBlurRendTarget2,NULL,FBlurRendTarget1,NULL,D3DTEXF_NONE);
		pV = (D3DTLVERTEX2*)vertBuff->Lock(0,0,0);
		for( u32 j = 0; j <= 5 ; j++ ) {
			D3DTLVERTEX2 SetTLV  =  TmpTLV[j];
			//(u,v)=(1,0),(0,1),(-2,0),(0,-2),(4,0),(0,4)........
			if( i % 2 == 0 )
				SetTLV.tu  =  TmpTLV[j].tu+(pow(2.f,(f32)(i / 2))*pow(-1.f,(f32)(i / 2))) / (f32)FBlurTex1->GetWidth();
			else
				SetTLV.tv  =  TmpTLV[j].tv+(pow(2.f,(f32)(i / 2))*pow(-1.f,(f32)(i / 2))) / (f32)FBlurTex1->GetHeight();
			memmove(pV, &SetTLV, vertBuff->GetVertexSize());
			pV++;
		}
		vertBuff->Unlock();
		//描画
		FDDDD->Get3DDevice()->DrawPrimitive(D3DPT_TRIANGLELIST,0, 2);

	}

	//レンダリングターゲットを元に戻す
	FDDDD->Get3DDevice()->SetRenderTarget(0, FDDDD->GetBackBuffers(0));

	//テクスチャサンプラーを元に戻す
	FDDDD->Get3DDevice()->SetSamplerState(0,D3DSAMP_ADDRESSU,UINT(D3DTADDRESS_WRAP));
	FDDDD->Get3DDevice()->SetSamplerState(0,D3DSAMP_ADDRESSV,UINT(D3DTADDRESS_WRAP));


	FBMCHFlg  =  bmNormal;
	FTexCHFlg  =  FBlurTex2;
	FChangeCHFlg = true;

	SAFE_DELETE(vertBuff);
	FDDDD->Get3DDevice()->SetStreamSource(0,FVertBuff->GetVertexBuffer(),0,FVertBuff->GetVertexSize());
	FDDDD->Get3DDevice()->SetFVF(FVertBuff->GetFVF());

}

void TSpriteEXManager::DrawSameSpr( u32 _poly, s32* _start )
{
	if( (_poly>0) ) {
		D3DTLVERTEX2 *p = FSysVertBuff;
		p += *_start;

		u32 MaxDraw = 2*(_poly);
		//if( MaxDraw>64 ) MaxDraw = 64;
		if( FCreateVB )
			FDDDD->Get3DDevice()->DrawPrimitive(D3DPT_TRIANGLELIST,*_start, 2*(_poly));
		else
			FDDDD->Get3DDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST,MaxDraw,p,sizeof(D3DTLVERTEX2));

		*_start += 6*(_poly);
		FTotalDraw++;
	}

}

u32 TSpriteEXManager::GetToneMappedCol( u32 _col, f32 _factor )
{
	//Red
	u32 red  =  ((_col & 0xff0000) >> 16);
	red  =  glm::min(255, RoundOff((f32)red*_factor));


	//Green
	u32 green  =  ((_col & 0xff00) >> 8);
	green  =  glm::min(255, RoundOff((f32)green*_factor));

	//Blue
	u32 blue  =  (_col & 0xff);
	blue  =  glm::min(255, RoundOff((f32)blue*_factor));

	return (_col & 0xff000000) | (red << 16) | (green << 8) | (blue);


}

TSpriteEXManager::TSpriteEXManager( TDGCarad& _dd, bool _bump, bool _bloom )
{

	// メンバ変数の初期化
	FDDDD 		 =  &_dd;
	FBMCHFlg 	 =  TBlendMode(-1); 		// 未定義とする
	FZX			 =  1.0f;
	FZY			 =  1.0f;
	FR			 =  PI_ZERO;
	FClearColor  =  0xFF000000;
	FVertBuff = NULL;
	FaspX = 1.0f;
	FaspY = 1.0f;
	FCreateBump = _bump;
	FCreateBloom = _bloom;
	FSaveSS = false;
	FSaveFileName = "";
	FBumpCHFlg = 255;
	FCopied = false;
	FCopied2 = false;
	FCopied3 = false;
	FBUMax = 1.0f;
	FBVMax = 1.0f;
	FCreateVB = true;  //falseは現在のところ不具合アリ（同じ描画条件のスプライトを大量描画すると落ちる。DrawPrimitives2をサポートしてない云々）


	FBGTex = NULL;
	FBGTex2 = NULL;
	FTempTex1 = NULL;
	FTempTex1 = NULL;
	FRendTarget = NULL;
	FRendTarget2 = NULL;
	FRendTarget3 = NULL;
	FAlphaEffectTex = NULL;
	FAddEffectTex = NULL;
	InitAfterReset();
	FChangeCHFlg = false;
	FTotalDraw = 0;
	FTotalSpr  = 0;
	FBloomLevel  =  0xff080808;
	FBloomBlendCnt = 2;
	FMakeBlurFlag = false;
	FMaxLuminance = 0;



}

TSpriteEXManager::~TSpriteEXManager()
{

	// リソースの解放

	FSprList.clear();

	FDDDD->Get3DDevice()->SetTexture( 0, NULL );
	FDDDD->Get3DDevice()->SetTexture( 1, NULL );
	FDDDD->Get3DDevice()->SetTexture( 2, NULL );
	//if( FBGTex!=NULL ) FBGTex.Free;//Release;

	SAFE_RELEASE(FRendTarget);
	SAFE_RELEASE(FRendTarget2);
	SAFE_RELEASE(FRendTarget3);
	SAFE_RELEASE(FTmpRendTarget1);
	SAFE_RELEASE(FTmpRendTarget2);
	SAFE_RELEASE(FBlurRendTarget1);
	SAFE_RELEASE(FBlurRendTarget2);
	SAFE_RELEASE(FAlphaRendTarget);
	SAFE_RELEASE(FAddRendTarget);
	SAFE_DELETE(FBGTex);
	SAFE_DELETE(FBGTex2);
	SAFE_DELETE(FTempTex1);
	SAFE_DELETE(FTempTex2);
	SAFE_DELETE(FBlurTex1);
	SAFE_DELETE(FBlurTex2);
	SAFE_DELETE(FAlphaEffectTex);
	SAFE_DELETE(FAddEffectTex);
	SAFE_DELETE(FVertBuff);

	if( FSysVertBuff!=NULL ) SAFE_FREE(FSysVertBuff);


}

void TSpriteEXManager::AddSprite( TSpriteBase* _spr )
{
	if( _spr->GetClassType() == SPR_CLASS_EX ) {
		TSpriteEX* spr = (TSpriteEX*)_spr;
		if( spr->FVisible ) FSprList.push_back(spr);
		if( FMaxLuminance < spr->FLuminance )
			FMaxLuminance  =  spr->FLuminance;
	}
	else if( _spr->GetClassType() == SPR_CLASS_COMBINER ) {
		for( u32 i  =  0 ; i < FSprList.size(); i++){
			if( ((TSpriteEX*)FSprList[i])->FVisible ) 
				FSprList.push_back(FSprList[i]);
		}
	}

}

void TSpriteEXManager::RemoveSprite( TSpriteBase* _spr )
{
	for( vector<TSpriteBase*>::iterator itr = FSprList.begin() ; itr != FSprList.end(); ++itr )
	{
		if( *itr == _spr )
		{
			FSprList.erase(itr);
			return;
		}
	}
}

void TSpriteEXManager::Render()
{
	//レンダリングステートの設定
	FDDDD->Get3DDevice()->SetRenderState(D3DRS_CULLMODE, UINT(D3DCULL_NONE ));     //カリングOFF
	FDDDD->Get3DDevice()->SetRenderState(D3DRS_LIGHTING, UINT(false));            //ライティングOFF
	FDDDD->Get3DDevice()->SetRenderState(D3DRS_ZENABLE, UINT(D3DZB_FALSE));       //ZバッファOFF
	FDDDD->Get3DDevice()->SetRenderState(D3DRS_ZWRITEENABLE, UINT(false));
	FDDDD->Get3DDevice()->SetRenderState(D3DRS_LOCALVIEWER, UINT(false));         //スペキュラOFF
	FDDDD->Get3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE,UINT(true));
	//
	FDDDD->Get3DDevice()->SetRenderState(D3DRS_SRCBLEND,UINT(D3DBLEND_SRCALPHA));
	FDDDD->Get3DDevice()->SetRenderState(D3DRS_DESTBLEND,UINT(D3DBLEND_INVSRCALPHA));
	FDDDD->Get3DDevice()->SetRenderState(D3DRS_BLENDOP,UINT(D3DBLENDOP_ADD));
	FDDDD->Get3DDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, UINT(false));
	//最初はアルファエフェクト用テクスチャにレンダリング

	FDDDD->Get3DDevice()->SetRenderTarget(0,FAlphaRendTarget);
	FDDDD->Get3DDevice()->Clear(0, NULL, D3DCLEAR_TARGET, 0x00000000, 1.0f, 0);

	if( (FVisible == false) || (FSprList.size() < 1) ) return; // 不可視、またはスプライト未登録

	s32 StartVertex = 0;

	FTotalDraw = 0;
	FTotalSpr  = 0;

	if( FCreateVB ) {
		FDDDD->Get3DDevice()->SetStreamSource(0,FVertBuff->GetVertexBuffer(),0,FVertBuff->GetVertexSize());
		FDDDD->Get3DDevice()->SetFVF(FVertBuff->GetFVF());
	}
	else {
		FDDDD->Get3DDevice()->SetFVF(SprFVF);
	}

	s32 nSameSpr = 0;
	FChangeCHFlg = false;
	FTexCHFlg = NULL;
	FChangeBLVFlg = 99;
	FSmoothCHFlg = false;
	bool FTargetChFlag1 = false;
	bool FTargetChFlag2 = false;
	// 全スプライト（登録済みのもの）について処理
	for( u32 i  =  0 ; i < FSprList.size(); i++) {
		TSpriteEX* spr = (TSpriteEX*)FSprList[i];
		if( ! spr->FVisible ) //スプライトの可視の確認
			continue;

		FTotalSpr++;

		// バイリニアフィルタの設定
		if( FSmoothCHFlg != spr->FSmooth )
		{
			FSmoothCHFlg  = spr->FSmooth;

			DrawSameSpr(nSameSpr,&StartVertex);
			nSameSpr = 0;
			FChangeCHFlg = true;

			if( FSmoothCHFlg && FSmooth ) {
				// フィルタを適用
				FDDDD->Get3DDevice()->SetSamplerState(0,D3DSAMP_MAGFILTER,UINT(D3DTEXF_LINEAR));
				FDDDD->Get3DDevice()->SetSamplerState(0,D3DSAMP_MINFILTER,UINT(D3DTEXF_LINEAR));
				FDDDD->Get3DDevice()->SetSamplerState(1,D3DSAMP_MAGFILTER,UINT(D3DTEXF_LINEAR));
				FDDDD->Get3DDevice()->SetSamplerState(1,D3DSAMP_MINFILTER,UINT(D3DTEXF_LINEAR));
			}
			else {      // フィルタをOFF
				FDDDD->Get3DDevice()->SetSamplerState(0,D3DSAMP_MAGFILTER,UINT(D3DTEXF_POINT));
				FDDDD->Get3DDevice()->SetSamplerState(0,D3DSAMP_MINFILTER,UINT(D3DTEXF_POINT));
				FDDDD->Get3DDevice()->SetSamplerState(1,D3DSAMP_MAGFILTER,UINT(D3DTEXF_POINT));
				FDDDD->Get3DDevice()->SetSamplerState(1,D3DSAMP_MINFILTER,UINT(D3DTEXF_POINT));
			}
		}

		//上で設定したポリゴン＆テクスチャ(=SRC)と下地の色(=DEST)のブレンド設定
		if( FBMCHFlg != spr->FBM ) {
			FBMCHFlg  =  spr->FBM;

			DrawSameSpr(nSameSpr,&StartVertex);
			nSameSpr = 0;
			FChangeCHFlg = true;
			switch( spr->FBM ){
			case bmNone:{
				//αブレンドを無効に
				//	asm finit; }
				//SetRenderState(D3DRS_ALPHABLENDENABLE,
				// 	UINT(false));
				//通常合成
				//	FDDDD->Get3DDevice()->SetRenderState(D3DRENDERSTATE_SRCBLEND,
				//	Integer(D3DBLEND_SRCALPHA));
				//FDDDD->Get3DDevice()->SetRenderState(D3DRENDERSTATE_DESTBLEND,
				//	Integer(D3DBLEND_ZERO));
						}break;
			case bmAdd:{
				//αブレンドを有効に
				//	asm finit; }
				//	SetRenderState(D3DRS_ALPHABLENDENABLE,
				//		UINT(true));
				//加算合成
				FDDDD->Get3DDevice()->SetRenderState(D3DRS_BLENDOP,UINT(D3DBLENDOP_ADD));
				FDDDD->Get3DDevice()->SetRenderState(D3DRS_DESTBLEND,
					UINT(D3DBLEND_ONE));
				FDDDD->Get3DDevice()->SetRenderState(D3DRS_SRCBLEND,
					UINT(D3DBLEND_SRCALPHA));
					   }break;
			case bmNormal:{
				//αブレンドを有効に
				//asm finit; }
				// SetRenderState(D3DRS_ALPHABLENDENABLE,
				//	  	UINT(true));
				FDDDD->Get3DDevice()->SetRenderState(D3DRS_BLENDOP,UINT(D3DBLENDOP_ADD));
				//通常合成
				FDDDD->Get3DDevice()->SetRenderState(D3DRS_SRCBLEND,
					UINT(D3DBLEND_SRCALPHA));
				FDDDD->Get3DDevice()->SetRenderState(D3DRS_DESTBLEND,
					UINT(D3DBLEND_INVSRCALPHA));

						  }break;
			case bmDec:{
				//αブレンドを有効に
				//asm finit; }
				//	SetRenderState(D3DRS_ALPHABLENDENABLE,
				//	UINT(true));
				//減算っぽい合成（ハイライト？）
				FDDDD->Get3DDevice()->SetRenderState(D3DRS_BLENDOP,UINT(D3DBLENDOP_ADD));
				FDDDD->Get3DDevice()->SetRenderState(D3DRS_SRCBLEND,
					UINT(D3DBLEND_SRCALPHA));
				FDDDD->Get3DDevice()->SetRenderState(D3DRS_DESTBLEND,
					UINT(D3DBLEND_DESTCOLOR));
					   }break;
			case bmSubAdd:{
				//αブレンドを有効に
				//	asm finit; }
				//SetRenderState(D3DRS_ALPHABLENDENABLE,
				//	 	UINT(true));
				//減算して加算
				FDDDD->Get3DDevice()->SetRenderState(D3DRS_BLENDOP,UINT(D3DBLENDOP_ADD));
				FDDDD->Get3DDevice()->SetRenderState(D3DRS_SRCBLEND,UINT(D3DBLEND_ONE));
				FDDDD->Get3DDevice()->SetRenderState(D3DRS_DESTBLEND,UINT(D3DBLEND_INVSRCALPHA));
						  }break;
			case bmMax:{
				//明るい方を描画
				FDDDD->Get3DDevice()->SetRenderState(D3DRS_BLENDOP,UINT(D3DBLENDOP_MAX));
				FDDDD->Get3DDevice()->SetRenderState(D3DRS_SRCBLEND,UINT(D3DBLEND_SRCALPHA));
				FDDDD->Get3DDevice()->SetRenderState(D3DRS_DESTBLEND,UINT(D3DBLEND_ONE));
					   }break;
				//乗算
			case bmMul:{
				FDDDD->Get3DDevice()->SetRenderState(D3DRS_BLENDOP,UINT(D3DBLENDOP_ADD));
				FDDDD->Get3DDevice()->SetRenderState(D3DRS_SRCBLEND,UINT(D3DBLEND_ZERO));
				FDDDD->Get3DDevice()->SetRenderState(D3DRS_DESTBLEND,UINT(D3DBLEND_SRCCOLOR));
					   }break;
			}
		}

		//テクスチャ指定
		if( !spr->FTexAnimAry.empty() ) {		// テクスチャが登録されていたら
			// テクスチャが以前のものと違っていたら変更する
			if( FTexCHFlg != spr->FTexAnimAry[spr->FAnimIdx].RTex ) {
				FTexCHFlg  =  spr->FTexAnimAry[spr->FAnimIdx].RTex;

				DrawSameSpr(nSameSpr,&StartVertex);
				nSameSpr = 0;
				FChangeCHFlg = true;

				// テクスチャをD3DDeviceへセット
				if( FTexCHFlg != NULL ) {
					FDDDD->Get3DDevice()->SetTexture(0, spr->FTexAnimAry[spr->FAnimIdx].RTex->GetTexture());
				}
				else {
					FDDDD->Get3DDevice()->SetTexture(0, NULL);
				}
			}


			if( (FBumpCHFlg != spr->FEffectNum)
				|| (FChangeBLVFlg != spr->FBumpLevel) ) {

					FBumpCHFlg = spr->FEffectNum;
					FChangeBLVFlg = spr->FBumpLevel;

					DrawSameSpr(nSameSpr,&StartVertex);
					nSameSpr = 0;
					FChangeCHFlg = true;

					if( (FBumpCHFlg>0) && FCreateBump ) {
						switch(FBumpCHFlg){
						case 1:{

							FDDDD->Get3DDevice()->SetTexture(1, spr->FBumpTex->GetTexture());
							//SetTexture(1, TexAnimAry[AnimIdx].RTex.Texture);
							FDDDD->Get3DDevice()->SetTexture(2, FBGTex2->GetTexture());

							FDDDD->Get3DDevice()->SetTextureStageState( 1, D3DTSS_BUMPENVMAT00,     F2DW(FChangeBLVFlg) );
							FDDDD->Get3DDevice()->SetTextureStageState( 1, D3DTSS_BUMPENVMAT11,     F2DW(FChangeBLVFlg) );
							FDDDD->Get3DDevice()->SetTextureStageState( 1, D3DTSS_COLOROP,          UINT(D3DTOP_BUMPENVMAP) );
							FDDDD->Get3DDevice()->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX,    0 );
							FDDDD->Get3DDevice()->SetTextureStageState( 1, D3DTSS_ALPHAOP,  	UINT(D3DTOP_DISABLE));
							FDDDD->Get3DDevice()->SetTextureStageState( 2, D3DTSS_COLOROP,          UINT(D3DTOP_MODULATE2X) );
							FDDDD->Get3DDevice()->SetTextureStageState( 2, D3DTSS_ALPHAOP,          UINT(D3DTOP_DISABLE) );
							   }
							   break;
						case 2:
						case 3:{
							FDDDD->Get3DDevice()->SetTexture(1, FBGTex->GetTexture());
							FDDDD->Get3DDevice()->SetTexture(2, NULL);
							FDDDD->Get3DDevice()->SetTextureStageState( 1, D3DTSS_COLOROP,   UINT(D3DTOP_MODULATE));
							FDDDD->Get3DDevice()->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, 1 );
							FDDDD->Get3DDevice()->SetTextureStageState( 1, D3DTSS_ALPHAOP,	UINT(D3DTOP_DISABLE));
							FDDDD->Get3DDevice()->SetTextureStageState( 2, D3DTSS_COLOROP,   UINT(D3DTOP_DISABLE) );
							   }
							   break;
						}
					}
					else {
						FDDDD->Get3DDevice()->SetTextureStageState( 1, D3DTSS_COLOROP,   UINT(D3DTOP_DISABLE) );
						FDDDD->Get3DDevice()->SetTextureStageState( 2, D3DTSS_COLOROP,   UINT(D3DTOP_DISABLE) );
						FDDDD->Get3DDevice()->SetTexture(1, NULL);
						FDDDD->Get3DDevice()->SetTexture(2, NULL);
					}
			}

			spr->VertexUVUpdate();   		// スプライトの頂点情報（UV座標）を更新
		}
		else {                          // テクスチャ未登録なら
			FDDDD->Get3DDevice()->SetTexture(0, NULL);
			FDDDD->Get3DDevice()->SetTexture(1, NULL);
			FDDDD->Get3DDevice()->SetTexture(2, NULL);
		}

		//描画処理
		//if( FCreateVB ) {
		//描画条件に変更がない時は、描画しない
		if( (! FChangeCHFlg)) {
			nSameSpr++;
			//ただし最後の1スプライトだったときは描画
			if( (i == FSprList.size() - 1) ) {
				DrawSameSpr(nSameSpr, &StartVertex);
			}
		}
		//最初のスプライトも描画しない
		else if( i == 0 ) {
			nSameSpr++;			FChangeCHFlg = false;
		}
		//描画条件に変更があった
		else {
			if( FCreateVB )
				FDDDD->Get3DDevice()->DrawPrimitive(D3DPT_TRIANGLELIST,StartVertex, 2);
			else {
				D3DTLVERTEX2* p = FSysVertBuff;
				p += StartVertex;
				FDDDD->Get3DDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST,2,p,sizeof(D3DTLVERTEX2));
			}

			StartVertex += 6;
			FTotalDraw++;
			FChangeCHFlg = false;
		}

		if( FCreateBump ) {

			//背景をテクスチャにコピー
			if( (! FCopied) && (spr->FZ <= 9100) ) {
				if( FCreateBump ) {
					FDDDD->Get3DDevice()->StretchRect(FDDDD->GetBackBuffers(0),NULL,FRendTarget,NULL,D3DTEXF_NONE);
				}
				FCopied = true;
			}

			if( (! FCopied2) && (spr->FZ <= 17100) ) {
				if( FCreateBump ) {
					FDDDD->Get3DDevice()->StretchRect(FDDDD->GetBackBuffers(0),NULL,FRendTarget2,NULL,D3DTEXF_NONE);

				}
				FCopied2 = true;
			}
		}

		if( FCreateBloom ) {
			if( (! FCopied3) && (spr->FZ<=2100) ) {
				MakeBlooming();
				FCopied3 = true;
			}
		}

		if( (FMakeBlurFlag) && (spr->FZ<=300) ) {
			MakeBlurInternal();
			FMakeBlurFlag = false;
		}

		if( (! FTargetChFlag1) && (spr->FZ <= 51200) ) {
			//加算エフェクト用テクスチャにレンダリング
			FDDDD->Get3DDevice()->SetRenderTarget(0,FAddRendTarget);
			FDDDD->Get3DDevice()->Clear(0, NULL, D3DCLEAR_TARGET, 0x00000000, 1.0, 0);
			FTargetChFlag1 = true;
		}

		if( (! FTargetChFlag2) && (spr->FZ <= 25601) ) {
			//バックバッファはクリアー不要（どうせ背景で上書きされる）
			FDDDD->Get3DDevice()->SetRenderTarget(0,FDDDD->GetBackBuffers(0));
			FTargetChFlag2 = true;
		}

	}


	FCopied = false;
	FCopied2 = false;
	FCopied3 = false;

	//フラグが立ってればスクリーンショットの保存
	if( FSaveSS ) {
		FDDDD->Get3DDevice()->CreateOffscreenPlainSurface(FDDDD->GetBackBufferWidth(), FDDDD->GetBackBufferHeight(), FDDDD->GetBackBufferFormat(), D3DPOOL_SYSTEMMEM, &FSSSurface, NULL);
		FDDDD->Get3DDevice()->GetRenderTargetData(FDDDD->GetBackBuffers(0), FSSSurface);
		SavingProcedure();
		FSaveSS = false;
		FSSSurface->Release();
	}


}

void TSpriteEXManager::Sort()
{
	SortOnBrendMode();
	SortOnTexture();
	SortOnZ();
}

void TSpriteEXManager::SortOnBrendMode()
{
	class BMSortCompare{
	public:
		bool operator()(const void* _item1, const void* _item2 ) const {
			return ( ((TSpriteEX*)_item1)->FBM < ((TSpriteEX*)_item2)->FBM );
		}
	};

	sort(FSprList.begin(), FSprList.end(), BMSortCompare());

}

void TSpriteEXManager::SortOnTexture()
{
	class TexSortCompare{
	public:
		bool operator()(const void* _item1, const void* _item2 ) const {
			return DWORD(((TSpriteEX*)_item1)->FTexAnimAry[0].RTex) < DWORD(((TSpriteEX*)_item2)->FTexAnimAry[0].RTex);
		}
	};

	sort(FSprList.begin(), FSprList.end(), TexSortCompare());
}

void TSpriteEXManager::SortOnZ()
{
	class ZSortCompare{
	public:
		bool operator()(const void* _item1, const void* _item2 ) const {
			return ((TSpriteEX*)_item1)->FZ < ((TSpriteEX*)_item2)->FZ;
		}
	};

	sort(FSprList.begin(), FSprList.end(), ZSortCompare());
}

void TSpriteEXManager::ClearSprite()
{
	FSprList.clear();
	FMaxLuminance = 0;

}

void TSpriteEXManager::ClearScreen()
{
	D3DRECT rect;
	rect.x1 = FClearRect.Left;
	rect.x2 = FClearRect.Right;
	rect.y1 = FClearRect.Top;
	rect.y2 = FClearRect.Bottom;
	FDDDD->Get3DDevice()->Clear(1, &rect, D3DCLEAR_TARGET, FClearColor, 1.0f, 0);

}

void TSpriteEXManager::MoveR( s32 _x, s32 _y )
{
	MoveR((f32)_x + 0.0f, (f32)_y + 0.0f);

}

void TSpriteEXManager::MoveR( f32 _x, f32 _y )
{
	FX  =  FX + _x;
	FY  =  FY + _y;

}

void TSpriteEXManager::Move( s32 _x, s32 _y )
{
	Move((f32)_x + 0.0f, (f32)_y + 0.0f);

}

void TSpriteEXManager::Move( f32 _x, f32 _y )
{
	MoveR(_x - FX, _y - FY);

}

void TSpriteEXManager::SlideMoveR( s32 _x, s32 _y )
{
	SlideMoveR((f32)_x + 0.0f, (f32)_y + 0.0f);

}

void TSpriteEXManager::SlideMoveR( f32 _x, f32 _y )
{
	exRotate(&_x, &_y, 0, 0, _x, _y, FR);
	FX  =  FX + _x;
	FY  =  FY + _y;

}

void TSpriteEXManager::RotateR( s16 _r )
{
	_r  =  _r & 0x0FFF;
	FR  =  (FR + _r) & 0x0FFF;

}

void TSpriteEXManager::Rotate( s16 _r )
{
	FR  =  _r & 0x0FFF;

}

void TSpriteEXManager::ZoomR( f32 _zx, f32 _zy )
{
	FZX  =  FZX * _zx;
	FZY  =  FZY * _zy;

}

void TSpriteEXManager::Zoom( f32 _zx, f32 _zy )
{
	FZX  =  _zx;
	FZY  =  _zy;

}

void TSpriteEXManager::GeometryProcess()
{
	if( FSprList.empty() ) return;                    // スプライト未登録なら抜ける
	//FSprList.Pack;                                      // リストを整理して
	//	FSprList.Capacity  =  FSprList.Count;                // 無駄なNULLポインタを削除

	if( FCreateVB ) {
		//既存の頂点バッファを開放
		SAFE_DELETE(FVertBuff);

		//頂点バッファ作成
		FVertBuff = new TDGVertexBuffer(FDDDD, 6*FSprList.size(), sizeof(D3DTLVERTEX2), D3DUSAGE_DYNAMIC, SprFVF, D3DPOOL_DEFAULT, "");

		u8* pV = (u8*)FVertBuff->Lock(0,0,D3DLOCK_NOOVERWRITE);

		// アルファエフェクトテクスチャへ描画する為のアスペクト比セット
		SetAspect((f32)FAlphaEffectTex->GetWidth() / (f32)640, (f32)FAlphaEffectTex->GetHeight() / (f32)480);

		// ワールド座標→スクリーン座標へのトランスフォーム
		// 全ての登録済スプライトについて処理
		for( u32 i  =  0 ; i < FSprList.size(); i++) {
			TSpriteEX* spr = ((TSpriteEX*)FSprList[i]);
			if( ! spr->FVisible ) continue;	   		// 見えないものはトランスフォーム不要

			//アスペクト比を戻す
			if( spr->FZ = 25601 ) SetAspect((f32)FDDDD->GetBackBufferWidth()/(f32)640, (f32)FDDDD->GetBackBufferHeight()/(f32)480);


			if( spr->FTransform ) {
				// トランスフォームする場合

				// スプライトの全頂点を処理
				for( u32 j  =  0 ; j <= 5; j++ ) {
					D3DTLVERTEX2& TmpTLV  =  spr->FTLVs[j];
					// 平行移動
					TmpTLV.sx  =  spr->FTLVs[j].sx - FX * FaspX;
					TmpTLV.sy  =  spr->FTLVs[j].sy - FY * FaspY;
					//特殊エフェクト用UV座標ずらし
					if( ((spr->FEffectNum = 1) || (spr->FEffectNum = 2)) && FCreateBump ) {
						TmpTLV.bu  =  (spr->FCX-(spr->FCX-TmpTLV.sx)*spr->FBumpSize) / (f32)640;
						TmpTLV.bv  =  (spr->FCY-(spr->FCY-TmpTLV.sy)*spr->FBumpSize) / (f32)480;
						if( TmpTLV.bu<0 )  TmpTLV.bu = 0;
						if( TmpTLV.bv<0 )  TmpTLV.bv = 0;
						if( TmpTLV.bu>1.0f )   TmpTLV.bu = 1.0f;
						if( TmpTLV.bv>1.0f )   TmpTLV.bv = 1.0f;
					}
					//擬似遠近法を適用する場合
					if( spr->FDistorte ) {
						if( (j<=1) || (j==3) )
							TmpTLV.sx  =  TmpTLV.sx + ((f32)FDDDD->GetBackBufferWidth() / FaspX * 0.5f - TmpTLV.sx) / spr->FDistortRate;
					}

					//トーンマッピングを適用
					if( spr->FToneMapping ) TmpTLV.color = GetToneMappedCol(spr->FTLVs[j].color, (f32)spr->FLuminance / FMaxLuminance);


					// 回転（カメラ向きと逆方向に回転させる）＆スケーリング
					u16 r  =  (PI_ZERO - FR) & 0x0FFF;
					exRotate(&TmpTLV.sx, &TmpTLV.sy, FCX, FCY, TmpTLV.sx, TmpTLV.sy, r);
					exZoom(&TmpTLV.sx, &TmpTLV.sy, FCX, FCY, TmpTLV.sx, TmpTLV.sy, FZX * FaspX, FZY * FaspY);

					memmove(pV, &TmpTLV, FVertBuff->GetVertexSize());
					pV += FVertBuff->GetVertexSize();
				}
			}
			else {
				// トランスフォームしない場合
				for( u32 j  =  0 ; j <= 5; j++ ) {
					memmove(pV, &spr->FTLVs[j], FVertBuff->GetVertexSize());
					pV += FVertBuff->GetVertexSize();
				}
			}
		}
		FVertBuff->Unlock();
	}
	else {
		FSysVertBuff = (D3DTLVERTEX2*)malloc(sizeof(D3DTLVERTEX2) * 6 * FSprList.size());
		D3DTLVERTEX2* p  =  FSysVertBuff;

		// ワールド座標→スクリーン座標へのトランスフォーム
		// 全ての登録済スプライトについて処理
		for( u32 i  =  0 ; i < FSprList.size(); i++) {
			TSpriteEX* spr  = ((TSpriteEX*)FSprList[i]);
			if( ! spr->FVisible ) continue;	   		// 見えないものはトランスフォーム不要

			if( spr->FTransform ) {
				// トランスフォームする場合

				// スプライトの全頂点を処理
				for( u32 j  =  0 ; j <= 5 ; j++) {
					*p  =  spr->FTLVs[j];
					// 平行移動
					p->sx  =  spr->FTLVs[j].sx - FX * FaspX;
					p->sy  =  spr->FTLVs[j].sy - FY * FaspY;
					if( spr->FDistorte ) {
						if( (j<=1) || (j==3) )
							p->sx  =  p->sx + ((f32)FDDDD->GetBackBufferWidth() / FaspX * 0.5f - p->sx) / spr->FDistortRate;
					}

					// 回転（カメラ向きと逆方向に回転させる）＆スケーリング
					u16 r  =  (PI_ZERO - FR) & 0x0FFF;
					exRotate(&p->sx, &p->sy, FCX, FCY, p->sx, p->sy, r);
					exZoom(&p->sx, &p->sy, FCX, FCY, p->sx, p->sy, FZX * FaspX, FZY * FaspY);

					p++;
				}
			}
			else {
				// トランスフォームしない場合
				for( u32 j  =  0 ; j <= 5; j++ ) {
					*p  =  spr->FTLVs[j];
					p++;
				}
			}
		}
	}

}

void TSpriteEXManager::RenderHitArea()
{
	//TmpTLV          : array [0..4] of D3DTLVERTEX2;
	//pV      :pBYTE;
	//vertBuff:TDGVertexBuffer;
	//numHitArea:Integer;
	//当たり判定表示用にレンダリングステートをワイヤフレームに＆テクスチャをOFF
	FDDDD->Get3DDevice()->SetRenderState(D3DRS_FILLMODE, UINT(D3DFILL_WIREFRAME));
	FDDDD->Get3DDevice()->SetTexture(0, NULL);


	//頂点バッファ作成
	TDGVertexBuffer* vertBuff = new TDGVertexBuffer(FDDDD, 5*FSprList.size(), sizeof(D3DTLVERTEX2), D3DUSAGE_DYNAMIC, SprFVF, D3DPOOL_DEFAULT,"");
	FDDDD->Get3DDevice()->SetStreamSource(0, vertBuff->GetVertexBuffer(), 0, vertBuff->GetVertexSize());
	FDDDD->Get3DDevice()->SetFVF(vertBuff->GetFVF());
	u8* pV = (u8*)vertBuff->Lock(0,0,D3DLOCK_NOOVERWRITE);
	s32 numHitArea = 0;

	//あたり判定座標をスクリーン座標に変換
	for( u32 k  =  0 ; k < FSprList.size() ; k++ ){
		TSpriteEX* spr = ((TSpriteEX*)FSprList[k]);
		if( ! spr->FHit ) continue;
		vector<THitArea> haa;
		spr->HitAreaTransform(haa);

		for( u32 i  =  0 ; i < haa.size(); i++ ) {
			D3DTLVERTEX2 TmpTLV[5];
			for( u32 j  =  0 ; j <= 3; j++ ) {
				TmpTLV[j]  =  MakeTLV( 0, 0, 1.f, 1.f, 0xffff0000, 0,   0,	0, 0, 0);
				TmpTLV[j].sx  =  haa[i].RPointAry[j].x * FaspX;
				TmpTLV[j].sy  =  haa[i].RPointAry[j].y * FaspY;
				PointTransformToSCR(&TmpTLV[j].sx, &TmpTLV[j].sy);
			}
			//終点と始点を結ぶ線を描画するため、一本よけいに頂点を作成
			TmpTLV[4]  =  MakeTLV( 0, 0, 1, 1, 0xffff0000, 0,   0,	0, 0, 0);
			TmpTLV[4].sx  =  TmpTLV[0].sx;
			TmpTLV[4].sy  =  TmpTLV[0].sy;

			//頂点バッファにコピー
			for( u32 j  =  0 ; j <= 4 ; j++) {
				memmove(pV, &TmpTLV[j], vertBuff->GetVertexSize());
				pV += vertBuff->GetVertexSize();
			}
			numHitArea++;
		}
	}
	vertBuff->Unlock();
	//線分を描画
	for( s32 i  =  0 ; i < numHitArea; i++ )
		FDDDD->Get3DDevice()->DrawPrimitive(D3DPT_LINESTRIP,i*5, 4);

	SAFE_DELETE(vertBuff);

}

void TSpriteEXManager::PointTransformToSCR( f32* _x, f32* _y )
{
	// スプライト頂点をスクリーン座標系に変換
	*_x  -= FX;
	*_y  -= FY;
	// 回転（カメラ向きと逆方向に回転させる）＆スケーリング
	u16 r  =  (PI_ZERO - FR) & 0x0FFF;
	exRotate(_x, _y, FCX, FCY, *_x, *_y, r);
	exZoom(_x, _y, FCX, FCY, *_x, *_y, FZX, FZY);

}

void TSpriteEXManager::CalcScreenBB( Rect* _rt )
{
	_rt->Left  =  0x7FFFFFFF;
	_rt->Right  =  0;
	_rt->Top  =  0x7FFFFFFF;
	_rt->Bottom  =  0;

	// トランスフォーム前のスクリーン頂点を定義
	TSPoint pt[4];
	pt[0]  =  SPoint(0, 0);
	pt[1]  =  SPoint((f32)FDDDD->GetBackBufferWidth(), 0);
	pt[2]  =  SPoint(0, (f32)FDDDD->GetBackBufferHeight());
	pt[3]  =  SPoint((f32)FDDDD->GetBackBufferWidth(), (f32)FDDDD->GetBackBufferHeight());

	// トランスフォーム後のスクリーン頂点を求める
	for( u32 i  =  0 ; i <= 3 ; i++) {
		exZoom(&pt[i].x, &pt[i].y, FCX, FCY, pt[i].x, pt[i].y, 1.f / FZX, 1.f / FZY);
		exRotate(&pt[i].x, &pt[i].y, FCX, FCY, pt[i].x, pt[i].y, FR);
		pt[i].x  =  pt[i].x + FX;
		pt[i].y  =  pt[i].y + FY;
	}

	// スクリーン矩形に対するバウンディングボックスを求める
	for( u32 i  =  0 ; i <= 3; i++ ) {
		if( (_rt->Left > pt[i].x)  	) _rt->Left 	 =  (s32)(pt[i].x);
		if( (_rt->Right < pt[i].x)	) _rt->Right 	 =  (s32)(pt[i].x);
		if( (_rt->Top > pt[i].y) 		) _rt->Top	 =  (s32)(pt[i].y);
		if( (_rt->Bottom < pt[i].y)	) _rt->Bottom  =  (s32)(pt[i].y);
	}

}

void TSpriteEXManager::SetAspect( f32 _ax, f32 _ay )
{
	FaspX = _ax;
	FaspY = _ay;

}

void TSpriteEXManager::SaveScreenShot( const string& _filename )
{
	FSaveSS = true;
	FSaveFileName = _filename;

}

void TSpriteEXManager::SavingProcedure()
{
	///////////////////////////
	BITMAPFILEHEADER bf;
	BITMAPINFOHEADER bi;
	vector<u32> lineBuf;
	u32 width = FDDDD->GetBackBufferWidth();
	u32 height = FDDDD->GetBackBufferHeight();

	//ヘッダ作成
	bf.bfType = 0x4d42;
	bf.bfSize = sizeof(bf) + sizeof(bi) + (width * height) * 4;
	bf.bfReserved1 = 0;
	bf.bfReserved2 = 0;
	bf.bfOffBits = sizeof(bf) + sizeof(bi);

	bi.biSize = sizeof(bi);
	bi.biWidth = width;
	bi.biHeight = height;
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = width * height * 4;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	//ヘッダ書き込み
	ofstream dest(FSaveFileName);
	dest.write(reinterpret_cast<char*>(&bf), sizeof(bf));
	dest.write(reinterpret_cast<char*>(&bi), sizeof(bi));

	lineBuf.resize(width);

	//変換。上下逆に書き込み

	D3DLOCKED_RECT SSLockRect;
	//変換して保存
	if( (FSSSurface->LockRect(&SSLockRect,NULL,0)==D3D_OK) ) {
		u32 pixsize = bi.biBitCount / 8;
		for( s32  y = height-1; y >= 0; y-- ) {
			u8* pLine = (u8*)SSLockRect.pBits;
			pLine += y*SSLockRect.Pitch;
			u8* pPix = pLine;

			//一ライン変換して
			for( u32  x = 0; x < width; x++ ) {
				u32 col = 0;
				memmove(&col, pPix, pixsize);
				lineBuf[x] = PhysicalToARGB(col, D3DFMT_R8G8B8);
				pPix += pixsize;
			}
			//書き込み
			dest.write(reinterpret_cast<char*>(&lineBuf[0]), width*4);
		}
		FSSSurface->UnlockRect();
	}

	lineBuf.clear();

	dest.close();
}

void TSpriteEXManager::ReleaseBeforeReset()
{
	SAFE_RELEASE(FRendTarget);
	SAFE_RELEASE(FRendTarget2);
	SAFE_RELEASE(FRendTarget3);
	SAFE_RELEASE(FTmpRendTarget1);
	SAFE_RELEASE(FTmpRendTarget2);
	SAFE_RELEASE(FBlurRendTarget1);
	SAFE_RELEASE(FBlurRendTarget2);
	SAFE_RELEASE(FAlphaRendTarget);
	SAFE_RELEASE(FAddRendTarget);
	SAFE_DELETE(FBGTex);
	SAFE_DELETE(FBGTex2);
	SAFE_DELETE(FTempTex1);
	SAFE_DELETE(FTempTex2);
	SAFE_DELETE(FBlurTex1);
	SAFE_DELETE(FBlurTex2);
	SAFE_DELETE(FAlphaEffectTex);
	SAFE_DELETE(FAddEffectTex);
}

void TSpriteEXManager::InitAfterReset()
{
	if( glm::max(FDDDD->GetBackBufferWidth(), FDDDD->GetBackBufferHeight() )>=1024 )
		FBGTexSize = 512;
	else if( glm::max(FDDDD->GetBackBufferWidth(), FDDDD->GetBackBufferHeight())<=512 )
		FBGTexSize = 128;
	else
		FBGTexSize = 256;

	D3DFORMAT fmt = FDDDD->GetBackBufferFormat();
	if( FCreateBump ) {
		FBUMax =  (f32)FBGTexSize / (f32)FDDDD->GetBackBufferWidth() ;
		FBVMax =  (f32)FBGTexSize / (f32)FDDDD->GetBackBufferHeight();
		if( FBGTex == NULL ) {
			FBGTex = new TDGTexture(FDDDD, &fmt,D3DUSAGE_RENDERTARGET, D3DPOOL_DEFAULT);
			FBGTex->SetSize(FBGTexSize,FBGTexSize);
			FBGTex->GetTexture()->GetSurfaceLevel(0, &FRendTarget);
		}
		if( FBGTex2==NULL ) {
			FBGTex2 = new TDGTexture(FDDDD, &fmt,D3DUSAGE_RENDERTARGET,D3DPOOL_DEFAULT);
			FBGTex2->SetSize(FBGTexSize,FBGTexSize);
			FBGTex2->GetTexture()->GetSurfaceLevel(0, &FRendTarget2);
		}
	}

	if( FCreateBloom ) {
		if( FTempTex1=NULL ) {
			FTempTex1 = new TDGTexture(FDDDD, &fmt,D3DUSAGE_RENDERTARGET,D3DPOOL_DEFAULT);
			FTempTex1->SetSize(FBGTexSize,FBGTexSize);
			FTempTex1->GetTexture()->GetSurfaceLevel(0, &FTmpRendTarget1);
		}
		if( FTempTex2=NULL ) {
			FTempTex2 = new TDGTexture(FDDDD, &fmt,D3DUSAGE_RENDERTARGET,D3DPOOL_DEFAULT);
			FTempTex2->SetSize(FBGTexSize,FBGTexSize);
			FTempTex2->GetTexture()->GetSurfaceLevel(0, &FTmpRendTarget2);
		}

	}

	if( FBlurTex1=NULL ) {
		FBlurTex1 = new TDGTexture(FDDDD, &fmt,D3DUSAGE_RENDERTARGET,D3DPOOL_DEFAULT);
		FBlurTex1->SetSize(FBGTexSize,FBGTexSize);
		FBlurTex1->GetTexture()->GetSurfaceLevel(0, &FBlurRendTarget1);
	}
	if( FBlurTex2=NULL ) {
		FBlurTex2 = new TDGTexture(FDDDD, &fmt,D3DUSAGE_RENDERTARGET,D3DPOOL_DEFAULT);
		FBlurTex2->SetSize(FBGTexSize,FBGTexSize);
		FBlurTex2->GetTexture()->GetSurfaceLevel(0, &FBlurRendTarget2);
	}

	if( FAlphaEffectTex=NULL ) {
		FAlphaEffectTex = new TDGTexture(FDDDD, DGFMT_ARGB,D3DUSAGE_RENDERTARGET,D3DPOOL_DEFAULT);
		FAlphaEffectTex->SetSize(FBGTexSize,FBGTexSize / 2);
		FAlphaEffectTex->GetTexture()->GetSurfaceLevel(0, &FAlphaRendTarget);
	}

	if( FAddEffectTex=NULL ) {
		FAddEffectTex = new TDGTexture(FDDDD, &fmt,D3DUSAGE_RENDERTARGET,D3DPOOL_DEFAULT);
		FAddEffectTex->SetSize(FBGTexSize,FBGTexSize / 2);
		FAddEffectTex->GetTexture()->GetSurfaceLevel(0, &FAddRendTarget);
	}

	//レンダリングステートの設定

		FDDDD->Get3DDevice()->SetTexture(0, NULL);

		FDDDD->Get3DDevice()->SetRenderState(D3DRS_CULLMODE, UINT(D3DCULL_NONE ));     //カリングOFF
		FDDDD->Get3DDevice()->SetRenderState(D3DRS_LIGHTING, UINT(false));            //ライティングOFF
		FDDDD->Get3DDevice()->SetRenderState(D3DRS_ZENABLE, UINT(D3DZB_FALSE));       //ZバッファOFF
		FDDDD->Get3DDevice()->SetRenderState(D3DRS_ZWRITEENABLE, UINT(false));
		FDDDD->Get3DDevice()->SetRenderState(D3DRS_LOCALVIEWER, UINT(false));         //スペキュラOFF
		FDDDD->Get3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE,UINT(true));


	//ポリゴンの色とテクセルのブレンド設定

		FDDDD->Get3DDevice()->SetTextureStageState(0, D3DTSS_COLORARG1,	UINT(D3DTA_TEXTURE));
		FDDDD->Get3DDevice()->SetTextureStageState(0, D3DTSS_COLORARG2,	UINT(D3DTA_DIFFUSE));
		FDDDD->Get3DDevice()->SetTextureStageState(0, D3DTSS_COLOROP,		UINT(D3DTOP_MODULATE));

		FDDDD->Get3DDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1,	UINT(D3DTA_TEXTURE));
		FDDDD->Get3DDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG2,	UINT(D3DTA_DIFFUSE));
		FDDDD->Get3DDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP,		UINT(D3DTOP_MODULATE));

		FDDDD->Get3DDevice()->SetSamplerState(0,D3DSAMP_ADDRESSU,UINT(D3DTADDRESS_WRAP));
		FDDDD->Get3DDevice()->SetSamplerState(0,D3DSAMP_ADDRESSV,UINT(D3DTADDRESS_WRAP));
		if( FCreateBump ) {
			FDDDD->Get3DDevice()->SetTextureStageState( 1, D3DTSS_COLOROP,   UINT(D3DTOP_BUMPENVMAP) );
			FDDDD->Get3DDevice()->SetTextureStageState( 1, D3DTSS_COLORARG1, UINT(D3DTA_CURRENT) );
			FDDDD->Get3DDevice()->SetTextureStageState( 1, D3DTSS_COLORARG2, UINT(D3DTA_TEXTURE) );

			FDDDD->Get3DDevice()->SetTextureStageState( 1, D3DTSS_BUMPENVMAT00,   F2DW(-0.6f) );
			FDDDD->Get3DDevice()->SetTextureStageState( 1, D3DTSS_BUMPENVMAT01,   F2DW(0.0f) );
			FDDDD->Get3DDevice()->SetTextureStageState( 1, D3DTSS_BUMPENVMAT10,   F2DW(0.0f) );
			FDDDD->Get3DDevice()->SetTextureStageState( 1, D3DTSS_BUMPENVMAT11,   F2DW(-0.6f) );

			FDDDD->Get3DDevice()->SetTextureStageState( 1, D3DTSS_ALPHAARG1,	UINT(D3DTA_TEXTURE));
			FDDDD->Get3DDevice()->SetTextureStageState( 1, D3DTSS_ALPHAARG2,	UINT(D3DTA_DIFFUSE));
			FDDDD->Get3DDevice()->SetTextureStageState( 1, D3DTSS_ALPHAOP,	UINT(D3DTOP_MODULATE));
			FDDDD->Get3DDevice()->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, 0 );

			FDDDD->Get3DDevice()->SetTextureStageState( 2, D3DTSS_COLOROP,   UINT(D3DTOP_ADDSMOOTH) );
			FDDDD->Get3DDevice()->SetTextureStageState( 2, D3DTSS_COLORARG1, UINT(D3DTA_TEXTURE) );
			FDDDD->Get3DDevice()->SetTextureStageState( 2, D3DTSS_COLORARG2, UINT(D3DTA_CURRENT));
			FDDDD->Get3DDevice()->SetTextureStageState( 2, D3DTSS_TEXCOORDINDEX, 1 );
			FDDDD->Get3DDevice()->SetTextureStageState( 2, D3DTSS_ALPHAOP,	UINT(D3DTOP_DISABLE));

			FDDDD->Get3DDevice()->SetSamplerState(2,D3DSAMP_MAGFILTER,UINT(D3DTEXF_LINEAR));
			FDDDD->Get3DDevice()->SetSamplerState(2,D3DSAMP_MINFILTER,UINT(D3DTEXF_LINEAR));

			FDDDD->Get3DDevice()->SetSamplerState(1,D3DSAMP_ADDRESSU,UINT(D3DTADDRESS_MIRROR));
			FDDDD->Get3DDevice()->SetSamplerState(1,D3DSAMP_ADDRESSV,UINT(D3DTADDRESS_MIRROR));

			FDDDD->Get3DDevice()->SetSamplerState(2,D3DSAMP_ADDRESSU,UINT(D3DTADDRESS_MIRROR));
			FDDDD->Get3DDevice()->SetSamplerState(2,D3DSAMP_ADDRESSV,UINT(D3DTADDRESS_MIRROR));
		}
		else {
			FDDDD->Get3DDevice()->SetTextureStageState( 1, D3DTSS_COLOROP,   UINT(D3DTOP_MODULATE) );
			FDDDD->Get3DDevice()->SetTextureStageState( 1, D3DTSS_COLORARG1, UINT(D3DTA_CURRENT) );
			FDDDD->Get3DDevice()->SetTextureStageState( 1, D3DTSS_COLORARG2, UINT(D3DTA_TEXTURE) );
			FDDDD->Get3DDevice()->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, 1 );
			FDDDD->Get3DDevice()->SetTextureStageState( 1, D3DTSS_ALPHAOP,	UINT(D3DTOP_DISABLE));
			FDDDD->Get3DDevice()->SetTextureStageState(1, D3DTSS_ALPHAARG1,	UINT(D3DTA_TEXTURE));
			FDDDD->Get3DDevice()->SetTextureStageState(1, D3DTSS_ALPHAARG2,	UINT(D3DTA_DIFFUSE));
		}


}

void TSpriteEXManager::MakeBlooming()
{
//TmpTLV          : array [0..5] of D3DTLVERTEX2;
//SetTLV          : D3DTLVERTEX2;
//pV              :pBYTE;
//vertBuff        :TDGVertexBuffer;
//	i,j               :Integer;
//r		: WORD;
//UVFactor        : Single;
	//  BltRect         : TRect;
	if( ! FCreateBloom ) return;

	f32 UVFactor  =  1.0f;
	//BltRect  =  Rect(0, 0, 256, 127);
	//作業用スプライト頂点作成
	D3DTLVERTEX2 TmpTLV[6];
	TmpTLV[0] = MakeTLV(          0, 		   0,   1.f, 1.f, 0xFFFFFFFF, 0,   0,	0, 0, 0);
	TmpTLV[1] = MakeTLV((f32)FTempTex1->GetWidth(), 	           0,  	1.f, 1.f, 0xFFFFFFFF, 0,   UVFactor,   0, 0, 0);
	TmpTLV[2] = MakeTLV(          0, 	(f32)FTempTex1->GetHeight(), 	1.f, 1.f, 0xFFFFFFFF, 0,   0,   UVFactor, 0, 0);
	TmpTLV[3] = MakeTLV((f32)FTempTex1->GetWidth(),	           0, 	1.f, 1.f, 0xFFFFFFFF, 0,   UVFactor,   0, 0, 0);
	TmpTLV[4] = MakeTLV(          0,	        (f32)FTempTex1->GetHeight(), 	1.f, 1.f, 0xFFFFFFFF, 0,   0,   UVFactor, 0, 0);
	TmpTLV[5] = MakeTLV((f32)FTempTex1->GetWidth(),	(f32)FTempTex1->GetHeight(), 	1.f, 1.f, 0xFFFFFFFF, 0,   UVFactor,   UVFactor, 0, 0);

	//バックバッファの内容をコピー
	FDDDD->Get3DDevice()->StretchRect(FDDDD->GetBackBuffers(0), NULL,FTmpRendTarget1,NULL,D3DTEXF_NONE);
	//FDDDD->Get3DDevice()->StretchRect(FTmpRendTarget1,NULL,FTmpRendTarget2,NULL,D3DTEXF_NONE);

	//頂点バッファ作成
	TDGVertexBuffer* vertBuff  = new TDGVertexBuffer(FDDDD,6,sizeof(D3DTLVERTEX2),D3DUSAGE_DYNAMIC,SprFVF,D3DPOOL_DEFAULT,"");

	SetAspect((f32)FTempTex1->GetWidth() / 640.f, (f32)FTempTex1->GetHeight() / 480.f);

	u8* pV = (u8*)vertBuff->Lock(0,0,D3DLOCK_DISCARD);

	// 頂点のトランスフォーム
	for( u32 i  =  0 ; i <= 5; i++ ) {
		// 平行移動
		D3DTLVERTEX2& SetTLV  =  TmpTLV[i];
		SetTLV.sx  =  TmpTLV[i].sx / FaspX;// + FX * FaspX;
		SetTLV.sy  =  TmpTLV[i].sy / FaspY;// + FY * FaspY;

		// 回転（カメラ向きと逆方向に回転させる）＆スケーリング
		u16 r  =  (PI_ZERO - FR) & 0x0FFF;
		exRotate(&SetTLV.sx, &SetTLV.sy, FCX, FCY, SetTLV.sx, SetTLV.sy, r);
		exZoom(&SetTLV.sx, &SetTLV.sy, FCX, FCY, SetTLV.sx, SetTLV.sy, FZX * FaspX, FZY * FaspY);

		memmove(pV, &SetTLV, vertBuff->GetVertexSize());
		pV += vertBuff->GetVertexSize();
	}

	vertBuff->Unlock();
	SetAspect( (f32)FDDDD->GetBackBufferWidth() / 640.f, (f32)FDDDD->GetBackBufferHeight() / 480.f);

	FDDDD->Get3DDevice()->SetStreamSource(0,vertBuff->GetVertexBuffer(),0,vertBuff->GetVertexSize());
	FDDDD->Get3DDevice()->SetFVF(vertBuff->GetFVF());

	//ブレンドモード設定
	FDDDD->Get3DDevice()->SetRenderState(D3DRS_BLENDOP,UINT(D3DBLENDOP_SUBTRACT));
	//減算合成
	FDDDD->Get3DDevice()->SetRenderState(D3DRS_SRCBLEND,UINT(D3DBLEND_ONE));
	FDDDD->Get3DDevice()->SetRenderState(D3DRS_DESTBLEND,UINT(D3DBLEND_ONE));

	//テクスチャサンプラー設定
	FDDDD->Get3DDevice()->SetSamplerState(0,D3DSAMP_ADDRESSU,UINT(D3DTADDRESS_MIRROR));
	FDDDD->Get3DDevice()->SetSamplerState(0,D3DSAMP_ADDRESSV,UINT(D3DTADDRESS_MIRROR));

	//レンダリングターゲット変更
	FDDDD->Get3DDevice()->SetRenderTarget(0, FTmpRendTarget2);
	FDDDD->Get3DDevice()->Clear(0, NULL, D3DCLEAR_TARGET, FBloomLevel, 1.0, 0);

	//テクスチャ設定
	FDDDD->Get3DDevice()->SetTexture(0, FTempTex1->GetTexture());

	//描画
	FDDDD->Get3DDevice()->DrawPrimitive(D3DPT_TRIANGLELIST,0, 2);
	//もう片方のテクスチャに内容をコピー
	//FDDDD->Get3DDevice()->StretchRect(FTmpRendTarget2,NULL,FTmpRendTarget1,NULL,D3DTEXF_NONE);

	//ブレンドモード設定
	FDDDD->Get3DDevice()->SetRenderState(D3DRS_BLENDOP,UINT(D3DBLENDOP_ADD));
	//                SetRenderState(D3DRS_BLENDOP,UINT(D3DBLENDOP_MAX));
	//                加算合成
	FDDDD->Get3DDevice()->SetRenderState(D3DRS_SRCBLEND,UINT(D3DBLEND_SRCALPHA));
	FDDDD->Get3DDevice()->SetRenderState(D3DRS_DESTBLEND,UINT(D3DBLEND_ONE));

	//                SetRenderState(D3DRS_SRCBLEND,UINT(D3DBLEND_SRCALPHA));
	//                SetRenderState(D3DRS_DESTBLEND,UINT(D3DBLEND_INVSRCALPHA));

	//                for( u32 j = 0 to 5 ) {
	//                  TmpTLV[j].color = 0xffffffff;
	//                }
	//描画位置をずらしながらお互いのテクスチャを繰り返し重ねていく
	for( s32 i = 0 ; i <= FBloomBlendCnt; i++ ) {
		FDDDD->Get3DDevice()->StretchRect(FTmpRendTarget2,NULL,FTmpRendTarget1,NULL,D3DTEXF_NONE);
		pV = (u8*)vertBuff->Lock(0,0,D3DLOCK_DISCARD);
		for( u32 j = 0 ; j <= 5; j++ ) {
			D3DTLVERTEX2& SetTLV = TmpTLV[j];
			if( i % 2 == 0 )
				SetTLV.tu  =  TmpTLV[j].tu+(pow(2.f, (f32)(i / 2))*pow(-1.f,(f32)(i / 2))) / (f32)FTempTex1->GetWidth();
			else
				SetTLV.tv  =  TmpTLV[j].tv+(pow(2.f, (f32)(i / 2))*pow(-1.f,(f32)(i / 2))) / (f32)FTempTex1->GetHeight();

			memmove(pV, &SetTLV, vertBuff->GetVertexSize());
			pV += vertBuff->GetVertexSize();
			TmpTLV[j].color  =  MakeCol(128,255,255,255);
		}
		vertBuff->Unlock();
		//描画
		FDDDD->Get3DDevice()->DrawPrimitive(D3DPT_TRIANGLELIST,0, 2);
	}

	//レンダリングターゲットを元に戻す
	FDDDD->Get3DDevice()->SetRenderTarget(0, FDDDD->GetBackBuffers(0));

	//テクスチャサンプラーを元に戻す
	FDDDD->Get3DDevice()->SetSamplerState(0,D3DSAMP_ADDRESSU,UINT(D3DTADDRESS_WRAP));
	FDDDD->Get3DDevice()->SetSamplerState(0,D3DSAMP_ADDRESSV,UINT(D3DTADDRESS_WRAP));


	FBMCHFlg  =  bmMax;
	FTexCHFlg  =  FTempTex2;
	FChangeCHFlg = true;

	SAFE_DELETE(vertBuff);
	FDDDD->Get3DDevice()->SetStreamSource(0,FVertBuff->GetVertexBuffer(),0,FVertBuff->GetVertexSize());
	FDDDD->Get3DDevice()->SetFVF(FVertBuff->GetFVF());

}

void TSpriteEXManager::SetBloomLevel( u32 _level )
{
	_level  =  _level & 0xff;

	FBloomLevel  =  0xffffffff - ((_level << 16) | (_level << 8) | _level);

}

void TSpriteEXManager::SetBloomLevelRed( u32 _level )
{
	_level  =  _level & 0xff;

	FBloomLevel  =  FBloomLevel | 0x00ff0000;

	FBloomLevel  =  FBloomLevel - (_level << 16);

}

void TSpriteEXManager::SetBloomLevelGreen( u32 _level )
{
	_level  =  _level & 0xff;

	FBloomLevel  =  FBloomLevel | 0x0000ff00;

	FBloomLevel  =  FBloomLevel - (_level << 8);

}

void TSpriteEXManager::SetBloomLevelBlue( u32 _level )
{
	_level  =  _level & 0xff;

	FBloomLevel  =  FBloomLevel | 0x000000ff;

	FBloomLevel  =  FBloomLevel - _level;

}

void TSpriteEXManager::SetBloomBlend( s32 _cnt )
{
	FBloomBlendCnt  =  _cnt;

}

void TSpriteEXManager::MakeBlur()
{
	FMakeBlurFlag  =  true;

}

void TSpriteEXManager::SetMaxLuminance( s32 _L )
{
	FMaxLuminance  =  _L;

}

void TSpriteEXManager::SetExtraEffect( bool _exeffect, bool _bloom )
{
	bool reset;
	//off → onの場合はテクスチャ再生成が必要
	if( (! FCreateBump) && _exeffect )
		reset = true;
	else
		reset = false;

	FCreateBump = _exeffect;
	FCreateBloom = _bloom;


	if( reset ) InitAfterReset();

}
