#pragma once

#include "UI\GausGUI\GxImage.h"
#include "UI\GausGUI\GxStaticEx.h"


// CFormTeachLoaderTR 폼 뷰입니다.

class CFormTeachLoaderTR : public CFormView
{
	DECLARE_DYNCREATE(CFormTeachLoaderTR)

protected:
public:
	CFormTeachLoaderTR();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CFormTeachLoaderTR();

	enum { IDD = IDD_FORM_TEACH_LOADER_TR };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	afx_msg LRESULT OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam);			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	afx_msg LRESULT OnChangeLanguage(WPARAM wParam, LPARAM lParam);			// 표시 언어가 변경되었다.

	afx_msg LRESULT OnUpdateLOWERMCRData(WPARAM wParam, LPARAM lParam);				// MCR Data가 수신되었다.
	afx_msg LRESULT OnUpdateLOWERMCRLive(WPARAM wParam, LPARAM lParam);				// MCR Live 영상 수신
	afx_msg LRESULT OnUpdateUPPERMCRData(WPARAM wParam, LPARAM lParam);				// MCR Data가 수신되었다.
	afx_msg LRESULT OnUpdateUPPERMCRLive(WPARAM wParam, LPARAM lParam);				// MCR Live 영상 수신

	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	BOOL m_bLowerShow;
	BOOL m_bUpperShow;
	BOOL m_bInit;
	BOOL m_bLowerLiveMode;
	BOOL m_bUpperLiveMode;
	AXIS_ID			m_CurAxis;
	TEACH_PARAM::ID	m_CurTeach;
	UINT			m_CurID;

	virtual void OnInitialUpdate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void UpdateLDTRState();
	void SetCurPageAxis(AXIS_ID axis, TEACH_PARAM::ID param, UINT nID);

	CGxImage			m_Lower_imgLive;
	CGxImage			m_Upper_imgLive;

	void SetMCRLive();
	void SetMCRLive(MCR_UNIT Unit);
	void SetMCRData(MCR_UNIT Unit,CString strBarcode = _T(""));
	DECLARE_EVENTSINK_MAP()
	void ClickGxbtnLowermcrLive();
	void ClickGxbtnUppermcrLive();
	void ClickGxbtnLowermcrRead();
	void ClickGxbtnUppermcrRead();
	void ClickGxbtnLdtrXGet();
	void ClickGxbtnLdtrXPut();
	void ClickGxbtnLdtrXLowermcrCh1();
	void ClickGxbtnLdtrXLowermcrCh2();
	void ClickGxbtnLdtrXUppermcrCh1();
	void ClickGxbtnLdtrXUppermcrCh2();
	void ClickGxbtnLdtrZGet();
	void ClickGxbtnLdtrZPut();
	void ClickGxbtnLdtrZLower();
	void ClickGxbtnLdtrZLowermcrCh1();
	void ClickGxbtnLdtrZLowermcrCh2();
	void ClickGxbtnLdtrZUppermcrCh1();
	void ClickGxbtnLdtrZUppermcrCh2();
	void ClickGxbtnLdtrVacuumCh1On();
	void ClickGxbtnLdtrVacuumCh2On();
	void ClickGxbtnLdtrVacuumCh1Off();
	void ClickGxbtnLdtrVacuumCh2Off();
	void ClickGxbtnLdtrZUpper();
	void ClickGxbtnLdtrZUp();
	void ClickGxbtnLdtrFpcbBlowOn();
	void ClickGxbtnLdtrFpcbBlowOff();
	void ClickGxbtnLowermcrReconnect();
	void ClickGxbtnUppermcrReconnect();
};


