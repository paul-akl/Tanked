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
	glBindAttribLocation(program,RT3D_VERTEX,"VertexPosition");
	glBindAttribLocation(program,RT3D_NORMAL,"normalDirection");
	glBindAttribLocation(program,RT3D_TEXTURE_COORD,"textureCoordinates");
	glLinkProgram(program);

	//clear up data
	delete[] vS;
	delete[] vF;
	printf("shaders compiled\n");

	return program;

}
void Shader::setModelMatrix(glm::mat4& p_ModelMatrix)
{
	int uniformIndex = glGetUniformLocation(m_ShaderLocation, "modelMatrix");
	glUniformMatrix4fv(uniformIndex, 1, GL_FALSE, glm::value_ptr(p_ModelMatrix)); 
}
void Shader::setModelView(glm::mat4& p_ModelViewMatrix)
{
	int uniformIndex = glGetUniformLocation(m_ShaderLocation, "modelview");
	glUniformMatrix4fv(uniformIndex, 1, GL_FALSE, glm::value_ptr(p_ModelViewMatrix)); 
}
void Shader::setProjectionMatrix(glm::mat4& p_ProjectionMatrix)
{
	int uniformIndex = glGetUniformLocation(m_ShaderLocation, "projection");
	glUniformMatrix4fv(uniformIndex, 1, GL_FALSE, glm::value_ptr(p_ProjectionMatrix)); 
}
void Shader::setNormalMatrix(glm::mat3& p_InvTransposeModelViewMatrix)
{
	int uniformIndex = glGetUniformLocation(m_ShaderLocation, "normalMatrix");
	glUniformMatrix3fv(uniformIndex, 1, GL_FALSE, glm::value_ptr(p_InvTransposeModelViewMatrix)); 
}
Shader::~Shader(void)
{
	glDeleteProgram(m_ShaderLocation);
}
