/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  
// [ Gaus Project ]
//
// GxDIOMgr : Digital I/O 제어 및 관리를 위한 Class
//
// 2014. 02. 27, jhLee (semsemi)
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Gaus.h"

#include "GxSingleton.h"
#include "GxService.h"
#include "File\GxIni.h"
#include "DIO\GxDIOInput.h"
#include "DIO\GxDIOOutput.h"
#include "DIO\GxDIODevice.h"

class _GAUS_LIB CGxDIOMgr : public CGxSingleton<CGxDIOMgr>	
{
private:
	//d static CGxDIOMgr* theDIOMgr;

	CString m_sName;						// 자신의 이름
	CString m_sBasePath;					// 기본이 되는 Path
	BOOL	m_bSimulate;						// 가상 모드 여부

	CGxIni	m_profile;						// profile 재정의, 기존 CGxProfile은 재귀적 성향이 있으므로 기본 ini 파일로 바꾼다.
	CGxLog	m_debug;						// debug log

	UINT m_nNumScanner;
	UINT m_nNumInput;
	UINT m_nNumOutput;

	CGxMapStringToIOScanner m_mapScanner;
	CGxMapStringToInput     m_mapInput;
	CGxMapStringToOutput    m_mapOutput;

private:

	// GxService로부터 상속받은 기능 수행 함수
	void PreServiceRun();
	void ServiceRun();
	void PostServiceRun();


	BOOL CreateScanner();
	BOOL StartupScanner();
	BOOL CloseScanner();
	void DeleteScanner();

	BOOL CreateIO();
	void DeleteIO();


public:
	CGxDIOMgr();
	virtual ~CGxDIOMgr();


	BOOL GetSimulate();							// Simulation mode 인가 ?
	void SetBasePath(CString sPath);			// 기본 경로를 설정하여 파일 경로를 갱신한다.
	BOOL Startup(void);							// 시스템을 기동 시킨다.
	void Shutdown(void);							// 동작중인 시스템을 종료처리한다.

	BOOL InitProfile(void);
	BOOL LoadProfile(void);
	BOOL SaveProfile(void);



	UINT GetNumScanner();
	UINT GetNumInput();
	UINT GetNumOutput();

	CGxDIODevice*	  GetScanner(LPCTSTR pszName);
	CGxDIOInput*     GetInput(LPCTSTR pszAddr);
	CGxDIOOutput*    GetOutput(LPCTSTR pszAddr);

	//! TEST용
	CGxDIOInput*     TEST_GetInput(LPCTSTR pszAddr);

	void ImmediateTimer();
	void ImmediateTimer(LPCTSTR pszName);

	void SetInput(LPCTSTR pszAddr, CGxDIOInput& ip);
	void SetInput(LPCTSTR pszAddr, CGxDIOInput& ip, LPCTSTR pszKey);
	void SetOutput(LPCTSTR pszAddr, CGxDIOOutput& op);
	void SetOutput(LPCTSTR pszAddr, CGxDIOOutput& op, LPCTSTR pszKey);
};


