// FormMain.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FormMain.h"

#include "GUIDefine.h"
#include "MainFrm.h"

//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"

// DLL에 존재하는 각종 Dialog 사용
#include "UI\GausGUI\GxNumPad.h"
#include "UI\GausGUI\GxNumPadEx.h"
#include "UI\GausGUI\GxMsgBox.h"

#include "CheetahApp.h"
#include "UI\Dialog\DlgUserLogin.h"
#include "UI\Dialog\DlgbarBottom.h"

#include "UI\Dialog\DlgLossTime.h"

#include "StateMachine/StateAMT.h"

#include "UI\GausGUI\GxButtonEx.h"
#include "DlgInputTrayID.h"
#include "DlgTrayScanError.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CFormMain

IMPLEMENT_DYNCREATE(CFormMain, CFormView)

CFormMain::CFormMain()
	: CFormView(CFormMain::IDD)
{
	InitCellElement();
	for (int i = 0; i < 13; i++)
		TrayStatus[i] = FALSE;

	//SerialOpen();
	// 2017.2.5 bgkim
	// Melsec 관련 버튼 토글 변수
	m_bAble = FALSE;
	m_bStart = FALSE;
	m_bComp = FALSE;
	m_bPause = FALSE;

	//2017-12-18,SKCHO, 추가
	m_nLDRobotOrgSeq = seqLdRobot_idle;

	//2018-04-06,NAM, set value for m_bReport
	m_Report=TRUE; 

}

CFormMain::~CFormMain()
{

}

void CFormMain::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDT_BOARD, m_edtBoard);
	DDX_Control(pDX, IDC_EDT_BOARD_CZONE, m_edtBoard2);
}

BEGIN_MESSAGE_MAP(CFormMain, CFormView)

	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(UDMSG_COUNT_UPDATE,	OnCountUpdate)			
	ON_MESSAGE(UDMSG_TACT_CHANGE,	OnTactChange)	
	ON_MESSAGE(UDMSG_MCR_UPDATE,	OnMCRUpdate)	
	ON_MESSAGE(UDMSG_UPPEREQP_COM_UPDATE,	OnUpper_Eqp_Com_Update)
	ON_MESSAGE(UDMSG_DCR_UPDATE,	OnDCRUpdate)	
	ON_MESSAGE(UDMSG_CZONE_UPDATE,	OnCZONEUpdate)	
	ON_MESSAGE(UDMSG_IDLECHAGNE_UPDATE,	OnIDLEChangeTimeUpdate)	
	ON_MESSAGE(UDMSG_IDLECHAGNERUN_UPDATE,	OnIDLEChangeRUNTimeUpdate)	
	ON_MESSAGE(UDMSG_IDLEAGVINSTART_UPDATE,	OnAGVInStartTimeUpdate)	
	ON_MESSAGE(UDMSG_IDLEAGVINEND_UPDATE,	OnAGVInEndTimeUpdate)	
	ON_MESSAGE(UDMSG_IDLEAGVOUTSTART_UPDATE,OnAGVOutStartTimeUpdate)	
	ON_MESSAGE(UDMSG_IDLEAGVOUTEND_UPDATE,	OnAGVOutEndTimeUpdate)	
	//2018wjp
	ON_MESSAGE(UDMSG_UPDATE_LOAD_CELL,	OnUpdateLoadCellID)	
	ON_MESSAGE(UDMSG_UPDATE_ONE_CELL,	OnUpdateOneCellID)	
	ON_MESSAGE(UDMSG_REMOVE_ALL_CELL,	OnRemoveALLCellID)	
	ON_MESSAGE(UDMSG_REMOVE_ONE_CELL,	OnRemoveOneCellID)	
	ON_MESSAGE(UDMSG_DCR_FAIL,	OnDCRFail)	
	ON_MESSAGE(UDMSG_CIM_BACK_FAIL,	OnDCRCimBackFail)	

	ON_WM_TIMER()
	
	ON_BN_CLICKED(IDC_BTN_GETCELLID, &CFormMain::OnBnClickedBtnGetcellid)
	ON_BN_CLICKED(IDC_BTN_CELLID11, &CFormMain::OnBnClickedBtnCellid11)
	ON_BN_CLICKED(IDC_BTN_CELLID21, &CFormMain::OnBnClickedBtnCellid21)
	ON_BN_CLICKED(IDC_BTN_CELLID12, &CFormMain::OnBnClickedBtnCellid12)
	ON_BN_CLICKED(IDC_BTN_CELLID22, &CFormMain::OnBnClickedBtnCellid22)
	ON_BN_CLICKED(IDC_BTN_CELLID13, &CFormMain::OnBnClickedBtnCellid13)
	ON_BN_CLICKED(IDC_BTN_CELLID23, &CFormMain::OnBnClickedBtnCellid23)
	ON_BN_CLICKED(IDC_BTN_CELLID14, &CFormMain::OnBnClickedBtnCellid14)
	ON_BN_CLICKED(IDC_BTN_CELLID24, &CFormMain::OnBnClickedBtnCellid24)
	ON_BN_CLICKED(IDC_BUTTON4, &CFormMain::OnBnClickedButton4)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CFormMain, CFormView)
	ON_EVENT(CFormMain, IDC_GXBTN_MODE_PM, DISPID_CLICK, CFormMain::ClickBtnModePM, VTS_NONE)
	
	ON_EVENT(CFormMain, IDC_GXBTN_START, DISPID_CLICK, CFormMain::ClickGxbtnStart, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXBTN_STOP, DISPID_CLICK, CFormMain::ClickGxbtnStop, VTS_NONE)

	//	ON_EVENT(CFormMain, IDC_GXBTN_TEST_IN, DISPID_CLICK, CFormMain::ClickGxbtnTestIn, VTS_NONE)
//	ON_EVENT(CFormMain, IDC_GXBTN_TEST_OUT, DISPID_CLICK, CFormMain::ClickGxbtnTestOut, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_CELLMOVE_POS1, DISPID_CLICK, CFormMain::ClickGxstcCellmovePos1, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_CELLMOVE_POS2, DISPID_CLICK, CFormMain::ClickGxstcCellmovePos2, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_CELLMOVE_POS4, DISPID_CLICK, CFormMain::ClickGxstcCellmovePos4, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_CELLMOVE_POS3, DISPID_CLICK, CFormMain::ClickGxstcCellmovePos3, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_CELLMOVE_POS6, DISPID_CLICK, CFormMain::ClickGxstcCellmovePos6, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_CELLMOVE_POS5, DISPID_CLICK, CFormMain::ClickGxstcCellmovePos5, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_CELLMOVE_POS8, DISPID_CLICK, CFormMain::ClickGxstcCellmovePos8, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_CELLMOVE_POS7, DISPID_CLICK, CFormMain::ClickGxstcCellmovePos7, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_CELLMOVE_POS10, DISPID_CLICK, CFormMain::ClickGxstcCellmovePos10, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_CELLMOVE_POS9, DISPID_CLICK, CFormMain::ClickGxstcCellmovePos9, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_CELLMOVE_POS12, DISPID_CLICK, CFormMain::ClickGxstcCellmovePos12, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_CELLMOVE_POS11, DISPID_CLICK, CFormMain::ClickGxstcCellmovePos11, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_CELLMOVE_POS14, DISPID_CLICK, CFormMain::ClickGxstcCellmovePos14, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_CELLMOVE_POS13, DISPID_CLICK, CFormMain::ClickGxstcCellmovePos13, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_CELLMOVE_POS16, DISPID_CLICK, CFormMain::ClickGxstcCellmovePos16, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_CELLMOVE_POS15, DISPID_CLICK, CFormMain::ClickGxstcCellmovePos15, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_CELLMOVE_POS17, DISPID_CLICK, CFormMain::ClickGxstcCellmovePos17, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_CELLMOVE_POS18, DISPID_CLICK, CFormMain::ClickGxstcCellmovePos18, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_CELLMOVE_POS20, DISPID_CLICK, CFormMain::ClickGxstcCellmovePos20, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_CELLMOVE_POS19, DISPID_CLICK, CFormMain::ClickGxstcCellmovePos19, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_CELLMOVE_POS22, DISPID_CLICK, CFormMain::ClickGxstcCellmovePos22, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_CELLMOVE_POS21, DISPID_CLICK, CFormMain::ClickGxstcCellmovePos21, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_CELLMOVE_POS24, DISPID_CLICK, CFormMain::ClickGxstcCellmovePos24, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_CELLMOVE_POS23, DISPID_CLICK, CFormMain::ClickGxstcCellmovePos23, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_CELLMOVE_POS25, DISPID_CLICK, CFormMain::ClickGxstcCellmovePos25, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXBTN_COUNT_CLEAR, DISPID_CLICK, CFormMain::ClickGxbtnCountClear, VTS_NONE)
	//ON_EVENT(CFormMain, IDC_GXBTN_COUNT_CLEAR2, DISPID_CLICK, CFormMain::ClickGxbtnCountClear2, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXBTN_MODE_PM2, DISPID_CLICK, CFormMain::ClickGxbtnModePm2, VTS_NONE)

	ON_EVENT(CFormMain, IDC_GXSTC_SEND_ABLE, DISPID_CLICK, CFormMain::ClickGxstcSendAble, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_SEND_START, DISPID_CLICK, CFormMain::ClickGxstcSendStart, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_SEND_COMPLETE, DISPID_CLICK, CFormMain::ClickGxstcSendComplete, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_CELLMOVE_POS33, DISPID_CLICK, CFormMain::ClickGxstcCellmovePos33, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_CELLMOVE_POS31, DISPID_CLICK, CFormMain::ClickGxstcCellmovePos31, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_CELLMOVE_POS32, DISPID_CLICK, CFormMain::ClickGxstcCellmovePos32, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_CELLMOVE_POS36, DISPID_CLICK, CFormMain::ClickGxstcCellmovePos36, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_CELLMOVE_POS38, DISPID_CLICK, CFormMain::ClickGxstcCellmovePos38, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_CELLMOVE_POS37, DISPID_CLICK, CFormMain::ClickGxstcCellmovePos37, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_CELLMOVE_POS29, DISPID_CLICK, CFormMain::ClickGxstcCellmovePos29, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_CELLMOVE_POS30, DISPID_CLICK, CFormMain::ClickGxstcCellmovePos30, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_CELLMOVE_POS28, DISPID_CLICK, CFormMain::ClickGxstcCellmovePos28, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_CELLMOVE_POS27, DISPID_CLICK, CFormMain::ClickGxstcCellmovePos27, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_CELLMOVE_POS26, DISPID_CLICK, CFormMain::ClickGxstcCellmovePos26, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_CELLMOVE_POS34, DISPID_CLICK, CFormMain::ClickGxstcCellmovePos34, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_CELLMOVE_POS35, DISPID_CLICK, CFormMain::ClickGxstcCellmovePos35, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXLBL_REPORT_AUTOCONTACT, DISPID_CLICK, CFormMain::ClickGxlblReportAutocontact, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_LOADER_DOOR5, DISPID_CLICK, CFormMain::ClickGxstcLoaderDoor5, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_MAIN_OLBSKIP, DISPID_CLICK, CFormMain::ClickGxstcMainOlbskip, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXLBL_MAIN_LOGCLEAR, DISPID_CLICK, CFormMain::ClickGxlblMainLogclear, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_AJIG_SKIP, DISPID_CLICK, CFormMain::ClickGxstcAjigSkip, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_BJIG_SKIP, DISPID_CLICK, CFormMain::ClickGxstcBjigSkip, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_CJIG_SKIP, DISPID_CLICK, CFormMain::ClickGxstcCjigSkip, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_DJIG_SKIP, DISPID_CLICK, CFormMain::ClickGxstcDjigSkip, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXLBL_MAIN_RETRYINFO, DISPID_CLICK, CFormMain::ClickGxlblMainRetryinfo, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_GOODTRAY, DISPID_CLICK, CFormMain::ClickGxstcGoodtray, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_NGTRAY, DISPID_CLICK, CFormMain::ClickGxstcNgtray, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_HYBRID_ROBOT_READY, DISPID_CLICK, CFormMain::ClickGxstcHybridRobotReady, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_HYBRID_ROBOT_RUN, DISPID_CLICK, CFormMain::ClickGxstcHybridRobotRun, VTS_NONE)
	ON_EVENT(CFormMain, IDC_GXSTC_HYBRID_ROBOT_COMPLETE, DISPID_CLICK, CFormMain::ClickGxstcHybridRobotComplete, VTS_NONE)
END_EVENTSINK_MAP()

// CFormMain 진단입니다.

#ifdef _DEBUG
void CFormMain::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormMain::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG



// CFormMain 메시지 처리기입니다.

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
LRESULT CFormMain::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{

		//kjpark 20161021 Main Form Cell Status view 구현
		SetTimer(DISPSTATETIME, 500, NULL);
		SetTimer(DISPVACTIER, 500, NULL);
		
		::PostMessage(m_hWnd, UDMSG_COUNT_UPDATE, NULL, NULL);
			//2017-12-25,SKCHO, 프로그램 시작 시간 표시
		CString str;
		str.Format(_T("%04u-%02u-%02u,%02u:%02u:%02u\n"), 
			theApp.tmStart.wYear, theApp.tmStart.wMonth, theApp.tmStart.wDay, theApp.tmStart.wHour, theApp.tmStart.wMinute, theApp.tmStart.wSecond);
			CGxUICtrl::SetStaticString(this,IDC_GXLBL_START_TIME,str);
	}
	else
	{
		//kjpark 20161021 Main Form Cell Status view 구현
		KillTimer(DISPSTATETIME);
		KillTimer(DISPVACTIER);
	}

	return 0;
}

// 표시 언어가 변경되었다.
LRESULT CFormMain::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	CGxUICtrl::BroadcastStaticCpationIndex(this, wParam, IDC_GXLBL_MAIN_TACT);
	CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_GXBTN_START);

	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


// 윈도우 생성전에 모양을 달리 지정한다.
BOOL CFormMain::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	cs.style = WS_OVERLAPPED | WS_CHILD;				// 테두리를 없앤다.

	return CFormView::PreCreateWindow(cs);
}


void CFormMain::OnTimer(UINT_PTR nIDEvent)
{
	//KillTimer(nIDEvent);
	static int nIdieTime = 0;
	if(nIDEvent == DISPVACTIER)
	{
		/*SYSTEMTIME	time;
		CString strLogHeader;
		GetLocalTime(&time);
		strLogHeader.Format(_T("%02u:%02u:%02u:%03u\t\n"), time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
		TRACE(strLogHeader);

		theSerialInterFace.SendCommand();
		*/

		DispVacuumState();
		//프로그램 시작 시 IDLE 상태 일 때 증감 시작
		if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)
		{
			nIdieTime++;		//500ms 이므로 10이면 5초.
		}
		else
			nIdieTime = 0;
		
		if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE && nIdieTime > 10)		//5초 초과이면
		{
			theProcBank.m_InDelayTime += 5; //OLBEOWN FORM의 시간에 5초 만큼 더해 준다.
			theMonitorBank.WriteInDelay(theProcBank.m_InDelayTime);
			::PostMessage( GetMainWnd(), GxMSG_GUI_CHANGE_VIEW, (WPARAM)eFORM_OLBDOWN, (LPARAM)1);
			nIdieTime = 0;
		}

	}
	//kjpark 20161021 Main Form Cell Status view 구현
	else if(nIDEvent == DISPSTATETIME)
	{		
		GetCellStatus();
		SetStautsChange();	
		//2018-05-10,MGYUN, 하이브리드 관련 I/O 볼 수 있게
		DispHybridIOState();
		//2018-03-10,NAM, LD CONV - LD TR까지 데이터 존재 할 시 STEP ID MELSEC 기록.
		theMonitorBank.WriteStepID();

		//2018-03-15,MGYUN, AMT JIG SKIP STATUS 체크 기록
		theMonitorBank.WriteJigSkipStatus();

		//2018-04-06,NAM, UPdate the Quantity and Percentage 
		if (m_Report==TRUE)
		{
			UpdatePrehourContact();
			UPdateWeekContact();
			UPdateMonthContact();
			m_Report=!m_Report;
		}
		//20170204 
		DoorStatusDisplay();
		OptionStatusDisplay();

		if(theProcBank.m_bUnitInterlock_LoadingStop == TRUE)
		{
			KillTimer(DISPSTATETIME);

			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
			dlgMsgBox.SetMessage(FALSE, 
					_T("전체 JIG 스킵으로 인터락이 발생하였습니다."),	//한
					_T("ALL JIGS SKIP INTERLOCK ALARM."),		//영
					_T("ALL JIGS SKIP INTERLOCK ALARM.") );		//베

			if(dlgMsgBox.DoModal() == IDOK)
			{
				theProcBank.ChangeMachineState(CONST_MACHINE_STATE::MACHINE_PAUSE);
				theProcBank.m_bUnitInterlock_LoadingStop = FALSE;
				SetTimer(DISPSTATETIME,500,NULL);
			}
		}
	}
	else if (nIDEvent == DRYRUN_TIMER)
	{
		theUnitFunc.CellData_SetDryRun(0);
		KillTimer(DRYRUN_TIMER);
		SetTimer(DRYRUN_TIMER,5000,NULL);
	}
	else if (nIDEvent == LDROBOTORG_TIMER)
	{
		LDRobotOrg();
		if(m_nLDRobotOrgSeq != seqLdRobot_idle)
			SetTimer(LDROBOTORG_TIMER, 50, NULL);
		
	}
	
	//2018-03-12,MGYUN, AGV IN,OUT 딜레이 시간 카운트 타이머
	else if(nIDEvent == AGV_IN_DELAY_TIMER)
	{
		theProcBank.nAGV_InDelayTime++;
		theMonitorBank.WriteAGVInDelay(theProcBank.nAGV_InDelayTime);
	}

	else if(nIDEvent == AGV_OUT_DELAY_TIMER)
	{
		theProcBank.nAGV_OutDelayTime++;
		theMonitorBank.WriteAGVOutDelay(theProcBank.nAGV_OutDelayTime);
	}

	CFormView::OnTimer(nIDEvent);
}

//kjpark 20161021 Main Form Cell Status view 구현
void CFormMain::GetCellStatus()
{	
	for(int i = CELL_POS_LD_CONV ; i < CELL_POS_MAX ; i++)
	{
		CCellTag tag = theCellBank.GetCellTag((CELL_POS)i);
		BOOL bCell = tag.IsExist();
		if(bCell)
		{			
			iMap[(CELL_POS)i].bExistCell = CELL_STATUS_EXIST;
			CCellInfo* cellInfo = theCellBank.GetCellInfo(tag);

			if( i > CELL_POS_LD_TR_CH2 &&
				(cellInfo->m_Zone_A.AZoneClass == GOOD_CELL
				|| cellInfo->m_Zone_B.BZoneClass == GOOD_CELL
				|| cellInfo->m_Zone_C.CZoneClass == GOOD_CELL
				|| cellInfo->m_Zone_D.DZoneClass == GOOD_CELL))
			{
				iMap[(CELL_POS)i].bExistCell = CELL_STATUS_GOOD;
			}			

			if( i > CELL_POS_LD_TR_CH2 &&
				(cellInfo->m_Zone_B.BZoneClass > GOOD_CELL
				|| cellInfo->m_Zone_C.CZoneClass > GOOD_CELL
				|| cellInfo->m_Zone_D.DZoneClass > GOOD_CELL))
				//(cellInfo->m_Zone_A.AZoneClass > GOOD_CELL
				//|| cellInfo->m_Zone_B.BZoneClass > GOOD_CELL
				//|| cellInfo->m_Zone_C.CZoneClass > GOOD_CELL
				//|| cellInfo->m_Zone_D.DZoneClass > GOOD_CELL))
			{
				iMap[(CELL_POS)i].bExistCell = CELL_STATUS_NG;
			}
			
		}
		else
		{
			iMap[(CELL_POS)i].bExistCell = CELL_STATUS_NONE;
		}
	}
}

//kjpark 20161021 Main Form Cell Status view 구현
void CFormMain::SetStautsChange(BOOL bValue)
{
	CString str;
	int nDelay = 0;
	map<CELL_POS, CELL_ELEMENT>::iterator itmap = iMap.begin();
	for(; itmap != iMap.end(); ++ itmap)
	{
		CELL_ELEMENT cellElement = itmap->second;
		switch(cellElement.bExistCell)
		{
			case CELL_STATUS_NONE:
				CGxUICtrl::SetStaticColor(this,cellElement.nResourceID, GXCOLOR_OFF);
				break;
			case CELL_STATUS_EXIST:
				CGxUICtrl::SetStaticColor(this,cellElement.nResourceID, GXCOLOR_MEDIUM_SPRING_GREEN);
				break;
			case CELL_STATUS_GOOD:
				CGxUICtrl::SetStaticColor(this,cellElement.nResourceID, GXCOLOR_ON);
				break;
			case CELL_STATUS_NG:
				CGxUICtrl::SetStaticColor(this,cellElement.nResourceID, GXCOLOR_RED);
				break;
			case CELL_STATUS_SKIP:
				CGxUICtrl::SetStaticColor(this,cellElement.nResourceID, GXCOLOR_INDIAN_RED);
				break;
			case CELL_STATUS_ALIGN_NG:
				CGxUICtrl::SetStaticColor(this,cellElement.nResourceID, GXCOLOR_FUCHSIA);
				break;
		}		
	}

	if((theUnitFunc.TrayData_Exist(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_IN_BUFFER)) && (TrayStatus[0] == FALSE))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_CELLMOVE_POS26, GXCOLOR_ON);
		TrayStatus[0] = TRUE;	
	}
	else if((TrayStatus[0] == TRUE) && (theUnitFunc.TrayData_Exist(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_IN_BUFFER) == FALSE))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_CELLMOVE_POS26, GXCOLOR_OFF);
		TrayStatus[0] = FALSE;	
	}

	if(theUnitFunc.TrayData_Exist(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_IN_LIFT) && (TrayStatus[1] == FALSE))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_CELLMOVE_POS27, GXCOLOR_ON);
		TrayStatus[1] = TRUE;	
	}
	else if((TrayStatus[1] == TRUE) && (theUnitFunc.TrayData_Exist(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_IN_LIFT) == FALSE))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_CELLMOVE_POS27, GXCOLOR_OFF);
		TrayStatus[1] = FALSE;	
	}

	if(theUnitFunc.TrayData_Exist(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_IN_ALIGN) && (TrayStatus[2] == FALSE))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_CELLMOVE_POS28, GXCOLOR_ON);
		TrayStatus[2] = TRUE;	
	}
	else if((TrayStatus[2] == TRUE) && (theUnitFunc.TrayData_Exist(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_IN_ALIGN) == FALSE))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_CELLMOVE_POS28, GXCOLOR_OFF);
		TrayStatus[2] = FALSE;	
	}

	if(theProcBank.m_bTrayVIResultRetry)
	{
		str.Format(_T("R"));
	}
	else
	{
		//2017-04-18,skcho, 트레이 개수 표시
		str.Format(_T("%d"),theProcBank.m_nSubGoodAlignCount);
	}
	CGxUICtrl::SetStaticString(this,IDC_GXSTC_CELLMOVE_POS28,str);

	//2017-07-07,skcho, 트레이 개수 표시
	str.Format(_T("%d"),theProcBank.m_nGoodAlignCount);
	CGxUICtrl::SetStaticString(this,IDC_GXSTC_CELLMOVE_POS27,str);

	if(theUnitFunc.TrayData_Exist(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_OUT_LIFT) && (TrayStatus[3] == FALSE))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_CELLMOVE_POS29, GXCOLOR_ON);
		TrayStatus[3] = TRUE;	
	}
	else if((TrayStatus[3] == TRUE) && (theUnitFunc.TrayData_Exist(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_OUT_LIFT) == FALSE))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_CELLMOVE_POS29, GXCOLOR_OFF);
		TrayStatus[3] = FALSE;	
	}
	//2017-04-18,skcho, 트레이 개수 표시
	str.Format(_T("%d"), theProcBank.GetGoodOutLiftCount());
	CGxUICtrl::SetStaticString(this,IDC_GXSTC_CELLMOVE_POS29,str);

	//20170212 byskcho
	//트레이정보는 GOOD_IN_TRAY_PORT 인덱스만 사용
	if(theUnitFunc.TrayData_Exist(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_OUT_BUFFER) && (TrayStatus[4] == FALSE))
	//if(theUnitFunc.TrayData_Exist(GOOD_OUT_TRAY_PORT, TRAY_INFO_POS_GOOD_OUT_BUFFER) && (TrayStatus[4] == FALSE))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_CELLMOVE_POS30, GXCOLOR_ON);
		TrayStatus[4] = TRUE;	
	}
	//20170212 byskcho
	//트레이정보는 GOOD_IN_TRAY_PORT 인덱스만 사용
	else if((TrayStatus[4] == TRUE) && (theUnitFunc.TrayData_Exist(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_OUT_BUFFER)== FALSE))
	//else if((TrayStatus[4] == TRUE) && (theUnitFunc.TrayData_Exist(GOOD_OUT_TRAY_PORT, TRAY_INFO_POS_GOOD_OUT_BUFFER) == FALSE))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_CELLMOVE_POS30, GXCOLOR_OFF);
		TrayStatus[4] = FALSE;	
	}

	if(theUnitFunc.TrayData_Exist(NG_TRAY_PORT, TRAY_INFO_POS_NG_OUT_REQ_BUFFER) && (TrayStatus[5] == FALSE))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_CELLMOVE_POS31, GXCOLOR_ON);
		TrayStatus[5] = TRUE;	
	}
	else if((TrayStatus[5] == TRUE) && (theUnitFunc.TrayData_Exist(NG_TRAY_PORT, TRAY_INFO_POS_NG_OUT_REQ_BUFFER) == FALSE))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_CELLMOVE_POS31, GXCOLOR_OFF);
		TrayStatus[5] = FALSE;	
	}

	if(theUnitFunc.TrayData_Exist(NG_TRAY_PORT, TRAY_INFO_POS_NG_IN_BUFFER) && (TrayStatus[6] == FALSE))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_CELLMOVE_POS32, GXCOLOR_ON);
		TrayStatus[6] = TRUE;	
	}
	else if((TrayStatus[6] == TRUE) && (theUnitFunc.TrayData_Exist(NG_TRAY_PORT, TRAY_INFO_POS_NG_IN_BUFFER) == FALSE))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_CELLMOVE_POS32, GXCOLOR_OFF);
		TrayStatus[6] = FALSE;	
	}

	if(theUnitFunc.TrayData_Exist(NG_TRAY_PORT, TRAY_INFO_POS_NG_IN_LIFT_ARM) && (TrayStatus[7] == FALSE))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_CELLMOVE_POS33, GXCOLOR_ON);
		TrayStatus[7] = TRUE;	
	}
	else if((TrayStatus[7] == TRUE) && (theUnitFunc.TrayData_Exist(NG_TRAY_PORT, TRAY_INFO_POS_NG_IN_LIFT_ARM) == FALSE))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_CELLMOVE_POS33, GXCOLOR_OFF);
		TrayStatus[7] = FALSE;	
	}

	if(theUnitFunc.TrayData_Exist(NG_TRAY_PORT, TRAY_INFO_POS_NG_IN_ALIGN) && (TrayStatus[8] == FALSE))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_CELLMOVE_POS34, GXCOLOR_ON);
		TrayStatus[8] = TRUE;	
	}
	else if((TrayStatus[8] == TRUE) && (theUnitFunc.TrayData_Exist(NG_TRAY_PORT, TRAY_INFO_POS_NG_IN_ALIGN) == FALSE))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_CELLMOVE_POS34, GXCOLOR_OFF);
		TrayStatus[8] = FALSE;	
	}

	if(theUnitFunc.TrayData_Exist(NG_TRAY_PORT, TRAY_INFO_POS_NG_OUT_LIFT_ARM) && (TrayStatus[9] == FALSE))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_CELLMOVE_POS35, GXCOLOR_ON);
		TrayStatus[9] = TRUE;	
	}
	else if((TrayStatus[9] == TRUE) && (theUnitFunc.TrayData_Exist(NG_TRAY_PORT, TRAY_INFO_POS_NG_OUT_LIFT_ARM) == FALSE))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_CELLMOVE_POS35, GXCOLOR_OFF);
		TrayStatus[9] = FALSE;	
	}

	if(theUnitFunc.TrayData_Exist(NG_TRAY_PORT, TRAY_INFO_POS_NG_OUT_COMP_BUFFER) && (TrayStatus[10] == FALSE))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_CELLMOVE_POS36, GXCOLOR_ON);
		TrayStatus[10] = TRUE;	
	}
	else if((TrayStatus[10] == TRUE) && (theUnitFunc.TrayData_Exist(NG_TRAY_PORT, TRAY_INFO_POS_NG_OUT_COMP_BUFFER) == FALSE))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_CELLMOVE_POS36, GXCOLOR_OFF);
		TrayStatus[10] = FALSE;	
	}

	if(theUnitFunc.TrayData_Exist(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_TR) && (TrayStatus[11] == FALSE))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_CELLMOVE_POS37, GXCOLOR_ON);
		TrayStatus[11] = TRUE;	
	}
	else if((TrayStatus[11] == TRUE) && (theUnitFunc.TrayData_Exist(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_TR) == FALSE))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_CELLMOVE_POS37, GXCOLOR_OFF);
		TrayStatus[11] = FALSE;	
	}

	if(theUnitFunc.TrayData_Exist(NG_TRAY_PORT,TRAY_INFO_POS_NG_TR) && (TrayStatus[12] == FALSE))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_CELLMOVE_POS38, GXCOLOR_ON);
		TrayStatus[12] = TRUE;	
	}
	else if((TrayStatus[12] == TRUE) && (theUnitFunc.TrayData_Exist(NG_TRAY_PORT, TRAY_INFO_POS_NG_TR) == FALSE))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_CELLMOVE_POS38, GXCOLOR_OFF);
		TrayStatus[12] = FALSE;	
	}

	if(theUnitFunc.GetZoneEnd(ZONE_ID_A))
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_MAIN_CELLMOVE_AZONE, GXCOLOR_ON);
	else
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_MAIN_CELLMOVE_AZONE, GXCOLOR_OFF);

	if(theUnitFunc.GetZoneEnd(ZONE_ID_B))
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_MAIN_CELLMOVE_BZONE, GXCOLOR_ON);
	else
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_MAIN_CELLMOVE_BZONE, GXCOLOR_OFF);

	if(theUnitFunc.GetZoneEnd(ZONE_ID_C))
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_MAIN_CELLMOVE_CZONE, GXCOLOR_ON);
	else
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_MAIN_CELLMOVE_CZONE, GXCOLOR_OFF);

	if(theUnitFunc.GetZoneEnd(ZONE_ID_D))
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_MAIN_CELLMOVE_DZONE, GXCOLOR_ON);
	else
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_MAIN_CELLMOVE_DZONE, GXCOLOR_OFF);

	// Step Interlock 발생시 어느 셀인지 알기 위해 추가 .2017/05/04 HSK.
	if(theProcBank.m_bShowCh1TrackInChk)
		CGxUICtrl::SetStaticString(this,IDC_GXSTC_CELLMOVE_POS13,_T("OK"));
	else
		CGxUICtrl::SetStaticString(this,IDC_GXSTC_CELLMOVE_POS13,_T(""));

	if(theProcBank.m_bShowCh2TrackInChk)
		CGxUICtrl::SetStaticString(this,IDC_GXSTC_CELLMOVE_POS14,_T("OK"));
	else
		CGxUICtrl::SetStaticString(this,IDC_GXSTC_CELLMOVE_POS14,_T(""));


	//2017-07-04,SKCHO AGV 상태를 메인 화면에 표시한다.
	if(theProcBank.m_strLoadAGVTC == _T("투입요청(350)"))
	{		
		str.Format(_T("%s\n\t%02u-%02u:%02u:%02u"), theProcBank.m_strLoadAGVTC,theProcBank.m_AGVLoadReqtime.wDay,theProcBank.m_AGVLoadReqtime.wHour, theProcBank.m_AGVLoadReqtime.wMinute, theProcBank.m_AGVLoadReqtime.wSecond);
		CGxUICtrl::SetStaticString(this,IDC_GXSTC_MAIN_AGV_OK_LOAD_TC_REQUEST,str);
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_MAIN_AGV_OK_LOAD_TC_REQUEST, GXCOLOR_ON);
		
	}
	else
	{
		CGxUICtrl::SetStaticString(this,IDC_GXSTC_MAIN_AGV_OK_LOAD_TC_REQUEST,theProcBank.m_strLoadAGVTC);
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_MAIN_AGV_OK_LOAD_TC_REQUEST, GXCOLOR_OFF);		
	}

	if(theProcBank.m_strUnLoadAGVTC == _T("배출대기(352)"))
	{
		str.Format(_T("%s\n\t%02u-%02u:%02u:%02u"), theProcBank.m_strUnLoadAGVTC,theProcBank.m_AGVUnLoadReqtime.wDay,theProcBank.m_AGVUnLoadReqtime.wHour, theProcBank.m_AGVUnLoadReqtime.wMinute, theProcBank.m_AGVUnLoadReqtime.wSecond);
		CGxUICtrl::SetStaticString(this,IDC_GXSTC_MAIN_AGV_OK_UNLOAD_TC_REQUEST,str);
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_MAIN_AGV_OK_UNLOAD_TC_REQUEST, GXCOLOR_ON);
		
	}
	else
	{
		CGxUICtrl::SetStaticString(this,IDC_GXSTC_MAIN_AGV_OK_UNLOAD_TC_REQUEST,theProcBank.m_strUnLoadAGVTC);
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_MAIN_AGV_OK_UNLOAD_TC_REQUEST, GXCOLOR_OFF);
			
	}

	//2017-10-07,SKCHO
	//if(theUnitFunc.GetInPutIOCheck(X_GOOD_LD_TRAY_LIFT_SEN))
	if(theUnitFunc.GetInPutIOState(X_GOOD_LD_TRAY_LIFT_SEN))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_GOOD_IN_LIFT_SEN, GXCOLOR_ON);
	}
	else
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_GOOD_IN_LIFT_SEN, GXCOLOR_RED);
	}

	//2017-10-07,SKCHO
	if(theUnitFunc.GetInPutIOState(X_GOOD_ULD_TRAY_LIFT_SEN))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_GOOD_OUT_LIFT_SEN, GXCOLOR_ON);
	}
	else
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_GOOD_OUT_LIFT_SEN, GXCOLOR_RED);
	}
	//2017-10-07,SKCHO
	if(theUnitFunc.GetInPutIOState(X_NG_TRAY_LIFT_ARM_TRAY_SEN_1) && theUnitFunc.GetInPutIOState(X_NG_TRAY_LIFT_ARM_TRAY_SEN_2))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_NG_IN_LIFT_SEN, GXCOLOR_ON);
	}
	else
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_NG_IN_LIFT_SEN, GXCOLOR_RED);
	}

	//2017-10-07,SKCHO
	if(theUnitFunc.GetInPutIOState(X_GOOD_LD_TRAY_LIFT_LIMIT_SEN_3))  // 중간센서
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_GOOD_IN_ALIGN_SEN, GXCOLOR_ON);
	}
	else
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_GOOD_IN_ALIGN_SEN, GXCOLOR_RED);
	}
	
	//2017-11-17,YMG
	if(theUnitFunc.GetInPutIOState(X_GOOD_ULD_BUFFER_CONV_TRAY_SEN_1))  
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_GOOD_OUT_BUF_SEN1, GXCOLOR_ON);
	}
	else
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_GOOD_OUT_BUF_SEN1, GXCOLOR_RED);
	}

	//2017-11-17,YMG
	if(theUnitFunc.GetInPutIOState(X_GOOD_LD_BUFFER_CONV_TRAY_SEN_4))  
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_GOOD_IN_BUF_SEN5, GXCOLOR_ON);
	}
	else
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_GOOD_IN_BUF_SEN5, GXCOLOR_RED);
	}

	//2018-03-15,GHLEE
	if(theUnitFunc.GetInPutIOState(X_GOOD_LD_TRAY_FINAL_SEN_1))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_GOOD_IN_FINAL_SEN1, GXCOLOR_ON);
	}
	else
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_GOOD_IN_FINAL_SEN1, GXCOLOR_RED);
	}
	//if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN
	//	&& theUnitFunc.GetInPutIOCheck(X_GOOD_LD_TRAY_LIFT_SEN) == FALSE
	//	&& theUnitFunc.GetInPutIOCheck(X_GOOD_LD_CONV_TRAY_SEN_1) == FALSE
	//	&& theUnitFunc.GetInPutIOCheck(X_GOOD_LD_CONV_TRAY_SEN_4) == FALSE
	//	&& theUnitFunc.GetInPutIOCheck(X_GOOD_LD_CONV_TRAY_SEN_5) == FALSE
	//	&& theUnitFunc.GetInPutIOCheck(X_GOOD_LD_BUFFER_CONV_TRAY_SEN_1) == FALSE
	//	&& theUnitFunc.GetInPutIOCheck(X_GOOD_LD_BUFFER_CONV_TRAY_SEN_4) == FALSE
	//	&& theUnitFunc.GetInPutIOCheck(X_GOOD_LD_BUFFER_CONV_TRAY_SEN_5) == FALSE
	//	&& theUnitFunc.GetInPutIOCheck(X_GOOD_LD_TRAY_LIFT_LIMIT_SEN_1) == FALSE
	//	&& theUnitFunc.GetInPutIOCheck(X_GOOD_LD_TRAY_LIFT_LIMIT_SEN_2) == TRUE
	//	&& theUnitFunc.GetInPutIOCheck(X_GOOD_LD_TRAY_LIFT_LIMIT_SEN_3) == FALSE
	//	&& theProcBank.m_bTRAYInChk == FALSE )
	//{
	//	::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_IDLEAGVINSTART_UPDATE, 0,0); 
	//	theProcBank.m_bTRAYInChk = TRUE;
	//	theUnitFunc.LampChange(LAMP_OFF,LAMP_ON,LAMP_ON,FALSE);
	//}

	//if( (theUnitFunc.GetInPutIOCheck(X_GOOD_LD_TRAY_LIFT_SEN) == TRUE
	//	|| theUnitFunc.GetInPutIOCheck(X_GOOD_LD_CONV_TRAY_SEN_1) == TRUE
	//	|| theUnitFunc.GetInPutIOCheck(X_GOOD_LD_CONV_TRAY_SEN_4) == TRUE
	//	|| theUnitFunc.GetInPutIOCheck(X_GOOD_LD_CONV_TRAY_SEN_5) == TRUE
	//	|| theUnitFunc.GetInPutIOCheck(X_GOOD_LD_BUFFER_CONV_TRAY_SEN_1) == TRUE
	//	|| theUnitFunc.GetInPutIOCheck(X_GOOD_LD_BUFFER_CONV_TRAY_SEN_4) == TRUE
	//	|| theUnitFunc.GetInPutIOCheck(X_GOOD_LD_BUFFER_CONV_TRAY_SEN_5) == TRUE
	//	|| theUnitFunc.GetInPutIOCheck(X_GOOD_LD_TRAY_LIFT_LIMIT_SEN_1) == TRUE
	//	|| theUnitFunc.GetInPutIOCheck(X_GOOD_LD_TRAY_LIFT_LIMIT_SEN_2) == FALSE
	//	|| theUnitFunc.GetInPutIOCheck(X_GOOD_LD_TRAY_LIFT_LIMIT_SEN_3) == TRUE)
	//	&& theProcBank.m_bTRAYInChk == TRUE )
	//{
	//	::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_IDLEAGVINEND_UPDATE, 0,0); 
	//	theProcBank.m_bTRAYInChk = FALSE;
	//	theUnitFunc.LampChange(LAMP_OFF,LAMP_OFF,LAMP_ON,FALSE);
	//}
		
	//2017-07-20,SKCHO, OLB 인터락 스킵
	if(theProcBank.m_bOLBInterLockSkip)
	{
		CGxUICtrl::SetStaticString(this,IDC_GXSTC_MAIN_OLBSKIP,_T("OLB INTERLOCK SKIP"));
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_MAIN_OLBSKIP, GXCOLOR_RED);
	}
	else
	{
		CGxUICtrl::SetStaticString(this,IDC_GXSTC_MAIN_OLBSKIP,_T("OLB INTERLOCK"));
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_MAIN_OLBSKIP, GXCOLOR_OFF);
	}


	if(theProcBank.m_nAGVDelayStep ==3)
	{
		//--theLog[LOG_AGV].AddBuf(_T("IN PORT AGV 요청 TIME OUT"));	
		theUnitFunc.DisplayAGVMsg();
		theProcBank.m_nAGVDelayStep = 5;
	}
	else if(theProcBank.m_nAGVDelayStep == 6)   //2017-11-17,SKCHO,TRAY 받으면 메시지 숨김
	{
		theUnitFunc.HideAGVMsg();
		theProcBank.m_nAGVDelayStep = 0;
	}

	if(theProcBank.m_nOutAGVDelayStep ==3)
	{
		//--theLog[LOG_AGV].AddBuf(_T("OUT PORT AGV 요청 TIME OUT"));	
		theUnitFunc.DisplayAGVMsg();
		theProcBank.m_nOutAGVDelayStep =5;
	}
	else if(theProcBank.m_nOutAGVDelayStep == 6)  //2017-11-17,SKCHO,TRAY 받으면 메시지 숨김
	{	
		theUnitFunc.HideAGVMsg();
		theProcBank.m_nOutAGVDelayStep =0;
	}
	
	//2017-09-12,SKCHO, PORT EMPTY 알림
	if(theProcBank.m_nGoodInPortEmptyStep  == 1) //실행
	{	
		theUnitFunc.LampChange(LAMP_IDLE,2);
		theUnitFunc.DisplayPortEmptyMsg();
		theProcBank.m_nGoodInPortEmptyStep = 2;  //한번만 실행

		//20171108,JSJUNG, AGV IN IDLE TIME 시작
		if(theProcBank.m_bTRAYInIDLE == FALSE)
		{
			::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_IDLEAGVINSTART_UPDATE, 0,0); 
			theLog[LOG_LOAD_DELAY_TACT].AddBuf(_T("AGV IN IDLE TIME 시작"));

			//2018-04-20,SKCHO, AGV IN IDEL TIME시작할때 알람 발생
			theUnitFunc.SetError(ALARM_OKTRAY_IN_PORT_ALIGN_EMPTY);
		}
	}
	
	if(theProcBank.m_nGoodInPortEmptyStep  == 3)  //해제
	{
		if(theProcBank.m_nGoodInPortEmptyStep  != 2)
		{
			int nLampState = theUnitFunc.GetMachinetoLampState();
			theUnitFunc.LampChange((STATUS_LAMP_DESCRIPTION)nLampState,0);//램프 이전상태로 해제
			
		}

		theProcBank.m_nGoodInPortEmptyStep = 0;
	}
	
	//2017-09-12,SKCHO, PORT EMPTY 알림
	if(theProcBank.m_nNGInPortEmptyStep  == 1 )  //실행
	{
		theUnitFunc.LampChange(LAMP_IDLE,2);
		theUnitFunc.DisplayPortEmptyMsg();
		theProcBank.m_nNGInPortEmptyStep = 2;  //한번만 실행

		//2018-04-20,SKCHO, AGV NG IN PORT에 TRAY 없을때 알람 발생
		theUnitFunc.SetError(ALARM_NGTRAY_IN_PORT_ALIGN_EMPTY);
	}
	
	if(theProcBank.m_nNGInPortEmptyStep  == 3 )  //해제
	{
		if(theProcBank.m_nNGInPortEmptyStep  != 2) 
		{
			int nLampState = theUnitFunc.GetMachinetoLampState();
			theUnitFunc.LampChange((STATUS_LAMP_DESCRIPTION)nLampState,0);  //램프 이전상태로 해제
			//PORT5, READY TO LOAD, NONE
			//2018-03-09,MGYUN, PORT STATUS MELSEC 기록 추가
			theMonitorBank.WritePortStatus(5,1,0);
		}
		theProcBank.m_nNGInPortEmptyStep = 0;
	}

	CString str1;

	if(theConfigBank.m_Option.m_bUseASkip)
	{
		str1 += _T("\n\tA JIG SKIP(");
		str1 += theConfigBank.m_Option.m_strASkipDescription;
		str1 += _T(")\n\t");
		CGxUICtrl::SetStaticVisible(this,IDC_GXSTC_AJIG_SKIP,TRUE);
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_AJIG_SKIP, GXCOLOR_YELLOW);
		CGxUICtrl::SetStaticTextColor(this,IDC_GXSTC_AJIG_SKIP, GXCOLOR_RED);
		CGxUICtrl::SetStaticString(this,IDC_GXSTC_AJIG_SKIP, str1);
		str1 = _T("");
	}
	else
	{
		CGxUICtrl::SetStaticVisible(this,IDC_GXSTC_AJIG_SKIP,FALSE);
	}
	if(theConfigBank.m_Option.m_bUseBSkip)
	{
		str1 += _T("\n\tB JIG SKIP(");
		str1 += theConfigBank.m_Option.m_strBSkipDescription;
		str1 += _T(")\n\t");
		CGxUICtrl::SetStaticVisible(this,IDC_GXSTC_BJIG_SKIP,TRUE);
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_BJIG_SKIP, GXCOLOR_YELLOW);
		CGxUICtrl::SetStaticTextColor(this,IDC_GXSTC_BJIG_SKIP, GXCOLOR_RED);
		CGxUICtrl::SetStaticString(this,IDC_GXSTC_BJIG_SKIP, str1);
		str1 = _T("");
	}
	else
	{
		CGxUICtrl::SetStaticVisible(this,IDC_GXSTC_BJIG_SKIP,FALSE);
	}
	if(theConfigBank.m_Option.m_bUseCSkip)
	{
		str1 += _T("\n\tC JIG SKIP(");
		str1 += theConfigBank.m_Option.m_strCSkipDescription;
		str1 += _T(")\n\t");
		CGxUICtrl::SetStaticVisible(this,IDC_GXSTC_CJIG_SKIP,TRUE);
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_CJIG_SKIP, GXCOLOR_YELLOW);
		CGxUICtrl::SetStaticTextColor(this,IDC_GXSTC_CJIG_SKIP, GXCOLOR_RED);
		CGxUICtrl::SetStaticString(this,IDC_GXSTC_CJIG_SKIP, str1);
		str1 = _T("");
	}
	else
	{
		CGxUICtrl::SetStaticVisible(this,IDC_GXSTC_CJIG_SKIP,FALSE);
	}
	if(theConfigBank.m_Option.m_bUseDSkip)
	{
		str1 += _T("\n\tD JIG SKIP(");
		str1 += theConfigBank.m_Option.m_strDSkipDescription;
		str1 += _T(")\n\t");
		CGxUICtrl::SetStaticVisible(this,IDC_GXSTC_DJIG_SKIP,TRUE);
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_DJIG_SKIP, GXCOLOR_YELLOW);
		CGxUICtrl::SetStaticTextColor(this,IDC_GXSTC_DJIG_SKIP, GXCOLOR_RED);
		CGxUICtrl::SetStaticString(this,IDC_GXSTC_DJIG_SKIP, str1);
		str1 = _T("");
	}
	else
	{
		CGxUICtrl::SetStaticVisible(this,IDC_GXSTC_DJIG_SKIP,FALSE);
	}
	

	 CCellTag tagCell;
	tagCell = theCellBank.GetCellTag(CELL_POS_LD_CONV);	
	str.Format(_T("%d"),tagCell.m_nIndex);
	CGxUICtrl::SetStaticString(this,IDC_GXSTC_CELLMOVE_POS1,str);

	tagCell = theCellBank.GetCellTag(CELL_POS_LDROBOT);	
	str.Format(_T("%d"),tagCell.m_nIndex);
	CGxUICtrl::SetStaticString(this,IDC_GXSTC_CELLMOVE_POS2,str);

	tagCell = theCellBank.GetCellTag(CELL_POS_UPPER_SHUTTLE_CH1);	
	str.Format(_T("%d"),tagCell.m_nIndex);
	CGxUICtrl::SetStaticString(this,IDC_GXSTC_CELLMOVE_POS3,str);

	tagCell = theCellBank.GetCellTag(CELL_POS_UPPER_SHUTTLE_CH2);	
	str.Format(_T("%d"),tagCell.m_nIndex);
	CGxUICtrl::SetStaticString(this,IDC_GXSTC_CELLMOVE_POS4,str);

	tagCell = theCellBank.GetCellTag(CELL_POS_LOWER_SHUTTLE_CH1);	
	str.Format(_T("%d"),tagCell.m_nIndex);
	CGxUICtrl::SetStaticString(this,IDC_GXSTC_CELLMOVE_POS5,str);

	tagCell = theCellBank.GetCellTag(CELL_POS_LOWER_SHUTTLE_CH2);	
	str.Format(_T("%d"),tagCell.m_nIndex);
	CGxUICtrl::SetStaticString(this,IDC_GXSTC_CELLMOVE_POS6,str);

	tagCell = theCellBank.GetCellTag(CELL_POS_PA_TR_CH1);	
	str.Format(_T("%d"),tagCell.m_nIndex);
	CGxUICtrl::SetStaticString(this,IDC_GXSTC_CELLMOVE_POS7,str);

	tagCell = theCellBank.GetCellTag(CELL_POS_PA_TR_CH2);	
	str.Format(_T("%d"),tagCell.m_nIndex);
	CGxUICtrl::SetStaticString(this,IDC_GXSTC_CELLMOVE_POS8,str);

	tagCell = theCellBank.GetCellTag(CELL_POS_PA_STAGE_CH1);	
	str.Format(_T("%d"),tagCell.m_nIndex);
	CGxUICtrl::SetStaticString(this,IDC_GXSTC_CELLMOVE_POS9,str);

	tagCell = theCellBank.GetCellTag(CELL_POS_PA_STAGE_CH2);	
	str.Format(_T("%d"),tagCell.m_nIndex);
	CGxUICtrl::SetStaticString(this,IDC_GXSTC_CELLMOVE_POS10,str);


	DispCellID(CELL_POS_LD_TR_CH1);
	DispCellID(CELL_POS_LD_TR_CH2);
	DispCellID(CELL_POS_AZONE_CH1);
	DispCellID(CELL_POS_AZONE_CH2);
	DispCellID(CELL_POS_BZONE_CH1);
	DispCellID(CELL_POS_BZONE_CH2);
	DispCellID(CELL_POS_CZONE_CH1);
	DispCellID(CELL_POS_CZONE_CH2);
	DispCellID(CELL_POS_DZONE_CH1);
	DispCellID(CELL_POS_DZONE_CH2);
	DispCellID(CELL_POS_UD_TR_CH1);
	DispCellID(CELL_POS_UD_TR_CH2);
	DispCellID(CELL_POS_UD_STAGE_CH1);
	DispCellID(CELL_POS_UD_STAGE_CH2);
	DispCellID(CELL_POS_UD_ROBOT);	

	//2017-09-16,SKCHO
	if(theProcBank.m_nPACH1RetryStep == 1)
	{
		CGxUICtrl::SetStaticTextColor(this,IDC_GXSTC_CELLMOVE_POS9, GXCOLOR_LIGHT_CORAL);
		CGxUICtrl::SetStaticString(this,IDC_GXSTC_CELLMOVE_POS9, _T("R"));
	}
	else
	{
		//--CGxUICtrl::SetStaticString(this,IDC_GXSTC_CELLMOVE_POS9, _T(""));
	}
	if(theProcBank.m_nPACH2RetryStep == 1)
	{
		CGxUICtrl::SetStaticTextColor(this,IDC_GXSTC_CELLMOVE_POS10, GXCOLOR_LIGHT_CORAL);
		CGxUICtrl::SetStaticString(this,IDC_GXSTC_CELLMOVE_POS10, _T("R"));
	}
	else
	{
		//--CGxUICtrl::SetStaticString(this,IDC_GXSTC_CELLMOVE_POS10, _T(""));
	}
	//CELL INFO RETRY 표시
	if(theProcBank.m_nCH1CellInfoRetryStep == 1)
	{
		CGxUICtrl::SetStaticTextColor(this,IDC_GXSTC_CELLMOVE_POS13, GXCOLOR_LIGHT_CORAL);
		CGxUICtrl::SetStaticString(this,IDC_GXSTC_CELLMOVE_POS13, _T("R"));
	}

	if(theProcBank.m_nCH2CellInfoRetryStep == 1)
	{
		CGxUICtrl::SetStaticTextColor(this,IDC_GXSTC_CELLMOVE_POS14, GXCOLOR_LIGHT_CORAL);
		CGxUICtrl::SetStaticString(this,IDC_GXSTC_CELLMOVE_POS14, _T("R"));
	}

	//2017-11-17,SKCHO
	if(theProcBank.m_nPATRUpperGetRetryStep == 1 || theProcBank.m_nPATRLowerGetRetryStep == 1)
	{
		CGxUICtrl::SetStaticTextColor(this,IDC_GXSTC_CELLMOVE_POS8, GXCOLOR_LIGHT_CORAL);
		CGxUICtrl::SetStaticString(this,IDC_GXSTC_CELLMOVE_POS8, _T("R"));

		CGxUICtrl::SetStaticTextColor(this,IDC_GXSTC_CELLMOVE_POS7, GXCOLOR_LIGHT_CORAL);
		CGxUICtrl::SetStaticString(this,IDC_GXSTC_CELLMOVE_POS7, _T("R"));
	}


	//2017-12-24,SKCHO, 로봇 셀 집은 개수 표시
	if(theProcBank.GOODTrayBitY == 1)
	{
		str.Format(_T("%d"),theProcBank.GOODTrayBitX);
	}
	else if(theProcBank.GOODTrayBitY == 2)
	{
		str.Format(_T("%d"),(theProcBank.GOODTrayBitY)+theProcBank.GOODTrayBitX);
	}
	else if(theProcBank.GOODTrayBitY == 3)
	{
		str.Format(_T("%d"),(theProcBank.GOODTrayBitY+1)+theProcBank.GOODTrayBitX);
	}
	else if(theProcBank.GOODTrayBitY == 4)
	{
		str.Format(_T("%d"),(theProcBank.GOODTrayBitY+2)+theProcBank.GOODTrayBitX);
	}
	else
	{
		str.Format(_T("%d"),theProcBank.GOODTrayBitX);	
	}
	CGxUICtrl::SetStaticString(this,IDC_GXSTC_GOODTRAY,str);


	if(theProcBank.NGTrayBitY == 1)
	{
		str.Format(_T("%d"),theProcBank.NGTrayBitX);
	}
	else if(theProcBank.NGTrayBitY == 2)
	{
		str.Format(_T("%d"),(theProcBank.NGTrayBitY)+theProcBank.NGTrayBitX);
	}
	else if(theProcBank.NGTrayBitY == 3)
	{
		str.Format(_T("%d"),(theProcBank.NGTrayBitY+1)+theProcBank.NGTrayBitX);
	}
	else if(theProcBank.NGTrayBitY == 4)
	{
		str.Format(_T("%d"),(theProcBank.NGTrayBitY+2)+theProcBank.NGTrayBitX);
	}
	else
	{
		str.Format(_T("%d"),theProcBank.NGTrayBitX);
	}
	CGxUICtrl::SetStaticString(this,IDC_GXSTC_NGTRAY,str);	


	//2017-12-27,SKCHO, FDC WRITE 상태 표시
	if(theFDCBank.m_bFDCCheckFlag)
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_MAIN_FDC_STATE, GXCOLOR_LIGHT_BLUE);
	}
	else
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_MAIN_FDC_STATE, GXCOLOR_GOLD);
	}


}

//kjpark 20161021 Main Form Cell Status view 구현
void CFormMain::InitCellElement()
{
	CELL_ELEMENT cellelement;	 
	cellelement.bExistCell = CELL_STATUS_NONE;

	cellelement.nResourceID = IDC_GXSTC_CELLMOVE_POS1; 
	iMap.insert(pair<CELL_POS, CELL_ELEMENT>(CELL_POS_LD_CONV, cellelement));

	cellelement.nResourceID = IDC_GXSTC_CELLMOVE_POS2; 
	iMap.insert(pair<CELL_POS, CELL_ELEMENT>(CELL_POS_LDROBOT, cellelement));

	cellelement.nResourceID = IDC_GXSTC_CELLMOVE_POS3;
	iMap.insert(pair<CELL_POS, CELL_ELEMENT>(CELL_POS_UPPER_SHUTTLE_CH1, cellelement));

	cellelement.nResourceID = IDC_GXSTC_CELLMOVE_POS4;
	iMap.insert(pair<CELL_POS, CELL_ELEMENT>(CELL_POS_UPPER_SHUTTLE_CH2, cellelement));

	cellelement.nResourceID = IDC_GXSTC_CELLMOVE_POS5;
	iMap.insert(pair<CELL_POS, CELL_ELEMENT>(CELL_POS_LOWER_SHUTTLE_CH1, cellelement));

	cellelement.nResourceID = IDC_GXSTC_CELLMOVE_POS6;
	iMap.insert(pair<CELL_POS, CELL_ELEMENT>(CELL_POS_LOWER_SHUTTLE_CH2, cellelement));

	cellelement.nResourceID = IDC_GXSTC_CELLMOVE_POS7;
	iMap.insert(pair<CELL_POS, CELL_ELEMENT>(CELL_POS_PA_TR_CH1, cellelement));

	cellelement.nResourceID = IDC_GXSTC_CELLMOVE_POS8;
	iMap.insert(pair<CELL_POS, CELL_ELEMENT>(CELL_POS_PA_TR_CH2, cellelement));

	cellelement.nResourceID = IDC_GXSTC_CELLMOVE_POS9;
	iMap.insert(pair<CELL_POS, CELL_ELEMENT>(CELL_POS_PA_STAGE_CH1, cellelement));

	cellelement.nResourceID = IDC_GXSTC_CELLMOVE_POS10;
	iMap.insert(pair<CELL_POS, CELL_ELEMENT>(CELL_POS_PA_STAGE_CH2, cellelement));

	cellelement.nResourceID = IDC_GXSTC_CELLMOVE_POS11;
	iMap.insert(pair<CELL_POS, CELL_ELEMENT>(CELL_POS_LD_TR_CH1, cellelement));

	cellelement.nResourceID = IDC_GXSTC_CELLMOVE_POS12;
	iMap.insert(pair<CELL_POS, CELL_ELEMENT>(CELL_POS_LD_TR_CH2, cellelement));

	cellelement.nResourceID = IDC_GXSTC_CELLMOVE_POS13;
	iMap.insert(pair<CELL_POS, CELL_ELEMENT>(CELL_POS_AZONE_CH1, cellelement));

	cellelement.nResourceID = IDC_GXSTC_CELLMOVE_POS14;
	iMap.insert(pair<CELL_POS, CELL_ELEMENT>(CELL_POS_AZONE_CH2, cellelement));

	cellelement.nResourceID = IDC_GXSTC_CELLMOVE_POS15;
	iMap.insert(pair<CELL_POS, CELL_ELEMENT>(CELL_POS_BZONE_CH1, cellelement));

	cellelement.nResourceID = IDC_GXSTC_CELLMOVE_POS16;
	iMap.insert(pair<CELL_POS, CELL_ELEMENT>(CELL_POS_BZONE_CH2, cellelement));

	cellelement.nResourceID = IDC_GXSTC_CELLMOVE_POS17;
	iMap.insert(pair<CELL_POS, CELL_ELEMENT>(CELL_POS_CZONE_CH1, cellelement));

	cellelement.nResourceID = IDC_GXSTC_CELLMOVE_POS18;
	iMap.insert(pair<CELL_POS, CELL_ELEMENT>(CELL_POS_CZONE_CH2, cellelement));

	cellelement.nResourceID = IDC_GXSTC_CELLMOVE_POS19;
	iMap.insert(pair<CELL_POS, CELL_ELEMENT>(CELL_POS_DZONE_CH1, cellelement));

	cellelement.nResourceID = IDC_GXSTC_CELLMOVE_POS20;
	iMap.insert(pair<CELL_POS, CELL_ELEMENT>(CELL_POS_DZONE_CH2, cellelement));

	cellelement.nResourceID = IDC_GXSTC_CELLMOVE_POS21;
	iMap.insert(pair<CELL_POS, CELL_ELEMENT>(CELL_POS_UD_TR_CH1, cellelement));

	cellelement.nResourceID = IDC_GXSTC_CELLMOVE_POS22;
	iMap.insert(pair<CELL_POS, CELL_ELEMENT>(CELL_POS_UD_TR_CH2, cellelement));

	cellelement.nResourceID = IDC_GXSTC_CELLMOVE_POS23;
	iMap.insert(pair<CELL_POS, CELL_ELEMENT>(CELL_POS_UD_STAGE_CH1, cellelement));

	cellelement.nResourceID = IDC_GXSTC_CELLMOVE_POS24;
	iMap.insert(pair<CELL_POS, CELL_ELEMENT>(CELL_POS_UD_STAGE_CH2, cellelement));

	cellelement.nResourceID = IDC_GXSTC_CELLMOVE_POS25;
	iMap.insert(pair<CELL_POS, CELL_ELEMENT>(CELL_POS_UD_ROBOT, cellelement));
	//2018-05-16,MGYUN,추가
	cellelement.nResourceID = IDC_GXSTC_CELLMOVE_POS39;
	iMap.insert(pair<CELL_POS, CELL_ELEMENT>(CELL_POS_UD_CONV, cellelement));
}


// 지정 화면으로 폼을 변경한다.
void CFormMain::ChangeForm(int nIdx, BOOL bForce)
{
	// CMainFrame에게 Message로 호면 전환을 지시한다.
	::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_CHANGE_VIEW, (WPARAM)nIdx, (LPARAM)bForce);
}

void CFormMain::ClickBtnModePM()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_PAUSE || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_INIT
		|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_FAULT || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_INTERLOCK
		|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_CHECK)//2018-04-26,NAM
	{
		pMain->setModePM(TRUE);
		ChangeForm(eFORM_PM);
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


void CFormMain::ClickGxbtnStart()
{
	//2017-12-18,SKCHO, ROBOT ORGINE 동작시에는 진행안되게
	if(m_nLDRobotOrgSeq != seqLdRobot_idle)
		return;


	CGxMsgBox	dlgMsgBox;
	CString     strMsg;
	CString     strMsg1;
	CString     strMsg2;

	JIG_ID nJig =JIG_ID_A;
	INPUT_ID nINPUTID_CH1 =X_A_JIG_CH1_VAC_SEN , nINPUTID_CH2=X_A_JIG_CH2_VAC_SEN;
	nJig = (JIG_ID)(theUnitFunc.AMT_GetAutoJigID((ZONE_ID)ZONE_ID_D));	
	if(nJig == JIG_ID_A)
	{
		nINPUTID_CH1 = X_A_JIG_CH1_VAC_SEN;
		nINPUTID_CH2 = X_A_JIG_CH2_VAC_SEN;		 
	}
	else if(nJig == JIG_ID_B)
	{
		nINPUTID_CH1 = X_B_JIG_CH1_VAC_SEN;
		nINPUTID_CH2 = X_B_JIG_CH2_VAC_SEN;		
	}
	else if(nJig == JIG_ID_C)
	{
		nINPUTID_CH1 = X_C_JIG_CH1_VAC_SEN;
		nINPUTID_CH2 = X_C_JIG_CH2_VAC_SEN;		
	}
	else if(nJig == JIG_ID_D)
	{
		nINPUTID_CH1 = X_D_JIG_CH1_VAC_SEN;
		nINPUTID_CH2 = X_D_JIG_CH2_VAC_SEN;		
	}

	//2017-12-05, 전체 지그 스킵 상태에서는 런 안됨
	if(theConfigBank.m_Option.m_bUseASkip
	   && theConfigBank.m_Option.m_bUseBSkip 
	   && theConfigBank.m_Option.m_bUseCSkip
	   && theConfigBank.m_Option.m_bUseDSkip)
	{

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("모든 JIGS SKIP으로 시작 할 수 없습니다.."),	//한
				_T("ALL JIGS SKIP NO RUN."),		//영
				_T("ALL JIGS SKIP NO RUN.") );		//베

		dlgMsgBox.DoModal();
		return;
	}


	if(theProcBank.GetCimState() == 0)  //offline
	{

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("CIM 프로그램을 Remote로 변경해 주세요."),	//한
				_T("Please! CIM Program State change to Remote."),		//영
				_T("Please! CIM Program State change to Remote.") );		//베

		dlgMsgBox.DoModal();
		return;
	}	

	//2018-06-09,SKCHO, PORT EMPTY/FULL 알람 발생 상태에서 시작 안되게 수정
	//2018-06-21,GHLEE, 홍영기 과장님 요청으로 START 시 TRAY ALARM 확인 하지 않도록 변경
	//if(theProcBank.GetTrayAlarmChk())
	//{
	//	if(theProcBank.m_bFaultOfTray[2] == TRUE)//ALARM_NGTRAY_IN_PORT_ALIGN_EMPTY)
	//	{
	//		strMsg.Format(_T("NGTRAY_IN_PORT_ALIGN_EMPTY 알람으로 인해 초기화 할 수 없습니다."));
	//		strMsg1.Format(_T("Can not Alarm Reset, Because NGTRAY_IN_PORT_ALIGN_EMPTY Alarm"));
	//		strMsg2.Format(_T("Can not Alarm Reset, Because NGTRAY_IN_PORT_ALIGN_EMPTY Alarm"));
	//
	//	}
	//	else if(theProcBank.m_bFaultOfTray[0] == TRUE)//ALARM_OKTRAY_IN_PORT_ALIGN_EMPTY)
	//	{
	//		strMsg.Format(_T("OKTRAY_IN_PORT_ALIGN_EMPTY 알람으로 인해 초기화 할 수 없습니다."));
	//		strMsg1.Format(_T("Can not Alarm Reset, Because OKTRAY_IN_PORT_ALIGN_EMPTY Alarm"));
	//		strMsg2.Format(_T("Can not Alarm Reset, Because OKTRAY_IN_PORT_ALIGN_EMPTY Alarm"));
	//
	//	}
	//	else if(theProcBank.m_bFaultOfTray[1] == TRUE)// ALARM_OKTRAY_OUT_BUFF_PORT_FULL)
	//	{
	//		strMsg.Format(_T("OKTRAY_OUT_BUFF_PORT_FULL 알람으로 인해 초기화 할 수 없습니다."));
	//		strMsg1.Format(_T("Can not Alarm Reset, Because OKTRAY_OUT_BUFF_PORT_FULL Alarm"));
	//		strMsg2.Format(_T("Can not Alarm Reset, Because OKTRAY_OUT_BUFF_PORT_FULL Alarm"));
	//	}
	//
	//	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	//	dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
	//	dlgMsgBox.SetMessage(FALSE, 
	//			strMsg,	//한
	//			strMsg1,		//영
	//			strMsg2);		//베
	//
	//	dlgMsgBox.DoModal();
	//	return;
	//
	//}

	if ( theProcBank.m_AlarmID != ALM_NONE )
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("알람을 초기화 해주세요."),	//한
				_T("Please! Alarm Clear."),		//영
				_T("Please! Alarm Clear.") );		//베

		dlgMsgBox.DoModal();
		return;
	}

	//2018-03-16,MGYUN, 런 시작 전 모터 서보 체크
	if (MotorServoCheck()==FALSE) 
		return;

	if(theUnitFunc.DoorInterLockChkStart() == FALSE)
		return;

	if(theUnitFunc.LightCurtainChkStart() == FALSE)
		return;

	// 2016.12.21
	if(theUnitFunc.DoorKeyModeChkStart() == FALSE)
		return; 
	
	if(theProcBank.m_bOriginSuccess == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("Origin 해주십시오."),	//한
				_T("Origin Please."),		//영
				_T("Xin vui lòng xứ.") );		//베

		dlgMsgBox.DoModal();
		return;
	}

	//2017-11-15,SKCHO, 시작시 버큠과 CELL 정보 정합성 체크
	if(CheckCellDataAndVac(nINPUTID_CH1   ,nINPUTID_CH2) == FALSE)
	{
		return;
	}	

	//런 돌리기전 통신 연결 상태 확인 한다. 2017/04/30 HSK
	BOOL bRet = theUnitFunc.NetWorkCheck();
	if(bRet == FALSE)
		return;

	//2017-11-27,SKCHO JUDGE OK 시 메세지 출력
	if(theConfigBank.m_Option.m_bUseJudgeOK)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(TRUE, 
				_T("판정 강제 OK 옵션이 켜 있습니다. 그래도 실행하시겠습니까?"),	//한
				_T("JUGE OK OPTION IS ON. Do you Run Continue?"),		//영
				_T("JUGE OK OPTION IS ON. Do you Run Continue?") );		//베

		if(dlgMsgBox.DoModal() != IDOK)
		    return;
	}

	
	
		//2017-12-18,SKCHO
	//theDeviceMelsec.RobotBitOn(LDROBOT_Y_SQERRRTN);   //error retrun
	BOOL bRtn[3] = {TRUE};
	
	bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_CVGET);
	bRtn[1] = (BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_LOWERSTGPUT);
	bRtn[2] = (BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_UPPERSTGPUT);
	if(bRtn[0] == FALSE 
		||bRtn[1] == FALSE
		|| bRtn[2] == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		
		if(bRtn[0] == FALSE)  
		{   if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_CVGETERR) == TRUE)  //1010,78LINE(ERR_Return) -> 원점이동후 셀 제거
			{
				strMsg = _T("Loader Robot GetInterlock 상태입니다.\n\t 원점이동 완료 후 Cell을 제거하여 주세요.");
				strMsg1 = _T("Loader Robot GetInterlock State.\n\t After Finished Origine, Please Take Loader Robot's Cell Get Out.");
				strMsg2 = _T("Loader Robot GetInterlock State.\n\t After Finished Origine, Please Take Loader Robot's Cell Get Out.");
			}
			else  //1010,64LINE(COMP_RETURN) -> 그냥 원점만 해도 됨
			{
				strMsg = _T("Loader Robot GetInterlock 상태입니다.\n\t 원점이동 완료 후 시작 해 주세요.");
				strMsg1 = _T("Loader Robot GetInterlock State.\n\t After Finished Origine, Please Start.");
				strMsg2 = _T("Loader Robot GetInterlock State.\n\t After Finished Origine, Please Start.");
			}
		}
		else if(bRtn[1] == FALSE)
		{
			strMsg = _T("Loader Robot LowerShuttle GetInterlock 상태입니다.\n\t원점이동 완료 후 시작 해 주세요.");
			strMsg1 = _T("Loader Robot LowerShuttle GetInterlock State.\n\t After Finished Origine, Please Start.");
			strMsg2 = _T("Loader Robot LowerShuttle GetInterlock State.\n\t After Finished Origine, Please Start.");
		}
		else if(bRtn[2] == FALSE)
		{
			strMsg = _T("Loader Robot UpperShuttle GetInterlock 상태입니다.\n\t원점이동 완료 후 시작 해 주세요.");
			strMsg1 = _T("Loader Robot UpperShuttle GetInterlock State.\n\t After Finished Origine, Please Start.");
			strMsg2 = _T("Loader Robot UpperShuttle GetInterlock State.\n\t After Finished Origine, Please Start.");
		}
	
	
		dlgMsgBox.SetMessage(FALSE, 
				strMsg,	//한
				strMsg1,		//영
				strMsg2);		//베

		
		if(dlgMsgBox.DoModal())
		{
			theUnitFunc.LDRobotBitAllOff(E_ORG);
			m_nLDRobotOrgSeq = seqLdRobot_1;
			KillTimer(LDROBOTORG_TIMER);
			SetTimer(LDROBOTORG_TIMER,200,NULL);			
			return;
		}

	}

	/*if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_CVGET) == FALSE)
	{

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("Loader Robot GetInterlock 상태입니다. Loader Robot Origin을 실행합니다."),	//한
				_T("Loader Robot GetInterlock Signal.Start Loader Robot Origin."),		//영
				_T("Loader Robot GetInterlock Signal.Start Loader Robot Origin.") );		//베

		if(dlgMsgBox.DoModal())
		{

			theUnitFunc.LDRobotBitAllOff(E_ORG);
			m_nLDRobotOrgSeq = seqLdRobot_1;
			KillTimer(LDROBOTORG_TIMER);
			SetTimer(LDROBOTORG_TIMER,200,NULL);

		    return;
		}

	}
	*/

	if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MOTORON_Y_LOWSPEED) ==FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("Not Energize Signal. Start Loader Robot Origin."),	//한
				_T("Not Energize Signal. Start Loader Robot Origin."),		//영
				_T("Not Energize Signal. Start Loader Robot Origin.") );		//베

		if(dlgMsgBox.DoModal())
		{

			theUnitFunc.LDRobotBitAllOff(E_ORG);
			m_nLDRobotOrgSeq = seqLdRobot_1;
			KillTimer(LDROBOTORG_TIMER);
			SetTimer(LDROBOTORG_TIMER,200,NULL);

		    return;
		}

	}
	
	if( (theProcBank.m_AlarmID == ALM_NONE || theProcBank.m_AlarmID == ALARM_OKTRAY_IN_PORT_ALIGN_EMPTY || theProcBank.m_AlarmID == ALARM_OKTRAY_OUT_BUFF_PORT_FULL || theProcBank.m_AlarmID == ALARM_NGTRAY_IN_PORT_ALIGN_EMPTY) 
		&& (theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_PAUSE || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_INIT
		|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_FAULT || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_INTERLOCK))
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(TRUE, 
			_T("지금 RUN START 하시겠습니까?"), 
			_T("Do you want to start now?"),		
			_T("Bạn có muốn bắt đầu bây giờ?") );

		int nRet = dlgMsgBox.DoModal();
		if(nRet != IDOK)
			return;

		//2017-05-21,skcho 비가동 로그 적기 위해서 추가
		if ( theLossTimeBank.m_rLosstimeRecord.sAlarmType != _T("A") )					// Alarm에 의한 정지가 아니 였다면,
		{	// MD가 아닌경우 무의미
			::GetLocalTime( &(theLossTimeBank.m_rLosstimeRecord.tmResetTime) );			// Reset 시간을 현재 시각으로 맞춘다.
		}
		// 비가동 종료 시각
		::GetLocalTime( &(theLossTimeBank.m_rLosstimeRecord.tmEndTime) );				// 처리 종료하고 다시 시작 시간
     		 
		theLossTimeBank.WriteLossTimeRecord( theLossTimeBank.m_rLosstimeRecord, FALSE );			 
		theLossTimeBank.ClearLosstimeInfo();
		//

		if(theRecipeBank.m_Cell.m_bFPCBuse)
			theUnitFunc.FPCBUseSol(TRUE);
		else
			theUnitFunc.FPCBUseSol(FALSE);

		if((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_ALARM_Y_EXTRESET))
			theDeviceMelsec.RobotBitOn(UDROBOT_X_RUNNING_Y_ERRRESET);
		else if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_ALARM_Y_EXTRESET))
			theDeviceMelsec.RobotBitOn(LDROBOT_X_RUNNING_Y_ERRRESET);


		// 2017.2.8 bgkim
		// 앞 설비에 Pause 해제 알려줌
		theUnitFunc.SendRcvPause(OFF);
		// Job End 타이머 시작
//		theProcBank.m_timerJobEnd.Start();

		theProcBank.m_CellLoadChkTact.Start();

		theProcBank.m_CellLoadChk = FALSE;

		theProcBank.m_CellLoadRunChk = FALSE;
	
		// 2017.2.11 bgkim
		// Lot End 시 Tray TR 작업이 끝나고 Out Lift Z가 동작되도록 하는 변수 초기화
		theProcBank.m_bOKTrayTRLotEnd = FALSE;
		theProcBank.m_bNGTrayTRLotEnd = FALSE;

		// 트레이 배출 후에 셀이 쌓인 첫번째 트레이를 빼기전에 LOT END할 경우 TRAY TR이 GET하지 않는 현상 막기 위함.
		theProcBank.m_bCarrierAssign = FALSE;

		// 혹시 몰라서 스타트 할때 메뉴얼 모드 꺼준다 2017/06/20 HSK.
		theDeviceMelsec.RobotBitOff(LDROBOT_X_MANUALIN_Y_MANUALSELECT);
		theDeviceMelsec.RobotBitOff(UDROBOT_X_MANUALIN_Y_MANUALSELECT);

		// Live 모드로 해놓고 끄지 않는 경향이 있어 운영에서 스타트 할 때 끄도록 해준다 2017/06/10 HSK.
		theSocketInterFace.SendMsgToGrabMode();
		/*
		if(theUnitFunc.CellDataMachineChk(CELL_POS_LDROBOT,CELL_POS_UD_ROBOT) == FALSE)
		{
			//심퀄 요청사항 - 장비 상태 보고시 AVAILABILITYSTATE 가 1에서 2로 변경되지 않는 부분이 있어서 추가 2017/06/15 JSJUNG.
			//theProcBank.ChangeMachineState(CONST_MACHINE_STATE::MACHINE_RUN);
			theProcBank.ChangeMachineState(CONST_MACHINE_STATE::MACHINE_IDLE);

			::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_IDLECHAGNE_UPDATE, 0,0); 
			theLog[LOG_LOAD_DELAY_TACT].AddBuf(_T("설비 IDLE 변경 시작"));
			theLog[LOG_SYSTEM].AddBuf(_T("자동 설비 IDLE 변경"));
		}
		else if(theUnitFunc.CellDataMachineChk(CELL_POS_LDROBOT,CELL_POS_UD_ROBOT))
		{
			theProcBank.ChangeMachineState(CONST_MACHINE_STATE::MACHINE_RUN);
		}
		*/

		//2018-04-20,SKCHO, RESTART 일때는 CHECK 이후에 설비 상태를 RUN으로 변경한다.
		//앞단 설비에서 셀받는 부분을 설비 RUN과 관계 없이 진행 해서 설비가 멈춰 있는 경우에 CELL이 LD CONV까지 오는 경우가 생기는데
		// 설비가 IDLE에서 알람이 난 상태 였는데 셀이 들어오고 설비 스타트를 하면 설비 상태가 RUN으로 바로 바뀌어 버리는데 이때
		//AVAILABILITYSTATE가 1에서 2로 안바뀐다. 때문에 설비 RUN으로 바꾸는 타이밍을 로더 로봇이 셀을 집고 난 후로 변경 한다. 2017/06/17 HSK. 
		/*if(theUnitFunc.CellDataMachineChk(CELL_POS_LDROBOT,CELL_POS_UD_ROBOT))
		{
			//2017-07-20, jsjung, 설비 IDLE->RUN 변경시 IDLE TIME 종료
			if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)
			{
				::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_IDLECHAGNERUN_UPDATE, 0,0); 
				theLog[LOG_LOAD_DELAY_TACT].AddBuf(_T("설비 IDLE->RUN 변경"));
			}
			theProcBank.ChangeMachineState(CONST_MACHINE_STATE::MACHINE_RUN);
		}
		else
		{
			theProcBank.ChangeMachineState(CONST_MACHINE_STATE::MACHINE_IDLE);
			::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_IDLECHAGNE_UPDATE, 0,0); 
			theLog[LOG_LOAD_DELAY_TACT].AddBuf(_T("설비 IDLE 변경 시작"));
		}
		*/
		//2018-04-20,SCKHO
		theProcBank.ChangeMachineState(CONST_MACHINE_STATE::MACHINE_CHECK);


	
		theUnitFunc.SetOutPutIO(Y_START_SW_LAMP,ON);
		theUnitFunc.SetOutPutIO(Y_STOP_SW_LAMP,OFF);

		//메뉴얼에서 동작 시켜 놓고 안끄는 경우가 있어서 설비 스타트시 꺼준다.2017/04/30 HSK.
		theUnitFunc.ConvRun(GOOD_IN_TRAY_PORT,OFF);
		theUnitFunc.ConvRun(GOOD_OUT_TRAY_PORT,OFF);
		theUnitFunc.ConvRun(NG_TRAY_PORT,OFF);
		theUnitFunc.BufConvRun(GOOD_IN_TRAY_PORT,OFF);
		theUnitFunc.BufConvRun(GOOD_OUT_TRAY_PORT,OFF);
		theUnitFunc.BufConvRun(NG_TRAY_PORT,OFF);

		//2017-11-27,SKCHO 위치 이동
		//런 돌리기전 통신 연결 상태 확인 한다. 2017/04/30 HSK
		//BOOL bRet = theUnitFunc.NetWorkCheck();
		//if(bRet == FALSE)
			//return;

		//2017-07-17,SKCHO, 런 시작시 포트 이상 없다는 보고
		if(theProcBank.m_nStatePortType == 3)
		{
			 theSocketInterFace.SendCmdTrayToDataPC(EQUIP_SET_PORTSTATECHANGE_REQUEST , _T("1"), _T("UP"));
			 theSocketInterFace.SendCmdTrayToDataPC(EQUIP_SET_PORTSTATECHANGE_REQUEST , _T("2"), _T("UP"));
		}
		else if(theProcBank.m_nStatePortType == 2)
		{			
			 theSocketInterFace.SendCmdTrayToDataPC(EQUIP_SET_PORTSTATECHANGE_REQUEST , _T("2"), _T("UP"));
		}
		else if(theProcBank.m_nStatePortType == 1)
		{
			 theSocketInterFace.SendCmdTrayToDataPC(EQUIP_SET_PORTSTATECHANGE_REQUEST , _T("1"), _T("UP"));
		}

		// 뺑뺑이 모드시에 강제로 넣어줘야할 내용들을 넣어 준다 아직 미사용 2017/04/26 HSK
		/*
		if (theConfigBank.m_Option.m_bUseRepeatInsp)
			theUnitFunc.Set_Table_CellData();
		*/

		//2017.10.26 JSJUNG. 트레이 비전 추가 사용 유무 로봇 송신
		if(theConfigBank.m_Option.m_bUseTrayVI)
		{
			theDeviceMelsec.RobotBitOn(UDROBOT_X_TRAYVIREADY_Y_TRAYVIUSE);  //트레이 비젼 사용 유무 비트 
		}
		else
		{
			theDeviceMelsec.RobotBitOff(UDROBOT_X_TRAYVIREADY_Y_TRAYVIUSE); //트레이 비젼 사용 유무 비트 
		}

		//2017.10.31, JSJUNG. OKOUTTray에 사용하는 Tray Align옵션 강제 False (On 하는 애들이 있어서)
		// 추후 전체 정리 필요함
		theConfigBank.m_Option.m_bUseTrayAlign = FALSE;
	}
}

void CFormMain::ClickGxbtnStop()
{
	
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)
	{
		theProcBank.m_bUserStop = TRUE;
	 //2017-05-20,skcho, TP CODE 버튼 눌렀을때 상태 보고 하는 위치로 이동
	 //	theProcBank.ChangeMachineState(CONST_MACHINE_STATE::MACHINE_PAUSE);
	

		// 비가동 log용 데이터 지정
		theLossTimeBank.m_rLosstimeRecord.sAlarmType	= _T("-");							// Alarm 발생으로 멈춘게 아니다.
		theLossTimeBank.m_rLosstimeRecord.sAlarmCode   = _T("");
		theLossTimeBank.m_rLosstimeRecord.sRepairCode	= _T("");
		//::GetLocalTime( &(m_pMCMain->m_rLosstimeRecord.tmStartTime) );			// 발생시각 -> 비가동 로그 항목 입력하는 곳에서 지정
		theLossTimeBank.m_rLosstimeRecord.nMode	= eMODE_STATE_AutoToStop;				// 현재 비가동 기록 모드 0: STOP으로 전환, 1:Auto로 전환

		::PostMessage( ( (CMainFrame*)AfxGetMainWnd() )->m_hWnd, GxMSG_GUI_SHOW_DLG, (WPARAM)eDLG_Losstime, (LPARAM)1);	

		// 정지버튼눌러서 세울 경우엔 센서 감지 이동 옵션일 경우 계속 굴러 간다. 2017/06/08 HSK.
		if(theConfigBank.m_Option.m_bLoadPitchIn == FALSE)
			theUnitFunc.LDConvStop();

		theProcBank.m_nStatePortType = 0;  
	}

	if (theConfigBank.m_Option.m_bUseDryRun)
	{
		//2018-03-15,MGYUN, 타이머를 죽이지 않아 정지 눌렀을 때 죽이게
		KillTimer(DRYRUN_TIMER);
		theUnitFunc.CellData_SetDryRun(1); //정지
	}
	else if (!theConfigBank.m_Option.m_bUseDryRun)
	{
		//2018-03-15,MGYUN, 타이머를 죽이지 않아 정지 눌렀을 때 죽이게
		KillTimer(DRYRUN_TIMER);
		theUnitFunc.CellData_SetDryRun(1); //정지
	}

}


//void CFormMain::ClickGxbtnTestIn()
//{
//	return ;
//
//	CCellTag tagCell;
//	tagCell = theCellBank.CreateCellTag();
//	theCellBank.SetCellTag(CELL_POS_LD_CONV,tagCell);
//	theUnitFunc.TactUpdate(LOADER_TACT);
//
//	tagCell = theCellBank.CreateCellTag();
//	theCellBank.SetCellTag(CELL_POS_UD_ROBOT,tagCell);
//	theUnitFunc.TactUpdate(UNLOADER_TACT);
//
//	tagCell = theCellBank.CreateCellTag();
//	theCellBank.SetCellTag(CELL_POS_AZONE_CH1,tagCell);
//
//	tagCell = theCellBank.CreateCellTag();
//	theCellBank.SetCellTag(CELL_POS_BZONE_CH1,tagCell);
//
//	tagCell = theCellBank.CreateCellTag();
//	theCellBank.SetCellTag(CELL_POS_CZONE_CH1,tagCell);
//
//	tagCell = theCellBank.CreateCellTag();
//	theCellBank.SetCellTag(CELL_POS_DZONE_CH1,tagCell);
//
//	CCellInfo* pCellInfo;
//	pCellInfo =  theCellBank.GetCellInfo(CELL_POS_DZONE_CH1);
//	pCellInfo->m_JigId = JIG_ID_A;
//	pCellInfo->m_JigCh = JIG_CH_1;
//	pCellInfo->m_strInnerID = _T("111111"); 
//	pCellInfo->m_strCellID = _T("gasdfqweg");
//	pCellInfo->LoaderAlignResult = ALIGN_RESULT_GOOD;
//	pCellInfo->Pre_AlignResultXY = ALIGN_RESULT_FAIL;
//	pCellInfo->m_Zone_A.CellLoadingClass = GOOD_CELL;
//	pCellInfo->m_Zone_B.m_nVI_AlignResult = ALIGN_RESULT_GOOD;
//	pCellInfo->m_Zone_C.CZoneClass = BIN2_CELL;
//	pCellInfo->m_Zone_C.CZoneDefect = _T("GOOD");
//
//	theUnitFunc.ZoneTimeStart();
//	Sleep(2222);
//	theUnitFunc.ZoneTimeEnd(ZONE_ID_A);
//	theUnitFunc.ZoneTimeEnd(ZONE_ID_B);
//	theUnitFunc.ZoneTimeEnd(ZONE_ID_C);
//	theUnitFunc.ZoneTimeEnd(ZONE_ID_D);
//
//	theUnitFunc.TactUpdate(AZONE_TACT);
//	theUnitFunc.TactUpdate(BZONE_TACT);
//	theUnitFunc.TactUpdate(CZONE_TACT);
//	theUnitFunc.TactUpdate(DZONE_TACT);
//	return;
//	theUnitFunc.TrayData_Create(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_ALIGN);
//	theUnitFunc.TrayData_Create(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_LIFT);
//	theUnitFunc.TrayData_Create(NG_TRAY_PORT,TRAY_INFO_POS_NG_IN_ALIGN);
//	theUnitFunc.TrayData_Create(NG_TRAY_PORT,TRAY_INFO_POS_NG_IN_LIFT_ARM);
//	return;
//	theUnitFunc.SetOutPutIO(Y_NG_TRAY_MUTING_LAMP_ON,ON);
//	theUnitFunc.SetOutPutIO(Y_NG_TRAY_OUT_REQ_SW,ON);
//	theUnitFunc.SetOutPutIO(Y_NG_TRAY_MUTE_ON,ON);
//}


//void CFormMain::ClickGxbtnTestOut()
//{
//	return;
//	theUnitFunc.SetOutPutIO(Y_NG_TRAY_MUTING_LAMP_ON,OFF);
//	theUnitFunc.SetOutPutIO(Y_NG_TRAY_OUT_REQ_SW,OFF);
//	theUnitFunc.SetOutPutIO(Y_NG_TRAY_MUTE_ON,OFF);
//	theUnitFunc.SetOutPutIO(Y_NG_TRAY_OUT_COMP_SW,ON);
//	theUnitFunc.TrayData_RemoveData(NG_TRAY_PORT,TRAY_INFO_POS_NG_OUT_COMP_BUFFER);
//}


void CFormMain::ClickGxstcCellmovePos1()
{
	CGxMsgBox	dlgMsgBox;
	BOOL bResult = FALSE;
	bResult = DispCellTag(CELL_POS_LD_CONV);
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)//2017-03-02, skcho, 런상태에서는 무시
	{
		return;
	}
	if(bResult)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(TRUE, 
				_T("로드 컨베어  셀 정보를 삭제 하시겠습니까?."),	//한
				_T("Load Conveyer Cell Info Delete?"),		//영
				_T("Load Conveyer Cell Info Delete?") );		//베

		if(dlgMsgBox.DoModal() == IDOK)
		{
			//theCellBank.RemoveCellTag(CELL_POS_LD_CONV);
			theCellBank.RemoveCellTagCellInfo(CELL_POS_LD_CONV);
			theProductBank.UpdateMainInputCountMinus();
		}
		else
		{
			return;
		}
	}	
}


void CFormMain::ClickGxstcCellmovePos2()
{	

	CGxMsgBox	dlgMsgBox;
	BOOL bResult = FALSE;
	bResult = DispCellTag(CELL_POS_LDROBOT);
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)//2017-03-02, skcho, 런상태에서는 무시
	{
		return;
	}
	if(bResult)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(TRUE, 
				_T("로드 로봇 셀 정보를 삭제 하시겠습니까?"),	//한
				_T("Load Robot  Cell Info Delete?."),		//영
				_T("Load Robot  Cell Info Delete?") );		//베

		if(dlgMsgBox.DoModal() == IDOK)
		{
			//theCellBank.RemoveCellTag(CELL_POS_LDROBOT);
			theCellBank.RemoveCellTagCellInfo(CELL_POS_LDROBOT);
			theProductBank.UpdateMainInputCountMinus();				
		}
		else
		{
			return;
		}
	}
	
}


void CFormMain::ClickGxstcCellmovePos4()
{
	
	CGxMsgBox	dlgMsgBox;
	BOOL bResult = FALSE;
	bResult =DispCellTag(CELL_POS_UPPER_SHUTTLE_CH2);
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)//2017-03-02, skcho, 런상태에서는 무시
	{
		return;
	}
	if(bResult)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(TRUE, 
				_T("상위 셔틀 CH2 셀 정보를 삭제 하시겠습니까?"),	//한
				_T("Upper Shuttle Ch2 Cell Info Delete?."),		//영
				_T("Upper Shuttle Ch2  Cell Info Delete?") );		//베

		if(dlgMsgBox.DoModal() == IDOK)
		{
			//theCellBank.RemoveCellTag(CELL_POS_UPPER_SHUTTLE_CH2);
			theCellBank.RemoveCellTagCellInfo(CELL_POS_UPPER_SHUTTLE_CH2);
			theProductBank.UpdateMainInputCountMinus();
			
			//2017-11-20,YMG 잔압이 남아 있어 BLOW를 불고 꺼줌
			theUnitFunc.UpperShuttle_VacuumOn(VAC_OFF,JIG_CH_2);
			Sleep(100);
			theUnitFunc.UpperShuttle_BlowOff(JIG_CH_2);
			
		}
		else
		{
			return;
		}
	}
}


void CFormMain::ClickGxstcCellmovePos3()
{		
	CGxMsgBox	dlgMsgBox;
	BOOL bResult = FALSE;
	bResult = DispCellTag(CELL_POS_UPPER_SHUTTLE_CH1);
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)//2017-03-02, skcho, 런상태에서는 무시
	{
		return;
	}
	if(bResult)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(TRUE, 
				_T("상위 셔틀 CH1 셀 정보를 삭제 하시겠습니까?"),	//한
				_T("Upper Shuttle Ch1 Cell Info Delete?."),		//영
				_T("Upper Shuttle Ch1  Cell Info Delete?") );		//베

		if(dlgMsgBox.DoModal() == IDOK)
		{
			//theCellBank.RemoveCellTag(CELL_POS_UPPER_SHUTTLE_CH1);
			theCellBank.RemoveCellTagCellInfo(CELL_POS_UPPER_SHUTTLE_CH1);
			theProductBank.UpdateMainInputCountMinus();
			
			//2017-11-20,YMG 잔압이 남아 있어 BLOW를 불고 꺼줌
			theUnitFunc.UpperShuttle_VacuumOn(VAC_OFF,JIG_CH_1);
			Sleep(100);
			theUnitFunc.UpperShuttle_BlowOff(JIG_CH_1);
		}
		else
		{
			return;
		}
	}
}


void CFormMain::ClickGxstcCellmovePos6()
{	
	CGxMsgBox	dlgMsgBox;
	BOOL bResult = FALSE;
	bResult = DispCellTag(CELL_POS_LOWER_SHUTTLE_CH2);
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)//2017-03-02, skcho, 런상태에서는 무시
	{
		return;
	}
	if(bResult)
	{

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(TRUE, 
				_T("하위 셔틀 CH2 셀 정보를 삭제 하시겠습니까?"),	//한
				_T("Lower Shuttle Ch2 Cell Info Delete?."),		//영
				_T("Lower Shuttle Ch2  Cell Info Delete?") );		//베

		if(dlgMsgBox.DoModal() == IDOK)
		{
			//theCellBank.RemoveCellTag(CELL_POS_LOWER_SHUTTLE_CH2);
			theCellBank.RemoveCellTagCellInfo(CELL_POS_LOWER_SHUTTLE_CH2);
			theProductBank.UpdateMainInputCountMinus();
			
			//2017-11-20,YMG 잔압이 남아 있어 BLOW를 불고 꺼줌
			theUnitFunc.LowerShuttle_VacuumOn(VAC_OFF,JIG_CH_2);
			Sleep(100);
			theUnitFunc.LowerShuttle_BlowOff(JIG_CH_2);
		}
		else
		{
			return;
		}
	}
}


void CFormMain::ClickGxstcCellmovePos5()
{	
	CGxMsgBox	dlgMsgBox;
	BOOL bResult = FALSE;
	bResult = DispCellTag(CELL_POS_LOWER_SHUTTLE_CH1);
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)//2017-03-02, skcho, 런상태에서는 무시
	{
		return;
	}
	if(bResult)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(TRUE, 
				_T("하위 셔틀 CH1 셀 정보를 삭제 하시겠습니까?"),	//한
				_T("Lower Shuttle Ch1 Cell Info Delete?."),		//영
				_T("Lower Shuttle Ch1 Cell Info Delete?") );		//베

		if(dlgMsgBox.DoModal() == IDOK)
		{
			//theCellBank.RemoveCellTag(CELL_POS_LOWER_SHUTTLE_CH1);
			theCellBank.RemoveCellTagCellInfo(CELL_POS_LOWER_SHUTTLE_CH1);
			theProductBank.UpdateMainInputCountMinus();
			//2017-11-20,YMG 잔압이 남아 있어 BLOW를 불고 꺼줌
			theUnitFunc.LowerShuttle_VacuumOn(VAC_OFF,JIG_CH_1);
			Sleep(100);
			theUnitFunc.LowerShuttle_BlowOff(JIG_CH_1);
		}
		else
		{
			return;
		}
	}
}


void CFormMain::ClickGxstcCellmovePos8()
{	
	CGxMsgBox	dlgMsgBox;
	BOOL bResult = FALSE;
	bResult = DispCellTag(CELL_POS_PA_TR_CH2);
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)//2017-03-02, skcho, 런상태에서는 무시
	{
		return;
	}
	if(bResult)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(TRUE, 
			_T("PA TR CH2 정보를 삭제 하시겠습니까?"),	//한
			_T("PA TR CH2  Cell Info Delete?."),		//영
			_T("PA TR CH2  Cell Info Delete?") );		//베

		if(dlgMsgBox.DoModal() == IDOK)
		{
			//theCellBank.RemoveCellTag(CELL_POS_PA_TR_CH2);
			theCellBank.RemoveCellTagCellInfo(CELL_POS_PA_TR_CH2);
			theProductBank.UpdateMainInputCountMinus();
			//2017-11-20,YMG 잔압이 남아 있어 BLOW를 불고 꺼줌
			theUnitFunc.PATR_Vacuum_On(VAC_OFF,JIG_CH_2);
			Sleep(100);
			theUnitFunc.PATR_BlowOff(JIG_CH_2);
		}
		else
		{
			return;
		}
	}
}


void CFormMain::ClickGxstcCellmovePos7()
{	
	CGxMsgBox	dlgMsgBox;
	BOOL bResult = FALSE;
	bResult = DispCellTag(CELL_POS_PA_TR_CH1);
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)//2017-03-02, skcho, 런상태에서는 무시
	{
		return;
	}
	if(bResult)
	{

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(TRUE, 
			_T("PA TR CH1 정보를 삭제 하시겠습니까?"),	//한
			_T("PA TR CH1  Cell Info Delete?."),		//영
			_T("PA TR CH1  Cell Info Delete?") );		//베

		if(dlgMsgBox.DoModal() == IDOK)
		{
			//theCellBank.RemoveCellTag(CELL_POS_PA_TR_CH1);
			theCellBank.RemoveCellTagCellInfo(CELL_POS_PA_TR_CH1);
			theProductBank.UpdateMainInputCountMinus();
			//2017-11-20,YMG 잔압이 남아 있어 BLOW를 불고 꺼줌
			theUnitFunc.PATR_Vacuum_On(VAC_OFF,JIG_CH_1);
			Sleep(100);
			theUnitFunc.PATR_BlowOff(JIG_CH_1);
		}
		else
		{
			return;
		}
	}

}


void CFormMain::ClickGxstcCellmovePos10()
{	
	CGxMsgBox	dlgMsgBox;
	BOOL bResult = FALSE;
	bResult = DispCellTag(CELL_POS_PA_STAGE_CH2);
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)//2017-03-02, skcho, 런상태에서는 무시
	{
		return;
	}
	if(bResult)
	{

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(TRUE, 
			_T("PA STAGE CH2 정보를 삭제 하시겠습니까?"),	//한
			_T("PA STAGE CH2  Cell Info Delete?."),		//영
			_T("PA STAGE CH2  Cell Info Delete?") );		//베

		if(dlgMsgBox.DoModal() == IDOK)
		{
			//theCellBank.RemoveCellTag(CELL_POS_PA_STAGE_CH2);
			theCellBank.RemoveCellTagCellInfo(CELL_POS_PA_STAGE_CH2);
			theProductBank.UpdateMainInputCountMinus();
			//2017-11-20,YMG 잔압이 남아 있어 BLOW를 불고 꺼줌
			theUnitFunc.PAStageVac(VAC_OFF,JIG_CH_2);
			Sleep(100);
			theUnitFunc.PAStage_BlowOff(JIG_CH_2);
		}
		else
		{
			return;
		}
	}
}


void CFormMain::ClickGxstcCellmovePos9()
{	
	CGxMsgBox	dlgMsgBox;
	BOOL bResult = FALSE;
	bResult = DispCellTag(CELL_POS_PA_STAGE_CH1);
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)//2017-03-02, skcho, 런상태에서는 무시
	{
		return;
	}
	if(bResult)
	{

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(TRUE, 
			_T("PA STAGE CH1 정보를 삭제 하시겠습니까?"),	//한
			_T("PA STAGE CH1  Cell Info Delete?."),		//영
			_T("PA STAGE CH1  Cell Info Delete?") );		//베

		if(dlgMsgBox.DoModal() == IDOK)
		{
			//theCellBank.RemoveCellTag(CELL_POS_PA_STAGE_CH1);
			theCellBank.RemoveCellTagCellInfo(CELL_POS_PA_STAGE_CH1);
			theProductBank.UpdateMainInputCountMinus();
			//2017-11-20,YMG 잔압이 남아 있어 BLOW를 불고 꺼줌
			theUnitFunc.PAStageVac(VAC_OFF,JIG_CH_1);
			Sleep(100);
			theUnitFunc.PAStage_BlowOff(JIG_CH_1);
		}
		else
		{
			return;
		}
	}

}


void CFormMain::ClickGxstcCellmovePos12()
{	
	CGxMsgBox	dlgMsgBox;
	BOOL bResult = FALSE;
	bResult = DispCellTag(CELL_POS_LD_TR_CH2);
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)//2017-03-02, skcho, 런상태에서는 무시
	{
		return;
	}
	if(bResult)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(TRUE, 
			_T("LD TR CH2 정보를 삭제 하시겠습니까?"),	//한
			_T("LD TR CH2  Cell Info Delete?."),		//영
			_T("LD TR CH2  Cell Info Delete?") );		//베

		if(dlgMsgBox.DoModal() == IDOK)
		{
			//theCellBank.RemoveCellTag(CELL_POS_LD_TR_CH2);
			theCellBank.RemoveCellTagCellInfo(CELL_POS_LD_TR_CH2);
			theProductBank.UpdateMainInputCountMinus();
			//2017-11-20,YMG 잔압이 남아 있어 BLOW를 불고 꺼줌
			theUnitFunc.LDTRVacOn(VAC_OFF,JIG_CH_2);
			Sleep(100);
			theUnitFunc.LDTR_BlowOff(JIG_CH_2);
		}
		else
		{
			return;
		}
	}
}


void CFormMain::ClickGxstcCellmovePos11()
{	
	CGxMsgBox	dlgMsgBox;
	BOOL bResult = FALSE;
	bResult = DispCellTag(CELL_POS_LD_TR_CH1);
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)//2017-03-02, skcho, 런상태에서는 무시
	{
		return;
	}
	if(bResult)
	{

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(TRUE, 
			_T("LD TR CH1 정보를 삭제 하시겠습니까?"),	//한
			_T("LD TR CH1  Cell Info Delete?."),		//영
			_T("LD TR CH1  Cell Info Delete?") );		//베

		if(dlgMsgBox.DoModal() == IDOK)
		{
			//theCellBank.RemoveCellTag(CELL_POS_LD_TR_CH1);
			theCellBank.RemoveCellTagCellInfo(CELL_POS_LD_TR_CH1);
			theProductBank.UpdateMainInputCountMinus();
			//2017-11-20,YMG 잔압이 남아 있어 BLOW를 불고 꺼줌
			theUnitFunc.LDTRVacOn(VAC_OFF,JIG_CH_1);
			Sleep(100);
			theUnitFunc.LDTR_BlowOff(JIG_CH_1);
		}
		else
		{
			return;
		}
	}
}

void CFormMain::DisplayMessage(CELL_POS nPos)
{
	CString strMsg1,strMsg2,strMsg3,strCh;
	CGxMsgBox	dlgMsgBox;
	CCellInfo* pCellInfo;
	pCellInfo =  theCellBank.GetCellInfo((CELL_POS)nPos);

	if(nPos == CELL_POS_AZONE_CH1)
	{
		strCh.Format(_T("A ZONE CH1"));
	}
	else if(nPos == CELL_POS_AZONE_CH2)
	{
		strCh.Format(_T("A ZONE CH2"));
	}
	else if(nPos == CELL_POS_BZONE_CH1)
	{
		strCh.Format(_T("B ZONE CH1"));
	}
	else if(nPos == CELL_POS_BZONE_CH2)
	{
		strCh.Format(_T("B ZONE CH2"));
	}
	else if(nPos == CELL_POS_CZONE_CH1)
	{
		strCh.Format(_T("C ZONE CH1"));
	}
	else if(nPos == CELL_POS_CZONE_CH2)
	{
		strCh.Format(_T("C ZONE CH2"));
	}
	else if(nPos == CELL_POS_DZONE_CH1)
	{
		strCh.Format(_T("D ZONE CH1"));
	}
	else if(nPos == CELL_POS_DZONE_CH2)
	{
		strCh.Format(_T("D ZONE CH2"));
	}

	if(pCellInfo->bTrackOutChk)
	{
		strMsg1.Format(_T("%s Cell을 임의 추출 하시겠습니까?"),strCh);
		strMsg2.Format(_T("Abstract %s Cell?"),strCh);
		strMsg3.Format(_T("Abstract %s Cell?"),strCh);

	}
	else
	{
        strMsg1.Format(_T("%s Cell 정보를 지우시겠습니까?"),strCh);
		strMsg2.Format(_T("%s Cell Info Delete?"),strCh);
		strMsg3.Format(_T("%s Cell Info Delete?"),strCh);
	}

	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
	dlgMsgBox.SetMessage(TRUE, 
			strMsg1,	//한
			strMsg2,		//영
			strMsg3);		//베

	if(dlgMsgBox.DoModal() == IDOK)
	{			
		theUnitFunc.CIM_CellLossReport(nPos);				
		theCellBank.RemoveCellTagCellInfo(nPos);
		theProductBank.UpdateMainInputCountMinus();
	}
	else
	{
		return;
	}



}
void CFormMain::ClickGxstcCellmovePos14()
{
    BOOL bResult = FALSE;
	CGxMsgBox	dlgMsgBox;
	bResult = DispCellTag(CELL_POS_AZONE_CH2);

	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)//2017-03-02, skcho, 런상태에서는 무시
	{
		return;
	}
	
	if(bResult)
	{
		/*dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(TRUE, 
				_T("현재 A_ZONE CH2 CELL을 임의 추출 하시겠습니까?"),	//한
				_T("현재 A_ZONE CH2 CELL을 임의 추출 하시겠습니까?"),		//영
				_T("현재 A_ZONE CH2 CELL을 임의 추출 하시겠습니까?") );		//베

		if(dlgMsgBox.DoModal() == IDOK)
		{			
			theUnitFunc.CIM_CellLossReport(CELL_POS_AZONE_CH2);		
			//theCellBank.RemoveCellTag(CELL_POS_AZONE_CH2);
			theCellBank.RemoveCellTagCellInfo(CELL_POS_AZONE_CH2);
			theProductBank.UpdateMainInputCountMinus();
		}
		else
		{
			return;
		}
		*/
		DisplayMessage(CELL_POS_AZONE_CH2);
	}
}


void CFormMain::ClickGxstcCellmovePos13()
{
	BOOL bResult = FALSE;

	bResult = DispCellTag(CELL_POS_AZONE_CH1);
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)//2017-03-02, skcho, 런상태에서는 무시
	{
		return;
	}

	if(bResult)
	{
		/*CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(TRUE, 
				_T("현재 A_ZONE CH1 CELL을 임의 추출 하시겠습니까?"),	//한
				_T("현재 A_ZONE CH1 CELL을 임의 추출 하시겠습니까?"),		//영
				_T("현재 A_ZONE CH1 CELL을 임의 추출 하시겠습니까?") );		//베

		if(dlgMsgBox.DoModal() == IDOK)
		{
			theUnitFunc.CIM_CellLossReport(CELL_POS_AZONE_CH1);	
			//theCellBank.RemoveCellTag(CELL_POS_AZONE_CH1);	
			theCellBank.RemoveCellTagCellInfo(CELL_POS_AZONE_CH1);
			theProductBank.UpdateMainInputCountMinus();
		}
		else
		{
			return;
		}
		*/
		DisplayMessage(CELL_POS_AZONE_CH1);
	}
}


void CFormMain::ClickGxstcCellmovePos16()
{	
    BOOL bResult = FALSE;
	bResult = DispCellTag(CELL_POS_BZONE_CH2);
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)//2017-03-02, skcho, 런상태에서는 무시
	{
		return;
	}
	if(bResult)
	{
		/*CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(TRUE, 
				_T("현재 B_ZONE CH2 CELL을 임의 추출 하시겠습니까?"),	//한
				_T("현재 B_ZONE CH2 CELL을 임의 추출 하시겠습니까?"),		//영
				_T("현재 B_ZONE CH2 CELL을 임의 추출 하시겠습니까?") );		//베

		if(dlgMsgBox.DoModal() == IDOK)
		{
			theUnitFunc.CIM_CellLossReport(CELL_POS_BZONE_CH2);	
			//theCellBank.RemoveCellTag(CELL_POS_BZONE_CH2);	
			theCellBank.RemoveCellTagCellInfo(CELL_POS_BZONE_CH2);
			theProductBank.UpdateMainInputCountMinus();
		}
		else
		{
			return;
		}*/
		DisplayMessage(CELL_POS_BZONE_CH2);
	}
}


void CFormMain::ClickGxstcCellmovePos15()
{
	BOOL bResult = FALSE;
	bResult = DispCellTag(CELL_POS_BZONE_CH1);
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)//2017-03-02, skcho, 런상태에서는 무시
	{
		return;
	}
	if(bResult)
	{
		/*CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(TRUE, 
				_T("현재 B_ZONE CH1 CELL을 임의 추출 하시겠습니까?"),	//한
				_T("현재 B_ZONE CH1 CELL을 임의 추출 하시겠습니까?"),		//영
				_T("현재 B_ZONE CH1 CELL을 임의 추출 하시겠습니까?") );		//베

		if(dlgMsgBox.DoModal() == IDOK)
		{
			theUnitFunc.CIM_CellLossReport(CELL_POS_BZONE_CH1);	
			//theCellBank.RemoveCellTag(CELL_POS_BZONE_CH1);
			theCellBank.RemoveCellTagCellInfo(CELL_POS_BZONE_CH1);
			theProductBank.UpdateMainInputCountMinus();
		}
		else
		{
			return;
		}*/
		DisplayMessage(CELL_POS_BZONE_CH1);
	}
}


void CFormMain::ClickGxstcCellmovePos17()
{
	BOOL bResult = FALSE;
	bResult = DispCellTag(CELL_POS_CZONE_CH1);
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)//2017-03-02, skcho, 런상태에서는 무시
	{
		return;
	}
	if(bResult)
	{
		/*CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(TRUE, 
				_T("현재 C_ZONE CH1 CELL을 임의 추출 하시겠습니까?"),	//한
				_T("현재 C_ZONE CH1 CELL을 임의 추출 하시겠습니까?"),		//영
				_T("현재 C_ZONE CH1 CELL을 임의 추출 하시겠습니까?") );		//베

		if(dlgMsgBox.DoModal() == IDOK)
		{
			theUnitFunc.CIM_CellLossReport(CELL_POS_CZONE_CH1);	
			//theCellBank.RemoveCellTag(CELL_POS_CZONE_CH1);
			theCellBank.RemoveCellTagCellInfo(CELL_POS_CZONE_CH1);
			theProductBank.UpdateMainInputCountMinus();
		}
		else
		{
			return;
		}*/
		DisplayMessage(CELL_POS_CZONE_CH1);
	}
}


void CFormMain::ClickGxstcCellmovePos18()
{
	BOOL bResult = FALSE;
	bResult = DispCellTag(CELL_POS_CZONE_CH2);
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)//2017-03-02, skcho, 런상태에서는 무시
	{
		return;
	}
	if(bResult)
	{
		/*CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(TRUE, 
				_T("현재 C_ZONE CH2 CELL을 임의 추출 하시겠습니까?"),	//한
				_T("현재 C_ZONE CH2 CELL을 임의 추출 하시겠습니까?"),		//영
				_T("현재 C_ZONE CH2 CELL을 임의 추출 하시겠습니까?") );		//베

		if(dlgMsgBox.DoModal() == IDOK)
		{
			theUnitFunc.CIM_CellLossReport(CELL_POS_CZONE_CH2);	
			//theCellBank.RemoveCellTag(CELL_POS_CZONE_CH2);	
			theCellBank.RemoveCellTagCellInfo(CELL_POS_CZONE_CH2);
			theProductBank.UpdateMainInputCountMinus();
		}
		else
		{
			return;
		}*/
		DisplayMessage(CELL_POS_CZONE_CH2);
	}
}


void CFormMain::ClickGxstcCellmovePos20()
{
	BOOL bResult = FALSE;
	bResult = DispCellTag(CELL_POS_DZONE_CH2);
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)//2017-03-02, skcho, 런상태에서는 무시
	{
		return;
	}
	if(bResult)
	{
		/*CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(TRUE, 
				_T("현재 D_ZONE CH2 CELL을 임의 추출 하시겠습니까?"),	//한
				_T("현재 D_ZONE CH2 CELL을 임의 추출 하시겠습니까?"),		//영
				_T("현재 D_ZONE CH2 CELL을 임의 추출 하시겠습니까?") );		//베

		if(dlgMsgBox.DoModal() == IDOK)
		{
			theUnitFunc.CIM_CellLossReport(CELL_POS_DZONE_CH2);	
			//theCellBank.RemoveCellTag(CELL_POS_DZONE_CH2);	
			theCellBank.RemoveCellTagCellInfo(CELL_POS_DZONE_CH2);
			theProductBank.UpdateMainInputCountMinus();
		}
		else
		{
			return;
		}*/
		DisplayMessage(CELL_POS_DZONE_CH2);
	}
}


void CFormMain::ClickGxstcCellmovePos19()
{
	BOOL bResult = FALSE;
	bResult = DispCellTag(CELL_POS_DZONE_CH1);
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)//2017-03-02, skcho, 런상태에서는 무시
	{
		return;
	}
	if(bResult)
	{
		/*CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(TRUE, 
				_T("현재 D_ZONE CH1 CELL을 임의 추출 하시겠습니까?"),	//한
				_T("현재 D_ZONE CH1 CELL을 임의 추출 하시겠습니까?"),		//영
				_T("현재 D_ZONE CH1 CELL을 임의 추출 하시겠습니까?") );		//베

		if(dlgMsgBox.DoModal() == IDOK)
		{
			theUnitFunc.CIM_CellLossReport(CELL_POS_DZONE_CH1);	
			//theCellBank.RemoveCellTag(CELL_POS_DZONE_CH1);	
			theCellBank.RemoveCellTagCellInfo(CELL_POS_DZONE_CH1);
			theProductBank.UpdateMainInputCountMinus();
		}
		else
		{
			return;
		}*/
		DisplayMessage(CELL_POS_DZONE_CH1);
	}
}


void CFormMain::ClickGxstcCellmovePos22()
{
	BOOL bResult = FALSE;

	bResult = DispCellTag(CELL_POS_UD_TR_CH2);
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)//2017-03-02, skcho, 런상태에서는 무시
	{
		return;
	}

	if(bResult)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(TRUE, 
			_T("UD TR CH2 정보를 삭제 하시겠습니까?"),	//한
			_T("UD TR CH2  Cell Info Delete?"),		//영
			_T("UD TR CH2  Cell Info Delete?") );		//베

		if(dlgMsgBox.DoModal() == IDOK)
		{
			//2017-05-13,skcho, track_out한 셀에 대해서는 임의추출보고 안함
			//--theUnitFunc.CIM_CellLossReport(CELL_POS_UD_TR_CH2);	
			//theCellBank.RemoveCellTag(CELL_POS_UD_TR_CH2);	
			theCellBank.RemoveCellTagCellInfo(CELL_POS_UD_TR_CH2);
			theProductBank.UpdateMainInputCountMinus();
		}
		else
		{
			return;
		}
	}
}


void CFormMain::ClickGxstcCellmovePos21()
{
	BOOL bResult = FALSE;

	bResult = DispCellTag(CELL_POS_UD_TR_CH1);
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)//2017-03-02, skcho, 런상태에서는 무시
	{
		return;
	}

	if(bResult)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(TRUE, 
			_T("UD TR CH1 정보를 삭제 하시겠습니까?"),	//한
			_T("UD TR CH1  Cell Info Delete?"),		//영
			_T("UD TR CH1  Cell Info Delete?") );		//베

		if(dlgMsgBox.DoModal() == IDOK)
		{
			//2017-05-13,skcho, track_out한 셀에 대해서는 임의추출보고 안함
			//--theUnitFunc.CIM_CellLossReport(CELL_POS_UD_TR_CH1);	
			//theCellBank.RemoveCellTag(CELL_POS_UD_TR_CH1);
			theCellBank.RemoveCellTagCellInfo(CELL_POS_UD_TR_CH1);
			theProductBank.UpdateMainInputCountMinus();
		}
		else
		{
			return;
		}
	}
}


void CFormMain::ClickGxstcCellmovePos24()
{
	BOOL bResult = FALSE;

	bResult = DispCellTag(CELL_POS_UD_STAGE_CH2);
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)//2017-03-02, skcho, 런상태에서는 무시
	{
		return;
	}

	if(bResult)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(TRUE, 
			_T("UD STAGE CH2 정보를 삭제 하시겠습니까?"),	//한
			_T("UD STAGE CH2  Cell Info Delete?"),		//영
			_T("UD STAGE CH2  Cell Info Delete?") );		//베

		if(dlgMsgBox.DoModal() == IDOK)
		{
			//2017-05-13,skcho, track_out한 셀에 대해서는 임의추출보고 안함
			//--theUnitFunc.CIM_CellLossReport(CELL_POS_UD_STAGE_CH2);	
			//theCellBank.RemoveCellTag(CELL_POS_UD_STAGE_CH2);	
			theCellBank.RemoveCellTagCellInfo(CELL_POS_UD_STAGE_CH2);
			theProductBank.UpdateMainInputCountMinus();
		}
		else
		{
			return;
		}
	}
}


void CFormMain::ClickGxstcCellmovePos23()
{
	BOOL bResult = FALSE;

	bResult = DispCellTag(CELL_POS_UD_STAGE_CH1);
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)//2017-03-02, skcho, 런상태에서는 무시
	{
		return;
	}

	if(bResult)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(TRUE, 
			_T("UD STAGE CH1 정보를 삭제 하시겠습니까?"),	//한
			_T("UD STAGE CH1  Cell Info Delete?"),		//영
			_T("UD STAGE CH1  Cell Info Delete?") );		//베

		if(dlgMsgBox.DoModal() == IDOK)
		{
			//2017-05-13,skcho, track_out한 셀에 대해서는 임의추출보고 안함
			//--theUnitFunc.CIM_CellLossReport(CELL_POS_UD_STAGE_CH1);	
			//theCellBank.RemoveCellTag(CELL_POS_UD_STAGE_CH1);	
			theCellBank.RemoveCellTagCellInfo(CELL_POS_UD_STAGE_CH1);
			theProductBank.UpdateMainInputCountMinus();
		}
		else
		{
			return;
		}
	}
}


void CFormMain::ClickGxstcCellmovePos25()
{
	BOOL bResult = FALSE;

	bResult = DispCellTag(CELL_POS_UD_ROBOT);
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)//2017-03-02, skcho, 런상태에서는 무시
	{
		return;
	}

	if(bResult)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(TRUE, 
			_T("UD ROBOT 정보를 삭제 하시겠습니까?"),	//한
			_T("UD ROBOT Cell Info Delete?"),		//영
			_T("UD ROBOT Cell Info Delete?") );		//베

		if(dlgMsgBox.DoModal() == IDOK)
		{
			//2017-05-13,skcho, track_out한 셀에 대해서는 임의추출보고 안함
			//--theUnitFunc.CIM_CellLossReport(CELL_POS_UD_ROBOT);	
			//theCellBank.RemoveCellTag(CELL_POS_UD_ROBOT);
			theCellBank.RemoveCellTagCellInfo(CELL_POS_UD_ROBOT);
			theProductBank.UpdateMainInputCountMinus();
		}
		else
		{
			return;
		}
	}
}


void CFormMain::ClickGxbtnCountClear()
{
	
	CTime time = CTime::GetCurrentTime();
	theProductBank.m_timeMainCount = time;
	theProductBank.m_MainCountINPUT.iValue = 0;
	theProductBank.m_MainCountGOOD.iValue = 0;
	theProductBank.m_MainCountREJECT.iValue = 0;
	theProductBank.m_MainCountMCR.iValue = 0;
	theProductBank.m_MainCountCONTACTALIGN.iValue = 0;
	theProductBank.m_MainCountVISIONALIGN.iValue = 0;

	//20170206 byskcho
	theProductBank.m_MainCountEDGEALIGN.iValue = 0;
	theProductBank.m_MainCountCONTACT.iValue = 0;	

	::PostMessage(m_hWnd, UDMSG_COUNT_UPDATE, NULL, NULL);

	//2018-05-25,MGYUN, TMD 테스트 - 송영현 대리
	//CCellInfo *pCell =NULL;
	//theCellBank.CreateCellInfo(CELL_POS_AZONE_CH1);
	//pCell = theCellBank.GetCellInfo(CELL_POS_AZONE_CH1);
	//pCell->m_strCellID.Format(_T("%s"),_T("A3NE1R7BFFBBT120"));
	//pCell->m_strInnerID.Format(_T("%s"),_T("123456"));
	//pCell->m_JigId = JIG_ID_A;
	//pCell->m_JigCh = JIG_CH_1;
	//theProcBank.m_nVI_MITAlignResult = ALIGN_RESULT_GOOD;
	//pCell->m_Zone_B.m_nVI_AlignResult = ALIGN_RESULT_GOOD;
	//pCell->m_Zone_C.VIClass = GOOD_CELL;
	//pCell->m_Zone_C.CZoneDefect = _T("GOOD");
	//pCell->m_Zone_A.m_strTMD_Info = _T("171214_AMB646NH01_E1_Mainbuild_AMT_V01.TMD");
	//theUnitFunc.A_Zone_TMDInfo(JIG_CH_1);
	//theUnitFunc.CIM_CellTrackIn(CELL_POS_AZONE_CH1);
	///////////////////////////////////////////////////////////////////

	//2018-03-16, Test
	//theProcBank.m_TrayBank.GetTrayInfo(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_ALIGN)->SetCellFull();

    //theUnitFunc.CellData_LDConvIn();
	/*theCellBank.RemoveCellTagCellInfo(CELL_POS_AZONE_CH1);
	theCellBank.RemoveCellTagCellInfo(CELL_POS_AZONE_CH2);
	theCellBank.RemoveCellTagCellInfo(CELL_POS_BZONE_CH1);
	theCellBank.RemoveCellTagCellInfo(CELL_POS_BZONE_CH2);
	theCellBank.RemoveCellTagCellInfo(CELL_POS_CZONE_CH1);
	theCellBank.RemoveCellTagCellInfo(CELL_POS_CZONE_CH2);
	theCellBank.RemoveCellTagCellInfo(CELL_POS_DZONE_CH1);
	theCellBank.RemoveCellTagCellInfo(CELL_POS_DZONE_CH2);
	

	theDeviceMelsec.UpperWordSend(OPTICAL_JIG_A_CH1_CELLID,_T(""),16,FALSE);
	theDeviceMelsec.UpperWordSend(OPTICAL_JIG_A_CH2_CELLID,_T(""),16,FALSE);
	theDeviceMelsec.UpperWordSend(OPTICAL_JIG_B_CH1_CELLID,_T(""),16,FALSE);
	theDeviceMelsec.UpperWordSend(OPTICAL_JIG_B_CH2_CELLID,_T(""),16,FALSE);
	theDeviceMelsec.UpperWordSend(OPTICAL_JIG_C_CH1_CELLID,_T(""),16,FALSE);
	theDeviceMelsec.UpperWordSend(OPTICAL_JIG_C_CH2_CELLID,_T(""),16,FALSE);
	theDeviceMelsec.UpperWordSend(OPTICAL_JIG_D_CH1_CELLID,_T(""),16,FALSE);
	theDeviceMelsec.UpperWordSend(OPTICAL_JIG_D_CH2_CELLID,_T(""),16,FALSE);
	theDeviceMelsec.UpperWordSend(OPTICAL_ULDTR_CH1_CELLID,_T(""),16,FALSE);
	theDeviceMelsec.UpperWordSend(OPTICAL_ULDTR_CH2_CELLID,_T(""),16,FALSE);
	theDeviceMelsec.UpperWordSend(OPTICAL_ULD_ROBOT_CELLID,_T(""),16,FALSE);
	*/

}


LRESULT CFormMain::OnCountUpdate(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	CGxUICtrl::SetStaticInt(this, theProductBank.m_MainCountINPUT.uID, theProductBank.m_MainCountINPUT.iValue);
	CGxUICtrl::SetStaticInt(this, theProductBank.m_MainCountGOOD.uID, theProductBank.m_MainCountGOOD.iValue);
	//--CGxUICtrl::SetStaticInt(this, theProductBank.m_MainCountREJECT.uID, theProductBank.m_MainCountREJECT.iValue);
	
	//2018-03-12,MGYUN, 실시간 생산량 MELSEC 기록
	//theMonitorBank.WriteProductInfo(theProductBank.m_MainCountINPUT.uID);

	int nTotal = theProductBank.m_MainCountGOOD.iValue + theProductBank.m_MainCountREJECT.iValue;


	//2017-07-14,SKCHO, REJECT에 퍼센트율 추가
	double dRejectRate = 0;
	if(nTotal > 0)
	{
		dRejectRate = ((double)theProductBank.m_MainCountREJECT.iValue / (double)nTotal) * 100.;
	}
	CString strReject;
	strReject.Format(_T("%d (%.1f%%)"), theProductBank.m_MainCountREJECT.iValue, dRejectRate);
	CGxUICtrl::SetStaticString(this, theProductBank.m_MainCountREJECT.uID, strReject);

	double dMCRRate = 0;
	if(nTotal > 0)
	{
		dMCRRate = ((double)theProductBank.m_MainCountMCR.iValue / (double)nTotal) * 100.;
	}

	CString strMCR;
	strMCR.Format(_T("%d (%.1f%%)"), theProductBank.m_MainCountMCR.iValue, dMCRRate);
	CGxUICtrl::SetStaticString(this, theProductBank.m_MainCountMCR.uID, strMCR);

	double dCONTACTALIGNRate = 0;
	if(nTotal > 0)
	{
		dCONTACTALIGNRate = ((double)theProductBank.m_MainCountCONTACTALIGN.iValue / (double)nTotal) * 100.;
	}

	CString strCONTACTALIGN;
	strCONTACTALIGN.Format(_T("%d (%.1f%%)"), theProductBank.m_MainCountCONTACTALIGN.iValue, dCONTACTALIGNRate);
	CGxUICtrl::SetStaticString(this, theProductBank.m_MainCountCONTACTALIGN.uID, strCONTACTALIGN);


	double dEDGEALIGNRate = 0;
	if(nTotal > 0)
	{
		dEDGEALIGNRate = ((double)theProductBank.m_MainCountEDGEALIGN.iValue / (double)nTotal) * 100.;
	}

	CString strEDGEALIGN;
	strEDGEALIGN.Format(_T("%d (%.1f%%)"), theProductBank.m_MainCountEDGEALIGN.iValue, dEDGEALIGNRate);
	CGxUICtrl::SetStaticString(this, theProductBank.m_MainCountEDGEALIGN.uID, strEDGEALIGN);

	double dCONTACTRate = 0;
	if(nTotal > 0)
	{
		dCONTACTRate = ((double)theProductBank.m_MainCountCONTACT.iValue / (double)nTotal) * 100.;
	}

	CString strCONTACT;
	strCONTACT.Format(_T("%d (%.1f%%)"), theProductBank.m_MainCountCONTACT.iValue, dCONTACTRate);
	CGxUICtrl::SetStaticString(this, theProductBank.m_MainCountCONTACT.uID, strCONTACT);
	
	//2018-03-12,MGYUN, 컨택 실시간 65C,5WORD
	//theMonitorBank.WriteContactInfo(dCONTACTRate);

	CTime time = theProductBank.m_timeMainCount;
	CString strTime;
	strTime.Format(_T("%02d-%02d %02d:%02d:%02d"), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	CGxUICtrl::SetStaticString(this, IDC_GXLBL_CLEAR_TIME, strTime);

	double dVisionAlignRate = 0;
	if(nTotal > 0)
	{
		dVisionAlignRate = ((double)theProductBank.m_MainCountVISIONALIGN.iValue / (double)nTotal) * 100.;
	}

	CString strVisionAlign;
	strVisionAlign.Format(_T("%d (%.1f%%)"), theProductBank.m_MainCountVISIONALIGN.iValue, dVisionAlignRate);
	CGxUICtrl::SetStaticString(this, theProductBank.m_MainCountVISIONALIGN.uID, strVisionAlign);

	
	// 2018-04-06,NAM, Will update Contact Percent for Day, Week, Month when m_Report=True
	m_Report=TRUE;

	/*//2018-03-15, NAM, Contact PerCent for Day,  Week, Month.
	UpdatePrehourContact();
	UPdateWeekContact();
	UPdateMonthContact();*/
	return 0;
}

//TACT TIME 실시간 기록 함수.
LRESULT CFormMain::OnTactChange(WPARAM wParam, LPARAM lParam)
{
	
	switch(lParam)
	{
	case LOADER_TACT:
		theProcBank.m_strLoadTack.Format(_T("%3.2f"),(double)wParam/1000.);
		CGxUICtrl::SetStaticString(this, IDC_GXLBL_LOAD_TACT, theProcBank.m_strLoadTack);
		//2018-03-12,MGYUN, TACT TIME WRITE MELSEC
		theMonitorBank.WriteTactTime(LOADER_TACT,theProcBank.m_strLoadTack);
		break;
	case UNLOADER_TACT:
		theProcBank.m_strUnLoadTack.Format(_T("%3.2f"),(double)wParam/1000.);
		CGxUICtrl::SetStaticString(this, IDC_GXLBL_UNLOAD_TACT, theProcBank.m_strUnLoadTack);
		//2018-03-12,MGYUN, TACT TIME WRITE MELSEC
		theMonitorBank.WriteTactTime(UNLOADER_TACT,theProcBank.m_strUnLoadTack);
		break;
	case AZONE_TACT:
		theProcBank.m_strAZoneTack.Format(_T("%3.2f"),(double)wParam/2000.);
		CGxUICtrl::SetStaticString(this, IDC_GXLBL_AZONE_TACT, theProcBank.m_strAZoneTack);
		//2018-03-12,MGYUN, TACT TIME WRITE MELSEC
		theMonitorBank.WriteTactTime(AZONE_TACT,theProcBank.m_strAZoneTack);
		break;
	case BZONE_TACT:
		theProcBank.m_strBZoneTack.Format(_T("%3.2f"),(double)wParam/2000.);
		CGxUICtrl::SetStaticString(this, IDC_GXLBL_BZONE_TACT, theProcBank.m_strBZoneTack);
		//2018-03-12,MGYUN, TACT TIME WRITE MELSEC
		theMonitorBank.WriteTactTime(BZONE_TACT,theProcBank.m_strBZoneTack);
		break;
	case CZONE_TACT:
		theProcBank.m_strCZoneTack.Format(_T("%3.2f"),(double)wParam/2000.);
		CGxUICtrl::SetStaticString(this, IDC_GXLBL_CZONE_TACT, theProcBank.m_strCZoneTack);
		//2018-03-12,MGYUN, TACT TIME WRITE MELSEC
		theMonitorBank.WriteTactTime(CZONE_TACT,theProcBank.m_strCZoneTack);
		break;
	case DZONE_TACT:
		theProcBank.m_strDZoneTack.Format(_T("%3.2f"),(double)wParam/2000.);
		CGxUICtrl::SetStaticString(this, IDC_GXLBL_DZONE_TACT, theProcBank.m_strDZoneTack);
		//2018-03-12,MGYUN, TACT TIME WRITE MELSEC
		theMonitorBank.WriteTactTime(DZONE_TACT,theProcBank.m_strDZoneTack);
		break;
	}

	return 0;
}


BOOL CFormMain::DispCellTag(int nPosNode)
{
	BOOL bResult = FALSE;
	CCellTag tagCell;
	CCellInfo* pCellInfo;
	pCellInfo =  theCellBank.GetCellInfo((CELL_POS)nPosNode);

	CString str;
	if( pCellInfo == NULL )
	{
		str.Format(_T("[%d] No Cell Here...!\n"), nPosNode);
		bResult = FALSE;
	}
	else
	{
		str.Format(_T("[%d] Jig_Ch(%d, %d), InnerID(%s), CellID(%s), Edge Align(%d), Contact Align(%d), ContactClass(%s), VI Align(%d), CZoneClass(%d), CZoneDefect(%s)\n"), nPosNode,
			pCellInfo->m_JigId, pCellInfo->m_JigCh, pCellInfo->m_strInnerID, pCellInfo->m_strCellID, pCellInfo->LoaderAlignResult, pCellInfo->Pre_AlignResultXY,
			pCellInfo->m_strContactResult,pCellInfo->m_Zone_B.m_nVI_AlignResult,pCellInfo->m_Zone_C.CZoneClass, pCellInfo->m_Zone_C.CZoneDefect);
		//2017-05-15,skcho,Track_in에 대해서만 임의추출 메세지
		//if(pCellInfo->bTrackInChk)
		///{
			bResult = TRUE;
		//}
		//else
		//{
		//	bResult = FALSE;
		//}
	}

	LibShowOut(str);

	return bResult;
}

void CFormMain::DispCellID(CELL_POS nPosNode)
{
	
	CRect Rect;
	UINT nColor = GXCOLOR_WHITE;
	UINT nID;
	CCellTag tagCell;
	CCellInfo* pCellInfo;
	pCellInfo =  theCellBank.GetCellInfo((CELL_POS)nPosNode);
	tagCell = theCellBank.GetCellTag(nPosNode);

	CString str = _T("");
	
	if(nPosNode == CELL_POS_LD_TR_CH1)
	{
		nID = IDC_GXLBL_LD_CELLID1;
	}
	else if(nPosNode ==	CELL_POS_LD_TR_CH2)
	{
		nID = IDC_GXLBL_LD_CELLID2;
	}
	else if(nPosNode ==CELL_POS_AZONE_CH1)
	{
		nID = IDC_GXLBL_A_CELLID1;
	}
	else if(nPosNode ==CELL_POS_AZONE_CH2)
	{
		nID = IDC_GXLBL_A_CELLID2;
	}
	else if(nPosNode ==CELL_POS_BZONE_CH1)
	{
		nID = IDC_GXLBL_B_CELLID1;
	}
	else if(nPosNode ==CELL_POS_BZONE_CH2)
	{
		nID = IDC_GXLBL_B_CELLID2;
	}
	else if(nPosNode ==CELL_POS_CZONE_CH1)
	{
		nID = IDC_GXLBL_C_CELLID1;
	}
	else if(nPosNode ==CELL_POS_CZONE_CH2)
	{
		nID = IDC_GXLBL_C_CELLID2;
	}
	else if(nPosNode ==CELL_POS_DZONE_CH1)
	{
		nID = IDC_GXLBL_D_CELLID1;
	}
	else if(nPosNode ==CELL_POS_DZONE_CH2)
	{
		nID = IDC_GXLBL_D_CELLID2;
	}
	else if(nPosNode ==CELL_POS_UD_TR_CH1)
	{
		nID = IDC_GXLBL_ULDTR_CELLID1;
	}
	else if(nPosNode ==CELL_POS_UD_TR_CH2)
	{
		nID = IDC_GXLBL_ULDTR_CELLID2;
	}
	else if(nPosNode ==CELL_POS_UD_STAGE_CH1)
	{
		nID = IDC_GXLBL_ULDSTAGE_CELLID1;
	}
	else if(nPosNode ==CELL_POS_UD_STAGE_CH2)
	{
		nID = IDC_GXLBL_ULDSTAGE_CELLID2;
	}
	else if(nPosNode ==CELL_POS_UD_ROBOT)
	{
		nID = IDC_GXLBL_ULDROBOT_CELLID;
	}	
	//2018-05-16,MGYUN,추가
	else if(nPosNode == CELL_POS_UD_CONV)
	{
		nID = IDC_GXLBL_ULDCONV_CELLID;
	}
	if( pCellInfo != NULL )	
	{
		str.Format(_T("%d:%s"),tagCell.m_nIndex,pCellInfo->m_strCellID);
		nColor = GXCOLOR_LIGHT_GREEN;
			
	}	
	GetDlgItem(nID)->GetWindowRect(&Rect);
	ScreenToClient(&Rect);
	InvalidateRect(Rect);
	CGxUICtrl::SetStaticColor(this,nID, nColor);
	CGxUICtrl::SetStaticString(this,nID,str);
	
}

void CFormMain::LibShowOut2(CString str)
{
	static int s_nLogLines_1 = 0;

	CString strOut;

	CString strTime;
	/*SYSTEMTIME	time;
	GetLocalTime(&time);
	strTime.Format(_T("[%04u-%02u-%02u  %02u:%02u:%02u:%03u]  "),
		time.wYear, time.wMonth, time.wDay, time.wHour, 
		time.wMinute, time.wSecond, time.wMilliseconds);*/

	strOut = /*strTime + */str + _T("\r\n");


	if( s_nLogLines_1 >= 2 )
	{
		m_edtBoard2.SetSel(0, -1);
		m_edtBoard2.ReplaceSel(strOut);
		m_edtBoard2.ScrollWindow(0, 2);

		s_nLogLines_1 = 0;
	}
	else
	{
		m_edtBoard2.SetSel(-1, -1);
		m_edtBoard2.ReplaceSel(strOut);
		m_edtBoard2.ScrollWindow(0, 2);
	}

	s_nLogLines_1++;
}

void CFormMain::LibShowOut(CString str)
{
	static int s_nLogLines = 0;

	CString strOut;

	CString strTime;
	SYSTEMTIME	time;
	GetLocalTime(&time);
	strTime.Format(_T("[%04u-%02u-%02u  %02u:%02u:%02u:%03u]  "),
		time.wYear, time.wMonth, time.wDay, time.wHour, 
		time.wMinute, time.wSecond, time.wMilliseconds);

	strOut = strTime + str + _T("\r\n");


	if( s_nLogLines >= 1000 )
	{
		m_edtBoard.SetSel(0, -1);
		m_edtBoard.ReplaceSel(strOut);
		m_edtBoard.ScrollWindow(0, 1000);

		s_nLogLines = 0;
	}
	else
	{
		m_edtBoard.SetSel(-1, -1);
		m_edtBoard.ReplaceSel(strOut);
		m_edtBoard.ScrollWindow(0, 1000);
	}

	s_nLogLines++;
}


/*void CFormMain::ClickGxbtnCountClear2()
{

	m_edtBoard.SetSel(0, -1);
	m_edtBoard.ReplaceSel(_T(""));
	m_edtBoard.ScrollWindow(0, 1000);

	m_edtBoard2.SetSel(0, -1);
	m_edtBoard2.ReplaceSel(_T(""));
}
*/


void CFormMain::ClickGxbtnModePm2()
{
	if(theConfigBank.m_Option.m_bUseDryRun)
	{
		SetTimer(DRYRUN_TIMER,5000,NULL);
	}
}



void CFormMain::DoorStatusDisplay() 
{
	UINT uCLOSE = RGB(0,255,0);
	UINT uOPEN = RGB(255,0,0);

#if(AMT_TYPE == 1)
	if(theUnitFunc.GetInPutIOState(X_LD_DOOR_SEN_1))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_LOADER_DOOR1, uOPEN);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_LOADER_DOOR1, uCLOSE);
	}


	if(theUnitFunc.GetInPutIOState(X_LD_DOOR_SEN_2))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_LOADER_DOOR2, uOPEN);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_LOADER_DOOR2, uCLOSE);
	}

	
	if(theUnitFunc.GetInPutIOState(X_LD_DOOR_SEN_3))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_LOADER_DOOR3, uOPEN);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_LOADER_DOOR3, uCLOSE);
	}

	if(theUnitFunc.GetInPutIOState(X_LD_DOOR_SEN_4))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_LOADER_DOOR4, uOPEN);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_LOADER_DOOR4, uCLOSE);
	}

	if(theUnitFunc.GetInPutIOState(X_LD_DOOR_SEN_5))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_LOADER_DOOR5, uOPEN);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_LOADER_DOOR5, uCLOSE);
	}

	if(theUnitFunc.GetInPutIOState(X_LD_DOOR_SEN_6))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_LOADER_DOOR6, uOPEN);
	}
	else
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_LOADER_DOOR6, uCLOSE);
	}

	if(theUnitFunc.GetInPutIOState(X_ULD_DOOR_SEN_1))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UNLOADER_DOOR1, uOPEN);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UNLOADER_DOOR1, uCLOSE);
	}

	if(theUnitFunc.GetInPutIOState(X_ULD_DOOR_SEN_2))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UNLOADER_DOOR2, uOPEN);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UNLOADER_DOOR2, uCLOSE);
	}

	if(theUnitFunc.GetInPutIOState(X_ULD_DOOR_SEN_3))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UNLOADER_DOOR3, uOPEN);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UNLOADER_DOOR3, uCLOSE);
	}

	if(theUnitFunc.GetInPutIOState(X_ULD_DOOR_SEN_4))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UNLOADER_DOOR4, uOPEN);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UNLOADER_DOOR4, uCLOSE);
	}

	if(theUnitFunc.GetInPutIOState(X_ULD_DOOR_SEN_5))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UNLOADER_DOOR5, uOPEN);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UNLOADER_DOOR5, uCLOSE);
	}

	if(theUnitFunc.GetInPutIOState(X_ULD_DOOR_SEN_6))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UNLOADER_DOOR6, uOPEN);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UNLOADER_DOOR6, uCLOSE);
	}
#else
	//if(theUnitFunc.GetInPutIOCheck(X_LD_DOOR_SEN_1))
	if(theUnitFunc.GetInPutIOState(X_LD_DOOR_SEN_1))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_LOADER_DOOR3, uOPEN);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_LOADER_DOOR3, uCLOSE);
	}

	if(theUnitFunc.GetInPutIOState(X_LD_DOOR_SEN_2))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_LOADER_DOOR2, uOPEN);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_LOADER_DOOR2, uCLOSE);
	}
	
	if(theUnitFunc.GetInPutIOState(X_LD_DOOR_SEN_3))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_LOADER_DOOR1, uOPEN);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_LOADER_DOOR1, uCLOSE);
	}

	if(theUnitFunc.GetInPutIOState(X_LD_DOOR_SEN_4))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_LOADER_DOOR6, uOPEN);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_LOADER_DOOR6, uCLOSE);
	}

	if(theUnitFunc.GetInPutIOState(X_LD_DOOR_SEN_5))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_LOADER_DOOR5, uOPEN);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_LOADER_DOOR5, uCLOSE);
	}

	if(theUnitFunc.GetInPutIOState(X_LD_DOOR_SEN_6))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_LOADER_DOOR4, uOPEN);
	}
	else
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_LOADER_DOOR4, uCLOSE);
	}

	if(theUnitFunc.GetInPutIOState(X_ULD_DOOR_SEN_1))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UNLOADER_DOOR6, uOPEN);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UNLOADER_DOOR6, uCLOSE);
	}

	if(theUnitFunc.GetInPutIOState(X_ULD_DOOR_SEN_2))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UNLOADER_DOOR1, uOPEN);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UNLOADER_DOOR1, uCLOSE);
	}

	if(theUnitFunc.GetInPutIOState(X_ULD_DOOR_SEN_3))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UNLOADER_DOOR2, uOPEN);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UNLOADER_DOOR2, uCLOSE);
	}

	if(theUnitFunc.GetInPutIOState(X_ULD_DOOR_SEN_4))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UNLOADER_DOOR3, uOPEN);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UNLOADER_DOOR3, uCLOSE);
	}

	if(theUnitFunc.GetInPutIOState(X_ULD_DOOR_SEN_5))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UNLOADER_DOOR4, uOPEN);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UNLOADER_DOOR4, uCLOSE);
	}

	if(theUnitFunc.GetInPutIOState(X_ULD_DOOR_SEN_6))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UNLOADER_DOOR5, uOPEN);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UNLOADER_DOOR5, uCLOSE);
	}
#endif
	if(theUnitFunc.GetInPutIOState(X_GOOD_IN_TRAY_LC_CHK))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_GOOD_IN_TRAY_CUTTON, uOPEN);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_GOOD_IN_TRAY_CUTTON, uCLOSE);
	}
	if(theUnitFunc.GetInPutIOState(X_GOOD_OUT_TRAY_LC_CHK))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_GOOD_OUT_TRAY_CUTTON, uOPEN);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_GOOD_OUT_TRAY_CUTTON, uCLOSE);
	}
	if(theUnitFunc.GetInPutIOState(X_NG_TRAY_LC_CHK))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_NG_TRAY_CUTTON, uOPEN);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_NG_TRAY_CUTTON, uCLOSE);
	}	

	//AGV PIO OK UNLOAD REQ X. JSJUNG. 20170307
	uOPEN = RGB(224,224,224);
	if(!theUnitFunc.GetInPutIOState(X_GOOD_ULD_CONV_TRAY_PIO_REQ))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_AGV_OK_UNLOAD_REQ_X, uOPEN);
	}
	else
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_AGV_OK_UNLOAD_REQ_X, uCLOSE);
	}

	//AGV PIO OK UNLOAD CON X. JSJUNG. 20170307
	if(!theUnitFunc.GetInPutIOState(X_GOOD_ULD_CONV_TRAY_PIO_CONNECTION))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_AGV_OK_UNLOAD_CON_X, uOPEN);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_AGV_OK_UNLOAD_CON_X, uCLOSE);
	}

	//AGV PIO OK UNLOAD RUN X. JSJUNG. 20170307
	if(!theUnitFunc.GetInPutIOState(X_GOOD_ULD_CONV_TRAY_PIO_BUSY))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_AGV_OK_UNLOAD_RUN_X, uOPEN);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_AGV_OK_UNLOAD_RUN_X, uCLOSE);
	}

	//AGV PIO OK LOAD REQ X. JSJUNG. 20170307
	if(!theUnitFunc.GetInPutIOState(X_GOOD_LD_CONV_TRAY_PIO_REQ))  //
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_AGV_OK_LOAD_REQ_X, uOPEN);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_AGV_OK_LOAD_REQ_X, uCLOSE);
	}

	//AGV PIO OK LOAD CON X. JSJUNG. 20170307
	if(!theUnitFunc.GetInPutIOState(X_GOOD_LD_CONV_TRAY_PIO_CONNECTION)) //
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_AGV_OK_LOAD_CON_X, uOPEN);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_AGV_OK_LOAD_CON_X, uCLOSE);
	}

	//AGV PIO OK LOAD RUN X. JSJUNG. 20170307
	if(!theUnitFunc.GetInPutIOState(X_GOOD_LD_CONV_TRAY_PIO_BUSY))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_AGV_OK_LOAD_RUN_X, uOPEN);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_AGV_OK_LOAD_RUN_X, uCLOSE);
	}

	//AGV PIO OK LOAD REQ Y. JSJUNG. 20170307
	if(!theUnitFunc.GetOutPutIOState(Y_GOOD_LD_CONV_TRAY_PIO_REQ))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_AGV_OK_LOAD_REQ_Y, uOPEN);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_AGV_OK_LOAD_REQ_Y, uCLOSE);
	}

	//AGV PIO OK LOAD CON Y. JSJUNG. 20170307
	if(!theUnitFunc.GetOutPutIOState(Y_GOOD_LD_CONV_TRAY_PIO_CONNECTION))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_AGV_OK_LOAD_CON_Y, uOPEN);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_AGV_OK_LOAD_CON_Y, uCLOSE);
	}

	//AGV PIO OK LOAD RUN Y. JSJUNG. 20170307
	if(!theUnitFunc.GetOutPutIOState(Y_GOOD_LD_CONV_TRAY_PIO_START))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_AGV_OK_LOAD_RUN_Y, uOPEN);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_AGV_OK_LOAD_RUN_Y, uCLOSE);
	}

	//AGV PIO OK UNLOAD REQ Y. JSJUNG. 20170307
	if(!theUnitFunc.GetOutPutIOState(Y_GOOD_ULD_CONV_TRAY_PIO_REQ))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_AGV_OK_UNLOAD_REQ_Y, uOPEN);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_AGV_OK_UNLOAD_REQ_Y, uCLOSE);
	}

	//AGV PIO OK UNLOAD CON Y. JSJUNG. 20170307
	if(!theUnitFunc.GetOutPutIOState(Y_GOOD_ULD_CONV_TRAY_PIO_CONNECTION))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_AGV_OK_UNLOAD_CON_Y, uOPEN);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_AGV_OK_UNLOAD_CON_Y, uCLOSE);
	}

	//AGV PIO OK UNLOAD RUN Y. JSJUNG. 20170307
	if(!theUnitFunc.GetOutPutIOState(Y_GOOD_ULD_CONV_TRAY_PIO_START))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_AGV_OK_UNLOAD_RUN_Y, uOPEN);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_AGV_OK_UNLOAD_RUN_Y, uCLOSE);
	}

	//2017-08-30,SKCHO AGV 신규 PIO 추가
	//unload abort io
	if(!theUnitFunc.GetOutPutIOState(Y_GOOD_ULD_CONV_TRAY_PIO_ABORT))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_AGV_OK_UNLOAD_ABORT_Y, uOPEN);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_AGV_OK_UNLOAD_ABORT_Y, uCLOSE);
	}

	//load abort io
	if(!theUnitFunc.GetOutPutIOState(Y_GOOD_LD_CONV_TRAY_PIO_ABORT))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_AGV_OK_LOAD_ABORT_Y, uOPEN);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_AGV_OK_LOAD_ABORT_Y, uCLOSE);
	}
	//UNLOADING
	if(!theUnitFunc.GetOutPutIOState(Y_GOOD_ULD_CONV_TRAY_PIO_UNLOADING))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_AGV_OK_UNLOAD_ON_Y, uOPEN);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_AGV_OK_UNLOAD_ON_Y, uCLOSE);
	}
	//LOADING
	if(!theUnitFunc.GetOutPutIOState(Y_GOOD_LD_CONV_TRAY_PIO_LOADING))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_AGV_OK_LOAD_ON_Y, uOPEN);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_AGV_OK_LOAD_ON_Y, uCLOSE);
	}

}
//2017-04-13,skcho, 옵션기능 화면에 표시
void CFormMain::OptionStatusDisplay(void)
{

	if(theConfigBank.m_Option.m_bUseLotEnd)
	{
		CGxUICtrl::SetStaticVisible(this,IDC_GXSTC_MAIN_LOTEND,TRUE);
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_MAIN_LOTEND, GXCOLOR_RED);
	}
	else
	{		
		CGxUICtrl::SetStaticVisible(this,IDC_GXSTC_MAIN_LOTEND,FALSE);
	}

	if(theConfigBank.m_Option.m_bUseDryRun)
	{
		CGxUICtrl::SetStaticVisible(this,IDC_GXSTC_MAIN_DRYRUN,TRUE);
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_MAIN_DRYRUN, GXCOLOR_RED);

		//2017-04-13,skcho, DRYRUN 옵션일때만 버튼 표시
		CGxUICtrl::SetStaticVisible(this,IDC_GXBTN_MODE_PM2,TRUE);
		
	}
	else
	{
		CGxUICtrl::SetStaticVisible(this,IDC_GXSTC_MAIN_DRYRUN,FALSE);
		CGxUICtrl::SetStaticVisible(this,IDC_GXBTN_MODE_PM2,FALSE);
	}

	if(!theConfigBank.m_Option.m_bUseRCMD)
	{
		CGxUICtrl::SetStaticVisible(this,IDC_GXSTC_MAIN_RCMDOFF,TRUE);
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_MAIN_RCMDOFF, GXCOLOR_RED);
	}
	else
	{
		CGxUICtrl::SetStaticVisible(this,IDC_GXSTC_MAIN_RCMDOFF,FALSE);
	}

	if(theConfigBank.m_Option.m_bUseCellInfoSkip)
	{
		CGxUICtrl::SetStaticVisible(this,IDC_GXSTC_MAIN_CELLINFOSKIP,TRUE);
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_MAIN_CELLINFOSKIP, GXCOLOR_RED);
	}
	else
	{
		CGxUICtrl::SetStaticVisible(this,IDC_GXSTC_MAIN_CELLINFOSKIP,FALSE);
	}

	if(theConfigBank.m_Option.m_bUseCarrierReportSkip)
	{
		CGxUICtrl::SetStaticVisible(this,IDC_GXSTC_MAIN_CARRIERREPORT,TRUE);
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_MAIN_CARRIERREPORT, GXCOLOR_RED);
	}
	else
	{
		CGxUICtrl::SetStaticVisible(this,IDC_GXSTC_MAIN_CARRIERREPORT,FALSE);
	}

	if(theConfigBank.m_Option.m_bLoadPitchIn)
	{
		CGxUICtrl::SetStaticVisible(this,IDC_GXSTC_MAIN_PITCHIN,TRUE);
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_MAIN_PITCHIN, GXCOLOR_RED);
	}
	else
	{
		CGxUICtrl::SetStaticVisible(this,IDC_GXSTC_MAIN_PITCHIN,FALSE);
	}
/*
	if(theConfigBank.m_Option.m_bUseManualReceive)
	{
		CGxUICtrl::SetStaticVisible(this,IDC_GXSTC_MAIN_MANUAL,TRUE);
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_MAIN_MANUAL, GXCOLOR_RED);
	}
	else
	{
		CGxUICtrl::SetStaticVisible(this,IDC_GXSTC_MAIN_MANUAL,FALSE);
	}
*/	
	if(theConfigBank.m_Option.m_bUseJudgeOK)
	{
		CGxUICtrl::SetStaticVisible(this,IDC_GXSTC_MAIN_JUDGE_OK,TRUE);
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_MAIN_JUDGE_OK, GXCOLOR_RED);
	}
	else
	{		
		CGxUICtrl::SetStaticVisible(this,IDC_GXSTC_MAIN_JUDGE_OK,FALSE);
	}

	if(theConfigBank.m_Option.m_bUseJobEnd)
	{
		CGxUICtrl::SetStaticVisible(this,IDC_GXSTC_MAIN_JOBEND,TRUE);
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_MAIN_JOBEND, GXCOLOR_RED);
	}
	else
	{		
		CGxUICtrl::SetStaticVisible(this,IDC_GXSTC_MAIN_JOBEND,FALSE);
	}

	if(theConfigBank.m_Option.m_bUseContactRetry)
	{
		CGxUICtrl::SetStaticVisible(this,IDC_GXSTC_MAIN_CONTACT_RETRY,TRUE);
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_MAIN_CONTACT_RETRY, GXCOLOR_RED);
	}
	else
	{		
		CGxUICtrl::SetStaticVisible(this,IDC_GXSTC_MAIN_CONTACT_RETRY,FALSE);
	}

	if(theConfigBank.m_Option.m_bGrabMCRUse)
	{
		CGxUICtrl::SetStaticVisible(this,IDC_GXSTC_MAIN_MCRGRAB,TRUE);
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_MAIN_MCRGRAB, GXCOLOR_RED);
	}
	else
	{		
		CGxUICtrl::SetStaticVisible(this,IDC_GXSTC_MAIN_MCRGRAB,FALSE);
	}

	CGxUICtrl::SetStaticString(this,IDC_GXSTC_MAIN_REJECTCODE,theConfigBank.m_Option.m_strRejectCode);
	CGxUICtrl::SetStaticColor(this,IDC_GXSTC_MAIN_REJECTCODE, GXCOLOR_RED);
	CGxUICtrl::SetStaticVisible(this,IDC_GXSTC_MAIN_REJECTCODE,TRUE);

	//2017-09-01,SKCHO, 옵션 표시 추가
	if(theConfigBank.m_Option.m_bSkipLDConvSen3)
	{
		CGxUICtrl::SetStaticVisible(this,IDC_GXSTC_LDCONVSEN3,TRUE);
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_LDCONVSEN3, GXCOLOR_RED);
	}
	else
	{		
		CGxUICtrl::SetStaticVisible(this,IDC_GXSTC_LDCONVSEN3,FALSE);
	}

	//2017-09-17,SKCHO, MCR 옵션 추가
    if(!theConfigBank.m_Option.m_bUseLowerMCRRead)
	{
		CGxUICtrl::SetStaticVisible(this,IDC_GXSTC_LOWERMCRSKIP,TRUE);
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_LOWERMCRSKIP, GXCOLOR_RED);
	}
	else
	{		
		CGxUICtrl::SetStaticVisible(this,IDC_GXSTC_LOWERMCRSKIP,FALSE);
	}	
	if(!theConfigBank.m_Option.m_bUseUpperMCRRead)
	{
		CGxUICtrl::SetStaticVisible(this,IDC_GXSTC_UPPERMCRSKIP,TRUE);
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UPPERMCRSKIP, GXCOLOR_RED);
	}
	else
	{		
		CGxUICtrl::SetStaticVisible(this,IDC_GXSTC_UPPERMCRSKIP,FALSE);
	}

	if(theConfigBank.m_Option.m_bUseTrayVI)
	{
		CGxUICtrl::SetStaticVisible(this,IDC_GXSTC_TRAYVISION,TRUE);
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_TRAYVISION, GXCOLOR_RED);
	}
	else
	{		
		CGxUICtrl::SetStaticVisible(this,IDC_GXSTC_TRAYVISION,FALSE);
	}

	
	//2017-11-28,SKCHO, GOOD TRAY TR RETRY UI 표시
	if(theConfigBank.m_Option.m_dGoodTRRetryLength == 0)
	{		
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_MAIN_CELLMOVE_UDCONVTR, GXCOLOR_ALICE_BLUE);
	}
	else
	{		
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_MAIN_CELLMOVE_UDCONVTR, GXCOLOR_LIGHT_BLUE);
	}
	//NG TRAY TR RETRY UI 표시
	if(theConfigBank.m_Option.m_dNGTRRetryLength == 0)
	{		
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_MAIN_CELLMOVE_UDCONVTR2, GXCOLOR_ALICE_BLUE);
	}
	else
	{		
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_MAIN_CELLMOVE_UDCONVTR2, GXCOLOR_LIGHT_BLUE);
	}
	//LD ROBOT RETRY UI 표시
	if(theConfigBank.m_Option.m_bUseLDRobotRetry==FALSE)
	{		
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_MAIN_CELLMOVE_SCARA, GXCOLOR_ALICE_BLUE);
	}
	else
	{		
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_MAIN_CELLMOVE_SCARA, GXCOLOR_LIGHT_BLUE);
	}
	//PA TR RETRY UI 표시
	if(theConfigBank.m_Option.m_bUsePATRRetry == FALSE)
	{		
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_MAIN_CELLMOVE_CELLTR, GXCOLOR_ALICE_BLUE);
	}
	else
	{		
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_MAIN_CELLMOVE_CELLTR, GXCOLOR_LIGHT_BLUE);
	}

	//UD TR
	if(theConfigBank.m_Option.m_bUseULDTRMode == FALSE)
	{		
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_MAIN_CELLMOVE_TRAY1, GXCOLOR_ALICE_BLUE);
	}
	else
	{		
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_MAIN_CELLMOVE_TRAY1, GXCOLOR_LIGHT_BLUE);
	}


	//PASTAGE ORG 표시
	if(theProcBank.m_bPreAlignOrgStart == FALSE)
	{		
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_MAIN_PASTAGE, GXCOLOR_ALICE_BLUE);
		CGxUICtrl::SetStaticString(this, IDC_GXSTC_MAIN_PASTAGE, _T("PRE ALIGN STAGE"));
	}
	else
	{		
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_MAIN_PASTAGE, GXCOLOR_LIGHT_CORAL);
		CGxUICtrl::SetStaticString(this, IDC_GXSTC_MAIN_PASTAGE, _T("PRE ALIGN STAGE\n\tORIGIN..."));
	}
	
}
//20170205 byskcho
//mcr reading 화면 표시
LRESULT CFormMain::OnMCRUpdate(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	int nChannel = (int)wParam;
	int nPos =(int)lParam;

	CCellTag tagCell;
	CCellInfo* pCellInfo;
	CString strCellID;

	CString strPos;
	int nFailCnt = 0;
	if(nPos == LOWER_MCR) //LD_TR
	{
		if(nChannel == JIG_CH_1)
		{
			tagCell = theCellBank.GetCellTag(CELL_POS_LD_TR_CH1);
			pCellInfo = theCellBank.GetCellInfo(CELL_POS_LD_TR_CH1);
			nFailCnt = theProcBank.m_nCH1LowerMCRFailCnt;
		}
		else
		{
			tagCell = theCellBank.GetCellTag(CELL_POS_LD_TR_CH2);
			pCellInfo = theCellBank.GetCellInfo(CELL_POS_LD_TR_CH2);
			nFailCnt = theProcBank.m_nCH2LowerMCRFailCnt;
		}
		strPos = _T("LOWER");
	}
	else         //AZONE
	{
		if(nChannel == JIG_CH_1)
		{
			tagCell = theCellBank.GetCellTag(CELL_POS_AZONE_CH1);
			pCellInfo = theCellBank.GetCellInfo(CELL_POS_AZONE_CH1);
			nFailCnt = theProcBank.m_nCH1UpperMCRFailCnt;
		}
		else
		{
			tagCell = theCellBank.GetCellTag(CELL_POS_AZONE_CH2);
			pCellInfo = theCellBank.GetCellInfo(CELL_POS_AZONE_CH2);
			nFailCnt = theProcBank.m_nCH2UpperMCRFailCnt;
		}
		strPos = _T("UPPER");
	}
	if(nChannel == JIG_CH_1)
	{
		strCellID = pCellInfo->m_strCellID;
		if(pCellInfo->m_strCellID == _T("SKIP"))
		{
			CGxUICtrl::SetStaticColor(this,IDC_GXSTC_CH1_MCR_DATA, RGB(100,100,100));
			
		}
		else if( pCellInfo->m_strCellID != _T("FAIL") &&  pCellInfo->m_strCellID != _T(""))
	    //if( pCellInfo->m_strCellID != _T("UV_FAIL") &&  pCellInfo->m_strCellID != _T("")) //2017-04-05,skcho, 코그닉스에서 FAIL 로 데이터를 줌
		{
			CGxUICtrl::SetStaticColor(this,IDC_GXSTC_CH1_MCR_DATA, RGB(0,255,0));			
		}		
		else
		{
			 CGxUICtrl::SetStaticColor(this,IDC_GXSTC_CH1_MCR_DATA, RGB(255,0,0));
			 strCellID.Format(_T("%s CH%d %s Count %d"),strPos,nChannel+1,pCellInfo->m_strCellID,nFailCnt); 
		}
		 CGxUICtrl::SetStaticString(this,IDC_GXSTC_CH1_MCR_DATA,strCellID);
	}
	else
	{
		strCellID = pCellInfo->m_strCellID;
		if(pCellInfo->m_strCellID == _T("SKIP"))
		{
			CGxUICtrl::SetStaticColor(this,IDC_GXSTC_CH2_MCR_DATA, RGB(100,100,100));
		}
		else if( pCellInfo->m_strCellID != _T("FAIL") &&  pCellInfo->m_strCellID != _T(""))
		//if( pCellInfo->m_strCellID != _T("UV_FAIL") &&  pCellInfo->m_strCellID != _T("")) //2017-04-05,skcho, 코그닉스에서 FAIL 로 데이터를 줌
		{
			CGxUICtrl::SetStaticColor(this,IDC_GXSTC_CH2_MCR_DATA, RGB(0,255,0));		
		}
		
		else
		{
			 CGxUICtrl::SetStaticColor(this,IDC_GXSTC_CH2_MCR_DATA, RGB(255,0,0));	
			  strCellID.Format(_T("%s CH%d %s Count %d"),strPos,nChannel+1,pCellInfo->m_strCellID,nFailCnt); 
		}
		CGxUICtrl::SetStaticString(this,IDC_GXSTC_CH2_MCR_DATA,strCellID);
	}

	return 0;
}
//20170205 byskcho
//mcr reading 화면 표시
LRESULT CFormMain::OnDCRUpdate(WPARAM wParam, LPARAM lParam)
{
	CGxUICtrl::SetStaticColor(this,IDC_GXSTC_DCR_DATA, RGB(0,255,0));
	if(theProcBank.m_strTrayID != _T(""))
	{
		CGxUICtrl::SetStaticString(this,IDC_GXSTC_DCR_DATA,theProcBank.m_strTrayID);
	
	}
	else
	{
		CGxUICtrl::SetStaticString(this,IDC_GXSTC_DCR_DATA,_T("DCR_FAIL"));
	}
	return 0;
}
//20170205 byskcho
//mcr reading 화면 표시
LRESULT CFormMain::OnCZONEUpdate(WPARAM wParam, LPARAM lParam)
{
	int nPos =(int)lParam;
	CString strData;
	if(nPos == 1)
	{
		strData.Format(_T("CH1_TH:%.5f"),	theUnitFunc.CellData_GetVTAlignth(JIG_CH_1,ZONE_ID_B));
	}
	else
	{	
		strData.Format(_T("CH2_TH:%.5f"),	theUnitFunc.CellData_GetVTAlignth(JIG_CH_2,ZONE_ID_B));
	}
	LibShowOut2(strData);
	return 0;
}
//20170205 byskcho
//mcr reading 화면 표시
LRESULT CFormMain::OnUpper_Eqp_Com_Update(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	int nOnOff = (int)wParam;
	int nPos = (int)lParam;


	switch(nPos)
	{
	case  OPTICAL_XY_SEND_ABLE:
		if(nOnOff == 1)
		{
			CGxUICtrl::SetStaticColor(this,IDC_GXSTC_SEND_ABLE, RGB(0,255,0));
		}
		else
		{
			CGxUICtrl::SetStaticColor(this,IDC_GXSTC_SEND_ABLE, RGB(255,255,255));
		}
		break;
	case  OPTICAL_XY_SEND_START:
		if(nOnOff == 1)
		{
			CGxUICtrl::SetStaticColor(this,IDC_GXSTC_SEND_START, RGB(0,255,0));
		}
		else
		{
			CGxUICtrl::SetStaticColor(this,IDC_GXSTC_SEND_START,RGB(255,255,255));
		}
		break;
	case OPTICAL_XY_SEND_COMP :
		if(nOnOff == 1)
		{
			CGxUICtrl::SetStaticColor(this,IDC_GXSTC_SEND_COMPLETE, RGB(0,255,0));
		}
		else
		{
			CGxUICtrl::SetStaticColor(this,IDC_GXSTC_SEND_COMPLETE, RGB(255,255,255));
		}
		break;
	case  OPTICAL_XY_SEND_IMM_PAUSE:
		if(nOnOff == 1)
		{
			CGxUICtrl::SetStaticColor(this,IDC_GXSTC_SEND_PAUSE, RGB(0,255,0));
		}
		else
		{
			CGxUICtrl::SetStaticColor(this,IDC_GXSTC_SEND_PAUSE, RGB(255,255,255));
		}
		break;
	case  OPTICAL_XY_RECEIVE_ABLE:
		if(nOnOff == 1)
		{
			CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UPPER_ABLE, RGB(0,255,0));
		}
		else
		{
			CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UPPER_ABLE, RGB(255,255,255));
		}
		break;
	case   OPTICAL_XY_RECEIVE_START:
		if(nOnOff == 1)
		{
			CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UPPER_START, RGB(0,255,0));
		}
		else
		{
			CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UPPER_START, RGB(255,255,255));
		}
		break;
	case  OPTICAL_XY_RECEIVE_COMP:
		if(nOnOff == 1)
		{
			CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UPPER_COMPLETE, RGB(0,255,0));
		}
		else
		{
			CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UPPER_COMPLETE, RGB(255,255,255));
		}
		break;
	case  OPTICAL_XY_RECEIVE_IMM_PAUSE:
		if(nOnOff == 1)
		{
			CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UPPER_PAUSE, RGB(0,255,0));
		}
		else
		{
			CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UPPER_PAUSE, RGB(255,255,255));
		}
		break;
	default:
		break;
	}


	return 0;
}

//2018wjp
LRESULT CFormMain::OnUpdateLoadCellID(WPARAM wParam, LPARAM lParam)
{
	CButton* pBtn=NULL;
	CString str=_T("");
	pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID11);
	pBtn->SetWindowTextW(_T("-"));
	pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID12);
	pBtn->SetWindowTextW(_T("-"));
	pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID13);
	pBtn->SetWindowTextW(_T("-"));
	pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID14);
	pBtn->SetWindowTextW(_T("-"));
	pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID21);
	pBtn->SetWindowTextW(_T("-"));
	pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID22);
	pBtn->SetWindowTextW(_T("-"));
	pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID23);
	pBtn->SetWindowTextW(_T("-"));
	pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID24);
	pBtn->SetWindowTextW(_T("-"));
	for(int i=0;i<CELL_ID_ROW_MAX;i++)
	{
		for(int j=0;j<CELL_ID_COL_MAX;j++)
		{
			if(i==0&&j==0)
			{
				pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID11);
				str=theProcBank.strCIMLoadCellID[0][0]==""?_T("-"):theProcBank.strCIMLoadCellID[0][0];
			}
			else if(i==0&&j==1)
			{
				pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID12);
				str=theProcBank.strCIMLoadCellID[0][1]==""?_T("-"):theProcBank.strCIMLoadCellID[0][1];
			}
			else if(i==0&&j==2)
			{
				pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID13);
				str=theProcBank.strCIMLoadCellID[0][2]==""?_T("-"):theProcBank.strCIMLoadCellID[0][2];
			}
			else if(i==0&&j==3)
			{
				pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID14);
				str=theProcBank.strCIMLoadCellID[0][3]==""?_T("-"):theProcBank.strCIMLoadCellID[0][3];
			}

			else if(i==1&&j==0)
			{
				pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID21);
				str=theProcBank.strCIMLoadCellID[1][0]==""?_T("-"):theProcBank.strCIMLoadCellID[1][0];
				str=theProcBank.strCIMLoadCellID[0][0]==""?_T("-"):theProcBank.strCIMLoadCellID[1][0];
			}
			else if(i==1&&j==1)
			{
				pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID22);
				str=theProcBank.strCIMLoadCellID[1][1]==""?_T("-"):theProcBank.strCIMLoadCellID[1][1];
			}
			else if(i==1&&j==2)
			{
				pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID23);
				str=theProcBank.strCIMLoadCellID[1][2]==""?_T("-"):theProcBank.strCIMLoadCellID[1][2];
			}
			else if(i==1&&j==3)
			{
				pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID24);
				str=theProcBank.strCIMLoadCellID[1][3]==""?_T("-"):theProcBank.strCIMLoadCellID[1][3];
			}

			pBtn->SetWindowTextW(str);
		}
	}
	pBtn=(CButton*)GetDlgItem(IDC_BTN_TRAYID);
	pBtn->SetWindowTextW(theProcBank.m_strTrayID);
	return 0;
}

LRESULT CFormMain::OnUpdateOneCellID(WPARAM wParam, LPARAM lParam)
{
	Cell_Coord* sCell=(Cell_Coord*)wParam;
	int i=sCell->row;
	int j=sCell->col;
	CButton* pBtn=NULL;
	CString strID=sCell->strCode.Trim();
	strID==strID.IsEmpty()?_T("FAIL"):strID;
	if(i==1&&j==1)
	{
		pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID11);
	}
	else if(i==1&&j==2)
	{
		pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID12);
	}
	else if(i==1&&j==3)
	{
		pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID13);
	}
	else if(i==1&&j==4)
	{
		pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID14);
	}

	else if(i==2&&j==1)
	{
		pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID21);
	}
	else if(i==2&&j==2)
	{
		pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID22);
	}
	else if(i==2&&j==3)
	{
		pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID23);
	}
	else if(i==2&&j==4)
	{
		pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID24);
	}
	if(NULL!=pBtn)
		pBtn->SetWindowTextW(strID);
	return 0;
}

LRESULT CFormMain::OnRemoveOneCellID(WPARAM wParam, LPARAM lParam)
{
	int i=wParam;
	int j=lParam;
	CButton* pBtn=NULL;
	if(i==1&&j==1)
	{
		pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID11);
	}
	else if(i==1&&j==2)
	{
		pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID12);
	}
	else if(i==1&&j==3)
	{
		pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID13);
	}
	else if(i==1&&j==4)
	{
		pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID14);
	}

	else if(i==2&&j==1)
	{
		pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID21);
	}
	else if(i==2&&j==2)
	{
		pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID22);
	}
	else if(i==2&&j==3)
	{
		pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID23);
	}
	else if(i==2&&j==4)
	{
		pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID24);
	}
	if(NULL!=pBtn)
		pBtn->SetWindowTextW(_T("-"));
	return 0;
}

LRESULT CFormMain::OnRemoveALLCellID(WPARAM wParam, LPARAM lParam)
{
	CButton* pBtn=NULL;
	pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID11);
	pBtn->SetWindowTextW(_T("-"));
	pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID12);
	pBtn->SetWindowTextW(_T("-"));
	pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID13);
	pBtn->SetWindowTextW(_T("-"));
	pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID14);
	pBtn->SetWindowTextW(_T("-"));
	pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID21);
	pBtn->SetWindowTextW(_T("-"));
	pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID22);
	pBtn->SetWindowTextW(_T("-"));
	pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID23);
	pBtn->SetWindowTextW(_T("-"));
	pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID24);
	pBtn->SetWindowTextW(_T("-"));
	pBtn=(CButton*)GetDlgItem(IDC_BTN_TRAYID);
	pBtn->SetWindowTextW(_T("-"));
	return 0;
}


void CFormMain::ClickGxstcSendAble()
{
	m_bAble = !m_bAble;
	theUnitFunc.SendRcvEnable((ONOFF)!m_bAble);

}


void CFormMain::ClickGxstcSendStart()
{
	m_bStart = !m_bStart;
	theUnitFunc.SendRcvStart((ONOFF)m_bStart);
	if(m_bStart)
	{
		theUnitFunc.LDConvMove();
	}
}


void CFormMain::ClickGxstcSendComplete()
{
	m_bComp = !m_bComp;
	theUnitFunc.SendRcvComp((ONOFF)m_bComp);
	theUnitFunc.LDConvStop();
	if(m_bComp)
	{
		theUnitFunc.LDConvStop();
		theUnitFunc.LDConvZeroPointSet();
	}
}


void CFormMain::ClickGxstcCellmovePos33()
{
	
	CGxMsgBox	dlgMsgBox;
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN 
		|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE
		|| !theUnitFunc.TrayData_Exist(NG_TRAY_PORT, TRAY_INFO_POS_NG_IN_LIFT_ARM))//2017-03-02, skcho, 런상태에서는 무시
	{
		return;
	}
	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
	dlgMsgBox.SetMessage(TRUE, 
			_T("NG IN LIFT ARM TRAY 정보를 삭제 하시겠습니까?"),	//한
			_T("Do you want delete NG IN LIFT ARM Tray Information?"),		//영
			_T("Do you want delete NG IN LIFT ARM Tray Information?") );		//베

	if(dlgMsgBox.DoModal() == IDOK)
	{
		// NG TRAY IN LIFT ARM TRAY INFO DEL
		theUnitFunc.TrayData_RemoveData(NG_TRAY_PORT,TRAY_INFO_POS_NG_IN_LIFT_ARM);

		//2017-07-04,SKCHO
		theLog[LOG_SEQUENCE_TRAY].AddBuf( _T("NG IN LIFT ARM 트레이 정보 삭제"));
	}
	else
	{
		return;
	}
}


void CFormMain::ClickGxstcCellmovePos31()
{
	CGxMsgBox	dlgMsgBox;
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN 
		|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE
		|| !theUnitFunc.TrayData_Exist(NG_TRAY_PORT, TRAY_INFO_POS_NG_OUT_REQ_BUFFER))//2017-03-02, skcho, 런상태에서는 무시
	{
		return;
	}
	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
	dlgMsgBox.SetMessage(TRUE, 
			_T("NG OUT REQ BUF TRAY 정보를 삭제 하시겠습니까?"),	//한
			_T("Do you want delete NG OUT REQ BUF Tray Information?"),		//영
			_T("Do you want delete NG OUT REQ BUF Tray Information?") );		//베

	if(dlgMsgBox.DoModal() == IDOK)
	{
		// NG TRAY REQUEST BUFFER TRAY INFO DEL
		theUnitFunc.TrayData_RemoveData(NG_TRAY_PORT,TRAY_INFO_POS_NG_OUT_REQ_BUFFER);

		//2017-07-04,SKCHO
		theLog[LOG_SEQUENCE_TRAY].AddBuf( _T("NG OUT REQ BUF 트레이 정보 삭제"));
	}
	else
	{
		return;
	}
	
}


void CFormMain::ClickGxstcCellmovePos32()
{
	CGxMsgBox	dlgMsgBox;
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN 
		|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE
		|| !theUnitFunc.TrayData_Exist(NG_TRAY_PORT, TRAY_INFO_POS_NG_IN_BUFFER))//2017-03-02, skcho, 런상태에서는 무시
	{
		return;
	}
	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
	dlgMsgBox.SetMessage(TRUE, 
			_T("NG IN BUF TRAY 정보를 삭제 하시겠습니까?"),	//한
			_T("Do you want delete NG IN BUF Tray Information?"),		//영
			_T("Do you want delete NG IN BUF Tray Information?") );		//베

	if(dlgMsgBox.DoModal() == IDOK)
	{
		// NG TRAY REQUEST BUFFER TRAY INFO DEL
		theUnitFunc.TrayData_RemoveData(NG_TRAY_PORT,TRAY_INFO_POS_NG_IN_BUFFER);

		//2017-07-04,SKCHO
		theLog[LOG_SEQUENCE_TRAY].AddBuf( _T("NG IN BUF 트레이 정보 삭제"));
	}
	else
	{
		return;
	}
}


void CFormMain::ClickGxstcCellmovePos36()
{
	CGxMsgBox	dlgMsgBox;
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN 
		|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE
		|| !theUnitFunc.TrayData_Exist(NG_TRAY_PORT, TRAY_INFO_POS_NG_OUT_COMP_BUFFER))//2017-03-02, skcho, 런상태에서는 무시
	{
		return;
	}
	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
	dlgMsgBox.SetMessage(TRUE, 
			_T("NG OUT COMP BUF TRAY 정보를 삭제 하시겠습니까?"),	//한
			_T("Do you want delete NG OUT COMP BUF Tray Information?"),		//영
			_T("Do you want delete NG OUT COMP BUF Tray Information?") );		//베

	if(dlgMsgBox.DoModal() == IDOK)
	{
		// NG TRAY REQUEST BUFFER TRAY INFO DEL
		theUnitFunc.TrayData_RemoveData(NG_TRAY_PORT,TRAY_INFO_POS_NG_OUT_COMP_BUFFER);

		//2017-07-04,SKCHO
		theLog[LOG_SEQUENCE_TRAY].AddBuf( _T("NG OUT COMP BUF 트레이 정보 삭제"));
	}
	else
	{
		return;
	}
}


void CFormMain::ClickGxstcCellmovePos38()
{
	CGxMsgBox	dlgMsgBox;
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN 
		|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE
		|| !theUnitFunc.TrayData_Exist(NG_TRAY_PORT, TRAY_INFO_POS_NG_TR))//2017-03-02, skcho, 런상태에서는 무시
	{
		return;
	}
	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
	dlgMsgBox.SetMessage(TRUE, 
			_T("NG TR TRAY 정보를 삭제 하시겠습니까?"),	//한
			_T("Do you want delete NG TR Tray Information?"),		//영
			_T("Do you want delete NG TR Tray Information?") );		//베

	if(dlgMsgBox.DoModal() == IDOK)
	{
		// NG TRAY REQUEST BUFFER TRAY INFO DEL
		theUnitFunc.TrayData_RemoveData(NG_TRAY_PORT,TRAY_INFO_POS_NG_TR);

		//2017-07-04,SKCHO
		theLog[LOG_SEQUENCE_TRAY].AddBuf( _T("NG TR 트레이 정보 삭제"));
	}
	else
	{
		return;
	}
}


void CFormMain::ClickGxstcCellmovePos37()
{
	CGxMsgBox	dlgMsgBox;
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN 
		|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE
		|| !theUnitFunc.TrayData_Exist(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_TR))//2017-03-02, skcho, 런상태에서는 무시
	{
		return;
	}
	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
	dlgMsgBox.SetMessage(TRUE, 
			_T("G00D TR TRAY 정보를 삭제 하시겠습니까?"),	//한
			_T("Do you want delete G00D TR Tray Information?"),		//영
			_T("Do you want delete G00D TR Tray Information?") );		//베

	if(dlgMsgBox.DoModal() == IDOK)
	{
		// NG TRAY REQUEST BUFFER TRAY INFO DEL
		theUnitFunc.TrayData_RemoveData(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_TR);

		//2017-07-04,SKCHO
		theLog[LOG_SEQUENCE_TRAY].AddBuf( _T("G00D TR 트레이 정보 삭제"));
	}
	else
	{
		return;
	}
}


void CFormMain::ClickGxstcCellmovePos29()
{
	CGxMsgBox	dlgMsgBox;
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN 
		|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE
		|| !theUnitFunc.TrayData_Exist(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_OUT_LIFT))//2017-03-02, skcho, 런상태에서는 무시
	{
		return;
	}

	//2017-07-17,SKCHO GOOD  OUT LIFT SENSOR감지 표시
	if(theUnitFunc.GetInPutIOCheck(X_GOOD_ULD_TRAY_LIFT_SEN))
	{
		return;
	}

	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
	dlgMsgBox.SetMessage(TRUE, 
			_T("G00D OUT LIFT TRAY 정보를 삭제 하시겠습니까?"),	//한
			_T("Do you want delete G00D OUT LIFT Tray Information?"),		//영
			_T("Do you want delete G00D OUT LIFT Tray Information?") );		//베

	if(dlgMsgBox.DoModal() == IDOK)
	{
		// NG TRAY REQUEST BUFFER TRAY INFO DEL
		theProcBank.ResetGoodOutLiftCount();  //2017-04-20, skcho, tray이 데이터 삭제할때 카운트 개수도 초기화
		theUnitFunc.TrayData_RemoveData(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_OUT_LIFT);

		//2017-07-04,SKCHO
		theLog[LOG_SEQUENCE_TRAY].AddBuf( _T("G00D OUT LIFT 트레이 정보 삭제"));

		//2017-07-20,SKCHO, GOOD OUT LIFT와 같이 삭제(수동으로 제거할때 필요)
		theProcBank.ResetSubGoodAlignCount();
		theUnitFunc.TrayData_RemoveData(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_ALIGN);
	}
	else
	{
		return;
	}
}


void CFormMain::ClickGxstcCellmovePos30()
{
	CGxMsgBox	dlgMsgBox;
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN 
		|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)//2017-03-02, skcho, 런상태에서는 무시
	{
		return;
	}
	
	if(theUnitFunc.TrayData_Exist(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_OUT_BUFFER))
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(TRUE, 
				_T("G00D OUT BUF TRAY 정보를 삭제 하시겠습니까?"),	//한
				_T("Do you want delete G00D OUT BUF Tray Information?"),		//영
			    _T("Do you want delete G00D OUT BUF Tray Information?") );		//베

		if(dlgMsgBox.DoModal() == IDOK)
		{
			// NG TRAY REQUEST BUFFER TRAY INFO DEL
			theUnitFunc.TrayData_RemoveData(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_OUT_BUFFER);

			//2017-07-04,SKCHO
		   theLog[LOG_SEQUENCE_TRAY].AddBuf( _T("G00D OUT BUF 트레이 정보 삭제"));
		}
		else
		{
			return;
		}
	}
	else
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(TRUE, 
				_T("G00D OUT BUF TRAY 정보를 생성 하시겠습니까?"),	//한
				_T("Do you want create G00D OUT BUF TRAY Information?"),		//영
			    _T("Do you want create G00D OUT BUF TRAY Information?") );		//베

		if(dlgMsgBox.DoModal() == IDOK)
		{
			// NG TRAY REQUEST BUFFER TRAY INFO DEL
			theUnitFunc.TrayData_Create(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_OUT_BUFFER);
			//2017-07-24,JSJUNG, GOOD TRAY OUT BUFFER에 정보 생성시 352보고 추가(AGV 호출용)
			theUnitFunc.CIM_TRAYProcess(_T("352"),_T("2"));

			//2017-12-04,SKCHO, 352보고 후 프로그램 재시작 후 정보 생성했을 때 353보고 조건 만들어줌
			theUnitFunc.ReadPortEvent353();
		}
		else
		{
			return;
		}
	}
}


void CFormMain::ClickGxstcCellmovePos28()
{
	CGxMsgBox	dlgMsgBox;
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN 
		|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE
		|| !theUnitFunc.TrayData_Exist(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_IN_ALIGN))//2017-03-02, skcho, 런상태에서는 무시
	{
		return;
	}

	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
	dlgMsgBox.SetMessage(TRUE, 
			_T("G00D IN ALIGN TRAY 정보를 삭제 하시겠습니까?"),	//한
			_T("Do you want delete G00D IN ALIGN Tray Information?"),		//영
			_T("Do you want delete G00D IN ALIGN Tray Information?") );		//베

	if(dlgMsgBox.DoModal() == IDOK)
	{
		//2017-07-10,SKCHO, 카운트를 삭제하면 260 뚜껑보고시 트레이 개수가 0으로 보고되는 버그 때문에 삭제
		//theProcBank.ResetSubGoodAlignCount();  //2017-07-07, skcho, tray이 데이터 삭제할때 카운트 개수도 초기화, 뚜껑덮기용
	
		//2017-07-10, SKCHO,카운트를 삭제하면 260 뚜껑보고시 트레이 개수가 0으로 보고되는 버그 수정
		theProcBank.MinusSubGoodAlignCount();
		theProcBank.MinusGoodAlignCount();

		theUnitFunc.TrayData_RemoveData(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_ALIGN);
		theProcBank.m_bGoodTrayReq = FALSE;  //뚜껑 release 보고 실패후 정보제거시 350 보고하기 위함. 20170513, JSJUNG

		//2017-07-04,SKCHO
		theLog[LOG_SEQUENCE_TRAY].AddBuf( _T("GOOD IN ALIGN 트레이 정보 삭제"));
		
	}
	else
	{
		return;
	}
}


void CFormMain::ClickGxstcCellmovePos27()
{
	CGxMsgBox	dlgMsgBox;
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN 
		|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE
		|| !theUnitFunc.TrayData_Exist(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_IN_LIFT))//2017-03-02, skcho, 런상태에서는 무시
	{
		return;
	}
	
	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
	dlgMsgBox.SetMessage(TRUE, 
			_T("G00D IN LIFT TRAY 정보를 삭제 하시겠습니까?"),	//한
			_T("Do you want delete G00D IN LIFT Tray Information?"),		//영
			_T("Do you want delete G00D IN LIFT Tray Information?") );		//베

	if(dlgMsgBox.DoModal() == IDOK)
	{		
		theProcBank.ResetGoodAlignCount();  //2017-07-07, skcho, tray이 데이터 삭제할때 카운트 개수도 초기화, 뚜껑열기용

		// TRAY_INFO_POS_GOOD_IN_LIFTDEL
		theUnitFunc.TrayData_RemoveData(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_LIFT);
		
		//2017-07-08,SKCHO,DCR읽기실패하여 트레이를 메뉴얼로 전부 배출했을 경우 다시 시작하면 m_bDCRChk = FALSE상태이르로 DCR 읽기 동작을 계속하게됨 방지
		theProcBank.m_bDCRChk = TRUE;
		
		//2017-07-04,SKCHO
		theLog[LOG_SEQUENCE_TRAY].AddBuf( _T("GOOD IN LIFT 트레이 정보 삭제"));
	
	}
	else
	{
		return;
	}
}


void CFormMain::ClickGxstcCellmovePos26()
{
	CGxMsgBox	dlgMsgBox;
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN 
		|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE
		|| !theUnitFunc.TrayData_Exist(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_IN_BUFFER))//2017-03-02, skcho, 런상태에서는 무시
	{
		return;
	}
	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
	dlgMsgBox.SetMessage(TRUE, 
			_T("G00D IN BUF CONV 트레이 정보를 삭제 하시겠습니까?"),	//한
			_T("Do you want delete G00D IN BUF CONV Tray Information?"),		//영
			_T("Do you want delete G00D IN BUF CONV Tray Information?") );		//베

	if(dlgMsgBox.DoModal() == IDOK)
	{
		theUnitFunc.TrayData_RemoveData(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_BUFFER);
		// 버퍼에 트레이 들어온걸 지워버리면 350 보고를 다시 해준다 2017/06/21 HSK.
		theUnitFunc.CIM_TRAYProcess(_T("350"),_T("1"));

		//2017-07-04,SKCHO
		theLog[LOG_SEQUENCE_TRAY].AddBuf( _T("GOOD IN BUF CONV 트레이 정보 삭제"));
	}
	else
	{
		return;
	}
}


void CFormMain::ClickGxstcCellmovePos34()
{
	CGxMsgBox	dlgMsgBox;
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN 
		|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE
		|| !theUnitFunc.TrayData_Exist(NG_TRAY_PORT, TRAY_INFO_POS_NG_IN_ALIGN))//2017-03-02, skcho, 런상태에서는 무시
	{
		return;
	}
	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
	dlgMsgBox.SetMessage(TRUE, 
			_T("NG IN ALIGN TRAY 정보를 삭제 하시겠습니까?"),	//한
			_T("Do you want delete NG IN ALIGN Tray Information?"),		//영
			_T("Do you want delete NG IN ALIGN Tray Information?") );		//베

	if(dlgMsgBox.DoModal() == IDOK)
	{
		// NG TRAY REQUEST BUFFER TRAY INFO DEL
		theUnitFunc.TrayData_RemoveData(NG_TRAY_PORT,TRAY_INFO_POS_NG_IN_ALIGN);

		//2017-07-04,SKCHO
		theLog[LOG_SEQUENCE_TRAY].AddBuf( _T("NG IN ALIGN 트레이 정보 삭제"));
	}
	else
	{
		return;
	}
}


void CFormMain::ClickGxstcCellmovePos35()
{
	CGxMsgBox	dlgMsgBox;
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN 
		|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE
		|| !theUnitFunc.TrayData_Exist(NG_TRAY_PORT, TRAY_INFO_POS_NG_OUT_LIFT_ARM))//2017-03-02, skcho, 런상태에서는 무시
	{
		return;
	}
	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
	dlgMsgBox.SetMessage(TRUE, 
			_T("NG OUT LIFT TRAY 정보를 삭제 하시겠습니까?"),	//한
			_T("Do you want delete NG OUT LIFT Tray Information?"),		//영
			_T("Do you want delete NG OUT LIFT Tray Information?") );		//베

	if(dlgMsgBox.DoModal() == IDOK)
	{
		// NG TRAY REQUEST BUFFER TRAY INFO DEL
		theUnitFunc.TrayData_RemoveData(NG_TRAY_PORT,TRAY_INFO_POS_NG_OUT_LIFT_ARM);

		//2017-07-04,SKCHO
		theLog[LOG_SEQUENCE_TRAY].AddBuf( _T("NG OUT LIFT 트레이 정보 삭제"));
	}
	else
	{
		return;
	}
}

LRESULT CFormMain::OnIDLEChangeTimeUpdate(WPARAM wParam, LPARAM lParam)
{
	CString szTime;
	CTime timeCur;
	timeCur = CTime::GetCurrentTime();
	szTime.Format(_T("%02u-%02u %02d:%02d:%02d"),timeCur.GetMonth(), timeCur.GetDay(), timeCur.GetHour(), timeCur.GetMinute(), timeCur.GetSecond() );

	//2017-07-20, jsjung, 설비 RUN->IDLE 변경시 IDLE TIME 시작
	theProcBank.IDLE_Info.szIDLEName = _T("IDLE");
	theProcBank.IDLE_Info.TimeOccur = CTime::GetCurrentTime();

	CGxUICtrl::SetStaticString(this, IDC_GXLBL_IDLE_TIME, szTime);
	return 0;
}

//2017-07-20, jsjung, 설비 IDLE->RUN 변경시 IDLE TIME 종료
LRESULT CFormMain::OnIDLEChangeRUNTimeUpdate(WPARAM wParam, LPARAM lParam)
{
	CString szTime;
	CTime timeCur;
	timeCur = CTime::GetCurrentTime();
	szTime =  _T(" ");

	theProcBank.IDLE_Info.TimeClear = CTime::GetCurrentTime();		
	theProcBank.IDLE_Info.SetTakeTime();
	theProcBank.IDLE_Info.WriteIDLEBuf();

	CGxUICtrl::SetStaticString(this, IDC_GXLBL_IDLE_TIME, szTime);

	return 0;
}
//2017-07-20, jsjung, 설비 RUN상태 TRAY IN IDLE TIME 시작
LRESULT CFormMain::OnAGVInStartTimeUpdate(WPARAM wParam, LPARAM lParam)
{
	CString szTime;
	CTime timeCur;
	timeCur = CTime::GetCurrentTime();
	szTime.Format(_T("%02u-%02u %02d:%02d:%02d"),timeCur.GetMonth(), timeCur.GetDay(), timeCur.GetHour(), timeCur.GetMinute(), timeCur.GetSecond() );
	
	theProcBank.AGVINIDLE_Info.szIDLEName = _T("AGV IN");
	theProcBank.AGVINIDLE_Info.TimeOccur = CTime::GetCurrentTime();
	//2018-03-12,MGYUN, AGV IN 시작 할때 타이머 시작 
	SetTimer(AGV_IN_DELAY_TIMER,1000,NULL);
	theProcBank.m_bTRAYInIDLE = TRUE;
	
	CGxUICtrl::SetStaticString(this, IDC_GXLBL_IDLE_TIME2, szTime);
	return 0;
}

//2017-07-20, jsjung, 설비 RUN상태 TRAY IN IDLE TIME 종료
LRESULT CFormMain::OnAGVInEndTimeUpdate(WPARAM wParam, LPARAM lParam)
{
	CString szTime;
	CTime timeCur;
	timeCur = CTime::GetCurrentTime();
	szTime =  _T(" ");

	theProcBank.AGVINIDLE_Info.TimeClear = CTime::GetCurrentTime();		
	theProcBank.AGVINIDLE_Info.SetTakeTime();
	theProcBank.AGVINIDLE_Info.WriteIDLEBuf();
	
	//2018-03-12,MGYUN, AGV IN 종료 할때 타이머 종료 및 시간 초기화
	
	KillTimer(AGV_IN_DELAY_TIMER);
	theProcBank.nAGV_InDelayTime = 0;
	theMonitorBank.WriteAGVInDelay(theProcBank.nAGV_InDelayTime);

	theProcBank.m_bTRAYInIDLE = FALSE;
	CGxUICtrl::SetStaticString(this, IDC_GXLBL_IDLE_TIME2, szTime);

	return 0;
}
//2017-07-20, jsjung, 설비 RUN상태 TRAY OUT IDLE TIME 시작
LRESULT CFormMain::OnAGVOutStartTimeUpdate(WPARAM wParam, LPARAM lParam)
{
	CString szTime;
	CTime timeCur;
	timeCur = CTime::GetCurrentTime();
	szTime.Format(_T("%02u-%02u %02d:%02d:%02d"),timeCur.GetMonth(), timeCur.GetDay(), timeCur.GetHour(), timeCur.GetMinute(), timeCur.GetSecond() );

	theProcBank.AGVOUTIDLE_Info.szIDLEName = _T("AGV OUT");
	theProcBank.AGVOUTIDLE_Info.TimeOccur = CTime::GetCurrentTime();
	
	//2018-03-12,MGYUN, AGV OUT 시작 할때 타이머 시작 및 시간
	SetTimer(AGV_OUT_DELAY_TIMER,1000,NULL);
	theProcBank.m_bTRAYOutIDLE = TRUE;

	CGxUICtrl::SetStaticString(this, IDC_GXLBL_IDLE_TIME3, szTime);
	return 0;
}

//2017-07-20, jsjung, 설비 RUN상태 TRAY OUT IDLE TIME 종료
LRESULT CFormMain::OnAGVOutEndTimeUpdate(WPARAM wParam, LPARAM lParam)
{
	CString szTime;
	CTime timeCur;
	timeCur = CTime::GetCurrentTime();
	szTime =  _T(" ");

	theProcBank.AGVOUTIDLE_Info.TimeClear = CTime::GetCurrentTime();		
	theProcBank.AGVOUTIDLE_Info.SetTakeTime();
	theProcBank.AGVOUTIDLE_Info.WriteIDLEBuf();
	
	//2018-03-12,MGYUN, AGV OUT 종료 할때 타이머종료 및 시간 초기화.
	KillTimer(AGV_OUT_DELAY_TIMER);
	theProcBank.nAGV_OutDelayTime = 0;
	theMonitorBank.WriteAGVOutDelay(theProcBank.nAGV_OutDelayTime);
	
	theProcBank.m_bTRAYOutIDLE = FALSE;

	CGxUICtrl::SetStaticString(this, IDC_GXLBL_IDLE_TIME3, szTime);

	return 0;
}


void CFormMain::ClickGxlblReportAutocontact()
{
	::PostMessage( ( (CMainFrame*)AfxGetMainWnd() )->m_hWnd, GxMSG_GUI_SHOW_DLG, (WPARAM)eDLG_Statement, (LPARAM)1);	
}


void CFormMain::ClickGxstcLoaderDoor5()
{
	//theCellBank.WriteTrackOutInfo(CELL_POS_UD_TR_CH1 );

	//long Data[1];
	//theDeviceMelsec.RobotWordRecvBuf(0x81,sizeof(long),Data);

	//long Data[8];
	//theDeviceMelsec.RobotWordRecvBuf(0x00,sizeof(long),Data);
	//theDeviceMelsec.RobotWordRecvBuf(0x10,sizeof(long)*8,Data);

	//BOOL aa = (BOOL)theDeviceMelsec.RobotBitRecv(0xA8);
	// aa = (BOOL)theDeviceMelsec.RobotBitRecv(0xA9);
	//theDeviceMelsec.RobotBitOff(0xA8);
	//theDeviceMelsec.RobotBitOff(0xA9);
	//theDeviceMelsec.RobotBitOn(0xE5);
	//theDeviceMelsec.RobotBitOff(0xE5);
	
/*	theConfigBank.m_System.ReadTMDJobFileInfo();							// 상위에서 보내온 Job file download관련 정보를 읽어온다.

	// 이후 시퀀스 진행 처리한다.
	::PostMessage( ( (CMainFrame*)AfxGetMainWnd() )->m_hWnd, UDMSG_TMDDOWNLOAD_UPDATE, 0, (LPARAM)1);	//2017-03-08, skcho, 상위에서 명령을 받으면 유저에게 PG 버튼을 깜빡여 알린다.
	theConfigBank.m_System.m_bJobStart = TRUE;
	*/
}


void CFormMain::ClickGxstcMainOlbskip()
{
	if(theProcBank.m_bOLBInterLockSkip) 
	{
		theProcBank.m_bOLBInterLockSkip = FALSE;
	}
	else
	{
		theProcBank.m_bOLBInterLockSkip = TRUE;
	}
}

void CFormMain::ClickGxlblMainLogclear()
{
	m_edtBoard.SetSel(0, -1);
	m_edtBoard.ReplaceSel(_T(""));
	m_edtBoard.ScrollWindow(0, 1000);

	m_edtBoard2.SetSel(0, -1);
	m_edtBoard2.ReplaceSel(_T(""));	

//CCellInfo *pCell =NULL;
	//theCellBank.CreateCellInfo(CELL_POS_BZONE_CH1);
	//pCell = theCellBank.GetCellInfo(CELL_POS_BZONE_CH1);
	//pCell->m_strCellID.Format(_T("%s"),_T("A3NE1R7BFFBBT120"));
	//pCell->m_strInnerID.Format(_T("%s"),_T("123456"));
	//pCell->m_JigId = JIG_ID_B;
	//pCell->m_JigCh = JIG_CH_1;
	//theProcBank.m_nVI_MITAlignResult = ALIGN_RESULT_GOOD;
	//pCell->m_Zone_B.m_nVI_AlignResult = ALIGN_RESULT_GOOD;
	//pCell->m_Zone_C.VIClass = GOOD_CELL;
	//pCell->m_Zone_C.CZoneDefect = _T("GOOD");
	//pCell->m_Zone_A.RegPinClass = GOOD_CELL;
	//pCell->m_Zone_A.CellLoadingClass = GOOD_CELL;
	//pCell->m_Zone_A.DicCheckClass = GOOD_CELL;
	//pCell->m_Zone_A.ContactCurrentClass = NONE_CELL;
	//theUnitFunc.B_Zone_SetContactCount(JIG_CH_1);
	//theUnitFunc.B_Zone_ContactChk(JIG_CH_1);
	/*theCellBank.CreateCellInfo(CELL_POS_AZONE_CH2);
	pCell = theCellBank.GetCellInfo(CELL_POS_AZONE_CH2);
	pCell->m_strCellID.Format(_T("%s"),_T("A3NE1R7DJGNDJF120"));
	pCell->m_strInnerID.Format(_T("%s"),_T("123456"));
	pCell->m_JigId = JIG_ID_A;
	pCell->m_JigCh = JIG_CH_2;
	theProcBank.m_nVI_MITAlignResult = ALIGN_RESULT_FAIL;
	pCell->m_Zone_B.m_nVI_AlignResult = ALIGN_RESULT_FAIL;
	pCell->m_Zone_C.VIClass = REJECT_CELL;
	pCell->m_Zone_C.CZoneDefect = _T("NG");*/
    /*
		theMonitorBank.WriteCellItemCH1(CELL_POS_UD_TR_CH1);
		theMonitorBank.WriteCellItemCH2(CELL_POS_UD_TR_CH2);
	*/

	/*CCellInfo *pCell =NULL;
    theCellBank.CreateCellInfo(CELL_POS_AZONE_CH1);
	pCell = theCellBank.GetCellInfo(CELL_POS_AZONE_CH1);
	pCell->m_strCellID.Format(_T("%s"),_T("A3NE1R7BFFBBT120"));

	theCellBank.CreateCellInfo(CELL_POS_BZONE_CH1);
	pCell = theCellBank.GetCellInfo(CELL_POS_BZONE_CH1);
	pCell->m_strCellID.Format(_T("%s"),_T("A3NE1R7BFFBBT121"));


	theCellBank.CreateCellInfo(CELL_POS_CZONE_CH1);
	pCell = theCellBank.GetCellInfo(CELL_POS_CZONE_CH1);
	pCell->m_strCellID.Format(_T("%s"),_T("A3NE1R7BFFBBT122"));

	theCellBank.CreateCellInfo(CELL_POS_DZONE_CH2);
	pCell = theCellBank.GetCellInfo(CELL_POS_DZONE_CH2);
	pCell->m_strCellID.Format(_T("%s"),_T("A3NE1R7BFFBBT123"));
	
	theDeviceMelsec.UpperWordSend(OPTICAL_JIG_A_CH1_CELLID,(LPVOID)pCell->m_strCellID.GetString(),40,TRUE);
	*/
	/*theDeviceMelsec.UpperWordSend(OPTICAL_JIG_A_CH1_CELLID,_T("A3NE1R7BFFBBT120"),16,TRUE);
	theDeviceMelsec.UpperWordSend(OPTICAL_JIG_A_CH2_CELLID,_T("A3NE1R7BFFBBT121"),16,TRUE);
	theDeviceMelsec.UpperWordSend(OPTICAL_JIG_B_CH1_CELLID,_T("A3NE1R7BFFBBT122"),16,TRUE);
	theDeviceMelsec.UpperWordSend(OPTICAL_JIG_B_CH2_CELLID,_T("A3NE1R7BFFBBT123"),16,TRUE);
	theDeviceMelsec.UpperWordSend(OPTICAL_JIG_C_CH1_CELLID,_T("A3NE1R7BFFBBT124"),16,TRUE);
	theDeviceMelsec.UpperWordSend(OPTICAL_JIG_C_CH2_CELLID,_T("A3NE1R7BFFBBT125"),16,TRUE);
	theDeviceMelsec.UpperWordSend(OPTICAL_JIG_D_CH1_CELLID,_T("A3NE1R7BFFBBT126"),16,TRUE);
	theDeviceMelsec.UpperWordSend(OPTICAL_JIG_D_CH2_CELLID,_T("A3NE1R7BFFBBT127"),16,TRUE);
	theDeviceMelsec.UpperWordSend(OPTICAL_ULDTR_CH1_CELLID,_T("A3NE1R7BFFBBT128"),16,TRUE);
	theDeviceMelsec.UpperWordSend(OPTICAL_ULDTR_CH2_CELLID,_T("A3NE1R7BFFBBT129"),16,TRUE);
	theDeviceMelsec.UpperWordSend(OPTICAL_ULD_ROBOT_CELLID,_T("A3NE1R7BFFBBT130"),16,TRUE);
	*/

	//theProcBank.m_nGoodInPortEmptyStep = 1;
	//	theProcBank.m_nNGInPortEmptyStep=1;	
	

}
void CFormMain::DispVacuumState()
{
	
	JIG_ID nJig =JIG_ID_A;
	INPUT_ID nINPUTID_CH1 =X_A_JIG_CH1_VAC_SEN , nINPUTID_CH2=X_A_JIG_CH2_VAC_SEN;
	UINT nCONTROLID_CH1=IDC_GXSTC_A_ZONE_VAC1, nCONTROLID_CH2=IDC_GXSTC_A_ZONE_VAC2;

	for(int i = 0; i < ZONE_ID_MAX; i++)
	{
	   nJig = (JIG_ID)(theUnitFunc.AMT_GetAutoJigID((ZONE_ID)i));
	   if(i == ZONE_ID_A)
	   {
		  nCONTROLID_CH1 =  IDC_GXSTC_A_ZONE_VAC1;
		  nCONTROLID_CH2 =  IDC_GXSTC_A_ZONE_VAC2;
	   }
	   else if(i == ZONE_ID_B)
	   {
		  nCONTROLID_CH1 =  IDC_GXSTC_B_ZONE_VAC1;
		  nCONTROLID_CH2 =  IDC_GXSTC_B_ZONE_VAC2;
	   }
	   else if(i == ZONE_ID_C)
	   {
		  nCONTROLID_CH1 =  IDC_GXSTC_C_ZONE_VAC1;
		  nCONTROLID_CH2 =  IDC_GXSTC_C_ZONE_VAC2;
	   }
	   else if(i == ZONE_ID_D)
	   {
		  nCONTROLID_CH1 =  IDC_GXSTC_D_ZONE_VAC1;
		  nCONTROLID_CH2 =  IDC_GXSTC_D_ZONE_VAC2;
	   }
	   if(nJig == JIG_ID_A)
	   {
          nINPUTID_CH1 = X_A_JIG_CH1_VAC_SEN;
		  nINPUTID_CH2 = X_A_JIG_CH2_VAC_SEN;		 
	   }
	   else if(nJig == JIG_ID_B)
	   {
		  nINPUTID_CH1 = X_B_JIG_CH1_VAC_SEN;
		  nINPUTID_CH2 = X_B_JIG_CH2_VAC_SEN;		
	   }
	   else if(nJig == JIG_ID_C)
	   {
		  nINPUTID_CH1 = X_C_JIG_CH1_VAC_SEN;
		  nINPUTID_CH2 = X_C_JIG_CH2_VAC_SEN;		
	   }
	   else if(nJig == JIG_ID_D)
	   {
		  nINPUTID_CH1 = X_D_JIG_CH1_VAC_SEN;
		  nINPUTID_CH2 = X_D_JIG_CH2_VAC_SEN;		
	   }

		if(theUnitFunc.GetInPutIOState(nINPUTID_CH1))
		{
			CGxUICtrl::SetStaticColor(this,nCONTROLID_CH1, GXCOLOR_LIGHT_BLUE);
		}
		else
		{
			CGxUICtrl::SetStaticColor(this,nCONTROLID_CH1, GXCOLOR_WHITE);
		}
		if(theUnitFunc.GetInPutIOState(nINPUTID_CH2))
		{
			CGxUICtrl::SetStaticColor(this,nCONTROLID_CH2, GXCOLOR_LIGHT_BLUE);
		}
		else
		{
			CGxUICtrl::SetStaticColor(this,nCONTROLID_CH2, GXCOLOR_WHITE);
		}	 

	}

	if(theUnitFunc.GetInPutIOState(X_LD_ROBOT_VAC_SEN))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_LD_ROBOT_VAC, GXCOLOR_LIGHT_BLUE);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_LD_ROBOT_VAC, GXCOLOR_WHITE);
	}

	if(theUnitFunc.GetInPutIOState(X_PA_TR_CH1_VAC_SEN))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_PRE_TR_VAC1, GXCOLOR_LIGHT_BLUE);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_PRE_TR_VAC1, GXCOLOR_WHITE);
	}

	if(theUnitFunc.GetInPutIOState(X_PA_TR_CH2_VAC_SEN))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_PRE_TR_VAC2, GXCOLOR_LIGHT_BLUE);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_PRE_TR_VAC2, GXCOLOR_WHITE);
	}

	if(theUnitFunc.GetInPutIOState(X_PA_STG_CH1_VAC_SEN))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_PRE_STAGE_VAC1, GXCOLOR_LIGHT_BLUE);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_PRE_STAGE_VAC1, GXCOLOR_WHITE);
	}

	if(theUnitFunc.GetInPutIOState(X_PA_STG_CH2_VAC_SEN))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_PRE_STAGE_VAC2, GXCOLOR_LIGHT_BLUE);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_PRE_STAGE_VAC2, GXCOLOR_WHITE);
	}

	if(theUnitFunc.GetInPutIOState(X_LD_UPPER_SHUTTLE_CH1_VAC_SEN))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UPPER_SHUTTLE_VAC1, GXCOLOR_LIGHT_BLUE);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UPPER_SHUTTLE_VAC1, GXCOLOR_WHITE);
	}

	if(theUnitFunc.GetInPutIOState(X_LD_UPPER_SHUTTLE_CH2_VAC_SEN))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UPPER_SHUTTLE_VAC2, GXCOLOR_LIGHT_BLUE);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UPPER_SHUTTLE_VAC2, GXCOLOR_WHITE);
	}

	if(theUnitFunc.GetInPutIOState(X_LD_LOWER_SHUTTLE_CH1_VAC_SEN))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_LOWER_SHUTTLE_VAC1, GXCOLOR_LIGHT_BLUE);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_LOWER_SHUTTLE_VAC1, GXCOLOR_WHITE);
	}

	if(theUnitFunc.GetInPutIOState(X_LD_LOWER_SHUTTLE_CH2_VAC_SEN))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_LOWER_SHUTTLE_VAC2, GXCOLOR_LIGHT_BLUE);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_LOWER_SHUTTLE_VAC2, GXCOLOR_WHITE);
	}

	if(theUnitFunc.GetInPutIOState(X_LD_TR_CH1_VAC_SEN))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_LD_TR_VAC1, GXCOLOR_LIGHT_BLUE);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_LD_TR_VAC1, GXCOLOR_WHITE);
	}

	if(theUnitFunc.GetInPutIOState(X_LD_TR_CH2_VAC_SEN))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_LD_TR_VAC2, GXCOLOR_LIGHT_BLUE);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_LD_TR_VAC2, GXCOLOR_WHITE);
	}

	if(theUnitFunc.GetInPutIOState(X_ULD_TR_CH1_VAC_SEN))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UD_TR_VAC1, GXCOLOR_LIGHT_BLUE);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UD_TR_VAC1, GXCOLOR_WHITE);
	}

	if(theUnitFunc.GetInPutIOState(X_ULD_TR_CH2_VAC_SEN))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UD_TR_VAC2, GXCOLOR_LIGHT_BLUE);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UD_TR_VAC2, GXCOLOR_WHITE);
	}

	if(theUnitFunc.GetInPutIOState(X_ULD_STAGE_CH1_VAC_SEN))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UD_STAGE_VAC1, GXCOLOR_LIGHT_BLUE);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UD_STAGE_VAC1, GXCOLOR_WHITE);
	}

	if(theUnitFunc.GetInPutIOState(X_ULD_STAGE_CH2_VAC_SEN))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UD_STAGE_VAC2, GXCOLOR_LIGHT_BLUE);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UD_STAGE_VAC2, GXCOLOR_WHITE);
	}

	if(theUnitFunc.GetInPutIOState(X_ULD_ROBOT_VAC_SEN))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UD_ROBOT_VAC, GXCOLOR_LIGHT_BLUE);
	}
	else
	{
	   CGxUICtrl::SetStaticColor(this,IDC_GXSTC_UD_ROBOT_VAC, GXCOLOR_WHITE);
	}

}


void CFormMain::ClickGxstcAjigSkip()
{
	/*CGxMsgBox	dlgMsgBox;

	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
	dlgMsgBox.SetMessage(TRUE, 
			_T("A JIG SKIP을 해제하시겠습니까?"),	//한
			_T("Do you want A JIG SKIP OFF"),		//영
			_T("Do you want A JIG SKIP OFF") );		//베

	if(dlgMsgBox.DoModal() == IDOK)
	{
       theConfigBank.m_Option.m_bUseASkip = FALSE;
	   theConfigBank.m_Option.m_strASkipDescription = _T("");		
	   theSocketInterFace.SendCmdUnitInterlockToDataPC(EQUIP_SET_UNIT_STATUS_CHANGE_INSPECTION,1);  //A JIG
	   theConfigBank.SaveOptionFile();
	}
	else
	{

	}
	*/
		
}


void CFormMain::ClickGxstcBjigSkip()
{
	/*CGxMsgBox	dlgMsgBox;

	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
	dlgMsgBox.SetMessage(TRUE, 
			_T("BJIG SKIP을 해제하시겠습니까?"),	//한
			_T("Do you want B JIG SKIP OFF"),		//영
			_T("Do you want B JIG SKIP OFF") );		//베

	if(dlgMsgBox.DoModal() == IDOK)
	{
       theConfigBank.m_Option.m_bUseBSkip = FALSE;
	   theConfigBank.m_Option.m_strBSkipDescription = _T("");	
	   theSocketInterFace.SendCmdUnitInterlockToDataPC(EQUIP_SET_UNIT_STATUS_CHANGE_INSPECTION,2); 
		theConfigBank.SaveOptionFile();
	}
	else
	{

	}
	*/
}


void CFormMain::ClickGxstcCjigSkip()
{
	/*CGxMsgBox	dlgMsgBox;

	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
	dlgMsgBox.SetMessage(TRUE, 
			_T("C JIG SKIP을 해제하시겠습니까?"),	//한
			_T("Do you want C JIG SKIP OFF"),		//영
			_T("Do you want C JIG SKIP OFF") );		//베

	if(dlgMsgBox.DoModal() == IDOK)
	{
       theConfigBank.m_Option.m_bUseCSkip = FALSE;
	   theConfigBank.m_Option.m_strCSkipDescription = _T("");
	   theSocketInterFace.SendCmdUnitInterlockToDataPC(EQUIP_SET_UNIT_STATUS_CHANGE_INSPECTION,3);  //C JIG
		theConfigBank.SaveOptionFile();	
	}
	else
	{

	}
	*/
}


void CFormMain::ClickGxstcDjigSkip()
{
	/*CGxMsgBox	dlgMsgBox;

	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
	dlgMsgBox.SetMessage(TRUE, 
			_T("D JIG SKIP을 해제하시겠습니까?"),	//한
			_T("Do you want D JIG SKIP OFF"),		//영
			_T("Do you want D JIG SKIP OFF") );		//베

	if(dlgMsgBox.DoModal() == IDOK)
	{
       theConfigBank.m_Option.m_bUseDSkip = FALSE;
	   theConfigBank.m_Option.m_strDSkipDescription = _T("");	
	   theSocketInterFace.SendCmdUnitInterlockToDataPC(EQUIP_SET_UNIT_STATUS_CHANGE_INSPECTION,4);  //D JIG
       theConfigBank.SaveOptionFile();
	}
	else
	{

	}
	*/
}
//2017-11-15,SKCHO, LOADER ROBOT에서 LD TR까지 셀 데이터와 VAC 비교
BOOL CFormMain::CheckCellDataAndVac(INPUT_ID nInput1,INPUT_ID nInput2 )
{

	CGxMsgBox	dlgMsgBox;

	//2017-11-27,SKCHO, 컨베어 데이터 로봇으로 자동 이동
	//LOADER ROBOT VAC 확인 후 CONV CELL정보 있고 실물 없으면 데이터 자동 이동
   	if((theUnitFunc.CellTagExist(CELL_POS_LD_CONV) && theUnitFunc.LDConvCellChk(CONV_SEN_3) == FALSE) 
		&& (!theUnitFunc.CellTagExist(CELL_POS_LDROBOT) && theUnitFunc.GetInPutIOState(X_LD_ROBOT_VAC_SEN)))
	{
		theUnitFunc.CellData_LDRobotFromLDConvIn();
	}
	

	//LOADER ROBOT CELL 정보 확인
    if((theUnitFunc.CellTagExist(CELL_POS_LDROBOT) && !theUnitFunc.GetInPutIOState(X_LD_ROBOT_VAC_SEN)) 
		|| (!theUnitFunc.CellTagExist(CELL_POS_LDROBOT) && theUnitFunc.GetInPutIOState(X_LD_ROBOT_VAC_SEN)))
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("LOADER ROBOT CELL 정보를 확인하여주십시오 ."),	//한
				_T("LOADER ROBOT CELL INFO Check Please."),		//영
				_T("LOADER ROBOT CELL INFO Check Please.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}

	//UPPER SHUTTLE CH1
	if((theUnitFunc.CellTagExist(CELL_POS_UPPER_SHUTTLE_CH1) && !theUnitFunc.GetInPutIOState(X_LD_UPPER_SHUTTLE_CH1_VAC_SEN)) 
		|| (!theUnitFunc.CellTagExist(CELL_POS_UPPER_SHUTTLE_CH1) && theUnitFunc.GetInPutIOState(X_LD_UPPER_SHUTTLE_CH1_VAC_SEN)))
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("UPPER SHUTTLE CH1 CELL 정보를 확인하여주십시오 ."),	//한
				_T("UPPER SHUTTLE CH1 CELL INFO Check Please."),		//영
				_T("UPPER SHUTTLE CH1 CELL INFO Check Please.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}

	//UPPER SHUTTLE CH2
	if((theUnitFunc.CellTagExist(CELL_POS_UPPER_SHUTTLE_CH2) && !theUnitFunc.GetInPutIOState(X_LD_UPPER_SHUTTLE_CH2_VAC_SEN)) 
		|| (!theUnitFunc.CellTagExist(CELL_POS_UPPER_SHUTTLE_CH2) && theUnitFunc.GetInPutIOState(X_LD_UPPER_SHUTTLE_CH2_VAC_SEN)))
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("UPPER SHUTTLE CH2 CELL 정보를 확인하여주십시오 ."),	//한
				_T("UPPER SHUTTLE CH2 CELL INFO Check Please."),		//영
				_T("UPPER SHUTTLE CH2 CELL INFO Check Please.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}
	//LOWER SHUTTLE CH1
	if((theUnitFunc.CellTagExist(CELL_POS_LOWER_SHUTTLE_CH1) && !theUnitFunc.GetInPutIOState(X_LD_LOWER_SHUTTLE_CH1_VAC_SEN)) 
		|| (!theUnitFunc.CellTagExist(CELL_POS_LOWER_SHUTTLE_CH1) && theUnitFunc.GetInPutIOState(X_LD_LOWER_SHUTTLE_CH1_VAC_SEN)))
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("LOWER SHUTTLE CH1 CELL 정보를 확인하여주십시오 ."),	//한
				_T("LOWER SHUTTLE CH1 CELL INFO Check Please."),		//영
				_T("LOWER SHUTTLE CH1 CELL INFO Check Please.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}

	//LOWER SHUTTLE CH2
	if((theUnitFunc.CellTagExist(CELL_POS_LOWER_SHUTTLE_CH2) && !theUnitFunc.GetInPutIOState(X_LD_LOWER_SHUTTLE_CH2_VAC_SEN)) 
		|| (!theUnitFunc.CellTagExist(CELL_POS_LOWER_SHUTTLE_CH2) && theUnitFunc.GetInPutIOState(X_LD_LOWER_SHUTTLE_CH2_VAC_SEN)))
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("LOWER SHUTTLE CH2 CELL 정보를 확인하여주십시오 ."),	//한
				_T("LOWER SHUTTLE CH2 CELL INFO Check Please."),		//영
				_T("LOWER SHUTTLE CH2 CELL INFO Check Please.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}
	//PREALIGN TR CH1
	if((theUnitFunc.CellTagExist(CELL_POS_PA_TR_CH1) && !theUnitFunc.GetInPutIOState(X_PA_TR_CH1_VAC_SEN)) 
		|| (!theUnitFunc.CellTagExist(CELL_POS_PA_TR_CH1) && theUnitFunc.GetInPutIOState(X_PA_TR_CH1_VAC_SEN)))
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("PREALIGN TR CH1 CELL 정보를 확인하여주십시오 ."),	//한
				_T("PREALIGN TR CH1 CELL INFO Check Please."),		//영
				_T("PREALIGN TR CH1 CELL INFO Check Please.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}
	//PREALIGN TR CH2
	if((theUnitFunc.CellTagExist(CELL_POS_PA_TR_CH2) && !theUnitFunc.GetInPutIOState(X_PA_TR_CH2_VAC_SEN)) 
		|| (!theUnitFunc.CellTagExist(CELL_POS_PA_TR_CH2) && theUnitFunc.GetInPutIOState(X_PA_TR_CH2_VAC_SEN)))
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("PREALIGN TR CH2 CELL 정보를 확인하여주십시오 ."),	//한
				_T("PREALIGN TR CH2 CELL INFO Check Please."),		//영
				_T("PREALIGN TR CH2 CELL INFO Check Please.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}
	//PREALIGN STAGE CH1
	if((theUnitFunc.CellTagExist(CELL_POS_PA_STAGE_CH1) && !theUnitFunc.GetInPutIOState(X_PA_STG_CH1_VAC_SEN)) 
		|| (!theUnitFunc.CellTagExist(CELL_POS_PA_STAGE_CH1) && theUnitFunc.GetInPutIOState(X_PA_STG_CH1_VAC_SEN)))
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("PREALIGN STAGE CH1 CELL 정보를 확인하여주십시오 ."),	//한
				_T("PREALIGN STAGE CH1 CELL INFO Check Please."),		//영
				_T("PREALIGN STAGE CH1 CELL INFO Check Please.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}
	//PREALIGN STAGE CH2
	if((theUnitFunc.CellTagExist(CELL_POS_PA_STAGE_CH2) && !theUnitFunc.GetInPutIOState(X_PA_STG_CH2_VAC_SEN)) 
		|| (!theUnitFunc.CellTagExist(CELL_POS_PA_STAGE_CH2) && theUnitFunc.GetInPutIOState(X_PA_STG_CH2_VAC_SEN)))
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("PREALIGN STAGE CH2 CELL 정보를 확인하여주십시오 ."),	//한
				_T("PREALIGN STAGE CH2 CELL INFO Check Please."),		//영
				_T("PREALIGN STAGE CH2 CELL INFO Check Please.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}


	//LOADER TR CH1
	if((theUnitFunc.CellTagExist(CELL_POS_LD_TR_CH1) && !theUnitFunc.GetInPutIOState(X_LD_TR_CH1_VAC_SEN)) 
		|| (!theUnitFunc.CellTagExist(CELL_POS_LD_TR_CH1) && theUnitFunc.GetInPutIOState(X_LD_TR_CH1_VAC_SEN)))
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("LOADER TR CH1 CELL 정보를 확인하여주십시오 ."),	//한
				_T("LOADER TR CH1 CELL INFO Check Please."),		//영
				_T("LOADER TR CH1 CELL INFO Check Please.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}
	//LOADER TR CH2
	if((theUnitFunc.CellTagExist(CELL_POS_LD_TR_CH2) && !theUnitFunc.GetInPutIOState(X_LD_TR_CH2_VAC_SEN)) 
		|| (!theUnitFunc.CellTagExist(CELL_POS_LD_TR_CH2) && theUnitFunc.GetInPutIOState(X_LD_TR_CH2_VAC_SEN)))
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("LOADER TR CH2 CELL 정보를 확인하여주십시오 ."),	//한
				_T("LOADER TR CH2 CELL INFO Check Please."),		//영
				_T("LOADER TR CH2 CELL INFO Check Please.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}




	//DZONE과 ULD TR CELL 확인
	if(theUnitFunc.CellTagExist(CELL_POS_DZONE_CH1) && !theUnitFunc.CellTagExist(CELL_POS_UD_TR_CH1) //2017-10-13 JSJUNG D존 베큠 에러 예외처리
		&& theUnitFunc.GetInPutIOState(X_ULD_TR_CH1_VAC_SEN) && !theUnitFunc.GetInPutIOState(nInput1))//!theUnitFunc.GetInPutIOState(X_D_JIG_CH1_VAC_SEN))
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("UNLOAD TR CELL과 D ZONE 정보를 확인하여주십시오 ."),	//한
				_T("UNLOAD TR CELL & D ZONE INFO Check Please."),		//영
				_T("UNLOAD TR CELL & D ZONE INFO Check Please.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}

	if(theUnitFunc.CellTagExist(CELL_POS_DZONE_CH2) && !theUnitFunc.CellTagExist(CELL_POS_UD_TR_CH2) //2017-10-13 JSJUNG D존 베큠 에러 예외처리
		&& theUnitFunc.GetInPutIOState(X_ULD_TR_CH2_VAC_SEN) &&  !theUnitFunc.GetInPutIOState(nInput2))//!theUnitFunc.GetInPutIOState(X_D_JIG_CH2_VAC_SEN))
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("UNLOAD TR CELL과 D ZONE 정보를 확인하여주십시오 ."),	//한
				_T("UNLOAD TR CELL & D ZONE INFO Check Please."),		//영
				_T("UNLOAD TR CELL & D ZONE INFO Check Please.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}
	//LOADER ROBOT CELL 정보 확인
    if((theUnitFunc.CellTagExist(CELL_POS_UD_ROBOT) && !theUnitFunc.GetInPutIOState(X_ULD_ROBOT_VAC_SEN)) 
		|| (!theUnitFunc.CellTagExist(CELL_POS_UD_ROBOT) && theUnitFunc.GetInPutIOState(X_ULD_ROBOT_VAC_SEN)))
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("UNLOADER ROBOT CELL 정보를 확인하여주십시오 ."),	//한
				_T("UNLOADER ROBOT CELL INFO Check Please."),		//영
				_T("UNLOADER ROBOT CELL INFO Check Please.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}

	return TRUE;
}


void CFormMain::ClickGxlblMainRetryinfo()
{
	::PostMessage( ( (CMainFrame*)AfxGetMainWnd() )->m_hWnd, GxMSG_GUI_SHOW_DLG, (WPARAM)eDLG_RETRYINFO, (LPARAM)1);	
}

//2017-12-18,SKCHO, 추가
void CFormMain::LDRobotOrg()
{
	int write = 0;
	switch(m_nLDRobotOrgSeq)
	{
	case seqLdRobot_idle:
		break;
	case seqLdRobot_1:
		if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_RUNNING_Y_ERRRESET))
		{
			theDeviceMelsec.RobotBitOff(LDROBOT_X_PLAYMODE_Y_EXTSTOP);
				
			//20170205 byskcho
			//알람이 발생하여 로봇이 언로딩위치 있을때 로봇을 원점기동하면 로봇이 다시 언로딩위치로 가는 버그 수정
			write = 0;
			theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);

		}
		m_nLDRobotOrgSeq++;
		m_Timer.Start();
		break;
	case seqLdRobot_2:
		theDeviceMelsec.RobotBitOn(LDROBOT_X_PGMEND_Y_PAUSE);
		m_nLDRobotOrgSeq++;
		m_Timer.Start();
		break;
	case seqLdRobot_3:
		if (m_Timer.Stop(FALSE) > 0.2)
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_X_PLAYMODE_Y_EXTSTOP);
			m_nLDRobotOrgSeq++;
			m_Timer.Start();
		}
		break;
	case seqLdRobot_4:
		if (m_Timer.Stop(FALSE) > 0.2)
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_X_STOPPING_Y_EXTMOTORON);
			m_nLDRobotOrgSeq++;
			m_Timer.Start();
		}
		break;
	case seqLdRobot_5:
		if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MOTORON_Y_LOWSPEED))
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_X_ALARM_Y_EXTRESET);
			m_nLDRobotOrgSeq++;
			m_Timer.Start();
		}
		if (m_Timer.Stop(FALSE) > 3)
		{
			theUnitFunc.LDRobotBitAllOff(E_ORG);
			m_nLDRobotOrgSeq = seqLdRobot_idle;
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("운전 준비가 ON 되지 않았습니다."),
				_T("Motor energized Not On"),		
				_T("Motor energized Not On"));

			dlgMsgBox.DoModal();
			break;
		}
		break;
	case seqLdRobot_6:
		if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_EXTRESET_Y_PGMSEL2))
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_X_PGMACK_Y_PGMSEL1);
			m_nLDRobotOrgSeq++;
			m_Timer.Start();
		}
		else if (m_Timer.Stop(FALSE) > 1)
		{
			theUnitFunc.LDRobotBitAllOff(E_ORG);
			m_nLDRobotOrgSeq = seqLdRobot_idle;
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("외부리셋이 ON 되지 않았습니다."),
				_T("EXT Reset Not On"),		
				_T("EXT Reset Not On"));

			dlgMsgBox.DoModal();
			break;
		}
		break;
	case seqLdRobot_7:
		if (m_Timer.Stop(FALSE) > 0.2)
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_X_HOMEPOS2_Y_PGMSTROBE);
			m_nLDRobotOrgSeq++;
			m_Timer.Start();
		}
		break;
	case seqLdRobot_8:
		if (m_Timer.Stop(FALSE) > 0.2)
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_X_TEACHMODE_Y_EXTSTART);
			m_nLDRobotOrgSeq++;
			m_Timer.Start();
		}
		break;
	case seqLdRobot_9:
		if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_RUNNING_Y_ERRRESET))
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_X_BATWARN_Y_ORG);
			m_nLDRobotOrgSeq++;
			m_Timer.Start();
		}
		else if (m_Timer.Stop(FALSE) > 10.)
		{	
			theUnitFunc.LDRobotBitAllOff(E_ORG);
			m_nLDRobotOrgSeq = seqLdRobot_idle;
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("로봇 러닝이 ON되지 않았습니다."),
				_T("Robot Running U1 Not On"),		
				_T("Robot Running U1 Not On"));

			dlgMsgBox.DoModal();
			break;
		}
		break;
	case seqLdRobot_10:
		if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_HOMEPOS1_Y_CYCLESTOP))
		{
			theDeviceMelsec.RobotBitOff(LDROBOT_X_BATWARN_Y_ORG);
			
			m_nLDRobotOrgSeq = seqLdRobot_idle;
			theUnitFunc.LDRobotBitAllOff(E_ORG);

			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("완료"), _T("Finish"), _T("Finish"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("로봇 움직임 완료"),
				_T("Robot Running Finish"),		
				_T("Robot Running Finish"));

			dlgMsgBox.DoModal();
			break;
		}
		else if (m_Timer.Stop(FALSE) > 10.)
		{	
			theUnitFunc.LDRobotBitAllOff(E_ORG);
			m_nLDRobotOrgSeq = seqLdRobot_idle;
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("로봇 러닝이 ON되지 않았습니다."),
				_T("Robot Running U1 Not On"),		
				_T("Robot Running U1 Not On"));

			dlgMsgBox.DoModal();
			break;
		}
		break;

	default:
		break;
	}
}

void CFormMain::ClickGxstcGoodtray()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CFormMain::ClickGxstcNgtray()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

//2018-03-15,NAM, to set for previous day( for week) 
void CFormMain::SetDateTime1(int iValue, BOOL btoday)		//	PREV나 NEXT 클릭시 날짜를 바꿔줌
{
	SYSTEMTIME today1;
	GetLocalTime(&today1);
	if(btoday)
	{
		GetLocalTime(&theProcBank.m_CurDate1);
	}
	CTime Compare1(theProcBank.m_CurDate1);
	CTime Compare2(today1);
	
	if(btoday)
	{
		if ( today1.wHour < 7 )
		{
			iValue = -1;
		}
	}
	else
	{
		if ( today1.wHour < 7 )
		{
			Compare2 += CTimeSpan(-1,0,0,0);
		}

		Compare1 += (CTimeSpan(iValue, 0, 0, 0));

		if ( Compare1 > Compare2 )
			return;
	}	

	CTime CalcTime(theProcBank.m_CurDate1);
	CalcTime += CTimeSpan(iValue,0,0,0);
	theProcBank.m_CurDate1.wYear = (WORD)CalcTime.GetYear();
	theProcBank.m_CurDate1.wMonth = (WORD)CalcTime.GetMonth();
	theProcBank.m_CurDate1.wDay = (WORD)CalcTime.GetDay();
			
	theProcBank.m_szCurrent_time1.Format(_T("%04u-%02u-%02u"),theProcBank.m_CurDate1.wYear, theProcBank.m_CurDate1.wMonth, theProcBank.m_CurDate1.wDay);
}
//2018-03-15,NAM, (to set for previous day( for month) 
void CFormMain::SetDateTime2(int iValue, BOOL btoday)		//	PREV나 NEXT 클릭시 날짜를 바꿔줌
{
	SYSTEMTIME today2;
	GetLocalTime(&today2);
	if(btoday)
	{
		GetLocalTime(&theProcBank.m_CurDate2);
	}
	CTime Compare1(theProcBank.m_CurDate2);
	CTime Compare2(today2);
	
	if(btoday)
	{
		if ( today2.wHour < 7 )
		{
			iValue = -1;
		}
	}
	else
	{
		if ( today2.wHour < 7 )
		{
			Compare2 += CTimeSpan(-1,0,0,0);
		}

		Compare1 += (CTimeSpan(iValue, 0, 0, 0));

		if ( Compare1 > Compare2 )
			return;
	}	

	CTime CalcTime(theProcBank.m_CurDate2);
	CalcTime += CTimeSpan(iValue,0,0,0);
	theProcBank.m_CurDate2.wYear = (WORD)CalcTime.GetYear();
	theProcBank.m_CurDate2.wMonth = (WORD)CalcTime.GetMonth();
	theProcBank.m_CurDate2.wDay = (WORD)CalcTime.GetDay();
			
	theProcBank.m_szCurrent_time2.Format(_T("%04u-%02u-%02u"),theProcBank.m_CurDate2.wYear, theProcBank.m_CurDate2.wMonth, theProcBank.m_CurDate2.wDay);
}

//2018-03-15,NAM, To get day contact percentage and Write to Monitoring device. 
void CFormMain::UPdateDayContactValue()
{
	theProductBank.ProductDayCall();
	ProductInfo tInfo1;
	tInfo1 = theProductBank.GetDataWholeDay();
	
	CString szPerContact = _T("(") + theProductBank.CalcPercent(tInfo1.tCount.iValue, tInfo1.tDefCont.iValue) + _T(")");
	theMonitorBank.WriteDayContact(szPerContact);

	// 2018-03-16, NAM, Write the Function to Monitoring Device for live production data
	theMonitorBank.WriteDayProduct(tInfo1.tCount.iValue);
}

//2018-03-15,NAM, to get contact percentage 1 hour ago and write to monitoring device.
void CFormMain::UPdatePreContactValue()
{
	theProductBank.ProductDayCall1();
	ProductInfo tInfo1;
	tInfo1 = theProductBank.GetDataWholeDay();

	CString szPerContact = _T("(") + theProductBank.CalcPercent(tInfo1.tCount.iValue, tInfo1.tDefCont.iValue) + _T(")");
	theMonitorBank.WritePreContact(szPerContact);
	
	//2018-03-16,NAM, Writing the Function to Monitoing Device for 1 hours for production data
	theMonitorBank.WritePreProduct(tInfo1.tCount.iValue);
}

///2018-03-15,NAM, to take value of contact percentage each day in a week  
double CFormMain::UPdateWeekContactValue()
{
	theProductBank.ProductDayCall();
	ProductInfo tInfo1;
	tInfo1 = theProductBank.GetDataWholeDay();

	return theProductBank.CalcPercent1(tInfo1.tCount.iValue, tInfo1.tDefCont.iValue);
}

//2018-03-15,NAM, to take value of contact percentage each day in a month
double CFormMain::UPdateMonthContactValue()
{
	theProductBank.ProductDayCall();
	ProductInfo tInfo1;
	tInfo1 = theProductBank.GetDataWholeDay();

	return theProductBank.CalcPercent1(tInfo1.tCount.iValue, tInfo1.tDefCont.iValue);
}

//2018-03-15,NAM, calculate percentage of week and write to monitoring device
void CFormMain::UPdateWeekContact()
{
	theProcBank.InitTime1();
	theProductBank.LoadReportFile1(theProcBank.m_szCurrent_time1.GetBuffer());
	UPdateDayContactValue();
	double AvePerWeek=0;
	int AveQuaPerWeek=0;
	ProductInfo tInfo2;
	
	int  todayofw;
	int dayow=Todayofweek(theProcBank.m_CurDate1.wDay,theProcBank.m_CurDate1.wMonth,theProcBank.m_CurDate1.wYear);
	
	
		if (dayow!=0) 
		   todayofw=dayow;
		else todayofw=7;

		for (int i=1;i<=todayofw; i++)
			{
			theProductBank.LoadReportFile1(theProcBank.m_szCurrent_time1.GetBuffer());
			AvePerWeek=AvePerWeek+UPdateWeekContactValue();

			//2018-03-16,NAM To Sum up Quantity each day in a Week. 
			tInfo2 = theProductBank.GetDataWholeDay();
			AveQuaPerWeek=AveQuaPerWeek+tInfo2.tCount.iValue;

			if (i==todayofw) 
			{
				CString szPer;
				AvePerWeek=AvePerWeek/i;
				szPer.Format(_T("%.2f"), AvePerWeek*100.);
				szPer += _T("%");
				theMonitorBank.WriteWeekContact(szPer);

				// 2018-03-16,NAM To calculate the average quantity for 1 week (Starting from Monday to present)
				theMonitorBank.WriteWeekProduct(AveQuaPerWeek);

			}
			SetDateTime1(-1);
			}
	
}
//2018-03-15,NAM, calculate percentage of month and write to monitoring device
void CFormMain::UPdateMonthContact()
{
	theProcBank.InitTime2();
	double AvePerMonth=0;
	int dayom=theProcBank.m_CurDate2.wDay;
	
	//2018-03-16,NAM, To define the Producti Infor
	ProductInfo tInfo2;
	int AveQuaPerMonth=0; 

	for (int i=1;i<=dayom; i++)
			{
			theProductBank.LoadReportFile1(theProcBank.m_szCurrent_time2.GetBuffer());
			AvePerMonth=AvePerMonth+UPdateMonthContactValue();

			//2018-03-16,NAM To Sum up Quantity each day in a Month(Starting from 1st day of month to present) 
			tInfo2 = theProductBank.GetDataWholeDay();
			AveQuaPerMonth=AveQuaPerMonth+tInfo2.tCount.iValue;
			if (i==dayom) 
			{
				CString szPer;
				AvePerMonth=AvePerMonth/i;
				szPer.Format(_T("%.2f"), AvePerMonth*100.);
				szPer += _T("%");
				theMonitorBank.WriteMonthContact(szPer);

				// 2018-03-16,NAM To calculate the average quantity for 1 Month (Starting from Monday to present)
				theMonitorBank.WriteMonthProduct(AveQuaPerMonth);

			}
			SetDateTime2(-1);
			}
	
}
//2018-03-15,NAM, calculate percentage of 1 hour before and write to monitoring device
void CFormMain::UpdatePrehourContact()
{
	theProcBank.InitTime3();
	theProductBank.LoadReportFile1(theProcBank.m_szCurrent_time3.GetBuffer());
	UPdatePreContactValue();
}


//2018-03-15,NAM, return day of week 
int CFormMain::Todayofweek(int day, int month, int year)
{
	int arr[]={0,3,2,5,3,5,1,4,6,2,4};
	if (month<3)
	{
		year--;
	}
	return (((year + year/4- year /100+year/400)+arr[month-1]+day)%7);
}

//2018-03-16,MGYUN, 런 시작 시 모터 서보 체크 함수.
BOOL CFormMain::MotorServoCheck()
{
	CGxMsgBox	dlgMsgBox;

	//2018-05-07,NAM, All Servo Motors checked when click start button
		AxisStatus AXIS_SVO_LOAD_CONV_status = theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_LOAD_CONV) ;
	if(AXIS_SVO_LOAD_CONV_status.bServoOn == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("LOAD CONVEYOR를 확인해 주세요."),	//한
				_T("Please! LOAD CONVEYOR MOTOR CHECK."),		//영
				_T("Please! LOAD CONVEYOR MOTOR CHECK.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}


		AxisStatus AXIS_SVO_LD_UPPERSHUTTLE_Y_status = theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_LD_UPPERSHUTTLE_Y) ;
	if(AXIS_SVO_LD_UPPERSHUTTLE_Y_status.bServoOn == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("LD UPPER SHUTTLE를 확인해 주세요."),	//한
				_T("Please! LD UPPER SHUTTLE MOTOR CHECK."),		//영
				_T("Please! LD UPPER SHUTTLE MOTOR CHECK.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}


		AxisStatus AXIS_SVO_LD_LOWERSHUTTLE_Y_status = theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_LD_LOWERSHUTTLE_Y) ;
	if(AXIS_SVO_LD_LOWERSHUTTLE_Y_status.bServoOn == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("LD LOWER SHUTTLE를 확인해 주세요."),	//한
				_T("Please! LD LOWER SHUTTLE MOTOR CHECK."),		//영
				_T("Please! LD LOWER SHUTTLE MOTOR CHECK.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}

		AxisStatus AXIS_SVO_PREALIGN_TR_Y_status = theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_PREALIGN_TR_Y) ;
	if(AXIS_SVO_PREALIGN_TR_Y_status.bServoOn == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("PREALIGN TR Y를 확인해 주세요."),	//한
				_T("Please! PREALIGN TR Y CHECK."),		//영
				_T("Please! PREALIGN TR Y CHECK.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}

		AxisStatus AXIS_SVO_PREALIGN_TR_Z_status = theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_PREALIGN_TR_Z) ;
	if(AXIS_SVO_PREALIGN_TR_Z_status.bServoOn == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("PRE ALIGN TR Z를 확인해 주세요."),	//한
				_T("Please! PRE ALIGN TR Z CHECK."),		//영
				_T("Please! PRE ALIGN TR Z CHECK.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}

		AxisStatus AXIS_SVO_PA_STAGE_X_status = theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_PA_STAGE_X) ;
	if(AXIS_SVO_PA_STAGE_X_status.bServoOn == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("PREAGLIGN STAGE를 확인해 주세요."),	//한
				_T("Please! PREAGLIGN STAGE MOTOR CHECK."),		//영
				_T("Please! PREAGLIGN STAGE MOTOR CHECK.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}

		AxisStatus AXIS_SVO_LD_TR_X_status = theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_LD_TR_X) ;
	if(AXIS_SVO_LD_TR_X_status.bServoOn == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("LD TR X를 확인해 주세요."),	//한
				_T("Please! LD TR X MOTOR CHECK."),		//영
				_T("Please! LD TR X MOTOR CHECK.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}

		AxisStatus AXIS_SVO_LD_TR_Z_status = theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_LD_TR_Z) ;
	if(AXIS_SVO_LD_TR_Z_status.bServoOn == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("LD TR Z를 확인해 주세요."),	//한
				_T("Please! LD TR Z MOTOR CHECK."),		//영
				_T("Please! LD TR Z MOTOR CHECK.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}

		AxisStatus AXIS_SVO_ALIGN_CAM_Y_status = theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_ALIGN_CAM_Y) ;
	if(AXIS_SVO_ALIGN_CAM_Y_status.bServoOn == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("ALGIN CAM Y를 확인해 주세요."),	//한
				_T("Please! ALGIN CAM Y MOTOR CHECK."),		//영
				_T("Please! ALGIN CAM Y MOTOR CHECK.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}


		AxisStatus AXIS_SVO_INS_X_status = theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_INS_X) ;
	if(AXIS_SVO_INS_X_status.bServoOn == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("INS  X MOTOR를 확인해 주세요."),	//한
				_T("Please! INS  X MOTOR CHECK."),		//영
				_T("Please! INS  X MOTOR CHECK.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}

		AxisStatus AXIS_SVO_INS_CH1_Z_status = theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_INS_CH1_Z) ;
	if(AXIS_SVO_INS_CH1_Z_status.bServoOn == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("INS CH1 Z를 확인해 주세요."),	//한
				_T("Please! INS CH1 Z MOTOR CHECK."),		//영
				_T("Please! INS CH1 Z MOTOR CHECK.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}

		AxisStatus AXIS_SVO_INS_CH2_Z_status = theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_INS_CH2_Z) ;
	if(AXIS_SVO_INS_CH2_Z_status.bServoOn == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("INS CH2 Z MOTOR 를 확인해 주세요."),	//한
				_T("Please! INS CH2 Z MOTOR CHECK."),		//영
				_T("Please! INS CH2 Z MOTOR CHECK.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}

		AxisStatus AXIS_SVO_ULD_TR_X_status = theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_ULD_TR_X) ;
	if(AXIS_SVO_ULD_TR_X_status.bServoOn == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("ULD TR X를 확인해 주세요."),	//한
				_T("Please! ULD TR X  MOTOR CHECK."),		//영
				_T("Please! ULD TR X MOTOR CHECK.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}

		AxisStatus AXIS_SVO_ULD_TR_Z_status = theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_ULD_TR_Z) ;
	if(AXIS_SVO_ULD_TR_Z_status.bServoOn == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("ULD TR Z를 확인해 주세요."),	//한
				_T("Please! ULD TR Z MOTOR CHECK."),		//영
				_T("Please! ULD TR Z MOTOR CHECK.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}


	//2017-11-06,MGYUN, GOOD OUT LIFT 서보 온 체크
	AxisStatus GoodOutLift_status = theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_GOOD_TRAY_OUT_LIFT_Z) ;
	if(GoodOutLift_status.bServoOn == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("OK OUT LIFT를 확인해 주세요."),	//한
				_T("Please! OK OUT LIFT CHECK."),		//영
				_T("Please! OK OUT LIFT CHECK.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}

	//2017-11-06,MGYUN, GOOD IN LIFT 서보 온 체크
	AxisStatus GoodInLift_status = theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_GOOD_TRAY_IN_LIFT_Z) ;
	if(GoodInLift_status.bServoOn == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("OK IN LIFT를 확인해 주세요."),	//한
				_T("Please! OK IN LIFT CHECK."),		//영
				_T("Please! OK IN LIFT CHECK.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}

	
	AxisStatus AXIS_SVO_GOOD_TRAY_TR_Y_status = theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_GOOD_TRAY_TR_Y) ;
	if(AXIS_SVO_GOOD_TRAY_TR_Y_status.bServoOn == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("GOOD TRAY TR Y를 확인해 주세요."),	//한
				_T("Please! GOOD TRAY TR Y MOTOR CHECK."),		//영
				_T("Please! GOOD TRAY TR Y MOTOR CHECK.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}

	
	AxisStatus AXIS_SVO_GOOD_TRAY_TR_Z_status = theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_GOOD_TRAY_TR_Z) ;
	if(AXIS_SVO_GOOD_TRAY_TR_Z_status.bServoOn == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("GOOD TRAY TR Z를 확인해 주세요."),	//한
				_T("Please! GOOD TRAY TR Z MOTOR CHECK."),		//영
				_T("Please! GOOD TRAY TR Z MOTOR CHECK.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}

	//2018-1-20,MGYUN, NG OUT LIFT 서보 온 체크 추가
	AxisStatus NGOutLift_status = theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_NG_TRAY_OUT_LIFT_Z) ;
	if(NGOutLift_status.bServoOn == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("NG OUT LIFT를 확인해 주세요."),	//한
				_T("Please! NG OUT LIFT CHECK."),		//영
				_T("Please! NG OUT LIFT CHECK.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}

	//2017-11-06,MGYUN, NG IN LIFT 서보 온 체크
	AxisStatus NGInLift_status = theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_NG_TRAY_IN_LIFT_Z) ;
	if(NGInLift_status.bServoOn == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("NG IN LIFT를 확인해 주세요."),	//한
				_T("Please! NG IN LIFT CHECK."),		//영
				_T("Please! NG IN LIFT CHECK.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}

	AxisStatus AXIS_SVO_NG_TRAY_TR_X_status = theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_NG_TRAY_TR_X) ;
	if(AXIS_SVO_NG_TRAY_TR_X_status.bServoOn == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("NG TRAY TR X를 확인해 주세요."),	//한
				_T("Please! NG TRAY TR X CHECK."),		//영
				_T("Please! NG TRAY TR X CHECK.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}

	AxisStatus AXIS_SVO_NG_TRAY_TR_Z_status = theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_NG_TRAY_TR_Z) ;
	if(AXIS_SVO_NG_TRAY_TR_Z_status.bServoOn == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("NG TRAY TR Z를 확인해 주세요."),	//한
				_T("Please! NG TRAY TR Z CHECK."),		//영
				_T("Please! NG TRAY TR Z CHECK.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}

	AxisStatus AXIS_STP_PA_CH1_X_status = theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, AXIS_STP_PA_CH1_X) ;
	if(AXIS_STP_PA_CH1_X_status.bServoOn == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("PA CH1 X MOTOR를 확인해 주세요."),	//한
				_T("Please! PA CH1 MOTOR X CHECK."),		//영
				_T("Please! PA CH1 MOTOR X CHECK.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}

	AxisStatus AXIS_STP_PA_CH2_X_status = theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, AXIS_STP_PA_CH2_X) ;
	if(AXIS_STP_PA_CH2_X_status.bServoOn == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("PA CH2 X MOTOR를 확인해 주세요."),	//한
				_T("Please! PA CH2 X MOTOR CHECK."),		//영
				_T("Please! PA CH2 X MOTOR CHECK.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}

	AxisStatus AXIS_STP_PA_CH1_Y_status = theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, AXIS_STP_PA_CH1_Y) ;
	if(AXIS_STP_PA_CH1_Y_status.bServoOn == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("PA CH1 Y MOTOR를 확인해 주세요."),	//한
				_T("Please! PA CH1 Y MOTOR CHECK."),		//영
				_T("Please! PA CH1 Y MOTOR CHECK.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}

	AxisStatus AXIS_STP_PA_CH2_Y_status = theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, AXIS_STP_PA_CH2_Y) ;
	if(AXIS_STP_PA_CH2_Y_status.bServoOn == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("PA CH2 Y MOTOR를 확인해 주세요."),	//한
				_T("Please! PA CH2 Y MOTOR CHECK."),		//영
				_T("Please! PA CH2 Y MOTOR CHECK.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}

	AxisStatus AXIS_STP_PA_CH1_T_status = theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, AXIS_STP_PA_CH1_T) ;
	if(AXIS_STP_PA_CH1_T_status.bServoOn == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("PA CH1 T MOTOR를 확인해 주세요."),	//한
				_T("Please! PA CH1 T MOTOR CHECK."),		//영
				_T("Please! PA CH1 T MOTOR CHECK.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}

	AxisStatus AXIS_STP_PA_CH2_T_status = theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, AXIS_STP_PA_CH2_T) ;
	if(AXIS_STP_PA_CH2_T_status.bServoOn == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("PA CH2 T MOTOR를 확인해 주세요."),	//한
				_T("Please! PA CH2 T MOTOR CHECK."),		//영
				_T("Please! PA CH2 T MOTOR CHECK.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}

	//2017-11-06,MGYUN, TurnTable 서보 온 체크
	AxisStatus TurnTable_status = theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_TURN_TABLE_T) ;
	if(TurnTable_status.bServoOn == TRUE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("TURN TABLE을 확인해 주세요."),	//한
				_T("Please! TURN TABLE CHECK."),		//영
				_T("Please! TURN TABLE CHECK.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}

	AxisStatus AXIS_STP_ALIGN_CAM_X_status = theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, AXIS_STP_ALIGN_CAM_X) ;
	if(AXIS_STP_ALIGN_CAM_X_status.bServoOn == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("ALIGN CAM X MOTOR을 확인해 주세요."),	//한
				_T("Please! ALIGN CAM X MOTOR CHECK."),		//영
				_T("Please! ALIGN CAM X MOTOR CHECK.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}

	AxisStatus AXIS_STP_INS_CH1_T_status = theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, AXIS_STP_INS_CH1_T) ;
	if(AXIS_STP_INS_CH1_T_status.bServoOn == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("INS CH1 T MOTOR을 확인해 주세요."),	//한
				_T("Please! INS CH1 T MOTOR CHECK."),		//영
				_T("Please! INS CH1 T MOTOR CHECK.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}

	AxisStatus AXIS_STP_INS_CH2_T_status = theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, AXIS_STP_INS_CH2_T) ;
	if(AXIS_STP_INS_CH2_T_status.bServoOn == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("INS CH2 T MOTOR을 확인해 주세요."),	//한
				_T("Please! INS CH2 T MOTOR CHECK."),		//영
				_T("Please! INS CH2 T MOTOR CHECK.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}

	AxisStatus AXIS_STP_INS_CH1_F_status = theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, AXIS_STP_INS_CH1_F) ;
	if(AXIS_STP_INS_CH1_F_status.bServoOn == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("INS CH1 F MOTOR을 확인해 주세요."),	//한
				_T("Please! INS CH1 F MOTOR CHECK."),		//영
				_T("Please! INS CH1 F MOTOR CHECK.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}

	AxisStatus AXIS_STP_INS_CH2_F_status = theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, AXIS_STP_INS_CH2_F) ;
	if(AXIS_STP_INS_CH2_F_status.bServoOn == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("INS CH2 F MOTOR을 확인해 주세요."),	//한
				_T("Please! INS CH2 F MOTOR CHECK."),		//영
				_T("Please! INS CH2 F MOTOR CHECK.") );		//베

		dlgMsgBox.DoModal();
		return FALSE;
	}

	return TRUE;
}

//2018-05-10,MGYUN, 하이브리드 작업 관련 I/O 
void CFormMain::ClickGxstcHybridRobotReady()
{
	// I/O 가 이미 ON이면 OFF
	if( theUnitFunc.GetOutPutIOCheck(Y_HYBRID_ROBOT_READY) == TRUE)
	{
		theUnitFunc.SendHybRobotReady(OFF);
	}
	else
		theUnitFunc.SendHybRobotReady(ON);
}


void CFormMain::ClickGxstcHybridRobotRun()
{
	// I/O 가 이미 ON이면 OFF
	if( theUnitFunc.GetOutPutIOCheck(Y_HYBRID_ROBOT_RUN) == TRUE)
	{
		theUnitFunc.SendHybRobotRun(OFF);
	}
	else
		theUnitFunc.SendHybRobotRun(ON);
}


void CFormMain::ClickGxstcHybridRobotComplete()
{
	// I/O 가 이미 ON이면 OFF
	if( theUnitFunc.GetOutPutIOCheck(Y_HYBRID_SEND_COMPLETE) == TRUE)
	{
		theUnitFunc.SendHybSendComplete(OFF);
	}
	else
		theUnitFunc.SendHybSendComplete(OFF);
;
}
//2018-05-10,MGYUN, 하이브리드 I/O 사용자가 UI에서 확인 할 수 있게,
void CFormMain::DispHybridIOState()
{
	UINT uCLOSE = RGB(0,255,0);
	UINT uOPEN = RGB(224,224,224);

	if(!theUnitFunc.GetInPutIOState(X_HYBRID_CONV_READY))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_HYBRID_CONV_READY, uOPEN);
	}
	else
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_HYBRID_CONV_READY, uCLOSE);
	}

	if(!theUnitFunc.GetInPutIOState(X_HYBRID_CONV_RUN))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_HYBRID_CONV_RUN, uOPEN);
	}
	else
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_HYBRID_CONV_RUN, uCLOSE);
	}

	if(!theUnitFunc.GetInPutIOState(X_HYBRID_CONV_INTERFACE))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_HYBRID_CONV_INTERFACE, uOPEN);
	}
	else
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_HYBRID_CONV_INTERFACE, uCLOSE);
	}

	/////////////////// OUT PUT /////////////////////////////////////////////////////

	if(!theUnitFunc.GetOutPutIOState(Y_HYBRID_ROBOT_READY))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_HYBRID_ROBOT_READY, uOPEN);
	}
	else
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_HYBRID_ROBOT_READY, uCLOSE);
	}

	if(!theUnitFunc.GetOutPutIOState(Y_HYBRID_ROBOT_RUN))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_HYBRID_ROBOT_RUN, uOPEN);
	}
	else
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_HYBRID_ROBOT_RUN, uCLOSE);
	}

	if(!theUnitFunc.GetOutPutIOState(Y_HYBRID_SEND_COMPLETE))
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_HYBRID_ROBOT_COMPLETE, uOPEN);
	}
	else
	{
		CGxUICtrl::SetStaticColor(this,IDC_GXSTC_HYBRID_ROBOT_COMPLETE, uCLOSE);
	}
}

//2018wjp
void CFormMain::OnBnClickedBtnGetcellid()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDlgInputTrayID dlg;
	if(dlg.DoModal()==IDOK)
	{
		CString str=dlg.GetTrayCode().Trim();
		if(!str.IsEmpty())
		{
			theProcBank.m_strTrayID=str.MakeUpper();
			theUnitFunc.CIM_CELLID_Info_Request();
		}
	}
}

void CFormMain::OnBnClickedBtnCellid11()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CButton* pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID11);
	CString str=_T("");
	pBtn->GetWindowTextW(str);
	if(str.IsEmpty()||str=="-")
		return;
	if(AfxMessageBox(_T("Are you sure to clear the cell id?"),MB_YESNO)==IDYES)
	{
		pBtn->SetWindowTextW(_T("-"));
		theCellBank.RemoveCellTag(CELL_POS_TRAY_11);
    }
}


void CFormMain::OnBnClickedBtnCellid21()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CButton* pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID21);
	CString str=_T("");
	pBtn->GetWindowTextW(str);
	if(str.IsEmpty()||str=="-")
		return;
	if(AfxMessageBox(_T("Are you sure to clear the cell id?"),MB_YESNO)==IDYES)
	{
		pBtn->SetWindowTextW(_T("-"));
		theCellBank.RemoveCellTag(CELL_POS_TRAY_21);
    }
}


void CFormMain::OnBnClickedBtnCellid12()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    CButton* pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID12);
	CString str=_T("");
	pBtn->GetWindowTextW(str);
	if(str.IsEmpty()||str=="-")
		return;
	if(AfxMessageBox(_T("Are you sure to clear the cell id?"),MB_YESNO)==IDYES)
	{
		pBtn->SetWindowTextW(_T("-"));
		theCellBank.RemoveCellTag(CELL_POS_TRAY_12);
    }
}


void CFormMain::OnBnClickedBtnCellid22()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CButton* pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID22);
	CString str=_T("");
	pBtn->GetWindowTextW(str);
	if(str.IsEmpty()||str=="-")
		return;
	if(AfxMessageBox(_T("Are you sure to clear the cell id?"),MB_YESNO)==IDYES)
	{
		pBtn->SetWindowTextW(_T("-"));
		theCellBank.RemoveCellTag(CELL_POS_TRAY_22);
    }
}


void CFormMain::OnBnClickedBtnCellid13()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CButton* pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID13);
	CString str=_T("");
	pBtn->GetWindowTextW(str);
	if(str.IsEmpty()||str=="-")
		return;
	if(AfxMessageBox(_T("Are you sure to clear the cell id?"),MB_YESNO)==IDYES)
	{
		pBtn->SetWindowTextW(_T("-"));
		theCellBank.RemoveCellTag(CELL_POS_TRAY_13);
    }
}


void CFormMain::OnBnClickedBtnCellid23()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CButton* pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID23);
	CString str=_T("");
	pBtn->GetWindowTextW(str);
	if(str.IsEmpty()||str=="-")
		return;
	if(AfxMessageBox(_T("Are you sure to clear the cell id?"),MB_YESNO)==IDYES)
	{
		pBtn->SetWindowTextW(_T("-"));
		theCellBank.RemoveCellTag(CELL_POS_TRAY_23);
    }
}


void CFormMain::OnBnClickedBtnCellid14()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CButton* pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID14);
	CString str=_T("");
	pBtn->GetWindowTextW(str);
	if(str.IsEmpty()||str=="-")
		return;
	if(AfxMessageBox(_T("Are you sure to clear the cell id?"),MB_YESNO)==IDYES)
	{
		pBtn->SetWindowTextW(_T("-"));
		theCellBank.RemoveCellTag(CELL_POS_TRAY_14);
    }
}


void CFormMain::OnBnClickedBtnCellid24()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CButton* pBtn=(CButton*)GetDlgItem(IDC_BTN_CELLID24);
	CString str=_T("");
	pBtn->GetWindowTextW(str);
	if(str.IsEmpty()||str=="-")
		return;
	if(AfxMessageBox(_T("Are you sure to clear the cell id?"),MB_YESNO)==IDYES)
	{
		pBtn->SetWindowTextW(_T("-"));
		theCellBank.RemoveCellTag(CELL_POS_TRAY_24);
    }
}


LRESULT CFormMain::OnDCRFail(WPARAM wParam, LPARAM lParam)
{
	CDlgTrayScanError dlg(NULL,1);
	dlg.DoModal();
	return 0;
}

LRESULT CFormMain::OnDCRCimBackFail(WPARAM wParam, LPARAM lParam)
{
	CDlgTrayScanError dlg(NULL,2);
	dlg.DoModal();
	return 0;
}



void CFormMain::OnBnClickedButton4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theProcBank.Test_duong=TRUE;
}
