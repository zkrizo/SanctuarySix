#include "FunctionList.h"
#include "UserInput.h"
#include <Outland\Singletons.h>
#include <iostream>
#include <fstream>

using namespace std;

FunctionList::FunctionList()
{
	ifstream File;

	File.open("config.ini");
	if(File)
	{

	}
	else
	{
		attack=new Function(KEY_MOUSELEFT);
		openinv=new Function(KEY_I);
		jump=new Function(KEY_SPACE);
		moveleft=new Function(KEY_A);
		moveright=new Function(KEY_D);
		moveforward=new Function(KEY_W);
		movebackward=new Function(KEY_S);
		menu=new Function(KEY_ESC);
		buildXP=new Function(KEY_RIGHTARROW);
		buildXM=new Function(KEY_LEFTARROW);
		buildZP=new Function(KEY_DOWNARROW);
		buildZM=new Function(KEY_UPARROW);
		buildYP=new Function(KEY_L);
		buildYM=new Function(KEY_M);
		openTech=new Function(KEY_T);
		openBuild=new Function(KEY_B);
		enter=new Function(KEY_ENTER);
	}

	for(int i=0;i<17;i++)
	{
		f[i]=false;
	}
	File.close();
}

FunctionList::~FunctionList()
{
	//saves config out to config.ini
	/*
	ofstream File;

	File.open("config.ini")

	//Data saving out to config file

	File.close();
	*/

	//Deletes the pointers for functions
	delete attack;
	delete openinv;
	delete jump;
	delete moveleft;
	delete moveright;
	delete moveforward;
	delete movebackward;
	delete menu;
	delete buildXP;
	delete buildXM;
	delete buildZP;
	delete buildZM;
	delete buildYP;
	delete buildYM;
	delete openTech;
	delete openBuild;
	delete enter;
}

int FunctionList::getKey(int function)
{
	switch(function)
	{
	case F_ATTACK:
		return attack->getKey();
		break;
	case F_OPENINV:
		return openinv->getKey();
		break;
	case F_JUMP:
		return jump->getKey();
		break;
	case F_MOVELEFT:
		return moveleft->getKey();
		break;
	case F_MOVERIGHT:
		return moveright->getKey();
		break;
	case F_MOVEFORWARD:
		return moveforward->getKey();
		break;
	case F_MOVEBACK:
		return movebackward->getKey();
		break;
	case F_MENU:
		return menu->getKey();
		break;
	case F_BUILDXP:
		return buildXP->getKey();
		break;
	case F_BUILDXM:
		return buildXM->getKey();
		break;
	case F_BUILDZP:
		return buildZP->getKey();
		break;
	case F_BUILDZM:
		return buildZM->getKey();
		break;
	case F_BUILDYP:
		return buildYP->getKey();
		break;
	case F_BUILDYM:
		return buildYM->getKey();
		break;
	case F_OPENBUILD:
		return openBuild->getKey();
		break;
	case F_OPENTECH:
		return openTech->getKey();
		break;
	case F_ENTER:
		return enter->getKey();
		break;
	default:
		return -1;
	}
}

void FunctionList::setKey(int function, int key)
{
	switch(function)
	{
	case F_ATTACK:
		attack->setKey(key);
		break;
	case F_OPENINV:
		openinv->setKey(key);
		break;
	case F_JUMP:
		jump->setKey(key);
		break;
	case F_MOVELEFT:
		moveleft->setKey(key);
		break;
	case F_MOVERIGHT:
		moveright->setKey(key);
		break;
	case F_MOVEFORWARD:
		moveforward->setKey(key);
		break;
	case F_MOVEBACK:
		movebackward->setKey(key);
		break;
	case F_MENU:
		menu->setKey(key);
		break;
	case F_BUILDXP:
		buildXP->setKey(key);
		break;
	case F_BUILDXM:
		buildXM->setKey(key);
		break;
	case F_BUILDZP:
		buildZP->setKey(key);
		break;
	case F_BUILDZM:
		buildZM->setKey(key);
		break;
	case F_BUILDYP:
		buildYP->setKey(key);
		break;
	case F_BUILDYM:
		buildYM->setKey(key);
		break;
	case F_OPENBUILD:
		openBuild->setKey(key);
		break;
	case F_OPENTECH:
		openTech->setKey(key);
		break;
	case F_ENTER:
		enter->setKey(key);
		break;
	}
}

void FunctionList::onFunction(int function)
{
	switch(function)
	{
	case F_ATTACK:

		break;
	case F_OPENINV:
		if(!f[F_OPENINV])
			uiCore->UI_GUI.setGUI(UI_RESOURCES);
		break;
	case F_JUMP:
		
		uiCore->UI_Camera.hasMoved(true);
		break;
	case F_MOVELEFT:
		uiCore->moveCam(-0.01f,0.0f,0.0f);
		uiCore->UI_Camera.hasMoved(true);
		break;
	case F_MOVERIGHT:
		uiCore->moveCam(0.01f,0.0f,0.0f);
		uiCore->UI_Camera.hasMoved(true);
		break;
	case F_MOVEFORWARD:
		uiCore->moveCam(0.0f,0.0f,-0.01f);
		uiCore->UI_Camera.hasMoved(true);
		break;
	case F_MOVEBACK:
		uiCore->moveCam(0.0f,0.0f,0.01f);
		uiCore->UI_Camera.hasMoved(true);
		break;
	case F_MENU:
		if(!f[F_MENU])
			uiCore->UI_GUI.setGUI(UI_MENU);
		break;
	case F_BUILDXP:
		if(!f[F_BUILDXP])
			uiCore->moveBuildMark(Vector3<s64>(1,0,0));
		break;
	case F_BUILDXM:
		if(!f[F_BUILDXM])
			uiCore->moveBuildMark(Vector3<s64>(-1,0,0));
		break;
	case F_BUILDZP:
		if(!f[F_BUILDZP])
			uiCore->moveBuildMark(Vector3<s64>(0,0,1));
		break;
	case F_BUILDZM:
		if(!f[F_BUILDZM])
			uiCore->moveBuildMark(Vector3<s64>(0,0,-1));
		break;
	case F_BUILDYP:
		uiCore->moveBuildMark(Vector3<s64>(0,1,0));
		break;
	case F_BUILDYM:
		uiCore->moveBuildMark(Vector3<s64>(0,-1,0));
		break;
	case F_OPENTECH:
		if(!f[F_OPENTECH])
			uiCore->UI_GUI.setGUI(UI_TECH);
		break;
	case F_OPENBUILD:
		if(!f[F_OPENBUILD])
			uiCore->UI_GUI.setGUI(UI_BUILDING);
		break;
	case F_ENTER:
		if(!f[F_ENTER])
		{
			uiCore->enterPos();
		}
		break;
	}
}