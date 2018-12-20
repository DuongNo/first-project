#pragma once
#include "BankGuild/UnitCtrlBank.h"

class CStateAMTTable : 	public CUnitCtrlBank
{
public:
	CStateAMTTable(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateAMTTable(void) {};

	enum
	{
		stepStart, // 테이블 이동 전 인터락 체크 및 각 존 SetZone 후 Turn
//		stepTableMoveChk, // 테이블 Turn 체크
//		stepWaitZoneSet, // 각 Zone 신호기 Set
		stepEnd, // 테이블 Turn 체크 후 End.
	};

	int Run();
};


class CStateAZone : 	public CUnitCtrlBank
{
public:
	CStateAZone(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateAZone(void) {};

	//20170201 byskcho
	//cell info request 추가
	/*enum
	{
		stepStart,
		stepCh1TrackInChk,
		stepCh2TrackIn,
		stepCh2TrackInChk,
		stepContactChk, // Contact 체크 및 A Zone에서 원하는 검사 실행.(Sleep Current?)
		stepEnd,  // 검사 실행 Check 후 End.
	};*/

	enum
	{
		stepStart = 0,
		//stepCh1CellRequest,
		stepCh1TrackIn,
		stepCh1TrackInChk,
		//stepCh2CellRequest,
		stepCh2TrackIn,
		stepCh2TrackInChk,
		stepContactChk, // Contact 체크 및 A Zone에서 원하는 검사 실행.(Sleep Current?)
		stepContactRetry,
		// 따로 딜레이가 필요없다. 체크 하는 시간이 딜레이다. 17/04/25 HSK.
		//stepContactDelay,
		stepEnd,  // 검사 실행 Check 후 End.
	};
	BOOL m_bTrackIn;

	int Run();

};


class CStateBZone : 	public CUnitCtrlBank
{
public:
	CStateBZone(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateBZone(void) {};

	enum
	{
		stepStart, // 
		stepContactChk, // 
		stepContactRetry,
		stepAlignMove, // VT Align 해야하는 Cell 인지 확인 후 Align 모터 Ch1 Left 마크 위치로 이동
		stepCh1LeftPosChk, // 모터 이동 체크 후 Ch1 Left 비젼 얼라인 요청.
		stepCh1LeftPosStable, // 모터 안정화.
		stepCh1LeftAlignChk, // 얼라인 성공 여부 확인 후 Ch1 Right 마크 이동.
		stepCh1RightPosChk, // 모터 이동 체크 후 Ch1 Right 비젼 얼라인 요청
		stepCh1RightPosStable, // 모터 안정화.
		stepCh1RightAlignChk, // 얼라인 성공 여부 확인 후 Ch2 Left 비젼 얼라인 위치로 이동
		stepCh2LeftPosChk, // 모터 이동 체크 후 Ch2 Left 비젼 얼라인 요청.
		stepCh2LeftPosStable, // 모터 안정화.
		stepCh2LeftAlignChk, // 얼라인 성공 여부 확인 후 Ch2 Right 마크 이동.
		stepCh2RightPosChk, // 모터 이동 체크 후 Ch2 Right 비젼 얼라인 요청
		stepCh2RightPosStable, // 모터 안정화.
		stepCh2RightAlignChk,  // 모터 이동 체크 후 Ch2 Right 비젼 얼라인 요청
		stepContactEnd,
		stepEnd,  // 대기 위치 이동 확인 후 End.
	};

	BOOL m_bCh1Exist;
	BOOL m_bCh2Exist;

	int Run();

};

class CStateCZone : 	public CUnitCtrlBank
{
public:
	CStateCZone(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateCZone(void) {};

	enum
	{
		stepStart, // 비젼 모터 축 검사 위치 확인 및 모터 축 이동.
		stepPosChk, // 비젼 모터 축 검사 위치 확인 후 Theta Offset 값 이동.
		stepOffsetPosChk, // B Zone에서 받은 Offset 값 이동 확인 후 그랩 스타트 전송
		stepBeginPosMove, // Grab End 체크 후 Turn 모터 기본 위치로 이동.
		stepEnd, // Turn 모터 기본위치 이동 체크
	};

	int Run();
};

class CStateDZone : 	public CUnitCtrlBank
{
public:
	CStateDZone(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateDZone(void) {};

	enum
	{
		stepStart,
		stepClassEndChk, // 비전 Class End Check 및 Contact Off 및 원하는 검사 실행 (White Current?)
		stepFileChk, // 비전 Class End Check 및 Contact Off 및 원하는 검사 실행 (White Current?)
		stepEnd, // Contact Off 확인 후 End ( Cell data 기록은 Unloader TR이 집어 갈때 기록.)
	};

	int Run();

};
//2017-09-26,SKCHO, T5 RESTART 관련 함수 8개 생성
class CStateCH1_T5ReStart : 	public CUnitCtrlBank
{
public:
	CStateCH1_T5ReStart(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	};
	~CStateCH1_T5ReStart(void) {};

	enum
	{
		stepStart,
		stepReSetAckChk, // 비전 Class End Check 및 Contact Off 및 원하는 검사 실행 (White Current?)
		stepChk, // 비전 Class End Check 및 Contact Off 및 원하는 검사 실행 (White Current?)
		stepEnd, // Contact Off 확인 후 End ( Cell data 기록은 Unloader TR이 집어 갈때 기록.)
	};

	int Run();

};
