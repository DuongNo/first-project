#pragma once


// CDlgRetryInfo ��ȭ �����Դϴ�.

class CDlgRetryInfo : public CDialog
{
	DECLARE_DYNAMIC(CDlgRetryInfo)

public:
	CDlgRetryInfo(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgRetryInfo();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_RETRYINFO };

	void DisplayRetryInfo();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// ȭ�� ó���� ���� �޼��� �Լ�
	afx_msg LRESULT OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam);			// View/Dialog�� ȭ�� ���ſ� Ÿ�̸��� ���� �� ���� ����


	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void ClickGxlblClear();
	void ClickGxlblOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
