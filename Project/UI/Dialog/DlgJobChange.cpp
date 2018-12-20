// DlgJobChange.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgJobChange.h"
#include "afxdialogex.h"

#include "UI\GausGUI\GxMsgBox.h"
// CDlgJobChange 대화 상자입니다.

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


// CDlgJobChange 메시지 처리기입니다.
BEGIN_EVENTSINK_MAP(CDlgJobChange, CDialogEx)
	ON_EVENT(CDlgJobChange, IDC_GXBTN_NG, DISPID_CLICK, CDlgJobChange::ClickGxbtnNg, VTS_NONE)
	ON_EVENT(CDlgJobChange, IDC_GXBTN_JOBCHANGE, DISPID_CLICK, CDlgJobChange::ClickGxbtnJobchange, VTS_NONE)
END_EVENTSINK_MAP()


void CDlgJobChange::ClickGxbtnNg()
{	
	CGxMsgBox	dlgMsgBox;

	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
	dlgMsgBox.SetMessage(TRUE, 
			_T("현재 CELL을 NG TRAY로 보내시겠습니까?"),	//한
			_T("현재 CELL을 NG TRAY로 보내시겠습니까?"),		//영
			_T("현재 CELL을 NG TRAY로 보내시겠습니까?") );		//베

	if(dlgMsgBox.DoModal() == IDOK)
	{
		//NG 일 경우 강제로 NG셀로 만들어 버림
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

	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
	dlgMsgBox.SetMessage(TRUE, 
			_T("정말로 JOB CHANGE을 하시겠습니까?"),	//한
			_T("정말로 JOB CHANGE을 하시겠습니까?"),		//영
			_T("정말로 JOB CHANGE을 하시겠습니까?") );		//베

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
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
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