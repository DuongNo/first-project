// CommThread.h: interface for the CCommThread class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "CQueueSerial.h"

struct CCommInfo 
{
	long portNo;
	long baudRate;
	long stopBits;
	long parity;
	long byteSize;

	CCommInfo() {
		portNo		= 1;
		baudRate	= CBR_9600;
		byteSize	= 8;
		parity		= NOPARITY;
		stopBits	= ONESTOPBIT;
	}

	CCommInfo(CCommInfo& commInfo) {
		portNo		= commInfo.portNo;
		baudRate	= commInfo.baudRate;
		byteSize	= commInfo.byteSize;
		parity		= commInfo.parity;
		stopBits	= commInfo.stopBits;
	}

	CString GetInfoStr() {
		CString stopBitsStr[]  = { _T("1"), _T("1.5"), _T("2") };
		CString infoStr = _T("");
		if (portNo > -1)
		{
			infoStr.Format(_T("COM%d/%dbps/%d/%d/%s"), portNo, baudRate, byteSize, parity, stopBitsStr[stopBits]);
		}
		return infoStr;
	}
};


class CCommListener
{
public:
	virtual void ReceivedData(CString strData) = 0;
};

class CCommThread 
{
	CCommListener* m_pListener;
public:
	CCommThread();
	~CCommThread();
	//--------- 환경 변수 -----------------------------------------//
	HANDLE		m_hComm;				// 통신 포트 파일 핸들
	CString		m_sPortName;			// 포트 이름 (COM1 ..)
	CString		m_sPortNickName;			// 포트 닉네임
	BOOL		m_bConnected;			// 포트가 열렸는지 유무를 나타냄.
	OVERLAPPED	m_osRead, m_osWrite;	// 포트 파일 Overlapped structure
	HANDLE		m_hThreadWatchComm;		// Watch함수 Thread 핸들.
	WORD		m_wPortID;				// WM_COMM_READ와 함께 보내는 인수.
	CQueueSerial      m_QueueRead;			//큐버퍼
	//--------- 외부 사용 함수 ------------------------------------//
	BOOL InitCommPort(int nPortNo, DWORD dwBaud, BYTE byData, BYTE byStop, BYTE byParity);
	BOOL	OpenPort(int nPort, DWORD dwBaud, BYTE byData, BYTE byStop, BYTE byParity );//포트 열기 
	void	ClosePort();				//포트 닫기
	DWORD	WriteComm(BYTE *pBuff, DWORD nToWrite);//포트에 데이터 쓰기

	//--------- 내부 사용 함수 ------------------------------------//
	DWORD	ReadComm(BYTE *pBuff, DWORD nToRead);//포트에서 데이터 읽어오기
	SERIAL_MODE m_serialMode;
	BOOL m_bRecvDone;
	
// 	CString byIndexComPort(int xPort);// 포트이름 받기 
// 	DWORD byIndexBaud(int xBaud);// 전송률 받기
// 	BYTE byIndexData(int xData);//데이터 비트 받기
// 	BYTE byIndexStop(int xStop);// 스톱비트 받기 
// 	BYTE byIndexParity(int xParity);// 페리티 받기


// 	void SetListener(CCommListener* pListener) { m_pListener = pListener; };
// 	void ProcessRcvData(BYTE *byBuf, int nSize);


//	BOOL		OpenPort(CString strPortName, DWORD dwBaud, WORD wParam);
};

// Thread로 사용할 함수 
DWORD	ThreadWatchComm(CCommThread* pComm);
