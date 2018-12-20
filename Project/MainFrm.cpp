
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "CheetahApp.h"
#include "MainFrm.h"


#include "UI\GausGUI\GxUICtrl.h"

// DLL에 존재하는 각종 Dialog 사용
#include "UI\GausGUI\GxNumPad.h"
#include "UI\GausGUI\GxNumPadEx.h"
#include "UI\GausGUI\GxMsgBox.h"


// 관리할 FormView / Dialog header file
#include "GUIDefine.h"									// 화면 Index 정의


#include "UI\Form\FormMain.h"
#include "UI\Form\FormPM.h"
#include "UI\Form\FormReport1.h"
#include "UI\Form\FormReport2.h"
#include "UI\Form\FormReport3.h"
#include "UI\Form\FormReport4.h"
#include "UI\Form\FormReport5.h"
#include "UI\Form\FormSignal.h"
#include "UI\Form\FormSetupIO.h"
#include "UI\Form\FormInitialize.h"
#include "UI\Form\FormMotorUnitTest.h"
#include "UI\Dialog\DlgbarJog.h"
#include "UI\Dialog\DlgbarBottom.h"
#include "UI\Dialog\DlgbarTop.h"
#include "UI\Dialog\DlgbarHReport.h"			// 상단 서브 메뉴
#include "UI\Dialog\DlgbarHSetup.h"
#include "UI\Dialog\DlgbarHTeach.h"
#include "UI\Dialog\DlgConfirm.h"
#include "UI\Dialog\DlgInform.h"
#include "UI\Dialog\DlgInitial.h"
#include "UI\Dialog\DlgMsgInterlock.h"
#include "UI\Dialog\DlgMsgOPCall.h"
#include "UI\Dialog\DlgMsgTerminal.h"
#include "UI\Form\FormAlarm.h"
#include "UI\Form\FormRecipe.h"
#include "UI\Dialog\DlgbarHConfig.h"
#include "UI\Form\FormAlarmLamp.h"
#include "UI\Form\FormConfig2.h"
#include "UI\Dialog\DlgBarHTeach2.h"
#include "UI\Form\FormOptions.h"
#include "UI\Dialog\DlgDoor.h"

#include "UI\Form\FormTeachLoader.h"
#include "UI\Form\FormTeachPreAlign.h"
#include "UI\Form\FormTeachLoaderTR.h"
#include "UI\Form\FormTeachTurnTable.h"
#include "UI\Form\FormTeachUnloaderTR.h"
#include "UI\Form\FormTeachGoodTray.h"
#include "UI\Form\FormTeachNGTray.h"
#include "UI\Form\FormTeachLDRobot.h"
#include "UI\Form\FormTeachUDRobot.h"
#include "UI\Form\FormOLBDown.h"

#include "UI\Dialog\DlgJobChange.h"
#include "UI\Dialog\DlgTMDDownLoad.h"

#include "UI\Dialog\DlgLossTime2.h"
#include "UI\Dialog\DlgStatement.h"  //셀로그 자료 정리 화면

#include "UI\Dialog\DlgVersion.h"
#include "UI\Dialog\DlgAGVMsg.h"
#include "UI\Dialog\DlgMsgPortEmpty.h"

#include "UI\Dialog\DlgRetryInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ACTIVATE()
	ON_WM_CLOSE()

	// 사용자 정의 메세지 처리
	ON_MESSAGE(GxMSG_GUI_CHANGE_VIEW,		OnViewChange)					// 메인 화면을 변경하라는 지령
	ON_MESSAGE(GxMSG_GUI_CHANGE_VIEW_END,	OnViewChangeEnd)				// 메인 화면이 변경되었다.
	ON_MESSAGE(GxMSG_GUI_REQUEST_LANGUAGE,	OnChangeRequestLanguage)	// 표시 언어를 바꾸라는 요청
	//kjpark 20160919 Topdlg에 connect status 
	ON_MESSAGE(UDMSG_COMM_STATE,			OnChangeCommState)					//  통신 상태가 변경되었다.
	//20160930 kjpark Serial Data 메세지 방식 구현
	ON_MESSAGE(WM_SERIAL,	OnSerialMsg)
	//kjpark 20161019 MainFrame timer 추가 - 인터락, 타워램프 컨트롤
	ON_WM_TIMER()

	ON_MESSAGE(WM_UPDATE_LOWER_MCRCONNECT,	OnUpdateLOWERMCRConnect)		// MCR이 연결/해제 되었다.
	ON_MESSAGE(WM_UPDATE_LOWER_MCRDATA,		OnUpdateLOWERMCRData)			// MCR이 수신되었다.
	ON_MESSAGE(WM_UPDATE_LOWER_MCRLIVE,		OnUpdateLOWERMCRLive)			// Live 화면이이 수신되었다.
	ON_MESSAGE(WM_UPDATE_UPPER_MCRCONNECT,	OnUpdateUPPERMCRConnect)		// MCR이 연결/해제 되었다.
	ON_MESSAGE(WM_UPDATE_UPPER_MCRDATA,		OnUpdateUPPERMCRData)			// MCR이 수신되었다.
	ON_MESSAGE(WM_UPDATE_UPPER_MCRLIVE,		OnUpdateUPPERMCRLive)			// Live 화면이이 수신되었다.
	//20170118 byskcho
	ON_MESSAGE(GxMSG_GUI_SHOW_DLG,			OnDialogView)					// 지정 Dialog를 보여주거나 감춘다.
	ON_MESSAGE(UDMSG_TMDDOWNLOAD_UPDATE,			OnTMDDownLoad)

END_MESSAGE_MAP()

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
	m_bOnlyOnce			= TRUE;
	m_bTerminateFlag	= FALSE;		// 프로그램을 종료해도 되는지 여부
	m_bModePM			= FALSE;		// PM 모드 여부.

	//kjpark 20161021 Setup 신호기 UI 구현
	m_nLangIdx			= eLANG_KOREAN;	
													// 표시되는 언어 인덱스, 0:한국어, 1:영어, 2:베트남어 ...
	m_SetValue_Flag = FALSE; // 레시피 창에서 모델편집 버튼 눌렀는지 여부
	m_iCurPageRep = eFORM_Report1;
	m_iCurPageSet = eFORM_Setup1;
	m_iCurPageTea1 = eFORM_Teach1_Loader;
	m_iCurPageCon = eFORM_Config1;

	m_iSelectedAxis = AXIS_ID_MAX;
	m_iTeachAxis = TEACH_PARAM::PARAM_MAX;
	m_dTeachOffset = 0.;

	//kjpark 20161019 MainFrame timer 추가 - 인터락, 타워램프 컨트롤
	m_bTeachMode = FALSE;

	m_bLampR_FLick = TRUE;
	m_bLampY_FLick = TRUE;
	m_bLampG_FLick = TRUE;

	
}


CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// Set Frame size & Remove Menu
	cs.x			= 0;											// 화면 시작위치
	cs.y			= 0;
	cs.cx			= 1280;											// 화면 크기
	cs.cy			= 1024;
	cs.hMenu		= NULL;											// 메뉴 제거
	// OLD cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE | WS_THICKFRAME;
	cs.style		= WS_OVERLAPPED | WS_POPUP;				// 윈도우 테두리 및 타이틀바를 없앤다.
	cs.lpszName	= _T("YW Cheetah");							// 타이틀바에 표시되는 프로그램 이름
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.


	// Icon 변경
	HICON hIcon = AfxGetApp()->LoadIcon(IDI_ICON_DEFAULT);	// IDR_MAINFRAME); 				// Icon
	SetIcon(hIcon, FALSE);

	// 3D 효과를 제거하여 완전한 Client 영역을 확보한다.
	ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED); 


	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	
	// GxGUIFrame객체를 생성한다.
	m_pGUIFrame = CGxGUIFrame::GetInstance();

	// Dialogbar를 추가한다.
	m_pGUIFrame->AddDlgBar( this, eDLGBAR_Top,		CDlgbarTop::IDD,		new CDlgbarTop(),			CGxGUIFrame::ePOS_DlgBarTop);		// 상위 메뉴
	m_pGUIFrame->AddDlgBar( this, eDLGBAR_Bottom,	CDlgbarBottom::IDD,		new CDlgbarBottom(),		CGxGUIFrame::ePOS_DlgBarBottom);	// 하단 메뉴

	// 왼쪽 측면 서브 메뉴 예
	// m_pGUIFrame->AddDlgBar( this, eDLGBAR_SubReport,CDlgbarVReport::IDD,	new CDlgbarVReport(),	CGxGUIFrame::ePOS_DlgBarLeft);	// Report 하위 메뉴 선택 (왼쪽 메뉴)

	//상단면 서브 메뉴 예
	m_pGUIFrame->AddDlgBar( this, eDLGBAR_SubReport,CDlgbarHReport::IDD,	new CDlgbarHReport(),		CGxGUIFrame::ePOS_DlgBarTop);		// Report 하위 메뉴 선택 (상부 메뉴)
	m_pGUIFrame->AddDlgBar( this, eDLGBAR_SubTeach, CDlgbarHTeach::IDD,		new CDlgbarHTeach(),		CGxGUIFrame::ePOS_DlgBarTop);		// Teaching 하위 메뉴 선택
	m_pGUIFrame->AddDlgBar( this, eDLGBAR_SubSetup, CDlgbarHSetup::IDD,		new CDlgbarHSetup(),		CGxGUIFrame::ePOS_DlgBarTop);		// Setup 하위 메뉴 선택
	m_pGUIFrame->AddDlgBar( this, eDLGBAR_SubConfig,CDlgbarHConfig::IDD,	new	CDlgbarHConfig(),		CGxGUIFrame::ePOS_DlgBarTop);		// Config 하위 메뉴 선택 [W]
	m_pGUIFrame->AddDlgBar( this, eDLGBAR_Jog,		CDlgbarJog::IDD,		new CDlgbarJog(),			CGxGUIFrame::ePOS_DlgBarRight);	// Jog 메뉴 선택

	// m_pGUIFrame->AssignDefaultView() 함수는 OnActivate() 함수에서 호출해준다.
	m_pGUIFrame->AddView(this, eFORM_Main,		new CFormMain(),		eDLGBAR_None,			eDLGBAR_None,	TRUE);
	m_pGUIFrame->AddView(this, eFORM_Alarm,		new CFormAlarm(),		eDLGBAR_None,			eDLGBAR_None,	TRUE);
	m_pGUIFrame->AddView(this, eFORM_PM,		new CFormPM(),			eDLGBAR_None,			eDLGBAR_None,	TRUE);	
	m_pGUIFrame->AddView(this, eFORM_OLBDOWN,		new CFormOLBDown(),			eDLGBAR_None,			eDLGBAR_None,	TRUE);	
	m_pGUIFrame->AddView(this, eFORM_Recipe,	new CFormRecipe(),		eDLGBAR_None,			eDLGBAR_None,	TRUE);	//	[W]	레시피 페이지

	m_pGUIFrame->AddView(this, eFORM_Alarm,	new CFormAlarm(),		eDLGBAR_None,		eDLGBAR_None,	TRUE);	// [W] 알람 페이지
	//2017018 byskcho
	m_pGUIFrame->AddDialog(this, eDLG_Losstime,			CDlgLosstime2::IDD,		new CDlgLosstime2());			// 비가동 원인 입력

	m_pGUIFrame->AddView(this, eFORM_Report1,	new CFormReport1(),		eDLGBAR_SubReport,		eDLGBAR_None,	TRUE);	// 하위 메뉴 사용, 부가 기능은 없음
	m_pGUIFrame->AddView(this, eFORM_Report2,	new CFormReport2(),		eDLGBAR_SubReport,		eDLGBAR_None,	TRUE);
	m_pGUIFrame->AddView(this, eFORM_Report3,	new CFormReport3(),		eDLGBAR_SubReport,		eDLGBAR_None,	TRUE);
	m_pGUIFrame->AddView(this, eFORM_Report4,	new CFormReport4(),		eDLGBAR_SubReport,		eDLGBAR_None,	TRUE);
	m_pGUIFrame->AddView(this, eFORM_Report5,	new CFormReport5(),		eDLGBAR_SubReport,		eDLGBAR_None,	TRUE);

	m_pGUIFrame->AddView(this, eFORM_Teach1_Loader,			new CFormTeachLoader(),			eDLGBAR_SubTeach,		eDLGBAR_Jog,	TRUE);
	m_pGUIFrame->AddView(this, eFORM_Teach1_PreAlign,		new CFormTeachPreAlign(),		eDLGBAR_SubTeach,		eDLGBAR_Jog,	TRUE);	// 하위 메뉴 사용, 부가 기능도 사용
	m_pGUIFrame->AddView(this, eFORM_Teach1_LoaderTR,		new CFormTeachLoaderTR(),		eDLGBAR_SubTeach,		eDLGBAR_Jog,	TRUE);
	m_pGUIFrame->AddView(this, eFORM_Teach1_TurnTable,		new CFormTeachTurnTable(),		eDLGBAR_SubTeach,		eDLGBAR_Jog,	TRUE);
	m_pGUIFrame->AddView(this, eFORM_Teach1_UnloaderTR,		new CFormTeachUnloaderTR(),		eDLGBAR_SubTeach,		eDLGBAR_Jog,	TRUE);
	m_pGUIFrame->AddView(this, eFORM_Teach1_GoodTray,		new CFormTeachGoodTray(),		eDLGBAR_SubTeach,		eDLGBAR_Jog,	TRUE);
	m_pGUIFrame->AddView(this, eFORM_Teach1_NGTray,			new CFormTeachNGTray(),			eDLGBAR_SubTeach,		eDLGBAR_Jog,	TRUE);
	m_pGUIFrame->AddView(this, eFORM_Teach1_LDRobot,		new CFormTeachLDRobot(),		eDLGBAR_SubTeach,		eDLGBAR_None,	TRUE);
	m_pGUIFrame->AddView(this, eFORM_Teach1_UDRobot,		new CFormTeachUDRobot(),		eDLGBAR_SubTeach,		eDLGBAR_None,	TRUE);

	m_pGUIFrame->AddView(this, eFORM_Setup1,	new CFormSetupIO(),		eDLGBAR_SubSetup,		eDLGBAR_None,	TRUE);
	m_pGUIFrame->AddView(this, eFORM_Setup2,	new CFormMotorUnitTest(),		eDLGBAR_SubSetup,		eDLGBAR_None,	TRUE);
	m_pGUIFrame->AddView(this, eFORM_Setup3,	new CFormSignal(),	eDLGBAR_SubSetup,		eDLGBAR_None,	TRUE);
	m_pGUIFrame->AddView(this, eFORM_Setup4,	new CFormInitialize(),		eDLGBAR_SubSetup,		eDLGBAR_None,	TRUE);

	m_pGUIFrame->AddView(this, eFORM_Config1,	new CFormAlarmLamp(),		eDLGBAR_SubConfig,		eDLGBAR_None,	TRUE);	//[W]
	m_pGUIFrame->AddView(this, eFORM_Config2,	new CFormOptions(),			eDLGBAR_SubConfig,		eDLGBAR_None,	TRUE);	//[W]
	m_pGUIFrame->AddView(this, eFORM_Config3,	new CFormConfig2(),		eDLGBAR_SubConfig,		eDLGBAR_None,	TRUE);
	


	// Dialog 추가
	m_pGUIFrame->AddDialog(this, eDLG_Confirm,			CDlgConfirm::IDD,			new CDlgConfirm());
	m_pGUIFrame->AddDialog(this, eDLG_Inform,			CDlgInform::IDD,			new CDlgInform());
	m_pGUIFrame->AddDialog(this, eDLG_Initial,			CDlgInitial::IDD,			new CDlgInitial());
	m_pGUIFrame->AddDialog(this, eDLG_MsgInterlock,		CDlgMsgInterlock::IDD,		new CDlgMsgInterlock());
	m_pGUIFrame->AddDialog(this, eDLG_MsgOPCall,		CDlgMsgOPCall::IDD,			new CDlgMsgOPCall());
	m_pGUIFrame->AddDialog(this, eDLG_MsgTerminal,		CDlgMsgTerminal::IDD,		new CDlgMsgTerminal());
	

	m_pGUIFrame->AddDialog(this, eDLG_JobChange,		CDlgJobChange::IDD,		new CDlgJobChange());//2017-03-06,skcho JobChange Dialog 추가
	m_pGUIFrame->AddDialog(this, eDLG_MsgTMDDownLoad,		CDlgTMDDownLoad::IDD,		new CDlgTMDDownLoad());//2017-03-08,skcho TMD DownLoad Dialog 추가

	m_pGUIFrame->AddDialog(this, eDLG_Statement,		CDlgStatement::IDD,		new CDlgStatement());//2017-04-18,skcho, 셀로그 요약화면 추가
	
	//2017-12-02,SKCHO
	m_pGUIFrame->AddDialog(this, eDLG_RETRYINFO,		CDlgRetryInfo::IDD,		new CDlgRetryInfo());//2017-04-18,skcho, 셀로그 요약화면 추가
	
	
	m_pGUIFrame->AddDialog(this, eDLG_VersionHistory,		CDlgVersion::IDD,		new CDlgVersion());//2017-07-04,  버전 이력화면 추가

	m_pGUIFrame->AddDialog(this, eDLG_AGVMsg,		CDlgAGVMsg::IDD,		new CDlgAGVMsg());//2017-09-01, agv 요청 알람 메시지 추가
	
	m_pGUIFrame->AddDialog(this, eDLG_PortEmptyMsg,		CDlgMsgPortEmpty::IDD,		new CDlgMsgPortEmpty());//2017-09-01, agv 요청 알람 메시지 추가
	// 생성된 기본적인 Dialogbar를 화면에 보여준다.
	m_pGUIFrame->ShowDlgBar( eDLGBAR_Top,		0 );				// 상부 Top menu
	m_pGUIFrame->ShowDlgBar( eDLGBAR_Bottom,	0 );				// 하단 Bottom Menu

	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	//kjpark 20161019 MainFrame timer 추가 - 인터락, 타워램프 컨트롤
	SetTimer(TMID_FLICK_LAMP,400,NULL);

	return 0;
}


void CMainFrame::OnDestroy()
{
	CFrameWnd::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// 생성하였던 GUI 관리용 객체를 파괴한다.
	m_pGUIFrame->ReleaseInstance();
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
}


void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CFrameWnd::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	// 최초 1회 실행
	if ( m_bOnlyOnce ) {
		m_bOnlyOnce = FALSE;

		// Start of GxGUI -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
		// 기본 View 지정
		m_pGUIFrame->AssignDefaultView();		// 기본적으로 생성되는 FormView Pointer를 획득한다.
		m_pGUIFrame->ChangeView(eFORM_Main, FALSE);	// 프로그램 처음 시작하면 Main 화면을 표시한다 [7/5/2016 OSC]
		// end of GxGUI -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

		SetMainWnd();

		m_pIDReader[LOWER_MCR] = new CGxIDReaderWrapper();				// 생성
		if ( m_pIDReader[LOWER_MCR] )
		{
			m_pIDReader[LOWER_MCR]->SetEventHandle(GetMainWnd(), WM_UPDATE_LOWER_MCRCONNECT, WM_UPDATE_LOWER_MCRDATA, WM_UPDATE_LOWER_MCRLIVE);
			m_pIDReader[LOWER_MCR]->SetAddress(LOWER_MCR_CAMERA_IP);				// IP 주소			
			m_pIDReader[LOWER_MCR]->SetLiveImageName(LOWER_MCRPath);	
		}

		m_pIDReader[UPPER_MCR] = new CGxIDReaderWrapper();				// 생성
		if ( m_pIDReader[UPPER_MCR] )
		{
			m_pIDReader[UPPER_MCR]->SetEventHandle(GetMainWnd(), WM_UPDATE_UPPER_MCRCONNECT, WM_UPDATE_UPPER_MCRDATA, WM_UPDATE_UPPER_MCRLIVE);
			m_pIDReader[UPPER_MCR]->SetAddress(UPPER_MCR_CAMERA_IP);				// IP 주소			
			m_pIDReader[UPPER_MCR]->SetLiveImageName(UPPER_MCRPath);	
		}
	}

}


void CMainFrame::OnClose()
{
	// 종료 여부를 아직 확인받지 않았다면
	if ( m_bTerminateFlag != TRUE )
	{
		// int nRet = AfxMessageBox(_T("Are you sure you want to exit ?"), MB_YESNO);
		//if ( nRet != IDYES ) return;		// 종료 취소

		CGxMsgBox	dlgMsgBox;
	
		//참고 dlgMsgBox.SetLangName(2, _T("CHN"));		// 해당 순번 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("시스템 종료"), _T("System Terminate"), _T("chấm dứt chương trình"));
		dlgMsgBox.SetMessage(TRUE, 
						_T("프로그램을 종료하시겠습니까 ?"), 
						_T("Do you want to terminate program ?"),		
						_T("Bạn có muốn chấm dứt chương trình ?"), m_nLangIdx );

		int nRet = dlgMsgBox.DoModal();

		// 종료를 원하지 않는다면 종료 처리 중지
		if ( nRet != IDOK ) return;

		// 
		m_bTerminateFlag = TRUE;			// 종료 가능

		//////////////////////////////////////////////////////////////////////////
		// Release Thread
		for(int i = 0; i < THREAD_INDEX_EVENT_VT_FILE; i++)
			theThreadInitializer.ReleaseThread( (THREAD_INDEX)i );
		//////////////////////////////////////////////////////////////////////////
		// Close Device
		theDeviceMotion.Close_Mp2100();
		theDeviceMotion.Close_NMC2();
		theDeviceIO.Close();
		theDeviceMelsec.Close(TRUE);
		theDeviceMelsec.Close(FALSE);
		//////////////////////////////////////////////////////////////////////////
		KillTimer(TMID_1000);
		KillTimer(TMID_CHECK);
		
		if(m_pIDReader[LOWER_MCR])
		{
			delete m_pIDReader[LOWER_MCR];
			m_pIDReader[LOWER_MCR] =NULL;
		}
		if(m_pIDReader[UPPER_MCR])
		{
			delete m_pIDReader[UPPER_MCR];
			m_pIDReader[UPPER_MCR] =NULL;
		}


		// Delete User List
		CString strKey;
		CUserAccount* pUsr = NULL;
		POSITION pos = m_mapUserAccount.GetStartPosition();
		while(pos)
		{
			m_mapUserAccount.GetNextAssoc(pos, strKey, pUsr);
			delete pUsr;
			pUsr = NULL;
		}
		m_mapUserAccount.RemoveAll();
	}

	CFrameWnd::OnClose();
}



// Message를 통해 화면 변경 요청이 들어왔다.
//
// wParam = MainMenu Select Button 
// lParam = SubMenu Select Button, 무시
//
LRESULT CMainFrame::OnViewChange(WPARAM wParam, LPARAM lParam)
{
	UINT nIdx	= wParam;					// 변경하고자 하는 화면 Index
	BOOL bForce = (BOOL)lParam;				// 강제로 화면을 변경할 것인가 ?

	if ( m_pGUIFrame )
	{
		m_pGUIFrame->ChangeView( nIdx, bForce );			// 화면 변경 명령
		m_SetValue_Flag= FALSE;
	}

	return 0;
}

// Message를 통해 화면 변경 요청에 대한 수행 완료가 들어왔다.
//
// wParam = MainMenu Select Button 
// lParam = SubMenu Select Button, 무시
//
LRESULT CMainFrame::OnViewChangeEnd(WPARAM wParam, LPARAM lParam)
{
	UINT nIdx	= wParam;					// 변경하고자 하는 화면 Index
	BOOL bForce = (BOOL)lParam;			// 강제로 화면을 변경할 것인가 ?

	if ( m_pGUIFrame )
	{
		// 메인/Sub 메뉴를 가지고 있는 모든 Dialog Bar에게 메뉴가 바뀌었음을 알려준다.
		m_pGUIFrame->SendMessageDlgBar(TRUE, GxMSG_GUI_CHANGE_VIEW_END, wParam, lParam);		// 지정 DialogBar에게 지정 Message를 보낸다.
	}

	return 0;
}


// 화면 표시 언어를 변경하도록 요청받았다. 
LRESULT CMainFrame::OnChangeRequestLanguage(WPARAM wParam, LPARAM lParam)
{
	// 등록된 모든 화면에 언어 변경 요청을 전송한다.
	if ( m_pGUIFrame )
	{
		m_pGUIFrame->SendMessageDialog(TRUE, GxMSG_GUI_CHANGE_LANGUAGE, wParam, lParam);		// Dialog들에게 표시 언어 변경을 통보한다.
		m_pGUIFrame->SendMessageDlgBar(TRUE, GxMSG_GUI_CHANGE_LANGUAGE, wParam, lParam);		// Dialog Bar 들에게 표시 언어 변경을 통보한다.
		m_pGUIFrame->SendMessageView(TRUE,	GxMSG_GUI_CHANGE_LANGUAGE, wParam, lParam);			// FormView들에게 표시 언어 변경을 통보한다.
	}

	return 0;
}

// 통신 상태가 변경되었다.
//kjpark 20160919 Topdlg에 connect status 
LRESULT CMainFrame::OnChangeCommState(WPARAM wParam, LPARAM lParam)
{
	// 각 Form View에게 변경 event을 통보한다.

	if ( m_pGUIFrame )
	{
		m_pGUIFrame->SendMessageDlgBar(eDLGBAR_Top, UDMSG_COMM_STATE, wParam, lParam);		// 상위 상태바에 변경 내용을 적용한다.
	}

	// MCR 재연결 할 때 디스커넥트 할 때 알람 안뜨도록 한다. 17/04/25 HSK
	if(wParam == 3 || wParam == 4)
	{
		if(theProcBank.ManualMCRReConnect && (lParam == FALSE))
		{
			theProcBank.ManualMCRReConnect = FALSE;
			return 0;
		}
	}

	int nCommID		= (int) wParam;			// 어떤 통신이 변경되었나 ?
	int nState		= (int) lParam;			// 어떠한 상태인가 ?
	int nErrCode	= ALM_NONE;				// 초기값은 Alarm 없음.

	if ( nState == 0 )							// 끊어졌을 경우 Alarm 발생
	{
		switch ( nCommID )
		{
			case eCOMM_SignalPC :		nErrCode = ALARM_COMM_SIG;			break;		
			case eCOMM_DataPC :			nErrCode = ALARM_COMM_DATA;			break;		
			case eCOMM_Lower_UVMCR :	nErrCode = ALARM_COMM_LOWER_MCR;	break;		
			case eCOMM_Upper_UVMCR :	nErrCode = ALARM_COMM_UPPER_MCR;	break;		
			case eCOMM_AVCamera1 :		nErrCode = ALARM_COMM_ACTIVE1;		break;		
			case eCOMM_AVCamera2 :		nErrCode = ALARM_COMM_ACTIVE2;		break;		
			case eCOMM_MITAlign :		nErrCode = ALARM_COMM_PREALIGN;		break;		
			case eCOMM_MITEdgeAlign :	nErrCode = ALARM_COMM_EDGEALIGN;	break;		
			case eCOMM_RobotAlign :		nErrCode = ALARM_COMM_ROBOTALIGN;	break;		
			case eCOMM_LdRobot :		nErrCode = ALARM_COMM_LDROBOT;		break;		
			case eCOMM_UdRobot :		nErrCode = ALARM_COMM_UDROBOT;		break;		
			case eCOMM_DCR :			nErrCode = ALARM_COMM_DCR;		break;		
			case eCOMM_Tray_Align :		nErrCode = ALARM_COMM_TRAYALIGN;		break;		
		}

		if ( nErrCode != ALM_NONE )				// 오류가 발생하였다면,
		{
			if(nErrCode == ALARM_COMM_LDROBOT || nErrCode == ALARM_COMM_UDROBOT
				|| nErrCode == ALARM_COMM_ROBOTALIGN)
			{
			}
			else
				theUnitFunc.SetError((ALARM_ID)nErrCode);
		}


	}//of if ( nState == 0 )					// 끊어졌을 경우 Alarm 발생

	return 0;

}


void CMainFrame::SetCurUserData(CUserAccount val)
{
	m_UserData = val;
	if(m_UserData.sID.IsEmpty())
		CGxUICtrl::SetStaticString(GetMainHandler()->m_pGUIFrame->GetDialogBar(eDLGBAR_Top), IDC_GXBTN_TOPBAR_LOGINER, _T(""));
	else
		CGxUICtrl::SetStaticString(GetMainHandler()->m_pGUIFrame->GetDialogBar(eDLGBAR_Top), IDC_GXBTN_TOPBAR_LOGINER, m_UserData.sID);
}

//---------------------------------------------------LOGIN 관련
int CMainFrame::getUserType(CString szID, CString szPW)		//	유저맵에서 ID와 PW가 있는지 체크후 존재한다면 해당 UserData의 Type을 return함.
{
	int nUserType = 0;

	CUserAccount* pUserData = NULL;


	if ( m_mapUserAccount.Lookup(szID, pUserData) == TRUE )	//유저맵에서 Find == TRUE
	{
		if (pUserData != NULL )		//	유저데이터가 존재시			
		{
			if ( pUserData->sPassword == szPW )
			{
				nUserType = pUserData->nLevel;
			}
		}			
	}

	return nUserType;
}


BOOL CMainFrame::FindUser(CString szID)
{
	CUserAccount* pUserData = NULL;

	if ( m_mapUserAccount.Lookup(szID, pUserData) == TRUE)
	{
		return TRUE;
	}

	return FALSE;

}

// UserInfo.ini를 가져와서 Map에 넣어준다.;
BOOL CMainFrame::LoadUserAccount()
{
	CString strFileName;
	//	strFileName.Format(_T("O:/Project/CYma_local/System/%s") , NAME_FILE_USER_LIST);
	strFileName.Format(_T("%s\\%s") , theConfigBank.m_System.m_strConfigPath,NAME_FILE_USER_LIST);

	// 파일이 존재하는지 점검
	if ( PathFileExists( strFileName ) == FALSE )		//  파일 존재여부
	{

		///MODIFY NEED	로그 뿌려줘야함
		//m_pLogSeq->Log(_T("[MAIN] User Account file read ERROR"));
		return FALSE;
	}
	
	CUserAccount*	pData;

	CString strUserID = _T("");			// ID
	CString strKorName = _T("");		// 한글이름
	CString strEngName = _T("");		// 영어이름
	CString strPw = _T("");				// 비밀번호
	CString strUserIDNo = _T("");		// 사원번호
	CString strUserLevel = _T("");		// 권한 ( 오퍼레이터:1 엔지니어:2 관리자:3)


	//파일을 한줄씩 읽어가면서 넣어준다.;
	// Text 파일 UniCode 방식으로 설정 변경 ( 베트남어 읽고 쓰기 가능)
	FILE *fStream;
	errno_t e = _tfopen_s(&fStream, strFileName.GetString(), _T("r,ccs=UTF-8"));	// UNICODE"));

	if (e != 0) return FALSE;

	CStdioFile		file(fStream);

	while(file.ReadString(strFileName))
	{
		if(strFileName.Left(1) == _T("!"))	continue;	// 처음 글자가 " ! " 일경우 다음 문장으로 넘어간다.;

		::AfxExtractSubString( strUserID,		strFileName	, 0, '\t');			
		::AfxExtractSubString( strKorName,		strFileName	, 2, '\t');			
		::AfxExtractSubString( strEngName,		strFileName	, 4, '\t');			
		::AfxExtractSubString( strUserIDNo,		strFileName	, 6, '\t');			
		::AfxExtractSubString( strPw,			strFileName	, 8, '\t');			
		::AfxExtractSubString( strUserLevel,	strFileName	, 10,'\t');			

		pData = new CUserAccount();			// 새로운 Data 생성

		// Data 할당		
		pData->sID		= strUserID;
		pData->sKorName		= strKorName;
		pData->sEngName	= strEngName;
		pData->sIDNo		= strUserIDNo;	// _wtoi(strUserIDNo);		// 사원번호는 절대 숫자가 아니고, 문자로 취급해야 한다.
		pData->sPassword		= strPw;
		pData->nLevel		= _wtoi(strUserLevel);

		if( strUserID == _T("") || strUserID == _T("NONE")) // ID값이 이상하면 지운다.
		{
			delete pData;
		}
		else	// 정상적인 ID라면
		{
			m_mapUserAccount.SetAt(pData->sID, pData);		// Map에 추가		
		}
	}

	file.Close();
	fclose(fStream);

	return TRUE;
}

int  CMainFrame::SerialOpen()
{
	//20160930 kjpark Serial Data 메세지 방식 구현
	int nOpenCnt = 0;	
	for(int i=SERIAL_GMS;i<SERIAL_MAX;i++)
	{
		if(!theSerialInterFace.SerialOpen((SERIAL_MODE)i))
		{
			switch(i)
			{
			case SERIAL_GMS:
				//AfxMessageBox(_T("[ GMS ] Control open fail"));
				break;
			case SERIAL_TEMPERATURE:
				//AfxMessageBox(_T("[ Temperature ] Control open fail"));
				break;
			case SERIAL_VACUUM_LD:
				//AfxMessageBox(_T("[ VACUUM Control ] open fail"));
				break;
			case SERIAL_VACUUM_INSP:
				//AfxMessageBox(_T("[ VACUUM Control ] open fail"));
				break;
			case SERIAL_VACUUM_UD:
				//AfxMessageBox(_T("[ VACUUM Control ] open fail"));
				break;
			case SERIAL_LABELPRINT:
				//AfxMessageBox(_T("[ Finger Print ] Control open fail"));
				break;
			case SERIAL_SERVOTORQUE:
				//AfxMessageBox(_T("[ Servo Torque ] Control open fail"));
				break;

			}
		}
	}//end :: for(int i=0;i<SERIAL_MAX;i++)
	
	return nOpenCnt;
}

int	 CMainFrame::SerialClose()
{
	int nCloseCnt=0;

	return nCloseCnt;
}

//kjpark 20161019 MainFrame timer 추가 - 인터락, 타워램프 컨트롤
void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	KillTimer(nIDEvent);
	//2017-10-09,SKCHO, GETINPUTIOCHECK함수에서 GETINPUTIOSTATE로 변경
	BOOL bTeachMode = theUnitFunc.GetInPutIOState(X_TEACH_KEY_SW);

	if(m_bTeachMode != bTeachMode)	
		theUnitFunc.TeachModeOn(bTeachMode);

	switch(nIDEvent)
	{
	case TMID_FLICK_LAMP:
		{
			if(m_bChkLampR_Flick == TRUE)
			{
				theUnitFunc.SetOutPutIO(Y_TOWER_LAMP_RED, m_bLampR_FLick);
				theUnitFunc.SetOutPutIO(Y_TOWER_LAMP_RED2, m_bLampR_FLick);
				m_bLampR_FLick = !m_bLampR_FLick;
			}

			if(m_bChkLampY_Flick == TRUE)
			{
				theUnitFunc.SetOutPutIO(Y_TOWER_LAMP_YELLOW, m_bLampY_FLick);
				theUnitFunc.SetOutPutIO(Y_TOWER_LAMP_YELLOW2, m_bLampY_FLick);
				m_bLampY_FLick = !m_bLampY_FLick;
			}

			if(m_bChkLampG_Flick == TRUE)
			{
				theUnitFunc.SetOutPutIO(Y_TOWER_LAMP_GREEN, m_bLampG_FLick);
				theUnitFunc.SetOutPutIO(Y_TOWER_LAMP_GREEN2, m_bLampG_FLick);
				m_bLampG_FLick = !m_bLampG_FLick;
			}

			SetTimer(TMID_FLICK_LAMP,400,NULL);
		}
		break;
	}
	

	CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::SetSelectedAxis(AXIS_ID val1, TEACH_PARAM::ID val2, double dOffset /*= 0.*/)		
{
	m_iSelectedAxis = val1; 
	m_iTeachAxis=val2; 
	m_dTeachOffset = dOffset;

	if ( m_pGUIFrame )
	{
		CDlgbarJog* pBarJog = (CDlgbarJog*)m_pGUIFrame->GetDialogBar(eDLGBAR_Jog);

		if ( pBarJog->GetSafeHwnd() == NULL )		return;
		
		pBarJog->SendMessage(CHANGE_MOTOR_AXIS );
	}
}

// MCR 연결 해제
LRESULT CMainFrame::OnUpdateLOWERMCRConnect(WPARAM wParam, LPARAM lParam)
{
	::PostMessage( this->m_hWnd, UDMSG_COMM_STATE, (WPARAM)eCOMM_Lower_UVMCR, lParam);

	return 0;
}


LRESULT CMainFrame::OnUpdateLOWERMCRData(WPARAM wParam, LPARAM lParam)
{
	if ( m_pGUIFrame )
	{		
		theUnitFunc.SaveResultImage( LOWER_MCRPath , LOWER_MCR); 
		// 제공해주는 함수에서 처리 한다 17/04/25 HSK
		//2017-05-23,skcho, mcr 데이터 채널별로 구분, 채널1에서 mcr이 타임아웃으로 fail일때 ch2 mcr reading에서 ch1 값이 들어오는 경우가 있음
		if(theProcBank.m_nMCRChannel == JIG_CH_1)
		{
			theProcBank.m_strCh1LastCellID = theUnitFunc.GetSoftTriggerData(LOWER_MCR); 
		}
		else
		{
			theProcBank.m_strCh2LastCellID = theUnitFunc.GetSoftTriggerData(LOWER_MCR); 
		}

		if ( m_pGUIFrame )
		{
			m_pGUIFrame->SendMessageView(eFORM_Teach1_LoaderTR, TRUE, WM_UPDATE_LOWER_MCRDATA, wParam, lParam);		
		}
	}

	return 0;
}

// MCR Live 영상 수신
LRESULT CMainFrame::OnUpdateLOWERMCRLive(WPARAM wParam, LPARAM lParam)
{
	if ( m_pGUIFrame )
	{
		m_pGUIFrame->SendMessageView(eFORM_Teach1_LoaderTR, TRUE, WM_UPDATE_LOWER_MCRLIVE, wParam, lParam);		
	}

	return 0;
}


// MCR 연결 해제
LRESULT CMainFrame::OnUpdateUPPERMCRConnect(WPARAM wParam, LPARAM lParam)
{
	::PostMessage( this->m_hWnd, UDMSG_COMM_STATE, (WPARAM)eCOMM_Upper_UVMCR, lParam);

	return 0;
}


LRESULT CMainFrame::OnUpdateUPPERMCRData(WPARAM wParam, LPARAM lParam)
{
	if ( m_pGUIFrame )
	{		
		theUnitFunc.SaveResultImage( UPPER_MCRPath  , UPPER_MCR); 
		
		// 제공해주는 함수에서 처리 한다 17/04/25 HSK
		//2017-05-23,skcho, mcr 데이터 채널별로 구분, 채널1에서 mcr이 타임아웃으로 fail일때 ch2 mcr reading에서 ch1 값이 들어오는 경우가 있음
		if(theProcBank.m_nMCRChannel == JIG_CH_1)
		{
			theProcBank.m_strCh1LastCellID = theUnitFunc.GetSoftTriggerData(UPPER_MCR); 
		}
		else
		{
			theProcBank.m_strCh2LastCellID = theUnitFunc.GetSoftTriggerData(UPPER_MCR); 
		}

		if ( m_pGUIFrame )
		{
			m_pGUIFrame->SendMessageView(eFORM_Teach1_LoaderTR, TRUE, WM_UPDATE_UPPER_MCRDATA, wParam, lParam);		
		}
	}

	return 0;
}

// MCR Live 영상 수신
LRESULT CMainFrame::OnUpdateUPPERMCRLive(WPARAM wParam, LPARAM lParam)
{
	if ( m_pGUIFrame )
	{
		m_pGUIFrame->SendMessageView(eFORM_Teach1_LoaderTR, TRUE, WM_UPDATE_UPPER_MCRLIVE, wParam, lParam);		
	}

	return 0;
}
// 지정 Dialog를 보여주거나 감춘다.
LRESULT CMainFrame::OnDialogView(WPARAM wParam, LPARAM lParam)
{
	UINT nIdx	= wParam;					// 변경하고자 하는 화면 Index
	BOOL bFlag  = (BOOL)lParam;			// 보여줄 것인가 TRUE, 감출것인가 ? FALSE

	if ( m_pGUIFrame )
	{
		if ( bFlag )
		{
			m_pGUIFrame->ShowDialog( nIdx );			// 보여준다
		}
		else
			m_pGUIFrame->HideDialog( nIdx );			// 감춘다.
	}

	return 0;
}
//2017-03-08, skcho, cim 프로그램에서 tmddownload신호를 받으면 PG 상태 버튼이 깜빡 거림
LRESULT CMainFrame::OnTMDDownLoad(WPARAM wParam, LPARAM lParam)
{	
	::PostMessage( this->m_hWnd, UDMSG_COMM_STATE, (WPARAM)eCOMM_TMDDownLoad, lParam);
	return 0;
}