#include "StdAfx.h"
#include "DeviceScaraRobot.h"
#include "BankGuild\ProcDataBank.h"
#include "CheetahApp.h"

// 사용 순서. 
// 1. Offset설정. ( SetZOffset, SetXOffset, SetYOffset, SetTOffset )
// 2. End Position 설정. ( SetEndPosition ) 
// 3. Move Start Bit On. ( MoveStart )
// 4. Move Finish Check. ( CheckFinish )
// 5. Move Finish Bit & Move Start Bit Off. ( MoveFinish ) 

cDeviceScaraRobot::cDeviceScaraRobot(void)
{
	m_PLCInfo.m_iNetChanel = INT_MNET_CHANNEL;
	m_PLCInfo.m_iNetMode = INT_MNET_MODE;
	m_PLCInfo.m_iNetWork = INT_MNET_NETWORK;
	m_PLCInfo.m_iStation = INT_MNET_STATION;
	m_PLCInfo.m_lNetPath = INT_MNET_PATH;

	m_strSndBuf = _T("");
	m_strRcvBuf = _T("");

	m_bIsConnected = FALSE;
}

cDeviceScaraRobot::~cDeviceScaraRobot(void)
{

}

BOOL cDeviceScaraRobot::Open()
{
	SetConnected(FALSE);
	
	if(MelsecOpen(m_PLCInfo.m_iNetChanel, m_PLCInfo.m_iNetMode, &m_PLCInfo.m_lNetPath) == 0)
	{
		SetConnected(TRUE);
		return TRUE;
	}
	else
	{
		theProcBank.AlarmHappen(ALM_SCARA_OPEN);
		return FALSE;
	}

	return FALSE; 
}

void cDeviceScaraRobot::Close()
{
	SetConnected(FALSE);
	MelsecClose( m_PLCInfo.m_lNetPath);
}

void cDeviceScaraRobot::Scara_BitOn(short devno)
{
	MelsecDevSet(m_PLCInfo.m_iNetWork, m_PLCInfo.m_iStation, DEV_TYP_BIT, devno);
}

void cDeviceScaraRobot::Scara_BitOff(short devno)
{
	MelsecDevRst(m_PLCInfo.m_iNetWork, m_PLCInfo.m_iStation, DEV_TYP_BIT, devno);
}

short cDeviceScaraRobot::Scara_ReadBit(short devno)
{
	BOOL bFlag;
	MelsecDevReceive(m_PLCInfo.m_iNetWork, m_PLCInfo.m_iStation, DEV_TYP_BIT, devno, &bFlag);

	return bFlag;
}

void cDeviceScaraRobot::Scara_ReadWord(short devno, short nSizebyByte, PSHORT pData)
{
	if(IsConnected() == FALSE)
		return;

	// WORD단위니까 Byte Size는 전부 짝수로 처리해야 함
	short nSize = nSizebyByte;
	if(nSize%2)	nSize++;

	short buf[128];
	memset(buf, NULL, sizeof(short)*128);

	short ret = MelsecReceive(m_PLCInfo.m_iNetWork, m_PLCInfo.m_iStation, DEV_TYP_WORD, devno, &nSize, buf);
	if(ret == 0)
	{
		memcpy(pData, buf, sizeof(short)*nSize);
	}
}

CString cDeviceScaraRobot::Scara_WriteWord(short devno, LPVOID data, short nSizebyByte, BOOL bString)
{
	if(IsConnected() == FALSE)
		return _T("");

	short sendData[256];
	memset(sendData, NULL, 256);

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

	short ret = MelsecSend(m_PLCInfo.m_iNetWork, m_PLCInfo.m_iStation, DEV_TYP_WORD, devno, &nSizebyByte,sendData);

	if(ret == 0)
	{
		return m_strSndBuf;
	}

	CString strErrMsg;
	strErrMsg.Format(_T(" Error!!! "));

	return strErrMsg;
}

void cDeviceScaraRobot::MoveStart()
{
	// Offset, Start Point, End Point, 이동 타입 세팅 완료후 동작. 
	//Start bit를 On해준다. 

	//Scara_BitOn(devno);
}

BOOL cDeviceScaraRobot::CheckFinish()
{
	//이동 명령후 이동이 완료 되었는지 체크 한다. 
	//이동이 완료 되면 1.

	BOOL bRet = FALSE;
	
	//bRet = Scara_ReadBit(USER OUTPUT address:: Move Finish);
	
	if(bRet == TRUE)	//이동 완료되면 Move start Bit와 Finish Bit를 Off 상태로 돌려놓는다. 
		MoveFinish();


	return bRet; 
}

void cDeviceScaraRobot::MoveFinish()
{
	//이동 완료후 Move Start Bit Off
	//이동 완료후 Move Finish Bit Off

	//Scara_BitOff(Start Bit address);
	//Scara_BitOff(Finish Bit address);
}

void cDeviceScaraRobot::SetEndPosition(SCARA_G_POINT ePoint)
{
	int nStartPos = GetPosition();
	m_nMoveType = TYPE_SPOS_EPOS;

	switch(ePoint)
	{
	case SCARA_WAIT_POS1:
		m_nMoveType = MoveToWaitOnePoint(nStartPos, SCARA_WAIT_POS1);
		break;

	case SCARA_WAIT_POS2:
		m_nMoveType = MoveToWaitTwoPoint(nStartPos, SCARA_WAIT_POS1);
		break;

	case SCARA_CELL_REVERSE:
		m_nMoveType = MoveToCellReverse(nStartPos, SCARA_WAIT_POS1);
		break;

	case SCARA_BUFFER_STAGE:
		m_nMoveType = MoveToBufferStage(nStartPos, SCARA_WAIT_POS1);
		break;

	case SCARA_TRAY1_POS:
		m_nMoveType = MoveToTrayOne(nStartPos, SCARA_WAIT_POS1);
		break;

	case SCARA_TRAY2_POS:
		m_nMoveType = MoveToTrayTwo(nStartPos, SCARA_WAIT_POS1);
		break;
	}

}

int cDeviceScaraRobot::MoveToWaitOnePoint(int nStartPoint, int nEndPoint)
{
	CString strData;
	int nMoveType = TYPE_SPOS_EPOS;

	//RWw11 Global Point Index Setting. ( 이동할 Global Point의 Index. )
	strData.Format(_T("index wait1"));	//int형 Position Index

	if(nStartPoint==SCARA_WAIT_POS2)			//wait2			==> wait1
		nMoveType = TYPE_SPOS_EPOS;
	else if(nStartPoint==SCARA_CELL_REVERSE)	//Cell Reverse	==> wait1
		nMoveType = TYPE_SPOS_EPOS;
	else if(nStartPoint==SCARA_BUFFER_STAGE)	//Buffer stage	==> wait1
		nMoveType = TYPE_SPOS_EPOS;
	else if(nStartPoint==SCARA_TRAY1_POS)		//Tray1			==> wait1
		nMoveType = TYPE_SPOS_EPOS;
	else if(nStartPoint==SCARA_TRAY2_POS)		//tray2	==> wait2 ==> wait1
		nMoveType = TYPE_W2_EPOS;

	//Scara_WriteWord(devno, (LPVOID)strData.GetString(), (short)strData.GetLength(), TRUE);

	return nMoveType;
}
int cDeviceScaraRobot::MoveToWaitTwoPoint(int nStartPoint, int nEndPoint)
{
	CString strData;
	int nMoveType = TYPE_SPOS_EPOS;

	//RWw11 Global Point Index Setting. ( 이동할 Global Point의 Index. )
	strData.Format(_T("index wait2"));	//int형 Position Index

	if(nStartPoint==SCARA_WAIT_POS1)			//wait1			==> wait2
		nMoveType = TYPE_SPOS_EPOS;
	else if(nStartPoint==SCARA_CELL_REVERSE)	//Reverse		==> wait2
		nMoveType = TYPE_SPOS_EPOS;
	else if(nStartPoint==SCARA_BUFFER_STAGE)	//Buffer ==> wait1 ==> wait2
		nMoveType = TYPE_W1_EPOS;
	else if(nStartPoint==SCARA_TRAY1_POS)		//Tray1			==> wait2
		nMoveType = TYPE_SPOS_EPOS;
	else if(nStartPoint==SCARA_TRAY2_POS)		//Tray2			==> wait2
		nMoveType = TYPE_SPOS_EPOS;

	//Scara_WriteWord(devno, (LPVOID)strData.GetString(), (short)strData.GetLength(), TRUE);

	return nMoveType;
}
int cDeviceScaraRobot::MoveToCellReverse(int nStartPoint, int nEndPoint)
{
	CString strData;
	int nMoveType = TYPE_SPOS_EPOS;

	//RWw11 Global Point Index Setting. ( 이동할 Global Point의 Index. )
	strData.Format(_T("index Reverse"));	//int형 Position Index

	if(nStartPoint==SCARA_WAIT_POS1)			//wait1		==> Reverse
		nMoveType = TYPE_SPOS_EPOS;
	else if(nStartPoint==SCARA_WAIT_POS2)		//wait2		==> Reverse
		nMoveType = TYPE_SPOS_EPOS;
	else if(nStartPoint==SCARA_BUFFER_STAGE)	//Buffer	==> Reverse
		nMoveType = TYPE_SPOS_EPOS;
	else if(nStartPoint==SCARA_TRAY1_POS)	//Tray1		==> Reverse
		nMoveType = TYPE_SPOS_EPOS;
	else if(nStartPoint==SCARA_TRAY2_POS)		//Tray2 ==> wait2 ==> Reverse
		nMoveType = TYPE_W2_EPOS;

	//Scara_WriteWord(devno, (LPVOID)strData.GetString(), (short)strData.GetLength(), TRUE);

	return nMoveType;
}
int cDeviceScaraRobot::MoveToBufferStage(int nStartPoint, int nEndPoint)
{
	CString strData;
	int nMoveType = TYPE_SPOS_EPOS;

	//RWw11 Global Point Index Setting. ( 이동할 Global Point의 Index. )
	strData.Format(_T("index Buffer"));	//int형 Position Index

	if(nStartPoint==SCARA_WAIT_POS1)			//wait1		==> Buffer
		nMoveType = TYPE_SPOS_EPOS;
	else if(nStartPoint==SCARA_WAIT_POS2)		//wait2 ==> wait1 ==> Buffer
		nMoveType = TYPE_W1_EPOS;
	else if(nStartPoint==SCARA_CELL_REVERSE)	//Reverse	==> Buffer
		nMoveType = TYPE_SPOS_EPOS;
	else if(nStartPoint==SCARA_TRAY1_POS)		//Tray1	==> wait1 ==> Buffer
		nMoveType = TYPE_W1_EPOS;
	else if(nStartPoint==SCARA_TRAY2_POS)		//Tray2	==> wait2 ==> wait1 ==> Buffer
		nMoveType = TYPE_SPOS_W1_W2_EPOS;

	//Scara_WriteWord(devno, (LPVOID)strData.GetString(), (short)strData.GetLength(), TRUE);

	return nMoveType;
}

int cDeviceScaraRobot::MoveToTrayOne(int nStartPoint, int nEndPoint)
{
	CString strData;
	int nMoveType = TYPE_SPOS_EPOS;

	//RWw11 Global Point Index Setting. ( 이동할 Global Point의 Index. )
	strData.Format(_T("index Buffer"));	//int형 Position Index

	if(nStartPoint==SCARA_WAIT_POS1)			//wait1 ==> Tray1
		nMoveType = TYPE_SPOS_EPOS;
	else if(nStartPoint==SCARA_WAIT_POS2)		//wait2 ==> Tray1
		nMoveType = TYPE_SPOS_EPOS;
	else if(nStartPoint==SCARA_CELL_REVERSE)	//Reverse ==> Tray1
		nMoveType = TYPE_SPOS_EPOS;
	else if(nStartPoint==SCARA_BUFFER_STAGE)	//Buffer ==> wait1 ==> Tray1
		nMoveType = TYPE_W1_EPOS;
	else if(nStartPoint==SCARA_TRAY2_POS)		//Tray2	==> wait2 ==> Tray1
		nMoveType = TYPE_W2_EPOS;

	//Scara_WriteWord(devno, (LPVOID)strData.GetString(), (short)strData.GetLength(), TRUE);

	return nMoveType;
}

int cDeviceScaraRobot::MoveToTrayTwo(int nStartPoint, int nEndPoint)
{
	CString strData;
	int nMoveType = TYPE_SPOS_EPOS;

	//RWw11 Global Point Index Setting. ( 이동할 Global Point의 Index. )
	strData.Format(_T("index Buffer"));	//int형 Position Index

	if(nStartPoint==SCARA_WAIT_POS1)			//wait1 ==> wait2 ==> Tray2
		nMoveType = TYPE_W2_EPOS;
	else if(nStartPoint==SCARA_WAIT_POS2)		//wait2 ==> Tray2
		nMoveType = TYPE_SPOS_EPOS;
	else if(nStartPoint==SCARA_CELL_REVERSE)	//Reverse ==> wait2 ==> Tray2
		nMoveType = TYPE_W2_EPOS;
	else if(nStartPoint==SCARA_BUFFER_STAGE)	//Buffer ==> wait1 ==> wait2 ==>Tray2
		nMoveType = TYPE_SPOS_W1_W2_EPOS;
	else if(nStartPoint==SCARA_TRAY1_POS)		//Tray1	==> wait2 ==> Tray2
		nMoveType = TYPE_W2_EPOS;

	//Scara_WriteWord(devno, (LPVOID)strData.GetString(), (short)strData.GetLength(), TRUE);

	return nMoveType;

}

void cDeviceScaraRobot::SetZOffset(int nOffset)
{
	// scara의 픽커가 올라오는 높이 설정. 
	//Global float입력. //RWw0~w7	Offset 입력 ( X, Y, T Offset )

	//Scara_WriteWord(short devno, LPVOID data, short nSizebyByte, BOOL bString);
}

void	cDeviceScaraRobot::SetTOffset(int nOffset)
{
	//Tray나 Stage의 Theta방향의 offset값을 세팅한다. 
	//RWw0~w7	Offset 입력 ( X, Y, T Offset )

	//Scara_WriteWord(short devno, LPVOID data, short nSizebyByte, BOOL bString);
}

void cDeviceScaraRobot::SetYOffset(int nMode, int nOffset)
{
	// Tray나 Stage의 세로 방향의 offset값을 세팅한다. 
	//RWw0~w7	Offset 입력 ( X, Y, T Offset )

	//Scara_WriteWord(short devno, LPVOID data, short nSizebyByte, BOOL bString);
}
void cDeviceScaraRobot::SetXOffset(int nMode, int nOffset)
{
	// Tray나 Stage의 가로 방향의 offset값을 세팅한다.  
	//RWw0~w7	Offset 입력 ( X, Y, T Offset )

	//Scara_WriteWord(short devno, LPVOID data, short nSizebyByte, BOOL bString);
}

int	cDeviceScaraRobot::GetPosition()
{
	// 현재 Scara 픽커의 위치를 알려준다 (Position Index)
	int nReturn = 0;
	//millimeter로 위치값을 얻어와 티칭 위치와 비교 현재위치의 Index를 반환해주는 기능. 


	//return 0는 대기위치, false(-1)은 오류.
	return nReturn;
}

//BOOL cDeviceScaraRobot::MovePosition(int nIdx)
//{
//	// 지정 포지션으로 이동 (scara내부에 지정된 인덱스 사용)
//
//	return FALSE;
//}
//BOOL cDeviceScaraRobot::StopWaitingPosition()
//{
//	// 대기 포지션으로 이동 동작을 멈추고 기다린다. 
//	return FALSE;
//}
//BOOL cDeviceScaraRobot::CheckTraytransfer(bool bLeft)
//{
//	// BufferTray의 Tray transfer가 scara동선에 겹치지 않는지 확인.
//
//
//	// 트레이트랜스퍼가 내려와있으면 FALSE 버퍼 트레이로 이동하면 안됨. 
//	return FALSE; 
//}
//int	cDeviceScaraRobot::CheckStageBuffer()
//{
//	// 0이나오면 Full, 1,2,3이 나오면 위치에 적재가능. 
//	int nReturn = 0;
//
//
//	//return false(-1)은 오류.
//	return nReturn;
//}