#include "Maze.h"
#include <stack>
#include <time.h>
#include "PosIterator.h"


MazeIterator::MazeIterator()
{
	m_index = -1;
}
MazeIterator::~MazeIterator()
{
	m_positionArray.clear();
}
Position MazeIterator::getNext()
{
	m_index++;
	return m_positionArray[m_index];
}
bool MazeIterator::hasNext()
{
	return m_index < (int)m_positionArray.size() - 1;
}

Maze::Maze(void)
{
	m_iterator = nullptr;
}

void Maze::generateMaze(int p_width, int p_height, int cellSize)
{
	printf("generating maze\n");
	//init random seed
	srand((unsigned int)time(NULL));
	//initial storage set up
	m_width = p_width;
	m_height = p_height;
	m_CellSize = cellSize;
	//initialise all grid cells to wall
	printf("init grid\n");

	m_Grid.resize(m_width);
	for (int i = 0; i < m_width;i++)


	{
		m_Grid[i].resize(m_height);
		for (int j = 0; j < m_height;j++)
		{
			m_Grid[i][j] = WALL;
		}
	}
	//a list of moves through the grid is needed.
	std::stack<Position> moves;

	//select a random starting position within workable grid area
	printf("start position selected\n");

	int x = rand() % (m_width-2);
	int y = rand() % (m_height-2);
	//add starting position to move stack
	moves.push(Position(x+1,y+1));
	//while the stack is not empty, the grid is not complete
	while ( ! moves.empty() )
	{
		//if the current move is not valid (not in grid, not a wall, not in criteria to become corridor)
		//remove from move list, and backtrack to last position
		if( ! isValid( moves.top() ) )
		{
			moves.pop();
		}
		//position is valid
		else
		{
			//set the current stack top position as visited
			setVisited( moves.top() );
			//printf("setting position %i,%i\n",moves.top().x,moves.top().y);

			PosIterator iter = getIterator( moves.top() );
			while (iter.hasNext())
			{
				moves.push( iter.getNext() );
			}
			//create a new position leading from current
			//this bit is more puzzling, needs research and thought through
			//push to stack
		}
	}
	//grid complete
	//add adjustments:
	//top left corner
	createRoom(Position(4,4));
	m_Grid[4][4] = GENERATOR;
	//top right
	createRoom(Position(m_width-5,4));
	m_Grid[m_width-5][4] = GENERATOR;

	//bottom left
	createRoom(Position(4,m_height-5));
	m_Grid[4][m_height-5] = GENERATOR;

	//bottom right
	createRoom(Position(m_width-5,m_height-5));
	m_Grid[m_width-5][m_height-5] = GENERATOR;

	//create central room
	int offsetX = 0;
	int offsetY = 0;
	if(m_width % 2 == 0)
	{
		offsetX = m_width/2;
	}
	else
	{
		offsetX = (m_width-1)/2;
	}
	if(m_height % 2 == 0)
	{
		offsetY = m_height/2;
	}
	else
	{
		offsetY = (m_height-1)/2;
	}	
	createRoom(Position(offsetX,offsetY));
	m_Grid[offsetX][offsetY] = START;
	//1 add 5 rooms, 1 centre, 4 edge or corner
	//layout: ### ###  5x5 room with item in centre
	//		  #     #
	//		   	 G   
	//		  #		#
	//		  ### ###
	//2 add start position in centre room as above
	//3 add generator positions in edge or corner rooms as above

	//maze complete

	if(m_iterator == nullptr)
		m_iterator = new MazeIterator();
	else
	{
		m_iterator->m_positionArray.clear();
		m_iterator->m_index = -1;
	}
	m_iterator->m_positionArray.clear();
	for(std::vector<std::vector<int>>::size_type i = 0; i < m_Grid.size(); i++)
		for(std::vector<int>::size_type j = 0; j < m_Grid[i].size(); j++)
			m_iterator->m_positionArray.push_back(Position(i, j));
}
bool Maze::isOk(glm::ivec2 p_Cell)
{
	if(p_Cell.x > 0 && p_Cell.x < m_width-1)
		if(p_Cell.y > 0 && p_Cell.y < m_height-1)
			if(!isWall(Position(p_Cell.x,p_Cell.y)))
				if(m_Grid[p_Cell.x][p_Cell.y]!=GENERATOR)
					return true;
				else return false;
			else return false;
		else return false;
	else return false;
}
bool Maze::isVisible(glm::ivec2 startPosition, glm::ivec2 enPosition)
{
	glm::ivec2 ray = enPosition-startPosition;
	glm::ivec2 rayDir;
	rayDir.x = ray.x/m_CellSize;
	rayDir.y = ray.y/m_CellSize;
	for (int i = 0; i < m_CellSize; i++)
	{
		int X = int(startPosition.x+rayDir.x*i);
		int Y = int(startPosition.y+rayDir.y*i);
		if(X < m_width && X >= 0)
		{
			if(Y < m_height && Y >= 0)
			{
				if(m_Grid[X][Y] == WALL)
				{
					return false;
				}
			}
			else return false;
		}
		else return false;

	}
	return true;
}

bool Maze::isOk(glm::vec3 p_Location)
{
	if(p_Location.x > 0 && p_Location.x < m_width*m_CellSize)
		if(p_Location.z > 0 && p_Location.z < m_height*m_CellSize)
			return true;
		else return false;
	else return false;
}
int Maze::getGridCellType(int  p_row, int p_column)
{
	return m_Grid[p_row][p_column];
}
int Maze::getGridCellType(Position p_pos)
{
	return m_Grid[p_pos.x][p_pos.y];
}
glm::vec3 Maze::getNearestCell(glm::vec3 p_position)
{
	glm::ivec2 cellLocation = getGridCell(p_position);
	return glm::vec3(cellLocation.x*m_CellSize, 0.0f, cellLocation.y*m_CellSize);
}
glm::ivec2 Maze::getGridCell(glm::vec3 p_position)
{
	glm::ivec2 cellLocation;

	cellLocation.x = (int)(p_position.x / m_CellSize);
	cellLocation.y = (int)(p_position.z / m_CellSize);

	return cellLocation;
}
glm::vec3 Maze::getCellPosition(glm::vec2 p_cell)
{
	return glm::vec3(p_cell.x*m_CellSize, 0.0f, p_cell.y*m_CellSize);
}
glm::vec3 Maze::getCellPosition(Position p_pos)
{
	return glm::vec3(p_pos.x*m_CellSize, 0.0f, p_pos.y*m_CellSize);
}
MazeIterator *Maze::getIterator()
{
	m_iterator->m_index = 0;
	return m_iterator;
}
PosIterator Maze::getIterator(Position p_pos)
{
	return PosIterator(p_pos);
}
void Maze::toConsole()
{
	for (int i = 0; i < m_width;i++)
	{
		for (int j = m_height-1; j > -1;j--)
		{
			if(m_Grid[i][j] == WALL)
				printf("#");
			else if(m_Grid[i][j] == GENERATOR)
				printf("G");
			else if(m_Grid[i][j] == START)
				printf("S");
			else
				printf(" ");
		}
		printf("\n");
	}
}
void Maze::setWall(Position p_pos)
{
	m_Grid[p_pos.x][p_pos.y] = WALL;
}
void Maze::setVisited(Position p_pos)
{
	m_Grid[p_pos.x][p_pos.y] = CORRIDOR;
}
bool Maze::isOK(Position p_pos)
{
	if(p_pos.x <= 0 || p_pos.x >= m_width-1 || p_pos.y >= m_height-1 || p_pos.y <=0)
	{
		return false;
	}
	return true;
}
bool Maze::isValid(Position p_pos)
{
	//if cell is inside workable area of maze
	if( isOK(p_pos) )
	{
		//if cell has not been visited
		if( isWall(p_pos) )
		{
			//if cell has at least 3 neighbouring walls
			if( neighbourCount(p_pos) > 2 )
			{
				return true;
			}
			return false;
		}
		return false;
	}
	return false;
}
int Maze::neighbourCount(Position p_pos)
{
	//count the number of neighbouring walls
	int count = 0;
	if( isWall( Position( p_pos.x+1,p_pos.y ) ) )
		count++;
	if( isWall( Position( p_pos.x-1,p_pos.y ) ) )
		count++;
	if( isWall( Position( p_pos.x,p_pos.y+1 ) ) )
		count++;
	if( isWall( Position( p_pos.x,p_pos.y-1 ) ) )
		count++;
	return count;
}
bool Maze::isWall(Position p_pos)
{
	if(m_Grid[p_pos.x][p_pos.y] == WALL)
		return true;
	return false;
}
void Maze::createRoom(Position p_pos)
{
	m_Grid[p_pos.x][p_pos.y] = CORRIDOR;
	
	//clears out an area 
	for (int i = 0; i < 3; i++)
	{
		m_Grid[p_pos.x][p_pos.y+i] = CORRIDOR;
		m_Grid[p_pos.x][p_pos.y-i] = CORRIDOR;
		m_Grid[p_pos.x-i][p_pos.y] = CORRIDOR;
		m_Grid[p_pos.x+i][p_pos.y] = CORRIDOR;
		m_Grid[p_pos.x+i][p_pos.y+i] = CORRIDOR;
		m_Grid[p_pos.x-i][p_pos.y-i] = CORRIDOR;
		m_Grid[p_pos.x+i][p_pos.y-i] = CORRIDOR;
		m_Grid[p_pos.x-i][p_pos.y+i] = CORRIDOR;
	}
	m_Grid[p_pos.x-2][p_pos.y+1] = CORRIDOR;
	m_Grid[p_pos.x-2][p_pos.y-1] = CORRIDOR;
	m_Grid[p_pos.x-1][p_pos.y+2] = CORRIDOR;
	m_Grid[p_pos.x-1][p_pos.y-2] = CORRIDOR;
	m_Grid[p_pos.x+2][p_pos.y+1] = CORRIDOR;
	m_Grid[p_pos.x+2][p_pos.y-1] = CORRIDOR;
	m_Grid[p_pos.x+1][p_pos.y+2] = CORRIDOR;
	m_Grid[p_pos.x+1][p_pos.y-2] = CORRIDOR;

}
Maze::~Maze(void)
{
	for (int j = 0; j < m_width;j++)
	{
		m_Grid[j].clear();
	}
	m_Grid.clear();
	delete m_iterator;
}
