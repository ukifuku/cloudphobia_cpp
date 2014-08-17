//(.+) = class\((.+)\)
#pragma once

using namespace std;

#include "types.h"
#include "TOBJ.h"
#include <vector>

//�z���f��
class TKagerouBasic : public TOBJ {
  private:
    s32 Rdeg;//��]�p
    s32 Speed;//�X�s�[�h
    u16 Pattern;
  public:
    TKagerouBasic(TOBJList* owner, s32 sx, s32 sy, s32 R, s32 S, s32 _lens, s32 _pat, f32 _zoom);
    void Move();  
    void Die();  
};

//�z������
class TKageExp : public TOBJ {
  private:
    s32 Life;
    u16 Level;
  public:
    TKageExp(TOBJList* owner, s32 sx, s32 sy, u16 _level);
    void Move();  
};

//�ʌ^�z��
class TBallKage : public TOBJ {
  private:
    s32 Rdeg;//��]�p
    s32 Speed;//�X�s�[�h
    u16 Pattern;
    s32 Life;
  public:
    TBallKage(TOBJList* owner, s32 sx, s32 sy, s32 R, s32 S, s32 _lens, s32 _pat, f32 _zoom);
    void Move();  
};

//���U�C�N�f��
class TMosaicBasic : public TOBJ {
  private:
  public:
    TMosaicBasic(TOBJList* owner, s32 sx, s32 sy, s32 _lens, s32 _width);
    void Move();  
};

//�u���[�~���O�f��
class TBloomingBasic : public TOBJ {
  private:
    f32 Current[5];
    f32 DestCol[5];
    f32 Speed[5];
    s32 Height,Width,Z;
  public:
    TBloomingBasic(TOBJList* owner, s32 _x, s32 _y, s32 _width, s32 _height, s32 _z);
    void Move();  
    void Show();
    void SetColor( s32 _add, s32 _alpha, s32 _r, s32 _g, s32 _b);
    void OnDeviceReset(); 
    void SetColorSoon( s32 _add, s32 _alpha, s32 _r, s32 _g, s32 _b);
};

//�ڂ����G�t�F�N�g
class TBlur : public TOBJ {
  private:
    s32 Alpha;       //�ڂ����̃A���t�@�l
    bool IsResume;    //�|�[�Y����̕��A���߂��o����
  public:
    TBlur(TOBJList* owner);
    void Move();  
    void Fadein();
    void Fadeout();
    void Hide();
    void Show();
    void Resume();
    void OnDeviceReset(); 
};

//�z���f�Ѓ����C�N
class TKagerouBasic2 : public TOBJ {
  private:
    s32 Rdeg;//��]�p
    s32 Speed;//�X�s�[�h
    u16 Pattern;
    s32 ActFlg;
  public:
    TKagerouBasic2(TOBJList* owner, s32 sx, s32 sy, s32 R, s32 S, s32 _lens, s32 _pat, f32 _zoom);
    void Move();  
    void Die();  
};

//���H�f��
class TWaterDrop : public TOBJ {
  private:
    s32 Alpha;
    s32 Size;
    f32 VX,VY;
  public:
    TWaterDrop(TOBJList* owner, s32 sx, s32 sy, s32 _size);
    void Move();  
    s32 OnExRender(); 
};

//���H�R���g���[��
class TWaterControl : public TOBJ {
  private:
    s32 Life;
  public:
    TWaterControl(TOBJList* owner, s32 sx, s32 sy);
    void Move();  
};

//�J
class TRain : public TOBJ {
  private:

  public:
    TRain(TOBJList* owner, s32 sx, s32 sy);
    void Move();  
};

//�Î��S�[�O��
class TNightVision : public TOBJ {
  private:
    s32 actflg;
    s32 black,white;
    bool whiteout;
  public:
    TNightVision(TOBJList* owner);
    void Move();  
    void Show(bool _show);
    void MakeWhite();
};

//�����X�N���[���c��
class TStage2ScrollBlur : public TOBJ {
  private:
    bool ShowState;
  public:
    TStage2ScrollBlur(TOBJList* owner);
    void Move();  
    u16 Show(bool _show);
};

u32 TotalKage;

//���U�C�N����ׂ�
void Mosaic(TOBJList* owner, s32 sx, s32 sy, s32 _width, s32 _height, s32 _level);

#define		MaxKage  32
