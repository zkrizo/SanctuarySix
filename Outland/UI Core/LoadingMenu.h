#ifndef LoadingMenu_H
#define LoadingMenu_H

#include "Button.h"
#include <Outland\UnitAbstraction.h>
#include <Outland\Vector.h>
#include <vector>
#include <Windows.h>
#include <gl\glew.h>
#include <gl\GLU.h>

class LoadingMenu
{
public:
	LoadingMenu();
	~LoadingMenu();
	void clicked(real32 X,real32 Y);
	void Render();
	void Startup();

	void updateSaves();
	void clearSaves(){while(saves.size()!=0) saves.pop_back();};
private:
	std::vector<Button> saves;
};

#endif