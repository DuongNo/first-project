#pragma once


// CDlgStatement ��ȭ �����Դϴ�.

class CDlgStatement : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgStatement)

public:
	CDlgStatement(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgStatement();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_STATEMENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	afx_msg LRESULT OnJigCountUpdate(WPARAM wParam, LPARAM lParam);	

	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// ȭ�� ó���� ���� �޼��� �Լ�
	afx_msg LRESULT OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam);			// View/Dialog�� ȭ�� ���ſ� Ÿ�̸��� ���� �� ���� ����
	

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void ClickGxlblOk();
	void ClickGxlblClear();
	void ClickGxlblA1Clear();
	void ClickGxlblA2Clear();
	void ClickGxlblB1Clear();
	void ClickGxlblB2Clear();
	void ClickGxlblC1Clear();
	void ClickGxlblC2Clear();
	void ClickGxlblD1Clear();
	void ClickGxlblD2Clear();
};
