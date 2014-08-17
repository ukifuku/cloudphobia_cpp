#pragma once

using namespace std;

#include <string>
#include <vector>
#include "types.h"

class TOBJ;
class TOBJList;

struct CommandData   //命令文格納構造体
{
    s32 CommandType;        //命令種別
    s32 CommandTiming;      //命令するタイミング
    s32 Relative;           //相対値
    s32 EndTiming;          //命令終了のタイミング
    s32 TimingType;         //タイミングの種別
    bool Done;               //処理終了か
    //パラメタ格納配列
    vector<s32> ParamInteger;
    vector<string> ParamString;
    vector<f32> ParamSingle;
    vector<bool> ParamBoolean;
};

typedef void (*TCommand)(TOBJList*, TOBJ*, CommandData&);

struct TCommandSet 
{
  TCommand 	Command;
  CommandData 	Data;
};

struct TProcessData
{
  TCommand	Command;
  string	name;
  s32 		numValue;
  string 	valueType;
};


//スクリプト制御エンジン
class TScriptEngine 
{
private:
    TOBJList*	FOwner;
    vector<TCommandSet> 	Commands;
    vector<TProcessData> 	Processes;
	void InitCommand( CommandData& _data );
	void RegisterProcess( TCommand _process, const string& _name, s32 numVal, const string& ValType );
	void Translate( s32 _num, const string& _str, CommandData& _data );
public:
    TScriptEngine(TOBJList* owner);
    ~TScriptEngine();
    void AddProcess(const string& _proc);
    void ExecuteProc(TOBJ* _obj);
    void UnDone();
    void Sort();
};

//スクリプトマネージャ
class TScriptManager 
{
private:
    TOBJList* FOwner;
    vector<TScriptEngine*> Process;
    vector<string> SourceFileNameList; //コンパイル対象のファイル名格納配列
public:
	TScriptManager(TOBJList* owner);
    ~TScriptManager();
    void AddScriptFile(const string& _filename);
    void CompileAll();
    void ExecuteProc(s32 _num, TOBJ* _obj);
    void UnDone(s32 _num);
};

#define		KTIMING_TYPE_AGE    0
#define		KTIMING_TYPE_OWNERDISTANCE   1

