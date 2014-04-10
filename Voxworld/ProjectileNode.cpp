#include "ProjectileNode.h"
#include "TransformNode.h"

ProjectileNode::ProjectileNode(void)
{
	m_BaseDamage = 0;
	m_MaxLifeTime = -1.0f;
	m_Timer = 0.0f;
	m_ProjectileType = DEFAULT_MAIN;
	m_VelocityScalar = 75.0f;
	m_DamageMultiplier = 1.0f;
}
void ProjectileNode::update(float p_DeltaTimeS)
{

	if(m_MaxLifeTime  == -1.0f)
		m_Position+=m_Velocity*p_DeltaTimeS;
	else
	{
		if(m_Timer > 0.0)
		{
			m_Position+=m_Velocity*p_DeltaTimeS;
			m_Timer-=p_DeltaTimeS;
		}
		else
		{
			deactivate();
		}
	}
	m_LocalTransform->reset();
	m_LocalTransform->translate(m_Position+glm::vec3(0.0f,10.0f,0.0f));
	m_LocalTransform->rotate(m_OrientationDeg,glm::vec3(0.0f,1.0f,0.0f));
	m_LocalTransform->scale(glm::vec3(m_Radius));
	SceneNode::update(p_DeltaTimeS);
}
void ProjectileNode::render(Renderer* p_Renderer)
{
	SceneNode::render(p_Renderer);
}
void ProjectileNode::Bounce(const glm::vec3& p_Normal)
{
	m_Velocity = glm::reflect(m_Velocity,p_Normal);
	glm::vec3 unit(glm::normalize(m_Velocity));
	m_OrientationDeg = (atan2(unit.x,unit.z)*RAD_TO_DEG)-180.0f;
}
//override SceneNode::setOrientation
void ProjectileNode::setOrientation(const float p_OrientationDeg)
{
	m_Velocity = glm::vec3(glm::sin((p_OrientationDeg)*(PI_OVER180)),0.0f,glm::cos((p_OrientationDeg)*(PI_OVER180)))*m_VelocityScalar;
	SceneNode::setOrientation(p_OrientationDeg);
}
ProjectileNode::~ProjectileNode(void)
{
}
