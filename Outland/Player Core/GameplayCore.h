#ifndef GameplayCore_H
#define GameplayCore_H

#include <Outland\UnitAbstraction.h>
#include <Outland\Vector.h>
#include "ResourceManager.h"
#include "player.h"
#include "Loader.h"
#include "BuildingManage.h"

class GameplayCore
{
public:
	static GameplayCore* Instance();
	void startup();
	void update(real32 time);
	void shutdown();
	void setSingleMode(tBOOL);
	tBOOL getSingleMode();
	void startSinglePlayer(bool newgame);
	void startMultiPlayer();

	Player				player;
	BuildingManage		bMan;
	Loader				levMan;
	ResourceManager		resourceMan;
private:
	GameplayCore();
	~GameplayCore();
	GameplayCore(const GameplayCore&);
	GameplayCore operator=(const GameplayCore&);

	tBOOL singlePlay;
	s32 resourceCounter;
};

#endif