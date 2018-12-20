// DlgbarHConfig.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgbarHConfig.h"

//GxGUIFrame 사용
#include "GUIDefine.h"
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "MainFrm.h"
// CDlgbarHConfig 대화 상자입니다.

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW

#endif
IMPLEMENT_DYNAMIC(CDlgbarHConfig, CDialogBar)

CDlgbarHConfig::CDlgbarHConfig()
{

}

CDlgbarHConfig::~CDlgbarHConfig()
{
}

void CDlgbarHConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgbarHConfig, CDialogBar)
	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_VIEW_END,	OnChangeViewEnd)			// 메인 화면이 변경 완료되었다.
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CDlgbarHConfig, CDialogBar)
	ON_EVENT(CDlgbarHConfig, IDC_BTNX_HCONFIG_SUBMENU1, DISPID_CLICK, CDlgbarHConfig::OnClickBtnxSubMenu1, VTS_NONE)
	ON_EVENT(CDlgbarHConfig, IDC_BTNX_HCONFIG_SUBMENU2, DISPID_CLICK, CDlgbarHConfig::OnClickBtnxSubMenu2, VTS_NONE)
	ON_EVENT(CDlgbarHConfig, IDC_BTNX_HCONFIG_OPTIONS, DISPID_CLICK, CDlgbarHConfig::ClickBtnxHconfigOptions, VTS_NONE)
END_EVENTSINK_MAP()
	

// CDlgbarHConfig 메시지 처리기입니다.
void CDlgbarHConfig::OnUpdateCmdUI(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

BOOL CDlgbarHConfig::PreCreateWindow(CREATESTRUCT& cs)
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
LRESULT CDlgbarHConfig::OnChangeViewEnd(WPARAM wParam, LPARAM lParam)
{
	DispCurrentSelect( (UINT)wParam );		// 선택된 메뉴 반전

	return 0;
}

// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
LRESULT CDlgbarHConfig::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
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
LRESULT CDlgbarHConfig::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_BTNX_HCONFIG_SUBMENU1);

	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// 현재 선택된 버튼의 색을 달리해준다.
void CDlgbarHConfig::DispCurrentSelect(UINT nID)
{
	// 현재 변경된 화면이 해당 버튼에 해당된다면 버튼의 색을 달리 해준다.
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_HCONFIG_SUBMENU1, ((nID == eFORM_Config1) ?	GXCOLOR_ON : GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_HCONFIG_OPTIONS, ((nID == eFORM_Config2) ?	GXCOLOR_ON : GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_HCONFIG_SUBMENU2, ((nID == eFORM_Config3) ?	GXCOLOR_ON : GXCOLOR_OFF));
}




// 지정 화면으로 폼을 변경한다.
void CDlgbarHConfig::ChangeForm(int nIdx, BOOL bForce)
{
	// CMainFrame에게 Message로 호면 전환을 지시한다.
	::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_CHANGE_VIEW, (WPARAM)nIdx, (LPARAM)bForce);

	CMainFrame* pMain = (CMainFrame*) AfxGetMainWnd();
	pMain->SetCurPageCon(nIdx);
}


void CDlgbarHConfig::OnClickBtnxSubMenu1()
{
	ChangeForm( eFORM_Config1 );

}


void CDlgbarHConfig::ClickBtnxHconfigOptions()
{
	ChangeForm( eFORM_Config2 );
}

void CDlgbarHConfig::OnClickBtnxSubMenu2()
{
	ChangeForm( eFORM_Config3 );
}
