#pragma once
#include "UIWindow.h"

class UIElement;
class ShieldGauge;
class WeaponGauge;
class Score;
class GeneratorDisplay;
class FrameCounter;

enum ElementType
{
	SHIELDGAUGE,
	WEAPONGAUGE,
	SCORE,
	GENERATORDISPLAY,
	FRAMECOUNTER,
	NOTIFICATION
};

class HUD : public UIWindow
{
public:
	HUD(void);
	HUD(std::string p_BgImage);
	~HUD(void);
	//UIElement * getElement(ElementType p_Type){return m_HudComponents[p_Type];}
	void setMetricCurrent(ElementType p_Type,float p_value);
	void setMetricMax(ElementType p_Type, float p_Value);
	virtual void init();
	virtual void update(float p_DeltaTimeS);
	virtual void render(Renderer * p_Renderer);
	
private:
	std::vector<UIElement* > m_HudComponents;
	ShieldGauge * m_Shield;
	WeaponGauge * m_Weapon;
	Score* m_Score;
	GeneratorDisplay* m_GeneratorDisplay;
	FrameCounter* m_FrameCounter;
};

