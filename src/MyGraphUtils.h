#ifndef __MYGRAPHUTILS__
#define __MYGRAPHUTILS__

#include <iostream>
#include <algorithm>
#include <set>

#include "MyGraph.h"
#include "GraphDatabase.h"
#include "Combinator.h"

using namespace std;

void Get2ConnectedGraphs( GraphDatabase& db, int MaxEdges );
MyGraph* GenerateNCycle( int Length );

set<int>& MaximalIndependentSet( set<int> I, MyGraph& g, int k );

#endif