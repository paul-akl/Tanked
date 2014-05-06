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
	m_NumObjects = 0;
}
void RoboGenFactory::init()
{
	m_DefaultGeneratorTexture = new TextureNode();
	m_DefaultGeneratorTexture->loadTexture("images/RoboGen_d.png");
	m_DefaultGeneratorTexture->setTextureType(DIFFUSE);
	m_DefaultGeneratorTexture->setName("DefaultRoboGenDiffuse");
	m_DefaultGeneratorNormal = new TextureNode();
	m_DefaultGeneratorNormal->loadTexture("images/RoboGen_n.jpg");
	m_DefaultGeneratorNormal->setTextureType(NORMAL);
	m_DefaultGeneratorNormal->setName("DefaultRoboGenNormal");
	m_DefaultGeneratorEmissive = new TextureNode();
	m_DefaultGeneratorEmissive->loadTexture("images/RoboGen_glow.png");
	m_DefaultGeneratorEmissive->setTextureType(EMISSIVE);
	m_DefaultGeneratorEmissive->setName("DefaultRoboGenEmissive");
	m_DefaultGeneratorSpecular = new TextureNode();
	m_DefaultGeneratorSpecular->loadTexture("images/RoboGen_g.jpg");
	m_DefaultGeneratorSpecular->setTextureType(SPECULAR);
	m_DefaultGeneratorSpecular->setName("DefaultRoboGenSPecular");

	m_DamagedGeneratorTexture = new TextureNode();
	m_DamagedGeneratorTexture->loadTexture("images/RoboGenDmg_d.png");
	m_DamagedGeneratorTexture->setTextureType(DIFFUSE);
	m_DamagedGeneratorTexture->setName("DefaultRoboGenDamagedDiffuse");
	m_DamagedGeneratorNormal = new TextureNode();
	m_DamagedGeneratorNormal->loadTexture("images/RoboGenDmg_n.png");
	m_DamagedGeneratorNormal->setTextureType(NORMAL);
	m_DamagedGeneratorNormal->setName("DefaultRoboGenDamagedDiffuse");
	m_DamagedGeneratorEmissive = new TextureNode();
	m_DamagedGeneratorEmissive->loadTexture("images/RoboGenDmg_glow.png");
	m_DamagedGeneratorEmissive->setTextureType(EMISSIVE);
	m_DamagedGeneratorEmissive->setName("DefaultRoboGenDamagedDiffuse");
	m_DamagedGeneratorSpecular = new TextureNode();
	m_DamagedGeneratorSpecular->loadTexture("images/RoboGenDmg_g 2.png");
	m_DamagedGeneratorSpecular->setTextureType(SPECULAR);
	m_DamagedGeneratorSpecular->setName("DefaultRoboGenDamagedDiffuse");

	m_DamagedRobotTexture = new TextureNode();
	m_DamagedRobotTexture->loadTexture("images/DefaultDamagedRobot.png");
	m_DamagedRobotTexture->setTextureType(DIFFUSE);
	m_DamagedRobotTexture->setName("DefaultDamagedRobotDiffuse");
	m_DefaultRobotTexture = new TextureNode();
	m_DefaultRobotTexture->loadTexture("images/DefaultRobot.png");
	m_DefaultRobotTexture->setTextureType(DIFFUSE);
	m_DefaultRobotTexture->setName("DefaultRobotDiffuse");
	m_RoboGenMesh = new MeshNode();
	m_RoboGenMesh->loadModel("models/Plasma_RoboGen.obj");
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
		
		for(std::list<RobotGenerator*>::iterator it = m_RoboGens.begin(); it != m_RoboGens.end();it++)
		{
			if(!m_RoboGens.front()->isActive())
			{
				m_Reusing = true;
				return m_RoboGens.front();
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
				robogen->addDamagedEmissiveTexture(m_DamagedGeneratorEmissive);
				robogen->addDamagedNormalTexture(m_DamagedGeneratorNormal);
				robogen->addDamagedSpecularTexture(m_DamagedGeneratorSpecular);
				robogen->addTexture(m_DefaultGeneratorTexture);
				robogen->addMesh(m_RoboGenMesh);
				//set personal collision data
				robogen->setRadius(5.0f);
				robogen->setBoundingRadius(5.0f);
				robogen->setScale(glm::vec3(10.0f));
				robogen->setType(ENEMY);
				robogen->setBoundaryType(CIRCLE);
				TransformNode* tmp = new TransformNode();
				tmp->reset();
				robogen->addTransform(tmp);
				//set visual data for created robots
				robogen->setRobotDefaultTexture(m_DefaultRobotTexture);
				robogen->addTexture(m_DefaultGeneratorNormal);
				robogen->addTexture(m_DefaultGeneratorEmissive);
				robogen->addTexture(m_DefaultGeneratorSpecular);
				robogen->setRobotDamagedTexture(m_DamagedRobotTexture);
				robogen->setRobotBodyMesh(m_RoboBodyMesh);
				robogen->setRobotHeadMesh(m_RoboHeadMesh);
				robogen->setRobotArmMesh(m_RoboArmMesh);
				//set character data
				robogen->setHitPoints(200+10*p_Difficulty);
				robogen->setMaxHitPoints(200+10*p_Difficulty);
				robogen->setDifficulty(p_Difficulty);
				robogen->setDetectionRadius(300.0f);

				LightNode *internalLight = new LightNode();
				internalLight->setColour(glm::vec3(0.65f,0.92f,0.92f));
				internalLight->setAmbientIntensity(2.5f);
				internalLight->setDiffuseIntensity(1.0f);
				internalLight->setAttenuation(2.0f,0.0f,0.02f);
				internalLight->setName("RoboGen Internal Light");
				robogen->addInternalLight(internalLight);
				internalLight->activate();

				LightNode *internalDmgLight = new LightNode();
				internalDmgLight->setColour(glm::vec3(1.0f,0.0f,0.0f));
				internalDmgLight->setAmbientIntensity(2.5f);
				internalDmgLight->setDiffuseIntensity(10.0f);
				internalDmgLight->setAttenuation(2.0f,0.0f,0.04f);
				internalDmgLight->setName("RoboGen Internal Damaged Light");
				robogen->addInternalDamagedLight(internalDmgLight);
				internalDmgLight->activate();
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
	robogen->init();
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
