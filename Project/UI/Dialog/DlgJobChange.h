#pragma once


// CDlgJobChange ��ȭ �����Դϴ�.

class CDlgJobChange : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgJobChange)

public:
	CDlgJobChange(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgJobChange();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_JOBCHANGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void ClickGxbtnNg();
	void ClickGxbtnJobchange();
	virtual BOOL OnInitDialog();

	LRESULT OnPopUpDialog(WPARAM wParam, LPARAM lParam);
};
