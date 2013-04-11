/**
 * A basic interface class for undirected graphs
 */

#ifndef __GS_UNDIRECTED_GRAPH__
#define __GS_UNDIRECTED_GRAPH__

#include <iostream>
#include <fstream>
#include <list>
#include "types.h"

using namespace std;


class UndirectedGraph {

public:

	// Properties
	virtual Uint32Type NumEdges( void ) = 0;
	virtual Uint32Type NumVertices( void ) = 0; 
	virtual VertexIdType** AdjMatrix( void ) = 0; /// Returns a new n^2 sized adjacency matrix
	virtual VertexIdType* LowerDiag( void ) = 0; /// Returns a new n(n-1)/2 sized lower diag adj matrix
	virtual bool IsEdge( VertexIdType vFirst, VertexIdType vSecond) = 0;

	// Manipulation
	virtual void Clear( void ) = 0;
	virtual void AddVertex( void ) = 0;
	virtual void AddEdge( VertexIdType vFirst, VertexIdType vSecond ) = 0;
	virtual void AddDisjointPath( VertexIdType vBegin, VertexIdType vEnd, Uint32Type pLen ) = 0;

	// Visualization
	virtual void PrintAdjacencyMatrix( void ) = 0;
	virtual void PrintLowerDiag( void ) = 0;
	virtual void PrintDotFile( const char* dFileName) = 0; /// Saves graph to graphviz dot file
};

#endif
