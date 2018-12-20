// DlgLogo.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgLogo.h"
#include "afxdialogex.h"


// CDlgLogo 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgLogo, CDialog)

CDlgLogo::CDlgLogo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLogo::IDD, pParent)
{

}

CDlgLogo::~CDlgLogo()
{
}

void CDlgLogo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GXSTC_LOGO_IMAGE, m_imgLogo);
}


BEGIN_MESSAGE_MAP(CDlgLogo, CDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()


BOOL CDlgLogo::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString szPath;
	szPath = _T("D:\\Cheetah\\Image\\LOGO.bmp");

	if(CFileSupport::FileCheck(szPath)  == TRUE )
	{
		m_imgLogo.SetStretchMode(TRUE);
		m_imgLogo.LoadImage(szPath.GetBuffer());
		m_imgLogo.Invalidate();	
	}

	// 1분후에 자동 닫기
	SetTimer(1,60000,NULL);
	return TRUE; 
}


void CDlgLogo::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(nIDEvent);

	if(nIDEvent == 1)
	{
		OnOK();
	}

	CDialog::OnTimer(nIDEvent);
}
