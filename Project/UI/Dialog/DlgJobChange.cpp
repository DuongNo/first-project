// DlgJobChange.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "DlgJobChange.h"
#include "afxdialogex.h"

#include "UI\GausGUI\GxMsgBox.h"
// CDlgJobChange ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgJobChange, CDialogEx)

CDlgJobChange::CDlgJobChange(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgJobChange::IDD, pParent)
{

}

CDlgJobChange::~CDlgJobChange()
{
}

void CDlgJobChange::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgJobChange, CDialogEx)
	ON_MESSAGE(UDMSG_JOBCHANGE_POPUP,			OnPopUpDialog)	
END_MESSAGE_MAP()


// CDlgJobChange �޽��� ó�����Դϴ�.
BEGIN_EVENTSINK_MAP(CDlgJobChange, CDialogEx)
	ON_EVENT(CDlgJobChange, IDC_GXBTN_NG, DISPID_CLICK, CDlgJobChange::ClickGxbtnNg, VTS_NONE)
	ON_EVENT(CDlgJobChange, IDC_GXBTN_JOBCHANGE, DISPID_CLICK, CDlgJobChange::ClickGxbtnJobchange, VTS_NONE)
END_EVENTSINK_MAP()


void CDlgJobChange::ClickGxbtnNg()
{	
	CGxMsgBox	dlgMsgBox;

	dlgMsgBox.SetLangName(2, _T("VTM"));		// ����� �̸��� ����
	dlgMsgBox.SetTitle(_T("�˸�"), _T("Confirm"), _T("Confirm"));
	dlgMsgBox.SetMessage(TRUE, 
			_T("���� CELL�� NG TRAY�� �����ðڽ��ϱ�?"),	//��
			_T("���� CELL�� NG TRAY�� �����ðڽ��ϱ�?"),		//��
			_T("���� CELL�� NG TRAY�� �����ðڽ��ϱ�?") );		//��

	if(dlgMsgBox.DoModal() == IDOK)
	{
		//NG �� ��� ������ NG���� ����� ����
		CCellInfo *pCellInfo = NULL;
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_UD_ROBOT);
		if(pCellInfo)
		{
			pCellInfo->m_LastClass = NONE_CELL;
		}

		theProcBank.ChangeMachineState(CONST_MACHINE_STATE::MACHINE_RUN);  //MOVESTATE = 2 
		theConfigBank.m_UnloadRunInfo.SetJobChangeType(JOBCHANGE_NONE);
		ShowWindow(SW_HIDE);
	}
	else
	{
		return;
	}
	
}


void CDlgJobChange::ClickGxbtnJobchange()
{
	CGxMsgBox	dlgMsgBox;

	dlgMsgBox.SetLangName(2, _T("VTM"));		// ����� �̸��� ����
	dlgMsgBox.SetTitle(_T("�˸�"), _T("Confirm"), _T("Confirm"));
	dlgMsgBox.SetMessage(TRUE, 
			_T("������ JOB CHANGE�� �Ͻðڽ��ϱ�?"),	//��
			_T("������ JOB CHANGE�� �Ͻðڽ��ϱ�?"),		//��
			_T("������ JOB CHANGE�� �Ͻðڽ��ϱ�?") );		//��

	if(dlgMsgBox.DoModal() == IDOK)
	{
		theProcBank.ChangeMachineState(CONST_MACHINE_STATE::MACHINE_RUN);  //MOVESTATE = 2 
		ShowWindow(SW_HIDE);
	}
	else
	{
		return;
	}
}


BOOL CDlgJobChange::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	::SetWindowPos(this->m_hWnd,NULL, 300,200,600,600,TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
LRESULT CDlgJobChange::OnPopUpDialog(WPARAM wParam, LPARAM lParam)
{
	int nType = (LPARAM)lParam;

	if(nType == 1)
	{
		ShowWindow(SW_SHOW);
	}
	else
	{
		ShowWindow(SW_HIDE);	
	}

	return 0;
}