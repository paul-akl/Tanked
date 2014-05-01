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
#include "ParticleSystemFactory.h"
//#include "Maze.h"
#include "AIManager.h"
#include "Maze.h"
#include "HUD.h"
#include "I_Audio.h"

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
struct ThreadEnemyBlock
{
	float deltaTimeS;
	std::list<EnemyNode*>* list;
	AIManager* solver;
	Maze* maze;
	SceneNode* player;
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
	void addRenderer(Renderer* p_Renderer);
	void addParticleFactory(ParticleSystemFactory* p_Factory);
	void addUI(HUD* p_Hud);
	void nextLevel();

	bool sceneComplete();
	bool gameOver(){return m_Defeat;}
	void addMaze(Maze* p_Maze);
	TestTankNode* getTank(){return m_Tank;}
	//void addRobotGenerator(RobotGenerator* p_Factory);
	//void addSubBossFactory(EnemyFactory* p_Factory);
	//void addBossFactory(EnemyFactory* p_Factory);
	void addAISolver(AIManager* p_Solver);
	bool isPaused(){return m_Paused;}
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
	void updateProjectiles();
	void updateUI();
	static int updateEnemies(void* p_EnemBlock);
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
	ParticleSystemFactory* m_ParticleFactory;
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
	Renderer* m_Renderer;
	CameraNode* m_Camera;	
	float m_MouseTurnSpeed;
	float m_MouseAcceleration;
	float m_MaxTurnSpeed;
	float m_CamFollowDistance;
	float m_CamFollowAngle;
	int m_PlayerScore;
	unsigned int m_GameDifficulty;
	RenderMode m_CurrentRenderMode;
	bool m_Victory;
	bool m_Defeat;
	bool m_Paused;
	bool m_PausePressed;
	bool m_MainFireButtonPressed;
	bool m_MoveKeyPressed;
	bool movesoundplaying;
	bool chargesoundplaying;
	SDL_Thread* projThread;
	SDL_Thread* enemyThread;
	SDL_Thread* enemyGenThread;
	SDL_Thread* collectThread;
	SDL_Thread* SceneryThread;
	SDL_Thread* floorThread;
	I_Audio* m_Audio;
};

