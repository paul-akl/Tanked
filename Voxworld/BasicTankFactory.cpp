#include "BasicTankFactory.h"
#include "TextureNode.h"
#include "MeshNode.h"
#include "TransformNode.h"
#include "TestTankNode.h"
#include "TurretNode.h"
#include "UpgradableTank.h"
BasicTankFactory::BasicTankFactory(void)
{
	m_NumInstances=0;

}
void BasicTankFactory::init()
{
	//set up tank mesh and texture nodes. These are uniform across all tanks
	m_BodyDiffuse1 = new TextureNode();
	m_BodyDiffuse1->setTextureType(DIFFUSE);
	m_BodyDiffuse1->setName("tank0diffuse0");
	m_BodyDiffuse1->loadTexture("images/Tank_D.tga");
	
	m_BodyEmissive1 = new TextureNode();
	m_BodyEmissive1->setTextureType(EMISSIVE);
	m_BodyEmissive1->setName("tank0emissive0");
	m_BodyEmissive1->loadTexture("images/Tank_Glow.tga");

	m_BodyNormal1 = new TextureNode();
	m_BodyNormal1->setTextureType(NORMAL);
	m_BodyNormal1->setName("tank0normal0");
	m_BodyNormal1->loadTexture("images/Tank_N.tga");

	m_BodyMesh1 = new MeshNode();
	m_BodyMesh1->loadModel("models/Tank_base.obj");
	//now set up uniform tank mesh and texture
	m_TurretMesh1  = new MeshNode();
	m_TurretMesh1->setName("turret0mesh0");
	m_TurretMesh1->loadModel("models/Tank_turret.obj");
}
TestTankNode* BasicTankFactory::getTank(void)
{
	//as only one tank will be used at any time, we will limit instances to one
	//if we have not created a tank yet
	if(m_NumInstances==0)
	{
		//make a tank trannsform node
		TransformNode* tankTransform = new TransformNode();
		LightNode* light = new LightNode();
		light->setColour(glm::vec3(1.0f,0.2f,0.2f));
		light->setAmbientIntensity(0.0f);
		light->setDiffuseIntensity(10.0f);
		light->setSpecularIntensity(0.75f);
		light->setSpecularPower(1.0f);
		light->setAttenuation(2.0f,2.0f,0.2f);
		light->setName("tank Hoverlight");
		SpotLight* spot = new SpotLight();
		spot->setColour(glm::vec3(0.8f,0.8f,1.0f));
		spot->setAmbientIntensity(0.0f);
		spot->setDiffuseIntensity(10.0f);
		spot->setSpecularIntensity(0.75f);
		spot->setSpecularPower(1.0f);
		spot->setAttenuation(1.0f,0.0f,0.004f);
		spot->setCutoffAngle(45.0f);
		//set it's name
		tankTransform->setName("tank0transform0");
		//and set it to identity matrix
		tankTransform->reset();
		//instantiate a tank node
		m_Instance =new TestTankNode;
		//set collidability parameters
		m_Instance->setRadius(10.0f);
		m_Instance->setType(PLAYER);
		//no heirarchical collision boundaries
		m_Instance->setRecursive(false);
		//set name
		m_Instance->setName("tank0");
		//set to two metres off the ground
		m_Instance->setPosition(glm::vec3(0.0f,0.0f,0.0f));
		m_Instance->addTransform(tankTransform);
		//this might be better to add to the mesh instead of the tank itself.
		//This way, multiple meshes can be added to a tank.
		m_Instance->addTexture(m_BodyDiffuse1);
		m_Instance->addTexture(m_BodyEmissive1);
		m_Instance->addTexture(m_BodyNormal1);
		m_Instance->addMesh(m_BodyMesh1);
		m_Instance->setOrientation(0.0f);
		m_Instance->setBoundaryType(CIRCLE);
		m_Instance->setType(PLAYER);
		m_Instance->setHoverLight(light);
		//turret factory code
		TurretNode* turret = new TurretNode();
		TransformNode* turtrans = new TransformNode();
		turtrans->reset();
		turret->addMesh(m_TurretMesh1);
		turret->addTexture(m_BodyDiffuse1);
		turret->addTexture(m_BodyEmissive1);
		turret->addTexture(m_BodyNormal1);
		turret->setSearchLight(spot);
		turret->addTransform(turtrans);
		//turret->setOrientation(180.0f);
		turret->setBoundingRadius(10.0f);
		m_Instance->setBoundingRadius(10.0f);
		turret->setName("turret0");
		turret->setPosition(glm::vec3(0.0f,1.5f,0.0f));
		m_Instance->addTurretNode(turret);
		//end turret factory code
		m_Instance->setPosition(glm::vec3(1.0,1.0f,1.0f));
		m_Instance->activate();
		m_NumInstances = 1;
		return m_Instance;
	}
	//if we have created a tank
	else
	{
		//and it is active
		if(m_Instance->isActive())
		{
			//return it, as we are still using it
			return m_Instance;
		}
		//else it is not in use
		else
		{
			//so delete it, and create a new one, then return it
			delete m_Instance;
			//make a tank trannsform node
			TransformNode* tankTransform = new TransformNode();
			//set it's name
			tankTransform->setName("tank0transform0");
			//and set it to identity matrix
			tankTransform->reset();
			//instantiate a tank node
			m_Instance =new TestTankNode();
			//set collidability parameters
			m_Instance->setRadius(10.0f);
			m_Instance->setType(PLAYER);
			//no heirarchical collision boundaries
			m_Instance->setRecursive(false);
			//set name
			m_Instance->setName("tank0");
			//set to two metres off the ground
			m_Instance->setPosition(glm::vec3(0.0f,1.5f,0.0f));
			m_Instance->addTransform(tankTransform);
			//this might be better to add to the mesh instead of the tank itself.
			//This way, multiple meshes can be added to a tank.
			m_Instance->addTexture(m_BodyDiffuse1);
			m_Instance->addMesh(m_BodyMesh1);
			m_Instance->setOrientation(0.0f);
			//turret factory code
			TurretNode* turret = new TurretNode();
			TransformNode* turtrans = new TransformNode();
			turtrans->reset();
			turret->addMesh(m_TurretMesh1);
			turret->addTexture(m_BodyDiffuse1);
			turret->addTransform(turtrans);
			//turret->setOrientation(180.0f);
			turret->setName("turret0");
			turret->setPosition(glm::vec3(0.0f,1.5f,0.0f));
			m_Instance->addTurretNode(turret);
			//end turret factory code
			m_Instance->setPosition(glm::vec3(1.0,1.0f,1.0f));
			return m_Instance;
		}
	}
}
BasicTankFactory::~BasicTankFactory(void)
{
	//cleanup
	if(m_NumInstances<0)
	{
		delete m_Instance;
	}
	delete m_BodyDiffuse1;
	delete m_BodyMesh1;

}
