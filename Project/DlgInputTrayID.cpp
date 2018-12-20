// DlgInputTrayID.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgInputTrayID.h"
#include "afxdialogex.h"


// CDlgInputTrayID 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgInputTrayID, CDialogEx)

CDlgInputTrayID::CDlgInputTrayID(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgInputTrayID::IDD, pParent)
{
	strTrayCode="";
}

CDlgInputTrayID::~CDlgInputTrayID()
{
}

void CDlgInputTrayID::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgInputTrayID, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgInputTrayID::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgInputTrayID::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgInputTrayID 메시지 처리기입니다.


void CDlgInputTrayID::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString str=_T("");
	GetDlgItemTextW(IDC_EDIT_TRAY_ID,str);
	strTrayCode=str;
	CDialogEx::OnOK();
}


void CDlgInputTrayID::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}
