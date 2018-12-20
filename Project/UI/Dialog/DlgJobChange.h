#pragma once


// CDlgJobChange 대화 상자입니다.

class CDlgJobChange : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgJobChange)

public:
	CDlgJobChange(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgJobChange();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_JOBCHANGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void ClickGxbtnNg();
	void ClickGxbtnJobchange();
	virtual BOOL OnInitDialog();

	LRESULT OnPopUpDialog(WPARAM wParam, LPARAM lParam);
};
