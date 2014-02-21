#pragma once
#include "uielement.h"
class TextElement;

class Score :
	public UIElement
{
public:
	Score(void);
	virtual ~Score(void);
	virtual void init();
	virtual void update(float p_DeltaTimeS);
	virtual void render(Renderer * p_Renderer);
	void setCurrent(float p_Current){m_CurrentValue = p_Current;}
	//void setMax(float p_Max){m_MaxValue = p_Max;}
private:
			
	float m_CurrentValue;
	TTF_Font* font;
	TextElement* m_Score;
	
};

