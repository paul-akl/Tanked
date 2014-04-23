#pragma once
#include "shader.h"

class GaussianBlurShader : public Shader
{
public:
	GaussianBlurShader(void);
	~GaussianBlurShader(void);

	void setBlurOffset(float p_offset) { glUniform1f(m_blurOffset, p_offset); }

protected:
	virtual void getDataHandles(GLuint p_programHandle);

	GLuint m_blurOffset;
};

