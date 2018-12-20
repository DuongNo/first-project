#pragma once


#define OLBDOWNTIMER 333
// CFormOLBDown 폼 뷰입니다.

class CFormOLBDown : public CFormView
{
	DECLARE_DYNCREATE(CFormOLBDown)

public:
	CFormOLBDown();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CFormOLBDown();

	int m_nSpanTime;
public:
	enum { IDD = IDD_FORM_OLBRUNDOWN };
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
	afx_msg void OnBnClickedButton1();
	void ChangeForm(int nIdx, BOOL bForce = FALSE);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam);			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
};


