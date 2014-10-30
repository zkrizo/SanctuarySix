#include "Button.h"
#include <iostream>
#include <Windows.h>
#include <fstream>

Button::Button():
	name(""),
	texture(NULL),
	sizeX(0),
	sizeY(0),
	posX(0),
	posY(0)
{}

Button::Button(string n, s32 sX, s32 sY, s32 pX, s32 pY, GLuint* text):
	name(n),
	texture(text),
	sizeX(sX),
	sizeY(sY),
	posX(pX),
	posY(pY)
{}

void Button::Render()
{
	glBindTexture(GL_TEXTURE_2D,*texture);

	//Draws a button
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f,0.0f);	glVertex2f(posX+sizeX,posY); //LR
		glTexCoord2f(0.0f,0.0f);	glVertex2f(posX,posY); //LL
		glTexCoord2f(0.0f,1.0f);	glVertex2f(posX,posY+sizeY); //UL
		glTexCoord2f(1.0f,1.0f);	glVertex2f(posX+sizeX,posY+sizeY); //UR
	glEnd();
}

bool Button::clicked(const s32& X,const s32& Y)
{
	if(X<=posX+sizeX && X>=posX)
		if(Y<=posY+sizeY && Y>=posY)
			return true;
		else
			return false;
	else
		return false;
}