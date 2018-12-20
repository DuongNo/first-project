#pragma once
#include "stdafx.h"
#include "SocketDialog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//kjpark 20160912 UVMCR �Ľ�
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
	//���߿� strMsg = strMsg.SpanExcluding(_T("\r\n"));			// CR\LF ����

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

//kjpark 20160912 UVMCR �Ľ�
// UV-MCR���� ������ ���ŵǾ���.
BOOL CSocketDialog::ParseUVMCRMessage(CString strReadingText )
{
	// �������ִ� �Լ����� ó�� �Ѵ� 17/04/25 HSK	
//	theProcBank.m_strLastCellID = strReadingText.SpanExcluding(_T("\n\r"));

	return TRUE;
}

//kjpark 20160912 UVMCR �Ľ�
// UV-MCR���� ������ ���ŵǾ���.
BOOL CSocketDialog::ParseUVMCRMessage( int nIdx, CString strReadingText )
{
	return TRUE;
}