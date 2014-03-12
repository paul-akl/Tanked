#pragma once
#include "Position.h"
#include <vector>
class PosIterator
{
public:
	PosIterator(void);
	PosIterator(Position pos);
	bool hasNext();
	Position getNext();
	~PosIterator(void);
private:
	std::vector<Position> m_Moves;
	int index;
};

