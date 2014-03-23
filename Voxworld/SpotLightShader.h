#pragma once
#include "pointlightshader.h"
class SpotLightShader :
	public PointLightShader
{
public:
	SpotLightShader(void);
	
	virtual void setWorldDirection(glm::vec3 p_worldDirection);
	virtual void setCutoffAngle(float p_cutoffAngle);

	virtual ~SpotLightShader(void);

protected:
	virtual void getDataHandles(GLuint p_programHandle);

	GLuint	m_worldDirection,
			m_cutoffAngle;	
};

