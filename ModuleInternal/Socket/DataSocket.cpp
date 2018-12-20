// DataSocket.cpp : implementation file
//

#include "stdafx.h"
#include "DataSocket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataSocket
CDataSocket::CDataSocket()
{
	InitDataSocket();
}

CDataSocket::~CDataSocket()
{
	listClientName.RemoveAll();

	if(m_bClient == FALSE)
	{
		CClientSocket* pClient;
		POSITION pos = listClient.GetHeadPosition();
		while(pos)
		{
			pClient = listClient.GetNext(pos);
			pClient->Close();
			delete pClient;
		}
		listClient.RemoveAll();
	}
}
/////////////////////////////////////////////////////////////////////////////
// CDataSocket member functions

void CDataSocket::OnReceive(int nErrorCode) 
{
	int nRtnLen = Receive(m_lastMessage, MAX_PACKET_BUFFER);
	m_Length = nRtnLen;
	switch (nRtnLen)
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
		m_Length = 0;
		break;
	default:
		if(nRtnLen>0)
		{
			m_lastMessage[nRtnLen] = NULL;
			if (m_hRcvWnd != NULL)
			{
				::SendMessage(m_hRcvWnd, WM_PACKET_RECEIVE, (WPARAM)this, nRtnLen);
			}
		}
	}	


	memset(m_lastMessage, NULL, sizeof(char)*(MAX_PACKET_BUFFER));

	CAsyncSocket::OnReceive(nErrorCode);
}

void CDataSocket::OnClose(int nErrorCode) 
{
	m_fSocketConnected = FALSE;

	CAsyncSocket::OnClose(nErrorCode);
	::SendMessage(m_hRcvWnd, WM_SOCKET_CLOSE, (WPARAM)this, NULL);
	::SendMessage(m_hRcvWnd, WM_RE_CONNECT, (WPARAM)this, TRUE);
}


void CDataSocket::OnConnect(int nErrorCode)
{
	if(nErrorCode==0)
	{
		m_fSocketConnected = TRUE;
		::SendMessage(m_hRcvWnd, WM_SOCKET_CONNECT, (WPARAM)this, NULL);
	}
	else
	{
      switch(nErrorCode)
      {
         case WSAEADDRINUSE: 
            TRACE(_T("The specified address is already in use.\n"));
            break;
         case WSAEADDRNOTAVAIL: 
            TRACE(_T("The specified address is not available from ")
            _T("the local machine.\n"));
            break;
         case WSAEAFNOSUPPORT: 
            TRACE(_T("Addresses in the specified family cannot be ")
            _T("used with this socket.\n"));
            break;
         case WSAECONNREFUSED: 
//             TRACE(_T("The attempt to connect was forcefully rejected.\n"));
			if(m_bIsServer == FALSE)					// Socket	  Create 여부
				::SendMessage(m_hRcvWnd, WM_RE_CONNECT, (WPARAM)this, FALSE);
           break;
         case WSAEDESTADDRREQ: 
            TRACE(_T("A destination address is required.\n"));
            break;
         case WSAEFAULT: 
            TRACE(_T("The lpSockAddrLen argument is incorrect.\n"));
            break;
         case WSAEINVAL: 
            TRACE(_T("The socket is already bound to an address.\n"));
            break;
         case WSAEISCONN: 
            TRACE(_T("The socket is already connected.\n"));
            break;
         case WSAEMFILE: 
            TRACE(_T("No more file descriptors are available.\n"));
            break;
         case WSAENETUNREACH: 
            TRACE(_T("The network cannot be reached from this host ")
            _T("at this time.\n"));
            break;
         case WSAENOBUFS: 
            TRACE(_T("No buffer space is available. The socket ")
               _T("cannot be connected.\n"));
            break;
         case WSAENOTCONN: 
            TRACE(_T("The socket is not connected.\n"));
            break;
         case WSAENOTSOCK: 
            TRACE(_T("The descriptor is a file, not a socket.\n"));
            break;
         case WSAETIMEDOUT: 
//             TRACE(_T("The attempt to connect timed out without ")
//                _T("establishing a connection. \n"));
			if(m_bIsServer == FALSE)					// Socket	  Create 여부
				::SendMessage(m_hRcvWnd, WM_RE_CONNECT, (WPARAM)this, FALSE);
           break;
         default:
//             TCHAR szError[256];
//             _stprintf_s(szError, _T("OnConnect error: %d"), nErrorCode);
//             TRACE(szError);
			if(m_bIsServer == FALSE)					// Socket	  Create 여부
				::SendMessage(m_hRcvWnd, WM_RE_CONNECT, (WPARAM)this, FALSE);
          break;
      }
//       TRACE(_T("Please close the application"));
   }


	CAsyncSocket::OnConnect(nErrorCode);
}

void CDataSocket::OnAccept(int nErrorCode)
{
	if(IsServer() == FALSE)
		return;

	CClientSocket* pClient = new CClientSocket;
	if (CAsyncSocket::Accept(*pClient))
	{
		CString strIP;
		UINT nPort;
		pClient->SetServerSocket(this);
		pClient->GetPeerName(strIP, nPort);
		pClient->SetIP(strIP);
		pClient->SetPort(nPort);
		if(AddClient(pClient))
		{
			m_fSocketConnected = TRUE;
			::SendMessage(m_hRcvWnd, WM_SOCKET_CONNECT, (WPARAM)pClient, NULL);
		}
		else
		{
			pClient->Close();
			delete pClient;
			pClient = NULL;
//			AfxMessageBox(_T("Error : Accept() in socket"));
		}
	}
	else
	{
		pClient->Close();
		delete pClient;
		pClient = NULL;
//		AfxMessageBox(_T("Error : Accept() in socket"));
	}

	CAsyncSocket::OnAccept(nErrorCode);
}

BOOL CDataSocket::SendClient( CString strSocketName, const void* lpBuf, int nBufLen )
{
	CClientSocket* pSocket;
	POSITION pos = listClient.GetHeadPosition();
	while(pos)
	{
		pSocket = listClient.GetNext(pos);
		if(pSocket->GetSocketName() == strSocketName)
		{
			if( nBufLen == pSocket->Send(lpBuf, nBufLen) )
				return TRUE;
			break;
		}
	}

	return FALSE;
}

BOOL CDataSocket::ConnectToServer()
{
	if(m_strIP == _T("") || m_PortNo == 0 || IsCreated() == FALSE)
		return FALSE;

	return Connect(m_strIP, m_PortNo);
}

BOOL CDataSocket::AddClient( CClientSocket* pClient )
{
	// 기존에 연결되있는게 있으면 해제한다.
	CClientSocket* pSocket;
	POSITION pos = listClient.GetHeadPosition();
	while(pos)
	{
		pSocket = listClient.GetNext(pos);
		if( (pSocket->GetIP() == pClient->GetIP()) 
			/*&& (pSocket->GetPort() == pClient->GetPort())*/ )
		{
			pSocket->Close();
			delete pSocket;
			if(pos)
			{
				listClient.GetPrev(pos);
				listClient.RemoveAt(pos);
			}
			else
			{
				listClient.RemoveAt(listClient.GetTailPosition());
			}
			break;
		}
	}

	// 소켓 이름을 부여한다.
	ClientSocketName socketname;
	pos = listClientName.GetHeadPosition();
	while(pos)
	{
		socketname = listClientName.GetNext(pos);
		if( (pClient->GetIP() == socketname.IP) /*&& (pClient->GetPort() == socketname.Port)*/ )
		{
			pClient->m_bClient = TRUE;
			pClient->SetSocketName(socketname.Name);
			listClient.AddTail(pClient);
			pClient->m_fSocketConnected = TRUE;
			return TRUE;
		}
	}
	return FALSE;
}

void CDataSocket::AddClientName( ClientSocketName socketName )
{
	ClientSocketName sockNameInList;
	POSITION posName = listClientName.GetHeadPosition();
	// 기존에 있는 건 지우고 새로 등록
	while(posName)
	{
		sockNameInList = listClientName.GetNext(posName);
		if(sockNameInList.Name == socketName.Name)
		{
			// 기존에 있는게 있으면 연결된 것도 끊는다
			RemoveClient(sockNameInList);
			if(posName)
			{
				listClientName.GetPrev(posName);
				listClientName.RemoveAt(posName);
			}
			else
			{
				listClientName.RemoveAt(listClient.GetTailPosition());
			}
			break;
		}
	}
	listClientName.AddTail(socketName);
}

void CDataSocket::RemoveClient( ClientSocketName socketName )
{
	CClientSocket* clientInList;
	POSITION pos = listClient.GetHeadPosition();
	while(pos)
	{
		clientInList = listClient.GetNext(pos);
		if(socketName.Name == clientInList->GetSocketName())
		{
			clientInList->Close();
			delete clientInList;
			if(pos)
			{
				listClient.GetPrev(pos);
				listClient.RemoveAt(pos);
			}
			else
			{
				listClient.RemoveAt(listClient.GetTailPosition());
			}
			break;
		}
	}
}

void CDataSocket::InitDataSocket()
{
	m_PortNo = -1;
	m_fSocketConnected = FALSE;
	m_strName = _T("");
	m_bIsServer = FALSE;
	m_hRcvWnd = NULL;
	m_bClient = FALSE;

	m_Length = 0;
}
