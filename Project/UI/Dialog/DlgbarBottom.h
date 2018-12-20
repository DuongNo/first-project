//==================================================================================================
// 
// CDlgbarBottom : 화면의 하단에 위치하며 주로 메인 메뉴 선택으로 화면 전환을 수행한다.
//
//==================================================================================================
#pragma once

#include "resource.h"

#include "MainFrm.h"

// CDlgbarBottom 대화 상자입니다.

class CDlgbarBottom : public CDialogBar
{
	DECLARE_DYNAMIC(CDlgbarBottom)

public:
	CDlgbarBottom();	// CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgbarBottom();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_BAR_BOTTOM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.


	// 화면 변환 명령
	void ChangeForm(int nIdx, BOOL bForce = FALSE);		// 지정 화면으로 폼을 변경한다.

	// 변경된 내용을 화면에 표시한다.
	void DispCurrentSelect(UINT nID);						// 현재 선택된 버튼의 색을 달리해준다.


	// afx_msg void OnBtnClickMenu1();
	afx_msg void OnUpdateCmdUI(CCmdUI* pCmdUI);

	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// 화면 처리에 대한 메세지 함수
	afx_msg LRESULT OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam);			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	afx_msg LRESULT OnChangeViewEnd(WPARAM wParam, LPARAM lParam);				// 메인 View 변환 완료 수신
	afx_msg LRESULT OnChangeLanguage(WPARAM wParam, LPARAM lParam);			// 표시 언어가 변경되었다.

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	DECLARE_MESSAGE_MAP()

public:

	// GxButtonEx Event
	DECLARE_EVENTSINK_MAP()
	void OnClickBtnxBottomMenu1();
	void OnClickBtnxBottomMenu2();
	void OnClickBtnxBottomMenu3();
	void OnClickBtnxBottomMenu4();
	void OnClickBtnxBottomMenu5();
	void OnClickBtnxBottomMenu6();
	void OnClickBtnxBottomMenu7();
	void OnClickBtnxBottomQuit();
	void ClickBtnxBottomLang();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	void SetEnable(BOOL val) { 		m_bEnable = val; 	}
	void SetModePM();

protected:
	CMainFrame* m_pMain;
	BOOL		m_bEnable;
};
