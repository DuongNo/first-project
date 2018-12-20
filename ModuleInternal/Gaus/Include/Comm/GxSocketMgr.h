//
// CGxSocketMgr : TCP/IP Socekt ��� ������ class
//
#pragma once

#include "Gaus.h"
#include "GxObject.h"
#include "GxSingleton.h"
#include "GxCallback.h"


#include "Comm\GxCommDefine.h"
#include "Comm\GxSockClient.h"
#include "Comm\GxSockServer.h"
#include "File\GxLog.h"


// TCP/IP Socket ������ ���� ���� ���� ����
typedef struct _GAUS_LIB _TGxSocketInfo
{
	// ������ ���� ����
	int			nID;					// Ư������ ID
	CString		sName;				// Socket �̸�
	int			nFlag;				// ���� ������ ������


	BOOL			bIsAutoRetry;		// �ڵ� ������ ����� ��� �� ���ΰ� ?
	BOOL			bEnabled;			// ��� ����, TRUE : ���, FALSE : ������� �ʴ´�. ������ ������ ������� �������� �� ����
	BOOL			bIsConnect;			// ���� ����, �� ������ ���� �ڵ� �������� �õ��Ѵ�. FALSE : ���� �õ� �ʿ�, TRUE : ���� �������̰ų� ���� �õ����̴�.
	BOOL			bIsConnectTry;		// ���� �õ����ΰ� ? ������ ����߿� �ٽ� ������ �õ����� �ʵ��� �Ѵ�.
	int			nTimeCount;			// �ڵ� ������ ���� �ð� ����

	CString		sIPAddress;			// IP �ּ�
	int			nPortNo;				// Port ��ȣ

	// Server Socket�� ��� ���Ǵ� ����
	BOOL			bIsServer;			// Server Socket�ΰ� ? TRUE : Server socket, FALSE : Cilent Socket
	BOOL			bIsMultiConnect;	// ���� ���� ��� ���� : ���� ��Ʈ�� �ټ��� IP�� ���� ����
	BOOL			bIsMultiPeer;		// ���� IP ���� ���� ����


	// Token ������ ���� ����
	UINT			nPacketMode;			// ��Ŷ�� �����ϴ� ���
	CByteArray	arrToken;			// nTokenMode�� eGxCOMM_PKT_TOKEN ����� ��� ��ū ���ڵ� �迭
	int			nTokenPos;			// Token ���� ��ġ
	UINT			nPacketLen;			// Token/������ ����


	// ���� �޼����� ȸ���� ����
	CWnd*					pWnd;
	CGxSockClient**	pClientSocket;			// Socket ��ſ� ������
	CGxSockServer**	pServerSocket;			// Server socket ������

	CGxCallback*		pFncReceive;			// Data�� �����Ͽ��� �� ���� �� �Լ� ������
	CGxCallback*		pFncConnect;			// Socket ������ �̷������ �� ���� �� �Լ� ������
	CGxCallback*		pFncClose;				// Socket ������ �������� �� ���� �� �Լ� ������


	_TGxSocketInfo();								// ������
	_TGxSocketInfo(_TGxSocketInfo& rSrc);	// ������ 2
	const  _TGxSocketInfo& operator =( const _TGxSocketInfo& rSrc);

} TGxSocketInfo;
typedef CTypedPtrArray<CPtrArray, TGxSocketInfo*> TGxSocketInfoPtrArray;


// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==


class _GAUS_LIB CGxSocketMgr : public CGxObject, public CGxSingleton <CGxSocketMgr>
{
public:
	CGxSocketMgr(void);
	~CGxSocketMgr(void);


private:

	int			m_nRetryInterval;							// ���� ��õ��ϴ� ����, ��

	CWnd*			m_pOwner;									// �޼����� ������ ���
	UINT			m_umConnect;								// ����� �޼���
	UINT			m_umClose;									// ������ �޼���
	UINT			m_umReceive;								// ������ ���Ž� �޼���
	UINT			m_umConnectFail;							// �������, ���� ��õ� ��û

	CWnd*			m_pEventOwner;								// �̺�Ʈ �޼����� ���Ź��� ���
	UINT			m_umStateChange;							// ���� Client Socket���� ���°� ����Ǿ��� ��� �뺸���ش�.
	CGxCallback*	m_pFncStateChange;					// ���� ������ �߻��Ͽ��� �� ȣ���� �Լ� 

	CGxLog		m_Log;										// Log class


	CCriticalSection m_CSClient;							// ���� ���� ����
	CCriticalSection m_CSServer;							


	// Clinet / Server Socket ����
	TGxSocketInfoPtrArray		m_arrClients;			// Client Socket ���� ���� Array
	TGxSocketInfoPtrArray		m_arrServers;			// Server Socket ���� ���� Array


	// Call-Back function
	typedef TCallbackDefine< CGxSocketMgr > TmyCallbackDefine;

	// for Client Socket
	TmyCallbackDefine *m_pFuncSocketConnect;			// ����� ȣ���� �Լ�
	TmyCallbackDefine *m_pFuncSocketClose;				// ���� ������ ȣ���� �Լ�
	TmyCallbackDefine *m_pFuncSocketConnectFail;		// ������н� ȣ���� �Լ�

	// for Server socket
	TmyCallbackDefine *m_pFuncServerConnect;			// ����� ȣ���� �Լ�
	TmyCallbackDefine *m_pFuncServerClose;				// ���� ������ ȣ���� �Լ�

	// Callback �Լ� ��ü
	void OnFncSocketConnect(void* p);					// Client�� ���ӿ� �����ߴ�.
	void OnFncSocketClose(void* p);						// Client ������ ��������.
	void OnFncSocketConnectFail(void* p);				// Client�� ���ӿ� �����Ͽ���.

	void OnFncServerConnect(void* p);					// Server�� Client�� �����ߴ�.
	void OnFncServerClose(void* p);						// Server���� Client�� ������ ��������.


	// ����
	BOOL ClientConnect(int nIdx);							// ���� Index�� Client Socket�� ������ �õ��Ѵ�.
	BOOL ServerListen(int nIdx);							// ���� Index�� Server Socekt�� ������ �����Ѵ�.


public:


	// �߰� / ����
	void SetRetryInterval(int nSec);						// �ڵ� ��õ� ������ �ʴ����� �����Ѵ�.
	int GetRetryInterval();									// �����Ǿ��ִ� �ڵ� ��õ� ������ ��ȸ�Ѵ�.

	BOOL GetAutoRetryFlag(int nID);						// ���� ������ �ڵ� �翬�� ���� ���θ� ��ȸ�Ѵ�.
	BOOL SetAutoRetryFlag(int nID, BOOL bFlag);		// ���� ������ �ڵ� �翬�� ������ �����Ѵ�.

	// ���� ���� ��ȸ
	BOOL IsConnect(int nID);								// ���� ID�� ���� Client Ȥ�� Server�� ������ �Ǿ��� ?
	BOOL IsClientConnect(int nID);						// ���� Client�� ���ӵ� �����ΰ� ?
	BOOL IsServerConnect(int nID);						// ���� Server�� Client�� ���ӵ� �����ΰ� ?


	//   ���� ���� ����� ȣ���� �Լ��� void* ���� TGxSocketInfo* ���� ĳ�����Ͽ� ����ϵ��� �Ѵ�.
	void SetEventHandle(CWnd* pOwner, UINT nStateChange, CGxCallback*	pFncStateChange=NULL);

	void SetMessageHandle(CWnd* pOwner, UINT nConnect, UINT nClose, UINT nReceive, UINT nConnectFail);
	int AddClientSocket(TGxSocketInfo &rInfo);		// Clinet socket ������ ����Ѵ�.
	int AddServerSocket(TGxSocketInfo &rInfo);		// Server socket ������ ����Ѵ�.


	// ���� ó�� !!
	void ConnectProcess();									// �����ֱ�� Main UI Thread���� 1�ʸ��� Timer�� ȣ�� �ϵ��� �Ѵ�.

	// ������ �ϵ��� �����Ѵ�. ��ٷ� ������ �����ϴ� ���� �ƴϹǷ� '����' �Ѵٰ� �̸�������.
	void SetConnect(int nID=-1);							// ��� ������ Client/Server�� ������ �����Ѵ�.
	void SetClientConnect(int nID=-1);					// ��� Client Ȥ�� ���� ID�� ������ Client�� ������ �����Ѵ�.
	void SetServerConnect(int nID=-1);					// ��� Server Ȥ�� ���� ID�� ������ Server�� ������ �����Ѵ�.

	// ���� ����
	void ConnectClose(int nID=-1);						// ����� Client Ȥ�� Server�� ������ �����Ѵ�.
	void ClientClose(int nID=-1);							// ��� Client Ȥ�� ���� ID�� ������ Client�� ������ �����ش�.
	void ServerClose(int nID=-1);							// ��� Server Ȥ�� ���� ID�� ������ Server�� ������ �����ش�. 


	// Index ��ȸ
	int GetClientIndex(int nID);							// �־��� ID�� ������ Client�� Index ��ȸ, ��ã���� -1 ����
	int GetClientIndex(CString sName);					// ������ �̸��� ������ Client�� Index ��ȸ, ��ã���� -1 ����
	int GetServerIndex(int nID);							// �־��� ID�� ������ Server�� Index ��ȸ, ��ã���� -1 ����
	int GetServerIndex(CString sName);					// ������ �̸��� ������ Server�� Index ��ȸ, ��ã���� -1 ����

	// Log ����� ���� ������ ��ȸ
	CGxLog*	GetLogPtr();

	// BOOL ConnectProcess(CGxCallback* pConnect, CGxCallback* pClose, CWnd* pOwner, UINT umReconnect);	// ������ �õ��Ѵ�.


};

