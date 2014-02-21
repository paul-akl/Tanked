#include "UIElement.h"

UIElement::UIElement()
{
	
}

UIElement::UIElement(std::string p_Image)
{
		
}


UIElement::~UIElement(void)
{

}

void UIElement::init()
{

}

void UIElement::update(float p_DeltaTimeS)
{
	m_LocalTransform->reset();
	m_LocalTransform->translate(m_Position);
	//m_LocalTransform->rotate(m_OrientationDeg, glm::vec3(0,0,1));
	m_LocalTransform->scale(glm::vec3(m_RenderRadius));
	SceneNode::update(p_DeltaTimeS);
}

void UIElement::render(Renderer * p_Renderer)
{
	SceneNode::render(p_Renderer);
}
