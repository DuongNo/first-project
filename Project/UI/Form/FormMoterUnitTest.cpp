// FormSetup4.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FormMoterUnitTest.h"

#include "GUIDefine.h"

//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "BankGuild\AxisParamBank.h"
#include "BankGuild\TeachBank.h"
#include "CheetahApp.h"
#include "UI\GausGUI\GxStaticEx.h"
#include "UI\GausGUI\GxButtonEx.h"



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

IMPLEMENT_DYNCREATE(CFormMoterUnitTest, CFormView)

CFormMoterUnitTest::CFormMoterUnitTest()
	: CFormView(CFormMoterUnitTest::IDD)
{
	m_iUpDownParam = 0;
	m_bFirst = TRUE;
	m_nLastMotorGridRow = -1;
	m_CurAxis = AXIS_ID_MAX;
	m_bParamEditing = FALSE;
	m_nLastTeachGridRow = -1;
// 	m_CurTeach = TEACH_PARAM::PARAM_MAX;
}

CFormMoterUnitTest::~CFormMoterUnitTest()
{
}

void CFormMoterUnitTest::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_GridControl(pDX, IDC_GRID_SETUP4_MOTORLIST, m_MotorGrid); //해당하는 Grid리스트 설정.
	DDX_GridControl(pDX, IDC_GRID_SETUP4_TEACHLIST, m_TeachGrid);
}

BEGIN_MESSAGE_MAP(CFormMoterUnitTest, CFormView)

	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	ON_NOTIFY(NM_CLICK, IDC_GRID_SETUP4_MOTORLIST, OnGridMoterEndSelChange)
	ON_NOTIFY(NM_CLICK, IDC_GRID_SETUP4_TEACHLIST, OnGridTeachEndSelChange)

	ON_WM_TIMER()
END_MESSAGE_MAP()


// CFormSetup4 진단입니다.

#ifdef _DEBUG
void CFormMoterUnitTest::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormMoterUnitTest::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


void CFormMoterUnitTest::InitializeGrid()
{

	if(m_MotorGrid.GetSafeHwnd() == NULL) return;
	if(m_TeachGrid.GetSafeHwnd() == NULL) return;

	//Grid 설정
	m_MotorGrid.SetEditable(FALSE);
	m_MotorGrid.SetDefCellHeight(30);
	m_MotorGrid.SetFixedColumnCount(1); 
	m_MotorGrid.SetFixedRowCount(1);
	m_MotorGrid.SetRowCount(8);
	m_MotorGrid.SetColumnCount(9);
	m_MotorGrid.SetRowResize(FALSE);
	m_MotorGrid.SetColumnResize(FALSE);		
	m_MotorGrid.SetListMode(FALSE);
	m_MotorGrid.EnableSelection(FALSE);

	//Teach Grid 설정
	m_TeachGrid.SetEditable(FALSE);
	m_TeachGrid.SetDefCellHeight(30);
	m_TeachGrid.SetFixedRowCount(1);
	m_TeachGrid.SetRowCount(6);
	m_TeachGrid.SetColumnCount(5);
	m_TeachGrid.SetRowResize(FALSE);
	m_TeachGrid.SetColumnResize(FALSE);
	m_TeachGrid.EnableSelection(FALSE);

	//Motor간격 설정
	m_MotorGrid.SetColumnWidth(0, 25); 
	m_MotorGrid.SetColumnWidth(1, 204);	
	m_MotorGrid.SetColumnWidth(2, 57);	
	m_MotorGrid.SetColumnWidth(3, 57);	
	m_MotorGrid.SetColumnWidth(4, 57);	
	m_MotorGrid.SetColumnWidth(5, 57);	
	m_MotorGrid.SetColumnWidth(6, 57);	
	m_MotorGrid.SetColumnWidth(7, 57);	
	m_MotorGrid.SetColumnWidth(8, 73);	

	//Teach 간격 설정
	m_TeachGrid.SetColumnWidth(0, 203);
	m_TeachGrid.SetColumnWidth(1, 95);
	m_TeachGrid.SetColumnWidth(2, 95);
	m_TeachGrid.SetColumnWidth(3, 70);
	m_TeachGrid.SetColumnWidth(4, 70);



	//Motor 이름설정
	m_MotorGrid.SetItemText(0,0,_T("NO"));		
	m_MotorGrid.SetItemText(0,1,_T("Name"));	
	m_MotorGrid.SetItemText(0,2,_T("Servo"));	
	m_MotorGrid.SetItemText(0,3,_T("Alarm"));	
	m_MotorGrid.SetItemText(0,4,_T("ORIGIN"));	
	m_MotorGrid.SetItemText(0,5,_T("-Limt"));	
	m_MotorGrid.SetItemText(0,6,_T("Home"));	
	m_MotorGrid.SetItemText(0,7,_T("+Limt"));	
	m_MotorGrid.SetItemText(0,8,_T("Tourqe"));

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
	lf.lfHeight = 14;
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
				m_TeachGrid.SetRowHeight(tRow, 52);
		}
	}


	m_MotorGrid.Refresh();
	m_TeachGrid.Refresh();

}

void CFormMoterUnitTest::HighlightCurAxis( BOOL bRefrash )
{
	// 이전 선택한 Row를 지우고 새로 클릭한 Row를 색칠한다
	if(m_nLastMotorGridRow >= 0)
		m_MotorGrid.SetItemBkColour(m_nLastMotorGridRow+1, 1, Color(Color::White).ToCOLORREF());

	int nNewRow = (int)m_CurAxis - m_iUpDownParam;
	
	if( (m_CurAxis != AXIS_ID_MAX) && (m_CurAxis >= m_iUpDownParam) && (nNewRow < m_MotorGrid.GetRowCount()-1) )
	{
		m_MotorGrid.SetItemBkColour(nNewRow+1, 1, Color(Color::LightBlue).ToCOLORREF());
		m_nLastMotorGridRow = nNewRow;
	}
	if(bRefrash)	m_MotorGrid.Refresh();
}

void CFormMoterUnitTest::UpdateMotorGrid()
{
	CAxisParamBank* pParamBank = &theAxisBank;		//	BankData 가져옴	

	CString snum;

	for(int i = 0; i < m_MotorGrid.GetRowCount(); i++)
	{	
		if ( i != 0 )														// 2번째 줄부터
		{
			snum.Format(_T("%d"),i+m_iUpDownParam);
			m_MotorGrid.SetItemText(i,0,(_T("%d"),snum));	

			m_MotorGrid.SetItemText(i,1, (_T("%s"),pParamBank->m_Axis[i-1 + m_iUpDownParam].m_strAxisName));
		}
	}

	HighlightCurAxis(FALSE);
	m_MotorGrid.Refresh();
}

void CFormMoterUnitTest::HighlightCurTeach( BOOL bRefrash )
{
	// 이전 선택한 Row를 지우고 새로 클릭한 Row를 색칠한다
	if(m_nLastTeachGridRow >= 0)
		m_TeachGrid.SetItemBkColour(m_nLastTeachGridRow+1, 1, Color(Color::White).ToCOLORREF());

// 	int nNewRow = m_nCurTeachRow;
// 	if( (m_CurTeach != TEACH_PARAM::PARAM_MAX) && (nNewRow < m_TeachGrid.GetRowCount()) )
// 	{
// 		m_TeachGrid.SetItemBkColour(nNewRow+1, 1, Color(Color::LightBlue).ToCOLORREF());
// 		m_nLastTeachGridRow = nNewRow;
// 	}
	if(bRefrash)	m_TeachGrid.Refresh();
}

void CFormMoterUnitTest::UpdateTeachGrid()
{
	CAxisParam pAxisData;
	if (!theAxisBank.GetParamNode(m_CurAxis, pAxisData) )
		return;

	CArray <TEACH_PARAM, TEACH_PARAM> *arr = new CArray <TEACH_PARAM, TEACH_PARAM>();	
	arr = &theTeachBank.GetTeachParamList((int)m_CurAxis);

	if ( arr->GetSize() != 0 )
	{
		for(int i =0; i < arr->GetSize(); i++ )
		{
			CString szPos,szSpeed, szAcc, szDec;
			szPos.Format(_T("%d"), arr->GetAt(i).dPos);
			szSpeed.Format(_T("%d"), arr->GetAt(i).dSpeed);
			szAcc.Format(_T("%d"), arr->GetAt(i).dAcc);
			szDec.Format(_T("%d"), arr->GetAt(i).dDec);

			m_TeachGrid.SetItemText(i+1,0, arr->GetAt(i).strName);
			m_TeachGrid.SetItemText(i+1,1, szPos);
			m_TeachGrid.SetItemText(i+1,2, szSpeed);
			m_TeachGrid.SetItemText(i+1,3, szAcc);
			m_TeachGrid.SetItemText(i+1,4, szDec);
		}

	}

	m_TeachGrid.Refresh();

	arr->RemoveAll();
	delete arr;
}

void CFormMoterUnitTest::UpdateMotorState()
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

			if( status.bServoOn && (m_MotorGrid.GetItemBkColour(i, 2) !=Color(Color::Lime).ToCOLORREF()) )
				m_MotorGrid.SetItemBkColour(i, 2, Color(Color::Lime).ToCOLORREF());

			if( status.bAlarmOn && (m_MotorGrid.GetItemBkColour(i, 3) !=Color(Color::Red).ToCOLORREF()) )
				m_MotorGrid.SetItemBkColour(i, 3, Color(Color::Red).ToCOLORREF());

			if( status.bOriginComplete && (m_MotorGrid.GetItemBkColour(i, 4) !=Color(Color::Lime).ToCOLORREF()) )
				m_MotorGrid.SetItemBkColour(i, 4, Color(Color::Lime).ToCOLORREF());

			if( status.bNLimitOn && (m_MotorGrid.GetItemBkColour(i, 5) !=Color(Color::Red).ToCOLORREF()) )
				m_MotorGrid.SetItemBkColour(i, 5, Color(Color::Red).ToCOLORREF());

			if( status.bHomeOn && (m_MotorGrid.GetItemBkColour(i, 6) !=Color(Color::Lime).ToCOLORREF()) )
				m_MotorGrid.SetItemBkColour(i, 6, Color(Color::Lime).ToCOLORREF());

			if( status.bPLimitOn && (m_MotorGrid.GetItemBkColour(i, 7) !=Color(Color::Red).ToCOLORREF()) )
				m_MotorGrid.SetItemBkColour(i, 7, Color(Color::Red).ToCOLORREF());

			strTourqe_Axis.Format(_T("%6.2f"), theDeviceMotion.GetTorque(MP2100_THREAD_MAIN_PROCESS, axis));
			strTourqe_Grid = m_MotorGrid.GetItemText(i, 8);
			if( strTourqe_Axis != strTourqe_Grid )
				m_MotorGrid.SetItemText(i, 8, strTourqe_Axis);
		}
	}
	m_MotorGrid.Refresh();
}

void CFormMoterUnitTest::UpdateMotorParam()
{
	// 선택된 한 축의 파라미터 및 상태 표시
	if(m_CurAxis == AXIS_ID_MAX)
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



	double dValue = 0.;
	int nValue = 0;
	theDeviceMotion.GetPosition(MP2100_THREAD_MAIN_PROCESS, m_CurAxis, dValue);
	pCurPosition->SetCaption(CStringSupport::N2C(dValue));

	dValue = theAxisBank.m_Axis[m_CurAxis].m_dJogSpeedHigh;
	pSpeedJogHigh->SetCaption(CStringSupport::N2C(dValue));

	dValue = theAxisBank.m_Axis[m_CurAxis].m_dJogSpeedMiddle;
	pSpeedJogMid->SetCaption(CStringSupport::N2C(dValue));

	dValue = theAxisBank.m_Axis[m_CurAxis].m_dJogSpeedLow;
	pSpeedJogLow->SetCaption(CStringSupport::N2C(dValue));

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
	pModuleNo->SetCaption(CStringSupport::N2C(nValue));

	nValue = theAxisBank.m_Axis[m_CurAxis].m_nInnerNo;
	pInnerNo->SetCaption(CStringSupport::N2C(nValue));
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
LRESULT CFormMoterUnitTest::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
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
LRESULT CFormMoterUnitTest::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	// CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_BTNX_BOTTOM_MENU1);

	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


// 윈도우 생성전에 모양을 달리 지정한다.
BOOL CFormMoterUnitTest::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	cs.style = WS_OVERLAPPED | WS_CHILD;				// 테두리를 없앤다.

	return CFormView::PreCreateWindow(cs);
}
BEGIN_EVENTSINK_MAP(CFormMoterUnitTest, CFormView)
	ON_EVENT(CFormMoterUnitTest, IDC_GXBTN_SETUP4_HALFUP, DISPID_CLICK, CFormMoterUnitTest::ClickGxbtnSetup4Halfup, VTS_NONE)
	ON_EVENT(CFormMoterUnitTest, IDC_GXBTN_SETUP4_HALFDOWN, DISPID_CLICK, CFormMoterUnitTest::ClickGxbtnSetup4Halfdown, VTS_NONE)
	ON_EVENT(CFormMoterUnitTest, IDC_GXBTN_SETUP4_UP, DISPID_CLICK, CFormMoterUnitTest::ClickGxbtnSetup4Up, VTS_NONE)
	ON_EVENT(CFormMoterUnitTest, IDC_GXBTN_SETUP4_DOWN, DISPID_CLICK, CFormMoterUnitTest::ClickGxbtnSetup4Down, VTS_NONE)
	ON_EVENT(CFormMoterUnitTest, IDC_GXBTN_SETUP4_SERVOON, DISPID_CLICK, CFormMoterUnitTest::ClickGxbtnSetup4Servoon, VTS_NONE)
	ON_EVENT(CFormMoterUnitTest, IDC_GXBTN_SETUP4_SERVOOFF, DISPID_CLICK, CFormMoterUnitTest::ClickGxbtnSetup4Servooff, VTS_NONE)
	ON_EVENT(CFormMoterUnitTest, IDC_GXBTN_SETUP4_RESET, DISPID_CLICK, CFormMoterUnitTest::ClickGxbtnSetup4Reset, VTS_NONE)
	ON_EVENT(CFormMoterUnitTest, IDC_GXBTN_SETUP4_ORIGIN, DISPID_CLICK, CFormMoterUnitTest::ClickGxbtnSetup4Origin, VTS_NONE)
	ON_EVENT(CFormMoterUnitTest, IDC_GXBTN_SETUP4_MOVE_STOP, DISPID_CLICK, CFormMoterUnitTest::ClickGxbtnSetup4MoveStop, VTS_NONE)
	ON_EVENT(CFormMoterUnitTest, IDC_GXBTN_SETUP4_SAVE, DISPID_CLICK, CFormMoterUnitTest::ClickGxbtnSetup4Save, VTS_NONE)
END_EVENTSINK_MAP()


void CFormMoterUnitTest::ClickGxbtnSetup4Halfup()
{
	if ( m_iUpDownParam - 3  < 0 )		
		m_iUpDownParam = 0;
	else
		m_iUpDownParam -= 3;

	UpdateMotorGrid();
}


void CFormMoterUnitTest::ClickGxbtnSetup4Halfdown()
{
	if ( m_iUpDownParam + 3  >= AXIS_ID_MAX )		
		return;

	m_iUpDownParam += 3;

	UpdateMotorGrid();
}


void CFormMoterUnitTest::ClickGxbtnSetup4Up()
{
	if ( m_iUpDownParam - 7  < 0 )		
		m_iUpDownParam = 0;
	else
		m_iUpDownParam -= 7;

	UpdateMotorGrid();
}


void CFormMoterUnitTest::ClickGxbtnSetup4Down()
{
	if ( m_iUpDownParam + 7  >= AXIS_ID_MAX )		
		return;
	
	m_iUpDownParam += 7;

	UpdateMotorGrid();
}

void CFormMoterUnitTest::SelectedAxis(int iSel, int iSel2)
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
	UpdateTeachGrid();
	
}

void CFormMoterUnitTest::OnGridMoterEndSelChange(NMHDR *pNotifyStruct, LRESULT *pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;	

	if ( pItem->iRow < 1 )		return;				// 메뉴 정의된 Row Click시 return	

	m_TeachGrid.SetEditable(TRUE);							//	Teach Grid Clear해줌
	CCellRange cRange;
	cRange.Set(1,0,5,4);
	m_TeachGrid.ClearCells(cRange);
	m_TeachGrid.SetEditable(FALSE);		

	
	// - 모터 Bank에서 일일이 검색할 시 속도문제가 발생하기 때문에
	// - 현재 select된 Grid의 No값을 가져와서 해당 No값의 Axis를 가져온다.

	CString szClickedNumber = m_MotorGrid.GetItemText(pItem->iRow,0);		//	클릭된 메뉴의 NO 값을 가져와서 int로 변경
	int iNum = _ttoi(szClickedNumber) - 1;

	// 현재 선택된 축 갱신
	if ( iNum >= AXIS_ID_MAX)		return;
	m_CurAxis = (AXIS_ID)iNum;

	// 이전 선택한 Row를 지우고 새로 클릭한 Row를 색칠한다
	HighlightCurAxis(TRUE);

	UpdateTeachGrid();

	*pResult = 0;
}

void CFormMoterUnitTest::OnGridTeachEndSelChange( NMHDR *pNotifyStruct, LRESULT *pResult )
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;	

	if ( pItem->iRow < 1 )		return;				// 메뉴 정의된 Row Click시 return	
}

void CFormMoterUnitTest::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	InitializeGrid();
}


void CFormMoterUnitTest::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
	case TIMER_MONITOR:
		KillTimer(nIDEvent);
		UpdateMotorState();
		UpdateMotorParam();
		SetTimer(TIMER_MONITOR, 100, NULL);
		break;
	}

	CFormView::OnTimer(nIDEvent);
}


void CFormMoterUnitTest::ClickGxbtnSetup4Servoon()
{
	theDeviceMotion.ServoOn(MP2100_THREAD_MAIN_PROCESS, m_CurAxis);
}


void CFormMoterUnitTest::ClickGxbtnSetup4Servooff()
{
	theDeviceMotion.ServoOff(MP2100_THREAD_MAIN_PROCESS, m_CurAxis);
}


void CFormMoterUnitTest::ClickGxbtnSetup4Reset()
{
	theDeviceMotion.ResetAlarm(MP2100_THREAD_MAIN_PROCESS, m_CurAxis);
}


void CFormMoterUnitTest::ClickGxbtnSetup4Origin()
{
	theDeviceMotion.OriginRetrun(MP2100_THREAD_MAIN_PROCESS, m_CurAxis);
}


void CFormMoterUnitTest::ClickGxbtnSetup4MoveStop()
{
	theDeviceMotion.AxisStop(MP2100_THREAD_MAIN_PROCESS, m_CurAxis);
}


void CFormMoterUnitTest::ClickGxbtnSetup4Save()
{
	// 편집중일때만 저장되도록
	if(m_bParamEditing == FALSE)
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
}
