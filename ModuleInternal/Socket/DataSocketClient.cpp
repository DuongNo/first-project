// DataSocket.cpp : implementation file
//

#include "stdafx.h"
#include "DataSocket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



CClientSocket* CDataSocket::GetClientAt( CString socketname )
{
	// 기존에 연결되있는게 있으면 해제한다.
	CClientSocket* pSocket = NULL;
	POSITION pos = listClient.GetHeadPosition();
	while(pos)
	{
		pSocket = listClient.GetNext(pos);
		if(pSocket->GetSocketName() == socketname)
			return pSocket;
		else 
			pSocket = NULL;
	}
	return pSocket;
}

/////////////////////////////////////////////////////////////////////////////
// CClientSocket
CClientSocket::CClientSocket()
{
	InitDataSocket();
	m_bClient = TRUE;
}

CClientSocket::~CClientSocket()
{
}

void CClientSocket::OnReceive(int nErrorCode)
{
	int nRtnLen = Receive(m_lastMessage, MAX_PACKET_BUFFER);
	m_Length = nRtnLen;

	switch (m_Length)
	{
// 	case 0:
// 		Close();
// 		break;
	case SOCKET_ERROR:
		if (GetLastError() != WSAEWOULDBLOCK) 
		{
			if (GetLastError() != WSAEMSGSIZE) {
				TRACE(_T("OnReceive error: %d"), GetLastError());
			} else {
				TRACE(_T("The datagram was too large and was truncated"));
			}
		}
		break;

	default:
		if(nRtnLen>0)
		{
			m_lastMessage[nRtnLen] = NULL;
			if (m_pServerSocket->GetReciveHwnd() != NULL)
			{
				::SendMessage(m_pServerSocket->GetReciveHwnd(), WM_PACKET_RECEIVE, (WPARAM)this, NULL);
			}

//kjpark 20160912 Socket test
#ifdef CMiLRe
			int nMsgLen = nRtnLen;
			wchar_t wszMsg[4096];
			if(nMsgLen > 4095) nMsgLen = 4095;

			CEtc::ConvertUnicode(m_lastMessage, nMsgLen, wszMsg, 4096);
			wszMsg[nMsgLen] = NULL;
			CString strMsg;
			strMsg.Format(_T("%s\n"), wszMsg);
			
			if(strMsg.CompareNoCase(_T("CHECK")))
			{
				TRACE(strMsg);
			}
#endif
		}
	}	
	memset(m_lastMessage, NULL, sizeof(m_lastMessage));

	CAsyncSocket::OnReceive(nErrorCode);
}

void CClientSocket::OnClose(int nErrorCode)
{
	m_fSocketConnected = FALSE;

	CAsyncSocket::OnClose(nErrorCode);

	if (m_pServerSocket->GetReciveHwnd() != NULL)
		::SendMessage(m_pServerSocket->GetReciveHwnd(), WM_SOCKET_CLOSE, (WPARAM)m_pServerSocket, (LPARAM)this);

}

