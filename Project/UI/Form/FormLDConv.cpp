// FormTeach1.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FormLDConv.h"

#include "GUIDefine.h"

//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CFormTeach1

IMPLEMENT_DYNCREATE(CFormLDConv, CFormView)

CFormLDConv::CFormLDConv()
	: CFormView(CFormLDConv::IDD)
{
	//kjpark 20161024 Load Conveyor 구현
	Init();
}

CFormLDConv::~CFormLDConv()
{
}

void CFormLDConv::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFormLDConv, CFormView)
	//kjpark 20161024 Load Conveyor 구현
	ON_WM_TIMER()
	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

END_MESSAGE_MAP()

//kjpark 20161024 Load Conveyor 구현
BEGIN_EVENTSINK_MAP(CFormLDConv, CFormView)
	ON_EVENT(CFormLDConv, IDC_GXBTN_TEACH_LDCONV_UP, DISPID_CLICK, CFormLDConv::ClickGxbtnCylinderUP, VTS_NONE)
	ON_EVENT(CFormLDConv, IDC_GXBTN_TEACH_LDCONV_DOWN, DISPID_CLICK, CFormLDConv::ClickGxbtnCylinderDown, VTS_NONE)
	ON_EVENT(CFormLDConv, IDC_GXBTN_TEACH_LDCONV_UP, DISPID_CLICK, CFormLDConv::ClickGxbtnTUP, VTS_NONE)
	ON_EVENT(CFormLDConv, IDC_GXBTN_TEACH_LDCONV_DOWN, DISPID_CLICK, CFormLDConv::ClickGxbtnTDown, VTS_NONE)
END_EVENTSINK_MAP()
// CFormTeach1 진단입니다.

#ifdef _DEBUG
void CFormLDConv::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormLDConv::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

//kjpark 20161024 Load Conveyor 구현
void CFormLDConv::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case 1:
		GetStatus();
		break;

	}
	CFormView::OnTimer(nIDEvent);
}

// CFormTeach1 메시지 처리기입니다.

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
LRESULT CFormLDConv::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{
		//kjpark 20161024 Load Conveyor 구현
		SetTimer(1, 1000, NULL);
	}
	else
	{
		//kjpark 20161024 Load Conveyor 구현
		KillTimer(1);
	}

	return 0;
}

// 표시 언어가 변경되었다.
LRESULT CFormLDConv::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	// CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_BTNX_BOTTOM_MENU1);

	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


// 윈도우 생성전에 모양을 달리 지정한다.
BOOL CFormLDConv::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	cs.style = WS_OVERLAPPED | WS_CHILD;				// 테두리를 없앤다.

	return CFormView::PreCreateWindow(cs);
	
	
}

//kjpark 20161024 Load Conveyor 구현
void CFormLDConv::Init()
{
	//컨베어 셀체크
	ConveryCell.bCheck = FALSE;
	ConveryCell.nBtnRecouseID = 0;
	ConveryCell.nStaticRecouseID = IDC_GXSTC_TEACH_LDCONV_IOLAMP;

	//리버스 업
	ReversUp.bCheck = FALSE;
	ReversUp.nBtnRecouseID = IDC_GXBTN_TEACH_LDCONV_UP;
	ReversUp.nStaticRecouseID = IDC_GXBTN_TEACH_LDCONV_LAMP;

	//리버스 다운
	ReversDown.bCheck = FALSE;
	ReversDown.nBtnRecouseID = IDC_GXBTN_TEACH_LDCONV_DOWN;
	ReversDown.nStaticRecouseID = IDC_GXBTN_TEACH_LDCONV_DOWN_LAMP;

	//진공인
	VacuumIN.bCheck = FALSE;
	VacuumIN.nBtnRecouseID = IDC_GXSTC_TEACH_LDCONV_VAC;
	VacuumIN.nStaticRecouseID = IDC_GXBTN_TEACH_LDCONV_VAC_LAMP;

	//진공 온
	VacuumON.bCheck = FALSE;
	VacuumON.nBtnRecouseID = IDC_GXSTC_TEACH_LDCONV_IO11;
	VacuumON.nStaticRecouseID = IDC_GXSTC_TEACH_LDCONV_IOLAMP9;

	//진공 오프
	VacuumOFF.bCheck = FALSE;
	VacuumOFF.nBtnRecouseID = IDC_GXSTC_TEACH_LDCONV_IO12;
	VacuumOFF.nStaticRecouseID = IDC_GXSTC_TEACH_LDCONV_IOLAMP10;

	//정회전
	T_Un.bCheck = FALSE;
	T_Un.axis = AXIS_CELL_REVERSE_T;
	T_Un.teach = TEACH_PARAM::CELL_REVERSE_THETA_CV;
	T_Un.nBtnRecouseID = IDC_GXBTN_TEACH_LDCONV_TURN;
	T_Un.nStaticRecouseID = IDC_GXBTN_TEACH_LDCONV_TURN_LAMP;

	//역회전
	T_Down.bCheck = FALSE;
	T_Down.teach = TEACH_PARAM::BUFFER_STG_Y_to_SCARA;
	T_Down.axis = AXIS_CELL_REVERSE_T;
	T_Down.nBtnRecouseID = IDC_GXBTN_TEACH_LDCONV_CCTURN;
	T_Down.nStaticRecouseID = IDC_GXBTN_TEACH_LDCONV_CCTURN_LAMP;

}

//kjpark 20161024 Load Conveyor 구현
void CFormLDConv::GetCellCheck()
{
	if(theUnitFunc.GetInPutIOCheck(123))
	{
		ConveryCell.bCheck = TRUE;
		CGxUICtrl::SetStaticColor(this, ConveryCell.nStaticRecouseID, GXCOLOR_ON);
	}
	else
	{
		ConveryCell.bCheck = FALSE;
		CGxUICtrl::SetStaticColor(this, ConveryCell.nStaticRecouseID, GXCOLOR_OFF);
	}
}

//kjpark 20161024 Load Conveyor 구현
void CFormLDConv::GetReverseCheck()
{
	if(theUnitFunc.GetInPutIOCheck(X_CELL_REVERSE_UP))
	{
		ReversUp.bCheck = TRUE;
		CGxUICtrl::SetStaticColor(this, ReversUp.nStaticRecouseID, GXCOLOR_ON);

	}
	else
	{
		ReversUp.bCheck = FALSE;
		CGxUICtrl::SetStaticColor(this, ReversUp.nStaticRecouseID, GXCOLOR_OFF);
	}

	if(theUnitFunc.GetInPutIOCheck(X_CELL_REVERSE_DOWN))
	{
		ReversDown.bCheck = TRUE;
		CGxUICtrl::SetStaticColor(this, ReversDown.nStaticRecouseID, GXCOLOR_ON);

	}
	else
	{
		ReversDown.bCheck = FALSE;
		CGxUICtrl::SetStaticColor(this, ReversDown.nStaticRecouseID, GXCOLOR_OFF);
	}
}

//kjpark 20161024 Load Conveyor 구현
void CFormLDConv::GetVacuumCheck()
{
	if(theUnitFunc.GetInPutIOCheck(X_CELL_REVERSE_VAC))
	{
		VacuumIN.bCheck =TRUE;
		CGxUICtrl::SetStaticColor(this, VacuumIN.nStaticRecouseID, GXCOLOR_ON);
	}
	else
	{
		VacuumIN.bCheck =FALSE;
		CGxUICtrl::SetStaticColor(this, VacuumIN.nStaticRecouseID, GXCOLOR_OFF);
	}
}
	
//kjpark 20161024 Load Conveyor 구현
void CFormLDConv::GetT_UpCheck()
{
	if(theUnitFunc.GetAxisCheck(T_Un.teach))
	{
		T_Un.bCheck = TRUE;
		CGxUICtrl::SetStaticColor(this, T_Un.nStaticRecouseID, GXCOLOR_ON);
	}
	else
	{
		T_Un.bCheck = FALSE;
		CGxUICtrl::SetStaticColor(this, T_Un.nStaticRecouseID, GXCOLOR_OFF);
	}
}

//kjpark 20161024 Load Conveyor 구현
void CFormLDConv::GetT_DownCheck()
{
	if(theUnitFunc.GetAxisCheck(T_Down.teach))
	{
		T_Down.bCheck = TRUE;
		CGxUICtrl::SetStaticColor(this, T_Down.nStaticRecouseID, GXCOLOR_ON);
	}
	else
	{
		T_Down.bCheck = FALSE;
		CGxUICtrl::SetStaticColor(this, T_Down.nStaticRecouseID, GXCOLOR_OFF);
	}
}

//kjpark 20161024 Load Conveyor 구현
void CFormLDConv::GetStatus()
{
	GetCellCheck();
	GetReverseCheck();
	GetVacuumCheck();
	GetT_UpCheck();
	GetT_DownCheck();
}

//kjpark 20161024 Load Conveyor 구현
void CFormLDConv::ClickGxbtnCylinderUP()
{
	theUnitFunc.SetOutPutIO(CYL_UP, Y_CELL_REVERSE_UP);
	theUnitFunc.SetOutPutIO(CYL_DOWN, Y_CELL_REVERSE_DOWN);
}

//kjpark 20161024 Load Conveyor 구현
void CFormLDConv::ClickGxbtnCylinderDown()
{
	theUnitFunc.SetOutPutIO(CYL_DOWN, Y_CELL_REVERSE_UP);
	theUnitFunc.SetOutPutIO(CYL_UP, Y_CELL_REVERSE_DOWN);
}

//kjpark 20161024 Load Conveyor 구현
void CFormLDConv::ClickGxbtnTUP()
{
	GetMainHandler()->SetSelectedAxis(T_Un.axis, T_Un.teach);
}

//kjpark 20161024 Load Conveyor 구현
void CFormLDConv::ClickGxbtnTDown()
{
	GetMainHandler()->SetSelectedAxis(T_Down.axis, T_Down.teach);
}