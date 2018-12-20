// DIgRecipeSelect.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "UI\Dialog\DlgLossTimeList.h"
#include "afxdialogex.h"
#include "BankGuild\RecipeBank.h"
#include "UI\GausGUI\GxMsgBox.h"
// DIgRecipeSelect 대화 상자입니다.

IMPLEMENT_DYNAMIC(DIgLossTimeList, CDialogEx)

DIgLossTimeList::DIgLossTimeList(CWnd* pParent /*=NULL*/)
	: CDialog(DIgLossTimeList::IDD, pParent)
{
	m_iUpDownParam=0;
	m_iSelectedRow=-1;

}

DIgLossTimeList::~DIgLossTimeList()
{
}

void DIgLossTimeList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID_RECIPE_MODELLIST, m_GridModelList);
}


BEGIN_MESSAGE_MAP(DIgLossTimeList, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_GRID_RECIPE_MODELLIST, OnGridRecipeEndSelChange)
END_MESSAGE_MAP()


// DIgRecipeSelect 메시지 처리기입니다.
BEGIN_EVENTSINK_MAP(DIgLossTimeList, CDialogEx)
	ON_EVENT(DIgLossTimeList, IDC_GXBTN_RECIPE_CANCLE, DISPID_CLICK, DIgLossTimeList::ClickGxbtnRecipeCancle, VTS_NONE)
	ON_EVENT(DIgLossTimeList, IDC_GXBTN_RECIPE_SELECT, DISPID_CLICK, DIgLossTimeList::ClickGxbtnRecipeSelect, VTS_NONE)
	ON_EVENT(DIgLossTimeList, IDC_GXBTN_TEACH1_MOVE_UP, DISPID_CLICK, DIgLossTimeList::ClickGxbtnTeach1MoveUp, VTS_NONE)
	ON_EVENT(DIgLossTimeList, IDC_GXBTN_TEACH1_MOVE_DOWN, DISPID_CLICK, DIgLossTimeList::ClickGxbtnTeach1MoveDown, VTS_NONE)
END_EVENTSINK_MAP()

BOOL DIgLossTimeList::OnInitDialog()
{
	CDialog::OnInitDialog();
	//Grid 설정
	m_GridModelList.SetEditable(FALSE);
	m_GridModelList.SetDefCellHeight(30);
	m_GridModelList.SetFixedColumnCount(1); 
	m_GridModelList.SetFixedRowCount(1);
	m_GridModelList.SetRowCount(8);
	m_GridModelList.SetColumnCount(2);
	m_GridModelList.SetRowResize(FALSE);
	m_GridModelList.SetColumnResize(FALSE);		
	m_GridModelList.SetListMode(FALSE);


	//간격 설정
	m_GridModelList.SetColumnWidth(0, 25); 
	m_GridModelList.SetColumnWidth(1, 360);	

	//이름설정
	m_GridModelList.SetItemText(0,0,_T("NO"));		
	m_GridModelList.SetItemText(0,1,_T("Name"));	

	m_GridModelList.Refresh();

	// 폰트
	m_dwTextStyle = DT_RIGHT|DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_SINGLELINE;

	CFont* pGridFont = m_GridModelList.GetFont();
	pGridFont->GetLogFont(&m_lf);

	memcpy(m_lf.lfFaceName, _T("굴림체"),sizeof(_T("굴림체")));
	m_lf.lfHeight = 14;
	m_lf.lfWeight = FW_BOLD;

	for(int i = 0; i < m_GridModelList.GetRowCount(); i++)
	{	
		for(int j = 0; j<m_GridModelList.GetColumnCount(); j++)
		{
			m_GridModelList.SetItemFont(i, j, &m_lf);
			m_GridModelList.SetItemFormat(i, j, m_dwTextStyle);

			if ( i == 0 )				m_GridModelList.SetRowHeight(i, 35);			//	높이
			else						m_GridModelList.SetRowHeight(i, 50);			//  높이

		}
	}
	LoadList();
	UpdateGird();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void DIgLossTimeList::UpdateGird()
{
	CString sNum;

	//m_GridModelList.SetRowCount(m_vecRecipe.size() + 1 );
	m_GridModelList.SetRowCount(10);


	for(int i = 0; i < m_GridModelList.GetRowCount(); i++ )
	{
		if ( i == 0 )	continue;
		else			m_GridModelList.SetRowHeight(i, 35);

		sNum.Format(_T("%d"),i+m_iUpDownParam);
		for(int j = 0; j < m_GridModelList.GetColumnCount(); j++ )
		{
			m_GridModelList.SetItemFont(i, j, &m_lf);
			m_GridModelList.SetItemFormat(i, j, m_dwTextStyle);

			if ( j == 0 )	m_GridModelList.SetItemText(i, j, sNum);
			else			
			{
				CString szText;
				if ( m_vecRecipe.size() > UINT(i-1 + m_iUpDownParam) )
					m_GridModelList.SetItemText(i, j, m_vecRecipe[i - 1 + m_iUpDownParam] );
				else
					m_GridModelList.SetItemText(i, j, _T(""));
			}
		}
	}
	m_GridModelList.ShowScrollBar(SB_HORZ,FALSE);		//	횡 스크롤 제거
	m_GridModelList.ShowScrollBar(SB_VERT,FALSE);		//	종 스크롤 제거
	m_GridModelList.Refresh();

}


void DIgLossTimeList::LoadList()
{
	CString settingFile, szEntry, szValue, szText,szTemp;
	int m_nTotalNum;
	CIni ini;

	m_vecRecipe.clear();

	//settingFile.Format(_T("D:\\Cheetah\\System\\LossTimeList.ini"));

	settingFile.Format(_T("%s\\%s"),PATH_SYSTEM, NAME_FILE_LOSSTIME_LIST);

	ini.SetIniFilename(settingFile);

	if (theProcBank.Loss_info.bFirstClass == TRUE)
	{
		m_nTotalNum = ini.GetLong(_T("GROUP"), _T("COUNT"));
		for(int i = 0 ; i < m_nTotalNum ; i++)
		{
			szEntry.Format(_T("GROUP_%02d"), i);
			szValue = ini.GetString(_T("GROUP"), szEntry);
			m_vecRecipe.push_back(szValue);
		}
	}
	else if(theProcBank.Loss_info.bSecondClass == TRUE)
	{
		m_nTotalNum = ini.GetLong(theProcBank.Loss_info.strFirstGroup, _T("COUNT"));
		for(int i = 0 ; i < m_nTotalNum ; i++)
		{
			szEntry.Format(_T("SUB_%02d"), i);
			szValue = ini.GetString(theProcBank.Loss_info.strFirstGroup, szEntry);
			m_vecRecipe.push_back(szValue);
		}
	}
	else	 
	{
		m_nTotalNum = ini.GetLong(theProcBank.Loss_info.strSecondGroup, _T("COUNT"));
		for(int i = 0 ; i < m_nTotalNum ; i++)
		{
			szEntry.Format(_T("ITEM_%02d"), i);
			szValue = ini.GetString(theProcBank.Loss_info.strSecondGroup, szEntry);
			m_vecRecipe.push_back(szValue);
		}
	}
}

void DIgLossTimeList::ClickGxbtnRecipeCancle()
{
	::SendMessage(this->m_hWnd, WM_CLOSE, NULL, NULL);
}



void DIgLossTimeList::ClickGxbtnRecipeSelect()
{
	CString sztemp;

	if(m_iSelectedRow==-1 || m_iSelectedRow==0 || m_GridModelList.GetItemText(m_iSelectedRow,1).GetLength()==0 )
	{
		CGxMsgBox	dlgMsgBox;
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("항목을 선택해주세요."), 
			_T("Please select."),		
			_T("Xin hãy chọn một mục") );

		dlgMsgBox.DoModal();
		return;
	}
	if (theProcBank.Loss_info.bFirstClass == TRUE)
	{
		theProcBank.Loss_info.strFirstClass=m_GridModelList.GetItemText(m_iSelectedRow,1);
		theProcBank.Loss_info.strFirstGroup.Format(_T("GROUP_%02d"),m_iSelectedRow-1);
	}
	else if (theProcBank.Loss_info.bSecondClass == TRUE)
	{
		sztemp=m_GridModelList.GetItemText(m_iSelectedRow,1);
		CTokenizer t(sztemp);
		sztemp = t.GetNextToken(_T(">"));
		theProcBank.Loss_info.strSecondClass = sztemp;
		theProcBank.Loss_info.strSecondGroup.Format(_T("%s_SUB_%02d"),theProcBank.Loss_info.strFirstGroup,m_iSelectedRow-1);
	}
	else
	{
		sztemp=m_GridModelList.GetItemText(m_iSelectedRow,1);
		CTokenizer t(sztemp);
		sztemp = t.GetNextToken(_T(">"));
		theProcBank.Loss_info.strFirstClass = sztemp;
		sztemp = t.GetNextToken(_T(">"));
		theProcBank.Loss_info.strLossCode = sztemp;
		sztemp = t.GetNextToken(_T(">"));
		theProcBank.Loss_info.strSecondClass = sztemp;
		sztemp = t.GetNextToken(_T(">"));
		theProcBank.Loss_info.strThirdClass = sztemp;
		sztemp = t.GetNextToken(_T(">"));
		theProcBank.Loss_info.strThirdClass = sztemp;
}
	
	::SendMessage(this->m_hWnd, WM_CLOSE, NULL, NULL);
}

void DIgLossTimeList::OnGridRecipeEndSelChange( NMHDR *pNotifyStruct, LRESULT *pResult )
{
	CMainFrame *pMainFrm = (CMainFrame *)AfxGetMainWnd();
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	m_iSelectedRow = pItem->iRow;

}


void DIgLossTimeList::ClickGxbtnTeach1MoveUp()
{
	if ( m_iUpDownParam - 5 < 0  )
		m_iUpDownParam = 0;
	else
		m_iUpDownParam -=5;

	UpdateGird();
}


void DIgLossTimeList::ClickGxbtnTeach1MoveDown()
{
	int a = m_iUpDownParam + 5;
	int b = m_vecRecipe.size();

	if (a >= b)		return;
	else
		m_iUpDownParam +=5;

	UpdateGird();
}
