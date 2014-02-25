#include "CollisionPairManager.h"
#include <time.h>
#include <iostream>

CollisionPairManager::CollisionPairManager(void)
{

}
void CollisionPairManager::init()
{
	for (int i = 0; i < MAX_PAIRS;i++)
	{
		m_Pairs.push_back(new CollisionPair(nullptr,nullptr));
	}
}
CollisionPair* CollisionPairManager::getPair()
{
	CollisionPair* pair = nullptr;
	bool found = false;
	for(size_t i = 0; i < m_Pairs.size();i++)
	{
		if(!m_Pairs[i]->m_InUse)
		{
			pair = m_Pairs[i];
			pair->m_InUse = true;
			pair->m_Collided = false;
			pair->m_Collidable_A = nullptr;
			pair->m_Collidable_B = nullptr;
			//printf("pair reuse\n");
			return pair;
		}
	}
	pair = new CollisionPair(nullptr,nullptr);
	m_Pairs.push_back(pair);
	//printf("new pair\n");
	return pair;

}
CollisionPairManager::~CollisionPairManager(void)
{
	for (size_t i = 0; i < m_Pairs.size();i++)
	{
		delete m_Pairs[i];
	}
	m_Pairs.clear();
}
