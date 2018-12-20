#include "StdAfx.h"
#include "DeviceSerialServoTorque.h"
#include "CheetahApp.h"
#include "Etc/Etc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDeviceSerialServoTorque::CDeviceSerialServoTorque(CString strSerialName)
{
	m_strSerialName = strSerialName;
	m_serialMode = SERIAL_SERVOTORQUE;
}


CDeviceSerialServoTorque::~CDeviceSerialServoTorque(void)
{
}

BOOL CDeviceSerialServoTorque::SerialOpen( short nPortNo )
{
	if(IsOpen()) return TRUE;

	CString str;

	return m_Serial.InitCommPort(nPortNo,CBR_19200,8,ONESTOPBIT,NOPARITY);
}

void CDeviceSerialServoTorque::SerialClose()
{
	if(IsOpen() == FALSE) return;

	m_Serial.ClosePort();
}

BOOL CDeviceSerialServoTorque::IsOpen()
{
	return m_Serial.m_bConnected;
}

void CDeviceSerialServoTorque::SendCommand(DEVICE_SERVOOTORQUE_MODE mode, BYTE* buf, int nBufSize)
{
	int nCheckSum = 0;
	for(int i = 1; i < nBufSize-3; i++)
	{
		nCheckSum += buf[i];
	}
	CStringA strCheckSum;
	strCheckSum.Format("%X", nCheckSum);
	// 	char cCheckSum[4];
	// 	_itoa_s(nCheckSum, cCheckSum, 4, 16);
	// 	buf[7] = cCheckSum[1];
	// 	buf[8] = cCheckSum[2];
	buf[nBufSize-3] = strCheckSum.GetAt(1);
	buf[nBufSize-2] = strCheckSum.GetAt(2);
	m_Serial.WriteComm(buf, nBufSize);
	WCHAR a[100];
	CEtc::ConvertUnicode((char*)buf, nBufSize, a, 100);
	a[nBufSize] = NULL;
	theLog[LOG_SERIAL].AddBuf(_T("SEND %s"), a);
}

void CDeviceSerialServoTorque::SendCommand_PressValue()
{
	if(IsOpen() == FALSE) return;
	BYTE buf[10] = {0x02, 0x30, 0x31, 0x52, 0x43, 0x57, 0x54, 0, 0, 0x03};
#if 1
	SendCommand(DEVICE_SERVOOTORQUE_PRESS_VALUE, buf, 10);
#else
	int nCheckSum = 0;
	for(int i = 1; i < 7; i++)
	{
		nCheckSum += buf[i];
	}
	CStringA strCheckSum;
	strCheckSum.Format("%X", nCheckSum);
// 	char cCheckSum[4];
// 	_itoa_s(nCheckSum, cCheckSum, 4, 16);
// 	buf[7] = cCheckSum[1];
// 	buf[8] = cCheckSum[2];
	buf[7] = strCheckSum.GetAt(1);
	buf[8] = strCheckSum.GetAt(2);
	m_Serial.WriteComm(buf, 10);
	WCHAR a[100];
	CEtc::ConvertUnicode((char*)buf, 10, a, 100);
	a[10] = NULL;
	theLog[LOG_SERIAL].AddBuf(_T("SEND %s"), a);
#endif
}

void CDeviceSerialServoTorque::SendCommand_Reference()
{
	if(IsOpen() == FALSE) return;
	BYTE buf[9] = {0x02, 0x30, 0x31, 0x57, 0x45, 0x52, 0, 0, 0x03};
	SendCommand(DEVICE_SERVOOTORQUE_PRESS_VALUE, buf, 9);
	
}

void CDeviceSerialServoTorque::SendCommand_Hold_Start()
{
	if(IsOpen() == FALSE) return;
	BYTE buf[10] = {0x02, 0x30, 0x31, 0x57, 0x48, 0x4F, 0x4C, 0, 0, 0x03};
	SendCommand(DEVICE_SERVOOTORQUE_PRESS_VALUE, buf, 10);
}

void CDeviceSerialServoTorque::SendCommand_Hold_Stop()
{
	if(IsOpen() == FALSE) return;
	BYTE buf[10] = {0x02, 0x30, 0x31, 0x57, 0x48, 0x52, 0x53, 0, 0, 0x03};
	SendCommand(DEVICE_SERVOOTORQUE_PRESS_VALUE, buf, 10);
}


double CDeviceSerialServoTorque::GetLoadValue()
{
	if(IsOpen() == FALSE) return ERR_VALUE;

	int nStxPos = m_Serial.m_QueueRead.GetSTXPos();
	int nEtxPos = m_Serial.m_QueueRead.GetETXPos();

	if ( nStxPos >= 0 && nEtxPos >= 0 )
	{
		BYTE buf[100];
		memset(buf, 0x00, 100);
		m_Serial.m_QueueRead.GetDataSTX_To_ETX(buf, 100);

		theLog[LOG_SERIAL].AddBuf(_T("RECV %s"), buf);

		return 0.;
	}
	else
	{
		m_Serial.m_QueueRead.Clear();
		return ERR_VALUE;
	}
}

void CDeviceSerialServoTorque::SetSerialName(CString strValue)
{
	m_strSerialName = strValue;
	m_Serial.m_serialMode = m_serialMode;
}