#pragma once
#include "collidablenode.h"
#include "RobotArm.h"
#include "RobotHead.h"

class Robot :
	public CollidableNode
{
public:
	Robot(void);
	virtual void render(Renderer* p_Renderer);
	virtual void update(float p_DeltaTimeS);
	void addLeftArm(RobotArm* p_LeftArm);
	void addRightArm(RobotArm* p_RightArm);
	void addHead(RobotHead* p_Head);
	const unsigned int getHitPoints();
	void dealDamage(unsigned int p_DamageAmount);
	void setMaxHitPoints(const unsigned int p_MaxHP);
	virtual ~Robot(void);
protected:
	RobotArm* m_LeftArm;
	RobotArm* m_RightArm;
	RobotHead* m_Head;
	unsigned int m_HitPoints;
	unsigned int m_MaxHitPoints;
	float m_TargetOrientationDeg;
};

