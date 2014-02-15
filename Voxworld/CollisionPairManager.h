#pragma once
#include <vector>
#include "CollisionPair.h"
class CollisionPairManager
{
public:
	CollisionPairManager(void);
	CollisionPair* getPair();
	void init();
	~CollisionPairManager(void);
private:

std::vector<CollisionPair*> m_Pairs;
static const int MAX_PAIRS = 1024;
};

 