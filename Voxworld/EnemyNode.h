#pragma once

#include "CollidableNode.h"

enum EnemyStatus
{
	PassiveStatus,
	AlertStatus,
	HostileStatus
};

class EnemyNode : 
	public CollidableNode
{
public:
	EnemyNode(void);
	~EnemyNode(void);
	
	int getState();									// Returns an EnemyStatus enum as integer
	void setState(int p_EnemyStatus);				// Sets a state to one of EnemyStatus enum
	float getDetectionRadius();						// Returns 'm_detectionRadius'
	void setDetectionRadius(float p_Radius);		// Sets 'm_detectionRadius'
	glm::vec3 getLookDirection();					// Returns 'm_lookDirection'
	float getViewConeAngle();						// Returns 'm_viewConeAngle'
	void setViewConeAngle(float p_Angle);			// Sets 'm_viewConeAngle'
	float getMaxLookDistance();						// Returns 'm_maxLookDistance'
	void setMaxLookDistance(float p_Distance);		// Sets 'm_maxLookDistance'
	glm::vec3 getMovementTarget();					// Returns 'm_movementTarget'
	void setMovementTarget(glm::vec3 p_Target);		// Sets 'm_movementTarget' takes in position in world space
	glm::vec3 getTargetPosition();					// Returns 'm_targetPosition'
	void setTargetPosition(glm::vec3 p_Target);		// Sets 'm_targetPosition' takes in position in world space

protected:
	glm::vec3	m_targetPosition,
				m_movementTarget,
				m_lookDirection,
				m_LastTarget;

	float	m_viewConeAngle,
			m_maxLookDistance,
			m_detectionRadius;

	int m_behaviourState;
	float m_StateTimer;
	bool m_PathChanged;
};