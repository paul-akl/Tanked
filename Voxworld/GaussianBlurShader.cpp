#include "GaussianBlurShader.h"

GaussianBlurShader::GaussianBlurShader(void) : Shader()
{
	m_blurOffset = 0;
}

GaussianBlurShader::~GaussianBlurShader(void)
{

}

void GaussianBlurShader::getDataHandles(GLuint p_programHandle)
{
	m_blurOffset  = glGetUniformLocation(p_programHandle, "blurOffset");
}
