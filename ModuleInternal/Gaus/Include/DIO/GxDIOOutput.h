/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  
// [ Gaus Project ]
//
// GxDIOOutput : Digital I/O의 출력 접점에 대한 데이터
//
// 2014. 02. 27, jhLee (semsemi)
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Gaus.h"
#include "DIO\GxDIOData.h"

// #define ON	(BOOL)1
// #define OFF	(BOOL)0

class CGxDIOMgr;

class _GAUS_LIB CGxDIOOutput  
{
	friend class CGxDIOMgr;		// GxDIOMgr에서 Private 멤버에 접근할 수 있도록 허용한다.

private:
	CString		m_sAddr;
	BOOL*		m_pValue;
	BOOL		m_bReverse;

	CGxEvent*	m_pevtOn;
	CGxEvent*	m_pevtOff;

public:
	CString		m_sName;

public:
	CGxDIOOutput();
	CGxDIOOutput(CGxDIOOutput& output);
	CGxDIOOutput(CGxDIOData& diData, LPCTSTR pszAddr);
	virtual ~CGxDIOOutput();

	void On();
	void Off();
	BOOL GetValue();
	void SetValue(BOOL bValue);
	void ReverseValue();
	LPCTSTR GetAddr();

	BOOL operator ==(BOOL bValue);
	CGxEvent* operator ^(BOOL bValue);

private:
	void Init(CGxDIOOutput* pOutput, BOOL bReverse);
};

typedef CTypedPtrList<CPtrList, CGxDIOOutput*> CGxDIOOutputPtrList;
typedef CTypedPtrArray<CPtrArray, CGxDIOOutput*> CGxDIOOutputPtrArray;
typedef CTypedPtrMap<CMapStringToPtr, CString, CGxDIOOutput*> CGxMapStringToOutput;

