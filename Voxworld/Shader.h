#pragma once
#include <GL\glew.h>
#include <glm\glm.hpp>
#include "TextureNode.h"
enum AttributeType
{
	RT3D_VERTEX,
	RT3D_COLOUR,
	RT3D_NORMAL,
	RT3D_TEXTURE_COORD,
	RT3D_TANGENT,
	RT3D_INDEX
};
class Shader
{
public:
	Shader(void);
	virtual void initShader(char* p_VertexShader, char* p_PixelShader);
	void printShaderError(const int shader);
	virtual void setModelMatrix(glm::mat4& p_ModelMatrix);
	virtual void setModelView(glm::mat4& p_ModelViewMatrix);
	virtual void setProjectionMatrix(glm::mat4& p_ProjectionMatrix);
	virtual void setNormalMatrix(glm::mat3& p_InvTransposeModelViewMatrix);
	void enable();
	void disable();
	GLuint getShaderLocation(){return m_ShaderLocation;}
	virtual void bindTexture(TextureType p_type, GLuint p_textureHandle);
	virtual ~Shader(void);

protected:
	virtual void getDataHandles(GLuint p_programHandle);
	GLuint loadShaders(char* p_VertexShader, char* p_PixelShader);
	char* loadFile(const char* p_FileName, int &p_FileSize);
	GLuint m_ShaderLocation;

	GLuint	m_modelViewLocation,
			m_modelLocation,
			m_normalLocation,
			m_projectionLocation,
			m_diffuseLocation,
			m_normalMapLocation,
			m_emissiveLocation,
			m_heightLocation;
};

