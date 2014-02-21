#pragma once
#include "uielement.h"
class GFXElement;
class TextElement;

class WeaponGauge :
	public UIElement
{
public:
	WeaponGauge();
	~WeaponGauge(void);
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
	TextElement* m_Ammo;
	GFXElement* m_ChargeBg;
	GFXElement* m_ChargeFg;
	TextureNode* m_Gauge;
	TextureNode* m_GaugeBg;
	bool charging;
};




