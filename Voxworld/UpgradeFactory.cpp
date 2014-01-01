#include "UpgradeFactory.h"
#include "MeshNode.h"
#include "TextureNode.h"
#include "TransformNode.h"
#include "AutoGun.h"
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
	m_DefaultProjectileMesh->loadModel("models/lowpolybullet.obj");
	m_DefaultProjectileMesh->setName("bulletDefaultMesh");
	m_DefaultProjectileTexture =  new TextureNode();
	m_DefaultProjectileTexture->loadTexture("images/bullet.png");
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
OffensiveUpgrade* UpgradeFactory::getInstanceFromPool(OffensiveUpgradeType p_Type)
{

	//object pool algorithm prioroties:
	//1. reuse objects
	//2. create new object if list not full and no spare slots
	//3. if list is full and no new slots, return nothing
	OffensiveUpgrade* temp = nullptr;
	//if pool has no objects
	if(m_OffensiveUpgrades.empty())
	{
		if(p_Type == DEFAULT)
		{
			temp = new OffensiveUpgrade();
			m_OffensiveUpgrades.push_back(temp);
		}
		if(p_Type == AUTOGUN)
		{
			temp = new AutoGun();
			m_OffensiveUpgrades.push_back(temp);
		}
		return temp;
	}
	//pool is not empty
	else
	{
		//search through list for a usable object or slot
		for(size_t i = 0; i < m_OffensiveUpgrades.size(); i++)
		{
			//if slot is in use
			if(m_OffensiveUpgrades[i] != nullptr)
			{
				//check if the object is not in use
				if(!m_OffensiveUpgrades[i]->isActive())
				{
					//if requested type matches current object, reuse the object
					if(p_Type == m_OffensiveUpgrades[i]->getUpgradeType())
					{
						m_Reusing = true;
						return 	m_OffensiveUpgrades[i];
					}
					//if requested type doesn't match
					else
					{
						//check if pool is at capacity. if so, then we need to reuse the slot
						if(m_OffensiveUpgrades.size() >= MAX_UPGRADES)
						{
							delete m_OffensiveUpgrades[i];
							//create object of requested type in current slot
							if(p_Type == DEFAULT)
							{
								temp = new OffensiveUpgrade();
								m_OffensiveUpgrades[i] = temp;
								return temp;
							}
							if(p_Type == AUTOGUN)
							{
								temp = new AutoGun();
								m_OffensiveUpgrades[i] = temp;
								return temp;
							}
						}
						//if the pool is not full, and no empty or unused slots are found, append a new object to the pool
						else
						{
							if(p_Type == DEFAULT)
							{
								temp = new OffensiveUpgrade();
								m_OffensiveUpgrades.push_back(temp);
								return temp;
							}
							if(p_Type == AUTOGUN)
							{
								temp = new AutoGun();
								m_OffensiveUpgrades.push_back(temp);
								return temp;
							}
						}
					}
				}
				//current object is in use, so skip to next object in pool
				else continue;
			}
			//if object slot is empty(unlikely), then we can create a new object and set it up
			else
			{
				if(p_Type == DEFAULT)
				{
					temp = new OffensiveUpgrade();
					m_OffensiveUpgrades[i] = temp;
					return temp;
				}
				if(p_Type == AUTOGUN)
				{
					temp = new AutoGun();
					m_OffensiveUpgrades[i] = temp;
					return temp;
				}
			}
		}
		//if we reach this block, a slot or object has not been found that can be used
		//so we append the list with a new one.
		if(m_OffensiveUpgrades.size() < MAX_UPGRADES)
		{
			if(p_Type == DEFAULT)
			{
				temp = new OffensiveUpgrade();
				m_OffensiveUpgrades.push_back(temp);
				return temp;
			}
			if(p_Type == AUTOGUN)
			{
				temp = new AutoGun();
				m_OffensiveUpgrades.push_back(temp);
				return temp;
			}
		}
		return nullptr;
	}
}
OffensiveUpgrade* UpgradeFactory::getOffensiveUpgrade(OffensiveUpgradeType p_Type)
{
	OffensiveUpgrade* v_Upgrade = getInstanceFromPool(p_Type);
	if(v_Upgrade!=nullptr)
	{
		//if the object has been reused
		if(m_Reusing)
		{
			//then just reactivate it
			v_Upgrade->activate();
		}
		//if object is new, it needs set up
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
		}
		//increment object creation count and set it's unique id, strings are slow, but functional. possible optimisation here.
		m_NumObjects++;
		std::stringstream ss;
		ss<<"OffensiveUpgrade"<<m_NumObjects;
		v_Upgrade->setName(ss.str());
		return v_Upgrade;
	}
	//this means pool is at capacity
	return nullptr;
}
UpgradeFactory::~UpgradeFactory(void)
{
	m_OffensiveUpgrades.clear();
	delete m_DefaultUpgradeMesh;
	delete m_DefaultUpgradeTexture;
	delete m_DefaultProjectileTexture;
	delete m_DefaultProjectileMesh;
}
