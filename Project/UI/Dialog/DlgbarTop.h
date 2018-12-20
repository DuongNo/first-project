//==================================================================================================
// 
// CDlgbarTop : 화면의 상부에 위치하며 주로 설비의 동작 상태 정보를 표시한다.
//
//==================================================================================================

#pragma once

#include "resource.h"
#include "UI\GausGUI\GxImage.h"

// CDlgbarTop 대화 상자입니다.
#define TMDDOWNLOAD 101

class CDlgbarTop : public CDialogBar
{
	DECLARE_DYNAMIC(CDlgbarTop)

public:
	CDlgbarTop();													// 표준 생성자입니다.
	virtual ~CDlgbarTop();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_BAR_TOP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	// afx_msg void OnBtnClickMenu1();
	afx_msg void OnUpdateCmdUI(CCmdUI* pCmdUI);

	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// 화면 처리에 대한 메세지 함수
	afx_msg LRESULT OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam);			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	afx_msg LRESULT OnChangeViewEnd(WPARAM wParam, LPARAM lParam);				// 메인 View 변환 완료 수신
	afx_msg LRESULT OnChangeLanguage(WPARAM wParam, LPARAM lParam);			// 표시 언어가 변경되었다.
	//kjpark 20160919 Topdlg에 connect status 
	afx_msg LRESULT OnChangeCommState(WPARAM wParam, LPARAM lParam);		// 통신 상태 변경
	afx_msg LRESULT OnChangeMachineState(WPARAM wParam, LPARAM lParam);		// 설비 상태 변경
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

public:

	DECLARE_EVENTSINK_MAP()
	void ClickLblxTopbarClock();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	BOOL m_bOldData_PC;

protected:
	void InitializeVer();
	void ShowEQID();
	void ClickProductLogin();
public:
	void ClickGxstcTopbarModelText();
	void ClickGxstcTopbarDcr();
	void ClickGxstcTopbarPg();
	void ClickGxstcTopbarLogo();
	void ClickGxstcTopbarVerText();
	void ClickGxstcTopbarPrealign();
};
