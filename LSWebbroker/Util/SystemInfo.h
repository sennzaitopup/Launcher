#pragma once

class CSystemInfo
{
protected:
	enum
	{
		MEGA = 1048576,
	};
protected:
	CString m_szSystemInfo;
	
protected:
	void SetMemoryInfo();
	void SetOS();
	void SetCPU();
	void SetHDD();

public:
	void SetupSystemInfo();
	CString &GetSystemInfo() { return m_szSystemInfo; }
public:
	CSystemInfo(void);
	virtual ~CSystemInfo(void);
};
