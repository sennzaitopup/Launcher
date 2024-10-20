#include "../stdafx.h"
#include "iolocalkorea.h"
#include "iolocalus.h"
#include "iolocaltaiwan.h"
#include "ioLocalGermany.h"
#include "ioLocalIndonesia.h"
#include ".\iolocalmanager.h"
#include "ioLocalJapan.h"
#include "ioLocalThailand.h"
#include "ioLocalChina.h"
#include "ioLocalEU.h"
#include "ioLocalLatin.h"
#include "ioLocalSingapore.h"

template<> ioLocalManager* Singleton< ioLocalManager >::ms_Singleton = 0;
ioLocalManager::LocalType ioLocalManager::m_eLocalType = ioLocalManager::LCT_KOREA;


ioLocalManager::ioLocalManager(void)
{
	m_vLocalVector.reserve( 10 );
}

ioLocalManager::~ioLocalManager(void)
{
	for(vLocalVector::iterator iter = m_vLocalVector.begin(); iter != m_vLocalVector.end(); ++iter)
	{
		delete *iter;
	}
	m_vLocalVector.clear();
}


ioLocalParent * ioLocalManager::CreateLocal( LocalType eLocalType )
{
	ioLocalParent *pLocal = NULL;
	if( eLocalType == LCT_KOREA )
		pLocal = new ioLocalKorea;
	else if( eLocalType == LCT_US )
		pLocal = new ioLocalUS;
	else if( eLocalType == LCT_TAIWAN )
		pLocal = new ioLocalTaiwan;
	else if( eLocalType == LCT_GERMANY )
		pLocal = new ioLocalGermany;
	else if( eLocalType == LCT_INDONESIA )
		pLocal = new ioLocalIndonesia;
	else if( eLocalType == LCT_JAPAN )
		pLocal = new ioLocalJapan;
	else if( eLocalType == LCT_THAILAND )
		pLocal = new ioLocalThailand;
	else if( eLocalType == LCT_CHINA )
		pLocal = new ioLocalChina;
	else if( eLocalType == LCT_EU )
		pLocal = new ioLocalEU;
	else if( eLocalType == LCT_LATIN )
		pLocal = new ioLocalLatin;
	else if( eLocalType == LCT_SINGAPORE )
		pLocal = new ioLocalSingapore;

	// 새로운 국가 추가
	//else
	//	LOG.PrintTimeAndLog( 0, "%s Type is wrong. %s", __FUNCTION__, GetLocalName( eLocalType ) );

	return pLocal;
}

void ioLocalManager::AddLocal( ioLocalParent *pLocal )
{
	if( !pLocal )
	{
	//	LOG.PrintTimeAndLog( 0, "%s pLocal == NULL." , __FUNCTION__ );
		return;
	}

	if( GetLocal( pLocal->GetType() ) )
	{
	//	LOG.PrintTimeAndLog( 0, "%s Type is existing.%s" , __FUNCTION__ , GetLocalName( pLocal->GetType() ) );
		return;
	}

	m_vLocalVector.push_back( pLocal );
}

void ioLocalManager::Init()
{
	AddLocal( CreateLocal( LCT_KOREA ) );
	AddLocal( CreateLocal( LCT_US ) );
	AddLocal( CreateLocal( LCT_TAIWAN ) );
	AddLocal( CreateLocal( LCT_INDONESIA ) );
	AddLocal( CreateLocal( LCT_JAPAN ) );
	AddLocal( CreateLocal( LCT_THAILAND ) );
	AddLocal( CreateLocal( LCT_CHINA ) );
	AddLocal( CreateLocal( LCT_EU ) );
	AddLocal( CreateLocal( LCT_LATIN ) );
	AddLocal( CreateLocal( LCT_SINGAPORE ));
	
	// 새로운 국가 추가
}

ioLocalParent * ioLocalManager::GetLocal( LocalType eLocalType )
{
	for(vLocalVector::iterator iter = m_vLocalVector.begin(); iter != m_vLocalVector.end(); ++iter)
	{
		ioLocalParent *pLocal = *iter;
		if( !pLocal )
			continue;
		if( pLocal->GetType() == eLocalType )
			return pLocal;
	}

	return NULL;
}

ioLocalManager &ioLocalManager::GetSingleton()
{
	return Singleton< ioLocalManager >::GetSingleton();
}

void ioLocalManager::SetLocalType( const char *szLocal )
{
	if( strcmp( szLocal , "KOREA" ) == 0 )
		m_eLocalType = LCT_KOREA;
	else if( strcmp( szLocal , "US" ) == 0 )
		m_eLocalType = LCT_US;
	else if( strcmp( szLocal , "TAIWAN" ) == 0 )
		m_eLocalType = LCT_TAIWAN;
	else if( strcmp( szLocal , "GERMANY" ) == 0 )
		m_eLocalType = LCT_GERMANY;
	else if( strcmp( szLocal , "INDONESIA" ) == 0 )
		m_eLocalType = LCT_INDONESIA;
	else if( strcmp( szLocal , "JAPAN" ) == 0 )
		m_eLocalType = LCT_JAPAN;
	else if( strcmp( szLocal , "THAILAND" ) == 0 )
		m_eLocalType = LCT_THAILAND;
	else if( strcmp( szLocal , "CHINA" ) == 0 )
		m_eLocalType = LCT_CHINA;
	else if( strcmp( szLocal , "EU" ) == 0 )
		m_eLocalType = LCT_EU;
	else if( strcmp( szLocal , "LATIN" ) == 0 )
		m_eLocalType = LCT_LATIN;
	else if( strcmp( szLocal , "SEA" ) == 0 )
		m_eLocalType = LCT_SINGAPORE;
}

const char * ioLocalManager::GetLocalName( LocalType eLocalType )
{
	if(  m_eLocalType == LCT_KOREA )
		return "KOREA";
	else if( m_eLocalType == LCT_US )
		return "US";
	else if( m_eLocalType == LCT_TAIWAN )
		return "TAIWAN";
	else if( m_eLocalType == LCT_GERMANY )
		return "GERMANY";
	else if( m_eLocalType == LCT_INDONESIA )
		return "INDONESIA";
	else if( m_eLocalType == LCT_JAPAN )
		return "JAPAN";
	else if( m_eLocalType == LCT_THAILAND )
		return "THAILAND";
	else if( m_eLocalType == LCT_CHINA )
		return "CHINA";
	else if( m_eLocalType == LCT_EU )
		return "EU";
	else if( m_eLocalType == LCT_SINGAPORE )
		return "SEA";
	else
		return "UNKNOWN";
}

bool ioLocalManager::ParseCmdForSetLocal( const char *szCmd )
{
	// ? 구분자 : , 확장 구분자
	int iSize    = strlen( szCmd );
	int iMarkCnt = 0;
	int iExpandMarkCnt = 0;
	char szLocalType[MAX_PATH]="";
	int  iLocalTypeCnt = 0;
	enum { MAX_MARK = 9, MAX_EXPAND_MARK = 9 };
	for (int i = 0; i < iSize ; i++)
	{
		if( szCmd[i] == '?' )
		{
			iMarkCnt++;
			continue;
		}

		if( iMarkCnt == MAX_MARK )
		{
			if( szCmd[i] == ',' )
			{
				iExpandMarkCnt++;
				continue;
			}

			if( iExpandMarkCnt == MAX_EXPAND_MARK )
			{
				szLocalType[iLocalTypeCnt] = szCmd[i];
				iLocalTypeCnt++;
				if( iLocalTypeCnt >= MAX_PATH )
					break;
			}
		}
		else if( iMarkCnt > MAX_MARK )
		{
			break;
		}
	}

	int iLocalType = atoi( szLocalType );

	if( iLocalType != LCT_KOREA &&
		iLocalType != LCT_US    &&
		iLocalType != LCT_TAIWAN&&
		iLocalType != LCT_JAPAN &&
		iLocalType != LCT_MALAYSIA &&
		iLocalType != LCT_INDONESIA&&
		iLocalType != LCT_VIETNAM &&
		iLocalType != LCT_GERMANY &&
		iLocalType != LCT_EU    &&
		iLocalType != LCT_SINGAPORE    &&
		iLocalType != LCT_LATIN    &&
		iLocalType != LCT_FRANCE )
	{
		return false;
	}

	m_eLocalType = (ioLocalManager::LocalType) iLocalType;
	return true;
}