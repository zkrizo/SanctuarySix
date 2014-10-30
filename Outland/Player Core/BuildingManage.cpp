#include "BuildingManage.h"
#include <iostream>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <Windows.h>
#include <Outland\Singletons.h>
#include <direct.h>
#include "BuildingGraph.h"

/****************************
* Constructor
****************************/
BuildingManage::BuildingManage():ghostRoom(NULL)
{
	//variables for resource name, cost, tech names, and levels
	//these are to be changed as necessary between the construction calls.
	s32 resn[4]={0,0,0,0};
	s32 resc[4]={0,0,0,0};
	s32 tecn[4]={0,0,0,0};
	s32 tecl[4]={0,0,0,0};
	
	
	/*block.setInfo(Vector3<s32>(-2,6,-2),3,270,RM_HALLWAY,0);
	temp.addBlock(block);
	block.setInfo(Vector3<s32>(-3,6,-5),2,0,RM_HALLWAY,1);
	temp.addBlock(block);
	block.setInfo(Vector3<s32>(-5,6,-2),2,90,RM_HALLWAY,0);
	temp.addBlock(block);
	block.setInfo(Vector3<s32>(-7,6,-4),1,270,RM_HALLWAY,0);
	temp.addBlock(block);
	block.setInfo(Vector3<s32>(-9,6,-4),0,0,RM_HALLWAY,0);
	temp.addBlock(block);*/


	//Building Info Setup routines
	T1Airlock=new BuildInfo("The airlock provides access to the outside of the settlement. This is necessary for exploration and resource gathering. However, the normal airlock is only large enough to let a small party of people go through, not resources. To get resources and vehicles into the settlement, a freight airlock is needed.",
		1,1,resn,resc,1,tecn,tecl,3);
					
	T1Bathrooms=new BuildInfo("Bathrooms are essential for crew comfort and cleanliness. Nobody wants to be stuck in a building with stinky people. Like Living Quarters, bathrooms can be built as large multi-user rooms but will take a penalty after 4 showers/baths.",
		2,2,resn,resc,1,tecn,tecl,3);
	
	T1CommandCenter=new BuildInfo("The Command Center is the central hub of the outpost and the space station. The Command Center is responsible for the flow of resources throughout the buildings. It is also where you will find your avatar most of the time. The original Command Center is transformed from the landing pod and becomes the center of your base.",
		10,10,resn,resc,1,tecn,tecl,3);
	
	T1FreightAirlock=new BuildInfo("The freight airlock is perfect for transporting materials into and out of settlements. This includes building resources, vehicles, and large items.",
		2,4,resn,resc,1,tecn,tecl,3);
	
	T1Galley=new BuildInfo("The Galley is the most basic eating establishment. It is basically a cafeteria. Food is cooked and served by the computer through machine cookers. It’s not the best tasting or the heartiest of foods but it suffices to fill your crew members after a long day of work.",
		8,8,resn,resc,1,tecn,tecl,3);
	
	T1Hallway=new BuildInfo("Hallways are the default construction option for players. They provide nice climate controlled access between rooms that is protected from the outside environment. These most basic versions of hallways look VERY unfinished with cables lying on the floor, steel plating for walls, and no windows or decorations to speak of. These are the ultimate definition of basic and only fulfill the most basic requirements of a hallway.",
		1,1,resn,resc,1,tecn,tecl,3);
	
	T1IndustrialWorkshop=new BuildInfo("The Industrial Workshop is the basic construction facility for everything from construction materials like girders to furniture used to furnish the interior spaces.",
		12,12,resn,resc,1,tecn,tecl,3);
	
	T1Laboratory=new BuildInfo("The Laboratory is used to research new technologies and advance current technologies. It is fundamental to building a full scale base and can bring many advances in knowledge that will benefit your crew.",
		5,5,resn,resc,1,tecn,tecl,3);
	
	T1Library=new BuildInfo("The library is where your crew can go to sit down and relax after a hard day’s work. The library offers traditional paper books for reading and a small amount of electronic media is available for reading as well. The library in this state cannot access the mainframe to retrieve the vast amounts of media stored within.",
		4,4,resn,resc,1,tecn,tecl,3);
	
	T1LivingQuarters=new BuildInfo("The Living Quarters is the home of the crew and residents. Living Quarters can be built as large dormitory like rooms where everyone sleeps in one room or you can build single rooms for each resident/family. Communal Living Quarters will suffer a penalty to stats if there are more than 6 beds in a room. While communal living is accepted, people like to have their own space.",
		4,4,resn,resc,1,tecn,tecl,3);
	
	T1MiningControlCenter=new BuildInfo("The Mining Control Center is the first major hub for mineral gathering. The Mining Control Center (MCC) is a large industrial workshop with a pilot-able mining machine. Using this mining machine, crew will be able to venture out onto the planet’s surface and mine surface materials for research and construction. The MCC is responsible for obtaining all surface materials but the basic mining machine will only be able to collect the most basic minerals at first. As you research new minerals, you will be able to upgrade your current mining machine to be able to collect tougher and more exotic surface minerals.",
		8,5,resn,resc,1,tecn,tecl,3);
	
	T1OxygenScrubber=new BuildInfo("The Oxygen Scrubber is an essential piece of equipment when living in a non-earth atmosphere. It provides almost limitless oxygen by recycling the oxygen within the station/outpost. It strips out the carbon molecules from carbon dioxide to provide oxygen.",
		2,2,resn,resc,1,tecn,tecl,3);
	
	T1PowerFusion=new BuildInfo("The Fusion Reactor is the first power plant available to you. It uses the power of fusion to provide power to the rest of the facility. It does have a very small chance to overload and cause damage to the nearby rooms. The fusion reactor provides 125 power and can be turned on and off quickly.",
		4,4,resn,resc,1,tecn,tecl,3);
	
	T1Warehouse=new BuildInfo("The Warehouse is where all of your materials are stored. This simple building is essential to keeping your material safe and secure within your new home. The basic warehouse can’t store very much per square of space, but it keeps your materials in a known location and keeps it safe from expose to the harsh environment.",
		5,3,resn,resc,1,tecn,tecl,3);
	
	T1WaterCollector=new BuildInfo("The Water Collector allows natural water from the planet’s atmosphere to be collected and added to the outpost’s water supply. This is only a viable option when the rain water is potable. If it contains hazardous chemicals and would like to increase the colonist’s water supply, this must be linked to the HAZARDOUS WATER TREATMENT PLANT.",
		3,2,resn,resc,1,tecn,tecl,3);
	
	T1WaterRecycling=new BuildInfo("The Water Recycler is responsible for maintaining the water supply. It takes all of the water that is used in the facility and purifies it to turn it back into usable water. Each recycler only purifies the water. It doesn’t add water into the system. A separate facility (Water Treatment and Water Collector) is needed to add new water into the water supply.",
		2,2,resn,resc,1,tecn,tecl,3);
}

/****************************
* Destructor
****************************/
BuildingManage::~BuildingManage()
{
	while(!Rooms.empty())
	{
		Rooms.pop_back();
	}

	clearInterRooms();
	
	delete T1Airlock;
	delete T1Bathrooms;
	delete T1CommandCenter;
	delete T1FreightAirlock;
	delete T1Galley;
	delete T1Hallway;
	delete T1IndustrialWorkshop;
	delete T1Laboratory;
	delete T1Library;
	delete T1LivingQuarters;
	delete T1MiningControlCenter;
	delete T1OxygenScrubber;
	delete T1PowerFusion;
	delete T1Warehouse;
	delete T1WaterCollector;
	delete T1WaterRecycling;
	if(ghostRoom)
		delete ghostRoom;
}

/****************************
* checkMergeRooms - Checks the rooms that could be within the position sent 
					to ensure the building doesn't overwrite already built rooms
					Returns true if there is a building already at that position
****************************/
tBOOL BuildingManage::checkMergeRooms(Vector3<s64> pos1, s32 type)
{
	for(std::vector<Room*>::iterator it(mergeRooms.begin());it!=mergeRooms.end();it++)
	{
		if((*it)->checkPos(pos1))
		{
			return TRUE;
		}
	}

	return FALSE;
}

/****************************
* checkNearbyRooms - Checks the rooms that could be within the position sent 
					to ensure the building doesn't overwrite already built rooms
****************************/
s32 BuildingManage::checkNearbyRooms(Vector3<s64> pos1,s32 type)
{
	tBOOL add(TRUE);

	for(std::vector<Room*>::iterator it(interRooms.begin());it!=interRooms.end();it++)
	{
		//Iterates through the existing rooms and ensures there is no room within that space
		if((*it)->checkPos(pos1))
		{
			//Room found within the space so the type is checked to see if the build is ignored or is to be merged to an existing room
			if((*it)->getType()==type)
			{
				for(std::vector<Room*>::iterator rit(mergeRooms.begin());rit!=mergeRooms.end();rit++)
				{
					//Checks to see if the current room is already inside the merge rooms list
					if((*rit)->getPos()==(*it)->getPos())
					{
						add=FALSE;
					}
				}

				//Adds the room to the merge list if it isn't already in it.
				if(add)
				{
					mergeRooms.push_back(*it);
				}
				return 2;
			}else
			{
				//If the existing room's type is not the same, the build is ignored
				return 0;
			}
		}
	}

	//No building present in the space, so pass the code that it is safe to build
	return 1;
}

s32 BuildingManage::checkRooms(const Vector3<s64>& pos1,const s32& type)
{
	for(std::vector<Room>::iterator it(Rooms.begin());it!=Rooms.end();it++)
	{
		//Iterates through the existing rooms and ensures there is no room within that space
		if(it->checkPos(pos1))
		{
			//Room found within the space so the type is checked to see if the build is ignored or is to be merged to an existing room
			if(it->getType()==type)
			{
				return 2;
			}else
			{
				//If the existing room's type is not the same, the build is ignored
				return 0;
			}
		}
	}

	//No building present in the space, so pass the code that it is safe to build
	return 1;
}

/****************************
* checkNearbyRooms - Checks the rooms bordering the two positions
					to see if they are the same. Called initially when
					building a room to ensure that it can't be merged with
					existing rooms.
****************************/
tBOOL BuildingManage::checkNearbyRooms(Vector3<s64> pos1, Vector3<s64> pos2, s32 type)
{
	std::vector<Room>::iterator it(Rooms.begin());
	Vector3<s64> rpos;

	pos1.x-=1;
	pos1.z-=1;
	pos2.x+=1;
	pos2.z+=1;
	s32 miny,maxy,minx,maxx,minz,maxz;

	clearInterRooms();

	while(it!=Rooms.end())
	{
		rpos=it->getPos();
		miny=it->getMinY();
		minx=it->getMinX();
		minz=it->getMinZ();
		maxy=it->getMaxY();
		maxx=it->getMaxX();
		maxz=it->getMaxZ();

		if((rpos.y+miny<=pos1.y && rpos.y+maxy>=pos1.y)||(rpos.y+miny<=pos2.y && rpos.y+maxy>=pos2.y))
		{
			if((rpos.x+minx<=pos1.x && rpos.x+maxx>=pos1.x)||(rpos.x+minx<=pos2.x && rpos.x+maxx>=pos2.x)||(rpos.x+minx>=pos1.x && rpos.x+maxx<=pos2.x))
			{
				if((rpos.z+minz<=pos1.z && rpos.z+maxz>=pos1.z)||(rpos.z+minz<=pos2.z && rpos.z+maxz>=pos2.z)||(rpos.z+minz>=pos1.z && rpos.z+maxz<=pos2.z))
				{
					interRooms.push_back(&(*it));
				}
			}
		}
		it++;
	}
	return !interRooms.empty();
}

/****************************
* BuildRect - This function handles the building
				of a rectangular room
****************************
void BuildingManage::BuildRect(Vector3<s64> pos3, Vector3<s64> pos4, s32 type, tBOOL merge, bool ghosted)
{
	Vector3<s64> pos1,pos2;

	if(pos3.x<=pos4.x)
	{
		pos1.x=pos3.x;
		pos2.x=pos4.x;
	}
	else
	{
		pos1.x=pos4.x;
		pos2.x=pos3.x;
	}

	if(pos3.y<=pos4.y)
	{
		pos1.y=pos3.y;
		pos2.y=pos4.y;
	}
	else
	{
		pos1.y=pos4.y;
		pos2.y=pos3.y;
	}
	
	if(pos3.z<=pos4.z)
	{
		pos1.z=pos3.z;
		pos2.z=pos4.z;
	}
	else
	{
		pos1.z=pos4.z;
		pos2.z=pos3.z;
	}
	
	if(!ghosted)
	{
		logger->LogMessage(4,"\nNew building being built:\n");
		logger->LogMessage(4,"Building type: " + uiCore->intToString(type) + "\n");
		logger->LogMessage(4,"Position 1 - X: " + uiCore->intToString(pos1.x) + " Y: " + uiCore->intToString(pos1.y) + " Z: " + uiCore->intToString(pos1.z) + "\n");
		logger->LogMessage(4,"Position 2 - X: " + uiCore->intToString(pos2.x) + " Y: " + uiCore->intToString(pos2.y) + " Z: " + uiCore->intToString(pos2.z) + "\n");
	}

	if(!checkNearbyRooms(pos1,pos2,type) || (checkNearbyRooms(pos1,pos2,type) && !merge) || (uiCore->build==2 && ghosted))	//new room
	{
		Room newRoom;
		newRoom.setPos(pos1);
		Building corner;
		Building wall;
		Building center;
		corner.setInfo(Vector3<s32>(pos1.x,pos1.y,pos1.z),2,-90,type,0);
		center.setInfo(Vector3<s32>(pos1.x+1,pos1.y,pos1.z+1),0,0,type,0);
		wall.setInfo(Vector3<s32>(pos1.x+1,pos1.y,pos1.z),1,0,type,0);

		if(!ghosted)
			logger->LogMessage(4,"No buildings of matching type nearby. New room being created!\n");

		//Checks to ensure player has resources necessary to build
		if(gameCore->resourceMan.checkRoomBuildResources(type,((pos2.x-pos1.x+1)*(pos2.y-pos1.y+1)*(pos2.z-pos1.z+1)),"Default"))
		{
			for(int i=pos1.x;i<=pos2.x;i++)
			{
				for(int j=pos1.z;j<=pos2.z;j++)
				{
					if(i==pos1.x)//checks for the left wall
					{
						if(j==pos1.z)//checks for front corner
						{
							corner.setOrient(180); //default 180
							corner.setPos(Vector3<s32>(0,0,0));
							if(pos2.x-pos1.x==0)//checks for corridor
							{
								if(pos2.z-pos1.z==0)//checks for single room block
								{
									corner.setOrient(0);
									corner.setWalls(4);
								}
								else
								{
									corner.setOrient(90);
									corner.setWalls(3);
								}
							}
							else if(pos2.z-pos1.z==0)
							{
								corner.setWalls(3);
							}
							newRoom.addBlock(corner);
						}
						else if(j==pos2.z)//checks for back corner
						{
							corner.setOrient(270);
							corner.setPos(Vector3<s32>(0,0,j-pos1.z));
							if(pos2.x-pos1.x==0)
							{
								corner.setWalls(3);
							}
							newRoom.addBlock(corner);
						}
						else //left wall tiles
						{
							wall.setOrient(270);
							wall.setPos(Vector3<s32>(0,0,j-pos1.z));
						
							if(pos2.x-pos1.x==0)
							{
								wall.setWalls(2);
								wall.setWallType(1);
							}
							newRoom.addBlock(wall);
						}
					}
					else if(i==pos2.x)//checks for right wall
					{
						if(j==pos1.z)//checks for front corner
						{
							corner.setOrient(90);
							corner.setPos(Vector3<s32>(i-pos1.x,0,0));
							if(pos2.z-pos1.z==0)
							{
								corner.setOrient(0);
								corner.setWalls(3);
							}
							newRoom.addBlock(corner);
						}
						else if(j==pos2.z)//checks for back corner
						{
							corner.setOrient(0);
							corner.setPos(Vector3<s32>(i-pos1.x,0,j-pos1.z));
							newRoom.addBlock(corner);
						}
						else //right wall tiles
						{
							wall.setOrient(90);
							wall.setPos(Vector3<s32>(i-pos1.x,0,j-pos1.z));
							if(pos2.z-pos1.z==0)
							{
								wall.setWalls(2);
								wall.setWallType(1);
							}
							newRoom.addBlock(wall);
						}
					}
					else //center and wall tiles
					{
						if(j==pos1.z)//checks for front wall
						{
							wall.setOrient(180);
							wall.setPos(Vector3<s32>(i-pos1.x,0,0));
							if(pos2.z-pos1.z==0)
							{
								wall.setWalls(2);
								wall.setWallType(1);
							}
							newRoom.addBlock(wall);
						}
						else if(j==pos2.z)//checks for back wall
						{
							wall.setOrient(0);
							wall.setPos(Vector3<s32>(i-pos1.x,0,j-pos1.z));
							newRoom.addBlock(wall);
						}
						else //center tiles
						{
							center.setPos(Vector3<s32>(i-pos1.x,0,j-pos1.z));
							newRoom.addBlock(center);
						}
					}
				}
			}
			//Caculates the power requirements for the room
			newRoom.recalcPower();

			if(ghosted)
			{
				if(ghostRoom)
				{
					delete ghostRoom;
					ghostRoom=new Room(newRoom);
				}
				else
					ghostRoom=new Room(newRoom);
			}
			else
			{
				//Adds the new room to the main building list
				Rooms.push_back(newRoom);
			}
		}
	}
	else		//Positions requested merge with an existing building.
	{
		//Creates new room just in case the surrounding rooms are different types
		Room newRoom;
		newRoom.setPos(pos1);
		Building corner;
		Building wall;
		Building center;
		Building tri;
		Building quad;
		quad.setInfo(Vector3<s32>(pos1.x,pos1.y,pos1.z),4,0,type,0);
		tri.setInfo(Vector3<s32>(pos1.x,pos1.y,pos1.z),3,-90,type,0);
		corner.setInfo(Vector3<s32>(pos1.x,pos1.y,pos1.z),2,-90,type,0);
		center.setInfo(Vector3<s32>(pos1.x+1,pos1.y,pos1.z+1),0,0,type,0);
		wall.setInfo(Vector3<s32>(pos1.x+1,pos1.y,pos1.z),1,0,type,0);
		std::vector<Room*>::iterator rit(interRooms.begin());
		s32 ***matrix=new s32**[pos2.x-pos1.x+3];
		s32 walls(0),spec(0),size(0);
		int i(0),j(0),k(0);
		bool needsMerged(false);

		if(!ghosted)
		{
			logger->LogMessage(4,"Matching Building type found nearby. Creating build matrix:\n");

			std::cout << "New Build Matrix Started:\n";
			std::cout << "Starting Position: X:" << pos1.x << " Y:" << pos1.y << " Z:" << pos1.z << "\n";
			std::cout << "Ending   Position: X:" << pos2.x << " Y:" << pos2.y << " Z:" << pos2.z << "\n";
		}

		for(i=0;i<=(pos2.x-pos1.x+2);i++)
		{
			matrix[i]=new s32*[pos2.y-pos1.y+1];
			for(j=0;j<=(pos2.y-pos1.y);j++)
			{
				matrix[i][j]=new s32[pos2.z-pos1.z+3];
			}
		}

		//Sets the build matrix
		for(i=0;i<=(pos2.x-pos1.x+2);i++)
		{
			for(j=0;j<=(pos2.y-pos1.y);j++)
			{
				for(k=0;k<=(pos2.z-pos1.z+2);k++)
				{
					matrix[i][j][k]=checkNearbyRooms(Vector3<s64>(pos1.x-1+i,pos1.y+j,pos1.z-1+k),type);
					if(!ghosted)
					{
						std::cout << matrix[i][j][k] << " ";
						logger->LogMessage(4,uiCore->intToString(matrix[i][j][k]) + " ");
					}
				}
				if(!ghosted)
				{
					std::cout << '\n';
					logger->LogMessage(4,"\n");
				}
			}
		}
		
		//Calculates the size of the room to calculate the resources needed to build
		for(i=1;i<=(pos2.x-pos1.x+1);i++)
		{
			for(j=0;j<=(pos2.y-pos1.y);j++)
			{
				for(k=1;k<=(pos2.z-pos1.z+1);k++)
				{
					if(matrix[i][j][k]==1)
						size++;
				}
			}
		}

		//Checks against the player's resources
		if(gameCore->resourceMan.checkRoomBuildResources(type,size,"Default"))
		{
			//Fills out the building matrix by adding the various rooms
			for(i=1;i<=(pos2.x-pos1.x+1);i++)
			{
				for(j=0;j<=(pos2.y-pos1.y);j++)
				{
					for(k=1;k<=(pos2.z-pos1.z+1);k++)
					{
						walls=0;
						spec=0;
						if(matrix[i][j][k]==1)
						{
							if(i==1)	//checks if x is min
							{
								if(i==(pos2.x-pos1.x+1))	//Checks for unique condition of x being both min and max (IE, corridor)
								{
									if(k==1) //k Min
									{
										if(k==(pos2.z-pos1.z+1)) //k also max
										{
											//x min, x max, k min, k max
											if(matrix[i-1][j][k]!=2)
											{
												//x- wall
												if(matrix[i+1][j][k]!=2)
												{
													//x+- wall
													if(matrix[i][j][k-1]!=2)
													{
														//x+- and k- wall
														if(matrix[i][j][k+1]!=2)
														{
															//quad
															quad.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(quad);
														}
														else
														{
															//x+- and k- wall, k+ no wall
															tri.setOrient(90);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
													}
													else
													{
														//x+- wall, k- no wall
														if(matrix[i][j][k+1]!=2)
														{
															//x+- and k+ wall, k- no wall
															tri.setOrient(270);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															//x+- wall, k+- no wall, corridor
															spec=1;
															corner.setOrient(90);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
												}
												else
												{
													//x- wall, x+ no wall
													if(matrix[i][j][k-1]!=2)
													{
														//x- and k- wall, x+ no wall
														if(matrix[i][j][k+1]!=2)
														{
															tri.setOrient(180);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															//x- and k- wall,x+ and k+ no wall
															spec=0;
															corner.setOrient(180);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
													else
													{
														//x- wall, x+ and k- no wall
														if(matrix[i][j][k+1]!=2)
														{
															corner.setOrient(270);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(270);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
												}
											}
											else
											{
												//x- no wall
												if(matrix[i+1][j][k]!=2)
												{
													//x+ wall, x- no wall
													if(matrix[i][j][k-1]!=2)
													{
														//x+ and k- wall, x- no wall
														if(matrix[i][j][k+1]!=2)
														{
															//x+ and k+- wall, x- no wall
															tri.setOrient(0);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															//x+ and k- wall, x- and k+ no wall
															spec=0;
															corner.setOrient(90);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
													else
													{
														//x+ wall, x- and k- no wall
														if(matrix[i][j][k+1]!=2)
														{
															//x+ and k+ wall, x- and k- no wall
															spec=0;
															corner.setOrient(0);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															//x+ wall, x- and k+- no wall
															wall.setOrient(90);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
												}
												else
												{
													//x+- no wall
													if(matrix[i][j][k-1]!=2)
													{
														//k- wall, x+- no wall
														if(matrix[i][j][k+1]!=2)
														{
															//k+- wall, x+- no wall
															spec=1;
															corner.setOrient(0);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															//k- wall, x+- and k+ no wall
															wall.setOrient(180);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
													else
													{
														//x+- and k- no wall
														if(matrix[i][j][k+1]!=2)
														{
															//k+ wall, x+- and k- no wall
															wall.setOrient(0);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
														else
														{
															//no walls
															center.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(center);
														}
													}
												}
											}
										}
										else if(k<(pos2.z-pos1.z+1))
										{
											//k min, k not max
											if(matrix[i-1][j][k]!=2)
											{
												//wall at x-
												if(matrix[i+1][j][k]!=2)
												{
													//wall at x+-
													if(matrix[i][j][k-1]!=2)
													{
														//wall at i+- and k-
														if(matrix[i][j][k+1]==0)
														{
															//quad
															quad.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(quad);
														}
														else
														{
															//tri
															tri.setOrient(90);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
													}
													else //wall at x+-, k min but no wall
													{
														if(matrix[i][j][k+1]==0)
														{
															tri.setOrient(270);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=1;
															corner.setOrient(90);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
												}
												else //wall at x-, no wall at x+, k min, walls 1
												{
													if(matrix[i][j][k-1]!=2)
													{
														//x- and k- wall, x+ no wall, k min
														if(matrix[i][j][k+1]==0)
														{
															//x- and k+- wall, x+ no wall
															tri.setOrient(180);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															//x- and k- wall, x+ and k+ no wall
															spec=0;
															corner.setOrient(180);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
													else
													{
														//wall at x-, x+ and k- no wall, k min
														if(matrix[i][j][k+1]==0)
														{
															//x- and k+ wall, x+ and k- no wall
															spec=0;
															corner.setOrient(270);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(270);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
												}
											}
											else //no wall at x-, k min, walls 0
											{
												if(matrix[i+1][j][k]!=2)
												{
													//x+ wall, x- no wall, k min
													if(matrix[i][j][k-1]!=2)
													{
														//x+ and k- wall, x- no wall
														if(matrix[i][j][k+1]==0)
														{
															//x+ and k+- wall, x- no wall
															tri.setOrient(0);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															//x+ and k- wall, x- and k+ no wall
															spec=0;
															corner.setOrient(90);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
													else
													{
														//x+ wall, x- and k- no wall
														if(matrix[i][j][k+1]==0)
														{
															spec=0;
															corner.setOrient(0);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
														}
														else
														{
															wall.setOrient(90);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
												}
												else
												{
													//x+- no wall k min
													if(matrix[i][j][k-1]!=2)
													{
														//k- wall, x+- no wall
														if(matrix[i][j][k+1]==0)
														{
															spec=1;
															corner.setOrient(0);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(180);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
													else
													{
														//x+- and k- no wall
														if(matrix[i][j][k+1]==0)
														{
															wall.setOrient(0);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
														else
														{
															center.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(center);
														}
													}
												}
											}
										}
									}
									else if(k>1)
									{
										if(k==(pos2.z-pos1.z+1)) //k max but not min, x min and max
										{
											if(matrix[i-1][j][k]!=2)
											{
												//x- wall
												if(matrix[i+1][j][k]!=2)
												{
													//x+- wall
													if(matrix[i][j][k-1]==0)
													{
														//x+- and k- wall
														if(matrix[i][j][k+1]!=2)
														{
															quad.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(quad);
														}
														else
														{
															tri.setOrient(90);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
													}
													else
													{
														//x+- wall, k- no wall
														if(matrix[i][j][k+1]!=2)
														{
															tri.setOrient(270);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=1;
															corner.setOrient(90);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
												}
												else
												{
													//x- wall, x+ no wall
													if(matrix[i][j][k-1]==0)
													{
														//x- and k- wall, x+ no wall
														if(matrix[i][j][k+1]!=2)
														{
															tri.setOrient(180);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=0;
															corner.setOrient(180);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
													else
													{
														//x- wall, x+ and k- no wall
														if(matrix[i][j][k+1]!=2)
														{
															spec=0;
															corner.setOrient(270);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(270);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
												}
											}
											else
											{
												//x- no wall
												if(matrix[i+1][j][k]!=2)
												{
													//x+ wall, x- no wall
													if(matrix[i][j][k-1]==0)
													{
														//x+ and k- wall, x- no wall
														if(matrix[i][j][k+1]!=2)
														{
															tri.setOrient(0);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=0;
															corner.setOrient(90);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
													else
													{
														//x+ wall, x- and k- no wall
														if(matrix[i][j][k+1]!=2)
														{
															spec=0;
															corner.setOrient(0);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(90);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
												}
												else
												{
													//x+- no wall
													if(matrix[i][j][k-1]==0)
													{
														//k- wall, x+- no wall
														if(matrix[i][j][k+1]!=2)
														{
															spec=1;
															corner.setOrient(0);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(180);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
													else
													{
														//x+- and k- no wall
														if(matrix[i][j][k+1]!=2)
														{
															wall.setOrient(0);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
														else
														{
															center.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(center);
														}
													}
												}
											}
										}
										else if(k<(pos2.z-pos1.z+1))//k neither min nor max
										{
											if(matrix[i-1][j][k]!=2)
											{
												//x- wall
												if(matrix[i+1][j][k]!=2)
												{
													//x+- wall
													if(matrix[i][j][k-1]==0)
													{
														//x+- and k- wall
														if(matrix[i][j][k+1]==0)
														{
													
															quad.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(quad);
														}
														else
														{
															tri.setOrient(0);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
													}
													else
													{
														//x+- wall, k- no wall
														if(matrix[i][j][k+1]==0)
														{
															tri.setOrient(270);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=1;
															corner.setOrient(90);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
												}
												else
												{
													//x- wall, x+ no wall
													if(matrix[i][j][k-1]==0)
													{
														//k- and x- wall, x+ no wall
														if(matrix[i][j][k+1]==0)
														{
															tri.setOrient(180);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=0;
															corner.setOrient(180);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
													else
													{
														//x- wall,x+ and k- no wall
														if(matrix[i][j][k+1]==0)
														{
															spec=0;
															corner.setOrient(270);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(270);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
												}
											}
											else
											{
												//x- no wall
												if(matrix[i+1][j][k]!=2)
												{
													//x+ wall, x- no wall
													if(matrix[i][j][k-1]==0)
													{
														//x+ and k- wall, x- no wall
														if(matrix[i][j][k+1]==0)
														{
															tri.setOrient(0);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=0;
															corner.setOrient(90);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
													else
													{
														//x+ wall, x- and k- no wall
														if(matrix[i][j][k+1]==0)
														{
															spec=0;
															corner.setOrient(0);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(90);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
												}
												else
												{
													//x+- no wall
													if(matrix[i][j][k-1]==0)
													{
														//k- wall, x+- no wall
														if(matrix[i][j][k+1]==0)
														{
															spec=1;
															corner.setOrient(0);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(180);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
													else
													{
														//x+- and k- no wall
														if(matrix[i][j][k+1]==0)
														{
															wall.setOrient(0);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
														else
														{
															center.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(center);
														}
													}
												}
											}
										}
									}
								}
								else if(i<(pos2.x-pos1.x+1))//x min but not max
								{
									if(k==1)
									{
										//x min, k min
										if(k==(pos2.z-pos1.z+1))
										{
											//x min, k min and max (hallway along x axis)
											if(matrix[i-1][j][k]!=2)
											{
												//x- wall
												if(matrix[i+1][j][k]==0)
												{
													//x+- wall
													if(matrix[i][j][k-1]!=2)
													{
														//x+- and k- wall
														if(matrix[i][j][k+1]!=2)
														{
															quad.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(quad);
														}
														else
														{
															tri.setOrient(90);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
													}
													else
													{
														//x+- wall, k- no wall
														if(matrix[i][j][k+1]!=2)
														{
															tri.setOrient(270);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=1;
															corner.setOrient(90);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
												}
												else
												{
													//x- wall, x+ no wall
													if(matrix[i][j][k-1]!=2)
													{
														//x- and k- wall, x+ no wall
														if(matrix[i][j][k+1]!=2)
														{
															tri.setOrient(180);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=0;
															corner.setOrient(180);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
													else
													{
														//x- wall, x+ and k- no wall
														if(matrix[i][j][k+1]!=2)
														{
															spec=0;
															corner.setOrient(270);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(270);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
												}
											}
											else
											{
												//x- no wall
												if(matrix[i+1][j][k]==0)
												{
													//x+ wall, x- no wall
													if(matrix[i][j][k-1]!=2)
													{
														//x+ and k- wall, x- no wall
														if(matrix[i][j][k+1]!=2)
														{
															tri.setOrient(0);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=0;
															corner.setOrient(90);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
													else
													{
														//x+ wall, x- and k- no wall
														if(matrix[i][j][k+1]!=2)
														{
															spec=0;
															corner.setOrient(90);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(90);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
												}
												else
												{
													//x+- no wall
													if(matrix[i][j][k-1]!=2)
													{
														//k- wall, x+- no wall
														if(matrix[i][j][k+1]!=2)
														{
															spec=1;
															corner.setOrient(0);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(180);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
													else
													{
														//x+- and k- no wall
														if(matrix[i][j][k+1]!=2)
														{
															wall.setOrient(0);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
														else
														{
															center.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(center);
														}
													}
												}
											}
										}
										else if(k<(pos2.z-pos1.z+1))
										{
											//x min, k min
											if(matrix[i-1][j][k]!=2)
											{
												//x- wall
												if(matrix[i+1][j][k]==0)
												{
													//x+- wall
													if(matrix[i][j][k-1]!=2)
													{
														//x+- and k- wall
														if(matrix[i][j][k+1]==0)
														{
															quad.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(quad);
														}
														else
														{
															tri.setOrient(90);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
													}
													else
													{
														//x+- wall, k- no wall
														if(matrix[i][j][k+1]==0)
														{
															tri.setOrient(270);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=1;
															corner.setOrient(90);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
												}
												else
												{
													//x- wall, x+ no wall
													if(matrix[i][j][k-1]!=2)
													{
														//x- and k- wall, x+ no wall
														if(matrix[i][j][k+1]==0)
														{
															tri.setOrient(180);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=0;
															corner.setOrient(180);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
													else
													{
														//x- wall, x+ and k- no wall
														if(matrix[i][j][k+1]==0)
														{
															spec=0;
															corner.setOrient(270);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(270);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
												}
											}
											else
											{
												//x- no wall
												if(matrix[i+1][j][k]==0)
												{
													//x+ wall, x- no wall
													if(matrix[i][j][k-1]!=2)
													{
														//x+ and k- wall, x- no wall
														if(matrix[i][j][k+1]==0)
														{
															tri.setOrient(0);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=0;
															corner.setOrient(90);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
													else
													{
														//x+ wall, x- and k- no wall
														if(matrix[i][j][k+1]==0)
														{
															spec=0;
															corner.setOrient(90);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(90);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
												}
												else
												{
													//x+- no wall
													if(matrix[i][j][k-1]!=2)
													{
														//k- wall, x+- no wall
														if(matrix[i][j][k+1]==0)
														{
															spec=1;
															corner.setOrient(0);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(180);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
													else
													{
														//x+- and k- no wall
														if(matrix[i][j][k+1]==0)
														{
															wall.setOrient(0);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
														else
														{
															center.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(center);
														}
													}
												}
											}
										}
									}
									else if(k>1)
									{
										//k is not min
										if(k==(pos2.z-pos1.z+1))
										{
											//k is max not min
											if(matrix[i-1][j][k]!=2)
											{
												//x- wall
												if(matrix[i+1][j][k]==0)
												{
													//x+- wall
													if(matrix[i][j][k-1]==0)
													{
														//x+- and k- wall
														if(matrix[i][j][k+1]!=2)
														{
															quad.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(quad);
														}
														else
														{
															tri.setOrient(90);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
													}
													else
													{
														//x+- wall, k- no wall
														if(matrix[i][j][k+1]!=2)
														{
															tri.setOrient(270);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=1;
															corner.setOrient(90);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
												}
												else
												{
													//x- wall, x+ no wall
													if(matrix[i][j][k-1]==0)
													{
														//x- and k- wall, x+ no wall
														if(matrix[i][j][k+1]!=2)
														{
															tri.setOrient(180);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=0;
															corner.setOrient(180);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
													else
													{
														//x- wall, x+ and k- no wall
														if(matrix[i][j][k+1]!=2)
														{
															spec=0;
															corner.setOrient(270);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(270);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
												}
											}
											else
											{
												//x- no wall
												if(matrix[i+1][j][k]==0)
												{
													//x+ wall, x- no wall
													if(matrix[i][j][k-1]==0)
													{
														//x+ and k- wall, x- no wall
														if(matrix[i][j][k+1]!=2)
														{
															tri.setOrient(0);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=0;
															corner.setOrient(180);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
													else
													{
														//x+ wall, x- and k- no wall
														if(matrix[i][j][k+1]!=2)
														{
															spec=0;
															corner.setOrient(0);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(90);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
												}
												else
												{
													//x+- no wall
													if(matrix[i][j][k-1]==0)
													{
														//k- wall, x+- no wall
														if(matrix[i][j][k+1]!=2)
														{
															spec=1;
															corner.setOrient(0);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(180);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
													else
													{
														//x+- and k- no wall
														if(matrix[i][j][k+1]!=2)
														{
															wall.setOrient(0);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
														else
														{
															center.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(center);
														}
													}
												}
											}
										}
										else if(k<(pos2.z-pos1.z+1))
										{
											//k is not min or max, x min
											if(matrix[i-1][j][k]!=2)
											{
												//x- wall
												if(matrix[i+1][j][k]==0)
												{
													//x+- wall
													if(matrix[i][j][k-1]==0)
													{
														//x+- and k- wall
														if(matrix[i][j][k+1]==0)
														{
															quad.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(quad);
														}
														else
														{
															tri.setOrient(90);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
													}
													else
													{
														//x+- wall, k- no wall
														if(matrix[i][j][k+1]==0)
														{
															tri.setOrient(270);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=1;
															corner.setOrient(90);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
												}
												else
												{
													//x- wall, x+ no wall
													if(matrix[i][j][k-1]==0)
													{
														//x- and k- wall, x+ no wall
														if(matrix[i][j][k+1]==0)
														{
															tri.setOrient(180);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=0;
															corner.setOrient(180);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
													else
													{
														//x- wall, x+ and k- no wall
														if(matrix[i][j][k+1]==0)
														{
															spec=0;
															corner.setOrient(270);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(270);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
												}
											}
											else
											{
												//x- no wall
												if(matrix[i+1][j][k]==0)
												{
													//x+ wall, x- no wall
													if(matrix[i][j][k-1]==0)
													{
														//x+ and k- wall, x- no wall
														if(matrix[i][j][k+1]==0)
														{
															tri.setOrient(0);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=0;
															corner.setOrient(90);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
													else
													{
														//x+ wall, x- and k- no wall
														if(matrix[i][j][k+1]==0)
														{
															spec=0;
															corner.setOrient(0);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(90);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
												}
												else
												{
													//x+- no wall
													if(matrix[i][j][k-1]==0)
													{
														//k- wall, x+- no wall
														if(matrix[i][j][k+1]==0)
														{
															spec=1;
															corner.setOrient(0);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(180);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
													else
													{
														//x+- and k- no wall
														if(matrix[i][j][k+1]==0)
														{
															wall.setOrient(0);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
														else
														{
															center.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(center);
														}
													}
												}

											}
										}
									}
								}
							}
							else if(i>1)
							{
								//X is not min
								if(i==(pos2.x-pos1.x+1))
								{
									//X is max
									if(k==1) //x max k is min
									{
										if(k==(pos2.z-pos1.z+1))
										{
											//k is min and max (hallway in x direction)
											if(matrix[i-1][j][k]==0)
											{
												//x- wall
												if(matrix[i+1][j][k]!=2)
												{
													//x+- wall
													if(matrix[i][j][k-1]!=2)
													{
														//x+- and k- wall
														if(matrix[i][j][k+1]!=2)
														{
															quad.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(quad);
														}
														else
														{
															tri.setOrient(270);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
													}
													else
													{
														//x+- wall, k- no wall
														if(matrix[i][j][k+1]!=2)
														{
															tri.setOrient(270);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=1;
															corner.setOrient(90);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
												}
												else
												{
													//x- wall, x+ no wall
													if(matrix[i][j][k-1]!=2)
													{
														//x- and k- wall, x+ no wall
														if(matrix[i][j][k+1]!=2)
														{
															tri.setOrient(180);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=0;
															corner.setOrient(180);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
													else
													{
														//x- wall, x+ and k- no wall
														if(matrix[i][j][k+1]!=2)
														{
															spec=0;
															corner.setOrient(270);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(270);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
												}
											}
											else
											{
												//x- no wall
												if(matrix[i+1][j][k]!=2)
												{
													//x+ wall, x- no wall
													if(matrix[i][j][k-1]!=2)
													{
														//x+ and k- wall, x- no wall
														if(matrix[i][j][k+1]!=2)
														{
															tri.setOrient(0);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=0;
															corner.setOrient(90);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
													else
													{
														//x+ wall, x- and k- no wall
														if(matrix[i][j][k+1]!=2)
														{
															spec=0;
															corner.setOrient(0);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(90);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
												}
												else
												{
													//x+- no wall
													if(matrix[i][j][k-1]!=2)
													{
														//k- wall, x+- no wall
														if(matrix[i][j][k+1]!=2)
														{
															spec=1;
															corner.setOrient(0);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(180);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
													else
													{
														//x+- and k- no wall
														if(matrix[i][j][k+1]!=2)
														{
															wall.setOrient(180);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
														else
														{
															center.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(center);
														}
													}
												}
											}
										}
										else if(k<(pos2.z-pos1.z+1))//k min not max
										{
											if(matrix[i-1][j][k]==0)
											{
												//x- wall
												if(matrix[i+1][j][k]!=2)
												{
													//x+- wall
													if(matrix[i][j][k-1]!=2)
													{
														//x+- and k- wall
														if(matrix[i][j][k+1]==0)
														{
															quad.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(quad);
														}
														else
														{
															tri.setOrient(90);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
													}
													else
													{
														//x+- wall, k- no wall
														if(matrix[i][j][k+1]==0)
														{
															tri.setOrient(270);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=1;
															corner.setOrient(90);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
												}
												else
												{
													//x- wall, x+ no wall
													if(matrix[i][j][k-1]!=2)
													{
														//x- and k- wall, x+ no wall
														if(matrix[i][j][k+1]==0)
														{
															tri.setOrient(180);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=0;
															corner.setOrient(180);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
													else
													{
														//x- wall, x+ and k- no wall
														if(matrix[i][j][k+1]==0)
														{
															spec=0;
															corner.setOrient(270);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(270);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
												}
											}
											else
											{
												//x- no wall
												if(matrix[i+1][j][k]!=2)
												{
													//x+ wall, x- no wall
													if(matrix[i][j][k-1]!=2)
													{
														//x+ and k- wall, x- no wall
														if(matrix[i][j][k+1]==0)
														{
															tri.setOrient(0);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=0;
															corner.setOrient(90);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
													else
													{
														//x+ wall, x- and k- no wall
														if(matrix[i][j][k+1]==0)
														{
															spec=0;
															corner.setOrient(0);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(90);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
												}
												else
												{
													//x+- no wall
													if(matrix[i][j][k-1]!=2)
													{
														//k- wall, x+- no wall
														if(matrix[i][j][k+1]==0)
														{
															spec=1;
															corner.setOrient(0);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(180);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
													else
													{
														//x+- and k- no wall
														if(matrix[i][j][k+1]==0)
														{
															wall.setOrient(180);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
														else
														{
															center.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(center);
														}
													}
												}
											}
										}
									}
									else if(k>1)
									{
										//x max, k not min
										if(k==(pos2.z-pos1.z+1))
										{
											//x max, k max
											if(matrix[i-1][j][k]==0)
											{
												//x- wall
												if(matrix[i+1][j][k]!=2)
												{
													//x+- wall
													if(matrix[i][j][k-1]==0)
													{
														//x+- and k- wall
														if(matrix[i][j][k+1]!=2)
														{
															quad.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(quad);
														}
														else
														{
															tri.setOrient(90);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
													}
													else
													{
														//x+- wall, k- no wall
														if(matrix[i][j][k+1]!=2)
														{
															tri.setOrient(270);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=1;
															corner.setOrient(90);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
												}
												else
												{
													//x- wall, x+ no wall
													if(matrix[i][j][k-1]==0)
													{
														//x- and k- wall, x+ no wall
														if(matrix[i][j][k+1]!=2)
														{
															tri.setOrient(180);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=0;
															corner.setOrient(180);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
													else
													{
														//x- wall,x+ and k- no wall
														if(matrix[i][j][k+1]!=2)
														{
															spec=0;
															corner.setOrient(270);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(270);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
												}
											}
											else
											{
												//x- no wall
												if(matrix[i+1][j][k]!=2)
												{
													//x+ wall, x- no wall
													if(matrix[i][j][k-1]==0)
													{
														//x+ and k- wall, x- no wall
														if(matrix[i][j][k+1]!=2)
														{
															tri.setOrient(0);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=0;
															corner.setOrient(90);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
													else
													{
														//x+ wall, x- and k- no wall
														if(matrix[i][j][k+1]!=2)
														{
															spec=0;
															corner.setOrient(0);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(90);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
												}
												else
												{
													//x+- no wall
													if(matrix[i][j][k-1]==0)
													{
														//k- wall, x+- no wall
														if(matrix[i][j][k+1]!=2)
														{
															spec=1;
															corner.setOrient(0);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(180);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
													else
													{
														//x+- and k- no wall
														if(matrix[i][j][k+1]!=2)
														{
															wall.setOrient(0);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
														else
														{
															center.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(center);
														}
													}
												}
											}
										}
										else if(k<(pos2.z-pos1.z+1))
										{
											//x max, k not min or max
											if(matrix[i-1][j][k]==0)
											{
												//x- wall
												if(matrix[i+1][j][k]!=2)
												{
													//x+- wall
													if(matrix[i][j][k-1]==0)
													{
														//x+- and k- wall
														if(matrix[i][j][k+1]==0)
														{
															quad.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(quad);
														}
														else
														{
															tri.setOrient(90);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
													}
													else
													{
														//x+- wall, k- no wall
														if(matrix[i][j][k+1]==0)
														{
															tri.setOrient(270);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=1;
															corner.setOrient(90);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
												}
												else
												{
													//x- wall, x+ no wall
													if(matrix[i][j][k-1]==0)
													{
														//x- and k- wall, x+ no wall
														if(matrix[i][j][k+1]==0)
														{
															tri.setOrient(180);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=0;
															corner.setOrient(180);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
													else
													{
														//x- wall, x+ and k- no wall
														if(matrix[i][j][k+1]==0)
														{
															spec=0;
															corner.setOrient(270);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(270);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
												}
											}
											else
											{
												//x- no wall
												if(matrix[i+1][j][k]!=2)
												{
													//x+ wall, x- no wall
													if(matrix[i][j][k-1]==0)
													{
														//x+ and k- wall, x- no wall
														if(matrix[i][j][k+1]==0)
														{
															tri.setOrient(0);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=0;
															corner.setOrient(90);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
													else
													{
														//x+ wall, x- and k- no wall
														if(matrix[i][j][k+1]==0)
														{
															spec=0;
															corner.setOrient(0);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(90);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
												}
												else
												{
													//x+- no wall
													if(matrix[i][j][k-1]==0)
													{
														//k- wall, x+- no wall
														if(matrix[i][j][k+1]==0)
														{
															spec=1;
															corner.setOrient(0);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(180);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
													else
													{
														//x+- and k- no wall
														if(matrix[i][j][k+1]==0)
														{
															wall.setOrient(0);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
														else
														{
															center.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(center);
														}
													}
												}
											}
										}
									}
								}
								else if(i<(pos2.x-pos1.x+1))
								{
									//X is not max or min
									if(k==1)
									{
										//x between max/min, k min
										if(k==(pos2.z-pos1.z+1))
										{
											//x between min/max, k min AND max
											if(matrix[i-1][j][k]==0)
											{
												//x- wall
												if(matrix[i+1][j][k]==0)
												{
													//x+- wall
													if(matrix[i][j][k-1]!=2)
													{
														//x+- and k- wall
														if(matrix[i][j][k+1]!=2)
														{
															quad.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(quad);
														}
														else
														{
															tri.setOrient(90);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
													}
													else
													{
														//x+- wall, k- no wall
														if(matrix[i][j][k+1]!=2)
														{
															tri.setOrient(270);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=1;
															corner.setOrient(90);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
												}
												else
												{
													//x- wall, x+ no wall
													if(matrix[i][j][k-1]!=2)
													{
														//x- and k- wall, x+ no wall
														if(matrix[i][j][k+1]!=2)
														{
															tri.setOrient(180);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=0;
															corner.setOrient(180);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
													else
													{
														//x- wall, x+ and k- no wall
														if(matrix[i][j][k+1]!=2)
														{
															spec=0;
															corner.setOrient(270);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(270);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
												}
											}
											else
											{
												//x- no wall
												if(matrix[i+1][j][k]==0)
												{
													//x+ wall, x- no wall
													if(matrix[i][j][k-1]!=2)
													{
														//x+ and k- wall, x- no wall
														if(matrix[i][j][k+1]!=2)
														{
															tri.setOrient(0);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=0;
															corner.setOrient(90);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
													else
													{
														//x+ wall, x- and k- no wall
														if(matrix[i][j][k+1]!=2)
														{
															spec=0;
															corner.setOrient(0);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(90);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
												}
												else
												{
													//x+- no wall
													if(matrix[i][j][k-1]!=2)
													{
														//k- wall, x+- no wall
														if(matrix[i][j][k+1]!=2)
														{
															spec=1;
															corner.setOrient(0);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(180);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
													else
													{
														//x+- and k- no wall
														if(matrix[i][j][k+1]!=2)
														{
															wall.setOrient(0);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
														else
														{
															center.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(center);
														}
													}
												}
											}
										}
										else if(k<(pos2.z-pos1.z+1))
										{
											//x between min/max, k min not max
											if(matrix[i-1][j][k]==0)
											{
												//x- wall
												if(matrix[i+1][j][k]==0)
												{
													//x+- wall
													if(matrix[i][j][k-1]!=2)
													{
														//x+- and k- wall
														if(matrix[i][j][k+1]==0)
														{
															quad.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(quad);
														}
														else
														{
															tri.setOrient(90);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
													}
													else
													{
														//x+- wall, k- no wall
														if(matrix[i][j][k+1]==0)
														{
															tri.setOrient(270);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=1;
															corner.setOrient(90);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
												}
												else
												{
													//x- wall, x+ no wall
													if(matrix[i][j][k-1]!=2)
													{
														//x- and k- wall, x+ no wall
														if(matrix[i][j][k+1]==0)
														{
															tri.setOrient(180);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=0;
															corner.setOrient(180);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
													else
													{
														//x- wall, x+ and k- no wall
														if(matrix[i][j][k+1]==0)
														{
															spec=0;
															corner.setOrient(270);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(270);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
												}
											}
											else
											{
												//x- no wall
												if(matrix[i+1][j][k]==0)
												{
													//x+ wall, x- no wall
													if(matrix[i][j][k-1]!=2)
													{
														//x+ and k- wall, x- no wall
														if(matrix[i][j][k+1]==0)
														{
															tri.setOrient(0);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=0;
															corner.setOrient(90);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
													else
													{
														//x+ wall, x- and k- no wall
														if(matrix[i][j][k+1]==0)
														{
															spec=0;
															corner.setOrient(0);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(90);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
												}
												else
												{
													//x+- no wall
													if(matrix[i][j][k-1]!=2)
													{
														//k- wall, x+- no wall
														if(matrix[i][j][k+1]==0)
														{
															spec=1;
															corner.setOrient(0);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(180);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
													else
													{
														//x+- and k- no wall
														if(matrix[i][j][k+1]==0)
														{
															wall.setOrient(0);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
														else
														{
															center.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(center);
														}
													}
												}
											}
										}
									}
									else if(k>1)
									{
										//x between max/min, k not min
										if(k==(pos2.z-pos1.z+1))
										{
											//x between min/max, k max
											if(matrix[i-1][j][k]==0)
											{
												//x- wall
												if(matrix[i+1][j][k]==0)
												{
													//x+- wall
													if(matrix[i][j][k-1]==0)
													{
														//x+- and k- wall
														if(matrix[i][j][k+1]!=2)
														{
															quad.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(quad);
														}
														else
														{
															tri.setOrient(90);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
													}
													else
													{
														//x+- wall, k- no wall
														if(matrix[i][j][k+1]!=2)
														{
															tri.setOrient(270);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=1;
															corner.setOrient(90);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
												}
												else
												{
													//x- wall, x+ no wall
													if(matrix[i][j][k-1]==0)
													{
														//x- and k- wall, x+ no wall
														if(matrix[i][j][k+1]!=2)
														{
															tri.setOrient(180);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=0;
															corner.setOrient(180);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
													else
													{
														//x- wall, x+ and k- no wall
														if(matrix[i][j][k+1]!=2)
														{
															spec=0;
															corner.setOrient(270);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(270);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
												}
											}
											else
											{
												//x- no wall
												if(matrix[i+1][j][k]==0)
												{
													//x+ wall, x- no wall
													if(matrix[i][j][k-1]==0)
													{
														//x+ and k- wall, x- no wall
														if(matrix[i][j][k+1]!=2)
														{
															tri.setOrient(0);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=0;
															corner.setOrient(90);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
													else
													{
														//x+ wall, x- and k- no wall
														if(matrix[i][j][k+1]!=2)
														{
															spec=0;
															corner.setOrient(0);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(270);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
												}
												else
												{
													//x+- no wall
													if(matrix[i][j][k-1]==0)
													{
														//k- wall, x+- no wall
														if(matrix[i][j][k+1]!=2)
														{
															spec=1;
															corner.setOrient(0);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(180);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
													else
													{
														//x+- and k- no wall
														if(matrix[i][j][k+1]!=2)
														{
															wall.setOrient(0);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
														else
														{
															center.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(center);
														}
													}
												}
											}
										}
										else
										{
											//x between min/max, k between min/max
											if(matrix[i-1][j][k]==0)
											{
												//x- wall
												if(matrix[i+1][j][k]==0)
												{
													//x+- wall
													if(matrix[i][j][k-1]==0)
													{
														//x+- and k- wall
														if(matrix[i][j][k+1]==0)
														{
															quad.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(quad);
														}
														else
														{
															tri.setOrient(90);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
													}
													else
													{
														//x+- wall, k- no wall
														if(matrix[i][j][k+1]==0)
														{
															tri.setOrient(270);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=1;
															corner.setOrient(90);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
												}
												else
												{
													//x- wall, x+ no wall
													if(matrix[i][j][k-1]==0)
													{
														//x- and k- wall, x+ no wall
														if(matrix[i][j][k+1]==0)
														{
															tri.setOrient(180);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=0;
															corner.setOrient(180);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
													else
													{
														//x- wall, x+ and k- no wall
														if(matrix[i][j][k+1]==0)
														{
															spec=0;
															corner.setOrient(270);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(270);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
												}
											}
											else
											{
												//x- no wall
												if(matrix[i+1][j][k]==0)
												{
													//x+ wall, x- no wall
													if(matrix[i][j][k-1]==0)
													{
														//x+ and k- wall, x- no wall
														if(matrix[i][j][k+1]==0)
														{
															tri.setOrient(0);
															tri.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(tri);
														}
														else
														{
															spec=0;
															corner.setOrient(180);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
													}
													else
													{
														//x+ wall, x- and k- no wall
														if(matrix[i][j][k+1]==0)
														{
															spec=0;
															corner.setOrient(0);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(90);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
												}
												else
												{
													//x+- no wall
													if(matrix[i][j][k-1]==0)
													{
														//k- wall, x+- no wall
														if(matrix[i][j][k+1]==0)
														{
															spec=1;
															corner.setOrient(0);
															corner.setWallType(spec);
															corner.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(corner);
														}
														else
														{
															wall.setOrient(180);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
													}
													else
													{
														//x+- and k- no wall
														if(matrix[i][j][k+1]==0)
														{
															wall.setOrient(0);
															wall.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(wall);
														}
														else
														{
															center.setPos(Vector3<s32>(i-1,j,k-1));
															newRoom.addBlock(center);
														}
													}
												}
											}
										}
									}
								}
							}
						}

						else if(!ghosted)
						{
							//if the matrix determines a room needs to be merged, call the function to alter
							//the room's established building - ONLY CHECKS INSIDE BUILDING SPACE!
							if(matrix[i][j][k]==2)
							{	
								if(needsMerged==false)
									needsMerged=true;
								if(i==1)
								{
									if(i==(pos2.x-pos1.x+1))
									{
										if(k==1)
										{
											if(k==(pos2.z-pos1.z+1))
											{
												alterMergeBlock(0,0,0,0,Vector3<s64>(pos1.x+i-1,pos1.y+j,pos1.z+k-1),TRUE);
											}
											else
											{
												alterMergeBlock(0,0,matrix[i][j][k+1],0,Vector3<s64>(pos1.x+i-1,pos1.y+j,pos1.z+k-1),TRUE);
											}
										}
										else if(k==(pos2.z-pos1.z+1))
										{
											alterMergeBlock(0,0,0,matrix[i][j][k-1],Vector3<s64>(pos1.x+i-1,pos1.y+j,pos1.z+k-1),TRUE);
										}
										else
										{
											alterMergeBlock(0,0,matrix[i][j][k+1],matrix[i][j][k-1],Vector3<s64>(pos1.x+i-1,pos1.y+j,pos1.z+k-1),TRUE);
										}
									}
									else
									{
										if(k==1)
										{
											if(k==(pos2.z-pos1.z+1))
											{
												alterMergeBlock(matrix[i+1][j][k],0,0,0,Vector3<s64>(pos1.x+i-1,pos1.y+j,pos1.z+k-1),TRUE);
											}
											else
											{
												alterMergeBlock(matrix[i+1][j][k],0,matrix[i][j][k+1],0,Vector3<s64>(pos1.x+i-1,pos1.y+j,pos1.z+k-1),TRUE);
											}
										}
										else if(k==(pos2.z-pos1.z+1))
										{
											alterMergeBlock(matrix[i+1][j][k],0,0,matrix[i][j][k-1],Vector3<s64>(pos1.x+i-1,pos1.y+j,pos1.z+k-1),TRUE);
										}
										else
										{
											alterMergeBlock(matrix[i+1][j][k],0,matrix[i][j][k+1],matrix[i][j][k-1],Vector3<s64>(pos1.x+i-1,pos1.y+j,pos1.z+k-1),TRUE);
										}
									}
								}
								else if(i==(pos2.x-pos1.x+1))
								{
									if(k==1)
									{
										alterMergeBlock(0,matrix[i-1][j][k],matrix[i][j][k+1],0,Vector3<s64>(pos1.x+i-1,pos1.y+j,pos1.z+k-1),TRUE);
									}
									else if(k==(pos2.z-pos1.z+1))
									{
										alterMergeBlock(0,matrix[i-1][j][k],0,matrix[i][j][k-1],Vector3<s64>(pos1.x+i-1,pos1.y+j,pos1.z+k-1),TRUE);
									}
									else
									{
										alterMergeBlock(0,matrix[i-1][j][k],matrix[i][j][k+1],matrix[i][j][k-1],Vector3<s64>(pos1.x+i-1,pos1.y+j,pos1.z+k-1),TRUE);
									}
								}
								else
								{
									//Sends the block's position and the build matrix of the north, south, east, and west blocks to determine how it needs to change
									alterMergeBlock(matrix[i+1][j][k],matrix[i-1][j][k],matrix[i][j][k+1],matrix[i][j][k-1],Vector3<s64>(pos1.x+i-1,pos1.y+j,pos1.z+k-1),TRUE);
								}
							}
						}
					}
				}
			}
		
			
			//checks if this is a ghost room
			if(ghosted)
			{
				//If ghosted, writes the room into the ghost room 
				if(ghostRoom)
				{
					delete ghostRoom;
					ghostRoom=new Room(newRoom);
				}
				else
					ghostRoom=new Room(newRoom);
			}
			else
			{
				//Loop through the outside of the matrix and determine merge for bordering rooms
				for(i=0;i<=(pos2.x-pos1.x+2);i++)
				{
					for(j=0;j<=(pos2.y-pos1.y);j++)
					{
						for(k=0;k<=(pos2.z-pos1.z+2);k++)
						{
							int xp,xm,zp,zm;
							if(i==0)
							{
								if(k>0 && k<(pos2.z-pos1.z+2))
								{
									if(needsMerged==false && matrix[i][j][k]==2)
										needsMerged=true;

									xm=0;
									zp=matrix[i][j][k+1]==1? 0:(matrix[i][j][k+1]==2? 1:0);
									zm=matrix[i][j][k-1]==1? 0:(matrix[i][j][k-1]==2? 1:0);
									xp=matrix[i+1][j][k]==1? 1:(matrix[i+1][j][k]==2? 1:0);
									alterMergeBlock(xp,xm,zp,zm,Vector3<s64>(pos1.x+i-1,pos1.y,pos1.z+k-1),TRUE);
								}
							}
							else if(i==(pos2.x-pos1.x+2))
							{
								if(k>0 && k<(pos2.z-pos1.z+2))
								{
									if(needsMerged==false && matrix[i][j][k]==2)
										needsMerged=true;

									xp=0;
									zp=matrix[i][j][k+1]==1? 0:(matrix[i][j][k+1]==2? 1:0);
									zm=matrix[i][j][k-1]==1? 0:(matrix[i][j][k-1]==2? 1:0);
									xm=matrix[i-1][j][k]==1? 1:0;
									alterMergeBlock(xp,xm,zp,zm,Vector3<s64>(pos1.x+i-1,pos1.y,pos1.z+k-1),TRUE);
								}
							}
							else
							{
								//X is between min and max
								if(k==0)
								{
									if(needsMerged==false && matrix[i][j][k]==2)
										needsMerged=true;

									zm=0;
									xp=matrix[i+1][j][k]==2? 1:0;
									xm=matrix[i-1][j][k]==2? 1:0;
									zp=matrix[i][j][k+1]==1? 1:(matrix[i][j][k+1]==2? 1:0);
									alterMergeBlock(xp,xm,zp,zm,Vector3<s64>(pos1.x+i-1,pos1.y,pos1.z+k-1),TRUE);
								}
								else if(k==(pos2.z-pos1.z+2))
								{
									if(needsMerged==false && matrix[i][j][k]==2)
										needsMerged=true;

									zp=0;
									zm=matrix[i][j][k-1]==1? 1:(matrix[i][j][k-1]==2? 1:0);
									xp=matrix[i+1][j][k]==2? 1:0;
									xm=matrix[i-1][j][k]==2? 1:0;
									alterMergeBlock(xp,xm,zp,zm,Vector3<s64>(pos1.x+i-1,pos1.y,pos1.z+k-1),TRUE);
								}
							}
						}
					}
				}
				//calculates the power requirements for the room
				newRoom.recalcPower();
				newRoom.updateCorners();

				if(needsMerged)
				{
					logger->LogMessage(4,"New room needs to be merged with an existing room. Commencing merge...\n");
					if(mergeRooms.size()>1)
					{
						std::vector<Building> newBlocks(newRoom.getBlocks());
						std::vector<Building>::iterator bit;
						std::vector<Room*>::iterator rit2;
						Vector3<s64> offset(pos1-mergeRooms[0]->getPos());
						Vector3<s32> newPos;

						//Adds the new blocks to the only room in the merge list
						for(bit=newBlocks.begin();bit!=newBlocks.end();bit++)
						{
							newPos=Vector3<s32>(offset.x,offset.y,offset.z)+bit->getPos();
							bit->setPos(newPos);
							mergeRooms[0]->addBlock(*bit);
						}

						//Adds the additional merge rooms to the first room
						for(rit2=(mergeRooms.begin()+1);rit2!=mergeRooms.end();rit2++)
						{
							newBlocks=(*rit2)->getBlocks();
							offset=(*rit2)->getPos()-mergeRooms[0]->getPos();
							for(bit=newBlocks.begin();bit!=newBlocks.end();bit++)
							{
								newPos=Vector3<s32>(offset.x,offset.y,offset.z)+bit->getPos();
								bit->setPos(newPos);
								mergeRooms[0]->addBlock(*bit);
							}
				
							mergeRooms[0]->recalcMaxMin();
							mergeRooms[0]->recalcPower();
							mergeRooms[0]->updateCorners();

							//delete the merge room from the main room list to prevent duplicates
							for(std::vector<Room>::iterator eraseit(Rooms.begin());eraseit!=Rooms.end();eraseit++)
							{
								//Find the merge room to be deleted in the main room list
								if(eraseit->getPos()==(*rit2)->getPos())
								{
									Rooms.erase(eraseit);
									break;
								}
							}
						}
					}
					else if(mergeRooms.size()==1)
					{
						std::vector<Building> newBlocks(newRoom.getBlocks());
						Vector3<s64> offset(pos1-mergeRooms[0]->getPos());

						//Adds the new blocks to the only room in the merge list
						for(std::vector<Building>::iterator bit(newBlocks.begin());bit!=newBlocks.end();bit++)
						{
							Vector3<s32> newPos(Vector3<s32>(offset.x,offset.y,offset.z)+bit->getPos());
							bit->setPos(newPos);
							mergeRooms[0]->addBlock(*bit);
						}

						mergeRooms[0]->recalcMaxMin();
						mergeRooms[0]->recalcPower();
						mergeRooms[0]->updateCorners();
					}
				}
				else //The only merge blocks in the matrix are the corners if any so the room does not need to be merged but instead added to the main list
				{
					newRoom.updateCorners();
					newRoom.recalcPower();
					Rooms.push_back(newRoom);
				}
			}
		}
		else
		{
			cout << "Not enough resources to build merge room!\n";
			logger->LogMessage(4,"Not enough resources to build the room.\n");
		}

		//free memory for dynamic array
		for(int m(0);m<=(pos2.x-pos1.x+2);m++)
		{
			for(int n(0);n<=(pos2.y-pos1.y);n++)
			{
				delete [] matrix[m][n];
			}
			delete [] matrix[m];
		}
		delete [] matrix;
	}

}
*/

/****************************
* RemoveRect -	This function handles the removal of
				a rectangular room
****************************/
void BuildingManage::RemoveRect(Vector3<s64> pos3, Vector3<s64> pos4)
{
	Vector3<s64> pos1,pos2;

	if(pos3.x<=pos4.x)
	{
		pos1.x=pos3.x;
		pos2.x=pos4.x;
	}
	else
	{
		pos1.x=pos4.x;
		pos2.x=pos3.x;
	}

	if(pos3.y<=pos4.y)
	{
		pos1.y=pos3.y;
		pos2.y=pos4.y;
	}
	else
	{
		pos1.y=pos4.y;
		pos2.y=pos3.y;
	}
	
	if(pos3.z<=pos4.z)
	{
		pos1.z=pos3.z;
		pos2.z=pos4.z;
	}
	else
	{
		pos1.z=pos4.z;
		pos2.z=pos3.z;
	}

	int i(0),j(0),k(0);
	
	checkNearbyRooms(pos1,pos2,0);

	for(i=0;i<=(pos2.x-pos1.x);i++)
	{
		for(j=0;j<=(pos2.y-pos1.y);j++)
		{
			for(k=0;k<=(pos2.z-pos1.z);k++)
			{
				//Erase the block from the room
				removeRoom(Vector3<s64>(pos1.x+i,pos1.y+j,pos1.z+k));
			}
		}
	}

	for(k=0;k<=(pos2.z-pos1.z);k++)
	{
		alterMergeBlock(Face_XP,Vector3<s64>(pos1.x-1,pos1.y,pos1.z+k),true);
		alterMergeBlock(Face_XM,Vector3<s64>(pos2.x+1,pos1.y,pos1.z+k),true);
	}
	for(i=0;i<=(pos2.x-pos1.x);i++)
	{
		alterMergeBlock(Face_ZP,Vector3<s64>(pos1.x+i,pos1.y,pos1.z-1),true);
		alterMergeBlock(Face_ZM,Vector3<s64>(pos1.x+i,pos1.y,pos2.z+1),true);
	}

	//Recalculates the power requirements for the rooms that were changed
	for(std::vector<Room*>::iterator it(interRooms.begin());it!=interRooms.end();it++)
	{
		(*it)->recalcPower();
		(*it)->updateCorners();
	}
}

/****************************
* Render -	Responsible for rendering all of the buildings
****************************/
void BuildingManage::Render()
{
	std::vector<Room>::iterator it(Rooms.begin());
	

	while(it!=Rooms.end())
	{
		it->Render();
		it++;
	}

	switch(uiCore->build)
	{
	case 0:
		break;
	case 1:
		if(ghostRoom)
			ghostRoom->Render();
		break;
	case 2:
		if(ghostRoom)
			ghostRoom->Render();
		break;
	}

}

/****************************
* clearInterRooms -	Clears the two vectors used by the building routine
****************************/
void BuildingManage::clearInterRooms()
{
	while(!interRooms.empty())
	{
		interRooms.pop_back();
	}
	
	while(!mergeRooms.empty())
	{
		mergeRooms.pop_back();
	}
}

/****************************
* alterMergeBlock -	This function is used by the building function to
					change blocks in currently established rooms in order to
					merge them into a single room both visually and in storage code
****************************/
void BuildingManage::alterMergeBlock(s32 xplus, s32 xminus, s32 zplus, s32 zminus,Vector3<s64> pos, tBOOL add)
{
	if(add)
	{
		//Iterates through the list of rooms to be merged
		for(std::vector<Room*>::iterator it(mergeRooms.begin());it!=mergeRooms.end();it++)
		{
			//If the block is found to exist in this room, call the room's alter function
			if((*it)->checkPos(pos))
			{
				(*it)->alterMergeBlock(xplus,xminus,zplus,zminus,pos,add);
			}
		}
	}
	else
	{
		for(std::vector<Room*>::iterator it(interRooms.begin());it!=interRooms.end();it++)
		{
			if((*it)->checkPos(pos))
			{
				(*it)->alterMergeBlock(xplus,xminus,zplus,zminus,pos,add);
			}
		}
	}
}

void BuildingManage::alterMergeBlock(const int& faceData, const Vector3<s64>& pos, const bool& add)
{
	for(std::vector<Room*>::iterator it(interRooms.begin());it!=interRooms.end();it++)
	{
		if((*it)->checkPos(pos))
		{
			(*it)->alterMergeBlock(faceData,pos,add);
		}
	}
}

/****************************
* removeRoom -	This function handles the removal of a single
				block within a room.
****************************/
void BuildingManage::removeRoom(Vector3<s64> pos)
{
	if(interRooms.size()==0)
		std::cout << "Error! Block trying to be deleted but no rooms available that intersect!\n";

	for(std::vector<Room*>::iterator it(interRooms.begin());it!=interRooms.end();it++)
	{
		(*it)->removeBlock(pos);
	}
}

real32 BuildingManage::powerUpdate(real32 curpower)
{
	real32 total(0);

	for(std::vector<Room>::iterator it(Rooms.begin());it!=Rooms.end();it++)
	{
		if(it->isPowered())
		{
			total+=it->getPower();

			if(curpower-total<=0)
			{
				it->setPower(FALSE);
			}
			else
			{
				it->setPower(TRUE);
			}
		}
		else
		{
			if(curpower-total-it->getPower()>0)
			{
				it->setPower(TRUE);
				total+=it->getPower();
			}
		}
	}

	return total;
}

void BuildingManage::addDoor(const Vector3<s64>& pos,const s32 direction)
{
	for(std::vector<Room>::iterator it(Rooms.begin());it!=Rooms.end();it++)
	{
		if(it->checkPos(pos))
		{
			Vector3<s64> pos2;
			s32 door2(0);
			switch(direction)
			{
			case RM_DOOR2:
				pos2=Vector3<s64>(pos.x,pos.y,pos.z+1);
				door2=RM_DOOR8;
				break;
			case RM_DOOR6:
				pos2=Vector3<s64>(pos.x+1,pos.y,pos.z);
				door2=RM_DOOR4;
				break;
			case RM_DOOR8:
				pos2=Vector3<s64>(pos.x,pos.y,pos.z-1);
				door2=RM_DOOR2;
				break;
			case RM_DOOR4:
				pos2=Vector3<s64>(pos.x-1,pos.y,pos.z);
				door2=RM_DOOR6;
				break;
			}

			if(checkRooms(pos2,it->getType())!=1)
			{
				for(std::vector<Room>::iterator it2(Rooms.begin());it2!=Rooms.end();it2++)
				{
					if(it2->checkPos(pos2))
					{
						it2->addDoor(pos2,door2);
						break;
					}
				}

				it->addDoor(pos,direction);
			}
		}
	}
}

void BuildingManage::rayTrace(const Ray<real32>& ray, RayCol<Building> &rayBuilding)
{
	for(std::vector<Room>::iterator it(Rooms.begin());it!=Rooms.end();it++)
	{
		it->rayTrace(ray,rayBuilding);
	}
}


void BuildingManage::SaveAllBuildings()
{
	FILE *file;
	errno_t err;
	s32 numRooms(Rooms.size());
	std::string filename="saves/" + gameCore->levMan.getWorldName() + "/worldRooms.dat";


	err=fopen_s(&file,filename.c_str(),"wt");

	if(err==0)
	{
		fwrite(&numRooms,sizeof(s32),1,file);

		//Iterate through all rooms and call each room's save
		for(std::vector<Room>::iterator rit(Rooms.begin());rit!=Rooms.end();++rit)
		{
			rit->SaveAll(file);
		}

		fclose(file);
	}
	else
	{
		logger->LogMessage(LOG_LOADER,"Error opening " + filename + " for writing. Make sure the directory has been created and is not write protected!\n");
	}
}

void BuildingManage::LoadAllBuildings()
{
	FILE *file;
	errno_t err;
	std::string filename="saves/" + gameCore->levMan.getWorldName() + "/worldRooms.dat";


	err=fopen_s(&file,filename.c_str(),"rt");

	if(err==0)
	{
		s32 numRooms(0);
		Vector3<s64> rpos(0,0,0);

		fread(&numRooms,sizeof(s32),1,file);

		for(int i(0);i<numRooms;++i)
		{
			Room rtemp;
			s32 numBlocks(0);
			fread(&rpos,sizeof(Vector3<s64>),1,file);
			fread(&numBlocks,sizeof(s32),1,file);

			rtemp.setPos(rpos);

			for(int j(0);j<numBlocks;++j)
			{
				Building btemp;
				
				Vector3<s32> bpos(0,0,0);
				s32 wall(0);
				s32 ori(0);
				s32 rType(0);
				s32 wType(0);
				s32 dPos(0);
				s32 cPos(0);

				//Gather the room data from the file
				fread(&bpos,sizeof(bpos),1,file);
				fread(&wall,sizeof(s32),1,file);
				fread(&ori,sizeof(s32),1,file);
				fread(&rType,sizeof(s32),1,file);
				fread(&wType,sizeof(s32),1,file);
				fread(&dPos,sizeof(s32),1,file);
				fread(&cPos,sizeof(s32),1,file);

				//Set the room data
				btemp.setPos(bpos);
				btemp.setWalls(wall);
				btemp.setOrient(ori);
				btemp.setRoomType(rType);
				btemp.setWallType(wType);
				btemp.setDoor(dPos);
				btemp.setCorner(cPos);

				//Add the block to the room
				rtemp.addBlock(btemp);
			}

			//Add the room to the building manager
			Rooms.push_back(rtemp);

		}
		
		fclose(file);
	}
	else
	{
		logger->LogMessage(LOG_LOADER,"Error opening " + filename + " for reading. Make sure the file has been created!\n");
	}
}


void BuildingManage::buildRoomGenerateGraph(const Vector3<s64> &firstPos, const Vector3<s64> &secondPos,const int &type, const bool &merge, const bool &ghosted)
{
	Vector3<s64> minPos,maxPos;
	minPos.x=firstPos.x<secondPos.x? firstPos.x:secondPos.x;
	minPos.y=firstPos.y<secondPos.y? firstPos.y:secondPos.y;
	minPos.z=firstPos.z<secondPos.z? firstPos.z:secondPos.z;
	
	maxPos.x=firstPos.x>secondPos.x? firstPos.x:secondPos.x;
	maxPos.y=firstPos.y>secondPos.y? firstPos.y:secondPos.y;
	maxPos.z=firstPos.z>secondPos.z? firstPos.z:secondPos.z;

	//populate the nearby rooms list for use
	checkNearbyRooms(minPos, maxPos,type);

	//This is the new code optimized to use the new bitmasks to build.
	BuildingGraph buildGraph(firstPos,secondPos,type,merge,ghosted);
	Vector3<s64> graphSize(buildGraph.getSize());
	Room newRoom;
	
	newRoom.setPos(buildGraph.getMinPos());

	
	if(!ghosted)
	{
		logger->LogMessage(LOG_BUILDING,"\nNew building being built:\n");
		logger->LogMessage(LOG_BUILDING,"Building type: " + uiCore->intToString(type) + "\n");
		logger->LogMessage(LOG_BUILDING,"Position 1 - X: " + uiCore->intToString(firstPos.x) + " Y: " + uiCore->intToString(firstPos.y) + " Z: " + uiCore->intToString(firstPos.z) + "\n");
		logger->LogMessage(LOG_BUILDING,"Position 2 - X: " + uiCore->intToString(secondPos.x) + " Y: " + uiCore->intToString(secondPos.y) + " Z: " + uiCore->intToString(secondPos.z) + "\n");
	}

	const int BUILD_NOT_VIABLE=1;
 	
	for(int xPos(2);xPos<graphSize.x;++xPos)
	{
	 	for(int yPos(2);yPos<graphSize.y;++yPos)
	 	{
	 	 	for(int zPos(2);zPos<graphSize.z;++zPos)
	 	 	{
	 	 	 	if(buildGraph.getData(xPos-1,yPos-1,zPos-1)!=BUILD_NOT_VIABLE)
	 	 	 	{
	 	 	 	 	//check the block on all 6 directions and generate a bitmask
	 	 	 	 	Vector3<s32> buildPos(xPos-1,yPos-1,zPos-1);
	 	 	 	 	int bitmask(buildGraph.getData(buildPos.x,buildPos.y,buildPos.z));

					if(bitmask!=Face_INVALID && bitmask!=Face_MERGE)
					{
	 	 	 	 		Building newBuilding(buildPos-Vector3<s32>(1,1,1), type, bitmask);
	 	 	 	 		newRoom.addBlock(newBuilding);
					}
	 	 	 	}
	 	 	}
	 	}
	}

	if(ghosted)
	{
		if(newRoom.getBlocks().size()!=0)
		{
			if(ghostRoom)
			{
				delete ghostRoom;
				ghostRoom=new Room(newRoom);
			}
			else
			{
				ghostRoom=new Room(newRoom);
			}
		}
	}
	else
	{
		if(newRoom.getBlocks().size()!=0)
		{
			newRoom.updateCorners();
			newRoom.recalcPower();
			if(buildGraph.needsMerged())
			{
				mergeExistingRooms(newRoom);
			}
			else
			{
				Rooms.push_back(newRoom);
			}
		}
	}
}

void BuildingManage::mergeExistingRooms(const Room& newRoom)
{
	logger->LogMessage(LOG_BUILDING,"New room needs to be merged with an existing room. Commencing merge...\n");
	if(mergeRooms.size()>1)
	{
		std::vector<Building> newBlocks(newRoom.getBlocks());
		std::vector<Building>::iterator bit;
		std::vector<Room*>::iterator rit2;
		Vector3<s64> offset(newRoom.getPos()-mergeRooms[0]->getPos());
		Vector3<s32> newPos;

		//Adds the new blocks to the only room in the merge list
		for(bit=newBlocks.begin();bit!=newBlocks.end();bit++)
		{
			newPos=Vector3<s32>(offset.x,offset.y,offset.z)+bit->getPos();
			bit->setPos(newPos);
			mergeRooms[0]->addBlock(*bit);
		}

		//Adds the additional merge rooms to the first room
		for(rit2=(mergeRooms.begin()+1);rit2!=mergeRooms.end();rit2++)
		{
			newBlocks=(*rit2)->getBlocks();
			offset=(*rit2)->getPos()-mergeRooms[0]->getPos();
			for(bit=newBlocks.begin();bit!=newBlocks.end();bit++)
			{
				newPos=Vector3<s32>(offset.x,offset.y,offset.z)+bit->getPos();
				bit->setPos(newPos);
				mergeRooms[0]->addBlock(*bit);
			}
				
			mergeRooms[0]->recalcMaxMin();
			mergeRooms[0]->recalcPower();
			mergeRooms[0]->updateCorners();

			//delete the merge room from the main room list to prevent duplicates
			for(std::vector<Room>::iterator eraseit(Rooms.begin());eraseit!=Rooms.end();eraseit++)
			{
				//Find the merge room to be deleted in the main room list
				if(eraseit->getPos()==(*rit2)->getPos())
				{
					Rooms.erase(eraseit);
					break;
				}
			}
		}
	}
	else if(mergeRooms.size()==1)
	{
		std::vector<Building> newBlocks(newRoom.getBlocks());
		Vector3<s64> offset(newRoom.getPos()-mergeRooms[0]->getPos());

		//Adds the new blocks to the only room in the merge list
		for(std::vector<Building>::iterator bit(newBlocks.begin());bit!=newBlocks.end();bit++)
		{
			Vector3<s32> newPos(Vector3<s32>(offset.x,offset.y,offset.z)+bit->getPos());
			bit->setPos(newPos);
			mergeRooms[0]->addBlock(*bit);
		}

		mergeRooms[0]->recalcMaxMin();
		mergeRooms[0]->recalcPower();
		mergeRooms[0]->updateCorners();
	}
}

int BuildingManage::generateRoomMask(const Vector3<s32>& current, const Vector3<s64>& graphSize)
{
	int bitmask(0);

	if(current.x==2)
	{
		if(current.x==graphSize.x-1)
			bitmask|=Face_XP;

		bitmask|=Face_XM;
	}
	else if(current.x==graphSize.x-1)
		bitmask|=Face_XP;

	/*if(current.y==2)
		bitmask|=Face_YM;
	else if(current.y==graphSize.y-1)
		bitmask|=Face_YP;
	*/

	if(current.z==2)
	{
		if(current.z==graphSize.z-1)
			bitmask|=Face_ZP;

		bitmask|=Face_ZM;
	}
	else if(current.z==graphSize.z-1)
		bitmask|=Face_ZP;

	return bitmask;
}