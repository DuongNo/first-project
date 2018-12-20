#pragma once

#include "resource.h"
#include "UI\GausGUI\GxButtonEx.h"
// CFormSetup1 폼 뷰입니다.

class CFormSignal : public CFormView
{
	DECLARE_DYNCREATE(CFormSignal)

public:
	CFormSignal();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CFormSignal();

public:
	enum { IDD = IDD_FORM_SETUP_SIGNAL };
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
	//kjpark 20161021 Setup 신호기 UI 구현
	BOOL m_bButtonCheck[4][5];
	UINT m_nCellButton[4][5];
	BOOL GetSelectButton(JIG_ID zone, JIG_CH nCh);
	BOOL m_bIsMutli;
	BOOL m_bIsShwoWindow;
	void SetLogListUpdate(CString strMessage);	

	void ClickBtnSelect_A1();
	void ClickBtnSelect_A2();
	void ClickBtnSelect_B1();
	void ClickBtnSelect_B2();
	void ClickBtnSelect_C1();
	void ClickBtnSelect_C2();
	void ClickBtnSelect_D1();
	void ClickBtnSelect_D2();
	void ClickBtnSelect_All();
	void ClickBtnSelect_Clear();
	void ClickBtnSelect_Single();
	void ClickBtnSelect_Multi();
	void ClickBtnZoneSelect_A();
	void ClickBtnZoneSelect_B();
	void ClickBtnZoneSelect_C();
	void ClickBtnZoneSelect_D();

	void ClickBtnPGSelect_ON();
	void ClickBtnPGSelect_OFF();
	void ClickBtnPGSelect_PREV();
	void ClickBtnPGSelect_NEXT();

	void ClickBtnPGSelect_Reset();
	void ClickBtnPGSelect_CellLoading();
	void ClickBtnPGSelect_TMDInfo();
	
	DECLARE_EVENTSINK_MAP()

	

	CListCtrl m_PGLog;
	
	void ClickGxbtnSetzoneA();
	void ClickGxbtnSetzoneB();
	void ClickGxbtnSetzoneC();
	void ClickGxbtnSetzoneD();
	void ClickGxbtnTeach1AjigSw();
	void ClickGxbtnTeach1AjigSw2();
	void ClickGxbtnTeach1AjigSw3();
	void ClickGxbtnTeach1AjigSw4();
	void ClickGxbtnSignalTmd1();
	void ClickGxbtnSignalTmd7();
	void ClickGxbtnSignalTmd2();
	void ClickGxbtnSignalTmd3();
	void ClickGxbtnSignalTmd4();
	void ClickGxbtnSignalTmd5();
	void ClickGxbtnSignalTmd8();
	void ClickGxbtnSignalTmd6();
};


