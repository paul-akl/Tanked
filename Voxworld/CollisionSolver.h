#pragma once
#include "CollisionPair.h"
#include "CollisionQuadTree.h"
#include <vector>
class CollisionSolver
{
public:
	CollisionSolver(void);
	void init(const float& p_WorldWidth,const float& p_WorldLength, const glm::vec3& p_WorldCentrePosition);
	void addCollidable(CollidableNode* p_Collidable);
	//these methods add objects to the quadtrees
	void addTank(CollidableNode* p_Collidable);
	void addScenery(CollidableNode* p_Collidable);
	void addProjectile(CollidableNode* p_Collidable);
	void addEnemy(CollidableNode* p_Collidable);
	void addCollectable(CollidableNode* p_Collidable);
	//these are called during update phase.
	void collideObject(CollidableNode* p_Collidable, std::vector<CollisionPair*>& p_Results);
	void update();
	//finally, this is called to process the final pairs of objects.
	void getResults(std::vector<CollisionPair*>& p_Results);
	virtual ~CollisionSolver(void);
protected:
	virtual void processCollision(CollisionPair* p_Pair);
	void circleVScircle(CollisionPair* p_Pair);
	void circleVSAAB(CollisionPair* p_Pair);
	bool pointVScircle(glm::vec3 p_Point,glm::vec3 p_CircleCentre, float p_Radius);
	bool circleVSLine(glm::vec3 p_Point1, glm::vec3 p_Point2, glm::vec3 p_CircleCentre, float p_Radius);
	std::vector<CollidableNode*> m_Collidables;
	std::vector<CollisionPair*> m_Pairs;
	// multiple trees for maximized efficiency
	CollisionQuadTree* m_TreeTool;
	CollisionQuadTree* m_WallTree;
	CollisionQuadTree* m_ProjectileTree;
	CollisionQuadTree* m_EnemyTree;
	CollisionQuadTree* m_CollectableTree;

};

