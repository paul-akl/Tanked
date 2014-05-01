#pragma once
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
class ParticleShader :
	public Shader
{
public:
	ParticleShader(void);
	~ParticleShader(void);

	virtual void bindEmissiveBuffer(GLuint p_textureHandle) { glUniform1i(m_emissiveBuffer, p_textureHandle);	}
	virtual void bindFinalBuffer(GLuint p_textureHandle)	{ glUniform1i(m_finalBuffer, p_textureHandle);		}

	virtual void setColour(glm::vec4 p_colour)	{ glUniform4f(m_colour, p_colour.x, p_colour.y, p_colour.z, p_colour.w);	}
	virtual void setMVP(glm::mat4 p_MVP)		{ glUniformMatrix4fv(m_MVP, 1, GL_FALSE, glm::value_ptr(p_MVP));			}

protected:
	void getDataHandles(GLuint p_programHandle);

	GLuint	m_emissiveBuffer,
			m_finalBuffer,
			m_colour,
			m_MVP;
};

