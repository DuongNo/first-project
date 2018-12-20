#pragma once

#include "resource.h"
#include "MainFrm.h"

// CFormTeach9 폼 뷰입니다.

class CFormTeach9 : public CFormView
{
	DECLARE_DYNCREATE(CFormTeach9)

public:
	CFormTeach9();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CFormTeach9();

public:
	enum { IDD = IDD_FORM_TEACH_TRAY };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	CMainFrame* m_pMain;
	afx_msg LRESULT OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam);			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void ClickGxbtnTrayTrYIn();
	void ClickGxbtnTrayTrYOut();
	void ClickGxbtnTrayTrZIn();
	void ClickGxbtnTrayTrZOut();
	void ClickGxbtnTrayLifInUp();
	void ClickGxbtnTrayLifInDown();
	void ClickGxbtnTrayLifOutUp();
	void ClickGxbtnTrayLifOutDown();
	void ClickGxbtnTrayTrYIn2();
	void ClickGxbtnTrayTrYOut2();
	void ClickGxbtnTrayTrZIn2();
	void ClickGxbtnTrayTrZOut2();
	void ClickGxbtnTrayLifInUp2();
	void ClickGxbtnTrayLifInDown2();
	void ClickGxbtnTrayLifOutUp2();
	void ClickGxbtnTrayLifOutDown2();
};


