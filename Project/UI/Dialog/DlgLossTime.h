#pragma once


// DlgLossTime ��ȭ �����Դϴ�.

class DlgLossTime : public CDialog
{
	DECLARE_DYNAMIC(DlgLossTime)

public:
	DlgLossTime(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~DlgLossTime();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_LOSSTIME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void ClickGxbtnLosstimeFirst();
	void ClickGxbtnLosstimeSecond();
	void ClickGxbtnLosstimeThird();
	void ClickGxbtnLosstimeSelect();
};
