
#ifndef _ioHashString_h_
#define _ioHashString_h_

#define HASH_CONST	5381
#define DEFAULT_CAPACITY	32

// Original version is www.gamza.net

class __EX ioHashString
{
private:
	char	*m_pString;
	DWORD	m_dwHashCode;
	int		m_iLength;
	int		m_iCapacity;

private:
	void ReAllocCapacity( int iNewCapacity );

public:
	char At( int i ) const;
	void Clear();

public:
	inline bool IsEmpty() const { return ( m_iLength == 0 ); }
	inline int  Length() const { return m_iLength; }
	inline DWORD GetHashCode() const { return m_dwHashCode; }
	inline const char* c_str() const { return m_pString; }

public:
	static DWORD CalcHashCode( const char *str );

public:
	inline bool operator<( const ioHashString &rhs ) const
	{
		if( m_dwHashCode != rhs.m_dwHashCode )
			return ( m_dwHashCode < rhs.m_dwHashCode );

		return ( strcmp( m_pString, rhs.m_pString ) < 0 );
	}

	ioHashString& operator=( const ioHashString &rhs );
	ioHashString& operator=( const char *szText );

	inline bool operator==( const ioHashString &rhs ) const
	{
		if( m_dwHashCode != rhs.m_dwHashCode )
			return false;

		if( strcmp( m_pString, rhs.m_pString ) != 0 )
			return false;

		return true;
	}

	inline bool operator==( const char *szText ) const
	{
		return ( strcmp( m_pString, szText ) == 0 );
	}

	inline bool operator!=( const ioHashString &rhs ) const
	{
		if( *this == rhs )
			return false;

		return true;
	}

	inline bool operator!=( const char *szText ) const
	{
		if( *this == szText )
			return false;

		return true;
	}

public:
	ioHashString();
	ioHashString( const char *str );
	ioHashString( const ioHashString &rhs );
	~ioHashString();
};

#endif
