#pragma once

#include "Accura/AccuraClient.h"

#define IP_ADDR_UPS	"192.168.10.210"
#define IP_ADDR_GPS	"192.168.10.211"

enum ACCURA_CLIENT
{
	ACCURA_CLIENT_UPS = 0,
	ACCURA_CLIENT_GPS,
	ACCURA_CLIENT_MAX
};

// 3상이 아닌 단상일 경우 값들이 R, S, T 중 한곳에 들어오는데 그게 
// 설비별로 일정치 않다고 하는 거 같아 Option화 [12/7/2016 OSC]
enum ACCURA_1PHASE_TYPE
{
	ACCURA_1PHASE_R = 0,
	ACCURA_1PHASE_S,
	ACCURA_1PHASE_T,
	ACCURA_1PHASE_MAX
};


typedef struct _TAccuraData 
{
	int	nState;					// 통신 상태, 통신이 이루어지지 않았으면 0, 한 번이라도 통신이 되었다며 1 로 지정된다.

	double dbVoltage_R;			// R상 전압
	double dbVoltage_S;			// S상 전압
	double dbVoltage_T;			// T상 전압

	double dbCurrent_R;			// R상 전류
	double dbCurrent_S;			// S상 전류
	double dbCurrent_T;			// T상 전류

	double dbWatt_R;				// R상 전력량
	double dbWatt_S;				// S상 전력량
	double dbWatt_T;				// T상 전력량
	double dbWattTotal;			// 순시 전력량
	double dbWattNet;				// 적산 전력량

	_TAccuraData()
	{
		nState			= 0;		// 통신하기 전 상태임

		dbVoltage_R		= 0.0;	// R상 전압
		dbVoltage_S		= 0.0;	// S상 전압
		dbVoltage_T		= 0.0;	// T상 전압

		dbCurrent_R		= 0.0;	// R상 전류
		dbCurrent_S		= 0.0;	// S상 전류
		dbCurrent_T		= 0.0;	// T상 전류

		dbWatt_R			= 0.0;	// R상 전력량
		dbWatt_S			= 0.0;	// S상 전력량
		dbWatt_T			= 0.0;	// T상 전력량
		dbWattTotal		= 0.0;	// 순시 전력량
		dbWattNet		= 0.0;	// 적산 전력량
	};

} TAccuraData;

class CDeviceAccura
{
public:
	CDeviceAccura(void);
	~CDeviceAccura(void);

	AccuraClients&	m_accClients;
	AccuraModbusTcpClient** m_pClientsArray;
	int m_nClientCount;

	int m_nUpsAverageCount;
	int m_nGpsAverageCount;

	TAccuraData		m_rDataBlock[ACCURA_CLIENT_MAX];	// 데이터를 저장하는 구조체 변수
	TAccuraData		m_rSumDataBlock[ACCURA_CLIENT_MAX];	// 데이터를 저장하는 구조체 변수
	
	ACCURA_1PHASE_TYPE m_1PhaseType[ACCURA_CLIENT_MAX];

	void InitAccura();

	void ReadValue(ACCURA_CLIENT client);
	float ToFloat(unsigned short* buffer, int index);
	int ToInt32(unsigned short* buffer, int index);
	void UPSAverageValue(int nAvgNum);
	void GPSAverageValue(int nAvgNum);
};

