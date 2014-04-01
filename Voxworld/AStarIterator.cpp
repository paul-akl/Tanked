#include "AStarIterator.h"
#include <algorithm>

AStarIterator::AStarIterator(void)
{
	count = -1;
	m_PossibleMoves.clear();
}
bool greater(AIPosition& a,AIPosition& b)
{
	return (a.F > b.F);
}
AStarIterator::AStarIterator(AIPosition p_Position)
{
	count = -1;
	//m_PossibleMoves.resize(8);
	//create 8 possible moves from position
	//m_PossibleMoves[0] = new AIPosition(p_Position->x+1,p_Position->y);
	//m_PossibleMoves[1] = new AIPosition(p_Position->x+1,p_Position->y-1);
	//m_PossibleMoves[2] = new AIPosition(p_Position->x,p_Position->y-1);
	//m_PossibleMoves[3] = new AIPosition(p_Position->x-1,p_Position->y-1);
	//m_PossibleMoves[4] = new AIPosition(p_Position->x-1,p_Position->y);
	//m_PossibleMoves[5] = new AIPosition(p_Position->x-1,p_Position->y+1);
	//m_PossibleMoves[6] = new AIPosition(p_Position->x,p_Position->y+1);
	//m_PossibleMoves[7] = new AIPosition(p_Position->x+1,p_Position->y+1);
	m_PossibleMoves.resize(4);
	m_PossibleMoves[0] = AIPosition(p_Position.x+1,p_Position.y);
	m_PossibleMoves[1] = AIPosition(p_Position.x,p_Position.y-1);
	m_PossibleMoves[2] = AIPosition(p_Position.x-1,p_Position.y);
	m_PossibleMoves[3] = AIPosition(p_Position.x,p_Position.y+1);
	//now sort in descending order, of F cost
	for (size_t i = 0; i < m_PossibleMoves.size();i++)
	{
		m_PossibleMoves[i].m_Parent = &p_Position;
	}

}

bool AStarIterator::hasNext()
{
	return count < 3;
	//return count < 7;
}
AIPosition AStarIterator::getNext()
{
	count++;
	return m_PossibleMoves[count];
}

AStarIterator::~AStarIterator(void)
{
	count = -1;
	m_PossibleMoves.clear();
}


