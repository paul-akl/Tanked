#include "TurretNode.h"
#include "TransformNode.h"
#include "SceneNode.h"

TurretNode::TurretNode(void)
{
}
void TurretNode::render(Renderer *p_Renderer)
{
	SceneNode::render(p_Renderer);
}
void TurretNode::update(float p_DeltaTimeS)
{
	if(!m_MainGunUpgrades.empty())
	{
		if (m_MainGunUpgrades.top()->getNumShots() == 0)
		{
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

	//m_WorldTransform=m_LocalTransform->getLocalTransform()*m_Parent->getWorldTransform();
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
	temp->setPosition(m_MuzzleOffset);
	temp->setOrientation(m_OrientationDeg);
	return temp;
}
ProjectileNode* TurretNode::getMainGunProjectile()
{
	//get a bullet, initialize it and return in
	ProjectileNode* temp = m_MainGunUpgrades.top()->getProjectile();
	temp->setPosition(m_MuzzleOffset);
	temp->setOrientation(m_OrientationDeg);
	return temp;
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
