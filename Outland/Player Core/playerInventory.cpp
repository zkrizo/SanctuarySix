#include "playerInventory.h"


/****************************
* Constructor
****************************/
PlayerInventory::PlayerInventory()
{
	//Initializes all resources to 0
	for(int i=0;i<RES_NUMRESOURCES;i++)
	{
		resources[i]=0;
	}
}

/****************************
* Destructor
****************************/
PlayerInventory::~PlayerInventory()
{

}

real32 PlayerInventory::getResource(s32 resource)
{
	if(resource>=0 && resource < RES_NUMRESOURCES)
		return resources[resource];
	else
		//throw exception instead of returning.
		return 0;
}

void PlayerInventory::changeResource(s32 resource, real32 add)
{
	resources[resource]+=add;

	if(resources[resource]<0)
		resources[resource]=0;

	else if(resource==RES_CURAIR)
	{
		if(resources[RES_CURAIR]>resources[RES_MAXAIR])
			resources[RES_CURAIR]=resources[RES_MAXAIR];
	}
	else if(resource==RES_CURPOWER)
	{
		if(resources[RES_CURPOWER]>resources[RES_MAXPOWER])
			resources[RES_CURPOWER]=resources[RES_MAXPOWER];
	}
}


void PlayerInventory::SaveResources(FILE *file)
{
	for(int i(0);i<RES_NUMRESOURCES;++i)
	{
		fwrite(&resources[i],sizeof(float),1,file);
	}
}

void PlayerInventory::LoadResources(FILE *file)
{
	for(int i(0);i<RES_NUMRESOURCES;++i)
	{
		fread(&resources[i],sizeof(float),1,file);
	}
}