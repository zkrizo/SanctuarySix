#ifndef Button_H
#define Button_H

#include <string>
#include <Outland\UnitAbstraction.h>
#include <Windows.h>
#include <gl\glew.h>
#include <gl\GLU.h>

using namespace std;

class Button
{
public:
	Button();
	Button(string name, s32 sizeX, s32 sizeY, s32 posX, s32 posY,GLuint *text);

	string name;
	//texture
	GLuint *texture;
	void Render();
	bool clicked(const s32& X,const s32& Y);

	//size
	s32 sizeX;
	s32 sizeY;
	s32 posX;
	s32 posY;
};

#endif