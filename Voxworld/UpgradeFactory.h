#pragma once
#include "OffensiveUpgrade.h"
class UpgradeFactory
{
public:
	UpgradeFactory(void);
	OffensiveUpgrade* getOffensiveUpgrade(OffensiveUpgradeType p_Type);
	//MobilityUpgrade* getMobilityUpgrade(MobilityUpgradeType p_Type);
	//DefensiveUpgrade* getDefensiveUpgrade*DefensiveUpgradeType p_Type);
	virtual void init();
	virtual ~UpgradeFactory(void);
protected:
	OffensiveUpgrade* getInstanceFromPool(OffensiveUpgradeType p_Type);
	//DefensiveUpgrade* getInstanceFromPool(DefensiveUpgradeType p_Type);
	//MobilityUpgrade* getInstanceFromPool(MobilityUpgradeType p_Type);
	std::vector<OffensiveUpgrade*> m_OffensiveUpgrades;
	static const int MAX_UPGRADES = 10;
	TextureNode* m_DefaultUpgradeTexture;
	MeshNode* m_DefaultUpgradeMesh;
	MeshNode* m_DefaultProjectileMesh;
	TextureNode* m_DefaultProjectileTexture;
	int m_NumObjects;
	bool m_Reusing;
};

