#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "MazeNode.h"

MazeNode::MazeNode(void)
{
	m_iteratorOutOfBounds = false;
	m_gridWidth = 0.0f;
	m_gridHeight = 0.0f;
	m_cellWidth = 0.0f;
	m_cellHeight = 0.0f;
	m_widthIterator = 0;
	m_heightIterator = 0;
	m_numColumns = 0;
	m_numRows = 0;
}
MazeNode::~MazeNode(void)
{
	for(int i=0; i<m_numColumns; i++)
		delete[] m_grid[i];
	delete[] m_grid;
}

bool MazeNode::isInside(SceneNode *p_sceneNode)
{
	glm::vec3 location = p_sceneNode->getLocation();
	return	(location.x >= m_startPosition.x && location.x <= m_startPosition.x + m_gridWidth) &&
			(location.y >= m_startPosition.y && location.y <= m_startPosition.y + m_gridHeight);
}
bool MazeNode::isOk(glm::vec2 p_cell)
{
	int width = (int)p_cell.x, height = (int)p_cell.y;

	if(	m_grid[width][height] == 0 &&
		width < m_numColumns-1 && width > 1 &&
		height < m_numRows-1 && height > 1)
	{
		if(	m_grid[width-1][height+1] == 0 &&
			m_grid[width-1][height	] == 0 &&
			m_grid[width-1][height-1] == 0 &&
			m_grid[width][height+1	] == 0 &&
			m_grid[width][height-1	] == 0 &&
			m_grid[width+1][height+1] == 0 &&
			m_grid[width+1][height	] == 0 &&
			m_grid[width+1][height-1] == 0 )
		{
			return true;
		}
	}
	return false;
}
bool MazeNode::isOk(glm::vec3 p_position)
{
	glm::vec2 cellLocation = getGridCell(p_position);
	return isOk(cellLocation);
}
glm::vec3 MazeNode::getNearestCell(glm::vec3 p_position)
{
	glm::vec2 cellLocation = getGridCell(p_position);
	return (glm::vec3(cellLocation.x*m_cellWidth, 0.0f, cellLocation.y*m_cellHeight) + m_startPosition);
}
bool MazeNode::isPassable(glm::vec3 p_position)
{
	glm::vec2 cellLocation = getGridCell(p_position);
	return m_grid[(int)cellLocation.x][(int)cellLocation.y] == 0;
}
void MazeNode::generateMaze(std::string p_filename)
{
	std::string tempWord;
	std::ifstream mazeFile;
	std::vector<std::string> individualWords;
	mazeFile.open(p_filename, std::ios::in);
	
	if(mazeFile.fail())
	{
		mazeFile.close();
		std::cout << "Maze file has failed to load" << std::endl;
	}
	
	while(!mazeFile.eof())
	{
		mazeFile >> tempWord;
		individualWords.push_back(tempWord);
	}
	mazeFile.close();
	
	for(std::vector<std::string>::size_type i=0; i < individualWords.size(); i++)
	{
		if(individualWords[i] == "cellHight")
		{
			m_cellWidth = std::atof(individualWords[i+1].c_str());
			i++;
		}
		if(individualWords[i] == "cellWidth")
		{
			m_cellHeight = std::atof(individualWords[i+1].c_str());
			i++;
		}
		if(individualWords[i] == "numRows")
		{
			m_numRows = std::atoi(individualWords[i+1].c_str());
			i++;
		}
		if(individualWords[i] == "numColumn")
		{
			m_numColumns = std::atoi(individualWords[i+1].c_str());
			i++;
		}
		if(individualWords[i] == "startPos")
		{
			m_startPosition = glm::vec3(std::atof(individualWords[i+1].c_str()), std::atof(individualWords[i+2].c_str()), std::atof(individualWords[i+3].c_str()));
			i+=3;
		}
		if(individualWords[i] == "maze")
		{
			m_grid = new int *[m_numColumns];
			for(int i=0; i<m_numColumns; i++) 
				m_grid[i] = new int[m_numRows];

			for(int h=0; h<m_numRows; h++)
			{
				for(int w=0; w<m_numColumns; w++)
				{
					i++;
					m_grid[w][h] = std::atoi(individualWords[i].c_str());
				}
			}
			
			for(int h=0; h<m_numRows; h++)
			{
				for(int w=0; w<m_numColumns; w++)
				{
					std::cout << m_grid[w][h] << " ";
				}
				std::cout << std::endl;
			}

		}
	}
	m_gridWidth = m_numColumns*m_cellWidth;
	m_gridHeight = m_numRows*m_cellHeight;
}
glm::vec2 MazeNode::getGridCell(glm::vec3 p_position)
{
	glm::vec3 position = p_position - m_startPosition;
	glm::vec2 cellLocation;

	cellLocation.x = ceil(position.x / m_cellWidth);
	cellLocation.y = ceil(position.z / m_cellHeight);

	if(cellLocation.x <= m_numColumns)
		cellLocation.x = fabs(position.x - cellLocation.x*m_cellWidth) > fabs((position.x - cellLocation.x-1)*m_cellWidth) ? cellLocation.x-1 : cellLocation.x;
	else
		cellLocation.x = m_numColumns;

	if(cellLocation.y <= m_numRows)
		cellLocation.y = fabs(position.y - cellLocation.y*m_cellHeight) > fabs((position.y - cellLocation.y-1)*m_cellHeight) ? cellLocation.y-1 : cellLocation.y;
	else
		cellLocation.y = m_numRows;

	return cellLocation;
}
glm::vec3 MazeNode::getCellPosition(glm::vec2 p_cell)
{
	return (glm::vec3(p_cell.x*m_cellWidth, 0.0f, p_cell.y*m_cellHeight) + m_startPosition);
}
glm::vec2 MazeNode::nextPosition()
{
	glm::vec2 returnVec((float)m_widthIterator, (float)m_heightIterator);
	m_widthIterator++;
	if(m_widthIterator >= m_numColumns)
	{
		m_widthIterator = 0;
		m_heightIterator++;
		if(m_heightIterator >= m_numRows)
		{
			m_heightIterator = 0;
			m_widthIterator = 0;
			m_iteratorOutOfBounds = true;
		}
	}
	return returnVec;
}
bool MazeNode::hasNextPosition()
{
	if(m_iteratorOutOfBounds)
	{
		m_iteratorOutOfBounds = false;
		return false;
	}
	else
		return true;
}
int MazeNode::getGridCellType(int p_row, int p_column)
{
	return m_grid[p_row][p_column];
}