#pragma once
#include "CollidableNode.h"
#include "TurretNode.h"
#include "OffensiveUpgrade.h"

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
	ProjectileNode* getAutoGunProjectile();
	void AddOffensiveUpgrade(OffensiveUpgrade* p_Upgrade);
	void chargeMainGun(float p_DeltaTimeS);
	const float getShieldHitPoints(){return m_ShieldHitPoints;}
	const float getMaxShieldHitPoints(){return m_ShieldMaxHitPoints;}
	const float getWeaponChargeLevel(){return m_WeaponChargeLevel;}
	const float getMaxWeaponChargeLevel(){return m_MaxWeaponChargeLevel;}
	virtual ~TestTankNode(void);
protected:
	TurretNode* m_Turret;
	float m_Thrust; //thrust amount measured in Newtons/s. calculations must be impulse so multiply by delta time
	float m_Mass;   //used for calculating impulses, measured in Kg
	float m_MaxVelocity; //maximum absolute change in position, M/s
	float m_TankAeroConstant; //used for air resistance calculations;
	float m_TargetOrientation; //used as a target for rotation animations
	float m_TurnAnimSpeed; //tank will turn towards the direction of movement
	bool m_Turning; //used to indicate if the tank is turning
	float m_MaxWeaponChargeLevel; //Maximum amount of charge this tanks weapon can hold
	float m_WeaponChargeLevel; //current charge level of main weapon
	float m_WeaponChargeRate; //rate at which main weapon charges in amount/s
	float m_ShieldHitPoints; //shield hp, essentially life points
	float m_ShieldMaxHitPoints; //maximum hp for this tank
	float m_ShieldChargeRate; //rate at which shield recharges per second
};

