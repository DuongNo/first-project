#pragma once


// CDlgTMDDownLoad ��ȭ �����Դϴ�.

class CDlgTMDDownLoad : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTMDDownLoad)

public:
	CDlgTMDDownLoad(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgTMDDownLoad();

	void LibShowOut(CString str);
	CEdit m_TmdListLog;
	CString strMsgIn;
	CString strMsg;
	BOOL	b_chk;
// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_MSG_TMDDOWNLOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	DECLARE_EVENTSINK_MAP()
	void ClickGxbtnLosstimeConfirm();
	void ClickGxbtnLosstimeCancel();
	void LibShowOut2(CString str);	

	afx_msg LRESULT OnUpdateLog(WPARAM wParam, LPARAM lParam);
};
