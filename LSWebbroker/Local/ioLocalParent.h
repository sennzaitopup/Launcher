#ifndef __ioLocalParent_h__
#define __ioLocalParent_h__

#include "ioLocalManager.h"

#define START_TOKEN        "?"
#define START_EXPAND_TOKEN ","

class ioLocalParent
{
protected:
	bool Connect( const char *szIP, int iPort );
	void GetIPPort( IN const char *szIPPortNum , OUT char *szIP, IN int iIPSize, OUT int &riPort );
	bool ReadURLData( IN const char *szURL, OUT char *szNewCmd, IN int iNewCmdSize, OUT char *szStartData, IN int iStartDataSize );
	void ParseServerIDList( IN char* szServerIDList, OUT char *szServerID, IN int iSize );
	bool SetRandomURL( IN int iMaxFileNameCnt, IN const char *szDefaultURL, OUT char *szNewURL, IN int iNewURLSize );

public:
	virtual ioLocalManager::LocalType GetType() = 0;
	virtual const char *GetTextListFileName() = 0;
	virtual const char *GetMemTextList() = 0;

	virtual bool GetNewCmdLine( IN const char *szCmd, OUT char *szNewCmd, IN int iNewCmdSize ) = 0;
	virtual bool IsRightGameServerID( const CString &rszGameServerID ) = 0;

	virtual const char *GetRegKey() = 0;

	virtual DWORD GetResourceIDBack() = 0;
	virtual DWORD GetResourceIDChange() = 0;
	virtual DWORD GetResourceIDSetup() = 0;

	virtual DWORD GetResourceIDErrorBack() = 0;
	virtual DWORD GetResourceIDErrorExit() = 0;
	virtual DWORD GetResourceIDErrorSolution() = 0;

	virtual const char *GetErrorSolutionURL() = 0;
	virtual void  SetRegKey( const char *szReg ) = 0;
	virtual void  SetStartURL( const char *szURL ) = 0;

	virtual bool  IsLoginDlg() = 0;
	virtual bool  IsReadRegClientPath() { return true; }
	virtual bool  GetLoginResultPostData( OUT char *szError, IN int iErrorSize, const char *szURL, const char *szID, const char *szPW ) = 0;

	virtual bool InitWinSock() { return true; }
	virtual void CloseWinSock() {}
	virtual const char *GetFontName() = 0;
		
public:
	ioLocalParent(void);
	virtual ~ioLocalParent(void);
};

#endif // __ioLocalParent_h__
