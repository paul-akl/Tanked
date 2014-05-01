#include "Robot.h"
#include "TransformNode.h"
#include "Renderer.h"
#include "MeshNode.h"
#include "TextureNode.h"
#include <iostream>

Robot::Robot(void)
{
	m_LeftArm = nullptr;
	m_RightArm = nullptr;
	m_Head = nullptr;
	m_MaxVelocityScalar = 150.0f;
	m_DamagedTextureDiffuse = nullptr;
	m_TurnSpeed = 25.0f;
	m_TargetOrientation = 0.0f;
	m_OrientationDeg = 0.0f;
	m_Thrust = 4000.0f;
	m_Mass = 300.0f;
	m_HeadPosition = glm::vec3(0.0f,3.5f,0.0f);
	m_LeftArmPosition = glm::vec3(0.0f,3.0f,-2.8f);
	m_RightArmPosition = glm::vec3(0.0f,3.0f,2.8f);
	m_Velocity = glm::vec3(0.0f);
	m_behaviourState=HostileStatus;
	m_Turning = false;
	m_BaseDamage = 50;
	m_LastMoveTarget =glm::vec3(0.0f); 
}
void Robot::addLeftArm(RobotArm* p_LeftArm)
{
	m_LeftArm = p_LeftArm;
	SceneNode::addNode(p_LeftArm);
	m_LeftArm->setPosition(m_LeftArmPosition);
}
void Robot::addRightArm(RobotArm* p_RightArm)
{
	m_RightArm = p_RightArm;
	SceneNode::addNode(p_RightArm);
	m_RightArm->setPosition(m_RightArmPosition);
}
void Robot::addHead(RobotHead* p_Head)
{
	m_Head = p_Head;
	SceneNode::addNode(p_Head);
	m_Head->setPosition(m_HeadPosition);
}
const int Robot::getHitPoints()
{
	return m_HitPoints;
}
void Robot::addDamagedTexture(TextureNode* p_Texture)
{
	m_DamagedTextureDiffuse = p_Texture;
	m_Head->addDamagedTexture(p_Texture);
	m_LeftArm->addDamagedTexture(p_Texture);
	m_RightArm->addDamagedTexture(p_Texture);
}
void Robot::dealDamage(unsigned int p_DamageAmount)
{
	if((int)m_HitPoints-p_DamageAmount<0)
	{
		m_HitPoints=0;
	}
	else
	{
		m_HitPoints-=p_DamageAmount;
	}
}
void Robot::setMaxHitPoints(const unsigned int p_MaxHP)
{
	m_MaxHitPoints = p_MaxHP;
}
void Robot::setHitPoints(const unsigned int p_HP)
{
	m_HitPoints = p_HP;
	m_Head->setDamaged(false);
	m_LeftArm->setDamaged(false);
	m_RightArm->setDamaged(false);
}
void Robot::turnLeft(float p_DeltaTimeS)
{
	m_OrientationDeg+=m_TurnSpeed*p_DeltaTimeS;
}
void Robot::turnRight(float p_DeltaTimeS)
{
	m_OrientationDeg-=m_TurnSpeed*p_DeltaTimeS;
}
void Robot::update(float p_DeltaTimeS)
{
	
	if(m_HitPoints <= 0)
	{
		deactivate();
	}
	else
	{
		m_CurrentTimer-=p_DeltaTimeS;
		if(m_HitPoints < (m_MaxHitPoints*0.5f))
		{
			m_RightArm->setDamaged(true);
			m_LeftArm->setDamaged(true);
			m_Head->setDamaged(true);
		}
	}
	/////////////////////////////////////////////////////////////////////////////
	// TARGET HANDLING														/////
	/////////////////////////////////////////////////////////////////////////////
	if(m_OrientationDeg >= 360.0f)
	{
		m_OrientationDeg-=360.0f;
	}
	else if(m_OrientationDeg < 0.0f)
	{
		m_OrientationDeg=360.0f+m_OrientationDeg;
	}
	if(m_behaviourState!=PassiveStatus)
	{
		if(true)//m_StateTimer>0.0f)
		{
			if(!m_RightArm->armRaised())
				m_RightArm->raiseArm();
			if(!m_LeftArm->armRaised())
				m_LeftArm->raiseArm();
				//if a path has been set
			if(!m_WayPoints.empty())
			{
				m_lookDirection=m_WayPoints.front()-m_Position;
				glm::vec3 targetDir = glm::normalize(m_lookDirection);
				if(glm::length(m_lookDirection) <= m_Radius*2.0f)
				{
					m_WayPoints.pop_front();
				}
				// calculate vector from current position to target position
				// convert vector to angle in degrees
				// set angle to head
					//m_TargetOrientation = acos(m_lookDirection.z)*RAD_TO_DEG;
	
					m_TargetOrientation = atanf(targetDir.z / targetDir.x);
					//std::cout << targetDir.x << " | " << std::endl << targetDir.z << std::endl << std::endl;


					//glm::vec3 axis = glm::cross(m_WayPoints.front(), m_Position);
					//axis = glm::normalize(axis);
					//float angle = acosf(glm::dot(m_WayPoints.front(), m_Position) / glm::length(m_WayPoints.front()) / glm::length(m_Position));
					//m_TargetOrientation = atan2f(m_Position.y * sinf(angle) - m_Position.x * m_Position.z * (1 - cosf(angle)), 1 - ((m_Position.y*m_Position.y)*(m_Position.z*m_Position.z) * (1-cosf(angle))));
					

					//if(m_TargetOrientation<0.0f)
					//	m_TargetOrientation+=PI*2.0f;
					m_TargetOrientation*=RAD_TO_DEG;
					m_OrientationDeg = m_TargetOrientation;
					m_Head->setTargetOrientation(m_TargetOrientation);
					float deltaOrientation = m_TargetOrientation-m_OrientationDeg;
					if(abs(deltaOrientation) < 5.5f)
					{
						m_Turning = false;
					}
					else
					{
						m_Turning = true;
					}
					if(m_Turning)
					{
						if (abs(deltaOrientation) > 180.0f)
							deltaOrientation += deltaOrientation > 0? -360.0f:360.0f;
							if(deltaOrientation < 0)
							{
								turnRight(p_DeltaTimeS);
							}
							else if(deltaOrientation > 0)
							{
								turnLeft(p_DeltaTimeS);
							}
					}
					m_Head->LookAt(m_movementTarget);
					glm::vec3 v_AccelerationDir = glm::normalize(m_lookDirection);
					//then calculate acceleration scalar, multiply that by acceleration direction, 
					//times delta time, to calculate impulse magnitude, and add to velocity
					//impulse is a force, applied over time, so we take the thrust force (in Newtons) X deltaTime, then X inverse Mass, for final acc
					m_Velocity+=v_AccelerationDir*((m_Thrust*(p_DeltaTimeS))*(1.0f/m_Mass));
					m_IsMoving = true;
				}
				//else
				//{
				//	printf("no path set\n");
				//}


		}
		else
		{
			m_StateTimer=5.0f;
			if(m_behaviourState==HostileStatus)
			{
				m_behaviourState=AlertStatus;
			}
			else if (m_behaviourState == AlertStatus)
			{
				m_behaviourState=PassiveStatus;
			}
		}
	}
	else
	{
		if(m_RightArm->armRaised())
			m_LeftArm->lowerArm();
		if(m_RightArm->armRaised())
			m_RightArm->lowerArm();
	}

	//////////////////////////////////////////////////////
	// VELOCITY INTERPOLATION
	//////////////////////////////////////////////////////
	//place a hard upper limit on max velocity, may not be needed as we have introduced air resistance
	if(glm::length(m_Velocity)> m_MaxVelocityScalar)
	{
		m_Velocity = glm::normalize(m_Velocity)*m_MaxVelocityScalar;
	}
	//update position
	///////////////////////////////////////////////////////////////////////////////////////
	//a quick and dirty model of air resistance impulse, in opposite direction of movement
	///////////////////////////////////////////////////////////////////////////////////////
	if(m_IsMoving)
	{
		m_Position+=m_Velocity*p_DeltaTimeS;

		//Fd = 0.5*air density* v^2 * TankAeroConstant * area
		//1.5f is air density, tank aero constant is the coefficient of it's cross sectional area
		float AirDensity = 1.5f;
		// an impulse is just a force, applied over a time period
		float crossSectionalArea = 20.0f;
		glm::vec3 AirResImpulse = (-m_Velocity*glm::length(m_Velocity)*AirDensity*1.0f*crossSectionalArea)*(p_DeltaTimeS);
		//when applying an impulse to an object, the effect is reduced by the object's mass (more mass = less effect)
		//Simulating air resistance accurately, as it turns out, makes virtually no difference to the overall velociy of a 1 ton tank.
		//another approach is needed for null input braking
		m_Velocity+=AirResImpulse/m_Mass;
		//so instead, cut off 0.5% each update, for a more immediate effect, and also it adds
		//m_Velocity*=0.995f;
	}
	//to prevent calculations moving out of floating point precision range
	//as we are only modelling
	if(glm::length(m_Velocity)<0.1f)
	{
		m_IsMoving = false;
		m_Velocity = glm::vec3(0.0f);
	}
	
	//deal with rotations
	//get target orientation from look direction or movement target
	// convert to angle
	// calculate delta angle (angle - orientation)
	// 

	m_LocalTransform->reset();
	m_LocalTransform->translate(m_Position+glm::vec3(0.0f,8.0f,0.0f));
	m_LocalTransform->rotate(m_OrientationDeg+180.0f,glm::vec3(0.0f,1.0f,0.0f));
	m_LocalTransform->scale(glm::vec3(1.5f));
	SceneNode::update(p_DeltaTimeS);

}
void Robot::render(Renderer* p_Renderer)
{
	if(m_HitPoints < (m_MaxHitPoints*0.5f))
	{
		if(m_Mesh!=nullptr && m_DamagedTextureDiffuse!=nullptr && m_LocalTransform != nullptr)
		{
			p_Renderer->begin();
			m_DamagedTextureDiffuse->render(p_Renderer);
			m_LocalTransform->render(p_Renderer);
			//any other texture render calls go in here
			m_Mesh->render(p_Renderer);
			p_Renderer->end();
			m_Head->render(p_Renderer);
			m_LeftArm->render(p_Renderer);
			m_RightArm->render(p_Renderer);
		}
	}
	else
	{
		SceneNode::render(p_Renderer);
	}
}
void Robot::reset()
{
	update(0.0f);
}
Robot::~Robot(void)
{
	delete m_LeftArm;
	delete m_RightArm;
	delete m_Head;
	m_DamagedTextureDiffuse = nullptr;
}
