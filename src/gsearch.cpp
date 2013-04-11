
#include <iostream>
#include "types.h"
#include "ugraph.h"


using namespace std;

int main( int argc, char* argv[] ) {
	
	Ugraph g(10);
	g.AddEdge(0,1);
	g.AddEdge(0,2);
	g.AddEdge(1,2);
	g.AddEdge(0,3);
	cout << "vertices: " << g.NumVertices() << endl;
        cout << "Edges: " << g.NumEdges() << endl;
	g.PrintAdjacencyMatrix();
	g.PrintLowerDiag();

	return 0;
}

