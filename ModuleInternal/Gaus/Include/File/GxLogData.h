//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// CGxLog로부터 상속받아 1줄 단위로 컬럼 데이터식으로 데이터를 기록하기위한 class
//
// 설명 :
//			콤마로 각 데이터 항목을 구분하는 CSV 파일 저장이 가능하며, 파일이 열린상태를 유지함으로 다른 곳에서의 변경에 대비할 수 있고,
//			미러링 파일 기록과, 기록 실패시 실패 내용 백업 저장기능이 추가되었다.
//			Cell log 및 비가동 로그등의 데이터 저장에 사용하기 위한 용도이다.
//
// 2015-09-01, jhLee
// 2015-09-09, jhLee, 
//

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <afxmt.h>												// CCriticalSection 사용

#include "Gaus.h"
#include "GxLog.h"

class _GAUS_LIB CGxLogData : public CGxLog
{
public:
	CGxLogData();
	virtual ~CGxLogData();

protected:		// 보호 속성 및 함수

	FILE*			m_pFileHandle;											// 기본적으로 사용되는 File Handle

	CString		GetMakeMirrorPath(CString sPath);				// 지정 경로명에서 미러파일을 보관할 경로를 확보한다.
	virtual BOOL WriteToUnicodeFile_KeepOpen(CString sPath, CString &sData);	// 지정 파일에 유니코드로 저장을 한다. 파일은 열린채로 진행한다.
	virtual BOOL WriteToMBCSFile_KeepOpen(CString sPath, CString &sData);		// 지정 파일에 MBCS로 저장을 한다. 파일은 열린채로 진행한다.

public:		// 공개 속성
	// Data Log 속성
	BOOL			m_bKeepOpen;											// 파일 열림상태를 유지할 것인가 ? DataLog에 한함, TRUE:열림유지, 他 프로그램에서 open 못함, FALSE:Write후 즉시 Close, 他 프로그램에서 사용가능
	BOOL			m_bMirrorCreate;										// 보조 파일을 생성할 것인가 ? TRUE:미러링 파일 생성, FALSE:생성 안함

	CString		m_sMirrorSuffix;										// 미러 파일 뒤에 붙일 접미사, 예:Temp -> TCP_150901_10_Temp.txt
	CString		m_sErrorSuffix;										// 기록에 실패한 내용을 별도로 저장하기위한 예외 처리 파일 접미사, 예: "Error" -> TCP_150901.txt_Error.txt
	CString		m_sItemData;											// 출력할 데이터, 각 항목별로 Add 하여 완성을 하는 데이터 문자열
	CString		m_sLastFilename;										// 마지막에 적용된 파일이름, 이름이 달라지면 기존 File은 Close 해준다.

	// 항목 구분자
	CString		m_sDeliItem;											// 항목과 항목사이 구분자. 주로 ',' 콤마가 사용된다.
	CString		m_sDeliDate;											// 날짜내 년/월/일 사이의 구분자, 주로 '-' 대쉬가 사용된다.
	CString		m_sDeliTime;											// 시각 정보의 시/분/초 사이의 구분자, 주로 ':' 콜론이 사용된다.
	CString		m_sDeliMiliSec;										// 시작 정보 중 초 데이터와 밀리초 데이터사이의 구분자. 주로 '.' 마침표 혹은 ':' 콜론이 사용된다.
	CString		m_sDeliDateTime;										// 날짜와 시각정보 사이에 값을 구분하는 구분자. 주로 ' ' 공백이 사용된다.


public:		// 공개 함수
	BOOL IsFileOpen();													// File이 열려져있는가 ? m_bKeepOpen 설정 여부에따라서 내용 상이 
	BOOL OpenFile(int nDaySpan = 0);									// 파일을 열어 소유권을 선점하도록 한다. m_bKeepOpen = TRUE 일때 유효
	void CloseFile();														// 열려진 File을 닫는다. m_bKeepOpen = TRUE 일때 유효


	void Clear();															// 조립중인 Data Body 문자열을 초기화 한다.
	void AddItem(CString sData);										// 지정 문자열을 출력할 List에 추가한다.
	void AddItem_Int(int nValue);										// 정수형 값을 추가한다.
	void AddItem_Double(double dbValue, int nPoint);			// 실수형 값을 추가한다. nPoint로 소숫점 이하 자릿수를 지정한다.
	void AddItem_Date(SYSTEMTIME tmDate);							// 현재 시각의 날짜를 더한다.
	void AddItem_Time(SYSTEMTIME tmDate, BOOL bUseMsec);		// 현재 시각의 시간을 더한다. bUseMsec : 밀리초를 표시할 것인가 ?
	void AddItem_DateTime(SYSTEMTIME tmDate, BOOL bUseMsec);	// 현재 시각의 날짜와 시간을 더한다.bUseMsec : 밀리초를 표시할 것인가 ?

public:		// 후손 재정의 가능 함수

	virtual CString GetFilePath(CTime tmNow);						// 지정한 시각에 해당되는 전체 경로와 파일이름을 생성한다.
	virtual CString GetMirrorFilePath(CString sFile);			// 주어진 파일에 해당하는 Mirror 파일의 파일경로를 생성한다.

	virtual void WriteItem(int nDaySpan = 0);						// 지정한 Item 문자열을 기록한다.
	virtual void Write(LPCTSTR szFmt, ...);						// 주어진 문자열을 로그로 남긴다.
	virtual void WriteDay(int nDaySpan, LPCTSTR szFmt, ...);	// 주어진 문자열을 로그로 남긴다. (지정한 시각으로 파일을 생성하는 기준 일자를 적용한다)
	virtual BOOL RemoveOldLog(int nDay = 0);						// 지정된 보관 기간이 지난 이전 로그를 삭제해준다. (외부에서 호출하는 Trigger 함수)

};
