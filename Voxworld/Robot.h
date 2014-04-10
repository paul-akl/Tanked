#pragma once
//#include "collidablenode.h"
#include "EnemyNode.h"
#include "RobotArm.h"
#include "RobotHead.h"

class Robot :
	public EnemyNode
{
public:
	Robot(void);
	virtual void render(Renderer* p_Renderer);
	virtual void update(float p_DeltaTimeS);
	void addLeftArm(RobotArm* p_LeftArm);
	void addRightArm(RobotArm* p_RightArm);
	void addHead(RobotHead* p_Head);
	void addDamagedTexture(TextureNode* p_Texture);
	const int getHitPoints();
	void setHitPoints(const unsigned int p_HP);
	void dealDamage(unsigned int p_DamageAmount);
	void setMaxHitPoints(const unsigned int p_MaxHP);
	void setDamageMultiplier(float p_DamageMultipler){m_DamageMultiplier = p_DamageMultipler;}
	float getDamageMultipler(){return m_DamageMultiplier;}
	float getMass(){return m_Mass;}
	int getDamage(){return m_BaseDamage*(1+m_DamageMultiplier);}
	virtual ~Robot(void);
protected:
	void turnLeft(float p_DeptaTimeS);
	void turnRight(float p_DeltaTimeS);
	RobotArm* m_LeftArm;
	RobotArm* m_RightArm;
	glm::vec3 m_HeadPosition;
	glm::vec3 m_LeftArmPosition;
	glm::vec3 m_RightArmPosition;
	RobotHead* m_Head;
	float m_TurnSpeed;
	bool m_Turning;
	TextureNode* m_DamagedTextureDiffuse;
	int m_HitPoints;
	int m_MaxHitPoints;
	float m_TargetOrientation;
	float m_MaxVelocityScalar; //max V, in m/s
	float m_Thrust; //forward thrust scalar measured in Newtons/second.
	float m_Mass; //mass of the robot, measured in Kilograms
	float m_DamageMultiplier;
	int m_BaseDamage;
	glm::vec3 m_LastMoveTarget;
};

