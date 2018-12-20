// FormMotorUnitTest.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FormMotorUnitTest.h"

#include "GUIDefine.h"

//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "BankGuild\AxisParamBank.h"
#include "BankGuild\TeachBank.h"
#include "CheetahApp.h"
#include "UI\GausGUI\GxStaticEx.h"
#include "UI\GausGUI\GxButtonEx.h"
#include "UI\GausGUI\GxMsgBox.h"
#include "UI\GausGUI\GxNumPadEx.h"
#include "UI\GausGUI\GxUICtrl.h"
//#include "UI\Dialog\DlgBoxKeyBoard.h"
#include "UI\Dialog\DlgBoxPassWord.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

enum
{
	TIMER_MONITOR = 0
};
// CFormMoterUnitTest

IMPLEMENT_DYNCREATE(CFormMotorUnitTest, CFormView)

CFormMotorUnitTest::CFormMotorUnitTest()
	: CFormView(CFormMotorUnitTest::IDD)
{
	m_iUpDownParam = 0;
	m_bInit = FALSE;
	m_nLastMotorGridRow = -1;
	m_CurAxis = AXIS_ID_MAX;
	m_nLastTeachGridRow = -1;
	m_nCurTeachRow = -1;
	m_JogSpeed = JOG_LOW;
	m_bMove = FALSE;
}

CFormMotorUnitTest::~CFormMotorUnitTest()
{
}

void CFormMotorUnitTest::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_GridControl(pDX, IDC_GRID_SETUP4_MOTORLIST, m_MotorGrid); //해당하는 Grid리스트 설정.
	DDX_GridControl(pDX, IDC_GRID_SETUP4_TEACHLIST, m_TeachGrid);
}

BEGIN_MESSAGE_MAP(CFormMotorUnitTest, CFormView)

	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	ON_NOTIFY(NM_CLICK, IDC_GRID_SETUP4_MOTORLIST, OnGridMoterEndSelChange)
	ON_NOTIFY(NM_CLICK, IDC_GRID_SETUP4_TEACHLIST, OnGridTeachEndSelChange)

	ON_WM_TIMER()
END_MESSAGE_MAP()


// CFormMotorUnitTest 진단입니다.

#ifdef _DEBUG
void CFormMotorUnitTest::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormMotorUnitTest::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


void CFormMotorUnitTest::InitializeGrid()
{

	if(m_MotorGrid.GetSafeHwnd() == NULL) return;
	if(m_TeachGrid.GetSafeHwnd() == NULL) return;

	//Grid 설정
	m_MotorGrid.SetEditable(FALSE);
	m_MotorGrid.SetDefCellHeight(30);
	m_MotorGrid.SetFixedColumnCount(1); 
	m_MotorGrid.SetFixedRowCount(1);
	m_MotorGrid.SetRowCount(8);
	m_MotorGrid.SetColumnCount(10);
	m_MotorGrid.SetRowResize(FALSE);
	m_MotorGrid.SetColumnResize(FALSE);		
	m_MotorGrid.SetListMode(FALSE);
	m_MotorGrid.EnableSelection(FALSE);

	//Teach Grid 설정
	m_TeachGrid.SetEditable(FALSE);
	m_TeachGrid.SetDefCellHeight(30);
	m_TeachGrid.SetFixedRowCount(1);
	m_TeachGrid.SetRowCount(7);
	m_TeachGrid.SetColumnCount(5);
	m_TeachGrid.SetRowResize(FALSE);
	m_TeachGrid.SetColumnResize(FALSE);
	m_TeachGrid.EnableSelection(FALSE);

	//Motor간격 설정
	m_MotorGrid.SetColumnWidth(0, 25); 
	m_MotorGrid.SetColumnWidth(1, 204);	
	m_MotorGrid.SetColumnWidth(2, 49);	
	m_MotorGrid.SetColumnWidth(3, 49);	
	m_MotorGrid.SetColumnWidth(4, 49);	
	m_MotorGrid.SetColumnWidth(5, 49);	
	m_MotorGrid.SetColumnWidth(6, 49);	
	m_MotorGrid.SetColumnWidth(7, 49);	
	m_MotorGrid.SetColumnWidth(8, 49);	
	m_MotorGrid.SetColumnWidth(9, 73);	

	//Teach 간격 설정
	m_TeachGrid.SetColumnWidth(0, 203);
	m_TeachGrid.SetColumnWidth(1, 95);
	m_TeachGrid.SetColumnWidth(2, 95);
	m_TeachGrid.SetColumnWidth(3, 70);
	m_TeachGrid.SetColumnWidth(4, 70);



	//Motor 이름설정
	m_MotorGrid.SetItemText(0,0,_T("NO"));		
	m_MotorGrid.SetItemText(0,1,_T("Name"));	
	m_MotorGrid.SetItemText(0,2,_T("Ready"));	
	m_MotorGrid.SetItemText(0,3,_T("Servo"));	
	m_MotorGrid.SetItemText(0,4,_T("Alarm"));	
	m_MotorGrid.SetItemText(0,5,_T("Origin"));	
	m_MotorGrid.SetItemText(0,6,_T("-Limit"));	
	m_MotorGrid.SetItemText(0,7,_T("Home"));	
	m_MotorGrid.SetItemText(0,8,_T("+Limit"));	
	m_MotorGrid.SetItemText(0,9,_T("Torque"));

	//Teach 이름설정
	m_TeachGrid.SetItemText(0,0,_T("Name"));
	m_TeachGrid.SetItemText(0,1,_T("Position"));
	m_TeachGrid.SetItemText(0,2,_T("Speed"));
	m_TeachGrid.SetItemText(0,3,_T("Acc"));
	m_TeachGrid.SetItemText(0,4,_T("Dec"));

	// Font & Style 설정
	DWORD dwTextStyle = DT_RIGHT|DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_SINGLELINE;
	LOGFONT lf;
	CFont* pGridFont = m_MotorGrid.GetFont();
	CFont* tGridFont = m_TeachGrid.GetFont();
	pGridFont->GetLogFont(&lf);
	tGridFont->GetLogFont(&lf);

	memcpy(lf.lfFaceName, _T("굴림체"),sizeof(_T("굴림체")));
	lf.lfHeight = 12;
	lf.lfWeight = FW_BOLD;

	for(int i = 0; i < m_MotorGrid.GetRowCount(); i++)
	{	
		for(int j = 0; j<m_MotorGrid.GetColumnCount(); j++)						
		{
			m_MotorGrid.SetItemFont(i, j, &lf);
			m_MotorGrid.SetItemFormat(i, j, dwTextStyle);

			if ( i == 0 )				m_MotorGrid.SetRowHeight(i, 35);
			else						m_MotorGrid.SetRowHeight(i, 50);

			m_MotorGrid.SetItemFont(i, j, &lf);
			m_MotorGrid.SetItemFormat(i, j, dwTextStyle);
		}

	}

	for(int tRow = 0; tRow<m_TeachGrid.GetRowCount(); tRow++)
	{
		for(int tCol = 0; tCol<m_TeachGrid.GetColumnCount(); tCol++)
		{
			m_TeachGrid.SetItemFont(tRow, tCol, &lf);
			m_TeachGrid.SetItemFormat(tRow, tCol, dwTextStyle);

			if ( tRow == 0 )
				m_TeachGrid.SetRowHeight(tRow, 46);
			else
				m_TeachGrid.SetRowHeight(tRow, 40);
		}
	}


	m_MotorGrid.Refresh();
	m_TeachGrid.Refresh();

}

void CFormMotorUnitTest::HighlightCurAxis( BOOL bRefrash )
{
	// 이전 선택한 Row를 지우고 새로 클릭한 Row를 색칠한다
	if( (m_nLastMotorGridRow >0) && (m_nLastMotorGridRow < m_MotorGrid.GetRowCount()) )
		m_MotorGrid.SetItemBkColour(m_nLastMotorGridRow, 1, Color(Color::White).ToCOLORREF());

	int nNewRow = (int)m_CurAxis - m_iUpDownParam +1;
	
	if( (m_CurAxis != AXIS_ID_MAX) && (m_CurAxis >= m_iUpDownParam) && (nNewRow < m_MotorGrid.GetRowCount()) )
	{
		m_MotorGrid.SetItemBkColour(nNewRow, 1, Color(Color::LightBlue).ToCOLORREF());
	}
	m_nLastMotorGridRow = nNewRow;
	if(bRefrash)	m_MotorGrid.Refresh();
}

void CFormMotorUnitTest::UpdateMotorGrid()
{
	CAxisParamBank* pParamBank = &theAxisBank;		//	BankData 가져옴	

	CString snum;
	int nAxisID;

	for(int i = 0; i < m_MotorGrid.GetRowCount(); i++)
	{	
		if ( i != 0 )														// 2번째 줄부터
		{
			snum.Format(_T("%d"),i+m_iUpDownParam);
			m_MotorGrid.SetItemText(i,0,(_T("%d"),snum));	

			nAxisID = i-1 + m_iUpDownParam;
			if(nAxisID < AXIS_ID_MAX)
				m_MotorGrid.SetItemText(i,1, (_T("%s"),pParamBank->m_Axis[nAxisID].m_strAxisName));
			else
				m_MotorGrid.SetItemText(i,1, _T(""));
		}
	}

	HighlightCurAxis(FALSE);
	m_MotorGrid.Refresh();
}

void CFormMotorUnitTest::HighlightCurTeach( BOOL bRefrash )
{
	// 이전 선택한 Row를 지우고 새로 클릭한 Row를 색칠한다
	if( (m_nLastTeachGridRow > 0) && (m_nLastTeachGridRow < m_TeachGrid.GetRowCount()) )
		m_TeachGrid.SetItemBkColour(m_nLastTeachGridRow, 0, Color(Color::White).ToCOLORREF());

	int nNewRow = m_nCurTeachRow;
	if( (nNewRow > 0) && (nNewRow < m_TeachGrid.GetRowCount()) )
	{
		m_TeachGrid.SetItemBkColour(nNewRow, 0, Color(Color::LightBlue).ToCOLORREF());
	}
	m_nLastTeachGridRow = nNewRow;
	if(bRefrash)	m_TeachGrid.Refresh();
}

void CFormMotorUnitTest::UpdateTeachGrid()
{
	CAxisParam pAxisData;
	if (!theAxisBank.GetParamNode(m_CurAxis, pAxisData) )
		return;

	m_arrTeach.RemoveAll();

	//2017-03-22,skcho, 런중인 레시피의 티칭값 불러오게,프로그램 시작당시 레시피 티칭값만 표시되어 있었음.
	theTeachBank.ReadTeachFile(theRecipeBank.m_strRecipeName);
	
	theTeachBank.GetTeachParamList((int)m_CurAxis, m_arrTeach);

	if ( m_arrTeach.GetSize() != 0 )
	{
		CString szPos,szSpeed, szAcc, szDec;
		for(int i =0; i < m_arrTeach.GetSize(); i++ )
		{
			szPos.Format(_T("%.3f"), m_arrTeach.GetAt(i).dPos);
			szSpeed.Format(_T("%.1f"), m_arrTeach.GetAt(i).dSpeed);
			szAcc.Format(_T("%d"), (int)(1000./m_arrTeach.GetAt(i).dAcc));
			szDec.Format(_T("%d"), (int)(1000./m_arrTeach.GetAt(i).dDec));

			m_TeachGrid.SetItemText(i+1,0, m_arrTeach.GetAt(i).strName);
			m_TeachGrid.SetItemText(i+1,1, szPos);
			m_TeachGrid.SetItemText(i+1,2, szSpeed);
			m_TeachGrid.SetItemText(i+1,3, szAcc);
			m_TeachGrid.SetItemText(i+1,4, szDec);
		}
	}

	m_TeachGrid.Refresh();
}

void CFormMotorUnitTest::CurAxisChange( AXIS_ID axis )
{
	m_TeachGrid.SetEditable(TRUE);							//	Teach Grid Clear해줌
	CCellRange cRange;
	cRange.Set(1,0,6,4);
	m_TeachGrid.ClearCells(cRange);
	m_TeachGrid.SetEditable(FALSE);		
	m_nCurTeachRow = -1;
	HighlightCurTeach(FALSE);

	m_CurAxis = axis;
	m_JogSpeed = JOG_LOW;	// 축이 바뀌면 조그 속도를 저속으로 바꾼다

	// 이전 선택한 Row를 지우고 새로 클릭한 Row를 색칠한다
	HighlightCurAxis(TRUE);

	UpdateMotorParam();

	UpdateTeachGrid();

}

void CFormMotorUnitTest::UpdateMotorListState()
{
	// Grid에 표시된 축 상태를 표시
	int nRowMax = m_MotorGrid.GetRowCount();
	CString strNumber;
	AXIS_ID axis;
	AxisStatus status;
	CString strTourqe_Grid;
	CString strTourqe_Axis;

	for(int i = 1; i < nRowMax; i++)
	{
		strNumber = m_MotorGrid.GetItemText(i, 0);
		axis = (AXIS_ID)(_ttoi(strNumber) - 1);
		if(axis < AXIS_ID_MAX)
		{
			status = theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, axis);

			if( (status.bBusy == FALSE) && (m_MotorGrid.GetItemBkColour(i, 2) !=Color(Color::Lime).ToCOLORREF()) )
				m_MotorGrid.SetItemBkColour(i, 2, Color(Color::Lime).ToCOLORREF());
			else if( status.bBusy && (m_MotorGrid.GetItemBkColour(i, 2) == Color(Color::Lime).ToCOLORREF()) )
				m_MotorGrid.SetItemBkColour(i, 2, Color(Color::White).ToCOLORREF());

			if(axis == AXIS_SVO_TURN_TABLE_T)
			{
				if( (status.bServoOn == FALSE) && (m_MotorGrid.GetItemBkColour(i, 3) !=Color(Color::Lime).ToCOLORREF()) )
					m_MotorGrid.SetItemBkColour(i, 3, Color(Color::Lime).ToCOLORREF());
				else if( status.bServoOn && (m_MotorGrid.GetItemBkColour(i, 3) == Color(Color::Lime).ToCOLORREF()) )
					m_MotorGrid.SetItemBkColour(i, 3, Color(Color::White).ToCOLORREF());
			}
			else
			{
				if( status.bServoOn && (m_MotorGrid.GetItemBkColour(i, 3) !=Color(Color::Lime).ToCOLORREF()) )
					m_MotorGrid.SetItemBkColour(i, 3, Color(Color::Lime).ToCOLORREF());
				else if( (status.bServoOn == FALSE) && (m_MotorGrid.GetItemBkColour(i, 3) == Color(Color::Lime).ToCOLORREF()) )
					m_MotorGrid.SetItemBkColour(i, 3, Color(Color::White).ToCOLORREF());
			}

			if( status.bAlarmOn && (m_MotorGrid.GetItemBkColour(i, 4) !=Color(Color::Red).ToCOLORREF()) )
				m_MotorGrid.SetItemBkColour(i, 4, Color(Color::Red).ToCOLORREF());
			else if( (status.bAlarmOn == FALSE) && (m_MotorGrid.GetItemBkColour(i, 4) == Color(Color::Red).ToCOLORREF()) )
				m_MotorGrid.SetItemBkColour(i, 4, Color(Color::White).ToCOLORREF());

			if( status.bOriginComplete && (m_MotorGrid.GetItemBkColour(i, 5) !=Color(Color::Lime).ToCOLORREF()) )
				m_MotorGrid.SetItemBkColour(i, 5, Color(Color::Lime).ToCOLORREF());
			else if( (status.bOriginComplete == FALSE) && (m_MotorGrid.GetItemBkColour(i, 5) == Color(Color::Lime).ToCOLORREF()) )
				m_MotorGrid.SetItemBkColour(i, 5, Color(Color::White).ToCOLORREF());

			if( status.bNLimitOn && (m_MotorGrid.GetItemBkColour(i, 6) !=Color(Color::Red).ToCOLORREF()) )
				m_MotorGrid.SetItemBkColour(i, 6, Color(Color::Red).ToCOLORREF());
			else if( (status.bNLimitOn == FALSE) && (m_MotorGrid.GetItemBkColour(i, 6) == Color(Color::Red).ToCOLORREF()) )
				m_MotorGrid.SetItemBkColour(i, 6, Color(Color::White).ToCOLORREF());

			if( status.bHomeOn && (m_MotorGrid.GetItemBkColour(i, 7) !=Color(Color::Lime).ToCOLORREF()) )
				m_MotorGrid.SetItemBkColour(i, 7, Color(Color::Lime).ToCOLORREF());
			else if( (status.bHomeOn == FALSE) && (m_MotorGrid.GetItemBkColour(i, 7) == Color(Color::Lime).ToCOLORREF()) )
				m_MotorGrid.SetItemBkColour(i, 7, Color(Color::White).ToCOLORREF());

			if( status.bPLimitOn && (m_MotorGrid.GetItemBkColour(i, 8) !=Color(Color::Red).ToCOLORREF()) )
				m_MotorGrid.SetItemBkColour(i, 8, Color(Color::Red).ToCOLORREF());
			else if( (status.bPLimitOn == FALSE) && (m_MotorGrid.GetItemBkColour(i, 8) == Color(Color::Red).ToCOLORREF()) )
				m_MotorGrid.SetItemBkColour(i, 8, Color(Color::White).ToCOLORREF());

			strTourqe_Axis.Format(_T("%6.2f"), theDeviceMotion.GetTorque(MP2100_THREAD_MAIN_PROCESS, axis));
			strTourqe_Grid = m_MotorGrid.GetItemText(i, 9);
			if( strTourqe_Axis != strTourqe_Grid )
				m_MotorGrid.SetItemText(i, 9, strTourqe_Axis);
		}
	}
	m_MotorGrid.Refresh();
}

void CFormMotorUnitTest::UpdateMotorState()
{
	// 선택된 한 축의 파라미터 및 상태 표시
	if(m_CurAxis == AXIS_ID_MAX)
		return;

	// OnInitialUpdate에서 한번만 하고 끝내고 싶은데 OnInitialUpdate 이후에 hWnd가 또 바뀌어서 일일이 해야 한다... -_-
	CGxStaticEx* pCurPosition = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_SETUP4_CUR_POSITION);

	double dValue = 0.;
	theDeviceMotion.GetPosition(MP2100_THREAD_MAIN_PROCESS, m_CurAxis, dValue);
	pCurPosition->SetCaption(CStringSupport::N2C(dValue));


	CGxButtonEx* pSpeedJogHighCheck = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_SPEED_JOG_HIGH_CHECK);
	CGxButtonEx* pSpeedJogMidCheck = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_SPEED_JOG_MID_CHECK);
	CGxButtonEx* pSpeedJogLowCheck = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_SPEED_JOG_LOW_CHECK);

	if( (m_JogSpeed == JOG_HIGH) && (pSpeedJogHighCheck->GetBackColor() != Color(Color::Lime).ToCOLORREF()) )
		pSpeedJogHighCheck->SetBackColor(Color(Color::Lime).ToCOLORREF());
	else if( (m_JogSpeed != JOG_HIGH) && (pSpeedJogHighCheck->GetBackColor() == Color(Color::Lime).ToCOLORREF()) )
		pSpeedJogHighCheck->SetBackColor(Color(Color::White).ToCOLORREF());

	if( (m_JogSpeed == JOG_MID) && (pSpeedJogMidCheck->GetBackColor() != Color(Color::Lime).ToCOLORREF()) )
		pSpeedJogMidCheck->SetBackColor(Color(Color::Lime).ToCOLORREF());
	else if( (m_JogSpeed != JOG_MID) && (pSpeedJogMidCheck->GetBackColor() == Color(Color::Lime).ToCOLORREF()) )
		pSpeedJogMidCheck->SetBackColor(Color(Color::White).ToCOLORREF());

	if( (m_JogSpeed == JOG_LOW) && (pSpeedJogLowCheck->GetBackColor() != Color(Color::Lime).ToCOLORREF()) )
		pSpeedJogLowCheck->SetBackColor(Color(Color::Lime).ToCOLORREF());
	else if( (m_JogSpeed != JOG_LOW) && (pSpeedJogLowCheck->GetBackColor() == Color(Color::Lime).ToCOLORREF()) )
		pSpeedJogLowCheck->SetBackColor(Color(Color::White).ToCOLORREF());
}

// CFormSetup4 메시지 처리기입니다.

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
LRESULT CFormMotorUnitTest::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{
		UpdateMotorGrid();
		SetTimer(TIMER_MONITOR, 100, NULL);
	}
	else
	{
		KillTimer(TIMER_MONITOR);
	}

	return 0;
}

// 표시 언어가 변경되었다.
LRESULT CFormMotorUnitTest::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	CGxUICtrl::BroadcastStaticCpationIndex(this, wParam, IDC_GXSTC_SETUP4_CAP2);
	CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_GXBTN_SETUP4_SERVOON);

	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


// 윈도우 생성전에 모양을 달리 지정한다.
BOOL CFormMotorUnitTest::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	cs.style = WS_OVERLAPPED | WS_CHILD;				// 테두리를 없앤다.

	return CFormView::PreCreateWindow(cs);
}
BEGIN_EVENTSINK_MAP(CFormMotorUnitTest, CFormView)
	ON_EVENT(CFormMotorUnitTest, IDC_GXBTN_SETUP4_HALFUP, DISPID_CLICK, CFormMotorUnitTest::ClickGxbtnSetup4Halfup, VTS_NONE)
	ON_EVENT(CFormMotorUnitTest, IDC_GXBTN_SETUP4_HALFDOWN, DISPID_CLICK, CFormMotorUnitTest::ClickGxbtnSetup4Halfdown, VTS_NONE)
	ON_EVENT(CFormMotorUnitTest, IDC_GXBTN_SETUP4_UP, DISPID_CLICK, CFormMotorUnitTest::ClickGxbtnSetup4Up, VTS_NONE)
	ON_EVENT(CFormMotorUnitTest, IDC_GXBTN_SETUP4_DOWN, DISPID_CLICK, CFormMotorUnitTest::ClickGxbtnSetup4Down, VTS_NONE)
	ON_EVENT(CFormMotorUnitTest, IDC_GXBTN_SETUP4_SERVOON, DISPID_CLICK, CFormMotorUnitTest::ClickGxbtnSetup4Servoon, VTS_NONE)
	ON_EVENT(CFormMotorUnitTest, IDC_GXBTN_SETUP4_SERVOOFF, DISPID_CLICK, CFormMotorUnitTest::ClickGxbtnSetup4Servooff, VTS_NONE)
	ON_EVENT(CFormMotorUnitTest, IDC_GXBTN_SETUP4_RESET, DISPID_CLICK, CFormMotorUnitTest::ClickGxbtnSetup4Reset, VTS_NONE)
	ON_EVENT(CFormMotorUnitTest, IDC_GXBTN_SETUP4_ORIGIN, DISPID_CLICK, CFormMotorUnitTest::ClickGxbtnSetup4Origin, VTS_NONE)
	ON_EVENT(CFormMotorUnitTest, IDC_GXBTN_SETUP4_MOVE_STOP, DISPID_CLICK, CFormMotorUnitTest::ClickGxbtnSetup4MoveStop, VTS_NONE)
	ON_EVENT(CFormMotorUnitTest, IDC_GXBTN_SETUP4_SAVE, DISPID_CLICK, CFormMotorUnitTest::ClickGxbtnSetup4Save, VTS_NONE)
	ON_EVENT(CFormMotorUnitTest, IDC_GXBTN_SETUP4_TEACHMOVE, DISPID_CLICK, CFormMotorUnitTest::ClickGxbtnSetup4Teachmove, VTS_NONE)
	ON_EVENT(CFormMotorUnitTest, IDC_GXBTN_SETUP4_TEACHSAVE, DISPID_CLICK, CFormMotorUnitTest::ClickGxbtnSetup4Teachsave, VTS_NONE)
	ON_EVENT(CFormMotorUnitTest, IDC_GXBTN_SETUP4_SPEED_JOG_HIGH, DISPID_CLICK, CFormMotorUnitTest::ClickGxbtnSetup4SpeedJogHigh, VTS_NONE)
	ON_EVENT(CFormMotorUnitTest, IDC_GXBTN_SETUP4_SPEED_JOG_MID, DISPID_CLICK, CFormMotorUnitTest::ClickGxbtnSetup4SpeedJogMid, VTS_NONE)
	ON_EVENT(CFormMotorUnitTest, IDC_GXBTN_SETUP4_SPEED_JOG_LOW, DISPID_CLICK, CFormMotorUnitTest::ClickGxbtnSetup4SpeedJogLow, VTS_NONE)
	ON_EVENT(CFormMotorUnitTest, IDC_GXBTN_SETUP4_SPEED_MOVE, DISPID_CLICK, CFormMotorUnitTest::ClickGxbtnSetup4SpeedMove, VTS_NONE)
	ON_EVENT(CFormMotorUnitTest, IDC_GXBTN_SETUP4_HOMESPEED, DISPID_CLICK, CFormMotorUnitTest::ClickGxbtnSetup4Homespeed, VTS_NONE)
	ON_EVENT(CFormMotorUnitTest, IDC_GXBTN_SETUP4_PLIMIT, DISPID_CLICK, CFormMotorUnitTest::ClickGxbtnSetup4Plimit, VTS_NONE)
	ON_EVENT(CFormMotorUnitTest, IDC_GXBTN_SETUP4_NLIMIT, DISPID_CLICK, CFormMotorUnitTest::ClickGxbtnSetup4Nlimit, VTS_NONE)
	ON_EVENT(CFormMotorUnitTest, IDC_GXBTN_SETUP4_RATIO, DISPID_CLICK, CFormMotorUnitTest::ClickGxbtnSetup4Ratio, VTS_NONE)
	ON_EVENT(CFormMotorUnitTest, IDC_GXBTN_SETUP4_SPEED_JOG_HIGH_CHECK, DISPID_CLICK, CFormMotorUnitTest::ClickGxbtnSetup4SpeedJogHighCheck, VTS_NONE)
	ON_EVENT(CFormMotorUnitTest, IDC_GXBTN_SETUP4_SPEED_JOG_MID_CHECK, DISPID_CLICK, CFormMotorUnitTest::ClickGxbtnSetup4SpeedJogMidCheck, VTS_NONE)
	ON_EVENT(CFormMotorUnitTest, IDC_GXBTN_SETUP4_SPEED_JOG_LOW_CHECK, DISPID_CLICK, CFormMotorUnitTest::ClickGxbtnSetup4SpeedJogLowCheck, VTS_NONE)
	ON_EVENT(CFormMotorUnitTest, IDC_GXBTN_SETUP4_JOG_N, DISPID_MOUSEDOWN, CFormMotorUnitTest::MouseDownGxbtnSetup4JogN, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CFormMotorUnitTest, IDC_GXBTN_SETUP4_JOG_N, DISPID_MOUSEUP, CFormMotorUnitTest::MouseUpGxbtnSetup4JogN, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CFormMotorUnitTest, IDC_GXBTN_SETUP4_JOG_P, DISPID_MOUSEDOWN, CFormMotorUnitTest::MouseDownGxbtnSetup4JogP, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CFormMotorUnitTest, IDC_GXBTN_SETUP4_JOG_P, DISPID_MOUSEUP, CFormMotorUnitTest::MouseUpGxbtnSetup4JogP, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CFormMotorUnitTest, IDC_GXBTN_SETUP4_POS_INC, DISPID_CLICK, CFormMotorUnitTest::ClickGxbtnSetup4PosInc, VTS_NONE)
	ON_EVENT(CFormMotorUnitTest, IDC_GXBTN_SETUP4_POS_ABS, DISPID_CLICK, CFormMotorUnitTest::ClickGxbtnSetup4PosAbs, VTS_NONE)
	ON_EVENT(CFormMotorUnitTest, IDC_GXBTN_SETUP4_INC_MOVE_N, DISPID_CLICK, CFormMotorUnitTest::ClickGxbtnSetup4IncMoveN, VTS_NONE)
	ON_EVENT(CFormMotorUnitTest, IDC_GXBTN_SETUP4_INC_MOVE_P, DISPID_CLICK, CFormMotorUnitTest::ClickGxbtnSetup4IncMoveP, VTS_NONE)
	ON_EVENT(CFormMotorUnitTest, IDC_GXBTN_SETUP4_ABS_MOVE, DISPID_CLICK, CFormMotorUnitTest::ClickGxbtnSetup4AbsMove, VTS_NONE)
	ON_EVENT(CFormMotorUnitTest, IDC_GXBTN_SETUP4_MOVE_ALLSTOP, DISPID_CLICK, CFormMotorUnitTest::ClickGxbtnSetup4MoveAllstop, VTS_NONE)
END_EVENTSINK_MAP()


void CFormMotorUnitTest::ClickGxbtnSetup4Halfup()
{
	if ( m_iUpDownParam - 3  < 0 )		
		m_iUpDownParam = 0;
	else
		m_iUpDownParam -= 3;

	UpdateMotorGrid();
}


void CFormMotorUnitTest::ClickGxbtnSetup4Halfdown()
{
	if ( m_iUpDownParam + 3  >= AXIS_ID_MAX )		
		return;

	m_iUpDownParam += 3;

	UpdateMotorGrid();
}


void CFormMotorUnitTest::ClickGxbtnSetup4Up()
{
	if ( m_iUpDownParam - 7  < 0 )		
		m_iUpDownParam = 0;
	else
		m_iUpDownParam -= 7;

	UpdateMotorGrid();
}


void CFormMotorUnitTest::ClickGxbtnSetup4Down()
{
	if ( m_iUpDownParam + 7  >= AXIS_ID_MAX )		
		return;
	
	m_iUpDownParam += 7;

	UpdateMotorGrid();
}

void CFormMotorUnitTest::SelectedAxis(int iSel, int iSel2)
{
	if ( m_TeachGrid.GetSafeHwnd() == NULL )		return;
	if ( m_MotorGrid.GetSafeHwnd() == NULL )		return;
	
	m_CurAxis = (AXIS_ID)iSel;

	if ( iSel > 7 )		
	{		
		m_iUpDownParam = 0;
		m_iUpDownParam += iSel;
		m_MotorGrid.SetSelectedRange(1,1,1,1);
	}
	else if(iSel >0)
	{
		m_iUpDownParam = 1;
		m_MotorGrid.SetSelectedRange(iSel,1,iSel,1);
	}
	else
	{
		m_MotorGrid.SetSelectedRange(iSel,1,iSel,1);
	}

	HighlightCurAxis(FALSE);
	UpdateMotorGrid();

	m_TeachGrid.SetSelectedRange(iSel2+1,0,iSel2+1,0);
	
	CurAxisChange(m_CurAxis);
	
}

void CFormMotorUnitTest::OnGridMoterEndSelChange(NMHDR *pNotifyStruct, LRESULT *pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;	

	if ( pItem->iRow < 1 )		return;				// 메뉴 정의된 Row Click시 return	

	
	// - 모터 Bank에서 일일이 검색할 시 속도문제가 발생하기 때문에
	// - 현재 select된 Grid의 No값을 가져와서 해당 No값의 Axis를 가져온다.

	CString szClickedNumber = m_MotorGrid.GetItemText(pItem->iRow,0);		//	클릭된 메뉴의 NO 값을 가져와서 int로 변경
	int iNum = _ttoi(szClickedNumber) - 1;

	// 현재 선택된 축 갱신
	if ( iNum >= AXIS_ID_MAX)		return;

	CurAxisChange((AXIS_ID)iNum);

	*pResult = 0;
}

void CFormMotorUnitTest::OnGridTeachEndSelChange( NMHDR *pNotifyStruct, LRESULT *pResult )
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;	

	if ( pItem->iRow < 1 )		return;				// 메뉴 정의된 Row Click시 return	
	m_nCurTeachRow = pItem->iRow;
	// 이전 선택한 Row를 지우고 새로 클릭한 Row를 색칠한다
	HighlightCurTeach(FALSE);

	if(pItem->iColumn == 1)
	{
		//////////////////////////////////////////////////////////////////////////
		// Position
		// Origin이 안되면 티칭값 저장 불가 [11/1/2016 OSC]
		if(theDeviceMotion.IsAxisReady(MP2100_THREAD_MAIN_PROCESS, m_CurAxis, FALSE) == FALSE)
		{
			CGxMsgBox	dlgMsgBox;
			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("ORIGIN을 먼저 실행해주세요."), 
				_T("Please run ORIGIN first"),		
				_T("Xin vui lòng chạy ORIGIN đầu tiên") );

			dlgMsgBox.DoModal();
		}
		else
		{
			CString strOldValue = m_TeachGrid.GetItemText(pItem->iRow, pItem->iColumn);
			CGxNumPadEx	dlgNumPad;

			dlgNumPad.SetTitle(_T("Teach Position Change"));
			dlgNumPad.SetValue(-1000., 1000., _tstof(strOldValue));

			int nRet = dlgNumPad.DoModal();

			// 정상적인 입력을 하였다.
			if ( nRet == IDOK )
			{
				m_TeachGrid.SetItemText(pItem->iRow, pItem->iColumn, dlgNumPad.GetValue());
			}
		}
	}
	if(pItem->iColumn == 2)
	{
		//////////////////////////////////////////////////////////////////////////
		// Speed
		CString strOldValue = m_TeachGrid.GetItemText(pItem->iRow, pItem->iColumn);
		CGxNumPadEx	dlgNumPad;

		dlgNumPad.SetTitle(_T("Teach Speed Change"));
		dlgNumPad.SetValue(0., 5000., _tstof(strOldValue));

		int nRet = dlgNumPad.DoModal();
		//2018-03-03,SKCHO,PASSWORD 다이얼로그 추가
		//CDlgBoxKeyBoard dlgKey;
		CDlgBoxPassWord dlgKey;
		
		//2017-11-17, YMG 모터 스피드 변경 시 비밀번호 추가.
		
		// 정상적인 입력을 하였다.
		if ( nRet == IDOK )
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
				m_TeachGrid.SetItemText(pItem->iRow, pItem->iColumn, dlgNumPad.GetValue());
			}
			//DlgKey cancel 버튼 누를 시 
			else
				return;
		}
		else 
			return;
	}
	m_TeachGrid.Refresh();
}

void CFormMotorUnitTest::EnableButtons(BOOL bVal)
{
	CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_SETUP4_SERVOON, bVal);
	CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_SETUP4_SERVOOFF, bVal);
	CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_SETUP4_RESET, bVal);
	CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_SETUP4_ORIGIN, bVal);
	CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_SETUP4_INC_MOVE_N, bVal);
	CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_SETUP4_INC_MOVE_P, bVal);
	CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_SETUP4_JOG_N, bVal);
	CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_SETUP4_JOG_P, bVal);
	CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_SETUP4_ABS_MOVE, bVal);
	CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_SETUP4_TEACHMOVE, bVal);
	CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_SETUP4_TEACHSAVE, bVal);
	CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_SETUP4_SAVE, bVal);
	CGxUICtrl::SetButtonEnabled(this, IDC_GXSTC_SETUP4_CUR_POSITION, bVal);
	CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_SETUP4_POS_INC, bVal);
	CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_SETUP4_POS_ABS, bVal);
	CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_SETUP4_SPEED_JOG_HIGH, bVal);
	CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_SETUP4_SPEED_JOG_MID, bVal);
	CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_SETUP4_SPEED_JOG_LOW, bVal);
	CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_SETUP4_SPEED_MOVE, bVal);
	CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_SETUP4_USED, bVal);
	CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_SETUP4_HOMESPEED, bVal);
	CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_SETUP4_PLIMIT, bVal);
	CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_SETUP4_NLIMIT, bVal);
	CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_SETUP4_RATIO, bVal);
	CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_SETUP4_MODULE, bVal);
	CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_SETUP4_INNER, bVal);
}

void CFormMotorUnitTest::OnInitialUpdate()
{
	// 중복실행방지 [10/24/2016 OSC]
	if(m_bInit)
		return;
	m_bInit = TRUE;
	CFormView::OnInitialUpdate();

	InitializeGrid();

	CurAxisChange(AXIS_SVO_LOAD_CONV);

}


void CFormMotorUnitTest::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
	case TIMER_MONITOR:
		KillTimer(nIDEvent);
		UpdateMotorListState();
		UpdateMotorState();
		// Setup 탭에서 모터 원점 잡는 도중에 모터 열림 체크 2017/06/08 HSK.
		if(DoorOpenChk() == FALSE)
		{
			AxisStatus status = theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, m_CurAxis);
			if(!status.bOriginComplete && theProcBank.m_bOriginAxisChk)
			{
				theDeviceMotion.AxisStop(MP2100_THREAD_MAIN_PROCESS, m_CurAxis);

				CGxMsgBox	dlgMsgBox;

				dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
				dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
				dlgMsgBox.SetMessage(FALSE, 
					_T("door가 열렸습니다. "),	//한
					_T("door opened."),			//영
					_T("cửa được mở.") );			//베

				dlgMsgBox.DoModal();

				theProcBank.m_bOriginAxisChk = FALSE;
			}
		}
		SetTimer(TIMER_MONITOR, 100, NULL);
		break;
	}

	CFormView::OnTimer(nIDEvent);
}


void CFormMotorUnitTest::ClickGxbtnSetup4Servoon()
{
	theDeviceMotion.ServoOn(MP2100_THREAD_MAIN_PROCESS, m_CurAxis);
}


void CFormMotorUnitTest::ClickGxbtnSetup4Servooff()
{
	theDeviceMotion.ServoOff(MP2100_THREAD_MAIN_PROCESS, m_CurAxis);
}


void CFormMotorUnitTest::ClickGxbtnSetup4Reset()
{
	theDeviceMotion.ResetAlarm(MP2100_THREAD_MAIN_PROCESS, m_CurAxis);
}


void CFormMotorUnitTest::ClickGxbtnSetup4Origin()
{
	if(theUnitFunc.DoorInterLockChkStart() == FALSE)
		return ;

	if(theUnitFunc.CheckOriginInterlock(m_CurAxis))
	{
		theProcBank.m_bOriginAxisChk = TRUE;
		theDeviceMotion.OriginRetrun(MP2100_THREAD_MAIN_PROCESS, m_CurAxis);
	}
	else
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			theUnitFunc.m_strLastKorMsg, 
			theUnitFunc.m_strLastEngMsg,		
			theUnitFunc.m_strLastVnmMsg );

		int nRet = dlgMsgBox.DoModal();
		return;
	}
}


void CFormMotorUnitTest::ClickGxbtnSetup4MoveStop()
{
	theDeviceMotion.AxisStop(MP2100_THREAD_MAIN_PROCESS, m_CurAxis);
}


void CFormMotorUnitTest::ClickGxbtnSetup4Save()
{
	// 선택된 한 축의 파라미터 및 상태 표시
	if(m_CurAxis == AXIS_ID_MAX)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("축을 선택해주세요."), 
			_T("Please Select Axis."),		
			_T("Vui lòng chọn Axis") );

		dlgMsgBox.DoModal();
		return;
	}

	CGxMsgBox	dlgMsgBox;

	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
	dlgMsgBox.SetMessage(TRUE, 
		_T("축설정값을 저장하시겠습니까?"), 
		_T("Do you want to save the axis settings?"),		
		_T("Bạn có muốn lưu các thiết lập axis?") );

	int nRet = dlgMsgBox.DoModal();
	if(nRet != IDOK)
		return;

	// OnInitialUpdate에서 한번만 하고 끝내고 싶은데 OnInitialUpdate 이후에 hWnd가 또 바뀌어서 일일이 해야 한다... -_-
	CGxStaticEx* pCurPosition = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_SETUP4_CUR_POSITION);
	CGxButtonEx* pSpeedJogHigh = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_SPEED_JOG_HIGH);
	CGxButtonEx* pSpeedJogMid = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_SPEED_JOG_MID);
	CGxButtonEx* pSpeedJogLow = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_SPEED_JOG_LOW);
	CGxButtonEx* pMoveSpeed = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_SPEED_MOVE);
// 	CGxButtonEx* pAcc = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_ACC);
// 	CGxButtonEx* pDec = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_DEC);
	CGxButtonEx* pHomeSpeed = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_HOMESPEED);
	CGxButtonEx* pPLimit = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_PLIMIT);
	CGxButtonEx* pNLimit = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_NLIMIT);
	CGxButtonEx* pRatio = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_RATIO);
	CGxButtonEx* pModuleNo = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_MODULE);
	CGxButtonEx* pInnerNo = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_INNER);

	CString strValue;

	strValue = pSpeedJogHigh->GetCaption();
	theAxisBank.m_Axis[m_CurAxis].m_dJogSpeedHigh = _tstof(strValue.GetString());

	strValue = pSpeedJogMid->GetCaption();
	theAxisBank.m_Axis[m_CurAxis].m_dJogSpeedMiddle = _tstof(strValue.GetString());

	strValue = pSpeedJogLow->GetCaption();
	theAxisBank.m_Axis[m_CurAxis].m_dJogSpeedLow = _tstof(strValue.GetString());

	strValue = pMoveSpeed->GetCaption();
	theAxisBank.m_Axis[m_CurAxis].m_dDefaultMoveSpeed = _tstof(strValue.GetString());

	strValue = pHomeSpeed->GetCaption();
	theAxisBank.m_Axis[m_CurAxis].m_dHomeSpeed = _tstof(strValue.GetString());

	strValue = pPLimit->GetCaption();
	theAxisBank.m_Axis[m_CurAxis].m_dPositiveLimit = _tstof(strValue.GetString());

	strValue = pNLimit->GetCaption();
	theAxisBank.m_Axis[m_CurAxis].m_dNegativeLimit = _tstof(strValue.GetString());

	strValue = pRatio->GetCaption();
	theAxisBank.m_Axis[m_CurAxis].m_dGearRatio = _tstof(strValue.GetString());

// 	strValue = pModuleNo->GetCaption();
// 	theAxisBank.m_Axis[m_CurAxis].m_nModuleNo = _tstoi(strValue.GetString());
// 
// 	strValue = pInnerNo->GetCaption();
// 	theAxisBank.m_Axis[m_CurAxis].m_nInnerNo = _tstoi(strValue.GetString());

	theAxisBank.SaveAxisParam();

	MotionSpeedSave();
}


void CFormMotorUnitTest::ClickGxbtnSetup4Teachmove()
{
	if(theUnitFunc.DoorInterLockChkStart() == FALSE)
		return ;

	// 뮤팅 동작 할 때 모터 이동 버튼은 전부 막아 버렸다 2017/06/08 HSK.
	if(theProcBank.m_bNGTrayMuteOn == TRUE || theProcBank.m_bGoodInTrayMuteOn == TRUE || theProcBank.m_bGoodOutTrayMuteOn == TRUE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE,
			_T("Mute Ramp가 켜져 있어, 모터를 구동 할 수 없습니다."), 
			_T("Mute Ramp On, Can not Motor Move"), 
			_T("Mute Ramp On, Can not Motor Move") );

		dlgMsgBox.DoModal();
		return;
	}

	CGxMsgBox	dlgMsgBox;
	int nRet;
	int nTeachIndex = m_nCurTeachRow -1;
	if( (nTeachIndex < 0) || (nTeachIndex > m_arrTeach.GetCount()-1) )
	{

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("TEACH값 중 하나를 선택해 주세요."), 
			_T("Please select one of the TEACH values."),		
			_T("Hãy chọn một trong các giá trị TEACH.") );

		nRet = dlgMsgBox.DoModal();
		return;
	}

	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
	dlgMsgBox.SetMessage(TRUE, 
		_T("선택한 위치로 이동하시겠습니까?"), 
		_T("Are you sure you want to go to the selected position?"),		
		_T("Bạn có chắc chắn muốn đi đến vị trí đã chọn?") );

	nRet = dlgMsgBox.DoModal();
	if(nRet != IDOK)
		return;

	TEACH_PARAM paramTeach = m_arrTeach.GetAt(nTeachIndex);
	AxisMoveParam paramMove;
	paramMove = paramTeach;

	if(theUnitFunc.CheckTeachMoveInterlock(paramTeach.idTeach))
	{
		theDeviceMotion.TeachMove(MP2100_THREAD_MAIN_PROCESS, paramTeach.idTeach);
	}
	else
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			theUnitFunc.m_strLastKorMsg, 
			theUnitFunc.m_strLastEngMsg,		
			theUnitFunc.m_strLastVnmMsg );

		nRet = dlgMsgBox.DoModal();
		return;
	}
}


void CFormMotorUnitTest::ClickGxbtnSetup4Teachsave()
{
	CGxMsgBox	dlgMsgBox;

	//2018-03-03,SKCHO,PASSWORD 다이얼로그 추가
	//CDlgBoxKeyBoard dlgKey;
	CDlgBoxPassWord dlgKey;

	if ( dlgKey.DoModal() == IDOK )
	{
		if (dlgKey.m_strReturnText != theConfigBank.m_System.m_strPassword)
		{
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("틀린 비밀번호 입니다."), 
				_T("Password Wrong"),		
				_T("Một mật khẩu sai.") );

			dlgMsgBox.DoModal();
			return;
		}
	}
	else
	{
		return;
	}

	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
	dlgMsgBox.SetMessage(TRUE, 
		_T("TEACH값을 저장하시겠습니까?"), 
		_T("Do you want to save the TEACH value?"),		
		_T("Bạn có muốn lưu các giá trị TEACH?") );

	int nRet = dlgMsgBox.DoModal();
	if(nRet != IDOK)
		return;

	if ( m_arrTeach.GetSize() != 0 )
	{
		TEACH_PARAM param;
		CString szPos, szSpeed, szAcc, szDec;
		for(int i =0; i < m_arrTeach.GetSize(); i++ )
		{
			param = m_arrTeach.GetAt(i);

			// 가감속은 일단 보류
			szPos = m_TeachGrid.GetItemText(i+1,1);
			szSpeed = m_TeachGrid.GetItemText(i+1,2);
// 			szAcc = m_TeachGrid.GetItemText(i+1,3);
// 			szDec = m_TeachGrid.GetItemText(i+1,4);

			param.dPos = _tstof(szPos.GetString());
			param.dSpeed = _tstof(szSpeed.GetString());
// 			param.dAcc = _tstof(szAcc.GetString());
// 			param.dDec = _tstof(szDec.GetString());

			m_arrTeach.SetAt(i, param);
		}
		// 혹시나 S/W Limit 등 체크할 것들이 있을 것을 대비해 최종 저장은 따로 For문을 돌린다

		for(int i =0; i < m_arrTeach.GetSize(); i++ )
		{
			param = m_arrTeach.GetAt(i);
			theTeachBank.SetTeachParam(param);
		}
		//20170201 byskcho
		//레시피 생성시  티칭값 생성관련 수정
		theTeachBank.SaveTeachFile(theRecipeBank.m_strRecipeName);
		//20170201 byskcho 레시피 생성 편집 
		// 티칭값을 RMS에 보고하므로 티칭값이 변경될 때마다 Recipe 변경 보고를 해야 한다 [12/1/2016 OSC]
		theSocketInterFace.SendCmdRecipeToDataPC(EQUIP_SET_PPID_MODIFY, theRecipeBank.m_strRecipeName);

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Notify"), _T("Notify"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("저장 완료."), 
			_T("Save completed."),		
			_T("Lưu hoàn thành.") );
		dlgMsgBox.DoModal();
	}
}


void CFormMotorUnitTest::ClickGxbtnSetup4SpeedJogHigh()
{
	CGxButtonEx* pButton = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_SPEED_JOG_HIGH);
	double dOldValue = _tstof(pButton->GetCaption());
	CGxNumPadEx	dlgNumPad;

	dlgNumPad.SetTitle(_T("Jog High Speed Change"));
	dlgNumPad.SetValue(0.0, 1000.0, dOldValue);

	int nRet = dlgNumPad.DoModal();

	// 정상적인 입력을 하였다.
	if ( nRet == IDOK )
	{
		pButton->SetCaption(dlgNumPad.GetValue());
	}
}


void CFormMotorUnitTest::ClickGxbtnSetup4SpeedJogMid()  
{
	CGxButtonEx* pButton = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_SPEED_JOG_MID);
	double dOldValue = _tstof(pButton->GetCaption());
	CGxNumPadEx	dlgNumPad;

	dlgNumPad.SetTitle(_T("Jog Mid Speed Change"));
	dlgNumPad.SetValue(0.0, 1000.0, dOldValue);

	int nRet = dlgNumPad.DoModal();

	// 정상적인 입력을 하였다.
	if ( nRet == IDOK )
	{
		pButton->SetCaption(dlgNumPad.GetValue());
	}
}


void CFormMotorUnitTest::ClickGxbtnSetup4SpeedJogLow()
{
	CGxButtonEx* pButton = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_SPEED_JOG_LOW);
	double dOldValue = _tstof(pButton->GetCaption());
	CGxNumPadEx	dlgNumPad;

	dlgNumPad.SetTitle(_T("Jog Low Speed Change"));
	dlgNumPad.SetValue(0.0, 1000.0, dOldValue);

	int nRet = dlgNumPad.DoModal();

	// 정상적인 입력을 하였다.
	if ( nRet == IDOK )
	{
		pButton->SetCaption(dlgNumPad.GetValue());
	}
}


void CFormMotorUnitTest::ClickGxbtnSetup4SpeedMove()
{
	CGxButtonEx* pButton = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_SPEED_MOVE);
	double dOldValue = _tstof(pButton->GetCaption());
	CGxNumPadEx	dlgNumPad;

	dlgNumPad.SetTitle(_T("Jog Move Speed Change"));
	dlgNumPad.SetValue(0.0, 1500.0, dOldValue);

	int nRet = dlgNumPad.DoModal();

	// 정상적인 입력을 하였다.
	if ( nRet == IDOK )
	{
		pButton->SetCaption(dlgNumPad.GetValue());
	}
}


void CFormMotorUnitTest::ClickGxbtnSetup4Homespeed()
{
	CGxButtonEx* pButton = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_HOMESPEED);
	double dOldValue = _tstof(pButton->GetCaption());
	CGxNumPadEx	dlgNumPad;

	dlgNumPad.SetTitle(_T("Jog Home Speed Change"));
	dlgNumPad.SetValue(0.0, 1000.0, dOldValue);

	int nRet = dlgNumPad.DoModal();

	// 정상적인 입력을 하였다.
	if ( nRet == IDOK )
	{
		pButton->SetCaption(dlgNumPad.GetValue());
	}
}


void CFormMotorUnitTest::ClickGxbtnSetup4Plimit()  
{
	CGxButtonEx* pButton = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_PLIMIT);
	double dOldValue = _tstof(pButton->GetCaption());
	CGxNumPadEx	dlgNumPad;

	dlgNumPad.SetTitle(_T("Jog Home Plimit Change"));
	dlgNumPad.SetValue(0.0, 100000.0, dOldValue);

	int nRet = dlgNumPad.DoModal();

	// 정상적인 입력을 하였다.
	if ( nRet == IDOK )
	{
		pButton->SetCaption(dlgNumPad.GetValue());
	}
}


void CFormMotorUnitTest::ClickGxbtnSetup4Nlimit()
{
	CGxButtonEx* pButton = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_NLIMIT);
	double dOldValue = _tstof(pButton->GetCaption());
	CGxNumPadEx	dlgNumPad;

	dlgNumPad.SetTitle(_T("Jog Home Nlimit Change"));
	dlgNumPad.SetValue(-1000000., 0., dOldValue);

	int nRet = dlgNumPad.DoModal();

	// 정상적인 입력을 하였다.
	if ( nRet == IDOK )
	{
		pButton->SetCaption(dlgNumPad.GetValue());
	}
}


void CFormMotorUnitTest::ClickGxbtnSetup4Ratio()
{
	CGxButtonEx* pButton = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_RATIO);
	double dOldValue = _tstof(pButton->GetCaption());
	CGxNumPadEx	dlgNumPad;

	dlgNumPad.SetTitle(_T("Jog Home Ratio Change"));
	dlgNumPad.SetValue(0.0, 100000.0, dOldValue);

	int nRet = dlgNumPad.DoModal();

	// 정상적인 입력을 하였다.
	if ( nRet == IDOK )
	{
		pButton->SetCaption(dlgNumPad.GetValue());
	}
}

void CFormMotorUnitTest::UpdateMotorParam()
{
	// 선택된 한 축의 파라미터 및 상태 표시
	if(m_CurAxis == AXIS_ID_MAX)
		return;

	// OnInitialUpdate에서 한번만 하고 끝내고 싶은데 OnInitialUpdate 이후에 hWnd가 또 바뀌어서 일일이 해야 한다... -_-
	CGxButtonEx* pSpeedJogHighValue = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_SPEED_JOG_HIGH);
	CGxButtonEx* pSpeedJogMidValue = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_SPEED_JOG_MID);
	CGxButtonEx* pSpeedJogLowValue = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_SPEED_JOG_LOW);
	CGxButtonEx* pMoveSpeed = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_SPEED_MOVE);
	// 	CGxButtonEx* pAcc = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_ACC);
	// 	CGxButtonEx* pDec = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_DEC);
	CGxButtonEx* pHomeSpeed = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_HOMESPEED);
	CGxButtonEx* pPLimit = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_PLIMIT);
	CGxButtonEx* pNLimit = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_NLIMIT);
	CGxButtonEx* pRatio = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_RATIO);
	CGxButtonEx* pModuleNo = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_MODULE);
	CGxButtonEx* pInnerNo = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_INNER);
	CGxButtonEx* pUsedAxis = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_USED);		//  [10/26/2016 WHLEE]
	
	double dValue = 0.;
	int nValue = 0;
	CString strValue;
	dValue = theAxisBank.m_Axis[m_CurAxis].m_dJogSpeedHigh;
	pSpeedJogHighValue->SetCaption(CStringSupport::N2C(dValue));

	dValue = theAxisBank.m_Axis[m_CurAxis].m_dJogSpeedMiddle;
	pSpeedJogMidValue->SetCaption(CStringSupport::N2C(dValue));

	dValue = theAxisBank.m_Axis[m_CurAxis].m_dJogSpeedLow;
	pSpeedJogLowValue->SetCaption(CStringSupport::N2C(dValue));

	dValue = theAxisBank.m_Axis[m_CurAxis].m_dDefaultMoveSpeed;
	pMoveSpeed->SetCaption(CStringSupport::N2C(dValue));

	// 	dValue = 250.;
	// 	pAcc->SetCaption(CStringSupport::N2C(dValue));
	// 
	// 	dValue = 250.;
	// 	pDec->SetCaption(CStringSupport::N2C(dValue));

	dValue = theAxisBank.m_Axis[m_CurAxis].m_dHomeSpeed;
	pHomeSpeed->SetCaption(CStringSupport::N2C(dValue));

	dValue = theAxisBank.m_Axis[m_CurAxis].m_dPositiveLimit;
	pPLimit->SetCaption(CStringSupport::N2C(dValue));

	dValue = theAxisBank.m_Axis[m_CurAxis].m_dNegativeLimit;
	pNLimit->SetCaption(CStringSupport::N2C(dValue));

	dValue = theAxisBank.m_Axis[m_CurAxis].m_dGearRatio;
	pRatio->SetCaption(CStringSupport::N2C(dValue));

	nValue = theAxisBank.m_Axis[m_CurAxis].m_nModuleNo;
	strValue.Format(_T("%X"), nValue+1);
	pModuleNo->SetCaption(strValue);

	nValue = theAxisBank.m_Axis[m_CurAxis].m_nInnerNo;
	if(theAxisBank.m_Axis[m_CurAxis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
	{
		int nNum1 = nValue/16;
		int nNum2 = (nValue)%16;
		nNum1 = nNum1 ? 3:1;
		nNum2 += 1;
		strValue.Format(_T("%d-%X(%d)"), nNum1, nNum2, nNum2);
	}
	else
		strValue.Format(_T("%d"), nValue+1);
	pInnerNo->SetCaption(strValue);


	// 축 정보가 초기화될 때(다른 축을 선택할 때) 절대이동, 상대이동 위치값을 초기화한다
	CGxButtonEx* pIncPos = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_POS_INC);
	CGxButtonEx* pAbsPos = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_POS_ABS);
	pIncPos->SetCaption(_T("0"));
	pAbsPos->SetCaption(_T("0"));


	
	if ( theAxisBank.m_Axis[m_CurAxis].m_bUse == TRUE )			//  [10/26/2016 WHLEE]
	{
		pUsedAxis->SetCaption(_T("USE"));
	}
	else
	{
		pUsedAxis->SetCaption(_T("NOT USE"));
	}


}


void CFormMotorUnitTest::ClickGxbtnSetup4SpeedJogHighCheck()
{
	m_JogSpeed = JOG_HIGH;
}


void CFormMotorUnitTest::ClickGxbtnSetup4SpeedJogMidCheck()
{
	m_JogSpeed = JOG_MID;
}


void CFormMotorUnitTest::ClickGxbtnSetup4SpeedJogLowCheck()
{
	m_JogSpeed = JOG_LOW;
}

BOOL CFormMotorUnitTest::PreTranslateMessage(MSG* pMsg)
{

	return CFormView::PreTranslateMessage(pMsg);
}


void CFormMotorUnitTest::MouseDownGxbtnSetup4JogN(short Button, short Shift, long x, long y)
{
	if(theUnitFunc.DoorInterLockChkStart() == FALSE)
		return ;

	// 뮤팅 동작 할 때 모터 이동 버튼은 전부 막아 버렸다 2017/06/08 HSK.			
	if(theProcBank.m_bNGTrayMuteOn == TRUE || theProcBank.m_bGoodInTrayMuteOn == TRUE || theProcBank.m_bGoodOutTrayMuteOn == TRUE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE,
			_T("Mute Ramp가 켜져 있어, 모터를 구동 할 수 없습니다."), 
			_T("Mute Ramp On, Can not Motor Move"), 
			_T("Mute Ramp On, Can not Motor Move") );

		dlgMsgBox.DoModal();
		return;
	}
	if(m_CurAxis == AXIS_ID_MAX)
		return;
	if(m_bMove)
		return;

	AxisMoveParam param;
	param.dAcc = ACC_300ms;
	param.dDec = ACC_300ms;
	if(m_JogSpeed == JOG_LOW)
		param.dSpeed = theAxisBank.m_Axis[m_CurAxis].m_dJogSpeedLow;
	else if(m_JogSpeed == JOG_MID)
		param.dSpeed = theAxisBank.m_Axis[m_CurAxis].m_dJogSpeedMiddle;
	else
		param.dSpeed = theAxisBank.m_Axis[m_CurAxis].m_dJogSpeedHigh;

	param.idAxis = m_CurAxis;
	theDeviceMotion.AxisMoveJog(MP2100_THREAD_MAIN_PROCESS, m_CurAxis, FALSE, param);
	m_bMove = TRUE;
}


void CFormMotorUnitTest::MouseUpGxbtnSetup4JogN(short Button, short Shift, long x, long y)
{
	theDeviceMotion.AxisStop(MP2100_THREAD_MAIN_PROCESS, m_CurAxis);
	m_bMove = FALSE;
}


void CFormMotorUnitTest::MouseDownGxbtnSetup4JogP(short Button, short Shift, long x, long y)
{
	if(theUnitFunc.DoorInterLockChkStart() == FALSE)
		return ;

	// 뮤팅 동작 할 때 모터 이동 버튼은 전부 막아 버렸다 2017/06/08 HSK.
	if(theProcBank.m_bNGTrayMuteOn == TRUE || theProcBank.m_bGoodInTrayMuteOn == TRUE || theProcBank.m_bGoodOutTrayMuteOn == TRUE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE,
			_T("Mute Ramp가 켜져 있어, 모터를 구동 할 수 없습니다."), 
			_T("Mute Ramp On, Can not Motor Move"), 
			_T("Mute Ramp On, Can not Motor Move") );

		dlgMsgBox.DoModal();
		return;
	}
	if(m_CurAxis == AXIS_ID_MAX)
		return;
	if(m_bMove)
		return;
	AxisMoveParam param;
	param.dAcc = ACC_300ms;
	param.dDec = ACC_300ms;
	if(m_JogSpeed == JOG_LOW)
		param.dSpeed = theAxisBank.m_Axis[m_CurAxis].m_dJogSpeedLow;
	else if(m_JogSpeed == JOG_MID)
		param.dSpeed = theAxisBank.m_Axis[m_CurAxis].m_dJogSpeedMiddle;
	else
		param.dSpeed = theAxisBank.m_Axis[m_CurAxis].m_dJogSpeedHigh;
	param.idAxis = m_CurAxis;
	theDeviceMotion.AxisMoveJog(MP2100_THREAD_MAIN_PROCESS, m_CurAxis, TRUE, param);
	m_bMove = TRUE;
}


void CFormMotorUnitTest::MouseUpGxbtnSetup4JogP(short Button, short Shift, long x, long y)
{
	theDeviceMotion.AxisStop(MP2100_THREAD_MAIN_PROCESS, m_CurAxis);
	m_bMove = FALSE;
}


void CFormMotorUnitTest::ClickGxbtnSetup4PosInc()
{
	CGxButtonEx* pButton = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_POS_INC);
	double dOldValue = _tstof(pButton->GetCaption());
	CGxNumPadEx	dlgNumPad;

	dlgNumPad.SetTitle(_T("INC Pich Change "));
	dlgNumPad.SetValue(0.0, 5000.0, dOldValue);

	int nRet = dlgNumPad.DoModal();

	// 정상적인 입력을 하였다.
	if ( nRet == IDOK )
	{
		pButton->SetCaption(dlgNumPad.GetValue());
	}
}


void CFormMotorUnitTest::ClickGxbtnSetup4PosAbs()
{
	CGxButtonEx* pButton = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_POS_ABS);
	double dOldValue = _tstof(pButton->GetCaption());
	CGxNumPadEx	dlgNumPad;

	dlgNumPad.SetTitle(_T("ABS Pich Change "));
	dlgNumPad.SetValue(-5000.0, 5000.0, dOldValue);

	int nRet = dlgNumPad.DoModal();

	// 정상적인 입력을 하였다.
	if ( nRet == IDOK )
	{
		pButton->SetCaption(dlgNumPad.GetValue());
	}
}


void CFormMotorUnitTest::ClickGxbtnSetup4IncMoveN()
{
	if(theUnitFunc.DoorInterLockChkStart() == FALSE)
		return ;

	// 뮤팅 동작 할 때 모터 이동 버튼은 전부 막아 버렸다 2017/06/08 HSK.
	if(theProcBank.m_bNGTrayMuteOn == TRUE || theProcBank.m_bGoodInTrayMuteOn == TRUE || theProcBank.m_bGoodOutTrayMuteOn == TRUE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE,
			_T("Mute Ramp가 켜져 있어, 모터를 구동 할 수 없습니다."), 
			_T("Mute Ramp On, Can not Motor Move"), 
			_T("Mute Ramp On, Can not Motor Move") );

		dlgMsgBox.DoModal();
		return;
	}
	CGxButtonEx* pButton = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_POS_INC);
	AxisMoveParam param;
	param.dAcc = ACC_300ms;
	param.dDec = ACC_300ms;
	param.dSpeed = theAxisBank.m_Axis[m_CurAxis].m_dDefaultMoveSpeed;
	param.idAxis = m_CurAxis;
	param.dTargetPosition = _tstof(pButton->GetCaption());
	param.dTargetPosition *= -1;
	theDeviceMotion.AxisMoveInc(MP2100_THREAD_MAIN_PROCESS, m_CurAxis, param);
}


void CFormMotorUnitTest::ClickGxbtnSetup4IncMoveP()
{
	if(theUnitFunc.DoorInterLockChkStart() == FALSE)
		return ;

	// 뮤팅 동작 할 때 모터 이동 버튼은 전부 막아 버렸다 2017/06/08 HSK.
	if(theProcBank.m_bNGTrayMuteOn == TRUE || theProcBank.m_bGoodInTrayMuteOn == TRUE || theProcBank.m_bGoodOutTrayMuteOn == TRUE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE,
			_T("Mute Ramp가 켜져 있어, 모터를 구동 할 수 없습니다."), 
			_T("Mute Ramp On, Can not Motor Move"), 
			_T("Mute Ramp On, Can not Motor Move") );

		dlgMsgBox.DoModal();
		return;
	}
	CGxButtonEx* pButton = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_POS_INC);
	AxisMoveParam param;
	param.dAcc = ACC_300ms;
	param.dDec = ACC_300ms;
	param.dSpeed = theAxisBank.m_Axis[m_CurAxis].m_dDefaultMoveSpeed;
	param.idAxis = m_CurAxis;
	param.dTargetPosition = _tstof(pButton->GetCaption());
	theDeviceMotion.AxisMoveInc(MP2100_THREAD_MAIN_PROCESS, m_CurAxis, param);
}


void CFormMotorUnitTest::ClickGxbtnSetup4AbsMove()
{
	if(theUnitFunc.DoorInterLockChkStart() == FALSE)
		return ;

	// 뮤팅 동작 할 때 모터 이동 버튼은 전부 막아 버렸다 2017/06/08 HSK.
	if(theProcBank.m_bNGTrayMuteOn == TRUE || theProcBank.m_bGoodInTrayMuteOn == TRUE || theProcBank.m_bGoodOutTrayMuteOn == TRUE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE,
			_T("Mute Ramp가 켜져 있어, 모터를 구동 할 수 없습니다."), 
			_T("Mute Ramp On, Can not Motor Move"), 
			_T("Mute Ramp On, Can not Motor Move") );

		dlgMsgBox.DoModal();
		return;
	}
	CGxButtonEx* pButton = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_POS_ABS);
	AxisMoveParam param;
	param.dAcc = ACC_300ms;
	param.dDec = ACC_300ms;
	param.dSpeed = theAxisBank.m_Axis[m_CurAxis].m_dDefaultMoveSpeed;
	param.idAxis = m_CurAxis;
	param.dTargetPosition = _tstof(pButton->GetCaption());
	theDeviceMotion.AxisMove(MP2100_THREAD_MAIN_PROCESS, m_CurAxis, param);
}

void CFormMotorUnitTest::MotionSpeedSave()
{
	if ( m_arrTeach.GetSize() != 0 )
	{
		TEACH_PARAM param;
		CString szPos, szSpeed, szAcc, szDec;
		for(int i =0; i < m_arrTeach.GetSize(); i++ )
		{
			param = m_arrTeach.GetAt(i);

			// 포지션 이동 속도는 축의 속도로 한다.
			CGxButtonEx* pMoveSpeed = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_SETUP4_SPEED_MOVE);
			szSpeed = pMoveSpeed->GetCaption();
			param.dSpeed  = _tstof(szSpeed.GetString());

			m_arrTeach.SetAt(i, param);
		}

		for(int i =0; i < m_arrTeach.GetSize(); i++ )
		{
			param = m_arrTeach.GetAt(i);
			theTeachBank.SetTeachParam(param);
		}

		//20170201 byskcho	
		//레시피 생성시  티칭값 생성관련 수정
		theTeachBank.SaveTeachFile(theRecipeBank.m_strRecipeName);
		//20170201 byskcho 레시피 생성 편집 
		// 티칭값을 RMS에 보고하므로 티칭값이 변경될 때마다 Recipe 변경 보고를 해야 한다
		theSocketInterFace.SendCmdRecipeToDataPC(EQUIP_SET_PPID_MODIFY, theRecipeBank.m_strRecipeName);
	}
}

void CFormMotorUnitTest::ClickGxbtnSetup4MoveAllstop()
{
	theUnitFunc.MotorAllStop();
}

BOOL CFormMotorUnitTest::DoorOpenChk()
{
	if(theUnitFunc.GetInPutIOCheck(X_LD_DOOR_SEN_1) || theUnitFunc.GetInPutIOCheck(X_LD_DOOR_SEN_2)
		|| theUnitFunc.GetInPutIOCheck(X_LD_DOOR_SEN_3) || theUnitFunc.GetInPutIOCheck(X_LD_DOOR_SEN_4)
		|| theUnitFunc.GetInPutIOCheck(X_LD_DOOR_SEN_5) || theUnitFunc.GetInPutIOCheck(X_LD_DOOR_SEN_6)
		|| theUnitFunc.GetInPutIOCheck(X_ULD_DOOR_SEN_1) || theUnitFunc.GetInPutIOCheck(X_ULD_DOOR_SEN_2)
		|| theUnitFunc.GetInPutIOCheck(X_ULD_DOOR_SEN_3) || theUnitFunc.GetInPutIOCheck(X_ULD_DOOR_SEN_4)
		|| theUnitFunc.GetInPutIOCheck(X_ULD_DOOR_SEN_5))
	{
		return FALSE;
	}

	return TRUE;
}
