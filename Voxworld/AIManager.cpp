#include "AIManager.h"
#include <math.h> 

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
	glm::vec2 enemLoc, enemMoveLoc;
	enemLoc =  m_maze->getGridCell(p_Enemy->getLocation());
	enemMoveLoc=m_maze->getGridCell(p_Enemy->getMovementTarget());
	p_Enemy->setTargetPosition(m_player->getLocation());
	if(isPlayerVisible(p_Enemy))
	{
		p_Enemy->setMovementTarget(m_player->getLocation());
		p_Enemy->setState(HostileStatus);
	}
	else
	{
		if(enemLoc == enemMoveLoc)
		{
			if(enemLoc == m_maze->getGridCell(m_player->getLocation()))
				p_Enemy->setMovementTarget(m_player->getLocation());
			else
				p_Enemy->setMovementTarget(m_maze->getCellPosition(getNextPathCell(p_Enemy->getLocation(), m_player->getLocation())));
		
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
	glm::vec2 pPos = m_maze->getGridCell(m_player->getLocation());
	//construct a 2d Ray to trace path from enemy to player
	glm::vec2 ePos = m_maze->getGridCell(p_Enemy->getLocation());


	return m_maze->isVisible(ePos,pPos);

	//return	(enemyPosition.x - playerPosition.x)*(enemyPosition.x - playerPosition.x) + 
	//		(enemyPosition.y - playerPosition.y)*(enemyPosition.y - playerPosition.y) + 
	//		(enemyPosition.z - playerPosition.z)*(enemyPosition.z - playerPosition.z) >= p_Enemy->getDetectionRadius()*p_Enemy->getDetectionRadius();
}

glm::vec2 AIManager::getNextPathCell(glm::vec3 p_startPosition, glm::vec3 p_targetPosition)
{
	int a = 0, b = 0, m = 0, u = 0, v = 0, temp = 0, corner = 0, 
		currentOpen = 0, numOpenList = 0, gTemp = 0, gAdded = 0, path = 0,
		parentXcoord = 0, parentYcoord = 0, openListID = 0, newOpenListID = 0,
		xTemp = 0, xPath = 0, yPath = 0, cellPosition = 0, closedListOffset = 0,
		startPosX = 0, startPosY = 0, targetPosX = 0, targetPosY = 0;

	m_pathLength = 0;		
	m_pathLocation = 0;
	
	std::fill(m_openList.begin(), m_openList.end(), 0);
	std::fill(m_openCoordX.begin(), m_openCoordX.end(), 0);
	std::fill(m_openCoordY.begin(), m_openCoordY.end(), 0);
	std::fill(m_fCost.begin(), m_fCost.end(), 0);
	std::fill(m_hCost.begin(), m_hCost.end(), 0);

	for(unsigned int i=0; i < m_parentCoordX.size(); i++)
	{
		std::fill(m_parentCoordX[i].begin(), m_parentCoordX[i].end(), 0);
		std::fill(m_parentCoordY[i].begin(), m_parentCoordY[i].end(), 0);
		std::fill(m_closedList[i].begin(), m_closedList[i].end(), 0);
		std::fill(m_gCost[i].begin(), m_gCost[i].end(), 0);
	}

	glm::vec2 startPos = m_maze->getGridCell(p_startPosition);		// Get starting cell
	startPosX = (int)startPos.x;
	startPosY = (int)startPos.y;
	
	glm::vec2 targetPos = m_maze->getGridCell(p_targetPosition);	// Get target cell
	targetPosX = (int)targetPos.x;
	targetPosY = (int)targetPos.y;

	if(startPosX == targetPosX && startPosY == targetPosY)			// Check if start point is at the same cell as target point
	{
		return glm::vec2(startPosX, startPosY);
	}

	if(m_maze->getGridCellType(targetPosX, targetPosY) == Unwalkable)	// Check if target point is a walkable cell
	{
		//return glm::vec2(0.0f, 0.0f);
	}

	//if(closedListOffset > 10000)	// Empty a shared closed list, once every now and then
	//{
	//	for(int i = 0; i < m_closedList.size(); i++)
	//		std::fill(m_closedList[i].begin(), m_closedList[i].end(), 0);
			//for(int j = 0; j < m_closedList[i].size(); j++)
				//m_closedList[i][j] = 0;
		closedListOffset = 0;	
	//}

	closedListOffset += 2;
	currentOpen--;
	m_pathLength = unstartedPath;
	m_pathLocation = unstartedPath;
	m_gCost[startPosX][startPosY] = 0;
	
	numOpenList = 1;
	m_openList[1] = 1;
	m_openCoordX[1] = startPosX; 
	m_openCoordY[1] = startPosY;

	while(true)		// Loop is infinite, since it exits from different places within
	{
		// If there are entries present in the open list,
		// process an entry with the lowest F cost
		if(numOpenList != 0)
		{
			// Remove first item fron the binary heap tree (as it is the lowest F cost)
			parentXcoord = m_openCoordX[m_openList[1]];
			parentYcoord = m_openCoordY[m_openList[1]];
			m_closedList[parentXcoord][parentYcoord] = closedListOffset;
			numOpenList--;
			
			m_openList[1] = m_openList[numOpenList + 1];
			v = 1;

			// Resort binary tree, so the first entry has the lowest value
			while(1)
			{
				u = v;
				// If all of the children are present,
				// get the one with a lower value
				if(u*2 + 1 <= numOpenList)
				{
					if (m_fCost[m_openList[u]] >= m_fCost[m_openList[2*u]]) 
						v = 2*u;
					if (m_fCost[m_openList[v]] >= m_fCost[m_openList[2*u + 1]]) 
						v = 2*u + 1;		
				}
				else
				{
					// If only one child is present,
					// check if it's value is lower than it's parent
					if (2*u <= numOpenList)
						if (m_fCost[m_openList[u]] >= m_fCost[m_openList[2*u]]) 
							v = 2*u;
				}

				if(v != u)
				{
					temp = m_openList[u];
					m_openList[u] = m_openList[v];
					m_openList[v] = temp;
				}
				else
					break;
			}
			
			// Process neighbour cells
			for(b = parentYcoord - 1; b <= parentYcoord + 1; b++)
			{
				for(a = parentXcoord - 1; a <= parentXcoord + 1; a++)
				{
					// Check if cell is inside the grid
					if(a != -1 && b != -1 && a != m_maze->getNumColumns() && b != m_maze->getNumRows())
					{
						// Check if cell isn't already on the closed list
						if(m_closedList[a][b] != closedListOffset)
						{
							// Check if cell is walkable
							if(m_maze->getGridCellType(a, b) != Unwalkable)
							{
								corner = Walkable;
								if(a == parentXcoord - 1)
								{
									if (b == parentYcoord - 1)
									{
										if(m_maze->getGridCellType(parentXcoord - 1, parentYcoord) == Unwalkable || m_maze->getGridCellType(parentXcoord, parentYcoord - 1) == Unwalkable)
											corner = Unwalkable;
									}
									else
									{
										if(b == parentXcoord + 1)
											if(m_maze->getGridCellType(parentXcoord, parentYcoord + 1) == Unwalkable || m_maze->getGridCellType(parentXcoord - 1, parentYcoord) == Unwalkable) 
												corner = Unwalkable; 
									}
								}
								else
								{
									if(a == parentXcoord + 1)
									{
										if(b == parentYcoord - 1)
										{
											if(m_maze->getGridCellType(parentXcoord, parentYcoord - 1) == Unwalkable || m_maze->getGridCellType(parentXcoord+1, parentYcoord) == Unwalkable) 
												corner = Unwalkable;
										}
										else
										{
											if(b == parentYcoord + 1)
												if(m_maze->getGridCellType(parentXcoord + 1, parentYcoord) == Unwalkable || m_maze->getGridCellType(parentXcoord, parentYcoord + 1) == Unwalkable)
													corner = Unwalkable; 
										}
									}
								}

								if(corner == Walkable)
								{
									// Check if cell is not on open list already
									if(m_closedList[a][b] != currentOpen)
									{
										// Add cell to open list, assign an ID, add it to binary tree
										newOpenListID++;
										m = numOpenList+1;
										m_openList[m] = newOpenListID;
										m_openCoordX[newOpenListID] = a;
										m_openCoordY[newOpenListID] = b;
										
										// Calculate cell's G cost
										if(abs(a - parentYcoord) == 1 && abs(b - parentYcoord) == 1)
											gAdded = 14;
										else	
											gAdded = 10;		
										m_gCost[a][b] = m_gCost[parentXcoord][parentYcoord] + gAdded;

										// Calculate cell's H and F cost
										m_hCost[m_openList[m]] = 10*(abs(a - targetPosX) + abs(b - targetPosY));
										m_fCost[m_openList[m]] = m_gCost[a][b] + m_hCost[m_openList[m]];
										m_parentCoordX[a][b] = parentXcoord ; m_parentCoordY[a][b] = parentYcoord;

										while (m != 1) // While not on top in binary heap
										{
											if (m_fCost[m_openList[m]] <= m_fCost[m_openList[m/2]])
											{
												temp = m_openList[m/2];
												m_openList[m/2] = m_openList[m];
												m_openList[m] = temp;
												m = m/2;
											}
											else
												break;
										}
										
										numOpenList++;

										m_closedList[a][b] = currentOpen;
									}
									else
									{
										if(abs(a - parentXcoord) == 1 && abs(b - parentYcoord) == 1)
											gAdded = 14; // Diagonal cost
										else	
											gAdded = 10; // Vertical and horizontal cost			
										gTemp = m_gCost[parentXcoord][parentYcoord] + gAdded;

										if(gTemp < m_gCost[a][b])
										{
											m_parentCoordX[a][b] = parentXcoord;
											m_parentCoordY[a][b] = parentYcoord;
											m_gCost[a][b] = gTemp;			

											// Recalculate F cost
											for(int x=1; x <= numOpenList; x++)
											{
												if(m_openCoordX[m_openList[x]] == a && m_openCoordY[m_openList[x]] == b)
												{
													m_fCost[m_openList[x]] = m_gCost[a][b] + m_hCost[m_openList[x]];
													// Reposition the item in the heap, if it is needed
													m = x;
													while(m != 1)
													{
														if(m_fCost[m_openList[m]] < m_fCost[m_openList[m/2]])
														{
															temp = m_openList[m/2];
															m_openList[m/2] = m_openList[m];
															m_openList[m] = temp;
															m = m/2;
														}
														else
															break;
													} 
													break;
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}

		}
		else
		{
			// Path doesn't exist
			path = nonexistentPath;
			break;
		}

		if (m_closedList[(int)targetPosX][(int)targetPosY] == currentOpen)
		{
			// Path has been found, exit the loop
			path = foundPath; 
			break;
		}
	}

	// If the path is found, return the next step from starting cell
	// This is done by tracing the path backwards from target to start cell
	if (path == foundPath)
	{
		int xTest = 0, yTest = 0;
		xPath = targetPosX; yPath = targetPosY;
		if(m_parentCoordX[xPath][yPath] != startPosX || m_parentCoordY[xPath][yPath] != startPosY)
{
		do
		{
			// Get parent cells
			xTemp = m_parentCoordX[xPath][yPath];		
			yPath = m_parentCoordY[xPath][yPath];
			xPath = xTemp;
		// If parent cells are the same as starting cell, stop the loop

			if(xPath < -100 || yPath < -100)
			{
				int xTest2 = m_parentCoordX[xTest][yTest];
				int yTest2 = m_parentCoordY[xTest][yTest];

				int test = xTest * yTest;
			}
			xTest = xPath;
			yTest = yPath;

		}while(m_parentCoordX[xPath][yPath] != startPosX || m_parentCoordY[xPath][yPath] != startPosY);

		// Return the last parent cell, which is the next step from starting position
		return glm::vec2(xPath, yPath);
}
else
	{
		return glm::vec2(xPath, yPath);
	}


	}
	else
	{
		return glm::vec2(xPath, yPath);
	}
	
	// Path doesn't exist or error
	//return glm::vec3(0.0f, 0.0f, 0.0f);
}