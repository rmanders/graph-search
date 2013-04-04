
#include "GraphDatabase.h"

//=============================================================================
// Constructor 1
//=============================================================================
GraphDatabase::GraphDatabase( void ) : m_database()
{
	// Do nothing
}

//=============================================================================
// Destructor
//=============================================================================
GraphDatabase::~GraphDatabase( void )
{
	m_database.clear();
}

//=============================================================================
// GetDatabase
//=============================================================================
vector<MyGraph*>& GraphDatabase::GetDatabase( void )
{
	return m_database;
}

//=============================================================================
// PrintDotFiles
//=============================================================================
void GraphDatabase::PrintDotFiles( string filename )
{
	vector<MyGraph*>::iterator itr;
	int count = 0;
	string fullpath;
	Dsatur* c = new Dsatur();

	for( itr = m_database.begin(); itr != m_database.end(); itr++ )
	{
		ostringstream ss;
		ss <<  "[" << (*itr)->NumVertices() << "-" << (*itr)->NumEdges() << "]_X[" << 
			c->run( (*itr)->NumVertices(), (*itr)->NumEdges(), (*itr)->GetGraphArray(), (*itr)->GraphArraySize() ) << 
			"]_"<< count++;
		fullpath = filename + ss.str() + ".dot";
		(*itr)->PrintDotFile( fullpath.c_str() );
	}

	delete c;
}

//=============================================================================
// InsertIntoDatabase
//=============================================================================
int GraphDatabase::InsertIntoDatabase( SqlManager& mgr )
{
	char data[1024];
	int bytes;
	int count;
	vector<MyGraph*>::iterator itr;

	count = 0;

	for( itr = m_database.begin(); itr != m_database.end(); itr++ )
	{
		memset( data, 0x0, 1024 );
		bytes = (*itr)->DumpToBitArray( data, 1024 );

		fprintf( stderr, "Inserting V: %d, E: %d into DB\n", (*itr)->NumVertices(), (*itr)->NumEdges());

		count += mgr.InsertEdgeGraph( (uint32)(*itr)->NumVertices(), (uint32)(*itr)->NumEdges(), data, (uint32) bytes );		
	}
	return count;
}

//=============================================================================
// SaveDatabase
//=============================================================================
// Dump database to following text format:
// <title>:<vertices>:<edges>:<arraysize>:<array(0|1)>
//=============================================================================
void GraphDatabase::SaveDatabase( const char *filename )
{
	ofstream FILE;
	FILE.open( filename );

	if( !FILE.is_open() )
	{
		exception e("Error:(SaveDatabase) Unable to open file");
		throw e;
	}

	int count = 0;
	vector<MyGraph*>::iterator itr;
	for( itr = m_database.begin(); itr != m_database.end(); itr++ )
	{
		FILE << "Graph" << count++ << ":" << (*itr)->NumVertices() << ":"
			 << (*itr)->NumEdges() << ":" << (*itr)->GraphArraySize() << ":";

		(*itr)->PrintGraphArray( FILE );
		FILE << endl;
	}
	FILE.close();
}

//=============================================================================
// LoadDatabase
//=============================================================================
// <title>:<vertices>:<edges>:<arraysize>:<array(0|1)>
//=============================================================================
bool GraphDatabase::LoadDatabase( const char* filename )
{
	ifstream FILE;
	FILE.open( filename );

	if( !FILE.is_open() )
	{
		exception e("Error:(LoadDatabase) Unable to open file");
		throw e;
	}

	// input variables
	char s[2];
	char buff[10];
	char title[30];
	char arraybuffer[MAX_MYGRAPH_ARRAY];
	int grapharray[MAX_MYGRAPH_ARRAY] = { { '\0' } };
	int arraysize;
	int edges;
	int vertices;

	MyGraph* g;

	s[1] = '\0';		// Set up s for easy atoi function usage;

	while( !FILE.getline(title, 30, ':').eof() )
	{

		FILE.getline( buff, 10, ':' );	// Get Vertices
		vertices = atoi( buff );

		FILE.getline( buff, 10, ':' );	// Get Edges
		edges = atoi( buff );

		FILE.getline( buff, 10, ':' ); // Get Graph Array Size
		arraysize = atoi( buff );

		FILE.getline( arraybuffer, MAX_MYGRAPH_ARRAY );

		// Convert grapharray buffer to integer array. 
		// Should be one character per entry
		for( int i = 0; i < arraysize; i++ )
		{
			if( !(arraybuffer[i] == '0' || arraybuffer[i] == '1') )
			{
				FILE.close();
				exception e("Error (LoadDatabase): Invalid input file format [array field]");
				throw e;
			}
			s[0] = arraybuffer[i];
			grapharray[i] = atoi( s );
		}

		// Create the new graph object
		g = NULL;
		g = new MyGraph( vertices, edges, arraysize, grapharray );

		if( NULL == g )
		{
			FILE.close();
			exception e("Error:(LoadDatabase) Out of memory!");
			throw e;
		}

		m_database.push_back( g );

	}

	FILE.close();
	return true;
}

//=============================================================================
// Clear
//=============================================================================
void GraphDatabase::Clear( void )
{
	m_database.clear();
}


//=============================================================================
// AddUnique
//=============================================================================
/// Adds a graph to the database if it is not ismorphic to any other graph in
/// the database. Returns true if the graph as added and false otherwise.
//=============================================================================
bool GraphDatabase::AddUnique(MyGraph *g, int CheckType)
{
	vector<MyGraph*>::iterator itr;
	for( itr = m_database.begin(); itr != m_database.end(); itr++ )
	{
		if( CheckType == 0)
		{
			if( g->NumEdges() == (*itr)->NumEdges() &&
				(*itr)->IsIsomorphicTo( *g ) )
			{
				return false;
			}
		}
		else if( CheckType == 1 )
		{
			if( g->NumEdges() == (*itr)->NumEdges() &&
				g->IsContainedBy( **itr ) )
			{
				return false;
			}
		}
	}
	m_database.push_back( g );
	return true;
}