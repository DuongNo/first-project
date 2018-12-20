#pragma once



// CFormTeachUDRobot 폼 뷰입니다.

class CFormTeachLDRobot : public CFormView
{
	DECLARE_DYNCREATE(CFormTeachLDRobot)

protected:
public:
	CFormTeachLDRobot();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CFormTeachLDRobot();

public:
	enum { IDD = IDD_FORM_TEACH_LDROBOT };
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
	void UpdateLDRobotState();

	void ClickGxbtnLdrobotOutput();
	void ClickGxbtnLdrobotOutput2();
	void ClickGxbtnLdrobotOutput3();
	void ClickGxbtnLdrobotOutput4();
	void ClickGxbtnLdrobotOutput5();
	void ClickGxbtnLdrobotOutput6();
	void ClickGxbtnLdrobotOutput7();
	void ClickGxbtnLdrobotOutput8();
	void ClickGxbtnLdrobotOutput9();
	void ClickGxbtnLdrobotOutput10();
	void ClickGxbtnLdrobotOutput11();
	void ClickGxbtnLdrobotOutput12();
	void ClickGxbtnLdrobotOutput13();
	void ClickGxbtnLdrobotOutput14();
	void ClickGxbtnLdrobotOutput15();
	void ClickGxbtnLdrobotOutput16();
	void ClickGxbtnLdrobotOutput17();
	void ClickGxbtnLdrobotOutput18();
	void ClickGxbtnLdrobotOutput19();
	void ClickGxbtnLdrobotOutput20();
	void ClickGxbtnLdrobotOutput21();
	void ClickGxbtnLdrobotOutput22();
	void ClickGxbtnLdrobotOutput23();
	void ClickGxbtnLdrobotOutput24();
	void ClickGxbtnLdrobotOutput25();
	void ClickGxbtnLdrobotOutput26();
	void ClickGxbtnLdrobotOutput27();
	void ClickGxbtnLdrobotOutput28();
	void ClickGxbtnLdrobotOutput29();
	void ClickGxbtnLdrobotOutput30();
	void ClickGxbtnLdrobotOutput31();
	void ClickGxbtnLdrobotOutput32();
	void ClickGxbtnLdrobotOutput33();
	void ClickGxbtnLdrobotOutput34();
	void ClickGxbtnLdrobotOutput35();
	void ClickGxbtnLdrobotOutput36();
	void ClickGxbtnLdrobotOutput37();
	void ClickGxbtnLdrobotOutput38();
	void ClickGxbtnLdrobotOutput39();
	void ClickGxbtnLdrobotOutput40();
	void ClickGxbtnLdrobotOutput41();
	void ClickGxbtnLdrobotOutput42();
	void ClickGxbtnLdrobotOutput43();
	void ClickGxbtnLdrobotOutput44();
	void ClickGxbtnLdrobotOutput45();
	void ClickGxbtnLdrobotOutput46();
	void ClickGxbtnLdrobotOutput47();
	void ClickGxbtnLdrobotOutput48();
	void ClickGxbtnLdrobotOutput49();
	void ClickGxbtnLdrobotOutput50();
	void ClickGxbtnLdrobotOutput51();
	void ClickGxbtnLdrobotOutput52();
	void ClickGxbtnLdrobotOutput53();
	void ClickGxbtnLdrobotOutput54();
	void ClickGxbtnLdrobotOutput55();
	void ClickGxbtnLdrobotOutput56();
	void ClickGxbtnLdrobotOutput57();
	void ClickGxbtnLdrobotOutput58();
	void ClickGxbtnLdrobotOutput59();
	void ClickGxbtnLdrobotOutput60();
	void ClickGxbtnLdrobotOutput61();
	void ClickGxbtnLdrobotOutput62();
	void ClickGxbtnLdrobotOutput63();
	void ClickGxbtnLdrobotOutput64();
	void ClickGxbtnLdrobotOutput65();
	void ClickGxbtnLdrobotOutput66();
	void ClickGxbtnLdrobotOutput67();
	void ClickGxbtnLdrobotOutput68();
	void ClickGxbtnLdrobotOutput69();
	void ClickGxbtnLdrobotOutput70();
	void ClickGxbtnLdrobotOutput71();
	void ClickGxbtnLdrobotOutput72();
	void ClickGxbtnLdrobotOutput73();
	void ClickGxbtnLdrobotOutput74();

	BOOL bStatus[100];
	void ClickGxbtnLdrobotGetApproach();
	void ClickGxbtnLdrobotGetWait();
	void ClickGxbtnLdrobotGetUp();
	void ClickGxbtnLdrobotGetDown();
	void ClickGxbtnLdrobotUpperApproach();
	void ClickGxbtnLdrobotUpperWait();
	void ClickGxbtnLdrobotUpperUp1();
	void ClickGxbtnLdrobotUpperDown1();
	void ClickGxbtnLdrobotUpperUp2();
	void ClickGxbtnLdrobotUpperDown2();
	void ClickGxbtnLdrobotLowerApproach();
	void ClickGxbtnLdrobotLowerWait();
	void ClickGxbtnLdrobotLowerUp();
	void ClickGxbtnLdrobotLowerDown();
	void ClickGxbtnLdrobotLowerUp2();
	void ClickGxbtnLdrobotLowerDown2();
	void ClickGxbtnLdrobotVacOn();
	void ClickGxbtnLdrobotVacOff();
	void ClickGxbtnLdrobotAlign();
	void ClickGxbtnLdrobotMove();

	BOOL ManualPosCheck(int Pos);
};


