#pragma once
#include "shader.h"
class PointLightShader :
	public Shader
{
public:
	PointLightShader(void);

	virtual void setModelMatrix(glm::mat4& p_ModelMatrix);
	virtual void setWorldPosition(glm::vec3 p_worldPosition);
	virtual void setColour(glm::vec3 p_colour);
	virtual void setAttenConstant(float p_attenConstant);
	virtual void setAttenLinear(float p_attenLinear);
	virtual void setAttenQuadratic(float p_attenQuadratic);
	virtual void setAmbientIntensity(float p_ambientI);
	virtual void setDiffuseIntensity(float p_diffuseI);
	virtual void setSpecularIntensity(float p_specI);
	virtual void setSpecularPower(float p_specP);
	virtual void setCameraWorldPosition(glm::vec3 p_cameraWorldPosition);
	virtual void setScreenSize(glm::vec2 screenSize);

	virtual void bindPositionBuffer(GLuint p_textureHandle);
	virtual void bindDiffuseBuffer(GLuint p_textureHandle);
	virtual void bindNormalBuffer(GLuint p_textureHandle);

	virtual ~PointLightShader(void);

protected:
	virtual void getDataHandles(GLuint p_programHandle);

	GLuint	m_worldPosition,
			m_colour,
			m_attenConstant,
			m_attenLinear,
			m_attenQuadratic,
			m_ambientIntensity,
			m_diffuseIntensity,
			m_specularIntensity,
			m_specularPower,
			m_cameraPosition,
			m_screenSize,
			m_positionBuffer,
			m_diffuseBuffer,
			m_normalBuffer;
};

