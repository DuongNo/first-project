#pragma once
#include "resource.h"


// CDlgBarHTeach1 ��ȭ �����Դϴ�.

class CDlgBarHTeach2 : public CDialogBar
{
	DECLARE_DYNAMIC(CDlgBarHTeach2)

public:
	CDlgBarHTeach2();   // ǥ�� �������Դϴ�.
	virtual ~CDlgBarHTeach2();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_BAR_HTEACH2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()


protected:
	// ������ ȭ������ ��ȯ�Ѵ�.
	void ChangeForm(int nIdx, BOOL bForce = FALSE);

	// ����� ������ ȭ�鿡 ǥ���Ѵ�.
	void DispCurrentSelect(UINT nID);						// ���� ���õ� ��ư�� ���� �޸����ش�.

	// afx_msg void OnBtnClickMenu1();
	afx_msg void OnUpdateCmdUI(CCmdUI* pCmdUI);

	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// ȭ�� ó���� ���� �޼��� �Լ�
	afx_msg LRESULT OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam);			// View/Dialog�� ȭ�� ���ſ� Ÿ�̸��� ���� �� ���� ����
	afx_msg LRESULT OnChangeViewEnd(WPARAM wParam, LPARAM lParam);				// ���� View ��ȯ �Ϸ� ����
	afx_msg LRESULT OnChangeLanguage(WPARAM wParam, LPARAM lParam);			// ǥ�� �� ����Ǿ���.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


public:
	DECLARE_EVENTSINK_MAP();
	void OnClickBtnxWorkTable();
	void OnClickBtnx_UD_TrStage();
	void OnClickBtnx_UDConv();
	void OnClickBtn_InspectionUnit();
};
