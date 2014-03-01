#pragma once
#include "uielement.h"
class GFXElement;
class TextElement;


class FrameCounter :
	public UIElement
{
public:
	FrameCounter();
	~FrameCounter(void);
	virtual void init();
	virtual void update(float p_DeltaTimeS);
	virtual void render(Renderer * p_Renderer);
	void setCurrent(float p_Current){m_CurrentValue = p_Current;}
	
private:	
	float m_CurrentValue;
	TTF_Font* font;
	TextElement* m_FrameCounter;
	float currentTime;
	float deltaTime;
	float lastTime;

};