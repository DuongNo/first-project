#pragma once

#include "resource.h"
#include "UI\GausGUI\GxImage.h"

// CFormPM 폼 뷰입니다.

class CFormPM : public CFormView
{
	DECLARE_DYNCREATE(CFormPM)

public:
	CFormPM();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CFormPM();

public:
	enum { IDD = IDD_FORM_PM };
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
	DECLARE_EVENTSINK_MAP()

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	void ClickBtnReleasePM();
	void ClickBtnOpenDoor();

	void ChangeForm(int nIdx, BOOL bForce = FALSE);

public:
	CGxImage m_imgPM;
	
};



