#pragma once
#include "Scene.h"
#include "CollisionPair.h"
class GameRules
{
public:
	GameRules(void);
	bool checkVictoryCondition(Scene* p_Scene);
	bool checkFailureCondition(Scene* p_Scene);
	void processCollisionEvent(CollisionPair* p_Event, Scene* p_Scene);
	//void processGameEvent(GameEvent* p_Event, Scene* p_Scene);
	//void processGameControlEvent(ControlEvent* p_Event, p_Scene);
	virtual ~GameRules(void);
};

