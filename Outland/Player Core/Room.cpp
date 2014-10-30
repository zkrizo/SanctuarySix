#include "Room.h"

/****************************
* Constructor
****************************/
Room::Room():
	pos(0,0,0),
	minX(0),
	minY(0),
	maxX(0),
	maxY(0),
	minZ(0),
	maxZ(0),
	power(0),
	powered(TRUE)
{}

/****************************
* Destructor
****************************/
Room::~Room()
{
	while(!blocks.empty())
	{
		blocks.pop_back();
	}
}

/****************************
* addBlock -	Adds the passed block to the vector for the room
****************************/
void Room::addBlock(const Building& block)
{
	Vector3<s32> posB=block.getPos();

	if(blocks.empty())
	{
		minX=posB.x;
		maxX=posB.x;
	}
	else if(posB.x<minX)
	{
		minX=posB.x;
	}
	else if(posB.x>maxX)
	{
		maxX=posB.x;
	}

	if(blocks.empty())
	{
		maxY=posB.y;
		minY=posB.y;
	}
	else if(posB.y>maxY)
		maxY=posB.y;
	else if(posB.y<minY)
		minY=posB.y;

	if(blocks.empty())
	{
		maxZ=posB.z;
		minZ=posB.z;
	}
	else if(posB.z>maxZ)
		maxZ=posB.z;
	else if(posB.z<minZ)
		minZ=posB.z;

	blocks.push_back(block);
}

/****************************
* removeBlock -	Removes the passed block from the room's vector

	HAS NOT BEEN IMPLEMENTED YET AS DESTRUCTION IS NOT IMPLEMENTED
****************************/
void Room::removeBlock(const Building& block)
{
}

/****************************
* Render -	Responsible for rendering the blocks of the rooms
****************************/
void Room::Render()
{
	if(!blocks.empty())
	{
		glPushMatrix();

		//translate to the beginning pos of the room
		glTranslatef((GLfloat)pos.x,(GLfloat)pos.y,(GLfloat)pos.z);

		//iterate through all of the room blocks and render each one
		for(std::vector<Building>::iterator it(blocks.begin());it!=blocks.end();it++)
		{
			it->Render(powered);
		}
	
		//pops the local room matrix off the stack
		glPopMatrix();
	}
}

/****************************
* checkPos -	Checks to see if the passed position is a block within the room
****************************/
tBOOL Room::checkPos(const Vector3<s64>& cpos)
{
	std::vector<Building>::iterator it;

	for(it=blocks.begin();it!=blocks.end();it++)
	{
		if(pos+Vector3<s64>(it->getPos().x,it->getPos().y,it->getPos().z)==cpos)
		{
			return TRUE;
		}
	}

	return FALSE;
}

/****************************
* alterMergeBlock -	Function used to alter the blocks within a room to visually and systematically change
					the blocks in preparation for merging with a newly constructed room.
****************************/
void Room::alterMergeBlock(const s32& xp, const s32& xm, const s32& zp, const s32& zm, const Vector3<s64>& bpos, const tBOOL& roomadd)
{
	//Need to figure out orientation, wall positions, and number of walls then use the data that was passed to destroy the walls that are
	//Being merged

	//Iterate to the block whose position is equal to the passed pos
	std::vector<Building>::iterator it;

	for(it=blocks.begin();it!=blocks.end();it++)
	{
		if((it->getPos().x+pos.x==bpos.x) && (it->getPos().y+pos.y==bpos.y) && (it->getPos().z+pos.z==bpos.z))
		{
			if(roomadd)
			{
				switch(it->getWalls())
				{
				case 0:
					break;
				case 1:
					switch(it->getOrient())
					{
					case 0:
						if(zp!=0)
						{
							it->setWalls(0);
						}
						break;
					case 90:
						if(xp!=0)
						{
							it->setWalls(0);
							it->setOrient(0);
						}
						break;
					case 180:
						if(zm!=0)
						{
							it->setWalls(0);
							it->setOrient(0);
						}
						break;
					case 270:
						if(xm!=0)
						{
							it->setWalls(0);
							it->setOrient(0);
						}
						break;
					default:
						break;
					}
					break;
				case 2:
					if(it->getWallType()==1)
					{
						switch(it->getOrient())
						{
						case 0:
							if(zp!=0)
							{
								if(zm!=0)
								{
									it->setWallType(0);
									it->setWalls(0);
								}
								else
								{
									it->setWallType(0);
									it->setWalls(1);
									it->setOrient(180);
								}
							}
							else
							{
								if(zm!=0)
								{
									it->setWallType(0);
									it->setWalls(1);
								}
							}
							break;
						case 90:
							if(xp!=0)
							{
								if(xm!=0)
								{
									it->setWallType(0);
									it->setWalls(0);
								}
								else
								{
									it->setWallType(0);
									it->setWalls(1);
									it->setOrient(270);
								}
							}
							else
							{
								if(xm!=0)
								{
									it->setWallType(0);
									it->setWalls(1);
								}
							}
							break;
						case 180:
							if(zp!=0)
							{
								if(zm!=0)
								{
									it->setWallType(0);
									it->setWalls(0);
								}
								else
								{
									it->setWallType(0);
									it->setWalls(1);
									it->setOrient(180);
								}
							}
							else
							{
								if(zm!=0)
								{
									it->setWallType(0);
									it->setWalls(1);
									it->setOrient(0);
								}
							}
							break;
						case 270:
							if(xp!=0)
							{
								if(xm!=0)
								{
									it->setWallType(0);
									it->setWalls(0);
								}
								else
								{
									it->setWallType(0);
									it->setWalls(1);
									it->setOrient(270);
								}
							}
							else
							{
								if(xm!=0)
								{
									it->setWallType(0);
									it->setOrient(90);
									it->setWalls(1);
								}
							}
							break;
						}
					}
					else
					{
						switch(it->getOrient())
						{
						case 0:
							if(zp!=0)
							{
								if(xp!=0)
								{
									it->setWalls(0);
								}
								else
								{
									it->setWalls(1);
									it->setOrient(90);
								}
							}
							else
							{
								if(xp!=0)
								{
									it->setWalls(1);
								}
							}
							break;
						case 90:
							if(xp!=0)
							{
								if(zm!=0)
								{
									it->setWalls(0);
								}
								else
								{
									it->setWalls(1);
									it->setOrient(180);
								}
							}
							else
							{
								if(zm!=0)
								{
									it->setWalls(1);
								}
							}
							break;
						case 180:
							if(zm!=0)
							{
								if(xm!=0)
								{
									it->setWalls(0);
								}
								else
								{
									it->setWalls(1);
									it->setOrient(270);
								}
							}
							else
							{
								if(xm!=0)
								{
									it->setWalls(1);
								}
							}
							break;
						case 270:
							if(xm!=0)
							{
								if(zp!=0)
								{
									it->setWalls(0);
								}
								else
								{
									it->setWalls(1);
									it->setOrient(0);
								}
							}
							else
							{
								if(zp!=0)
								{
									it->setWalls(1);	
								}
							}
							break;
						default:
							break;
						}
					}
					break;
				case 3:
					switch(it->getOrient())
					{
					case 0:
						if(zp!=0)
						{
							if(xp!=0)
							{
								if(zm!=0)
								{
									it->setWalls(0);
								}
								else
								{
									it->setWalls(1);
									it->setOrient(180);
								}
							}
							else
							{
								if(zm!=0)
								{
									it->setWalls(1);
									it->setOrient(90);
								}
								else
								{
									it->setWalls(2);
									it->setOrient(90);
								}
							}
						}
						else
						{
							if(xp!=0)
							{
								if(zm!=0)
								{
									it->setWalls(1);
								}
								else
								{
									it->setWalls(2);
									it->setWallType(1);
								}
							}
							else
							{
								if(zm!=0)
								{
									it->setWalls(2);
								}
							}
						}
						break;
					case 90:
						if(xp!=0)
						{
							if(zm!=0)
							{
								if(xm!=0)
								{
									it->setWalls(0);
								}
								else
								{
									it->setWalls(1);
									it->setOrient(270);
								}
							}
							else
							{
								if(xm!=0)
								{
									it->setWalls(1);
									it->setOrient(180);
								}
								else
								{
									it->setWalls(2);
									it->setOrient(180);
								}
							}
						}
						else
						{
							if(zm!=0)
							{
								if(xm!=0)
								{
									it->setWalls(1);
								}
								else
								{
									it->setWalls(2);
									it->setWallType(1);
								}
							}
							else
							{
								if(xm!=0)
								{
									it->setWalls(2);
								}
							}
						}
						break;
					case 180:
						if(zm!=0)
						{
							if(xm!=0)
							{
								if(zp!=0)
								{
									it->setWalls(0);
								}
								else
								{
									it->setWalls(1);
									it->setOrient(0);
								}
							}
							else
							{
								if(zp!=0)
								{
									it->setWalls(1);
									it->setOrient(270);
								}
								else
								{
									it->setWalls(2);
									it->setOrient(270);
								}
							}
						}
						else
						{
							if(xm!=0)
							{
								if(zp!=0)
								{
									it->setWalls(1);
								}
								else
								{
									it->setWalls(2);
									it->setWallType(1);
								}
							}
							else
							{
								if(zp!=0)
								{
									it->setWalls(2);	
								}
							}
						}
						break;
					case 270:
						if(xm!=0)
						{
							if(zp!=0)
							{
								if(xp!=0)
								{
									it->setWalls(0);
								}
								else
								{
									it->setWalls(1);
									it->setOrient(90);
								}
							}
							else
							{
								if(xp!=0)
								{
									it->setWalls(1);
									it->setOrient(0);
								}
								else
								{
									it->setWalls(2);
									it->setOrient(0);
								}
							}
						}
						else
						{
							if(zp!=0)
							{
								if(xp!=0)
								{
									it->setWalls(1);
								}
								else
								{
									it->setWalls(2);
									it->setWallType(1);
								}
							}
							else
							{
								if(xp!=0)
								{
									it->setWalls(2);
								}
							}
						}
						break;
					default:
						break;
					}
					break;
				case 4:
					if(zp!=0)
					{
						if(xp!=0)
						{
							if(zm!=0)
							{
								if(xm!=0)
								{
									it->setWalls(0);
								}
								else
								{
									it->setWalls(1);
									it->setOrient(270);
								}
							}
							else
							{
								if(xm!=0)
								{
									it->setWalls(1);
									it->setOrient(180);
								}
								else
								{
									it->setWalls(2);
									it->setOrient(180);
								}
							}
						}
						else
						{
							if(zm!=0)
							{
								if(xm!=0)
								{
									it->setWalls(1);
									it->setOrient(90);
								}
								else
								{
									it->setWalls(2);
									it->setWallType(1);
									it->setOrient(90);
								}
							}
							else
							{
								if(xm!=0)
								{
									it->setWalls(2);
									it->setOrient(90);
								}
								else
								{
									it->setWalls(3);
									it->setOrient(90);
								}
							}
						}
					}
					else
					{
						if(xp!=0)
						{
							if(zm!=0)
							{
								if(xm!=0)
								{
									it->setWalls(1);
									it->setOrient(0);
								}
								else
								{
									it->setWalls(2);
									it->setOrient(270);
								}
							}
							else
							{
								if(xm!=0)
								{
									it->setWalls(2);
									it->setWallType(1);
									it->setOrient(0);
								}
								else
								{
									it->setWalls(3);
									it->setOrient(180);
								}
							}
						}
						else
						{
							if(zm!=0)
							{
								if(xm!=0)
								{
									it->setWalls(2);
									it->setOrient(0);
								}
								else
								{
									it->setWalls(3);
									it->setOrient(270);
								}
							}
							else
							{
								if(xm!=0)
								{
									it->setWalls(3);
									it->setOrient(0);
								}
							}
						}
					}
					break;
				default:
					break;
				}
				
				//exit the for loop
				break;
			}
			else //room was removed so walls need to be added
			{
				switch(it->getWalls())
				{
				case 0:
					if(zp!=0)
					{
						it->setWalls(1);
						it->setOrient(0);
					}
					else if(xp!=0)
					{
						it->setWalls(1);
						it->setOrient(90);
					}
					else if(zm!=0)
					{
						it->setWalls(1);
						it->setOrient(180);
					}
					else if(xm!=0)
					{
						it->setWalls(1);
						it->setOrient(270);
					}
						
					break;
				case 1:
					switch(it->getOrient())
					{
					case 0:
						if(xp!=0)
						{
							it->setWalls(2);
							it->setOrient(0);
						}
						else if(zm!=0)
						{
							it->setWallType(1);
							it->setWalls(2);
						}
						else if(xm!=0)
						{
							it->setWalls(2);
							it->setOrient(270);
						}
						break;
					case 90:
						if(zp!=0)
						{
							it->setWalls(2);
							it->setOrient(0);
						}
						else if(zm!=0)
						{
							it->setWalls(2);
						}
						else if(xm!=0)
						{
							it->setWalls(2);
							it->setWallType(1);
						}
						break;
					case 180:
						if(zp!=0)
						{
							it->setWalls(2);
							it->setWallType(1);
						}
						else if(xp!=0)
						{
							it->setWalls(2);
							it->setOrient(90);
						}
						else if(xm!=0)
						{
							it->setWalls(2);
						}
						break;
					case 270:
						if(zp!=0)
						{
							it->setWalls(2);
						}
						else if(xp!=0)
						{
							it->setWalls(2);
							it->setWallType(1);
						}
						else if(zm!=0)
						{
							it->setWalls(2);
							it->setOrient(180);
						}
						break;
					default:
						break;
					}
					break;
				case 2:
					if(it->getWallType()==0)
					{
						switch(it->getOrient())
						{
						case 0:
							if(zm!=0)
							{
								it->setWalls(3);
							}
							else if(xm!=0)
							{
								it->setWalls(3);
								it->setOrient(270);
							}
							break;
						case 90:
							if(zp!=0)
							{
								it->setWalls(3);
								it->setOrient(0);
							}
							else if(xm!=0)
							{
								it->setWalls(3);
							}
							break;
						case 180:
							if(zp!=0)
							{
								it->setWalls(3);
							}
							else if(xp!=0)
							{
								it->setWalls(3);
								it->setOrient(90);
							}
							break;
						case 270:
							if(xp!=0)
							{
								it->setWalls(3);
							}
							else if(zm!=0)
							{
								it->setWalls(3);
								it->setOrient(180);
							}
							break;
						default:
							break;
						}
					}
					else //Corridor piece
					{
						switch(it->getOrient())
						{
						case 0:
							if(xp!=0)
							{
								it->setWalls(3);
								it->setWallType(0);
							}
							else if(xm!=0)
							{
								it->setWalls(3);
								it->setOrient(180);
								it->setWallType(0);
							}
							break;
						case 90:
							if(zp!=0)
							{
								it->setWalls(3);
								it->setOrient(270);
								it->setWallType(0);
							}
							else if(zm!=0)
							{
								it->setWalls(3);
								it->setWallType(0);
							}
							break;
						case 180:
							if(xp!=0)
							{
								it->setWalls(3);
								it->setOrient(0);
								it->setWallType(0);
							}
							else if(xm!=0)
							{
								it->setWalls(3);
								it->setOrient(180);
								it->setWallType(0);
							}
							break;
						case 270:
							if(zp!=0)
							{
								it->setWalls(3);
								it->setOrient(270);
								it->setWallType(0);
							}
							else if(zm!=0)
							{
								it->setWalls(3);
								it->setWallType(0);
							}
							break;
						default:
							break;
						}
					}
					break;
				case 3:
					switch(it->getOrient())
					{
					case 0:
						if(xm!=0)
						{
							it->setWalls(4);
						}
						break;
					case 90:
						if(zp!=0)
						{
							it->setWalls(4);
						}
						break;
					case 180:
						if(xp!=0)
						{
							it->setWalls(4);
						}
						break;
					case 270:
						if(zm!=0)
						{
							it->setWalls(4);
						}
						break;
					default:
						break;
					}
					break;
				default:	//Breaks if there are 4 walls or invalid input
					break;
				}

				//exit the for loop
				break;
			}
		}
	}
}

void Room::alterMergeBlock(const int& faceData, const Vector3<s64>& bpos, const bool& add)
{
	std::vector<Building>::iterator it;

	for(it=blocks.begin();it!=blocks.end();it++)
	{
		if((it->getPos().x+pos.x==bpos.x) && (it->getPos().y+pos.y==bpos.y) && (it->getPos().z+pos.z==bpos.z))
		{
			if(!add)
			{
				int removeWallBitmask=it->getWallBitmask()&faceData;
				it->removeWall(removeWallBitmask);
			}
			else
			{
				it->addWall(faceData);
			}
		}
	}
}

void Room::removeBlock(const Vector3<s64>& npos)
{
	std::vector<Building>::iterator it;

	for(it=blocks.begin();it!=blocks.end();it++)
	{
		if((pos.x+it->getPos().x)==npos.x && (pos.y+it->getPos().y)==npos.y && (pos.z+it->getPos().z)==npos.z)
		{
			blocks.erase(it);
			break;
		}
	}
}

void Room::recalcPower()
{
	if(blocks.size()!=0)
	{
		switch(blocks[0].getRoomType())
		{
			//Tech 1 Rooms:
		case RM_HALLWAY:
		case RM_AIRLOCK:
		case RM_BATHROOM:
		case RM_COMMANDCENTER:
		case RM_FREIGHTAIRLOCK:
		case RM_GALLEY:
		case RM_INDUSTRIALWORKSHOP:
		case RM_LABORATORY:
		case RM_LIBRARY:
		case RM_LIVINGQUARTERS:
		case RM_MININGCONTROLCENTER:
		case RM_OXYGENSCRUBBER:
		case RM_POWERFUSION:
		case RM_WAREHOUSE:
		case RM_WATERCOLLECTOR:
		case RM_WATERRECYCLING:
			power=(float)(blocks.size()*1*0.75*0.25);

			//Tech 2 Rooms:
		}
	}
	else
	{
		power=0;
	}
}

void Room::recalcMaxMin()
{
	std::vector<Building>::iterator it;

	for(it=blocks.begin();it!=blocks.end();it++)
	{
		if(it->getPos().x<minX)
		{
			minX=it->getPos().x;
		}
		else if(it->getPos().x>maxX)
		{
			maxX=it->getPos().x;
		}
		if(it->getPos().y<minY)
		{
			minY=it->getPos().y;
		}
		else if(it->getPos().y>maxY)
		{
			maxY=it->getPos().y;
		}
		if(it->getPos().z<minZ)
		{
			minZ=it->getPos().z;
		}
		else if(it->getPos().z>maxZ)
		{
			maxZ=it->getPos().z;
		}
	}
}

void Room::addDoor(const Vector3<s64>& cpos,const s32& direction)
{
	std::vector<Building>::iterator it;
	s32 ndir=0;

	for(it=blocks.begin();it!=blocks.end();it++)
	{
		if((Vector3<s32>(pos.x,pos.y,pos.z)+it->getPos())==Vector3<s32>(cpos.x,cpos.y,cpos.z))
		{
			switch(it->getOrient())
			{
			case 0:
				switch(direction)
				{
				case RM_DOOR8:
					ndir=RM_DOOR2;
					break;
				case RM_DOOR6:
					ndir=RM_DOOR4;
					break;
				case RM_DOOR2:
					ndir=RM_DOOR8;
					break;
				case RM_DOOR4:
					ndir=RM_DOOR6;
					break;
				}
				break;
			case 90:
				switch(direction)
				{
				case RM_DOOR8:
					ndir=RM_DOOR4;
					break;
				case RM_DOOR6:
					ndir=RM_DOOR8;
					break;
				case RM_DOOR2:
					ndir=RM_DOOR6;
					break;
				case RM_DOOR4:
					ndir=RM_DOOR2;
					break;
				}
				break;
			case 180:
				ndir=direction;
				break;
			case 270:
				switch(direction)
				{
				case RM_DOOR8:
					ndir=RM_DOOR6;
					break;
				case RM_DOOR6:
					ndir=RM_DOOR2;
					break;
				case RM_DOOR2:
					ndir=RM_DOOR4;
					break;
				case RM_DOOR4:
					ndir=RM_DOOR8;
					break;
				}
				break;
			}
			it->addDoor(ndir);
		}
	}
}

void Room::updateCorners()
{
	Building *xp,*xm,*zp,*zm;
	s32 vzp(0),vxp(0),vzm(0),vxm(0),corners(0);

	for(std::vector<Building>::iterator it(blocks.begin());it!=blocks.end();it++)
	{
		vzp=Face_INVALID;vxp=Face_INVALID;vzm=Face_INVALID;vxm=Face_INVALID,corners=0;
		zp=getBuilding(Vector3<s32>(0,0,1)+it->getPos());
		xp=getBuilding(Vector3<s32>(1,0,0)+it->getPos());
		zm=getBuilding(Vector3<s32>(0,0,-1)+it->getPos());
		xm=getBuilding(Vector3<s32>(-1,0,0)+it->getPos());

		if(zp!=NULL )
		{
			vzp=zp->getWallBitmask();
		}

		if(zm!=NULL)
		{
			vzm=zm->getWallBitmask();
		}

		if(xp!=NULL)
		{
			vxp=xp->getWallBitmask();
		}

		if(xm!=NULL)
		{
			vxm=xm->getWallBitmask();
		}

		if(((vzm&Face_XP)==Face_XP) &&
			((vzm&Face_ZP)==0) &&
			((vxp&Face_ZM)==Face_ZM) &&
			((vxp&Face_XM)==0))
		{
			if((it->getWallBitmask()&(Face_ZM|Face_XP))==0)
			{
				it->addCorner(Corner9);
			}
		}
		
		if(((vzm&Face_XM)==Face_XM) &&
			((vzm&Face_ZP)==0) &&
			((vxm&Face_ZM)==Face_ZM) &&
			((vxm&Face_XP)==0))
		{
			if((it->getWallBitmask()&(Face_ZM|Face_XM))==0)
			{
				it->addCorner(Corner7);
			}
		}
		
		if(((vzp&Face_XM)==Face_XM) &&
			((vzp&Face_ZM)==0) &&
			((vxm&Face_ZP)==Face_ZP) &&
			((vxm&Face_XP)==0))
		{
			if((it->getWallBitmask()&(Face_XM|Face_ZP))==0)
			{
				it->addCorner(Corner1);
			}
		}
		
		if(((vzp&Face_XP)==Face_XP) &&
			((vzp&Face_ZM)==0) &&
			((vxp&Face_ZP)==Face_ZP) &&
			((vxp&Face_XM)==0))
		{
			if((it->getWallBitmask()&(Face_ZP|Face_XP))==0)
			{
				it->addCorner(Corner3);
			}
		}
	}
}

/*void Room::updateCorners()
{
	Building *xp,*xm,*zp,*zm;
	s32 vzp(0),vxp(0),vzm(0),vxm(0),corners(0);

	for(std::vector<Building>::iterator it(blocks.begin());it!=blocks.end();it++)
	{
		vzp=0;vxp=0;vzm=0;vxm=0,corners=0;
		zp=getBuilding(Vector3<s32>(0,0,1)+it->getPos());
		xp=getBuilding(Vector3<s32>(1,0,0)+it->getPos());
		zm=getBuilding(Vector3<s32>(0,0,-1)+it->getPos());
		xm=getBuilding(Vector3<s32>(-1,0,0)+it->getPos());

		if(zp!=NULL)
		{
			switch(zp->getWalls())
			{
			case 1:
				switch(zp->getOrient())
				{
				case 90:
					vzp=2;
					break;
				case 270:
					vzp=1;
					break;
				}
				break;
			case 2:
				if(zp->getWallType()==1)
				{
					vzp=3;
				}
				else
				{
					switch(zp->getOrient())
					{
					case 0:
						vzp=2;
						break;
					case 270:
						vzp=1;
						break;
					}
				}
				break;
			case 3:
				vzp=3;
				break;
			}
		}

		if(xp!=NULL)
		{
			switch(xp->getWalls())
			{
			case 1:
				switch(xp->getOrient())
				{
				case 0:
					vxp=1;
					break;
				case 180:
					vxp=2;
					break;
				}
				break;
			case 2:
				if(xp->getWallType()==1)
				{
					vxp=3;
				}
				else
				{
					switch(xp->getOrient())
					{
					case 0:
						vxp=1;
						break;
					case 90:
						vxp=2;
						break;
					}
				}
				break;
			case 3:
				vxp=3;
				break;
			}
		}

		if(zm!=NULL)
		{
			switch(zm->getWalls())
			{
			case 1:
				switch(zm->getOrient())
				{
				case 90:
					vzm=1;
					break;
				case 270:
					vzm=2;
					break;
				}
				break;
			case 2:
				if(zm->getWallType()==1)
				{
					vzm=3;
				}
				else
				{
					switch(zm->getOrient())
					{
					case 90:
						vzm=1;
						break;
					case 180:
						vzm=2;
						break;
					}
				}
				break;
			case 3:
				vzm=3;
				break;
			}
		}

		if(xm!=NULL)
		{
			switch(xm->getWalls())
			{
			case 1:
				switch(xm->getOrient())
				{
				case 0:
					vxm=2;
					break;
				case 180:
					vxm=1;
					break;
				}
				break;
			case 2:
				if(xm->getWallType()==1)
				{
					vxm=3;
				}
				else
				{
					switch(xm->getOrient())
					{
					case 180:
						vxm=1;
						break;
					case 270:
						vxm=2;
						break;
					}
				}
				break;
			case 3:
				vxm=3;
				break;
			}
		}

		switch(it->getWalls())
		{
		case 0:
			switch(vzp)
			{
			case 0:
				switch(vzm)
				{
				case 0:
					break;
				case 1:
					if(vxp==2 || vxp==3)
					{
						corners=RM_CORNER9;
					}
					break;
				case 2:
					if(vxm==1 || vxm==3)
					{
						corners=RM_CORNER7;
					}
					break;
				case 3:
					if(vxm==1||vxm==3)
					{
						if(vxp==2||vxp==3)
						{
							corners=RM_CORNER97;
						}
						else
						{
							corners=RM_CORNER7;
						}
					}
					else
					{
						if(vxp==2 || vxp==3)
						{
							corners=RM_CORNER9;
						}
					}
					break;
				}
				break;
			case 1: //RM_CORNER1 possible
				switch(vzm)
				{
				case 0:
					if(vxm==2 || vxm==3)
					{
						corners=RM_CORNER1;
					}
					break;
				case 1: //RM_CORNER1 possible, RM_CORNER9 possible
					if(vxp==2 || vxp==3)
					{
						if(vxm==2 || vxm==3)
						{
							corners=RM_CORNER91;
						}
						else
						{
							corners=RM_CORNER9;
						}
					}
					else
					{
						if(vxm==2 || vxm==3)
						{
							corners=RM_CORNER1;
						}
					}
					break;
				case 2: //RM_CORNER1 possible, RM_CORNER7 possible
					if(vxm==1)
					{
						corners=RM_CORNER7;
					}
					else if(vxm==2)
					{
						corners=RM_CORNER1;
					}
					else if(vxm==3)
					{
						corners=RM_CORNER17;
					}
					break;
				case 3: //RM_CORNER1 possible, RM_CORNER97 possible
					if(vxp==2 || vxp==3)
					{
						if(vxm==1)
						{
							corners=RM_CORNER97;
						}
						else if(vxm==2)
						{
							corners=RM_CORNER91;
						}
						else if(vxm==3)
						{
							corners=RM_CORNER917;
						}
						else
						{
							corners=RM_CORNER9;
						}
					}
					else
					{
						if(vxm==1)
						{
							corners=RM_CORNER7;
						}
						else if(vxm==2)
						{
							corners=RM_CORNER1;
						}
						else if(vxm==3)
						{
							corners=RM_CORNER17;
						}
					}
					break;
				}
				break;
			case 2: //RM_CORNER3 possible
				switch(vxp)
				{
				case 0:
					if(vzm==2 || vzm==3)
					{
						if(vxm==1 || vxm==3)
						{
							corners=RM_CORNER7;
						}
					}
					break;
				case 1:
					if(vzm==2 || vzm==3)
					{
						if(vxm==1 || vxm==3)
						{
							corners=RM_CORNER37;
						}
						else
						{
							corners=RM_CORNER3;
						}
					}
					else
					{
						corners=RM_CORNER3;
					}
					break;
				case 2: // RM_CORNER 9 and 7 possible
					switch(vzm)
					{
					case 1:
						corners=RM_CORNER9;
						break;
					case 2:
						if(vxm==1 || vxm==3)
							corners=RM_CORNER7;
						break;
					case 3:
						if(vxm==1 || vxm==3)
							corners=RM_CORNER97;
						else
							corners=RM_CORNER9;
						break;
					}
					break;
				case 3://RM_CORNER3 valid, 9 possible
					switch(vzm)
					{
					case 0:
						corners=RM_CORNER3;
						break;
					case 1://RM_CORNER3 valid, 9 valid
						corners=RM_CORNER93;
						break;
					case 2://RM_CORNER3 valid, 7 possible
						if(vxm==1 || vxm==3)
							corners=RM_CORNER37;
						else
							corners=RM_CORNER3;
						break;
					case 3://RM_CORNER3 valid, 9 valid, 7 possible
						if(vxm==1 || vxm==3)
							corners=RM_CORNER937;
						else
							corners=RM_CORNER93;
						break;
					}
					break;
				}
				break;
			case 3: //RM_CORNER31 possible
				switch(vxp)
				{
				case 0: //RM_CORNER 3 and 9 impossible, 1 possible
					switch(vzm)
					{
					case 0: //1 possible
					case 1: //1 possible
						if(vxm==2 || vxm==3)
							corners=RM_CORNER1;
						break;
					case 2: //7 and 1 possible
					case 3: //7 and 1 possible
						switch(vxm)
						{
						case 1:
							corners=RM_CORNER7;
							break;
						case 2:
							corners=RM_CORNER1;
							break;
						case 3:
							corners=RM_CORNER17;
							break;
						}
						break;
					}
					break;
				case 1: //RM_CORNER3 valid, 1 possible
					switch(vzm)
					{
					case 0:
					case 1:
						if(vxm==2||vxm==3)
						{
							corners=RM_CORNER31;
						}
						else
						{
							corners=RM_CORNER3;
						}
						break;
					case 2:
					case 3:
						if(vxm==1)
						{
							corners=RM_CORNER37;
						}
						else if(vxm==2)
						{
							corners=RM_CORNER31;
						}
						else if(vxm==3)
						{
							corners=RM_CORNER317;
						}
						else
						{
							corners=RM_CORNER3;
						}
						break;
					}
					break;
				case 2: //RM_CORNER9 and 1 possible
					switch(vzm)
					{
					case 0:
						if(vxm==2 || vxm==3)
							corners=RM_CORNER1;
						break;
					case 1:
						switch(vxm)
						{
						case 0:
						case 1:
							corners=RM_CORNER9;
							break;
						case 2:
						case 3:
							corners=RM_CORNER91;
							break;
						}
						break;
					case 2:
						switch(vxm)
						{
						case 0:
						case 1:
							corners=RM_CORNER7;
							break;
						case 2:
						case 3:
							corners=RM_CORNER1;
							break;
						}
						break;
					case 3:
						switch(vxm)
						{
						case 0:
							corners=RM_CORNER9;
							break;
						case 1:
							corners=RM_CORNER97;
							break;
						case 2:
							corners=RM_CORNER91;
						case 3:
							corners=RM_CORNER917;
							break;
						}
						break;
					}
					break;
				case 3: //RM_CORNER3 valid, 9 and 1 possible
					switch(vzm)
					{
					case 0:
						switch(vxm)
						{
						case 0:
						case 1:
							corners=RM_CORNER3;
							break;
						case 2:
						case 3:
							corners=RM_CORNER31;
							break;
						}
						break;
					case 1: //3 and 9 valid 1 possible
						if(vxm==2 || vxm==3)
							corners=RM_CORNER931;
						else
							corners=RM_CORNER93;
						break;
					case 2:
						switch(vxm)
						{
						case 0:
							corners=RM_CORNER3;
							break;
						case 1:
							corners=RM_CORNER37;
							break;
						case 2:
							corners=RM_CORNER31;
							break;
						case 3:
							corners=RM_CORNER317;
							break;
						}
						break;
					case 3:
						switch(vxm)
						{
						case 0:
							corners=RM_CORNER93;
							break;
						case 1:
							corners=RM_CORNER937;
							break;
						case 2:
							corners=RM_CORNER931;
							break;
						case 3:
							corners=RM_CORNER9317;
							break;
						}
						break;
					}
					break;
				}
				break;
			}
			break;
		case 1: 
			switch(it->getOrient())
			{
			case 0:	//Wall at ZP
				switch(vzm)
				{
				case 1:
					if(vxp==2 || vxp==3)
					{
						corners=RM_CORNER9;
					}
					break;
				case 2:
					if(vxm==1 || vxm==3)
					{
						corners=RM_CORNER7;
					}
					break;
				case 3:
					if(vxm==1 || vxm==3)
					{
						if(vxp==2 || vxp==3)
						{
							corners=RM_CORNER97;
						}
						else
						{
							corners=RM_CORNER7;
						}
					}
					else
					{
						if(vxp==2 || vxp==3)
						{
							corners=RM_CORNER9;
						}
					}
					break;
				}
				break;
			case 90: //Wall at xP
				switch(vxm)
				{
				case 1:
					if(vzm==2 || vzm==3)
					{
						corners=RM_CORNER9;
					}
					break;
				case 2:
					if(vzp==1 || vzp==3)
					{
						corners=RM_CORNER7;
					}
					break;
				case 3:
					if(vzm==2 || vzm==3)
					{
						if(vzp==1 || vzp==3)
						{
							corners=RM_CORNER97;
						}
						else
						{
							corners=RM_CORNER7;
						}
					}
					else
					{
						if(vzp==1 || vzp==3)
						{
							corners=RM_CORNER9;
						}
					}
					break;
				}
				break;
			case 180:
				switch(vzp)
				{
				case 1:
					if(vxm==2 || vxm==3)
					{
						corners=RM_CORNER9;
					}
					break;
				case 2:
					if(vxp==1 || vxp==3)
					{
						corners=RM_CORNER7;
					}
					break;
				case 3:
					if(vxp==1 || vxp==3)
					{
						if(vxm==2 || vxm==3)
						{
							corners=RM_CORNER97;
						}
						else
						{
							corners=RM_CORNER7;
						}
					}
					else
					{
						if(vxp==1 || vxp==3)
						{
							corners=RM_CORNER9;
						}
					}
					break;
				}
				break;
			case 270:
				switch(vxp)
				{
				case 1:
					if(vzp==2 || vzp==3)
					{
						corners=RM_CORNER9;
					}
					break;
				case 2:
					if(vzm==1 || vzm==3)
					{
						corners=RM_CORNER7;
					}
					break;
				case 3:
					if(vzp==2 || vzp==3)
					{
						if(vzm==1 || vzm==3)
						{
							corners=RM_CORNER97;
						}
						else
						{
							corners=RM_CORNER9;
						}
					}
					else
					{
						if(vzm==1 || vzm==3)
						{
							corners=RM_CORNER7;
						}
					}
					break;
				}
				break;
			}
			break;
		case 2:
			if(it->getWallType()==0)
			{
				switch(it->getOrient())
				{
				case 0:
					if(vzm==2 || vzm==3)
					{
						if(vxm==1 || vxm==3)
						{
							corners=RM_CORNER3;
						}
					}
					break;
				case 90:
					if(vxm==2 || vxm==3)
					{
						if(vzp==1 || vzp==3)
						{
							corners=RM_CORNER3;
						}
					}
					break;
				case 180:
					if(vzp==2 || vzp==3)
					{
						if(vxp==1 || vxp==3)
						{
							corners=RM_CORNER3;
						}
					}
					break;
				case 270:
					if(vxp==2 || vxp==3)
					{
						if(vzm==1 || vzm==3)
						{
							corners=RM_CORNER3;
						}
					}
					break;
				}
			}
			break;
		}

		it->setCorner(corners);
	}
}*/

Building* const Room::getBuilding(const Vector3<s32> &pos)
{
	std::vector<Building>::iterator it;
	for(it=blocks.begin(); it!=blocks.end();it++)
	{
		if(it->getPos()==pos)
		{
			return &(*it);
		}
	}

	return NULL;
}

void Room::rayTrace(const Ray<real32>& ray, RayCol<Building> &rayBuilding)
{
	real64 tymin,tymax,tzmin,tzmax,txmax,txmin,totmin(-1);

	for(std::vector<Building>::iterator it(blocks.begin());it!=blocks.end();it++)
	{
		Vector3<real64> posconv(pos.x+it->getPos().x,pos.y+it->getPos().y,pos.z+it->getPos().z);
		Vector3<real64> offset(0.5,0.5,0.5),bounds[2]={(posconv-offset),(posconv+offset)};
		s32 facetrack(0),fy(0),fz(0),fx(0);
		real64 ty(10000),tz(10000),tx(10000),least(10000);
		
		//Calculate the intersection points
		txmin=(bounds[ray.sign[0]].x - ray.orig.x) * ray.invdir.x;
		txmax=(bounds[1-ray.sign[0]].x-ray.orig.x) * ray.invdir.x;
		tymin=(bounds[ray.sign[1]].y - ray.orig.y) * ray.invdir.y;
		tymax=(bounds[1-ray.sign[1]].y-ray.orig.y) * ray.invdir.y;
		tzmin=(bounds[ray.sign[2]].z - ray.orig.z) * ray.invdir.z;
		tzmax=(bounds[1-ray.sign[2]].z-ray.orig.z) * ray.invdir.z;

		//Calculate the y intersection with reversed rays
		if(ray.sign[1]==1)
		{
			if(((ray.orig.x+(ray.dir.x*tymax))>=bounds[0].x && (ray.orig.x+(ray.dir.x*tymax))<=bounds[1].x) 
				&& ((ray.orig.z+(ray.dir.z*tymax))>=bounds[0].z && (ray.orig.z+(ray.dir.z*tymax))<=bounds[1].z)
				&& tymax > 0)
			{
				ty=tymax;
				fy=2;
			}
		}
		else
		{
			if(((ray.orig.x+(ray.dir.x*tymin))>=bounds[0].x && (ray.orig.x+(ray.dir.x*tymin))<=bounds[1].x) 
				&& ((ray.orig.z+(ray.dir.z*tymin))>=bounds[0].z && (ray.orig.z+(ray.dir.z*tymin))<=bounds[1].z)
				&& tymin > 0)
			{
				ty=tymin;
				fy=1;
			}
		}

		switch(it->getWalls())
		{
		case 4:
			//check 4 surrounding walls to get min x and min z intersect
			if(((ray.orig.x+(ray.dir.x*tzmin))>=bounds[0].x && (ray.orig.x+(ray.dir.x*tzmin))<=bounds[1].x) 
				&& ((ray.orig.y+(ray.dir.y*tzmin))>=bounds[0].y && (ray.orig.y+(ray.dir.y*tzmin))<=bounds[1].y)
				&& tzmin > 0)
			{
				if(ray.sign[2]==1)
					fz=5;
				else
					fz=6;
				tz=tzmin;
			}
			else if(((ray.orig.x+(ray.dir.x*tzmax))>=bounds[0].x && (ray.orig.x+(ray.dir.x*tzmax))<=bounds[1].x) 
				&& ((ray.orig.y+(ray.dir.y*tzmax))>=bounds[0].y && (ray.orig.y+(ray.dir.y*tzmax))<=bounds[1].y)
				&& tzmax > 0)
			{
				if(ray.sign[2]==1)
					fz=6;
				else
					fz=5;
				tz=tzmax;
			}

			if(((ray.orig.z+(ray.dir.z*txmin))>=bounds[0].z && (ray.orig.z+(ray.dir.z*txmin))<=bounds[1].z) 
				&& ((ray.orig.y+(ray.dir.y*txmin))>=bounds[0].y && (ray.orig.y+(ray.dir.y*txmin))<=bounds[1].y)
				&& txmin > 0)
			{
				if(ray.sign[0]==1)
					fx=3;
				else
					fx=4;
				tx=txmin;
			}
			else if(((ray.orig.z+(ray.dir.z*txmax))>=bounds[0].z && (ray.orig.z+(ray.dir.z*txmax))<=bounds[1].z) 
				&& ((ray.orig.y+(ray.dir.y*txmax))>=bounds[0].y && (ray.orig.y+(ray.dir.y*txmax))<=bounds[1].y)
				&& txmax > 0)
			{
				if(ray.sign[0]==1)
					fx=4;
				else
					fx=3;
				tx=txmax;
			}

			//Determine the least intersection point between all valid intersections
			if(ty<tz)
			{
				if(ty<tx)
				{
					//TY is least intersection point
					least=ty;
					facetrack=fy;
				}
				else
				{
					//TX is least intersection point
					least=tx;
					facetrack=fx;
				}
			}
			else
			{
				if(tz<tx)
				{
					//TZ is least intersection point
					least=tz;
					facetrack=fz;
				}
				else
				{
					//TX is least intersection point
					least=tx;
					facetrack=fx;
				}
			}
			break;
		case 3:
			switch(it->getOrient())
			{
			case 0:
				if(ray.sign[0]==1)
				{
					if(((ray.orig.z+(ray.dir.z*txmin))>=bounds[0].z && (ray.orig.z+(ray.dir.z*txmin))<=bounds[1].z) 
						&& ((ray.orig.y+(ray.dir.y*txmin))>=bounds[0].y && (ray.orig.y+(ray.dir.y*txmin))<=bounds[1].y)
						&& txmin>0){
						tx=txmin;
						fx=3;
					}
				}
				else
				{
					if(((ray.orig.z+(ray.dir.z*txmax))>=bounds[0].z && (ray.orig.z+(ray.dir.z*txmax))<=bounds[1].z) 
						&& ((ray.orig.y+(ray.dir.y*txmax))>=bounds[0].y && (ray.orig.y+(ray.dir.y*txmax))<=bounds[1].y)
						&& txmax>0){
						tx=txmax;
						fx=3;
					}
				}

				if(((ray.orig.x+(ray.dir.x*tzmin))>=bounds[0].x && (ray.orig.x+(ray.dir.x*tzmin))<=bounds[1].x) 
					&& ((ray.orig.y+(ray.dir.y*tzmin))>=bounds[0].y && (ray.orig.y+(ray.dir.y*tzmin))<=bounds[1].y)
					&& tzmin > 0)
				{
					if(ray.sign[2]==1)
						fz=5;
					else
						fz=6;
					tz=tzmin;
				}
				else if(((ray.orig.x+(ray.dir.x*tzmax))>=bounds[0].x && (ray.orig.x+(ray.dir.x*tzmax))<=bounds[1].x) 
					&& ((ray.orig.y+(ray.dir.y*tzmax))>=bounds[0].y && (ray.orig.y+(ray.dir.y*tzmax))<=bounds[1].y)
					&& tzmax > 0)
				{
					if(ray.sign[2]==1)
						fz=6;
					else
						fz=5;
					tz=tzmax;
				}
				break;
			case 90:
				//ZP empty
				if(ray.sign[2]==1)
				{
					if(((ray.orig.x+(ray.dir.x*tzmax))>=bounds[0].x && (ray.orig.x+(ray.dir.x*tzmax))<=bounds[1].x) 
						&& ((ray.orig.y+(ray.dir.y*tzmax))>=bounds[0].y && (ray.orig.y+(ray.dir.y*tzmax))<=bounds[1].y)
						&& tzmax > 0)
					{
						tz=tzmax;
						fz=6;
					}
				}
				else
				{
					if(((ray.orig.x+(ray.dir.x*tzmin))>=bounds[0].x && (ray.orig.x+(ray.dir.x*tzmin))<=bounds[1].x) 
						&& ((ray.orig.y+(ray.dir.y*tzmin))>=bounds[0].y && (ray.orig.y+(ray.dir.y*tzmin))<=bounds[1].y)
						&& tzmin > 0)
					{
						tz=tzmin;
						fz=6;
					}
				}

				if(((ray.orig.z+(ray.dir.z*txmin))>=bounds[0].z && (ray.orig.z+(ray.dir.z*txmin))<=bounds[1].z) 
					&& ((ray.orig.y+(ray.dir.y*txmin))>=bounds[0].y && (ray.orig.y+(ray.dir.y*txmin))<=bounds[1].y)
					&& txmin > 0)
				{
					if(ray.sign[0]==1)
						fx=3;
					else
						fx=4;
					tx=txmin;
				}
				else if(((ray.orig.z+(ray.dir.z*txmax))>=bounds[0].z && (ray.orig.z+(ray.dir.z*txmax))<=bounds[1].z) 
					&& ((ray.orig.y+(ray.dir.y*txmax))>=bounds[0].y && (ray.orig.y+(ray.dir.y*txmax))<=bounds[1].y)
					&& txmax > 0)
				{
					if(ray.sign[0]==1)
						fx=4;
					else
						fx=3;
					tx=txmax;
				}
				break;
			case 180:
				if(ray.sign[0]==1)
				{
					if(((ray.orig.z+(ray.dir.z*txmax))>=bounds[0].z && (ray.orig.z+(ray.dir.z*txmax))<=bounds[1].z) 
						&& ((ray.orig.y+(ray.dir.y*txmax))>=bounds[0].y && (ray.orig.y+(ray.dir.y*txmax))<=bounds[1].y)
						&& txmax>0){
						tx=txmax;
						fx=4;
					}
				}
				else
				{
					if(((ray.orig.z+(ray.dir.z*txmin))>=bounds[0].z && (ray.orig.z+(ray.dir.z*txmin))<=bounds[1].z) 
						&& ((ray.orig.y+(ray.dir.y*txmin))>=bounds[0].y && (ray.orig.y+(ray.dir.y*txmin))<=bounds[1].y)
						&& txmin>0){
						tx=txmin;
						fx=4;
					}
				}

				if(((ray.orig.x+(ray.dir.x*tzmin))>=bounds[0].x && (ray.orig.x+(ray.dir.x*tzmin))<=bounds[1].x) 
					&& ((ray.orig.y+(ray.dir.y*tzmin))>=bounds[0].y && (ray.orig.y+(ray.dir.y*tzmin))<=bounds[1].y)
					&& tzmin > 0)
				{
					if(ray.sign[2]==1)
						fz=5;
					else
						fz=6;
					tz=tzmin;
				}
				else if(((ray.orig.x+(ray.dir.x*tzmax))>=bounds[0].x && (ray.orig.x+(ray.dir.x*tzmax))<=bounds[1].x) 
					&& ((ray.orig.y+(ray.dir.y*tzmax))>=bounds[0].y && (ray.orig.y+(ray.dir.y*tzmax))<=bounds[1].y)
					&& tzmax > 0)
				{
					if(ray.sign[2]==1)
						fz=6;
					else
						fz=5;
					tz=tzmax;
				}
				break;
			case 270:
				if(ray.sign[2]==1)
				{
					if(((ray.orig.x+(ray.dir.x*tzmin))>=bounds[0].x && (ray.orig.x+(ray.dir.x*tzmin))<=bounds[1].x) 
						&& ((ray.orig.y+(ray.dir.y*tzmin))>=bounds[0].y && (ray.orig.y+(ray.dir.y*tzmin))<=bounds[1].y)
						&& tzmin > 0)
					{
						tz=tzmin;
						fz=5;
					}
				}
				else
				{
					if(((ray.orig.x+(ray.dir.x*tzmax))>=bounds[0].x && (ray.orig.x+(ray.dir.x*tzmax))<=bounds[1].x) 
						&& ((ray.orig.y+(ray.dir.y*tzmax))>=bounds[0].y && (ray.orig.y+(ray.dir.y*tzmax))<=bounds[1].y)
						&& tzmax > 0)
					{
						tz=tzmax;
						fz=5;
					}
				}

				if(((ray.orig.z+(ray.dir.z*txmin))>=bounds[0].z && (ray.orig.z+(ray.dir.z*txmin))<=bounds[1].z) 
					&& ((ray.orig.y+(ray.dir.y*txmin))>=bounds[0].y && (ray.orig.y+(ray.dir.y*txmin))<=bounds[1].y)
					&& txmin > 0)
				{
					if(ray.sign[0]==1)
						fx=3;
					else
						fx=4;
					tx=txmin;
				}
				else if(((ray.orig.z+(ray.dir.z*txmax))>=bounds[0].z && (ray.orig.z+(ray.dir.z*txmax))<=bounds[1].z) 
					&& ((ray.orig.y+(ray.dir.y*txmax))>=bounds[0].y && (ray.orig.y+(ray.dir.y*txmax))<=bounds[1].y)
					&& txmax > 0)
				{
					if(ray.sign[0]==1)
						fx=4;
					else
						fx=3;
					tx=txmax;
				}
				break;
			}
			break;
		case 2:
			switch(it->getWallType())
			{
			case 0:	//corner
				switch(it->getOrient())
				{
				case 0:
					//zp xp
					if(ray.sign[2]==1)
					{
						if(((ray.orig.x+(ray.dir.x*tzmin))>=bounds[0].x && (ray.orig.x+(ray.dir.x*tzmin))<=bounds[1].x) 
							&& ((ray.orig.y+(ray.dir.y*tzmin))>=bounds[0].y && (ray.orig.y+(ray.dir.y*tzmin))<=bounds[1].y)
							&& tzmin > 0)
						{
							tz=tzmin;
							fz=5;
						}
					}
					else
					{
						if(((ray.orig.x+(ray.dir.x*tzmax))>=bounds[0].x && (ray.orig.x+(ray.dir.x*tzmax))<=bounds[1].x) 
							&& ((ray.orig.y+(ray.dir.y*tzmax))>=bounds[0].y && (ray.orig.y+(ray.dir.y*tzmax))<=bounds[1].y)
							&& tzmax > 0)
						{
							tz=tzmax;
							fz=5;
						}
					}

					if(ray.sign[0]==1)
					{
						if(((ray.orig.z+(ray.dir.z*txmin))>=bounds[0].z && (ray.orig.z+(ray.dir.z*txmin))<=bounds[1].z) 
							&& ((ray.orig.y+(ray.dir.y*txmin))>=bounds[0].y && (ray.orig.y+(ray.dir.y*txmin))<=bounds[1].y)
							&& txmin>0){
							tx=txmin;
							fx=3;
						}
					}
					else
					{
						if(((ray.orig.z+(ray.dir.z*txmax))>=bounds[0].z && (ray.orig.z+(ray.dir.z*txmax))<=bounds[1].z) 
							&& ((ray.orig.y+(ray.dir.y*txmax))>=bounds[0].y && (ray.orig.y+(ray.dir.y*txmax))<=bounds[1].y)
							&& txmax>0){
							tx=txmax;
							fx=3;
						}
					}
					break;
				case 90:
					//xp zm
					if(ray.sign[0]==1)
					{
						if(((ray.orig.z+(ray.dir.z*txmin))>=bounds[0].z && (ray.orig.z+(ray.dir.z*txmin))<=bounds[1].z) 
							&& ((ray.orig.y+(ray.dir.y*txmin))>=bounds[0].y && (ray.orig.y+(ray.dir.y*txmin))<=bounds[1].y)
							&& txmin>0){
							tx=txmin;
							fx=3;
						}
					}
					else
					{
						if(((ray.orig.z+(ray.dir.z*txmax))>=bounds[0].z && (ray.orig.z+(ray.dir.z*txmax))<=bounds[1].z) 
							&& ((ray.orig.y+(ray.dir.y*txmax))>=bounds[0].y && (ray.orig.y+(ray.dir.y*txmax))<=bounds[1].y)
							&& txmax>0){
							tx=txmax;
							fx=3;
						}
					}

					if(ray.sign[2]==1)
					{
						if(((ray.orig.x+(ray.dir.x*tzmax))>=bounds[0].x && (ray.orig.x+(ray.dir.x*tzmax))<=bounds[1].x) 
							&& ((ray.orig.y+(ray.dir.y*tzmax))>=bounds[0].y && (ray.orig.y+(ray.dir.y*tzmax))<=bounds[1].y)
							&& tzmax > 0)
						{
							tz=tzmax;
							fz=6;
						}
					}
					else
					{
						if(((ray.orig.x+(ray.dir.x*tzmin))>=bounds[0].x && (ray.orig.x+(ray.dir.x*tzmin))<=bounds[1].x) 
							&& ((ray.orig.y+(ray.dir.y*tzmin))>=bounds[0].y && (ray.orig.y+(ray.dir.y*tzmin))<=bounds[1].y)
							&& tzmin > 0)
						{
							tz=tzmin;
							fz=6;
						}
					}
					break;
				case 180:
					//zm xm
					if(ray.sign[2]==1)
					{
						if(((ray.orig.x+(ray.dir.x*tzmin))>=bounds[0].x && (ray.orig.x+(ray.dir.x*tzmin))<=bounds[1].x) 
							&& ((ray.orig.y+(ray.dir.y*tzmin))>=bounds[0].y && (ray.orig.y+(ray.dir.y*tzmin))<=bounds[1].y)
							&& tzmin > 0)
						{
							tz=tzmin;
							fz=6;
						}
					}
					else
					{
						if(((ray.orig.x+(ray.dir.x*tzmax))>=bounds[0].x && (ray.orig.x+(ray.dir.x*tzmax))<=bounds[1].x) 
							&& ((ray.orig.y+(ray.dir.y*tzmax))>=bounds[0].y && (ray.orig.y+(ray.dir.y*tzmax))<=bounds[1].y)
							&& tzmax > 0)
						{
							tz=tzmax;
							fz=6;
						}
					}

					if(ray.sign[0]==1)
					{
						if(((ray.orig.z+(ray.dir.z*txmax))>=bounds[0].z && (ray.orig.z+(ray.dir.z*txmax))<=bounds[1].z) 
							&& ((ray.orig.y+(ray.dir.y*txmax))>=bounds[0].y && (ray.orig.y+(ray.dir.y*txmax))<=bounds[1].y)
							&& txmax>0){
							tx=txmax;
							fx=4;
						}
					}
					else
					{
						if(((ray.orig.z+(ray.dir.z*txmin))>=bounds[0].z && (ray.orig.z+(ray.dir.z*txmin))<=bounds[1].z) 
							&& ((ray.orig.y+(ray.dir.y*txmin))>=bounds[0].y && (ray.orig.y+(ray.dir.y*txmin))<=bounds[1].y)
							&& txmin>0){
							tx=txmin;
							fx=4;
						}
					}
					break;
				case 270:
					//xm zp
					if(ray.sign[0]==1)
					{
						if(((ray.orig.z+(ray.dir.z*txmax))>=bounds[0].z && (ray.orig.z+(ray.dir.z*txmax))<=bounds[1].z) 
							&& ((ray.orig.y+(ray.dir.y*txmax))>=bounds[0].y && (ray.orig.y+(ray.dir.y*txmax))<=bounds[1].y)
							&& txmax>0){
							tx=txmax;
							fx=4;
						}
					}
					else
					{
						if(((ray.orig.z+(ray.dir.z*txmin))>=bounds[0].z && (ray.orig.z+(ray.dir.z*txmin))<=bounds[1].z) 
							&& ((ray.orig.y+(ray.dir.y*txmin))>=bounds[0].y && (ray.orig.y+(ray.dir.y*txmin))<=bounds[1].y)
							&& txmin>0){
							tx=txmin;
							fx=4;
						}
					}

					if(ray.sign[2]==1)
					{
						if(((ray.orig.x+(ray.dir.x*tzmin))>=bounds[0].x && (ray.orig.x+(ray.dir.x*tzmin))<=bounds[1].x) 
							&& ((ray.orig.y+(ray.dir.y*tzmin))>=bounds[0].y && (ray.orig.y+(ray.dir.y*tzmin))<=bounds[1].y)
							&& tzmin > 0)
						{
							tz=tzmin;
							fz=5;
						}
					}
					else
					{
						if(((ray.orig.x+(ray.dir.x*tzmax))>=bounds[0].x && (ray.orig.x+(ray.dir.x*tzmax))<=bounds[1].x) 
							&& ((ray.orig.y+(ray.dir.y*tzmax))>=bounds[0].y && (ray.orig.y+(ray.dir.y*tzmax))<=bounds[1].y)
							&& tzmax > 0)
						{
							tz=tzmax;
							fz=5;
						}
					}
					break;
				}
				break;
			case 1: //hallway
				switch(it->getOrient())
				{
				case 0:
					if(((ray.orig.x+(ray.dir.x*tzmin))>=bounds[0].x && (ray.orig.x+(ray.dir.x*tzmin))<=bounds[1].x) 
						&& ((ray.orig.y+(ray.dir.y*tzmin))>=bounds[0].y && (ray.orig.y+(ray.dir.y*tzmin))<=bounds[1].y)
						&& tzmin > 0)
					{
						if(ray.sign[2]==1)
							fz=5;
						else
							fz=6;
						tz=tzmin;
					}
					else if(((ray.orig.x+(ray.dir.x*tzmax))>=bounds[0].x && (ray.orig.x+(ray.dir.x*tzmax))<=bounds[1].x) 
						&& ((ray.orig.y+(ray.dir.y*tzmax))>=bounds[0].y && (ray.orig.y+(ray.dir.y*tzmax))<=bounds[1].y)
						&& tzmax > 0)
					{
						if(ray.sign[2]==1)
							fz=6;
						else
							fz=5;
						tz=tzmax;
					}
					break;
				case 90:
					if(((ray.orig.z+(ray.dir.z*txmin))>=bounds[0].z && (ray.orig.z+(ray.dir.z*txmin))<=bounds[1].z) 
						&& ((ray.orig.y+(ray.dir.y*txmin))>=bounds[0].y && (ray.orig.y+(ray.dir.y*txmin))<=bounds[1].y)
						&& txmin > 0)
					{
						if(ray.sign[0]==1)
							fx=3;
						else
							fx=4;
						tx=txmin;
					}
					else if(((ray.orig.z+(ray.dir.z*txmax))>=bounds[0].z && (ray.orig.z+(ray.dir.z*txmax))<=bounds[1].z) 
						&& ((ray.orig.y+(ray.dir.y*txmax))>=bounds[0].y && (ray.orig.y+(ray.dir.y*txmax))<=bounds[1].y)
						&& txmax > 0)
					{
						if(ray.sign[0]==1)
							fx=4;
						else
							fx=3;
						tx=txmax;
					}
					break;
				case 180:
					if(((ray.orig.x+(ray.dir.x*tzmin))>=bounds[0].x && (ray.orig.x+(ray.dir.x*tzmin))<=bounds[1].x) 
						&& ((ray.orig.y+(ray.dir.y*tzmin))>=bounds[0].y && (ray.orig.y+(ray.dir.y*tzmin))<=bounds[1].y)
						&& tzmin > 0)
					{
						if(ray.sign[2]==1)
							fz=5;
						else
							fz=6;
						tz=tzmin;
					}
					else if(((ray.orig.x+(ray.dir.x*tzmax))>=bounds[0].x && (ray.orig.x+(ray.dir.x*tzmax))<=bounds[1].x) 
						&& ((ray.orig.y+(ray.dir.y*tzmax))>=bounds[0].y && (ray.orig.y+(ray.dir.y*tzmax))<=bounds[1].y)
						&& tzmax > 0)
					{
						if(ray.sign[2]==1)
							fz=6;
						else
							fz=5;
						tz=tzmax;
					}
					break;
				case 270:
					if(((ray.orig.z+(ray.dir.z*txmin))>=bounds[0].z && (ray.orig.z+(ray.dir.z*txmin))<=bounds[1].z) 
						&& ((ray.orig.y+(ray.dir.y*txmin))>=bounds[0].y && (ray.orig.y+(ray.dir.y*txmin))<=bounds[1].y)
						&& txmin > 0)
					{
						if(ray.sign[0]==1)
							fx=3;
						else
							fx=4;
						tx=txmin;
					}
					else if(((ray.orig.z+(ray.dir.z*txmax))>=bounds[0].z && (ray.orig.z+(ray.dir.z*txmax))<=bounds[1].z) 
						&& ((ray.orig.y+(ray.dir.y*txmax))>=bounds[0].y && (ray.orig.y+(ray.dir.y*txmax))<=bounds[1].y)
						&& txmax > 0)
					{
						if(ray.sign[0]==1)
							fx=4;
						else
							fx=3;
						tx=txmax;
					}
					break;
				}
				break;
			}
			break;
		case 1:
			switch(it->getOrient())
			{
			case 0:
				if(ray.sign[2]==1)
				{
					if(((ray.orig.x+(ray.dir.x*tzmin))>=bounds[0].x && (ray.orig.x+(ray.dir.x*tzmin))<=bounds[1].x) 
						&& ((ray.orig.y+(ray.dir.y*tzmin))>=bounds[0].y && (ray.orig.y+(ray.dir.y*tzmin))<=bounds[1].y)
						&& tzmin > 0)
					{
						tz=tzmin;
						fz=5;
					}
				}
				else
				{
					if(((ray.orig.x+(ray.dir.x*tzmax))>=bounds[0].x && (ray.orig.x+(ray.dir.x*tzmax))<=bounds[1].x) 
						&& ((ray.orig.y+(ray.dir.y*tzmax))>=bounds[0].y && (ray.orig.y+(ray.dir.y*tzmax))<=bounds[1].y)
						&& tzmax > 0)
					{
						tz=tzmax;
						fz=5;
					}
				}
				break;
			case 90:
				if(ray.sign[0]==1)
				{
					if(((ray.orig.z+(ray.dir.z*txmin))>=bounds[0].z && (ray.orig.z+(ray.dir.z*txmin))<=bounds[1].z) 
						&& ((ray.orig.y+(ray.dir.y*txmin))>=bounds[0].y && (ray.orig.y+(ray.dir.y*txmin))<=bounds[1].y)
						&& txmin>0){
						tx=txmin;
						fx=3;
					}
				}
				else
				{
					if(((ray.orig.z+(ray.dir.z*txmax))>=bounds[0].z && (ray.orig.z+(ray.dir.z*txmax))<=bounds[1].z) 
						&& ((ray.orig.y+(ray.dir.y*txmax))>=bounds[0].y && (ray.orig.y+(ray.dir.y*txmax))<=bounds[1].y)
						&& txmax>0){
						tx=txmax;
						fx=3;
					}
				}
				break;
			case 180:
				if(ray.sign[2]==1)
				{
					if(((ray.orig.x+(ray.dir.x*tzmax))>=bounds[0].x && (ray.orig.x+(ray.dir.x*tzmax))<=bounds[1].x) 
						&& ((ray.orig.y+(ray.dir.y*tzmax))>=bounds[0].y && (ray.orig.y+(ray.dir.y*tzmax))<=bounds[1].y)
						&& tzmax > 0)
					{
						tz=tzmax;
						fz=6;
					}
				}
				else
				{
					if(((ray.orig.x+(ray.dir.x*tzmin))>=bounds[0].x && (ray.orig.x+(ray.dir.x*tzmin))<=bounds[1].x) 
						&& ((ray.orig.y+(ray.dir.y*tzmin))>=bounds[0].y && (ray.orig.y+(ray.dir.y*tzmin))<=bounds[1].y)
						&& tzmin > 0)
					{
						tz=tzmin;
						fz=6;
					}
				}
				break;
			case 270:
				if(ray.sign[0]==1)
				{
					if(((ray.orig.z+(ray.dir.z*txmax))>=bounds[0].z && (ray.orig.z+(ray.dir.z*txmax))<=bounds[1].z) 
						&& ((ray.orig.y+(ray.dir.y*txmax))>=bounds[0].y && (ray.orig.y+(ray.dir.y*txmax))<=bounds[1].y)
						&& txmax>0){
						tx=txmax;
						fx=4;
					}
				}
				else
				{
					if(((ray.orig.z+(ray.dir.z*txmin))>=bounds[0].z && (ray.orig.z+(ray.dir.z*txmin))<=bounds[1].z) 
						&& ((ray.orig.y+(ray.dir.y*txmin))>=bounds[0].y && (ray.orig.y+(ray.dir.y*txmin))<=bounds[1].y)
						&& txmin>0){
						tx=txmin;
						fx=4;
					}
				}
				break;
			}
			break;
		case 0:
			break;
		}
		
		//Determine the least intersection point between all valid intersections
		if(ty<tz)
		{
			if(ty<tx)
			{
				//TY is least intersection point
				least=ty;
				facetrack=fy;
			}
			else
			{
				//TX is least intersection point
				least=tx;
				facetrack=fx;
			}
		}
		else
		{
			if(tz<tx)
			{
				//TZ is least intersection point
				least=tz;
				facetrack=fz;
			}
			else if(tx<10000)
			{
				//TX is least intersection point
				least=tx;
				facetrack=fx;
			}
		}

		if(least>0 && least<totmin){
			totmin=least;
		}
		else if(totmin==-1 && least>0){
			totmin=least;
		}

		if(totmin<rayBuilding.dist){
			rayBuilding.dist=totmin;
			rayBuilding.blockCol=&(*it);
			rayBuilding.face=facetrack;
			rayBuilding.sOffset=pos;
		}
	}
}

void Room::SaveAll(FILE *file)
{
	s32 blocksize(blocks.size());
	fwrite(&pos,sizeof(pos),1,file);
	fwrite(&blocksize,sizeof(s32),1,file);

	for(std::vector<Building>::iterator bit(blocks.begin());bit!=blocks.end();++bit)
	{
		Vector3<s32> bpos(bit->getPos());
		s32 walls(bit->getWalls());
		s32 orient(bit->getOrient());
		s32 roomType(bit->getRoomType());
		s32 wallType(bit->getWallType());
		s32 doorPosition(bit->getDoorPosition());
		s32 cornerPosition(bit->getCornerPosition());

		fwrite(&bpos,sizeof(bpos),1,file);
		fwrite(&walls,sizeof(s32),1,file);
		fwrite(&orient,sizeof(s32),1,file);
		fwrite(&roomType,sizeof(s32),1,file);
		fwrite(&wallType,sizeof(s32),1,file);
		fwrite(&doorPosition,sizeof(s32),1,file);
		fwrite(&cornerPosition,sizeof(s32),1,file);
	}
}