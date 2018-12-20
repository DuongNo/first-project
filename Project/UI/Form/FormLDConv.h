#pragma once

#include "resource.h"

//kjpark 20161024 Load Conveyor 구현
struct LD_CONVERY_ELEMENT
{
	UINT nBtnRecouseID;
	UINT nStaticRecouseID;
	BOOL bCheck;
};

//kjpark 20161024 Load Conveyor 구현
struct LD_CONVERY_T_ELEMENT
{
	AXIS_ID axis;
	TEACH_PARAM::ID teach;
	UINT nBtnRecouseID;
	UINT nStaticRecouseID;
	BOOL bCheck;
};
// CFormTeach1 폼 뷰입니다.

class CFormLDConv : public CFormView
{
	DECLARE_DYNCREATE(CFormLDConv)

public:
	CFormLDConv();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CFormLDConv();

public:
	enum { IDD = IDD_FORM_TEACH_LDCONVEYOR };
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
	//kjpark 20161024 Load Conveyor 구현
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	DECLARE_MESSAGE_MAP()
	//kjpark 20161024 Load Conveyor 구현
	DECLARE_EVENTSINK_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	//kjpark 20161024 Load Conveyor 구현
	LD_CONVERY_ELEMENT ConveryCell;
	LD_CONVERY_ELEMENT ReversUp;
	LD_CONVERY_ELEMENT ReversDown;
	LD_CONVERY_ELEMENT VacuumIN;
	LD_CONVERY_ELEMENT VacuumON;
	LD_CONVERY_ELEMENT VacuumOFF;

	//kjpark 20161024 Load Conveyor 구현
	LD_CONVERY_T_ELEMENT T_Un;
	LD_CONVERY_T_ELEMENT T_Down;
	void Init();
	void GetStatus();

	//kjpark 20161024 Load Conveyor 구현
	void GetCellCheck();
	void GetReverseCheck();
	void GetVacuumCheck();
	void GetT_UpCheck();
	void GetT_DownCheck();
	void ClickGxbtnCylinderUP();
	void ClickGxbtnCylinderDown();
	void ClickGxbtnTUP();
	void ClickGxbtnTDown();
};


