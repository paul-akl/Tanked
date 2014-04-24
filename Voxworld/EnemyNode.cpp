#include "EnemyNode.h"

EnemyNode::EnemyNode(void)
{
	m_movementTarget = glm::vec3(0.0f);
	m_targetPosition = glm::vec3(0.0f);
	m_PathChanged = false;
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
}
void EnemyNode::setStateTimer(float p_TimerS)
{
	m_StateTimer = p_TimerS;
	m_CurrentTimer = p_TimerS;
}
float EnemyNode::getDetectionRadius()
{
	return m_detectionRadius;
}
float EnemyNode::getCurrentTimer()
{
	return m_CurrentTimer;
}
void EnemyNode::resetTimer()
{
	m_CurrentTimer = m_StateTimer;
}
std::list<glm::vec3>& EnemyNode::getWayPointList()
{
	return m_WayPoints;
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
	m_PathChanged = true;
		//printf("target changed\n");
	m_lookDirection = glm::normalize(p_Target-m_Position);
	//printf("move to: %f,%f",m_movementTarget.x,m_movementTarget.z);
}
glm::vec3 EnemyNode::getTargetPosition()
{
	return m_targetPosition;
}
void EnemyNode::setTargetPosition(glm::vec3 p_Target)
{
	m_targetPosition = p_Target;
}