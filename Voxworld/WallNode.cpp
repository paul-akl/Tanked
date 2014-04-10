#include "WallNode.h"
#include "TransformNode.h"

WallNode::WallNode(void)
{
	m_Radius = 0.0f;
	m_Width = 0.0f;
	m_Length = 0.0f;
}
void WallNode::render(Renderer* p_Renderer)
{
	SceneNode::render(p_Renderer);
}
void WallNode::update(float p_DeltaTimeS)
{
	m_LocalTransform->reset();
	m_LocalTransform->translate(m_Position+glm::vec3(0.0f,m_Radius,0.0f));
	m_LocalTransform->rotate(m_OrientationDeg,glm::vec3(0.0f,1.0f,0.0f));
	m_LocalTransform->scale(glm::vec3(m_Radius,m_Radius,m_Radius));
	SceneNode::update(p_DeltaTimeS);
}
WallNode::~WallNode(void)
{
}
