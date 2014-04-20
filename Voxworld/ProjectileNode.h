#pragma once
#include "collidablenode.h"
#include "LightNode.h"
enum ProjectileType
{
	DEFAULT_MAIN,
	DEFAULT_SECONDARY,
	BOUNCING_SHELL,
	INCENDIARY,
	ICE_SHOT,
	GRAVITY_SHELL,
	CONCUSSION_SHELL
};
class ProjectileNode :
	public CollidableNode
{
public:
	ProjectileNode(void);
	//get/set base damage inflicted when this collides with something
	float getBaseDamage(){return m_BaseDamage;}
	void setBaseDamage(float p_BaseDamage){m_BaseDamage = p_BaseDamage;}
	//get/set projectile life time. enter -1.0f for no lifetime
	void setDamageMultiplier(float p_Multiplier){m_DamageMultiplier=p_Multiplier;}
	void setLifeTime(float p_Seconds){m_MaxLifeTime = p_Seconds;m_Timer = p_Seconds;}
	float getLifeTimeS(){return m_MaxLifeTime;}
	float getTimeToLive(){return m_Timer;}
	float getDamage(){return m_BaseDamage*m_DamageMultiplier;}
	void addLight(LightNode* p_Light);
	//used for when a projectile hits something.
	//Effect* getCollisionEffect();
	void setOrientation(const float p_OrientationDeg);
	virtual void update(float p_DeltaTimeS);
	virtual void render(Renderer* p_Renderer);
	virtual void Bounce(const glm::vec3& p_Normal);
	const ProjectileType getProjectileType(){return m_ProjectileType;}
	void setProjectileType(const ProjectileType p_Type){m_ProjectileType=p_Type;}
	void reset();
	virtual ~ProjectileNode(void);
protected:
	float m_MaxLifeTime;
	float m_Timer;
	float m_BaseDamage;
	float m_DamageMultiplier;
	float m_VelocityScalar;
	ProjectileType m_ProjectileType;
	LightNode* m_Light;
};

