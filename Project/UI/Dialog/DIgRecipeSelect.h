//#pragma once
//
//
//// CDIgRecipeSelect ��ȭ �����Դϴ�.
//
//class CDIgRecipeSelect : public CDialog
//{
//	DECLARE_DYNAMIC(CDIgRecipeSelect)
//
//public:
//	CDIgRecipeSelect(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
//	virtual ~CDIgRecipeSelect();
//
//// ��ȭ ���� �������Դϴ�.
//	enum { IDD = IDD_DLG_RECIPE_SELECT };
//
//protected:
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
//
//	DECLARE_MESSAGE_MAP()
//public:
//	void ClickGxbtnRecipeCancle();
//};
#pragma once

#include "UI\GridCtrl\GridCtrl.h"
// DIgRecipeSelect ��ȭ �����Դϴ�.

class CDIgRecipeSelect : public CDialog
{
	DECLARE_DYNAMIC(CDIgRecipeSelect)

public:
	CDIgRecipeSelect(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDIgRecipeSelect();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_RECIPE_SELECT };

protected:
	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	virtual BOOL OnInitDialog();
	DWORD	m_dwTextStyle;
	LOGFONT	m_lf;
	std::vector<CString> m_vecRecipe;
	std::vector<CString> m_vecRecipeTemp;
	int m_iUpDownParam;
	int m_iSelectedRow;
	afx_msg void OnGridRecipeEndSelChange(NMHDR *pNotifyStruct, LRESULT *pResult);	
	afx_msg LRESULT OnChangeLanguage(WPARAM wParam, LPARAM lParam);			// ǥ�� �� ����Ǿ���.

	
public:
	DECLARE_EVENTSINK_MAP()
	
	void ClickGxbtnRecipeCancle();
	void UpdateGird();
	void LoadRecipe();
	CGridCtrl m_GridModelList;
	void ClickGxbtnRecipeSelect();
	void ClickGxbtnTeach1MoveUp();
	void ClickGxbtnTeach1MoveDown();
};
