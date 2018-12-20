//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// CGxLogBase : 주어진 문자열을 화면과 파일로 기록하는 class
//
// 설명 : 
//			여러 패턴으로 파일 경로 및 이름을 자동 생성시켜주고 Unicode 및 MBCS로 선택적 저장이 가능하다.
//			과거 로그를 삭제하기 위한 트리거 함수로 외부 쓰레드에서 삭제 수행이 용이하도록 하였다.
//
// 2015-09-01, jhLee, 파일 및 화면 ListCtrl에 기록하는 기본 Base class
// 2015-09-09, jhlee, 헤더 출력기능 추가, 삭제함수, GetToday 함수 보완
// 2015-10-29, jhLee, Debug용 Trace 보완, CMap 및 CArray 구조체 추가
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <afxmt.h>														// CCriticalSection 사용

#include "Gaus.h"

class _GAUS_LIB CGxLog
{
public:
	CGxLog();
	virtual ~CGxLog();

	enum {	// Folder 생성 이름에 대한 설정값

		// m_nDirNameMode 지정 값, 자동으로 파일 이름을 생성하는 패턴 지정
		eNAME_None			= 0,											// 지정한 폴더에서만 파일 생성
		eNAME_Year,															// 년별로 새로운 폴더 생성,				\LOG\2015\test.txt
		eNAME_Month,														// 년/월 별로 새로운 폴더 생성,			\LOG\2015\09\test.txt
		eNAME_Day,															// 년/월/일 별로 새로운 폴더 생성,		\LOG\2015\09\03\test.txt
		eNAME_MonthOnly,													// 월별로 생성한다. (년은 생략),		\LOG\09\test.txt
		eNAME_DayOnly,														// 일별로 생성한다. (년/월은 생략),		\LOG\03\test.txt
		eNAME_YearMonth,													// 년월 별로 새로운 폴더 생성,			\LOG\201509\test.txt
		eNAME_YearMonthDay,												// 년월일 별로 새로운 폴더 생성,		\LOG\20150903\test.txt
		eNAME_MonthDay,													// 월일 별로 새로운 폴더 생성,			\LOG\0903\test.txt
		
		// 파일 이름 생성시 날짜 정보를 어느곳에 넣을것인지 지정, 중간 구분자와 날짜 정보를 어떻게 배치할 것인지 지정
		// m_nDirDatePos, m_nFileDatePos 에서 사용
		ePOS_DateNone		= 0,											// 파일 이름에 날짜 정보를 넣지 않는다. -> 사용 금지, 파일이 거대해질 수 있다.
		ePOS_DateFirst,													// 파일 이름 / 경로의 중간 구분자 앞에 날짜를 넣는다.	D:\AUTO_TEST\LOG\05\CELL_DATA\20150512_CELL_Y01.csv
		ePOS_DateLast,														// 파일 이름 / 경로의 중간 구분자 뒤에 날짜를 넣는다.	D:\Machine_Data\Alarm_Data\20150110\Alarm_20150110.csv

	};


protected:

	// 화면의 List Control에 표기를 하기위한 변수
	CWnd*			m_pCaller;												// 내용을 표시할 화면 pointer
	UINT			m_nListID;												// 화면 표시용 List Control resource ID

	DWORD			m_dwUIThread;											// UI에서 호출하는 Main Thread ID, Thread Safety 적용시 Deadlock 방지용
	CCriticalSection m_CSLogWrite;									// 다중 접근 방지

	// 해더 출력용 변수
	CString		m_sHeader;												// 파일이 새로 생설될 때 출력하는 헤더 내용
	WCHAR			*m_pwszHeaderUnicode;								// Unicode 기록용 Header 문자열
	char			*m_pszHeaderMBCS;										// MBCS 파일 기록용 Header 문자열


	CString		GetPrefix();											// Data에 저장할 데이터의 접두문자를 구한다. (날짜/시각 등)
	virtual void WriteToList(CString &sData);						// 화면 List Control에 내용을 표시한다.
	virtual void WriteToUnicodeFile_s(CString sPath, CString &sData);		// 지정 파일에 유니코드로 저장을 한다. (Thread Safety 지원)
	virtual void WriteToMBCSFile_s(CString sPath, CString &sData);			// 지정 파일에 MBCS로 저장을 한다. (Thread Safety 지원)
	virtual void RemoveEmptyFolder(CString sPath);				// 지정 폴더가 비어있다면 삭제를 한다.

public:
	// for Debug
	BOOL			m_bDebugLog;											// 디버깅용 TRACE Log를 남길 것인가 ?


	// 기본적인 기능 속성
	int			m_nMaxLineCount;										// Listcontrol에 표시할 라인의 최대 줄수 제한
	int			m_nKeepDay;												// Log를 보관하는 일수
	int			m_nDirNameMode;										// 자동으로 이름을 생성시 경로명 지정 모드 eNAME_None ~ 
	int			m_nDirDatePos;											// 자동으로 경로명을 생성시 날짜와 Group 이름을 어떻게 배치할 것인가 ? ePOS_DateNone ~
	int			m_nFileDatePos;										// 자동으로 파일이름을 생성시 지정 파일 이름과 생성 날자를 어떻게 배치할 것인가 ? ePOS_DateNone ~
	int			m_nTodayHour;											// 하루의 기준을 정하는 시간 ( 0 ~ 23 )
	int			m_nTodayMin;											// 하루의 기준을 정하는 분 ( 0 ~ 59 )

	BOOL			m_bIsFolderCreate;									// 폴더가 존재하지 않는다면 TRUE:강제로 만들어준다, FALSE:수행 포기
	BOOL			m_bIsCustomToday;										// 하루를 정의하는 기준을 사용자가 임의로 지정한다. TRUE:SetToday() 설정값에의해 동작, FALSE:0시 기준으로 하루를 나눈다.
	BOOL			m_bIsHourly;											// 파일을 매시간단위로 생성할것인가 ? TRUE:시간단위로 파일 생성, FALSE:일(日) 단위로 파일 생성
	BOOL			m_bIsThreadLock;										// 파일 저장시 Multi-Thread 보호를 수해할 것인가 ? TRUE:보호 수행, FALSE:미수행(GUI 홤경에서는 반드시 FALSE로 지정)
	BOOL			m_bIsAutoDelete;										// 자동으로 과거 log 파일을 삭제할 것인가? TRUE:자동삭제, FALSE:삭제하지 않음

	BOOL			m_bIsUnicode;											// Unicode로 파일을 저장할 것인가 ? TRUE:Unicode로 파일저장, FALSE:MBCS(ANSI)로 파일 저장
	BOOL			m_bIsDatePrint;										// 날짜를 자동으로 삽입하여 출력할 것인가 ? TRUE:각 줄마다 년-월-일 표기, FALSE:표기 안함
	BOOL			m_bIsTimePrint;										// 시각을 자동으로 삽입하여 출력할 것인가 ? TRUE:각 줄마다 시:분:초:밀리초 표기, FALSE:표기안함
	BOOL			m_bIsHeaderPrint;										// 파일의 시작마다 각 컬럼의 헤더를 출력할 것인가 ? TRUE:헤더 출력, FALSE:출력 안함.

	// 기록 옵션, 설정 옵션 및 기본 조건이 모두 성립되어야 지정한 기능을 수행한다.
	BOOL			m_bIsEnabled;											// Log 기능을 사용할 것인가 ?
	BOOL			m_bIsFileWrite;										// 파일에 기록을 할 것인가 ? TRUE:파일로 기록, FALSE:기록 안함
	BOOL			m_bIsListWrite;										// 화면에 기록을 할 것인가 ? TRUE:화면 ListCtrl에 기록 함, FALSE:기록 안함.


	// 문자열 데이터
	CString		m_sExtention;											// 파일 확장자, 예:txt , csv, log ....
	CString		m_sBasePath;											// 로그가 기록되는 기본 폴더, 지정 폴더의 하위에 기록이 된다. 예:D:\Work\AMT\Log
	CString		m_sGroupName;											// 로그의 그룹(중간 구분자) 이름 지정
	CString		m_sFilename;											// 로그를 기록 할 기본 파일이름, 본 파일 이름뒤에 날짜가 붙는다. 예:TCP
	CString		m_sLastFilename;										// 마지막에 적용된 파일이름


public:		// 공개 Function block

	void		SetEnabled(BOOL bFlag);									// 기록 여부를 지정한다.
	BOOL		GetEnabled();												// 기록 여부를 조회한다.

	void		SetListHandle(CWnd* pViewHandle, int nListID);					// 화면에 보여줄 View 핸들과 CListCtrl 리소스 ID
	void		SetFilePath(CString sPath, CString sGoup, CString sFile);	// 파일을 기록할 경로와 그룹/ 파일 이름을 지정한다.
	void		SetHeader(CString sData);								// Header 문자열을 지정한다.

	void		SetToday(int nHour, int nMin);						// 하루의 기준 시각을 지정한다.	
	CTime		GetToday(int nDaySpan = 0);							// 주어진 시각에 해당하는 로그 기록 일자를 되돌려준다.


	void ClearList();														// 화면 List의 내용을 모두 지운다.
	virtual CString	GetFilePath(CTime tmNow);					// 지정한 시각에 해당되는 전체 경로와 파일이름을 생성한다.
	virtual void Write(LPCTSTR szFmt, ...);						// 주어진 문자열을 로그로 남긴다. (현재 시각으로 자동 파일 생성)
	virtual void WriteDay(int nDaySpan, LPCTSTR szFmt, ...);	// 주어진 문자열을 로그로 남긴다. (지정한 시각으로 파일을 생성하는 기준 일자를 적용한다)
	virtual BOOL RemoveOldLog(int nDay = 0);						// 지정된 보관 기간이 지난 이전 로그를 삭제해준다. (외부에서 호출하는 Trigger 함수)

public:		// 공개 범용 함수

	static BOOL	IsExistFile(CString sFilePath);					// 지정 파일이 존재하는가 ?
	static BOOL	IsExistFolder(CString sPath);						// 지정 폴더가 존재하는가 ?
	static BOOL IsEmptyFolder(CString sPath);						// 비어있는 폴더인가 ?
	static BOOL	CheckFolder(CString sPath, BOOL bCreate);		// 폴더의 존재 여부를 확인하고 존재하지 않는다면 새로이 만들어 준다.

	static void WriteToUnicodeFile(CString sPath, CString &sData);		// 지정 파일에 유니코드로 저장을 한다.
	static void WriteToMBCSFile(CString sPath, CString &sData);			// 지정 파일에 MBCS로 저장을 한다.

};

// CGxLog 관리用
typedef CArray<CGxLog, CGxLog&> CGxLogArray;
typedef CTypedPtrArray<CPtrArray, CGxLog*> CGxLogPtrArray;
typedef CTypedPtrMap<CMapStringToPtr, CString, CGxLog*> CGxMapStringToLog;
typedef CTypedPtrMap<CMapWordToPtr, WORD, CGxLog*> CGxMapWordToLog;

