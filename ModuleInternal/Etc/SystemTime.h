#pragma once

/********************************************************************
	created:	2014/10/02
	modefied:	07:10:2014   09:54
	author:		OSC
	
	purpose:	msec까지 관리하는 시간 Class
				SyncSystemTime() 호출시 표준시간대를 변경해도 같이 
					따라가도록 SYSTEMTIME으로 현재시간을 가져온다.
*********************************************************************/

class CSystemTime
{
public:
	CSystemTime(void);
	~CSystemTime(void);

private:
	CTime m_Time;
	WORD m_wMilliseconds;

public:
	int GetYear();
	int GetMonth();
	int GetDay();
	int GetHour();
	int GetMinute();
	int GetSecond();
	WORD GetMillisecond();

	CString Format(LPCTSTR pFormat);

	void SyncSystemTime();
	CTime ToCTime();
	static CTime GetSystemTime();

	void operator=(const CTime &src);
	void operator=(const SYSTEMTIME &src);

	void operator+=(const CTimeSpan& src);
	void operator-=(const CTimeSpan& src);

};

