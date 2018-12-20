#pragma once

//#include "File\GxLog.h"
//#include "BankGuild\MCMain.h"

#include "File\GxIni.h"
#include "BankGuild\MCDefine.h"

// CDlgLossTime_New 대화 상자입니다.

class CDlgLossTimeNew : public CDialog
{
	DECLARE_DYNAMIC(CDlgLossTimeNew)
	enum {
		eMAX_GroupCount1		= 8,			// 1st Group 수
		eMAX_GroupCount2		= 8,			// 2nd Group 수
		eMAX_GroupCount3		= 24,			// 3rd 항목 수
		eMAX_ItemCount			= 8,			// 한 화면에 표시할 수 있는 비가동 3rd 항목 수
	};
public:
	CDlgLossTimeNew(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgLossTimeNew();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_LOSSTIME_NEW };
	
	//CMCMain*			m_pMCMain;
	CCellInfo*		m_pCellInfo;				// Cell Data 저장 Class
	//CGxLog*			m_pLogGUI;					// 화면 조작 Log
	//CGxLog*			m_pLogSeq;					// 처리순서 Log

	// 언어 설정
	int				m_nLangIdx;			// 언어 설정 Index

	// Defect List의 Page 번호, 24개가 넘어가면 복수개의 Page가 지정된다.
	int				m_nSelect_1st;				// 선택된 1번째 Class의 순번 , 0 ~ 7
	int				m_nSelect_2nd;				// 선택된 2번째 Class의 순번 , 0 ~ 7
	int				m_nSelect_3rd;				// 선택된 3번째 Class의 순번 , 0 ~ 7

	int				m_n2ndCount;				// 지정 1st class에서 관리되는 2nd Class 수 최대 8개
	int				m_n3rdCount;				// 지정 2st class에서 관리되는 3rd Class 수 최대 24개

	int				m_nPageCount;				// 최대 Page 수, 2nd class가 바뀔때마다 class 3rd 수량에 따라서 갱신된다.
	int				m_nCurrentPage;			// 현재 보여주고 있는 page
	int				m_nStartIndex;				// 현재 page에서의 시작되는 3rd item의 순번, 이 순번 과 눌려진 버튼의 index로 조합하여 사용된다.
	int				m_nDisp3rdCount;			// 현재 page에서 표시해주는 3rdClass 항목 수

	CString			m_sSelectReason;			// 최종 선택된 중지 사유

	CString			m_sWorkUser;					// 작업자

	// 버튼 리소스 아이디를 관리할 배열
	UINT				m_idBtn_1st[eMAX_GroupCount1];		// 1st class 버튼 관리
	UINT				m_idBtn_2nd[eMAX_GroupCount2];		// 2nd class 버튼 관리
	UINT				m_idBtn_3rd[eMAX_ItemCount];			// 3rd class 버튼 관리

	void SelectGroup_1st(int nIdx);						// 지정 1st class를 선택하였다.
	void SelectGroup_2nd(int nIdx);						// 지정 2nd class를 선택하였다.
	void SelectGroup_3rd(int nIdx);						// 지정 3rd class를 선택하였다.
	void DispDefault();										// 기본적인 화면을 그려준다.
	void DispGroup_1st();
	void DispGroup_2nd();
	void DispGroup_3rd(int nPage);						// Page별 3rd class 항목 표시

	
	void DispCurrentTime();							// 현재 시각을 반영한다.
	void DispCurrentData();							// 현재 설정된 값을 반영해 준다.



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam);						// 화면내용을 갱신해준다

public:
	DECLARE_EVENTSINK_MAP()
	afx_msg void ClickGxbtn_Confirm();
	afx_msg void ClickGxbtn_Cancel();
	afx_msg void ClickGxbtn_Back();
	afx_msg void ClickGxbtn_Next();
	afx_msg void ClickGxbtn_Login();

	// 1st Class 선택
	afx_msg void ClickGxbtn_Select1stClass1();
	afx_msg void ClickGxbtn_Select1stClass2();
	afx_msg void ClickGxbtn_Select1stClass3();
	afx_msg void ClickGxbtn_Select1stClass4();
	afx_msg void ClickGxbtn_Select1stClass5();
	afx_msg void ClickGxbtn_Select1stClass6();
	afx_msg void ClickGxbtn_Select1stClass7();
	afx_msg void ClickGxbtn_Select1stClass8();

	afx_msg void ClickGxbtn_Select2ndClass1();
	afx_msg void ClickGxbtn_Select2ndClass2();
	afx_msg void ClickGxbtn_Select2ndClass3();
	afx_msg void ClickGxbtn_Select2ndClass4();
	afx_msg void ClickGxbtn_Select2ndClass5();
	afx_msg void ClickGxbtn_Select2ndClass6();
	afx_msg void ClickGxbtn_Select2ndClass7();
	afx_msg void ClickGxbtn_Select2ndClass8();

	afx_msg void ClickGxbtn_Select3rdClass1();
	afx_msg void ClickGxbtn_Select3rdClass2();
	afx_msg void ClickGxbtn_Select3rdClass3();
	afx_msg void ClickGxbtn_Select3rdClass4();
	afx_msg void ClickGxbtn_Select3rdClass5();
	afx_msg void ClickGxbtn_Select3rdClass6();
	afx_msg void ClickGxbtn_Select3rdClass7();
	afx_msg void ClickGxbtn_Select3rdClass8();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
