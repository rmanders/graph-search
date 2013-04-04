#ifndef __MYGRAPH__
#define __MYGRAPH__

#include <iostream>
#include <fstream>
#include <list>
#include "Types.h"
#include "argraph.h"
#include "argedit.h"
#include "vf2_state.h"
#include "vf2_sub_state.h"
#include "vf2_mono_state.h"
#include "match.h"


using namespace std;

#define MAX_MYGRAPH_ARRAY 400

class MyGraph
{
private:

	uint32	m_grapharr[MAX_MYGRAPH_ARRAY];
	int		m_array_size;
	int		m_edges;
	int		m_vertices;
	Graph*	m_graph;
	ARGEdit m_graphbuilder;
	bool	m_graphisdirty;

	void	UpdateGraph( void );
	void	_AddVertex( void );	
	bool	_AddEdge( int v1, int v2 );

public:

	MyGraph( void );
	MyGraph( int vertices );
	MyGraph( const MyGraph& g );
	MyGraph( int vertices, int edges, int arraysize, int grapharray[] );
	~MyGraph();

	// Properties
	int NumEdges( void );
	int NumVertices( void );
	int GraphArraySize( void );
	Graph* GetGraph( void );

	// Methods
	bool IsIsomorphicTo( MyGraph& g2 );
	bool IsContainedBy( MyGraph& g2 );

	// Manipulation
	void Clear( void );
	void AddVertex( void );
	void AddEdge( int v1, int v2 );
	void AddDisjointPath( int Begin, int End, int PathLen );
	bool IsEdge( int v1, int v2 );

	// Visualization
	void PrintAdjacencyMatrix( void );
	void PrintGraphArray( void );
	void PrintGraphArray( ofstream& s );
	void PrintDotFile( const char* filename );
	int DumpToBitArray( char* bits, int size );

	
	// TEMPOARY USED ONLY FOR COLORING
	uint32* GetGraphArray( void );

	// Operators
	MyGraph& operator=( const MyGraph& g );
};

#endif