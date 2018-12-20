// Dialog\DlgLossTime2.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgLossTime2.h"
#include "MainFrm.h"

// #include "afxdialogex.h"

#include "GUIDefine.h"

//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "UI\GausGUI\GxButtonEx.h"
#include "UI\GausGUI\GxStaticEx.h"
#include "UI\GausGUI\GxMsgBox.h"

// CDlgLossTime2 대화 상자입니다.

// 2017-03-20, jhLee, TP Code list 정의
#define LOSS_MSG_BM1_1	_T("BM>03000>고장(수동)>고장(수동)>Breakdown (Manual)>thất bại(thụ động)")
#define LOSS_MSG_BM2_1	_T("PM>12000>정기 PM>정기 PM>Regular PM>Đều đặn PM")
#define LOSS_MSG_BM2_2	_T("PM>15100>설비점검>설비점검>Equipment Check>Kiểm tra thiết bị")
#define LOSS_MSG_BM2_3	_T("PM>15200>품질점검>품질점검>Quality Check>Kiểm tra chất lượng")
#define LOSS_MSG_BM3_1	_T("CM>17200>설비 개량 보전(EE)>설비 개량 보전(EE)>Improve Equipment(EE)>Cải tiến máy móc(EE)")
#define LOSS_MSG_BM3_2	_T("CM>17300>공정 개량 보전(EI)>공정 개량 보전(EI)>Improve Processing(EI)>cải tiến kỹ thuật(EI)")
#define LOSS_MSG_BM3_3	_T("CM>14000>신제품 SETUP>신제품 SETUP>Setup New Product>Thay đổi mô hình mới")
#define LOSS_MSG_BM4_1	_T("Change Model>41100>동품종교체>동품종교체>Change Same Model>Thay đổi Mô hình Tương tự")
#define LOSS_MSG_BM4_2	_T("Change Model>41200>이품종교체>이품종교체>Change Different Model>Thay đổi mô hình khác nhau")
#define LOSS_MSG_BM5_1	_T("Material>35000>자재교체>자재교체>Change Material>thay đổi vật liệu")
#define LOSS_MSG_BM5_2	_T("Material>51000>자재DOWN>자재 DOWN>Down Material>Sự thiếu hụt nguyên vật liệu")


IMPLEMENT_DYNAMIC(CDlgLosstime2, CDialog)

CDlgLosstime2::CDlgLosstime2(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLosstime2::IDD, pParent)
{
	

	// 선택할 수 있는 버튼 Resource ID
	m_mapItems.SetAt(LOSS_MSG_BM1_1,	IDC_GXBTN_LOSSTIME2_ITEM1_1);
	m_mapItems.SetAt(LOSS_MSG_BM2_1,	IDC_GXBTN_LOSSTIME2_ITEM2_1);
	m_mapItems.SetAt(LOSS_MSG_BM2_2,	IDC_GXBTN_LOSSTIME2_ITEM2_2);
	m_mapItems.SetAt(LOSS_MSG_BM2_3, IDC_GXBTN_LOSSTIME2_ITEM2_3);
	m_mapItems.SetAt(LOSS_MSG_BM3_1, IDC_GXBTN_LOSSTIME2_ITEM3_1);
	m_mapItems.SetAt(LOSS_MSG_BM3_2, IDC_GXBTN_LOSSTIME2_ITEM3_2);
	m_mapItems.SetAt(LOSS_MSG_BM3_3, IDC_GXBTN_LOSSTIME2_ITEM3_3);
	m_mapItems.SetAt(LOSS_MSG_BM4_1,	IDC_GXBTN_LOSSTIME2_ITEM4_1);
	m_mapItems.SetAt(LOSS_MSG_BM4_2,	IDC_GXBTN_LOSSTIME2_ITEM4_2);
	m_mapItems.SetAt(LOSS_MSG_BM5_1,	IDC_GXBTN_LOSSTIME2_ITEM5_1);
	m_mapItems.SetAt(LOSS_MSG_BM5_2,	IDC_GXBTN_LOSSTIME2_ITEM5_2);
}

CDlgLosstime2::~CDlgLosstime2()
{
	m_mapItems.RemoveAll();
}

void CDlgLosstime2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgLosstime2, CDialog)
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

//	ON_COMMAND(IDCANCEL, &CDlgLosstime::OnIdcancel)
END_MESSAGE_MAP()


// CDlgLossTime2 메시지 처리기입니다.



// 화면 내용을 Update한다.
LRESULT CDlgLosstime2::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	if ( wParam == 1 )
	{
		CString sFile;
		//m_pMCMain		= CMCMain::GetInstance();

		DispClear();

		// 이전에 선택된 내용이 있다면 점멸시켜준다.
		DispTPCode( theLossTimeBank.m_rLosstimeRecord.sReasonLine );
	}
	else 
	{
		DispClear();			// Blink 시킨 버튼을 초기화 한다.
	}

	return 0;
}


// Blink 시킨 버튼을 초기화 한다.
void CDlgLosstime2::DispClear()
{
		UINT nBtnID = 0;
		POSITION pos = NULL;
		CString sKey;

		pos = m_mapItems.GetStartPosition();
		while( pos != NULL ) 
		{
			nBtnID = 0;

			m_mapItems.GetNextAssoc(pos, sKey, nBtnID);			// Data를 인출한다.

			if ( nBtnID > 0 )	
			{
				CGxUICtrl::SetButtonBlink(this, nBtnID, FALSE);
			}
		}
}



//	afx_msg LRESULT OnChangeLanguage(WPARAM wParam, LPARAM lParam);		// 표시 언어 변경
//	ON_MESSAGE(UM_DISP_LANG_CHANGE,		OnChangeLanguage)			// 표시 언어 변경

//설정되어 있는 프로그램 언어로 변경한다.
LRESULT CDlgLosstime2::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_GXBTN_LOSSTIME2_CANCEL);

	return 0;
}

//
//// 취소 버튼 클릭
//void CDlgLosstime::ClickGxbtn_Cancel()
//{
//	ShowWindow(SW_HIDE);							// 화면을 감춘다.
//}

// 취소 선택
//void CDlgLosstime::OnIdcancel()
//{
//	ShowWindow(SW_HIDE);							// 화면을 감춘다.
//}
//
BEGIN_EVENTSINK_MAP(CDlgLosstime2, CDialog)
	ON_EVENT(CDlgLosstime2, IDC_GXBTN_LOSSTIME2_CANCEL, DISPID_CLICK, CDlgLosstime2::ClickGxbtnLosstime2Cancel, VTS_NONE)
	ON_EVENT(CDlgLosstime2, IDC_GXBTN_LOSSTIME2_ITEM1_1, DISPID_CLICK, CDlgLosstime2::ClickBtn_Item1_1, VTS_NONE)
	ON_EVENT(CDlgLosstime2, IDC_GXBTN_LOSSTIME2_ITEM2_1, DISPID_CLICK, CDlgLosstime2::ClickBtn_Item2_1, VTS_NONE)
	ON_EVENT(CDlgLosstime2, IDC_GXBTN_LOSSTIME2_ITEM2_2, DISPID_CLICK, CDlgLosstime2::ClickBtn_Item2_2, VTS_NONE)
	ON_EVENT(CDlgLosstime2, IDC_GXBTN_LOSSTIME2_ITEM2_3, DISPID_CLICK, CDlgLosstime2::ClickBtn_Item2_3, VTS_NONE)
	ON_EVENT(CDlgLosstime2, IDC_GXBTN_LOSSTIME2_ITEM3_1, DISPID_CLICK, CDlgLosstime2::ClickBtn_Item3_1, VTS_NONE)
	ON_EVENT(CDlgLosstime2, IDC_GXBTN_LOSSTIME2_ITEM3_2, DISPID_CLICK, CDlgLosstime2::ClickBtn_Item3_2, VTS_NONE)
	ON_EVENT(CDlgLosstime2, IDC_GXBTN_LOSSTIME2_ITEM3_3, DISPID_CLICK, CDlgLosstime2::ClickBtn_Item3_3, VTS_NONE)
	ON_EVENT(CDlgLosstime2, IDC_GXBTN_LOSSTIME2_ITEM4_1, DISPID_CLICK, CDlgLosstime2::ClickBtn_Item4_1, VTS_NONE)
	ON_EVENT(CDlgLosstime2, IDC_GXBTN_LOSSTIME2_ITEM4_2, DISPID_CLICK, CDlgLosstime2::ClickBtn_Item4_2, VTS_NONE)
	ON_EVENT(CDlgLosstime2, IDC_GXBTN_LOSSTIME2_ITEM5_1, DISPID_CLICK, CDlgLosstime2::ClickBtn_Item5_1, VTS_NONE)
	ON_EVENT(CDlgLosstime2, IDC_GXBTN_LOSSTIME2_ITEM5_2, DISPID_CLICK, CDlgLosstime2::ClickBtn_Item5_2, VTS_NONE)

END_EVENTSINK_MAP()


void CDlgLosstime2::ClickGxbtnLosstime2Cancel()
{
	DispClear();									// Blink 시킨 버튼을 초기화 한다.
	ShowWindow(SW_HIDE);							// 화면을 감춘다.
}


void CDlgLosstime2::OnCancel()
{
	DispClear();									// Blink 시킨 버튼을 초기화 한다.
	ShowWindow(SW_HIDE);							// 화면을 감춘다.
	
	CDialog::OnCancel();
}


void CDlgLosstime2::OnOK()
{
	DispClear();									// Blink 시킨 버튼을 초기화 한다.
	ShowWindow(SW_HIDE);							// 화면을 감춘다.

	CDialog::OnOK();
}


void CDlgLosstime2::ClickBtn_Item1_1()
{
	SetTPCode( LOSS_MSG_BM1_1 );
}


void CDlgLosstime2::ClickBtn_Item2_1()
{
	SetTPCode( LOSS_MSG_BM2_1 );
}


void CDlgLosstime2::ClickBtn_Item2_2()
{
	SetTPCode( LOSS_MSG_BM2_2 );
}


void CDlgLosstime2::ClickBtn_Item2_3()
{
	SetTPCode( LOSS_MSG_BM2_3 );
}

void CDlgLosstime2::ClickBtn_Item3_1()
{
	SetTPCode( LOSS_MSG_BM3_1 );
}


void CDlgLosstime2::ClickBtn_Item3_2()
{
	SetTPCode( LOSS_MSG_BM3_2 );
}


void CDlgLosstime2::ClickBtn_Item3_3()
{
	SetTPCode( LOSS_MSG_BM3_3 );
}

void CDlgLosstime2::ClickBtn_Item4_1()
{
	SetTPCode( LOSS_MSG_BM4_1 );
}


void CDlgLosstime2::ClickBtn_Item4_2()
{	
	SetTPCode( LOSS_MSG_BM4_2 );
}

void CDlgLosstime2::ClickBtn_Item5_1()
{
	SetTPCode( LOSS_MSG_BM5_1 );
}


void CDlgLosstime2::ClickBtn_Item5_2()
{
	SetTPCode( LOSS_MSG_BM5_2 );
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 지정 비가동 사유에 해당되는 버튼을 점멸시킨다.
void CDlgLosstime2::DispTPCode(CString sLine)
{
	if ( sLine.IsEmpty() == TRUE ) return;								// 보여줄 것이 없다.

	CString sKey;
	UINT nBtnID = 0;

	if ( m_mapItems.Lookup( sLine, nBtnID ) == TRUE )
	{
		if ( nBtnID > 0 )
		{
			CGxUICtrl::SetButtonBlink(this, nBtnID, TRUE);
		}
	}
}



// 지정 비가동 사유를 적용한다
void CDlgLosstime2::SetTPCode(CString sLine)
{
	if ( sLine.IsEmpty() ) return;											// 빈 문자열은 무시한다.

	///CGxMaster*	pMaster		= CGxMaster::GetInstance();				// System 흐름 제어용
	CString sMsg = _T("");

	// 선택된 비가동 사유 취득
	// m_sSelectReason = "PM>000>Shift점검>프로그램 점검>Check Program"
	::AfxExtractSubString( theLossTimeBank.m_rLosstimeRecord.s1stClass,		sLine, 0, DELIMITER_DEFECT);			// 1st class
	::AfxExtractSubString( theLossTimeBank.m_rLosstimeRecord.sTPSystemCode,sLine, 1, DELIMITER_DEFECT);			// MESCode
	::AfxExtractSubString( theLossTimeBank.m_rLosstimeRecord.s2ndClass,		sLine, 2, DELIMITER_DEFECT);			// 2nd class
	::AfxExtractSubString( theLossTimeBank.m_rLosstimeRecord.s3rdClass,		sLine, 3, DELIMITER_DEFECT);			// 3rd class
	::AfxExtractSubString( theLossTimeBank.m_rLosstimeRecord.sTPSystemName, sLine, 4, DELIMITER_DEFECT);	// 3rd class 영문 = TPSystem 설명


	//2017-03-25, skcho,작업자가 없으므로 삭제
	//d theLossTimeBank.m_rLosstimeRecord.sTPSystemCode	= theLossTimeBank.m_rLosstimeRecord.sMESCode;				// TPSystem Code
	theLossTimeBank.m_rLosstimeRecord.sID					= m_sWorkUser;//theLossTimeBank.GetCurrentUserID();			// 현재 작업자 ID로 대입
	theLossTimeBank.m_rLosstimeRecord.sIDNo				= m_sWorkUser;//theLossTimeBank.GetCurrentUserIDNo();
	theLossTimeBank.m_rLosstimeRecord.sReasonLine		= sLine;						// 선택된 내용


	// Auto -> Stop으로의 전환인가 ?
	if ( theLossTimeBank.m_rLosstimeRecord.nMode == eMODE_STATE_AutoToStop )
	{
		// 시작 시간을 갱신한다. 
		::GetLocalTime( &(theLossTimeBank.m_rLosstimeRecord.tmStartTime) );	// 현재 시각으로 초기화 치환한다.
		::GetLocalTime( &(theLossTimeBank.m_rLosstimeRecord.tmResetTime) );	
		::GetLocalTime( &(theLossTimeBank.m_rLosstimeRecord.tmEndTime) );

		theLossTimeBank.m_sTPSystemCode = theLossTimeBank.m_rLosstimeRecord.sTPSystemCode;	// TPSystem CODE 상위에 보고되는 변수에 저장 20141226-2 jhkim1
		theLossTimeBank.m_sTPSystemName = theLossTimeBank.m_rLosstimeRecord.sTPSystemName;	// TPSystem NAME 상위에 보고되는 변수에 저장 20141226-2 jhkim1

		// 선택된 내용 저장
		// 현재 설정된 비가동 정보를 파일로 기록한다. (임시보관)
		theLossTimeBank.SaveLosstimeInfo();
		
		//2017-03-25, skcho,임시 삭제rundown 기능 구현되지 않음
		//theLossTimeBank.ClearRunDownTimer();					// AutoRunDown 타이머를 초기화 해준다.
		
		//2017-07-20, jsjung, 설비 IDLE->PAUSE 변경시 IDLE TIME 종료
		if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)
		{
			::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_IDLECHAGNERUN_UPDATE, 0,0); 
			theLog[LOG_LOAD_DELAY_TACT].AddBuf(_T("설비 IDLE->PAUSE 변경"));
		}
		//2017-05-20, skcho, 정지 상태 보고하고 TP CODE  보고
		theProcBank.ChangeMachineState(CONST_MACHINE_STATE::MACHINE_PAUSE);

		SleepEx(25, NULL);			// 2015-03-07, jhLee, 멈추는 현상 목격, 곧바로 전환하지 않고 약간의 시간을 두고 수행하도록 Sleep을 주었다. 

		//2017-03-25,skcho, 변경
		//-pMaster->SetStop();			// Stop으로 변경
		theProcBank.m_bUserStop = TRUE;
		// 상위 보고는 MainFrm에서 상태 변경 메세지를 받아서 전송한다.
		//2014-12-24, jhLee, 상태 변경 후 상위에 TPSystem Code를 전송한다.
		//theLossTimeBank.CIM_TPSCodeReport(theLossTimeBank.m_rLosstimeRecord.sTPSystemCode, theLossTimeBank.m_rLosstimeRecord.sTPSystemName);	// 기존에는 코드 먼저 보내고 상태 보고했는데 상태 보고후 코드 보냄으로 주석 20141226-2 jhkim1
	    //2017-03-25,skcho 추가
	    theSocketInterFace.SendCmdTPSystemToDataPC(theLossTimeBank.m_rLosstimeRecord.sTPSystemCode, theLossTimeBank.m_rLosstimeRecord.sTPSystemName);
	
		theUnitFunc.SendRcvEnable(OFF);
		theUnitFunc.SendRcvStart(OFF);
		theUnitFunc.SendRcvComp(OFF);

		theUnitFunc.SetOutPutIO(Y_START_SW_LAMP,OFF);
		theUnitFunc.SetOutPutIO(Y_STOP_SW_LAMP,ON);

	}
	else		// STOP -> AUTO로의 전환인가 ?
	{
		if ( theLossTimeBank.m_rLosstimeRecord.sAlarmType != _T("A") )					// Alarm에 의한 정지가 아니 였다면,
		{
			// MD가 아닌경우 무의미
			::GetLocalTime( &(theLossTimeBank.m_rLosstimeRecord.tmResetTime) );			// Reset 시간을 현재 시각으로 맞춘다.
		}

		// 비가동 종료 시각
		::GetLocalTime( &(theLossTimeBank.m_rLosstimeRecord.tmEndTime) );				// 처리 종료하고 다시 시작 시간

		//2017-03-25,skcho,  수정
		// Login과는 다른 사용자 입력이다.
		//theLossTimeBank.m_rLosstimeRecord.sID		= theLossTimeBank.GetCurrentUserID();		// 현재 작업자 ID로 대입, 로그인한 사용자가 아닌, 현재 폼에서 입력한 작업자를 기입힌다.
		//theLossTimeBank.m_rLosstimeRecord.sIDNo	= theLossTimeBank.GetCurrentUserIDNo();
		theLossTimeBank.m_rLosstimeRecord.sID	= m_sWorkUser;	
		theLossTimeBank.m_rLosstimeRecord.sIDNo	= m_sWorkUser;


		//20170118 byskho 임시삭제rundown 기능 구현되지 않음
		/*
		// 중지 사유에 따른 처리
		// Auto RunDown에 의해 중지 되었다면,
		if ( (theLossTimeBank.m_rLosstimeRecord.s2ndClass == _T("AUTO RUN DOWN")) || (theLossTimeBank.m_rLosstimeRecord.s2ndClass == _T("AUTO RUNDOWN") ) )
		{
			// AUTO RUNDOWN에 의한 중지였다면,
			//d if ( theLossTimeBank.m_rLosstimeRecord.sReasonLine == theLossTimeBank.m_rLosstimeList.sRunDownCode )
			//d {
	 		// theLossTimeBank.WriteLosstimeData( theLossTimeBank.m_rProfileData.nTIME_RunDownTime );	

			// 비가동 Log 저장시 AUTO RUn down 기준 시간을 더하여 기록한다.
			theLossTimeBank.m_rLosstimeRecord.dbAddLossTime =  theLossTimeBank.m_rProfileData.nTIME_RunDownTime;
		}
		else
	 		// theLossTimeBank.WriteLosstimeData( 0 );														// 비가동 Log 저장
			theLossTimeBank.m_rLosstimeRecord.dbAddLossTime = 0.0;
         */

		// 파일로 기록을 한다.
		//old theLossTimeBank.WriteLossTimeRecord( theLossTimeBank.m_rLosstimeRecord, FALSE );			 
		theLossTimeBank.WriteLossTimeRecord( theLossTimeBank.m_rLosstimeRecord, FALSE );			 
		theLossTimeBank.ClearLosstimeInfo();														// 비가동 임시기억 초기화

		//2017-03-25,skcho, 삭제
		//theLossTimeBank.m_rProfileData.bTACTStartTimeUpdateFlag	= FALSE;			// 정상적인 방법으로 STOP으로 전환여부 Flag 초기화
		//theLossTimeBank.ClearRunDownTimer();					// RunDown 타이머를 초기화 해준다.

		//2017-03-25,skcho, 변경
		//pMaster->SetAuto();			// Auto 지정
		theProcBank.m_bUserStop = TRUE;
		/*Operator가 없는 설비 이므로 삭제
		// Defect을 입력해야하는 Mode라면,
		if ( (theLossTimeBank.GetDefectInputMode() == TRUE) && (theLossTimeBank.m_rProfileData.bUse_Robot == FALSE) )
		{
			::PostMessage(((CMainFrame*)AfxGetMainWnd())->m_hWnd, UDMSG_DISPFROM_DEFECT, 1, 0);		// Defect 입력 폼으로 전환 
		}
		*/
	} //of if

	ShowWindow(SW_HIDE);							// 화면을 감춘다.
}