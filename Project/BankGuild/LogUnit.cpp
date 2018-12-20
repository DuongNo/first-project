#include "StdAfx.h"
#include "LogUnit.h"
#include "ETC/FileSupport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CLogUnit::CLogUnit(void)
{
	m_hLogEvent = CreateEvent(NULL,FALSE,TRUE,_T("Log"));
	m_WriteType = WriteType_Buffer;
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
	m_nOffsetHour = 0;
	m_nOffsetMin = 0;
}

CLogUnit::CLogUnit( WRITE_TYPE logType, CString strFileName, CString strPath, int nOffsetHour, int nOffsetMin, 
	BOOL bAddDay /*= FALSE*/, BOOL bAddTime /*= TRUE*/, BOOL bPartition /*= FALSE*/ )
{
	m_hLogEvent = CreateEvent(NULL,FALSE,TRUE,_T("Log"));
	m_WriteType = logType;
	m_pBuf = NULL;
	m_nWriteStart = 0;
	m_strBuf = _T("");
	m_nBufferSize = 4096;
	SetBufferSize(m_nBufferSize);
	m_strFolderPath = strPath;
	m_strFileName = strFileName;
	m_bAddTime = bAddTime;
	m_bAddDay = bAddDay;
	m_bPartition = bPartition;
	m_nOffsetHour = nOffsetHour;
	m_nOffsetMin = nOffsetMin;
}

CLogUnit::~CLogUnit(void)
{
	// 종료시 버퍼에 Data가 남아있으면 기록 후 종료
	WriteBuf();
	CloseHandle(m_hLogEvent);
}

void CLogUnit::SetType( WRITE_TYPE type )
{
	m_WriteType = type;
}

void CLogUnit::SetFolderPath( CString strFolderPath )
{
	m_strFolderPath = strFolderPath;
}

void CLogUnit::SetSubFolderName( CString strFolderName )
{
	m_strSubFolderName = strFolderName;
}

void CLogUnit::SetFileName( CString szFileName )
{
	m_strFileName = szFileName;
}

void CLogUnit::SetBufferSize( UINT nBufferSize )
{
	// new와 delete로 관리하면 귀찮으니 CString을 사용해서 알아서 관리하게 만든다.
	m_nBufferSize = nBufferSize;
	m_strBuf = _T("");
	m_strBuf.ReleaseBuffer();
	m_pBuf = m_strBuf.GetBuffer(nBufferSize);
	m_nWriteStart = 0;
}

void CLogUnit::SetOffsetHour( int nOffsetHour )
{
	m_nOffsetHour = nOffsetHour;
}

void CLogUnit::SetOffsetMin( int nOffsetMin )
{
	m_nOffsetMin = nOffsetMin;
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

void CLogUnit::AddBuf( LPCTSTR fmt, ... )
{
	if( WaitForSingleObject(m_hLogEvent, 10000) != WAIT_OBJECT_0)
	{
		TRACE(_T("AddBuf Event Error"));
		return;
	}

	CString strLogText;
	va_list argList;
	va_start( argList, fmt );
	strLogText.FormatV( fmt, argList );
	va_end( argList );

	if(strLogText.GetLength() < 1)
	{
		SetEvent(m_hLogEvent);
		return;
	}
	CString strLogHeader = _T("");

	if(m_bAddTime)
	{
		if (m_bAddDay)
		{
			SYSTEMTIME	time;
			GetLocalTime(&time);
			strLogHeader.Format(_T("%04u-%02u-%02u\t%02u:%02u:%02u:%03u\t%s\n"),time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, strLogText);
			strLogText = strLogHeader;
		}
		else
		{
			SYSTEMTIME	time;
			GetLocalTime(&time);
			strLogHeader.Format(_T("%02u:%02u:%02u:%03u\t%s\n"), time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, strLogText);
			strLogText = strLogHeader;
		}

	}
	else
	{
		strLogHeader.Format(_T("%s\n"), strLogText);
		strLogText = strLogHeader;
	}

	// 바로 적는 거면 여기서 바로 적는다.
	if(m_WriteType == WriteType_NoWait)
	{
		Write(strLogText);
	}
	else
	{
		int nLength = strLogText.GetLength();
		ASSERT(m_nBufferSize >= nLength);

		// Buffer가 꽉 찼을 경우 WriteType에 관계없이 무조건 적고 Buffer를 비운다.
		if(m_nWriteStart + nLength > m_nBufferSize)
		{
			WriteBuf(TRUE);
		}

		memcpy(m_pBuf+m_nWriteStart, strLogText.GetString() , nLength+1);
		// 		_tcscpy_s(m_pBuf+m_nWriteStart, nLength, _szLogText);
		m_nWriteStart += nLength;
	}
	//  #ifdef _DEBUG
	//  	TRACE(strLogText);
	//  #endif

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

	// 끝에 빈 공간 제거
	m_strBuf.ReleaseBuffer();

	// File에 기록
	FILE *stream;
	CString strFilePath ;
	if(m_bPartition)
	{
		int nExtIndex = m_strFileName.ReverseFind('.');
		strFilePath.Format(_T("%s\\%s_%02u%s"), strFolderPath, m_strFileName.Left(nExtIndex), time.wHour, m_strFileName.Mid(nExtIndex));
	}
	else
		strFilePath.Format(_T("%s\\%s"), strFolderPath, m_strFileName);

#ifdef _UNICODE
	// 처음 적을 때 유니코드로 적지 않으면 영어 빼고 모두 깨진다.
	// 그리고 유니코드 파일 생성하려면 w로밖에 생성 못한다 -_-; [4/9/2013 OSC]
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
#else
	if(_tfopen_s( &stream, strFilePath, _T("a+t") ) != 0 )
	{
		TRACE(_T("Log File open Fail! (%s)"), strFilePath);
	}
#endif

	// WriteType_Buffer가 아닌 데도 Buffer 용량 초과로 인해 적인 경우 파일에 Error 기록
	if(m_WriteType != WriteType_Buffer && bOverFlow == TRUE)
	{
		m_strBuf = _T("\nBuffer Overflow!\n");
		_ftprintf(stream, _T("%s"), m_strBuf);
	}
	fclose(stream);


	// 새로 할당
	SetBufferSize(m_nBufferSize);
}

void CLogUnit::Write(LPCTSTR szLogText)
{
	if(m_WriteType != WriteType_NoWait)
		return;
	//if(_tcslen((LPCSTR)szLogText) < 1 || _tcslen(m_strFolderPath) < 1 || _tcslen(m_strFileName) < 1)
	if(_tcslen(szLogText) < 1 || _tcslen(m_strFolderPath) < 1 || _tcslen(m_strFileName) < 1)
		return;

	// Directory Check
	SYSTEMTIME time;
	CString strFolderPath;
	if(GetFullPath(strFolderPath, time) == FALSE)
		return;

	// File에 기록
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
#ifdef _UNICODE
	// 처음 적을 때 유니코드로 적지 않으면 영어 빼고 모두 깨진다.
	// 그리고 유니코드 파일 생성하려면 w로밖에 생성 못한다 -_-; [4/9/2013 OSC]
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
#else
	if(_tfopen_s( &stream, strFilePath, _T("a+t") ) != 0 )
	{
		TRACE(_T("Log File open Fail! (%s)"), strFilePath);
		return;
	}
#endif

	_ftprintf(stream, _T("%s"), szLogText);
	// 	TRACE(_T("%d. %s\n"), stream, szLogText);
	fclose(stream);
}

BOOL CLogUnit::GetFullPath( CString &strPath, SYSTEMTIME &time )
{
	GetLocalTime(&time);

	if (time.wHour == m_nOffsetHour)
	{
		if(time.wMinute < m_nOffsetMin)
		{
			CTimeSpan timeOffset(1, 0, 0, 0);
			CTime tmYesterday = CTime::GetCurrentTime() - timeOffset; 

			time.wYear=tmYesterday.GetYear(); 
			time.wMonth=tmYesterday.GetMonth(); 
			time.wDay=tmYesterday.GetDay(); 
		}
	}
	else if(time.wHour < m_nOffsetHour)
	{
		CTimeSpan timeOffset(1, 0, 0, 0);
		CTime tmYesterday = CTime::GetCurrentTime() - timeOffset; 

		time.wYear=tmYesterday.GetYear(); 
		time.wMonth=tmYesterday.GetMonth(); 
		time.wDay=tmYesterday.GetDay(); 
	}

	CString strFolderPath;
	if(m_strSubFolderName.GetLength() > 0)
		strFolderPath.Format(_T("%s\\%04u-%02u-%02u\\%s"), m_strFolderPath, time.wYear, time.wMonth, time.wDay, m_strSubFolderName);
	else
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
