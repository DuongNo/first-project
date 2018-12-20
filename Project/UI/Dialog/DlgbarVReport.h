//
// Report 그룹의 하위 메뉴 선택용 왼쪽/오른쪽 수직 메뉴 Dialogbar
//
//

#pragma once

#include "resource.h"

// CDlgbarVReport 대화 상자입니다.

class CDlgbarVReport : public CDialogBar
{
	DECLARE_DYNAMIC(CDlgbarVReport)

public:
	CDlgbarVReport();   // 표준 생성자입니다.
	virtual ~CDlgbarVReport();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_BAR_VREPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	// 지정한 화면으로 변환한다.
	void ChangeForm(int nIdx, BOOL bForce = FALSE);

	// 변경된 내용을 화면에 표시한다.
	void DispCurrentSelect(UINT nID);						// 현재 선택된 버튼의 색을 달리해준다.


	DECLARE_MESSAGE_MAP()
	afx_msg void OnUpdateCmdUI(CCmdUI* pCmdUI);

	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// 화면 처리에 대한 메세지 함수
	afx_msg LRESULT OnChangeViewEnd(WPARAM wParam, LPARAM lParam);				// 메인 View 변환 완료 수신
	afx_msg LRESULT OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam);			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	afx_msg LRESULT OnChangeLanguage(WPARAM wParam, LPARAM lParam);			// 표시 언어가 변경되었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

public:

	// ActiveX 처리
	DECLARE_EVENTSINK_MAP()
	void OnClickBtnxSubMenu1();
	void OnClickBtnxSubMenu2();
	void OnClickBtnxSubMenu3();
	void OnClickBtnxSubMenu4();
	void OnClickBtnxSubMenu5();
};
