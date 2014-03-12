#include "Scene.h"
#include "Renderer.h"
#include <glm\glm.hpp>
#include <iostream>
#include "TransformNode.h"
#include <ctime>
#include <typeinfo>
Scene::Scene(void)
{
	m_CurrentRenderMode = FILLED;
	m_CamFollowDistance = -15.0f;
	m_CamFollowAngle = -15.0f;
	m_MaxTurnSpeed = 350.0f;
	m_MouseTurnSpeed = 0.0f;
	m_MouseAcceleration = 50.0f;
	m_MainFireButtonPressed=false;
	m_Victory = false;
	m_Defeat = false;
}
bool Scene::sceneComplete()
{
	if(m_Defeat || m_Victory)
	{
		return true;
	}
	return false;
}
void Scene::init()
{
	printf("init scene");
	m_LocalTransform=new TransformNode();
	m_LocalTransform->reset();
	m_WallFactory->init();
	m_FloorFactory->init();
	m_TankFactory->init();
	m_RoboGenFactory->init();
	m_UpgradeFactory->init();
	//will be changed upon integration of maze node
	//addTank(mazeObject->getStartPoint());
	//add 4 robot generators to world according to maze
	//add walls and floors according to maze
	float worldWidth = m_TestMaze->getGridWidth();
	m_Camera = new CameraNode();
	m_Camera->rotatePitch(m_CamFollowAngle);
	m_Camera->moveForward(m_CamFollowDistance);
	m_Camera->moveUp(3.0f);
	m_GameDifficulty = 1;
	m_TestMaze->generateMaze(25+m_GameDifficulty, 25+m_GameDifficulty, 30.0f);
	//m_TestMaze->toConsole();
	m_AISolver->setMaze(m_TestMaze);
	m_Solver->init(m_TestMaze->getGridWidth(),glm::vec3(m_TestMaze->getGridWidth()*0.5f,0.0f,m_TestMaze->getGridHeight()*0.5f));
	MazeIterator *v_mazeIterator = m_TestMaze->getIterator();
	while(v_mazeIterator->hasNext())
	{
		Position cell = v_mazeIterator->getNext();

		//std::cout<<cell.x<<" "<<cell.y<<std::endl;

		switch(m_TestMaze->getGridCellType(cell))
		{
		case WALL:
			{
				addWall(m_TestMaze->getCellPosition(cell));
			}break;
		case GENERATOR:
			{
				addRobotGenerator(m_TestMaze->getCellPosition(cell));
			}break;
		case START:
			{
				addTank(m_TestMaze->getCellPosition(cell));
			}break;
		};
	}
	addFloor(glm::vec3(m_TestMaze->getGridWidth() / 2, 0.0f, m_TestMaze->getGridHeight() / 2), m_TestMaze->getNumRows(), m_TestMaze->getGridWidth());
	m_Hud = new HUD();
	m_Hud->setName("mainhud");
	m_Hud->setPosition(glm::vec3(0));
	m_Hud->setOrientation(0);
	m_Hud->setRenderRadius(glm::vec3(1.0));
	m_Hud->init();
	m_Hud->setMetricMax(SHIELDGAUGE, m_Tank->getMaxShieldHitPoints());
	m_Hud->setMetricMax(WEAPONGAUGE, m_Tank->getMaxWeaponChargeLevel());
	printf("init scene complete");
}
void Scene::addTank(glm::vec3 p_Location)
{
	m_Tank = m_TankFactory->getTank();
	m_Tank->setPosition(p_Location);
	m_Tank->setRadius(6.0f);
	//SceneNode::addNode(m_Tank);
	m_Solver->addTank(m_Tank);
	m_AISolver->setPlayer(m_Tank);
	OffensiveUpgrade* upgrade = m_UpgradeFactory->getOffensiveUpgrade(AUTOGUN);
	upgrade->init();
	m_Tank->AddOffensiveUpgrade(upgrade);
	upgrade = m_UpgradeFactory->getOffensiveUpgrade(DEFAULT);
	upgrade->init();
	m_Tank->AddOffensiveUpgrade(upgrade);
}
void Scene::addFloor(glm::vec3 p_Location, float p_wallWidth, int p_gridWidth)
{
	FloorNode* floor = m_FloorFactory->getFloor(p_gridWidth, p_wallWidth);
	floor->setPosition(p_Location);
	m_Floors.push_back(floor);
}
void Scene::addWall(glm::vec3 p_Location)
{
	WallNode* wall = m_WallFactory->getInstance();
	wall->setWidth(30.0f);
	wall->setRadius(15.0f);
	wall->setLength(30.0f);
	wall->setPosition(p_Location);
	m_Solver->addScenery(wall);
	m_Walls.push_back(wall);
}
void Scene::addRobotGenerator(glm::vec3 p_Location)
{
	//this ideally will have various algorithms which will selectively create new types of generators
	RobotGenerator* robogen = m_RoboGenFactory->getRobotGenerator(m_GameDifficulty,ROBOT_DEFAULT);
	robogen->setPosition(p_Location);
	m_RoboGens.push_front(robogen);
	m_Solver->addEnemyGen(robogen);
}
void Scene::addRobot(Robot* p_Robot)
{
	m_Robots.push_front(p_Robot);
	//m_Solver->addEnemy(p_Robot);
}
void Scene::addCollisionSolver(CollisionSolver* p_Solver)
{
	m_Solver = p_Solver;
}
void Scene::detectCollisions()
{
	m_Solver->processCollisions(m_CollisionEvents);
}
void Scene::addAISolver(AIManager* p_Solver)
{
	m_AISolver = p_Solver;
}
void Scene::addMaze(Maze* p_Maze)
{
	m_TestMaze = p_Maze;
}
void Scene::addUI(HUD* p_Hud)
{
	m_Hud = p_Hud;
}
void Scene::updateGameObjects()
{
	//multi-threading candidates
	int num;
	//create thread data blocks for new threads
	ThreadDBlock projblock, enemBlock,enemyGenBlock;
	ThreadCDBlock collectblock;
	ThreadSceneryBlock floorBlock, wallBlock;

	//populate data blocks and create threads.
	projblock.list = (std::list<SceneNode*>*)&m_Projectiles;
	projblock.deltaTimeS = m_DeltaTimeSeconds;
	projThread = SDL_CreateThread(updateObjects,"proj thread",&projblock);

	wallBlock.vector = (std::vector<SceneNode*>*)&m_Walls;
	wallBlock.deltaTimeS = m_DeltaTimeSeconds;
	SceneryThread = SDL_CreateThread(updateScenery,"wall thread", &wallBlock);

	enemBlock.deltaTimeS = m_DeltaTimeSeconds;
	enemBlock.list = (std::list<SceneNode*>*)&m_Robots;
	enemyThread = SDL_CreateThread(updateObjects,"enemy thread",&enemBlock);
	
	enemyGenBlock.deltaTimeS = m_DeltaTimeSeconds;
	enemyGenBlock.list = (std::list<SceneNode*>*)&m_RoboGens;
	enemyGenThread = SDL_CreateThread(updateObjects,"robogen thread",&enemyGenBlock);

	collectblock.deltaTimeS = m_DeltaTimeSeconds;
	collectblock.list = (std::list<UpgradeNode*>*)&m_AmmoBoxes;
	collectThread = SDL_CreateThread(updateCollectables,"proj thread",&collectblock);

	floorBlock.deltaTimeS = m_DeltaTimeSeconds;
	floorBlock.vector = (std::vector<SceneNode*>*)&m_Floors;
	floorThread = SDL_CreateThread(updateScenery,"floor Thread", &floorBlock);
	//signal OS to wait until threads are complete before moving on.
	SDL_WaitThread(projThread, &num);
	SDL_WaitThread(enemyThread, &num);
	SDL_WaitThread(SceneryThread, &num);
	SDL_WaitThread(floorThread, &num);
	SDL_WaitThread(enemyGenThread, &num);
	SDL_WaitThread(collectThread, &num);
	///////////////////////////////////
	//detach threads???
	///////////////////////////////////
}
void Scene::updateUI()
{
	m_Hud->update(m_DeltaTimeSeconds);
}

//NOT NEEDED
void Scene::updateWalls()
{
	//update walls
	for (size_t i = 0; i <m_Walls.size();i++)
	{
		if(m_Walls[i]->isActive())
		{
			m_Walls[i]->update(m_DeltaTimeSeconds);
		}
	} 
}
//NOT NEEDED
void Scene::updateEnemies()
{
	//nested multi-threading?
	if(!m_Robots.empty())
	{
		for (std::list<Robot*>::iterator it = m_Robots.begin(); it!= m_Robots.end();it++)
		{
			if((*it)->isActive())
			{
				m_AISolver->doBehaviour((*it));
				(*it)->update(m_DeltaTimeSeconds);
			}
		}
	}
	//update robo generators
	if(!m_RoboGens.empty())
	{
		for (std::list<RobotGenerator*>::iterator it = m_RoboGens.begin(); it!= m_RoboGens.end();it++)
		{
			if((*it)->isActive())
			{
				m_AISolver->doBehaviour((*it));
				(*it)->update(m_DeltaTimeSeconds);
			}
		}
	}
}
//NOT NEEDED
void Scene::updateFloors()
{
	//draw floors
	for (size_t i = 0; i <m_Floors.size();i++)
	{
		if(m_Floors[i]->isActive())
		{
			m_Floors[i]->update(m_DeltaTimeSeconds);
		}
	} 
}
//NOT NEEDED
void Scene::updateProjectiles()
{
	if(!m_Projectiles.empty())
	{
		for (std::list<ProjectileNode*>::iterator it = m_Projectiles.begin(); it!= m_Projectiles.end();it++)
		{
			if((*it)->isActive())
			{
				(*it)->update(m_DeltaTimeSeconds);
			}
		}
	}
}
int Scene::updateScenery(void* p_Scenery)
{
	ThreadSceneryBlock* block = static_cast<ThreadSceneryBlock*>(p_Scenery);

	for (std::vector<SceneNode*>::iterator it = block->vector->begin();it!= block->vector->end();)
	{
		if((*it)->isActive())
		{
			(*it)->update(block->deltaTimeS);
			it++;
		}
		else
			it = block->vector->erase(it);
	}
	return 0;
}
int Scene::updateCollectables(void* p_Collectables)
{
	ThreadCDBlock* block = static_cast<ThreadCDBlock*>(p_Collectables);
	for (std::list<UpgradeNode*>::iterator it = block->list->begin(); it != block->list->end();)
	{
		if((*it)->isActive() && !(*it)->isCollected())
		{
			(*it)->update(block->deltaTimeS);
			it++;
		}
		else
			it = block->list->erase(it);
			
	}
	return 0;

}
int Scene::updateObjects(void* p_Objects)
{
	ThreadDBlock* block = static_cast<ThreadDBlock*> (p_Objects);
	for (std::list<SceneNode*>::iterator it = block->list->begin(); it != block->list->end();)
	{
		if((*it)->isActive())
		{
			(*it)->update(block->deltaTimeS);
			it++;
		}
		else
			it = block->list->erase(it);
			
	}
	return 0;
}

void Scene::checkGameConditions()
{
	// simple collision event handling & correction system, with some added game logic
	if(!m_CollisionEvents.empty())
	{
		for (size_t i = 0; i <m_CollisionEvents.size(); i++)
		{
			if(m_CollisionEvents[i]->hasCollided())
			{
				switch(m_CollisionEvents[i]->getResultType())
				{
				// handles collisions between player controlled objects and scenery (walls, for the moment, but could include more)
				case PLAYERVSSCENERY:
					{
						if(m_CollisionEvents[i]->m_Collidable_A->getType() == PLAYER)
						{
							glm::vec3 normal = glm::vec3(m_CollisionEvents[i]->m_CollisionNormal.x,0.0f,m_CollisionEvents[i]->m_CollisionNormal.z);
							glm::vec3 velocity = m_CollisionEvents[i]->m_Collidable_A->getVelocity();
							glm::vec3 reflection = glm::reflect(velocity,normal);
							if(m_CollisionEvents[i]->m_Collidable_A->isMoving())
							{
								m_CollisionEvents[i]->m_Collidable_A->stop();
								m_CollisionEvents[i]->m_Collidable_A->setVelocity(glm::vec3(reflection.x,0.0f,reflection.z)*0.8f);
							}
						}
						else
						{
							glm::vec3 normal = glm::vec3(m_CollisionEvents[i]->m_CollisionNormal.x,0.0f,m_CollisionEvents[i]->m_CollisionNormal.z);
							glm::vec3 reflection = glm::reflect(m_CollisionEvents[i]->m_Collidable_B->getVelocity(),glm::normalize(-normal))*0.8f;
							if(m_CollisionEvents[i]->m_Collidable_B->isMoving())
							{
								m_CollisionEvents[i]->m_Collidable_B->stop();
								m_CollisionEvents[i]->m_Collidable_B->setVelocity(glm::vec3(reflection.x,0.0f,reflection.z)*0.8f);
							}
						}
					}break;
				// handles collision between player controlled objects and collectable objects, which can be stored in player controlled objects
				case PLAYERVSCOLLECTABLE:
					{
						if(m_CollisionEvents[i]->m_Collidable_A->getType()==COLLECTABLE)
						{
							if(m_CollisionEvents[i]->m_Collidable_A->isActive())
							{
								TestTankNode* tank = dynamic_cast<TestTankNode*>(m_CollisionEvents[i]->m_Collidable_B);
								tank->AddOffensiveUpgrade((OffensiveUpgrade*)m_CollisionEvents[i]->m_Collidable_A);
								//std::cout<<"PLAYERvsCOLLECTABLE"<<std::endl;
							}
						}
						else
						{
							if(m_CollisionEvents[i]->m_Collidable_B->isActive())
							{
								TestTankNode* tank = dynamic_cast<TestTankNode*>(m_CollisionEvents[i]->m_Collidable_A);
								tank->AddOffensiveUpgrade((OffensiveUpgrade*)m_CollisionEvents[i]->m_Collidable_B);
								//std::cout<<"COLLECTABLEvsPLAYER"<<std::endl;
							}
						}
					}break;
				//handles collision events between Scenery and projectiles
				case PROJECTILEVSSCENERY:
					{
						glm::vec3 normal = glm::vec3(m_CollisionEvents[i]->m_CollisionNormal.x,0.0f,m_CollisionEvents[i]->m_CollisionNormal.z);
						if(m_CollisionEvents[i]->m_Collidable_A->getType()==PROJECTILE)
						{
							ProjectileNode* tmp = dynamic_cast<ProjectileNode*>(m_CollisionEvents[i]->m_Collidable_A);
							//this selectes whether the angle is shallow enough for the bullet to reflect
							if(abs(glm::dot(normal,glm::normalize(tmp->getVelocity())))<0.707f)
							{
								//glm::vec3 correction = -glm::normalize(m_CollisionEvents[i]->m_Collidable_A->getVelocity())*(m_CollisionEvents[i]->m_Penetration);
								//glm::vec3 pos(m_CollisionEvents[i]->m_Collidable_A->getLocation());
								//m_CollisionEvents[i]->m_Collidable_A->setPosition(pos+correction);
								//leave a bullet bounce effect
								tmp->Bounce(normal);
							}
							else
							{
								tmp->deactivate();
								//leave death effect in it's place (future feature)
							}
						}
						else
						{
							ProjectileNode* tmp = dynamic_cast<ProjectileNode*>(m_CollisionEvents[i]->m_Collidable_B);
							//this selectes whether the angle is shallow enough for the bullet to reflect
							if(abs(glm::dot(normal,glm::normalize(tmp->getVelocity())))<0.707f)
							{
								glm::vec3 correction = -glm::normalize(m_CollisionEvents[i]->m_Collidable_B->getVelocity())*(m_CollisionEvents[i]->m_Penetration);
								glm::vec3 pos(m_CollisionEvents[i]->m_Collidable_B->getLocation());
								m_CollisionEvents[i]->m_Collidable_B->setPosition(pos+correction);
								//leave a bullet bounce effect
								tmp->Bounce(normal);
							}
							else
							{
								tmp->deactivate();
								//leave death effect in it's place (future feature)
							}
						}
					}break;
					//handles collision events between enemies and projectiles
				case ENEMYVSPROJECTILE:
				{
					if(m_CollisionEvents[i]->m_Collidable_A->getType() == PROJECTILE)
					{
						ProjectileNode* proj = dynamic_cast<ProjectileNode*>(m_CollisionEvents[i]->m_Collidable_A);
						Robot* robot = dynamic_cast<Robot*>(m_CollisionEvents[i]->m_Collidable_B);
						robot->dealDamage(proj->getDamage());
						proj->deactivate();
						//leave bullet death effect in its place
						if(robot->getHitPoints() == 0)
						{
							//update player score
							robot->deactivate();
							//leave non hazard death effect
						}
					}
					else
					{
						ProjectileNode* proj = dynamic_cast<ProjectileNode*>(m_CollisionEvents[i]->m_Collidable_B);
						Robot* robot = dynamic_cast<Robot*>(m_CollisionEvents[i]->m_Collidable_A);
						robot->dealDamage(proj->getDamage());
						proj->deactivate();
						//leave bullet death effect in it's place
						if(robot->getHitPoints() == 0)
						{
							//update player score
							robot->deactivate();
							//leave non hazard death effect
						}
					}
				}break;
				case ENEMYGENVSPROJECTILE:
				{
						RobotGenerator* gen = nullptr;
						ProjectileNode* proj = nullptr;
						if(m_CollisionEvents[i]->m_Collidable_A->getType() == PROJECTILE)
						{
							proj = dynamic_cast<ProjectileNode*>(m_CollisionEvents[i]->m_Collidable_A);
							gen = dynamic_cast<RobotGenerator*>(m_CollisionEvents[i]->m_Collidable_B);
							gen->dealDamage(proj->getDamage());
							proj->deactivate();
							//leave bullet death effect in its place
							if(gen->getHitPoints() == 0)
							{
								//update player score
								gen->deactivate();
								//leave non hazard death effect
							}
						}
						else
						{
							proj = dynamic_cast<ProjectileNode*>(m_CollisionEvents[i]->m_Collidable_B);
							gen = dynamic_cast<RobotGenerator*>(m_CollisionEvents[i]->m_Collidable_A);
							gen->dealDamage(proj->getDamage());
							proj->deactivate();
							//leave bullet death effect in it's place
							if(gen->getHitPoints() == 0)
							{
								//update player score
								gen->deactivate();
								//leave non hazard death effect
							}
						}
					}break;
					//handles collision events between enemies and scenery
				case ENEMYVSSCENERY:
					{
						if(m_CollisionEvents[i]->m_Collidable_A->getType() == ENEMY_ROBOT)
						{
							glm::vec3 normal = glm::vec3(m_CollisionEvents[i]->m_CollisionNormal.x,0.0f,m_CollisionEvents[i]->m_CollisionNormal.z);
							glm::vec3 velocity = m_CollisionEvents[i]->m_Collidable_A->getVelocity();
							glm::vec3 reflection = glm::reflect(velocity,normal);
							m_CollisionEvents[i]->m_Collidable_A->setPosition(m_CollisionEvents[i]->m_Collidable_A->getLocation()-glm::normalize(m_CollisionEvents[i]->m_Collidable_A->getVelocity())*(m_CollisionEvents[i]->m_Penetration));
							m_CollisionEvents[i]->m_Collidable_A->stop();
							m_CollisionEvents[i]->m_Collidable_A->setVelocity(glm::vec3(reflection.x,0.0f,reflection.z)*0.8f);
						}
						else
						{
							glm::vec3 normal = glm::vec3(m_CollisionEvents[i]->m_CollisionNormal.x,0.0f,m_CollisionEvents[i]->m_CollisionNormal.z);
							glm::vec3 reflection = glm::reflect(m_CollisionEvents[i]->m_Collidable_B->getVelocity(),glm::normalize(-normal))*0.8f;
							m_CollisionEvents[i]->m_Collidable_B->setPosition(m_CollisionEvents[i]->m_Collidable_B->getLocation()-glm::normalize(m_CollisionEvents[i]->m_Collidable_B->getVelocity())*(m_CollisionEvents[i]->m_Penetration));
							m_CollisionEvents[i]->m_Collidable_B->stop();
							m_CollisionEvents[i]->m_Collidable_B->setVelocity(glm::vec3(reflection.x,0.0f,reflection.z)*0.8f);

						}
					}break;
					//handles collisions between enemies and player controlled objects, this will be expanded to include different enemy types
				case PLAYERVSENEMY:
					{
						if(m_CollisionEvents[i]->m_Collidable_B->getType() == ENEMY_ROBOT)
						{
							Robot* robot = dynamic_cast<Robot*>(m_CollisionEvents[i]->m_Collidable_B);
							//m_Tank->dealDamage(robot->getDeathDamage());
							//leave death effect in it's place (future feature), hazard death effect will deal damage instead
							robot->deactivate();
						}
						if(m_CollisionEvents[i]->m_Collidable_B->getType() == ENEMY)
						{
							//if robogen is passive or alert, set to aggressive
						}
						if(m_CollisionEvents[i]->m_Collidable_A->getType() == ENEMY_ROBOT)
						{
							Robot* robot = dynamic_cast<Robot*>(m_CollisionEvents[i]->m_Collidable_A);
							//leave death effect in it's place (future feature), hazard death effect will deal damage instead
							//m_Tank->dealDamage(robot->getDeathDamage());
							robot->deactivate();
						}
						if(m_CollisionEvents[i]->m_Collidable_A->getType() == ENEMY)
						{
							//if robogen is passive or alert, set to aggressive
						}
						//calculate reflected vector
						glm::vec3 normal = glm::normalize(m_Tank->getLocation()-m_CollisionEvents[i]->m_Collidable_B->getLocation());
						glm::vec3 reflected = glm::reflect(m_Tank->getVelocity()*0.5f,normal);
						m_Tank->stop();
						m_Tank->setVelocity(glm::vec3(reflected.x,0.0f,reflected.z));
					}break;
			}
			m_CollisionEvents[i]->m_InUse = false;
			m_CollisionEvents[i]->m_Collided = false;
			m_CollisionEvents[i]->m_Collidable_A = nullptr;
			m_CollisionEvents[i]->m_Collidable_B = nullptr;
			m_CollisionEvents[i] = nullptr;
		}
	}
	m_CollisionEvents.clear();
	//Other game rules:
	// if no active generators remain, the maze is complete
	if(!m_RoboGens.empty())
	{
		for (std::list<RobotGenerator*>::iterator it = m_RoboGens.begin(); it != m_RoboGens.end(); it++)
		{
			//if robogen is ready to spawn, active OR aggressive, alert, and m_Tank is within aggression radius, spawn robots
			if((*it)->isActive() && (*it)->isReady() &&(*it)->getState()!=PassiveStatus && glm::distance(m_Tank->getLocation(),(*it)->getLocation()) <= 100.0f)
			{
				addRobot((*it)->getRobot());
			}
		}
	}
	else
	{
		m_Victory = true;
	}
	// if robogen is active and is alert OR agressive & is ready to spawn, spawn robots
	// if tank hp is depleted game is over
	if(m_Tank->getShieldHitPoints() <= 0)
	{
		//defeat condition met
		m_Defeat = true;
		//update hud
	}
}
}
void Scene::ProcessUserInput()
{
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
		//m_Camera->rotateYaw(m_MouseTurnSpeed*m_DeltaTimeSeconds);

		//m_Tank->rotateTurret(m_MouseTurnSpeed*m_DeltaTimeSeconds);
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
	m_Camera->rotateYaw(m_MouseTurnSpeed*m_DeltaTimeSeconds);
	m_Tank->rotateTurret(m_MouseTurnSpeed*m_DeltaTimeSeconds);
	if(m_Controller->getButtonState(FORWARD))
	{
		//move tank forward
		m_Tank->moveForward(m_DeltaTimeSeconds);
		//printf("forward\n");
	}
	if(m_Controller->getButtonState(BACKPEDAL))
	{
		//move tank back
		m_Tank->moveBack(m_DeltaTimeSeconds);
		//printf("back\n");
	}
	if(m_Controller->getButtonState(STRAFE_L))
	{
		m_Tank->moveLeft(m_DeltaTimeSeconds);
	}
	if(m_Controller->getButtonState(STRAFE_R))
	{
		m_Tank->moveRight(m_DeltaTimeSeconds);
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
		m_Tank->chargeMainGun(m_DeltaTimeSeconds);
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
		m_CurrentRenderMode = DEBUG;
	}
	if(m_Controller->getButtonState(RENDERMODE_DEFAULT))
	{
		m_CurrentRenderMode = FILLED;
	}
}
void Scene::addUpgradeFactory(UpgradeFactory* p_Factory)
{
	m_UpgradeFactory = p_Factory;
}
void Scene::addController(Controller* p_Controller)
{
	m_Controller = p_Controller;
}
void Scene::addRoboGenFactory(RoboGenFactory* p_Factory)
{
	m_RoboGenFactory = p_Factory;
}
void Scene::addFloorFactory(FloorFactory* p_Factory)
{
	m_FloorFactory = p_Factory;
}
void Scene::addTankFactory(BasicTankFactory* p_Factory)
{
	m_TankFactory = p_Factory;
}
void Scene::addWallFactory(WallFactory* p_Factory)
{
	m_WallFactory = p_Factory;
}
void Scene::render(Renderer* p_Renderer)
{
	p_Renderer->beginRenderCycle(m_CurrentRenderMode);
	m_Camera->render(p_Renderer);
	//draw static objects

	m_Tank->render(p_Renderer);
	//draw robot generators
	for(std::list<RobotGenerator*>::iterator it = m_RoboGens.begin(); it != m_RoboGens.end(); it++)
	{
		if((*it)->isActive()) //isActive returns a bool
		{
			(*it)->render(p_Renderer);
		}
	}
	//draw ammo boxes
	for(std::list<OffensiveUpgrade*>::iterator it = m_AmmoBoxes.begin(); it != m_AmmoBoxes.end(); it++)
	{
		if((*it)->isActive() && !(*it)->isCollected()) //isActive & isCollected returns a bool
		{
			(*it)->render(p_Renderer);
		}
	}
	//draw robots
	for(std::list<Robot*>::iterator it = m_Robots.begin(); it != m_Robots.end(); it++)
	{
		if((*it)->isActive()) //isActive returns a bool
		{
			(*it)->render(p_Renderer);
		}
	}
	//draw robots
	for(std::list<ProjectileNode*>::iterator it = m_Projectiles.begin(); it != m_Projectiles.end(); it++)
	{
		if((*it)->isActive()) //isActive returns a bool
		{
			(*it)->render(p_Renderer);
		}
	}
	//draw walls
	for (size_t i = 0; i <m_Walls.size();i++)
	{
		if(m_Walls[i]->isActive())
		{
			m_Walls[i]->render(p_Renderer);
		}
	} 
	//draw floors
	for (size_t i = 0; i <m_Floors.size();i++)
	{
		if(m_Floors[i]->isActive())
		{
			m_Floors[i]->render(p_Renderer);
		}
	} 

	//draw ui
	m_Hud->render(p_Renderer);
	p_Renderer->endRenderCycle();
}
void Scene::update(float p_DeltaTimeS)
{
	m_DeltaTimeSeconds = p_DeltaTimeS;

	if(!m_Victory && !m_Defeat)
	{
		detectCollisions();
		checkGameConditions();
		ProcessUserInput();
		m_Tank->update(m_DeltaTimeSeconds);	
		updateGameObjects();
		cleanup();
		m_Solver->update(m_DeltaTimeSeconds);
		m_Camera->LookAt(m_Tank->getLocation(),m_CamFollowDistance);
		m_Camera->moveUp(7.0f);
		m_Hud->setMetricCurrent(SHIELDGAUGE, m_Tank->getShieldHitPoints());
		m_Hud->setMetricCurrent(WEAPONGAUGE, m_Tank->getWeaponChargeLevel());
		m_Hud->setMetricCurrent(GENERATORDISPLAY, m_RoboGens.size());
		m_Hud->update(m_DeltaTimeSeconds);
	}
	m_Hud->setMetricCurrent(FRAMECOUNTER, m_DeltaTimeSeconds*CLOCKS_PER_SEC);
}
void Scene::cleanup()
{
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
	for(std::list<Robot*>::iterator it = m_Robots.begin(); it != m_Robots.end(); /*increment in body*/)
	{
		if(!(*it)->isActive()) //isActive returns a bool
		{
			it = m_Robots.erase(it);
		}
		else
		{
			++it;
		}
	}
	for(std::list<RobotGenerator*>::iterator it = m_RoboGens.begin(); it != m_RoboGens.end(); /*increment in body*/)
	{
		if(!(*it)->isActive()) //isActive returns a bool
		{
			it = m_RoboGens.erase(it);
		}
		else
		{
			++it;
		}
	}
	for(std::list<OffensiveUpgrade*>::iterator it = m_AmmoBoxes.begin(); it != m_AmmoBoxes.end(); /*increment in body*/)
	{
		if(!(*it)->isActive() || (*it)->isCollected()) //isActive & isCollected returns a bool
		{
			it = m_AmmoBoxes.erase(it);
		}
		else
		{
			++it;
		}
	}
}
Scene::~Scene(void)
{
	delete m_WallFactory;
	delete m_FloorFactory;
	delete m_TankFactory;
	delete m_RoboGenFactory;
	delete m_UpgradeFactory;
	delete m_Solver;
	delete m_AISolver;
	delete m_TestMaze;
	m_Tank = nullptr;
	m_Robots.clear();
	m_RoboGens.clear();
	m_Projectiles.clear();
	m_AmmoBoxes.clear();
	m_Walls.clear();
	m_Floors.clear();
	delete m_Camera;
}
