#include "StdAfx.h"
#include "DeviceMelsec.h"
#include "BankGuild\ProcDataBank.h"
#include "CheetahApp.h"

// 사용 순서. 
// 1. Offset설정. ( SetZOffset, SetXOffset, SetYOffset, SetTOffset )
// 2. End Position 설정. ( SetEndPosition ) 
// 3. Move Start Bit On. ( MoveStart )
// 4. Move Finish Check. ( CheckFinish )
// 5. Move Finish Bit & Move Start Bit Off. ( OnTimer ) 

cDeviceMelsec::cDeviceMelsec(void)
{
	m_CCInfo.m_iNetChanel = INT_MNET_CCLINK_CHANNEL;
	m_CCInfo.m_iNetMode = INT_MNET_MODE;
	m_CCInfo.m_iNetWork = INT_MNET_NETWORK;
	m_CCInfo.m_iStation = INT_MNET_STATION;
	m_CCInfo.m_lNetPath = INT_MNET_CCLINK_PATH;

	m_OPInfo.m_iNetChanel = INT_MNET_OPTICAL_CHANNEL;
	m_OPInfo.m_iNetMode = INT_MNET_OPTICAL_MODE;
	m_OPInfo.m_iNetWork = INT_MNET_OPTICAL_NETWORK; // 2017.2.4 bgkim 
	m_OPInfo.m_iStation = INT_MNET_OPTICAL_STATION; // 2017.2.4 bgkim
	m_OPInfo.m_lNetPath = INT_MNET_OPTICAL_PATH;


	//m_OPInfo.m_iNetWork = INT_MNET_OPTICAL_NETWORK; // 2017.2.4 bgkim 
	//m_OPInfo.m_iStation = INT_MNET_OPTICAL_STATION; // 2017.2.4 bgkim

	m_strSndBuf = _T("");
	m_strRcvBuf = _T("");

	m_bCCIsConnected = FALSE;
	m_bOPIsConnected = FALSE;
}

cDeviceMelsec::~cDeviceMelsec(void)
{

}

BEGIN_MESSAGE_MAP(cDeviceMelsec, CMelsecNet)
	ON_WM_TIMER()
END_MESSAGE_MAP()


BOOL cDeviceMelsec::Open(BOOL bRobot)
{
	if (bRobot == TRUE)
	{
		CCSetConnected(FALSE);

		if(MelsecOpen(m_CCInfo.m_iNetChanel, m_CCInfo.m_iNetMode, &m_CCInfo.m_lNetPath, ROBOT) == 0)
		{
			CCSetConnected(TRUE);
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		OPSetConnected(FALSE);

		if(MelsecOpen(m_OPInfo.m_iNetChanel, m_OPInfo.m_iNetMode, &m_OPInfo.m_lNetPath, UPPER_EQP) == 0)
		{
			OPSetConnected(TRUE);
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	return FALSE; 
}

void cDeviceMelsec::Close(BOOL bRobot)
{
	if (bRobot == TRUE)
	{
		CCSetConnected(FALSE);
		MelsecClose( m_CCInfo.m_lNetPath);
	}
	else
	{
		OPSetConnected(FALSE);
		MelsecClose( m_OPInfo.m_lNetPath);
	}
}

short cDeviceMelsec::UpperBitOn(short devno)
{
	return MelsecDevSet(m_OPInfo.m_iNetWork, m_OPInfo.m_iStation, DEV_TYP_OPTICAL_BIT, devno, UPPER_EQP);
}

short cDeviceMelsec::RobotBitOn(short devno)
{
	return MelsecDevSet(m_CCInfo.m_iNetWork, m_CCInfo.m_iStation, DEV_TYP_WRIT, devno, ROBOT);
}

short cDeviceMelsec::UpperBitOff(short devno)
{
	return MelsecDevRst(m_OPInfo.m_iNetWork, m_OPInfo.m_iStation, DEV_TYP_OPTICAL_BIT, devno, UPPER_EQP);

}
short cDeviceMelsec::RobotBitOff(short devno)
{
	return MelsecDevRst(m_CCInfo.m_iNetWork, m_CCInfo.m_iStation, DEV_TYP_WRIT, devno, ROBOT);
}

short cDeviceMelsec::UpperBitRecv(short devno)
{
	int nData = 0;
	MelsecDevReceive(m_OPInfo.m_iNetWork, m_OPInfo.m_iStation, DEV_TYP_OPTICAL_BIT, devno, &nData, UPPER_EQP);

	return nData;
}

short cDeviceMelsec::RobotBitRecv(short devno)
{
	int nData = 0;
	BOOL bFlag = FALSE;
	MelsecDevReceive(m_CCInfo.m_iNetWork, m_CCInfo.m_iStation, DEV_TYP_READ, devno, &nData, ROBOT);

	if(nData == 1)
	{
		bFlag = TRUE;
	}
	else
	{
		bFlag = FALSE;
	}
	return bFlag;
}

void cDeviceMelsec::UpperWordRecvBuf(short devno, short nSizebyByte, PSHORT pData)
{
	if(OPIsConnected() == FALSE)
		return;

	// WORD단위니까 Byte Size는 전부 짝수로 처리해야 함
	short nSize = nSizebyByte;
	if(nSize%2)	nSize++;

	short buf[128];
	short ret;
	memset(buf, NULL, sizeof(short)*128);
	ret = MelsecReceive(m_OPInfo.m_iNetWork, m_OPInfo.m_iStation, DEV_TYP_OPTICAL_WORD, devno, &nSize, buf, UPPER_EQP);

	//Return 값 확인해봐야함. 읽으면 어떤 값이 return 되는가 ? 
	if(ret == 0)
	{
		memcpy(pData, buf, sizeof(BYTE)*nSize);
	}
}

void cDeviceMelsec::RobotWordRecvBuf(short devno, short nSizebyByte, PSHORT pData)
{
	if(CCIsConnected() == FALSE)
		return;

	// WORD단위니까 Byte Size는 전부 짝수로 처리해야 함
	short nSize = nSizebyByte;
	if(nSize%2)	nSize++;

	short buf[128];
	short ret;
	memset(buf, NULL, sizeof(short)*128);
	ret = MelsecReceive(m_CCInfo.m_iNetWork, m_CCInfo.m_iStation, DEV_TYP_CCLINK_WORDR, devno, &nSize, buf, ROBOT);
	
	//Return 값 확인해봐야함. 읽으면 어떤 값이 return 되는가 ? 
	if(ret == 0)
	{
		memcpy(pData, buf, sizeof(BYTE)*nSize);
	}
}
//2018/01/26, JSJUNG, LD/UD ROBOT 티칭값/현재 위치값(워드영역/READ 부) : 2WORD
void cDeviceMelsec::RobotWordRecvBuf(short devno, short nSizebyByte, PLONG pData)
{
	if(CCIsConnected() == FALSE)
		return;

	// WORD단위니까 Byte Size는 전부 짝수로 처리해야 함
	short nSize = nSizebyByte;
	if(nSize%2)	nSize++;

	short buf[256];
	short ret;
	memset(buf, NULL, sizeof(short)*256);
	ret = MelsecReceive(m_CCInfo.m_iNetWork, m_CCInfo.m_iStation, DEV_TYP_CCLINK_WORDR, devno, &nSize, buf, ROBOT);
	
	//Return 값 확인해봐야함. 읽으면 어떤 값이 return 되는가 ? 
	if(ret == 0)
	{
		memcpy(pData, buf, sizeof(BYTE)*nSize);
	}
}


CString cDeviceMelsec::UpperWordSend(short devno, LPVOID data, short nSizebyByte, BOOL bString)
{
	if(OPIsConnected() == FALSE)
		return _T("");

	char sendData[256];
	
	short ret;
	//2018-03-12, 추가
	nSizebyByte = nSizebyByte * 2;

	memset(sendData,NULL, sizeof(char)*256);

	
	if(bString)
	{
		char szBuff[256];
		memset(szBuff,NULL, sizeof(char)*256);

		CEtc::ConvertMultibyte((wchar_t*)data, szBuff, 256);
		memcpy(sendData, szBuff, nSizebyByte);		
	}
	else
		memcpy(sendData, data, nSizebyByte);

	// WORD단위니까 Byte Size는 전부 짝수로 처리해야 함 
	if(nSizebyByte%2)	
		nSizebyByte++;

	ret = MelsecSend(m_OPInfo.m_iNetWork, m_OPInfo.m_iStation, DEV_TYP_OPTICAL_WORD, devno, &nSizebyByte,sendData, UPPER_EQP);
	
	if(ret == 0)
	{
		return m_strSndBuf;
	}

	CString strErrMsg;
	strErrMsg.Format(_T(" Error!!! "));

	return strErrMsg;
}

CString cDeviceMelsec::RobotWordSend(short devno, LPVOID data, short nSizebyByte, BOOL bString)
{

	if(CCIsConnected() == FALSE)
		return _T("");

	short sendData[256];
	short ret;
	memset(sendData, NULL, sizeof(short)*256);

	if(bString)
	{
		char szBuff[256];
		CEtc::ConvertMultibyte((wchar_t*)data, szBuff, 256);
		memcpy(sendData, szBuff, nSizebyByte);
	}
	else
		memcpy(sendData, data, nSizebyByte);

	// WORD단위니까 Byte Size는 전부 짝수로 처리해야 함 
	if(nSizebyByte%2)	
		nSizebyByte++;

	ret = MelsecSend(m_CCInfo.m_iNetWork, m_CCInfo.m_iStation, DEV_TYP_CCLINK_WORDW, devno, &nSizebyByte,sendData,ROBOT);

	if(ret == 0)
	{
		return m_strSndBuf;
	}

	CString strErrMsg;
	strErrMsg.Format(_T(" Error!!! "));

	return strErrMsg;
}

void cDeviceMelsec::OnTimer(UINT_PTR nIDEvent)
{
	CMelsecNet::OnTimer(nIDEvent);
}
