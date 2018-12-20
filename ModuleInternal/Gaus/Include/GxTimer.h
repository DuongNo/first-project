//
// GxTimer : 지정 시간이 경과 하였는지 check한다.
//

#pragma once

#include "Gaus.h"

class _GAUS_LIB CGxTimer  
{
private:
	LONG m_time;
	LONG m_timespan;
	LARGE_INTEGER m_freq;
	BOOL m_bUse_qpc;				// QueryPerformanceFrequency를 사용할 수 있는가 ?

public:
	CGxTimer();
	virtual ~CGxTimer();

	LONG GetTime();

	void Start();
	void Pause();
	void Resume();

	BOOL IsTimeUp(LONG nTm);
	void WaitTimeUp(LONG nTm);

	LONG GetTimeSpan();
};


