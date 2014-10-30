#include "CameraControl.h"

#include <Windows.h>
#include <gl\glew.h>
#include <gl\GLU.h>
#include <Outland\Singletons.h>
#include <Outland\Ray.h>
#include <cstring>
#include <iostream>

//External declarations
extern int RES_WIDTH;
extern int RES_HEIGHT;
extern int RENDER_DIST;
extern int CHUNK_SIZE;

/****************************
* Constructor
****************************/
CameraControl::CameraControl():
	camX(0),
	camY(10),
	camZ(5),
	camRoll(0),
	camYaw(0),
	camPitch(0),
	moved(TRUE)
{
	rotation=new real32[16];
	upmat=new real32[16];
	cstr=new char[5];
	upV=new char[5];
	viewV=new char[5];
}

/****************************
* Destructor
****************************/
CameraControl::~CameraControl()
{
	delete rotation;
	delete cstr;
	delete upmat;
	delete upV;
	delete viewV;
}

/****************************
* Update
****************************/
void CameraControl::Update()
{
	//moved=FALSE;

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(uiCore->getView())
	{
		glTranslatef((GLfloat)gameCore->player.getPos().x,(GLfloat)gameCore->player.getPos().y,(GLfloat)gameCore->player.getPos().z);

		glPushMatrix();

		glTranslatef(-1*(GLfloat)gameCore->player.getPos().x,-1*(GLfloat)gameCore->player.getPos().y,-1*(GLfloat)gameCore->player.getPos().z);

		glRotatef(gameCore->player.rotPitch,1,0,0);
		glRotatef(gameCore->player.rotYaw,0,1,0);
	}
	else
	{
		glTranslatef(camX,camY,camZ);
	
		//saves camera coordinates
		glPushMatrix();
	
		glTranslatef(-1*camX,-1*camY,-1*camZ);

		glRotatef(camPitch,1,0,0);
		glRotatef(camYaw,0,1,0);
	}
	debugUpdate();

	//Sends the rotation matrix onto the stack
	//glMultMatrixf(upmat);
	//glMultMatrixf(rotation);

	//saves the camera matrix to the stack and transforms it to local coordinates
	glPushMatrix();
}

/****************************
* Init
****************************/
void CameraControl::Init()
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

}

/****************************
* ReverseCamMove
****************************/
void CameraControl::ReverseCamMove()
{
	//This function reverses the camera transformations and is used to return the matrix to absolute world coordinates
	//glRotatef(camYaw,0,1,0);
	//glRotatef(camPitch,1,0,0);
	
	if(uiCore->getView())
	{
		glTranslatef(-(GLfloat)gameCore->player.getPos().x,-(GLfloat)gameCore->player.getPos().y,-(GLfloat)gameCore->player.getPos().z);
	}
	else
	{
		glTranslatef(-(GLfloat)camX,-(GLfloat)camY,-(GLfloat)camZ);
	}
	/*real32 matrix[4][4];
	QuatToMatrix(matrix,&cam);
	GLfloat *mat=new GLfloat[16];
	//converts [4][4] matrix into an opengl float [16] matrix
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			mat[4*i+j]=matrix[j][i];
		}
	}
	
	glMultMatrixf(mat);
	glPushMatrix();*/
}

/****************************
* setrotateCam -	This sets the rotations to the passed values
****************************/
void CameraControl::setRotateCam(float roll, float yaw, float pitch)
{
	if(uiCore->getView())
	{
		gameCore->player.rotPitch=pitch;
		gameCore->player.rotYaw=yaw;
	}
	else
	{
		camRoll=roll;
		camYaw=yaw;
		camPitch=pitch;
	}
}

/****************************
* rotateCam
****************************/
void CameraControl::rotateCam(float roll, float yaw, float pitch)
{
	//This increments the camera's rotation by the passed values
	if(uiCore->getView())
	{
		gameCore->player.rotYaw+=yaw;
		if(gameCore->player.rotYaw>=360)
		{
			gameCore->player.rotYaw-=360;
		}
		else if(gameCore->player.rotYaw<0)
		{
			gameCore->player.rotYaw+=360;
		}

		gameCore->player.rotPitch+=pitch;
		if(gameCore->player.rotPitch>90)
		{
			gameCore->player.rotPitch=90;
		}
		else if(gameCore->player.rotPitch<-90)
		{
			gameCore->player.rotPitch=-90;
		}
	}
	else
	{
		camYaw+=yaw;
		if(camYaw>=360)
		{
			camYaw-=360;
		}
		else if(camYaw<0)
		{
			camYaw+=360;
		}

		camPitch+=pitch;
		if(camPitch>90)
		{
			camPitch=90;
		}
		else if(camPitch<-90)
		{
			camPitch=-90;
		}

		camRoll=roll;
		if(camRoll>=360)
		{
			camRoll-=360;
		}
		else if(camRoll<0)
		{
			camRoll+=360;
		}
	}
	//recalculate the up directional vector used for picking

	//recalcQuats();
}

/****************************
* moveCam
****************************/
void CameraControl::setCamPos(float x, float y, float z)
{
	if(!uiCore->getView())
	{
		//This sets the camera to the position passed
		camX=x;
		camY=y;
		camZ=z;
	}
	else
	{
		gameCore->player.setPos(Vector3<real64>(x,y,z));
	}
}

/****************************
* moveCam
****************************/
void CameraControl::moveCam(float x, float y, float z)
{
	//This is a custom function to control the position of the camera in 1st and 3rd person
	
	if(!uiCore->getView())		//false is 3rd person camera
	{
		//increments the camera based on the floats passed
		camX+=(float)(x*cos(camYaw*3.1415927/180)+z*sin((camYaw-180)*3.1415927/180));
		camY+=y;
		camZ+=(float)(z*cos(camYaw*3.1415927/180)+x*-sin((camYaw-180)*3.1415927/180));

		real32 rDist=(real32)RENDER_DIST;

		//insures the camera doesn't move beyond the loaded chunks
		if(camX>(grCore->getCenterPos().x+(CHUNK_SIZE)+((rDist)*CHUNK_SIZE)/2)-CHUNK_SIZE)
		{
			camX=(float)((grCore->getCenterPos().x+(CHUNK_SIZE)+((rDist)*CHUNK_SIZE)/2)-CHUNK_SIZE);
		}
		else if(camX<(grCore->getCenterPos().x+(CHUNK_SIZE)-((rDist)*CHUNK_SIZE)/2)-CHUNK_SIZE)
		{
			camX=(float)((grCore->getCenterPos().x+(CHUNK_SIZE)-((rDist)*CHUNK_SIZE)/2)-CHUNK_SIZE);
		}
		
		//insures the camera doesn't move beyond the loaded chunks
		/*if(camY>(grCore->getCenterPos().y+(CHUNK_SIZE/2)+((rDist+5)*CHUNK_SIZE)/2))
		{
			camY=(grCore->getCenterPos().y+(CHUNK_SIZE/2)+((rDist+5)*CHUNK_SIZE)/2);
		}
		else if(camY<(grCore->getCenterPos().y+(CHUNK_SIZE/2)-((rDist)*CHUNK_SIZE)/2))
		{
			camY=(grCore->getCenterPos().y+(CHUNK_SIZE/2)-((rDist)*CHUNK_SIZE)/2);
		}*/
	
		//insures the camera doesn't move beyond the loaded chunks
		if(camZ>(grCore->getCenterPos().z+(CHUNK_SIZE)+((rDist)*CHUNK_SIZE)/2)-CHUNK_SIZE)
		{
			camZ=(float)((grCore->getCenterPos().z+(CHUNK_SIZE)+((rDist)*CHUNK_SIZE)/2)-CHUNK_SIZE);
		}
		else if(camZ<(grCore->getCenterPos().z+(CHUNK_SIZE)-((rDist)*CHUNK_SIZE)/2)-CHUNK_SIZE)
		{
			camZ=(float)((grCore->getCenterPos().z+(CHUNK_SIZE)-((rDist)*CHUNK_SIZE)/2)-CHUNK_SIZE);
		}

		view.x=cos(camPitch)-sin(camYaw);
		view.y=cos(camYaw);
		view.z=sin(camPitch);

		view.normalize();
	}
	//if view is in first person, make sure the player position is updated
	else
	{
		float nx,ny,nz;
		nx=(float)(x*cos(gameCore->player.rotYaw*3.1415927/180)+z*sin((gameCore->player.rotYaw-180)*3.1415927/180));
		ny=y;
		nz=(float)(z*cos(gameCore->player.rotYaw*3.1415927/180)+x*-sin((gameCore->player.rotYaw-180)*3.1415927/180));
		gameCore->player.movePosition(Vector3<real64>(nx,ny,nz));
	}
}

/****************************
* getCamPosX
****************************/
float CameraControl::getCamPosX()
{
	return camX;
}

/****************************
* getCamPosY
****************************/
float CameraControl::getCamPosY()
{
	return camY;
}

/****************************
* getCamPosZ
****************************/
float CameraControl::getCamPosZ()
{
	return camZ;
}

/****************************
* getCamRotR
****************************/
float CameraControl::getCamRotR()
{
	return camRoll;
}

/****************************
* getCamRotY
****************************/
float CameraControl::getCamRotY()
{
	return camYaw;
}

/****************************
* getCamRotP
****************************/
float CameraControl::getCamRotP()
{
	return camPitch;
}

/****************************
* hasMoved
****************************/
void CameraControl::hasMoved(tBOOL value)
{
	moved=value;
}

/****************************
* getMove
****************************/
tBOOL CameraControl::getMove()
{
	return moved;
}


/****************************
* recalcQuats -	Recalculates the quaternions for view
****************************/
void CameraControl::recalcQuats()
{
//	real32 tempYaw,tempPitch,tempRoll;
	
	////converts the angles into radians
	//tempRoll=camRoll*3.1415927/180;
	//tempYaw=camYaw*3.1415927/180;
	//tempPitch=camPitch*3.1415927/180;

	
	//Converts the euler angles to the quaternion
	EulerToQuat(camRoll,camPitch,camYaw,&cam);
	EulerToQuat(-camRoll,-camPitch,-camYaw,&revcam);

	cam.Normalize();
	real32 matrix[4][4];
	QuatToMatrix(matrix,&cam);

	//converts [4][4] matrix into an opengl float [16] matrix
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			rotation[4*i+j]=matrix[j][i];
		}
	}
}


/****************************
* debugUpdate -	Recalculates the information for the debug screen
****************************/
void CameraControl::debugUpdate()
{
	real64 x,y,z;
	x=(s64)uiCore->UI_Camera.getCamPosX();
	y=(s64)uiCore->UI_Camera.getCamPosY();
	z=(s64)uiCore->UI_Camera.getCamPosZ();

	std::string str="";
	str.append("X: ");
	if(x<0)
	{
		str.append("-");
		str.append(uiCore->floatToString(-1*x,2));
	}
	else
		str.append(uiCore->floatToString(x,2));
	str.append(" Y: ");
	if(y<0)
	{
		str.append("-");
		str.append(uiCore->floatToString(-1*y,2));
	}
	else
		str.append(uiCore->floatToString(y,2));
	str.append(" Z: ");
	if(z<0)
	{
		str.append("-");
		str.append(uiCore->floatToString(-1*z,2));
	}
	else
		str.append(uiCore->floatToString(z,2));
	delete [] cstr;
	cstr=new char[str.size()+1];

	strcpy_s(cstr,(str.size()+1)*sizeof(char),str.c_str());

	str="Up Vector: X ";

	str.append(uiCore->floatToString(updir.x,2));

	str.append(" Y ");
	str.append(uiCore->floatToString(updir.y,2));

	str.append(" Z ");
	str.append(uiCore->floatToString(updir.z,2));
	delete [] upV;
	upV=new char[str.size()+1];
	strcpy_s(upV,(str.size()+1)*sizeof(char),str.c_str());

	str="View Vector: X ";

	str.append(uiCore->floatToString(view.x,2));

	str.append(" Y ");
	str.append(uiCore->floatToString(view.y,2));

	str.append(" Z ");
	str.append(uiCore->floatToString(view.z,2));
	delete [] viewV;
	viewV=new char[str.size()+1];
	strcpy_s(viewV,(str.size()+1)*sizeof(char),str.c_str());
}


/****************************
* CalcPicking -	Recalculates the vectors and picking information
****************************/
void CameraControl::CalcPicking()
{
	real64 RAD(3.1415926535897932384626433832795/180);
	real32 fov((real32)(60*RAD));
	real32 vLength,hLength; 
	real32 width(RES_WIDTH),height(RES_HEIGHT);


	//variables to hold the matrices
	GLfloat proj[16];
	GLfloat model[16];

	glGetFloatv(GL_PROJECTION_MATRIX,proj);
	glGetFloatv(GL_MODELVIEW_MATRIX,model);

	//combine matrices
	glPushMatrix();
	glLoadMatrixf(proj);
	glMultMatrixf(model);
	glGetFloatv(GL_MODELVIEW_MATRIX,model);
	glPopMatrix();	

	//Grab the up vector
	updir.x=model[1];
	updir.y=model[5];
	updir.z=model[9];

	//Grab the forward vector
	view.x=model[2];
	view.y=model[6];
	view.z=model[10];

	//normalize the up vector
	updir.normalize();

	//normalize the heading vector
	view.normalize();

	h=CrossProduct(view,updir);
	v=updir;

	vLength=(real32)(tan(fov/2)*.1);
	hLength=((width/height)*vLength);

	v.scale(vLength);
	h.scale(vLength);
}

void CameraControl::rayTrace(real32 X, real32 Y)
{
	Vector3<real32> camPos(camX,camY,camZ),pos,dir;
	Vector3<real32> temp1(view),temp2(h),temp3(v);
	X-=(RES_WIDTH)/2;
	Y-=(RES_HEIGHT+15)/2;

	cout << "Ray Trace:\n";
	cout << "View Direction: X:" << temp1.x << " Y:"<<temp1.y << " Z:" << temp1.z;
	cout << "\nHorizontal: X:" << temp2.x << " Y:" << temp2.y << " Z:" << temp2.z;
	cout << "\nVertical: X:" << temp3.x << " Y:" << temp3.y << " Z:" << temp3.z << "\n\n";

	X=X/((RES_WIDTH)/2);
	Y=Y/((RES_HEIGHT)/2);
	temp1.scale((real32)0.1);
	temp2.scale(X);
	temp3.scale(Y);
	pos=camPos+temp1+temp2+temp3;
	dir=pos-camPos;
	dir.normalize();

#ifdef _DEBUG
	grCore->pRenderer->drawLine(pos,dir,30);
#endif

	s32 build(uiCore->build);	
	RayCol<Block> rayBlock;
	RayCol<Building> rayBuilding;
	Ray<real32> ray(pos,dir);


	switch(build)
	{
	case 0:
		gameCore->bMan.rayTrace(ray,rayBuilding);
		grCore->chunkMan.rayTrace(ray,rayBlock);

		if(rayBuilding.dist!=10000)
		{
			//Display that room's GUI
		}
		else
		{
			//Switch to showing no GUI
		}
		break;
	case 1:
	case 2:
		grCore->chunkMan.rayTrace(ray,rayBlock);

		//Checks to make sure that something has been stored as a collision and then offsets it to the correct position based on the face clicked.
		if(rayBlock.dist!=100000)
		{
			Vector3<s32> offset;
			switch(rayBlock.face)
			{
			case 1:
				offset=Vector3<s32>(0,1,0);
				break;
			case 2:
				offset=Vector3<s32>(0,-1,0);
				break;
			case 3:
				offset=Vector3<s32>(1,0,0);
				break;
			case 4:
				offset=Vector3<s32>(-1,0,0);
				break;
			case 5:
				offset=Vector3<s32>(0,0,1);
				break;
			case 6:
				offset=Vector3<s32>(0,0,-1);
				break;
			}

			if(uiCore->getBuildMarkState(1))
			{
				uiCore->setBuild2(Vector3<s64>(rayBlock.blockCol->getPos().x,rayBlock.blockCol->getPos().y,rayBlock.blockCol->getPos().z)+Vector3<s64>(offset.x,offset.y,offset.z)+rayBlock.sOffset);
			}
			else
			{
				uiCore->setBuild1(Vector3<s64>(rayBlock.blockCol->getPos().x,rayBlock.blockCol->getPos().y,rayBlock.blockCol->getPos().z)+Vector3<s64>(offset.x,offset.y,offset.z)+rayBlock.sOffset);
			}
		}
		break;
	case 3:
		gameCore->bMan.rayTrace(ray,rayBuilding);

		if(rayBuilding.dist!=10000)
		{
			s32 door(0);
			switch(rayBuilding.face)
			{
			case 1:
				break;
			case 2:
				break;
			case 3:
				door=RM_DOOR6;
				break;
			case 4:
				door=RM_DOOR4;
				break;
			case 5:
				door=RM_DOOR2;
				break;
			case 6:
				door=RM_DOOR8;
				break;
			}

			if(door!=0)
			{
				gameCore->bMan.addDoor(Vector3<s64>(rayBuilding.blockCol->getPos().x+rayBuilding.sOffset.x,rayBuilding.blockCol->getPos().y+rayBuilding.sOffset.y,rayBuilding.blockCol->getPos().z+rayBuilding.sOffset.z),door);
				uiCore->build=0;
			}
		}
		break;
	}
}