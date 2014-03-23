#include "PointLightShader.h"
#include <glm/gtc/type_ptr.hpp>

PointLightShader::PointLightShader(void)
{

}

void PointLightShader::setModelMatrix(glm::mat4& p_ModelMatrix)
{
	glUniformMatrix4fv(m_modelLocation, 1, GL_FALSE, glm::value_ptr(p_ModelMatrix)); 
}
void PointLightShader::setWorldPosition(glm::vec3 p_worldPosition)
{
	glUniform3f(m_worldPosition, p_worldPosition.x, p_worldPosition.y, p_worldPosition.z);
}
void PointLightShader::setColour(glm::vec3 p_colour)
{
	glUniform3f(m_colour, p_colour.x, p_colour.y, p_colour.z);
}
void PointLightShader::setAttenConstant(float p_attenConstant)
{
	glUniform1f(m_attenConstant, p_attenConstant);
}
void PointLightShader::setAttenLinear(float p_attenLinear)
{
	glUniform1f(m_attenLinear, p_attenLinear);
}
void PointLightShader::setAttenQuadratic(float p_attenQuadratic)
{
	glUniform1f(m_attenQuadratic, p_attenQuadratic);
}
void PointLightShader::setAmbientIntensity(float p_ambientI)
{
	glUniform1f(m_ambientIntensity, p_ambientI);
}
void PointLightShader::setDiffuseIntensity(float p_diffuseI)
{
	glUniform1f(m_diffuseIntensity, p_diffuseI);
}
void PointLightShader::setSpecularIntensity(float p_specI)
{
	glUniform1f(m_specularIntensity, p_specI);
}
void PointLightShader::setSpecularPower(float p_specP)
{
	glUniform1f(m_specularPower, p_specP);
}
void PointLightShader::setCameraWorldPosition(glm::vec3 p_cameraWorldPosition)
{
	glUniform3f(m_cameraPosition, p_cameraWorldPosition.x, p_cameraWorldPosition.y, p_cameraWorldPosition.z);
}
void PointLightShader::setScreenSize(glm::vec2 screenSize)
{
	glUniform2f(m_screenSize, screenSize.x, screenSize.y);
}

void PointLightShader::bindPositionBuffer(GLuint p_textureHandle)
{
	glUniform1i(m_positionBuffer, p_textureHandle);
}
void PointLightShader::bindDiffuseBuffer(GLuint p_textureHandle)
{
	glUniform1i(m_diffuseBuffer, p_textureHandle);
}
void PointLightShader::bindNormalBuffer(GLuint p_textureHandle)
{
	glUniform1i(m_normalBuffer, p_textureHandle);
}

void PointLightShader::getDataHandles(GLuint p_programHandle)
{
	m_modelLocation		= glGetUniformLocation(p_programHandle, "modelMatrix");
	m_worldPosition		= glGetUniformLocation(p_programHandle, "light.position");
	m_colour			= glGetUniformLocation(p_programHandle, "light.base.color");
	m_attenConstant		= glGetUniformLocation(p_programHandle, "light.atten.constant");
	m_attenLinear		= glGetUniformLocation(p_programHandle, "light.atten.linear");
	m_attenQuadratic	= glGetUniformLocation(p_programHandle, "light.atten.quad");
	m_ambientIntensity	= glGetUniformLocation(p_programHandle, "light.base.ambientIntensity");
	m_diffuseIntensity  = glGetUniformLocation(p_programHandle, "light.base.diffuseIntensity");
	m_specularIntensity = glGetUniformLocation(p_programHandle, "specularIntensity");
	m_specularPower		= glGetUniformLocation(p_programHandle, "specularPower");

	m_cameraPosition	= glGetUniformLocation(p_programHandle, "cameraPos");
	m_screenSize		= glGetUniformLocation(p_programHandle, "screenSize");

	m_positionBuffer	= glGetUniformLocation(p_programHandle, "positionMap");
	m_diffuseBuffer		= glGetUniformLocation(p_programHandle, "diffuseMap");
	m_normalBuffer		= glGetUniformLocation(p_programHandle, "normalMap");
}

PointLightShader::~PointLightShader(void)
{

}
