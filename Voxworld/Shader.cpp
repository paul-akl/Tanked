#include "Shader.h"
#include <iostream>
#include <fstream>
#include <exception>
#include <glm/gtc/type_ptr.hpp>
Shader::Shader(void)
{
	m_ShaderLocation = 0;
}
void Shader::enable()
{
	glUseProgram(m_ShaderLocation);
	//fprintf(stderr, "Error: %s\n", glewGetErrorString(glGetError()));
}
void Shader::disable()
{
	glUseProgram(0);
}
void Shader::initShader(char* p_VertexShader, char* p_PixelShader)
{
	m_ShaderLocation = loadShaders(p_VertexShader,p_PixelShader);
	getDataHandles(m_ShaderLocation);
}
void Shader::printShaderError(const int shader)
{
	GLint maxLength = 0;
	GLint logLength = 0;
	char* logMessage;

	//find out how long message is
	if(!glIsShader(shader))
		glGetProgramiv(shader,GL_INFO_LOG_LENGTH,&maxLength);
	else
		glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&maxLength);

	if(maxLength>0)//if log message has some content
	{
		logMessage = new GLchar[maxLength];
		if(!glIsShader(shader))
			glGetProgramInfoLog(shader,maxLength,&logLength,logMessage);
		else
			glGetShaderInfoLog(shader,maxLength,&logLength,logMessage);

		printf("Shader Info Log: %s\n",logMessage);
		delete[] logMessage;
	}
	
}
void Shader::getDataHandles(GLuint p_programHandle)
{
	glBindAttribLocation(p_programHandle, RT3D_VERTEX,"VertexPosition");
	glBindAttribLocation(p_programHandle, RT3D_NORMAL,"normalDirection");
	glBindAttribLocation(p_programHandle, RT3D_TEXTURE_COORD,"textureCoordinates");

	m_modelViewLocation  = glGetUniformLocation(p_programHandle, "modelview");
	m_modelLocation		 = glGetUniformLocation(p_programHandle, "modelMatrix");
	m_normalLocation	 = glGetUniformLocation(p_programHandle, "normalMatrix");
	m_projectionLocation = glGetUniformLocation(p_programHandle, "projection");
	m_diffuseLocation	 = glGetUniformLocation(p_programHandle, "diffuseMap");
	m_normalMapLocation	 = glGetUniformLocation(p_programHandle, "normalMap");
	m_emissiveLocation	 = glGetUniformLocation(p_programHandle, "emissiveMap");
	m_heightLocation	 = glGetUniformLocation(p_programHandle, "heightMap");
}
char* Shader::loadFile(const char* p_FileName, int &p_FileSize)
{
	int size;
	char* memBlock;
	std::ifstream file(p_FileName, std::ios::in|std::ios::binary|std::ios::ate);
	if(file.is_open())
	{
		size = (int)file.tellg(); //get location of file point at end, which is the file size
		memBlock = new char[size];
		file.seekg(0,std::ios::beg);
		file.read(memBlock,size);
		file.close();
		printf("file: %s loaded\n",p_FileName);
		p_FileSize = (GLint)size;
	}
	else
	{
		printf("Unable to open file: %s\n",p_FileName);
		size=0;
		throw std::exception::exception("unable to open File\n");
	}
	return memBlock;
}
GLuint Shader::loadShaders(char* p_VertexShader,char* p_PixelShader)
{
	GLuint program=0;
	GLuint pixel=0;
	GLuint vert=0;
	char* vF=NULL;
	char* vS=NULL;
	vert = glCreateShader(GL_VERTEX_SHADER);
	pixel = glCreateShader(GL_FRAGMENT_SHADER);

	//load shaders and get length of each
	int vLength=0;
	int pLength=0;
	vS = loadFile(p_VertexShader,vLength);
	vF = loadFile(p_PixelShader,pLength);

	const char* VV = vS;
	const char* FF = vF;

	glShaderSource(vert,1,&VV,&vLength);
	glShaderSource(pixel,1,&FF,&pLength);

	int compiled=0;
	glCompileShader(vert);
	glGetShaderiv(vert,GL_COMPILE_STATUS,&compiled);
	if(!compiled)
	{
		printf("%s failed to compile\n",p_VertexShader);
		printShaderError(vert);
	}
	glCompileShader(pixel);
	glGetShaderiv(pixel,GL_COMPILE_STATUS,&compiled);
	if(!compiled)
	{
		printf("%s failed to compile\n",p_PixelShader);
		printShaderError(pixel);
	}
	program = glCreateProgram();
	glAttachShader(program,vert);
	glAttachShader(program,pixel);
	glLinkProgram(program);

	//clear up data
	delete[] vS;
	delete[] vF;
	printf("shaders compiled\n");

	return program;

}
void Shader::setModelMatrix(glm::mat4& p_ModelMatrix)
{
	glUniformMatrix4fv(m_modelLocation, 1, GL_FALSE, glm::value_ptr(p_ModelMatrix)); 
}
void Shader::setModelView(glm::mat4& p_ModelViewMatrix)
{
	glUniformMatrix4fv(m_modelViewLocation, 1, GL_FALSE, glm::value_ptr(p_ModelViewMatrix)); 
}
void Shader::setProjectionMatrix(glm::mat4& p_ProjectionMatrix)
{
	glUniformMatrix4fv(m_projectionLocation, 1, GL_FALSE, glm::value_ptr(p_ProjectionMatrix)); 
}
void Shader::setNormalMatrix(glm::mat3& p_InvTransposeModelViewMatrix)
{
	glUniformMatrix3fv(m_normalLocation, 1, GL_FALSE, glm::value_ptr(p_InvTransposeModelViewMatrix)); 
}
void Shader::bindTexture(TextureType p_type, GLuint p_textureHandle)
{
	glActiveTexture(GL_TEXTURE0 + p_type);
	glBindTexture(GL_TEXTURE_2D, p_textureHandle);
	switch(p_type)
	{
		case(DIFFUSE):
		glUniform1i(m_diffuseLocation, p_type);
		break;

		case(NORMAL):
		glUniform1i(m_normalMapLocation, p_type);
		break;

		case(EMISSIVE):
		glUniform1i(m_emissiveLocation, p_type);
		break;

		case(HEIGHT):
		glUniform1i(m_heightLocation, p_type);
		break;
	}
}
Shader::~Shader(void)
{
	glDeleteProgram(m_ShaderLocation);
}
