//////////////////////////////////////////////////////////////////////////
// _MBCS, _UNICODE에 따른 File Open 방식 차등 적용 [8/7/2015 OSC]
// Format 형식 누락된거 수정 [11/16/2014 OSC]
//////////////////////////////////////////////////////////////////////////

#pragma once

class CLogUnit
{
public:

	enum WRITE_TYPE
	{
		WriteType_Buffer,	// Buffer가 다 차면 기록
		WriteType_NoWait,	// 대기하지 않고 바로 기록
		WriteType_Max
	};

	CLogUnit(void);
	CLogUnit(WRITE_TYPE logType, CString strFileName, CString strPath, int nOffsetHour, int nOffsetMin, 
		BOOL bAddDay = FALSE, BOOL bAddTime = TRUE, BOOL bPartition = FALSE);
	~CLogUnit(void);

private:
	HANDLE m_hLogEvent;

	WRITE_TYPE m_WriteType;
	int m_nBufferSize;
	CString m_strBuf;
	TCHAR* m_pBuf;

	int m_nWriteStart;
	CString m_strFolderPath;
	CString m_strSubFolderName;
	CString m_strFileName;
	int m_nOffsetHour;
	int m_nOffsetMin;
	

	BOOL m_bAddTime;
	BOOL m_bAddDay;
	BOOL m_bPartition;

	void Write(LPCTSTR szLogText);

public:
	void SetType(WRITE_TYPE type);
	void SetFolderPath(CString strFolderPath);
	void SetSubFolderName(CString strFolderName);	// 날짜폴더 밑에 폴더 하나 더 만들고 싶을 때...
	void SetFileName(CString szFileName);
	void SetBufferSize(UINT nBufferSize);
	void SetOffsetHour(int nOffsetHour);
	void SetOffsetMin(int nOffsetMin);
	void SetAddTime(BOOL bAdd);
	void SetAddDay(BOOL bAdd);
	void SetPartition(BOOL bUse);	// 시간대로 파일명 분류

	void AddBuf(LPCTSTR fmt, ...);
	void WriteBuf(BOOL bOverFlow = FALSE);
	BOOL GetFullPath(CString &strPath, SYSTEMTIME &time);
};
