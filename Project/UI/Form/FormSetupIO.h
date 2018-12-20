
#pragma once

#include "resource.h"
#include "UI\GridCtrl\GridCtrl.h"

// CFormSetup2 폼 뷰입니다.

class CFormSetupIO : public CFormView
{
	DECLARE_DYNCREATE(CFormSetupIO)

public:
	CFormSetupIO();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CFormSetupIO();

public:
	enum { IDD = IDD_FORM_SETUP_IO };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// 화면 처리에 대한 메세지 함수
	afx_msg LRESULT OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam);			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	afx_msg LRESULT OnChangeLanguage(WPARAM wParam, LPARAM lParam);			// 표시 언어가 변경되었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	DECLARE_MESSAGE_MAP()

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	CGridCtrl m_GridINput;
	CGridCtrl m_GridOutput;

	void InitGrid();
	void UpdateGrid();
	void IORefresh();

protected:
	int	m_iSelectedRowIn;
	int	m_iSelectedRowOut;
	int m_iMoveInput;
	int m_iMoveOutput;
	int m_iCurSelected;
	int m_iPageNumIn;
	int m_iPageNumOut;

public:
	DECLARE_EVENTSINK_MAP()
	virtual void OnInitialUpdate();

	afx_msg void onGridClickInputIO(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void onGridClickOutputIO(NMHDR *pNotifyStruct, LRESULT* pResult);

	void ClickBtnInChgPageUp();
	void ClickBtnInChgPageDown();
	void ClickBtnOutChgPageUp();
	void ClickBtnOutChgPageDown();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void ClickPageInput();
	void ClickPageOutput();
};


