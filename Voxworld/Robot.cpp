#include "Robot.h"
#include "TransformNode.h"
#include "Renderer.h"
#include "MeshNode.h"
#include "TextureNode.h"

Robot::Robot(void)
{
	m_LeftArm = nullptr;
	m_RightArm = nullptr;
	m_Head = nullptr;
	m_MaxVelocityScalar = 125.0f;
	m_DamagedTextureDiffuse = nullptr;
	m_TurnSpeed = 2.5f;
	m_TargetOrientation = 0.0f;
	m_OrientationDeg = 0.0f;
	m_Thrust = 4000.0f;
	m_Mass = 400.0f;
	m_HeadPosition = glm::vec3(0.0f,3.5f,0.0f);
	m_LeftArmPosition = glm::vec3(-2.8f,3.0f,0.0f);
	m_RightArmPosition = glm::vec3(2.8f,3.0f,0.0f);
	m_Velocity = glm::vec3(0.0f);
	m_behaviourState=HostileStatus;
	m_Turning = false;
	m_BaseDamage = 50;
	m_TargetOrientation = 0.0f;
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
const unsigned int Robot::getHitPoints()
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
	m_OrientationDeg-=m_TurnSpeed*p_DeltaTimeS;
}
void Robot::turnRight(float p_DeltaTimeS)
{
	m_OrientationDeg+=m_TurnSpeed*p_DeltaTimeS;
}
void Robot::update(float p_DeltaTimeS)
{

	if(m_HitPoints <= 0)
	{
		deactivate();
	}
	else
	{
		if(m_HitPoints < (m_MaxHitPoints*0.5f))
		{
			m_RightArm->setDamaged(true);
			m_LeftArm->setDamaged(true);
			m_Head->setDamaged(true);
		}
	}
	if(m_behaviourState!=PassiveStatus)
	{
		m_Head->LookAt(m_targetPosition);
		//m_StateTimer-=p_DeltaTimeS;
		if(true)//m_StateTimer>0.0f)
		{
			if(!m_RightArm->armRaised())
				m_RightArm->raiseArm();
			if(!m_LeftArm->armRaised())
				m_LeftArm->raiseArm();
			m_TargetOrientation = m_Head->getOrientation();
			glm::vec3 v_AccelerationDir = glm::normalize(m_movementTarget-m_Position);
			//then calculate acceleration scalar, multiply that by acceleration direction, 
			//times delta time, to calculate impulse magnitude, and add to velocity
			//impulse is a force, applied over time, so we take the thrust force (in Newtons) X deltaTime, then X inverse Mass, for final acc
			m_Velocity+=v_AccelerationDir*((m_Thrust*(p_DeltaTimeS))*(1.0f/m_Mass));
			m_IsMoving = true;
			m_Position+=m_Velocity*p_DeltaTimeS;

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

	if(m_TargetOrientation >= 360.0f)
	{
		m_TargetOrientation-=360.0f;
	}
	else if(m_TargetOrientation < 0.0f)
	{
		m_TargetOrientation=360.0f+m_TargetOrientation;
	}
	//normalize current orientation if needed
	if(m_OrientationDeg >= 360.0f)
	{
		m_OrientationDeg-=360.0f;
	}
	else if(m_OrientationDeg < 0.0f)
	{
		m_OrientationDeg=360.0f+m_OrientationDeg;
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

	if(m_TargetOrientation!=m_OrientationDeg)
	{
		if(abs(m_TargetOrientation-m_OrientationDeg)<5.5f)
		{
			m_OrientationDeg = m_TargetOrientation;
			m_Turning = false;
		}
		else
		{
			m_Turning = true;
			float deltaAngle = m_TargetOrientation-m_OrientationDeg;
			//if theta is greater than 180 degrees, then make a small adjustment, to make deltaAngle relative to zero
			//(make deltaAngle either positive or negative
			if (abs(deltaAngle) > 180.0f)
				deltaAngle += deltaAngle > 0? -360.0f:360.0f;
			if(deltaAngle < 0)
			{
				turnLeft(p_DeltaTimeS);
			}
			else if(deltaAngle > 0)
			{
				turnRight(p_DeltaTimeS);
			}
		}
	}
	m_LocalTransform->reset();
	m_LocalTransform->translate(m_Position+glm::vec3(0.0f,8.0f,0.0f));
	m_LocalTransform->rotate(m_OrientationDeg,glm::vec3(0.0f,1.0f,0.0f));
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
Robot::~Robot(void)
{
	delete m_LeftArm;
	delete m_RightArm;
	delete m_Head;
	m_DamagedTextureDiffuse = nullptr;
}
