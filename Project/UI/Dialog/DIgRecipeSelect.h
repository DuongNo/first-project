//#pragma once
//
//
//// CDIgRecipeSelect 대화 상자입니다.
//
//class CDIgRecipeSelect : public CDialog
//{
//	DECLARE_DYNAMIC(CDIgRecipeSelect)
//
//public:
//	CDIgRecipeSelect(CWnd* pParent = NULL);   // 표준 생성자입니다.
//	virtual ~CDIgRecipeSelect();
//
//// 대화 상자 데이터입니다.
//	enum { IDD = IDD_DLG_RECIPE_SELECT };
//
//protected:
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
//
//	DECLARE_MESSAGE_MAP()
//public:
//	void ClickGxbtnRecipeCancle();
//};
#pragma once

#include "UI\GridCtrl\GridCtrl.h"
// DIgRecipeSelect 대화 상자입니다.

class CDIgRecipeSelect : public CDialog
{
	DECLARE_DYNAMIC(CDIgRecipeSelect)

public:
	CDIgRecipeSelect(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDIgRecipeSelect();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_RECIPE_SELECT };

protected:
	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	virtual BOOL OnInitDialog();
	DWORD	m_dwTextStyle;
	LOGFONT	m_lf;
	std::vector<CString> m_vecRecipe;
	std::vector<CString> m_vecRecipeTemp;
	int m_iUpDownParam;
	int m_iSelectedRow;
	afx_msg void OnGridRecipeEndSelChange(NMHDR *pNotifyStruct, LRESULT *pResult);	
	afx_msg LRESULT OnChangeLanguage(WPARAM wParam, LPARAM lParam);			// 표시 언어가 변경되었다.

	
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
