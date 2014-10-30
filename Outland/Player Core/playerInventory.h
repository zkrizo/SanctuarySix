#ifndef playerInventory_H
#define playerInventory_H

#include <Outland\UnitAbstraction.h>

class PlayerInventory
{
public:
	PlayerInventory();
	~PlayerInventory();
	real32 getResource(s32 resource);
	void changeResource(s32 resource, real32 add);
	void SaveResources(FILE *file);
	void LoadResources(FILE *file);
private:
	real32 resources[RES_NUMRESOURCES];

};

#endif