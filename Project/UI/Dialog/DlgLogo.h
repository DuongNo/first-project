#pragma once
#include "UI\GausGUI\GxImage.h"

// CDlgLogo ��ȭ �����Դϴ�.

class CDlgLogo : public CDialog
{
	DECLARE_DYNAMIC(CDlgLogo)

public:
	CDlgLogo(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgLogo();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_LOGO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	CGxImage m_imgLogo;
	afx_msg void OnTimer(UINT_PTR nIDEvent);

};
