#ifndef Loader_H
#define Loader_H

#include <Outland\UnitAbstraction.h>
#include <Outland\Vector.h>
#include <Outland\Quaternion.h>
#include "player.h"
#include <Outland\Graphics Core\Chunk.h>

using namespace std;

class Loader
{
public:
	Loader();
	~Loader();
	
	void startGame(bool singlePlayer);
	void Load();
	void Save();
	void LoadChunk(Chunk& temp, const Vector3<s64>& pos);
	void SaveChunk(const Chunk& data);
	void setWorldName(const std::string& name){worldName=name;};
	std::string getWorldName(){return worldName;};
private:
	string intToString(const s64& data);
	s64 stringToInt(std::string data);
	std::string worldName;

};

#endif