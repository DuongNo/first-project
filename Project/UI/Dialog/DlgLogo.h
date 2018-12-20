#pragma once
#include "UI\GausGUI\GxImage.h"

// CDlgLogo 대화 상자입니다.

class CDlgLogo : public CDialog
{
	DECLARE_DYNAMIC(CDlgLogo)

public:
	CDlgLogo(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgLogo();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_LOGO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	CGxImage m_imgLogo;
	afx_msg void OnTimer(UINT_PTR nIDEvent);

};
