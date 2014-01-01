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
	renderingMode = FILLED;
	m_CamFollowDistance = -15.0f;
	m_CamFollowAngle = -15.0f;
	m_MaxTurnSpeed = 400.0f;
	m_MouseTurnSpeed = 0.0f;
	m_MouseAcceleration = 25.0f;
	m_MainFireButtonPressed=false;
}
void MainGameMode::init(Game& iGame)
{
	printf("init main mode \n");
	m_TankFactory = new BasicTankFactory();
	m_TankFactory->init();
	m_Tank = m_TankFactory->getTank();
	m_Tank->setPosition(glm::vec3(0.0f,0.0f,-50.0f));
	m_Tank->setRadius(6.0f);
	m_Camera = new CameraNode();
	m_Camera->rotatePitch(m_CamFollowAngle);
	m_Camera->moveForward(m_CamFollowDistance);
	m_Camera->moveUp(3.0f);
	//generate floors TEMP CODE
	m_FloorFactory = new FloorFactory();
	m_FloorFactory->init();
	FloorNode* floor = m_FloorFactory->getFloor();
	floor->setPosition(glm::vec3(0.0f));
	floor->setSize(15.0f);
	m_Ground.push_back(floor);
	floor = m_FloorFactory->getFloor();
	floor->setPosition(glm::vec3(0.0f,0.0f,30.0f));
	floor->setSize(15.0f);
	m_Ground.push_back(floor);
	floor = m_FloorFactory->getFloor();
	floor->setPosition(glm::vec3(-30.0f,0.0f,30.0f));
	floor->setSize(15.0f);
	m_Ground.push_back(floor);
	floor = m_FloorFactory->getFloor();
	floor->setPosition(glm::vec3(30.0f,0.0f,-30.0f));
	floor->setSize(15.0f);
	m_Ground.push_back(floor);
	floor = m_FloorFactory->getFloor();
	floor->setPosition(glm::vec3(0.0,0.0f,-30.0f));
	floor->setSize(15.0f);
	m_Ground.push_back(floor);
	//generate walls TEMP CODE
	m_WallFactory = new WallFactory();
	m_WallFactory->init();
	WallNode* wall1 = m_WallFactory->getInstance();
	wall1->setPosition(glm::vec3(30.0f,15.0f,0.0f));
	wall1->setWidth(30.0f);
	wall1->setRadius(15.0f);
	wall1->setLength(30.0f);
	m_Walls.push_back(wall1);
	WallNode* wall2 = m_WallFactory->getInstance();
	wall2->setPosition(glm::vec3(-30.0f,15.0f,0.0f));
	wall2->setWidth(30.0f);
	wall2->setLength(30.0f);
	wall2->setRadius(15.0f);
	m_Walls.push_back(wall2);
	wall2 = m_WallFactory->getInstance();
	wall2->setPosition(glm::vec3(30.0f,15.0f,30.0f));
	wall2->setWidth(30.0f);
	wall2->setRadius(15.0f);
	wall2->setLength(30.0f);
	m_Walls.push_back(wall2);
	wall2 = m_WallFactory->getInstance();
	wall2->setPosition(glm::vec3(-30.0f,15.0f,-30.0f));
	wall2->setWidth(30.0f);
	wall2->setRadius(15.0f);
	wall2->setLength(30.0f);
	m_Walls.push_back(wall2);
	//scene = new Scene();
	m_Solver = new CollisionSolver();
	m_Solver->init(1000.0f,1000.0f,glm::vec3(0.0f));
	for (size_t i = 0; i < m_Walls.size();i++)
	{
		m_Solver->addScenery(m_Walls[i]);
	}
	//m_Solver->addScenery(m_Walls[0]);
	m_Solver->addTank(m_Tank);
	m_ResultDecider = new CollisionResponse();
	m_UpgradeMaker = new UpgradeFactory();
	m_UpgradeMaker->init();
	OffensiveUpgrade* temp = m_UpgradeMaker->getOffensiveUpgrade(DEFAULT);
	temp->setPosition(glm::vec3(0.0f,0.0f,25.0f));
	temp->setRadius(3.0f);
	temp->setLifeTimeS(120.0f);
	temp->setPosition(glm::vec3(0.0f,0.0f,30.0f));
	m_AmmoBoxes.push_back(temp);
	for (size_t i = 0; i < m_AmmoBoxes.size();i++)
	{
		m_Solver->addCollectable(m_AmmoBoxes[i]);
	}
	temp = nullptr;
	temp = m_UpgradeMaker->getOffensiveUpgrade(AUTOGUN);
	m_Tank->AddOffensiveUpgrade(temp);
	//add mesh, texture, transform, to floor node
}
void MainGameMode::update(float deltaTime)
{

	//game logic function.
	//ideally, code below will be abstracted away, within manager classes
	float deltaTimeS = deltaTime/1000.0f;
	m_Solver->update();

	////////////////////////////////////////////////////////////////////////
	//////////				 USER INPUT           //////////////////////////
	////////////////////////////////////////////////////////////////////////
	int mouseXmove = m_Controller->getMouseMovementX();

	if(m_Controller->getButtonState(MOUSELEFT))
	{
		if(m_MouseTurnSpeed<m_MaxTurnSpeed)
			m_MouseTurnSpeed+=m_MouseAcceleration;
		else
			m_MouseTurnSpeed = m_MaxTurnSpeed;
		/*m_Camera->rotateYaw(m_MouseTurnSpeed*deltaTimeMS);

		m_Tank->rotateTurret(m_MouseTurnSpeed*deltaTimeMS);*/
	}

	else if(m_Controller->getButtonState(MOUSERIGHT))
	{
		if(m_MouseTurnSpeed>-m_MaxTurnSpeed)
			m_MouseTurnSpeed-=m_MouseAcceleration;
		else
			m_MouseTurnSpeed = -m_MaxTurnSpeed;

	}
	else
	{
		m_MouseTurnSpeed *=0.5;
	}
	m_Camera->rotateYaw(m_MouseTurnSpeed*deltaTimeS);
	m_Tank->rotateTurret(m_MouseTurnSpeed*deltaTimeS);
	if(m_Controller->getButtonState(FORWARD))
	{
		//move tank forward
		m_Tank->moveForward(deltaTimeS);
		//printf("forward\n");
	}
	if(m_Controller->getButtonState(BACKPEDAL))
	{
		//move tank back
		m_Tank->moveBack(deltaTimeS);
		//printf("back\n");
	}
	if(m_Controller->getButtonState(STRAFE_L))
	{
		m_Tank->moveLeft(deltaTimeS);
	}
	if(m_Controller->getButtonState(STRAFE_R))
	{
		m_Tank->moveRight(deltaTimeS);
		//printf("right\n");
		//move tank right
	}
	if(m_Controller->getButtonState(PRIMARY_ATTACK))
	{
		//unsupported operation
		//request autogun projectile from tank
		//std::cout<<"LMB pressed"<<std::endl;
		ProjectileNode* tmp = m_Tank->getAutoGunProjectile();
		if(tmp!=nullptr)
		{
			m_Projectiles.push_back(tmp);
			m_Solver->addProjectile(tmp);
		}
	}
	if(m_Controller->getButtonState(SECONDARY_ATTACK))
	{
		//std::cout<<"RMB pressed"<<std::endl;
		m_MainFireButtonPressed=true;
		m_Tank->chargeMainGun(deltaTimeS);
	}
	else
	{
		if(m_MainFireButtonPressed)
		{
			//std::cout<<"RMB released"<<std::endl;
			m_MainFireButtonPressed = false;
			ProjectileNode* tmp = m_Tank->getMainGunProjectile();
			if(tmp!=nullptr)
			{
				m_Projectiles.push_front(tmp);
				m_Solver->addProjectile(tmp);
			}
		}
	}
	//DEBUG CONTROLS
	if(m_Controller->getButtonState(RENDERMODE_DEBUG))
	{
		renderingMode = DEBUG;
	}
	if(m_Controller->getButtonState(RENDERMODE_DEFAULT))
	{
		renderingMode = FILLED;
	}

	m_Camera->LookAt(m_Tank->getLocation(),m_CamFollowDistance);
	m_Camera->moveUp(5.0f);
	//temprary code: will be reviewed
	if(m_Controller->getButtonState(EXIT))running = false;
	else running = true;
	/////////////////////////////////////////////////////////////////
	//////////////		COLLISION DETECTION				/////////////
	/////////////////////////////////////////////////////////////////

	std::vector<CollisionPair*> p_Results;
	m_Solver->collideObject(m_Tank, p_Results);
	for(std::list<ProjectileNode*>::iterator it = m_Projectiles.begin(); it!= m_Projectiles.end(); it++)
	{
		m_Solver->collideObject((*it),p_Results);
	}
	m_Solver->getResults(p_Results);

	//////////////////////////////////////////////////////////
	//////////			COLLISION RESPONSE			//////////
	//////////////////////////////////////////////////////////

	if(!p_Results.empty())
	{
		for(size_t i = 0; i < p_Results.size();i++)
		{
			m_ResultDecider->processResult(p_Results[i]);
		}
	}
	p_Results.clear();
	/////////////////////////////////////////////////////////////////////
	//////////////				UPDATE GAME OBJECTS				/////////
	/////////////////////////////////////////////////////////////////////
	for(size_t i = 0; i < m_Walls.size();i++)
	{
		m_Walls[i]->update(deltaTimeS);
	}
	for(size_t i = 0; i < m_Ground.size();i++)
	{
		m_Ground[i]->update(deltaTimeS);
	}
	for(std::list<ProjectileNode*>::iterator it = m_Projectiles.begin(); it!= m_Projectiles.end(); it++)
	{
		(*it)->update(deltaTimeS);
	}

	for(size_t i = 0; i < m_AmmoBoxes.size();i++)
	{
		m_AmmoBoxes[i]->update(deltaTimeS);
	}
	m_Tank->update(deltaTimeS);
	for(std::list<ProjectileNode*>::iterator it = m_Projectiles.begin(); it != m_Projectiles.end(); /*increment in body*/)
	{
		if(!(*it)->isActive()) //isActive returns a bool
		{
			it = m_Projectiles.erase(it);
		}
		else
		{
			++it;
		}
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
	p_Renderer->beginRenderCycle(renderingMode);
	p_Renderer->render(m_Camera);

	for (size_t i = 0; i < m_Ground.size();i++)
	{
		m_Ground[i]->render(p_Renderer);
	}
	for (size_t i = 0; i < m_Walls.size();i++)
	{
		m_Walls[i]->render(p_Renderer);
	}
	//m_Walls[0]->render(p_Renderer);
	for(std::list<ProjectileNode*>::iterator it = m_Projectiles.begin(); it!= m_Projectiles.end(); it++)
	{
		if((*it)->isActive())
		(*it)->render(p_Renderer);
	}

	for(size_t i = 0; i < m_AmmoBoxes.size(); i++)
	{
		if(m_AmmoBoxes[i]->isActive())
		m_AmmoBoxes[i]->render(p_Renderer);
	}

	m_Tank->render(p_Renderer);
	p_Renderer->endRenderCycle();
	//SDL_WarpMouseInWindow(p_Renderer->getWindow(),400,300);

}
MainGameMode::~MainGameMode(void)
{
	m_Tank = nullptr;
	m_TankFactory = nullptr;
	printf("destroy main mode \n");
}
