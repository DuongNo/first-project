/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  
// [ Gaus Project ]
//
// GxProfile : 설비 기동에 필요한 IO 및 Event, Motion 정보를 관리하는 Class
//
// 2014. 02. 27, jhLee (semsemi)
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Gaus.h"

#include "File\GxIni.h"
#include "GxEvent.h"
#include "DIO\GxDIOInput.h"
#include "DIO\GxDIOOutput.h"

//d class CGxDIOMgr;
//d class CGxEventMgr;

class _GAUS_LIB CGxProfile : public CGxIni
{
protected:
	CGxMapStringToInput  m_inputMap;
	CGxMapStringToOutput m_outputMap;


public:
	CGxProfile();
	virtual ~CGxProfile();

	void Load();
	void Load(LPCTSTR pszFile);
	void Save();
	void Save(LPCTSTR pszFile);

	BOOL AddInput(LPCTSTR pszKey, CGxDIOInput& input, BOOL bCheckAddr=TRUE);
	BOOL AddInput(LPCTSTR pszSect, LPCTSTR pszKey, CGxDIOInput& input, BOOL bCheckAddr=TRUE);
	BOOL AddOutput(LPCTSTR pszKey, CGxDIOOutput& output, BOOL bCheckAddr=TRUE);
	BOOL AddOutput(LPCTSTR pszSect, LPCTSTR pszKey, CGxDIOOutput& output, BOOL bCheckAddr=TRUE);

	BOOL SetupInput(LPCTSTR pszKey, CGxDIOInput& input, BOOL bCheckAddr=TRUE);
	BOOL SetupInput(LPCTSTR pszSect, LPCTSTR pszKey, CGxDIOInput& input, BOOL bCheckAddr=TRUE);
	BOOL SetupOutput(LPCTSTR pszKey, CGxDIOOutput& output, BOOL bCheckAddr=TRUE);
	BOOL SetupOutput(LPCTSTR pszSect, LPCTSTR pszKey, CGxDIOOutput& output, BOOL bCheckAddr=TRUE);
};

typedef CArray<CGxProfile, CGxProfile&> CGxProfileArray;
typedef CTypedPtrArray<CPtrArray, CGxProfile*> CGxProfilePtrArray;
