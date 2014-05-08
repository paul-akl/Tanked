#pragma once
#include "OffensiveUpgrade.h"
#include "MobilityUpgrade.h"
#include "DefensiveUpgrade.h"
class UpgradeFactory
{
public:
	UpgradeFactory(void);
	OffensiveUpgrade* getOffensiveUpgrade(OffensiveUpgradeType p_Type);
	MobilityUpgrade* getMobilityUpgrade();
	DefensiveUpgrade* getDefensiveUpgrade();
	virtual void init();
	virtual ~UpgradeFactory(void);
protected:
	OffensiveUpgrade* getInstanceFromPool(OffensiveUpgradeType p_Type);
	DefensiveUpgrade* getDefInstanceFromPool();
	MobilityUpgrade* getMobInstanceFromPool();
	std::vector<OffensiveUpgrade*> m_OffensiveUpgrades;
	std::vector<DefensiveUpgrade*> m_DefensiveUpgrades;
	std::vector<MobilityUpgrade*> m_MobilityUpgrades;
	static const int MAX_UPGRADES = 10;
	TextureNode* m_DefaultUpgradeTexture;
	TextureNode* m_OffensiveEmissive;
	TextureNode* m_MobilityEmissive;
	TextureNode* m_DefensiveEmissive;
	TextureNode* m_UpgradeNormalMap;
	MeshNode* m_DefaultUpgradeMesh;
	TextureNode* m_DefensiveDiffuse;
	TextureNode* m_MobilityDiffuse;
	TextureNode* m_MainGunProjectileDiffuse;
	TextureNode* m_AutoGunProjectileEmissive;
	TextureNode* m_MainGunProjectileEmissive;
	TextureNode* m_FireGunProjectileEmissive;
	TextureNode* m_IceGunProjectileEmissive;
	TextureNode* m_OffensiveDiffuse;
	MeshNode* m_DefaultProjectileMesh;
	TextureNode* m_DefaultProjectileTexture;
	int m_NumObjects;
	bool m_Reusing;
};

