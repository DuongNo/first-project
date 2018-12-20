#pragma once

#include "UI\GausGUI\GxButtonEx.h"


// CFormTeachUnloaderTR 폼 뷰입니다.

class CFormTeachUnloaderTR : public CFormView
{
	DECLARE_DYNCREATE(CFormTeachUnloaderTR)

protected:
public:
	CFormTeachUnloaderTR();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CFormTeachUnloaderTR();

	enum { IDD = IDD_FORM_TEACH_UNLOADER_TR };
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
	BOOL m_bShow;
	BOOL m_bInit;
	AXIS_ID			m_CurAxis;
	TEACH_PARAM::ID	m_CurTeach;
	UINT			m_CurID;
	JIG_CH nCh;


	BOOL m_bButtonCheck[4][4];
	UINT m_nCellButton[4][4];
	void GetSelectButton(int x, int y);
	void SetCurPageAxis(AXIS_ID axis, TEACH_PARAM::ID param, UINT nID);
	void UpdateULDTRState();

	virtual void OnInitialUpdate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_EVENTSINK_MAP()
	void ClickGxstcUnldtrUnldtrXGet();
	void ClickGxstcUnldtrUnldtrXPut();
	void ClickGxstcUnldtrUnldtrZGet();
	void ClickGxstcUnldtrUnldtrZPut();
	void ClickGxstcUnldtrUnldtrZUp();
	void ClickGxbtnUldRobotUldstgCh1Get();
	void ClickGxbtnUldRobotUldstgCh2Get();
	void ClickGxbtnUldrobotVacOn();
	void ClickGxbtnUldrobotVacOff();
	void ClickGxbtnUdRobotOrg();
	void ClickGxbtnUnldRobotTray00();
	void ClickGxbtnUnldRobotTray01();
	void ClickGxbtnUnldRobotTray02();
	void ClickGxbtnUnldRobotTray03();
	void ClickGxbtnUnldRobotTray10();
	void ClickGxbtnUnldRobotTray11();
	void ClickGxbtnUnldRobotTray12();
	void ClickGxbtnUnldRobotTray13();
	void ClickGxbtnUnldRobotTray20();
	void ClickGxbtnUnldRobotTray21();
	void ClickGxbtnUnldRobotTray22();
	void ClickGxbtnUnldRobotTray23();
	void ClickGxbtnUnldRobotTray30();
	void ClickGxbtnUnldRobotTray31();
	void ClickGxbtnUnldRobotTray32();
	void ClickGxbtnUnldRobotTray33();
	void ClickGxbtnUnldRobotGtray();
	void ClickGxbtnUnldRobotNgtray();
	void ClickGxbtnUnldtrCh1VacOn();
	void ClickGxbtnUnldtrCh1VacOff();
	void ClickGxbtnUnldtrCh2VacOn();
	void ClickGxbtnUnldtrCh2VacOff();
	void ClickGxbtnUldstgCh1VacOn();
	void ClickGxbtnUldstgCh1VacOff();
	void ClickGxbtnUldstgCh2VacOn();
	void ClickGxbtnUldstgCh2VacOff();

	int m_nASeq;
	int m_nBSeq;
	int m_nCSeq;
	CStopWatch m_Timer;

	void UDRobotOrg();
	void UDRobotGet();
	void UDRobotPut();
	enum 
	{
		seqA_idle = -1,
		seqA_1,
		seqA_2,
		seqA_3,
		seqA_4,
		seqA_5,
		seqA_6,
		seqA_7,
		seqA_8,
		seqA_9,
		seqA_10,
		seqA_End
	};

	enum 
	{
		seqB_idle = -1,
		seqB_1,
		seqB_2,
		seqB_3,
		seqB_4,
		seqB_5,
		seqB_6,
		seqB_7,
		seqB_8,
		seqB_9,
		seqB_End
	};

	enum 
	{
		seqC_idle = -1,
		seqC_1,
		seqC_2,
		seqC_3,
		seqC_4,
		seqC_5,
		seqC_6,
		seqC_7,
		seqC_8,
		seqC_9,
		seqC_End
	};
	void ClickGxbtnUnldRobotTrayPitch();
	void ClickGxbtnUldRobotUldstgCh1GetOk();
	void ClickGxbtnUldRobotUldstgCh1GetNg();
	void ClickGxbtnUldRobotUldstgCh2GetOk();
	void ClickGxbtnUldRobotUldstgCh2GetNg();

	BOOL bCh1OK;
	BOOL bCh2OK;
};


