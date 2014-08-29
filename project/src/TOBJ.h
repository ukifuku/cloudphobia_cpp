#pragma once

#include "types.h"
#include "Define.h"
#include "PolyJoint.h"
class TOBJList;
class TJoint;

#define MaxJoint 49     //最大間接数

//オブジェクト基幹クラス
class TOBJ
{
friend TOBJList;
protected:
	string FObjectName;   //オブジェクトの名前（デバッグログ用）
	s32 Age;			  //生成されてからの時間
	s32 FID;
	TOBJKind FKind;     //敵の種類
	TOBJList* FOwner;    //オブザーバーとなるTOBJListのインスタンス
	bool FDead;      //死にフラグ
	s32 FSTR;       //当たったときの攻撃力
	s32 FDef;       //防御力
	s32 Score;      //得点
	s32 Locknum;    //ロックオン番号
	s32 numSpr;                //オブジェクト一個あたりのスプライト数
	bool CommandRed;
	s32 ComBonus;
	f32 ResilienceX;
	f32 ResilienceY;   //反発力（当たり判定後の衝突応答で使用）
	bool FIsParentExists;     //親がいるか（リセット時、消滅処理を親に任せているか）

public:
	s32 Sprite_Center;
	s32 LastSlashed;			 //ブレード連続ヒット防止カウンタ
	TJoint* Sprite[MaxJoint];   //スプライト
	s32 X;
	s32 Y;        //座標
	s32 HP;         //ヒットポイント
	s32 TempHP;     //ロックオン処理用HP
	bool Hit[MaxJoint];  //当たりフラグ
	s32 distance;   //あるオブジェクト間の距離
	// TempSlide:TPoint;   //振動したときの移動量
	s32 BonusRate; //ボーナスレート
	s32 TempValue[8];
	s32 MaxHP;
	bool deleted;

public:
	TOBJ( TOBJList* _owner );
	virtual ~TOBJ();
	virtual void MakeSprite( TDGCarad* _dddd, TDGTexture* _Tex, s32 _num, s32 TX, s32 TY, s32 TW, s32 TH, s32 Z, s32 HX, s32 HY, s32 HW, s32 HH, s32 _cx, s32 _cy, bool com );
	virtual void MakeSpriteLite( s32 _num, s32 _x, s32 _y, s32 _w, s32 _h, s32 _z, TBlendMode _blend );
	virtual bool CheckTouch( TOBJ* Target );
	virtual void Move();
	virtual void OnDamaged( TOBJ* Target );
	virtual void Die();
	virtual s32 OnDraw();
	virtual TOBJKind Kind() { return FKind; }
	virtual s32 STR() { return FSTR; }
	virtual bool Dead() { return FDead; }
	virtual s32 ID() { return FID; }
	virtual s32 Def() { return FDef; }
	virtual bool IsParentExists() { return FIsParentExists; }
	virtual void OnBroken(); 
	virtual void OnResponse(stringstream& _res);
	virtual s32 OnExRender();
	virtual void OnDeviceLost(); 
	virtual void OnDeviceReset(); 
	virtual s32 GetAge();
	virtual void MakeSprFromID( TDGCarad* _dddd, s32 _id, TDGTexture* _tex, s32 _z, bool _extra );
	virtual void MotionControl( s32 _id );
	virtual void MotionControlSoon( s32 _id );
	virtual void CollisionReaction( f32 _x, f32 _y );

protected:
	virtual void CalcBonus();
};