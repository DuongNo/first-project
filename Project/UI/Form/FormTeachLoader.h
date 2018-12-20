#pragma once



// CFormTeachLoader 폼 뷰입니다.

class CFormTeachLoader : public CFormView
{
	DECLARE_DYNCREATE(CFormTeachLoader)

protected:
public:
	CFormTeachLoader();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CFormTeachLoader();

	enum { IDD = IDD_FORM_TEACH_LOADER };
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
	AXIS_ID			m_CurAxis;
	TEACH_PARAM::ID	m_CurTeach;
	UINT			m_CurID;

	JIG_CH nCh;
	virtual void OnInitialUpdate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void UpdateLoaderState();
	void SetCurPageAxis(AXIS_ID axis, TEACH_PARAM::ID param, UINT nID);

	void ClickGxbtnLdShuttleShuttle1Load();
	void ClickGxbtnLdConvPitch();
	void ClickGxbtnLdRobotConvGetPos();
	void ClickGxbtnLdRobotShuttle1Ch1Put();
	void ClickGxbtnLdRobotShuttle1Ch2Put();
	void ClickGxbtnLdRobotShuttle2Ch1Put();
	void ClickGxbtnLdRobotShuttle2Put();
	void ClickGxbtnLdRobotVacOn();
	void ClickGxbtnLdRobotVacOff();
	void ClickGxbtnLdShuttleShuttle1Unload();
	void ClickGxbtnLdShuttleShuttle1VacCh1On();
	void ClickGxbtnLdShuttleShuttle1VacCh1Off();
	void ClickGxbtnLdShuttleShuttle1VacCh2On();
	void ClickGxbtnLdShuttleShuttle1VacCh2Off();
	void ClickGxbtnLdShuttleShuttle2Load();
	void ClickGxbtnLdShuttleShuttle2Unload();
	void ClickGxbtnLdShuttleShuttle2VacCh1On();
	void ClickGxbtnLdShuttleShuttle2VacCh1Off();
	void ClickGxbtnLdShuttleShuttle2VacCh2On();
	void ClickGxbtnLdShuttleShuttle2VacCh2Off();
	void ClickGxbtnLdRobotOrg();
	void ClickGxbtnEdgeAlignStart();
	void ClickGxstcEdgeAlignTeaching();

	int m_nASeq;
	int m_nBSeq;
	int m_nCSeq;
	int m_nDSeq;
	CStopWatch m_Timer;

	void LDRobotOrg();
	void LDRobotGet();
	void LDRobotUpperPut();
	void LDRobotLowerPut();
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
	enum 
	{
		seqD_idle = -1,
		seqD_1,
		seqD_2,
		seqD_3,
		seqD_4,
		seqD_5,
		seqD_6,
		seqD_7,
		seqD_8,
		seqD_9,
		seqD_End
	};
};


