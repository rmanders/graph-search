//=============================================================================
//    Header file for tinygraph class
//
//    Copyright (C) 2009  Ryan Anderson, University of Cincinnati
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//=============================================================================
#ifndef __TINYGRAPH__
#define __TINYGRAPH__

#include <iostream>
#include <fstream>
#include "Types.h"
#include "Bitvector.h"

#define TG_BYTE_LEN             8								// Bits per byte (As if this will ever change)
#define TG_MAX_SIZE				20                              // Max graph Array Size
#define TG_MAX_VERTICES         TG_BYTE_LEN * TG_MAX_SIZE		// Max number of vertices/bits

#define TG_ERR_EXEEDEDMAX		1

using namespace std;
//=============================================================================
// Tinygraph is designed to be a very compact way of representing an undirected
// graph.  The graph is represented as the lower diagional of an adjacency
// matrix. The matrix is implemented using a bitvector in an array of unsigned
// integers where the least significant bit would be index[0][1] in a
// traditional 2d array. For this reason, the tinygraph can ONLY handle vertex
// adjacency and NOT weighted values.
//=============================================================================
class tinygraph
{
private:

  Bitvector     m_matrix;					   // The Adjacency Matrix
  unsigned int  m_vertices;                    // Total number of vertices present
  unsigned int  m_edges;                       // Total number of edges present

public:

  tinygraph( unsigned int vertices );           // Constructor with num Vertices specified
  tinygraph( const tinygraph& tg );             // Copy Constructor
  tinygraph( uint32 vertices, uchar* arr,		// Constructor from array of bits
	         uint32 size );
  ~tinygraph( void );                           // Destructor

  uint32 Vertices( void ) const;				// Returns the number of vertices
  uint32 Edges( void ) const;					// Returns the number of edges

  int AddVertex( void );                        // Adds a new, disconnected vertex to the graph
  int AddEdge( uint32 u, uint32 v );            // Adds a new edges between vertices u and v
  int AddEar( uint32 u, uint32 v, uint32 l );   // Adds a internally disjoint path of lengh L between vertices u and v
  int DeleteVertex( uint32 v );					// Deletes a vertex and any edges connected to it. Forces a resizing
  int DeleteEdge( uint32 u, uint32 v );			// Removes an edge from the graph

  void ClearEdges( void );                      // Removes all edges from the graph
  void ClearGraph( void );                      // Removed all edges and vertices from the graph

  bool IsAdjacent( uint32 u, uint32 v);         // Returns true is u is adjacent to v

  void PrintAdjacencyMatrix( void );            // Prints adjaceny matrix to stdout
  void PrintGraphArray( void );                 // Prints contents of graph array to stdout
  //void PrintGraphArrayS( ofstream& s );         // Prints contents of graph array to filestream
  void PrintDotFile( const char* filename );    // Dumps the graph to a dot file

  tinygraph& operator=  ( const tinygraph& g );	// Assignment operator overload
};

#endif
