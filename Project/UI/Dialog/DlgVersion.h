#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDlgVersion 대화 상자입니다.

class CDlgVersion : public CDialog
{
	DECLARE_DYNAMIC(CDlgVersion)

public:
	CDlgVersion(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgVersion();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_VERSION_HISTORY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	void ReadVersionHistory(void);
	
	
	virtual BOOL OnInitDialog();
	CEdit m_ctrlEditVersion;
};
