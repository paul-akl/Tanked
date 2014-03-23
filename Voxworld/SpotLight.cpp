#include "SpotLight.h"


SpotLight::SpotLight(void)
{
}

void SpotLight::setTransform(glm::mat4& p_WorldTransform)
{
	m_TempData.m_TransformedDirection = glm::vec3(p_WorldTransform*glm::vec4(m_Direction,0.0f));
	m_TempData.m_TransformedPosition = glm::vec3(p_WorldTransform*glm::vec4(m_Position,1.0f));
}
SpotLight::~SpotLight(void)
{
}
