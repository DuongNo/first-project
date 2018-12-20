// DlgbarHAlarm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgbarHAlarm.h"
#include "afxdialogex.h"

#include "..\..\GUIDefine.h"

//GxGUIFrame 사용
#include "GausGUI\GxGUIFrame.h"
#include "GausGUI\GxUICtrl.h"



// CDlgbarHAlarm 대화 상자입니다.


////////////////////////////////	[W] 알람 소메뉴 클래스 추가 160920



IMPLEMENT_DYNAMIC(CDlgbarHAlarm, CDialogBar)

CDlgbarHAlarm::CDlgbarHAlarm()
{

}

CDlgbarHAlarm::~CDlgbarHAlarm() 
{
}

void CDlgbarHAlarm::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
}   


BEGIN_MESSAGE_MAP(CDlgbarHAlarm, CDialogBar)
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_VIEW_END,	OnChangeViewEnd)			// 메인 화면이 변경 완료되었다.
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CDlgbarHAlarm, CDialogBar)
	ON_EVENT(CDlgbarHAlarm, IDC_BTNX_HALARM_SUBMENU1, DISPID_CLICK, CDlgbarHAlarm::OnClickBtnxSubMenu1, VTS_NONE)
	ON_EVENT(CDlgbarHAlarm, IDC_BTNX_HALARM_SUBMENU2, DISPID_CLICK, CDlgbarHAlarm::OnClickBtnxSubMenu2, VTS_NONE)
	ON_EVENT(CDlgbarHAlarm, IDC_BTNX_HALARM_SUBMENU3, DISPID_CLICK, CDlgbarHAlarm::OnClickBtnxSubMenu3, VTS_NONE)
	ON_EVENT(CDlgbarHAlarm, IDC_BTNX_HALARM_SUBMENU4, DISPID_CLICK, CDlgbarHAlarm::OnClickBtnxSubMenu4, VTS_NONE)
END_EVENTSINK_MAP()
// CDlgbarHAlarm 메시지 처리기입니다.




// 메인 메뉴 선택 화면 전환 완료
LRESULT CDlgbarHAlarm::OnChangeViewEnd(WPARAM wParam, LPARAM lParam)
{
	//DispCurrentSelect( (UINT)wParam );		// 선택된 메뉴 반전

	return 0;
}

// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
LRESULT CDlgbarHAlarm::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
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
LRESULT CDlgbarHAlarm::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_BTNX_HALARM_SUBMENU1);

	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// 현재 선택된 버튼의 색을 달리해준다.
void CDlgbarHAlarm::DispCurrentSelect(UINT nID)
{
	// 현재 변경된 화면이 해당 버튼에 해당된다면 버튼의 색을 달리 해준다.
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_HALARM_SUBMENU1, ((nID == eFORM_Alarm1) ?	GXCOLOR_ON : GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_HALARM_SUBMENU2, ((nID == eFORM_Alarm2) ?	GXCOLOR_ON : GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_HALARM_SUBMENU3, ((nID == eFORM_Alarm3) ?	GXCOLOR_ON : GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_HALARM_SUBMENU4, ((nID == eFORM_Alarm4) ?	GXCOLOR_ON : GXCOLOR_OFF));
}




// 지정 화면으로 폼을 변경한다.
void CDlgbarHAlarm::ChangeForm(int nIdx, BOOL bForce)
{
	// CMainFrame에게 Message로 호면 전환을 지시한다.
	::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_CHANGE_VIEW, (WPARAM)nIdx, (LPARAM)bForce);
}


void CDlgbarHAlarm::OnClickBtnxSubMenu1()
{
	ChangeForm( eFORM_Alarm1 );
}


void CDlgbarHAlarm::OnClickBtnxSubMenu2()
{
	ChangeForm( eFORM_Alarm2 );
}


void CDlgbarHAlarm::OnClickBtnxSubMenu3()
{
	ChangeForm( eFORM_Alarm3 );
}


void CDlgbarHAlarm::OnClickBtnxSubMenu4()
{
	ChangeForm( eFORM_Alarm4 );
}




