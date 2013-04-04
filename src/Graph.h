

#ifndef __RMA_GRAPH__
#define __RMA_GRAPH__

#include "Types.h"

#include "argraph.h"
#include "argedit.h"


#define MAX_GRAPH_ARRAY 405

//=============================================================================
// Wrapper class for an UNDIRECTED Graph
//=============================================================================
class RGraph
{
protected:

	uint32 m_graph[MAX_GRAPH_ARRAY];							// Array representing the graph
	int    m_edges;												// Number of edges
	int	   m_vertices;											// Number of vertices
	int    m_array_size;										// size of internal graph array

public:

	RGraph( void );												// Constructor Default
	RGraph( uint32 g[], int array_size, int numvertices);		// Constructor 1
	RGraph( uint32 g[], int numvertices );						// Constructor 2 (Adjacency Matrix)

	int EdgeCount( void );										// Returns the number of edges
	int VertexCount( void );									// returns the number of vertices

	void FillGraphArray( uint32 g[], int size );				// Fills a graph Array;
	void FillARGLoader( ARGEdit* e );							// Fills a graph loader object
	Graph& CreateVFGraph( void );								// Creates a new VF Graph object

	void PrintAdjMatrix( void );								// Prints adjacency Matrix
	void PrintGraphRaw( void );									// Prints internal graph array
	void Print( void );											// Prints Graph with stats

	RGraph& operator=( const RGraph& g );						// Assignment Overload
};


#endif