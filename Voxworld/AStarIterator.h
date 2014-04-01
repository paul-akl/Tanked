#pragma once
#include <glm\glm.hpp>
#include <vector>
struct AIPosition
{
	//x,y: position in grid
	//F = G+H = heuristic cost
	//G = manhattan distance from start to this
	//H = est Distance (Manhattan) to start
	AIPosition* m_Parent;
	int x,y,F,G,H;
	AIPosition(const AIPosition& rhs)
	{
		m_Parent = rhs.m_Parent;
		x=rhs.x;
		y=rhs.y;
		G=rhs.G;
		H=rhs.H;
		F=rhs.F;
	}
	AIPosition(void):x(0),y(0),F(0),G(0),H(0)
	{
		m_Parent = nullptr;
	}
	AIPosition(int X, int Y):F(0),G(0),H(0)
	{
		m_Parent = nullptr;
		x=X;
		y=Y;
	}
	AIPosition& operator=(const AIPosition& rhs) 
	{
		m_Parent = rhs.m_Parent;
		x=rhs.x;
		y=rhs.y;
		G=rhs.G;
		H=rhs.H;
		F=rhs.F;
		return *this;
	}
	bool operator==(const AIPosition& rhs) const
	{
		return (rhs.x==x) && (rhs.y == y);
	}
	bool operator < (const AIPosition& lhs) const
	{
		return (F < lhs.F);
	}
};
class AStarIterator
{
public:
	AStarIterator(void);
	AStarIterator(AIPosition p_Position);
	bool hasNext();
	AIPosition getNext();
	~AStarIterator(void);
private:
	std::vector<AIPosition> m_PossibleMoves;
	int count;
};

