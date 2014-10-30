#ifndef ResourceManager_H
#define ResourceManager_H

#include <Outland\Player Core\player.h>
#include <string>
#include <vector>

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();


	void startup();
	void update();
	void shutdown();

	s32 checkResource(s32 resource,std::string player);
	void changeResource(s32 resource, real32 add, std::string player);
	tBOOL checkRoomBuildResources(s32 buildType,s32 size,std::string playerName);
	real32 calcResources(s32 buildType,s32 size,s32 resource);

	void addPlayer(Player* player);
private:
	std::vector<Player*> Players;
};

#endif