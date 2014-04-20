#include "AIManager.h"
#include <math.h> 
#include "AStarIterator.h"
#include <list>
#include <queue>
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

	if(isPlayerVisible(p_Enemy))
	{
		p_Enemy->setMovementTarget(tPos);
		p_Enemy->setState(HostileStatus);
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
				getNextPathCell(p_Enemy->getLocation(), tPos,m_Path);
				if(m_Path.size()>0)
				{
						//this IF statement is just for testing & should be removed once working
						//printf("%s\n",p_Enemy->getName().c_str());
						//printf("target: %f:%f\n",tPos.x,tPos.z);
						//printf("position: %f:%f\n",p_Enemy->getLocation().x,p_Enemy->getLocation().z);
						//printf("player : %f:%f\n",p_Enemy->getLocation().x,p_Enemy->getLocation().y);
						//path contains start position, so pop last element to get next cell
						glm::vec2 pos = glm::vec2((float)m_Path.back().x,(float)m_Path.back().y);
						m_Path.clear();

						//printf("next cell: %f:%f\n",pos.x,pos.y);
						glm::vec3 moveTo = m_maze->getCellPosition(pos);
						//printf("enemyUpdated\n ");
						p_Enemy->setTargetPosition(moveTo);
						p_Enemy->setMovementTarget(moveTo);
						p_Enemy->setState(HostileStatus);
				}
				else
				{
					p_Enemy->setState(PassiveStatus);
					//printf("no path found: %s\n",p_Enemy->getName().c_str());
				}
			}
		}
		else
		{
			//enemy has not reached target location plot new path
			getNextPathCell(p_Enemy->getLocation(), tPos,m_Path);
			if(m_Path.size()>0)
			{
				//printf("%s\n",p_Enemy->getName().c_str());
				//printf("target: %f:%f\n",tPos.x,tPos.z);
				//printf("position: %f:%f\n",p_Enemy->getLocation().x,p_Enemy->getLocation().z);
				//printf("player : %f:%f\n",p_Enemy->getLocation().x,p_Enemy->getLocation().y);
				//path contains start position, so pop last element to get next cell
				glm::vec2 pos = glm::vec2((float)m_Path.back().x,(float)m_Path.back().y);
				m_Path.clear();
				//printf("next cell: %f:%f\n",pos.x,pos.y);
				glm::vec3 moveTo = m_maze->getCellPosition(pos);
				//printf("enemyUpdated\n ");
				p_Enemy->setTargetPosition(moveTo);
				p_Enemy->setMovementTarget(moveTo);
				p_Enemy->setState(HostileStatus);

			}
			else
			{
				p_Enemy->setState(PassiveStatus);
			}
		}
	}
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
	int range =  (int)p_Enemy->getDetectionRadius();
	//create ray direction from resulting vector
	//set increment at player radius
	//for each increment of player radius
	glm::vec3 rayDir = ray / m_player->getBoundingRadius();
	float distToTarget = glm::length(ray);
	if(distToTarget<=p_Enemy->getDetectionRadius())
	{

		for (int i = 0; i <range; i++)
		{
			glm::vec3 position = p_Enemy->getLocation()+rayDir*(float)i;
			glm::ivec2 pos = m_maze->getGridCell(position);
			if(!m_maze->isOk(pos))
			{
				return false;
			}
		}
		return true;
	}
	return false;
	//if loop concludes return false
}
bool comp(AIPosition& a, AIPosition& b)
{
	return a.F>b.F; 
}
int distance(AIPosition& a, AIPosition& b)
{
	return (abs(b.x-a.x)+abs(b.y-a.y));
}
int findPosition(AIPosition& a, std::vector<AIPosition>& v)
{
	for(size_t i = 0; i < v.size();i++)
	{
		if(v[i].x==a.x && v[i].y==a.y)
			return i;
	}
	return -1;
}
void constructPath(std::vector<AIPosition*>& path, AIPosition& foundGoal)
{
	AIPosition *current = new AIPosition(foundGoal);
	while(current->m_Parent!=nullptr)
	{
		path.push_back(current);
		current = current->m_Parent;
	}
}

void AIManager::getNextPathCell(glm::vec3 p_startPosition, glm::vec3 p_targetPosition,std::vector<glm::ivec2>& path)
{
	//set up basic data.
	bool foundPath = false;
	glm::ivec2 start = m_maze->getGridCell(p_startPosition);
	glm::ivec2 end = m_maze->getGridCell(p_targetPosition);
	if(!m_maze->isOk(start))
		return;
	//set up temporary data used in pathfinding
	int gridX = 0, gridY = 0, m = 0, u = 0, v = 0, temp = 0, corner = 0, 
		currentOpen = 0, NotVisitedListSize = 0, gTemp = 0, gAdded = 0,
		parentXcoord = 0, parentYcoord = 0, openListID = 0, newOpenListID = 0,
		xTemp = 0, /*xPath = 0, yPath = 0,*/ cellPosition = 0, closedListOffset = 0;

	m_pathLength = 0;		
	m_pathLocation = 0;
	int numSucessors = 4;
	int sucessorsX[4];
	int sucessorsY[4];
	std::fill(m_openList.begin(), m_openList.end(), 0);
	std::fill(m_openCoordX.begin(), m_openCoordX.end(), 0);
	std::fill(m_openCoordY.begin(), m_openCoordY.end(), 0);
	std::fill(m_fCost.begin(), m_fCost.end(), 0);
	std::fill(m_hCost.begin(), m_hCost.end(), 0);

	for(unsigned int i=0; i < m_parentCoordX.size(); i++)
	{
		std::fill(m_parentCoordX[i].begin(), m_parentCoordX[i].end(), 0);
		std::fill(m_parentCoordY[i].begin(), m_parentCoordY[i].end(), 0);
		std::fill(m_closedList[i].begin(), m_closedList[i].end(),0);
		std::fill(m_gCost[i].begin(), m_gCost[i].end(), 0);
	}

	if(start.x == end.x && start.y == end.y)			// Check if start point is at the same cell as target point
	{
		return;
	}

	currentOpen--;
	m_pathLength = unstartedPath;
	m_pathLocation = unstartedPath;
	m_gCost[start.x][start.y] = 0;
	NotVisitedListSize = 1;
	m_openList[1] = 1;
	m_openCoordX[1] = start.x;
	m_openCoordY[1] = start.y;

	while(NotVisitedListSize>0)
	{
		//make the parent = the top of the min heap Not-visited list
		parentXcoord = m_openCoordX[m_openList[1]];
		parentYcoord = m_openCoordY[m_openList[1]];

		m_closedList[parentXcoord][parentYcoord] = true;
		NotVisitedListSize--;
		m_openList[1] = m_openList[NotVisitedListSize + 1];
		int offset1 = 0;
		int offset2 = 1;
		//check if current position == end
		if(m_closedList[end.x][end.y] == true)
		{
			//if so, then no need to proceed further, only path reconstruction is necessary now.
			foundPath = true;
			break;
		}
		//if(m_openCoordX[m_openList[1]] == end.x && m_openCoordY[m_openList[1]] == end.y)
		//{
		//	//if so, then no need to proceed further, only path reconstruction is necessary now.
		//	foundPath = true;
		//	break;
		//}
		//re-sort the min heap, so that the lowest F value is at the top
		while(true)
		{
			offset1 = offset2;
			//if both children are in list, check both.
			if(offset1 * 2 + 1 <= NotVisitedListSize)
			{
				//if one of the children has a lower F cost, change offset for swapping
				if (m_fCost[m_openList[offset1]] >= m_fCost[m_openList[2*offset1]]) 
					offset2 = 2*offset1;
				if (m_fCost[m_openList[offset2]] >= m_fCost[m_openList[2*offset1 + 1]]) 
					offset2 = 2*offset1 + 1;
			}
			else
			{
				// If only one child is present,
				// check if it's value is lower than it's parent. if so a swap is required
				if (2*offset1 <= NotVisitedListSize)
					if (m_fCost[m_openList[offset1]] >= m_fCost[m_openList[2*offset1]]) 
						offset2 = 2*offset1;
			}
			//if offsets dont match then we have a swap to perform
			if(offset1 != offset2)
			{
				temp = m_openList[offset1];
				m_openList[offset1] = m_openList[offset2];
				m_openList[offset2] = temp;
			}
			//at this stage, we have no swaps left to make, so list is sorted
			else
			{
				break;
			}
		}

		//process list of possible successor moves
		//only 4 here for E/S/W/N

		//north
		sucessorsX[0] = 0+parentXcoord;
		sucessorsY[0] = 1+parentYcoord;
		//south
		sucessorsX[1] = 0+parentXcoord;
		sucessorsY[1] = -1+parentYcoord;
		//east
		sucessorsX[2] = -1+parentXcoord;
		sucessorsY[2] = 0+parentYcoord;
		//west
		sucessorsX[3] = 1+parentXcoord;
		sucessorsY[3] = 0+parentYcoord;
		for (int i = 0; i < numSucessors; i++)
		{

			//check if the successor is either outside of the grid or a wall
			if(!m_maze->isOk(glm::ivec2(sucessorsX[i],sucessorsY[i])))
			{
				//if so, skip to next successor
				continue;
			}
			else
			{
				//if it is a valid move, check if it is already visited (in the closedList)
				//this line is weird: what is closedListOffset? How is this used?
				//closedListOffset is never altered
				if(m_closedList[sucessorsX[i]][sucessorsY[i]] == true)
				{
					//if so, skip to next successor
					continue;
				}
				else
				{
					//if not visited yet, a final check is needed to see if the successor is already in the Not-visited(open) list
					bool onOpenList = false;
					for (int j = 1; j < NotVisitedListSize; j++)
					{
						//this seems to be the actual open list check for current position
						if(m_openCoordX[m_openList[j]] == sucessorsX[i] && m_openCoordY[m_openList[j]] == sucessorsY[i])
						{
							int successorG = m_gCost[parentXcoord][parentYcoord] + 10;
							
							
							//if successor is in open list, if its G score is less than open list entry
							if(successorG < m_gCost[sucessorsX[i]][sucessorsY[i]])
							{
								//recalculate open list entry G Score and set open list entry parent = successor parent
								//add values to the parent grid, which we will use to reconstruct the path later
								m_parentCoordX[sucessorsX[i]][sucessorsY[i]] = parentXcoord;
								m_parentCoordY[sucessorsX[i]][sucessorsY[i]] = parentYcoord;
								m_gCost[sucessorsX[i]][sucessorsY[i]] = successorG;
								//recalculate F cost
								m_fCost[m_openList[j]] = m_gCost[sucessorsX[i]][sucessorsY[i]]+m_hCost[m_openList[j]];
								onOpenList=true;
								int position = j;
								//while position is not root of the min heap
								while(position != 1)
								{
									//if f cost at position is less than parent, swap them
									if(m_fCost[m_openList[position]] < m_fCost[m_openList[position/2]])
									{
										temp = m_openList[position/2];
										m_openList[position/2] = m_openList[position];
										m_openList[position] = temp;
										position /=2;
									}
									else
										break;
								}
								break;
							}
							else
							{
								//if successor G score >= open list entry G, skip to next successor
								continue;
							}
		
							//and re-sort min heap

						}

					}
					if(!onOpenList)
					{
						if(newOpenListID > 676)
						{
							return;
						}
						newOpenListID++;
						int position = NotVisitedListSize+1;
						m_openList[position] = newOpenListID;
						m_openCoordX[newOpenListID] = sucessorsX[i];
						m_openCoordY[newOpenListID] = sucessorsY[i];
						//calculate G cost for new entry
						int successorG = m_gCost[parentXcoord][parentYcoord] + 10;
						// Calculate cell's H and F cost
						m_hCost[m_openList[position]] = 10*(abs(sucessorsX[i] - end.x) + abs(sucessorsY[i] - end.y));
						m_fCost[m_openList[position]] = m_gCost[sucessorsX[i]][sucessorsY[i]] + m_hCost[m_openList[position]];
						//add an entry into parent list for reconstructing path later
						m_parentCoordX[sucessorsX[i]][sucessorsY[i]] = parentXcoord; 
						m_parentCoordY[sucessorsX[i]][sucessorsY[i]] = parentYcoord;
						//and re-sort min heap
						//while position is not root of the min heap
						while(position != 0)
						{
							//if f cost at position is less than parent, swap them
							if(m_fCost[m_openList[position]] < m_fCost[m_openList[position/2]])
							{
								temp = m_openList[position/2];
								m_openList[position/2] = m_openList[position];
								m_openList[position] = temp;
								position /=2;
							}
							else
								break;
						}
						//pretty sure ethis needs to be here, as the open list has had an entry added
						NotVisitedListSize++;
					}
				}
			}
		}
	}
	//loop ends, if path found, reconstruct it

	if(foundPath)
	{
		//printf("path found\n");
		//printf("%i operations\n",newOpenListID);	
		int pathX = end.x, pathY = end.y;
		while(m_parentCoordX[pathX][pathY] != start.x || m_parentCoordY[pathX][pathY] != start.y)
		{
			//place an entry into path list
			path.push_back(glm::ivec2(pathX,pathY));
			int X = m_parentCoordX[pathX][pathY];
			int Y = m_parentCoordY[pathX][pathY];
			pathX = X;
			pathY = Y;
		};
	}
	else
	{
		//printf("no path found\n");
	}
}
