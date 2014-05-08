#pragma once
#include "collidablenode.h"
#include <list>
#include "Robot.h"
#include "EnemyNode.h"
#include "LightNode.h"
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
	void setRobotDefaultTexture(TextureNode* p_Texture)		{ m_DefaultRobotDiffuse=p_Texture;			}
	void setRobotDamagedTexture(TextureNode* p_Texture)		{ m_DamagedRobotDiffuse=p_Texture;			}
	void setRobotDefaultEmissive(TextureNode* p_Texture)	{ m_DefaultRobotEmissive=p_Texture;			}
	void setRobotDamagedEmissive(TextureNode* p_Texture)	{ m_DamagedRobotEmissive=p_Texture;			}
	void setRobotDefaultNormal(TextureNode* p_Texture)		{ m_DefaultRobotNormal=p_Texture;			}
	void setRobotDamagedNormal(TextureNode* p_Texture)		{ m_DamagedRobotNormal=p_Texture;			}
	void setRobotDefaultSpecular(TextureNode* p_Texture)	{ m_DefaultRobotSpecular=p_Texture;			}
	void setRobotDamagedSpecular(TextureNode* p_Texture)	{ m_DamagedRobotSpecular=p_Texture;			}
	void setRobotBodyMesh(MeshNode* p_Mesh)					{ m_RobotBodyMesh=p_Mesh;					}
	void setRobotHeadMesh(MeshNode* p_Mesh)					{ m_RobotHeadMesh=p_Mesh;					}
	void setRobotArmMesh(MeshNode* p_Mesh)					{ m_RobotArmMesh = p_Mesh;					}
	void addDamagedDiffuseTexture(TextureNode* p_Texture)	{ m_DamagedDiffuseTexture=p_Texture;		}
	void addDamagedNormalTexture(TextureNode* p_Texture)	{ m_DamagedNormalTexture=p_Texture;			}
	void addDamagedEmissiveTexture(TextureNode* p_Texture)	{ m_DamagedEmissiveTexture=p_Texture;		}
	void addDamagedSpecularTexture(TextureNode* p_Texture)	{ m_DamagedSpecularTexture=p_Texture;		}
	void setHitPoints(const unsigned int p_HP)				{ m_HitPoints = p_HP;						}
	int getHitPoints()										{ return m_HitPoints;						}
	void dealDamage(const unsigned int p_DamageAmount)		{ m_HitPoints-=p_DamageAmount;				}
	void setMaxHitPoints(const unsigned int p_MaxHP)		{ m_MaxHitPoints = p_MaxHP;					}
	void setDifficulty(const unsigned int p_Difficulty);
	void addInternalLight(LightNode *p_light);
	void addInternalDamagedLight(LightNode *p_light);
	virtual ~RobotGenerator(void);
protected:
	bool deleteAll(Robot* p_Robot);
	RobotHead* getHead();
	RobotArm* getArm(bool p_Left);
	Robot* getRobotFromPool();
	bool m_Reusing;
	float m_BaseTimer;
	static const int MAX_ROBOTS = 5;
	TextureNode		*m_DefaultRobotDiffuse,
					*m_DamagedRobotDiffuse,

					*m_DefaultRobotEmissive,
					*m_DamagedRobotEmissive,

					*m_DamagedRobotNormal,
					*m_DefaultRobotNormal,

					*m_DamagedRobotSpecular,
					*m_DefaultRobotSpecular,

					*m_DamagedSpecular,
					*m_DamagedDiffuseTexture,
					*m_DamagedEmissiveTexture,
					*m_DamagedNormalTexture,
					*m_DamagedSpecularTexture;
	MeshNode* m_RobotBodyMesh;
	MeshNode* m_RobotHeadMesh;
	MeshNode* m_RobotArmMesh;
	LightNode	*m_interalPointLight,
				*m_internalDamagedPointLight;
	std::list<Robot*> m_Robots;
	unsigned int m_Level; //number of the current level of difficulty
	glm::vec3	m_RobotSpawnPoint,
				m_InternalLightOffset;
	int m_HitPoints;
	unsigned int m_MaxHitPoints;
	float m_SpawnDelay;
	float m_SpawnTimer;
	float m_SpawnDistance;
	int m_NumRobots;

};

