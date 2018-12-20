#pragma once


// CDlgMsgPortEmpty 대화 상자입니다.

class CDlgMsgPortEmpty : public CDialog
{
	DECLARE_DYNAMIC(CDlgMsgPortEmpty)

public:
	CDlgMsgPortEmpty(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgMsgPortEmpty();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_MSG_PORTEMPTY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void ClickGxbtnMsgAgvOk();
};
