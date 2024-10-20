#ifndef __TimeOutSocket_h__
#define __TimeOutSocket_h__

#include "afxsock.h"

class CTimeOutSocket : 	public CSocket
{
private:
	int m_iTimerID;

protected:
	virtual BOOL OnMessagePending();

public:
	BOOL SetTimeOut(UINT uTimeOut);
	BOOL KillTimeOut();

public:
	CTimeOutSocket(void);
	virtual ~CTimeOutSocket(void);
};

#endif // __TimeOutSocket_h__