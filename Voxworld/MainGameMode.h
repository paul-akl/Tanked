#pragma once
#include "gamemode.h"
#include <glm\glm.hpp>
#include "BasicTankFactory.h"
#include "CameraNode.h"
#include "FloorFactory.h"
#include "WallFactory.h"
#include "CollidableNode.h"
#include "CollisionResponse.h"
#include "UpgradeFactory.h"
#include <list>
#include "CollisionSolver.h"
#include "DeferredRenderer.h"
#include "Scene.h"
#include "HUD.h"
#include <SDL_thread.h>

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
	virtual void init(Game& iGame,Renderer* p_Renderer);
	virtual void update(float deltaTime);
	virtual bool handleEvent(Game& igame);
	virtual void draw(Renderer* p_Renderer);
	~MainGameMode(void);
private:
	void OpenMenu(Game& iGame);

	bool running;
	Scene* scene;

	HUD * hud;
};

