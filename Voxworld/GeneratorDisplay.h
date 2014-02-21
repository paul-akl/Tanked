#pragma once
#include "uielement.h"
class TextElement;

class GeneratorDisplay :
	public UIElement
{
public:
	GeneratorDisplay(void);
	virtual ~GeneratorDisplay(void);
	virtual void init();
	virtual void update(float p_DeltaTimeS);
	virtual void render(Renderer * p_Renderer);
	void setCurrent(float p_Current){m_CurrentValue = p_Current;}
	//void setMax(float p_Max){m_MaxValue = p_Max;}
private:
			
	float m_CurrentValue;
	TTF_Font* font;
	TextElement* m_GeneratorDisplay;
	
};

