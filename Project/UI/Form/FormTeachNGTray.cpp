// FormTeachNGTray.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "resource.h"
#include "FormTeachNGTray.h"
#include "GUIDefine.h"

//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "UI\GausGUI\GxButtonEx.h"
#include "UI\GausGUI\GxStaticEx.h"

#include "UI\GausGUI\GxMsgBox.h"
// CFormTeachNGTray

IMPLEMENT_DYNCREATE(CFormTeachNGTray, CFormView)

CFormTeachNGTray::CFormTeachNGTray()
	: CFormView(CFormTeachNGTray::IDD)
{
	m_bInit = FALSE;
}

CFormTeachNGTray::~CFormTeachNGTray()
{
}

void CFormTeachNGTray::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFormTeachNGTray, CFormView)
	ON_WM_TIMER()
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)			// 표시 언어가 바뀌었다.
END_MESSAGE_MAP()


// CFormTeachNGTray 진단입니다.

#ifdef _DEBUG
void CFormTeachNGTray::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormTeachNGTray::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormTeachNGTray 메시지 처리기입니다.

BOOL CFormTeachNGTray::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style = WS_OVERLAPPED | WS_CHILD;				// 테두리를 없앤다.

	return CFormView::PreCreateWindow(cs);
}

BEGIN_EVENTSINK_MAP(CFormTeachNGTray, CFormView)
	ON_EVENT(CFormTeachNGTray, IDC_GXBTN_NGTRAY_CONV_IN, DISPID_CLICK, CFormTeachNGTray::ClickGxbtnNgtrayConvIn, VTS_NONE)
	ON_EVENT(CFormTeachNGTray, IDC_GXBTN_NGTRAY_CONV_OUT, DISPID_CLICK, CFormTeachNGTray::ClickGxbtnNgtrayConvOut, VTS_NONE)
	ON_EVENT(CFormTeachNGTray, IDC_GXBTN_NGTRAY_CONV_STOP, DISPID_CLICK, CFormTeachNGTray::ClickGxbtnNgtrayConvStop, VTS_NONE)
	ON_EVENT(CFormTeachNGTray, IDC_GXBTN_NGTRAY_CONV_CYLUP, DISPID_CLICK, CFormTeachNGTray::ClickGxbtnNgtrayConvCylup, VTS_NONE)
	ON_EVENT(CFormTeachNGTray, IDC_GXBTN_NGTRAY_BUFFERCONV_IN, DISPID_CLICK, CFormTeachNGTray::ClickGxbtnNgtrayBufferconvIn, VTS_NONE)
	ON_EVENT(CFormTeachNGTray, IDC_GXBTN_NGTRAY_BUFFERCONV_OUT, DISPID_CLICK, CFormTeachNGTray::ClickGxbtnNgtrayBufferconvOut, VTS_NONE)
	ON_EVENT(CFormTeachNGTray, IDC_GXBTN_NGTRAY_BUFFERCONV_STOP, DISPID_CLICK, CFormTeachNGTray::ClickGxbtnNgtrayBufferconvStop, VTS_NONE)
	ON_EVENT(CFormTeachNGTray, IDC_GXBTN_NGTRAY_CONV_CYLDOWN, DISPID_CLICK, CFormTeachNGTray::ClickGxbtnNgtrayConvCyldown, VTS_NONE)
	ON_EVENT(CFormTeachNGTray, IDC_GXBTN_NGTRAYIN_LOAD_POS, DISPID_CLICK, CFormTeachNGTray::ClickGxbtnNgtrayinLoadPos, VTS_NONE)
	ON_EVENT(CFormTeachNGTray, IDC_GXBTN_NGTRAYIN_UP_MOVE, DISPID_CLICK, CFormTeachNGTray::ClickGxbtnNgtrayinUpMove, VTS_NONE)
	ON_EVENT(CFormTeachNGTray, IDC_GXBTN_NGTRAYIN_PITCHUP_MOVE, DISPID_CLICK, CFormTeachNGTray::ClickGxbtnNgtrayinPitchupMove, VTS_NONE)
	ON_EVENT(CFormTeachNGTray, IDC_GXBTN_NGTRAYIN_PITCHDOWN_MOVE, DISPID_CLICK, CFormTeachNGTray::ClickGxbtnNgtrayinPitchdownMove, VTS_NONE)
	ON_EVENT(CFormTeachNGTray, IDC_GXBTN_NGTRAYIN_LIFT_BWD, DISPID_CLICK, CFormTeachNGTray::ClickGxbtnNgtrayinLiftBwd, VTS_NONE)
	ON_EVENT(CFormTeachNGTray, IDC_GXBTN_NGTRAYIN_LIFT_FWD, DISPID_CLICK, CFormTeachNGTray::ClickGxbtnNgtrayinLiftFwd, VTS_NONE)
	ON_EVENT(CFormTeachNGTray, IDC_GXBTN_NGTRAYOUT_UNLOAD_POS, DISPID_CLICK, CFormTeachNGTray::ClickGxbtnNgtrayoutUnloadPos, VTS_NONE)
	ON_EVENT(CFormTeachNGTray, IDC_GXBTN_NGTRAYOUT_UP_MOVE, DISPID_CLICK, CFormTeachNGTray::ClickGxbtnNgtrayoutUpMove, VTS_NONE)
	ON_EVENT(CFormTeachNGTray, IDC_GXBTN_NGTRAYOUT_PITCHUP_MOVE, DISPID_CLICK, CFormTeachNGTray::ClickGxbtnNgtrayoutPitchupMove, VTS_NONE)
	ON_EVENT(CFormTeachNGTray, IDC_GXBTN_NGTRAYOUT_PITCHDOWN_MOVE, DISPID_CLICK, CFormTeachNGTray::ClickGxbtnNgtrayoutPitchdownMove, VTS_NONE)
	ON_EVENT(CFormTeachNGTray, IDC_GXBTN_NGTRAYOUT_LIFT_BWD, DISPID_CLICK, CFormTeachNGTray::ClickGxbtnNgtrayoutLiftBwd, VTS_NONE)
	ON_EVENT(CFormTeachNGTray, IDC_GXBTN_NGTRAYOUT_LIFT_FWD, DISPID_CLICK, CFormTeachNGTray::ClickGxbtnNgtrayoutLiftFwd, VTS_NONE)
	ON_EVENT(CFormTeachNGTray, IDC_GXBTN_NGTRAYIN_IO_TOP_FWD, DISPID_CLICK, CFormTeachNGTray::ClickGxbtnNgtrayinIoTopFwd, VTS_NONE)
	ON_EVENT(CFormTeachNGTray, IDC_GXBTN_NGTRAYIN_IO_TOP_BWD, DISPID_CLICK, CFormTeachNGTray::ClickGxbtnNgtrayinIoTopBwd, VTS_NONE)
	ON_EVENT(CFormTeachNGTray, IDC_GXBTN_NGTRAYIN_IO_BTM_FWD, DISPID_CLICK, CFormTeachNGTray::ClickGxbtnNgtrayinIoBtmFwd, VTS_NONE)
	ON_EVENT(CFormTeachNGTray, IDC_GXBTN_NGTRAYIN_IO_BTM_BWD, DISPID_CLICK, CFormTeachNGTray::ClickGxbtnNgtrayinIoBtmBwd, VTS_NONE)
	ON_EVENT(CFormTeachNGTray, IDC_GXBTN_NGTRAYIN_IO_ALIGNGUIDE_FWD, DISPID_CLICK, CFormTeachNGTray::ClickGxbtnNgtrayinIoAlignguideFwd, VTS_NONE)
	ON_EVENT(CFormTeachNGTray, IDC_GXBTN_NGTRAYIN_IO_ALIGNGUIDE_BWD, DISPID_CLICK, CFormTeachNGTray::ClickGxbtnNgtrayinIoAlignguideBwd, VTS_NONE)
	ON_EVENT(CFormTeachNGTray, IDC_GXBTN_NGTRAYTR_VACON, DISPID_CLICK, CFormTeachNGTray::ClickGxbtnNgtraytrVacon, VTS_NONE)
	ON_EVENT(CFormTeachNGTray, IDC_GXBTN_NGTRAYTR_VACOFF, DISPID_CLICK, CFormTeachNGTray::ClickGxbtnNgtraytrVacoff, VTS_NONE)
	ON_EVENT(CFormTeachNGTray, IDC_GXBTN_NGTRAYTR_GETPOS, DISPID_CLICK, CFormTeachNGTray::ClickGxbtnNgtraytrGetpos, VTS_NONE)
	ON_EVENT(CFormTeachNGTray, IDC_GXBTN_NGTRAYTR_PUTPOS, DISPID_CLICK, CFormTeachNGTray::ClickGxbtnNgtraytrPutpos, VTS_NONE)
	ON_EVENT(CFormTeachNGTray, IDC_GXBTN_NGTRAYTR_UPPOS, DISPID_CLICK, CFormTeachNGTray::ClickGxbtnNgtraytrUppos, VTS_NONE)
	ON_EVENT(CFormTeachNGTray, IDC_GXBTN_NGTRAYTR_GETDOWNPOS, DISPID_CLICK, CFormTeachNGTray::ClickGxbtnNgtraytrGetDownpos, VTS_NONE)
	ON_EVENT(CFormTeachNGTray, IDC_GXBTN_NGTRAYTR_PUTDOWNPOS, DISPID_CLICK, CFormTeachNGTray::ClickGxbtnNgtraytrPutDownpos, VTS_NONE)
	ON_EVENT(CFormTeachNGTray, IDC_GXBTN_NGTRAYIN_TRAY_UP, DISPID_CLICK, CFormTeachNGTray::ClickGxbtnNgtrayinTrayUp, VTS_NONE)
	ON_EVENT(CFormTeachNGTray, IDC_GXBTN_NGTRAYIN_TRAY_DOWN, DISPID_CLICK, CFormTeachNGTray::ClickGxbtnNgtrayinTrayDown, VTS_NONE)
	ON_EVENT(CFormTeachNGTray, IDC_GXBTN_NGTRAYOUT_TRAY_UP, DISPID_CLICK, CFormTeachNGTray::ClickGxbtnNgtrayoutTrayUp, VTS_NONE)
	ON_EVENT(CFormTeachNGTray, IDC_GXBTN_NGTRAYOUT_TRAY_DOWN, DISPID_CLICK, CFormTeachNGTray::ClickGxbtnNgtrayoutTrayDown, VTS_NONE)
	ON_EVENT(CFormTeachNGTray, IDC_GXBTN_NGBUFTRAY_CONV_CYLUP, DISPID_CLICK, CFormTeachNGTray::ClickGxbtnNgbuftrayConvCylup, VTS_NONE)
	ON_EVENT(CFormTeachNGTray, IDC_GXBTN_NGBUFTRAY_CONV_CYLDOWN, DISPID_CLICK, CFormTeachNGTray::ClickGxbtnNgbuftrayConvCyldown, VTS_NONE)
	ON_EVENT(CFormTeachNGTray, IDC_GXBTN_NGTRAY_ALIGN_START, DISPID_CLICK, CFormTeachNGTray::ClickGxbtnNgtrayAlignStart, VTS_NONE)
	ON_EVENT(CFormTeachNGTray, IDC_GXBTN_NGTRAY_ALIGN_TEACHING, DISPID_CLICK, CFormTeachNGTray::ClickGxbtnNgtrayAlignTeaching, VTS_NONE)
	ON_EVENT(CFormTeachNGTray, IDC_GXBTN_NGTRAYIN_MID_POS, DISPID_CLICK, CFormTeachNGTray::ClickGxbtnNgtrayinMidPos, VTS_NONE)
END_EVENTSINK_MAP()

LRESULT CFormTeachNGTray::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{
		UpdateNGTrayINState();
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

void CFormTeachNGTray::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case 1:
		UpdateNGTrayINState();
		break;
	case 2:
		KillTimer(2);
		if (theProcBank.m_strNGTray_AlignOK == _T("OK"))
		{
			CGxUICtrl::SetStaticColor(this, IDC_GXSTC_NGTRAY_ALIGN_OK, GXCOLOR_ON);
			CGxUICtrl::SetStaticString(this, IDC_GXSTC_NGTRAY_ALIGN_OK, _T("ALIGN OK"));
		}
		else
		{
			CGxUICtrl::SetStaticColor(this, IDC_GXSTC_NGTRAY_ALIGN_OK, GXCOLOR_RED);
			CGxUICtrl::SetStaticString(this, IDC_GXSTC_NGTRAY_ALIGN_OK, _T("ALIGN NG"));
		}
		break;
	case 3:
		KillTimer(3);
		if (theProcBank.m_strNGTray_TeachingOK == _T("OK"))
		{
			CGxUICtrl::SetStaticColor(this, IDC_GXSTC_NGTRAY_TEACHING_OK, GXCOLOR_ON);
			CGxUICtrl::SetStaticString(this, IDC_GXSTC_NGTRAY_TEACHING_OK, _T("TEACHING OK"));
		}
		else
		{
			CGxUICtrl::SetStaticColor(this, IDC_GXSTC_NGTRAY_TEACHING_OK, GXCOLOR_RED);
			CGxUICtrl::SetStaticString(this, IDC_GXSTC_NGTRAY_TEACHING_OK, _T("TEACHING NG"));
		}
		break;
	}

	CFormView::OnTimer(nIDEvent);
}

LRESULT CFormTeachNGTray::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	CGxUICtrl::BroadcastStaticCpationIndex(this, wParam, IDC_GXSTC_TRAYIN_VAC3);
	CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_GXBTN_NGTRAYIN_LOAD_POS);

	return 0;
}

void CFormTeachNGTray::SetCurPageAxis(AXIS_ID axis, TEACH_PARAM::ID param, UINT nID)
{
	m_CurAxis = axis;
	m_CurTeach = param;
	m_CurID	  = nID;	

	GetMainHandler()->SetSelectedAxis(m_CurAxis, m_CurTeach);
}

void CFormTeachNGTray::UpdateNGTrayINState()
{
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_NGTRAY_CONV_IN, theUnitFunc.GetOutPutIOCheck(Y_NG_CONV_TRAY_INVERTER_CW_RUN) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_NGTRAY_CONV_OUT, theUnitFunc.GetOutPutIOCheck(Y_NG_CONV_TRAY_INVERTER_CCW_RUN) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_NGTRAY_CONV_STOP, ((!theUnitFunc.GetOutPutIOCheck(Y_NG_CONV_TRAY_INVERTER_CW_RUN)) && 
														  (!theUnitFunc.GetOutPutIOCheck(Y_NG_CONV_TRAY_INVERTER_CCW_RUN)) ? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_NGTRAYIN_LOAD_POS, theUnitFunc.InNGTrayLiftZ_DownChk()? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_NGTRAYIN_UP_MOVE, theUnitFunc.InNGTrayLiftZ_UpChk()? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_NGTRAYIN_IO_TOP_FWD, theUnitFunc.AlignCylFwdBwdChk(NG_TRAY_PORT, CYL_FWD) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_NGTRAYIN_IO_TOP_BWD, theUnitFunc.AlignCylFwdBwdChk(NG_TRAY_PORT, CYL_BWD) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_NGTRAYIN_IO_BTM_FWD, theUnitFunc.DivisionCylFwdBwdChk(NG_TRAY_PORT, CYL_FWD) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_NGTRAYIN_IO_BTM_BWD, theUnitFunc.DivisionCylFwdBwdChk(NG_TRAY_PORT, CYL_BWD) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_NGTRAY_CONV_CYLUP, theUnitFunc.BottomStoperUpDownChk(NG_TRAY_PORT, CYL_UP, TRAY_IN) ? GXCOLOR_ON:GXCOLOR_OFF);  //설비 반영 후 센서 넘버 확인.
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_NGTRAY_CONV_CYLDOWN, theUnitFunc.BottomStoperUpDownChk(NG_TRAY_PORT, CYL_DOWN, TRAY_IN) ? GXCOLOR_ON:GXCOLOR_OFF);  //설비 반영 후 센서 넘버 확인.
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_NGBUFTRAY_CONV_CYLUP, theUnitFunc.BottomStoperUpDownChk(NG_TRAY_PORT, CYL_UP) ? GXCOLOR_ON:GXCOLOR_OFF);  //설비 반영 후 센서 넘버 확인.
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_NGBUFTRAY_CONV_CYLDOWN, theUnitFunc.BottomStoperUpDownChk(NG_TRAY_PORT, CYL_DOWN) ? GXCOLOR_ON:GXCOLOR_OFF);  //설비 반영 후 센서 넘버 확인.
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_NGTRAYIN_LIFT_BWD, theUnitFunc.NGLiftArmFwdBwdChk(NG_TRAY_PORT, CYL_BWD) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_NGTRAYIN_LIFT_FWD, theUnitFunc.NGLiftArmFwdBwdChk(NG_TRAY_PORT, CYL_FWD) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_NGTRAYOUT_UNLOAD_POS, theUnitFunc.OutNGTrayLiftZ_DownChk() ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_NGTRAYOUT_UP_MOVE, theUnitFunc.OutNGTrayLiftZ_UpChk()? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_NGTRAYOUT_LIFT_BWD, theUnitFunc.NGLiftArmFwdBwdChk(NG_BUFFER_TRAY_PORT, CYL_BWD) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_NGTRAYOUT_LIFT_FWD, theUnitFunc.NGLiftArmFwdBwdChk(NG_BUFFER_TRAY_PORT, CYL_FWD) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_NGTRAYTR_VACON, theUnitFunc.NGTrayTr_VacChk(VAC_ON) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_NGTRAYTR_VACOFF, theUnitFunc.NGTrayTr_VacChk(VAC_OFF) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_NGTRAYTR_GETPOS, theUnitFunc.NGTrayTrX_InChk() ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_NGTRAYTR_PUTPOS, theUnitFunc.NGTrayTrX_OutChk() ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_NGTRAYTR_UPPOS, theUnitFunc.NGTrayTrZ_UpChk() ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_NGTRAYTR_GETDOWNPOS, theUnitFunc.NGTrayTrZ_InChk() ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_NGTRAYTR_PUTDOWNPOS, theUnitFunc.NGTrayTrZ_OutChk() ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_NGTRAYIN_IO_ALIGNGUIDE_FWD, theUnitFunc.BottomAlignCylFwdBwdChk(NG_TRAY_PORT, CYL_FWD) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_NGTRAYIN_IO_ALIGNGUIDE_BWD, theUnitFunc.BottomAlignCylFwdBwdChk(NG_TRAY_PORT, CYL_BWD) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_NGTRAY_BUFFERCONV_IN, theUnitFunc.GetOutPutIOCheck(Y_NG_BUFFER_CONV_TRAY_INVERTER_CW_RUN) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_NGTRAY_BUFFERCONV_OUT, theUnitFunc.GetOutPutIOCheck(Y_NG_BUFFER_CONV_TRAY_INVERTER_CCW_RUN) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_NGTRAY_BUFFERCONV_STOP, ((!theUnitFunc.GetOutPutIOCheck(Y_NG_BUFFER_CONV_TRAY_INVERTER_CW_RUN)) && 
															   (!theUnitFunc.GetOutPutIOCheck(Y_NG_BUFFER_CONV_TRAY_INVERTER_CCW_RUN)) ? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_NGTRAYIN_TRAY_UP,    theUnitFunc.NGTrayUpDownChk(NG_TRAY_PORT, CYL_UP) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_NGTRAYIN_TRAY_DOWN,   theUnitFunc.NGTrayUpDownChk(NG_TRAY_PORT, CYL_DOWN) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_NGTRAYOUT_TRAY_UP,    theUnitFunc.NGTrayUpDownChk(NG_BUFFER_TRAY_PORT, CYL_UP) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_NGTRAYOUT_TRAY_DOWN,  theUnitFunc.NGTrayUpDownChk(NG_BUFFER_TRAY_PORT, CYL_DOWN) ? GXCOLOR_ON:GXCOLOR_OFF);

	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_NGTRAYIN_TRAYCHK3, ((theUnitFunc.NGConv_TrayChk(NG_TRAY_PORT, CONV_SEN_2, SENSOR_ON)) &&
		(theUnitFunc.NGConv_TrayChk(NG_TRAY_PORT, CONV_SEN_3, SENSOR_ON)) &&
		(theUnitFunc.NGConv_TrayChk(NG_TRAY_PORT, CONV_SEN_4, SENSOR_ON))) ? GXCOLOR_ON:GXCOLOR_OFF);

	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_NGTRAYIN_TRAYCHK, theUnitFunc.NGTrayLiftTrayChk(SENSOR_ON) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_NGTRAYIN_TRAYCHK2, theUnitFunc.NGTrayInAlignTrayChk(SENSOR_ON) ? GXCOLOR_ON:GXCOLOR_OFF);

	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_NGTRAYOUT_TRAYCHK2, ((theUnitFunc.NGConv_TrayChk(NG_BUFFER_TRAY_PORT, CONV_SEN_2, SENSOR_ON)) &&
		(theUnitFunc.NGConv_TrayChk(NG_BUFFER_TRAY_PORT, CONV_SEN_3, SENSOR_ON)) &&
		(theUnitFunc.NGConv_TrayChk(NG_BUFFER_TRAY_PORT, CONV_SEN_4, SENSOR_ON))) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_NGTRAYOUT_TRAYCHK, theUnitFunc.NGTrayOutLiftTrayChk(SENSOR_ON) ? GXCOLOR_ON:GXCOLOR_OFF);
}

void CFormTeachNGTray::ClickGxbtnNgtrayConvIn()
{
	theUnitFunc.ConvRun(NG_TRAY_PORT,ON,CW);
}


void CFormTeachNGTray::ClickGxbtnNgtrayConvOut()
{
	theUnitFunc.ConvRun(NG_TRAY_PORT,ON,CCW);
}


void CFormTeachNGTray::ClickGxbtnNgtrayConvStop()
{
	theUnitFunc.ConvRun(NG_TRAY_PORT,OFF);
}


void CFormTeachNGTray::ClickGxbtnNgtrayConvCylup()
{
	theUnitFunc.BottomStoperUpDown(NG_TRAY_PORT, CYL_UP, TRAY_IN);
}


void CFormTeachNGTray::ClickGxbtnNgtrayConvCyldown()
{
	theUnitFunc.BottomStoperUpDown(NG_TRAY_PORT, CYL_DOWN, TRAY_IN);
}

void CFormTeachNGTray::ClickGxbtnNgtrayBufferconvIn()
{
	theUnitFunc.BufConvRun(NG_BUFFER_TRAY_PORT,ON,CW);
}


void CFormTeachNGTray::ClickGxbtnNgtrayBufferconvOut()
{
	theUnitFunc.BufConvRun(NG_BUFFER_TRAY_PORT,ON,CCW);
}


void CFormTeachNGTray::ClickGxbtnNgtrayBufferconvStop()
{
	theUnitFunc.BufConvRun(NG_BUFFER_TRAY_PORT,OFF);
}


void CFormTeachNGTray::ClickGxbtnNgtrayinLoadPos()
{	
	CGxMsgBox	dlgMsgBox;
	//Teachinterlock에서는 시퀀스에서도 반영되서 여기로 뺌 2017/05/03 HSK.
	if((theUnitFunc.NGConv_TrayChk(NG_TRAY_PORT, CONV_SEN_3, SENSOR_ON)
		|| theUnitFunc.NGConv_TrayChk(NG_TRAY_PORT, CONV_SEN_4, SENSOR_ON)
		|| theUnitFunc.NGConv_TrayChk(NG_TRAY_PORT, CONV_SEN_5, SENSOR_ON)
		|| theUnitFunc.TrayData_Exist(NG_TRAY_PORT, TRAY_INFO_POS_NG_IN_BUFFER)))
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("NG TRAY IN CONV TRAY를 빼주세요"),	//한
				_T("NG TRAY IN CONV TRAY CHECK Please."),		//영
				_T("NG TRAY IN CONV TRAY CHECK Please.") );		//베

		dlgMsgBox.DoModal();
		return;
	}
	m_CurAxis = (AXIS_ID)AXIS_SVO_NG_TRAY_IN_LIFT_Z;
	m_CurTeach = TEACH_PARAM::NG_TRAY_IN_LIFT_Z_TO_DOWN;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_NGTRAYIN_LOAD_POS);
}


void CFormTeachNGTray::ClickGxbtnNgtrayinUpMove()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_NG_TRAY_IN_LIFT_Z;
	m_CurTeach = TEACH_PARAM::NG_TRAY_IN_LIFT_Z_TO_UP;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_NGTRAYIN_UP_MOVE);
}


void CFormTeachNGTray::ClickGxbtnNgtrayinPitchupMove()
{
	theUnitFunc.InNGTrayLiftZ_PitchUp(2);
}


void CFormTeachNGTray::ClickGxbtnNgtrayinPitchdownMove()
{
	theUnitFunc.InNGTrayLiftZ_PitchDown(1.5);
}


void CFormTeachNGTray::ClickGxbtnNgtrayinLiftBwd()
{
	// NG TRAY IN LIFT 티칭 인터락 추가 2017/05/01 HSK
	CGxMsgBox	dlgMsgBox;
	if(theUnitFunc.InNGTrayLiftZ_MidPosChk() || theUnitFunc.InNGTrayLiftZ_DownChk())
		theUnitFunc.NGLiftArmFwdBwd(NG_TRAY_PORT, CYL_BWD);
	else
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(TRUE, 
			_T("LIFT ARM을 미들 포지션으로 이동 시켜 주세요"), 
			_T("LIFT ARM MID POS MOVE"),		
			_T("LIFT ARM MID POS MOVE") );

		dlgMsgBox.DoModal();
		return;
	}
}


void CFormTeachNGTray::ClickGxbtnNgtrayinLiftFwd()
{
	// NG TRAY IN LIFT 티칭 인터락 추가 2017/05/01 HSK
	CGxMsgBox	dlgMsgBox;
	if((theUnitFunc.TrayData_Exist(NG_TRAY_PORT, TRAY_INFO_POS_NG_IN_BUFFER) == FALSE)
		&& (theUnitFunc.NGConv_TrayChk(NG_TRAY_PORT, CONV_SEN_3, SENSOR_OFF)
		&& theUnitFunc.NGConv_TrayChk(NG_TRAY_PORT, CONV_SEN_4, SENSOR_OFF)
		&& theUnitFunc.NGConv_TrayChk(NG_TRAY_PORT, CONV_SEN_5, SENSOR_OFF)))
		theUnitFunc.NGLiftArmFwdBwd(NG_TRAY_PORT, CYL_FWD);
	else
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(TRUE, 
			_T("NG IN PORT TRAY 상태를 확인 해주세요"), 
			_T("NG IN PORT TRAY CHECK"),		
			_T("NG IN PORT TRAY CHECK") );

		dlgMsgBox.DoModal();
		return;
	}

}


void CFormTeachNGTray::ClickGxbtnNgtrayoutUnloadPos()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_NG_TRAY_OUT_LIFT_Z;
	m_CurTeach = TEACH_PARAM::NG_TRAY_OUT_LIFT_Z_TO_DOWN;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_NGTRAYOUT_UNLOAD_POS);
}


void CFormTeachNGTray::ClickGxbtnNgtrayoutUpMove()
{
	m_CurAxis =(AXIS_ID)AXIS_SVO_NG_TRAY_OUT_LIFT_Z;
	//m_CurAxis = (AXIS_ID)AXIS_SVO_NG_TRAY_IN_LIFT_Z;
	m_CurTeach = TEACH_PARAM::NG_TRAY_OUT_LIFT_Z_TO_UP;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_NGTRAYOUT_UP_MOVE);
}


void CFormTeachNGTray::ClickGxbtnNgtrayoutPitchupMove()
{
	theUnitFunc.OutNGTrayLiftZ_PitchUp(2);
}


void CFormTeachNGTray::ClickGxbtnNgtrayoutPitchdownMove()
{
	theUnitFunc.OutNGTrayLiftZ_PitchDown(1);
}


void CFormTeachNGTray::ClickGxbtnNgtrayoutLiftBwd()
{
	theUnitFunc.NGLiftArmFwdBwd(NG_BUFFER_TRAY_PORT, CYL_BWD);
}


void CFormTeachNGTray::ClickGxbtnNgtrayoutLiftFwd()
{
	theUnitFunc.NGLiftArmFwdBwd(NG_BUFFER_TRAY_PORT, CYL_FWD);
}


void CFormTeachNGTray::ClickGxbtnNgtrayinIoTopFwd()
{
	theUnitFunc.AlignCylFwdBwd(NG_TRAY_PORT, CYL_FWD);
}


void CFormTeachNGTray::ClickGxbtnNgtrayinIoTopBwd()
{
	theUnitFunc.AlignCylFwdBwd(NG_TRAY_PORT, CYL_BWD);
}


void CFormTeachNGTray::ClickGxbtnNgtrayinIoBtmFwd()
{
	theUnitFunc.DivisionCylFwdBwd(NG_TRAY_PORT, CYL_FWD);
}


void CFormTeachNGTray::ClickGxbtnNgtrayinIoBtmBwd()
{
	theUnitFunc.DivisionCylFwdBwd(NG_TRAY_PORT, CYL_BWD);
}


void CFormTeachNGTray::ClickGxbtnNgtrayinIoAlignguideFwd()
{
	theUnitFunc.BottomAlignCylFwdBwd(NG_TRAY_PORT, CYL_FWD);
}


void CFormTeachNGTray::ClickGxbtnNgtrayinIoAlignguideBwd()
{
	theUnitFunc.BottomAlignCylFwdBwd(NG_TRAY_PORT, CYL_BWD);
}

void CFormTeachNGTray::ClickGxbtnNgtraytrVacon()
{
	theUnitFunc.NGTrayTr_VacOnOff(VAC_ON);
}


void CFormTeachNGTray::ClickGxbtnNgtraytrVacoff()
{
	theUnitFunc.NGTrayTr_VacOnOff(VAC_OFF);
	Sleep(200);
	theUnitFunc.NGTrayTr_BlowOff();
}

void CFormTeachNGTray::ClickGxbtnNgtraytrGetpos()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_NG_TRAY_TR_X;
	m_CurTeach = TEACH_PARAM::NG_TRAY_TR_X_TO_IN;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_NGTRAYTR_GETPOS);
}


void CFormTeachNGTray::ClickGxbtnNgtraytrPutpos()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_NG_TRAY_TR_X;
	m_CurTeach = TEACH_PARAM::NG_TRAY_TR_X_TO_OUT;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_NGTRAYTR_PUTPOS);
}


void CFormTeachNGTray::ClickGxbtnNgtraytrUppos()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_NG_TRAY_TR_Z;
	m_CurTeach = TEACH_PARAM::NG_TRAY_TR_Z_TO_UP;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_NGTRAYTR_UPPOS);
}


void CFormTeachNGTray::ClickGxbtnNgtraytrGetDownpos()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_NG_TRAY_TR_Z;
	m_CurTeach = TEACH_PARAM::NG_TRAY_TR_Z_TO_IN;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_NGTRAYTR_GETDOWNPOS);
}


void CFormTeachNGTray::ClickGxbtnNgtraytrPutDownpos()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_NG_TRAY_TR_Z;
	m_CurTeach = TEACH_PARAM::NG_TRAY_TR_Z_TO_OUT;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_NGTRAYTR_PUTDOWNPOS);
}


void CFormTeachNGTray::ClickGxbtnNgtrayinTrayUp()
{
	theUnitFunc.NGTrayUpDown(NG_TRAY_PORT, CYL_UP);
}


void CFormTeachNGTray::ClickGxbtnNgtrayinTrayDown()
{
	theUnitFunc.NGTrayUpDown(NG_TRAY_PORT, CYL_DOWN);
}


void CFormTeachNGTray::ClickGxbtnNgtrayoutTrayUp()
{
	theUnitFunc.NGTrayUpDown(NG_BUFFER_TRAY_PORT, CYL_UP);
}


void CFormTeachNGTray::ClickGxbtnNgtrayoutTrayDown()
{
	theUnitFunc.NGTrayUpDown(NG_BUFFER_TRAY_PORT, CYL_DOWN);
}


void CFormTeachNGTray::ClickGxbtnNgbuftrayConvCylup()
{
	theUnitFunc.BottomStoperUpDown(NG_TRAY_PORT, CYL_UP);
}


void CFormTeachNGTray::ClickGxbtnNgbuftrayConvCyldown()
{
	theUnitFunc.BottomStoperUpDown(NG_TRAY_PORT, CYL_DOWN);
}


void CFormTeachNGTray::ClickGxbtnNgtrayAlignStart()
{
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_NGTRAY_ALIGN_OK, GXCOLOR_OFF);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_NGTRAY_ALIGN_OK, _T(""));

	theProcBank.m_strNGTray_AlignOK = _T("");

	theUnitFunc.TrayAlignStart(_T("NGTRAY"));
	SetTimer(2,2000,NULL);
}


void CFormTeachNGTray::ClickGxbtnNgtrayAlignTeaching()
{
	CGxUICtrl::SetStaticColor(this, IDC_GXSTC_NGTRAY_TEACHING_OK, GXCOLOR_OFF);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_NGTRAY_TEACHING_OK, _T(""));

	theProcBank.m_strNGTray_TeachingOK = _T("");

	theUnitFunc.TrayAlignTeaching(_T("NGTRAY"));
	SetTimer(3,2000,NULL);
}


void CFormTeachNGTray::ClickGxbtnNgtrayinMidPos()
{
	// NG TRAY IN LIFT MID POS 동작 UI 추가 2017/05/01 HSK
	m_CurAxis = (AXIS_ID)AXIS_SVO_NG_TRAY_IN_LIFT_Z;
	m_CurTeach = TEACH_PARAM::NG_TRAY_IN_LIFT_Z_TO_MID;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_NGTRAYIN_MID_POS);
}
