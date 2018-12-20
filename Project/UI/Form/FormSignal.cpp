// FormSetup1.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FormSignal.h"

#include "GUIDefine.h"

//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CFormSignal

IMPLEMENT_DYNCREATE(CFormSignal, CFormView)

CFormSignal::CFormSignal()
	: CFormView(CFormSignal::IDD)
{
	//kjpark 20161021 Setup 신호기 UI 구현
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
			m_bButtonCheck[i][j] = FALSE;
	}	

	m_nCellButton[JIG_ID_A][JIG_CH_1] = IDC_GXBTN_TEACH1_PG_A1;
	m_nCellButton[JIG_ID_A][JIG_CH_2] = IDC_GXBTN_TEACH1_PG_A2;
	m_nCellButton[JIG_ID_B][JIG_CH_1] = IDC_GXBTN_TEACH1_PG_B1;
	m_nCellButton[JIG_ID_B][JIG_CH_2] = IDC_GXBTN_TEACH1_PG_B2;
	m_nCellButton[JIG_ID_C][JIG_CH_1] = IDC_GXBTN_TEACH1_PG_C1;
	m_nCellButton[JIG_ID_C][JIG_CH_2] = IDC_GXBTN_TEACH1_PG_C2;
	m_nCellButton[JIG_ID_D][JIG_CH_1] = IDC_GXBTN_TEACH1_PG_D1;
	m_nCellButton[JIG_ID_D][JIG_CH_2] = IDC_GXBTN_TEACH1_PG_D2;

	m_bIsMutli = TRUE;
	m_bIsShwoWindow = FALSE;
}

CFormSignal::~CFormSignal()
{
}

void CFormSignal::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//kjpark 20161021 Setup 신호기 UI 구현
	DDX_Control(pDX, IDC_LIST_TEACH1_LOG, m_PGLog);
}

BEGIN_MESSAGE_MAP(CFormSignal, CFormView)

	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

END_MESSAGE_MAP()

//kjpark 20161021 Setup 신호기 UI 구현
BEGIN_EVENTSINK_MAP(CFormSignal, CFormView)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PG_A1, DISPID_CLICK, CFormSignal::ClickBtnSelect_A1, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PG_A2, DISPID_CLICK, CFormSignal::ClickBtnSelect_A2, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PG_B1, DISPID_CLICK, CFormSignal::ClickBtnSelect_B1, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PG_B2, DISPID_CLICK, CFormSignal::ClickBtnSelect_B2, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PG_C1, DISPID_CLICK, CFormSignal::ClickBtnSelect_C1, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PG_C2, DISPID_CLICK, CFormSignal::ClickBtnSelect_C2, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PG_D1, DISPID_CLICK, CFormSignal::ClickBtnSelect_D1, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PG_D2, DISPID_CLICK, CFormSignal::ClickBtnSelect_D2, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PG_SELECT_ALL, DISPID_CLICK, CFormSignal::ClickBtnSelect_All, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PG_SELECT_CLEAR, DISPID_CLICK, CFormSignal::ClickBtnSelect_Clear, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PG_SELECT_S, DISPID_CLICK, CFormSignal::ClickBtnSelect_Single, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PG_SELECT_M, DISPID_CLICK, CFormSignal::ClickBtnSelect_Multi, VTS_NONE)

	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_AZONE, DISPID_CLICK, CFormSignal::ClickBtnZoneSelect_A, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_BZONE, DISPID_CLICK, CFormSignal::ClickBtnZoneSelect_B, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_CZONE, DISPID_CLICK, CFormSignal::ClickBtnZoneSelect_C, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_DZONE, DISPID_CLICK, CFormSignal::ClickBtnZoneSelect_D, VTS_NONE)

	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PATTERN1, DISPID_CLICK, CFormSignal::ClickBtnPGSelect_ON, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PATTERN2, DISPID_CLICK, CFormSignal::ClickBtnPGSelect_OFF, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PATTERN3, DISPID_CLICK, CFormSignal::ClickBtnPGSelect_PREV, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_PATTERN4, DISPID_CLICK, CFormSignal::ClickBtnPGSelect_NEXT, VTS_NONE)

	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_FUNCTION1, DISPID_CLICK, CFormSignal::ClickBtnPGSelect_Reset, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_FUNCTION2, DISPID_CLICK, CFormSignal::ClickBtnPGSelect_CellLoading, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_FUNCTION3, DISPID_CLICK, CFormSignal::ClickBtnPGSelect_TMDInfo, VTS_NONE)
	
	ON_EVENT(CFormSignal, IDC_GXBTN_SETZONE_A, DISPID_CLICK, CFormSignal::ClickGxbtnSetzoneA, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_SETZONE_B, DISPID_CLICK, CFormSignal::ClickGxbtnSetzoneB, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_SETZONE_C, DISPID_CLICK, CFormSignal::ClickGxbtnSetzoneC, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_SETZONE_D, DISPID_CLICK, CFormSignal::ClickGxbtnSetzoneD, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_AJIG_SW, DISPID_CLICK, CFormSignal::ClickGxbtnTeach1AjigSw, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_AJIG_SW2, DISPID_CLICK, CFormSignal::ClickGxbtnTeach1AjigSw2, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_AJIG_SW3, DISPID_CLICK, CFormSignal::ClickGxbtnTeach1AjigSw3, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_TEACH1_AJIG_SW4, DISPID_CLICK, CFormSignal::ClickGxbtnTeach1AjigSw4, VTS_NONE)

	ON_EVENT(CFormSignal, IDC_GXBTN_SIGNAL_TMD1, DISPID_CLICK, CFormSignal::ClickGxbtnSignalTmd1, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_SIGNAL_TMD7, DISPID_CLICK, CFormSignal::ClickGxbtnSignalTmd7, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_SIGNAL_TMD2, DISPID_CLICK, CFormSignal::ClickGxbtnSignalTmd2, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_SIGNAL_TMD3, DISPID_CLICK, CFormSignal::ClickGxbtnSignalTmd3, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_SIGNAL_TMD4, DISPID_CLICK, CFormSignal::ClickGxbtnSignalTmd4, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_SIGNAL_TMD5, DISPID_CLICK, CFormSignal::ClickGxbtnSignalTmd5, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_SIGNAL_TMD8, DISPID_CLICK, CFormSignal::ClickGxbtnSignalTmd8, VTS_NONE)
	ON_EVENT(CFormSignal, IDC_GXBTN_SIGNAL_TMD6, DISPID_CLICK, CFormSignal::ClickGxbtnSignalTmd6, VTS_NONE)

END_EVENTSINK_MAP()
// CFormSetup1 진단입니다.

#ifdef _DEBUG
void CFormSignal::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormSignal::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormSetup1 메시지 처리기입니다.


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
LRESULT CFormSignal::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{
		//kjpark 20161021 Setup 신호기 UI 구현
		m_bIsShwoWindow = TRUE;
		if(m_bIsMutli)
		{
			CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_PG_SELECT_S, GXCOLOR_OFF);
			CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_PG_SELECT_M, GXCOLOR_ON);
		}
		else
		{
			CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_PG_SELECT_S, GXCOLOR_ON);
			CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_PG_SELECT_M, GXCOLOR_OFF);
		}

		if((theUnitFunc.GetOutPutIOCheck(Y_SWITCH_1) == TRUE)
			&& (theUnitFunc.GetOutPutIOCheck(Y_SWITCH_2) == FALSE))
		{
			CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_AJIG_SW, GXCOLOR_ON);
			CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_AJIG_SW2, GXCOLOR_OFF);
			CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_AJIG_SW3, GXCOLOR_OFF);
			CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_AJIG_SW4, GXCOLOR_OFF);
		}
		else if((theUnitFunc.GetOutPutIOCheck(Y_SWITCH_1) == FALSE)
			&& (theUnitFunc.GetOutPutIOCheck(Y_SWITCH_2) == TRUE))
		{
			CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_AJIG_SW, GXCOLOR_OFF);
			CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_AJIG_SW2, GXCOLOR_ON);
			CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_AJIG_SW3, GXCOLOR_OFF);
			CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_AJIG_SW4, GXCOLOR_OFF);
		}
		else if((theUnitFunc.GetOutPutIOCheck(Y_SWITCH_1) == TRUE)
			&& (theUnitFunc.GetOutPutIOCheck(Y_SWITCH_2) == TRUE))
		{
			CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_AJIG_SW, GXCOLOR_OFF);
			CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_AJIG_SW2, GXCOLOR_OFF);
			CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_AJIG_SW3, GXCOLOR_ON);
			CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_AJIG_SW4, GXCOLOR_OFF);
		}
		else if((theUnitFunc.GetOutPutIOCheck(Y_SWITCH_1) == FALSE)
			&& (theUnitFunc.GetOutPutIOCheck(Y_SWITCH_2) == FALSE))
		{
			CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_AJIG_SW, GXCOLOR_OFF);
			CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_AJIG_SW2, GXCOLOR_OFF);
			CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_AJIG_SW3, GXCOLOR_OFF);
			CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_AJIG_SW4, GXCOLOR_ON);
		}

		LV_COLUMN lCol;
		lCol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;

		lCol.fmt = LVCFMT_CENTER;   // 칼럼의 정렬을 왼쪽정렬로 지정(LVCFMT_CENTER, LVCFMT_LEFT, LVCFMT_RIGHT중 선택)
		lCol.pszText = _T("NO");    //  칼럼의 제목을 지정
		lCol.iSubItem = 0;    
		CRect rect;//  서브아이템의 인덱스를 지정
		m_PGLog.GetWindowRect(rect);
		lCol.cx = rect.Width()*2;               //  칼럼의 넓이를 지정
		m_PGLog.InsertColumn(0, &lCol);  //  LVCOLUMN구조체로 만들어진 값을 토대로 리스트 컨트롤에 칼럼을 삽입
	}
	else
	{
		m_bIsShwoWindow = FALSE;
	}

	return 0;
}

// 표시 언어가 변경되었다.
LRESULT CFormSignal::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	CGxUICtrl::BroadcastStaticCpationIndex(this, wParam, IDC_GXSTC_TEACH1_PG);
	CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_GXBTN_TEACH1_PG_A1);

	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


// 윈도우 생성전에 모양을 달리 지정한다.
BOOL CFormSignal::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	cs.style = WS_OVERLAPPED | WS_CHILD;				// 테두리를 없앤다.

	return CFormView::PreCreateWindow(cs);
}

//kjpark 20161021 Setup 신호기 UI 구현
BOOL CFormSignal::GetSelectButton(JIG_ID zone, JIG_CH nCh)
{	
	m_bButtonCheck[zone][nCh] = !m_bButtonCheck[zone][nCh];

	if(m_bIsMutli)
	{
		if(m_bButtonCheck[zone][nCh])
		{
			CGxUICtrl::SetButtonColor(this,m_nCellButton[zone][nCh], GXCOLOR_ON);	
		}
		else
		{
			CGxUICtrl::SetButtonColor(this,m_nCellButton[zone][nCh], GXCOLOR_OFF);
		}
	}
	else
	{
		for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
		{
			for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
			{
				m_bButtonCheck[i][j] = FALSE;
				CGxUICtrl::SetButtonColor(this,m_nCellButton[i][j], GXCOLOR_OFF);
			}
		}
		m_bButtonCheck[zone][nCh] = TRUE;
		CGxUICtrl::SetButtonColor(this,m_nCellButton[zone][nCh], GXCOLOR_ON);
	}
	

	return m_bButtonCheck[zone][nCh];
}

void CFormSignal::ClickBtnSelect_A1()
{
	GetSelectButton(JIG_ID_A, JIG_CH_1);
}

void CFormSignal::ClickBtnSelect_A2()
{
	GetSelectButton(JIG_ID_A, JIG_CH_2);
}

void CFormSignal::ClickBtnSelect_B1()
{
	GetSelectButton(JIG_ID_B, JIG_CH_1);
}

void CFormSignal::ClickBtnSelect_B2()
{
	GetSelectButton(JIG_ID_B, JIG_CH_2);
}

void CFormSignal::ClickBtnSelect_C1()
{
	GetSelectButton(JIG_ID_C, JIG_CH_1);
}

void CFormSignal::ClickBtnSelect_C2()
{
	GetSelectButton(JIG_ID_C, JIG_CH_2);
}

void CFormSignal::ClickBtnSelect_D1()
{
	GetSelectButton(JIG_ID_D, JIG_CH_1);
}

void CFormSignal::ClickBtnSelect_D2()
{
	GetSelectButton(JIG_ID_D, JIG_CH_2);
}

void CFormSignal::ClickBtnSelect_All()
{
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			m_bButtonCheck[i][j] = TRUE;
			CGxUICtrl::SetButtonColor(this,m_nCellButton[i][j], GXCOLOR_ON);
		}
	}	
	ClickBtnSelect_Multi();
}

void CFormSignal::ClickBtnSelect_Clear()
{
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			m_bButtonCheck[i][j] = FALSE;
			CGxUICtrl::SetButtonColor(this,m_nCellButton[i][j], GXCOLOR_OFF);
		}
	}	
}

void CFormSignal::ClickBtnSelect_Single()
{
	m_bIsMutli = FALSE;
	ClickBtnSelect_Clear();
	CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_PG_SELECT_S, GXCOLOR_ON);
	CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_PG_SELECT_M, GXCOLOR_OFF);
}

void CFormSignal::ClickBtnSelect_Multi()
{
	m_bIsMutli = TRUE;
	CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_PG_SELECT_S, GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_PG_SELECT_M, GXCOLOR_ON);
}


void CFormSignal::ClickBtnZoneSelect_A()
{

	for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
	{
		if (m_bButtonCheck[JIG_ID_A][j] == TRUE)
		{
			m_bButtonCheck[JIG_ID_A][j] = FALSE;
			CGxUICtrl::SetButtonColor(this,m_nCellButton[JIG_ID_A][j], GXCOLOR_OFF);
		}
		else
		{
			m_bButtonCheck[JIG_ID_A][j] = TRUE;
			CGxUICtrl::SetButtonColor(this,m_nCellButton[JIG_ID_A][j], GXCOLOR_ON);
		}
	}		
}

void CFormSignal::ClickBtnZoneSelect_B()
{
	for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
	{
		if (m_bButtonCheck[JIG_ID_B][j] == TRUE)
		{
			m_bButtonCheck[JIG_ID_B][j] = FALSE;
			CGxUICtrl::SetButtonColor(this,m_nCellButton[JIG_ID_B][j], GXCOLOR_OFF);
		}
		else
		{
			m_bButtonCheck[JIG_ID_B][j] = TRUE;
			CGxUICtrl::SetButtonColor(this,m_nCellButton[JIG_ID_B][j], GXCOLOR_ON);
		}
	}		
}

void CFormSignal::ClickBtnZoneSelect_C()
{
	for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
	{
		if (m_bButtonCheck[JIG_ID_C][j] == TRUE)
		{
			m_bButtonCheck[JIG_ID_C][j] = FALSE;
			CGxUICtrl::SetButtonColor(this,m_nCellButton[JIG_ID_C][j], GXCOLOR_OFF);
		}
		else
		{
			m_bButtonCheck[JIG_ID_C][j] = TRUE;
			CGxUICtrl::SetButtonColor(this,m_nCellButton[JIG_ID_C][j], GXCOLOR_ON);
		}
	}		
}

void CFormSignal::ClickBtnZoneSelect_D()
{
	for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
	{
		if (m_bButtonCheck[JIG_ID_D][j] == TRUE)
		{
			m_bButtonCheck[JIG_ID_D][j] = FALSE;
			CGxUICtrl::SetButtonColor(this,m_nCellButton[JIG_ID_D][j], GXCOLOR_OFF);
		}
		else
		{
			m_bButtonCheck[JIG_ID_D][j] = TRUE;
			CGxUICtrl::SetButtonColor(this,m_nCellButton[JIG_ID_D][j], GXCOLOR_ON);
		}
	}		
}


void CFormSignal::ClickBtnPGSelect_ON()
{
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.SendMassageToPatternGenerator(TURN_ON, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

void CFormSignal::ClickBtnPGSelect_OFF()
{
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.SendMassageToPatternGenerator(TURN_OFF, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

void CFormSignal::ClickBtnPGSelect_PREV()
{
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.SendMassageToPatternGenerator(BACK, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

void CFormSignal::ClickBtnPGSelect_NEXT()
{
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.SendMassageToPatternGenerator(NEXT, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}


void CFormSignal::ClickBtnPGSelect_Reset()
{
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.SendMassageToPatternGenerator(RESET, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

void CFormSignal::ClickBtnPGSelect_CellLoading()
{
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.SendMassageToPatternGenerator(CELL_LOADING, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}

void CFormSignal::ClickBtnPGSelect_TMDInfo()
{
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.SendMassageToPatternGenerator(TMD_INFO, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}
//kjpark 20161021 Setup 신호기 UI 구현
void CFormSignal::SetLogListUpdate(CString strMessage)
{
	m_PGLog.InsertItem(m_PGLog.GetItemCount(), strMessage);

	int nCount = m_PGLog.GetItemCount();
	if (nCount > 0)
		m_PGLog.EnsureVisible(nCount-1, FALSE);
}

void CFormSignal::ClickGxbtnSetzoneA()
{
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.SendMassageToPatternGenerator(SET_ZONE_A, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}


void CFormSignal::ClickGxbtnSetzoneB()
{
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.SendMassageToPatternGenerator(SET_ZONE_B, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}


void CFormSignal::ClickGxbtnSetzoneC()
{
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.SendMassageToPatternGenerator(SET_ZONE_C, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}


void CFormSignal::ClickGxbtnSetzoneD()
{
	for(int i = JIG_ID_A ; i < JIG_ID_MAX ; i++)
	{
		for(int j = JIG_CH_1 ; j < JIG_CH_MAX ; j++)
		{
			if(m_bButtonCheck[i][j])
			{
				theSocketInterFace.SendMassageToPatternGenerator(SET_ZONE_D, (JIG_ID)i,(JIG_CH)j);
			}
		}
	}	
}


void CFormSignal::ClickGxbtnTeach1AjigSw()
{
	theUnitFunc.SetOutPutIO(Y_SWITCH_1,TRUE);
	theUnitFunc.SetOutPutIO(Y_SWITCH_2,FALSE);
	CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_AJIG_SW, GXCOLOR_ON);
	CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_AJIG_SW2, GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_AJIG_SW3, GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_AJIG_SW4, GXCOLOR_OFF);
}


void CFormSignal::ClickGxbtnTeach1AjigSw2()
{
	theUnitFunc.SetOutPutIO(Y_SWITCH_1,FALSE);
	theUnitFunc.SetOutPutIO(Y_SWITCH_2,TRUE);
	CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_AJIG_SW, GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_AJIG_SW2, GXCOLOR_ON);
	CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_AJIG_SW3, GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_AJIG_SW4, GXCOLOR_OFF);
}


void CFormSignal::ClickGxbtnTeach1AjigSw3()
{
	theUnitFunc.SetOutPutIO(Y_SWITCH_1,TRUE);
	theUnitFunc.SetOutPutIO(Y_SWITCH_2,TRUE);
	CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_AJIG_SW, GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_AJIG_SW2, GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_AJIG_SW3, GXCOLOR_ON);
	CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_AJIG_SW4, GXCOLOR_OFF);
}


void CFormSignal::ClickGxbtnTeach1AjigSw4()
{
	theUnitFunc.SetOutPutIO(Y_SWITCH_1,FALSE);
	theUnitFunc.SetOutPutIO(Y_SWITCH_2,FALSE);
	CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_AJIG_SW, GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_AJIG_SW2, GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_AJIG_SW3, GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this,IDC_GXBTN_TEACH1_AJIG_SW4, GXCOLOR_ON);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 2017-02-27, jhLee, Job download Test 진행

// 신호기에게 'ROUTE' 명령을 전송한다.
void CFormSignal::ClickGxbtnSignalTmd1()
{
	/*
	CString FilePath;
	CString m_strServerPath;
	CString m_strJobFilename;
	CString FileBackupPath;		
	CString FileBackupFullPath;		

	m_strServerPath = _T("GROUP\\AMB528ND02");
	m_strJobFilename = _T("AMB585NC10-E1-Golden-AMT-170418_PORBRINGUP_V0.TMD");
	FileBackupPath = _T("D:\\Cheetah\\Runtime\\TMDBackUp");	

	FileBackupFullPath.Format(_T("%s\\%s\\%s"),FileBackupPath,m_strServerPath,m_strJobFilename);
	if(CFileSupport::DirectoryCheck(FileBackupPath) == FALSE)
	{
		if(CFileSupport::CreateDirectory(FileBackupPath) == FALSE)
			return;
	}

	FilePath.Format(_T("X:\\%s\\%s"),m_strServerPath,m_strJobFilename);
	::CopyFile(FilePath,FileBackupFullPath, FALSE);

	return;
	*/
	CString strMsg;
	CString sPath = theConfigBank.m_System.m_strServerPath;					// 서버에서 download할 path
	theSocketInterFace.PG_TMD_SendRoute( sPath );							// 경로 지정 전송
	
	strMsg.Format(_T("서버 경로 전송 %s"),sPath);
	SetLogListUpdate(strMsg);
}


// 신호기에게 'TMD' 명령을 전송한다.
void CFormSignal::ClickGxbtnSignalTmd7()
{
	CString strMsg;
	CString sCmd = theConfigBank.m_System.GetTMDFileList();					// 지정된 데이터로 신호기로 보내기위한 파일 리스트 문자열을 만들어준다.
	theSocketInterFace.PG_TMD_SendFileList( sCmd );

	strMsg.Format(_T("TMD 명령 전송:%s"),sCmd);
	SetLogListUpdate(strMsg);
}

// 신호기에게 'TMD_COPY_START'를 전송한다.
void CFormSignal::ClickGxbtnSignalTmd2()
{
	CString strMsg;
	theSocketInterFace.PG_TMD_SendCopyStart();												// MES Server로부터 복사를 지령한다.;

    strMsg.Format(_T("MES 서버로부터 파일 복사 시작."));
	SetLogListUpdate(strMsg);
}


// 신호기에게 'TMD_CHECKSUM_START'를 전송한다.
void CFormSignal::ClickGxbtnSignalTmd3()
{
	CString strMsg;
	theSocketInterFace.PG_TMD_SendChecksumStart();											// Download한 파일들의 Checksum 검사한다.
	strMsg.Format(_T("다운로드된 파일 체크섬 검사 시작."));
	SetLogListUpdate(strMsg);

}

// 신호기에게 'CLIENT_CHECKSUM_START'를 전송한다.
void CFormSignal::ClickGxbtnSignalTmd4()
{
	CString strMsg;
	theSocketInterFace.PG_TMD_SendClientChecksum();										// Download한 파일과 각 신호기의 현재 파일과의 Checksum 비교를 한다.
	strMsg.Format(_T("다운로드된 파일과 신호기 현재 파일 체크섬 검사 시작."));
	SetLogListUpdate(strMsg);

}

// 신호기에게 'TMD_DOWNLOAD_START'를 전송한다.
void CFormSignal::ClickGxbtnSignalTmd5()
{
	CString strMsg;
	theSocketInterFace.PG_TMD_SendDownloadStart();											// Download한 파일들을 각 신호기에게 전송한다.
	strMsg.Format(_T("다운로드된 파일을 각 신호기에 전송 시작."));
	SetLogListUpdate(strMsg);
}

// MES에서 보내온 정보 파일을 직접 읽어온다. 
void CFormSignal::ClickGxbtnSignalTmd8()
{
	CString strMsg;
	theConfigBank.m_System.ReadTMDJobFileInfo();							// 상위에서 보내온 Job file download관련 정보를 읽어온다.

    strMsg.Format(_T("JobFile 관련 정보 READ."));
	SetLogListUpdate(strMsg);
}


// MES로 현재 신행 상태를 S6F11 CEID 109로 보고한다.
void CFormSignal::ClickGxbtnSignalTmd6()
{
	CString strMsg;
	CString sAction = theConfigBank.m_System.m_strActionFlag;				// 현재 동작중인 단계
	CString sResult	= _T("OK");												// 처리 결과

	theSocketInterFace.SendToTMDDownloadState(sAction, sResult);

	strMsg.Format(_T("상위에 현재 진행 상태 보고"));
	SetLogListUpdate(strMsg);
}
