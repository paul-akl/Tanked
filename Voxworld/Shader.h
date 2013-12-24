#pragma once
#include <GL\glew.h>
#include <glm\glm.hpp>
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
	void initShader(char* p_VertexShader, char* p_PixelShader);
	void printShaderError(const int shader);
	virtual void setModelMatrix(glm::mat4& p_ModelMatrix);
	virtual void setModelView(glm::mat4& p_ModelViewMatrix);
	virtual void setProjectionMatrix(glm::mat4& p_ProjectionMatrix);
	virtual void setNormalMatrix(glm::mat3& p_InvTransposeModelViewMatrix);
	void enable();
	void disable();
	GLuint getShaderLocation(){return m_ShaderLocation;}
	virtual ~Shader(void);
private:
	GLuint loadShaders(char* p_VertexShader, char* p_PixelShader);
	char* loadFile(const char* p_FileName, int &p_FileSize);
	unsigned int m_ShaderLocation;
};

