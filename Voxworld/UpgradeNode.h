#pragma once
#include "collidablenode.h"
enum UpgradeType
{
	OFFENSIVE,
	DEFENSIVE,
	MOBILITY,
};
enum OffensiveUpgradeType
{
	DEFAULT, //default main gun, simple explosive shot
	AUTOGUN, //machine gun
	FIRE,  //impact leaves fire aoe effect
	GRAVITY, //impact leaves gravity well, which pulls in enemies
	CONCUSSION, //concussion blast on impact, pushing enemies away
	ICE //impact freezes enemies on the spot
};
enum DefensiveUpgradeType
{
	SHIELDHPBUFF, //temprarily increases maximum shield hit points
	SHIELDRECHARGE, //temporarily increases shield recharge rate
};
enum MobilityUpgradeType
{
	MASSREDUCTION, //temporarily reduces mass which increases responsiveness
	THRUSTBUFF, //temporarily increases thrust, increasing acceleration and top speed
};
class UpgradableTank;
class UpgradeNode :
	public CollidableNode
{
public:
	UpgradeNode(void);
	virtual void update(float p_DeltaTimeS);
	virtual void render(Renderer* p_Renderer);
	virtual void init()=0;
	virtual void Collect(UpgradableTank* p_Tank)=0;
	void setCollected(){m_Collected = true;}
	bool isCollected(){return m_Collected;}
	void setLifeTimeS(float p_Seconds){m_Timer = p_Seconds;}
	const UpgradeType& getUpgradeType(){return m_UpgradeType;}
	virtual ~UpgradeNode(void);
protected:
	bool m_Collected;
	float m_Timer;
	UpgradeType m_UpgradeType;
};

