#include "Block.h"
#include <Windows.h>
#include <gl\glew.h>
#include <gl\GLU.h>
#include <Outland\Singletons.h>

/****************************
* Constructor
****************************/
Block::Block():
	blockType(0),
	faceData(0),
	isActive(false),
	pos(0,0,0)
{}

/****************************
* Constructor
****************************/
Block::Block(int Type, bool act,Vector3<s32> data):
	blockType(Type),
	isActive(act),
	pos(data)
{}

/****************************
* Destructor
****************************/
Block::~Block()
{

}

/****************************
* checkActive
****************************/
bool Block::checkActive()
{
	return isActive;
}

/****************************
* setActive
****************************/
void Block::setActive(bool active)
{
	isActive=active;

	//if true, add to render list
	//if false, remove from render list
}

/****************************
* Render
****************************/
void Block::Render()
{
	if(blockType!=BlockType_Air)
	{
		float h=0.5f;
		float w=0.5f;
		float l=0.5f;
	
		glPushMatrix();
			glColor4f(1.0f,0.0f,0.0f,0.5f);
			glTranslatef((GLfloat)pos.x,(GLfloat)pos.y,(GLfloat)pos.z);
			glBegin(GL_QUADS);
				glNormal3f(0.0f,0.0f,-1.0f);

				glVertex3f(l,-h,-w);
				glVertex3f(-l,-h,-w);
				glVertex3f(-l,h,-w);
				glVertex3f(l,h,-w);

				glNormal3f(0.0f,0.0f,1.0f);
				glVertex3f(-l,-h,w);
				glVertex3f(l,-h,w);
				glVertex3f(l,h,w);
				glVertex3f(-l,h,w);

			
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

		glColor4f(1.0f,1.0f,1.0f,1.0f);
	}
}


/****************************
* setType
****************************/
void Block::setType(s32 type)
{
	blockType=type;
	clearFaceChecks();
}

/****************************
* setPos
****************************/
void Block::setPos(Vector3<s32> newpos)
{
	pos=newpos;
}

void Block::addFace(const int & faceMask)
{
	faceData|=faceMask;
	recordFaceCheck(faceMask);
}

void Block::removeFace(const int & faceMask)
{
	int temp(faceData);
	temp&=faceMask;
	faceData-=temp;
	recordFaceCheck(faceMask);
}

void Block::recordFaceCheck(const int & faceMask)
{
	int temp(faceMask<<8);
	faceData|=temp;
}

void Block::clearFaceChecks()
{
	int temp(faceData),checkMask(655280);
	temp=temp&checkMask;
	faceData-=temp;
}

bool Block::check6Faces(const Vector3<s64>& chunkPos)
{
	Vector3<s64> blockPos(chunkPos.x+pos.x,chunkPos.y+pos.y,chunkPos.z+pos.z);
	bool activeTrack=false;
	int checkData=faceData>>8;

	//Begin checking for what faces still need evaluating
	if((checkData&Face_YP)==0)
	{
		Vector3<s64> yPlus(blockPos.x,blockPos.y+1,blockPos.z);

		Block* checkBlock(grCore->chunkMan.getReferenceToBlock(yPlus));
		
		if(checkBlock!=NULL)
		{
			if(checkBlock->getType()==BlockType_Air)
			{
				this->addFace(Face_YP);
				activeTrack=true;
			}
			else
			{
				this->recordFaceCheck(Face_YP);
				checkBlock->recordFaceCheck(Face_YM);
			}
		}
		else
		{
			this->addFace(Face_YP);
			activeTrack=true;
		}
	}

	if((checkData&Face_YM)==0)
	{
		Vector3<s64> yMinus(blockPos.x,blockPos.y-1,blockPos.z);

		Block* checkBlock(grCore->chunkMan.getReferenceToBlock(yMinus));
		
		if(checkBlock!=NULL)
		{
			if(checkBlock->getType()==BlockType_Air)
			{
				this->addFace(Face_YM);
				activeTrack=true;
			}
			else
			{
				this->recordFaceCheck(Face_YM);
				checkBlock->recordFaceCheck(Face_YP);
			}
		}
		else
		{
			this->addFace(Face_YM);
			activeTrack=true;
		}
	}

	if((checkData&Face_XP)==0)
	{
		Vector3<s64> xPlus(blockPos.x+1,blockPos.y,blockPos.z);

		Block* checkBlock(grCore->chunkMan.getReferenceToBlock(xPlus));
		
		if(checkBlock!=NULL)
		{
			if(checkBlock->getType()==BlockType_Air)
			{
				this->addFace(Face_XP);
				activeTrack=true;
			}
			else
			{
				this->recordFaceCheck(Face_XP);
				checkBlock->recordFaceCheck(Face_XM);
			}
		}
		else
		{
			this->addFace(Face_XP);
			activeTrack=true;
		}
	}

	if((checkData&Face_XM)==0)
	{
		Vector3<s64> xMinus(blockPos.x-1,blockPos.y,blockPos.z);

		Block* checkBlock(grCore->chunkMan.getReferenceToBlock(xMinus));
		
		if(checkBlock!=NULL)
		{
			if(checkBlock->getType()==BlockType_Air)
			{
				this->addFace(Face_XM);
				activeTrack=true;
			}
			else
			{
				this->recordFaceCheck(Face_XM);
				checkBlock->recordFaceCheck(Face_XP);
			}
		}
		else
		{
			this->addFace(Face_XM);
			activeTrack=true;
		}
	}

	if((checkData&Face_ZP)==0)
	{
		Vector3<s64> zPlus(blockPos.x,blockPos.y,blockPos.z+1);

		Block* checkBlock(grCore->chunkMan.getReferenceToBlock(zPlus));
		
		if(checkBlock!=NULL)
		{
			if(checkBlock->getType()==BlockType_Air)
			{
				this->addFace(Face_ZP);
				activeTrack=true;
			}
			else
			{
				this->recordFaceCheck(Face_ZP);
				checkBlock->recordFaceCheck(Face_ZM);
			}
		}
		else
		{
			this->addFace(Face_ZP);
			activeTrack=true;
		}
	}

	if((checkData&Face_ZM)==0)
	{
		Vector3<s64> zMinus(blockPos.x,blockPos.y,blockPos.z-1);

		Block* checkBlock(grCore->chunkMan.getReferenceToBlock(zMinus));
		
		if(checkBlock!=NULL)
		{
			if(checkBlock->getType()==BlockType_Air)
			{
				this->addFace(Face_ZM);
				activeTrack=true;
			}
			else
			{
				this->recordFaceCheck(Face_ZM);
				checkBlock->recordFaceCheck(Face_ZP);
			}
		}
		else
		{
			this->addFace(Face_ZM);
			activeTrack=true;
		}
	}

	return activeTrack;
}

