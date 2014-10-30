#include "ResourceManager.h"
#include <Outland\Singletons.h>
#include <iostream>

using namespace std;

/****************************
* Constructor
****************************/
ResourceManager::ResourceManager()
{

}

/****************************
* Destructor
****************************/
ResourceManager::~ResourceManager()
{

}

void ResourceManager::startup()
{

}

void ResourceManager::update()
{
	vector<Player*>::iterator playerIt;

	for(playerIt=Players.begin();playerIt!=Players.end();playerIt++)
	{
		real32 powerCost,powerSupply=15,airSupply=5;
		real32 curPower=(*playerIt)->inventory.getResource(RES_CURPOWER);
		//calculate power cost
		powerCost=gameCore->bMan.powerUpdate(curPower);

		(*playerIt)->inventory.changeResource(RES_CURPOWER,powerSupply-powerCost);
		cout << "Power update: Current Power: " << (*playerIt)->inventory.getResource(RES_CURPOWER) << "\n";

		if(curPower+(powerSupply-powerCost)<0)
		{
			(*playerIt)->inventory.changeResource(RES_CURAIR,-1);
		}
		else
		{
			(*playerIt)->inventory.changeResource(RES_CURAIR,airSupply);
		}
	}
}

void ResourceManager::shutdown()
{
	for(int i=Players.size();i>0;i--)
	{
		Players.pop_back();
	}	
}

void ResourceManager::addPlayer(Player *player)
{
	Players.push_back(player);
}

/****************************
* checkResource
****************************/
s32 ResourceManager::checkResource(s32 resource,string player)
{
	vector<Player*>::iterator playerIt;

	for(playerIt=Players.begin();playerIt!=Players.end();playerIt++)
	{
		if((*playerIt)->name==player)
		{
			return (s32)((*playerIt)->inventory.getResource(resource));
		}
	}

	return 0;
}

/****************************
* changeResource
****************************/
void ResourceManager::changeResource(s32 resource, real32 add, std::string player)
{
	vector<Player*>::iterator playerIt;

	for(playerIt=Players.begin();playerIt!=Players.end();playerIt++)
	{
		if((*playerIt)->name==player)
		{
			(*playerIt)->inventory.changeResource(resource,add);
		}
	}
}

/****************************
* buildResources -	This function checks against the player's resources and
					subtracts the building cost from them if there are enough.
					Returns success or failure.
****************************/
tBOOL ResourceManager::checkRoomBuildResources(s32 buildType,s32 size,std::string playerName)
{
	vector<Player*>::iterator playerIt;

	for(playerIt=Players.begin();playerIt!=Players.end();playerIt++)
	{
		if((*playerIt)->name==playerName)
		{
			switch(buildType)
			{
			case RM_GHOST:
				return TRUE;
			case RM_HALLWAY: 
				if((*playerIt)->inventory.getResource(RES_STEEL) >= size*5)
				{
					(*playerIt)->inventory.changeResource(RES_STEEL,size*-5);
					return TRUE;
				}
				break;
			case RM_AIRLOCK: 
				if((*playerIt)->inventory.getResource(RES_STEEL) >= size*5)
				{
					(*playerIt)->inventory.changeResource(RES_STEEL,size*-5);
					return TRUE;
				}
				break;
			case RM_BATHROOM: 
				if((*playerIt)->inventory.getResource(RES_STEEL) >= size*5)
				{
					(*playerIt)->inventory.changeResource(RES_STEEL,size*-5);
					return TRUE;
				}
				break;
			case RM_COMMANDCENTER: 
				if((*playerIt)->inventory.getResource(RES_STEEL) >= size*5)
				{
					(*playerIt)->inventory.changeResource(RES_STEEL,size*-5);
					return TRUE;
				}
				break;
			case RM_FREIGHTAIRLOCK: 
				if((*playerIt)->inventory.getResource(RES_STEEL) >= size*5)
				{
					(*playerIt)->inventory.changeResource(RES_STEEL,size*-5);
					return TRUE;
				}
				break;
			case RM_GALLEY: 
				if((*playerIt)->inventory.getResource(RES_STEEL) >= size*5)
				{
					(*playerIt)->inventory.changeResource(RES_STEEL,size*-5);
					return TRUE;
				}
				break;
			case RM_INDUSTRIALWORKSHOP: 
				if((*playerIt)->inventory.getResource(RES_STEEL) >= size*5)
				{
					(*playerIt)->inventory.changeResource(RES_STEEL,size*-5);
					return TRUE;
				}
				break;
			case RM_LABORATORY: 
				if((*playerIt)->inventory.getResource(RES_STEEL) >= size*5)
				{
					(*playerIt)->inventory.changeResource(RES_STEEL,size*-5);
					return TRUE;
				}
				break;
			case RM_LIBRARY: 
				if((*playerIt)->inventory.getResource(RES_STEEL) >= size*5)
				{
					(*playerIt)->inventory.changeResource(RES_STEEL,size*-5);
					return TRUE;
				}
				break;
			case RM_LIVINGQUARTERS: 
				if((*playerIt)->inventory.getResource(RES_STEEL) >= size*5)
				{
					(*playerIt)->inventory.changeResource(RES_STEEL,size*-5);
					return TRUE;
				}
				break;
			case RM_MININGCONTROLCENTER: 
				if((*playerIt)->inventory.getResource(RES_STEEL) >= size*5)
				{
					(*playerIt)->inventory.changeResource(RES_STEEL,size*-5);
					return TRUE;
				}
				break;
			case RM_OXYGENSCRUBBER: 
				if((*playerIt)->inventory.getResource(RES_STEEL) >= size*5)
				{
					(*playerIt)->inventory.changeResource(RES_STEEL,size*-5);
					return TRUE;
				}
				break;
			case RM_POWERFUSION: 
				if((*playerIt)->inventory.getResource(RES_STEEL) >= size*5)
				{
					(*playerIt)->inventory.changeResource(RES_STEEL,size*-5);
					return TRUE;
				}
				break;
			case RM_WAREHOUSE: 
				if((*playerIt)->inventory.getResource(RES_STEEL) >= size*5)
				{
					(*playerIt)->inventory.changeResource(RES_STEEL,size*-5);
					return TRUE;
				}
				break;
			case RM_WATERCOLLECTOR: 
				if((*playerIt)->inventory.getResource(RES_STEEL) >= size*5)
				{
					(*playerIt)->inventory.changeResource(RES_STEEL,size*-5);
					return TRUE;
				}
				break;
			case RM_WATERRECYCLING: 
				if((*playerIt)->inventory.getResource(RES_STEEL) >= size*5)
				{
					(*playerIt)->inventory.changeResource(RES_STEEL,size*-5);
					return TRUE;
				}
				break;
			}
		}
	}

	return FALSE;
}

real32 ResourceManager::calcResources(s32 buildType,s32 size,s32 resource)
{
	real32 total=0;
	switch(buildType)
	{
	case RM_HALLWAY: 
		if(resource==RES_STEEL)
			total=size*5;
		break;
	case RM_AIRLOCK: 
				
		break;
	case RM_BATHROOM: 
		if(resource==RES_STEEL)
			total=size*5;		
		break;
	case RM_COMMANDCENTER: 
		if(resource==RES_STEEL)
			total=size*5;		
		break;
	case RM_FREIGHTAIRLOCK: 
				
		break;
	case RM_GALLEY: 
				
		break;
	case RM_INDUSTRIALWORKSHOP: 
				
		break;
	case RM_LABORATORY: 
				
		break;
	case RM_LIBRARY: 
				
		break;
	case RM_LIVINGQUARTERS: 
		if(resource==RES_STEEL)
			total=size*5;		
		break;
	case RM_MININGCONTROLCENTER: 
				
		break;
	case RM_OXYGENSCRUBBER: 
				
		break;
	case RM_POWERFUSION: 
				
		break;
	case RM_WAREHOUSE: 
				
		break;
	case RM_WATERCOLLECTOR: 
				
		break;
	case RM_WATERRECYCLING: 
				
		break;
	}

	return total;
}