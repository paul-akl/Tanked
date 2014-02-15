#include "CollisionQuadTree.h"
#include <iostream>
#include <list>
CollisionQuadTree::CollisionQuadTree(void)
{
	m_NW = nullptr;
	m_NE = nullptr;
	m_SW = nullptr;
	m_SE = nullptr;
	m_Depth = 0;
	m_Width = 0;
	m_Height = 0;
}
void CollisionQuadTree::update()
{
	if(!m_Objects.empty())
	{
		//this function is used to check if objects are moving, and if so, containment needs to be re-checked 
		for(std::list<CollidableNode*>::iterator it = m_Objects.begin();it!=m_Objects.end();)
		{
			if((*it)->isActive())
			{
				if((*it)->isMoving())
				{
					if(!contains((*it)))
					{
						if(m_Parent!=nullptr)
						{
							m_Parent->insert((*it));
							it = m_Objects.erase(it);
						}
						else it++;
					}
					else it++;
				}
				else it++;
			}
			else
			{
				it = m_Objects.erase(it);
			}
		}
	}
	if(m_NW!=nullptr)
	{
		m_NW->update();
	}
	if(m_NE!=nullptr)
	{
		m_NE->update();
	}
	if(m_SW!=nullptr)
	{
		m_SW->update();
	}
	if(m_SE!=nullptr)
	{
		m_SE->update();
	}
}
CollisionQuadTree::CollisionQuadTree(glm::vec3 p_Centre, const float p_Width, const float p_Height,const int p_Depth,CollisionQuadTree* p_Parent)
{
	m_Depth = p_Depth;
	m_Width = p_Width;
	m_Height = p_Height;
	m_CentrePosition = p_Centre;
	if(m_Depth < MAX_DEPTH)
	{
		m_NW = new CollisionQuadTree(m_CentrePosition+glm::vec3(-m_Width/2.0f,0.0f,-m_Height/2.0f),m_Width/2.0f,m_Height/2.0f,m_Depth+1, this);
		m_NE = new CollisionQuadTree(m_CentrePosition+glm::vec3( m_Width/2.0f,0.0f,-m_Height/2.0f),m_Width/2.0f,m_Height/2.0f,m_Depth+1, this);
		m_SE = new CollisionQuadTree(m_CentrePosition+glm::vec3( m_Width/2.0f,0.0f, m_Height/2.0f),m_Width/2.0f,m_Height/2.0f,m_Depth+1, this);
		m_SW = new CollisionQuadTree(m_CentrePosition+glm::vec3(-m_Width/2.0f,0.0f, m_Height/2.0f),m_Width/2.0f,m_Height/2.0f,m_Depth+1, this);
	}
	else
	{
		m_NW = nullptr;
		m_NE = nullptr;
		m_SW = nullptr;
		m_SE = nullptr;
	}
	m_Parent = p_Parent;
}
void CollisionQuadTree::toConsole()
{
	if(!m_Objects.empty())
	{
		for(std::list<CollidableNode*>::iterator it = m_Objects.begin();it != m_Objects.end(); it++)
		{
			std::cout<<(*it)->getName()<<std::endl;
		}
	}

	if(m_NW!=nullptr)
	{
		std::cout<<"NW"<<std::endl;
		m_NW->toConsole();
	}
	if(m_NE!=nullptr)
	{
		std::cout<<"NE"<<std::endl;
		m_NE->toConsole();
	}
	if(m_SW!=nullptr)
	{
		std::cout<<"SW"<<std::endl;
		m_SW->toConsole();
	}
	if(m_SE!=nullptr)
	{
		std::cout<<"SE"<<std::endl;
		m_SE->toConsole();
	}
}

bool CollisionQuadTree::contains(CollidableNode* p_Node)
{
	glm::vec3 pos(p_Node->getLocation().x,0.0f,p_Node->getLocation().z);
	glm::vec3 seperation = pos-m_CentrePosition;
	if(p_Node->getBoundaryType() == CIRCLE)
	{
		if(seperation.x < m_Width/2.0f && seperation.z < m_Height/2.0f
			&&	pos.x-p_Node->getRadius() > m_CentrePosition.x-m_Width/2.0f
			&&	pos.x+p_Node->getRadius() < m_CentrePosition.x+m_Width/2.0f
			&&	pos.z-p_Node->getRadius() > m_CentrePosition.x-m_Height/2.0f
			&&	pos.z+p_Node->getRadius() < m_CentrePosition.x+m_Height/2.0f)
			return true;
		else
			return false;
	}
	if(p_Node->getBoundaryType() == AAB)
	{
		if(seperation.x < m_Width/2.0f && seperation.z < m_Height/2.0f)
			if(pos.x-p_Node->getWidth()/2.0f > m_CentrePosition.x-m_Width/2.0f)
				if(pos.x+p_Node->getWidth()/2.0f < m_CentrePosition.x+m_Width/2.0f)
					if(pos.z-p_Node->getLength()/2.0f > m_CentrePosition.z-m_Height/2.0f)
						if(pos.z+p_Node->getLength()/2.0f < m_CentrePosition.z+m_Height/2.0f)
							return true;
		return false;
	}
	if(p_Node->getBoundaryType() == OBB)
	{
		//TODO
		return false;
	}
	if(p_Node->getBoundaryType()==RAY)
	{
		//TODO
		return false;
	}
	if(p_Node->getBoundaryType()==POINT)
	{
		//TODO
		return false;
	}
	else
	{
		return false;
	}
}
bool CollisionQuadTree::validPair(CollidableNode* p_NodeA, CollidableNode* p_NodeB)
{
	switch(p_NodeA->getType())
	{
		case PLAYER:
			{
				switch(p_NodeB->getType())
				{
					case PLAYER:
						{
							return false;
						}
						break;
					case ENEMY:
						{
							return true;
						}
						break;
					case ENEMY_ROBOT:
						{
							return true;
						}
						break;
					case PROJECTILE:
						{
							return false;
						}
						break;
					case SCENERY:
						{
							return true;
						}
						break;
					case COLLECTABLE:
						{
							return true;
						}
						break;
					case MISC:
						{
							return false;
						}
						break;
					case PHASING:
						{
							return false;
						}
						break;
				};
			}
			break;
		case ENEMY:
			{
				switch(p_NodeB->getType())
				{
					case PLAYER:
						{
							return true;
						}
						break;
					case ENEMY:
						{
							return true;
						}
						break;
					case ENEMY_ROBOT:
						{
							return true;
						}
						break;
					case PROJECTILE:
						{
							return true;
						}
						break;
					case SCENERY:
						{
							return false;
						}
						break;
					case COLLECTABLE:
						{
							return false;
						}
						break;
					case MISC:
						{
							return false;
						}
						break;
					case PHASING:
						{
							return false;
						}
						break;
				};
			}
			break;
		case ENEMY_ROBOT:
			{
				switch(p_NodeB->getType())
				{
					case PLAYER:
						{
							return true;
						}
						break;
					case ENEMY:
						{
							return true;
						}
						break;
					case ENEMY_ROBOT:
						{
							return false;
						}
						break;
					case PROJECTILE:
						{
							return true;
						}
						break;
					case SCENERY:
						{
							return false;
						}
						break;
					case COLLECTABLE:
						{
							return false;
						}
						break;
					case MISC:
						{
							return false;
						}
						break;
					case PHASING:
						{
							return false;
						}
						break;
				};
			}
			break;
		case PROJECTILE:
			{
				switch(p_NodeB->getType())
				{
					case PLAYER:
						{
							return false;
						}
						break;
					case ENEMY:
						{
							return true;
						}
						break;
					case ENEMY_ROBOT:
						{
							return true;
						}
						break;
					case PROJECTILE:
						{
							return false;
						}
						break;
					case SCENERY:
						{
							return true;
						}
						break;
					case COLLECTABLE:
						{
							return true;
						}
						break;
					case MISC:
						{
							return false;
						}
						break;
					case PHASING:
						{
							return false;
						}
						break;
				};
			}
			break;
		case SCENERY:
			{
				switch(p_NodeB->getType())
				{
					case PLAYER:
						{
							return true;
						}
						break;
					case ENEMY:
						{
							return true;
						}
						break;
					case ENEMY_ROBOT:
						{
							return false;
						}
						break;
					case PROJECTILE:
						{
							return true;
						}
						break;
					case SCENERY:
						{
							return true;
						}
						break;
					case COLLECTABLE:
						{
							return true;
						}
						break;
					case MISC:
						{
							return false;
						}
						break;
					case PHASING:
						{
							return false;
						}
						break;
				};
			}
			break;
		case COLLECTABLE:
			{
				switch(p_NodeB->getType())
				{
					case PLAYER:
						{
							return true;
						}
						break;
					case ENEMY:
						{
							return false;
						}
						break;
					case ENEMY_ROBOT:
						{
							return false;
						}
						break;
					case PROJECTILE:
						{
							return false;
						}
						break;
					case SCENERY:
						{
							return false;
						}
						break;
					case COLLECTABLE:
						{
							return false;
						}
						break;
					case MISC:
						{
							return false;
						}
						break;
					case PHASING:
						{
							return false;
						}
						break;
				};
			}
			break;
		case MISC:
			{
				return false;
			}
			break;
		case PHASING:
			{
				return false;
			}
			break;
	};
}
bool CollisionQuadTree::overLaps(CollidableNode* p_Node)
{
	glm::vec3 pos(p_Node->getLocation().x,0.0f,p_Node->getLocation().z);
	glm::vec3 seperation = pos-m_CentrePosition;
	float maxDisplacement = glm::sqrt((m_Width*m_Width)+(m_Height*m_Height));
	if(p_Node->getBoundaryType() ==AAB || p_Node->getBoundaryType() == OBB)
	{
		if((seperation.x <= p_Node->getWidth()+m_Width) && seperation.z <= m_Height+p_Node->getLength())
		{
			return true;
		}
		return false;
	}
	else if(p_Node->getBoundaryType() == CIRCLE)
	{
		if (glm::length(seperation)<=maxDisplacement+p_Node->getRadius())
		{
			return true;
		}
	}
	return false;
}
void CollisionQuadTree::collideObject(CollidableNode* p_Node, std::list<CollisionPair*>& p_Results)
{
	if(!m_Objects.empty())
	{
		for(std::list<CollidableNode*>::iterator it = m_Objects.begin();it!=m_Objects.end();it++)
		{
			if(validPair(p_Node,(*it)))
			{
				p_Results.push_front(new CollisionPair(p_Node,(*it)));
			}
		}
	}
	if(m_NW!=nullptr)
	{
		if(m_NW->contains(p_Node) || m_NW->overLaps(p_Node))
		{
			m_NW->collideObject(p_Node,p_Results);
		}	
	}

	if(m_SW!=nullptr)
	{
		if(m_SW->contains(p_Node) || m_SW->overLaps(p_Node))
		{
			m_SW->collideObject(p_Node,p_Results);
		}
	}

	if(m_NE!=nullptr)
	{
		if(m_NE->contains(p_Node) || m_NE->overLaps(p_Node))
		{
			m_NE->collideObject(p_Node,p_Results);
		}
	}

	if(m_SE!=nullptr)
	{
		if(m_SE->contains(p_Node) || m_SE->overLaps(p_Node))
		{
			m_SE->collideObject(p_Node,p_Results);
		}
	}

}

void CollisionQuadTree::insert(CollidableNode* p_Node)
{
	// MUST BE REVIEWED AND RECODED FOR NEW INSERTION ALGORITHM:
	// STEP 1: CHECK IF COLLIDABLE CONTAINS THIS NODE(is the collidable bigger than the node?):
	// IF TRUE, INSERT INTO ALL SUBTREES
	// ELSE
	// STEP 2: CHECK IF COLLIDABLE INTERSECTS SUBNODES(Does this object affect the current nodes children?):
	// IF TRUE, INSERT INTO ALL INTERSECTED SUBTREES
	// ELSE
	// STEP 3: CHECK IF SUBTREES CONTAIN CELL(object is implicitly smaller than node, is it positioned inside it?).
	// IF TRUE, INSERT INTO CONTAINING SUBTREE.
	if(m_Depth >= MAX_DEPTH)
	{
		m_Objects.push_front(p_Node);
	}
	else
		//subtree insertion
	{
		if(m_NW->contains(p_Node))
		{
			m_NW->insert(p_Node);
		}
		else if(m_NE->contains(p_Node))
		{
			m_NE->insert(p_Node);
		}
		else if(m_SE->contains(p_Node))
		{
			m_SE->insert(p_Node);
		}
		else if(m_SW->contains(p_Node))
		{
			m_SW->insert(p_Node);
		}
		else
		{
			m_Objects.push_back(p_Node);
		}
	}
}


CollisionQuadTree::~CollisionQuadTree(void)
{
	while(!m_Objects.empty())
	{
		m_Objects.pop_back();
	}
	delete m_NW;
	delete m_NE;
	delete m_SE;
	delete m_SW;
}
