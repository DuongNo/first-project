#pragma once
#include "resource.h"
#include "UI\GridCtrl\GridCtrl.h"



// CFormRecipe 폼 뷰입니다.

class CFormRecipe : public CFormView
{
	DECLARE_DYNCREATE(CFormRecipe)

public:
	CFormRecipe();           // 동적 만들기에 사용되는 protected 생성자입니다.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// 화면 처리에 대한 메세지 함수
	afx_msg LRESULT OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam);			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	afx_msg LRESULT OnChangeLanguage(WPARAM wParam, LPARAM lParam);			// 표시 언어가 변경되었다.
	afx_msg LRESULT OnRecipeUpdate(WPARAM wParam, LPARAM lParam);			// 표시 언어가 변경되었다.
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
	
	std::vector<CString> m_vecSetValueTemp;//임시 설정값 저장

	CGridCtrl m_GridModelList;
	CRecipeBank			m_RecipeBank;
	LOGFONT	m_lf;
	DWORD	m_dwTextStyle;
	//NM_GRIDVIEW* m_pItem;
	int		m_iSelectedRow;
	int     m_iSetVelNumIndex;
	int		m_iUpDownParam;
	int     m_SetVelNum;
	
	void    UpdateRuleInfo();  //2017-03-09, skcho, rule info 업데이트 

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


