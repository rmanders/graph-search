#include "Sql.h"
#include <sstream>

//=============================================================================
// SqlManager Constructor
//=============================================================================
SqlManager::SqlManager( sqlcon_t* conn )
{
	strcpy( this->m_conn.database, conn->database );
	strcpy( this->m_conn.hostname, conn->hostname );
	strcpy( this->m_conn.password, conn->password );
	strcpy( this->m_conn.portnum,  conn->portnum  );
	strcpy( this->m_conn.username, conn->username );
}

//=============================================================================
// SqlManager Destructor
//=============================================================================
SqlManager::~SqlManager( void )
{
	// Do nothing
}

//=============================================================================
// Inserts a graph value into the database, returns 0 if failed, 1 if success
//=============================================================================
int SqlManager::InsertEdgeGraph(uint32 vertices, uint32 edges, char *data, uint32 size)
{
	int result;
	int len;
	char convdata[1024*2];  // Converted Graphs up to 1kb in size
	char query[5000];
	char* qu = "INSERT INTO GRAPHS (VERTICES,EDGES,DATA,DATA_SIZE) VALUES('%d','%d','%s','%d')";

	MYSQL*        sqlcon;

	if( vertices == 0 || NULL == data || size <= 0 )
	{
		fprintf( stderr, "SqlManager::InsertEdgeGraph: Invalid Parameters\n");
		return 0;
	}
	if( size > 1024 )
	{
		fprintf( stderr, "SqlManager::InsertEdgeGraph: Graph size exceeds 1kb\n");
		return 0;
	}

	sqlcon = mysql_init( NULL );

	if( !mysql_real_connect( 
		sqlcon, 
		this->m_conn.hostname, 
		this->m_conn.username, 
		this->m_conn.password, 
		this->m_conn.database, 
		0, 
		NULL, 
		0 ) )
	{
		fprintf(stderr, "SqlManager::InsertEdgeGraph: %s\n", mysql_error( sqlcon ) );
		return 0;
	}

	mysql_real_escape_string( sqlcon, convdata, data, (unsigned long) size );
	len = sprintf_s( query, sizeof(qu) + sizeof(convdata), qu, vertices, edges, convdata, size );
	
	mysql_real_query( sqlcon, query, len );

	mysql_close( sqlcon );
	return 0;
}
