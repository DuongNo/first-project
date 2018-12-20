#pragma once


// CDlgStatement 대화 상자입니다.

class CDlgStatement : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgStatement)

public:
	CDlgStatement(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgStatement();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_STATEMENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	afx_msg LRESULT OnJigCountUpdate(WPARAM wParam, LPARAM lParam);	

	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// 화면 처리에 대한 메세지 함수
	afx_msg LRESULT OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam);			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	

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
