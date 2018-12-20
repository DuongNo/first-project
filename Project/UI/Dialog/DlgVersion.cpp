// DlgVersion.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "DlgVersion.h"
#include "afxdialogex.h"
#include "locale.h"

// CDlgVersion ��ȭ �����Դϴ�.

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


// CDlgVersion �޽��� ó�����Դϴ�.


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
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
