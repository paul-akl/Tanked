#pragma once
#include "upgradenode.h"
class MobilityUpgrade :
	public UpgradeNode
{
public:
	MobilityUpgrade(void);
	virtual void update(float p_DeltaTimeS)			{UpgradeNode::update(p_DeltaTimeS);}
	virtual void render(Renderer* p_Renderer)		{UpgradeNode::render(p_Renderer);}
	virtual void init()								{;}
	virtual void Collect(UpgradableTank* p_Tank);
	float getAeroModifier()							{return m_AeroModifier;}
	float getThrustModifier()						{return m_ThrustModifier;}
	float getMassModifier()							{return m_MassModifier;}
	float getBoostDuration()						{return m_BoostDurationS;}
	void setAeroModifier(float p_AeroModifier)		{ m_AeroModifier=p_AeroModifier;}
	void setThrustModifier(float p_ThrustModifier)	{ m_ThrustModifier=p_ThrustModifier;}
	void setMassModifier(float p_MassModifier)		{ m_MassModifier=p_MassModifier;}
	void setBoostDuration(float p_DurationS)		{m_BoostDurationS = p_DurationS;}
	virtual ~MobilityUpgrade(void);
protected:
	float m_MassModifier;
	float m_ThrustModifier;
	float m_AeroModifier;
	float m_BoostDurationS;
};

