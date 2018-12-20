/////////////////////////////////////////////////////////////////////////
// Ini.h: interface for the CIni class.
//
// Written by Bjarke Viksoe (bjarke@viksoe.dk)
// Copyright (c) 2000.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
//
// Beware of bugs.
////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// # 4/5/2009 OSC, GetBoolean 기능 수정
//
// # 2014-07-31, jhLee : 이름을 기존 CIni -> GxIniFile로 Gaus에 편입, Int형 지원 추가,
// # 2015-07-31, jhLee : WriteDouble() 함수에 소숫점 자릿수를 지원하도록 추가, default=3
//
//////////////////////////////////////////////////////////////////////////

#pragma once

//
// INI file class
//
// Author:
// Bjarke Viks?
// Description:
// Implements helper functions to access
// an .INI configuration file using
// conventional CString operations
//

// Ini-file wrapper class

#include "Gaus.h"


class _GAUS_LIB CGxIniFile : public CObject
{
public:
	CGxIniFile();
	CGxIniFile( LPCTSTR IniFilename );
	virtual ~CGxIniFile();


typedef enum {
	RET_OK = 0,
	RET_ERROR = -1,
	RET_UNSUPPORTED = -2,
	RET_NOTIMPLEMENTED = -3,
	RET_OUTOFMEMORY = -4,
	RET_OUTOFRESOURCES = -5,
	RET_UNKNOWN = -6,
	RET_ALREADYDEFINED = -7,
	RET_CONNECTERROR = -8,
	RET_WRONGUSERNAME = -9,
	RET_WRONGPASSWORD = -10,
	RET_WRONGNAME = -11,
	RET_CANNOTCREATE = -12,
	RET_CANNOTFIND = -13,
	RET_TIMEOUT = -14,
	RET_NOTFOUND = -15,
	RET_NOTINITIALIZED = -16,
	RET_UNKNOWNTYPE = -17,
	RET_NODATA = -18,
	RET_NOTOPEN = -19,
	RET_FILEERROR = -31,
	RET_EMPTY = -32,
	RET_PATHERROR = -34,
	RET_BUSY = -35,
	RET_NOACCESS = -36,
	RET_NORIGHTS = -37,
	//
	RET_INVALIDARGS = -100,
	RET_BADMEMORY = -101,
	RET_BADTYPE = -102,
	RET_BADFILE = -103,
	RET_BADID = -104,
	RET_BADFILEID = -105,
	RET_BADDOMAINID = -106,
	RET_BADFILENAME = -107,
	RET_BADNAME = -108,
	RET_BADSECTORID = -109,
	RET_BADFORMAT = -110,
	RET_TOOLARGE = -111,
	//
	RET_SQLERROR = -200,
	RET_SQLFAILED = -201,
	RET_SQLDOWN = -202,
	RET_SQLNOCONNECTION = -203,
	RET_SQLTIMEOUT = -204,
	RET_KILLED
} RET_CODE;


	// Methods
public:
	// Sets the current Ini-file to use.
	int SetIniFilename(LPCTSTR IniFilename);
	// Add
	CString GetIniFilename();

	//BOOL SetIniFilename(LPCTSTR IniFilename);

	// 정수형 읽기
	int GetInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault=0);
	// Reads an 32bit signed integer from the ini-file.
	long GetLong(LPCTSTR lpszSection, LPCTSTR lpszEntry, long lDefault=0);
	// Reads an double from the ini-file.
	double GetDouble(LPCTSTR lpszSection, LPCTSTR lpszEntry, double dDefault=0.0);
	// Reads a boolean value from the ini-file.
	BOOL GetBoolean(LPCTSTR lpszSection, LPCTSTR lpszEntry, BOOL bDefault=FALSE);
	// Reads a string from the ini-file.
	CString GetString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault=_T(""));
	// Reads a binaryt lump of data from the ini-file.
	BOOL GetBinary(LPCTSTR lpszSection, LPCTSTR lpszEntry, BYTE** ppData, UINT* pBytes);
	//

	// 정수형 기록
	BOOL WriteInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue);
	// Writes an 32bit signed integer to the ini-file.
	BOOL WriteLong(LPCTSTR lpszSection, LPCTSTR lpszEntry, long lValue);
	// Writes an double to the ini-file.
	BOOL WriteDouble(LPCTSTR lpszSection, LPCTSTR lpszEntry, double dValue, UINT nPoint = 3);		// 소숫점 자릿수 추가
	// Writes a boolean value to the ini-file.
	BOOL WriteBoolean(LPCTSTR lpszSection, LPCTSTR lpszEntry, BOOL bValue);
	// Writes a string to the ini-file.
	BOOL WriteString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue);
	// Writes a binary lump of data to the ini-file.
	BOOL WriteBinary(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPBYTE pData, UINT nBytes);
	// Writes an 'expand string' to the ini-file.
	BOOL WriteExpandString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue);

	//
	// Removes an item from the current ini-file.
	BOOL DeleteKey(LPCTSTR lpszSection, LPCTSTR lpszEntry);
	// Removes a complete section from the ini-file.
	BOOL DeleteSection(LPCTSTR lpszSection);


	// Variables
protected:
	CString m_IniFilename; // The current ini-file used.

};


