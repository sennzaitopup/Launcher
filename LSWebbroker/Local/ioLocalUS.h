#ifndef __ioLocalUS_h__
#define __ioLocalUS_h__

#include "ioLocalParent.h"

#define OGP_TOKEN          " "
#define OGP_VALUE_TOKEN    "="
#define SGW_VALUE_TOKEN		":"

#ifdef STEAM_ATTACH
#include "../SetupAndStart.h"
#endif

class ioLocalUS : public ioLocalParent
{
protected:
	char m_szRegKey[MAX_PATH];
	char m_szStartURL[MAX_PATH*2];

public:
	virtual ioLocalManager::LocalType GetType();
	virtual const char *GetTextListFileName();
	virtual const char *GetMemTextList();

	virtual bool GetNewCmdLine( IN const char *szCmd, OUT char *szNewCmd, IN int iNewCmdSize );
	virtual bool IsRightGameServerID( const CString &rszGameServerID );

	virtual const char *GetRegKey();

	virtual DWORD GetResourceIDBack();
	virtual DWORD GetResourceIDChange();
	virtual DWORD GetResourceIDSetup();

	virtual DWORD GetResourceIDErrorBack();
	virtual DWORD GetResourceIDErrorExit();
	virtual DWORD GetResourceIDErrorSolution();

	virtual const char *GetErrorSolutionURL();
	virtual void  SetRegKey( const char *szReg );
	virtual void  SetStartURL( const char *szURL );

	virtual bool  IsLoginDlg();
	virtual bool GetLoginResultPostData( OUT char *szError, IN int iErrorSize, const char *szURL, const char *szID, const char *szPW );
	virtual const char *GetFontName();

#ifdef STEAM_ATTACH
	bool CheckFileInFolder(CString* const strPath, CString* const strFile );
	bool UpdateNRunSteamLauncher(CSetupAndStart* SetUpAndStart);
#endif
public:
	ioLocalUS(void);
	virtual ~ioLocalUS(void);
};

#endif // __ioLocalUS_h__