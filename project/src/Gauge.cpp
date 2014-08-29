#include "Gauge.h"
#include "TOBJList.h"

THPGauge::THPGauge( TOBJList* owner, s32 _x, s32 _y, s32 _now, s32 _max ) : TOBJ(owner)
{
X:=_x;
Y:=_y;
FKind:=kGauge;

	if _now<0 then _now:=0;

	with sblt do begin
RBM     := bmNormal; // αブレンドモード
RColor  := $FFFFFFFF; // ポリゴン色
RDDDD   := QD;     // TDDDDの参照
RHeight := 16;       //高さ
RWidth  := 160;      //幅
RX      := 0;        // X座標
RY      := 0;        // Y座標
RZ      := 500;       // Z値
	end;

	Sprite[0]:= TJoint.Create(@sblt);         // スプライトの生成
	sblt.RZ:=600;
	Sprite[1]:= TJoint.Create(@sblt);
	sblt.RZ:=700;
	Sprite[2]:= TJoint.Create(@sblt);
	Inc(numSpr,3);
	with Sprite[0].Spr do begin
		AddTexture(Eff1Tex, 32, 1, 192, 15);    // テクスチャを登録
	Move(X,Y);
	Zoom(1,1);
	//SetColor(0,ctRed);
	//SetColor(0,ctBlue);
	//SetColor(255,ctGreen);
Visible:=True;
Smooth:=False;
	end;

	with Sprite[1].Spr do begin
		AddTexture(Eff1Tex, 17, 1, 31, 15);    // テクスチャを登録
	Move(X+2,Y+4);
	Zoom(_now/_max*0.975,0.5);
Visible:=True;
	SetColor(255,ctRed);
	SetColor(0,ctBlue);
	SetColor(120,ctGreen);
	//SetColor(128,ctAlpha);
Smooth:=False;
	end;

	with Sprite[2].Spr do begin
		AddTexture(Eff1Tex, 17, 1, 31, 15);    // テクスチャを登録
	Move(X+2,Y+4);
	Zoom(0.98,0.5);
Visible:=True;
	SetColor(0,ctRed);
	SetColor(0,ctBlue);
	SetColor(0,ctGreen);
	SetColor(96,ctAlpha);
Smooth:=False;
	end;


}

void THPGauge::Move()
{
	Die;
	inherited;

}


TMSGauge::TMSGauge( TOBJList* owner, s32 _x, s32 _y, s32 _now, s32 _max ) : TOBJ(owner)
{

}

void TMSGauge::Move()
{

}

TEnemyGauge::TEnemyGauge( TOBJList* owner, s32 _x, s32 _y, s32 _now, s32 _max ) : TOBJ(owner)
{

}

void TEnemyGauge::Move()
{

}

TBossGauge::TBossGauge( TOBJList* owner, s32 _x, s32 _y, s32 _now, s32 _max ) : TOBJ(owner)
{

}

void TBossGauge::Move()
{

}

TWarningGauge::TWarningGauge( TOBJList* owner, s32 _x, s32 _y, s32 _distance, s32 _pat ) : TOBJ(owner)
{

}

void TWarningGauge::Move()
{

}

TMiSGauge::TMiSGauge( TOBJList* owner, s32 _x, s32 _y, s32 _now ) : TOBJ(owner)
{

}

void TMiSGauge::Move()
{

}

TSoundGauge::TSoundGauge( TOBJList* owner, s32 _x, s32 _y, s32 _now ) : TOBJ(owner)
{

}

void TSoundGauge::Move()
{

}

THPGauge2::THPGauge2( TOBJList* owner, s32 _x, s32 _y, s32 _pat ) : TOBJ(owner)
{

}

void THPGauge2::Move()
{

}

TMSGauge2::TMSGauge2( TOBJList* owner, s32 _x, s32 _y, s32 _pat ) : TOBJ(owner)
{

}

void TMSGauge2::Move()
{

}

TMiSGauge2::TMiSGauge2( TOBJList* owner, s32 _x, s32 _y, s32 _pat ) : TOBJ(owner)
{

}

void TMiSGauge2::Move()
{

}

TMiSGaugeParts::TMiSGaugeParts( TOBJList* owner, s32 _x, s32 _y, s32 _num, s32 _pat ) : TOBJ(owner)
{

}

void TMiSGaugeParts::Move()
{

}

TEnemyGauge2::TEnemyGauge2( TOBJList* owner, TOBJ* _object, s32 _ox, s32 _oy ) : TOBJ(owner)
{

}

void TEnemyGauge2::Move()
{

}

void TEnemyGauge2::Show()
{

}

void TEnemyGauge2::Hide()
{

}

s32 TEnemyGauge2::OnDraw()
{

}

TBossGauge2::TBossGauge2( TOBJList* owner, TOBJ* _object, s32 _ox, s32 _oy ) : TOBJ(owner)
{

}

void TBossGauge2::Move()
{

}

void TBossGauge2::Show()
{

}

void TBossGauge2::Hide()
{

}

TLimitTime::TLimitTime( TOBJList* owner, s32 _x, s32 _y ) : TOBJ(owner)
{

}

void TLimitTime::Move()
{

}

void TLimitTime::Die()
{

}

TMojiBack::TMojiBack( TOBJList* owner, s32 _x, s32 _y, s32 _long ) : TOBJ(owner)
{

}

void TMojiBack::Move()
{

}

TFPSIndicator::TFPSIndicator( TOBJList* owner, s32 _x, s32 _y ) : TOBJ(owner)
{

}

void TFPSIndicator::Move()
{

}

void TFPSIndicator::Die()
{

}

TWarningGauge2::TWarningGauge2( TOBJList* owner, s32 _x, s32 _y, s32 _distance, s32 _pat ) : TOBJ(owner)
{

}

void TWarningGauge2::Move()
{

}

TWarningGaugePart::TWarningGaugePart( TOBJList* owner, s32 _x, s32 _y, s32 _pat ) : TOBJ(owner)
{

}

void TWarningGaugePart::Move()
{

}

TWarningGaugeCircle::TWarningGaugeCircle( TOBJList* owner, s32 _x, s32 _y ) : TOBJ(owner)
{

}

void TWarningGaugeCircle::Move()
{

}

TConnectingGauge::TConnectingGauge( TOBJList* owner ) : TOBJ(owner)
{

}

TConnectingGauge::~TConnectingGauge()
{

}

void TConnectingGauge::Move()
{

}

void TConnectingGauge::Die()
{

}

void TKeyBoard::MakeButton( const string& _moji, s32 _x, s32 _y )
{

}

TKeyBoard::TKeyBoard( TOBJList* owner ) : TOBJ(owner)
{

}

TKeyBoard::~TKeyBoard()
{

}

void TKeyBoard::Move()
{

}

void TKeyBoard::Imput( s32 _x, s32 _y, bool _b1, bool _b2, bool _b3 )
{

}

TAttractionCursorPart::TAttractionCursorPart( TOBJList* owner, s32 _x, s32 _y, s32 _long ) : TOBJ(owner)
{

}

void TAttractionCursorPart::Move()
{

}

void TAttractionCursorPart::SetColor( s32 _r, s32 _g, s32 _b )
{

}

TAttractionCursor::TAttractionCursor( TOBJList* owner, s32 _x, s32 _y, s32 _long, s32 _life ) : TOBJ(owner)
{

}

TAttractionCursor::~TAttractionCursor()
{

}

void TAttractionCursor::Move()
{

}

TBGMTitle::TBGMTitle( TOBJList* owner, const string& _title ) : TOBJ(owner)
{

}

TBGMTitle::~TBGMTitle()
{

}

void TBGMTitle::Move()
{

}

TMothership_Tutorial::TMothership_Tutorial( TOBJList* owner, s32 _x, s32 _y ) : TOBJ(owner)
{

}

void TMothership_Tutorial::Move()
{

}

TDangerAlarm::TDangerAlarm( TOBJList* owner, s32 _x ) : TOBJ(owner)
{

}

void TDangerAlarm::Move()
{

}

void TDangerAlarm::Place( s32 _x )
{

}

void TDangerAlarm::Show( bool _visible )
{

}

void TStSelCursor::MakeCursor( const string& _caption, s32 _x, s32 _y )
{

}

void TStSelCursor::SetStageTitle( s32 _num )
{

}

TStSelCursor::TStSelCursor( TOBJList* owner ) : TOBJ(owner)
{

}

void TStSelCursor::Move()
{

}

void TStSelCursor::Up()
{

}

void TStSelCursor::Down()
{

}

void TStSelCursor::Die()
{

}

void TStSelCursor::ShowStar( bool _tf )
{

}

TLoadingScreen::TLoadingScreen( TOBJList* owner ) : TOBJ(owner)
{

}

TLoadingScreen::~TLoadingScreen()
{

}

void TLoadingScreen::Move()
{

}

void TLoadingScreen::Die()
{

}

TCountDown::TCountDown( TOBJList* owner, s32 _x, s32 _y, s32 _pat ) : TOBJ(owner)
{

}

void TCountDown::Move()
{

}

TMotherShipDamaged::TMotherShipDamaged( TOBJList* owner, s32 _x, s32 _y ) : TOBJ(owner)
{

}

void TMotherShipDamaged::Move()
{

}

TJikiHitArea::TJikiHitArea( TOBJList* owner, TOBJ* _parent ) : TOBJ(owner)
{

}

void TJikiHitArea::Move()
{

}

void TJikiHitArea::Show()
{

}

void TJikiHitArea::Place( f32 _x, f32 _y )
{

}

TPracticeTitle::TPracticeTitle( TOBJList* owner ) : TOBJ(owner)
{

}

TPracticeTitle::~TPracticeTitle()
{

}

void TPracticeTitle::Move()
{

}
