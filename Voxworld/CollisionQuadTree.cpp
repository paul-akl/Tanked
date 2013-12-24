#include "CollisionQuadTree.h"
#include <iostream>
CollisionQuadTree::CollisionQuadTree(void)
{

}
CollisionQuadTree::CollisionQuadTree(glm::vec3 p_Centre, const float p_Width, const float p_Height,const int p_Depth)
{
	m_Depth = p_Depth;
	m_Width = p_Width;
	m_Height = p_Height;
	m_CentrePosition = p_Centre;
	m_NW = nullptr;
	m_NE = nullptr;
	m_SE = nullptr;
	m_SW = nullptr;
}
void CollisionQuadTree::toConsole()
{
	if(m_Depth>=MAX_DEPTH)
	{
		if(!m_Nodes.empty())
		for (size_t i = 0; i < m_Nodes.size(); i++)
		{
			std::cout<<m_Nodes[i]->getName()<<std::endl;
		}
		std::cout<<std::endl;
	}
	else
	{
		std::cout<<"NW"<<std::endl;
		m_NW->toConsole();
		std::cout<<"NE"<<std::endl;
		m_NE->toConsole();
		std::cout<<"SE"<<std::endl;
		m_SE->toConsole();
		std::cout<<"SW"<<std::endl;
		m_SW->toConsole();
	}

}
bool CollisionQuadTree::contains(CollidableNode* p_Node)
{
	glm::vec3 pos(p_Node->getLocation().x,0.0f,p_Node->getLocation().z);
	glm::vec3 seperation = pos-m_CentrePosition;

		if ((abs(seperation.x)<m_Width)&&(abs(seperation.z)<m_Height))
		{
			return true;
		}
		return false;
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
		m_Nodes.push_back(p_Node);
	}
	else
	{//NORTHWEST QUADRANT CHECK
		if(m_NW!=nullptr)
		{
			if(m_NW->contains(p_Node)|| m_NW->overLaps(p_Node))
			{
				m_NW->insert(p_Node);
			}
		}
		else
		{
			m_NW = new CollisionQuadTree(m_CentrePosition+glm::vec3(-m_Width/2.0f,0.0f,-m_Height/2.0f),m_Width/2.0f,m_Height/2.0f,m_Depth+1);
			if(m_NW->contains(p_Node)|| m_NW->overLaps(p_Node))
			{
				m_NW->insert(p_Node);
			}
		}
		//NORTHEAST
		if(m_NE!=nullptr)
		{
			if(m_NE->contains(p_Node)|| m_NE->overLaps(p_Node))
			{
				m_NE->insert(p_Node);
			}
		}
		else
		{

			m_NE = new CollisionQuadTree(m_CentrePosition+glm::vec3(m_Width/2.0f,0.0f,-m_Height/2.0f),m_Width/2.0f,m_Height/2.0f,m_Depth+1);
			if(m_NE->contains(p_Node)|| m_NE->overLaps(p_Node))
			{
				m_NE->insert(p_Node);
			}
		}
		//SOUTHWEAST
		if(m_SE!=nullptr)
		{
			if(m_SE->contains(p_Node)|| m_SE->overLaps(p_Node))
			{
				m_SE->insert(p_Node);
			}
		}
		else
		{

			m_SE = new CollisionQuadTree(m_CentrePosition+glm::vec3(m_Width/2.0f,0.0f,m_Height/2.0f),m_Width/2.0f,m_Height/2.0f,m_Depth+1);
			if(m_SE->contains(p_Node)|| m_SE->overLaps(p_Node))
			{
				m_SE->insert(p_Node);
			}
		}
		//SOUTHWEST
		if(m_SW!=nullptr)
		{
			if(m_SW->contains(p_Node)|| m_SW->overLaps(p_Node))
			{
				m_SW->insert(p_Node);
			}
		}
		else
		{

			m_SW = new CollisionQuadTree(m_CentrePosition+glm::vec3(m_Width/2.0f,0.0f,m_Height/2.0f),m_Width/2.0f,m_Height/2.0f,m_Depth+1);
			if(m_SW->contains(p_Node)|| m_SW->overLaps(p_Node))
			{
				m_SW->insert(p_Node);
			}
		}
	}
}
void CollisionQuadTree::cullPairs(std::vector<CollisionPair*>& p_Pairs)
{
	//this is a nasty one, but we are most likely to be dealing with small groups, so it's fine
	//to cull duplicate pairs
	//for each pair
	for (size_t i = 0;i < p_Pairs.size();i++)
	{ 
		if(p_Pairs[i]!=nullptr)
		{
			//for each other pair
			for (size_t j = 0; j < p_Pairs.size();j++)
			{
				if(p_Pairs[j]!=nullptr)
				{
					if(i != j)
					{ 
						//if they contain the same two collidables
						if(p_Pairs[i]->equals(p_Pairs[j]))
						{
							//do nothing
							delete p_Pairs[j];
							p_Pairs[j] = nullptr;
						}
					}
				}
			}
		}
	}
	//Layer culling phase
	//very simple layering for the moment.
	//will be improved on, for projectile collisions.
	//as projectiles may be enemy owned, or player owned.
	//so we will check the type of the parent of the projectile
	//anything with phasing cannot collide so the pair will be deleted
	for (size_t i = 0; i < p_Pairs.size();i++)
	{
		if(p_Pairs[i]!=nullptr)
		{
			switch(p_Pairs[i]->m_Collidable_A->getType())
			{
					//players can hit enemies, collectables, misc, projectiles, and scenery
					///////////////////////////////////////////////
					//this section needs updating for projectile //
					//Parent checking							 //
					///////////////////////////////////////////////
			case PLAYER:
				{
					switch (p_Pairs[i]->m_Collidable_B->getType())
					{
					case PHASING:
						{
							delete p_Pairs[i];
							p_Pairs[i] = nullptr;
						}break;
					default:
						break;
					}
				}break;
					//enemies can hit other enemies, players, scenery, projectiles, and misc
					///////////////////////////////////////////////
					//this section needs updating for projectile //
					//Parent checking							 //
					///////////////////////////////////////////////
				case ENEMY:
				{
					switch (p_Pairs[i]->m_Collidable_B->getType())
					{
					case PHASING:
						{
							delete p_Pairs[i];
							p_Pairs[i] = nullptr;
						}break;
					case COLLECTABLE:
						{
							delete p_Pairs[i];
							p_Pairs[i] = nullptr;
						}break;
					default:
						break;
				}break;
				//projectiles cannot hit phasing and other projectiles
				case PROJECTILE:
				{
					switch (p_Pairs[i]->m_Collidable_B->getType())
					{
						case PHASING:
							{
								delete p_Pairs[i];
								p_Pairs[i] = nullptr;
							}break;
						case PROJECTILE:
							{
								delete p_Pairs[i];
								p_Pairs[i] = nullptr;
							}break;
							default:
								break;
					}
				}break;
				//everything except phasing collides with scenery
				case SCENERY:
				{
					switch (p_Pairs[i]->m_Collidable_B->getType())
						{
						case PHASING:
							{
								delete p_Pairs[i];
								p_Pairs[i] = nullptr;
							}break;
						case SCENERY:
							{
								delete p_Pairs[i];
								p_Pairs[i] = nullptr;
							}break;
						case COLLECTABLE:
							{
								delete p_Pairs[i];
								p_Pairs[i] = nullptr;
							}break;
						default:
							break;
						}
				}break;
				case COLLECTABLE:
				{
					switch (p_Pairs[i]->m_Collidable_B->getType())
						{
						case ENEMY:
							{
								delete p_Pairs[i];
								p_Pairs[i] = nullptr;
							}break;
						case PHASING:
							{
								delete p_Pairs[i];
								p_Pairs[i] = nullptr;	
							}break;
						case PROJECTILE:
							{
								delete p_Pairs[i];
								p_Pairs[i] = nullptr;
							}break;
						case SCENERY:
							{
								delete p_Pairs[i];
								p_Pairs[i] = nullptr;
							}break;
						case MISC:
							{
								delete p_Pairs[i];
								p_Pairs[i] = nullptr;
							}break;
						default:
							break;
						}
				}break;
			case MISC:
				{
					switch (p_Pairs[i]->m_Collidable_B->getType())
						{
						case PROJECTILE:
							{
								delete p_Pairs[i];
								p_Pairs[i] = nullptr;
							}break;
						case MISC:
							{
								delete p_Pairs[i];
								p_Pairs[i] = nullptr;
							}break;
						default:
							break;
						}
				}break;

				default:
					break;
				}
			}
		}
	}
}
void CollisionQuadTree::generatePairs(std::vector<CollisionPair*>& p_Pairs)
{
	if(m_Depth >= MAX_DEPTH)
	{
		if(m_Nodes.size() >= 2)
		{
			std::vector<CollisionPair*> v_Pairs;
			//for each collidable loop
			for(size_t i = 0; i < m_Nodes.size(); i++)
			{
				//for each other collidable loop
				for (size_t j = 0; j < m_Nodes.size(); j++)
				{
					//quick check to see if they are not the same object
					if(j != i)
					{
						//create a pair with them
						v_Pairs.push_back(new CollisionPair(m_Nodes[i],m_Nodes[j]));
					}
				}//end loop
			}//end loop
			cullPairs(v_Pairs);
			for (size_t i = 0; i < v_Pairs.size();i++)
			{
				if (v_Pairs[i] !=nullptr)
				{
					p_Pairs.push_back(v_Pairs[i]);
					v_Pairs[i] = nullptr;
				}
			}
			v_Pairs.clear();
		}
	}
	// max depth not reached
	else
	{
		//recursive calls
		if(m_NW!=nullptr)
		{
			m_NW->generatePairs(p_Pairs);
		}
		if(m_NE!=nullptr)
		{
			m_NE->generatePairs(p_Pairs);
		}
		if(m_SW!=nullptr)
		{
			m_SW->generatePairs(p_Pairs);
		}
		if(m_SE!=nullptr)
		{
			m_SE->generatePairs(p_Pairs);
		}
	}
	for(size_t i = 0; i < m_Nodes.size(); i++)
	{
		m_Nodes[i] = nullptr;
	}
	cullPairs(p_Pairs);
	m_Nodes.clear();
}
CollisionQuadTree::~CollisionQuadTree(void)
{
	if(!m_Nodes.empty())
	for(size_t i = 0; i < m_Nodes.size();i++)
	{
		m_Nodes[i] = nullptr;
	}
	m_Nodes.clear();
	delete m_NW;
	delete m_NE;
	delete m_SE;
	delete m_SW;
}
