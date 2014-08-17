#pragma once

#include "types.h"
#include <windows.h>

class TJoyPad 
{
private:
	JOYINFO 	JoyInfo;
	s8	 	FStickX,FStickY;
	bool	FButton[16];
	bool	JoyExist;
public:
	s8 	StickX() { return FStickX; }
	s8 	StickY() { return FStickY; }
	bool	getbutton(u16 bnum);
	TJoyPad();
	void Scan(u16 _port);
};


#define	PKey 1
#define	PJoy1 2

