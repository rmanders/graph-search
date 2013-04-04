/*
 * The author of this software is Michael Trick.  Copyright (c) 1994 by 
 * Michael Trick.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose without fee is hereby granted, provided that this entire notice
 * is included in all copies of any software which is or includes a copy
 * or modification of this software and in all copies of the supporting
 * documentation for such software.
 * THIS SOFTWARE IS BEING PROVIDED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTY.  IN PARTICULAR, NEITHER THE AUTHOR DOES NOT MAKE ANY
 * REPRESENTATION OR WARRANTY OF ANY KIND CONCERNING THE MERCHANTABILITY
 * OF THIS SOFTWARE OR ITS FITNESS FOR ANY PARTICULAR PURPOSE.
 */

/*
   COLOR.C: Easy code for graph coloring
   Author: Michael A. Trick, Carnegie Mellon University, trick+@cmu.edu
   Last Modified: November 2, 1994


Graph is input in a file.  First line contains the number of nodes and
edges.  All following contain the node numbers (from 1 to n) incident to 
each edge.  Sample:

4 4
1 2
2 3
3 4
1 4

represents a four node cycle graph.

Code is probably insufficiently debugged, but may be useful to some people.

For more information on this code, see Anuj Mehrotra and Michael A. Trick,
"A column generation approach to graph coloring", GSIA Technical report series.

*/

#ifndef __DSATUR__
#define __DSATUR__

// Timming Includes
#include "BasicLibTypes.h"
#include "BasicLibTime.h"

#define MAX_RAND (2.0*(1 << 30))
#define MAX_NODE 600
#define TRUE 1
#define FALSE 0
#define INF 100000.0


class Dsatur
{
private:

	BasicLib::sint64 current_time;
	BasicLib::sint64 start_time;
	int adj[MAX_NODE][MAX_NODE];			// Graph Adjacency Matrix
	int BestColoring;						// The best k-coloring found 
	int num_node;							// The number of vertices
	int ColorClass[MAX_NODE];
	int prob_count;
	int Order[MAX_NODE];
	int Handled[MAX_NODE];
	int ColorAdj[MAX_NODE][MAX_NODE];
	int ColorCount[MAX_NODE];
	int lb;
	int num_prob;
	int max_prob;

	int best_clique;

public:

	Dsatur( void );
	~Dsatur( void );
	int color(int i, int current_color);
	int greedy_clique(int* valid, int* clique);
	int max_w_clique( int* valid, int* clique, int lower, int target);
	void AssignColor(int node, int color);
	void RemoveColor(int node, int color);
	void print_colors( void );
	uint32 run( uint32 vertices, uint32 edges, const uint32* grapharr, uint32 size );
};

#endif