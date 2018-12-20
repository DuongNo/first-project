#pragma once

/********************************************************************
	created:	2014/10/02
	modefied:	07:10:2014   09:54
	author:		OSC
	
	purpose:	msec���� �����ϴ� �ð� Class
				SyncSystemTime() ȣ��� ǥ�ؽð��븦 �����ص� ���� 
					���󰡵��� SYSTEMTIME���� ����ð��� �����´�.
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

