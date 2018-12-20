// DlgTrayScanError.cpp : implementation file
//

#include "stdafx.h"
#include "DlgTrayScanError.h"
#include "afxdialogex.h"


// CDlgTrayScanError dialog

IMPLEMENT_DYNAMIC(CDlgTrayScanError, CDialogEx)

CDlgTrayScanError::CDlgTrayScanError(CWnd* pParent,int model)
	: CDialogEx(CDlgTrayScanError::IDD, pParent)
{
	iModel=model;
}

CDlgTrayScanError::~CDlgTrayScanError()
{
}

void CDlgTrayScanError::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Title);
}


BEGIN_MESSAGE_MAP(CDlgTrayScanError, CDialogEx)
	ON_BN_CLICKED(ID_RETRAY, &CDlgTrayScanError::OnBnClickedRetray)
	ON_BN_CLICKED(ID_REMOVE, &CDlgTrayScanError::OnBnClickedRemove)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgTrayScanError message handlers

void CDlgTrayScanError::SetModel(int model)
{
	CButton* pBtn1=(CButton*)GetDlgItem(ID_RETRAY);
	CButton* pBtn2=(CButton*)GetDlgItem(ID_REMOVE);
	if(1==model)
	{
		SetDlgItemTextW(IDC_EDIT1,_T("Scan tray code error. you can retray or remove"));
		pBtn1->ShowWindow(TRUE);
		pBtn2->ShowWindow(TRUE);
	}
	else if(2==model)
	{
		SetDlgItemTextW(IDC_EDIT1,_T("Scan tray code ok,but read cim data fail. you can remove only"));
		pBtn1->ShowWindow(FALSE);
		pBtn2->ShowWindow(TRUE);
	}
}

void CDlgTrayScanError::OnBnClickedRetray()
{
	// TODO: Add your control notification handler code here
	theUnitFunc.DCRTrigger(ON);
	SetTimer(1,1000, NULL );
}

void CDlgTrayScanError::OnBnClickedRemove()
{
	// TODO: Add your control notification handler code here
	if(theProcBank.m_bModel2FullTray)
		theProcBank.m_bModel2FullTray=FALSE;
	theProcBank.MinusSubGoodAlignCount();
	theProcBank.MinusGoodAlignCount();
	theProcBank.GOODTrayBitX=0;
	theProcBank.GOODTrayBitY=0;
	CDlgTrayScanError::OnOK();
}


BOOL CDlgTrayScanError::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CButton* pBtn1=(CButton*)GetDlgItem(ID_RETRAY);
	CButton* pBtn2=(CButton*)GetDlgItem(ID_REMOVE);
	if(1==iModel)
	{
		SetDlgItemTextW(IDC_EDIT1,_T("Scan tray code error. you can retray or remove"));
		pBtn1->ShowWindow(TRUE);
		pBtn2->ShowWindow(TRUE);
	}
	else if(2==iModel)
	{
		SetDlgItemTextW(IDC_EDIT1,_T("Scan tray code ok,but read cim data fail. you can remove only"));
		pBtn1->ShowWindow(FALSE);
		pBtn2->ShowWindow(TRUE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgTrayScanError::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(1==nIDEvent)
	{
		KillTimer(1);
		theUnitFunc.DCRTrigger(OFF);
		if(!theUnitFunc.DCRChk())
		{
			AfxMessageBox(_T("Read DCR Error!"));
		}
		else
		{
			theProcBank.m_strTrayID.MakeUpper();
			::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_REMOVE_ALL_CELL, 0,0);
			theUnitFunc.CIM_CELLID_Info_Request();
			SetTimer(2, 1000, NULL );
		}
	}
	if(2==nIDEvent)
	{
		KillTimer(2);
		if(theProcBank.m_bGetCellIDComplate)
		{
			theProcBank.m_bGetCellIDComplate=FALSE;
			theUnitFunc.CellData_Tray_ClearAll();
			theUnitFunc.CellData_Tray_CreateAll();
			theUnitFunc.TrayData_Create(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_LIFT);
			theUnitFunc.TrayData_from_toAlign(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_LIFT,TRAY_INFO_POS_GOOD_IN_ALIGN);
			for(int j=0;j<theRecipeBank.m_Tray.m_nCountY;j++)
			{
				for(int i=0;i<theRecipeBank.m_Tray.m_nCountX;i++)
				{
					theProcBank.m_TrayBank.GetTrayInfo(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_ALIGN)->AddCell(CELL_POS_GOOD_TRAY,theProcBank.strCIMLoadCellID[i][j],i+1,j+1);
				}
			}
			theProcBank.MinusSubGoodAlignCount();
			theProcBank.MinusGoodAlignCount();
			if(theProcBank.m_bModel2FullTray)
				theProcBank.m_bModel2FullTray=FALSE;
			theProcBank.GOODTrayBitX=theRecipeBank.m_Tray.m_nCountX+1;
			theProcBank.GOODTrayBitY=theRecipeBank.m_Tray.m_nCountY+1;
			::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_DCR_UPDATE, 0,0);  
			CDlgTrayScanError::OnOK();
		}
		else
		{
			CButton* pBtn1=(CButton*)GetDlgItem(ID_RETRAY);
			pBtn1->ShowWindow(FALSE);
			AfxMessageBox(_T("CIM Back Fail,you can remove!"));
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}
