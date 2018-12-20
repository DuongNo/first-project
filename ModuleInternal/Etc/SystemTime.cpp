#include "StdAfx.h"
#include "SystemTime.h"


CSystemTime::CSystemTime(void)
{
	m_wMilliseconds = 0;
}


CSystemTime::~CSystemTime(void)
{
}

int CSystemTime::GetYear()
{
	return m_Time.GetYear();
}

int CSystemTime::GetMonth()
{
	return m_Time.GetMonth();
}

int CSystemTime::GetDay()
{
	return m_Time.GetDay();
}

int CSystemTime::GetHour()
{
	return m_Time.GetHour();
}

int CSystemTime::GetMinute()
{
	return m_Time.GetMinute();
}

int CSystemTime::GetSecond()
{
	return m_Time.GetSecond();
}

WORD CSystemTime::GetMillisecond()
{
	return m_wMilliseconds;
}

CString CSystemTime::Format(LPCTSTR pFormat)
{
	return m_Time.Format(pFormat);
}

void CSystemTime::SyncSystemTime()
{
	SYSTEMTIME time;
	::GetSystemTime(&time);
	m_Time = CTime(time);
	m_wMilliseconds = time.wMilliseconds;
}

CTime CSystemTime::ToCTime()
{
	return m_Time;
}

CTime CSystemTime::GetSystemTime()
{
	CSystemTime time;
	time.SyncSystemTime();
	return time.ToCTime();
}

void CSystemTime::operator=( const CTime &src )
{
	m_Time = src;
	m_wMilliseconds = 0;
}

void CSystemTime::operator=( const SYSTEMTIME &src )
{
	m_Time = CTime(src);
	m_wMilliseconds = src.wMilliseconds;
}

void CSystemTime::operator+=( const CTimeSpan& src )
{
	m_Time += src;
}

void CSystemTime::operator-=( const CTimeSpan& src )
{
	m_Time -= src;
}
