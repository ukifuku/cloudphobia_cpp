#include "JoyScan.h"

bool TJoyPad::getbutton( u16 bnum )
{
	return FButton[bnum];
}

TJoyPad::TJoyPad()
{
	FStickX = 0;
	FStickY = 0;
	for( s32 i = 0 ; i <= 15; i++ )
		FButton[i] = false;

	if(joyGetPos(JOYSTICKID1, &JoyInfo) == JOYERR_NOERROR ) 
		JoyExist = true;
	else 
		JoyExist = false;

}

void TJoyPad::Scan( u16 _port )
{
	FStickX = 0;
	FStickY = 0;
	for( s32 i = 0 ; i <= 15; i++ )
		FButton[i] = false;

	switch(_port){
	case PKey:
		{
			if( (GetKeyState(VK_UP) < 0) ) FStickY = -1;
			else if( (GetKeyState(VK_DOWN) < 0) ) FStickY = 1;

			if( (GetKeyState(VK_LEFT) < 0) ) FStickX = -1;
			else if( (GetKeyState(VK_RIGHT) < 0) ) FStickX = 1;

			if( (GetKeyState('Z') < 0) ) FButton[0] = true;
			if( (GetKeyState('X') < 0) ) FButton[1] = true;
			if( (GetKeyState('C') < 0) ) FButton[2] = true;
			if( (GetKeyState('A') < 0) ) FButton[3] = true;
			if( (GetKeyState('S') < 0) ) FButton[4] = true;
			if( (GetKeyState('D') < 0) ) FButton[5] = true;
			if( (GetKeyState('Q') < 0) ) FButton[6] = true;
			if( (GetKeyState('W') < 0) ) FButton[7] = true;
			if( (GetKeyState('E') < 0) ) FButton[8] = true;
			if( (GetKeyState('1') < 0) ) FButton[9] = true;
			if( (GetKeyState('2') < 0) ) FButton[10] = true;
			if( (GetKeyState('3') < 0) ) FButton[11] = true;
			if( (GetKeyState('4') < 0) ) FButton[12] = true;
			if( (GetKeyState('5') < 0) ) FButton[13] = true;
			if( (GetKeyState('6') < 0) ) FButton[14] = true;
			if( (GetKeyState('7') < 0) ) FButton[15] = true;
		}break;
	case PJoy1:
		{
			if( JoyExist ) {
				joyGetPos(JOYSTICKID1, &JoyInfo);
				if( JoyInfo.wYpos < (65535*0.3) ) FStickY = -1;
				if( JoyInfo.wYpos > (65535*0.7) ) FStickY = 1;
				if( JoyInfo.wXpos < (65535*0.3) ) FStickX = -1;
				if( JoyInfo.wXpos > (65535*0.7) ) FStickX = 1;

				if( JoyInfo.wButtons && JOY_BUTTON1 !=0 ) FButton[0] = true;
				if( JoyInfo.wButtons && JOY_BUTTON2 !=0 ) FButton[1] = true;
				if( JoyInfo.wButtons && JOY_BUTTON3 !=0 ) FButton[2] = true;
				if( JoyInfo.wButtons && JOY_BUTTON4 !=0 ) FButton[3] = true;
				if( JoyInfo.wButtons && JOY_BUTTON5 !=0 ) FButton[4] = true;
				if( JoyInfo.wButtons && JOY_BUTTON6 !=0 ) FButton[5] = true;
				if( JoyInfo.wButtons && JOY_BUTTON7 !=0 ) FButton[6] = true;
				if( JoyInfo.wButtons && JOY_BUTTON8 !=0 ) FButton[7] = true;
				if( JoyInfo.wButtons && JOY_BUTTON9 !=0 ) FButton[8] = true;
				if( JoyInfo.wButtons && JOY_BUTTON10 !=0 ) FButton[9] = true;
				if( JoyInfo.wButtons && JOY_BUTTON11 !=0 ) FButton[10] = true;
				if( JoyInfo.wButtons && JOY_BUTTON12 !=0 ) FButton[11] = true;
				if( JoyInfo.wButtons && JOY_BUTTON13 !=0 ) FButton[12] = true;
				if( JoyInfo.wButtons && JOY_BUTTON14 !=0 ) FButton[13] = true;
				if( JoyInfo.wButtons && JOY_BUTTON15 !=0 ) FButton[14] = true;
				if( JoyInfo.wButtons && JOY_BUTTON16 !=0 ) FButton[15] = true;
			}
		}break;
	case PKey+PJoy1:
		{
			if( (GetKeyState(VK_UP) < 0) ) FStickY = -1;
			else if( (GetKeyState(VK_DOWN) < 0) ) FStickY = 1;

			if( (GetKeyState(VK_LEFT) < 0) ) FStickX = -1;
			else if( (GetKeyState(VK_RIGHT) < 0) ) FStickX = 1;

			if( (GetKeyState('Z') < 0) ) FButton[0] = true;
			if( (GetKeyState('X') < 0) ) FButton[1] = true;
			if( (GetKeyState('C') < 0) ) FButton[2] = true;
			if( (GetKeyState('A') < 0) ) FButton[3] = true;
			if( (GetKeyState('S') < 0) ) FButton[4] = true;
			if( (GetKeyState('D') < 0) ) FButton[5] = true;
			if( (GetKeyState('Q') < 0) ) FButton[6] = true;
			if( (GetKeyState('W') < 0) ) FButton[7] = true;
			if( (GetKeyState('E') < 0) ) FButton[8] = true;
			if( (GetKeyState('1') < 0) ) FButton[9] = true;
			if( (GetKeyState('2') < 0) ) FButton[10] = true;
			if( (GetKeyState('3') < 0) ) FButton[11] = true;
			if( (GetKeyState('4') < 0) ) FButton[12] = true;
			if( (GetKeyState('5') < 0) ) FButton[13] = true;
			if( (GetKeyState('6') < 0) ) FButton[14] = true;
			if( (GetKeyState('7') < 0) ) FButton[15] = true;

			if( JoyExist ) {
				joyGetPos(JOYSTICKID1, &JoyInfo);
				if( JoyInfo.wYpos < (65535*0.3) ) FStickY = -1;
				if( JoyInfo.wYpos > (65535*0.7) ) FStickY = 1;
				if( JoyInfo.wXpos < (65535*0.3) ) FStickX = -1;
				if( JoyInfo.wXpos > (65535*0.7) ) FStickX = 1;

				if( JoyInfo.wButtons && JOY_BUTTON1 !=0 ) FButton[0] = true;
				if( JoyInfo.wButtons && JOY_BUTTON2 !=0 ) FButton[1] = true;
				if( JoyInfo.wButtons && JOY_BUTTON3 !=0 ) FButton[2] = true;
				if( JoyInfo.wButtons && JOY_BUTTON4 !=0 ) FButton[3] = true;
				if( JoyInfo.wButtons && JOY_BUTTON5 !=0 ) FButton[4] = true;
				if( JoyInfo.wButtons && JOY_BUTTON6 !=0 ) FButton[5] = true;
				if( JoyInfo.wButtons && JOY_BUTTON7 !=0 ) FButton[6] = true;
				if( JoyInfo.wButtons && JOY_BUTTON8 !=0 ) FButton[7] = true;
				if( JoyInfo.wButtons && JOY_BUTTON9 !=0 ) FButton[8] = true;
				if( JoyInfo.wButtons && JOY_BUTTON10 !=0 ) FButton[9] = true;
				if( JoyInfo.wButtons && JOY_BUTTON11 !=0 ) FButton[10] = true;
				if( JoyInfo.wButtons && JOY_BUTTON12 !=0 ) FButton[11] = true;
				if( JoyInfo.wButtons && JOY_BUTTON13 !=0 ) FButton[12] = true;
				if( JoyInfo.wButtons && JOY_BUTTON14 !=0 ) FButton[13] = true;
				if( JoyInfo.wButtons && JOY_BUTTON15 !=0 ) FButton[14] = true;
				if( JoyInfo.wButtons && JOY_BUTTON16 !=0 ) FButton[15] = true;
			}
		}break;
	}

}

