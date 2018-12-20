// FormConfig1.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FormAlarmLamp.h"
#include "UI\GausGUI\GxMsgBox.h"
//#include "UI\Dialog\DlgBoxKeyBoard.h"
#include "UI\Dialog\DlgBoxPassWord.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



IMPLEMENT_DYNCREATE(CFormAlarmLamp, CFormView)

CFormAlarmLamp::CFormAlarmLamp()
	: CFormView(CFormAlarmLamp::IDD)
{
	Init();	
}

CFormAlarmLamp::~CFormAlarmLamp()
{
}

void CFormAlarmLamp::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFormAlarmLamp, CFormView)
	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	//ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
END_MESSAGE_MAP()

//kjpark 20161022 설정->램프 설정 구현
BEGIN_EVENTSINK_MAP(CFormAlarmLamp, CFormView)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_BUTTON_SAVE, DISPID_CLICK, CFormAlarmLamp::OnButtonSave, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_INIT_RED, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_INIT_Red, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_INIT_YELLOW, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_INIT_Yellow, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_INIT_GREEN, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_INIT_Green, VTS_NONE)

	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_IDLE_RED, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_IDLE_Red, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_IDLE_YELLOW, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_IDLE_Yellow, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_IDLE_GREEN, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_IDLE_Green, VTS_NONE)

	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_STOP_RED, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_STOP_Red, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_STOP_YELLOW, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_STOP_Yellow, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_STOP_GREEN, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_STOP_Green, VTS_NONE)

	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_RUN_RED, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_RUN_Red, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_RUN_YELLOW, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_RUN_Yellow, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_RUN_GREEN, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_RUN_Green, VTS_NONE)

	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_ALARM_RED, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_ALARM_Red, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_ALARM_YELLOW, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_ARARM_Yellow, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_ALARM_GREEN, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_ARARM_Green, VTS_NONE)

	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_PM_RED, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_PM_Red, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_PM_YELLOW, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_PM_Yellow, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_PM_GREEN, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_PM_Green, VTS_NONE)

	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_OPCALL_RED, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_OPCALL_Red, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_OPCALL_YELLOW, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_OPCALL_Yellow, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_OPCALL_GREEN, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_OPCALL_Green, VTS_NONE)

	ON_EVENT(CFormAlarmLamp, IDC_GXBTN_ALARM_LAMP_TITLE_INIT, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_Title_Init, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXBTN_ALARM_LAMP_TITLE_IDLE, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_Title_Idle, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXBTN_ALARM_LAMP_TITLE_STOP, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_Title_Stop, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXBTN_ALARM_LAMP_TITLE_RUN, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_Title_Run, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXBTN_ALARM_LAMP_TITLE_ALARM, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_Title_Ararm, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXBTN_ALARM_LAMP_TITLE_PM, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_Title_Pm, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXBTN_ALARM_LAMP_TITLE_OPCALL, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_Title_OPCall, VTS_NONE)

	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_AGV_RED, DISPID_CLICK, CFormAlarmLamp::ClickGxlblAlarm4TitleAgvRed, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_AGV_YELLOW, DISPID_CLICK, CFormAlarmLamp::ClickGxlblAlarm4TitleAgvYellow, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_AGV_GREEN, DISPID_CLICK, CFormAlarmLamp::ClickGxlblAlarm4TitleAgvGreen, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXBTN_ALARM_LAMP_TITLE_AGV, DISPID_CLICK, CFormAlarmLamp::ClickGxbtnAlarmLampTitleAgv, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXBTN_ALARM_LAMP_TITLE_CHECK, DISPID_CLICK, CFormAlarmLamp::ClickGxbtnAlarmLampTitleCheck, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_CHECK_RED, DISPID_CLICK, CFormAlarmLamp::ClickGxlblAlarm4TitleCheckRed, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_CHECK_YELLOW, DISPID_CLICK, CFormAlarmLamp::ClickGxlblAlarm4TitleCheckYellow, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_CHECK_GREEN, DISPID_CLICK, CFormAlarmLamp::ClickGxlblAlarm4TitleCheckGreen, VTS_NONE)
END_EVENTSINK_MAP()
// CFormConfig1 진단입니다.
//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonSave()
{
	//2017-11-03, YMG 램프 설정 변경 시 비밀번호
	//2017-11-03, YMG 램프 설정 변경 시 비밀번호
	//2018-03-03,SKCHO,PASSWORD 다이얼로그 추가
	//CDlgBoxKeyBoard dlgKey;
	CDlgBoxPassWord dlgKey;
	CGxMsgBox	dlgMsgBox;
	
	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
	dlgMsgBox.SetMessage(TRUE, 
		_T("TOWER LAMP 값을 저장하시겠습니까?"), 
		_T("Do you want to save the TOWER LAMP value?"),		
		_T("B?n co mu?n l?u cac gia tr? TOWER LAMP?") );
	
	int nRet = dlgMsgBox.DoModal();
	if(nRet == IDOK)
	{
		if ( dlgKey.DoModal() == IDOK )
		{
			if (dlgKey.m_strReturnText != theConfigBank.m_System.m_strLampPassword)
			{
		
				CGxMsgBox	dlgMsgBox;

				dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
				dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
				dlgMsgBox.SetMessage(FALSE, 
					_T("틀린 비밀번호 입니다."), 
					_T("Password Wrong"),		
					_T("M?t m?t kh?u sai.") );

				dlgMsgBox.DoModal();
				return;
			}
		}
		else
			return;
	}
	else
		return;

	SaveAlarmFile();
	ReadAlarmLabpFile();
	setLamp();
	
	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Notify"), _T("Notify"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("저장 완료."), 
			_T("Save complete."),		
			_T("l?u") );
		dlgMsgBox.DoModal();
		return;
}
#ifdef _DEBUG
void CFormAlarmLamp::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormAlarmLamp::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

//kjpark 20161022 설정->램프 설정 구현
LRESULT CFormAlarmLamp::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{
// 		Init();
		setLamp();
	}
	else
	{
	}

	return 0;
}


// CFormConfig1 메시지 처리기입니다.

//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_INIT_Red()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[LAMP_INIT].Red.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[LAMP_INIT].Red.lamp_state = (LAMP_STATE)nState;
// 	theConfigBank.m_Option.iMapLampSettingList[LAMP_INIT].Red.lamp_state = ! theConfigBank.m_Option.iMapLampSettingList[LAMP_INIT].Red.bCheck;
	setLamp();
}
//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_INIT_Yellow()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[LAMP_INIT].Yellow.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[LAMP_INIT].Yellow.lamp_state = (LAMP_STATE)nState;
//	theConfigBank.m_Option.iMapLampSettingList[LAMP_INIT].Yellow.lamp_state = ! theConfigBank.m_Option.iMapLampSettingList[LAMP_INIT].Yellow.bCheck;
	setLamp();
}
//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_INIT_Green()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[LAMP_INIT].Green.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[LAMP_INIT].Green.lamp_state = (LAMP_STATE)nState;
// 	theConfigBank.m_Option.iMapLampSettingList[LAMP_INIT].Green.lamp_state = ! theConfigBank.m_Option.iMapLampSettingList[LAMP_INIT].Green.bCheck;
	setLamp();
}

//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_IDLE_Red()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[LAMP_IDLE].Red.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[LAMP_IDLE].Red.lamp_state = (LAMP_STATE)nState;
// 	theConfigBank.m_Option.iMapLampSettingList[LAMP_IDLE].Red.lamp_state = ! theConfigBank.m_Option.iMapLampSettingList[LAMP_IDLE].Red.bCheck;
	setLamp();
}
//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_IDLE_Yellow()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[LAMP_IDLE].Yellow.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[LAMP_IDLE].Yellow.lamp_state = (LAMP_STATE)nState;
// 	theConfigBank.m_Option.iMapLampSettingList[LAMP_IDLE].Yellow.lamp_state = ! theConfigBank.m_Option.iMapLampSettingList[LAMP_IDLE].Yellow.bCheck;
	setLamp();
}
//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_IDLE_Green()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[LAMP_IDLE].Green.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[LAMP_IDLE].Green.lamp_state = (LAMP_STATE)nState;
// 	theConfigBank.m_Option.iMapLampSettingList[LAMP_IDLE].Green.lamp_state = ! theConfigBank.m_Option.iMapLampSettingList[LAMP_IDLE].Green.bCheck;
	setLamp();
}

//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_STOP_Red()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[LAMP_STOP].Red.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[LAMP_STOP].Red.lamp_state = (LAMP_STATE)nState;
// 	theConfigBank.m_Option.iMapLampSettingList[LAMP_STOP].Red.lamp_state = ! theConfigBank.m_Option.iMapLampSettingList[LAMP_STOP].Red.bCheck;
	setLamp();
}

//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_STOP_Yellow()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[LAMP_STOP].Yellow.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[LAMP_STOP].Yellow.lamp_state = (LAMP_STATE)nState;
// 	theConfigBank.m_Option.iMapLampSettingList[LAMP_STOP].Yellow.bCheck = ! theConfigBank.m_Option.iMapLampSettingList[LAMP_STOP].Yellow.bCheck;
	setLamp();
}

//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_STOP_Green()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[LAMP_STOP].Green.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[LAMP_STOP].Green.lamp_state = (LAMP_STATE)nState;
// 	theConfigBank.m_Option.iMapLampSettingList[LAMP_STOP].Green.bCheck = ! theConfigBank.m_Option.iMapLampSettingList[LAMP_STOP].Green.bCheck;
	setLamp();
}


//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_RUN_Red()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[LAMP_RUN].Red.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[LAMP_RUN].Red.lamp_state = (LAMP_STATE)nState;
// 	theConfigBank.m_Option.iMapLampSettingList[LAMP_RUN].Red.bCheck = ! theConfigBank.m_Option.iMapLampSettingList[LAMP_RUN].Red.bCheck;
	setLamp();
}
//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_RUN_Yellow()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[LAMP_RUN].Yellow.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[LAMP_RUN].Yellow.lamp_state = (LAMP_STATE)nState;
// 	theConfigBank.m_Option.iMapLampSettingList[LAMP_RUN].Yellow.bCheck = ! theConfigBank.m_Option.iMapLampSettingList[LAMP_RUN].Yellow.bCheck;
	setLamp();
}
//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_RUN_Green()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[LAMP_RUN].Green.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[LAMP_RUN].Green.lamp_state = (LAMP_STATE)nState;
// 	theConfigBank.m_Option.iMapLampSettingList[LAMP_RUN].Green.bCheck = ! theConfigBank.m_Option.iMapLampSettingList[LAMP_RUN].Green.bCheck;
	setLamp();
}
	 
//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_ALARM_Red()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[LAMP_ALARM].Red.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[LAMP_ALARM].Red.lamp_state = (LAMP_STATE)nState;
// 	theConfigBank.m_Option.iMapLampSettingList[LAMP_ALARM].Red.bCheck = ! theConfigBank.m_Option.iMapLampSettingList[LAMP_ALARM].Red.bCheck;
	setLamp();
}
//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_ARARM_Yellow()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[LAMP_ALARM].Yellow.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[LAMP_ALARM].Yellow.lamp_state = (LAMP_STATE)nState;
// 	theConfigBank.m_Option.iMapLampSettingList[LAMP_ALARM].Yellow.bCheck = ! theConfigBank.m_Option.iMapLampSettingList[LAMP_ALARM].Yellow.bCheck;
	setLamp();
}
//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_ARARM_Green()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[LAMP_ALARM].Green.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[LAMP_ALARM].Green.lamp_state = (LAMP_STATE)nState;
// 	theConfigBank.m_Option.iMapLampSettingList[LAMP_ALARM].Green.bCheck = ! theConfigBank.m_Option.iMapLampSettingList[LAMP_ALARM].Green.bCheck;
	setLamp();
}
	 
//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_PM_Red()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[LAMP_PM].Red.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[LAMP_PM].Red.lamp_state = (LAMP_STATE)nState;
// 	theConfigBank.m_Option.iMapLampSettingList[LAMP_PM].Red.bCheck = ! theConfigBank.m_Option.iMapLampSettingList[LAMP_PM].Red.bCheck;
	setLamp();
}
//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_PM_Yellow()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[LAMP_PM].Yellow.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[LAMP_PM].Yellow.lamp_state = (LAMP_STATE)nState;
// 	theConfigBank.m_Option.iMapLampSettingList[LAMP_PM].Yellow.bCheck = ! theConfigBank.m_Option.iMapLampSettingList[LAMP_PM].Yellow.bCheck;
	setLamp();
}
//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_PM_Green()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[LAMP_PM].Green.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[LAMP_PM].Green.lamp_state = (LAMP_STATE)nState;
// 	theConfigBank.m_Option.iMapLampSettingList[LAMP_PM].Green.bCheck = ! theConfigBank.m_Option.iMapLampSettingList[LAMP_PM].Green.bCheck;
	setLamp();
}
	 
//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_OPCALL_Red()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[LAMP_OP_CALL].Red.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[LAMP_OP_CALL].Red.lamp_state = (LAMP_STATE)nState;
// 	theConfigBank.m_Option.iMapLampSettingList[LAMP_OP_CALL].Red.bCheck = ! theConfigBank.m_Option.iMapLampSettingList[LAMP_OP_CALL].Red.bCheck;
	setLamp();
}
//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_OPCALL_Yellow()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[LAMP_OP_CALL].Yellow.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[LAMP_OP_CALL].Yellow.lamp_state = (LAMP_STATE)nState;
// 	theConfigBank.m_Option.iMapLampSettingList[LAMP_OP_CALL].Yellow.bCheck = ! theConfigBank.m_Option.iMapLampSettingList[LAMP_OP_CALL].Yellow.bCheck;
	setLamp();
}
//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_OPCALL_Green()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[LAMP_OP_CALL].Green.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[LAMP_OP_CALL].Green.lamp_state = (LAMP_STATE)nState;
// 	theConfigBank.m_Option.iMapLampSettingList[LAMP_OP_CALL].Green.bCheck = ! theConfigBank.m_Option.iMapLampSettingList[LAMP_OP_CALL].Green.bCheck;
	setLamp();
}

//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_Title_Init()
{
	map<STATUS_LAMP_DESCRIPTION, LAMP_SETTING_LIST>::iterator itmap = theConfigBank.m_Option.iMapLampSettingList.begin();
	for(; itmap != theConfigBank.m_Option.iMapLampSettingList.end(); ++ itmap)
	{	
		itmap->second.bSelect = FALSE;	
	}

	theConfigBank.m_Option.iMapLampSettingList[LAMP_INIT].bSelect = TRUE;
	setLamp();
}
//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_Title_Idle()
{
	map<STATUS_LAMP_DESCRIPTION, LAMP_SETTING_LIST>::iterator itmap = theConfigBank.m_Option.iMapLampSettingList.begin();
	for(; itmap != theConfigBank.m_Option.iMapLampSettingList.end(); ++ itmap)
	{	
		itmap->second.bSelect = FALSE;	
	}

	theConfigBank.m_Option.iMapLampSettingList[LAMP_IDLE].bSelect = TRUE;
	setLamp();
}
//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_Title_Stop()
{
	map<STATUS_LAMP_DESCRIPTION, LAMP_SETTING_LIST>::iterator itmap = theConfigBank.m_Option.iMapLampSettingList.begin();
	for(; itmap != theConfigBank.m_Option.iMapLampSettingList.end(); ++ itmap)
	{	
		itmap->second.bSelect = FALSE;	
	}

	theConfigBank.m_Option.iMapLampSettingList[LAMP_STOP].bSelect = TRUE;
	setLamp();
}
//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_Title_Run()
{
	map<STATUS_LAMP_DESCRIPTION, LAMP_SETTING_LIST>::iterator itmap = theConfigBank.m_Option.iMapLampSettingList.begin();
	for(; itmap != theConfigBank.m_Option.iMapLampSettingList.end(); ++ itmap)
	{	
		itmap->second.bSelect = FALSE;	
	}

	theConfigBank.m_Option.iMapLampSettingList[LAMP_RUN].bSelect = TRUE;
	setLamp();
}
//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_Title_Ararm()
{
	map<STATUS_LAMP_DESCRIPTION, LAMP_SETTING_LIST>::iterator itmap = theConfigBank.m_Option.iMapLampSettingList.begin();
	for(; itmap != theConfigBank.m_Option.iMapLampSettingList.end(); ++ itmap)
	{	
		itmap->second.bSelect = FALSE;	
	}

	theConfigBank.m_Option.iMapLampSettingList[LAMP_ALARM].bSelect = TRUE;
	setLamp();
}
//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_Title_Pm()
{
	map<STATUS_LAMP_DESCRIPTION, LAMP_SETTING_LIST>::iterator itmap = theConfigBank.m_Option.iMapLampSettingList.begin();
	for(; itmap != theConfigBank.m_Option.iMapLampSettingList.end(); ++ itmap)
	{	
		itmap->second.bSelect = FALSE;	
	}

	theConfigBank.m_Option.iMapLampSettingList[LAMP_PM].bSelect = TRUE;
	setLamp();
}
//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_Title_OPCall()
{
	map<STATUS_LAMP_DESCRIPTION, LAMP_SETTING_LIST>::iterator itmap = theConfigBank.m_Option.iMapLampSettingList.begin();
	for(; itmap != theConfigBank.m_Option.iMapLampSettingList.end(); ++ itmap)
	{	
		itmap->second.bSelect = FALSE;	
	}

	theConfigBank.m_Option.iMapLampSettingList[LAMP_OP_CALL].bSelect = TRUE;
	setLamp();
}

//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::Init()
{
	InitLamp();
	InitList();
}
//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::InitLamp()
{
	iMapLamp.insert(pair<UINT, BOOL>(IDC_GXLBL_ALARM4_GRAUND_RED_BUTTON, FALSE));	
	iMapLamp.insert(pair<UINT, BOOL>(IDC_GXLBL_ALARM4_GRAUND_YELLOW_BUTTON, FALSE));	
	iMapLamp.insert(pair<UINT, BOOL>(IDC_GXLBL_ALARM4_GRAUND_GREEN_BUTTON, FALSE));

	//파일을 읽어와서 BOOL을 재정의
	if(this->GetSafeHwnd() != NULL)
	{
		CGxUICtrl::SetButtonColor(this,IDC_GXLBL_ALARM4_GRAUND_RED_BUTTON, GXCOLOR_OFF);
		CGxUICtrl::SetButtonColor(this,IDC_GXLBL_ALARM4_GRAUND_YELLOW_BUTTON, GXCOLOR_OFF);
		CGxUICtrl::SetButtonColor(this,IDC_GXLBL_ALARM4_GRAUND_GREEN_BUTTON, GXCOLOR_OFF);
	}
	
	
}
//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::SetDisplay(map<STATUS_LAMP_DESCRIPTION, LAMP_SETTING_LIST>::iterator iter)
{
	if(iter->second.Red.lamp_state == LAMP_ON)
	{
		CGxUICtrl::SetButtonColor(this,IDC_GXLBL_ALARM4_GRAUND_RED_BUTTON, GXCOLOR_RED);
		CGxUICtrl::SetButtonColor(this,iter->second.Red.nResourceID, GXCOLOR_ON);		
		CGxUICtrl::SetButtonString(this, iter->second.Red.nResourceID, _T("ON"));		
	}
	else if(iter->second.Red.lamp_state == LAMP_FLICKER)
	{
		CGxUICtrl::SetButtonColor(this,IDC_GXLBL_ALARM4_GRAUND_RED_BUTTON, GXCOLOR_RED);
		CGxUICtrl::SetButtonColor(this,iter->second.Red.nResourceID, GXCOLOR_YELLOW);		
		CGxUICtrl::SetButtonString(this, iter->second.Red.nResourceID, _T("FLICK"));		
	}
	else
	{
		CGxUICtrl::SetButtonColor(this,IDC_GXLBL_ALARM4_GRAUND_RED_BUTTON, GXCOLOR_OFF);
		CGxUICtrl::SetButtonColor(this,iter->second.Red.nResourceID, GXCOLOR_OFF);		
		CGxUICtrl::SetButtonString(this,iter->second.Red.nResourceID, _T("OFF"));		
	}

	if(iter->second.Yellow.lamp_state == LAMP_ON)
	{
		CGxUICtrl::SetButtonColor(this,IDC_GXLBL_ALARM4_GRAUND_YELLOW_BUTTON, GXCOLOR_YELLOW);
		CGxUICtrl::SetButtonColor(this,iter->second.Yellow.nResourceID, GXCOLOR_ON);		
		CGxUICtrl::SetButtonString(this,iter->second.Yellow.nResourceID, _T("ON"));		
	}
	else if(iter->second.Yellow.lamp_state == LAMP_FLICKER)
	{
		CGxUICtrl::SetButtonColor(this,IDC_GXLBL_ALARM4_GRAUND_YELLOW_BUTTON, GXCOLOR_YELLOW);
		CGxUICtrl::SetButtonColor(this,iter->second.Yellow.nResourceID, GXCOLOR_YELLOW);		
		CGxUICtrl::SetButtonString(this, iter->second.Yellow.nResourceID, _T("FLICK"));		
	}
	else
	{
		CGxUICtrl::SetButtonColor(this,IDC_GXLBL_ALARM4_GRAUND_YELLOW_BUTTON, GXCOLOR_OFF);
		CGxUICtrl::SetButtonColor(this,iter->second.Yellow.nResourceID, GXCOLOR_OFF);
		CGxUICtrl::SetButtonString(this,iter->second.Yellow.nResourceID, _T("OFF"));
	}

	if(iter->second.Green.lamp_state == LAMP_ON)
	{
		CGxUICtrl::SetButtonColor(this,IDC_GXLBL_ALARM4_GRAUND_GREEN_BUTTON, GXCOLOR_GREEN);
		CGxUICtrl::SetButtonColor(this,iter->second.Green.nResourceID, GXCOLOR_ON);		
		CGxUICtrl::SetButtonString(this,iter->second.Green.nResourceID, _T("ON"));		
	}
	else if(iter->second.Green.lamp_state == LAMP_FLICKER)
	{
		CGxUICtrl::SetButtonColor(this,IDC_GXLBL_ALARM4_GRAUND_GREEN_BUTTON, GXCOLOR_GREEN);
		CGxUICtrl::SetButtonColor(this,iter->second.Green.nResourceID, GXCOLOR_YELLOW);		
		CGxUICtrl::SetButtonString(this, iter->second.Green.nResourceID, _T("FLICK"));		
	}
	else
	{
		CGxUICtrl::SetButtonColor(this,IDC_GXLBL_ALARM4_GRAUND_GREEN_BUTTON, GXCOLOR_OFF);
		CGxUICtrl::SetButtonColor(this,iter->second.Green.nResourceID, GXCOLOR_OFF);
		CGxUICtrl::SetButtonString(this,iter->second.Green.nResourceID, _T("OFF"));
	}
// 
// 	theUnitFunc.SetOutPutIO(Y_INSP_TOWER_LAMP_RED, iter->second.Red.bCheck);
// 	theUnitFunc.SetOutPutIO(Y_INSP_TOWER_LAMP_YELLOW, iter->second.Yellow.bCheck);
// 	theUnitFunc.SetOutPutIO(Y_INSP_TOWER_LAMP_GREEN, iter->second.Green.bCheck);
// 
// 	theUnitFunc.SetOutPutIO(Y_LOADER_TOWERLAMP_RED, iter->second.Red.bCheck);
// 	theUnitFunc.SetOutPutIO(Y_LOADER_TOWERLAMP_YELLOW, iter->second.Yellow.bCheck);
// 	theUnitFunc.SetOutPutIO(Y_LOADER_TOWERLAMP_GREEN, iter->second.Green.bCheck);
// 
// 	theUnitFunc.SetOutPutIO(Y_UNLOADER_TOWER_LAMP_RED, iter->second.Red.bCheck);
// 	theUnitFunc.SetOutPutIO(Y_UNLOADER_TOWER_LAMP_YELLOW, iter->second.Yellow.bCheck);
// 	theUnitFunc.SetOutPutIO(Y_UNLOADER_TOWER_LAMP_GREEN, iter->second.Green.bCheck);
}

//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::setLamp()
{
	if(this->GetSafeHwnd() != NULL)
	{
		map<STATUS_LAMP_DESCRIPTION, LAMP_SETTING_LIST>::iterator itmap = theConfigBank.m_Option.iMapLampSettingList.begin();
		for(; itmap != theConfigBank.m_Option.iMapLampSettingList.end(); ++ itmap)
		{	
			SetDisplay(itmap);				
		}


		itmap = theConfigBank.m_Option.iMapLampSettingList.begin();
		for(; itmap != theConfigBank.m_Option.iMapLampSettingList.end(); ++ itmap)
		{			
			if(itmap->second.bSelect)
			{
				SetDisplay(itmap);				
				CGxUICtrl::SetButtonColor(this,itmap->second.nResourceID, GXCOLOR_ON);
			}
			else
			{
				CGxUICtrl::SetButtonColor(this,itmap->second.nResourceID, GXCOLOR_OFF);
			}
		}
		
	}
	//2017-09-12,SKCHO, LAMPCHANGE 함수 수정
	int nState = theProcBank.GetMachineState();
	/*theUnitFunc.LampChange(theConfigBank.m_Option.iMapLampSettingList[(STATUS_LAMP_DESCRIPTION)nState].Red.lamp_state
		,theConfigBank.m_Option.iMapLampSettingList[(STATUS_LAMP_DESCRIPTION)nState].Yellow.lamp_state
		, theConfigBank.m_Option.iMapLampSettingList[(STATUS_LAMP_DESCRIPTION)nState].Green.lamp_state
		,0);
		*/
	theUnitFunc.LampChange((STATUS_LAMP_DESCRIPTION)nState,0);
}

//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::InitList()
{
// 	if(GetMainHandler() == NULL)
// 		return;

	LAMP_SETTING_LIST_BUTTON setting;
	LAMP_SETTING_LIST button;
	setting.lamp_state = LAMP_OFF;

	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_INIT_RED;
	setting.strColor = _T("RED");
	button.Red = setting;
	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_INIT_YELLOW;
	setting.strColor = _T("YELLOW");
	button.Yellow = setting;
	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_INIT_GREEN;
	setting.strColor = _T("GREEN");
	button.Green = setting;	
	button.strList = _T("LAMP_INIT");
	button.bSelect = TRUE;
	button.nResourceID = IDC_GXBTN_ALARM_LAMP_TITLE_INIT;
	theConfigBank.m_Option.iMapLampSettingList.insert(pair<STATUS_LAMP_DESCRIPTION, LAMP_SETTING_LIST>(LAMP_INIT, button));

	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_IDLE_RED;
	setting.strColor = _T("RED");
	button.Red = setting;
	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_IDLE_YELLOW;
	setting.strColor = _T("YELLOW");
	button.Yellow = setting;
	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_IDLE_GREEN;
	setting.strColor = _T("GREEN");
	button.Green = setting;
	button.strList = _T("LAMP_IDLE");
	button.bSelect = FALSE;
	button.nResourceID = IDC_GXBTN_ALARM_LAMP_TITLE_IDLE;
	theConfigBank.m_Option.iMapLampSettingList.insert(pair<STATUS_LAMP_DESCRIPTION, LAMP_SETTING_LIST>(LAMP_IDLE, button));	

	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_STOP_RED;
	setting.strColor = _T("RED");
	button.Red = setting;
	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_STOP_YELLOW;
	setting.strColor = _T("YELLOW");
	button.Yellow = setting;
	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_STOP_GREEN;
	setting.strColor = _T("GREEN");
	button.Green = setting;
	button.strList = _T("LAMP_STOP");
	button.bSelect = FALSE;
	button.nResourceID = IDC_GXBTN_ALARM_LAMP_TITLE_STOP;
	theConfigBank.m_Option.iMapLampSettingList.insert(pair<STATUS_LAMP_DESCRIPTION, LAMP_SETTING_LIST>(LAMP_STOP, button));	

	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_RUN_RED;
	setting.strColor = _T("RED");
	button.Red = setting;
	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_RUN_YELLOW;
	setting.strColor = _T("YELLOW");
	button.Yellow = setting;
	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_RUN_GREEN;
	setting.strColor = _T("GREEN");
	button.Green = setting;
	button.strList = _T("LAMP_RUN");
	button.bSelect = FALSE;
	button.nResourceID = IDC_GXBTN_ALARM_LAMP_TITLE_RUN;
	theConfigBank.m_Option.iMapLampSettingList.insert(pair<STATUS_LAMP_DESCRIPTION, LAMP_SETTING_LIST>(LAMP_RUN, button));	

	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_ALARM_RED;
	setting.strColor = _T("RED");
	button.Red = setting;
	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_ALARM_YELLOW;
	setting.strColor = _T("YELLOW");
	button.Yellow = setting;
	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_ALARM_GREEN;
	setting.strColor = _T("GREEN");
	button.Green = setting;
	button.strList = _T("LAMP_ALARM");
	button.bSelect = FALSE;
	button.nResourceID = IDC_GXBTN_ALARM_LAMP_TITLE_ALARM;
	theConfigBank.m_Option.iMapLampSettingList.insert(pair<STATUS_LAMP_DESCRIPTION, LAMP_SETTING_LIST>(LAMP_ALARM, button));	

	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_PM_RED;
	setting.strColor = _T("RED");
	button.Red = setting;
	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_PM_YELLOW;
	setting.strColor = _T("YELLOW");
	button.Yellow = setting;
	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_PM_GREEN;
	setting.strColor = _T("GREEN");
	button.Green = setting;
	button.strList = _T("LAMP_PM");
	button.bSelect = FALSE;
	button.nResourceID = IDC_GXBTN_ALARM_LAMP_TITLE_PM;
	theConfigBank.m_Option.iMapLampSettingList.insert(pair<STATUS_LAMP_DESCRIPTION, LAMP_SETTING_LIST>(LAMP_PM, button));	

	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_OPCALL_RED;
	setting.strColor = _T("RED");
	button.Red = setting;
	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_OPCALL_YELLOW;
	setting.strColor = _T("YELLOW");
	button.Yellow = setting;
	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_OPCALL_GREEN;
	setting.strColor = _T("GREEN");
	button.Green = setting;
	button.strList = _T("LAMP_OP_CALL");
	button.bSelect = FALSE;
	button.nResourceID = IDC_GXBTN_ALARM_LAMP_TITLE_OPCALL;
	theConfigBank.m_Option.iMapLampSettingList.insert(pair<STATUS_LAMP_DESCRIPTION, LAMP_SETTING_LIST>(LAMP_OP_CALL, button));	


	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_AGV_RED;
	setting.strColor = _T("RED");
	button.Red = setting;
	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_AGV_YELLOW;
	setting.strColor = _T("YELLOW");
	button.Yellow = setting;
	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_AGV_GREEN;
	setting.strColor = _T("GREEN");
	button.Green = setting;
	button.strList = _T("LAMP_AGV");
	button.bSelect = FALSE;
	button.nResourceID = IDC_GXBTN_ALARM_LAMP_TITLE_AGV;
	theConfigBank.m_Option.iMapLampSettingList.insert(pair<STATUS_LAMP_DESCRIPTION, LAMP_SETTING_LIST>(LAMP_AGV, button));	

	//2018-04-20,SKCHO CHECK 상태 추가
	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_CHECK_RED;
	setting.strColor = _T("RED");
	button.Red = setting;
	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_CHECK_YELLOW;
	setting.strColor = _T("YELLOW");
	button.Yellow = setting;
	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_CHECK_GREEN;
	setting.strColor = _T("GREEN");
	button.Green = setting;
	button.strList = _T("LAMP_CHECK");
	button.bSelect = FALSE;
	button.nResourceID = IDC_GXBTN_ALARM_LAMP_TITLE_CHECK;
	theConfigBank.m_Option.iMapLampSettingList.insert(pair<STATUS_LAMP_DESCRIPTION, LAMP_SETTING_LIST>(LAMP_CHECK, button));	


	ReadAlarmLabpFile();

}

//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::ReadAlarmLabpFile()
{
	CString strPath;
	strPath.Format(_T("%s\\AlarmLamp.ini"), theConfigBank.m_System.m_strConfigPath);
	ReadFile(strPath);
}

//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::SaveAlarmFile()
{
	CTime time = CTime::GetCurrentTime();
	CString strPath, strBackupPath;
	strPath.Format(_T("%s\\AlarmLamp.ini"), theConfigBank.m_System.m_strConfigPath);
	strBackupPath.Format(_T("%s\\%d_%d\\AlarmLamp%s.ini"), theConfigBank.m_System.m_strConfigBackupPath, 
		time.GetYear(), time.GetMonth(), time.Format(_T("%H%M%S")));


	CString strFolderPath = CFileSupport::GetPathName(strPath);
	if(CFileSupport::DirectoryCheck(strFolderPath) == FALSE)
	{
		if(CFileSupport::CreateDirectory(strFolderPath) == FALSE)
			return;
	}
	strFolderPath = CFileSupport::GetPathName(strBackupPath);
	if(CFileSupport::DirectoryCheck(strFolderPath) == FALSE)
	{
		if(CFileSupport::CreateDirectory(strFolderPath) == FALSE)
			return;
	}

	if(CFileSupport::FileCheck(strPath))
	{
		// 기존 파일 백업
		::CopyFile(strPath, strBackupPath, FALSE);
		// File 내용을 깔끔하게 하기 위해 기존껀 지우고 새로 적는다
		::DeleteFile(strPath);
	}
		SaveFile(strPath);
}


//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::ReadFile( CString strFilePath )
{
	// 파일 입출력은 티칭값과 속도만 한다.
	CIni ini(strFilePath);

	map<STATUS_LAMP_DESCRIPTION, LAMP_SETTING_LIST>::iterator itmap = theConfigBank.m_Option.iMapLampSettingList.begin();
	for(; itmap != theConfigBank.m_Option.iMapLampSettingList.end(); ++ itmap)
	{
		itmap->second.Red.lamp_state = (LAMP_STATE)ini.GetLong(itmap->second.strList, itmap->second.Red.strColor);
		itmap->second.Yellow.lamp_state = (LAMP_STATE)ini.GetLong(itmap->second.strList, itmap->second.Yellow.strColor);
		itmap->second.Green.lamp_state = (LAMP_STATE)ini.GetLong(itmap->second.strList, itmap->second.Green.strColor);
	}
}

//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::SaveFile( CString strFilePath)
{
	// 파일 입출력은 티칭값과 속도속만 한다.
	CIni ini(strFilePath);

	map<STATUS_LAMP_DESCRIPTION, LAMP_SETTING_LIST>::iterator itmap = theConfigBank.m_Option.iMapLampSettingList.begin();
	for(; itmap != theConfigBank.m_Option.iMapLampSettingList.end(); ++ itmap)
	{
		LAMP_SETTING_LIST list = itmap->second;
		ini.WriteLong(list.strList, list.Red.strColor, list.Red.lamp_state);
		ini.WriteLong(list.strList, list.Yellow.strColor, list.Yellow.lamp_state);
		ini.WriteLong(list.strList, list.Green.strColor, list.Green.lamp_state);
	}	
}


void CFormAlarmLamp::ClickGxlblAlarm4TitleAgvRed()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[LAMP_AGV].Red.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[LAMP_AGV].Red.lamp_state = (LAMP_STATE)nState;
// 	theConfigBank.m_Option.iMapLampSettingList[LAMP_OP_CALL].Red.bCheck = ! theConfigBank.m_Option.iMapLampSettingList[LAMP_OP_CALL].Red.bCheck;
	setLamp();
}


void CFormAlarmLamp::ClickGxlblAlarm4TitleAgvYellow()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[LAMP_AGV].Yellow.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[LAMP_AGV].Yellow.lamp_state = (LAMP_STATE)nState;
// 	theConfigBank.m_Option.iMapLampSettingList[LAMP_OP_CALL].Red.bCheck = ! theConfigBank.m_Option.iMapLampSettingList[LAMP_OP_CALL].Red.bCheck;
	setLamp();
}


void CFormAlarmLamp::ClickGxlblAlarm4TitleAgvGreen()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[LAMP_AGV].Green.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[LAMP_AGV].Green.lamp_state = (LAMP_STATE)nState;
// 	theConfigBank.m_Option.iMapLampSettingList[LAMP_OP_CALL].Red.bCheck = ! theConfigBank.m_Option.iMapLampSettingList[LAMP_OP_CALL].Red.bCheck;
	setLamp();
}


void CFormAlarmLamp::ClickGxbtnAlarmLampTitleAgv()
{
	map<STATUS_LAMP_DESCRIPTION, LAMP_SETTING_LIST>::iterator itmap = theConfigBank.m_Option.iMapLampSettingList.begin();
	for(; itmap != theConfigBank.m_Option.iMapLampSettingList.end(); ++ itmap)
	{	
		itmap->second.bSelect = FALSE;	
	}

	theConfigBank.m_Option.iMapLampSettingList[LAMP_AGV].bSelect = TRUE;
	setLamp();
}


void CFormAlarmLamp::ClickGxbtnAlarmLampTitleCheck()
{
	map<STATUS_LAMP_DESCRIPTION, LAMP_SETTING_LIST>::iterator itmap = theConfigBank.m_Option.iMapLampSettingList.begin();
	for(; itmap != theConfigBank.m_Option.iMapLampSettingList.end(); ++ itmap)
	{	
		itmap->second.bSelect = FALSE;	
	}

	theConfigBank.m_Option.iMapLampSettingList[LAMP_CHECK].bSelect = TRUE;
	setLamp();
}


void CFormAlarmLamp::ClickGxlblAlarm4TitleCheckRed()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[LAMP_CHECK].Red.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[LAMP_CHECK].Red.lamp_state = (LAMP_STATE)nState;
// 	theConfigBank.m_Option.iMapLampSettingList[LAMP_OP_CALL].Red.bCheck = ! theConfigBank.m_Option.iMapLampSettingList[LAMP_OP_CALL].Red.bCheck;
	setLamp();
}


void CFormAlarmLamp::ClickGxlblAlarm4TitleCheckYellow()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[LAMP_CHECK].Yellow.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[LAMP_CHECK].Yellow.lamp_state = (LAMP_STATE)nState;
// 	theConfigBank.m_Option.iMapLampSettingList[LAMP_OP_CALL].Red.bCheck = ! theConfigBank.m_Option.iMapLampSettingList[LAMP_OP_CALL].Red.bCheck;
	setLamp();
}


void CFormAlarmLamp::ClickGxlblAlarm4TitleCheckGreen()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[LAMP_CHECK].Green.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[LAMP_CHECK].Green.lamp_state = (LAMP_STATE)nState;
// 	theConfigBank.m_Option.iMapLampSettingList[LAMP_OP_CALL].Red.bCheck = ! theConfigBank.m_Option.iMapLampSettingList[LAMP_OP_CALL].Red.bCheck;
	setLamp();
}
