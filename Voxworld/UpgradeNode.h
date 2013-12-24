#pragma once
#include "collidablenode.h"
enum UpgradeType
{
	OFFENSIVE,
	DEFENSIVE,
	MOBILITY,
};
class UpgradeNode :
	public CollidableNode
{
public:
	UpgradeNode(void);
	virtual void update(float p_DeltaTimeS);
	virtual void render(Renderer* p_Renderer);
	virtual void init()=0;
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

