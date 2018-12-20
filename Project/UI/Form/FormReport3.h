#pragma once

#include "resource.h"
#include <map>
typedef    struct
{
	CString			 Alarm_name;		
	CString				 nLevel;
} alarm_current[ALM_MAX];
// CFormReport3 폼 뷰입니다.

class CFormReport3 : public CFormView
{
	DECLARE_DYNCREATE(CFormReport3)

public:
	CFormReport3();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CFormReport3();
	int m_eIDX_Shift;// 확인할 작업자 상태
	CString m_sel_time;
	std::vector<CString> m_vecErrorCode;
	map<int, int> m_alarm_map;

public:
	enum { IDD = IDD_FORM_REPORT3 };
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
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	DECLARE_EVENTSINK_MAP()
	int sel_date;
	CTime time;
	alarm_current alarm_input;
	void ListUpdate();
	void ClearData();
	void AlarmUdate(int sel);
	void ShowHide();
	void ClickGxlblAlarmDay();
	void ClickGxlblAlarmSw();
	void ClickGxlblAlarmGy();
	void ClickGxlblAlarmPrev();
	void ClickGxlblAlarmNext();
	void ClickGxlblAlarmToday();
};


