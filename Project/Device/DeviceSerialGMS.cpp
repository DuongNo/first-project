#include "StdAfx.h"
#include "DeviceSerialGMS.h"
#include "CheetahApp.h"
#include "Etc/Etc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDeviceSerialGMS::CDeviceSerialGMS(CString strSerialName)
{
	m_strSerialName = strSerialName;
	m_Serial.m_QueueRead.SetSTX(DEL);
	m_Serial.m_QueueRead.SetETX(ETX);

	for(int nGmsIndex = 0; nGmsIndex < GMS_SUB_CH_MAX; nGmsIndex++)
	{
		//5ch�� ������� �����Ƿ� DISABLE
		if(nGmsIndex == GMS_SUB_CH5)
		{
			m_LastGMSValue[nGmsIndex] = GMS_VALUE_CH_DISABLE;
		}
		else
		{
			m_LastGMSValue[nGmsIndex] = _T("0");
		}
	}
	m_LastBZonePGMS = GMS_VALUE_CH_DISABLE;
	m_LastDZonePGMS = GMS_VALUE_CH_DISABLE;

	//2017-08-31,SKCHO ���� �ʱ�ȭ
	m_nGMSAverageCount = 0;
	memset(&m_dSumGMSValue,0,sizeof(m_dSumGMSValue));

}


CDeviceSerialGMS::~CDeviceSerialGMS(void)
{
}

BOOL CDeviceSerialGMS::SerialOpen( short nPortNo )
{
	if(IsOpen()) return TRUE;


	//return m_Serial.OpenPort(nPortNo,CBR_19200,8,ONESTOPBIT,NOPARITY);
	return m_Serial.InitCommPort(nPortNo,CBR_19200,8,ONESTOPBIT,NOPARITY);
}

void CDeviceSerialGMS::SerialClose()
{
	if(IsOpen() == FALSE) return;

	m_Serial.ClosePort();
}

BOOL CDeviceSerialGMS::IsOpen()
{
	return m_Serial.m_bConnected;
}

void CDeviceSerialGMS::SendCommand_Read(GMS_MODULE_CH ch)
{
	// LOADWS   [0x10][0x02]FORCE_TOUCH_CH001_GMS__123402[0x03]
	// LOADWS   [0x10][0x02]FORCE_TOUCH_CH003_PGMS_12340?[0x03]
	CStringA strBody, strPacket;
	switch(ch)
	{
		case GMS_CH001_GMS: 
			strBody.Format("%c%c%s%s%c", DEL, STX, "LOADWS   ", GMS_NICKNAME_JIG_GMS, PGMS_BUZZER_OFF);
			break;
		case GMS_CH002_PGMS: 
			strBody.Format("%c%c%s%s%c", DEL, STX, "LOADWS   ", GMS_NICKNAME_BZONE_PGMS, PGMS_BUZZER_OFF);
			break;
		case GMS_CH003_PGMS: 
			strBody.Format("%c%c%s%s%c", DEL, STX, "LOADWS   ", GMS_NICKNAME_DZONE_PGMS, PGMS_BUZZER_OFF);
			break;
	}
	strPacket.Format("%s%c%c", strBody, GetChecksum((BYTE*)strBody.GetString(), strBody.GetLength()), ETX);
	int nLength = strPacket.GetLength();

	m_Serial.WriteComm((BYTE*)strPacket.GetString(), nLength);

// 	WCHAR a[100];
// 	CEtc::ConvertUnicode((char*)strPacket.GetString(), nLength, a, 100);
// 	a[nLength] = NULL;
// 	theLog[LOG_SERIAL_GMS].AddBuf(_T("SEND %s"), a);
}

// Checksum�� ���Ѵ�.
// XOR�� ���� 2�� ������ ������ش�.
BYTE CDeviceSerialGMS::GetChecksum(BYTE* pData, int nLen)
{
	BYTE bySum = pData[2];

	// DLE, STX�� �����ϰ� ����Ѵ�.
	for (int i = 3; i < nLen; i++) 
	{
		bySum = bySum ^ pData[i];			// XOR ����
	}

	// ���� Check sum ���� Ư���� ���� ������ 0xff�� ��ü�Ѵ�.
	if( (bySum == 0x00) || (bySum == 0x10) || (bySum == 0x0d) )
	{
		bySum = 0xff;
	}

	return bySum;							// ���� ������ ����
}

BOOL CDeviceSerialGMS::ParsingReciveValue()
{
	if(IsOpen() == FALSE) return FALSE;

	BYTE buf[100];
	CString str;
	WCHAR a[100];


	int nReciveLength = m_Serial.m_QueueRead.GetDataSTX_To_ETX(buf, 100);
	
	if(nReciveLength < 1)	return FALSE;
	
	//2017-07-08,SKCHO, ��ŵ����Ͱ� 100�� �ϰ�� ���α׷� �ٿ� ������ �־� �Ʒ��� �̵�
	//--buf[nReciveLength] = NULL;
	//theLog[LOG_SERIAL_GMS].AddBuf(_T("RECIVE Length [%d] - Data [%s]"), nReciveLength, buf);
	
	if(nReciveLength != 60)
	{
		theLog[LOG_SERIAL_GMS].AddBuf(_T("RECIVE Length ERR %d"), nReciveLength);
		return FALSE;
	}

    buf[nReciveLength] = NULL;
	//2017-12-27,SKCHO, LOG��� �߰�
	CEtc::ConvertUnicode((char*)buf, nReciveLength, a, 100);
	a[nReciveLength] = NULL;	
	theLog[LOG_SERIAL_GMS].AddBuf(_T("RECIVE Length %d - %s"), nReciveLength,a);

	// NickName Ȯ�� [12/7/2016 OSC]
	int nSize = strlen(GMS_NICKNAME_JIG_GMS);
	BYTE bufNickName[sizeof(GMS_NICKNAME_JIG_GMS)];
	memcpy(bufNickName, &buf[11], nSize);
	bufNickName[nSize] = NULL;
	if(strcmp((char *)bufNickName, GMS_NICKNAME_JIG_GMS) == 0)
	{
		// GMS
		char GMS_VALUE[GMS_SUB_CH_MAX][GMS_VALUE_SIZE+1];
		CStringA strValue;
		for(int i = 0; i < GMS_SUB_CH_MAX; i++)
		{
			memcpy(GMS_VALUE[i], &buf[ 38 + (i*GMS_VALUE_SIZE) ], GMS_VALUE_SIZE);
			GMS_VALUE[i][GMS_VALUE_SIZE] = NULL;
			strValue.Format("%s", GMS_VALUE[i]);
			// 999�� �����ϸ� ���װ��� �ƴ�
			if(strValue.Left(3) == GMS_VALUE_ERR)
			{
				//2017.07.22. JSJUNG. ���װ� �ƴѵ� �� ����Ǵ���?? ����
				//m_LastGMSValue[i] = strValue;
			}
			else
			{
				//2017-08-31,SKCHO, ��հ��� ���ϱ� ���ؼ� ���� �߰�
				//m_LastGMSValue[i].Format(_T("%f"), atoi(GMS_VALUE[i])/100.);	// 100�� ������ ������ �´�
			    m_dGMSValue[i] = (double)(atoi(GMS_VALUE[i])/100.);
			}
		}

		// GMS 5��ä���� ������� �ʴ´�. ������ CIM���� ������ 5���� �����ؾ� �Ѵ�. [12/16/2016 OSC]
		m_LastGMSValue[GMS_SUB_CH5] = GMS_VALUE_CH_DISABLE;
		m_LastBZonePGMS = GMS_VALUE_CH_DISABLE;
		m_LastDZonePGMS = GMS_VALUE_CH_DISABLE;

	}
	/*else if(strcmp((char *)bufNickName, GMS_NICKNAME_BZONE_PGMS) == 0)
	{
		// PGMS
		CStringA str;
		str.Format("%c", buf[57]);
		m_LastBZonePGMS.Format(_T("%d"), atoi(str));
// 		theLog[LOG_SERIAL_GMS].AddBuf(_T("RECV B ZONE PGMS %s"), m_LastBZonePGMS);
	}
	else if(strcmp((char *)bufNickName, GMS_NICKNAME_DZONE_PGMS) == 0)
	{
		// PGMS
		CStringA str;
		str.Format("%c", buf[57]);
		m_LastDZonePGMS.Format(_T("%d"), atoi(str));
// 		theLog[LOG_SERIAL_GMS].AddBuf(_T("RECV D ZONE PGMS %s"), m_LastDZonePGMS);
	}*/

	// ���̻� ó���Ұ� ������ TRUE [12/7/2016 OSC]
	if(m_Serial.m_QueueRead.GetDataSTX_To_ETX() <= 0)
		return TRUE;

	return FALSE;
}

void CDeviceSerialGMS::SetSerialName(CString strValue)
{
	m_strSerialName = strValue;
}

void CDeviceSerialGMS::AverageValue(int nAvgNum)
{
	double dGMS_MAX = theConfigBank.m_Option.m_dFDCRange[GMS_MAX];
	double dGMS_MIN = theConfigBank.m_Option.m_dFDCRange[GMS_MIN];
	double dGMSValue = 0;
	if(nAvgNum < 1) return;

	if(m_nGMSAverageCount < nAvgNum)
	{
		dGMSValue = m_dGMSValue[GMS_A_JIG];
		if(dGMSValue > dGMS_MAX || dGMSValue < dGMS_MIN)
		{
			dGMSValue = 0.1;   //����Ʈ��
		}
		m_dSumGMSValue[GMS_A_JIG] += dGMSValue;

		dGMSValue = m_dGMSValue[GMS_B_JIG];
		if(dGMSValue > dGMS_MAX || dGMSValue < dGMS_MIN)
		{
			dGMSValue = 0.1;   //����Ʈ��
		}
		m_dSumGMSValue[GMS_B_JIG] += dGMSValue;

		dGMSValue = m_dGMSValue[GMS_C_JIG];
		if(dGMSValue > dGMS_MAX || dGMSValue < dGMS_MIN)
		{
			dGMSValue = 0.1;   //����Ʈ��
		}
		m_dSumGMSValue[GMS_C_JIG] += dGMSValue;

		dGMSValue = m_dGMSValue[GMS_D_JIG];
		if(dGMSValue > dGMS_MAX || dGMSValue < dGMS_MIN)
		{
			dGMSValue = 0.1;   //����Ʈ��
		}
		m_dSumGMSValue[GMS_D_JIG] += dGMSValue;

		m_nGMSAverageCount++;
	}
	else
	{
		m_LastGMSValue[GMS_A_JIG].Format(_T("%f"),(double)( m_dSumGMSValue[GMS_A_JIG]/nAvgNum));
		m_LastGMSValue[GMS_B_JIG].Format(_T("%f"),(double)( m_dSumGMSValue[GMS_B_JIG]/nAvgNum));
		m_LastGMSValue[GMS_C_JIG].Format(_T("%f"),(double)( m_dSumGMSValue[GMS_C_JIG]/nAvgNum));
		m_LastGMSValue[GMS_D_JIG].Format(_T("%f"),(double)( m_dSumGMSValue[GMS_D_JIG]/nAvgNum));
		m_nGMSAverageCount = 0;
		memset(&m_dSumGMSValue,0,sizeof(m_dSumGMSValue));

	}

}