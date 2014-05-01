#include "FloorNode.h"
#include "GLTestRender.h"
#include "TransformNode.h"
FloorNode::FloorNode(void)
{

}
void FloorNode::render(Renderer* p_Renderer)
{
	SceneNode::render(p_Renderer);
}
void FloorNode::update(float p_DeltaTimeS)
{
	m_LocalTransform->reset();
	m_LocalTransform->translate(m_Position);
	m_LocalTransform->rotate(m_OrientationDeg,glm::vec3(0.0,1.0,0.0));
	m_LocalTransform->scale(glm::vec3(m_Size,1.0f,m_Size));
	SceneNode::update(p_DeltaTimeS);
}
FloorNode::~FloorNode(void)
{
}
