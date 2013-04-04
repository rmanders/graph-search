//=============================================================================
// This header defines all of the datastructures and classes used for 
// implementing various types of graphs.
//=============================================================================
#ifndef __RMA_GRAPHTYPES__
#define __RMA_GRAPHTYPES__

#include <list>
#include <set>
#include "Types.h"


class Vertex;
class Edge;

//=============================================================================
// Defines a heavy-duty vertex class;
//=============================================================================
class Vertex
{
private:
	uint32 mId;
	uint32 mColor;
	void*  mData;

public:

	list<Edge&> EdgeList;

	Vertex( uint32 id );
	~Vertex( void );

	uint32 Id( void );
	uint32 Degree( void );
	uint32 Color( void );
	void SetColor( uint32 color );
	set<Vertex&> Neighborhood( void );

	void AddEdge( Edge* e );
	void DelEdge( Edge* e );

}

#endif