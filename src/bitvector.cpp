//=============================================================================
//    Bitvector class implementation
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
#include "bitvector.h"
#include "errors.h"
#include <bitset>
#include <iostream>
#include <limits.h>
#include <exception>
#include <string.h>

//=============================================================================
// Constructor(1)
//=============================================================================
Bitvector::Bitvector( unsigned int size )
{
	this->m_numbits   = size;
	this->m_numbytes = m_numbits/BV_BYTE_LEN;

	if( (m_numbits % BV_BYTE_LEN) != 0 ) {
		this->m_numbytes += 1;		
	}

	this->m_bits = new unsigned char [m_numbytes];
	
	if( m_bits == NULL ) {
		throw GSERR_OUT_OF_MEMORY;
	}

	Clear();
}

//=============================================================================
// Constructor(2)
//=============================================================================
Bitvector::Bitvector( const Bitvector &b )
{
	this->m_numbits = b.m_numbits;
	this->m_numbytes = b.m_numbytes;
	this->m_bits = new unsigned char [m_numbytes];

	if( m_bits == NULL ) {
		throw GSERR_OUT_OF_MEMORY;
	}

	for( unsigned int i=0; i<m_numbytes; i++ ) {
		m_bits[i] = b.m_bits[i];
	}
}

//=============================================================================
// Constructor(3)
//=============================================================================
Bitvector::Bitvector( unsigned long num )
{
	unsigned char* pnum = (unsigned char*) &num;

	this->m_numbytes = sizeof(num);
	this->m_numbits  = m_numbytes * BV_BYTE_LEN;
	this->m_bits     = new unsigned char [m_numbytes];
	if( m_bits == NULL )
	{
		throw GSERR_OUT_OF_MEMORY;
	}

	for( unsigned int i=0; i<m_numbytes; i++ )
	{
		this->m_bits[i] = pnum[i];
	}
}

//=============================================================================
// Constructor(4)
//=============================================================================
Bitvector::Bitvector( unsigned char* bits, unsigned int size )
{
	this->m_numbytes = size;
	this->m_numbits = size * BV_BYTE_LEN;
	this->m_bits = new unsigned char [m_numbytes];
	if( m_bits == NULL )
	{
		throw GSERR_OUT_OF_MEMORY;
	}
	for( unsigned int i=0; i<m_numbytes; i++ )
	{
		this->m_bits[i] = bits[i];
	}
}

//=============================================================================
// Constructor(5)
//=============================================================================
Bitvector::Bitvector( unsigned int *bits, unsigned int size )
{
	this->m_numbits = size;
	this->m_numbytes = m_numbits/BV_BYTE_LEN;

	if( (m_numbits % BV_BYTE_LEN) != 0 )
	{
		this->m_numbytes += 1;		
	}
	this->m_bits = new unsigned char [m_numbytes];
	if( m_bits == NULL )
	{
		throw GSERR_OUT_OF_MEMORY;
	}

	Clear();

	unsigned int i    = 0;
	unsigned int byte = 0;

	while ( i < size && byte < m_numbytes )
	{
		for( unsigned int j=0; i<BV_BYTE_LEN; j++ )
		{
			if( bits[i+j] )
			{
				this->m_bits[byte] |= (0x1 << j);
			}
		}
		byte++;
		i +=BV_BYTE_LEN;
	}
}

//=============================================================================
// Destructor
//=============================================================================
Bitvector::~Bitvector( void )
{
	delete [] m_bits;
}

//=============================================================================
// Clear - Sets all bits to 0
//=============================================================================
void Bitvector::Clear( void )
{
	for( unsigned int i=0; i<m_numbytes; i++ )
	{
		this->m_bits[i] = 0x00;
	}
}

//=============================================================================
// Set(1) - Sets the bit at specified position to value
//=============================================================================
void Bitvector::Set( unsigned int pos, bool value )
{
	if( pos >= m_numbits )
	{
		throw GSERR_OUT_OF_RANGE;
	}
	if( value )
	{
		this->m_bits[pos/BV_BYTE_LEN] |= (0x01 << (pos % BV_BYTE_LEN));
	}
	else
	{
		this->m_bits[pos/BV_BYTE_LEN] &= ~(0x01 << (pos % BV_BYTE_LEN));
	}
}

//=============================================================================
// Test - tests if the bit at the given position is set
//=============================================================================
bool Bitvector::Test( unsigned int pos ) const
{
	if( pos >= m_numbits )
	{
		throw GSERR_OUT_OF_RANGE;
	}
	
	return (m_bits[pos/BV_BYTE_LEN] & (0x01 << (pos % BV_BYTE_LEN))) != 0;
}

//=============================================================================
// Expand - Increases the size of the bitvector by amount of bits
//=============================================================================
void Bitvector::Expand( unsigned int amount )
{
	if( amount == 0 )
	{
		return;
	}

	// If amount to expand is less than the remaining free bits in the last byte
	if( (amount + (m_numbits % BV_BYTE_LEN)) <= BV_BYTE_LEN)  
	{
		// Set new bits to zero
		this->m_bits[m_numbytes-1] &= (0xFF >> (BV_BYTE_LEN - (m_numbits % BV_BYTE_LEN)));
	}
	else
	{
		unsigned int freebits = BV_BYTE_LEN - (m_numbits % BV_BYTE_LEN);
		unsigned int newbytes = (amount-freebits)/BV_BYTE_LEN + ((amount-freebits)%BV_BYTE_LEN != 0);

		if( newbytes + m_numbytes > UINT_MAX )
		{
			throw GSERR_OVER_INDEX_LIMIT;  // Can't access elements larger than max UINT
		}

		this->m_bits[m_numbytes-1] &= (0xFF >> freebits);
		unsigned char* newbits = new unsigned char [m_numbytes+newbytes];
		memcpy( newbits, m_bits, m_numbytes );
		memset( &newbits[m_numbytes], 0x00, newbytes );

		delete [] this->m_bits;
		this->m_bits = newbits;
		this->m_numbytes += newbytes;
		this->m_numbits  += amount;		
	}
}

//=============================================================================
// Contract - Decreases the size of the bitvector by the amount of bits
//=============================================================================
void Bitvector::Contract( unsigned int amount )
{
	if( m_numbits - amount > 0 )
	{
		size_t oldbits  = m_numbits;
		size_t oldbytes = m_numbytes;

		m_numbits -= amount;
		m_numbytes = m_numbits/BV_BYTE_LEN + ((m_numbits%BV_BYTE_LEN) != 0);
		unsigned char* newbits = new unsigned char [m_numbytes];
		memcpy( newbits, m_bits, m_numbytes );
		newbits[m_numbytes-1] &= (0xFF >> (BV_BYTE_LEN - (m_numbits % BV_BYTE_LEN)));
		delete [] this->m_bits;
		this->m_bits = newbits;
	}
}

//=============================================================================
// Toggle - Toggles/flips the bit at the specified position
//=============================================================================
void Bitvector::Toggle( unsigned int pos )
{
	if ( pos >= m_numbits )
	{
		throw GSERR_OUT_OF_RANGE;
	}
	m_bits[pos] ^= (0x01 << (pos % BV_BYTE_LEN));
}

//=============================================================================
// Size - Returns the size in bits of the bitvector
//=============================================================================
unsigned int Bitvector::Size( void )
{
	return this->m_numbits;
}

//=============================================================================
// Bytes - Returns the number of bytes used by the bitvector
//=============================================================================
unsigned int Bitvector::Bytes( void )
{
	return this->m_numbytes;
}

//=============================================================================
// Count - Returns the number of bits that are set
//=============================================================================
unsigned int Bitvector::Count( void ) const
{
	static char BitsPerHex[] = {0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4};
	unsigned int cnt;

	cnt = 0;
	for( unsigned int i=0; i<this->m_numbytes; ++i )		// Assumes extra bits in last byte are 0!!!!
	{
		for( unsigned char j = m_bits[i]; j != 0; j>>=4 )
		{
			cnt += BitsPerHex[j & 0xF];
		}
	}
	return cnt;
}

//=============================================================================
// Print - Temp code to print
//=============================================================================
void Bitvector::Print( void )
{
	for( unsigned int i=0; i<m_numbits; i++ )
	{
		std::cout << Test(i);
	}
	std::cout << std::endl;
}

//=============================================================================
// Operator [] Overload(1)
//=============================================================================
bool Bitvector::operator []( unsigned int index ) const
{
	return ( Test( index ) );
}

//=============================================================================
// Operator [] Overload(2)
//=============================================================================
Bitvector::reference Bitvector::operator []( unsigned int index )
{
	return reference( *this, index );
}
