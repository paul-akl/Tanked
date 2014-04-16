#include "TestTankNode.h"
#include "Renderer.h"
#include "TransformNode.h"
#include "TurretNode.h"
TestTankNode::TestTankNode(void)
{
	m_Parent=nullptr;
	m_Position = glm::vec3(0.0,0.0,0.0);
	m_MaxVelocity = 150.0f; //max velocity in m/s
	m_TankAeroConstant = 1.0f;
	m_Mass = 750.0f; //mass is one metric ton
	m_Thrust = 15000.0f; //thrust is in Newtons
	m_IsMoving = false;
	m_Velocity = glm::vec3(0.0f); //velocity is only non zero if moving
	m_TurnAnimSpeed = 150.0f; //used for smooth rotation of tank body
	m_TargetOrientation = 0.0f; //no target oriention on start
	m_OrientationDeg = 0.0f; //orientation initialized to zero
	m_MaxWeaponChargeLevel = 1.0f;
	m_WeaponChargeRate=0.6f;
	m_WeaponChargeLevel = 0.0f;
	m_ShieldHitPoints = 100.0f;
	m_ShieldMaxHitPoints = 200.0f;
	m_ShieldChargeRate = 1.0f;
	m_HoverLampOffset = glm::vec3(0.0f, 3.0f, 0.0f);
	m_RHeadLight = nullptr;
	m_LHeadlight = nullptr;
	m_HoverLight = nullptr;
}

void TestTankNode::chargeMainGun(float p_DeltaTimeS)
{
	m_WeaponChargeLevel += m_WeaponChargeRate*p_DeltaTimeS;
	if(m_WeaponChargeLevel >= m_MaxWeaponChargeLevel)
		m_WeaponChargeLevel = m_MaxWeaponChargeLevel;
}
ProjectileNode* TestTankNode::getMainGunProjectile()
{
	if(m_WeaponChargeLevel > 0.2f)
	{
		ProjectileNode* tmp = m_Turret->getMainGunProjectile();
		if(tmp!=nullptr)
		{
			tmp->setDamageMultiplier(1.0f+m_WeaponChargeLevel);
		}

		m_WeaponChargeLevel = 0.0f;
		return tmp;
	}
	else
	{
		m_WeaponChargeLevel = 0.0f;
		return nullptr;
	}
}
ProjectileNode* TestTankNode::getAutoGunProjectile()
{
	ProjectileNode* tmp = m_Turret->getAutoGunProjectile();
	return tmp;
}
void TestTankNode::AddOffensiveUpgrade(OffensiveUpgrade* p_Upgrade)
{
	if(p_Upgrade->getProjectileType()!=DEFAULT_SECONDARY)
	{
		m_Turret->setMainGun(p_Upgrade);
		p_Upgrade->setCollected();
	}
	else
	{
		m_Turret->setAutoGun(p_Upgrade);
		p_Upgrade->setCollected();
	}
}
void TestTankNode::addTurretNode(TurretNode* p_Turret)
{
	//keep a reference and add to children, in the default way.
	m_Turret = p_Turret;
	SceneNode::addNode(p_Turret);
	m_OrientationDeg=m_Turret->getOrientation();
}
void TestTankNode::setHoverLight(LightNode* p_Light)
{
	m_HoverLampOffset = glm::vec3(0.0,0.0f,0.0f);
	m_HoverLight = p_Light;
	m_HoverLight->setParent(this);
	m_HoverLight->setPosition(m_HoverLampOffset);
}
void TestTankNode::setHeadLight(SpotLight* p_Light,bool p_Right)
{
	if(p_Right)
	{
		m_RHLampOffset = glm::vec3(m_Radius,5.0f,m_Radius/2.0f);
		m_RHeadLight = p_Light;
		m_RHeadLight->setPosition(m_RHLampOffset);
		m_RHeadLight->setParent(this);
	}
	else
	{
		m_LHLampOffset = glm::vec3(m_Radius,5.0f,-m_Radius/2.0f);
		m_LHeadlight = p_Light;
		m_LHeadlight->setPosition(m_LHLampOffset);
		m_LHeadlight->setParent(this);
	}
}
void TestTankNode::rotateTurret(const float p_Rotation)
{
	m_Turret->rotateTurret(p_Rotation);
}
void TestTankNode::moveForward(float p_DeltaTimeS)
{
	m_TargetOrientation = m_Turret->getOrientation();
	//perform a quick conversion to radians and calculate direction of acceleration
	float v_OrientationRad = m_TargetOrientation*(PI/180.0f);
	glm::vec3 v_AccelerationDir = glm::normalize(glm::vec3(glm::sin(v_OrientationRad),0.0f,glm::cos(v_OrientationRad)));
	//then calculate acceleration scalar, multiply that by acceleration direction, 
	//times delta time, to calculate impulse magnitude, and add to velocity
	//impulse is a force, applied over time, so we take the thrust force (in Newtons) X deltaTime, then X inverse Mass, for final acc
	m_Velocity+=v_AccelerationDir*((m_Thrust*(p_DeltaTimeS))*(1.0f/m_Mass));


	m_IsMoving = true;

}
void TestTankNode::moveBack(float p_DeltaTimeS)
{
	m_TargetOrientation = m_Turret->getOrientation()+180.0f;
	//perform a quick conversion to radians and calculate direction of acceleration
	float v_OrientationRad = m_TargetOrientation*(PI/180.0f);
	glm::vec3 v_AccelerationDir = glm::normalize(glm::vec3(glm::sin(v_OrientationRad),0.0f,glm::cos(v_OrientationRad)));
	//then calculate acceleration scalar, multiply that by acceleration direction, 
	//impulse is a force, applied over time, so we take the thrust force (in Newtons) X deltaTime, then X inverse Mass, for final acc
	m_Velocity+=v_AccelerationDir*((m_Thrust*(p_DeltaTimeS))*(1.0f/m_Mass));

	m_IsMoving = true;
}
void TestTankNode::moveLeft(float p_DeltaTimeS)
{
	m_TargetOrientation = m_Turret->getOrientation()+90.0f;
	//perform a quick conversion to radians and calculate direction of acceleration
	float v_OrientationRad = m_TargetOrientation*(PI/180.0f);
	glm::vec3 v_AccelerationDir = glm::normalize(glm::vec3(glm::sin(v_OrientationRad),0.0f,glm::cos(v_OrientationRad)));
	//then calculate acceleration scalar, multiply that by acceleration direction, 
	//impulse is a force, applied over time, so we take the thrust force (in Newtons) X deltaTime, then X inverse Mass, for final acc
	m_Velocity+=v_AccelerationDir*((m_Thrust*(p_DeltaTimeS))*(1.0f/m_Mass));

	m_IsMoving = true;
}
void TestTankNode::moveRight(float p_DeltaTimeS)
{
	m_TargetOrientation = m_Turret->getOrientation()-90.0f;
	//perform a quick conversion to radians and calculate direction of acceleration
	float v_OrientationRad = m_TargetOrientation*(PI/180.0f);
	glm::vec3 v_AccelerationDir = glm::normalize(glm::vec3(glm::sin(v_OrientationRad),0.0f,glm::cos(v_OrientationRad)));
	//then calculate acceleration scalar, multiply that by acceleration direction, 
	//impulse is a force, applied over time, so we take the thrust force (in Newtons) X deltaTime, then X inverse Mass, for final acc
	m_Velocity+=v_AccelerationDir*((m_Thrust*(p_DeltaTimeS))*(1.0f/m_Mass));

	m_IsMoving = true;
}
void TestTankNode::update(float p_DeltaTimeS)
{
	//normalize target orientation, if needed
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
	////////////////////////////////////////////
	//ANGLE ANIMATION
	////////////////////////////////////////////
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
				m_OrientationDeg-=m_TurnAnimSpeed*p_DeltaTimeS;
			}
			else if(deltaAngle > 0)
			{
				m_OrientationDeg+=m_TurnAnimSpeed*p_DeltaTimeS;
			}
		}
	}
	
	//////////////////////////////////////////////////////
	//SHIELD RECHARGE INTERPOLATION
	//////////////////////////////////////////////////////
	if(m_ShieldHitPoints<m_ShieldMaxHitPoints)
		m_ShieldHitPoints+=m_ShieldChargeRate*p_DeltaTimeS;
	else
		m_ShieldHitPoints=m_ShieldMaxHitPoints;
	//m_ShieldHitPoints -= 0.1;
	//////////////////////////////////////////////////////
	// VELOCITY INTERPOLATION
	//////////////////////////////////////////////////////
	//place a hard upper limit on max velocity, may not be needed as we have introduced air resistance
	if(glm::length(m_Velocity)> m_MaxVelocity)
	{
		m_Velocity = glm::normalize(m_Velocity)*m_MaxVelocity;
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
		glm::vec3 AirResImpulse = ((-m_Velocity*glm::length(m_Velocity)*AirDensity*m_TankAeroConstant*crossSectionalArea)*(p_DeltaTimeS))/m_Mass;
		//when applying an impulse to an object, the effect is reduced by the object's mass (more mass = less effect)
		//Simulating air resistance accurately, as it turns out, makes virtually no difference to the overall velociy of a 1 ton tank.
		//another approach is needed for null input braking
		m_Velocity+=AirResImpulse;
		//so instead, cut off 0.5% each update, for a more immediate effect, and also it adds
		//m_Velocity*=0.985f;

	}
	//to prevent calculations moving out of floating point precision range
	//as we are only modelling
	if(glm::length(m_Velocity)<0.1f)
	{
		m_IsMoving = false;
		m_Velocity = glm::vec3(0.0f);
	}
	m_Position+=m_Velocity*(p_DeltaTimeS);

	//update local transformation
	m_LocalTransform->reset();
	m_LocalTransform->translate(m_Position+glm::vec3(0.0,5.0f,0.0));
	m_LocalTransform->rotate(m_OrientationDeg,glm::vec3(0.0,1.0f,0.0));
	m_LocalTransform->scale(glm::vec3(1.0f));
	//then do default behaviour
	SceneNode::update(p_DeltaTimeS);
	//update light transforms
	if(m_LHeadlight!=nullptr)
	{
		if(m_LHeadlight->isActive())
			m_LHeadlight->update(p_DeltaTimeS);
	}
	if(m_LHeadlight!=nullptr)
	{
		if(m_RHeadLight->isActive())
			m_RHeadLight->update(p_DeltaTimeS);
	}
	if(m_HoverLight!=nullptr)
	{
		if(m_HoverLight->isActive())
			m_HoverLight->update(p_DeltaTimeS);
	}

	
	
}
void TestTankNode::render(Renderer* p_Renderer)
{
	SceneNode::render(p_Renderer);
	if(m_LHeadlight!=nullptr)
	{
		if(m_LHeadlight->isActive())
			m_LHeadlight->render(p_Renderer);
	}
	if(m_LHeadlight!=nullptr)
	{
		if(m_RHeadLight->isActive())
			m_RHeadLight->render(p_Renderer);
	}
	if(m_HoverLight!=nullptr)
	{
		if(m_HoverLight->isActive())
			m_HoverLight->render(p_Renderer);
	}
}
TestTankNode::~TestTankNode(void)
{
	delete m_Turret;
}
