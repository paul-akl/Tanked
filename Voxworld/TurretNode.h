#pragma once
#include "SceneNode.h"
#include "OffensiveUpgrade.h"
#include <stack>

class TurretNode :
	public SceneNode
{
public:
	TurretNode(void);
	//TO DO
	//finish class
	virtual void render(Renderer* p_Renderer);
	virtual void update(float p_DeltaTimeS);
	void rotateTurret(float p_Rotation);
	//method stub: to be reviewed
	ProjectileNode* getAutoGunProjectile();
	ProjectileNode* getMainGunProjectile();
	void setMainGun(OffensiveUpgrade* p_Upgrade);
	void setAutoGun(OffensiveUpgrade* p_Upgrade);
	virtual ~TurretNode(void);
protected:
	glm::vec3 m_MuzzleOffset;
	std::stack<OffensiveUpgrade*> m_MainGunUpgrades;
	OffensiveUpgrade* m_AutoGun;
};

