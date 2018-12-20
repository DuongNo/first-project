/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  
// [ Gaus Project ]
//
// GxTask : Thread를 기본 바탕으로 동작하며 자동화 처리 Class들의 부모 Class
//
// 2014. 02. 27, jhLee (semsemi)
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Gaus.h"
#include "GxDefine.h"
#include "GxThread.h"

#include "GxProfile.h"
#include "GxEvent.h"
#include "File\GxLog.h"			


class _GAUS_LIB CGxTask : public CGxThread
{
protected:
	UINT		m_nThreadCount;			// Thread 수량

	int		m_nMyState;				// 현재 자신의 실행 상태를 보관
	CString	m_sBasePath;			// 기본이 되는 경로

	BOOL		m_bSimulate;			// H/W 연결없이 가상모드로 동작할 것인가 ?
	BOOL		m_bDryRun;				// Dry-Run (Cell 없이 가상으로 동작시키는가 ?)
	BOOL		m_bIsDebugLog;			// 디버깅용 상태 변환 Log를 남길것인가 ? False로 지정하면, Overhead 시간을 줄일 수 있다.


	// Gaus Lib 사용
	CGxProfile  m_profile;				// 각종 설정정보 저장용

	// 각종 상태를 전환하기 위한 event, Master에서 지정.
	CGxEvent	m_evtAbort;				// 동작을 취소하고 초기 상태로 되돌린다.
	CGxEvent	m_evtStop;				// Stop 상태인가 ?
	CGxEvent	m_evtAuto;				// Auto 신호
	CGxEvent	m_evtManual;			// Manual 동작 신호
	CGxEvent	m_evtShutdown;			// 객체를 소멸할 때, Block된 Thread를 풀기위한 event


	CGxLog	m_log;					// Log 기록용
	CGxLog	m_debug;				// Debug 기록용
	CRITICAL_SECTION	m_csStateUpdate;	// 상태를 갱신하는 객체의 공유 지원, EnterState()에서 사용


	// 각종 상태를 보고하는 함수
	void EnterState(int nState);	// Master에게 지정 상태에 진입하였다고 통보한다.
	void ClearEvents(void);			// 지정 event들을 Reset 시켜준다.

public:
	CGxTask(void);
	CGxTask(UINT nCount);			// Thread 수 지정 초기화
	virtual ~CGxTask(void);


public:

	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// 이름 및 상태 지정/조회
	void SetName(CString sName);
	CString GetName(void);
	CString GetBasePath();			// Base 경로를 조회한다.


	BOOL GetSimulate(void);			// H/W 연결없이 가상모드로 동작할 것인가 ?
	BOOL GetDryRun(void);			// Dry-Run (Cell 없이 가상으로 동작시키는가 ?)


	// 상태 지정 / 조회 
	void SetState(int nState);
	int  GetState(void);
	void SetReset(void);				// 초기/Abort시 사용 준비를 해준다.

	CGxLog* GetLogPtr(void);		// Log 관련 control의 pointer 조회
	CGxLog* GetDebugPtr(void);	// Debug용 Log 관련 control의 pointer 조회

	void SetDebugOut(BOOL bFlag);	// Debug용 출력을 할것인가 ?
	void SetLogOut(BOOL bFlag);		// Log를 출력 할것인가 ?
	BOOL GetDebugOut();				// Debug 출력 중인가 ?
	BOOL GetLogOut();				// log를 출력중인가 ?

	// 각종 Event 지정
	void SetEventAbort();			// Abort event 지정
	void SetEventStop();				// Stop Event 지정
	void SetEventAuto();				// Auto Event 지정
	void SetEventManual();			// Manual event 지정
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


	// Auto 상태에서 Alarm을 지정하고 그에대한 응답을 대기한다.
	void SetErrDetailMsg(int nECode, CString sMsg);		// 상세 발생원인 지정
	int SetError(UINT nECode, int nQuery = eQUERY_RETRY, BOOL bEmgStop = FALSE);				// Alarm을 등록하고 사용자의 응답을 기다린다.
	int SetError(UINT nECode, int nQuery, BOOL bEmgStop, CString sDetailMsg);	// 상세 메세지를 함께 지정한다.
	void SetEmgError(UINT nECode, CString sDetailMsg=_T(""));		// 비상정지를 설정한다. 응답을 기다리지 않고 다음을 진행

	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// Thread 관련 제어
	virtual BOOL Startup(void);					// Thread 생성
	virtual void Shutdown(void);				// Thread 삭제

	// 상태 전환전에 수행할 함수
	virtual	void BeforeAuto();					// AUTO로 전환전에 수행 할 내용
	virtual	void BeforeStop();					// STOP으로 전환전에 수행 할 내용

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 실행 흐름 제어
	void TaskWait(UINT nTime);					// 일정시간 Delay, System/Service에서 Master의 State와 상관없이 종료만 감시한다.
	void TaskWaitAuto(UINT nTime);				// AutoRun() 진행 제어

	void TaskWaitManual(void);					// ManualRun() 진행 제어
	void WaitingForAuto(void);					// Auto 동작 중 Stop이 된 상태에서 다시 Auto를 기다린다.

	void WaitInAlarm(void);						// Alarm 상태에서 Event 대기

	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// 단순하게 사용하는 Event 대기 함수 (무한정 대기)
	void WaitEvent(CGxEvent* pEvt);				// 지정 이벤트를 무한정 기다린다.	(AutoRun() 에서 사용)
	void WaitAllEvent(CGxEvent* pEvt, ...);		// 지정 이벤트들를 무한정 기다린다.	(AutoRun() 에서 사용, 인수의 끝은 NULL 표기)
	DWORD WaitEvent(DWORD dwTimeout, CGxEvent* pEvt);	// 지정 이벤트를 정해진 시간마큼 기다린다.	(AutoRun() 에서 사용)

	// 좀더 복잡한 조건이 들어가는 대기 함수들

	// 보통의 SyncObject Event의 모든 Signal 대기
	DWORD WaitAllEvents(DWORD dwTimeout, CSyncObjPtrArray &evtArr);
	DWORD WaitAllEventsNS(DWORD dwTimeout, CSyncObjPtrArray &evtArr);		// STOP 신호에 반응하지 않는다.
	DWORD WaitAllEvents(DWORD dwTimeout, CSyncObject* pEvt, ...);
	DWORD WaitAllEventsNS(DWORD dwTimeout, CSyncObject* pEvt, ...);

	// CGxEvent의 모든 Signal 대기
	DWORD WaitAllEvents(DWORD dwTimeout, CGxEventPtrArray &evtArr);
	DWORD WaitAllEventsNS(DWORD dwTimeout, CGxEventPtrArray &evtArr);
	DWORD WaitAllEvents(DWORD dwTimeout, CGxEvent* pEvt, ...);
	DWORD WaitAllEventsNS(DWORD dwTimeout, CGxEvent* pEvt, ...);

	// 응용, Manual과 Auto를 구분해서 적용한다.
	DWORD WaitAllEvents(BOOL bManual, DWORD dwTimeout, CGxEvent* pEvt, ...);


	// Event의 특정 Signal 대기
	DWORD WaitEvents(DWORD dwTimeout, CSyncObjPtrArray &evtArr);
	DWORD WaitEventsNS(DWORD dwTimeout, CSyncObjPtrArray &evtArr);
	DWORD WaitEvents(DWORD dwTimeout, CSyncObject* pEvt, ...);
	DWORD WaitEventsNS(DWORD dwTimeout, CSyncObject* pEvt, ...);

	// CGxEvent의 특정 Signal 대기
	DWORD WaitEvents(DWORD dwTimeout, CGxEventPtrArray &evtArr);
	DWORD WaitEventsNS(DWORD dwTimeout, CGxEventPtrArray &evtArr);
	DWORD WaitEvents(DWORD dwTimeout, CGxEvent* pEvt, ...);
	DWORD WaitEventsNS(DWORD dwTimeout, CGxEvent* pEvt, ...);


	// System에서 사용하기 위한 Event대기
	DWORD WaitEventsForSystem(DWORD dwTimeout, CSyncObjPtrArray& evtArr);
	DWORD WaitEventsForSystem(DWORD dwTimeout, CGxEvent* pEvt, ...);

};

