#include "DGCarad9.h"
#include "QDArc.h"
#include <string.h>

//���j�b�g���[�J���ȃO���[�o���ϐ�

//D3DCOLOR(ARGB)���e�N�X�`����̐F�\���ɂ���
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
						  //�ȉ��A�o���v�}�b�v�p
						  //ARGB�́AR��L(W)�BG��U�AB��V�Ƃ��ēǂ� A��A���̂܂܂Ƃ��AQ�Ƃ�
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

//8�r�b�gARGB�l���e�N�X�`����̐F�\���ɂ���
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

						  //�ȉ��A�o���v�}�b�v�p

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

//�r�b�g���𑝂₵��8�r�b�g��(nBit�͂��Ƃ̃r�b�g��)
u32 GainBit( u32 c, u32 nBit )
{
	return c * 255 / (1 << (nBit - 1));
}


//�����J���[��D3DCOLOR�ցB
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
					//�ȉ��A�o���v�}�b�v�p
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
		//�����t�Ȃ̂ŁAGainBit�͎g���Ȃ�
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


//���̃t�H�[�}�b�g�͐F�\���p�̃t�H�[�}�b�g���H
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

//�P�s�N�Z��������̃r�b�g���𓾂�
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

//���b�N�����͈͂�BMP��ǂݍ���
void LoadBMPStreamToLockedRect(D3DLOCKED_RECT& LockedRect, Rect& lockedRegion, D3DSURFACE_DESC ddsd, istream& source)
{
	BITMAPINFOHEADER bih;
	BITMAPFILEHEADER bfh;
	u32 bypp;    //�T�[�t�F�X��Bytes/Pix
	u32 fbypp;   //�t�@�C����Bytes/Pix
	TRGBQUAD pal[256];

	u32 bpl,fbpl;
	u32 trBytes;          //�]������o�C�g��/�s
	s32 trHeight,trWidth; //�]������T�C�Y
	u8* pix;
	u8* buf;
	u8* dest;


	//�t�@�C����8�r�b�g�̏ꍇ�̕ϊ����[�`��
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

	//�t�@�C����16�r�b�g�̏ꍇ�̕ϊ����[�`��
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
			/*ARGB1555�Ƃ��ēǂݍ���*/				\
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

	//�t�@�C����24�r�b�g�̏ꍇ�̕ϊ����[�`��
#define Process24bit( pix, buf )							\
	{														\
		u8* pSrc;											\
		u8* pDest;											\
		u32 logi;   /*�t�@�C������ǂ񂾘_���J���[*/		\
		u32 phys;  /*�T�[�t�F�X�ɏ������ޕ����J���[*/		\
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

	//�t�@�C����32�r�b�g�̏ꍇ�̕ϊ����[�`��
#define Process32bit( pix, buf )								\
	{															\
		u32*	pSrc;											\
		u8*		pDest;											\
		u32		logi;   /*�t�@�C������ǂ񂾘_���J���[*/		\
		u32		phys;  /*�T�[�t�F�X�ɏ������ޕ����J���[*/		\
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

	//buf�̒��g���Apix�ɕϊ�����
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
		//BMP�t�@�C���w�b�_��ǂ�(ID�̃`�F�b�N)
		source.read(reinterpret_cast<char*>(&bfh), sizeof(bfh));
	}
	catch(std::istream::failure e){
		//PutDebugMessage("�r�b�g�}�b�v�̓ǂ݂��ݒ��ɁA�G���[���������܂���");
		return;
	}

	if( bfh.bfType != 0x4d42 ) {
		//PutDebugMessage("�s����BMP�t�@�C���ł�");
		return;
	}

	//BMP�C���t�H�w�b�_�̓ǂݍ���
	try{
		source.read(reinterpret_cast<char*>(&bih), sizeof(bih));
	}
	catch(std::istream::failure e){
		//PutDebugMessage("LoadBMP���ɁA�ǂݍ��݃G���[���������܂���");
		return;
	}

	//8�r�b�g�`32�r�b�g�ȊO��BMP�̓_��
	if((bih.biBitCount < 8) || (bih.biBitCount > 32) ) {
		//TRYDG(1, "�F����8�r�b�g�`32�r�b�g�͈̔͂̃r�b�g�}�b�v���ALoadBMP�ɂ͎w�肵�Ă�������");
		return;
	}


	//�T�[�t�F�X�� bypp �𓾂�
	bypp = GetPixelBits(ddsd.Format) / 8;

	//�t�@�C���� bypp�� ����
	fbypp  =  bih.biBitCount / 8;


	if(ddsd.Format == D3DFMT_P8 ) {

		//{ �T�[�t�F�X��8�r�b�g�p���b�g���̏ꍇ }

		if((fbypp>1) ) {
			//{ �t�@�C�����n�C�J���[�ȏ�Ȃ�A�]���ł��܂��� }
			//TRYDG(1, "8�r�b�g�p���b�g�t���e�N�X�`���ɑ΂���16�r�b�g�ȏ��BMP�����[�h���鎖�͏o���܂���");
			return;

		} else {
			//{ �t�@�C���� 1bypp �Ȃ�A���ڗ������� }

			try{
				source.read(reinterpret_cast<char*>(&pal[0]),1024);
			}
			catch(std::istream::failure e){
				//PutDebugMessage("LoadBMP���ɁA�ǂݍ��݃G���[���������܂���");
				return;
			}

			source.seekg(bfh.bfOffBits, std::ios::beg);

			//�C���[�W�o�b�t�@�̍쐬
			bpl  =  ((u32)(bih.biWidth)*bypp+3) & 0xFFFFFFFC;
			fbpl  =  ((u32)(bih.biWidth)*fbypp+3) & 0xFFFFFFFC;

			pix = (u8*)malloc(bpl);
			buf = (u8*)malloc(fbpl);


			//DIB���㉺�������܂ɃR�s�[
			trBytes  =  LockedRect.Pitch;

			//�]������o�C�g���́A�e�N�X�`���̂P���C�����̏��ʂƁA�r�b�g�}�b�v�̂��ꂤ���A���Ȃ��ق�
			if(trBytes > bpl)
				trBytes  =  bpl;

			//�ǂݍ��ރ��C�����́A���b�N�͈͂̍s���ƁA�r�b�g�}�b�v�̍s���̂����A���Ȃ��ق�
			trHeight = lockedRegion.Bottom - lockedRegion.Top;
			if(trHeight > bih.biHeight )
				trHeight  =  bih.biHeight;

			trWidth = lockedRegion.Right - lockedRegion.Left;
			if(trWidth > bih.biWidth )
				trWidth = bih.biWidth;

			dest = (u8*)LockedRect.pBits;
			dest += LockedRect.Pitch * (trHeight-1);

			//�ǂݍ��݌��r�b�g�}�b�v�ɑ΂��āA�]���͈͂��������ꍇ�́A�ǂݍ��݌������炷(DIB���㉺�t������)
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
				//PutDebugMessage("LoadBMP���ɁA�ǂݍ��݃G���[���������܂���");
				return;
			}
			free(pix);
			free(buf);


		}
	} else {
		//{�T�[�t�F�X���p���b�g�t���ł͂Ȃ��ꍇ}

		//�p���b�g�̓ǂݍ���(�ǂݍ��݌����p���b�g�t��BMP�̎�)
		if(fbypp == 1 ) {
			try{
				source.read(reinterpret_cast<char*>(&pal[0]), 1024);
			}
			catch(std::istream::failure e){
				//PutDebugMessage("LoadBMP���ɁA�ǂݍ��݃G���[���������܂���");
				return;
			}
		}

		source.seekg( bfh.bfOffBits, std::ios::beg );

		//�C���[�W�o�b�t�@�̍쐬
		bpl  =  ((u32)(bih.biWidth)*bypp+3) & 0xFFFFFFFC;
		fbpl  =  ((u32)(bih.biWidth)*fbypp+3) & 0xFFFFFFFC;

		pix = (u8*)malloc(bpl);
		buf = (u8*)malloc(fbpl);

		//DIB���㉺�������܂ɃR�s�[
		trBytes  =  LockedRect.Pitch;
		if((trBytes > bpl) )
			trBytes  =  bpl;

		//�ǂݍ��ރ��C�����́A���b�N�͈͂̍s���ƁA�r�b�g�}�b�v�̍s���̂����A���Ȃ��ق�
		trHeight = lockedRegion.Bottom - lockedRegion.Top;
		if(trHeight > bih.biHeight )
			trHeight  =  bih.biHeight;

		trWidth = lockedRegion.Right - lockedRegion.Left;
		if(trWidth > bih.biWidth )
			trWidth = bih.biWidth;

		dest = (u8*)LockedRect.pBits;
		dest += LockedRect.Pitch * (trHeight-1);

		//�ǂݍ��݌��r�b�g�}�b�v�ɑ΂��āA�]���͈͂��������ꍇ�́A�ǂݍ��݌������炷(DIB���㉺�t������)
		if(bih.biHeight > trHeight )
			source.seekg( fbpl * (bih.biHeight - trHeight), std::ios::cur );     

		try{
			for( s32 i = 0; i < trHeight; i++ ) {
				source.read(reinterpret_cast<char*>(buf), fbpl);

				ProcessLine(pix, buf);    //�ϊ�
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
			//PutDebugMessage("LoadBMP���ɁA�ǂݍ��݃G���[���������܂���");
			return;
		}

		free(pix);
		free(buf);

	}

}

//�e�N�X�`���ɃX�g���[������BMP��ǂݍ���
void LoadBMPStreamToTexture(IDirect3DTexture9* texture, istream& source )
{
	D3DSURFACE_DESC ddsd;
	D3DLOCKED_RECT lockRect;

	//�Ǎ���e�N�X�`���̃T�C�Y���Q�b�`��
	texture->GetLevelDesc(0, &ddsd);

	//�T�[�t�F�X�̃��b�N
	texture->LockRect(0, &lockRect, NULL, 0);


	//�ǂݍ���
	LoadBMPStreamToLockedRect(lockRect, Rect(0, 0, ddsd.Width, ddsd.Height), ddsd, source);

	//�A�����b�N
	texture->UnlockRect(0);
}

void TDGCarad::SetDebugOption( TDGDebugOption Value )
{
	FDebugOption  =  Value;

	DGCarad_HaltOnError  = (Value & dgoHaltOnError) != 0;
	DGCarad_DetailedLog  = (Value & dgoDetailedLog) != 0;
	DGCarad_DebugMessage  = (Value & dgoDebugMessage) != 0;

}


//��Ԃ���Depth/Stencil�o�b�t�@�t�H�[�}�b�g�𓾂�
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
//	PutTestMessage("��]�̃f�v�X�X�e���V���T�[�t�F�X�̃t�H�[�}�b�g��������܂���ł���");

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
	//PutTestMessage("��������ʃ��[�h�����������܂�������");
	//DGCarad_debugIndent++;

	//�v���[���e�[�V�����p�����[�^�ݒ�
	ZeroMemory(&prep, sizeof(prep));
	prep.Windowed = windowed;
	prep.SwapEffect = swapEffect;
	prep.BackBufferWidth = mode.Width;
	prep.BackBufferHeight = mode.Height;
	prep.BackBufferFormat = mode.Format;
	prep.MultiSampleType = multiSampleType;
	prep.PresentationInterval = presentationInterval;


	//�h���C�o���ʏ��̕\��
	FD3D->GetAdapterIdentifier(Adapter, 0, &ai);
	drvName = string(ai.Driver);
	drvDesc = string(ai.Description);

	//PutTestMessage("Driver : %s", drvName);
	//PutTestMessage(Format("Description : %s", [drvDesc]));
	//PutTestMessage(Format("VendorID : %.8x, DeviceID : %.8x, SubSysID : %.8x, Revision : %.8x", [ai.VendorID,ai.DeviceID,ai.SubsysID,ai.Revision]));

	if(windowed ) {
		//PutTestMessage(Format("�E�B���h�E���[�h %dx%d�ŏ��������܂�",[mode.Width, mode.Height]));
	} else {
		//PutTestMessage(Format("�t���X�N���[�����[�h %dx%d %dHz %s �ŏ��������܂�",[mode.Width,mode.Height,mode.RefreshRate,D3DFormatToString(mode.Format)]));    prep.BackBufferWidth = mode.Width;
		prep.FullScreen_RefreshRateInHz = mode.RefreshRate;
	}

	prep.hDeviceWindow = m_owner.m_handle;
	prep.EnableAutoDepthStencil  =  (depthStencilFmt.size() != 0);

	if((FD3D->GetDeviceCaps(Adapter, D3DDEVTYPE_HAL, &caps) == D3D_OK) && (! FUseREFDevice) ) {//= ) {// D3D_OK ) {
		//HAL�͂�����炵����
		//PutTestMessage("�n�[�h�E�F�A�A�N�Z�����[�V�����𗘗p���܂�");

		prep.AutoDepthStencilFormat = BestZSFormat(D3DDEVTYPE_HAL, depthStencilFmt, prep, Adapter );
		//PutTestMessage("�f�v�X�X�e���V���T�[�t�F�X�̃t�H�[�}�b�g..." + D3DFORMATToString(prep.AutoDepthStencilFormat));

		//H/W T&L �ɑΉ����Ă�Ȃ�AH/W T&L������
		if((caps.DevCaps && (u32)(D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0) || (FUseSoftwareVertexProcessing) ) {
			//PutTestMessage("�\�t�g�E�F�AT&L�𗘗p���܂�");
			CreateDevice(Adapter, D3DDEVTYPE_HAL, m_owner.m_handle, D3DCREATE_SOFTWARE_VERTEXPROCESSING, prep);
		} else {
			//PutTestMessage("�n�[�h�E�F�AT&L�𗘗p���܂�");
			CreateDevice(Adapter, D3DDEVTYPE_HAL, m_owner.m_handle, D3DCREATE_HARDWARE_VERTEXPROCESSING, prep);
		}

	} else {
		//HEL�̂݁A�V���{
		//PutTestMessage("�\�t�g�E�F�A�G�~�����[�V�����𗘗p���܂�");

		prep.AutoDepthStencilFormat = BestZSFormat(D3DDEVTYPE_REF, depthStencilFmt, prep, Adapter );
		//PutTestMessage("�f�v�X�X�e���V���T�[�t�F�X�̃t�H�[�}�b�g..." + D3DFORMATToString(prep.AutoDepthStencilFormat));

		CreateDevice(Adapter, D3DDEVTYPE_REF, m_owner.m_handle, D3DCREATE_SOFTWARE_VERTEXPROCESSING, prep);
	}

	//DGCarad_debugIndent--;
	//PutTestMessage("��������ʃ��[�h�����������܂���������");
	//PutTestMessage("");

}

IDirect3DSurface9* TDGCarad::GetBackBuffers( s32 idx )
{
	return FBackBuffers[idx];
}

void TDGCarad::CleanupResources()
{
	//PutTestMessage("�S�Ă�DGCarad���\�[�X��������܂�");

	for( u32 i = 0; i < Resources.size(); ++i ){
		Resources[i]->resource->Cleanup();
	}
}

void TDGCarad::RecoverResources()
{
	//PutTestMessage("�S�Ă�DGCarad���\�[�X���č\�z���܂�");

	for( u32 i = 0; i < Resources.size(); ++i ){
		Resources[i]->resource->Recover();
	}
}

void TDGCarad::Loaded()
{
	//u16 cw;  //FPU�̃R���g���[�����[�h�w��p

	////�ȉ��́A���s���̂�
	//if(! (csDesigning in ComponentState) ) {
	//	FD3DDevice = NULL;
	//	FBackBuffers = NULL;
	//	FDepthStencilSurface = NULL;
	//	FBackBufferCount = 0;

	//	//�v���p�e�B�n���h�����Ă�ŁA�e�O���[�o���ϐ��ɓK�p
	//	DebugOption = DebugOption;

	//	//8087CW(0x133f) �Ɠ����AOpenGL�ADirect3D�Ƃ����g���Ƃ��̂��܂��Ȃ�
	//	//�u�s���ȕ����������Z���߁v���o�Ȃ��Ȃ�
	//	cw = 0x133f;
	//	asm
	//		FNCLEX
	//		FLDCW cw;
	//}

	//Resources = TList.Create;
	//NextResourceID = 0;

	//PutTestMessage("");
	//PutTestMessage("�������N�����̏����������܂�������");
	//Inc(DGCarad_debugIndent);

	////Direct3D�̐���
	//PutTestMessage("Direct3D�𐶐����܂�");
	//FD3D = Direct3DCreate9(D3D_SDK_VERSION);
	//if(D3D = NULL ) {
	//	TRYDG(1, "Direct3D�I�v�W�F�N�g���琶���ł��܂���A�_�O");
	//	return;
	//}

	////�ŏ��̓E�B���h�E���[�h�ŏ��������Ƃ�
	//PutTestMessage("�E�B���h�E�̃N���C�A���g�̈�̃T�C�Y�ɍ��킹�ď��������J�n���܂�");
	//WindowMode(FOwner.ClientWidth, FOwner.ClientHeight, []);

	//Dec(DGCarad_debugIndent);
	//PutTestMessage("������DGCarad �N�����܂���������");
	//PutTestMessage("");
}

TDGCarad::TDGCarad( WindowInfo& _owner )
{
	m_owner = _owner;

	//�R���|�[�l���g�̃f�t�H���g�l���Z�b�g
	FDebugOption = dgoHaltOnError | dgoDetailedLog | dgoDebugMessage;
	FUseREFDevice = false;
	FUseSoftwareVertexProcessing = false;

	//u16 cw;  //FPU�̃R���g���[�����[�h�w��p

	//�ȉ��́A���s���̂�
		FD3DDevice = NULL;
		FDepthStencilSurface = NULL;
		FBackBufferCount = 0;

		//�v���p�e�B�n���h�����Ă�ŁA�e�O���[�o���ϐ��ɓK�p
		//DebugOption = DebugOption;

		//8087CW(0x133f) �Ɠ����AOpenGL�ADirect3D�Ƃ����g���Ƃ��̂��܂��Ȃ�
		//�u�s���ȕ����������Z���߁v���o�Ȃ��Ȃ�
		//cw = 0x133f;
		//asm
		//	FNCLEX
		//	FLDCW cw;

	NextResourceID = 0;

	//PutTestMessage("");
	//PutTestMessage("�������N�����̏����������܂�������");
	//Inc(DGCarad_debugIndent);

	//Direct3D�̐���
	//PutTestMessage("Direct3D�𐶐����܂�");
	FD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(FD3D == NULL ) {
		//TRYDG(1, "Direct3D�I�v�W�F�N�g���琶���ł��܂���A�_�O");
		return;
	}

	//�ŏ��̓E�B���h�E���[�h�ŏ��������Ƃ�
	//PutTestMessage("�E�B���h�E�̃N���C�A���g�̈�̃T�C�Y�ɍ��킹�ď��������J�n���܂�");
	//WindowMode(FOwner.ClientWidth, FOwner.ClientHeight, []);

}

TDGCarad::~TDGCarad()
{
		//PutTestMessage("");
		//PutTestMessage("���������ʂ�̎�������Ă܂���܂���������");
		//Inc(DGCarad_debugIndent);

		//���������[�N�̃`�F�b�N
		//while( Resources.size() > 0 ) {
		//	//PutTestMessage(Format("ID %d(%s) ��DGCarad���\�[�X�́A�����I�ɉ������Ă��܂���",[TDGResourceDesc(Resources[0]).id, TDGResourceDesc(Resources[0]).debugName]));
		//	//�Öق̂����ɉ��
		//	free (((TDGResourceDesc*)Resources[0])->resource);
		//}

		//PutTestMessage("DGCarad�I�u�W�F�N�g�̉���ɔ����A�S�Ă̊֘A�I�u�W�F�N�g��������܂�");
		ReleaseDevice();

		//PutTestMessage("Direct3D�I�u�W�F�N�g�̉�����s���܂�");
		//TRYDG(FD3D->Release, "Direct3D�I�u�W�F�N�g�̉���Ɏ��s���܂���");
		FD3D->Release();

		for( u32  i = 0; i < Resources.size(); i++ ){
			SAFE_DELETE(Resources[i]->resource);
			SAFE_DELETE(Resources[i]);
		}
		Resources.clear();

		//Dec(DGCarad_debugIndent);
		//PutTestMessage("����������ł͂�������悤������");
		//PutTestMessage("");

}

u32 TDGCarad::RegisterResouce( TDGCommonResource* res, string debugName /*= ""*/ )
{
	TDGResourceDesc* p;

	//PutTestMessage(Format("DGCarad���\�[�X(%s)��o�^���܂� ID �� %d(%s) �ł�", [res.ClassName, NextResourceID, debugName]));
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
			//PutTestMessage(Format("DGCarad���\�[�X(%s) : ID %d(%s) ��o�^�������܂�", [((TDGResourceDesc*)Resources[i])->resource.ClassName, id, ((TDGResourceDesc*)Resources[i])->debugName]));
			SAFE_DELETE(((TDGResourceDesc*)(*itr))->resource);
			SAFE_DELETE(*itr);
			Resources.erase(itr);
			return;
		}
	}
	//TRYDG(1, Format("���X�g�ɓ����ĂȂ�DGCarad���\�[�X(ID:%d)���AUnregisterResource�葱���ɓn����܂���",[id]));

}

void TDGCarad::CreateDevice( u32 Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, u32 BehaviorFlags, D3DPRESENT_PARAMETERS& presentationParameters )
{
	//���Ɋm�ۂ���Ă���f�o�C�X�I�u�W�F�N�g�̉��
	if(FD3DDevice != NULL ) {
		//PutTestMessage("�V�����f�o�C�X�̐����̑O�ɁA�S�Ă̊֘A�I�u�W�F�N�g��������܂�");
		ReleaseDevice();
	}

	//Direct3D�f�o�C�X�̍쐬
	//PutTestMessage("Direct3DDevice�I�u�W�F�N�g�̐������s���܂�");
	FD3D->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, &presentationParameters, &FD3DDevice);


	//BackBuffer�A�X�e���V���o�b�t�@���Q�b�g
	FBackBufferCount = presentationParameters.BackBufferCount;
	FBackBuffers.resize( FBackBufferCount );

	for( u32  i = 0 ; i < FBackBufferCount; ++i )
		FD3DDevice->GetBackBuffer(0, i, D3DBACKBUFFER_TYPE_MONO, &FBackBuffers[i]);

	if(FD3DDevice->GetDepthStencilSurface(&FDepthStencilSurface) != D3D_OK )
		FDepthStencilSurface = NULL;

	//�v���[���e�[�V�����p�����[�^�̕ۑ�
	FLastPP = presentationParameters;

	//���낢��o���Ƃ�
	FDeviceType = DeviceType;
	FAdapter = Adapter;

	FWindowed = FLastPP.Windowed;
	FBackBufferWidth = FLastPP.BackBufferWidth;
	FBackBufferHeight = FLastPP.BackBufferHeight;
	FBackBufferFormat = FLastPP.BackBufferFormat;

	//���\�[�X�̍č\�z
	RecoverResources();

}

void TDGCarad::ReleaseDevice()
{
	//PutTestMessage("");
	//PutTestMessage("������Direct3D�f�o�C�X��������܂�������");
	//Inc(DGCarad_debugIndent);

	//���\�[�X�̉��
	CleanupResources();

	//�f�v�X�X�e���V���T�[�t�F�X�̉��
	if(FDepthStencilSurface != NULL ) {
		//PutTestMessage("�f�v�X�X�e���V���T�[�t�F�X��������܂�");
		//TRYDG(FDepthStencilSurface.Release, "�f�v�X�X�e���V���T�[�t�F�X�̉���Ɏ��s���܂���");
		FDepthStencilSurface->Release();
		FDepthStencilSurface = NULL;
	}

	//�o�b�N�o�b�t�@�̉��
	if(!FBackBuffers.empty()) {
		//PutTestMessage("�o�b�N�o�b�t�@��������܂�");
		for( u32  i = 0; i < FBackBufferCount; i++ ){
			//TRYDG(BackBuffers[i].Release, "�o�b�N�o�b�t�@ #" + IntToStr(i) + "�@�̉���Ɏ��s���܂���");
			FBackBuffers[i]->Release();
		}
		FBackBuffers.clear();
	}

	if(FD3DDevice != NULL ) {
		//PutTestMessage("Direct3DDevice�I�u�W�F�N�g�̉�����s���܂�");
		//TRYDG(FD3DDevice->Release, "Direct3DDevice�I�u�W�F�N�g�̉���Ɏ��s���܂���");
		FD3DDevice->Release();
		FD3DDevice = NULL;
	}

	//Dec(DGCarad_debugIndent);
	//PutTestMessage("������Direct3D�f�o�C�X��������܂���������");
	//PutTestMessage("");

}

void TDGCarad::WindowMode( u32 width, u32 height, vector<D3DFORMAT>& depthStencilFmt, u32 presentationInterval /*= D3DPRESENT_INTERVAL_DEFAULT*/, D3DSWAPEFFECT swapEffect /*= D3DSWAPEFFECT_DISCARD*/, D3DMULTISAMPLE_TYPE multiSampleType /*= D3DMULTISAMPLE_NONE*/, u32 Adapter /*= D3DADAPTER_DEFAULT*/ )
{
	//PutTestMessage("");
	//PutTestMessage("������Window���[�h�ֈڍs���܂�������");
	//Inc(DGCarad_debugIndent);

	//�������O�Ƀf�o�C�X�I�u�W�F�N�g��S�������c�������Ȃ��ƁA�t���X�N���[������̕ύX�́u���݂̃f�B�X�v���C���[�h�v�������Ȃ�
	ReleaseDevice();

	D3DDISPLAYMODE mode = GetCurrentDisplayMode(Adapter);
	mode.Width = width;
	mode.Height = height;

	SetDisplayMode(Adapter, true, mode, multiSampleType, depthStencilFmt, swapEffect, presentationInterval);

	//�Ȃ���SetWindowPos����Ȃ��Ƃ����ƈʒu�E�T�C�Y���ς���Ă���Ȃ�(^^;)
	SetWindowPos(
		m_owner.m_handle,
		HWND_NOTOPMOST,
		m_owner.m_left,
		m_owner.m_top,
		width - UINT(m_owner.m_clientWidth - m_owner.m_width),
		height - UINT(m_owner.m_clientHeight - m_owner.m_height),
		SWP_SHOWWINDOW);


	//Dec(DGCarad_debugIndent);
	//PutTestMessage("������Window���[�h�ֈڍs���܂���������");
	//PutTestMessage("");

}

void TDGCarad::FullscreenMode( u32 width, u32 height, D3DFORMAT format, vector<D3DFORMAT>& depthStencilFmt, u32 refreshRate /*= 0*/, u32 presentationInterval /*= D3DPRESENT_INTERVAL_IMMEDIATE*/, D3DSWAPEFFECT swapEffect /*= D3DSWAPEFFECT_DISCARD*/, D3DMULTISAMPLE_TYPE multiSampleType /*= D3DMULTISAMPLE_NONE*/, u32 Adapter /*= D3DADAPTER_DEFAULT*/ )
{
	//PutTestMessage("");
	//PutTestMessage("�������t���X�N���[�����[�h�ֈڍs���܂�������");
	//Inc(DGCarad_debugIndent);

	D3DDISPLAYMODE mode;
	mode.Width = width;
	mode.Height = height;
	mode.RefreshRate = refreshRate;
	mode.Format = format;
	SetDisplayMode(Adapter, false, mode, multiSampleType, depthStencilFmt, swapEffect, presentationInterval);

	//Dec(DGCarad_debugIndent);
	//PutTestMessage("�������t���X�N���[�����[�h�ֈڍs���܂���������");
	//PutTestMessage("");

}

void TDGCarad::Reset()
{
	//PutTestMessage("");
	//PutTestMessage("������Direct3DDevice�����Z�b�g���܂�������");
	//Inc(DGCarad_debugIndent);


	//���\�[�X�̃��Z�b�g����
	for( u32  i = 0; i < Resources.size(); ++i ) {
		Resources[i]->resource->Reset();
	}


	//�o�b�N�o�b�t�@�̉��
	for( u32  i = 0; i < FBackBufferCount; ++i ) {
		FBackBuffers[i]->Release();
		FBackBuffers[i] = NULL;
	}

	//�f�v�X�X�e���V���T�[�t�F�X�̉��
	if(FDepthStencilSurface != NULL ) {
		FDepthStencilSurface->Release();
		FDepthStencilSurface = NULL;
	}

	//TRYDG(FD3DDevice->Reset(FLastPP), "���Z�b�g�Ɏ��s���܂���");
	FD3DDevice->Reset(&FLastPP);

	for( u32 i = 0 ; i < FBackBufferCount; ++i )
		FD3DDevice->GetBackBuffer(0, i, D3DBACKBUFFER_TYPE_MONO, &FBackBuffers[i]);


	FD3DDevice->GetDepthStencilSurface(&FDepthStencilSurface);


	//���\�[�X�̃��Z�b�g��C�x���g���Ă�
	for( s32  i = 0; Resources.size(); ++i ) {
		Resources[i]->resource->AfterReset();
	}

	//Dec(DGCarad_debugIndent);
	//PutTestMessage("������Direct3DDevice�����Z�b�g���܂���������");
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
	D3DDISPLAYMODE mode;  //���͂��ꂽ�f�B�X�v���C���[�h
	D3DDISPLAYMODE enumMode;  //�񋓂��ꂽ���[�h
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
			//TRYDG(1, "���b�N�����͈͊O�̃e�N�Z���𑀍�悤�Ƃ��܂���");
			return result;
		}

		if(FLockedLevel != level ) {
			//TRYDG(1, "���b�N�������x���Ɠ������x���Ńe�N�Z���𑀍삵�Ă�������");
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
			//TRYDG(1, "���b�N�����͈͊O�̃e�N�Z���𑀍�悤�Ƃ��܂���");
			return;
		}

		if(FLockedLevel != level ) {
			//TRYDG(1, "���b�N�������x���Ɠ������x���Ńe�N�Z���𑀍삵�Ă�������");
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
	//�f�t�H���g�ł́A�o�b�N�o�b�t�@�̃t�H�[�}�b�g
	FOwner->GetBackBuffers(0)->GetDesc(&desc);
	D3DFORMAT result = desc.Format;

	//�g����t�H�[�}�b�g�����]����T��
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
		//PutTestMessage("�e�N�X�`���̍Đ����ɔ����A�ȑO�̃e�N�X�`����������܂�");
		//TRYDG(FTexture.Release, "�e�N�X�`���̉���Ɏ��s���܂���");
		FTexture->Release();
	}

	//�t�H�[�}�b�g���X�g�����ԗǂ��t�H�[�}�b�g�𓾂�
	FFormat = SearchBestFormat();
	//PutTestMessage(SysUtils.Format("�g�p�����t�H�[�}�b�g�� %s �ł�",[D3DFORMATToString(Format)]));

	//TRYDG(FOwner.FD3DDevice->CreateTexture(w,h, FMipLevel, Usage, Format, Pool, FTexture, NULL), "�e�N�X�`���̐����Ɏ��s���܂���");
	FOwner->Get3DDevice()->CreateTexture(w,h, FMipLevel, FUsage, FFormat, FPool, &FTexture, NULL);

	//PutTestMessage(SysUtils.Format("�c��e�N�X�`���e�ʂ́A��%fMB�ł�", [FOwner.FD3DDevice->GetAvailableTextureMem / (1024*1024)]));

	FPixelSize = GetPixelBits(FFormat) / 8;

}

void TDGTexture::Cleanup()
{
	if(FTexture != NULL ) {
		//���݂̏�Ԃ�ۑ�(kmFromStream�̎�)
		//kmFromFile,kmFromQDA�̎��́A�t�@�C������ǂݍ���
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
		case kmDontKeep:     //���g�͕������Ȃ��Ă���
			SetSize(FWidth, FHeight);
			break;
		case kmFromFile: { //�t�@�C������ǂݒ���(�e�N�X�`���̐����́A�ǂݍ��݃��\�b�h���ōs����)
				LoadFromFile(KeepFilename);
			}
			break;
		case kmFromQDA: {  //QDA����
			LoadFromFile(KeepQDAID, KeepQDAName);
			}
			break;
		case kmFromStream: {
			 LoadFromStream(KeepStream); //�������āA����܂Ŏg���Ă�KeepStream�͏���
			 KeepStream.str("");
			}
			break;
		}
	} else {
		//�I�u�W�F�N�g�����͕���
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

	//POOL_Default�͎����������Ȃ�
	FAutoKeep  =  FPool != D3DPOOL_DEFAULT;

	//�t�H�[�}�b�g���X�g���R�s�[����
	for( s32 i=0; i<sizeof(formats)/sizeof(D3DFORMAT); i++ )
		FormatList.push_back(formats[i]);

	//DGCarad�ɓo�^
	ID = FOwner->RegisterResouce(this, debugName);
	FDebugName = debugName;
	//ID = FOwner.RegisterTexture(Self);


}

TDGTexture::~TDGTexture()
{
	//PutTestMessage("�e�N�X�`���I�u�W�F�N�g��������܂�");
	if(FTexture != NULL ) {
		//TRYDG(FTexture.Release, "�e�N�X�`���̉���Ɏ��s���܂���");
		FTexture->Release();
	}

	KeepStream.str("");

	//�o�^����
	FOwner->UnregisterResource(this->ID);

	FormatList.clear();

}

void TDGTexture::LoadFromFile( string filename, string qdaName /*= ""*/ )
{
	BITMAPINFOHEADER bi;

	if(qdaName.empty() ) {
		//�A�[�J�C�u���������ꂽ�Ȃ�A�t�@�C������ǂݍ���

		//PutTestMessage(Sysutils.Format("ID %d(%s) �̃e�N�X�`���ɁA%s ��ǂݍ��݂܂�",[ID, FDebugName, filename]));

		try{
			ifstream ss;
			ss.open(filename);

			//�w�b�_��ǂ�
			ss.seekg(sizeof(BITMAPFILEHEADER), std::ios::beg);
			ss.read(reinterpret_cast<char*>(&bi),sizeof(BITMAPINFOHEADER));
			SetSize(bi.biWidth, bi.biHeight);

			//�����߂�
			ss.seekg(0, std::ios::beg);
			LoadBMPStreamToTexture(FTexture, ss);
			ss.close();
		}
		catch (std::ifstream::failure e) {
		//	TRYDG(1, filename + "����́A�ǂݍ��݃G���[');
		}

		//�������������X�V
		KeepMethod = kmFromFile;
		KeepFilename = filename;
	} else {
		//�A�[�J�C�u����ǂݍ���

		//PutTestMessage(Sysutils.Format("ID %d(%s) �̃e�N�X�`���ɁAQDA�t�@�C��%s ���� ID %s ��ǂݍ��݂܂�",[ID, FDebugName, qdaName, filename]));
		try{
			stringstream ms;
			ExtractFromQDAFile(ms, qdaName, filename);

			//�w�b�_��ǂ�
			ms.seekg(sizeof(BITMAPFILEHEADER), std::ios::beg);
			ms.read(reinterpret_cast<char*>(&bi), sizeof(BITMAPINFOHEADER));
			SetSize(bi.biWidth, bi.biHeight);

			//�����߂�
			ms.seekg(0, std::ios::beg);
			LoadBMPStreamToTexture(FTexture, ms);
		}
		catch (std::ifstream::failure e) {
		//	TRYDG(1, filename + " �� " + qdaName + "����́A�ǂݍ��݃G���[');
		}

		//�������������X�V
		KeepMethod = kmFromQDA;
		KeepQDAName = qdaName;
		KeepQDAID = filename;
	}
}

void TDGTexture::LoadFromStream( istream& source )
{
	BITMAPINFOHEADER bi;
	ifstream::pos_type pos;//�ǂݏo���O�̈ʒu
	//PutTestMessage(Sysutils.Format("ID %d �̃e�N�X�`���ɁA�X�g���[������ǂݍ��݂܂�",[ID]));

	try{
		//�w�b�_��ǂ�
		pos = source.tellg();
		source.seekg(sizeof(BITMAPFILEHEADER), std::ios::beg);
		source.read(reinterpret_cast<char*>(&bi), sizeof(BITMAPINFOHEADER));
		SetSize(bi.biWidth, bi.biHeight);

		//�����߂��ēǂݍ���
		source.seekg(pos);
		LoadBMPStreamToTexture(FTexture, source);

		//�������������X�V�c�K�v�ɂȂ���������X�g���[���ɏ���ɕۑ�������
		KeepMethod = kmFromStream;
	}
	catch (std::ifstream::failure e) {
		//TRYDG(1, "�X�g���[������́A�ǂݍ��݃G���[');
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

	//�w�b�_�쐬
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

	//�w�b�_��������
	dest.write(reinterpret_cast<char*>(&bf), sizeof(bf));
	dest.write(reinterpret_cast<char*>(&bi), sizeof(bi));

	lineBuf.resize(FWidth);

	//�ϊ��B�㉺�t�ɏ�������
	LockRect(NULL, 0, Rect(0,0,FWidth,FHeight), D3DLOCK_READONLY);
	for( s32  y = FHeight-1; y >= 0; y-- ) {
		pLine = (u8*)FLockedRect.pBits;
		pLine += y*FLockedRect.Pitch;
		pPix = pLine;

		//�ꃉ�C���ϊ�����
		for( u32  x = 0; x < FWidth; x++ ) {
			col = 0;
			memmove(&col, pPix, FPixelSize);
			lineBuf[x] = PhysicalToARGB(col, FFormat);
			pPix += FPixelSize;
		}
		//��������
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

//�J���[�L�[�̐ݒ�A�ϊ����[�`��
u32 SetColorKeyOP( TDGTexture* Sender, u32 src, s32 x, s32 y, void* data)
{
	D3DCOLOR col = PhysicalToARGB(src, Sender->GetFormat()) & 0x00FFFFFF;

	if( col != *(u32*)data ) 
		col |= 0xFF000000;

	return D3DColorToPhysical(col, Sender->GetFormat());
}


void TDGTexture::SetColorKey( Rect& region, D3DCOLOR col )
{
	u32 ck = D3DColorToPhysical(col, FFormat) & 0x00FFFFFF;  //���l����U�[���ɂ���OP�ɓn��
	PerPixelOperation(region, SetColorKeyOP, &ck);

}

//�̋P�x�����l��(�ϊ����[�`���{��)
u32 BlueToAlphaOP( TDGTexture* Sender, u32 src, s32 x, s32 y, void* data)
{
	D3DCOLOR col = PhysicalToARGB(src, Sender->GetFormat());
	col = (col & 0x00FFFFFF) | (col & 0xFF << 24);  //B �� A �ɃR�s�[
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

		//�ŏ��̈ꃉ�C���͓]�����܂���łȂ�Ƃ�����
		u8* pPix = (u8*)FLockedRect.pBits;
		for( s32  x = region.Left; x < region.Right; x++ ) {
			memmove(pPix, &phys, FPixelSize);
			pPix += FPixelSize;
		}

		//���Ƃ̓R�s�[
		pPix = (u8*)FLockedRect.pBits;
		for( s32  y = region.Top+1; y < region.Bottom; y++ ) {
			pPix += FLockedRect.Pitch;
			memmove(pPix, FLockedRect.pBits, FWidth * FPixelSize);
		}

	} else {

		//�}�X�N�t���c�P�ɓ]���Ƃ����킯�ɂ͂�����̂�
		for( s32  y = region.Top; y < region.Bottom; y++ ) {
			u8* pPix = (u8*)FLockedRect.pBits;
			pPix += (y-region.Top)  * FLockedRect.Pitch;
			for( s32  x = region.Left; x < region.Right; x++ ) {
				//�Q�b�`��
				u32 phys = 0;
				memmove(&phys, pPix, FPixelSize);

				D3DCOLOR srcCol = PhysicalToARGB(phys, FFormat);
				srcCol = (srcCol & 0xFF000000) | (col & 0x00FFFFFF); 

				//�ϊ����Ă�����
				phys = D3DColorToPhysical(srcCol, FFormat);
				memmove(pPix, &phys, FPixelSize);

				pPix += FPixelSize;
			}
		}
	}

	UnlockRect();

}

//2�̐����ׂ��A���Ax�ȏ�̒��ōł����������𓾂�
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

	//Width�Ƃ��āA�ŏ��Ɏw�肳�ꂽw,h���L�^
	FWidth = w;
	FHeight = h;

	FOwner->GetD3D()->GetDeviceCaps(FOwner->GetAdapter(), FOwner->GetDeviceType(), &caps);

	//if((caps.MaxTextureWidth < w) || (caps.MaxTextureHeight < h) )
	//	PutTestMessage(SysUtils.Format("�v�����ꂽ�e�N�X�`���̃T�C�Y�͑傫�����܂��B%d x %d �ȓ��ɂ��ĉ�����",[caps.MaxTextureWidth, caps.MaxTextureHeight]));

	//SQUAREONLY�̏ꍇ�A��ӂ̃T�C�Y���f�J�����ɍ��킹��
	if((caps.TextureCaps && D3DPTEXTURECAPS_SQUAREONLY) != 0 ) {
		if(w > h )
			h = w;
		else
			w = h;
	}

	//POW2�Ȃ�A2�̐����ׂ��ɂ��낦��
	if((caps.TextureCaps && D3DPTEXTURECAPS_POW2) != 0 ) {
		w = AlignP2(w);
		h = AlignP2(h);
	}

	//TotalWidth,TotalHeight�Ƃ��āA���H���w,h���L�^
	FTotalWidth = w;
	FTotalHeight = h;

	//�E������uv���W���Z�o
	FU = (f32)FWidth / (f32)FTotalWidth;
	FV = (f32)FHeight / (f32)FTotalHeight;

	//�e�N�X�`������
	CreateTexture(w, h);

	//�{�[�_�[�F�Ŗ��߂�
	if(FPool != D3DPOOL_DEFAULT )
		Fill(Rect(0,0,FTotalWidth,FTotalHeight), FBorderColor, false);

	//�L�[�v���̍폜(���J�o�[��������)
	if(! Recovering ) {
		KeepStream.str("");
		KeepMethod = kmFromStream;
	}

}

bool TDGTexture::LockRect( D3DLOCKED_RECT* _out, u32 level, Rect& rect, u32 flags )
{
	FLocked = true;

	//���b�N���ď����������e�N�X�`���́A�t�@�C������̎��������̑Ώۂɂ��Ȃ�
	if((flags && D3DLOCK_READONLY) == 0 ){
		if(KeepMethod != kmDontKeep ){
			KeepMethod = kmFromStream;
		}
	}

	if(FTexture == NULL ) {
		//TRYDG(1, "�e�N�X�`�����m�ۂ���Ă��܂���BLockRect�̑O��SetSize���邩�ABMP��ǂݍ���ł�������");
		return false;
	}

	FLockedRegion = rect;
	//TRYDG(FTexture.LockRect(level, FLockedRect, &rect, flags), "LockRect�Ɏ��s���܂���");
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
	//TRYDG(FTexture.UnlockRect(FLockedLevel), "�A�����b�N�Ɏ��s���܂���");
	FTexture->UnlockRect(FLockedLevel);
	FLocked = false;

}

void TDGVertexBuffer::SetVertexCount( u32 Value )
{
	if(FVBuf != NULL )
		FVBuf->Release();
		//TRYDG(FVBuf.Release, "VertexBuffer�̉���Ɏ��s���܂���");

	if((KeepBuf != NULL) && (Value != FVertexCount) ) {
		//Value = FVertexCount�̏ꍇ�́A����KeepBuf�������Ȃ�(Recover���̂��Ƃ�����)
		SAFE_FREE(KeepBuf);
	}

	FVertexCount  =  Value;
	//TRYDG( FOwner.FD3DDevice->CreateVertexBuffer(VertexSize*Count, Usage, FVF, Pool, FVBuf, NULL), "VertexBuffer�̐����Ɏ��s���܂���");
	FOwner->Get3DDevice()->CreateVertexBuffer(FVertexSize * FVertexCount, FUsage, FFVF, FPool, &FVBuf, NULL);

}

void TDGVertexBuffer::Cleanup()
{
	if( FAutoKeep )
		Keep();

	if( FVBuf != NULL ) {
		//TRYDG(FVBuf.Release, "VertexBuffer�̉���Ɏ��s���܂���");
		FVBuf->Release();
		FVBuf = NULL;
	}
}

void TDGVertexBuffer::Recover()
{

	SetVertexCount(FVertexCount);  

	//�����߂�
	//D3DPOOL_DEFAULT��������A���e�͖��t���[���t���b�V������Ă��ǂ����Ƃ��O��
	//�Ȃ̂ŁA�����߂��Ȃ�
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
	//D3DPOOL_DEFAULT��������A���e�͖��t���[���t���b�V������Ă��ǂ����Ƃ��O��
	//�Ȃ̂ŁA�L�[�v���Ȃ�
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

	//�o�^
	ID = FOwner->RegisterResouce(this, debugName);

}

TDGVertexBuffer::~TDGVertexBuffer()
{
	if(KeepBuf != NULL )
		SAFE_FREE(KeepBuf);

	//TRYDG(FVBuf.Release, "VertexBuffer�̉���Ɏ��s���܂���");
	FVBuf->Release();

	FOwner->UnregisterResource(ID);

}

void* TDGVertexBuffer::Lock( u32 start, u32 count, u32 Flags )
{
	//TRYDG( FVBuf.Lock(start*VertexSize, count*VertexSize, PLocked, Flags), "VertexBuffer�̃��b�N�Ɏ��s���܂���");
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
	//TRYDG(FVBuf.Unlock, "VertexBuffer�̃A�����b�N�Ɏ��s���܂���");
	FVBuf->Unlock();
	FLocked = false;
}

void TDGVertexBuffer::GetVertex( u32 idx, void* v )
{
	u8* p;
	u32 ofs;
	if(! FLocked ) {
		//�����b�N�Ȃ烍�b�N���Ă���
		if(idx >= FVertexCount )
			return;
			//TRYDG(1, "VertexBuffer�͈̔͊O��ǂݍ������Ƃ��܂���");

		p = (u8*)Lock(idx, 1, D3DLOCK_READONLY);
		memmove(v, p, FVertexSize);
		Unlock();
	} else {
		//�͈̓`�F�b�N���ēǂݍ���
		if((idx < FLockStart) || (idx > FLockEnd) ) {
			//TRYDG(1, "VertexBuffer�̃��b�N���ꂽ�͈͂̊O��ǂ����Ƃ��܂���");
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
	//�����b�N�Ȃ烍�b�N����
	if(! FLocked ) {
		if(idx >= FVertexCount ){
			return;
			//TRYDG(1, "VertexBuffer�͈̔͊O�ɏ����������Ƃ��܂���");
		}
		p = (u8*)Lock(idx, 1, 0);
		memmove(p, v, FVertexSize);
		Unlock();
	} else {
		//�͈̓`�F�b�N���ď�������
		if((idx < FLockStart) || (idx > FLockEnd) ){
			//TRYDG(1, "VertexBuffer�̃��b�N���ꂽ�͈͂̊O�֏����������Ƃ��܂���");
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
		//�����b�N�Ȃ烍�b�N���Ă���

		if(idx >= FCount ){
			return 0;
			//TRYDG(1, "IndexBuffer�͈̔͊O��ǂݍ������Ƃ��܂���");
		}
		//Lock(idx*sizeof(Word), sizeof(Word), D3DLOCK_READONLY);
		result = *Lock(idx,1,D3DLOCK_READONLY);
		Unlock();
	} else {
		//�͈̓`�F�b�N���ēǂݍ���
		if((idx < FLockStart) || (idx > FLockEnd) ) {
			//TRYDG(1, "IndexBuffer�̃��b�N���ꂽ�͈͂̊O��ǂ����Ƃ��܂���");
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
		//TRYDG(FIBuf.Release, "IndexBuffer�̉���Ɏ��s���܂���");
	}

	if((KeepBuf != NULL) && (Value != FCount) ) {
		//Value = FIndexCount�̏ꍇ�́A����KeepBuf�������Ȃ�(Recover���̂��Ƃ�����)
		SAFE_FREE(KeepBuf);
	}

	FCount  =  Value;
	FOwner->Get3DDevice()->CreateIndexBuffer(FCount*sizeof(u16), FUsage, D3DFMT_INDEX16, FPool, &FIBuf, NULL);
	//TRYDG( FOwner.FD3DDevice->CreateIndexBuffer(FCount*sizeof(Word), Usage, D3DFMT_INDEX16, Pool, FIBuf, NULL), "IndexBuffer�̐����Ɏ��s���܂���");

}

void TDGIndexBuffer::SetIndices( u32 idx, u16 Value )
{
	u16* p;
	if(! FLocked ) {
		//�����b�N�Ȃ烍�b�N���Ă���

		if(idx >= FCount ){
			return;
			//TRYDG(1, "IndexBuffer�͈̔͊O�֏����������Ƃ��܂���");
		}
		Lock(idx,1,0);
		*((u16*)PLocked) = Value;
		Unlock();
	} else {
		//�͈̓`�F�b�N���ēǂݍ���
		if((idx < FLockStart) || (idx > FLockEnd) ) {
			//TRYDG(1, "IndexBuffer�̃��b�N���ꂽ�͈͂̊O�֏������Ƃ��܂���");
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
		//TRYDG(FIBuf.Release, "IndexBuffer�̉���Ɏ��s���܂���");
		FIBuf = NULL;
	}

}

void TDGIndexBuffer::Recover()
{
	SetCount( FCount );  //�v���p�e�B�n���h���̌Ăяo���ŁAIBuf�Đ���

	//�����߂�
	//D3DPOOL_DEFAULT��������A���e�͖��t���[���t���b�V������Ă��ǂ����Ƃ��O��
	//�Ȃ̂ŁA�����߂��Ȃ�
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
	//D3DPOOL_DEFAULT��������A���e�͖��t���[���t���b�V������Ă��ǂ����Ƃ��O��
	//�Ȃ̂ŁA�L�[�v���Ȃ�
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
	//�o�^
	ID = FOwner->RegisterResouce(this, debugName);

}

TDGIndexBuffer::~TDGIndexBuffer()
{
	if(KeepBuf != NULL )
		SAFE_FREE(KeepBuf);

	FIBuf->Release();
	//TRYDG(FIBuf.Release, "IndexBuffer�̉���Ɏ��s���܂���");

	FOwner->UnregisterResource(ID);

}

u16* TDGIndexBuffer::Lock( u32 start, u32 count, u32 Flags )
{
	FIBuf->Lock(start*sizeof(u16), count*sizeof(u16), &PLocked, Flags);
	//TRYDG( FIBuf.Lock(start*sizeof(Word), count*sizeof(Word), PLocked, Flags), "IndexBuffer�̃��b�N�Ɏ��s���܂���");

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
	//TRYDG(FIBuf.Unlock, "IndexBuffer�̃A�����b�N�Ɏ��s���܂���");
	FLocked = false;

}
