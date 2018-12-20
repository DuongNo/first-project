// DlgbarHReport.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgbarHReport.h"

//d #include "MainFrm.h"

//GxGUIFrame 사용
#include "GUIDefine.h"
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW

#endif
// CDlgbarHReport 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgbarHReport, CDialogBar)

CDlgbarHReport::CDlgbarHReport()
{
}

CDlgbarHReport::~CDlgbarHReport()
{
}

void CDlgbarHReport::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgbarHReport, CDialogBar)
	// ON_UPDATE_COMMAND_UI(IDC_BTN_HREPORT_SUBMENU1, OnUpdateCmdUI)
	// ON_BN_CLICKED(IDC_BTN_HREPORT_SUBMENU1, &CDlgbarHReport::OnBnClickedBtnHreportSubmenu1)

	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_VIEW_END,	OnChangeViewEnd)			// 메인 화면이 변경 완료되었다.
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CDlgbarHReport, CDialogBar)
	ON_EVENT(CDlgbarHReport, IDC_BTNX_HREPORT_SUBMENU1, DISPID_CLICK, CDlgbarHReport::OnClickBtnxSubMenu1, VTS_NONE)
	ON_EVENT(CDlgbarHReport, IDC_BTNX_HREPORT_SUBMENU2, DISPID_CLICK, CDlgbarHReport::OnClickBtnxSubMenu2, VTS_NONE)
	ON_EVENT(CDlgbarHReport, IDC_BTNX_HREPORT_SUBMENU3, DISPID_CLICK, CDlgbarHReport::OnClickBtnxSubMenu3, VTS_NONE)
	ON_EVENT(CDlgbarHReport, IDC_BTNX_HREPORT_SUBMENU4, DISPID_CLICK, CDlgbarHReport::OnClickBtnxSubMenu4, VTS_NONE)
	
	ON_EVENT(CDlgbarHReport, IDC_BTNX_HREPORT_SUBMENU5, DISPID_CLICK, CDlgbarHReport::ClickBtnxHreportSubmenu5, VTS_NONE)
END_EVENTSINK_MAP()


// CDlgbarHReport 메시지 처리기입니다.
void CDlgbarHReport::OnUpdateCmdUI(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}


BOOL CDlgbarHReport::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style = WS_OVERLAPPED | WS_CHILD;	// 폼의 테두리를 없앤다.

	return CDialogBar::PreCreateWindow(cs);
}

// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
//
// CGxGUIFrame에서 ShowDialog를 수행하면 아래 순서대로 메세지를 호출한다.
//
// ShowDlgBar() 호출시
// 2. GxMSG_GUI_CHANGE_SUBMENU,	nSubID
// 3. GxMSG_GUI_UPDATE_TIMER, TRUE
//
// HideDlgBar() 호출시
// 1. GxMSG_GUI_UPDATE_TIMER, FALSE
//

// 메인 메뉴 선택 화면 전환 완료
LRESULT CDlgbarHReport::OnChangeViewEnd(WPARAM wParam, LPARAM lParam)
{
	DispCurrentSelect( (UINT)wParam );		// 선택된 메뉴 반전

	return 0;
}

// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
LRESULT CDlgbarHReport::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{

	}
	else
	{

	}

	return 0;
}


// 표시 언어가 변경되었다.
LRESULT CDlgbarHReport::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_BTNX_HREPORT_SUBMENU1);

	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// 현재 선택된 버튼의 색을 달리해준다.
void CDlgbarHReport::DispCurrentSelect(UINT nID)
{
	// 현재 변경된 화면이 해당 버튼에 해당된다면 버튼의 색을 달리 해준다.
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_HREPORT_SUBMENU1, ((nID == eFORM_Report1) ?	GXCOLOR_ON : GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_HREPORT_SUBMENU2, ((nID == eFORM_Report2) ?	GXCOLOR_ON : GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_HREPORT_SUBMENU3, ((nID == eFORM_Report3) ?	GXCOLOR_ON : GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_HREPORT_SUBMENU4, ((nID == eFORM_Report4) ?	GXCOLOR_ON : GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_HREPORT_SUBMENU5, ((nID == eFORM_Report5) ?	GXCOLOR_ON : GXCOLOR_OFF));
}




// 지정 화면으로 폼을 변경한다.
void CDlgbarHReport::ChangeForm(int nIdx, BOOL bForce)
{
	// CMainFrame에게 Message로 호면 전환을 지시한다.
	::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_CHANGE_VIEW, (WPARAM)nIdx, (LPARAM)bForce);
	
	CMainFrame* pMain = (CMainFrame*) AfxGetMainWnd();
	pMain->SetCurPageRep(nIdx);
}


void CDlgbarHReport::OnClickBtnxSubMenu1()
{
	ChangeForm( eFORM_Report1 );	
}


void CDlgbarHReport::OnClickBtnxSubMenu2()
{
	ChangeForm( eFORM_Report2 );	
}


void CDlgbarHReport::OnClickBtnxSubMenu3()
{
	ChangeForm( eFORM_Report3 );	
}


void CDlgbarHReport::OnClickBtnxSubMenu4()
{
	ChangeForm( eFORM_Report4 );
}



void CDlgbarHReport::ClickBtnxHreportSubmenu5()
{
	ChangeForm( eFORM_Report5 );
}
