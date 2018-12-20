#pragma once
#include "stdafx.h"
#include "SocketDialog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//kjpark 20160912 UVMCR 파싱
void CSocketDialog::SetUVMCRDataProcessing(CDataSocket* pSocket)
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
		
		ParseUVMCRMessage(strMsg);
	}
}

//kjpark 20160912 UVMCR 파싱
// UV-MCR에서 응답이 수신되었다.
BOOL CSocketDialog::ParseUVMCRMessage(CString strReadingText )
{
	// 제공해주는 함수에서 처리 한다 17/04/25 HSK	
//	theProcBank.m_strLastCellID = strReadingText.SpanExcluding(_T("\n\r"));

	return TRUE;
}

//kjpark 20160912 UVMCR 파싱
// UV-MCR에서 응답이 수신되었다.
BOOL CSocketDialog::ParseUVMCRMessage( int nIdx, CString strReadingText )
{
	return TRUE;
}