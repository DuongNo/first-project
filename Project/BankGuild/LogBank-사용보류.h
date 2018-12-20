#pragma once

enum
{
	LogType_Buffer,	// Buffer가 다 차면 기록
	LogType_NoWait,	// 대기하지 않고 바로 기록
	LogType_Max
};

enum
{
	LOG_SYSTEM,			// Program 내부 구동 관련 로그
	LOG_SYSTEM_ERROR,	// 프로그램적으로 분석 필요한 것만 모아놓은 로그
 	LOG_SEQUENCE,		// Seq 변경 현황 기록 로그
	LOG_COMM,			// 검사부 통신 관련 기록 로그
	LOG_OPERATION,		// 유저 버튼 조작 기록 로그
	LOG_ERR,			// 에러명 기록 로그
	LOG_MOTOR_ERROR,
	LOG_MP2100,			// MP2100 Error Code
	LOG_NMC,			// NMC Error Code
	LOG_TACKTIME,		//JH-PARK 2014.1.20
	LOG_CELL_FLOW,		// Cell 흐름 추적 위해 만듬

	LOG_MAX					// Log 폴더 총 갯수
};

class CLogUnit
{
public:
	CLogUnit(void);
	~CLogUnit(void);

private:
	HANDLE m_hLogEvent;

	int m_nWriteType;
	int m_nBufferSize;
	CString m_strBuf;
	//char* m_pBuf;
	TCHAR* m_pBuf;

	int m_nWriteStart;
	CString m_strFolderPath;
	CString m_strFileName;
	
	int m_nHourOffset;		// 날짜변경을 00시가 아니라 다른 시간으로 바꾸고 싶을 때

	BOOL m_bAddTime;
	BOOL m_bAddDay;
	BOOL m_bPartition;

	void Write(LPCTSTR szLogText);


public:
	void SetType(int nType);
	void SetHourOffset(int nHourOffset);
	void SetFolderPath(CString strFolderPath);
	void SetFileName(CString szFileName);
	void SetBufferSize(UINT nBufferSize);
	void SetAddTime(BOOL bAdd);
	void SetAddDay(BOOL bAdd);
	void SetPartition(BOOL bUse);	// 시간대로 파일명 분류

	void AddBuf(LPCTSTR szLogText);
	void WriteBuf(BOOL bOverFlow = FALSE);
	BOOL GetFullPath(CString &strPath, SYSTEMTIME &time);
};

class CLogBank
{
public:
	CLogBank(void);
	~CLogBank(void);

private:
	CLogUnit m_Log[LOG_MAX];

public:
	void Initialize();
	void SetHourOffset(int nHourOffset);
	void AddBuf(int nLogID, LPCTSTR fmt, ...);
	BOOL GetFullPath(int nLogID, CString &strPath);
	void WriteNowAll();	// 현재 버퍼에 쌓여있는 것들을 모두 적는 함수
	void WriteNowAt(int nLogID);	// 현재 버퍼에 쌓여있는 것들을 모두 적는 함수
};
