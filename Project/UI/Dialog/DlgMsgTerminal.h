#pragma once

#include "resource.h"

// CDlgMsgTerminal 대화 상자입니다.

class CDlgMsgTerminal : public CDialog
{
	DECLARE_DYNAMIC(CDlgMsgTerminal)

public:
	CDlgMsgTerminal(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgMsgTerminal();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_MSG_TERMINAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.


	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// 화면 처리에 대한 메세지 함수
	afx_msg LRESULT OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam);			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	afx_msg LRESULT OnChangeLanguage(WPARAM wParam, LPARAM lParam);			// 표시 언어가 변경되었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_EVENTSINK_MAP()
	void ClickGxbtnOk();
};
