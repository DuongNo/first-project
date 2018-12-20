#include "StdAfx.h"
#include "DeviceAccura.h"


CDeviceAccura::CDeviceAccura(void) :  m_accClients(*AccuraClients::Create())
{
	 memset(&m_rSumDataBlock[ACCURA_CLIENT_UPS],0,sizeof(m_rSumDataBlock[ACCURA_CLIENT_UPS]));
	 memset(&m_rSumDataBlock[ACCURA_CLIENT_GPS],0,sizeof(m_rSumDataBlock[ACCURA_CLIENT_GPS]));
	 m_nUpsAverageCount= 0;
	 m_nGpsAverageCount = 0;
}


CDeviceAccura::~CDeviceAccura(void)
{
	for (int i = 0; i< m_accClients.Count(); i++)
	{
		m_accClients.RemoveDevice(i);
	}
	delete &m_accClients;
}

void CDeviceAccura::InitAccura()
{
	// 첫번째 전력량계, GPS
	m_accClients.AddDevice( IP_ADDR_UPS );

	// 두번째 전력량계, UPS
	m_accClients.AddDevice( IP_ADDR_GPS );

	m_pClientsArray = m_accClients.GetDevicesAsAllocatedArray( m_nClientCount );		// Client 수량을 받아서 가져온다.

	m_pClientsArray[ ACCURA_CLIENT_UPS ]->SetConnectionCheckTime(1);
	m_pClientsArray[ ACCURA_CLIENT_UPS ]->SetTimeout(1);
	m_pClientsArray[ ACCURA_CLIENT_GPS ]->SetConnectionCheckTime(1);
	m_pClientsArray[ ACCURA_CLIENT_GPS ]->SetTimeout(1);

	m_1PhaseType[ACCURA_CLIENT_UPS] = ACCURA_1PHASE_T;	// 2호기가 T에 들어온다
	m_1PhaseType[ACCURA_CLIENT_GPS] = ACCURA_1PHASE_R;	// 3상이면 R로 해줌

}

void CDeviceAccura::ReadValue(ACCURA_CLIENT client)
{
	AccuraModbusTcpClient* pAccuraClient = NULL;

	pAccuraClient = m_pClientsArray[ client ];		

	unsigned short fetchAndValidity[42];
	unsigned short* selectedIndex		= &fetchAndValidity[0];
	unsigned short* ptValidity			= &fetchAndValidity[1];
	unsigned short* subunitValidity		= &fetchAndValidity[2];
	unsigned short ptData[100];	// 56];
	unsigned short subunitData[150];
#if(AMT_TYPE == 1)
	int nSubIdx = 0;				// 각 Client에 속한 SubUnit(2350 모듈)의 Index, 통상 0 혹은 1을 사용하는데, 우리는 0으로 사용하고 한다. 1은 2300 본체
#else
	int nSubIdx = 1;				// 각 Client에 속한 SubUnit(2350 모듈)의 Index, 통상 0 혹은 1을 사용하는데, 우리는 0으로 사용하고 한다. 1은 2300 본체
#endif

	CString sMsg;

	// Selected Measured data index
	// 선택된 계측 데이터 인덱스, Index selection의 계측 데이터 인덱스가 유효할 경우, 이 레지스터에 선택된 인덱스값이 나타나며
	// 유효하지 않을 경우 FFFF로 읽힌다.
	// 이 레지스터를 읽으면 선택된 계측 데이터를 가지고 온다.
	BOOL bRet = pAccuraClient->ReadHoldingRegisters(11044, 42, fetchAndValidity);
	if ( bRet == TRUE )
	{
		if ( *ptValidity == 1 ) 
		{
			// PT Measured Data : Data Details 섹션의 PT Measured Data 참조
			// Aggregation Selection이 0 ~ 15, 또는 200일 경우에만 사용됨
			//
			// 표시창에 표시되는 기본 전압 데이터를 가져온다.
			bRet = pAccuraClient->ReadHoldingRegisters(11101, 56, ptData);		// 데이터 BYTE수는 현재 56개 밖에 없다.
			if ( bRet == TRUE )
			{
				m_rDataBlock[client].nState = 1;	

				if ( m_1PhaseType[client] == ACCURA_1PHASE_T )						// T상을 사용한다.
				{
					m_rDataBlock[client].dbVoltage_R	= ToFloat(ptData, 12);		// C상 (T상) 라인 전압
					m_rDataBlock[client].dbVoltage_S	= ToFloat(ptData, 10);		// B상 (S상) 라인 전압
					m_rDataBlock[client].dbVoltage_T	= ToFloat(ptData, 8);		// A상 (R상) 라인 전압
				}
				else if ( m_1PhaseType[client] == ACCURA_1PHASE_S )					// S상 을 사용하는가 ?
				{
					m_rDataBlock[client].dbVoltage_R	= ToFloat(ptData, 10);		// B상 (S상) 라인 전압
					m_rDataBlock[client].dbVoltage_S	= ToFloat(ptData, 8);		// A상 (R상) 라인 전압
					m_rDataBlock[client].dbVoltage_T	= ToFloat(ptData, 12);		// C상 (T상) 라인 전압
				}
				else			// 그 밖에는 원래대로 R상을 그대로 사용한다.
				{
					m_rDataBlock[client].dbVoltage_R	= ToFloat(ptData, 8);		// A상 (R상) 라인 전압
					m_rDataBlock[client].dbVoltage_S	= ToFloat(ptData, 10);		// B상 (S상) 라인 전압
					m_rDataBlock[client].dbVoltage_T	= ToFloat(ptData, 12);		// C상 (T상) 라인 전압
				}
				sMsg.Format(_T("Client:%d Ret:%d, Va=%.3f, Vb=%.3f, Vc=%.3f")
					, client
					, bRet
					, m_rDataBlock[client].dbVoltage_R, m_rDataBlock[client].dbVoltage_S, m_rDataBlock[client].dbVoltage_T );
			}
			else
				sMsg.Format(_T("* Client:%d PT Measure data read Fail "), client);
		}
		else
			sMsg.Format(_T("* Client:%d State is Invalidity "), client);

		// 하위 Accura 2350 unit의 종류별로 읽는 위치를 달리하여 처리한다.
		// 0이면 3상, 1이면 단상
		// nSubIdx는 현재 0으로 고정
		if ( subunitValidity[ nSubIdx ] == 0)		// 3상 인가 ?
		{
			// Sub Unit의 Measured Data를 읽어온다.
			// 주소는 11201 부터 Unit 0의 주소가 시작되며, 각 Unit 별로 150 Bytes 단위로 증가한다.
			bRet = pAccuraClient->ReadHoldingRegisters((11201 + (nSubIdx * 150)), 141, subunitData);
			if ( bRet == TRUE )		// 성공적으로 읽었다면,
			{
				m_rDataBlock[client].nState = 1;											// 통신 정상

				m_rDataBlock[client].dbCurrent_R	= ToFloat(subunitData, 0);	// A상 (R상) 전류
				m_rDataBlock[client].dbCurrent_S	= ToFloat(subunitData, 2);	// B상 (S상) 전류
				m_rDataBlock[client].dbCurrent_T	= ToFloat(subunitData, 4);	// C상 (T상) 전류

				m_rDataBlock[client].dbWatt_R	= ToFloat(subunitData, 60);	// R상 전력량
				m_rDataBlock[client].dbWatt_S	= ToFloat(subunitData, 62);	// S상 전력량
				m_rDataBlock[client].dbWatt_T	= ToFloat(subunitData, 64);	// T상 전력량
				
				//2017-08-31,SKCHO, 평균값 사용으로 변경하여 삭제
				//2017.07.22. JSJUNG, 인터락 문제로 인하여 INSTANTANEOUS_POWER 3이상 값은 무시_고객사요청
				/*if(ToFloat(subunitData, 66) < 3 && ToFloat(subunitData, 66) > 0)
				{
					m_rDataBlock[client].dbWattTotal	= ToFloat(subunitData, 66);	// 순시 전력량
				}
				else
				{
					m_rDataBlock[client].dbWattTotal = 0.2;
				}*/

				m_rDataBlock[client].dbWattTotal	= ToFloat(subunitData, 66);	// 순시 전력량
				m_rDataBlock[client].dbWattNet	= (double)ToInt32(subunitData, 92);	// 적산 전력량

				sMsg.AppendFormat(_T(", SubID:%d Ia=%.3f, Ib=%.3f, Ic=%.3f, Pa=%.3f, Pb=%.3f, Pc=%.3f, PTot=%.3f, PNet=%.3f")
					, nSubIdx
					, m_rDataBlock[client].dbCurrent_R, m_rDataBlock[client].dbCurrent_S, m_rDataBlock[client].dbCurrent_T
					, m_rDataBlock[client].dbWatt_R, m_rDataBlock[client].dbWatt_S, m_rDataBlock[client].dbWatt_T
					, m_rDataBlock[client].dbWattTotal, m_rDataBlock[client].dbWattNet );
			}
			else
				sMsg.AppendFormat(_T(", * SubID:%d Detail measured data read Fail"), nSubIdx);
		}
		else if (subunitValidity[ nSubIdx ] == 1) // 1P : 단상인가 ?
		{
			bRet = pAccuraClient->ReadHoldingRegisters((11201 + (nSubIdx * 150)), 56, subunitData);

			if ( bRet == TRUE )		// 성공적으로 읽었다면,
			{

				m_rDataBlock[client].dbCurrent_R	= ToFloat(subunitData, 0);	// A상 (R상) 전류
				m_rDataBlock[client].dbWatt_R	= ToFloat(subunitData, 16);	// A상 (R상) 전력량
				

				//2017-08-31,SKCHO, 평균값 사용으로 변경하여 삭제
				//2017.07.22. JSJUNG, 인터락 문제로 인하여 INSTANTANEOUS_POWER 3이상 값은 무시_고객사요청
				/*if(ToFloat(subunitData, 16) <3 && ToFloat(subunitData, 16) >0 )
				{
					m_rDataBlock[client].dbWattTotal	= ToFloat(subunitData, 16);	// 순시 전력량, 유효전력
				}
				else
				{
					m_rDataBlock[client].dbWattTotal = 0.2;
				}*/

				m_rDataBlock[client].dbWattTotal	= ToFloat(subunitData, 16);	// 순시 전력량, 유효전력
				m_rDataBlock[client].dbWattNet	= (double)ToInt32(subunitData, 30);	// 적산 전력량

				sMsg.AppendFormat(_T(", SubID:%d I=%.3f, P=%.3f, PTot=%.3f, PNet=%.3f")
					, nSubIdx
					, m_rDataBlock[client].dbCurrent_R 
					, m_rDataBlock[client].dbWatt_R
					, m_rDataBlock[client].dbWattTotal, m_rDataBlock[client].dbWattNet );

				//m_debug.Log(_T("1 [0] %x, %x, [16] %x, %x, [30] %x, %x"),
				//	ptData[0], ptData[1], ptData[16], ptData[17], ptData[30], ptData[31]);

			}
			else
				sMsg.AppendFormat(_T(", * SubID:%d Detail measured data read Fail"), nSubIdx);
		}//of if 단상인가 ?
		else
			sMsg.Format(_T("* Client:%d Validity State read Fail "), client);

	}
// 	theLog[LOG_ACCURA].AddBuf(sMsg);
}

float CDeviceAccura::ToFloat(unsigned short* buffer, int index)
{
	unsigned short reversed[2];

	reversed[0] = buffer[index + 1];
	reversed[1] = buffer[index + 0];

	return *(float*)reversed;
}

int CDeviceAccura::ToInt32(unsigned short* buffer, int index)
{
	unsigned short reversed[2];

	reversed[0] = buffer[index + 1];
	reversed[1] = buffer[index + 0];

	return *(int*)reversed;
}




void CDeviceAccura::UPSAverageValue(int nAvgNum)
{
	double MAX_VOLT		=theConfigBank.m_Option.m_dFDCRange[UPS_VOLT_MAX];
	double MIN_VOLT		=theConfigBank.m_Option.m_dFDCRange[UPS_VOLT_MIN];
	double MAX_CURRENT  =theConfigBank.m_Option.m_dFDCRange[UPS_CURRENT_MAX];
	double MIN_CURRENT  =theConfigBank.m_Option.m_dFDCRange[UPS_CURRENT_MIN];
	double MAX_POWER    =theConfigBank.m_Option.m_dFDCRange[UPS_POWER_MAX];
	double MIN_POWER    =theConfigBank.m_Option.m_dFDCRange[UPS_POWER_MIN];

	double dCurrent= 0 ;
	double dVolt = 0;
	double dWatt = 0;

	if(nAvgNum < 1 ) return;

	if(m_nUpsAverageCount < nAvgNum)
	{
		dCurrent = m_rDataBlock[ACCURA_CLIENT_UPS].dbCurrent_R;
		dVolt = m_rDataBlock[ACCURA_CLIENT_UPS].dbVoltage_R;
		dWatt = m_rDataBlock[ACCURA_CLIENT_UPS].dbWattTotal;

		if(dCurrent > MAX_CURRENT || dCurrent < MIN_CURRENT)
		{
			dCurrent = 0.5;   //범위밖이면 10.0A로 고정
		}		
		m_rSumDataBlock[ACCURA_CLIENT_UPS].dbCurrent_R += dCurrent;
		
		m_rSumDataBlock[ACCURA_CLIENT_UPS].dbCurrent_S += m_rDataBlock[ACCURA_CLIENT_UPS].dbCurrent_S;
		m_rSumDataBlock[ACCURA_CLIENT_UPS].dbCurrent_T += m_rDataBlock[ACCURA_CLIENT_UPS].dbCurrent_T;

		if(dVolt > MAX_VOLT || dVolt < MIN_VOLT)
		{
			dVolt = 220.0;   //범위밖이면 220.0V로 고정
		}
		m_rSumDataBlock[ACCURA_CLIENT_UPS].dbVoltage_R += dVolt;

		m_rSumDataBlock[ACCURA_CLIENT_UPS].dbVoltage_S += m_rDataBlock[ACCURA_CLIENT_UPS].dbVoltage_S;
		m_rSumDataBlock[ACCURA_CLIENT_UPS].dbVoltage_T += m_rDataBlock[ACCURA_CLIENT_UPS].dbVoltage_T;

		if(dWatt > MAX_POWER || dWatt < MIN_POWER)
		{
			dWatt = 0.2;   //범위밖이면 0.2W로 고정
		}
		m_rSumDataBlock[ACCURA_CLIENT_UPS].dbWattTotal += dWatt;

		m_rSumDataBlock[ACCURA_CLIENT_UPS].dbWattNet   += m_rDataBlock[ACCURA_CLIENT_UPS].dbWattNet  ;		
		m_rSumDataBlock[ACCURA_CLIENT_UPS].dbWatt_R    += m_rDataBlock[ACCURA_CLIENT_UPS].dbWatt_R   ;
		m_rSumDataBlock[ACCURA_CLIENT_UPS].dbWatt_S    += m_rDataBlock[ACCURA_CLIENT_UPS].dbWatt_S   ;
		m_rSumDataBlock[ACCURA_CLIENT_UPS].dbWatt_T    += m_rDataBlock[ACCURA_CLIENT_UPS].dbWatt_T   ;

		m_nUpsAverageCount++;
	}
	else
	{

		theFDCBank.m_rDataBlock[ACCURA_CLIENT_UPS].dbCurrent_R = (double)(m_rSumDataBlock[ACCURA_CLIENT_UPS].dbCurrent_R / nAvgNum);
		theFDCBank.m_rDataBlock[ACCURA_CLIENT_UPS].dbCurrent_S = (double)(m_rSumDataBlock[ACCURA_CLIENT_UPS].dbCurrent_S / nAvgNum);
		theFDCBank.m_rDataBlock[ACCURA_CLIENT_UPS].dbCurrent_T = (double)(m_rSumDataBlock[ACCURA_CLIENT_UPS].dbCurrent_T / nAvgNum);
		theFDCBank.m_rDataBlock[ACCURA_CLIENT_UPS].dbVoltage_R = (double)(m_rSumDataBlock[ACCURA_CLIENT_UPS].dbVoltage_R / nAvgNum);
		theFDCBank.m_rDataBlock[ACCURA_CLIENT_UPS].dbVoltage_S = (double)(m_rSumDataBlock[ACCURA_CLIENT_UPS].dbVoltage_S / nAvgNum);
		theFDCBank.m_rDataBlock[ACCURA_CLIENT_UPS].dbVoltage_T = (double)(m_rSumDataBlock[ACCURA_CLIENT_UPS].dbVoltage_T / nAvgNum);
		theFDCBank.m_rDataBlock[ACCURA_CLIENT_UPS].dbWattNet   = (double)(m_rSumDataBlock[ACCURA_CLIENT_UPS].dbWattNet   / nAvgNum);
		theFDCBank.m_rDataBlock[ACCURA_CLIENT_UPS].dbWattTotal = (double)(m_rSumDataBlock[ACCURA_CLIENT_UPS].dbWattTotal / nAvgNum);
		theFDCBank.m_rDataBlock[ACCURA_CLIENT_UPS].dbWatt_R    = (double)(m_rSumDataBlock[ACCURA_CLIENT_UPS].dbWatt_R   / nAvgNum);
		theFDCBank.m_rDataBlock[ACCURA_CLIENT_UPS].dbWatt_S    = (double)(m_rSumDataBlock[ACCURA_CLIENT_UPS].dbWatt_S   / nAvgNum);
		theFDCBank.m_rDataBlock[ACCURA_CLIENT_UPS].dbWatt_T    = (double)(m_rSumDataBlock[ACCURA_CLIENT_UPS].dbWatt_T   / nAvgNum);;
	
	   m_nUpsAverageCount  = 0;
	   memset(&m_rSumDataBlock[ACCURA_CLIENT_UPS],0,sizeof(m_rSumDataBlock[ACCURA_CLIENT_UPS]));
	}

}


void CDeviceAccura::GPSAverageValue(int nAvgNum)
{
	double MAX_VOLT		=theConfigBank.m_Option.m_dFDCRange[GPS_VOLT_MAX];
	double MIN_VOLT		=theConfigBank.m_Option.m_dFDCRange[GPS_VOLT_MIN];
	double MAX_CURRENT  =theConfigBank.m_Option.m_dFDCRange[GPS_CURRENT_MAX];
	double MIN_CURRENT  =theConfigBank.m_Option.m_dFDCRange[GPS_CURRENT_MIN];
	double MAX_POWER    =theConfigBank.m_Option.m_dFDCRange[GPS_POWER_MAX];
	double MIN_POWER    =theConfigBank.m_Option.m_dFDCRange[GPS_POWER_MIN];

	double dCurrent= 0 ;
	double dVolt = 0;
	double dWatt = 0;
	if(nAvgNum < 1 ) return;

	if(m_nGpsAverageCount < nAvgNum)
	{
		dCurrent = m_rDataBlock[ACCURA_CLIENT_GPS].dbCurrent_R;
		dVolt = m_rDataBlock[ACCURA_CLIENT_GPS].dbVoltage_R;
		dWatt = m_rDataBlock[ACCURA_CLIENT_GPS].dbWattTotal;

		if(dCurrent > MAX_CURRENT || dCurrent < MIN_CURRENT)
		{
			dCurrent = 0.5;   //범위밖이면 10.0A로 고정
		}		
		m_rSumDataBlock[ACCURA_CLIENT_GPS].dbCurrent_R += dCurrent;

		//사용하지 않음
		m_rSumDataBlock[ACCURA_CLIENT_GPS].dbCurrent_S += m_rDataBlock[ACCURA_CLIENT_GPS].dbCurrent_S;
		m_rSumDataBlock[ACCURA_CLIENT_GPS].dbCurrent_T += m_rDataBlock[ACCURA_CLIENT_GPS].dbCurrent_T;

		if(dVolt > MAX_VOLT || dVolt < MIN_VOLT)
		{
			dVolt = 220.0;   //범위밖이면 220.0V로 고정
		}
		m_rSumDataBlock[ACCURA_CLIENT_GPS].dbVoltage_R += dVolt;

		m_rSumDataBlock[ACCURA_CLIENT_GPS].dbVoltage_S += m_rDataBlock[ACCURA_CLIENT_GPS].dbVoltage_S;
		m_rSumDataBlock[ACCURA_CLIENT_GPS].dbVoltage_T += m_rDataBlock[ACCURA_CLIENT_GPS].dbVoltage_T;


		if(dWatt > MAX_POWER || dWatt < MIN_POWER)
		{
			dWatt = 0.2;   //범위밖이면 0.2W로 고정
		}
		m_rSumDataBlock[ACCURA_CLIENT_GPS].dbWattTotal += dWatt;

		m_rSumDataBlock[ACCURA_CLIENT_GPS].dbWattNet   += m_rDataBlock[ACCURA_CLIENT_GPS].dbWattNet;		
		m_rSumDataBlock[ACCURA_CLIENT_GPS].dbWatt_R    += m_rDataBlock[ACCURA_CLIENT_GPS].dbWatt_R   ;
		m_rSumDataBlock[ACCURA_CLIENT_GPS].dbWatt_S    += m_rDataBlock[ACCURA_CLIENT_GPS].dbWatt_S   ;
		m_rSumDataBlock[ACCURA_CLIENT_GPS].dbWatt_T    += m_rDataBlock[ACCURA_CLIENT_GPS].dbWatt_T   ;

		m_nGpsAverageCount++;
	}
	else
	{
		theFDCBank.m_rDataBlock[ACCURA_CLIENT_GPS].dbCurrent_R = (double)(m_rSumDataBlock[ACCURA_CLIENT_GPS].dbCurrent_R / nAvgNum);
		theFDCBank.m_rDataBlock[ACCURA_CLIENT_GPS].dbCurrent_S = (double)(m_rSumDataBlock[ACCURA_CLIENT_GPS].dbCurrent_S / nAvgNum);
		theFDCBank.m_rDataBlock[ACCURA_CLIENT_GPS].dbCurrent_T = (double)(m_rSumDataBlock[ACCURA_CLIENT_GPS].dbCurrent_T / nAvgNum);
		theFDCBank.m_rDataBlock[ACCURA_CLIENT_GPS].dbVoltage_R = (double)(m_rSumDataBlock[ACCURA_CLIENT_GPS].dbVoltage_R / nAvgNum);
		theFDCBank.m_rDataBlock[ACCURA_CLIENT_GPS].dbVoltage_S = (double)(m_rSumDataBlock[ACCURA_CLIENT_GPS].dbVoltage_S / nAvgNum);
		theFDCBank.m_rDataBlock[ACCURA_CLIENT_GPS].dbVoltage_T = (double)(m_rSumDataBlock[ACCURA_CLIENT_GPS].dbVoltage_T / nAvgNum);
		theFDCBank.m_rDataBlock[ACCURA_CLIENT_GPS].dbWattNet   = (double)(m_rSumDataBlock[ACCURA_CLIENT_GPS].dbWattNet   / nAvgNum);
		theFDCBank.m_rDataBlock[ACCURA_CLIENT_GPS].dbWattTotal = (double)(m_rSumDataBlock[ACCURA_CLIENT_GPS].dbWattTotal / nAvgNum);
		theFDCBank.m_rDataBlock[ACCURA_CLIENT_GPS].dbWatt_R    = (double)(m_rSumDataBlock[ACCURA_CLIENT_GPS].dbWatt_R   / nAvgNum);
		theFDCBank.m_rDataBlock[ACCURA_CLIENT_GPS].dbWatt_S    = (double)(m_rSumDataBlock[ACCURA_CLIENT_GPS].dbWatt_S   / nAvgNum);
		theFDCBank.m_rDataBlock[ACCURA_CLIENT_GPS].dbWatt_T    = (double)(m_rSumDataBlock[ACCURA_CLIENT_GPS].dbWatt_T   / nAvgNum);
	
	   m_nGpsAverageCount  = 0;
	   memset(&m_rSumDataBlock[ACCURA_CLIENT_GPS],0,sizeof(m_rSumDataBlock[ACCURA_CLIENT_GPS]));
	}

}