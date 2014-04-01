#include "SpotLight.h"
#include "DeferredRenderer.h"

SpotLight::SpotLight(void)
{
}
void SpotLight::update(float p_DeltaTimeS)
{
	LightNode::update(p_DeltaTimeS);
	m_TempData.m_TransformedDirection = glm::vec3(m_WorldTransform*glm::vec4(m_Direction,0.0f));
	m_TempData.m_TransformedPosition = glm::vec3(m_WorldTransform*glm::vec4(m_Position,1.0f));
}
void SpotLight::render(Renderer* p_Renderer)
{
	p_Renderer->render(this);
}
void SpotLight::setTransform(glm::mat4& p_WorldTransform)
{
	m_TempData.m_TransformedDirection = glm::vec3(p_WorldTransform*glm::vec4(m_Direction,0.0f));
	m_TempData.m_TransformedPosition = glm::vec3(p_WorldTransform*glm::vec4(m_Position,1.0f));
}
SpotLight::~SpotLight(void)
{
}
