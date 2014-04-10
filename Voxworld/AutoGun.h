#pragma once
#include "offensiveupgrade.h"
class AutoGun :
	public OffensiveUpgrade
{
public:
	AutoGun(void);
	virtual ProjectileNode* getProjectile();
	virtual ProjectileNode* createProjectileFromPool();
	virtual void init();
	virtual void Collect(UpgradableTank* p_Tank){;}
	virtual void render(Renderer* p_Renderer);
	virtual void update(float p_DeltaTimeS);
	virtual ~AutoGun(void);
private:
	static const int MAX_PROJECTILES = 128;
};

