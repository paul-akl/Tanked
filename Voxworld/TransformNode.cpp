#include "TransformNode.h"
#include <glm\gtc\matrix_transform.hpp>
#include "GLTestRender.h"

TransformNode::TransformNode(void)
{
	m_Matrix = glm::mat4(1.0f);
}
void TransformNode::reset(void)
{
	m_Matrix = glm::mat4(1.0);
}
void TransformNode::translate(const glm::vec3 p_Translation)
{
	m_Matrix = glm::translate(m_Matrix,p_Translation);
}
void TransformNode::rotate(const float p_Angle,const glm::vec3& p_Axis)
{
	m_Matrix = glm::rotate(m_Matrix,p_Angle,p_Axis);
}
void TransformNode::scale(const glm::vec3& p_Scale)
{
	m_Matrix = glm::scale(m_Matrix, p_Scale);
}
const glm::mat4& TransformNode::getLocalTransform(void)
{
	return m_Matrix;
}
const glm::mat4& TransformNode::getWorldTransform(void)
{
	return m_Parent->getWorldTransform();
}
void TransformNode::render(Renderer* p_Renderer)
{
	p_Renderer->render(this);
}
TransformNode::~TransformNode(void)
{
}
