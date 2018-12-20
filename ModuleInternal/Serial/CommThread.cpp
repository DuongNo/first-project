// CommThread.cpp: implementation of the CCommThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommThread.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//--- 클래스 생성자
CCommThread::CCommThread()
{
	m_bConnected = FALSE;
	m_bRecvDone =FALSE;
}

CCommThread::~CCommThread()
{

}


BOOL CCommThread::OpenPort(int nPortNo, DWORD dwBaud, BYTE byData, BYTE byStop, BYTE byParity )
{

	// Local 변수.
	COMMTIMEOUTS	timeouts;
	DCB				dcb;
	DWORD			dwThreadID;

	// overlapped structure 변수 초기화.
	m_osRead.Offset = 0;
	m_osRead.OffsetHigh = 0;
	//--> Read 이벤트 생성에 실패..
	if ( !(m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)) ) 	
	{
		return FALSE;
	}


	m_osWrite.Offset = 0;
	m_osWrite.OffsetHigh = 0;
	//--> Write 이벤트 생성에 실패..
	if (! (m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))
	{
		return FALSE;
	}

	//--> 포트명 저장..
	//m_sPortName = strPortName;
	m_sPortName.Format(_T("\\\\.\\COM%d"), nPortNo);
	//--> 실제적인...RS 232 포트 열기..
	m_hComm = CreateFile( m_sPortName, 
		GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 
		NULL);
	

	//--> 포트 열기에 실해하면..
	if (m_hComm == (HANDLE) -1)
	{
		//AfxMessageBox("fail Port ofen");
		return FALSE;
	}


	//===== 포트 상태 설정. =====

	// EV_RXCHAR event 설정...데이터가 들어오면.. 수신 이벤트가 발생하게끔..
	//SetCommMask( m_hComm, EV_RXCHAR);	
	SetCommMask( m_hComm, ETX);	

	// InQueue, OutQueue 크기 설정.
	SetupComm( m_hComm, SERIAL_MAX_BUFFER, SERIAL_MAX_BUFFER);	

	// 포트 비우기.
	PurgeComm( m_hComm,					
		PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);

	// timeout 설정.
	timeouts.ReadIntervalTimeout = 0xFFFFFFFF;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 0;

	timeouts.WriteTotalTimeoutMultiplier = 2*CBR_9600 / dwBaud;
	timeouts.WriteTotalTimeoutConstant = 0;

	SetCommTimeouts( m_hComm, &timeouts);

	// dcb 설정.... 포트의 실제적인..제어를 담당하는 DCB 구조체값 셋팅..
	dcb.DCBlength = sizeof(DCB);

	//--> 현재 설정된 값 중에서..
	GetCommState( m_hComm, &dcb);	

	//--> 보드레이트를 바꾸고..
	dcb.BaudRate = dwBaud;


	//--> Data 8 Bit
	dcb.ByteSize = byData;

	//--> Noparity
	dcb.Parity = byParity;

	//--> 1 Stop Bit
	dcb.StopBits = byStop;


	//--> 포트를 재..설정값으로.. 설정해보고..
	if( !SetCommState( m_hComm, &dcb) )	
	{
		return FALSE;
	}


	// 포트 감시 쓰레드 생성.
	m_bConnected = TRUE;

	//--> 포트 감시 쓰레드 생성.
	m_hThreadWatchComm = CreateThread( NULL, 0, 
		(LPTHREAD_START_ROUTINE)ThreadWatchComm, 
		this, 0, &dwThreadID);

	//--> 쓰레드 생성에 실패하면..
	if (! m_hThreadWatchComm)
	{
		//--> 열린 포트를 닫고..
		ClosePort();
		return FALSE;
	}
	return TRUE;
}
	

BOOL CCommThread::InitCommPort(int nPortNo, 
							   DWORD dwBaud, 
							   BYTE byData, 
							   BYTE byStop, 
							   BYTE byParity)
{
	if (m_bConnected) {
		ClosePort();
	}

	// Local 변수.
	COMMTIMEOUTS	timeouts;
	DCB				dcb;
	DWORD			dwThreadID;

	m_bConnected = FALSE;

	m_osRead.Offset = 0;
	m_osRead.OffsetHigh = 0;
	if (!(m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL))) {
		return FALSE;
	}

	m_osWrite.Offset = 0;
	m_osWrite.OffsetHigh = 0;
	if (!(m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL))) {
		CloseHandle(m_osRead.hEvent);
		return FALSE;
	}
	
	m_sPortName.Format(_T("\\\\.\\COM%d"), nPortNo);

	m_hComm = CreateFile(m_sPortName, 
						 GENERIC_READ | GENERIC_WRITE, 0, NULL,
						 OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 
						 NULL);
	if (m_hComm == (HANDLE) - 1) {
		return FALSE;
	}

	// EV_RXCHAR event 설정
	SetCommMask(m_hComm, EV_RXCHAR);	
	//SetCommMask( m_hComm, ETX);	

	// InQueue, OutQueue 크기 설정.
	SetupComm(m_hComm, 4096, 4096);	

	// 포트 비우기.
	PurgeComm(m_hComm,	PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);

	// timeout 설정.
	//timeouts.ReadIntervalTimeout = 0xFFFFFFFF;
	timeouts.ReadIntervalTimeout = 20;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 0;

	timeouts.WriteTotalTimeoutMultiplier = 2*CBR_9600 / dwBaud;
	timeouts.WriteTotalTimeoutConstant = 0;
	
	SetCommTimeouts(m_hComm, &timeouts);

	
	// dcb 설정.... 포트의 실제적인..제어를 담당하는 DCB 구조체값 셋팅..
	dcb.DCBlength = sizeof(DCB);

	//--> 현재 설정된 값 중에서..
	GetCommState( m_hComm, &dcb);
	dcb.BaudRate = dwBaud;		//Baud Rate
	dcb.ByteSize = byData;		//Data Bit
	dcb.StopBits = byStop;		//Stop Bit
	dcb.Parity   = byParity;	//Parity
	dcb.fDtrControl = DTR_CONTROL_DISABLE; // flow control none
	dcb.EvtChar = CR;

	//--> 포트를 재..설정값으로.. 설정해보고..
	if (!SetCommState( m_hComm, &dcb)) {
		ClosePort();
		return FALSE;
	}

	//--> 포트 감시 쓰레드 생성.
	m_hThreadWatchComm = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadWatchComm, this, 0, &dwThreadID);
	if (!m_hThreadWatchComm) {
		ClosePort();
		return FALSE;
	}

	// 포트 감시 쓰레드 생성.
	m_bConnected = TRUE;
	
	return TRUE;
}

void CCommThread::ClosePort()
{
	m_bConnected = FALSE;

	SetCommMask(m_hComm, 0);
	
	PurgeComm(m_hComm,	PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);
	
	CloseHandle(m_hComm);
}

DWORD CCommThread::WriteComm(BYTE *pBuff, DWORD nToWrite)
{
	DWORD	dwWritten, dwError, dwErrorFlags;
	COMSTAT	comstat;

	//--> 포트가 연결되지 않은 상태이면..
	if( !m_bConnected )		
	{
		return 0;
	}


	//--> 인자로 들어온 버퍼의 내용을 nToWrite 만큼 쓰고.. 쓴 갯수를.,dwWrite 에 넘김.
	if( !WriteFile( m_hComm, pBuff, nToWrite, &dwWritten, &m_osWrite))
	{
		//--> 아직 전송할 문자가 남았을 경우..
		if (GetLastError() == ERROR_IO_PENDING)
		{
			// 읽을 문자가 남아 있거나 전송할 문자가 남아 있을 경우 Overapped IO의
			// 특성에 따라 ERROR_IO_PENDING 에러 메시지가 전달된다.
			//timeouts에 정해준 시간만큼 기다려준다.
			while (! GetOverlappedResult( m_hComm, &m_osWrite, &dwWritten, TRUE))
			{
				dwError = GetLastError();
				if (dwError != ERROR_IO_INCOMPLETE)
				{
					ClearCommError( m_hComm, &dwErrorFlags, &comstat);
					break;
				}
			}
		}
		else
		{
			dwWritten = 0;
			ClearCommError( m_hComm, &dwErrorFlags, &comstat);
		}
	}

	//--> 실제 포트로 쓰여진 갯수를 리턴..
	return dwWritten;
}

DWORD CCommThread::ReadComm(BYTE *pBuff, DWORD nToRead)
{
	DWORD	dwRead, dwError, dwErrorFlags;
	COMSTAT	comstat;

	//--- system queue에 도착한 byte수만 미리 읽는다.
	ClearCommError( m_hComm, &dwErrorFlags, &comstat);

	//--> 시스템 큐에서 읽을 거리가 있으면..
	dwRead = comstat.cbInQue;
	
	if (dwRead > 0)	{
		if (!ReadFile( m_hComm, pBuff, nToRead, &dwRead, &m_osRead)) {
			//--> 읽을 거리가 남았으면..
			if (GetLastError() == ERROR_IO_PENDING)	{
				//--------- timeouts에 정해준 시간만큼 기다려준다.
				while (!GetOverlappedResult( m_hComm, &m_osRead, &dwRead, TRUE)) {
					dwError = GetLastError();
					if (dwError != ERROR_IO_INCOMPLETE)	{
						ClearCommError( m_hComm, &dwErrorFlags, &comstat);
						break;
					}
				}
			}
			else {
				dwRead = 0;
				ClearCommError( m_hComm, &dwErrorFlags, &comstat);
			}
		}
	}

	return dwRead;
}

// void CCommThread::ProcessRcvData(BYTE *byBuf, int nSize)
// {
// 	if (!nSize)	return;
// 
// 	CString	strRecvData(byBuf);
// 
// 	
// 	m_pListener->ReceivedData(strRecvData);
// }

// 포트를 감시하고, 읽힌 내용이 있으면
// m_ReadData에 저장한 뒤에 MainWnd에 메시지를 보내어 Buffer의 내용을
// 읽어가라고 신고한다.

DWORD	ThreadWatchComm(CCommThread* pComm)
{
	DWORD           dwEvent;
	OVERLAPPED      os;
	BOOL            bOk = TRUE;
	BYTE            buff[SERIAL_MAX_BUFFER];      // 읽기 버퍼
	DWORD           dwRead =0;  // 읽은 바이트수.

	// Event, OS 설정.
	memset( &os, 0, sizeof(OVERLAPPED));

	//--> 이벤트 설정..
	if( !(os.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL)) )
	{
		bOk = FALSE;
	}

	//--> 이벤트 마스크..
	if( !SetCommMask( pComm->m_hComm, EV_RXCHAR) )
	{
		bOk = FALSE;
	}

	//--> 이벤트나..마스크 설정에 실패함..
	if( !bOk )
	{
		return FALSE;
	}

	while (pComm ->m_bConnected)//포트가 연결되면 무한 루프에 들어감
	{
		dwEvent = 0;

		// 포트에 읽을 거리가 올때까지 기다린다.
		WaitCommEvent( pComm->m_hComm, &dwEvent, NULL);

		//--> 데이터가 수신되었다는 메세지가 발생하면..
		if ((dwEvent & EV_RXCHAR) == EV_RXCHAR)
		{
			// 포트에서 읽을 수 있는 만큼 읽는다.
			//--> buff 에 받아놓고..
			do
			{
				dwRead = pComm->ReadComm( buff, SERIAL_MAX_BUFFER); //들어온 데이터 읽어 오기 
				if(SERIAL_MAX_BUFFER - pComm->m_QueueRead.GetSize() > (int)dwRead)
				{
					for( WORD i = 0; i < dwRead; i++ )
					{
						pComm->m_QueueRead.PutByte(buff[i]);//큐 버퍼에  들어온 데이터 넣기
					}
				}
				else
				{
					pComm->m_QueueRead.Clear();
				}
			}while(dwRead);
			//			::PostMessage(hCommWnd, WM_COMM_READ, 0, 0 );//데이터가 들어왔다는 메시지를 보냄
		}
		Sleep(50);	// 받은 데이터를 화면에 보여줄 시간을 벌기 위해.
		// 데이터를 연속으로 받으면 cpu점유율이 100%가 되어 화면에 뿌려주는 작업이 잘 안되고. 결과적으로 
		// 큐 버퍼에 데이터가 쌓이게 됨
#ifndef _DEBUG		
		pComm->m_bRecvDone = TRUE;
		int nBufferSize = pComm->m_QueueRead.GetSize();
		BYTE            buf[SERIAL_MAX_BUFFER]; 
		memset(buf, 0x00, SERIAL_MAX_BUFFER);

		

		pComm->m_QueueRead.GetData(buf, pComm->m_QueueRead.GetSize());
		if(nBufferSize)
		{		
			
			CString strGetData, strGetDatas;
			wchar_t wszMsg[SERIAL_MAX_BUFFER];
			MultiByteToWideChar(CP_ACP, 0, (char*)buf, nBufferSize, wszMsg, SERIAL_MAX_BUFFER);

			strGetData.Format(_T("%s"), wszMsg);
			strGetDatas = strGetData.Mid(0, nBufferSize);

			SYSTEMTIME	time;
			CString strLogHeader;
			GetLocalTime(&time);
			strLogHeader.Format(_T("%02u:%02u:%02u:%03u\t__ThreadWatchComm\n"), time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
			TRACE(strLogHeader);
			
			SERIALEvent* pMsg = new SERIALEvent;
			pMsg->message = pComm->m_serialMode;
			pMsg->pParam = (LPARAM)buf;
			pMsg->nParam1 = nBufferSize;
			::PostMessage(GetMainWnd(), WM_SERIAL, NULL, (LPARAM)pMsg);		
			
			
		}
	

#endif
	}

	CloseHandle( os.hEvent);

	//--> 쓰레드 종료가 되겠죠?
	pComm->m_hThreadWatchComm = NULL;

	return TRUE;

}