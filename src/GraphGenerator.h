
#ifndef __RMA_GRAPHGENERATOR__
#define __RMA_GRAPHGENERATOR__

#include "Types.h"
#include "Graph.h"
#include "argraph.h"
#include "argedit.h"

#define MAX_BITS			105

//=============================================================================
// Class used to generate all possible graphs on the set of n vertices.
// We also add options to eliminate certain types of graphs (i.e. non-connected)
//=============================================================================
class GraphGenerator
{
private:

	int m_vertices;							// Number of vertices of the graph
	uint32 m_graph[MAX_BITS];				// Holds current permutation of the graph
	uint64 m_permutation;					// How many permutations so far

	// Gray Code state variables
	int m_size;								// Size of Permute array needed
	uint32 m_ai;							// Parity "bit" for Gray code generation
	int m_j;								// Current index for Gray code generation
	

	void InitGraph( void );					// resets CurPermute to all 0's

public:

	// Constructors
	GraphGenerator( void );					// Default constructor
	GraphGenerator( int iVertexCount );		// Constructor 1

	void Reset( void );						// Re-initialize & set all counters & indexes to zero
	int Step( void );						// Generate next available permutation

	// Printing methods
	void PrintPermutation( void );			// Prints graph as 1d permutation array
	void PrintAdjMatrix( void );			// Prints graph as adjacency matrix

	// Accessor methods
	int GetVertexCount( void );				// Get # of vertices we are premuting over
	int GetCurGraph( int g[], int size );	// Fills an NxN adj matrix rep. the cur graph
	int NumEdges( void );					// Returns the number of edges on the current graph
	void SetVertices( int iVertexCount );	// Set # of Vertices
	uint64 GetCurPermuteCnt( void );		// Current permutation count
	uint64 TotalPermutes( void );			// Total possible permutations

	RGraph& CreateGraph( void );			// Creates and returns a reference to a graph object;
	Graph& CreateVFGraph( void );			// Creates VF Graph
	void FillGraph( RGraph& g );			// Fills an RGraph object with the current permutation
	void FillARGLoader( ARGEdit& e );		// Fill a VF Graph object with the currrnt permutation

};

#endif