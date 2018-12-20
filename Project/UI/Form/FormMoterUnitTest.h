#pragma once

#include "resource.h"
#include "UI\GridCtrl\GridCtrl.h"
#include "CheetahApp.h"


// CFormSetup4 폼 뷰입니다.

class CFormMoterUnitTest : public CFormView
{
	DECLARE_DYNCREATE(CFormMoterUnitTest)

public:
	CFormMoterUnitTest();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CFormMoterUnitTest();

public:
	enum { IDD = IDD_FORM_MOTERUNITTEST };
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


protected:					//	함수입니다.
	void InitializeGrid();
	void HighlightCurAxis(BOOL bRefrash);
	void UpdateMotorGrid();
	void HighlightCurTeach(BOOL bRefrash);
	void UpdateTeachGrid();

	// Timer
	void UpdateMotorState();
	void UpdateMotorParam();

protected:					//	변수입니다.
	CGridCtrl m_MotorGrid;
	CGridCtrl m_TeachGrid;

	int m_iUpDownParam;
	BOOL m_bFirst;

	int m_nLastMotorGridRow;
	AXIS_ID m_CurAxis;
	int m_nLastTeachGridRow;
// 	int m_nCurTeachRow;
// 	TEACH_PARAM::ID m_TeachList[10];

	BOOL m_bParamEditing;

public:
	void SelectedAxis(int iSell, int iSel2);

public:
	DECLARE_EVENTSINK_MAP()
	void ClickGxbtnSetup4Halfup();
	void ClickGxbtnSetup4Halfdown();
	void ClickGxbtnSetup4Up();
	void ClickGxbtnSetup4Down();
	

protected:
	afx_msg void OnGridMoterEndSelChange(NMHDR *pNotifyStruct, LRESULT *pResult);
	afx_msg void OnGridTeachEndSelChange(NMHDR *pNotifyStruct, LRESULT *pResult);
public:
	virtual void OnInitialUpdate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void ClickGxbtnSetup4Servoon();
	void ClickGxbtnSetup4Servooff();
	void ClickGxbtnSetup4Reset();
	void ClickGxbtnSetup4Origin();
	void ClickGxbtnSetup4MoveStop();
	void ClickGxbtnSetup4Save();
};


