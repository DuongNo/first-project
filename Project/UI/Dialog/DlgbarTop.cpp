// DlgbarTop.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgbarTop.h"

//GxGUIFrame 사용
#include "GUIDefine.h"
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "BankGuild\MCDefine.h"
#include "MainFrm.h"
#include "UI\Dialog\DlgUserLogin.h"
#include "UI\Dialog\DIgRecipeSelect.h"
#include "UI\GausGUI\GxMsgBox.h"
#include "UI\Dialog\DlgLogo.h"
#include "UI\Dialog\DlgBoxKeyBoard.h"
#include "UI\Dialog\DlgLogo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW

#endif
// CDlgbarTop 대화 상자입니다.
IMPLEMENT_DYNAMIC(CDlgbarTop, CDialogBar)

CDlgbarTop::CDlgbarTop()
{
	m_bOldData_PC = FALSE;
}

CDlgbarTop::~CDlgbarTop()
{
}

void CDlgbarTop::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgbarTop, CDialogBar)
	// ON_BN_CLICKED(IDC_BTN_BOTTOM_MENU1, OnBtnClickMenu1)
	// ON_UPDATE_COMMAND_UI(IDC_BTN_BOTTOM_MENU1, OnUpdateCmdUI)

	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()


	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_VIEW_END,	OnChangeViewEnd)			// 메인 화면이 변경 완료되었다.
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	//kjpark 20160919 Topdlg에 connect status 
	ON_MESSAGE(UDMSG_COMM_STATE,			OnChangeCommState)		//  통신 상태가 변경되었다.
	ON_MESSAGE(UDMSG_STATE_CHANGE,			OnChangeMachineState)		//  설비 상태가 변경되었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
END_MESSAGE_MAP()


// Active-X Event
BEGIN_EVENTSINK_MAP(CDlgbarTop, CDialogBar)
	ON_EVENT(CDlgbarTop, IDC_LBLX_TOPBAR_CLOCK, DISPID_CLICK, CDlgbarTop::ClickLblxTopbarClock, VTS_NONE)
	ON_EVENT(CDlgbarTop, IDC_GXBTN_TOPBAR_LOGIN, DISPID_CLICK, CDlgbarTop::ClickProductLogin, VTS_NONE)

	ON_EVENT(CDlgbarTop, IDC_GXSTC_TOPBAR_MODEL_TEXT, DISPID_CLICK, CDlgbarTop::ClickGxstcTopbarModelText, VTS_NONE)
	ON_EVENT(CDlgbarTop, IDC_GXSTC_TOPBAR_DCR, DISPID_CLICK, CDlgbarTop::ClickGxstcTopbarDcr, VTS_NONE)
	ON_EVENT(CDlgbarTop, IDC_GXSTC_TOPBAR_PG, DISPID_CLICK, CDlgbarTop::ClickGxstcTopbarPg, VTS_NONE)
	ON_EVENT(CDlgbarTop, IDC_GXSTC_TOPBAR_SLOGO, DISPID_CLICK, CDlgbarTop::ClickGxstcTopbarLogo, VTS_NONE)
	ON_EVENT(CDlgbarTop, IDC_GXSTC_TOPBAR_VER_TEXT, DISPID_CLICK, CDlgbarTop::ClickGxstcTopbarVerText, VTS_NONE)
	ON_EVENT(CDlgbarTop, IDC_GXSTC_TOPBAR_PREALIGN, DISPID_CLICK, CDlgbarTop::ClickGxstcTopbarPrealign, VTS_NONE)
END_EVENTSINK_MAP()


// CDlgbarTop 메시지 처리기입니다.
void CDlgbarTop::OnUpdateCmdUI(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
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
LRESULT CDlgbarTop::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{
		SetTimer(100, 1000, NULL);				// 시계를 표시하는 타이머를 구동시켜준다.
	}
	else
	{
		KillTimer(100);							// 시계 표시 타이머 중지
	}

	return 0;
}

// 메인 메뉴 선택 화면 전환 완료
LRESULT CDlgbarTop::OnChangeViewEnd(WPARAM wParam, LPARAM lParam)
{
	// ToDo : 화면을 표시하기전에 하위 메뉴에 대한 동작을 기술

	InitializeVer();		//	버전표시

	//메인 화면 탑에 EQID 보여지도록 수정 2017/06/11 HSK.
	ShowEQID();

	return 0;
}

// 표시 언어가 변경되었다.
LRESULT CDlgbarTop::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	CGxUICtrl::BroadcastStaticCpationIndex(this, wParam, IDC_LBLX_TOPBAR_CLOCK);

	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// 왼쪽 버튼을 누른 채 드래그하면 윈도우의 위치를 변경할 수 있다.
void CDlgbarTop::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
   CWnd *pWnd = AfxGetMainWnd();
   HWND hUIWnd = pWnd->m_hWnd;
   ::DefWindowProc(hUIWnd, WM_SYSCOMMAND, SC_MOVE+1,MAKELPARAM(point.x,point.y));   

	CDialogBar::OnLButtonDown(nFlags, point);
}


// 시계표시 타이머 동작
void CDlgbarTop::OnTimer(UINT_PTR nIDEvent)
{
	static char cBlink = 0;
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if ( nIDEvent == 100 )
	{
		CString sTime;
		CString sVetClock;
		CString sRecipe;
		SYSTEMTIME tmKorNow;
		::GetLocalTime( &tmKorNow );			// 현재 시각

		CTime tmPGMTimeCheck;  //버퍼 시간 저장
		int nTime = 0;

		CTime tmVetNow = (CTime)tmKorNow;		// [W] 베트남 시간 적용
		tmVetNow -= CTimeSpan(0,2,0,0);

		sRecipe=theRecipeBank.m_strRecipeName; // 현재 사용중인 레시피

		//2018-03-07,SKCHO,추가
		theProcBank.m_strPPID = sRecipe;		//모델_이름 과 같으므로 

		//2018-03-12,MGYUN, PPID MELSEC WRITE
		theMonitorBank.WritePPID(theProcBank.m_strPPID);

	// 시각 표시
		sTime.Format(_T("%04u-%02u-%02u\n K: %02u:%02u:%02u\n V: %02u:%02u:%02u"), 
			tmKorNow.wYear, tmKorNow.wMonth, tmKorNow.wDay, tmKorNow.wHour, tmKorNow.wMinute, tmKorNow.wSecond, 
			tmVetNow.GetHour(),tmVetNow.GetMinute(), tmVetNow.GetSecond());
		CGxUICtrl::SetStaticString(this, IDC_LBLX_TOPBAR_CLOCK, sTime);				// 현재 시각을 표시한다.

		CGxUICtrl::SetStaticString(this, IDC_GXSTC_TOPBAR_MODEL_TEXT, sRecipe);


		//2017-12-23,SKCHO, PA 자동 원점 기능
		tmPGMTimeCheck = (CTime)tmKorNow;
		if(theConfigBank.m_Option.m_nPaOrgTime == 0 || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_INIT)  //0이면 사용 안함
		{
			theProcBank.m_bPreAlignOrgStart = FALSE;
		}
		else
		{
			nTime =(int) (60*60* theConfigBank.m_Option.m_nPaOrgTime);  //시간을 초로 계산
			if(tmPGMTimeCheck - theProcBank.m_tmPGMStart >= nTime)
			{
				theLog[LOG_SEQUENCE].AddBuf(_T("PA ORG START TIME"));
				theProcBank.m_bPreAlignOrgStart = TRUE;
				theProcBank.m_tmPGMStart = tmPGMTimeCheck;			
			}
		}
	
		//2017-09-21,SKCHO, AUTO/TEACH키 상태 표시
		if(theUnitFunc.GetInPutIOState(X_TEACH_KEY_SW) == OFF)
		{
			CGxUICtrl::SetStaticString(this, IDC_GXSTC_TOPBAR_AUTO, _T("Teach"));
			CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TOPBAR_AUTO, GXCOLOR_RED);
		}
		else
		{
			CGxUICtrl::SetStaticString(this, IDC_GXSTC_TOPBAR_AUTO, _T("Auto"));
			CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TOPBAR_AUTO, GXCOLOR_ON);
		}	


		//2017-10-09,SKCHO, CIM 프로그램 상태 표시 추가
		if(theProcBank.GetCimState() == 0)  //offline
		{
			CGxUICtrl::SetStaticString(this, IDC_GXSTC_TOPBAR_CIM_STATE, _T("OFF_LINE"));
			CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TOPBAR_CIM_STATE, GXCOLOR_RED);
		}
		else if(theProcBank.GetCimState() == 1)  //remote
		{
			CGxUICtrl::SetStaticString(this, IDC_GXSTC_TOPBAR_CIM_STATE, _T("REMOTE"));
			CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TOPBAR_CIM_STATE, GXCOLOR_ON);
		}
		else if(theProcBank.GetCimState() == 2)  //local
		{
			CGxUICtrl::SetStaticString(this, IDC_GXSTC_TOPBAR_CIM_STATE, _T("LOCAL"));
			CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TOPBAR_CIM_STATE, GXCOLOR_WHITE);
		}
		
	}
	else if(nIDEvent == TMDDOWNLOAD)
	{

		if(cBlink == 0)
		{
			cBlink = 1;
		}
		else
		{
			cBlink = 0;
		}
		CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TOPBAR_PG, cBlink ? GXCOLOR_WHITE : GXCOLOR_BLUE);
		CGxUICtrl::SetStaticTextColor(this, IDC_GXSTC_TOPBAR_PG, cBlink ? GXCOLOR_BLUE : GXCOLOR_WHITE);
	}

	CDialogBar::OnTimer(nIDEvent);
}


// 시계를 클릭하면 창을 최소화 시킨다.
void CDlgbarTop::ClickLblxTopbarClock()
{
	// 창 최소화 메세지 전송
	::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
}

// 윈도우 생성시 테두리 제거 작업
BOOL CDlgbarTop::PreCreateWindow(CREATESTRUCT& cs)
{
	GxDEF_REMOVE_FRAME

	return CDialogBar::PreCreateWindow(cs);
}

//kjpark 20160919 Topdlg에 connect status 
// 통신 상태가 변경되었다.
LRESULT CDlgbarTop::OnChangeCommState(WPARAM wParam, LPARAM lParam)
{
	UINT nID = 0;
	BOOL bFlag;
	int nMessage = (int)wParam;
	int nStart = (int)lParam;

#if 0
	CMCMain* pMCMain = CMCMain::GetInstance();

	if ( pMCMain == NULL ) return 0;			// 오류
#endif

	// MCR에서 제공하는 ISConnect 함수에서 계속 -1만 반환 하여 추가. 17/04/25 HSK.
	if((nMessage == eCOMM_Lower_UVMCR) )
	{
		if(nStart == FALSE)
		{
			CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TOPBAR_MCR2, GXCOLOR_RED);
			return 0;
		}
	}
	if((nMessage == eCOMM_Upper_UVMCR))
	{
		if(nStart == FALSE)
		{
			CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TOPBAR_MCR, GXCOLOR_RED);
			return 0;
		}
	}

	for (int i = eCOMM_SignalPC; i < eMAX_COMMCount; i++)
	{
		nID = 0;

		switch ( i )
		{
		case eCOMM_SignalPC :		nID = IDC_GXSTC_TOPBAR_PG; break;			
		case eCOMM_MITEdgeAlign :	nID = IDC_GXSTC_TOPBAR_EDGEALIGN; break;			
		case eCOMM_RobotAlign :		nID = IDC_GXSTC_TOPBAR_ROBOTALIGN; break;			
		case eCOMM_LdRobot:			nID = IDC_GXSTC_TOPBAR_LD_ROBOT; break;
		case eCOMM_UdRobot:			nID = IDC_GXSTC_TOPBAR_UD_ROBOT; break;
		case eCOMM_DataPC :			nID = IDC_GXSTC_TOPBAR_CIM; break;			
		case eCOMM_Lower_UVMCR :	nID = IDC_GXSTC_TOPBAR_MCR2; break;			
		case eCOMM_Upper_UVMCR :	nID = IDC_GXSTC_TOPBAR_MCR; break;			
		case eCOMM_AVCamera1:		nID = IDC_GXSTC_TOPBAR_ACTIVE1; break;
		case eCOMM_AVCamera2:		nID = IDC_GXSTC_TOPBAR_ACTIVE2; break;
		case eCOMM_MITAlign:		nID = IDC_GXSTC_TOPBAR_PREALIGN; break;
		case eCOMM_DCR :			nID = IDC_GXSTC_TOPBAR_DCR; break;			
		case eCOMM_Tray_Align :		nID = IDC_GXSTC_TOPBAR_TRAYALIGN; break;			
		case eCOMM_AccuraGPS:		nID = 0; break;
		case eCOMM_AccuraUPS:		nID = 0; break;
		}

		// 잘못된 통신 Index는 Skip
		if ( nID <= 0 ) continue;
#if 0
		bFlag = 	pMCMain->GetCommConnected( i );			// 연결상태 모니터링
#endif
		//GetSocketIndex
		CMainFrame* pMainFrm = (CMainFrame*)::AfxGetMainWnd();
		if(i == eCOMM_SignalPC && theSocketInterFace.m_pSocket_PatternGenerator.IsConnected() == TRUE)
		{
			bFlag = theSocketInterFace.m_pSocket_PatternGenerator.GetClientAt(SOCKET_PGPC)->IsConnected();
		}
		else if(i == eCOMM_MITEdgeAlign && theSocketInterFace.m_pSocket_MITEdgeAlign.IsConnected() == TRUE)
		{
			bFlag = theSocketInterFace.m_pSocket_MITEdgeAlign.GetClientAt(SOCKET_MITEDGEALIGN)->IsConnected();
		}
		else if(i == eCOMM_RobotAlign && theSocketInterFace.m_pSocket_RobotAlign.IsConnected() == TRUE)
		{
			bFlag = theSocketInterFace.m_pSocket_RobotAlign.GetClientAt(SOCKET_ROBOTALIGN)->IsConnected();
		}
		else if(i == eCOMM_LdRobot && theSocketInterFace.m_pSocket_Robot[E_LD_ROBOT].IsConnected() == TRUE)
		{
			bFlag = theSocketInterFace.m_pSocket_Robot[E_LD_ROBOT].GetClientAt(SOCKET_LD_ROBOT)->IsConnected();
		}
		else if(i == eCOMM_UdRobot && theSocketInterFace.m_pSocket_Robot[E_UD_ROBOT].IsConnected() == TRUE)
		{
			bFlag = theSocketInterFace.m_pSocket_Robot[E_UD_ROBOT].GetClientAt(SOCKET_UD_ROBOT)->IsConnected();
		}
		else if(i == eCOMM_Tray_Align && theSocketInterFace.m_pSocket_Tray_Align.IsConnected() == TRUE)
		{
			bFlag = theSocketInterFace.m_pSocket_Tray_Align.GetClientAt(SOCKET_TRAY_ALIGN)->IsConnected();
		}
		else if(i == eCOMM_DataPC )
		{
			bFlag = pMainFrm->GetSocket()->GetCommConnected(eCOMM_DataPC);	
			if(m_bOldData_PC !=bFlag )
			{
				if(bFlag)
				{
					theProcBank.InitUnitInterlock();
				}
				else
				{
					theProcBank.ChangeCimState(CONST_CIM_STATE::CIM_OFFLINE);
				}
				m_bOldData_PC =bFlag;
			}		
			
		}
		else
		{
			bFlag = pMainFrm->GetSocket()->GetCommConnected(i);	
		}

		CGxUICtrl::SetStaticColor(this, nID, bFlag ? GXCOLOR_ON : GXCOLOR_RED);
		
		//2018-04-04,NAM, To check status of Lower MCR
		if (nID==IDC_GXSTC_TOPBAR_MCR2)
		{
		//2018-03-08,MGYUN, LOWER MCR 상태 기록
		theMonitorBank.WriteMCRStatus(eCOMM_Lower_UVMCR, bFlag);
		}
		//2018-04-04,NAM, To check status of Upper MCR
		if(nID==IDC_GXSTC_TOPBAR_MCR)
		{
		//2018-03-08,MGYUN, UPPER MCR 상태 기록
		theMonitorBank.WriteMCRStatus(eCOMM_Upper_UVMCR, bFlag);
		}
		//CIM VERSION 표시
		if(nID == IDC_GXSTC_TOPBAR_CIM)
		{
			CGxUICtrl::SetStaticString(this, nID,CIM_VERSION);		
		}
	}//of for i

	if(nMessage == eCOMM_TMDDownLoad && nStart)
	{
		SetTimer(TMDDOWNLOAD,1000,0);
	}
	// 잡파일 다운로드가 아닌 통신 상태가 변경 되었을때 타이머가 죽어버리는 현상 수정 2017/06/16 HSK.
	else if(nMessage == eCOMM_TMDDownLoad && (nStart == FALSE))
	{
		// UI 상에 애매한 색으로 멈춰버린다 원래 색으로 바꺼주자 2017/06/16 HSK.
		CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TOPBAR_PG, GXCOLOR_ON);
		CGxUICtrl::SetStaticTextColor(this, IDC_GXSTC_TOPBAR_PG, GXCOLOR_BLACK);

		KillTimer(TMDDOWNLOAD);
	}
	else if(nMessage == eCOMM_SignalPC && theSocketInterFace.m_pSocket_PatternGenerator.GetClientAt(SOCKET_PGPC)->IsConnected() == FALSE)
	{
		// PG 끊어져도 상태 갱신 2017/06/16 HSK.
		CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TOPBAR_PG, GXCOLOR_RED);
		CGxUICtrl::SetStaticTextColor(this, IDC_GXSTC_TOPBAR_PG, GXCOLOR_BLACK);

		KillTimer(TMDDOWNLOAD);
	}
	return 0;
}

LRESULT CDlgbarTop::OnChangeMachineState( WPARAM wParam, LPARAM lParam )
{
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TOPBAR_STATE, ConstMachineState[theProcBank.GetMachineState()].strName);
	return 0;
}


void CDlgbarTop::InitializeVer()				//	버전 가져와서 보여주기
{
	//	IDC_GXSTC_TOPBAR_VER_TEXT

	HRSRC hRsrc = FindResource(NULL, MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION);		//버전 리소스에서 정보를 가져옴

	if (hRsrc != NULL)
	{
		HGLOBAL hGlobalMemory = LoadResource(NULL, hRsrc);			//	글로벌핸들에 리소스넣음
		if (hGlobalMemory != NULL)
		{
			void *pVersionResouece = LockResource(hGlobalMemory);	//	핸들에서 정보 긁어옴
			void *pVersion;
			UINT uLength;

			// 아래줄에 041204B0는 리소스 파일(*.rc)에서 가져옴.

			if( VerQueryValue(pVersionResouece, _T("StringFileInfo\\041204B0\\ProductVersion"), &pVersion, &uLength) != 0 )
			{
				CString szVer = reinterpret_cast<wchar_t*>(pVersion);

				CGxUICtrl::SetStaticString(this, IDC_GXSTC_TOPBAR_VER_TEXT, szVer);				// 버전을 표시함
			}
			FreeResource(hGlobalMemory);
		}
	}

}

void CDlgbarTop::ShowEQID()				//	버전 가져와서 보여주기
{
	CString strPath;
	strPath.Format(_T("D:\\Cheetah_CIM\\CIM_Config\\Setup.ini"));

	CIni ini(strPath);

	CString strBuf;
	strBuf	= ini.GetString(_T("EQUIP")	, _T("EQID")	, FALSE);

	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TOPBAR_EQID_TEXT, strBuf);
	
	//2018-03-07,SKCHO, MELSEC 데이터 추가
	theMonitorBank.WriteEQPID(strBuf);

}
void CDlgbarTop::ClickProductLogin()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CDlgUserLogin dlgLogin;

	if(pMain->GetCurUserData().sID.IsEmpty())
	{
		if ( dlgLogin.DoModal() == IDOK )
		{
			CGxUICtrl::SetStaticString(this, IDC_GXBTN_TOPBAR_LOGINER, pMain->GetCurUserData().sID);
		}
	}
	else
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(TRUE, 
			_T("작업자를 로그아웃 하시겠습니까?"), 
			_T("Logout operator now?"),		
			_T("Bạn có muốn thoát của người lao động?") );

		int nRet = dlgMsgBox.DoModal();
		if(nRet != IDOK)
			return;
		if (theConfigBank.m_Option.m_bUseLoginRequest)
		{
			theSocketInterFace.SendCmdLoginToDataPC(EQUIP_SET_USER_LOGOUT_REQUEST, 
				pMain->GetCurUserData().sID, pMain->GetCurUserData().sPassword);
			theSocketInterFace.SendCmdLoginToDataPC(EQUIP_SET_INS1_USER_LOGOUT);
		}
		else
		{
			CUserAccount user;
			pMain->SetCurUserData(user);
		}
	}
}


void CDlgbarTop::ClickGxstcTopbarModelText()
{
	//2017-03-22,skcho, RUN일때는 모델 변경 다이얼로그 팝업 안되게, CIM 검수
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN
		|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)
	{
		return;
	}

	CDIgRecipeSelect dlgRecipeSel;	
	INT_PTR iRet = dlgRecipeSel.DoModal();
}


void CDlgbarTop::ClickGxstcTopbarDcr()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDlgbarTop::ClickGxstcTopbarPg()  //2017-03-08,skcho, TMD 파일 다운로드 이벤트가 발생하면 깜빡거리고 누르면 다운로드 메시지 팝업
{	
	if(theConfigBank.m_System.m_bJobStart)
	{
		::PostMessage( ( (CMainFrame*)AfxGetMainWnd() )->m_hWnd, GxMSG_GUI_SHOW_DLG, (WPARAM)eDLG_MsgTMDDownLoad, (LPARAM)1);	
	}
}



void CDlgbarTop::ClickGxstcTopbarLogo()
{
	/*CDlgBoxKeyBoard dlgKey;

	if ( dlgKey.DoModal() == IDOK )
	{
		if (dlgKey.m_strReturnText == theConfigBank.m_System.m_strPassword)
		{
			CDlgLogo	dlgLogo;

			dlgLogo.DoModal();
			return;
		}
		else
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
	}*/
	CDlgLogo	dlgLogo;

	dlgLogo.DoModal();
	return;

}


void CDlgbarTop::ClickGxstcTopbarVerText()
{
	::PostMessage( ( (CMainFrame*)AfxGetMainWnd() )->m_hWnd, GxMSG_GUI_SHOW_DLG, (WPARAM)eDLG_VersionHistory, (LPARAM)1);	
}


void CDlgbarTop::ClickGxstcTopbarPrealign()
{
	if(theSocketInterFace.m_pSocket_MITAlign.IsConnected())
	{
		theSocketInterFace.CreateMITAlignSocket();
		Sleep(500);
		theSocketInterFace.ConnectMITAlignSocket();
	}
}
