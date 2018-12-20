#pragma once

#include "Gaus.h"
#include "GxTask.h"
#include "GxProfile.h"


class _GAUS_LIB CGxStation : public CGxTask
{
protected:
	int		m_nAutoSeq;				// 자동실행 순번
	int		m_nAutoSeqSub;			// 자동실행 보조 순번
	int		m_nAutoSeqPrev;		// 이전에 사용되었던 자동 실행 순번

	int		m_nManualIdx;			// 수동동작 수행 인덱스, 0이면 동작할 내용 없음
	int		m_nManualSub;			// 수동 동작 수행시 하위 선택 순번

	CString	m_sSeqName;				// 자동실행 순번에 대한 이름(설명?)


	UINT ThreadRun1();
	UINT ThreadRun2();
	UINT ThreadRun3();


	// 실제 기능 수행 함수
	// AutoRun 및 Manual Run에서 초기화 및 종료 처리를 실행시켜준다.
	virtual void PreAutoRun()		{};			// AutoRun()을 최초로 수행하기 전에 동작
	virtual void AutoRun()			= 0;			// 자동실행시 반복적으로 호출 될 함수
	virtual void PostAutoRun()		{};			// AutoRun() 수행을 마치고 종료하는 시점에 동작

	virtual void PreManualRun()	{};			// ManualRun()을 최초로 수행하기 전에 동작
	virtual void ManualRun()		= 0;			// Manual 실행시 반복적으로 호출될 함수
	virtual void PostManualRun()	{};			// ManualRun() 수행을 마치고 종료하는 시점에 동작

public:

	//// MP2100을 위한 동기 처리용 이벤트
	//CGxEvent m_evtStartupAuto;						// AutoRun Startup을 알리는 Event 
	//CGxEvent m_evtStartupManual;					// ManaulRun Startup을 알리는 Event 
	//CGxEvent m_evtStartupFinish;					// 초기 시작 처리 완료 


	// 예외 처리
	virtual void Abort()				= 0;			// ABORT 처리, 자손들이 재정의하여 사용한다.
	
	// AutoRun, ManualRun 실행 순번
	void SetAutoSeq(int nSeq);
	int	GetAutoSeq(void);
	void SetManualIdx(int nIdx, int nSub);
	int	GetManualIdx(void);
	int	GetManualSub(void);

	virtual bool SetManualCommand(int nIdx, int nSub);		// 수동 명령을 지령한다.


	void  SetSeqName(CString sName);				// 현재 스텝의 자동실행 이름 대입
	CString GetSeqName(void);						// 현재 자동실행중인 step 이름 조회


	// Station을 제어한다.
	virtual BOOL Startup(void);					// 기동 시킨다.
	virtual void Shutdown(void);					// 종료처리한다.

	virtual BOOL InitProfile(void);
	virtual BOOL LoadProfile(void);
	virtual BOOL SaveProfile(void);
	

public:
	CGxStation(void);
	CGxStation(UINT nCount);
	virtual ~CGxStation(void);
	virtual void SetBasePath(CString sPath);							// 기본 경로를 설정하여 파일 경로를 갱신한다.


};

typedef CArray<CGxStation, CGxStation&> CGxStationArray;
typedef CTypedPtrArray<CPtrArray, CGxStation*> CGxStationPtrArray;
typedef CTypedPtrMap<CMapStringToPtr, CString, CGxStation*> CGxMapStringToStation;

