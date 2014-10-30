#ifndef AIEntity_H
#define AIEntity_H

#include <Outland\UnitAbstraction.h>
#include <Outland\Vector.h>
#include <Outland\Quaternion.h>
#include "StateMachine.h"


class AIEntity
{
public:
	AIEntity();
	~AIEntity();

	void Update();
	void Render();
	void setPos(Vector3<real64> newpos);
	void setHealth(s32 health);
	void movePos(Vector3<real64> movepos);
	void changeHealth(s32 healthChange);

	//retrieval methods
	s32 getHealth();
	Vector3<real64> getPos();
	
private:
	Vector3<real64> position;
	s32				health;
	real32			rotate;
	StateMachine	StateController;
};

#endif