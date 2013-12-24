#pragma once
#include "CollidableNode.h"
#include "TurretNode.h"
#include "OffensiveUpgrade.h"
static const float PI = 3.14159265f;
class TestTankNode :
	public CollidableNode
{
public:
	TestTankNode(void);
	void addTurretNode(TurretNode* p_Turret);
	void rotateTurret(const float p_Rotation);
	void moveForward(float p_DeltaTimeS);
	void moveBack(float p_DeltaTimeS);
	void moveLeft(float p_DeltaTimeS);
	void moveRight(float p_DeltaTimeS);
	virtual void render(Renderer* p_Renderer);
	virtual void update(float p_DeltaTimeS);
	ProjectileNode* getMainGunProjectile();
	void AddOffensiveUpgrade(OffensiveUpgrade* p_Upgrade);
	void chargeMainGun(float p_DeltaTimeS);
	virtual ~TestTankNode(void);
protected:
	TurretNode* m_Turret; //child node. not implemented yet.
	float m_Thrust; //thrust amount measured in Newtons/s. calculations must be impulse so multiply by delta time
	float m_Mass;   //used for calculating impulses, measured in Kg
	float m_MaxVelocity; //maximum absolute change in position, M/s
	bool m_IsMoving;
	float m_TankAeroConstant; //used for air resistance calculations;
	float m_TargetOrientation;
	float m_TurnAnimSpeed; //tank will turn towards the direction of movement
	bool m_Turning;
	float m_MaxWeaponChargeLevel;
	float m_WeaponChargeLevel;
	float m_WeaponChargeRate;
};

