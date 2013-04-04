//=============================================================================
// This code implements the connection to a sql server and manages inserting
// graph data into it.
//=============================================================================
#ifndef __RMASQL__
#define __RMASQL__

#ifdef _WIN32
	#include <config-win.h>
#endif

#include <mysql.h>
#include <iostream>
#include "Types.h"

using namespace std;

typedef struct
{
	char hostname[128];
	char username[32];
	char password[64];
	char database[64];
	char portnum [16];
} sqlcon_t;

class SqlManager
{
private:
	sqlcon_t m_conn;

public:
	SqlManager( sqlcon_t* conn );
	~SqlManager( void );
	
	int InsertEdgeGraph( uint32 vertices, uint32 edges, char* data, uint32 size );
};

#endif
