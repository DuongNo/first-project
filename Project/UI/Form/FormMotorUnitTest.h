#pragma once

#include "resource.h"
#include "UI\GridCtrl\GridCtrl.h"
#include "CheetahApp.h"

// CFormMotorUnitTest 폼 뷰입니다.

class CFormMotorUnitTest : public CFormView
{
	DECLARE_DYNCREATE(CFormMotorUnitTest)

public:
	CFormMotorUnitTest();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CFormMotorUnitTest();

public:
	enum { IDD = IDD_FORM_MOTORUNITTEST };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	enum JOG_SPEED
	{
		JOG_LOW = 0,
		JOG_MID,
		JOG_HIGH
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// 화면 처리에 대한 메세지 함수
	afx_msg LRESULT OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam);			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	afx_msg LRESULT OnChangeLanguage(WPARAM wParam, LPARAM lParam);			// 표시 언어가 변경되었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	DECLARE_MESSAGE_MAP()

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);


protected:					//	함수입니다.
	void InitializeGrid();
	void HighlightCurAxis(BOOL bRefrash);
	void UpdateMotorGrid();
	void HighlightCurTeach(BOOL bRefrash);
	void UpdateTeachGrid();

	void CurAxisChange(AXIS_ID axis);

	void UpdateMotorParam();

	// Timer
	void UpdateMotorListState();
	void UpdateMotorState();

protected:					//	변수입니다.
	CGridCtrl m_MotorGrid;
	CGridCtrl m_TeachGrid;

	int m_iUpDownParam;
	BOOL m_bInit;

	int m_nLastMotorGridRow;
	AXIS_ID m_CurAxis;
	int m_nLastTeachGridRow;
	int m_nCurTeachRow;
	CArray <TEACH_PARAM, TEACH_PARAM> m_arrTeach	;

	JOG_SPEED m_JogSpeed;
	BOOL m_bMove;


public:
	void SelectedAxis(int iSell, int iSel2);

public:
	DECLARE_EVENTSINK_MAP()
	void ClickGxbtnSetup4Halfup();
	void ClickGxbtnSetup4Halfdown();
	void ClickGxbtnSetup4Up();
	void ClickGxbtnSetup4Down();
	

protected:
	afx_msg void OnGridMoterEndSelChange(NMHDR *pNotifyStruct, LRESULT *pResult);
	afx_msg void OnGridTeachEndSelChange(NMHDR *pNotifyStruct, LRESULT *pResult);
public:
	virtual void OnInitialUpdate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void ClickGxbtnSetup4Servoon();
	void ClickGxbtnSetup4Servooff();
	void ClickGxbtnSetup4Reset();
	void ClickGxbtnSetup4Origin();
	void ClickGxbtnSetup4MoveStop();
	void ClickGxbtnSetup4Save();
	void ClickGxbtnSetup4Teachmove();
	void ClickGxbtnSetup4Teachsave();
	void ClickGxbtnSetup4SpeedJogHigh();
	void ClickGxbtnSetup4SpeedJogMid();
	void ClickGxbtnSetup4SpeedJogLow();
	void ClickGxbtnSetup4SpeedMove();
	void ClickGxbtnSetup4Homespeed();
	void ClickGxbtnSetup4Plimit();
	void ClickGxbtnSetup4Nlimit();
	void ClickGxbtnSetup4Ratio();
	void ClickGxbtnSetup4SpeedJogHighCheck();
	void ClickGxbtnSetup4SpeedJogMidCheck();
	void ClickGxbtnSetup4SpeedJogLowCheck();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void MouseDownGxbtnSetup4JogN(short Button, short Shift, long x, long y);
	void MouseUpGxbtnSetup4JogN(short Button, short Shift, long x, long y);
	void MouseDownGxbtnSetup4JogP(short Button, short Shift, long x, long y);
	void MouseUpGxbtnSetup4JogP(short Button, short Shift, long x, long y);
	void ClickGxbtnSetup4PosInc();
	void ClickGxbtnSetup4PosAbs();
	void ClickGxbtnSetup4IncMoveN();
	void ClickGxbtnSetup4IncMoveP();
	void ClickGxbtnSetup4AbsMove();

	void EnableButtons(BOOL bVal);

	void MotionSpeedSave();
	void ClickGxbtnSetup4MoveAllstop();

	// 환경안전에서 모텅 움직일 시에 도어 열리면 바로 모터 정지 요청하여 메세지 뿌려주는거 말고 도어 열림만 바로 체크 하는 함수 추가.
	BOOL DoorOpenChk();

};


