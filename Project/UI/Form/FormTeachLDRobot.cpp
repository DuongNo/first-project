// FormTeachUDRobot.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "resource.h"
#include "FormTeachLDRobot.h"

#include "GUIDefine.h"

//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "UI\GausGUI\GxButtonEx.h"
#include "UI\GausGUI\GxStaticEx.h"

#include "UI\GausGUI\GxMsgBox.h"

// CFormTeachUDRobot

IMPLEMENT_DYNCREATE(CFormTeachLDRobot, CFormView)

CFormTeachLDRobot::CFormTeachLDRobot()
	: CFormView(CFormTeachLDRobot::IDD)
{
	m_bInit = FALSE;

	for(int i = 0 ; i < 100; i++)
	{
		bStatus[i] = FALSE;
	}
}

CFormTeachLDRobot::~CFormTeachLDRobot()
{
}

void CFormTeachLDRobot::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFormTeachLDRobot, CFormView)
	ON_WM_TIMER()
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)			// 표시 언어가 바뀌었다.
END_MESSAGE_MAP()


// CFormTeachUDRobot 진단입니다.

#ifdef _DEBUG
void CFormTeachLDRobot::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormTeachLDRobot::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormTeachUDRobot 메시지 처리기입니다.

BOOL CFormTeachLDRobot::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style = WS_OVERLAPPED | WS_CHILD;				// 테두리를 없앤다.

	return CFormView::PreCreateWindow(cs);
}

BEGIN_EVENTSINK_MAP(CFormTeachLDRobot, CFormView)

	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT2, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput2, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT3, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput3, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT4, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput4, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT5, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput5, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT6, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput6, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT7, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput7, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT8, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput8, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT9, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput9, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT10, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput10, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT11, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput11, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT12, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput12, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT13, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput13, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT14, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput14, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT15, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput15, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT16, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput16, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT17, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput17, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT18, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput18, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT19, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput19, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT20, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput20, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT21, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput21, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT22, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput22, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT23, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput23, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT24, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput24, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT25, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput25, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT26, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput26, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT27, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput27, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT28, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput28, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT29, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput29, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT30, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput30, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT31, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput31, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT32, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput32, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT33, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput33, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT34, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput34, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT35, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput35, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT36, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput36, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT37, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput37, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT38, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput38, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT39, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput39, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT40, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput40, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT41, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput41, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT42, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput42, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT43, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput43, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT44, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput44, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT45, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput45, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT46, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput46, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT47, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput47, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT48, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput48, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT49, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput49, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT50, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput50, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT51, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput51, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT52, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput52, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT53, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput53, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT54, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput54, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT55, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput55, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT56, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput56, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT57, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput57, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT58, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput58, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT59, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput59, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT60, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput60, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT61, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput61, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT62, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput62, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT63, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput63, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT64, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput64, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT65, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput65, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT66, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput66, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT67, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput67, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT68, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput68, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT69, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput69, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT70, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput70, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT71, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput71, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT72, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput72, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT73, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput73, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_OUTPUT74, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotOutput74, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_GET_APPROACH, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotGetApproach, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_GET_WAIT, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotGetWait, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_GET_UP, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotGetUp, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_GET_DOWN, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotGetDown, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_UPPER_APPROACH, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotUpperApproach, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_UPPER_WAIT, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotUpperWait, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_UPPER_UP1, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotUpperUp1, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_UPPER_DOWN1, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotUpperDown1, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_UPPER_UP2, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotUpperUp2, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_UPPER_DOWN2, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotUpperDown2, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_LOWER_APPROACH, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotLowerApproach, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_LOWER_WAIT, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotLowerWait, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_LOWER_UP, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotLowerUp, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_LOWER_DOWN, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotLowerDown, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_LOWER_UP2, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotLowerUp2, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_LOWER_DOWN2, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotLowerDown2, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_VAC_ON, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotVacOn, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_VAC_OFF, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotVacOff, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_ALIGN, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotAlign, VTS_NONE)
	ON_EVENT(CFormTeachLDRobot, IDC_GXBTN_LDROBOT_MOVE, DISPID_CLICK, CFormTeachLDRobot::ClickGxbtnLdrobotMove, VTS_NONE)
END_EVENTSINK_MAP()


void CFormTeachLDRobot::OnInitialUpdate()
{
	if(m_bInit)	return;	// OnInitialUpdate 두번 실행되는 버그 방지 [10/24/2016 OSC]
	m_bInit = TRUE;
	CFormView::OnInitialUpdate();
}

LRESULT CFormTeachLDRobot::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{
		UpdateLDRobotState();
		theDeviceMelsec.RobotBitOn(LDROBOT_X_MANUALIN_Y_MANUALSELECT);
		SetTimer(1, 100, NULL);
	}
	else
	{
		theDeviceMelsec.RobotBitOff(LDROBOT_X_MANUALIN_Y_MANUALSELECT);
		KillTimer(1);
	}
	return 0;
}

void CFormTeachLDRobot::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
	case 1:
		UpdateLDRobotState();
		break;
	case 2:
		if (theProcBank.m_strLA_AlignOK == _T("OK"))
		{
			CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_ALIGN_SEARCH_OK, GXCOLOR_ON);
			CGxUICtrl::SetStaticString(this, IDC_GXSTC_LDROBOT_ALIGN_SEARCH_OK, _T("ALIGN OK"));
		}
		else
		{
			CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_ALIGN_SEARCH_OK, GXCOLOR_RED);
			CGxUICtrl::SetStaticString(this, IDC_GXSTC_LDROBOT_ALIGN_SEARCH_OK, _T("ALIGN NG"));
		}
		break;
	}

	CFormView::OnTimer(nIDEvent);
}

LRESULT CFormTeachLDRobot::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

void CFormTeachLDRobot::UpdateLDRobotState()
{
	short Data[1];
	theDeviceMelsec.RobotWordRecvBuf(LDROBOT_MANUAL_POS,sizeof(short),Data);

	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_TEACHMODE_Y_EXTSTART			)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS2,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_PLAYMODE_Y_EXTSTOP			)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS3,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_RUNNING_Y_ERRRESET			)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS4,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_STOPPING_Y_EXTMOTORON		)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS5,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_INTERLOCKALARM_Y_EXTMOTOROFF	)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS6,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MOTORON_Y_LOWSPEED			)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS7,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_PGMEND_Y_PAUSE				)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS8,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_ALARM_Y_EXTRESET				)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS9,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_BATWARN_Y_ORG				)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS10,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_HOMEPOS1_Y_CYCLESTOP		)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS11,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_HOMEPOS2_Y_PGMSTROBE		)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS12,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_PGMACK_Y_PGMSEL1			)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS13,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_EXTRESET_Y_PGMSEL2			)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS14,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_STATEOUT1_Y_PGMSEL3			)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS15,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_STATEOUT2_Y_PGMSEL4			)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS16,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_UNITREADY_Y_PGMSEL5			)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS17,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_BUSY_Y_PGMSEL6				)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS18,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_CYCLESTOP_Y_PGMSEL7			)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS19,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_ORGERR_Y_PGMSEL8			)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS20,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_DATAERR_Y_SPEEDIN1			)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS21,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MEMORYERR_Y_SPEEDIN2		)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS22,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MANUALIN_Y_MANUALSELECT		)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS23,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MANUALCOMP_Y_MANUALGO		)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS49,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MANUALMOVE_STANBY			)? GXCOLOR_ON:GXCOLOR_OFF));
															   
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS24,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_VACON						)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS25,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_VACOFF						)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS26,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_VACERR						)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS27,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_CVGET						)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS48,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_VICOMP						)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS47,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_Y_VIERR						)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS28,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_CVGETERR					)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS29,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_UPPERSTGPUT				)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS30,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_UPPERSTGVAC1ON				)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS31,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_UPPERSTGVAC1OFF			)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS32,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_UPPERSTGVAC2ON				)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS33,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_UPPERSTGVAC2OFF			)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS34,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_UPPERSTGVAC1ERR				)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS35,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_UPPERSTGVAC2ERR				)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS36,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_UPPERSTGPUTCOMP				)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS37,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_LOWERSTGPUT				)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS38,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_LOWERSTGVAC1ON				)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS39,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_LOWERSTGVAC1OFF			)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS40,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_LOWERSTGVAC2ON				)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS41,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_LOWERSTGVAC2OFF			)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS42,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_LOWERSTGVAC1ERR				)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS43,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_LOWERSTGVAC2ERR				)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS44,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_LOWERSTGPUTCOMP				)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS45,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_LDROBOTINTERLOCK1			)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LDROBOT_STATUS46,((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_LDROBOTINTERLOCK2			)? GXCOLOR_ON:GXCOLOR_OFF));

	if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_VICOMP))
		theUnitFunc.LDRobotSendAlignData(theUnitFunc.LoaderAlignResultX(),theUnitFunc.LoaderAlignResultY(),theUnitFunc.LoaderAlignResultT());

	// LD ROBOT TEACH POS 표시 2017/06/20 HSK.

	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_GET_APPROACH,(((long)Data[0] == 100)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_GET_WAIT,(((long)Data[0] == 101)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_GET_UP,(((long)Data[0] == 110)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_GET_DOWN,(((long)Data[0] == 111)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_UPPER_APPROACH,(((long)Data[0] == 200)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_UPPER_WAIT,(((long)Data[0] == 201)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_UPPER_UP1,(((long)Data[0] == 210)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_UPPER_DOWN1,(((long)Data[0] == 211)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_UPPER_UP2,(((long)Data[0] == 220)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_UPPER_DOWN2,(((long)Data[0] == 221)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_LOWER_APPROACH,(((long)Data[0] == 300)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_LOWER_WAIT,(((long)Data[0] == 301)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_LOWER_UP,(((long)Data[0] == 310)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_LOWER_DOWN,(((long)Data[0] == 311)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_LOWER_UP2,(((long)Data[0] == 320)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_LOWER_DOWN2,(((long)Data[0] == 321)? GXCOLOR_ON:GXCOLOR_OFF));
}


void CFormTeachLDRobot::ClickGxbtnLdrobotOutput()
{
	if(bStatus[0])
	{
		bStatus[0] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_X_TEACHMODE_Y_EXTSTART);
	}
	else
	{
		bStatus[0] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_X_TEACHMODE_Y_EXTSTART);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput2()
{
	if(bStatus[1])
	{
		bStatus[1] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT2, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_X_PLAYMODE_Y_EXTSTOP);
	}
	else
	{
		bStatus[1] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT2, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_X_PLAYMODE_Y_EXTSTOP);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput3()
{
	if(bStatus[3])
	{
		bStatus[3] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT3, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_X_RUNNING_Y_ERRRESET);
	}
	else
	{
		bStatus[3] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT3, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_X_RUNNING_Y_ERRRESET);
	}

}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput4()
{
	if(bStatus[4])
	{
		bStatus[4] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT4, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_X_STOPPING_Y_EXTMOTORON);
	}
	else
	{
		bStatus[4] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT4, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_X_STOPPING_Y_EXTMOTORON);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput5()
{
	if(bStatus[5])
	{
		bStatus[5] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT5, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_X_INTERLOCKALARM_Y_EXTMOTOROFF);
	}
	else
	{
		bStatus[5] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT5, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_X_INTERLOCKALARM_Y_EXTMOTOROFF);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput6()
{
	if(bStatus[6])
	{
		bStatus[6] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT6, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_X_MOTORON_Y_LOWSPEED);
	}
	else
	{
		bStatus[6] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT6, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_X_MOTORON_Y_LOWSPEED);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput7()
{
	if(bStatus[7])
	{
		bStatus[7] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT7, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_X_PGMEND_Y_PAUSE);
	}
	else
	{
		bStatus[7] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT7, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_X_PGMEND_Y_PAUSE);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput8()
{
	if(bStatus[8])
	{
		bStatus[8] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT8, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_X_ALARM_Y_EXTRESET);
	}
	else
	{
		bStatus[8] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT8, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_X_ALARM_Y_EXTRESET);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput9()
{
	if(bStatus[9])
	{
		bStatus[9] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT9, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_X_BATWARN_Y_ORG);
	}
	else
	{
		bStatus[9] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT9, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_X_BATWARN_Y_ORG);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput10()
{
	if(bStatus[10])
	{
		bStatus[10] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT10, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_X_HOMEPOS1_Y_CYCLESTOP);
	}
	else
	{
		bStatus[10] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT10, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_X_HOMEPOS1_Y_CYCLESTOP);
	}
}


void CFormTeachLDRobot::ClickGxbtnLdrobotOutput11()
{
	if(bStatus[11])
	{
		bStatus[11] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT11, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_X_HOMEPOS2_Y_PGMSTROBE);
	}
	else
	{
		bStatus[11] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT11, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_X_HOMEPOS2_Y_PGMSTROBE);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput12()
{
	if(bStatus[12])
	{
		bStatus[12] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT12, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_X_PGMACK_Y_PGMSEL1);
	}
	else
	{
		bStatus[12] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT12, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_X_PGMACK_Y_PGMSEL1);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput13()
{
	if(bStatus[13])
	{
		bStatus[13] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT13, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_X_EXTRESET_Y_PGMSEL2);
	}
	else
	{
		bStatus[13] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT13, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_X_EXTRESET_Y_PGMSEL2);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput14()
{
	if(bStatus[14])
	{
		bStatus[14] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT14, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_X_STATEOUT1_Y_PGMSEL3);
	}
	else
	{
		bStatus[14] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT14, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_X_STATEOUT1_Y_PGMSEL3);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput15()
{
	if(bStatus[15])
	{
		bStatus[15] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT15, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_X_STATEOUT2_Y_PGMSEL4);
	}
	else
	{
		bStatus[15] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT15, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_X_STATEOUT2_Y_PGMSEL4);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput16()
{
	if(bStatus[16])
	{
		bStatus[16] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT16, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_X_UNITREADY_Y_PGMSEL5);
	}
	else
	{
		bStatus[16] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT16, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_X_UNITREADY_Y_PGMSEL5);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput17()
{
	if(bStatus[17])
	{
		bStatus[17] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT17, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_X_BUSY_Y_PGMSEL6);
	}
	else
	{
		bStatus[17] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT17, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_X_BUSY_Y_PGMSEL6);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput18()
{
	if(bStatus[18])
	{
		bStatus[18] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT18, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_X_CYCLESTOP_Y_PGMSEL7);
	}
	else
	{
		bStatus[18] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT18, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_X_CYCLESTOP_Y_PGMSEL7);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput19()
{
	if(bStatus[19])
	{
		bStatus[19] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT19, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_X_ORGERR_Y_PGMSEL8);
	}
	else
	{
		bStatus[19] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT19, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_X_ORGERR_Y_PGMSEL8);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput20()
{
	if(bStatus[20])
	{
		bStatus[20] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT20, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_X_DATAERR_Y_SPEEDIN1);
	}
	else
	{
		bStatus[20] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT20, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_X_DATAERR_Y_SPEEDIN1);
	}
}


void CFormTeachLDRobot::ClickGxbtnLdrobotOutput21()
{
	if(bStatus[21])
	{
		bStatus[21] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT21, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_X_MEMORYERR_Y_SPEEDIN2);
	}
	else
	{
		bStatus[21] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT21, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_X_MEMORYERR_Y_SPEEDIN2);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput22()
{
	if(bStatus[22])
	{
		bStatus[22] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT22, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_Y_SPEEDIN3);
	}
	else
	{
		bStatus[22] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT22, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_Y_SPEEDIN3);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput23()
{
	if(bStatus[23])
	{
		bStatus[23] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT23, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_Y_SPEEDIN4);
	}
	else
	{
		bStatus[23] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT23, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_Y_SPEEDIN4);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput24()
{
	if(bStatus[24])
	{
		bStatus[24] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT24, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_Y_SPEEDIN5);
	}
	else
	{
		bStatus[24] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT24, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_Y_SPEEDIN5);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput25()
{
	if(bStatus[25])
	{
		bStatus[25] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT25, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_Y_SPEEDIN6);
	}
	else
	{
		bStatus[25] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT25, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_Y_SPEEDIN6);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput26()
{
	if(bStatus[26])
	{
		bStatus[26] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT26, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_Y_SPEEDIN7);
	}
	else
	{
		bStatus[26] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT26, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_Y_SPEEDIN7);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput27()
{
	if(bStatus[27])
	{
		bStatus[27] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT27, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_Y_RETRY);
	}
	else
	{
		bStatus[27] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT27, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_Y_RETRY);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput28()
{
	if(bStatus[28])
	{
		bStatus[28] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT28, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_X_MANUALIN_Y_MANUALSELECT);
	}
	else
	{
		bStatus[28] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT28, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_X_MANUALIN_Y_MANUALSELECT);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput29()
{
	if(bStatus[29])
	{
		bStatus[29] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT29, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_X_MANUALCOMP_Y_MANUALGO);
	}
	else
	{
		bStatus[29] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT29, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_X_MANUALCOMP_Y_MANUALGO);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput30()
{
	if(bStatus[30])
	{
		bStatus[30] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT30, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_XY_VACON);
	}
	else
	{
		bStatus[30] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT30, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_XY_VACON);
	}
}


void CFormTeachLDRobot::ClickGxbtnLdrobotOutput31()
{
	if(bStatus[31])
	{
		bStatus[31] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT31, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_XY_VACOFF);
	}
	else
	{
		bStatus[31] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT31, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_XY_VACOFF);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput32()
{
	if(bStatus[32])
	{
		bStatus[32] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT32, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_XY_CVGET);
	}
	else
	{
		bStatus[32] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT32, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_XY_CVGET);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput33()
{
	if(bStatus[33])
	{
		bStatus[33] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT33, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_XY_VICOMP);
	}
	else
	{
		bStatus[33] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT33, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_XY_VICOMP);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput34()
{
	if(bStatus[34])
	{
		bStatus[34] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT34, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_Y_VIERR);
	}
	else
	{
		bStatus[34] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT34, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_Y_VIERR);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput35()
{
	if(bStatus[35])
	{
		bStatus[35] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT35, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_XY_UPPERSTGPUT);
	}
	else
	{
		bStatus[35] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT35, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_XY_UPPERSTGPUT);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput36()
{
	if(bStatus[36])
	{
		bStatus[36] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT36, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_XY_UPPERSTGVAC1ON);
	}
	else
	{
		bStatus[36] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT36, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_XY_UPPERSTGVAC1ON);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput37()
{
	if(bStatus[37])
	{
		bStatus[37] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT37, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_XY_UPPERSTGVAC1OFF);
	}
	else
	{
		bStatus[37] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT37, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_XY_UPPERSTGVAC1OFF);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput38()
{
	if(bStatus[38])
	{
		bStatus[38] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT38, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_XY_UPPERSTGVAC2ON);
	}
	else
	{
		bStatus[38] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT38, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_XY_UPPERSTGVAC2ON);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput39()
{
	if(bStatus[39])
	{
		bStatus[39] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT39, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_XY_UPPERSTGVAC2OFF);
	}
	else
	{
		bStatus[39] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT39, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_XY_UPPERSTGVAC2OFF);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput40()
{
	if(bStatus[40])
	{
		bStatus[40] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT40, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_XY_LOWERSTGPUT);
	}
	else
	{
		bStatus[40] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT40, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_XY_LOWERSTGPUT);
	}
}


void CFormTeachLDRobot::ClickGxbtnLdrobotOutput41()
{
	if(bStatus[41])
	{
		bStatus[41] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT41, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_XY_LOWERSTGVAC1ON);
	}
	else
	{
		bStatus[41] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT41, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_XY_LOWERSTGVAC1ON);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput42()
{
	if(bStatus[42])
	{
		bStatus[42] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT42, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_XY_LOWERSTGVAC1OFF);
	}
	else
	{
		bStatus[42] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT42, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_XY_LOWERSTGVAC1OFF);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput43()
{
	if(bStatus[43])
	{
		bStatus[43] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT43, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_XY_LOWERSTGVAC2ON);
	}
	else
	{
		bStatus[43] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT43, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_XY_LOWERSTGVAC2ON);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput44()
{
	if(bStatus[44])
	{
		bStatus[44] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT44, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_XY_LOWERSTGVAC2OFF);
	}
	else
	{
		bStatus[44] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT44, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_XY_LOWERSTGVAC2OFF);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput45()
{
	if(bStatus[45])
	{
		bStatus[45] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT45, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_XY_LDROBOTINTERLOCK1);
	}
	else
	{
		bStatus[45] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT45, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_XY_LDROBOTINTERLOCK1);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput46()
{
	if(bStatus[46])
	{
		bStatus[46] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT46, GXCOLOR_OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_XY_LDROBOTINTERLOCK2);
	}
	else
	{
		bStatus[46] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT46, GXCOLOR_ON);
		theDeviceMelsec.RobotBitOn(LDROBOT_XY_LDROBOTINTERLOCK2);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput47()
{
	if(bStatus[47])
	{
		bStatus[47] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT47, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[47] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT47, GXCOLOR_ON);
		long write = 4; 
		theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput48()
{
	if(bStatus[48])
	{
		bStatus[48] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT48, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[48] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT48, GXCOLOR_ON);
		long write = 8; 
		theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput49()
{
	if(bStatus[49])
	{
		bStatus[49] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT49, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[49] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT49, GXCOLOR_ON);
		long write = 16; 
		theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput50()
{
	if(bStatus[50])
	{
		bStatus[50] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT50, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[50] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT50, GXCOLOR_ON);
		long write = 32; 
		theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
}

void CFormTeachLDRobot::ClickGxbtnLdrobotOutput51()
{
	if(bStatus[51])
	{
		bStatus[51] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT51, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[51] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT51, GXCOLOR_ON);
		long write = 64; 
		theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput52()
{
	if(bStatus[52])
	{
		bStatus[52] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT52, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[52] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT52, GXCOLOR_ON);
		long write = 128; 
		theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput53()
{
	if(bStatus[53])
	{
		bStatus[53] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT53, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[53] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT53, GXCOLOR_ON);
		long write = 256; 
		theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput54()
{
	if(bStatus[54])
	{
		bStatus[54] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT54, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[54] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT54, GXCOLOR_ON);
		long write = 512; 
		theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput55()
{
	if(bStatus[55])
	{
		bStatus[55] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT55, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[55] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT55, GXCOLOR_ON);
		long write = 1024; 
		theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput56()
{
	if(bStatus[56])
	{
		bStatus[56] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT56, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[56] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT56, GXCOLOR_ON);
		long write = 2048; 
		theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput57()
{
	if(bStatus[57])
	{
		bStatus[57] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT57, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[57] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT57, GXCOLOR_ON);
		long write = 4096; 
		theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput58()
{
	if(bStatus[58])
	{
		bStatus[58] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT58, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[58] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT58, GXCOLOR_ON);
		long write = 8192; 
		theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput59()
{
	if(bStatus[59])
	{
		bStatus[59] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT59, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[59] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT59, GXCOLOR_ON);
		long write = 16384; 
		theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput60()
{
	if(bStatus[60])
	{
		bStatus[60] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT60, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[60] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT60, GXCOLOR_ON);
		long write = 32768; 
		theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
}


void CFormTeachLDRobot::ClickGxbtnLdrobotOutput61()
{
	if(bStatus[61])
	{
		bStatus[61] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT61, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[61] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT61, GXCOLOR_ON);
		long write = 65536; 
		theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput62()
{
	if(bStatus[62])
	{
		bStatus[62] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT62, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[62] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT62, GXCOLOR_ON);
		long write = 131072; 
		theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput63()
{
	if(bStatus[63])
	{
		bStatus[63] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT63, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(LDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[63] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT63, GXCOLOR_ON);
		long write = 1; 
		theDeviceMelsec.RobotWordSend(LDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput64()
{
	if(bStatus[64])
	{
		bStatus[64] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT64, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(LDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[64] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT64, GXCOLOR_ON);
		long write = 2; 
		theDeviceMelsec.RobotWordSend(LDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput65()
{
	if(bStatus[65])
	{
		bStatus[65] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT65, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(LDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[65] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT65, GXCOLOR_ON);
		long write = 4; 
		theDeviceMelsec.RobotWordSend(LDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput66()
{
	if(bStatus[66])
	{
		bStatus[66] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT66, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(LDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[66] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT66, GXCOLOR_ON);
		long write = 8; 
		theDeviceMelsec.RobotWordSend(LDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput67()
{
	if(bStatus[67])
	{
		bStatus[67] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT67, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(LDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[67] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT67, GXCOLOR_ON);
		long write = 16; 
		theDeviceMelsec.RobotWordSend(LDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput68()
{
	if(bStatus[68])
	{
		bStatus[63] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT68, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(LDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[68] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT68, GXCOLOR_ON);
		long write = 32; 
		theDeviceMelsec.RobotWordSend(LDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput69()
{
	if(bStatus[69])
	{
		bStatus[69] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT69, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(LDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[69] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT69, GXCOLOR_ON);
		long write = 64; 
		theDeviceMelsec.RobotWordSend(LDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput70()
{
	if(bStatus[70])
	{
		bStatus[70] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT70, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(LDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[70] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT70, GXCOLOR_ON);
		long write = 128; 
		theDeviceMelsec.RobotWordSend(LDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
	}
}

void CFormTeachLDRobot::ClickGxbtnLdrobotOutput71()
{
	if(bStatus[71])
	{
		bStatus[71] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT71, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(LDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[71] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT71, GXCOLOR_ON);
		long write = 256; 
		theDeviceMelsec.RobotWordSend(LDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput72()
{
	if(bStatus[72])
	{
		bStatus[72] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT72, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(LDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[72] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT72, GXCOLOR_ON);
		long write = 512; 
		theDeviceMelsec.RobotWordSend(LDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput73()
{
	if(bStatus[73])
	{
		bStatus[73] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT73, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(LDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[73] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT73, GXCOLOR_ON);
		long write = 1024; 
		theDeviceMelsec.RobotWordSend(LDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
	}
}
void CFormTeachLDRobot::ClickGxbtnLdrobotOutput74()
{
	if(bStatus[74])
	{
		bStatus[74] = FALSE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT74, GXCOLOR_OFF);
		long write = 0; 
		theDeviceMelsec.RobotWordSend(LDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
	}
	else
	{
		bStatus[74] = TRUE;
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDROBOT_OUTPUT74, GXCOLOR_ON);
		long write = 2048; 
		theDeviceMelsec.RobotWordSend(LDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
	}
}

void CFormTeachLDRobot::ClickGxbtnLdrobotGetApproach()
{
	if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MANUALMOVE_STANBY) == FALSE)
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
	if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MANUALIN_Y_MANUALSELECT) == FALSE)
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


	if(ManualPosCheck(100))
	{
		long write;
		write = 100; 
		theDeviceMelsec.RobotWordSend(LDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
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

void CFormTeachLDRobot::ClickGxbtnLdrobotGetWait()
{
	if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MANUALMOVE_STANBY) == FALSE)
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
	if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MANUALIN_Y_MANUALSELECT) == FALSE)
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


	if(ManualPosCheck(101))
	{
		long write;
		write = 101; 
		theDeviceMelsec.RobotWordSend(LDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
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


void CFormTeachLDRobot::ClickGxbtnLdrobotGetUp()
{
	if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MANUALMOVE_STANBY) == FALSE)
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
	if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MANUALIN_Y_MANUALSELECT) == FALSE)
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


	if(ManualPosCheck(110))
	{
		long write;
		write = 110; 
		theDeviceMelsec.RobotWordSend(LDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
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


void CFormTeachLDRobot::ClickGxbtnLdrobotGetDown()
{
	if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MANUALMOVE_STANBY) == FALSE)
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
	if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MANUALIN_Y_MANUALSELECT) == FALSE)
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

	if(ManualPosCheck(111))
	{
		long write;
		write = 111; 
		theDeviceMelsec.RobotWordSend(LDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
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


void CFormTeachLDRobot::ClickGxbtnLdrobotUpperApproach()
{
	if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MANUALMOVE_STANBY) == FALSE)
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
	if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MANUALIN_Y_MANUALSELECT) == FALSE)
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


	if(ManualPosCheck(200))
	{
		long write;
		write = 200; 
		theDeviceMelsec.RobotWordSend(LDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
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


void CFormTeachLDRobot::ClickGxbtnLdrobotUpperWait()
{
	if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MANUALMOVE_STANBY) == FALSE)
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
	if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MANUALIN_Y_MANUALSELECT) == FALSE)
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


	if(ManualPosCheck(201))
	{
		long write;
		write = 201; 
		theDeviceMelsec.RobotWordSend(LDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
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


void CFormTeachLDRobot::ClickGxbtnLdrobotUpperUp1()
{
	if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MANUALMOVE_STANBY) == FALSE)
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
	if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MANUALIN_Y_MANUALSELECT) == FALSE)
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


	if(ManualPosCheck(210))
	{
		long write;
		write = 210; 
		theDeviceMelsec.RobotWordSend(LDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
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


void CFormTeachLDRobot::ClickGxbtnLdrobotUpperDown1()
{
	if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MANUALMOVE_STANBY) == FALSE)
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
	if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MANUALIN_Y_MANUALSELECT) == FALSE)
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


	if(ManualPosCheck(211))
	{
		long write;
		write = 211; 
		theDeviceMelsec.RobotWordSend(LDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
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


void CFormTeachLDRobot::ClickGxbtnLdrobotUpperUp2()
{
	if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MANUALMOVE_STANBY) == FALSE)
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
	if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MANUALIN_Y_MANUALSELECT) == FALSE)
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


	if(ManualPosCheck(220))
	{
		long write;
		write = 220; 
		theDeviceMelsec.RobotWordSend(LDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
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


void CFormTeachLDRobot::ClickGxbtnLdrobotUpperDown2()
{
	if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MANUALMOVE_STANBY) == FALSE)
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
	if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MANUALIN_Y_MANUALSELECT) == FALSE)
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


	if(ManualPosCheck(221))
	{
		long write;
		write = 221; 
		theDeviceMelsec.RobotWordSend(LDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
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


void CFormTeachLDRobot::ClickGxbtnLdrobotLowerApproach()
{
	if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MANUALMOVE_STANBY) == FALSE)
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
	if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MANUALIN_Y_MANUALSELECT) == FALSE)
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

	if(ManualPosCheck(300))
	{
		long write;
		write = 300; 
		theDeviceMelsec.RobotWordSend(LDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
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


void CFormTeachLDRobot::ClickGxbtnLdrobotLowerWait()
{
	if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MANUALMOVE_STANBY) == FALSE)
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
	if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MANUALIN_Y_MANUALSELECT) == FALSE)
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

	if(ManualPosCheck(301))
	{
		long write;
		write = 301; 
		theDeviceMelsec.RobotWordSend(LDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
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


void CFormTeachLDRobot::ClickGxbtnLdrobotLowerUp()
{
	if (theUnitFunc.Chk_UpperShuttle_UnLoadPos() == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("Upper Shuttle Unload Pos Move"),
			_T("Upper Shuttle Unload Pos Move"),		
			_T("Upper Shuttle Unload Pos Move"));

		dlgMsgBox.DoModal();
	}
	else
	{
		if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MANUALMOVE_STANBY) == FALSE)
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
		if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MANUALIN_Y_MANUALSELECT) == FALSE)
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

		if(310)
		{
			long write;
			write = 310; 
			theDeviceMelsec.RobotWordSend(LDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
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


void CFormTeachLDRobot::ClickGxbtnLdrobotLowerDown()
{
	if (theUnitFunc.Chk_UpperShuttle_UnLoadPos() == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("Upper Shuttle Unload Pos Move"),
			_T("Upper Shuttle Unload Pos Move"),		
			_T("Upper Shuttle Unload Pos Move"));

		dlgMsgBox.DoModal();
	}
	else
	{
		if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MANUALMOVE_STANBY) == FALSE)
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
		if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MANUALIN_Y_MANUALSELECT) == FALSE)
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

		if(311)
		{
			long write;
			write = 311; 
			theDeviceMelsec.RobotWordSend(LDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
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


void CFormTeachLDRobot::ClickGxbtnLdrobotLowerUp2()
{
	if (theUnitFunc.Chk_UpperShuttle_UnLoadPos() == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("Upper Shuttle Unload Pos Move"),
			_T("Upper Shuttle Unload Pos Move"),		
			_T("Upper Shuttle Unload Pos Move"));

		dlgMsgBox.DoModal();
	}
	else
	{
		if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MANUALMOVE_STANBY) == FALSE)
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
		if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MANUALIN_Y_MANUALSELECT) == FALSE)
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

		if(320)
		{
			long write;
			write = 320; 
			theDeviceMelsec.RobotWordSend(LDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
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


void CFormTeachLDRobot::ClickGxbtnLdrobotLowerDown2()
{
	if (theUnitFunc.Chk_UpperShuttle_UnLoadPos() == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("Upper Shuttle Unload Pos Move"),
			_T("Upper Shuttle Unload Pos Move"),		
			_T("Upper Shuttle Unload Pos Move"));

		dlgMsgBox.DoModal();
	}
	else
	{
		if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MANUALMOVE_STANBY) == FALSE)
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
		if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MANUALIN_Y_MANUALSELECT) == FALSE)
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

		if(321)
		{
			long write;
			write = 321; 
			theDeviceMelsec.RobotWordSend(LDROBOT_MANUAL_POS,&write,sizeof(long), FALSE);
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


void CFormTeachLDRobot::ClickGxbtnLdrobotVacOn()
{
	theUnitFunc.RobotCellVacOn(VAC_ON,E_LD_ROBOT);
	theDeviceMelsec.RobotBitOn(LDROBOT_XY_VACON);
	theDeviceMelsec.RobotBitOff(LDROBOT_XY_VACOFF);
}


void CFormTeachLDRobot::ClickGxbtnLdrobotVacOff()
{
	theUnitFunc.RobotCellVacOn(VAC_OFF,E_LD_ROBOT);
	Sleep(200);
	theUnitFunc.RobotBlowOff(E_LD_ROBOT);
	theDeviceMelsec.RobotBitOff(LDROBOT_XY_VACON);
	theDeviceMelsec.RobotBitOn(LDROBOT_XY_VACOFF);
}


void CFormTeachLDRobot::ClickGxbtnLdrobotAlign()
{
	theProcBank.m_strLA_AlignOK = _T("");

	theSocketInterFace.SendMsgToMITEdgeAlign(LA_ALIGN_REQ, _T("MANUAL"), _T(""));

	SetTimer(25,2000,NULL);
}



void CFormTeachLDRobot::ClickGxbtnLdrobotMove()
{
	theDeviceMelsec.RobotBitOn(LDROBOT_X_MANUALCOMP_Y_MANUALGO);
	Sleep(200);
	theDeviceMelsec.RobotBitOff(LDROBOT_X_MANUALCOMP_Y_MANUALGO);
}


BOOL CFormTeachLDRobot::ManualPosCheck(int Pos)
{
	short Data[1];
	theDeviceMelsec.RobotWordRecvBuf(LDROBOT_MANUAL_POS,sizeof(short),Data);
	
	if(Pos == 100)
	{
		if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_HOMEPOS1_Y_CYCLESTOP)
			|| (long)Data[0] == 100 || (long)Data[0] == 101 || (long)Data[0] == 200 || (long)Data[0] == 300)
			return TRUE;
		else
			return FALSE;
	}
	else if(Pos == 101)
	{
		if((long)Data[0] == 100 || (long)Data[0] == 101 || (long)Data[0] == 110)
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
	else if(Pos == 200)
	{
		if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_HOMEPOS1_Y_CYCLESTOP)
			|| (long)Data[0] == 100 || (long)Data[0] == 200 || (long)Data[0] == 201)
			return TRUE;
		else
			return FALSE;
	}
	else if(Pos == 201)
	{
		if((long)Data[0] == 200 || (long)Data[0] == 201 || (long)Data[0] == 210 || (long)Data[0] == 220)
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
	else if(Pos == 220)
	{
		if((long)Data[0] == 201 || (long)Data[0] == 220 || (long)Data[0] == 221)
			return TRUE;
		else
			return FALSE;
	}
	else if(Pos == 221)
	{
		if((long)Data[0] == 220 || (long)Data[0] == 221)
			return TRUE;
		else
			return FALSE;
	}
	else if(Pos == 300)
	{
		if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_HOMEPOS1_Y_CYCLESTOP)
			|| (long)Data[0] == 100 || (long)Data[0] == 300 || (long)Data[0] == 301)
			return TRUE;
		else
			return FALSE;
	}
	else if(Pos == 301)
	{
		if((long)Data[0] == 300 || (long)Data[0] == 301 || (long)Data[0] == 310 || (long)Data[0] == 320)
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
	else if(Pos == 320)
	{
		if((long)Data[0] == 301 || (long)Data[0] == 320 || (long)Data[0] == 321)
			return TRUE;
		else
			return FALSE;
	}
	else if(Pos == 321)
	{
		if((long)Data[0] == 320 || (long)Data[0] == 321)
			return TRUE;
		else
			return FALSE;
	}
	return TRUE;
}
