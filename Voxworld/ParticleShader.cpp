#include "ParticleShader.h"

ParticleShader::ParticleShader(void)
{
	m_ShaderLocation = 0;
}

ParticleShader::~ParticleShader(void)
{

}

void ParticleShader::getDataHandles(GLuint p_programHandle)
{
	m_emissiveBuffer	= glGetUniformLocation(p_programHandle, "emissiveBuffer");
	m_finalBuffer		= glGetUniformLocation(p_programHandle, "finalBuffer");
	m_colour			= glGetUniformLocation(p_programHandle, "colour");
}
