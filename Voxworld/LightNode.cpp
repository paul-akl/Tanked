#include "LightNode.h"
#include "GLTestRender.h"

LightNode::LightNode(void)
{
	
}
void LightNode::update(float p_DeltaTimeS)
{
	//SceneNode::update(p_DeltaTimeS);
	glm::mat4 transform(1.0f);
	m_BaseData.m_LightModel = m_Parent->getWorldTransform();
}
void LightNode::render(Renderer* p_Renderer)
{
	p_Renderer->render(this);
}
void LightNode::setAttenuation(float p_Constant, float p_Linear, float p_Quadratic)
{
	m_BaseData.m_Attenuation.m_Constant = p_Constant;
	m_BaseData.m_Attenuation.m_Linear = p_Linear;
	m_BaseData.m_Attenuation.m_Quadratic = p_Quadratic;
}
LightNode::~LightNode(void)
{
}
