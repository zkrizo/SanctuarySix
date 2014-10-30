#include "UserInput.h"
#include <Outland\Singletons.h>
#include "FunctionList.h"


extern int RES_WIDTH;
extern int RES_HEIGHT;

/****************************
* Constructor
****************************/
UserInput::UserInput()
{
	//curUI=STATE_INTRO;

	curUI=STATE_GAME;
	fList=new FunctionList;
	mouseLeft=FALSE;
}

/****************************
* Destructor
****************************/
UserInput::~UserInput()
{
	delete fList;
}

/****************************
* Update
****************************/
void UserInput::Update()
{
	blockChange=FALSE;

	//checks each of the keys that are down
	std::forward_list<int>::iterator iterator;
	iterator=keysDown.begin();


	if(!keysDown.empty())
	{
		//continues until the end of the list of keys held down
		while(iterator!=keysDown.end())
		{
			switch(curUI)
			{
			case STATE_INTRO:
				
				break;
			case STATE_MAINMENU:

				break;
			case STATE_GAME:
				//I'd like to substitute an if-else cluster with a switch statement, but need to figure out how to do variables with it.
				if(fList->getKey(F_ATTACK)==*iterator)
				{
					fList->onFunction(F_ATTACK);
					fList->f[F_ATTACK]=true;
				}
				else if(fList->getKey(F_JUMP)==*iterator)
				{
					fList->onFunction(F_JUMP);
					fList->f[F_JUMP]=true;
				}
				else if(fList->getKey(F_OPENINV)==*iterator)
				{
					fList->onFunction(F_OPENINV);
					fList->f[F_OPENINV]=true;
				}
				else if(fList->getKey(F_OPENTECH)==*iterator)
				{
					fList->onFunction(F_OPENTECH);
					fList->f[F_OPENTECH]=true;
				}
				else if(fList->getKey(F_OPENBUILD)==*iterator)
				{
					fList->onFunction(F_OPENBUILD);
					fList->f[F_OPENBUILD]=true;
				}
				else if(fList->getKey(F_MOVELEFT)==*iterator)
				{
					fList->onFunction(F_MOVELEFT);
					uiCore->UI_Camera.hasMoved(TRUE);
				}
				else if(fList->getKey(F_MOVERIGHT)==*iterator)
				{
					fList->onFunction(F_MOVERIGHT);
					uiCore->UI_Camera.hasMoved(TRUE);
				}
				else if(fList->getKey(F_MOVEFORWARD)==*iterator)
				{
					fList->onFunction(F_MOVEFORWARD);
					uiCore->UI_Camera.hasMoved(TRUE);
				}
				else if(fList->getKey(F_MOVEBACK)==*iterator)
				{
					fList->onFunction(F_MOVEBACK);
					uiCore->UI_Camera.hasMoved(TRUE);
				}
				else if(fList->getKey(F_MENU)==*iterator)
				{
					fList->onFunction(F_MENU);
					fList->f[F_MENU]=true;
				}
				else if(fList->getKey(F_BUILDXP)==*iterator)
				{
					fList->onFunction(F_BUILDXP);
					fList->f[F_BUILDXP]=true;
				}
				else if(fList->getKey(F_BUILDXM)==*iterator)
				{
					fList->onFunction(F_BUILDXM);
					fList->f[F_BUILDXM]=true;
				}
				else if(fList->getKey(F_BUILDZP)==*iterator)
				{
					fList->onFunction(F_BUILDZP);
					fList->f[F_BUILDZP]=true;
				}
				else if(fList->getKey(F_BUILDZM)==*iterator)
				{
					fList->onFunction(F_BUILDZM);
					fList->f[F_BUILDZM]=true;
				}
				else if(fList->getKey(F_BUILDYP)==*iterator)
				{
					fList->onFunction(F_BUILDYP);
				}
				else if(fList->getKey(F_BUILDYM)==*iterator)
				{
					fList->onFunction(F_BUILDYM);
				}
				else if(KEY_NUMPLUS==*iterator)
				{
					uiCore->UI_Camera.moveCam(0,0.01f,0);
					uiCore->UI_Camera.hasMoved(TRUE);
				}
				else if(KEY_NUMMINUS==*iterator)
				{
					uiCore->UI_Camera.moveCam(0,-0.01f,0);
					uiCore->UI_Camera.hasMoved(TRUE);
				}
				else if(fList->getKey(F_ENTER)==*iterator)
				{
					fList->onFunction(F_ENTER);
					fList->f[F_ENTER]=true;
				}
				break;
			case STATE_INVENTORY:

				break;
			case STATE_GAMEMENU:

				break;
			case STATE_FIRSTPERSON:

				break;
			}

			iterator++;
		};
	}


	//handle unique case of left mouse click
	if(mouseLeft)
	{	
		switch(uiCore->getState())
		{
		case STATE_MAINMENU:
			uiCore->UI_MMenu.clicked((real32)mouseX,(real32)mouseY);
			break;
		case STATE_INVENTORY:

			break;
		default:
			uiCore->UI_GUI.clicked((s32)mouseX,(s32)mouseY);
			break;
		}

		mouseLeft=FALSE;
	}
}

/****************************
* MouseMove
****************************/
void UserInput::MouseMove(int x, int y)
{
	if(isPressed(KEY_MOUSERIGHT))
	{
		uiCore->rotateCam(0,0.25f*(float)(x-mouseX),0.25f*(float)(y-mouseY));
		uiCore->UI_Camera.hasMoved(TRUE);
	}
	if(isPressed(KEY_MOUSELEFT))
	{
		if(uiCore->getState()!=STATE_MAINMENU)
			uiCore->UI_Camera.rayTrace(x,(RES_HEIGHT-30)-y);
	}
	mouseX=x;
	mouseY=y;
}

/****************************
* KeyPress
****************************/
void UserInput::KeyPress(bool value, int key)
{
	switch(key)
	{
	case 0:   //Key 'A'
		if(value)
			keysDown.push_front(KEY_A);
		else
			keysDown.remove(KEY_A);
		break;
	case 1:   //Key 'B'
		if(value)
			keysDown.push_front(KEY_B);
		else
		{
			keysDown.remove(KEY_B);
			fList->f[F_OPENBUILD]=false;
		}
		break;
	case 2:   //Key 'C'
		if(value)
			keysDown.push_front(KEY_C);
		else
			keysDown.remove(KEY_C);
		break;
	case 3:   //Key 'D'
		if(value)
			keysDown.push_front(KEY_D);
		else
			keysDown.remove(KEY_D);
		break;
	case 4:   //Key 'E'
		if(value)
			keysDown.push_front(KEY_E);
		else
			keysDown.remove(KEY_E);
		break;
	case 5:   //Key 'F'
		if(value)
			keysDown.push_front(KEY_F);
		else
			keysDown.remove(KEY_F);
		break;
	case 6:   //Key 'G'
		if(value)
			keysDown.push_front(KEY_G);
		else
			keysDown.remove(KEY_G);
		break;
	case 7:   //Key 'H'
		if(value)
			keysDown.push_front(KEY_H);
		else
			keysDown.remove(KEY_H);
		break;
	case 8:   //Key 'I'
		if(value)
			keysDown.push_front(KEY_I);
		else
		{
			keysDown.remove(KEY_I);
			fList->f[F_OPENINV]=false;
		}
		break;
	case 9:   //Key 'J'
		if(value)
			keysDown.push_front(KEY_J);
		else
			keysDown.remove(KEY_J);
		break;
	case 10:   //Key 'K'
		if(value)
			keysDown.push_front(KEY_K);
		else
			keysDown.remove(KEY_K);
		break;
	case 11:   //Key 'L'
		if(value)
			keysDown.push_front(KEY_L);
		else
			keysDown.remove(KEY_L);
		break;
	case 12:   //Key 'M'
		if(value)
			keysDown.push_front(KEY_M);
		else
			keysDown.remove(KEY_M);
		break;
	case 13:   //Key 'N'
		if(value)
			keysDown.push_front(KEY_N);
		else
			keysDown.remove(KEY_N);
		break;
	case 14:   //Key 'O'
		if(value)
			keysDown.push_front(KEY_O);
		else
			keysDown.remove(KEY_O);
		break;
	case 15:   //Key 'P'
		if(value)
			keysDown.push_front(KEY_P);
		else
			keysDown.remove(KEY_P);
		break;
	case 16:   //Key 'Q'
		if(value)
			keysDown.push_front(KEY_Q);
		else
			keysDown.remove(KEY_Q);
		break;
	case 17:   //Key 'R'
		if(value)
			keysDown.push_front(KEY_R);
		else
			keysDown.remove(KEY_R);
		break;
	case 18:   //Key 'S'
		if(value)
			keysDown.push_front(KEY_S);
		else
			keysDown.remove(KEY_S);
		break;
	case 19:   //Key 'T'
		if(value)
			keysDown.push_front(KEY_T);
		else
		{
			keysDown.remove(KEY_T);
			fList->f[F_OPENTECH]=false;
		}
		break;
	case 20:   //Key 'U'
		if(value)
			keysDown.push_front(KEY_U);
		else
			keysDown.remove(KEY_U);
		break;
	case 21:   //Key 'V'
		if(value)
			keysDown.push_front(KEY_V);
		else
			keysDown.remove(KEY_V);
		break;
	case 22:   //Key 'W'
		if(value)
			keysDown.push_front(KEY_W);
		else
			keysDown.remove(KEY_W);
		break;
	case 23:   //Key 'X'
		if(value)
			keysDown.push_front(KEY_X);
		else
			keysDown.remove(KEY_X);
		break;
	case 24:   //Key 'Y'
		if(value)
			keysDown.push_front(KEY_Y);
		else
			keysDown.remove(KEY_Y);
		break;
	case 25:   //Key 'Z'
		if(value)
			keysDown.push_front(KEY_Z);
		else
			keysDown.remove(KEY_Z);
		break;
	case 26:   //Key '0'
		if(value)
			keysDown.push_front(KEY_0);
		else
			keysDown.remove(KEY_0);
		break;
	case 27:   //Key '1'
		if(value)
			keysDown.push_front(KEY_1);
		else
			keysDown.remove(KEY_1);
		break;
	case 28:   //Key '2'
		if(value)
			keysDown.push_front(KEY_2);
		else
			keysDown.remove(KEY_2);
		break;
	case 29:   //Key '3'
		if(value)
			keysDown.push_front(KEY_3);
		else
			keysDown.remove(KEY_3);
		break;
	case 30:   //Key '4'
		if(value)
			keysDown.push_front(KEY_4);
		else
			keysDown.remove(KEY_4);
		break;
	case 31:   //Key '5'
		if(value)
			keysDown.push_front(KEY_5);
		else
			keysDown.remove(KEY_5);
		break;
	case 32:   //Key '6'
		if(value)
			keysDown.push_front(KEY_6);
		else
			keysDown.remove(KEY_6);
		break;
	case 33:   //Key '7'
		if(value)
			keysDown.push_front(KEY_7);
		else
			keysDown.remove(KEY_7);
		break;
	case 34:   //Key '8'
		if(value)
			keysDown.push_front(KEY_8);
		else
			keysDown.remove(KEY_8);
		break;
	case 35:   //Key '9'
		if(value)
			keysDown.push_front(KEY_9);
		else
			keysDown.remove(KEY_9);
		break;
	case 36:   //Key 'MINUS'
		if(value)
			keysDown.push_front(KEY_MINUS);
		else
			keysDown.remove(KEY_MINUS);
		break;
	case 37:   //Key 'PLUS'
		if(value)
			keysDown.push_front(KEY_PLUS);
		else
			keysDown.remove(KEY_PLUS);
		break;
	case 38:   //Key 'BACKSPACE'
		if(value)
			keysDown.push_front(KEY_BACKSPACE);
		else
			keysDown.remove(KEY_BACKSPACE);		
		break;
	case 39:   //Key 'TILDA'
		if(value)
			keysDown.push_front(KEY_TILDA);
		else
			keysDown.remove(KEY_TILDA);
		break;
	case 40:   //Key 'TAB'
		if(value)
			keysDown.push_front(KEY_TAB);
		else
			keysDown.remove(KEY_TAB);
		break;
	case 41:   //Key 'CAPS'
		if(value)
			keysDown.push_front(KEY_CAPS);
		else
			keysDown.remove(KEY_CAPS);
		break;
	case 42:   //Key 'SHIFT'
		if(value)
			keysDown.push_front(KEY_SHIFT);
		else
			keysDown.remove(KEY_SHIFT);
		break;
	case 43:   //Key 'CTRL'
		if(value)
			keysDown.push_front(KEY_CTRL);
		else
			keysDown.remove(KEY_CTRL);
		break;
	case 44:   //Key 'ALT'
		if(value)
			keysDown.push_front(KEY_ALT);
		else
			keysDown.remove(KEY_ALT);
		break;
	case 45:   //Key 'ENTER'
		if(value)
			keysDown.push_front(KEY_ENTER);
		else
		{
			keysDown.remove(KEY_ENTER);
			fList->f[F_ENTER]=false;
		}
		break;
	case 46:   //Key 'SPACE'
		if(value)
			keysDown.push_front(KEY_SPACE);
		else
			keysDown.remove(KEY_SPACE);
		break;
	case 47:   //Key 'UPARROW'
		if(value)
			keysDown.push_front(KEY_UPARROW);
		else
		{
			keysDown.remove(KEY_UPARROW);
			fList->f[F_BUILDZM]=false;
		}
		break;
	case 48:   //Key 'DOWNARROW'
		if(value)
			keysDown.push_front(KEY_DOWNARROW);
		else
		{
			keysDown.remove(KEY_DOWNARROW);
			fList->f[F_BUILDZP]=false;
		}
		break;
	case 49:   //Key 'LEFTARROW'
		if(value)
			keysDown.push_front(KEY_LEFTARROW);
		else
		{
			keysDown.remove(KEY_LEFTARROW);
			fList->f[F_BUILDXM]=false;
		}
		break;
	case 50:	//Key 'RIGHTARROW
		if(value)
			keysDown.push_front(KEY_RIGHTARROW);
		else
		{
			keysDown.remove(KEY_RIGHTARROW);
			fList->f[F_BUILDXP]=false;
		}
		break;
	case 51:	//Key 'ESC'
		if(value)
			keysDown.push_front(KEY_ESC);
		else
		{
			keysDown.remove(KEY_ESC);
			fList->f[F_MENU]=false;
		}
		break;
	case 52:	//Key 'F1'
		if(value)
			keysDown.push_front(KEY_F1);
		else
			keysDown.remove(KEY_F1);
		break;
	case 53:	//Key 'F2'
		if(value)
			keysDown.push_front(KEY_F2);
		else
			keysDown.remove(KEY_F2);
		break;
	case 54:	//Key 'F3'
		if(value)
			keysDown.push_front(KEY_F3);
		else
			keysDown.remove(KEY_F3);
		break;
	case 55:	//Key 'F4'
		if(value)
			keysDown.push_front(KEY_F4);
		else
			keysDown.remove(KEY_F4);
		break;
	case 56:	//Key 'F5'
		if(value)
			keysDown.push_front(KEY_F5);
		else
			keysDown.remove(KEY_F5);
		break;
	case 57:	//Key 'F6'
		if(value)
			keysDown.push_front(KEY_F6);
		else
			keysDown.remove(KEY_F6);
		break;
	case 58:	//Key 'F7'
		if(value)
			keysDown.push_front(KEY_F7);
		else
			keysDown.remove(KEY_F7);
		break;
	case 59:	//Key 'F8'
		if(value)
			keysDown.push_front(KEY_F8);
		else
			keysDown.remove(KEY_F8);
		break;
	case 60:	//Key 'F9'
		if(value)
			keysDown.push_front(KEY_F9);
		else
			keysDown.remove(KEY_F9);
		break;
	case 61:	//Key 'F10'
		if(value)
			keysDown.push_front(KEY_F10);
		else
			keysDown.remove(KEY_F10);
		break;
	case 62:	//Key 'F11'
		if(value)
			keysDown.push_front(KEY_F11);
		else
			keysDown.remove(KEY_F11);
		break;
	case 63:	//Key 'F12'
		if(value)
			keysDown.push_front(KEY_F12);
		else
			keysDown.remove(KEY_F12);
		break;
	case 64:	//Key 'Numlock'
		if(value)
			keysDown.push_front(KEY_NUMLOCK);
		else
			keysDown.remove(KEY_NUMLOCK);
		break;
	case 65:	//Key 'Numpad slash'
		if(value)
			keysDown.push_front(KEY_NUMSLASH);
		else
			keysDown.remove(KEY_NUMSLASH);
		break;
	case 66:	//Key 'Numpad *'
		if(value)
			keysDown.push_front(KEY_NUMMULTIPLY);
		else
			keysDown.remove(KEY_NUMMULTIPLY);
		break;
	case 67:	//Key 'Numpad Minus'
		if(value)
			keysDown.push_front(KEY_NUMMINUS);
		else
			keysDown.remove(KEY_NUMMINUS);
		break;
	case 68:	//Key 'Numpad plus'
		if(value)
			keysDown.push_front(KEY_NUMPLUS);
		else
			keysDown.remove(KEY_NUMPLUS);
		break;
	case 69:	//Key 'Numpad enter'
		if(value)
			keysDown.push_front(KEY_NUMENTER);
		else
			keysDown.remove(KEY_NUMENTER);
		break;
	case 70:	//Key 'Numpad period'
		if(value)
			keysDown.push_front(KEY_NUMPERIOD);
		else
			keysDown.remove(KEY_NUMPERIOD);
		break;
	case 71:	//Key 'Numpad 0'
		if(value)
			keysDown.push_front(KEY_NUM0);
		else
			keysDown.remove(KEY_NUM0);
		break;
	case 72:	//Key 'Numpad 1'
		if(value)
			keysDown.push_front(KEY_NUM1);
		else
			keysDown.remove(KEY_NUM1);
		break;
	case 73:	//Key 'Numpad 2'
		if(value)
			keysDown.push_front(KEY_NUM2);
		else
			keysDown.remove(KEY_NUM2);
		break;
	case 74:	//Key 'Numpad 3'
		if(value)
			keysDown.push_front(KEY_NUM3);
		else
			keysDown.remove(KEY_NUM3);
		break;
	case 75:	//Key 'Numpad 4'
		if(value)
			keysDown.push_front(KEY_NUM4);
		else
			keysDown.remove(KEY_NUM4);
		break;
	case 76:	//Key 'Numpad 5'
		if(value)
			keysDown.push_front(KEY_NUM5);
		else
			keysDown.remove(KEY_NUM5);
		break;
	case 77:	//Key 'Numpad 6'
		if(value)
			keysDown.push_front(KEY_NUM6);
		else
			keysDown.remove(KEY_NUM6);
		break;
	case 78:	//Key 'Numpad 7'
		if(value)
			keysDown.push_front(KEY_NUM7);
		else
			keysDown.remove(KEY_NUM7);
		break;
	case 79:	//Key 'Numpad 8'
		if(value)
			keysDown.push_front(KEY_NUM8);
		else
			keysDown.remove(KEY_NUM8);
		break;
	case 80:	//Key 'Numpad 9'
		if(value)
			keysDown.push_front(KEY_NUM9);
		else
			keysDown.remove(KEY_NUM9);
		break;
	case 81:	//Key 'MOUSELEFT'
		if(value)
		{
			keysDown.push_front(KEY_MOUSELEFT);
			leftX=mouseX;
			leftY=mouseY;
			if(uiCore->build!=0)
			{
				uiCore->clearMarks();
				uiCore->UI_Camera.rayTrace(mouseX,(RES_HEIGHT-30)-mouseY);
			}
		}
		else
		{
			keysDown.remove(KEY_MOUSELEFT);
			if(mouseX==leftX)
			{
				if(mouseY==leftY)
				{
					//user has left clicked and not moved the mouse
					mouseLeft=TRUE;
				}
				else
				{
					uiCore->UI_Camera.rayTrace(mouseX,(RES_HEIGHT-30)-mouseY);
					uiCore->enterPos();
				}
			}
			else
			{
				uiCore->UI_Camera.rayTrace(mouseX,(RES_HEIGHT-30)-mouseY);
				uiCore->enterPos();
			}
		}
		break;
	case 82:	//Key 'MOUSERIGHT'
		if(value)
			keysDown.push_front(KEY_MOUSERIGHT);
		else
			keysDown.remove(KEY_MOUSERIGHT);
		break;
	case 83:	//Key 'MOUSEMIDDLE'
		if(value)
			keysDown.push_front(KEY_MOUSEMIDDLE);
		else
			keysDown.remove(KEY_MOUSEMIDDLE);
		break;
	}
}

/****************************
* isPressed
****************************/
tBOOL UserInput::isPressed(int key)
{
	std::forward_list<int>::iterator iterator;
	iterator=keysDown.begin();

	if(keysDown.empty())
	{
		return false;
	}
	else
	{
		while(iterator!=keysDown.end())
		{
			if(*iterator==key)
				return true;
			iterator++;
		};
	}

	return false;
}

tBOOL UserInput::blockChanged()
{
	return blockChange;
}