#pragma once
#include "stdafx.h"
#include "SocketDialog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CSocketDialog::SetTrayReaderDataProcessing(CDataSocket* pSocket)
{
	int nMsgLen = pSocket->GetLastLength();

	wchar_t wszMsg[4096];
	if(nMsgLen > 4095)
		nMsgLen = 4095;
	CEtc::ConvertUnicode(pSocket->GetLastMsg(), nMsgLen, wszMsg, 4096);
	wszMsg[nMsgLen] = NULL;
	CString strMsg;
	strMsg.Format(_T("%s"), wszMsg);

	theLog[LOG_TRAY_READER].AddBuf( _T("%s -> %s\t%s"), pSocket->GetSocketName(), SOCKET_MAIN, strMsg);
	
	//2018-03-10,MGYUN, DCR FAIL or DCR ID MELSEC WRITE
	theMonitorBank.WriteDCRID(strMsg);
	
	if(nMsgLen > 0)
	{
		ParseTrayReaderMessage(strMsg);
	}
}

BOOL CSocketDialog::ParseTrayReaderMessage(CString strMsg)
{
	CString str1;
	str1 = strMsg.SpanExcluding(_T("\n\r"));

	theProcBank.m_strTrayID = str1.Mid(0,theConfigBank.m_Option.m_nTrayIDCount);
	//theProcBank.m_strTrayID = strMsg.SpanExcluding(_T("\n\r"));
	
	return TRUE;
}
