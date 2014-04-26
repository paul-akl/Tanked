#pragma once
#include <stdlib.h>  
#include <vector>
#include <list>

struct AStarPosition
{
	int x, y;
	bool inUse;
	int F, G, H;
	AStarPosition* m_Parent;
	int indexPosition;
	AStarPosition():x(0),y(0),inUse(false),F(0),G(0),H(0)
	{
		m_Parent = nullptr;
	}
	void setSuccessor(AStarPosition* const p_Parent,int p_Xoffset,int p_Yoffset)
	{
		x = p_Parent->x+p_Xoffset;
		y = p_Parent->y+p_Yoffset;
		m_Parent = p_Parent;
	}
	void estimateCost(const AStarPosition* const p_EndPosition)
	{
		H = manhattanHeuristic(p_EndPosition);
		F = G + H;
	}
	int manhattanHeuristic(const AStarPosition* const p_EndPosition)
	{
		return 10*(abs(p_EndPosition->x-x)+abs(p_EndPosition->y-y));
	}
	bool operator < (const AStarPosition* const lhs)
	{
		return (lhs->F<F);
	}

	bool equals(const AStarPosition* const lhs)
	{
		return lhs->x==x && lhs->y==y;
	}
	void toConsole()
	{
		printf("X:%d,Y:%d\n",x,y);
		if(m_Parent!=nullptr)
			m_Parent->toConsole();
	}
};
class Compare
{
public:
	bool operator() (AStarPosition* a , AStarPosition* b )
	{
		return a->F<b->F;
	}
};
class Equality
{
public:
	bool operator()(AStarPosition* a , AStarPosition* b )
	{
		return a->x==b->x && a->y==b->y;
	}
};
class AStarPool
{
public:
	AStarPool(void);
	void init(int maxPositions);
	AStarPosition* getPosition(AStarPosition* const p_Parent, int p_Xoffset,int p_Yoffset);
	void release(AStarPosition* p_OldPos);
	~AStarPool(void);
private:
	std::vector<AStarPosition*> m_Positions;
	std::list<int> m_PosQueue;
	int MAX_POSITIONS;
	int numUsed;
};

