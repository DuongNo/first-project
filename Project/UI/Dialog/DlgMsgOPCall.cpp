﻿// DlgMsgOPCall.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgMsgOPCall.h"

// #include "MainFrm.h"

#include "GUIDefine.h"

//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "UI\GausGUI\GxStaticEx.h"
#include "UI\GausGUI\GxButtonEx.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CDlgMsgOPCall 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgMsgOPCall, CDialog)

CDlgMsgOPCall::CDlgMsgOPCall(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMsgOPCall::IDD, pParent)
{

}

CDlgMsgOPCall::~CDlgMsgOPCall()
{
}

void CDlgMsgOPCall::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMsgOPCall, CDialog)

	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgMsgOPCall 메시지 처리기입니다.


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
LRESULT CDlgMsgOPCall::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{
		if(theConfigBank.m_Option.iMapLampSettingList.size() > 0)
		{

			//2017-09-12,SKCHO, LAMPCHANGE 함수 수정
			/*theUnitFunc.LampChange(theConfigBank.m_Option.iMapLampSettingList[LAMP_OP_CALL].Red.lamp_state
				,theConfigBank.m_Option.iMapLampSettingList[LAMP_OP_CALL].Yellow.lamp_state
				, theConfigBank.m_Option.iMapLampSettingList[LAMP_OP_CALL].Green.lamp_state
				,1);
				*/
			theUnitFunc.LampChange((STATUS_LAMP_DESCRIPTION)LAMP_OP_CALL,1);
		}		
	}
	else
	{

	}

	return 0;
}

// 표시 언어가 변경되었다.
LRESULT CDlgMsgOPCall::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	// CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_BTNX_BOTTOM_MENU1);

	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
BEGIN_EVENTSINK_MAP(CDlgMsgOPCall, CDialog)
	ON_EVENT(CDlgMsgOPCall, IDC_GXBTN_OK, DISPID_CLICK, CDlgMsgOPCall::ClickGxbtnOk, VTS_NONE)
END_EVENTSINK_MAP()


void CDlgMsgOPCall::ClickGxbtnOk()
{
	int i = 0;
	SYSTEMTIME AlarmTime;
	CString strTemp;
	KillTimer(1);
	
	//2017-03-22,skcho, interlock 발생하면 에러 리스트에 남게 하기 위해서	,cim 추가 내용
	for(i = 0; i < theProcBank.m_MsgOpCallCnt; i++)
	{
		if(i >= OPCALLMAX) return;

		strTemp.Format(_T("%s"),theProcBank.m_strOPCallMsg[i]);
		strTemp.TrimRight();	  

		GetLocalTime(&AlarmTime);
		//theProcBank.alarm_info.iERR_ID = ALARM_CIM_OPCALL;

		//2017-04-15,skcho, 다중알람발생시 로그에 정확한 시간 기록하기 위해서 배열로 만듦
		theProcBank.alarm_info_op[i].szERR_ID = theProcBank.m_strOPCallID;
		theProcBank.alarm_info_op[i].szERR_NAME = strTemp;
		//theProcBank.alarm_info[i].TimeOccur = CTime(AlarmTime);
		//theProcBank.alarm_info[i].szLevel = CStringSupport::N2C(ALARM_LEVEL_ALARM);	
				
		if (theProcBank.alarm_info_op[i].szERR_ID != _T(""))
		{
			theProcBank.alarm_info_op[i].TimeClear = CTime::GetCurrentTime();		
			theProcBank.alarm_info_op[i].SetTakeTime();
			theProcBank.alarm_info_op[i].WriteErrorBuf();
		}	
	}

	//20170131 byskcho
	for( i = 0; i < 10; i++)
	{
		if(theProcBank.m_strOPCallMsg[i] == _T(""))
		{
			theLog[LOG_ERROR].AddBuf(_T("OPCALL\t%d\t%s\t%s\t%d"), ALARM_CIM_OPCALL, theProcBank.m_strOPCallMsg[i],"", ALARM_LEVEL_ALARM);
		}

	}
	for ( i = 0; i < 10; i++)
		theProcBank.m_strOPCallMsg[i] = _T("");

	theProcBank.m_strOldOPCallID = _T("");
	theProcBank.m_MsgOpCallCnt = 0;
	OnOK();
}


void CDlgMsgOPCall::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	CGxStaticEx* OPCallID = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_OP_CALL_ID);
	OPCallID->SetCaption(theProcBank.m_strOPCallID);

	CGxStaticEx* OPCallMsg = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_OPCALL_MSG);
	OPCallMsg->SetCaption(theProcBank.m_strOPCallMsg[0]);

	SetTimer(1,100,NULL);
}


void CDlgMsgOPCall::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnTimer(nIDEvent);
	KillTimer(1);
	if(nIDEvent == 1)
	{

		CGxStaticEx* OPCallID = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_OP_CALL_ID);
	    OPCallID->SetCaption(theProcBank.m_strOPCallID);

		CGxStaticEx* OPCallMsg = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_OPCALL_MSG);
		OPCallMsg->SetCaption(theProcBank.m_strOPCallMsg[0]+theProcBank.m_strOPCallMsg[1]+theProcBank.m_strOPCallMsg[2]
		+theProcBank.m_strOPCallMsg[3]+theProcBank.m_strOPCallMsg[4]+theProcBank.m_strOPCallMsg[5]
		+theProcBank.m_strOPCallMsg[6]+theProcBank.m_strOPCallMsg[7]+theProcBank.m_strOPCallMsg[8]
		+theProcBank.m_strOPCallMsg[9]);

		SetTimer(1,100,NULL);
	}
}
