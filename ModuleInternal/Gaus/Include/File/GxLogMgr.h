//
// CGxLogMgr : GxLog와 GxLogData를 등록하고 조회하며, 정해진 주기로 삭제 trigger를 실행해주는 Manager class
//
// 2015-10-27, jhLee
// 2015-10-29, jhLee, 완료
//

#pragma once

#include "Gaus.h"
#include "File\GxLog.h"


class _GAUS_LIB CGxLogMgr
{
private:
	static CGxLogMgr*	theLogMgr;						// SingleTone 객체

	CGxLogPtrArray			m_arrLog;					// System 보관 Array
	CGxMapStringToLog		m_mapLogName;				// 이름에 의한 검색을 위한 Map
	CGxMapWordToLog		m_mapLogID;					// ID에 의한 검색을 위한 Map

	CEvent	m_evtTerminate;							// 종료를 위한 동기화 이벤트
	BOOL		m_bTerminate;								// 종료를 할것인가 flag 변수
	BOOL		m_bAutoDeleteFlag;						// 자동 삭제를 수행할 것인가 ?
	BOOL		m_bDebugLog;								// 디비깅을 위한 Trace log를 표시할 것인가 ?

	UINT		m_nInterval;								// 반복주기, 어느 주기로 삭제를 시도할 것인가 ? 각 Log별 반복 주기, Minute(분)단위
	DWORD		m_dwSleepTick;								// 각 Log 객체별 순환을 위한 Sleep 주기, msec (밀리초) 단위
	int		m_nCheckDay;								// 오늘기준으로 몇일 이나 점검을 할 것인가 ?

	HANDLE	m_hThreadHandle;							// Thread Handle


	enum {
		eTIME_DefaultTick			= 60000,				// 기본적인 유휴시간, 60초
		eTIME_LeastTick			= 1000,				// 최소한의 유휴시간 지정, 1초

		eMAX_CheckDay				= 7,					// 최대 7일간 데이터를 검색하여 삭제한다.
	};


public:
	CGxLogMgr();
	~CGxLogMgr();

	static CGxLogMgr* GetInstance();					// LogMgr 객체 조회
	static void ReleaseInstance();					// SingleTone, LogMgr 객체 파괴
	UINT ThreadRun();										// Thread 반복 처리 실행문

	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
	void SetDebugLog(BOOL bFlag);						// Debugging용 Trace log를 남길것인지 지정

	int GetCount();										// 등록된 log 객체 수량
	int AddByName(CString sName, CGxLog* pLog);	// 이름을 키로 Log 객체를 추가한다.
	int AddByID(int nIdx, CGxLog* pLog);			// ID를 키로 Log 객체를 추가한다.
	void RemoveAll();										// Add로 통해 등록된 모든 Log 정보를 삭제해준다.

	void Startup(int nMin);								// 기동을 시킨다. 몇 분 주기로 각 Log에게 이전 데이터 삭제를 지령할 것인가 ?
	void Shutdown();										// 기능을 중지하고 종료시킨다.


	// 등록된 Log 객체들을 제어한다.
	void SetLogEnabled(BOOL bFlag);					// 등록된 모든 Log들의 기록 여부를 지정한다.
	CGxLog* GetLogByName(CString sName);			// 이름으로 log 객체를 검색하여 포인터를 획득한다.
	CGxLog* GetLogByID(int nID);						// ID로 Log 객체를
	CGxLog* GetLogByIndex(int nIdx);					// Index로 Log 객체를 조회한다.

	void RemoveOldLog(int nDay);						// 수작업으로 기준일 이전의 log들을 일괄 제거한다.

	// 자동 삭제 기능을 지정한다.
	void SetInterval(int nSpan);						// 얼마의 주기로 자동 삭제를 실행할 것인가 ? 초
	void SetDeleteEnabled(BOOL bFlag);				// 자동 삭제 기능 활성/비활성화
	void SetCheckDay(int nDay);						// 오늘기준으로 과거로 몇일 이나 점검을 할 것인가 ?
};

