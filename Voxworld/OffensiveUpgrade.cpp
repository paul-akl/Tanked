#include "OffensiveUpgrade.h"
#include "MeshNode.h"
#include "TextureNode.h"
#include "TransformNode.h"
#include <sstream>
OffensiveUpgrade::OffensiveUpgrade(void)
{
	m_NumObjects = 0;
	m_Reusing = false;
}
void OffensiveUpgrade::init()
{

}
void OffensiveUpgrade::render(Renderer* p_Renderer)
{
	UpgradeNode::render(p_Renderer);
}
void OffensiveUpgrade::update(float p_DeltaTimeS)
{
	if(!m_Collected)
	{
		m_LocalTransform->reset();
		m_LocalTransform->translate(m_Position+glm::vec3(0.0f,m_Radius/2.0f,0.0f));
		m_LocalTransform->rotate(m_OrientationDeg,glm::vec3(0.0f,1.0f,0.0f));
		m_LocalTransform->scale(glm::vec3(m_Radius));
	}
	if(m_Collected)
	{
		m_TimeSinceLastShot+=p_DeltaTimeS;
		if(m_TimeSinceLastShot > m_SecondsPerShot)
			m_TimeSinceLastShot = m_SecondsPerShot;
	}
	else
	{
		UpgradeNode::update(p_DeltaTimeS);
	}
}
ProjectileNode* OffensiveUpgrade::getProjectile()
{
	if(isReadyToFire())
	{
		m_TimeSinceLastShot = 0.0f;

		ProjectileNode* v_Instance = createProjectileFromPool();
		if(m_Reusing)
		{
			v_Instance->addMesh(m_DefaultProjectileMesh);
			v_Instance->addTexture(m_DefaultProjectileTexture);
			TransformNode* tmp = new TransformNode;
			tmp->setName("ProjectileTransform");
			v_Instance->addTransform(tmp);
			v_Instance->setBoundaryType(CIRCLE);
			v_Instance->setType(PROJECTILE);
			v_Instance->setRadius(7.5f);
			v_Instance->setLifeTime(3.0f);
			v_Instance->setBaseDamage(10.0f);
		}
		std::stringstream ss;
		m_NumObjects++;
		ss<<"Projectile"<<m_NumObjects;
		v_Instance->setName(ss.str());
		v_Instance->activate();
		return v_Instance;

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
				if(m_Projectiles[i]->isActive())
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
	m_Projectiles.clear();
}
