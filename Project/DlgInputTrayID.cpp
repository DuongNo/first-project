// DlgInputTrayID.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "DlgInputTrayID.h"
#include "afxdialogex.h"


// CDlgInputTrayID ��ȭ �����Դϴ�.

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


// CDlgInputTrayID �޽��� ó�����Դϴ�.


void CDlgInputTrayID::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString str=_T("");
	GetDlgItemTextW(IDC_EDIT_TRAY_ID,str);
	strTrayCode=str;
	CDialogEx::OnOK();
}


void CDlgInputTrayID::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CDialogEx::OnCancel();
}
