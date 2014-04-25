#include "AIManager.h"
#include <math.h> 
#include "AStarPool.h"
#include <list>
#include <set>
#include <queue>
#include <algorithm>
AIManager::AIManager(void)
{
	m_maze = NULL;
	m_player = NULL;
	m_currentEnemy = NULL;
	//m_parentCoordX = nullptr;
	//m_parentCoordY = nullptr;
	//m_openCoordX = nullptr;
	//m_openCoordY = nullptr;
	//m_parentCoordX = nullptr;
	//m_parentCoordX = nullptr;
}
AIManager::~AIManager(void)
{

}

void AIManager::setMaze(Maze *p_Maze)
{
	m_maze = p_Maze;

	m_openList.resize(m_maze->getNumColumns()*m_maze->getNumRows()+2);
	m_openCoordX.resize(m_maze->getNumColumns()*m_maze->getNumRows()+2);
	m_openCoordY.resize(m_maze->getNumColumns()*m_maze->getNumRows()+2);
	m_fCost.resize(m_maze->getNumColumns()*m_maze->getNumRows()+2);
	m_hCost.resize(m_maze->getNumColumns()*m_maze->getNumRows()+2);

	m_parentCoordX.resize(m_maze->getNumColumns()+1);
	m_parentCoordY.resize(m_maze->getNumColumns()+1);
	m_closedList.resize(m_maze->getNumColumns()+1);
	m_gCost.resize(m_maze->getNumColumns()+1);

	for(int i=0; i<m_maze->getNumColumns()+1; i++)
	{
		m_parentCoordX[i].resize(m_maze->getNumRows()+1);
		m_parentCoordY[i].resize(m_maze->getNumRows()+1);
		m_closedList[i].resize(m_maze->getNumRows()+1);
		m_gCost[i].resize(m_maze->getNumRows()+1);
	}
	
}
void AIManager::setPlayer(SceneNode *p_sceneNode)
{
	m_player = p_sceneNode;
}

void AIManager::scoreGrid()
{

}
void AIManager::doBehaviour(EnemyNode *p_Enemy)
{
//	switch(p_Enemy->getState())
//	{
//	case PassiveStatus:
//		doPassiveBehaviour(p_Enemy);
//		break;
		
//	case AlertStatus:
//		doAlertBehaviour(p_Enemy);
//		break;
		
//	case HostileStatus:
//		doHostileBehaviour(p_Enemy);
//		break;
//	}

	// If enemy has reached the target grid cell, recalculate a path and set next step in a path as a movement target
	glm::ivec2 enemLoc, enemMoveLoc;

	enemLoc =  m_maze->getGridCell(p_Enemy->getLocation());
	enemMoveLoc = m_maze->getGridCell(p_Enemy->getMovementTarget());
	glm::vec3 tPos = m_player->getLocation();
	p_Enemy->setTargetPosition(tPos);
	glm::ivec2 pLoc = (glm::ivec2)m_maze->getGridCell(m_player->getLocation());
	if(p_Enemy->getCurrentTimer()<=0.0f)
	{
		if(isPlayerVisible(p_Enemy))
		{
			p_Enemy->setMovementTarget(tPos);
			p_Enemy->setState(HostileStatus);
			p_Enemy->getWayPointList().clear();
			p_Enemy->getWayPointList().push_front(tPos);
			//printf("target:\n %f:%f\n",tPos.x,tPos.z);
			//aprintf("Robo pos: %d:%d\n",enemLoc.x,enemLoc.y);

		}
		else
		{

			//if enemy has reached movement target
			if((enemLoc.x == enemMoveLoc.x)&&(enemLoc.y == enemMoveLoc.y))
			{
				//if enemy loc matches player loc
				if(abs((int)enemLoc.x - pLoc.x)<2 && abs((int)enemLoc.y - pLoc.y)<2)
				{
					//set direct course for player
					p_Enemy->setMovementTarget(m_player->getLocation());
					p_Enemy->setState(HostileStatus);
					//printf("enemy close: %s\n",p_Enemy->getName().c_str());
					//printf("Robo pos: %f:%f\n",enemLoc.x,enemLoc.y);

				}
				//player not nearby, plot path and take first waypoint
				else
				{
					getNextPathCell(p_Enemy->getLocation(), tPos,p_Enemy->getWayPointList());
				}
			}
			else
			{
				//enemy has not reached target location plot new path
				getNextPathCell(p_Enemy->getLocation(), tPos,p_Enemy->getWayPointList());
			}
		}
		p_Enemy->resetTimer();
	}
	
	m_LastPlayerLocation = pLoc;
}
void AIManager::doHostileBehaviour(EnemyNode *p_Enemy)
{
	//if(!isPlayerVisible(p_Enemy))
	//	p_Enemy->setState(AlertStatus);
	//else
		p_Enemy->setTargetPosition(m_player->getLocation());
		//above needs changed to plot path to player, then set the next cell in the path as target
}
void AIManager::doAlertBehaviour(EnemyNode *p_Enemy)
{
	if(isPlayerVisible(p_Enemy))
		p_Enemy->setState(HostileStatus);
}
void AIManager::doPassiveBehaviour(EnemyNode *p_Enemy)
{
	if(isPlayerVisible(p_Enemy))
		p_Enemy->setState(HostileStatus);
}
bool AIManager::isPlayerVisible(EnemyNode *p_Enemy)
{
	//get player & enemy positions
	glm::vec3 ray = m_player->getLocation() - p_Enemy->getLocation();
	float rad = m_player->getBoundingRadius();
	glm::ivec2 pPos = m_maze->getGridCell(m_player->getLocation());
	int range =  (int)p_Enemy->getDetectionRadius();
	//create ray direction from resulting vector
	//set increment at player radius
	//for each increment of player radius
	glm::vec3 rayDir = glm::normalize(ray);
	float distToTarget = glm::length(ray);
	if(distToTarget<=p_Enemy->getDetectionRadius())
	{
		glm::vec3 position = p_Enemy->getLocation();
		int steps = range/rad;
		for (int i = 0; i <steps; i++)
		{
			position+=rayDir*(rad*i);
			glm::ivec2 pos = m_maze->getGridCell(position);
			if(!m_maze->isOk(pos))
			{
				return false;
			}
			else if(pos==pPos)
			{
				return true;
			}
		}
		return true;
	}
	return false;
	//if loop concludes return false
}

void AIManager::init()
{
	m_Pool = new AStarPool();
	m_Pool->init(512);
}
bool equal(AStarPosition* lhs, AStarPosition* rhs)
{
	return lhs->x==rhs->x && lhs->y==rhs->y;
}
int find(std::vector<AStarPosition*>& p_Vec,AStarPosition* p_SearchParam)
{
	for (size_t i = 0; i < p_Vec.size();i++)
	{
		if(p_Vec[i]->equals(p_SearchParam))
			return i;
	}
	return -1;
}
bool less(AStarPosition* a, AStarPosition* b)
{
	return (a->F > b->F); 
}
void AIManager::getNextPathCell(glm::vec3 p_startPosition, glm::vec3 p_targetPosition,std::list<glm::vec3>& path)
{
	glm::ivec2 startPos = m_maze->getGridCell(p_startPosition);
	glm::ivec2 goalPos = m_maze->getGridCell(p_targetPosition);
	path.clear();
	AStarPosition* start = m_Pool->getPosition(nullptr,startPos.x,startPos.y);
	AStarPosition* goal = m_Pool->getPosition(nullptr,goalPos.x,goalPos.y);
	start->G=0;
	start->estimateCost(goal);
	bool pathFound = false;
	//printf("%d\n",sizeof(AStarPosition));
	//used to hold positions representing visited grid positions 
	std::vector<AStarPosition*> usedList;
	//create a min heap to hold positions
	std::vector<AStarPosition*> openList;
	//add the start to the min heap
	openList.push_back(start);
	std::make_heap(openList.begin(),openList.end(),less);
	//set all closed list values to zero (false)
	//for (size_t i = 0; i < m_closedList.size();i++)
	//{
	//	std::fill(m_closedList[i].begin(),m_closedList[i].end(),0);
	//}
	//main algorithm:
	int a = 0;
	while(openList.size() > 0 && pathFound==false)
	{
		//remove root from openList (position with lowest F cost)
		//std::sort_heap(openList.begin(),openList.end(),less);

		std::pop_heap(openList.begin(),openList.end(),less);

		AStarPosition* parent = openList.back();
		openList.pop_back();
		//openList.erase(openList.begin());
		//if current position == goal
		if(parent->equals(goal))
		{
			pathFound = true;
		}
		else
		{
			//get successor positions (4)
			std::vector<AStarPosition*> successors;
			//NORTH
			successors.push_back(m_Pool->getPosition(parent,0,1));
			//EAST						
			successors.push_back(m_Pool->getPosition(parent,1,0));
			//SOUTH
			successors.push_back(m_Pool->getPosition(parent,0,-1));
			//WEST
			successors.push_back(m_Pool->getPosition(parent,-1,0));
			//for each successor loop
			for (int i = 0; i < 4;i++)
			{
				//calculate G,H,F costs
				successors[i]->G = parent->G+10;
				int heuristic = (abs(goal->x-successors[i]->x)+abs(goal->y-successors[i]->y))*10;
				successors[i]->H = heuristic;
				successors[i]->F = successors[i]->G+successors[i]->H;
				if(heuristic < 0)
							printf("weird heuristic\n");
				//if position is a wall or outside maze maze
				if(!m_maze->isOk(glm::ivec2(successors[i]->x,successors[i]->y)))
				{
					//continue
					m_Pool->release(successors[i]);
					successors[i]=nullptr;
					continue;
				}
				else
				{
					int posO = find(openList,successors[i]);
					int posC = find(usedList,successors[i]);
					//if successor is in closed list
					if(posC>-1)
					{
						//skip this successor
						m_Pool->release(successors[i]);
						successors[i]=nullptr;
						continue;
					}

					//else if current exists within openlist
					else if(posO!=-1)
					{
						//skip this successor
						m_Pool->release(successors[i]);
						successors[i]=nullptr;
						continue;
					}
					//else not in open list
					else
					{
						//insert successor into open list

						openList.push_back(successors[i]);

						////////////////////////////////////////////////////
						//line below crashes program
						std::push_heap(openList.begin(),openList.end(),less);
						successors[i]=nullptr;

					}
				}
			}
			successors.clear();
		}
		usedList.push_back(parent);
	}
	if(pathFound)
	{
		//reconstruct path
		int num = 0;
		int numPos = usedList.size();
		AStarPosition* current = usedList.back();
		//printf("new path\n");
		while(current!=nullptr && !start->equals(current))
		{
			path.push_front(m_maze->getCellPosition(glm::vec2(current->x,current->y)));
			//printf("X:%i,Y:%i\n",current->x,current->y);

			current=current->m_Parent;
			num++;
		}
	}
	//usedList.back()->toConsole();
	//and clean up
	for (std::vector<AStarPosition*>::iterator it = openList.begin();it!=openList.end();)
	{
		m_Pool->release((*it));
		it = openList.erase(it);
	}

	for (std::vector<AStarPosition*>::iterator it = usedList.begin();it!=usedList.end();)
	{
		m_Pool->release((*it));
		it = usedList.erase(it);
	}
	m_Pool->release(start);
	m_Pool->release(goal);

}
