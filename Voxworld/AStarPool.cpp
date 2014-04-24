#include "AStarPool.h"


AStarPool::AStarPool(void)
{
	numUsed = 0;
}
void AStarPool::init(int maxPositions)
{
	MAX_POSITIONS = maxPositions;
	for (int i = 0; i < MAX_POSITIONS;i++)
	{
		m_Positions.push_back(new AStarPosition());
		m_Positions.back()->indexPosition = i;
		m_PosQueue.push_back(i);
	}
}
AStarPosition* AStarPool::getPosition(AStarPosition* const p_Parent,int p_Xoffset,int p_Yoffset)
{
	//uses queued up indices to access unused elements in constant time
	AStarPosition* pos = nullptr;
	if(!m_PosQueue.empty())
	{
		int posIndex = m_PosQueue.front();
		pos = m_Positions[posIndex];
		m_PosQueue.pop_front();
		while(pos->inUse && !m_PosQueue.empty())
		{
			posIndex = m_PosQueue.front();
			m_PosQueue.pop_front();
			pos = m_Positions[posIndex];
			//numUsed++;
		}
		pos->x=0;
		pos->y=0;
		pos->F=-1;
		pos->G=-1;
		pos->H=-1;
		pos->m_Parent = nullptr;
		if(p_Parent == nullptr)
		{
			pos->x = p_Xoffset;
			pos->y = p_Yoffset;

		}
		else
		{
			pos->m_Parent = p_Parent;
			pos->x = p_Parent->x+p_Xoffset;
			pos->y = p_Parent->y+p_Yoffset;
			pos->setSuccessor(p_Parent,p_Xoffset,p_Yoffset);
		}
		pos->inUse = true;
		numUsed++;
		return pos;
	}
	return nullptr;
}
void AStarPool::release(AStarPosition* p_OldPos)
{
	//turn off node and add index to queue for reuse
	if(p_OldPos!=nullptr)
	{
		m_Positions[p_OldPos->indexPosition]->inUse = false;
		p_OldPos->m_Parent = nullptr;
		int index = p_OldPos->indexPosition;
		m_PosQueue.push_back(index);
		//numUsed--;
	}
	else
		printf("null pointer reference");
}
AStarPool::~AStarPool(void)
{
	for (size_t i = 0; i < MAX_POSITIONS;i++)
	{
		delete m_Positions.back();
		m_Positions.pop_back();
	}
	m_PosQueue.clear();
}
