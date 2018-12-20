// FormTeachUDRobot.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "resource.h"
#include "FormTeachUDRobot.h"

#include "GUIDefine.h"

//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "UI\GausGUI\GxButtonEx.h"
#include "UI\GausGUI\GxStaticEx.h"

#include "UI\GausGUI\GxMsgBox.h"
// CFormTeachUDRobot

IMPLEMENT_DYNCREATE(CFormTeachUDRobot, CFormView)

	CFormTeachUDRobot::CFormTeachUDRobot()
	: CFormView(CFormTeachUDRobot::IDD)
{
	m_bInit = FALSE;

	for(int i = 0 ; i < 100; i++)
	{
		bStatus[i] = FALSE;
	}
}

CFormTeachUDRobot::~CFormTeachUDRobot()
{
}

void CFormTeachUDRobot::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFormTeachUDRobot, CFormView)
	ON_WM_TIMER()
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)			// 표시 언어가 바뀌었다.
END_MESSAGE_MAP()


// CFormTeachUDRobot 진단입니다.

#ifdef _DEBUG
void CFormTeachUDRobot::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormTeachUDRobot::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormTeachUDRobot 메시지 처리기입니다.

BOOL CFormTeachUDRobot::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style = WS_OVERLAPPED | WS_CHILD;				// 테두리를 없앤다.

	return CFormView::PreCreateWindow(cs);
}

BEGIN_EVENTSINK_MAP(CFormTeachUDRobot, CFormView)

	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT2, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput2, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT3, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput3, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT4, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput4, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT5, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput5, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT6, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput6, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT7, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput7, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT8, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput8, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT9, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput9, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT10, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput10, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT11, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput11, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT12, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput12, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT13, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput13, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT14, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput14, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT15, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput15, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT16, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput16, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT17, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput17, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT18, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput18, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT19, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput19, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT20, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput20, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT21, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput21, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT22, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput22, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT23, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput23, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT24, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput24, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT25, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput25, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT26, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput26, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT27, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput27, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT28, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput28, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT29, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput29, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT30, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput30, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT31, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput31, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT32, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput32, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT33, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput33, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT34, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput34, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT35, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput35, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT36, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput36, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT37, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput37, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT38, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput38, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT39, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput39, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT40, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput40, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT41, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput41, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT42, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput42, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT43, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput43, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT44, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput44, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT45, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput45, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT46, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput46, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT47, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput47, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT48, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput48, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT49, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput49, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT50, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput50, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT51, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput51, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT52, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput52, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT53, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput53, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT54, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput54, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT55, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput55, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT56, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput56, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT57, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput57, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT58, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput58, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT59, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput59, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT60, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput60, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT61, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput61, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT62, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput62, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT63, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput63, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT64, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput64, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT65, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput65, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT66, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput66, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT67, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput67, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT68, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput68, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT69, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput69, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT70, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput70, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT71, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput71, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT72, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput72, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_OUTPUT73, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotOutput73, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_GET_APPROACH, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotGetApproach, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_GET_WAIT, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotGetWait, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_GET_CH1_UP, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotGetCh1Up, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_GET_CH1_DOWN, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotGetCh1Down, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_GET_CH2_UP, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotGetCh2Up, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_GET_CH2_DOWN, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotGetCh2Down, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_GOODPUT_APPROACH, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotGoodputApproach, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_GOODPUT_WAIT, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotGoodputWait, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_GOODPUT_UP, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotGoodputUp, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_GOODPUT_DOWN, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotGoodputDown, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_NGPUT_APPROACH, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotNgputApproach, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_NGPUT_WAIT, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotNgputWait, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_NGPUT_UP, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotNgputUp, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_NGPUT_DOWN, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotNgputDown, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UNLD_ROBOT_TRAY_00, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUnldRobotTray00, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UNLD_ROBOT_TRAY_01, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUnldRobotTray01, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UNLD_ROBOT_TRAY_02, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUnldRobotTray02, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UNLD_ROBOT_TRAY_03, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUnldRobotTray03, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UNLD_ROBOT_TRAY_10, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUnldRobotTray10, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UNLD_ROBOT_TRAY_11, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUnldRobotTray11, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UNLD_ROBOT_TRAY_12, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUnldRobotTray12, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UNLD_ROBOT_TRAY_13, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUnldRobotTray13, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UNLD_ROBOT_TRAY_20, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUnldRobotTray20, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UNLD_ROBOT_TRAY_21, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUnldRobotTray21, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UNLD_ROBOT_TRAY_22, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUnldRobotTray22, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UNLD_ROBOT_TRAY_23, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUnldRobotTray23, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UNLD_ROBOT_TRAY_30, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUnldRobotTray30, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UNLD_ROBOT_TRAY_31, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUnldRobotTray31, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UNLD_ROBOT_TRAY_32, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUnldRobotTray32, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UNLD_ROBOT_TRAY_33, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUnldRobotTray33, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_VAC_ON, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotVacOn, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_VAC_OFF, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotVacOff, VTS_NONE)
	ON_EVENT(CFormTeachUDRobot, IDC_GXBTN_UDROBOT_MOVE, DISPID_CLICK, CFormTeachUDRobot::ClickGxbtnUdrobotMove, VTS_NONE)
	END_EVENTSINK_MAP()


void CFormTeachUDRobot::OnInitialUpdate()
{
	if(m_bInit)	return;	// OnInitialUpdate 두번 실행되는 버그 방지 [10/24/2016 OSC]
	m_bInit = TRUE;
	CFormView::OnInitialUpdate();
}

LRESULT CFormTeachUDRobot::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{
		UpdateUDRobotState();
		theDeviceMelsec.RobotBitOn(UDROBOT_X_MANUALIN_Y_MANUALSELECT);
		Sleep(300);
		theSocketInterFace.SendMsgToRobot(E_UD_ROBOT);
		SetTimer(1, 100, NULL);
	}
	else
	{
		theDeviceMelsec.RobotBitOff(UDROBOT_X_MANUALIN_Y_MANUALSELECT);
		KillTimer(1);
	}
	return 0;
}

void CFormTeachUDRobot::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
	case 1:
		UpdateUDRobotState();
		break;
	}

	CFormView::OnTimer(nIDEvent);
}

LRESULT CFormTeachUDRobot::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

void CFormTeachUDRobot::UpdateUDRobotState()
{
	short Data[1];
	theDeviceMelsec.RobotWordRecvBuf(UDROBOT_MANUAL_POS,sizeof(short),Data);

	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_TEACHMODE_Y_EXTSTART			)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS2,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_PLAYMODE_Y_EXTSTOP			)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS3,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_RUNNING_Y_ERRRESET			)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS4,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_STOPPING_Y_EXTMOTORON			)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS5,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_INTERLOCKALARM_Y_EXTMOTOROFF	)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS6,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_MOTORON_Y_LOWSPEED			)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS7,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_PGMEND_Y_PAUSE				)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS8,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_ALARM_Y_EXTRESET				)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS9,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_BATWARN_Y_ORG					)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS10,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_HOMEPOS1_Y_CYCLESTOP			)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS11,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_HOMEPOS2_Y_PGMSTROBE			)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS12,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_PGMACK_Y_PGMSEL1				)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS13,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_EXTRESET_Y_PGMSEL2			)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS14,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_STATEOUT1_Y_PGMSEL3			)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS15,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_STATEOUT2_Y_PGMSEL4			)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS16,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_UNITREADY_Y_PGMSEL5			)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS17,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_BUSY_Y_PGMSEL6				)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS18,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_CYCLESTOP_Y_PGMSEL7			)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS19,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_ORGERR_Y_PGMSEL8				)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS20,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_DATAERR_Y_SPEEDIN1			)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS21,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_MEMORYERR_Y_SPEEDIN2			)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS22,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_MANUALIN_Y_MANUALSELECT		)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS23,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_MANUALCOMP_Y_MANUALGO		)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS27,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_MANUALMOVE_STANBY			)? GXCOLOR_ON:GXCOLOR_OFF));

	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS24,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_VACON						)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS25,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_VACOFF						)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS26,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_VACERR						)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS28,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_GETSTAGE					)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS29,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_STGCH1VACON					)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS30,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_STGCH1VACOFF				)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS31,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_STGCH2VACON_Y_STG1OK			)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS32,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_STGCH2VACOFF_Y_STG1NG		)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS33,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_STG1GETERR_Y_STG2VACOFF		)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS34,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_STG2GETERR_Y_STG2OK			)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS35,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_STGGETCOMP_Y_STG2NG			)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS36,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_NGTRAYPUT					)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS37,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_NGTRAYPUTERR					)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS38,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_NGTRAYPUTCOMP				)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS39,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_OKTRAYPUT					)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS40,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_OKTRAYPUTERR					)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS41,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_OKTRAYPUTCOMP				)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS42,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_UDROBOTINTERLOCK1			)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_UDROBOT_STATUS43,((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_UDROBOTINTERLOCK2			)? GXCOLOR_ON:GXCOLOR_OFF));

	// UD ROBOT TEACH POS 표시 2017/06/20 HSK.
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_GET_APPROACH,(((long)Data[0] == 100)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_GET_WAIT,(((long)Data[0] == 101)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_GET_CH1_UP,(((long)Data[0] == 110)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_GET_CH1_DOWN,(((long)Data[0] == 111)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_GET_CH2_UP,(((long)Data[0] == 120)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_GET_CH2_DOWN,(((long)Data[0] == 121)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_GOODPUT_APPROACH,(((long)Data[0] == 300)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_GOODPUT_WAIT,(((long)Data[0] == 301)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_GOODPUT_UP,(((long)Data[0] == 310)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_GOODPUT_DOWN,(((long)Data[0] == 311)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_NGPUT_APPROACH,(((long)Data[0] == 200)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_NGPUT_WAIT,(((long)Data[0] == 201)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_NGPUT_UP,(((long)Data[0] == 210)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_NGPUT_DOWN,(((long)Data[0] == 211)? GXCOLOR_ON:GXCOLOR_OFF));
}


void CFormTeachUDRobot::ClickGxbtnUdrobotOutput()
{
	if(bStatus[0])
	{
		bStatus[0] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_X_TEACHMODE_Y_EXTSTART);
	}
	else
	{
		bStatus[0] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_X_TEACHMODE_Y_EXTSTART);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput2()
{
	if(bStatus[1])
	{
		bStatus[1] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT2, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_X_PLAYMODE_Y_EXTSTOP);
	}
	else
	{
		bStatus[1] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT2, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_X_PLAYMODE_Y_EXTSTOP);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput3()
{
	if(bStatus[2])
	{
		bStatus[2] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT3, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_X_RUNNING_Y_ERRRESET);
	}
	else
	{
		bStatus[2] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT3, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_X_RUNNING_Y_ERRRESET);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput4()
{
	if(bStatus[3])
	{
		bStatus[3] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT4, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_X_STOPPING_Y_EXTMOTORON);
	}
	else
	{
		bStatus[3] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT4, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_X_STOPPING_Y_EXTMOTORON);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput5()
{
	if(bStatus[4])
	{
		bStatus[4] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT5, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_X_INTERLOCKALARM_Y_EXTMOTOROFF);
	}
	else
	{
		bStatus[4] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT5, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_X_INTERLOCKALARM_Y_EXTMOTOROFF);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput6()
{
	if(bStatus[5])
	{
		bStatus[5] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT6, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_X_MOTORON_Y_LOWSPEED);
	}
	else
	{
		bStatus[5] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT6, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_X_MOTORON_Y_LOWSPEED);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput7()
{
	if(bStatus[6])
	{
		bStatus[6] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT7, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_X_PGMEND_Y_PAUSE);
	}
	else
	{
		bStatus[6] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT7, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_X_PGMEND_Y_PAUSE);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput8()
{
	if(bStatus[7])
	{
		bStatus[7] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT8, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_X_ALARM_Y_EXTRESET);
	}
	else
	{
		bStatus[7] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT8, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_X_ALARM_Y_EXTRESET);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput9()
{
	if(bStatus[8])
	{
		bStatus[8] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT9, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_X_BATWARN_Y_ORG);
	}
	else
	{
		bStatus[8] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT9, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_X_BATWARN_Y_ORG);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput10()
{
	if(bStatus[9])
	{
		bStatus[9] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT10, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_X_HOMEPOS1_Y_CYCLESTOP);
	}
	else
	{
		bStatus[9] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT10, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_X_HOMEPOS1_Y_CYCLESTOP);
	}
}


void CFormTeachUDRobot::ClickGxbtnUdrobotOutput11()
{
	if(bStatus[10])
	{
		bStatus[10] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT11, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_X_HOMEPOS2_Y_PGMSTROBE);
	}
	else
	{
		bStatus[10] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT11, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_X_HOMEPOS2_Y_PGMSTROBE);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput12()
{
	if(bStatus[11])
	{
		bStatus[11] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT12, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_X_PGMACK_Y_PGMSEL1);
	}
	else
	{
		bStatus[11] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT12, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_X_PGMACK_Y_PGMSEL1);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput13()
{
	if(bStatus[12])
	{
		bStatus[12] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT13, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_X_EXTRESET_Y_PGMSEL2);
	}
	else
	{
		bStatus[12] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT13, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_X_EXTRESET_Y_PGMSEL2);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput14()
{
	if(bStatus[13])
	{
		bStatus[13] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT14, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_X_STATEOUT1_Y_PGMSEL3);
	}
	else
	{
		bStatus[13] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT14, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_X_STATEOUT1_Y_PGMSEL3);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput15()
{
	if(bStatus[14])
	{
		bStatus[14] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT15, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_X_STATEOUT2_Y_PGMSEL4);
	}
	else
	{
		bStatus[14] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT15, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_X_STATEOUT2_Y_PGMSEL4);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput16()
{
	if(bStatus[15])
	{
		bStatus[15] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT16, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_X_UNITREADY_Y_PGMSEL5);
	}
	else
	{
		bStatus[15] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT16, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_X_UNITREADY_Y_PGMSEL5);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput17()
{
	if(bStatus[16])
	{
		bStatus[16] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT17, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_X_BUSY_Y_PGMSEL6);
	}
	else
	{
		bStatus[16] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT17, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_X_BUSY_Y_PGMSEL6);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput18()
{
	if(bStatus[17])
	{
		bStatus[17] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT18, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_X_CYCLESTOP_Y_PGMSEL7);
	}
	else
	{
		bStatus[17] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT18, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_X_CYCLESTOP_Y_PGMSEL7);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput19()
{
	if(bStatus[18])
	{
		bStatus[18] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT19, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_X_ORGERR_Y_PGMSEL8);
	}
	else
	{
		bStatus[18] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT19, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_X_ORGERR_Y_PGMSEL8);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput20()
{
	if(bStatus[19])
	{
		bStatus[19] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT20, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_X_DATAERR_Y_SPEEDIN1);
	}
	else
	{
		bStatus[19] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT20, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_X_DATAERR_Y_SPEEDIN1);
	}
}


void CFormTeachUDRobot::ClickGxbtnUdrobotOutput21()
{
	if(bStatus[20])
	{
		bStatus[20] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT21, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_X_MEMORYERR_Y_SPEEDIN2);
	}
	else
	{
		bStatus[20] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT21, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_X_MEMORYERR_Y_SPEEDIN2);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput22()
{
	if(bStatus[21])
	{
		bStatus[21] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT22, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_Y_SPEEDIN3);
	}
	else
	{
		bStatus[21] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT22, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_Y_SPEEDIN3);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput23()
{
	if(bStatus[22])
	{
		bStatus[22] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT23, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_Y_SPEEDIN4);
	}
	else
	{
		bStatus[22] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT23, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_Y_SPEEDIN4);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput24()
{
	if(bStatus[23])
	{
		bStatus[23] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT24, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_Y_SPEEDIN5);
	}
	else
	{
		bStatus[23] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT24, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_Y_SPEEDIN5);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput25()
{
	if(bStatus[24])
	{
		bStatus[24] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT25, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_Y_SPEEDIN6);
	}
	else
	{
		bStatus[24] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT25, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_Y_SPEEDIN6);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput26()
{
	if(bStatus[25])
	{
		bStatus[25] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT26, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_Y_SPEEDIN7);
	}
	else
	{
		bStatus[25] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT26, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_Y_SPEEDIN7);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput27()
{
	if(bStatus[26])
	{
		bStatus[26] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT27, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_Y_RETRY);
	}
	else
	{
		bStatus[26] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT27, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_Y_RETRY);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput28()
{
	if(bStatus[27])
	{
		bStatus[27] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT28, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_X_MANUALIN_Y_MANUALSELECT);
	}
	else
	{
		bStatus[27] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT28, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_X_MANUALIN_Y_MANUALSELECT);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput29()
{
	if(bStatus[28])
	{
		bStatus[28] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT29, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_X_MANUALCOMP_Y_MANUALGO);
	}
	else
	{
		bStatus[28] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT29, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_X_MANUALCOMP_Y_MANUALGO);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput30()
{
	if(bStatus[29])
	{
		bStatus[29] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT30, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_XY_VACON);
	}
	else
	{
		bStatus[29] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT30, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_XY_VACON);
	}
}


void CFormTeachUDRobot::ClickGxbtnUdrobotOutput31()
{
	if(bStatus[30])
	{
		bStatus[30] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT31, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_XY_VACOFF);
	}
	else
	{
		bStatus[30] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT31, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_XY_VACOFF);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput32()
{
	if(bStatus[31])
	{
		bStatus[31] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT32, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_XY_GETSTAGE);
	}
	else
	{
		bStatus[31] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT32, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_XY_GETSTAGE);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput33()
{
	if(bStatus[32])
	{
		bStatus[32] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT33, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_XY_STGCH1VACON);
	}
	else
	{
		bStatus[32] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT33, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_XY_STGCH1VACON);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput34()
{
	if(bStatus[33])
	{
		bStatus[33] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT34, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_XY_STGCH1VACOFF);
	}
	else
	{
		bStatus[33] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT34, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_XY_STGCH1VACOFF);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput35()
{
	if(bStatus[34])
	{
		bStatus[34] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT35, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_X_STGCH2VACON_Y_STG1OK);
	}
	else
	{
		bStatus[34] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT35, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_X_STGCH2VACON_Y_STG1OK);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput36()
{
	if(bStatus[35])
	{
		bStatus[35] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT36, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_X_STGCH2VACOFF_Y_STG1NG);
	}
	else
	{
		bStatus[35] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT36, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_X_STGCH2VACOFF_Y_STG1NG);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput37()
{
	if(bStatus[36])
	{
		bStatus[36] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT37, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_Y_STG2VACON);
	}
	else
	{
		bStatus[36] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT37, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_Y_STG2VACON);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput38()
{
	if(bStatus[37])
	{
		bStatus[37] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT38, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_X_STG1GETERR_Y_STG2VACOFF);
	}
	else
	{
		bStatus[37] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT38, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_X_STG1GETERR_Y_STG2VACOFF);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput39()
{
	if(bStatus[38])
	{
		bStatus[38] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT39, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_X_STG2GETERR_Y_STG2OK);
	}
	else
	{
		bStatus[38] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT39, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_X_STG2GETERR_Y_STG2OK);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput40()
{
	if(bStatus[39])
	{
		bStatus[39] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT40, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_X_STGGETCOMP_Y_STG2NG);
	}
	else
	{
		bStatus[39] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT40, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_X_STGGETCOMP_Y_STG2NG);
	}
}


void CFormTeachUDRobot::ClickGxbtnUdrobotOutput41()
{
	if(bStatus[40])
	{
		bStatus[40] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT41, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_XY_NGTRAYPUT);
	}
	else
	{
		bStatus[40] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT41, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_XY_NGTRAYPUT);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput42()
{
	if(bStatus[41])
	{
		bStatus[41] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT42, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_XY_OKTRAYPUT);
	}
	else
	{
		bStatus[41] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT42, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_XY_OKTRAYPUT);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput43()
{
	if(bStatus[42])
	{
		bStatus[42] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT43, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_XY_UDROBOTINTERLOCK1);
	}
	else
	{
		bStatus[42] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT43, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_XY_UDROBOTINTERLOCK1);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput44()
{
	if(bStatus[43])
	{
		bStatus[43] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT44, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_XY_UDROBOTINTERLOCK2);
	}
	else
	{
		bStatus[43] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT44, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(UDROBOT_XY_UDROBOTINTERLOCK2);
	}
}

void CFormTeachUDRobot::ClickGxbtnUdrobotOutput45()
{
	if(bStatus[45])
	{
		bStatus[45] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT45, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[45] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT45, GXCOLOR_ON);
		long write = 4; 
		theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput46()
{
	if(bStatus[46])
	{
		bStatus[46] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT46, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[46] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT46, GXCOLOR_ON);
		long write = 8; 
		theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput47()
{
	if(bStatus[47])
	{
		bStatus[47] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT47, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[47] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT47, GXCOLOR_ON);
		long write = 16; 
		theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput48()
{
	if(bStatus[48])
	{
		bStatus[48] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT48, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[48] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT48, GXCOLOR_ON);
		long write = 32; 
		theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput49()
{
	if(bStatus[49])
	{
		bStatus[49] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT49, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[49] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT49, GXCOLOR_ON);
		long write = 64; 
		theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput50()
{
	if(bStatus[50])
	{
		bStatus[50] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT50, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[50] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT50, GXCOLOR_ON);
		long write = 128; 
		theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
}


void CFormTeachUDRobot::ClickGxbtnUdrobotOutput51()
{
	if(bStatus[51])
	{
		bStatus[51] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT51, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[51] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT51, GXCOLOR_ON);
		long write = 256; 
		theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput52()
{
	if(bStatus[52])
	{
		bStatus[52] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT52, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[52] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT52, GXCOLOR_ON);
		long write = 512; 
		theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput53()
{
	if(bStatus[53])
	{
		bStatus[53] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT53, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[53] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT53, GXCOLOR_ON);
		long write = 1024; 
		theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput54()
{
	if(bStatus[54])
	{
		bStatus[54] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT54, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[54] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT54, GXCOLOR_ON);
		long write = 2048; 
		theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput55()
{
	if(bStatus[55])
	{
		bStatus[55] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT55, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[55] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT55, GXCOLOR_ON);
		long write = 4096; 
		theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput56()
{
	if(bStatus[56])
	{
		bStatus[56] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT56, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[56] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT56, GXCOLOR_ON);
		long write = 8192; 
		theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput57()
{
	if(bStatus[57])
	{
		bStatus[57] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT57, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(UDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[57] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT57, GXCOLOR_ON);
		long write = 1; 
		theDeviceMelsec.RobotWordSend(UDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput58()
{
	if(bStatus[58])
	{
		bStatus[58] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT58, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(UDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[58] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT58, GXCOLOR_ON);
		long write = 2; 
		theDeviceMelsec.RobotWordSend(UDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput59()
{
	if(bStatus[59])
	{
		bStatus[59] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT59, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(UDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[59] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT59, GXCOLOR_ON);
		long write = 4; 
		theDeviceMelsec.RobotWordSend(UDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput60()
{
	if(bStatus[60])
	{
		bStatus[60] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT60, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(UDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[60] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT60, GXCOLOR_ON);
		long write = 8; 
		theDeviceMelsec.RobotWordSend(UDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput61()
{
	if(bStatus[61])
	{
		bStatus[61] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT61, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(UDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[61] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT61, GXCOLOR_ON);
		long write = 16; 
		theDeviceMelsec.RobotWordSend(UDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput62()
{
	if(bStatus[62])
	{
		bStatus[62] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT62, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(UDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[62] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT62, GXCOLOR_ON);
		long write = 32; 
		theDeviceMelsec.RobotWordSend(UDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput63()
{
	if(bStatus[63])
	{
		bStatus[63] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT63, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(UDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[63] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT63, GXCOLOR_ON);
		long write = 64; 
		theDeviceMelsec.RobotWordSend(UDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput64()
{
	if(bStatus[64])
	{
		bStatus[64] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT64, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(UDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[64] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT64, GXCOLOR_ON);
		long write = 128; 
		theDeviceMelsec.RobotWordSend(UDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput65()
{
	if(bStatus[65])
	{
		bStatus[65] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT65, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(UDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[65] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT65, GXCOLOR_ON);
		long write = 256; 
		theDeviceMelsec.RobotWordSend(UDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput66()
{
	if(bStatus[66])
	{
		bStatus[66] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT66, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[66] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT66, GXCOLOR_ON);
		long write = 1; 
		theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput67()
{
	if(bStatus[67])
	{
		bStatus[67] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT67, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[67] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT67, GXCOLOR_ON);
		long write = 2; 
		theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput68()
{
	if(bStatus[68])
	{
		bStatus[68] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT68, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[68] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT68, GXCOLOR_ON);
		long write = 4; 
		theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput69()
{
	if(bStatus[69])
	{
		bStatus[69] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT69, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[69] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT69, GXCOLOR_ON);
		long write = 8; 
		theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput70()
{
	if(bStatus[70])
	{
		bStatus[70] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT70, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[70] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT70, GXCOLOR_ON);
		long write = 16; 
		theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput71()
{
	if(bStatus[71])
	{
		bStatus[71] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT71, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[71] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT71, GXCOLOR_ON);
		long write = 32; 
		theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput72()
{
	if(bStatus[72])
	{
		bStatus[72] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT72, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[72] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT72, GXCOLOR_ON);
		long write = 64; 
		theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
	}
}
void CFormTeachUDRobot::ClickGxbtnUdrobotOutput73()
{
	if(bStatus[73])
	{
		bStatus[73] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT73, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[73] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UDROBOT_OUTPUT73, GXCOLOR_ON);
		long write = 128; 
		theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
	}
}


void CFormTeachUDRobot::ClickGxbtnUdrobotGetApproach()
{
	if((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_MANUALMOVE_STANBY) == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("현재 로봇 메뉴얼 모드를 확인 해주세요."),
			_T("Robot Manual Stanby Check, Please"),		
			_T("Robot Manual Stanby Check, Please"));

		dlgMsgBox.DoModal();
		return;
	}
	if((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_MANUALIN_Y_MANUALSELECT) == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("현재 로봇 메뉴얼 모드를 확인 해주세요."),
			_T("Robot Manual Mode Check, Please"),		
			_T("Robot Manual Mode Check, Please"));

		dlgMsgBox.DoModal();
		return;
	}

	if (theUnitFunc.MoveUnloadTR_X_GetPosChk() == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("UD TR Get Pos Move"),
			_T("UD TR Get Pos Move"),		
			_T("UD TR Get Pos Move"));

		dlgMsgBox.DoModal();
	}
	else
	{
		if(ManualPosCheck(100))
		{
			long write;
			write = 100; 
			theDeviceMelsec.RobotWordSend(UDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
		}
		else
		{
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("현재 로봇 포지션을 확인 해주세요."),
				_T("Robot Pos Check, Please"),		
				_T("Robot Pos Check, Please"));

			dlgMsgBox.DoModal();
		}
	}
}


void CFormTeachUDRobot::ClickGxbtnUdrobotGetWait()
{
	if((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_MANUALMOVE_STANBY) == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("현재 로봇 메뉴얼 모드를 확인 해주세요."),
			_T("Robot Manual Stanby Check, Please"),		
			_T("Robot Manual Stanby Check, Please"));

		dlgMsgBox.DoModal();
		return;
	}
	if((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_MANUALIN_Y_MANUALSELECT) == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("현재 로봇 메뉴얼 모드를 확인 해주세요."),
			_T("Robot Manual Mode Check, Please"),		
			_T("Robot Manual Mode Check, Please"));

		dlgMsgBox.DoModal();
		return;
	}

	if (theUnitFunc.MoveUnloadTR_X_GetPosChk() == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("UD TR Get Pos Move"),
			_T("UD TR Get Pos Move"),		
			_T("UD TR Get Pos Move"));

		dlgMsgBox.DoModal();
	}
	else
	{
		if(ManualPosCheck(101))
		{
			long write;
			write = 101; 
			theDeviceMelsec.RobotWordSend(UDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
		}
		else
		{
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("현재 로봇 포지션을 확인 해주세요."),
				_T("Robot Pos Check, Please"),		
				_T("Robot Pos Check, Please"));

			dlgMsgBox.DoModal();
		}
	}
}


void CFormTeachUDRobot::ClickGxbtnUdrobotGetCh1Up()
{
	if((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_MANUALMOVE_STANBY) == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("현재 로봇 메뉴얼 모드를 확인 해주세요."),
			_T("Robot Manual Stanby Check, Please"),		
			_T("Robot Manual Stanby Check, Please"));

		dlgMsgBox.DoModal();
		return;
	}
	if((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_MANUALIN_Y_MANUALSELECT) == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("현재 로봇 메뉴얼 모드를 확인 해주세요."),
			_T("Robot Manual Mode Check, Please"),		
			_T("Robot Manual Mode Check, Please"));

		dlgMsgBox.DoModal();
		return;
	}

	if (theUnitFunc.MoveUnloadTR_X_GetPosChk() == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("UD TR Get Pos Move"),
			_T("UD TR Get Pos Move"),		
			_T("UD TR Get Pos Move"));

		dlgMsgBox.DoModal();
	}
	else
	{
		if(ManualPosCheck(110))
		{
			long write;
			write = 110; 
			theDeviceMelsec.RobotWordSend(UDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
		}
		else
		{
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("현재 로봇 포지션을 확인 해주세요."),
				_T("Robot Pos Check, Please"),		
				_T("Robot Pos Check, Please"));

			dlgMsgBox.DoModal();
		}
	}
}


void CFormTeachUDRobot::ClickGxbtnUdrobotGetCh1Down()
{
	if((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_MANUALMOVE_STANBY) == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("현재 로봇 메뉴얼 모드를 확인 해주세요."),
			_T("Robot Manual Stanby Check, Please"),		
			_T("Robot Manual Stanby Check, Please"));

		dlgMsgBox.DoModal();
		return;
	}
	if((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_MANUALIN_Y_MANUALSELECT) == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("현재 로봇 메뉴얼 모드를 확인 해주세요."),
			_T("Robot Manual Mode Check, Please"),		
			_T("Robot Manual Mode Check, Please"));

		dlgMsgBox.DoModal();
		return;
	}

	if (theUnitFunc.MoveUnloadTR_X_GetPosChk() == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("UD TR Get Pos Move"),
			_T("UD TR Get Pos Move"),		
			_T("UD TR Get Pos Move"));

		dlgMsgBox.DoModal();
	}
	else
	{
		if(ManualPosCheck(111))
		{
			long write;
			write = 111; 
			theDeviceMelsec.RobotWordSend(UDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
		}
		else
		{
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("현재 로봇 포지션을 확인 해주세요."),
				_T("Robot Pos Check, Please"),		
				_T("Robot Pos Check, Please"));

			dlgMsgBox.DoModal();
		}
	}
}


void CFormTeachUDRobot::ClickGxbtnUdrobotGetCh2Up()
{
	if((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_MANUALMOVE_STANBY) == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("현재 로봇 메뉴얼 모드를 확인 해주세요."),
			_T("Robot Manual Stanby Check, Please"),		
			_T("Robot Manual Stanby Check, Please"));

		dlgMsgBox.DoModal();
		return;
	}
	if((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_MANUALIN_Y_MANUALSELECT) == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("현재 로봇 메뉴얼 모드를 확인 해주세요."),
			_T("Robot Manual Mode Check, Please"),		
			_T("Robot Manual Mode Check, Please"));

		dlgMsgBox.DoModal();
		return;
	}

	if (theUnitFunc.MoveUnloadTR_X_GetPosChk() == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("UD TR Get Pos Move"),
			_T("UD TR Get Pos Move"),		
			_T("UD TR Get Pos Move"));

		dlgMsgBox.DoModal();
	}
	else
	{
		if(ManualPosCheck(120))
		{
			long write;
			write = 120; 
			theDeviceMelsec.RobotWordSend(UDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
		}
		else
		{
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("현재 로봇 포지션을 확인 해주세요."),
				_T("Robot Pos Check, Please"),		
				_T("Robot Pos Check, Please"));

			dlgMsgBox.DoModal();
		}
	}
}


void CFormTeachUDRobot::ClickGxbtnUdrobotGetCh2Down()
{
	if((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_MANUALMOVE_STANBY) == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("현재 로봇 메뉴얼 모드를 확인 해주세요."),
			_T("Robot Manual Stanby Check, Please"),		
			_T("Robot Manual Stanby Check, Please"));

		dlgMsgBox.DoModal();
		return;
	}
	if((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_MANUALIN_Y_MANUALSELECT) == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("현재 로봇 메뉴얼 모드를 확인 해주세요."),
			_T("Robot Manual Mode Check, Please"),		
			_T("Robot Manual Mode Check, Please"));

		dlgMsgBox.DoModal();
		return;
	}

	if (theUnitFunc.MoveUnloadTR_X_GetPosChk() == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("UD TR Get Pos Move"),
			_T("UD TR Get Pos Move"),		
			_T("UD TR Get Pos Move"));

		dlgMsgBox.DoModal();
	}
	else
	{
		if(ManualPosCheck(121))
		{
			long write;
			write = 121; 
			theDeviceMelsec.RobotWordSend(UDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
		}
		else
		{
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("현재 로봇 포지션을 확인 해주세요."),
				_T("Robot Pos Check, Please"),		
				_T("Robot Pos Check, Please"));

			dlgMsgBox.DoModal();
		}	
	}
}


void CFormTeachUDRobot::ClickGxbtnUdrobotGoodputApproach()
{
	if((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_MANUALMOVE_STANBY) == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("현재 로봇 메뉴얼 모드를 확인 해주세요."),
			_T("Robot Manual Stanby Check, Please"),		
			_T("Robot Manual Stanby Check, Please"));

		dlgMsgBox.DoModal();
		return;
	}
	if((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_MANUALIN_Y_MANUALSELECT) == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("현재 로봇 메뉴얼 모드를 확인 해주세요."),
			_T("Robot Manual Mode Check, Please"),		
			_T("Robot Manual Mode Check, Please"));

		dlgMsgBox.DoModal();
		return;
	}

	if (theUnitFunc.TrayTrY_OutChk() == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("Good Tray TR Put Pos Move"),
			_T("Good Tray TR Put Pos Move"),		
			_T("Good Tray TR Put Pos Move"));

		dlgMsgBox.DoModal();
	}
	else
	{
		if(ManualPosCheck(300))
		{
			long write;
			write = 300; 
			theDeviceMelsec.RobotWordSend(UDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
		}
		else
		{
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("현재 로봇 포지션을 확인 해주세요."),
				_T("Robot Pos Check, Please"),		
				_T("Robot Pos Check, Please"));

			dlgMsgBox.DoModal();
		}
	}
}


void CFormTeachUDRobot::ClickGxbtnUdrobotGoodputWait()
{
	if((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_MANUALMOVE_STANBY) == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("현재 로봇 메뉴얼 모드를 확인 해주세요."),
			_T("Robot Manual Stanby Check, Please"),		
			_T("Robot Manual Stanby Check, Please"));

		dlgMsgBox.DoModal();
		return;
	}
	if((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_MANUALIN_Y_MANUALSELECT) == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("현재 로봇 메뉴얼 모드를 확인 해주세요."),
			_T("Robot Manual Mode Check, Please"),		
			_T("Robot Manual Mode Check, Please"));

		dlgMsgBox.DoModal();
		return;
	}

	if (theUnitFunc.TrayTrY_OutChk() == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("Good Tray TR Put Pos Move"),
			_T("Good Tray TR Put Pos Move"),		
			_T("Good Tray TR Put Pos Move"));

		dlgMsgBox.DoModal();
	}
	else
	{
		if(ManualPosCheck(301))
		{
			long write;
			write = 301; 
			theDeviceMelsec.RobotWordSend(UDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
		}
		else
		{
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("현재 로봇 포지션을 확인 해주세요."),
				_T("Robot Pos Check, Please"),		
				_T("Robot Pos Check, Please"));

			dlgMsgBox.DoModal();
		}
	}
}


void CFormTeachUDRobot::ClickGxbtnUdrobotGoodputUp()
{
	if((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_MANUALMOVE_STANBY) == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("현재 로봇 메뉴얼 모드를 확인 해주세요."),
			_T("Robot Manual Stanby Check, Please"),		
			_T("Robot Manual Stanby Check, Please"));

		dlgMsgBox.DoModal();
		return;
	}
	if((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_MANUALIN_Y_MANUALSELECT) == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("현재 로봇 메뉴얼 모드를 확인 해주세요."),
			_T("Robot Manual Mode Check, Please"),		
			_T("Robot Manual Mode Check, Please"));

		dlgMsgBox.DoModal();
		return;
	}

	if (theUnitFunc.TrayTrY_OutChk() == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("Good Tray TR Put Pos Move"),
			_T("Good Tray TR Put Pos Move"),		
			_T("Good Tray TR Put Pos Move"));

		dlgMsgBox.DoModal();
	}
	else
	{
		if(ManualPosCheck(310))
		{
			long write;
			write = 310; 
			theDeviceMelsec.RobotWordSend(UDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
		}
		else
		{
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("현재 로봇 포지션을 확인 해주세요."),
				_T("Robot Pos Check, Please"),		
				_T("Robot Pos Check, Please"));

			dlgMsgBox.DoModal();
		}
	}
}


void CFormTeachUDRobot::ClickGxbtnUdrobotGoodputDown()
{
	if((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_MANUALMOVE_STANBY) == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("현재 로봇 메뉴얼 모드를 확인 해주세요."),
			_T("Robot Manual Stanby Check, Please"),		
			_T("Robot Manual Stanby Check, Please"));

		dlgMsgBox.DoModal();
		return;
	}
	if((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_MANUALIN_Y_MANUALSELECT) == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("현재 로봇 메뉴얼 모드를 확인 해주세요."),
			_T("Robot Manual Mode Check, Please"),		
			_T("Robot Manual Mode Check, Please"));

		dlgMsgBox.DoModal();
		return;
	}

	if (theUnitFunc.TrayTrY_OutChk() == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("Good Tray TR Put Pos Move"),
			_T("Good Tray TR Put Pos Move"),		
			_T("Good Tray TR Put Pos Move"));

		dlgMsgBox.DoModal();
	}
	else
	{
		if(ManualPosCheck(311))
		{
			long write;
			write = 311; 
			theDeviceMelsec.RobotWordSend(UDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
		}
		else
		{
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("현재 로봇 포지션을 확인 해주세요."),
				_T("Robot Pos Check, Please"),		
				_T("Robot Pos Check, Please"));

			dlgMsgBox.DoModal();
		}
	}
}


void CFormTeachUDRobot::ClickGxbtnUdrobotNgputApproach()
{
	if((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_MANUALMOVE_STANBY) == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("현재 로봇 메뉴얼 모드를 확인 해주세요."),
			_T("Robot Manual Stanby Check, Please"),		
			_T("Robot Manual Stanby Check, Please"));

		dlgMsgBox.DoModal();
		return;
	}
	if((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_MANUALIN_Y_MANUALSELECT) == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("현재 로봇 메뉴얼 모드를 확인 해주세요."),
			_T("Robot Manual Mode Check, Please"),		
			_T("Robot Manual Mode Check, Please"));

		dlgMsgBox.DoModal();
		return;
	}

	if (theUnitFunc.NGTrayTrX_OutChk() == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("NG Tray TR Put Pos Move"),
			_T("NG Tray TR Put Pos Move"),		
			_T("NG Tray TR Put Pos Move"));

		dlgMsgBox.DoModal();
	}
	else
	{
		if(ManualPosCheck(200))
		{
			long write;
			write = 200; 
			theDeviceMelsec.RobotWordSend(UDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
		}
		else
		{
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("현재 로봇 포지션을 확인 해주세요."),
				_T("Robot Pos Check, Please"),		
				_T("Robot Pos Check, Please"));

			dlgMsgBox.DoModal();
		}
	}
}


void CFormTeachUDRobot::ClickGxbtnUdrobotNgputWait()
{
	if((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_MANUALMOVE_STANBY) == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("현재 로봇 메뉴얼 모드를 확인 해주세요."),
			_T("Robot Manual Stanby Check, Please"),		
			_T("Robot Manual Stanby Check, Please"));

		dlgMsgBox.DoModal();
		return;
	}
	if((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_MANUALIN_Y_MANUALSELECT) == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("현재 로봇 메뉴얼 모드를 확인 해주세요."),
			_T("Robot Manual Mode Check, Please"),		
			_T("Robot Manual Mode Check, Please"));

		dlgMsgBox.DoModal();
		return;
	}

	if (theUnitFunc.NGTrayTrX_OutChk() == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("NG Tray TR Put Pos Move"),
			_T("NG Tray TR Put Pos Move"),		
			_T("NG Tray TR Put Pos Move"));

		dlgMsgBox.DoModal();
	}
	else
	{
		if(ManualPosCheck(201))
		{
			long write;
			write = 201; 
			theDeviceMelsec.RobotWordSend(UDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
		}
		else
		{
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("현재 로봇 포지션을 확인 해주세요."),
				_T("Robot Pos Check, Please"),		
				_T("Robot Pos Check, Please"));

			dlgMsgBox.DoModal();
		}
	}
}


void CFormTeachUDRobot::ClickGxbtnUdrobotNgputUp()
{
	if((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_MANUALMOVE_STANBY) == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("현재 로봇 메뉴얼 모드를 확인 해주세요."),
			_T("Robot Manual Stanby Check, Please"),		
			_T("Robot Manual Stanby Check, Please"));

		dlgMsgBox.DoModal();
		return;
	}
	if((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_MANUALIN_Y_MANUALSELECT) == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("현재 로봇 메뉴얼 모드를 확인 해주세요."),
			_T("Robot Manual Mode Check, Please"),		
			_T("Robot Manual Mode Check, Please"));

		dlgMsgBox.DoModal();
		return;
	}

	if (theUnitFunc.NGTrayTrX_OutChk() == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("NG Tray TR Put Pos Move"),
			_T("NG Tray TR Put Pos Move"),		
			_T("NG Tray TR Put Pos Move"));

		dlgMsgBox.DoModal();
	}
	else
	{
		if(ManualPosCheck(210))
		{
			long write;
			write = 210; 
			theDeviceMelsec.RobotWordSend(UDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
		}
		else
		{
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("현재 로봇 포지션을 확인 해주세요."),
				_T("Robot Pos Check, Please"),		
				_T("Robot Pos Check, Please"));

			dlgMsgBox.DoModal();
		}
	}
}


void CFormTeachUDRobot::ClickGxbtnUdrobotNgputDown()
{
	if((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_MANUALMOVE_STANBY) == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("현재 로봇 메뉴얼 모드를 확인 해주세요."),
			_T("Robot Manual Stanby Check, Please"),		
			_T("Robot Manual Stanby Check, Please"));

		dlgMsgBox.DoModal();
		return;
	}
	if((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_MANUALIN_Y_MANUALSELECT) == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("현재 로봇 메뉴얼 모드를 확인 해주세요."),
			_T("Robot Manual Mode Check, Please"),		
			_T("Robot Manual Mode Check, Please"));

		dlgMsgBox.DoModal();
		return;
	}

	if (theUnitFunc.NGTrayTrX_OutChk() == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("NG Tray TR Put Pos Move"),
			_T("NG Tray TR Put Pos Move"),		
			_T("NG Tray TR Put Pos Move"));

		dlgMsgBox.DoModal();
	}
	else
	{
		if(ManualPosCheck(211))
		{
			long write;
			write = 211; 
			theDeviceMelsec.RobotWordSend(UDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
		}
		else
		{
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("현재 로봇 포지션을 확인 해주세요."),
				_T("Robot Pos Check, Please"),		
				_T("Robot Pos Check, Please"));

			dlgMsgBox.DoModal();
		}
	}
}


void CFormTeachUDRobot::ClickGxbtnUnldRobotTray00()
{
	long write = 17; 
	theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
}


void CFormTeachUDRobot::ClickGxbtnUnldRobotTray01()
{
	long write = 33; 
	theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
}


void CFormTeachUDRobot::ClickGxbtnUnldRobotTray02()
{
	long write = 49; 
	theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
}


void CFormTeachUDRobot::ClickGxbtnUnldRobotTray03()
{
	long write = 65; 
	theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
}


void CFormTeachUDRobot::ClickGxbtnUnldRobotTray10()
{
	long write = 18; 
	theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
}


void CFormTeachUDRobot::ClickGxbtnUnldRobotTray11()
{
	long write = 34; 
	theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
}


void CFormTeachUDRobot::ClickGxbtnUnldRobotTray12()
{
	long write = 50; 
	theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
}


void CFormTeachUDRobot::ClickGxbtnUnldRobotTray13()
{
	long write = 66; 
	theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);

}


void CFormTeachUDRobot::ClickGxbtnUnldRobotTray20()
{
	long write = 19; 
	theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
}


void CFormTeachUDRobot::ClickGxbtnUnldRobotTray21()
{
	long write = 35; 
	theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
}


void CFormTeachUDRobot::ClickGxbtnUnldRobotTray22()
{
	long write = 51; 
	theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
}


void CFormTeachUDRobot::ClickGxbtnUnldRobotTray23()
{
	long write = 67; 
	theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
}


void CFormTeachUDRobot::ClickGxbtnUnldRobotTray30()
{
	long write = 20; 
	theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
}


void CFormTeachUDRobot::ClickGxbtnUnldRobotTray31()
{
	long write = 36; 
	theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
}


void CFormTeachUDRobot::ClickGxbtnUnldRobotTray32()
{
	long write = 52; 
	theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
}


void CFormTeachUDRobot::ClickGxbtnUnldRobotTray33()
{
	long write = 68; 
	theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
}


void CFormTeachUDRobot::ClickGxbtnUdrobotVacOn()
{
	theUnitFunc.RobotCellVacOn(VAC_ON,E_UD_ROBOT);
	theDeviceMelsec.RobotBitOn(UDROBOT_XY_VACON);
	theDeviceMelsec.RobotBitOff(UDROBOT_XY_VACOFF);
}


void CFormTeachUDRobot::ClickGxbtnUdrobotVacOff()
{
	theUnitFunc.RobotCellVacOn(VAC_OFF,E_UD_ROBOT);
	Sleep(200);
	theUnitFunc.RobotBlowOff(E_UD_ROBOT);
	theDeviceMelsec.RobotBitOff(UDROBOT_XY_VACON);
	theDeviceMelsec.RobotBitOn(UDROBOT_XY_VACOFF);
}


void CFormTeachUDRobot::ClickGxbtnUdrobotMove()
{
	theDeviceMelsec.RobotBitOn(UDROBOT_X_MANUALCOMP_Y_MANUALGO);
	Sleep(200);
	theDeviceMelsec.RobotBitOff(UDROBOT_X_MANUALCOMP_Y_MANUALGO);
}

BOOL CFormTeachUDRobot::ManualPosCheck(int Pos)
{
	short Data[1];
	theDeviceMelsec.RobotWordRecvBuf(UDROBOT_MANUAL_POS,sizeof(short),Data);
	
	if(Pos == 100)
	{
		if((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_HOMEPOS1_Y_CYCLESTOP)
			|| (long)Data[0] == 100 || (long)Data[0] == 101 || (long)Data[0] == 200 || (long)Data[0] == 201
			|| (long)Data[0] == 300 || (long)Data[0] == 301)
			return TRUE;
		else
			return FALSE;
	}
	else if(Pos == 101)
	{
		if((long)Data[0] == 100 || (long)Data[0] == 101 || (long)Data[0] == 110 || (long)Data[0] == 120)
			return TRUE;
		else
			return FALSE;
	}
	else if(Pos == 110)
	{
		if((long)Data[0] == 101 || (long)Data[0] == 110 || (long)Data[0] == 111)
			return TRUE;
		else
			return FALSE;
	}
	else if(Pos == 111)
	{
		if((long)Data[0] == 110 || (long)Data[0] == 111)
			return TRUE;
		else
			return FALSE;
	}
	else if(Pos == 120)
	{
		if((long)Data[0] == 101 || (long)Data[0] == 120 || (long)Data[0] == 121)
			return TRUE;
		else
			return FALSE;
	}
	else if(Pos == 121)
	{
		if((long)Data[0] == 120 || (long)Data[0] == 121)
			return TRUE;
		else
			return FALSE;
	}
	else if(Pos == 200)
	{
		if((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_HOMEPOS1_Y_CYCLESTOP)
			|| (long)Data[0] == 100 || (long)Data[0] == 200 || (long)Data[0] == 201)
			return TRUE;
		else
			return FALSE;
	}
	else if(Pos == 201)
	{
		if((long)Data[0] == 100 || (long)Data[0] == 200 || (long)Data[0] == 201 || (long)Data[0] == 210)
			return TRUE;
		else
			return FALSE;
	}
	else if(Pos == 210)
	{
		if((long)Data[0] == 201 || (long)Data[0] == 210 || (long)Data[0] == 211)
			return TRUE;
		else
			return FALSE;
	}
	else if(Pos == 211)
	{
		if((long)Data[0] == 210 || (long)Data[0] == 211)
			return TRUE;
		else
			return FALSE;
	}
	else if(Pos == 300)
	{
		if((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_HOMEPOS1_Y_CYCLESTOP)
			|| (long)Data[0] == 100 || (long)Data[0] == 300 || (long)Data[0] == 301)
			return TRUE;
		else
			return FALSE;
	}
	else if(Pos == 301)
	{
		if((long)Data[0] == 100 || (long)Data[0] == 300 || (long)Data[0] == 301 || (long)Data[0] == 310)
			return TRUE;
		else
			return FALSE;
	}
	else if(Pos == 310)
	{
		if((long)Data[0] == 301 || (long)Data[0] == 310 || (long)Data[0] == 311)
			return TRUE;
		else
			return FALSE;
	}
	else if(Pos == 311)
	{
		if((long)Data[0] == 310 || (long)Data[0] == 311)
			return TRUE;
		else
			return FALSE;
	}

	return FALSE;
}
