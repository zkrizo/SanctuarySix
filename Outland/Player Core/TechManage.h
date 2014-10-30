#ifndef TechManage_H
#define TechManage_H

#include "Technology.h"

class TechManage
{
public:
	Technology **tech;

	TechManage();
	~TechManage();

	void update();
	void changeScientists(s32 num);
	void setScientists(s32 num);
	s32 getScientists();
private:
	s32 scientists;

};

#endif