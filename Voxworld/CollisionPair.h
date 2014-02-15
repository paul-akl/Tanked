#pragma once
#include "CollidableNode.h"

////////////////////////////////////////////////////////////////////
// Simple class to hold two collidables, their collision status,
// and a collision point, if any.
// Nothing more than a data storage class really
////////////////////////////////////////////////////////////////////
enum ResultType
{
	PLAYERVSENEMY,
	PLAYERVSSCENERY,
	PLAYERVSCOLLECTABLE,
	PLAYERVSPROJECTILE,
	ENEMYVSPROJECTILE,
	ENEMYVSSCENERY,
	ENEMYVSENEMY,
	PROJECTILEVSSCENERY
};
class CollisionPair
{
public:
	CollisionPair(CollidableNode* p_Collidable_A,CollidableNode* p_Collidable_B);
	void setCollided(bool p_Collided){m_Collided = true;}
	bool hasCollided(){return m_Collided;}
	void setCollisionPoint(glm::vec3 p_Point){m_CollisionPoint = p_Point;}
	glm::vec3 getCollisionPoint(){return m_CollisionPoint;}
	bool operator==(CollisionPair* p_OtherPair);
	bool equals(CollisionPair* p_OtherPair);
	const ResultType getResultType(){return m_ResultType;}
	void setResultType(const ResultType p_ResultType){m_ResultType = p_ResultType;}
	void toConsole();
	CollidableNode* m_Collidable_A;
	CollidableNode* m_Collidable_B;
	bool m_Collided;
	glm::vec3 m_CollisionPoint;
	glm::vec3 m_CollisionNormal;
	float m_Penetration;
	bool m_InUse;
	ResultType m_ResultType;
	~CollisionPair(void);

};

