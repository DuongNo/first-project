// DlgDoor.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgDoor.h"


// CDlgDoor 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgDoor, CDialog)

CDlgDoor::CDlgDoor(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDoor::IDD, pParent)
{

}

CDlgDoor::~CDlgDoor()
{
}

void CDlgDoor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID_DOOR_LIST, m_Grid);	
}


BEGIN_MESSAGE_MAP(CDlgDoor, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_GRID_DOOR_LIST, OnGridDoorSelChange)
	ON_WM_TIMER()
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CDlgDoor, CDialog)
	ON_EVENT(CDlgDoor, IDC_GXBTN_DOOR_ALL_OPEN, DISPID_CLICK, CDlgDoor::ClickBtnDoorAllOpen, VTS_NONE)
	ON_EVENT(CDlgDoor, IDC_GXBTN_DOOR_ALL_CLOSE, DISPID_CLICK, CDlgDoor::ClickBtnDoorAllClose, VTS_NONE)
	ON_EVENT(CDlgDoor, IDC_GXBTN_DOOR_CANCEL, DISPID_CLICK, CDlgDoor::ClickBtnDoorCancel, VTS_NONE)
	END_EVENTSINK_MAP()


	
BOOL CDlgDoor::OnInitDialog()
{
	CDialog::OnInitDialog();
	InitGrid();
	UpdateGrid();

	return TRUE; 
}


void CDlgDoor::InitGrid()
{
	tDoor t;


	t.nIn = X_LD_DOOR_SEN_1;						t.nOut = Y_LD_DOOR_UNLOCK_1;					m_vecDoor.push_back(t);
	t.nIn = X_LD_DOOR_SEN_2;						t.nOut = Y_LD_DOOR_UNLOCK_2;					m_vecDoor.push_back(t);
	t.nIn = X_LD_DOOR_SEN_3;						t.nOut = Y_LD_DOOR_UNLOCK_3;					m_vecDoor.push_back(t);
	t.nIn = X_LD_DOOR_SEN_4;						t.nOut = Y_LD_DOOR_UNLOCK_4;					m_vecDoor.push_back(t);
	t.nIn = X_LD_DOOR_SEN_5;						t.nOut = Y_LD_DOOR_UNLOCK_5;					m_vecDoor.push_back(t);
	t.nIn = X_LD_DOOR_SEN_6;						t.nOut = Y_LD_DOOR_UNLOCK_6;					m_vecDoor.push_back(t);
	t.nIn = X_ULD_DOOR_SEN_1;						t.nOut = Y_UD_DOOR_UNLOCK_1;					m_vecDoor.push_back(t);
	t.nIn = X_ULD_DOOR_SEN_2;						t.nOut = Y_UD_DOOR_UNLOCK_2;					m_vecDoor.push_back(t);
	t.nIn = X_ULD_DOOR_SEN_3;						t.nOut = Y_UD_DOOR_UNLOCK_3;					m_vecDoor.push_back(t);
	t.nIn = X_ULD_DOOR_SEN_4;						t.nOut = Y_UD_DOOR_UNLOCK_4;					m_vecDoor.push_back(t);
	t.nIn = X_ULD_DOOR_SEN_5;						t.nOut = Y_UD_DOOR_UNLOCK_5;					m_vecDoor.push_back(t);
	t.nIn = X_ULD_DOOR_SEN_6;						t.nOut = Y_UD_DOOR_UNLOCK_6;					m_vecDoor.push_back(t);


	//===============================================================================================================================
	m_Grid.SetEditable(FALSE);
	m_Grid.SetDefCellHeight(50);
	m_Grid.SetFixedColumnCount(1); 
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetRowCount(m_vecDoor.size() + 1);
	m_Grid.SetColumnCount(4);
	m_Grid.SetRowResize(FALSE);
	m_Grid.SetColumnResize(FALSE);		
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableSelection(FALSE);

	m_Grid.SetColumnWidth(0,10);
	m_Grid.SetColumnWidth(1,300);
	m_Grid.SetRowHeight(0,35);

	//이름
	m_Grid.SetItemText(0,0,_T("NO"));
	m_Grid.SetItemText(0,1,_T("NAME"));
	m_Grid.SetItemText(0,2,_T("OPEN/CLOSE"));
	m_Grid.SetItemText(0,3,_T("OUTPUT"));

	// Font & Style 설정
	DWORD dwTextStyle = DT_RIGHT|DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_SINGLELINE;

	for(int i = 0; i < m_Grid.GetRowCount(); i++ )
	{
		m_Grid.SetItemFormat(i, 0, dwTextStyle);
		m_Grid.SetItemFormat(i, 1, dwTextStyle);
		m_Grid.SetItemFormat(i, 2, dwTextStyle);
		m_Grid.SetItemFormat(i, 3, dwTextStyle);
	}
}

void CDlgDoor::UpdateGrid()
{
	CString szNum = _T("");
	CString szInput = _T("");
	CString szStateIn = _T("");
	CString szStateOut = _T("");

	for(int i = 0; i < m_Grid.GetRowCount(); i++ )
	{
		if ( i == 0 )	continue;

		szNum.Format(_T("%d"),i);
		szInput = theIOBank.GetInputIOParam((INPUT_ID)m_vecDoor[i-1].nIn).m_strIOName;
		szStateIn = theUnitFunc.GetInPutIOCheck((INPUT_ID)m_vecDoor[i-1].nIn) ? _T("OPENED") : _T("CLOSED");
		szStateOut = theUnitFunc.GetOutPutIOCheck((OUTPUT_ID)m_vecDoor[i-1].nOut) ? _T("UNLOCK") : _T("LOCK");

		m_Grid.SetItemText(i, 0, szNum.GetBuffer());
		m_Grid.SetItemText(i, 1, szInput.GetBuffer());
		m_Grid.SetItemText(i, 2, szStateIn.GetBuffer());
		m_Grid.SetItemText(i, 3, szStateOut.GetBuffer());
	}

	m_Grid.ExpandColumnsToFit();
	m_Grid.Refresh();

	SetTimer(1, 100, NULL);
}


//===============================================================================================================================
//===============================================================================================================================

BOOL CDlgDoor::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4)
		return TRUE;
	if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE )
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}



void CDlgDoor::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(nIDEvent);

	if(nIDEvent == 1)
	{
		UpdateGrid();
		SetTimer(1, 100,NULL);
	}

	CDialog::OnTimer(nIDEvent);
}




//===============================================================================================================================
//===============================================================================================================================
void CDlgDoor::ClickBtnDoorAllOpen()
{
	std::vector<tDoor>::iterator it = m_vecDoor.begin();
	for(it = m_vecDoor.begin(); it != m_vecDoor.end(); it++ )
	{
		//kjpark 20161027 IO Output check bug 수정
		theUnitFunc.SetOutPutIO((OUTPUT_ID)it->nOut, ON);
	}	
}


void CDlgDoor::ClickBtnDoorAllClose()
{
	std::vector<tDoor>::iterator it = m_vecDoor.begin();
	for(it = m_vecDoor.begin(); it != m_vecDoor.end(); it++ )
	{
		//kjpark 20161027 IO Output check bug 수정
		theUnitFunc.SetOutPutIO((OUTPUT_ID)it->nOut, OFF);
	}	
}


void CDlgDoor::ClickBtnDoorCancel()
{
	EndDialog(IDCANCEL);
}

//===============================================================================================================================
//===============================================================================================================================


void CDlgDoor::OnGridDoorSelChange(NMHDR *pNotifyStruct, LRESULT *pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	int iRow = pItem->iRow;
	if ( iRow < 1 )		return;		//	0번줄 클릭시 return
	
	CString szClick = m_Grid.GetItemText(iRow, 1);
	CString szComp = _T("");
	CString szState = _T("");
	CIOParam tParam;

	BOOL	bState = FALSE;

	std::vector<tDoor>::iterator it = m_vecDoor.begin();
	for(it = m_vecDoor.begin(); it != m_vecDoor.end(); it++)
	{
		//bState = theUnitFunc.GetInPutIOCheck((INPUT_ID)it->nIn);
		tParam = theIOBank.GetInputIOParam((INPUT_ID)it->nIn);
		szComp = tParam.m_strIOName;
				
		szState = m_Grid.GetItemText(iRow,3);
		
		if ( szComp.CompareNoCase(szClick.GetBuffer()) == 0 )			//	같으면 0을 리턴
		{
			if ( szState == _T("LOCK") )
				//kjpark 20161027 IO Output check bug 수정
				theUnitFunc.SetOutPutIO((OUTPUT_ID)it->nOut, ON);		//	현재 잠그는 신호가 가고있다면 UNLOCK 신호를 준다
			else if ( szState == _T("UNLOCK") )	
				//kjpark 20161027 IO Output check bug 수정
				theUnitFunc.SetOutPutIO((OUTPUT_ID)it->nOut, OFF);		//	현재 문을여는 신호가 가고있다면 LOCK 신호를 준다.

			break;
		}
	}
}




