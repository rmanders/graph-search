//=============================================================================
// Coloring.h
// This class is used to represent the coloring of a graph. It is a mapping from 
// the vertex of a graph represented by a positive integer to a color 
// (also repesented by a positive integer). Right now, this class has no 
// awareness if the coloring is valid or proper. It is only used to represent
// "a" coloring.
//=============================================================================

#ifndef __RMA_COLORING__
#define __RMA_COLORING__

typedef enum
{
	RED,
	GREEN,
	BLUE,
	YELLOW,
	PURPLE
} color_t;

class Coloring
{
private:
	int  mVertices;
	int* mColoring;

public:
	Coloring( unsigned int NumVertices );
	~Coloring( void );

	int& operator[] ( const unsigned int iIndex );
};

#endif