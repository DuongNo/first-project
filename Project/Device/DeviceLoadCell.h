#pragma once

#include "Serial/CommThread.h"

#define ERR_VALUE	-9999

enum DEVICE_LOADCELL_MODE
{	
	DEVICE_LOADCELL_PRESS_VALUE = 0,
	DEVICE_LOADCELL_REFERENCE_SET,
	DEVICE_LOADCELL_HOLD_START,	
	DEVICE_LOADCELL_HOLD_STOP,
	STEP_ZONE_A_CELL_MAX,
};

class CDeviceLoadCell
{
private:
	CCommThread m_Serial;
	CString m_strSerialName;
	//kjpark 20161024 압력센서 읽기 위한 쓰레드 구현
	UINT m_nSerialNO;
	HANDLE m_hVACMsgBufEvent;

	static UINT thrRecvScheduler(LPVOID pParam);
public:
	//kjpark 20161024 압력센서 읽기 위한 쓰레드 구현
	CDeviceLoadCell(UINT nSerialNO = 0, CString strSerialName = _T(""));
	~CDeviceLoadCell(void);


	BOOL SerialOpen(short nPortNo);
	void SerialClose();
	BOOL IsOpen();

	void SendCommand(DEVICE_LOADCELL_MODE mode, BYTE* buf, int nBufSize);
	void SendCommand_PressValue();
	void SendCommand_Reference();
	void SendCommand_Hold_Start();
	void SendCommand_Hold_Stop();
	//kjpark 20161024 압력센서 읽기 위한 쓰레드 구현
	double GetLoadValue(CString &strValue);
	CCommThread GetCommThreadHandler() {return m_Serial;};	
	CString GetSerialName() {return m_strSerialName;};
	void SetSerialName(CString strValue);
	SERIAL_MODE m_serialMode;
	

	int nCount;
};

