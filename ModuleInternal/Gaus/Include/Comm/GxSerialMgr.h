//
// Serial 통신 관리자
//

#pragma once

#include "Gaus.h"
#include "GxSingleton.h"
#include "GxObject.h"
#include "GxProfile.h"
#include "Comm\GxSerial.h"

//d class __declspec(dllexport) CGxSerialMgr : public CAxService
class _GAUS_LIB CGxSerialMgr : public CGxObject, public CGxSingleton<CGxSerialMgr>	
{
private:
	// static CGxSerialMgr* theSerialMgr;	

	UINT  m_nNumSerial;
	CGxMapStringToSerial m_mapSerial;

public:
	//m CString		m_sName;								// Manager name

	BOOL		m_bSimulate;
	CGxProfile	m_profile;

	CWnd* m_pParent;
	UINT  m_umOpen;
	UINT  m_umClose;
	UINT  m_umSend;
	UINT  m_umReceive;

private:

	BOOL CreateSerial();
	BOOL StartupSerial();
	void DeleteSerial();

public:
	CGxSerialMgr();
	virtual ~CGxSerialMgr();
	// static CGxSerialMgr* GetSerialMgr();

	BOOL Setup();
	BOOL InitProfile();
	BOOL LoadProfile();
	BOOL SaveProfile();
	BOOL Startup();

	BOOL GetSimulate();

	void SetMsgInfo(CWnd* pWnd, UINT umOpen, UINT umClose, UINT umSend, UINT umReceive);
	void SetSerialMsgInfo(CWnd* pWnd, UINT umOpen, UINT umClose, UINT umSend, UINT umReceive);

	UINT GetNumSerial();
	CGxSerial* GetSerial(LPCTSTR pszName);

	void OpenSerial();
	void OpenSerial(LPCTSTR pszName);

	void CloseSerial();
	void CloseSerial(LPCTSTR pszName);
};

