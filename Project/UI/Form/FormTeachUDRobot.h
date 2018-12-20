#pragma once



// CFormTeachUDRobot 폼 뷰입니다.

class CFormTeachUDRobot : public CFormView
{
	DECLARE_DYNCREATE(CFormTeachUDRobot)

protected:
public:
	CFormTeachUDRobot();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CFormTeachUDRobot();

public:
	enum { IDD = IDD_FORM_TEACH_UDROBOT };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	afx_msg LRESULT OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam);			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	afx_msg LRESULT OnChangeLanguage(WPARAM wParam, LPARAM lParam);			// 표시 언어가 변경되었다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	DECLARE_EVENTSINK_MAP()


	BOOL m_bInit;

	virtual void OnInitialUpdate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void UpdateUDRobotState();

	void ClickGxbtnUdrobotOutput();
	void ClickGxbtnUdrobotOutput2();
	void ClickGxbtnUdrobotOutput3();
	void ClickGxbtnUdrobotOutput4();
	void ClickGxbtnUdrobotOutput5();
	void ClickGxbtnUdrobotOutput6();
	void ClickGxbtnUdrobotOutput7();
	void ClickGxbtnUdrobotOutput8();
	void ClickGxbtnUdrobotOutput9();
	void ClickGxbtnUdrobotOutput10();
	void ClickGxbtnUdrobotOutput11();
	void ClickGxbtnUdrobotOutput12();
	void ClickGxbtnUdrobotOutput13();
	void ClickGxbtnUdrobotOutput14();
	void ClickGxbtnUdrobotOutput15();
	void ClickGxbtnUdrobotOutput16();
	void ClickGxbtnUdrobotOutput17();
	void ClickGxbtnUdrobotOutput18();
	void ClickGxbtnUdrobotOutput19();
	void ClickGxbtnUdrobotOutput20();
	void ClickGxbtnUdrobotOutput21();
	void ClickGxbtnUdrobotOutput22();
	void ClickGxbtnUdrobotOutput23();
	void ClickGxbtnUdrobotOutput24();
	void ClickGxbtnUdrobotOutput25();
	void ClickGxbtnUdrobotOutput26();
	void ClickGxbtnUdrobotOutput27();
	void ClickGxbtnUdrobotOutput28();
	void ClickGxbtnUdrobotOutput29();
	void ClickGxbtnUdrobotOutput30();
	void ClickGxbtnUdrobotOutput31();
	void ClickGxbtnUdrobotOutput32();
	void ClickGxbtnUdrobotOutput33();
	void ClickGxbtnUdrobotOutput34();
	void ClickGxbtnUdrobotOutput35();
	void ClickGxbtnUdrobotOutput36();
	void ClickGxbtnUdrobotOutput37();
	void ClickGxbtnUdrobotOutput38();
	void ClickGxbtnUdrobotOutput39();
	void ClickGxbtnUdrobotOutput40();
	void ClickGxbtnUdrobotOutput41();
	void ClickGxbtnUdrobotOutput42();
	void ClickGxbtnUdrobotOutput43();
	void ClickGxbtnUdrobotOutput44();
	void ClickGxbtnUdrobotOutput45();
	void ClickGxbtnUdrobotOutput46();
	void ClickGxbtnUdrobotOutput47();
	void ClickGxbtnUdrobotOutput48();
	void ClickGxbtnUdrobotOutput49();
	void ClickGxbtnUdrobotOutput50();
	void ClickGxbtnUdrobotOutput51();
	void ClickGxbtnUdrobotOutput52();
	void ClickGxbtnUdrobotOutput53();
	void ClickGxbtnUdrobotOutput54();
	void ClickGxbtnUdrobotOutput55();
	void ClickGxbtnUdrobotOutput56();
	void ClickGxbtnUdrobotOutput57();
	void ClickGxbtnUdrobotOutput58();
	void ClickGxbtnUdrobotOutput59();
	void ClickGxbtnUdrobotOutput60();
	void ClickGxbtnUdrobotOutput61();
	void ClickGxbtnUdrobotOutput62();
	void ClickGxbtnUdrobotOutput63();
	void ClickGxbtnUdrobotOutput64();
	void ClickGxbtnUdrobotOutput65();
	void ClickGxbtnUdrobotOutput66();
	void ClickGxbtnUdrobotOutput67();
	void ClickGxbtnUdrobotOutput68();
	void ClickGxbtnUdrobotOutput69();
	void ClickGxbtnUdrobotOutput70();
	void ClickGxbtnUdrobotOutput71();
	void ClickGxbtnUdrobotOutput72();
	void ClickGxbtnUdrobotOutput73();

	BOOL bStatus[100];
	void ClickGxbtnUdrobotGetApproach();
	void ClickGxbtnUdrobotGetWait();
	void ClickGxbtnUdrobotGetCh1Up();
	void ClickGxbtnUdrobotGetCh1Down();
	void ClickGxbtnUdrobotGetCh2Up();
	void ClickGxbtnUdrobotGetCh2Down();
	void ClickGxbtnUdrobotGoodputApproach();
	void ClickGxbtnUdrobotGoodputWait();
	void ClickGxbtnUdrobotGoodputUp();
	void ClickGxbtnUdrobotGoodputDown();
	void ClickGxbtnUdrobotNgputApproach();
	void ClickGxbtnUdrobotNgputWait();
	void ClickGxbtnUdrobotNgputUp();
	void ClickGxbtnUdrobotNgputDown();
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
	void ClickGxbtnUdrobotVacOn();
	void ClickGxbtnUdrobotVacOff();
	void ClickGxbtnUdrobotMove();

	BOOL ManualPosCheck(int Pos);
};


