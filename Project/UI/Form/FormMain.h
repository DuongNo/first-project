#pragma once

#include "resource.h"

// CFormMain 폼 뷰입니다.
//kjpark 20161021 Main Form Cell Status view 구현
#include <vector>
#include <map>
using namespace std; 

//kjpark 20161021 Main Form Cell Status view 구현
enum CELL_STATUS
{
	CELL_STATUS_NONE,
	CELL_STATUS_EXIST,
	CELL_STATUS_GOOD,	
	CELL_STATUS_NG,
	CELL_STATUS_SKIP,
	CELL_STATUS_ALIGN_NG
};
//kjpark 20161021 Main Form Cell Status view 구현
struct CELL_ELEMENT
{
	UINT nResourceID;
	CELL_STATUS bExistCell;
};



#define OK_IN_AGV 10
#define OK_OUT_AGV 11

class CFormMain : public CFormView
{
	DECLARE_DYNCREATE(CFormMain)

  CFormMain();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CFormMain();

public:
	enum { IDD = IDD_FORM_MAIN };
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
	afx_msg LRESULT OnCountUpdate(WPARAM wParam, LPARAM lParam);			
	afx_msg LRESULT OnTactChange(WPARAM wParam, LPARAM lParam);			
	afx_msg LRESULT OnMCRUpdate(WPARAM wParam, LPARAM lParam);		
	afx_msg LRESULT OnDCRUpdate(WPARAM wParam, LPARAM lParam);	
	afx_msg LRESULT OnCZONEUpdate(WPARAM wParam, LPARAM lParam);	
	afx_msg LRESULT OnIDLEChangeTimeUpdate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnIDLEChangeRUNTimeUpdate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAGVInStartTimeUpdate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAGVInEndTimeUpdate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAGVOutStartTimeUpdate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAGVOutEndTimeUpdate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpper_Eqp_Com_Update(WPARAM wParam, LPARAM lParam);

	//2018wjp
	afx_msg LRESULT OnUpdateLoadCellID(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateOneCellID(WPARAM wParam, LPARAM lParam);	
	afx_msg LRESULT OnRemoveALLCellID(WPARAM wParam, LPARAM lParam);	
	afx_msg LRESULT OnRemoveOneCellID(WPARAM wParam, LPARAM lParam);	

	afx_msg LRESULT OnDCRFail(WPARAM wParam, LPARAM lParam);	
	afx_msg LRESULT OnDCRCimBackFail(WPARAM wParam, LPARAM lParam);	


	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	//kjpark 20161021 Main Form Cell Status view 구현
	void SetStautsChange(BOOL bValue = FALSE);
	void GetCellStatus();

	//kjpark 20161021 Main Form Cell Status view 구현
	map<CELL_POS, CELL_ELEMENT> iMap;
	BOOL TrayStatus[13];
	void InitCellElement();

	void ChangeForm(int nIdx, BOOL bForce = FALSE);
	void ClickBtnModePM();

	void ClickGxbtnStart();
	void ClickGxbtnStop();
//	void ClickGxbtnTestIn();
//	void ClickGxbtnTestOut();
	void ClickGxstcCellmovePos1();
	void ClickGxstcCellmovePos2();
	void ClickGxstcCellmovePos4();
	void ClickGxstcCellmovePos3();
	void ClickGxstcCellmovePos6();
	void ClickGxstcCellmovePos5();
	void ClickGxstcCellmovePos8();
	void ClickGxstcCellmovePos7();
	void ClickGxstcCellmovePos10();
	void ClickGxstcCellmovePos9();
	void ClickGxstcCellmovePos12();
	void ClickGxstcCellmovePos11();
	void ClickGxstcCellmovePos14();
	void ClickGxstcCellmovePos13();
	void ClickGxstcCellmovePos16();
	void ClickGxstcCellmovePos15();
	void ClickGxstcCellmovePos17();
	void ClickGxstcCellmovePos18();
	void ClickGxstcCellmovePos20();
	void ClickGxstcCellmovePos19();
	void ClickGxstcCellmovePos22();
	void ClickGxstcCellmovePos21();
	void ClickGxstcCellmovePos24();
	void ClickGxstcCellmovePos23();
	void ClickGxstcCellmovePos25();
	void ClickGxbtnCountClear();

	//20170204 byskcho
	void DoorStatusDisplay() ;
	void OptionStatusDisplay();
	

	CEdit m_edtBoard;
	CEdit m_edtBoard2;
	BOOL DispCellTag(int nPosNode);
	void LibShowOut(CString str);
	void LibShowOut2(CString str);
	//void ClickGxbtnCountClear2();
	void ClickGxbtnModePm2();
//	void ClickGxbtnTestIn2();

	void ClickGxstcSendAble();

	// 2017.2.5 bgkim
	// Melsec 관련 버튼 토글 변수
	BOOL m_bAble;
	BOOL m_bStart;
	BOOL m_bComp;
	BOOL m_bPause;
	void ClickGxstcSendStart();
	void ClickGxstcSendComplete();
	void ClickGxstcCellmovePos33();
	void ClickGxstcCellmovePos31();
	void ClickGxstcCellmovePos32();
	void ClickGxstcCellmovePos36();
	void ClickGxstcCellmovePos38();
	void ClickGxstcCellmovePos37();
	void ClickGxstcCellmovePos29();
	void ClickGxstcCellmovePos30();
	void ClickGxstcCellmovePos28();
	void ClickGxstcCellmovePos27();
	void ClickGxstcCellmovePos26();
	void ClickGxstcCellmovePos34();
	void ClickGxstcCellmovePos35();
	void ClickGxstcCh1McrData();
	
	void ClickGxlblReportAutocontact();
	void ClickGxstcLoaderDoor5();
	void ClickGxstcMainOlbskip();

	
	void ClickGxlblMainLogclear();
	void DisplayMessage(CELL_POS nPos);
	void DispCellID(CELL_POS nPosNode);
	void DispVacuumState();
	void ClickGxstcAjigSkip();
	void ClickGxstcBjigSkip();
	void ClickGxstcCjigSkip();
	void ClickGxstcDjigSkip();

	//2017-11-15,SKCHO
	BOOL CheckCellDataAndVac(INPUT_ID nInput1,INPUT_ID nInput2 );
	void ClickGxlblMainRetryinfo();

	void LDRobotOrg();
	int m_nLDRobotOrgSeq;

	//2017-03-14, NAM, Set Date Time 1 and 2  to return the Date to get Data
	void SetDateTime1(int iValue = 0,BOOL btoday=FALSE);
	void SetDateTime2(int iValue = 0,BOOL btoday=FALSE);
	// 2017-03-14,NAM  Return the Value of Contact Percentage each day of Week and Month  
	double UPdateWeekContactValue();
	double UPdateMonthContactValue();
	// 2017-03-15, NAM, Update the Percentage of Value of Day, Week, Month 
	void UPdateDayContactValue();
	void UPdatePreContactValue();
	void UpdatePrehourContact();
	void UPdateWeekContact();
	void UPdateMonthContact();
	//2018-04-06,NAM, Define when Update Product and Contact Percentage  
	BOOL m_Report;

	//2017-03-15, NAM , to return the day of week 
	int Todayofweek(int day, int month, int year);

	enum 
	{
		seqLdRobot_idle = -1,
		seqLdRobot_1,
		seqLdRobot_2,
		seqLdRobot_3,
		seqLdRobot_4,
		seqLdRobot_5,
		seqLdRobot_6,
		seqLdRobot_7,
		seqLdRobot_8,
		seqLdRobot_9,
		seqLdRobot_10,
		seqLdRobot_End
	};

	CStopWatch m_Timer;
	void ClickGxstcGoodtray();
	void ClickGxstcNgtray();

	//2018-03-16,MGYUN, 모터 서보 체크 함수
	BOOL MotorServoCheck();

	//2018-05-10,MGYUN, 하이브리드 관련 작업
	void ClickGxstcHybridRobotReady();
	void ClickGxstcHybridRobotRun();
	void ClickGxstcHybridRobotComplete();
	void DispHybridIOState();

	//2018wjp
	afx_msg void OnBnClickedBtnGetcellid();
	afx_msg void OnBnClickedBtnCellid11();
	afx_msg void OnBnClickedBtnCellid21();
	afx_msg void OnBnClickedBtnCellid12();
	afx_msg void OnBnClickedBtnCellid22();
	afx_msg void OnBnClickedBtnCellid13();
	afx_msg void OnBnClickedBtnCellid23();
	afx_msg void OnBnClickedBtnCellid14();
	afx_msg void OnBnClickedBtnCellid24();
	afx_msg void OnBnClickedButton4();
};


