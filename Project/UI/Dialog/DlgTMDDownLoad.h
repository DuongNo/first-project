#pragma once


// CDlgTMDDownLoad 대화 상자입니다.

class CDlgTMDDownLoad : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTMDDownLoad)

public:
	CDlgTMDDownLoad(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTMDDownLoad();

	void LibShowOut(CString str);
	CEdit m_TmdListLog;
	CString strMsgIn;
	CString strMsg;
	BOOL	b_chk;
// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_MSG_TMDDOWNLOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	DECLARE_EVENTSINK_MAP()
	void ClickGxbtnLosstimeConfirm();
	void ClickGxbtnLosstimeCancel();
	void LibShowOut2(CString str);	

	afx_msg LRESULT OnUpdateLog(WPARAM wParam, LPARAM lParam);
};
