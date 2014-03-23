#include "SpotLightShader.h"


SpotLightShader::SpotLightShader(void)
{
	
}

void SpotLightShader::setWorldDirection(glm::vec3 p_worldDirection)
{
	glUniform3f(m_worldDirection, p_worldDirection.x, p_worldDirection.y, p_worldDirection.z);
}
void SpotLightShader::setCutoffAngle(float p_cutoffAngle)
{
	glUniform1f(m_cutoffAngle, p_cutoffAngle);
}

void SpotLightShader::getDataHandles(GLuint p_programHandle)
{
	PointLightShader::getDataHandles(p_programHandle);

	m_worldDirection	= glGetUniformLocation(p_programHandle, "light.direction");
	m_cutoffAngle		= glGetUniformLocation(p_programHandle, "light.cutoff");
}

SpotLightShader::~SpotLightShader(void)
{
	
}
