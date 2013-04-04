//=============================================================================
//    Header for Graph Database Class
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

#ifndef __RMA_GRAPHDATABASE__
#define __RMA_GRAPHDATABASE__

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

//#include <boost/graph/graphviz.hpp>

#include "MyGraph.h"
#include "Combinator.h"
#include "Sql.h"
#include "Dsatur.h"

using namespace std;


#define	ISOMORPHISM				0
#define	SUBGRAPH_MONOMORPHISM	1
#define	SUBGRAPH_ISOMORPHISM	2
#define CHECK_DEFAULT			1


class GraphDatabase
{
protected:

	vector<MyGraph*> m_database;

public:

	GraphDatabase( void );
	~GraphDatabase( void );


	inline int Length( void ) { return (int) this->m_database.size(); }

	vector<MyGraph*>& GetDatabase( void );
	void PrintDotFiles( string filenme );

	int  InsertIntoDatabase( SqlManager& mgr );
	void SaveDatabase( const char* filename );
	bool LoadDatabase( const char* filename );

	void Clear( void );
	bool AddUnique( MyGraph* g, int CheckType = 1 );
		
};

#endif