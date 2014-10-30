#include "messageBox.h"
#include <Outland\Singletons.h>

messageBox::messageBox():
	texture(NULL),
	Message(""),
	posX(0),
	posY(0),
	sizeX(0),
	sizeY(0),
	next(NULL),
	skip(NULL)
{}

messageBox::~messageBox()
{
	if(next!=NULL)
		delete next;
	if(skip!=NULL)
		delete skip;
}

void messageBox::Startup()
{
	texture= grCore->pRenderer->getTexture(2,GUI_WINDOW);
	next=new Button("Next",60,30,posX+sizeX-70,posY+40,grCore->pRenderer->getTexture(2,GUI_NEXT));
	skip=new Button("Skip",60,30,posX+sizeX-140,posY+40,grCore->pRenderer->getTexture(2,GUI_SKIP));
}

void messageBox::Render()
{
	glColor4f(1.0f,1.0f,1.0f,0.8f);

	glBindTexture(GL_TEXTURE_2D,*texture);

	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,1.0f);	glVertex2f(posX+sizeX,posY); //LR
		glTexCoord2f((float)480.0f/512.0f,1.0f);	glVertex2f(posX,posY); //LL
		glTexCoord2f((float)480.0f/512.0f,1.0f-(float)421.0f/512.0f);	glVertex2f(posX,posY+sizeY); //UL
		glTexCoord2f(0.0f,1.0f-(float)421.0f/512.0f);	glVertex2f(posX+sizeX,posY+sizeY); //UR
	glEnd();

	glColor3f(1.0f,1.0f,1.0f);

	next->Render();
	skip->Render();

	glBindTexture(GL_TEXTURE_2D,0);
	
	uiCore->renderText(posX+10,posY+sizeY-15,Message.c_str());
	uiCore->renderText(posX+10,posY+sizeY-30,Message2.c_str());
	uiCore->renderText(posX+10,posY+sizeY-45,Message3.c_str());
	uiCore->renderText(posX+10,posY+sizeY-60,Message4.c_str());
}

void messageBox::setInfo(string message,string message2,string message3,string message4, s32 posx, s32 posy, s32 sizex, s32 sizey, bool nextv, bool skipv)
{
	Message=message;
	Message2=message2;
	Message3=message3;
	Message4=message4;
	posX=posx;
	posY=posy;
	sizeX=sizex;
	sizeY=sizey;
	nextShow=nextv;
	skipShow=skipv;
	next->posX=posX+sizeX-70;
	skip->posX=posX+sizeX-140;
	next->posY=posY+5;
	skip->posY=posY+5;
}

s32 messageBox::click(s32 X, s32 Y)
{
	if(nextShow)
		if(next->clicked(X,Y))
			return 1;

	if(skipShow)
		if(skip->clicked(X,Y))
			return 2;

	if(X>=posX && X<=posX+sizeX)
		if(Y>=posY && Y<=posY+sizeY)
			return 3;

	return 0;
}