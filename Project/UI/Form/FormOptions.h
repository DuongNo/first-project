#pragma once
#include "resource.h"


// CFormOptions 폼 뷰입니다.

class CFormOptions : public CFormView
{
	DECLARE_DYNCREATE(CFormOptions)

public:
	CFormOptions();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CFormOptions();

public:
	enum { IDD = IDD_FORM_CONFIG_OPTION };
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
	DECLARE_EVENTSINK_MAP()

public:	
	//void ClickBtnOption(UINT nID);
	BOOL m_lot_end;
	BOOL m_dry_run;
	BOOL m_UpperIF;
	BOOL m_Uppermcr_read;
	BOOL m_Lowermcr_read;
	BOOL m_pre_align;
	BOOL m_Vi_align;
	BOOL m_Edge_align;
	BOOL m_Login_Request;
	BOOL m_Manual_Receive;
	BOOL m_RCMD;
	BOOL m_VIInsp;
	BOOL m_DCRUse;
	BOOL m_TrayAlignUse;
	BOOL m_ContactUse;
	BOOL m_JudgeOKUse;

	BOOL m_CellInfoSkip;

	//2017-08-14 KAKAKA
	BOOL m_NGINLIFTSEN1;
	BOOL m_NGINLIFTSEN2;
	BOOL m_NGOUTLIFTSEN1;
	BOOL m_NGOUTLIFTSEN2;

	// 2017.2.8 bgkim
	double m_dJobEndTime;
	BOOL m_bUseJobEnd;

	double m_dVIGrabEndWaitTime;
	double m_dVIClassEndWaitTime;
	int m_nMaxGoodTrayCount;
	int m_nMaxNGTrayCount;

	int m_nVisionContinuesNGCnt;

	double m_dRunDownWaitTime;

	double m_dMCRDelayTime;

	BOOL m_bUseAskip;
	BOOL m_bUseBskip;
	BOOL m_bUseCskip;
	BOOL m_bUseDskip;
	BOOL m_bUseACh1skip;
	BOOL m_bUseBCh1skip;
	BOOL m_bUseCCh1skip;
	BOOL m_bUseDCh1skip;
	BOOL m_bUseACh2skip;
	BOOL m_bUseBCh2skip;
	BOOL m_bUseCCh2skip;
	BOOL m_bUseDCh2skip;

	BOOL m_bUseContactRetry;

	BOOL m_bUseRepeatInsp;

	BOOL m_bUseConvPitchIn;

	BOOL m_bUseMCRGrab;

	CString m_strRejectCode;

	//2017-07-20,SKCHO
	BOOL m_bDivision;

	//2017-07-10,SKCHO
	int m_nTrayIDCount;
	int m_nAGVDelayTime;

	int m_nT5RestartTime;
	//2017-12-23,SKCHO
	int m_nPAOrgTime;

	//2018-05-11,NAM
	int m_nContactDelay;

	//2018-06-29,NAM
	int m_nConveyorDelay;

	//2018-06-29,NAM
	BOOL  m_bUseConveyorMode;

	//2018wjp
	BOOL    m_bUseBuffer;
	int     m_nBufferUseWaitTime;
	double  m_nBufferUseTrayDownZ;

	double m_dGoodTRRetryLength;
	double m_dNGTRRetryLength;

	double m_dLDTRBlowerOffTime;

	BOOL m_bSkipLDConvSen3;

	BOOL m_first_load;
	void LoadOption();
	BOOL SaveOptionCompare();//변경 여부를 확인하기 위해 초기 설정값 저장
	void ClickGxbtnOptionSave();
	void ClickGxbtnOptionLoad();
	void ClickGxbtnOptionLotendOnoff();
	void ClickGxbtnOptionDryrunOnoff();
	void ClickGxbtnOptionUppermcrreadOnoff();
	void ClickGxbtnOptionLowermcrreadOnoff();
	void ClickGxbtnOptionPrealignOnoff();
	void ClickGxbtnOptionVialignOnoff();
	void ClickGxbtnOptionEdgealignOnoff();
	void ClickGxbtnOptionUpperifOnoff();
	void ClickGxbtnOptionLoginRequestOnoff();
	void ClickGxbtnOptionManualReceive();
	void ClickGxbtnOptionRCMD();
	void ClickGxbtnOptionViinspOnoff();
	void ClickGxbtnOptionDcrUse();
	void ClickGxbtnOptionTrayAlignUse();
	void ClickGxbtnOptionContactUse();
	void ClickGxbtnOptionAskipOnoff();
	void ClickGxbtnOptionBskipOnoff();
	void ClickGxbtnOptionCskipOnoff();
	void ClickGxbtnOptionDskipOnoff();
	void ClickGxbtnOptionCellinfoskipOnoff();
	void ClickGxbtnJobEndTime();
	void ClickGxbtnUseJobEnd();
	void ClickGxbtnOptionJudgeokUse();
	void ClickGxbtnViGrabEndWaitTime();
	void ClickGxbtnViClassEndWaitTime();
	void ClickGxbtnGoodTrayMaxCount();
	void ClickGxbtnNgTrayMaxCount();
	void ClickGxbtnOptionAskipch1Onoff();
	void ClickGxbtnOptionAskipch2Onoff();
	void ClickGxbtnOptionBskipch1Onoff();
	void ClickGxbtnOptionBskipch2Onoff();
	void ClickGxbtnOptionCskipch1Onoff();
	void ClickGxbtnOptionCskipch2Onoff();
	void ClickGxbtnOptionDskipch1Onoff();
	void ClickGxbtnOptionDskipch2Onoff();
	void ClickGxbtnOptionContactRetry();
	void ClickGxbtnRunDownWaitTime();
	void ClickGxbtnOptionCarrierReport();
	void ClickGxbtnOptionRepeatInsp();
	void ClickGxbtnVisionContinuesNgCount();
	void ClickGxbtnMcrTime();
	void ClickGxbtnOptionConvPitchIn();
	void ClickGxbtnOptionMcrgrabUse();
	void ClickGxbtnTrayidCount();	
	void ClickGxbtnMcrCompchk();
	void ClickGxbtnOptionDivision();
	void ClickGxbtnOptionNgin1Onoff();
	void ClickGxbtnOptionNgin2Onoff();
	void ClickGxbtnOptionNgout1Onoff();
	void ClickGxbtnOptionNgout2Onoff();
	void ClickGxbtnAgvDelaytime();
	void ClickGxbtnOptionLdconvsen3skip();
	void ClickGxbtnLdtrblowerWaitTime();	
	void ClickGxbtnOptionUldtrmode();
	void ClickGxbtnT5RestartTime();
	void ClickGxbtnOptionPorttrmode();
	void ClickGxbtnOptionNgtrlength();
	void ClickGxbtnOptionLdrobotRetry();
	void ClickGxbtnOptionTrayvi();
	void ClickGxbtnOptionPatrRetry();
	void ClickGxbtnOptionJigblow();
	void ClickGxbtnPaOrgTime();
	void ClickGxbtnOptionFPCBSen();
	void ClickGxbtnOptionGoodInFinalSen();
	void ClickGxbtnPaOrgTime2();
	void ClickGxbtnContactDelayTime();
	void ClickGxbtnOptionConveyormode();
	void ClickGxbtnConveyorDelayTime();
	//2018wjp
	void ClickSetUseBuffer();
	void ClickSetUseBufferWaitTime();
	void ClickUseBufferTrayZUpDis();
	void ClickSetUseConveyorTestModel();
};


