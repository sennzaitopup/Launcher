
#include "../stdafx.h"
#include "ioHashString.h"
#include <assert.h>

ioHashString::ioHashString()
{
	m_iCapacity = DEFAULT_CAPACITY;
	m_pString = new char[ m_iCapacity ];

	Clear();

	m_dwHashCode = CalcHashCode( m_pString );
}

ioHashString::ioHashString( const char* str ) : m_pString(NULL)
{
	m_iLength = strlen( str );

	int iCapacity = max( m_iLength + 1, DEFAULT_CAPACITY );
	ReAllocCapacity( iCapacity );

	strcpy( m_pString, str );
	m_dwHashCode = CalcHashCode( str );
}

ioHashString::ioHashString( const ioHashString &rhs ) : m_pString(NULL)
{
	m_iLength = rhs.Length();

	ReAllocCapacity( rhs.m_iCapacity );

	strcpy( m_pString, rhs.m_pString );
	m_dwHashCode = rhs.m_dwHashCode;
}

ioHashString::~ioHashString()
{
	SAFEDELETEARRAY( m_pString );
}

void ioHashString::Clear()
{
	memset( m_pString, 0, m_iCapacity );
	m_iLength = 0;
}

void ioHashString::ReAllocCapacity( int iNewCapacity )
{
	SAFEDELETEARRAY( m_pString );

	m_iCapacity = iNewCapacity;
	m_pString = new char[iNewCapacity];
}

DWORD ioHashString::CalcHashCode( const char *str )
{
	DWORD dwLen, dwCh, dwResult;

	dwLen    = strlen( str );
	dwResult = HASH_CONST;

	for( DWORD i=0; i<dwLen ; i++ )
	{
		dwCh     = (DWORD)str[i];
		dwResult = ((dwResult<< 5) + dwResult) + dwCh; // hash * 33 + ch
	}

	return dwResult;
}

char ioHashString::At( int i ) const
{
	if( m_iLength < i || !m_pString )
	{
		assert( false );
		return 0;
	}

	return m_pString[i];
}

ioHashString& ioHashString::operator=( const ioHashString &rhs )
{
	m_iLength = rhs.Length();
	if( m_iCapacity < m_iLength + 1 )
	{
		ReAllocCapacity( m_iLength + 1 );
	}

	strcpy(m_pString, rhs.m_pString );
	m_dwHashCode = rhs.m_dwHashCode;

	return *this;
}

ioHashString& ioHashString::operator=( const char *szText )
{
	m_iLength = strlen( szText );
	if( m_iCapacity < m_iLength + 1 )
	{
		ReAllocCapacity( m_iLength + 1 );
	}

	strcpy( m_pString, szText );
	m_dwHashCode = CalcHashCode( szText );

	return *this;
}
