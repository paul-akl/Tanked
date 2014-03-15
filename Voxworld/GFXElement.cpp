#include "GFXElement.h"


GFXElement::GFXElement(void)
{
	setPosition(glm::vec3(0));
	setScale(glm::vec3(1.0f));
	setOrientation(0);
}


GFXElement::~GFXElement(void)
{
}

void GFXElement::init()
{

}

void GFXElement::update(float p_DeltaTimeS)
{
	UIElement::update(p_DeltaTimeS);
}

void GFXElement::render(Renderer* p_Renderer)
{
	UIElement::render(p_Renderer);
}