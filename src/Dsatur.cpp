
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "Types.h"
#include "Dsatur.h"

Dsatur::Dsatur( void )
{
	// Do nothing
}

Dsatur::~Dsatur( void )
{
	// Do nothing
}

//=============================================================================
// greedy_clique
//=============================================================================
// Finds the maximum
//=============================================================================
int Dsatur::greedy_clique(int* valid, int* clique)
{
	int i,j,k;
	int max;
	int place,done;
	int *order;
	int weight[MAX_NODE];

	// (Step 1) Initialize clique array to zero
	for (i=0; i<num_node; i++) 
	{
		clique[i] = 0;
	}
	
	// (Step 2 ) Allocate the array, Order, of ints of the size of n + 1 
	order = (int *)calloc(num_node+1,sizeof(int));
	place = 0;
	
	// (Step 3) If a vertex is marked as "valid" then add it to the Order array
	for (i=0; i<num_node ;i++) 
	{
		if (valid[i]) 
		{
			order[place] = i;
			place++;
		}
	}

	// (Step 4) Init Weights array to 0
	for (i=0; i<num_node; i++)
	{ 
		weight[i] = 0; 
	}

	// (Step 5) For each vertex, if it IS marked as "valid", and if it shares an edge with
	// any other "valid" vertex, then increment it's weight by 1 for each other "valid"
	// vertex it shares an edge with.
	for (i=0; i<num_node; i++) 
	{
		if (!valid[i])
		{ 
			continue; 
		}
		for (j=0; j<num_node; j++) 
		{
			if (!valid[j]) 
			{
				continue;
			}
			if (adj[i][j]) 
			{
				weight[i]++;
			}
		}
	} // End Outter For
  
	// (Step 6) 
	done = FALSE;
	while (!done) 
	{
		done = TRUE;

		// For each vertex in the Order array
		for (i=0;i <place-1; i++) 
		{
			j = order[i];
			k = order[i+1];
			if (weight[j] < weight[k]) 
			{
				order[i] = k;
				order[i+1] = j;
				done = FALSE;
			}
		}// End For
	} // End While


	clique[order[0]] = TRUE;
	for (i=1;i<place;i++) 
	{
		j = order[i];
		for (k=0;k<i;k++) 
		{
			if (clique[order[k]] && !adj[j][order[k]]) 
			{
				break;
			}
		}
		if (k==i) 
		{
			clique[j] = TRUE;
		}
		else 
		{
			clique[j] = FALSE;
		}
	}
	
	max = 0;
	for (i=0;i<place;i++) 
	{
		if (clique[order[i]]) 
		{
			max ++;
		}
	}
  
	free(order);
	//printf("Clique found of size %d\n",max);
  
	return max;
}

//=============================================================================
// max_w_clique
//=============================================================================
// Target is a goal value:  once a clique is found with value target
// it is possible to return
// 
// Lower is a bound representing an already found clique:  once it is
// determined that no clique exists with value better than lower, it
// is permitted to return with a suboptimal clique.
//
// Note, to find a clique of value 1, it is not permitted to just set
// the lower to 1:  the recursion will not work.  Lower represents a
// value that is the goal for the recursion.
//=============================================================================
int Dsatur::max_w_clique( int* valid, int* clique, int lower, int target)
{
	int start,j,k;
	int incumb,new_weight;
	int *valid1,*clique1;
	int *order;
	int *value;
	int i,place,finish,done,place1;
	//int place2;
	int total_left;
  
	/*  printf("entered with lower %d target %d\n",lower,target);*/

	num_prob++;	// Increment teh number of problems each time this method is called.
  if (num_prob > max_prob) return -1;
  for (j=0;j<num_node;j++) clique[j] = 0;
  total_left = 0;
  for (i=0;i<num_node;i++)
    if (valid[i]) total_left ++;
  if (total_left < lower) {
    return 0;
  }

  order = (int *)calloc(num_node+1,sizeof(int));
  value = (int *) calloc(num_node,sizeof(int));
  incumb = greedy_clique(valid,clique);
  if (incumb >=target) return incumb;
  if (incumb > best_clique) {
    best_clique=incumb;
/*    printf("Clique of size %5d found.\n",best_clique);*/
  }
/*  printf("Greedy gave %f\n",incumb);*/
  
  place = 0;
  for (i=0;i<num_node;i++) {
    if (clique[i]) {
      order[place] = i;
      total_left --;
      place++;
    }
  }
  start = place;
  for (i=0;i<num_node;i++) {
    if (!clique[i]&&valid[i]) {
      order[place] = i;
      place++;
    }
  }
  finish = place;
  for (place=start;place<finish;place++) {
    i = order[place];
    value[i] = 0;
    for (j=0;j<num_node;j++) {
      if (valid[j] && adj[i][j]) value[i]++;
    }
  }

  done = FALSE;
  while (!done) {
    done = TRUE;
    for (place=start;place<finish-1;place++) {
      i = order[place];
      j = order[place+1];
      if (value[i] < value[j] ) {
	order[place] = j;
	order[place+1] = i;
	done = FALSE;
      }
    }
  }
  free(value);
  for (place=start;place<finish;place++) {
    if (incumb + total_left < lower) {
      return 0;
    }
    
    j = order[place];
    total_left --;
    
    if (clique[j]) continue;
    
    valid1 = (int *)calloc(num_node,sizeof(int));
    clique1 = (int *)calloc(num_node,sizeof(int));
    for (place1=0;place1 < num_node;place1++) valid1[place1] = FALSE;
    for (place1=0;place1<place;place1++) {
      k = order[place1];
      if (valid[k] && (adj[j][k])){
	valid1[k] = TRUE;
      }
      else
	valid1[k] = FALSE;
	}
    new_weight = max_w_clique(valid1,clique1,incumb-1,target-1);
	if (new_weight+1 > incumb)  {
/*      printf("Taking new\n");*/
      incumb = new_weight+1;
      for (k=0;k<num_node;k++) clique[k] = clique1[k];
      clique[j] = TRUE;
      if (incumb > best_clique) {
	best_clique=incumb;
/*	printf("Clique of size %5d found.\n",best_clique);*/
      }
	}
    
  /*    else printf("Taking incumb\n");*/
    free(valid1);
    free(clique1);
    if (incumb >=target) break;
  }
  free(order);
  return(incumb);
}


//=============================================================================
// AssignColor
//=============================================================================
void Dsatur::AssignColor(int node, int color)
{
	int node1;

	//printf("  %d color +%d\n",node,color);

	ColorClass[node] = color;
	
	for (node1=0;node1<num_node;node1++) 
	{
		if (node==node1) 
		{
			continue;
		}
		if (adj[node][node1]) 
		{
			if (ColorAdj[node1][color]==0) 
			{
				ColorCount[node1]++;
			}
			ColorAdj[node1][color]++;
			ColorAdj[node1][0]--;
			if (ColorAdj[node1][0] < 0) 
			{
				printf("ERROR on assign\n");
			}
		}
	} // End For
}

//=============================================================================
// RemoveColor
//=============================================================================
void Dsatur::RemoveColor(int node, int color)
{
  int node1;
/*  printf("  %d color -%d\n",node,color);  */
  ColorClass[node] = 0;
  for (node1=0;node1<num_node;node1++) 
    {
      if (node==node1) continue;
      if (adj[node][node1]) 
	{
	  ColorAdj[node1][color]--;
	  if (ColorAdj[node1][color]==0) ColorCount[node1]--;
	  if (ColorAdj[node1][color] < 0) printf("ERROR on assign\n");
	  ColorAdj[node1][0]++;
	}
    }
  
}

//=============================================================================
// color
//=============================================================================
int Dsatur::color(int i, int current_color)
{
  int j,new_val;
  int k,max,place;
//  int count;
  

  prob_count++;
  if (current_color >=BestColoring) return(current_color);
  if (BestColoring <=lb) return(BestColoring);
  
  if (i >= num_node) return(current_color);
/*  printf("Node %d, num_color %d\n",i,current_color);*/
  
/* Find node with maximum color_adj */
  max = -1;
  place = -1;
  for(k=0;k<num_node;k++) 
    {
/*      count = 0;*/
      if (Handled[k]) continue;
/*      for (j=1;j<=current_color;j++)
	if (ColorAdj[k][j] > 0) count++;
      if (count!=ColorCount[k]) printf("Trouble with color count\n");
*/      
/*      printf("ColorCount[%3d] = %d\n",k,ColorCount[k]);*/
      if ((ColorCount[k] > max) || ((ColorCount[k]==max)&&(ColorAdj[k][0]>ColorAdj[place][0])))
	{
/*	  printf("Best now at %d\n",k);*/
	  max = ColorCount[k];
	  place = k;
	}
    }
  if (place==-1) 
    {
      printf("Graph is disconnected.  This code needs to be updated for that case.\n");
      exit(1);
    }

  
  Order[i] = place;
  Handled[place] = TRUE;
/*  printf("Using node %d at level %d\n",place,i);*/
  for (j=1;j<=current_color;j++) 
    {
      if (!ColorAdj[place][j]) 
	{
	  ColorClass[place] = j;
	  AssignColor(place,j);
	  new_val = color(i+1,current_color);
	  if (new_val < BestColoring){
	    BestColoring = new_val;
	    print_colors();
	  }
	  RemoveColor(place,j);
	  if (BestColoring<=current_color) {
	    Handled[place] = FALSE;
	    return(BestColoring);
	  }
	}
    }
  if (current_color+1 < BestColoring) 
    {
      ColorClass[place] = current_color+1;
      AssignColor(place,current_color+1);
      new_val = color(i+1,current_color+1);
      if (new_val < BestColoring) {
	BestColoring = new_val;
	print_colors();
      }
      
      RemoveColor(place,current_color+1);
    }
  Handled[place] = FALSE;
  return(BestColoring);
}

//=============================================================================
// print_colors
//=============================================================================
void Dsatur::print_colors( void ) 
{
	int i,j;

	// Timer variables
	current_time =  BasicLib::GetTimeMS();
  
//	printf("Best coloring is %d at time %7.1f\n",BestColoring,(current_time-start_time));
  
//	for (i=0;i<num_node;i++)
//	{
//		printf("Color[%3d] = %d\n",i,ColorClass[i]);
//	}

	for (i=0;i<num_node;i++)
	{
		for (j=0;j<num_node;j++)
		{
			if (i==j)
			{
				continue;
			}
			if (adj[i][j] && (ColorClass[i]==ColorClass[j]))
			{
				printf("Error with nodes %d and %d and color %d\n",i,j,ColorClass[i]);
			}
		} // End inner for
	} // End outer for
}

//=============================================================================
// main - Local Main entry point
//=============================================================================

uint32 Dsatur::run( uint32 vertices, uint32 edges, const uint32* grapharr, uint32 size )
{
//	FILE *fp;
	int m,i,j,val;
//	int k;
	int valid[MAX_NODE],clique[MAX_NODE];
	int place;
  
  	// Set the number of vertices and edges
	num_node = vertices;
	m = edges;

	// Initialize adjacency matrix to all zeros
	for (i=0; i<num_node; i++)
	{
		for (j=0; j<num_node; j++)
		{
			adj[i][j] = FALSE;
		}
	}

	// Add each edge to the adjacency matrix
	for( j=0; j<num_node; j++ )
	{
		for( i=0; i<num_node; i++ )
		{
            if (i < j)
            {
                adj[j][i] =  grapharr[j * (j - 1) / 2 + i];
            }
            else if (i > j)
            {
                adj[j][i] =  grapharr[i * (i - 1) / 2 + j];
            }
            else
            {
                adj[j][i] = 0;
            }
		}
	}
//	for (k=0; k<m; k++) 
//	{
//		fscanf(fp,"%d %d",&i,&j);
//		adj[i-1][j-1] = TRUE;
//		adj[j-1][i-1] = TRUE;
//	}

	// Done reading from the file
//	fclose(fp);

	prob_count = 0;
	
	// Initialize Color Adjacency matrix to all zeros
	for (i=0; i<num_node; i++)
	{
		for (j=0; j<=num_node; j++)
		{
			ColorAdj[i][j] = 0;
		}
	}

	// Set the degree for each vertex in ColorAdj[i][0], 0 <= i < n
	// I.e. First column in ColorAdj has the degree of vertex i
	for (i=0; i<num_node; i++)
	{
		for (j=0; j<num_node; j++)
		{
			if (adj[i][j])
			{
				ColorAdj[i][0]++;
			}
		}
	}

	// Initialize the ColorCount array to zero for each vertex
	for (i=0; i<num_node; i++)
	{
		ColorCount[i]=0;
	}

	// Start timing
	start_time = BasicLib::GetTimeMS();

	//printf("Graph %s read with %d nodes and %d edges\n","test",num_node,m);

	// Initialize the Handled array to zero for each vertex
	for (i=0;i <num_node; i++)
	{
		Handled[i] = FALSE;
	}

	// Set the best-coloring so far to n + 1
	BestColoring = num_node+1;

//  ColorClass[0] = 1;
//  AssignColor(0,1);
//  Handled[0] = TRUE;

	// Initialize the valid array to 1 for each vertex
	for (i=0; i<num_node; i++) 
	{
		valid[i] = TRUE;
	}

	best_clique = 0;
	num_prob    = 0;
	max_prob    = 10000;
  
	lb = max_w_clique( valid, clique, 0, num_node);
	place = 0;

	for (i=0;i<num_node;i++) 
	{
		if (clique[i]) 
		{
			Order[place] = i;
			Handled[i] = TRUE;
			place++;
			AssignColor(i,place);

			for (j=0;j<num_node;j++)
			{
				if ((i!=j)&&clique[j] && (!adj[i][j])) 
				{
					printf("Result is not a clique!\n");
				}
			} // End Inner For
		}
	} // End Outer For
  
//	printf("Lower bound is %d",lb);

	if (num_prob >=max_prob) 
	{
		printf(" (not confirmed)\n");
	}
	else 
	{
		printf("\n");
	}

	val = color(place,place);
	current_time = BasicLib::GetTimeMS();  

//	printf("Best coloring has value %d, subproblems: %d time:%7.1f\n",val,prob_count,(current_time-start_time));  
	return (uint32) val;
}
	

