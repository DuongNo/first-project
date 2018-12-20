
// DlgbarBottom.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgbarBottom.h"


#include "GUIDefine.h"

//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "UI\GausGUI\GxMsgBox.h"
#include "UI\Dialog\DlgBoxPassWord.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CDlgbarBottom 대화 상자입니다.
IMPLEMENT_DYNAMIC(CDlgbarBottom, CDialogBar)

CDlgbarBottom::CDlgbarBottom()
{	
}

CDlgbarBottom::~CDlgbarBottom()
{
}

void CDlgbarBottom::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgbarBottom, CDialogBar)
	// ON_BN_CLICKED(IDC_BTN_BOTTOM_MENU1, OnBtnClickMenu1)
	// ON_UPDATE_COMMAND_UI(IDC_BTN_BOTTOM_MENU1, OnUpdateCmdUI)

	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)				// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_VIEW_END,	OnChangeViewEnd)				// 메인 화면이 변경 완료되었다.
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

END_MESSAGE_MAP()


// ActivX Event 처리
BEGIN_EVENTSINK_MAP(CDlgbarBottom, CDialogBar)
	ON_EVENT(CDlgbarBottom, IDC_BTNX_BOTTOM_MENU1, DISPID_CLICK, CDlgbarBottom::OnClickBtnxBottomMenu1, VTS_NONE)
	ON_EVENT(CDlgbarBottom, IDC_BTNX_BOTTOM_MENU2, DISPID_CLICK, CDlgbarBottom::OnClickBtnxBottomMenu2, VTS_NONE)
	ON_EVENT(CDlgbarBottom, IDC_BTNX_BOTTOM_MENU3, DISPID_CLICK, CDlgbarBottom::OnClickBtnxBottomMenu3, VTS_NONE)
	ON_EVENT(CDlgbarBottom, IDC_BTNX_BOTTOM_MENU4, DISPID_CLICK, CDlgbarBottom::OnClickBtnxBottomMenu4, VTS_NONE)
	ON_EVENT(CDlgbarBottom, IDC_BTNX_BOTTOM_MENU5, DISPID_CLICK, CDlgbarBottom::OnClickBtnxBottomMenu5, VTS_NONE)
	ON_EVENT(CDlgbarBottom, IDC_BTNX_BOTTOM_MENU6, DISPID_CLICK, CDlgbarBottom::OnClickBtnxBottomMenu6, VTS_NONE)
	ON_EVENT(CDlgbarBottom, IDC_BTNX_BOTTOM_MENU7, DISPID_CLICK, CDlgbarBottom::OnClickBtnxBottomMenu7, VTS_NONE)
	ON_EVENT(CDlgbarBottom, IDC_BTNX_BOTTOM_QUIT, DISPID_CLICK, CDlgbarBottom::OnClickBtnxBottomQuit, VTS_NONE)
	ON_EVENT(CDlgbarBottom, IDC_BTNX_BOTTOM_LANG, DISPID_CLICK, CDlgbarBottom::ClickBtnxBottomLang, VTS_NONE)
END_EVENTSINK_MAP()




// CDlgbarBottom 메시지 처리기입니다.
void CDlgbarBottom::OnUpdateCmdUI(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

BOOL CDlgbarBottom::PreCreateWindow(CREATESTRUCT& cs)
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

// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
LRESULT CDlgbarBottom::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
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


// 화면 변경이 완료되었다.
LRESULT CDlgbarBottom::OnChangeViewEnd(WPARAM wParam, LPARAM lParam)
{
	m_pMain = GetMainHandler();

	DispCurrentSelect( (UINT)wParam );		// 선택된 메뉴 반전

	SetModePM();

	return 0;
}


// 표시 언어가 변경되었다.
LRESULT CDlgbarBottom::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_BTNX_BOTTOM_MENU1);

	// 국기를 표시하는 버튼은 특별히 처리를 해준다.
	CGxUICtrl::SetButtonIcon(this, IDC_BTNX_BOTTOM_LANG, wParam);	// 각 언어의 Index에 해당하는 아이콘을 표시한다.

	return 0;
}


// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


// 지정 화면으로 폼을 변경한다.
void CDlgbarBottom::ChangeForm(int nIdx, BOOL bForce)
{
	// CMainFrame에게 Message로 화면 전환을 지시한다.
	// ::PostMessage( ( (CMainFrame*)AfxGetMainWnd() )->m_hWnd, GxMSG_GUI_CHANGE_VIEW, (WPARAM)nIdx, (LPARAM)bForce);		// #include "MainFrm.h" 필요
	if(nIdx == 19)
		theProcBank.m_bMotorSetupChk = TRUE;

	::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_CHANGE_VIEW, (WPARAM)nIdx, (LPARAM)bForce);
}


// 현재 선택된 버튼의 색을 달리해준다.
void CDlgbarBottom::DispCurrentSelect(UINT nID)
{
	// 현재 변경된 화면이 해당 버튼에 해당된다면 버튼의 색을 달리 해준다.

	// 단일 화면 메뉴의 경우
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_BOTTOM_MENU1, ((nID == eFORM_Main) ?		GXCOLOR_ON : GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_BOTTOM_MENU2, ((nID == eFORM_Alarm) ?	GXCOLOR_ON : GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_BOTTOM_MENU7, ((nID == eFORM_Recipe) ?	GXCOLOR_ON : GXCOLOR_OFF));

	// 하위 메뉴가 있는 경우
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_BOTTOM_MENU3, ((nID >= eFORM_Report1 && nID <= eFORM_Report4) ? GXCOLOR_ON : GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_BOTTOM_MENU4, ((nID >= eFORM_Teach1_Loader	&& nID <= eFORM_Teach1_UDRobot) ?	GXCOLOR_ON : GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_BOTTOM_MENU5, ((nID >= eFORM_Setup1	&& nID <= eFORM_Setup4) ?	GXCOLOR_ON : GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_BOTTOM_MENU6, ((nID >= eFORM_Config1	&& nID <= eFORM_Config3) ?	GXCOLOR_ON : GXCOLOR_OFF));	//[W]


	
}



// 메인 화면 선택
void CDlgbarBottom::OnClickBtnxBottomMenu1()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CMainFrame* pMain = (CMainFrame*) AfxGetMainWnd();
	if(pMain->m_pGUIFrame->GetCurrentViewID() == eFORM_Main)
		return;

	ChangeForm( eFORM_Main );								// Main 화면
}


// Alarm 화면
void CDlgbarBottom::OnClickBtnxBottomMenu2()
{
	CMainFrame* pMain = (CMainFrame*) AfxGetMainWnd();
	if(pMain->m_pGUIFrame->GetCurrentViewID() == eFORM_Alarm)
		return;

	ChangeForm( eFORM_Alarm);							// Alarm 표시 화면
}
// Report 화면
void CDlgbarBottom::OnClickBtnxBottomMenu3()
{	
	CMainFrame* pMain = (CMainFrame*) AfxGetMainWnd();

	if(pMain->m_pGUIFrame->GetCurrentViewID() == m_pMain->GetCurPageRep())
		return;

	ChangeForm(m_pMain->GetCurPageRep());						// 각종 Report 표시 화면
}

// Teach 화면
void CDlgbarBottom::OnClickBtnxBottomMenu4()
{
	CMainFrame* pMain = (CMainFrame*) AfxGetMainWnd();
	
	// 알람 및 통계 화면을 제외하고 다른 페이지는 엔지니어 로그인 후에 들어 갈수 있다 2017/06/16 HSK.
	if(pMain->GetCurUserData().nLevel != 3)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("엔지니어 로그인 해주십시오."), 
			_T("Engineer Login, Please"),		
			_T("Engineer Login, Please") );

		dlgMsgBox.DoModal();
		return;
	}

	if(pMain->m_pGUIFrame->GetCurrentViewID() == m_pMain->GetCurPageTea1())
		return;

	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_PAUSE || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_INIT
		|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_FAULT || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_INTERLOCK
		|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_CHECK)//2018-04-26,NAM
	{
		ChangeForm(m_pMain->GetCurPageTea1());						// 모듈별 티칭 화면
	}
	else
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("설비 물류가 진행중입니다"), 
			_T("Machine is running"),		
			_T("Các trang thiết bị đang được tiến hành") );

		dlgMsgBox.DoModal();
		return;
	}
}

// Setup 화면
void CDlgbarBottom::OnClickBtnxBottomMenu5()
{
	CMainFrame* pMain = (CMainFrame*) AfxGetMainWnd();
	if(pMain->GetCurUserData().nLevel != 3)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("엔지니어 로그인 해주십시오."), 
			_T("Engineer Login, Please"),		
			_T("Engineer Login, Please") );

		dlgMsgBox.DoModal();
		return;
	}

	if(pMain->m_pGUIFrame->GetCurrentViewID() == m_pMain->GetCurPageSet())
		return;

	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_PAUSE || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_INIT
		|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_FAULT || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_INTERLOCK
		|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_CHECK)//2018-04-26,NAM
		ChangeForm(m_pMain->GetCurPageSet());						// 디바이스 설정 화면
	else
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("설비 물류가 진행중입니다"), 
			_T("Machine is running"),		
			_T("Các trang thiết bị đang được tiến hành") );

		dlgMsgBox.DoModal();
		return;
	}	
}

// Recipe 화면
void CDlgbarBottom::OnClickBtnxBottomMenu6()			
{	
	CMainFrame* pMain = (CMainFrame*) AfxGetMainWnd();
	if(pMain->GetCurUserData().nLevel != 3)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("엔지니어 로그인 해주십시오."), 
			_T("Engineer Login, Please"),		
			_T("Engineer Login, Please") );

		dlgMsgBox.DoModal();
		return;
	}

	if(pMain->m_pGUIFrame->GetCurrentViewID() == m_pMain->GetCurPageCon())
		return;

	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_PAUSE || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_INIT
		|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_FAULT || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_INTERLOCK
		|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_CHECK)//2018-04-26,NAM
		ChangeForm(m_pMain->GetCurPageCon());						//	파라미터 설정 화면
	else
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("설비 물류가 진행중입니다"), 
			_T("Machine is running"),		
			_T("Các trang thiết bị đang được tiến hành") );

		dlgMsgBox.DoModal();
		return;
	}		
}


//  화면
void CDlgbarBottom::OnClickBtnxBottomMenu7()
{
	CMainFrame* pMain = (CMainFrame*) AfxGetMainWnd();
	if(pMain->GetCurUserData().nLevel != 3)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("엔지니어 로그인 해주십시오."), 
			_T("Engineer Login, Please"),		
			_T("Engineer Login, Please") );

		dlgMsgBox.DoModal();
		return;
	}

	if(pMain->m_pGUIFrame->GetCurrentViewID() == eFORM_Recipe)
		return;

	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_PAUSE || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_INIT
		|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_FAULT || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_INTERLOCK
		|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_CHECK)//2018-04-26,NAM
		ChangeForm( eFORM_Recipe );							// 레시피 등록,편집 화면
	else
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("설비 물류가 진행중입니다"), 
			_T("Machine is running"),		
			_T("Các trang thiết bị đang được tiến hành") );

		dlgMsgBox.DoModal();
		return;
	}			
}


// 종료
void CDlgbarBottom::OnClickBtnxBottomQuit()
{
	CDlgBoxPassWord dlgKey;

	if ( dlgKey.DoModal() == IDOK )
	{
		if (dlgKey.m_strReturnText != theConfigBank.m_System.m_strPassword)
		{
		
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("틀린 비밀번호 입니다."), 
				_T("Password Wrong"),		
				_T("Một mật khẩu sai.") );

			dlgMsgBox.DoModal();
			return;
		}
	}
	else
	{
		return;
	}
	// 종료하도록 이벤트를 보낸다.
	::SendMessage(((CFrameWnd *)AfxGetMainWnd())->m_hWnd, WM_CLOSE, 0, 0);
}



// 표시되는 언어를 변경해준다.
void CDlgbarBottom::ClickBtnxBottomLang()
{
	CMainFrame *pMainFrm = (CMainFrame *)AfxGetMainWnd();

	pMainFrm->m_nLangIdx = ( pMainFrm->m_nLangIdx + 1 ) % 3;							// 표시되는 언어 인덱스, 0:한국어, 1:영어, 2:베트남어 ...

	// 표시 언어가 변경되었다고 알려준다.
	::PostMessage(pMainFrm->m_hWnd, GxMSG_GUI_REQUEST_LANGUAGE, pMainFrm->m_nLangIdx, 0);
}




/*
	// 모든 View와 Dialog에 표시 언어를 변경하도록 전파한다.
	m_pGUIData->SendMessageView(FALSE, UM_DISP_LANG_CHANGE, wParam, lParam);
	m_pGUIData->SendMessageDialog(FALSE, UM_DISP_LANG_CHANGE, wParam, lParam);
	m_pGUIData->SendMessageDialogBar(FALSE, UM_DISP_LANG_CHANGE, wParam, lParam);

	*/




void CDlgbarBottom::SetModePM()
{
	BOOL bPM = m_pMain->getModePM();

	bPM = m_pMain->getModePM() ? FALSE : TRUE;

	CGxUICtrl::SetButtonEnabled(this, IDC_BTNX_BOTTOM_MENU1, bPM);
	CGxUICtrl::SetButtonEnabled(this, IDC_BTNX_BOTTOM_MENU2, bPM);
	CGxUICtrl::SetButtonEnabled(this, IDC_BTNX_BOTTOM_MENU7, bPM);
	CGxUICtrl::SetButtonEnabled(this, IDC_BTNX_BOTTOM_MENU3, bPM);
	CGxUICtrl::SetButtonEnabled(this, IDC_BTNX_BOTTOM_MENU4, bPM);
	CGxUICtrl::SetButtonEnabled(this, IDC_BTNX_BOTTOM_MENU5, bPM);
	CGxUICtrl::SetButtonEnabled(this, IDC_BTNX_BOTTOM_MENU6, bPM);
	CGxUICtrl::SetButtonEnabled(this, IDC_BTNX_BOTTOM_MENU1, bPM);
	
}


 