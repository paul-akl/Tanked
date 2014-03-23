#pragma once
#include "CollisionPair.h"
#include "CollisionQuadTree.h"
#include "QTNode.h"
#include <vector>
#include <list>
#include "CollisionPairManager.h"
#include <SDL_thread.h>
class CollisionSolver
{
public:
	CollisionSolver(void);
	void init(float p_WorldWidth,glm::vec3 p_WorldCentre);
	void addTank(CollidableNode* p_Tank);
	void addEnemy(CollidableNode* p_Enemy);
	void addCollectable(CollidableNode* p_Collectable);
	void addAreaEffect(CollidableNode* p_AOE);
	void addProjectile(CollidableNode* p_Projectile);
	void addScenery(CollidableNode* p_Scenery);
	void addEnemyGen(CollidableNode* p_Gen);
	void reset();
	virtual void update(float p_DeltaTimeS);
	virtual void processCollisions(std::vector<CollisionPair*>& p_Pairs);
	virtual ~CollisionSolver(void);
protected:
	QTNode* buildTree(int p_Depth, float p_HalfWidth, glm::vec3 centre);
	void insert(QTNode* p_Node, CollidableNode* p_Collidable);
	void updateTree(QTNode* p_Node);
	void CollideObject(QTNode* p_Node,CollidableNode* p_Collidable, std::vector<CollisionPair*>& p_Pairs);
	bool overlaps(QTNode* p_Node, CollidableNode* p_Collidable);
	bool contains(QTNode* p_Node, CollidableNode* p_Collidable);
	float circleVsCircle(float p_RadiusA, float p_RadiusB, glm::vec3 p_CentreA, glm::vec3 p_CentreB);
	CollisionPair*  circleVsAAB(glm::vec3 p_CircleCentre, glm::vec3 p_RectCentre, float p_Radius, float p_Width, float p_Height);
	float circleVsLine(glm::vec3 p_PointA,glm::vec3 p_PointB, glm::vec3 p_CircleCentre,float p_CircleRadius);
	float pointVScircle(glm::vec3 p_Point,glm::vec3 p_CircleCentre, float p_Radius);
	bool pointVsRect(glm::vec3 p_Point, glm::vec3 axisA, glm::vec3 axisB);
	int updateCollectables(void* ptr);
	int updateEnemies(void* ptr);
	int updateProjectiles(void* ptr);
	int updateAreaEffects(void* ptr);
	int updateTanks(void* ptr);
	static int updateList(void* p_List);
	std::vector<CollisionPair*> m_Pairs;
	std::list<CollidableNode*> m_Projectiles;
	std::list<CollidableNode*> m_Scenery;
	std::list<CollidableNode*> m_Enemies;
	std::list<CollidableNode*> m_Tanks;
	std::list<CollidableNode*> m_Collectables;
	std::list<CollidableNode*> m_EnemyGens;
	//not used yet, but will be
	std::list<CollidableNode*> m_AreaEffects;
	CollisionPairManager* m_PairBoss;
	// multiple trees for maximized efficiency
	SDL_Thread* enemyThread;
	SDL_Thread* tankThread;
	SDL_Thread* collectableThread;
	SDL_Thread* genThread;
	SDL_Thread* areaEffectThread;
	SDL_Thread* projectileThread;
	static const int MAX_DEPTH = 3;
	QTNode* m_StaticRoot;
};

