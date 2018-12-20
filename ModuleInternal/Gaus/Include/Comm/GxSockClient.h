//
//
//
//

#pragma once


#include <afxsock.h>

#include "Gaus.h"
#include "GxCallback.h"
#include "GxCommDefine.h"
#include "File\GxLog.h"


class CGxSockServer;
class _GAUS_LIB CGxSockClient : public CAsyncSocket
{

public:
	CString		m_sName;														// 고유 식별 이름, 이것을 가지고 판단한다.
	int			m_nID;														// 고유 번호, 참고용
	int			m_nIndex;													// 순번, 일반 Client일경우는 항상 0, Server에 Accept되어 생선된 Client인경우 생성 순번
	int			m_nTag;														// 다용도 꼬리표

	CString		m_sIP;
	UINT			m_nPort;

	CGxLog		m_debug;

protected:
	int			m_nError;
	BOOL			m_bConnect;
	BOOL			m_bSimulate;

	CWnd*			m_pOwner;													// 메세지를 수신할 대상
	UINT			m_umConnect;												// 연결시 메세지
	UINT			m_umClose;													// 단절시 메세지
	UINT			m_umReceive;												// 데이터 수신시 메세지
	UINT			m_umConnectFail;											// 연결시도가 실패하였다. 연결 재시도 요청

	// Callback 함수 
	CGxCallback*	m_pFncConnect;											// 연결 이벤트 발생시 호출하는 Callback함수, Connect/Accept
	CGxCallback*	m_pFncClose;											// 연결 끊김 이벤트 발생시 호출
	CGxCallback*	m_pFncReceive;											// 수신 이벤트 발생시 호출하는 Callback함수
	CGxCallback*	m_pFncConnectFail;									// 연결시도가 실패하였을때 호출하는 Callback함수

	CGxCommQueue   m_RcvQueue;												// 수신된 데이터가 저장되는 Queue
	CGxSockServer* m_pServer;


public:
	CGxSockClient(int nID = 0, CGxSockServer* pServer=NULL);
	virtual ~CGxSockClient();
	
	int	GetIndex();														// 지정된 Index를 조회한다.
	int	GetID();															// 지정된 ID를 조회한다.
	CString GetName();													// 이름을 조회한다.
	

	int	GetError();
	BOOL  IsConnect();
	UINT	GetPacketMode(int& nIdx, int& nLen);						// 패킷 구분자 모드 및 정보

	void  SetMsgInfo(CWnd* pWnd, UINT umConnect, UINT umClose, UINT umReceive, UINT umConnectFail=0);
	void  SetPacketInfo(UINT nMode, CByteArray* parrToken=NULL, int nIndex=0, UINT nLen=0);
	void  SetCallbackFunction(CGxCallback* pConnect, CGxCallback* pClose, CGxCallback* pReceive, CGxCallback* pConnectFail=0);

	void SetAddress(CString sAddr, int nPort);					// 주소/포트 지정

	BOOL  Connect();														// 설정된 주소로 접속 시도
	BOOL	Connect(CString sAddr, int nPort);						// 지정하는 주소로 접속 시도
	void  Close();

	void  Clear();
	UINT  IsPacket();														// 수신된 패킷이 있는지 길이를 조회한다. 0:없음, n ~ 패킷이 존재하며 그 길이는 n 이다.
	UINT	GetReceiveSize();												// 수신된 데이터 길이를 조회한다 == Ispacket();
	UINT  GetPacket(BYTE* pbyData);									// 수신된 패킷을 읽어온다. (원본 Clear)
	UINT  GetPacket(BYTE* pbyData, int nLen);						// 지정된 길이만큼 패킷을 읽어온다.

	CString GetReceiveString();										// 수신된 모든 내용을 CString 형으로 받아온다. MBCS -> Unicode
	CString GetReceiveString(int nLen);								// 지정된 길이만큼 패킷을 CString 형으로 받아온다. MBCS -> Unicode
	CString GetPacketString();											// 수신된 패킷단위로 CString 형으로 받아온다. MBCS -> Unicode

	int	SendString(CString sSendData);							// 지정된 문자열을 전송한다.

protected:
	virtual void HandleException(int nExp);
	virtual void SetError(int nError);


public:
	//{{AFX_VIRTUAL(CGxSockClient)
	public:
	virtual void OnConnect(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL
};

typedef CTypedPtrList<CPtrList, CGxSockClient*> CGxSockClientPtrList;
typedef CTypedPtrArray<CPtrArray, CGxSockClient*> CGxSockClientPtrArray;
typedef CTypedPtrMap<CMapStringToPtr, CString, CGxSockClient*> CGxMapStringToSockClient;

