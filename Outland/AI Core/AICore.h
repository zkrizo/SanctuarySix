#ifndef AICore_H
#define AICore_H

#include <Outland\UnitAbstraction.h>
#include <Outland\Vector.h>
#include <Outland\Quaternion.h>
#include <vector>
#include "AIEntity.h"

class AICore
{
public:
	AICore();
	~AICore();

	void Startup();
	void Update();
	void Shutdown();
	void Render();

	void SaveBots();
private:
	std::vector<AIEntity> botList;
	void LoadBots();
};

#endif