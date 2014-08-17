#pragma once

using namespace std;

#include <string>
#include <vector>
#include "types.h"

class TOBJ;
class TOBJList;

struct CommandData   //���ߕ��i�[�\����
{
    s32 CommandType;        //���ߎ��
    s32 CommandTiming;      //���߂���^�C�~���O
    s32 Relative;           //���Βl
    s32 EndTiming;          //���ߏI���̃^�C�~���O
    s32 TimingType;         //�^�C�~���O�̎��
    bool Done;               //�����I����
    //�p�����^�i�[�z��
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


//�X�N���v�g����G���W��
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

//�X�N���v�g�}�l�[�W��
class TScriptManager 
{
private:
    TOBJList* FOwner;
    vector<TScriptEngine*> Process;
    vector<string> SourceFileNameList; //�R���p�C���Ώۂ̃t�@�C�����i�[�z��
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

