/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  
// [ Gaus Project ]
//
// GxDIOScanner : 주요 Class들의 근간이되는 최고 조상 Class
//
// 2014. 02. 27, jhLee (semsemi)
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Gaus.h"
#include "GxWaitTimer.h"
#include "DIO\GxDIOData.h"
#include "File\GxLog.h"
#include "File\GxIni.h"

typedef enum
{
	DIO_DEIVCE_HILSCHER = 0,
//	IOS_TP_MMC,
//	IOS_TP_YS6016,
//	IOS_TP_CCLINK,
//	IOS_TP_MEI,	     // MEI SynqNET
//	IOS_TP_SPIP40,   // SoftPlan Input40 
};
// EIosType;

class CGxDIOMgr;

class _GAUS_LIB CGxDIODevice 
{
	friend class CGxDIOMgr;				// CGxDIOMgr에서 Private 멤버들을 직접 접근할 수 있도록 지정해준다.

protected:
	enum 
	{
		MIN_SCAN_TIME = 10,
		MAX_SCAN_TIME = 100,
		MAX_NUM_IO	  = 1000,
	};

	enum
	{
		ERR_NONE = 0,
		ERR_EXCEPTION = 1,
	};

	// Common 
	CString		m_sName;        // Name
	UINT		m_nID;          // Index
	UINT		m_nType;        // 0(Hilscher), 1(Mmc), 2(YS6016), 3(CCLink)
	UINT		m_nIndex;       // Type 내에서 Index

	BOOL		m_bInit;
	int			m_nError;
	BOOL		m_bSimulate;
	CGxIni		m_profile;

	// Additional 
	UINT		m_nScanTime;    // 폴링시간(업데이트시간) (ms)
	BOOL		m_bTerminate;
	HANDLE		m_hScanThread;
	CMutex		m_mutex;
	HANDLE		m_hMutex;

	UINT		m_nNumInput;    // 입력접점 수 
	UINT		m_nNumOutput;   // 출력접점 수
	UINT		m_nIpStartAddr; // 입력접점 시작 어드레스 (보드가 여러장일때)
	UINT		m_nOpStartAddr; // 출력접점 시작 어드레스 (보드가 여러장일때) 

	// CGxTrace m_trace;
	CGxLog	m_debug;				// Debug 기록용

	CGxWaitTimer m_timer;
	CGxDIODataArray m_arrDioIpData;
	CGxDIODataArray m_arrDioOpData;

	// Object Pointer
	CGxDIOMgr* m_pDIOMgr;

public:
	CGxDIODevice();
	virtual ~CGxDIODevice();

	CGxLog* GetDebugPtr(void);	// Debug용 Log 관련 control의 pointer 조회
	void SetDebugOut(bool bFlag);	// Debug용 출력을 할것인가 ?


	virtual BOOL InitProfile(UINT nID, LPCTSTR pszFile);
	virtual BOOL LoadProfile();
	virtual BOOL SaveProfile();
	virtual BOOL Startup();

	virtual BOOL InitScanner() = 0;
	virtual BOOL ResetScanner() = 0;
	virtual BOOL CloseScanner() = 0;
	
	UINT		 AutoScan();
	void		 ImmediateTimer();
	int			GetError();
	BOOL		 IsInit();
	
	UINT		 GetNumInput();
	UINT		 GetNumOutput();
	CGxDIOData&	 GetDioIpData(int nIndex);
	CGxDIOData&	 GetDioOpData(int nIndex);

protected:
	virtual void HandleException(int nExp) = 0;
	virtual void SetError(int nError);

	BOOL		 SetVariable();
	BOOL		 CheckVariable();
	
	virtual void OnTimer() = 0;
	virtual void GetInputData() = 0;  // Input Data -> CGxDIOData
	virtual void SetOutputData() = 0; // CGxDIOData -> Output Data

	BOOL		 CreateDioData();
};

typedef CTypedPtrList<CPtrList, CGxDIODevice*> CGxDIODevicePtrList;
typedef CTypedPtrArray<CPtrArray, CGxDIODevice*> CGxDIODevicePtrArray;
typedef CTypedPtrMap<CMapStringToPtr, CString, CGxDIODevice*> CGxMapStringToIOScanner;
