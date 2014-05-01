#include "ParticleShader.h"

ParticleShader::ParticleShader(void)
{
	m_ShaderLocation = 0;
	m_emissiveBuffer = 0;
	m_finalBuffer = 0;
	m_maxLifeTime = 0;
	m_colour = 0;
	m_MVP = 0;
}

ParticleShader::~ParticleShader(void)
{

}

void ParticleShader::getDataHandles(GLuint p_programHandle)
{
	m_emissiveBuffer	= glGetUniformLocation(p_programHandle, "emissiveBuffer");
	m_finalBuffer		= glGetUniformLocation(p_programHandle, "finalBuffer");
	m_maxLifeTime		= glGetUniformLocation(p_programHandle, "maxLifeTime");
	m_colour			= glGetUniformLocation(p_programHandle, "colour");
	m_MVP				= glGetUniformLocation(p_programHandle, "MVP");
}
