//
// Server 기능을 담당하는 Socket class
//

#pragma once


#include <afxsock.h>
#include "GxCommDefine.h"

#include "Gaus.h"
#include "GxCallback.h"
#include "File\GxLog.h"


//class CGxSockMgr;
class CGxSockClient;
class _GAUS_LIB CGxSockServer : public CAsyncSocket
{


public:
	BOOL			m_bMultiConnect;										// Client 접속 관리 모드, FALSE:단일 peer 접속만 허용, TRUE:다중 Client 접속 허용
	BOOL			m_bMultiPeer;											// 다중 Client 접속 모드에서 동일 IP에 대하여 복수개의 연결을 허용할 것인가 ?

	CString		m_sName;														// 고유 식별 이름, 이것을 가지고 판단한다.
	int			m_nID;														// 고유 번호, 참고용
	int			m_nIndex;													// 순번, 일반 Client일경우는 항상 0, Server에 Accept되어 생선된 Client인경우 생성 순번
	int			m_nTag;														// 다용도 꼬리표

	UINT			m_nPort;


protected:
	int			m_nError;
	BOOL			m_bCreate;

//d	CGxIni	 m_profile;
	CGxLog		m_debug;

	CWnd*			m_pParent;
	UINT			m_umAccept;
	UINT			m_umClose;
	UINT			m_umReceive;

	UINT			m_nPktMode;
	CByteArray	m_arrPktToken;
	UINT			m_nPktIndex;
	UINT			m_nPktLen;

	// Callback 함수 
	CGxCallback*	m_pFncConnect;										// 연결요청 이벤트 발생시 호출하는 Callback함수, Connect
	CGxCallback*	m_pFncClose;										// 연결 끊김 이벤트 발생시 호출
	CGxCallback*	m_pFncReceive;										// 수신 이벤트 발생시 호출하는 Callback함수

	CTypedPtrList<CPtrList, CGxSockClient*> m_listClient;		// 접속된 Client 관리 List

public:
	CGxSockServer(int nIdx = 0);
	virtual ~CGxSockServer();

	// 접속 모드 지정
	void SetMultiConnect(BOOL bFlag);								// 다중 Client 접속을 허용할 것인가 ?
	void SetMultiPeer(BOOL bFlag);									// 다중 Client 접속시 동일 IP에 대하여 다중 접속을 허용할 것인가 ?

	int	GetIndex();														// 지정된 Index를 조회한다.
	int	GetID();															// 지정된 ID를 조회한다.
	CString GetName();													// 이름을 조회한다.


	int	GetError();
	BOOL  IsCreate();
	UINT  GetClientCount();												// 접속된 Client 수
	CGxSockClient* GetClient(CString sIP, int nPos = 0);		// 지정 주소로 접속된 Client를 조회한다.
	CGxSockClient* GetClient(int nIdx);								// 지정 순번의 접속된 Client를 조회한다.


	void  SetMsgInfo(CWnd* pWnd, UINT umAccept, UINT umClose, UINT umReceive);
	void  SetPacketInfo(UINT nMode, CByteArray* parrToken=NULL, int nIndex=0, UINT nLen=0);
	void  SetCallbackFunction(CGxCallback* pConnect, CGxCallback* pClose, CGxCallback* pReceive);

	void	SetPort(int nPort);														// Port 지정
	BOOL  Listen();																	// Listen을 시작한다. 
	BOOL	Listen(int nPort);														// 지정한 Port로 Listen을 시작한다.
	void  Close();
	void	Disconnect(CString sPeerIP=_T(""));									//  지정 / 모든 연결을 Close 시킨다.

	// BYTE 배열로 전송
	int	Send(CString sIP, const void* pBuff, int nLen);					// 지정 IP를 가지는 Client에게 데이터를 전송한다.
	int	Send(CGxSockClient* pClient, const void* pBuff, int nLen);	// 지정 Client에게 데이터를 전송한다.
	int	Send(int nIdx, const void* pBuff, int nLen);						// 지정 순번의 Client에게 데이터를 전송한다.
	int	Send(const void* pBuff, int nLen);									// 접속한 모든 Client에게 데이터를 전송한다.
	
	// CString으로 전송
	int	SendString(CString sIP, CString sMsg);								// 지정 IP를 가지는 Client에게 String 데이터를 전송한다.
	int	SendString(CGxSockClient* pClient, CString sMsg);				// 지정 Client에게 Strig 데이터를 전송한다.
	int	SendString(int nIdx, CString sMsg);									// 지정 순번의 Client에게 String 데이터를 전송한다.
	int	SendString(CString sMsg);												// 접속한 모든 Client에게 String 데이터를 전송한다.


protected:
	virtual void HandleException(int nExp);
	virtual void SetError(int nError);


public:
	//{{AFX_VIRTUAL(CGxSockServer)
	public:
	virtual void OnAccept(int nErrorCode);
	//}}AFX_VIRTUAL
	void OnCloseClient(CGxSockClient* pClient);
};

typedef CTypedPtrList<CPtrList, CGxSockServer*> CGxSockServerPtrList;
typedef CTypedPtrArray<CPtrArray, CGxSockServer*> CGxSockServerPtrArray;
typedef CTypedPtrMap<CMapStringToPtr, CString, CGxSockServer*> CGxMapStringToSockServer;

