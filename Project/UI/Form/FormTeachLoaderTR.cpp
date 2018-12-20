// FormTeachLoaderTR.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "resource.h"
#include "FormTeachLoaderTR.h"


#include "GUIDefine.h"
//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "UI\GausGUI\GxButtonEx.h"
#include "UI\GausGUI\GxStaticEx.h"

// CFormTeachLoaderTR

IMPLEMENT_DYNCREATE(CFormTeachLoaderTR, CFormView)

CFormTeachLoaderTR::CFormTeachLoaderTR()
	: CFormView(CFormTeachLoaderTR::IDD)
{
	m_bLowerLiveMode = FALSE;
	m_bUpperLiveMode = FALSE;
	m_bInit = FALSE;

}

CFormTeachLoaderTR::~CFormTeachLoaderTR()
{
}

void CFormTeachLoaderTR::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GXSTC_LDTR_MCR_MCR1_VIEW, m_Lower_imgLive);
	DDX_Control(pDX, IDC_GXSTC_LDTR_MCR_MCR2_VIEW, m_Upper_imgLive);
}

BEGIN_MESSAGE_MAP(CFormTeachLoaderTR, CFormView)
	ON_WM_TIMER()
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)			// 표시 언어가 바뀌었다.

	ON_MESSAGE(WM_UPDATE_LOWER_MCRDATA,		OnUpdateLOWERMCRData)			// MCR이 수신되었다.
	ON_MESSAGE(WM_UPDATE_LOWER_MCRLIVE,		OnUpdateLOWERMCRLive)			// Live 화면이이 수신되었다.
	ON_MESSAGE(WM_UPDATE_UPPER_MCRDATA,		OnUpdateUPPERMCRData)			// MCR이 수신되었다.
	ON_MESSAGE(WM_UPDATE_UPPER_MCRLIVE,		OnUpdateUPPERMCRLive)			// Live 화면이이 수신되었다.
END_MESSAGE_MAP()


BEGIN_EVENTSINK_MAP(CFormTeachLoaderTR, CFormView)
	ON_EVENT(CFormTeachLoaderTR, IDC_GXBTN_LOWERMCR_LIVE, DISPID_CLICK, CFormTeachLoaderTR::ClickGxbtnLowermcrLive, VTS_NONE)
	ON_EVENT(CFormTeachLoaderTR, IDC_GXBTN_UPPERMCR_LIVE, DISPID_CLICK, CFormTeachLoaderTR::ClickGxbtnUppermcrLive, VTS_NONE)
	ON_EVENT(CFormTeachLoaderTR, IDC_GXBTN_LOWERMCR_READ, DISPID_CLICK, CFormTeachLoaderTR::ClickGxbtnLowermcrRead, VTS_NONE)
	ON_EVENT(CFormTeachLoaderTR, IDC_GXBTN_UPPERMCR_READ, DISPID_CLICK, CFormTeachLoaderTR::ClickGxbtnUppermcrRead, VTS_NONE)
	ON_EVENT(CFormTeachLoaderTR, IDC_GXBTN_LDTR_X_GET, DISPID_CLICK, CFormTeachLoaderTR::ClickGxbtnLdtrXGet, VTS_NONE)
	ON_EVENT(CFormTeachLoaderTR, IDC_GXBTN_LDTR_X_PUT, DISPID_CLICK, CFormTeachLoaderTR::ClickGxbtnLdtrXPut, VTS_NONE)
	ON_EVENT(CFormTeachLoaderTR, IDC_GXBTN_LDTR_X_LOWERMCR_CH1, DISPID_CLICK, CFormTeachLoaderTR::ClickGxbtnLdtrXLowermcrCh1, VTS_NONE)
	ON_EVENT(CFormTeachLoaderTR, IDC_GXBTN_LDTR_X_LOWERMCR_CH2, DISPID_CLICK, CFormTeachLoaderTR::ClickGxbtnLdtrXLowermcrCh2, VTS_NONE)
	ON_EVENT(CFormTeachLoaderTR, IDC_GXBTN_LDTR_X_UPPERMCR_CH1, DISPID_CLICK, CFormTeachLoaderTR::ClickGxbtnLdtrXUppermcrCh1, VTS_NONE)
	ON_EVENT(CFormTeachLoaderTR, IDC_GXBTN_LDTR_X_UPPERMCR_CH2, DISPID_CLICK, CFormTeachLoaderTR::ClickGxbtnLdtrXUppermcrCh2, VTS_NONE)
	ON_EVENT(CFormTeachLoaderTR, IDC_GXBTN_LDTR_Z_GET, DISPID_CLICK, CFormTeachLoaderTR::ClickGxbtnLdtrZGet, VTS_NONE)
	ON_EVENT(CFormTeachLoaderTR, IDC_GXBTN_LDTR_Z_PUT, DISPID_CLICK, CFormTeachLoaderTR::ClickGxbtnLdtrZPut, VTS_NONE)
	ON_EVENT(CFormTeachLoaderTR, IDC_GXBTN_LDTR_Z_LOWER, DISPID_CLICK, CFormTeachLoaderTR::ClickGxbtnLdtrZLower, VTS_NONE)
	ON_EVENT(CFormTeachLoaderTR, IDC_GXBTN_LDTR_Z_UPPER, DISPID_CLICK, CFormTeachLoaderTR::ClickGxbtnLdtrZUpper, VTS_NONE)
	ON_EVENT(CFormTeachLoaderTR, IDC_GXBTN_LDTR_VACUUM_CH1_ON, DISPID_CLICK, CFormTeachLoaderTR::ClickGxbtnLdtrVacuumCh1On, VTS_NONE)
	ON_EVENT(CFormTeachLoaderTR, IDC_GXBTN_LDTR_VACUUM_CH2_ON, DISPID_CLICK, CFormTeachLoaderTR::ClickGxbtnLdtrVacuumCh2On, VTS_NONE)
	ON_EVENT(CFormTeachLoaderTR, IDC_GXBTN_LDTR_VACUUM_CH1_OFF, DISPID_CLICK, CFormTeachLoaderTR::ClickGxbtnLdtrVacuumCh1Off, VTS_NONE)
	ON_EVENT(CFormTeachLoaderTR, IDC_GXBTN_LDTR_VACUUM_CH2_OFF, DISPID_CLICK, CFormTeachLoaderTR::ClickGxbtnLdtrVacuumCh2Off, VTS_NONE)
	ON_EVENT(CFormTeachLoaderTR, IDC_GXBTN_LDTR_Z_UP, DISPID_CLICK, CFormTeachLoaderTR::ClickGxbtnLdtrZUp, VTS_NONE)
	ON_EVENT(CFormTeachLoaderTR, IDC_GXBTN_LDTR_FPCB_BLOW_ON, DISPID_CLICK, CFormTeachLoaderTR::ClickGxbtnLdtrFpcbBlowOn, VTS_NONE)
	ON_EVENT(CFormTeachLoaderTR, IDC_GXBTN_LDTR_FPCB_BLOW_OFF, DISPID_CLICK, CFormTeachLoaderTR::ClickGxbtnLdtrFpcbBlowOff, VTS_NONE)
	ON_EVENT(CFormTeachLoaderTR, IDC_GXBTN_LOWERMCR_RECONNECT, DISPID_CLICK, CFormTeachLoaderTR::ClickGxbtnLowermcrReconnect, VTS_NONE)
	ON_EVENT(CFormTeachLoaderTR, IDC_GXBTN_UPPERMCR_RECONNECT, DISPID_CLICK, CFormTeachLoaderTR::ClickGxbtnUppermcrReconnect, VTS_NONE)
END_EVENTSINK_MAP()

// CFormTeachLoaderTR 진단입니다.

#ifdef _DEBUG
void CFormTeachLoaderTR::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormTeachLoaderTR::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormTeachLoaderTR 메시지 처리기입니다.


BOOL CFormTeachLoaderTR::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style = WS_OVERLAPPED | WS_CHILD;				// 테두리를 없앤다.

	return CFormView::PreCreateWindow(cs);
}


void CFormTeachLoaderTR::OnInitialUpdate()
{
	if(m_bInit)	return;	// OnInitialUpdate 두번 실행되는 버그 방지 [10/24/2016 OSC]
	m_bInit = TRUE;
	CFormView::OnInitialUpdate();

	m_CurAxis = (AXIS_ID)AXIS_SVO_LD_TR_X;
	m_CurTeach = TEACH_PARAM::LD_TR_X_GET;
	m_CurID	  = IDC_GXBTN_LDTR_X_GET;
}

LRESULT CFormTeachLoaderTR::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{
		m_Lower_imgLive.SetStretchMode(TRUE);
		m_Lower_imgLive.LoadImage(LOWER_MCRPath);
		m_Lower_imgLive.Invalidate();	
		m_Upper_imgLive.SetStretchMode(TRUE);
		m_Upper_imgLive.LoadImage(UPPER_MCRPath);
		m_Upper_imgLive.Invalidate();	
		UpdateLDTRState();
		SetTimer(1, 100, NULL);
	}
	else
	{
		KillTimer(1);
	}
	//kjpark 20161025 MCR 구현
	m_bLowerShow = bFlag;
	m_bUpperShow = bFlag;
	return 0;
}

void CFormTeachLoaderTR::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
	case 1:
		UpdateLDTRState();
		break;
	}

	CFormView::OnTimer(nIDEvent);
}

LRESULT CFormTeachLoaderTR::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	CGxUICtrl::BroadcastStaticCpationIndex(this, wParam, IDC_GXSTC_LDTR_LDTR);
	CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_GXBTN_LDTR_X_GET);

	return 0;
}

void CFormTeachLoaderTR::SetCurPageAxis(AXIS_ID axis, TEACH_PARAM::ID param, UINT nID)
{
	m_CurAxis = axis;
	m_CurTeach = param;
	m_CurID	  = nID;	

	GetMainHandler()->SetSelectedAxis(m_CurAxis, m_CurTeach);
}


void CFormTeachLoaderTR::UpdateLDTRState()
{
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDTR_X_GET,theUnitFunc.LDTRXGetPosChk()? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDTR_X_PUT,theUnitFunc.LDTRXPutPosChk()? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDTR_X_LOWERMCR_CH1,theUnitFunc.LDTR_X_LOWERMCRPosChk(JIG_CH_1)? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDTR_X_LOWERMCR_CH2,theUnitFunc.LDTR_X_LOWERMCRPosChk(JIG_CH_2)? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDTR_X_UPPERMCR_CH1,theUnitFunc.LDTR_X_UPPERMCRPosChk(JIG_CH_1)? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDTR_X_UPPERMCR_CH2,theUnitFunc.LDTR_X_UPPERMCRPosChk(JIG_CH_2)? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDTR_Z_GET,theUnitFunc.LDTRZGetPosChk()? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDTR_Z_PUT,theUnitFunc.LDTRZPutPosChk()? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDTR_Z_LOWER,theUnitFunc.LDTR_Z_LOWERMCRPosChk()? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDTR_Z_UPPER,theUnitFunc.LDTR_Z_UPPERMCRPosChk()? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDTR_Z_UP,theUnitFunc.LDTRZUpPosChk()? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDTR_VACUUM_CH1_ON,theUnitFunc.GetInPutIOCheck(X_LD_TR_CH1_VAC_SEN)? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDTR_VACUUM_CH1_OFF,!theUnitFunc.GetInPutIOCheck(X_LD_TR_CH1_VAC_SEN)? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDTR_VACUUM_CH2_ON,theUnitFunc.GetInPutIOCheck(X_LD_TR_CH2_VAC_SEN)? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDTR_VACUUM_CH2_OFF,!theUnitFunc.GetInPutIOCheck(X_LD_TR_CH2_VAC_SEN)? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDTR_FPCB_BLOW_ON,theUnitFunc.GetOutPutIOCheck(Y_LD_TR_CH1_FPCB_BLOW_SOL) && theUnitFunc.GetOutPutIOCheck(Y_LD_TR_CH2_FPCB_BLOW_SOL) ? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LDTR_FPCB_BLOW_OFF,theUnitFunc.GetOutPutIOCheck(Y_LD_TR_CH1_FPCB_BLOW_SOL) && theUnitFunc.GetOutPutIOCheck(Y_LD_TR_CH2_FPCB_BLOW_SOL) ? GXCOLOR_OFF:GXCOLOR_ON);

}

void CFormTeachLoaderTR::SetMCRLive(MCR_UNIT Unit)
{

	// 취득한 이미지를 표시한다.
	if (Unit == LOWER_MCR)
	{
		if ( m_bLowerShow == FALSE) return ;

		if(CFileSupport::FileCheck(LOWER_MCRPath)  && m_bLowerShow)
		{
			m_Lower_imgLive.SetStretchMode(TRUE);
			m_Lower_imgLive.LoadImage( LOWER_MCRPath );
			m_Lower_imgLive.Invalidate();
		}
	}
	else
	{
		if ( m_bUpperShow == FALSE) return ;
		if(CFileSupport::FileCheck(UPPER_MCRPath)  && m_bUpperShow)
		{
			m_Upper_imgLive.SetStretchMode(TRUE);
			m_Upper_imgLive.LoadImage( UPPER_MCRPath );
			m_Upper_imgLive.Invalidate();
		}
	}
}

//kjpark 20161025 MCR 구현
void CFormTeachLoaderTR::SetMCRData(MCR_UNIT Unit,CString strBarcode)
{
	if (Unit == LOWER_MCR)
	{
		if(m_bLowerShow == FALSE)
			return;

		CGxUICtrl::SetStaticString(this, IDC_GXSTC_LOWER_MCR_DATA, strBarcode);

		CString sFile = theUnitFunc.GetLastSavedImage(LOWER_MCR);
		if(CFileSupport::FileCheck(sFile)  && m_bLowerShow )
		{
			m_Lower_imgLive.SetStretchMode(TRUE);
			m_Lower_imgLive.LoadImage( sFile );
			m_Lower_imgLive.Invalidate();
		}
	}
	else
	{
		if(m_bUpperShow == FALSE)
			return;

		CGxUICtrl::SetStaticString(this, IDC_GXSTC_UPPER_MCR_DATA, strBarcode);

		CString sFile = theUnitFunc.GetLastSavedImage(UPPER_MCR);
		if(CFileSupport::FileCheck(sFile)  && m_bUpperShow )
		{
			m_Upper_imgLive.SetStretchMode(TRUE);
			m_Upper_imgLive.LoadImage( sFile );
			m_Upper_imgLive.Invalidate();
		}
	}

}

LRESULT CFormTeachLoaderTR::OnUpdateLOWERMCRData(WPARAM wParam, LPARAM lParam)
{
	SetMCRData(LOWER_MCR,theUnitFunc.GetSoftTriggerData(LOWER_MCR));

	return 0;
}

// MCR Live 영상 수신
//kjpark 20161025 MCR 구현
LRESULT CFormTeachLoaderTR::OnUpdateLOWERMCRLive(WPARAM wParam, LPARAM lParam)
{
	SetMCRLive(LOWER_MCR);

	return 0;
}


LRESULT CFormTeachLoaderTR::OnUpdateUPPERMCRData(WPARAM wParam, LPARAM lParam)
{
	SetMCRData(UPPER_MCR,theUnitFunc.GetSoftTriggerData(UPPER_MCR));

	return 0;
}

// MCR Live 영상 수신
//kjpark 20161025 MCR 구현
LRESULT CFormTeachLoaderTR::OnUpdateUPPERMCRLive(WPARAM wParam, LPARAM lParam)
{
	SetMCRLive(UPPER_MCR);

	return 0;
}

void CFormTeachLoaderTR::ClickGxbtnLowermcrLive()
{
	if ( m_bLowerShow == FALSE) return ;

	m_bLowerLiveMode = !m_bLowerLiveMode;
	theUnitFunc.SetLiveMode(m_bLowerLiveMode,LOWER_MCR);
	if(m_bLowerLiveMode)
	{
		CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_LOWERMCR_READ, FALSE);
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LOWERMCR_LIVE, GXCOLOR_ON);

	}
	else
	{
		CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_LOWERMCR_READ, TRUE);
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LOWERMCR_LIVE, GXCOLOR_OFF);
	}
}

void CFormTeachLoaderTR::ClickGxbtnLowermcrRead()
{
	theUnitFunc.Trigger(LOWER_MCR);	
}

void CFormTeachLoaderTR::ClickGxbtnUppermcrLive()
{
	if ( m_bUpperShow == FALSE) return ;

	m_bUpperLiveMode = !m_bUpperLiveMode;
	theUnitFunc.SetLiveMode(m_bUpperLiveMode,UPPER_MCR);
	if(m_bUpperLiveMode)
	{
		CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_UPPERMCR_READ, FALSE);
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UPPERMCR_LIVE, GXCOLOR_ON);

	}
	else
	{
		CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_UPPERMCR_READ, TRUE);
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_UPPERMCR_LIVE, GXCOLOR_OFF);
	}
}

void CFormTeachLoaderTR::ClickGxbtnUppermcrRead()
{
	theUnitFunc.Trigger(UPPER_MCR);
}


void CFormTeachLoaderTR::ClickGxbtnLdtrXGet()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_LD_TR_X;
	m_CurTeach = TEACH_PARAM::LD_TR_X_GET;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_LDTR_X_GET);
}


void CFormTeachLoaderTR::ClickGxbtnLdtrXPut()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_LD_TR_X;
	m_CurTeach = TEACH_PARAM::LD_TR_X_PUT;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_LDTR_X_PUT);
}


void CFormTeachLoaderTR::ClickGxbtnLdtrXLowermcrCh1()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_LD_TR_X;
	m_CurTeach = TEACH_PARAM::LD_TR_X_CH1_LOWER_MCR;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_LDTR_X_LOWERMCR_CH1);
	
	//2017-05-23,skcho ,채널 구분
	theProcBank.m_nMCRChannel = JIG_CH_1;
}


void CFormTeachLoaderTR::ClickGxbtnLdtrXLowermcrCh2()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_LD_TR_X;
	m_CurTeach = TEACH_PARAM::LD_TR_X_CH2_LOWER_MCR;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_LDTR_X_LOWERMCR_CH2);

	//2017-05-23,skcho ,채널 구분
	theProcBank.m_nMCRChannel = JIG_CH_2;
}


void CFormTeachLoaderTR::ClickGxbtnLdtrXUppermcrCh1()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_LD_TR_X;
	m_CurTeach = TEACH_PARAM::LD_TR_X_CH1_UPPER_MCR;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_LDTR_X_UPPERMCR_CH1);

	//2017-05-23,skcho ,채널 구분
	theProcBank.m_nMCRChannel = JIG_CH_1;
}


void CFormTeachLoaderTR::ClickGxbtnLdtrXUppermcrCh2()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_LD_TR_X;
	m_CurTeach = TEACH_PARAM::LD_TR_X_CH2_UPPER_MCR;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_LDTR_X_UPPERMCR_CH2);

	//2017-05-23,skcho ,채널 구분
	theProcBank.m_nMCRChannel = JIG_CH_2;
}


void CFormTeachLoaderTR::ClickGxbtnLdtrZGet()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_LD_TR_Z;
	m_CurTeach = TEACH_PARAM::LD_TR_Z_GET;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_LDTR_Z_GET);
}


void CFormTeachLoaderTR::ClickGxbtnLdtrZPut()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_LD_TR_Z;
	m_CurTeach = TEACH_PARAM::LD_TR_Z_PUT;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_LDTR_Z_PUT);
}

void CFormTeachLoaderTR::ClickGxbtnLdtrZUp()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_LD_TR_Z;
	m_CurTeach = TEACH_PARAM::LD_TR_Z_UP;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_LDTR_Z_UP);
}

void CFormTeachLoaderTR::ClickGxbtnLdtrZLower()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_LD_TR_Z;
	m_CurTeach = TEACH_PARAM::LD_TR_Z_LOWER_MCR;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_LDTR_Z_LOWER);
}


void CFormTeachLoaderTR::ClickGxbtnLdtrZUpper()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_LD_TR_Z;
	m_CurTeach = TEACH_PARAM::LD_TR_Z_UPPER_MCR;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_LDTR_Z_UPPER);

}


void CFormTeachLoaderTR::ClickGxbtnLdtrVacuumCh1On()
{
	theUnitFunc.LDTRVacOn(VAC_ON,JIG_CH_1);
}


void CFormTeachLoaderTR::ClickGxbtnLdtrVacuumCh2On()
{
	theUnitFunc.LDTRVacOn(VAC_ON,JIG_CH_2);
}


void CFormTeachLoaderTR::ClickGxbtnLdtrVacuumCh1Off()
{
	theUnitFunc.LDTRVacOn(VAC_OFF,JIG_CH_1);
	Sleep(200);
	theUnitFunc.LDTR_BlowOff(JIG_CH_1);
}


void CFormTeachLoaderTR::ClickGxbtnLdtrVacuumCh2Off()
{
	theUnitFunc.LDTRVacOn(VAC_OFF,JIG_CH_2);
	Sleep(200);
	theUnitFunc.LDTR_BlowOff(JIG_CH_2);
}



void CFormTeachLoaderTR::ClickGxbtnLdtrFpcbBlowOn()
{
	theUnitFunc.LDTRFPCBBlowOn(BLOW_ON,TRUE,TRUE);
}


void CFormTeachLoaderTR::ClickGxbtnLdtrFpcbBlowOff()
{
	theUnitFunc.LDTRFPCBBlowOn(BLOW_OFF,TRUE,TRUE);
}

// MCR 재연결 버튼 추가. 17/04/25 HSK
void CFormTeachLoaderTR::ClickGxbtnLowermcrReconnect()
{
	if(GetMainHandler()->m_pIDReader[LOWER_MCR]->IsConnect())
	{
		GetMainHandler()->m_pIDReader[LOWER_MCR]->DisConnect();
		Sleep(500);
	}
	GetMainHandler()->m_pIDReader[LOWER_MCR]->Connect();
	
	theProcBank.ManualMCRReConnect = TRUE;
}


void CFormTeachLoaderTR::ClickGxbtnUppermcrReconnect()
{
	if(GetMainHandler()->m_pIDReader[UPPER_MCR]->IsConnect())
	{
		GetMainHandler()->m_pIDReader[UPPER_MCR]->DisConnect();
		Sleep(500);
	}
	GetMainHandler()->m_pIDReader[UPPER_MCR]->Connect();

	theProcBank.ManualMCRReConnect = TRUE;
}
