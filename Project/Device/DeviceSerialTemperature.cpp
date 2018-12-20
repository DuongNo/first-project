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

	// 튀는 노이즈성 데이터를 방지하기 위한 울타리 경계
	m_nBoundLimit		= 3;						// 몇회 이상 상/하한 기준으로 반복되어어야 인정할 것인가 ?
	m_nUBoundCount		= 0;						// 상한으로 튀는 값 연속 발생 횟수
	m_nLBoundCount		= 0;						// 하한값 밑으로 튀는 값 연속 발생 횟수
	m_dbUBoundLimit	= 10;						// 상한으로 튀는 값 기준
	m_dbLBoundLimit	= 40;						// 하한으로 튀는 값 기준

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
	// 필요한 데이터만 전송한다.
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

	bySend[nLen++]	= '0';				// Read-Addr, 4, 데이터를 읽어 올 주소
	bySend[nLen++]	= '0';
	bySend[nLen++]	= '0';
	bySend[nLen++]	= '0';

	bySend[nLen++]	= '0';				// Bit-Pos, 2
	bySend[nLen++]	= '0';

	bySend[nLen++]	= '0';				// Number-Elements, 4, 1개의 데이터를 읽어온다.
	bySend[nLen++]	= '0';
	bySend[nLen++]	= '0';
	bySend[nLen++]	= '1';

	bySend[nLen++]	= 0x03;				// ETX

	bySend[nLen++]	=  GetChecksum(bySend, nLen);		// Checksum을 만든다.
	
	// COM Port로 전송한다.
	int nSendLen =  m_Serial.WriteComm( bySend, nLen );

}
// Checksum을 구한다.
// XOR한 값에 2의 보수를 만들어준다.
BYTE CDeviceSerialTemperature::GetChecksum(BYTE* pData, int nLen)
{
	BYTE bySum = 0;

	// STX와 ETX를 제외하고 계산한다.
	for (int i = 1; i < nLen; i++) 
	{
		// Data Sum 연산이 아님, bySum = (bySum + pData[i]) & 0xFF;			// 합산 수행

		bySum = bySum ^ pData[i];			// XOR 수행
	}

	return bySum;							// 계산된 데이터 전송
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

	//2017-12-29,SKCHO, 추가
	if(nReciveLength < 1 || nReciveLength >= 200)
	{	
		return FALSE;
	}
	
	buf[nReciveLength] = NULL;	

	//2017-12-27,SKCHO, LOG기록 추가
	CEtc::ConvertUnicode((char*)buf2, nReciveLength, a, 100);
	a[nReciveLength] = NULL;	
	theLog[LOG_SERIAL].AddBuf(_T("RECIVE Length %d - %s"), nReciveLength,a);

	//[ 0100000101000000000019 ]
	if ( nReciveLength > 0 )			// 수신된 데이터가 있는가 ?
	{
		int nPos = 0;
		long lReadValue = 0;

		for (int i = 0; i < nReciveLength; i++)
		{
			// ETX가 나올때 까지 검색하여 처리한다.
			if (buf2[i] == 0x02 )		// 종료 문자 ETX가 존재하는가 ?
			{
				nStartPos = 0;			// 종료 문자 위치
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


		// 종료문자 ETX가 존재하는가 ?
		if ( nPos > 0 )
		{
			// 초기 문자가 STX이고, 데이터길이가  23바이트가 넘는가 ?
			if ( (nPos >= 23) && (buf[0] == 0x02) )		// 정상적인 데이터인가 ?
			{
				// 정상적인 응답인지 확인
				if (	(buf[ 5]	= '0') &&		// End Code가 '00' 인가 ?
						(buf[ 6]	= '0') &&

						(buf[11]	= '0') &&		// Response Code가 '0000'인가 ?
						(buf[12]	= '0') &&
						(buf[13]	= '0') &&
						(buf[14]	= '0') )
				{
					char szValue[6];			// 문자로 변활 할 공간 할당
					TCHAR szMsg[6] = { 0, };
					double dbValue = 0.0;		// 임시 값

					// Data 부분만 취한다.
					szValue[0] = buf[19];
					szValue[1] = buf[20];
					szValue[2] = buf[21];
					szValue[3] = buf[22];
					szValue[4] = NULL;					// 널 문자

					lReadValue = strtol(szValue, NULL, 16);								// 16진수문자열을 정수형으로 변환

					for(int i = 0; i < 5; i++) szMsg[i] = szValue[i];
					szMsg[4] = 0x00;
					m_sRcvData[0].Format(_T("%s"), szMsg);							// 수신된 데이터 자체를 표시

					dbValue = lReadValue * 1.0;										// 온도값 취득

					// 튀는 값 잡아주는 부분

					// 높은값으로 튀는 경우
					if ( dbValue >= m_dbUBoundLimit )								
					{
						if ( m_nUBoundCount > m_nBoundLimit )						// 지정된 횟수이상 연속으로 들어왔다면 
						{
							m_dbTempValue[m_nDeviceAddr-1] = dbValue;								// 해당값을 인정하고 받아들인다.
						    bCheckFlag = TRUE;
						}
						else
						{
							m_nUBoundCount++;
						}// 연속으로 들어 온 횟수를 증가시킨다.
					}
					else
					{
						if ( dbValue < m_dbLBoundLimit )							// 지정 하한값 아래로 들어왔다.
						{
							if ( m_nLBoundCount > m_nBoundLimit )					// 지정된 횟수이상 연속으로 들어왔다면 
							{
								m_dbTempValue[m_nDeviceAddr-1] = dbValue;							// 해당값을 인정하고 받아들인다.
							    bCheckFlag = TRUE;
							}
							else
							{
								m_nLBoundCount++;									// 연속으로 들어 온 횟수를 증가시킨다.
							}
						}
						else		// 범위안에 들어오는 값이다.
						{
							m_dbTempValue[m_nDeviceAddr-1] = dbValue;							// 해당값을 인정하고 받아들인다.
						    bCheckFlag = TRUE;
							m_nUBoundCount = 0;
							m_nLBoundCount = 0;
						}	
					}

				}//of if 응답코드가 정상적이라면

				//m_Serial.m_QueueRead.Clear();
			}//of Correct Data		
		
		}//of if CR Received ?						// 처리완료 
	    if(bCheckFlag == TRUE)
		{
            bCheckFlag = FALSE;
			//IncDeviceAddr();
		}
	}

	// 더이상 처리할게 없으면 TRUE [12/7/2016 OSC]
	if(m_Serial.m_QueueRead.GetDataSTX_To_ETX() <= 0)
		return TRUE;

	return FALSE;
}
//주소값 증가
void CDeviceSerialTemperature::IncDeviceAddr(void)
{
	//Receive를 먼저하고  Send 를 하기때문에 1-6까지 증가하고
	if(m_nDeviceAddr < eMAX_TEMPCount)
	{
		m_nDeviceAddr++;
	}
	else
	{
		m_nDeviceAddr = 1;  //6에서 Receive하면 1로 초기화
	}

}
// 지정 순번의 공압 값을 읽어온다.
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
		if(eIDX_TEMP_PCRACK	== nIdx)				// PC-RACK 온도 센서
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