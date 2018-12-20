#include "StdAfx.h"
#include "DeviceSerialTemperature.h"
#include "CheetahApp.h"
#include "Etc/Etc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDeviceSerialTemperature::CDeviceSerialTemperature(CString strSerialName)
{
	m_strSerialName = strSerialName;
	m_serialMode = SERIAL_TEMPERATURE;

	m_Serial.m_QueueRead.SetSTX(STX);
	m_Serial.m_QueueRead.SetETX(ETX);

	for (int i = 0; i < eMAX_TEMPCount; i++) 
	{
		m_dbTempValue[i]	= 0.0;	
		m_nTempAverageCount[i]	= 0;	
		m_dSumTempValue[i]	= 0.0;	
	}

	// Ƣ�� ����� �����͸� �����ϱ� ���� ��Ÿ�� ���
	m_nBoundLimit		= 3;						// ��ȸ �̻� ��/���� �������� �ݺ��Ǿ��� ������ ���ΰ� ?
	m_nUBoundCount		= 0;						// �������� Ƣ�� �� ���� �߻� Ƚ��
	m_nLBoundCount		= 0;						// ���Ѱ� ������ Ƣ�� �� ���� �߻� Ƚ��
	m_dbUBoundLimit	= 10;						// �������� Ƣ�� �� ����
	m_dbLBoundLimit	= 40;						// �������� Ƣ�� �� ����

	m_nDeviceAddr = 1;
	
}


CDeviceSerialTemperature::~CDeviceSerialTemperature(void)
{
}

BOOL CDeviceSerialTemperature::SerialOpen( short nPortNo )
{
	if(IsOpen()) return TRUE;

	return m_Serial.InitCommPort(nPortNo,CBR_9600,8,ONESTOPBIT,NOPARITY);
}

void CDeviceSerialTemperature::SerialClose()
{
	if(IsOpen() == FALSE) return;

	m_Serial.ClosePort();
}

BOOL CDeviceSerialTemperature::IsOpen()
{
	return m_Serial.m_bConnected;
}

void CDeviceSerialTemperature::SendCommand(DEVICE_TEMPERATURE_MODE mode, BYTE* buf, int nBufSize)
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

void CDeviceSerialTemperature::SendCommand_TempValue(int nDeviceNum)
{
	// �ʿ��� �����͸� �����Ѵ�.
	BYTE	bySend[32];
	int		nLen = 0;

	CString sAddr;
	sAddr.Format(_T("%02d"),  nDeviceNum);

	bySend[nLen++]	= 0x02;				// STX
	bySend[nLen++]	=  (BYTE)sAddr.GetAt(0);	//'0';				// Node, 2
	bySend[nLen++]	=  (BYTE)sAddr.GetAt(1);	//'6';

	bySend[nLen++]	= '0';				// Sub-Addr, 2
	bySend[nLen++]	= '0';

	bySend[nLen++]	= '0';				// SID, 1

	bySend[nLen++]	= '0';				// MRC, 2
	bySend[nLen++]	= '1';

	bySend[nLen++]	= '0';				// SRC, 2
	bySend[nLen++]	= '1';

	bySend[nLen++]	= 'C';				// VarType, 2
	bySend[nLen++]	= '0';

	bySend[nLen++]	= '0';				// Read-Addr, 4, �����͸� �о� �� �ּ�
	bySend[nLen++]	= '0';
	bySend[nLen++]	= '0';
	bySend[nLen++]	= '0';

	bySend[nLen++]	= '0';				// Bit-Pos, 2
	bySend[nLen++]	= '0';

	bySend[nLen++]	= '0';				// Number-Elements, 4, 1���� �����͸� �о�´�.
	bySend[nLen++]	= '0';
	bySend[nLen++]	= '0';
	bySend[nLen++]	= '1';

	bySend[nLen++]	= 0x03;				// ETX

	bySend[nLen++]	=  GetChecksum(bySend, nLen);		// Checksum�� �����.
	
	// COM Port�� �����Ѵ�.
	int nSendLen =  m_Serial.WriteComm( bySend, nLen );

}
// Checksum�� ���Ѵ�.
// XOR�� ���� 2�� ������ ������ش�.
BYTE CDeviceSerialTemperature::GetChecksum(BYTE* pData, int nLen)
{
	BYTE bySum = 0;

	// STX�� ETX�� �����ϰ� ����Ѵ�.
	for (int i = 1; i < nLen; i++) 
	{
		// Data Sum ������ �ƴ�, bySum = (bySum + pData[i]) & 0xFF;			// �ջ� ����

		bySum = bySum ^ pData[i];			// XOR ����
	}

	return bySum;							// ���� ������ ����
}
void CDeviceSerialTemperature::SendCommand_Reference()
{
	if(IsOpen() == FALSE) return;
	BYTE buf[9] = {0x02, 0x30, 0x31, 0x57, 0x45, 0x52, 0, 0, 0x03};
	SendCommand(DEVICE_TEMPERATURE_PRESS_VALUE, buf, 9);
	
}

void CDeviceSerialTemperature::SendCommand_Hold_Start()
{
	if(IsOpen() == FALSE) return;
	BYTE buf[10] = {0x02, 0x30, 0x31, 0x57, 0x48, 0x4F, 0x4C, 0, 0, 0x03};
	SendCommand(DEVICE_TEMPERATURE_PRESS_VALUE, buf, 10);
}

void CDeviceSerialTemperature::SendCommand_Hold_Stop()
{
	if(IsOpen() == FALSE) return;
	BYTE buf[10] = {0x02, 0x30, 0x31, 0x57, 0x48, 0x52, 0x53, 0, 0, 0x03};
	SendCommand(DEVICE_TEMPERATURE_PRESS_VALUE, buf, 10);
}

BOOL CDeviceSerialTemperature::ParsingReciveValue()
{
	if(IsOpen() == FALSE) return FALSE;

	BYTE buf[200];
	BYTE buf1[200];
	BYTE buf2[200];
	WCHAR a[200];
	int nReciveLength = m_Serial.m_QueueRead.GetDataSTX_To_ETX(buf2, 200);
	int nStartPos = 0;
	int nEndPos = 0;
	BOOL bCheckFlag = FALSE;	

	//2017-12-29,SKCHO, �߰�
	if(nReciveLength < 1 || nReciveLength >= 200)
	{	
		return FALSE;
	}
	
	buf[nReciveLength] = NULL;	

	//2017-12-27,SKCHO, LOG��� �߰�
	CEtc::ConvertUnicode((char*)buf2, nReciveLength, a, 100);
	a[nReciveLength] = NULL;	
	theLog[LOG_SERIAL].AddBuf(_T("RECIVE Length %d - %s"), nReciveLength,a);

	//[ 0100000101000000000019 ]
	if ( nReciveLength > 0 )			// ���ŵ� �����Ͱ� �ִ°� ?
	{
		int nPos = 0;
		long lReadValue = 0;

		for (int i = 0; i < nReciveLength; i++)
		{
			// ETX�� ���ö� ���� �˻��Ͽ� ó���Ѵ�.
			if (buf2[i] == 0x02 )		// ���� ���� ETX�� �����ϴ°� ?
			{
				nStartPos = 0;			// ���� ���� ��ġ
				//break;
			}
			buf1[nStartPos++] = buf2[i];

			if(nStartPos > 0 && buf2[i] == 0x03)
			{
				for(int j = 0; j < nStartPos; j++)
				{
					buf[j] = buf1[j];
					
				}
				nPos = nStartPos;
				break;
			}

		}// for


		// ���Ṯ�� ETX�� �����ϴ°� ?
		if ( nPos > 0 )
		{
			// �ʱ� ���ڰ� STX�̰�, �����ͱ��̰�  23����Ʈ�� �Ѵ°� ?
			if ( (nPos >= 23) && (buf[0] == 0x02) )		// �������� �������ΰ� ?
			{
				// �������� �������� Ȯ��
				if (	(buf[ 5]	= '0') &&		// End Code�� '00' �ΰ� ?
						(buf[ 6]	= '0') &&

						(buf[11]	= '0') &&		// Response Code�� '0000'�ΰ� ?
						(buf[12]	= '0') &&
						(buf[13]	= '0') &&
						(buf[14]	= '0') )
				{
					char szValue[6];			// ���ڷ� ��Ȱ �� ���� �Ҵ�
					TCHAR szMsg[6] = { 0, };
					double dbValue = 0.0;		// �ӽ� ��

					// Data �κи� ���Ѵ�.
					szValue[0] = buf[19];
					szValue[1] = buf[20];
					szValue[2] = buf[21];
					szValue[3] = buf[22];
					szValue[4] = NULL;					// �� ����

					lReadValue = strtol(szValue, NULL, 16);								// 16�������ڿ��� ���������� ��ȯ

					for(int i = 0; i < 5; i++) szMsg[i] = szValue[i];
					szMsg[4] = 0x00;
					m_sRcvData[0].Format(_T("%s"), szMsg);							// ���ŵ� ������ ��ü�� ǥ��

					dbValue = lReadValue * 1.0;										// �µ��� ���

					// Ƣ�� �� ����ִ� �κ�

					// ���������� Ƣ�� ���
					if ( dbValue >= m_dbUBoundLimit )								
					{
						if ( m_nUBoundCount > m_nBoundLimit )						// ������ Ƚ���̻� �������� ���Դٸ� 
						{
							m_dbTempValue[m_nDeviceAddr-1] = dbValue;								// �ش簪�� �����ϰ� �޾Ƶ��δ�.
						    bCheckFlag = TRUE;
						}
						else
						{
							m_nUBoundCount++;
						}// �������� ��� �� Ƚ���� ������Ų��.
					}
					else
					{
						if ( dbValue < m_dbLBoundLimit )							// ���� ���Ѱ� �Ʒ��� ���Դ�.
						{
							if ( m_nLBoundCount > m_nBoundLimit )					// ������ Ƚ���̻� �������� ���Դٸ� 
							{
								m_dbTempValue[m_nDeviceAddr-1] = dbValue;							// �ش簪�� �����ϰ� �޾Ƶ��δ�.
							    bCheckFlag = TRUE;
							}
							else
							{
								m_nLBoundCount++;									// �������� ��� �� Ƚ���� ������Ų��.
							}
						}
						else		// �����ȿ� ������ ���̴�.
						{
							m_dbTempValue[m_nDeviceAddr-1] = dbValue;							// �ش簪�� �����ϰ� �޾Ƶ��δ�.
						    bCheckFlag = TRUE;
							m_nUBoundCount = 0;
							m_nLBoundCount = 0;
						}	
					}

				}//of if �����ڵ尡 �������̶��

				//m_Serial.m_QueueRead.Clear();
			}//of Correct Data		
		
		}//of if CR Received ?						// ó���Ϸ� 
	    if(bCheckFlag == TRUE)
		{
            bCheckFlag = FALSE;
			//IncDeviceAddr();
		}
	}

	// ���̻� ó���Ұ� ������ TRUE [12/7/2016 OSC]
	if(m_Serial.m_QueueRead.GetDataSTX_To_ETX() <= 0)
		return TRUE;

	return FALSE;
}
//�ּҰ� ����
void CDeviceSerialTemperature::IncDeviceAddr(void)
{
	//Receive�� �����ϰ�  Send �� �ϱ⶧���� 1-6���� �����ϰ�
	if(m_nDeviceAddr < eMAX_TEMPCount)
	{
		m_nDeviceAddr++;
	}
	else
	{
		m_nDeviceAddr = 1;  //6���� Receive�ϸ� 1�� �ʱ�ȭ
	}

}
// ���� ������ ���� ���� �о�´�.
double CDeviceSerialTemperature::GetTempValue(int nIdx)
{
	if ( (nIdx < 0) || (nIdx >= eMAX_TEMPCount) ) return 0.0;

	return m_dbTempValue[nIdx];	
}
void CDeviceSerialTemperature::SetSerialName(CString strValue)
{
	m_strSerialName = strValue;
	m_Serial.m_serialMode = m_serialMode;
}
void CDeviceSerialTemperature::TempAverageValue(int nIdx, int nAvgNum)
{
	double dValue = 24;
	if(nAvgNum < 1) return;

	if(m_nTempAverageCount[nIdx]  < nAvgNum)
	{

		m_dSumTempValue[nIdx] += GetTempValue(nIdx);
		m_nTempAverageCount[nIdx]++;
	}
	else
	{
		dValue = (double)(m_dSumTempValue[nIdx]/nAvgNum);
		if(eIDX_TEMP_PCRACK	== nIdx)				// PC-RACK �µ� ����
		{
			if(dValue < 23 || dValue >= 28)
			{
				theFDCBank.m_dPc_Rack_Temp = 24;
			}
			else
			{
				theFDCBank.m_dPc_Rack_Temp = dValue;
			}
		}
		else if(eIDX_TEMP_LOAD== nIdx)
		{
			if(dValue < 23 || dValue >= 28)
			{
				theFDCBank.m_dLoad_Temp = 24;
			}
			else
			{
				theFDCBank.m_dLoad_Temp = dValue;
			}
		}
		else if(eIDX_TEMP_UPS== nIdx)
		{
			if(dValue < 23 || dValue >= 28)
			{
				theFDCBank.m_dUps_Temp = 24;
			}
			else
			{
				theFDCBank.m_dUps_Temp = dValue;
			}
		}
		else if(eIDX_TEMP_UNLOAD== nIdx)
		{
			if(dValue < 23 || dValue >= 28)
			{
				theFDCBank.m_dUnload_Temp = 24;
			}
			else
			{
				theFDCBank.m_dUnload_Temp = dValue;
			}
		}
		else if(eIDX_TEMP_GPS== nIdx)
		{
			if(dValue < 23 || dValue >= 28)
			{
				theFDCBank.m_dGps_Temp = 24;
			}
			else
			{
				theFDCBank.m_dGps_Temp = dValue;
			}
		}
		else if(eIDX_TEMP_DRIVEBOX== nIdx)
		{
			if(dValue < 23 || dValue >= 28)
			{
				theFDCBank.m_dGps_Temp = 24;
			}
			else
			{
				theFDCBank.m_dGps_Temp = dValue;
			}
		}

		m_dSumTempValue[nIdx] = 0;
		m_nTempAverageCount[nIdx] = 0;
	}

	if(theFDCBank.m_dPc_Rack_Temp == 0)
	{
		theFDCBank.m_dPc_Rack_Temp = 25;
	}
	
}