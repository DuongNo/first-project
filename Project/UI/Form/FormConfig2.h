#pragma once
#include "resource.h"



// CFormConfig2 폼 뷰입니다.

class CFormConfig2 : public CFormView
{
	DECLARE_DYNCREATE(CFormConfig2)

public:
	CFormConfig2();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CFormConfig2();

public:
	enum { IDD = IDD_FORM_CONFIG2 };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	void SetUpdateData(void);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_EVENTSINK_MAP()
	void ClickOffsetButton();
};


