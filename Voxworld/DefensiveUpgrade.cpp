#include "DefensiveUpgrade.h"
#include "UpgradableTank.h"

DefensiveUpgrade::DefensiveUpgrade(void)
	:	m_HPBoost(0.0f),
		m_ShieldRechargeRate(0.0f),
		m_MaxHPModifier(0.0f)
{
}
void DefensiveUpgrade::Collect(UpgradableTank* p_Tank)
{
	p_Tank->addUpgrade(this);
	m_Collected = true;
}

DefensiveUpgrade::~DefensiveUpgrade(void)
{
}
