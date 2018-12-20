/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  
// [ Gaus Project ]
//
// GxEventObj : GxEvent를 각종 Waiting류 함수에 적용하기 위한 응용 class
//
// 2014. 12. 18, jhLee (semsemi)
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Gaus.h"
#include "GxEvent.h"

class _GAUS_LIB CGxEventObj
{
public:
	CString		m_sName;
	CString		m_sAddr;

	CGxEvent	m_evtOn;
	CGxEvent	m_evtOff;

	CGxEvent*	m_pevtOn;
	CGxEvent*	m_pevtOff;

	BOOL		m_bValue;
	BOOL*		m_pValue;
	BOOL		m_bReverse;

	
public:
	CGxEventObj();
	CGxEventObj(CGxEventObj& eEvent);
	virtual ~CGxEventObj();

	BOOL GetValue();
	LPCTSTR GetAddr();

	void Set();
	void Reset();
	BOOL IsSet();
	BOOL IsReset();

	void SetValue(BOOL bValue);	
	void ReverseValue();				

	BOOL operator ==(BOOL bValue);
	CGxEvent* operator ^(BOOL bValue);

private:
	void Init(CGxEventObj* pEvent, BOOL bReverse);
};

typedef CTypedPtrList<CPtrList, CGxEventObj*> CGxEventObjPtrList;
typedef CTypedPtrArray<CPtrArray, CGxEventObj*> CGxEventObjPtrArray;
typedef CTypedPtrMap<CMapStringToPtr, CString, CGxEventObj*> CGxMapStringToEventObj;
