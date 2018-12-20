// FormInitialize.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FormInitialize.h"

#include "GUIDefine.h"

//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "UI\GausGUI\GxMsgBox.h"
#include "UI\GausGUI\GxButtonEx.h"

// CFormInitialize

enum TIMER_INDEX
{
	TIMER_MONITOR,
	TIMER_STEP
};
IMPLEMENT_DYNCREATE(CFormInitialize, CFormView)

CFormInitialize::CFormInitialize()
	: CFormView(CFormInitialize::IDD)
{
	m_bInit = FALSE;
}

CFormInitialize::~CFormInitialize()
{
}

void CFormInitialize::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_INITIALIZE_LOGLIST, m_ListLog);
}

BEGIN_MESSAGE_MAP(CFormInitialize, CFormView)

	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	ON_WM_TIMER()
END_MESSAGE_MAP()


// CFormInitialize 진단입니다.

#ifdef _DEBUG
void CFormInitialize::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormInitialize::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormInitialize 메시지 처리기입니다.

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
LRESULT CFormInitialize::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{
		m_Step = stepIdle;
		InitStatus();
		SetTimer(TIMER_MONITOR, 100, NULL);

	}
	else
	{
		KillTimer(TIMER_MONITOR);
		KillTimer(TIMER_STEP);
	}

	return 0;
}

// 표시 언어가 변경되었다.
LRESULT CFormInitialize::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	CGxUICtrl::BroadcastStaticCpationIndex(this, wParam, IDC_GXSTC_INITIALIZE_LIST1);
	CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_GXBTN_INITIALIZE_INITIALIZED);

	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


// 윈도우 생성전에 모양을 달리 지정한다.
BOOL CFormInitialize::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	cs.style = WS_OVERLAPPED | WS_CHILD;				// 테두리를 없앤다.

	return CFormView::PreCreateWindow(cs);
}
BEGIN_EVENTSINK_MAP(CFormInitialize, CFormView)
	ON_EVENT(CFormInitialize, IDC_GXBTN_INITIALIZE_INITIALIZED, DISPID_CLICK, CFormInitialize::ClickGxbtnInitializeInitialized, VTS_NONE)
	ON_EVENT(CFormInitialize, IDC_GXBTN_INITIALIZE_CANCEL, DISPID_CLICK, CFormInitialize::ClickGxbtnInitializeCancel, VTS_NONE)
END_EVENTSINK_MAP()


void CFormInitialize::OnInitialUpdate()
{
	if(m_bInit)	return;	// OnInitialUpdate 두번 실행되는 버그 방지 [10/24/2016 OSC]
	m_bInit = TRUE;
	CFormView::OnInitialUpdate();

	AddAxis_to_List();

	m_Color[ORIGIN_NONE] = Color(Color::LightGray).ToCOLORREF();
	m_Color[ORIGIN_START] = Color(Color::Yellow).ToCOLORREF();
	m_Color[ORIGIN_FINISH] = Color(Color::Lime).ToCOLORREF();
	m_Color[ORIGIN_FAIL] = Color(Color::Red).ToCOLORREF();

	LV_COLUMN lCol;
	lCol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;

	lCol.fmt = LVCFMT_CENTER;   // 칼럼의 정렬을 왼쪽정렬로 지정(LVCFMT_CENTER, LVCFMT_LEFT, LVCFMT_RIGHT중 선택)
	lCol.pszText = _T("NO");    //  칼럼의 제목을 지정
	lCol.iSubItem = 0;    
	CRect rect;//  서브아이템의 인덱스를 지정
	m_ListLog.GetWindowRect(rect);
	lCol.cx = rect.Width()-30;               //  칼럼의 넓이를 지정(Scroll Bar를 안생기게 하기 위해 -5 한다)
	m_ListLog.InsertColumn(0, &lCol);  //  LVCOLUMN구조체로 만들어진 값을 토대로 리스트 컨트롤에 칼럼을 삽입
}


void CFormInitialize::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(nIDEvent);
	BOOL bContinue = FALSE;
	switch(nIDEvent)
	{
	case TIMER_MONITOR:
		UpdateAllStatus();
		SetTimer(nIDEvent, 100, NULL);
		break;
	case TIMER_STEP:
		if(CheckInterlock())  //환경안전에서 초기화 도중 문열리면 인터락 발생하게
		{
			//2018wjp
			if(theProcBank.m_bModel2FullTray)
				theProcBank.m_bModel2FullTray=FALSE;
			if(Initializing())
			{
				bContinue = TRUE;
			}
		}
		if(bContinue)
		{
			SetTimer(nIDEvent, 100, NULL);
		}
		else if(m_StepOriginStatus[LED_FINISH] == ORIGIN_FINISH)
		{
			theProcBank.ChangeMachineState(CONST_MACHINE_STATE::MACHINE_PAUSE);

			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("알림"), _T("Inform"), _T("Inform"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("초기화 완료"), 
				_T("Initialize Success"), 
				_T("khởi tạo thành công") );

			dlgMsgBox.DoModal();
		}
		else
		{
			AxisAllStop();
		}
		break;
	}

	CFormView::OnTimer(nIDEvent);
}


void CFormInitialize::AddAxis_to_List()
{
	m_list1stPart.RemoveAll();
	m_list2ndPart.RemoveAll();
	m_list3rdPart.RemoveAll();

	// 첫번째 순서로 원점잡을 축들 [10/22/2016 OSC]
	m_list1stPart.AddTail(AXIS_SVO_PREALIGN_TR_Z);
	m_list1stPart.AddTail(AXIS_SVO_LD_TR_Z);
	m_list1stPart.AddTail(AXIS_SVO_ALIGN_CAM_Y);
	m_list1stPart.AddTail(AXIS_SVO_INS_CH1_Z);
	m_list1stPart.AddTail(AXIS_SVO_INS_X);
	m_list1stPart.AddTail(AXIS_SVO_INS_CH2_Z);
	m_list1stPart.AddTail(AXIS_SVO_ULD_TR_Z);
	m_list1stPart.AddTail(AXIS_SVO_GOOD_TRAY_IN_LIFT_Z);
	m_list1stPart.AddTail(AXIS_SVO_GOOD_TRAY_OUT_LIFT_Z);
	m_list1stPart.AddTail(AXIS_SVO_GOOD_TRAY_TR_Z);
	m_list1stPart.AddTail(AXIS_SVO_NG_TRAY_IN_LIFT_Z);
	m_list1stPart.AddTail(AXIS_SVO_NG_TRAY_OUT_LIFT_Z);
	m_list1stPart.AddTail(AXIS_SVO_NG_TRAY_TR_Z);
	m_list1stPart.AddTail(AXIS_STP_PA_CH1_X);
	m_list1stPart.AddTail(AXIS_STP_PA_CH1_Y);
	m_list1stPart.AddTail(AXIS_STP_PA_CH1_T);
	m_list1stPart.AddTail(AXIS_STP_PA_CH2_X);
	m_list1stPart.AddTail(AXIS_STP_PA_CH2_Y);
	m_list1stPart.AddTail(AXIS_STP_PA_CH2_T);
	m_list1stPart.AddTail(AXIS_STP_ALIGN_CAM_X);
	m_list1stPart.AddTail(AXIS_STP_INS_CH1_T);
	m_list1stPart.AddTail(AXIS_STP_INS_CH1_F);
	m_list1stPart.AddTail(AXIS_STP_INS_CH2_T);
	m_list1stPart.AddTail(AXIS_STP_INS_CH2_F);

	// 두번째 순서로 원점잡을 축들 [10/22/2016 OSC]
	m_list2ndPart.AddTail(AXIS_SVO_LD_UPPERSHUTTLE_Y);
	m_list2ndPart.AddTail(AXIS_SVO_LD_LOWERSHUTTLE_Y);
	m_list2ndPart.AddTail(AXIS_SVO_PREALIGN_TR_Y);
	m_list2ndPart.AddTail(AXIS_SVO_PA_STAGE_X);
	m_list2ndPart.AddTail(AXIS_SVO_LD_TR_X);
	m_list2ndPart.AddTail(AXIS_SVO_ULD_TR_X);
	m_list2ndPart.AddTail(AXIS_SVO_GOOD_TRAY_TR_Y);
	m_list2ndPart.AddTail(AXIS_SVO_NG_TRAY_TR_X);

	// 세번째 순서로 원점잡을 축들 [10/22/2016 OSC]
	m_list3rdPart.AddTail(AXIS_SVO_TURN_TABLE_T);
}

void CFormInitialize::InitStatus()
{
	for(int i = 0; i < E_ROBOT_MAX; i++)
		m_RobotOriginStatus[i] = ORIGIN_NONE;

	for(int i = 0; i < AXIS_ID_MAX; i++)
		m_AxisOriginStatus[i] = ORIGIN_NONE;

	for(int i = 0; i < STEP_LED_MAX; i++)
		m_StepOriginStatus[i] = ORIGIN_NONE;

	m_strLastMsg = _T("");
}

BOOL CFormInitialize::CheckPreInterlock( CString &strKorMsg, CString &strEngMsg, CString &strVnmMsg )
{
	return TRUE;
	if(theUnitFunc.TrayExistChk(GOOD_IN_TRAY_PORT))
	{
		strKorMsg = _T("GOOD IN PORT TRAY를 제거해주세요.");
		strEngMsg = _T("Please eject GOOD IN PORT Tray.");
		strVnmMsg = _T("Please eject GOOD IN PORT Tray.");
		return FALSE;
	}
	if(theUnitFunc.TrayExistChk(GOOD_OUT_TRAY_PORT))
	{
		strKorMsg = _T("GOOD OUT PORT TRAY를 제거해주세요.");
		strEngMsg = _T("Please eject GOOD OUT PORT Tray.");
		strVnmMsg = _T("Please eject GOOD OUT PORT Tray.");
		return FALSE;
	}
	if(theUnitFunc.TrayExistChk(NG_TRAY_PORT))
	{
		strKorMsg = _T("NG PORT TRAY를 제거해주세요.");
		strEngMsg = _T("Please eject NG PORT Tray.");
		strVnmMsg = _T("Please eject NG PORT Tray.");
		return FALSE;
	}
	return TRUE;
}

BOOL CFormInitialize::CheckInterlock()
{
	// Origin 진행중 Door, Light Curtain 등의 Interlock Sensor Check  [10/22/2016 OSC]
	if(theUnitFunc.DoorInterLockChkStart() == FALSE)
		return FALSE;

	if(theUnitFunc.LightCurtainChkStart() == FALSE)
		return FALSE;
	return TRUE;
}

void CFormInitialize::InitCylinder()
{
	theUnitFunc.AlignCylFwdBwd(GOOD_IN_TRAY_PORT,CYL_BWD);
	theUnitFunc.AlignCylFwdBwd(NG_TRAY_PORT,CYL_BWD);
	theUnitFunc.DivisionCylFwdBwd(GOOD_IN_TRAY_PORT,CYL_BWD);
	theUnitFunc.DivisionCylFwdBwd(NG_TRAY_PORT,CYL_BWD);
	theUnitFunc.BottomAlignCylFwdBwd(GOOD_IN_TRAY_PORT,CYL_BWD);
	theUnitFunc.BottomAlignCylFwdBwd(GOOD_OUT_TRAY_PORT,CYL_BWD);
	theUnitFunc.BottomAlignCylFwdBwd(NG_TRAY_PORT,CYL_BWD);
	theUnitFunc.BottomStoperUpDown(GOOD_IN_TRAY_PORT,CYL_DOWN);
	theUnitFunc.BottomStoperUpDown(GOOD_OUT_TRAY_PORT,CYL_DOWN);
	theUnitFunc.BottomStoperUpDown(NG_TRAY_PORT,CYL_DOWN);
	theUnitFunc.BottomStoperUpDown(GOOD_IN_TRAY_PORT,CYL_DOWN,TRAY_IN);
	theUnitFunc.BottomStoperUpDown(GOOD_OUT_TRAY_PORT,CYL_DOWN,TRAY_IN);
	theUnitFunc.BottomStoperUpDown(NG_TRAY_PORT,CYL_DOWN,TRAY_IN);
	theUnitFunc.NGLiftArmFwdBwd(NG_TRAY_PORT,CYL_FWD);
	theUnitFunc.NGLiftArmFwdBwd(NG_BUFFER_TRAY_PORT,CYL_BWD);
	theUnitFunc.NGTrayUpDown(NG_TRAY_PORT,CYL_DOWN);
	theUnitFunc.NGTrayUpDown(NG_BUFFER_TRAY_PORT,CYL_DOWN);
}

BOOL CFormInitialize::CheckCylinder()
{
	CString strMsg = _T("");
	//////////////////////////////////////////////////////////////////////////
	// Tray

	if(theUnitFunc.AlignCylFwdBwdChk(GOOD_IN_TRAY_PORT,CYL_BWD) == FALSE)
		strMsg.Format(_T("Good In Tray Port Upper Align Guide Backward Fail"));
	if(theUnitFunc.AlignCylFwdBwdChk(NG_TRAY_PORT,CYL_BWD) == FALSE)
		strMsg.Format(_T("NG Tray Port Upper Align Guide Backward Fail"));

	if(theUnitFunc.DivisionCylFwdBwdChk(GOOD_IN_TRAY_PORT,CYL_BWD) == FALSE)
		strMsg.Format(_T("Good In Tray Port Division Guide Backward Fail"));
	if(theUnitFunc.DivisionCylFwdBwdChk(NG_TRAY_PORT,CYL_BWD) == FALSE)
		strMsg.Format(_T("NG Tray Port Division Guide Backward Fail"));

	if(theUnitFunc.BottomAlignCylFwdBwdChk(GOOD_IN_TRAY_PORT,CYL_BWD) == FALSE)
		strMsg.Format(_T("Good In Tray Port Bottom Align Guide Backward Fail"));
	if(theUnitFunc.BottomAlignCylFwdBwdChk(GOOD_OUT_TRAY_PORT,CYL_BWD) == FALSE)
		strMsg.Format(_T("Good Out Tray Port Bottom Align Guide Backward Fail"));
	if(theUnitFunc.BottomAlignCylFwdBwdChk(NG_TRAY_PORT,CYL_BWD) == FALSE)
		strMsg.Format(_T("NG Tray Port Bottom Align Guide Backward Fail"));

	if(theUnitFunc.BottomStoperUpDownChk(GOOD_IN_TRAY_PORT,CYL_DOWN) == FALSE)
		strMsg.Format(_T("Good In Tray Port Bottom Stopper 2 Up Fail"));
	if(theUnitFunc.BottomStoperUpDownChk(GOOD_OUT_TRAY_PORT,CYL_DOWN) == FALSE)
		strMsg.Format(_T("Good Out Tray Port Bottom Stopper 2 Up Fail"));
	if(theUnitFunc.BottomStoperUpDownChk(NG_TRAY_PORT,CYL_DOWN) == FALSE)
		strMsg.Format(_T("NG Tray Port Bottom Stopper 2 Up Fail"));
	if(theUnitFunc.BottomStoperUpDownChk(GOOD_IN_TRAY_PORT,CYL_DOWN) == FALSE)
		strMsg.Format(_T("Good In Tray Port Bottom Stopper 1 Down Fail"));
	if(theUnitFunc.BottomStoperUpDownChk(GOOD_OUT_TRAY_PORT,CYL_DOWN) == FALSE)
		strMsg.Format(_T("Good Out Tray Port Bottom Stopper 1 Down Fail"));
	if(theUnitFunc.BottomStoperUpDownChk(NG_TRAY_PORT,CYL_DOWN) == FALSE)
		strMsg.Format(_T("NG Tray Port Bottom Stopper 1 Down Fail"));

	if(theUnitFunc.NGLiftArmFwdBwdChk(NG_TRAY_PORT,CYL_FWD) == FALSE)
		strMsg.Format(_T("NG Tray Lift Arm BackWard Fail"));
	if(theUnitFunc.NGLiftArmFwdBwdChk(NG_BUFFER_TRAY_PORT,CYL_BWD) == FALSE)
		strMsg.Format(_T("NG Buffer Tray Lift Arm BackWard Fail"));

	if(theUnitFunc.NGTrayUpDownChk(NG_TRAY_PORT,CYL_DOWN) == FALSE)
		strMsg.Format(_T("NG Tray Tray Up Cyl Down Fail"));
	if(theUnitFunc.NGTrayUpDownChk(NG_BUFFER_TRAY_PORT,CYL_DOWN) == FALSE)
		strMsg.Format(_T("NG Buffer Tray Tray Up Cyl Down Fail"));


	if(strMsg.GetLength() > 0)
	{
		m_strLastMsg = strMsg;
		return FALSE;
	}

	return TRUE;
}

BOOL CFormInitialize::ServoOff()
{
	CString strMsg = _T("");
	for(int i = 0; i < AXIS_ID_MAX; i++)
	{
		if( theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, (AXIS_ID)i).bAlarmOn
			|| (theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, (AXIS_ID)i).bServoOn == FALSE) )
		{
			if(theDeviceMotion.ServoOff(MP2100_THREAD_MAIN_PROCESS, (AXIS_ID)i) == FALSE)
			{
				m_AxisOriginStatus[i] = ORIGIN_FAIL;
				strMsg.Format(_T("%s Servo Off Fail"), theAxisBank.m_Axis[i].m_strAxisName);
				break;
			}
		}
	}

	if(strMsg.GetLength() > 0)
	{
		SetLogListUpdate(strMsg);
		return FALSE;
	}

	return TRUE;
}

BOOL CFormInitialize::AlarmReset()
{
	CString strMsg = _T("");
	for(int i = 0; i < AXIS_ID_MAX; i++)
	{
		if(theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, (AXIS_ID)i).bAlarmOn)
		{
			if(theDeviceMotion.ResetAlarm(MP2100_THREAD_MAIN_PROCESS, (AXIS_ID)i) == FALSE)
			{
				m_AxisOriginStatus[i] = ORIGIN_FAIL;
				strMsg.Format(_T("%s Alarm Reset Fail"), theAxisBank.m_Axis[i].m_strAxisName);
				break;
			}
		}
	}

	if(strMsg.GetLength() > 0)
	{
		SetLogListUpdate(strMsg);
		return FALSE;
	}

	return TRUE;
}

BOOL CFormInitialize::ServoOn()
{
	CString strMsg = _T("");
	for(int i = 0; i < AXIS_ID_MAX; i++)
	{
//		if(theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, (AXIS_ID)i).bServoOn == FALSE)
//		{
			if(theDeviceMotion.ServoOn(MP2100_THREAD_MAIN_PROCESS, (AXIS_ID)i) == FALSE)
			{
				m_AxisOriginStatus[i] = ORIGIN_FAIL;
				strMsg.Format(_T("%s Servo On Fail"), theAxisBank.m_Axis[i].m_strAxisName);
				break;
			}
//		}
	}

	if(strMsg.GetLength() > 0)
	{
		SetLogListUpdate(strMsg);
		return FALSE;
	}

	return TRUE;
}

BOOL CFormInitialize::InitAxisList( CList <AXIS_ID, AXIS_ID> *pList )
{
	POSITION pos = pList->GetHeadPosition();
	AXIS_ID axis;
	CAxisParam param;
	while (pos)
	{
		axis = pList->GetNext(pos);
		theAxisBank.GetParamNode((AXIS_ID)axis, param);
		if(param.m_bUse)
		{
			if(theDeviceMotion.OriginRetrun(MP2100_THREAD_MAIN_PROCESS, axis))
			{
				m_AxisOriginStatus[axis] = ORIGIN_START;
			}
			else
			{
				m_AxisOriginStatus[axis] = ORIGIN_FAIL;
				return FALSE;
			}
		}
	}
	return TRUE;
}

BOOL CFormInitialize::CheckAxisOriginComplete( CList <AXIS_ID, AXIS_ID> *pList )
{
	POSITION pos = pList->GetHeadPosition();
	AXIS_ID axis;
	BOOL bReturn = TRUE;
	CAxisParam param;
	while (pos)
	{
		axis = pList->GetNext(pos);
		theAxisBank.GetParamNode((AXIS_ID)axis, param);
		if(param.m_bUse)
		{
			if(theDeviceMotion.IsAxisReady(MP2100_THREAD_MAIN_PROCESS, axis, FALSE))
			{
				m_AxisOriginStatus[axis] = ORIGIN_FINISH;
			}
			else if(theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, axis).bAlarmOn)
			{
				m_AxisOriginStatus[axis] = ORIGIN_FAIL;
			}
			else
			{
				bReturn = FALSE;
			}
		}
	}
	return bReturn;
}

BOOL CFormInitialize::CheckAxisOriginFail()
{
	for(int i = 0; i < AXIS_ID_MAX; i++)
	{
		if(m_AxisOriginStatus[i] == ORIGIN_FAIL)
		{
			CString strMsg;
			strMsg.Format(_T("%s is Fail"), theAxisBank.m_Axis[i].m_strAxisName);
			SetLogListUpdate(strMsg);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CFormInitialize::Initializing()
{	long write = 0;
	int nStep = (int)m_Step;
	switch(nStep)
	{
	case stepStart:
		//20170103 byskcho 
		//초기화 중일때 상태를 INIT 로 변경
		theProcBank.ChangeMachineState(CONST_MACHINE_STATE::MACHINE_INIT);
		theUnitFunc.TrayData_RemoveData(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_ALIGN);
		theUnitFunc.TrayData_RemoveData(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_NG_IN_ALIGN);

		theUnitFunc.RobotCellVacOn(VAC_OFF,E_LD_ROBOT);
		theUnitFunc.RobotCellVacOn(VAC_OFF,E_UD_ROBOT);
		theUnitFunc.UpperShuttle_VacuumOn(VAC_OFF,JIG_CH_MAX);
		theUnitFunc.LowerShuttle_VacuumOn(VAC_OFF,JIG_CH_MAX);
		theUnitFunc.PATR_Vacuum_On(VAC_OFF,JIG_CH_MAX);
		theUnitFunc.PAStageVac(VAC_OFF,JIG_CH_MAX);
		theUnitFunc.LDTRVacOn(VAC_OFF,JIG_CH_MAX);
		theUnitFunc.UnloadTRVacOnOff(JIG_CH_MAX, VAC_OFF);
		theUnitFunc.TrayTr_VacOnOff(VAC_OFF);
		theUnitFunc.NGTrayTr_VacOnOff(VAC_OFF);

		theUnitFunc.LDRobotBitAllOff(E_ORG);
		theUnitFunc.UDRobotBitAllOff(E_ORG);
		theUnitFunc.LDRobotBitAllOff(E_CELL_LOADING,FALSE);
		theUnitFunc.UDRobotBitAllOff(E_CELL_LOADING,FALSE);
		theUnitFunc.LDRobotBitAllOff(E_PUT_1,FALSE);
		theUnitFunc.UDRobotBitAllOff(E_PUT_1,FALSE);
		theUnitFunc.LDRobotBitAllOff(E_PUT_2,FALSE);
		theUnitFunc.UDRobotBitAllOff(E_PUT_2,FALSE);
		//2018wjp
	
		theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
		theUnitFunc.UDRobotBitAllOff(E_PUT_3,FALSE);
		/////
		theUnitFunc.ConvRun(GOOD_IN_TRAY_PORT,OFF);
		theUnitFunc.ConvRun(GOOD_OUT_TRAY_PORT,OFF);
		theUnitFunc.ConvRun(NG_TRAY_PORT,OFF);
		theUnitFunc.BufConvRun(GOOD_IN_TRAY_PORT,OFF);
		theUnitFunc.BufConvRun(GOOD_OUT_TRAY_PORT,OFF);
		theUnitFunc.BufConvRun(NG_TRAY_PORT,OFF);

		m_Timer.Start();
		nStep++;
		break;
	case stepInitCylinder:
		if(m_Timer.Stop(FALSE) > 0.5)
		{
			m_StepOriginStatus[LED_CYLINDER_INIT] = ORIGIN_START;
			SetLogListUpdate(_T("Cylinder Init Start"));

			theUnitFunc.RobotBlowOff(E_LD_ROBOT);
			theUnitFunc.RobotBlowOff(E_UD_ROBOT);
			theUnitFunc.UpperShuttle_BlowOff(JIG_CH_MAX);
			theUnitFunc.LowerShuttle_BlowOff(JIG_CH_MAX);
			theUnitFunc.PATR_BlowOff(JIG_CH_MAX);
			theUnitFunc.PAStage_BlowOff(JIG_CH_MAX);
			theUnitFunc.LDTR_BlowOff(JIG_CH_MAX);
			theUnitFunc.UnloadTR_BlowOff(JIG_CH_MAX);
			theUnitFunc.TrayTr_BlowOff();
			theUnitFunc.NGTrayTr_BlowOff();

			InitCylinder();
			m_Timer.Start();
			nStep++;
		}
		break;
	case stepCheckCylinder:
		if(CheckCylinder())
		{
			m_StepOriginStatus[LED_CYLINDER_INIT] = ORIGIN_FINISH;
			SetLogListUpdate(_T("Cylinder Init Finish"));
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			nStep = stepAlarm;
			m_StepOriginStatus[LED_CYLINDER_INIT] = ORIGIN_FAIL;
			SetLogListUpdate(m_strLastMsg);
		}
		break;
	case stepServoOff:
		SetLogListUpdate(_T("Alarmed Axis Servo Off"));
		if(ServoOff())
		{
			m_StepOriginStatus[LED_SERVO_OFF] = ORIGIN_FINISH;
			m_Timer.Start();
			nStep++;
		}
		else
		{
			nStep = stepAlarm;
			m_StepOriginStatus[LED_SERVO_OFF] = ORIGIN_FAIL;
			SetLogListUpdate(m_strLastMsg);
		}
		break;
	case stepAlarmReset:
		if(m_Timer.Stop(FALSE) > 1.5)
		{
			SetLogListUpdate(_T("Alarm Reset"));
			if(AlarmReset())
			{
				m_StepOriginStatus[LED_ALARM_RESET] = ORIGIN_FINISH;
				m_Timer.Start();
				nStep++;
			}
			else
			{
				nStep = stepAlarm;
				m_StepOriginStatus[LED_ALARM_RESET] = ORIGIN_FAIL;
				SetLogListUpdate(m_strLastMsg);
			}
		}
		break;
	case stepServoOn:
		if(m_Timer.Stop(FALSE) > 2.)
		{
			SetLogListUpdate(_T("Servo On"));
			if(ServoOn())
			{
				m_StepOriginStatus[LED_SERVO_ON] = ORIGIN_FINISH;
				m_RobotOriginStatus[E_LD_ROBOT] = ORIGIN_START;
				m_RobotOriginStatus[E_UD_ROBOT] = ORIGIN_START;
				m_Timer.Start();
				nStep++;
			}
			else
			{
				nStep = stepAlarm;
				m_StepOriginStatus[LED_SERVO_ON] = ORIGIN_FAIL;
				SetLogListUpdate(m_strLastMsg);
			}
		}
		break;
	case stepRobotRunChk:
		if (m_Timer.Stop(FALSE) > 0.2)
		{
			if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_RUNNING_Y_ERRRESET))
			{
				theDeviceMelsec.RobotBitOff(LDROBOT_X_PLAYMODE_Y_EXTSTOP);
			}
			if((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_RUNNING_Y_ERRRESET))
			{
				theDeviceMelsec.RobotBitOff(UDROBOT_X_PLAYMODE_Y_EXTSTOP);
			}
			SetLogListUpdate(_T("Robot Run Chk"));
			m_Timer.Start();
			nStep++;
		}
		break;
	case stepRobotPause:
		if (m_Timer.Stop(FALSE) > 0.2)
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_X_PGMEND_Y_PAUSE);
			theDeviceMelsec.RobotBitOn(UDROBOT_X_PGMEND_Y_PAUSE);
			SetLogListUpdate(_T("Robot Pause"));
			m_Timer.Start();
			nStep++;
		}
		break;
	case stepRobotExtStop:
		if (m_Timer.Stop(FALSE) > 0.2)
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_X_PLAYMODE_Y_EXTSTOP);
			theDeviceMelsec.RobotBitOn(UDROBOT_X_PLAYMODE_Y_EXTSTOP);
			SetLogListUpdate(_T("Robot ExtStop"));
			m_Timer.Start();
			nStep++;
		}
		break;
	case stepRobotMotorRun:
		if (m_Timer.Stop(FALSE) > 0.2)
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_X_STOPPING_Y_EXTMOTORON);
			theDeviceMelsec.RobotBitOn(UDROBOT_X_STOPPING_Y_EXTMOTORON);
			SetLogListUpdate(_T("Robot Motor Run"));
			m_Timer.Start();
			nStep++;
		}
		break;
	case stepRobotExtReset:
		if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MOTORON_Y_LOWSPEED)
			&& (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_MOTORON_Y_LOWSPEED))

		{
			theDeviceMelsec.RobotBitOn(LDROBOT_X_ALARM_Y_EXTRESET);
			theDeviceMelsec.RobotBitOn(UDROBOT_X_ALARM_Y_EXTRESET);
			SetLogListUpdate(_T("Robot Ext Reset"));
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			nStep = stepAlarm;
			m_StepOriginStatus[LED_1ST_ORIGIN] = ORIGIN_FAIL;
			if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MOTORON_Y_LOWSPEED) == FALSE)
			{
				SetLogListUpdate(_T("LD Robot Motor energized Not On"));
				m_RobotOriginStatus[E_LD_ROBOT] = ORIGIN_FAIL;
			}
			if ((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_MOTORON_Y_LOWSPEED) == FALSE)
			{
				SetLogListUpdate(_T("UD Robot Motor energized Not On"));
				m_RobotOriginStatus[E_UD_ROBOT] = ORIGIN_FAIL;
			}
		}
		break;
	case stepRobotPGMSel:
/*		if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_EXTRESET_Y_PGMSEL2)
			&& (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_EXTRESET_Y_PGMSEL2))
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_X_PGMACK_Y_PGMSEL1);
			theDeviceMelsec.RobotBitOn(UDROBOT_X_PGMACK_Y_PGMSEL1);
			SetLogListUpdate(_T("Robot Ext Start"));
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			nStep = stepAlarm;
			m_StepOriginStatus[LED_1ST_ORIGIN] = ORIGIN_FAIL;
			if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_EXTRESET_Y_PGMSEL2) == FALSE)
			{
				SetLogListUpdate(_T("LD Robot EXT . Reset Not On"));
				m_RobotOriginStatus[E_LD_ROBOT] = ORIGIN_FAIL;
			}
			if ((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_EXTRESET_Y_PGMSEL2) == FALSE)
			{
				SetLogListUpdate(_T("UD Robot EXT . Reset Not On"));
				m_RobotOriginStatus[E_UD_ROBOT] = ORIGIN_FAIL;
			}
		}
		*/	
		if (m_Timer.Stop(FALSE) > 0.2)
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_X_PGMACK_Y_PGMSEL1);
			theDeviceMelsec.RobotBitOn(UDROBOT_X_PGMACK_Y_PGMSEL1);
			SetLogListUpdate(_T("Robot Ext Start"));
			m_Timer.Start();
			nStep++;
		}
		break;
	case stepRobotPGMStrobe:
		if (m_Timer.Stop(FALSE) > 0.2)
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_X_HOMEPOS2_Y_PGMSTROBE);
			theDeviceMelsec.RobotBitOn(UDROBOT_X_HOMEPOS2_Y_PGMSTROBE);
			SetLogListUpdate(_T("Robot PGM Strobe"));
			m_Timer.Start();
			nStep++;
		}
		break;
	case stepRobotEXTStart:
		if (m_Timer.Stop(FALSE) > 0.2)
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_X_TEACHMODE_Y_EXTSTART);
			theDeviceMelsec.RobotBitOn(UDROBOT_X_TEACHMODE_Y_EXTSTART);
			SetLogListUpdate(_T("Robot EXT Start"));
			m_Timer.Start();
			nStep++;
		}
		break;
	case stepRobotOrg:
		if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_RUNNING_Y_ERRRESET)
			&& (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_RUNNING_Y_ERRRESET))
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_X_BATWARN_Y_ORG);
			theDeviceMelsec.RobotBitOn(LDROBOT_X_BATWARN_Y_ORG);
			SetLogListUpdate(_T("Robot ORG"));
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 10.)
		{
			nStep = stepAlarm;
			m_StepOriginStatus[LED_1ST_ORIGIN] = ORIGIN_FAIL;
			if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_RUNNING_Y_ERRRESET) == FALSE)
			{
				SetLogListUpdate(_T("LD Robot Robot Running U1 Not On"));
				m_RobotOriginStatus[E_LD_ROBOT] = ORIGIN_FAIL;
			}
			if ((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_RUNNING_Y_ERRRESET) == FALSE)
			{
				SetLogListUpdate(_T("UD Robot Robot Running U1 Not On"));
				m_RobotOriginStatus[E_UD_ROBOT] = ORIGIN_FAIL;
			}
		}
		break;
	case stepRobotOrgOff:
		if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_HOMEPOS1_Y_CYCLESTOP)
			&& (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_HOMEPOS1_Y_CYCLESTOP))
		{
			theDeviceMelsec.RobotBitOff(LDROBOT_X_BATWARN_Y_ORG);
			theDeviceMelsec.RobotBitOff(UDROBOT_X_BATWARN_Y_ORG);
			SetLogListUpdate(_T("Robot Org Finish"));
			m_RobotOriginStatus[E_LD_ROBOT] = ORIGIN_FINISH;
			m_RobotOriginStatus[E_UD_ROBOT] = ORIGIN_FINISH;
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 20.)
		{
			nStep = stepAlarm;
			m_StepOriginStatus[LED_1ST_ORIGIN] = ORIGIN_FAIL;
			if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_PGMACK_Y_PGMSEL1) == FALSE)
			{
				SetLogListUpdate(_T("LD Robot Program Ack Not On"));
				m_RobotOriginStatus[E_LD_ROBOT] = ORIGIN_FAIL;
			}
			if ((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_PGMACK_Y_PGMSEL1) == FALSE)
			{
				SetLogListUpdate(_T("UD Robot Program Ack Not On"));
				m_RobotOriginStatus[E_UD_ROBOT] = ORIGIN_FAIL;
			}
		}
		break;
	case stepInit1stList:
		if(m_Timer.Stop(FALSE) > 2.)
		{
			m_StepOriginStatus[LED_1ST_ORIGIN] = ORIGIN_START;
			SetLogListUpdate(_T("1st Axis Origin Start"));
			if(InitAxisList(&m_list1stPart))
			{
				m_Timer.Start();
				nStep++;
			}
			else
			{
				m_StepOriginStatus[LED_1ST_ORIGIN] = ORIGIN_FAIL;
				nStep = stepAlarm;
			}
		}
		break;
	case stepCheck1stList:
		if(CheckAxisOriginComplete(&m_list1stPart))
		{
			m_StepOriginStatus[LED_1ST_ORIGIN] = ORIGIN_FINISH;
			SetLogListUpdate(_T("1st Axis Origin Finish"));
			nStep++;
		}
		else if( (m_Timer.Stop(FALSE) > 120.) || CheckAxisOriginFail() )
		{
			nStep = stepAlarm;
			SetLogListUpdate(_T("1st Axis Origin Fail"));
			m_StepOriginStatus[LED_1ST_ORIGIN] = ORIGIN_FAIL;
		}
		break;
	case stepAlignYReady:
		theUnitFunc.B_Zone_Cam_Y_Ready();
		SetLogListUpdate(_T("Align Y Axis Ready Pos Move"));
		
		m_Timer.Start();
		nStep++;
		break;
	case stepAlignYCheck:
		if(theUnitFunc.B_Zone_Cam_Y_Ready_Chk())
		{
			SetLogListUpdate(_T("Align Y Axis Ready Pos Move Complete"));
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 10.)
		{
			nStep = stepAlarm;
			SetLogListUpdate(_T("Align Y Axis Ready Pos Move Fail"));
			m_StepOriginStatus[LED_1ST_ORIGIN] = ORIGIN_FAIL;
		}
		break;
	case stepInit2ndList:
		m_StepOriginStatus[LED_2ND_ORIGIN] = ORIGIN_START;
		SetLogListUpdate(_T("2nd Axis Origin Start"));
		if(InitAxisList(&m_list2ndPart))
		{
			m_Timer.Start();
			nStep++;
		}
		else
		{
			m_StepOriginStatus[LED_2ND_ORIGIN] = ORIGIN_FAIL;
			nStep = stepAlarm;
		}
		break;
	case stepCheck2ndList:
		if(CheckAxisOriginComplete(&m_list2ndPart))
		{
			m_StepOriginStatus[LED_2ND_ORIGIN] = ORIGIN_FINISH;
			SetLogListUpdate(_T("2nd Axis Origin Finish"));
			nStep++;
		}
		else if( (m_Timer.Stop(FALSE) > 60.) || CheckAxisOriginFail() )
		{
			nStep = stepAlarm;
			SetLogListUpdate(_T("2nd Axis Origin Fail"));
			m_StepOriginStatus[LED_2ND_ORIGIN] = ORIGIN_FAIL;
		}
		break;
	case stepInit3rdList:
		m_StepOriginStatus[LED_3RD_ORIGIN] = ORIGIN_START;
		SetLogListUpdate(_T("3rd Axis Origin Start"));
		if(InitAxisList(&m_list3rdPart))
		{
			m_Timer.Start();
			nStep++;
		}
		else
		{
			m_StepOriginStatus[LED_3RD_ORIGIN] = ORIGIN_FAIL;
			nStep = stepAlarm;
		}
		break;
	case stepCheck3rdList:
		if(CheckAxisOriginComplete(&m_list3rdPart))
		{
			m_StepOriginStatus[LED_3RD_ORIGIN] = ORIGIN_FINISH;
			SetLogListUpdate(_T("3rd Axis Origin Finish"));
			nStep++;
		}
		else if( (m_Timer.Stop(FALSE) > 90.) || CheckAxisOriginFail() )
		{
			nStep = stepAlarm;
			SetLogListUpdate(_T("3rd Axis Origin Fail"));
			m_StepOriginStatus[LED_3RD_ORIGIN] = ORIGIN_FAIL;
		}
		break;
	case stepEnd:
		//wip2018
		if(theUnitFunc.LDTrayLiftTrayChk(SENSOR_ON))
		{
			theUnitFunc.BottomAlignCylFwdBwd(GOOD_IN_TRAY_PORT,CYL_FWD);
			while(!theUnitFunc.BottomAlignCylFwdBwdChk(GOOD_IN_TRAY_PORT,CYL_FWD))
			{   
				if(m_Timer.Stop(FALSE)>10)
				{nStep = stepAlarm;
				SetLogListUpdate(_T("Bottom align release Fail"));
				theUnitFunc.BottomAlignCylFwdBwd(GOOD_IN_TRAY_PORT,CYL_BWD);
				break;
				}
			}
			Sleep(1000);
			theUnitFunc.BottomAlignCylFwdBwd(GOOD_IN_TRAY_PORT,CYL_BWD);
		}

		if(theUnitFunc.ULDTrayLiftTrayChk(SENSOR_ON))
		{theUnitFunc.BottomAlignCylFwdBwd(GOOD_OUT_TRAY_PORT,CYL_FWD);
		while(!theUnitFunc.BottomAlignCylFwdBwdChk(GOOD_OUT_TRAY_PORT,CYL_FWD))
		{   
			if(m_Timer.Stop(FALSE)>10)
			{nStep = stepAlarm;
			SetLogListUpdate(_T("Bottom align release Fail"));
			theUnitFunc.BottomAlignCylFwdBwd(GOOD_IN_TRAY_PORT,CYL_BWD);
			break;
			}
		}
		Sleep(1000);
		theUnitFunc.BottomAlignCylFwdBwd(GOOD_OUT_TRAY_PORT,CYL_BWD);

		}

		if(	theUnitFunc.BottomAlignCylFwdBwdChk(GOOD_OUT_TRAY_PORT,CYL_BWD)&&theUnitFunc.BottomAlignCylFwdBwdChk(GOOD_IN_TRAY_PORT,CYL_BWD))
		{

		}
		else
		{
			theUnitFunc.BottomAlignCylFwdBwd(GOOD_OUT_TRAY_PORT,CYL_BWD);
			theUnitFunc.BottomAlignCylFwdBwd(GOOD_IN_TRAY_PORT,CYL_BWD);
		}
		Sleep(1000);
		if(	!theUnitFunc.BottomAlignCylFwdBwdChk(GOOD_OUT_TRAY_PORT,CYL_BWD)||!theUnitFunc.BottomAlignCylFwdBwdChk(GOOD_IN_TRAY_PORT,CYL_BWD))
		{
			nStep = stepAlarm;
			SetLogListUpdate(_T("Bottom align release Fail"));
			break;
		}

		nStep = stepIdle;
		theProcBank.m_bOriginSuccess = TRUE;
		theProcBank.ResetTemporarilyValue();
		theUnitFunc.AMT_PGSetZone();
		m_StepOriginStatus[LED_FINISH] = ORIGIN_FINISH;
		SetLogListUpdate(_T("All Initialize Finish"));
		CGxButtonEx *pBtn = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_INITIALIZE_INITIALIZED);
		pBtn->SetEnable(TRUE);
		break;
	}
	m_Step = (INITIALIZE_STEP)nStep;

	// 계속 진행 가능하면 TRUE [10/22/2016 OSC]
	if(m_Step > stepEnd)
		return FALSE;
	else
		return TRUE;
}

void CFormInitialize::AxisAllStop()
{
	for(int i = 0; i < AXIS_ID_MAX; i++)
	{
		if(theDeviceMotion.IsAxisReady(MP2100_THREAD_MAIN_PROCESS, (AXIS_ID)i, FALSE) == FALSE)
			theDeviceMotion.AxisStop(MP2100_THREAD_MAIN_PROCESS, (AXIS_ID)i);
	}
}

void CFormInitialize::UpdateAllStatus()
{
	m_pRobotOriginLed[E_LD_ROBOT						] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_LD_ROBOT					);
	m_pRobotOriginLed[E_UD_ROBOT						] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_ULD_ROBOT				);

	for(int i = 0; i < E_ROBOT_MAX; i++)
	{
		if(m_pRobotOriginLed[i]->GetBackColor() != m_Color[m_RobotOriginStatus[i]])
			m_pRobotOriginLed[i]->SetBackColor(m_Color[m_RobotOriginStatus[i]]);
	}

	m_pAxisOriginLed[AXIS_SVO_LOAD_CONV						] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_AXIS_LD_CONV					);
	m_pAxisOriginLed[AXIS_SVO_LD_UPPERSHUTTLE_Y				] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_AXIS_UPPERSHUTTLE_Y			);
	m_pAxisOriginLed[AXIS_SVO_LD_LOWERSHUTTLE_Y				] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_AXIS_LOWERSHUTTLE_Y			);
	m_pAxisOriginLed[AXIS_SVO_PREALIGN_TR_Y					] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_AXIS_PREALIGN_TR_Y			);
	m_pAxisOriginLed[AXIS_SVO_PREALIGN_TR_Z					] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_AXIS_PREALIGN_TR_Z			);
	m_pAxisOriginLed[AXIS_SVO_PA_STAGE_X					] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_AXIS_STAGE_X					);
	m_pAxisOriginLed[AXIS_SVO_LD_TR_X						] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_AXIS_LD_TR_X					);
	m_pAxisOriginLed[AXIS_SVO_LD_TR_Z						] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_AXIS_LD_TR_Z					);
	m_pAxisOriginLed[AXIS_SVO_TURN_TABLE_T					] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_AXIS_TURN_TABLE_T			);
	m_pAxisOriginLed[AXIS_SVO_ALIGN_CAM_Y					] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_AXIS_ALIGN_CAM_X				);
	m_pAxisOriginLed[AXIS_SVO_INS_X							] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_AXIS_INSP_X					);
	m_pAxisOriginLed[AXIS_SVO_INS_CH1_Z						] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_AXIS_INSP_CH1_Z				);
	m_pAxisOriginLed[AXIS_SVO_INS_CH2_Z						] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_AXIS_INSP_CH2_Z				);
	m_pAxisOriginLed[AXIS_SVO_ULD_TR_X						] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_AXIS_ULD_TR_X				);
	m_pAxisOriginLed[AXIS_SVO_ULD_TR_Z						] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_AXIS_ULD_TR_Z				);
	m_pAxisOriginLed[AXIS_SVO_GOOD_TRAY_IN_LIFT_Z			] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_AXIS_GOOD_TRAY_IN_LIFT_Z		);
	m_pAxisOriginLed[AXIS_SVO_GOOD_TRAY_OUT_LIFT_Z			] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_AXIS_GOOD_TRAY_OUT_LIFT_Z	);
	m_pAxisOriginLed[AXIS_SVO_GOOD_TRAY_TR_Y				] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_AXIS_GOOD_TRAY_TR_Y			);
	m_pAxisOriginLed[AXIS_SVO_GOOD_TRAY_TR_Z				] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_AXIS_GOOD_TRAY_TR_Z			);
	m_pAxisOriginLed[AXIS_SVO_NG_TRAY_IN_LIFT_Z				] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_AXIS_NG_TRAY_IN_LIFT_Z		);
	m_pAxisOriginLed[AXIS_SVO_NG_TRAY_OUT_LIFT_Z			] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_AXIS_NG_TRAY_OUT_LIFT_Z		);
	m_pAxisOriginLed[AXIS_SVO_NG_TRAY_TR_X					] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_AXIS_NG_TRAY_TR_X			);
	m_pAxisOriginLed[AXIS_SVO_NG_TRAY_TR_Z					] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_AXIS_NG_TRAY_TR_Z			);
	m_pAxisOriginLed[AXIS_STP_PA_CH1_X						] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_AXIS_PREALIGN_CH1_X			);
	m_pAxisOriginLed[AXIS_STP_PA_CH1_Y						] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_AXIS_PREALIGN_CH1_Y			);
	m_pAxisOriginLed[AXIS_STP_PA_CH1_T						] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_AXIS_PREALIGN_CH1_T			);
	m_pAxisOriginLed[AXIS_STP_PA_CH2_X						] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_AXIS_PREALIGN_CH2_X			);
	m_pAxisOriginLed[AXIS_STP_PA_CH2_Y						] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_AXIS_PREALIGN_CH2_Y			);
	m_pAxisOriginLed[AXIS_STP_PA_CH2_T						] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_AXIS_PREALIGN_CH2_T			);
	m_pAxisOriginLed[AXIS_STP_ALIGN_CAM_X					] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_AXIS_ALIGN_CAM_Y				);
	m_pAxisOriginLed[AXIS_STP_INS_CH1_T						] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_AXIS_INSP_CH1_T				);
	m_pAxisOriginLed[AXIS_STP_INS_CH1_F						] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_AXIS_INSP_CH1_F				);
	m_pAxisOriginLed[AXIS_STP_INS_CH2_T						] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_AXIS_INSP_CH2_T				);
	m_pAxisOriginLed[AXIS_STP_INS_CH2_F						] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_AXIS_INSP_CH2_F				);
	
	for(int i = 0; i < AXIS_ID_MAX; i++)
	{
		if(i != AXIS_STP_RESERVED)
		{
			if(m_pAxisOriginLed[i]->GetBackColor() != m_Color[m_AxisOriginStatus[i]])
				m_pAxisOriginLed[i]->SetBackColor(m_Color[m_AxisOriginStatus[i]]);
		}
	}

	m_pInitStepLed[LED_CYLINDER_INIT	] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_STEP_CYLINDER_CHECK	);
	m_pInitStepLed[LED_SERVO_OFF		] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_STEP_SERVO_OFF		);
	m_pInitStepLed[LED_ALARM_RESET		] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_STEP_ALARM_RESET		);
	m_pInitStepLed[LED_SERVO_ON			] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_STEP_SERVO_ON		);
	m_pInitStepLed[LED_1ST_ORIGIN		] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_STEP_1ST_ORIGIN		);
	m_pInitStepLed[LED_2ND_ORIGIN		] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_STEP_2ND_ORIGIN		);
	m_pInitStepLed[LED_3RD_ORIGIN		] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_STEP_3RD_ORIGIN		);
	m_pInitStepLed[LED_FINISH			] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_STEP_FINISH			);

	for(int i = 0; i < STEP_LED_MAX; i++)
	{
		if(m_pInitStepLed[i]->GetBackColor() != m_Color[m_StepOriginStatus[i]])
			m_pInitStepLed[i]->SetBackColor(m_Color[m_StepOriginStatus[i]]);
	}
}

void CFormInitialize::SetLogListUpdate( CString strMessage )
{
	if(strMessage.GetLength() < 1)
		return;
	m_ListLog.InsertItem(m_ListLog.GetItemCount(), strMessage);

	int nCount = m_ListLog.GetItemCount();
	if (nCount > 0)
		m_ListLog.EnsureVisible(nCount-1, FALSE);
}

void CFormInitialize::ClickGxbtnInitializeInitialized()
{
	if(m_Step != stepIdle)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("초기화 진행중입니다."), 
			_T("Now initializing..."), 
			_T("Bây giờ khởi tạo ...") );

		dlgMsgBox.DoModal();
		return;
	}

	CString strKor, strEng, strVnm;
	BOOL bRet = CheckPreInterlock(strKor, strEng, strVnm);
	if(bRet == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, strKor, strEng, strVnm);

		dlgMsgBox.DoModal();
		return;
	}
	//2017-04-18,skcho, 뮤팅램프 켜져 있으면 초기화 안되게, 환경안전 지적사항
	if(theProcBank.m_bNGTrayMuteOn == TRUE || theProcBank.m_bGoodInTrayMuteOn == TRUE || theProcBank.m_bGoodOutTrayMuteOn)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE,
			_T("Mute Ramp가 켜져 있어, 초기화를 할 수 없습니다."), 
			_T("Mute Ramp On, Can not Initialize"), 
			_T("Mute Ramp On, Can not Initialize") );

		dlgMsgBox.DoModal();
		return;
	}
// 리프트 홈 위치 아래로 이동으로 트레이를 초기화시 빼지 않는다.
/*
	if(theUnitFunc.LDTrayLiftTrayChk(SENSOR_ON))
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("양품 투입 트레이가 있음."), 
			_T("GOOD IN TRAY Exist."), 
			_T("GOOD IN TRAY có.") );

		dlgMsgBox.DoModal();
		return;
	}

	if(theUnitFunc.ULDTrayLiftTrayChk(SENSOR_ON))
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("양품 배출 트레이가 있음."), 
			_T("GOOD OUT TRAY Exist."), 
			_T("GOOD OUT TRAY có.") );

		dlgMsgBox.DoModal();
		return;
	}
*/
	if(theUnitFunc.NGTrayLiftTrayChk(SENSOR_ON) && (theConfigBank.m_Option.m_bUseDryRun == FALSE))
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("불량 트레이가 있음."), 
			_T("NG TRAY Exist."), 
			_T("NG TRAY có.") );

		dlgMsgBox.DoModal();
		return;
	}

	if(theUnitFunc.NGTrayOutLiftTrayChk(SENSOR_ON) && (theConfigBank.m_Option.m_bUseDryRun == FALSE))
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("불량 배출 트레이가 있음."), 
			_T("NG OUT TRAY Exist."), 
			_T("NG OUT TRAY có.") );

		dlgMsgBox.DoModal();
		return;
	}

	CGxMsgBox	dlgMsgBox;

	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("확인"), _T("Question"), _T("Question"));
	dlgMsgBox.SetMessage(TRUE, 
		_T("초기화를 진행하시겠습니까?"), 
		_T("Initializing Now?"), 
		_T("Khởi tạo lại?") );

	if(dlgMsgBox.DoModal() != IDOK)
		return;


	InitStatus();
	m_Step = stepStart;

	CGxButtonEx *pBtn = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_INITIALIZE_INITIALIZED);
	pBtn->SetEnable(FALSE);

	SetLogListUpdate(_T("Initialize Start"));
	
	//2018-06-26,NAM,Set ready ON when initilising.
	//theUnitFunc.SetOutPutIO(Y_HYBRID_ROBOT_READY,ON);

	SetTimer(TIMER_STEP, 100, NULL);
}


void CFormInitialize::ClickGxbtnInitializeCancel()
{
	KillTimer(TIMER_STEP);
	m_Step = stepIdle;
	CGxButtonEx *pBtn = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_INITIALIZE_INITIALIZED);
	pBtn->SetEnable(TRUE);
	//20170103 byskcho
	theProcBank.ChangeMachineState(CONST_MACHINE_STATE::MACHINE_PAUSE);
	//
	AxisAllStop();
}
