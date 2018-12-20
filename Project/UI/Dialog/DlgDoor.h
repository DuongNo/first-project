#pragma once
#include "UI\GridCtrl\GridCtrl.h"

#include <vector>

typedef struct
{
	int nIn;
	int nOut;
}tDoor;


// CDlgDoor 대화 상자입니다.

class CDlgDoor : public CDialog
{
	DECLARE_DYNAMIC(CDlgDoor)

public:
	CDlgDoor(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgDoor();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_DOOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()

protected:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	void OnGridDoorSelChange(NMHDR *pNotifyStruct, LRESULT *pResult);
	void InitGrid();
	void UpdateGrid();
	void ClickBtnDoorAllOpen();
	void ClickBtnDoorAllClose();
	void ClickBtnDoorCancel();

protected:
	CGridCtrl m_Grid;

	std::vector<tDoor> m_vecDoor;

public:

	
	


};
