#pragma once

#include "Serial/CommThread.h"

class CDeviceSerialVACUUM
{
	// A/D Board 하나당 최대 32ch까지 가능
	// 1Slot당 4Ch로 최대 8Slot
	enum 
	{
		eMAX_ADSlot		= 8,		// 모두 2열로 4Slot씩 구성되어있음, 최대 8개 설정가능 
		eMAX_ADChannel	= 4,		// A/D 보드에서 한 번 읽었을 떄 최대 읽을 수 있는 수량은 8개이다.

		eLEN_STX		= 1,				// STX 길이
		eLEN_Header		= 8,				// Header
		eLEN_Data		= 4,				// 데이터 값이 길이
		eLEN_Delimiter	= 1,

		ePOS_ResCode	= 6,				// 응답 코드가 위치한 곳
		ePOS_DataStart	= eLEN_STX + eLEN_Header,		// = 9
	};

private:
	CCommThread m_Serial;
	CString m_strSerialName;
	int m_nReadCount;
	CString	m_sRcvData[eMAX_ADSlot][eMAX_ADChannel];	// 실제 수신된 문자
	double m_dOffset;			// 전압강하로 발생하는 Offset
public:
	int m_nReadSlot;
	double	m_dbVacValue[eMAX_ADSlot][eMAX_ADChannel];	// 한 번에 읽을 수 있는 초대 수량을 기본으로 사용한다. eMAX_VACCount];		// 읽어 올 진공센서 수량

	CDeviceSerialVACUUM();
	~CDeviceSerialVACUUM(void);


	BOOL SerialOpen(short nPortNo);
	void SerialClose();
	BOOL IsOpen();

	// A/D보드로부터 232포트까지 선 상태에 따라 전압강하가 일어나서
	// 실제 수치보다 일정량 모자르게 받는다.
	// 그걸 메꾸기 위한 Offset
	void SetValueOffset(double dOffset) { m_dOffset = dOffset; }	

	BOOL ParsingReciveValue();
	void ClearQueue();


	BYTE GetChecksum(BYTE* pData, int nLen);
	DWORD SendReadCommand(int nCount);
	DWORD SendRangeSetCommand(int nCh, int nMode);

	CCommThread GetCommThreadHandler() {return m_Serial;};
	void SetSerialName(CString strValue);

	int m_nVaccAverageCount;
	double m_dSumVacValue[eMAX_ADSlot][eMAX_ADChannel];
	void AverageUDVacValue(int nAvgNum);
};

