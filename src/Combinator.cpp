#include "Combinator.h"
#include <exception>

//=============================================================================
// Constructor 1
//=============================================================================
BinaryCombinator::BinaryCombinator( int n, int k )
{
	if (k > n)
	{
		exception* e = new exception("k is greater than n!");
		throw e;
	}

	if( k > MAX_COMB_ARRAY + 2 )
	{
		exception* e = new exception("k is too big!");
		throw e;
	}

	m_n = n;
	m_k = k;
	m_j = 1;

	// init array
	for( int i = 0; i < MAX_COMB_ARRAY; i++ )
	{
		m_c[i] = -1;
	}

	m_array_size = k + 3;
	for (m_i = 1; m_i <= m_k; m_i++)
	{
		m_c[m_i] = m_i;
	}
	
	m_c[m_k + 1] = m_n + 1;
	m_c[m_k + 2] = 0;
	m_j = m_k;
}

//=============================================================================
// Visit
//=============================================================================
void BinaryCombinator::Visit( int varr[] )
{
	int i = 0;
	for( m_i = m_k; m_i >= 1; m_i-- )
	{
		varr[i++] = m_c[m_i];
	}
}

//=============================================================================
// Step
//=============================================================================
int BinaryCombinator::Step( void )
{
	// Visit

	if( m_j > m_k )
	{
		return 0;
	}

    if (m_j > 0) 
    {
        m_x = m_j+1; 
        goto incr;
    }

    if (m_c[1] + 1 < m_c[2])
    {
        m_c[1] += 1;
        return 1; //goto visit;
    }
    m_j = 2;

do_more:
 
	m_c[m_j - 1] = m_j - 1;
    m_x = m_c[m_j] + 1;
    if (m_x == m_c[m_j + 1]) 
    {
        m_j++; goto do_more;
    }

    if (m_j > m_k)
    {
        return 0;
    }
incr:
    m_c[m_j] = m_x;
    m_j--;
    return 1; //goto visit;
}        
