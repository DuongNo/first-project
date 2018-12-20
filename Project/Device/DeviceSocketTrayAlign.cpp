#pragma once
#include "stdafx.h"
#include "SocketDialog.h"
#include "CheetahApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW

#endif
void CSocketDialog::SetTrayAlignDataProcessing(CDataSocket* pSocket)
{
	int nMsgLen = pSocket->GetLastLength();

	wchar_t wszMsg[4096];
	if(nMsgLen > 4095)
		nMsgLen = 4095;

	CEtc::ConvertUnicode(pSocket->GetLastMsg(), nMsgLen, wszMsg, 4096);

	wszMsg[nMsgLen] = NULL;
	CString strMsg;
	strMsg.Format(_T("%s"), wszMsg);

	theLog[LOG_TRAY_ALIGN].AddBuf(_T("%s -> %s\t%d\t%s"), pSocket->GetSocketName(), SOCKET_MAIN,nMsgLen,strMsg);

	int i = 0;
	while (strMsg.Find(STX, 0) != -1)
	{
		if( i > 0) 
			theLog[LOG_TRAY_ALIGN].AddBuf(_T("%dth Parsing %d\t%s"), i+1, strMsg.GetLength(), strMsg);
		strMsg = ParseTrayAlignMessage(strMsg);
		i++;
	}
	if(strMsg.GetLength() > 0)
		theLog[LOG_TRAY_ALIGN].AddBuf(_T("Parsing After %d\t%s"), strMsg.GetLength(), strMsg);
}


CString CSocketDialog::ParseTrayAlignMessage( CString strMsg )
{
	CString strCommand = _T("");
	CString strParam1 = _T("");
	CString strParam2 = _T("");

	BOOL bNack = FALSE;

	int nIndex = strMsg.Find(STX);
	if(nIndex > 0)
	{
		theLog[LOG_TRAY_ALIGN].AddBuf(_T("Before STX Packet[%d] : %s"), nIndex, strMsg.Mid(0, nIndex));
		strMsg.Delete(0, nIndex);
	}

	nIndex = strMsg.Find(ETX);
	if(nIndex < 0)
	{
		theLog[LOG_TRAY_ALIGN].AddBuf(_T("ETX Not Found[%d] : %s"), strMsg);
		// 두개 붙어서 나올 것을 대비해 다음 STX까지 지운다
		return PacketClear(SOCKET_TRAY_ALIGN, strMsg);
	}

	CString strTemp = strMsg.Mid(0, nIndex+1);

	// STX, ETX 제거
	strTemp.TrimLeft(STX);
	strTemp.TrimRight(ETX);
	CTokenizer t(strTemp);

	// Command
	strCommand = t.GetNextToken(_T("\\"));
	// Param1
	strParam1 = t.GetNextToken(_T("\\"));
	// Param2
	strParam2 = t.GetNextToken(_T("\\"));

	// 운용PC랑 접속 되어 있는 상태를 보내줌
	if(TRAY_CURRENT_STATE == strCommand)
	{
		// strParam1 == Recipe Name
		// strParam2 == OK or NG
		theProcBank.m_strTrayAlignRecipeName = strParam1;

		if(strParam2 == _T("OK"))
		{
			// OK
			theLog[LOG_TRAY_ALIGN].AddBuf(_T("%s [%s][%s]"), strCommand, strParam1, strParam2);
		}
		else
		{
			// NG
			theLog[LOG_TRAY_ALIGN].AddBuf(_T("%s [%s][%s]"), strCommand, strParam1, strParam2);
		}

	}
	// edge search
	else if(TRAY_ALIGN_RES == strCommand)
	{
		// strParam1 == 포트
		// strParam2 == OK,NG 결과
		if (strParam1 == _T("OKTRAY"))
		{
			theProcBank.m_strOKTray_AlignOK = strParam2;
			theLog[LOG_TRAY_ALIGN].AddBuf(_T("%s %s [%s]"), strCommand, strParam1, strParam2);
		}
		else
		{
			theProcBank.m_strNGTray_AlignOK = strParam2;
			theLog[LOG_TRAY_ALIGN].AddBuf(_T("%s %s [%s]"), strCommand, strParam1, strParam2);
		}
	}
	// Recipe
	else if(TRAY_CHG_RECIPE_RES == strCommand)
	{
		if(strParam1 == _T("OK"))	// OK
		{
			theLog[LOG_TRAY_ALIGN].AddBuf(_T("%s [%s]"), strCommand, strParam1);
		}
		else						// NG
		{
			theLog[LOG_TRAY_ALIGN].AddBuf(_T("%s [%s][%s]"), strCommand, strParam1, strParam2);
			theProcBank.m_strTrayAlignRecipeName = strParam2;
		}
	}
	else if(TRAY_ALIGN_TEACHING_RES == strCommand)
	{
		// strParam1 == 포트
		// strParam2 == OK,NG;
		if (strParam1 == _T("OKTRAY"))
		{
			theProcBank.m_strOKTray_TeachingOK = strParam2;
			theLog[LOG_MITEDGEALIGN].AddBuf(_T("%s %s [%s]"), strCommand, strParam1, strParam2);
		}
		else
		{
			theProcBank.m_strNGTray_TeachingOK = strParam2;
			theLog[LOG_MITEDGEALIGN].AddBuf(_T("%s %s [%s]"), strCommand, strParam1, strParam2);
		}
	}

	return PacketClear(SOCKET_MITEDGEALIGN, strMsg);
}

BOOL CSocketDialog::SendMsgToTrayAlign( CString strCommand, CString strParam1 )
{
	CString strMsg;
	CClientSocket* pSocket = m_pSocket_Tray_Align.GetClientAt(SOCKET_TRAY_ALIGN);

	if ( pSocket == NULL )		// 연결 실패
	{
		theLog[LOG_MITEDGEALIGN].AddBuf(_T("운영 -> %s\tSocket NULL [%s]"), SOCKET_TRAY_ALIGN, strMsg);
		return FALSE;
	}

	strMsg.Format(_T("%c%s\\%s%c"), STX, strCommand, strParam1, ETX);

	char szBuff[256];
	CEtc::ConvertMultibyte((wchar_t*)strMsg.GetString(), szBuff, 256);

	if(m_pSocket_Tray_Align.SendClient(SOCKET_TRAY_ALIGN, szBuff, strlen(szBuff)))
	{
		theLog[LOG_TRAY_ALIGN].AddBuf(_T("운영 -> %s\t%s"), SOCKET_TRAY_ALIGN, strMsg);
		return TRUE;
	}
	else
	{
		theLog[LOG_TRAY_ALIGN].AddBuf(_T("운영 -> %s\t전송 실패[%s]"), SOCKET_TRAY_ALIGN, strMsg);
		return FALSE;
	}

}
