// FormTeachPreAlign.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "resource.h"
#include "FormTeachPreAlign.h"
#include "GUIDefine.h"

//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "UI\GausGUI\GxButtonEx.h"
#include "UI\GausGUI\GxStaticEx.h"

// CFormTeachPreAlign

IMPLEMENT_DYNCREATE(CFormTeachPreAlign, CFormView)

CFormTeachPreAlign::CFormTeachPreAlign()
	: CFormView(CFormTeachPreAlign::IDD)
{
	m_bInit = FALSE;
	m_Jig = JIG_ID_A;
	m_nSeq = seqC_idle;
	m_nTeachingTablePos = CONST_TABLE_POS::POS_1;
}

CFormTeachPreAlign::~CFormTeachPreAlign()
{
}

void CFormTeachPreAlign::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT1, m_edtMsg);
}

BEGIN_MESSAGE_MAP(CFormTeachPreAlign, CFormView)
	ON_WM_TIMER()
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)			// 표시 언어가 바뀌었다.
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CFormTeachPreAlign, CFormView)
	ON_EVENT(CFormTeachPreAlign, IDC_GXBTN_LD_PREALIGNTR_Y_GET, DISPID_CLICK, CFormTeachPreAlign::ClickGxbtnLdPrealigntrYGet, VTS_NONE)
	ON_EVENT(CFormTeachPreAlign, IDC_GXBTN_LD_PREALIGNTR_Y_PUT, DISPID_CLICK, CFormTeachPreAlign::ClickGxbtnLdPrealigntrYPut, VTS_NONE)
	ON_EVENT(CFormTeachPreAlign, IDC_GXBTN_LD_PREALIGNTR_Y_WAIT, DISPID_CLICK, CFormTeachPreAlign::ClickGxbtnLdPrealigntrYWait, VTS_NONE)
	ON_EVENT(CFormTeachPreAlign, IDC_GXBTN_LD_PREALIGNTR_Z_GETSHUTTLE1, DISPID_CLICK, CFormTeachPreAlign::ClickGxbtnLdPrealigntrZGetshuttle1, VTS_NONE)
	ON_EVENT(CFormTeachPreAlign, IDC_GXBTN_LD_PREALIGNTR_Z_GETSHUTTLE2, DISPID_CLICK, CFormTeachPreAlign::ClickGxbtnLdPrealigntrZGetshuttle2, VTS_NONE)
	ON_EVENT(CFormTeachPreAlign, IDC_GXBTN_LD_PREALIGNTR_Z_PUT, DISPID_CLICK, CFormTeachPreAlign::ClickGxbtnLdPrealigntrZPut, VTS_NONE)
	ON_EVENT(CFormTeachPreAlign, IDC_GXBTN_LD_PREALIGNTR_Z_UP, DISPID_CLICK, CFormTeachPreAlign::ClickGxbtnLdPrealigntrZUp, VTS_NONE)
	ON_EVENT(CFormTeachPreAlign, IDC_GXBTN_LD_PREALIGN_TR_VAC_CH1_ON, DISPID_CLICK, CFormTeachPreAlign::ClickGxbtnLdPrealignTrVacCh1On, VTS_NONE)
	ON_EVENT(CFormTeachPreAlign, IDC_GXBTN_LD_PREALIGN_TR_VAC_CH1_OFF, DISPID_CLICK, CFormTeachPreAlign::ClickGxbtnLdPrealignTrVacCh1Off, VTS_NONE)
	ON_EVENT(CFormTeachPreAlign, IDC_GXBTN_LD_PREALIGN_TR_VAC_CH2_ON, DISPID_CLICK, CFormTeachPreAlign::ClickGxbtnLdPrealignTrVacCh2On, VTS_NONE)
	ON_EVENT(CFormTeachPreAlign, IDC_GXBTN_LD_PREALIGN_TR_VAC_CH2_OFF, DISPID_CLICK, CFormTeachPreAlign::ClickGxbtnLdPrealignTrVacCh2Off, VTS_NONE)
	ON_EVENT(CFormTeachPreAlign, IDC_GXBTN_PRE_ALIGN_X_CH1, DISPID_CLICK, CFormTeachPreAlign::ClickGxbtnPreAlignXCh1, VTS_NONE)
	ON_EVENT(CFormTeachPreAlign, IDC_GXBTN_PRE_ALIGN_X_CH2, DISPID_CLICK, CFormTeachPreAlign::ClickGxbtnPreAlignXCh2, VTS_NONE)
	ON_EVENT(CFormTeachPreAlign, IDC_GXBTN_PRE_ALIGN_Y_CH1, DISPID_CLICK, CFormTeachPreAlign::ClickGxbtnPreAlignYCh1, VTS_NONE)
	ON_EVENT(CFormTeachPreAlign, IDC_GXBTN_PRE_ALIGN_Y_CH2, DISPID_CLICK, CFormTeachPreAlign::ClickGxbtnPreAlignYCh2, VTS_NONE)
	ON_EVENT(CFormTeachPreAlign, IDC_GXBTN_PRE_ALIGN_T_CH1, DISPID_CLICK, CFormTeachPreAlign::ClickGxbtnPreAlignTCh1, VTS_NONE)
	ON_EVENT(CFormTeachPreAlign, IDC_GXBTN_PRE_ALIGN_T_CH2, DISPID_CLICK, CFormTeachPreAlign::ClickGxbtnPreAlignTCh2, VTS_NONE)
	ON_EVENT(CFormTeachPreAlign, IDC_GXBTN_PRE_STG_LOAD_POS, DISPID_CLICK, CFormTeachPreAlign::ClickGxbtnPreStgLoadPos, VTS_NONE)
	ON_EVENT(CFormTeachPreAlign, IDC_GXBTN_PRE_STG_UNLOAD_POS, DISPID_CLICK, CFormTeachPreAlign::ClickGxbtnPreStgUnloadPos, VTS_NONE)
	ON_EVENT(CFormTeachPreAlign, IDC_GXBTN_PRE_STG_CH1_VAC_ON, DISPID_CLICK, CFormTeachPreAlign::ClickGxbtnPreStgCh1VacOn, VTS_NONE)
	ON_EVENT(CFormTeachPreAlign, IDC_GXBTN_PRE_STG_CH2_VAC_ON, DISPID_CLICK, CFormTeachPreAlign::ClickGxbtnPreStgCh2VacOn, VTS_NONE)
	ON_EVENT(CFormTeachPreAlign, IDC_GXBTN_PRE_STG_CH1_VAC_OFF, DISPID_CLICK, CFormTeachPreAlign::ClickGxbtnPreStgCh1VacOff, VTS_NONE)
	ON_EVENT(CFormTeachPreAlign, IDC_GXBTN_PRE_STG_CH2_VAC_OFF, DISPID_CLICK, CFormTeachPreAlign::ClickGxbtnPreStgCh2VacOff, VTS_NONE)
	ON_EVENT(CFormTeachPreAlign, IDC_GXBTN_PRE_AUTO_TEACH, DISPID_CLICK, CFormTeachPreAlign::ClickGxbtnPreAutoTeach, VTS_NONE)
	ON_EVENT(CFormTeachPreAlign, IDC_GXBTN_PRE_STG_ALIGN_POS, DISPID_CLICK, CFormTeachPreAlign::ClickGxbtnPreStgAlignPos, VTS_NONE)
	ON_EVENT(CFormTeachPreAlign, IDC_GXBTN_PRE_ALIGN_MANUUAL_CH1, DISPID_CLICK, CFormTeachPreAlign::ClickGxbtnPreAlignManuualCh1, VTS_NONE)
	ON_EVENT(CFormTeachPreAlign, IDC_GXBTN_PRE_ALIGN_MANUUAL_CH2, DISPID_CLICK, CFormTeachPreAlign::ClickGxbtnPreAlignManuualCh2, VTS_NONE)
END_EVENTSINK_MAP()
// CFormTeachPreAlign 진단입니다.

#ifdef _DEBUG
void CFormTeachPreAlign::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormTeachPreAlign::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormTeachPreAlign 메시지 처리기입니다.


BOOL CFormTeachPreAlign::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style = WS_OVERLAPPED | WS_CHILD;				// 테두리를 없앤다.

	return CFormView::PreCreateWindow(cs);
}


void CFormTeachPreAlign::OnInitialUpdate()
{
	if(m_bInit)	return;	// OnInitialUpdate 두번 실행되는 버그 방지 [10/24/2016 OSC]
	m_bInit = TRUE;
	CFormView::OnInitialUpdate();

	m_CurAxis = (AXIS_ID)AXIS_SVO_PREALIGN_TR_Y;
	m_CurTeach = TEACH_PARAM::PA_TR_Y_GET;
	m_CurID	  = IDC_GXBTN_LD_PREALIGNTR_Y_GET;
}

LRESULT CFormTeachPreAlign::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{
		UpdatePAState();
		SetTimer(1, 100, NULL);
	}
	else
	{
		KillTimer(1);
	}
	return 0;
}

void CFormTeachPreAlign::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
	case 1:
		UpdatePAState();
		break;
	case 2:
		CGxButtonEx* pBtnX = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_PRE_AUTO_TEACH);

		KillTimer(2);
		SeqContactAutoTeaching();
		if(m_nSeq != seqC_idle)
			SetTimer(2, 50, NULL);
		else
			pBtnX->SetCaption(_T("Auto Teach"));
		break;
	}

	CFormView::OnTimer(nIDEvent);
}

LRESULT CFormTeachPreAlign::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	CGxUICtrl::BroadcastStaticCpationIndex(this, wParam, IDC_GXSTC_LD_PREALIGNTR);
	CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_GXBTN_LD_PREALIGNTR_Y_GET);

	return 0;
}

void CFormTeachPreAlign::SetCurPageAxis(AXIS_ID axis, TEACH_PARAM::ID param, UINT nID)
{
	m_CurAxis = axis;
	m_CurTeach = param;
	m_CurID	  = nID;	

	GetMainHandler()->SetSelectedAxis(m_CurAxis, m_CurTeach);
}

void CFormTeachPreAlign::UpdatePAState()
{
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LD_PREALIGNTR_Y_GET,theUnitFunc.Chk_PATR_YAxis_GetPos()? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LD_PREALIGNTR_Y_PUT,theUnitFunc.Chk_PATR_YAxis_PutPos()? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LD_PREALIGNTR_Y_WAIT,theUnitFunc.Chk_PATR_YAxis_WaitPos()? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LD_PREALIGNTR_Z_GETSHUTTLE1,theUnitFunc.Chk_PATR_ZAxis_GetUpperShuttlePos()? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LD_PREALIGNTR_Z_GETSHUTTLE2,theUnitFunc.Chk_PATR_ZAxis_GetLowerShuttlePos()? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LD_PREALIGNTR_Z_PUT,theUnitFunc.Chk_PATR_ZAxis_PutPos()? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LD_PREALIGNTR_Z_UP,theUnitFunc.Chk_PATR_ZAxis_UpPos()? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LD_PREALIGN_TR_VAC_CH1_ON,(theUnitFunc.GetInPutIOCheck(X_PA_TR_CH1_VAC_SEN)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LD_PREALIGN_TR_VAC_CH1_OFF,(!theUnitFunc.GetInPutIOCheck(X_PA_TR_CH1_VAC_SEN)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LD_PREALIGN_TR_VAC_CH2_ON,(theUnitFunc.GetInPutIOCheck(X_PA_TR_CH2_VAC_SEN)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_LD_PREALIGN_TR_VAC_CH2_OFF,(!theUnitFunc.GetInPutIOCheck(X_PA_TR_CH2_VAC_SEN)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_PRE_ALIGN_X_CH1,(theUnitFunc.PAXPosChk(JIG_CH_1)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_PRE_ALIGN_X_CH2,(theUnitFunc.PAXPosChk(JIG_CH_2)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_PRE_ALIGN_Y_CH1,(theUnitFunc.PAYPosChk(JIG_CH_1)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_PRE_ALIGN_Y_CH2,(theUnitFunc.PAYPosChk(JIG_CH_2)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_PRE_ALIGN_T_CH1,(theUnitFunc.PATPosChk(JIG_CH_1)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_PRE_ALIGN_T_CH2,(theUnitFunc.PATPosChk(JIG_CH_2)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_PRE_STG_LOAD_POS,(theUnitFunc.PAStageLoadPosChk()? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_PRE_STG_ALIGN_POS,(theUnitFunc.PAStageAlignPosChk()? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_PRE_STG_UNLOAD_POS,(theUnitFunc.PAStageUnloadPosChk()? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_PRE_STG_CH1_VAC_ON,(theUnitFunc.GetInPutIOCheck(X_PA_STG_CH1_VAC_SEN)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_PRE_STG_CH1_VAC_OFF,(!theUnitFunc.GetInPutIOCheck(X_PA_STG_CH1_VAC_SEN)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_PRE_STG_CH2_VAC_ON,(theUnitFunc.GetInPutIOCheck(X_PA_STG_CH2_VAC_SEN)? GXCOLOR_ON:GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_PRE_STG_CH2_VAC_OFF,(!theUnitFunc.GetInPutIOCheck(X_PA_STG_CH2_VAC_SEN)? GXCOLOR_ON:GXCOLOR_OFF));
}


void CFormTeachPreAlign::ClickGxbtnLdPrealigntrYGet()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_PREALIGN_TR_Y;
	m_CurTeach = TEACH_PARAM::PA_TR_Y_GET;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_LD_PREALIGNTR_Y_GET);
}


void CFormTeachPreAlign::ClickGxbtnLdPrealigntrYPut()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_PREALIGN_TR_Y;
	m_CurTeach = TEACH_PARAM::PA_TR_Y_PUT;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_LD_PREALIGNTR_Y_PUT);
}


void CFormTeachPreAlign::ClickGxbtnLdPrealigntrYWait()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_PREALIGN_TR_Y;
	m_CurTeach = TEACH_PARAM::PA_TR_Y_WAIT;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_LD_PREALIGNTR_Y_WAIT);
}


void CFormTeachPreAlign::ClickGxbtnLdPrealigntrZGetshuttle1()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_PREALIGN_TR_Z;
	m_CurTeach = TEACH_PARAM::PA_TR_Z_UPPER_GET;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_LD_PREALIGNTR_Z_GETSHUTTLE1);
}


void CFormTeachPreAlign::ClickGxbtnLdPrealigntrZGetshuttle2()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_PREALIGN_TR_Z;
	m_CurTeach = TEACH_PARAM::PA_TR_Z_LOWER_GET;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_LD_PREALIGNTR_Z_GETSHUTTLE2);
}


void CFormTeachPreAlign::ClickGxbtnLdPrealigntrZPut()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_PREALIGN_TR_Z;
	m_CurTeach = TEACH_PARAM::PA_TR_Z_PUT;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_LD_PREALIGNTR_Z_PUT);
}


void CFormTeachPreAlign::ClickGxbtnLdPrealigntrZUp()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_PREALIGN_TR_Z;
	m_CurTeach = TEACH_PARAM::PA_TR_Z_UP;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_LD_PREALIGNTR_Z_UP);
}


void CFormTeachPreAlign::ClickGxbtnLdPrealignTrVacCh1On()
{
	theUnitFunc.PATR_Vacuum_On(VAC_ON,JIG_CH_1);
}


void CFormTeachPreAlign::ClickGxbtnLdPrealignTrVacCh1Off()
{
	theUnitFunc.PATR_Vacuum_On(VAC_OFF,JIG_CH_1);
	Sleep(200);
	theUnitFunc.PATR_BlowOff(JIG_CH_1);
}


void CFormTeachPreAlign::ClickGxbtnLdPrealignTrVacCh2On()
{
	theUnitFunc.PATR_Vacuum_On(VAC_ON,JIG_CH_2);
}


void CFormTeachPreAlign::ClickGxbtnLdPrealignTrVacCh2Off()
{
	theUnitFunc.PATR_Vacuum_On(VAC_OFF,JIG_CH_2);
	Sleep(200);
	theUnitFunc.PATR_BlowOff(JIG_CH_2);
}

void CFormTeachPreAlign::ClickGxbtnPreAlignXCh1()
{
	m_CurAxis = (AXIS_ID)AXIS_STP_PA_CH1_X;
	m_CurTeach = TEACH_PARAM::PA_CH1_X;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_PRE_ALIGN_X_CH1);
}


void CFormTeachPreAlign::ClickGxbtnPreAlignXCh2()
{
	m_CurAxis = (AXIS_ID)AXIS_STP_PA_CH2_X;
	m_CurTeach = TEACH_PARAM::PA_CH2_X;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_PRE_ALIGN_X_CH2);
}


void CFormTeachPreAlign::ClickGxbtnPreAlignYCh1()
{
	m_CurAxis = (AXIS_ID)AXIS_STP_PA_CH1_Y;
	m_CurTeach = TEACH_PARAM::PA_CH1_Y;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_PRE_ALIGN_Y_CH1);
}


void CFormTeachPreAlign::ClickGxbtnPreAlignYCh2()
{
	m_CurAxis = (AXIS_ID)AXIS_STP_PA_CH2_Y;
	m_CurTeach = TEACH_PARAM::PA_CH2_Y;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_PRE_ALIGN_Y_CH2);
}


void CFormTeachPreAlign::ClickGxbtnPreAlignTCh1()
{
	m_CurAxis = (AXIS_ID)AXIS_STP_PA_CH1_T;
	m_CurTeach = TEACH_PARAM::PA_CH1_T;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_PRE_ALIGN_T_CH1);
}


void CFormTeachPreAlign::ClickGxbtnPreAlignTCh2()
{
	m_CurAxis = (AXIS_ID)AXIS_STP_PA_CH2_T;
	m_CurTeach = TEACH_PARAM::PA_CH2_T;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_PRE_ALIGN_T_CH2);
}


void CFormTeachPreAlign::ClickGxbtnPreStgLoadPos()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_PA_STAGE_X;
	m_CurTeach = TEACH_PARAM::PA_STAGE_LOAD_POS;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_PRE_STG_LOAD_POS);
}


void CFormTeachPreAlign::ClickGxbtnPreStgUnloadPos()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_PA_STAGE_X;
	m_CurTeach = TEACH_PARAM::PA_STAGE_UNLOAD_POS;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_PRE_STG_UNLOAD_POS);
}


void CFormTeachPreAlign::ClickGxbtnPreStgCh1VacOn()
{
	theUnitFunc.PAStageVac(VAC_ON,JIG_CH_1);
}


void CFormTeachPreAlign::ClickGxbtnPreStgCh2VacOn()
{
	theUnitFunc.PAStageVac(VAC_ON,JIG_CH_2);
}


void CFormTeachPreAlign::ClickGxbtnPreStgCh1VacOff()
{
	theUnitFunc.PAStageVac(VAC_OFF,JIG_CH_1);
	Sleep(200);
	theUnitFunc.PAStage_BlowOff(JIG_CH_1);
}


void CFormTeachPreAlign::ClickGxbtnPreStgCh2VacOff()
{
	theUnitFunc.PAStageVac(VAC_OFF,JIG_CH_2);
	Sleep(200);
	theUnitFunc.PAStage_BlowOff(JIG_CH_2);
}

void CFormTeachPreAlign::ClickGxbtnPreAutoTeach()
{
	CGxButtonEx* pBtnX = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_PRE_AUTO_TEACH);

	if (pBtnX->GetCaption() == _T("Auto Teach"))
	{
		if (theUnitFunc.DoorInterLockChk() == FALSE)
			return;

		if(m_nSeq != seqC_idle)
		{
			CString strMsg;
			strMsg.Format(_T("Contact AutoTeaching Is Running! %d"), m_nSeq);
			m_edtMsg.SetWindowText(strMsg);
			return;
		}
		// Live 모드로 해놓고 끄지 않는 경향이 있어 끄도록 해준다 2017/06/10 HSK.
		theSocketInterFace.SendMsgToGrabMode();
		m_nSeq = seqC_AMTVacOn;
		m_Jig = JIG_ID_A;
		KillTimer(2);
		SetTimer(2,100,NULL);

		pBtnX->SetCaption(_T("Cancel"));
	}
	else
	{
		KillTimer(2);
		m_nSeq = seqC_idle;
		pBtnX->SetCaption(_T("Auto Teach"));
	}
}

void CFormTeachPreAlign::ResetReturnValue()
{
	memset(m_bRtnComp, TRUE, sizeof(BYTE)*MAX_RTN_SIZE);
	memset(m_bRtn, TRUE, sizeof(BYTE)*MAX_RTN_SIZE);
}

BOOL CFormTeachPreAlign::IsReturnOk()
{
	return (memcmp(m_bRtn, m_bRtnComp, MAX_RTN_SIZE) == 0) ? TRUE:FALSE;
}

void CFormTeachPreAlign::SeqContactAutoTeaching()
{
	if (theUnitFunc.DoorInterLockChk() == FALSE)
	{
		m_nSeq = seqC_idle;
		return;
	}

	CString strMsg;
	strMsg.Format(_T("Contact AutoTeaching Seq = %d"), m_nSeq);
	m_edtMsg.SetWindowText(strMsg);
	ResetReturnValue();

	switch(m_nSeq)
	{
	case seqC_idle:
		break;
	case seqC_AMTVacOn:
		for (int i = 0; i < JIG_ID_MAX; i++)
		{
			for (int j = 0; j < JIG_CH_MAX; j++)
				theUnitFunc.AMTVacOn(VAC_ON,(JIG_ID)i,(JIG_CH)j);	
		}

		m_nTeachingTablePos = CONST_TABLE_POS::POS_1;
		strMsg.Format(_T("Contact AutoTeaching Seq = seqC_AMTVacOn"));
		m_edtMsg.SetWindowText(strMsg);
		
		//2017-12-22,SKCHO, 카운팅 조사용
		theLog[LOG_SEQUENCE].AddBuf( _T("[AutoTeaching] AutoTeaching Start"));

		m_nSeq++;
		m_Timer.Start();
		break;
	case seqC_Zup:
		m_bRtn[0] = theUnitFunc.AMTVacChk(JIG_ID_A,JIG_CH_1);
		m_bRtn[1] = theUnitFunc.AMTVacChk(JIG_ID_A,JIG_CH_2);
		m_bRtn[2] = theUnitFunc.AMTVacChk(JIG_ID_B,JIG_CH_1);
		m_bRtn[3] = theUnitFunc.AMTVacChk(JIG_ID_B,JIG_CH_2);
		m_bRtn[4] = theUnitFunc.AMTVacChk(JIG_ID_C,JIG_CH_1);
		m_bRtn[5] = theUnitFunc.AMTVacChk(JIG_ID_C,JIG_CH_2);
		m_bRtn[6] = theUnitFunc.AMTVacChk(JIG_ID_D,JIG_CH_1);
		m_bRtn[7] = theUnitFunc.AMTVacChk(JIG_ID_D,JIG_CH_2);
		if (IsReturnOk())
		{
			theUnitFunc.LDTRZUpPos();
			theUnitFunc.Move_PATR_ZAxis_UpPos();
			theUnitFunc.MoveUnLoadTR_ZAxis_Up();

			strMsg.Format(_T("Contact AutoTeaching Seq = seqC_Zup"));
			m_edtMsg.SetWindowText(strMsg);

			m_nSeq++;
			m_Timer.Start();
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			if (m_bRtn[0] == FALSE)				strMsg.Format(_T("JIG A Ch 1 Vac On Fail"));	
			else if (m_bRtn[1] == FALSE)		strMsg.Format(_T("JIG A Ch 2 Vac On Fail"));	
			else if (m_bRtn[2] == FALSE)		strMsg.Format(_T("JIG B Ch 1 Vac On Fail"));	
			else if (m_bRtn[3] == FALSE)		strMsg.Format(_T("JIG B Ch 2 Vac On Fail"));	
			else if (m_bRtn[4] == FALSE)		strMsg.Format(_T("JIG C Ch 1 Vac On Fail"));	
			else if (m_bRtn[5] == FALSE)		strMsg.Format(_T("JIG C Ch 2 Vac On Fail"));	
			else if (m_bRtn[6] == FALSE)		strMsg.Format(_T("JIG D Ch 1 Vac On Fail"));	
			else if (m_bRtn[7] == FALSE)		strMsg.Format(_T("JIG D Ch 2 Vac On Fail"));	
			m_edtMsg.SetWindowText(strMsg);
			m_nSeq = seqC_idle;
		}
		break;
	case seqC_Ready:
		m_bRtn[0] = theUnitFunc.LDTRZUpPosChk();
		m_bRtn[1] = theUnitFunc.Chk_PATR_ZAxis_UpPos();
		m_bRtn[2] = theUnitFunc.MoveUnLoadTR_ZAxis_UpChk();
		if (IsReturnOk())
		{
			theUnitFunc.B_Zone_Cam_X_Ready();
			theUnitFunc.B_Zone_Cam_Y_Ready();
			theUnitFunc.LDTRXPutPos();
			theUnitFunc.PAStageUnloadPos();

			theUnitFunc.PAXPos(JIG_CH_1);
			theUnitFunc.PAXPos(JIG_CH_2);
			theUnitFunc.PAYPos(JIG_CH_1);
			theUnitFunc.PAYPos(JIG_CH_2);
			theUnitFunc.PATPos(JIG_CH_1);
			theUnitFunc.PATPos(JIG_CH_2);

			theUnitFunc.Move_PATR_YAxis_WaitPos();
			strMsg.Format(_T("Contact AutoTeaching Seq = seqC_Ready"));
			m_edtMsg.SetWindowText(strMsg);
			m_nSeq++;
			m_Timer.Start();
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			if (m_bRtn[0] == FALSE)				strMsg.Format(_T("LD TR Z Up Chk Fail"));	
			else if (m_bRtn[1] == FALSE)		strMsg.Format(_T("PA TR Z Up Chk Fail"));	
			else if (m_bRtn[2] == FALSE)		strMsg.Format(_T("UD TR Z Up Chk Fail"));	
			m_edtMsg.SetWindowText(strMsg);
			m_nSeq = seqC_idle;
		}
		break;
	case seqC_TableEntry:
		m_bRtn[0] = theUnitFunc.B_Zone_Cam_X_Ready_Chk();
		m_bRtn[1] = theUnitFunc.B_Zone_Cam_Y_Ready_Chk();
		m_bRtn[2] = theUnitFunc.LDTRXPutPosChk();
		m_bRtn[3] = theUnitFunc.PAStageUnloadPosChk();
		m_bRtn[4] = theUnitFunc.PAXPosChk(JIG_CH_1);
		m_bRtn[5] = theUnitFunc.PAXPosChk(JIG_CH_2);
		m_bRtn[6] = theUnitFunc.PAYPosChk(JIG_CH_1);
		m_bRtn[7] = theUnitFunc.PAYPosChk(JIG_CH_2);
		m_bRtn[8] = theUnitFunc.PATPosChk(JIG_CH_1);
		m_bRtn[9] = theUnitFunc.PATPosChk(JIG_CH_2);
		m_bRtn[10] = theUnitFunc.Chk_PATR_YAxis_WaitPos();
		if (IsReturnOk())
		{
			if(theUnitFunc.GetEntryTablePos(0.02) != m_nTeachingTablePos)
				theUnitFunc.MoveToTblPos(m_nTeachingTablePos,0.5);
			strMsg.Format(_T("Contact AutoTeaching Seq = seqC_TableEntry"));
			m_edtMsg.SetWindowText(strMsg);

			m_nSeq++;
			m_Timer.Start();
		}
		else if (m_Timer.Stop(FALSE) > 5.)
		{
			if (m_bRtn[0] == FALSE)				strMsg.Format(_T("B Zone Cam X Ready Chk Fail"));	
			else if (m_bRtn[1] == FALSE)		strMsg.Format(_T("B Zone Cam Y Ready  Chk Fail"));	
			else if (m_bRtn[2] == FALSE)		strMsg.Format(_T("LD TR X Put Chk Fail"));	
			else if (m_bRtn[3] == FALSE)		strMsg.Format(_T("PA Stage Unload Chk Fail"));	
			else if (m_bRtn[4] == FALSE)		strMsg.Format(_T("PA X Ch1 Pos Chk Fail"));	
			else if (m_bRtn[5] == FALSE)		strMsg.Format(_T("PA X Ch2 Pos Chk Fail"));	
			else if (m_bRtn[6] == FALSE)		strMsg.Format(_T("PA Y Ch1 Pos Chk Fail"));	
			else if (m_bRtn[7] == FALSE)		strMsg.Format(_T("PA Y Ch2 Pos Chk Fail"));	
			else if (m_bRtn[8] == FALSE)		strMsg.Format(_T("PA T Ch1 Pos Chk Fail"));	
			else if (m_bRtn[9] == FALSE)		strMsg.Format(_T("PA T Ch2 Pos Chk Fail"));	
			else if (m_bRtn[10] == FALSE)		strMsg.Format(_T("PA TR Y WAIT Pos Chk Fail"));	
			m_edtMsg.SetWindowText(strMsg);
			m_nSeq = seqC_idle;
		}
		break;
	case seqC_LDTRZDown:
		if (theUnitFunc.GetEntryTablePos(0.02) == m_nTeachingTablePos)
			m_bRtn[0] = TRUE;
		else
			m_bRtn[0] = FALSE;

		if (IsReturnOk())
		{
			theUnitFunc.LDTRZPutPos();
			theUnitFunc.LDTRVacOn(VAC_ON,JIG_CH_MAX);
			strMsg.Format(_T("Contact AutoTeaching Seq = seqC_LDTRZDown"));
			m_edtMsg.SetWindowText(strMsg);

			m_nSeq++;
			m_Timer.Start();
		}
		else if (m_Timer.Stop(FALSE) > 10.)
		{
			strMsg.Format(_T("Turn Table Move Fail"));	
			m_edtMsg.SetWindowText(strMsg);
			m_nSeq = seqC_idle;
		}
		break;
	case seqC_TableVacOff:
		m_bRtn[0] = theUnitFunc.LDTRZPutPosChk();
		m_bRtn[1] = theUnitFunc.LDTRCellVacChk(JIG_CH_1,VAC_ON);
		m_bRtn[2] = theUnitFunc.LDTRCellVacChk(JIG_CH_2,VAC_ON);

		if (IsReturnOk())
		{
			theUnitFunc.AMTVacOn(FALSE, m_Jig, JIG_CH_MAX);
			strMsg.Format(_T("Contact AutoTeaching Seq = seqC_TableVacOff"));
			m_edtMsg.SetWindowText(strMsg);

			m_nSeq++;
			m_Timer.Start();
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			if (m_bRtn[0] == FALSE)				strMsg.Format(_T("LD TR Z Put Move Fail"));	
			else if (m_bRtn[1] == FALSE)		strMsg.Format(_T("LD TR Ch1 Vac On Fail"));	
			else if (m_bRtn[2] == FALSE)		strMsg.Format(_T("LD TR Ch2 Vac On Fail"));	
			m_edtMsg.SetWindowText(strMsg);
			m_nSeq = seqC_idle;
		}
		break;
	case seqC_LDTRZUp:
		m_bRtn[0] = theUnitFunc.AMTVacChk(m_Jig,JIG_CH_1);
		m_bRtn[1] = theUnitFunc.AMTVacChk(m_Jig,JIG_CH_2);

		if (m_bRtn[0] == FALSE && m_bRtn[1] == FALSE && m_Timer.Stop(FALSE) > 1.)
		{
			theUnitFunc.LDTRZUpPos(0.5);
			strMsg.Format(_T("Contact AutoTeaching Seq = seqC_LDTRZUp"));
			m_edtMsg.SetWindowText(strMsg);

			m_nSeq++;
			m_Timer.Start();
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			if (m_bRtn[0] == TRUE)			strMsg.Format(_T("AMT Ch1 Vac Off Fail"));	
			else if (m_bRtn[1] == TRUE)		strMsg.Format(_T("AMT Ch2 Vac Off Fail"));	
			m_edtMsg.SetWindowText(strMsg);
			m_nSeq = seqC_idle;
		}
		break;
	case seqC_LDTRXGet:
		m_bRtn[0] = theUnitFunc.LDTRCellVacChk(JIG_CH_1,VAC_ON);
		m_bRtn[1] = theUnitFunc.LDTRCellVacChk(JIG_CH_2,VAC_ON);
		m_bRtn[2] = theUnitFunc.LDTRZUpPosChk();

		if (IsReturnOk())
		{
			theUnitFunc.LDTRXGetPos();
			strMsg.Format(_T("Contact AutoTeaching Seq = seqC_LDTRXGet"));
			m_edtMsg.SetWindowText(strMsg);

			m_nSeq++;
			m_Timer.Start();
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			if (m_bRtn[0] == FALSE)				strMsg.Format(_T("LD TR Ch1 Vac On Fail"));	
			else if (m_bRtn[1] == FALSE)		strMsg.Format(_T("LD TR Ch2 Vac On Fail"));	
			else if (m_bRtn[2] == FALSE)		strMsg.Format(_T("LD TR Z Up Move Fail"));	
			m_edtMsg.SetWindowText(strMsg);
			m_nSeq = seqC_idle;
		}
		break;
	case seqC_LDTRZGet:
		m_bRtn[0] = theUnitFunc.LDTRXGetPosChk();

		if (IsReturnOk())
		{
			theUnitFunc.LDTRZGetPos();
			theUnitFunc.PAStageVac(VAC_ON,JIG_CH_MAX);
			strMsg.Format(_T("Contact AutoTeaching Seq = seqC_LDTRZGet"));
			m_edtMsg.SetWindowText(strMsg);

			m_nSeq++;
			m_Timer.Start();
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			strMsg.Format(_T("LD TR X Get Move Fail"));	
			m_edtMsg.SetWindowText(strMsg);
			m_nSeq = seqC_idle;
		}
		break;
	case seqC_LDTRVacOff:
		m_bRtn[0] = theUnitFunc.LDTRZGetPosChk();

		if (IsReturnOk())
		{
			theUnitFunc.LDTRVacOn(VAC_OFF,JIG_CH_MAX);

			strMsg.Format(_T("Contact AutoTeaching Seq = seqC_LDTRVacOff"));
			m_edtMsg.SetWindowText(strMsg);

			m_nSeq++;
			m_Timer.Start();
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			strMsg.Format(_T("LD TR Z Get Move Fail"));	
			m_edtMsg.SetWindowText(strMsg);
			m_nSeq = seqC_idle;
		}
		break;
	case seqC_LDTRZUp2:
		m_bRtn[0] = theUnitFunc.LDTRCellVacChk(JIG_CH_1,VAC_OFF);
		m_bRtn[1] = theUnitFunc.LDTRCellVacChk(JIG_CH_2,VAC_OFF);
		m_bRtn[2] = theUnitFunc.PAStageVacChk(JIG_CH_1,VAC_ON);
		m_bRtn[3] = theUnitFunc.PAStageVacChk(JIG_CH_2,VAC_ON);

		if (IsReturnOk())
		{
			theUnitFunc.LDTR_BlowOff(JIG_CH_MAX);
			theUnitFunc.LDTRZUpPos();

			strMsg.Format(_T("Contact AutoTeaching Seq = seqC_LDTRZUp2"));
			m_edtMsg.SetWindowText(strMsg);

			m_nSeq++;
			m_Timer.Start();
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			if (m_bRtn[0] == FALSE)				strMsg.Format(_T("LD TR Ch1 Vac Off Fail"));	
			else if (m_bRtn[1] == FALSE)		strMsg.Format(_T("LD TR Ch2 Vac Off Fail"));	
			else if (m_bRtn[2] == FALSE)		strMsg.Format(_T("PA Stage Ch1 Vac On Fail"));	
			else if (m_bRtn[3] == FALSE)		strMsg.Format(_T("PA Stage Ch2 Vac On Fail"));	
			m_edtMsg.SetWindowText(strMsg);
			m_nSeq = seqC_idle;
		}
		break;
	case seqC_PASTGLoadPos:
		m_bRtn[0] = theUnitFunc.LDTRZUpPosChk();

		if (IsReturnOk())
		{
			theUnitFunc.PAStageAlignPos();

			strMsg.Format(_T("Contact AutoTeaching Seq = seqC_PASTGLoadPos"));
			m_edtMsg.SetWindowText(strMsg);

			m_nSeq++;
			m_Timer.Start();
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			strMsg.Format(_T("LD TR Z Up Move Fail"));	
			m_edtMsg.SetWindowText(strMsg);
			m_nSeq = seqC_idle;
		}
		break;
	case seqC_MarkSave:
		m_bRtn[0] = theUnitFunc.PAStageAlignPosChk();

		if (IsReturnOk())
		{
			theSocketInterFace.SendMsgToContactSave(m_Jig,JIG_CH_1);

			strMsg.Format(_T("Contact AutoTeaching Seq = seqC_MarkSave"));
			m_edtMsg.SetWindowText(strMsg);

			m_nSeq++;
			m_Timer.Start();
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			strMsg.Format(_T("PA Stage Aling Pos Move Fail"));	
			m_edtMsg.SetWindowText(strMsg);
			m_nSeq = seqC_idle;
		}
		break;

	case seqC_MarkSaveChk:
		m_bRtn[0] = theProcBank.m_bMarkSaveRecv;
		if(IsReturnOk())
		{
			if( theProcBank.m_bMarkSaveSuccess)
			{
				theUnitFunc.PAStageUnloadPos();
				strMsg.Format(_T("Contact AutoTeaching Seq = seqC_MarkSaveChk"));

				m_nSeq++;
				m_Timer.Start();
			}
			else
			{
				strMsg.Format(_T("Error Seq = %d, Mark Search Fail"), m_nSeq);
				m_edtMsg.SetWindowText(strMsg);
				m_nSeq = seqC_idle;
			}
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			strMsg.Format(_T("Error Seq = %d, m_bMarkSaveRecv"), m_nSeq);
			m_edtMsg.SetWindowText(strMsg);
			m_nSeq = seqC_idle;
		}
		break;
	case seqC_LDTRZGet2:
		m_bRtn[0] = theUnitFunc.PAStageUnloadPosChk();

		if (IsReturnOk())
		{
			theUnitFunc.LDTRZGetPos();
			theUnitFunc.LDTRVacOn(VAC_ON,JIG_CH_MAX);
			strMsg.Format(_T("Contact AutoTeaching Seq = seqC_LDTRZGet2"));
			m_edtMsg.SetWindowText(strMsg);

			m_nSeq++;
			m_Timer.Start();
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			strMsg.Format(_T("PA Stage Unload Pos Move Fail"));	
			m_edtMsg.SetWindowText(strMsg);
			m_nSeq = seqC_idle;
		}
		break;
	case seqC_PASTGVacOff:
		m_bRtn[0] = theUnitFunc.LDTRZGetPosChk();
		m_bRtn[1] = theUnitFunc.LDTRCellVacChk(JIG_CH_1,VAC_ON);
		m_bRtn[2] = theUnitFunc.LDTRCellVacChk(JIG_CH_2,VAC_ON);

		if (IsReturnOk())
		{
			theUnitFunc.PAStageVac(VAC_OFF,JIG_CH_MAX);
			strMsg.Format(_T("Contact AutoTeaching Seq = seqC_PASTGVacOff"));
			m_edtMsg.SetWindowText(strMsg);

			m_nSeq++;
			m_Timer.Start();
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			if (m_bRtn[0] == FALSE)				strMsg.Format(_T("LD TR Z Get Move Fail"));	
			else if (m_bRtn[1] == FALSE)		strMsg.Format(_T("LD TR Ch1 Vac ON Fail"));	
			else if (m_bRtn[2] == FALSE)		strMsg.Format(_T("LD TR Ch2 Vac ON Fail"));	
			m_edtMsg.SetWindowText(strMsg);
			m_nSeq = seqC_idle;
		}
		break;
	case seqC_LDTRZUp3:
		m_bRtn[0] = theUnitFunc.PAStageVacChk(JIG_CH_1,VAC_OFF);
		m_bRtn[1] = theUnitFunc.PAStageVacChk(JIG_CH_2,VAC_OFF);

		if (IsReturnOk())
		{
			theUnitFunc.PAStage_BlowOff(JIG_CH_MAX);
			theUnitFunc.LDTRZUpPos();

			strMsg.Format(_T("Contact AutoTeaching Seq = seqC_LDTRZUp3"));
			m_edtMsg.SetWindowText(strMsg);

			m_nSeq++;
			m_Timer.Start();
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			if (m_bRtn[0] == FALSE)				strMsg.Format(_T("PA Stage Ch1 Vac Off Fail"));	
			else if (m_bRtn[1] == FALSE)		strMsg.Format(_T("PA Stage Ch2 Vac Off Fail"));	
			m_edtMsg.SetWindowText(strMsg);
			m_nSeq = seqC_idle;
		}
		break;
	case seqC_LDTRXPut:
		m_bRtn[0] = theUnitFunc.LDTRZUpPosChk();
		m_bRtn[1] = theUnitFunc.LDTRCellVacChk(JIG_CH_1,VAC_ON);
		m_bRtn[2] = theUnitFunc.LDTRCellVacChk(JIG_CH_2,VAC_ON);

		if (IsReturnOk())
		{
			theUnitFunc.LDTRXPutPos();

			strMsg.Format(_T("Contact AutoTeaching Seq = seqC_LDTRXPut"));
			m_edtMsg.SetWindowText(strMsg);

			m_nSeq++;
			m_Timer.Start();
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			if (m_bRtn[0] == FALSE)				strMsg.Format(_T("LD TR Z Up Move Fail"));	
			else if (m_bRtn[1] == FALSE)				strMsg.Format(_T("LD TR Ch1 Vac ON Fail"));	
			else if (m_bRtn[2] == FALSE)				strMsg.Format(_T("LD TR Ch2 Vac ON Fail"));	
			m_edtMsg.SetWindowText(strMsg);
			m_nSeq = seqC_idle;
		}
		break;
	case seqC_LDTRZPut:
		m_bRtn[0] = theUnitFunc.LDTRXPutPosChk();

		if (IsReturnOk())
		{
			theUnitFunc.LDTRZPutPos();
			theUnitFunc.AMTVacOn(VAC_ON,m_Jig,JIG_CH_MAX);
			strMsg.Format(_T("Contact AutoTeaching Seq = seqC_LDTRZPut"));
			m_edtMsg.SetWindowText(strMsg);

			m_nSeq++;
			m_Timer.Start();
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			if (m_bRtn[0] == FALSE)				strMsg.Format(_T("LD TR X Put Move Fail"));	
			m_edtMsg.SetWindowText(strMsg);
			m_nSeq = seqC_idle;
		}
		break;
	case seqC_LDTRVacOff2:
		m_bRtn[0] = theUnitFunc.LDTRZPutPosChk();
		m_bRtn[1] = theUnitFunc.AMTVacChk(m_Jig,JIG_CH_1);
		m_bRtn[2] = theUnitFunc.AMTVacChk(m_Jig,JIG_CH_2);

		if (IsReturnOk())
		{
			theUnitFunc.LDTRVacOn(VAC_OFF,JIG_CH_MAX);

			strMsg.Format(_T("Contact AutoTeaching Seq = seqC_LDTRVacOff2"));
			m_edtMsg.SetWindowText(strMsg);

			m_nSeq++;
			m_Timer.Start();
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			if (m_bRtn[0] == FALSE)				strMsg.Format(_T("LD TR Z Put Move Fail"));	
			else if (m_bRtn[1] == FALSE)				strMsg.Format(_T("AMT AJIG CH1 Vac On Fail"));	
			else if (m_bRtn[2] == FALSE)				strMsg.Format(_T("AMT AJIG CH2 Vac On Fail"));	
			m_edtMsg.SetWindowText(strMsg);
			m_nSeq = seqC_idle;
		}
		break;
	case seqC_LDTRZUp4:
		m_bRtn[0] = theUnitFunc.LDTRCellVacChk(JIG_CH_1,VAC_OFF);
		m_bRtn[1] = theUnitFunc.LDTRCellVacChk(JIG_CH_2,VAC_OFF);

		if (IsReturnOk())
		{
			theUnitFunc.LDTR_BlowOff(JIG_CH_MAX);
			theUnitFunc.LDTRZUpPos();

			strMsg.Format(_T("Contact AutoTeaching Seq = seqC_LDTRZUp4"));
			m_edtMsg.SetWindowText(strMsg);

			m_nSeq++;
			m_Timer.Start();
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			if (m_bRtn[0] == FALSE)					strMsg.Format(_T("LD TR Ch1 Vac Off Fail"));	
			else if (m_bRtn[1] == FALSE)				strMsg.Format(_T("LD TR Ch2 Vac Off Fail"));	
			m_edtMsg.SetWindowText(strMsg);
			m_nSeq = seqC_idle;
		}
		break;
	case seqC_CalcNextStep:
		m_bRtn[0] = theUnitFunc.LDTRZUpPosChk();

		if (IsReturnOk())
		{
			if(m_nTeachingTablePos >= CONST_TABLE_POS::POS_4)
			{
				m_nSeq = seqC_End;
			}
			else
			{
				switch(m_Jig)
				{
				case JIG_ID_A:	m_Jig = JIG_ID_D;	break;
				case JIG_ID_B:	m_Jig = JIG_ID_A;	break;
				case JIG_ID_C:	m_Jig = JIG_ID_B;	break;
				case JIG_ID_D:	m_Jig = JIG_ID_C;	break;
				}
				m_nTeachingTablePos++;
				m_nSeq = seqC_TableEntry;
			}
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			if (m_bRtn[0] == FALSE)					strMsg.Format(_T("LD TR Z Up Move Fail"));	
			m_edtMsg.SetWindowText(strMsg);
			m_nSeq = seqC_idle;
		}
		break;
	case seqC_End:
		strMsg.Format(_T("Contact AutoTeaching Seq = seqC_End"));
		m_edtMsg.SetWindowText(strMsg);

		m_nSeq = seqC_idle;
		break;

	default:
		break;
	}
}


void CFormTeachPreAlign::ClickGxbtnPreStgAlignPos()
{
	m_CurAxis = (AXIS_ID)AXIS_SVO_PA_STAGE_X;
	m_CurTeach = TEACH_PARAM::PA_STAGE_ALIGN_POS;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_PRE_STG_ALIGN_POS);
}


void CFormTeachPreAlign::ClickGxbtnPreAlignManuualCh1()
{
	CCellTag tagCell;
	CCellInfo* pCellInfo;

	tagCell = theCellBank.GetCellTag(CELL_POS_PA_STAGE_CH1);
	pCellInfo = theCellBank.GetCellInfo(CELL_POS_PA_STAGE_CH1);
	if(!tagCell.IsExist())
	{
       return;
	}

	pCellInfo->Pre_AlignResultT = ALIGN_RESULT_NONE;
	pCellInfo->Pre_AlignResultXY = ALIGN_RESULT_NONE;

	theUnitFunc.PA_SendAlignStart(TRUE);
}


void CFormTeachPreAlign::ClickGxbtnPreAlignManuualCh2()
{
	CCellTag tagCell;
	CCellInfo* pCellInfo;

	tagCell = theCellBank.GetCellTag(CELL_POS_PA_STAGE_CH2);
	pCellInfo = theCellBank.GetCellInfo(CELL_POS_PA_STAGE_CH2);
	if(!tagCell.IsExist())
	{
       return;
	}

	pCellInfo->Pre_AlignResultT = ALIGN_RESULT_NONE;
	pCellInfo->Pre_AlignResultXY = ALIGN_RESULT_NONE;

	theUnitFunc.PA_SendAlignStart(TRUE);
}
