/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  
// [ Gaus Project ]
//
// GxWaitTimer : 일정시간 시간을 지연시킨다.
//
// 2014. 02. 28, jhLee (semsemi)
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Gaus.h"

class _GAUS_LIB CGxWaitTimer  
{
private:
	HANDLE m_hTimer;
	int    m_nDueTm;
	int    m_nPeriod;

public:
	CGxWaitTimer();
	virtual ~CGxWaitTimer();

	void SetTimer(int nDueTm, int nPeriod);
	void WaitTimer();
	void ImmediateTimer();
};

