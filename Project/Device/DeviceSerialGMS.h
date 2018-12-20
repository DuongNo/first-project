#pragma once

#include "Serial/CommThread.h"

#define GMS_NICKNAME_JIG_GMS	"SDV_AMT_GMS_UNIT_MAIN_12345"	// 무조건 27글자
#define GMS_NICKNAME_BZONE_PGMS	"FTI_CH002_PGMS_BZONE_123456"	// 무조건 27글자
#define GMS_NICKNAME_DZONE_PGMS	"FTI_CH003_PGMS_DZONE_123456"	// 무조건 27글자

#define PGMS_BUZZER_ON	'1'
#define PGMS_BUZZER_OFF	'0'

enum GMS_MODULE_CH
{
	GMS_CH001_GMS = 0,
	GMS_CH002_PGMS,
	GMS_CH003_PGMS,
	GMS_MODULE_CH_MAX
};

enum GMS_SUB_CH
{
	GMS_A_JIG = 0,	// A Jig
	GMS_B_JIG,		// B Jig
	GMS_C_JIG,		// C Jig
	GMS_D_JIG,		// D Jig
	GMS_SUB_CH5,		// 미사용
	GMS_SUB_CH_MAX
};

#define GMS_VALUE_SIZE	4
#define GMS_VALUE_ERR			"999"
#define GMS_VALUE_CH_DISABLE	"9997"
#define GMS_VALUE_RANGE_OVER	"9998"
#define GMS_VALUE_OPEN			"9999"

class CDeviceSerialGMS
{
private:
	CCommThread m_Serial;
	CString m_strSerialName;
public:
	CDeviceSerialGMS(CString strSerialName = _T(""));
	~CDeviceSerialGMS(void);


	BOOL SerialOpen(short nPortNo);
	void SerialClose();
	BOOL IsOpen();

	void SendCommand_Read(GMS_MODULE_CH ch);	// bGMS is TRUE => GMS, else PGMS
	BYTE GetChecksum(BYTE* pData, int nLen);
	BOOL ParsingReciveValue();

	CString m_LastGMSValue[GMS_SUB_CH_MAX];
	CString m_LastBZonePGMS, m_LastDZonePGMS;	// 0:링NG S/W On
												// 1:링OK S/W On
												// 2:링NG S/W Off
												// 3:링OK S/W Off


	double GetLoadValue();
	CCommThread GetCommThreadHandler() {return m_Serial;};
	CString GetSerialName() {return m_strSerialName;};
	void SetSerialName(CString strValue);

	int m_nGMSAverageCount;
	double m_dSumGMSValue[GMS_SUB_CH_MAX];
	double m_dGMSValue[GMS_SUB_CH_MAX];
	void AverageValue(int nAvgNum);
};

