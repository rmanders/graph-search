/**
 * Implementation of Ugraph 
 */

#include <fstream>
#include "ugraph.h"
#include "errors.h"

/**
 * ============================================================================
 * Initialization method called from constructors
 * ============================================================================
 */
void Ugraph::init( void ) {
	for( Uint32Type i=0; i<MAX_GRAPH_SIZE; i++) {
                mLowerDiag[i] = 0;
        }
	mVertices = 0;
	mEdges = 0;
	mLowerDiagLen = 0;
}

/**
 * ============================================================================
 * Default constructor
 * ============================================================================
 */
Ugraph::Ugraph( void ) {
	init();
}

/**
 * ============================================================================
 * Construct with number of vertices
 * ============================================================================
 */
Ugraph::Ugraph( Uint32Type nVertices ) {

	init();
	mVertices = nVertices;

	// Calculate LowerDiag size
	Uint32Type sz = (mVertices * (mVertices - 1)) / 2;
	
	// Check for size
	if( sz >= MAX_GRAPH_SIZE)
		throw GsException(GSERR_MAX_GRAPH_SIZE_EXCEEDED);

	mLowerDiagLen = sz;	
}

/**
 * ============================================================================
 * Copy constructor
 * ============================================================================
 */
Ugraph::Ugraph( const Ugraph& uGraph ) {

	for( Uint32Type i=0; i<MAX_GRAPH_SIZE; i++)
		mLowerDiag[i] = uGraph.mLowerDiag[i];

	mLowerDiagLen = uGraph.mLowerDiagLen;
	mVertices = uGraph.mVertices;
	mEdges = uGraph.mEdges;	
}

/**
 * ============================================================================
 * Construct external array and parameters which must all agree. 
 * ============================================================================
 * 
 * The external array will be translated to 1's and 0's as into the lower
 * diagonal array. If the external array value is 0 or less it will translate
 * to a 0 in the lower diag. If it is greater than 0, it will translate to a 1.
 */
Ugraph::Ugraph( Uint32Type nVertices, Uint32Type nEdges, Uint32Type sLowerDiagLen, ByteType aLowerDiag[] ) {

	init();
	
	Uint32Type arrSize = (Uint32Type) (sizeof(aLowerDiag) / sizeof(ByteType));

	// Check array sizes
	if( sLowerDiagLen > MAX_GRAPH_SIZE )
		throw GsException(GSERR_MAX_GRAPH_SIZE_EXCEEDED);
	
	// Check lower diag actual size agrees with number of vertices
	// TODO: make exception for this
	Uint32Type expectedSize = (nVertices * (nVertices - 1)) / 2;
	if( expectedSize != sLowerDiagLen || expectedSize > arrSize)
		throw GsException();
	
	// Copy over the data
	mLowerDiagLen = sLowerDiagLen;
	mVertices = nVertices;
	mEdges = nEdges;
	
	Uint32Type actualEdges = 0;	
	for( Uint32Type i=0; i<mLowerDiagLen; i++ ) {
		mLowerDiag[i] = (aLowerDiag[i] > 0) ? 1 : 0;
		if( mLowerDiag[i] == 1) { actualEdges++; }
	}

	// Check for edge agreement
	if( mEdges != actualEdges ) {
		throw GsException(GSERR_EDGE_MISMATCH);
	}
}

/**
 * ============================================================================
 * Destructor
 * ============================================================================
 */
Ugraph::~Ugraph( void ) {
	// Do Nothing
}

/**
 * ============================================================================
 * Return the number of vertices
 * ============================================================================
 */
Uint32Type Ugraph::NumVertices( void ) {
	return mVertices;
}

/**
 * ============================================================================
 * Return the number of edges
 * ============================================================================
 */
Uint32Type Ugraph::NumEdges( void ) {
        return mEdges;
}

/**
 * ============================================================================
 * Return true is an edge exists between the two vertices
 * ============================================================================
 */
bool Ugraph::IsEdge( VertexIdType vFirst, VertexIdType vSecond ) {
        if( vFirst >= mVertices || vSecond >= mVertices )
		throw GsException(GSERR_NO_SUCH_VERTEX);

	// No Self edges
	if( vFirst == vSecond) {
		return false;
	}
	
	return mLowerDiag[VOFFSET(MAX(vFirst,vSecond)) + MIN(vFirst,vSecond)] == 1;
}

/**
 * ============================================================================
 * Returns a newly allocated n x n adjacency matrix
 * ============================================================================
 */
VertexIdType** Ugraph::AdjMatrix( void ) {

	VertexIdType** adj = new VertexIdType*[mVertices];
	for(Uint32Type i=0; i<mVertices; i++) {
		adj[i] = new VertexIdType[mVertices];
		for(Uint32Type j=0; j<mVertices; j++) {
			if(i<j) {
				adj[i][j] = mLowerDiag[VOFFSET(i)+j];
			}
			else if(j<i) {
				adj[i][j] = mLowerDiag[VOFFSET(j)+i];
			} 
			else {
				adj[i][j] = 0;
			}
		}
	}
	return adj;
}

/**
 * ============================================================================
 * Returns a newly allocated n * (n-1) / 2 sized array of the lower diagonal
 * ============================================================================
 */
VertexIdType* Ugraph::LowerDiag( void ) {
	VertexIdType* lowerDiag = new VertexIdType[mLowerDiagLen];
	memcpy(lowerDiag, mLowerDiag, sizeof(mLowerDiag));
	return lowerDiag;
}


/**
 * ============================================================================
 * Clears all graph data
 * ============================================================================
 */
void Ugraph::Clear( void ) {
	init();
}

/**
 * ============================================================================
 * Add a new vertex to the graph
 * ============================================================================
 */
void Ugraph::AddVertex( void ) {
	
	// make sure there is enough room to add a new vertex
	if( mLowerDiagLen + mVertices > MAX_GRAPH_SIZE )
		throw GsException(GSERR_MAX_GRAPH_SIZE_EXCEEDED);
	
	// Initialize new vertex space
	for(Uint32Type i=mLowerDiagLen; i<(mLowerDiagLen+mVertices); i++)
		mLowerDiag[i] = 0;

	// Set the new space;
	mLowerDiagLen += mVertices;
	mVertices++;
}

/**
 * ============================================================================
 * Add a new edge between two vertices
 * ============================================================================
 */
void Ugraph::AddEdge( VertexIdType vFirst, VertexIdType vSecond ) {
        if( vFirst >= mVertices || vSecond >= mVertices )
                throw GsException(GSERR_NO_SUCH_VERTEX);

        // No Self edges, just do nothing and return
        if( vFirst == vSecond)
		return;

	Uint32Type i = VOFFSET(MAX(vFirst,vSecond)) + MIN(vFirst,vSecond);

	// Throw exception if overwriting an existing edge. This may be excessive, but 
	// for now want to know if program is unintentionally overwriting edges.
        if(mLowerDiag[i] == 1)
		throw GsException(GSERR_EDGE_ALREADY_EXISTS);
	else
		mLowerDiag[i] = 1;
		mEdges++;
}

/**
 * ============================================================================
 * Add a path of arbitrary length between two vertices. 
 * ============================================================================
 */
void Ugraph::AddDisjointPath( VertexIdType vBegin, VertexIdType vEnd, Uint32Type pLen ) {

        if( vBegin >= mVertices || vEnd >= mVertices )
                throw GsException(GSERR_NO_SUCH_VERTEX);
	
	if( pLen == 1) {
		AddEdge(vBegin, vEnd);
	} else {
		for(Uint32Type i=0; i<pLen; i++) {
			if( i == 0 ) {
				AddVertex();
				AddEdge(vBegin, mVertices-1);
			}
			else if( i+1 == pLen ) {
				AddEdge(mVertices-1, vEnd);
			}
			else {
				AddVertex();
				AddEdge(mVertices-2, mVertices-1);
			}
		}
	}
}

/**
 * ============================================================================
 * Print Adjacency matrix to stdout
 * ============================================================================
 */
void Ugraph::PrintAdjacencyMatrix( void ) {

       for(Uint32Type i=0; i<mVertices; i++) {
                for(Uint32Type j=0; j<mVertices; j++) {
                        if(i<j) {
                                cout << (int) mLowerDiag[(VOFFSET(j)+i)] << " ";
                        }
                        else if(j<i) {
                                cout << (int) mLowerDiag[VOFFSET(i)+j] << " ";
                        }
                        else {
                                cout << "0 ";
                        }
                }
		cout << endl;
        }
	cout << endl;
}

/**
 * ============================================================================
 * Print the lower diagonal of the adjacency matrix as an array
 * ============================================================================
 */
void Ugraph::PrintLowerDiag( void ) {
	for(Uint32Type i=0; i<mLowerDiagLen; i++)
		cout << (int) mLowerDiag[i];
	
	cout << endl;
}

/**
 * ============================================================================
 * Output the graph as a graphviz dot file
 * ============================================================================
 */
void Ugraph::PrintDotFile( const char* dFileName) {
	
	ofstream FILE;
	FILE.open( dFileName );
	if( !FILE.is_open() ) {
		cerr << "Error:(PrintDotFile) Unable to open file" << endl;
		return;
	}

	FILE << "digraph G {" << endl << "node [shape=circle];" << endl << "edge [dir=\"none\"];" << endl;

	// Print Vertices
	for( Uint32Type i=0; i<mVertices; i++ ) {
		FILE << i << " ;" << endl;
	}

	// Print Edges O(n^2)
	for( Uint32Type i=0; i<mVertices; i++ ) {
		for( Uint32Type j=0; j<mVertices; j++ ) {
			if (i < j) {
				if( mLowerDiag[VOFFSET(j) + i] == 1 ) {
					FILE << i << " -> " << j << " [];" << endl;
				}
			}
			/*
			else if (i > j) { 
				if( m_grapharr[i * (i - 1) / 2 + j] == 1) {
					FILE << i << " -> " << j << " [];" << endl;
				}
			}
			*/
		}
	}

	FILE << "}" << endl;
	FILE.close();
}

