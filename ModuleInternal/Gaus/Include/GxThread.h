//==================================================================================================
// CGxThread : Gaus Library의 근간이 되는 Thread 생성/관리 class
//
// #2014-03-13, jhLee (semsemi)
// #2015-04-25, jhLee, CGxObject 상속으로 변경
//
// 파생 Class : CGxTask, CGxMaster
//
//==================================================================================================
#pragma once

#include "Gaus.h"
#include "GxObject.h"


class _GAUS_LIB CGxThread : public CGxObject
{
public:
	enum {
		eIDX_Thread1	= 0,					// Thread의 순번, Primery 주 쓰레드
		eIDX_Thread2,
		eIDX_Thread3,	
		eMAX_ThreadCount						// Thread의 수량
	};

	BOOL		m_bTerminate;					// 종료 여부를 지정하는 Flag, TRUE:종료, FALSE:계속실행
	HANDLE	m_hThreads[eMAX_ThreadCount];		// Thread Handle 배열

protected:
	UINT	m_nInterval;						// 대기시간 ScanTime;
	UINT	m_nThreadCount;						// Thread 수량


   static UINT __stdcall ThreadProc1(void* PtrToInstance);
   static UINT __stdcall ThreadProc2(void* PtrToInstance);
   static UINT __stdcall ThreadProc3(void* PtrToInstance);


public:
	CGxThread(void);
	virtual ~CGxThread(void);

	void SetThreadInterval(UINT nValue);		// Thread 동작시 Sleep Interval 지정
	UINT GetThreadInterval(void);				// 지정된 Interval 조회


	// Thread 수행 함수, 반드시 재정의하여 사용
	virtual UINT ThreadRun1() = 0;
	virtual UINT ThreadRun2() = 0;
	virtual UINT ThreadRun3() = 0;


	BOOL CreateThreads(UINT nNum);

	BOOL SuspendGxThread(HANDLE hThread);		// 지정 Thread 중지
	BOOL ResumeGxThread(HANDLE hThread);		// 지정 Thread 재개
	void DeleteGxThread(HANDLE hThread);		// 지정 Thread 종료

	void ResumeThreads();						// 소유한 모든 Thread 재개
	void DeleteThreads();						// 소유한 모든 Thread 종료
};

