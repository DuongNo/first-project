#include "StdAfx.h"
#include "StopWatch.h"

// timeGetTime()
#include <Mmsystem.h>
#pragma comment(lib, "Winmm.lib")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW

#endif
CStopWatch::CStopWatch(void)
{
	m_StopCount = 0;
	Start();
}

CStopWatch::~CStopWatch(void)
{
}

void CStopWatch::Start()
{
	m_StartCount = timeGetTime();
}

double CStopWatch::Stop( BOOL StartAgain )
{
	m_StopCount = timeGetTime();
	double theElapsedTime = ElapsedTime();
	if(StartAgain)
		m_StartCount = m_StopCount; 
	return theElapsedTime;
}

double CStopWatch::ElapsedTime()
{
	if(m_StartCount > m_StopCount)
		return 0.;
	return ( (double)(m_StopCount - m_StartCount) / 1000. );
}