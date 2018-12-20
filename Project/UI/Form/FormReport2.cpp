// FormReport2.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FormReport2.h"

#include "GUIDefine.h"

//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "UI\GausGUI\GxStaticEx.h"
#include "UI\GausGUI\GxButtonEx.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CFormReport2

IMPLEMENT_DYNCREATE(CFormReport2, CFormView)

CFormReport2::CFormReport2()
	: CFormView(CFormReport2::IDD)
{
	m_iPart = DAY;
	Init();
}

CFormReport2::~CFormReport2()
{
}

void CFormReport2::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFormReport2, CFormView)

	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

END_MESSAGE_MAP()


// CFormReport2 진단입니다.

#ifdef _DEBUG
void CFormReport2::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormReport2::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

void CFormReport2::Init()		//	시간 설정
{
	//2017-05-25, SKCHO, 프로그램 시작할때 계산하는위치로 이동
	/*GetLocalTime(&theProcBank.m_CurDate);

	CTime cTime(theProcBank.m_CurDate);

	if ( theProcBank.m_CurDate.wHour < 7 )
		cTime += CTimeSpan(-1,0,0,0);
	
	
	theProcBank.m_CurDate.wYear = (WORD)cTime.GetYear();
	theProcBank.m_CurDate.wMonth = (WORD)cTime.GetMonth();
	theProcBank.m_CurDate.wDay = (WORD)cTime.GetDay();
	*/
	//m_szCurrent_time.Format(_T("%04u-%02u-%02u"),m_CurDate.wYear, m_CurDate.wMonth, m_CurDate.wDay);

	
}

void CFormReport2::UpdateUI()			//	UI화면을 UPDATE 해줌
{
	theProductBank.CalcAllInfo();	//	임의로 여기서 호출
	CGxButtonEx* pReportDayBtn = (CGxButtonEx*)GetDlgItem( IDC_GXLBL_REPORT_DAY );
	CGxButtonEx* pReportSwBtn = (CGxButtonEx*)GetDlgItem( IDC_GXLBL_REPORT_SW );
	CGxButtonEx* pReportGyBtn = (CGxButtonEx*)GetDlgItem( IDC_GXLBL_REPORT_GY );
 	int iTimeVal = -1;
 	if ( m_iPart == DAY )
	{
		iTimeVal = 7;				//	해당작업조의 시작 시간
		pReportDayBtn->SetBackColor(Color(Color::Lime).ToCOLORREF());
		pReportSwBtn->SetBackColor(Color(Color::White).ToCOLORREF());
		pReportGyBtn->SetBackColor(Color(Color::White).ToCOLORREF());
	}
 	else if ( m_iPart == SW )		
	{
		iTimeVal = 15;
		pReportDayBtn->SetBackColor(Color(Color::White).ToCOLORREF());
		pReportSwBtn->SetBackColor(Color(Color::Lime).ToCOLORREF());
		pReportGyBtn->SetBackColor(Color(Color::White).ToCOLORREF());
	}
 	else if ( m_iPart == GY )		
	{
		iTimeVal = 23;
		pReportDayBtn->SetBackColor(Color(Color::White).ToCOLORREF());
		pReportSwBtn->SetBackColor(Color(Color::White).ToCOLORREF());
		pReportGyBtn->SetBackColor(Color(Color::Lime).ToCOLORREF());
	}

	short iSumCount = 0;					//	각 작업조별 데이터 합
	short iSumDefect = 0;
	short iSumReject = 0;
	short iSumMCR = 0;
	short iSumPre = 0;
	short iSumCont = 0;
 
 	ProductInfo tInfo;
 	CString szValue;
	for(int i = 0; i < 8; i++ )
	{
		//2017-05-26,SKCHO
		if ( iTimeVal + i >= 24 )		iTimeVal = -1; 
		//if ( iTimeVal + i > 24 )		iTimeVal = -1; 

		tInfo = theProductBank.GetMapData()[iTimeVal+i];

		CGxUICtrl::SetStaticString(this, tInfo.tHour.uID, CStringSupport::N2C(tInfo.tHour.iValue) );
		SetStaticUIString(tInfo);
	}

	CGxUICtrl::SetStaticString(this, IDC_GXLBL_REPORT_DATE, theProcBank.m_szCurrent_time);

	int iSize = theProductBank.GetVecDataSum().size();
	for (int j = 0; j < iSize; j++ )
	{
		tInfo = theProductBank.GetVecDataSum()[j];
		SetStaticUIString(tInfo);
	}	

	
	if ( m_iPart == DAY )
	{
		tInfo = theProductBank.GetVecDataSum()[0];
		SetStaticUIString(tInfo, IDC_GXLBL_REPORT_TOTAL_1, IDC_GXLBL_REPORT_TOTAL_2, IDC_GXLBL_REPORT_TOTAL_4, IDC_GXLBL_REPORT_TOTAL_5, IDC_GXLBL_REPORT_TOTAL_6, IDC_GXLBL_REPORT_TOTAL_7);
	}
	else if ( m_iPart == SW )
	{
		tInfo = theProductBank.GetVecDataSum()[1];
		SetStaticUIString(tInfo, IDC_GXLBL_REPORT_TOTAL_1, IDC_GXLBL_REPORT_TOTAL_2, IDC_GXLBL_REPORT_TOTAL_4, IDC_GXLBL_REPORT_TOTAL_5, IDC_GXLBL_REPORT_TOTAL_6, IDC_GXLBL_REPORT_TOTAL_7);
	}
	else if ( m_iPart == GY )
	{
		tInfo = theProductBank.GetVecDataSum()[2];
		SetStaticUIString(tInfo, IDC_GXLBL_REPORT_TOTAL_1, IDC_GXLBL_REPORT_TOTAL_2, IDC_GXLBL_REPORT_TOTAL_4, IDC_GXLBL_REPORT_TOTAL_5, IDC_GXLBL_REPORT_TOTAL_6, IDC_GXLBL_REPORT_TOTAL_7);
	}
	
	tInfo = theProductBank.GetDataWholeDay();			//	전체
	SetStaticUIString(tInfo);	
}


void CFormReport2::SetStaticUIString(ProductInfo tInfo)			//	ProductInfo 의 정보를 static 에 셋팅해주는 함수
{
	CString szPerMCR = _T("(") + theProductBank.CalcPercent(tInfo.tCount.iValue, tInfo.tDefMCR.iValue) + _T(")");
	CString szPerPreAlign = _T("(") + theProductBank.CalcPercent(tInfo.tCount.iValue, tInfo.tDefAlign.iValue) + _T(")");
	CString szPerContact = _T("(") + theProductBank.CalcPercent(tInfo.tCount.iValue, tInfo.tDefCont.iValue) + _T(")");

	CGxUICtrl::SetStaticString(this, tInfo.tCount.uID, CStringSupport::N2C(tInfo.tCount.iValue) );
	CGxUICtrl::SetStaticString(this, tInfo.tDefect.uID, CStringSupport::N2C(tInfo.tDefect.iValue) );
	CGxUICtrl::SetStaticString(this, tInfo.tReject.uID, CStringSupport::N2C(tInfo.tReject.iValue) );
	CGxUICtrl::SetStaticString(this, tInfo.tDefMCR.uID, CStringSupport::N2C(tInfo.tDefMCR.iValue) + szPerMCR);
	CGxUICtrl::SetStaticString(this, tInfo.tDefAlign.uID, CStringSupport::N2C(tInfo.tDefAlign.iValue) + szPerPreAlign);
	CGxUICtrl::SetStaticString(this, tInfo.tDefCont.uID, CStringSupport::N2C(tInfo.tDefCont.iValue) + szPerContact);
}

void CFormReport2::SetStaticUIString(ProductInfo tInfo, UINT IDCount, UINT IDDefect, UINT IDReject, UINT IDPerMCR, UINT IDPerAlign, UINT IDCont)			//	ProductInfo 의 정보를 static 에 셋팅해주는 함수, ID를 입력받는다
{
	CString szPerMCR = _T("(") + theProductBank.CalcPercent(tInfo.tCount.iValue, tInfo.tDefMCR.iValue) + _T(")");
	CString szPerPreAlign = _T("(") + theProductBank.CalcPercent(tInfo.tCount.iValue, tInfo.tDefAlign.iValue) + _T(")");
	CString szPerContact = _T("(") + theProductBank.CalcPercent(tInfo.tCount.iValue, tInfo.tDefCont.iValue) + _T(")");

	CGxUICtrl::SetStaticString(this, IDCount, CStringSupport::N2C(tInfo.tCount.iValue) );
	CGxUICtrl::SetStaticString(this, IDDefect, CStringSupport::N2C(tInfo.tDefect.iValue) );
	CGxUICtrl::SetStaticString(this, IDReject, CStringSupport::N2C(tInfo.tReject.iValue) );
	CGxUICtrl::SetStaticString(this, IDPerMCR, CStringSupport::N2C(tInfo.tDefMCR.iValue) + szPerMCR );
	CGxUICtrl::SetStaticString(this, IDPerAlign, CStringSupport::N2C(tInfo.tDefAlign.iValue) + szPerPreAlign );
	CGxUICtrl::SetStaticString(this, IDCont, CStringSupport::N2C(tInfo.tDefCont.iValue) + szPerContact );
}


// CFormReport2 메시지 처리기입니다.


// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
//
// CGxGUIFrame에서 ShowDialog를 수행하면 아래 순서대로 메세지를 호출한다.
//
// ShowDialog() 호출시
// 1. GxMSG_GUI_UPDATE_TIMER, TRUE
//
// HideDlgBar() 호출시
// 1. GxMSG_GUI_UPDATE_TIMER, FALSE
//

// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
LRESULT CFormReport2::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )																//	Report2 화면으로 들어올떄
	{
		theProductBank.LoadReportFile(theProcBank.m_szCurrent_time.GetBuffer());
		//theProductBank.LoadReportFile(m_szCurrent_time.GetBuffer());
		UpdateUI();
	}
	else																		//	Report2 화면을 떠날때
	{

	}

	return 0;
}

// 표시 언어가 변경되었다.
LRESULT CFormReport2::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	CGxUICtrl::BroadcastStaticCpationIndex(this, wParam, IDC_GXLBL_REPORT_TIME);
	CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_GXLBL_REPORT_PREV);

	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

// 윈도우 생성전에 모양을 달리 지정한다.
BOOL CFormReport2::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	cs.style = WS_OVERLAPPED | WS_CHILD;				// 테두리를 없앤다.

	return CFormView::PreCreateWindow(cs);
}
BEGIN_EVENTSINK_MAP(CFormReport2, CFormView)
	ON_EVENT(CFormReport2, IDC_GXLBL_REPORT_DAY, DISPID_CLICK, CFormReport2::ClickDay, VTS_NONE)
	ON_EVENT(CFormReport2, IDC_GXLBL_REPORT_SW, DISPID_CLICK, CFormReport2::ClickSW, VTS_NONE)
	ON_EVENT(CFormReport2, IDC_GXLBL_REPORT_GY, DISPID_CLICK, CFormReport2::ClickGY, VTS_NONE)
	ON_EVENT(CFormReport2, IDC_GXLBL_REPORT_PREV, DISPID_CLICK, CFormReport2::ClickPrev, VTS_NONE)
	ON_EVENT(CFormReport2, IDC_GXLBL_REPORT_NEXT, DISPID_CLICK, CFormReport2::ClickNext, VTS_NONE)
	ON_EVENT(CFormReport2, IDC_GXLBL_REPORT_TODAY, DISPID_CLICK, CFormReport2::ClickGxlblReportToday, VTS_NONE)
END_EVENTSINK_MAP()


void CFormReport2::ClickDay()		//	DAY 버튼 클릭시
{
	m_iPart = DAY;
	UpdateUI();
}


void CFormReport2::ClickSW()		//	SW 버튼 클릭시
{
	m_iPart = SW;
	UpdateUI();
}


void CFormReport2::ClickGY()		//GY 버튼 클릭시
{
	m_iPart = GY;
	UpdateUI();
}



void CFormReport2::ClickPrev()		//	날짜 PREV 클릭시
{
	SetDateTime(-1);

	theProductBank.LoadReportFile(theProcBank.m_szCurrent_time.GetBuffer());
	UpdateUI();
}


void CFormReport2::ClickNext()		// 날짜 NEXT 클릭시
{
	SetDateTime(+1);

	theProductBank.LoadReportFile(theProcBank.m_szCurrent_time.GetBuffer());
	UpdateUI();
}


void CFormReport2::SetDateTime(int iValue, BOOL btoday)		//	PREV나 NEXT 클릭시 날짜를 바꿔줌
{
	SYSTEMTIME today;
	GetLocalTime(&today);
	if(btoday)
	{
		GetLocalTime(&theProcBank.m_CurDate);
	}
	CTime Compare1(theProcBank.m_CurDate);
	CTime Compare2(today);
	
	if(btoday)
	{
		if ( today.wHour < 7 )
		{
			iValue = -1;
		}
	}
	else
	{
		if ( today.wHour < 7 )
		{
			Compare2 += CTimeSpan(-1,0,0,0);
		}

		Compare1 += (CTimeSpan(iValue, 0, 0, 0));

		if ( Compare1 > Compare2 )
			return;
	}	

	CTime CalcTime(theProcBank.m_CurDate);
	CalcTime += CTimeSpan(iValue,0,0,0);
	theProcBank.m_CurDate.wYear = (WORD)CalcTime.GetYear();
	theProcBank.m_CurDate.wMonth = (WORD)CalcTime.GetMonth();
	theProcBank.m_CurDate.wDay = (WORD)CalcTime.GetDay();
			
	theProcBank.m_szCurrent_time.Format(_T("%04u-%02u-%02u"),theProcBank.m_CurDate.wYear, theProcBank.m_CurDate.wMonth, theProcBank.m_CurDate.wDay);
}

void CFormReport2::ClickGxlblReportToday()
{
	SetDateTime(0, TRUE);
	theProductBank.LoadReportFile(theProcBank.m_szCurrent_time.GetBuffer());
	UpdateUI();
}
