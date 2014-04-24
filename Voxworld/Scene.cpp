#include "Scene.h"
#include "Renderer.h"
#include "NotificationFactory.h"
#include "Locator.h"
#include "TransformNode.h"
#include <glm\glm.hpp>
#include <iostream>
#include <ctime>
#include <typeinfo>

Scene::Scene(void)
{
	m_CurrentRenderMode = FILLED;
	m_CamFollowDistance = -25.0f;
	m_CamFollowAngle = -25.0f;
	m_MaxTurnSpeed = 350.0f;
	m_MouseTurnSpeed = -100.0f;
	m_MouseAcceleration = 50.0f;
	m_MainFireButtonPressed=false;
	m_MoveKeyPressed = false;
	movesoundplaying = false;
	chargesoundplaying = false;
	m_Victory = false;
	m_Defeat = false;
	m_Paused = false;
	m_PausePressed = false;
	m_Tank = nullptr;
	m_Audio = Locator::getAudioSystem();
	m_DeltaTimeSeconds = 0.0f;
	m_TestMaze = nullptr;
	m_AISolver = nullptr;
	m_Solver = nullptr;
	m_WallFactory = nullptr;
	m_FloorFactory = nullptr;
	m_TankFactory = nullptr;
	m_UpgradeFactory = nullptr;
	m_RoboGenFactory = nullptr;
	m_Hud = nullptr;
	m_Tank = nullptr;
	m_Controller = nullptr;
	m_Camera = nullptr;
	m_PlayerScore = 0;
	m_GameDifficulty = 0;
	projThread = nullptr;
	enemyThread = nullptr;
	enemyGenThread = nullptr;
	collectThread = nullptr;
	SceneryThread = nullptr;
	floorThread = nullptr;
}
bool Scene::sceneComplete()
{
	if(m_Victory)
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
	int worldWidth = m_TestMaze->getGridWidth();
	m_Camera = new CameraNode();
	m_Camera->rotatePitch(m_CamFollowAngle);
	m_Camera->moveForward(m_CamFollowDistance);
	m_Camera->moveUp(3.0f);
	m_GameDifficulty = 1;
	m_PlayerScore = 0;
	m_TestMaze->generateMaze(20, 20, 30);
	m_TestMaze->toConsole();
	//m_TestMaze->toConsole();
	m_AISolver->setMaze(m_TestMaze);
	m_AISolver->init();
	m_Solver->init((float)m_TestMaze->getGridWidth(),glm::vec3(m_TestMaze->getGridWidth()*0.5f,0.0f,m_TestMaze->getGridHeight()*0.5f));
	MazeIterator *v_mazeIterator = m_TestMaze->getIterator();
	int num = 0;
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
				if (num < 1)
				{
					addRobotGenerator(m_TestMaze->getCellPosition(cell));
					num++;
				}
			}break;
		case START:
			{
				addTank(m_TestMaze->getCellPosition(cell));
			}break;
		};
	}
	addFloor(glm::vec3(m_TestMaze->getGridWidth() / 2, 0.0f, m_TestMaze->getGridHeight() / 2), (float)m_TestMaze->getNumRows(), (int)m_TestMaze->getGridWidth());
	m_Hud = new HUD();
	m_Hud->setName("mainhud");
	m_Hud->setPosition(glm::vec3(0));
	m_Hud->setOrientation(0);
	m_Hud->setScale(glm::vec3(1.0));
	m_Hud->init();
	m_Hud->setMetricMax(SHIELDGAUGE, m_Tank->getMaxShieldHitPoints());
	m_Hud->setMetricMax(WEAPONGAUGE, m_Tank->getMaxWeaponChargeLevel());
	m_Hud->setMetricCurrent(SCORE,0);

	m_Audio->setMusicVolume(0.1f);
	m_Audio->Music(TRACK3).play(1.0f);

	printf("init scene complete");
	
}
void Scene::nextLevel()
{
	//clear world entity lists
	for(std::list<ProjectileNode*>::iterator it = m_Projectiles.begin();it!= m_Projectiles.end();)
	{
		(*it)->deactivate();
		it = m_Projectiles.erase(it);
	}
	for(std::list<Robot*>::iterator it = m_Robots.begin();it!= m_Robots.end();)
	{
		(*it)->deactivate();
		it = m_Robots.erase(it);
	}
	for(std::list<UpgradeNode*>::iterator it = m_Upgrades.begin();it!= m_Upgrades.end();)
	{
		(*it)->deactivate();
		it = m_Upgrades.erase(it);
	}
	for(std::list<RobotGenerator*>::iterator it = m_RoboGens.begin();it!= m_RoboGens.end();)
	{
		(*it)->deactivate();
		it = m_RoboGens.erase(it);
	}
	for(std::list<OffensiveUpgrade*>::iterator it = m_AmmoBoxes.begin();it!= m_AmmoBoxes.end();)
	{
		(*it)->deactivate();
		it = m_AmmoBoxes.erase(it);
	}
	for(std::vector<WallNode*>::iterator it = m_Walls.begin();it!= m_Walls.end();)
	{
		(*it)->deactivate();
		it = m_Walls.erase(it);
	}
	//clear area effect & particle system lists
	//only one floor used
	m_Floors[0]->deactivate();
	m_Floors.clear();
	m_Solver->reset();
	printf("init scene");

	//will be changed upon integration of maze node
	//addTank(mazeObject->getStartPoint());
	//add 4 robot generators to world according to maze
	//add walls and floors according to maze

	m_GameDifficulty++;
	m_TestMaze->generateMaze(25+m_GameDifficulty, 25+m_GameDifficulty, 30);
	int worldWidth = m_TestMaze->getGridWidth();

	m_TestMaze->toConsole();

	m_Solver->init((float)m_TestMaze->getGridWidth(),glm::vec3(m_TestMaze->getGridWidth()*0.5f,0.0f,m_TestMaze->getGridHeight()*0.5f));
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
	addFloor(glm::vec3(m_TestMaze->getGridWidth() / 2, 0.0f, m_TestMaze->getGridHeight() / 2), (float)m_TestMaze->getNumRows(), (int)m_TestMaze->getGridWidth());
	printf("init scene complete");
	m_Defeat = false;
	m_Victory = false;
	//clear all object lists, reset collision & ai solver
	//increment level variable, set paused to true
	//reinitialise level
	//generate maze
	//set maze to ai solver
	//get maze data
	//construct level from walls, generators, floor
}
void Scene::addTank(glm::vec3 p_Location)
{
	if(m_Tank==nullptr)
	{
		m_Tank = m_TankFactory->getTank();
		OffensiveUpgrade* upgrade = m_UpgradeFactory->getOffensiveUpgrade(AUTOGUN);
		upgrade->init();
		m_Tank->AddOffensiveUpgrade(upgrade);
		upgrade = m_UpgradeFactory->getOffensiveUpgrade(DEFAULT);
		upgrade->init();
		m_Tank->AddOffensiveUpgrade(upgrade);
	}
	m_Tank->setPosition(p_Location);
	m_Tank->setRadius(6.0f);
	//SceneNode::addNode(m_Tank);
	m_Solver->addTank(m_Tank);
	m_AISolver->setPlayer(m_Tank);

}
void Scene::addFloor(glm::vec3 p_Location, float p_wallWidth, int p_gridWidth)
{
	FloorNode* floor = m_FloorFactory->getFloor(p_gridWidth, p_wallWidth);
	floor->setPosition(p_Location);
	float rad = sqrt(((p_gridWidth/2.0f)*(p_gridWidth/2.0f))+((p_gridWidth/2.0f)*(p_gridWidth/2.0f)));
	floor->setBoundingRadius(rad);
	m_Floors.push_back(floor);
}
void Scene::addWall(glm::vec3 p_Location)
{
	WallNode* wall = m_WallFactory->getInstance();
	wall->setWidth(30.0f);
	wall->setRadius(15.0f);
	wall->setLength(30.0f);
	wall->setPosition(p_Location);
	// magic number represents hypotenuse of a 15 x 15 triangle very dirty must change!!!!
	wall->setBoundingRadius(21.24f);
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
	if(p_Robot!=nullptr)
	{
		m_Robots.push_front(p_Robot);
		m_Solver->addEnemy(p_Robot);
	}
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
void Scene::addRenderer(Renderer* p_Renderer)
{
	m_Renderer = p_Renderer;
}
void Scene::updateGameObjects()
{
	//multi-threading candidates
	int num;
	bool hasEnemies = false;
	//create thread data blocks for new threads
	ThreadDBlock projblock, enemBlock,enemyGenBlock;
	ThreadEnemyBlock enemyBlock;
	ThreadCDBlock collectblock;
	ThreadSceneryBlock floorBlock, wallBlock;

	//populate data blocks and create threads.
	projblock.list = (std::list<SceneNode*>*)&m_Projectiles;
	projblock.deltaTimeS = m_DeltaTimeSeconds;
	projThread = SDL_CreateThread(updateObjects,"proj thread",&projblock);

	wallBlock.vector = (std::vector<SceneNode*>*)&m_Walls;
	wallBlock.deltaTimeS = m_DeltaTimeSeconds;
	SceneryThread = SDL_CreateThread(updateScenery,"wall thread", &wallBlock);

	if(!m_Robots.empty())
	{
		hasEnemies = true;
		enemyBlock.deltaTimeS = m_DeltaTimeSeconds;
		enemyBlock.list = (std::list<EnemyNode*>*)&m_Robots;
		enemyBlock.maze=m_TestMaze;
		enemyBlock.player = m_Tank;
		enemyBlock.solver=m_AISolver;
		enemyThread = SDL_CreateThread(updateEnemies,"enemy thread",&enemyBlock);
	}
	
	enemyGenBlock.deltaTimeS = m_DeltaTimeSeconds;
	enemyGenBlock.list = (std::list<SceneNode*>*)&m_RoboGens;
	enemyGenThread = SDL_CreateThread(updateObjects,"robogen thread",&enemyGenBlock);

	collectblock.deltaTimeS = m_DeltaTimeSeconds;
	collectblock.list = (std::list<UpgradeNode*>*)&m_AmmoBoxes;
	collectThread = SDL_CreateThread(updateCollectables,"proj thread",&collectblock);

	floorBlock.deltaTimeS = m_DeltaTimeSeconds;
	floorBlock.vector = (std::vector<SceneNode*>*)&m_Floors;
	floorThread = SDL_CreateThread(updateScenery,"floor Thread", &floorBlock);
	m_Renderer->endRenderCycle();

	//signal OS to wait until threads are complete before moving on.
	if(hasEnemies)
		SDL_WaitThread(enemyThread, &num);
	SDL_WaitThread(projThread, &num);
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
int Scene::updateEnemies(void* p_EnemBlock)
{
	ThreadEnemyBlock* block = static_cast<ThreadEnemyBlock*>(p_EnemBlock);
	std::list<EnemyNode*>::iterator it = block->list->begin();	
	std::list<EnemyNode*>::iterator jt = block->list->end();
	for(;it!=jt; it++)
	{
		(*it)->update(block->deltaTimeS);
		block->solver->doBehaviour((*it));
	}

	return 0;
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
								m_Audio->Effect(TANKCOLLISION).play();
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
								m_Audio->Effect(TANKCOLLISION).play();
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
								m_Audio->Effect(PICKUP).play();
								TestTankNode* tank = dynamic_cast<TestTankNode*>(m_CollisionEvents[i]->m_Collidable_B);
								tank->AddOffensiveUpgrade((OffensiveUpgrade*)m_CollisionEvents[i]->m_Collidable_A);
								//std::cout<<"PLAYERvsCOLLECTABLE"<<std::endl;
							}
						}
						else
						{
							if(m_CollisionEvents[i]->m_Collidable_B->isActive())
							{
								m_Audio->Effect(PICKUP).play();
								TestTankNode* tank = dynamic_cast<TestTankNode*>(m_CollisionEvents[i]->m_Collidable_A);
								tank->AddOffensiveUpgrade(dynamic_cast<OffensiveUpgrade*>(m_CollisionEvents[i]->m_Collidable_B));
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
								m_Audio->Effect(RICOCHET).play(0.1f);
								//leave a bullet bounce effect
								tmp->Bounce(normal);
							}
							else
							{
								m_Audio->Effect(PROJECTILECOLLISION).play(0.1f);
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
								m_Audio->Effect(RICOCHET).play(0.1f);
								//leave a bullet bounce effect
								tmp->Bounce(normal);
							}
							else
							{
								m_Audio->Effect(PROJECTILECOLLISION).play(0.1f);
								tmp->deactivate();
								//leave a bullet bounce effect
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
						robot->dealDamage((unsigned int)proj->getDamage());
						proj->deactivate();
						m_Audio->Effect(PROJECTILEWITHENEMY).play();
						//leave bullet death effect in its place
						if(robot->getHitPoints() <= 0)
						{
							m_Audio->Effect(EXPLOSION).play();
							//update player score
							robot->deactivate();
							m_PlayerScore+=25.0f;
							//leave non hazard death effect
						}
					}
					else
					{
						ProjectileNode* proj = dynamic_cast<ProjectileNode*>(m_CollisionEvents[i]->m_Collidable_B);
						Robot* robot = dynamic_cast<Robot*>(m_CollisionEvents[i]->m_Collidable_A);
						robot->dealDamage((unsigned int)proj->getDamage());
						proj->deactivate();
						m_Audio->Effect(PROJECTILEWITHENEMY).play();
						//leave bullet death effect in it's place
						if(robot->getHitPoints() <= 0)
						{
							m_Audio->Effect(EXPLOSION).play();
							//update player score
							m_PlayerScore+=25;
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
							gen->dealDamage((unsigned int)proj->getDamage());
							proj->deactivate();
							m_Audio->Effect(PROJECTILEWITHENEMY).play();
							//leave bullet death effect in its place
							if(gen->getHitPoints() <= 0)
							{
								m_Audio->Effect(EXPLOSION).play();
								//update player score
								gen->deactivate();
								m_PlayerScore+=50;
								//leave non hazard death effect
							}
						}
						else
						{
							proj = dynamic_cast<ProjectileNode*>(m_CollisionEvents[i]->m_Collidable_B);
							gen = dynamic_cast<RobotGenerator*>(m_CollisionEvents[i]->m_Collidable_A);
							gen->dealDamage((unsigned int)proj->getDamage());
							proj->deactivate();
							m_Audio->Effect(PROJECTILEWITHENEMY).play();
							//leave bullet death effect in it's place
							if(gen->getHitPoints() <= 0)
							{
								m_Audio->Effect(EXPLOSION).play();
								//update player score
								m_PlayerScore+=50.0f;
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
							m_CollisionEvents[i]->m_Collidable_A->stop();
							//m_CollisionEvents[i]->m_Collidable_A->setPosition(m_CollisionEvents[i]->m_Collidable_A->getLocation()-glm::normalize(m_CollisionEvents[i]->m_Collidable_A->getVelocity())*(m_CollisionEvents[i]->m_Penetration));

							m_CollisionEvents[i]->m_Collidable_A->setVelocity(glm::vec3(reflection.x,0.0f,reflection.z)*0.8f);
						}
						else
						{
							glm::vec3 normal = glm::vec3(m_CollisionEvents[i]->m_CollisionNormal.x,0.0f,m_CollisionEvents[i]->m_CollisionNormal.z);
							glm::vec3 reflection = glm::reflect(m_CollisionEvents[i]->m_Collidable_B->getVelocity(),glm::normalize(-normal))*0.8f;
							//m_CollisionEvents[i]->m_Collidable_B->setPosition(m_CollisionEvents[i]->m_Collidable_B->getLocation()-glm::normalize(m_CollisionEvents[i]->m_Collidable_B->getVelocity())*(m_CollisionEvents[i]->m_Penetration));
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
							m_Tank->dealDamage((int)robot->getDamage());
							m_Audio->Effect(EXPLOSION).play();
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
							m_Tank->dealDamage((int)robot->getDamage());
							m_Audio->Effect(EXPLOSION).play();
							robot->deactivate();
						}
						if(m_CollisionEvents[i]->m_Collidable_A->getType() == ENEMY)
						{
							//if robogen is passive or alert, set to aggressive
							RobotGenerator* robot = dynamic_cast<RobotGenerator*>(m_CollisionEvents[i]->m_Collidable_A);
							robot->setState(HostileStatus);
						}
						//calculate reflected vector
						glm::vec3 reflected = glm::reflect(m_Tank->getVelocity()*2.0f,m_CollisionEvents[i]->m_CollisionNormal);
						m_Tank->stop();
						m_Tank->setVelocity(glm::vec3(reflected.x,0.0f,reflected.z));
					}break;
				//enemy hitting enemy , circle/circle collision response, no game events
				case ENEMYVSENEMY:
				{

					glm::vec3 normal = glm::vec3(m_CollisionEvents[i]->m_CollisionNormal.x,0.0f,m_CollisionEvents[i]->m_CollisionNormal.z);
					glm::vec3 velocity = m_CollisionEvents[i]->m_Collidable_A->getVelocity();
					glm::vec3 velocity2 = m_CollisionEvents[i]->m_Collidable_B->getVelocity();
					glm::vec3 reflection1 = glm::reflect(velocity,normal);
					glm::vec3 reflection2 = glm::reflect(velocity2,normal);
					if(m_CollisionEvents[i]->m_Collidable_A->isMoving())
					{
						m_CollisionEvents[i]->m_Collidable_A->stop();
						m_CollisionEvents[i]->m_Collidable_A->setVelocity(glm::vec3(reflection1.x,0.0f,reflection1.z)*0.8f);
						m_CollisionEvents[i]->m_Collidable_B->stop();
						m_CollisionEvents[i]->m_Collidable_B->setVelocity(glm::vec3(reflection2.x,0.0f,reflection2.z)*0.8f);
					}
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
	}
	//Other game rules:
	// if no active generators remain, the maze is complete
	if(!m_RoboGens.empty())
	{
		for (std::list<RobotGenerator*>::iterator it = m_RoboGens.begin(); it != m_RoboGens.end(); it++)
		{
			//if robogen is ready to spawn, active OR aggressive, alert, and m_Tank is within aggression radius, spawn robots
			if((*it)->getState()!=PassiveStatus && (*it)->isActive() && (*it)->isReady())
			{
					addRobot((*it)->getRobot());
			}
			//if robogen is ready to spawn, active OR aggressive, alert, and m_Tank is within aggression radius, spawn robots
			//if((*it)->isActive() && (*it)->getState()==PassiveStatus && glm::distance(m_Tank->getLocation(),(*it)->getLocation()) <= 200.0f)
			//{
			//	(*it)->setState(AlertStatus);
			//}
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
void Scene::ProcessUserInput()
{
	////////////////////////////////////////////////////////////////////////
	//////////				 USER INPUT           //////////////////////////
	////////////////////////////////////////////////////////////////////////
	if (!m_Controller->getButtonState(PAUSE))
	{
		if(m_PausePressed)
		{
			if(m_Paused)
			{
				m_Audio->musicChannel()->setPaused(false);
				m_Audio->effectChannel()->setPaused(false);
				
				m_Paused = false;
				m_PausePressed = false;
			}
			else
			{
				m_Audio->musicChannel()->setPaused(true);
				m_Audio->effectChannel()->setPaused(true);
				m_Paused = true;
				m_PausePressed = false;
			}
		}
	}
	else
	{
		m_PausePressed = true;
	}
	if(!m_Paused)
	{
		int mouseXmove = m_Controller->getMouseMovementX();

		if(m_Controller->getButtonState(MOUSELEFT))
		{
			//if(m_MouseTurnSpeed<m_MaxTurnSpeed)
			//	m_MouseTurnSpeed+=m_MouseAcceleration;
			//else
			//	m_MouseTurnSpeed = m_MaxTurnSpeed;
			m_Camera->rotateYaw(m_MouseTurnSpeed*mouseXmove*m_DeltaTimeSeconds);

			m_Tank->rotateTurret(m_MouseTurnSpeed*mouseXmove*m_DeltaTimeSeconds);
		}

		else if(m_Controller->getButtonState(MOUSERIGHT))
		{
			m_Camera->rotateYaw(m_MouseTurnSpeed*mouseXmove*m_DeltaTimeSeconds);

			m_Tank->rotateTurret(m_MouseTurnSpeed*mouseXmove*m_DeltaTimeSeconds);

		}
		//m_Camera->rotateYaw(m_MouseTurnSpeed*mouseXmove*m_DeltaTimeSeconds);
		//m_Tank->rotateTurret(m_MouseTurnSpeed*mouseXmove*m_DeltaTimeSeconds);
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
				m_Audio->Effect(TANKAUTOGUN).play(0.8f);
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
					m_Audio->Effect(TANKCANNON).play(0.8f);
					m_Projectiles.push_front(tmp);
					m_Solver->addProjectile(tmp);
				}
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
	m_Controller->resetMouse();
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
	if(!m_Paused)
	{
		m_Camera->render(p_Renderer);
		p_Renderer->updateViewFrustum();
		p_Renderer->beginRenderCycle(m_CurrentRenderMode);
	
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
		//p_Renderer->endRenderCycle();

		if(!NotificationFactory::m_NotificationList.empty())
		for(std::list<Notification*>::iterator it = NotificationFactory::m_NotificationList.begin(); it != NotificationFactory::m_NotificationList.end(); it++)
			(*it)->render(p_Renderer);
	}
}

void Scene::update(float p_DeltaTimeS)
{
	m_DeltaTimeSeconds = p_DeltaTimeS;
	//if game ending conditions are not met
	ProcessUserInput();

	if(!m_Victory && !m_Defeat)
	{
		//and game is not paused
		if(!m_Paused)
		{
			detectCollisions();
			m_Tank->update(m_DeltaTimeSeconds);	
			//if(!m_Robots.empty())
			//{
			//	for (std::list<Robot*>::iterator it = m_Robots.begin(); it!= m_Robots.end();it++)
			//	{
			//		if((*it)->isActive())
			//		{
			//			m_AISolver->doBehaviour((*it));
			//		}
			//	}
			//}

			if( m_Controller->getButtonState(FORWARD) || 
				m_Controller->getButtonState(BACKPEDAL) || 
				m_Controller->getButtonState(STRAFE_L) || 
				m_Controller->getButtonState(STRAFE_R) )  
				m_MoveKeyPressed = true; 
			else 
				m_MoveKeyPressed = false;
							
			if(m_MoveKeyPressed && !movesoundplaying)
			{				
				
				m_Audio->Effect(TANKMOVEMENT).playLooping(0.3f);				
				movesoundplaying = true;
			}	
			if(!m_Tank->isMoving())
			{
				m_Audio->Effect(TANKMOVEMENT).Stop();
				movesoundplaying = false;
			}
			
			if(m_MainFireButtonPressed && !chargesoundplaying)
			{
				m_Audio->Effect(WEAPONCHARGE).playLooping(1.0f);
				chargesoundplaying = true;
			}
			
			if(!m_MainFireButtonPressed || m_Tank->getWeaponChargeLevel() == m_Tank->getMaxWeaponChargeLevel())
			{
				m_Audio->Effect(WEAPONCHARGE).Stop();
				chargesoundplaying = false;
			}
			
			updateGameObjects();
			checkGameConditions();

			cleanup();
			m_Solver->update(m_DeltaTimeSeconds);
			m_Camera->LookAt(m_Tank->getLocation(),m_CamFollowDistance);
			m_Camera->moveUp(7.0f);
			m_Hud->setMetricCurrent(SHIELDGAUGE, m_Tank->getShieldHitPoints());
			m_Hud->setMetricCurrent(WEAPONGAUGE, m_Tank->getWeaponChargeLevel());
			m_Hud->setMetricCurrent(GENERATORDISPLAY, (float)m_RoboGens.size());
			m_Hud->update(m_DeltaTimeSeconds);
			m_Hud->setMetricCurrent(FRAMECOUNTER, m_DeltaTimeSeconds*CLOCKS_PER_SEC);
			m_Hud->setMetricCurrent(SCORE,m_PlayerScore);

		}
	}

	if(!NotificationFactory::m_NotificationList.empty())
	{
		// loop through notifications and check if their duration has timed out, if so erase from list, else update
		for(std::list<Notification*>::iterator it = NotificationFactory::m_NotificationList.begin(); it != NotificationFactory::m_NotificationList.end();)
			if ((*it)->timeOut())
			{
				it = NotificationFactory::m_NotificationList.erase((it));
			}
			else
			{
				(*it)->update(m_DeltaTimeSeconds);
				it++;
			}		
	}

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
