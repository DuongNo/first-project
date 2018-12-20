// FormTeachUnloaderTR.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "resource.h"
#include "FormTeachUnloaderTR.h"

#include "GUIDefine.h"

//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "UI\GausGUI\GxButtonEx.h"
#include "UI\GausGUI\GxStaticEx.h"

#include "UI\GausGUI\GxMsgBox.h"
// CFormTeachUnloaderTR

IMPLEMENT_DYNCREATE(CFormTeachUnloaderTR, CFormView)

CFormTeachUnloaderTR::CFormTeachUnloaderTR()
	: CFormView(CFormTeachUnloaderTR::IDD)
{
	m_bInit = FALSE;
	m_nASeq = seqC_idle;
	m_nBSeq = seqC_idle;
	m_nCSeq = seqC_idle;
	bCh1OK = TRUE;
	bCh2OK = TRUE;
}

CFormTeachUnloaderTR::~CFormTeachUnloaderTR()
{
}

void CFormTeachUnloaderTR::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFormTeachUnloaderTR, CFormView)
	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_WM_TIMER()
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
END_MESSAGE_MAP()


// CFormTeachUnloaderTR 진단입니다.

#ifdef _DEBUG
void CFormTeachUnloaderTR::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormTeachUnloaderTR::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormTeachUnloaderTR 메시지 처리기입니다.


// CFormTeachTurnTable 메시지 처리기입니다.

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
LRESULT CFormTeachUnloaderTR::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{
		UpdateULDTRState();
		SetTimer(1, 100, NULL);
		ClickGxbtnUldRobotUldstgCh1GetOk();
		ClickGxbtnUldRobotUldstgCh2GetOk();
	}
	else
	{
		KillTimer(1);
	}

	return 0;
}

void CFormTeachUnloaderTR::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
	case 1:
		UpdateULDTRState();
		break;
	case 2:
		UDRobotOrg();
		if(m_nASeq != seqA_idle)
			SetTimer(2, 50, NULL);
		break;
	case 3:
		UDRobotGet();
		if(m_nBSeq != seqB_idle)
			SetTimer(3, 50, NULL);
		break;
	case 4:
		UDRobotPut();
		if(m_nCSeq != seqC_idle)
			SetTimer(4, 50, NULL);
		break;
	}

	CFormView::OnTimer(nIDEvent);
}

// 표시 언어가 변경되었다.
LRESULT CFormTeachUnloaderTR::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	CGxUICtrl::BroadcastStaticCpationIndex(this, wParam, IDC_GXSTC_UNLDTR_UNLDTR);
	CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_GXSTC_UNLDTR_UNLDTR_X_GET);

	return 0;
}

BOOL CFormTeachUnloaderTR::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style = WS_OVERLAPPED | WS_CHILD;				// 테두리를 없앤다.

	return CFormView::PreCreateWindow(cs);
}

void CFormTeachUnloaderTR::UpdateULDTRState()
{
	CGxUICtrl::SetButtonColor(this, IDC_GXSTC_UNLDTR_UNLDTR_X_GET, theUnitFunc.MoveUnloadTR_X_GetPosChk() ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXSTC_UNLDTR_UNLDTR_X_PUT, theUnitFunc.MoveUnloadTR_X_PutPosChk() ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXSTC_UNLDTR_UNLDTR_Z_GET, theUnitFunc.MoveUnLoadTR_Z_GetPosChk() ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXSTC_UNLDTR_UNLDTR_Z_PUT, theUnitFunc.MoveUnLoadTR_Z_PutPosChk() ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXSTC_UNLDTR_UNLDTR_Z_UP, theUnitFunc.MoveUnLoadTR_ZAxis_UpChk() ? GXCOLOR_ON:GXCOLOR_OFF);
	
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UNLDTR_CH1_VAC_ON, (theUnitFunc.UnloadTRVacChk(TRUE, TRUE, FALSE) ? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UNLDTR_CH1_VAC_OFF, (theUnitFunc.UnloadTRVacChk(FALSE, TRUE, FALSE) ? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UNLDTR_CH2_VAC_ON, (theUnitFunc.UnloadTRVacChk(TRUE, FALSE, TRUE) ? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UNLDTR_CH2_VAC_OFF, (theUnitFunc.UnloadTRVacChk(FALSE, FALSE, TRUE) ? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_ULDROBOT_VAC_ON, (theUnitFunc.RobotCellVacOnChk(E_UD_ROBOT,VAC_ON) ? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_ULDROBOT_VAC_OFF, (!theUnitFunc.RobotCellVacOnChk(E_UD_ROBOT,VAC_ON) ? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_ULDSTG_CH1_VAC_ON, (theUnitFunc.ULDBuffSTGVacChk(TRUE, TRUE, FALSE) ? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_ULDSTG_CH1_VAC_OFF, (theUnitFunc.ULDBuffSTGVacChk(FALSE, TRUE, FALSE) ? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_ULDSTG_CH2_VAC_ON, (theUnitFunc.ULDBuffSTGVacChk(TRUE, FALSE, TRUE) ? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_ULDSTG_CH2_VAC_OFF, (theUnitFunc.ULDBuffSTGVacChk(FALSE, FALSE, TRUE) ? GXCOLOR_ON:GXCOLOR_OFF));
}


void CFormTeachUnloaderTR::OnInitialUpdate()
{
	if(m_bInit)	return;	// OnInitialUpdate 두번 실행되는 버그 방지 [10/24/2016 OSC]
	m_bInit = TRUE;
	CFormView::OnInitialUpdate();

	m_CurAxis = (AXIS_ID)AXIS_SVO_ULD_TR_X;
	m_CurTeach = TEACH_PARAM::ULD_TR_X_GET;
	m_CurID	  = IDC_GXSTC_UNLDTR_UNLDTR_X_GET;
	nCh = JIG_CH_1;

	for(int i = 0 ; i < 4 ; i++)
	{
		for(int j = 0 ; j < 4 ; j++)
		{
			m_bButtonCheck[i][j] = FALSE;
		}
	}	

	m_nCellButton[0][0] = IDC_GXBTN_UNLD_ROBOT_TRAY_00;
	m_nCellButton[0][1] = IDC_GXBTN_UNLD_ROBOT_TRAY_01;
	m_nCellButton[0][2] = IDC_GXBTN_UNLD_ROBOT_TRAY_02;
	m_nCellButton[0][3] = IDC_GXBTN_UNLD_ROBOT_TRAY_03;
	m_nCellButton[1][0] = IDC_GXBTN_UNLD_ROBOT_TRAY_10;
	m_nCellButton[1][1] = IDC_GXBTN_UNLD_ROBOT_TRAY_11;
	m_nCellButton[1][2] = IDC_GXBTN_UNLD_ROBOT_TRAY_12;
	m_nCellButton[1][3] = IDC_GXBTN_UNLD_ROBOT_TRAY_13;
	m_nCellButton[2][0] = IDC_GXBTN_UNLD_ROBOT_TRAY_20;
	m_nCellButton[2][1] = IDC_GXBTN_UNLD_ROBOT_TRAY_21;
	m_nCellButton[2][2] = IDC_GXBTN_UNLD_ROBOT_TRAY_22;
	m_nCellButton[2][3] = IDC_GXBTN_UNLD_ROBOT_TRAY_23;
	m_nCellButton[3][0] = IDC_GXBTN_UNLD_ROBOT_TRAY_30;
	m_nCellButton[3][1] = IDC_GXBTN_UNLD_ROBOT_TRAY_31;
	m_nCellButton[3][2] = IDC_GXBTN_UNLD_ROBOT_TRAY_32;
	m_nCellButton[3][3] = IDC_GXBTN_UNLD_ROBOT_TRAY_33;

	ClickGxbtnUnldRobotTray00();
}

void CFormTeachUnloaderTR::SetCurPageAxis(AXIS_ID axis, TEACH_PARAM::ID param, UINT nID)
{
	m_CurAxis = axis;
	m_CurTeach = param;
	m_CurID	  = nID;	

	GetMainHandler()->SetSelectedAxis(m_CurAxis, m_CurTeach);
}
BEGIN_EVENTSINK_MAP(CFormTeachUnloaderTR, CFormView)
	ON_EVENT(CFormTeachUnloaderTR, IDC_GXSTC_UNLDTR_UNLDTR_X_GET, DISPID_CLICK, CFormTeachUnloaderTR::ClickGxstcUnldtrUnldtrXGet, VTS_NONE)
	ON_EVENT(CFormTeachUnloaderTR, IDC_GXSTC_UNLDTR_UNLDTR_X_PUT, DISPID_CLICK, CFormTeachUnloaderTR::ClickGxstcUnldtrUnldtrXPut, VTS_NONE)
	ON_EVENT(CFormTeachUnloaderTR, IDC_GXSTC_UNLDTR_UNLDTR_Z_GET, DISPID_CLICK, CFormTeachUnloaderTR::ClickGxstcUnldtrUnldtrZGet, VTS_NONE)
	ON_EVENT(CFormTeachUnloaderTR, IDC_GXSTC_UNLDTR_UNLDTR_Z_PUT, DISPID_CLICK, CFormTeachUnloaderTR::ClickGxstcUnldtrUnldtrZPut, VTS_NONE)
	ON_EVENT(CFormTeachUnloaderTR, IDC_GXSTC_UNLDTR_UNLDTR_Z_UP, DISPID_CLICK, CFormTeachUnloaderTR::ClickGxstcUnldtrUnldtrZUp, VTS_NONE)
	ON_EVENT(CFormTeachUnloaderTR, IDC_GXBTN_ULD_ROBOT_ULDSTG_CH1_GET, DISPID_CLICK, CFormTeachUnloaderTR::ClickGxbtnUldRobotUldstgCh1Get, VTS_NONE)
	ON_EVENT(CFormTeachUnloaderTR, IDC_GXBTN_ULD_ROBOT_ULDSTG_CH2_GET, DISPID_CLICK, CFormTeachUnloaderTR::ClickGxbtnUldRobotUldstgCh2Get, VTS_NONE)
	ON_EVENT(CFormTeachUnloaderTR, IDC_GXBTN_ULDROBOT_VAC_ON, DISPID_CLICK, CFormTeachUnloaderTR::ClickGxbtnUldrobotVacOn, VTS_NONE)
	ON_EVENT(CFormTeachUnloaderTR, IDC_GXBTN_ULDROBOT_VAC_OFF, DISPID_CLICK, CFormTeachUnloaderTR::ClickGxbtnUldrobotVacOff, VTS_NONE)
	ON_EVENT(CFormTeachUnloaderTR, IDC_GXBTN_UD_ROBOT_ORG, DISPID_CLICK, CFormTeachUnloaderTR::ClickGxbtnUdRobotOrg, VTS_NONE)
	ON_EVENT(CFormTeachUnloaderTR, IDC_GXBTN_UNLD_ROBOT_TRAY_00, DISPID_CLICK, CFormTeachUnloaderTR::ClickGxbtnUnldRobotTray00, VTS_NONE)
	ON_EVENT(CFormTeachUnloaderTR, IDC_GXBTN_UNLD_ROBOT_TRAY_01, DISPID_CLICK, CFormTeachUnloaderTR::ClickGxbtnUnldRobotTray01, VTS_NONE)
	ON_EVENT(CFormTeachUnloaderTR, IDC_GXBTN_UNLD_ROBOT_TRAY_02, DISPID_CLICK, CFormTeachUnloaderTR::ClickGxbtnUnldRobotTray02, VTS_NONE)
	ON_EVENT(CFormTeachUnloaderTR, IDC_GXBTN_UNLD_ROBOT_TRAY_03, DISPID_CLICK, CFormTeachUnloaderTR::ClickGxbtnUnldRobotTray03, VTS_NONE)
	ON_EVENT(CFormTeachUnloaderTR, IDC_GXBTN_UNLD_ROBOT_TRAY_10, DISPID_CLICK, CFormTeachUnloaderTR::ClickGxbtnUnldRobotTray10, VTS_NONE)
	ON_EVENT(CFormTeachUnloaderTR, IDC_GXBTN_UNLD_ROBOT_TRAY_11, DISPID_CLICK, CFormTeachUnloaderTR::ClickGxbtnUnldRobotTray11, VTS_NONE)
	ON_EVENT(CFormTeachUnloaderTR, IDC_GXBTN_UNLD_ROBOT_TRAY_12, DISPID_CLICK, CFormTeachUnloaderTR::ClickGxbtnUnldRobotTray12, VTS_NONE)
	ON_EVENT(CFormTeachUnloaderTR, IDC_GXBTN_UNLD_ROBOT_TRAY_13, DISPID_CLICK, CFormTeachUnloaderTR::ClickGxbtnUnldRobotTray13, VTS_NONE)
	ON_EVENT(CFormTeachUnloaderTR, IDC_GXBTN_UNLD_ROBOT_TRAY_20, DISPID_CLICK, CFormTeachUnloaderTR::ClickGxbtnUnldRobotTray20, VTS_NONE)
	ON_EVENT(CFormTeachUnloaderTR, IDC_GXBTN_UNLD_ROBOT_TRAY_21, DISPID_CLICK, CFormTeachUnloaderTR::ClickGxbtnUnldRobotTray21, VTS_NONE)
	ON_EVENT(CFormTeachUnloaderTR, IDC_GXBTN_UNLD_ROBOT_TRAY_22, DISPID_CLICK, CFormTeachUnloaderTR::ClickGxbtnUnldRobotTray22, VTS_NONE)
	ON_EVENT(CFormTeachUnloaderTR, IDC_GXBTN_UNLD_ROBOT_TRAY_23, DISPID_CLICK, CFormTeachUnloaderTR::ClickGxbtnUnldRobotTray23, VTS_NONE)
	ON_EVENT(CFormTeachUnloaderTR, IDC_GXBTN_UNLD_ROBOT_TRAY_30, DISPID_CLICK, CFormTeachUnloaderTR::ClickGxbtnUnldRobotTray30, VTS_NONE)
	ON_EVENT(CFormTeachUnloaderTR, IDC_GXBTN_UNLD_ROBOT_TRAY_31, DISPID_CLICK, CFormTeachUnloaderTR::ClickGxbtnUnldRobotTray31, VTS_NONE)
	ON_EVENT(CFormTeachUnloaderTR, IDC_GXBTN_UNLD_ROBOT_TRAY_32, DISPID_CLICK, CFormTeachUnloaderTR::ClickGxbtnUnldRobotTray32, VTS_NONE)
	ON_EVENT(CFormTeachUnloaderTR, IDC_GXBTN_UNLD_ROBOT_TRAY_33, DISPID_CLICK, CFormTeachUnloaderTR::ClickGxbtnUnldRobotTray33, VTS_NONE)
	ON_EVENT(CFormTeachUnloaderTR, IDC_GXBTN_UNLD_ROBOT_GTRAY, DISPID_CLICK, CFormTeachUnloaderTR::ClickGxbtnUnldRobotGtray, VTS_NONE)
	ON_EVENT(CFormTeachUnloaderTR, IDC_GXBTN_UNLD_ROBOT_NGTRAY, DISPID_CLICK, CFormTeachUnloaderTR::ClickGxbtnUnldRobotNgtray, VTS_NONE)
	ON_EVENT(CFormTeachUnloaderTR, IDC_GXBTN_UNLDTR_CH1_VAC_ON, DISPID_CLICK, CFormTeachUnloaderTR::ClickGxbtnUnldtrCh1VacOn, VTS_NONE)
	ON_EVENT(CFormTeachUnloaderTR, IDC_GXBTN_UNLDTR_CH1_VAC_OFF, DISPID_CLICK, CFormTeachUnloaderTR::ClickGxbtnUnldtrCh1VacOff, VTS_NONE)
	ON_EVENT(CFormTeachUnloaderTR, IDC_GXBTN_UNLDTR_CH2_VAC_ON, DISPID_CLICK, CFormTeachUnloaderTR::ClickGxbtnUnldtrCh2VacOn, VTS_NONE)
	ON_EVENT(CFormTeachUnloaderTR, IDC_GXBTN_UNLDTR_CH2_VAC_OFF, DISPID_CLICK, CFormTeachUnloaderTR::ClickGxbtnUnldtrCh2VacOff, VTS_NONE)
	ON_EVENT(CFormTeachUnloaderTR, IDC_GXBTN_ULDSTG_CH1_VAC_ON, DISPID_CLICK, CFormTeachUnloaderTR::ClickGxbtnUldstgCh1VacOn, VTS_NONE)
	ON_EVENT(CFormTeachUnloaderTR, IDC_GXBTN_ULDSTG_CH1_VAC_OFF, DISPID_CLICK, CFormTeachUnloaderTR::ClickGxbtnUldstgCh1VacOff, VTS_NONE)
	ON_EVENT(CFormTeachUnloaderTR, IDC_GXBTN_ULDSTG_CH2_VAC_ON, DISPID_CLICK, CFormTeachUnloaderTR::ClickGxbtnUldstgCh2VacOn, VTS_NONE)
	ON_EVENT(CFormTeachUnloaderTR, IDC_GXBTN_ULDSTG_CH2_VAC_OFF, DISPID_CLICK, CFormTeachUnloaderTR::ClickGxbtnUldstgCh2VacOff, VTS_NONE)
	ON_EVENT(CFormTeachUnloaderTR, IDC_GXBTN_UNLD_ROBOT_TRAY_PITCH, DISPID_CLICK, CFormTeachUnloaderTR::ClickGxbtnUnldRobotTrayPitch, VTS_NONE)
	ON_EVENT(CFormTeachUnloaderTR, IDC_GXBTN_ULD_ROBOT_ULDSTG_CH1_GET_OK, DISPID_CLICK, CFormTeachUnloaderTR::ClickGxbtnUldRobotUldstgCh1GetOk, VTS_NONE)
	ON_EVENT(CFormTeachUnloaderTR, IDC_GXBTN_ULD_ROBOT_ULDSTG_CH1_GET_NG, DISPID_CLICK, CFormTeachUnloaderTR::ClickGxbtnUldRobotUldstgCh1GetNg, VTS_NONE)
	ON_EVENT(CFormTeachUnloaderTR, IDC_GXBTN_ULD_ROBOT_ULDSTG_CH2_GET_OK, DISPID_CLICK, CFormTeachUnloaderTR::ClickGxbtnUldRobotUldstgCh2GetOk, VTS_NONE)
	ON_EVENT(CFormTeachUnloaderTR, IDC_GXBTN_ULD_ROBOT_ULDSTG_CH2_GET_NG, DISPID_CLICK, CFormTeachUnloaderTR::ClickGxbtnUldRobotUldstgCh2GetNg, VTS_NONE)
END_EVENTSINK_MAP()


void CFormTeachUnloaderTR::ClickGxstcUnldtrUnldtrXGet()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_ULD_TR_X;
	m_CurTeach = TEACH_PARAM::ULD_TR_X_GET;
	SetCurPageAxis(m_CurAxis, m_CurTeach, (UINT)IDC_GXSTC_UNLDTR_UNLDTR_X_GET);
}


void CFormTeachUnloaderTR::ClickGxstcUnldtrUnldtrXPut()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_ULD_TR_X;
	m_CurTeach = TEACH_PARAM::ULD_TR_X_PUT;
	SetCurPageAxis(m_CurAxis, m_CurTeach, (UINT)IDC_GXSTC_UNLDTR_UNLDTR_X_PUT);
}


void CFormTeachUnloaderTR::ClickGxstcUnldtrUnldtrZGet()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_ULD_TR_Z;
	m_CurTeach = TEACH_PARAM::ULD_TR_Z_GET;
	SetCurPageAxis(m_CurAxis, m_CurTeach, (UINT)IDC_GXSTC_UNLDTR_UNLDTR_Z_GET);
}


void CFormTeachUnloaderTR::ClickGxstcUnldtrUnldtrZPut()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_ULD_TR_Z;
	m_CurTeach = TEACH_PARAM::ULD_TR_Z_PUT;
	SetCurPageAxis(m_CurAxis, m_CurTeach, (UINT)IDC_GXSTC_UNLDTR_UNLDTR_Z_PUT);
}


void CFormTeachUnloaderTR::ClickGxstcUnldtrUnldtrZUp()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_ULD_TR_Z;
	m_CurTeach = TEACH_PARAM::ULD_TR_Z_UP;
	SetCurPageAxis(m_CurAxis, m_CurTeach, (UINT)IDC_GXSTC_UNLDTR_UNLDTR_Z_UP);
}


void CFormTeachUnloaderTR::ClickGxbtnUldRobotUldstgCh1Get()
{
	//2017-12-13,SKCHO 안전사항 추가
	if(theUnitFunc.DoorInterLockChkStart() == FALSE)
		return ;
	
	if((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_RUNNING_Y_ERRRESET))
	{
		if (theUnitFunc.MoveUnloadTR_X_GetPosChk() == TRUE)
		{

			theUnitFunc.UDRobotBitAllOff(E_CELL_LOADING);
			Sleep(200);
			theDeviceMelsec.RobotBitOn(UDROBOT_XY_VACOFF);
			theDeviceMelsec.RobotBitOff(UDROBOT_XY_VACON);
			theDeviceMelsec.RobotBitOn(UDROBOT_XY_STGCH1VACON);
			theDeviceMelsec.RobotBitOff(UDROBOT_XY_STGCH1VACOFF);
	
			long write = 4; //GETOK1
			theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
			nCh = JIG_CH_1;
			m_nBSeq = seqB_1;
			KillTimer(3);
			SetTimer(3,200,NULL);
		}	
		else
		{
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("UD TR X 로딩 위치가 아닙니다."),
				_T("Please, UD TR X Pos Check"),		
				_T("Please, UD TR X Pos Check"));

			dlgMsgBox.DoModal();
		}
	}
	else
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("로봇 오리진 체크."),
			_T("Robot ORG Check"),		
			_T("Robot ORG Check"));

		dlgMsgBox.DoModal();
	}
}


void CFormTeachUnloaderTR::ClickGxbtnUldRobotUldstgCh2Get()
{
	//2017-12-13,SKCHO 안전사항 추가
	if(theUnitFunc.DoorInterLockChkStart() == FALSE)
		return ;
	
	if((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_RUNNING_Y_ERRRESET))
	{
		if (theUnitFunc.MoveUnloadTR_X_GetPosChk() == TRUE)
		{
			theUnitFunc.UDRobotBitAllOff(E_CELL_LOADING);
			Sleep(200);
			theDeviceMelsec.RobotBitOn(UDROBOT_XY_VACOFF);
			theDeviceMelsec.RobotBitOff(UDROBOT_XY_VACON);
			theDeviceMelsec.RobotBitOn(UDROBOT_Y_STG2VACON);
			theDeviceMelsec.RobotBitOff(UDROBOT_X_STG1GETERR_Y_STG2VACOFF);

			long write = 4; //GETOK1
			theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
			nCh = JIG_CH_2;
			m_nBSeq = seqC_1;
			KillTimer(3);
			SetTimer(3,200,NULL);
		}
		else
		{
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("UD TR X 로딩 위치가 아닙니다."),
				_T("Please, UD TR X Pos Check"),		
				_T("Please, UD TR X Pos Check"));

			dlgMsgBox.DoModal();
		}
	}
	else
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("로봇 오리진 체크."),
			_T("Robot ORG Check"),		
			_T("Robot ORG Check"));

		dlgMsgBox.DoModal();
	}
}


void CFormTeachUnloaderTR::ClickGxbtnUldrobotVacOn()
{
	//2017-12-13,SKCHO 안전사항 추가
	if(theUnitFunc.DoorInterLockChkStart() == FALSE)
		return ;

	theUnitFunc.RobotCellVacOn(VAC_ON,E_UD_ROBOT);
	theDeviceMelsec.RobotBitOn(UDROBOT_XY_VACON);
	theDeviceMelsec.RobotBitOff(UDROBOT_XY_VACOFF);

}


void CFormTeachUnloaderTR::ClickGxbtnUldrobotVacOff()
{
	//2017-12-13,SKCHO 안전사항 추가
	if(theUnitFunc.DoorInterLockChkStart() == FALSE)
		return ;

	theUnitFunc.RobotCellVacOn(VAC_OFF,E_UD_ROBOT);
	Sleep(200);
	theUnitFunc.RobotBlowOff(E_UD_ROBOT);
	theDeviceMelsec.RobotBitOff(UDROBOT_XY_VACON);
	theDeviceMelsec.RobotBitOn(UDROBOT_XY_VACOFF);
}


void CFormTeachUnloaderTR::ClickGxbtnUdRobotOrg()
{
	//2017-12-13,SKCHO 안전사항 추가
	if(theUnitFunc.DoorInterLockChkStart() == FALSE)
		return ;

	theUnitFunc.UDRobotBitAllOff(E_ORG);
	m_nASeq = seqA_1;
	KillTimer(2);
	SetTimer(2,200,NULL);
}

void CFormTeachUnloaderTR::GetSelectButton(int x , int y)
{	
	for (int i = 1; i < 5; i++)
	{
		for (int j = 1; j < 5 ; j++)
		{
			CGxUICtrl::SetButtonColor(this,m_nCellButton[i-1][j-1], GXCOLOR_OFF);	
		}
	}
	CGxUICtrl::SetButtonColor(this,m_nCellButton[x-1][y-1], GXCOLOR_ON);	
}

void CFormTeachUnloaderTR::ClickGxbtnUnldRobotTray00()
{
	GetSelectButton(1,1);
	long write = 17; 
	theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
}


void CFormTeachUnloaderTR::ClickGxbtnUnldRobotTray01()
{
	GetSelectButton(1,2);
	long write = 33; 
	theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
}


void CFormTeachUnloaderTR::ClickGxbtnUnldRobotTray02()
{
	GetSelectButton(1,3);
	long write = 49; 
	theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
}


void CFormTeachUnloaderTR::ClickGxbtnUnldRobotTray03()
{
	GetSelectButton(1,4);
	long write = 65; 
	theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
}


void CFormTeachUnloaderTR::ClickGxbtnUnldRobotTray10()
{
	GetSelectButton(2,1);
	long write = 18; 
	theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
}

void CFormTeachUnloaderTR::ClickGxbtnUnldRobotTray11()
{
	GetSelectButton(2,2);
	long write = 34; 
	theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
}

void CFormTeachUnloaderTR::ClickGxbtnUnldRobotTray12()
{
	GetSelectButton(2,3);
	long write = 50; 
	theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
}


void CFormTeachUnloaderTR::ClickGxbtnUnldRobotTray13()
{
	GetSelectButton(2,4);
	long write = 66; 
	theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
}


void CFormTeachUnloaderTR::ClickGxbtnUnldRobotTray20()
{
	GetSelectButton(3,1);
	long write = 19; 
	theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
}


void CFormTeachUnloaderTR::ClickGxbtnUnldRobotTray21()
{
	GetSelectButton(3,2);
	long write = 35; 
	theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
}


void CFormTeachUnloaderTR::ClickGxbtnUnldRobotTray22()
{
	GetSelectButton(3,3);
	long write = 51; 
	theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
}


void CFormTeachUnloaderTR::ClickGxbtnUnldRobotTray23()
{
	GetSelectButton(3,4);
	long write = 67; 
	theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
}


void CFormTeachUnloaderTR::ClickGxbtnUnldRobotTray30()
{
	GetSelectButton(4,1);
	long write = 20; 
	theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
}


void CFormTeachUnloaderTR::ClickGxbtnUnldRobotTray31()
{
	GetSelectButton(4,2);
	long write = 36; 
	theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
}


void CFormTeachUnloaderTR::ClickGxbtnUnldRobotTray32()
{
	GetSelectButton(4,3);
	long write = 52; 
	theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
}


void CFormTeachUnloaderTR::ClickGxbtnUnldRobotTray33()
{
	GetSelectButton(4,4);
	long write = 68; 
	theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
}


void CFormTeachUnloaderTR::ClickGxbtnUnldRobotGtray()
{
	if((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_RUNNING_Y_ERRRESET))
	{
		if (theUnitFunc.TrayTrY_OutChk() == TRUE)
		{
			theUnitFunc.UDRobotBitAllOff(E_PUT_1);
			Sleep(200);
			theDeviceMelsec.RobotBitOn(UDROBOT_XY_OKTRAYPUT);
			theDeviceMelsec.RobotBitOn(UDROBOT_XY_VACON);

			long write = 128; //PUTOK3
			theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
			nCh = JIG_CH_1;
			m_nCSeq = seqC_1;
			m_Timer.Start();
			KillTimer(4);
			SetTimer(4,100,NULL);
		}
		else
		{
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("GOOD TRAY TR Y가 PUT 위치가 아닙니다."),
				_T("Please, GOOD TRAY TR Y Pos Check"),		
				_T("Please, GOOD TRAY TR Y Pos Check"));

			dlgMsgBox.DoModal();
		}
	}
	else
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("로봇 오리진 체크."),
			_T("Robot ORG Check"),		
			_T("Robot ORG Check"));

		dlgMsgBox.DoModal();
	}
}


void CFormTeachUnloaderTR::ClickGxbtnUnldRobotNgtray()
{
	if((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_RUNNING_Y_ERRRESET))
	{
		if (theUnitFunc.NGTrayTrX_OutChk() == TRUE)
		{
			theUnitFunc.UDRobotBitAllOff(E_PUT_2);
			Sleep(200);
			theDeviceMelsec.RobotBitOn(UDROBOT_XY_NGTRAYPUT);
			theDeviceMelsec.RobotBitOn(UDROBOT_XY_VACON);
			long write = 32; //PUTOK2
			theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
			nCh = JIG_CH_2;
			m_nCSeq = seqC_1;
			m_Timer.Start();
			KillTimer(4);
			SetTimer(4,100,NULL);
		}
		else
		{
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("NG TRAY TR X가 PUT 위치가 아닙니다."),
				_T("Please, NG TRAY TR Y Pos Check"),		
				_T("Please, NG TRAY TR Y Pos Check"));

			dlgMsgBox.DoModal();
		}
	}
	else
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("로봇 오리진 체크."),
			_T("Robot ORG Check"),		
			_T("Robot ORG Check"));

		dlgMsgBox.DoModal();
	}
}


void CFormTeachUnloaderTR::ClickGxbtnUnldtrCh1VacOn()
{
	theUnitFunc.UnloadTRVacOnOff(JIG_CH_1, VAC_ON);
}


void CFormTeachUnloaderTR::ClickGxbtnUnldtrCh1VacOff()
{
	theUnitFunc.UnloadTRVacOnOff(JIG_CH_1, VAC_OFF);
	Sleep(200);
	theUnitFunc.UnloadTR_BlowOff(JIG_CH_1);
}


void CFormTeachUnloaderTR::ClickGxbtnUnldtrCh2VacOn()
{
	theUnitFunc.UnloadTRVacOnOff(JIG_CH_2, VAC_ON);
}


void CFormTeachUnloaderTR::ClickGxbtnUnldtrCh2VacOff()
{
	theUnitFunc.UnloadTRVacOnOff(JIG_CH_2, VAC_OFF);
	Sleep(200);
	theUnitFunc.UnloadTR_BlowOff(JIG_CH_2);
}


void CFormTeachUnloaderTR::ClickGxbtnUldstgCh1VacOn()
{
	theUnitFunc.ULDBuffSTGVacOnOff(VAC_ON, TRUE,FALSE);
}


void CFormTeachUnloaderTR::ClickGxbtnUldstgCh1VacOff()
{
	theUnitFunc.ULDBuffSTGVacOnOff(VAC_OFF, TRUE,FALSE);
	Sleep(200);
	theUnitFunc.ULDBuffSTG_BlowOff(TRUE,FALSE);
}


void CFormTeachUnloaderTR::ClickGxbtnUldstgCh2VacOn()
{
	theUnitFunc.ULDBuffSTGVacOnOff(VAC_ON, FALSE,TRUE);
}


void CFormTeachUnloaderTR::ClickGxbtnUldstgCh2VacOff()
{
	theUnitFunc.ULDBuffSTGVacOnOff(VAC_OFF, FALSE,TRUE);
	Sleep(200);
	theUnitFunc.ULDBuffSTG_BlowOff(FALSE,TRUE);
}


void CFormTeachUnloaderTR::UDRobotOrg()
{
	switch(m_nASeq)
	{
	case seqA_idle:
		break;
	case seqA_1:
		if((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_RUNNING_Y_ERRRESET))
		{
			theDeviceMelsec.RobotBitOff(UDROBOT_X_PLAYMODE_Y_EXTSTOP);
		}
		m_nASeq++;
		m_Timer.Start();
		break;
	case seqA_2:
		theDeviceMelsec.RobotBitOn(UDROBOT_X_PGMEND_Y_PAUSE);
		m_nASeq++;
		m_Timer.Start();
		break;
	case seqA_3:
		if (m_Timer.Stop(FALSE) > 0.2)
		{
			theDeviceMelsec.RobotBitOn(UDROBOT_X_PLAYMODE_Y_EXTSTOP);
			m_nASeq++;
			m_Timer.Start();
		}
		break;
	case seqA_4:
		if (m_Timer.Stop(FALSE) > 0.2)
		{
			theDeviceMelsec.RobotBitOn(UDROBOT_X_STOPPING_Y_EXTMOTORON);
			m_nASeq++;
			m_Timer.Start();
		}
		break;
	case seqA_5:
		if ((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_MOTORON_Y_LOWSPEED))
		{
			theDeviceMelsec.RobotBitOn(UDROBOT_X_ALARM_Y_EXTRESET);
			m_nASeq++;
			m_Timer.Start();
		}
		if (m_Timer.Stop(FALSE) > 3)
		{
			theUnitFunc.UDRobotBitAllOff(E_ORG);
			m_nASeq = seqA_idle;
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
	case seqA_6:
		if ((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_EXTRESET_Y_PGMSEL2))
		{
			theDeviceMelsec.RobotBitOn(UDROBOT_X_PGMACK_Y_PGMSEL1);
			m_nASeq++;
			m_Timer.Start();
		}
		else if (m_Timer.Stop(FALSE) > 1)
		{
			theUnitFunc.UDRobotBitAllOff(E_ORG);
			m_nASeq = seqA_idle;
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
	case seqA_7:
		if (m_Timer.Stop(FALSE) > 0.2)
		{
			theDeviceMelsec.RobotBitOn(UDROBOT_X_HOMEPOS2_Y_PGMSTROBE);
			m_nASeq++;
			m_Timer.Start();
		}
		break;
	case seqA_8:
		if (m_Timer.Stop(FALSE) > 0.2)
		{
			theDeviceMelsec.RobotBitOn(UDROBOT_X_TEACHMODE_Y_EXTSTART);
			m_nASeq++;
			m_Timer.Start();
		}
		break;
	case seqA_9:
		if ((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_RUNNING_Y_ERRRESET))
		{
			theDeviceMelsec.RobotBitOn(UDROBOT_X_BATWARN_Y_ORG);
			m_nASeq++;
			m_Timer.Start();
		}
		else if (m_Timer.Stop(FALSE) > 3)
		{	
			theUnitFunc.UDRobotBitAllOff(E_ORG);
			m_nASeq = seqA_idle;
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
	case seqA_10:
		if ((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_HOMEPOS1_Y_CYCLESTOP))
		{
			theDeviceMelsec.RobotBitOff(UDROBOT_X_BATWARN_Y_ORG);
			m_nASeq = seqA_idle;
			theUnitFunc.UDRobotBitAllOff(E_ORG);

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
			theUnitFunc.UDRobotBitAllOff(E_ORG);
			m_nASeq = seqA_idle;
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

void CFormTeachUnloaderTR::UDRobotGet()
{
	switch(m_nBSeq)
	{
	case seqB_idle:
		break;
	case seqB_1:
		if (nCh == JIG_CH_1)
		{
			if ((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_BUSY_Y_PGMSEL6) == FALSE)
			{
				theDeviceMelsec.RobotBitOn(UDROBOT_XY_GETSTAGE);
				if(bCh1OK)
				{
					theDeviceMelsec.RobotBitOn(UDROBOT_X_STGCH2VACON_Y_STG1OK);
					theDeviceMelsec.RobotBitOff(UDROBOT_X_STGCH2VACOFF_Y_STG1NG);
				}
				else
				{
					theDeviceMelsec.RobotBitOff(UDROBOT_X_STGCH2VACON_Y_STG1OK);
					theDeviceMelsec.RobotBitOn(UDROBOT_X_STGCH2VACOFF_Y_STG1NG);
				}
				
				m_nBSeq++;
				m_Timer.Start();
			}
			else if(m_Timer.Stop(FALSE) > 3)
			{	
				theUnitFunc.UDRobotBitAllOff(E_CELL_LOADING);
				m_nBSeq = seqB_idle;
				CGxMsgBox	dlgMsgBox;

				dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
				dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
				dlgMsgBox.SetMessage(FALSE, 
					_T("로봇이 정지되지 않았습니다."),
					_T("Robot Busy Not Off"),		
					_T("Robot Busy Not Off"));

				dlgMsgBox.DoModal();
				break;
			}
		}
		else
		{
			if ((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_BUSY_Y_PGMSEL6) == FALSE)
			{
				theDeviceMelsec.RobotBitOn(UDROBOT_XY_GETSTAGE);
				if(bCh2OK)
				{
					theDeviceMelsec.RobotBitOn(UDROBOT_X_STG2GETERR_Y_STG2OK);
					theDeviceMelsec.RobotBitOff(UDROBOT_X_STGGETCOMP_Y_STG2NG);
				}
				else
				{
					theDeviceMelsec.RobotBitOff(UDROBOT_X_STG2GETERR_Y_STG2OK);
					theDeviceMelsec.RobotBitOn(UDROBOT_X_STGGETCOMP_Y_STG2NG);
				}
				m_nBSeq++;
				m_Timer.Start();
			}
			else if(m_Timer.Stop(FALSE) > 3)
			{	
				theUnitFunc.UDRobotBitAllOff(E_CELL_LOADING);
				m_nBSeq = seqB_idle;
				CGxMsgBox	dlgMsgBox;

				dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
				dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
				dlgMsgBox.SetMessage(FALSE, 
					_T("로봇이 정지되지 않았습니다."),
					_T("Robot Busy Not Off"),		
					_T("Robot Busy Not Off"));

				dlgMsgBox.DoModal();
				break;
			}
		}
		break;
	case seqB_2:
		if (nCh == JIG_CH_1)
		{
			if ((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_GETSTAGE) == FALSE)
			{
				m_nBSeq++;
				m_Timer.Start();
			}
			else if(m_Timer.Stop(FALSE) > 3)
			{	
				theUnitFunc.UDRobotBitAllOff(E_CELL_LOADING);
				m_nBSeq = seqB_idle;
				CGxMsgBox	dlgMsgBox;

				dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
				dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
				dlgMsgBox.SetMessage(FALSE, 
					_T("로딩 인터락이 OfF 되지 않았음."),
					_T("Stage Get Interlock Not off"),		
					_T("Stage Get Interlock Not off"));

				dlgMsgBox.DoModal();
				break;
			}
		}
		else
		{
			if ((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_GETSTAGE) == FALSE)
			{
				m_nBSeq++;
				m_Timer.Start();
			}
			else if(m_Timer.Stop(FALSE) > 3)
			{	
				theUnitFunc.UDRobotBitAllOff(E_CELL_LOADING);
				m_nBSeq = seqB_idle;
				CGxMsgBox	dlgMsgBox;

				dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
				dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
				dlgMsgBox.SetMessage(FALSE, 
					_T("로딩 인터락이 OfF 되지 않았음."),
					_T("Stage Get Interlock Not off"),		
					_T("Stage Get Interlock Not off"));

				dlgMsgBox.DoModal();
				break;
			}
		}
		break;
	case seqB_3:
		if ((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_VACON))
		{
			theUnitFunc.RobotCellVacOn(VAC_ON,E_UD_ROBOT);
			m_nBSeq++;
			m_Timer.Start();
		}
		else if(m_Timer.Stop(FALSE) > 3)
		{	
			theUnitFunc.UDRobotBitAllOff(E_CELL_LOADING);
			m_nBSeq = seqB_idle;
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("로봇 VAC ON신호가 ON되지 않았습니다"),
				_T("Robot VAC On Not On"),		
				_T("Robot VAC On Not On"));

			dlgMsgBox.DoModal();
			break;
		}
		break;
	case seqB_4:
		if (theUnitFunc.RobotCellVacOnChk(E_UD_ROBOT,VAC_ON))
		{
			theDeviceMelsec.RobotBitOn(UDROBOT_XY_VACON);
			theDeviceMelsec.RobotBitOff(UDROBOT_XY_VACOFF);
			m_nBSeq++;
			m_Timer.Start();
		}
		else if(m_Timer.Stop(FALSE) > 3)
		{	
			theUnitFunc.UDRobotBitAllOff(E_CELL_LOADING);
			m_nBSeq = seqB_idle;
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("로봇 VAC ON되지 않았습니다"),
				_T("Robot VAC On Not On"),		
				_T("Robot VAC On Not On"));

			dlgMsgBox.DoModal();
			break;
		}
		break;
	case seqB_5:
		if (nCh == JIG_CH_1)
		{
			if ((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_STGCH1VACOFF))
			{
				theUnitFunc.ULDBuffSTGVacOnOff(VAC_OFF,TRUE,FALSE);
				m_nBSeq++;
				m_Timer.Start();
			}
			else if(m_Timer.Stop(FALSE) > 3)
			{	
				theUnitFunc.UDRobotBitAllOff(E_CELL_LOADING);
				m_nBSeq = seqB_idle;
				CGxMsgBox	dlgMsgBox;

				dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
				dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
				dlgMsgBox.SetMessage(FALSE, 
					_T("Vac Off가 On되지 않았습니다."),
					_T("Vac Off Not On"),		
					_T("Vac Off Not On"));

				dlgMsgBox.DoModal();
				break;
			}
		}
		else
		{
			if ((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_STGCH2VACOFF_Y_STG1NG))
			{
				theUnitFunc.ULDBuffSTGVacOnOff(VAC_OFF,FALSE,TRUE);
				m_nBSeq++;
				m_Timer.Start();
			}
			else if(m_Timer.Stop(FALSE) > 3)
			{	
				theUnitFunc.UDRobotBitAllOff(E_CELL_LOADING);
				m_nBSeq = seqB_idle;
				CGxMsgBox	dlgMsgBox;

				dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
				dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
				dlgMsgBox.SetMessage(FALSE, 
					_T("Vac Off가 On되지 않았습니다."),
					_T("Vac Off Not On"),		
					_T("Vac Off Not On"));

				dlgMsgBox.DoModal();
				break;
			}
		}
		break;
	case seqB_6:
		if ((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_STGGETCOMP_Y_STG2NG))
		{
			m_nBSeq++;
			m_Timer.Start();
			theUnitFunc.ULDBuffSTG_BlowOff(TRUE,TRUE);
		}
		else if(m_Timer.Stop(FALSE) > 3)
		{	
			theUnitFunc.UDRobotBitAllOff(E_CELL_LOADING);
			m_nBSeq = seqB_idle;
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("스테이지 로딩 완료되지 않았습니다."),
				_T("Stage Get Comp"),		
				_T("Stage Get Comp"));

			dlgMsgBox.DoModal();
			break;
		}
		break;
	case seqB_7:
		if ((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_GETSTAGE))
		{
			theUnitFunc.UDRobotBitAllOff(E_CELL_LOADING);
			m_nBSeq = seqC_idle;
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
		else if(m_Timer.Stop(FALSE) > 6)
		{	
			theUnitFunc.UDRobotBitAllOff(E_CELL_LOADING);
			m_nBSeq = seqB_idle;
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("로딩 인터락이 ON 되지 않았습니다."),
				_T("Get Interlock Not On"),		
				_T("Get Interlock Not On"));

			dlgMsgBox.DoModal();
			break;
		}
		break;

	default:
		break;
	}
}

void CFormTeachUnloaderTR::UDRobotPut()
{
	switch(m_nCSeq)
	{
	case seqC_idle:
		break;
	case seqC_1:
		if (nCh == JIG_CH_1)
		{
			if ((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_OKTRAYPUT) == FALSE)
			{
				m_nCSeq++;
				m_Timer.Start();
			}
			else if(m_Timer.Stop(FALSE) > 5)
			{	
				theUnitFunc.UDRobotBitAllOff(E_PUT_1);
				m_nCSeq = seqC_idle;
				CGxMsgBox	dlgMsgBox;

				dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
				dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
				dlgMsgBox.SetMessage(FALSE, 
					_T("양품트레이인터락이 OFF가 아닙니다."),
					_T("Ok tray Put Interlock Not Off"),		
					_T("Ok tray Put Interlock Not Off"));

				dlgMsgBox.DoModal();
				break;
			}
		}
		else 
		{
			if ((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_NGTRAYPUT) == FALSE)
			{
				m_nCSeq++;
				m_Timer.Start();
			}
			else if(m_Timer.Stop(FALSE) > 5)
			{	
				theUnitFunc.UDRobotBitAllOff(E_PUT_2);
				m_nCSeq = seqC_idle;
				CGxMsgBox	dlgMsgBox;

				dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
				dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
				dlgMsgBox.SetMessage(FALSE, 
					_T("NG트레이인터락이 OFF가 아닙니다."),
					_T("NG tray Put Interlock Not Off"),		
					_T("NG tray Put Interlock Not Off"));

				dlgMsgBox.DoModal();
				break;
			}
		}
		break;
	case seqC_2:
		if (nCh == JIG_CH_1)
		{
			if ((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_VACOFF))
			{
				theUnitFunc.RobotCellVacOn(VAC_OFF,E_UD_ROBOT);
				m_nCSeq++;
				m_Timer.Start();
			}
			else if(m_Timer.Stop(FALSE) > 5)
			{	
				theUnitFunc.UDRobotBitAllOff(E_PUT_1);
				m_nCSeq = seqC_idle;
				CGxMsgBox	dlgMsgBox;

				dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
				dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
				dlgMsgBox.SetMessage(FALSE, 
					_T("VAC OFF가 ON이 아닙니다."),
					_T("Vac Off Not On"),		
					_T("Vac Off Not On"));

				dlgMsgBox.DoModal();
				break;
			}
		}
		else 
		{
			if ((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_VACOFF))
			{
				theDeviceMelsec.RobotBitOn(UDROBOT_XY_VACOFF);
				theUnitFunc.RobotCellVacOn(VAC_OFF,E_UD_ROBOT);
				m_nCSeq++;
				m_Timer.Start();
			}
			else if(m_Timer.Stop(FALSE) > 5)
			{	
				theUnitFunc.UDRobotBitAllOff(E_PUT_2);
				m_nCSeq = seqC_idle;
				CGxMsgBox	dlgMsgBox;

				dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
				dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
				dlgMsgBox.SetMessage(FALSE, 
					_T("VAC OFF가 ON이 아닙니다."),
					_T("Vac Off Not On"),		
					_T("Vac Off Not On"));

				dlgMsgBox.DoModal();
				break;
			}
		}
		break;
	case seqC_3:
		if (theUnitFunc.RobotCellVacOnChk(E_UD_ROBOT,VAC_OFF))
		{
			theUnitFunc.RobotBlowOff(E_UD_ROBOT);
			theDeviceMelsec.RobotBitOn(UDROBOT_XY_VACOFF);
			theDeviceMelsec.RobotBitOff(UDROBOT_XY_VACON);
			m_nCSeq++;
			m_Timer.Start();
		}
		else if(m_Timer.Stop(FALSE) > 3)
		{	
			if (nCh == JIG_CH_1)
				theUnitFunc.UDRobotBitAllOff(E_PUT_1);
			else
				theUnitFunc.UDRobotBitAllOff(E_PUT_2);

			m_nCSeq = seqC_idle;
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("VAC OFF가 아닙니다."),
				_T("Vac Not Off"),		
				_T("Vac Not Off"));

			dlgMsgBox.DoModal();
			break;
		}
		break;
	case seqC_4:
		if (nCh == JIG_CH_1)
		{
			if ((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_OKTRAYPUTCOMP))
			{
				m_nCSeq++;
				m_Timer.Start();
			}
			else if(m_Timer.Stop(FALSE) > 3)
			{	
				theUnitFunc.UDRobotBitAllOff(E_PUT_1);
				m_nCSeq = seqC_idle;
				CGxMsgBox	dlgMsgBox;

				dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
				dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
				dlgMsgBox.SetMessage(FALSE, 
					_T("양품트레이언로딩완료 아닙니다."),
					_T("Ok tray Put Comp Not On"),		
					_T("Ok tray Put Comp Not On"));

				dlgMsgBox.DoModal();
				break;
			}
		}
		else
		{
			if ((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_NGTRAYPUTCOMP))
			{
				m_nCSeq++;
				m_Timer.Start();
			}
			else if(m_Timer.Stop(FALSE) > 3)
			{	
				theUnitFunc.UDRobotBitAllOff(E_PUT_2);
				m_nCSeq = seqC_idle;
				CGxMsgBox	dlgMsgBox;

				dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
				dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
				dlgMsgBox.SetMessage(FALSE, 
					_T("NG트레이언로딩완료 아닙니다."),
					_T("NG tray Put Comp Not On"),		
					_T("NG tray Put Comp Not On"));

				dlgMsgBox.DoModal();
				break;
			}
		}
		break;
	case seqC_5:
		if (nCh == JIG_CH_1)
		{
			if ((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_OKTRAYPUT))
			{
				theUnitFunc.UDRobotBitAllOff(E_PUT_1);
				m_nCSeq = seqC_idle;
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
			else if(m_Timer.Stop(FALSE) > 6)
			{	
				theUnitFunc.UDRobotBitAllOff(E_PUT_1);
				m_nCSeq = seqC_idle;
				CGxMsgBox	dlgMsgBox;

				dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
				dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
				dlgMsgBox.SetMessage(FALSE, 
					_T("양품트레이인터락이 ON이 아닙니다."),
					_T("Tray Put Interlock Not On"),		
					_T("Tray Put Interlock Not On"));

				dlgMsgBox.DoModal();
				break;
			}
		}
		else
		{
			if ((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_NGTRAYPUT))
			{
				theUnitFunc.UDRobotBitAllOff(E_PUT_2);
				m_nCSeq = seqC_idle;
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
			else if(m_Timer.Stop(FALSE) > 6)
			{	
				theUnitFunc.UDRobotBitAllOff(E_PUT_2);
				m_nCSeq = seqC_idle;
				CGxMsgBox	dlgMsgBox;

				dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
				dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
				dlgMsgBox.SetMessage(FALSE, 
					_T("NG트레이인터락이 ON이 아닙니다."),
					_T("NG Tray Put Interlock Not On"),		
					_T("NG Tray Put Interlock Not On"));

				dlgMsgBox.DoModal();
				break;
			}
		}
		break;

	default:
		break;
	}
}


void CFormTeachUnloaderTR::ClickGxbtnUnldRobotTrayPitch()
{
	theSocketInterFace.SendMsgToRobot(E_UD_ROBOT);
}


void CFormTeachUnloaderTR::ClickGxbtnUldRobotUldstgCh1GetOk()
{
	//2017-12-13,SKCHO 안전사항 추가
	if(theUnitFunc.DoorInterLockChkStart() == FALSE)
		return ;
	bCh1OK = TRUE;
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_ULD_ROBOT_ULDSTG_CH1_GET_OK, GXCOLOR_ON);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_ULD_ROBOT_ULDSTG_CH1_GET_NG, GXCOLOR_OFF);
}


void CFormTeachUnloaderTR::ClickGxbtnUldRobotUldstgCh1GetNg()
{
	//2017-12-13,SKCHO 안전사항 추가
	if(theUnitFunc.DoorInterLockChkStart() == FALSE)
		return ;
	
	bCh1OK = FALSE;
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_ULD_ROBOT_ULDSTG_CH1_GET_OK, GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_ULD_ROBOT_ULDSTG_CH1_GET_NG, GXCOLOR_ON);
}


void CFormTeachUnloaderTR::ClickGxbtnUldRobotUldstgCh2GetOk()
{
	//2017-12-13,SKCHO 안전사항 추가
	if(theUnitFunc.DoorInterLockChkStart() == FALSE)
		return ;
	bCh2OK = TRUE;
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_ULD_ROBOT_ULDSTG_CH2_GET_OK, GXCOLOR_ON);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_ULD_ROBOT_ULDSTG_CH2_GET_NG, GXCOLOR_OFF);
}


void CFormTeachUnloaderTR::ClickGxbtnUldRobotUldstgCh2GetNg()
{
	//2017-12-13,SKCHO 안전사항 추가
	if(theUnitFunc.DoorInterLockChkStart() == FALSE)
		return ;
	bCh2OK = FALSE;
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_ULD_ROBOT_ULDSTG_CH2_GET_OK, GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_ULD_ROBOT_ULDSTG_CH2_GET_NG, GXCOLOR_ON);
}
