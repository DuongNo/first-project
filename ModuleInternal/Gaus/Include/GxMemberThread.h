//////////////////////////////////////////////////////////////////////
// 
// GxMemberThread : Class안에 Member로서 사용될 수 있는 Thread class
//
// #2007-06-20 : 초기 원형 (ExThreadUnit class)
// #2014-12-21 : Gaus lib에 채용, CGxMemberThread로 이름 변경
// 
//
//////////////////////////////////////////////////////////////////////
#pragma once


#include <process.h>					// for _beginthreadex()
#include <afxmt.h>					// for CEvent

#include "Gaus.h"
#include "GxCallback.h"				// For use Function Pointer 


/* 사용 방법

	0. 멤버 변수로 CGxCallback 포인터를 선언, 이 변수로 함수 포인터 대입과 호출을 수행한다.
		CGxCallback*		m_pFuncPtr;			// 실행시킬 함수 포인터를 기억할 변수
		( CGxMemberThread내애서 선언하여 사용하므로 사용시에는 추후로 선언할 필요는 없다. )
	
	1. 멤버 함수를 대입하여 사용을 하고자 하는 Class에서 자신의 Class 명을 대입한 typedef 문을 선언한다.
	   typedef TFunctionDefine< 클래스명 > TmyFunctionDefine;

   2. TmyFunctionDefine의 변수를 선언하고 생성자 함수나 SetFunction을 이용하여 호출되어질 멤버 함수를 대입시킨다.
		TmyFunctionDefine *pDispFunc = new TmyFunctionDefine;

		pDispFunc->SetFunction(this, &클래스명::멤버함수명);
		or
		pDispFunc->SetFunction(this, 멤버함수명);

	3. Thread 객체 생성
	   CGxMemberThread *m_Thread;
		m_Thread = new CGxMemberThread;

	4. TmyFunctionDefine의 객체를 대입 (포인터)
		m_Thread->SetExecFunction( pDispFunc );
	
		혹은 , 2,4번 항목을 한 번에 수행할 수 있다.
		m_pThread->SetExecFunction(new TmyFunctionDefine(this, ExecServerProcess) );


	5. Thread 생성 및 사용...
		m_Thread->CreateThread(i);
		m_Thread->Resume();	
		m_Thread->Suspend();	

*/


class _GAUS_LIB CGxMemberThread 
{
public:
	CGxMemberThread();
	virtual ~CGxMemberThread();

	enum { 
		eDefault_Interval	= 200,			// 주기적으로 Sleep을 수행하는 시간
		eDefalut_Timeout	= 3000,			// Thread 종료시 Time-out시간 
	};

private:
	CString	m_strMsg;

	CEvent	m_evPause;
	DWORD	m_dwInterval;

	int		m_bPauseFlag;
	int		m_nLockFlag;
	int		m_nLoopFlag;
	BOOL	m_bLoopMode;					// 반복 모드여부, 반복 모드가 아닐 경우에는 1회만 지정한 함수를 호출한다.

   unsigned m_threadID;
	HANDLE	m_hThread;

	CWinThread* m_pThread;

   static unsigned __stdcall ThreadProc (void* PtrToInstance);
   // static unsigned int ThreadProc (void* PtrToInstance);

	CGxCallback*	m_pFuncPtr;				// 실행시킬 함수 포인터를 기억할 변수
	void		ThreadExecute();				// 생성된 Thread가 실행되는 함수


public:
	int		Tag;				// Thread 구분자
	int		TraceFlag;		// 실행 추적용 문자열을 출력할 것인가 ?
	
	BOOL	CreateThread(int nTag, BOOL bLoopMode=FALSE, BOOL bAutoStart=FALSE);		// Thread 생성
	int		DeleteThread();
	void	Start();							// Thead 함수를 시작해 준다 (생성시 bAutoStart값이 FALSE인 경우에만 1회 유효)

	void	SetExecFunction(CGxCallback* p);	// 실행용 함수 포인터 대입
	void	SetInterval(int nInt);				// 샐행 주기 설정
	int		GetInterval();						// 실행 주기 조회
	int		SetPriority(int nPri);

	void	Suspend();
	void	Resume();

	void	Lock();
	void	Unlock();

	CEvent*	GetPauseEventPointer();				// Pause중인 Thread를 깨운기 위한 Event pointer

//	virtual void OnExecute() = 0;			// 실제 실행을 구현시킬 함수, 반드시 구현하여야 함.

};

