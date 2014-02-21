#include "UIWindow.h"
#include "MeshNode.h"
#include "TextureNode.h"
#include "TransformNode.h"

UIWindow::UIWindow(void)
{

}

UIWindow::~UIWindow(void)
{
}

void UIWindow::init()
{
	
}

void UIWindow::update(float p_DeltaTimeS)
{
	m_LocalTransform->reset();
	m_LocalTransform->translate(m_Position);
	m_LocalTransform->rotate(m_OrientationDeg, glm::vec3(0,0,1));
	m_LocalTransform->scale(glm::vec3(m_RenderRadius));
	SceneNode::update(p_DeltaTimeS);
}

void UIWindow::render(Renderer * p_Renderer)
{
	SceneNode::render(p_Renderer);
}