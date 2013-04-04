
#include "MyGraphUtils.h"


//=============================================================================
// Generates all 2-connected graphs starting with K3 up to teh max number 
// of edges specified.
//=============================================================================
void Get2ConnectedGraphs( GraphDatabase& db, int MaxEdges )
{

	// (STEP A) Initialize Graph Database with g3 & g4
	db.AddUnique( GenerateNCycle( 3 ), CHECK_DEFAULT );
	db.AddUnique( GenerateNCycle( 4 ), CHECK_DEFAULT );

	for( int n = 5; n < MaxEdges; n++ )
	{
		// (STEP B) Initialize Database With new N-Cycle
		db.AddUnique( GenerateNCycle( n ), CHECK_DEFAULT );

		// (STEP C) Copy DB to temp array
		vector<MyGraph*> tmpvec( db.GetDatabase() );
		vector<MyGraph*>::iterator itr;

		// (STEP D) For each graph in the DB Array find unique bi-connected
		int c=0;
		for( itr = tmpvec.begin(); itr != tmpvec.end(); itr++ )
		{
			cout << "\r Edge [" << n << " of " << MaxEdges << "] Iteration ["
				 << c++ << " of " << tmpvec.size() << "]";

			// (STEP E) Compute Edges that need to be added
			int pathlen = n - (*itr)->NumEdges();

			// (STEP F) Only add edges if the path length is greater than 0
			if( pathlen > 0 )
			{
				// (STEP G) Create a Combinator and add the new path in all possible pays
				BinaryCombinator nCr( (*itr)->NumVertices(), 2 );

				do
				{
					// (STEP H) Create a test graph copy of k, add path, and test for isomorphism
					int result[3] = { 0 };
					nCr.Visit( result );

					MyGraph* g = NULL;
					g = new MyGraph( **itr );

					if( NULL == g )
					{
						exception e("Error in Get2ConnectedGraphs: Out of memory!");
						throw e;
					}

					g->AddDisjointPath( result[0]-1, result[1]-1, pathlen );
					if( !db.AddUnique( g, CHECK_DEFAULT ) )
					{
						delete g;
						g = NULL;
					}					

				} while( nCr.Step() );
			}
		}
	}
}

//=============================================================================
// GenerateNCycle
//=============================================================================
MyGraph* GenerateNCycle( int Length )
{
	MyGraph* g = NULL;
	g = new MyGraph( Length );

	if( NULL == g )
	{
		exception e("Unable to allocate memory!");
		throw e;
	}

	for( int i = 0; i < Length; i++ )
	{
		g->AddEdge( i, (i+1)%Length );
	}

	return g;
}

//=============================================================================
//set<int>& MaximalIndependentSet( MyGraph& g );
//=============================================================================
//set<int>& MaximalIndependentSet( set<int> I, MyGraph& g, int k )
//{
//	
//}