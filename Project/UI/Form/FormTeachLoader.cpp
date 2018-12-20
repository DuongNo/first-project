// FormTeachLoader.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "resource.h"
#include "FormTeachLoader.h"
#include "GUIDefine.h"

//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "UI\GausGUI\GxButtonEx.h"
#include "UI\GausGUI\GxStaticEx.h"

#include "UI\GausGUI\GxMsgBox.h"
//#include "UI\Dialog\DlgBoxKeyBoard.h"
#include "UI\Dialog\DlgBoxPassWord.h"
// CFormTeachLoader

IMPLEMENT_DYNCREATE(CFormTeachLoader, CFormView)

CFormTeachLoader::CFormTeachLoader()
	: CFormView(CFormTeachLoader::IDD)
{
	m_bInit = FALSE;
	m_nASeq = seqA_idle;
	m_nBSeq = seqB_idle;
	m_nCSeq = seqC_idle;
	m_nDSeq = seqD_idle;
}

CFormTeachLoader::~CFormTeachLoader()
{
}

void CFormTeachLoader::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFormTeachLoader, CFormView)
	ON_WM_TIMER()
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)			// 표시 언어가 바뀌었다.
END_MESSAGE_MAP()


// CFormTeachLoader 진단입니다.

#ifdef _DEBUG
void CFormTeachLoader::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormTeachLoader::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormTeachLoader 메시지 처리기입니다.


BOOL CFormTeachLoader::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style = WS_OVERLAPPED | WS_CHILD;				// 테두리를 없앤다.

	return CFormView::PreCreateWindow(cs);
}
BEGIN_EVENTSINK_MAP(CFormTeachLoader, CFormView)
	ON_EVENT(CFormTeachLoader, IDC_GXBTN_LD_CONV_PITCH, DISPID_CLICK, CFormTeachLoader::ClickGxbtnLdConvPitch, VTS_NONE)
	ON_EVENT(CFormTeachLoader, IDC_GXBTN_LD_ROBOT_CONV_GET_POS, DISPID_CLICK, CFormTeachLoader::ClickGxbtnLdRobotConvGetPos, VTS_NONE)
	ON_EVENT(CFormTeachLoader, IDC_GXBTN_LD_ROBOT_SHUTTLE1_CH1_PUT, DISPID_CLICK, CFormTeachLoader::ClickGxbtnLdRobotShuttle1Ch1Put, VTS_NONE)
	ON_EVENT(CFormTeachLoader, IDC_GXBTN_LD_ROBOT_SHUTTLE1_CH2_PUT, DISPID_CLICK, CFormTeachLoader::ClickGxbtnLdRobotShuttle1Ch2Put, VTS_NONE)
	ON_EVENT(CFormTeachLoader, IDC_GXBTN_LD_ROBOT_SHUTTLE2_CH1_PUT, DISPID_CLICK, CFormTeachLoader::ClickGxbtnLdRobotShuttle2Ch1Put, VTS_NONE)
	ON_EVENT(CFormTeachLoader, IDC_GXBTN_LD_ROBOT_SHUTTLE2_PUT, DISPID_CLICK, CFormTeachLoader::ClickGxbtnLdRobotShuttle2Put, VTS_NONE)
	ON_EVENT(CFormTeachLoader, IDC_GXBTN_LD_ROBOT_VAC_ON, DISPID_CLICK, CFormTeachLoader::ClickGxbtnLdRobotVacOn, VTS_NONE)
	ON_EVENT(CFormTeachLoader, IDC_GXBTN_LD_ROBOT_VAC_OFF, DISPID_CLICK, CFormTeachLoader::ClickGxbtnLdRobotVacOff, VTS_NONE)
	ON_EVENT(CFormTeachLoader, IDC_GXBTN_LD_SHUTTLE_SHUTTLE1_LOAD, DISPID_CLICK, CFormTeachLoader::ClickGxbtnLdShuttleShuttle1Load, VTS_NONE)
	ON_EVENT(CFormTeachLoader, IDC_GXBTN_LD_SHUTTLE_SHUTTLE1_UNLOAD, DISPID_CLICK, CFormTeachLoader::ClickGxbtnLdShuttleShuttle1Unload, VTS_NONE)
	ON_EVENT(CFormTeachLoader, IDC_GXBTN_LD_SHUTTLE_SHUTTLE1_VAC_CH1_ON, DISPID_CLICK, CFormTeachLoader::ClickGxbtnLdShuttleShuttle1VacCh1On, VTS_NONE)
	ON_EVENT(CFormTeachLoader, IDC_GXBTN_LD_SHUTTLE_SHUTTLE1_VAC_CH1_OFF, DISPID_CLICK, CFormTeachLoader::ClickGxbtnLdShuttleShuttle1VacCh1Off, VTS_NONE)
	ON_EVENT(CFormTeachLoader, IDC_GXBTN_LD_SHUTTLE_SHUTTLE1_VAC_CH2_ON, DISPID_CLICK, CFormTeachLoader::ClickGxbtnLdShuttleShuttle1VacCh2On, VTS_NONE)
	ON_EVENT(CFormTeachLoader, IDC_GXBTN_LD_SHUTTLE_SHUTTLE1_VAC_CH2_OFF, DISPID_CLICK, CFormTeachLoader::ClickGxbtnLdShuttleShuttle1VacCh2Off, VTS_NONE)
	ON_EVENT(CFormTeachLoader, IDC_GXBTN_LD_SHUTTLE_SHUTTLE2_LOAD, DISPID_CLICK, CFormTeachLoader::ClickGxbtnLdShuttleShuttle2Load, VTS_NONE)
	ON_EVENT(CFormTeachLoader, IDC_GXBTN_LD_SHUTTLE_SHUTTLE2_UNLOAD, DISPID_CLICK, CFormTeachLoader::ClickGxbtnLdShuttleShuttle2Unload, VTS_NONE)
	ON_EVENT(CFormTeachLoader, IDC_GXBTN_LD_SHUTTLE_SHUTTLE2_VAC_CH1_ON, DISPID_CLICK, CFormTeachLoader::ClickGxbtnLdShuttleShuttle2VacCh1On, VTS_NONE)
	ON_EVENT(CFormTeachLoader, IDC_GXBTN_LD_SHUTTLE_SHUTTLE2_VAC_CH1_OFF, DISPID_CLICK, CFormTeachLoader::ClickGxbtnLdShuttleShuttle2VacCh1Off, VTS_NONE)
	ON_EVENT(CFormTeachLoader, IDC_GXBTN_LD_SHUTTLE_SHUTTLE2_VAC_CH2_ON, DISPID_CLICK, CFormTeachLoader::ClickGxbtnLdShuttleShuttle2VacCh2On, VTS_NONE)
	ON_EVENT(CFormTeachLoader, IDC_GXBTN_LD_SHUTTLE_SHUTTLE2_VAC_CH2_OFF, DISPID_CLICK, CFormTeachLoader::ClickGxbtnLdShuttleShuttle2VacCh2Off, VTS_NONE)
	ON_EVENT(CFormTeachLoader, IDC_GXBTN_LD_ROBOT_ORG, DISPID_CLICK, CFormTeachLoader::ClickGxbtnLdRobotOrg, VTS_NONE)
	ON_EVENT(CFormTeachLoader, IDC_GXBTN_EDGE_ALIGN_START, DISPID_CLICK, CFormTeachLoader::ClickGxbtnEdgeAlignStart, VTS_NONE)
	ON_EVENT(CFormTeachLoader, IDC_GXSTC_EDGE_ALIGN_TEACHING, DISPID_CLICK, CFormTeachLoader::ClickGxstcEdgeAlignTeaching, VTS_NONE)
END_EVENTSINK_MAP()


void CFormTeachLoader::OnInitialUpdate()
{
	if(m_bInit)	return;	// OnInitialUpdate 두번 실행되는 버그 방지 [10/24/2016 OSC]
	m_bInit = TRUE;
	CFormView::OnInitialUpdate();

	m_CurAxis = (AXIS_ID)AXIS_SVO_LOAD_CONV;
	m_CurTeach = TEACH_PARAM::LD_ROBOT_CONV;
	m_CurID	  = IDC_GXBTN_LD_CONV_PITCH;
	nCh = JIG_CH_1;
}

LRESULT CFormTeachLoader::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{
		UpdateLoaderState();
		SetTimer(1, 100, NULL);
	}
	else
	{
		KillTimer(1);
	}
	return 0;
}

void CFormTeachLoader::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
	case 1:
		UpdateLoaderState();
		break;
	case 2:
		LDRobotOrg();
		if(m_nASeq != seqA_idle)
			SetTimer(2, 50, NULL);
		break;
	case 3:
		LDRobotGet();
		if(m_nBSeq != seqB_idle)
			SetTimer(3, 50, NULL);
		break;
	case 4:
		LDRobotUpperPut();
		if(m_nCSeq != seqC_idle)
			SetTimer(4, 50, NULL);
		break;
	case 5:
		LDRobotLowerPut();
		if(m_nDSeq != seqD_idle)
			SetTimer(5, 50, NULL);
		break;
	case 25:
		KillTimer(25);
		if (theProcBank.m_strLA_AlignOK == _T("OK"))
		{
			CGxUICtrl::SetStaticColor(this, IDC_GXSTC_EDGE_ALIGN_OK, GXCOLOR_ON);
			CGxUICtrl::SetStaticString(this, IDC_GXSTC_EDGE_ALIGN_OK, _T("ALIGN OK"));

			CGxUICtrl::SetStaticColor(this, IDC_GXSTC_ALIGN_XDATA, GXCOLOR_ON);
			CGxUICtrl::SetStaticString(this, IDC_GXSTC_ALIGN_XDATA, theProcBank.m_strLAResX);
			CGxUICtrl::SetStaticColor(this, IDC_GXSTC_ALIGN_YDATA, GXCOLOR_ON);
			CGxUICtrl::SetStaticString(this, IDC_GXSTC_ALIGN_YDATA, theProcBank.m_strLAResY);
			CGxUICtrl::SetStaticColor(this, IDC_GXSTC_ALIGN_TDATA, GXCOLOR_ON);
			CGxUICtrl::SetStaticString(this, IDC_GXSTC_ALIGN_TDATA, theProcBank.m_strLAResT);
		}
		else
		{
			CGxUICtrl::SetStaticColor(this, IDC_GXSTC_EDGE_ALIGN_OK, GXCOLOR_RED);
			CGxUICtrl::SetStaticString(this, IDC_GXSTC_EDGE_ALIGN_OK, _T("ALIGN NG"));
			CGxUICtrl::SetStaticColor(this, IDC_GXSTC_ALIGN_XDATA, GXCOLOR_RED);
			CGxUICtrl::SetStaticString(this, IDC_GXSTC_ALIGN_XDATA, _T("NG"));
			CGxUICtrl::SetStaticColor(this, IDC_GXSTC_ALIGN_YDATA, GXCOLOR_RED);
			CGxUICtrl::SetStaticString(this, IDC_GXSTC_ALIGN_YDATA, _T("NG"));
			CGxUICtrl::SetStaticColor(this, IDC_GXSTC_ALIGN_TDATA, GXCOLOR_RED);
			CGxUICtrl::SetStaticString(this, IDC_GXSTC_ALIGN_TDATA, _T("NG"));
		}
		break;
	case 26:
		KillTimer(26);
		if (theProcBank.m_strLA_TeachingOK == _T("OK"))
		{
			CGxUICtrl::SetStaticColor(this, IDC_GXSTC_EDGE_ALIGN_TEACHING_OK, GXCOLOR_ON);
			CGxUICtrl::SetStaticString(this, IDC_GXSTC_EDGE_ALIGN_TEACHING_OK, _T("TEACHING OK"));
		}
		else
		{
			CGxUICtrl::SetStaticColor(this, IDC_GXSTC_EDGE_ALIGN_TEACHING_OK, GXCOLOR_RED);
			CGxUICtrl::SetStaticString(this, IDC_GXSTC_EDGE_ALIGN_TEACHING_OK, _T("TEACHING NG"));
		}
		break;
	case 27:
		// LoadConv 티칭 구동 시에 셀 감지 되면 세운다 2017/05/01 HSK.
		if(theUnitFunc.LDConvCellChk(CONV_SEN_3))
		{
			KillTimer(27);
			theUnitFunc.LDConvStop();
		}
	}

	CFormView::OnTimer(nIDEvent);
}

LRESULT CFormTeachLoader::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	CGxUICtrl::BroadcastStaticCpationIndex(this, wParam, IDC_GXSTC_LD_CONV);
	CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_GXBTN_LD_CONV_PITCH);

	return 0;
}

void CFormTeachLoader::SetCurPageAxis(AXIS_ID axis, TEACH_PARAM::ID param, UINT nID)
{
	m_CurAxis = axis;
	m_CurTeach = param;
	m_CurID	  = nID;	

	GetMainHandler()->SetSelectedAxis(m_CurAxis, m_CurTeach);
}

void CFormTeachLoader::UpdateLoaderState()
{
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LD_SHUTTLE_SHUTTLE1_LOAD,theUnitFunc.Chk_UpperShuttle_LoadPos()? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LD_SHUTTLE_SHUTTLE1_UNLOAD,theUnitFunc.Chk_UpperShuttle_UnLoadPos()? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LD_SHUTTLE_SHUTTLE2_LOAD,theUnitFunc.Chk_LowerShuttle_LoadPos()? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LD_SHUTTLE_SHUTTLE2_UNLOAD,theUnitFunc.Chk_LowerShuttle_UnLoadPos()? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_LD_CONV_CELLSTATE,(theUnitFunc.GetInPutIOCheck(X_LD_CONV_CELL_SEN_1)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LD_ROBOT_VAC_ON,(theUnitFunc.GetInPutIOCheck(X_LD_ROBOT_VAC_SEN)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LD_ROBOT_VAC_OFF,(!theUnitFunc.GetInPutIOCheck(X_LD_ROBOT_VAC_SEN)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LD_SHUTTLE_SHUTTLE1_VAC_CH1_ON,(theUnitFunc.GetInPutIOCheck(X_LD_UPPER_SHUTTLE_CH1_VAC_SEN)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LD_SHUTTLE_SHUTTLE1_VAC_CH1_OFF,(!theUnitFunc.GetInPutIOCheck(X_LD_UPPER_SHUTTLE_CH1_VAC_SEN)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LD_SHUTTLE_SHUTTLE1_VAC_CH2_ON,(theUnitFunc.GetInPutIOCheck(X_LD_UPPER_SHUTTLE_CH2_VAC_SEN)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LD_SHUTTLE_SHUTTLE1_VAC_CH2_OFF,(!theUnitFunc.GetInPutIOCheck(X_LD_UPPER_SHUTTLE_CH2_VAC_SEN)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LD_SHUTTLE_SHUTTLE2_VAC_CH1_ON,(theUnitFunc.GetInPutIOCheck(X_LD_LOWER_SHUTTLE_CH1_VAC_SEN)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LD_SHUTTLE_SHUTTLE2_VAC_CH1_OFF,(!theUnitFunc.GetInPutIOCheck(X_LD_LOWER_SHUTTLE_CH1_VAC_SEN)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LD_SHUTTLE_SHUTTLE2_VAC_CH2_ON,(theUnitFunc.GetInPutIOCheck(X_LD_LOWER_SHUTTLE_CH2_VAC_SEN)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LD_SHUTTLE_SHUTTLE2_VAC_CH2_OFF,(!theUnitFunc.GetInPutIOCheck(X_LD_LOWER_SHUTTLE_CH2_VAC_SEN)? GXCOLOR_ON:GXCOLOR_OFF));
}

void CFormTeachLoader::ClickGxbtnLdConvPitch()
{
	theUnitFunc.LDConvZeroPointSet();
	theUnitFunc.LDConvMove();
	SetTimer(27,10,NULL);
}


void CFormTeachLoader::ClickGxbtnLdRobotConvGetPos()
{
	if(theUnitFunc.DoorInterLockChkStart() == FALSE)
		return ;

	if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_RUNNING_Y_ERRRESET))
	{
		theUnitFunc.LDRobotBitAllOff(E_CELL_LOADING);
		Sleep(200);
		theDeviceMelsec.RobotBitOn(LDROBOT_XY_VACOFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_XY_VACON);
		long write = 4; // GETOK1
		theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
		m_nBSeq = seqB_1;
		KillTimer(3);
		SetTimer(3,300,NULL);
	}
	else
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("로봇 오리진 체크."),
			_T("Robot ORG Check"),		
			_T("Robot gốc Kiểm tra"));

		dlgMsgBox.DoModal();
	}
}


void CFormTeachLoader::ClickGxbtnLdRobotShuttle1Ch1Put()
{
	if(theUnitFunc.DoorInterLockChkStart() == FALSE)
		return ;

	if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_RUNNING_Y_ERRRESET))
	{
		theUnitFunc.LDRobotBitAllOff(E_PUT_1);
		Sleep(200);
		theDeviceMelsec.RobotBitOff(LDROBOT_XY_VACOFF);
		theDeviceMelsec.RobotBitOn(LDROBOT_XY_VACON);
		theDeviceMelsec.RobotBitOn(LDROBOT_XY_UPPERSTGVAC1OFF);
		long write = 32; //PUTOK2
		theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
		nCh = JIG_CH_1;
		m_nCSeq = seqC_1;
		KillTimer(4);
		SetTimer(4,1000,NULL);
	}
	else
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("로봇 오리진 체크."),
			_T("Robot ORG Check"),		
			_T("Robot gốc Kiểm tra"));

		dlgMsgBox.DoModal();
	}
}


void CFormTeachLoader::ClickGxbtnLdRobotShuttle1Ch2Put()
{
	if(theUnitFunc.DoorInterLockChkStart() == FALSE)
		return ;

	if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_RUNNING_Y_ERRRESET))
	{
		theUnitFunc.LDRobotBitAllOff(E_PUT_1);
		Sleep(200);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_VACOFF);
			theDeviceMelsec.RobotBitOn(LDROBOT_XY_VACON);
		theDeviceMelsec.RobotBitOn(LDROBOT_XY_UPPERSTGVAC2OFF);
		long write = 32; //PUTOK2
		theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
		nCh = JIG_CH_2;
		m_nCSeq = seqC_1;
		KillTimer(4);
		SetTimer(4,1000,NULL);
	}
	else
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("로봇 오리진 체크."),
			_T("Robot ORG Check"),		
			_T("Robot gốc Kiểm tra"));

		dlgMsgBox.DoModal();
	}
}


void CFormTeachLoader::ClickGxbtnLdRobotShuttle2Ch1Put()
{
	if(theUnitFunc.DoorInterLockChkStart() == FALSE)
		return ;

	if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_RUNNING_Y_ERRRESET))
	{
		if (theUnitFunc.Chk_UpperShuttle_UnLoadPos())
		{
			theUnitFunc.LDRobotBitAllOff(E_PUT_2);
			Sleep(200);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_VACOFF);
			theDeviceMelsec.RobotBitOn(LDROBOT_XY_VACON);
			theDeviceMelsec.RobotBitOn(LDROBOT_XY_LOWERSTGVAC1OFF);
			long write = 128; //PUTOK3
			theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
			nCh = JIG_CH_1;
			m_nDSeq = seqD_1;
			KillTimer(5);
			SetTimer(5,1000,NULL);
		}
		else
		{
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("UPPER SHUTTLE이 UNLOAD 위치가 아닙니다."),
				_T("Please, UPPER SHUTTLE Y Pos Check"),		
				_T("Lên trên Shuttle không phải là vị trí đặt."));

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
			_T("Robot gốc Kiểm tra"));

		dlgMsgBox.DoModal();
	}
}

void CFormTeachLoader::ClickGxbtnLdRobotShuttle2Put()
{
	if(theUnitFunc.DoorInterLockChkStart() == FALSE)
		return ;

	if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_RUNNING_Y_ERRRESET))
	{
		if (theUnitFunc.Chk_UpperShuttle_UnLoadPos())
		{
			theUnitFunc.LDRobotBitAllOff(E_PUT_2);
			Sleep(200);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_VACOFF);
			theDeviceMelsec.RobotBitOn(LDROBOT_XY_VACON);
			theDeviceMelsec.RobotBitOn(LDROBOT_XY_LOWERSTGVAC2OFF);
			long write = 128; //PUTOK3
			theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
			nCh = JIG_CH_2;
			m_nDSeq = seqD_1;
			KillTimer(5);
			SetTimer(5,1000,NULL);
		}
		else
		{
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("UPPER SHUTTLE이 UNLOAD 위치가 아닙니다."),
				_T("Please, UPPER SHUTTLE Y Pos Check"),		
				_T("Lên trên Shuttle không phải là vị trí đặt."));

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
			_T("Robot gốc Kiểm tra"));

		dlgMsgBox.DoModal();
	}
}


void CFormTeachLoader::ClickGxbtnLdRobotVacOn()
{
	if(theUnitFunc.DoorInterLockChkStart() == FALSE)
		return ;

	theUnitFunc.RobotCellVacOn(VAC_ON,E_LD_ROBOT);
	theDeviceMelsec.RobotBitOn(LDROBOT_XY_VACON);
	theDeviceMelsec.RobotBitOff(LDROBOT_XY_VACOFF);
}


void CFormTeachLoader::ClickGxbtnLdRobotVacOff()
{
	if(theUnitFunc.DoorInterLockChkStart() == FALSE)
		return ;

	theUnitFunc.RobotCellVacOn(VAC_OFF,E_LD_ROBOT);
	Sleep(200);
	theUnitFunc.RobotBlowOff(E_LD_ROBOT);
	theDeviceMelsec.RobotBitOff(LDROBOT_XY_VACON);
	theDeviceMelsec.RobotBitOn(LDROBOT_XY_VACOFF);
}


void CFormTeachLoader::ClickGxbtnLdShuttleShuttle1Load()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_LD_UPPERSHUTTLE_Y;
	m_CurTeach = TEACH_PARAM::LD_UPPERSHUTTLE_LOAD;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_LD_SHUTTLE_SHUTTLE1_LOAD);
}


void CFormTeachLoader::ClickGxbtnLdShuttleShuttle1Unload()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_LD_UPPERSHUTTLE_Y;
	m_CurTeach = TEACH_PARAM::LD_UPPERSHUTTLE_UNLOAD;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_LD_SHUTTLE_SHUTTLE1_UNLOAD);
}


void CFormTeachLoader::ClickGxbtnLdShuttleShuttle1VacCh1On()
{
	theUnitFunc.UpperShuttle_VacuumOn(VAC_ON,JIG_CH_1);
}


void CFormTeachLoader::ClickGxbtnLdShuttleShuttle1VacCh1Off()
{
	theUnitFunc.UpperShuttle_VacuumOn(VAC_OFF,JIG_CH_1);
	Sleep(200);
	theUnitFunc.UpperShuttle_BlowOff(JIG_CH_1);
}


void CFormTeachLoader::ClickGxbtnLdShuttleShuttle1VacCh2On()
{
	theUnitFunc.UpperShuttle_VacuumOn(VAC_ON,JIG_CH_2);
}


void CFormTeachLoader::ClickGxbtnLdShuttleShuttle1VacCh2Off()
{
	theUnitFunc.UpperShuttle_VacuumOn(VAC_OFF,JIG_CH_2);
	Sleep(200);
	theUnitFunc.UpperShuttle_BlowOff(JIG_CH_2);
}


void CFormTeachLoader::ClickGxbtnLdShuttleShuttle2Load()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_LD_LOWERSHUTTLE_Y;
	m_CurTeach = TEACH_PARAM::LD_LOWERSHUTTLE_LOAD;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_LD_SHUTTLE_SHUTTLE2_LOAD);
}


void CFormTeachLoader::ClickGxbtnLdShuttleShuttle2Unload()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_LD_LOWERSHUTTLE_Y;
	m_CurTeach = TEACH_PARAM::LD_LOWERSHUTTLE_UNLOAD;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_LD_SHUTTLE_SHUTTLE2_UNLOAD);
}


void CFormTeachLoader::ClickGxbtnLdShuttleShuttle2VacCh1On()
{

	theUnitFunc.LowerShuttle_VacuumOn(VAC_ON,JIG_CH_1);
}


void CFormTeachLoader::ClickGxbtnLdShuttleShuttle2VacCh1Off()
{
	theUnitFunc.LowerShuttle_VacuumOn(VAC_OFF,JIG_CH_1);
	Sleep(200);
	theUnitFunc.LowerShuttle_BlowOff(JIG_CH_1);
}


void CFormTeachLoader::ClickGxbtnLdShuttleShuttle2VacCh2On()
{
	theUnitFunc.LowerShuttle_VacuumOn(VAC_ON,JIG_CH_2);
}

void CFormTeachLoader::ClickGxbtnLdShuttleShuttle2VacCh2Off()
{
	theUnitFunc.LowerShuttle_VacuumOn(VAC_OFF,JIG_CH_2);
	Sleep(200);
	theUnitFunc.LowerShuttle_BlowOff(JIG_CH_2);
}

void CFormTeachLoader::ClickGxbtnLdRobotOrg()
{

	if(theUnitFunc.DoorInterLockChkStart() == FALSE)
		return ;

	theUnitFunc.LDRobotBitAllOff(E_ORG);
	m_nASeq = seqA_1;
	KillTimer(2);
	SetTimer(2,200,NULL);
}

void CFormTeachLoader::ClickGxbtnEdgeAlignStart()
{
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_EDGE_ALIGN_OK, GXCOLOR_OFF);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_EDGE_ALIGN_OK, _T(""));

	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_ALIGN_XDATA, GXCOLOR_OFF);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_ALIGN_XDATA, _T(""));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_ALIGN_YDATA, GXCOLOR_OFF);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_ALIGN_YDATA, _T(""));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_ALIGN_TDATA, GXCOLOR_OFF);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_ALIGN_TDATA, _T(""));

	theProcBank.m_strLA_AlignOK = _T("");

	theSocketInterFace.SendMsgToMITEdgeAlign(LA_ALIGN_REQ, _T("MANUAL"), _T(""));

	SetTimer(25,2000,NULL);
}

void CFormTeachLoader::ClickGxstcEdgeAlignTeaching()
{
	CGxMsgBox	dlgMsgBox;
	//2018-03-03,SKCHO,PASSWORD 다이얼로그 추가
	//CDlgBoxKeyBoard dlgKey;
	CDlgBoxPassWord dlgKey;

	dlgMsgBox.SetTitle(_T("TEACHING 적용"), _T("TEACHING APPLY"), _T("TEACHING APPLY"));
	dlgMsgBox.SetMessage(TRUE, 
					_T("티칭 적용 하시겠습니까?"), 
					_T("Do you want to Applying Teaching ?"),		
					_T("Bạn có muốn áp dụng giảng dạy?") );

	int nRet = dlgMsgBox.DoModal();

	if(nRet == IDOK)
	{
		//얼라인 티칭시 확인 후 비밀번호 한번더 확인 2017/06/16 HSK.
		if ( dlgKey.DoModal() == IDOK )
		{
			CString szInput = dlgKey.m_strReturnText;
			if(szInput != _T("4871"))
			{
				dlgMsgBox.SetTitle(_T("TEACHING 적용"), _T("TEACHING APPLY"), _T("TEACHING APPLY"));
				dlgMsgBox.SetMessage(TRUE, 
				_T("비밀번호가 틀렸습니다."), 
				_T("Wrong Password"),		
				_T("Wrong Password") );
				dlgMsgBox.DoModal();
				return;
			}
		}
		//2017-06-18 JSJUNG. 비밀번호 다이얼로그에서 cancle 시 티칭값 저장되는 부분 막음
		else
			return;
	}
	else
		return;

	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_EDGE_ALIGN_TEACHING_OK, GXCOLOR_OFF);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_EDGE_ALIGN_TEACHING_OK, _T(""));

	theProcBank.m_strLA_TeachingOK = _T("");
	theSocketInterFace.SendMsgToMITEdgeAlign(LA_ALIGN_TEACHING, _T(""), _T(""));

	SetTimer(26,2000,NULL);
}

void CFormTeachLoader::LDRobotOrg()
{
	int write = 0;
	switch(m_nASeq)
	{
	case seqA_idle:
		break;
	case seqA_1:
		if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_RUNNING_Y_ERRRESET))
		{
			theDeviceMelsec.RobotBitOff(LDROBOT_X_PLAYMODE_Y_EXTSTOP);
				
			//20170205 byskcho
			//알람이 발생하여 로봇이 언로딩위치 있을때 로봇을 원점기동하면 로봇이 다시 언로딩위치로 가는 버그 수정
			write = 0;
			theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);

		}
		m_nASeq++;
		m_Timer.Start();
		break;
	case seqA_2:
		theDeviceMelsec.RobotBitOn(LDROBOT_X_PGMEND_Y_PAUSE);
		m_nASeq++;
		m_Timer.Start();
		break;
	case seqA_3:
		if (m_Timer.Stop(FALSE) > 0.2)
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_X_PLAYMODE_Y_EXTSTOP);
			m_nASeq++;
			m_Timer.Start();
		}
		break;
	case seqA_4:
		if (m_Timer.Stop(FALSE) > 0.2)
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_X_STOPPING_Y_EXTMOTORON);
			m_nASeq++;
			m_Timer.Start();
		}
		break;
	case seqA_5:
		if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MOTORON_Y_LOWSPEED))
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_X_ALARM_Y_EXTRESET);
			m_nASeq++;
			m_Timer.Start();
		}
		if (m_Timer.Stop(FALSE) > 3)
		{
			theUnitFunc.LDRobotBitAllOff(E_ORG);
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
		if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_EXTRESET_Y_PGMSEL2))
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_X_PGMACK_Y_PGMSEL1);
			m_nASeq++;
			m_Timer.Start();
		}
		else if (m_Timer.Stop(FALSE) > 1)
		{
			theUnitFunc.LDRobotBitAllOff(E_ORG);
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
			theDeviceMelsec.RobotBitOn(LDROBOT_X_HOMEPOS2_Y_PGMSTROBE);
			m_nASeq++;
			m_Timer.Start();
		}
		break;
	case seqA_8:
		if (m_Timer.Stop(FALSE) > 0.2)
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_X_TEACHMODE_Y_EXTSTART);
			m_nASeq++;
			m_Timer.Start();
		}
		break;
	case seqA_9:
		if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_RUNNING_Y_ERRRESET))
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_X_BATWARN_Y_ORG);
			m_nASeq++;
			m_Timer.Start();
		}
		else if (m_Timer.Stop(FALSE) > 10.)
		{	
			theUnitFunc.LDRobotBitAllOff(E_ORG);
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
		if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_HOMEPOS1_Y_CYCLESTOP))
		{
			theDeviceMelsec.RobotBitOff(LDROBOT_X_BATWARN_Y_ORG);
			
			m_nASeq = seqA_idle;
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
	


void CFormTeachLoader::LDRobotGet()
{
	switch(m_nBSeq)
	{
	case seqB_idle:
		break;
	case seqB_1:
		m_nBSeq++;
		m_Timer.Start();
		break;
	case seqB_2:
		if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_VICOMP))
		{
			m_nBSeq++;
			m_Timer.Start();
		}
		else if(m_Timer.Stop(FALSE) > 10.)
		{	
			theUnitFunc.LDRobotBitAllOff(E_CELL_LOADING);
			m_nBSeq = seqB_idle;
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("비젼 데이타 요청이 ON되지 않았습니다."),
				_T("Vision data Request Not On"),		
				_T("Vision data Request Not On"));

			dlgMsgBox.DoModal();
			break;
		}
		break;
	case seqB_3:
		if(m_Timer.Stop(FALSE) > 0.5)
		{
			theUnitFunc.LDRobotSendAlignData(_tstof(theProcBank.m_strLAResX),_tstof(theProcBank.m_strLAResY),_tstof(theProcBank.m_strLAResT));
			theDeviceMelsec.RobotBitOn(LDROBOT_XY_VICOMP);
			theDeviceMelsec.RobotBitOn(LDROBOT_XY_CVGET);
			m_nBSeq++;
			m_Timer.Start();
		}
		break;
	case seqB_4:
		if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_CVGET) == FALSE)
		{
			m_nBSeq++;
			m_Timer.Start();
		}
		else if(m_Timer.Stop(FALSE) > 10.)
		{	
			theUnitFunc.LDRobotBitAllOff(E_CELL_LOADING);
			m_nBSeq = seqB_idle;
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("컨베어 인터락이 OFF 되지 않았음."),
				_T("Conv Get Interlock off Not On"),		
				_T("Conv Get Interlock off Not On"));

			dlgMsgBox.DoModal();
			break;
		}
		break;
	case seqB_5:
		if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_VACON))
		{
			theUnitFunc.RobotCellVacOn(VAC_ON,E_LD_ROBOT);
			m_nBSeq++;
			m_Timer.Start();
		}
		else if(m_Timer.Stop(FALSE) > 3.)
		{	
			theUnitFunc.LDRobotBitAllOff(E_CELL_LOADING);
			m_nBSeq = seqB_idle;
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("진공 흡착이 ON되지 않았습니다."),
				_T("Vac On Not On"),		
				_T("Vac On Not On"));

			dlgMsgBox.DoModal();
			break;
		}
		break;
	case seqB_6:
		if (theUnitFunc.RobotCellVacOnChk(E_LD_ROBOT,VAC_ON))
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_XY_VACON);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_VACOFF);
			m_nBSeq++;
			m_Timer.Start();
		}
		else if(m_Timer.Stop(FALSE) > 3.)
		{	
			theUnitFunc.LDRobotBitAllOff(E_CELL_LOADING);
			m_nBSeq = seqB_idle;
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("진공 흡착이 되지 않았습니다."),
				_T("VAC Not On"),		
				_T("VAC Not On"));

			dlgMsgBox.DoModal();
			break;
		}
		break;
	case seqB_7:
		if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_CVGETCOMP))
		{
			m_nBSeq++;
			m_Timer.Start();
		}
		else if(m_Timer.Stop(FALSE) > 3.)
		{	
			theUnitFunc.LDRobotBitAllOff(E_CELL_LOADING);
			m_nBSeq = seqB_idle;
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("컨베어 로딩 컴플릿이 ON되지 않았습니다."),
				_T("Conv Get Comp Not On"),		
				_T("Conv Get Comp Not On"));

			dlgMsgBox.DoModal();
			break;
		}
		break;
	case seqB_8:
		if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_CVGET))
		{
			theUnitFunc.LDRobotBitAllOff(E_CELL_LOADING);
			m_nBSeq = seqB_idle;
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
		else if(m_Timer.Stop(FALSE) > 10.)
		{	
			theUnitFunc.LDRobotBitAllOff(E_CELL_LOADING);
			m_nBSeq = seqB_idle;
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("컨베어 인터락이 ON되지 않았습니다."),
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


void CFormTeachLoader::LDRobotUpperPut()
{
	switch(m_nCSeq)
	{
	case seqC_idle:
		break;
	case seqC_1:
		m_nCSeq++;
		m_Timer.Start();
		break;
	case seqC_2:
		if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_BUSY_Y_PGMSEL6) == FALSE)
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_XY_UPPERSTGPUT);
			m_nCSeq++;
			m_Timer.Start();
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{	
			theUnitFunc.LDRobotBitAllOff(E_PUT_1);
			m_nCSeq = seqC_idle;
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("로봇 정지가 OFF되지 않았습니다."),
				_T("Robot Busy Not OFF"),		
				_T("Robot Busy Not OFF"));

			dlgMsgBox.DoModal();
			break;
		}
		break;
	case seqC_3:
		if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_BUSY_Y_PGMSEL6) == FALSE)
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_XY_UPPERSTGPUT);
			m_nCSeq++;
			m_Timer.Start();
		}
		else if(m_Timer.Stop(FALSE) > 10.)
		{	
			theUnitFunc.LDRobotBitAllOff(E_PUT_1);
			m_nCSeq = seqC_idle;
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("로봇 정지가 OFF되지 않았습니다."),
				_T("Robot Busy Not OFF"),		
				_T("Robot Busy Not OFF"));

			dlgMsgBox.DoModal();
			break;

		}
		break;
	case seqC_4:
		if (nCh == JIG_CH_1)
		{
			if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_UPPERSTGPUT) == FALSE)
			{
				m_nCSeq++;
				m_Timer.Start();
			}
			else if(m_Timer.Stop(FALSE) > 10.)
			{	
				theUnitFunc.LDRobotBitAllOff(E_PUT_1);
				m_nCSeq = seqC_idle;
				CGxMsgBox	dlgMsgBox;

				dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
				dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
				dlgMsgBox.SetMessage(FALSE, 
					_T("Put Permit이 OFF되지 않았음"),
					_T("Put Permit Not OFF"),		
					_T("Put Permit Not OFF"));

				dlgMsgBox.DoModal();
				break;
			}
		}
		else
		{
			if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_UPPERSTGPUT) == FALSE)
			{
				m_nCSeq++;
				m_Timer.Start();
			}
			else if(m_Timer.Stop(FALSE) > 10.)
			{	
				theUnitFunc.LDRobotBitAllOff(E_PUT_1);
				m_nCSeq = seqC_idle;
				CGxMsgBox	dlgMsgBox;

				dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
				dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
				dlgMsgBox.SetMessage(FALSE, 
					_T("Put Permit이 OFF되지 않았음"),
					_T("Put Permit Not OFF"),		
					_T("Put Permit Not OFF"));

				dlgMsgBox.DoModal();
				break;
			}
		}
		break;
	case seqC_5:
		if (nCh == JIG_CH_1)
		{
			if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_UPPERSTGVAC1ON))
			{
				theDeviceMelsec.RobotBitOn(LDROBOT_XY_UPPERSTGVAC1ON);
				theDeviceMelsec.RobotBitOff(LDROBOT_XY_UPPERSTGVAC1OFF);
				theUnitFunc.UpperShuttle_VacuumOn(VAC_ON,JIG_CH_1);
				m_nCSeq++;
				m_Timer.Start();
			}
			else if(m_Timer.Stop(FALSE) > 10.)
			{	
				theUnitFunc.LDRobotBitAllOff(E_PUT_1);
				m_nCSeq = seqC_idle;
				CGxMsgBox	dlgMsgBox;

				dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
				dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
				dlgMsgBox.SetMessage(FALSE, 
					_T("VAC ON이 ON 되지 않았습니다."),
					_T("VAC ON Not ON"),		
					_T("VAC ON Not ON"));

				dlgMsgBox.DoModal();
				break;
			}
		}
		else
		{
			if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_UPPERSTGVAC2ON))
			{
				theDeviceMelsec.RobotBitOn(LDROBOT_XY_UPPERSTGVAC2ON);
				theDeviceMelsec.RobotBitOff(LDROBOT_XY_UPPERSTGVAC2OFF);
				theUnitFunc.UpperShuttle_VacuumOn(VAC_ON,JIG_CH_2);
				m_nCSeq++;
				m_Timer.Start();
			}
			else if(m_Timer.Stop(FALSE) > 10.)
			{	
				theUnitFunc.LDRobotBitAllOff(E_PUT_1);
				m_nCSeq = seqC_idle;
				CGxMsgBox	dlgMsgBox;

				dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
				dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
				dlgMsgBox.SetMessage(FALSE, 
					_T("VAC ON이 ON 되지 않았습니다."),
					_T("VAC ON Not ON"),		
					_T("VAC ON Not ON"));

				dlgMsgBox.DoModal();
				break;
			}
		}
		break;
	case seqC_6:
		if (nCh == JIG_CH_1)
		{
			if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_VACOFF) && theUnitFunc.UpperShuttle_VacuumChk(JIG_CH_1,VAC_ON))
			{
				theUnitFunc.RobotCellVacOn(VAC_OFF,E_LD_ROBOT);
				m_nCSeq++;
				m_Timer.Start();
			}
			else if(m_Timer.Stop(FALSE) > 10.)
			{	
				theUnitFunc.LDRobotBitAllOff(E_PUT_1);
				m_nCSeq = seqC_idle;
				CGxMsgBox	dlgMsgBox;

				dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
				dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
				dlgMsgBox.SetMessage(FALSE, 
					_T("VAC OFF가 ON 되지 않았습니다."),
					_T("VAC OFF Not ON"),		
					_T("VAC OFF Not ON"));

				dlgMsgBox.DoModal();
				break;
			}
		}
		else
		{
			if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_VACOFF) && theUnitFunc.UpperShuttle_VacuumChk(JIG_CH_2,VAC_ON))
			{
				theUnitFunc.RobotCellVacOn(VAC_OFF,E_LD_ROBOT);
				m_nCSeq++;
				m_Timer.Start();
			}
			else if(m_Timer.Stop(FALSE) > 10.)
			{	
				theUnitFunc.LDRobotBitAllOff(E_PUT_1);
				m_nCSeq = seqC_idle;
				CGxMsgBox	dlgMsgBox;

				dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
				dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
				dlgMsgBox.SetMessage(FALSE, 
					_T("VAC OFF가 ON 되지 않았습니다."),
					_T("VAC OFF Not ON"),		
					_T("VAC OFF Not ON"));

				dlgMsgBox.DoModal();
				break;
			}
		}
		break;
	case seqC_7:
		if (theUnitFunc.RobotCellVacOnChk(E_LD_ROBOT,VAC_OFF))
		{
			theUnitFunc.RobotBlowOff(E_LD_ROBOT);
			theDeviceMelsec.RobotBitOn(LDROBOT_XY_VACOFF);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_VACON);
			m_nCSeq++;
			m_Timer.Start();
		}
		else if(m_Timer.Stop(FALSE) > 10.)
		{	
			theUnitFunc.LDRobotBitAllOff(E_PUT_1);
			m_nCSeq = seqC_idle;
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("LD ROBOT이 VAC ON되지 않았습니다."),
				_T("VAC Not ON"),		
				_T("VAC Not ON"));

			dlgMsgBox.DoModal();
			break;
		}
		break;
	case seqC_8:
		if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_UPPERSTGPUTCOMP))
		{
			m_nCSeq++;
			m_Timer.Start();
		}
		else if(m_Timer.Stop(FALSE) > 10.)
		{	
			theUnitFunc.LDRobotBitAllOff(E_PUT_1);
			m_nCSeq = seqC_idle;
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("스테이지 언로딩 완료 되지 않았습니다."),
				_T("Stage Put Comp Not ON"),		
				_T("Stage Put Comp Not ON"));

			dlgMsgBox.DoModal();
			break;
		}
		break;
	case seqC_9:
		if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_UPPERSTGPUT))
		{
			theUnitFunc.LDRobotBitAllOff(E_PUT_1);
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
		else if(m_Timer.Stop(FALSE) > 10.)
		{	
			theUnitFunc.LDRobotBitAllOff(E_PUT_1);
			m_nCSeq = seqC_idle;
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("언로딩 인터락이 OFF 되지 않았습니다."),
				_T("Put Interlock Not OFF"),		
				_T("Put Interlock Not OFF"));

			dlgMsgBox.DoModal();
			break;
		}
		break;
	default:
		break;
	}
}


void CFormTeachLoader::LDRobotLowerPut()
{
	switch(m_nDSeq)
	{
	case seqD_idle:
		break;
	case seqD_1:
		m_nDSeq++;
		m_Timer.Start();
		break;
	case seqD_2:
		if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_BUSY_Y_PGMSEL6) == FALSE)
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_XY_LOWERSTGPUT);
			m_nDSeq++;
			m_Timer.Start();
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{	
			theUnitFunc.LDRobotBitAllOff(E_PUT_2);
			m_nDSeq = seqD_idle;
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("로봇 정지가 OFF되지 않았습니다."),
				_T("Robot Busy Not OFF"),		
				_T("Robot Busy Not OFF"));

			dlgMsgBox.DoModal();
			break;
		}
		break;
	case seqD_3:
		if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_BUSY_Y_PGMSEL6) == FALSE)
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_XY_LOWERSTGPUT);
			m_nDSeq++;
			m_Timer.Start();
		}
		else if(m_Timer.Stop(FALSE) > 10.)
		{	
			theUnitFunc.LDRobotBitAllOff(E_PUT_2);
			m_nDSeq = seqD_idle;
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("로봇 정지가 OFF되지 않았습니다."),
				_T("Robot Busy Not OFF"),		
				_T("Robot Busy Not OFF"));

			dlgMsgBox.DoModal();
			break;
		}
		break;
	case seqD_4:
		if (nCh == JIG_CH_1)
		{
			if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_LOWERSTGPUT) == FALSE)
			{
				m_nDSeq++;
				m_Timer.Start();
			}
			else if(m_Timer.Stop(FALSE) > 10.)
			{	
				theUnitFunc.LDRobotBitAllOff(E_PUT_2);
				m_nDSeq = seqD_idle;
				CGxMsgBox	dlgMsgBox;

				dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
				dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
				dlgMsgBox.SetMessage(FALSE, 
					_T("Put Permit이 OFF되지 않았음"),
					_T("Put Permit Not OFF"),		
					_T("Put Permit Not OFF"));

				dlgMsgBox.DoModal();
				break;
			}
		}
		else
		{
			if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_LOWERSTGPUT) == FALSE)
			{
				m_nDSeq++;
				m_Timer.Start();
			}
			else if(m_Timer.Stop(FALSE) > 10.)
			{	
				theUnitFunc.LDRobotBitAllOff(E_PUT_2);
				m_nDSeq = seqD_idle;
				CGxMsgBox	dlgMsgBox;

				dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
				dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
				dlgMsgBox.SetMessage(FALSE, 
					_T("Put Permit이 OFF되지 않았음"),
					_T("Put Permit Not OFF"),		
					_T("Put Permit Not OFF"));

				dlgMsgBox.DoModal();
				break;
			}
		}
		break;
	case seqD_5:
		if (nCh == JIG_CH_1)
		{
			if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_LOWERSTGVAC1ON))
			{
				theDeviceMelsec.RobotBitOn(LDROBOT_XY_LOWERSTGVAC1ON);
				theUnitFunc.LowerShuttle_VacuumOn(VAC_ON,JIG_CH_1);
				m_nDSeq++;
				m_Timer.Start();
			}
			else if(m_Timer.Stop(FALSE) > 10.)
			{	
				theUnitFunc.LDRobotBitAllOff(E_PUT_2);
				m_nDSeq = seqD_idle;
				CGxMsgBox	dlgMsgBox;

				dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
				dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
				dlgMsgBox.SetMessage(FALSE, 
					_T("VAC ON이 ON 되지 않았습니다."),
					_T("VAC ON Not ON"),		
					_T("VAC ON Not ON"));

				dlgMsgBox.DoModal();
				break;
			}
		}
		else
		{
			if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_LOWERSTGVAC2ON))
			{
				theDeviceMelsec.RobotBitOn(LDROBOT_XY_LOWERSTGVAC2ON);
				theUnitFunc.LowerShuttle_VacuumOn(VAC_ON,JIG_CH_2);
				m_nDSeq++;
				m_Timer.Start();
			}
			else if(m_Timer.Stop(FALSE) > 10.)
			{	
				theUnitFunc.LDRobotBitAllOff(E_PUT_2);
				m_nDSeq = seqD_idle;
				CGxMsgBox	dlgMsgBox;

				dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
				dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
				dlgMsgBox.SetMessage(FALSE, 
					_T("VAC ON이 ON 되지 않았습니다."),
					_T("VAC ON Not ON"),		
					_T("VAC ON Not ON"));

				dlgMsgBox.DoModal();
				break;
			}
		}
		break;
	case seqD_6:
		if (nCh == JIG_CH_1)
		{
			if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_VACOFF) && theUnitFunc.LowerShuttle_VacuumChk(JIG_CH_1,VAC_ON))
			{
				theUnitFunc.RobotCellVacOn(VAC_OFF,E_LD_ROBOT);
				m_nDSeq++;
				m_Timer.Start();
			}
			else if(m_Timer.Stop(FALSE) > 10.)
			{	
				theUnitFunc.LDRobotBitAllOff(E_PUT_2);
				m_nDSeq = seqD_idle;
				CGxMsgBox	dlgMsgBox;

				dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
				dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
				dlgMsgBox.SetMessage(FALSE, 
					_T("VAC OFF가 ON 되지 않았습니다."),
					_T("VAC OFF Not ON"),		
					_T("VAC OFF Not ON"));

				dlgMsgBox.DoModal();
				break;
			}
		}
		else
		{
			if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_VACOFF) && theUnitFunc.LowerShuttle_VacuumChk(JIG_CH_2,VAC_ON))
			{
				theUnitFunc.RobotCellVacOn(VAC_OFF,E_LD_ROBOT);
				m_nDSeq++;
				m_Timer.Start();
			}
			else if(m_Timer.Stop(FALSE) > 10.)
			{	
				theUnitFunc.LDRobotBitAllOff(E_PUT_2);
				m_nDSeq = seqD_idle;
				CGxMsgBox	dlgMsgBox;

				dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
				dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
				dlgMsgBox.SetMessage(FALSE, 
					_T("VAC OFF가 ON 되지 않았습니다."),
					_T("VAC OFF Not ON"),		
					_T("VAC OFF Not ON"));

				dlgMsgBox.DoModal();
				break;
			}
		}
		break;
	case seqD_7:
		if (theUnitFunc.RobotCellVacOnChk(E_LD_ROBOT,VAC_OFF))
		{
			theUnitFunc.RobotBlowOff(E_LD_ROBOT);
			theDeviceMelsec.RobotBitOn(LDROBOT_XY_VACOFF);
			m_nDSeq++;
			m_Timer.Start();
		}
		else if(m_Timer.Stop(FALSE) > 10.)
		{	
			theUnitFunc.LDRobotBitAllOff(E_PUT_2);
			m_nDSeq = seqD_idle;
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("LD ROBOT이 VAC ON되지 않았습니다."),
				_T("VAC Not ON"),		
				_T("VAC Not ON"));

			dlgMsgBox.DoModal();
			break;
		}
		break;
	case seqD_8:
		if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_LOWERSTGPUTCOMP))
		{
			m_nDSeq++;
			m_Timer.Start();
		}
		else if(m_Timer.Stop(FALSE) > 10.)
		{	
			theUnitFunc.LDRobotBitAllOff(E_PUT_2);
			m_nDSeq = seqD_idle;
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("스테이지 언로딩 완료 되지 않았습니다"),
				_T("Stage Put Comp Not ON"),		
				_T("Stage Put Comp Not ON"));

			dlgMsgBox.DoModal();
			break;
		}
		break;
	case seqD_9:
		if ((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_LOWERSTGPUT))
		{
			theUnitFunc.LDRobotBitAllOff(E_PUT_2);
			m_nDSeq = seqD_idle;
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
		else if(m_Timer.Stop(FALSE) > 10.)
		{	
			theUnitFunc.LDRobotBitAllOff(E_PUT_2);
			m_nDSeq = seqD_idle;
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("언로딩 인터락이 OFF 되지 않았습니다."),
				_T("Interlock Not OFF"),		
				_T("Interlock Not OFF"));

			dlgMsgBox.DoModal();
			break;
		}
		break;
	default:
		break;
	}
}