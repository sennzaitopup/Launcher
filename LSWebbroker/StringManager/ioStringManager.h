#ifndef __ioStringManager_h__
#define __ioStringManager_h__

#define STRING_MANAGER
#define TEXT_PIECE_LOAD // For Autoupgrade

#include "ioSingleton.h"
#include "ioHashString.h"

#ifdef TEXT_PIECE_LOAD
	#include <map>
#endif

#ifdef EXTRACT_HANGEUL_TOOL
	#include <atlstr.h>
#endif

class __EX ioStringManager : public Singleton< ioStringManager >
{
public:
	enum FileNamePrefix
	{
		FNP_EXE = 0,
		FNP_XML = 1,
		FNP_INI = 2,
	};

protected:

#ifdef EXTRACT_HANGEUL_TOOL
	struct KeyNameSort // 맵 디폴트로 정렬하지 않고 알파벳 순서로 정렬
	{ 
		bool operator()(const ioHashString& lhs, const ioHashString& rhs) const 
		{ 
			// 9 10 11 순으로 정렬하기 위해서 09로 만듬
			CString slhs = lhs.c_str();
			slhs.MakeReverse();
			for (int i = 0; i < 20 ; i++)
			{
				if( slhs.GetAt(i) == '_' )
				{
					if( i < 7 ) // 1000000
					{
						for (int i2 = 0; i2+i < 7  ; i2++)
						{
							slhs.Insert(i, "0" );	
						}
					}
				}
			}
			slhs.MakeReverse();

			CString rlhs = rhs.c_str();
			rlhs.MakeReverse();
			for (int i = 0; i < 20 ; i++)
			{
				if( rlhs.GetAt(i) == '_' )
				{
					if( i < 7 ) // 1000000
					{
						for (int i2 = 0; i2+i < 7  ; i2++)
						{
							rlhs.Insert(i, "0" );	
						}
					}
				}
			}
			rlhs.MakeReverse();

			int iCmpValue = stricmp( (LPCTSTR)slhs, (LPCTSTR)rlhs );     
			if( iCmpValue < 0 )
				return true;
			return false;
		} 
	}; 
	typedef std::map< ioHashString, ioHashString, KeyNameSort > StringInfoMap;
#else
	typedef std::map< ioHashString, ioHashString > StringInfoMap;
#endif

protected:
	StringInfoMap        m_StringInfoMap;
	static ioHashString  m_szTextPieceFileName;

protected:
	int GetStringMgrTextNum( const char *szText );

public:
	void LoadData( const char *szPath, const char *szFileName, const char *szMemTextList, bool bLoadMemText = false );

public:
	char *GetString( const char *szFileName, int iNum, FileNamePrefix eFileNamePrefix );
	const char *GetStringXML( const char *szKey, const char *szText );
	void GetStringINI( IN const char *szKey, OUT char *szText, IN int iTextSize );

#ifndef EXTRACT_HANGEUL_TOOL
	void WrtieTextList( const char *szFileName );
	bool InsertTextList( const char *szFunc, int iFuncNum, const char *szText );
#endif

public:
	static ioStringManager &GetSingleton();

public:
	ioStringManager(void);
	virtual ~ioStringManager(void);
};

#define g_StringMgr ioStringManager::GetSingleton()
#define STR( iNum ) g_StringMgr.GetString( __FUNCTION__, iNum, ioStringManager::FNP_EXE )

#endif // __ioStringManager_h__

