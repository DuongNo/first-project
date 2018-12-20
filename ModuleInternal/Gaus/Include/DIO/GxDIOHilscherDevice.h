/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  
// [ Gaus Project ]
//
// GxDIOHilscherDeivce : Hilscher DeviceNET DIO를 위한 실제 IO 담당수행 Class
//
// 2014. 02. 28, jhLee (semsemi)
// 
// 필요File :	Include - CIFUSER.H
//				Lib - CIF32DLL.LIB
//				DLL - cif32dll.dll
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Gaus.h"
#include "DIO\GxDIODevice.h"

class _GAUS_LIB CGxDIOHilscherDevice : public CGxDIODevice
{
private:
	enum 
	{
		MAX_DEVICE = 4,
		DATA_BITS  = 8,
	};

	// profile
	UINT m_nDeviceNum;

	// Internal
	int  m_nSizeIpData;
	int  m_nSizeOpData;
	CByteArray m_arrIpData;
	CByteArray m_arrOpData;
	
public:
	CGxDIOHilscherDevice();
	virtual ~CGxDIOHilscherDevice();

	BOOL InitProfile(UINT nID, LPCTSTR pszFile);
	BOOL LoadProfile();
	BOOL SaveProfile();
	BOOL Startup();

	BOOL InitScanner();		// virtual
	BOOL ResetScanner();		// virtual
	BOOL CloseScanner();		// virtual

private:
	void HandleException(int nExp);	// virtual
	void SetError(int nError);			// virtual

	BOOL SetVariable();	  // 
	BOOL CheckVariable();	// 

	BOOL CheckScannerError(int nVal, BOOL bReInit);
	
	void OnTimer();			// virtual
	void GetInputData();		// virtual
	void SetOutputData();	// virtual

	BOOL ReadData();			// from scanner
	BOOL WriteData();			// to scanner
	BOOL ReadWriteData();	// from, to scanner
};
