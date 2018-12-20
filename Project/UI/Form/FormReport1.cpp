// FormReport1.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FormReport1.h"

#include "GUIDefine.h"

//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CFormReport1

IMPLEMENT_DYNCREATE(CFormReport1, CFormView)

CFormReport1::CFormReport1()
	: CFormView(CFormReport1::IDD)
{

}

CFormReport1::~CFormReport1()
{
}

void CFormReport1::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFormReport1, CFormView)

	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

END_MESSAGE_MAP()


// CFormReport1 진단입니다.

#ifdef _DEBUG
void CFormReport1::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormReport1::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormReport1 메시지 처리기입니다.


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
LRESULT CFormReport1::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{
		UpdateUI();
	}
	else
	{

	}

	return 0;
}

// 표시 언어가 변경되었다.
LRESULT CFormReport1::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	CGxUICtrl::BroadcastStaticCpationIndex(this, wParam, IDC_GXSTC_TACTTIME_LIST_0_1);
	CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_GXLBL_CLEAR);

	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

// 윈도우 생성전에 모양을 달리 지정한다.
BOOL CFormReport1::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	cs.style = WS_OVERLAPPED | WS_CHILD;				// 테두리를 없앤다.

	return CFormView::PreCreateWindow(cs);
}
BEGIN_EVENTSINK_MAP(CFormReport1, CFormView)
	ON_EVENT(CFormReport1, IDC_GXLBL_CLEAR, DISPID_CLICK, CFormReport1::ClickGxlblClear, VTS_NONE)
END_EVENTSINK_MAP()


void CFormReport1::UpdateUI()
{
	CString strDataPath;
	double dBuf;
	CString	strTact;
	CString strBuf;
	CIni ini;

	strDataPath.Format(_T("%s\\ViewTactTime.ini"), theConfigBank.m_System.m_strConfigPath);

	ini.SetIniFilename(strDataPath);

	dBuf = ini.GetDouble(_T("AZONE TACT"),_T("RECENTLY"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_1_1, strTact);

	dBuf = ini.GetDouble(_T("AZONE TACT"),_T("MIN"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_1_2, strTact);

	dBuf = ini.GetDouble(_T("AZONE TACT"),_T("MAX"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_1_3, strTact);

	dBuf = ini.GetDouble(_T("AZONE TACT"),_T("AVR"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_1_4, strTact);

	dBuf = ini.GetDouble(_T("BZONE TACT"),_T("RECENTLY"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_2_1, strTact);

	dBuf = ini.GetDouble(_T("BZONE TACT"),_T("MIN"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_2_2, strTact);

	dBuf = ini.GetDouble(_T("BZONE TACT"),_T("MAX"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_2_3, strTact);

	dBuf = ini.GetDouble(_T("BZONE TACT"),_T("AVR"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_2_4, strTact);

	dBuf = ini.GetDouble(_T("CZONE TACT"),_T("RECENTLY"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_3_1, strTact);

	dBuf = ini.GetDouble(_T("CZONE TACT"),_T("MIN"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_3_2, strTact);

	dBuf = ini.GetDouble(_T("CZONE TACT"),_T("MAX"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_3_3, strTact);

	dBuf = ini.GetDouble(_T("CZONE TACT"),_T("AVR"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_3_4, strTact);

	dBuf = ini.GetDouble(_T("DZONE TACT"),_T("RECENTLY"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_4_1, strTact);

	dBuf = ini.GetDouble(_T("DZONE TACT"),_T("MIN"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_4_2, strTact);

	dBuf = ini.GetDouble(_T("DZONE TACT"),_T("MAX"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_4_3, strTact);

	dBuf = ini.GetDouble(_T("DZONE TACT"),_T("AVR"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_4_4, strTact);

	dBuf = ini.GetDouble(_T("LOAD TACT"),_T("RECENTLY"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_9_1, strTact);

	dBuf = ini.GetDouble(_T("LOAD TACT"),_T("MIN"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_9_2, strTact);

	dBuf = ini.GetDouble(_T("LOAD TACT"),_T("MAX"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_9_3, strTact);

	dBuf = ini.GetDouble(_T("LOAD TACT"),_T("AVR"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_9_4, strTact);

	dBuf = ini.GetDouble(_T("UNLOAD TACT"),_T("RECENTLY"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_10_1, strTact);

	dBuf = ini.GetDouble(_T("UNLOAD TACT"),_T("MIN"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_10_2, strTact);

	dBuf = ini.GetDouble(_T("UNLOAD TACT"),_T("MAX"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_10_3, strTact);

	dBuf = ini.GetDouble(_T("UNLOAD TACT"),_T("AVR"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_10_4, strTact);

	dBuf = ini.GetDouble(_T("UNLOAD TACT"),_T("RECENTLY1"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_14_1, strTact);

	dBuf = ini.GetDouble(_T("UNLOAD TACT"),_T("RECENTLY2"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_14_2, strTact);

	dBuf = ini.GetDouble(_T("UNLOAD TACT"),_T("RECENTLY3"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_14_3, strTact);

	dBuf = ini.GetDouble(_T("UNLOAD TACT"),_T("RECENTLY4"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_14_4, strTact);

	dBuf = ini.GetDouble(_T("UNLOAD TACT"),_T("RECENTLY5"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_14_5, strTact);

	dBuf = ini.GetDouble(_T("UNLOAD TACT"),_T("RECENTLY6"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_14_6, strTact);

	dBuf = ini.GetDouble(_T("UNLOAD TACT"),_T("RECENTLY7"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_14_7, strTact);

	dBuf = ini.GetDouble(_T("UNLOAD TACT"),_T("RECENTLY8"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_14_8, strTact);

	dBuf = ini.GetDouble(_T("UNLOAD TACT"),_T("RECENTLY9"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_14_9, strTact);

	dBuf = ini.GetDouble(_T("UNLOAD TACT"),_T("RECENTLY10"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_14_10, strTact);

	dBuf = ini.GetDouble(_T("UNLOAD TACT"),_T("RECENTLY11"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_14_11, strTact);

	dBuf = ini.GetDouble(_T("UNLOAD TACT"),_T("RECENTLY12"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_14_12, strTact);

	dBuf = ini.GetDouble(_T("UNLOAD TACT"),_T("RECENTLY13"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_14_13, strTact);

	dBuf = ini.GetDouble(_T("UNLOAD TACT"),_T("RECENTLY14"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_14_14, strTact);

	dBuf = ini.GetDouble(_T("UNLOAD TACT"),_T("RECENTLY15"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_14_15, strTact);

	dBuf = ini.GetDouble(_T("UNLOAD TACT"),_T("RECENTLY16"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_14_16, strTact);

	dBuf = ini.GetDouble(_T("UNLOAD TACT"),_T("RECENTLY17"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_14_17, strTact);

	dBuf = ini.GetDouble(_T("UNLOAD TACT"),_T("RECENTLY18"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_14_18, strTact);

	dBuf = ini.GetDouble(_T("UNLOAD TACT"),_T("RECENTLY19"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_14_19, strTact);

	dBuf = ini.GetDouble(_T("UNLOAD TACT"),_T("RECENTLY20"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_14_20, strTact);

	dBuf = ini.GetDouble(_T("UNLOAD TACT"),_T("20AVR"));
	strTact.Format(_T("%3.2f"),(double)dBuf);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_14_21, strTact);

	strBuf = ini.GetString(_T("UNLOAD TACT"),_T("RECENTLYID1"));
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_13_1, strBuf);

	strBuf = ini.GetString(_T("UNLOAD TACT"),_T("RECENTLYID2"));
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_13_2, strBuf);

	strBuf = ini.GetString(_T("UNLOAD TACT"),_T("RECENTLYID3"));
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_13_3, strBuf);

	strBuf = ini.GetString(_T("UNLOAD TACT"),_T("RECENTLYID4"));
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_13_4, strBuf);

	strBuf = ini.GetString(_T("UNLOAD TACT"),_T("RECENTLYID5"));
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_13_5, strBuf);

	strBuf = ini.GetString(_T("UNLOAD TACT"),_T("RECENTLYID6"));
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_13_6, strBuf);

	strBuf = ini.GetString(_T("UNLOAD TACT"),_T("RECENTLYID7"));
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_13_7, strBuf);

	strBuf = ini.GetString(_T("UNLOAD TACT"),_T("RECENTLYID8"));
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_13_8, strBuf);

	strBuf = ini.GetString(_T("UNLOAD TACT"),_T("RECENTLYID9"));
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_13_9, strBuf);

	strBuf = ini.GetString(_T("UNLOAD TACT"),_T("RECENTLYID10"));
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_13_10, strBuf);

	strBuf = ini.GetString(_T("UNLOAD TACT"),_T("RECENTLYID11"));
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_13_11, strBuf);

	strBuf = ini.GetString(_T("UNLOAD TACT"),_T("RECENTLYID12"));
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_13_12, strBuf);

	strBuf = ini.GetString(_T("UNLOAD TACT"),_T("RECENTLYID13"));
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_13_13, strBuf);

	strBuf = ini.GetString(_T("UNLOAD TACT"),_T("RECENTLYID14"));
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_13_14, strBuf);

	strBuf = ini.GetString(_T("UNLOAD TACT"),_T("RECENTLYID15"));
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_13_15, strBuf);

	strBuf = ini.GetString(_T("UNLOAD TACT"),_T("RECENTLYID16"));
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_13_16, strBuf);

	strBuf = ini.GetString(_T("UNLOAD TACT"),_T("RECENTLYID17"));
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_13_17, strBuf);

	strBuf = ini.GetString(_T("UNLOAD TACT"),_T("RECENTLYID18"));
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_13_18, strBuf);

	strBuf = ini.GetString(_T("UNLOAD TACT"),_T("RECENTLYID19"));
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_13_19, strBuf);

	strBuf = ini.GetString(_T("UNLOAD TACT"),_T("RECENTLYID20"));
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TACTTIME_LIST_13_20, strBuf);
}

void CFormReport1::ClickGxlblClear()
{
	CString strDataPath;
	strDataPath.Format(_T("%s\\ViewTactTime.ini"), theConfigBank.m_System.m_strConfigPath);
	::DeleteFile(strDataPath);
	UpdateUI();
}
