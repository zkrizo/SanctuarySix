#include "ChunkManager.h"
#include <Outland\Singletons.h>

/****************************
* Constructor
****************************/
ChunkManager::ChunkManager():
	p_chunks(NULL),
	blockHit(NULL)
{
	p_chunks=new Chunk**[RENDER_DIST];

	for(int i=0;i<RENDER_DIST;i++)
	{
		p_chunks[i]=new Chunk*[HEIGHT_CHUNK];
		for(int j=0;j<HEIGHT_CHUNK;j++)
		{
			p_chunks[i][j]=new Chunk[RENDER_DIST];
		}
	}
}

/****************************
* Update
****************************/
ChunkManager::~ChunkManager()
{
	//Unload the chunks
	for(int i=0;i<RENDER_DIST;++i)
	{
		for(int j=0;j<HEIGHT_CHUNK;++j)
		{
			delete [] p_chunks[i][j];
		}

		delete [] p_chunks[i];
	}

	delete [] p_chunks;
}

/****************************
* RDistChange
****************************/
void ChunkManager::RDistChange(int newDist)
{
	//This function is called when the user changes the render distance within the configuration. This will rebuild the chunk array to the
	//appropriate size. This can be expensive because of the dynamic memory clearing and re-assigning.

	//Unload the chunks
	for(int i=0;i<RENDER_DIST;++i)
	{
		for(int j=0;j<HEIGHT_CHUNK;++j)
		{
			delete [] p_chunks[i][j];
		}

		delete [] p_chunks[i];
	}

	delete [] p_chunks;

	//sets the new render distance
	RENDER_DIST=newDist;

	//uses the ChunkManager constructor to rebuild the dynamic chunk array with the new render distance
	ChunkManager();
}

/****************************
* Render
****************************/
void ChunkManager::Render()
{
	/*	Render function list:
		1. Cycle through active chunks
		2. Apply frustrum, occlusion, and inactive block culling if needed
		3. Render active and in-view blocks
	*/
	std::vector<Chunk*>::iterator it=renderList.begin();

	while(it!=renderList.end())
	{
		(*it)->Render();
		it++;
	}
	
	
	glPolygonMode(GL_FRONT,GL_FILL);
	glBindTexture(GL_TEXTURE_2D,0);

#ifdef _DEBUG
	tempRay.Render();
#endif
	/*//temporarily go through all chunks
	for(int i=0;i<RENDER_DIST;i++)
	{
		for(int j=0;j<RENDER_DIST;j++)
		{
			for(int k=0;k<RENDER_DIST;k++)
			{
				p_chunks[i][j][k].Render();
			}
		}
	}	*/
}

/****************************
* Load - This function is responsible for loading all chunks around
		the player on initial startup. For singular chunk loading, 
		use function LoadChunk();
****************************/
void ChunkManager::Load()
{
	/*Load Function List:
		1. Obtain Player Position
		2. Load chunks from file around the player
			a. If file isn't there, generate chunks around player
		3. Perform Culling
		4. 
	*/


	//Calls the level manager to load the level data
	
	//Obtain player position
	Vector3<real64> playerPos=gameCore->player.getPos();

	/*//Loads the chunks around the player's position into the chunk data

	*/
	Vector3<s64> chunkPos;
	s32 renderVar=RENDER_DIST/2;
	
	for(int i=0;i<RENDER_DIST;i++)
	{
		for(int j=0;j<HEIGHT_CHUNK;j++)
		{
			for(int k=0;k<RENDER_DIST;k++)
			{
				chunkPos.x=(s64)((10*i-(10*renderVar)));
				chunkPos.y=(s64)10*j;		//may need to comment out later since we'll only have a set variance in y
				chunkPos.z=(s64)((10*k-(10*renderVar)));		//or replace with an if command controlling it.
				p_chunks[i][j][k].pos=chunkPos;
				p_chunks[i][j][k].Generate();
				std::string chunkKey(uiCore->intToString(chunkPos.x) + uiCore->intToString(chunkPos.y) + uiCore->intToString(chunkPos.z));
				chunkMap.emplace(make_pair(chunkKey,&p_chunks[i][j][k]));
				GUILoad->incrementAndRender();
			}
		}
	}
	
	//Runs the active culling once all chunks are loaded
	for(int i=0;i<RENDER_DIST;i++)
	{
		for(int j=0;j<HEIGHT_CHUNK;j++)
		{
			for(int k=0;k<RENDER_DIST;k++)
			{
				p_chunks[i][j][k].activeCulling();
				GUILoad->incrementAndRender();
			}
		}
	}
}

/****************************
* UnloadAll - This function is used to unload all current chunks
				from the render list. Used when changing planets
				in later versions, but also on shutdown
****************************/
void ChunkManager::UnloadAll()
{
	Chunk emptyChunk;

	while(renderList.size()!=0)
	{
		renderList.pop_back();
	}

	//Unloads all of the chunks from the list
	for(int xvar=0;xvar<RENDER_DIST;++xvar)
	{
		for(int yvar=0;yvar<RENDER_DIST;++yvar)
		{
			for(int zvar=0;zvar<RENDER_DIST;++zvar)
			{
				//p_chunks[xvar][yvar][zvar]=emptyChunk;
			}
		}
	}
}

/****************************
* Unload
****************************/
void ChunkManager::Unload(Chunk* oldchunk)
{
	//This function is responsible for removing the chunk from the render list and the list of chunks loaded
	std::vector<Chunk*>::iterator iterate=renderList.begin();

	while(iterate!=renderList.end())
	{
		if((*iterate)==oldchunk)
		{
			renderList.erase(iterate);

			std::string chunkKey(uiCore->intToString((*iterate)->pos.x) + uiCore->intToString((*iterate)->pos.y) + uiCore->intToString((*iterate)->pos.z));

			hash_map<std::string,Chunk*>::iterator findIt;
			findIt=chunkMap.find(chunkKey);
	
			if(findIt!=chunkMap.end())
			{
				Chunk* it(NULL);
				Chunk emptyChunk;
				it=findIt->second;

				*it=emptyChunk;
			}
		}
	}
}

/****************************
* ChangeBlock - This function is called whenever a block is added or removed
				and will alter the surrounding blocks to correct the active
				and visibility culling lists.
****************************/
void ChunkManager::ChangeBlock(Vector3<s64> pos,bool removed)
{
	s32 i,j,k;

	Vector3<s64> chunkPos(calculateChunkPos(pos));
	Vector3<s32> blockPos(calculateBlockPos(pos,chunkPos));

	if(blockPos.x<0){
		chunkPos.x-=10;
		blockPos.x+=10;}
	if(blockPos.y<0){
		chunkPos.y-=10;
		blockPos.y+=10;}
	if(blockPos.z<0){
		chunkPos.z-=10;
		blockPos.z+=10;}

	//retrieve chunk for the given position
	for(i=0;i<RENDER_DIST;i++)
	{
		for(j=0;j<HEIGHT_CHUNK;j++)
		{
			for(k=0;k<RENDER_DIST;k++)
			{
				if(p_chunks[i][j][k].pos==pos)
					break;
			}
		}
	}

	
	//toggles the changed block
	p_chunks[i][j][k].ChangeActive(blockPos);

	//lower block edge
	if(blockPos.x==0)
	{
		//send rightmost block in left chunk
		p_chunks[i-1][j][k].CheckActive(Vector3<s32>(CHUNK_SIZE-1,blockPos.y,blockPos.z),removed);
		p_chunks[i][j][k].CheckActive(Vector3<s32>(blockPos.x+1,blockPos.y,blockPos.z),removed);
	}
	//higher block edge
	else if(blockPos.x==CHUNK_SIZE-1)
	{
		//send leftmost block in right chunk
		p_chunks[i+1][j][k].CheckActive(Vector3<s32>(0,blockPos.y,blockPos.z),removed);
		p_chunks[i][j][k].CheckActive(Vector3<s32>(blockPos.x-1,blockPos.y,blockPos.z),removed);
	}
	//all blocks within chunk for x value
	else
	{
		p_chunks[i][j][k].CheckActive(Vector3<s32>(blockPos.x-1,blockPos.y,blockPos.z),removed);
		p_chunks[i][j][k].CheckActive(Vector3<s32>(blockPos.x+1,blockPos.y,blockPos.z),removed);
	}

	//Vertical Chunk
	//lower block edge
	if(blockPos.y==0)
	{
		//send rightmost block in left chunk
		p_chunks[i][j-1][k].CheckActive(Vector3<s32>(blockPos.x,CHUNK_SIZE-1,blockPos.z),removed);
		p_chunks[i][j][k].CheckActive(Vector3<s32>(blockPos.x,blockPos.y+1,blockPos.z),removed);
	}
	//higher block edge
	else if(blockPos.y==CHUNK_SIZE-1)
	{
		//send leftmost block in right chunk
		p_chunks[i][j+1][k].CheckActive(Vector3<s32>(blockPos.x,0,blockPos.z),removed);
		p_chunks[i][j][k].CheckActive(Vector3<s32>(blockPos.x,blockPos.y-1,blockPos.z),removed);
	}
	//all blocks within chunk for x value
	else
	{
		p_chunks[i][j][k].CheckActive(Vector3<s32>(blockPos.x,blockPos.y-1,blockPos.z),removed);
		p_chunks[i][j][k].CheckActive(Vector3<s32>(blockPos.x,blockPos.y+1,blockPos.z),removed);
	}


	//Depth chunk
	//lower block edge
	if(blockPos.z==0)
	{
		//send rightmost block in left chunk
		p_chunks[i][j][k-1].CheckActive(Vector3<s32>(blockPos.x,blockPos.y,CHUNK_SIZE-1),removed);
		p_chunks[i][j][k].CheckActive(Vector3<s32>(blockPos.x,blockPos.y,blockPos.z+1),removed);
	}
	//higher block edge
	else if(blockPos.z==CHUNK_SIZE-1)
	{
		//send leftmost block in right chunk
		p_chunks[i][j][k+1].CheckActive(Vector3<s32>(blockPos.x,blockPos.y,0),removed);
		p_chunks[i][j][k].CheckActive(Vector3<s32>(blockPos.x,blockPos.y,blockPos.z-1),removed);
	}
	//all blocks within chunk for x value
	else
	{
		p_chunks[i][j][k].CheckActive(Vector3<s32>(blockPos.x,blockPos.y,blockPos.z-1),removed);
		p_chunks[i][j][k].CheckActive(Vector3<s32>(blockPos.x,blockPos.y,blockPos.z+1),removed);
	}

}

/****************************
* CheckEdges - This function iterates through all loaded chunks
				and ensures that the edges of chunks are rendering correctly

**--PROCESSOR INTENSIVE!!--**
****************************/
void ChunkManager::CheckEdges()
{
	//Check end faces (z=0, z=RenderDist-1, x=0, x=Renderdist-1, y=0,y=RenderDist-1)

	//Iterate through the interior spaces of loaded cubes
	for(int i=1;i<RENDER_DIST-1;i++)
	{
		for(int j=1;j<HEIGHT_CHUNK-1;j++)
		{
			for(int k=1;k<RENDER_DIST-1;k++)
			{
				//begin edge checking for chunks
				for(int l=0;l<CHUNK_SIZE;l++)
				{
					for(int m=0;m<CHUNK_SIZE;m++)
					{
						//Front Edges
						tBOOL active=p_chunks[i][j][k].blocks[l][m][0].checkActive();
						s32 type;
						type=p_chunks[i][j][k].blocks[l][m][0].getType();

						//See-through blocktype check
						if(type!=BlockType_Air)
						{
							//check 6DOF around block
							CheckEdges(Vector3<s64>(l,m,0));
						}

						//Back Edges
						active=p_chunks[i][j][k].blocks[l][m][CHUNK_SIZE-1].checkActive();
						type=p_chunks[i][j][k].blocks[l][m][CHUNK_SIZE-1].getType();

						//See-through blocktype check
						if(type!=BlockType_Air)
						{
							//check 6DOF around block
							CheckEdges(Vector3<s64>(l,m,CHUNK_SIZE-1));
						}

						//Left Edges
						active=p_chunks[i][j][k].blocks[0][l][m].checkActive();
						type=p_chunks[i][j][k].blocks[0][l][m].getType();

						//See-through blocktype check
						if(type!=BlockType_Air)
						{
							//check 6DOF around block
							CheckEdges(Vector3<s64>(0,l,m));
						}

						//Right Edges
						active=p_chunks[i][j][k].blocks[CHUNK_SIZE-1][l][m].checkActive();
						type=p_chunks[i][j][k].blocks[CHUNK_SIZE-1][l][m].getType();

						//See-through blocktype check
						if(type!=BlockType_Air)
						{
							//check 6DOF around block
							CheckEdges(Vector3<s64>(CHUNK_SIZE-1,l,m));
						}

						//Bottom Edges
						active=p_chunks[i][j][k].blocks[l][0][m].checkActive();
						type=p_chunks[i][j][k].blocks[l][0][m].getType();

						//See-through blocktype check
						if(type!=BlockType_Air)
						{
							//check 6DOF around block
							CheckEdges(Vector3<s64>(l,0,m));
						}

						//Top Edges
						active=p_chunks[i][j][k].blocks[l][CHUNK_SIZE-1][m].checkActive();
						type=p_chunks[i][j][k].blocks[l][CHUNK_SIZE-1][m].getType();

						//See-through blocktype check
						if(type!=BlockType_Air)
						{
							//check 6DOF around block
							CheckEdges(Vector3<s64>(l,CHUNK_SIZE-1,m));
						}
					}	
				}
			}
		}
	}
}

/****************************
* CheckEdges - 	This function only checks the immediate blocks surrounding the
				passed position to ensure valid data. To be set as active, the
				blocks must have a transparent block on one side.
****************************/
void ChunkManager::CheckEdges(const Vector3<s64>& pos)
{
	s32 i(0),j(0),k(0);
	tBOOL active=FALSE;

	Vector3<s64> chunkPos(calculateChunkPos(pos));
	Vector3<s32> blockPos(calculateBlockPos(pos,chunkPos));

	if(blockPos.x<0){
		chunkPos.x-=10;
		blockPos.x+=10;}
	if(blockPos.y<0){
		chunkPos.y-=10;
		blockPos.y+=10;}
	if(blockPos.z<0){
		chunkPos.z-=10;
		blockPos.z+=10;}

	//makes sure the passed value is an edge or doesn't do anything
	if((blockPos.x==0 || blockPos.x==9)||(blockPos.y==0 || blockPos.y==9)||(blockPos.z==0||blockPos.z==9))
	{
		std::string chunkKey(uiCore->intToString(chunkPos.x) + uiCore->intToString(chunkPos.y) + uiCore->intToString(chunkPos.z));
		Chunk* it=chunkMap[chunkKey];

		if(it)
		{
			if(it->CheckType(Vector3<s32>(blockPos.x,blockPos.y,blockPos.z))==BlockType_Air)
				active=true;
		}
		else
		{
			logger->LogMessage(LOG_CHUNK,"Error with chunk hashmap. Cannot find chunk at key " + chunkKey);
		}

		//retrieve chunk for the given position
		for(i=0;i<RENDER_DIST;i++)
		{
			for(j=0;j<HEIGHT_CHUNK;j++)
			{
				for(k=0;k<RENDER_DIST;k++)
				{
					if(p_chunks[i][j][k].pos==pos)
						break;
				}
			}
		}
		
		if(p_chunks[i][j][k].blocks[blockPos.x-1][blockPos.y][blockPos.z].checkActive())
		{
			if(p_chunks[i][j][k].blocks[blockPos.x+1][blockPos.y][blockPos.z].checkActive())
			{
				if(p_chunks[i][j][k].blocks[blockPos.x][blockPos.y-1][blockPos.z].checkActive())
				{
					if(p_chunks[i][j][k].blocks[blockPos.x][blockPos.y+1][blockPos.z].checkActive())
					{
						if(p_chunks[i][j][k].blocks[blockPos.x][blockPos.y][blockPos.z-1].checkActive())
						{
							if(p_chunks[i][j][k].blocks[blockPos.x][blockPos.y][blockPos.z+1].checkActive())
							{
								active=false;
							}
							else 
							{
								active=true;
							}
						}
						else //5th block inactive
						{
							if((p_chunks[i][j][k].blocks[blockPos.x][blockPos.y][blockPos.z+1].getType()==BlockType_Air))
							{
								if(p_chunks[i][j][k].blocks[blockPos.x][blockPos.y][blockPos.z+1].checkActive())
								{
									active=false;
								}
								else
								{
									active=true;
								}
							}
							else
							{
								active=TRUE;
							}
						}
					}
					else //4th block inactive
					{
						active=TRUE;
					}
				}
				else //3rd block inactive
				{
					active=TRUE;
				}
			}
			else //2nd block inactive
			{
				active=TRUE;
			}
		}
		else //1st block inactive
		{
			//Further tests for inactive blocks - any fail and the block needs to be active
			if(!p_chunks[i][j][k].blocks[blockPos.x+1][blockPos.y][blockPos.z].checkActive())
			{
				if(!p_chunks[i][j][k].blocks[blockPos.x][blockPos.y-1][blockPos.z].checkActive())
				{
					if(!p_chunks[i][j][k].blocks[blockPos.x][blockPos.y+1][blockPos.z].checkActive())
					{
						if(!p_chunks[i][j][k].blocks[blockPos.x][blockPos.y][blockPos.z-1].checkActive())
						{
							//checks if the last check is positive. If it is inactive, the default value of FALSE is held
							if(p_chunks[i][j][k].blocks[blockPos.x][blockPos.y][blockPos.z+1].checkActive())
							{
								active=TRUE;
							}
						}
						else
						{
							active=TRUE;
						}
					}
					else
					{
						active=TRUE;
					}
				}
				else
				{
					active=TRUE;
				}
			}
			else
			{
				active=TRUE;
			}
		}
	}
}

bool ChunkManager::CheckBlock(const Vector3<s64>& pos)
{
	//iff valid pos
	if(pos.y>=0)
	{
		Vector3<s64> chunkPos(calculateChunkPos(pos));
		Vector3<s32> blockPos(calculateBlockPos(pos,chunkPos));

		if(blockPos.x<0){
			chunkPos.x-=10;
			blockPos.x+=10;}
		if(blockPos.y<0){
			chunkPos.y-=10;
			blockPos.y+=10;}
		if(blockPos.z<0){
			chunkPos.z-=10;
			blockPos.z+=10;}


		std::string chunkKey(uiCore->intToString(chunkPos.x) + uiCore->intToString(chunkPos.y) + uiCore->intToString(chunkPos.z));

		hash_map<std::string,Chunk*>::iterator findIt;
		findIt=chunkMap.find(chunkKey);

		if(findIt!=chunkMap.end())
		{
			Chunk* it(NULL);
			it=chunkMap.find(chunkKey)->second;

			if(it)
			{
				if(it->CheckType(blockPos)==BlockType_Air)
					return false;
				else 
					return true;
			}
			else
				return false;
		}
		else
		{
			//logger->LogMessage(4,"Error with chunk hashmap. Cannot find chunk at key " + chunkKey);
			return false;
		}
	}
	else
		return false;
}

Block* ChunkManager::getReferenceToBlock(const Vector3<s64>& pos)
{
	Vector3<s64> chunkPos(calculateChunkPos(pos));
	Vector3<s32> blockPos(calculateBlockPos(pos,chunkPos));

	if(blockPos.x<0){
		chunkPos.x-=10;
		blockPos.x+=10;}
	if(blockPos.y<0){
		chunkPos.y-=10;
		blockPos.y+=10;}
	if(blockPos.z<0){
		chunkPos.z-=10;
		blockPos.z+=10;}

	std::string chunkKey(uiCore->intToString(chunkPos.x) + uiCore->intToString(chunkPos.y) + uiCore->intToString(chunkPos.z));

	hash_map<std::string,Chunk*>::iterator findIt;
	findIt=chunkMap.find(chunkKey);
	
	if(findIt!=chunkMap.end())
	{
		Chunk* it(NULL);
		it=findIt->second;

		return &it->blocks[blockPos.x][blockPos.y][blockPos.z];
	}
	else
		return NULL;

}

/****************************
* Check6Block -	Checks the 6 surrounding voxels. Returns true if block needs to be active
				and false otherwise.
****************************/
bool ChunkManager::Check6Block(const Vector3<s64>& pos)
{
	Vector3<s64>	zp(pos.x,pos.y,pos.z+1),zm(pos.x,pos.y,pos.z-1),yp(pos.x,pos.y+1,pos.z),
					ym(pos.x,pos.y-1,pos.z),xp(pos.x+1,pos.y,pos.z),xm(pos.x-1,pos.y,pos.z);

	if(CheckBlock(zp)){
		if(CheckBlock(zm)){
			if(CheckBlock(yp)){
				if(CheckBlock(ym)){
					if(CheckBlock(xp)){
						if(CheckBlock(xm))
							return false;
						else
							return true;
					}
					else
						return true;
				}
				else
					return true;
			}
			else
				return true;
		}
		else
			return true;
	}
	else
		return true;

}
/****************************
* ACull -	Active culling algorithm
****************************/
void ChunkManager::ACull()
{
	for(int i=0;i<RENDER_DIST;i++)
	{
		for(int j=0;j<HEIGHT_CHUNK;j++)
		{
			for(int k=0;k<RENDER_DIST;k++)
			{
				p_chunks[i][j][k].activeCulling();
			}
		}
	}
}

/****************************
* VCull -	View frustrum culling algorithm
****************************/
void ChunkManager::VCull()
{
	//variables to hold the matrices
	GLfloat proj[16];
	GLfloat model[16];

	glGetFloatv(GL_PROJECTION_MATRIX,proj);
	glGetFloatv(GL_MODELVIEW_MATRIX,model);

	//combine matrices
	glPushMatrix();
	glLoadMatrixf(proj);
	glMultMatrixf(model);
	glGetFloatv(GL_MODELVIEW_MATRIX,model);
	glPopMatrix();

	//calculate the planes
	//LEFT plane
	planes[0].x=model[3]+model[0];
	planes[0].y=model[7]+model[4];
	planes[0].z=model[11]+model[8];
	planes[0].s=model[15]+model[12];

	//RIGHT plane
	planes[1].x=model[3]-model[0];
	planes[1].y=model[7]-model[4];
	planes[1].z=model[11]-model[8];
	planes[1].s=model[15]-model[12];

	//BOTTOM plane
	planes[2].x=model[3]+model[1];
	planes[2].y=model[7]+model[5];
	planes[2].z=model[11]+model[9];
	planes[2].s=model[15]+model[13];

	//TOP plane
	planes[3].x=model[3]-model[1];
	planes[3].y=model[7]-model[5];
	planes[3].z=model[11]-model[9];
	planes[3].s=model[15]-model[13];

	//NEAR plane
	planes[4].x=model[3]+model[2];
	planes[4].y=model[7]+model[6];
	planes[4].z=model[11]+model[10];
	planes[4].s=model[15]+model[14];

	//FAR plane
	planes[5].x=model[3]-model[2];
	planes[5].y=model[7]-model[6];
	planes[5].z=model[11]-model[10];
	planes[5].s=model[15]-model[14];

	//normalizes all of the planes
	for(int i=0;i<6;i++)
	{
		planes[i].normalize();
	}

	//clears the render list
	while(!renderList.empty())
	{
		renderList.pop_back();
	}
	
	//perform frustrum checks on the chunks. This has to be done with bounding boxes around the chunks
	for(int i=0;i<RENDER_DIST;i++)
	{
		for(int j=0;j<HEIGHT_CHUNK;j++)
		{
			for(int k=0;k<RENDER_DIST;k++)
			{
				tBOOL add=TRUE;
				s32 x,y,z,size;
				
				//changes the position to the center of the chunk
				x=p_chunks[i][j][k].pos.x+(CHUNK_SIZE/2);
				y=p_chunks[i][j][k].pos.y+(CHUNK_SIZE/2);
				z=p_chunks[i][j][k].pos.z+(CHUNK_SIZE/2);

				//calculates the size of the chunk from the center
				size=CHUNK_SIZE/2;

				//checks against the bounding box of the chunk
				for(int p = 0; p < 6; p++ )
				{
					if( planes[p].x * (x - size) + planes[p].y * (y - size) + planes[p].z * (z - size) + planes[p].s >= 0 )
						continue;
					if( planes[p].x * (x + size) + planes[p].y * (y - size) + planes[p].z * (z - size) + planes[p].s >= 0 )
						continue;
					if( planes[p].x * (x - size) + planes[p].y * (y + size) + planes[p].z * (z - size) + planes[p].s >= 0 )
						continue;
					if( planes[p].x * (x + size) + planes[p].y * (y + size) + planes[p].z * (z - size) + planes[p].s >= 0 )
						continue;
					if( planes[p].x * (x - size) + planes[p].y * (y - size) + planes[p].z * (z + size) + planes[p].s >= 0 )
						continue;
					if( planes[p].x * (x + size) + planes[p].y * (y - size) + planes[p].z * (z + size) + planes[p].s >= 0 )
						continue;
					if( planes[p].x * (x - size) + planes[p].y * (y + size) + planes[p].z * (z + size) + planes[p].s >= 0 )
						continue;
					if( planes[p].x * (x + size) + planes[p].y * (y + size) + planes[p].z * (z + size) + planes[p].s >= 0 )
						continue;
					add=FALSE;
				}

				if(add==TRUE)		//chunk is at least partially within view, more detailed culling may be needed
				{
					//calls the visibility culling method for the chunk
					p_chunks[i][j][k].visibleCulling();
					renderList.push_back(&p_chunks[i][j][k]);
				}
			}
		}
	}	
}

tBOOL ChunkManager::FrustrumCheck(Vector3<s64> pos)
{

	return FALSE;
}

void ChunkManager::rayTrace(const Ray<real32>& ray,RayCol<Block>& rayBlock)
{
	s32 face(0);
	real32 tmin(100000),store;

	//Iterates through the chunks that are visible and stores the smallest distance collision
	for(std::vector<Chunk*>::iterator it(renderList.begin());it!=renderList.end();it++)
	{
		if((*it)->rayTrace(ray)>=-20)
		{
			store=(*it)->blockRayTrace(ray,face,rayBlock);
			if(store>=0 && store<tmin)
				tmin=store;
		}
	}

	if(tmin!=100000)
	{
		tempRay.setPos(rayBlock.blockCol->getPos() + Vector3<s32>(0,1,0)+Vector3<s32>(rayBlock.sOffset.x,rayBlock.sOffset.y,rayBlock.sOffset.z));
		tempRay.setType(BlockType_MARKER);
	}
}


Vector3<s64> ChunkManager::calculateChunkPos(const Vector3<s64> &pos)
{
	Vector3<s64> chunkPos(((s32)pos.x/10)*10,((s32)pos.y/10)*10,((s32)pos.z/10)*10);
	return chunkPos;
}

Vector3<s32> ChunkManager::calculateBlockPos(const Vector3<s64> &pos, const Vector3<s64> &chunkPos)
{
	Vector3<s32> blockPos(pos.x-chunkPos.x,pos.y-chunkPos.y,pos.z-chunkPos.z);
	return blockPos;
}