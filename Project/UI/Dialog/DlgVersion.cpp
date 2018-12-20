// DlgVersion.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgVersion.h"
#include "afxdialogex.h"
#include "locale.h"

// CDlgVersion 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgVersion, CDialog)

CDlgVersion::CDlgVersion(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgVersion::IDD, pParent)
{
	
}

CDlgVersion::~CDlgVersion()
{
}

void CDlgVersion::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);		
	DDX_Control(pDX, IDC_EDIT1, m_ctrlEditVersion);
}


BEGIN_MESSAGE_MAP(CDlgVersion, CDialog)
END_MESSAGE_MAP()


// CDlgVersion 메시지 처리기입니다.


void CDlgVersion::ReadVersionHistory(void)
{
	CString filename;
	filename.Format(_T("VersionHistory.txt"));	
	char *pLocale = setlocale(LC_ALL,"Korean");
	
	CStdioFile		file;
	CString strBuf;
	if(file.Open(filename,CFile::modeRead | CFile::typeText))
	{
		while(file.ReadString(strBuf))
		{	
			m_ctrlEditVersion.ReplaceSel(strBuf);
			m_ctrlEditVersion.ReplaceSel(L"\r\n");
		}

		file.Close();
	}	
	
}


BOOL CDlgVersion::OnInitDialog()
{
	CDialog::OnInitDialog();
    m_ctrlEditVersion.SetSel(0,-1);
	m_ctrlEditVersion.Clear();

	ReadVersionHistory();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
