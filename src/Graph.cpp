

#include <iostream>
#include "Graph.h"

using namespace std;

//=============================================================================
// Constructor Default
//=============================================================================
RGraph::RGraph( void )
{
	this->m_array_size = 0;
	this->m_edges = 0;
	this->m_vertices = 0;
}

//=============================================================================
// Constructor 1
//=============================================================================
RGraph::RGraph(uint32 g[], int array_size, int numvertices)
{
	int i = 0;
	int edgecount = 0;

	if( array_size > MAX_GRAPH_ARRAY)
	{
		cout << "FATAL(Graph::Constructor1): Attempted to create graph of Illegal size: " << array_size << 
			". Exiting Program." << endl;
		exit(1);
	}

	// Copy graph data
	for( i = 0; i < array_size; i++ )
	{
		this->m_graph[i] = g[i];
		if( g[i] == 1)
		{
			edgecount++;
		}
	}

	// Init remaining array space
	for( i = i; i < MAX_GRAPH_ARRAY; i++ )
		this->m_graph[i] = 2;

	// Set local member vars with graph statistics
	this->m_vertices = numvertices;
	this->m_edges = edgecount;
	this->m_array_size = array_size;
}

//=============================================================================
// Constructor 2 (from adjacency matrix)
// Input is assumed to be a SQUARE matrix of size: numvertices
//=============================================================================
RGraph::RGraph(uint32 g[], int numvertices)
{
	int i,j;
	int k = 0;
	int edgecount = 0;
	int array_size = numvertices*(numvertices-1)/2;

	// Check for valid sized graph
	if( array_size > MAX_GRAPH_ARRAY)
	{
		cout << "FATAL(Graph::Constructor2): Attempted to create graph of Illegal size: " << array_size << 
			". Exiting Program." << endl;
		exit(1);
	}

	this->m_vertices = numvertices;

	// Copy graph data (this can be improved by setting i to numvertices-1 after i touches
	// the diagonal. But since this is in the constructor, who cares? :)
	for( j = 0; j < this->m_vertices; j++ )
	{
		for( i = 0; i < this->m_vertices; i++ )
		{
			if( i < j )
			{
				this->m_graph[k] = g[j*numvertices + i];
				if( this->m_graph[k] == 1 )
				{
					edgecount++;
				}
				k++;
			}
		}
	}
	this->m_edges = edgecount;
	this->m_array_size = array_size;
}

//=============================================================================
// EdgeCount
// Returns the number of edges
//=============================================================================
int RGraph::EdgeCount( void )
{
	return this->m_edges;
}

//=============================================================================
// VertexCount
// Returns the number of vertices
//=============================================================================
int RGraph::VertexCount( void )
{
	return this->m_vertices;
}

//=============================================================================
// GetAdjMatrix
// Fills a graph array
//=============================================================================
void RGraph::FillGraphArray( uint32 g[], int size )
{
	if( size > MAX_GRAPH_ARRAY || size < this->m_array_size )
	{
		cout << "FATAL(Graph::GetGraphArray): Attempted to fill graph array of Illegal size: " << size << 
			". Exiting Program." << endl;
		exit(1);		
	}

	for( int i = 0; i < this->m_array_size; i++ )
	{
		g[i] = this->m_graph[i];
	}
}

//=============================================================================
// CreateVFGraph
//=============================================================================
Graph& RGraph::CreateVFGraph( void )
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
// PrintAdjMatrix
// Prints the adjacency matrix for a graph.
//=============================================================================
void RGraph::PrintAdjMatrix( void )
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
// PrintGraphRaw
// Prints the "raw" internal graph array format
//=============================================================================
void RGraph::PrintGraphRaw( void )
{
	for( int i = 0; i < this->m_array_size; i++ )
	{
		cout << this->m_graph[i] << " ";
	}
	cout << endl;
}

//=============================================================================
// Print
// Prints the graph with adjacency matrix plus num edges and vertices.
//=============================================================================
void RGraph::Print( void )
{
	cout << "----------------------------" << endl;
	cout << "Vertices: " << this->m_vertices << ", Edges: " << this->m_edges << endl;
	cout << "----------------------------" << endl;
	this->PrintAdjMatrix();
}

//=============================================================================
// FillARGLoader
//=============================================================================
void RGraph::FillARGLoader( ARGEdit* e )
{
	e->Clear();
		
	// Insert Vertices
	for( int i = 0; i < this->m_vertices; i++ )
	{
		e->InsertNode( NULL );
	}

	// Insert Edges
	for( int j = 0; j < this->m_vertices; j++ )
	{		
		for( int i = 0; i < this->m_vertices; i++ )
		{
			if( ( i < j ) && ( this->m_graph[ j*(j-1)/2 + i ] == 1 ) )
			{
				e->InsertEdge( i, j, NULL );
				e->InsertEdge( j, i, NULL );
			}			
		}
	}
}

//=============================================================================
// OVERLOAD operator= 
//=============================================================================
RGraph& RGraph::operator=( const RGraph& g )
{
	if( &g == this )
	{
		return *this;
	}

	this->m_edges = g.m_edges;
	this->m_vertices = g.m_vertices;
	this->m_array_size = g.m_array_size;

	for( int i = 0; i < g.m_array_size; i++ )
		this->m_graph[i] = g.m_graph[i];

	return *this;
}
