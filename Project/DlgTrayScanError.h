#pragma once
#include "afxwin.h"


// CDlgTrayScanError dialog

class CDlgTrayScanError : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTrayScanError)

public:
	CDlgTrayScanError(CWnd* pParent = NULL,int model=1);   // standard constructor
	virtual ~CDlgTrayScanError();

// Dialog Data
	enum { IDD = IDD_DLG_TRAY_CODE_ERROR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	int iModel;
public:
	CEdit m_Title;
	void SetModel(int model);

	afx_msg void OnBnClickedRetray();
	afx_msg void OnBnClickedRemove();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
