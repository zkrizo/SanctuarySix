#include "player.h"
#include <Outland\Vector.h>


/****************************
* Constructor
****************************/
Player::Player():
	Object(Vector3<real64>(0,6,0),""),
	health(50),
	rotYaw(0),
	rotPitch(0),
	name("Default")
{}

/****************************
* Destructor
****************************/
Player::~Player()
{

}

void Player::movePosition(const Vector3<real64>& move)
{
	pos+=move;
}

void Player::Save(FILE *file)
{
	int namesize=name.size();
	fwrite(&health,sizeof(int),1,file);
	fwrite(&rotYaw,sizeof(rotYaw),1,file);
	fwrite(&rotPitch,sizeof(rotPitch),1,file);
	fwrite(&namesize,sizeof(int),1,file);
	for(int i(0);i<namesize;++i)
		fwrite(&name[i],sizeof(char),1,file);

	//call tech save method

	//call inventory save method
	inventory.SaveResources(file);
}

void Player::Load(FILE *file)
{
	s32 namesize;
	real32 ry(0),rp(0);
	std::string newname;
	fread(&health,sizeof(int),1,file);
	fread(&ry,sizeof(rotYaw),1,file);
	fread(&rp,sizeof(rotPitch),1,file);
	fread(&namesize,sizeof(int),1,file);
	newname.resize(namesize);
	for(int i(0);i<namesize;++i)
		fread(&newname[i],sizeof(char),1,file);

	name=newname;
	rotYaw=ry;
	rotPitch=rp;
	//call tech load method

	//call inventory load method
	inventory.LoadResources(file);
}