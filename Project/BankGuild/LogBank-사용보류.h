#pragma once

enum
{
	LogType_Buffer,	// Buffer�� �� ���� ���
	LogType_NoWait,	// ������� �ʰ� �ٷ� ���
	LogType_Max
};

enum
{
	LOG_SYSTEM,			// Program ���� ���� ���� �α�
	LOG_SYSTEM_ERROR,	// ���α׷������� �м� �ʿ��� �͸� ��Ƴ��� �α�
 	LOG_SEQUENCE,		// Seq ���� ��Ȳ ��� �α�
	LOG_COMM,			// �˻�� ��� ���� ��� �α�
	LOG_OPERATION,		// ���� ��ư ���� ��� �α�
	LOG_ERR,			// ������ ��� �α�
	LOG_MOTOR_ERROR,
	LOG_MP2100,			// MP2100 Error Code
	LOG_NMC,			// NMC Error Code
	LOG_TACKTIME,		//JH-PARK 2014.1.20
	LOG_CELL_FLOW,		// Cell �帧 ���� ���� ����

	LOG_MAX					// Log ���� �� ����
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
	
	int m_nHourOffset;		// ��¥������ 00�ð� �ƴ϶� �ٸ� �ð����� �ٲٰ� ���� ��

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
	void SetPartition(BOOL bUse);	// �ð���� ���ϸ� �з�

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
	void WriteNowAll();	// ���� ���ۿ� �׿��ִ� �͵��� ��� ���� �Լ�
	void WriteNowAt(int nLogID);	// ���� ���ۿ� �׿��ִ� �͵��� ��� ���� �Լ�
};
