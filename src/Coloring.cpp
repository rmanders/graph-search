#include "Coloring.h"

//=============================================================================
// Constructor
//=============================================================================
Coloring::Coloring( unsigned int NumVertices)
{
	this->mColoring = new int [NumVertices];
}
//=============================================================================
// Destructor
//=============================================================================
Coloring::~Coloring( void )
{
	delete [] this->mColoring;
}

//=============================================================================
// Operator Overload on Subscript Operator
//=============================================================================
int& Coloring::operator[] ( const unsigned int iIndex )
{
	return this->mColoring[iIndex];
}