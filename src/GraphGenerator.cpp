
#include <iostream>
#include "GraphGenerator.h"

using namespace std;

//=============================================================================
// InitGraph
// Sets the current graph permutation array to all 0's
//=============================================================================
void GraphGenerator::InitGraph( void )
{
	for( int i = 0; i < MAX_BITS; i++ )
	{
		this->m_graph[i] = 0; 
	}
}

//=============================================================================
// Constructor (Default)
//=============================================================================
GraphGenerator::GraphGenerator( void )
{
	this->m_vertices = 1;
	this->m_permutation = 1;
	this->m_size = 1;
	this->m_ai = 0;
	this->m_j = 0;
	this->InitGraph();
}

//=============================================================================
// Constructor 1
//=============================================================================
GraphGenerator::GraphGenerator(int iVertexCount )
{
	this->m_vertices = iVertexCount;
	this->m_permutation = 1;
	this->m_size = iVertexCount*(iVertexCount - 1)/2;
	this->m_ai = 0;
	this->m_j = 0;
	this->InitGraph();
}

//=============================================================================
// Step
// Generates the next graph/adjacency matrix permutation using gray codes.
// The algorithm(G) is taken straight out of Knuth's Art of Computer Programming
// Vol 4, Fasicle 2. Returns 1 if successfull, 0 if reached end of possible 
// permutations.
//=============================================================================
int GraphGenerator::Step( void )
{
	if( this->m_j >= this->m_size )
	{
		return 0;
	}
	else
	{
		this->m_ai = this->m_ai ^ 1;	// Complement parity "bit"

		if( this->m_ai == 1 )
		{
			this->m_j = 0;
		}
		else
		{
			int i = 0;
			for( i = 0; i < this->m_size; i ++ )
			{
				if( this->m_graph[i] == 1 )
					break;
			}
			this->m_j = i + 1;
		}

		if( this->m_j < this->m_size )
		{
			this->m_graph[this->m_j] = this->m_graph[this->m_j] ^ 1;
			this->m_permutation++;
			return 1;
		}
		else
		{
			return 0;
		}
	}	
}

//=============================================================================
// GetCurGraph
// Fills an array of size: (size x size) with the adjacency matrix of the 
// current graph permutation. This allows operations to be performed on the
// graph in a standard format. Returns 1 for success, 0 for failure.
//=============================================================================
int GraphGenerator::GetCurGraph( int g[], int size )
{
	if( size != this->m_vertices * this->m_vertices )
	{
		return 0;
	}

	for( int j = 0; j < this->m_vertices; j++ )
	{		
		for( int i = 0; i < this->m_vertices; i++ )
		{
			if( i == j )		//On Diagonal
			{
				g[ i + j*this->m_vertices] = 0;
			}
			else if( i > j )	// Above Diagonal
			{
				cout << this->m_graph[ i*(i-1)/2 + j ] << " ";
			}
			else				// Below Diagonal
			{
				cout << this->m_graph[ j*(j-1)/2 + i ] << " ";
			}			
		}
		cout << endl;
	}

	return 1;
}
//=============================================================================
// PrintPermutation
//=============================================================================
void GraphGenerator::PrintPermutation( void )
{
	for( int i = 0; i < this->m_size && i < MAX_BITS; i++)
	{
		cout << this->m_graph[i] << " ";
	}
	cout << endl;
}

//=============================================================================
// PrintAdjMatrix
// O(n^2)
//=============================================================================
void GraphGenerator::PrintAdjMatrix( void )
{
	for( int j = 0; j < this->m_vertices; j++ )
	{		
		for( int i = 0; i < this->m_vertices; i++ )
		{
			if( i == j )		//On Diagonal
			{
				cout << "0 ";
			}
			else if( i > j )	// Above Diagonal
			{
				cout << this->m_graph[ i*(i-1)/2 + j ] << " ";
			}
			else				// Below Diagonal
			{
				cout << this->m_graph[ j*(j-1)/2 + i ] << " ";
			}			
		}
		cout << endl;
	}
	cout << endl;
}

//=============================================================================
// NumEdges
// Returns the number of edges in the current graph
//=============================================================================
int GraphGenerator::NumEdges( void )
{
	int edges = 0;
	for( int i = 0; i < this->m_size; i++ )
	{
		if( this->m_graph[i] == 1 )
		{
			edges++;
		}
	}
	return edges;
}

//=============================================================================
// CreateGraph
// Creates a new object of type Graph, and returns it's reference
//=============================================================================
RGraph& GraphGenerator::CreateGraph( void )
{
	RGraph* g = NULL;


	g = new RGraph(this->m_graph, this->m_size, this->m_vertices );
	if( NULL == g )
	{
		cout << "FATAL(GraphGenerator::CreateGraph): Unable to create new Graph Object" << endl;
		exit( 1 );
	}
	
	return (*g);
}

//=============================================================================
// CreateVFGraph
//=============================================================================
Graph& GraphGenerator::CreateVFGraph( void )
{
	ARGEdit e;
	
	// Insert Vertices
	for( int i = 0; i < this->m_vertices; i++ )
	{
		e.InsertNode( NULL );
	}

	// Insert Edges
	for( int j = 0; j < this->m_vertices; j++ )
	{		
		for( int i = 0; i < this->m_vertices; i++ )
		{
			if( ( i < j ) && ( this->m_graph[ j*(j-1)/2 + i ] == 1 ) )
			{
				e.InsertEdge( i, j, NULL );
				e.InsertEdge( j, i, NULL );
			}			
		}
	}

	Graph* g = NULL;
	g = new Graph( &e );

	if( NULL == g )
	{
		cout << "FATAL(RGraph::CreateVFGraph) Failed to allocate memory." << endl;
		exit( 1 );
	}

	return (*g);
}

//=============================================================================
// FillGraph
//=============================================================================
void GraphGenerator::FillGraph( RGraph& g )
{
	RGraph gn(this->m_graph, this->m_size, this->m_vertices );
	g = gn;	

}

//=============================================================================
// FillVFGraph
//=============================================================================
void GraphGenerator::FillARGLoader( ARGEdit& e )
{
	ARGEdit ne;
	
	// Insert Vertices
	for( int i = 0; i < this->m_vertices; i++ )
	{
		ne.InsertNode( NULL );
	}

	// Insert Edges
	for( int j = 0; j < this->m_vertices; j++ )
	{		
		for( int i = 0; i < this->m_vertices; i++ )
		{
			if( ( i < j ) && ( this->m_graph[ j*(j-1)/2 + i ] == 1 ) )
			{
				ne.InsertEdge( i, j, NULL );
				ne.InsertEdge( j, i, NULL );
			}			
		}
	}
	e = ne;
}

//=============================================================================
// GetCurPermuteCount
//=============================================================================
uint64 GraphGenerator::GetCurPermuteCnt( void )
{
	return this->m_permutation;
}