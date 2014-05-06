#pragma once
#include "RobotGenerator.h"
enum RobotType
{
	ROBOT_DEFAULT,
	ROBOT_FIRE,
	ROBOT_ICE,
	ROBOT_GRAVITY,
	ROBOT_CONCUSSIVE,
};
class RoboGenFactory
{
public:
	RoboGenFactory(void);
	void init();
	RobotGenerator* getRobotGenerator(unsigned int p_DifficultyLevel,RobotType p_Type);
	~RoboGenFactory(void);
protected:
	RobotGenerator* getRoboGenFromPool();
	//default robot visual data. Maps will be used when other robot types are added
	TextureNode		*m_DefaultGeneratorTexture,
					*m_DefaultGeneratorNormal,
					*m_DefaultGeneratorEmissive,
					*m_DefaultGeneratorSpecular,
					*m_DamagedGeneratorTexture,
					*m_DamagedGeneratorNormal,
					*m_DamagedGeneratorEmissive,
					*m_DamagedGeneratorSpecular,
					*m_DamagedRobotTexture,
					*m_DefaultRobotTexture;
	MeshNode* m_RoboGenMesh;
	MeshNode* m_RoboHeadMesh;
	MeshNode* m_RoboArmMesh;
	MeshNode* m_RoboBodyMesh;
	// list of generators created for 
	std::list<RobotGenerator*> m_RoboGens;
	unsigned int m_NumObjects;
	bool m_Reusing;
};

