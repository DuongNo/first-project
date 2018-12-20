// DlgBarHTeach1.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgBarHTeach2.h"


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


// CDlgBarHTeach1 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgBarHTeach2, CDialogBar)

CDlgBarHTeach2::CDlgBarHTeach2()
{

}

CDlgBarHTeach2::~CDlgBarHTeach2()
{
}

void CDlgBarHTeach2::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgBarHTeach2, CDialogBar)
	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_VIEW_END,	OnChangeViewEnd)			// 메인 화면이 변경 완료되었다.
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
END_MESSAGE_MAP()


// Active-X Event 처리
BEGIN_EVENTSINK_MAP(CDlgBarHTeach2, CDialogBar)
	ON_EVENT(CDlgBarHTeach2, IDC_BTNX_HTEACH2_WORKTABLE, DISPID_CLICK, CDlgBarHTeach2::OnClickBtnxWorkTable, VTS_NONE)
	ON_EVENT(CDlgBarHTeach2, IDC_BTNX_HTEACH2_UDTR_UDSTAGE, DISPID_CLICK, CDlgBarHTeach2::OnClickBtnx_UD_TrStage, VTS_NONE)
	ON_EVENT(CDlgBarHTeach2, IDC_BTNX_HTEACH2_UDCONVTR, DISPID_CLICK, CDlgBarHTeach2::OnClickBtnx_UDConv, VTS_NONE)
	ON_EVENT(CDlgBarHTeach2, IDC_BTNX_HTEACH2_INSPECTION_UNIT, DISPID_CLICK, CDlgBarHTeach2::OnClickBtn_InspectionUnit, VTS_NONE)
END_EVENTSINK_MAP()

// CDlgBarHTeach1 메시지 처리기입니다.



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
LRESULT CDlgBarHTeach2::OnChangeViewEnd(WPARAM wParam, LPARAM lParam)
{
	DispCurrentSelect( (UINT)wParam );		// 선택된 메뉴 반전

	return 0;
}

// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
LRESULT CDlgBarHTeach2::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
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
LRESULT CDlgBarHTeach2::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_BTNX_HTEACH2_WORKTABLE);
	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 지정 화면으로 폼을 변경한다.
void CDlgBarHTeach2::ChangeForm(int nIdx, BOOL bForce)
{
	// CMainFrame에게 Message로 호면 전환을 지시한다.
	::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_CHANGE_VIEW, (WPARAM)nIdx, (LPARAM)bForce);

	CMainFrame* pMain = (CMainFrame*) AfxGetMainWnd();
	pMain->SetCurPageTea2(nIdx);
}

// 현재 선택된 버튼의 색을 달리해준다.
void CDlgBarHTeach2::DispCurrentSelect(UINT nID)
{
	// 현재 변경된 화면이 해당 버튼에 해당된다면 버튼의 색을 달리 해준다.
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_HTEACH2_WORKTABLE, ((nID == eFORM_Teach2_WkTable) ?	GXCOLOR_ON : GXCOLOR_OFF));	
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_HTEACH2_UDTR_UDSTAGE, ((nID == eFORM_Teach2_UD_TrStg) ?	GXCOLOR_ON : GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_HTEACH2_UDCONVTR, ((nID == eFORM_Teach2_UD_Conv) ?	GXCOLOR_ON : GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_HTEACH2_INSPECTION_UNIT, ((nID == eFORM_Teach2_Insp) ?	GXCOLOR_ON : GXCOLOR_OFF));
}




// 하위 메뉴 1 선택
void CDlgBarHTeach2::OnClickBtnxWorkTable()
{
	ChangeForm( eFORM_Teach2_WkTable );
}


void CDlgBarHTeach2::OnClickBtnx_UD_TrStage()
{
	ChangeForm( eFORM_Teach2_UD_TrStg );	
}


void CDlgBarHTeach2::OnClickBtnx_UDConv()
{
	ChangeForm( eFORM_Teach2_UD_Conv );
}


void CDlgBarHTeach2::OnClickBtn_InspectionUnit()
{
	ChangeForm( eFORM_Teach2_Insp );
}
