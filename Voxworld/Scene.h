#pragma once
#include "scenenode.h"
#include "BasicTankFactory.h"
#include "FloorFactory.h"
#include "CollisionSolver.h"
#include "WallFactory.h"
#include "UpgradeFactory.h"
#include "Controller.h"
class Scene :
	public SceneNode
{
public:
	Scene(void);
	void addTankNode(TestTankNode* p_Tank);
	void addTankFactory(BasicTankFactory* p_Factory);
	void addWallFactory(WallFactory* p_Factory);
	void addFloorFactory(FloorFactory* p_Factory);
	void addUpgradeFactory(UpgradeFactory* p_Factory);
	void addCollisionSolver(CollisionSolver* p_Solver);
	void addController(Controller* p_Controller);
	//void addMazeFactory(MazeFactory* p_Factory);
	//void addEnemyFactory(EnemyFactory* p_Factory);
	//void addSubBossFactory(EnemyFactory* p_Factory);
	//void addBossFactory(EnemyFactory* p_Factory);
	//void addAISolver(AISolver* p_Solver);
	void detectCollisions(CollisionSolver* p_Solver);
	virtual void render(Renderer* p_Renderer);
	virtual void update(float p_DeltaTimeS);
	~Scene(void);
protected:
	//not used for now
	//std::vector<EnemyNode*> m_Enemies;
	//SubBossEnemy* m_SubBoss;
	//BossEnemy* m_Boss;
	std::vector<TestTankNode*> m_Tanks;
	std::vector<FloorNode*> m_Floors;
	std::vector<WallNode*> m_Walls;
	Controller* m_Controller;

};

