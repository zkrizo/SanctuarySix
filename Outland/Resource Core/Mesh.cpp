#include "Mesh.h"
#include <assimp\postprocess.h>
#include <assert.h>

Mesh::MeshEntry::MeshEntry()
{
    VB = 0;
    IB = 0;
    NumIndices  = 0;
    MaterialIndex = INVALID_MATERIAL;
};

Mesh::MeshEntry::~MeshEntry()
{
    if (VB != 0)
    {
        glDeleteBuffers(1, &VB);
    }

    if (IB != 0)
    {
        glDeleteBuffers(1, &IB);
    }
}

void Mesh::MeshEntry::Init(const std::vector<Vertex>& Vertices,
                          const std::vector<unsigned int>& Indices)
{
    NumIndices = Indices.size();

    glGenBuffers(1, &VB);
  	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * NumIndices, &Indices[0], GL_STATIC_DRAW);
}

/****************************
* Default Constructor
****************************/
Mesh::Mesh()
{
}

/****************************
* Deconstructor
****************************/
Mesh::~Mesh()
{
	//Clear();
}

void Mesh::Clear()
{
	for(unsigned int i(0);i<m_Textures.size();i++){
		if(m_Textures[i])
		{
			delete m_Textures[i];
			m_Textures[i]=NULL;
		}
	}
}


bool Mesh::LoadMesh(const std::string& file)
{
	//const char* error=Importer.GetErrorString();
	Clear();
	bool Ret(false);
	Assimp::Importer Importer;
	
	const aiScene* pScene(Importer.ReadFile(file,aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs));
	if(pScene)
	{
		Ret=InitFromScene(pScene,file);
	}
	else
	{

	}

	return Ret;
}

bool Mesh::LoadMesh(const std::string& file, GLuint* diffuse, GLuint* normals, GLuint* bump)
{
	Clear();
	bool Ret(false);
	Assimp::Importer Importer;
	
	const aiScene* pScene(Importer.ReadFile(file,aiProcess_Triangulate | aiProcess_GenSmoothNormals));

	if(pScene)
	{
		Ret=InitFromScene(pScene,file);
		if(diffuse)
			m_Textures[0]=new Texture(GL_TEXTURE_2D,file.c_str(),diffuse);
		if(normals)
			m_Textures[1]=new Texture(GL_TEXTURE_2D,file.c_str(),normals);
		if(bump)
			m_Textures[2]=new Texture(GL_TEXTURE_2D,file.c_str(),bump);

	}
	else
	{

	}

	return Ret;
}

bool Mesh::InitFromScene(const aiScene* pScene, const std::string& file)
{
	m_Entries.resize(pScene->mNumMeshes);
	m_Textures.resize(pScene->mNumMaterials);

	for(unsigned int i(0);i<m_Entries.size();i++)
	{
		const aiMesh* paiMesh(pScene->mMeshes[i]);
		InitMesh(i,paiMesh);
	}

	return InitMaterials(pScene,file);
}

void Mesh::InitMesh(unsigned int Index, const aiMesh* paiMesh)
{
	m_Entries[Index].MaterialIndex = paiMesh->mMaterialIndex;

	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;

	const aiVector3D Zero3D(0.0f,0.0f,0.0f);

	for (unsigned int i(0); i < paiMesh->mNumVertices; i++) {
        const aiVector3D* pPos = &(paiMesh->mVertices[i]);
        const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
        const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

        Vertex v(Vector3<float>(pPos->x, pPos->y, pPos->z),
                Vector2<float>(pTexCoord->x, pTexCoord->y),
                Vector3<float>(pNormal->x, pNormal->y, pNormal->z));

        Vertices.push_back(v);
    }

	for(unsigned int i(0);i<paiMesh->mNumFaces;i++){
		const aiFace& Face(paiMesh->mFaces[i]);
		assert(Face.mNumIndices==3);
		Indices.push_back(Face.mIndices[0]);
		Indices.push_back(Face.mIndices[1]);
		Indices.push_back(Face.mIndices[2]);
	}

	m_Entries[Index].Init(Vertices,Indices);
}

bool Mesh::InitMaterials(const aiScene* pScene, const std::string& file)
{
	std::string::size_type SlashIndex(file.find_last_of("/"));
	std::string Dir;

	if(SlashIndex == std::string::npos){
		Dir=".";
	}
	else if(SlashIndex==0){
		Dir="/";
	}
	else{
		Dir=file.substr(0,SlashIndex);
	}

	bool Ret(true);

	for(unsigned int i(0);i<pScene->mNumMaterials;i++){
		const aiMaterial* pMaterial(pScene->mMaterials[i]);

		m_Textures[i]=NULL;

		if(pMaterial->GetTextureCount(aiTextureType_DIFFUSE)>0){
			aiString Path;

			if(pMaterial->GetTexture(aiTextureType_DIFFUSE,0,&Path,NULL,NULL,NULL,NULL,NULL)==AI_SUCCESS){
				std::string FullPath(Dir+"/"+Path.data);
				//m_Textures[i]=new Texture(GL_TEXTURE_2D,FullPath.c_str());
				
				/*if(!m_Textures[i]->Load()){
					logger->LogMessage(3,"Error loading textures from mesh file");
					delete m_Textures[i];
					m_Textures[i]=NULL;
					Ret=false;
				}
				else{

				}*/
			}
		}

		if(!m_Textures[i]){
			//m_Textures[i]=new Texture(GL_TEXTURE_2D,"./white.png",0);

			//Ret = m_Textures[i]->Load();
		}
	}
	return Ret;
}

void Mesh::Render()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	for (unsigned int i = 0 ; i < m_Entries.size() ; i++) {
		glBindBuffer(GL_ARRAY_BUFFER, m_Entries[i].VB);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Entries[i].IB);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		const unsigned int MaterialIndex(m_Entries[i].MaterialIndex);
		if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex]) {
			glTexCoordPointer(2,GL_FLOAT,sizeof(Vertex),(const GLvoid*)12);
			m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
		}

		glDrawElements(GL_TRIANGLES, m_Entries[i].NumIndices, GL_UNSIGNED_INT, 0);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void Mesh::AddTexture(const GLuint* texture)
{
	if(texture)
		if(m_Textures.size()!=0)
			if(m_Textures[0]==NULL)
				m_Textures[0]=new Texture(GL_TEXTURE_2D,texture);
			else
				m_Textures.push_back(new Texture(GL_TEXTURE_2D,texture));
}