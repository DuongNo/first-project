#pragma once

#include "Serial/CommThread.h"

#define ERR_VALUE	-9999

enum DEVICE_TEMPERATURE_MODE
{	
	DEVICE_TEMPERATURE_PRESS_VALUE = 0,
	DEVICE_TEMPERATURE_REFERENCE_SET,
	DEVICE_TEMPERATURE_HOLD_START,	
	DEVICE_TEMPERATURE_HOLD_STOP,
	DEVICE_TEMPERATURE_MAX
};

class CDeviceSerialTemperature
{
private:
	CCommThread m_Serial;
	CString m_strSerialName;
public:
	CDeviceSerialTemperature(CString strSerialName = _T(""));
	~CDeviceSerialTemperature(void);


		// 튀는 노이즈성 데이터를 방지하기 위한 울타리 경계
	int		m_nBoundLimit;						// 몇회 이상 상/하한 기준으로 반복되어어야 인정할 것인가 ?
	int		m_nUBoundCount;						// 상한으로 튀는 값 연속 발생 횟수
	int		m_nLBoundCount;						// 하한값 밑으로 튀는 값 연속 발생 횟수
	double	m_dbUBoundLimit;					// 상한으로 튀는 값 기준
	double	m_dbLBoundLimit;					// 하한으로 튀는 값 기준

	double	m_dbTempValue[eMAX_TEMPCount];		// 읽어온 온도센서 값 저장
	CString	m_sRcvData[eMAX_TEMPCount];			// 실제 수신된 데이터

	int     m_nDeviceAddr;
    double GetTempValue(int nIdx);
	void IncDeviceAddr(void);   //주소 증가


	BOOL SerialOpen(short nPortNo);
	void SerialClose();
	BOOL IsOpen();
	BYTE GetChecksum(BYTE* pData, int nLen);
	void SendCommand(DEVICE_TEMPERATURE_MODE mode, BYTE* buf, int nBufSize);
	void SendCommand_TempValue(int nDeviceNum);
	void SendCommand_Reference();
	void SendCommand_Hold_Start();
	void SendCommand_Hold_Stop();
	BOOL ParsingReciveValue();
	CCommThread GetCommThreadHandler() {return m_Serial;};
	CString GetSerialName() {return m_strSerialName;};
	void SetSerialName(CString strValue);
	SERIAL_MODE m_serialMode;

	int m_nTempAverageCount[eMAX_TEMPCount];
	double m_dSumTempValue[eMAX_TEMPCount];
	void TempAverageValue(int nIdx,int nAvgNum);
};

