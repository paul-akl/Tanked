#include "EnemyNode.h"

EnemyNode::EnemyNode(void)
{

}
EnemyNode::~EnemyNode(void)
{

}

int EnemyNode::getState()
{
	return m_behaviourState;
}
void EnemyNode::setState(int p_EnemyStatus)
{
	m_behaviourState = p_EnemyStatus;
	m_StateTimer = 5.0f;
}
float EnemyNode::getDetectionRadius()
{
	return m_detectionRadius;
}
void EnemyNode::setDetectionRadius(float p_Radius)
{
	m_detectionRadius = p_Radius;
}
glm::vec3 EnemyNode::getLookDirection()
{
	return m_lookDirection;
}
float EnemyNode::getViewConeAngle()
{
	return m_viewConeAngle;
}
void EnemyNode::setViewConeAngle(float p_Angle)
{
	m_viewConeAngle = p_Angle;
}
float EnemyNode::getMaxLookDistance()
{
	return m_maxLookDistance;
}
void EnemyNode::setMaxLookDistance(float p_Distance)
{
	m_maxLookDistance = p_Distance;
}
glm::vec3 EnemyNode::getMovementTarget()
{
	return m_movementTarget;
}
void EnemyNode::setMovementTarget(glm::vec3 p_Target)
{
	m_movementTarget = p_Target;
}
glm::vec3 EnemyNode::getTargetPosition()
{
	return m_targetPosition;
}
void EnemyNode::setTargetPosition(glm::vec3 p_Target)
{
	m_targetPosition = p_Target;
}