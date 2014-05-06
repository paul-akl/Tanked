#include "LightShader.h"


LightShader::LightShader(void)
{
	m_cameraPosition	= 0;
	m_screenSize		= 0;
	m_positionBuffer	= 0;
	m_diffuseBuffer		= 0;
	m_normalBuffer		= 0;
	m_numPointLights	= 0;
	m_numSpotLights		= 0;
	m_pointLightBlockIndex	= 0;
	m_spotLightBlockIndex	= 0;
	m_pointLightBlockSize	= 0;
	m_spotLightBlockSize	= 0;
}

LightShader::~LightShader(void)
{

}

void LightShader::getDataHandles(GLuint p_programHandle)
{
	m_cameraPosition	= glGetUniformLocation(p_programHandle, "cameraPos");
	m_screenSize		= glGetUniformLocation(p_programHandle, "screenSize");
	m_numPointLights	= glGetUniformLocation(p_programHandle, "numPointLights");
	m_numSpotLights		= glGetUniformLocation(p_programHandle, "numSpotLights");
	m_positionBuffer	= glGetUniformLocation(p_programHandle, "positionMap");
	m_diffuseBuffer		= glGetUniformLocation(p_programHandle, "diffuseMap");
	m_normalBuffer		= glGetUniformLocation(p_programHandle, "normalMap");
	m_emissiveBuffer	= glGetUniformLocation(p_programHandle, "emissiveMap");

	m_pointLightBlockIndex = glGetUniformBlockIndex(p_programHandle, "PointLights");
	glGetActiveUniformBlockiv(p_programHandle, m_pointLightBlockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &m_pointLightBlockSize);
	
	m_spotLightBlockIndex = glGetUniformBlockIndex(p_programHandle, "SpotLights");
	glGetActiveUniformBlockiv(p_programHandle, m_spotLightBlockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &m_spotLightBlockSize);
}