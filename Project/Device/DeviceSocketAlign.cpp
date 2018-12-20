#pragma once
#include "stdafx.h"
#include "SocketDialog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW

#endif
void CSocketDialog::SetEdgeAlignDataProcessing(CDataSocket* pSocket)
{
	int nMsgLen = pSocket->GetLastLength();

	wchar_t wszMsg[4096];

	if (nMsgLen > 4095) 
	{
		nMsgLen = 4095;
	}

	CEtc::ConvertUnicode(pSocket->GetLastMsg(), nMsgLen, wszMsg, 4096);
	wszMsg[nMsgLen] = NULL;

	CString strMsg;
	strMsg.Format(_T("%s"), wszMsg);
	//나중에 strMsg = strMsg.SpanExcluding(_T("\r\n"));			// CR\LF 제거

	if(nMsgLen > 0)
	{
#ifndef CMiLRe
		m_pLogTCP->Write(_T("<- %s\t%s"), pSocket->GetSocketName(), strMsg);
#else
		TRACE(_T("<- %s\t%s\n"), pSocket->GetSocketName(), strMsg);
#endif

		ParseEdgeAlignMessage(strMsg);
	}
}
void CSocketDialog::SetPreAlignDataProcessing(CDataSocket* pSocket)
{
	int nMsgLen = pSocket->GetLastLength();

	wchar_t wszMsg[4096];

	if (nMsgLen > 4095) 
	{
		nMsgLen = 4095;
	}

	CEtc::ConvertUnicode(pSocket->GetLastMsg(), nMsgLen, wszMsg, 4096);
	wszMsg[nMsgLen] = NULL;

	CString strMsg;
	strMsg.Format(_T("%s"), wszMsg);
	//나중에 strMsg = strMsg.SpanExcluding(_T("\r\n"));			// CR\LF 제거

	if(nMsgLen > 0)
	{
#ifndef CMiLRe
		m_pLogTCP->Write(_T("<- %s\t%s"), pSocket->GetSocketName(), strMsg);
#else
		TRACE(_T("<- %s\t%s\n"), pSocket->GetSocketName(), strMsg);
#endif

		ParsePreAlignMessage(strMsg);
	}
}
BOOL CSocketDialog::ParseEdgeAlignMessage(CString strReadingText )
{

	return TRUE;

}
BOOL CSocketDialog::ParsePreAlignMessage(CString strReadingText )
{

	return TRUE;

}
BOOL CSocketDialog::SendMassageToEdgeAlign()
{
	CString strMsg;

	if(m_pSocket_EdgeAlign.IsConnected() == FALSE)
		return FALSE;

	// 데이터 받았다. Run 해라 
	strMsg.Format(_T("Run"));

	char szBuff[256];

	CEtc::ConvertMultibyte((wchar_t*)strMsg.GetString(), szBuff, 256);
	int nSndLen = m_pSocket_EdgeAlign.Send(szBuff, strlen(szBuff));

	if ( nSndLen == strlen(szBuff) )
	{
		TRACE(_T("MAIN -> %s\t%s\n"), SOCKET_EDGEALIGN, strMsg);
		return TRUE;
	}
	else
	{
		TRACE(_T("MAIN *> %s\t Send Fail [%s]\n"), SOCKET_EDGEALIGN, strMsg);
		return FALSE;
	}
}
BOOL CSocketDialog::SendMassageToPreAlign()
{
	CString strMsg;

	if(m_pSocket_PreAlign.IsConnected() == FALSE)
		return FALSE;
	// 데이터 받았다. Run 해라 
	strMsg.Format(_T("Run"));

	char szBuff[256];

	CEtc::ConvertMultibyte((wchar_t*)strMsg.GetString(), szBuff, 256);
	int nSndLen = m_pSocket_EdgeAlign.Send(szBuff, strlen(szBuff));

	if ( nSndLen == strlen(szBuff) )
	{
		TRACE(_T("MAIN -> %s\t%s\n"), SOCKET_EDGEALIGN, strMsg);
		return TRUE;
	}
	else
	{
		TRACE(_T("MAIN *> %s\t Send Fail [%s]\n"), SOCKET_EDGEALIGN, strMsg);
		return FALSE;
	}

	return TRUE;

}
