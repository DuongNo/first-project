#pragma once

#include "resource.h"
#include "UI\GausGUI\GxStaticEx.h"
#include "afxcmn.h"


// CFormInitialize 폼 뷰입니다.

class CFormInitialize : public CFormView
{
	DECLARE_DYNCREATE(CFormInitialize)

public:
	CFormInitialize();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CFormInitialize();

public:
	enum { IDD = IDD_FORM_INITIALIZE };
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

	// OnInitialUpdate가 두번 실행되기에 한번을 막는다 [10/24/2016 OSC]
	BOOL m_bInit;

	enum INITIALIZE_STEP
	{
		stepStart,
		stepInitCylinder,
		stepCheckCylinder,
		stepServoOff,
		stepAlarmReset,
		stepServoOn,
		stepRobotRunChk,
		stepRobotPause,
		stepRobotExtStop,
		stepRobotMotorRun,
		stepRobotExtReset,
		stepRobotPGMSel,
		stepRobotPGMStrobe,
		stepRobotEXTStart,
		stepRobotOrg,
		stepRobotOrgOff,
		stepInit1stList,
		stepCheck1stList,
		stepAlignYReady,
		stepAlignYCheck,
		stepInit2ndList,
		stepCheck2ndList,
		stepInit3rdList,
		stepCheck3rdList,
		stepEnd,
		stepAlarm,
		stepIdle
	};

	enum ORIGIN_STATUS
	{
		ORIGIN_NONE,
		ORIGIN_START,
		ORIGIN_FINISH,
		ORIGIN_FAIL,
		ORIGIN_STATUS_MAX
	};

	enum STEP_LED
	{
		LED_CYLINDER_INIT	,
		LED_SERVO_OFF		,
		LED_ALARM_RESET		,
		LED_SERVO_ON		,
		LED_1ST_ORIGIN		,
		LED_2ND_ORIGIN		,
		LED_3RD_ORIGIN		,
		LED_FINISH			,
		STEP_LED_MAX
	};

	INITIALIZE_STEP m_Step;
	CStopWatch m_Timer;

	ORIGIN_STATUS m_AxisOriginStatus[AXIS_ID_MAX];
	COLORREF m_Color[ORIGIN_STATUS_MAX];
	CGxStaticEx* m_pAxisOriginLed[AXIS_ID_MAX];

	ORIGIN_STATUS m_RobotOriginStatus[E_ROBOT_MAX];
	CGxStaticEx* m_pRobotOriginLed[E_ROBOT_MAX];

	ORIGIN_STATUS m_StepOriginStatus[STEP_LED_MAX];
	CGxStaticEx* m_pInitStepLed[STEP_LED_MAX];


	CList <AXIS_ID, AXIS_ID> m_list1stPart;
	CList <AXIS_ID, AXIS_ID> m_list2ndPart;
	CList <AXIS_ID, AXIS_ID> m_list3rdPart;

	CString m_strLastMsg;

	void AddAxis_to_List();
	void InitStatus();

	BOOL CheckPreInterlock(CString &strKorMsg, CString &strEngMsg, CString &strVnmMsg);
	BOOL CheckInterlock();

	void InitCylinder();
	BOOL CheckCylinder();
	BOOL ServoOff();
	BOOL AlarmReset();
	BOOL ServoOn();
	BOOL InitAxisList(CList <AXIS_ID, AXIS_ID> *pList);
	BOOL CheckAxisOriginComplete(CList <AXIS_ID, AXIS_ID> *pList);
	BOOL CheckAxisOriginFail();

	BOOL Initializing();
	void AxisAllStop();

	void UpdateAllStatus();
	void SetLogListUpdate(CString strMessage);	

public:
	DECLARE_EVENTSINK_MAP()
	virtual void OnInitialUpdate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void ClickGxbtnInitializeInitialized();
	void ClickGxbtnInitializeCancel();
	CListCtrl m_ListLog;
};


