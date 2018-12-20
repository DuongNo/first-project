#include "StdAfx.h"
#include "DeviceScaraRobot.h"
#include "BankGuild\ProcDataBank.h"
#include "CheetahApp.h"

// ��� ����. 
// 1. Offset����. ( SetZOffset, SetXOffset, SetYOffset, SetTOffset )
// 2. End Position ����. ( SetEndPosition ) 
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

	// WORD�����ϱ� Byte Size�� ���� ¦���� ó���ؾ� ��
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

	// WORD�����ϱ� Byte Size�� ���� ¦���� ó���ؾ� �� 
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
	// Offset, Start Point, End Point, �̵� Ÿ�� ���� �Ϸ��� ����. 
	//Start bit�� On���ش�. 

	//Scara_BitOn(devno);
}

BOOL cDeviceScaraRobot::CheckFinish()
{
	//�̵� ����� �̵��� �Ϸ� �Ǿ����� üũ �Ѵ�. 
	//�̵��� �Ϸ� �Ǹ� 1.

	BOOL bRet = FALSE;
	
	//bRet = Scara_ReadBit(USER OUTPUT address:: Move Finish);
	
	if(bRet == TRUE)	//�̵� �Ϸ�Ǹ� Move start Bit�� Finish Bit�� Off ���·� �������´�. 
		MoveFinish();


	return bRet; 
}

void cDeviceScaraRobot::MoveFinish()
{
	//�̵� �Ϸ��� Move Start Bit Off
	//�̵� �Ϸ��� Move Finish Bit Off

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

	//RWw11 Global Point Index Setting. ( �̵��� Global Point�� Index. )
	strData.Format(_T("index wait1"));	//int�� Position Index

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

	//RWw11 Global Point Index Setting. ( �̵��� Global Point�� Index. )
	strData.Format(_T("index wait2"));	//int�� Position Index

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

	//RWw11 Global Point Index Setting. ( �̵��� Global Point�� Index. )
	strData.Format(_T("index Reverse"));	//int�� Position Index

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

	//RWw11 Global Point Index Setting. ( �̵��� Global Point�� Index. )
	strData.Format(_T("index Buffer"));	//int�� Position Index

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

	//RWw11 Global Point Index Setting. ( �̵��� Global Point�� Index. )
	strData.Format(_T("index Buffer"));	//int�� Position Index

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

	//RWw11 Global Point Index Setting. ( �̵��� Global Point�� Index. )
	strData.Format(_T("index Buffer"));	//int�� Position Index

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
	// scara�� ��Ŀ�� �ö���� ���� ����. 
	//Global float�Է�. //RWw0~w7	Offset �Է� ( X, Y, T Offset )

	//Scara_WriteWord(short devno, LPVOID data, short nSizebyByte, BOOL bString);
}

void	cDeviceScaraRobot::SetTOffset(int nOffset)
{
	//Tray�� Stage�� Theta������ offset���� �����Ѵ�. 
	//RWw0~w7	Offset �Է� ( X, Y, T Offset )

	//Scara_WriteWord(short devno, LPVOID data, short nSizebyByte, BOOL bString);
}

void cDeviceScaraRobot::SetYOffset(int nMode, int nOffset)
{
	// Tray�� Stage�� ���� ������ offset���� �����Ѵ�. 
	//RWw0~w7	Offset �Է� ( X, Y, T Offset )

	//Scara_WriteWord(short devno, LPVOID data, short nSizebyByte, BOOL bString);
}
void cDeviceScaraRobot::SetXOffset(int nMode, int nOffset)
{
	// Tray�� Stage�� ���� ������ offset���� �����Ѵ�.  
	//RWw0~w7	Offset �Է� ( X, Y, T Offset )

	//Scara_WriteWord(short devno, LPVOID data, short nSizebyByte, BOOL bString);
}

int	cDeviceScaraRobot::GetPosition()
{
	// ���� Scara ��Ŀ�� ��ġ�� �˷��ش� (Position Index)
	int nReturn = 0;
	//millimeter�� ��ġ���� ���� ƼĪ ��ġ�� �� ������ġ�� Index�� ��ȯ���ִ� ���. 


	//return 0�� �����ġ, false(-1)�� ����.
	return nReturn;
}

//BOOL cDeviceScaraRobot::MovePosition(int nIdx)
//{
//	// ���� ���������� �̵� (scara���ο� ������ �ε��� ���)
//
//	return FALSE;
//}
//BOOL cDeviceScaraRobot::StopWaitingPosition()
//{
//	// ��� ���������� �̵� ������ ���߰� ��ٸ���. 
//	return FALSE;
//}
//BOOL cDeviceScaraRobot::CheckTraytransfer(bool bLeft)
//{
//	// BufferTray�� Tray transfer�� scara������ ��ġ�� �ʴ��� Ȯ��.
//
//
//	// Ʈ����Ʈ�����۰� ������������ FALSE ���� Ʈ���̷� �̵��ϸ� �ȵ�. 
//	return FALSE; 
//}
//int	cDeviceScaraRobot::CheckStageBuffer()
//{
//	// 0�̳����� Full, 1,2,3�� ������ ��ġ�� ���簡��. 
//	int nReturn = 0;
//
//
//	//return false(-1)�� ����.
//	return nReturn;
//}