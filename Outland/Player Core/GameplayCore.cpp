#include "GameplayCore.h"
#include <Outland\Singletons.h>

GameplayCore* GameplayCore::Instance()
{
	static GameplayCore instance;
	return &instance;
}

GameplayCore::GameplayCore():
	player(),
	bMan(),
	levMan(),
	resourceMan(),
	singlePlay(FALSE),
	resourceCounter(0)
{}

GameplayCore::~GameplayCore()
{
}


void GameplayCore::startup()
{

}

void GameplayCore::update(real32 time)
{
	//Switch for state dependent behaviour
	switch(uiCore->getState())
	{
	case STATE_MAINMENU:
		break;
	/*case STATE_GAME:		//3rd person camera, player shouldn't be moving
		//Updates the player's resources
		if(resourceCounter==30)
		{
			resourceCounter=0;
			resourceMan.update();
		}
		else
			resourceCounter++;

		break;*/
	case STATE_FIRSTPERSON:
		//Updates the player's resources
		if(resourceCounter==30)
		{
			resourceCounter=0;
			resourceMan.update();
		}
		else
			resourceCounter++;

		break;
	default:
		//Updates the player's resources
		if(resourceCounter==30)
		{
			resourceCounter=0;
			resourceMan.update();
		}
		else
			resourceCounter++;

		break;
	}
}

void GameplayCore::shutdown()
{
	/*Gameplay core shutdown list:
		1.	Call saving methods to save all gameplay related data
			from player position to research and technology progress.
	*/
}

void GameplayCore::setSingleMode(tBOOL mode)
{
	singlePlay=mode;
}

tBOOL GameplayCore::getSingleMode()
{
	return singlePlay;
}

void GameplayCore::startSinglePlayer(bool newgame)
{
	if(newgame)
	{
		//Adds player to the resource management queue
		resourceMan.addPlayer(&player);

		//Sets the starting resources
		resourceMan.changeResource(RES_MAXPOWER,1000,"Default");
		resourceMan.changeResource(RES_MAXAIR,1000,"Default");
		resourceMan.changeResource(RES_CURPOWER,100,"Default");
		resourceMan.changeResource(RES_CURAIR,1000,"Default");
		resourceMan.changeResource(RES_STEEL,1500,"Default");
		resourceMan.changeResource(RES_FOOD,500,"Default");
		resourceMan.changeResource(RES_WATER,200,"Default");

		uiCore->UI_Tutorial.Startup();

		/****************************
		* TEST BUILDING ROUTINES! TO BE REMOVED ONCE BUILDING CODE IS FULLY FUNCTIONAL
		****************************/

		/*bMan.BuildRect(Vector3<s64>(0,7,-1),Vector3<s64>(5,7,-5),0,TRUE);
		bMan.BuildRect(Vector3<s64>(-1,7,4),Vector3<s64>(-1,7,-6),0,TRUE);
		bMan.BuildRect(Vector3<s64>(0,7,3),Vector3<s64>(0,7,3),0,TRUE);
		bMan.BuildRect(Vector3<s64>(3,7,3),Vector3<s64>(6,7,3),0,TRUE);
		bMan.BuildRect(Vector3<s64>(3,7,-2),Vector3<s64>(6,7,-7),0,TRUE);
		bMan.BuildRect(Vector3<s64>(0,7,0),Vector3<s64>(6,7,0),0,TRUE);
		bMan.BuildRect(Vector3<s64>(1,7,1),Vector3<s64>(1,7,3),0,TRUE);
		*/
		bMan.buildRoomGenerateGraph(Vector3<s64>(0,6,-2),Vector3<s64>(3,6,-5),RM_COMMANDCENTER,TRUE,false);
		//bMan.buildRoomGenerateGraph(Vector3<s64>(-2,6,-2),Vector3<s64>(-1,6,-3),RM_COMMANDCENTER,TRUE,false);
		//bMan.buildRoomGenerateGraph(Vector3<s64>(0,6,-7),Vector3<s64>(3,6,-7),RM_COMMANDCENTER,TRUE,false);
		bMan.buildRoomGenerateGraph(Vector3<s64>(-1,6,-3),Vector3<s64>(0,6,-7),RM_HALLWAY,TRUE,false);
		bMan.buildRoomGenerateGraph(Vector3<s64>(-2,6,-3),Vector3<s64>(-2,6,-5),RM_HALLWAY,TRUE,false);
		
		//bMan.BuildRect(Vector3<s64>(-2,6,-2),Vector3<s64>(-1,6,-3),RM_HALLWAY,TRUE,false);
		//bMan.addDoor(Vector3<s64>(-2,7,-2),RM_DOOR6);
		//bMan.addDoor(Vector3<s64>(-2,7,-2),RM_DOOR4);
		//bMan.addDoor(Vector3<s64>(-2,7,-2),RM_DOOR2);
		//bMan.addDoor(Vector3<s64>(-2,7,-2),RM_DOOR8);
		//bMan.addDoor(Vector3<s64>(-1,7,-5),RM_DOOR6);
		//bMan.addDoor(Vector3<s64>(-1,7,-4),RM_DOOR6);
		//bMan.BuildRect(Vector3<s64>(10,7,9),Vector3<s64>(13,7,6),RM_LIVINGQUARTERS,TRUE);
		//bMan.BuildRect(Vector3<s64>(14,7,9),Vector3<s64>(16,7,6),RM_BATHROOM,TRUE);
		//bMan.RemoveRect(Vector3<s64>(1,7,-2),Vector3<s64>(4,7,-4));
		//bMan.BuildRect(Vector3<s64>(-5,7,0),Vector3<s64>(-3,7,4),0,TRUE);

		/****************************
		* TEST BUILDING ROUTINES! TO BE REMOVED ONCE BUILDING CODE IS FULLY FUNCTIONAL
		****************************/
	}
	else
	{
		resourceMan.addPlayer(&player);
		levMan.Load();
	}
}