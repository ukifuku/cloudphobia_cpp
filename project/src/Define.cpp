#define _USE_MATH_DEFINES
#include <math.h>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include "Define.h"
#include "SpriteEX.h"
#include <stdlib.h>
#include <time.h>


void PosRoll( f32& _x, f32& _y, f32 _cx, f32 _cy, s32 _R )
{
	_R  =  _R & 0x0fff;

	// 角度0なら計算しない
	if(_R != 0 ) {
		_x  =  _x - _cx;
		_y  =  _y - _cy;
		f32 ax  =  _x * costable[_R] - _y * sintable[_R];
		f32 ay  =  _x * sintable[_R] + _y * costable[_R];
		_x  =  ax + _cx;
		_y  =  ay + _cy;
	}
}

std::string TimeToStr( s32 _time )
{
	return InttoStr(_time / 1000) + "." + InttoStr((_time / 100) - ( _time / 1000) * 10 ) + InttoStr((_time / 10) - (_time / 100)*10);
}

bool Equal( s32 _int, s32 _num )
{
	return (_int == _num );
}

void InitSprZ()
{
	KageExpZ	= 5000;
	BeamZ		= 2200;
	SmokeZ		= 3000;
	ExpZ		= 19500;
	ClExpZ		= 12100;
	TamaZ		= 3000;
	SplashZ		= 18000;
}

std::string InttoStr( s32 _val )
{
	std::stringstream ss;
	ss << _val;
	std::string str = ss.str(); 

	return str;
}

std::string ReadSegment( string _s, s32 _n )
{
	s32 i = 0;  //現在の文字番号
	s32 j = 0;  //見つかったカンマの数
	string segment = "";
	char temp = 0;
	bool yen = false;

	const char* c = _s.c_str();
	//無限ループ
	while( i >= 0 ) {
		//文字列を一文字ずつ走査
		temp  =  c[i];

		//カンマを探す
		if((! yen) && ((temp == ',') || (temp == '\n')) ) 
			j++;

		//取り出したい文節番号と、見つかったカンマの数が等しい間、文字を取り出す
		if((yen) || ((temp != ' ' ) && (temp != ',') && (temp != '\n') && (temp != '\\'))) {
			if( j == _n ) segment = segment+temp;
			yen = false;
		}

		if((! yen) && (temp == '\\') ) {
			yen = true;
		}

		//カンマの数が文節番号を超えたら終了
		if( j > _n ) break;
		i++;
	}

	return segment;
}

s32 GetDiffR( s32 _r1, s32 _r2 )
{
	s32 tempDiffR  =  (_r2 - _r1) & 0x0fff;

	//-2048～2047以内の角度を返す
	if( tempDiffR >= 2048 )
		tempDiffR  =  (4096 - tempDiffR) *(-1);

	return tempDiffR;
}

void PutDebugLog( string _msg )
{
	DebugLogArray.push_back(_msg);
}

void WriteDebugLog()
{
	if( DebugLogArray.empty() ) return;

	std::string line;
	std::ofstream ofs("cp_log.txt");

	for( u32 i = 0; i < DebugLogArray.size(); i++ ){
		ofs << DebugLogArray[i] << endl;
	}
	ofs.close();
}

void InitDebugLog()
{
	DebugLogArray.clear();
	LastMovedOBJ = "";
}

bool CheckDistance( s32 X1, s32 Y1, s32 X2, s32 Y2, s32 dist )
{
	f32 DX = (f32)(X1-X2);
	f32 DY = (f32)(Y1-Y2);
	f32 currentdistance = pow( DX*DX + DY*DY, 0.5f );

	return currentdistance <= (f32)dist;
}

s32 GetRfromArcTan( f32 _x, f32 _y, f32 _dx, f32 _dy )
{
	f32 DiffX = _dx - _x;
	f32 DiffY = _dy - _y;
	return RoundOff(atan2(DiffY,DiffX) / (f32)M_PI * 2048.f);
}

glm::vec2 GetReflectionVector( glm::vec2& _vec, THitArea* _hitarea )
{
	f32 maxCos = -1;
	glm::vec2 NearestLine( 1.f, 0.f );

	//当たり判定の四角形のうち、_vecに最も近い辺を求める
	for( s32 i = 0; i < 4; i++ ) {
		glm::vec2 v1  =  _vec - glm::vec2(_hitarea[0].RPointAry[i].x, _hitarea[0].RPointAry[i].y);
		glm::vec2 v2  =  glm::vec2(_hitarea[0].RPointAry[(i + 1) & 3].x, _hitarea[0].RPointAry[(i + 1) & 3].y)
			- glm::vec2(_hitarea[0].RPointAry[i].x, _hitarea[0].RPointAry[i].y);

		//ベクトルの内積を長さで割ってcosθを算出
		f32 nowCos = glm::dot(v1,v2) / (glm::length(v1) * (glm::length(v2)));

		//cosθが最も大きい→角度が最も小さい→最もvecに近い辺
		if(nowCos >= maxCos ) {
			maxCos = nowCos;
			NearestLine = v2;
		}
	}

	//90度回転させて辺の法線とする
	return glm::normalize(Rot2D(NearestLine, 1024));
}

string GetMacAddress()
{
	//TAstat Adapter;
	//TNCB Ncb;
	//TLanaEnum lenum;
	//FillChar(Ncb, SizeOf(NCB), 0);
	//Ncb.ncb_command  =  Char(NCBENUM);
	//Ncb.ncb_buffer  =  &lenum;
	//Ncb.ncb_length  =  SizeOf(lenum);
	//s32 RetCode  =  (s32)(Netbios( &Ncb ));

	//for( s32 i = 0; i < lenum.length; i++ ) {
	//	FillChar(Ncb, SizeOf(NCB), 0);
	//	Ncb.ncb_command  =  Char(NCBRESET);
	//	Ncb.ncb_lana_num  =  lenum.lana[i];
	//	RetCode  =  (s32)(Netbios( &Ncb ));

	//	FillChar(Ncb, SizeOf(NCB), 0);
	//	Ncb.ncb_command  =  Char(NCBASTAT);
	//	Ncb.ncb_lana_num  =  lenum.lana[i];
	//	Ncb.ncb_callname  =  "* ";
	//	Ncb.ncb_buffer  =  &Adapter;
	//	Ncb.ncb_length  =  SizeOf(Adapter);

	//	RetCode  =  (s32)(Netbios( &Ncb ));

	//	if(RetCode == 0 ) {
	//		return IntToHex(integer(Adapter.adapt.adapter_address[0]), 2)
	//			+ IntToHex(integer(Adapter.adapt.adapter_address[1]), 2)
	//			+ IntToHex(integer(Adapter.adapt.adapter_address[2]), 2)
	//			+ IntToHex(integer(Adapter.adapt.adapter_address[3]), 2)
	//			+ IntToHex(integer(Adapter.adapt.adapter_address[4]), 2)
	//			+ IntToHex(integer(Adapter.adapt.adapter_address[5]),
	//	}
	//}

	return "";
}

string TotalTimeToStr( s32 _time )
{
	s32 Total;
	if(_time == 0 )
		Total = 0;
	else
		Total = 180000 * 5 - _time;

	s32 min  =  Total / 60000;
	s32 sec  =  (Total / 1000) % 60;
	s32 ms   =  (Total % 1000) / 10;

	char tmp[8];
	sprintf_s( tmp, "%2d:%2d:%2d", min, sec, ms );

	string result(tmp);

	return result;
}

glm::vec2 Rot2D( glm::vec2& _vec, s32 _r )
{
	_r &= 0x0fff;

	glm::vec2 result;

	result.x = _vec.x * costable[_r] - _vec.y * sintable[_r];
	result.y = _vec.x * sintable[_r] + _vec.y * costable[_r];

	return result;
}

s32 Sign( s32 val )
{
	if( val > 0 )
	{
		return 1;
	}
	else if( val == 0)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

u32 random(u32 _max)
{
	return rand() % _max;
}

void GetDate( s32& _year, s32& _mon, s32& _day )
{
	time_t timer;
	struct tm local;
	timer = time(NULL);

	localtime_s(&local, &timer); 

	_year = local.tm_year + 1900;
	_mon = local.tm_mon + 1;
	_day = local.tm_mday;
}

s32 BoolToInt( bool _val )
{
	return _val ? 1 : 0;
}

bool IntToBool( s32 _val )
{
	return _val == 0 ? false : true;
}
