#include "StdAfx.h"
#include "DeviceSerialVACUUM.h"
#include "CheetahApp.h"
#include "Etc/Etc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDeviceSerialVACUUM::CDeviceSerialVACUUM()
{
	m_Serial.m_QueueRead.SetSTX(STX);
	m_Serial.m_QueueRead.SetETX(ETX);
	m_nReadCount	= 4;
	m_nReadSlot = 0;

	memset(m_dbVacValue, 0, sizeof(double)*eMAX_ADSlot*eMAX_ADChannel);

	m_dOffset = 0.;

	m_nVaccAverageCount= 0;
	memset(m_dSumVacValue, 0, sizeof(double)*eMAX_ADSlot*eMAX_ADChannel);
}


CDeviceSerialVACUUM::~CDeviceSerialVACUUM(void)
{
}

BOOL CDeviceSerialVACUUM::SerialOpen( short nPortNo )
{
	if(IsOpen()) return TRUE;

	return m_Serial.InitCommPort(nPortNo,CBR_19200,8,ONESTOPBIT,EVENPARITY);
}

void CDeviceSerialVACUUM::SerialClose()
{
	if(IsOpen() == FALSE) return;

	m_Serial.ClosePort();
}

BOOL CDeviceSerialVACUUM::IsOpen()
{
	return m_Serial.m_bConnected;
}

BOOL CDeviceSerialVACUUM::ParsingReciveValue()
{
	if(IsOpen() == FALSE) return FALSE;

	BYTE buf[1000];
	WCHAR a[1000];

	int nReciveLength = m_Serial.m_QueueRead.GetDataSTX_To_ETX(buf, 100);
	if(nReciveLength < 1)	return FALSE;
	buf[nReciveLength] = NULL;

	int nSizeLimit = 8 + (m_nReadCount * 5);		// �ּ� ������ ���� = �ش� + ���ſ�û ������ ���� * �����ʹ� ����
	char szValue[6];			// �ʿ��� ��ŭ�� �Ҵ�
	TCHAR szMsg[256] = { 0, };

	CEtc::ConvertUnicode((char*)buf, nReciveLength, a, 100);
    a[nReciveLength] = NULL;

	if(nReciveLength < nSizeLimit)
	{		
		theLog[LOG_SERIAL].AddBuf(_T("RECIVE Length ERR %d/%d [%s]"), nReciveLength, nSizeLimit, a);
		return FALSE;
	}
	//������ �� ���Ƽ� ����
	//--theLog[LOG_SERIAL].AddBuf(_T("RECIVE Length %d/%d [%s]"), nReciveLength, nSizeLimit, a);

	if (	(buf[ ePOS_ResCode ]	= '0') &&		// Response Code�� '00' �ΰ� ?
			(buf[ ePOS_ResCode + 1 ]	= '0') )
	{
		//9, 14, 19, 24,
		// 9 + (0 * 5) = 9
		// 9 + (1 * 5) = 14
		// 9 + (2 * 5) = 19
		// 9 + (3 * 5) = 24
		int nStart = ePOS_DataStart;	// 9;				// ������ ������ġ

		for (int i = 0; i < m_nReadCount; i++)
		{
			// nStart = 9 + (i * 5);
			nStart = ePOS_DataStart + (i * (eLEN_Data + eLEN_Delimiter));		// �����͸� ������ ��ġ

			for (int j = 0; j < eLEN_Data; j++)
			{
				szValue[j]	= buf[ nStart + j ];			// 4Bytes�� �����Ѵ�.
				szMsg[j]	= szValue[j];
			}
			szValue[eLEN_Data]	= 0x00;				// �ι��ڷ� ���� ��Ų��.
			szMsg[eLEN_Data]	= 0x00;

			m_sRcvData[m_nReadSlot][i].Format(_T("%s"), szMsg);					// ���ŵ� ������ ��ü�� ǥ��

			if ( szValue[0] != 'L' )		// ���� ������ �������� �ʾҴٸ�, "LLLL"
			{
				m_dbVacValue[m_nReadSlot][i] = atof(szValue) + m_dOffset;		// �Ǽ������� ��ȯ�Ѵ�.
			}
			else
				m_dbVacValue[m_nReadSlot][i] = 0.0;		// �б� ����, ���� �Է� �ҷ� ���� ����
		}//of for i
	}

	// ���̻� ó���Ұ� ������ TRUE [12/7/2016 OSC]
	if(m_Serial.m_QueueRead.GetDataSTX_To_ETX() <= 0)
		return TRUE;

	return FALSE;

}

void CDeviceSerialVACUUM::ClearQueue()
{
	m_Serial.m_QueueRead.Clear();
}

// Checksum�� ���Ѵ�.
// XOR�� ���� 2�� ������ ������ش�.
BYTE CDeviceSerialVACUUM::GetChecksum(BYTE* pData, int nLen)
{
	BYTE bySum = pData[0];

	for (int i = 1; i < nLen; i++) 
	{
		// XOR ������ �ƴ� bySum = bySum ^ pData[i];			// XOR ����
		bySum = (bySum + pData[i]) & 0xFF;			// �ջ� ����
	}

	return (( ~bySum + 1 ) & 0xFF);		// Byte size�� �°� �ٲپ� �ش�.
}
// ����� �����Ѵ�.
// Step�� 0 ~
// Count�� 1 ~
DWORD CDeviceSerialVACUUM::SendReadCommand(int nCount)
{
	// �ʿ��� �����͸� �����Ѵ�.
	BYTE	bySend[32] = { 0x00, };
	int		nLen = 0;
	CString sCheck;

	int nBaseAddr =1000;
	int nNewAddr = nBaseAddr + (nCount * m_nReadSlot);			// �о�� �ּҸ� ����


	CString sAddr;
	sAddr.Format(_T("%04d"),  nNewAddr);


	bySend[nLen++]	= 0x02;				// STX
	bySend[nLen++]	= '0';
	bySend[nLen++]	= '1';
	bySend[nLen++]	= '0';
	bySend[nLen++]	= '1';
	bySend[nLen++]	= 'X';

	bySend[nLen++]	= 'R';				// ������ �б�
	bySend[nLen++]	= 'S';
	bySend[nLen++]	= ',';

	//bySend[nLen++]	= '1';				// �����͸� �о� �� �ּ�
	//bySend[nLen++]	= '0';
	//bySend[nLen++]	= '0';
	//bySend[nLen++]	= '0';
	//bySend[nLen++]	= 'W';

	// �����͸� �о� �� �ּ�
	bySend[nLen++]	= (BYTE)sAddr.GetAt(0);				
	bySend[nLen++]	= (BYTE)sAddr.GetAt(1);	
	bySend[nLen++]	= (BYTE)sAddr.GetAt(2);	
	bySend[nLen++]	= (BYTE)sAddr.GetAt(3);	
	bySend[nLen++]	= 'W';

	bySend[nLen++]	= ',';
	bySend[nLen++]	= '1' + (nCount - 1);				// �ִ� 4���� �����͸� �о�´�. -? 2015-07-31, jhLee, 5 - 8���� ������ ��� ������ �ȵ�
	bySend[nLen++]	= 0x03;				// ETX

	sCheck.Format(_T("%02X"), GetChecksum(bySend, nLen));		// Checksum�� �����.

	bySend[nLen++]	= (BYTE)sCheck.GetAt(0);					// Checksum�� BYTE 2�ڸ��� ����� �ش�.
	bySend[nLen++]	= (BYTE)sCheck.GetAt(1);

	bySend[nLen++]	= 0x0D;		// CR
	bySend[nLen++]	= 0x0A;		// LF
	bySend[nLen]	= 0x00;		// NULL

	int nSendLen =  m_Serial.WriteComm( bySend, nLen );

	return nSendLen;
}

// �Է� ���� ���� ����� �����Ѵ�.
//
// [in] int nCh : ������ �����ϰ��� �ϴ� ä�� ����, 0 ~ 31
// [in] int nMode : �����ϰ��� �ϴ� ���� ��尪
//
DWORD CDeviceSerialVACUUM::SendRangeSetCommand(int nCh, int nMode)
{
	// �ʿ��� �����͸� �����Ѵ�.
	BYTE	bySend[32];
	int		nLen = 0;
	CString sCheck;

	int nNewAddr = 3000 + nCh;			// ������ ������ ä�� �ּ�, 0 ~ 31

	CString sAddr, sValue;
	sAddr.Format(_T("%04d"),  nNewAddr);		// �ּҸ� ���ڿ��� ������ش�.
	sValue.Format(_T("%04d"), nMode);			// ���� ��带 ����� �ش�. "0003"

	bySend[nLen++]	= 0x02;				// STX
	bySend[nLen++]	= '0';
	bySend[nLen++]	= '1';
	bySend[nLen++]	= '0';
	bySend[nLen++]	= '1';
	bySend[nLen++]	= 'X';


	bySend[nLen++]	= 'W';				// ������ ����
	bySend[nLen++]	= 'S';
	bySend[nLen++]	= ',';

	//bySend[nLen++]	= '3';				// �����͸� �� �ּ�
	//bySend[nLen++]	= '0';
	//bySend[nLen++]	= '0';
	//bySend[nLen++]	= '4';

	// �����͸� �о� �� �ּ�
	bySend[nLen++]	= (BYTE)sAddr.GetAt(0);				
	bySend[nLen++]	= (BYTE)sAddr.GetAt(1);	
	bySend[nLen++]	= (BYTE)sAddr.GetAt(2);	
	bySend[nLen++]	= (BYTE)sAddr.GetAt(3);	
	bySend[nLen++]	= 'W';

	bySend[nLen++]	= ',';

	//bySend[nLen++]	= '0';				
	//bySend[nLen++]	= '0';				
	//bySend[nLen++]	= '0';				// ������
	//bySend[nLen++]	= '3';				

	bySend[nLen++]	= (BYTE)sValue.GetAt(0);				
	bySend[nLen++]	= (BYTE)sValue.GetAt(1);	
	bySend[nLen++]	= (BYTE)sValue.GetAt(2);	
	bySend[nLen++]	= (BYTE)sValue.GetAt(3);	

	bySend[nLen++]	= 0x03;				// ETX

	sCheck.Format(_T("%02X"), GetChecksum(bySend, nLen));		// Checksum�� �����.

	bySend[nLen++]	= (BYTE)sCheck.GetAt(0);					// Checksum�� BYTE 2�ڸ��� ����� �ش�.
	bySend[nLen++]	= (BYTE)sCheck.GetAt(1);

	bySend[nLen++]	= 0x0D;		// CR
	bySend[nLen++]	= 0x0A;		// LF
	bySend[nLen]	= 0x00;		// NULL


	int nSendLen =  m_Serial.WriteComm( bySend, nLen );

	return nSendLen;
}

void CDeviceSerialVACUUM::SetSerialName(CString strValue)
{
	m_strSerialName = strValue;
// 	m_Serial.m_serialMode = m_serialMode;
}

void CDeviceSerialVACUUM::AverageUDVacValue(int nAvgNum)
{
	double dAIR_MAX = theConfigBank.m_Option.m_dFDCRange[AIR_MAX];
	double dAIR_MIN = theConfigBank.m_Option.m_dFDCRange[AIR_MIN];

	double dVAC_MAX = theConfigBank.m_Option.m_dFDCRange[VAC_MAX];
	double dVAC_MIN = theConfigBank.m_Option.m_dFDCRange[VAC_MIN];

	double dValue = 0;

	if(nAvgNum < 1) return;

	if(m_nVaccAverageCount < nAvgNum)
	{
		dValue = m_dbVacValue[0][0];
		if((dValue> dVAC_MAX  && dValue < -20) || dValue < dVAC_MIN )  //���κ�Ũ�� ���� �ȵ��ð���� ����ؼ� -20 ������ �߰�
		{
			dValue = -60;
		}
		m_dSumVacValue[0][0] += dValue ;

		dValue = m_dbVacValue[0][1];
		if((dValue> dVAC_MAX  && dValue < -20)||  dValue < dVAC_MIN)
		{
			dValue = -60;
		}
		m_dSumVacValue[0][1] +=dValue ;

		dValue = m_dbVacValue[0][2];
		if(dValue> dAIR_MAX ||  (dValue < dAIR_MIN && dValue > 20))
		{
			dValue = 55;
		}
		m_dSumVacValue[0][2] += dValue ;

		dValue = m_dbVacValue[0][3];
		if(dValue> dAIR_MAX ||  (dValue < dAIR_MIN && dValue > 20))
		{
			dValue = 55;
		}
		m_dSumVacValue[0][3] += dValue ;


		m_dSumVacValue[1][0] += m_dbVacValue[1][0] ;
		m_dSumVacValue[1][1] += m_dbVacValue[1][1] ;
		m_dSumVacValue[1][2] += m_dbVacValue[1][2] ;
		m_dSumVacValue[1][3] += m_dbVacValue[1][3] ;

		m_nVaccAverageCount++;

	}
	else
	{
		theFDCBank.m_dMainVac_1 = (double)(m_dSumVacValue[0][0]/nAvgNum)  ;
		theFDCBank.m_dMainVac_2 = (double)(m_dSumVacValue[0][1]/nAvgNum)  ;
		theFDCBank.m_dMainAir_1 = (double)(m_dSumVacValue[0][2]/nAvgNum)  ;
		theFDCBank.m_dMainAir_2 = (double)(m_dSumVacValue[0][3]/nAvgNum)  ;

		theFDCBank.m_dInspVac[ULD_TR_CH1]    = (double)(m_dSumVacValue[1][0]/nAvgNum)  ;
		theFDCBank.m_dInspVac[ULD_TR_CH2]    = (double)(m_dSumVacValue[1][1]/nAvgNum)  ;
		theFDCBank.m_dInspVac[ULD_STAGE_CH1] = (double)(m_dSumVacValue[1][2]/nAvgNum)  ;
		theFDCBank.m_dInspVac[ULD_STAGE_CH2] = (double)(m_dSumVacValue[1][3]/nAvgNum)  ;

		
		theFDCBank.m_dMainVac_1_OUT = theFDCBank.m_dMainVac_1 + theFDCBank.m_dAdOffset[MAIN_VAC_1];
		theFDCBank.m_dMainVac_2_OUT = theFDCBank.m_dMainVac_2 + theFDCBank.m_dAdOffset[MAIN_VAC_2];
		theFDCBank.m_dMainAir_1_OUT = (theFDCBank.m_dMainAir_1 * 10.0) + theFDCBank.m_dAdOffset[MAIN_AIR_1];
		if(theFDCBank.m_dMainAir_1_OUT > 690 || theFDCBank.m_dMainAir_1_OUT < 515)
		{
			theFDCBank.m_dMainAir_1_OUT =630;
		}

		theFDCBank.m_dMainAir_2_OUT = (theFDCBank.m_dMainAir_2 * 10.0) + theFDCBank.m_dAdOffset[MAIN_AIR_2];		
		if(theFDCBank.m_dMainAir_2_OUT > 690 || theFDCBank.m_dMainAir_2_OUT < 515)
		{
			theFDCBank.m_dMainAir_2_OUT =630;
		}

		theFDCBank.m_dInspVac_OUT[ULD_TR_CH1] =    theFDCBank.m_dInspVac[ULD_TR_CH1]   		+theFDCBank.m_dAdOffset[ULD_TR_CH1];
		theFDCBank.m_dInspVac_OUT[ULD_TR_CH2] =    theFDCBank.m_dInspVac[ULD_TR_CH2]   		+theFDCBank.m_dAdOffset[ULD_TR_CH2];
		theFDCBank.m_dInspVac_OUT[ULD_STAGE_CH1] = theFDCBank.m_dInspVac[ULD_STAGE_CH1]		+theFDCBank.m_dAdOffset[ULD_STAGE_CH1];
		theFDCBank.m_dInspVac_OUT[ULD_STAGE_CH2] = theFDCBank.m_dInspVac[ULD_STAGE_CH2]		+theFDCBank.m_dAdOffset[ULD_STAGE_CH2];
	    
		memset(m_dSumVacValue, 0, sizeof(double)*eMAX_ADSlot*eMAX_ADChannel);
		m_nVaccAverageCount = 0;

	}


}