#include "CollisionPair.h"
#include <iostream>

CollisionPair::CollisionPair(CollidableNode* p_Collidable_A,CollidableNode* p_Collidable_B)
{
	m_Collidable_A=p_Collidable_A;
	m_Collidable_B=p_Collidable_B;
	m_Collided = false;
	m_Penetration = 0.0f;
}
void CollisionPair::toConsole()
{
	std::cout<<m_Collidable_A->getName()<<" vs "<<m_Collidable_B->getName()<<std::endl;
}
bool CollisionPair::operator==(CollisionPair* p_OtherPair)
{
	//as nodes are named uniquely, a simple string check can make sure we have a different pair
	//of nodes from the parameter pair
	//if we have a match
	if( m_Collidable_A->getName() == p_OtherPair->m_Collidable_A->getName() ||
		m_Collidable_B->getName() == p_OtherPair->m_Collidable_A->getName() &&
		m_Collidable_A->getName() == p_OtherPair->m_Collidable_B->getName() ||
		m_Collidable_B->getName() == p_OtherPair->m_Collidable_B->getName() )
	{
		return true;
	}
	//else
	return false;
}
bool CollisionPair::equals(CollisionPair* p_OtherPair)
{
	//as nodes are named uniquely, a simple string check can make sure we have a different pair
	//of nodes from the parameter pair
	//if we have a match
	if( (m_Collidable_A->getName() == p_OtherPair->m_Collidable_A->getName() &&
		m_Collidable_B->getName() == p_OtherPair->m_Collidable_B->getName()) ||
		(m_Collidable_B->getName() == p_OtherPair->m_Collidable_A->getName() &&
		m_Collidable_A->getName() == p_OtherPair->m_Collidable_B->getName()) )
	{
		return true;
	}
	//else
	return false;
}
CollisionPair::~CollisionPair(void)
{
	m_Collidable_A = nullptr;
	m_Collidable_B = nullptr;
}
