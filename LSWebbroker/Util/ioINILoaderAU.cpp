

#include "../stdafx.h"

#include "ioINILoaderAU.h"
#include <strsafe.h>

ioINILoaderAU::ioINILoaderAU()
{
	ZeroMemory( m_szFileName, MAX_PATH );
	ZeroMemory( m_szTitle, MAX_PATH );

// 	m_pParser = NULL;
}

ioINILoaderAU::ioINILoaderAU( const char *szFileName )
{
	StringCbCopy( m_szFileName, sizeof(m_szFileName), szFileName );
	ZeroMemory( m_szTitle, MAX_PATH );

// 	m_pParser = NULL;
}

ioINILoaderAU::ioINILoaderAU( const char *szPath, const char *szFileName )
{
	StringCbPrintf( m_szFileName, sizeof(m_szFileName), "%s/%s", szPath, szFileName );

// 	m_pParser = NULL;
}

ioINILoaderAU::~ioINILoaderAU()
{
// 	SAFEDELETE( m_pParser );
}

void ioINILoaderAU::SetFileName( const char *szFileName )
{
	StringCbCopy( m_szFileName, sizeof(m_szFileName), szFileName );

// 	SAFEDELETE( m_pParser );
}

void ioINILoaderAU::SetTitle( const char *szTitle )
{
	StringCbCopy( m_szTitle, sizeof(m_szTitle), szTitle );
}

bool ioINILoaderAU::DoMemoryParsing()
{
// 	SAFEDELETE( m_pParser );
// 
// 	m_pParser = new ioINIParser;
// 	if( m_pParser->ParsingFile( m_szFileName ) )
// 		return true;
// 
// 	SAFEDELETE( m_pParser );

	return false;
}

bool ioINILoaderAU::LoadBool( const char *szTitle, const char *szKeyName, bool bDefault )
{
	int iValue = 0;
	if( bDefault )
	{
		iValue = 1;
	}

// 	if( m_pParser )
// 	{
// 		const char *szValue = m_pParser->GetValue( szTitle, szKeyName );
// 		if( szValue )
// 		{
// 			iValue = atoi( szValue );
// 		}
// 	}
// 	else
	{
		iValue = GetPrivateProfileInt( szTitle, szKeyName, iValue, m_szFileName );
	}

	if( iValue != 0 )
		return true;

	return false;

}

int ioINILoaderAU::LoadInt( const char *szTitle, const char *szKeyName, int iDefault )
{
	int iValue = iDefault;
// 	if( m_pParser )
// 	{
// 		const char *szValue = m_pParser->GetValue( szTitle, szKeyName );
// 		if( szValue )
// 		{
// 			iValue = atoi( szValue );
// 		}
// 	}
// 	else
	{
		iValue = GetPrivateProfileInt( szTitle, szKeyName, iDefault, m_szFileName );
	}

	return iValue;
}

float ioINILoaderAU::LoadFloat( const char *szTitle, const char *szKeyName, float fDefault )
{
	float fValue = fDefault;
	
// 	if( m_pParser )
// 	{
// 		const char *szValue = m_pParser->GetValue( szTitle, szKeyName );
// 		if( szValue )
// 		{
// 			fValue = atof( szValue );
// 		}
// 	}
// 	else
	{
		char szBuf[MAX_PATH];
		GetPrivateProfileString( szTitle, szKeyName, "", szBuf, MAX_PATH, m_szFileName );
		if( strcmp( szBuf, "" ) )
		{
			fValue = (float)atof( szBuf );
		}
	}

	return fValue;
}

// void ioINILoaderAU::LoadVector( const char *szTitle, D3DXVECTOR3 *pVec )
// {
// 	pVec->x = LoadFloat( szTitle, "X", 0.0f );
// 	pVec->x = LoadFloat( szTitle, "Y", 0.0f );
// 	pVec->x = LoadFloat( szTitle, "Z", 0.0f );
// }

void ioINILoaderAU::LoadString( const char *szTitle,
							  const char *szKeyName,
							  const char *szDefault,
							  char *szBuf,
							  int iBufLen )
{
// 	if( m_pParser )
// 	{
// 		const char *szValue = m_pParser->GetValue( szTitle, szKeyName );
// 		if( szValue )
// 		{
// 			strncpy( szBuf, szValue, iBufLen );
// 		}
// 		else
// 		{
// 			strncpy( szBuf, szDefault, iBufLen );
// 		}
// 	}
// 	else
	{
		GetPrivateProfileString( szTitle, szKeyName, szDefault, szBuf, iBufLen, m_szFileName );
	}
}

bool ioINILoaderAU::LoadBool( const char *szKeyName, bool bDefault )
{
	return LoadBool( m_szTitle, szKeyName, bDefault );
}

int ioINILoaderAU::LoadInt( const char *szKeyName, int iDefault )
{
	return LoadInt( m_szTitle, szKeyName, iDefault );
}

float ioINILoaderAU::LoadFloat( const char *szKeyName, float fDefault )
{
	return LoadFloat( m_szTitle, szKeyName, fDefault );
}

// void ioINILoaderAU::LoadVector( D3DXVECTOR3 *pVec )
// {
// 	LoadVector( m_szTitle, pVec );
// }

void ioINILoaderAU::LoadString( const char *szKeyName,
							  const char *szDefault,
							  char *szBuf,
							  int iBufLen )
{
	LoadString( m_szTitle, szKeyName, szDefault, szBuf, iBufLen );
}

void ioINILoaderAU::SaveString( const char *szTitle, const char *szKeyName, const char *szBuf )
{
	WritePrivateProfileString( szTitle, szKeyName, szBuf, m_szFileName );
}

void ioINILoaderAU::SaveInt( const char *szTitle, const char *szKeyName, int iValue )
{
	char szBuf[MAX_PATH];
	StringCbPrintf( szBuf, sizeof(szBuf), "%d", iValue );

	WritePrivateProfileString( szTitle, szKeyName, szBuf, m_szFileName );
}

void ioINILoaderAU::SaveFloat( const char *szTitle, const char *szKeyName, float fValue, bool bNoLimit )
{
	char szBuf[MAX_PATH];

	if( bNoLimit )
	{
		StringCbPrintf( szBuf, sizeof(szBuf), "%f", fValue );
	}
	else
	{
		StringCbPrintf( szBuf, sizeof(szBuf), "%.2f", fValue );
	}

	WritePrivateProfileString( szTitle, szKeyName, szBuf, m_szFileName );
}

void ioINILoaderAU::SaveBool( const char *szTitle, const char *szKeyName, bool bValue )
{
	if( bValue )
	{
		WritePrivateProfileString( szTitle, szKeyName, "1", m_szFileName );
	}
	else
	{
		WritePrivateProfileString( szTitle, szKeyName, "0", m_szFileName );
	}
}

// void ioINILoaderAU::SaveVector( const char *szTitle, const D3DXVECTOR3 &vVec )
// {
// 	char szBuf[MAX_PATH];
// 
// 	sprintf( szBuf, "%.2f", vVec.x );
// 	WritePrivateProfileString( szTitle, "X", szBuf, m_szFileName );
// 
// 	sprintf( szBuf, "%.2f", vVec.y );
// 	WritePrivateProfileString( szTitle, "Y", szBuf, m_szFileName );
// 
// 	sprintf( szBuf, "%.2f", vVec.z );
// 	WritePrivateProfileString( szTitle, "Z", szBuf, m_szFileName );
// }

void ioINILoaderAU::SaveString( const char *szKeyName, const char *szBuf )
{
	SaveString( m_szTitle, szKeyName, szBuf );
}

void ioINILoaderAU::SaveInt( const char *szKeyName, int iValue )
{
	SaveInt( m_szTitle, szKeyName, iValue );
}

void ioINILoaderAU::SaveFloat( const char *szKeyName, float fValue, bool bNoLimit )
{
	SaveFloat( m_szTitle, szKeyName, fValue, bNoLimit );
}

void ioINILoaderAU::SaveBool( const char *szKeyName, bool bValue )
{
	SaveBool( m_szTitle, szKeyName, bValue );
}

// void ioINILoaderAU::SaveVector( const D3DXVECTOR3 &vVec )
// {
// 	SaveVector( m_szTitle, vVec );
// }
