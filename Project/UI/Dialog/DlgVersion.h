#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDlgVersion ��ȭ �����Դϴ�.

class CDlgVersion : public CDialog
{
	DECLARE_DYNAMIC(CDlgVersion)

public:
	CDlgVersion(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgVersion();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_VERSION_HISTORY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	void ReadVersionHistory(void);
	
	
	virtual BOOL OnInitDialog();
	CEdit m_ctrlEditVersion;
};
