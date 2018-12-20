#include "StdAfx.h"
#include "DeviceSerialPrinter.h"
#include "CheetahApp.h"
#include "Etc/Etc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDeviceSerialPrinter::CDeviceSerialPrinter(CString strSerialName)
{
	m_strSerialName = strSerialName;
//kjpark 20161010 �������� PG PC ���� ���	
#ifdef OPERATOR_PC
	m_serialMode = SERIAL_LABELPRINT;
#else
	m_serialMode = SERIAL_MAX;
#endif
}


CDeviceSerialPrinter::~CDeviceSerialPrinter(void)
{
}

BOOL CDeviceSerialPrinter::SerialOpen( short nPortNo )
{
	if(IsOpen()) return TRUE;

	CString str;

	return m_Serial.InitCommPort(nPortNo,CBR_19200,8,ONESTOPBIT,NOPARITY);
}

void CDeviceSerialPrinter::SerialClose()
{
	if(IsOpen() == FALSE) return;

	m_Serial.ClosePort();
}

BOOL CDeviceSerialPrinter::IsOpen()
{
	return m_Serial.m_bConnected;
}

void CDeviceSerialPrinter::SendCommand(DEVICE_PRINETR_MODE mode, BYTE* buf, int nBufSize)
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

void CDeviceSerialPrinter::SendCommand_PressValue()
{
	if(IsOpen() == FALSE) return;
	BYTE buf[10] = {0x02, 0x30, 0x31, 0x52, 0x43, 0x57, 0x54, 0, 0, 0x03};
#if 1
	SendCommand(DEVICE_PRINETR_PRESS_VALUE, buf, 10);
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

void CDeviceSerialPrinter::SendCommand_Reference()
{
	if(IsOpen() == FALSE) return;
	BYTE buf[9] = {0x02, 0x30, 0x31, 0x57, 0x45, 0x52, 0, 0, 0x03};
	SendCommand(DEVICE_PRINETR_PRESS_VALUE, buf, 9);
	
}

void CDeviceSerialPrinter::SendCommand_Hold_Start()
{
	if(IsOpen() == FALSE) return;
	BYTE buf[10] = {0x02, 0x30, 0x31, 0x57, 0x48, 0x4F, 0x4C, 0, 0, 0x03};
	SendCommand(DEVICE_PRINETR_PRESS_VALUE, buf, 10);
}

void CDeviceSerialPrinter::SendCommand_Hold_Stop()
{
	if(IsOpen() == FALSE) return;
	BYTE buf[10] = {0x02, 0x30, 0x31, 0x57, 0x48, 0x52, 0x53, 0, 0, 0x03};
	SendCommand(DEVICE_PRINETR_PRESS_VALUE, buf, 10);
}


double CDeviceSerialPrinter::GetLoadValue()
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

void CDeviceSerialPrinter::SetSerialName(CString strValue)
{
	m_strSerialName = strValue;
	m_Serial.m_serialMode = m_serialMode;
}

//kjpark 20161004 Label Printer IF �߰�
BOOL CDeviceSerialPrinter::TestBarcodePrint(CString sMsg)
{
	// ��Ʈ�� ���� ��쿡�� ����Ѵ�.
	if ( m_Serial.m_bConnected != TRUE ) return 0;

	CString sSendMsg;
	TCHAR cTCCmd[eMAX_PrinterBuffer];
	char	cSendData[eMAX_PrinterBuffer];


	int nLen = 0;
	int nSize = 3;
	int nPosX = 140;
	int nPosY = 0;

	// Label Printer�� �´� ��ɹ��� ����� �ش�.
	sSendMsg.Format( _T("B2%d,%d,D,%d,N,\'%s\'%c%c"), nPosX, nPosY, nSize, sMsg, 0x0d, 0x0a);		

	nLen = sSendMsg.GetLength();				// ������ ���� ��

	_tcscpy_s(cTCCmd,	_countof(cTCCmd),	sSendMsg);	// CString -> TCHAR�κ�ȯ
	::WideCharToMultiByte(CP_ACP, NULL, cTCCmd, -1, cSendData, eMAX_PrinterBuffer, NULL, FALSE);		// TCHAR -> char�� ��ȯ

	return  m_Serial.WriteComm( (BYTE*)cSendData, strlen( cSendData ) );// nLen );
}

//kjpark 20161004 Label Printer IF �߰�
// Test����� �Ѵ�.
BOOL CDeviceSerialPrinter::TestPrintOut()
{
	CString sMsg;
	CString sDefect = _T("Test Print"); 

	sMsg.Format(_T("SM%d,%d"), 130, 40);
	LabelPrintOut(-1, sMsg);						// ��/�� ���� ����

	LabelPrintOut(0, sDefect);						// Cell ID ���
	LabelPrintOut(1, _T("U123456")) ;				// �ҷ� �ڵ�
	LabelPrintOut(2, _T("Label Test Print") );		// �ҷ���

	CTime dateTm = CTime::GetCurrentTime();
	CString strLineMsg;

	strLineMsg.Format(_T("%04d%02d%02d_LAVEL_PRINTER_TEST"),dateTm.GetYear(), dateTm.GetMonth(),dateTm.GetDay());						// �ð� + LIne��;

	LabelPrintOut(3, strLineMsg );		// �ð�+Line��

	TestBarcodePrint(_T("A2EG1S53DOKAD024"));			// 2���� ���ڵ� ���


	return LabelPrintOut(-1, _T("P1") ) > 0;					// ���� ��� ���
}

//kjpark 20161004 Label Printer IF �߰�
DWORD CDeviceSerialPrinter::LabelPrintOut(int nLn, CString sMsg)
{
	// ��Ʈ�� ���� ��쿡�� ����Ѵ�.
	if ( m_Serial.m_bConnected != TRUE ) return 0;

	CString sSendMsg;
	TCHAR cTCCmd[eMAX_PrinterBuffer];
	char	cSendData[eMAX_PrinterBuffer];


	int nLen = 0;
	int nHeight = 0;
	int nGap = 0;

	if ( nLn < 0 )			// �� ��ȣ�� �������, Ư�� ���� ����̴�.
	{
		sSendMsg.Format( _T("%s%c%c"), sMsg, 0x0d, 0x0a);				// �ڿ� CR/LF�� �ٿ��ش�.
	}
	else
	{
		switch ( nLn )
		{
		case 0 : nHeight = 0;		break;
		case 1 : nHeight = 20;		break;
		case 2 : nHeight = 40;		break;
		case 3 : nHeight = 60;		break;
		case 4 : nHeight = 80;		break;
		default :
			nHeight = nLn * 20;
		}//of switch

		// Label Printer�� �´� ��ɹ��� ����� �ش�.
		sSendMsg.Format( _T("T%d,%d,c,1,1,0,0,N,N,\'%s\'%c%c"), nGap, nHeight, sMsg, 0x0d, 0x0a);		
	}//of else


	nLen = sSendMsg.GetLength();				// ������ ���� ��

	_tcscpy_s(cTCCmd,	_countof(cTCCmd),	sSendMsg);	// CString -> TCHAR�κ�ȯ
	::WideCharToMultiByte(CP_ACP, NULL, cTCCmd, -1, cSendData, eMAX_PrinterBuffer, NULL, FALSE);		// TCHAR -> char�� ��ȯ

	return  m_Serial.WriteComm( (BYTE*)cSendData, strlen( cSendData ) );// nLen );
}