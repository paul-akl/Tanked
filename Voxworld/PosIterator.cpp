#include "PosIterator.h"
#include <time.h>

PosIterator::PosIterator(void)
{
}
PosIterator::PosIterator(Position pos)
{
	//create a list of all possible moves from the parameter position
	std::vector<Position> sequentialMoves;
	sequentialMoves.push_back( Position( pos.x+1,pos.y ) );
	sequentialMoves.push_back( Position( pos.x-1,pos.y ) );
	sequentialMoves.push_back( Position( pos.x,pos.y+1 ) );
	sequentialMoves.push_back( Position( pos.x,pos.y-1 ) );
	//then shuffle and store them
	for( int i = 0 ; i < 4 ; i++ )
	{
		//generate random number between 0 and current index. this will eventually converge on zero

		int x = ( ( rand() % sequentialMoves.size() ) );
		//move the selected position to stored moves and remove from temporary list
		m_Moves.push_back( sequentialMoves[ x ] );
		sequentialMoves.erase( sequentialMoves.begin()+x );
	}
	index = -1;
}
bool PosIterator::hasNext()
{
	if ( index < (int)m_Moves.size() -1 )
		return true;
	return false;
}
Position PosIterator::getNext()
{
	index++;
	return m_Moves[index];
}
PosIterator::~PosIterator(void)
{
	m_Moves.clear();
}
