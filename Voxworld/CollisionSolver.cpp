#include "CollisionSolver.h"
#include "Scene.h"
#include <iostream>
#include <algorithm>

CollisionSolver::CollisionSolver(void)
{
	m_StaticRoot = nullptr;
	m_PairBoss = nullptr;
}
void CollisionSolver::init(float p_WorldWidth,glm::vec3 p_WorldCentre)
{
	m_PairBoss = new CollisionPairManager();
	m_PairBoss->init();
	m_StaticRoot = buildTree(0,p_WorldWidth*0.5f,p_WorldCentre);
}
QTNode* CollisionSolver::buildTree(int p_Depth, float p_HalfWidth, glm::vec3 centre)
{
	QTNode* p_Root = new QTNode();
	p_Root->m_Centre = centre;
	p_Root->m_halfWidth = p_HalfWidth;
	p_Root->m_Depth = p_Depth;

	if(p_Depth < MAX_DEPTH)
	{
		//recursively construct 4 subchildren
		glm::vec3 offset;
		float step = p_HalfWidth*0.5f;
		for (int i =0; i < 4; i++)
		{
			offset.x = ((i & 1) ? step : -step);
			offset.z = ((i & 2) ? step : -step);
			p_Root->m_Children[i] = buildTree(p_Depth+1,step,p_Root->m_Centre+offset);
			p_Root->m_Children[i]->m_Parent = p_Root;
		}
	}
	return p_Root;

}
void CollisionSolver::insert(QTNode* p_Node, CollidableNode* p_Collidable)
{
	int index =  0, straddle = 0;
	//compute quadrant number 0...3
	//if straddling the X or Z planes, exit directly.
	if(p_Node->m_Depth < MAX_DEPTH)
	{
		//if straddling, insert into list, else find quadrant and insert
		float deltaX = p_Collidable->getLocation().x - p_Node->m_Centre.x;
		float deltaZ = p_Collidable->getLocation().z - p_Node->m_Centre.z;
		if(abs(deltaX) < p_Node->m_halfWidth+p_Collidable->getWidth()*0.5f)
		{
			straddle = 1;
		}
		if(abs(deltaZ) < p_Node->m_halfWidth+p_Collidable->getLength()*0.5f)
		{
			straddle = 1;
		}
		if(!straddle)
		{
			if(p_Collidable->getLocation().x > p_Node->m_Centre.x) index+=1;
			if(p_Collidable->getLocation().z > p_Node->m_Centre.z) index+=2;
			if(p_Node->m_Children[index]!=nullptr)
			{
				insert(p_Node->m_Children[index],p_Collidable);
			}
			else
				p_Node->m_Objects.push_front(p_Collidable);
		}
		else
			p_Node->m_Objects.push_front(p_Collidable);
	}
	else
	{
		p_Node->m_Objects.push_front(p_Collidable);
	}
}
bool CollisionSolver::overlaps(QTNode* p_Node,CollidableNode* p_Collidable)
{
	float dX =  abs(p_Node->m_Centre.x-p_Collidable->getLocation().x);
	float dZ =  abs(p_Node->m_Centre.z-p_Collidable->getLocation().z);
	if(p_Collidable->getBoundaryType()==CIRCLE)
	{
		if(dX < p_Collidable->getRadius()+p_Node->m_halfWidth && dZ < p_Collidable->getRadius()+p_Node->m_halfWidth)
			return true;
		return false;
	}
	else if(p_Collidable->getBoundaryType()==AAB)
	{
		if(dX < (p_Collidable->getWidth()*0.5f)+p_Node->m_halfWidth && dZ < (p_Collidable->getLength()*0.5f)+p_Node->m_halfWidth)
			return true;
		return false;
	}
	else return false;
}
void CollisionSolver::CollideObject(QTNode* p_Node,CollidableNode* p_Collidable, std::vector<CollisionPair*>& p_Pairs)
{
	if(p_Node->m_Depth < MAX_DEPTH)
	{
		if(!p_Node->m_Objects.empty())
		{
			for (std::list<CollidableNode*>::iterator it = p_Node->m_Objects.begin();it != p_Node->m_Objects.end();it++)
			{
				//match types and perform appropriate collision detection
				//case projectile : case scenery: circle vs aab
				//					case ENEMY: circle vs circle
				//same for enemy_ROBOT
				//same for PLAYER
				CollisionPair* pair = circleVsAAB(p_Collidable->getLocation(),(*it)->getLocation(),p_Collidable->getRadius(),(*it)->getWidth(),(*it)->getLength());
				if(pair!=nullptr && pair!=NULL)
				{
					//positive collision.
					pair->m_Collidable_B = (*it);
					pair->m_Collidable_A = p_Collidable;
					//calculate point of contact and move circle outside collision zone.
					float ratio = pair->m_Penetration;
					ratio/=glm::dot(p_Collidable->getVelocity(),pair->m_CollisionNormal);
					glm::vec3 position = p_Collidable->getLocation();
					glm::vec3 v = p_Collidable->getVelocity();
					float x = position.x+v.x*ratio*5.0f;
					float z = position.z+v.z*ratio*5.0f;
					p_Collidable->setPosition(glm::vec3(x,0.0f,z));
					pair->m_CollisionPoint = p_Collidable->getLocation()-pair->m_CollisionNormal*p_Collidable->getRadius();
					switch(p_Collidable->getType())
					{
					case PROJECTILE:
						{
							pair->m_ResultType = PROJECTILEVSSCENERY;
						}
						break;
					case PLAYER:
						{
							pair->m_ResultType = PLAYERVSSCENERY;
						}break;
					case ENEMY_ROBOT:
						{
							pair->m_ResultType = ENEMYVSSCENERY;
						}
					};
					p_Pairs.push_back(pair);
					//identify entity types
				}
			}
		}
		if(overlaps(p_Node->m_Children[0],p_Collidable))
		{
			CollideObject(p_Node->m_Children[0],p_Collidable,p_Pairs);
		}
		if(overlaps(p_Node->m_Children[1],p_Collidable))
		{
			CollideObject(p_Node->m_Children[1],p_Collidable,p_Pairs);
		}
		if(overlaps(p_Node->m_Children[2],p_Collidable))
		{
			CollideObject(p_Node->m_Children[2],p_Collidable,p_Pairs);
		}
		if(overlaps(p_Node->m_Children[3],p_Collidable))
		{
			CollideObject(p_Node->m_Children[3],p_Collidable,p_Pairs);
		}
	}
}
bool CollisionSolver::contains(QTNode* p_Node,CollidableNode* p_Collidable)
{
	glm::vec3 pos = p_Collidable->getLocation();
	float dX =  abs(p_Node->m_Centre.x-pos.x);
	float dZ =  abs(p_Node->m_Centre.z-pos.z);
	if(p_Collidable->getBoundaryType()==CIRCLE)
	{
		if(dX <= p_Node->m_halfWidth-p_Collidable->getRadius()&& dZ <= p_Node->m_halfWidth-p_Collidable->getRadius())
			return true;
		return false;
	}
	else if(p_Collidable->getBoundaryType()==AAB)
	{
		if(dX <= p_Node->m_halfWidth-p_Collidable->getWidth() && dZ <= p_Node->m_halfWidth-p_Collidable->getLength())
			return true;
		return false;
	}
	else return false;
}
void CollisionSolver::updateTree(QTNode* p_Node)
{
	if(!p_Node->m_Objects.empty())
	{
		for (std::list<CollidableNode*>::iterator it = p_Node->m_Objects.begin();it != p_Node->m_Objects.end();)
		{
			if(!(*it)->isActive())
			{
				it = p_Node->m_Objects.erase(it);
			}
			else
				it++;
		}
	}
	if(!(p_Node->m_Depth < MAX_DEPTH))
	{
		updateTree(p_Node->m_Children[0]);
		updateTree(p_Node->m_Children[1]);
		updateTree(p_Node->m_Children[2]);
		updateTree(p_Node->m_Children[3]);
	}
}
void CollisionSolver::addTank(CollidableNode* p_Tank)
{
	m_Tanks.push_front(p_Tank);
}
void CollisionSolver::addProjectile(CollidableNode* p_Projectile)
{
	m_Projectiles.push_front(p_Projectile);
}
void CollisionSolver::addEnemy(CollidableNode* p_Enemy)
{
	m_Enemies.push_front(p_Enemy);
}
void CollisionSolver::addCollectable(CollidableNode* p_Collectable)
{
	m_Collectables.push_front(p_Collectable);
}
void CollisionSolver::addAreaEffect(CollidableNode* p_AOE)
{
	m_AreaEffects.push_front(p_AOE);
}
void CollisionSolver::addEnemyGen(CollidableNode* p_Gen)
{
	m_EnemyGens.push_front(p_Gen);
}
void CollisionSolver::addScenery(CollidableNode* p_Scenery)
{
	//m_Scenery.push_front(p_Scenery);
	insert(m_StaticRoot,p_Scenery);
}
int CollisionSolver::updateProjectiles(void* ptr)
{
	for (std::list<CollidableNode*>::iterator it = m_Projectiles.begin(); it != m_Projectiles.end();)
	{
		if(!(*it)->isActive())
		{
			//remove from list
			it = m_Projectiles.erase(it);
		}
		else
			it++;
	}
	return 0;
}
int CollisionSolver::updateEnemies(void* ptr)
{
	for (std::list<CollidableNode*>::iterator it = m_Enemies.begin(); it != m_Enemies.end();)
	{
		if(!(*it)->isActive())
		{
			//remove from list
			it = m_Enemies.erase(it);
		}
		else
			it++;
	}
	return 0;
		
}
int CollisionSolver::updateCollectables(void* ptr)
{
	for (std::list<CollidableNode*>::iterator it = m_Collectables.begin(); it != m_Collectables.end();)
	{
		if(!(*it)->isActive())
		{
			//remove from list
			it = m_Collectables.erase(it);
		}
		else
			it++;
	}
	return 0;
}
int CollisionSolver::updateTanks(void* ptr)
{
	for (std::list<CollidableNode*>::iterator it = m_Tanks.begin(); it != m_Tanks.end();)
	{
		if(!(*it)->isActive())
		{
			//remove from list
			it = m_Tanks.erase(it);
		}
		else
			it++;
	}
	return 0;
}
int CollisionSolver::updateAreaEffects(void* ptr)
{
	for (std::list<CollidableNode*>::iterator it = m_AreaEffects.begin(); it != m_AreaEffects.end();)
	{
		if(!(*it)->isActive())
		{
			//remove from list
			it = m_Collectables.erase(it);
		}
		else
			it++;
	}
	return 0;
}
//simple function to update lists, designed to be compatible with c style declaration, to favour threading
//example prototype: int _cdecl (void* ptr) 
int CollisionSolver::updateList(void* p_List)
{
	std::list<CollidableNode*>* list = static_cast<std::list<CollidableNode*>*>(p_List);
	for (std::list<CollidableNode*>::iterator it = list->begin(); it != list->end();)
	{
		if(!(*it)->isActive())
		{
			//remove from list
			it = list->erase(it);
		}
		else
			it++;
	}
	return 0;
}
void CollisionSolver::reset()
{
	delete m_StaticRoot;
	m_StaticRoot = nullptr;
	m_Pairs.clear();
	m_Projectiles.clear();
	m_Scenery.clear();
	m_Enemies.clear();
	m_Tanks.clear();
	m_Collectables.clear();
	m_EnemyGens.clear();
	//not used yet, but will be
	m_AreaEffects.clear();
}
void CollisionSolver::update(float p_DeltaTime)
{
	//this can be multi-threaded
	//////////////////////////////////////
	//remove inactive entries from lists
	//one method for each thread
	int num;
	projectileThread = SDL_CreateThread(updateList,"projthread",&m_Projectiles);
	tankThread = SDL_CreateThread(updateList,"tankthread",&m_Tanks);
	enemyThread = SDL_CreateThread(updateList,"enemythread",&m_Enemies);
	collectableThread = SDL_CreateThread(updateList,"collectableThread",&m_Collectables);
	genThread = SDL_CreateThread(updateList,"collectableThread",&m_EnemyGens);
	//wait until threads are complete then clean up.
	SDL_WaitThread(projectileThread, &num);
	SDL_WaitThread(enemyThread, &num);
	SDL_WaitThread(tankThread, &num);
	SDL_WaitThread(collectableThread, &num);
	SDL_WaitThread(genThread,&num);
	//projectileThread = SDL_CreateThread(updateList,"projthread",&m_AreaEffects);

}
void CollisionSolver::processCollisions(std::vector<CollisionPair*>& p_Pairs)
{
	//this could potentially be mutithreaded, if re-coded carefully
	//check projectiles against enemies, walls
	if (!m_Projectiles.empty())
	{
		for (std::list<CollidableNode*>::iterator it = m_Projectiles.begin();it != m_Projectiles.end();it++)
		{
			if(!m_Enemies.empty())
			{
				for (std::list<CollidableNode*>::iterator jt =  m_Enemies.begin(); jt != m_Enemies.end();jt++)
				{
					//do circle/circle check
					float overlap = circleVsCircle((*it)->getRadius(),(*jt)->getRadius(),(*it)->getLocation(),(*jt)->getLocation());
					if(overlap > 0)
					{
						CollisionPair* pair = m_PairBoss->getPair();
						pair->m_Collidable_A = (*it);
						pair->m_Collidable_B = (*jt);
						pair->m_Collided = true;
						pair->m_Penetration = sqrt(overlap);
						pair->m_CollisionNormal = (glm::normalize( (*it)->getLocation() - (*jt)->getLocation() ));
						pair->m_CollisionPoint = (*jt)->getLocation() + pair->m_CollisionNormal*overlap;
						//pair->m_Collidable_B->setPosition(pair->m_Collidable_B->getLocation()-pair->m_CollisionNormal*pair->m_Penetration);
						pair->m_ResultType = ENEMYVSPROJECTILE;
						p_Pairs.push_back(pair);
					}
					//get overlap of two circles. if overlap is <=0 collision is true
					//create a new collision pair, input absolute overlap as penetration depth
					//collision normal is glm::normalize(aPos-bPos)
				}
			}
			CollideObject(m_StaticRoot,(*it),p_Pairs);
			if(!m_EnemyGens.empty())
			{
				for (std::list<CollidableNode*>::iterator jt = m_EnemyGens.begin(); jt!=m_EnemyGens.end();jt++)
				{
						//do circle/circle check
						float overlap = circleVsCircle((*it)->getRadius(),(*jt)->getRadius(),(*it)->getLocation(),(*jt)->getLocation());
						if(overlap > 0)
						{
							CollisionPair* pair = m_PairBoss->getPair();
							pair->m_Collidable_A = (*it);
							pair->m_Collidable_B = (*jt);
							pair->m_Collided = true;
							pair->m_Penetration = sqrt(overlap);
							pair->m_CollisionNormal = (glm::normalize( (*it)->getLocation() - (*jt)->getLocation() ));
							pair->m_CollisionPoint = (*jt)->getLocation() + pair->m_CollisionNormal*overlap;
							pair->m_ResultType = ENEMYGENVSPROJECTILE;
							p_Pairs.push_back(pair);
						}
						//get overlap of two circles. if overlap is >=0 collision is true
						//create a new collision pair, input absolute overlap as penetration depth
						//collision normal is glm::normalize(aPos-bPos)
				}
			}
		}

	}
	//check for enemy/enemy collision
	//for all enemies loop
	if(!m_Enemies.empty())
	{
		for (std::list<CollidableNode*>::iterator it = m_Enemies.begin(); it != m_Enemies.end();it++)
		{
			if(m_Enemies.size()>=2)
			{
				//for current position end, back to current position loop
				std::list<CollidableNode*>::iterator jt = it;
				jt++;
				for (;jt!=m_Enemies.end();jt++)
				{
					//do circle/circle check
					float overlap = circleVsCircle((*it)->getRadius(),(*jt)->getRadius(),(*it)->getLocation(),(*jt)->getLocation());
					if(overlap > 0)
					{
						CollisionPair* pair = m_PairBoss->getPair();
						pair->m_Collidable_A = (*it);
						pair->m_Collidable_B = (*jt);
						pair->m_Collided = true;
						pair->m_Penetration = sqrt(overlap);
						pair->m_CollisionNormal = (glm::normalize( (*it)->getLocation() - (*jt)->getLocation() ));
						float ratio = pair->m_Penetration/glm::dot(pair->m_Collidable_A->getVelocity(),pair->m_CollisionNormal);
						pair->m_Collidable_A->setPosition(pair->m_Collidable_A->getLocation()+pair->m_Collidable_A->getVelocity()*ratio);
						ratio = pair->m_Penetration/glm::dot(pair->m_Collidable_B->getVelocity(),pair->m_CollisionNormal);

						pair->m_Collidable_B->setPosition(pair->m_Collidable_B->getLocation()+pair->m_Collidable_B->getVelocity()*ratio);
						pair->m_CollisionPoint = (*jt)->getLocation() + pair->m_CollisionNormal*pair->m_Penetration;
						pair->m_ResultType = ENEMYVSENEMY;
						p_Pairs.push_back(pair);

					}
					//get overlap of two circles. if overlap is <=0 collision is true
					//create a new collision pair, input absolute overlap as penetration depth
					//collision normal is glm::normalize(aPos-bPos)
				}
			}
			CollideObject(m_StaticRoot,(*it),p_Pairs);
		}
	}
	//check tank against walls, enemies, collectables
	if(!m_Tanks.empty())
	{
		for (std::list<CollidableNode*>::iterator it = m_Tanks.begin();it != m_Tanks.end();it++)
		{
			if(!m_Enemies.empty())
			{
				for (std::list<CollidableNode*>::iterator jt = m_Enemies.begin(); jt != m_Enemies.end();jt++)
				{
					//do circle/circle check
					float overlap = circleVsCircle((*it)->getRadius(),(*jt)->getRadius(),(*it)->getLocation(),(*jt)->getLocation());
					if(overlap > 0)
					{
						CollisionPair* pair = m_PairBoss->getPair();
						pair->m_Collidable_A = (*it);
						pair->m_Collidable_B = (*jt);
						pair->m_Collided = true;
						pair->m_Penetration = sqrt(overlap);
						pair->m_CollisionNormal = (glm::normalize( (*it)->getLocation() - (*jt)->getLocation() ));
						pair->m_CollisionPoint = (*jt)->getLocation() + pair->m_CollisionNormal*pair->m_Penetration;
						pair->m_ResultType = PLAYERVSENEMY;
						p_Pairs.push_back(pair);

					}
					//get overlap of two circles. if overlap is <=0 collision is true
					//create a new collision pair, input absolute overlap as penetration depth
					//collision normal is glm::normalize(aPos-bPos)
				}
			}
			if(!m_EnemyGens.empty())
			{
				for (std::list<CollidableNode*>::iterator jt = m_EnemyGens.begin(); jt!=m_EnemyGens.end();jt++)
				{
						//do circle/circle check
						float overlap = circleVsCircle((*it)->getRadius(),(*jt)->getRadius(),(*it)->getLocation(),(*jt)->getLocation());
						if(overlap > 0)
						{
							CollisionPair* pair = m_PairBoss->getPair();
							pair->m_Collidable_A = (*it);
							pair->m_Collidable_B = (*jt);
							pair->m_Collided = true;
							pair->m_Penetration = sqrt(overlap);
							pair->m_CollisionNormal = (glm::normalize( (*it)->getLocation() - (*jt)->getLocation() ));
							pair->m_CollisionPoint = (*jt)->getLocation() + pair->m_CollisionNormal*pair->m_Penetration;
							pair->m_ResultType = PLAYERVSENEMY;
							p_Pairs.push_back(pair);
						}
						//get overlap of two circles. if overlap is >=0 collision is true
						//create a new collision pair, input absolute overlap as penetration depth
						//collision normal is glm::normalize(aPos-bPos)
				}
			}
			CollideObject(m_StaticRoot,(*it),p_Pairs);

		//additional checks needed for collectable objects.
		}
	}
}
float CollisionSolver::circleVsCircle(float radiusA, float radiusB, glm::vec3 positionA, glm::vec3 positionB)
{
	glm::vec3 delta = positionB-positionA;
	float deltaSquared = glm::dot(delta,delta);
	float combinedRadius = ( radiusA*radiusA ) + ( radiusB*radiusB );
	if(deltaSquared<=combinedRadius)
	{
		return combinedRadius-deltaSquared;
	}
	return -1.0f;
}
float CollisionSolver::pointVScircle(glm::vec3 p_Point,glm::vec3 p_CircleCentre, float p_Radius)
{
	glm::vec3 line = p_Point-p_CircleCentre;
	float deltaSquared = glm::dot(line,line);
	if(deltaSquared <= (p_Radius*p_Radius))
	{
		return deltaSquared-(p_Radius*p_Radius);
	}
	return -1.0f;
}
float CollisionSolver::circleVsLine(glm::vec3 p_PointA,glm::vec3 p_PointB, glm::vec3 p_CircleCentre,float p_CircleRadius)
{
	//find the line
	//find the closest point on the line
	//find the distance between the centre of the circle and the line
	//if less than radius, collision is true
	//penetration depth = radius - (distance between circle centre and closest point)
	glm::vec3 v_Edge = p_PointB-p_PointA;
	glm::vec3 normal = glm::normalize(v_Edge);
	glm::vec3 line = normal;
	normal = glm::vec3(-normal.z,normal.y,normal.x);
	float lineOrigin = glm::dot(normal,p_PointA);
	float circleproj = glm::dot(normal,p_CircleCentre);
	float delta = abs(circleproj-lineOrigin);
	if(delta<=p_CircleRadius)
	{
		return p_CircleRadius-delta;
	}
	return -1.0f;
}
CollisionPair* CollisionSolver::circleVsAAB(glm::vec3 p_CircleCentre, glm::vec3 p_RectCentre, float p_Radius, float p_Width, float p_Height)
{

	//Cases: 
	//	Case 1: no intersection, return null
	//	Case 2: containment, calculate normal + penetration depth.
	//	Case 3: edge intersection, calc normal + penetration depth.
	//	Case 4: corner circle intersection, calc normal + penetration depth
	glm::vec3 delta = p_CircleCentre-p_RectCentre;
	glm::vec3 absDelta = glm::vec3(abs(delta.x),abs(delta.y),abs(delta.z));
	glm::vec3 v_Points[4];
	float halfWidth = p_Width*0.5f;
	float halfHeight = p_Height*0.5f;
	//std::vector<glm::vec3> v_Points;



	if(absDelta.x < halfWidth+p_Radius)
	{
		if(absDelta.z < halfHeight+p_Radius)
		{
			//case 1 ruled out
			//do further collision checking
			//case 2: calculate containment
			if(absDelta.x < halfWidth-p_Radius)
			{
				if(absDelta.z < halfHeight-p_Radius)
				{
					//case 2 is true
					//calculate normal, penetration depth
					CollisionPair* pair = m_PairBoss->getPair();
					//check axis for least penetration
					if(absDelta.x < absDelta.z)
					{
						pair->m_Collided = true;
						pair->m_Penetration = absDelta.z+p_Radius;
						if(delta.z!=0.0f)
						{
							if(delta.z < 0.0f)
							{
								pair->m_CollisionNormal.z = -1.0f;
								pair->m_CollisionNormal.y = 0.0f;
								pair->m_CollisionNormal.x = 0.0f;
							}
							else 
							{
								pair->m_CollisionNormal.z = 1.0f;
								pair->m_CollisionNormal.y = 0.0f;
								pair->m_CollisionNormal.x = 0.0f;
							}
						}
					}
					else
					{
						pair->m_Penetration = absDelta.x+p_Radius;
						if(delta.x !=0.0f)
						{
							if(delta.x < 0.0f)
							{
								pair->m_CollisionNormal.x = -1.0f;
								pair->m_CollisionNormal.y = 0.0f;
								pair->m_CollisionNormal.z = 0.0f;
							}
							else if(delta.x > 0.0f)
							{
								pair->m_CollisionNormal.x = 1.0f;
								pair->m_CollisionNormal.y = 0.0f;
								pair->m_CollisionNormal.z = 0.0f;
							}
						}
					}
					return pair;
				}
				//no containment
				else
				{
					v_Points[0] = p_RectCentre+glm::vec3(-halfWidth,0.0f,-halfHeight);
					v_Points[1] = p_RectCentre+glm::vec3(-halfWidth,0.0f,halfHeight);
					v_Points[2] = p_RectCentre+glm::vec3(halfWidth,0.0f,halfHeight);
					v_Points[3] = p_RectCentre+glm::vec3(halfWidth,0.0f,-halfHeight);
					//case 3 edge circle collision check
					glm::vec3 normal(0.0f);
					float min = 0.0f;
					for (size_t i = 0; i < 4; i++)
					{
						size_t secondIndex = 0;
						if(i+1 < 4)
							secondIndex = i+1;

						float intersection = circleVsLine(v_Points[i],v_Points[secondIndex],p_CircleCentre,p_Radius);
						if(intersection>0.0f)
						{
	
							if(min == 0.0f || intersection < min)
							{
								normal = glm::normalize(v_Points[secondIndex] - v_Points[i]);
								min=intersection;
							}

						}
						else
							continue;
					}
					if(min>0.0f)
					{
						//calculate displacement vector. need more data from circleVsLine
						CollisionPair* pair = m_PairBoss->getPair();
						if(pair!=nullptr)
						{
							pair->m_Collided = true;
							pair->m_Penetration = sqrt(min);
							pair->m_CollisionNormal = glm::vec3(-normal.z,0.0f,normal.x);
							return pair;
						}
						else
							return nullptr;
					}
					else
						return nullptr;
				}
			} //no containment. 
			else
			{
				//case 3 edge circle collision check
				v_Points[0] = p_RectCentre+glm::vec3(-halfWidth,0.0f,-halfHeight);
				v_Points[1] = p_RectCentre+glm::vec3(-halfWidth,0.0f,halfHeight);
				v_Points[2] = p_RectCentre+glm::vec3(halfWidth,0.0f,halfHeight);
				v_Points[3] = p_RectCentre+glm::vec3(halfWidth,0.0f,-halfHeight);
				//case 3 edge circle collision check
				glm::vec3 normal(0.0f);
				float min = 0.0f;
				for (size_t i = 0; i < 4; i++)
				{
					size_t secondIndex = 0;
					if(i+1 < 4)
						secondIndex = i+1;

					float intersection = circleVsLine(v_Points[i],v_Points[secondIndex],p_CircleCentre,p_Radius);
					if(intersection>0.0f)
					{
						if(min == 0.0f || intersection < min)
						{
							normal = glm::normalize(v_Points[secondIndex] - v_Points[i]);
							min=intersection;
						}

					}
					else continue;
				}
				//calculate displacement vector. need more data from circleVsLine
				if(min>0)
				{
					CollisionPair* pair = m_PairBoss->getPair();
					if(pair!=nullptr)
					{
						pair->m_Collided = true;
						pair->m_Penetration = sqrt(min);
						pair->m_CollisionNormal = glm::vec3(-normal.z,0.0f,normal.x);
						//better to calculate collision point based on velocity
						//pair->m_CollisionPoint = p_CircleCentre-pair->m_CollisionNormal*(p_Radius-pair->m_Penetration);
						return pair;
					}
					else return nullptr;
				}
				else
					return nullptr;
			}
		}
		else 
			return nullptr;
	}
	else 
		return nullptr;
}
CollisionSolver::~CollisionSolver(void)
{
	delete m_PairBoss;
	delete m_StaticRoot;
}
