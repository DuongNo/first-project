#pragma once


// CDlgRetryInfo 대화 상자입니다.

class CDlgRetryInfo : public CDialog
{
	DECLARE_DYNAMIC(CDlgRetryInfo)

public:
	CDlgRetryInfo(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgRetryInfo();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_RETRYINFO };

	void DisplayRetryInfo();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// 화면 처리에 대한 메세지 함수
	afx_msg LRESULT OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam);			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정


	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void ClickGxlblClear();
	void ClickGxlblOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
