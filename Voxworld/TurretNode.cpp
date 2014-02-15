#include "TurretNode.h"
#include "TransformNode.h"
#include "SceneNode.h"

TurretNode::TurretNode(void)
{
	m_MuzzleOffset = glm::vec3(0.0f,3.0f,0.0f);
}
void TurretNode::render(Renderer *p_Renderer)
{
	SceneNode::render(p_Renderer);
}
void TurretNode::update(float p_DeltaTimeS)
{
	if(m_OrientationDeg > 360.0f)
	{
		m_OrientationDeg-=360.0f;
	}
	else if (m_OrientationDeg < 0.0f)
	{
		m_OrientationDeg+=360.0f;
	}
	if(!m_MainGunUpgrades.empty())
	{
		if (m_MainGunUpgrades.top()->getNumShots() == 0)
		{
			m_MainGunUpgrades.top()=nullptr;
			m_MainGunUpgrades.pop();
		}
	}
	//update local transform
	m_LocalTransform->reset();
	m_LocalTransform->rotate(-m_Parent->getOrientation(),glm::vec3(0.0,1.0,0.0));
	m_LocalTransform->translate(m_Position);
	m_LocalTransform->rotate(m_OrientationDeg,glm::vec3(0.0,1.0,0.0));
	m_LocalTransform->scale(glm::vec3(1.0));
	//then perform default behaviour
	SceneNode::update(p_DeltaTimeS);
	m_AutoGun->update(p_DeltaTimeS);
	if(!m_MainGunUpgrades.empty())
	m_MainGunUpgrades.top()->update(p_DeltaTimeS);
}
void TurretNode::rotateTurret(float p_Rotation)
{
	 m_OrientationDeg+=p_Rotation;
}
void TurretNode::setAutoGun(OffensiveUpgrade* p_Upgrade)
{
	m_AutoGun = p_Upgrade;
}
void TurretNode::setMainGun(OffensiveUpgrade* p_Upgrade)
{
	m_MainGunUpgrades.push(p_Upgrade);
}
ProjectileNode* TurretNode::getAutoGunProjectile()
{
	//get a bullet, initialize it and return in
	ProjectileNode* temp = m_AutoGun->getProjectile();
	if(temp!=nullptr)
	{
		//temp->setParent(m_Parent);
		temp->setOrientation(m_OrientationDeg);
		glm::vec3 offset(glm::normalize(temp->getVelocity()));
		temp->setPosition(m_Parent->getLocation()+offset*10.0f);
		temp->setDamageMultiplier(1.0f);
	}
	return temp;
}
ProjectileNode* TurretNode::getMainGunProjectile()
{
	//get a bullet, initialize it and return in
	if(!m_MainGunUpgrades.empty())
	{
		ProjectileNode* temp = m_MainGunUpgrades.top()->getProjectile();
		temp->setOrientation(m_OrientationDeg);
		glm::vec3 offset(glm::normalize(temp->getVelocity()));
		temp->setPosition(m_Parent->getLocation()+offset*10.0f);

		return temp;
	}
	else return nullptr;
}
TurretNode::~TurretNode(void)
{
	for (size_t i = 0; i < m_MainGunUpgrades.size();i++)
	{
		if(m_MainGunUpgrades.top() != nullptr)
		{
			m_MainGunUpgrades.top()->deactivate();
			m_MainGunUpgrades.top() = nullptr;
		}
		m_MainGunUpgrades.pop();
	}
	if(m_AutoGun!=nullptr)
	{
		m_AutoGun->deactivate();
		m_AutoGun = nullptr;
	}
}
