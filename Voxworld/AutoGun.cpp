#include "AutoGun.h"
#include "TransformNode.h"
#include <sstream>
#include "ProjectileNode.h"

AutoGun::AutoGun(void)
{
	m_ProjectileType = DEFAULT_SECONDARY;
	m_SecondsPerShot=0.2f;
	m_TimeSinceLastShot = 0.0f;
}
ProjectileNode* AutoGun::getProjectile()
{
	if(isReadyToFire())
	{
		m_TimeSinceLastShot = 0.0f;
		ProjectileNode* v_Instance = createProjectileFromPool();
		if(v_Instance!=nullptr)
		{
			if(!m_Reusing)
			{
				v_Instance->addMesh(m_DefaultProjectileMesh);
				v_Instance->addTexture(m_DefaultProjectileTexture);
				TransformNode* tmp = new TransformNode;
				tmp->reset();
				tmp->setName("ProjectileTransform");
				v_Instance->addTransform(tmp);
				v_Instance->setBoundaryType(CIRCLE);
				v_Instance->setProjectileType(DEFAULT_SECONDARY);
				v_Instance->setType(PROJECTILE);
				v_Instance->setRadius(1.0f);
				v_Instance->setLifeTime(3.0f);
				v_Instance->setBaseDamage(10.0f);
			}
			v_Instance->activate();
			m_NumObjects++;
			std::stringstream ss;
			ss<<"AutoGunProjectile"<<m_NumObjects;
			v_Instance->setLifeTime(3.0f);
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
ProjectileNode* AutoGun::createProjectileFromPool()
{
	ProjectileNode* temp = nullptr;
	//if projectile pool is empty
	if(m_Projectiles.empty())
	{
		temp = new ProjectileNode();
		//printf("creating from empty\n");
		m_Reusing = false;
		m_Projectiles.push_back(temp);
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
				if(!(m_Projectiles[i]->isActive()))
				{
					m_Reusing = true;
					//printf("re-using projectile\n");
					return m_Projectiles[i];
				}
				else continue;
			}
			//empty slot found
			else
			{
				m_Projectiles[i] = new ProjectileNode;
				m_Reusing = false;
				//printf("filling slot\n");
				return m_Projectiles[i];
			}
		}
		//if this block is reached, then we append the list with a new object
		temp = new ProjectileNode();
		//printf("creating from full\n");
		m_Reusing = false;
		m_Projectiles.push_back(temp);
		return temp;
	}
}
void AutoGun::init()
{
	for (size_t i = 0; i < MAX_PROJECTILES;i++)
	{
		ProjectileNode* temp = new ProjectileNode();
		temp->addMesh(m_DefaultProjectileMesh);
		temp->addTexture(m_DefaultProjectileTexture);
		TransformNode* tmp = new TransformNode;
		tmp->reset();
		tmp->setName("ProjectileTransform");
		temp->addTransform(tmp);
		temp->setBoundaryType(CIRCLE);
		temp->setProjectileType(DEFAULT_SECONDARY);
		temp->setType(PROJECTILE);
		temp->setRadius(1.5f);
		temp->setLifeTime(3.0f);
		temp->setBaseDamage(10.0f);
		temp->deactivate();
		m_Projectiles.push_back(temp);
	}
}
void AutoGun::render(Renderer* p_Renderer)
{
	//nothing new here, so default upgrade behaviour
	UpgradeNode::render(p_Renderer);
}
void AutoGun::update(float p_DeltaTimeS)
{
	OffensiveUpgrade::update(p_DeltaTimeS);
}
AutoGun::~AutoGun(void)
{

}
