#pragma once


// CDlgAGVMsg ��ȭ �����Դϴ�.

class CDlgAGVMsg : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAGVMsg)

public:
	CDlgAGVMsg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgAGVMsg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_MSG_AGV };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
		afx_msg LRESULT OnShowMsg( WPARAM wParam, LPARAM lParam );

	DECLARE_MESSAGE_MAP()

public:
	DECLARE_EVENTSINK_MAP()
	void ClickGxbtnMsgAgvOk();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	void ClickGxbtnAgvMsgCancel();
};
