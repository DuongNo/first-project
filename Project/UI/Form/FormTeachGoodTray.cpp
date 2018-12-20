// FormTeachGoodTray.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "resource.h"
#include "CheetahApp.h"

#include "FormTeachGoodTray.h"
#include "GUIDefine.h"

//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "UI\GausGUI\GxButtonEx.h"
#include "UI\GausGUI\GxStaticEx.h"
// CFormTeachGoodTray

IMPLEMENT_DYNCREATE(CFormTeachGoodTray, CFormView)

CFormTeachGoodTray::CFormTeachGoodTray()
	: CFormView(CFormTeachGoodTray::IDD)
{
	m_bInit = FALSE;
}

CFormTeachGoodTray::~CFormTeachGoodTray()
{
}

void CFormTeachGoodTray::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFormTeachGoodTray, CFormView)
	ON_WM_TIMER()
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)			// 표시 언어가 바뀌었다.
END_MESSAGE_MAP()


// CFormTeachGoodTray 진단입니다.

#ifdef _DEBUG
void CFormTeachGoodTray::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormTeachGoodTray::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormTeachGoodTray 메시지 처리기입니다.

BOOL CFormTeachGoodTray::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style = WS_OVERLAPPED | WS_CHILD;				// 테두리를 없앤다.

	return CFormView::PreCreateWindow(cs);
}

BEGIN_EVENTSINK_MAP(CFormTeachGoodTray, CFormView)

	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYIN_CONV_IN, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTrayinConvIn, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYIN_CONV_OUT, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTrayinConvOut, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYIN_CONV_STOP, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTrayinConvStop, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYIN_CONV_CYLUP, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTrayinConvCylup, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYIN_CONV_CYLDOWN, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTrayinConvCyldown, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYIN_LOAD_POS, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTrayinLoadPos, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYIN_UP_MOVE, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTrayinUpMove, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYIN_TOP_MOVE, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTrayinTopMove, VTS_NONE)//2018wjp
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYIN_PITCHUP_MOVE, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTrayinPitchupMove, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYIN_PITCHDOWN_MOVE, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTrayinPitchdownMove, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYIN_IO_TOP_FWD, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTrayinIoTopFwd, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYIN_IO_TOP_BWD, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTrayinIoTopBwd, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYIN_IO_BTM_FWD, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTrayinIoBtmFwd, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYIN_IO_BTM_BWD, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTrayinIoBtmBwd, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYOUT_CONV_IN, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTrayoutConvIn, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYOUT_CONV_OUT, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTrayoutConvOut, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYOUT_CONV_STOP, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTrayoutConvStop, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYOUT_CONV_CYLUP, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTrayoutConvCylup, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYOUT_CONV_CYLDOWN, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTrayoutConvCyldown, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYOUT_UNLOAD_POS, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTrayoutUnloadPos, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYOUT_UP_MOVE, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTrayoutUpMove, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYOUT_PITCHUP_MOVE, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTrayoutPitchupMove, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYOUT_PITCHDOWN_MOVE, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTrayoutPitchdownMove, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYTR_VACON, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTraytrVacon, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYTR_VACOFF, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTraytrVacoff, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYTR_GETPOS, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTraytrGetpos, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYTR_PUTPOS, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTraytrPutpos, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYTR_UPPOS, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTraytrUppos, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYTR_GETDOWNPOS, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTraytrGetDownpos, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYTR_PUTDOWNPOS, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTraytrPutDownpos, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYIN_IO_ALIGNGUIDE_FWD, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTrayinIoAlignguideFwd, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYIN_IO_ALIGNGUIDE_BWD, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTrayinIoAlignguideBwd, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYOUT_IO_ALIGNGUIDE_FWD, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTrayoutIoAlignguideFwd, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYOUT_IO_ALIGNGUIDE_BWD, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTrayoutIoAlignguideBwd, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYIN_BUFFERCONV_IN, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTrayinBufferconvIn, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYIN_BUFFERCONV_OUT, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTrayinBufferconvOut, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYIN_BUFFERCONV_STOP, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTrayinBufferconvStop, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYOUT_BUFFERCONV_IN, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTrayoutBufferconvIn, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYOUT_BUFFERCONV_OUT, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTrayoutBufferconvOut, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYOUT_BUFFERCONV_STOP, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTrayoutBufferconvStop, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYIN_CONV_CYLUP2, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTrayinConvCylup2, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYIN_CONV_CYLDN2, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTrayinConvCyldn2, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYOUT_CONV_CYLUP2, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTrayoutConvCylup2, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYOUT_CONV_CYLDOWN2, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTrayoutConvCyldown2, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_OKTRAY_ALIGN_START, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnOktrayAlignStart, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_OKTRAY_ALIGN_TEACHING, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnOktrayAlignTeaching, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYID_READING, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTrayIDReading, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYID_RECONNECT, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTrayidReconnect, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYIN_ALIGN_POS, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTrayinAlignPos, VTS_NONE)
	ON_EVENT(CFormTeachGoodTray, IDC_GXBTN_TRAYOUT_ALIGN_POS, DISPID_CLICK, CFormTeachGoodTray::ClickGxbtnTrayoutAlignPos, VTS_NONE)
END_EVENTSINK_MAP()


LRESULT CFormTeachGoodTray::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{
		UpdateOKTrayINState();
		SetTimer(1, 100, NULL);
	}
	else
	{
		KillTimer(1);
	}
	//kjpark 20161025 MCR 구현
	m_bShow = bFlag;
	return 0;
}

void CFormTeachGoodTray::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case 1:
		UpdateOKTrayINState();
		break;
	case 2:
		KillTimer(2);
		if (theProcBank.m_strOKTray_AlignOK == _T("OK"))
		{
			CGxUICtrl::SetStaticColor(this, IDC_GXSTC_OKTRAY_ALIGN_OK, GXCOLOR_ON);
			CGxUICtrl::SetStaticString(this, IDC_GXSTC_OKTRAY_ALIGN_OK, _T("ALIGN OK"));
		}
		else
		{
			CGxUICtrl::SetStaticColor(this, IDC_GXSTC_OKTRAY_ALIGN_OK, GXCOLOR_RED);
			CGxUICtrl::SetStaticString(this, IDC_GXSTC_OKTRAY_ALIGN_OK, _T("ALIGN NG"));
		}
		break;
	case 3:
		KillTimer(3);
		if (theProcBank.m_strOKTray_TeachingOK == _T("OK"))
		{
			CGxUICtrl::SetStaticColor(this, IDC_GXSTC_OKTRAY_TEACHING_OK, GXCOLOR_ON);
			CGxUICtrl::SetStaticString(this, IDC_GXSTC_OKTRAY_TEACHING_OK, _T("TEACHING OK"));
		}
		else
		{
			CGxUICtrl::SetStaticColor(this, IDC_GXSTC_OKTRAY_TEACHING_OK, GXCOLOR_RED);
			CGxUICtrl::SetStaticString(this, IDC_GXSTC_OKTRAY_TEACHING_OK, _T("TEACHING NG"));
		}
		break;
	case 4:
		KillTimer(4);
		if(theProcBank.m_strTrayID != _T(""))
		{
			CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TRAY_ID, GXCOLOR_ON);
			CGxUICtrl::SetStaticString(this, IDC_GXSTC_TRAY_ID, theProcBank.m_strTrayID);
			theUnitFunc.DCRTrigger(OFF);
		}
		else
		{
			CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TRAY_ID, GXCOLOR_RED);
			CGxUICtrl::SetStaticString(this, IDC_GXSTC_TRAY_ID, theProcBank.m_strTrayID);
			theUnitFunc.DCRTrigger(OFF);
		}
		::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_DCR_UPDATE, 0,0);

		break;
	}

	CFormView::OnTimer(nIDEvent);
}

LRESULT CFormTeachGoodTray::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	CGxUICtrl::BroadcastStaticCpationIndex(this, wParam, IDC_GXSTC_TRAYIN_VAC2);
	CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_GXBTN_TRAYIN_CONV_IN);

	return 0;
}

void CFormTeachGoodTray::SetCurPageAxis(AXIS_ID axis, TEACH_PARAM::ID param, UINT nID)
{
	m_CurAxis = axis;
	m_CurTeach = param;
	m_CurID	  = nID;	

	GetMainHandler()->SetSelectedAxis(m_CurAxis, m_CurTeach);
}

void CFormTeachGoodTray::UpdateOKTrayINState()
{
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYIN_CONV_IN, theUnitFunc.GetOutPutIOCheck(Y_GOOD_LD_CONV_TRAY_INVERTER_CW_RUN) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYIN_CONV_OUT, theUnitFunc.GetOutPutIOCheck(Y_GOOD_LD_CONV_TRAY_INVERTER_CCW_RUN) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYIN_CONV_STOP, ((!theUnitFunc.GetOutPutIOCheck(Y_GOOD_LD_CONV_TRAY_INVERTER_CW_RUN)) && (!theUnitFunc.GetOutPutIOCheck(Y_GOOD_LD_CONV_TRAY_INVERTER_CCW_RUN)) ? GXCOLOR_ON:GXCOLOR_OFF));
 	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYIN_CONV_CYLUP, theUnitFunc.BottomStoperUpDownChk(GOOD_IN_TRAY_PORT, CYL_UP,TRAY_IN) ? GXCOLOR_ON:GXCOLOR_OFF);  //설비 반영 후 센서 넘버 확인.
 	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYIN_CONV_CYLDOWN, theUnitFunc.BottomStoperUpDownChk(GOOD_IN_TRAY_PORT, CYL_DOWN,TRAY_IN) ? GXCOLOR_ON:GXCOLOR_OFF);  //설비 반영 후 센서 넘버 확인.
 	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYIN_CONV_CYLUP2, theUnitFunc.BottomStoperUpDownChk(GOOD_IN_TRAY_PORT, CYL_UP) ? GXCOLOR_ON:GXCOLOR_OFF);  //설비 반영 후 센서 넘버 확인.
 	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYIN_CONV_CYLDN2, theUnitFunc.BottomStoperUpDownChk(GOOD_IN_TRAY_PORT, CYL_DOWN) ? GXCOLOR_ON:GXCOLOR_OFF);  //설비 반영 후 센서 넘버 확인.
 	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYIN_LOAD_POS, theUnitFunc.InTrayLiftZ_DownChk()? GXCOLOR_ON:GXCOLOR_OFF);
 	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYIN_UP_MOVE, theUnitFunc.InTrayLiftZ_UpChk()? GXCOLOR_ON:GXCOLOR_OFF);
 	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYIN_IO_TOP_FWD, theUnitFunc.AlignCylFwdBwdChk(GOOD_IN_TRAY_PORT, CYL_FWD) ? GXCOLOR_ON:GXCOLOR_OFF);
 	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYIN_IO_TOP_BWD, theUnitFunc.AlignCylFwdBwdChk(GOOD_IN_TRAY_PORT, CYL_BWD) ? GXCOLOR_ON:GXCOLOR_OFF);
 	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYIN_IO_BTM_FWD, theUnitFunc.DivisionCylFwdBwdChk(GOOD_IN_TRAY_PORT, CYL_FWD) ? GXCOLOR_ON:GXCOLOR_OFF);
 	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYIN_IO_BTM_BWD, theUnitFunc.DivisionCylFwdBwdChk(GOOD_IN_TRAY_PORT, CYL_BWD) ? GXCOLOR_ON:GXCOLOR_OFF);
 	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYOUT_CONV_IN, theUnitFunc.GetOutPutIOCheck(Y_GOOD_ULD_CONV_TRAY_INVERTER_CW_RUN) ? GXCOLOR_ON:GXCOLOR_OFF);
 	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYOUT_CONV_OUT, theUnitFunc.GetOutPutIOCheck(Y_GOOD_ULD_CONV_TRAY_INVERTER_CCW_RUN) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYOUT_CONV_STOP,  ((!theUnitFunc.GetOutPutIOCheck(Y_GOOD_ULD_CONV_TRAY_INVERTER_CW_RUN)) && 
															 (!theUnitFunc.GetOutPutIOCheck(Y_GOOD_ULD_CONV_TRAY_INVERTER_CCW_RUN)) ? GXCOLOR_ON:GXCOLOR_OFF));
 	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYOUT_CONV_CYLUP, theUnitFunc.BottomStoperUpDownChk(GOOD_OUT_TRAY_PORT, CYL_UP,TRAY_IN) ? GXCOLOR_ON:GXCOLOR_OFF);  //설비 반영 후 센서 넘버 확인.
 	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYOUT_CONV_CYLDOWN, theUnitFunc.BottomStoperUpDownChk(GOOD_OUT_TRAY_PORT, CYL_DOWN,TRAY_IN) ? GXCOLOR_ON:GXCOLOR_OFF);  //설비 반영 후 센서 넘버 확인.
 	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYOUT_CONV_CYLUP2, theUnitFunc.BottomStoperUpDownChk(GOOD_OUT_TRAY_PORT, CYL_UP) ? GXCOLOR_ON:GXCOLOR_OFF);  //설비 반영 후 센서 넘버 확인.
 	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYOUT_CONV_CYLDOWN2, theUnitFunc.BottomStoperUpDownChk(GOOD_OUT_TRAY_PORT, CYL_DOWN) ? GXCOLOR_ON:GXCOLOR_OFF);  //설비 반영 후 센서 넘버 확인.
 	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYOUT_UNLOAD_POS, theUnitFunc.OutTrayLiftZ_DownChk()? GXCOLOR_ON:GXCOLOR_OFF);
 	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYOUT_UP_MOVE, theUnitFunc.OutTrayLiftZ_UpChk()? GXCOLOR_ON:GXCOLOR_OFF);
 	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYOUT_IO_TOP_FWD, theUnitFunc.AlignCylFwdBwdChk(GOOD_OUT_TRAY_PORT, CYL_FWD) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYOUT_IO_TOP_BWD, theUnitFunc.AlignCylFwdBwdChk(GOOD_OUT_TRAY_PORT, CYL_BWD) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYOUT_IO_BTM_FWD, theUnitFunc.DivisionCylFwdBwdChk(GOOD_OUT_TRAY_PORT, CYL_FWD) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYOUT_IO_BTM_BWD, theUnitFunc.DivisionCylFwdBwdChk(GOOD_OUT_TRAY_PORT, CYL_BWD) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYTR_VACON, theUnitFunc.TrayTr_VacChk(VAC_ON) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYTR_VACOFF, theUnitFunc.TrayTr_VacChk(VAC_OFF) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYTR_GETPOS, theUnitFunc.TrayTrY_InChk() ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYTR_PUTPOS, theUnitFunc.TrayTrY_OutChk() ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYTR_UPPOS, theUnitFunc.TrayTrZ_UpChk() ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYTR_GETDOWNPOS, theUnitFunc.TrayTrZ_InChk() ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYTR_PUTDOWNPOS, theUnitFunc.TrayTrZ_OutChk() ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYIN_IO_ALIGNGUIDE_FWD, theUnitFunc.BottomAlignCylFwdBwdChk(GOOD_IN_TRAY_PORT, CYL_FWD) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYIN_IO_ALIGNGUIDE_BWD, theUnitFunc.BottomAlignCylFwdBwdChk(GOOD_IN_TRAY_PORT, CYL_BWD) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYOUT_IO_ALIGNGUIDE_FWD, theUnitFunc.BottomAlignCylFwdBwdChk(GOOD_OUT_TRAY_PORT, CYL_FWD) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYOUT_IO_ALIGNGUIDE_BWD, theUnitFunc.BottomAlignCylFwdBwdChk(GOOD_OUT_TRAY_PORT, CYL_BWD) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYIN_BUFFERCONV_IN, theUnitFunc.GetOutPutIOCheck(Y_GOOD_LD_BUFFER_CONV_TRAY_INVERTER_CW_RUN) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYIN_BUFFERCONV_OUT, theUnitFunc.GetOutPutIOCheck(Y_GOOD_LD_BUFFER_CONV_TRAY_INVERTER_CCW_RUN) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYIN_BUFFERCONV_STOP, ((!theUnitFunc.GetOutPutIOCheck(Y_GOOD_LD_BUFFER_CONV_TRAY_INVERTER_CW_RUN)) && 
															   (!theUnitFunc.GetOutPutIOCheck(Y_GOOD_LD_BUFFER_CONV_TRAY_INVERTER_CCW_RUN)) ? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYOUT_BUFFERCONV_IN, theUnitFunc.GetOutPutIOCheck(Y_GOOD_ULD_BUFFER_CONV_TRAY_INVERTER_CW_RUN) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYOUT_BUFFERCONV_OUT, theUnitFunc.GetOutPutIOCheck(Y_GOOD_ULD_BUFFER_CONV_TRAY_INVERTER_CCW_RUN) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TRAYOUT_BUFFERCONV_STOP, ((!theUnitFunc.GetOutPutIOCheck(Y_GOOD_ULD_BUFFER_CONV_TRAY_INVERTER_CW_RUN)) && 
																(!theUnitFunc.GetOutPutIOCheck(Y_GOOD_ULD_BUFFER_CONV_TRAY_INVERTER_CCW_RUN)) ? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TRAYIN_TRAYCHK3, ((theUnitFunc.InConv_TrayChk(CONV_SEN_2, SENSOR_ON)) &&
														(theUnitFunc.InConv_TrayChk(CONV_SEN_3, SENSOR_ON)) &&
														(theUnitFunc.InConv_TrayChk(CONV_SEN_4, SENSOR_ON))) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TRAYOUT_TRAYCHK3, ((theUnitFunc.OutConv_TrayChk(CONV_SEN_2, SENSOR_ON)) &&
														 (theUnitFunc.OutConv_TrayChk(CONV_SEN_3, SENSOR_ON)) &&
														 (theUnitFunc.OutConv_TrayChk(CONV_SEN_4, SENSOR_ON))) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TRAYIN_TRAYCHK, theUnitFunc.LDTrayLiftTrayChk(SENSOR_ON) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TRAYIN_TRAYCHK2, theUnitFunc.InTrayAlignTrayChk(SENSOR_ON) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TRAYOUT_TRAYCHK, theUnitFunc.ULDTrayLiftTrayChk(SENSOR_ON) ? GXCOLOR_ON:GXCOLOR_OFF);
}

void CFormTeachGoodTray::ClickGxbtnTrayinConvIn()
{
	theUnitFunc.ConvRun(GOOD_IN_TRAY_PORT,ON,CW);
}

void CFormTeachGoodTray::ClickGxbtnTrayinConvOut()
{
	theUnitFunc.ConvRun(GOOD_IN_TRAY_PORT,ON,CCW);
}

void CFormTeachGoodTray::ClickGxbtnTrayinConvStop()
{
	theUnitFunc.ConvRun(GOOD_IN_TRAY_PORT,OFF);
}

void CFormTeachGoodTray::ClickGxbtnTrayinConvCylup()
{
	theUnitFunc.BottomStoperUpDown(GOOD_IN_TRAY_PORT, CYL_UP, TRAY_IN);
}

void CFormTeachGoodTray::ClickGxbtnTrayinConvCyldown()
{
	theUnitFunc.BottomStoperUpDown(GOOD_IN_TRAY_PORT, CYL_DOWN, TRAY_IN);
}

void CFormTeachGoodTray::ClickGxbtnTrayinBufferconvIn()
{
	theUnitFunc.BufConvRun(GOOD_IN_TRAY_PORT, ON, CW);
}

void CFormTeachGoodTray::ClickGxbtnTrayinBufferconvOut()
{
	theUnitFunc.BufConvRun(GOOD_IN_TRAY_PORT, ON, CCW);
}

void CFormTeachGoodTray::ClickGxbtnTrayinBufferconvStop()
{
	theUnitFunc.BufConvRun(GOOD_IN_TRAY_PORT, OFF);
}

void CFormTeachGoodTray::ClickGxbtnTrayinConvCylup2()
{
	theUnitFunc.BottomStoperUpDown(GOOD_IN_TRAY_PORT, CYL_UP);
}

void CFormTeachGoodTray::ClickGxbtnTrayinConvCyldn2()
{
	theUnitFunc.BottomStoperUpDown(GOOD_IN_TRAY_PORT, CYL_DOWN);
}

void CFormTeachGoodTray::ClickGxbtnTrayinLoadPos()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_GOOD_TRAY_IN_LIFT_Z;
	m_CurTeach = TEACH_PARAM::OK_TRAY_IN_LIFT_Z_TO_DOWN;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_TRAYIN_LOAD_POS);
}

void CFormTeachGoodTray::ClickGxbtnTrayinUpMove()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_GOOD_TRAY_IN_LIFT_Z;
	m_CurTeach = TEACH_PARAM::OK_TRAY_IN_LIFT_Z_TO_UP;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_TRAYIN_UP_MOVE);
}

//2018wjp
void CFormTeachGoodTray::ClickGxbtnTrayinTopMove()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_GOOD_TRAY_IN_LIFT_Z;
	m_CurTeach = TEACH_PARAM::OK_TRAY_IN_LIFT_Z_TO_TOP;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_TRAYIN_TOP_MOVE);
}

void CFormTeachGoodTray::ClickGxbtnTrayinPitchupMove()
{
	theUnitFunc.InTrayLiftZ_PitchUp(2);
}

void CFormTeachGoodTray::ClickGxbtnTrayinPitchdownMove()
{
	theUnitFunc.InTrayLiftZ_PitchDown(1.5);

	//2017-05-20,skcho, ok tray out 배출시 알람 발생하여 시퀀스가 끝나지 않은 상태에서 작업자가 트레이 뺐을 경우
	//ok in lift가 align을 하지 않고 무한정지하는 것을 임시 방지
	theProcBank.m_bCarrierReportWaitCheck = FALSE;
}

void CFormTeachGoodTray::ClickGxbtnTrayinIoTopFwd()
{
	theUnitFunc.AlignCylFwdBwd(GOOD_IN_TRAY_PORT, CYL_FWD);
}

void CFormTeachGoodTray::ClickGxbtnTrayinIoTopBwd()
{
	theUnitFunc.AlignCylFwdBwd(GOOD_IN_TRAY_PORT, CYL_BWD);
}

void CFormTeachGoodTray::ClickGxbtnTrayinIoBtmFwd()
{
	theUnitFunc.DivisionCylFwdBwd(GOOD_IN_TRAY_PORT, CYL_FWD);
}

void CFormTeachGoodTray::ClickGxbtnTrayinIoBtmBwd()
{
	theUnitFunc.DivisionCylFwdBwd(GOOD_IN_TRAY_PORT, CYL_BWD);
}

void CFormTeachGoodTray::ClickGxbtnTrayoutConvIn()
{
	theUnitFunc.ConvRun(GOOD_OUT_TRAY_PORT,ON,CW);
}

void CFormTeachGoodTray::ClickGxbtnTrayoutConvOut()
{
	theUnitFunc.ConvRun(GOOD_OUT_TRAY_PORT,ON,CCW);
}

void CFormTeachGoodTray::ClickGxbtnTrayoutConvStop()
{
	theUnitFunc.ConvRun(GOOD_OUT_TRAY_PORT,OFF);
}

void CFormTeachGoodTray::ClickGxbtnTrayoutConvCylup()
{
	theUnitFunc.BottomStoperUpDown(GOOD_OUT_TRAY_PORT, CYL_UP,TRAY_IN);
}

void CFormTeachGoodTray::ClickGxbtnTrayoutConvCyldown()
{
	theUnitFunc.BottomStoperUpDown(GOOD_OUT_TRAY_PORT, CYL_DOWN,TRAY_IN);
}

void CFormTeachGoodTray::ClickGxbtnTrayoutBufferconvIn()
{
	theUnitFunc.BufConvRun(GOOD_OUT_TRAY_PORT, ON, CW);
}

void CFormTeachGoodTray::ClickGxbtnTrayoutBufferconvOut()
{
	theUnitFunc.BufConvRun(GOOD_OUT_TRAY_PORT, ON, CCW);
}

void CFormTeachGoodTray::ClickGxbtnTrayoutBufferconvStop()
{
	theUnitFunc.BufConvRun(GOOD_OUT_TRAY_PORT, OFF);
}

void CFormTeachGoodTray::ClickGxbtnTrayoutUnloadPos()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_GOOD_TRAY_OUT_LIFT_Z;
	m_CurTeach = TEACH_PARAM::OK_TRAY_OUT_LIFT_Z_TO_DOWN;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_TRAYOUT_UNLOAD_POS);
}

void CFormTeachGoodTray::ClickGxbtnTrayoutUpMove()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_GOOD_TRAY_OUT_LIFT_Z;
	m_CurTeach = TEACH_PARAM::OK_TRAY_OUT_LIFT_Z_TO_UP;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_TRAYOUT_UP_MOVE);
}

void CFormTeachGoodTray::ClickGxbtnTrayoutPitchupMove()
{
	theUnitFunc.OutTrayLiftZ_PitchUp(2);
}

void CFormTeachGoodTray::ClickGxbtnTrayoutPitchdownMove()
{
	theUnitFunc.OutTrayLiftZ_PitchDown(1);
}

void CFormTeachGoodTray::ClickGxbtnTraytrVacon()
{
	theUnitFunc.TrayTr_VacOnOff(VAC_ON);
}

void CFormTeachGoodTray::ClickGxbtnTraytrVacoff()
{
	theUnitFunc.TrayTr_VacOnOff(VAC_OFF);
	Sleep(200);
	theUnitFunc.TrayTr_BlowOff();

}

void CFormTeachGoodTray::ClickGxbtnTraytrGetpos()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_GOOD_TRAY_TR_Y;
	m_CurTeach = TEACH_PARAM::OK_TRAY_TR_Y_TO_IN;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_TRAYTR_GETPOS);
}

void CFormTeachGoodTray::ClickGxbtnTraytrPutpos()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_GOOD_TRAY_TR_Y;
	m_CurTeach = TEACH_PARAM::OK_TRAY_TR_Y_TO_OUT;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_TRAYTR_PUTPOS);
}

void CFormTeachGoodTray::ClickGxbtnTraytrUppos()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_GOOD_TRAY_TR_Z;
	m_CurTeach = TEACH_PARAM::OK_TRAY_TR_Z_TO_UP;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_TRAYTR_UPPOS);
}

void CFormTeachGoodTray::ClickGxbtnTraytrGetDownpos()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_GOOD_TRAY_TR_Z;
	m_CurTeach = TEACH_PARAM::OK_TRAY_TR_Z_TO_IN;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_TRAYTR_GETDOWNPOS);
}

void CFormTeachGoodTray::ClickGxbtnTraytrPutDownpos()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_GOOD_TRAY_TR_Z;
	m_CurTeach = TEACH_PARAM::OK_TRAY_TR_Z_TO_OUT;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_TRAYTR_PUTDOWNPOS);
}

void CFormTeachGoodTray::ClickGxbtnTrayinIoAlignguideFwd()
{
	theUnitFunc.BottomAlignCylFwdBwd(GOOD_IN_TRAY_PORT, CYL_FWD);
}

void CFormTeachGoodTray::ClickGxbtnTrayinIoAlignguideBwd()
{
	theUnitFunc.BottomAlignCylFwdBwd(GOOD_IN_TRAY_PORT, CYL_BWD);
}

void CFormTeachGoodTray::ClickGxbtnTrayoutIoAlignguideFwd()
{
	theUnitFunc.BottomAlignCylFwdBwd(GOOD_OUT_TRAY_PORT, CYL_FWD);
}

void CFormTeachGoodTray::ClickGxbtnTrayoutIoAlignguideBwd()
{
	theUnitFunc.BottomAlignCylFwdBwd(GOOD_OUT_TRAY_PORT, CYL_BWD);
}

void CFormTeachGoodTray::ClickGxbtnTrayoutConvCylup2()
{
	theUnitFunc.BottomStoperUpDown(GOOD_OUT_TRAY_PORT, CYL_UP);
}

void CFormTeachGoodTray::ClickGxbtnTrayoutConvCyldown2()
{
	theUnitFunc.BottomStoperUpDown(GOOD_OUT_TRAY_PORT, CYL_DOWN);
}

void CFormTeachGoodTray::ClickGxbtnOktrayAlignStart()
{
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_OKTRAY_ALIGN_OK, GXCOLOR_OFF);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_OKTRAY_ALIGN_OK, _T(""));

	theProcBank.m_strOKTray_AlignOK = _T("");

	theUnitFunc.TrayAlignStart(_T("OKTRAY"));
	SetTimer(2,2000,NULL);
}

void CFormTeachGoodTray::ClickGxbtnOktrayAlignTeaching()
{
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_OKTRAY_TEACHING_OK, GXCOLOR_OFF);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_OKTRAY_TEACHING_OK, _T(""));

	theProcBank.m_strOKTray_TeachingOK = _T("");

	theUnitFunc.TrayAlignTeaching(_T("OKTRAY"));
	SetTimer(3,2000,NULL);
}

void CFormTeachGoodTray::ClickGxbtnTrayIDReading()
{
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TRAY_ID, GXCOLOR_OFF);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TRAY_ID, _T(""));

	theUnitFunc.DCRTrigger(ON);
	SetTimer(4,2000,NULL);
}

void CFormTeachGoodTray::ClickGxbtnTrayidReconnect()
{
	if(theSocketInterFace.m_pSocket_DCR.IsConnected())
	{
		theSocketInterFace.CreateDCRSocket();
		Sleep(500);
		theSocketInterFace.ConnectDCRSocket();
	}
	
}


void CFormTeachGoodTray::ClickGxbtnTrayinAlignPos()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_GOOD_TRAY_IN_LIFT_Z;
	m_CurTeach = TEACH_PARAM::OK_TRAY_IN_LIFT_Z_TO_ALIGN;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_TRAYIN_ALIGN_POS);
}


void CFormTeachGoodTray::ClickGxbtnTrayoutAlignPos()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_GOOD_TRAY_OUT_LIFT_Z;
	m_CurTeach = TEACH_PARAM::OK_TRAY_OUT_LIFT_Z_TO_ALIGN;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_TRAYOUT_ALIGN_POS);
}
