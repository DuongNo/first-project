// DlgMsgPortEmpty.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "DlgMsgPortEmpty.h"
#include "afxdialogex.h"

#include "..\..\..\ModuleInternal\UI\GausGUI\GxStaticEx.h"
// CDlgMsgPortEmpty ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgMsgPortEmpty, CDialog)

CDlgMsgPortEmpty::CDlgMsgPortEmpty(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMsgPortEmpty::IDD, pParent)
{

}

CDlgMsgPortEmpty::~CDlgMsgPortEmpty()
{
}

void CDlgMsgPortEmpty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMsgPortEmpty, CDialog)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CDlgMsgPortEmpty, CDialog)
	
	ON_EVENT(CDlgMsgPortEmpty, IDC_GXBTN_MSG_AGV_OK, DISPID_CLICK, CDlgMsgPortEmpty::ClickGxbtnMsgAgvOk, VTS_NONE)
END_EVENTSINK_MAP()

// CDlgMsgPortEmpty �޽��� ó�����Դϴ�.


void CDlgMsgPortEmpty::ClickGxbtnMsgAgvOk()
{
	ShowWindow(SW_HIDE);
}
