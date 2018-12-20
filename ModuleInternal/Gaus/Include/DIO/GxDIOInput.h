/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  
// [ Gaus Project ]
//
// GxDIOInput : Digital I/O의 Input 접점에 대한 class
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

class _GAUS_LIB CGxDIOInput  
{
	friend class CGxDIOMgr;			// GxDIOMgr에서 Private 멤버에 접근할 수 있도록 허용한다.

private:
	CString   m_sAddr;
	CGxEvent* m_pevtOn;
	CGxEvent* m_pevtOff;

	BOOL*     m_pValue;
	BOOL      m_bReverse;

public:
	CString m_sName;
	
public:
	CGxDIOInput();
	CGxDIOInput(CGxDIOInput& input);
	CGxDIOInput(CGxDIOData& diData, LPCTSTR pszAddr);
	virtual ~CGxDIOInput();

	BOOL GetValue();
	LPCTSTR GetAddr();

	void SetValue(BOOL bValue);	// Test용
	void ReverseValue();				// Test용


	BOOL operator ==(BOOL bValue);
	CGxEvent* operator ^(BOOL bValue);

private:
	void Init(CGxDIOInput* pInput, BOOL bReverse);
};

typedef CTypedPtrList<CPtrList, CGxDIOInput*> CGxDIOInputPtrList;
typedef CTypedPtrArray<CPtrArray, CGxDIOInput*> CGxDIOInputPtrArray;
typedef CTypedPtrMap<CMapStringToPtr, CString, CGxDIOInput*> CGxMapStringToInput;
