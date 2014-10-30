#include "OpenGLRenderer.h"
#include "Bitmap.h"
#include <Outland\Singletons.h>
#include <string>
#include <iostream>
#include <Windows.h>
#include <gl\glew.h>
#include <gl\GLU.h>
#include <fstream>
#include <cstdio>
#include <SOIL.h>
#include <Outland\Singletons.h>

//External declarations
extern HDC hDC;
extern int CHUNK_SIZE;
extern int RENDER_DIST;
extern int RES_WIDTH;
extern int RES_HEIGHT;

//GLuint tempTexture;

/****************************
* Constructor
****************************/
OpenGLRenderer::OpenGLRenderer():
	box(0),
	room0Walls(0),
	room1Walls(0),
	room2WallsHall(0),
	room2WallsCorner(0),
	room3Walls(0),
	room4Walls(0)
{
	//create a new memory space for each block texture
	blockTextures=new GLuint[(BlockType_numTypes-1)];
	menuTextures=new GLuint[6];
	GUITextures=new GLuint[GUI_TOTALRESOURCES];
	roomTextures=new GLuint[RM_NUMTYPES];
	floorTextures=new GLuint[FL_NUMFLOORS];
	outerTextures=new GLuint[WALL_NUMWALLS];
	modelTexturesDiffuse=new GLuint[MODEL_TOTALNUM];
	modelTexturesNormals=new GLuint[MODEL_TOTALNUM];
	modelTexturesBump=new GLuint[MODEL_TOTALNUM];
}

/****************************
* Destructor
****************************/
OpenGLRenderer::~OpenGLRenderer()
{
	//deallocate the block textures
	delete [] blockTextures;
	delete [] menuTextures;
	delete [] GUITextures;
	delete [] roomTextures;
	delete [] floorTextures;
	delete [] outerTextures;
	delete [] modelTexturesDiffuse;
	delete [] modelTexturesNormals;
	delete [] modelTexturesBump;
}

/****************************
* startup -	Responsible for loading all menu textures
****************************/
void OpenGLRenderer::startup()
{
	//Loads the Main Menu textures
	LoadBMPImage("Textures/Menu/TitleTemplate.tga",menuTextures[0]);	//Loads the title bar
	LoadBMPImage("Textures/Menu/NewU.tga",menuTextures[1]);			//loads the new universe
	LoadBMPImage("Textures/Menu/LoadU.tga",menuTextures[2]);			//loads the load universe
	LoadBMPImage("Textures/Menu/Multi.tga",menuTextures[3]);			//loads the multiplayer
	LoadBMPImage("Textures/Menu/Options.tga",menuTextures[4]);		//loads the options
	LoadBMPImage("Textures/Menu/Quit.tga",menuTextures[5]);				//loads the quit

	if(!generateLists())
		cout << "Generation of Display Lists for OpenGL Failed!\n";
}

/****************************
* RenderTestCube
****************************/
void OpenGLRenderer::RenderTestCube()
{
	float h=0.5f;
	float w=0.5f;
	float l=0.5f;
	glColor3f(1,0,0);
	//for(int i=0;i<10;++i)
	//{
	glPushMatrix();
		//glTranslatef((float)x,(float)y,(float)z);
		glBegin(GL_QUADS);
			glNormal3f(0.0f,0.0f,-1.0f);
			glVertex3f(l,-h,-w);
			glVertex3f(-l,-h,-w);
			glVertex3f(-l,h,-w);
			glVertex3f(l,h,-w);
			glEnd();
		//glPolygonMode(GL_FRONT,GL_FILL);
		//glBindTexture(GL_TEXTURE_2D,tempTexture);
		//glDisable(GL_COLOR_MATERIAL);

		GLuint errcode=glGetError();
		const GLubyte *error;
		error=gluErrorString(errcode);

		glBegin(GL_TRIANGLE_STRIP);
			glNormal3f(0.0f,0.0f,1.0f);
			glTexCoord2f(0,0);
			glVertex3f(-l,-h,w);
			glTexCoord2f(1,0);
			glVertex3f(l,-h,w);
			glTexCoord2f(1,1);
			glVertex3f(l,h,w);
			glTexCoord2f(0,1);
			glVertex3f(-l,h,w);

			glEnd();
			glBindTexture(GL_TEXTURE_2D,0);
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			glBegin(GL_TRIANGLE_STRIP);			

			glNormal3f(1,0,0);
			glVertex3f(l,-h,w);
			glVertex3f(l,-h,-w);
			glVertex3f(l,h,-w);
			glVertex3f(l,h,w);

			glNormal3f(-1,0,0);
			glVertex3f(-l,-h,-w);
			glVertex3f(-l,-h,w);
			glVertex3f(-l,h,w);
			glVertex3f(-l,h,-w);

			glNormal3f(0,-1,0);
			glVertex3f(-l,-h,-w);
			glVertex3f(l,-h,-w);
			glVertex3f(l,-h,w);
			glVertex3f(-l,-h,w);

			glNormal3f(0,1,0);
			glVertex3f(l,h,-w);
			glVertex3f(-l,h,-w);
			glVertex3f(-l,h,w);
			glVertex3f(l,h,w);
		glEnd();
	glPopMatrix();
		errcode=glGetError();
		error=gluErrorString(errcode);
	//x++;
	//}
}

/****************************
* RenderTestCube
****************************/
void OpenGLRenderer::RenderTestCube(const Block& block,int x,int y, int z)
{
	int faceData=block.getFaceData();
	if((faceData | 0) !=0)
	{
	 	glPolygonMode(GL_FRONT,GL_FILL);
	
	 	setBlockTexture(block.getType());


	 	glPushMatrix();
	 	 	glTranslatef((float)x,(float)y,(float)z);

	 	 	if((faceData & Face_YP) == Face_YP)
	 	 	 	glCallList(FacePolyYP);
	 	 	if((faceData & Face_YM) == Face_YM)
	 	 	 	glCallList(FacePolyYM);
	 	 	if((faceData & Face_XP) == Face_XP)
	 	 	 	glCallList(FacePolyXP);
	 	 	if((faceData & Face_XM) == Face_XM)
	 	 	 	glCallList(FacePolyXM);
	 	 	if((faceData & Face_ZP) == Face_ZP)
	 	 	 	glCallList(FacePolyZP);
	 	 	if((faceData & Face_ZM) == Face_ZM)
	 	 	 	glCallList(FacePolyZM);
	 	glPopMatrix();
	}


	//	errcode=glGetError();
		//error=gluErrorString(errcode);
}

/****************************
* RenderTestChunk
****************************/
void OpenGLRenderer::RenderTestChunk(Chunk *chunk)
{

	//Renders the chunks
	chunk->Render();

	Chunk chunk2;
	Chunk chunk3,chunk4,chunk5,chunk6;

	chunk2.pos.x=-10;
	chunk3.pos.z=-10;
	chunk4.pos.x=-20;
	chunk5.pos.x=-20;
	chunk5.pos.z=-10;
	chunk6.pos.x=-10;
	chunk6.pos.z=-10;

	chunk2.Render();
	chunk3.Render();
	chunk4.Render();
	chunk5.Render();
	chunk6.Render();

	//pops the world coordinates back off the stack to - back to local camera coordinates
	glPopMatrix();

}

/****************************
* LoadBMPImage
****************************/
tBOOL OpenGLRenderer::LoadBMPImage(t8 *file,GLuint &texture)
{
	texture=SOIL_load_OGL_texture(file,SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y | SOIL_FLAG_MIPMAPS);
	if(texture==0)
	{
		return FALSE;
	}

	glBindTexture(GL_TEXTURE_2D,texture);
	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);

	GUILoad->incrementAndRender();
	return TRUE;
}

/****************************
* LoadBMPImageNoFlip
****************************/
tBOOL OpenGLRenderer::LoadBMPImageNoFlip(t8 *file,GLuint &texture)
{
	texture=SOIL_load_OGL_texture(file,SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_MIPMAPS);
	if(texture==0)
	{
		return FALSE;
	}

	glBindTexture(GL_TEXTURE_2D,texture);
	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);

	return TRUE;
}
/****************************
* RenderTestBuilding
****************************/
void OpenGLRenderer::RenderTestBuilding()
{
	//This is to test the rendering of a standard room. This will draw a 2x2 room for now
	float ih=0.48f;
	float iw=0.48f;
	float il=0.48f; //these are the inner dimensions for the room. Each room will be slightly smaller than a block.

	float h=0.5f;
	float w=0.5f;
	float l=0.5f;

	float x=0,y=0,z=5;
	int sizeX=1;
	int sizeY=2;

	glColor3f(0.0f,0.5f,0.0f);
	glPolygonMode(GL_FRONT,GL_FILL);
	glPushMatrix();
	glTranslatef((float)x,(float)y,(float)z);

	glBegin(GL_QUADS);
	
		for(int i=0;i<sizeX;i++)
		{
			//floor X
			glVertex3f(iw+i,-ih,il);
			glVertex3f(iw+i,-ih,-il);
			glVertex3f(-iw+i,-ih,-il);
			glVertex3f(-iw+i,-ih,il);


			//ceiling (see through from 3rd person)
			glVertex3f(-iw+i,ih,-il);
			glVertex3f(iw+i,ih,-il);
			glVertex3f(iw+i,ih,il);
			glVertex3f(-iw+i,ih,il);
			for(int j=1;j<sizeY;j++)
			{
				//floor Y
				glVertex3f(iw+i,-ih,il+j);
				glVertex3f(iw+i,-ih,-il+j);
				glVertex3f(-iw+i,-ih,-il+j);
				glVertex3f(-iw+i,-ih,il+j);

				//ceiling (see through from 3rd person)
				glVertex3f(-iw+i,ih,-il+j);
				glVertex3f(iw+i,ih,-il+j);
				glVertex3f(iw+i,ih,il+j);
				glVertex3f(-iw+i,ih,il+j);
			}		
		}
		
		for(int i=0;i<sizeY;i++)
		{
			glColor3f(0.0f,0.5f,0.0f);
			//Inner left wall
			glVertex3f(-iw,-ih,-il+i);
			glVertex3f(-iw,ih,-il+i);
			glVertex3f(-iw,ih,il+i);
			glVertex3f(-iw,-ih,il+i);

			//Inner right wall
			glVertex3f(iw+(sizeX-1),-ih,-il+i);
			glVertex3f(iw+(sizeX-1),-ih,il+i);
			glVertex3f(iw+(sizeX-1),ih,il+i);
			glVertex3f(iw+(sizeX-1),ih,-il+i);

			glColor3f(0.0f,0.0f,0.5f);
			//upper walls for 3rd person cam
			//left wall
			glVertex3f(-iw,ih,il+i);
			glVertex3f(-iw,ih,-il+i);
			glVertex3f(-w,h,-l+i);
			glVertex3f(-w,h,l+i);

			//right
			glVertex3f(iw+(sizeX-1),ih,-il+i);
			glVertex3f(iw+(sizeX-1),ih,il+i);
			glVertex3f(w+(sizeX-1),h,l+i);
			glVertex3f(w+(sizeX-1),h,-l+i);

			glColor3f(0.5f,0.5f,0.0f);
			//right
			glVertex3f(w+(sizeX-1),h,l+i);
			glVertex3f(w+(sizeX-1),-h,l+i);
			glVertex3f(w+(sizeX-1),-h,-l+i);
			glVertex3f(w+(sizeX-1),h,-l+i);

			//left
			glVertex3f(-w,h,l+i);
			glVertex3f(-w,h,-l+i);
			glVertex3f(-w,-h,-l+i);
			glVertex3f(-w,-h,l+i);
		}

		for(int i=0;i<sizeX;i++)
		{
			glColor3f(0.0f,0.5f,0.0f);
			//Inner back wall
			glVertex3f(-iw+i,-ih,-il);
			glVertex3f(iw+i,-ih,-il);
			glVertex3f(iw+i,ih,-il);
			glVertex3f(-iw+i,ih,-il);

			//Inner front wall
			glVertex3f(-iw+i,-ih,il+(sizeY-1));
			glVertex3f(-iw+i,ih,il+(sizeY-1));
			glVertex3f(iw+i,ih,il+(sizeY-1));
			glVertex3f(iw+i,-ih,il+(sizeY-1));

			glColor3f(0.0f,0.0f,0.5f);
			//back
			glVertex3f(-iw+i,ih,-il);
			glVertex3f(iw+i,ih,-il);
			glVertex3f(w+i,h,-l);
			glVertex3f(-w+i,h,-l);

			//front
			glVertex3f(-iw+i,ih,il+(sizeY-1));
			glVertex3f(-w+i,h,l+(sizeY-1));
			glVertex3f(w+i,h,l+(sizeY-1));
			glVertex3f(iw+i,ih,il+(sizeY-1));		

			glColor3f(0.5f,0.5f,0.0f);
			//Outer walls
			//front
			glVertex3f(w+i,h,l+(sizeY-1));
			glVertex3f(-w+i,h,l+(sizeY-1));
			glVertex3f(-w+i,-h,l+(sizeY-1));
			glVertex3f(w+i,-h,l+(sizeY-1));

			//back
			glVertex3f(w+i,h,-l);
			glVertex3f(w+i,-h,-l);
			glVertex3f(-w+i,-h,-l);
			glVertex3f(-w+i,h,-l);
		}
	glEnd();
	glPopMatrix();
}

/****************************
* RenderTestBuilding - individual block
****************************/
void OpenGLRenderer::RenderTestBuilding(Vector3<s32> pos, s32 orient, s32 walls, s32 type, tBOOL special,tBOOL powered)
{
	float ih=0.48f;
	float iw=0.48f;
	float il=0.48f; //these are the inner dimensions for the room. Each room will be slightly smaller than a block.

	float h=0.5f;
	float w=0.5f;
	float l=0.5f;

	glPushMatrix();
	glTranslatef(GLfloat(pos.x),GLfloat(pos.y),GLfloat(pos.z));
	glPushMatrix();
	glRotatef((GLfloat)orient,0.0f,1.0f,0.0f);

	
	//main drawing switch
	switch(walls)
	{
	case 4:
		//sets color to white
		glColor3f(1.0f,1.0f,1.0f);

		//determines the texture from the building type
		switch(type)
		{
		case RM_HALLWAY:
		case RM_AIRLOCK:
		case RM_BATHROOM:
		case RM_COMMANDCENTER:
		case RM_FREIGHTAIRLOCK:
		case RM_GALLEY:
		case RM_INDUSTRIALWORKSHOP:
		case RM_LABORATORY:
		case RM_LIBRARY:
		case RM_LIVINGQUARTERS:
		case RM_MININGCONTROLCENTER:
		case RM_OXYGENSCRUBBER:
		case RM_POWERFUSION:
		case RM_WAREHOUSE:
		case RM_WATERCOLLECTOR:
		case RM_WATERRECYCLING:
			glBindTexture(GL_TEXTURE_2D,outerTextures[0]);
			break;
		default:
			glBindTexture(GL_TEXTURE_2D,0);
			glColor3f(0.5f,0.5f,0.0f);
			break;
		}
		//Begins drawing
		glBegin(GL_QUADS);

			//Outer wall front
			glTexCoord2f(1.0f,1.0f);		glVertex3f(w,h,l);
			glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,h,l);
			glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-h,l);
			glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-h,l);

			//Outer wall right
			glTexCoord2f(1.0f,1.0f);		glVertex3f(w,h,l);
			glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-h,l);
			glTexCoord2f(0.0f,0.0f);		glVertex3f(w,-h,-l);
			glTexCoord2f(0.0f,1.0f);		glVertex3f(w,h,-l);

			//Outer wall back
			glTexCoord2f(1.0f,1.0f);		glVertex3f(w,h,-l);
			glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-h,-l);
			glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-h,-l);
			glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,h,-l);

			//Outer wall left
			glTexCoord2f(1.0f,1.0f);		glVertex3f(-w,h,l);
			glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,h,-l);
			glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-h,-l);
			glTexCoord2f(1.0f,0.0f);		glVertex3f(-w,-h,l);

		glEnd();

		//sets color to white if powered or red if unpowered
		if(powered)
			glColor3f(1.0f,1.0f,1.0f);
		else
			glColor3f(1.0f,0.25f,0.25f);

		//determins the texture from the building type
		switch(type)
			{
			case RM_HALLWAY:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_HALLWAY]);
				break;
			case RM_AIRLOCK:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_AIRLOCK]);
				break;
			case RM_BATHROOM:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_BATHROOM]);
				break;
			case RM_COMMANDCENTER:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_COMMANDCENTER]);
				break;
			case RM_FREIGHTAIRLOCK:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_FREIGHTAIRLOCK]);
				break;
			case RM_GALLEY:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_GALLEY]);
				break;
			case RM_INDUSTRIALWORKSHOP:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_INDUSTRIALWORKSHOP]);
				break;
			case RM_LABORATORY:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_LABORATORY]);
				break;
			case RM_LIBRARY:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_LIBRARY]);
				break;
			case RM_LIVINGQUARTERS:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_LIVINGQUARTERS]);
				break;
			case RM_MININGCONTROLCENTER:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_MININGCONTROLCENTER]);
				break;
			case RM_OXYGENSCRUBBER:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_OXYGENSCRUBBER]);
				break;
			case RM_POWERFUSION:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_POWERFUSION]);
				break;
			case RM_WAREHOUSE:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_WAREHOUSE]);
				break;
			case RM_WATERCOLLECTOR:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_WATERCOLLECTOR]);
				break;
			case RM_WATERRECYCLING:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_WATERRECYCLING]);
				break;
			default:
				glBindTexture(GL_TEXTURE_2D,0);
				glColor3f(0.0f,0.5f,0.0f);
				break;
			}

		//Begins drawing
		glBegin(GL_QUADS);

			//Inner wall front
			glTexCoord2f(0.0f,0.04f);		glVertex3f(-iw,-ih,il);
			glTexCoord2f(0.0f,0.96f);		glVertex3f(-iw,ih,il);
			glTexCoord2f(0.96f,0.96f);		glVertex3f(iw,ih,il);
			glTexCoord2f(0.96f,0.04f);		glVertex3f(iw,-ih,il);
		
			//Inner wall right
			glTexCoord2f(0.04f,0.04f);		glVertex3f(iw,-ih,-il);
			glTexCoord2f(0.96f,0.04f);		glVertex3f(iw,-ih,il);
			glTexCoord2f(0.96f,0.96f);		glVertex3f(iw,ih,il);
			glTexCoord2f(0.04f,0.96f);		glVertex3f(iw,ih,-il);

			//Inner wall back
			glTexCoord2f(0.0f,0.04f);		glVertex3f(-iw,-ih,-il);
			glTexCoord2f(0.96f,0.04f);		glVertex3f(iw,-ih,-il);
			glTexCoord2f(0.96f,0.96f);		glVertex3f(iw,ih,-il);
			glTexCoord2f(0.0f,0.96f);		glVertex3f(-iw,ih,-il);

			//Inner wall left
			glTexCoord2f(0.04f,0.04f);		glVertex3f(-iw,-ih,-il);
			glTexCoord2f(0.04f,0.96f);		glVertex3f(-iw,ih,-il);
			glTexCoord2f(0.96f,0.96f);		glVertex3f(-iw,ih,il);
			glTexCoord2f(0.96f,0.04f);		glVertex3f(-iw,-ih,il);

		glEnd();

		//clears texture and sets color to slightly grey
		glBindTexture(GL_TEXTURE_2D,0);
		glColor3f(0.05f,0.05f,0.05f);
		
		//Begins drawing
		glBegin(GL_QUADS);

			//Upper wall front
			glVertex3f(-iw,ih,il);
			glVertex3f(-w,h,l);
			glVertex3f(w,h,l);
			glVertex3f(iw,ih,il);	

			//upper wall right
			glVertex3f(iw,ih,-il);
			glVertex3f(iw,ih,il);	
			glVertex3f(w,h,l);
			glVertex3f(w,h,-l);

			//Upper wall back
			glVertex3f(-iw,ih,-il);
			glVertex3f(iw,ih,-il);	
			glVertex3f(w,h,-l);
			glVertex3f(-w,h,-l);

			//upper wall left
			glVertex3f(-iw,ih,-il);	
			glVertex3f(-w,h,-l);
			glVertex3f(-w,h,l);
			glVertex3f(-iw,ih,il);

		glEnd();

		//sets color to white if powered or red if unpowered
		if(powered)
			glColor3f(1.0f,1.0f,1.0f);
		else
			glColor3f(1.0f,0.25f,0.25f);

		//determine the texture from the building type
		switch(type)
		{
		case RM_HALLWAY:
			glBindTexture(GL_TEXTURE_2D,floorTextures[FL_GRATE]);
			break;
		case RM_AIRLOCK:
		case RM_BATHROOM:
			glBindTexture(GL_TEXTURE_2D,floorTextures[FL_TILED]);
			break;
		case RM_COMMANDCENTER:
			glBindTexture(GL_TEXTURE_2D,floorTextures[FL_METAL]);
			break;
		case RM_FREIGHTAIRLOCK:
		case RM_GALLEY:
		case RM_INDUSTRIALWORKSHOP:
		case RM_LABORATORY:
		case RM_LIBRARY:
		case RM_LIVINGQUARTERS:
			glBindTexture(GL_TEXTURE_2D,floorTextures[FL_METAL]);
			break;
		case RM_MININGCONTROLCENTER:
		case RM_OXYGENSCRUBBER:
		case RM_POWERFUSION:
		case RM_WAREHOUSE:
		case RM_WATERCOLLECTOR:
		case RM_WATERRECYCLING:
		default:
			//sets the texture to blank and the debug color
			glBindTexture(GL_TEXTURE_2D,0);
			glColor3f(0.5f,0.5f,0.5f);
			break;
		}

		//Begins drawing
		glBegin(GL_QUADS);

			//floor
			glTexCoord2f(0.96f,0.04f);		glVertex3f(iw,-ih,il);
			glTexCoord2f(0.96f,0.96f);		glVertex3f(iw,-ih,-il);
			glTexCoord2f(0.04f,0.96f);		glVertex3f(-iw,-ih,-il);
			glTexCoord2f(0.04f,0.04f);		glVertex3f(-iw,-ih,il);

		glEnd();

		//sets color to white if powered or red if unpowered
		if(powered)
			glColor3f(1.0f,1.0f,1.0f);
		else
			glColor3f(1.0f,0.25f,0.25f);

		//determine the texture from the building type
		switch(type)
		{
		case RM_HALLWAY:
		case RM_AIRLOCK:
		case RM_BATHROOM:
		case RM_COMMANDCENTER:
		case RM_FREIGHTAIRLOCK:
		case RM_GALLEY:
		case RM_INDUSTRIALWORKSHOP:
		case RM_LABORATORY:
		case RM_LIBRARY:
		case RM_LIVINGQUARTERS:
		case RM_MININGCONTROLCENTER:
		case RM_OXYGENSCRUBBER:
		case RM_POWERFUSION:
		case RM_WAREHOUSE:
		case RM_WATERCOLLECTOR:
		case RM_WATERRECYCLING:
		default:
			//sets the texture to blank and the debug color
			glBindTexture(GL_TEXTURE_2D,0);
			glColor3f(0.5f,0.5f,0.5f);
			break;
		}

		//Begins drawing
		glBegin(GL_QUADS);
			//ceiling
			glTexCoord2f(0.04f,0.96f);		glVertex3f(-iw,ih,-il);
			glTexCoord2f(0.96f,0.96f);		glVertex3f(iw,ih,-il);
			glTexCoord2f(0.96f,0.04f);		glVertex3f(iw,ih,il);
			glTexCoord2f(0.04f,0.04f);		glVertex3f(-iw,ih,il);

		glEnd();

		break;
	case 3:
		//sets color to white
		glColor3f(1.0f,1.0f,1.0f);

		//determines the texture from the building type
		switch(type)
		{
		case RM_HALLWAY:
		case RM_AIRLOCK:
		case RM_BATHROOM:
		case RM_COMMANDCENTER:
		case RM_FREIGHTAIRLOCK:
		case RM_GALLEY:
		case RM_INDUSTRIALWORKSHOP:
		case RM_LABORATORY:
		case RM_LIBRARY:
		case RM_LIVINGQUARTERS:
		case RM_MININGCONTROLCENTER:
		case RM_OXYGENSCRUBBER:
		case RM_POWERFUSION:
		case RM_WAREHOUSE:
		case RM_WATERCOLLECTOR:
		case RM_WATERRECYCLING:
			glBindTexture(GL_TEXTURE_2D,outerTextures[0]);
			break;
		default:
			glBindTexture(GL_TEXTURE_2D,0);
			glColor3f(0.5f,0.5f,0.0f);
			break;
		}

		//Begins drawing
		glBegin(GL_QUADS);

			//Outer wall front
			glTexCoord2f(1.0f,1.0f);		glVertex3f(w,h,l);
			glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,h,l);
			glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-h,l);
			glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-h,l);

			//Outer wall right
			glTexCoord2f(1.0f,1.0f);		glVertex3f(w,h,l);
			glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-h,l);
			glTexCoord2f(0.0f,0.0f);		glVertex3f(w,-h,-l);
			glTexCoord2f(0.0f,1.0f);		glVertex3f(w,h,-l);

			//Outer wall back
			glTexCoord2f(1.0f,1.0f);		glVertex3f(w,h,-l);
			glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-h,-l);
			glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-h,-l);
			glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,h,-l);


		glEnd();

		//sets color to white if powered or red if unpowered
		if(powered)
			glColor3f(1.0f,1.0f,1.0f);
		else
			glColor3f(1.0f,0.25f,0.25f);

		//determins the texture from the building type
		switch(type)
			{
			case RM_HALLWAY:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_HALLWAY]);
				break;
			case RM_AIRLOCK:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_AIRLOCK]);
				break;
			case RM_BATHROOM:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_BATHROOM]);
				break;
			case RM_COMMANDCENTER:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_COMMANDCENTER]);
				break;
			case RM_FREIGHTAIRLOCK:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_FREIGHTAIRLOCK]);
				break;
			case RM_GALLEY:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_GALLEY]);
				break;
			case RM_INDUSTRIALWORKSHOP:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_INDUSTRIALWORKSHOP]);
				break;
			case RM_LABORATORY:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_LABORATORY]);
				break;
			case RM_LIBRARY:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_LIBRARY]);
				break;
			case RM_LIVINGQUARTERS:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_LIVINGQUARTERS]);
				break;
			case RM_MININGCONTROLCENTER:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_MININGCONTROLCENTER]);
				break;
			case RM_OXYGENSCRUBBER:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_OXYGENSCRUBBER]);
				break;
			case RM_POWERFUSION:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_POWERFUSION]);
				break;
			case RM_WAREHOUSE:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_WAREHOUSE]);
				break;
			case RM_WATERCOLLECTOR:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_WATERCOLLECTOR]);
				break;
			case RM_WATERRECYCLING:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_WATERRECYCLING]);
				break;
			default:
				glBindTexture(GL_TEXTURE_2D,0);
				glColor3f(0.0f,0.5f,0.0f);
				break;
			}

		//Begins drawing
		glBegin(GL_QUADS);

			//Inner wall front
			glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-ih,il);
			glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,ih,il);
			glTexCoord2f(0.98f,1.0f);		glVertex3f(iw,ih,il);
			glTexCoord2f(0.98f,0.0f);		glVertex3f(iw,-ih,il);
		
			//Inner wall right
			glTexCoord2f(0.02f,0.0f);		glVertex3f(iw,-ih,-l);
			glTexCoord2f(0.98f,0.0f);		glVertex3f(iw,-ih,il);
			glTexCoord2f(0.98f,1.0f);		glVertex3f(iw,ih,il);
			glTexCoord2f(0.02f,1.0f);		glVertex3f(iw,ih,-l);

			//Inner wall back
			glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-ih,-il);
			glTexCoord2f(0.98f,0.0f);		glVertex3f(iw,-ih,-il);
			glTexCoord2f(0.98f,1.0f);		glVertex3f(iw,ih,-il);
			glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,ih,-il);

		glEnd();

		//clears texture and sets color to slightly grey
		glBindTexture(GL_TEXTURE_2D,0);
		glColor3f(0.05f,0.05f,0.05f);
		
		//Begins drawing
		glBegin(GL_QUADS);

			//Upper wall front
			glVertex3f(-w,ih,il);
			glVertex3f(-w,h,l);
			glVertex3f(w,h,l);
			glVertex3f(w,ih,il);	

			//upper wall right
			glVertex3f(iw,ih,-l);
			glVertex3f(iw,ih,il);	
			glVertex3f(w,h,l);
			glVertex3f(w,h,-l);

			//Upper wall back
			glVertex3f(-w,ih,-il);
			glVertex3f(w,ih,-il);
			glVertex3f(w,h,-l);
			glVertex3f(-w,h,-l);

		glEnd();

		//sets color to white if powered or red if unpowered
		if(powered)
			glColor3f(1.0f,1.0f,1.0f);
		else
			glColor3f(1.0f,0.25f,0.25f);

		//determine the texture from the building type
		switch(type)
		{
		case RM_HALLWAY:
			glBindTexture(GL_TEXTURE_2D,floorTextures[FL_GRATE]);
			break;
		case RM_AIRLOCK:
		case RM_BATHROOM:
			glBindTexture(GL_TEXTURE_2D,floorTextures[FL_TILED]);
			break;
		case RM_COMMANDCENTER:
			glBindTexture(GL_TEXTURE_2D,floorTextures[FL_METAL]);
			break;
		case RM_FREIGHTAIRLOCK:
		case RM_GALLEY:
			glBindTexture(GL_TEXTURE_2D,floorTextures[FL_METAL]);
			break;
		case RM_INDUSTRIALWORKSHOP:
		case RM_LABORATORY:
			glBindTexture(GL_TEXTURE_2D,floorTextures[FL_TILED]);
			break;
		case RM_LIBRARY:
		case RM_LIVINGQUARTERS:
			glBindTexture(GL_TEXTURE_2D,floorTextures[FL_METAL]);
			break;
		case RM_MININGCONTROLCENTER:
		case RM_OXYGENSCRUBBER:
		case RM_POWERFUSION:
			glBindTexture(GL_TEXTURE_2D,floorTextures[FL_METAL]);
			break;
		case RM_WAREHOUSE:
		case RM_WATERCOLLECTOR:
		case RM_WATERRECYCLING:
		default:
			//sets the texture to blank and the debug color
			glBindTexture(GL_TEXTURE_2D,0);
			glColor3f(0.5f,0.5f,0.5f);
			break;
		}

		//Begins drawing
		glBegin(GL_QUADS);

			//floor
			glTexCoord2f(0.98f,0.02f);		glVertex3f(iw,-ih,il);
			glTexCoord2f(0.98f,0.98f);		glVertex3f(iw,-ih,-il);
			glTexCoord2f(0.0f,0.98f);		glVertex3f(-w,-ih,-il);
			glTexCoord2f(0.0f,0.02f);		glVertex3f(-w,-ih,il);

		glEnd();

		//sets color to white if powered or red if unpowered
		if(powered)
			glColor3f(1.0f,1.0f,1.0f);
		else
			glColor3f(1.0f,0.25f,0.25f);

		//determine the texture from the building type
		switch(type)
		{
		case RM_HALLWAY:
		case RM_AIRLOCK:
		case RM_BATHROOM:
		case RM_COMMANDCENTER:
		case RM_FREIGHTAIRLOCK:
		case RM_GALLEY:
		case RM_INDUSTRIALWORKSHOP:
		case RM_LABORATORY:
		case RM_LIBRARY:
		case RM_LIVINGQUARTERS:
		case RM_MININGCONTROLCENTER:
		case RM_OXYGENSCRUBBER:
		case RM_POWERFUSION:
		case RM_WAREHOUSE:
		case RM_WATERCOLLECTOR:
		case RM_WATERRECYCLING:
		default:
			//sets the texture to blank and the debug color
			glBindTexture(GL_TEXTURE_2D,0);
			glColor3f(0.5f,0.5f,0.5f);
			break;
		}
		
		//Begins drawing
		glBegin(GL_QUADS);

			//ceiling
			glTexCoord2f(0.0f,0.98f);		glVertex3f(-w,ih,-il);
			glTexCoord2f(0.98f,0.98f);		glVertex3f(iw,ih,-il);
			glTexCoord2f(0.98f,0.02f);		glVertex3f(iw,ih,il);
			glTexCoord2f(0.0f,0.02f);		glVertex3f(-w,ih,il);

		glEnd();

		break;	//end of case 3
	case 2:
		//sets color to white
		glColor3f(1.0f,1.0f,1.0f);

		//determines the texture from the building type
		switch(type)
		{
		case RM_HALLWAY:
		case RM_AIRLOCK:
		case RM_BATHROOM:
		case RM_COMMANDCENTER:
		case RM_FREIGHTAIRLOCK:
		case RM_GALLEY:
		case RM_INDUSTRIALWORKSHOP:
		case RM_LABORATORY:
		case RM_LIBRARY:
		case RM_LIVINGQUARTERS:
		case RM_MININGCONTROLCENTER:
		case RM_OXYGENSCRUBBER:
		case RM_POWERFUSION:
		case RM_WAREHOUSE:
		case RM_WATERCOLLECTOR:
		case RM_WATERRECYCLING:
			glBindTexture(GL_TEXTURE_2D,outerTextures[0]);
			break;
		default:
			glBindTexture(GL_TEXTURE_2D,0);
			glColor3f(0.5f,0.5f,0.0f);
			break;
		}

		//If special is false, it is a corner, if true, it is a corridor
		if(!special)
		{
			//Begins drawing
			glBegin(GL_QUADS);

				//Outer wall front
				glTexCoord2f(1.0f,1.0f);		glVertex3f(w,h,l);
				glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,h,l);
				glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-h,l);
				glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-h,l);

				//Outer wall right
				glTexCoord2f(1.0f,1.0f);		glVertex3f(w,h,l);
				glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-h,l);
				glTexCoord2f(0.0f,0.0f);		glVertex3f(w,-h,-l);
				glTexCoord2f(0.0f,1.0f);		glVertex3f(w,h,-l);

			glEnd();

			//sets color to white if powered or red if unpowered
			if(powered)
				glColor3f(1.0f,1.0f,1.0f);
			else
				glColor3f(1.0f,0.25f,0.25f);

			//determins the texture from the building type
			switch(type)
			{
			case RM_HALLWAY:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_HALLWAY]);
				break;
			case RM_AIRLOCK:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_AIRLOCK]);
				break;
			case RM_BATHROOM:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_BATHROOM]);
				break;
			case RM_COMMANDCENTER:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_COMMANDCENTER]);
				break;
			case RM_FREIGHTAIRLOCK:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_FREIGHTAIRLOCK]);
				break;
			case RM_GALLEY:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_GALLEY]);
				break;
			case RM_INDUSTRIALWORKSHOP:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_INDUSTRIALWORKSHOP]);
				break;
			case RM_LABORATORY:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_LABORATORY]);
				break;
			case RM_LIBRARY:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_LIBRARY]);
				break;
			case RM_LIVINGQUARTERS:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_LIVINGQUARTERS]);
				break;
			case RM_MININGCONTROLCENTER:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_MININGCONTROLCENTER]);
				break;
			case RM_OXYGENSCRUBBER:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_OXYGENSCRUBBER]);
				break;
			case RM_POWERFUSION:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_POWERFUSION]);
				break;
			case RM_WAREHOUSE:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_WAREHOUSE]);
				break;
			case RM_WATERCOLLECTOR:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_WATERCOLLECTOR]);
				break;
			case RM_WATERRECYCLING:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_WATERRECYCLING]);
				break;
			default:
				glBindTexture(GL_TEXTURE_2D,0);
				glColor3f(0.0f,0.5f,0.0f);
				break;
			}

			//Begins drawing
			glBegin(GL_QUADS);

				//Inner wall front
				glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-ih,il);	
				glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,ih,il);	
				glTexCoord2f(0.98f,1.0f);		glVertex3f(iw,ih,il);	
				glTexCoord2f(0.98f,0.0f);		glVertex3f(iw,-ih,il);	
		
				//Inner wall right
				glTexCoord2f(0.02f,0.0f);		glVertex3f(iw,-ih,-l);
				glTexCoord2f(0.98f,0.0f);		glVertex3f(iw,-ih,il);
				glTexCoord2f(0.98f,1.0f);		glVertex3f(iw,ih,il);
				glTexCoord2f(0.02f,1.0f);		glVertex3f(iw,ih,-l);

			glEnd();

			//clears texture and sets color to slightly grey
			glBindTexture(GL_TEXTURE_2D,0);
			glColor3f(0.05f,0.05f,0.05f);
		
			//Begins drawing
			glBegin(GL_QUADS);

				//Upper wall front
				glVertex3f(-w,ih,il);
				glVertex3f(-w,h,l);
				glVertex3f(w,h,l);
				glVertex3f(w,ih,il);	

				//upper wall right
				glVertex3f(iw,ih,-l);
				glVertex3f(iw,ih,il);	
				glVertex3f(w,h,l);
				glVertex3f(w,h,-l);

			glEnd();

			//sets color to white if powered or red if unpowered
			if(powered)
				glColor3f(1.0f,1.0f,1.0f);
			else
				glColor3f(1.0f,0.25f,0.25f);

			//determine the texture from the building type
			switch(type)
			{
			case RM_HALLWAY:
				glBindTexture(GL_TEXTURE_2D,floorTextures[FL_GRATE]);
				break;
			case RM_AIRLOCK:
			case RM_BATHROOM:
				glBindTexture(GL_TEXTURE_2D,floorTextures[FL_TILED]);
				break;
			case RM_COMMANDCENTER:
				glBindTexture(GL_TEXTURE_2D,floorTextures[FL_METAL]);
				break;
			case RM_FREIGHTAIRLOCK:
			case RM_GALLEY:
				glBindTexture(GL_TEXTURE_2D,floorTextures[FL_METAL]);
				break;
			case RM_INDUSTRIALWORKSHOP:
			case RM_LABORATORY:
				glBindTexture(GL_TEXTURE_2D,floorTextures[FL_TILED]);
				break;
			case RM_LIBRARY:
			case RM_LIVINGQUARTERS:
				glBindTexture(GL_TEXTURE_2D,floorTextures[FL_METAL]);
				break;
			case RM_MININGCONTROLCENTER:
			case RM_OXYGENSCRUBBER:
			case RM_POWERFUSION:
				glBindTexture(GL_TEXTURE_2D,floorTextures[FL_METAL]);
				break;
			case RM_WAREHOUSE:
			case RM_WATERCOLLECTOR:
			case RM_WATERRECYCLING:
			default:
				//sets the texture to blank and the debug color
				glBindTexture(GL_TEXTURE_2D,0);
				glColor3f(0.5f,0.5f,0.5f);
				break;
			}

			//Begins drawing
			glBegin(GL_QUADS);

				//floor
				glTexCoord2f(0.02f,0.02f);		glVertex3f(iw,-ih,il);
				glTexCoord2f(0.02f,1.0f);		glVertex3f(iw,-ih,-l);
				glTexCoord2f(1.0f,1.0f);		glVertex3f(-w,-ih,-l);
				glTexCoord2f(1.0f,0.02f);		glVertex3f(-w,-ih,il);

			glEnd();
			
			//sets color to white if powered or red if unpowered
			if(powered)
				glColor3f(1.0f,1.0f,1.0f);
			else
				glColor3f(1.0f,0.25f,0.25f);

			//determine the texture from the building type
			switch(type)
			{
			case RM_HALLWAY:
			case RM_AIRLOCK:
			case RM_BATHROOM:
			case RM_COMMANDCENTER:
			case RM_FREIGHTAIRLOCK:
			case RM_GALLEY:
			case RM_INDUSTRIALWORKSHOP:
			case RM_LABORATORY:
			case RM_LIBRARY:
			case RM_LIVINGQUARTERS:
			case RM_MININGCONTROLCENTER:
			case RM_OXYGENSCRUBBER:
			case RM_POWERFUSION:
			case RM_WAREHOUSE:
			case RM_WATERCOLLECTOR:
			case RM_WATERRECYCLING:
			default:
				//sets the texture to blank and the debug color
				glBindTexture(GL_TEXTURE_2D,0);
				glColor3f(0.5f,0.5f,0.5f);
				break;
			}

			//Begins drawing
			glBegin(GL_QUADS);

				//ceiling
				glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,ih,-l);
				glTexCoord2f(0.98f,1.0f);		glVertex3f(iw,ih,-l);
				glTexCoord2f(0.98f,0.02f);		glVertex3f(iw,ih,il);
				glTexCoord2f(0.0f,0.02f);		glVertex3f(-w,ih,il);

			glEnd();
		}
		else
		{
			//Begins drawing
			glBegin(GL_QUADS);

				//Outer wall front
				glTexCoord2f(1.0f,1.0f);		glVertex3f(w,h,l);
				glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,h,l);
				glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-h,l);
				glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-h,l);

				//Outer wall back
				glTexCoord2f(1.0f,1.0f);		glVertex3f(w,h,-l);
				glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-h,-l);
				glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-h,-l);
				glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,h,-l);

			glEnd();

			//sets color to white if powered or red if unpowered
			if(powered)
				glColor3f(1.0f,1.0f,1.0f);
			else
				glColor3f(1.0f,0.25f,0.25f);

			//determins the texture from the building type
			switch(type)
			{
			case RM_HALLWAY:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_HALLWAY]);
				break;
			case RM_AIRLOCK:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_AIRLOCK]);
				break;
			case RM_BATHROOM:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_BATHROOM]);
				break;
			case RM_COMMANDCENTER:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_COMMANDCENTER]);
				break;
			case RM_FREIGHTAIRLOCK:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_FREIGHTAIRLOCK]);
				break;
			case RM_GALLEY:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_GALLEY]);
				break;
			case RM_INDUSTRIALWORKSHOP:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_INDUSTRIALWORKSHOP]);
				break;
			case RM_LABORATORY:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_LABORATORY]);
				break;
			case RM_LIBRARY:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_LIBRARY]);
				break;
			case RM_LIVINGQUARTERS:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_LIVINGQUARTERS]);
				break;
			case RM_MININGCONTROLCENTER:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_MININGCONTROLCENTER]);
				break;
			case RM_OXYGENSCRUBBER:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_OXYGENSCRUBBER]);
				break;
			case RM_POWERFUSION:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_POWERFUSION]);
				break;
			case RM_WAREHOUSE:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_WAREHOUSE]);
				break;
			case RM_WATERCOLLECTOR:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_WATERCOLLECTOR]);
				break;
			case RM_WATERRECYCLING:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_WATERRECYCLING]);
				break;
			default:
				glBindTexture(GL_TEXTURE_2D,0);
				glColor3f(0.0f,0.5f,0.0f);
				break;
			}

			//Begins drawing
			glBegin(GL_QUADS);

				//Inner wall front
				glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-ih,il);
				glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,ih,il);
				glTexCoord2f(1.0f,1.0f);		glVertex3f(w,ih,il);
				glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-ih,il);
		
				//Inner wall right
				glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-ih,-il);
				glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-ih,-il);
				glTexCoord2f(1.0f,1.0f);		glVertex3f(w,ih,-il);
				glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,ih,-il);

			glEnd();

			//clears texture and sets color to slightly grey
			glBindTexture(GL_TEXTURE_2D,0);
			glColor3f(0.05f,0.05f,0.05f);
		
			//Begins drawing
			glBegin(GL_QUADS);

				//Upper wall front
				glVertex3f(-w,ih,il);
				glVertex3f(-w,h,l);
				glVertex3f(w,h,l);
				glVertex3f(w,ih,il);	

				//Upper wall front
				glVertex3f(-w,ih,-il);
				glVertex3f(w,ih,-il);
				glVertex3f(w,h,-l);
				glVertex3f(-w,h,-l);	

			glEnd();

			//sets color to white if powered or red if unpowered
			if(powered)
				glColor3f(1.0f,1.0f,1.0f);
			else
				glColor3f(1.0f,0.25f,0.25f);

			//determine the texture from the building type
			switch(type)
			{
			case RM_HALLWAY:
				glBindTexture(GL_TEXTURE_2D,floorTextures[FL_GRATE]);
				break;
			case RM_AIRLOCK:
			case RM_BATHROOM:
				glBindTexture(GL_TEXTURE_2D,floorTextures[FL_TILED]);
				break;
			case RM_COMMANDCENTER:
				glBindTexture(GL_TEXTURE_2D,floorTextures[FL_METAL]);
				break;
			case RM_FREIGHTAIRLOCK:
			case RM_GALLEY:
				glBindTexture(GL_TEXTURE_2D,floorTextures[FL_METAL]);
				break;
			case RM_INDUSTRIALWORKSHOP:
			case RM_LABORATORY:
				glBindTexture(GL_TEXTURE_2D,floorTextures[FL_TILED]);
				break;
			case RM_LIBRARY:
			case RM_LIVINGQUARTERS:
				glBindTexture(GL_TEXTURE_2D,floorTextures[FL_METAL]);
				break;
			case RM_MININGCONTROLCENTER:
			case RM_OXYGENSCRUBBER:
			case RM_POWERFUSION:
				glBindTexture(GL_TEXTURE_2D,floorTextures[FL_METAL]);
				break;
			case RM_WAREHOUSE:
			case RM_WATERCOLLECTOR:
			case RM_WATERRECYCLING:
			default:
				//sets the texture to blank and the debug color
				glBindTexture(GL_TEXTURE_2D,0);
				glColor3f(0.5f,0.5f,0.5f);
				break;
			}

			//Begins drawing
			glBegin(GL_QUADS);

				//floor
				glTexCoord2f(1.0f,0.98f);		glVertex3f(w,-ih,il);
				glTexCoord2f(1.0f,0.02f);		glVertex3f(w,-ih,-il);
				glTexCoord2f(0.0f,0.02f);		glVertex3f(-w,-ih,-il);
				glTexCoord2f(0.0f,0.98f);		glVertex3f(-w,-ih,il);

			glEnd();
			
			//sets color to white if powered or red if unpowered
			if(powered)
				glColor3f(1.0f,1.0f,1.0f);
			else
				glColor3f(1.0f,0.25f,0.25f);

			//determine the texture from the building type
			switch(type)
			{
			case RM_HALLWAY:
			case RM_AIRLOCK:
			case RM_BATHROOM:
			case RM_COMMANDCENTER:
			case RM_FREIGHTAIRLOCK:
			case RM_GALLEY:
			case RM_INDUSTRIALWORKSHOP:
			case RM_LABORATORY:
			case RM_LIBRARY:
			case RM_LIVINGQUARTERS:
			case RM_MININGCONTROLCENTER:
			case RM_OXYGENSCRUBBER:
			case RM_POWERFUSION:
			case RM_WAREHOUSE:
			case RM_WATERCOLLECTOR:
			case RM_WATERRECYCLING:
			default:
				//sets the texture to blank and the debug color
				glBindTexture(GL_TEXTURE_2D,0);
				glColor3f(0.5f,0.5f,0.5f);
				break;
			}

			//Begins drawing
			glBegin(GL_QUADS);

				//ceiling
				glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,ih,-il);
				glTexCoord2f(0.98f,1.0f);		glVertex3f(w,ih,-il);
				glTexCoord2f(0.98f,0.02f);		glVertex3f(w,ih,il);
				glTexCoord2f(0.0f,0.02f);		glVertex3f(-w,ih,il);

			glEnd();
		}

		break;	//end of case 2
	case 1:
		//sets color to white
		glColor3f(1.0f,1.0f,1.0f);

		//determines the texture from the building type
		switch(type)
		{
		case RM_HALLWAY:
		case RM_AIRLOCK:
		case RM_BATHROOM:
		case RM_COMMANDCENTER:
		case RM_FREIGHTAIRLOCK:
		case RM_GALLEY:
		case RM_INDUSTRIALWORKSHOP:
		case RM_LABORATORY:
		case RM_LIBRARY:
		case RM_LIVINGQUARTERS:
		case RM_MININGCONTROLCENTER:
		case RM_OXYGENSCRUBBER:
		case RM_POWERFUSION:
		case RM_WAREHOUSE:
		case RM_WATERCOLLECTOR:
		case RM_WATERRECYCLING:
			glBindTexture(GL_TEXTURE_2D,outerTextures[0]);
			break;
		default:
			glBindTexture(GL_TEXTURE_2D,0);
			glColor3f(0.5f,0.5f,0.0f);
			break;
		}
		//Begins drawing
		glBegin(GL_QUADS);

			//Outer wall
			glTexCoord2f(1.0f,1.0f);		glVertex3f(w,h,l);
			glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,h,l);
			glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-h,l);
			glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-h,l);

		glEnd();

		//sets color to white if powered or red if unpowered
		if(powered)
			glColor3f(1.0f,1.0f,1.0f);
		else
			glColor3f(1.0f,0.25f,0.25f);

		//determins the texture from the building type
		switch(type)
		{
		case RM_HALLWAY:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_HALLWAY]);
			break;
		case RM_AIRLOCK:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_AIRLOCK]);
			break;
		case RM_BATHROOM:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_BATHROOM]);
			break;
		case RM_COMMANDCENTER:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_COMMANDCENTER]);
			break;
		case RM_FREIGHTAIRLOCK:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_FREIGHTAIRLOCK]);
			break;
		case RM_GALLEY:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_GALLEY]);
			break;
		case RM_INDUSTRIALWORKSHOP:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_INDUSTRIALWORKSHOP]);
			break;
		case RM_LABORATORY:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_LABORATORY]);
			break;
		case RM_LIBRARY:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_LIBRARY]);
			break;
		case RM_LIVINGQUARTERS:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_LIVINGQUARTERS]);
			break;
		case RM_MININGCONTROLCENTER:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_MININGCONTROLCENTER]);
			break;
		case RM_OXYGENSCRUBBER:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_OXYGENSCRUBBER]);
			break;
		case RM_POWERFUSION:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_POWERFUSION]);
			break;
		case RM_WAREHOUSE:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_WAREHOUSE]);
			break;
		case RM_WATERCOLLECTOR:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_WATERCOLLECTOR]);
			break;
		case RM_WATERRECYCLING:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_WATERRECYCLING]);
			break;
		default:
			glBindTexture(GL_TEXTURE_2D,0);
			glColor3f(0.0f,0.5f,0.0f);
			break;
		}

		//Begins drawing
		glBegin(GL_QUADS);

			//Inner wall
			glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-ih,il);
			glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,ih,il);
			glTexCoord2f(1.0f,1.0f);		glVertex3f(w,ih,il);
			glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-ih,il);
		
		glEnd();

		//clears texture and sets color to slightly grey
		glBindTexture(GL_TEXTURE_2D,0);
		glColor3f(0.05f,0.05f,0.05f);
		
		//Begins drawing
		glBegin(GL_QUADS);

			//Upper wall
			glVertex3f(-w,ih,il);
			glVertex3f(-w,h,l);
			glVertex3f(w,h,l);
			glVertex3f(w,ih,il);	

		glEnd();

		//sets color to white if powered or red if unpowered
		if(powered)
			glColor3f(1.0f,1.0f,1.0f);
		else
			glColor3f(1.0f,0.25f,0.25f);

		//determine the texture from the building type
		switch(type)
		{
		case RM_HALLWAY:
			glBindTexture(GL_TEXTURE_2D,floorTextures[FL_GRATE]);
			break;
		case RM_AIRLOCK:
		case RM_BATHROOM:
			glBindTexture(GL_TEXTURE_2D,floorTextures[FL_TILED]);
			break;
		case RM_COMMANDCENTER:
			glBindTexture(GL_TEXTURE_2D,floorTextures[FL_METAL]);
			break;
		case RM_FREIGHTAIRLOCK:
		case RM_GALLEY:
			glBindTexture(GL_TEXTURE_2D,floorTextures[FL_METAL]);
			break;
		case RM_INDUSTRIALWORKSHOP:
		case RM_LABORATORY:
			glBindTexture(GL_TEXTURE_2D,floorTextures[FL_TILED]);
			break;
		case RM_LIBRARY:
		case RM_LIVINGQUARTERS:
			glBindTexture(GL_TEXTURE_2D,floorTextures[FL_METAL]);
			break;
		case RM_MININGCONTROLCENTER:
		case RM_OXYGENSCRUBBER:
		case RM_POWERFUSION:
			glBindTexture(GL_TEXTURE_2D,floorTextures[FL_METAL]);
			break;
		case RM_WAREHOUSE:
		case RM_WATERCOLLECTOR:
		case RM_WATERRECYCLING:
		default:
			//sets the texture to blank and the debug color
			glBindTexture(GL_TEXTURE_2D,0);
			glColor3f(0.5f,0.5f,0.5f);
			break;
		}

		//Begins drawing
		glBegin(GL_QUADS);

			//floor
			glTexCoord2f(1.0f,0.02f);		glVertex3f(w,-ih,il);
			glTexCoord2f(1.0f,1.0f);		glVertex3f(w,-ih,-l);
			glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,-ih,-l);
			glTexCoord2f(0.0f,0.02f);		glVertex3f(-w,-ih,il);

		glEnd();
		
		//sets color to white if powered or red if unpowered
		if(powered)
			glColor3f(1.0f,1.0f,1.0f);
		else
			glColor3f(1.0f,0.25f,0.25f);

		//determine the texture from the building type
		switch(type)
		{
		case RM_HALLWAY:
		case RM_AIRLOCK:
		case RM_BATHROOM:
		case RM_COMMANDCENTER:
		case RM_FREIGHTAIRLOCK:
		case RM_GALLEY:
		case RM_INDUSTRIALWORKSHOP:
		case RM_LABORATORY:
		case RM_LIBRARY:
		case RM_LIVINGQUARTERS:
		case RM_MININGCONTROLCENTER:
		case RM_OXYGENSCRUBBER:
		case RM_POWERFUSION:
		case RM_WAREHOUSE:
		case RM_WATERCOLLECTOR:
		case RM_WATERRECYCLING:
		default:
			//sets the texture to blank and the debug color
			glBindTexture(GL_TEXTURE_2D,0);
			glColor3f(0.5f,0.5f,0.5f);
			break;
		}

		//Begins drawing
		glBegin(GL_QUADS);

			//ceiling
			glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,ih,-l);
			glTexCoord2f(1.0f,1.0f);		glVertex3f(w,ih,-l);
			glTexCoord2f(1.0f,0.04f);		glVertex3f(w,ih,il);
			glTexCoord2f(0.0f,0.04f);		glVertex3f(-w,ih,il);

		glEnd();
		break;	//end case 1 wall switch
	default:	//case 0 usually

		//sets color to white if powered or red if unpowered
		if(powered)
			glColor3f(1.0f,1.0f,1.0f);
		else
			glColor3f(1.0f,0.25f,0.25f);

		//determine the texture from the building type
		switch(type)
		{
		case RM_HALLWAY:
			glBindTexture(GL_TEXTURE_2D,floorTextures[FL_GRATE]);
			break;
		case RM_AIRLOCK:
		case RM_BATHROOM:
			glBindTexture(GL_TEXTURE_2D,floorTextures[FL_TILED]);
			break;
		case RM_COMMANDCENTER:
			glBindTexture(GL_TEXTURE_2D,floorTextures[FL_METAL]);
			break;
		case RM_FREIGHTAIRLOCK:
		case RM_GALLEY:
			glBindTexture(GL_TEXTURE_2D,floorTextures[FL_METAL]);
			break;
		case RM_INDUSTRIALWORKSHOP:
		case RM_LABORATORY:
			glBindTexture(GL_TEXTURE_2D,floorTextures[FL_TILED]);
			break;
		case RM_LIBRARY:
		case RM_LIVINGQUARTERS:
			glBindTexture(GL_TEXTURE_2D,floorTextures[FL_METAL]);
			break;
		case RM_MININGCONTROLCENTER:
		case RM_OXYGENSCRUBBER:
		case RM_POWERFUSION:
			glBindTexture(GL_TEXTURE_2D,floorTextures[FL_METAL]);
			break;
		case RM_WAREHOUSE:
		case RM_WATERCOLLECTOR:
		case RM_WATERRECYCLING:
		default:
			//sets the texture to blank and the debug color
			glBindTexture(GL_TEXTURE_2D,0);
			glColor3f(0.5f,0.5f,0.5f);
			break;
		}
		
		//Begins drawing
		glBegin(GL_QUADS);

			//floor
			glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-ih,l);
			glTexCoord2f(1.0f,1.0f);		glVertex3f(w,-ih,-l);
			glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,-ih,-l);
			glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-ih,l);

		glEnd();
		
		//sets color to white if powered or red if unpowered
		if(powered)
			glColor3f(1.0f,1.0f,1.0f);
		else
			glColor3f(1.0f,0.25f,0.25f);

		//determine the texture from the building type
		switch(type)
		{
		case RM_HALLWAY:
		case RM_AIRLOCK:
		case RM_BATHROOM:
		case RM_COMMANDCENTER:
		case RM_FREIGHTAIRLOCK:
		case RM_GALLEY:
		case RM_INDUSTRIALWORKSHOP:
		case RM_LABORATORY:
		case RM_LIBRARY:
		case RM_LIVINGQUARTERS:
		case RM_MININGCONTROLCENTER:
		case RM_OXYGENSCRUBBER:
		case RM_POWERFUSION:
		case RM_WAREHOUSE:
		case RM_WATERCOLLECTOR:
		case RM_WATERRECYCLING:
		default:
			//sets the texture to blank and the debug color
			glBindTexture(GL_TEXTURE_2D,0);
			glColor3f(0.5f,0.5f,0.5f);
			break;
		}
		
		//Begins drawing
		glBegin(GL_QUADS);

			//ceiling
			glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,ih,-l);
			glTexCoord2f(1.0f,1.0f);		glVertex3f(w,ih,-l);
			glTexCoord2f(1.0f,0.0f);		glVertex3f(w,ih,l);
			glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,ih,l);

		glEnd();
		break;		//end of default case
	}

	glPopMatrix();
	glPopMatrix();
}

/****************************
* RenderTestBuilding - individual block
****************************/
void OpenGLRenderer::RenderTestBuilding(Building* const building, tBOOL powered)
{
	GLfloat ih(0.48f);
	GLfloat iw(0.48f);
	GLfloat il(0.48f); //these are the inner dimensions for the room. Each room will be slightly smaller than a block.

	GLfloat h(0.5f);
	GLfloat w(0.5f);
	GLfloat l(0.5f);
	
	s32 door=building->getDoorPosition();
	s32 corner=building->getCornerPosition();

	glPushMatrix();
	glTranslatef(GLfloat(building->getPos().x),GLfloat(building->getPos().y),GLfloat(building->getPos().z));
	glPushMatrix();
	
	//draws ceiling and floor for texture accuracy
	
	//sets color to white if powered or red if unpowered
	if(powered)
		glColor3f(1.0f,1.0f,1.0f);
	else
		glColor3f(1.0f,0.25f,0.25f);

	//determine the texture from the building building->getRoomType()
	switch(building->getRoomType())
	{
	case RM_GHOST:
		glBindTexture(GL_TEXTURE_2D,0);
		glColor4f(0.6f,0.6f,0.6f,0.5f);
		break;
	case RM_HALLWAY:
		glBindTexture(GL_TEXTURE_2D,floorTextures[FL_GRATE]);
		break;
	case RM_AIRLOCK:
	case RM_BATHROOM:
		glBindTexture(GL_TEXTURE_2D,floorTextures[FL_TILED]);
		break;
	case RM_COMMANDCENTER:
		glBindTexture(GL_TEXTURE_2D,floorTextures[FL_METAL]);
		break;
	case RM_FREIGHTAIRLOCK:
	case RM_GALLEY:
	case RM_INDUSTRIALWORKSHOP:
	case RM_LABORATORY:
	case RM_LIBRARY:
	case RM_LIVINGQUARTERS:
		glBindTexture(GL_TEXTURE_2D,floorTextures[FL_METAL]);
		break;
	case RM_MININGCONTROLCENTER:
	case RM_OXYGENSCRUBBER:
	case RM_POWERFUSION:
	case RM_WAREHOUSE:
	case RM_WATERCOLLECTOR:
	case RM_WATERRECYCLING:
	default:
		//sets the texture to blank and the debug color
		glBindTexture(GL_TEXTURE_2D,0);
		glColor3f(0.5f,0.5f,0.5f);
		break;
	}

	//Begins drawing
	glBegin(GL_QUADS);

		//floor
		glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-ih,l);
		glTexCoord2f(1.0f,1.0f);		glVertex3f(w,-ih,-l);
		glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,-ih,-l);
		glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-ih,l);

	glEnd();

	//sets color to white if powered or red if unpowered
	if(powered)
		glColor3f(1.0f,1.0f,1.0f);
	else
		glColor3f(1.0f,0.25f,0.25f);

	//determine the texture from the building building->getRoomType()
	switch(building->getRoomType())
	{
	case RM_GHOST:
		glBindTexture(GL_TEXTURE_2D,0);
		glColor4f(0.6f,0.6f,0.6f,0.5f);
		break;
	case RM_HALLWAY:
	case RM_AIRLOCK:
	case RM_BATHROOM:
	case RM_COMMANDCENTER:
	case RM_FREIGHTAIRLOCK:
	case RM_GALLEY:
	case RM_INDUSTRIALWORKSHOP:
	case RM_LABORATORY:
	case RM_LIBRARY:
	case RM_LIVINGQUARTERS:
	case RM_MININGCONTROLCENTER:
	case RM_OXYGENSCRUBBER:
	case RM_POWERFUSION:
	case RM_WAREHOUSE:
	case RM_WATERCOLLECTOR:
	case RM_WATERRECYCLING:
	default:
		//sets the texture to blank and the debug color
		glBindTexture(GL_TEXTURE_2D,0);
		glColor3f(0.5f,0.5f,0.5f);
		break;
	}

	//Begins drawing
	glBegin(GL_QUADS);
		//ceiling
		glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,ih,-l);
		glTexCoord2f(1.0f,1.0f);		glVertex3f(w,ih,-l);
		glTexCoord2f(1.0f,0.0f);		glVertex3f(w,ih,l);
		glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,ih,l);

	glEnd();

	glRotatef((GLfloat)building->getOrient(),0.0f,1.0f,0.0f);

	
	//main drawing switch
	switch(building->getWalls())
	{
	case 4:
		//sets color to white
		glColor3f(1.0f,1.0f,1.0f);

		//determines the texture from the building building->getRoomType()
		switch(building->getRoomType())
		{
		case RM_GHOST:
			glBindTexture(GL_TEXTURE_2D,0);
			glColor4f(0.6f,0.6f,0.6f,0.5f);
			break;
		case RM_HALLWAY:
		case RM_AIRLOCK:
		case RM_BATHROOM:
		case RM_COMMANDCENTER:
		case RM_FREIGHTAIRLOCK:
		case RM_GALLEY:
		case RM_INDUSTRIALWORKSHOP:
		case RM_LABORATORY:
		case RM_LIBRARY:
		case RM_LIVINGQUARTERS:
		case RM_MININGCONTROLCENTER:
		case RM_OXYGENSCRUBBER:
		case RM_POWERFUSION:
		case RM_WAREHOUSE:
		case RM_WATERCOLLECTOR:
		case RM_WATERRECYCLING:
			glBindTexture(GL_TEXTURE_2D,outerTextures[0]);
			break;
		default:
			glBindTexture(GL_TEXTURE_2D,0);
			glColor3f(0.5f,0.5f,0.0f);
			break;
		}

		//Begins drawing
		glBegin(GL_QUADS);
			if(	door!=RM_DOOR8 &&
				door!=RM_DOOR86 &&
				door!=RM_DOOR862 &&
				door!=RM_DOOR864 &&
				door!=RM_DOOR8624 &&
				door!=RM_DOOR82 &&
				door!=RM_DOOR824 &&
				door!=RM_DOOR84)
			{

				//Outer wall front
				glTexCoord2f(1.0f,1.0f);		glVertex3f(w,h,l);
				glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,h,l);
				glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-h,l);
				glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-h,l);
			}
			else
			{
				//Right side of door section
				glTexCoord2f(1.0f,1.0f);		glVertex3f(w,h,l);
				glTexCoord2f(0.75f,1.0f);		glVertex3f(w/2,h,l);
				glTexCoord2f(0.75f,0.0f);		glVertex3f(w/2,-h,l);
				glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-h,l);
				//Top side of door section
				glTexCoord2f(0.75f,1.0f);		glVertex3f(w/2,h,l);
				glTexCoord2f(0.25f,1.0f);		glVertex3f(-w/2,h,l);
				glTexCoord2f(0.25f,0.75f);		glVertex3f(-w/2,h/2,l);
				glTexCoord2f(0.75f,0.75f);		glVertex3f(w/2,h/2,l);
				//left side of door section
				glTexCoord2f(0.25f,1.0f);		glVertex3f(-w/2,h,l);
				glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,h,l);
				glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-h,l);
				glTexCoord2f(0.25f,0.0f);		glVertex3f(-w/2,-h,l);
			}
			
			if(door!=RM_DOOR4 &&
				door!=RM_DOOR24 &&
				door!=RM_DOOR64 &&
				door!=RM_DOOR624 &&
				door!=RM_DOOR84 &&
				door!=RM_DOOR824 &&
				door!=RM_DOOR8624 &&
				door!=RM_DOOR864)
			{
				//Outer wall left
				glTexCoord2f(1.0f,1.0f);		glVertex3f(w,h,l);
				glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-h,l);
				glTexCoord2f(0.0f,0.0f);		glVertex3f(w,-h,-l);
				glTexCoord2f(0.0f,1.0f);		glVertex3f(w,h,-l);
			}
			else
			{
				//Right side of door section
				glTexCoord2f(1.0f,1.0f);		glVertex3f(w,h,l);
				glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-h,l);
				glTexCoord2f(0.75f,0.0f);		glVertex3f(w,-h,l/2);
				glTexCoord2f(0.75f,1.0f);		glVertex3f(w,h,l/2);
				//Top side of door section
				glTexCoord2f(0.75f,1.0f);		glVertex3f(w,h,l/2);
				glTexCoord2f(0.75f,0.75f);		glVertex3f(w,h/2,l/2);
				glTexCoord2f(0.25f,0.75f);		glVertex3f(w,h/2,-l/2);
				glTexCoord2f(0.25f,1.0f);		glVertex3f(w,h,-l/2);
				//left side of door section
				glTexCoord2f(0.25f,1.0f);		glVertex3f(w,h,-l/2);
				glTexCoord2f(0.25f,0.0f);		glVertex3f(w,-h,-l/2);
				glTexCoord2f(0.0f,0.0f);		glVertex3f(w,-h,-l);
				glTexCoord2f(0.0f,1.0f);		glVertex3f(w,h,-l);
			}

			if( door!=RM_DOOR2 &&
				door!=RM_DOOR24 &&
				door!=RM_DOOR624 &&
				door!=RM_DOOR62 &&
				door!=RM_DOOR824 &&
				door!=RM_DOOR82 &&
				door!=RM_DOOR8624 &&
				door!=RM_DOOR862)
			{
				//Outer wall back
				glTexCoord2f(1.0f,1.0f);		glVertex3f(w,h,-l);
				glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-h,-l);
				glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-h,-l);
				glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,h,-l);
			}
			else
			{
				//Right side of door section
				glTexCoord2f(1.0f,1.0f);		glVertex3f(w,h,-l);
				glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-h,-l);
				glTexCoord2f(0.75f,0.0f);		glVertex3f(w/2,-h,-l);
				glTexCoord2f(0.75f,1.0f);		glVertex3f(w/2,h,-l);
				//Top side of door section
				glTexCoord2f(0.75f,1.0f);		glVertex3f(w/2,h,-l);
				glTexCoord2f(0.75f,0.75f);		glVertex3f(w/2,h/2,-l);
				glTexCoord2f(0.25f,0.75f);		glVertex3f(-w/2,h/2,-l);
				glTexCoord2f(0.25f,1.0f);		glVertex3f(-w/2,h,-l);
				//left side of door section
				glTexCoord2f(0.25f,1.0f);		glVertex3f(-w/2,h,-l);
				glTexCoord2f(0.25f,0.0f);		glVertex3f(-w/2,-h,-l);
				glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-h,-l);
				glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,h,-l);
			}
			
			if( door!=RM_DOOR64 &&
				door!=RM_DOOR624 &&
				door!=RM_DOOR62 &&
				door!=RM_DOOR6 &&
				door!=RM_DOOR8624 &&
				door!=RM_DOOR864 &&
				door!=RM_DOOR862 &&
				door!=RM_DOOR86)
			{
				//Outer wall right
				glTexCoord2f(1.0f,1.0f);		glVertex3f(-w,h,l);
				glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,h,-l);
				glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-h,-l);
				glTexCoord2f(1.0f,0.0f);		glVertex3f(-w,-h,l);
			}
			else
			{
				//Right side of door section
				glTexCoord2f(1.0f,1.0f);		glVertex3f(-w,h,l);
				glTexCoord2f(0.75f,1.0f);		glVertex3f(-w,h,l/2);
				glTexCoord2f(0.75f,0.0f);		glVertex3f(-w,-h,l/2);
				glTexCoord2f(1.0f,0.0f);		glVertex3f(-w,-h,l);
				//Top side of door section
				glTexCoord2f(0.75f,1.0f);		glVertex3f(-w,h,l/2);
				glTexCoord2f(0.25f,1.0f);		glVertex3f(-w,h,-l/2);
				glTexCoord2f(0.25f,0.75f);		glVertex3f(-w,h/2,-l/2);
				glTexCoord2f(0.75f,0.75f);		glVertex3f(-w,h/2,l/2);
				//left side of door section
				glTexCoord2f(0.25f,1.0f);		glVertex3f(-w,h,-l/2);
				glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,h,-l);
				glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-h,-l);
				glTexCoord2f(0.25f,0.0f);		glVertex3f(-w,-h,-l/2);
			}
		glEnd();

		//sets color to white if powered or red if unpowered
		if(powered)
			glColor3f(1.0f,1.0f,1.0f);
		else
			glColor3f(1.0f,0.25f,0.25f);

		//determins the texture from the building building->getRoomType()
		switch(building->getRoomType())
		{
		case RM_GHOST:
			glBindTexture(GL_TEXTURE_2D,0);
			glColor4f(0.6f,0.6f,0.6f,0.5f);
			break;
		case RM_HALLWAY:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_HALLWAY]);
			break;
		case RM_AIRLOCK:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_AIRLOCK]);
			break;
		case RM_BATHROOM:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_BATHROOM]);
			break;
		case RM_COMMANDCENTER:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_COMMANDCENTER]);
			break;
		case RM_FREIGHTAIRLOCK:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_FREIGHTAIRLOCK]);
			break;
		case RM_GALLEY:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_GALLEY]);
			break;
		case RM_INDUSTRIALWORKSHOP:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_INDUSTRIALWORKSHOP]);
			break;
		case RM_LABORATORY:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_LABORATORY]);
			break;
		case RM_LIBRARY:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_LIBRARY]);
			break;
		case RM_LIVINGQUARTERS:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_LIVINGQUARTERS]);
			break;
		case RM_MININGCONTROLCENTER:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_MININGCONTROLCENTER]);
			break;
		case RM_OXYGENSCRUBBER:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_OXYGENSCRUBBER]);
			break;
		case RM_POWERFUSION:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_POWERFUSION]);
			break;
		case RM_WAREHOUSE:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_WAREHOUSE]);
			break;
		case RM_WATERCOLLECTOR:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_WATERCOLLECTOR]);
			break;
		case RM_WATERRECYCLING:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_WATERRECYCLING]);
			break;
		default:
			glBindTexture(GL_TEXTURE_2D,0);
			glColor3f(0.0f,0.5f,0.0f);
			break;
		}

		//Begins drawing
		glBegin(GL_QUADS);
			if(	door!=RM_DOOR8 &&
				door!=RM_DOOR86 &&
				door!=RM_DOOR862 &&
				door!=RM_DOOR864 &&
				door!=RM_DOOR8624 &&
				door!=RM_DOOR82 &&
				door!=RM_DOOR824 &&
				door!=RM_DOOR84)
			{
				//Inner wall front
				glTexCoord2f(0.0f,0.04f);		glVertex3f(-iw,-ih,il);
				glTexCoord2f(0.0f,0.96f);		glVertex3f(-iw,ih,il);
				glTexCoord2f(0.96f,0.96f);		glVertex3f(iw,ih,il);
				glTexCoord2f(0.96f,0.04f);		glVertex3f(iw,-ih,il);
			}
			else
			{
				//Right side of door section
				glTexCoord2f(1.0f,1.0f);		glVertex3f(w,ih,l);
				glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-ih,l);
				glTexCoord2f(0.75f,0.0f);		glVertex3f(w/2,-ih,l);
				glTexCoord2f(0.75f,1.0f);		glVertex3f(w/2,ih,l);
				//Top side of door section
				glTexCoord2f(0.75f,1.0f);		glVertex3f(w/2,ih,l);
				glTexCoord2f(0.75f,0.75f);		glVertex3f(w/2,ih/2,l);
				glTexCoord2f(0.25f,0.75f);		glVertex3f(-w/2,ih/2,l);
				glTexCoord2f(0.25f,1.0f);		glVertex3f(-w/2,ih,l);
				//left side of door section
				glTexCoord2f(0.25f,1.0f);		glVertex3f(-w/2,ih,l);
				glTexCoord2f(0.25f,0.0f);		glVertex3f(-w/2,-ih,l);
				glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-ih,l);
				glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,ih,l);
			}
		
			if( door!=RM_DOOR4 &&
				door!=RM_DOOR24 &&
				door!=RM_DOOR64 &&
				door!=RM_DOOR624 &&
				door!=RM_DOOR84 &&
				door!=RM_DOOR824 &&
				door!=RM_DOOR8624 &&
				door!=RM_DOOR864)
			{
				//Inner wall left
				glTexCoord2f(0.04f,0.04f);		glVertex3f(w,-ih,-l);
				glTexCoord2f(0.96f,0.04f);		glVertex3f(w,-ih,l);
				glTexCoord2f(0.96f,0.96f);		glVertex3f(w,ih,l);
				glTexCoord2f(0.04f,0.96f);		glVertex3f(w,ih,-l);
			}
			else
			{
				//Right side of door section
				glTexCoord2f(1.0f,1.0f);		glVertex3f(w,ih,l);
				glTexCoord2f(0.75f,1.0f);		glVertex3f(w,ih,l/2);
				glTexCoord2f(0.75f,0.0f);		glVertex3f(w,-ih,l/2);
				glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-ih,l);
				//Top side of door section
				glTexCoord2f(0.75f,1.0f);		glVertex3f(w,ih,l/2);
				glTexCoord2f(0.25f,1.0f);		glVertex3f(w,ih,-l/2);
				glTexCoord2f(0.25f,0.75f);		glVertex3f(w,ih/2,-l/2);
				glTexCoord2f(0.75f,0.75f);		glVertex3f(w,ih/2,l/2);
				//left side of door section
				glTexCoord2f(0.25f,1.0f);		glVertex3f(w,ih,-l/2);
				glTexCoord2f(0.0f,1.0f);		glVertex3f(w,ih,-l);
				glTexCoord2f(0.0f,0.0f);		glVertex3f(w,-ih,-l);
				glTexCoord2f(0.25f,0.0f);		glVertex3f(w,-ih,-l/2);
			}

			if( door!=RM_DOOR2 &&
				door!=RM_DOOR24 &&
				door!=RM_DOOR624 &&
				door!=RM_DOOR62 &&
				door!=RM_DOOR824 &&
				door!=RM_DOOR82 &&
				door!=RM_DOOR8624 &&
				door!=RM_DOOR862)
			{
				//Inner wall back
				glTexCoord2f(0.0f,0.04f);		glVertex3f(-w,-ih,-l);
				glTexCoord2f(0.96f,0.04f);		glVertex3f(w,-ih,-l);
				glTexCoord2f(0.96f,0.96f);		glVertex3f(w,ih,-l);
				glTexCoord2f(0.0f,0.96f);		glVertex3f(-w,ih,-l);
			}
			else
			{
				//Right side of door section
				glTexCoord2f(1.0f,1.0f);		glVertex3f(w,ih,-l);
				glTexCoord2f(0.75f,1.0f);		glVertex3f(w/2,ih,-l);
				glTexCoord2f(0.75f,0.0f);		glVertex3f(w/2,-ih,-l);
				glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-ih,-l);
				//Top side of door section
				glTexCoord2f(0.75f,1.0f);		glVertex3f(w/2,ih,-l);
				glTexCoord2f(0.25f,1.0f);		glVertex3f(-w/2,ih,-l);
				glTexCoord2f(0.25f,0.75f);		glVertex3f(-w/2,ih/2,-l);
				glTexCoord2f(0.75f,0.75f);		glVertex3f(w/2,ih/2,-l);
				//left side of door section
				glTexCoord2f(0.25f,1.0f);		glVertex3f(-w/2,ih,-l);
				glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,ih,-l);
				glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-ih,-l);
				glTexCoord2f(0.25f,0.0f);		glVertex3f(-w/2,-ih,-l);
			}
			
			if( door!=RM_DOOR64 &&
				door!=RM_DOOR624 &&
				door!=RM_DOOR62 &&
				door!=RM_DOOR6 &&
				door!=RM_DOOR8624 &&
				door!=RM_DOOR864 &&
				door!=RM_DOOR862 &&
				door!=RM_DOOR86)
			{
				//Inner wall right
				glTexCoord2f(0.04f,0.04f);		glVertex3f(-iw,-ih,-il);
				glTexCoord2f(0.04f,0.96f);		glVertex3f(-iw,ih,-il);
				glTexCoord2f(0.96f,0.96f);		glVertex3f(-iw,ih,il);
				glTexCoord2f(0.96f,0.04f);		glVertex3f(-iw,-ih,il);
			}
			else
			{
				//Right side of door section
				glTexCoord2f(1.0f,1.0f);		glVertex3f(-iw,ih,l);
				glTexCoord2f(1.0f,0.0f);		glVertex3f(-iw,-ih,l);
				glTexCoord2f(0.75f,0.0f);		glVertex3f(-iw,-ih,l/2);
				glTexCoord2f(0.75f,1.0f);		glVertex3f(-iw,ih,l/2);
				//Top side of door section
				glTexCoord2f(0.75f,1.0f);		glVertex3f(-iw,ih,l/2);
				glTexCoord2f(0.75f,0.75f);		glVertex3f(-iw,ih/2,l/2);
				glTexCoord2f(0.25f,0.75f);		glVertex3f(-iw,ih/2,-l/2);
				glTexCoord2f(0.25f,1.0f);		glVertex3f(-iw,ih,-l/2);
				//left side of door section
				glTexCoord2f(0.25f,1.0f);		glVertex3f(-iw,ih,-l/2);
				glTexCoord2f(0.25f,0.0f);		glVertex3f(-iw,-ih,-l/2);
				glTexCoord2f(0.0f,0.0f);		glVertex3f(-iw,-ih,-l);
				glTexCoord2f(0.0f,1.0f);		glVertex3f(-iw,ih,-l);
			}

		glEnd();

		//clears texture and sets color to slightly grey
		glBindTexture(GL_TEXTURE_2D,0);
		glColor3f(0.05f,0.05f,0.05f);
		
		//Begins drawing
		glBegin(GL_QUADS);

			//Upper wall front
			glVertex3f(-iw,ih,il);
			glVertex3f(-w,h,l);
			glVertex3f(w,h,l);
			glVertex3f(iw,ih,il);	

			//upper wall right
			glVertex3f(iw,ih,-il);
			glVertex3f(iw,ih,il);	
			glVertex3f(w,h,l);
			glVertex3f(w,h,-l);

			//Upper wall back
			glVertex3f(-iw,ih,-il);
			glVertex3f(iw,ih,-il);	
			glVertex3f(w,h,-l);
			glVertex3f(-w,h,-l);

			//upper wall left
			glVertex3f(-iw,ih,-il);	
			glVertex3f(-w,h,-l);
			glVertex3f(-w,h,l);
			glVertex3f(-iw,ih,il);

		glEnd();

		break;
	case 3:
		//sets color to white
		glColor3f(1.0f,1.0f,1.0f);

		//determines the texture from the building building->getRoomType()
		switch(building->getRoomType())
		{
		case RM_GHOST:
			glBindTexture(GL_TEXTURE_2D,0);
			glColor4f(0.6f,0.6f,0.6f,0.5f);
			break;
		case RM_HALLWAY:
		case RM_AIRLOCK:
		case RM_BATHROOM:
		case RM_COMMANDCENTER:
		case RM_FREIGHTAIRLOCK:
		case RM_GALLEY:
		case RM_INDUSTRIALWORKSHOP:
		case RM_LABORATORY:
		case RM_LIBRARY:
		case RM_LIVINGQUARTERS:
		case RM_MININGCONTROLCENTER:
		case RM_OXYGENSCRUBBER:
		case RM_POWERFUSION:
		case RM_WAREHOUSE:
		case RM_WATERCOLLECTOR:
		case RM_WATERRECYCLING:
			glBindTexture(GL_TEXTURE_2D,outerTextures[0]);
			break;
		default:
			glBindTexture(GL_TEXTURE_2D,0);
			glColor3f(0.5f,0.5f,0.0f);
			break;
		}

		//Begins drawing
		glBegin(GL_QUADS);
			if(	door!=RM_DOOR8 &&
				door!=RM_DOOR86 &&
				door!=RM_DOOR862 &&
				door!=RM_DOOR864 &&
				door!=RM_DOOR8624 &&
				door!=RM_DOOR82 &&
				door!=RM_DOOR824 &&
				door!=RM_DOOR84)
			{
				//Outer wall front
				glTexCoord2f(1.0f,1.0f);		glVertex3f(w,h,l);
				glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,h,l);
				glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-h,l);
				glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-h,l);
			}
			else
			{
				//Right side of door section
				glTexCoord2f(1.0f,1.0f);		glVertex3f(w,h,l);
				glTexCoord2f(0.75f,1.0f);		glVertex3f(w/2,h,l);
				glTexCoord2f(0.75f,0.0f);		glVertex3f(w/2,-h,l);
				glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-h,l);
				//Top side of door section
				glTexCoord2f(0.75f,1.0f);		glVertex3f(w/2,h,l);
				glTexCoord2f(0.25f,1.0f);		glVertex3f(-w/2,h,l);
				glTexCoord2f(0.25f,0.75f);		glVertex3f(-w/2,h/2,l);
				glTexCoord2f(0.75f,0.75f);		glVertex3f(w/2,h/2,l);
				//left side of door section
				glTexCoord2f(0.25f,1.0f);		glVertex3f(-w/2,h,l);
				glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,h,l);
				glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-h,l);
				glTexCoord2f(0.25f,0.0f);		glVertex3f(-w/2,-h,l);
			}
			
			if( door!=RM_DOOR4 &&
				door!=RM_DOOR24 &&
				door!=RM_DOOR64 &&
				door!=RM_DOOR624 &&
				door!=RM_DOOR84 &&
				door!=RM_DOOR824 &&
				door!=RM_DOOR8624 &&
				door!=RM_DOOR864)
			{
				//Outer wall left
				glTexCoord2f(1.0f,1.0f);		glVertex3f(w,h,l);
				glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-h,l);
				glTexCoord2f(0.0f,0.0f);		glVertex3f(w,-h,-l);
				glTexCoord2f(0.0f,1.0f);		glVertex3f(w,h,-l);
			}
			else
			{
				//Right side of door section
				glTexCoord2f(1.0f,1.0f);		glVertex3f(w,h,l);
				glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-h,l);
				glTexCoord2f(0.75f,0.0f);		glVertex3f(w,-h,l/2);
				glTexCoord2f(0.75f,1.0f);		glVertex3f(w,h,l/2);
				//Top side of door section
				glTexCoord2f(0.75f,1.0f);		glVertex3f(w,h,l/2);
				glTexCoord2f(0.75f,0.75f);		glVertex3f(w,h/2,l/2);
				glTexCoord2f(0.25f,0.75f);		glVertex3f(w,h/2,-l/2);
				glTexCoord2f(0.25f,1.0f);		glVertex3f(w,h,-l/2);
				//left side of door section
				glTexCoord2f(0.25f,1.0f);		glVertex3f(w,h,-l/2);
				glTexCoord2f(0.25f,0.0f);		glVertex3f(w,-h,-l/2);
				glTexCoord2f(0.0f,0.0f);		glVertex3f(w,-h,-l);
				glTexCoord2f(0.0f,1.0f);		glVertex3f(w,h,-l);
			}

			if( door!=RM_DOOR2 &&
				door!=RM_DOOR24 &&
				door!=RM_DOOR624 &&
				door!=RM_DOOR62 &&
				door!=RM_DOOR824 &&
				door!=RM_DOOR82 &&
				door!=RM_DOOR8624 &&
				door!=RM_DOOR862)
			{
				//Outer wall back
				glTexCoord2f(1.0f,1.0f);		glVertex3f(w,h,-l);
				glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-h,-l);
				glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-h,-l);
				glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,h,-l);
			}
			else
			{
				//Right side of door section
				glTexCoord2f(1.0f,1.0f);		glVertex3f(w,h,-l);
				glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-h,-l);
				glTexCoord2f(0.75f,0.0f);		glVertex3f(w/2,-h,-l);
				glTexCoord2f(0.75f,1.0f);		glVertex3f(w/2,h,-l);
				//Top side of door section
				glTexCoord2f(0.75f,1.0f);		glVertex3f(w/2,h,-l);
				glTexCoord2f(0.75f,0.75f);		glVertex3f(w/2,h/2,-l);
				glTexCoord2f(0.25f,0.75f);		glVertex3f(-w/2,h/2,-l);
				glTexCoord2f(0.25f,1.0f);		glVertex3f(-w/2,h,-l);
				//left side of door section
				glTexCoord2f(0.25f,1.0f);		glVertex3f(-w/2,h,-l);
				glTexCoord2f(0.25f,0.0f);		glVertex3f(-w/2,-h,-l);
				glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-h,-l);
				glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,h,-l);
			}

		glEnd();

		//sets color to white if powered or red if unpowered
		if(powered)
			glColor3f(1.0f,1.0f,1.0f);
		else
			glColor3f(1.0f,0.25f,0.25f);

		//determins the texture from the building building->getRoomType()
		switch(building->getRoomType())
		{
		case RM_GHOST:
			glBindTexture(GL_TEXTURE_2D,0);
			glColor4f(0.6f,0.6f,0.6f,0.5f);
			break;
		case RM_HALLWAY:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_HALLWAY]);
			break;
		case RM_AIRLOCK:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_AIRLOCK]);
			break;
		case RM_BATHROOM:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_BATHROOM]);
			break;
		case RM_COMMANDCENTER:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_COMMANDCENTER]);
			break;
		case RM_FREIGHTAIRLOCK:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_FREIGHTAIRLOCK]);
			break;
		case RM_GALLEY:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_GALLEY]);
			break;
		case RM_INDUSTRIALWORKSHOP:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_INDUSTRIALWORKSHOP]);
			break;
		case RM_LABORATORY:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_LABORATORY]);
			break;
		case RM_LIBRARY:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_LIBRARY]);
			break;
		case RM_LIVINGQUARTERS:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_LIVINGQUARTERS]);
			break;
		case RM_MININGCONTROLCENTER:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_MININGCONTROLCENTER]);
			break;
		case RM_OXYGENSCRUBBER:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_OXYGENSCRUBBER]);
			break;
		case RM_POWERFUSION:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_POWERFUSION]);
			break;
		case RM_WAREHOUSE:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_WAREHOUSE]);
			break;
		case RM_WATERCOLLECTOR:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_WATERCOLLECTOR]);
			break;
		case RM_WATERRECYCLING:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_WATERRECYCLING]);
			break;
		default:
			glBindTexture(GL_TEXTURE_2D,0);
			glColor3f(0.0f,0.5f,0.0f);
			break;
		}

		//Begins drawing
		glBegin(GL_QUADS);

			if(	door!=RM_DOOR8 &&
				door!=RM_DOOR86 &&
				door!=RM_DOOR862 &&
				door!=RM_DOOR864 &&
				door!=RM_DOOR8624 &&
				door!=RM_DOOR82 &&
				door!=RM_DOOR824 &&
				door!=RM_DOOR84)
			{
				//Inner wall front
				glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-ih,il);
				glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,ih,il);
				glTexCoord2f(0.98f,1.0f);		glVertex3f(iw,ih,il);
				glTexCoord2f(0.98f,0.0f);		glVertex3f(iw,-ih,il);
			}
			else
			{
				//Right side of door section
				glTexCoord2f(1.0f,1.0f);		glVertex3f(w,ih,il);
				glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-ih,il);
				glTexCoord2f(0.75f,0.0f);		glVertex3f(w/2,-ih,il);
				glTexCoord2f(0.75f,1.0f);		glVertex3f(w/2,ih,il);
				//Top side of door section
				glTexCoord2f(0.75f,1.0f);		glVertex3f(w/2,ih,il);
				glTexCoord2f(0.75f,0.75f);		glVertex3f(w/2,ih/2,il);
				glTexCoord2f(0.25f,0.75f);		glVertex3f(-w/2,ih/2,il);
				glTexCoord2f(0.25f,1.0f);		glVertex3f(-w/2,ih,il);
				//left side of door section
				glTexCoord2f(0.25f,1.0f);		glVertex3f(-w/2,ih,il);
				glTexCoord2f(0.25f,0.0f);		glVertex3f(-w/2,-ih,il);
				glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-ih,il);
				glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,ih,il);
			}
		
			if( door!=RM_DOOR4 &&
				door!=RM_DOOR24 &&
				door!=RM_DOOR64 &&
				door!=RM_DOOR624 &&
				door!=RM_DOOR84 &&
				door!=RM_DOOR824 &&
				door!=RM_DOOR8624 &&
				door!=RM_DOOR864)
			{
				//Inner wall left
				glTexCoord2f(0.02f,0.0f);		glVertex3f(iw,-ih,-l);
				glTexCoord2f(0.98f,0.0f);		glVertex3f(iw,-ih,il);
				glTexCoord2f(0.98f,1.0f);		glVertex3f(iw,ih,il);
				glTexCoord2f(0.02f,1.0f);		glVertex3f(iw,ih,-l);
			}
			else
			{
				//Right side of door section
				glTexCoord2f(1.0f,1.0f);		glVertex3f(iw,ih,l);
				glTexCoord2f(0.75f,1.0f);		glVertex3f(iw,ih,l/2);
				glTexCoord2f(0.75f,0.0f);		glVertex3f(iw,-ih,l/2);
				glTexCoord2f(1.0f,0.0f);		glVertex3f(iw,-ih,l);
				//Top side of door section
				glTexCoord2f(0.75f,1.0f);		glVertex3f(iw,ih,l/2);
				glTexCoord2f(0.25f,1.0f);		glVertex3f(iw,ih,-l/2);
				glTexCoord2f(0.25f,0.75f);		glVertex3f(iw,ih/2,-l/2);
				glTexCoord2f(0.75f,0.75f);		glVertex3f(iw,ih/2,l/2);
				//left side of door section
				glTexCoord2f(0.25f,1.0f);		glVertex3f(iw,ih,-l/2);
				glTexCoord2f(0.0f,1.0f);		glVertex3f(iw,ih,-l);
				glTexCoord2f(0.0f,0.0f);		glVertex3f(iw,-ih,-l);
				glTexCoord2f(0.25f,0.0f);		glVertex3f(iw,-ih,-l/2);
			}

			if( door!=RM_DOOR2 &&
				door!=RM_DOOR24 &&
				door!=RM_DOOR624 &&
				door!=RM_DOOR62 &&
				door!=RM_DOOR824 &&
				door!=RM_DOOR82 &&
				door!=RM_DOOR8624 &&
				door!=RM_DOOR862)
			{
				//Inner wall back
				glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-ih,-il);
				glTexCoord2f(0.98f,0.0f);		glVertex3f(iw,-ih,-il);
				glTexCoord2f(0.98f,1.0f);		glVertex3f(iw,ih,-il);
				glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,ih,-il);
			}
			else
			{
				//Right side of door section
				glTexCoord2f(1.0f,1.0f);		glVertex3f(w,ih,-il);
				glTexCoord2f(0.75f,1.0f);		glVertex3f(w/2,ih,-il);
				glTexCoord2f(0.75f,0.0f);		glVertex3f(w/2,-ih,-il);
				glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-ih,-il);
				//Top side of door section
				glTexCoord2f(0.75f,1.0f);		glVertex3f(w/2,ih,-il);
				glTexCoord2f(0.25f,1.0f);		glVertex3f(-w/2,ih,-il);
				glTexCoord2f(0.25f,0.75f);		glVertex3f(-w/2,ih/2,-il);
				glTexCoord2f(0.75f,0.75f);		glVertex3f(w/2,ih/2,-il);
				//left side of door section
				glTexCoord2f(0.25f,1.0f);		glVertex3f(-w/2,ih,-il);
				glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,ih,-il);
				glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-ih,-il);
				glTexCoord2f(0.25f,0.0f);		glVertex3f(-w/2,-ih,-il);
			}

		glEnd();

		//clears texture and sets color to slightly grey
		glBindTexture(GL_TEXTURE_2D,0);
		glColor3f(0.05f,0.05f,0.05f);
		
		//Begins drawing
		glBegin(GL_QUADS);

			//Upper wall front
			glVertex3f(-w,ih,il);
			glVertex3f(-w,h,l);
			glVertex3f(w,h,l);
			glVertex3f(w,ih,il);	

			//upper wall right
			glVertex3f(iw,ih,-l);
			glVertex3f(iw,ih,il);	
			glVertex3f(w,h,l);
			glVertex3f(w,h,-l);

			//Upper wall back
			glVertex3f(-w,ih,-il);
			glVertex3f(w,ih,-il);
			glVertex3f(w,h,-l);
			glVertex3f(-w,h,-l);

		glEnd();

		break;	//end of case 3
	case 2:
		//sets color to white
		glColor3f(1.0f,1.0f,1.0f);

		//determines the texture from the building building->getRoomType()
		switch(building->getRoomType())
		{
		case RM_GHOST:
			glBindTexture(GL_TEXTURE_2D,0);
			glColor4f(0.6f,0.6f,0.6f,0.5f);
			break;
		case RM_HALLWAY:
		case RM_AIRLOCK:
		case RM_BATHROOM:
		case RM_COMMANDCENTER:
		case RM_FREIGHTAIRLOCK:
		case RM_GALLEY:
		case RM_INDUSTRIALWORKSHOP:
		case RM_LABORATORY:
		case RM_LIBRARY:
		case RM_LIVINGQUARTERS:
		case RM_MININGCONTROLCENTER:
		case RM_OXYGENSCRUBBER:
		case RM_POWERFUSION:
		case RM_WAREHOUSE:
		case RM_WATERCOLLECTOR:
		case RM_WATERRECYCLING:
			glBindTexture(GL_TEXTURE_2D,outerTextures[0]);
			break;
		default:
			glBindTexture(GL_TEXTURE_2D,0);
			glColor3f(0.5f,0.5f,0.0f);
			break;
		}

		//If special is false, it is a corner, if true, it is a corridor
		if(!building->getWallType())
		{
			//Begins drawing
			glBegin(GL_QUADS);
				if(	door!=RM_DOOR8 &&
					door!=RM_DOOR86 &&
					door!=RM_DOOR862 &&
					door!=RM_DOOR864 &&
					door!=RM_DOOR8624 &&
					door!=RM_DOOR82 &&
					door!=RM_DOOR824 &&
					door!=RM_DOOR84)
				{
					//Outer wall front
					glTexCoord2f(1.0f,1.0f);		glVertex3f(w,h,l);
					glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,h,l);
					glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-h,l);
					glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-h,l);
				}
				else
				{
					//Right side of door section
					glTexCoord2f(1.0f,1.0f);		glVertex3f(w,h,l);
					glTexCoord2f(0.75f,1.0f);		glVertex3f(w/2,h,l);
					glTexCoord2f(0.75f,0.0f);		glVertex3f(w/2,-h,l);
					glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-h,l);
					//Top side of door section
					glTexCoord2f(0.75f,1.0f);		glVertex3f(w/2,h,l);
					glTexCoord2f(0.25f,1.0f);		glVertex3f(-w/2,h,l);
					glTexCoord2f(0.25f,0.75f);		glVertex3f(-w/2,h/2,l);
					glTexCoord2f(0.75f,0.75f);		glVertex3f(w/2,h/2,l);
					//left side of door section
					glTexCoord2f(0.25f,1.0f);		glVertex3f(-w/2,h,l);
					glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,h,l);
					glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-h,l);
					glTexCoord2f(0.25f,0.0f);		glVertex3f(-w/2,-h,l);
				}
				
				if( door!=RM_DOOR4 &&
					door!=RM_DOOR24 &&
					door!=RM_DOOR64 &&
					door!=RM_DOOR624 &&
					door!=RM_DOOR84 &&
					door!=RM_DOOR824 &&
					door!=RM_DOOR8624 &&
					door!=RM_DOOR864)
				{
					//Outer wall left
					glTexCoord2f(1.0f,1.0f);		glVertex3f(w,h,l);
					glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-h,l);
					glTexCoord2f(0.0f,0.0f);		glVertex3f(w,-h,-l);
					glTexCoord2f(0.0f,1.0f);		glVertex3f(w,h,-l);
				}
				else
				{
					//Right side of door section
					glTexCoord2f(1.0f,1.0f);		glVertex3f(w,h,l);
					glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-h,l);
					glTexCoord2f(0.75f,0.0f);		glVertex3f(w,-h,l/2);
					glTexCoord2f(0.75f,1.0f);		glVertex3f(w,h,l/2);
					//Top side of door section
					glTexCoord2f(0.75f,1.0f);		glVertex3f(w,h,l/2);
					glTexCoord2f(0.75f,0.75f);		glVertex3f(w,h/2,l/2);
					glTexCoord2f(0.25f,0.75f);		glVertex3f(w,h/2,-l/2);
					glTexCoord2f(0.25f,1.0f);		glVertex3f(w,h,-l/2);
					//left side of door section
					glTexCoord2f(0.25f,1.0f);		glVertex3f(w,h,-l/2);
					glTexCoord2f(0.25f,0.0f);		glVertex3f(w,-h,-l/2);
					glTexCoord2f(0.0f,0.0f);		glVertex3f(w,-h,-l);
					glTexCoord2f(0.0f,1.0f);		glVertex3f(w,h,-l);
				}

			glEnd();

			//sets color to white if powered or red if unpowered
			if(powered)
				glColor3f(1.0f,1.0f,1.0f);
			else
				glColor3f(1.0f,0.25f,0.25f);

			//determins the texture from the building building->getRoomType()
			switch(building->getRoomType())
			{
			case RM_GHOST:
				glBindTexture(GL_TEXTURE_2D,0);
				glColor4f(0.6f,0.6f,0.6f,0.5f);
				break;
			case RM_HALLWAY:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_HALLWAY]);
				break;
			case RM_AIRLOCK:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_AIRLOCK]);
				break;
			case RM_BATHROOM:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_BATHROOM]);
				break;
			case RM_COMMANDCENTER:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_COMMANDCENTER]);
				break;
			case RM_FREIGHTAIRLOCK:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_FREIGHTAIRLOCK]);
				break;
			case RM_GALLEY:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_GALLEY]);
				break;
			case RM_INDUSTRIALWORKSHOP:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_INDUSTRIALWORKSHOP]);
				break;
			case RM_LABORATORY:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_LABORATORY]);
				break;
			case RM_LIBRARY:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_LIBRARY]);
				break;
			case RM_LIVINGQUARTERS:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_LIVINGQUARTERS]);
				break;
			case RM_MININGCONTROLCENTER:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_MININGCONTROLCENTER]);
				break;
			case RM_OXYGENSCRUBBER:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_OXYGENSCRUBBER]);
				break;
			case RM_POWERFUSION:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_POWERFUSION]);
				break;
			case RM_WAREHOUSE:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_WAREHOUSE]);
				break;
			case RM_WATERCOLLECTOR:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_WATERCOLLECTOR]);
				break;
			case RM_WATERRECYCLING:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_WATERRECYCLING]);
				break;
			default:
				glBindTexture(GL_TEXTURE_2D,0);
				glColor3f(0.0f,0.5f,0.0f);
				break;
			}

			//Begins drawing
			glBegin(GL_QUADS);
				if(	door!=RM_DOOR8 &&
					door!=RM_DOOR86 &&
					door!=RM_DOOR862 &&
					door!=RM_DOOR864 &&
					door!=RM_DOOR8624 &&
					door!=RM_DOOR82 &&
					door!=RM_DOOR824 &&
					door!=RM_DOOR84)
				{
					//Inner wall front
					glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-ih,il);	
					glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,ih,il);	
					glTexCoord2f(0.98f,1.0f);		glVertex3f(iw,ih,il);	
					glTexCoord2f(0.98f,0.0f);		glVertex3f(iw,-ih,il);	
				}
				else
				{
					//Right side of door section
					glTexCoord2f(1.0f,1.0f);		glVertex3f(w,ih,il);
					glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-ih,il);
					glTexCoord2f(0.75f,0.0f);		glVertex3f(w/2,-ih,il);
					glTexCoord2f(0.75f,1.0f);		glVertex3f(w/2,ih,il);
					//Top side of door section
					glTexCoord2f(0.75f,1.0f);		glVertex3f(w/2,ih,il);
					glTexCoord2f(0.75f,0.75f);		glVertex3f(w/2,ih/2,il);
					glTexCoord2f(0.25f,0.75f);		glVertex3f(-w/2,ih/2,il);
					glTexCoord2f(0.25f,1.0f);		glVertex3f(-w/2,ih,il);
					//left side of door section
					glTexCoord2f(0.25f,1.0f);		glVertex3f(-w/2,ih,il);
					glTexCoord2f(0.25f,0.0f);		glVertex3f(-w/2,-ih,il);
					glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-ih,il);
					glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,ih,il);
				}
				
				if( door!=RM_DOOR4 &&
					door!=RM_DOOR24 &&
					door!=RM_DOOR64 &&
					door!=RM_DOOR624 &&
					door!=RM_DOOR84 &&
					door!=RM_DOOR824 &&
					door!=RM_DOOR8624 &&
					door!=RM_DOOR864)
				{
					//Inner wall left
					glTexCoord2f(0.02f,0.0f);		glVertex3f(iw,-ih,-l);
					glTexCoord2f(0.98f,0.0f);		glVertex3f(iw,-ih,il);
					glTexCoord2f(0.98f,1.0f);		glVertex3f(iw,ih,il);
					glTexCoord2f(0.02f,1.0f);		glVertex3f(iw,ih,-l);
				}
				else
				{
					//Right side of door section
					glTexCoord2f(1.0f,1.0f);		glVertex3f(w,ih,il);
					glTexCoord2f(0.75f,1.0f);		glVertex3f(w,ih,l/2);
					glTexCoord2f(0.75f,0.0f);		glVertex3f(w,-ih,l/2);
					glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-ih,il);
					//Top side of door section
					glTexCoord2f(0.75f,1.0f);		glVertex3f(w,ih,l/2);
					glTexCoord2f(0.25f,1.0f);		glVertex3f(w,ih,-l/2);
					glTexCoord2f(0.25f,0.75f);		glVertex3f(w,ih/2,-l/2);
					glTexCoord2f(0.75f,0.75f);		glVertex3f(w,ih/2,l/2);
					//left side of door section
					glTexCoord2f(0.25f,1.0f);		glVertex3f(w,ih,-l/2);
					glTexCoord2f(0.0f,1.0f);		glVertex3f(w,ih,-il);
					glTexCoord2f(0.0f,0.0f);		glVertex3f(w,-ih,-il);
					glTexCoord2f(0.25f,0.0f);		glVertex3f(w,-ih,-l/2);
				}

				//Corner
				if(corner == RM_CORNER3)
				{
					glTexCoord2f(0.98f,0.0f);		glVertex3f(-iw,-ih,-il);
					glTexCoord2f(1.0f,0.0f);		glVertex3f(-iw,-ih,-l);
					glTexCoord2f(1.0f,1.0f);		glVertex3f(-iw,ih,-l);
					glTexCoord2f(0.98f,1.0f);		glVertex3f(-iw,ih,-il);
					
					glTexCoord2f(1.0f,0.0f);		glVertex3f(-w,-ih,-il);
					glTexCoord2f(0.98f,0.0f);		glVertex3f(-iw,-ih,-il);
					glTexCoord2f(0.98f,1.0f);		glVertex3f(-iw,ih,-il);
					glTexCoord2f(1.0f,1.0f);		glVertex3f(-w,ih,-il);
				}

			glEnd();

			//clears texture and sets color to slightly grey
			glBindTexture(GL_TEXTURE_2D,0);
			glColor3f(0.05f,0.05f,0.05f);
		
			//Begins drawing
			glBegin(GL_QUADS);

				//Upper wall front
				glVertex3f(-w,ih,il);
				glVertex3f(-w,h,l);
				glVertex3f(w,h,l);
				glVertex3f(w,ih,il);	

				//upper wall right
				glVertex3f(iw,ih,-l);
				glVertex3f(iw,ih,il);	
				glVertex3f(w,h,l);
				glVertex3f(w,h,-l);

				if(corner == RM_CORNER3)
				{
					glVertex3f(-iw,ih,-l);
					glVertex3f(-w,h,-l);
					glVertex3f(-w,ih,-il);
					glVertex3f(-iw,ih,-il);
				}

			glEnd();
		}
		else
		{
			//Begins drawing
			glBegin(GL_QUADS);
				if(	door!=RM_DOOR8 &&
					door!=RM_DOOR86 &&
					door!=RM_DOOR862 &&
					door!=RM_DOOR864 &&
					door!=RM_DOOR8624 &&
					door!=RM_DOOR82 &&
					door!=RM_DOOR824 &&
					door!=RM_DOOR84)
				{
					//Outer wall front
					glTexCoord2f(1.0f,1.0f);		glVertex3f(w,h,l);
					glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,h,l);
					glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-h,l);
					glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-h,l);
				}
				else
				{
					//Right side of door section
					glTexCoord2f(1.0f,1.0f);		glVertex3f(w,h,l);
					glTexCoord2f(0.75f,1.0f);		glVertex3f(w/2,h,l);
					glTexCoord2f(0.75f,0.0f);		glVertex3f(w/2,-h,l);
					glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-h,l);
					//Top side of door section
					glTexCoord2f(0.75f,1.0f);		glVertex3f(w/2,h,l);
					glTexCoord2f(0.25f,1.0f);		glVertex3f(-w/2,h,l);
					glTexCoord2f(0.25f,0.75f);		glVertex3f(-w/2,h/2,l);
					glTexCoord2f(0.75f,0.75f);		glVertex3f(w/2,h/2,l);
					//left side of door section
					glTexCoord2f(0.25f,1.0f);		glVertex3f(-w/2,h,l);
					glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,h,l);
					glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-h,l);
					glTexCoord2f(0.25f,0.0f);		glVertex3f(-w/2,-h,l);
				}

				if(	door!=RM_DOOR862 &&
					door!=RM_DOOR8624 &&
					door!=RM_DOOR82 &&
					door!=RM_DOOR824 &&
					door!=RM_DOOR62 &&
					door!=RM_DOOR624 &&
					door!=RM_DOOR2 &&
					door!=RM_DOOR24)
				{
					//Outer wall back
					glTexCoord2f(1.0f,1.0f);		glVertex3f(w,h,-l);
					glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-h,-l);
					glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-h,-l);
					glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,h,-l);
				}
				else
				{
					//Right side of door section
					glTexCoord2f(1.0f,1.0f);		glVertex3f(w,h,-l);
					glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-h,-l);
					glTexCoord2f(0.75f,0.0f);		glVertex3f(w/2,-h,-l);
					glTexCoord2f(0.75f,1.0f);		glVertex3f(w/2,h,-l);
					//Top side of door section
					glTexCoord2f(0.75f,1.0f);		glVertex3f(w/2,h,-l);
					glTexCoord2f(0.75f,0.75f);		glVertex3f(w/2,h/2,-l);
					glTexCoord2f(0.25f,0.75f);		glVertex3f(-w/2,h/2,-l);
					glTexCoord2f(0.25f,1.0f);		glVertex3f(-w/2,h,-l);
					//left side of door section
					glTexCoord2f(0.25f,1.0f);		glVertex3f(-w/2,h,-l);
					glTexCoord2f(0.25f,0.0f);		glVertex3f(-w/2,-h,-l);
					glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-h,-l);
					glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,h,-l);
				}

			glEnd();

			//sets color to white if powered or red if unpowered
			if(powered)
				glColor3f(1.0f,1.0f,1.0f);
			else
				glColor3f(1.0f,0.25f,0.25f);

			//determins the texture from the building building->getRoomType()
			switch(building->getRoomType())
			{
			case RM_GHOST:
				glBindTexture(GL_TEXTURE_2D,0);
				glColor4f(0.6f,0.6f,0.6f,0.5f);
				break;	
			case RM_HALLWAY:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_HALLWAY]);
				break;
			case RM_AIRLOCK:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_AIRLOCK]);
				break;
			case RM_BATHROOM:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_BATHROOM]);
				break;
			case RM_COMMANDCENTER:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_COMMANDCENTER]);
				break;
			case RM_FREIGHTAIRLOCK:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_FREIGHTAIRLOCK]);
				break;
			case RM_GALLEY:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_GALLEY]);
				break;
			case RM_INDUSTRIALWORKSHOP:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_INDUSTRIALWORKSHOP]);
				break;
			case RM_LABORATORY:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_LABORATORY]);
				break;
			case RM_LIBRARY:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_LIBRARY]);
				break;
			case RM_LIVINGQUARTERS:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_LIVINGQUARTERS]);
				break;
			case RM_MININGCONTROLCENTER:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_MININGCONTROLCENTER]);
				break;
			case RM_OXYGENSCRUBBER:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_OXYGENSCRUBBER]);
				break;
			case RM_POWERFUSION:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_POWERFUSION]);
				break;
			case RM_WAREHOUSE:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_WAREHOUSE]);
				break;
			case RM_WATERCOLLECTOR:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_WATERCOLLECTOR]);
				break;
			case RM_WATERRECYCLING:
				glBindTexture(GL_TEXTURE_2D,roomTextures[RM_WATERRECYCLING]);
				break;
			default:
				glBindTexture(GL_TEXTURE_2D,0);
				glColor3f(0.0f,0.5f,0.0f);
				break;
			}

			//Begins drawing
			glBegin(GL_QUADS);
				if(	door!=RM_DOOR8 &&
					door!=RM_DOOR86 &&
					door!=RM_DOOR862 &&
					door!=RM_DOOR864 &&
					door!=RM_DOOR8624 &&
					door!=RM_DOOR82 &&
					door!=RM_DOOR824 &&
					door!=RM_DOOR84)
				{
					//Inner wall front
					glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-ih,il);
					glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,ih,il);
					glTexCoord2f(1.0f,1.0f);		glVertex3f(w,ih,il);
					glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-ih,il);
				}
				else
				{
					//Right side of door section
					glTexCoord2f(1.0f,1.0f);		glVertex3f(w,ih,il);
					glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-ih,il);
					glTexCoord2f(0.75f,0.0f);		glVertex3f(w/2,-ih,il);
					glTexCoord2f(0.75f,1.0f);		glVertex3f(w/2,ih,il);
					//Top side of door section
					glTexCoord2f(0.75f,1.0f);		glVertex3f(w/2,ih,il);
					glTexCoord2f(0.75f,0.75f);		glVertex3f(w/2,ih/2,il);
					glTexCoord2f(0.25f,0.75f);		glVertex3f(-w/2,ih/2,il);
					glTexCoord2f(0.25f,1.0f);		glVertex3f(-w/2,ih,il);
					//left side of door section
					glTexCoord2f(0.25f,1.0f);		glVertex3f(-w/2,ih,il);
					glTexCoord2f(0.25f,0.0f);		glVertex3f(-w/2,-ih,il);
					glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-ih,il);
					glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,ih,il);
				}
				
				if(	door!=RM_DOOR862 &&
					door!=RM_DOOR8624 &&
					door!=RM_DOOR82 &&
					door!=RM_DOOR824 &&
					door!=RM_DOOR62 &&
					door!=RM_DOOR624 &&
					door!=RM_DOOR2 &&
					door!=RM_DOOR24)
				{
					//Inner wall back
					glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-ih,-il);
					glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-ih,-il);
					glTexCoord2f(1.0f,1.0f);		glVertex3f(w,ih,-il);
					glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,ih,-il);
				}
				else
				{
					//Right side of door section
					glTexCoord2f(1.0f,1.0f);		glVertex3f(w,ih,-il);
					glTexCoord2f(0.75f,1.0f);		glVertex3f(w/2,ih,-il);
					glTexCoord2f(0.75f,0.0f);		glVertex3f(w/2,-ih,-il);
					glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-ih,-il);
					//Top side of door section
					glTexCoord2f(0.75f,1.0f);		glVertex3f(w/2,ih,-il);
					glTexCoord2f(0.25f,1.0f);		glVertex3f(-w/2,ih,-il);
					glTexCoord2f(0.25f,0.75f);		glVertex3f(-w/2,ih/2,-il);
					glTexCoord2f(0.75f,0.75f);		glVertex3f(w/2,ih/2,-il);
					//left side of door section
					glTexCoord2f(0.25f,1.0f);		glVertex3f(-w/2,ih,-il);
					glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,ih,-il);
					glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-ih,-il);
					glTexCoord2f(0.25f,0.0f);		glVertex3f(-w/2,-ih,-il);
				}
			glEnd();

			//clears texture and sets color to slightly grey
			glBindTexture(GL_TEXTURE_2D,0);
			glColor3f(0.05f,0.05f,0.05f);
		
			//Begins drawing
			glBegin(GL_QUADS);

				//Upper wall front
				glVertex3f(-w,ih,il);
				glVertex3f(-w,h,l);
				glVertex3f(w,h,l);
				glVertex3f(w,ih,il);	

				//Upper wall back
				glVertex3f(-w,ih,-il);
				glVertex3f(w,ih,-il);
				glVertex3f(w,h,-l);
				glVertex3f(-w,h,-l);	

			glEnd();
		}

		break;	//end of case 2
	case 1:
		//sets color to white
		glColor3f(1.0f,1.0f,1.0f);

		//determines the texture from the building building->getRoomType()
		switch(building->getRoomType())
		{
		case RM_GHOST:
			glBindTexture(GL_TEXTURE_2D,0);
			glColor4f(0.6f,0.6f,0.6f,0.5f);
			break;
		case RM_HALLWAY:
		case RM_AIRLOCK:
		case RM_BATHROOM:
		case RM_COMMANDCENTER:
		case RM_FREIGHTAIRLOCK:
		case RM_GALLEY:
		case RM_INDUSTRIALWORKSHOP:
		case RM_LABORATORY:
		case RM_LIBRARY:
		case RM_LIVINGQUARTERS:
		case RM_MININGCONTROLCENTER:
		case RM_OXYGENSCRUBBER:
		case RM_POWERFUSION:
		case RM_WAREHOUSE:
		case RM_WATERCOLLECTOR:
		case RM_WATERRECYCLING:
			glBindTexture(GL_TEXTURE_2D,outerTextures[0]);
			break;
		default:
			glBindTexture(GL_TEXTURE_2D,0);
			glColor3f(0.5f,0.5f,0.0f);
			break;
		}
		//Begins drawing
		glBegin(GL_QUADS);
			if(	door!=RM_DOOR8 &&
				door!=RM_DOOR86 &&
				door!=RM_DOOR862 &&
				door!=RM_DOOR864 &&
				door!=RM_DOOR8624 &&
				door!=RM_DOOR82 &&
				door!=RM_DOOR824 &&
				door!=RM_DOOR84)
			{
				//Outer wall
				glTexCoord2f(1.0f,1.0f);		glVertex3f(w,h,l);
				glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,h,l);
				glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-h,l);
				glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-h,l);
			}
			else
			{
				//Right side of door section
				glTexCoord2f(1.0f,1.0f);		glVertex3f(w,h,l);
				glTexCoord2f(0.75f,1.0f);		glVertex3f(w/2,h,l);
				glTexCoord2f(0.75f,0.0f);		glVertex3f(w/2,-h,l);
				glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-h,l);
				//Top side of door section
				glTexCoord2f(0.75f,1.0f);		glVertex3f(w/2,h,l);
				glTexCoord2f(0.25f,1.0f);		glVertex3f(-w/2,h,l);
				glTexCoord2f(0.25f,0.75f);		glVertex3f(-w/2,h/2,l);
				glTexCoord2f(0.75f,0.75f);		glVertex3f(w/2,h/2,l);
				//left side of door section
				glTexCoord2f(0.25f,1.0f);		glVertex3f(-w/2,h,l);
				glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,h,l);
				glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-h,l);
				glTexCoord2f(0.25f,0.0f);		glVertex3f(-w/2,-h,l);
			}

		glEnd();

		//sets color to white if powered or red if unpowered
		if(powered)
			glColor3f(1.0f,1.0f,1.0f);
		else
			glColor3f(1.0f,0.25f,0.25f);

		//determins the texture from the building building->getRoomType()
		switch(building->getRoomType())
		{
		case RM_GHOST:
			glBindTexture(GL_TEXTURE_2D,0);
			glColor4f(0.6f,0.6f,0.6f,0.5f);
			break;
		case RM_HALLWAY:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_HALLWAY]);
			break;
		case RM_AIRLOCK:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_AIRLOCK]);
			break;
		case RM_BATHROOM:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_BATHROOM]);
			break;
		case RM_COMMANDCENTER:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_COMMANDCENTER]);
			break;
		case RM_FREIGHTAIRLOCK:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_FREIGHTAIRLOCK]);
			break;
		case RM_GALLEY:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_GALLEY]);
			break;
		case RM_INDUSTRIALWORKSHOP:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_INDUSTRIALWORKSHOP]);
			break;
		case RM_LABORATORY:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_LABORATORY]);
			break;
		case RM_LIBRARY:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_LIBRARY]);
			break;
		case RM_LIVINGQUARTERS:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_LIVINGQUARTERS]);
			break;
		case RM_MININGCONTROLCENTER:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_MININGCONTROLCENTER]);
			break;
		case RM_OXYGENSCRUBBER:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_OXYGENSCRUBBER]);
			break;
		case RM_POWERFUSION:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_POWERFUSION]);
			break;
		case RM_WAREHOUSE:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_WAREHOUSE]);
			break;
		case RM_WATERCOLLECTOR:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_WATERCOLLECTOR]);
			break;
		case RM_WATERRECYCLING:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_WATERRECYCLING]);
			break;
		default:
			glBindTexture(GL_TEXTURE_2D,0);
			glColor3f(0.0f,0.5f,0.0f);
			break;
		}

		//Begins drawing
		glBegin(GL_QUADS);
			if(	door!=RM_DOOR8 &&
				door!=RM_DOOR86 &&
				door!=RM_DOOR862 &&
				door!=RM_DOOR864 &&
				door!=RM_DOOR8624 &&
				door!=RM_DOOR82 &&
				door!=RM_DOOR824 &&
				door!=RM_DOOR84)
			{
				//Inner wall
				glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-ih,il);
				glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,ih,il);
				glTexCoord2f(1.0f,1.0f);		glVertex3f(w,ih,il);
				glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-ih,il);
			}
			else
			{
				//Right side of door section
				glTexCoord2f(1.0f,1.0f);		glVertex3f(w,ih,il);
				glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-ih,il);
				glTexCoord2f(0.75f,0.0f);		glVertex3f(w/2,-ih,il);
				glTexCoord2f(0.75f,1.0f);		glVertex3f(w/2,ih,il);
				//Top side of door section
				glTexCoord2f(0.75f,1.0f);		glVertex3f(w/2,ih,il);
				glTexCoord2f(0.75f,0.75f);		glVertex3f(w/2,ih/2,il);
				glTexCoord2f(0.25f,0.75f);		glVertex3f(-w/2,ih/2,il);
				glTexCoord2f(0.25f,1.0f);		glVertex3f(-w/2,ih,il);
				//left side of door section
				glTexCoord2f(0.25f,1.0f);		glVertex3f(-w/2,ih,il);
				glTexCoord2f(0.25f,0.0f);		glVertex3f(-w/2,-ih,il);
				glTexCoord2f(0.0f,0.0f);		glVertex3f(-w,-ih,il);
				glTexCoord2f(0.0f,1.0f);		glVertex3f(-w,ih,il);
			}

			if(corner == RM_CORNER9 || corner == RM_CORNER97)
			{
				glTexCoord2f(0.98f,0.0f);		glVertex3f(iw,-ih,-il);
				glTexCoord2f(0.98f,1.0f);		glVertex3f(iw,ih,-il);
				glTexCoord2f(1.0f,1.0f);		glVertex3f(iw,ih,-l);
				glTexCoord2f(1.0f,0.0f);		glVertex3f(iw,-ih,-l);
					
				glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-ih,-il);
				glTexCoord2f(1.0f,1.0f);		glVertex3f(w,ih,-il);
				glTexCoord2f(0.98f,1.0f);		glVertex3f(iw,ih,-il);
				glTexCoord2f(0.98f,0.0f);		glVertex3f(iw,-ih,-il);
			}

			if(corner == RM_CORNER7 || corner == RM_CORNER97)
			{
				glTexCoord2f(0.98f,0.0f);		glVertex3f(-iw,-ih,-il);
				glTexCoord2f(1.0f,0.0f);		glVertex3f(-iw,-ih,-l);
				glTexCoord2f(1.0f,1.0f);		glVertex3f(-iw,ih,-l);
				glTexCoord2f(0.98f,1.0f);		glVertex3f(-iw,ih,-il);
					
				glTexCoord2f(1.0f,0.0f);		glVertex3f(-w,-ih,-il);
				glTexCoord2f(0.98f,0.0f);		glVertex3f(-iw,-ih,-il);
				glTexCoord2f(0.98f,1.0f);		glVertex3f(-iw,ih,-il);
				glTexCoord2f(1.0f,1.0f);		glVertex3f(-w,ih,-il);
			}
		
		glEnd();

		//clears texture and sets color to slightly grey
		glBindTexture(GL_TEXTURE_2D,0);
		glColor3f(0.05f,0.05f,0.05f);
		
		//Begins drawing
		glBegin(GL_QUADS);

			//Upper wall
			glVertex3f(-w,ih,il);
			glVertex3f(-w,h,l);
			glVertex3f(w,h,l);
			glVertex3f(w,ih,il);	

			if(corner == RM_CORNER9 || corner == RM_CORNER97)
			{
				glVertex3f(iw,ih,-il);
				glVertex3f(w,ih,-il);
				glVertex3f(w,h,-l);
				glVertex3f(iw,ih,-l);
			}

			if(corner == RM_CORNER7 || corner == RM_CORNER97)
			{
				glVertex3f(-iw,ih,-il);
				glVertex3f(-iw,ih,-l);
				glVertex3f(-w,h,-l);
				glVertex3f(-w,ih,-il);
			}
		glEnd();

		break;	//end case 1 wall switch
	default:	//case 0 usually

		//sets color to white if powered or red if unpowered
		if(powered)
			glColor3f(1.0f,1.0f,1.0f);
		else
			glColor3f(1.0f,0.25f,0.25f);
		
		//determins the texture from the building building->getRoomType()
		switch(building->getRoomType())
		{
		case RM_GHOST:
			glBindTexture(GL_TEXTURE_2D,0);
			glColor4f(0.6f,0.6f,0.6f,0.5f);
			break;
		case RM_HALLWAY:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_HALLWAY]);
			break;
		case RM_AIRLOCK:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_AIRLOCK]);
			break;
		case RM_BATHROOM:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_BATHROOM]);
			break;
		case RM_COMMANDCENTER:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_COMMANDCENTER]);
			break;
		case RM_FREIGHTAIRLOCK:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_FREIGHTAIRLOCK]);
			break;
		case RM_GALLEY:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_GALLEY]);
			break;
		case RM_INDUSTRIALWORKSHOP:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_INDUSTRIALWORKSHOP]);
			break;
		case RM_LABORATORY:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_LABORATORY]);
			break;
		case RM_LIBRARY:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_LIBRARY]);
			break;
		case RM_LIVINGQUARTERS:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_LIVINGQUARTERS]);
			break;
		case RM_MININGCONTROLCENTER:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_MININGCONTROLCENTER]);
			break;
		case RM_OXYGENSCRUBBER:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_OXYGENSCRUBBER]);
			break;
		case RM_POWERFUSION:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_POWERFUSION]);
			break;
		case RM_WAREHOUSE:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_WAREHOUSE]);
			break;
		case RM_WATERCOLLECTOR:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_WATERCOLLECTOR]);
			break;
		case RM_WATERRECYCLING:
			glBindTexture(GL_TEXTURE_2D,roomTextures[RM_WATERRECYCLING]);
			break;
		default:
			glBindTexture(GL_TEXTURE_2D,0);
			glColor3f(0.0f,0.5f,0.0f);
			break;
		}

		glBegin(GL_QUADS);
			if( corner == RM_CORNER9 || 
				corner == RM_CORNER97||
				corner == RM_CORNER93||
				corner == RM_CORNER931||
				corner == RM_CORNER9317||
				corner == RM_CORNER937||
				corner == RM_CORNER91||
				corner == RM_CORNER917)
			{
				glTexCoord2f(0.98f,0.0f);		glVertex3f(iw,-ih,-il);
				glTexCoord2f(0.98f,1.0f);		glVertex3f(iw,ih,-il);
				glTexCoord2f(1.0f,1.0f);		glVertex3f(iw,ih,-l);
				glTexCoord2f(1.0f,0.0f);		glVertex3f(iw,-ih,-l);
					
				glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-ih,-il);
				glTexCoord2f(1.0f,1.0f);		glVertex3f(w,ih,-il);
				glTexCoord2f(0.98f,1.0f);		glVertex3f(iw,ih,-il);
				glTexCoord2f(0.98f,0.0f);		glVertex3f(iw,-ih,-il);
			}

			if(	corner == RM_CORNER7 ||
				corner == RM_CORNER97||
				corner == RM_CORNER937||
				corner == RM_CORNER9317||
				corner == RM_CORNER917||
				corner == RM_CORNER37||
				corner == RM_CORNER317||
				corner == RM_CORNER17)
			{
				glTexCoord2f(0.98f,0.0f);		glVertex3f(-iw,-ih,-il);
				glTexCoord2f(1.0f,0.0f);		glVertex3f(-iw,-ih,-l);
				glTexCoord2f(1.0f,1.0f);		glVertex3f(-iw,ih,-l);
				glTexCoord2f(0.98f,1.0f);		glVertex3f(-iw,ih,-il);
					
				glTexCoord2f(1.0f,0.0f);		glVertex3f(-w,-ih,-il);
				glTexCoord2f(0.98f,0.0f);		glVertex3f(-iw,-ih,-il);
				glTexCoord2f(0.98f,1.0f);		glVertex3f(-iw,ih,-il);
				glTexCoord2f(1.0f,1.0f);		glVertex3f(-w,ih,-il);
			}

			if( corner == RM_CORNER3 ||
				corner == RM_CORNER37||
				corner == RM_CORNER937||
				corner == RM_CORNER9317||
				corner == RM_CORNER931||
				corner == RM_CORNER37||
				corner == RM_CORNER317||
				corner == RM_CORNER31)
			{
				glTexCoord2f(0.98f,0.0f);		glVertex3f(iw,-ih,il);
				glTexCoord2f(1.0f,0.0f);		glVertex3f(iw,-ih,l);
				glTexCoord2f(1.0f,1.0f);		glVertex3f(iw,ih,l);
				glTexCoord2f(0.98f,1.0f);		glVertex3f(iw,ih,il);
					
				glTexCoord2f(1.0f,0.0f);		glVertex3f(w,-ih,il);
				glTexCoord2f(0.98f,0.0f);		glVertex3f(iw,-ih,il);
				glTexCoord2f(0.98f,1.0f);		glVertex3f(iw,ih,il);
				glTexCoord2f(1.0f,1.0f);		glVertex3f(w,ih,il);
			}
			
			if( corner == RM_CORNER1 ||
				corner == RM_CORNER17||
				corner == RM_CORNER917||
				corner == RM_CORNER9317||
				corner == RM_CORNER317||
				corner == RM_CORNER31||
				corner == RM_CORNER317||
				corner == RM_CORNER91)
			{
				glTexCoord2f(0.98f,0.0f);		glVertex3f(-iw,-ih,il);
				glTexCoord2f(0.98f,1.0f);		glVertex3f(-iw,ih,il);
				glTexCoord2f(1.0f,1.0f);		glVertex3f(-iw,ih,l);
				glTexCoord2f(1.0f,0.0f);		glVertex3f(-iw,-ih,l);
					
				glTexCoord2f(1.0f,0.0f);		glVertex3f(-w,-ih,il);
				glTexCoord2f(1.0f,1.0f);		glVertex3f(-w,ih,il);
				glTexCoord2f(0.98f,1.0f);		glVertex3f(-iw,ih,il);
				glTexCoord2f(0.98f,0.0f);		glVertex3f(-iw,-ih,il);
			}
		
		glEnd();
		//clears texture and sets color to slightly grey
		glBindTexture(GL_TEXTURE_2D,0);
		glColor3f(0.05f,0.05f,0.05f);
		
		//Begins drawing
		glBegin(GL_QUADS);

			if( corner == RM_CORNER9 || 
				corner == RM_CORNER97||
				corner == RM_CORNER93||
				corner == RM_CORNER931||
				corner == RM_CORNER9317||
				corner == RM_CORNER937||
				corner == RM_CORNER91||
				corner == RM_CORNER917)
			{
				glVertex3f(iw,ih,-il);
				glVertex3f(w,ih,-il);
				glVertex3f(w,h,-l);
				glVertex3f(iw,ih,-l);
			}

			if(	corner == RM_CORNER7 ||
				corner == RM_CORNER97||
				corner == RM_CORNER937||
				corner == RM_CORNER9317||
				corner == RM_CORNER917||
				corner == RM_CORNER37||
				corner == RM_CORNER317||
				corner == RM_CORNER17)
			{
				glVertex3f(-iw,ih,-il);
				glVertex3f(-iw,ih,-l);
				glVertex3f(-w,h,-l);
				glVertex3f(-w,ih,-il);
			}

			if( corner == RM_CORNER3 ||
				corner == RM_CORNER37||
				corner == RM_CORNER937||
				corner == RM_CORNER9317||
				corner == RM_CORNER931||
				corner == RM_CORNER37||
				corner == RM_CORNER317||
				corner == RM_CORNER31)
			{
				glVertex3f(iw,ih,il);
				glVertex3f(iw,ih,l);
				glVertex3f(w,h,l);
				glVertex3f(w,ih,il);
			}
			
			if( corner == RM_CORNER1 ||
				corner == RM_CORNER17||
				corner == RM_CORNER917||
				corner == RM_CORNER9317||
				corner == RM_CORNER317||
				corner == RM_CORNER31||
				corner == RM_CORNER317||
				corner == RM_CORNER91)
			{
				glVertex3f(-iw,ih,il);
				glVertex3f(-w,ih,il);
				glVertex3f(-w,h,l);
				glVertex3f(-iw,ih,l);
			}
		glEnd();

		break;		//end of default case
	}

	glPopMatrix();
	glPopMatrix();
}

void OpenGLRenderer::RenderTestBuilding(const Building& building, const bool& powered)
{
	int wallBit(building.getWallBitmask()),cornerBit(building.getCornerPosition()),doorBit(building.getDoorBitmask());
	glPushMatrix();
	glTranslatef(building.getPos().x,building.getPos().y,building.getPos().z);
	glPushMatrix();
	glTranslatef(0.5f,0.5f,0.5f);
	glPushMatrix();

	setBuildingCeilingTexture(building.getRoomType(), powered);
	glCallList(BuildingCeiling);

	setBuildingFloorTexture(building.getRoomType(),powered);
	glCallList(BuildingFloor);

	//going with the assumption that gl rotations are more expensive than texture assignments
	//all walls of a side will be rendered before rotating. Testing needed on this assumption!!!

	if((wallBit&Face_ZM)==Face_ZM)
	{
		if((doorBit&Face_ZM)==Face_ZM)
		{
			setBuildingOuterWallTexture(building.getRoomType(),powered);
			glCallList(BuildingOuterWallDoor);
		}
		else
		{
			setBuildingOuterWallTexture(building.getRoomType(),powered);
			glCallList(BuildingOuterWall);
		}

		setBuildingUpperWallTexture(building.getRoomType(),powered);
		glCallList(BuildingUpperWall);

		if((doorBit&Face_ZM)==Face_ZM)
		{
			setBuildingInnerWallTexture(building.getRoomType(),powered);
			glCallList(BuildingInnerWallDoor);
		}
		else
		{
			setBuildingInnerWallTexture(building.getRoomType(),powered);
			glCallList(BuildingInnerWall);
		}
	} 
	else if((cornerBit&Corner9)==Corner9)
	{
		//Draw Corner
		setBuildingUpperWallTexture(building.getRoomType(),powered);
		glCallList(BuildingUpperCorner);

		setBuildingInnerWallTexture(building.getRoomType(),powered);
		glCallList(BuildingInnerCorner);
	}

	glRotatef(90,0,1,0);

	//gl rotates counterclockwise????
	if((wallBit&Face_XM)==Face_XM)
	{
		if((doorBit&Face_XM)==Face_XM)
		{
			setBuildingOuterWallTexture(building.getRoomType(),powered);
			glCallList(BuildingOuterWallDoor);
		}
		else
		{
			setBuildingOuterWallTexture(building.getRoomType(),powered);
			glCallList(BuildingOuterWall);
		}

		setBuildingUpperWallTexture(building.getRoomType(),powered);
		glCallList(BuildingUpperWall);

		if((doorBit&Face_XM)==Face_XM)
		{
			setBuildingInnerWallTexture(building.getRoomType(),powered);
			glCallList(BuildingInnerWallDoor);
		}
		else
		{
			setBuildingInnerWallTexture(building.getRoomType(),powered);
			glCallList(BuildingInnerWall);
		}
	} 
	else if((cornerBit&Corner7)==Corner7)
	{
		//Draw Corner
		setBuildingUpperWallTexture(building.getRoomType(),powered);
		glCallList(BuildingUpperCorner);

		setBuildingInnerWallTexture(building.getRoomType(),powered);
		glCallList(BuildingInnerCorner);
	}

	glRotatef(90,0,1,0);


	if((wallBit&Face_ZP)==Face_ZP)
	{
		if((doorBit&Face_ZP)==Face_ZP)
		{
			setBuildingOuterWallTexture(building.getRoomType(),powered);
			glCallList(BuildingOuterWallDoor);
		}
		else
		{
			setBuildingOuterWallTexture(building.getRoomType(),powered);
			glCallList(BuildingOuterWall);
		}

		setBuildingUpperWallTexture(building.getRoomType(),powered);
		glCallList(BuildingUpperWall);

		if((doorBit&Face_ZP)==Face_ZP)
		{
			setBuildingInnerWallTexture(building.getRoomType(),powered);
			glCallList(BuildingInnerWallDoor);
		}
		else
		{
			setBuildingInnerWallTexture(building.getRoomType(),powered);
			glCallList(BuildingInnerWall);
		}
	} 
	else if((cornerBit&Corner1)==Corner1)
	{
		//Draw Corner
		setBuildingUpperWallTexture(building.getRoomType(),powered);
		glCallList(BuildingUpperCorner);

		setBuildingInnerWallTexture(building.getRoomType(),powered);
		glCallList(BuildingInnerCorner);
	}

	glRotatef(90,0,1,0);

	if((wallBit&Face_XP)==Face_XP)
	{

		if((doorBit&Face_XP)==Face_XP)
		{
			setBuildingOuterWallTexture(building.getRoomType(),powered);
			glCallList(BuildingOuterWallDoor);
		}
		else
		{
			setBuildingOuterWallTexture(building.getRoomType(),powered);
			glCallList(BuildingOuterWall);
		}

		setBuildingUpperWallTexture(building.getRoomType(),powered);
		glCallList(BuildingUpperWall);

		if((doorBit&Face_XP)==Face_XP)
		{
			setBuildingInnerWallTexture(building.getRoomType(),powered);
			glCallList(BuildingInnerWallDoor);
		}
		else
		{
			setBuildingInnerWallTexture(building.getRoomType(),powered);
			glCallList(BuildingInnerWall);
		}
	} 
	else if((cornerBit&Corner3)==Corner3)
	{
		//Draw Corner
		setBuildingUpperWallTexture(building.getRoomType(),powered);
		glCallList(BuildingUpperCorner);

		setBuildingInnerWallTexture(building.getRoomType(),powered);
		glCallList(BuildingInnerCorner);
	}

	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

/****************************
* BatchBMPLoad
****************************/
void OpenGLRenderer::BatchBMPLoad()
{
//Load all TGA files for the Main GUI
	LoadBMPImage("Textures/GUI/MainGUI.tga",GUITextures[GUI_MAIN]);			//Main GUI Background
	LoadBMPImage("Textures/GUI/ResourcesButton.tga", GUITextures[GUI_RESOURCES]);	//Resources button
	LoadBMPImage("Textures/GUI/BuildingButton.tga",GUITextures[GUI_BUILDING]);		//Building button
	LoadBMPImage("Textures/GUI/TechButton.tga",GUITextures[GUI_TECH]);			//Tech Button
	LoadBMPImage("Textures/GUI/ViewButton.tga",GUITextures[GUI_VIEW]);			//View button
	LoadBMPImage("Textures/GUI/SystemButton.tga",GUITextures[GUI_SYSTEM]);		//System button
	LoadBMPImage("Textures/GUI/GUIWindow.tga",GUITextures[GUI_WINDOW]);			//Window texture for secondary GUI Window
	LoadBMPImage("Textures/GUI/GUIWindowSecondary.tga",GUITextures[GUI_SECONDWINDOW]);		//Next Button	
	LoadBMPImage("Textures/GUI/CloseButton.tga",GUITextures[GUI_CLOSE]);		//Close button on secondary GUI
	LoadBMPImage("Textures/GUI/QuitButton.tga",GUITextures[GUI_QUIT]);			//quit button
	LoadBMPImage("Textures/GUI/BuildButton.tga",GUITextures[GUI_BUILD]);		//Build button
	LoadBMPImage("Textures/GUI/DemolishButton.tga",GUITextures[GUI_DEMOLISH]);	//Demolish button
	
	LoadBMPImage("Textures/GUI/HighlightIcon.tga",GUITextures[GUI_SELECTICON]);	//Selection Icon
	LoadBMPImage("Textures/GUI/HallwayIcon.tga",GUITextures[GUI_HALLWAYICON]);		//Hallway Icon
	LoadBMPImage("Textures/GUI/BathroomIcon.tga",GUITextures[GUI_BATHROOMICON]);		//Bathroom Icon
	LoadBMPImage("Textures/GUI/LQIcon.tga",GUITextures[GUI_LIVINGICON]);			//Living Quarters Icon
	LoadBMPImage("Textures/GUI/CommandIcon.tga",GUITextures[GUI_COMMANDICON]);		//Command Center Icon
	LoadBMPImage("Textures/GUI/GalleyIcon.tga",GUITextures[GUI_GALLEYICON]);		//Galley Icon
	LoadBMPImage("Textures/GUI/AirlockIcon.tga",GUITextures[GUI_AIRLOCKICON]);		//Airlock Icon
	LoadBMPImage("Textures/GUI/FreightAirlockIcon.tga",GUITextures[GUI_FREIGHTAIRLOCKICON]);		//Freight Airlock Icon
	LoadBMPImage("Textures/GUI/IndustrialWorkshopIcon.tga",GUITextures[GUI_INDUSTRIALWORKSHOPICON]);		//Industrial Workshop Icon
	LoadBMPImage("Textures/GUI/LaboratoryIcon.tga",GUITextures[GUI_LABORATORYICON]);		//Laboratory Icon
	LoadBMPImage("Textures/GUI/LibraryIcon.tga",GUITextures[GUI_LIBRARYICON]);		//Library Icon
	LoadBMPImage("Textures/GUI/MCCIcon.tga",GUITextures[GUI_MCCICON]);		//MCC Icon
	LoadBMPImage("Textures/GUI/WarehouseIcon.tga",GUITextures[GUI_WAREHOUSEICON]);		//Warehouse Icon
	LoadBMPImage("Textures/GUI/PowerPlantIcon.tga",GUITextures[GUI_POWERPLANTICON]);		//Power Plant Icon
	
	LoadBMPImage("Textures/GUI/DoorIcon.tga",GUITextures[GUI_DOORICON]);		//Door Icon

	LoadBMPImage("Textures/GUI/SkipButton.tga",GUITextures[GUI_SKIP]);		//Skip Button
	LoadBMPImage("Textures/GUI/NextButton.tga",GUITextures[GUI_NEXT]);		//Next Button

	
	LoadBMPImage("Textures/GUI/Tech1Tab.tga",GUITextures[GUI_TECH1TAB]);		//Tech 1 Tab Button
	LoadBMPImage("Textures/GUI/Tech2Tab.tga",GUITextures[GUI_TECH2TAB]);		//Tech 2 Tab Button
	LoadBMPImage("Textures/GUI/Tech3Tab.tga",GUITextures[GUI_TECH3TAB]);		//Tech 3 Tab Button
	LoadBMPImage("Textures/GUI/Tech4Tab.tga",GUITextures[GUI_TECH4TAB]);		//Tech 4 Tab Button
	LoadBMPImage("Textures/GUI/ObjectsTab.tga",GUITextures[GUI_OBJECTTAB]);		//Objects Tab Button


	//Loads the block textures
	LoadBMPImage("Textures/Blocks/Dirt.tga",blockTextures[0]);				//dirt

	//loads the floor textures
	LoadBMPImage("Textures/Rooms/FloorGrate.tga",floorTextures[0]);
	LoadBMPImage("Textures/Rooms/TileFloor.tga",floorTextures[1]);
	LoadBMPImage("Textures/Rooms/MetalFloor.tga",floorTextures[2]);
	

	//loads the rooms wall textures
	LoadBMPImage("Textures/Rooms/Hallway.tga",roomTextures[RM_HALLWAY]);
	LoadBMPImage("Textures/Rooms/Bedroom.tga",roomTextures[RM_LIVINGQUARTERS]);
	LoadBMPImage("Textures/Rooms/CommandCenter.tga",roomTextures[RM_COMMANDCENTER]);
	

	//loads the outer wall textures
	LoadBMPImage("Textures/Rooms/Outer.tga",outerTextures[0]);

	//Loads the diffuse model textures
	LoadBMPImageNoFlip("Textures/Models/Testing/ArrowBotUVTexture.bmp",modelTexturesDiffuse[MODEL_TEST]);

	//Loads the normal model textures

	//Loads the bump model textures
}

/****************************
* ready3D
****************************/
void OpenGLRenderer::ready3D()
{
	//creates the camera
	glViewport(0,0,RES_WIDTH,RES_HEIGHT);

	//sets up the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//sets up the projection viewport
	gluPerspective(60,(RES_WIDTH/RES_HEIGHT),0.1,10*(RENDER_DIST*CHUNK_SIZE));

	//set the modelview matrix and clear it
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT,GL_FILL);
}

/****************************
* ready2D
****************************/
void OpenGLRenderer::ready2D()
{
	//sets up the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//sets up the projection viewport
	glOrtho(0,RES_WIDTH,0,RES_HEIGHT,-1,1);

	//set the modelview matrix and clear it
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//enables texturing and filling modes
	glEnable(GL_TEXTURE_2D);
	glPolygonMode(GL_FRONT,GL_FILL);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}

/****************************
* getTexture -	Returns the texture address to the loaded
				textures.
****************************/
GLuint* OpenGLRenderer::getTexture(u32 texttype,s32 index)
{
	switch(texttype)
	{
	case 0:		//Blocks
		return &blockTextures[index];
		break;
	case 1:		//menu textures
		return &menuTextures[index];
		break;
	case 2:		//GUI textures
		return &GUITextures[index];
		break;
	case 3:		//room Textures
		return &roomTextures[index];
	case 4:
		return &modelTexturesDiffuse[index];
	case 5:
		return &modelTexturesNormals[index];
	case 6:
		return &modelTexturesNormals[index];
	default:
		return NULL;
	}
}

/****************************
* 
****************************/
void OpenGLRenderer::drawLine(Vector3<real32> pos1, Vector3<real32> pos2)
{
	lines.push_back(pos1);
	lines.push_back(pos2);
}

/****************************
* 
****************************/
void OpenGLRenderer::drawLine(Vector3<real32> origin, Vector3<real32> direction, s32 magnitude)
{
	direction.scale(magnitude);

	lines.push_back(origin);
	lines.push_back(direction+origin);
}

/****************************
* 
****************************/
void OpenGLRenderer::debugDraw()
{
	if(!lines.empty())
	{
		std::vector<Vector3<real32>>::iterator it;
		glPushMatrix();
		glColor3f(1.0f,0.0f,0.0f);

		//Iterate through the debug lines and 
		for(it=lines.begin();it!=lines.end();it++)
		{
			GLfloat x(it->x);
			GLfloat y(it->y);
			GLfloat z(it->z);
			it++;
			GLfloat a(it->x);
			GLfloat b(it->y);
			GLfloat c(it->z);

			glBegin(GL_LINES);
				glVertex3f(x,y,z);
				glVertex3f(a,b,c);
			glEnd();
		}

		glPopMatrix();
		glColor3f(1.0f,1.0f,1.0f);
	}
}

/****************************
* 
****************************/
tBOOL OpenGLRenderer::generateLists()
{
	float h=1;
	float w=1;
	float l=1;

	FacePolyYP=glGenLists(1);
	glNewList(FacePolyYP,GL_COMPILE);
	 	glBegin(GL_QUADS);
	 	 	glNormal3i(0,1,0);
	 	 	glTexCoord2i(0,0); 	 	glVertex3i(0,h,0);
	 	 	glTexCoord2i(0,1); 	 	glVertex3i(0,h,l);
	 	 	glTexCoord2i(1,1); 	 	glVertex3i(w,h,l);
	 	 	glTexCoord2i(1,0); 	 	glVertex3i(w,h,0);
	 	glEnd();
	glEndList();
 
	FacePolyYM=glGenLists(1);
	glNewList(FacePolyYM,GL_COMPILE);
	 	glBegin(GL_QUADS);
	 	 	glNormal3i(0,-1,0);
	 	 	glTexCoord2i(0,0); 	 	glVertex3i(w,0,l);
	 	 	glTexCoord2i(1,0); 	 	glVertex3i(0,0,l);
	 	 	glTexCoord2i(1,1); 	 	glVertex3i(0,0,0);
	 	 	glTexCoord2i(0,1); 	 	glVertex3i(w,0,0);
	 	glEnd();
	glEndList();
 
	FacePolyXP=glGenLists(1);
	glNewList(FacePolyXP,GL_COMPILE);
	 	glBegin(GL_QUADS);
	 	 	glNormal3i(1,0,0);
	 	 	glTexCoord2i(0,0); 	 	glVertex3i(w,0,0);
	 	 	glTexCoord2i(0,1); 	 	glVertex3i(w,h,0);
	 	 	glTexCoord2i(1,1); 	 	glVertex3i(w,h,l);
	 	 	glTexCoord2i(1,0); 	 	glVertex3i(w,0,l);
	 	glEnd();
	glEndList();
 
	FacePolyXM=glGenLists(1);
	glNewList(FacePolyXM,GL_COMPILE);
	 	glBegin(GL_QUADS);
	 	 	glNormal3i(-1,0,0);
	 	 	glTexCoord2i(0,0); 	 	glVertex3i(0,0,l);
	 	 	glTexCoord2i(0,1); 	 	glVertex3i(0,h,l);
	 	 	glTexCoord2i(1,1); 	 	glVertex3i(0,h,0);
	 	 	glTexCoord2i(1,0); 	 	glVertex3i(0,0,0);
	 	glEnd();
	glEndList();
 
	FacePolyZP=glGenLists(1);
	glNewList(FacePolyZP,GL_COMPILE);
	 	glBegin(GL_QUADS);
	 	 	glNormal3i(0,0,1);
	 	 	glTexCoord2i(0,0); 	 	glVertex3i(w,0,l);
	 	 	glTexCoord2i(0,1); 	 	glVertex3i(w,h,l);
	 	 	glTexCoord2i(1,1); 	 	glVertex3i(0,h,l);
	 	 	glTexCoord2i(1,0); 	 	glVertex3i(0,0,l);
	 	glEnd();
	glEndList();
 
	FacePolyZM=glGenLists(1);
	glNewList(FacePolyZM,GL_COMPILE);
	 	glBegin(GL_QUADS);
	 	 	glNormal3i(0,0,-1);
	 	 	glTexCoord2i(0,0); 	 	glVertex3i(0,0,0);
	 	 	glTexCoord2i(0,1); 	 	glVertex3i(0,h,0);
	 	 	glTexCoord2i(1,1); 	 	glVertex3i(w,h,0);
	 	 	glTexCoord2i(1,0); 	 	glVertex3i(w,0,0);
	 	glEnd();
	glEndList();

	box=glGenLists(1);
	glNewList(box,GL_COMPILE);
		//Store Cube data
		glBegin(GL_QUADS);
			glNormal3f(0.0f,0.0f,-1.0f);
			glTexCoord2f(0,0);		glVertex3f(l,-h,-w);
			glTexCoord2f(1,0);		glVertex3f(-l,-h,-w);
			glTexCoord2f(1,1);		glVertex3f(-l,h,-w);
			glTexCoord2f(0,1);		glVertex3f(l,h,-w);

			glNormal3f(0.0f,0.0f,1.0f);
			glTexCoord2f(0,0);		glVertex3f(-l,-h,w);
			glTexCoord2f(1,0);		glVertex3f(l,-h,w);
			glTexCoord2f(1,1);		glVertex3f(l,h,w);
			glTexCoord2f(0,1);		glVertex3f(-l,h,w);

			glNormal3f(1,0,0);
			glTexCoord2f(0,0);		glVertex3f(l,-h,w);
			glTexCoord2f(1,0);		glVertex3f(l,-h,-w);
			glTexCoord2f(1,1);		glVertex3f(l,h,-w);
			glTexCoord2f(0,1);		glVertex3f(l,h,w);

			glNormal3f(-1,0,0);
			glTexCoord2f(0,0);		glVertex3f(-l,-h,-w);
			glTexCoord2f(1,0);		glVertex3f(-l,-h,w);
			glTexCoord2f(1,1);		glVertex3f(-l,h,w);
			glTexCoord2f(0,1);		glVertex3f(-l,h,-w);

			glNormal3f(0,-1,0);
			glTexCoord2f(0,0);		glVertex3f(-l,-h,-w);
			glTexCoord2f(1,0);		glVertex3f(l,-h,-w);
			glTexCoord2f(1,1);		glVertex3f(l,-h,w);
			glTexCoord2f(0,1);		glVertex3f(-l,-h,w);

			glNormal3f(0,1,0);
			glTexCoord2f(0,0);		glVertex3f(l,h,-w);
			glTexCoord2f(1,0);		glVertex3f(-l,h,-w);
			glTexCoord2f(1,1);		glVertex3f(-l,h,w);
			glTexCoord2f(0,1);		glVertex3f(l,h,w);
		glEnd();
	glEndList();

	BuildingFloor=glGenLists(1);
	glNewList(BuildingFloor,GL_COMPILE);
		glBegin(GL_QUADS);

		glNormal3i(0,1,0);
		glTexCoord2f(1,0);		glVertex3f(0.5f,-0.48f,0.5f);
		glTexCoord2f(1,1);		glVertex3f(0.5f,-0.48f,-0.5f);
		glTexCoord2f(0,1);		glVertex3f(-0.5f,-0.48f,-0.5f);
		glTexCoord2f(0,0);		glVertex3f(-0.5f,-0.48f,0.5f);

		glEnd();
	glEndList();

	BuildingCeiling=glGenLists(1);
	glNewList(BuildingCeiling,GL_COMPILE);
		glBegin(GL_QUADS);

		glNormal3i(0,-1,0);
		glTexCoord2f(0,1);		glVertex3f(-0.5f,0.48f,-0.5f);
		glTexCoord2f(1,1);		glVertex3f(0.5f,0.48f,-0.5f);
		glTexCoord2f(1,0);		glVertex3f(0.5f,0.48f,0.5f);
		glTexCoord2f(0,0);		glVertex3f(-0.5f,0.48f,0.5f);

		glEnd();
	glEndList();

	BuildingOuterWall=glGenLists(1);
	glNewList(BuildingOuterWall,GL_COMPILE);
		glBegin(GL_QUADS);

		glNormal3i(0,0,-1);
		glTexCoord2f(1.0f,1.0f);		glVertex3f(0.5f,0.5f,-0.5f);
		glTexCoord2f(1.0f,0.0f);		glVertex3f(0.5f,-0.5f,-0.5f);
		glTexCoord2f(0.0f,0.0f);		glVertex3f(-0.5f,-0.5f,-0.5f);
		glTexCoord2f(0.0f,1.0f);		glVertex3f(-0.5f,0.5f,-0.5f);
		
		glEnd();
	glEndList();

	BuildingUpperWall=glGenLists(1);
	glNewList(BuildingUpperWall,GL_COMPILE);
		glBegin(GL_QUADS);

		glNormal3i(0,1,0);
		glVertex3f(-0.5f,0.48f,-0.48f);
		glVertex3f(0.5f,0.48f,-0.48f);	
		glVertex3f(0.5f,0.5f,-0.5f);
		glVertex3f(-0.5f,0.5f,-0.5f);

		glEnd();
	glEndList();

	BuildingInnerWall=glGenLists(1);
	glNewList(BuildingInnerWall,GL_COMPILE);
		glBegin(GL_QUADS);

		glNormal3i(0,0,1);
		glTexCoord2f(0,0);		glVertex3f(-0.5f,-0.48f,-0.48f);
		glTexCoord2f(1,0);		glVertex3f(0.5f,-0.48f,-0.48f);
		glTexCoord2f(1,1);		glVertex3f(0.5f,0.48f,-0.48f);
		glTexCoord2f(0,1);		glVertex3f(-0.5f,0.48f,-0.48f);

		glEnd();
	glEndList();

	BuildingUpperCorner=glGenLists(1);
	glNewList(BuildingUpperCorner,GL_COMPILE);
		glBegin(GL_QUADS);

		glNormal3i(0,1,0);
		glTexCoord2f(0,0);		glVertex3f(0.48f,0.48f,-0.48f);
		glTexCoord2f(1,0);		glVertex3f(0.5f,0.48f,-0.48f);
		glTexCoord2f(1,1);		glVertex3f(0.5f,0.5f,-0.5f);
		glTexCoord2f(0,1);		glVertex3f(0.48f,0.48f,-0.5f);

		glEnd();
	glEndList();

	BuildingInnerCorner=glGenLists(1);
	glNewList(BuildingInnerCorner,GL_COMPILE);
		glBegin(GL_QUADS);

		glNormal3i(-1,0,0);
		glTexCoord2f(1,0);		glVertex3f(0.48f,-0.48f,-0.5f);
		glTexCoord2f(1,0);		glVertex3f(0.48f,-0.48f,-0.48f);
		glTexCoord2f(1,1);		glVertex3f(0.48f,0.48f,-0.48f);
		glTexCoord2f(1,1);		glVertex3f(0.48f,0.48f,-0.5f);

		glNormal3i(0,0,1);
		glTexCoord2f(0,0);		glVertex3f(0.48f,-0.48f,-0.48f);
		glTexCoord2f(0,0);		glVertex3f(0.5f,-0.48f,-0.48f);
		glTexCoord2f(0,1);		glVertex3f(0.5f,0.48f,-0.48f);
		glTexCoord2f(0,1);		glVertex3f(0.48f,0.48f,-0.48f);

		glEnd();
	glEndList();
	
	BuildingOuterWallDoor=glGenLists(1);
	glNewList(BuildingOuterWallDoor,GL_COMPILE);
		glBegin(GL_QUADS);

		glNormal3i(0,1,0);

		glEnd();
	glEndList();
	
	BuildingInnerWallDoor=glGenLists(1);
	glNewList(BuildingInnerWallDoor,GL_COMPILE);
		glBegin(GL_QUADS);

		glNormal3i(0,1,0);

		glEnd();
	glEndList();
	return TRUE;
}

void OpenGLRenderer::setBlockTexture(const int &blockType)
{
	switch(blockType)
	{
	case BlockType_Dirt:
		glBindTexture(GL_TEXTURE_2D,blockTextures[0]);
		glColor3f(1.0f,1.0f,1.0f);
		break;
	case BlockType_Rock:
		glBindTexture(GL_TEXTURE_2D,0);
		glColor3f(0.7f,0.7f,0.7f);
		break;
	case BlockType_MARKER:
		glBindTexture(GL_TEXTURE_2D,0);
		glColor3f(0.0f,0.0f,0.8f);
	default:
		glColor3f(1.0f,0.0f,0.0f);
		break;
	}
}

void OpenGLRenderer::setBuildingCeilingTexture(const int& type, const bool& powered)
{
	//determine the texture from the building type
	switch(type)
	{
	case RM_GHOST:
		glBindTexture(GL_TEXTURE_2D,0);
		glColor4f(0.6f,0.6f,0.6f,0.5f);
		break;
	case RM_HALLWAY:
	case RM_AIRLOCK:
	case RM_BATHROOM:
	case RM_COMMANDCENTER:
	case RM_FREIGHTAIRLOCK:
	case RM_GALLEY:
	case RM_INDUSTRIALWORKSHOP:
	case RM_LABORATORY:
	case RM_LIBRARY:
	case RM_LIVINGQUARTERS:
	case RM_MININGCONTROLCENTER:
	case RM_OXYGENSCRUBBER:
	case RM_POWERFUSION:
	case RM_WAREHOUSE:
	case RM_WATERCOLLECTOR:
	case RM_WATERRECYCLING:
	default:
		//sets the texture to blank and the debug color
		glBindTexture(GL_TEXTURE_2D,0);
		glColor3f(0.5f,0.5f,0.5f);
		break;
	}

	//sets color to white if powered or red if unpowered
	if(powered)
		glColor3f(1.0f,1.0f,1.0f);
	else
		glColor3f(1.0f,0.25f,0.25f);
}

void OpenGLRenderer::setBuildingFloorTexture(const int& type, const bool& powered)
{
	//determine the texture from the building type
	switch(type)
	{
	case RM_GHOST:
		glBindTexture(GL_TEXTURE_2D,0);
		glColor4f(0.6f,0.6f,0.6f,0.5f);
		break;
	case RM_HALLWAY:
		glBindTexture(GL_TEXTURE_2D,floorTextures[FL_GRATE]);
		break;
	case RM_AIRLOCK:
	case RM_BATHROOM:
		glBindTexture(GL_TEXTURE_2D,floorTextures[FL_TILED]);
		break;
	case RM_COMMANDCENTER:
		glBindTexture(GL_TEXTURE_2D,floorTextures[FL_METAL]);
		break;
	case RM_FREIGHTAIRLOCK:
	case RM_GALLEY:
	case RM_INDUSTRIALWORKSHOP:
	case RM_LABORATORY:
	case RM_LIBRARY:
	case RM_LIVINGQUARTERS:
		glBindTexture(GL_TEXTURE_2D,floorTextures[FL_METAL]);
		break;
	case RM_MININGCONTROLCENTER:
	case RM_OXYGENSCRUBBER:
	case RM_POWERFUSION:
	case RM_WAREHOUSE:
	case RM_WATERCOLLECTOR:
	case RM_WATERRECYCLING:
	default:
		//sets the texture to blank and the debug color
		glBindTexture(GL_TEXTURE_2D,0);
		glColor3f(0.5f,0.5f,0.5f);
		break;
	}

	//sets color to white if powered or red if unpowered
	if(powered)
		glColor3f(1.0f,1.0f,1.0f);
	else
		glColor3f(1.0f,0.25f,0.25f);
} 

void OpenGLRenderer::setBuildingInnerWallTexture(const int& type, const bool& powered)
{
	//determins the texture from the building type
	switch(type)
	{
	case RM_GHOST:
		glBindTexture(GL_TEXTURE_2D,0);
		glColor4f(0.6f,0.6f,0.6f,0.5f);
		break;
	case RM_HALLWAY:
		glBindTexture(GL_TEXTURE_2D,roomTextures[RM_HALLWAY]);
		break;
	case RM_AIRLOCK:
		glBindTexture(GL_TEXTURE_2D,roomTextures[RM_AIRLOCK]);
		break;
	case RM_BATHROOM:
		glBindTexture(GL_TEXTURE_2D,roomTextures[RM_BATHROOM]);
		break;
	case RM_COMMANDCENTER:
		glBindTexture(GL_TEXTURE_2D,roomTextures[RM_COMMANDCENTER]);
		break;
	case RM_FREIGHTAIRLOCK:
		glBindTexture(GL_TEXTURE_2D,roomTextures[RM_FREIGHTAIRLOCK]);
		break;
	case RM_GALLEY:
		glBindTexture(GL_TEXTURE_2D,roomTextures[RM_GALLEY]);
		break;
	case RM_INDUSTRIALWORKSHOP:
		glBindTexture(GL_TEXTURE_2D,roomTextures[RM_INDUSTRIALWORKSHOP]);
		break;
	case RM_LABORATORY:
		glBindTexture(GL_TEXTURE_2D,roomTextures[RM_LABORATORY]);
		break;
	case RM_LIBRARY:
		glBindTexture(GL_TEXTURE_2D,roomTextures[RM_LIBRARY]);
		break;
	case RM_LIVINGQUARTERS:
		glBindTexture(GL_TEXTURE_2D,roomTextures[RM_LIVINGQUARTERS]);
		break;
	case RM_MININGCONTROLCENTER:
		glBindTexture(GL_TEXTURE_2D,roomTextures[RM_MININGCONTROLCENTER]);
		break;
	case RM_OXYGENSCRUBBER:
		glBindTexture(GL_TEXTURE_2D,roomTextures[RM_OXYGENSCRUBBER]);
		break;
	case RM_POWERFUSION:
		glBindTexture(GL_TEXTURE_2D,roomTextures[RM_POWERFUSION]);
		break;
	case RM_WAREHOUSE:
		glBindTexture(GL_TEXTURE_2D,roomTextures[RM_WAREHOUSE]);
		break;
	case RM_WATERCOLLECTOR:
		glBindTexture(GL_TEXTURE_2D,roomTextures[RM_WATERCOLLECTOR]);
		break;
	case RM_WATERRECYCLING:
		glBindTexture(GL_TEXTURE_2D,roomTextures[RM_WATERRECYCLING]);
		break;
	default:
		glBindTexture(GL_TEXTURE_2D,0);
		glColor3f(0.0f,0.5f,0.0f);
		break;
	}

	//sets color to white if powered or red if unpowered
	if(powered)
		glColor3f(1.0f,1.0f,1.0f);
	else
		glColor3f(1.0f,0.25f,0.25f);
}

void OpenGLRenderer::setBuildingOuterWallTexture(const int& type, const bool& powered)
{
	//determines the texture from the building type
	switch(type)
	{
	case RM_GHOST:
		glBindTexture(GL_TEXTURE_2D,0);
		glColor4f(0.6f,0.6f,0.6f,0.5f);
		break;
	case RM_HALLWAY:
	case RM_AIRLOCK:
	case RM_BATHROOM:
	case RM_COMMANDCENTER:
	case RM_FREIGHTAIRLOCK:
	case RM_GALLEY:
	case RM_INDUSTRIALWORKSHOP:
	case RM_LABORATORY:
	case RM_LIBRARY:
	case RM_LIVINGQUARTERS:
	case RM_MININGCONTROLCENTER:
	case RM_OXYGENSCRUBBER:
	case RM_POWERFUSION:
	case RM_WAREHOUSE:
	case RM_WATERCOLLECTOR:
	case RM_WATERRECYCLING:
		glBindTexture(GL_TEXTURE_2D,outerTextures[0]);
		break;
	default:
		glBindTexture(GL_TEXTURE_2D,0);
		glColor3f(0.5f,0.5f,0.0f);
		break;
	}
}

void OpenGLRenderer::setBuildingUpperWallTexture(const int& type, const bool& powered)
{
	//clears texture and sets color to slightly grey
	if(type==RM_GHOST)
	{
		glBindTexture(GL_TEXTURE_2D,0);
		glColor4f(0.6f,0.6f,0.6f,0.5f);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D,0);
		glColor3f(0.05f,0.05f,0.05f);
	}
}