#include "TechManage.h"

TechManage::TechManage():
	scientists(1)
{
	tech=new Technology*[NUM_TECHS];
	
	for(int i=0;i<NUM_TECHS;i++)
	{
		tech[i]=new Technology;
	}

	//setup the basic structure for the technologies
	tech[TECH_ELECTRONICS]->setTech(0,1,0,TECH_NONE,TECH_NONE,TECH_NONE,0,0,0);
	tech[TECH_SCIENCE]->setTech(0,1,0,TECH_ELECTRONICS,TECH_NONE,TECH_NONE,2,0,0);
	tech[TECH_AGRICULTURE]->setTech(0,1,0,TECH_NONE,TECH_NONE,TECH_NONE,0,0,0);
	tech[TECH_ENERGY]->setTech(0,1,0,TECH_SCIENCE,TECH_ELECTRONICS,TECH_NONE,2,3,0);
}

TechManage::~TechManage()
{
	for(int i=0;i<NUM_TECHS;i++)
	{
		delete tech[i];
	}
	delete [] tech;
	
}

void TechManage::changeScientists(s32 num)
{
	scientists=scientists+num;
}

void TechManage::setScientists(s32 num)
{
	scientists=num;
}

s32 TechManage::getScientists()
{
	return scientists;
}