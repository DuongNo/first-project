#pragma once


// CDlgInputTrayID 대화 상자입니다.

class CDlgInputTrayID : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgInputTrayID)

public:
	CDlgInputTrayID(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgInputTrayID();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_INPUT_TRAYID };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
private:
	CString strTrayCode;
public:
	CString GetTrayCode(){return strTrayCode;}
};
