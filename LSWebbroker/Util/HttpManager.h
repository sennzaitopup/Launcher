#ifndef __HttpManager_h__
#define __HttpManager_h__

#include "../StringManager/ioSingleton.h"

#define HTTP_AGENT "LOSTSAGA_HTTP_MANAGER_TGFEG" // TGEFG 유니크값

#pragma comment(lib, "Wininet.lib")

class HttpManager : public Singleton< HttpManager >
{
public:
	static HttpManager &GetSingleton();

public:
	bool GetResultGetData( IN const char *szURL, OUT char *szReturnData, IN int iReturnDataSize );
	bool GetResultPostData( IN const char *szURL, IN const char *szPostData, OUT char *szReturnData, IN int iReturnSize, IN bool bHttps = true );

public:
	HttpManager(void);
	virtual ~HttpManager(void);
};

#define g_HttpMgr HttpManager::GetSingleton()

#endif // __HttpManager_h__