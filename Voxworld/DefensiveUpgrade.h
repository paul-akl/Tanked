#pragma once
#include "upgradenode.h"
class DefensiveUpgrade :
	public UpgradeNode
{
public:
	DefensiveUpgrade(void);
	virtual void update(float p_DeltaTimeS)						{UpgradeNode::update(p_DeltaTimeS);}
	virtual void render(Renderer* p_Renderer)					{UpgradeNode::render(p_Renderer);}
	virtual void init()											{;}
	virtual void Collect(UpgradableTank* p_Tank);
	void setMaxHPModifier(float p_MaxHPModifier)				{m_MaxHPModifier=p_MaxHPModifier;}
	void setShieldRechargeRate(float p_ShieldRechargeRate)		{m_ShieldRechargeRate=p_ShieldRechargeRate;}
	void setHPBoost(float p_HPBoost)							{m_HPBoost=p_HPBoost;}
	float getMaxHPModifier()									{return m_MaxHPModifier;}
	float getShieldRechargeRate()								{return m_ShieldRechargeRate;}
	float getHPBoost()											{return m_HPBoost;}
	virtual ~DefensiveUpgrade(void);
protected:
	float m_HPBoost;
	float m_ShieldRechargeRate;
	float m_MaxHPModifier;
};

