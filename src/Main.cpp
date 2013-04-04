
#include <iostream>
#include <set>
#include <cstdlib>
#include <math.h>

#include "Types.h"
#include "Graph.h"
#include "GraphGenerator.h"
#include "GraphDatabase.h"

// Timming Includes
#include "BasicLibTypes.h"
#include "BasicLibTime.h"

#include "MyGraphUtils.h"
#include "MyGraph.h"
#include "Combinator.h"
#include "libconfig.h"
#include "sql.h"
#include "Bitvector.h"
#include <bitset>

using namespace std;

uint64 f( int n );
uint64 nCr( int n, int k );


//=============================================================================
// Main
//=============================================================================
int main( int argc, char* argv[] )
{

//	Bitvector test( (size_t) 6 );
//	cout << test.Test( 5 ) << endl;
//	test.Set( 5, true );
//	cout << test.Test( 5 ) << endl;
//	test.Print();
//	test.Expand( 4 );
//	test.Print();


	
//	sqlcon_t conn;

//	strcpy( conn.database, "graphdb");
//	strcpy( conn.hostname, "192.168.1.104");
//	strcpy( conn.password, "star310");
//	strcpy( conn.username, "graphman");
//	strcpy( conn.portnum, "3306");

//	SqlManager mgr( &conn );
	

	// TEST CASE: K4 vs 4-cycle

	//GraphDatabase DB;
	//DB.LoadDatabase("C:\\development\\Distance_One_Graphs\\debug\\forbidden.txt");
	//DB.PrintDotFiles("C:\\development\\Distance_One_Graphs\\debug");

	
//	GraphDatabase db;
//	Get2ConnectedGraphs( db, 7 );
//	
//	cout << db.Length() << endl;
//
//	db.PrintDotFiles("C:\\development\\Distance_One_Graphs\\debug");
//	db.SaveDatabase("C:\\development\\Distance_One_Graphs\\debug\\testdb.txt");
	

	int length = 0;

	// Handle command line input
	if( argc != 2 )
	{
		cout << "Usage: " << argv[0] << " <number of Edges>" << endl;
		return 1;
	}

	length = (int) atoi( argv[1] );

	if( length > 25 || length < 1 )
	{
		length = 1;
	}
	else
	{
		length = length++;
	}

	//length = nCr( length, 2 );

	// Timer variables
	BasicLib::sint64 t_begin;
	BasicLib::sint64 t_end;

	// Declare Databases
	GraphDatabase MainDB;
	GraphDatabase DrawableDB;
	GraphDatabase NotDrawableDB;
	GraphDatabase ForbiddenDB;

	// Populate databases
	ForbiddenDB.LoadDatabase("C:\\development\\Distance_One_Graphs\\Databases\\forbidden_default.txt");
	MainDB.LoadDatabase("C:\\development\\Distance_One_Graphs\\Databases\\test.txt");
	//Get2ConnectedGraphs( MainDB, length );

	// Separate out graphs containing forbidden graphs
	bool flag = false;
	vector<MyGraph*>::iterator itr;
	vector<MyGraph*>::iterator itr2;

	// Get timer
	t_begin = BasicLib::GetTimeMS();

	for( itr = MainDB.GetDatabase().begin(); itr != MainDB.GetDatabase().end(); itr++ )
	{
		flag = false;
		for( itr2 = ForbiddenDB.GetDatabase().begin(); itr2 != ForbiddenDB.GetDatabase().end(); itr2++ )
		{
			if( (*itr2)->IsContainedBy( *(*itr) ) )
			{
				NotDrawableDB.AddUnique( (*itr), CHECK_DEFAULT );
				flag = true;
			}
		}
		if( !flag )
		{
			DrawableDB.GetDatabase().push_back( (*itr) );
		}
	}

	NotDrawableDB.PrintDotFiles("C:\\Development\\Distance_One_Graphs\\Graph_Pictures\\NotDrawable");
	DrawableDB.PrintDotFiles("C:\\Development\\Distance_One_Graphs\\Graph_Pictures\\Drawable");
	ForbiddenDB.PrintDotFiles("C:\\development\\Distance_One_Graphs\\debug\\Forbidden");
	DrawableDB.SaveDatabase("C:\\Development\\Distance_One_Graphs\\Databases\\Drawable.txt");

	//	DrawableDB.InsertIntoDatabase( mgr );


	// Set End Time
	t_end = BasicLib::GetTimeMS();

	cout << endl;
	cout << endl;
	cout << "\tRuntime: " << t_end-t_begin << " MS" << endl << endl;

	return 0;
}

//=============================================================================
// f
//=============================================================================
uint64 f( int n )
{
	uint64 f = (uint64) n;

	for( uint64 i = (uint64)n-1; i > 0; i-- )
	{
		f = f * i;
	}
	return f;
}

//=============================================================================
// nCr
//=============================================================================
uint64 nCr( int n, int k )
{
	return f( n ) / (f( k ) * f( n - k ));
}

