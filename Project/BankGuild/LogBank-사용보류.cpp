#include "StdAfx.h"
#include "LogBank.h"
#include "Etc/FileSupport.h"

CLogUnit::CLogUnit(void)
{
	m_hLogEvent = CreateEvent(NULL,FALSE,TRUE,_T("Log"));
	m_nWriteType = LogType_Buffer;
	m_pBuf = NULL;
	m_nWriteStart = 0;
	m_strBuf = _T("");
	m_nBufferSize = 4096;
	SetBufferSize(m_nBufferSize);
	m_strFolderPath = _T("");
	m_strFileName = _T("");
	m_bAddTime = TRUE;
	m_bAddDay = TRUE;
	m_bPartition = FALSE;
	m_nHourOffset = 0;
}

CLogUnit::~CLogUnit(void)
{
	// ����� ���ۿ� Data�� ���������� ��� �� ����
	WriteBuf();
	CloseHandle(m_hLogEvent);
}

void CLogUnit::SetType( int nType )
{
	m_nWriteType = nType;
}

void CLogUnit::SetHourOffset( int nHourOffset )
{
	m_nHourOffset = nHourOffset;
}

void CLogUnit::SetFolderPath( CString strFolderPath )
{
	m_strFolderPath = strFolderPath;
}

void CLogUnit::SetFileName( CString szFileName )
{
	m_strFileName = szFileName;
}

void CLogUnit::SetBufferSize( UINT nBufferSize )
{
	// new�� delete�� �����ϸ� �������� CString�� ����ؼ� �˾Ƽ� �����ϰ� �����.
	m_nBufferSize = nBufferSize;
	m_strBuf = _T("");
	m_strBuf.ReleaseBuffer();
	m_pBuf = m_strBuf.GetBuffer(nBufferSize);
	m_nWriteStart = 0;
}

void CLogUnit::SetAddTime( BOOL bAdd )
{
	m_bAddTime = bAdd;
}

void CLogUnit::SetAddDay( BOOL bAdd )
{
	m_bAddDay = bAdd;
}

void CLogUnit::SetPartition( BOOL bUse )
{
	m_bPartition = bUse;
}

void CLogUnit::AddBuf( LPCTSTR szLogText )
{
	if( WaitForSingleObject(m_hLogEvent, 10000) != WAIT_OBJECT_0)
	{
		TRACE(_T("AddBuf Event Error"));
		return;
	}

	if(_tcslen(szLogText) < 1)
	{
		SetEvent(m_hLogEvent);
		return;
	}
	CString strLogText = _T("");

	if(m_bAddTime)
	{
		if (m_bAddDay)
		{
			SYSTEMTIME	time;
			GetLocalTime(&time);
			strLogText.Format(_T("%04u-%02u-%02u\t%02u:%02u:%02u:%03u\t%s\n"),time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, szLogText);
			szLogText = strLogText.GetString();
		}
		else
		{
			SYSTEMTIME	time;
			GetLocalTime(&time);
			strLogText.Format(_T("%02u:%02u:%02u:%03u\t%s\n"), time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, szLogText);
			szLogText = strLogText.GetString();
		}
	}
	else
	{
		strLogText.Format(_T("%s\n"), szLogText);
		szLogText = strLogText.GetString();
	}

	// �ٷ� ���� �Ÿ� ���⼭ �ٷ� ���´�.
	if(m_nWriteType == LogType_NoWait)
	{
		Write(szLogText);
	}
	else
	{
		int nLength = _tcslen(szLogText);
		ASSERT(m_nBufferSize >= nLength);

		// Buffer�� �� á�� ��� WriteType�� ������� ������ ���� Buffer�� ����.
		if(m_nWriteStart + nLength > m_nBufferSize)
		{
			WriteBuf(TRUE);
		}

		memcpy(m_pBuf+m_nWriteStart, szLogText , nLength+1);
// 		_tcscpy_s(m_pBuf+m_nWriteStart, nLength, _szLogText);
		m_nWriteStart += nLength;
	}

	SetEvent(m_hLogEvent);
}

void CLogUnit::WriteBuf(BOOL bOverFlow /*= FALSE*/)
{
	if(_tcslen(m_pBuf) < 1 || _tcslen(m_strFolderPath) < 1 || _tcslen(m_strFileName) < 1)
		return SetBufferSize(m_nBufferSize);

	// Directory Check
	SYSTEMTIME time;
	CString strFolderPath;
	if(GetFullPath(strFolderPath, time) == FALSE)
		return;

	// ���� �� ���� ����
	m_strBuf.ReleaseBuffer();

	// File�� ���
	FILE *stream;
	CString strFilePath ;
	if(m_bPartition)
	{
		int nExtIndex = m_strFileName.ReverseFind('.');
		strFilePath.Format(_T("%s\\%s_%02u%s"), strFolderPath, m_strFileName.Left(nExtIndex), time.wHour, m_strFileName.Mid(nExtIndex));
	}
	else
		strFilePath.Format(_T("%s\\%s"), strFolderPath, m_strFileName);

// 	if(_tfopen_s( &stream, strFilePath, _T("a+t") ) == 0 )
// 	{
// 		_ftprintf(stream, _T("%s"), m_strBuf);
// 
// 		// WriteType_Buffer�� �ƴ� ���� Buffer �뷮 �ʰ��� ���� ���� ��� ���Ͽ� Error ���
// 		if(m_nWriteType != LogType_Buffer && bOverFlow == TRUE)
// 		{
// 			m_strBuf = _T("\nBuffer Overflow!\n");
// 			_ftprintf(stream, _T("%s"), m_strBuf);
// 		}
// 		fclose(stream);
// 	}
// 	else
// 	{
// 		TRACE(_T("Log File open Fail! (%s)"), strFilePath);
// 	}

	// ó�� ���� �� �����ڵ�� ���� ������ ���� ���� ��� ������.
	// �׸��� �����ڵ� ���� �����Ϸ��� w�ιۿ� ���� ���Ѵ� -_-; [4/9/2013 OSC]
	if( CFileSupport::FileCheck(strFilePath) )
	{
		if(_tfopen_s( &stream, strFilePath, _T("a+t, ccs=UTF-8") ) != 0 )
		{
			TRACE(_T("Log File open Fail! (%s)"), strFilePath);
		}
	}
	else
	{
		if(_tfopen_s( &stream, strFilePath, _T("w, ccs=UTF-8") ) != 0 )
		{
			TRACE(_T("Log File open Fail! (%s)"), strFilePath);
		}
	}

	// WriteType_Buffer�� �ƴ� ���� Buffer �뷮 �ʰ��� ���� ���� ��� ���Ͽ� Error ���
	if(m_nWriteType != LogType_Buffer && bOverFlow == TRUE)
	{
		m_strBuf = _T("\nBuffer Overflow!\n");
		_ftprintf(stream, _T("%s"), m_strBuf);
	}
	fclose(stream);


	// ���� �Ҵ�
	SetBufferSize(m_nBufferSize);
}

void CLogUnit::Write(LPCTSTR szLogText)
{
	if(m_nWriteType != LogType_NoWait)
		return;
	//if(_tcslen((LPCSTR)szLogText) < 1 || _tcslen(m_strFolderPath) < 1 || _tcslen(m_strFileName) < 1)
	if(_tcslen(szLogText) < 1 || _tcslen(m_strFolderPath) < 1 || _tcslen(m_strFileName) < 1)
		return;

	// Directory Check
	SYSTEMTIME time;
	CString strFolderPath;
	if(GetFullPath(strFolderPath, time) == FALSE)
		return;

	// File�� ���
	FILE *stream;
	CString strFilePath ;
	if(m_bPartition)
	{
		int nExtIndex = m_strFileName.ReverseFind('.');
		strFilePath.Format(_T("%s\\%s_%02u%s"), strFolderPath, m_strFileName.Left(nExtIndex), time.wHour, m_strFileName.Mid(nExtIndex));
	}
	else
		strFilePath.Format(_T("%s\\%s"), strFolderPath, m_strFileName);

// 	if(_tfopen_s( &stream, strFilePath, _T("a+t") ) == 0 )
// 	{
// 		_ftprintf(stream, _T("%s"), szLogText);
// 		TRACE(_T("%d. %s\n"), stream, szLogText);
// 		fclose(stream);
// 	}
	// ó�� ���� �� �����ڵ�� ���� ������ ���� ���� ��� ������.
	// �׸��� �����ڵ� ���� �����Ϸ��� w�ιۿ� ���� ���Ѵ� -_-; [4/9/2013 OSC]
	if( CFileSupport::FileCheck(strFilePath) )
	{
		if(_tfopen_s( &stream, strFilePath, _T("a+t, ccs=UTF-8") ) != 0 )
		{
			TRACE(_T("Log File open Fail! (%s)"), strFilePath);
			return;
		}
	}
	else
	{
		if(_tfopen_s( &stream, strFilePath, _T("w, ccs=UTF-8") ) != 0 )
		{
			TRACE(_T("Log File open Fail! (%s)"), strFilePath);
			return;
		}
	}

	_ftprintf(stream, _T("%s"), szLogText);
// 	TRACE(_T("%d. %s\n"), stream, szLogText);
	fclose(stream);
}

BOOL CLogUnit::GetFullPath( CString &strPath, SYSTEMTIME &time )
{
	GetLocalTime(&time);

	if(time.wHour < m_nHourOffset)
	{
		CTimeSpan timeOffset(1, 0, 0, 0);
		CTime tmYesterday = CTime::GetCurrentTime() - timeOffset; 

		time.wYear=tmYesterday.GetYear(); 
		time.wMonth=tmYesterday.GetMonth(); 
		time.wDay=tmYesterday.GetDay(); 
	}

	CString strFolderPath;
	strFolderPath.Format(_T("%s\\%04u-%02u-%02u"), m_strFolderPath, time.wYear, time.wMonth, time.wDay);
	if(CFileSupport::DirectoryCheck(strFolderPath) == FALSE)
	{
		if(CFileSupport::CreateDirectory(strFolderPath) == FALSE)
			return FALSE;
	}
	strPath = strFolderPath;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// CLogBank
//////////////////////////////////////////////////////////////////////////
CLogBank::CLogBank( void )
{
	Initialize();
}

CLogBank::~CLogBank(void)
{
}

void CLogBank::AddBuf( int nLogID, LPCTSTR fmt, ... )
{
	CString str;
	va_list argList;
	va_start( argList, fmt );
	str.FormatV( fmt, argList );
	va_end( argList );

	m_Log[nLogID].AddBuf(str);
}

BOOL CLogBank::GetFullPath( int nLogID, CString &strPath )
{
	SYSTEMTIME time;
	return m_Log[nLogID].GetFullPath(strPath, time);
}

void CLogBank::WriteNowAll()
{
	for(int i = 0; i < LOG_MAX; i++)
		m_Log[i].WriteBuf();
}

void CLogBank::WriteNowAt( int nLogID )
{
	m_Log[nLogID].WriteBuf();
}

void CLogBank::Initialize()
{
	// 07�� �������� ��¥ ����
	SetHourOffset(07);

	m_Log[LOG_SYSTEM].SetType(LogType_NoWait);
	m_Log[LOG_SYSTEM].SetFileName(_T("System.log"));
	m_Log[LOG_SYSTEM].SetFolderPath(_T("D:\\Log"));
	m_Log[LOG_SYSTEM].SetAddTime(TRUE);
	m_Log[LOG_SYSTEM].SetAddDay(FALSE);

	m_Log[LOG_SYSTEM_ERROR].SetType(LogType_NoWait);
	m_Log[LOG_SYSTEM_ERROR].SetFileName(_T("SystemError.log"));
	m_Log[LOG_SYSTEM_ERROR].SetFolderPath(_T("D:\\Log"));
	m_Log[LOG_SYSTEM_ERROR].SetAddTime(TRUE);
	m_Log[LOG_SYSTEM_ERROR].SetAddDay(FALSE);

	m_Log[LOG_SEQUENCE].SetType(LogType_NoWait);
	m_Log[LOG_SEQUENCE].SetFileName(_T("Sequence.log"));
	m_Log[LOG_SEQUENCE].SetFolderPath(_T("D:\\Log"));
	m_Log[LOG_SEQUENCE].SetAddTime(TRUE);
	m_Log[LOG_SEQUENCE].SetAddDay(FALSE);
	m_Log[LOG_SEQUENCE].SetPartition(TRUE);

	m_Log[LOG_COMM].SetType(LogType_NoWait);
	m_Log[LOG_COMM].SetFileName(_T("Comm.log"));
	m_Log[LOG_COMM].SetFolderPath(_T("D:\\Log"));
	m_Log[LOG_COMM].SetAddTime(TRUE);
	m_Log[LOG_COMM].SetAddDay(FALSE);
	m_Log[LOG_COMM].SetPartition(TRUE);

	m_Log[LOG_OPERATION].SetType(LogType_NoWait);
	m_Log[LOG_OPERATION].SetFileName(_T("Operation.log"));
	m_Log[LOG_OPERATION].SetFolderPath(_T("D:\\Log"));
	m_Log[LOG_OPERATION].SetAddTime(TRUE);
	m_Log[LOG_OPERATION].SetAddDay(FALSE);

	m_Log[LOG_ERR].SetType(LogType_NoWait);
	m_Log[LOG_ERR].SetFileName(_T("ErrorType.log"));
	m_Log[LOG_ERR].SetFolderPath(_T("D:\\Log"));
	m_Log[LOG_ERR].SetAddTime(TRUE);
	m_Log[LOG_ERR].SetAddDay(FALSE);

	m_Log[LOG_MOTOR_ERROR].SetType(LogType_NoWait);
	m_Log[LOG_MOTOR_ERROR].SetFileName(_T("MotorError.log"));
	m_Log[LOG_MOTOR_ERROR].SetFolderPath(_T("D:\\Log"));
	m_Log[LOG_MOTOR_ERROR].SetAddTime(TRUE);
	m_Log[LOG_MOTOR_ERROR].SetAddDay(FALSE);

	m_Log[LOG_MP2100].SetType(LogType_NoWait);
	m_Log[LOG_MP2100].SetFileName(_T("MP2100.log"));
	m_Log[LOG_MP2100].SetFolderPath(_T("D:\\Log"));
	m_Log[LOG_MP2100].SetAddTime(TRUE);
	m_Log[LOG_MP2100].SetAddDay(FALSE);

	m_Log[LOG_NMC].SetType(LogType_NoWait);
	m_Log[LOG_NMC].SetFileName(_T("NMC.log"));
	m_Log[LOG_NMC].SetFolderPath(_T("D:\\Log"));
	m_Log[LOG_NMC].SetAddTime(TRUE);
	m_Log[LOG_NMC].SetAddDay(FALSE);

	//JH-PARK 2014.1.20
	m_Log[LOG_TACKTIME].SetType(LogType_NoWait);
	m_Log[LOG_TACKTIME].SetFileName(_T("TackTime.log"));
	m_Log[LOG_TACKTIME].SetFolderPath(_T("D:\\Log"));
	m_Log[LOG_TACKTIME].SetAddTime(TRUE);
	m_Log[LOG_TACKTIME].SetAddDay(FALSE);
	
	m_Log[LOG_CELL_FLOW].SetType(LogType_NoWait);
	m_Log[LOG_CELL_FLOW].SetFileName(_T("CellFlow.log"));
	m_Log[LOG_CELL_FLOW].SetFolderPath(_T("D:\\Log"));
	m_Log[LOG_CELL_FLOW].SetAddTime(TRUE);
	m_Log[LOG_CELL_FLOW].SetAddDay(FALSE);

}

void CLogBank::SetHourOffset( int nHourOffset )
{
	if( (nHourOffset >= 0) && (nHourOffset <= 23) )
	{
		for(int i = 0; i < LOG_MAX; i++)
			m_Log[i].SetHourOffset(nHourOffset);
	}
}
