#include "RobotGenerator.h"
#include "MeshNode.h"
#include "TransformNode.h"
#include "TextureNode.h"
#include "Renderer.h"
#include <sstream>

RobotGenerator::RobotGenerator(void)
{
	m_DefaultRobotDiffuse = nullptr;
	m_DamagedRobotDiffuse = nullptr;
	m_DefaultRobotEmissive = nullptr;
	m_DamagedRobotEmissive = nullptr;
	m_DamagedRobotNormal = nullptr;
	m_DamagedRobotNormal = nullptr;
	m_DamagedRobotSpecular = nullptr;
	m_DefaultRobotSpecular = nullptr;
	m_DamagedDiffuseTexture = nullptr;
	m_RobotBodyMesh = nullptr;
	m_RobotHeadMesh = nullptr;
	m_RobotArmMesh = nullptr;
	m_Reusing = false;
	m_SpawnTimer = 0.0f;
	m_NumRobots = 0;
	m_SpawnDelay = 5.0f;
	m_BaseTimer = 5.0f;
	m_SpawnDistance = 10.0f;
	m_behaviourState = HostileStatus;
	m_InternalLightOffset=glm::vec3(0.0f,0.5f,0.0f);
}
bool RobotGenerator::isReady()
{
	return (m_SpawnTimer >= m_SpawnDelay);
}
void RobotGenerator::setDifficulty(const unsigned int p_Difficulty)
{
	m_Level = p_Difficulty;
	m_SpawnDelay = m_BaseTimer-(m_BaseTimer*0.01f)*m_Level;
}
void RobotGenerator::addInternalLight(LightNode *p_light)
{
	m_interalPointLight = p_light;
	m_interalPointLight->setPosition(m_InternalLightOffset);
}
void RobotGenerator::addInternalDamagedLight(LightNode *p_light)
{
	m_internalDamagedPointLight = p_light;
	m_internalDamagedPointLight->setPosition(m_InternalLightOffset);
}
void RobotGenerator::init()
{
	for (int i = 0; i < MAX_ROBOTS;i++)
	{
		Robot* tmp = new Robot();
		tmp->setType(ENEMY_ROBOT);
		tmp->setBoundaryType(CIRCLE);
		tmp->setRadius(5.0f);
		tmp->setBoundingRadius(5.0f);
		//visual data 
		tmp->addMesh(m_RobotBodyMesh);
		tmp->addTexture(m_DefaultRobotDiffuse);
		tmp->addTexture(m_DefaultRobotEmissive);
		tmp->addTexture(m_DefaultRobotNormal);
		tmp->addTexture(m_DefaultRobotSpecular);
		TransformNode* temp = new TransformNode();
		temp->reset();
		temp->setName("RobotTransform");
		tmp->addTransform(temp);
		//character data
		tmp->addHead(getHead());
		tmp->addLeftArm(getArm(true));
		tmp->addRightArm(getArm(false));
		tmp->addDamagedTexture(m_DamagedRobotDiffuse);
		tmp->addDamagedEmissive(m_DamagedEmissiveTexture);
		tmp->addDamagedNormal(m_DamagedNormalTexture);
		tmp->addDamagedSpecular(m_DamagedRobotSpecular);
		tmp->setDetectionRadius(400.0f);
		tmp->setStateTimer(0.1f);
		tmp->setPosition(glm::vec3(0.0f));
		m_Robots.push_front(tmp);
		tmp->deactivate();
	}
	m_interalPointLight->setParent(this);
	m_internalDamagedPointLight->setParent(this);
}
Robot* RobotGenerator::getRobotFromPool()
{
	if(m_Robots.empty())
	{
		return nullptr;
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
		return nullptr;
	}
}
Robot* RobotGenerator::getRobot()
{
	Robot* temp = nullptr;
	m_SpawnTimer = 0.0f;
	temp = getRobotFromPool();
	if(temp!=nullptr)
	{
		if(m_Reusing)
		{

			temp->setPosition(m_RobotSpawnPoint);
			//character data
			temp->setMaxHitPoints(200+10*m_Level);
			temp->setHitPoints(200+10*m_Level);
			temp->setDamageMultiplier(0.05f*m_Level);
			temp->activate();
		}
		else
		{
			return nullptr;
		}
		m_NumRobots++;
		std::stringstream ss;
		ss<<getName()<<"Robot"<<m_NumRobots;
		temp->setName(ss.str());
		ss.str(std::string());
		temp->reset();
	}
	return temp;
}
RobotArm* RobotGenerator::getArm(bool p_Left)
{
	RobotArm* arm = new RobotArm();
	arm->addMesh(m_RobotArmMesh);
	TransformNode* temp = new TransformNode();
	arm->setBoundingRadius(3.0f);
	temp->reset();
	
	arm->addTransform(temp);
	arm->activate();

	if(p_Left)
	{
		arm->setLeft();
		arm->setName("RobotLeftArm");
		temp->setName("leftArmTransform");
	}
	else
	{
		arm->setName("RobotRightArm");
		temp->setName("rightArmTransform");
	}

	arm->addTexture(m_DefaultRobotDiffuse);
	arm->addDamagedTexture(m_DamagedRobotDiffuse);
	arm->addTexture(m_DefaultRobotEmissive);
	arm->addTexture(m_DefaultRobotNormal);
	arm->addTexture(m_DefaultRobotSpecular);

	return arm;
}
RobotHead* RobotGenerator::getHead()
{
	RobotHead* head = new RobotHead();
	head->setBoundingRadius(3.0f);
	head->addMesh(m_RobotHeadMesh);
	head->addTexture(m_DefaultRobotDiffuse);
	head->addTexture(m_DefaultRobotEmissive);
	head->addTexture(m_DefaultRobotNormal);
	head->addTexture(m_DefaultRobotSpecular);
	head->addDamagedTexture(m_DamagedRobotDiffuse);
	TransformNode* temp = new TransformNode();
	temp->setName("HeadTransform");

	temp->reset();
	head->addTransform(temp);
	head->setName("RobotHead");
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
			m_RobotSpawnPoint = m_Position+(glm::normalize(m_targetPosition-m_Position)*m_SpawnDistance);
			//m_RobotSpawnPoint = m_Position+(glm::vec3(1.0f,0.0f,0.0f)*m_SpawnDistance);
		}
		m_LocalTransform->reset();
		m_LocalTransform->translate(m_Position+glm::vec3(0.0f,0.0f,0.0f));
		m_LocalTransform->rotate(m_OrientationDeg,glm::vec3(0.0f,1.0f,0.0f));
		m_LocalTransform->scale(m_Scale);
		SceneNode::update(p_DeltaTimeS);
		//m_interalPointLight->setPosition(m_InternalLightOffset);
		m_interalPointLight->update(p_DeltaTimeS);
		m_internalDamagedPointLight->update(p_DeltaTimeS);
		//m_internalDamagedPointLight->setPosition(m_InternalLightOffset);
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
		if(m_Mesh!=nullptr && m_DamagedDiffuseTexture!=nullptr && m_LocalTransform != nullptr)
		{
			p_Renderer->begin();
			m_DamagedDiffuseTexture->render(p_Renderer);
			m_DamagedEmissiveTexture->render(p_Renderer);
			m_DamagedNormalTexture->render(p_Renderer);
			m_DamagedSpecularTexture->render(p_Renderer);
			m_LocalTransform->render(p_Renderer);
			//any other texture render calls go in here
			m_Mesh->render(p_Renderer);
			p_Renderer->end();
			m_internalDamagedPointLight->render(p_Renderer);
		}
	}
	else
	{
		SceneNode::render(p_Renderer);
		m_WorldTransform;
		m_interalPointLight->render(p_Renderer);
	}
}
bool RobotGenerator::deleteAll(Robot* p_Robot)
{
	delete p_Robot;
	return true;
}
RobotGenerator::~RobotGenerator(void)
{
	m_DefaultRobotDiffuse = nullptr;
	m_DamagedRobotDiffuse = nullptr;
	m_DamagedDiffuseTexture = nullptr;
	m_RobotBodyMesh = nullptr;
	m_RobotHeadMesh = nullptr;
	m_RobotArmMesh = nullptr;
	m_DamagedDiffuseTexture = nullptr;
	while(!m_Robots.empty()) delete m_Robots.front(), m_Robots.pop_front();
}
