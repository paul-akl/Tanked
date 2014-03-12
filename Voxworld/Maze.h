#pragma once
#include "Position.h"
#include <vector>
#include <glm\glm.hpp>

enum Passability	// Used for pathfinding and the m_grid array
{
	Walkable,
	Unwalkable
};

enum CellType
{
	CORRIDOR = 0,
	WALL,
	GENERATOR,
	START
};

class MazeIterator
{
public:
	MazeIterator();
	~MazeIterator();

	Position getNext();
	bool hasNext();

private:

	friend class Maze;
	std::vector<Position> m_positionArray;
	int m_index;
};

class Maze
{
public:
	Maze(void);

	void generateMaze(int p_width, int p_height, float p_cellSize);
	bool isOk(glm::vec2 p_Cell);								// Checks if a cell is at least 1 cell away from any wall, in any direction
	bool isOk(glm::vec3 p_Location);							// Takes in world position and converts it into grid cell array position first
	int getGridCellType(int p_row, int p_column);				// Returns GridType enum of a cell as integer
	int getGridCellType(Position p_pos);						// Returns GridType enum of a cell as integer
	glm::vec3 getNearestCell(glm::vec3 p_position);				// Returns a world position of the nearest cell. If p_position is not in the maze, returns nearest edge cell
	glm::vec2 getGridCell(glm::vec3 p_position);				// Returns a grid cell position (row and column), takes in position in world space
	glm::vec3 getCellPosition(glm::vec2 p_cell);				// Returns world position of a cell, takes in grid cell array position (row and column)
	glm::vec3 getCellPosition(Position p_pos);
	MazeIterator *getIterator();
	void toConsole();

	inline int getGridWidth()	{ return m_width * m_CellSize;	}
	inline int getGridHeight()	{ return m_height * m_CellSize; }
	inline int getNumRows()		{ return m_width;	 }
	inline int getNumColumns()	{ return m_width;	 }
	inline float getCellSize()	{ return m_CellSize; }

	~Maze(void);

private:
	bool isOK(Position p_pos);
	bool isWall(Position p_pos);
	bool isValid(Position p_pos);
	void setWall(Position p_pos);
	void setVisited(Position p_pos);
	void createRoom(Position p_pos);

	PosIterator getIterator(Position p_pos);
	int neighbourCount(Position p_pos);

	std::vector<std::vector<int>> m_Grid;
	MazeIterator *m_iterator;
	int m_width, m_height;
	float m_CellSize;
};