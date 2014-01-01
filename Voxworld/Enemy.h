#pragma once
#include "CollidableNode.h"
enum EnemyStatus
{
	ENEMY_PASSIVE,
	ENEMY_ALERT,
	ENEMY_AGGRESSIVE
};
enum EnemyType
{
	ENEMYTYPE_DEFAULT,
	ENEMYTYPE_FIRE,
	ENEMYTYPE_ICE,
	ENEMYTYPE_GRAVITY,
	ENEMYTYPE_CONCUSSIVE
};
class Enemy :
	public CollidableNode
{
public:
	Enemy(void);
	void setEnemyStatus(const EnemyStatus p_StatusChange);
	const EnemyStatus getEnemyStatus();
	void setDestination(const glm::vec3& p_Destination);
	void setEnemyType(const EnemyType p_EnemyType);
	const EnemyType getEnemyType();
	virtual ~Enemy(void);
protected:
	unsigned int m_HitPoints;
	EnemyStatus m_State;
	EnemyType m_EnemyType;
};

