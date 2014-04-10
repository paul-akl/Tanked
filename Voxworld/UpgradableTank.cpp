#include "UpgradableTank.h"
#include "MobilityUpgrade.h"
#include "DefensiveUpgrade.h"

UpgradableTank::UpgradableTank(void)
{
}
void UpgradableTank::addUpgrade(DefensiveUpgrade* p_Upgrade)
{
	m_DefensiveUpgrades.push_front(p_Upgrade);
	m_ShieldHitPoints+=m_ShieldMaxHitPoints*p_Upgrade->getHPBoost();
}
void UpgradableTank::addUpgrade(MobilityUpgrade* p_Upgrade)
{
	m_MobilityUpgrades.push_front(p_Upgrade);
}
void UpgradableTank::update(float p_DeltaTimeS)
{
	//record current state of tank
	m_BaseAeroConstant=m_TankAeroConstant;
	m_BaseShieldRechargeRate=m_ShieldChargeRate;
	m_BaseShieldMaxHitPoints=m_ShieldMaxHitPoints;
	m_BaseMass=m_Mass;
	m_BaseThrust=m_Thrust;
	//create some temporary data
	float v_AeroConstantMod=0.0f;
	float v_ShieldRechargeRateMod=0.0f;
	float v_ShieldMaxHitPointsMod=0.0f;
	float v_MassMod=0.0f;
	float v_ThrustMod=0.0f;

	//set modifiers
	std::list<MobilityUpgrade*>::iterator it = m_MobilityUpgrades.begin();
	std::list<DefensiveUpgrade*>::iterator jt = m_DefensiveUpgrades.begin();
	std::list<MobilityUpgrade*>::iterator kt = m_MobilityUpgrades.end();
	std::list<DefensiveUpgrade*>::iterator lt = m_DefensiveUpgrades.end();
	//calculate total modifiers, and remove expires bonus's
	for(;it!=kt;)
	{
		if(!(*it)->isActive())
		{
			it = m_MobilityUpgrades.erase(it);
		}
		else
		{
			(*it)->update(p_DeltaTimeS);
			v_AeroConstantMod*=(*it)->getAeroModifier();
			v_MassMod*=(*it)->getMassModifier();
			v_ThrustMod+=(*it)->getThrustModifier();
			it++;
		}
	}
	//apply modifiers to mobility attributes
	m_TankAeroConstant=v_AeroConstantMod*m_BaseAeroConstant;
	m_Mass=v_MassMod*m_BaseMass;
	v_ThrustMod*m_BaseThrust;
	//calculate total modifiers, and remove expires bonus's
	for(;jt!=lt;)
	{
		if(!(*jt)->isActive())
		{
			jt = m_DefensiveUpgrades.erase(jt);
		}
		else
		{
			(*jt)->update(p_DeltaTimeS);
			v_ShieldRechargeRateMod += (*jt)->getShieldRechargeRate();
			v_ShieldMaxHitPointsMod += (*jt)->getMaxHPModifier();
			jt++;
		}
	}
	//apply modifiers to defensive attributes
	m_ShieldMaxHitPoints =	(1.0f+v_ShieldMaxHitPointsMod) * m_BaseShieldMaxHitPoints;
	m_ShieldChargeRate = (1.0f+v_ShieldRechargeRateMod) * m_BaseShieldRechargeRate;
	//update tank
	TestTankNode::update(p_DeltaTimeS);
	//now change stats back
	m_TankAeroConstant=m_BaseAeroConstant;
	m_ShieldChargeRate=m_BaseShieldRechargeRate;
	m_ShieldMaxHitPoints=m_BaseShieldMaxHitPoints;
	m_Mass=m_BaseMass;
	m_Thrust=m_BaseThrust;
}
void UpgradableTank::render(Renderer* p_Renderer)
{
	TestTankNode::render(p_Renderer);
}

UpgradableTank::~UpgradableTank(void)
{
	m_MobilityUpgrades.clear();
	m_DefensiveUpgrades.clear();
}
