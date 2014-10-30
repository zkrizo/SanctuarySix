#ifndef FunctionList_H
#define FunctionList_H

#include "Function.h"

class FunctionList
{
public:
	FunctionList();
	~FunctionList();
	int getKey(int function);
	void setKey(int function, int key);

	void onFunction(int function);
	bool f[20];
private:
	Function *attack;
	Function *openinv;
	Function *jump;
	Function *moveleft;
	Function *moveright;
	Function *moveforward;
	Function *movebackward;
	Function *menu;
	Function *buildXP;
	Function *buildXM;
	Function *buildZP;
	Function *buildZM;
	Function *buildYP;
	Function *buildYM;
	Function *openTech;
	Function *openBuild;
	Function *enter;
};



#endif