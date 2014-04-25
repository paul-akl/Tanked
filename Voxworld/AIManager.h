#pragma once

#include "EnemyNode.h"
#include "Maze.h"
#include "SceneNode.h"
#include "AStarPool.h"

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
	void init();
	void setMaze(Maze *p_Maze);			// Sets current maze, takes in a pointer to a MazeNode
	void setPlayer(SceneNode *p_sceneNode);	// Sets current player, takes in a pointer to a SceneNode
	void doBehaviour(EnemyNode *p_Enemy);	// Process AI behaviour of EnemyNode, calls behaviour functions internaly

protected:
	Maze *m_maze;
	SceneNode *m_player;
	EnemyNode *m_currentEnemy;
	AStarPool* m_Pool;
	void scoreGrid();								
	void doHostileBehaviour(EnemyNode *p_Enemy);
	void doAlertBehaviour(EnemyNode *p_Enemy); 
	void doPassiveBehaviour(EnemyNode *p_Enemy); 
	bool isPlayerVisible(EnemyNode *p_Enemy); 

	// Main pathfinding algorithm, takes in start and target position in world space coordinates
	void getNextPathCell(glm::vec3 p_startPosition, glm::vec3 p_targetPosition,std::list<glm::vec3> &path);
	// Variables only used by pathfinding
	std::vector<int>	m_openList,		
						m_openCoordX,		
						m_openCoordY,	
						m_fCost,			
						m_hCost;
	std::vector<glm::ivec2> m_Path;
	std::vector<std::vector<int>>	m_parentCoordX,	
									m_parentCoordY,				
									m_gCost;		
	std::vector<std::vector<int>> m_closedList;
	
	int m_pathLength,		
		m_pathLocation;	
	glm::ivec2 m_LastPlayerLocation;
};