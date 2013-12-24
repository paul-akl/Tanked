#pragma once
#include "CollisionPair.h"
class CollisionResponse
{
public:
	CollisionResponse(void);
	void processResult(CollisionPair* p_Pair);
	~CollisionResponse(void);
};

