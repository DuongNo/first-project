#pragma once

#include "resource.h"
// CDlgLossTime2 ��ȭ �����Դϴ�.



class CDlgLosstime2 : public CDialog
{
	DECLARE_DYNAMIC(CDlgLosstime2)

public:
	CDlgLosstime2(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgLosstime2();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_LOSSTIME2 };


	// CArray <UINT, UINT&> m_arrItems;							// ������ �� �ִ� ��ư Resource ID
	CMap <CString, LPCTSTR, UINT, UINT> m_mapItems;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam);						// ȭ�鳻���� �������ش�
	afx_msg LRESULT OnChangeLanguage(WPARAM wParam, LPARAM lParam);		// ǥ�� ��� ����
//	afx_msg void OnIdcancel();

	void DispTPCode(CString sLine);							// ���� �񰡵� ������ �ش�Ǵ� ��ư�� �����Ų��.
	void DispClear();												// Blink ��Ų ��ư�� �ʱ�ȭ �Ѵ�.
	void SetTPCode(CString sLine);							// ���� �񰡵� ������ �����Ѵ�.
	
	CString			m_sWorkUser;					// �۾��� 2017-03-25,skcho �߰�

public:
	DECLARE_EVENTSINK_MAP()
	void ClickGxbtnLosstime2Cancel();
	virtual void OnCancel();
	virtual void OnOK();
	void ClickBtn_Item1_1();
	void ClickBtn_Item2_1();
	void ClickBtn_Item2_2();
	void ClickBtn_Item2_3();
	void ClickBtn_Item3_1();
	void ClickBtn_Item3_2();
	void ClickBtn_Item3_3();
	void ClickBtn_Item4_1();
	void ClickBtn_Item4_2();
	void ClickBtn_Item5_1();
	void ClickBtn_Item5_2();
};
