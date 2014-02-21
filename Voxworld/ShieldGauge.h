#pragma once
#include "uielement.h"
class GFXElement;
class TextElement;


class ShieldGauge :
	public UIElement
{
public:
	ShieldGauge();
	~ShieldGauge(void);
	virtual void init();
	virtual void update(float p_DeltaTimeS);
	virtual void render(Renderer * p_Renderer);
	void setCurrent(float p_Current){m_CurrentValue = p_Current;}
	void setMax(float p_Max){m_MaxValue = p_Max;}
private:
	
	float m_ScaleFactor;	
	float m_CurrentValue;
	float m_MaxValue;
	TTF_Font* font;
	TextElement* m_ShieldValue;
	GFXElement* m_ShieldImage;
	GFXElement* m_TankImage;
	TextureNode* m_ShieldFull;
	TextureNode* m_ShieldDamaged;
	
};



