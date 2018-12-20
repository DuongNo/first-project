#pragma once
#include "..\..\Resource.h"
#include "ui/gridctrl/gridctrl.h"


// CDlgPageIO 대화 상자입니다.

class CDlgPageIO : public CDialog
{
	DECLARE_DYNAMIC(CDlgPageIO)

public:
	CDlgPageIO(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgPageIO();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_IO_PAGE };

DECLARE_EVENTSINK_MAP()

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	void SetPageCount(int iRow)	{	m_iRowCount = iRow;	};
	void SetBoolInput(BOOL val) {	m_bInput = val; }


protected:
	afx_msg void OnGridSelChange(NMHDR *pNotifyStruct, LRESULT *pResult);
protected:
	CGridCtrl	m_GridPage;
	BOOL		m_bInput;
	int			m_iRowCount;
	int			m_iMove;
		

public:
	void InitGrid();
	void UpdateGrid();

	void ClickBtnPageDown();
	void ClickBtnPageUp();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
};
