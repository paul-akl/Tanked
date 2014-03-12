#pragma once
class Position
{
public:
	Position(void);
	Position(int iX, int iY);
	~Position(void);
private:
	int x, y;
	friend class Backtracker;
	friend class Maze;
	friend class PosIterator;
};

