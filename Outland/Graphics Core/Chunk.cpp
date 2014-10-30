#include "Chunk.h"
#include <Outland\Singletons.h>

//Global Externs
extern int CHUNK_SIZE;
extern int HEIGHT_CHUNK;
extern bool activeRender;
extern bool polyRender;
extern bool polyWireframe;
extern bool lowPolyRender;

/****************************
* Constructor
****************************/
Chunk::Chunk():
	blocks(NULL),
	pos(0,0,0),
	TOTAL_RENDER(FALSE)
{
	//create the blocks
	blocks=new Block**[CHUNK_SIZE];
	for(int i=0;i<CHUNK_SIZE;i++)
	{
		blocks[i]=new Block*[CHUNK_SIZE];

		for(int j=0;j<CHUNK_SIZE;j++)
		{
			blocks[i][j]=new Block[CHUNK_SIZE];
		}
	}

	//Assign the block positional data within the chunk
	for(int i=0;i<CHUNK_SIZE;++i)
	{
		for(int j=0;j<CHUNK_SIZE;++j)
		{
			for(int k=0;k<CHUNK_SIZE;++k)
			{
				blocks[i][j][k].setPos(Vector3<s32>(i,j,k));
			}
		}
	}
}
	
Chunk::Chunk(const Vector3<s64>& newpos):
	blocks(NULL),
	pos(newpos),
	TOTAL_RENDER(FALSE)
{
	//create the blocks
	blocks=new Block**[CHUNK_SIZE];
	for(int i=0;i<CHUNK_SIZE;i++)
	{
		blocks[i]=new Block*[CHUNK_SIZE];

		for(int j=0;j<CHUNK_SIZE;j++)
		{
			blocks[i][j]=new Block[CHUNK_SIZE];
		}
	}

	//Assign the block positional data within the chunk
	for(int i=0;i<CHUNK_SIZE;++i)
	{
		for(int j=0;j<CHUNK_SIZE;++j)
		{
			for(int k=0;k<CHUNK_SIZE;++k)
			{
				blocks[i][j][k].setPos(Vector3<s32>(i,j,k));
			}
		}
	}
}

/****************************
* Destructor
****************************/
Chunk::~Chunk()
{
	while(!blockRenderList.empty())
	{
		blockRenderList.pop_back();
	}

	if(blocks!=NULL)
	{
		//Unload the blocks
		for(int i=0;i<CHUNK_SIZE;++i)
		{
			for(int j=0;j<CHUNK_SIZE;++j)
			{
				delete [] blocks[i][j];
			}

			delete [] blocks[i];
		}

		delete [] blocks;
	}

	blocks=NULL;
}

/****************************
* TestRender
****************************/
void Chunk::Render()
{

	glPushMatrix();	

	glTranslatef((real32)pos.x,(real32)pos.y,(real32)pos.z);

	if(TOTAL_RENDER)
	{
		for(int i=0;i<CHUNK_SIZE;++i)
		{
			for(int j=0;j<CHUNK_SIZE;++j)
			{
				for(int k=0;k<CHUNK_SIZE;++k)
				{
					//calls the render function from the OpenGL Renderer class for cubes
					if(blocks[i][j][k].getType()==BlockType_Dirt || blocks[i][j][k].getType()==BlockType_MARKER)		//TO BE REPLACED WITH CALLS TO THE RENDER LIST VECTOR
					{
						grCore->pRenderer->RenderTestCube(blocks[i][j][k],i,j,k);
					}
				}
			}
		}
	}
	else if(activeRender)
	{
		//Iterates through the active blocks and renders them on screen
		for(std::vector<Block>::iterator it(blockRenderList.begin());it!=blockRenderList.end();++it)
		{
			grCore->pRenderer->RenderTestCube((*it),(*it).getPos().x,(*it).getPos().y,(*it).getPos().z);
		}
	}
	
	if(polyWireframe)
	{
		glPolygonMode(GL_FRONT,GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT,GL_FILL);
	}
	
	if(polyRender)
	{

		polys.renderGroup();
		
	}
		
	if(lowPolyRender)
	{
		polys.setRender(false);
		polys.renderGroup();
	}

	glPopMatrix();
}

/****************************
* CheckActive - This function will check if the block matches the passed bool and toggles it
****************************/
void Chunk::CheckActive(const Vector3<s32>& blockPos,const bool& remove)
{
	//if the block's status doesn't match, call the change function
	if(blocks[blockPos.x][blockPos.y][blockPos.z].checkActive()!=remove)
	{
		ChangeActive(blockPos);
	}

}

/****************************
* ChangeActive
****************************/
void Chunk::ChangeActive(const Vector3<s32>& blockPos)
{
	//Toggles the state of the block's active statues
	if(blocks[blockPos.x][blockPos.y][blockPos.z].checkActive())
	{	
		//sets false
		blocks[blockPos.x][blockPos.y][blockPos.z].setActive(FALSE);
		
		//Iterates to the block in the render list
		std::vector<Block>::iterator iterate=blockRenderList.begin();
		while(iterate!=blockRenderList.end())
		{
			if(iterate->getPos()==blockPos)
			{
				break;
			}
			iterate++;
		}

		blockRenderList.erase(iterate);
	}
	else
	{
		blocks[blockPos.x][blockPos.y][blockPos.z].setActive(TRUE);
		blockRenderList.push_back(blocks[blockPos.x][blockPos.y][blockPos.z]);
	}
}

/****************************
* Generate - This function will generate a
				newly created chunk based on
				it's position and the world seed.
****************************/
void Chunk::Generate()
{
	real64 height;
	s64 h;
	s64 y(pos.y);

	for(int i=0;i<CHUNK_SIZE;i++)
	{
		for(int j=0;j<CHUNK_SIZE;j++)
		{
			height=grCore->LGen.genValue(pos.x+i,pos.z+j,grCore->getSeed());
			
			if(height<0)
				height=height*-1;
			h=(s64)height+4;
			
			for(int k=0;k<CHUNK_SIZE;k++)
			{
				if(y+k>h)
				{
					blocks[i][k][j].setType(BlockType_Air);
				}
				else if(y+k==h/* || y+k+1==h*/)
				{
					blocks[i][k][j].setType(BlockType_Dirt);
				}
				else
				{
					blocks[i][k][j].setType(BlockType_Rock);
				}
			}
		}
	}
}

/****************************
* activeCulling - Runs the active culling algorithm to hide blocks
					that won't be able to be seen.
****************************/
void Chunk::activeCulling()
{
	//clears the block render list
	while(!blockRenderList.empty())
	{
		blockRenderList.pop_back();
	}

	for(int i=0;i<CHUNK_SIZE;++i)
	{
		for(int j=0;j<CHUNK_SIZE;++j)
		{
			for(int k=0;k<CHUNK_SIZE;++k)
			{
				if(blocks[i][j][k].getType()!=BlockType_Air)
				{
					bool blockactive(blocks[i][j][k].check6Faces(pos));
					blocks[i][j][k].setActive(blockactive);
					if(blockactive){
						blockRenderList.push_back(blocks[i][j][k]);
						addPolysFromBlocks(blocks[i][j][k]);
					}
					//check surrounding blocks
					/*if(i==0 || i==CHUNK_SIZE-1)
					{
						blocks[i][j][k].setActive(TRUE);
						blockRenderList.push_back(blocks[i][j][k]);
					}
					else if(j==0 || j==CHUNK_SIZE-1)
					{
						blocks[i][j][k].setActive(TRUE);
						blockRenderList.push_back(blocks[i][j][k]);
					}
					else if(k==0 || k==CHUNK_SIZE-1)
					{
						blocks[i][j][k].setActive(TRUE);
						blockRenderList.push_back(blocks[i][j][k]);
					}
					else if(blocks[i+1][j][k].getType()==BlockType_Air)
					{
						blocks[i][j][k].setActive(TRUE);
						blockRenderList.push_back(blocks[i][j][k]);
					}
					else if(blocks[i-1][j][k].getType()==BlockType_Air)
					{
						blocks[i][j][k].setActive(TRUE);
						blockRenderList.push_back(blocks[i][j][k]);
					}
					else if(blocks[i][j+1][k].getType()==BlockType_Air)
					{
						blocks[i][j][k].setActive(TRUE);
						blockRenderList.push_back(blocks[i][j][k]);
					}
					else if(blocks[i][j-1][k].getType()==BlockType_Air)
					{
						blocks[i][j][k].setActive(TRUE);
						blockRenderList.push_back(blocks[i][j][k]);
					}
					else if(blocks[i][j][k+1].getType()==BlockType_Air)
					{
						blocks[i][j][k].setActive(TRUE);
						blockRenderList.push_back(blocks[i][j][k]);
					}
					else if(blocks[i][j][k-1].getType()==BlockType_Air)
					{
						blocks[i][j][k].setActive(TRUE);
						blockRenderList.push_back(blocks[i][j][k]);
					}
					else
					{
						blocks[i][j][k].setActive(FALSE);
					}*/
				}
				else
				{
					blocks[i][j][k].setActive(false);
				}
			}
		}
	}

	polys.toggleSetup();
	polys.polygonReduction();
}

/****************************
* visibleCulling -	Runs the visible culling algorithm to trim all blocks that are obstructed
					from view
****************************/
void Chunk::visibleCulling()
{

}

/****************************
* rayTrace -	Runs the ray trace check against the full chunk.
				If there is an intersection with the chunk, it returns true
				and the algorithm goes into check each active block.
****************************/
real32 Chunk::rayTrace(const Ray<real32>& ray)
{
	Vector3<real64> posconv(pos.x,pos.y,pos.z);
	Vector3<real64> offset(0.5,0.5,0.5),bounds[2]={(posconv-offset),(posconv-offset+Vector3<real64>(10,10,10))};
	real64 tmin,tmax,tymin,tymax,tzmin,tzmax;
	
	tmin=(bounds[ray.sign[0]].x - ray.orig.x) * ray.invdir.x;
	tmax=(bounds[1-ray.sign[0]].x-ray.orig.x) * ray.invdir.x;
	tymin=(bounds[ray.sign[1]].y - ray.orig.y) * ray.invdir.y;
	tymax=(bounds[1-ray.sign[1]].y-ray.orig.y) * ray.invdir.y;

	if((tmin>tymax) || (tymin>tmax))
		return -1;
	if(tymin > tmin)
		tmin=tymin;
	if(tymax<tmax)
		tmax=tymax;
	
	tzmin=(bounds[ray.sign[2]].z - ray.orig.z) * ray.invdir.z;
	tzmax=(bounds[1-ray.sign[2]].z-ray.orig.z) * ray.invdir.z;

	if((tmin>tzmax)||(tzmin>tmax))
		return -1;
	if(tzmin>tmin)
		tmin=tzmin;
	if(tzmax<tmax)
		tmax=tzmax;

	return tmin;
}

/****************************
* blockRayTrace -	Runs the block ray trace check. If a block intersects
					the ray, it is kept until the full chunk is iterated through. 
					Only the nearest block is kept and returned to the algorithm.
****************************/
real32 Chunk::blockRayTrace(const Ray<real32>& ray,s32 &face,RayCol<Block>& rayBlock)
{
	real64 totmin(-1);

	for(std::vector<Block>::iterator it(blockRenderList.begin());it!=blockRenderList.end();it++)
	{
		Vector3<real64> posconv(pos.x+it->getPos().x,pos.y+it->getPos().y,pos.z+it->getPos().z);
		Vector3<real64> offset(1,1,1),bounds[2]={(posconv),(posconv+offset)};
		real64 tmin,tmax,tymin,tymax,tzmin,tzmax;
		s32 facetrack;
	
		tmin=(bounds[ray.sign[0]].x - ray.orig.x) * ray.invdir.x;
		tmax=(bounds[1-ray.sign[0]].x-ray.orig.x) * ray.invdir.x;
		tymin=(bounds[ray.sign[1]].y - ray.orig.y) * ray.invdir.y;
		tymax=(bounds[1-ray.sign[1]].y-ray.orig.y) * ray.invdir.y;
		if(ray.sign[0]==1)
			facetrack=3;
		else
			facetrack=4;

		if((tmin>tymax) || (tymin>tmax))
			continue;
		if(tymin > tmin)
		{
			tmin=tymin;
			if(ray.sign[1]==1)
				facetrack=1;
			else
				facetrack=2;
		}
		if(tymax<tmax)
			tmax=tymax;
	
		tzmin=(bounds[ray.sign[2]].z - ray.orig.z) * ray.invdir.z;
		tzmax=(bounds[1-ray.sign[2]].z-ray.orig.z) * ray.invdir.z;

		if((tmin>tzmax)||(tzmin>tmax))
			continue;
		if(tzmin>tmin)
		{
			tmin=tzmin;
			if(ray.sign[2]==1)
				facetrack=5;
			else
				facetrack=6;
		}
		if(tzmax<tmax)
			tmax=tzmax;

		if(tmin>0 && tmin<totmin){
			totmin=tmin;
			face=facetrack;
		}
		else if(totmin==-1 && tmin>0){
			totmin=tmin;
			face=facetrack;
		}

		if(totmin<rayBlock.dist && it->getPos().y==5){
			rayBlock.dist=totmin;
			rayBlock.blockCol=&(*it);
			rayBlock.face=face;
			rayBlock.sOffset=pos;
		}
	}

	return totmin;
}

void Chunk::CopyChunk(const Chunk& chunk)
{
	for(int i(0);i<CHUNK_SIZE;++i)
	{
		for(int j(0);j<CHUNK_SIZE;++j)
		{
			for(int k(0);k<CHUNK_SIZE;++k)
			{
				blocks[i][j][k]=chunk.blocks[i][j][k];
			}
		}
	}

	activeCulling();

	pos=chunk.pos;
}

void Chunk::addPolysFromBlocks(const Block& block)
{
	if((block.getFaceData()&Face_YP)==Face_YP)
	{
		Poly newFace(Vector3<s32>(block.getPos().x,block.getPos().y+1,block.getPos().z),
			Vector3<s32>(block.getPos().x,block.getPos().y+1,block.getPos().z+1),
			Vector3<s32>(block.getPos().x+1,block.getPos().y+1,block.getPos().z+1),
			Vector3<s32>(block.getPos().x+1,block.getPos().y+1,block.getPos().z),
			block.getType());

		polys.addPoly(newFace);
	}
}