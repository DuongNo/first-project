//
// Report �׷��� ���� �޴� ���ÿ� ����/������ ���� �޴� Dialogbar
//
//

#pragma once

#include "resource.h"

// CDlgbarVReport ��ȭ �����Դϴ�.

class CDlgbarVReport : public CDialogBar
{
	DECLARE_DYNAMIC(CDlgbarVReport)

public:
	CDlgbarVReport();   // ǥ�� �������Դϴ�.
	virtual ~CDlgbarVReport();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_BAR_VREPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	// ������ ȭ������ ��ȯ�Ѵ�.
	void ChangeForm(int nIdx, BOOL bForce = FALSE);

	// ����� ������ ȭ�鿡 ǥ���Ѵ�.
	void DispCurrentSelect(UINT nID);						// ���� ���õ� ��ư�� ���� �޸����ش�.


	DECLARE_MESSAGE_MAP()
	afx_msg void OnUpdateCmdUI(CCmdUI* pCmdUI);

	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// ȭ�� ó���� ���� �޼��� �Լ�
	afx_msg LRESULT OnChangeViewEnd(WPARAM wParam, LPARAM lParam);				// ���� View ��ȯ �Ϸ� ����
	afx_msg LRESULT OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam);			// View/Dialog�� ȭ�� ���ſ� Ÿ�̸��� ���� �� ���� ����
	afx_msg LRESULT OnChangeLanguage(WPARAM wParam, LPARAM lParam);			// ǥ�� �� ����Ǿ���.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

public:

	// ActiveX ó��
	DECLARE_EVENTSINK_MAP()
	void OnClickBtnxSubMenu1();
	void OnClickBtnxSubMenu2();
	void OnClickBtnxSubMenu3();
	void OnClickBtnxSubMenu4();
	void OnClickBtnxSubMenu5();
};
