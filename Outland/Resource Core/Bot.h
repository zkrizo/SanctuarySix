#ifndef Bot_H
#define Bot_H

#include <Outland\UnitAbstraction.h>
#include "Object.h"
#include <gl\glew.h>
#include <string>

class Bot: public Object
{
public:
	Bot(const Vector3<real64>& npos,const std::string& filename);
	Bot(const Vector3<real64>& npos,const std::string& filename,const GLuint* diffuse,const GLuint* normals,const GLuint* bump);
	~Bot();
	s32 getMaxSpeed(){return maxspeed;};
	s32 getCurSpeed(){return curspeed;};
	s32 getTurnSpeed(){return turnspeed;};

	void setMaxSpeed(const s32& speed){maxspeed=speed;};
	void setCurSpeed(const s32& speed){curspeed=speed;};
	void setTurnSpeed(const s32& speed){turnspeed=speed;};

	void addCurSpeed(const s32& speed){curspeed+=speed; if(curspeed>maxspeed) curspeed=maxspeed;};
private:
	s32 maxspeed;
	s32 curspeed;
	s32 turnspeed;
};

#endif