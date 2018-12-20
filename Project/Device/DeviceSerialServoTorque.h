#pragma once

#include "Serial/CommThread.h"

#define ERR_VALUE	-9999

enum DEVICE_SERVOOTORQUE_MODE
{	
	DEVICE_SERVOOTORQUE_PRESS_VALUE = 0,
	DEVICE_SERVOOTORQUE_REFERENCE_SET,
	DEVICE_SERVOOTORQUE_HOLD_START,	
	DEVICE_SERVOOTORQUE_HOLD_STOP,
	DEVICE_SERVOOTORQUE_MAX
};

class CDeviceSerialServoTorque
{
private:
	CCommThread m_Serial;
	CString m_strSerialName;
public:
	CDeviceSerialServoTorque(CString strSerialName = _T(""));
	~CDeviceSerialServoTorque(void);


	BOOL SerialOpen(short nPortNo);
	void SerialClose();
	BOOL IsOpen();

	void SendCommand(DEVICE_SERVOOTORQUE_MODE mode, BYTE* buf, int nBufSize);
	void SendCommand_PressValue();
	void SendCommand_Reference();
	void SendCommand_Hold_Start();
	void SendCommand_Hold_Stop();
	double GetLoadValue();
	CCommThread GetCommThreadHandler() {return m_Serial;};
	CString GetSerialName() {return m_strSerialName;};
	void SetSerialName(CString strValue);
	SERIAL_MODE m_serialMode;
};

