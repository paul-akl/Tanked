#include "LightNode.h"
#include "GLTestRender.h"
#include <glm/gtc/matrix_transform.hpp>
#include "TransformNode.h"

LightNode::LightNode(void)
{
	m_LocalTransform = new TransformNode();
}
void LightNode::update(float p_DeltaTimeS)
{
	//glm::mat4 transform(1.0f);
	//m_BaseData.m_LightModel.reset();
	//m_BaseData.m_LightModel.translate(m_Position+glm::vec3(0.0,5.0f,0.0));
	//m_BaseData.m_LightModel.rotate(m_OrientationDeg,glm::vec3(0.0,1.0f,0.0));
	//m_BaseData.m_LightModel.scale(glm::vec3(1.0f));

	m_LocalTransform->reset();
	m_LocalTransform->translate(m_Position);
	m_LocalTransform->scale(glm::vec3(100.0f));

	SceneNode::update(p_DeltaTimeS);

	m_BaseData.m_LightModel = m_WorldTransform;

	//m_BaseData.m_LightModel = glm::scale(m_BaseData.m_LightModel, glm::vec3(5000.0f));
	//m_BaseData.m_LightModel = m_Parent->getWorldTransform();
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
	delete m_LocalTransform;
}
