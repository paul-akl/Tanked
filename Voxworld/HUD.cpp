#include "HUD.h"
#include "Renderer.h"
#include "UIElement.h"
#include "ShieldGauge.h"
#include "WeaponGauge.h"
#include "Score.h"
#include "GeneratorDisplay.h"

HUD::HUD(void): UIWindow()
{
	//m_HudComponents = new HUDComponent[MAX_COMPONENTS];
	
}

HUD::HUD(std::string p_BgImage)
{
		
	
}


HUD::~HUD(void)
{
}

void HUD::init()
{
	m_LocalTransform = new TransformNode();
	m_LocalTransform->setName("hud transform");
	m_LocalTransform->reset();
	m_LocalTransform->setParent(this);
	
	m_Shield = new ShieldGauge();
	m_Shield->setName("shield gauge");
	m_Shield->setPosition(glm::vec3(-0.8,-0.8,0.0));
	m_Shield->setOrientation(0);
	m_Shield->setRenderRadius(glm::vec3(0.2f));
	m_Shield->init();	
	SceneNode::addNode(m_Shield);
	m_HudComponents.push_back(m_Shield);
	
	m_Weapon = new WeaponGauge();
	m_Weapon->setName("weapon gauge");
	m_Weapon->setPosition(glm::vec3(0.9,-0.6,0.0));
	m_Weapon->setOrientation(0);
	m_Weapon->setRenderRadius(glm::vec3(0.4,0.6,0.4));
	m_Weapon->init();
	SceneNode::addNode(m_Weapon);
	m_HudComponents.push_back(m_Weapon);

	m_Score = new Score();
	m_Score->setName("player score");
	m_Score->setPosition(glm::vec3(0.8,0.95,0.0));
	m_Score->setOrientation(0);
	m_Score->setRenderRadius(glm::vec3(0.1,0.03,1.0));
	m_Score->init();
	SceneNode::addNode(m_Score);
	m_HudComponents.push_back(m_Score);

	m_GeneratorDisplay= new GeneratorDisplay();
	m_GeneratorDisplay->setName("Generator count");
	m_GeneratorDisplay->setPosition(glm::vec3(-0.65,0.95,0.0));
	m_GeneratorDisplay->setOrientation(0);
	m_GeneratorDisplay->setRenderRadius(glm::vec3(0.3,0.03,1.0));
	m_GeneratorDisplay->init();
	SceneNode::addNode(m_GeneratorDisplay);
	m_HudComponents.push_back(m_GeneratorDisplay);
}

void HUD::setMetricCurrent(ElementType p_Type, float p_Value)
{
	switch(p_Type)
	{
		case(SHIELDGAUGE):
			{
				m_Shield->setCurrent(p_Value);
				break;
			}
		case(WEAPONGAUGE):
			{
				m_Weapon->setCurrent(p_Value);
				break;
			}		
		case(SCORE):
			{
				m_Score->setCurrent(p_Value);
				break;
			}
		case(GENERATORDISPLAY):
			{
				m_GeneratorDisplay->setCurrent(p_Value);
				break;
			}
		case(NOTIFICATION):
			{
				break;
			}
	};
}

void HUD::setMetricMax(ElementType p_Type, float p_Value)
{

	switch(p_Type)
	{
		case(SHIELDGAUGE):
			{
				m_Shield->setMax(p_Value);
				break;
			}
			
		case(WEAPONGAUGE):
			{
				m_Weapon->setMax(p_Value);
				break;
			}
		
		case(NOTIFICATION):
			{
				break;
			}
		};
}


void HUD::update(float p_DeltaTimeS)
{	
	UIWindow::update(p_DeltaTimeS);
	m_HudComponents[SHIELDGAUGE]->update(p_DeltaTimeS);
	m_HudComponents[WEAPONGAUGE]->update(p_DeltaTimeS);
	m_HudComponents[SCORE]->update(p_DeltaTimeS);
	m_HudComponents[GENERATORDISPLAY]->update(p_DeltaTimeS);
	SceneNode::update(p_DeltaTimeS);
}

void HUD::render(Renderer * p_Renderer)
{
	
	// the hud itself is not actually rendered so only its children call render
	p_Renderer->beginUIPhase();	
	
	for(int i = 0; i < m_HudComponents.size(); i++)
		m_HudComponents[i]->render(p_Renderer);
	
	p_Renderer->endRenderCycle();
}

