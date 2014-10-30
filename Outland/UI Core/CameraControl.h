#ifndef CameraControl_H
#define CameraControl_H

#include <Outland\UnitAbstraction.h>
#include <Outland\Quaternion.h>

class CameraControl
{
public:
	CameraControl();
	~CameraControl();

	void Update();
	void Init();
	void ReverseCamMove();
	void setRotateCam(float roll, float yaw, float pitch);
	void rotateCam(float roll, float yaw, float pitch);
	void setCamPos(float x, float y, float z);
	void moveCam(float x, float y, float z);
	float getCamPosX();
	float getCamPosY();
	float getCamPosZ();
	float getCamRotR();
	float getCamRotY();
	float getCamRotP();
	void hasMoved(tBOOL value);
	void recalcQuats();
	void CalcPicking();
	tBOOL getMove();
	void rayTrace(real32 X, real32 Y);

	void debugUpdate();

	char* cstr;
	char* upV;
	char* viewV;
	Vector3<real32> updir;
	Vector3<real32> view;
private:
	//Current camera variables
	float camX,camY,camZ;
	float camRoll,camYaw,camPitch;
	tBOOL moved;
	Quaternion cam;
	Quaternion up;
	Quaternion revcam;
	real32 *rotation;
	real32 *upmat;
	void rotateX(real32 X);
	void rotateY(real32 Y);
	Vector3<real32> h,v;

};

#endif