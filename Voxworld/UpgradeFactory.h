#pragma once
#include "OffensiveUpgrade.h"
class UpgradeFactory
{
public:
	UpgradeFactory(void);
	OffensiveUpgrade* getOffensiveUpgrade();
	virtual void init();
	virtual ~UpgradeFactory(void);
protected:
	virtual OffensiveUpgrade* getInstanceFromPool();
	std::vector<OffensiveUpgrade*> m_OffensiveUpgrades;
	static const int MAX_UPGRADES = 10;
	TextureNode* m_DefaultUpgradeTexture;
	MeshNode* m_DefaultUpgradeMesh;
	MeshNode* m_DefaultProjectileMesh;
	TextureNode* m_DefaultProjectileTexture;
	int m_NumObjects;
	bool m_Reusing;
};

