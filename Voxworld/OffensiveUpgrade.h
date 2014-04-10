#pragma once
#include "upgradenode.h"
#include "ProjectileNode.h"

class OffensiveUpgrade :
	public UpgradeNode
{
public:
	OffensiveUpgrade(void);
	virtual ProjectileNode* getProjectile();
	virtual ProjectileNode* createProjectileFromPool();
	virtual void init();
	virtual void render(Renderer* p_Renderer);
	virtual void update(float p_DeltaTimeS);
	virtual void Collect(UpgradableTank* p_Tank){;}
	ProjectileType getProjectileType(){return m_ProjectileType;}
	void setProjectileMesh(MeshNode* p_DefaultProjectileMesh){m_DefaultProjectileMesh = p_DefaultProjectileMesh;}
	void setProjectileDiffuse(TextureNode* p_DefaultProjectileTexture){m_DefaultProjectileTexture = p_DefaultProjectileTexture;}
	virtual int getNumShots(){return -1;}
	void activate();
	virtual ~OffensiveUpgrade(void);
protected:
	bool isReadyToFire();
	//derived offensive upgrades can override this method to create the projectile of their choosing
	std::vector<ProjectileNode*> m_Projectiles;
	float m_SecondsPerShot;
	float m_TimeSinceLastShot;
	int m_MaxProjectiles;
	MeshNode* m_DefaultProjectileMesh;
	TextureNode* m_DefaultProjectileTexture;
	ProjectileType m_ProjectileType;
	int m_NumShots;
	int m_NumObjects;
	bool m_Reusing;
};

