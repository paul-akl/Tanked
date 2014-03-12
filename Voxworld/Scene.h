#pragma once
#include "scenenode.h"
#include "BasicTankFactory.h"
#include "FloorFactory.h"
#include "CollisionSolver.h"
#include "WallFactory.h"
#include "UpgradeFactory.h"
#include "Controller.h"
#include "RoboGenFactory.h"
#include "CollisionPair.h"
#include "CameraNode.h"
#include "Renderer.h"
#include "CollisionResponse.h"
//#include "Maze.h"
#include "AIManager.h"
#include "Maze.h"
#include "HUD.h"

#include <SDL_thread.h>

struct ThreadDBlock
{
	float deltaTimeS;
	std::list<SceneNode*>* list;
};
struct ThreadCDBlock
{
	float deltaTimeS;
	std::list<UpgradeNode*>* list;
};
struct ThreadSceneryBlock
{
	float deltaTimeS;
	std::vector<SceneNode*>* vector;
};
class Scene :
	public SceneNode
{
public:
	Scene(void);
	void init();
	void addRoboGenFactory(RoboGenFactory* p_Factory);
	void addTankFactory(BasicTankFactory* p_Factory);
	void addWallFactory(WallFactory* p_Factory);
	void addFloorFactory(FloorFactory* p_Factory);
	void addUpgradeFactory(UpgradeFactory* p_Factory);
	void addCollisionSolver(CollisionSolver* p_Solver);
	void addController(Controller* p_Controller);
	void addUI(HUD* p_Hud);
	bool sceneComplete();
	void addMaze(Maze* p_Maze);
	TestTankNode* getTank(){return m_Tank;}
	//void addRobotGenerator(RobotGenerator* p_Factory);
	//void addSubBossFactory(EnemyFactory* p_Factory);
	//void addBossFactory(EnemyFactory* p_Factory);
	void addAISolver(AIManager* p_Solver);
	virtual void render(Renderer* p_Renderer);
	virtual void update(float p_DeltaTimeS);
	~Scene(void);
protected:
	void addTank(glm::vec3 p_Location);
	void addWall(glm::vec3 p_Location);
	void addFloor(glm::vec3 p_Location, float p_wallWidth, int p_gridWidth);
	void addRobot(Robot* p_Robot);
	void addRobotGenerator(glm::vec3 p_Location);
	void setGameDifficulty(unsigned int p_Difficulty);
	//private functions to be called during update cycle
	void detectCollisions();
	void ProcessUserInput();
	void checkGameConditions();
	void cleanup();
	void updateGameObjects();
	void updateWalls();
	void updateFloors();
	void updateEnemies();
	void updateProjectiles();
	void updateUI();
	static int updateScenery(void* p_Scenery);
	static int updateCollectables(void* p_Collectables);
	static int updateObjects(void* p_Objects);
	//not used for now
	//SubBossEnemy* m_SubBoss;
	float m_DeltaTimeSeconds;
	//BossEnemy* m_Boss;
	Maze* m_TestMaze;
	AIManager* m_AISolver;
	CollisionSolver* m_Solver;
	WallFactory* m_WallFactory;
	FloorFactory* m_FloorFactory;
	BasicTankFactory* m_TankFactory;
	UpgradeFactory* m_UpgradeFactory;
	RoboGenFactory* m_RoboGenFactory;
	HUD* m_Hud;
	TestTankNode* m_Tank;
	std::vector<CollisionPair*> m_CollisionEvents;
	std::list<ProjectileNode*> m_Projectiles;
	std::list<Robot*> m_Robots;
	std::list<RobotGenerator*> m_RoboGens;
	std::list<UpgradeNode*> m_Upgrades;
	std::list<OffensiveUpgrade*> m_AmmoBoxes;
	std::vector<WallNode*> m_Walls;
	std::vector<FloorNode*> m_Floors;
	//std::List<DefensiveUpgrade*> m_ShieldBoosts;
	//std::list<MobilityUpgrade*> m_SpeedBoosts;
	Controller* m_Controller;
	CameraNode* m_Camera;
	bool m_MainFireButtonPressed;
	float m_MouseTurnSpeed;
	float m_MouseAcceleration;
	float m_MaxTurnSpeed;
	float m_CamFollowDistance;
	float m_CamFollowAngle;
	unsigned int m_GameDifficulty;
	RenderMode m_CurrentRenderMode;
	bool m_Victory;
	bool m_Defeat;
	SDL_Thread* projThread;
	SDL_Thread* enemyThread;
	SDL_Thread* enemyGenThread;
	SDL_Thread* collectThread;
	SDL_Thread* SceneryThread;
	SDL_Thread* floorThread;
};

