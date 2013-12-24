#pragma once
#include "CollisionPair.h"
#include "CollisionQuadTree.h"
#include <vector>
class Scene;
class CollisionSolver
{
public:
	CollisionSolver(void);
	void addCollidable(CollidableNode* p_Collidable);
	void generatePairs(float p_Width, float p_Height, glm::vec3 p_Centre);
	void CollisionPass(Scene* p_Scene);
	void getResults(std::vector<CollisionPair*>& p_Results);
	void processCollisions();
	~CollisionSolver(void);
protected:

	virtual void processCollision(CollisionPair* p_Pair);
	void circleVScircle(CollisionPair* p_Pair);
	void circleVSAAB(CollisionPair* p_Pair);
	bool pointVScircle(glm::vec3 p_Point,glm::vec3 p_CircleCentre, float p_Radius);
	bool circleVSLine(glm::vec3 p_Point1, glm::vec3 p_Point2, glm::vec3 p_CircleCentre, float p_Radius);
	std::vector<CollidableNode*> m_Collidables;
	std::vector<CollisionPair*> m_Pairs;
	CollisionQuadTree* m_TreeTool;
};

