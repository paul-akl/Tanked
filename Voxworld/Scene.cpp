#include "Scene.h"


Scene::Scene(void)
{
}
void Scene::addTankNode(TestTankNode* p_Tank)
{
	m_Tanks.push_back(p_Tank);
	SceneNode::addNode(p_Tank);
}

void Scene::detectCollisions(CollisionSolver* p_Solver)
{
	for(size_t i = 0; i < m_Tanks.size();i++)
	{
		p_Solver->addCollidable(m_Tanks[i]);
	}
	//walls
	//for(size_t i = 0; i < m_Collectables.size();i++)
	//{
	//	p_Solver->addCollidable(m_Collectables[i]);
	//}
	////collectables
	////for(size_t i = 0; i < m_Collectables.size();i++)
	////{
	////	p_Solver->addCollidable(m_Tanks[i]);
	////}
	////enemies
	//for(size_t i = 0; i < m_Enemies.size();i++)
	//{
	//	p_Solver->addCollidable(m_Enemies[i]);
	//}
}
void Scene::render(Renderer* p_Renderer)
{
	SceneNode::render(p_Renderer);
}
void Scene::update(float p_DeltaTimeS)
{
	//game logic function.
	//ideally, code below will be abstracted away, within manager classes
	//float deltaTimeMS = deltaTime/1000.0f;
	//m_Tank->update(deltaTimeMS);
	//m_Ground->update(deltaTimeMS);
	////as will this:
	////query input buffer, and change gamestate accordingly
	////temporary code: will be reviewed
	////turn turret, will be switched to mouse movement
	//int mouseX = m_Controller->getMouseMovementX();
	//float panSpeed = 300.0f;
	//if(m_Controller->getButtonState(MOUSELEFT))
	//{
	//	m_Tank->rotateTurret(panSpeed*deltaTimeMS);
	//	m_Camera->rotateYaw(panSpeed*deltaTimeMS);
	//}
	//if(m_Controller->getButtonState(MOUSERIGHT))
	//{
	//	m_Tank->rotateTurret(-panSpeed*deltaTimeMS);
	//	m_Camera->rotateYaw(-panSpeed*deltaTimeMS);
	//}

	//if(m_Controller->getButtonState(FORWARD))
	//{
	//	//move tank forward
	//	m_Tank->moveForward(deltaTimeMS);
	//	//printf("forward\n");
	//}
	//if(m_Controller->getButtonState(BACKPEDAL))
	//{
	//	//move tank back
	//	m_Tank->moveBack(deltaTimeMS);
	//	//printf("back\n");
	//}
	//if(m_Controller->getButtonState(STRAFE_L))
	//{
	//	m_Tank->moveLeft(deltaTimeMS);
	//}
	//if(m_Controller->getButtonState(STRAFE_R))
	//{
	//	m_Tank->moveRight(deltaTimeMS);
	//	//printf("right\n");
	//	//move tank right
	//}
	//if(m_Controller->getButtonState(PRIMARY_ATTACK))
	//{
	//	//unsupported operation
	//	//fire tank automatic gun
	//}
	//if(m_Controller->getButtonState(SECONDARY_ATTACK))
	//{
	//	//unsupported operation
	//	//fire tank main gun
	//}
	////DEBUG CONTROLS
	//if(m_Controller->getButtonState(RENDERMODE_DEBUG))
	//{
	//	renderingMode = DEBUG;
	//}
	//if(m_Controller->getButtonState(RENDERMODE_DEFAULT))
	//{
	//	renderingMode = FILLED;
	//}

	//m_Camera->LookAt(m_Tank->getLocation(),m_CamFollowDistance);
	//m_Camera->moveUp(5.0f);
	////temprary code: will be reviewed
	SceneNode::update(p_DeltaTimeS);
}
Scene::~Scene(void)
{
}
