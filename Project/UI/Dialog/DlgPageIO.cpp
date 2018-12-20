// DlgPageIO.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgPageIO.h"

#include "..\..\CheetahApp.h"



// CDlgPageIO 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgPageIO, CDialog)

CDlgPageIO::CDlgPageIO(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPageIO::IDD, pParent)
{

}

CDlgPageIO::~CDlgPageIO()
{
}

void CDlgPageIO::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID_IO_PAGE, m_GridPage);
}


// CDlgPageIO 메시지 처리기입니다.

BEGIN_MESSAGE_MAP(CDlgPageIO, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_GRID_IO_PAGE, OnGridSelChange)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CDlgPageIO, CDialog)
	ON_EVENT(CDlgPageIO, IDC_GXBTN_IO_PAGE_UP, DISPID_CLICK, CDlgPageIO::ClickBtnPageUp, VTS_NONE)
	ON_EVENT(CDlgPageIO, IDC_GXBTN_IO_PAGE_DOWN, DISPID_CLICK, CDlgPageIO::ClickBtnPageDown, VTS_NONE)
END_EVENTSINK_MAP()

BOOL CDlgPageIO::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_iMove = 0;

	InitGrid();

	UpdateGrid();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgPageIO::InitGrid()
{
	m_GridPage.SetEditable(FALSE);
	m_GridPage.SetDefCellHeight(50);
	m_GridPage.SetFixedColumnCount(1); 
	m_GridPage.SetFixedRowCount(1);
	m_GridPage.SetRowCount(9);
	m_GridPage.SetColumnCount(2);
	m_GridPage.SetRowResize(FALSE);
	m_GridPage.SetColumnResize(FALSE);		
	m_GridPage.SetListMode(FALSE);
	m_GridPage.EnableSelection(FALSE);
	//m_GridPage.EnableWindow(FALSE);

	m_GridPage.SetColumnWidth(0,10);
	m_GridPage.SetColumnWidth(1,300);
	m_GridPage.SetRowHeight(0,15);

	//이름
	m_GridPage.SetItemText(0,0,_T("NO"));
	m_GridPage.SetItemText(0,1,_T("Address Range"));

		// Font & Style 설정
	DWORD dwTextStyle = DT_RIGHT|DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_SINGLELINE;

	for(int i = 0; i < m_GridPage.GetRowCount(); i++ )
	{
		m_GridPage.SetItemFormat(i, 0, dwTextStyle);
		m_GridPage.SetItemFormat(i, 1, dwTextStyle);
	}
}

void CDlgPageIO::UpdateGrid()
{
	CString szNum;
	CString szAddress;
	int iBeginAdd = 0;
	int iEndAdd = 0;

	m_GridPage.SetEditable(TRUE);
	CCellRange cRange(1,0,8,1);
	m_GridPage.ClearCells(cRange);
	m_GridPage.SetEditable(FALSE);
	
	for (int iRow = 0; iRow < m_GridPage.GetRowCount(); iRow++ )
	{
		if (iRow == 0)	continue;

		iBeginAdd	=	(iRow+m_iMove-1) * 16;
		iEndAdd		=	( (iRow+m_iMove) * 16) -1;

		if  (m_bInput == TRUE )
		{
			if ( iBeginAdd >= MAX_IN_ID )	break;
			szAddress.Format(_T("%s  ~  %s"), theIOBank.m_In[iBeginAdd].m_strIOAddress, theIOBank.m_In[iEndAdd].m_strIOAddress);
		}
		else
		{
			if ( iBeginAdd >= MAX_OUT_ID )	break;
			szAddress.Format(_T("%s  ~  %s"), theIOBank.m_Out[iBeginAdd].m_strIOAddress, theIOBank.m_Out[iEndAdd].m_strIOAddress);
		}

		szNum.Format(_T("%d"), iRow + m_iMove);
		m_GridPage.SetItemText(iRow, 0, szNum.GetBuffer());
		
		m_GridPage.SetItemText(iRow, 1, szAddress);
	}
	
	m_GridPage.ExpandToFit();	
	m_GridPage.Refresh();
}

void CDlgPageIO::OnGridSelChange(NMHDR *pNotifyStruct, LRESULT *pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	int iCurRow = m_iMove + pItem->iRow;
	if ( pItem->iRow < 1 )		return;		//	0번줄 클릭시 return

	if  (m_bInput == TRUE )	
	{
		if ( iCurRow *16 > MAX_IN_ID )			return;
	}
	else
	{
		if ( iCurRow * 16 > MAX_OUT_ID)	return;
	}

	EndDialog(iCurRow);
}
void CDlgPageIO::ClickBtnPageUp()
{
	if ( m_iMove - 8 < 0 )			
		return;

	m_iMove -= 8;
	UpdateGrid();
}


void CDlgPageIO::ClickBtnPageDown()
{
	if ( m_iMove + 8  >= m_iRowCount )		
		return;

	m_iMove += 8;
	UpdateGrid();
}


BOOL CDlgPageIO::PreTranslateMessage(MSG* pMsg)					//	[W] 키보드의 종료 옵션들 제거
{
	if(pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4)
		return TRUE;
	if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE )
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgPageIO::OnClose()
{
	EndDialog(-1);

	CDialog::OnClose();
}
