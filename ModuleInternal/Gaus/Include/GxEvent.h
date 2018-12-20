//
// Event Class
//

#pragma once

#include <afxmt.h>												// CEvent 사용

#include "Gaus.h"

class _GAUS_LIB CGxEvent
{
public:
	CString	m_sName;					// Event 이름
	CString	m_sAddress;					// Event 고유 주소
	BOOL	m_bValue;					// 실행 상태 디버깅용


	CEvent	m_Event;					// Event 객체
	CEvent	m_REvent;					// Reverse Event 객체 (m_Event의 반대 논리)
	CEvent*	m_pEvent;					// 지정한 Event의 포인터 보관, Init()에 의해 등록해준다. 
	CEvent*	m_pREvent;					// 반전에 대한 Event 주소

	CEvent*	GetEventPtr(void);			// Event Pointer 획득
	CEvent*	GetREventPtr(void);			// Reverse Event Pointer 획득
	
	BOOL	Set(void);					// Signal 처리
	BOOL	Reset(void);				// Signal Clear 처리
	BOOL	IsSet(void);				// Signal 여부
	BOOL	IsRSet(void);				// Reserse Signal 여부, 삭제 예정
	BOOL	IsReset(void);				// Reserse Signal 여부, New
	BOOL	GetValue(void);				// 디버깅용, 내부 변수 조회
	BOOL	SetValue(BOOL bFlag);		// 값을 지정한다. TRUE:Set, FALSE:Reset

	BOOL operator ==(BOOL bValue);
	CEvent* operator ^(BOOL bValue);

	void Init(CGxEvent* pEvent);		// 지정한 Event로 초기화 시킨다.

public:
	CGxEvent(void);
	CGxEvent(CString sName);
	CGxEvent(CString sAddr, CString sName);
	CGxEvent(BOOL bInitiallyOwn, BOOL bManualReset, CString sName, CString sAddr);
				
	~CGxEvent(void);
};


typedef CArray<CGxEvent, CGxEvent&> CGxEventArray;
typedef CTypedPtrArray<CPtrArray, CGxEvent*> CGxEventPtrArray;
typedef CTypedPtrMap<CMapStringToPtr, CString, CGxEvent*> CMapStringToEvent;