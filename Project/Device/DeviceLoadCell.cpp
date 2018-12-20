#include "StdAfx.h"
#include "DeviceLoadCell.h"
#include "CheetahApp.h"
#include "Etc/Etc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//kjpark 20161024 압력센서 읽기 위한 쓰레드 구현
CDeviceLoadCell::CDeviceLoadCell(UINT nSerialNO , CString strSerialName)
{
	m_strSerialName = strSerialName;
	m_nSerialNO = nSerialNO;
	m_serialMode = SERIAL_LABELPRINT;
	nCount =1;
	m_hVACMsgBufEvent = CreateEvent(NULL,FALSE,TRUE,_T("VACMsgBuf"));
	
}


CDeviceLoadCell::~CDeviceLoadCell(void)
{
	CloseHandle(m_hVACMsgBufEvent);	
}

BOOL CDeviceLoadCell::SerialOpen( short nPortNo )
{
	if(IsOpen()) return TRUE;

	CString str;
	BOOL bRet = FALSE;
	bRet = m_Serial.InitCommPort(nPortNo,CBR_19200,8,ONESTOPBIT,NOPARITY);
	
	return bRet;
}

void CDeviceLoadCell::SerialClose()
{
	if(IsOpen() == FALSE) return;

	m_Serial.ClosePort();
}

BOOL CDeviceLoadCell::IsOpen()
{
	return m_Serial.m_bConnected;
}

void CDeviceLoadCell::SendCommand(DEVICE_LOADCELL_MODE mode, BYTE* buf, int nBufSize)
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
	
	
	CString strTemp;
	strTemp.Format(_T("%d___Send"), nCount);
// 	TRACE(strTemp);	// Trace 계속 갱신되서 주석 
	//theLog[LOG_SERIAL].AddBuf(_T("SEND %s"), a);
	//theLog[LOG_SERIAL].AddBuf(_T("SEND %s"), strTemp);
	nCount++;
}

void CDeviceLoadCell::SendCommand_PressValue()
{
	if(IsOpen() == FALSE) return;
	BYTE buf[10] = {0x02, 0x30, 0x31, 0x52, 0x43, 0x57, 0x54, 0, 0, 0x03};
#if 1
	buf[1] = buf[1] + nCount;
	SendCommand(DEVICE_LOADCELL_PRESS_VALUE, buf, 10);
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

void CDeviceLoadCell::SendCommand_Reference()
{
	if(IsOpen() == FALSE) return;
	BYTE buf[9] = {0x02, 0x30, 0x31, 0x57, 0x45, 0x52, 0, 0, 0x03};
	SendCommand(DEVICE_LOADCELL_PRESS_VALUE, buf, 9);
	
}

void CDeviceLoadCell::SendCommand_Hold_Start()
{
	if(IsOpen() == FALSE) return;
	BYTE buf[10] = {0x02, 0x30, 0x31, 0x57, 0x48, 0x4F, 0x4C, 0, 0, 0x03};
	SendCommand(DEVICE_LOADCELL_PRESS_VALUE, buf, 10);
}

void CDeviceLoadCell::SendCommand_Hold_Stop()
{
	if(IsOpen() == FALSE) return;
	BYTE buf[10] = {0x02, 0x30, 0x31, 0x57, 0x48, 0x52, 0x53, 0, 0, 0x03};
	SendCommand(DEVICE_LOADCELL_PRESS_VALUE, buf, 10);
}


//kjpark 20161024 압력센서 읽기 위한 쓰레드 구현
double CDeviceLoadCell::GetLoadValue(CString &strValue)
{
	strValue.Empty();
	//strValue = _T("dfghjk");
	if(IsOpen() == FALSE) return ERR_VALUE;

	int nStxPos = m_Serial.m_QueueRead.GetSTXPos();
	int nEtxPos = m_Serial.m_QueueRead.GetETXPos();
	
	if ( nStxPos >= 0 && nEtxPos >= 0 )
	{
		BYTE buf[100];
		memset(buf, 0x00, 100);
		m_Serial.m_QueueRead.GetDataSTX_To_ETX(buf, 100);

		//theLog[LOG_SERIAL].AddBuf(_T("RECV %s"), buf);

		int nBufferSize = nEtxPos-nStxPos;
		CString strGetData, strGetDatas;
		wchar_t wszMsg[SERIAL_MAX_BUFFER];
		MultiByteToWideChar(CP_ACP, 0, (char*)buf, nBufferSize, wszMsg, SERIAL_MAX_BUFFER);

		strGetData.Format(_T("%s"), wszMsg);
		strGetDatas = strGetData.Mid(0, nBufferSize);
		strValue = strGetDatas;
		SYSTEMTIME	time;
		CString strLogHeader;
		GetLocalTime(&time);
		strLogHeader.Format(_T("%02u:%02u:%02u:%03u\t__ThreadWatchComm\n"), time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
		TRACE(strLogHeader);

		SERIALEvent* pMsg = new SERIALEvent;
		pMsg->message = m_serialMode;
		pMsg->pParam = (LPARAM)buf;
		pMsg->nParam1 = nBufferSize;
		//::PostMessage(GetMainWnd(), WM_SERIAL, NULL, (LPARAM)pMsg);		
		Sleep(10);

		//SendCommand_PressValue();
		return 0.;
	}
	else
	{
		m_Serial.m_QueueRead.Clear();
		return ERR_VALUE;
	}
}

void CDeviceLoadCell::SetSerialName(CString strValue)
{
	m_strSerialName = strValue;
	m_Serial.m_serialMode = m_serialMode;
}
