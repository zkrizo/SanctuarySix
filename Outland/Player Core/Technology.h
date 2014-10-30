#ifndef Technology_H
#define Technology_H

#include <Outland\UnitAbstraction.h>
#include <Outland\Vector.h>
#include <Outland\Quaternion.h>

class Technology
{
public:
	Technology();
	Technology(real32,real32,s32,s32,s32,s32,s32,s32,s32);
	~Technology();

	s32 getLevel();
	s32 getReq1();
	s32 getReq2();
	s32 getReq3();
	s32 getRL1();
	s32 getRL2();
	s32 getRL3();

	void setTech(real32 research,real32 rate,s32 level);	
	void setTech(real32 research,real32 rate,s32 level, s32 req1,s32 req2,s32 req3,s32 reqlev1,s32 reqlev2,s32 reqlev3);

	void incrementResearch(real32);
private:
	real32 research;
	real32 rate;
	s32 level;
	s32 req1;
	s32 req2;
	s32 req3;
	s32 l1;
	s32 l2;
	s32 l3;
	void incLevel();
};

#endif