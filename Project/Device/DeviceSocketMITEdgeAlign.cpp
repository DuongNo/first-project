#pragma once
#include "stdafx.h"
#include "SocketDialog.h"
#include "CheetahApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW

#endif
void CSocketDialog::SetMITEdgeAlignDataProcessing(CDataSocket* pSocket)
{
	int nMsgLen = pSocket->GetLastLength();

	wchar_t wszMsg[4096];
	if(nMsgLen > 4095)
		nMsgLen = 4095;

	CEtc::ConvertUnicode(pSocket->GetLastMsg(), nMsgLen, wszMsg, 4096);

	wszMsg[nMsgLen] = NULL;
	CString strMsg;
	strMsg.Format(_T("%s"), wszMsg);

	theLog[LOG_MITEDGEALIGN].AddBuf(_T("%s -> %s\t%d\t%s"), pSocket->GetSocketName(), SOCKET_MAIN,nMsgLen,strMsg);

	int i = 0;
	while (strMsg.Find(STX, 0) != -1)
	{
		if( i > 0) 
			theLog[LOG_MITEDGEALIGN].AddBuf(_T("%dth Parsing %d\t%s"), i+1, strMsg.GetLength(), strMsg);
		strMsg = ParseMITEdgeAlignMessage(strMsg);
		i++;
	}
	if(strMsg.GetLength() > 0)
		theLog[LOG_MITEDGEALIGN].AddBuf(_T("Parsing After %d\t%s"), strMsg.GetLength(), strMsg);
}


CString CSocketDialog::ParseMITEdgeAlignMessage( CString strMsg )
{
	CString strCommand = _T("");
	CString strParam1 = _T("");
	CString strParam2 = _T("");
	CString strParam2Y = _T("");		// ALIGN_RES Y축 저장할 변수
	CString strParam2T = _T("");		// ALIGN_RES Theta 저장할 변수
	CString strBuf;
	BOOL bNack = FALSE;

	int nIndex = strMsg.Find(STX);
	if(nIndex > 0)
	{
		theLog[LOG_MITEDGEALIGN].AddBuf(_T("Before STX Packet[%d] : %s"), nIndex, strMsg.Mid(0, nIndex));
		strMsg.Delete(0, nIndex);
	}

	// 구분자 : \\
	// [STX]Command\Parameter1\Parameter2[ETX]

	nIndex = strMsg.Find(ETX);
	if(nIndex < 0)
	{
		theLog[LOG_MITEDGEALIGN].AddBuf(_T("ETX Not Found[%d] : %s"), strMsg);
		// 두개 붙어서 나올 것을 대비해 다음 STX까지 지운다
		return PacketClear(SOCKET_MITEDGEALIGN, strMsg);
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
	// Param2 Y축
	strParam2Y = t.GetNextToken(_T("\\"));
	// Param2 Theta축
	strParam2T = t.GetNextToken(_T("\\"));


	// 운용PC랑 접속 되어 있는 상태를 보내줌
	if(LA_CURRENT_STATE == strCommand)
	{
		// strParam1 == Recipe Name
		// strParam2 == OK or NG

		theProcBank.m_strLARecipeName = strParam1;

		if(strParam2 == _T("OK"))
		{
			// OK
			theLog[LOG_MITEDGEALIGN].AddBuf(_T("%s [%s][%s]"), strCommand, strParam1, strParam2);
		}
		else
		{
			// NG
			theLog[LOG_MITEDGEALIGN].AddBuf(_T("%s [%s][%s]"), strCommand, strParam1, strParam2);
		}

	}
	// edge search
	else if(LA_ALIGN_RES == strCommand)
	{
		// strParam1 == OK or NG
		// strParam2 == OK : X, Y, Theta Value
		if(strParam1 == _T("OK"))	// OK
		{
			// 위치값 저장
			theProcBank.m_strLA_AlignOK = strParam1;
			theProcBank.m_strLAResX = strParam2;
			theProcBank.m_strLAResY = strParam2Y;
			theProcBank.m_strLAResT = strParam2T;

			theLog[LOG_MITEDGEALIGN].AddBuf(_T("%s [%s][%s][%s][%s]"), strCommand, strParam1, strParam2, strParam2Y, strParam2T);
			
			//2018-03-08,MGYUN,EDGE ALIGN MELSEC 기록
			theMonitorBank.WriteEdgeAlign(strParam1, strParam2, strParam2Y, strParam2T);
			
			CCellInfo* pCellInfo;
			CCellTag tagCell;
			pCellInfo = theCellBank.GetCellInfo(CELL_POS_LD_CONV);
			tagCell = theCellBank.GetCellTag(CELL_POS_LD_CONV);

			if(tagCell.IsExist())
			{
				pCellInfo->LoaderAlignDataX = _tstof(theProcBank.m_strLAResX);
				pCellInfo->LoaderAlignDataY = _tstof(theProcBank.m_strLAResY);
				pCellInfo->LoaderAlignDataT = _tstof(theProcBank.m_strLAResT);
				pCellInfo->LoaderAlignResult = ALIGN_RESULT_GOOD;
			}
		}
		else						// NG
		{		
			theProcBank.m_strLA_AlignOK = strParam1;
			theLog[LOG_MITEDGEALIGN].AddBuf(_T("%s [%s]"), strCommand, strParam1);

			CCellInfo* pCellInfo;
			CCellTag tagCell;
			pCellInfo = theCellBank.GetCellInfo(CELL_POS_LD_CONV);
			tagCell = theCellBank.GetCellTag(CELL_POS_LD_CONV);

			if(tagCell.IsExist())
			{
				pCellInfo->LoaderAlignResult = ALIGN_RESULT_FAIL;
			}

		}
	}
	// Recipe
	else if(LA_CHG_RECIPE_RES == strCommand)
	{
		// strParam1 == OK or NG
		// strParam2 == NG : Recipe Name

		if(strParam1 == _T("OK"))	// OK
		{
			theLog[LOG_MITEDGEALIGN].AddBuf(_T("%s [%s]"), strCommand, strParam1);
		}
		else						// NG
		{
			theLog[LOG_MITEDGEALIGN].AddBuf(_T("%s [%s][%s]"), strCommand, strParam1, strParam2);
			theProcBank.m_strLARecipeName = strParam2;
		}
	}
	// Align Teaching
	else if(LA_ALIGN_TEACHING_RES == strCommand)
	{
		// strParam1 == OK or NG

		if(strParam1 == _T("OK"))	// OK
		{
			// 위치값 저장
			theProcBank.m_strLA_TeachingOK = strParam1;

			theLog[LOG_MITEDGEALIGN].AddBuf(_T("%s [%s]"), strCommand, strParam1);
		}
		else						// NG
		{		
			theProcBank.m_strLA_TeachingOK = strParam1;
			theLog[LOG_MITEDGEALIGN].AddBuf(_T("%s [%s]"), strCommand, strParam1);
		}
	}

	return PacketClear(SOCKET_MITEDGEALIGN, strMsg);
}

BOOL CSocketDialog::SendMsgToMITEdgeAlign( CString strCommand, CString strParam1, CString strParam2 )
{
	CString strMsg;
	CClientSocket* pSocket = m_pSocket_MITEdgeAlign.GetClientAt(SOCKET_MITEDGEALIGN);

	if ( pSocket == NULL )		// 연결 실패
	{
		theLog[LOG_MITEDGEALIGN].AddBuf(_T("운영 -> %s\tSocket NULL [%s]"), SOCKET_MITEDGEALIGN, strMsg);
		return FALSE;
	}

	if(strParam1.GetLength() < 1)			// 파라미터1 없으면 커맨드만 Send
		strMsg.Format(_T("%c%s%c"), STX, strCommand, ETX);
	else if(strParam2.GetLength() < 1)		// 파라미터2 업으면 커맨드, 파라미터1 Send
		strMsg.Format(_T("%c%s\\%s%c"), STX, strCommand, strParam1, ETX);
	else									// 파라미터1,2 다 있으면 커맨드, 파라미터1, 파라미터2 Send
		strMsg.Format(_T("%c%s\\%s\\%s%c"), STX, strCommand, strParam1, strParam2, ETX);

	char szBuff[256];
	CEtc::ConvertMultibyte((wchar_t*)strMsg.GetString(), szBuff, 256);

	if(m_pSocket_MITEdgeAlign.SendClient(SOCKET_MITEDGEALIGN, szBuff, strlen(szBuff)))
	{
		theLog[LOG_MITEDGEALIGN].AddBuf(_T("운영 -> %s\t%s"), SOCKET_MITEDGEALIGN, strMsg);
		return TRUE;
	}
	else
	{
		theLog[LOG_MITEDGEALIGN].AddBuf(_T("운영 -> %s\t전송 실패[%s]"), SOCKET_MITEDGEALIGN, strMsg);
		return FALSE;
	}

}
