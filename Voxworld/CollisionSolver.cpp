#include "CollisionSolver.h"
#include "Scene.h"
#include <iostream>

CollisionSolver::CollisionSolver(void)
{
	m_TreeTool = nullptr;
}
void CollisionSolver::CollisionPass(Scene* p_Scene)
{
	p_Scene->detectCollisions(this);
}
void CollisionSolver::addCollidable(CollidableNode* p_Collidable)
{
	m_Collidables.push_back(p_Collidable);
}
void CollisionSolver::generatePairs(float p_Width, float p_Height, glm::vec3 p_Centre)
{
	//create a new quad tree, and insert collidables
	if(m_TreeTool==nullptr)
		m_TreeTool = new CollisionQuadTree(p_Centre,p_Width,p_Height,1);
	for(size_t i = 0; i < m_Collidables.size();i++)
	{
		m_TreeTool->insert(m_Collidables[i]);
		m_Collidables[i] = nullptr;
	}
	//now collidables are sorted spacially, generate pairs
	//m_TreeTool->toConsole();
	m_TreeTool->generatePairs(m_Pairs);
	//std::cout<<m_Pairs.size()<<" pairs "<<std::endl;

	//m_TreeTool->cullPairs(m_Pairs);
	//std::cout<<m_Pairs.size()<<" pairs "<<std::endl;
	//for (size_t i = 0; i < m_Pairs.size();i++)
	//{
	//	if(m_Pairs[i]!=nullptr)
	//	{
	//		m_Pairs[i]->toConsole();
	//	}
	//}
	m_Collidables.clear();
}
void CollisionSolver::getResults(std::vector<CollisionPair*>& p_Results)
{
	for (size_t i = 0; i < m_Pairs.size(); i++)
	{
		if(m_Pairs[i]!=nullptr)
		if(m_Pairs[i]->hasCollided())
		{
			p_Results.push_back(m_Pairs[i]);
			m_Pairs[i] = nullptr;
		}
	}
	m_Pairs.clear();
}
void CollisionSolver::processCollisions()
{
	for(size_t i = 0;i < m_Pairs.size();i++)
	{
		processCollision(m_Pairs[i]);
	}
}
void CollisionSolver::processCollision(CollisionPair* p_Pair)
{

	///////////////////////////////////////////////////////////////////
	//NEW CODE NEW CODE NEW CODE NEW CODE NEW CODE NEW CODE NEW CODE //
	///////////////////////////////////////////////////////////////////
	if(p_Pair!=nullptr)
	switch (p_Pair->m_Collidable_A->getType())
	{
		case PLAYER:
			{
				switch(p_Pair->m_Collidable_B->getType())
				{
				case SCENERY:
					{
						p_Pair->setResultType(PLAYERVSSCENERY);
						circleVSAAB(p_Pair);
					}break;
				case PROJECTILE:
					{
						p_Pair->setResultType(PLAYERVSPROJECTILE);
						circleVScircle(p_Pair);
					}break;
				case COLLECTABLE:
					{
						p_Pair->setResultType(PLAYERVSCOLLECTABLE);
						circleVScircle(p_Pair);
					}break;
				default:
					{
						p_Pair->setResultType(PLAYERVSSCENERY);
						circleVSAAB(p_Pair);
					}break;
				}
			}break;
		case ENEMY:
			{
				switch(p_Pair->m_Collidable_B->getType())
				{
				case SCENERY:
					{
						p_Pair->setResultType(ENEMYVSSCENERY);
						circleVSAAB(p_Pair);
					}break;
				case PROJECTILE:
					{
						p_Pair->setResultType(ENEMYVSPROJECTILE);
						circleVScircle(p_Pair);
					}break;
				default:
					{
						p_Pair->setResultType(ENEMYVSSCENERY);
						circleVSAAB(p_Pair);
					}break;
				}
			}break;
		case PROJECTILE:
			{
				switch(p_Pair->m_Collidable_B->getType())
				{
				case SCENERY:
					{
						p_Pair->setResultType(PROJECTILEVSSCENERY);
						circleVSAAB(p_Pair);
					}break;
				default:
					{
						p_Pair->setResultType(PROJECTILEVSSCENERY);
						circleVSAAB(p_Pair);
					}break;
				}
			}break;
		case SCENERY:
			{
				switch(p_Pair->m_Collidable_B->getType())
				{
				case PLAYER:
					{
						p_Pair->setResultType(PLAYERVSSCENERY);
						circleVSAAB(p_Pair);
					}break;
				case PROJECTILE:
					{
						p_Pair->setResultType(PROJECTILEVSSCENERY);
						circleVSAAB(p_Pair);
					}break;
				default:
					{
						p_Pair->setResultType(ENEMYVSSCENERY);
						circleVSAAB(p_Pair);
					}break;
				}
			}break;
		case COLLECTABLE:
			{
				switch(p_Pair->m_Collidable_B->getType())
				{
				case PLAYER:
					{
						p_Pair->setResultType(PLAYERVSCOLLECTABLE);
						circleVScircle(p_Pair);
					}break;
				}
			}break;
	}
}
void CollisionSolver::circleVScircle(CollisionPair* p_Pair)
{
	//simplest of all collision detection, radius checks
	glm::vec3 circleA(p_Pair->m_Collidable_A->getLocation().x,0.0f,p_Pair->m_Collidable_A->getLocation().z);
	glm::vec3 circleB(p_Pair->m_Collidable_B->getLocation().x,0.0f,p_Pair->m_Collidable_B->getLocation().z);
	glm::vec3 AtoB = circleA-circleB;
	float minDistance = p_Pair->m_Collidable_A->getRadius()+p_Pair->m_Collidable_B->getRadius();
	float distance = glm::length(AtoB);
	glm::vec3 v_Direction = glm::normalize(AtoB);
	if(distance <= minDistance)
	{
		p_Pair->setCollided(true);
		p_Pair->setCollisionPoint(p_Pair->m_Collidable_A->getLocation()+v_Direction*(p_Pair->m_Collidable_A->getRadius()));
	}

}
void CollisionSolver::circleVSAAB(CollisionPair* p_Pair)
{
	CollidableNode* v_BoxCollider = nullptr;
	CollidableNode* v_CircleCollider = nullptr;
	std::vector<glm::vec3> v_Points;
	if(p_Pair->m_Collidable_A->getBoundaryType() == AAB)
	{
		v_BoxCollider = p_Pair->m_Collidable_A;
		v_CircleCollider = p_Pair->m_Collidable_B;
	}
	else
	{
		v_CircleCollider = p_Pair->m_Collidable_A;
		v_BoxCollider = p_Pair->m_Collidable_B;
	}
	//a collision in this case is that either one of the corners of the box intersects the circle:
	//points in anti clockwise order
	//top left
	glm::vec3 boxPos(v_BoxCollider->getLocation().x,0.0f,v_BoxCollider->getLocation().z);

	v_Points.push_back(boxPos+glm::vec3(-v_BoxCollider->getWidth()/2.0f,0.0f,-v_BoxCollider->getLength()/2.0f));
	v_Points.push_back(boxPos+glm::vec3(-v_BoxCollider->getWidth()/2.0f,0.0f,v_BoxCollider->getLength()/2.0f));
	v_Points.push_back(boxPos+glm::vec3(v_BoxCollider->getWidth()/2.0f,0.0f,v_BoxCollider->getLength()/2.0f));
	v_Points.push_back(boxPos+glm::vec3(v_BoxCollider->getWidth()/2.0f,0.0f,-v_BoxCollider->getLength()/2.0f));

	glm::vec3 circlePos = glm::vec3(v_CircleCollider->getLocation().x,0.0f,v_CircleCollider->getLocation().z);
	//now run checks
	for(size_t i = 0; i < 4; i ++)
	{
		if(pointVScircle(v_Points[i],circlePos,v_CircleCollider->getRadius()))
		{
			p_Pair->m_CollisionNormal = glm::normalize(v_Points[i]-circlePos);
			p_Pair->m_Collided = true;
			p_Pair->m_Penetration = v_CircleCollider->getRadius() - glm::length((circlePos-v_Points[i]));
			return;
		}
	}
	//or the circle is intersecting the edge of the box, depending on the size of each collider
	/////////////////////////////////////////
	//this code could be heavily optimised///
	/////////////////////////////////////////
	if(!p_Pair->hasCollided())
	{
		for(size_t i = 0; i < 4; i++)
		{
			size_t secondIndex = 0;
			if(i+1 >=4)
				secondIndex = 0;
			else
				secondIndex = i+1;
			if(circleVSLine(v_Points[i],v_Points[secondIndex],circlePos,v_CircleCollider->getRadius()))
			{
				glm::vec3 v_Edge = (v_Points[secondIndex]-v_Points[i]);
				//calculate collision normal, and penetration depth along that normal
				p_Pair->m_CollisionNormal = glm::normalize(glm::vec3(-v_Edge.z,0,v_Edge.x));
				p_Pair->m_Collided = true;
				float lineDotN = glm::dot(p_Pair->m_CollisionNormal,v_Points[i]);
				float cDotN = glm::dot(p_Pair->m_CollisionNormal,v_CircleCollider->getLocation());
				p_Pair->m_Penetration = v_CircleCollider->getRadius()-(cDotN-lineDotN);
				return;
			}
		}
	}
}
bool CollisionSolver::circleVSLine(glm::vec3 p_Point1, glm::vec3 p_Point2, glm::vec3 p_CircleCentre, float p_Radius)
{
	//find the line
	//find the closest point on the line
	//find the distance between the centre of the circle and the line
	//if less than radius, collision is true
	//penetration depth = radius - (distance between circle centre and closest point)
	glm::vec3 v_Edge = p_Point2-p_Point1;
	glm::vec3 normal = glm::normalize(v_Edge);
	normal = glm::vec3(-normal.z,normal.y,normal.x);
	//projections to calculate distance
	float v_CircleLineProj = glm::dot(glm::normalize(v_Edge),p_CircleCentre);
	float v_EdgeProjectionMin = glm::dot(glm::normalize(v_Edge),p_Point1);
	float v_EdgeProjectionMax = glm::dot(glm::normalize(v_Edge),p_Point2);
	//distance & location of closest point
	float v_ClosestDistance = v_CircleLineProj-v_EdgeProjectionMin;
	glm::vec3 v_ClosestPoint = p_Point1+glm::normalize(v_Edge)*v_ClosestDistance;
	if(glm::length(p_CircleCentre-v_ClosestPoint) <= p_Radius)
	{
		if(v_CircleLineProj >= v_EdgeProjectionMin && v_CircleLineProj <= v_EdgeProjectionMax)
			return true;
		return false;
	}
	return false;
	
}
bool CollisionSolver::pointVScircle(glm::vec3 p_Point,glm::vec3 p_CircleCentre, float p_Radius)
{
	float distance = glm::length(p_Point - p_CircleCentre);
	if(distance <= p_Radius)
	{
		return true;
	}
	return false;
}
CollisionSolver::~CollisionSolver(void)
{
	delete m_TreeTool;
}
