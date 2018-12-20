///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// GxSerial class : RS232/485 통신을 위한 Base class
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Gaus.h"
#include "GxDefine.h"
#include "Comm\GxCommDefine.h"
#include "File\GxIni.h"
#include "File\GxLog.h"

typedef enum
{
	eSERIAL_Normal	= 0,
	eMAX_SerialCount,

	//SRL_TP_NORMAL,
	//SRL_TP_ZXSF11,  
	//SRL_TP_E5XN,   
	//SRL_TP_VERSA6, // Pump Versa6
} ETYPE_Serial;

//d class CGxSerialMgr;
class _GAUS_LIB CGxSerial 
{
protected:
	//enum
	//{
	//	ERR_NONE = 0,
	//	ERR_EXCEPTION = 1,
	//};

public:
	CString	   m_sName;
	UINT	   m_nID;
	UINT	   m_nType;
	UINT	   m_nIndex;
	UINT	   m_nAddr;

	UINT	   m_nPort;
	UINT	   m_nBaudRate;
	UINT	   m_nDataBits;
	UINT	   m_nParity;
	UINT	   m_nStopBits;
	BOOL	   m_bXonXoff;

	// for log
	CGxLog   m_log;
	CGxLog   m_debug;

protected:
	int		m_nError;
	BOOL	   m_bOpenPort;
	BOOL	   m_bDeleteThread;
	BOOL	   m_bSimulate;
	CGxIni   m_profile;

	HANDLE	   m_hPort;	  
	HANDLE	   m_hThread; 
	OVERLAPPED m_osRead;
	OVERLAPPED m_osWrite;

	CWnd*	   m_pParent;
	UINT	   m_umOpen;
	UINT	   m_umClose;
	UINT	   m_umSend;
	UINT	   m_umReceive;

	void*	   m_pRedirObj;
	CGxCommQueue  m_Queue;

	//d CGxSerialMgr* m_pSerialMgr;

public:
	CGxSerial();
	virtual	~CGxSerial();

	virtual BOOL InitProfile(UINT nID, LPCTSTR pszFile);
	virtual BOOL LoadProfile();
	virtual BOOL SaveProfile();
	virtual BOOL Startup();

	CGxLog* GetLogPtr();				// Log 관련 control의 pointer 조회
	CGxLog* GetDebugPtr();				// Debug용 Log 관련 control의 pointer 조회


	int	GetError();
	BOOL  IsOpen();
	BOOL  IsSimulate();
	void* GetRedirObj();
	void  SetMsgInfo(CWnd* pWnd, UINT umOpen, UINT umClose, UINT umSend, UINT umReceive);
	void  SetPacketInfo(UINT nMode, CByteArray* parrToken=NULL, int nIndex=0, UINT nLen=0);
	void  SetRedirectInfo(void* pRedirObj, void (*pf)(void* pSerial));

	BOOL  Open();
	void  Close();

	void  Clear();
	UINT  IsPacket();
	UINT  GetPacket(BYTE* pbyData);
	UINT  GetPacket(BYTE* pbyData, int nLen);
	UINT  GetRecvCount();

	int   SendData(BYTE* pbyData, int nSize);
	int   ReceiveData(BYTE* pbyReceive);
	virtual void OnReceive();

protected:
	virtual void HandleException(int nExp);
	virtual void SetError(int nError);

	void  (*OnRecvFn)(void* pSerial);

	BOOL  CreateThread();
	BOOL  ResumeThread();
	BOOL  SuspendThread();
	BOOL  DeleteThread();

	static DWORD ThreadFn(LPVOID lpData);
};

typedef CTypedPtrList<CPtrList, CGxSerial*> CGxSerialPtrList;
typedef CTypedPtrArray<CPtrArray, CGxSerial*> CGxSerialPtrArray;
typedef CTypedPtrMap<CMapStringToPtr, CString, CGxSerial*> CGxMapStringToSerial;



