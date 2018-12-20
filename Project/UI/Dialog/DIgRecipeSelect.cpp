// DIgRecipeSelect.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DIgRecipeSelect.h"
#include "afxdialogex.h"
#include "BankGuild\RecipeBank.h"
#include "UI\GausGUI\GxMsgBox.h"
#include "UI\GausGUI\GxUICtrl.h"
// DIgRecipeSelect 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDIgRecipeSelect, CDialogEx)

	CDIgRecipeSelect::CDIgRecipeSelect(CWnd* pParent /*=NULL*/)
	: CDialog(CDIgRecipeSelect::IDD, pParent)
{
	m_iUpDownParam=0;
	m_iSelectedRow=-1;

}

CDIgRecipeSelect::~CDIgRecipeSelect()
{
}

void CDIgRecipeSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID_RECIPE_MODELLIST, m_GridModelList);
}


BEGIN_MESSAGE_MAP(CDIgRecipeSelect, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_GRID_RECIPE_MODELLIST, OnGridRecipeEndSelChange)
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
END_MESSAGE_MAP()


// DIgRecipeSelect 메시지 처리기입니다.
BEGIN_EVENTSINK_MAP(CDIgRecipeSelect, CDialogEx)
	ON_EVENT(CDIgRecipeSelect, IDC_GXBTN_RECIPE_CANCLE, DISPID_CLICK, CDIgRecipeSelect::ClickGxbtnRecipeCancle, VTS_NONE)
	ON_EVENT(CDIgRecipeSelect, IDC_GXBTN_RECIPE_SELECT, DISPID_CLICK, CDIgRecipeSelect::ClickGxbtnRecipeSelect, VTS_NONE)
	ON_EVENT(CDIgRecipeSelect, IDC_GXBTN_TEACH1_MOVE_UP, DISPID_CLICK, CDIgRecipeSelect::ClickGxbtnTeach1MoveUp, VTS_NONE)
	ON_EVENT(CDIgRecipeSelect, IDC_GXBTN_TEACH1_MOVE_DOWN, DISPID_CLICK, CDIgRecipeSelect::ClickGxbtnTeach1MoveDown, VTS_NONE)
END_EVENTSINK_MAP()

BOOL CDIgRecipeSelect::OnInitDialog()
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
	LoadRecipe();
	UpdateGird();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDIgRecipeSelect::UpdateGird()
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


void CDIgRecipeSelect::LoadRecipe()
{

	CString input;
	CString dir;
	CFileFind finder;

	m_vecRecipe.clear();
	input.Format(_T("D:\\Cheetah\\Recipe\\*"));


	BOOL bWorking = finder.FindFile(input);

	while (bWorking)
	{    
		bWorking = finder.FindNextFile();

		if(finder.IsDirectory() && !finder.IsDots())
		{
			dir= finder.GetFileName();
			input.Delete(input.GetLength()-1);
			m_vecRecipe.push_back(dir);

		}
	}
}

void CDIgRecipeSelect::ClickGxbtnRecipeCancle()
{
	::SendMessage(this->m_hWnd, WM_CLOSE, NULL, NULL);
}



void CDIgRecipeSelect::ClickGxbtnRecipeSelect()
{
	CString sztemp;
	if(m_iSelectedRow==-1 || m_iSelectedRow==0 || m_GridModelList.GetItemText(m_iSelectedRow,1).GetLength()==0 )
	{
		CGxMsgBox	dlgMsgBox;
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("사용할 레시피 모델을 선택해주세요."), 
			_T("Please select a recipe model to use."),		
			_T("Vui lòng chọn một recipe mô hình để sử dụng") );

		dlgMsgBox.DoModal();
		return;
	}
	CString strOldRecipeName = theRecipeBank.m_strRecipeName;
	sztemp=m_GridModelList.GetItemText(m_iSelectedRow,1);
	theRecipeBank.SetMainRecipeName(sztemp);
	theRecipeBank.ReadRecipeFile(sztemp);
	theRecipeBank.SaveLastRecipeFile();


	// Recipe 변경 보고 [11/30/2016 OSC]
	theSocketInterFace.SendCmdRecipeToDataPC(EQUIP_SET_PPID_CHANGE, theRecipeBank.m_strRecipeName, strOldRecipeName);
	::SendMessage(this->m_hWnd, WM_CLOSE, NULL, NULL);


}

void CDIgRecipeSelect::OnGridRecipeEndSelChange( NMHDR *pNotifyStruct, LRESULT *pResult )
{
	CMainFrame *pMainFrm = (CMainFrame *)AfxGetMainWnd();
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	m_iSelectedRow = pItem->iRow;

}


void CDIgRecipeSelect::ClickGxbtnTeach1MoveUp()
{
	if ( m_iUpDownParam - 5 < 0  )
		m_iUpDownParam = 0;
	else
		m_iUpDownParam -=5;

	UpdateGird();
}


void CDIgRecipeSelect::ClickGxbtnTeach1MoveDown()
{
	int a = m_iUpDownParam + 5;
	int b = m_vecRecipe.size();

	if (a >= b)		return;
	else
		m_iUpDownParam +=5;

	UpdateGird();
}


// 표시 언어가 변경되었다.
LRESULT CDIgRecipeSelect::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	CGxUICtrl::BroadcastStaticCpationIndex(this, wParam, IDC_GXLBL_RECIPE_TITLE1);
	CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_GXBTN_RECIPE_SELECT);

	return 0;
}
