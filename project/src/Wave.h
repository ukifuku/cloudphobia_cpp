#pragma once

using namespace std;

#include <string>
#include "types.h"

struct WaveSet  //敵配置構造体
{
  s32	Relative[256];
  s32	EventCount;
  s32	EventMaxCount;
  s32	EventDistance[256];
  s32	EnemyType[256];  //出現させる敵の種類
  s32	X[256];
  s32	Y[256];        //出現させる座標
  s32	OPT1[256];
	s32 	OPT2[256];
	s32	OPT3[256];
  bool	FLG1[256];
  bool	FLG2[256];
};

void EWaveReset( WaveSet* WS );
void EWaveTrans( WaveSet* WS, s32 _num, const string& _line );
void EWaveSort( WaveSet* WS );
void EWaveLoad( WaveSet* WS, const string& _file );

