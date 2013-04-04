//=============================================================================
//    Implementation of the tinygraph class
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
#include <exception>
#include <iostream>
#include <fstream>
#include <math.h>
#include "Tinygraph.h"

//=============================================================================
// Constructor(1)
//=============================================================================
tinygraph::tinygraph( unsigned int vertices ) : m_matrix( ((m_vertices*(m_vertices-1))/2 ) )
{
	this->m_vertices   = vertices;
	this->m_edges      = 0;
	this->ClearEdges();
}

//=============================================================================
// Constructor(2)
//=============================================================================
tinygraph::tinygraph( const tinygraph &tg ) : m_matrix( tg.m_matrix )
{
	this->m_vertices = tg.m_vertices;
	this->m_edges = tg.m_edges;
}

//=============================================================================
// Constructor(3)
//=============================================================================
tinygraph::tinygraph( uint32 vertices, uchar *arr, uint32 size ) : m_matrix( arr, size )
{	
	this->m_vertices   = vertices;
	this->m_edges      = m_matrix.Count(); // Num Edges equals num set bits
}

//=============================================================================
// Destructor
//=============================================================================
tinygraph::~tinygraph( void )
{
	// Do Nothing
}

//=============================================================================
// Vertices - Returns the number of vertices
//=============================================================================
uint32 tinygraph::Vertices( void ) const
{
	return this->m_vertices;
}

//=============================================================================
// Edges - Returns the number of edges
//=============================================================================
uint32 tinygraph::Edges( void ) const
{
	return this->m_edges;
}
//=============================================================================
// AddVertex - Adds a new, disconnected vertex to the graph
//=============================================================================
int tinygraph::AddVertex( void )
{		
	// For a new vertex, we need one bit for every old vertex

	this->m_matrix.Expand( this->m_vertices );
	this->m_vertices++;

	return 1;
}
//=============================================================================
// AddEdge - Adds a new edges between vertices u and v
//=============================================================================

int tinygraph::AddEdge( uint32 u, uint32 v )
{
	if( u == v )
	{
		exception e("Edge can't begin and end in the same vertex!");
		throw e;
	}

	if( u >=this->m_vertices || v >= this->m_vertices )
	{
		exception e("Tinygraph: Unable to add edge to non-existant vertex!");
		throw e;
	}

	uint32 i;
	if( u < v )
	{
		i = ( v * ( v - 1 ) / 2 ) + u;
	}
	else
	{
		i = ( u * ( u - 1 ) / 2 ) + v;
	}

	this->m_matrix[i] = true;
	return 1;
}

//=============================================================================
// AddEar - Adds a internally disjoint path of lengh L between vertices u and v
//=============================================================================
int tinygraph::AddEar( uint32 u, uint32 v, uint32 l )
{
	if( u >= this->m_vertices || v >= this->m_vertices || l == 0)
	{
		exception e("Tinygraph::AddEar: Invalid vertex parameters.");
		throw e;
	}

	if( l == 1 )
	{
		return this->AddEdge( u, v );
	}
	else
	{
		this->AddVertex();
		this->AddEdge( u, this->m_vertices-1 );

		for( uint32 i=1; i<l-1; i++ )
		{
			this->AddVertex();
			this->AddEdge( this->m_vertices-2, this->m_vertices-1 );
		}

		this->AddEdge( this->m_vertices-1, v );
	}
	return 1;
}

//=============================================================================
// DeleteVertex - Deletes a vertex and any edges connected to it. Forces a resizing
//=============================================================================
int tinygraph::DeleteVertex( uint32 v )
{
	if( v >= this->m_vertices )
	{
		exception e("TinyGraph::DeleteVertex: Can't delete non-existant vertex.");
		throw e;
	}

}

//=============================================================================
// ClearEdges Removes all edges from the graph
//=============================================================================
void tinygraph::ClearEdges()
{
	this->m_matrix.Clear();
}
