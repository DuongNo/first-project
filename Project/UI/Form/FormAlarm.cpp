// CFormAlarm1.cpp : 구현 파일입니다.
#include "stdafx.h"
#include "FormAlarm.h"

#include "GUIDefine.h"

//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
//kjpark 20161021 Setup 신호기 UI 구현
#include "UI\GausGUI\GxMsgBox.h"
#include "UI\GausGUI\GxStaticEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CFormAlarm1

IMPLEMENT_DYNCREATE(CFormAlarm, CFormView)

CFormAlarm::CFormAlarm()
	: CFormView(CFormAlarm::IDD)
{

}

CFormAlarm::~CFormAlarm()
{
}

void CFormAlarm::ViewAlarmMsg()
{
	CGxStaticEx* pstcAlarmName = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_ALARM_NAME);
	CGxStaticEx* pstcAlarmTime = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_ALARM_TIME);
	CGxStaticEx* pstcAlarmId = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_ALARM_ID);
	CGxStaticEx* pstcAlarmLevel = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_ALARM_LEVEL);
	CGxStaticEx* pstcAlarmPart = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_ALARM_PART);
	CGxStaticEx* pstcAlarmCause = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_ALARM_CAUSE_1);
	CGxStaticEx* pstcAlarmMethod = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_ALARM_METHOD_1);

	if(theProcBank.m_AlarmID != ALM_NONE)
	{
		CString strCaption;

		pstcAlarmName->SetCaption(theProcBank.m_AlarmMsg[theProcBank.m_AlarmID].strName[GetMainHandler()->m_nLangIdx]);

		strCaption.Format(_T("%04d-%02d-%02d %02d:%02d:%02d.%03d"), 
			theProcBank.m_AlarmTime.wYear, theProcBank.m_AlarmTime.wMonth, theProcBank.m_AlarmTime.wDay,
			theProcBank.m_AlarmTime.wHour, theProcBank.m_AlarmTime.wMinute, theProcBank.m_AlarmTime.wSecond,
			theProcBank.m_AlarmTime.wMilliseconds);
		pstcAlarmTime->SetCaption(strCaption);

		strCaption.Format(_T("%d"), theProcBank.m_AlarmID);
		pstcAlarmId->SetCaption(strCaption);

		strCaption.Format(_T("%d"), theProcBank.m_AlarmMsg[theProcBank.m_AlarmID].nAlarmLevel);
		pstcAlarmLevel->SetCaption(strCaption);

		pstcAlarmPart->SetCaption(ConstEquipPart[theProcBank.GetEqiupPart_to_AlarmID(theProcBank.m_AlarmID)].strNameKor);

		pstcAlarmCause->SetCaption(theProcBank.m_AlarmMsg[theProcBank.m_AlarmID].strCause[GetMainHandler()->m_nLangIdx]);

		pstcAlarmMethod->SetCaption(theProcBank.m_AlarmMsg[theProcBank.m_AlarmID].strMethod[GetMainHandler()->m_nLangIdx]);
	
		if(theProcBank.m_bLightAlarmChk)
			CGxUICtrl::SetStaticColor(this,IDC_GXSTC_LIGHT_ALARM, GXCOLOR_RED);
		else
			CGxUICtrl::SetStaticColor(this,IDC_GXSTC_LIGHT_ALARM, GXCOLOR_WHITE);

	}
}

void CFormAlarm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFormAlarm, CFormView)
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)				// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	ON_WM_TIMER()
END_MESSAGE_MAP()

//kjpark 20161021 Setup 신호기 UI 구현
BEGIN_EVENTSINK_MAP(CFormAlarm, CFormView)
	ON_EVENT(CFormAlarm, IDC_GXBTN_ALARM_RESET, DISPID_CLICK, CFormAlarm::ClickBtnAlarm_Reset, VTS_NONE)
	ON_EVENT(CFormAlarm, IDC_GXBTN_ALARM_RETRY, DISPID_CLICK, CFormAlarm::ClickBtnAlarm_Retry, VTS_NONE)
	ON_EVENT(CFormAlarm, IDC_GXBTN_ALARM_MUTE, DISPID_CLICK, CFormAlarm::ClickBtnAlarm_Buzzer_Mute, VTS_NONE)
	 
END_EVENTSINK_MAP()

// CFormAlarm1 진단입니다.

#ifdef _DEBUG
void CFormAlarm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormAlarm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG





// CFormAlarm1 메시지 처리기입니다.

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
LRESULT CFormAlarm::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{
		ViewAlarmMsg();
	}
	else
	{

	}

	return 0;
}

// 표시 언어가 변경되었다.
LRESULT CFormAlarm::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	CGxUICtrl::BroadcastStaticCpationIndex(this, wParam, IDC_GXLBL_ALARM_CAP1);
	CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_GXBTN_ALARM_RESET);
	ViewAlarmMsg();
	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


// 윈도우 생성전에 모양을 달리 지정한다.
BOOL CFormAlarm::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	cs.style = WS_OVERLAPPED | WS_CHILD;				// 테두리를 없앤다.

	return CFormView::PreCreateWindow(cs);
}

//kjpark 20161021 Setup 신호기 UI 구현
void CFormAlarm::ClickBtnAlarm_Reset()
{
/*CGxMsgBox	dlgMsgBox;
	CString     strMsg;
	CString     strMsg1;
	CString     strMsg2;


	if(theProcBank.GetTrayAlarmChk())
	{
		if(theProcBank.m_bFaultOfTray[2] == TRUE)//ALARM_NGTRAY_IN_PORT_ALIGN_EMPTY)
		{
			strMsg.Format(_T("NGTRAY_IN_PORT_ALIGN_EMPTY 알람으로 인해 초기화 할 수 없습니다."));
			strMsg1.Format(_T("Can not Alarm Reset, Because NGTRAY_IN_PORT_ALIGN_EMPTY Alarm"));
			strMsg2.Format(_T("Can not Alarm Reset, Because NGTRAY_IN_PORT_ALIGN_EMPTY Alarm"));

		}
		else if(theProcBank.m_bFaultOfTray[0] == TRUE)//ALARM_OKTRAY_IN_PORT_ALIGN_EMPTY)
		{
			strMsg.Format(_T("OKTRAY_IN_PORT_ALIGN_EMPTY 알람으로 인해 초기화 할 수 없습니다."));
			strMsg1.Format(_T("Can not Alarm Reset, Because OKTRAY_IN_PORT_ALIGN_EMPTY Alarm"));
			strMsg2.Format(_T("Can not Alarm Reset, Because OKTRAY_IN_PORT_ALIGN_EMPTY Alarm"));

		}
		else if(theProcBank.m_bFaultOfTray[1] == TRUE)// ALARM_OKTRAY_OUT_BUFF_PORT_FULL)
		{
			strMsg.Format(_T("OKTRAY_OUT_BUFF_PORT_FULL 알람으로 인해 초기화 할 수 없습니다."));
			strMsg1.Format(_T("Can not Alarm Reset, Because OKTRAY_OUT_BUFF_PORT_FULL Alarm"));
			strMsg2.Format(_T("Can not Alarm Reset, Because OKTRAY_OUT_BUFF_PORT_FULL Alarm"));
		}

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				strMsg,	//한
				strMsg1,		//영
				strMsg2);		//베

		dlgMsgBox.DoModal();
		return;

	}*/
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	theProcBank.AlarmClear();


	// 비가동 사유를 자동으로 입력시켜준다.
	if ( theLossTimeBank.m_rLosstimeRecord.sAlarmType	!= _T("-") )		// Alarm에 의해 멈추었다면, 
	{
		// Alarm에 의해 멈추었다면 자동으로 내용을 입력한다.
		::GetLocalTime( &(theLossTimeBank.m_rLosstimeRecord.tmResetTime) );	// Alarm Reset 시각지정
		::GetLocalTime( &(theLossTimeBank.m_rLosstimeRecord.tmEndTime) );	// Alarm 처리 종료하고 다시 시작 시간

		theLossTimeBank.m_rLosstimeRecord.s1stClass = _T("MD");
		theLossTimeBank.m_rLosstimeRecord.s2ndClass = _T("ETC");
		// pMCMain->m_rLosstimeRecord.s3rdClass = _T("ALARM CANCEL");
		theLossTimeBank.m_rLosstimeRecord.sRepairCode = _T("998");
		
		// 파일로 기록을 한다.
		//old m_pMCMain->WriteLossTimeRecord( m_pMCMain->m_rLosstimeRecord, FALSE );			 
		theLossTimeBank.WriteLossTimeRecord( theLossTimeBank.m_rLosstimeRecord, FALSE );			 
		theLossTimeBank.ClearLosstimeInfo();														// 비가동 임시기억 초기화

		//::PostMessage( ( (CMainFrame*)AfxGetMainWnd() )->m_hWnd, GXMSG_REQUEST_START, (WPARAM)1, (LPARAM)0);
		return;
	}
}

//kjpark 20161021 Setup 신호기 UI 구현
void CFormAlarm::ClickBtnAlarm_Retry()
{

	
	





}

//kjpark 20161021 Setup 신호기 UI 구현
void CFormAlarm::ClickBtnAlarm_Buzzer_Mute()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	theUnitFunc.SetOutPutIO(Y_BUZZER_K1, OFF);
	theUnitFunc.SetOutPutIO(Y_BUZZER_K2, OFF);
	theUnitFunc.SetOutPutIO(Y_BUZZER_K3, OFF);
	theUnitFunc.SetOutPutIO(Y_BUZZER_K4, OFF);
}

