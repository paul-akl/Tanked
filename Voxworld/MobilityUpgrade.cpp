#include "MobilityUpgrade.h"
#include "UpgradableTank.h"

MobilityUpgrade::MobilityUpgrade(void)	
	:	m_MassModifier(0.0f),
		m_ThrustModifier(0.0f),
		m_AeroModifier(0.0f)
{
}
void MobilityUpgrade::Collect(UpgradableTank* p_Tank)
{
	p_Tank->addUpgrade(this);
	m_Collected = true;
}

MobilityUpgrade::~MobilityUpgrade(void)
{
}
