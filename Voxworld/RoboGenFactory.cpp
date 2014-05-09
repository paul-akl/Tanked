#include "RoboGenFactory.h"
#include "TextureNode.h"
#include "MeshNode.h"
#include "TransformNode.h"
#include <sstream>

RoboGenFactory::RoboGenFactory(void)
{
	m_DefaultGeneratorTexture = nullptr;
	m_DamagedGeneratorTexture= nullptr;
	m_DamagedRobotDiffuse= nullptr;
	m_DefaultRobotDiffuse= nullptr;
	m_RoboGenMesh= nullptr;
	m_RoboHeadMesh= nullptr;
	m_RoboArmMesh= nullptr;
	m_RoboBodyMesh= nullptr;
	m_Reusing = false;
	m_NumObjects = 0;
}
void RoboGenFactory::init()
{
	//assets for robot generator
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
		//load assets for robot
	m_DamagedRobotDiffuse = new TextureNode();
	m_DamagedRobotDiffuse->loadTexture("images/DefaultDamagedRobot.png");
	m_DamagedRobotDiffuse->setTextureType(DIFFUSE);
	m_DamagedRobotDiffuse->setName("DefaultDamagedRobotDiffuse");
	m_DefaultRobotDiffuse = new TextureNode();
	m_DefaultRobotDiffuse->loadTexture("images/DefaultRobot.png");
	m_DefaultRobotDiffuse->setTextureType(DIFFUSE);
	m_DefaultRobotDiffuse->setName("DefaultRobotDiffuse");

	m_DamagedRobotEmissive = new TextureNode();
	m_DamagedRobotEmissive->loadTexture("images/DefaultRobot_glow.png");
	m_DamagedRobotEmissive->setTextureType(EMISSIVE);
	m_DamagedRobotEmissive->setName("DefaultDamagedRobotEmissive");
	m_DefaultRobotEmissive = new TextureNode();
	m_DefaultRobotEmissive->loadTexture("images/DefaultRobot_glow.png");
	m_DefaultRobotEmissive->setTextureType(EMISSIVE);
	m_DefaultRobotEmissive->setName("DefaultRobotEmissive");

	m_DamagedRobotSpecular = new TextureNode();
	m_DamagedRobotSpecular->loadTexture("images/DefaultRobot_g.png");
	m_DamagedRobotSpecular->setTextureType(SPECULAR);
	m_DamagedRobotSpecular->setName("DefaultDamagedRobotSpecular");
	m_DefaultRobotSpecular = new TextureNode();
	m_DefaultRobotSpecular->loadTexture("images/DefaultRobot_g.png");
	m_DefaultRobotSpecular->setTextureType(SPECULAR);
	m_DefaultRobotSpecular->setName("DefaultRobotSpecular");

	m_DamagedRobotNormal = new TextureNode();
	m_DamagedRobotNormal->loadTexture("images/DefaultRobot_n.png");
	m_DamagedRobotNormal->setTextureType(NORMAL);
	m_DamagedRobotNormal->setName("DefaultDamagedRobotNormal");
	m_DefaultRobotNormal = new TextureNode();
	m_DefaultRobotNormal->loadTexture("images/DefaultRobot_n.png");
	m_DefaultRobotNormal->setTextureType(NORMAL);
	m_DefaultRobotNormal->setName("DefaultRobotNormal");

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
				robogen->addTexture(m_DefaultGeneratorNormal);
				robogen->addTexture(m_DefaultGeneratorEmissive);
				robogen->addTexture(m_DefaultGeneratorSpecular);

				robogen->setRobotDefaultTexture(m_DefaultRobotDiffuse);
				robogen->setRobotDamagedTexture(m_DamagedRobotDiffuse);
				robogen->setRobotDefaultEmissive(m_DefaultRobotEmissive);
				robogen->setRobotDamagedEmissive(m_DamagedRobotEmissive);
				robogen->setRobotDefaultNormal(m_DefaultRobotNormal);
				robogen->setRobotDamagedNormal(m_DamagedRobotNormal);
				robogen->setRobotDefaultSpecular(m_DefaultRobotSpecular);
				robogen->setRobotDamagedSpecular(m_DamagedRobotSpecular);

				robogen->setRobotBodyMesh(m_RoboBodyMesh);
				robogen->setRobotHeadMesh(m_RoboHeadMesh);
				robogen->setRobotArmMesh(m_RoboArmMesh);


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
	//set character data
	robogen->setHitPoints(200+10*p_Difficulty);
	robogen->setMaxHitPoints(200+10*p_Difficulty);
	robogen->setDifficulty(p_Difficulty);
	robogen->setDetectionRadius(300.0f);
	robogen->init();
	return robogen;

}
RoboGenFactory::~RoboGenFactory(void)
{
	delete m_DefaultGeneratorTexture;
	delete m_DamagedGeneratorTexture;
	delete m_DamagedRobotDiffuse;
	delete m_DefaultRobotDiffuse;
	delete m_RoboGenMesh;
	delete m_RoboHeadMesh;
	delete m_RoboArmMesh;
	delete m_RoboBodyMesh;
	while(!m_RoboGens.empty()) delete m_RoboGens.front(),m_RoboGens.pop_front();
}
