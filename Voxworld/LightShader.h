#pragma once
#include "shader.h"
class LightShader : public Shader
{
public:
	LightShader(void);
	~LightShader(void);

	virtual int getPointLightBlockSize() { return m_pointLightBlockSize;		}
	virtual int getSpotLightBlockSize() { return m_spotLightBlockSize;			}
	virtual GLuint getPointLightBlockIndex() { return m_pointLightBlockIndex;	}
	virtual GLuint getSpotLightBlockIndex() { return m_spotLightBlockIndex;		}
	
	virtual void setNumPointLights(int p_numLights)							{ glUniform1i(m_numPointLights, p_numLights);					}
	virtual void setNumSpotLights(int p_numLights)							{ glUniform1i(m_numSpotLights, p_numLights);					}
	virtual void setScreenSize(glm::vec2 p_screenSize)						{ glUniform2f(m_screenSize, p_screenSize.x, p_screenSize.y);	}
	virtual void setCameraWorldPosition(glm::vec3 p_cameraWorldPosition)	{ glUniform3f(m_cameraPosition, p_cameraWorldPosition.x, p_cameraWorldPosition.y, p_cameraWorldPosition.z); }

	virtual void bindPositionBuffer(GLuint p_textureHandle)					{ glUniform1i(m_positionBuffer, p_textureHandle);	}
	virtual void bindDiffuseBuffer(GLuint p_textureHandle)					{ glUniform1i(m_diffuseBuffer, p_textureHandle);	}
	virtual void bindNormalBuffer(GLuint p_textureHandle)					{ glUniform1i(m_normalBuffer, p_textureHandle);		}
	virtual void bindEmissiveBuffer(GLuint p_textureHandle)					{ glUniform1i(m_emissiveBuffer, p_textureHandle);	}

protected:
	virtual void getDataHandles(GLuint p_programHandle);
	
	GLuint	m_cameraPosition,
			m_screenSize,
			m_positionBuffer,
			m_diffuseBuffer,
			m_normalBuffer,
			m_emissiveBuffer,
			m_numPointLights,
			m_numSpotLights,
			m_pointLightBlockIndex,
			m_spotLightBlockIndex;
	GLint	m_pointLightBlockSize,
			m_spotLightBlockSize;
};

