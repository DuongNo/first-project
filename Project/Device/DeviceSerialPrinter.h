#pragma once

#include "Serial/CommThread.h"

#define ERR_VALUE	-9999

enum DEVICE_PRINETR_MODE
{	
	DEVICE_PRINETR_PRESS_VALUE = 0,
	DEVICE_PRINETR_REFERENCE_SET,
	DEVICE_PRINETR_HOLD_START,	
	DEVICE_PRINETR_HOLD_STOP,
	DEVICE_PRINETR_MAX
};

class CDeviceSerialPrinter
{
private:
	CCommThread m_Serial;
	CString m_strSerialName;
public:
	CDeviceSerialPrinter(CString strSerialName = _T(""));
	~CDeviceSerialPrinter(void);


	BOOL SerialOpen(short nPortNo);
	void SerialClose();
	BOOL IsOpen();

	void SendCommand(DEVICE_PRINETR_MODE mode, BYTE* buf, int nBufSize);
	void SendCommand_PressValue();
	void SendCommand_Reference();
	void SendCommand_Hold_Start();
	void SendCommand_Hold_Stop();
	double GetLoadValue();
	CCommThread GetCommThreadHandler() {return m_Serial;};
	CString GetSerialName() {return m_strSerialName;};
	void SetSerialName(CString strValue);
	SERIAL_MODE m_serialMode;

	//kjpark 20161004 Label Printer IF Ãß°¡
	BOOL TestPrintOut();
	BOOL TestBarcodePrint(CString sMsg);

	DWORD LabelPrintOut(int nLn, CString sMsg);
};

