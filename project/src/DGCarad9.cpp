#include "DGCarad9.h"
#include "QDArc.h"
#include <string.h>

//ユニットローカルなグローバル変数

//D3DCOLOR(ARGB)をテクスチャ上の色表現にする
u32 D3DColorToPhysical( D3DCOLOR col, D3DFORMAT fmt)
{
	u32 a,r,g,b;
	u32 result;

	switch( fmt ) {
	case D3DFMT_R8G8B8: {
		result = col;
						}
						break;
	case D3DFMT_A8R8G8B8: {
		result = col;
						  }
						  break;
	case D3DFMT_X8R8G8B8: {
		result = col & 0x00FFFFFF;
						  }
						  break;
	case D3DFMT_R5G6B5: {
		r = (col & 0x00FF0000) >> 19; //16+3
		g = (col & 0x0000FF00) >> 10; //8+2
		b = (col & 0x000000FF) >> 3;
		result = (r << 11) | (g << 5) | b;
						}
						break;
	case D3DFMT_X1R5G5B5: {
		r = (col & 0x00FF0000) >> 19; //16+3
		g = (col & 0x0000FF00) >> 11; //8+3
		b = (col & 0x000000FF) >> 3;
		result = (r << 10) | (g << 5) | b;
						  }
						  break;
	case D3DFMT_A1R5G5B5: {
		a = col >> 31; //24+7
		r = (col & 0x00FF0000) >> 19; //16+3
		g = (col & 0x0000FF00) >> 11; //8+3
		b = (col & 0x000000FF) >> 3;
		result = (a << 15) | (r << 10) | (g << 5) | b;
						  }
						  break;
	case D3DFMT_A4R4G4B4: {
		a = col >> 28; //24+4
		r = (col & 0x00FF0000) >> 20; //16+4
		g = (col & 0x0000FF00) >> 12; //8+4
		b = (col & 0x000000FF) >> 4;
		result = (a << 12) | (r << 8) | (g << 4) | b;
						  }
						  break;
	case D3DFMT_R3G3B2: {
		r = (col & 0x00FF0000) >> 21; //16+5
		g = (col & 0x0000FF00) >> 13; //8+5
		b = (col & 0x000000FF) >> 6;
		result = (r << 5) | (g << 2) | b;
						}
						break;
	case D3DFMT_A8: {
		result = col >> 24;
					}
					break;
	case D3DFMT_A8R3G3B2: {
		a = col >> 24;
		r = (col & 0x00FF0000) >> 21; //16+5
		g = (col & 0x0000FF00) >> 13; //8+5
		b = (col & 0x000000FF) >> 6;
		result = (a << 8) | (r << 5) | (g << 2) | b;
						  }
						  break;
	case D3DFMT_X4R4G4B4: {
		r = (col & 0x00FF0000) >> 20; //16+4
		g = (col & 0x0000FF00) >> 12; //8+4
		b = (col & 0x000000FF) >> 4;
		result = (r << 8) | (g << 4) | b;
						  }
						  break;
						  //以下、バンプマップ用
						  //ARGBの、RをL(W)。GをU、BをVとして読む AはAそのままとか、Qとか
	case D3DFMT_X8L8V8U8: {
		result = col & 0x00FFFFFF;
						  }
						  break;
	case D3DFMT_Q8W8V8U8: {
		result = col;
						  }
						  break;

	case D3DFMT_V16U16: {
		g = (col & 0x0000FF00) >> 8;
		b = (col & 0x000000FF);
		result = (g << 24) | (b << 8);
						}
						break;
	case D3DFMT_L6V5U5: {
		r = (col & 0x00FF0000) >> 18; //16+2
		g = (col & 0x0000FF00) >> 11; //8+3
		b = (col & 0x000000FF) >> 3;
		result = (r << 10) | (g << 5) | b;
						}
						break;
	case D3DFMT_V8U8: {
		result = col & 0x0000FFFF;
					  }
					  break;
	case D3DFMT_A8L8: {
		a = (col & 0xFF000000) >> 24;
		r = (col & 0x00FF0000) >> 16;
		result = (a << 8) | r;
					  }
					  break;
	case D3DFMT_L8: {
		result = (col & 0x00FF0000) >> 16;
					}
					break;
	case D3DFMT_A4L4: {
		a = (col & 0xFF000000) >> 28;  //24 + 4;
		r = (col & 0x00FF0000) >> 20;  //16 + 4
		result = (a << 4) | r;
					  }
					  break;

	default:
		result = 0;

	}

	return result;
}

//8ビットARGB値をテクスチャ上の色表現にする
u32 ARGBToPhysical( u32 a, u32 r, u32 g, u32 b, D3DFORMAT fmt)
{
	u32 result;

	switch( fmt ){
	case D3DFMT_R8G8B8: {
		result = (r << 16) | (g << 8) | b;
						}
						break;
	case D3DFMT_A8R8G8B8: {
		result = (a << 24) | (r << 16) | (g << 8) | b;
						  }
						  break;
	case D3DFMT_X8R8G8B8: {
		result = (r << 16) | (g << 8) | b;
						  }
						  break;
	case D3DFMT_R5G6B5: {
		r = r >> 3;
		g = g >> 2;
		b = b >> 3;
		result = (r << 11) | (g << 5) | b;
						}
						break;
	case D3DFMT_X1R5G5B5: {
		r = r >> 3;
		g = g >> 3;
		b = b >> 3;
		result = (r << 10) | (g << 5) | b;
						  }
						  break;
	case D3DFMT_A1R5G5B5: {
		a = a >> 7;
		r = r >> 3;
		g = g >> 3;
		b = b >> 3;
		result = (a << 15) | (r << 10) | (g << 5) | b;
						  }
						  break;
	case D3DFMT_A4R4G4B4: {
		a = a >> 4;
		r = r >> 4;
		g = g >> 4;
		b = b >> 4;
		result = (a << 12) | (r << 8) | (g << 4) | b;
						  }
						  break;
	case D3DFMT_R3G3B2: {
		r = r >> 5;
		g = g >> 5;
		b = b >> 6;
		result = (r << 5) | (g << 2) | b;
						}
						break;
	case D3DFMT_A8: {
		result = a;
					}
					break;
	case D3DFMT_A8R3G3B2: {
		r = r >> 5;
		g = g >> 5;
		b = b >> 6;
		result = (a << 8) | (r << 5) | (g << 2) | b;
						  }
						  break;
	case D3DFMT_X4R4G4B4: {
		r = r >> 4;
		g = g >> 4;
		b = b >> 4;
		result = (r << 8) | (g << 4) | b;
						  }
						  break;

						  //以下、バンプマップ用

	case D3DFMT_X8L8V8U8: {
		result = (r << 16) | (g << 8) | b;
						  }
						  break;
	case D3DFMT_Q8W8V8U8: {
		result = (a << 24) | (r << 16) | (g << 8) | b;
						  }
						  break;
	case D3DFMT_V16U16: {
		result = (g << 24) | (b << 8);
						}
						break;
	case D3DFMT_L6V5U5: {
		r = r >> 2;
		g = g >> 3;
		b = b >> 3;
		result = (r << 10) | (g << 5) | b;
						}
						break;
	case D3DFMT_V8U8: {
		result = (g << 8) | b;
					  }
					  break;
	case D3DFMT_A8L8: {
		result = (a << 8) | r;
					  }
					  break;
	case D3DFMT_L8: {
		result = r;
					}
					break;
	case D3DFMT_A4L4: {
		a = a >> 4;
		r = r >> 4;
		result = (a << 4) | r;
					  }
					  break;
	default:
		result = 0;

	}

	return result;
}

//ビット数を増やして8ビットに(nBitはもとのビット数)
u32 GainBit( u32 c, u32 nBit )
{
	return c * 255 / (1 << (nBit - 1));
}


//物理カラーをD3DCOLORへ。
D3DCOLOR PhysicalToARGB( u32 pcol, D3DFORMAT fmt){

	u32 a, r, g, b;
	u32 result;
	switch( fmt ){
	case D3DFMT_R8G8B8:
	case D3DFMT_X8R8G8B8: {
		result = 0xFF000000 | pcol;
						  }

	case D3DFMT_A8R8G8B8: {
		result = pcol;
						  }
						  break;
	case D3DFMT_R5G6B5: {
		r = GainBit(pcol & 0x0000F800 >> 11, 5);
		g = GainBit(pcol & 0x000003E0 >> 5 , 6);
		b = GainBit(pcol & 0x0000001F       , 5);
		result = 0xFF000000 | (r << 16) | (g << 8) | b;
						}
						break;
	case D3DFMT_X1R5G5B5: {
		r = GainBit(pcol & 0x00007C00 >> 10 ,5);
		g = GainBit(pcol & 0x000001E0 >> 5  ,5);
		b = GainBit(pcol & 0x0000001F        ,5);
		result = 0xFF000000 | (r << 16) | (g << 8) | b;
						  }
						  break;
	case D3DFMT_A1R5G5B5: {
		a = pcol >> 15 & 0xFF;
		r = GainBit(pcol >> 10 & 0x1F , 5);
		g = GainBit(pcol >> 5  & 0x1F , 5);
		b = GainBit(pcol & 0x0000001F  , 5);

		result = (a << 24) | (r << 16) | (g << 8) | b;
						  }
						  break;
	case D3DFMT_A4R4G4B4: {
		a = GainBit(pcol               >> 12, 4);
		r = GainBit(pcol & 0x00000F00 >> 8,  4);
		g = GainBit(pcol & 0x000000F0 >> 4,  4);
		b = GainBit(pcol & 0x0000000F,        4);
		result = (a << 24) | (r << 16) | (g << 8) | b;
						  }
						  break;
	case D3DFMT_X4R4G4B4: {
		r = GainBit(pcol & 0x00000F00 >> 8, 4);
		g = GainBit(pcol & 0x000000F0 >> 4, 4);
		b = GainBit(pcol & 0x0000000F,       4);
		result = 0xFF000000 | (r << 16) | (g << 8) | b;
						  }
						  break;
	case D3DFMT_R3G3B2: {
		r = GainBit(pcol >>  5 & 0x000000E0, 3);
		g = GainBit(pcol >>  2 & 0x0000001C, 3);
		b = GainBit(pcol        & 0x00000003, 2);
		result = 0xFF000000 | (r << 16) | (g << 8) | b;
						}
						break;
	case D3DFMT_A8R3G3B2: {
		a = pcol >> 24;
		r = GainBit(pcol >>  5 & 0x000000E0, 3);
		g = GainBit(pcol >>  2 & 0x0000001C, 3);
		b = GainBit(pcol        & 0x00000003, 2);
		result = (a << 24) | (r << 16) | (g << 8) | b;
						  }
						  break;
	case D3DFMT_A8: {
		result = pcol << 24;
					}
					break;
					//以下、バンプマップ用
	case D3DFMT_X8L8V8U8: {
		result = pcol;
						  }
						  break;
	case D3DFMT_Q8W8V8U8: {
		result = pcol;
						  }
						  break;
	case D3DFMT_V16U16: {
		g = pcol >> 16;
		b = pcol        & 0x000FFFF;
		result = (g << 8) | b;
						}
						break;
	case D3DFMT_L6V5U5: {
		//符号付なので、GainBitは使えない
		r = (pcol >> 10 & 0x0000003F) << 2;
		g = (pcol >>  5 & 0x0000001F) << 3;
		b = (pcol        & 0x0000001F) << 3;
		result = (r << 16) | (g << 8) | b;
						}
						break;
	case D3DFMT_V8U8: {
		result = pcol;
					  }
					  break;
	case D3DFMT_A8L8: {
		result = pcol << 16;
					  }
					  break;
	case D3DFMT_L8: {
		result = pcol << 16;
					}
					break;
	case D3DFMT_A4L4: {
		a = GainBit(pcol >> 4 & 0x0000000F, 4);
		r = GainBit(pcol       & 0x0000000F, 4);
		result = (a << 24) | (r << 16);
					  }
					  break;
	default:
		result = 0;

	}

	return result;
}


//このフォーマットは色表現用のフォーマットか？
bool IsARGBFormat(D3DFORMAT fmt)
{
	return  fmt == D3DFMT_R8G8B8 
		|| fmt == D3DFMT_A8R8G8B8
		|| fmt == D3DFMT_R5G6B5
		|| fmt == D3DFMT_X1R5G5B5
		|| fmt == D3DFMT_A1R5G5B5
		|| fmt == D3DFMT_A4R4G4B4
		|| fmt == D3DFMT_A8
		|| fmt == D3DFMT_A8R3G3B2
		|| fmt == D3DFMT_X4R4G4B4
		;
}

//１ピクセルあたりのビット数を得る
u32 GetPixelBits( D3DFORMAT fmt )
{
	u32 result;

	switch( fmt ) {
	case D3DFMT_R8G8B8:    result = 24; break;
	case D3DFMT_A8R8G8B8:  result = 32; break;
	case D3DFMT_X8R8G8B8:  result = 32; break;
	case D3DFMT_R5G6B5:    result = 16; break;
	case D3DFMT_X1R5G5B5:  result = 16; break;
	case D3DFMT_A1R5G5B5:  result = 16; break;
	case D3DFMT_A4R4G4B4:  result = 16; break;
	case D3DFMT_R3G3B2:    result = 8;  break;
	case D3DFMT_A8:        result = 8;  break;
	case D3DFMT_A8R3G3B2:  result = 16; break;
	case D3DFMT_X4R4G4B4:  result = 16; break;
	case D3DFMT_A8P8:      result = 16; break;
	case D3DFMT_P8:        result = 8;  break;
	case D3DFMT_L8:        result = 8;  break;
	case D3DFMT_A8L8:      result = 16; break;
	case D3DFMT_A4L4:      result = 8;  break;
	case D3DFMT_V8U8:      result = 16; break;
	case D3DFMT_L6V5U5:    result = 16; break;
	case D3DFMT_X8L8V8U8:  result = 32; break;
	case D3DFMT_Q8W8V8U8:  result = 32; break;
	case D3DFMT_V16U16:    result = 32; break;
	case D3DFMT_D16_LOCKABLE:  result = 16; break;
	case D3DFMT_D32:       result = 32; break;
	case D3DFMT_D15S1:     result = 16; break;
	case D3DFMT_D24S8:     result = 32; break;
	case D3DFMT_D16:       result = 16; break;
	case D3DFMT_D24X8:     result = 32; break;
	case D3DFMT_D24X4S4:   result = 32; break;
	default:
		result = 0;
	}

	return result;
}

//ロックした範囲にBMPを読み込む
void LoadBMPStreamToLockedRect(D3DLOCKED_RECT& LockedRect, Rect& lockedRegion, D3DSURFACE_DESC ddsd, istream& source)
{
	BITMAPINFOHEADER bih;
	BITMAPFILEHEADER bfh;
	u32 bypp;    //サーフェスのBytes/Pix
	u32 fbypp;   //ファイルのBytes/Pix
	TRGBQUAD pal[256];

	u32 bpl,fbpl;
	u32 trBytes;          //転送するバイト数/行
	s32 trHeight,trWidth; //転送するサイズ
	u8* pix;
	u8* buf;
	u8* dest;


	//ファイル＝8ビットの場合の変換ルーチン
#define Process8bit( pix, buf )																					\
	{																											\
		u8* pSrc;																								\
		u8* pDest;																								\
		u8	idx; 																								\
		u32 phys; 																								\
		pSrc = (u8*)buf;																						\
		pDest = (u8*)pix;																						\
		for( s32 i = 0; i < trWidth; ++i ) {																	\
			idx = *pSrc;																						\
			phys = ARGBToPhysical(0xFF, pal[idx].rgbRed, pal[idx].rgbGreen, pal[idx].rgbBlue, ddsd.Format);		\
			memmove( (void*)pDest, &phys, bypp );																\
			pSrc++;																								\
			pDest += bypp;																						\
		}																										\
	}																													

	//ファイル＝16ビットの場合の変換ルーチン
#define Process16bit( pix, buf )					\
	{												\
		u16* pSrc;									\
		u8	*pDest;									\
		u16 logi;  									\
		u32 phys;									\
		pSrc = (u16*)buf;							\
		pDest = (u8*)pix;							\
		for( s32 i = 0; i < trWidth; i++ ) {		\
			logi = *pSrc;							\
			/*ARGB1555として読み込み*/				\
			phys = ARGBToPhysical(					\
				logi >> 15,							\
				(logi & 0x7C00) >> 10,				\
				(logi & 0x03E0) >> 5,				\
				(logi & 0x001F),					\
				ddsd.Format);						\
			memmove(pDest, &phys, bypp);			\
			pSrc++;									\
			pDest += bypp;							\
		}											\
	}																

	//ファイル＝24ビットの場合の変換ルーチン
#define Process24bit( pix, buf )							\
	{														\
		u8* pSrc;											\
		u8* pDest;											\
		u32 logi;   /*ファイルから読んだ論理カラー*/		\
		u32 phys;  /*サーフェスに書き込む物理カラー*/		\
		pSrc = (u8*)buf;									\
		pDest = (u8*)pix;									\
		for( s32 i = 0; i < trWidth; i++ ) {				\
			memmove(&logi, pSrc, 3);						\
			phys = ARGBToPhysical(							\
				0xFF,										\
				(logi & 0x00FF0000) >> 16,					\
				(logi & 0x0000FF00) >> 8,					\
				(logi & 0x000000FF),						\
				ddsd.Format);								\
			memmove(pDest, &phys, bypp);					\
			pSrc += 3;										\
			pDest += bypp;									\
		}													\
	}														

	//ファイル＝32ビットの場合の変換ルーチン
#define Process32bit( pix, buf )								\
	{															\
		u32*	pSrc;											\
		u8*		pDest;											\
		u32		logi;   /*ファイルから読んだ論理カラー*/		\
		u32		phys;  /*サーフェスに書き込む物理カラー*/		\
		pSrc = (u32*)buf;										\
		pDest = (u8*)pix;										\
		for( s32 i = 0; i < trWidth; ++i ) {					\
			logi = *pSrc;										\
			phys = D3DColorToPhysical(logi, ddsd.Format);		\
			memmove(pDest, &phys, bypp);						\
			pSrc++;												\
			pDest += bypp;										\
		}														\
	}

	//bufの中身を、pixに変換する
#define ProcessLine( pix, buf)						\
	{												\
		switch( fbypp )								\
		{											\
		case 1:Process8bit(pix, buf);	break;		\
		case 2:Process16bit(pix, buf);	break;		\
		case 3:Process24bit(pix, buf);	break;		\
		case 4:Process32bit(pix, buf);	break;		\
		}											\
	}

	try{
		//BMPファイルヘッダを読む(IDのチェック)
		source.read(reinterpret_cast<char*>(&bfh), sizeof(bfh));
	}
	catch(std::istream::failure e){
		//PutDebugMessage("ビットマップの読みこみ中に、エラーが発生しました");
		return;
	}

	if( bfh.bfType != 0x4d42 ) {
		//PutDebugMessage("不正なBMPファイルです");
		return;
	}

	//BMPインフォヘッダの読み込み
	try{
		source.read(reinterpret_cast<char*>(&bih), sizeof(bih));
	}
	catch(std::istream::failure e){
		//PutDebugMessage("LoadBMP中に、読み込みエラーが発生しました");
		return;
	}

	//8ビット～32ビット以外のBMPはダメ
	if((bih.biBitCount < 8) || (bih.biBitCount > 32) ) {
		//TRYDG(1, "色数が8ビット～32ビットの範囲のビットマップを、LoadBMPには指定してください");
		return;
	}


	//サーフェスの bypp を得る
	bypp = GetPixelBits(ddsd.Format) / 8;

	//ファイルの byppを 得る
	fbypp  =  bih.biBitCount / 8;


	if(ddsd.Format == D3DFMT_P8 ) {

		//{ サーフェスが8ビットパレットつきの場合 }

		if((fbypp>1) ) {
			//{ ファイルがハイカラー以上なら、転送できません }
			//TRYDG(1, "8ビットパレット付きテクスチャに対して16ビット以上のBMPをロードする事は出来ません");
			return;

		} else {
			//{ ファイルも 1bypp なら、直接流し込む }

			try{
				source.read(reinterpret_cast<char*>(&pal[0]),1024);
			}
			catch(std::istream::failure e){
				//PutDebugMessage("LoadBMP中に、読み込みエラーが発生しました");
				return;
			}

			source.seekg(bfh.bfOffBits, std::ios::beg);

			//イメージバッファの作成
			bpl  =  ((u32)(bih.biWidth)*bypp+3) & 0xFFFFFFFC;
			fbpl  =  ((u32)(bih.biWidth)*fbypp+3) & 0xFFFFFFFC;

			pix = (u8*)malloc(bpl);
			buf = (u8*)malloc(fbpl);


			//DIBを上下さかさまにコピー
			trBytes  =  LockedRect.Pitch;

			//転送するバイト数は、テクスチャの１ライン分の情報量と、ビットマップのそれうち、少ないほう
			if(trBytes > bpl)
				trBytes  =  bpl;

			//読み込むライン数は、ロック範囲の行数と、ビットマップの行数のうち、少ないほう
			trHeight = lockedRegion.Bottom - lockedRegion.Top;
			if(trHeight > bih.biHeight )
				trHeight  =  bih.biHeight;

			trWidth = lockedRegion.Right - lockedRegion.Left;
			if(trWidth > bih.biWidth )
				trWidth = bih.biWidth;

			dest = (u8*)LockedRect.pBits;
			dest += LockedRect.Pitch * (trHeight-1);

			//読み込み元ビットマップに対して、転送範囲が小さい場合は、読み込み元をずらす(DIBが上下逆だから)
			if(bih.biHeight > trHeight )
				source.seekg( fbpl * (bih.biHeight - trHeight), std::ios::cur);

			try{
				for( s32 i = 0; i < trHeight; i++ ) {
					source.read(reinterpret_cast<char*>(buf), fbpl);
					//if(bpp=fbpp )
					memmove(dest, buf, trBytes);
					//else {
					//  convDIBLine(pix,buf,bih.biWidth,bpp,fbpp,pal,ddsd.ddpfPixelFormat);
					//  Move(pix^,dest^,pitch);
					//}

					dest -= LockedRect.Pitch;
				}
			}
			catch(std::istream::failure e){
				//PutDebugMessage("LoadBMP中に、読み込みエラーが発生しました");
				return;
			}
			free(pix);
			free(buf);


		}
	} else {
		//{サーフェスがパレット付きではない場合}

		//パレットの読み込み(読み込み元がパレット付きBMPの時)
		if(fbypp == 1 ) {
			try{
				source.read(reinterpret_cast<char*>(&pal[0]), 1024);
			}
			catch(std::istream::failure e){
				//PutDebugMessage("LoadBMP中に、読み込みエラーが発生しました");
				return;
			}
		}

		source.seekg( bfh.bfOffBits, std::ios::beg );

		//イメージバッファの作成
		bpl  =  ((u32)(bih.biWidth)*bypp+3) & 0xFFFFFFFC;
		fbpl  =  ((u32)(bih.biWidth)*fbypp+3) & 0xFFFFFFFC;

		pix = (u8*)malloc(bpl);
		buf = (u8*)malloc(fbpl);

		//DIBを上下さかさまにコピー
		trBytes  =  LockedRect.Pitch;
		if((trBytes > bpl) )
			trBytes  =  bpl;

		//読み込むライン数は、ロック範囲の行数と、ビットマップの行数のうち、少ないほう
		trHeight = lockedRegion.Bottom - lockedRegion.Top;
		if(trHeight > bih.biHeight )
			trHeight  =  bih.biHeight;

		trWidth = lockedRegion.Right - lockedRegion.Left;
		if(trWidth > bih.biWidth )
			trWidth = bih.biWidth;

		dest = (u8*)LockedRect.pBits;
		dest += LockedRect.Pitch * (trHeight-1);

		//読み込み元ビットマップに対して、転送範囲が小さい場合は、読み込み元をずらす(DIBが上下逆だから)
		if(bih.biHeight > trHeight )
			source.seekg( fbpl * (bih.biHeight - trHeight), std::ios::cur );     

		try{
			for( s32 i = 0; i < trHeight; i++ ) {
				source.read(reinterpret_cast<char*>(buf), fbpl);

				ProcessLine(pix, buf);    //変換
				//if(bpp=fbpp )
				memmove(dest, pix, trBytes);
				//else {
				//  convDIBLine(pix,buf,bih.biWidth,bpp,fbpp,pal,ddsd.ddpfPixelFormat);
				//  Move(pix^,dest^,pitch);
				//}
				dest -= LockedRect.Pitch;
			}
		}
		catch(std::istream::failure e){
			//PutDebugMessage("LoadBMP中に、読み込みエラーが発生しました");
			return;
		}

		free(pix);
		free(buf);

	}

}

//テクスチャにストリームからBMPを読み込む
void LoadBMPStreamToTexture(IDirect3DTexture9* texture, istream& source )
{
	D3DSURFACE_DESC ddsd;
	D3DLOCKED_RECT lockRect;

	//読込先テクスチャのサイズをゲッチュ
	texture->GetLevelDesc(0, &ddsd);

	//サーフェスのロック
	texture->LockRect(0, &lockRect, NULL, 0);


	//読み込み
	LoadBMPStreamToLockedRect(lockRect, Rect(0, 0, ddsd.Width, ddsd.Height), ddsd, source);

	//アンロック
	texture->UnlockRect(0);
}

void TDGCarad::SetDebugOption( TDGDebugOption Value )
{
	FDebugOption  =  Value;

	DGCarad_HaltOnError  = (Value & dgoHaltOnError) != 0;
	DGCarad_DetailedLog  = (Value & dgoDetailedLog) != 0;
	DGCarad_DebugMessage  = (Value & dgoDebugMessage) != 0;

}


//一番いいDepth/Stencilバッファフォーマットを得る
D3DFORMAT TDGCarad::BestZSFormat( D3DDEVTYPE devType, vector<D3DFORMAT>& depthStencilFmt, D3DPRESENT_PARAMETERS& prep, u32 Adapter )
{
	D3DFORMAT result = D3DFMT_UNKNOWN;
	if(prep.EnableAutoDepthStencil ) {
		for( u32 i = 0; i < depthStencilFmt.size(); i++ ) {
			if(FD3D->CheckDepthStencilMatch(Adapter, devType, prep.BackBufferFormat, prep.BackBufferFormat, depthStencilFmt[i]) == D3D_OK ) {
				return depthStencilFmt[i];
			}
		}
	}
//	PutTestMessage("希望のデプスステンシルサーフェスのフォーマットが見つかりませんでした");

	return result;
}

void TDGCarad::SetDisplayMode( u32 Adapter, bool windowed, D3DDISPLAYMODE mode, D3DMULTISAMPLE_TYPE multiSampleType, vector<D3DFORMAT>& depthStencilFmt, D3DSWAPEFFECT swapEffect, u32 presentationInterval )
{
	D3DPRESENT_PARAMETERS prep;
	D3DCAPS9 caps;
	D3DADAPTER_IDENTIFIER9 ai;
	string drvName;
	string drvDesc;

	//PutTestMessage("");
	//PutTestMessage("■■■画面モードを初期化します■■■");
	//DGCarad_debugIndent++;

	//プレゼンテーションパラメータ設定
	ZeroMemory(&prep, sizeof(prep));
	prep.Windowed = windowed;
	prep.SwapEffect = swapEffect;
	prep.BackBufferWidth = mode.Width;
	prep.BackBufferHeight = mode.Height;
	prep.BackBufferFormat = mode.Format;
	prep.MultiSampleType = multiSampleType;
	prep.PresentationInterval = presentationInterval;


	//ドライバ識別情報の表示
	FD3D->GetAdapterIdentifier(Adapter, 0, &ai);
	drvName = string(ai.Driver);
	drvDesc = string(ai.Description);

	//PutTestMessage("Driver : %s", drvName);
	//PutTestMessage(Format("Description : %s", [drvDesc]));
	//PutTestMessage(Format("VendorID : %.8x, DeviceID : %.8x, SubSysID : %.8x, Revision : %.8x", [ai.VendorID,ai.DeviceID,ai.SubsysID,ai.Revision]));

	if(windowed ) {
		//PutTestMessage(Format("ウィンドウモード %dx%dで初期化します",[mode.Width, mode.Height]));
	} else {
		//PutTestMessage(Format("フルスクリーンモード %dx%d %dHz %s で初期化します",[mode.Width,mode.Height,mode.RefreshRate,D3DFormatToString(mode.Format)]));    prep.BackBufferWidth = mode.Width;
		prep.FullScreen_RefreshRateInHz = mode.RefreshRate;
	}

	prep.hDeviceWindow = m_owner.m_handle;
	prep.EnableAutoDepthStencil  =  (depthStencilFmt.size() != 0);

	if((FD3D->GetDeviceCaps(Adapter, D3DDEVTYPE_HAL, &caps) == D3D_OK) && (! FUseREFDevice) ) {//= ) {// D3D_OK ) {
		//HALはつかえるらしいね
		//PutTestMessage("ハードウェアアクセラレーションを利用します");

		prep.AutoDepthStencilFormat = BestZSFormat(D3DDEVTYPE_HAL, depthStencilFmt, prep, Adapter );
		//PutTestMessage("デプスステンシルサーフェスのフォーマット..." + D3DFORMATToString(prep.AutoDepthStencilFormat));

		//H/W T&L に対応してるなら、H/W T&Lさせる
		if((caps.DevCaps && (u32)(D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0) || (FUseSoftwareVertexProcessing) ) {
			//PutTestMessage("ソフトウェアT&Lを利用します");
			CreateDevice(Adapter, D3DDEVTYPE_HAL, m_owner.m_handle, D3DCREATE_SOFTWARE_VERTEXPROCESSING, prep);
		} else {
			//PutTestMessage("ハードウェアT&Lを利用します");
			CreateDevice(Adapter, D3DDEVTYPE_HAL, m_owner.m_handle, D3DCREATE_HARDWARE_VERTEXPROCESSING, prep);
		}

	} else {
		//HELのみ、ショボ
		//PutTestMessage("ソフトウェアエミュレーションを利用します");

		prep.AutoDepthStencilFormat = BestZSFormat(D3DDEVTYPE_REF, depthStencilFmt, prep, Adapter );
		//PutTestMessage("デプスステンシルサーフェスのフォーマット..." + D3DFORMATToString(prep.AutoDepthStencilFormat));

		CreateDevice(Adapter, D3DDEVTYPE_REF, m_owner.m_handle, D3DCREATE_SOFTWARE_VERTEXPROCESSING, prep);
	}

	//DGCarad_debugIndent--;
	//PutTestMessage("■■■画面モードを初期化しました■■■");
	//PutTestMessage("");

}

IDirect3DSurface9* TDGCarad::GetBackBuffers( s32 idx )
{
	return FBackBuffers[idx];
}

void TDGCarad::CleanupResources()
{
	//PutTestMessage("全てのDGCaradリソースを解放します");

	for( u32 i = 0; i < Resources.size(); ++i ){
		Resources[i]->resource->Cleanup();
	}
}

void TDGCarad::RecoverResources()
{
	//PutTestMessage("全てのDGCaradリソースを再構築します");

	for( u32 i = 0; i < Resources.size(); ++i ){
		Resources[i]->resource->Recover();
	}
}

void TDGCarad::Loaded()
{
	//u16 cw;  //FPUのコントロールワード指定用

	////以下は、実行時のみ
	//if(! (csDesigning in ComponentState) ) {
	//	FD3DDevice = NULL;
	//	FBackBuffers = NULL;
	//	FDepthStencilSurface = NULL;
	//	FBackBufferCount = 0;

	//	//プロパティハンドラを呼んで、各グローバル変数に適用
	//	DebugOption = DebugOption;

	//	//8087CW(0x133f) と同じ、OpenGL、Direct3Dとかを使うときのおまじない
	//	//「不正な浮動小数演算命令」が出なくなる
	//	cw = 0x133f;
	//	asm
	//		FNCLEX
	//		FLDCW cw;
	//}

	//Resources = TList.Create;
	//NextResourceID = 0;

	//PutTestMessage("");
	//PutTestMessage("■■■起動時の初期化をします■■■");
	//Inc(DGCarad_debugIndent);

	////Direct3Dの生成
	//PutTestMessage("Direct3Dを生成します");
	//FD3D = Direct3DCreate9(D3D_SDK_VERSION);
	//if(D3D = NULL ) {
	//	TRYDG(1, "Direct3Dオプジェクトすら生成できません、論外");
	//	return;
	//}

	////最初はウィンドウモードで初期化しとく
	//PutTestMessage("ウィンドウのクライアント領域のサイズに合わせて初期化を開始します");
	//WindowMode(FOwner.ClientWidth, FOwner.ClientHeight, []);

	//Dec(DGCarad_debugIndent);
	//PutTestMessage("■■■DGCarad 起動しました■■■");
	//PutTestMessage("");
}

TDGCarad::TDGCarad( WindowInfo& _owner )
{
	m_owner = _owner;

	//コンポーネントのデフォルト値をセット
	FDebugOption = dgoHaltOnError | dgoDetailedLog | dgoDebugMessage;
	FUseREFDevice = false;
	FUseSoftwareVertexProcessing = false;

	//u16 cw;  //FPUのコントロールワード指定用

	//以下は、実行時のみ
		FD3DDevice = NULL;
		FDepthStencilSurface = NULL;
		FBackBufferCount = 0;

		//プロパティハンドラを呼んで、各グローバル変数に適用
		//DebugOption = DebugOption;

		//8087CW(0x133f) と同じ、OpenGL、Direct3Dとかを使うときのおまじない
		//「不正な浮動小数演算命令」が出なくなる
		//cw = 0x133f;
		//asm
		//	FNCLEX
		//	FLDCW cw;

	NextResourceID = 0;

	//PutTestMessage("");
	//PutTestMessage("■■■起動時の初期化をします■■■");
	//Inc(DGCarad_debugIndent);

	//Direct3Dの生成
	//PutTestMessage("Direct3Dを生成します");
	FD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(FD3D == NULL ) {
		//TRYDG(1, "Direct3Dオプジェクトすら生成できません、論外");
		return;
	}

	//最初はウィンドウモードで初期化しとく
	//PutTestMessage("ウィンドウのクライアント領域のサイズに合わせて初期化を開始します");
	//WindowMode(FOwner.ClientWidth, FOwner.ClientHeight, []);

}

TDGCarad::~TDGCarad()
{
		//PutTestMessage("");
		//PutTestMessage("■■■お別れの時がやってまいりました■■■");
		//Inc(DGCarad_debugIndent);

		//メモリリークのチェック
		//while( Resources.size() > 0 ) {
		//	//PutTestMessage(Format("ID %d(%s) のDGCaradリソースは、明示的に解放されていません",[TDGResourceDesc(Resources[0]).id, TDGResourceDesc(Resources[0]).debugName]));
		//	//暗黙のうちに解放
		//	free (((TDGResourceDesc*)Resources[0])->resource);
		//}

		//PutTestMessage("DGCaradオブジェクトの解放に伴い、全ての関連オブジェクトを解放します");
		ReleaseDevice();

		//PutTestMessage("Direct3Dオブジェクトの解放を行います");
		//TRYDG(FD3D->Release, "Direct3Dオブジェクトの解放に失敗しました");
		FD3D->Release();

		for( u32  i = 0; i < Resources.size(); i++ ){
			SAFE_DELETE(Resources[i]->resource);
			SAFE_DELETE(Resources[i]);
		}
		Resources.clear();

		//Dec(DGCarad_debugIndent);
		//PutTestMessage("■■■それではごきげんよう■■■");
		//PutTestMessage("");

}

u32 TDGCarad::RegisterResouce( TDGCommonResource* res, string debugName /*= ""*/ )
{
	TDGResourceDesc* p;

	//PutTestMessage(Format("DGCaradリソース(%s)を登録します ID は %d(%s) です", [res.ClassName, NextResourceID, debugName]));
	u32 result = NextResourceID;

	p = new TDGResourceDesc();
	p->id = NextResourceID;
	p->resource = res;
	p->debugName = debugName;
	Resources.push_back(p);

	NextResourceID++;

	return result;
}

void TDGCarad::UnregisterResource( u32 id )
{
	vector<TDGResourceDesc*>::iterator itr;
	for( itr = Resources.begin(); itr != Resources.end(); ++itr ) {
		if(((TDGResourceDesc*)(*itr))->id == id ) {
			//PutTestMessage(Format("DGCaradリソース(%s) : ID %d(%s) を登録解除します", [((TDGResourceDesc*)Resources[i])->resource.ClassName, id, ((TDGResourceDesc*)Resources[i])->debugName]));
			SAFE_DELETE(((TDGResourceDesc*)(*itr))->resource);
			SAFE_DELETE(*itr);
			Resources.erase(itr);
			return;
		}
	}
	//TRYDG(1, Format("リストに入ってないDGCaradリソース(ID:%d)が、UnregisterResource手続きに渡されました",[id]));

}

void TDGCarad::CreateDevice( u32 Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, u32 BehaviorFlags, D3DPRESENT_PARAMETERS& presentationParameters )
{
	//既に確保されているデバイスオブジェクトの解放
	if(FD3DDevice != NULL ) {
		//PutTestMessage("新しいデバイスの生成の前に、全ての関連オブジェクトを解放します");
		ReleaseDevice();
	}

	//Direct3Dデバイスの作成
	//PutTestMessage("Direct3DDeviceオブジェクトの生成を行います");
	FD3D->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, &presentationParameters, &FD3DDevice);


	//BackBuffer、ステンシルバッファをゲット
	FBackBufferCount = presentationParameters.BackBufferCount;
	FBackBuffers.resize( FBackBufferCount );

	for( u32  i = 0 ; i < FBackBufferCount; ++i )
		FD3DDevice->GetBackBuffer(0, i, D3DBACKBUFFER_TYPE_MONO, &FBackBuffers[i]);

	if(FD3DDevice->GetDepthStencilSurface(&FDepthStencilSurface) != D3D_OK )
		FDepthStencilSurface = NULL;

	//プレゼンテーションパラメータの保存
	FLastPP = presentationParameters;

	//いろいろ覚えとく
	FDeviceType = DeviceType;
	FAdapter = Adapter;

	FWindowed = FLastPP.Windowed;
	FBackBufferWidth = FLastPP.BackBufferWidth;
	FBackBufferHeight = FLastPP.BackBufferHeight;
	FBackBufferFormat = FLastPP.BackBufferFormat;

	//リソースの再構築
	RecoverResources();

}

void TDGCarad::ReleaseDevice()
{
	//PutTestMessage("");
	//PutTestMessage("■■■Direct3Dデバイスを解放します■■■");
	//Inc(DGCarad_debugIndent);

	//リソースの解放
	CleanupResources();

	//デプスステンシルサーフェスの解放
	if(FDepthStencilSurface != NULL ) {
		//PutTestMessage("デプスステンシルサーフェスを解放します");
		//TRYDG(FDepthStencilSurface.Release, "デプスステンシルサーフェスの解放に失敗しました");
		FDepthStencilSurface->Release();
		FDepthStencilSurface = NULL;
	}

	//バックバッファの解放
	if(!FBackBuffers.empty()) {
		//PutTestMessage("バックバッファを解放します");
		for( u32  i = 0; i < FBackBufferCount; i++ ){
			//TRYDG(BackBuffers[i].Release, "バックバッファ #" + IntToStr(i) + "　の解放に失敗しました");
			FBackBuffers[i]->Release();
		}
		FBackBuffers.clear();
	}

	if(FD3DDevice != NULL ) {
		//PutTestMessage("Direct3DDeviceオブジェクトの解放を行います");
		//TRYDG(FD3DDevice->Release, "Direct3DDeviceオブジェクトの解放に失敗しました");
		FD3DDevice->Release();
		FD3DDevice = NULL;
	}

	//Dec(DGCarad_debugIndent);
	//PutTestMessage("■■■Direct3Dデバイスを解放しました■■■");
	//PutTestMessage("");

}

void TDGCarad::WindowMode( u32 width, u32 height, vector<D3DFORMAT>& depthStencilFmt, u32 presentationInterval /*= D3DPRESENT_INTERVAL_DEFAULT*/, D3DSWAPEFFECT swapEffect /*= D3DSWAPEFFECT_DISCARD*/, D3DMULTISAMPLE_TYPE multiSampleType /*= D3DMULTISAMPLE_NONE*/, u32 Adapter /*= D3DADAPTER_DEFAULT*/ )
{
	//PutTestMessage("");
	//PutTestMessage("■■■Windowモードへ移行します■■■");
	//Inc(DGCarad_debugIndent);

	//初期化前にデバイスオブジェクトを全部消す…こうしないと、フルスクリーンからの変更は「現在のディスプレイモード」が得られない
	ReleaseDevice();

	D3DDISPLAYMODE mode = GetCurrentDisplayMode(Adapter);
	mode.Width = width;
	mode.Height = height;

	SetDisplayMode(Adapter, true, mode, multiSampleType, depthStencilFmt, swapEffect, presentationInterval);

	//なぜかSetWindowPosじゃないとちゃんと位置・サイズが変わってくれない(^^;)
	SetWindowPos(
		m_owner.m_handle,
		HWND_NOTOPMOST,
		m_owner.m_left,
		m_owner.m_top,
		width - UINT(m_owner.m_clientWidth - m_owner.m_width),
		height - UINT(m_owner.m_clientHeight - m_owner.m_height),
		SWP_SHOWWINDOW);


	//Dec(DGCarad_debugIndent);
	//PutTestMessage("■■■Windowモードへ移行しました■■■");
	//PutTestMessage("");

}

void TDGCarad::FullscreenMode( u32 width, u32 height, D3DFORMAT format, vector<D3DFORMAT>& depthStencilFmt, u32 refreshRate /*= 0*/, u32 presentationInterval /*= D3DPRESENT_INTERVAL_IMMEDIATE*/, D3DSWAPEFFECT swapEffect /*= D3DSWAPEFFECT_DISCARD*/, D3DMULTISAMPLE_TYPE multiSampleType /*= D3DMULTISAMPLE_NONE*/, u32 Adapter /*= D3DADAPTER_DEFAULT*/ )
{
	//PutTestMessage("");
	//PutTestMessage("■■■フルスクリーンモードへ移行します■■■");
	//Inc(DGCarad_debugIndent);

	D3DDISPLAYMODE mode;
	mode.Width = width;
	mode.Height = height;
	mode.RefreshRate = refreshRate;
	mode.Format = format;
	SetDisplayMode(Adapter, false, mode, multiSampleType, depthStencilFmt, swapEffect, presentationInterval);

	//Dec(DGCarad_debugIndent);
	//PutTestMessage("■■■フルスクリーンモードへ移行しました■■■");
	//PutTestMessage("");

}

void TDGCarad::Reset()
{
	//PutTestMessage("");
	//PutTestMessage("■■■Direct3DDeviceをリセットします■■■");
	//Inc(DGCarad_debugIndent);


	//リソースのリセット準備
	for( u32  i = 0; i < Resources.size(); ++i ) {
		Resources[i]->resource->Reset();
	}


	//バックバッファの解放
	for( u32  i = 0; i < FBackBufferCount; ++i ) {
		FBackBuffers[i]->Release();
		FBackBuffers[i] = NULL;
	}

	//デプスステンシルサーフェスの解放
	if(FDepthStencilSurface != NULL ) {
		FDepthStencilSurface->Release();
		FDepthStencilSurface = NULL;
	}

	//TRYDG(FD3DDevice->Reset(FLastPP), "リセットに失敗しました");
	FD3DDevice->Reset(&FLastPP);

	for( u32 i = 0 ; i < FBackBufferCount; ++i )
		FD3DDevice->GetBackBuffer(0, i, D3DBACKBUFFER_TYPE_MONO, &FBackBuffers[i]);


	FD3DDevice->GetDepthStencilSurface(&FDepthStencilSurface);


	//リソースのリセット後イベントを呼ぶ
	for( s32  i = 0; Resources.size(); ++i ) {
		Resources[i]->resource->AfterReset();
	}

	//Dec(DGCarad_debugIndent);
	//PutTestMessage("■■■Direct3DDeviceをリセットしました■■■");
	//PutTestMessage("");

}

D3DDISPLAYMODE TDGCarad::GetCurrentDisplayMode( u32 Adapter )
{
	D3DDISPLAYMODE dm;
	FD3D->GetAdapterDisplayMode(Adapter, &dm);
	return dm;
}

bool TDGCarad::IsAvailableDisplayMode( u32 Adapter, u32 screenWidth, u32 screenHeight, u32 freq, D3DFORMAT fmt )
{
	D3DDISPLAYMODE mode;  //入力されたディスプレイモード
	D3DDISPLAYMODE enumMode;  //列挙されたモード
	mode.Width = screenWidth;
	mode.Height = screenHeight;
	mode.RefreshRate = freq;
	mode.Format = fmt;

	u32 nMode = FD3D->GetAdapterModeCount(Adapter, fmt);

	for( u32  i = 0; i < nMode; ++i ) {
		FD3D->EnumAdapterModes(Adapter, fmt, i, &enumMode);
		if(memcmp(&enumMode, &mode, sizeof(D3DDISPLAYMODE)) == 0) {
			return true;
		}
	}

	return false;
}


u32 TDGTexture::GetTexels( u32 level, s32 x, s32 y )
{
	u8* p;
	u32 result = 0;

	if( FLocked ) {
		if((x >= FLockedRegion.Right) || (x < FLockedRegion.Left) || (y < FLockedRegion.Top) || (y >= FLockedRegion.Bottom) ) {
			//TRYDG(1, "ロックした範囲外のテクセルを操作ようとしました");
			return result;
		}

		if(FLockedLevel != level ) {
			//TRYDG(1, "ロックしたレベルと同じレベルでテクセルを操作してください");
			return result;
		}

		p = (u8*)FLockedRect.pBits;
		p += (y - FLockedRegion.Top) * FLockedRect.Pitch + (x-FLockedRegion.Left)*FPixelSize;
		memmove(&result, p, FPixelSize);
	} else {
		LockRect( NULL, level, Rect(x,y,x,y), D3DLOCK_READONLY);
		memmove(&result, FLockedRect.pBits, FPixelSize);
		UnlockRect();
	}
	return result;
}

void TDGTexture::SetTexels( u32 level, s32 x, s32 y, u32 Value )
{
	u8* p;
	if(FLocked ) {
		if((x >= FLockedRegion.Right) || (x < FLockedRegion.Left) || (y < FLockedRegion.Top) || (y >= FLockedRegion.Bottom) ) {
			//TRYDG(1, "ロックした範囲外のテクセルを操作ようとしました");
			return;
		}

		if(FLockedLevel != level ) {
			//TRYDG(1, "ロックしたレベルと同じレベルでテクセルを操作してください");
			return;
		}

		p = (u8*)FLockedRect.pBits;
		p += (y-FLockedRegion.Top)*FLockedRect.Pitch + (x-FLockedRegion.Left)*FPixelSize;
		memmove(p, &Value, FPixelSize);
	} else {
		LockRect(NULL, level, Rect(x,y,x,y), 0);
		memmove(FLockedRect.pBits, &Value, FPixelSize);
		UnlockRect();
	}

}

D3DFORMAT TDGTexture::SearchBestFormat()
{
	D3DSURFACE_DESC desc;
	//デフォルトでは、バックバッファのフォーマット
	FOwner->GetBackBuffers(0)->GetDesc(&desc);
	D3DFORMAT result = desc.Format;

	//使えるフォーマットを第一希望から探索
	if( !FormatList.empty() ) {
		for( u32 i = 0; i < FormatList.size(); i++ ) {
			if(FOwner->GetD3D()->CheckDeviceFormat(FOwner->GetAdapter(), FOwner->GetDeviceType(), desc.Format, FUsage, D3DRTYPE_TEXTURE, FormatList[i]) == D3D_OK ) {
				result = FormatList[i];
				break;
			}
		}
	}
	return result;
}

void TDGTexture::Keep()
{
	KeepStream.str("");

	SaveToStream(KeepStream);
	KeepStream.seekg(0);

}

void TDGTexture::CreateTexture( u32 w, u32 h )
{
	if(FTexture != NULL ) {
		//PutTestMessage("テクスチャの再生成に伴い、以前のテクスチャを解放します");
		//TRYDG(FTexture.Release, "テクスチャの解放に失敗しました");
		FTexture->Release();
	}

	//フォーマットリストから一番良いフォーマットを得る
	FFormat = SearchBestFormat();
	//PutTestMessage(SysUtils.Format("使用されるフォーマットは %s です",[D3DFORMATToString(Format)]));

	//TRYDG(FOwner.FD3DDevice->CreateTexture(w,h, FMipLevel, Usage, Format, Pool, FTexture, NULL), "テクスチャの生成に失敗しました");
	FOwner->Get3DDevice()->CreateTexture(w,h, FMipLevel, FUsage, FFormat, FPool, &FTexture, NULL);

	//PutTestMessage(SysUtils.Format("残りテクスチャ容量は、約%fMBです", [FOwner.FD3DDevice->GetAvailableTextureMem / (1024*1024)]));

	FPixelSize = GetPixelBits(FFormat) / 8;

}

void TDGTexture::Cleanup()
{
	if(FTexture != NULL ) {
		//現在の状態を保存(kmFromStreamの時)
		//kmFromFile,kmFromQDAの時は、ファイルから読み込む
		if((KeepMethod == kmFromStream) && FAutoKeep )
			Keep();

		FTexture->Release();
		FTexture = NULL;
	}

}

void TDGTexture::Recover()
{
	Recovering = true;

	if( FAutoKeep ) {
		switch( KeepMethod ){
		case kmDontKeep:     //中身は復元しなくていい
			SetSize(FWidth, FHeight);
			break;
		case kmFromFile: { //ファイルから読み直し(テクスチャの生成は、読み込みメソッド内で行われる)
				LoadFromFile(KeepFilename);
			}
			break;
		case kmFromQDA: {  //QDAから
			LoadFromFile(KeepQDAID, KeepQDAName);
			}
			break;
		case kmFromStream: {
			 LoadFromStream(KeepStream); //復元して、それまで使ってたKeepStreamは消す
			 KeepStream.str("");
			}
			break;
		}
	} else {
		//オブジェクトだけは復元
		SetSize(FWidth, FHeight);
	}

	Recovering = false;

}

void TDGTexture::Reset()
{
	if(FPool == D3DPOOL_DEFAULT )
		Cleanup();

}

void TDGTexture::AfterReset()
{
	if(FPool == D3DPOOL_DEFAULT )
		SetSize(FWidth, FHeight);

}

TDGTexture::TDGTexture( TDGCarad* AOwner, const D3DFORMAT formats[], u32 _usage /*= 0*/, D3DPOOL _pool /*= D3DPOOL_MANAGED*/, u32 mipLevel /*= 0*/, string debugName /*= ""*/ )
{
	FOwner = AOwner;
	FTexture = NULL;
	FUsage = _usage;
	FFormat = D3DFMT_UNKNOWN;
	KeepStream.str("");
	KeepMethod = kmDontKeep;
	Recovering = false;
	FPool = _pool;
	FMipLevel = mipLevel;
	FBorderColor = 0x00000000;

	//POOL_Defaultは自動復元しない
	FAutoKeep  =  FPool != D3DPOOL_DEFAULT;

	//フォーマットリストをコピーする
	for( s32 i=0; i<sizeof(formats)/sizeof(D3DFORMAT); i++ )
		FormatList.push_back(formats[i]);

	//DGCaradに登録
	ID = FOwner->RegisterResouce(this, debugName);
	FDebugName = debugName;
	//ID = FOwner.RegisterTexture(Self);


}

TDGTexture::~TDGTexture()
{
	//PutTestMessage("テクスチャオブジェクトを解放します");
	if(FTexture != NULL ) {
		//TRYDG(FTexture.Release, "テクスチャの解放に失敗しました");
		FTexture->Release();
	}

	KeepStream.str("");

	//登録解除
	FOwner->UnregisterResource(this->ID);

	FormatList.clear();

}

void TDGTexture::LoadFromFile( string filename, string qdaName /*= ""*/ )
{
	BITMAPINFOHEADER bi;

	if(qdaName.empty() ) {
		//アーカイブ名が略されたなら、ファイルから読み込む

		//PutTestMessage(Sysutils.Format("ID %d(%s) のテクスチャに、%s を読み込みます",[ID, FDebugName, filename]));

		try{
			ifstream ss;
			ss.open(filename);

			//ヘッダを読む
			ss.seekg(sizeof(BITMAPFILEHEADER), std::ios::beg);
			ss.read(reinterpret_cast<char*>(&bi),sizeof(BITMAPINFOHEADER));
			SetSize(bi.biWidth, bi.biHeight);

			//巻き戻し
			ss.seekg(0, std::ios::beg);
			LoadBMPStreamToTexture(FTexture, ss);
			ss.close();
		}
		catch (std::ifstream::failure e) {
		//	TRYDG(1, filename + "からの、読み込みエラー');
		}

		//自動復元情報を更新
		KeepMethod = kmFromFile;
		KeepFilename = filename;
	} else {
		//アーカイブから読み込む

		//PutTestMessage(Sysutils.Format("ID %d(%s) のテクスチャに、QDAファイル%s から ID %s を読み込みます",[ID, FDebugName, qdaName, filename]));
		try{
			stringstream ms;
			ExtractFromQDAFile(ms, qdaName, filename);

			//ヘッダを読む
			ms.seekg(sizeof(BITMAPFILEHEADER), std::ios::beg);
			ms.read(reinterpret_cast<char*>(&bi), sizeof(BITMAPINFOHEADER));
			SetSize(bi.biWidth, bi.biHeight);

			//巻き戻し
			ms.seekg(0, std::ios::beg);
			LoadBMPStreamToTexture(FTexture, ms);
		}
		catch (std::ifstream::failure e) {
		//	TRYDG(1, filename + " ＠ " + qdaName + "からの、読み込みエラー');
		}

		//自動復元情報を更新
		KeepMethod = kmFromQDA;
		KeepQDAName = qdaName;
		KeepQDAID = filename;
	}
}

void TDGTexture::LoadFromStream( istream& source )
{
	BITMAPINFOHEADER bi;
	ifstream::pos_type pos;//読み出し前の位置
	//PutTestMessage(Sysutils.Format("ID %d のテクスチャに、ストリームから読み込みます",[ID]));

	try{
		//ヘッダを読む
		pos = source.tellg();
		source.seekg(sizeof(BITMAPFILEHEADER), std::ios::beg);
		source.read(reinterpret_cast<char*>(&bi), sizeof(BITMAPINFOHEADER));
		SetSize(bi.biWidth, bi.biHeight);

		//巻き戻して読み込み
		source.seekg(pos);
		LoadBMPStreamToTexture(FTexture, source);

		//自動復元情報を更新…必要になったら内蔵ストリームに勝手に保存＆復元
		KeepMethod = kmFromStream;
	}
	catch (std::ifstream::failure e) {
		//TRYDG(1, "ストリームからの、読み込みエラー');
	}
}

void TDGTexture::SaveToStream( ostream& dest )
{
	BITMAPFILEHEADER bf;
	BITMAPINFOHEADER bi;
	u8* pLine;
	u8* pPix;
	u32 col;
	vector<u32> lineBuf;

	//ヘッダ作成
	bf.bfType = 0x4d42;
	bf.bfSize = sizeof(bf) + sizeof(bi) + (FWidth * FHeight) * 4;
	bf.bfReserved1 = 0;
	bf.bfReserved2 = 0;
	bf.bfOffBits = sizeof(bf) + sizeof(bi);

	bi.biSize = sizeof(bi);
	bi.biWidth = FWidth;
	bi.biHeight = FHeight;
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = FWidth * FHeight * 4;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	//ヘッダ書き込み
	dest.write(reinterpret_cast<char*>(&bf), sizeof(bf));
	dest.write(reinterpret_cast<char*>(&bi), sizeof(bi));

	lineBuf.resize(FWidth);

	//変換。上下逆に書き込み
	LockRect(NULL, 0, Rect(0,0,FWidth,FHeight), D3DLOCK_READONLY);
	for( s32  y = FHeight-1; y >= 0; y-- ) {
		pLine = (u8*)FLockedRect.pBits;
		pLine += y*FLockedRect.Pitch;
		pPix = pLine;

		//一ライン変換して
		for( u32  x = 0; x < FWidth; x++ ) {
			col = 0;
			memmove(&col, pPix, FPixelSize);
			lineBuf[x] = PhysicalToARGB(col, FFormat);
			pPix += FPixelSize;
		}
		//書き込み
		dest.write(reinterpret_cast<char*>(&lineBuf[0]), FWidth*4);

	}
	UnlockRect();

	lineBuf.clear();
}

void TDGTexture::PerPixelOperation( Rect& region, TDGPerPixelOp op, void* data )
{
	u8* pPix;
	u32 src,dest;
	LockRect(NULL, 0, region, 0);

	for( s32  y = region.Top; y < region.Bottom; y++ ) {
		pPix = (u8*)FLockedRect.pBits;
		pPix += (y - region.Top)*FLockedRect.Pitch;
		for( s32  x = region.Left; x < region.Right; x++ ) {
			src = 0;

			memmove(&src, pPix, FPixelSize);
			dest = op(this, src, x,y, data);
			memmove(pPix, &dest, FPixelSize);

			pPix += FPixelSize;
		}
	}

	UnlockRect();

}

//カラーキーの設定、変換ルーチン
u32 SetColorKeyOP( TDGTexture* Sender, u32 src, s32 x, s32 y, void* data)
{
	D3DCOLOR col = PhysicalToARGB(src, Sender->GetFormat()) & 0x00FFFFFF;

	if( col != *(u32*)data ) 
		col |= 0xFF000000;

	return D3DColorToPhysical(col, Sender->GetFormat());
}


void TDGTexture::SetColorKey( Rect& region, D3DCOLOR col )
{
	u32 ck = D3DColorToPhysical(col, FFormat) & 0x00FFFFFF;  //α値を一旦ゼロにしてOPに渡す
	PerPixelOperation(region, SetColorKeyOP, &ck);

}

//青の輝度をα値へ(変換ルーチン本体)
u32 BlueToAlphaOP( TDGTexture* Sender, u32 src, s32 x, s32 y, void* data)
{
	D3DCOLOR col = PhysicalToARGB(src, Sender->GetFormat());
	col = (col & 0x00FFFFFF) | (col & 0xFF << 24);  //B を A にコピー
	return D3DColorToPhysical(col, Sender->GetFormat());
}

void TDGTexture::BlueToAlpha( Rect& region )
{
	PerPixelOperation(region, BlueToAlphaOP, NULL);
}

void TDGTexture::Fill( Rect& region, D3DCOLOR col, bool maskA )
{
	LockRect(NULL, 0, region, 0);

	if(! maskA ) {
		u32 phys = D3DColorToPhysical(col, FFormat);

		//最初の一ラインは転送しまくりでなんとかする
		u8* pPix = (u8*)FLockedRect.pBits;
		for( s32  x = region.Left; x < region.Right; x++ ) {
			memmove(pPix, &phys, FPixelSize);
			pPix += FPixelSize;
		}

		//あとはコピー
		pPix = (u8*)FLockedRect.pBits;
		for( s32  y = region.Top+1; y < region.Bottom; y++ ) {
			pPix += FLockedRect.Pitch;
			memmove(pPix, FLockedRect.pBits, FWidth * FPixelSize);
		}

	} else {

		//マスク付き…単に転送というわけにはいかんのよ
		for( s32  y = region.Top; y < region.Bottom; y++ ) {
			u8* pPix = (u8*)FLockedRect.pBits;
			pPix += (y-region.Top)  * FLockedRect.Pitch;
			for( s32  x = region.Left; x < region.Right; x++ ) {
				//ゲッチュ
				u32 phys = 0;
				memmove(&phys, pPix, FPixelSize);

				D3DCOLOR srcCol = PhysicalToARGB(phys, FFormat);
				srcCol = (srcCol & 0xFF000000) | (col & 0x00FFFFFF); 

				//変換してかきこ
				phys = D3DColorToPhysical(srcCol, FFormat);
				memmove(pPix, &phys, FPixelSize);

				pPix += FPixelSize;
			}
		}
	}

	UnlockRect();

}

//2の整数べき、かつ、x以上の中で最も小さい数を得る
u32 AlignP2( u32 x)
{
	u32 result = 1;
	while( result < x ){
		result  =  result << 1;
	}
	return result;
}

void TDGTexture::SetSize( u32 w, u32 h )
{
	D3DCAPS9 caps;

	//Widthとして、最初に指定されたw,hを記録
	FWidth = w;
	FHeight = h;

	FOwner->GetD3D()->GetDeviceCaps(FOwner->GetAdapter(), FOwner->GetDeviceType(), &caps);

	//if((caps.MaxTextureWidth < w) || (caps.MaxTextureHeight < h) )
	//	PutTestMessage(SysUtils.Format("要求されたテクスチャのサイズは大きすぎます。%d x %d 以内にして下さい",[caps.MaxTextureWidth, caps.MaxTextureHeight]));

	//SQUAREONLYの場合、一辺のサイズをデカい方に合わせる
	if((caps.TextureCaps && D3DPTEXTURECAPS_SQUAREONLY) != 0 ) {
		if(w > h )
			h = w;
		else
			w = h;
	}

	//POW2なら、2の整数べきにそろえる
	if((caps.TextureCaps && D3DPTEXTURECAPS_POW2) != 0 ) {
		w = AlignP2(w);
		h = AlignP2(h);
	}

	//TotalWidth,TotalHeightとして、加工後のw,hを記録
	FTotalWidth = w;
	FTotalHeight = h;

	//右下隅のuv座標を算出
	FU = (f32)FWidth / (f32)FTotalWidth;
	FV = (f32)FHeight / (f32)FTotalHeight;

	//テクスチャ生成
	CreateTexture(w, h);

	//ボーダー色で埋める
	if(FPool != D3DPOOL_DEFAULT )
		Fill(Rect(0,0,FTotalWidth,FTotalHeight), FBorderColor, false);

	//キープ情報の削除(リカバー中を除く)
	if(! Recovering ) {
		KeepStream.str("");
		KeepMethod = kmFromStream;
	}

}

bool TDGTexture::LockRect( D3DLOCKED_RECT* _out, u32 level, Rect& rect, u32 flags )
{
	FLocked = true;

	//ロックして書き換えたテクスチャは、ファイルからの自動復元の対象にしない
	if((flags && D3DLOCK_READONLY) == 0 ){
		if(KeepMethod != kmDontKeep ){
			KeepMethod = kmFromStream;
		}
	}

	if(FTexture == NULL ) {
		//TRYDG(1, "テクスチャが確保されていません。LockRectの前にSetSizeするか、BMPを読み込んでください");
		return false;
	}

	FLockedRegion = rect;
	//TRYDG(FTexture.LockRect(level, FLockedRect, &rect, flags), "LockRectに失敗しました");
	RECT rc;
	rc.bottom = rect.Bottom;
	rc.top	= rect.Top;
	rc.left = rect.Left;
	rc.right= rect.Right;
	FTexture->LockRect(level, &FLockedRect, &rc, flags);
	if( _out )
		*_out = FLockedRect;
	return true;
}

void TDGTexture::UnlockRect()
{
	//TRYDG(FTexture.UnlockRect(FLockedLevel), "アンロックに失敗しました");
	FTexture->UnlockRect(FLockedLevel);
	FLocked = false;

}

void TDGVertexBuffer::SetVertexCount( u32 Value )
{
	if(FVBuf != NULL )
		FVBuf->Release();
		//TRYDG(FVBuf.Release, "VertexBufferの解放に失敗しました");

	if((KeepBuf != NULL) && (Value != FVertexCount) ) {
		//Value = FVertexCountの場合は、特にKeepBufを消さない(Recover中のことがある)
		SAFE_FREE(KeepBuf);
	}

	FVertexCount  =  Value;
	//TRYDG( FOwner.FD3DDevice->CreateVertexBuffer(VertexSize*Count, Usage, FVF, Pool, FVBuf, NULL), "VertexBufferの生成に失敗しました");
	FOwner->Get3DDevice()->CreateVertexBuffer(FVertexSize * FVertexCount, FUsage, FFVF, FPool, &FVBuf, NULL);

}

void TDGVertexBuffer::Cleanup()
{
	if( FAutoKeep )
		Keep();

	if( FVBuf != NULL ) {
		//TRYDG(FVBuf.Release, "VertexBufferの解放に失敗しました");
		FVBuf->Release();
		FVBuf = NULL;
	}
}

void TDGVertexBuffer::Recover()
{

	SetVertexCount(FVertexCount);  

	//書き戻し
	//D3DPOOL_DEFAULTだったら、内容は毎フレームフラッシュされても良いことが前提
	//なので、書き戻さない
	if((KeepBuf != NULL) && (FPool != D3DPOOL_DEFAULT) ) {
		Lock(0, 0, 0);
		memmove(PLocked, KeepBuf, FVertexSize * FVertexCount);
		Unlock();
	}
}

void TDGVertexBuffer::Reset()
{
	if(FPool == D3DPOOL_DEFAULT )
		Cleanup();
}

void TDGVertexBuffer::AfterReset()
{
	if(FPool == D3DPOOL_DEFAULT )
		Recover();    
}

void TDGVertexBuffer::Keep()
{
	//D3DPOOL_DEFAULTだったら、内容は毎フレームフラッシュされても良いことが前提
	//なので、キープしない
	if(FPool == D3DPOOL_DEFAULT )
		return;

	if(KeepBuf != NULL )
		SAFE_FREE(KeepBuf);

	KeepBuf = malloc(FVertexSize * FVertexCount);
	Lock(0,0, D3DLOCK_READONLY);
	memmove(KeepBuf, PLocked,  FVertexSize * FVertexCount);
	Unlock();

}

TDGVertexBuffer::TDGVertexBuffer( TDGCarad* AOwner, u32 _vertexCount, u32 _vertexSize, u32 _Usage, u32 _FVF, D3DPOOL _Pool, string debugName /*= ""*/ )
{
	FOwner = AOwner;
	FLocked = false;
	FVBuf = NULL;
	KeepBuf = NULL;
	FAutoKeep = true;
	FDebugName = debugName;

	FVertexSize = _vertexSize;
	FUsage = _Usage;
	FFVF = _FVF;
	FPool = _Pool;

	SetVertexCount(_vertexCount);  

	//登録
	ID = FOwner->RegisterResouce(this, debugName);

}

TDGVertexBuffer::~TDGVertexBuffer()
{
	if(KeepBuf != NULL )
		SAFE_FREE(KeepBuf);

	//TRYDG(FVBuf.Release, "VertexBufferの解放に失敗しました");
	FVBuf->Release();

	FOwner->UnregisterResource(ID);

}

void* TDGVertexBuffer::Lock( u32 start, u32 count, u32 Flags )
{
	//TRYDG( FVBuf.Lock(start*VertexSize, count*VertexSize, PLocked, Flags), "VertexBufferのロックに失敗しました");
	FVBuf->Lock(start*FVertexSize, count*FVertexSize, &PLocked, Flags);

	FLocked = true;

	if(count == 0 ) {
		FLockEnd = FVertexCount-1;
	} else {
		FLockStart = start;
		FLockEnd = start + count-1;
	}

	return PLocked;

}

void TDGVertexBuffer::Unlock()
{
	//TRYDG(FVBuf.Unlock, "VertexBufferのアンロックに失敗しました");
	FVBuf->Unlock();
	FLocked = false;
}

void TDGVertexBuffer::GetVertex( u32 idx, void* v )
{
	u8* p;
	u32 ofs;
	if(! FLocked ) {
		//未ロックならロックしてから
		if(idx >= FVertexCount )
			return;
			//TRYDG(1, "VertexBufferの範囲外を読み込もうとしました");

		p = (u8*)Lock(idx, 1, D3DLOCK_READONLY);
		memmove(v, p, FVertexSize);
		Unlock();
	} else {
		//範囲チェックして読み込み
		if((idx < FLockStart) || (idx > FLockEnd) ) {
			//TRYDG(1, "VertexBufferのロックされた範囲の外を読もうとしました");
			return;
		}

		ofs = idx * FVertexSize;
		p = (u8*)PLocked;
		p += ofs;
		
		memmove(v, p, FVertexSize);
	}
}

void TDGVertexBuffer::SetVertex( u32 idx, void* v )
{
	u8* p;
	//未ロックならロックする
	if(! FLocked ) {
		if(idx >= FVertexCount ){
			return;
			//TRYDG(1, "VertexBufferの範囲外に書き込もうとしました");
		}
		p = (u8*)Lock(idx, 1, 0);
		memmove(p, v, FVertexSize);
		Unlock();
	} else {
		//範囲チェックして書き込み
		if((idx < FLockStart) || (idx > FLockEnd) ){
			//TRYDG(1, "VertexBufferのロックされた範囲の外へ書き込もうとしました");
			return;
		}

		u32 ofs = (idx-FLockStart) * FVertexSize;
		p = (u8*)PLocked;
		p += ofs;

		memmove(p, v, FVertexSize);
	}

}

u16 TDGIndexBuffer::GetIndices( u32 idx )
{
	u16* p;
	u16 result = 0;
	if(! FLocked ) {
		//未ロックならロックしてから

		if(idx >= FCount ){
			return 0;
			//TRYDG(1, "IndexBufferの範囲外を読み込もうとしました");
		}
		//Lock(idx*sizeof(Word), sizeof(Word), D3DLOCK_READONLY);
		result = *Lock(idx,1,D3DLOCK_READONLY);
		Unlock();
	} else {
		//範囲チェックして読み込み
		if((idx < FLockStart) || (idx > FLockEnd) ) {
			//TRYDG(1, "IndexBufferのロックされた範囲の外を読もうとしました");
			return 0;
		}

		p = (u16*)PLocked;
		p += idx;
		result = *p;
	}

	return result;

}

void TDGIndexBuffer::SetCount( u32 Value )
{
	if(FIBuf != NULL ){
		return;
		//TRYDG(FIBuf.Release, "IndexBufferの解放に失敗しました");
	}

	if((KeepBuf != NULL) && (Value != FCount) ) {
		//Value = FIndexCountの場合は、特にKeepBufを消さない(Recover中のことがある)
		SAFE_FREE(KeepBuf);
	}

	FCount  =  Value;
	FOwner->Get3DDevice()->CreateIndexBuffer(FCount*sizeof(u16), FUsage, D3DFMT_INDEX16, FPool, &FIBuf, NULL);
	//TRYDG( FOwner.FD3DDevice->CreateIndexBuffer(FCount*sizeof(Word), Usage, D3DFMT_INDEX16, Pool, FIBuf, NULL), "IndexBufferの生成に失敗しました");

}

void TDGIndexBuffer::SetIndices( u32 idx, u16 Value )
{
	u16* p;
	if(! FLocked ) {
		//未ロックならロックしてから

		if(idx >= FCount ){
			return;
			//TRYDG(1, "IndexBufferの範囲外へ書き込もうとしました");
		}
		Lock(idx,1,0);
		*((u16*)PLocked) = Value;
		Unlock();
	} else {
		//範囲チェックして読み込み
		if((idx < FLockStart) || (idx > FLockEnd) ) {
			//TRYDG(1, "IndexBufferのロックされた範囲の外へ書こうとしました");
			return;
		}

		p = (u16*)PLocked;
		p += idx;
		*p = Value;
	}

}

void TDGIndexBuffer::Cleanup()
{
	if( FAutoKeep )
		Keep();

	if(FIBuf != NULL ) {
		//TRYDG(FIBuf.Release, "IndexBufferの解放に失敗しました");
		FIBuf = NULL;
	}

}

void TDGIndexBuffer::Recover()
{
	SetCount( FCount );  //プロパティハンドラの呼び出しで、IBuf再生成

	//書き戻し
	//D3DPOOL_DEFAULTだったら、内容は毎フレームフラッシュされても良いことが前提
	//なので、書き戻さない
	if((KeepBuf != NULL) && (FPool != D3DPOOL_DEFAULT) ) {
		Lock(0,0, 0);
		memmove(PLocked, KeepBuf, FCount*sizeof(u16));
		Unlock();
	}

}

void TDGIndexBuffer::Reset()
{
	if(FPool == D3DPOOL_DEFAULT ) {
		Cleanup();
	}

}

void TDGIndexBuffer::AfterReset()
{
	if(FPool == D3DPOOL_DEFAULT ) {
		Recover();
	}

}

void TDGIndexBuffer::Keep()
{
	//D3DPOOL_DEFAULTだったら、内容は毎フレームフラッシュされても良いことが前提
	//なので、キープしない
	if(FPool == D3DPOOL_DEFAULT )
		return;

	if(KeepBuf != NULL )
		SAFE_FREE(KeepBuf);

	KeepBuf = malloc(FCount*sizeof(u16));
	Lock(0,0, D3DLOCK_READONLY);
	memmove(KeepBuf, PLocked, FCount*sizeof(u16));
	Unlock();

}

TDGIndexBuffer::TDGIndexBuffer( TDGCarad* AOwner, u32 _indexCount, u32 _Usage, D3DPOOL _Pool, string debugName /*= ""*/ )
{
	FOwner = AOwner;
	FLocked = false;
	FIBuf = NULL;
	KeepBuf = NULL;
	FAutoKeep = true;
	FDebugName = debugName;

	FUsage = _Usage;
	FPool = _Pool;

	SetCount( _indexCount );
	//登録
	ID = FOwner->RegisterResouce(this, debugName);

}

TDGIndexBuffer::~TDGIndexBuffer()
{
	if(KeepBuf != NULL )
		SAFE_FREE(KeepBuf);

	FIBuf->Release();
	//TRYDG(FIBuf.Release, "IndexBufferの解放に失敗しました");

	FOwner->UnregisterResource(ID);

}

u16* TDGIndexBuffer::Lock( u32 start, u32 count, u32 Flags )
{
	FIBuf->Lock(start*sizeof(u16), count*sizeof(u16), &PLocked, Flags);
	//TRYDG( FIBuf.Lock(start*sizeof(Word), count*sizeof(Word), PLocked, Flags), "IndexBufferのロックに失敗しました");

	if(count == 0 ) {
		FLockEnd = FCount-1;
	} else {
		FLocked = true;
		FLockStart = start;
		FLockEnd = start + count-1;
	}

	return (u16*)PLocked;

}

void TDGIndexBuffer::Unlock()
{
	FIBuf->Unlock();
	//TRYDG(FIBuf.Unlock, "IndexBufferのアンロックに失敗しました");
	FLocked = false;

}
