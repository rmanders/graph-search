
#include <fstream>
#include <exception>

#include "MyGraph.h"

//=============================================================================
// Constructor 1
//=============================================================================
MyGraph::MyGraph( void ) : m_graphbuilder()
{
	for( int i = 0; i < MAX_MYGRAPH_ARRAY; i++ )
	{
		m_grapharr[i] = 0;
	}

	m_graphisdirty = true;
	m_array_size = 0;
	m_edges = 0;
	m_vertices = 0;
	m_graph = NULL;
}

//=============================================================================
// Constructor 2
//=============================================================================
MyGraph::MyGraph(int vertices) : m_graphbuilder()
{
	for( int i = 0; i < MAX_MYGRAPH_ARRAY; i++ )
	{
		m_grapharr[i] = 0;
	}

	m_graphisdirty = true;
	m_array_size = 0;
	m_edges = 0;
	m_vertices = 0;
	m_graph = NULL;

	// Add default vertices
	m_vertices = vertices;

	// Calculate new graph array size
	int NewArraySize = (m_vertices * (m_vertices - 1)) / 2;

	if( NewArraySize > MAX_MYGRAPH_ARRAY )
	{
		exception e("Constructor: Can't add new vertices to graph. Exceeds maximum graph array size!");
		throw e;
	}
	m_array_size = NewArraySize;

	// Sync with graphbuilder
	//for( int i = 0; i < vertices; i++ )
	//{
	//	m_graphbuilder.InsertNode( NULL );
	//}
	UpdateGraph();
}

//=============================================================================
// Constructor 3
//=============================================================================
MyGraph::MyGraph( const MyGraph& g ) : m_graphbuilder()
{
	for( int i = 0; i < MAX_MYGRAPH_ARRAY; i++ )
	{
		m_grapharr[i] = g.m_grapharr[i];
	}

	m_graphisdirty = g.m_graphisdirty;
	m_array_size = g.m_array_size;
	m_edges = g.m_edges;
	m_vertices = g.m_vertices;

	// Create the internal graph object
	m_graph = NULL;
	UpdateGraph();

}

//=============================================================================
// Constructor 4
//=============================================================================
MyGraph::MyGraph( int vertices, int edges, int arraysize, int grapharray[] ) : m_graphbuilder()
{
	if( arraysize > MAX_MYGRAPH_ARRAY )
	{
		exception e("Constructor 4; Array size parameter too large!");
		throw e;
	}

	for( int i = 0; i < arraysize; i++ )
	{
		if( !(grapharray[i] == 0 || grapharray[i] == 1) )
		{
			exception e("Constructor 4: Invalid contents of grapharray parameter! (only 1s or 0s allowed)");
			throw e;
		}
		m_grapharr[i] = grapharray[i];
		m_array_size = arraysize;
		m_vertices = vertices;
		m_edges = edges;
		m_graph = NULL;
		m_graphisdirty = true;

		UpdateGraph();
	}
}
//=============================================================================
// Destructor
//=============================================================================
MyGraph::~MyGraph( void )
{	
	m_graphbuilder.Clear();
	if( NULL != m_graph )
	{
		delete m_graph;
	}
}

//=============================================================================
// UpdateGraph
//=============================================================================
void MyGraph::UpdateGraph( void )
{
	// Rebuild the graphbuilder object
	m_graphbuilder.Clear();

	for( int i = 0; i < m_vertices; i++ )
	{
		m_graphbuilder.InsertNode( NULL );
	}

    for (int j = 0; j < m_vertices; j++)
    {
        for (int i = 0; i < m_vertices; i++)
        {
            //int test = j * (j - 1) / 2 + i;
            if ((i < j) && (m_grapharr[j * (j - 1) / 2 + i] == 1))
            {
				m_graphbuilder.InsertEdge( i, j, NULL );
				m_graphbuilder.InsertEdge( j, i, NULL );
            }
        }
    }

	// Recreated the graph object
	if( NULL != m_graph )
	{
		delete m_graph;
	}
	m_graph = NULL;

	m_graph = new Graph( &m_graphbuilder );
	if( NULL == m_graph )
	{
		exception e("Out of memory!!");
		throw e;
	}
	m_graphisdirty = false;
}

//=============================================================================
// _AddVertex
//=============================================================================
void MyGraph::_AddVertex( void )
{
	m_vertices++;

	// Calculate new graph array size
	int NewArraySize = (m_vertices * (m_vertices - 1)) / 2;

	if( NewArraySize > MAX_MYGRAPH_ARRAY )
	{
		exception e("Can't add new vertex to graph. Exceeds maximum graph array size!");
		throw e;
	}

	// Re-init new area
	for( int i = m_array_size; i < NewArraySize; i++ )
	{
		m_grapharr[i] = 0;
		m_array_size = NewArraySize;
	}
}

//=============================================================================
// _AddEdge
//=============================================================================
bool MyGraph::_AddEdge( int v1, int v2 )
{
	if( v1 == v2 )
	{
		exception e("Edge can't begin and end in the same vertex!");
		throw e;
	}

	if( v1 >= m_vertices || v2 >= m_vertices )
	{
		exception e("Unable to add edge to non-existant vertex!");
		throw e;
	}

	int i = 0;

	if( v1 < v2 )
	{
		i = (v2 * (v2 - 1) / 2) + v1;
	}
	else
	{
		i = (v1 * (v1 - 1) / 2) + v2;
	}

	// Check Edge doesn't already exist
	if( m_grapharr[i] == 0 )
	{
		m_grapharr[i] = 1;
		m_edges++;
		m_graphisdirty = true;
		return true;
	}
	else
	{
		return false;
	}
}

//=============================================================================
// NumEdges
//=============================================================================
int MyGraph::NumEdges( void )
{
	return m_edges;
}

//=============================================================================
// NumVertices
//=============================================================================
int MyGraph::NumVertices( void )
{
	return m_vertices;
}

//=============================================================================
// GraphArraySize
//=============================================================================
int MyGraph::GraphArraySize( void )
{
	return m_array_size;
}

//=============================================================================
// GetGraph
//=============================================================================
Graph* MyGraph::GetGraph( void )
{
	return m_graph;
}

//=============================================================================
// IsIsomorphicTo
//=============================================================================
bool MyGraph::IsIsomorphicTo( MyGraph &g2 )
{
	node_id n1[100],n2[100];
	int n;

	VF2State s0( this->m_graph, g2.GetGraph(), true );
	return match( &s0, &n, n1, n2 );
}

//=============================================================================
// Contains
//=============================================================================
bool MyGraph::IsContainedBy( MyGraph &g2 )
{
	node_id n1[100],n2[100];
	int n;

	VF2SubState ss0( this->m_graph, g2.GetGraph(), true );

	if( match( &ss0, &n, n1, n2 ) )
	{
		return true;
	}

	// If no match, check for "monomorphism"
	VF2MonoState ms0( this->m_graph, g2.GetGraph(), true );
	return match( &ms0, &n, n1, n2 );
	
}

//=============================================================================
// Clear
//=============================================================================
void MyGraph::Clear( void )
{
	for( int i = 0; i < MAX_MYGRAPH_ARRAY; i++ )
	{
		m_grapharr[i] = 0;
	}

	m_array_size = 0;
	m_vertices = 0;
	m_edges = 0;
	m_graphisdirty = false;
	m_graphbuilder.Clear();

	if( NULL != m_graph )
	{
		delete m_graph;
	}
	m_graph = NULL;
}

//=============================================================================
// AddVertex
//=============================================================================
void MyGraph::AddVertex( void )
{
	_AddVertex();
	UpdateGraph();
}

//=============================================================================
// AddEdge
//=============================================================================
void MyGraph::AddEdge( int v1, int v2 )
{
	if( _AddEdge( v1, v2 ) )
	{
		UpdateGraph();
	}
}

//=============================================================================
// AddDisjointPath
//=============================================================================
void MyGraph::AddDisjointPath( int Begin, int End, int PathLen )
{
	// Check for invalid input
	if( Begin >= m_vertices || End >= m_vertices || Begin < 0 || End < 0 || PathLen < 0 )
	{
		exception e("Invalid input for adding a disjoint path!");
		throw e;
	}

	if( PathLen == 1 )
	{
		_AddEdge( Begin, End );
	}
	else
	{	
		for( int i = 0; i < PathLen; i++ )
		{
			if( i == 0 )
			{
				_AddVertex();
				_AddEdge( Begin,m_vertices-1 );
			}
			else if( i+1 == PathLen )
			{
				_AddEdge( m_vertices-1, End );
			}
			else
			{
				_AddVertex();
				_AddEdge( m_vertices-2, m_vertices-1 );
			}
		}
	}
	UpdateGraph();
}

//=============================================================================
// IsEdge
//=============================================================================
bool MyGraph::IsEdge( int v1, int v2 )
{
    if (v1 >= m_vertices || v2 >= m_vertices || v1 < 0 || v2 < 0) 
	{ 
		return false; 
	}
    if (v1 < v2)
    {
        return m_grapharr[v2 * (v2 - 1) / 2 + v1] == 1;
    }
    else if (v1 > v2)
    {
        return m_grapharr[v1 * (v1 - 1) / 2 + v2] == 1;
    }
    else
    {
        return false;
    }
}

//=============================================================================
// PrintAdjacencyMatrix
//=============================================================================
void MyGraph::PrintAdjacencyMatrix( void )
{
    for (int j = 0; j < m_vertices; j++)
    {
        for (int i = 0; i < m_vertices; i++)
        {
            if (i < j)
            {
                cout << m_grapharr[j * (j - 1) / 2 + i] << " ";
            }
            else if (i > j)
            {
                cout << m_grapharr[i * (i - 1) / 2 + j] << " ";
            }
            else
            {
                cout << "0 ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

//=============================================================================
// PrintGraphArray
//=============================================================================
void MyGraph::PrintGraphArray( void )
{
	cout << "[ ";
	for( int i = 0; i < m_array_size; i++ )
	{
		cout << m_grapharr[i] << " ";
	}
	cout << "]" << endl;
}

//=============================================================================
// PrintGraphArray 2
//=============================================================================
void MyGraph::PrintGraphArray( std::ofstream &s )
{
	for( int i = 0; i < m_array_size; i++ )
	{
		s << m_grapharr[i];
	}
}

//=============================================================================
// PrintDotFile
//=============================================================================
void MyGraph::PrintDotFile( const char* filename )
{
	ofstream FILE;
	FILE.open( filename );
	if( !FILE.is_open() )
	{
		cout << "Error:(PrintDotFile) Unable to open file" << endl;
		return;
	}

	FILE << "digraph G {" << endl << "node [shape=circle];" << endl
		 << "edge [dir=\"none\"];" << endl;

	// Print Vertices
	for( int i = 0; i < m_vertices; i++ )
	{
		FILE << i << " ;" << endl;
	}

	// Print Edges O(n^2)
	for( int i = 0; i < m_vertices; i++ )
	{
		for( int j = 0; j < m_vertices; j++ )
		{
            if (i < j) 				
            {
                if( m_grapharr[j * (j - 1) / 2 + i] == 1 )
				{
					FILE << i << " -> " << j << " [];" << endl;
				}
            }
			/*
            else if (i > j)
            {
                if( m_grapharr[i * (i - 1) / 2 + j] == 1)
				{
					FILE << i << " -> " << j << " [];" << endl;
				}
            }
			*/
		}
	}

	FILE << "}" << endl;
	FILE.close();
}

//=============================================================================
// DumpToBitArray - returns bytes written (please clean this up!)
//=============================================================================
int MyGraph::DumpToBitArray(char *bits, int size)
{
	uint32 i,j;

	j = 0;
	if( size <= 0 )
	{ 
		return 0; 
	}

	for( i=0; i<(uint32)this->m_array_size && i < (uint32)size; i++ )
	{
		j = (i / 8);  // integer divsion	
		if( this->m_grapharr[i] == 1 )
		{
			bits[j] |= (1 << (7-(i%8)));
		}
		else
		{
			if( bits[j] & (1<<(7-(i%8))) )
			{
				bits[j] ^= (1<<(7-(i%8)) );
			}
		}
	}

	if( i > 0  )
	{
		return j + 1;
	}
	else
	{
		return 0;
	}
}

//=============================================================================
// GetGraphArray - TEMPOARY USED ONLY FOR COLORING
//=============================================================================
uint32* MyGraph::GetGraphArray( void ) 
{
	return m_grapharr;
}

//=============================================================================
// Operator=
//=============================================================================
MyGraph& MyGraph::operator =(const MyGraph &g)
{
	if( this == &g )
	{
		return *(this);
	}

	for( int i = 0; i < MAX_MYGRAPH_ARRAY; i++ )
	{
		m_grapharr[i] = g.m_grapharr[i];
	}

	m_graphisdirty = g.m_graphisdirty;
	m_array_size = g.m_array_size;
	m_edges = g.m_edges;
	m_vertices = g.m_vertices;
	m_graphbuilder = g.m_graphbuilder;

	// Create the internal graph object
	UpdateGraph();

	return *(this);
}