#include "AIEntity.h"
#include <Outland\Vector.h>
#include <Windows.h>
#include <gl\glew.h>
#include <gl\GLU.h>


/****************************
* Constructor
****************************/
AIEntity::AIEntity()
{
	position=Vector3<real64>(0,0,0);
	health=0;
}

/****************************
* Destructor
****************************/
AIEntity::~AIEntity()
{

}

/****************************
* Update
****************************/
void AIEntity::Update()
{
	StateController.Update();
}

/****************************
* Render
****************************/
void AIEntity::Render()
{
	glPushMatrix();
	glTranslatef((GLfloat)position.x,(GLfloat)position.y,(GLfloat)position.z);
	glPushMatrix();
	glRotatef(rotate,0,1,0);

	//clear texture
	glBindTexture(GL_TEXTURE_2D,0);

	//begins drawing quads
	glBegin(GL_QUADS);

	//Draw the Entity

	glEnd();

	glPopMatrix();
	glPopMatrix();
}

/****************************
* setPos
****************************/
void AIEntity::setPos(Vector3<real64> newpos)
{
	position=newpos;
}

/****************************
* setHealth
****************************/
void AIEntity::setHealth(s32 healthTotal)
{
	health=healthTotal;
}

/****************************
* movePos
****************************/
void AIEntity::movePos(Vector3<real64> movepos)
{
	position+=movepos;
}

/****************************
* changeHealth
****************************/
void AIEntity::changeHealth(s32 healthChange)
{
	health+=healthChange;
}

/****************************
* getHealth
****************************/
s32 AIEntity::getHealth()
{
	return health;
}


/****************************
* getPos
****************************/
Vector3<real64> AIEntity::getPos()
{
	return position;
}
