#pragma once

using namespace std;

#include <string>
#include <sstream>
#include "types.h"

class TOBJ;
class TOBJList;

class TPostThread {
private:
    TOBJ* FOwner;
    stringstream FResponse;
    stringstream FmemStream;
    string FUrl;
public:
    TPostThread(TOBJ* _owner, const string& _url, stringstream& _memstream);
    void Execute(); 
    void ConnectEnd();
};

class TGetThread {
private:
	TOBJ* FOwner;
	stringstream FResponse;
	string FUrl;
public:
    TGetThread(TOBJ* _owner, const string& _url);
    void Execute();
    void ConnectEnd();
};

class  TLoadStageThread {
private:
    TOBJList* FOwner;
    s32 StageNum;
public:
    TLoadStageThread(TOBJList* _owner, s32 _stage);
    void Execute();
};
