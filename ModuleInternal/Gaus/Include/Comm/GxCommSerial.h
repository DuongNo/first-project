////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// CGxCommSerial : Serial 통신 Base class
//
// #2014-12-19, jhLee : 기존 CommThread class를 Polling -> Event 방식으로 사용하기위해 변형 함.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Gaus.h"
#include "GxMemberThread.h"
#include "GxCallback.h"


#define GxCommMAX_BUFF	4096				// 버퍼사이즈 4KB

//struct _CCommInfo 
//{
//	long portNo;
//	long baudRate;
//	long stopBits;
//	long parity;
//	long byteSize;
//
//	_CCommInfo() {
//		portNo		= 1;
//		baudRate		= CBR_9600;
//		byteSize		= 8;
//		parity		= NOPARITY;
//		stopBits		= ONESTOPBIT;
//	}
//
//	_CCommInfo(_CCommInfo& commInfo) {
//		portNo		= commInfo.portNo;
//		baudRate		= commInfo.baudRate;
//		byteSize		= commInfo.byteSize;
//		parity		= commInfo.parity;
//		stopBits		= commInfo.stopBits;
//	}
//
//	CString GetInfoStr() {
//		CString stopBitsStr[]  = { _T("1"), _T("1.5"), _T("2") };
//		CString infoStr = _T("");
//		if (portNo > -1)
//		{
//			infoStr.Format(_T("COM%d/%dbps/%d/%d/%s"), portNo, baudRate, byteSize, parity, stopBitsStr[stopBits]);
//		}
//		return infoStr;
//	}
//};


class _GAUS_LIB CGxCommSerial 
{
public:
	CGxCommSerial();
	~CGxCommSerial();

	//--------- 환경 변수 -----------------------------------------//
	int			m_nTag;					// Tag
	HANDLE		m_hComm;					// 통신 포트 파일 핸들
	CString		m_sPortName;			// 포트 이름 (COM1 ..)
	BOOL			m_bConnected;			// 포트가 열렸는지 유무를 나타냄.
	OVERLAPPED	m_osRead, m_osWrite;	// 포트 파일 Overlapped structure
	HANDLE		m_hThreadWatchComm;		// Watch함수 Thread 핸들.
	WORD			m_wPortID;				// WM_COMM_READ와 함께 보내는 인수.

//	BYTE			m_byCommBuffer[GxCommMAX_BUFF];		// 통신용 데이터 버퍼
	BYTE			m_byRcvBuffer[GxCommMAX_BUFF];		// 수신데이터 버퍼
	int			m_nRcvBufferIdx;							// 수신데이터 버퍼 Index

	BOOL			m_bOneByteReadMode;		// 1Byte씩 읽어서 Event를 호출할 것인가 ?

	//--------- 외부 사용 함수 ------------------------------------//
	//d BOOL	OpenPort(CString strPortName, DWORD dwBaud, BYTE byData, BYTE byStop, BYTE byParity );//포트 열기 
	BOOL	OpenPort(int nPortNo, DWORD dwBaud, BYTE byData, BYTE byStop, BYTE byParity);
	void	ClosePort();				//포트 닫기
	DWORD	WriteComm(BYTE *pBuff, DWORD nToWrite);	//포트에 데이터 쓰기

	//--------- 내부 사용 함수 ------------------------------------//
	DWORD	ReadComm(BYTE *pBuff, DWORD nToRead);		//포트에서 데이터 읽어오기

	//--------- Thread 처리 ---------------------------------------//
	typedef TCallbackDefine< CGxCommSerial > TmyCallbackDefine;
	CGxMemberThread	m_thrCommWatch;					// Thread member
	void CommSerialThreadWatchComm(void *pData);		// 통신 모니터링 Thread 수행 함수


	// 수신 이벤트 관련
	// Callback 함수 
	CGxCallback*	m_pRcvFunction;				// 수신데이터를 처리하는 Callback함수

	void SetRcvFunction(CGxCallback* p);		// 수신 처리 함수 치환
	void OnReceive(int nSize);						// 데이터 수신 알림


	CString m_sName;					// 어떠한 통신인가 ?
	HWND	m_hEventDest;			// 수신 이벤트를 보낼 핸들
	UINT	m_nEventMsg;			// 어떠한 이벤트로 보낼것인가 ?
	void SetEventHandle(CString sName, HWND hHandle, UINT nMsg);			// 핸들과 이벤트 종류 지정

};

// Thread로 사용할 함수 
//DWORD	CommSerialThreadWatchComm(CGxCommSerial* pComm);
