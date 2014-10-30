#include "Technology.h"

Technology::Technology():
	research(0),
	rate(1),
	level(1),
	req1(TECH_NONE),
	req2(TECH_NONE),
	req3(TECH_NONE),
	l1(0),
	l2(0),
	l3(0)
{}

Technology::~Technology()
{

}

Technology::Technology(real32 res,real32 r,s32 lev, s32 r1,s32 r2,s32 r3,s32 rl1,s32 rl2,s32 rl3):
	research(res),
	rate(r),
	level(lev),
	req1(r1),
	req2(r2),
	req3(r3),
	l1(rl1),
	l2(rl2),
	l3(rl3)
{}

void Technology::setTech(real32 res,real32 r,s32 lev)
{
	research=res;
	rate=r;
	level=lev;
}

void Technology::setTech(real32 res,real32 r,s32 lev, s32 r1,s32 r2,s32 r3,s32 rl1,s32 rl2,s32 rl3)
{
	research=res;
	rate=r;
	level=lev;
	req1=r1;
	req2=r2;
	req3=r3;
	l1=rl1;
	l2=rl2;
	l3=rl3;
}

void Technology::incrementResearch(real32 val)
{
	research=research+(val*rate);
	if(research>=100.0)
	{
		//increment the level of technology
		incLevel();

		//take away the 100%
		research=(real32)(research-100.0);

		//calculate the new rate - 3/4 for now
		rate=(real32)(rate*0.75);

		//For any leftover research, multiply it by the new rate
		research=research*rate;
	}
}

void Technology::incLevel()
{
	level=level+1;
}

s32 Technology::getLevel()
{
	return level;
}

s32 Technology::getReq1()
{
	return req1;
}

s32 Technology::getReq2()
{
	return req2;
}

s32 Technology::getReq3()
{
	return req3;
}

s32 Technology::getRL1()
{
	return l1;
}

s32 Technology::getRL2()
{
	return l2;
}

s32 Technology::getRL3()
{
	return l3;
}

