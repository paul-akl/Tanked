#include "TextElement.h"
#include"GeneralUtils.h"

TextElement::TextElement(void)
{
	m_Colour = glm::vec4(0);
	m_CurrentFont = nullptr;
	needsUpdate = false;
	setPosition(glm::vec3(0));
	setRenderRadius(glm::vec3(1.0f));
	setOrientation(0);
	//m_Diffuse->setTexture(NULL);
}


TextElement::~TextElement(void)
{
}

void TextElement::init()
{

}

void TextElement::update(float p_DeltaTimeS)
{
	//if(needsUpdate)
	m_Diffuse->setTexture(Utils::GeneralUtils::textToTexture(m_Diffuse->getTexture(),m_CurrentText.c_str(), m_CurrentFont, m_Colour));
	UIElement::update(p_DeltaTimeS);
}

void TextElement::render(Renderer* p_Renderer)
{
	UIElement::render(p_Renderer);
}