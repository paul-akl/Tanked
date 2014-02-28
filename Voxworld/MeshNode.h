#pragma once

#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include "scenenode.h"

enum BufferType
{
	BUFFER_VERTEX=0,
	BUFFER_COLOUR,
	BUFFER_NORMAL,
	BUFFER_TEXTURE_COORD,
	BUFFER_TANGENT,
	BUFFER_BITANGENT,
	BUFFER_INDEX,
};
struct face
{
	GLuint m_VertIndex[3];
	GLuint m_UVIndex[3];
	GLuint m_NormalIndex[3];
	face(GLuint p_VertIndex1,GLuint p_VertIndex2,GLuint p_VertIndex3,GLuint p_UVIndex1,GLuint p_UVIndex2,GLuint p_UVIndex3
		,GLuint p_NormIndex1,GLuint p_NormIndex2,GLuint p_NormIndex3)
	{
		m_VertIndex[0] = p_VertIndex1;
		m_VertIndex[1] = p_VertIndex2;
		m_VertIndex[2] = p_VertIndex3;
		m_UVIndex[0] = p_UVIndex1;
		m_UVIndex[1] = p_UVIndex2;
		m_UVIndex[2] = p_UVIndex3;
		m_NormalIndex[0] = p_NormIndex1;
		m_NormalIndex[1] = p_NormIndex2;
		m_NormalIndex[2] = p_NormIndex3;
	}
};
class MeshNode :
	public SceneNode
{
public:
	MeshNode(void);
	//throws exception
	void loadModel(std::string p_FileName);
	void loadQuad(int p_numTiles);
	GLuint getNumVerts(void){return m_NumVerts;}
	GLuint getMeshLocation(void){return m_MeshLocation;}
	void clearModelData(void);
	void deleteModel(void);
	virtual void render(Renderer* p_Renderer);
	virtual void update(float p_DeltaTimeMS){;}
	virtual ~MeshNode(void);
protected:
	void generateTangents(std::vector<face*>& p_Faces,std::vector<glm::vec2>& p_UVs, std::vector<glm::vec3> p_Normals);
	void loadFromScene(const aiScene *p_assimpScene);
	void loadFromMesh(const aiMesh* p_assimpMesh);
	void initialise(void);
	GLuint m_NumVerts;
	struct Mesh
	{
		Mesh()
		{
			m_NumIndices = 0;
			m_BaseVertex = 0;
			m_BaseIndex = 0;
			m_MaterialIndex = 0;
		}
			
		int m_NumIndices;
		int m_BaseVertex;
		int m_BaseIndex;
		int m_MaterialIndex;
	};
	std::vector<Mesh> m_MeshPool;
	std::vector<glm::vec3> m_Vertices;
	std::vector<glm::vec3> m_Normals;
	std::vector<glm::vec2> m_TextureCoordinates;
	std::vector<glm::vec3> m_Tangents;
	std::vector<glm::vec3> m_Bitangents;
	std::vector<GLuint> m_Indices;
	GLuint m_VBOData[7];
	GLuint m_MeshLocation;
};

