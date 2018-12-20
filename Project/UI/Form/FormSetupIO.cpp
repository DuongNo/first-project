// FormSetup2.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FormSetupIO.h"

#include "GUIDefine.h"

//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "BankGuild\IOParamBank.h"
#include "CheetahApp.h"
#include "..\Dialog\DlgPageIO.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CFormSetupIO

IMPLEMENT_DYNCREATE(CFormSetupIO, CFormView)

CFormSetupIO::CFormSetupIO()
	: CFormView(CFormSetupIO::IDD)
{

}

CFormSetupIO::~CFormSetupIO()
{
}

void CFormSetupIO::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
		
	InitGrid();
	m_iMoveInput = 0;
	m_iMoveOutput = 0;
	m_iSelectedRowIn = -1;
	m_iSelectedRowOut = -1;
	m_iPageNumIn = 1;
	m_iPageNumOut = 1;
}


void CFormSetupIO::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID_SETUP_INPUT_IO, m_GridINput);
	DDX_Control(pDX, IDC_GRID_SETUP_OUTPUT_IO, m_GridOutput);
}

BEGIN_MESSAGE_MAP(CFormSetupIO, CFormView)

	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_NOTIFY(NM_CLICK, IDC_GRID_SETUP_OUTPUT_IO, onGridClickOutputIO)	
	ON_WM_TIMER()

END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CFormSetupIO, CFormView)
	ON_EVENT(CFormSetupIO, IDC_GXBTN_SETUP2_INPUT_DOWN, DISPID_CLICK, CFormSetupIO::ClickBtnInChgPageDown, VTS_NONE)
	ON_EVENT(CFormSetupIO, IDC_GXBTN_SETUP2_INPUT_UP, DISPID_CLICK, CFormSetupIO::ClickBtnInChgPageUp, VTS_NONE)	
	ON_EVENT(CFormSetupIO, IDC_GXBTN_SETUP2_OUTPUT_DOWN, DISPID_CLICK, CFormSetupIO::ClickBtnOutChgPageDown, VTS_NONE)
	ON_EVENT(CFormSetupIO, IDC_GXBTN_SETUP2_OUTPUT_UP, DISPID_CLICK, CFormSetupIO::ClickBtnOutChgPageUp, VTS_NONE)

	ON_EVENT(CFormSetupIO, IDC_GXLBL_SETUP_IO_INPUT_PAGE, DISPID_CLICK, CFormSetupIO::ClickPageInput, VTS_NONE)
	ON_EVENT(CFormSetupIO, IDC_GXLBL_SETUP_IO_OUTPUT_PAGE, DISPID_CLICK, CFormSetupIO::ClickPageOutput, VTS_NONE)
END_EVENTSINK_MAP()



// CFormSetup2 진단입니다.

#ifdef _DEBUG
void CFormSetupIO::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormSetupIO::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormSetup2 메시지 처리기입니다.

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
LRESULT CFormSetupIO::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{

	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{
		UpdateGrid();
		SetTimer(0, 100, NULL);
	}
	else
	{

	}

	return 0;
}

// 표시 언어가 변경되었다.
LRESULT CFormSetupIO::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	// CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_BTNX_BOTTOM_MENU1);

	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

// 윈도우 생성전에 모양을 달리 지정한다.
BOOL CFormSetupIO::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	cs.style = WS_OVERLAPPED | WS_CHILD;				// 테두리를 없앤다.

	return CFormView::PreCreateWindow(cs);
}


void CFormSetupIO::InitGrid()
{
	//설정
	m_GridINput.SetEditable(FALSE);
	m_GridINput.SetDefCellHeight(40);
	m_GridINput.SetFixedColumnCount(1); 
	m_GridINput.SetFixedRowCount(1);
	m_GridINput.SetRowCount(17);
	m_GridINput.SetColumnCount(4);
	m_GridINput.SetRowResize(FALSE);
	m_GridINput.SetColumnResize(FALSE);		
	m_GridINput.SetListMode(FALSE);
	m_GridINput.EnableSelection(FALSE);
	m_GridINput.EnableWindow(FALSE);

	m_GridOutput.SetEditable(FALSE);
	m_GridOutput.SetDefCellHeight(40);
	m_GridOutput.SetFixedColumnCount(1); 
	m_GridOutput.SetFixedRowCount(1);
	m_GridOutput.SetRowCount(17);
	m_GridOutput.SetColumnCount(4);
	m_GridOutput.SetRowResize(FALSE);
	m_GridOutput.SetColumnResize(FALSE);		
	m_GridOutput.SetListMode(FALSE);
	m_GridOutput.EnableSelection(FALSE);


	//간격
	m_GridINput.SetColumnWidth(0, 20); 
	m_GridINput.SetColumnWidth(1, 60);
	m_GridINput.SetColumnWidth(2, 250);
	m_GridINput.SetColumnWidth(3, 60);


	m_GridOutput.SetColumnWidth(0, 20); 
	m_GridOutput.SetColumnWidth(1, 60);
	m_GridOutput.SetColumnWidth(2, 250);
	m_GridOutput.SetColumnWidth(3, 60);
	
	//이름
	m_GridINput.SetItemText(0,0,_T("NO"));
	m_GridINput.SetItemText(0,1,_T("Address"));
	m_GridINput.SetItemText(0,2,_T("I/O Name"));
	m_GridINput.SetItemText(0,3,_T("State"));
	
	m_GridOutput.SetItemText(0,0,_T("NO"));
	m_GridOutput.SetItemText(0,1,_T("Address"));
	m_GridOutput.SetItemText(0,2,_T("I/O Name"));
	m_GridOutput.SetItemText(0,3,_T("State"));
}

void CFormSetupIO::UpdateGrid()
{
	m_iPageNumIn = ((m_iMoveInput / 16 == 0) ? 1 : m_iMoveInput / 16 + 1);
	m_iPageNumOut = ((m_iMoveOutput / 16 == 0) ? 1 : m_iMoveOutput / 16 + 1);

	CString szPageIn;
	CString szPageOut;

	szPageIn.Format(_T("%d"),m_iPageNumIn);
	szPageOut.Format(_T("%d"),m_iPageNumOut);

	CGxUICtrl::SetStaticString(this,IDC_GXLBL_SETUP_IO_INPUT_PAGE,szPageIn);
	CGxUICtrl::SetStaticString(this,IDC_GXLBL_SETUP_IO_OUTPUT_PAGE,szPageOut);
	
	CString sNumInput = _T("");
	CString sNumOutput = _T("");
	
	int iValIn = 0;
	for ( int iRow = 0; iRow < m_GridINput.GetRowCount(); iRow++ )
	{
		if ( iRow == 0 )	continue;
		
		sNumInput.Format(_T("%d"),iRow + m_iMoveInput);
		m_GridINput.SetItemText(iRow,0,(_T("%d"),sNumInput));	
			
		iValIn = iRow-1 + m_iMoveInput;

		m_GridINput.SetItemText(iRow, 1, (_T("%s"),theIOBank.m_In[iValIn].m_strIOAddress));
		m_GridINput.SetItemText(iRow, 2, (_T("%s"),theIOBank.m_In[iValIn].m_strIOName)); 
	}

	int iValOut = 0;
	for ( int iRow = 0; iRow < m_GridOutput.GetRowCount(); iRow++ )
	{
		if ( iRow == 0 ) continue;

		sNumOutput.Format(_T("%d"),iRow + m_iMoveOutput);
		m_GridOutput.SetItemText(iRow, 0, (_T("%d"), sNumOutput));

		iValOut = iRow-1 + m_iMoveOutput;

		m_GridOutput.SetItemText(iRow, 1, (_T("%s"), theIOBank.m_Out[iValOut].m_strIOAddress));
		m_GridOutput.SetItemText(iRow, 2, (_T("%s"), theIOBank.m_Out[iValOut].m_strIOName));
	}


	m_GridINput.ExpandColumnsToFit();	
	m_GridINput.ExpandToFit();
	m_GridINput.Refresh();

	m_GridOutput.ExpandColumnsToFit();	
	m_GridOutput.ExpandToFit();
	m_GridOutput.Refresh();
}

void CFormSetupIO::IORefresh()
{	
	CString strInputPLCNo = m_GridINput.GetItemText(1,1);
	CString strOutputPLCNo = m_GridOutput.GetItemText(1,1);
	
	int nIBit		= wcstol(strInputPLCNo.Right(3), NULL, 16);
	int nOBit		= wcstol(strOutputPLCNo.Right(3), NULL, 16);
	int nIStationNum =	nIBit / DEV_INPUT;
	int nOStationNum =	nOBit / DEV_OUTPUT;

	WORD nInput = theDeviceIO.ReadInWord(nIStationNum);
	WORD nOutput = theDeviceIO.ReadOutWord(nOStationNum);

	for(int i=0; i<DEV_INPUT; i++)
	{
		if(nInput >> i & 0x01)
		{
			if(m_GridINput.GetItemBkColour(i + 1, 3) != RGB(0,255,0))
			{
				m_GridINput.SetItemBkColour(i + 1, 3, RGB(0,255,0));
				//m_GridINput.SetItemFgColour(i + 1, 3, RGB(255,255,255));
			}
		}
		else
		{
			if(m_GridINput.GetItemBkColour(i + 1, 3) != RGB(255,255,255))
			{
				m_GridINput.SetItemBkColour(i + 1, 3, RGB(255,255,255));
				//m_GridINput.SetItemFgColour(i + 1, 3, RGB(25,25,25));
			}
		}
	}

	for(int i=0; i<DEV_OUTPUT; i++)
	{
		if(nOutput >> i & 0x01)
		{
			if(m_GridOutput.GetItemBkColour(i + 1, 3) != RGB(0,255,0))
			{
				m_GridOutput.SetItemBkColour(i + 1, 3, RGB(0,255,0));
				//m_GridOutput.SetItemFgColour(i + 1, 3, RGB(255,255,255));
			}
		}
		else
		{
			if(m_GridOutput.GetItemBkColour(i + 1, 3) != RGB(255,255,255))
			{
				m_GridOutput.SetItemBkColour(i + 1, 3, RGB(255,255,255));
				//m_GridOutput.SetItemFgColour(i + 1, 3, RGB(25,25,25));
			}
		}
	}

	m_GridINput.Refresh();
	m_GridOutput.Refresh();
}


void CFormSetupIO::onGridClickOutputIO(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
		
	if ( pItem->iRow < 1 )
		return;

	CString szSelected = m_GridOutput.GetItemText(pItem->iRow,0);
	m_iCurSelected = _ttoi(szSelected) -1;

	CString szIONo = m_GridOutput.GetItemText(pItem->iRow, 1);
	int nBitNO = wcstol(szIONo.Right(3), NULL, 16);

	theDeviceIO.WriteOutBit(nBitNO, theDeviceIO.ReadOutBit(nBitNO)==IO_ON ? IO_OFF : IO_ON);

// 	m_GridOutput.SetFocusCell(-1,-1);
// 	m_GridOutput.Refresh();
}


////////////////////////////////////////////////////////////////////////////////////////

void CFormSetupIO::ClickBtnInChgPageDown()
{
	if ( m_iMoveInput - 16  < 0 )		
		m_iMoveInput = 0;
	else
		m_iMoveInput -= 16;
			
	UpdateGrid();	
}


void CFormSetupIO::ClickBtnInChgPageUp()
{
	if ( m_iMoveInput + 16  >= MAX_IN_ID )		
		return;

	m_iMoveInput += 16;

	UpdateGrid();
}


void CFormSetupIO::ClickBtnOutChgPageDown()
{
	if ( m_iMoveOutput - 16  < 0)		
		m_iMoveOutput = 0;
	else
		m_iMoveOutput -= 16;

	
	UpdateGrid();
}


void CFormSetupIO::ClickBtnOutChgPageUp()
{
	if ( m_iMoveOutput + 16  >= MAX_OUT_ID )		
		return;

	m_iMoveOutput += 16;
		
	UpdateGrid();
}


void CFormSetupIO::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(nIDEvent);
	switch(nIDEvent)
	{
	case 0:
		IORefresh();
		SetTimer(nIDEvent, 100, NULL);
		break;
	}
	CFormView::OnTimer(nIDEvent);
}

void CFormSetupIO::ClickPageInput()
{
	CDlgPageIO dlgPgIn;

	int iPgCount = (MAX_IN_ID % 16 == 0) ? MAX_IN_ID/16 : MAX_IN_ID/16 + 1;

	dlgPgIn.SetBoolInput(TRUE);
	dlgPgIn.SetPageCount( iPgCount );
	
	INT_PTR iRet = dlgPgIn.DoModal();

	if ( iRet < 0 )		return;

	m_iPageNumIn = iRet;
	m_iMoveInput = (iRet-1)*16;

	UpdateGrid();
}


void CFormSetupIO::ClickPageOutput()
{
	CDlgPageIO dlgPgOut;

	int iPgCount = (MAX_OUT_ID % 16 == 0) ? (MAX_OUT_ID)/16 : (MAX_OUT_ID)/16 + 1;

	dlgPgOut.SetBoolInput(FALSE);
	dlgPgOut.SetPageCount( iPgCount );
	
	INT_PTR iRet = dlgPgOut.DoModal();

	if ( iRet < 0 )		return;
		
	m_iPageNumOut = iRet;
	m_iMoveOutput = (iRet-1)*16;

	UpdateGrid();
}
