#pragma once


#define OLBDOWNTIMER 333
// CFormOLBDown �� ���Դϴ�.

class CFormOLBDown : public CFormView
{
	DECLARE_DYNCREATE(CFormOLBDown)

public:
	CFormOLBDown();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	void ChangeForm(int nIdx, BOOL bForce = FALSE);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam);			// View/Dialog�� ȭ�� ���ſ� Ÿ�̸��� ���� �� ���� ����
};


