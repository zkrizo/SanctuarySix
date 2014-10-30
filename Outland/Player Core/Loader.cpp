#include "Loader.h"
#include <Outland\Singletons.h>
#include <string>
#include <sstream>
#include <direct.h>

extern int RENDER_DIST;
extern int CHUNK_SIZE;
extern int HEIGHT_CHUNK;

Loader::Loader():
	worldName("LevelTest")
{

}

Loader::~Loader()
{

}

/****************************
* startGame -	This function is responsible for loading the assets and
				displaying the loading screen to the player.
****************************/
void Loader::startGame(bool singlePlayer)
{
	//Call the loading screen to initialize and and start tracking items
	GUILoad->Render();

	//Final version will let the player choose planet type and location. For now, start a new world.
	uiCore->setState(STATE_GAME);
	gameCore->setSingleMode(TRUE);

	//Sends call to the graphics core to start the loading of the chunks and such
	grCore->startSingleGameplay(singlePlayer);
	gameCore->startSinglePlayer(singlePlayer);
}

/****************************
* Save -	This function is responsible for
			saving the current level to file
****************************/
void Loader::Save()
{
	FILE *file;
	errno_t err;
	
	

	std::string filename("saves/" + worldName + "/playerInfo.dat");
	
	err=fopen_s(&file,filename.c_str(),"wt");
	if(err==0)
	{
		gameCore->player.Save(file);
		fclose(file);
	}
	else if(err==2)
	{
		string dirname="saves/" + worldName;
		string dirname2="saves";
		//Directory has not been created yet
		err=_mkdir(dirname2.c_str());
		err=_mkdir(dirname.c_str());
		err=fopen_s(&file,filename.c_str(),"wb");
		if(err==0)
		{
			gameCore->player.Save(file);
			fclose(file);
		}
		else
		{
			logger->LogMessage(LOG_LOADER,"Error opening playerInfo.dat for writing. Ensure directory is not write locked!\n");
		}
	}
	else
	{
		logger->LogMessage(LOG_LOADER,"Error opening playerInfo.dat for writing. Ensure directory is not write locked!\n");
	}

	for(int i(0);i<RENDER_DIST;++i)
	{
		for(int j(0);j<HEIGHT_CHUNK;++j)
		{
			for(int k(0);k<RENDER_DIST;++k)
			{
				SaveChunk(grCore->chunkMan.p_chunks[i][j][k]);
			}
		}
	}

	//Calls the building manager class to create the save files for the buildings
	gameCore->bMan.SaveAllBuildings();
}

/****************************
* Load - This function is responsible for loading the
			world that is called
****************************/
void Loader::Load()
{
	s32 center;		//center index of the chunk
	Vector3<real64> centerPos(0,0,0);

	//Get player position - will have to load position from file if possible
	FILE *file;
	errno_t err;
	std::string filename="saves/" + worldName + "/playerInfo.dat";

	err=fopen_s(&file,filename.c_str(),"rt");

	if(err==0)
	{
		gameCore->player.Load(file);
		fclose(file);
	}
	else
	{
		//World has not been created yet, so create an arbitrary position for the player position
		centerPos=Vector3<real64>(0,0,0);
	}
	

	//Convert player position to chunk coordinates
	Vector3<s64> chunkPos;

	//Uses integer conversion to strip the one's digit and replace it with 0.
	chunkPos.x=(s64)centerPos.x/10*10;
	chunkPos.y=(s64)centerPos.y/10*10;
	chunkPos.z=(s64)centerPos.z/10*10;

	//Interpolates the center index based on if the render distance is even or odd
	if(float(RENDER_DIST%2)!=0)
	{
		center=RENDER_DIST/2-1;
	}
	else
	{
		center=RENDER_DIST/2;
	}

	//Loads the chunks
	for(int i=0;i<RENDER_DIST;i++)
	{
		for(int j=0;j<HEIGHT_CHUNK;j++)
		{
			for(int k=0;k<RENDER_DIST;k++)
			{
				Chunk temp(Vector3<s64>(chunkPos.x+i*10-center*10,chunkPos.y+j*10,chunkPos.z+k*10-center*10));
				LoadChunk(temp,temp.pos);
				//call the Individual chunk load algorithm with the newly determined coordinates
				grCore->chunkMan.p_chunks[i][j][k].CopyChunk(temp);
				GUILoad->incrementAndRender();
			}
		}
	}

	//Load the buildings
	gameCore->bMan.LoadAllBuildings();
}

std::string Loader::intToString(const s64& data)
{
	std::string val="";
	std::stringstream convert;
	convert << std::hex << data;
	
	val=convert.str();
	return val;
}

s64 Loader::stringToInt(std::string data)
{

	s64 val=0;

	return val;
}

void Loader::LoadChunk(Chunk & temp, const Vector3<s64>& pos)
{
	std::string filename;
	errno_t err;
	FILE *load;

	//Builds the dynamic filename for loading.
	filename="saves/" + worldName + "/";
	filename.append(intToString(pos.x));
	filename.append("x");
	filename.append(intToString(pos.y));
	filename.append("x");
	filename.append(intToString(pos.z));
	filename.append(".dat");

	err=fopen_s(&load,filename.c_str(), "rt");
	if(err==0)
	{
		//load block data
		for(int i(0);i<CHUNK_SIZE;++i)
		{
			for(int j(0);j<CHUNK_SIZE;++j)
			{
				for(int k(0);k<CHUNK_SIZE;++k)
				{
					//load block type
					s32 type(0);
					bool active(false);
					fread(&type,sizeof(s32),1,load);
					fread(&active,sizeof(bool),1,load);
					
					temp.blocks[i][j][k].setType(type);
					temp.blocks[i][j][k].setActive(active);

					//set position of the block inside chunk
					temp.blocks[i][j][k].setPos(Vector3<s32>(i,j,k));
				}
			}
		}
		fclose(load);
	}
	else
	{
		//File does not exist so landscape must be generated
		temp.Generate();
	}
}

void Loader::SaveChunk(const Chunk& data)
{
	std::string filename;
	errno_t err;
	FILE *load;

	//Builds the dynamic filename for loading.
	filename="saves/" + worldName + "/";
	filename.append(intToString(data.pos.x));
	filename.append("x");
	filename.append(intToString(data.pos.y));
	filename.append("x");
	filename.append(intToString(data.pos.z));
	filename.append(".dat");

	err=fopen_s(&load,filename.c_str(), "wt");
	if(err==0)
	{
		//save block data
		for(int i(0);i<CHUNK_SIZE;++i)
		{
			for(int j(0);j<CHUNK_SIZE;++j)
			{
				for(int k(0);k<CHUNK_SIZE;++k)
				{
					s32 type=data.blocks[i][j][k].getType();
					bool active=data.blocks[i][j][k].checkActive();
					//load block type
					fwrite(&type,sizeof(s32),1,load);

					//load active flag
					fwrite(&active,sizeof(bool),1,load);
				}
			}
		}
		

		fclose(load);
	}
	else
	{
		logger->LogMessage(LOG_LOADER,"Error writing to " + filename + ". Ensure directory is not write protected!\n");
	}
}
