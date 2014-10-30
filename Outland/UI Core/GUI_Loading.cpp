#include <Outland\Singletons.h>

extern int RES_WIDTH;
extern int RES_HEIGHT;
extern int HEIGHT_CHUNK;
extern int RENDER_DIST;
extern HDC hDC;

GUI_Loading::~GUI_Loading()
{

}

GUI_Loading* GUI_Loading::Instance()
{
	static GUI_Loading instance;
	return &instance;
}


void GUI_Loading::Render()
{
	//Only renders the loading bar if we are past the preload phase of loading textures
	if(current>GUI_MENUTOTAL)
	{
		glClearColor(0.0f,0.0f,0.0f,0.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		grCore->pRenderer->ready2D();

		//Draw the loading background
		if(background)
		{
			glColor3f(1.0f,1.0f,1.0f);
			glBindTexture(GL_TEXTURE_2D,*background);
			glBegin(GL_QUADS);
				glTexCoord2i(0,0);	glVertex2i(0,RES_HEIGHT);
				glTexCoord2i(0,1);	glVertex2i(0,0);
				glTexCoord2i(1,1);	glVertex2i(RES_WIDTH,0);
				glTexCoord2i(1,0);	glVertex2i(RES_WIDTH,RES_HEIGHT);
			glEnd();
		}

		//draw the loading bar outline
		if(barBG)
		{
			glColor3f(1.0f,1.0f,1.0f);
			glBindTexture(GL_TEXTURE_2D,*barBG);
			glBegin(GL_QUADS);
				glTexCoord2i(0,0);	glVertex2f(xM,yP);
				glTexCoord2i(0,1);	glVertex2f(xM,yM);
				glTexCoord2i(1,1);	glVertex2f(xP,yM);
				glTexCoord2i(1,0);	glVertex2f(xP,yP);
			glEnd();
		}
		else
		{
			glColor3f(1.0f,0.0f,0.0f);
			glBindTexture(GL_TEXTURE_2D,0);
			glPolygonMode(GL_FRONT,GL_LINE);
			glBegin(GL_QUADS);
				glVertex2f(xM,yP);
				glVertex2f(xM,yM);
				glVertex2f(xP,yM);
				glVertex2f(xP,yP);
			glEnd();
		
			glPolygonMode(GL_FRONT,GL_FILL);
		}

		real32 barPercent=real32(xT*(real32(current)/real32(total)));

		//Draw the loading bar
		if(barFG)
		{
			glColor3f(1.0f,1.0f,1.0f);
			glBindTexture(GL_TEXTURE_2D,*barFG);
			glBegin(GL_QUADS);
				glTexCoord2i(0,0);	glVertex2f(xM,yP);
				glTexCoord2i(0,1);	glVertex2f(xM,yM);
				glTexCoord2i(1,1);	glVertex2f(xM+barPercent,yM);
				glTexCoord2i(1,0);	glVertex2f(xM+barPercent,yP);
			glEnd();
		}
		else
		{
			glColor3f(1.0f,0.0f,0.0f);
			glBindTexture(GL_TEXTURE_2D,0);
			glBegin(GL_QUADS);
				glTexCoord2i(0,0);	glVertex2f(xM,yP);
				glTexCoord2i(0,1);	glVertex2f(xM,yM);
				glTexCoord2i(1,1);	glVertex2f(xM+barPercent,yM);
				glTexCoord2i(1,0);	glVertex2f(xM+barPercent,yP);
			glEnd();
		}

		std::string loadingText;
	
		if(current<=numMenuTextures)
			loadingText="Loading User Interface Textures";
		else if(current<=(numMenuTextures+numBlockTextures))
			loadingText="Loading Block Textures";
		else if(current<=(numMenuTextures+numBlockTextures+numWallTextures))
			loadingText="Loading Wall Textures";
		else if(current<=(numMenuTextures+numBlockTextures+numWallTextures+numFloorTextures))
			loadingText="Loading Floor Textures";
		else if(current<=(numMenuTextures+numBlockTextures+numWallTextures+numFloorTextures+numCeilingTextures))
			loadingText="Loading Ceiling Textures";
		else if(current<=(numMenuTextures+numBlockTextures+numWallTextures+numFloorTextures+numCeilingTextures+numModels))
			loadingText="Loading Model Files";
		else if(current<=(numMenuTextures+numBlockTextures+numWallTextures+numFloorTextures+numCeilingTextures+numModels+numChunks))
			loadingText="Loading Chunks";
		else if(current<=(numMenuTextures+numBlockTextures+numWallTextures+numFloorTextures+numCeilingTextures+numModels+numChunks))
			loadingText="Loading Resources";
		else if(current<=total)
			loadingText="Loading Performance Enhancements";
	
		glBindTexture(GL_TEXTURE_2D,0);
		int xoffset=loadingText.size()/2*8;
		uiCore->renderText(RES_WIDTH/2-xoffset,yText,loadingText.c_str());

		grCore->pRenderer->ready3D();

		SwapBuffers(hDC);
	}
}

void GUI_Loading::Startup()
{
	numMenuTextures=GUI_TOTALRESOURCES+GUI_MENUTOTAL;
	numBlockTextures=BlockType_numTypes-7;		//will be -3 in final
	numWallTextures=3;//RM_NUMTYPES;
	numFloorTextures=FL_NUMFLOORS;
	numCeilingTextures=0;//WALL_NUMWALLS;
	numModels=MODEL_TOTALNUM;
	numChunks=RENDER_DIST*RENDER_DIST*HEIGHT_CHUNK; //Once for chunk loading and again for initial active culling
	total=numMenuTextures+numBlockTextures+numWallTextures+numFloorTextures+numCeilingTextures+numChunks+numChunks;
	current=0;

	//Set background image to texture

	xP=RES_WIDTH*0.9;
	xM=RES_WIDTH/10;
	yP=RES_HEIGHT*0.2;
	yM=RES_HEIGHT*0.1;
	xT=xP-xM;
	yT=yP-yM;
	yText=RES_HEIGHT*0.25;
}

void GUI_Loading::incrementAndRender()
{
	++current;
	Render();
}