#pragma once


// CDlgInputTrayID ��ȭ �����Դϴ�.

class CDlgInputTrayID : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgInputTrayID)

public:
	CDlgInputTrayID(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgInputTrayID();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_INPUT_TRAYID };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
private:
	CString strTrayCode;
public:
	CString GetTrayCode(){return strTrayCode;}
};
