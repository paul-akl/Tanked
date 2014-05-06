#include "OffensiveUpgrade.h"
#include "MeshNode.h"
#include "TextureNode.h"
#include "TransformNode.h"
#include <sstream>
#include "LightNode.h"
OffensiveUpgrade::OffensiveUpgrade(void)
{
	m_NumObjects = 0;
	m_Reusing = false;
	m_ProjectileType = DEFAULT_MAIN;
}
void OffensiveUpgrade::init()
{
	ProjectileNode* v_Instance = nullptr;
	for (size_t i = 0; i < 10;i++)
	{
		v_Instance = new ProjectileNode();
		v_Instance->addMesh(m_DefaultProjectileMesh);
		v_Instance->addTexture(m_DefaultProjectileTexture);
		v_Instance->addTexture(m_DefaultProjEmissiveTexture);
		TransformNode* tmp = new TransformNode;
		tmp->setName("ProjectileTransform");
		v_Instance->addTransform(tmp);
		v_Instance->setBoundaryType(CIRCLE);
		v_Instance->setType(PROJECTILE);
		v_Instance->setProjectileType(DEFAULT_MAIN);
		v_Instance->setRadius(4.0f);
		v_Instance->setBoundingRadius(4.0f);
		v_Instance->setLifeTime(3.0f);
		v_Instance->setBaseDamage(50.0f);
		v_Instance->deactivate();
		LightNode* light = new LightNode();
		light->setColour(glm::vec3(1.0f,1.0f,0.5f));
		light->setAmbientIntensity(0.0f);
		light->setDiffuseIntensity(4.0f);
		light->setSpecularIntensity(1.0f);
		light->setSpecularPower(1.0f);
		light->setAttenuation(1.0f,0.0f,0.002f);
		light->setName("BulletLight");
		v_Instance->addLight(light);
		m_Projectiles.push_back(v_Instance);
	}
}
void OffensiveUpgrade::render(Renderer* p_Renderer)
{
	//nothing new here, so default upgrade behaviour
	UpgradeNode::render(p_Renderer);
}
void OffensiveUpgrade::update(float p_DeltaTimeS)
{
	//if not collected then it has a world representation, so update it's transform
	if(!m_Collected)
	{
		m_LocalTransform->reset();
		m_LocalTransform->translate(m_Position+glm::vec3(0.0f,m_Radius/2.0f,0.0f));
		m_LocalTransform->rotate(m_OrientationDeg,glm::vec3(0.0f,1.0f,0.0f));
		m_LocalTransform->scale(glm::vec3(m_Radius));
		UpgradeNode::update(p_DeltaTimeS);
	}
	//object has been collected, so switch to internal functionality
	else
	{
		m_TimeSinceLastShot+=p_DeltaTimeS;
		if(m_TimeSinceLastShot > m_SecondsPerShot)
			m_TimeSinceLastShot = m_SecondsPerShot;
	}
}
ProjectileNode* OffensiveUpgrade::getProjectile()
{
	if(isReadyToFire())
	{
		m_TimeSinceLastShot = 0.0f;
		ProjectileNode* v_Instance = createProjectileFromPool();
		if(v_Instance!=nullptr)
		{
			if(!m_Reusing)
			{
				v_Instance->reset();
				v_Instance->addMesh(m_DefaultProjectileMesh);
				v_Instance->addTexture(m_DefaultProjectileTexture);
				TransformNode* tmp = new TransformNode;
				tmp->setName("ProjectileTransform");
				v_Instance->addTransform(tmp);
				v_Instance->setBoundaryType(CIRCLE);
				v_Instance->setType(PROJECTILE);
				v_Instance->setProjectileType(DEFAULT_MAIN);
				v_Instance->setRadius(3.0f);
				v_Instance->setBoundingRadius(3.0f);
				v_Instance->setLifeTime(3.0f);
				v_Instance->setBaseDamage(50.0f);
				LightNode* light = new LightNode();
				light->setColour(glm::vec3(0.5f,1.0f,1.0f));
				light->setAmbientIntensity(0.0f);
				light->setDiffuseIntensity(10.0f);
				light->setSpecularIntensity(1.0f);
				light->setSpecularPower(1.0f);
				light->setAttenuation(1.0f,0.0f,0.001f);
				light->setName("BulletLight");
				v_Instance->addLight(light);
			}
			else
			{
				v_Instance->activate();
				v_Instance->setLifeTime(3.0f);
			}
			m_NumObjects++;
			std::stringstream ss;
			ss<<"MainProjectile"<<m_NumObjects;
			v_Instance->setName(ss.str());
			v_Instance->activate();
			return v_Instance;
		}
		else return nullptr;
	}
	else
	{
		return nullptr;
	}
}
bool OffensiveUpgrade::isReadyToFire()
{
	if(m_TimeSinceLastShot >= m_SecondsPerShot)
		return true;
	return false;
}
ProjectileNode* OffensiveUpgrade::createProjectileFromPool()
{
	ProjectileNode* temp = nullptr;
	//if projectile pool is empty
	if(m_Projectiles.empty())
	{
		temp = new ProjectileNode();
		m_Projectiles.push_back(temp);
		m_Reusing = false;
		return temp;
	}
	else
	{
		//search for an empty slot, or inactive object
		for (size_t i = 0; i < m_Projectiles.size();i++)
		{
			//if empty slot not found
			if (m_Projectiles[i] != nullptr)
			{
				//check if it's active
				if(!m_Projectiles[i]->isActive())
				{
					m_Reusing = true;
					return m_Projectiles[i];
				}
				else continue;
			}
			//empty slot found
			else
			{
				m_Projectiles[i] = new ProjectileNode;
				m_Reusing = false;
				return m_Projectiles[i];
			}
		}
		//if this block is reached, then we append the list with a new object
		temp = new ProjectileNode();
		m_Reusing = false;
		m_Projectiles.push_back(temp);
		return temp;
	}
}
//overrides SceneNode::activate();
void OffensiveUpgrade::activate()
{
	m_Collected = false;
	SceneNode::activate();
}
OffensiveUpgrade::~OffensiveUpgrade(void)
{
	while(!m_Projectiles.empty()) delete m_Projectiles.back(),m_Projectiles.pop_back();
}
