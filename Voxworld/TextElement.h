#pragma once
#include "uielement.h"
class TextElement :
	public UIElement
{
public:
	TextElement(void);
	virtual ~TextElement(void);
	void setText(std::string text){m_CurrentText = text;}
	void setFont(TTF_Font * font){m_CurrentFont = font;}
	void setTextColour(glm::vec4 colour){m_Colour = colour;}
	virtual void init();
	virtual void update(float p_DeltaTimeS);
	virtual void render(Renderer * p_Renderer);
protected:
	std::string m_CurrentText;
	TTF_Font* m_CurrentFont;
	glm::vec4 m_Colour;
	bool needsUpdate;
};

