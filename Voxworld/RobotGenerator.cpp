#include "RobotGenerator.h"
#include "MeshNode.h"
#include "TransformNode.h"
#include "TextureNode.h"
#include "Renderer.h"
#include <sstream>

RobotGenerator::RobotGenerator(void)
{
	m_DefaultRobotDiffuseTexture = nullptr;
	m_DamagedRobotDiffuseTexture = nullptr;
	m_DamagedDiffuseTexture = nullptr;
	m_RobotBodyMesh = nullptr;
	m_RobotHeadMesh = nullptr;
	m_RobotArmMesh = nullptr;
	m_Reusing = false;
	m_SpawnTimer = 0.0f;
	m_NumRobots = 0;
	m_SpawnDelay = 1.5f;
	m_BaseTimer = 1.5f;
	m_SpawnDistance = 5.0f;
}
bool RobotGenerator::isReady()
{
	return (m_SpawnTimer>=m_SpawnDelay);
}
void RobotGenerator::setDifficulty(const unsigned int p_Difficulty)
{
	m_Level = p_Difficulty;
	m_SpawnDelay = m_BaseTimer-(m_BaseTimer*0.01f)*m_Level;
}
Robot* RobotGenerator::getRobotFromPool()
{
	if(m_Robots.empty())
	{
		Robot* tmp = new Robot();
		m_Robots.push_front(tmp);
		return tmp;
	}
	else
	{
		std::list<Robot*>::iterator it = m_Robots.begin();
		while(it !=m_Robots.end())
		{
			if(!(*it)->isActive())
			{
				m_Reusing = true;
				return *it;
			}
			else
			{
				it++;
			}
		}
		//if we reach this part, then we haven't found a reusable robot, so make a new one.
		m_Reusing = false;
		Robot* tmp = new Robot();
		m_Robots.push_front(tmp);
		return tmp;
	}
}
Robot* RobotGenerator::getRobot()
{
	Robot* temp = nullptr;
	temp = getRobotFromPool();
	{
		if(m_Reusing)
		{
			temp->activate();
		}
		else
		{
			//set up "on creation" data:
			//object type
			//collision data
			temp->setType(ENEMY_ROBOT);
			temp->setBoundaryType(CIRCLE);
			temp->setRadius(7.5f);
			//visual data 
			temp->addMesh(m_RobotBodyMesh);
			temp->addTexture(m_DefaultRobotDiffuseTexture);
			TransformNode* tmp = new TransformNode();
			tmp->reset();
			temp->addTransform(tmp);
			temp->setPosition(m_RobotSpawnPoint);
			//character data
			temp->addHead(getHead());
			temp->addLeftArm(getArm(true));
			temp->addRightArm(getArm(false));
			temp->setMaxHitPoints(200+10*m_Level);
			temp->setHitPoints(200+10*m_Level);
			temp->setDamageMultiplier(0.05f*m_Level);
			temp->addDamagedTexture(m_DamagedRobotDiffuseTexture);

			//mesh, texture & transform nodes
			//collision data: radius, position, collidable metric type 
			//character data: hp/max hp.
			//create robot parts
			//add mesh, texture and transform nodes to 
			
		}
		m_NumRobots++;
		std::stringstream ss;
		ss<<getName()<<"Robot"<<m_NumRobots;
		temp->setName(ss.str());
		ss.clear();
	}
	return temp;
}
RobotArm* RobotGenerator::getArm(bool p_Left)
{
	RobotArm* arm = new RobotArm();
	arm->addMesh(m_RobotArmMesh);
	TransformNode* temp = new TransformNode();
	temp->reset();
	arm->addTransform(temp);
	arm->activate();
	if(p_Left)
		arm->setLeft();
	return arm;
}
RobotHead* RobotGenerator::getHead()
{
	RobotHead* head = new RobotHead();
	head->addMesh(m_RobotHeadMesh);
	TransformNode* temp = new TransformNode();
	temp->reset();
	head->addTransform(temp);
	head->activate();
	return head;
}
void RobotGenerator::update(float p_DeltaTimeS)
{
	if(m_HitPoints > 0)
	{
		if(m_behaviourState!=PassiveStatus)
		{
			if(m_SpawnTimer<m_SpawnDelay)
			{
				m_SpawnTimer+=p_DeltaTimeS;
			}
			m_LocalTransform->reset();
			m_LocalTransform->translate(m_Position+glm::vec3(0.0f,9.0f,0.0f));
			m_LocalTransform->rotate(m_OrientationDeg,glm::vec3(0.0f,1.0f,0.0f));
			m_LocalTransform->scale(glm::vec3(m_Radius));
			SceneNode::update(p_DeltaTimeS);
			m_RobotSpawnPoint = m_Position+(glm::normalize(m_targetPosition-m_Position)*m_SpawnDistance);
		}
	}
	else
	{
		deactivate();
	}
}
void RobotGenerator::render(Renderer* p_Renderer)
{
	if(m_HitPoints < (m_MaxHitPoints*0.5f))
	{
		if(m_Mesh!=nullptr && m_DamagedRobotDiffuseTexture!=nullptr && m_LocalTransform != nullptr)
		{
			p_Renderer->begin();
			m_DamagedRobotDiffuseTexture->render(p_Renderer);
			m_LocalTransform->render(p_Renderer);
			//any other texture render calls go in here
			m_Mesh->render(p_Renderer);
			p_Renderer->end();
		}
	}
	else
	{
		SceneNode::render(p_Renderer);
	}
}
bool RobotGenerator::deleteAll(Robot* p_Robot)
{
	delete p_Robot;
	return true;
}
RobotGenerator::~RobotGenerator(void)
{
	m_DefaultRobotDiffuseTexture = nullptr;
	m_DamagedRobotDiffuseTexture = nullptr;
	m_DamagedDiffuseTexture = nullptr;
	m_RobotBodyMesh = nullptr;
	m_RobotHeadMesh = nullptr;
	m_RobotArmMesh = nullptr;
	m_DamagedDiffuseTexture = nullptr;
	while(!m_Robots.empty()) delete m_Robots.front(), m_Robots.pop_front();
}
