#include "stdafx.h"
#include "DeviceIO.h"
#include "BankGuild/IOParamBank.h"	// Output이 0부터가 아니라 2C0부터 시작해서 그 Offset 계산하느라 사용

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW

#endif
CDeviceIO::CDeviceIO()
{
	m_nRcvSize = -1;
	m_nSendSize = -1;
}

CDeviceIO::~CDeviceIO()
{
}

BOOL CDeviceIO::Open()
{
	if(m_nRcvSize < 0)	m_nRcvSize = MAX_IN_ID;
	if(m_nSendSize < 0)	 m_nSendSize = MAX_OUT_ID;

	m_nRcvSize *= 2;
	m_nSendSize *= 2;

	m_pRcvData	= (WORD*)malloc(m_nRcvSize);
	m_pSendData = (WORD*)malloc(m_nSendSize);

	if(!m_CIF32.Open())
		return FALSE;
	return TRUE;
}

BOOL CDeviceIO::Close()
{
	BOOL bRet = m_CIF32.Close();

	if(m_pRcvData)	free(m_pRcvData);
	if(m_pSendData)	free(m_pSendData);

	return bRet;
}

void CDeviceIO::SetMaxInOutNum( int nMaxInput, int nMaxOutput )
{
	// DeviceNet은 한 국번마다 16개씩 할당된다 [7/5/2016 OSC]
	m_nRcvSize = (nMaxInput/16)+(nMaxInput%16);
	m_nSendSize = (nMaxOutput/16)+(nMaxOutput%16);

	if(m_nRcvSize < 0)	m_nRcvSize = MAX_IN_ID;
	if(m_nSendSize < 0)	 m_nSendSize = MAX_OUT_ID;
	if(m_nRcvSize > MAX_IN_ID) m_nRcvSize = MAX_IN_ID;
	if(m_nSendSize > MAX_OUT_ID) m_nSendSize = MAX_OUT_ID;
}

BOOL CDeviceIO::ReadInBit( int nBit )
{
	BOOL bRet	= FALSE;

	if (nBit < 0 || nBit >= MAX_IN_ID) 
		return bRet;

	int nStationNum	= nBit / DEV_INPUT;

	if(m_CIF32.ExchangeIO(&m_wSendData, m_nSendSize, m_wReadData, m_nRcvSize) != DRV_NO_ERROR)
		return bRet;

	bRet	= (m_wReadData[nStationNum] >> (nBit % 16)) & 0x01;

	return bRet;
}

WORD CDeviceIO::ReadInWord( int nStationNum )
{	
	WORD nRet = 0;

	if(m_CIF32.ExchangeIO(&m_wSendData, m_nSendSize, m_wReadData, m_nRcvSize) != DRV_NO_ERROR)
		return nRet;

	nRet	= m_wReadData[nStationNum];

	return nRet;
}

BOOL CDeviceIO::ReadOutBit( int nBit )
{
	BOOL bRet		= FALSE;

	if (nBit < 0 || nBit >= MAX_OUT_ID) 
		return bRet;

	int nStationNum	= (nBit) / DEV_OUTPUT;

	if(m_CIF32.ExchangeIO(&m_wSendData, m_nSendSize, m_wReadData, m_nRcvSize) != DRV_NO_ERROR)
		return bRet;

	bRet	= (m_wSendData[nStationNum] >> (nBit % 16)) & 0x01;

	return bRet;
}

WORD CDeviceIO::ReadOutWord(int nStationNum)
{	
	WORD nRet = 0;

	if(m_CIF32.ExchangeIO(&m_wSendData, m_nSendSize, m_wReadData, m_nRcvSize) != DRV_NO_ERROR)
		return nRet;

	nRet	= m_wSendData[nStationNum];

	return nRet;
}

void CDeviceIO::WriteOutBit( int nBit, BOOL bVal /*= TRUE*/ )
{
	int nStationNum	= (nBit) / DEV_OUTPUT;
	int nPortNum = 1 << (nBit % DEV_OUTPUT);

	if(bVal)
		m_wSendData[nStationNum]	= m_wSendData[nStationNum] | nPortNum;
	else
		m_wSendData[nStationNum]	= m_wSendData[nStationNum] & (~nPortNum);

	m_CIF32.ExchangeIO(m_wSendData, m_nSendSize, m_wReadData, m_nRcvSize);
}

void CDeviceIO::WriteOutWord(int nStationNum, WORD nVal)
{
	m_wSendData[nStationNum]	= nVal;

	m_CIF32.ExchangeIO(m_wSendData, m_nSendSize, m_wReadData, m_nRcvSize);
}