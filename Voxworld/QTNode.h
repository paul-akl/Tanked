#pragma once
#include <glm/glm.hpp>
#include <list>
#include "CollidableNode.h"
//a much simplified version of original CollisionQuadTree code
//has no member functions, as the CollisionSolver host will deal directly with nodes.
class QTNode
{
public:
	QTNode(void);
	~QTNode(void);
	QTNode* m_Parent;
	QTNode* m_Children[4];
	glm::vec3 m_Centre;
	float m_halfWidth;
	int m_Depth;
	std::list<CollidableNode*> m_Objects;
};

