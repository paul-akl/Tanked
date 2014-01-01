#pragma once
#include "gamemode.h"
#include <glm\glm.hpp>
#include "BasicTankFactory.h"
#include "CameraNode.h"
#include "FloorFactory.h"
//#include "Scene.h"
#include "WallFactory.h"
#include "CollidableNode.h"
#include "CollisionResponse.h"
#include "UpgradeFactory.h"
#include <list>
//each game mode will have it's own render class.
class Renderer;
/////////////////////////////////////////
// The main game mode class			   //
// This class handles game logic for   //
// the core engine					   //
/////////////////////////////////////////
class MainGameMode :
	public GameMode
{
public:
	MainGameMode(void);
	virtual void init(Game& iGame);
	virtual void update(float deltaTime);
	virtual bool handleEvent(Game& igame);
	virtual void draw(Renderer* p_Renderer);
	~MainGameMode(void);
private:
	void OpenMenu(Game& iGame);
	BasicTankFactory* m_TankFactory;
	FloorFactory* m_FloorFactory;
	WallFactory* m_WallFactory;
	TestTankNode* m_Tank;
	CameraNode* m_Camera;
	std::vector<SceneNode*> m_Ground;
	std::vector<CollidableNode*> m_Walls;
	bool running;
	RenderMode renderingMode;
	float m_CamFollowDistance;
	float m_CamFollowAngle;
	//Scene* scene;
	float m_MouseTurnSpeed;
	float m_MaxTurnSpeed;
	float m_MouseAcceleration;
	CollisionSolver* m_Solver;
	CollisionResponse* m_ResultDecider;
	UpgradeFactory* m_UpgradeMaker;
	std::vector<OffensiveUpgrade*> m_AmmoBoxes;
	std::list<ProjectileNode*> m_Projectiles;
	bool m_MainFireButtonPressed;
};

