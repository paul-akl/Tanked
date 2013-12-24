#pragma once
#include <glm\glm.hpp>
#include <vector>
#include "CollidableNode.h"
#include "CollisionPair.h"
// data structure for sorting a scene into small sections which are then used to generate collision pairs
// maximum width would ideally be the size of a game level, with X and Z, axes used for height/width.
// each Quadtree can store a number of collidable nodes that fall into it's area, but only if that quad tree is of
// the maximum depth (which in turn, is a binary subdivision of the level space)
// the premise is that N collidables will produce N^N possible collisions, which is an intractable problem,
// on large scales. By subdiving area spaces, we can reduced the number of pairs generates, down to nodes
// which are spacially "near" each other. once the scene has been sorted in this way, we can then
// populate a much smaller list of potential collisions. 
// collision detection problems remain (N-1)^2 (O(n^2)), but on a much smaller scale, so an upper limit is placed on
// the intractable problem, making it more managable.
// example of principle:
// 10 collidable items will generate 99 pairs (n*N)-1). By dividing them spatially, into a group of 4 and a group of 6
// the number of pairs drops to 34 ((5^2) + (3^2) = (25+9), which is a 64% reduction in problem size
// a further subdivision into 2 groups of 2 and 2 groups of 3 produces 8 pairs. Thus the problem has been reduced by 92%,
// after only two subdivisions.

class CollisionQuadTree
{
public:
	CollisionQuadTree(void);
	CollisionQuadTree(glm::vec3 p_Centre, const float p_Width, const float p_Height,const int p_Depth);
	void insert(CollidableNode* p_Node);
	void generatePairs(std::vector<CollisionPair*>& p_Pairs);
	void cullPairs(std::vector<CollisionPair*>& p_Pairs);
	void toConsole();
	virtual ~CollisionQuadTree(void);
private:
	bool contains(CollidableNode* p_Node);
	bool overLaps(CollidableNode* p_Node);
	glm::vec3 m_CentrePosition;
	float m_Width;
	float m_Height;
	static const int MAX_DEPTH = 2;
	int m_Depth;
	CollisionQuadTree* m_NW;
	CollisionQuadTree* m_NE;
	CollisionQuadTree* m_SE;
	CollisionQuadTree* m_SW;
	std::vector<CollidableNode*> m_Nodes;
	std::string m_Name;
};

