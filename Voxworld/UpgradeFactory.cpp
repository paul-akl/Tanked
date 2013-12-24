#include "UpgradeFactory.h"
#include "MeshNode.h"
#include "TextureNode.h"
#include "TransformNode.h"
#include <sstream>
UpgradeFactory::UpgradeFactory(void)
{
	m_NumObjects = false;
	m_Reusing = false;
}
void UpgradeFactory::init()
{
	//assets for the objects the upgrade will create
	m_DefaultProjectileMesh =  new MeshNode();
	m_DefaultProjectileMesh->loadModel("images/Bullet.obj");
	m_DefaultProjectileMesh->setName("bulletDefaultMesh");
	m_DefaultProjectileTexture =  new TextureNode();
	m_DefaultProjectileTexture->loadTexture("images/Bullet_Texture.png");
	m_DefaultProjectileTexture->setTextureType(DIFFUSE);
	m_DefaultProjectileTexture->setName("bulletDefaultTexture");
	//assets for the upgrade itself
	m_DefaultUpgradeMesh = new MeshNode();
	m_DefaultUpgradeMesh->loadModel("models/crate.obj");
	m_DefaultUpgradeMesh->setName("defaultupgrademesh");

	m_DefaultUpgradeTexture = new TextureNode();
	m_DefaultUpgradeTexture->loadTexture("images/ammo.png");
	m_DefaultUpgradeTexture->setTextureType(DIFFUSE);
	m_DefaultUpgradeTexture->setName("defaultupgradetexture");
}
OffensiveUpgrade* UpgradeFactory::getInstanceFromPool()
{
	OffensiveUpgrade* temp = nullptr;
	if(m_OffensiveUpgrades.empty())
	{
		temp = new OffensiveUpgrade();
		return temp;
	}
	else
	{
		for(size_t i = 0; i < m_OffensiveUpgrades.size(); i++)
		{
			//search for an empty slot, for whatever reason
			if(m_OffensiveUpgrades[i] != nullptr)
			{
				//if no slot empty slot found yet, check if the object is inactive
				//if not, see if we can reuse the object
				if(!m_OffensiveUpgrades[i]->isActive())
				{
					m_Reusing = true;
					return 	m_OffensiveUpgrades[i];
				}
				//else skip to next object in list
				else continue;
			}
			//if object slot is empty, then we can create a new object and set it up
			else
			{
				m_OffensiveUpgrades[i] = new OffensiveUpgrade();
				return m_OffensiveUpgrades[i];
			}
		}
		//if we reach this block, a slot or object has not been found that can be used
		//so we append the list with a new one.
		temp = new OffensiveUpgrade();
		return temp;
		
	}
}
OffensiveUpgrade* UpgradeFactory::getOffensiveUpgrade()
{
	OffensiveUpgrade* v_Upgrade = getInstanceFromPool();
	if(m_Reusing)
	{
		m_NumObjects++;
		v_Upgrade->activate();
	}
	else
	{
		TransformNode* tmp = new TransformNode();
		tmp->reset();
		v_Upgrade->addTransform(tmp);
		v_Upgrade->setProjectileDiffuse(m_DefaultProjectileTexture);
		v_Upgrade->setProjectileMesh(m_DefaultProjectileMesh);
		v_Upgrade->activate();
		v_Upgrade->setType(COLLECTABLE);
		v_Upgrade->setBoundaryType(CIRCLE);
		v_Upgrade->addMesh(m_DefaultUpgradeMesh);
		v_Upgrade->addTexture(m_DefaultUpgradeTexture);
		v_Upgrade->setLifeTimeS(30.0f);
		m_NumObjects++;

	}
	std::stringstream ss;
	ss<<"OffensiveUpgrade"<<m_NumObjects;
	v_Upgrade->setName(ss.str());
	return v_Upgrade;
}
UpgradeFactory::~UpgradeFactory(void)
{
	m_OffensiveUpgrades.clear();
	delete m_DefaultUpgradeMesh;
	delete m_DefaultUpgradeTexture;
	delete m_DefaultProjectileTexture;
	delete m_DefaultProjectileMesh;
}
