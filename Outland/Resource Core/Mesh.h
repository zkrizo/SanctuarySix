#ifndef Mesh_H
#define Mesh_H

#include <Outland\UnitAbstraction.h>
#include <Outland\Vector.h>
#include <vector>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <gl\glew.h>
#include <gl\GLU.h>
#include "Texture.h"

struct Vertex
{
    Vector3<float> m_pos;
    Vector2<float> m_tex;
    Vector3<float> m_normal;

    Vertex() {}

    Vertex(const Vector3<float>& pos, const Vector2<float>& tex, const Vector3<float>& normal)
    {
        m_pos    = pos;
        m_tex    = tex;
        m_normal = normal;
    }
};

class Mesh
{
public:
	Mesh();
	~Mesh();
	void Render();
	bool LoadMesh(const std::string& file);
	bool LoadMesh(const std::string& file, GLuint* diffuse, GLuint* normals, GLuint* bump);
	void AddTexture(const GLuint* texture);

private:
	bool InitFromScene(const aiScene* pScene, const std::string& file);
	bool InitMaterials(const aiScene* pScene, const std::string& file);
	void InitMesh(unsigned int Index, const aiMesh* paiMesh);
	void Clear();

	#define INVALID_MATERIAL 0xFFFFFFFF

	struct MeshEntry {
		MeshEntry();

		~MeshEntry();

		void Init(const std::vector<Vertex>& Vertices,
		const std::vector<unsigned int>& Indices);

		GLuint VB;
		GLuint IB;
		unsigned int NumIndices;
		unsigned int MaterialIndex;
	};

    std::vector<MeshEntry> m_Entries;
    std::vector<Texture*> m_Textures;
};

#endif