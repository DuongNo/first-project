#pragma once

#include "resource.h"
#include "MainFrm.h"

// CFormTeach9 �� ���Դϴ�.

class CFormTeach9 : public CFormView
{
	DECLARE_DYNCREATE(CFormTeach9)

public:
	CFormTeach9();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	CMainFrame* m_pMain;
	afx_msg LRESULT OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam);			// View/Dialog�� ȭ�� ���ſ� Ÿ�̸��� ���� �� ���� ����

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


