#pragma once


// CDlgMsgPortEmpty ��ȭ �����Դϴ�.

class CDlgMsgPortEmpty : public CDialog
{
	DECLARE_DYNAMIC(CDlgMsgPortEmpty)

public:
	CDlgMsgPortEmpty(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgMsgPortEmpty();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_MSG_PORTEMPTY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void ClickGxbtnMsgAgvOk();
};
