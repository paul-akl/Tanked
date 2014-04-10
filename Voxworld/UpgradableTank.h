#pragma once
#include "testtanknode.h"
#include "MobilityUpgrade.h"
#include "DefensiveUpgrade.h"
#include <list>
class UpgradableTank :
	public TestTankNode
{
public:
	UpgradableTank(void);
	virtual void update(float p_DeltaTimeS);
	virtual void render(Renderer* p_Renderer);
	void addUpgrade(DefensiveUpgrade* p_Upgrade);
	void addUpgrade(MobilityUpgrade* p_Upgrade);
	virtual ~UpgradableTank(void);
protected:
	std::list<MobilityUpgrade*> m_MobilityUpgrades;
	std::list<DefensiveUpgrade*> m_DefensiveUpgrades;
	float m_BaseAeroConstant;
	float m_BaseShieldRechargeRate;
	float m_BaseShieldMaxHitPoints;
	float m_BaseMass;
	float m_BaseThrust;

};

