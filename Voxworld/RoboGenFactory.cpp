#include "RoboGenFactory.h"
#include "TextureNode.h"
#include "MeshNode.h"
#include "TransformNode.h"
#include <sstream>

RoboGenFactory::RoboGenFactory(void)
{
	m_DefaultGeneratorTexture = nullptr;
	m_DamagedGeneratorTexture= nullptr;
	m_DamagedRobotTexture= nullptr;
	m_DefaultRobotTexture= nullptr;
	m_RoboGenMesh= nullptr;
	m_RoboHeadMesh= nullptr;
	m_RoboArmMesh= nullptr;
	m_RoboBodyMesh= nullptr;
	m_Reusing = false;
}
void RoboGenFactory::init()
{
	m_DefaultGeneratorTexture = new TextureNode();
	m_DefaultGeneratorTexture->loadTexture("images/RoboGenDefault.png");
	m_DefaultGeneratorTexture->setTextureType(DIFFUSE);
	m_DefaultGeneratorTexture->setName("DefaultRoboGenDiffuse");
	m_DamagedGeneratorTexture = new TextureNode();
	m_DamagedGeneratorTexture->loadTexture("images/RoboGenDamaged.png");
	m_DamagedGeneratorTexture->setTextureType(DIFFUSE);
	m_DamagedGeneratorTexture->setName("DefaultRoboGenDamagedDiffuse");
	m_DamagedRobotTexture = new TextureNode();
	m_DamagedRobotTexture->loadTexture("images/DefaultDamagedRobot.png");
	m_DamagedRobotTexture->setTextureType(DIFFUSE);
	m_DamagedRobotTexture->setName("DefaultDamagedRobotDiffuse");
	m_DefaultRobotTexture = new TextureNode();
	m_DefaultRobotTexture->loadTexture("images/DefaultRobot.png");
	m_DefaultRobotTexture->setTextureType(DIFFUSE);
	m_DefaultRobotTexture->setName("DefaultRobotDiffuse");
	m_RoboGenMesh = new MeshNode();
	m_RoboGenMesh->loadModel("models/RoboGen.obj");
	m_RoboGenMesh->setName("RoboGenMesh");
	m_RoboHeadMesh = new MeshNode();
	m_RoboHeadMesh->loadModel("models/RobotHead.obj");
	m_RoboHeadMesh->setName("RobotHead");
	m_RoboArmMesh = new MeshNode();
	m_RoboArmMesh->loadModel("models/RobotArm.obj");
	m_RoboArmMesh->setName("RobotArm");
	m_RoboBodyMesh = new MeshNode();
	m_RoboBodyMesh->loadModel("models/RobotBody.obj");
	m_RoboBodyMesh->setName("RobotBody");

}
RobotGenerator* RoboGenFactory::getRoboGenFromPool()
{
	RobotGenerator* robogen = nullptr;
	if(m_RoboGens.empty())
	{
		m_Reusing = false;
		robogen = new RobotGenerator();
		m_RoboGens.push_front(robogen);
		return robogen;
	}
	else
	{
		std::list<RobotGenerator*>::iterator it = m_RoboGens.begin();
		while(it != m_RoboGens.end())
		{
			if(!(*it)->isActive())
			{
				m_Reusing = true;
				return (*it);
			}
		}
		m_Reusing = false;
		robogen = new RobotGenerator();
		m_RoboGens.push_front(robogen);
		return robogen;
	}
}
RobotGenerator* RoboGenFactory::getRobotGenerator(unsigned int p_Difficulty, RobotType p_Type)
{
	RobotGenerator* robogen = getRoboGenFromPool();
	switch(p_Type)
	{
	case ROBOT_DEFAULT:
		{
			if(!m_Reusing)
			{
				//add "personal" visual data
				robogen->addDamagedDiffuseTexture(m_DamagedGeneratorTexture);
				robogen->addTexture(m_DefaultGeneratorTexture);
				robogen->addMesh(m_RoboGenMesh);
				//set personal collision data
				robogen->setRadius(5.0f);
				robogen->setType(ENEMY);
				robogen->setBoundaryType(CIRCLE);
				TransformNode* tmp = new TransformNode();
				tmp->reset();
				robogen->addTransform(tmp);
				//set visual data for created robots
				robogen->setRobotDefaultTexture(m_DefaultRobotTexture);
				robogen->setRobotDamagedTexture(m_DamagedRobotTexture);
				robogen->setRobotBodyMesh(m_RoboBodyMesh);
				robogen->setRobotHeadMesh(m_RoboHeadMesh);
				robogen->setRobotArmMesh(m_RoboArmMesh);
				//set character data
				robogen->setHitPoints(200+10*p_Difficulty);
				robogen->setMaxHitPoints(200+10*p_Difficulty);
				robogen->setDifficulty(p_Difficulty);
			}

		}break;
	default:
		{
			return nullptr;
		}break;
	}
	robogen->activate();
	m_NumObjects++;
	std::stringstream ss;
	ss<<"RobotGenerator"<<m_NumObjects;
	robogen->setName(ss.str());
	ss.clear();
	return robogen;

}
RoboGenFactory::~RoboGenFactory(void)
{
	delete m_DefaultGeneratorTexture;
	delete m_DamagedGeneratorTexture;
	delete m_DamagedRobotTexture;
	delete m_DefaultRobotTexture;
	delete m_RoboGenMesh;
	delete m_RoboHeadMesh;
	delete m_RoboArmMesh;
	delete m_RoboBodyMesh;
	while(!m_RoboGens.empty()) delete m_RoboGens.front(),m_RoboGens.pop_front();
}
