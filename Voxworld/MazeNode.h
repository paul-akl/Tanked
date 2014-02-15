#pragma once
#pragma once

#include "SceneNode.h"
#include <glm/glm.hpp>
#include <string>

enum CellType	// Used for pathfinding and the m_grid array
{
	Walkable,
	Unwalkable
};

enum GridType	// Used for maze generation (reading maze data file)
{
	Floor,
	Wall,
	MonsterGenerator,
	StartPosition
};

class MazeNode
{
public:
	MazeNode(void);
	~MazeNode(void);
	
	bool isOk(glm::vec2 p_cell);					// Checks if a cell is at least 1 cell away from any wall, in any direction
	bool isOk(glm::vec3 p_position);				// Takes in world position and converts it into grid cell array position first
	bool isInside(SceneNode *p_sceneNode);			// Checks if a sceneNode is inside the maze
	bool isPassable(glm::vec3 p_position);			// Checks if a grid cell at p_position is passable (i.e. floor)
	int getGridCellType(int p_row, int p_column);	// Returns GridType enum of a cell as integer
	void generateMaze(std::string p_filename);		// Generates(reads from file) maze, takes in a filename of the data file
	glm::vec3 getNearestCell(glm::vec3 p_position);	// Returns a world position of the nearest cell. If p_position is not in the maze, returns nearest edge cell
	glm::vec2 getGridCell(glm::vec3 p_position);	// Returns a grid cell position (row and column), takes in position in world space
	glm::vec3 getCellPosition(glm::vec2 p_cell);	// Returns world position of a cell, takes in grid cell array position (row and column)
	glm::vec2 nextPosition();						// Returns a next grid cell array position (row and column) of the next cell, determined by an iterator
	bool hasNextPosition();							// Checks if there is a next grid cell in an array (checks if iterator is valid)

	inline float getGridWidth()	{ return m_gridWidth;  }	// Returns a maze width
	inline float getGridHeight(){ return m_gridHeight; }	// Returns a maze height
	inline float getCellWidth()	{ return m_cellWidth;  }	// Returns an individual cell's width
	inline float getCellHeight(){ return m_cellHeight; }	// Returns an individual cell's height
	inline int getNumColumns()	{ return m_numColumns; }	// Returns the total number of columns in the maze
	inline int getNumRows()		{ return m_numRows;	   }	// Returns the total number of rows in the maze

protected:
	float	m_gridWidth,
			m_gridHeight,
			m_cellWidth, 
			m_cellHeight;

	int m_widthIterator,
		m_heightIterator,
		m_numColumns,
		m_numRows,
		**m_grid;

	glm::vec3 m_startPosition;		// Offset position for the whole maze
	bool m_iteratorOutOfBounds;	
};