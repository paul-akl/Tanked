#pragma once

#include "EnemyNode.h"
#include "Maze.h"
#include "SceneNode.h"

enum PathFindingStates	// Used for pathfinding algorithm
{
	unfinishedPath	= 0,
	unstartedPath	= 0,
	foundPath		= 1,
	nonexistentPath = 2
};

class AIManager
{
public:
	AIManager(void);
	~AIManager(void);

	void setMaze(Maze *p_Maze);			// Sets current maze, takes in a pointer to a MazeNode
	void setPlayer(SceneNode *p_sceneNode);	// Sets current player, takes in a pointer to a SceneNode
	void doBehaviour(EnemyNode *p_Enemy);	// Process AI behaviour of EnemyNode, calls behaviour functions internaly

protected:
	Maze *m_maze;
	SceneNode *m_player;
	EnemyNode *m_currentEnemy;

	void scoreGrid();								
	void doHostileBehaviour(EnemyNode *p_Enemy);
	void doAlertBehaviour(EnemyNode *p_Enemy); 
	void doPassiveBehaviour(EnemyNode *p_Enemy); 
	bool isPlayerVisible(EnemyNode *p_Enemy); 

	// Main pathfinding algorithm, takes in start and target position in world space coordinates
	glm::vec2 getNextPathCell(glm::vec3 p_startPosition, glm::vec3 p_targetPosition);
	
	// Variables only used by pathfinding
	std::vector<int>	m_openList,		
						m_openCoordX,		
						m_openCoordY,	
						m_fCost,			
						m_hCost;
std::vector<std::vector<int>>	m_parentCoordX,	
								m_parentCoordY,		
								m_closedList,		
								m_gCost;		
	int m_pathLength,		
		m_pathLocation;		
};