
#include "GLTestRender.h"
#include "MeshNode.h"
#include "GeneralUtils.h"

MeshNode::MeshNode(void)
{
}
void MeshNode::render(Renderer* p_Renderer)
{
	p_Renderer->render(this);
}
void MeshNode::loadModel(std::string p_FileName)
{
	Assimp::Importer v_assimpImporter;	// Handle for importing the model

	const aiScene* v_assimpScene = v_assimpImporter.ReadFile(p_FileName,				// Load the model from file
		aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_SortByPType);	// and set post-processing flags

	if(!v_assimpScene)	// Check if loading was successful
		throw new std::exception((p_FileName + " model failed to load (" + v_assimpImporter.GetErrorString() + ").").c_str());

	loadFromScene(v_assimpScene);	// Start model loading from the Assimp scene structure
	
	//and finally generate tangents
	//generateTangents(v_Faces,temp_uvs,temp_normals);
	initialise();
}
void MeshNode::loadQuad(int p_numTiles)
{
	std::string numTiles = "20";//Utils::toString(p_numTiles);
	std::string quadString = "\n\
v 1.000000 0.000001 -1.000000\n\
v -1.000000 0.000001 -1.000000\n\
v -1.000000 -0.000001 1.000000\n\
v 1.000000 -0.000001 1.000000\n\
vt " + numTiles + ".000000 " + numTiles + ".000000\n\
vt 0.000000 " + numTiles + ".000000\n\
vt 0.000000 0.000000\n\
vt " + numTiles + ".000000 0.000000\n\
vn 0.000000 1.000000 0.000000\n\
s off\n\
f 1/1/1 2/2/1 3/3/1\n\
f 4/4/1 1/1/1 3/3/1\n\
";
	/*std::string quadString = "\n\
v 1.000000 0.000001 -1.000000\n\
v -1.000000 0.000001 -1.000000\n\
v -1.000000 -0.000001 1.000000\n\
v 1.000000 -0.000001 1.000000\n\
vt 1.500000 1.500000\n\
vt 0.500000 1.500000\n\
vt 0.500000 0.500000\n\
vt 1.500000 0.500000\n\
vn -0.000000 1.000000 0.000001\n\
s off\n\
f 1/1/1 2/2/1 3/3/1\n\
f 4/4/1 1/1/1 3/3/1\n\
";*/

	Assimp::Importer v_assimpImporter;	// Handle for importing the model

	const aiScene* v_assimpScene = v_assimpImporter.ReadFileFromMemory(quadString.c_str(), quadString.size(),
		aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_SortByPType);

	if(!v_assimpScene)	// Check if loading was successful
	{
		std::string errorString = "Quad model failed to load (";
		throw new std::exception((errorString + v_assimpImporter.GetErrorString() + ").").c_str());
	}

	loadFromScene(v_assimpScene);	// Start model loading from the Assimp scene structure
	
	//and finally generate tangents
	//generateTangents(v_Faces,temp_uvs,temp_normals);
	initialise();
}
void MeshNode::loadFromScene(const aiScene *p_assimpScene)
{
	// Reserve space in the vectors for every mesh
	m_MeshPool.resize(p_assimpScene->mNumMeshes);

	unsigned int v_numVerticesTotal = 0;
	unsigned int v_numIndicesTotal = 0;

	// Count the number of vertices and indices
	for (unsigned int i=0 ; i < p_assimpScene->mNumMeshes ; i++)					// For each mesh in the model
	{       
	    m_MeshPool[i].m_MaterialIndex = p_assimpScene->mMeshes[i]->mMaterialIndex;	// Get index for a material
	    m_MeshPool[i].m_NumIndices = p_assimpScene->mMeshes[i]->mNumFaces * 3;		// Calculate the number of indices in the mesh
		    
	    m_MeshPool[i].m_BaseVertex = v_numVerticesTotal;	// Get base vertex and index, wich work similar as offset, when drawing from buffers
	    m_MeshPool[i].m_BaseIndex = v_numIndicesTotal;		

	    v_numVerticesTotal += p_assimpScene->mMeshes[i]->mNumVertices;	// Calculate the total number of vertices
	    v_numIndicesTotal  += m_MeshPool[i].m_NumIndices;				// and indices
	}

	m_NumVerts = v_numIndicesTotal;
		
	// Reserve space in the vectors, to slighly improve performance of '.push_back's
	m_Vertices.reserve(v_numVerticesTotal);
	m_Normals.reserve(v_numVerticesTotal);
	m_Tangents.reserve(v_numVerticesTotal);
	m_Bitangents.reserve(v_numVerticesTotal);
	m_TextureCoordinates.reserve(v_numVerticesTotal);
	m_Indices.reserve(v_numIndicesTotal);

	// Deal with each mesh
	for(unsigned int i=0; i < p_assimpScene->mNumMeshes; i++)
		loadFromMesh(p_assimpScene->mMeshes[i]);
}
void MeshNode::loadFromMesh(const aiMesh* p_assimpMesh)
{
	// Make sure that the texture coordinates array exist (by checking if the first member of the array does)
	bool v_textureCoordsExist = p_assimpMesh->mTextureCoords[0] ? true : false;

	// Put the vertex positions, normals, tangents, bitangents, texture coordinate data from Assimp data sets to our data sets (vectors)
	for(unsigned int i=0; i < p_assimpMesh->mNumVertices; i++)
	{
		m_Vertices.push_back(glm::vec3(p_assimpMesh->mVertices[i].x, p_assimpMesh->mVertices[i].y, p_assimpMesh->mVertices[i].z));
		m_Normals.push_back(glm::vec3(p_assimpMesh->mNormals[i].x, p_assimpMesh->mNormals[i].y, p_assimpMesh->mNormals[i].z));
		if(p_assimpMesh->mTangents != NULL)
			m_Tangents.push_back(glm::vec3(p_assimpMesh->mTangents[i].x, p_assimpMesh->mTangents[i].y, p_assimpMesh->mTangents[i].z));
		if(p_assimpMesh->mBitangents != NULL)
			m_Bitangents.push_back(glm::vec3(p_assimpMesh->mBitangents[i].x, p_assimpMesh->mBitangents[i].y, p_assimpMesh->mBitangents[i].z));
		if(v_textureCoordsExist)	// Proceed with texture coordinates only if they exsist
			m_TextureCoordinates.push_back(glm::vec2(p_assimpMesh->mTextureCoords[0][i].x, p_assimpMesh->mTextureCoords[0][i].y));
	}

	// Populate the indices (from Assimp to our vectors)
	for(unsigned int i=0; i < p_assimpMesh->mNumFaces; i++)
		if(p_assimpMesh->mFaces[i].mNumIndices == 3)
		{
			m_Indices.push_back(p_assimpMesh->mFaces[i].mIndices[0]);
			m_Indices.push_back(p_assimpMesh->mFaces[i].mIndices[1]);
			m_Indices.push_back(p_assimpMesh->mFaces[i].mIndices[2]);
		}
}
void MeshNode::initialise(void)
{
	//GLenum error = glGetError();

	//create and enable new vertex array object
	glGenVertexArrays(1,&m_MeshLocation);
	glBindVertexArray(m_MeshLocation);
	//create a VBO for the vertex data
	if(m_Vertices.empty())
	{
		//no verts
		return;
	}

	//error = glGetError();
	GLuint VBO;
	glGenBuffers(1,&VBO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,m_Indices.size()*sizeof(glm::vec3),m_Vertices.data(),GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)BUFFER_VERTEX,3,GL_FLOAT,GL_FALSE,0,0);
	glEnableVertexAttribArray(BUFFER_VERTEX);
	m_VBOData[BUFFER_VERTEX] = VBO;

	//create a VBO for normal data
	glGenBuffers(1,&VBO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,m_Indices.size()*sizeof(glm::vec3),m_Normals.data(),GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)BUFFER_NORMAL,3,GL_FLOAT,GL_FALSE,0,0);
	//error = glGetError();
	glEnableVertexAttribArray(BUFFER_NORMAL);
	//error = glGetError();
	m_VBOData[BUFFER_NORMAL] = VBO;

	//create a VBO for uv data
	glGenBuffers(1,&VBO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,m_Indices.size()*sizeof(float)*2,m_TextureCoordinates.data(),GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)BUFFER_TEXTURE_COORD,2,GL_FLOAT,GL_FALSE,0,0);
	glEnableVertexAttribArray(BUFFER_TEXTURE_COORD);
	m_VBOData[BUFFER_TEXTURE_COORD] = VBO;

	//create a VBO for tangent data
	glGenBuffers(1,&VBO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,m_Indices.size()*sizeof(glm::vec3),m_Tangents.data(),GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)BUFFER_TANGENT,3,GL_FLOAT,GL_FALSE,0,0);
	glEnableVertexAttribArray(BUFFER_TANGENT);
	m_VBOData[BUFFER_TANGENT] = VBO;
	
	//create a VBO for bitangent data
	glGenBuffers(1,&VBO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,m_Indices.size()*sizeof(glm::vec3),m_Bitangents.data(),GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)BUFFER_BITANGENT,3,GL_FLOAT,GL_FALSE,0,0);
	glEnableVertexAttribArray(BUFFER_BITANGENT);
	m_VBOData[BUFFER_BITANGENT] = VBO;

	//and finally a VBO for indices
	glGenBuffers(1,&VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(GLuint), m_Indices.data(), GL_STATIC_DRAW);
	m_VBOData[BUFFER_INDEX] = VBO;	

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void MeshNode::generateTangents(std::vector<face*>& p_Faces, std::vector<glm::vec2>& p_UVs, std::vector<glm::vec3> p_Normals)
{	
	//Original code from : 
	//Lengyel, Eric. “Computing Tangent Space Basis Vectors for an Arbitrary Mesh”. Terathon Software 3D Graphics Library, 2001. http://www.terathon.com/code/tangent.html
	std::vector<glm::vec3> v_CoTangents;
	std::vector<glm::vec3> v_Tangents;
	//generates tangent vectors for tangent space calculations(Bump mapping, normal mapping, parallax, relief mapping)
	for (size_t i = 0; i < p_Faces.size();i++)
	{
		//get specified vertices
		int v_Index1 = p_Faces[i]->m_VertIndex[0];
		int v_Index2 = p_Faces[i]->m_VertIndex[1];
		int v_Index3 = p_Faces[i]->m_VertIndex[2];
		glm::vec3 v1 = m_Vertices[v_Index1];
		glm::vec3 v2 = m_Vertices[v_Index2];
		glm::vec3 v3 = m_Vertices[v_Index3];
		//get specified UV coordinates
		int v_UVindex1 = p_Faces[i]->m_UVIndex[0];
		int v_UVindex2 = p_Faces[i]->m_UVIndex[1];
		int v_UVindex3 = p_Faces[i]->m_UVIndex[2];
		glm::vec2 UV1 = p_UVs[v_UVindex1];
		glm::vec2 UV2 = p_UVs[v_UVindex2];
		glm::vec2 UV3 = p_UVs[v_UVindex3];
		//get specified normals
		int v_NormalIndex1 = p_Faces[i]->m_NormalIndex[0];
		int v_NormalIndex2 = p_Faces[i]->m_NormalIndex[1];
		int v_NormalIndex3 = p_Faces[i]->m_NormalIndex[2];
		glm::vec3 n1 = p_Normals[v_NormalIndex1];
		glm::vec3 n2 = p_Normals[v_NormalIndex2];
		glm::vec3 n3 = p_Normals[v_NormalIndex3];
		//generate edges for the face
		float x1 = v2.x - v1.x;
        float x2 = v3.x - v1.x;
        float y1 = v2.y - v1.y;
        float y2 = v3.y - v1.y;
        float z1 = v2.z - v1.z;
        float z2 = v3.z - v1.z;
		//generate uv directions
		float s1 = UV2.x - UV1.x;
        float s2 = UV3.x - UV1.x;
        float t1 = UV2.y - UV1.y;
        float t2 = UV3.y - UV1.y;
		//get determinate of 2x2 matrix representing texture space
		float r = 1.0F / (s1 * t2 - s2 * t1);
		//and generate tangent space basic vectors
		glm::vec3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
                (t2 * z1 - t1 * z2) * r);
		glm::vec3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
                (s1 * z2 - s2 * z1) * r);
		//store tangents and cotangents temporarily
		v_Tangents.push_back(sdir);
		v_Tangents.push_back(sdir);
		v_Tangents.push_back(sdir);

		v_CoTangents.push_back(tdir);
		v_CoTangents.push_back(tdir);
		v_CoTangents.push_back(tdir);
	}
	for (size_t i = 0; i < m_Indices.size();i++)
	{
		glm::vec3 n = v_Tangents[i];
		glm::vec3 t = v_CoTangents[i];
		// Gram-Schmidt orthogonalize
		//m_Tangents.push_back(glm::normalize(glm::vec4 (t - n * glm::dot(n, t),0.0f )) );
		//calculate handedness of tangent space basis vector.
		//m_Tangents[i].w = (glm::dot(glm::cross(n,t),v_CoTangents[i]) ? -1.0f:1.0f);
	}
	//and clean up temporary data.
	v_Tangents.clear();
	v_CoTangents.clear();
}
void MeshNode::clearModelData(void)
{
	m_Vertices.clear();
	m_TextureCoordinates.clear();
	m_Normals.clear();
	m_Tangents.clear();
	m_Bitangents.clear();
	m_Indices.clear();
}
void MeshNode::deleteModel(void)
{
	glDeleteBuffersARB(1,&m_VBOData[BUFFER_VERTEX]);
	glDeleteBuffersARB(1,&m_VBOData[BUFFER_TEXTURE_COORD]);
	glDeleteBuffersARB(1,&m_VBOData[BUFFER_NORMAL]);
	glDeleteBuffersARB(1,&m_VBOData[BUFFER_TANGENT]);
	glDeleteBuffersARB(1,&m_VBOData[BUFFER_BITANGENT]);
	glDeleteBuffersARB(1,&m_VBOData[BUFFER_INDEX]);
	glDeleteVertexArrays(1,&m_MeshLocation);
}
MeshNode::~MeshNode(void)
{
	clearModelData();
	deleteModel();
}
