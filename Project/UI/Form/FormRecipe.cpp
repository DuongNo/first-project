// FormRecipe.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "FormRecipe.h"
#include "MainFrm.h"
//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "UI\Dialog\DlgBoxKeyBoard.h"
#include <algorithm>
#include "UI\GausGUI\GxNumPadEx.h"
#include "UI\GausGUI\GxMsgBox.h"
#include "BankGuild\RecipeBank.h"
#include "ETC/FileSupport.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CFormRecipe

IMPLEMENT_DYNCREATE(CFormRecipe, CFormView)

CFormRecipe::CFormRecipe()
	: CFormView(CFormRecipe::IDD)
{
	m_iUpDownParam = 0;
	m_SetVelNum=15;// 설정할 값들의 종류
	m_iSetVelNumIndex=0;
	m_iSelectedRow=1;
	LoadRecipe();
	


}

CFormRecipe::~CFormRecipe()
{
	
}

void CFormRecipe::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID_RECIPE_MODELLIST, m_GridModelList);
}

BEGIN_MESSAGE_MAP(CFormRecipe, CFormView)
	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	ON_MESSAGE(UDMSG_RECIPE_UPDATE,			OnRecipeUpdate)	

	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	ON_NOTIFY(NM_CLICK, IDC_GRID_RECIPE_MODELLIST, OnGridRecipeEndSelChange)
END_MESSAGE_MAP()


BEGIN_EVENTSINK_MAP(CFormRecipe, CFormView)
	ON_EVENT(CFormRecipe, IDC_GXBTN_RECIPE_CREATE, DISPID_CLICK, CFormRecipe::ClickGxbtnTeach1Create, VTS_NONE)
	ON_EVENT(CFormRecipe, IDC_GXBTN_RECIPE_DELETE, DISPID_CLICK, CFormRecipe::ClickGxbtnRecipeDelete, VTS_NONE)
	ON_EVENT(CFormRecipe, IDC_GXBTN_TEACH1_MOVE_UP, DISPID_CLICK, CFormRecipe::ClickGxbtnRecipeMoveUp, VTS_NONE)
	ON_EVENT(CFormRecipe, IDC_GXBTN_TEACH1_MOVE_DOWN, DISPID_CLICK, CFormRecipe::ClickGxbtnRecipeMoveDown, VTS_NONE)
	ON_EVENT(CFormRecipe, IDC_GXBTN_RECIPE_EDIT, DISPID_CLICK, CFormRecipe::ClickGxbtnRecipeEdit, VTS_NONE)
	ON_EVENT(CFormRecipe, IDC_GXBTN_RECIPE_ITEM_VALUE1, DISPID_CLICK, CFormRecipe::ClickGxbtnRecipeCellWidth, VTS_NONE)
	ON_EVENT(CFormRecipe, IDC_GXBTN_RECIPE_ITEM_VALUE2, DISPID_CLICK, CFormRecipe::ClickGxbtnRecipeCellHeight, VTS_NONE)
	ON_EVENT(CFormRecipe, IDC_GXBTN_RECIPE_ITEM_VALUE3, DISPID_CLICK, CFormRecipe::ClickGxbtnRecipeCellFPCBUse, VTS_NONE)
	ON_EVENT(CFormRecipe, IDC_GXBTN_RECIPE_ITEM_VALUE4, DISPID_CLICK, CFormRecipe::ClickGxbtnRecipeItemValue4, VTS_NONE)
	ON_EVENT(CFormRecipe, IDC_GXBTN_RECIPE_ITEM_VALUE5, DISPID_CLICK, CFormRecipe::ClickGxbtnRecipeItemValue5, VTS_NONE)
	ON_EVENT(CFormRecipe, IDC_GXBTN_RECIPE_ITEM_VALUE6, DISPID_CLICK, CFormRecipe::ClickGxbtnRecipeItemValue6, VTS_NONE)
	ON_EVENT(CFormRecipe, IDC_GXBTN_RECIPE_ITEM_VALUE7, DISPID_CLICK, CFormRecipe::ClickGxbtnRecipeItemValue7, VTS_NONE)
	ON_EVENT(CFormRecipe, IDC_GXBTN_RECIPE_ITEM_VALUE9, DISPID_CLICK, CFormRecipe::ClickGxbtnRecipeTrayCountX, VTS_NONE)
	ON_EVENT(CFormRecipe, IDC_GXBTN_RECIPE_ITEM_VALUE10, DISPID_CLICK, CFormRecipe::ClickGxbtnRecipeTrayCountY, VTS_NONE)
	ON_EVENT(CFormRecipe, IDC_GXBTN_RECIPE_ITEM_VALUE11, DISPID_CLICK, CFormRecipe::ClickGxbtnRecipeTrayPitchX, VTS_NONE)
	ON_EVENT(CFormRecipe, IDC_GXBTN_RECIPE_ITEM_VALUE12, DISPID_CLICK, CFormRecipe::ClickGxbtnRecipeTrayPitchY, VTS_NONE)
	ON_EVENT(CFormRecipe, IDC_GXBTN_RECIPE_ITEM_VALUE13, DISPID_CLICK, CFormRecipe::ClickGxbtnRecipeTrayPitchOffsetX, VTS_NONE)
	ON_EVENT(CFormRecipe, IDC_GXBTN_RECIPE_ITEM_VALUE14, DISPID_CLICK, CFormRecipe::ClickGxbtnRecipeTrayPitchOffsetY, VTS_NONE)
	ON_EVENT(CFormRecipe, IDC_GXBTN_RECIPE_ITEM_VALUE15, DISPID_CLICK, CFormRecipe::ClickGxbtnRecipeTrayThickness, VTS_NONE)
	ON_EVENT(CFormRecipe, IDC_GXBTN_RECIPE_ITEM_VALUE16, DISPID_CLICK, CFormRecipe::ClickGxbtnRecipeItemValue16, VTS_NONE)
	ON_EVENT(CFormRecipe, IDC_GXBTN_RECIPE_MODIFY_SAVE, DISPID_CLICK, CFormRecipe::ClickGxbtnRecipeModifySave, VTS_NONE)
	ON_EVENT(CFormRecipe, IDC_GXBTN_RECIPE_MODIFY_CANCLE, DISPID_CLICK, CFormRecipe::ClickGxbtnRecipeModifyCancle, VTS_NONE)

	ON_EVENT(CFormRecipe, IDC_GXBTN_RULE_INFO_READ, DISPID_CLICK, CFormRecipe::ClickGxbtnRuleInfoRead, VTS_NONE)
	ON_EVENT(CFormRecipe, IDC_GXBTN_CURRENT_PRODUCT, DISPID_CLICK, CFormRecipe::ClickGxbtnCurrentProduct, VTS_NONE)
	ON_EVENT(CFormRecipe, IDC_GXBTN_CURRENT_PPID, DISPID_CLICK, CFormRecipe::ClickGxbtnCurrentPpid, VTS_NONE)
	ON_EVENT(CFormRecipe, IDC_GXBTN_TRAY_QTY_IN_LOT, DISPID_CLICK, CFormRecipe::ClickGxbtnTrayQtyInLot, VTS_NONE)
	ON_EVENT(CFormRecipe, IDC_GXBTN_CELL_QTY_IN_LOT, DISPID_CLICK, CFormRecipe::ClickGxbtnCellQtyInLot, VTS_NONE)
	ON_EVENT(CFormRecipe, IDC_GXBTN_NEXT_STEPID, DISPID_CLICK, CFormRecipe::ClickGxbtnNextStepid, VTS_NONE)
	ON_EVENT(CFormRecipe, IDC_GXBTN_RULE_INFO_SAVE, DISPID_CLICK, CFormRecipe::ClickGxbtnRuleInfoSave, VTS_NONE)
END_EVENTSINK_MAP()

// CFormRecipe 진단입니다.

#ifdef _DEBUG
void CFormRecipe::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormRecipe::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormRecipe 메시지 처리기입니다.



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
LRESULT CFormRecipe::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	CMainFrame *pMainFrm = (CMainFrame *)AfxGetMainWnd();
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if(pMainFrm->m_SetValue_Flag && m_vecSetValue!=m_vecSetValueTemp)
	{
		ClickGxbtnRecipeModifySave();
	}
	InitRecipeGrid();
	LoadRecipe();
	UpdateGird();
	//--LoadRecipe();  //2017-03-22,skcho, 위로 위치 이동, 상위에서 생성시 업데이트가 안되는 현상 수정
	UpdateSetValue(m_iSelectedRow-1);
	m_GridModelList.SetSelectedRange(m_iSelectedRow,1,m_iSelectedRow,1);

	SetValueUse(pMainFrm->m_SetValue_Flag);
	if ( bFlag )
	{
		
		
		
	}
	else
	{

	}

	UpdateRuleInfo();  //2017-03-09,skcho, rule info 업데이트

	return 0;
}

// 표시 언어가 변경되었다.
LRESULT CFormRecipe::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	CGxUICtrl::BroadcastStaticCpationIndex(this, wParam, IDC_GXLBL_RECIPE_TITLE1);
	CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_GXBTN_RECIPE_CREATE);

	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

// 윈도우 생성전에 모양을 달리 지정한다.
BOOL CFormRecipe::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	cs.style = WS_OVERLAPPED | WS_CHILD;				// 테두리를 없앤다.

	return CFormView::PreCreateWindow(cs);
}



void CFormRecipe::InitRecipeGrid()
{
	if(m_GridModelList.GetSafeHwnd() == NULL) return;

	//Grid 설정
	m_GridModelList.SetEditable(FALSE);
	m_GridModelList.SetDefCellHeight(30);
	m_GridModelList.SetFixedColumnCount(1); 
	m_GridModelList.SetFixedRowCount(1);
	m_GridModelList.SetRowCount(4);
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
}

void CFormRecipe::UpdateGird()
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


void CFormRecipe::LoadRecipe()
{
	CString input;
	CString dir;
	CString TeachFile;
	CFileFind finder;
	input.Format(_T("D:\\Cheetah\\Recipe\\*"));
	m_vecRecipeTemp.clear();
	
	//m_vecRecipe.clear();
	m_vecSetValue.clear();
	
	BOOL bWorking = finder.FindFile(input);

	while (bWorking)
	{    
		bWorking = finder.FindNextFile();

		if(finder.IsDirectory() && !finder.IsDots())
		{
			dir= finder.GetFileName();
			
			// 레시피 불러 들일때 Teach 파일 없을 경우에 해당 모델 삭제 한다. 2017/06/15 HSK.
			TeachFile.Format(_T("D:\\Cheetah\\Recipe\\%s\\Teach.ini"),dir);
			if(CFileSupport::FileCheck(TeachFile) == FALSE)
			{
				TeachFile.Format(_T("D:\\Cheetah\\Recipe\\%s"),dir);
				CFileSupport::DeleteDirectory(TeachFile);
				// 상위 보고 할지 말지는 협의를 해봐야 할 것 같은데....2017/06/15 HSK.
				theSocketInterFace.SendCmdRecipeToDataPC(EQUIP_SET_PPID_DELETE, dir);
			}
			else
			{
				input.Delete(input.GetLength()-1);
				m_vecRecipeTemp.push_back(dir);
			}			
		}
	}
	if(m_vecRecipe.size()!=m_vecRecipeTemp.size())
	{
		int vec_size=(int)m_vecRecipe.size();
		for(int i=0; i<(int)m_vecRecipeTemp.size()-vec_size;i++)
		{
			m_vecRecipe.push_back(m_vecRecipeTemp[vec_size+i]);
		

		}
	}
	for(int i=0; i<(int)m_vecRecipe.size(); i++)
	{
		CString sztemp;
		m_RecipeBank.ReadRecipeFile(m_vecRecipe[i]);
		sztemp.Format(_T("%f"),m_RecipeBank.m_Cell.m_dWidth);
		m_vecSetValue.push_back(sztemp);
		sztemp.Format(_T("%f"),m_RecipeBank.m_Cell.m_dHeight);
		m_vecSetValue.push_back(sztemp);
		if(m_RecipeBank.m_Cell.m_bFPCBuse)
		{
			m_vecSetValue.push_back(_T("Y"));
		}
		else
		{
			m_vecSetValue.push_back(_T("N"));
		}
		sztemp.Format(_T("%f"),m_RecipeBank.m_Cell.m_dMarkDistance);
		m_vecSetValue.push_back(sztemp);
		sztemp.Format(_T(""));
		m_vecSetValue.push_back(sztemp);
		m_vecSetValue.push_back(sztemp);
		m_vecSetValue.push_back(sztemp);
		sztemp.Format(_T("%d"),m_RecipeBank.m_Tray.m_nCountX);
		m_vecSetValue.push_back(sztemp);
		sztemp.Format(_T("%d"),m_RecipeBank.m_Tray.m_nCountY);
		m_vecSetValue.push_back(sztemp);
		sztemp.Format(_T("%f"),m_RecipeBank.m_Tray.m_dPitchX);
		m_vecSetValue.push_back(sztemp);
		sztemp.Format(_T("%f"),m_RecipeBank.m_Tray.m_dPitchY);
		m_vecSetValue.push_back(sztemp);
		sztemp.Format(_T("%f"),m_RecipeBank.m_Tray.m_dPitchOffsetX);
		m_vecSetValue.push_back(sztemp);
		sztemp.Format(_T("%f"),m_RecipeBank.m_Tray.m_dPitchOffsetY);
		m_vecSetValue.push_back(sztemp);
		sztemp.Format(_T("%f"),m_RecipeBank.m_Tray.m_dTrayThickness);
		m_vecSetValue.push_back(sztemp);
		sztemp.Format(_T("	"));
		m_vecSetValue.push_back(sztemp);


	}
	m_vecSetValueTemp=m_vecSetValue;
}
		
		

void CFormRecipe::UpdateSetValue(int sel)
{	
	if (sel >= (int)m_vecRecipe.size())
	{
		//존재하지 않는 레시피 모델 클릭 시 모델별 설정 값 표시 안하게 함.
		ClearSetValue();
		return;
	}
	// 존재하는 레시피 모델 클릭 시 해당 모델별 설정 값을 표시
	CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE1, m_vecSetValue[sel*m_SetVelNum]);
	CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE2, m_vecSetValue[sel*m_SetVelNum+1]);
	CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE3, m_vecSetValue[sel*m_SetVelNum+2]);
	CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE4, m_vecSetValue[sel*m_SetVelNum+3]);
	CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE5,  m_vecSetValue[sel*m_SetVelNum+4]);
	CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE6,  m_vecSetValue[sel*m_SetVelNum+5]);
	CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE7,  m_vecSetValue[sel*m_SetVelNum+6]);
	CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE9,  m_vecSetValue[sel*m_SetVelNum+7]);
	CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE10,  m_vecSetValue[sel*m_SetVelNum+8]);
	CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE11,  m_vecSetValue[sel*m_SetVelNum+9]);
	CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE12,  m_vecSetValue[sel*m_SetVelNum+10]);
	CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE13,  m_vecSetValue[sel*m_SetVelNum+11]);
	CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE14,  m_vecSetValue[sel*m_SetVelNum+12]);
	CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE15,  m_vecSetValue[sel*m_SetVelNum+13]);
	CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE16,  m_vecSetValue[sel*m_SetVelNum+14]);
		
}

void CFormRecipe::ClearSetValue()
{	
	
	CMainFrame *pMainFrm = (CMainFrame *)AfxGetMainWnd();
	SetValueUse(pMainFrm->m_SetValue_Flag);
	CString val;
	val.Format(_T(""));
	CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE1, val);
	CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE2, val);
	CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE3, val);
	CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE4, val);
	CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE5, val);
	CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE6, val);
	CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE7, val);
	CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE9, val);
	CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE10, val);
	CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE11, val);
	CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE12, val);
	CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE13, val);
	CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE14, val);
	CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE15, val);
	CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE16, val);
			
}

void CFormRecipe::ClickGxbtnTeach1Create()
{
	CDlgBoxKeyBoard dlgKey;
	CString sztemp,strRecipeName;
	RecipeChangeConfirm();
	if ( dlgKey.DoModal() == IDOK )
	{
		CString szInput = dlgKey.m_strReturnText;
		
		// 중복검사
		if ( std::find(m_vecRecipe.begin(), m_vecRecipe.end(), szInput) != m_vecRecipe.end() )
		{
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("이미 존재하고 있는 레시피 입니다."), 
				_T("Existing Recipe"),		
				_T("Một item mà tồn tại.") );

			dlgMsgBox.DoModal();
			return;
		}
		//신규 레시피가 생성되면 임의의 초기값들을 입력해준다.
		m_vecRecipe.push_back(szInput);
		//20170201 byskcho
		//m_RecipeBank.m_strRecipeName은 현재 선택된   모델명이므로 신규모델명을 저장하면 안된다.
		//m_RecipeBank.m_strRecipeName=szInput;
		strRecipeName = szInput;

		//cell 설정 초기값 입력
		sztemp.Format(_T("%f"),theRecipeBank.m_Cell.m_dWidth);
		m_vecSetValue.push_back(sztemp);
		m_RecipeBank.m_Cell.m_dWidth=theRecipeBank.m_Cell.m_dWidth;

		sztemp.Format(_T("%f"),theRecipeBank.m_Cell.m_dHeight);
		m_vecSetValue.push_back(sztemp);
		m_RecipeBank.m_Cell.m_dHeight=theRecipeBank.m_Cell.m_dHeight;;

		if(theRecipeBank.m_Cell.m_bFPCBuse)
		{
			sztemp.Format(_T("Y"));
		}
		else
		{
			sztemp.Format(_T("N"));
		}
		m_vecSetValue.push_back(sztemp);
		m_RecipeBank.m_Cell.m_bFPCBuse=theRecipeBank.m_Cell.m_bFPCBuse;

		sztemp.Format(_T("%f"),theRecipeBank.m_Cell.m_dMarkDistance);
		m_vecSetValue.push_back(sztemp);
		m_RecipeBank.m_Cell.m_dMarkDistance=theRecipeBank.m_Cell.m_dMarkDistance;;

		sztemp.Format(_T(""));
		m_vecSetValue.push_back(sztemp);
		m_vecSetValue.push_back(sztemp);
		m_vecSetValue.push_back(sztemp);

		//tray 설정 초기값 입력
		sztemp.Format(_T("%d"),theRecipeBank.m_Tray.m_nCountX);
		m_vecSetValue.push_back(sztemp);
		m_RecipeBank.m_Tray.m_nCountX=theRecipeBank.m_Tray.m_nCountX;

		sztemp.Format(_T("%d"),theRecipeBank.m_Tray.m_nCountY);
		m_vecSetValue.push_back(sztemp);
		m_RecipeBank.m_Tray.m_nCountY=theRecipeBank.m_Tray.m_nCountY;

		sztemp.Format(_T("%f"),theRecipeBank.m_Tray.m_dPitchX);
		m_vecSetValue.push_back(sztemp);
		m_RecipeBank.m_Tray.m_dPitchX=theRecipeBank.m_Tray.m_dPitchX;

		sztemp.Format(_T("%f"),theRecipeBank.m_Tray.m_dPitchY);
		m_vecSetValue.push_back(sztemp);
		m_RecipeBank.m_Tray.m_dPitchY=theRecipeBank.m_Tray.m_dPitchY;

		sztemp.Format(_T("%f"),theRecipeBank.m_Tray.m_dPitchOffsetX);
		m_vecSetValue.push_back(sztemp);
		m_RecipeBank.m_Tray.m_dPitchOffsetX=theRecipeBank.m_Tray.m_dPitchOffsetX;

		sztemp.Format(_T("%f"),theRecipeBank.m_Tray.m_dPitchOffsetY);
		m_vecSetValue.push_back(sztemp);
		m_RecipeBank.m_Tray.m_dPitchOffsetY=theRecipeBank.m_Tray.m_dPitchOffsetY;

		sztemp.Format(_T("%f"),theRecipeBank.m_Tray.m_dTrayThickness);
		m_vecSetValue.push_back(sztemp);
		m_RecipeBank.m_Tray.m_dTrayThickness=theRecipeBank.m_Tray.m_dTrayThickness;

		sztemp.Format(_T("	"));
		m_vecSetValue.push_back(sztemp);

		m_RecipeBank.SaveRecipeFile(szInput);

		//20170201 byskcho
		//레시피 생성시 티칭값 생성
		theTeachBank.SaveTeachFile(strRecipeName);

		if(m_iSetVelNumIndex==0)
		{
			m_iSetVelNumIndex=1;
		}
		UpdateSetValue(m_iSetVelNumIndex-1);
		LoadRecipe();
		UpdateGird();								// 그리드를 다시 그려준다.	

		// Recipe 생성 보고 [11/30/2016 OSC]
		theSocketInterFace.SendCmdRecipeToDataPC(EQUIP_SET_PPID_CREATE, szInput);
	}
}
void CFormRecipe::RecipeChangeConfirm()
{
	CMainFrame *pMainFrm = (CMainFrame *)AfxGetMainWnd();
	
	
	if(pMainFrm->m_SetValue_Flag )
	{
		pMainFrm->m_SetValue_Flag=FALSE;
		ClickGxbtnRecipeModifySave();
		
	
	}
}

void CFormRecipe::OnGridRecipeEndSelChange(NMHDR *pNotifyStruct, LRESULT *pResult)
{
	CString sztemp;
	CMainFrame *pMainFrm = (CMainFrame *)AfxGetMainWnd();
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;	
	RecipeChangeConfirm();

	m_vecSetValueTemp.clear();
	m_vecSetValueTemp=m_vecSetValue;
	if ( pItem->iRow < 1 )		return;				// 메뉴 정의된 Row Click시 return

	m_iSelectedRow = pItem->iRow;
	sztemp=m_GridModelList.GetItemText(m_iSelectedRow,0);
	m_iSetVelNumIndex= _ttoi(sztemp);
	UpdateSetValue(m_iSetVelNumIndex-1);
	
	//모델 편집 버튼 실행여부 초기화
	pMainFrm->m_SetValue_Flag=FALSE;
	SetValueUse(pMainFrm->m_SetValue_Flag);
		
}

void CFormRecipe:: SetValueUse(BOOL flag)
{
		CGxUICtrl::SetStaticColor(this, IDC_GXBTN_RECIPE_ITEM_VALUE1, ((flag) ?		GXCOLOR_WHITE : GXCOLOR_OFF));
		CGxUICtrl::SetStaticColor(this, IDC_GXBTN_RECIPE_ITEM_VALUE2, ((flag) ?		GXCOLOR_WHITE : GXCOLOR_OFF));
		CGxUICtrl::SetStaticColor(this, IDC_GXBTN_RECIPE_ITEM_VALUE3, ((flag) ?		GXCOLOR_WHITE : GXCOLOR_OFF));
		CGxUICtrl::SetStaticColor(this, IDC_GXBTN_RECIPE_ITEM_VALUE4, ((flag) ?		GXCOLOR_WHITE : GXCOLOR_OFF));
 		CGxUICtrl::SetStaticColor(this, IDC_GXBTN_RECIPE_ITEM_VALUE9, ((flag) ?		GXCOLOR_WHITE : GXCOLOR_OFF));
		CGxUICtrl::SetStaticColor(this, IDC_GXBTN_RECIPE_ITEM_VALUE10, ((flag) ?		GXCOLOR_WHITE : GXCOLOR_OFF));
		CGxUICtrl::SetStaticColor(this, IDC_GXBTN_RECIPE_ITEM_VALUE11, ((flag) ?		GXCOLOR_WHITE : GXCOLOR_OFF));
		CGxUICtrl::SetStaticColor(this, IDC_GXBTN_RECIPE_ITEM_VALUE12, ((flag) ?		GXCOLOR_WHITE : GXCOLOR_OFF));
		CGxUICtrl::SetStaticColor(this, IDC_GXBTN_RECIPE_ITEM_VALUE13, ((flag) ?		GXCOLOR_WHITE : GXCOLOR_OFF));
		CGxUICtrl::SetStaticColor(this, IDC_GXBTN_RECIPE_ITEM_VALUE14, ((flag) ?		GXCOLOR_WHITE : GXCOLOR_OFF));
		CGxUICtrl::SetStaticColor(this, IDC_GXBTN_RECIPE_ITEM_VALUE15, ((flag) ?		GXCOLOR_WHITE : GXCOLOR_OFF));
		CGxUICtrl::SetStaticTextColor(this,IDC_GXBTN_RECIPE_ITEM_VALUE1, ((flag) ?		GXCOLOR_BLACK : GXCOLOR_GRAY));
		CGxUICtrl::SetStaticTextColor(this,IDC_GXBTN_RECIPE_ITEM_VALUE2, ((flag) ?		GXCOLOR_BLACK : GXCOLOR_GRAY));
		CGxUICtrl::SetStaticTextColor(this,IDC_GXBTN_RECIPE_ITEM_VALUE3, ((flag) ?		GXCOLOR_BLACK : GXCOLOR_GRAY));
		CGxUICtrl::SetStaticTextColor(this,IDC_GXBTN_RECIPE_ITEM_VALUE4, ((flag) ?		GXCOLOR_BLACK : GXCOLOR_GRAY));
 		CGxUICtrl::SetStaticTextColor(this,IDC_GXBTN_RECIPE_ITEM_VALUE9, ((flag) ?		GXCOLOR_BLACK : GXCOLOR_GRAY));
		CGxUICtrl::SetStaticTextColor(this,IDC_GXBTN_RECIPE_ITEM_VALUE10, ((flag) ?		GXCOLOR_BLACK : GXCOLOR_GRAY));
		CGxUICtrl::SetStaticTextColor(this,IDC_GXBTN_RECIPE_ITEM_VALUE11, ((flag) ?		GXCOLOR_BLACK : GXCOLOR_GRAY));
		CGxUICtrl::SetStaticTextColor(this,IDC_GXBTN_RECIPE_ITEM_VALUE12, ((flag) ?		GXCOLOR_BLACK : GXCOLOR_GRAY));
		CGxUICtrl::SetStaticTextColor(this,IDC_GXBTN_RECIPE_ITEM_VALUE13, ((flag) ?		GXCOLOR_BLACK : GXCOLOR_GRAY));
		CGxUICtrl::SetStaticTextColor(this,IDC_GXBTN_RECIPE_ITEM_VALUE14, ((flag) ?		GXCOLOR_BLACK : GXCOLOR_GRAY));
		CGxUICtrl::SetStaticTextColor(this,IDC_GXBTN_RECIPE_ITEM_VALUE15, ((flag) ?		GXCOLOR_BLACK : GXCOLOR_GRAY));	
}


void CFormRecipe::ClickGxbtnRecipeDelete()
{
	CMainFrame *pMainFrm = (CMainFrame *)AfxGetMainWnd();
	CGxMsgBox	dlgMsgBox;
	RecipeChangeConfirm();
	int i=0;
	if ( m_iSelectedRow == 0 )	m_iSelectedRow=1;
	if ( m_vecRecipe.size() < UINT(m_iSelectedRow + m_iUpDownParam) )
		return;
	

	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
	dlgMsgBox.SetMessage(TRUE, 
		_T("정말로 삭제하시겠습니까?"), 
		_T("Are you sure you want to Delete?"),		
		_T("Bạn có chắc chắn muốn xóa?") );

	int nRet = dlgMsgBox.DoModal();
	if(nRet != IDOK)
		return;
	
		
	std::vector<CString>::iterator iter = m_vecRecipe.begin();
	pMainFrm->m_SetValue_Flag=FALSE;
	CString strRecipeName;

	while(iter != m_vecRecipe.end() )
	{
		if(theRecipeBank.m_strRecipeName== m_vecRecipe[m_iSelectedRow+m_iUpDownParam - 1] )
		{
			//현재 사용중이므로 삭제가 불가능 하다는 메세지 출력
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("현재 사용중인 레시피 입니다."), 
				_T("The recipe currently in use."),		
				_T("Các mục bạn đang sử dụng.") );

			dlgMsgBox.DoModal();
			return;
		}
		if ( iter->GetString() == m_vecRecipe[m_iSelectedRow+m_iUpDownParam - 1] )
		{
			strRecipeName = iter->GetString();
			CString path;
			path.Format(_T("D:\\Cheetah\\Recipe\\"+strRecipeName));
			m_vecRecipe.erase(iter);
			CFileSupport::DeleteDirectory( (LPCTSTR)path); //DeleteDirectory(dir);

			//삭제된 모델별 설정값 만큼 따라서 삭제
			m_vecSetValue.erase(m_vecSetValue.begin()+(i*m_SetVelNum),m_vecSetValue.begin()+(i*m_SetVelNum)+m_SetVelNum);
			break;
		}
		else
		{
			++iter;
			++i;
		}
	}

	UpdateGird();
	UpdateSetValue(i);	

	// Recipe 삭제 보고 [11/30/2016 OSC]
	theSocketInterFace.SendCmdRecipeToDataPC(EQUIP_SET_PPID_DELETE, strRecipeName);
}


void CFormRecipe::ClickGxbtnRecipeMoveUp()
{
	if ( m_iUpDownParam - 5 < 0  )
		m_iUpDownParam = 0;
	else
		m_iUpDownParam -=5;

	UpdateGird();
}


void CFormRecipe::ClickGxbtnRecipeMoveDown()
{
	int a = m_iUpDownParam + 5;
	int b = m_vecRecipe.size();

	if (a >= b)		return;
	else
		m_iUpDownParam +=5;

	UpdateGird();
}


void CFormRecipe::ClickGxbtnRecipeEdit()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CMainFrame *pMainFrm = (CMainFrame *)AfxGetMainWnd();
	// 레시피 창에서 모델편집이 가능하도록 변경
	if(m_vecRecipe.size()==0)
	{
		return;
	}
	pMainFrm->m_SetValue_Flag=TRUE;
	SetValueUse(pMainFrm->m_SetValue_Flag);
			
}



void CFormRecipe::ClickGxbtnRecipeCellWidth()
{
	CMainFrame *pMainFrm = (CMainFrame *)AfxGetMainWnd();
	if(pMainFrm->m_SetValue_Flag)// 모델 편집 버튼 클릭이 되었다면
	{
		CGxNumPadEx	dlgNumPad;
		if(m_iSetVelNumIndex==0)
			m_iSetVelNumIndex=1;
		double dOldValue = _tstof(m_vecSetValueTemp[(m_iSetVelNumIndex-1)*m_SetVelNum]);
		dlgNumPad.SetTitle(_T("Cell Setiing width Change"));
		dlgNumPad.SetValue(0.0, 200.0, dOldValue);
		if ( dlgNumPad.DoModal() == IDOK )
		{
			int findOffset=0;
			CString szInput = dlgNumPad.GetValue();
			//변경된 설정값을 임시 저장
			m_vecSetValueTemp[(m_iSetVelNumIndex-1)*m_SetVelNum]=szInput;
			CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE1, szInput);
				
		}
	else// 모델 편집 버튼 클릭 하지 않았다면
		return;
	}
	
}





void CFormRecipe::ClickGxbtnRecipeCellHeight()
{
	
	CMainFrame *pMainFrm = (CMainFrame *)AfxGetMainWnd();
	if(pMainFrm->m_SetValue_Flag)// 모델 편집 버튼 클릭이 되었다면
	{
		CGxNumPadEx	dlgNumPad;
		if(m_iSetVelNumIndex==0)
			m_iSetVelNumIndex=1;
		double dOldValue = _tstof(m_vecSetValueTemp[(m_iSetVelNumIndex-1)*m_SetVelNum+1]);
		dlgNumPad.SetTitle(_T("Cell Setiing height Change"));
		dlgNumPad.SetValue(0.0, 200.0, dOldValue);
		if ( dlgNumPad.DoModal() == IDOK )
		{
			CString szInput = dlgNumPad.GetValue();
			//변경된 설정값을 임시 저장
			m_vecSetValueTemp[(m_iSetVelNumIndex-1)*m_SetVelNum+1]=szInput;
			CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE2, szInput);
				
		}
	else// 모델 편집 버튼 클릭 하지 않았다면
		return;


	}

}


void CFormRecipe::ClickGxbtnRecipeCellFPCBUse()
{
	CMainFrame *pMainFrm = (CMainFrame *)AfxGetMainWnd();
	if(pMainFrm->m_SetValue_Flag)// 모델 편집 버튼 클릭이 되었다면
	{
		if(m_iSetVelNumIndex==0)
			m_iSetVelNumIndex=1;
		if(m_vecSetValueTemp[(m_iSetVelNumIndex-1)*m_SetVelNum+2]==_T("Y"))
		{
			m_vecSetValueTemp[(m_iSetVelNumIndex-1)*m_SetVelNum+2]=_T("N");
			CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE3,_T("N"));
		}
		else
		{
			m_vecSetValueTemp[(m_iSetVelNumIndex-1)*m_SetVelNum+2]=_T("Y");
			CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE3,_T("Y"));
		}
	}
	else
	{
		return;
	}
}


void CFormRecipe::ClickGxbtnRecipeItemValue4()
{
	CMainFrame *pMainFrm = (CMainFrame *)AfxGetMainWnd();
	if(pMainFrm->m_SetValue_Flag)// 모델 편집 버튼 클릭이 되었다면
	{
		CGxNumPadEx	dlgNumPad;
		if(m_iSetVelNumIndex==0)
			m_iSetVelNumIndex=1;
		double dOldValue = _tstof(m_vecSetValueTemp[(m_iSetVelNumIndex-1)*m_SetVelNum+3]);
		dlgNumPad.SetTitle(_T("Cell Setiing Mark distance Change"));
		dlgNumPad.SetValue(0.0, 200.0, dOldValue);
		if ( dlgNumPad.DoModal() == IDOK )
		{
			CString szInput = dlgNumPad.GetValue();
			//변경된 설정값을 임시 저장
			m_vecSetValueTemp[(m_iSetVelNumIndex-1)*m_SetVelNum+3]=szInput;
			CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE4, szInput);
				
		}
	else// 모델 편집 버튼 클릭 하지 않았다면
		return;


	}
}


void CFormRecipe::ClickGxbtnRecipeItemValue5()
{

}


void CFormRecipe::ClickGxbtnRecipeItemValue6()
{

}


void CFormRecipe::ClickGxbtnRecipeItemValue7()
{
	
}


void CFormRecipe::ClickGxbtnRecipeTrayCountX()
{
	CMainFrame *pMainFrm = (CMainFrame *)AfxGetMainWnd();
	if(pMainFrm->m_SetValue_Flag)// 모델 편집 버튼 클릭이 되었다면
	{
		CGxNumPadEx	dlgNumPad;
		if(m_iSetVelNumIndex==0)
			m_iSetVelNumIndex=1;
		double dOldValue = _tstof(m_vecSetValueTemp[(m_iSetVelNumIndex-1)*m_SetVelNum+7]);
		dlgNumPad.SetTitle(_T("Tray Setting CountX Change"));
		dlgNumPad.SetValue(0.0, 20.0, dOldValue);
		dlgNumPad.SetMode(true, true);
		if ( dlgNumPad.DoModal() == IDOK )
		{
			CString szInput = dlgNumPad.GetValue();
			
			m_vecSetValueTemp[(m_iSetVelNumIndex-1)*m_SetVelNum+7]=szInput;
			CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE9, szInput);
		}
	else// 모델 편집 버튼 클릭 하지 않았다면
		return;


	}
	
}


void CFormRecipe::ClickGxbtnRecipeTrayCountY()
{
	CMainFrame *pMainFrm = (CMainFrame *)AfxGetMainWnd();
	if(pMainFrm->m_SetValue_Flag)// 모델 편집 버튼 클릭이 되었다면
	{
		CGxNumPadEx	dlgNumPad;
		if(m_iSetVelNumIndex==0)
			m_iSetVelNumIndex=1;
		
		double dOldValue = _tstof(m_vecSetValueTemp[(m_iSetVelNumIndex-1)*m_SetVelNum+8]);
		dlgNumPad.SetTitle(_T("Tray Setting CountY Change"));
		dlgNumPad.SetValue(0.0, 20.0, dOldValue);
		dlgNumPad.SetMode(true, true);
		if ( dlgNumPad.DoModal() == IDOK )
		{
			CString szInput = dlgNumPad.GetValue();
			//변경된 설정값을 임시 저장
			m_vecSetValueTemp[(m_iSetVelNumIndex-1)*m_SetVelNum+8]=szInput;
			CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE10, szInput);
		}
	else// 모델 편집 버튼 클릭 하지 않았다면
		return;


	}

}


void CFormRecipe::ClickGxbtnRecipeTrayPitchX()
{
	CMainFrame *pMainFrm = (CMainFrame *)AfxGetMainWnd();
	if(pMainFrm->m_SetValue_Flag)// 모델 편집 버튼 클릭이 되었다면
	{
		CGxNumPadEx	dlgNumPad;
		if(m_iSetVelNumIndex==0)
			m_iSetVelNumIndex=1;
		double dOldValue = _tstof(m_vecSetValueTemp[(m_iSetVelNumIndex-1)*m_SetVelNum+9]);
		dlgNumPad.SetTitle(_T("Tray Setting PitchX Change"));
		dlgNumPad.SetValue(0.0, 1000.0, dOldValue);
		if ( dlgNumPad.DoModal() == IDOK )
		{
			CString szInput = dlgNumPad.GetValue();
			//변경된 설정값을 임시 저장
			m_vecSetValueTemp[(m_iSetVelNumIndex-1)*m_SetVelNum+9]=szInput;
			CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE11, szInput);
		}
	else// 모델 편집 버튼 클릭 하지 않았다면
		return;


	}

}


void CFormRecipe::ClickGxbtnRecipeTrayPitchY()
{
	CMainFrame *pMainFrm = (CMainFrame *)AfxGetMainWnd();
	if(pMainFrm->m_SetValue_Flag)// 모델 편집 버튼 클릭이 되었다면
	{
		if(m_iSetVelNumIndex==0)
			m_iSetVelNumIndex=1;
		CGxNumPadEx	dlgNumPad;
		double dOldValue = _tstof(m_vecSetValueTemp[(m_iSetVelNumIndex-1)*m_SetVelNum+10]);
		dlgNumPad.SetTitle(_T("Tray Setting PitchY Change"));
		dlgNumPad.SetValue(0.0, 1000.0, dOldValue);
		if ( dlgNumPad.DoModal() == IDOK )
		{
			CString szInput = dlgNumPad.GetValue();
			//변경된 설정값을 임시 저장
			m_vecSetValueTemp[(m_iSetVelNumIndex-1)*m_SetVelNum+10]=szInput;
			CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE12, szInput);
		}
	else// 모델 편집 버튼 클릭 하지 않았다면
		return;


	}
	
}


void CFormRecipe::ClickGxbtnRecipeTrayPitchOffsetX()
{
	CMainFrame *pMainFrm = (CMainFrame *)AfxGetMainWnd();
	if(pMainFrm->m_SetValue_Flag)// 모델 편집 버튼 클릭이 되었다면
	{
		if(m_iSetVelNumIndex==0)
			m_iSetVelNumIndex=1;
		CGxNumPadEx	dlgNumPad;
		double dOldValue = _tstof(m_vecSetValueTemp[(m_iSetVelNumIndex-1)*m_SetVelNum+11]);
		dlgNumPad.SetTitle(_T("Tray Setting PitchOffsetX Change"));
		dlgNumPad.SetValue(0.0, 1000.0, dOldValue);
		if ( dlgNumPad.DoModal() == IDOK )
		{
			CString szInput = dlgNumPad.GetValue();
			//변경된 설정값을 임시 저장
			m_vecSetValueTemp[(m_iSetVelNumIndex-1)*m_SetVelNum+11]=szInput;
			CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE13, szInput);
		}
	else// 모델 편집 버튼 클릭 하지 않았다면
		return;


	}
	
}


void CFormRecipe::ClickGxbtnRecipeTrayPitchOffsetY()
{
	CMainFrame *pMainFrm = (CMainFrame *)AfxGetMainWnd();
	if(pMainFrm->m_SetValue_Flag)// 모델 편집 버튼 클릭이 되었다면
	{
		if(m_iSetVelNumIndex==0)
			m_iSetVelNumIndex=1;
		CGxNumPadEx	dlgNumPad;
		double dOldValue = _tstof(m_vecSetValueTemp[(m_iSetVelNumIndex-1)*m_SetVelNum+12]);
		dlgNumPad.SetTitle(_T("Tray Setting PitchOffsetY Change"));
		dlgNumPad.SetValue(0.0, 1000.0, dOldValue);
		if ( dlgNumPad.DoModal() == IDOK )
		{
			CString szInput = dlgNumPad.GetValue();
			//변경된 설정값을 임시 저장
			m_vecSetValueTemp[(m_iSetVelNumIndex-1)*m_SetVelNum+12]=szInput;
			CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE14, szInput);
		}
	else// 모델 편집 버튼 클릭 하지 않았다면
		return;


	}
	
}

void CFormRecipe::ClickGxbtnRecipeTrayThickness()
{
	CMainFrame *pMainFrm = (CMainFrame *)AfxGetMainWnd();
	if(pMainFrm->m_SetValue_Flag)// 모델 편집 버튼 클릭이 되었다면
	{
		if(m_iSetVelNumIndex==0)
			m_iSetVelNumIndex=1;
		CGxNumPadEx	dlgNumPad;
		double dOldValue = _tstof(m_vecSetValueTemp[(m_iSetVelNumIndex-1)*m_SetVelNum+13]);
		dlgNumPad.SetTitle(_T("Tray Setting TrayThickness Change"));
		dlgNumPad.SetValue(0.0, 50.0, dOldValue);
		if ( dlgNumPad.DoModal() == IDOK )
		{
			CString szInput = dlgNumPad.GetValue();
			//변경된 설정값을 임시 저장
			// 			if(m_iSetVelNumIndex==0)
			// 				m_vecSetValueTemp[(m_iSetVelNumIndex-1)*m_SetVelNum+14]=szInput;
			// 			else
			m_vecSetValueTemp[(m_iSetVelNumIndex-1)*m_SetVelNum+13]=szInput;
			CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE15, szInput);
		}
		else// 모델 편집 버튼 클릭 하지 않았다면
			return;


	}

}

void CFormRecipe::ClickGxbtnRecipeItemValue16()
{
	
}

void CFormRecipe::ClickGxbtnRecipeModifySave()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CMainFrame *pMainFrm = (CMainFrame *)AfxGetMainWnd();
	CString sname;
	// 변경된 임시 저장값들을 실제 저장값에 적용
	// 레시피 창에서 모델편집이 가능하도록 변경


	CGxMsgBox	dlgMsgBox;

	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
	dlgMsgBox.SetMessage(TRUE, 
		_T("수정하신 작업을 적용하겠습니까?"), 
		_T("Would you like to apply your modifications?"),		
		_T("Bạn có muốn áp dụng những thay đổi của bạn?") );

	int nRet = dlgMsgBox.DoModal();
	if(nRet != IDOK)
		return;
	pMainFrm->m_SetValue_Flag=FALSE;
	m_vecSetValue.clear();
	m_vecSetValue=m_vecSetValueTemp;
	if(m_iSelectedRow==0)
	{
		m_iSelectedRow=1;
	}
	sname.Format(_T("%s"),m_GridModelList.GetItemText(m_iSelectedRow,1));
	m_RecipeBank.m_strRecipeName=sname;
	int num=_tstol(m_GridModelList.GetItemText(m_iSelectedRow,0));
	m_RecipeBank.m_Cell.m_dWidth=_tstof(m_vecSetValue[(num-1)*m_SetVelNum+0]);
	m_RecipeBank.m_Cell.m_dHeight=_tstof(m_vecSetValue[(num-1)*m_SetVelNum+1]);

	if((m_vecSetValue[(num-1)*m_SetVelNum+2])==_T("Y"))
		m_RecipeBank.m_Cell.m_bFPCBuse=TRUE;
	else
		m_RecipeBank.m_Cell.m_bFPCBuse=FALSE;

	m_RecipeBank.m_Cell.m_dMarkDistance=_tstof(m_vecSetValue[(num-1)*m_SetVelNum+3]);

	m_RecipeBank.m_Tray.m_nCountX=_tstoi(m_vecSetValue[(num-1)*m_SetVelNum+7]);
	m_RecipeBank.m_Tray.m_nCountY=_tstoi(m_vecSetValue[(num-1)*m_SetVelNum+8]);
	m_RecipeBank.m_Tray.m_dPitchX=_tstof(m_vecSetValue[(num-1)*m_SetVelNum+9]);
	m_RecipeBank.m_Tray.m_dPitchY=_tstof(m_vecSetValue[(num-1)*m_SetVelNum+10]);
	m_RecipeBank.m_Tray.m_dPitchOffsetX=_tstof(m_vecSetValue[(num-1)*m_SetVelNum+11]);
	m_RecipeBank.m_Tray.m_dPitchOffsetY=_tstof(m_vecSetValue[(num-1)*m_SetVelNum+12]);
	m_RecipeBank.m_Tray.m_dTrayThickness=_tstof(m_vecSetValue[(num-1)*m_SetVelNum+13]);
	m_RecipeBank.SaveRecipeFile(sname);
	if(m_iSetVelNumIndex==0)
	{
		UpdateSetValue(m_iSetVelNumIndex);
	}
	else
	{
		UpdateSetValue(m_iSetVelNumIndex-1);
	}
	//LoadRecipe();
	SetValueUse(pMainFrm->m_SetValue_Flag);
	// 현재 Recipe를 수정한 거라면 새로 불러온다 [11/30/2016 OSC]
	if(m_RecipeBank.m_strRecipeName == theRecipeBank.m_strRecipeName)
		theRecipeBank.ReadRecipeFile(theRecipeBank.m_strRecipeName);


	// Recipe 수정 보고 [11/30/2016 OSC]

	//theSocketInterFace.SendCmdRecipeToDataPC(EQUIP_SET_PPID_MODIFY, m_RecipeBank.m_strRecipeName);
 	
}
void CFormRecipe::ClickGxbtnRecipeModifyCancle()
{
	CMainFrame *pMainFrm = (CMainFrame *)AfxGetMainWnd();
	//2017-05-16, JSJUNG. 그냥 눌럿을때 프로그램 죽음 
	if(pMainFrm->m_SetValue_Flag)// 모델 편집 버튼 클릭이 되었다면
	{
		pMainFrm->m_SetValue_Flag=FALSE;
		m_vecSetValueTemp.clear();
		m_vecSetValueTemp=m_vecSetValue;
		UpdateSetValue(m_iSetVelNumIndex-1);
		SetValueUse(pMainFrm->m_SetValue_Flag);
	}
}
void CFormRecipe::UpdateRuleInfo()
{
    CGxUICtrl::SetStaticString(this, IDC_GXBTN_CURRENT_PRODUCT, theConfigBank.m_UnloadRunInfo.m_sCurrentProduct);
	CGxUICtrl::SetStaticString(this, IDC_GXBTN_CURRENT_PPID, theConfigBank.m_UnloadRunInfo.m_sCurrentPPID);
	CGxUICtrl::SetStaticString(this, IDC_GXBTN_TRAY_QTY_IN_LOT, theConfigBank.m_UnloadRunInfo.m_sTrayQtyInLot);
	CGxUICtrl::SetStaticString(this, IDC_GXBTN_CELL_QTY_IN_LOT, theConfigBank.m_UnloadRunInfo.m_sCellQtyInTray);
	CGxUICtrl::SetStaticString(this, IDC_GXBTN_NEXT_STEPID,  theConfigBank.m_UnloadRunInfo.m_sNextStepID);

}

LRESULT CFormRecipe::OnRecipeUpdate(WPARAM wParam, LPARAM lParam)
{
	CString sztemp,strRecipeName;
	RecipeChangeConfirm();

	// 상위에서 내려 줄때도 중복 방지. 2017/06/15 HSK.
	if ( std::find(m_vecRecipe.begin(), m_vecRecipe.end(), theProcBank.m_strMESRecipeName) != m_vecRecipe.end() )
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("상위 생성 - 이미 존재하고 있는 레시피 입니다."), 
			_T("MES Create - Existing Recipe"),		
			_T("MES Create - Một item mà tồn tại.") );

		dlgMsgBox.DoModal();
		return 0;
	}
		
	//신규 레시피가 생성되면 임의의 초기값들을 입력해준다.
	m_vecRecipe.push_back(theProcBank.m_strMESRecipeName);
	//20170201 byskcho
	//m_RecipeBank.m_strRecipeName은 현재 선택된   모델명이므로 신규모델명을 저장하면 안된다.
	//m_RecipeBank.m_strRecipeName=szInput;
	strRecipeName = theProcBank.m_strMESRecipeName;

	//cell 설정 초기값 입력
	sztemp.Format(_T("%f"),theRecipeBank.m_Cell.m_dWidth);
	m_vecSetValue.push_back(sztemp);
	m_RecipeBank.m_Cell.m_dWidth=theRecipeBank.m_Cell.m_dWidth;

	sztemp.Format(_T("%f"),theRecipeBank.m_Cell.m_dHeight);
	m_vecSetValue.push_back(sztemp);
	m_RecipeBank.m_Cell.m_dHeight=theRecipeBank.m_Cell.m_dHeight;;

	if(theRecipeBank.m_Cell.m_bFPCBuse)
	{
		sztemp.Format(_T("Y"));
	}
	else
	{
		sztemp.Format(_T("N"));
	}
	m_vecSetValue.push_back(sztemp);
	m_RecipeBank.m_Cell.m_bFPCBuse=theRecipeBank.m_Cell.m_bFPCBuse;

	sztemp.Format(_T("%f"),theRecipeBank.m_Cell.m_dMarkDistance);
	m_vecSetValue.push_back(sztemp);
	m_RecipeBank.m_Cell.m_dMarkDistance=theRecipeBank.m_Cell.m_dMarkDistance;;

	sztemp.Format(_T(""));
	m_vecSetValue.push_back(sztemp);
	m_vecSetValue.push_back(sztemp);
	m_vecSetValue.push_back(sztemp);

	//tray 설정 초기값 입력
	sztemp.Format(_T("%d"),theRecipeBank.m_Tray.m_nCountX);
	m_vecSetValue.push_back(sztemp);
	m_RecipeBank.m_Tray.m_nCountX=theRecipeBank.m_Tray.m_nCountX;

	sztemp.Format(_T("%d"),theRecipeBank.m_Tray.m_nCountY);
	m_vecSetValue.push_back(sztemp);
	m_RecipeBank.m_Tray.m_nCountY=theRecipeBank.m_Tray.m_nCountY;

	sztemp.Format(_T("%f"),theRecipeBank.m_Tray.m_dPitchX);
	m_vecSetValue.push_back(sztemp);
	m_RecipeBank.m_Tray.m_dPitchX=theRecipeBank.m_Tray.m_dPitchX;

	sztemp.Format(_T("%f"),theRecipeBank.m_Tray.m_dPitchY);
	m_vecSetValue.push_back(sztemp);
	m_RecipeBank.m_Tray.m_dPitchY=theRecipeBank.m_Tray.m_dPitchY;

	sztemp.Format(_T("%f"),theRecipeBank.m_Tray.m_dPitchOffsetX);
	m_vecSetValue.push_back(sztemp);
	m_RecipeBank.m_Tray.m_dPitchOffsetX=theRecipeBank.m_Tray.m_dPitchOffsetX;

	sztemp.Format(_T("%f"),theRecipeBank.m_Tray.m_dPitchOffsetY);
	m_vecSetValue.push_back(sztemp);
	m_RecipeBank.m_Tray.m_dPitchOffsetY=theRecipeBank.m_Tray.m_dPitchOffsetY;

	sztemp.Format(_T("%f"),theRecipeBank.m_Tray.m_dTrayThickness);
	m_vecSetValue.push_back(sztemp);
	m_RecipeBank.m_Tray.m_dTrayThickness=theRecipeBank.m_Tray.m_dTrayThickness;

	sztemp.Format(_T("	"));
	m_vecSetValue.push_back(sztemp);

	m_RecipeBank.SaveRecipeFile(strRecipeName);

	//20170201 byskcho
	//레시피 생성시 티칭값 생성
	theTeachBank.SaveTeachFile(strRecipeName);

	if(m_iSetVelNumIndex==0)
	{
		m_iSetVelNumIndex=1;
	}
	UpdateSetValue(m_iSetVelNumIndex-1);
	LoadRecipe();
	UpdateGird();								// 그리드를 다시 그려준다.	

	// Recipe 생성 보고 [11/30/2016 OSC]
	theSocketInterFace.SendCmdRecipeToDataPC(EQUIP_SET_PPID_CREATE, theProcBank.m_strMESRecipeName);

	return 0;
}

void CFormRecipe::ClickGxbtnCurrentProduct()
{
	CDlgBoxKeyBoard dlgKey;
	if ( dlgKey.DoModal() == IDOK )
	{
		CString szInput = dlgKey.m_strReturnText;
		theConfigBank.m_UnloadRunInfo.m_sCurrentProduct = szInput;

		CGxUICtrl::SetStaticString(this, IDC_GXBTN_CURRENT_PRODUCT, theConfigBank.m_UnloadRunInfo.m_sCurrentProduct);
	}
}


void CFormRecipe::ClickGxbtnCurrentPpid()
{
	CDlgBoxKeyBoard dlgKey;
	if ( dlgKey.DoModal() == IDOK )
	{
		CString szInput = dlgKey.m_strReturnText;
		theConfigBank.m_UnloadRunInfo.m_sCurrentPPID = szInput;

		CGxUICtrl::SetStaticString(this, IDC_GXBTN_CURRENT_PPID, theConfigBank.m_UnloadRunInfo.m_sCurrentPPID);
	}
}


void CFormRecipe::ClickGxbtnTrayQtyInLot()
{
	CDlgBoxKeyBoard dlgKey;
	if ( dlgKey.DoModal() == IDOK )
	{
		CString szInput = dlgKey.m_strReturnText;
		theConfigBank.m_UnloadRunInfo.m_sTrayQtyInLot = szInput;

 		CGxUICtrl::SetStaticString(this, IDC_GXBTN_TRAY_QTY_IN_LOT, theConfigBank.m_UnloadRunInfo.m_sTrayQtyInLot);
	}
}


void CFormRecipe::ClickGxbtnCellQtyInLot()
{
	CDlgBoxKeyBoard dlgKey;
	if ( dlgKey.DoModal() == IDOK )
	{
		CString szInput = dlgKey.m_strReturnText;
		theConfigBank.m_UnloadRunInfo.m_sCellQtyInTray = szInput;

		CGxUICtrl::SetStaticString(this, IDC_GXBTN_CELL_QTY_IN_LOT, theConfigBank.m_UnloadRunInfo.m_sCellQtyInTray);
	}
}


void CFormRecipe::ClickGxbtnNextStepid()
{
	CDlgBoxKeyBoard dlgKey;
	if ( dlgKey.DoModal() == IDOK )
	{
		CString szInput = dlgKey.m_strReturnText;
		theConfigBank.m_UnloadRunInfo.m_sNextStepID = szInput;
 
		CGxUICtrl::SetStaticString(this, IDC_GXBTN_NEXT_STEPID,  theConfigBank.m_UnloadRunInfo.m_sNextStepID);
	}
}


void CFormRecipe::ClickGxbtnRuleInfoSave()
{
	// 레시피 창에서 변경 가능 하도록 수정. 2017/06/15 HSK.
	theConfigBank.SaveUnLoadRunInfo();
	theConfigBank.ReadUnLoadRunInfo();
    UpdateRuleInfo();
}

void CFormRecipe::ClickGxbtnRuleInfoRead()
{
	theConfigBank.ReadUnLoadRunInfo();//2017-03-06, skcho, jobchange를 위한 값을 읽어온다.(unload tray id와 관련 있음)
    UpdateRuleInfo();
}