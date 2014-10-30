#include "Polygroup.h"
#include <Outland\Singletons.h>

Polygroup::Polygroup():
	highPolyReady(false),
	initialSetup(true)
{};

void Polygroup::addPoly(const Poly& poly)
{
	bool add(true);
	int polyX(poly.getPoint(0).x), polyZ(poly.getPoint(0).z);
	for(std::vector<Poly>::iterator gridIt(grid[polyX][polyZ].begin());gridIt!=grid[polyX][polyZ].end();++gridIt)
	{
		if((*gridIt)==poly)
		{
			add=false;
		}
	}

	if(add){
		grid[polyX][polyZ].push_back(poly);
		lowEfficientPolys.push_back(poly);
		if(!initialSetup)
		{
			//create a new thread and run the polygon reduction again

		}
		else
		{
			highEfficientPolys.push_back(poly);
		}
	}
}

void Polygroup::removePoly(const Poly& poly)
{
	int polyX(poly.getPoint(0).x), polyZ(poly.getPoint(0).z);

	//Remove poly from the grid
	for(std::vector<Poly>::iterator gridIt(grid[polyX][polyZ].begin());gridIt!=grid[polyX][polyZ].end();++gridIt)
	{
		if((*gridIt)==poly)
		{
			grid[polyX][polyZ].erase(gridIt);
		}
	}
	
	//Remove poly from the low efficiency poly list
	for(std::vector<Poly>::iterator lowIt(lowEfficientPolys.begin());lowIt!=lowEfficientPolys.end();++lowIt)
	{
		if((*lowIt)==poly)
		{
			lowEfficientPolys.erase(lowIt);
		}
	}

	if(!initialSetup)
	{
		//create a new thread and run the polygon reduction again
	}
}

void Polygroup::polygonReduction()
{
	//This function will be started in a new thread by the calling function so it will not interfere with main operation
	highEfficientPolys.clear();
	highPolyReady=false;
	std::vector<Poly> gridCopy[10][10];

	logger->LogMessage(LOG_POLYGON,"\nStarting new polygon reduction. Printing grid of heights:\n\n");

	for(int x(0);x<10;++x)
	{
		for(int z(0);z<10;++z)
		{
			for(std::vector<Poly>::iterator copyIt(grid[x][z].begin());copyIt!=grid[x][z].end();++copyIt)
			{
				gridCopy[x][z].push_back((*copyIt));
				logger->LogMessage(3,uiCore->intToString(gridCopy[x][z][0].getPoint(0).y) + " ");
			}
		}

		logger->LogMessage(3,"\n");
	}

	for(int xPos(0);xPos<10;++xPos)
	{
		for(int zPos(0);zPos<10;++zPos)
		{
			for(std::vector<Poly>::iterator gridIt(gridCopy[xPos][zPos].begin());gridIt!=gridCopy[xPos][zPos].end();++gridIt)
			{
				bool breakout=false;
				if(gridIt->getNormal()==Vector3<s32>(0,1,0) || gridIt->getNormal()==Vector3<s32>(0,-1,0))
				{
					Poly storedPoly((*gridIt));
					int xPlus(0),zPlus(0);
					bool xFailed(false),zFailed(false);

					while(!xFailed || !zFailed)
					{
						if(!xFailed)
						{
							++xPlus;
							for(int nextX(zPos);nextX<=(zPos+zPlus);++nextX)
							{
								if(xPos+xPlus==10 && !xFailed)
								{
									xFailed=true;
									if(xPlus>0)
										--xPlus;
								}
								else
								{
									bool fail(true);
									for(std::vector<Poly>::iterator posIt(gridCopy[xPos+xPlus][nextX].begin());posIt!=gridCopy[xPos+xPlus][nextX].end();++posIt)
									{
										if((posIt->getNormal()==storedPoly.getNormal()) && (posIt->getPoint(0).y==storedPoly.getPoint(0).y))
										{
											fail=false;
											break;
										}
									}

									if(fail)
									{
										xFailed=true;
										if(xPlus>0)
											--xPlus;
									}
								}
							}

						}

						if(!zFailed)
						{
							++zPlus;
							for(int nextZ(xPos);nextZ<=(xPos+xPlus);++nextZ)
							{
								if(zPos+zPlus==10 && !zFailed)
								{
									zFailed=true;
									if(zPlus>0)
										--zPlus;
								}
								else
								{
									bool fail(true);
									for(std::vector<Poly>::iterator posIt(gridCopy[nextZ][zPos+zPlus].begin());posIt!=gridCopy[nextZ][zPos+zPlus].end();++posIt)
									{
										if((posIt->getNormal()==storedPoly.getNormal()) && (posIt->getPoint(0).y==storedPoly.getPoint(0).y))
										{
											fail=false;
											break;
										}
									}

									if(fail)
									{
										zFailed=true;
										if(zPlus>0)
											--zPlus;
									}
								}
							}

						}
					}

					Poly newPoly(storedPoly.getPoint(0),
						Vector3<s32>(storedPoly.getPoint(0).x,storedPoly.getPoint(0).y,storedPoly.getPoint(0).z+zPlus+1),
						Vector3<s32>(storedPoly.getPoint(0).x+xPlus+1,storedPoly.getPoint(0).y,storedPoly.getPoint(0).z+zPlus+1),
						Vector3<s32>(storedPoly.getPoint(0).x+xPlus+1,storedPoly.getPoint(0).y,storedPoly.getPoint(0).z),
						storedPoly.getType());	//removing +1 temporarily

					for(int x(xPos);x<=xPos+xPlus;++x)
					{
						for(int z(zPos);z<=zPos+zPlus;++z)
						{
							for(std::vector<Poly>::iterator posIt(gridCopy[x][z].begin());posIt!=gridCopy[x][z].end();++posIt)
							{
								if(posIt->getNormal()==storedPoly.getNormal() && (posIt->getPoint(0).y==storedPoly.getPoint(0).y))
								{
									gridCopy[x][z].erase(posIt);
									logger->LogMessage(LOG_POLYGON,"Deleting Poly from grid at X: " + uiCore->intToString(x) + " Z: " + uiCore->intToString(z) + "\n");
					
									breakout=true;
									break;
								}
							}
						}
					}

					highEfficientPolys.push_back(newPoly);
					logger->LogMessage(LOG_POLYGON,"Creating new Poly:\n");
					logger->LogMessage(LOG_POLYGON,"minX: " + uiCore->intToString(newPoly.getPoint(0).x) + " minZ: " + uiCore->intToString(newPoly.getPoint(0).z) + "\n");
					logger->LogMessage(LOG_POLYGON,"maxX: " + uiCore->intToString(newPoly.getPoint(2).x-1) + " maxZ: " + uiCore->intToString(newPoly.getPoint(2).z-1) + "\n");
				}
				else
				{
					highEfficientPolys.push_back((*gridIt));
				}

				if(breakout)
					break;
			}
		}
	}

	highPolyReady=true;
}

void Polygroup::renderGroup()
{
	if(!highPolyReady)
	{
		for(std::vector<Poly>::iterator lowPoly(lowEfficientPolys.begin());lowPoly!=lowEfficientPolys.end();++lowPoly)
		{
			lowPoly->renderPoly();
		}
	}
	else
	{
		for(std::vector<Poly>::iterator highPoly(highEfficientPolys.begin());highPoly!=highEfficientPolys.end();++highPoly)
		{
			highPoly->renderPoly();
		}
	}
}

void Polygroup::clearPolygroup()
{
	lowEfficientPolys.clear();
	highEfficientPolys.clear();
	highPolyReady=false;
}