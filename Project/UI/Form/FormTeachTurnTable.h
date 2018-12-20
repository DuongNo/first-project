#pragma once

#include "UI\GausGUI\GxButtonEx.h"


// CFormTeachTurnTable 폼 뷰입니다.

class CFormTeachTurnTable : public CFormView
{
	DECLARE_DYNCREATE(CFormTeachTurnTable)

protected:
public:
	CFormTeachTurnTable();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CFormTeachTurnTable();

	enum { IDD = IDD_FORM_TEACH_TURNTABLE };
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
	DECLARE_EVENTSINK_MAP()

	BOOL m_bShow;
	BOOL m_bInit;
	AXIS_ID			m_CurAxis;
	TEACH_PARAM::ID	m_CurTeach;
	UINT			m_CurID;

	virtual void OnInitialUpdate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void UpdateTurnTableState();
	void ClickGxbtnWorktACh1VacOn();
	void ClickGxbtnWorktACh1VacOff();
	void ClickGxbtnWorktACh2VacOn();
	void ClickGxbtnWorktACh2VacOff();
	void ClickGxbtnWorktBCh1VacOn();
	void ClickGxbtnWorktBCh1VacOff();
	void ClickGxbtnWorktBCh2VacOn();
	void ClickGxbtnWorktBCh2VacOff();
	void ClickGxbtnWorktCCh1VacOn();
	void ClickGxbtnWorktCCh1VacOff();
	void ClickGxbtnWorktCCh2VacOn();
	void ClickGxbtnWorktCCh2VacOff();
	void ClickGxbtnWorktDCh1VacOn();
	void ClickGxbtnWorktDCh1VacOff();
	void ClickGxbtnWorktDCh2VacOn();
	void ClickGxbtnWorktDCh2VacOff();

	void SetCurPageAxis(AXIS_ID axis, TEACH_PARAM::ID param, UINT nID);

	void ClickGxbtnWorktStart();
	void ClickGxbtnWorktNextMove();
	void ClickGxbtnWorktBCamCh1Lx();
	void ClickGxbtnWorktBCamCh1Rx();
	void ClickGxbtnWorktBCamCh2Lx();
	void ClickGxbtnWorktBCamCh2Rx();
	void ClickGxbtnWorktBCamCh1Ly();
	void ClickGxbtnWorktBCamCh1Ry();
	void ClickGxbtnWorktBCamCh2Ly();
	void ClickGxbtnWorktBCamCh2Ry();
	void ClickGxbtnWorktCCamX();
	void ClickGxbtnWorktCLcamZoom();
	void ClickGxbtnWorktCLcamTheta();
	void ClickGxbtnWorktCLcamFocus();
	void ClickGxbtnWorktCRcamZoom();
	void ClickGxbtnWorktCRcamTheta();
	void ClickGxbtnWorktCRcamFocus();
	void ClickGxbtnWorktBCamYReady();
	void ClickGxbtnWorktBCamXReady();
	void ClickGxbtnWorktBCamCh1lAlign();
	void ClickGxbtnWorktBCamCh1rAlign();
	void ClickGxbtnWorktBCamCh2lAlign();
	void ClickGxbtnWorktBCamCh2rAlign();
};


