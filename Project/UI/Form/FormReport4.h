#pragma once

#include "resource.h"
#include "UI\GridCtrl\GridCtrl.h"


// CFormReport4 폼 뷰입니다.

class CFormReport4 : public CFormView
{
	DECLARE_DYNCREATE(CFormReport4)

public:
	CFormReport4();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CFormReport4();

public:
	enum { IDD = IDD_FORM_REPORT4 };
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
protected:
	int m_iUpDownParam;
	CGridCtrl m_AlarmGrid;
	BOOL	IsOpenFile; //파일열기 버튼을 눌렀는지 확인
	CString logpath; //error 파일 저장된 경로
	std::vector<CString> m_vecErrorCode;
	std::vector<CString> m_vecAlarmName;
	std::vector<CString> m_vecAlarm_StartTime;
	std::vector<CString> m_vecAlarm_EndTime;
	std::vector<CString> m_vecAlarmTakeTime;
	//std::vector<CString> m_vecAlarmStat;
	//std::vector<CString> m_vecAlarmCode;
	std::vector<CString> m_vecAlarmLevel;
	void	AlarmGrid();
	void	LoadArarm();
	void	UpdateArarmGrid();


public:

	DECLARE_EVENTSINK_MAP()
	void ClickGxbtnHalfdown();
	void ClickGxbtnDown();
	void ClickGxbtnHalfup();
	void ClickGxbtnUp();
	void ClickGxbtn();
};


