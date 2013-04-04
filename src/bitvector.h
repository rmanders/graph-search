//=============================================================================
//    Header for Bitvector class
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

#ifndef __BITVECTOR__
#define __BITVECTOR__

#include <iostream>

//=============================================================================
// Defines a bitvector class, slighty different functionality than bitset
//=============================================================================

#define BV_MAX_SIZE     25                      // Max bytes allowed in a bitset
#define BV_BYTE_LEN     8                       // Number of bits per byte
#define BV_MAX_BITS	BV_MAX_SIZE*BV_BYTE_LEN // Max number of bits allowed

#define BV_ERR_OUTOFRANGE       2 // Out of Range Error
#define BV_ERR_OUTOFMEMORY      3 // Out of Memory Error
#define BV_ERR_OVERINDEXLIMIT   4 // Bitvector too large Error

using namespace std;

class Bitvector
{
private:
	unsigned char* m_bits;     // Pointer to array of bytes
	unsigned int   m_numbytes; // The size (in bytes) of the m_bits array
	unsigned int   m_numbits; // The size in bits of the bitvector (defines offset in last byte)

public:

	// Internal reference class used so the 
	// subscript operator can access individial bits
	class reference
	{
		friend class Bitvector;
	public:
		reference& operator= ( bool value )
		{
			m_pBitvector->Set( m_index, value );
			return (*this);
		}

		reference& operator= ( const reference& ref )
		{
			m_pBitvector->Set( m_index, bool( ref ) );
			return (*this);
		}

		operator bool( void ) const
		{
			return m_pBitvector->Test( m_index );
		}	

		bool operator~( void ) const
		{
			return !(m_pBitvector->Test( m_index ));
		}

		reference& toggle( void )
		{
			m_pBitvector->Toggle( m_index );
			return (*this);
		}

	private:
		reference( Bitvector& pBitvector, unsigned int index ) 
			: m_pBitvector( &pBitvector ), m_index( index ) 
		{ 
			// Do Nothing
		}
		Bitvector*	 m_pBitvector;	
		unsigned int m_index;
	};

	Bitvector( unsigned int size );                      // Construct with sepcified num bits
	Bitvector( const Bitvector& b );                     // Copy Constructor
	Bitvector( unsigned long num );                      // Construct from unsigned integer
	Bitvector( unsigned char* bits, unsigned int size ); // Construct from unsigned char array
	Bitvector( unsigned int* bits, unsigned int size );  // Construct from int array of bits
	~Bitvector( void );                                  // Destructor

	void Clear( void );                                  // Sets all bits to 0
	void Set( unsigned int pos, bool value );            // Sets the specified bit to value
	bool Test( unsigned int pos ) const;                 // Returns true if bit is 1, false if 0
	void Expand( unsigned int amount );                  // Increase size of bitvector by amount
	void Contract( unsigned int amount );                // Decrease size of bitvector by amount
	void Toggle( unsigned int pos );                     // Toggle/flip the bit at specified position

	unsigned int Size( void );                           // Returns the size/number of bits in the vector
	unsigned int Bytes( void );                          // returns the number of bytes used
	unsigned int Count( void ) const;                    // Returns the number of bit that are set

	void Print( void );                                  // Prints bitvector contents to stdout

	bool operator[] ( unsigned int index ) const;        // Subscript operator overload. Access a bit
	reference operator[] (unsigned int index );          // Subscript operator overload. Access a bit
	Bitvector& operator= ( const Bitvector& b );         // Assignment operator overload
};

#endif
