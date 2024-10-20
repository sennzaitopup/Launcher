#include ".\timeoutsocket.h"

CTimeOutSocket::CTimeOutSocket(void)
{
	m_iTimerID = 0;
}

CTimeOutSocket::~CTimeOutSocket(void)
{
}

BOOL CTimeOutSocket::OnMessagePending()
{
	MSG msg;
	if(::PeekMessage(&msg, NULL, WM_TIMER, WM_TIMER, PM_NOREMOVE))
	{
		if (msg.wParam == (UINT) m_iTimerID)
		{
			// Remove the message and call CancelBlockingCall.
			::PeekMessage(&msg, NULL, WM_TIMER, WM_TIMER, PM_REMOVE);
			CancelBlockingCall();
			return FALSE;  // No need for idle time processing.
		};
	};

	return CSocket::OnMessagePending();
}

BOOL CTimeOutSocket::SetTimeOut(UINT uTimeOut)
{
	m_iTimerID = (int)SetTimer(NULL,0,uTimeOut,NULL);
	return m_iTimerID;
}

BOOL CTimeOutSocket::KillTimeOut()
{
	return KillTimer(NULL,m_iTimerID);
}
