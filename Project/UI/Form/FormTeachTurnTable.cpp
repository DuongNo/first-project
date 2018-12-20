// FormTeachTurnTable.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "resource.h"
#include "FormTeachTurnTable.h"

#include "UI\GausGUI\GxUICtrl.h"

// CFormTeachTurnTable

IMPLEMENT_DYNCREATE(CFormTeachTurnTable, CFormView)

CFormTeachTurnTable::CFormTeachTurnTable()
	: CFormView(CFormTeachTurnTable::IDD)
{

}

CFormTeachTurnTable::~CFormTeachTurnTable()
{
}

void CFormTeachTurnTable::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFormTeachTurnTable, CFormView)
	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CFormTeachTurnTable 진단입니다.

#ifdef _DEBUG
void CFormTeachTurnTable::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormTeachTurnTable::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


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
LRESULT CFormTeachTurnTable::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{
		UpdateTurnTableState();
		SetTimer(1, 100, NULL);

	}
	else
	{
		KillTimer(1);
	}

	return 0;
}

// 표시 언어가 변경되었다.
LRESULT CFormTeachTurnTable::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	CGxUICtrl::BroadcastStaticCpationIndex(this, wParam, IDC_GXSTC_WORKT_TITLE_A);
	CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_GXBTN_WORKT_A_CH1_VAC_ON);

	return 0;
}

BOOL CFormTeachTurnTable::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style = WS_OVERLAPPED | WS_CHILD;				// 테두리를 없앤다.

	return CFormView::PreCreateWindow(cs);
}
BEGIN_EVENTSINK_MAP(CFormTeachTurnTable, CFormView)
	ON_EVENT(CFormTeachTurnTable, IDC_GXBTN_WORKT_B_CAM_CH1_LX, DISPID_CLICK, CFormTeachTurnTable::ClickGxbtnWorktBCamCh1Lx, VTS_NONE)
	ON_EVENT(CFormTeachTurnTable, IDC_GXBTN_WORKT_A_CH1_VAC_ON, DISPID_CLICK, CFormTeachTurnTable::ClickGxbtnWorktACh1VacOn, VTS_NONE)
	ON_EVENT(CFormTeachTurnTable, IDC_GXBTN_WORKT_A_CH1_VAC_OFF, DISPID_CLICK, CFormTeachTurnTable::ClickGxbtnWorktACh1VacOff, VTS_NONE)
	ON_EVENT(CFormTeachTurnTable, IDC_GXBTN_WORKT_A_CH2_VAC_ON, DISPID_CLICK, CFormTeachTurnTable::ClickGxbtnWorktACh2VacOn, VTS_NONE)
	ON_EVENT(CFormTeachTurnTable, IDC_GXBTN_WORKT_A_CH2_VAC_OFF, DISPID_CLICK, CFormTeachTurnTable::ClickGxbtnWorktACh2VacOff, VTS_NONE)
	ON_EVENT(CFormTeachTurnTable, IDC_GXBTN_WORKT_B_CH1_VAC_ON, DISPID_CLICK, CFormTeachTurnTable::ClickGxbtnWorktBCh1VacOn, VTS_NONE)
	ON_EVENT(CFormTeachTurnTable, IDC_GXBTN_WORKT_B_CH1_VAC_OFF, DISPID_CLICK, CFormTeachTurnTable::ClickGxbtnWorktBCh1VacOff, VTS_NONE)
	ON_EVENT(CFormTeachTurnTable, IDC_GXBTN_WORKT_B_CH2_VAC_ON, DISPID_CLICK, CFormTeachTurnTable::ClickGxbtnWorktBCh2VacOn, VTS_NONE)
	ON_EVENT(CFormTeachTurnTable, IDC_GXBTN_WORKT_B_CH2_VAC_OFF, DISPID_CLICK, CFormTeachTurnTable::ClickGxbtnWorktBCh2VacOff, VTS_NONE)
	ON_EVENT(CFormTeachTurnTable, IDC_GXBTN_WORKT_C_CH1_VAC_ON, DISPID_CLICK, CFormTeachTurnTable::ClickGxbtnWorktCCh1VacOn, VTS_NONE)
	ON_EVENT(CFormTeachTurnTable, IDC_GXBTN_WORKT_C_CH1_VAC_OFF, DISPID_CLICK, CFormTeachTurnTable::ClickGxbtnWorktCCh1VacOff, VTS_NONE)
	ON_EVENT(CFormTeachTurnTable, IDC_GXBTN_WORKT_C_CH2_VAC_ON, DISPID_CLICK, CFormTeachTurnTable::ClickGxbtnWorktCCh2VacOn, VTS_NONE)
	ON_EVENT(CFormTeachTurnTable, IDC_GXBTN_WORKT_C_CH2_VAC_OFF, DISPID_CLICK, CFormTeachTurnTable::ClickGxbtnWorktCCh2VacOff, VTS_NONE)
	ON_EVENT(CFormTeachTurnTable, IDC_GXBTN_WORKT_D_CH1_VAC_ON, DISPID_CLICK, CFormTeachTurnTable::ClickGxbtnWorktDCh1VacOn, VTS_NONE)
	ON_EVENT(CFormTeachTurnTable, IDC_GXBTN_WORKT_D_CH1_VAC_OFF, DISPID_CLICK, CFormTeachTurnTable::ClickGxbtnWorktDCh1VacOff, VTS_NONE)
	ON_EVENT(CFormTeachTurnTable, IDC_GXBTN_WORKT_D_CH2_VAC_ON, DISPID_CLICK, CFormTeachTurnTable::ClickGxbtnWorktDCh2VacOn, VTS_NONE)
	ON_EVENT(CFormTeachTurnTable, IDC_GXBTN_WORKT_D_CH2_VAC_OFF, DISPID_CLICK, CFormTeachTurnTable::ClickGxbtnWorktDCh2VacOff, VTS_NONE)
	ON_EVENT(CFormTeachTurnTable, IDC_GXBTN_WORKT_START, DISPID_CLICK, CFormTeachTurnTable::ClickGxbtnWorktStart, VTS_NONE)
	ON_EVENT(CFormTeachTurnTable, IDC_GXBTN_WORKT_NEXT_MOVE, DISPID_CLICK, CFormTeachTurnTable::ClickGxbtnWorktNextMove, VTS_NONE)
	ON_EVENT(CFormTeachTurnTable, IDC_GXBTN_WORKT_B_CAM_CH1_RX, DISPID_CLICK, CFormTeachTurnTable::ClickGxbtnWorktBCamCh1Rx, VTS_NONE)
	ON_EVENT(CFormTeachTurnTable, IDC_GXBTN_WORKT_B_CAM_CH2_LX, DISPID_CLICK, CFormTeachTurnTable::ClickGxbtnWorktBCamCh2Lx, VTS_NONE)
	ON_EVENT(CFormTeachTurnTable, IDC_GXBTN_WORKT_B_CAM_CH2_RX, DISPID_CLICK, CFormTeachTurnTable::ClickGxbtnWorktBCamCh2Rx, VTS_NONE)
	ON_EVENT(CFormTeachTurnTable, IDC_GXBTN_WORKT_B_CAM_CH1_LY, DISPID_CLICK, CFormTeachTurnTable::ClickGxbtnWorktBCamCh1Ly, VTS_NONE)
	ON_EVENT(CFormTeachTurnTable, IDC_GXBTN_WORKT_B_CAM_CH1_RY, DISPID_CLICK, CFormTeachTurnTable::ClickGxbtnWorktBCamCh1Ry, VTS_NONE)
	ON_EVENT(CFormTeachTurnTable, IDC_GXBTN_WORKT_B_CAM_CH2_LY, DISPID_CLICK, CFormTeachTurnTable::ClickGxbtnWorktBCamCh2Ly, VTS_NONE)
	ON_EVENT(CFormTeachTurnTable, IDC_GXBTN_WORKT_B_CAM_CH2_RY, DISPID_CLICK, CFormTeachTurnTable::ClickGxbtnWorktBCamCh2Ry, VTS_NONE)
	ON_EVENT(CFormTeachTurnTable, IDC_GXBTN_WORKT_C_CAM_X, DISPID_CLICK, CFormTeachTurnTable::ClickGxbtnWorktCCamX, VTS_NONE)
	ON_EVENT(CFormTeachTurnTable, IDC_GXBTN_WORKT_C_LCAM_ZOOM, DISPID_CLICK, CFormTeachTurnTable::ClickGxbtnWorktCLcamZoom, VTS_NONE)
	ON_EVENT(CFormTeachTurnTable, IDC_GXBTN_WORKT_C_LCAM_THETA, DISPID_CLICK, CFormTeachTurnTable::ClickGxbtnWorktCLcamTheta, VTS_NONE)
	ON_EVENT(CFormTeachTurnTable, IDC_GXBTN_WORKT_C_LCAM_FOCUS, DISPID_CLICK, CFormTeachTurnTable::ClickGxbtnWorktCLcamFocus, VTS_NONE)
	ON_EVENT(CFormTeachTurnTable, IDC_GXBTN_WORKT_C_RCAM_ZOOM, DISPID_CLICK, CFormTeachTurnTable::ClickGxbtnWorktCRcamZoom, VTS_NONE)
	ON_EVENT(CFormTeachTurnTable, IDC_GXBTN_WORKT_C_RCAM_THETA, DISPID_CLICK, CFormTeachTurnTable::ClickGxbtnWorktCRcamTheta, VTS_NONE)
	ON_EVENT(CFormTeachTurnTable, IDC_GXBTN_WORKT_C_RCAM_FOCUS, DISPID_CLICK, CFormTeachTurnTable::ClickGxbtnWorktCRcamFocus, VTS_NONE)
	ON_EVENT(CFormTeachTurnTable, IDC_GXBTN_WORKT_B_CAM_Y_READY, DISPID_CLICK, CFormTeachTurnTable::ClickGxbtnWorktBCamYReady, VTS_NONE)
	ON_EVENT(CFormTeachTurnTable, IDC_GXBTN_WORKT_B_CAM_X_READY, DISPID_CLICK, CFormTeachTurnTable::ClickGxbtnWorktBCamXReady, VTS_NONE)
	ON_EVENT(CFormTeachTurnTable, IDC_GXBTN_WORKT_B_CAM_CH1L_ALIGN, DISPID_CLICK, CFormTeachTurnTable::ClickGxbtnWorktBCamCh1lAlign, VTS_NONE)
	ON_EVENT(CFormTeachTurnTable, IDC_GXBTN_WORKT_B_CAM_CH1R_ALIGN, DISPID_CLICK, CFormTeachTurnTable::ClickGxbtnWorktBCamCh1rAlign, VTS_NONE)
	ON_EVENT(CFormTeachTurnTable, IDC_GXBTN_WORKT_B_CAM_CH2L_ALIGN, DISPID_CLICK, CFormTeachTurnTable::ClickGxbtnWorktBCamCh2lAlign, VTS_NONE)
	ON_EVENT(CFormTeachTurnTable, IDC_GXBTN_WORKT_B_CAM_CH2R_ALIGN, DISPID_CLICK, CFormTeachTurnTable::ClickGxbtnWorktBCamCh2rAlign, VTS_NONE)
END_EVENTSINK_MAP()

void CFormTeachTurnTable::UpdateTurnTableState()
{
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_WORKT_B_CAM_CH1_LX, theUnitFunc.B_Zone_Cam_X_Check(JIG_CH_1, CAMPOS_LEFT) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_WORKT_B_CAM_CH1_RX, theUnitFunc.B_Zone_Cam_X_Check(JIG_CH_1, CAMPOS_RIGHT) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_WORKT_B_CAM_CH2_LX, theUnitFunc.B_Zone_Cam_X_Check(JIG_CH_2, CAMPOS_LEFT) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_WORKT_B_CAM_CH2_RX, theUnitFunc.B_Zone_Cam_X_Check(JIG_CH_2, CAMPOS_RIGHT) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_WORKT_B_CAM_CH1_LY, theUnitFunc.B_Zone_Cam_Y_Check(JIG_CH_1, CAMPOS_LEFT) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_WORKT_B_CAM_CH1_RY, theUnitFunc.B_Zone_Cam_Y_Check(JIG_CH_1, CAMPOS_RIGHT) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_WORKT_B_CAM_CH2_LY, theUnitFunc.B_Zone_Cam_Y_Check(JIG_CH_2, CAMPOS_LEFT) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_WORKT_B_CAM_CH2_RY, theUnitFunc.B_Zone_Cam_Y_Check(JIG_CH_2, CAMPOS_RIGHT) ? GXCOLOR_ON:GXCOLOR_OFF);
	
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_WORKT_B_CAM_Y_READY, theUnitFunc.B_Zone_Cam_Y_Ready_Chk() ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_WORKT_B_CAM_X_READY, theUnitFunc.B_Zone_Cam_X_Ready_Chk() ? GXCOLOR_ON:GXCOLOR_OFF);

	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_WORKT_C_CAM_X, theUnitFunc.C_Zone_CamXChk() ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_WORKT_C_LCAM_ZOOM, theUnitFunc.C_Zone_LCamZChk() ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_WORKT_C_LCAM_THETA, theUnitFunc.C_Zone_LCam_ThetaChk() ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_WORKT_C_LCAM_FOCUS, theUnitFunc.C_Zone_LCam_FocusChk() ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_WORKT_C_RCAM_ZOOM, theUnitFunc.C_Zone_RCamZChk() ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_WORKT_C_RCAM_THETA, theUnitFunc.C_Zone_RCam_ThetaChk() ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_WORKT_C_RCAM_FOCUS, theUnitFunc.C_Zone_RCam_FocusChk() ? GXCOLOR_ON:GXCOLOR_OFF);

	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_WORKT_A_CH1_VAC_ON, (theUnitFunc.AMTVacChk(JIG_ID_A, JIG_CH_1) ? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_WORKT_A_CH1_VAC_OFF, (!theUnitFunc.AMTVacChk(JIG_ID_A, JIG_CH_1) ? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_WORKT_A_CH2_VAC_ON, (theUnitFunc.AMTVacChk(JIG_ID_A, JIG_CH_2) ? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_WORKT_A_CH2_VAC_OFF, (!theUnitFunc.AMTVacChk(JIG_ID_A, JIG_CH_2) ? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_WORKT_B_CH1_VAC_ON, (theUnitFunc.AMTVacChk(JIG_ID_B, JIG_CH_1) ? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_WORKT_B_CH1_VAC_OFF, (!theUnitFunc.AMTVacChk(JIG_ID_B, JIG_CH_1) ? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_WORKT_B_CH2_VAC_ON, (theUnitFunc.AMTVacChk(JIG_ID_B, JIG_CH_2) ? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_WORKT_B_CH2_VAC_OFF, (!theUnitFunc.AMTVacChk(JIG_ID_B, JIG_CH_2) ? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_WORKT_C_CH1_VAC_ON, (theUnitFunc.AMTVacChk(JIG_ID_C, JIG_CH_1) ? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_WORKT_C_CH1_VAC_OFF, (!theUnitFunc.AMTVacChk(JIG_ID_C, JIG_CH_1) ? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_WORKT_C_CH2_VAC_ON, (theUnitFunc.AMTVacChk(JIG_ID_C, JIG_CH_2) ? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_WORKT_C_CH2_VAC_OFF, (!theUnitFunc.AMTVacChk(JIG_ID_C, JIG_CH_2) ? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_WORKT_D_CH1_VAC_ON, (theUnitFunc.AMTVacChk(JIG_ID_D, JIG_CH_1) ? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_WORKT_D_CH1_VAC_OFF, (!theUnitFunc.AMTVacChk(JIG_ID_D, JIG_CH_1) ? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_WORKT_D_CH2_VAC_ON, (theUnitFunc.AMTVacChk(JIG_ID_D, JIG_CH_2) ? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_WORKT_D_CH2_VAC_OFF, (!theUnitFunc.AMTVacChk(JIG_ID_D, JIG_CH_2) ? GXCOLOR_ON:GXCOLOR_OFF));

	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_WORKT_START, (theUnitFunc.GetEntryTablePos() == CONST_TABLE_POS::POS_1 ? GXCOLOR_ON:GXCOLOR_OFF));

	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_WORKT_BPOS, (theUnitFunc.GetEntryTablePos() == CONST_TABLE_POS::POS_4 ? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_WORKT_CPOS, (theUnitFunc.GetEntryTablePos() == CONST_TABLE_POS::POS_3 ? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_WORKT_DPOS, (theUnitFunc.GetEntryTablePos() == CONST_TABLE_POS::POS_2 ? GXCOLOR_ON:GXCOLOR_OFF));

}

void CFormTeachTurnTable::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	KillTimer(nIDEvent);

	switch(nIDEvent)
	{
	case 1:
		UpdateTurnTableState();
		CString str;
		str.Format(_T("%f"),theProcBank.m_dVI_MITAlignTH);
		CGxUICtrl::SetStaticString(this, IDC_GXSTC_VI_ALIGN_DATA, str);
		
		SetTimer(1, 100, NULL);
		break;
	}

	CFormView::OnTimer(nIDEvent);
}


void CFormTeachTurnTable::ClickGxbtnWorktACh1VacOn()
{
	theUnitFunc.AMTVacOn(TRUE, JIG_ID_A, JIG_CH_1);
}


void CFormTeachTurnTable::ClickGxbtnWorktACh1VacOff()
{
	theUnitFunc.AMTVacOn(FALSE, JIG_ID_A, JIG_CH_1);
}


void CFormTeachTurnTable::ClickGxbtnWorktACh2VacOn()
{
	theUnitFunc.AMTVacOn(TRUE, JIG_ID_A, JIG_CH_2);
}


void CFormTeachTurnTable::ClickGxbtnWorktACh2VacOff()
{
	theUnitFunc.AMTVacOn(FALSE, JIG_ID_A, JIG_CH_2);
}


void CFormTeachTurnTable::ClickGxbtnWorktBCh1VacOn()
{
	theUnitFunc.AMTVacOn(TRUE, JIG_ID_B, JIG_CH_1);
}


void CFormTeachTurnTable::ClickGxbtnWorktBCh1VacOff()
{
	theUnitFunc.AMTVacOn(FALSE, JIG_ID_B, JIG_CH_1);
}


void CFormTeachTurnTable::ClickGxbtnWorktBCh2VacOn()
{
	theUnitFunc.AMTVacOn(TRUE, JIG_ID_B, JIG_CH_2);
}


void CFormTeachTurnTable::ClickGxbtnWorktBCh2VacOff()
{
	theUnitFunc.AMTVacOn(FALSE, JIG_ID_B, JIG_CH_2);
}


void CFormTeachTurnTable::ClickGxbtnWorktCCh1VacOn()
{
	theUnitFunc.AMTVacOn(TRUE, JIG_ID_C, JIG_CH_1);
}


void CFormTeachTurnTable::ClickGxbtnWorktCCh1VacOff()
{
	theUnitFunc.AMTVacOn(FALSE, JIG_ID_C, JIG_CH_1);
}


void CFormTeachTurnTable::ClickGxbtnWorktCCh2VacOn()
{
	theUnitFunc.AMTVacOn(TRUE, JIG_ID_C, JIG_CH_2);
}


void CFormTeachTurnTable::ClickGxbtnWorktCCh2VacOff()
{
	theUnitFunc.AMTVacOn(FALSE, JIG_ID_C, JIG_CH_2);
}


void CFormTeachTurnTable::ClickGxbtnWorktDCh1VacOn()
{
	theUnitFunc.AMTVacOn(TRUE, JIG_ID_D, JIG_CH_1);
}


void CFormTeachTurnTable::ClickGxbtnWorktDCh1VacOff()
{
	theUnitFunc.AMTVacOn(FALSE, JIG_ID_D, JIG_CH_1);
}


void CFormTeachTurnTable::ClickGxbtnWorktDCh2VacOn()
{
	theUnitFunc.AMTVacOn(TRUE, JIG_ID_D, JIG_CH_2);
}


void CFormTeachTurnTable::ClickGxbtnWorktDCh2VacOff()
{
	theUnitFunc.AMTVacOn(FALSE, JIG_ID_D, JIG_CH_2);
}


void CFormTeachTurnTable::OnInitialUpdate()
{
	if(m_bInit)	return;	// OnInitialUpdate 두번 실행되는 버그 방지 [10/24/2016 OSC]
	m_bInit = TRUE;
	CFormView::OnInitialUpdate();

	m_CurAxis = (AXIS_ID)AXIS_SVO_TURN_TABLE_T;
	m_CurTeach = TEACH_PARAM::WORK_TABLE_T_TO_WORK;
	m_CurID	  = IDC_GXBTN_WORKT_START;
	SetCurPageAxis(m_CurAxis, m_CurTeach, (UINT)m_CurID);
}

void CFormTeachTurnTable::SetCurPageAxis(AXIS_ID axis, TEACH_PARAM::ID param, UINT nID)
{
	m_CurAxis = axis;
	m_CurTeach = param;
	m_CurID	  = nID;	

	GetMainHandler()->SetSelectedAxis(m_CurAxis, m_CurTeach);
}

void CFormTeachTurnTable::ClickGxbtnWorktStart()
{
	theProcBank.m_bTurnStart = TRUE;
	m_CurAxis = (AXIS_ID)AXIS_SVO_TURN_TABLE_T;
	m_CurTeach = TEACH_PARAM::WORK_TABLE_T_TO_WORK;
	SetCurPageAxis(m_CurAxis, m_CurTeach, (UINT)IDC_GXBTN_WORKT_START);
	theUnitFunc.SetOutPutIO(Y_SWITCH_1,TRUE);
	theUnitFunc.SetOutPutIO(Y_SWITCH_2,TRUE);

}

void CFormTeachTurnTable::ClickGxbtnWorktNextMove()
{
	theProcBank.m_bTurnStart = FALSE;
	m_CurAxis = (AXIS_ID)AXIS_SVO_TURN_TABLE_T;
	m_CurTeach = TEACH_PARAM::WORK_TABLE_T_TO_WORK;
	SetCurPageAxis(m_CurAxis, m_CurTeach, (UINT)IDC_GXBTN_WORKT_NEXT_MOVE);
}

void CFormTeachTurnTable::ClickGxbtnWorktBCamCh1Lx()
{
	m_CurAxis = (AXIS_ID)AXIS_STP_ALIGN_CAM_X;
	m_CurTeach = TEACH_PARAM::TT_VALIGN_CAM_CH1_X1;
	SetCurPageAxis(m_CurAxis, m_CurTeach, (UINT)IDC_GXBTN_WORKT_B_CAM_CH1_LX);
}

void CFormTeachTurnTable::ClickGxbtnWorktBCamCh1Rx()
{
	m_CurAxis = (AXIS_ID)AXIS_STP_ALIGN_CAM_X;
	m_CurTeach = TEACH_PARAM::TT_VALIGN_CAM_CH1_X2;
	SetCurPageAxis(m_CurAxis, m_CurTeach, (UINT)IDC_GXBTN_WORKT_B_CAM_CH1_RX);
}

void CFormTeachTurnTable::ClickGxbtnWorktBCamCh2Lx()
{
	m_CurAxis = (AXIS_ID)AXIS_STP_ALIGN_CAM_X;
	m_CurTeach = TEACH_PARAM::TT_VALIGN_CAM_CH2_X1;
	SetCurPageAxis(m_CurAxis, m_CurTeach, (UINT)IDC_GXBTN_WORKT_B_CAM_CH2_LX);
}

void CFormTeachTurnTable::ClickGxbtnWorktBCamCh2Rx()
{
	m_CurAxis = (AXIS_ID)AXIS_STP_ALIGN_CAM_X;
	m_CurTeach = TEACH_PARAM::TT_VALIGN_CAM_CH2_X2;
	SetCurPageAxis(m_CurAxis, m_CurTeach, (UINT)IDC_GXBTN_WORKT_B_CAM_CH2_RX);
}

void CFormTeachTurnTable::ClickGxbtnWorktBCamCh1Ly()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_ALIGN_CAM_Y;
	m_CurTeach = TEACH_PARAM::TT_VALIGN_CAM_CH1_Y1;
	SetCurPageAxis(m_CurAxis, m_CurTeach, (UINT)IDC_GXBTN_WORKT_B_CAM_CH1_LY);
}

void CFormTeachTurnTable::ClickGxbtnWorktBCamCh1Ry()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_ALIGN_CAM_Y;
	m_CurTeach = TEACH_PARAM::TT_VALIGN_CAM_CH1_Y2;
	SetCurPageAxis(m_CurAxis, m_CurTeach, (UINT)IDC_GXBTN_WORKT_B_CAM_CH1_RY);
}

void CFormTeachTurnTable::ClickGxbtnWorktBCamCh2Ly()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_ALIGN_CAM_Y;
	m_CurTeach = TEACH_PARAM::TT_VALIGN_CAM_CH2_Y1;
	SetCurPageAxis(m_CurAxis, m_CurTeach, (UINT)IDC_GXBTN_WORKT_B_CAM_CH2_LY);
}

void CFormTeachTurnTable::ClickGxbtnWorktBCamCh2Ry()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_ALIGN_CAM_Y;
	m_CurTeach = TEACH_PARAM::TT_VALIGN_CAM_CH2_Y2;
	SetCurPageAxis(m_CurAxis, m_CurTeach, (UINT)IDC_GXBTN_WORKT_B_CAM_CH2_RY);
}

void CFormTeachTurnTable::ClickGxbtnWorktCCamX()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_INS_X;
	m_CurTeach = TEACH_PARAM::TT_INSP_CAM_X;
	SetCurPageAxis(m_CurAxis, m_CurTeach, (UINT)IDC_GXBTN_WORKT_C_CAM_X);
}

void CFormTeachTurnTable::ClickGxbtnWorktCLcamZoom()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_INS_CH1_Z;
	m_CurTeach = TEACH_PARAM::TT_INSP_CH1_CAM_ZOOM_Z;
	SetCurPageAxis(m_CurAxis, m_CurTeach, (UINT)IDC_GXBTN_WORKT_C_LCAM_ZOOM);
}

void CFormTeachTurnTable::ClickGxbtnWorktCLcamTheta()
{
	m_CurAxis = (AXIS_ID)AXIS_STP_INS_CH1_T;
	m_CurTeach = TEACH_PARAM::TT_INSP_CH1_CAM_TURN_T;
	SetCurPageAxis(m_CurAxis, m_CurTeach, (UINT)IDC_GXBTN_WORKT_C_LCAM_THETA);
}

void CFormTeachTurnTable::ClickGxbtnWorktCLcamFocus()
{
	m_CurAxis = (AXIS_ID)AXIS_STP_INS_CH1_F;
	m_CurTeach = TEACH_PARAM::TT_INSP_CH1_CAM_FOCUS_F;
	SetCurPageAxis(m_CurAxis, m_CurTeach, (UINT)IDC_GXBTN_WORKT_C_LCAM_FOCUS);
}


void CFormTeachTurnTable::ClickGxbtnWorktCRcamZoom()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_INS_CH2_Z;
	m_CurTeach = TEACH_PARAM::TT_INSP_CH2_CAM_ZOOM_Z;
	SetCurPageAxis(m_CurAxis, m_CurTeach, (UINT)IDC_GXBTN_WORKT_C_RCAM_ZOOM);
}


void CFormTeachTurnTable::ClickGxbtnWorktCRcamTheta()
{
	m_CurAxis = (AXIS_ID)AXIS_STP_INS_CH2_T;
	m_CurTeach = TEACH_PARAM::TT_INSP_CH2_CAM_TURN_T;
	SetCurPageAxis(m_CurAxis, m_CurTeach, (UINT)IDC_GXBTN_WORKT_C_RCAM_THETA);
}


void CFormTeachTurnTable::ClickGxbtnWorktCRcamFocus()
{
	m_CurAxis = (AXIS_ID)AXIS_STP_INS_CH2_F;
	m_CurTeach = TEACH_PARAM::TT_INSP_CH2_CAM_FOCUS_F;
	SetCurPageAxis(m_CurAxis, m_CurTeach, (UINT)IDC_GXBTN_WORKT_C_RCAM_FOCUS);
}


void CFormTeachTurnTable::ClickGxbtnWorktBCamYReady()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_ALIGN_CAM_Y;
	m_CurTeach = TEACH_PARAM::TT_VALIGN_CAM_Y_READY;
	SetCurPageAxis(m_CurAxis, m_CurTeach, (UINT)IDC_GXBTN_WORKT_B_CAM_Y_READY);
}


void CFormTeachTurnTable::ClickGxbtnWorktBCamXReady()
{
	m_CurAxis = (AXIS_ID)AXIS_STP_ALIGN_CAM_X;
	m_CurTeach = TEACH_PARAM::TT_VALIGN_CAM_X_READY;
	SetCurPageAxis(m_CurAxis, m_CurTeach, (UINT)IDC_GXBTN_WORKT_B_CAM_X_READY);
}


void CFormTeachTurnTable::ClickGxbtnWorktBCamCh1lAlign()
{
	theUnitFunc.B_Zone_Align_Grab(JIG_CH_1, TRUE , TRUE);	
}


void CFormTeachTurnTable::ClickGxbtnWorktBCamCh1rAlign()
{
	theUnitFunc.B_Zone_Align_Grab(JIG_CH_1, FALSE, TRUE);	
}


void CFormTeachTurnTable::ClickGxbtnWorktBCamCh2lAlign()
{
	theUnitFunc.B_Zone_Align_Grab(JIG_CH_2, TRUE, TRUE);	
}


void CFormTeachTurnTable::ClickGxbtnWorktBCamCh2rAlign()
{
	theUnitFunc.B_Zone_Align_Grab(JIG_CH_2, FALSE, TRUE);	
}
