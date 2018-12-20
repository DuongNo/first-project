//////////////////////////////////////////////////////////////////////////
// CDataSocket & CClientSocket
// CDataSocket은 Server와 Client 통합형 Class
// CClientSocket은 Server로써 동작하는 CDataSocket가 관리하는 Client Class
// 외부와의 연결은 Message로 이루어진다
// 

/* **************************************************************************
// Server 사용법
CDataSocket m_ServerSocket;
m_ServerSocket.SetReciveHwnd(this->GetSafeHwnd());
m_ServerSocket.SetSocketName(_T("SERVER_NAME"));
m_ServerSocket.SetServer(TRUE);
m_ServerSocket.SetPort(7000);

// AddClientName()를 통해 허용할 IP만 listClientName 리스트에 등록한다.
// Accept시 listClientName 리스트에 등록된 것만 연결된다.
ClientSocketName socketName;
socketName.Name = _T("CLIENT_NAME1");
socketName.IP = _T("192.168.10.101);
m_ServerSocket.AddClientName(socketName);
socketName.Name = _T("CLIENT_NAME2");
socketName.IP = _T("192.168.10.101);
m_ServerSocket.AddClientName(socketName);

if(m_ServerSocket.Create(m_ServerSocket.GetPort()))
{
m_ServerSocket.Listen();
}

 ************************************************************************** */

/* **************************************************************************
// Client 사용법
CDataSocket m_Socket;
m_Socket.SetReciveHwnd(this->GetSafeHwnd());
m_Socket.SetSocketName(_T("NAME"));
m_Socket.SetServer(FALSE);
m_Socket.SetIP(_T("127.0.0.1"));
m_Socket.SetPort(7000);
m_Socket.Create();
m_Socket.ConnectToServer();
// 연결 끊길 시 WM_RE_CONNECT 메세지를 사용하면 계속 재연결시도 할 수 있다
 ************************************************************************** */


// History
// CSocket을 CAyncSocket으로 변경 [31/5/2009 OSC]
// 동관꺼와 병합 [9/8/2015 OSC]
// CientSocket Close시 hWnd 잘못 보내는거 수정 [3/8/2016 OSC]
// m_strErrorText & WM_SOCKET_ERROR_MSG 삭제 [7/11/2016 OSC]
// m_fSocketCreated 삭제 [7/11/2016 OSC]
// 사용방법 재정리 [7/11/2016 OSC]
//////////////////////////////////////////////////////////////////////////
#pragma once
#include "Device/SockDefine.h"

#define MAX_PACKET_BUFFER	4096
#define WM_PACKET_RECEIVE	WM_USER+500
#define WM_RE_CONNECT		WM_USER+501	// Connect가 되지 않아 재연결이 필요할 때마다 호출된다 (Client 전용)
#define WM_SOCKET_CLOSE		WM_USER+502	// 소켓이 끊겼을 때
#define WM_SOCKET_CONNECT	WM_USER+503	
#define UM_INITIAL_DATAUPDATE WM_USER+504 // View 화면을 초기화하고 사용을 활성화 / 비활성화 한다.
/////////////////////////////////////////////////////////////////////////////
// CDataSocket command target
class CClientSocket;

struct ClientSocketName 
{
	CString IP;
	int Port;
	CString Name;
} ;

class CDataSocket : public CAsyncSocket
{
public:
	CDataSocket();
	virtual ~CDataSocket();

protected:
	UINT m_PortNo;
	CString m_strName;
	CString m_strIP;

	BOOL m_fSocketConnected;
	BOOL m_bIsServer;
	BOOL m_bClient;

	HWND m_hRcvWnd;
	CList <ClientSocketName, ClientSocketName> listClientName;
	CList <CClientSocket*, CClientSocket*> listClient;

// public:
	char m_lastMessage[MAX_PACKET_BUFFER];
	long m_Length;

public:
	void InitDataSocket();
	void SetReciveHwnd(HWND hWnd) { m_hRcvWnd = hWnd; }
	HWND GetReciveHwnd() { return m_hRcvWnd; }
	void SetPort(long portNo) { m_PortNo = portNo; }
	long GetPort() { return m_PortNo; }
	void SetSocketName(CString strName) { m_strName = strName; }
	CString GetSocketName() { return m_strName; }
	void SetIP(CString ip) { m_strIP = ip; }
	CString GetIP() { return m_strIP; }
	char* GetLastMsg() { return m_lastMessage; }
	int GetLastLength() { return m_Length; }

	BOOL IsCreated() {	return m_hSocket == INVALID_SOCKET ? FALSE:TRUE; }
	BOOL IsConnected() { return m_fSocketConnected; }
	void SetServer(BOOL bIsServer) { m_bIsServer = bIsServer; }
	BOOL IsServer() {return m_bIsServer; }

	//////////////////////////////////////////////////////////////////////////
	// Server 전용
public:
	virtual void OnAccept(int nErrorCode);
	BOOL SendClient(CString strSocketName, const void* lpBuf, int nBufLen);
	void AddClientName(ClientSocketName socketName);
	BOOL AddClient(CClientSocket* pClient);
	void RemoveClient(ClientSocketName socketName);
	CClientSocket* GetClientAt(CString socketname);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Client 전용
public:
	BOOL ConnectToServer();
	virtual void OnConnect(int nErrorCode);
	//////////////////////////////////////////////////////////////////////////

public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);

};

class CClientSocket	:	public CDataSocket
{
public:
	CClientSocket();
	virtual ~CClientSocket();

private:
	CDataSocket* m_pServerSocket;

public:
	void SetServerSocket(CDataSocket* pServerSocket) { m_pServerSocket = pServerSocket; }

public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};
