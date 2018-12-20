#pragma once

#include "IO/CIF32Ex.h"

// 임의로 정한 최대치. 바꿔도 상관없음
#define DEV_INPUT				16
#define DEV_OUTPUT				16


enum IO_BIT
{
	IO_OFF=0,
	IO_ON,
};

class CDeviceIO
{
public:
	CDeviceIO();
	~CDeviceIO();

protected:
	int			m_nRcvSize/*Byte*/;
	int			m_nSendSize/*Byte*/;
	WORD* 		m_pRcvData;
    WORD*	 	m_pSendData;

	CCIF32Ex	m_CIF32;

public:
	BOOL		Open();
	BOOL		Close();

	void		SetMaxInOutNum(int nMaxInput, int nMaxOutput);

	BOOL		ReadInBit(int nBit);
	WORD		ReadInWord(int nStationNum);

	void		WriteOutBit(int nBit, BOOL bVal = TRUE);
	void		WriteOutWord(int nStationNum, WORD nVal);

	BOOL		ReadOutBit(int nBit);
	WORD		ReadOutWord(int nStationNum);

	WORD 		m_wReadData[MAX_IN_ID];
	WORD	 	m_wSendData[MAX_OUT_ID];
};
