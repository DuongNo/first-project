#pragma once

//#include "File\GxLog.h"
//#include "BankGuild\MCMain.h"

#include "File\GxIni.h"
#include "BankGuild\MCDefine.h"

// CDlgLossTime_New ��ȭ �����Դϴ�.

class CDlgLossTimeNew : public CDialog
{
	DECLARE_DYNAMIC(CDlgLossTimeNew)
	enum {
		eMAX_GroupCount1		= 8,			// 1st Group ��
		eMAX_GroupCount2		= 8,			// 2nd Group ��
		eMAX_GroupCount3		= 24,			// 3rd �׸� ��
		eMAX_ItemCount			= 8,			// �� ȭ�鿡 ǥ���� �� �ִ� �񰡵� 3rd �׸� ��
	};
public:
	CDlgLossTimeNew(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgLossTimeNew();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_LOSSTIME_NEW };
	
	//CMCMain*			m_pMCMain;
	CCellInfo*		m_pCellInfo;				// Cell Data ���� Class
	//CGxLog*			m_pLogGUI;					// ȭ�� ���� Log
	//CGxLog*			m_pLogSeq;					// ó������ Log

	// ��� ����
	int				m_nLangIdx;			// ��� ���� Index

	// Defect List�� Page ��ȣ, 24���� �Ѿ�� �������� Page�� �����ȴ�.
	int				m_nSelect_1st;				// ���õ� 1��° Class�� ���� , 0 ~ 7
	int				m_nSelect_2nd;				// ���õ� 2��° Class�� ���� , 0 ~ 7
	int				m_nSelect_3rd;				// ���õ� 3��° Class�� ���� , 0 ~ 7

	int				m_n2ndCount;				// ���� 1st class���� �����Ǵ� 2nd Class �� �ִ� 8��
	int				m_n3rdCount;				// ���� 2st class���� �����Ǵ� 3rd Class �� �ִ� 24��

	int				m_nPageCount;				// �ִ� Page ��, 2nd class�� �ٲ𶧸��� class 3rd ������ ���� ���ŵȴ�.
	int				m_nCurrentPage;			// ���� �����ְ� �ִ� page
	int				m_nStartIndex;				// ���� page������ ���۵Ǵ� 3rd item�� ����, �� ���� �� ������ ��ư�� index�� �����Ͽ� ���ȴ�.
	int				m_nDisp3rdCount;			// ���� page���� ǥ�����ִ� 3rdClass �׸� ��

	CString			m_sSelectReason;			// ���� ���õ� ���� ����

	CString			m_sWorkUser;					// �۾���

	// ��ư ���ҽ� ���̵� ������ �迭
	UINT				m_idBtn_1st[eMAX_GroupCount1];		// 1st class ��ư ����
	UINT				m_idBtn_2nd[eMAX_GroupCount2];		// 2nd class ��ư ����
	UINT				m_idBtn_3rd[eMAX_ItemCount];			// 3rd class ��ư ����

	void SelectGroup_1st(int nIdx);						// ���� 1st class�� �����Ͽ���.
	void SelectGroup_2nd(int nIdx);						// ���� 2nd class�� �����Ͽ���.
	void SelectGroup_3rd(int nIdx);						// ���� 3rd class�� �����Ͽ���.
	void DispDefault();										// �⺻���� ȭ���� �׷��ش�.
	void DispGroup_1st();
	void DispGroup_2nd();
	void DispGroup_3rd(int nPage);						// Page�� 3rd class �׸� ǥ��

	
	void DispCurrentTime();							// ���� �ð��� �ݿ��Ѵ�.
	void DispCurrentData();							// ���� ������ ���� �ݿ��� �ش�.



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam);						// ȭ�鳻���� �������ش�

public:
	DECLARE_EVENTSINK_MAP()
	afx_msg void ClickGxbtn_Confirm();
	afx_msg void ClickGxbtn_Cancel();
	afx_msg void ClickGxbtn_Back();
	afx_msg void ClickGxbtn_Next();
	afx_msg void ClickGxbtn_Login();

	// 1st Class ����
	afx_msg void ClickGxbtn_Select1stClass1();
	afx_msg void ClickGxbtn_Select1stClass2();
	afx_msg void ClickGxbtn_Select1stClass3();
	afx_msg void ClickGxbtn_Select1stClass4();
	afx_msg void ClickGxbtn_Select1stClass5();
	afx_msg void ClickGxbtn_Select1stClass6();
	afx_msg void ClickGxbtn_Select1stClass7();
	afx_msg void ClickGxbtn_Select1stClass8();

	afx_msg void ClickGxbtn_Select2ndClass1();
	afx_msg void ClickGxbtn_Select2ndClass2();
	afx_msg void ClickGxbtn_Select2ndClass3();
	afx_msg void ClickGxbtn_Select2ndClass4();
	afx_msg void ClickGxbtn_Select2ndClass5();
	afx_msg void ClickGxbtn_Select2ndClass6();
	afx_msg void ClickGxbtn_Select2ndClass7();
	afx_msg void ClickGxbtn_Select2ndClass8();

	afx_msg void ClickGxbtn_Select3rdClass1();
	afx_msg void ClickGxbtn_Select3rdClass2();
	afx_msg void ClickGxbtn_Select3rdClass3();
	afx_msg void ClickGxbtn_Select3rdClass4();
	afx_msg void ClickGxbtn_Select3rdClass5();
	afx_msg void ClickGxbtn_Select3rdClass6();
	afx_msg void ClickGxbtn_Select3rdClass7();
	afx_msg void ClickGxbtn_Select3rdClass8();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
