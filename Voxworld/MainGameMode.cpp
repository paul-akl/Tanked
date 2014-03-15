#include "MainGameMode.h"
#include <GL\glew.h>
#include "GLTestRender.h"
#include "FloorFactory.h"
#include <algorithm>
#include "CollisionSolver.h"
#include "CollisionResponse.h"

#include <iostream>
MainGameMode::MainGameMode(void)
{
	printf("create main mode \n");	
	scene=nullptr;
	running = true;
}
void MainGameMode::init(Game& iGame)
{
	scene = new Scene();
	scene->activate();
	scene->addAISolver(new AIManager());
	scene->addController(m_Controller);
	scene->addCollisionSolver(new CollisionSolver());
	scene->addFloorFactory(new FloorFactory());
	scene->addRoboGenFactory(new RoboGenFactory());
	scene->addMaze(new Maze());
	scene->addWallFactory(new WallFactory());
	scene->addUpgradeFactory(new UpgradeFactory());
	scene->addTankFactory(new BasicTankFactory());
	scene->init();
	/*hud = new HUD();
	hud->setName("mainhud");
	hud->setPosition(glm::vec3(0));
	hud->setScale(glm::vec3(1.0));
	hud->init();	
	scene->addUI(hud);*/

}
void MainGameMode::update(float deltaTime)
{

	//game logic function.
	//ideally, code below will be abstracted away, within manager classes
	float deltaTimeS = deltaTime/1000.0f;
	if(m_Controller->getButtonState(EXIT))
	{
		running = false;
	}
	else
	{
		scene->update(deltaTimeS);
		
	}
}
bool MainGameMode::handleEvent(Game& iGame)
{
	//not much to do here...except changeState calls, 
	//which won't be needed till later

	return running;
}
void MainGameMode::draw(Renderer* p_Renderer)
{
	//render scene
	//ideally this will be abstracted away behind p_Renderer->renderScene(SceneNode* p_Scene);
	//p_Scene will be a root node for the entire level
	scene->render(p_Renderer);
	
}
MainGameMode::~MainGameMode(void)
{
	delete scene;
	printf("destroy main mode \n");
}
