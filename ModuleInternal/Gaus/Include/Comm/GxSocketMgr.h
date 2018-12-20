//
// CGxSocketMgr : TCP/IP Socekt 통신 관리용 class
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


// TCP/IP Socket 연결을 위한 각종 정보 보관
typedef struct _GAUS_LIB _TGxSocketInfo
{
	// 접속을 위한 정보
	int			nID;					// 특정지을 ID
	CString		sName;				// Socket 이름
	int			nFlag;				// 연결 동작의 구분자


	BOOL			bIsAutoRetry;		// 자동 재접속 기능을 사용 할 것인가 ?
	BOOL			bEnabled;			// 사용 여부, TRUE : 사용, FALSE : 사용하지 않는다. 강제로 접속을 끊으라고 지령했을 때 설정
	BOOL			bIsConnect;			// 접속 여부, 이 변수를 보고 자동 재접속을 시도한다. FALSE : 접속 시도 필요, TRUE : 현재 접속중이거나 접속 시도중이다.
	BOOL			bIsConnectTry;		// 접속 시도중인가 ? 재접속 대기중에 다시 접속을 시도하지 않도록 한다.
	int			nTimeCount;			// 자동 재접속 지연 시간 누적

	CString		sIPAddress;			// IP 주소
	int			nPortNo;				// Port 번호

	// Server Socket일 경우 사용되는 변수
	BOOL			bIsServer;			// Server Socket인가 ? TRUE : Server socket, FALSE : Cilent Socket
	BOOL			bIsMultiConnect;	// 다중 접속 허용 여부 : 동일 포트에 다수의 IP를 가진 연결
	BOOL			bIsMultiPeer;		// 동일 IP 다중 접속 여부


	// Token 설정을 위한 변수
	UINT			nPacketMode;			// 패킷을 구분하는 방법
	CByteArray	arrToken;			// nTokenMode가 eGxCOMM_PKT_TOKEN 모든인 경우 토큰 문자들 배열
	int			nTokenPos;			// Token 기준 위치
	UINT			nPacketLen;			// Token/정보의 길이


	// 각종 메세지를 회신할 변수
	CWnd*					pWnd;
	CGxSockClient**	pClientSocket;			// Socket 통신용 포인터
	CGxSockServer**	pServerSocket;			// Server socket 포인터

	CGxCallback*		pFncReceive;			// Data를 수신하였을 때 실행 할 함수 포인터
	CGxCallback*		pFncConnect;			// Socket 연결이 이루어졌을 때 실행 할 함수 포인터
	CGxCallback*		pFncClose;				// Socket 연결이 끊어졌을 때 실행 할 함수 포인터


	_TGxSocketInfo();								// 생성자
	_TGxSocketInfo(_TGxSocketInfo& rSrc);	// 생성자 2
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

	int			m_nRetryInterval;							// 연결 재시도하는 간격, 초

	CWnd*			m_pOwner;									// 메세지를 수신할 대상
	UINT			m_umConnect;								// 연결시 메세지
	UINT			m_umClose;									// 단절시 메세지
	UINT			m_umReceive;								// 데이터 수신시 메세지
	UINT			m_umConnectFail;							// 연결실패, 연결 재시도 요청

	CWnd*			m_pEventOwner;								// 이벤트 메세지를 수신받을 대상
	UINT			m_umStateChange;							// 각종 Client Socket들의 상태가 변경되었을 경우 통보해준다.
	CGxCallback*	m_pFncStateChange;					// 상태 변경이 발생하였을 때 호출할 함수 

	CGxLog		m_Log;										// Log class


	CCriticalSection m_CSClient;							// 다중 접근 방지
	CCriticalSection m_CSServer;							


	// Clinet / Server Socket 관리
	TGxSocketInfoPtrArray		m_arrClients;			// Client Socket 정보 보관 Array
	TGxSocketInfoPtrArray		m_arrServers;			// Server Socket 정보 보관 Array


	// Call-Back function
	typedef TCallbackDefine< CGxSocketMgr > TmyCallbackDefine;

	// for Client Socket
	TmyCallbackDefine *m_pFuncSocketConnect;			// 연결시 호출할 함수
	TmyCallbackDefine *m_pFuncSocketClose;				// 연결 해제시 호출할 함수
	TmyCallbackDefine *m_pFuncSocketConnectFail;		// 연결실패시 호출할 함수

	// for Server socket
	TmyCallbackDefine *m_pFuncServerConnect;			// 연결시 호출할 함수
	TmyCallbackDefine *m_pFuncServerClose;				// 연결 해제시 호출할 함수

	// Callback 함수 본체
	void OnFncSocketConnect(void* p);					// Client가 접속에 성공했다.
	void OnFncSocketClose(void* p);						// Client 접속이 끊어졌다.
	void OnFncSocketConnectFail(void* p);				// Client가 접속에 실패하였다.

	void OnFncServerConnect(void* p);					// Server에 Client가 접속했다.
	void OnFncServerClose(void* p);						// Server에서 Client가 접속이 끊어졌다.


	// 연결
	BOOL ClientConnect(int nIdx);							// 지정 Index의 Client Socket의 연결을 시도한다.
	BOOL ServerListen(int nIdx);							// 지정 Index의 Server Socekt의 연결을 지정한다.


public:


	// 추가 / 설정
	void SetRetryInterval(int nSec);						// 자동 재시도 간격을 초단위로 설정한다.
	int GetRetryInterval();									// 설정되어있는 자도 재시도 간격을 조회한다.

	BOOL GetAutoRetryFlag(int nID);						// 지정 소켓의 자동 재연결 설정 여부를 조회한다.
	BOOL SetAutoRetryFlag(int nID, BOOL bFlag);		// 지정 소켓의 자도 재연결 설정을 지정한다.

	// 연결 여부 조회
	BOOL IsConnect(int nID);								// 지정 ID를 가진 Client 혹은 Server가 연결이 되었나 ?
	BOOL IsClientConnect(int nID);						// 지정 Client가 접속된 상태인가 ?
	BOOL IsServerConnect(int nID);						// 지정 Server에 Client가 접속된 상태인가 ?


	//   연결 상태 변경시 호출할 함수는 void* 형을 TGxSocketInfo* 으로 캐스팅하여 사용하도록 한다.
	void SetEventHandle(CWnd* pOwner, UINT nStateChange, CGxCallback*	pFncStateChange=NULL);

	void SetMessageHandle(CWnd* pOwner, UINT nConnect, UINT nClose, UINT nReceive, UINT nConnectFail);
	int AddClientSocket(TGxSocketInfo &rInfo);		// Clinet socket 정보를 등록한다.
	int AddServerSocket(TGxSocketInfo &rInfo);		// Server socket 정보를 등록한다.


	// 연결 처리 !!
	void ConnectProcess();									// 일정주기로 Main UI Thread에서 1초마다 Timer로 호출 하도록 한다.

	// 연결을 하도록 지정한다. 곧바로 연결을 실행하는 것이 아니므로 '지정' 한다고 이름지었다.
	void SetConnect(int nID=-1);							// 모든 지정된 Client/Server의 연결을 지정한다.
	void SetClientConnect(int nID=-1);					// 모든 Client 혹은 지정 ID를 가지는 Client의 접속을 지정한다.
	void SetServerConnect(int nID=-1);					// 모든 Server 혹은 지정 ID를 가지는 Server의 접속을 지정한다.

	// 연결 해제
	void ConnectClose(int nID=-1);						// 연결된 Client 혹은 Server의 연결을 해제한다.
	void ClientClose(int nID=-1);							// 모든 Client 혹은 지정 ID를 가지는 Client의 연결을 끊어준다.
	void ServerClose(int nID=-1);							// 모든 Server 혹은 지정 ID를 가지는 Server의 연결을 끊어준다. 


	// Index 조회
	int GetClientIndex(int nID);							// 주어진 ID를 가지는 Client의 Index 조회, 못찾으면 -1 리턴
	int GetClientIndex(CString sName);					// 주정진 이름을 가지는 Client의 Index 조회, 못찾으면 -1 리턴
	int GetServerIndex(int nID);							// 주어진 ID를 가지는 Server의 Index 조회, 못찾으면 -1 리턴
	int GetServerIndex(CString sName);					// 주정진 이름을 가지는 Server의 Index 조회, 못찾으면 -1 리턴

	// Log 기록을 위한 포인터 조회
	CGxLog*	GetLogPtr();

	// BOOL ConnectProcess(CGxCallback* pConnect, CGxCallback* pClose, CWnd* pOwner, UINT umReconnect);	// 접속을 시도한다.


};

