/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  
// [ Gaus Project ]
//
// GxDIOData : Digital 입/출력을 제어하기위한 자료구조
//
// 2014. 02. 27, jhLee (semsemi)
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Gaus.h"
#include "GxEvent.h"

#define ON	(BOOL)1
#define OFF	(BOOL)0

class CGxDIOInput;
class CGxDIOOutput;
class CGxDIOMgr;

class _GAUS_LIB CGxDIOData 
{

	// 아래의 Class에서 Private 멤머를 접근할 수 있도록 한다.
	friend class CGxDIOInput;
	friend class CGxDIOOutput;
	friend class CGxDIOMgr;

private:
	CGxEvent m_evtOn;
	CGxEvent m_evtOff;

	BOOL     m_bValue;

public:
	CString  m_sName;
	CString  m_sAddr;

public:
	CGxDIOData();
	virtual ~CGxDIOData();

	void SetValue(BOOL bValue);
	BOOL GetValue();
	void FireEvents();
};

typedef CTypedPtrList<CPtrList, CGxDIOData*> CGxDIODataPtrList;
typedef CTypedPtrArray<CPtrArray, CGxDIOData*> CGxDIODataPtrArray;
typedef CTypedPtrMap<CMapStringToPtr, CString, CGxDIOData*> CAxMapStringToDioData;

typedef CArray<CGxDIOData, CGxDIOData&> CGxDIODataArray;
