#include "AICore.h"
#include <Outland\Vector.h>


/****************************
* Constructor
****************************/
AICore::AICore()
{

}

/****************************
* Destructor
****************************/
AICore::~AICore()
{
	while(!botList.empty())
	{
		botList.pop_back();
	}
}

/****************************
* Startup
****************************/
void AICore::Startup()
{

}

/****************************
* Update
****************************/
void AICore::Update()
{

}

/****************************
* Shutdown
****************************/
void AICore::Shutdown()
{

}

/****************************
* SaveBots
****************************/
void AICore::SaveBots()
{

}

/****************************
* LoadBots
****************************/
void AICore::LoadBots()
{

}

/****************************
* Render
****************************/
void AICore::Render()
{
	std::vector<AIEntity>::iterator it;
	it=botList.begin();

	while(it!=botList.end())
	{
		it->Render();
		it++;
	}
}