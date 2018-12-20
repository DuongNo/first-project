#pragma once


// CDlgAGVMsg 대화 상자입니다.

class CDlgAGVMsg : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAGVMsg)

public:
	CDlgAGVMsg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgAGVMsg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_MSG_AGV };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
		afx_msg LRESULT OnShowMsg( WPARAM wParam, LPARAM lParam );

	DECLARE_MESSAGE_MAP()

public:
	DECLARE_EVENTSINK_MAP()
	void ClickGxbtnMsgAgvOk();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	void ClickGxbtnAgvMsgCancel();
};
