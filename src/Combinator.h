
#ifndef __BINARY_COMBINATOR__
#define __BINARY_COMBINATOR__

#include <iostream>

using namespace std;

#define MAX_COMB_ARRAY 1024

class BinaryCombinator
{
private:

	int m_n;
	int m_k;
	int m_c[MAX_COMB_ARRAY];
	int m_j;
	int m_i;
	int m_x;
	int m_array_size;

public:

	BinaryCombinator( int n, int k );
	void Visit( int varr[] );
	int Step();
};


#endif