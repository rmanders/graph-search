/**
 * An implementation of an undirected graph
 */

#ifndef __GS_UGRAPH__
#define __GS_UGRAPH__

#define VOFFSET(v)	v*(v-1)/2

#include <iostream>
#include <fstream>
#include <list>
#include "types.h"
#include "undirectedgraph.h"

/**
 * The number of of vertices that n bytes can accomodate can be caluclated
 * by the formula: v = floor(0.5 * (1 + sqrt(8 * n) ) )
 */
#define MAX_GRAPH_SIZE  1000

using namespace std;


class Ugraph : public UndirectedGraph {

private:
	ByteType mLowerDiag[MAX_GRAPH_SIZE];
	Uint32Type mLowerDiagLen;
	Uint32Type mEdges;
	Uint32Type mVertices;

	void init( void );
	
public:
	// Constructors
	Ugraph( void );
	Ugraph( Uint32Type nVertices );
	Ugraph( const Ugraph& uGraph );
	Ugraph( Uint32Type nVertices, Uint32Type nEdges, Uint32Type sLowerDiagLen, ByteType aLowerDiag[] );
	~Ugraph();

	// Properties
	virtual Uint32Type NumEdges( void );
	virtual Uint32Type NumVertices( void ); 
	virtual VertexIdType** AdjMatrix( void );
	virtual VertexIdType* LowerDiag( void );
	virtual bool IsEdge( VertexIdType vFirst, VertexIdType vSecond);

	// Manipulation
	virtual void Clear( void );
	virtual void AddVertex( void );
	virtual void AddEdge( VertexIdType vFirst, VertexIdType vSecond );
	virtual void AddDisjointPath( VertexIdType vBegin, VertexIdType vEnd, Uint32Type pLen );

	// Visualization
	virtual void PrintAdjacencyMatrix( void );
	virtual void PrintLowerDiag( void );
	virtual void PrintDotFile( const char* dFileName);

	// Operators
	Ugraph& operator=( const Ugraph& uGraph );
};

#endif
