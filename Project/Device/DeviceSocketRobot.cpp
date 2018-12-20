#pragma once
#include "stdafx.h"
#include "SocketDialog.h"
#include "CheetahApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW

#endif

void CSocketDialog::SetRobotAlignDataProcessing(CDataSocket* pSocket)
{
	int nMsgLen = pSocket->GetLastLength();

	wchar_t wszMsg[4096];
	if(nMsgLen > 4095)
		nMsgLen = 4095;

	CEtc::ConvertUnicode(pSocket->GetLastMsg(), nMsgLen, wszMsg, 4096);

	wszMsg[nMsgLen] = NULL;
	CString strMsg;
	strMsg.Format(_T("%s"), wszMsg);

	theLog[LOG_NACHIROBOT].AddBuf(_T("%s -> %s\t%d\t%s"), pSocket->GetSocketName(), SOCKET_MAIN,nMsgLen,strMsg);

	int i = 0;
	while (strMsg.Find(STX, 0) != -1)
	{
		if( i > 0) 
			theLog[LOG_NACHIROBOT].AddBuf(_T("%dth Parsing %d\t%s"), i+1, strMsg.GetLength(), strMsg);
		strMsg = ParseRobotAlignMessage(pSocket, strMsg);
		i++;
	}
	if(strMsg.GetLength() > 0)
		theLog[LOG_NACHIROBOT].AddBuf(_T("Parsing After %d\t%s"), strMsg.GetLength(), strMsg);
}


CString CSocketDialog::ParseRobotAlignMessage(CDataSocket *pSocket, CString strMsg )
{
	theLog[LOG_NACHIROBOT].AddBuf(_T("%s -> %s\t: %s"), pSocket->GetSocketName(),SOCKET_MAIN, strMsg);

	CString strCommand = _T("");
	CString strTemp = strMsg;
	strTemp.TrimRight(CR);
	CTokenizer t(strTemp);

	// OK or NG
	strTemp = t.GetNextToken(_T(","));
	// Command
	strCommand = t.GetNextToken(_T(","));

	theLog[LOG_NACHIROBOT].AddBuf( _T("%s Packet Command Error %s | %s"), pSocket->GetSocketName(), strTemp, strMsg);

	return PacketClear(pSocket->GetSocketName(), strMsg);
}

BOOL CSocketDialog::SendMsgToRobotAlign(double dPosX , double dPosY, double dPosT)
{
	CString strMsg;
	strMsg.Format(_T("%09.5f,%09.5f,000.00000,000.00000,000.00000,%09.5f%c"),dPosX, dPosY, dPosT, CR);

	CString strMsg2;
	strMsg2.Format(_T("X[%09.5f],Y[%09.5f],T[%09.5f]"),dPosX, dPosY, dPosT);

	char cMsg[256];
	CEtc::ConvertMultibyte(	(wchar_t*)strMsg.GetString(), cMsg, 256);

	if (m_pSocket_RobotAlign.SendClient(SOCKET_ROBOTALIGN, cMsg, strMsg.GetLength()))
	{
		theLog[LOG_NACHIROBOT].AddBuf( _T("%s -> %s\t%s"), SOCKET_MAIN, SOCKET_ROBOTALIGN, strMsg);
		theLog[LOG_LDRB_TEACH_MOVE].AddBuf( _T("LD GET ALIGN %s"),strMsg2);
		return TRUE;
	}
	else
	{
		theLog[LOG_NACHIROBOT].AddBuf( _T("%s -> %s\tSEND FAIL : %s"), SOCKET_MAIN, SOCKET_ROBOTALIGN, strMsg);
		return FALSE;
	}
}


void CSocketDialog::SetRobotDataProcessing(CDataSocket* pSocket)
{
	
}


CString CSocketDialog::ParseRobotMessage()
{
	return _T("");
}


BOOL CSocketDialog::SendMsgToRobot(E_ROBOT Robot)
{
	ROBOT_DATA Data;
	
	Data.nData[0] = (DWORD)(theRecipeBank.m_Tray.m_dPitchX * 10000);
	Data.nData[1] = (DWORD)(theRecipeBank.m_Tray.m_dPitchY * 10000);

	for (int i = 0; i < MAX_ROBOT_DATA; i++)
		Data.nData[i] = SwapDWORD(Data.nData[i]);
		
	int nLen = sizeof(ROBOT_DATA);
	CString SocketName;
	if(Robot == E_LD_ROBOT)
		SocketName = SOCKET_LD_ROBOT;
	else
		SocketName = SOCKET_UD_ROBOT;
		
	if (m_pSocket_Robot[Robot].SendClient(SocketName, &Data, nLen))
	{
		if (Robot == E_LD_ROBOT)
			theLog[LOG_NACHIROBOT].AddBuf(_T("%s -> %s\t Robot Data Send"),SOCKET_MAIN,SOCKET_LD_ROBOT);
		else
			theLog[LOG_NACHIROBOT].AddBuf(_T("%s -> %s\t Robot Data Send : X = %f , Y = %f"),SOCKET_MAIN,SOCKET_UD_ROBOT,theRecipeBank.m_Tray.m_dPitchX, theRecipeBank.m_Tray.m_dPitchY);

		return TRUE;
	}
	else
	{
		if (Robot == E_LD_ROBOT)
			theLog[LOG_NACHIROBOT].AddBuf(_T("%s -> %s\t Robot Data Send Fail"),SOCKET_MAIN,SOCKET_LD_ROBOT);
		else
			theLog[LOG_NACHIROBOT].AddBuf(_T("%s -> %s\t Robot Data Send Fail"),SOCKET_MAIN,SOCKET_UD_ROBOT);

		return FALSE;
	}
	return TRUE;
}



DWORD CSocketDialog::SwapDWORD(DWORD dwValue)
{
	// 원본 데이터를 Byte 배열에 복사한다.
	BYTE src[4] ={ 0, };

	memcpy(src, (PBYTE)&dwValue, sizeof(DWORD));
	// 버퍼에 순서를 뒤집어서 저장
	BYTE buf[4] ={ 0, };

	buf[0] = src[3];
	buf[1] = src[2];
	buf[2] = src[1];
	buf[3] = src[0];
	// 복사본을 리턴 한다.
	DWORD dwRet;

	memcpy((PBYTE)&dwRet, buf, sizeof(DWORD));

	return dwRet;
}
