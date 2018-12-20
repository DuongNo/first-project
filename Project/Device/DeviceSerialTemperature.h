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


		// Ƣ�� ����� �����͸� �����ϱ� ���� ��Ÿ�� ���
	int		m_nBoundLimit;						// ��ȸ �̻� ��/���� �������� �ݺ��Ǿ��� ������ ���ΰ� ?
	int		m_nUBoundCount;						// �������� Ƣ�� �� ���� �߻� Ƚ��
	int		m_nLBoundCount;						// ���Ѱ� ������ Ƣ�� �� ���� �߻� Ƚ��
	double	m_dbUBoundLimit;					// �������� Ƣ�� �� ����
	double	m_dbLBoundLimit;					// �������� Ƣ�� �� ����

	double	m_dbTempValue[eMAX_TEMPCount];		// �о�� �µ����� �� ����
	CString	m_sRcvData[eMAX_TEMPCount];			// ���� ���ŵ� ������

	int     m_nDeviceAddr;
    double GetTempValue(int nIdx);
	void IncDeviceAddr(void);   //�ּ� ����


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

