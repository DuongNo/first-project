#pragma once
#include "stdafx.h"
#include "Device\SerialManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW

#endif

//20160930 kjpark Serial IF
CSerialManager::CSerialManager(void)
{	


}

//20160930 kjpark Serial IF
CSerialManager::~CSerialManager(void)
{

}

DWORD CSerialManager::SerialSendData(SERIAL_MODE nPort, BYTE *pBuff, DWORD nToWrite)
{
#if 0
	if(nToWrite == m_vctNPortReal[nPort].WriteComm((BYTE*)pBuff, nToWrite))
		return nToWrite;
	else
		return FALSE;
#else
	return 0;
#endif
}

int CSerialManager::SerialGetDataBuffer(SERIAL_MODE nPort, BYTE *cstr, int nMaxSize)
{
#if 0
	if( !m_vctNPortReal[nPort].m_bConnected) 
		return false;

	int nSizeOfbuf = m_vctNPortReal[nPort].m_QueueRead.GetSize();

	if ( nSizeOfbuf > 0 )
	{
		nSizeOfbuf = min(nSizeOfbuf, nMaxSize);
		memcpy(cstr, &m_vctNPortReal[nPort].m_QueueRead.buff[m_vctNPortReal[nPort].m_QueueRead.m_iTail], nSizeOfbuf);
	}

	return nSizeOfbuf;
#else
	return 0;
#endif
}

//20160930 kjpark Serial IF
BOOL CSerialManager::SerialOpen(SERIAL_MODE nPort)
{

	BOOL bRet = FALSE;
	
	switch(nPort)
	{
			case SERIAL_GMS:
				{
					if(!m_pDeviceGMS.GetCommThreadHandler().m_bConnected) 
					{
						bRet = m_pDeviceGMS.SerialOpen(SerialModeSet[nPort].nPort);
						if(bRet)
						{
							m_pDeviceGMS.SetSerialName(SerialModeSet[nPort].m_sPortNickName);
						}
					}
				}
				break;		
			case SERIAL_TEMPERATURE:
				{
					if(!m_pDeviceTemperature.GetCommThreadHandler().m_bConnected) 
					{
						bRet = m_pDeviceTemperature.SerialOpen(SerialModeSet[nPort].nPort);
						if(bRet)
						{
							m_pDeviceTemperature.SetSerialName(SerialModeSet[nPort].m_sPortNickName);
						}
					}
				}
				break;
			case SERIAL_VACUUM_LD:
				{
					if(m_DeviceVACUUMLD.IsOpen() == FALSE) 
					{
						bRet = m_DeviceVACUUMLD.SerialOpen(SerialModeSet[nPort].nPort);
						if(bRet)
						{
							m_DeviceVACUUMLD.SetSerialName(SerialModeSet[nPort].m_sPortNickName);
						}
					}
				}
				break;
			case SERIAL_VACUUM_INSP:
				{
					if(m_DeviceVACUUMINSP.IsOpen() == FALSE) 
					{
						bRet = m_DeviceVACUUMINSP.SerialOpen(SerialModeSet[nPort].nPort);
						if(bRet)
						{
							m_DeviceVACUUMINSP.SetSerialName(SerialModeSet[nPort].m_sPortNickName);
						}
					}
				}
				break;
			case SERIAL_VACUUM_UD:
				{
					if(m_DeviceVACUUMUD.IsOpen() == FALSE) 
					{
						bRet = m_DeviceVACUUMUD.SerialOpen(SerialModeSet[nPort].nPort);
						if(bRet)
						{
							m_DeviceVACUUMUD.SetSerialName(SerialModeSet[nPort].m_sPortNickName);
						}
					}
				}
				break;
			default:
				break;
	}
	
	return bRet;
	
}

//20160930 kjpark Serial IF
BOOL CSerialManager::SerialClose(SERIAL_MODE nPort)
{
	BOOL bRet = FALSE;

	switch(nPort)
	{
		case SERIAL_GMS:
			{	
				m_pDeviceGMS.SerialClose();			
			}
			break;
		case SERIAL_TEMPERATURE:
			{	
				m_pDeviceTemperature.SerialClose();			
			}
			break;
		case SERIAL_VACUUM_LD:
			{	
				m_DeviceVACUUMLD.SerialClose();			
			}
			break;	
		case SERIAL_VACUUM_INSP:
			{	
				m_DeviceVACUUMINSP.SerialClose();			
			}
			break;	
		case SERIAL_VACUUM_UD:
			{	
				m_DeviceVACUUMUD.SerialClose();			
			}
			break;
		default:
			break;
	}

	return bRet;
}
BOOL CSerialManager::SerialCodeSend(int nCode, int nPort)
{
#if 0
	m_vctNPortReal[nPort].m_QueueRead.Clear();

	CString szMessage;
	szMessage.Format(_T("%c%c%c%c%03d%c"),ENQ, STX, ESC, 0x56, ETX);


	CEtc::ConvertMultibyte((wchar_t*)szMessage.GetString(), (char*)m_vctStrData[nPort][0], 255);
#endif
	return FALSE;
}

BOOL CSerialManager::SerialAckCheck(SERIAL_MODE nPort)
{
	CString szData;
	BYTE buffer[256];

	int nLength = nLength = SerialGetDataBuffer(SerialModeSet[nPort].SerialMode, buffer, 255);

	if(nLength > 0)
	{
		buffer[255] = NULL;
		for(int nBuf=0;nBuf<nLength;nBuf++)
		{
			if(buffer[nBuf] == ACK)	// 0x06	: acknowledge
				return TRUE;
		}
	}

	return FALSE;
}

void CSerialManager::SendCommand(void)
{
	
}

//20160930 kjpark Serial IF
void CSerialManager::PacketReceive(SERIAL_MODE serialMode)
{

}

//20161227 byskcho
void CSerialManager::SetSerialInfo(SERIAL_MODE nPort,UINT nPortNum)
{
	if(nPort > PORT_MAX || nPortNum == 0L)
	{
		return;
	}
	SerialModeSet[nPort].nPort = nPortNum;

}


void CSerialManager::SendSetVacRange(int nType)
{
	int i = 0;
// 	m_nRangeSetCh	// 범위를 설정하고자하는 채널 0 ~ 31
// 	m_nRangeSetMode	// 범위를 설정하고자 하는 모드, 0 ~ 3까지 해서 양수인 값으로넣자

	// ... 뭔가 읽으려는 슬롯마다 다 해줘야 할듯
/*	m_DeviceVACUUMLD.SendRangeSetCommand(15, 2);	// LD-CH16
	m_DeviceVACUUMUD.SendRangeSetCommand(4, 2);		// UD-CH05
	Sleep(100);
	m_DeviceVACUUMUD.SendRangeSetCommand(15, 2);		// UD-CH16
	Sleep(100);

	//20170130 byskcho
	//cim oss 파일 변경으로 인해 추가  zone 버큠
	//UD TR1,2 UD STAGE1,2 추가
	for(i = 0; i< 4; i++)
	{
		m_DeviceVACUUMUD.SendRangeSetCommand(i, 3);		// UDTR1,2 UDSTAGE1,2
		Sleep(100);
	}
	//20170129 byskcho
	//cim oss 파일 변경으로 인해 추가  zone 버큠
	for( i = 0;  i< 8; i++)
	{
		m_DeviceVACUUMINSP.SendRangeSetCommand(i, 3);
		Sleep(100);
	}
*/	
#if (AMT_TYPE == 2)
	if(nType == 1)
	{
		for(int i = 0; i < 8; i++)
		{
			m_DeviceVACUUMINSP.SendRangeSetCommand(i, 1);		// 1-> 음수
			Sleep(100);
		}
	}
	else
	{
		for(int i = 0; i < 8; i++)
		{
			m_DeviceVACUUMUD.SendRangeSetCommand(i, 1);		// 1 -> 음수
			Sleep(100);
		}

		m_DeviceVACUUMUD.SendRangeSetCommand(2, 3);		// 1 -> 음수
		Sleep(100);
		m_DeviceVACUUMUD.SendRangeSetCommand(3, 3);		// 1 -> 음수
		Sleep(100);
	}
#else



#endif
}

void CSerialManager::SendVacReadLD(int nSlot)
{
	m_DeviceVACUUMLD.m_nReadSlot = nSlot;
	m_DeviceVACUUMLD.SendReadCommand(4);	
}

BOOL CSerialManager::ParsingLDVACUUMValue()
{
	return m_DeviceVACUUMLD.ParsingReciveValue();
}

void CSerialManager::SendVacReadUD(int nSlot)
{	
	m_DeviceVACUUMUD.m_nReadSlot = nSlot;
	m_DeviceVACUUMUD.SendReadCommand(4);

}
void CSerialManager::SendVacReadINSP(int nSlot)
{
	m_DeviceVACUUMINSP.m_nReadSlot = nSlot;
	m_DeviceVACUUMINSP.SendReadCommand(4);

}
BOOL CSerialManager::ParsingUDVACUUMValue()
{
	return m_DeviceVACUUMUD.ParsingReciveValue();
}

BOOL CSerialManager::ParsingINSPVACUUMValue()
{
	return m_DeviceVACUUMINSP.ParsingReciveValue();
}
BOOL CSerialManager::ParsingTemperatureValue()
{
	return m_pDeviceTemperature.ParsingReciveValue();

}
void CSerialManager::SendTempRead(int nAddr)
{
	m_pDeviceTemperature.SendCommand_TempValue(nAddr);
}

void CSerialManager::CopyTempValue_to_FDCBank(int nAddr)
{
	int nBuff = (int)(nAddr - 1);

	//2017-08-31,SKCHO, 5회 평균
	m_pDeviceTemperature.TempAverageValue(nBuff,3);

	/*if(eIDX_TEMP_PCRACK	== nBuff)				// PC-RACK 온도 센서
	{
		theFDCBank.m_dPc_Rack_Temp = m_pDeviceTemperature.GetTempValue(eIDX_TEMP_PCRACK);
	}
	else if(eIDX_TEMP_LOAD== nBuff)
	{
		theFDCBank.m_dLoad_Temp = m_pDeviceTemperature.GetTempValue(eIDX_TEMP_LOAD);
	}
	else if(eIDX_TEMP_UPS== nBuff)
	{
		theFDCBank.m_dUps_Temp = m_pDeviceTemperature.GetTempValue(eIDX_TEMP_UPS);
	}
	else if(eIDX_TEMP_UNLOAD== nBuff)
	{
		theFDCBank.m_dUnload_Temp = m_pDeviceTemperature.GetTempValue(eIDX_TEMP_UNLOAD);
	}
	else if(eIDX_TEMP_GPS== nBuff)
	{
		theFDCBank.m_dGps_Temp = m_pDeviceTemperature.GetTempValue(eIDX_TEMP_GPS);
	}
	else if(eIDX_TEMP_DRIVEBOX== nBuff)
	{
		theFDCBank.m_dDriverBox_Temp = m_pDeviceTemperature.GetTempValue(eIDX_TEMP_DRIVEBOX);
	}
	*/
	
}
int CSerialManager::GetDeviceAddr()
{
	return m_pDeviceTemperature.m_nDeviceAddr;
}
void CSerialManager::CopyAirValue_to_FDCBank(SERIAL_MODE nMode)
{

	CCellInfo* pCellInfo= NULL;
	CCellInfo* pCellInfo1= NULL;
	CCellInfo* pCellInfo2= NULL;
	int nIndex = 0;

#if (AMT_TYPE == 1)
	if(nMode == SERIAL_VACUUM_LD)
	{
		// Load Main Air -> Load A/D Board의 16번채널(4Slot-4Ch)
		theFDCBank.m_dMainAir_1 = (m_DeviceVACUUMLD.m_dbVacValue[0][0] * -10.0)	+ theFDCBank.m_dAdOffset[MAIN_VAC_1];
		theFDCBank.m_dMainAir_2 = (m_DeviceVACUUMLD.m_dbVacValue[0][1] * -10.0)	+ theFDCBank.m_dAdOffset[MAIN_VAC_2];
		theFDCBank.m_dMainVac_1 = (m_DeviceVACUUMLD.m_dbVacValue[0][2])			+ theFDCBank.m_dAdOffset[MAIN_AIR_1];
		theFDCBank.m_dMainVac_2 = (m_DeviceVACUUMLD.m_dbVacValue[0][3])			+ theFDCBank.m_dAdOffset[MAIN_AIR_2];
	}
	else if(nMode == SERIAL_VACUUM_INSP)
	{
		// Insp Main Air -> Uload A/D Board의 5번채널(2Slot-1Ch)
		//theFDCBank.m_dInspMainAir = m_DeviceVACUUMINSP.m_dbVacValue[0][0];
		//20170129 byskcho
		//cim oss  파일 변경으로 인해 추가
		theFDCBank.m_dInspVac[A_ZONE_CH1] = (m_DeviceVACUUMINSP.m_dbVacValue[0][0]* -1.0)+ theFDCBank.m_dAdOffset[A_ZONE_CH1];
		theFDCBank.m_dInspVac[A_ZONE_CH2] = (m_DeviceVACUUMINSP.m_dbVacValue[0][1]* -1.0)+ theFDCBank.m_dAdOffset[A_ZONE_CH2];
		theFDCBank.m_dInspVac[B_ZONE_CH1] = (m_DeviceVACUUMINSP.m_dbVacValue[0][2]* -1.0)+ theFDCBank.m_dAdOffset[B_ZONE_CH1];
		theFDCBank.m_dInspVac[B_ZONE_CH2] = (m_DeviceVACUUMINSP.m_dbVacValue[0][3]* -1.0)+ theFDCBank.m_dAdOffset[B_ZONE_CH2];
		theFDCBank.m_dInspVac[C_ZONE_CH1] = (m_DeviceVACUUMINSP.m_dbVacValue[1][0]* -1.0)+ theFDCBank.m_dAdOffset[C_ZONE_CH1];
		theFDCBank.m_dInspVac[C_ZONE_CH2] = (m_DeviceVACUUMINSP.m_dbVacValue[1][1]* -1.0)+ theFDCBank.m_dAdOffset[C_ZONE_CH2];
		theFDCBank.m_dInspVac[D_ZONE_CH1] = (m_DeviceVACUUMINSP.m_dbVacValue[1][2]* -1.0)+ theFDCBank.m_dAdOffset[D_ZONE_CH1];
		theFDCBank.m_dInspVac[D_ZONE_CH2] = (m_DeviceVACUUMINSP.m_dbVacValue[1][3]* -1.0)+ theFDCBank.m_dAdOffset[D_ZONE_CH2];
	}
	else if(nMode == SERIAL_VACUUM_UD)
	{
		// Unload Main Air -> Uload A/D Board의 16번채널(4Slot-4Ch)
		//theFDCBank.m_dUnloadMainAir = m_DeviceVACUUMUD.m_dbVacValue[0][0];
		//20170129 byskcho
		//cim oss  파일 변경으로 인해 추가
		theFDCBank.m_dInspVac[ULD_TR_CH1] = (m_DeviceVACUUMUD.m_dbVacValue[0][0]* -1.0)    +theFDCBank.m_dAdOffset[ULD_TR_CH1];
		theFDCBank.m_dInspVac[ULD_TR_CH2] = (m_DeviceVACUUMUD.m_dbVacValue[0][1]* -1.0)	  +theFDCBank.m_dAdOffset[ULD_TR_CH2];
		theFDCBank.m_dInspVac[ULD_STAGE_CH1] = (m_DeviceVACUUMUD.m_dbVacValue[0][2]* -1.0) +theFDCBank.m_dAdOffset[ULD_STAGE_CH1];
		theFDCBank.m_dInspVac[ULD_STAGE_CH2] = (m_DeviceVACUUMUD.m_dbVacValue[0][3]* -1.0) +theFDCBank.m_dAdOffset[ULD_STAGE_CH2];
	}
	else
	{
		;
	}
#else
	if(nMode == SERIAL_VACUUM_LD)
	{
	}
	else if(nMode == SERIAL_VACUUM_INSP)
	{
		if(theUnitFunc.GetEntryTablePos() == CONST_TABLE_POS::POS_1) //JIG_A
		{
			theFDCBank.m_dInspVac[A_ZONE_CH1] = m_DeviceVACUUMINSP.m_dbVacValue[0][0];
			theFDCBank.m_dInspVac[A_ZONE_CH2] = m_DeviceVACUUMINSP.m_dbVacValue[0][1];
			theFDCBank.m_dInspVac[B_ZONE_CH1] = m_DeviceVACUUMINSP.m_dbVacValue[0][2];
			theFDCBank.m_dInspVac[B_ZONE_CH2] = m_DeviceVACUUMINSP.m_dbVacValue[0][3];
			theFDCBank.m_dInspVac[C_ZONE_CH1] = m_DeviceVACUUMINSP.m_dbVacValue[1][0];
			theFDCBank.m_dInspVac[C_ZONE_CH2] = m_DeviceVACUUMINSP.m_dbVacValue[1][1];
			theFDCBank.m_dInspVac[D_ZONE_CH1] = m_DeviceVACUUMINSP.m_dbVacValue[1][2];
			theFDCBank.m_dInspVac[D_ZONE_CH2] = m_DeviceVACUUMINSP.m_dbVacValue[1][3];
		}
		else if(theUnitFunc.GetEntryTablePos() == CONST_TABLE_POS::POS_2) //JIG_D
		{
			theFDCBank.m_dInspVac[A_ZONE_CH1] = m_DeviceVACUUMINSP.m_dbVacValue[1][2];  //D
			theFDCBank.m_dInspVac[A_ZONE_CH2] = m_DeviceVACUUMINSP.m_dbVacValue[1][3];
			theFDCBank.m_dInspVac[B_ZONE_CH1] = m_DeviceVACUUMINSP.m_dbVacValue[0][0];  //A
			theFDCBank.m_dInspVac[B_ZONE_CH2] = m_DeviceVACUUMINSP.m_dbVacValue[0][1];
			theFDCBank.m_dInspVac[C_ZONE_CH1] = m_DeviceVACUUMINSP.m_dbVacValue[0][2]; //B
			theFDCBank.m_dInspVac[C_ZONE_CH2] = m_DeviceVACUUMINSP.m_dbVacValue[0][3];
			theFDCBank.m_dInspVac[D_ZONE_CH1] = m_DeviceVACUUMINSP.m_dbVacValue[1][0]; //C
			theFDCBank.m_dInspVac[D_ZONE_CH2] = m_DeviceVACUUMINSP.m_dbVacValue[1][1];	
		}
		else if(theUnitFunc.GetEntryTablePos() == CONST_TABLE_POS::POS_3) //JIG_C
		{
			theFDCBank.m_dInspVac[A_ZONE_CH1] = m_DeviceVACUUMINSP.m_dbVacValue[1][0];  //C
			theFDCBank.m_dInspVac[A_ZONE_CH2] = m_DeviceVACUUMINSP.m_dbVacValue[1][1];
			theFDCBank.m_dInspVac[B_ZONE_CH1] = m_DeviceVACUUMINSP.m_dbVacValue[1][2];  //D
			theFDCBank.m_dInspVac[B_ZONE_CH2] = m_DeviceVACUUMINSP.m_dbVacValue[1][3];
			theFDCBank.m_dInspVac[C_ZONE_CH1] = m_DeviceVACUUMINSP.m_dbVacValue[0][0]; //A
			theFDCBank.m_dInspVac[C_ZONE_CH2] = m_DeviceVACUUMINSP.m_dbVacValue[0][1];
			theFDCBank.m_dInspVac[D_ZONE_CH1] = m_DeviceVACUUMINSP.m_dbVacValue[0][2]; //B
			theFDCBank.m_dInspVac[D_ZONE_CH2] = m_DeviceVACUUMINSP.m_dbVacValue[0][3];	

		}
		else if(theUnitFunc.GetEntryTablePos() == CONST_TABLE_POS::POS_4) //JIG_B
		{
            theFDCBank.m_dInspVac[A_ZONE_CH1] = m_DeviceVACUUMINSP.m_dbVacValue[0][2];  //B
			theFDCBank.m_dInspVac[A_ZONE_CH2] = m_DeviceVACUUMINSP.m_dbVacValue[0][3];
			theFDCBank.m_dInspVac[B_ZONE_CH1] = m_DeviceVACUUMINSP.m_dbVacValue[1][0];  //C
			theFDCBank.m_dInspVac[B_ZONE_CH2] = m_DeviceVACUUMINSP.m_dbVacValue[1][1];
			theFDCBank.m_dInspVac[C_ZONE_CH1] = m_DeviceVACUUMINSP.m_dbVacValue[1][2]; //D
			theFDCBank.m_dInspVac[C_ZONE_CH2] = m_DeviceVACUUMINSP.m_dbVacValue[1][3];
			theFDCBank.m_dInspVac[D_ZONE_CH1] = m_DeviceVACUUMINSP.m_dbVacValue[0][0]; //A
			theFDCBank.m_dInspVac[D_ZONE_CH2] = m_DeviceVACUUMINSP.m_dbVacValue[0][1];	

		}
		else
		{
			theFDCBank.m_dInspVac[A_ZONE_CH1] = m_DeviceVACUUMINSP.m_dbVacValue[0][0];
			theFDCBank.m_dInspVac[A_ZONE_CH2] = m_DeviceVACUUMINSP.m_dbVacValue[0][1];
			theFDCBank.m_dInspVac[B_ZONE_CH1] = m_DeviceVACUUMINSP.m_dbVacValue[0][2];
			theFDCBank.m_dInspVac[B_ZONE_CH2] = m_DeviceVACUUMINSP.m_dbVacValue[0][3];
			theFDCBank.m_dInspVac[C_ZONE_CH1] = m_DeviceVACUUMINSP.m_dbVacValue[1][0];
			theFDCBank.m_dInspVac[C_ZONE_CH2] = m_DeviceVACUUMINSP.m_dbVacValue[1][1];
			theFDCBank.m_dInspVac[D_ZONE_CH1] = m_DeviceVACUUMINSP.m_dbVacValue[1][2];
			theFDCBank.m_dInspVac[D_ZONE_CH2] = m_DeviceVACUUMINSP.m_dbVacValue[1][3];

		}
			theFDCBank.m_dInspVac_OUT[A_ZONE_CH1] = theFDCBank.m_dInspVac[A_ZONE_CH1] + theFDCBank.m_dAdOffset[A_ZONE_CH1];
			theFDCBank.m_dInspVac_OUT[A_ZONE_CH2] = theFDCBank.m_dInspVac[A_ZONE_CH2] + theFDCBank.m_dAdOffset[A_ZONE_CH2];
			theFDCBank.m_dInspVac_OUT[B_ZONE_CH1] = theFDCBank.m_dInspVac[B_ZONE_CH1] + theFDCBank.m_dAdOffset[B_ZONE_CH1];
			theFDCBank.m_dInspVac_OUT[B_ZONE_CH2] = theFDCBank.m_dInspVac[B_ZONE_CH2] + theFDCBank.m_dAdOffset[B_ZONE_CH2];
			theFDCBank.m_dInspVac_OUT[C_ZONE_CH1] = theFDCBank.m_dInspVac[C_ZONE_CH1] + theFDCBank.m_dAdOffset[C_ZONE_CH1];
			theFDCBank.m_dInspVac_OUT[C_ZONE_CH2] = theFDCBank.m_dInspVac[C_ZONE_CH2] + theFDCBank.m_dAdOffset[C_ZONE_CH2];
			theFDCBank.m_dInspVac_OUT[D_ZONE_CH1] = theFDCBank.m_dInspVac[D_ZONE_CH1] + theFDCBank.m_dAdOffset[D_ZONE_CH1];
			theFDCBank.m_dInspVac_OUT[D_ZONE_CH2] = theFDCBank.m_dInspVac[D_ZONE_CH2] + theFDCBank.m_dAdOffset[D_ZONE_CH2];

			if(theFDCBank.m_dInspVac_OUT[A_ZONE_CH1]>0) theFDCBank.m_dInspVac_OUT[A_ZONE_CH1]= theFDCBank.m_dInspVac_OUT[A_ZONE_CH1] * -1;
			if(theFDCBank.m_dInspVac_OUT[A_ZONE_CH2]>0) theFDCBank.m_dInspVac_OUT[A_ZONE_CH2]= theFDCBank.m_dInspVac_OUT[A_ZONE_CH2] * -1;
			if(theFDCBank.m_dInspVac_OUT[B_ZONE_CH1]>0) theFDCBank.m_dInspVac_OUT[B_ZONE_CH1]= theFDCBank.m_dInspVac_OUT[B_ZONE_CH1] * -1;
			if(theFDCBank.m_dInspVac_OUT[B_ZONE_CH2]>0) theFDCBank.m_dInspVac_OUT[B_ZONE_CH2]= theFDCBank.m_dInspVac_OUT[B_ZONE_CH2] * -1;
			if(theFDCBank.m_dInspVac_OUT[C_ZONE_CH1]>0) theFDCBank.m_dInspVac_OUT[C_ZONE_CH1]= theFDCBank.m_dInspVac_OUT[C_ZONE_CH1] * -1;
			if(theFDCBank.m_dInspVac_OUT[C_ZONE_CH2]>0) theFDCBank.m_dInspVac_OUT[C_ZONE_CH2]= theFDCBank.m_dInspVac_OUT[C_ZONE_CH2] * -1;
			if(theFDCBank.m_dInspVac_OUT[D_ZONE_CH1]>0) theFDCBank.m_dInspVac_OUT[D_ZONE_CH1]= theFDCBank.m_dInspVac_OUT[D_ZONE_CH1] * -1;
			if(theFDCBank.m_dInspVac_OUT[D_ZONE_CH2]>0) theFDCBank.m_dInspVac_OUT[D_ZONE_CH2]= theFDCBank.m_dInspVac_OUT[D_ZONE_CH2] * -1;

	}
	else if(nMode == SERIAL_VACUUM_UD)
	{
		

		m_DeviceVACUUMUD.AverageUDVacValue(5);

		/*theFDCBank.m_dMainVac_1 =m_DeviceVACUUMUD.m_dbVacValue[0][0] ;
		theFDCBank.m_dMainVac_2 =m_DeviceVACUUMUD.m_dbVacValue[0][1] ;
		theFDCBank.m_dMainAir_1 =m_DeviceVACUUMUD.m_dbVacValue[0][2] ;
		theFDCBank.m_dMainAir_2 =m_DeviceVACUUMUD.m_dbVacValue[0][3] ;

		//2017.07.22. JSJUNG, 인터락 문제로 인하여 스펙 벗어나는 값은 무시_고객사요청
		if(theFDCBank.m_dMainVac_1 + theFDCBank.m_dAdOffset[MAIN_VAC_1] < -40 && theFDCBank.m_dMainVac_1 + theFDCBank.m_dAdOffset[MAIN_VAC_1] > -73)
			theFDCBank.m_dMainVac_1_OUT = theFDCBank.m_dMainVac_1 + theFDCBank.m_dAdOffset[MAIN_VAC_1];
		else
			theFDCBank.m_dMainVac_1_OUT = -60.8;
		if(theFDCBank.m_dMainVac_2 + theFDCBank.m_dAdOffset[MAIN_VAC_2] < -40 && theFDCBank.m_dMainVac_2 + theFDCBank.m_dAdOffset[MAIN_VAC_2] > -73)
			theFDCBank.m_dMainVac_2_OUT = theFDCBank.m_dMainVac_2 + theFDCBank.m_dAdOffset[MAIN_VAC_2];
		else
			theFDCBank.m_dMainVac_2_OUT = -61.3;
		if(theFDCBank.m_dMainAir_1 * 10.0 + theFDCBank.m_dAdOffset[MAIN_AIR_1]>400 && theFDCBank.m_dMainAir_1 * 10.0 + theFDCBank.m_dAdOffset[MAIN_AIR_1] <700)
			theFDCBank.m_dMainAir_1_OUT = theFDCBank.m_dMainAir_1 * 10.0 + theFDCBank.m_dAdOffset[MAIN_AIR_1];
		else
			theFDCBank.m_dMainAir_1_OUT = 588;
		if(theFDCBank.m_dMainAir_2 * 10.0 + theFDCBank.m_dAdOffset[MAIN_AIR_2]>400 && theFDCBank.m_dMainAir_2 * 10.0 + theFDCBank.m_dAdOffset[MAIN_AIR_2] <700)
			theFDCBank.m_dMainAir_2_OUT = theFDCBank.m_dMainAir_2 * 10.0 + theFDCBank.m_dAdOffset[MAIN_AIR_2];		
		else
			theFDCBank.m_dMainAir_2_OUT = 594;
			

		theFDCBank.m_dInspVac[ULD_TR_CH1]    = m_DeviceVACUUMUD.m_dbVacValue[1][0] ;     
		theFDCBank.m_dInspVac[ULD_TR_CH2]    = m_DeviceVACUUMUD.m_dbVacValue[1][1] ;     
		theFDCBank.m_dInspVac[ULD_STAGE_CH1] = m_DeviceVACUUMUD.m_dbVacValue[1][2] ;  
		theFDCBank.m_dInspVac[ULD_STAGE_CH2] = m_DeviceVACUUMUD.m_dbVacValue[1][3] ;  
		
		theFDCBank.m_dInspVac_OUT[ULD_TR_CH1] =    theFDCBank.m_dInspVac[ULD_TR_CH1]   		+theFDCBank.m_dAdOffset[ULD_TR_CH1];
		theFDCBank.m_dInspVac_OUT[ULD_TR_CH2] =    theFDCBank.m_dInspVac[ULD_TR_CH2]   		+theFDCBank.m_dAdOffset[ULD_TR_CH2];
		theFDCBank.m_dInspVac_OUT[ULD_STAGE_CH1] = theFDCBank.m_dInspVac[ULD_STAGE_CH1]		+theFDCBank.m_dAdOffset[ULD_STAGE_CH1];
		theFDCBank.m_dInspVac_OUT[ULD_STAGE_CH2] = theFDCBank.m_dInspVac[ULD_STAGE_CH2]		+theFDCBank.m_dAdOffset[ULD_STAGE_CH2];
	*/
	}
	else
	{
		;
	}
#endif

}
