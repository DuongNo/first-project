#pragma once

#include "resource.h"
// CDlgLossTime2 대화 상자입니다.



class CDlgLosstime2 : public CDialog
{
	DECLARE_DYNAMIC(CDlgLosstime2)

public:
	CDlgLosstime2(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgLosstime2();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_LOSSTIME2 };


	// CArray <UINT, UINT&> m_arrItems;							// 선택할 수 있는 버튼 Resource ID
	CMap <CString, LPCTSTR, UINT, UINT> m_mapItems;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam);						// 화면내용을 갱신해준다
	afx_msg LRESULT OnChangeLanguage(WPARAM wParam, LPARAM lParam);		// 표시 언어 변경
//	afx_msg void OnIdcancel();

	void DispTPCode(CString sLine);							// 지정 비가동 사유에 해당되는 버튼을 점멸시킨다.
	void DispClear();												// Blink 시킨 버튼을 초기화 한다.
	void SetTPCode(CString sLine);							// 지정 비가동 사유를 적용한다.
	
	CString			m_sWorkUser;					// 작업자 2017-03-25,skcho 추가

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
