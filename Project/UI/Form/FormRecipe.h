#pragma once
#include "resource.h"
#include "UI\GridCtrl\GridCtrl.h"



// CFormRecipe �� ���Դϴ�.

class CFormRecipe : public CFormView
{
	DECLARE_DYNCREATE(CFormRecipe)

public:
	CFormRecipe();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CFormRecipe();

public:
	enum { IDD = IDD_FORM_RECIPE };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// ȭ�� ó���� ���� �޼��� �Լ�
	afx_msg LRESULT OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam);			// View/Dialog�� ȭ�� ���ſ� Ÿ�̸��� ���� �� ���� ����
	afx_msg LRESULT OnChangeLanguage(WPARAM wParam, LPARAM lParam);			// ǥ�� �� ����Ǿ���.
	afx_msg LRESULT OnRecipeUpdate(WPARAM wParam, LPARAM lParam);			// ǥ�� �� ����Ǿ���.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	DECLARE_MESSAGE_MAP()

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	DECLARE_EVENTSINK_MAP()
	void ClickGxbtnTeach1Create();
	void RecipeChangeConfirm();

protected:
	std::vector<CString> m_vecRecipe;
	std::vector<CString> m_vecRecipeTemp;
	std::vector<CString> m_vecSetValue;
	
	std::vector<CString> m_vecSetValueTemp;//�ӽ� ������ ����

	CGridCtrl m_GridModelList;
	CRecipeBank			m_RecipeBank;
	LOGFONT	m_lf;
	DWORD	m_dwTextStyle;
	//NM_GRIDVIEW* m_pItem;
	int		m_iSelectedRow;
	int     m_iSetVelNumIndex;
	int		m_iUpDownParam;
	int     m_SetVelNum;
	
	void    UpdateRuleInfo();  //2017-03-09, skcho, rule info ������Ʈ 

protected:
	afx_msg void OnGridRecipeEndSelChange(NMHDR *pNotifyStruct, LRESULT *pResult);
	
	void InitRecipeGrid();
	void UpdateGird();
	void UpdateSetValue(int sel);
	void LoadRecipe();
	void ClearSetValue();
	void SetValueUse(BOOL flag);
public:
	void ClickGxbtnRecipeDelete();
	void ClickGxbtnRecipeMoveUp();
	void ClickGxbtnRecipeMoveDown();
	void ClickGxbtnRecipeEdit();
	void ClickGxbtnRecipeCellWidth();
	void ClickGxbtnRecipeCellHeight();
	void ClickGxbtnRecipeCellFPCBUse();
	void ClickGxbtnRecipeItemValue4();
	void ClickGxbtnRecipeItemValue5();
	void ClickGxbtnRecipeItemValue6();
	void ClickGxbtnRecipeItemValue7();
	void ClickGxbtnRecipeTrayCountX();
	void ClickGxbtnRecipeTrayCountY();
	void ClickGxbtnRecipeTrayPitchX();
	void ClickGxbtnRecipeTrayPitchY();
	void ClickGxbtnRecipeTrayPitchOffsetX();
	void ClickGxbtnRecipeTrayPitchOffsetY();
	void ClickGxbtnRecipeTrayThickness();
	void ClickGxbtnRecipeItemValue16();
	void ClickGxbtnRecipeModifySave();
	void ClickGxbtnRecipeModifyCancle();

	void ClickGxbtnRuleInfoRead();
	void ClickGxbtnCurrentProduct();
	void ClickGxbtnCurrentPpid();
	void ClickGxbtnTrayQtyInLot();
	void ClickGxbtnCellQtyInLot();
	void ClickGxbtnNextStepid();
	void ClickGxbtnRuleInfoSave();
};


