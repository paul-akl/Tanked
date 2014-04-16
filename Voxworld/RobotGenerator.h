#pragma once
#include "collidablenode.h"
#include <list>
#include "Robot.h"
#include "EnemyNode.h"
class RobotGenerator :
	public EnemyNode
{
public:
	RobotGenerator(void);
	virtual void render(Renderer* p_Renderer);
	virtual void update(float p_DeltaTimeS);
	bool isReady();
	Robot* getRobot();
	void init();
	void setRobotDefaultTexture(TextureNode* p_Texture){m_DefaultRobotDiffuseTexture=p_Texture;}
	void setRobotDamagedTexture(TextureNode* p_Texture){m_DamagedRobotDiffuseTexture=p_Texture;}
	void setRobotBodyMesh(MeshNode* p_Mesh){m_RobotBodyMesh=p_Mesh;}
	void setRobotHeadMesh(MeshNode* p_Mesh){m_RobotHeadMesh=p_Mesh;}
	void setRobotArmMesh(MeshNode* p_Mesh){m_RobotArmMesh = p_Mesh;}
	void addDamagedDiffuseTexture(TextureNode* p_Texture){m_DamagedDiffuseTexture=p_Texture;}
	void setHitPoints(const unsigned int p_HP){m_HitPoints = p_HP;}
	int getHitPoints(){return m_HitPoints;}
	void dealDamage(const unsigned int p_DamageAmount){m_HitPoints-=p_DamageAmount;}
	void setMaxHitPoints(const unsigned int p_MaxHP){m_MaxHitPoints = p_MaxHP;}
	void setDifficulty(const unsigned int p_Difficulty);
	virtual ~RobotGenerator(void);
protected:
	bool deleteAll(Robot* p_Robot);
	RobotHead* getHead();
	RobotArm* getArm(bool p_Left);
	Robot* getRobotFromPool();
	bool m_Reusing;
	float m_BaseTimer;
	static const int MAX_ROBOTS = 3;
	TextureNode* m_DefaultRobotDiffuseTexture;
	TextureNode* m_DamagedRobotDiffuseTexture;
	TextureNode* m_DamagedDiffuseTexture;
	MeshNode* m_RobotBodyMesh;
	MeshNode* m_RobotHeadMesh;
	MeshNode* m_RobotArmMesh;
	std::list<Robot*> m_Robots;
	unsigned int m_Level; //number of the current level of difficulty
	glm::vec3 m_RobotSpawnPoint;
	int m_HitPoints;
	unsigned int m_MaxHitPoints;
	float m_SpawnDelay;
	float m_SpawnTimer;
	float m_SpawnDistance;
	int m_NumRobots;

};

