#include "ScriptEngine.h"
#include "TOBJ.h"
#include "TOBJList.h"
#include "SXScene.h"
#include "Sound.h"
#include "QDArc.h"

void Kill( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	_obj->Die();
}

void TTutorial_MakeCursor( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	(TTutorial*)(_obj)->UpdateMessage(_Params.ParamString[0],
		_Params.ParamInteger[0],
		_Params.ParamInteger[1],
		_Params.ParamInteger[2],
		_Params.ParamInteger[3],
		_Params.ParamInteger[4]);

}

void TKeyBoard_Imput( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	(TTutorial*)(_obj)->KeyBoard.Imput(_Params.ParamInteger[0],
		_Params.ParamInteger[1],
		_Params.ParamBoolean[0],
		_Params.ParamBoolean[1],
		_Params.ParamBoolean[2]);

}

void ZakoGene( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	switch(_Params.ParamInteger[0])
	{
		case 0:_owner->Add(new TZ01Gene(_owner,_Params.ParamInteger[1],
		_Params.ParamInteger[2],
		_Params.ParamInteger[3],
		false));
			break;
		case 1:_owner->Add(new TZ02Gene(_owner,_Params.ParamInteger[1],
			_Params.ParamInteger[2],
			_Params.ParamInteger[3]));
			break;
		case 2:_owner->Add(new TZ02_6Gene(_owner,_Params.ParamInteger[1],
			_Params.ParamInteger[2],
			_Params.ParamInteger[3]));
			break;
		case 3:_owner->Add(new TStage4Guard(_owner,_Params.ParamInteger[1],
			_Params.ParamInteger[2]));
			break;
	};
}

void AttractionCursor( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	_owner->Add(new TAttractionCursor(_owner,_Params.ParamInteger[0],
		_Params.ParamInteger[1],
		_Params.ParamInteger[2],
		_Params.ParamInteger[3]));

}

void Mother_Tutorial( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	_owner->Add(new TMothership_Tutorial(_owner,_Params.ParamInteger[0],
		_Params.ParamInteger[1]));

}

void Zako2_01Gene( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	_owner->Add(new TZ02_1Gene(_owner,_Params.ParamInteger[0],
		_Params.ParamInteger[1],
		_Params.ParamInteger[2]));

}

void Zako2_02Gene( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	_owner->Add(new TZako2_02( _owner,_Params.ParamInteger[0],
		_Params.ParamInteger[1],
		_Params.ParamInteger[2],
		_Params.ParamInteger[3],
		_Params.ParamInteger[4],
		_Params.ParamInteger[5],
		_Params.ParamInteger[6],
		_Params.ParamBoolean[0]));

}

void Zako2_03Gene( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	_owner->Add(new TZako2_03(_owner,_Params.ParamInteger[0],
		_Params.ParamInteger[1],
		_Params.ParamInteger[2],
		_Params.ParamInteger[3],
		_Params.ParamInteger[4],
		_Params.ParamBoolean[0]));

}

void Zako2_04Gene( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	_owner->Add(new TZako2_04(_owner,_Params.ParamInteger[0],
		_Params.ParamInteger[1],
		_Params.ParamInteger[2],
		_Params.ParamBoolean[0]));


}

void Zako2_05Gene( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	_owner->Add(new TZ02_5Gene(_owner,_Params.ParamInteger[0],
		_Params.ParamInteger[1],
		_Params.ParamInteger[2],
		_Params.ParamInteger[3]));


}

void Zako2_06Gene( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	_owner->Add(new TZako2_06(_owner,_Params.ParamInteger[0],
		_Params.ParamInteger[1],
		_Params.ParamInteger[2],
		_Params.ParamInteger[3],
		_Params.ParamInteger[4],
		_Params.ParamBoolean[0]));


}

void Zako3_01Gene( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	_owner->Add(new TZako3_01(_owner,_Params.ParamInteger[0],
		_Params.ParamInteger[1],
		_Params.ParamInteger[2],
		_Params.ParamInteger[3],
		_Params.ParamInteger[4],
		_Params.ParamBoolean[0]));

}

void Boss2_1Gene( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	_owner->Add(new TStage2Boss1(_owner,_Params.ParamInteger[0],
		_Params.ParamInteger[1]));

}

void Boss2_2Gene( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	_owner->Add(new TStage2Boss2(_owner,_Params.ParamInteger[0],
		_Params.ParamInteger[1]));


}

void Boss3_1Gene( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	_owner->Add(new TStage3Boss(_owner,_Params.ParamInteger[0],
		_Params.ParamInteger[1]));

}

void WarningGauge( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	_owner->Add(new TWarningGauge2(_owner,_Params.ParamInteger[0],
		_Params.ParamInteger[1],
		_Params.ParamInteger[2],
		_Params.ParamInteger[3]));

}

void ShowFriend( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	if( _Params.ParamInteger[2] < 9 ){
		_owner->Add(new TFriend(_owner,_Params.ParamInteger[0],
		_Params.ParamInteger[1],
		_Params.ParamSingle[0],
		_Params.ParamInteger[2]));
	}
	else{
		_owner->Add(new TFriend2(_owner,_Params.ParamInteger[0],
			_Params.ParamInteger[1],
			_Params.ParamSingle[0],
			_Params.ParamInteger[2]));
	}
}

void SetDistance( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	_owner->Distance  =  _Params.ParamInteger[0];

}

void NightVision( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	_owner->Add(new TNightVision(_owner));

}

void NightVisionFade( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	(TNightVision*)(_obj)->Show(false);

}

void NightVisionWhite( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	(TNightVision*)(_obj)->MakeWhite();

}

void BossGene( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	switch(_Params.ParamInteger[0]){
		case 0:_owner->Add(new TStage2Boss1(_owner,_Params.ParamInteger[1],
		_Params.ParamInteger[2]));
			break;
		case 1:_owner->Add(new TStage2Boss2(_owner,_Params.ParamInteger[1],
			_Params.ParamInteger[2]));
			break;
		case 2:_owner->Add(new TStage3Boss(_owner,_Params.ParamInteger[1],
			_Params.ParamInteger[2]));
			break;
		case 3:_owner->Add(new TStage4Boss1(_owner,_Params.ParamInteger[1],
			_Params.ParamInteger[2]));
			break;
		case 4:_owner->Add(new TStage4Boss2Gene(_owner,_Params.ParamInteger[1],
			_Params.ParamInteger[2]));
			break;
		case 5:_owner->Add(new TStage5Boss1(_owner,_Params.ParamInteger[1],
			_Params.ParamInteger[2]));
			break;
		case 6:_owner->Add(new TStage5Boss2(_owner,_Params.ParamInteger[1],
			_Params.ParamInteger[2]));
			break;
		case 7:_owner->Add(new TStage5Boss3(_owner,_Params.ParamInteger[1],
			_Params.ParamInteger[2]));
			break;
	}

}

void SetClipNear( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	_owner->GetScene()->SetClipNear(_Params.ParamSingle[0]);

}

void SetBloomColor( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	_owner->SetBloomingColor(_Params.ParamInteger[0],
		_Params.ParamInteger[1],
		_Params.ParamInteger[2],
		_Params.ParamInteger[3],
		_Params.ParamInteger[4]);

}

void MakeSprite( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	(TDemoManager*)(_obj)->MakeSprite(_Params.ParamInteger[0],
		_Params.ParamInteger[1],
		_Params.ParamInteger[2],
		_Params.ParamInteger[3],
		_Params.ParamInteger[4],
		_Params.ParamInteger[5],
		_Params.ParamInteger[6],
		_Params.ParamInteger[7],
		_Params.ParamInteger[8],
		_Params.ParamInteger[9],
		_Params.ParamInteger[10],
		_Params.ParamInteger[11],
		_Params.ParamInteger[12],
		_Params.ParamInteger[13]
	);

}

void SprMove( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	(TDemoManager*)(_obj)->SprMove(_Params.ParamInteger[0],
		_Params.ParamSingle[0],
		_Params.ParamSingle[1]
	);

}

void SprMoveR( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	(TDemoManager*)(_obj)->SprMoveR(_Params.ParamInteger[0],
		_Params.ParamSingle[0],
		_Params.ParamSingle[1]
	);

}

void SprSlideMoveR( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	(TDemoManager*)(_obj)->SprSlideMoveR(_Params.ParamInteger[0],
		_Params.ParamSingle[0],
		_Params.ParamSingle[1]
	);

}

void SprRotate( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	(TDemoManager*)(_obj)->SprRotate(_Params.ParamInteger[0],
		_Params.ParamInteger[1]
	);

}

void SprRotateR( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	(TDemoManager*)(_obj)->SprRotateR(_Params.ParamInteger[0],
		_Params.ParamInteger[1]
	);

}

void SprZoom( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	(TDemoManager*)(_obj)->SprZoom(_Params.ParamInteger[0],
		_Params.ParamSingle[0],
		_Params.ParamSingle[1]
	);

}

void SprZoomR( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	(TDemoManager*)(_obj)->SprZoomR(_Params.ParamInteger[0],
		_Params.ParamSingle[0],
		_Params.ParamSingle[1]
	);

}

void SprColor( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	(TDemoManager*)(_obj)->SprColor(_Params.ParamInteger[0],
		_Params.ParamInteger[1]
	);

}

void SprColorR( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	(TDemoManager*)(_obj)->SprColorR(_Params.ParamInteger[0],
		_Params.ParamInteger[1]
	);

}

void SprAlpha( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	(TDemoManager*)(_obj)->SprAlpha(_Params.ParamInteger[0],
		_Params.ParamInteger[1]
	);

}

void SprAlphaR( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	(TDemoManager*)(_obj)->SprAlphaR(_Params.ParamInteger[0],
		_Params.ParamInteger[1]
	);

}

void Fade( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	_owner->Add(new TBoxContrl(_owner,_Params.ParamInteger[0]));

}

void SprRelease( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	(TDemoManager*)(_obj)->SprRelease(_Params.ParamInteger[0]);

}

void CameraMove( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	_owner->GetScene()->CameraMove(_Params.ParamSingle[0],_Params.ParamSingle[1],_Params.ParamSingle[2]);

}

void CameraRotate( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	_owner->GetScene()->CameraRotate(_Params.ParamInteger[0],_Params.ParamInteger[1],_Params.ParamInteger[2]);

}

void Quake( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	_owner->Quake(_Params.ParamInteger[0],
		_Params.ParamInteger[1]);

}

void WavPlay( TOBJList* _owner, TOBJ* _obj, CommandData& _Params )
{
	if( CreateSE ) _owner->SoundEffect->WavPlay2(_Params.ParamInteger[0],
		_Params.ParamInteger[1],
		_Params.ParamInteger[2]);

}

void TScriptEngine::RegisterProcess( TCommand _process, const string& _name, s32 numVal, const string& ValType )
{
	TProcessData newData;
	newData.Command  =  _process;
	newData.name = _name;
	newData.numValue = numVal;
	newData.valueType = ValType;
	Processes.push_back(newData);
}

void TScriptEngine::Translate( s32 _num, const string& _str, CommandData& _data )
{
	for( s32 i = 0 ; i < Processes[_num].numValue ; i++ ) {
		if( ReadSegment(Processes[_num].valueType,i) == "int" ) {
			_data.ParamInteger.push_back(stoi(ReadSegment(_str,5+i)));
		}
		else if( ReadSegment(Processes[_num].valueType,i) == "str" ) {
			_data.ParamString.push_back(ReadSegment(_str,5+i));
		}
		else if( ReadSegment(Processes[_num].valueType,i) == "single" ) {
			_data.ParamSingle.push_back(stof(ReadSegment(_str,5+i)));
		}
		else if( ReadSegment(Processes[_num].valueType,i) == "bool" ) {
			_data.ParamBoolean.push_back(stoi(ReadSegment(_str,5+i)) == 1);
		}
	}
}

void TScriptEngine::InitCommand( CommandData& _data )
{
	_data.CommandTiming = 0;
	_data.EndTiming = 0;
	_data.TimingType = 0;
	_data.Done = false;
	_data.Relative = 0;

	_data.ParamInteger.clear();
	_data.ParamString.clear();
	_data.ParamSingle.clear();
	_data.ParamBoolean.clear();
}

TScriptEngine::TScriptEngine( TOBJList* owner )
{
	FOwner = owner;
	Commands.clear();
	Processes.clear();

	RegisterProcess(Kill,"Kill",0,"");
	RegisterProcess(TKeyBoard_Imput,"TKeyBoard_Imput",5,"int,int,bool,bool,bool,");
	RegisterProcess(TTutorial_MakeCursor,"TTutorial_MakeCursor",6,"str,int,int,int,int,int,");
	RegisterProcess(ZakoGene,"ZakoGene",4,"int,int,int,int,");
	RegisterProcess(AttractionCursor,"AttractionCursor",4,"int,int,int,int,");
	RegisterProcess(Zako2_01Gene,"Zako2_01Gene",3,"int,int,int,");
	RegisterProcess(Zako2_02Gene,"Zako2_02Gene",8,"int,int,int,int,int,int,int,bool,");
	RegisterProcess(Zako2_03Gene,"Zako2_03Gene",6,"int,int,int,int,int,bool,");
	RegisterProcess(Zako2_04Gene,"Zako2_04Gene",4,"int,int,int,bool,");
	RegisterProcess(Zako2_05Gene,"Zako2_05Gene",4,"int,int,int,int,");
	RegisterProcess(Zako2_06Gene,"Zako2_06Gene",6,"int,int,int,int,int,bool,");
	RegisterProcess(Boss2_1Gene,"Boss2_1Gene",2,"int,int,");
	RegisterProcess(Boss2_2Gene,"Boss2_2Gene",2,"int,int,");
	RegisterProcess(WarningGauge,"WarningGauge",4,"int,int,int,int,");
	RegisterProcess(ShowFriend,"ShowFriend",4,"int,int,single,int,");
	RegisterProcess(SetDistance,"SetDistance",1,"int,");
	RegisterProcess(NightVision,"NightVision",0,"");
	RegisterProcess(NightVisionFade,"NightVisionFade",0,"");
	RegisterProcess(Zako3_01Gene,"Zako3_01Gene",6,"int,int,int,int,int,bool,");
	RegisterProcess(Boss3_1Gene,"Boss3_1Gene",2,"int,int,");
	RegisterProcess(BossGene,"BossGene",3,"int,int,int,");
	RegisterProcess(SetClipNear,"SetClipNear",1,"single,");
	RegisterProcess(SetBloomColor,"SetBloomColor",5,"int,int,int,int,int,");
	RegisterProcess(NightVisionWhite,"NightVisionWhite",0,"");

	RegisterProcess(MakeSprite,"MakeSprite",14,"int,int,int,int,int,int,int,int,int,int,int,int,int,int,");
	RegisterProcess(SprMove,"SprMove",3,"int, single, single,");
	RegisterProcess(SprMoveR,"SprMoveR",3,"int, single, single,");
	RegisterProcess(SprRotate,"SprRotate",2,"int, int, ");
	RegisterProcess(SprRotateR,"SprRotateR",2,"int, int,");
	RegisterProcess(SprZoom,"SprZoom",3,"int, single, single,");
	RegisterProcess(SprZoomR,"SprZoomR",3,"int, single, single,");
	RegisterProcess(SprColor,"SprColor",2,"int, int,");
	RegisterProcess(SprColorR,"SprColorR",2,"int, int,");
	RegisterProcess(SprAlpha,"SprAlpha",2,"int, int,");
	RegisterProcess(SprAlphaR,"SprAlphaR",2,"int, int,");
	RegisterProcess(Fade,"Fade",1,"int,");
	RegisterProcess(SprRelease,"SprRelease",1,"int,");
	RegisterProcess(SprSlideMoveR,"SprSlideMoveR",3,"int, single, single,");
	RegisterProcess(CameraMove,"CameraMove",3,"single, single, single,");
	RegisterProcess(CameraRotate,"CameraRotate",3,"int, int, int,");
	RegisterProcess(WavPlay,"WavPlay",3,"int, int, int,");
	RegisterProcess(Quake,"Quake",2,"int, int,");

}

TScriptEngine::~TScriptEngine()
{
	Commands.clear();
	Processes.clear();
}

void TScriptEngine::AddProcess( const string& _proc )
{
	//コメント行は飛ばす
	if( _proc.find_first_of("//") == 0 ) return;

	TCommandSet newSet;

	InitCommand(newSet.Data);

	//命令語に応じて識別コードを獲得する
	string WCode = ReadSegment(_proc,3);
	newSet.Data.Relative  =  stoi(ReadSegment(_proc,0));
	newSet.Data.CommandTiming = stoi(ReadSegment(_proc,1));
	newSet.Data.EndTiming = stoi(ReadSegment(_proc,2));
	newSet.Data.TimingType = stoi(ReadSegment(_proc,4));

	for( s32 i  =  0 ; i < Processes.size(); i++) {
		if( WCode == Processes[i].name ) {
			newSet.Command  =  Processes[i].Command;
			Translate(i, _proc, newSet.Data);
			break;
		}
	}
	Commands.push_back(newSet);
}

void TScriptEngine::ExecuteProc( TOBJ* _obj )
{
	if( Commands.empty() ) return;

	s32 tmpCount = 0;
	for( s32 i  =  0 ; i < Commands.size(); i++ ) {
		//実行タイミングの種別を選定
		switch( Commands[i].Data.TimingType ){
		case KTIMING_TYPE_AGE          : tmpCount  =  _obj->GetAge();		break;
		case KTIMING_TYPE_OWNERDISTANCE: tmpCount  =  FOwner->Distance;	break;
		}

		//まだ実行終了しておらず、指定されたタイミングに達した命令があれば実行
		if( (! Commands[i].Data.Done) && (Commands[i].Data.CommandTiming <= tmpCount) ) {
			Commands[i].Command(FOwner, _obj, Commands[i].Data);
			//命令終了タイミングに達していれば実行終了フラグを立てる
			if( (Commands[i].Data.CommandTiming + Commands[i].Data.EndTiming) <= tmpCount ) Commands[i].Data.Done = true;
		}
	}
}

void TScriptEngine::UnDone()
{
	if( Commands.empty() ) return;

	for( s32 i  =  0 ; i < Commands.size(); i++ ) {
		Commands[i].Data.Done  =  false;
	}

}

void TScriptEngine::Sort()
{
	if( Commands.empty() ) return;

	//まずは相対値が0<のものからソート
	for( s32 i  =  0 ; i < Commands.size(); i++ ) {
		//相対値がプラスなら、前の行の距離から自分の距離を計算する
		//前の行ですでに距離が決定されていることが前提
		if( Commands[i].Data.Relative>0 )
			Commands[i].Data.CommandTiming = Commands[i-Commands[i].Data.Relative].Data.CommandTiming + Commands[i].Data.CommandTiming;
	}

	//次に相対値マイナスのものをソート
	for( s32 i  =  0 ; i < Commands.size(); i++ ) {
		//相対値がマイナスのとき、次以降の行の距離から自分の距離を計算する
		//次以降の行ですでに距離が決定されていることが前提
		if( Commands[i].Data.Relative<0 )
			Commands[i].Data.CommandTiming = Commands[i-Commands[i].Data.Relative].Data.CommandTiming - Commands[i].Data.CommandTiming;
	}


}


TScriptManager::TScriptManager( TOBJList* owner )
{
	FOwner = owner;

	SourceFileNameList.clear();
	Process.clear();
}

TScriptManager::~TScriptManager()
{
	SourceFileNameList.clear();

	for( s32 i = 0 ; i < Process.size(); i++ ) {
		SAFE_DELETE(Process[i]);
	}
	Process.clear();

}

void TScriptManager::AddScriptFile( const string& _filename )
{
	SourceFileNameList.push_back(_filename);
}

void TScriptManager::CompileAll()
{
//LoadLine: String;
//LoadFile: TextFile;
//stream:TMemoryStream;
//tempstring:TStringList;
	for( s32 i = 0 ; i < Process.size(); i++ ) {
		SAFE_DELETE(Process[i]);
	}
	Process.clear();
	
	for( s32 i  =  0 ; i < SourceFileNameList.size(); i++ ) {
		TScriptEngine* newScr = new TScriptEngine(FOwner);

		if( DebugMode ) {
			// テキストファイル読み込み
			ifstream ifs(SourceFileNameList[i]);
			string LoadLine;
			while (std::getline(ifs, LoadLine)) {
				newScr->AddProcess(LoadLine);
			}

			Process.push_back(newScr);
		}
		else {
			//圧縮ファイルから読み込み
			stringstream str;
			ExtractFromQDAFile(str, "./Data/script.BIN", SourceFileNameList[i]);

			string LoadLine;
			while (std::getline(str, LoadLine)) {
				newScr->AddProcess(LoadLine);
			}
			Process.push_back(newScr);
		}
	
		newScr->Sort();
	}
}

void TScriptManager::ExecuteProc( s32 _num, TOBJ* _obj )
{
	Process[_num]->ExecuteProc(_obj);

}

void TScriptManager::UnDone( s32 _num )
{
	Process[_num]->UnDone;

}
