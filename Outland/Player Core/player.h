#ifndef Player_H
#define Player_H

#include "playerInventory.h"
#include "TechManage.h"
#include <Outland\UnitAbstraction.h>
#include <Outland\Vector.h>
#include <Outland\Quaternion.h>
#include <Outland\Resource Core\Object.h>
#include <string>

class Player: public Object
{
public:
	Player();
	~Player();

	void movePosition(const Vector3<real64>& move);
	void Save(FILE* file);
	void Load(FILE* file);
	s32				health;
	real32			rotYaw;
	real32			rotPitch;
	std::string		name;
	PlayerInventory	inventory;
private:
	TechManage tech;
};

#endif