// FormOptions.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CheetahApp.h"
#include "FormOptions.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "UI\GausGUI\GxButtonEx.h"
#include "UI\GausGUI\GxMsgBox.h"
#include "UI\GausGUI\GxNumPadEx.h"

// CFormOptions

IMPLEMENT_DYNCREATE(CFormOptions, CFormView)

CFormOptions::CFormOptions()
	: CFormView(CFormOptions::IDD)
{
	m_first_load=FALSE;
}

CFormOptions::~CFormOptions()
{
}

void CFormOptions::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFormOptions, CFormView)
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
END_MESSAGE_MAP()


BEGIN_EVENTSINK_MAP(CFormOptions, CFormView)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_SAVE, DISPID_CLICK, CFormOptions::ClickGxbtnOptionSave, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_LOAD, DISPID_CLICK, CFormOptions::ClickGxbtnOptionLoad, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_LOTEND_ONOFF, DISPID_CLICK, CFormOptions::ClickGxbtnOptionLotendOnoff, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_DRYRUN_ONOFF, DISPID_CLICK, CFormOptions::ClickGxbtnOptionDryrunOnoff, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_PREALIGN_ONOFF, DISPID_CLICK, CFormOptions::ClickGxbtnOptionPrealignOnoff, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_EDGEALIGN_ONOFF, DISPID_CLICK, CFormOptions::ClickGxbtnOptionEdgealignOnoff, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_UPPERMCRREAD_ONOFF, DISPID_CLICK, CFormOptions::ClickGxbtnOptionUppermcrreadOnoff, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_LOWERMCRREAD_ONOFF, DISPID_CLICK, CFormOptions::ClickGxbtnOptionLowermcrreadOnoff, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_VIALIGN_ONOFF, DISPID_CLICK, CFormOptions::ClickGxbtnOptionVialignOnoff, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_UPPERIF_ONOFF, DISPID_CLICK, CFormOptions::ClickGxbtnOptionUpperifOnoff, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_LOGIN_REQUEST_ONOFF, DISPID_CLICK, CFormOptions::ClickGxbtnOptionLoginRequestOnoff, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_MANUAL_RECEIVE, DISPID_CLICK, CFormOptions::ClickGxbtnOptionManualReceive, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_RCMD, DISPID_CLICK, CFormOptions::ClickGxbtnOptionRCMD, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_VIINSP_ONOFF, DISPID_CLICK, CFormOptions::ClickGxbtnOptionViinspOnoff, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_DCR_USE, DISPID_CLICK, CFormOptions::ClickGxbtnOptionDcrUse, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_TRAY_ALIGN_USE, DISPID_CLICK, CFormOptions::ClickGxbtnOptionTrayAlignUse, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_CONTACT_USE, DISPID_CLICK, CFormOptions::ClickGxbtnOptionContactUse, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_ASKIP_ONOFF, DISPID_CLICK, CFormOptions::ClickGxbtnOptionAskipOnoff, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_BSKIP_ONOFF, DISPID_CLICK, CFormOptions::ClickGxbtnOptionBskipOnoff, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_CSKIP_ONOFF, DISPID_CLICK, CFormOptions::ClickGxbtnOptionCskipOnoff, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_DSKIP_ONOFF, DISPID_CLICK, CFormOptions::ClickGxbtnOptionDskipOnoff, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_CELLINFOSKIP_ONOFF, DISPID_CLICK, CFormOptions::ClickGxbtnOptionCellinfoskipOnoff, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_JOB_END_TIME, DISPID_CLICK, CFormOptions::ClickGxbtnJobEndTime, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_USE_JOB_END, DISPID_CLICK, CFormOptions::ClickGxbtnUseJobEnd, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_JUDGEOK_USE, DISPID_CLICK, CFormOptions::ClickGxbtnOptionJudgeokUse, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_VI_GRAB_END_WAIT_TIME, DISPID_CLICK, CFormOptions::ClickGxbtnViGrabEndWaitTime, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_VI_CLASS_END_WAIT_TIME, DISPID_CLICK, CFormOptions::ClickGxbtnViClassEndWaitTime, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_GOOD_TRAY_MAX_COUNT, DISPID_CLICK, CFormOptions::ClickGxbtnGoodTrayMaxCount, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_NG_TRAY_MAX_COUNT, DISPID_CLICK, CFormOptions::ClickGxbtnNgTrayMaxCount, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_ASKIPCH1_ONOFF, DISPID_CLICK, CFormOptions::ClickGxbtnOptionAskipch1Onoff, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_ASKIPCH2_ONOFF, DISPID_CLICK, CFormOptions::ClickGxbtnOptionAskipch2Onoff, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_BSKIPCH1_ONOFF, DISPID_CLICK, CFormOptions::ClickGxbtnOptionBskipch1Onoff, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_BSKIPCH2_ONOFF, DISPID_CLICK, CFormOptions::ClickGxbtnOptionBskipch2Onoff, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_CSKIPCH1_ONOFF, DISPID_CLICK, CFormOptions::ClickGxbtnOptionCskipch1Onoff, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_CSKIPCH2_ONOFF, DISPID_CLICK, CFormOptions::ClickGxbtnOptionCskipch2Onoff, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_DSKIPCH1_ONOFF, DISPID_CLICK, CFormOptions::ClickGxbtnOptionDskipch1Onoff, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_DSKIPCH2_ONOFF, DISPID_CLICK, CFormOptions::ClickGxbtnOptionDskipch2Onoff, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_CONTACT_RETRY, DISPID_CLICK, CFormOptions::ClickGxbtnOptionContactRetry, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_RUN_DOWN_WAIT_TIME, DISPID_CLICK, CFormOptions::ClickGxbtnRunDownWaitTime, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_CARRIER_REPORT, DISPID_CLICK, CFormOptions::ClickGxbtnOptionCarrierReport, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_REPEAT_INSP, DISPID_CLICK, CFormOptions::ClickGxbtnOptionRepeatInsp, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_VISION_CONTINUES_NG_COUNT, DISPID_CLICK, CFormOptions::ClickGxbtnVisionContinuesNgCount, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_MCR_TIME, DISPID_CLICK, CFormOptions::ClickGxbtnMcrTime, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_CONV_PITCH_IN, DISPID_CLICK, CFormOptions::ClickGxbtnOptionConvPitchIn, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_MCRGRAB_USE, DISPID_CLICK, CFormOptions::ClickGxbtnOptionMcrgrabUse, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_TRAYID_COUNT, DISPID_CLICK, CFormOptions::ClickGxbtnTrayidCount, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_MCR_COMPCHK, DISPID_CLICK, CFormOptions::ClickGxbtnMcrCompchk, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_DIVISION, DISPID_CLICK, CFormOptions::ClickGxbtnOptionDivision, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_NGIN1_ONOFF, DISPID_CLICK, CFormOptions::ClickGxbtnOptionNgin1Onoff, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_NGIN2_ONOFF, DISPID_CLICK, CFormOptions::ClickGxbtnOptionNgin2Onoff, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_NGOUT1_ONOFF, DISPID_CLICK, CFormOptions::ClickGxbtnOptionNgout1Onoff, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_NGOUT2_ONOFF, DISPID_CLICK, CFormOptions::ClickGxbtnOptionNgout2Onoff, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_AGV_DELAYTIME, DISPID_CLICK, CFormOptions::ClickGxbtnAgvDelaytime, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_LDCONVSEN3SKIP, DISPID_CLICK, CFormOptions::ClickGxbtnOptionLdconvsen3skip, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_LDTRBLOWER_WAIT_TIME, DISPID_CLICK, CFormOptions::ClickGxbtnLdtrblowerWaitTime, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_ULDTRMODE, DISPID_CLICK, CFormOptions::ClickGxbtnOptionUldtrmode, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_T5_RESTART_TIME, DISPID_CLICK, CFormOptions::ClickGxbtnT5RestartTime, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_PORTTRMODE, DISPID_CLICK, CFormOptions::ClickGxbtnOptionPorttrmode, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_NGTRLENGTH, DISPID_CLICK, CFormOptions::ClickGxbtnOptionNgtrlength, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_LDROBOT_RETRY, DISPID_CLICK, CFormOptions::ClickGxbtnOptionLdrobotRetry, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_TRAYVI, DISPID_CLICK, CFormOptions::ClickGxbtnOptionTrayvi, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_PATR_RETRY, DISPID_CLICK, CFormOptions::ClickGxbtnOptionPatrRetry, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_JIGBLOW, DISPID_CLICK, CFormOptions::ClickGxbtnOptionJigblow, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_PA_ORG_TIME, DISPID_CLICK, CFormOptions::ClickGxbtnPaOrgTime, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_FPCBSEN, DISPID_CLICK, CFormOptions::ClickGxbtnOptionFPCBSen, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_LASTTRAYSEN, DISPID_CLICK, CFormOptions::ClickGxbtnOptionGoodInFinalSen, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_CONTACT_DELAY_TIME, DISPID_CLICK, CFormOptions::ClickGxbtnContactDelayTime, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_CONVEYORMODE, DISPID_CLICK, CFormOptions::ClickGxbtnOptionConveyormode, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_CONVEYOR_DELAY_TIME, DISPID_CLICK, CFormOptions::ClickGxbtnConveyorDelayTime, VTS_NONE)
	//2018wjp
	ON_EVENT(CFormOptions, IDC_MODEL_BUFFER_USE, DISPID_CLICK, CFormOptions::ClickSetUseBuffer, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_MODEL_BUFFER_WAIT_TIME, DISPID_CLICK, CFormOptions::ClickSetUseBufferWaitTime, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_MODEL_BUFFER_Z_DIS, DISPID_CLICK, CFormOptions::ClickUseBufferTrayZUpDis, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_CONVEYOR_TEST_MODEL_SET, DISPID_CLICK, CFormOptions::ClickSetUseConveyorTestModel, VTS_NONE)
END_EVENTSINK_MAP()


// CFormOptions 진단입니다.

#ifdef _DEBUG
void CFormOptions::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormOptions::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormOptions 메시지 처리기입니다.
LRESULT CFormOptions::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	BOOL bFlag = (BOOL)wParam;
/*
	if(SaveOptionCompare() && m_first_load)
		ClickGxbtnOptionSave();
	LoadOption();
	m_first_load=TRUE;
*/
	LoadOption();

	//2017-07-15,SKCHO, LOWER MCR에서 실패하여 UPPER MCR에서 읽기 전에 A ZONE에 있는 CELL 정보를 지우면
	//theProcBank.m_bUpperMCRCompChk = FALSE상태가 되어 LOADER TR이 GET 위치로 이동하지 않는다
	CGxButtonEx* pUpperCompChk = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_MCR_COMPCHK);
	if(theProcBank.m_bUpperMCRCompChk == FALSE)
	{		
		pUpperCompChk->EnableWindow(TRUE);
		pUpperCompChk->SetBackColor(RGB(255,0,0));
	}
	else
	{
		pUpperCompChk->EnableWindow(FALSE);
	}
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{
		
	
	}
	else
	{

	}

	return 0;
}

void CFormOptions::LoadOption()
{
	CString strtemp;

	CGxButtonEx* pLotEndOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_LOTEND_ONOFF);
	CGxButtonEx* pDryRunOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_DRYRUN_ONOFF);
	CGxButtonEx* pUpperIFOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_UPPERIF_ONOFF);
	CGxButtonEx* pUpperMCRReadOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_UPPERMCRREAD_ONOFF);
	CGxButtonEx* pLowerMCRReadOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_LOWERMCRREAD_ONOFF);
	CGxButtonEx* pPreAlignOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_PREALIGN_ONOFF);
	CGxButtonEx* pVIAlignOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_VIALIGN_ONOFF);
	CGxButtonEx* pEdgeAlingOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_EDGEALIGN_ONOFF);
	CGxButtonEx* pLoginRequestOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_LOGIN_REQUEST_ONOFF);
	CGxButtonEx* pManualReceive = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_MANUAL_RECEIVE);
	CGxButtonEx* pRCMD = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_RCMD);
	CGxButtonEx* pVIInsp = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_VIINSP_ONOFF);
	//2017-07-06,SKCHO,  고객사 요청으로 DCR 옵션 기능 삭제
	//CGxButtonEx* pDCROnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_DCR_USE);
	CGxButtonEx* pTrayAlignOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_TRAY_ALIGN_USE);
	CGxButtonEx* pContactOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_CONTACT_USE);
	CGxButtonEx* pJudgeOKOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_JUDGEOK_USE);
	//20170201 byskcho
	CGxButtonEx* pCellInfoSkip = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_CELLINFOSKIP_ONOFF);
	// 2017.2.8 bgkim
	CGxButtonEx* pJobEndTime = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_JOB_END_TIME);
	CGxButtonEx* pViGrabEndWaitTime = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_VI_GRAB_END_WAIT_TIME);
	CGxButtonEx* pViClassEndWaitTime = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_VI_CLASS_END_WAIT_TIME);
	CGxButtonEx* pGoodTrayMaxCount = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_GOOD_TRAY_MAX_COUNT);
	CGxButtonEx* pNGTrayMaxCount = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_NG_TRAY_MAX_COUNT);
	CGxButtonEx* pUseJobEnd = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_USE_JOB_END);

	//2017-03-23,skcho, IDLE 상태변환 시간 설정 추가
	CGxButtonEx* pRunDownWaitTime = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_RUN_DOWN_WAIT_TIME);

	//Lower MCR Delay Time 설정 추가 2017/06/09 HSK.
	CGxButtonEx* pMCRDelayTime = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_MCR_TIME);

	//화상 검사 연속 NG 체크
	CGxButtonEx* pVisionContinuesNGCnt = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_VISION_CONTINUES_NG_COUNT);

	// Zone , Ch Skip
	CGxButtonEx* pASkipOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_ASKIP_ONOFF);
	CGxButtonEx* pBSkipOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_BSKIP_ONOFF);
	CGxButtonEx* pCSkipOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_CSKIP_ONOFF);
	CGxButtonEx* pDSkipOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_DSKIP_ONOFF);

	CGxButtonEx* pACh1SkipOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_ASKIPCH1_ONOFF);
	CGxButtonEx* pBCh1SkipOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_BSKIPCH1_ONOFF);
	CGxButtonEx* pCCh1SkipOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_CSKIPCH1_ONOFF);
	CGxButtonEx* pDCh1SkipOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_DSKIPCH1_ONOFF);
	CGxButtonEx* pACh2SkipOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_ASKIPCH2_ONOFF);
	CGxButtonEx* pBCh2SkipOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_BSKIPCH2_ONOFF);
	CGxButtonEx* pCCh2SkipOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_CSKIPCH2_ONOFF);
	CGxButtonEx* pDCh2SkipOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_DSKIPCH2_ONOFF);

	CGxButtonEx* pContactRetry = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_CONTACT_RETRY);

	CGxButtonEx* pCarrierReportSkip = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_CARRIER_REPORT);  //2017-04-17,skcho

	CGxButtonEx* pRepeatInsp = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_REPEAT_INSP);

	CGxButtonEx* pConvPitchIn = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_CONV_PITCH_IN);

	CGxButtonEx* pUseMCRGrab = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_MCRGRAB_USE);

	CGxButtonEx* pRejectCode = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_REJECT_CODE);
	
	//2017-07-10,SKCHO, TRAYID 자릿수
	CGxButtonEx* pTrayIDCount = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_TRAYID_COUNT);

	//2017-07-20,SKCHO
	CGxButtonEx* pUseDivision = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_DIVISION);
	
	//2017-08-30,SKCHO
	CGxButtonEx* pAGVDelayTime = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_AGV_DELAYTIME);
	
	CGxButtonEx* pLDConvSen3Skip = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_LDCONVSEN3SKIP);

	//2017-09-02,SKCHO
	CGxButtonEx* pLDTRBlowerOff =(CGxButtonEx*)GetDlgItem(IDC_GXBTN_LDTRBLOWER_WAIT_TIME);
	
	//2017-09-19,SKCHO, ULD TR 가감속 모드 추가
	CGxButtonEx* pUseULDTRMode = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_ULDTRMODE);

	CGxButtonEx* pLDRobot = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_LDROBOT_RETRY);

	CGxButtonEx* pTrayVI = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_TRAYVI);
	
	//2017-09-21,SKCHO
	CGxButtonEx* pT5RestartTime = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_T5_RESTART_TIME);
	//2017-09-21,SKCHO
	CGxButtonEx* pGOODTRMode = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_GOODTRLENGTH);
	CGxButtonEx* pNGTRMode = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_NGTRLENGTH);
	
	//2017-08-14,KAKAKA
	CGxButtonEx* pUseNGIN1 = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_NGIN1_ONOFF);
	CGxButtonEx* pUseNGIN2 = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_NGIN2_ONOFF);
	CGxButtonEx* pUseNGOUT1 = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_NGOUT1_ONOFF);
	CGxButtonEx* pUseNGOUT2 = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_NGOUT2_ONOFF);

	CGxButtonEx* pPATR = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_PATR_RETRY);

	CGxButtonEx* pJigBlow= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_JIGBLOW);
	CGxButtonEx* pPaOrgTime = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_PA_ORG_TIME);

	//2018-05-11,NAM
	CGxButtonEx* pContactButton = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_CONTACT_DELAY_TIME);

	//2018-06-29,NAM
	CGxButtonEx* pConveyorDelay = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_CONVEYOR_DELAY_TIME);

	//2018-06-29,NAM
	CGxButtonEx* pConveyorMode= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_CONVEYORMODE);

	//2018-01-20,GHLEE
	CGxButtonEx* pFPCBSen= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_FPCBSEN);
	//2018-03-15,GHLEE
	CGxButtonEx* pGoodInFinalSen= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_LASTTRAYSEN);

	//2018wjp
	CGxButtonEx* pBufferModelUse      = (CGxButtonEx*)GetDlgItem(IDC_MODEL_BUFFER_USE);
	CGxButtonEx* pBufferModelWaitTime = (CGxButtonEx*)GetDlgItem(IDC_MODEL_BUFFER_WAIT_TIME);
	CGxButtonEx* pBufferModelTrayZDis = (CGxButtonEx*)GetDlgItem(IDC_MODEL_BUFFER_Z_DIS);
	CGxButtonEx* pUseConveyorTestModel = (CGxButtonEx*)GetDlgItem(IDC_CONVEYOR_TEST_MODEL_SET);

	strtemp.Format(_T("%.1f"), theConfigBank.m_Option.m_dJobEndTime);
	pJobEndTime->SetCaption(strtemp);

	strtemp.Format(_T("%.1f"), theConfigBank.m_Option.m_dGrabEndWaitTime);
	pViGrabEndWaitTime->SetCaption(strtemp);

	strtemp.Format(_T("%.1f"), theConfigBank.m_Option.m_dClassEndWaitTime);
	pViClassEndWaitTime->SetCaption(strtemp);

	strtemp.Format(_T("%d"), theConfigBank.m_Option.m_nMaxGoodTrayCount);
	pGoodTrayMaxCount->SetCaption(strtemp);

	strtemp.Format(_T("%d"), theConfigBank.m_Option.m_nMaxNGTrayCount);
	pNGTrayMaxCount->SetCaption(strtemp);

	strtemp.Format(_T("%.1f"), theConfigBank.m_Option.m_dRunDownWaitTime);  //2017-03-23,skcho
	pRunDownWaitTime->SetCaption(strtemp);
	
	strtemp.Format(_T("%.1f"), theConfigBank.m_Option.m_dMCRDelayTime);  
	pMCRDelayTime->SetCaption(strtemp);

	strtemp.Format(_T("%d"), theConfigBank.m_Option.m_nVisionContinuesNGCnt);
	pVisionContinuesNGCnt->SetCaption(strtemp);

	pRejectCode->SetCaption(theConfigBank.m_Option.m_strRejectCode);

	//2017-07-10,SKCHO, TRAYID 자릿수
	strtemp.Format(_T("%d"), theConfigBank.m_Option.m_nTrayIDCount);
	pTrayIDCount->SetCaption(strtemp);

	strtemp.Format(_T("%d"), theConfigBank.m_Option.m_nAGVDelayTime);
	pAGVDelayTime->SetCaption(strtemp);

	strtemp.Format(_T("%.1f"), theConfigBank.m_Option.m_dLDTRBlowerOffTime);
	pLDTRBlowerOff->SetCaption(strtemp);

	//2017-09-19,SKCHO
	CGxButtonEx* pCellInfoWait =(CGxButtonEx*)GetDlgItem(IDC_GXBTN_CELLINFO_WAIT_TIME);
	strtemp.Format(_T("%.1f"), theConfigBank.m_Option.m_dCellInfoWaitTime);
	pCellInfoWait->SetCaption(strtemp);

	//2017-09-19,SKCHO
	CGxButtonEx* pCarrierInfoWait =(CGxButtonEx*)GetDlgItem(IDC_GXBTN_CARRIERINFO_WAIT_TIME);
	strtemp.Format(_T("%.1f"), theConfigBank.m_Option.m_dCarrierInfoWaitTime);
	pCarrierInfoWait->SetCaption(strtemp);

	strtemp.Format(_T("%d"), theConfigBank.m_Option.m_nT5RestartTime);
	pT5RestartTime->SetCaption(strtemp);

	strtemp.Format(_T("%.1f"), theConfigBank.m_Option.m_dGoodTRRetryLength);
	if(theConfigBank.m_Option.m_dGoodTRRetryLength == 0)
	{
		pGOODTRMode->SetCaption(_T("OFF"));
		pGOODTRMode->SetBackColor(GXCOLOR_OFF);
	}
	else
	{
		pGOODTRMode->SetCaption(strtemp);
		pGOODTRMode->SetBackColor(GXCOLOR_ON);
	}
	

	strtemp.Format(_T("%.1f"), theConfigBank.m_Option.m_dNGTRRetryLength);
	if(theConfigBank.m_Option.m_dNGTRRetryLength == 0)
	{
		pNGTRMode->SetCaption(_T("OFF"));
		pNGTRMode->SetBackColor(GXCOLOR_OFF);
	}
	else
	{
		pNGTRMode->SetCaption(strtemp);
		pNGTRMode->SetBackColor(GXCOLOR_ON);
	}
	//2017-12-23,SKCHO
	strtemp.Format(_T("%d"), theConfigBank.m_Option.m_nPaOrgTime);
	pPaOrgTime->SetCaption(strtemp);

	//2018-05-11,NAM
	strtemp.Format(_T("%d"), theConfigBank.m_Option.m_nContactDelay);
	pContactButton->SetCaption(strtemp);


	//2018-06-29,NAM
	strtemp.Format(_T("%d"), theConfigBank.m_Option.m_nConveyorDelay);
	pConveyorDelay->SetCaption(strtemp);


	theConfigBank.m_Option.m_bUseLotEnd?pLotEndOnOff->SetCaption(_T("ON")):pLotEndOnOff->SetCaption(_T("OFF"));
	pLotEndOnOff->GetCaption()==_T("ON")?pLotEndOnOff->SetBackColor(GXCOLOR_ON):pLotEndOnOff->SetBackColor(GXCOLOR_OFF);

	theConfigBank.m_Option.m_bUseDryRun?pDryRunOnOff->SetCaption(_T("ON")):pDryRunOnOff->SetCaption(_T("OFF"));
	pDryRunOnOff->GetCaption()==_T("ON")?pDryRunOnOff->SetBackColor(GXCOLOR_ON):pDryRunOnOff->SetBackColor(GXCOLOR_OFF);

	theConfigBank.m_Option.m_bUseUpperMCRRead?pUpperMCRReadOnOff->SetCaption(_T("ON")):pUpperMCRReadOnOff->SetCaption(_T("OFF"));
	pUpperMCRReadOnOff->GetCaption()==_T("ON")?pUpperMCRReadOnOff->SetBackColor(GXCOLOR_ON):pUpperMCRReadOnOff->SetBackColor(GXCOLOR_OFF);

	theConfigBank.m_Option.m_bUseLowerMCRRead?pLowerMCRReadOnOff->SetCaption(_T("ON")):pLowerMCRReadOnOff->SetCaption(_T("OFF"));
	pLowerMCRReadOnOff->GetCaption()==_T("ON")?pLowerMCRReadOnOff->SetBackColor(GXCOLOR_ON):pLowerMCRReadOnOff->SetBackColor(GXCOLOR_OFF);

	theConfigBank.m_Option.m_bUsePreAlign?pPreAlignOnOff->SetCaption(_T("ON")):pPreAlignOnOff->SetCaption(_T("OFF"));
	pPreAlignOnOff->GetCaption()==_T("ON")?pPreAlignOnOff->SetBackColor(GXCOLOR_ON):pPreAlignOnOff->SetBackColor(GXCOLOR_OFF);

	theConfigBank.m_Option.m_bUseVIAlign?pVIAlignOnOff->SetCaption(_T("ON")):pVIAlignOnOff->SetCaption(_T("OFF"));
	pVIAlignOnOff->GetCaption()==_T("ON")?pVIAlignOnOff->SetBackColor(GXCOLOR_ON):pVIAlignOnOff->SetBackColor(GXCOLOR_OFF);

	theConfigBank.m_Option.m_bUseEdgeAlign?pEdgeAlingOnOff->SetCaption(_T("ON")):pEdgeAlingOnOff->SetCaption(_T("OFF"));
	pEdgeAlingOnOff->GetCaption()==_T("ON")?pEdgeAlingOnOff->SetBackColor(GXCOLOR_ON):pEdgeAlingOnOff->SetBackColor(GXCOLOR_OFF);

	theConfigBank.m_Option.m_bUseLoginRequest?pLoginRequestOnOff->SetCaption(_T("ON")):pLoginRequestOnOff->SetCaption(_T("OFF"));
	pLoginRequestOnOff->GetCaption()==_T("ON")?pLoginRequestOnOff->SetBackColor(GXCOLOR_ON):pLoginRequestOnOff->SetBackColor(GXCOLOR_OFF);

	theConfigBank.m_Option.m_bUseManualReceive?pManualReceive->SetCaption(_T("ON")):pManualReceive->SetCaption(_T("OFF"));
	pManualReceive->GetCaption()==_T("ON")?pManualReceive->SetBackColor(GXCOLOR_ON):pManualReceive->SetBackColor(GXCOLOR_OFF);

	theConfigBank.m_Option.m_bUseRCMD?pRCMD->SetCaption(_T("ON")):pRCMD->SetCaption(_T("OFF"));
	pRCMD->GetCaption()==_T("ON")?pRCMD->SetBackColor(GXCOLOR_ON):pRCMD->SetBackColor(GXCOLOR_OFF);

	theConfigBank.m_Option.m_bUseVIInsp?pVIInsp->SetCaption(_T("ON")):pVIInsp->SetCaption(_T("OFF"));
	pVIInsp->GetCaption()==_T("ON")?pVIInsp->SetBackColor(GXCOLOR_ON):pVIInsp->SetBackColor(GXCOLOR_OFF);

	//2017-07-06,SKCHO,  고객사 요청으로 DCR 옵션 기능 삭제
	//theConfigBank.m_Option.m_bUseDCR?pDCROnOff->SetCaption(_T("ON")):pDCROnOff->SetCaption(_T("OFF"));
	//pDCROnOff->GetCaption()==_T("ON")?pDCROnOff->SetBackColor(GXCOLOR_ON):pDCROnOff->SetBackColor(GXCOLOR_OFF);

	theConfigBank.m_Option.m_bUseTrayAlign?pTrayAlignOnOff->SetCaption(_T("ON")):pTrayAlignOnOff->SetCaption(_T("OFF"));
	pTrayAlignOnOff->GetCaption()==_T("ON")?pTrayAlignOnOff->SetBackColor(GXCOLOR_ON):pTrayAlignOnOff->SetBackColor(GXCOLOR_OFF);

	theConfigBank.m_Option.m_bUseContact?pContactOnOff->SetCaption(_T("ON")):pContactOnOff->SetCaption(_T("OFF"));
	pContactOnOff->GetCaption()==_T("ON")?pContactOnOff->SetBackColor(GXCOLOR_ON):pContactOnOff->SetBackColor(GXCOLOR_OFF);

	theConfigBank.m_Option.m_bUseJudgeOK?pJudgeOKOnOff->SetCaption(_T("ON")):pJudgeOKOnOff->SetCaption(_T("OFF"));
	pJudgeOKOnOff->GetCaption()==_T("ON")?pJudgeOKOnOff->SetBackColor(GXCOLOR_ON):pJudgeOKOnOff->SetBackColor(GXCOLOR_OFF);

	//20170201 byskcho
	theConfigBank.m_Option.m_bUseCellInfoSkip?pCellInfoSkip->SetCaption(_T("ON")):pCellInfoSkip->SetCaption(_T("OFF"));
	pCellInfoSkip->GetCaption()==_T("ON")?pCellInfoSkip->SetBackColor(GXCOLOR_ON):pCellInfoSkip->SetBackColor(GXCOLOR_OFF);

	// 2017.2.8 bgkim
	theConfigBank.m_Option.m_bUseJobEnd?pUseJobEnd->SetCaption(_T("ON")):pUseJobEnd->SetCaption(_T("OFF"));
	pUseJobEnd->GetCaption()==_T("ON")?pUseJobEnd->SetBackColor(GXCOLOR_ON):pUseJobEnd->SetBackColor(GXCOLOR_OFF);

	// 2017-09-19,SKCHO,Zone Ch Skip
	strtemp = theConfigBank.m_Option.m_strASkipDescription;
	theConfigBank.m_Option.m_bUseASkip?pASkipOnOff->SetCaption(strtemp):pASkipOnOff->SetCaption(_T("OFF"));
	pASkipOnOff->GetCaption()!=_T("OFF")?pASkipOnOff->SetBackColor(GXCOLOR_ON):pASkipOnOff->SetBackColor(GXCOLOR_OFF);

	strtemp = theConfigBank.m_Option.m_strBSkipDescription;
	theConfigBank.m_Option.m_bUseBSkip?pBSkipOnOff->SetCaption(strtemp):pBSkipOnOff->SetCaption(_T("OFF"));
	pBSkipOnOff->GetCaption()!=_T("OFF")?pBSkipOnOff->SetBackColor(GXCOLOR_ON):pBSkipOnOff->SetBackColor(GXCOLOR_OFF);

	strtemp = theConfigBank.m_Option.m_strCSkipDescription;
	theConfigBank.m_Option.m_bUseCSkip?pCSkipOnOff->SetCaption(strtemp):pCSkipOnOff->SetCaption(_T("OFF"));
	pCSkipOnOff->GetCaption()!=_T("OFF")?pCSkipOnOff->SetBackColor(GXCOLOR_ON):pCSkipOnOff->SetBackColor(GXCOLOR_OFF);

	strtemp = theConfigBank.m_Option.m_strDSkipDescription;
	theConfigBank.m_Option.m_bUseDSkip?pDSkipOnOff->SetCaption(strtemp):pDSkipOnOff->SetCaption(_T("OFF"));
	pDSkipOnOff->GetCaption()!=_T("OFF")?pDSkipOnOff->SetBackColor(GXCOLOR_ON):pDSkipOnOff->SetBackColor(GXCOLOR_OFF);



	theConfigBank.m_Option.m_bUseACh1Skip?pACh1SkipOnOff->SetCaption(_T("ON")):pACh1SkipOnOff->SetCaption(_T("OFF"));
	pACh1SkipOnOff->GetCaption()==_T("ON")?pACh1SkipOnOff->SetBackColor(GXCOLOR_ON):pACh1SkipOnOff->SetBackColor(GXCOLOR_OFF);

	theConfigBank.m_Option.m_bUseBCh1Skip?pBCh1SkipOnOff->SetCaption(_T("ON")):pBCh1SkipOnOff->SetCaption(_T("OFF"));
	pBCh1SkipOnOff->GetCaption()==_T("ON")?pBCh1SkipOnOff->SetBackColor(GXCOLOR_ON):pBCh1SkipOnOff->SetBackColor(GXCOLOR_OFF);

	theConfigBank.m_Option.m_bUseCCh1Skip?pCCh1SkipOnOff->SetCaption(_T("ON")):pCCh1SkipOnOff->SetCaption(_T("OFF"));
	pCCh1SkipOnOff->GetCaption()==_T("ON")?pCCh1SkipOnOff->SetBackColor(GXCOLOR_ON):pCCh1SkipOnOff->SetBackColor(GXCOLOR_OFF);

	theConfigBank.m_Option.m_bUseDCh1Skip?pDCh1SkipOnOff->SetCaption(_T("ON")):pDCh1SkipOnOff->SetCaption(_T("OFF"));
	pDCh1SkipOnOff->GetCaption()==_T("ON")?pDCh1SkipOnOff->SetBackColor(GXCOLOR_ON):pDCh1SkipOnOff->SetBackColor(GXCOLOR_OFF);

	theConfigBank.m_Option.m_bUseACh2Skip?pACh2SkipOnOff->SetCaption(_T("ON")):pACh2SkipOnOff->SetCaption(_T("OFF"));
	pACh2SkipOnOff->GetCaption()==_T("ON")?pACh2SkipOnOff->SetBackColor(GXCOLOR_ON):pACh2SkipOnOff->SetBackColor(GXCOLOR_OFF);

	theConfigBank.m_Option.m_bUseBCh2Skip?pBCh2SkipOnOff->SetCaption(_T("ON")):pBCh2SkipOnOff->SetCaption(_T("OFF"));
	pBCh2SkipOnOff->GetCaption()==_T("ON")?pBCh2SkipOnOff->SetBackColor(GXCOLOR_ON):pBCh2SkipOnOff->SetBackColor(GXCOLOR_OFF);

	theConfigBank.m_Option.m_bUseCCh2Skip?pCCh2SkipOnOff->SetCaption(_T("ON")):pCCh2SkipOnOff->SetCaption(_T("OFF"));
	pCCh2SkipOnOff->GetCaption()==_T("ON")?pCCh2SkipOnOff->SetBackColor(GXCOLOR_ON):pCCh2SkipOnOff->SetBackColor(GXCOLOR_OFF);

	theConfigBank.m_Option.m_bUseDCh2Skip?pDCh2SkipOnOff->SetCaption(_T("ON")):pDCh2SkipOnOff->SetCaption(_T("OFF"));
	pDCh2SkipOnOff->GetCaption()==_T("ON")?pDCh2SkipOnOff->SetBackColor(GXCOLOR_ON):pDCh2SkipOnOff->SetBackColor(GXCOLOR_OFF);

	theConfigBank.m_Option.m_bUseContactRetry?pContactRetry->SetCaption(_T("ON")):pContactRetry->SetCaption(_T("OFF"));
	pContactRetry->GetCaption()==_T("ON")?pContactRetry->SetBackColor(GXCOLOR_ON):pContactRetry->SetBackColor(GXCOLOR_OFF);

	theConfigBank.m_Option.m_bUseCarrierReportSkip?pCarrierReportSkip->SetCaption(_T("ON")):pCarrierReportSkip->SetCaption(_T("OFF"));
	pCarrierReportSkip->GetCaption()==_T("ON")?pCarrierReportSkip->SetBackColor(GXCOLOR_ON):pCarrierReportSkip->SetBackColor(GXCOLOR_OFF);

	theConfigBank.m_Option.m_bUseRepeatInsp?pRepeatInsp->SetCaption(_T("ON")):pRepeatInsp->SetCaption(_T("OFF"));
	pRepeatInsp->GetCaption()==_T("ON")?pRepeatInsp->SetBackColor(GXCOLOR_ON):pRepeatInsp->SetBackColor(GXCOLOR_OFF);

	theConfigBank.m_Option.m_bLoadPitchIn?pConvPitchIn->SetCaption(_T("ON")):pConvPitchIn->SetCaption(_T("OFF"));
	pConvPitchIn->GetCaption()==_T("ON")?pConvPitchIn->SetBackColor(GXCOLOR_ON):pConvPitchIn->SetBackColor(GXCOLOR_OFF);

	theConfigBank.m_Option.m_bGrabMCRUse?pUseMCRGrab->SetCaption(_T("ON")):pUseMCRGrab->SetCaption(_T("OFF"));
	pUseMCRGrab->GetCaption()==_T("ON")?pUseMCRGrab->SetBackColor(GXCOLOR_ON):pUseMCRGrab->SetBackColor(GXCOLOR_OFF);
    
	theConfigBank.m_Option.m_bDivision?pUseDivision->SetCaption(_T("ON")):pUseDivision->SetCaption(_T("OFF"));
	pUseDivision->GetCaption()==_T("ON")?pUseDivision->SetBackColor(GXCOLOR_ON):pUseDivision->SetBackColor(GXCOLOR_OFF);

	theConfigBank.m_Option.m_bSkipLDConvSen3?pLDConvSen3Skip->SetCaption(_T("ON")):pLDConvSen3Skip->SetCaption(_T("OFF"));
	pLDConvSen3Skip->GetCaption()==_T("ON")?pLDConvSen3Skip->SetBackColor(GXCOLOR_ON):pLDConvSen3Skip->SetBackColor(GXCOLOR_OFF);
	
	//2017-09-19,SKCHO
	theConfigBank.m_Option.m_bUseULDTRMode?pUseULDTRMode->SetCaption(_T("ON")):pUseULDTRMode->SetCaption(_T("OFF"));
	pUseULDTRMode->GetCaption()==_T("ON")?pUseULDTRMode->SetBackColor(GXCOLOR_ON):pUseULDTRMode->SetBackColor(GXCOLOR_OFF);
	
	//2017-09-19,SKCHO
	theConfigBank.m_Option.m_bUseLDRobotRetry?pLDRobot->SetCaption(_T("ON")):pLDRobot->SetCaption(_T("OFF"));
	pLDRobot->GetCaption()==_T("ON")?pLDRobot->SetBackColor(GXCOLOR_ON):pLDRobot->SetBackColor(GXCOLOR_OFF);
	
	theConfigBank.m_Option.m_bUseTrayVI?pTrayVI->SetCaption(_T("ON")):pTrayVI->SetCaption(_T("OFF"));
	pTrayVI->GetCaption()==_T("ON")?pTrayVI->SetBackColor(GXCOLOR_ON):pTrayVI->SetBackColor(GXCOLOR_OFF);

	//2017-08-14 KAKAKA
	theConfigBank.m_Option.m_NGINLIFTSEN1?pUseNGIN1->SetCaption(_T("ON")):pUseNGIN1->SetCaption(_T("OFF"));
	pUseNGIN1->GetCaption()==_T("ON")?pUseNGIN1->SetBackColor(GXCOLOR_ON):pUseNGIN1->SetBackColor(GXCOLOR_OFF);

	theConfigBank.m_Option.m_NGINLIFTSEN2?pUseNGIN2->SetCaption(_T("ON")):pUseNGIN2->SetCaption(_T("OFF"));
	pUseNGIN2->GetCaption()==_T("ON")?pUseNGIN2->SetBackColor(GXCOLOR_ON):pUseNGIN2->SetBackColor(GXCOLOR_OFF);

	theConfigBank.m_Option.m_NGOUTLIFTSEN1?pUseNGOUT1->SetCaption(_T("ON")):pUseNGOUT1->SetCaption(_T("OFF"));
	pUseNGOUT1->GetCaption()==_T("ON")?pUseNGOUT1->SetBackColor(GXCOLOR_ON):pUseNGOUT1->SetBackColor(GXCOLOR_OFF);

	theConfigBank.m_Option.m_NGOUTLIFTSEN2?pUseNGOUT2->SetCaption(_T("ON")):pUseNGOUT2->SetCaption(_T("OFF"));
	pUseNGOUT2->GetCaption()==_T("ON")?pUseNGOUT2->SetBackColor(GXCOLOR_ON):pUseNGOUT2->SetBackColor(GXCOLOR_OFF);

	theConfigBank.m_Option.m_bUsePATRRetry?pPATR->SetCaption(_T("ON")):pPATR->SetCaption(_T("OFF"));
	pPATR->GetCaption()==_T("ON")?pPATR->SetBackColor(GXCOLOR_ON):pPATR->SetBackColor(GXCOLOR_OFF);

	theConfigBank.m_Option.m_bUseJigBlow?pJigBlow->SetCaption(_T("ON")):pJigBlow->SetCaption(_T("OFF"));
	pJigBlow->GetCaption()==_T("ON")?pJigBlow->SetBackColor(GXCOLOR_ON):pJigBlow->SetBackColor(GXCOLOR_OFF);

	//2018-06-29,NAM, Add Option for Conveyor Mode 
	theConfigBank.m_Option.m_bUseConveyorMode?pConveyorMode->SetCaption(_T("ON")):pConveyorMode->SetCaption(_T("OFF"));
	pConveyorMode->GetCaption()==_T("ON")?pConveyorMode->SetBackColor(GXCOLOR_ON):pConveyorMode->SetBackColor(GXCOLOR_OFF);

	//2018-01-20,GHLEE
	theConfigBank.m_Option.m_bUseFPCBSen?pFPCBSen->SetCaption(_T("ON")):pFPCBSen->SetCaption(_T("OFF"));
	pFPCBSen->GetCaption()==_T("ON")?pFPCBSen->SetBackColor(GXCOLOR_ON):pFPCBSen->SetBackColor(GXCOLOR_OFF);

	//2018-03-15,GHLEE
	theConfigBank.m_Option.m_bGoodInFinalSen?pGoodInFinalSen->SetCaption(_T("ON")):pFPCBSen->SetCaption(_T("OFF"));
	pGoodInFinalSen->GetCaption()==_T("ON")?pGoodInFinalSen->SetBackColor(GXCOLOR_ON):pGoodInFinalSen->SetBackColor(GXCOLOR_OFF);
	
	//2018wjp
	theConfigBank.m_Option.m_bUseBuffer?pBufferModelUse->SetCaption(_T("ON")):pBufferModelUse->SetCaption(_T("OFF"));
	pBufferModelUse->GetCaption()==_T("ON")?pBufferModelUse->SetBackColor(GXCOLOR_ON):pBufferModelUse->SetBackColor(GXCOLOR_OFF);

	theConfigBank.m_Option.m_bUseConveyorTestModel?pUseConveyorTestModel->SetCaption(_T("ON")):pUseConveyorTestModel->SetCaption(_T("OFF"));
	pUseConveyorTestModel->GetCaption()==_T("ON")?pUseConveyorTestModel->SetBackColor(GXCOLOR_ON):pUseConveyorTestModel->SetBackColor(GXCOLOR_OFF);


	strtemp.Format(_T("%d"), theConfigBank.m_Option.m_nBufferUseWaitTime);
	pBufferModelWaitTime->SetCaption(strtemp);

	strtemp.Format(_T("%.1f"), theConfigBank.m_Option.m_nBufferUseTrayDownZ);
    pBufferModelTrayZDis->SetCaption(strtemp);
}



void CFormOptions::ClickGxbtnOptionSave()
{
//	CMainFrame *pMainFrm = (CMainFrame *)AfxGetMainWnd();
	CGxMsgBox	dlgMsgBox;
	CGxButtonEx* pLotEndOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_LOTEND_ONOFF);
	CGxButtonEx* pDryRunOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_DRYRUN_ONOFF);
	CGxButtonEx* pUpperIFOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_UPPERIF_ONOFF);
	CGxButtonEx* pUpperMCRReadOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_UPPERMCRREAD_ONOFF);
	CGxButtonEx* pLowerMCRReadOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_LOWERMCRREAD_ONOFF);
	CGxButtonEx* pPreAlignOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_PREALIGN_ONOFF);
	CGxButtonEx* pVIAlignOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_VIALIGN_ONOFF);
	CGxButtonEx* pEdgeAlingOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_EDGEALIGN_ONOFF);
	CGxButtonEx* pLoginRequestOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_LOGIN_REQUEST_ONOFF);
	CGxButtonEx* pManualReceive = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_MANUAL_RECEIVE);
	CGxButtonEx* pRCMD = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_RCMD);
	CGxButtonEx* pVIInsp = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_VIINSP_ONOFF);
	//2017-07-06,SKCHO,  고객사 요청으로 DCR 옵션 기능 삭제
	//CGxButtonEx* pDCROnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_DCR_USE);
	CGxButtonEx* pTrayAlignOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_TRAY_ALIGN_USE);
	CGxButtonEx* pContactOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_CONTACT_USE);
	CGxButtonEx* pJudgeOKOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_JUDGEOK_USE);
	//20170201 byskcho
	CGxButtonEx* pCellInfoSkip = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_CELLINFOSKIP_ONOFF);
	// 2017.2.8 bgkim
	CGxButtonEx* pJobEndTime = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_JOB_END_TIME); 
	CGxButtonEx* pUseJobEnd = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_USE_JOB_END);

	CGxButtonEx* pViGrabEndWaitTime = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_VI_GRAB_END_WAIT_TIME);
	CGxButtonEx* pViClassEndWaitTime = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_VI_CLASS_END_WAIT_TIME);
	CGxButtonEx* pGoodTrayMaxCount = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_GOOD_TRAY_MAX_COUNT);
	CGxButtonEx* pNGTrayMaxCount = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_NG_TRAY_MAX_COUNT);

	//화상 검사 연속 NG 체크
	CGxButtonEx* pVisionContinuesNGCnt = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_VISION_CONTINUES_NG_COUNT);

	// Zone , Ch Skip
	CGxButtonEx* pASkipOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_ASKIP_ONOFF);
	CGxButtonEx* pBSkipOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_BSKIP_ONOFF);
	CGxButtonEx* pCSkipOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_CSKIP_ONOFF);
	CGxButtonEx* pDSkipOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_DSKIP_ONOFF);
	CGxButtonEx* pACh1SkipOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_ASKIPCH1_ONOFF);
	CGxButtonEx* pBCh1SkipOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_BSKIPCH1_ONOFF);
	CGxButtonEx* pCCh1SkipOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_CSKIPCH1_ONOFF);
	CGxButtonEx* pDCh1SkipOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_DSKIPCH1_ONOFF);
	CGxButtonEx* pACh2SkipOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_ASKIPCH2_ONOFF);
	CGxButtonEx* pBCh2SkipOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_BSKIPCH2_ONOFF);
	CGxButtonEx* pCCh2SkipOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_CSKIPCH2_ONOFF);
	CGxButtonEx* pDCh2SkipOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_DSKIPCH2_ONOFF);

	CGxButtonEx* pContactRetry = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_CONTACT_RETRY);

	CGxButtonEx* pCarrierReportSkip = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_CARRIER_REPORT);

	CGxButtonEx* pRepeatInsp = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_REPEAT_INSP);
	//2017-03-23,skcho
	CGxButtonEx* pRunDownWaitTime = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_RUN_DOWN_WAIT_TIME);

	CGxButtonEx* pMCRDelayTime = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_MCR_TIME);

	CGxButtonEx* pConvPitchIn = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_CONV_PITCH_IN);

	CGxButtonEx* pUseMCRGrab = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_MCRGRAB_USE);

	CGxButtonEx* pRejectCode = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_REJECT_CODE);

	//2017-07-10,SKCHO, TRAYID 자릿수
	CGxButtonEx* pTrayIDCount = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_TRAYID_COUNT);

	//2017-07-20,SKCHO,
	CGxButtonEx* pUseDivision = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_DIVISION);
	
	CGxButtonEx* pLDConvSen3Skip = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_LDCONVSEN3SKIP);

	CGxButtonEx* pAGVDelayTime = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_AGV_DELAYTIME);
	//2017-09-02,SKCHO
	CGxButtonEx* pLDTRBlowerOff =(CGxButtonEx*)GetDlgItem(IDC_GXBTN_LDTRBLOWER_WAIT_TIME);

	//2017-09-19,SKCHO
	CGxButtonEx* pCellInfoWait =(CGxButtonEx*)GetDlgItem(IDC_GXBTN_CELLINFO_WAIT_TIME);
	//2017-09-19,SKCHO
	CGxButtonEx* pCarrierrInfoWait =(CGxButtonEx*)GetDlgItem(IDC_GXBTN_CARRIERINFO_WAIT_TIME);
	CGxButtonEx* pUseULDTRMode = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_ULDTRMODE);
	//2017-09-21,SKCHO
	CGxButtonEx* pT5RestartTime = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_T5_RESTART_TIME);
	CGxButtonEx* pGOODTRMode = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_GOODTRLENGTH);
	CGxButtonEx* pNGTRMode = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_NGTRLENGTH);
	CGxButtonEx* pLDRobot = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_LDROBOT_RETRY);
	
	CGxButtonEx* pTrayVI = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_TRAYVI);

	CGxButtonEx* pUseNGIN1 = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_NGIN1_ONOFF);
	CGxButtonEx* pUseNGIN2 = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_NGIN2_ONOFF);
	CGxButtonEx* pUseNGOUT1 = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_NGOUT1_ONOFF);
	CGxButtonEx* pUseNGOUT2 = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_NGOUT2_ONOFF);
	
	CGxButtonEx* pPATR = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_PATR_RETRY);

	//2017-12-23,SKCHO
	CGxButtonEx* pJigBlow = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_JIGBLOW);
	CGxButtonEx* pPaOrgTime = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_PA_ORG_TIME);

	//2018-05-11,NAM
	CGxButtonEx* pContactButton = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_CONTACT_DELAY_TIME);

	
	//2018-06-29,NAM
	CGxButtonEx* pConveyorDelay = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_CONVEYOR_DELAY_TIME);

	//2018-06-29,NAM
	CGxButtonEx* pConveyorMode = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_CONVEYORMODE);

	//2018-01-20,GHLEE
	CGxButtonEx* pFPCBSen = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_FPCBSEN);
	//2018-03-15,GHLEE
	CGxButtonEx* pGoodInFinalSen = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_LASTTRAYSEN);

	//2018wjp
	CGxButtonEx* pBufferModelUse      = (CGxButtonEx*)GetDlgItem(IDC_MODEL_BUFFER_USE);
	CGxButtonEx* pBufferModelWaitTime = (CGxButtonEx*)GetDlgItem(IDC_MODEL_BUFFER_WAIT_TIME);
	CGxButtonEx* pBufferModelTrayZDis = (CGxButtonEx*)GetDlgItem(IDC_MODEL_BUFFER_Z_DIS);
	CGxButtonEx* pUseConveyorTestModel = (CGxButtonEx*)GetDlgItem(IDC_CONVEYOR_TEST_MODEL_SET);

	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
	dlgMsgBox.SetMessage(TRUE, 
		_T("수정하신 작업을 적용하겠습니까?"), 
		_T("Would you like to apply your modifications?"),		
		_T("Bạn có muốn áp dụng những thay đổi của bạn?") );

	int nRet = dlgMsgBox.DoModal();
	if(nRet != IDOK)
		return;

	pLotEndOnOff->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bUseLotEnd=TRUE:theConfigBank.m_Option.m_bUseLotEnd=FALSE;
	pDryRunOnOff->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bUseDryRun=TRUE:theConfigBank.m_Option.m_bUseDryRun=FALSE;
	pUpperMCRReadOnOff->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bUseUpperMCRRead=TRUE:theConfigBank.m_Option.m_bUseUpperMCRRead=FALSE;
	pLowerMCRReadOnOff->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bUseLowerMCRRead=TRUE:theConfigBank.m_Option.m_bUseLowerMCRRead=FALSE;
	pPreAlignOnOff->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bUsePreAlign=TRUE:theConfigBank.m_Option.m_bUsePreAlign=FALSE;
	pVIAlignOnOff->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bUseVIAlign=TRUE:theConfigBank.m_Option.m_bUseVIAlign=FALSE;
	pEdgeAlingOnOff->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bUseEdgeAlign=TRUE:theConfigBank.m_Option.m_bUseEdgeAlign=FALSE;
	pLoginRequestOnOff->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bUseLoginRequest=TRUE:theConfigBank.m_Option.m_bUseLoginRequest=FALSE;
	pManualReceive->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bUseManualReceive=TRUE:theConfigBank.m_Option.m_bUseManualReceive=FALSE;
	pRCMD->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bUseRCMD=TRUE:theConfigBank.m_Option.m_bUseRCMD=FALSE;
	pVIInsp->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bUseVIInsp=TRUE:theConfigBank.m_Option.m_bUseVIInsp=FALSE;
	//2017-07-06,SKCHO,  고객사 요청으로 DCR 옵션 기능 삭제
	//pDCROnOff->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bUseDCR=TRUE:theConfigBank.m_Option.m_bUseDCR=FALSE;
	pTrayAlignOnOff->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bUseTrayAlign=TRUE:theConfigBank.m_Option.m_bUseTrayAlign=FALSE;
	pContactOnOff->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bUseContact=TRUE:theConfigBank.m_Option.m_bUseContact=FALSE;
	pJudgeOKOnOff->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bUseJudgeOK=TRUE:theConfigBank.m_Option.m_bUseJudgeOK=FALSE;
	//20170201 byskcho
	pCellInfoSkip->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bUseCellInfoSkip=TRUE:theConfigBank.m_Option.m_bUseCellInfoSkip=FALSE;
	pUseJobEnd->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bUseJobEnd=TRUE:theConfigBank.m_Option.m_bUseJobEnd=FALSE;
	
	pUseConveyorTestModel->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bUseConveyorTestModel=TRUE:theConfigBank.m_Option.m_bUseConveyorTestModel=FALSE;
	
	//2017-09-19,SKCHO	
	if(pASkipOnOff->GetCaption()!=(_T("OFF")))
	{
		theConfigBank.m_Option.m_bUseASkip=TRUE;
	}
	else
	{
		theConfigBank.m_Option.m_bUseASkip=FALSE;
	}

	if(pBSkipOnOff->GetCaption()!=(_T("OFF")))
	{
		theConfigBank.m_Option.m_bUseBSkip=TRUE;
	}
	else
	{
		theConfigBank.m_Option.m_bUseBSkip=FALSE;
	}

	if(pCSkipOnOff->GetCaption()!=(_T("OFF")))
	{
		theConfigBank.m_Option.m_bUseCSkip=TRUE;
	}
	else
	{
		theConfigBank.m_Option.m_bUseCSkip=FALSE;
	}

	if(pDSkipOnOff->GetCaption()!=(_T("OFF")))
	{
		theConfigBank.m_Option.m_bUseDSkip=TRUE;
	}
	else
	{
		theConfigBank.m_Option.m_bUseDSkip=FALSE;
	}
	
	theConfigBank.m_Option.m_strASkipDescription = pASkipOnOff->GetCaption();
	theConfigBank.m_Option.m_strBSkipDescription = pBSkipOnOff->GetCaption();
	theConfigBank.m_Option.m_strCSkipDescription = pCSkipOnOff->GetCaption();
	theConfigBank.m_Option.m_strDSkipDescription = pDSkipOnOff->GetCaption();
	
	
	pACh1SkipOnOff->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bUseACh1Skip=TRUE:theConfigBank.m_Option.m_bUseACh1Skip=FALSE;
	pBCh1SkipOnOff->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bUseBCh1Skip=TRUE:theConfigBank.m_Option.m_bUseBCh1Skip=FALSE;
	pCCh1SkipOnOff->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bUseCCh1Skip=TRUE:theConfigBank.m_Option.m_bUseCCh1Skip=FALSE;
	pDCh1SkipOnOff->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bUseDCh1Skip=TRUE:theConfigBank.m_Option.m_bUseDCh1Skip=FALSE;
	pACh2SkipOnOff->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bUseACh2Skip=TRUE:theConfigBank.m_Option.m_bUseACh2Skip=FALSE;
	pBCh2SkipOnOff->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bUseBCh2Skip=TRUE:theConfigBank.m_Option.m_bUseBCh2Skip=FALSE;
	pCCh2SkipOnOff->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bUseCCh2Skip=TRUE:theConfigBank.m_Option.m_bUseCCh2Skip=FALSE;
	pDCh2SkipOnOff->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bUseDCh2Skip=TRUE:theConfigBank.m_Option.m_bUseDCh2Skip=FALSE;

	pContactRetry->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bUseContactRetry=TRUE:theConfigBank.m_Option.m_bUseContactRetry=FALSE;

	pCarrierReportSkip->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bUseCarrierReportSkip=TRUE:theConfigBank.m_Option.m_bUseCarrierReportSkip=FALSE;
	
	pRepeatInsp->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bUseRepeatInsp=TRUE:theConfigBank.m_Option.m_bUseRepeatInsp=FALSE;

	pConvPitchIn->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bLoadPitchIn=TRUE:theConfigBank.m_Option.m_bLoadPitchIn=FALSE;

	pUseMCRGrab->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bGrabMCRUse=TRUE:theConfigBank.m_Option.m_bGrabMCRUse=FALSE;

	//2017-07-20,SKCHO
	pUseDivision->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bDivision=TRUE:theConfigBank.m_Option.m_bDivision=FALSE;
	pLDConvSen3Skip->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bSkipLDConvSen3=TRUE:theConfigBank.m_Option.m_bSkipLDConvSen3=FALSE;
	//2017-09-19,SKCHO
	pUseULDTRMode->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bUseULDTRMode=TRUE:theConfigBank.m_Option.m_bUseULDTRMode=FALSE;
	//2017-09-19,SKCHO
	pLDRobot->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bUseLDRobotRetry=TRUE:theConfigBank.m_Option.m_bUseLDRobotRetry=FALSE;
	
	
	pTrayVI->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bUseTrayVI=TRUE:theConfigBank.m_Option.m_bUseTrayVI=FALSE;

	pUseNGIN1->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_NGINLIFTSEN1=TRUE:theConfigBank.m_Option.m_NGINLIFTSEN1=FALSE; 
	pUseNGIN2->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_NGINLIFTSEN2=TRUE:theConfigBank.m_Option.m_NGINLIFTSEN2=FALSE; 
	pUseNGOUT1->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_NGOUTLIFTSEN1=TRUE:theConfigBank.m_Option.m_NGOUTLIFTSEN1=FALSE;
	pUseNGOUT2->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_NGOUTLIFTSEN2=TRUE:theConfigBank.m_Option.m_NGOUTLIFTSEN2=FALSE;

	theConfigBank.m_Option.m_dJobEndTime = _tstof(pJobEndTime->GetCaption());
	theConfigBank.m_Option.m_dGrabEndWaitTime = _tstof(pViGrabEndWaitTime->GetCaption());
	theConfigBank.m_Option.m_dClassEndWaitTime = _tstof(pViClassEndWaitTime->GetCaption());
	theConfigBank.m_Option.m_nMaxGoodTrayCount = _tstoi(pGoodTrayMaxCount->GetCaption());
	theConfigBank.m_Option.m_nMaxNGTrayCount = _tstoi(pNGTrayMaxCount->GetCaption());

	theConfigBank.m_Option.m_dRunDownWaitTime = _tstof(pRunDownWaitTime->GetCaption());

	theConfigBank.m_Option.m_dMCRDelayTime = _tstof(pMCRDelayTime->GetCaption());

	theConfigBank.m_Option.m_nVisionContinuesNGCnt = _tstoi(pVisionContinuesNGCnt->GetCaption());

	theConfigBank.m_Option.m_strRejectCode = pRejectCode->GetCaption();

	//2017-07-10,SKCHO
	theConfigBank.m_Option.m_nTrayIDCount =  _tstoi(pTrayIDCount->GetCaption());

	//2017-08-30,SKCHO
	theConfigBank.m_Option.m_nAGVDelayTime =  _tstoi(pAGVDelayTime->GetCaption());
	theConfigBank.m_Option.m_dLDTRBlowerOffTime = _tstof(pLDTRBlowerOff->GetCaption());

	theConfigBank.m_Option.m_nT5RestartTime =  _tstoi(pT5RestartTime->GetCaption());
	theConfigBank.m_Option.m_dGoodTRRetryLength =  _tstof(pGOODTRMode->GetCaption());
	theConfigBank.m_Option.m_dNGTRRetryLength =  _tstof(pNGTRMode->GetCaption());
	
	//2018-05-11,NAM
	theConfigBank.m_Option.m_nContactDelay =  _tstoi(pContactButton->GetCaption());

	//2018-06-29,NAM
	theConfigBank.m_Option.m_nConveyorDelay =  _tstoi(pConveyorDelay->GetCaption());

	//2018-06-29,NAM
	pConveyorMode->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bUseConveyorMode=TRUE:theConfigBank.m_Option.m_bUseConveyorMode=FALSE;
	

	//2017-09-19,SKCHO
	theConfigBank.m_Option.m_dCellInfoWaitTime = _tstof(pCellInfoWait->GetCaption());
	//2017-09-19,SKCHO
	theConfigBank.m_Option.m_dCarrierInfoWaitTime = _tstof(pCarrierrInfoWait->GetCaption());

	//2017-11-17,SKCHO
	pPATR->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bUsePATRRetry=TRUE:theConfigBank.m_Option.m_bUsePATRRetry=FALSE;
	
	//2017-11-17,SKCHO
	pJigBlow->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bUseJigBlow=TRUE:theConfigBank.m_Option.m_bUseJigBlow=FALSE;
	
	theConfigBank.m_Option.m_nPaOrgTime =  _tstoi(pPaOrgTime->GetCaption());

	//2018-01-20,GHLEE
	pFPCBSen->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bUseFPCBSen=TRUE:theConfigBank.m_Option.m_bUseFPCBSen=FALSE;
	//2018-03-15,GHLEE
	pGoodInFinalSen->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bGoodInFinalSen=TRUE:theConfigBank.m_Option.m_bGoodInFinalSen=FALSE;

	//2017-11-01,SKCHO, 옵션에서 사용자가 JIG SKIP 선택하면 102보고하게
	if(theConfigBank.m_Option.m_bOldUseASkip  != theConfigBank.m_Option.m_bUseASkip)
	{
		if(theConfigBank.m_Option.m_bUseASkip == TRUE )
		{
			theSocketInterFace.SendCmdUnitInterlockToDataPC(EQUIP_SET_UNIT_STATUS_CHANGE_INSPECTION,1,_T("E_EQST_MATCH_INTERLOCK"),_T("0"));  //A JIG	
		}
		else 
		{
			theSocketInterFace.SendCmdUnitInterlockToDataPC(EQUIP_SET_UNIT_STATUS_CHANGE_INSPECTION,1,_T(""),_T("0"));  //A JIG	
		}
	}
	Sleep(10);
	if(theConfigBank.m_Option.m_bOldUseBSkip  != theConfigBank.m_Option.m_bUseBSkip)
	{
		if(theConfigBank.m_Option.m_bUseBSkip == TRUE )
		{
			theSocketInterFace.SendCmdUnitInterlockToDataPC(EQUIP_SET_UNIT_STATUS_CHANGE_INSPECTION,2,_T("E_EQST_MATCH_INTERLOCK"),_T("0"));  //BJIG	
		}
		else
		{
			theSocketInterFace.SendCmdUnitInterlockToDataPC(EQUIP_SET_UNIT_STATUS_CHANGE_INSPECTION,2,_T(""),_T("0"));  //B JIG	
		}
	}
	Sleep(10);
	if(theConfigBank.m_Option.m_bOldUseCSkip  != theConfigBank.m_Option.m_bUseCSkip)
	{
		if(theConfigBank.m_Option.m_bUseCSkip == TRUE )
		{
			theSocketInterFace.SendCmdUnitInterlockToDataPC(EQUIP_SET_UNIT_STATUS_CHANGE_INSPECTION,3,_T("E_EQST_MATCH_INTERLOCK"),_T("0"));  //C JIG	
		}
		else
		{
			theSocketInterFace.SendCmdUnitInterlockToDataPC(EQUIP_SET_UNIT_STATUS_CHANGE_INSPECTION,3,_T(""),_T("0"));  //C JIG	
		}
	}
	Sleep(10);
	if(theConfigBank.m_Option.m_bOldUseDSkip  != theConfigBank.m_Option.m_bUseDSkip)
	{
		if(theConfigBank.m_Option.m_bUseDSkip == TRUE )
		{
			theSocketInterFace.SendCmdUnitInterlockToDataPC(EQUIP_SET_UNIT_STATUS_CHANGE_INSPECTION,4,_T("E_EQST_MATCH_INTERLOCK"),_T("0"));  //D JIG	
		}
		else 
		{
			theSocketInterFace.SendCmdUnitInterlockToDataPC(EQUIP_SET_UNIT_STATUS_CHANGE_INSPECTION,4,_T(""),_T("0"));  //D JIG	
		}
	}

	theConfigBank.m_Option.m_bOldUseASkip  = theConfigBank.m_Option.m_bUseASkip;
	theConfigBank.m_Option.m_bOldUseBSkip  = theConfigBank.m_Option.m_bUseBSkip;
	theConfigBank.m_Option.m_bOldUseCSkip  = theConfigBank.m_Option.m_bUseCSkip;
	theConfigBank.m_Option.m_bOldUseDSkip  = theConfigBank.m_Option.m_bUseDSkip;

	///// 설비 런 중 UNIT 전부 스킵하면 셀 받지 않고 진행중인 셀 배출 후 설비 정지함
	if(theConfigBank.m_Option.m_bUseASkip
	   && theConfigBank.m_Option.m_bUseBSkip 
	   && theConfigBank.m_Option.m_bUseCSkip
	   && theConfigBank.m_Option.m_bUseDSkip)
	{
		theProcBank.m_bAllUnitInterlock_Stop = TRUE;
	}
	else if(!theConfigBank.m_Option.m_bUseASkip
	   || !theConfigBank.m_Option.m_bUseBSkip 
	   || !theConfigBank.m_Option.m_bUseCSkip
	   || !theConfigBank.m_Option.m_bUseDSkip)
	{
		theProcBank.m_bAllUnitInterlock_Stop = FALSE;
	}
	else if(theConfigBank.m_Option.m_bUseASkip == FALSE
	   && theConfigBank.m_Option.m_bUseBSkip == FALSE  
	   && theConfigBank.m_Option.m_bUseCSkip == FALSE
	   && theConfigBank.m_Option.m_bUseDSkip == FALSE)
	{
		theProcBank.m_cUnitModuleNum = 0X00;
	}
	//2018wjp
	pBufferModelUse->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bUseBuffer=TRUE:theConfigBank.m_Option.m_bUseBuffer=FALSE;
	theConfigBank.m_Option.m_nBufferUseWaitTime =  _tstoi(pBufferModelWaitTime->GetCaption());
	theConfigBank.m_Option.m_nBufferUseTrayDownZ =  _tstof(pBufferModelTrayZDis->GetCaption());      
	pUseConveyorTestModel->GetCaption()==(_T("ON"))?theConfigBank.m_Option.m_bUseConveyorTestModel=TRUE:theConfigBank.m_Option.m_bUseConveyorTestModel=FALSE;

	theConfigBank.SaveOptionFile();
	LoadOption();
}


void CFormOptions::ClickGxbtnOptionLotendOnoff()
{
	CString strValue;
	CGxButtonEx* pLotEndOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_LOTEND_ONOFF);
	strValue = pLotEndOnOff->GetCaption();
	if(strValue==_T("OFF"))
	{
		pLotEndOnOff->SetCaption(_T("ON"));
		pLotEndOnOff->SetBackColor(GXCOLOR_ON);
	}
	else
	{
		pLotEndOnOff->SetCaption(_T("OFF"));
		pLotEndOnOff->SetBackColor(GXCOLOR_OFF);
	}
}

void CFormOptions::ClickGxbtnOptionDryrunOnoff()
{
	CString strValue;
	CGxButtonEx* pDryRunOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_DRYRUN_ONOFF);
	strValue = pDryRunOnOff->GetCaption();
	if(strValue==_T("OFF"))
	{
		pDryRunOnOff->SetCaption(_T("ON"));
		pDryRunOnOff->SetBackColor(GXCOLOR_ON);
	}
	else
	{
		pDryRunOnOff->SetCaption(_T("OFF"));
		pDryRunOnOff->SetBackColor(GXCOLOR_OFF);
	}
}

void CFormOptions::ClickGxbtnOptionUpperifOnoff()
{
	CString strValue;
	CGxButtonEx* pUpperIFOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_UPPERIF_ONOFF);
	strValue = pUpperIFOnOff->GetCaption();
	if(strValue==_T("OFF"))
	{
		pUpperIFOnOff->SetCaption(_T("ON"));
		pUpperIFOnOff->SetBackColor(GXCOLOR_ON);
	}
	else
	{
		pUpperIFOnOff->SetCaption(_T("OFF"));
		pUpperIFOnOff->SetBackColor(GXCOLOR_OFF);
	}
}

void CFormOptions::ClickGxbtnOptionUppermcrreadOnoff()
{
	CString strValue;
	CGxButtonEx* pUpperMCRReadOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_UPPERMCRREAD_ONOFF);
	strValue = pUpperMCRReadOnOff->GetCaption();
	if(strValue==_T("OFF"))
	{
		pUpperMCRReadOnOff->SetCaption(_T("ON"));
		pUpperMCRReadOnOff->SetBackColor(GXCOLOR_ON);
	}
	else
	{
		pUpperMCRReadOnOff->SetCaption(_T("OFF"));
		pUpperMCRReadOnOff->SetBackColor(GXCOLOR_OFF);
	}
}


void CFormOptions::ClickGxbtnOptionLowermcrreadOnoff()
{
	CString strValue;
	CGxButtonEx* pLowerMCRReadOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_LOWERMCRREAD_ONOFF);
	strValue = pLowerMCRReadOnOff->GetCaption();
	if(strValue==_T("OFF"))
	{
		pLowerMCRReadOnOff->SetCaption(_T("ON"));
		pLowerMCRReadOnOff->SetBackColor(GXCOLOR_ON);
	}
	else
	{
		pLowerMCRReadOnOff->SetCaption(_T("OFF"));
		pLowerMCRReadOnOff->SetBackColor(GXCOLOR_OFF);
	}
}

void CFormOptions::ClickGxbtnOptionPrealignOnoff()
{
	CString strValue;
	CGxButtonEx* pPreAlignOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_PREALIGN_ONOFF);
	strValue = pPreAlignOnOff->GetCaption();
	if(strValue==_T("OFF"))
	{
		pPreAlignOnOff->SetCaption(_T("ON"));
		pPreAlignOnOff->SetBackColor(GXCOLOR_ON);

	}
	else
	{
		pPreAlignOnOff->SetCaption(_T("OFF"));
		pPreAlignOnOff->SetBackColor(GXCOLOR_OFF);
	}
}

void CFormOptions::ClickGxbtnOptionVialignOnoff()
{
	CString strValue;
	CGxButtonEx* pVIAlignOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_VIALIGN_ONOFF);
	strValue = pVIAlignOnOff->GetCaption();
	if(strValue==_T("OFF"))
	{
		pVIAlignOnOff->SetCaption(_T("ON"));
		pVIAlignOnOff->SetBackColor(GXCOLOR_ON);

	}
	else
	{
		pVIAlignOnOff->SetCaption(_T("OFF"));
		pVIAlignOnOff->SetBackColor(GXCOLOR_OFF);
	}
}

void CFormOptions::ClickGxbtnOptionEdgealignOnoff()
{
	CString strValue;
	CGxButtonEx* pEdgeAlignOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_EDGEALIGN_ONOFF);
	strValue = pEdgeAlignOnOff->GetCaption();
	if(strValue==_T("OFF"))
	{
		pEdgeAlignOnOff->SetCaption(_T("ON"));
		pEdgeAlignOnOff->SetBackColor(GXCOLOR_ON);
	}
	else
	{
		pEdgeAlignOnOff->SetCaption(_T("OFF"));
		pEdgeAlignOnOff->SetBackColor(GXCOLOR_OFF);
	}
}

void CFormOptions::ClickGxbtnOptionLoginRequestOnoff()
{
	CString strValue;
	CGxButtonEx* pLoginRequestOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_LOGIN_REQUEST_ONOFF);
	strValue = pLoginRequestOnOff->GetCaption();
	if(strValue==_T("OFF"))
	{
		pLoginRequestOnOff->SetCaption(_T("ON"));
		pLoginRequestOnOff->SetBackColor(GXCOLOR_ON);
	}
	else
	{
		pLoginRequestOnOff->SetCaption(_T("OFF"));
		pLoginRequestOnOff->SetBackColor(GXCOLOR_OFF);
	}
}

void CFormOptions::ClickGxbtnOptionLoad()
{
	theConfigBank.ReadOptionFile();
	LoadOption();
}

BOOL CFormOptions::SaveOptionCompare()
{
	CGxButtonEx* pLotEndOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_LOTEND_ONOFF);
	CGxButtonEx* pDryRunOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_DRYRUN_ONOFF);
	CGxButtonEx* pUpperIFOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_UPPERIF_ONOFF);
	CGxButtonEx* pUpperMCRReadOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_UPPERMCRREAD_ONOFF);
	CGxButtonEx* pLowerMCRReadOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_LOWERMCRREAD_ONOFF);
	CGxButtonEx* pPreAlignOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_PREALIGN_ONOFF);
	CGxButtonEx* pVIAlignOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_VIALIGN_ONOFF);
	CGxButtonEx* pEdgeAlingOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_EDGEALIGN_ONOFF);
	CGxButtonEx* pLoginRequestOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_LOGIN_REQUEST_ONOFF);
	CGxButtonEx* pManualReceive = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_MANUAL_RECEIVE);
	CGxButtonEx* pRCMD = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_RCMD);
	CGxButtonEx* pVIInsp = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_VIINSP_ONOFF);
	//2017-07-06,SKCHO,  고객사 요청으로 DCR 옵션 기능 삭제
	//CGxButtonEx* pDCROnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_DCR_USE);
	CGxButtonEx* pTrayAlignOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_TRAY_ALIGN_USE);
	CGxButtonEx* pContactOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_CONTACT_USE);
	CGxButtonEx* pJudgeOKOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_JUDGEOK_USE);
	CGxButtonEx* pCellInfoSkip = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_CELLINFOSKIP_ONOFF);
	CGxButtonEx* pJobEndTime = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_JOB_END_TIME);
	CGxButtonEx* pUseJobEnd = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_USE_JOB_END);
	CGxButtonEx* pVIGrabEndWaitTime = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_VI_GRAB_END_WAIT_TIME);
	CGxButtonEx* pVIClassEndWaitTime = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_VI_CLASS_END_WAIT_TIME);
	CGxButtonEx* pMaxGoodTrayCount = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_GOOD_TRAY_MAX_COUNT);
	CGxButtonEx* pMaxNGTrayCount = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_NG_TRAY_MAX_COUNT);

	//화상 검사 연속 NG 체크
	CGxButtonEx* pVisionContinuesNGCnt = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_VISION_CONTINUES_NG_COUNT);

	// Zone , Ch Skip
	CGxButtonEx* pASkipOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_ASKIP_ONOFF);
	CGxButtonEx* pBSkipOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_BSKIP_ONOFF);
	CGxButtonEx* pCSkipOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_CSKIP_ONOFF);
	CGxButtonEx* pDSkipOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_DSKIP_ONOFF);
	CGxButtonEx* pACh1SkipOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_ASKIPCH1_ONOFF);
	CGxButtonEx* pBCh1SkipOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_BSKIPCH1_ONOFF);
	CGxButtonEx* pCCh1SkipOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_CSKIPCH1_ONOFF);
	CGxButtonEx* pDCh1SkipOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_DSKIPCH1_ONOFF);
	CGxButtonEx* pACh2SkipOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_ASKIPCH2_ONOFF);
	CGxButtonEx* pBCh2SkipOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_BSKIPCH2_ONOFF);
	CGxButtonEx* pCCh2SkipOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_CSKIPCH2_ONOFF);
	CGxButtonEx* pDCh2SkipOnOff = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_DSKIPCH2_ONOFF);

	CGxButtonEx* pContactRetry = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_CONTACT_RETRY);

	CGxButtonEx* pRepeatInsp = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_REPEAT_INSP);
	//2017-03-23,skcho
	CGxButtonEx* pRunDownWaitTime = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_RUN_DOWN_WAIT_TIME);

	CGxButtonEx* pMCRDelayTime = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_MCR_TIME);

	CGxButtonEx* pConvPitchIn = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_CONV_PITCH_IN);

	CGxButtonEx* pUseMCRGrab = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_MCRGRAB_USE);

	CGxButtonEx* pRejectCode = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_REJECT_CODE);

	//2017-07-20,SKCHO
	CGxButtonEx* pUseDivision = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_DIVISION);


	pLotEndOnOff->GetCaption()==_T("ON")?m_lot_end=TRUE:m_lot_end=FALSE;
	pDryRunOnOff->GetCaption()==_T("ON")?m_dry_run=TRUE:m_dry_run=FALSE;
	pUpperIFOnOff->GetCaption()==_T("ON")?m_UpperIF=TRUE:m_UpperIF=FALSE;
	pUpperMCRReadOnOff->GetCaption()==_T("ON")?m_Uppermcr_read=TRUE:m_Uppermcr_read=FALSE;
	pLowerMCRReadOnOff->GetCaption()==_T("ON")?m_Lowermcr_read=TRUE:m_Lowermcr_read=FALSE;
	pPreAlignOnOff->GetCaption()==_T("ON")?m_pre_align=TRUE:m_pre_align=FALSE;
	pVIAlignOnOff->GetCaption()==_T("ON")?m_Vi_align=TRUE:m_Vi_align=FALSE;
	pEdgeAlingOnOff->GetCaption()==_T("ON")?m_Edge_align=TRUE:m_Edge_align=FALSE;
	pLoginRequestOnOff->GetCaption()==_T("ON")?m_Login_Request=TRUE:m_Login_Request=FALSE;
	pManualReceive->GetCaption()==_T("ON")?m_Manual_Receive=TRUE:m_Manual_Receive=FALSE;
	pRCMD->GetCaption()==_T("ON")?m_RCMD=TRUE:m_RCMD=FALSE;
	pVIInsp->GetCaption()==_T("ON")?m_VIInsp=TRUE:m_VIInsp=FALSE;
	//2017-07-06,SKCHO,  고객사 요청으로 DCR 옵션 기능 삭제
	//pDCROnOff->GetCaption()==_T("ON")?m_DCRUse=TRUE:m_DCRUse=FALSE;
	pTrayAlignOnOff->GetCaption()==_T("ON")?m_TrayAlignUse=TRUE:m_TrayAlignUse=FALSE;
	pContactOnOff->GetCaption()==_T("ON")?m_ContactUse=TRUE:m_ContactUse=FALSE;
	pJudgeOKOnOff->GetCaption()==_T("ON")?m_JudgeOKUse=TRUE:m_JudgeOKUse=FALSE;
	pCellInfoSkip->GetCaption() == _T("ON")?m_CellInfoSkip=TRUE:m_CellInfoSkip=FALSE;
	pUseJobEnd->GetCaption() == _T("ON")?m_bUseJobEnd=TRUE:m_bUseJobEnd=FALSE;

	pASkipOnOff->GetCaption() == _T("ON")?m_bUseAskip=TRUE:m_bUseAskip=FALSE;
	pBSkipOnOff->GetCaption() == _T("ON")?m_bUseBskip=TRUE:m_bUseBskip=FALSE;
	pCSkipOnOff->GetCaption() == _T("ON")?m_bUseCskip=TRUE:m_bUseCskip=FALSE;
	pDSkipOnOff->GetCaption() == _T("ON")?m_bUseDskip=TRUE:m_bUseDskip=FALSE;
	pACh1SkipOnOff->GetCaption() == _T("ON")?m_bUseACh1skip=TRUE:m_bUseACh1skip=FALSE;
	pBCh1SkipOnOff->GetCaption() == _T("ON")?m_bUseBCh1skip=TRUE:m_bUseBCh1skip=FALSE;
	pCCh1SkipOnOff->GetCaption() == _T("ON")?m_bUseCCh1skip=TRUE:m_bUseCCh1skip=FALSE;
	pDCh1SkipOnOff->GetCaption() == _T("ON")?m_bUseDCh1skip=TRUE:m_bUseDCh1skip=FALSE;
	pACh2SkipOnOff->GetCaption() == _T("ON")?m_bUseACh2skip=TRUE:m_bUseACh2skip=FALSE;
	pBCh2SkipOnOff->GetCaption() == _T("ON")?m_bUseBCh2skip=TRUE:m_bUseBCh2skip=FALSE;
	pCCh2SkipOnOff->GetCaption() == _T("ON")?m_bUseCCh2skip=TRUE:m_bUseCCh2skip=FALSE;
	pDCh2SkipOnOff->GetCaption() == _T("ON")?m_bUseDCh2skip=TRUE:m_bUseDCh2skip=FALSE;

	pContactRetry->GetCaption() == _T("ON")?m_bUseContactRetry=TRUE:m_bUseContactRetry=FALSE;

	pConvPitchIn->GetCaption() == _T("ON")?m_bUseConvPitchIn=TRUE:m_bUseConvPitchIn=FALSE;

	pUseMCRGrab->GetCaption() == _T("ON")?m_bUseMCRGrab=TRUE:m_bUseMCRGrab=FALSE;

	//2017-07-20,SKCHO
	pUseDivision->GetCaption() == _T("ON")?m_bDivision=TRUE:m_bDivision=FALSE; 

	m_dJobEndTime = _tstof(pJobEndTime->GetCaption());
	m_dVIGrabEndWaitTime = _tstof(pVIGrabEndWaitTime->GetCaption());
	m_dVIClassEndWaitTime = _tstof(pVIClassEndWaitTime->GetCaption());
	m_nMaxGoodTrayCount = _tstoi(pMaxGoodTrayCount->GetCaption());
	m_nMaxNGTrayCount = _tstoi(pMaxNGTrayCount->GetCaption());
	m_dRunDownWaitTime = _tstof(pRunDownWaitTime->GetCaption());
	m_dMCRDelayTime = _tstof(pMCRDelayTime->GetCaption());

	m_nVisionContinuesNGCnt = _tstoi(pVisionContinuesNGCnt->GetCaption());

	m_strRejectCode = pRejectCode->GetCaption();

	if(m_dry_run!= theConfigBank.m_Option.m_bUseDryRun || m_lot_end!= theConfigBank.m_Option.m_bUseLotEnd
		|| m_Uppermcr_read!= theConfigBank.m_Option.m_bUseUpperMCRRead
		|| m_Lowermcr_read!= theConfigBank.m_Option.m_bUseLowerMCRRead || m_pre_align!= theConfigBank.m_Option.m_bUsePreAlign
		|| m_Vi_align!= theConfigBank.m_Option.m_bUseVIAlign || m_Edge_align!= theConfigBank.m_Option.m_bUseEdgeAlign
		|| m_Login_Request!= theConfigBank.m_Option.m_bUseLoginRequest || m_Manual_Receive!= theConfigBank.m_Option.m_bUseManualReceive
		|| m_RCMD!= theConfigBank.m_Option.m_bUseRCMD || m_VIInsp!= theConfigBank.m_Option.m_bUseVIInsp
		//2017-07-06,SKCHO,  고객사 요청으로 DCR 옵션 기능 삭제
		//|| m_DCRUse!= theConfigBank.m_Option.m_bUseDCR
		|| m_TrayAlignUse!= theConfigBank.m_Option.m_bUseTrayAlign
		|| m_ContactUse!= theConfigBank.m_Option.m_bUseContact || m_JudgeOKUse!= theConfigBank.m_Option.m_bUseJudgeOK
		|| m_CellInfoSkip != theConfigBank.m_Option.m_bUseCellInfoSkip || m_dJobEndTime != theConfigBank.m_Option.m_dJobEndTime
		|| m_bUseJobEnd != theConfigBank.m_Option.m_bUseJobEnd || m_dVIGrabEndWaitTime != theConfigBank.m_Option.m_dGrabEndWaitTime
		|| m_dVIClassEndWaitTime != theConfigBank.m_Option.m_dClassEndWaitTime || m_nMaxGoodTrayCount != theConfigBank.m_Option.m_nMaxGoodTrayCount
		|| m_nMaxNGTrayCount != theConfigBank.m_Option.m_nMaxNGTrayCount || m_bUseAskip != theConfigBank.m_Option.m_bUseASkip
		|| m_bUseBskip != theConfigBank.m_Option.m_bUseBSkip || m_bUseCskip != theConfigBank.m_Option.m_bUseCSkip
		|| m_bUseDskip != theConfigBank.m_Option.m_bUseDSkip || m_bUseACh1skip != theConfigBank.m_Option.m_bUseACh1Skip
		|| m_bUseBCh1skip != theConfigBank.m_Option.m_bUseBCh1Skip || m_bUseCCh1skip != theConfigBank.m_Option.m_bUseCCh1Skip
		|| m_bUseDCh1skip != theConfigBank.m_Option.m_bUseDCh1Skip || m_bUseACh2skip != theConfigBank.m_Option.m_bUseACh2Skip
		|| m_bUseBCh2skip != theConfigBank.m_Option.m_bUseBCh2Skip || m_bUseCCh2skip != theConfigBank.m_Option.m_bUseCCh2Skip
		|| m_bUseDCh2skip != theConfigBank.m_Option.m_bUseDCh2Skip || m_bUseContactRetry != theConfigBank.m_Option.m_bUseContactRetry
		|| m_dRunDownWaitTime != theConfigBank.m_Option.m_dRunDownWaitTime || m_nVisionContinuesNGCnt != theConfigBank.m_Option.m_nVisionContinuesNGCnt
		|| m_dMCRDelayTime != theConfigBank.m_Option.m_dMCRDelayTime || m_bUseRepeatInsp != theConfigBank.m_Option.m_bUseRepeatInsp
		|| m_bUseConvPitchIn != theConfigBank.m_Option.m_bLoadPitchIn || m_bUseMCRGrab != theConfigBank.m_Option.m_bGrabMCRUse
		|| m_strRejectCode != theConfigBank.m_Option.m_strRejectCode
		|| m_bDivision != theConfigBank.m_Option.m_bDivision
		|| m_bSkipLDConvSen3 != theConfigBank.m_Option.m_bSkipLDConvSen3)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}


void CFormOptions::ClickGxbtnOptionManualReceive()
{
	CString strValue;
	CGxButtonEx* pManualReceive = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_MANUAL_RECEIVE);
	strValue = pManualReceive->GetCaption();
	if(strValue==_T("OFF"))
	{
		pManualReceive->SetCaption(_T("ON"));
		pManualReceive->SetBackColor(GXCOLOR_ON);
	}
	else
	{
		pManualReceive->SetCaption(_T("OFF"));
		pManualReceive->SetBackColor(GXCOLOR_OFF);
	}
}


void CFormOptions::ClickGxbtnOptionRCMD()
{
	CString strValue;
	CGxButtonEx* pRCMD = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_RCMD);
	strValue = pRCMD->GetCaption();
	if(strValue==_T("OFF"))
	{
		pRCMD->SetCaption(_T("ON"));
		pRCMD->SetBackColor(GXCOLOR_ON);
	}
	else
	{
		pRCMD->SetCaption(_T("OFF"));
		pRCMD->SetBackColor(GXCOLOR_OFF);
	}
}


void CFormOptions::ClickGxbtnOptionViinspOnoff()
{
	CString strValue;
	CGxButtonEx* pVIInsp = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_VIINSP_ONOFF);
	strValue = pVIInsp->GetCaption();
	if(strValue==_T("OFF"))
	{
		pVIInsp->SetCaption(_T("ON"));
		pVIInsp->SetBackColor(GXCOLOR_ON);
	}
	else
	{
		pVIInsp->SetCaption(_T("OFF"));
		pVIInsp->SetBackColor(GXCOLOR_OFF);
	}
}

// 표시 언어가 변경되었다.
LRESULT CFormOptions::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	CGxUICtrl::BroadcastStaticCpationIndex(this, wParam, IDC_GXSTC_OPTION_TITIE1);
	CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_GXBTN_OPTION_SAVE);

	return 0;
}

void CFormOptions::ClickGxbtnOptionDcrUse()
{
	CString strValue;
	CGxButtonEx* pDCRUse = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_DCR_USE);
	strValue = pDCRUse->GetCaption();
	if(strValue==_T("OFF"))
	{
		pDCRUse->SetCaption(_T("ON"));
		pDCRUse->SetBackColor(GXCOLOR_ON);
	}
	else
	{
		pDCRUse->SetCaption(_T("OFF"));
		pDCRUse->SetBackColor(GXCOLOR_OFF);
	}
}


void CFormOptions::ClickGxbtnOptionTrayAlignUse()
{
	CString strValue;
	CGxButtonEx* pTrayAlignUse = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_TRAY_ALIGN_USE);
	strValue = pTrayAlignUse->GetCaption();
	if(strValue==_T("OFF"))
	{
		pTrayAlignUse->SetCaption(_T("ON"));
		pTrayAlignUse->SetBackColor(GXCOLOR_ON);
	}
	else
	{
		pTrayAlignUse->SetCaption(_T("OFF"));
		pTrayAlignUse->SetBackColor(GXCOLOR_OFF);
	}
}


void CFormOptions::ClickGxbtnOptionContactUse()
{
	CString strValue;
	CGxButtonEx* pContactUse = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_CONTACT_USE);
	strValue = pContactUse->GetCaption();
	if(strValue==_T("OFF"))
	{
		pContactUse->SetCaption(_T("ON"));
		pContactUse->SetBackColor(GXCOLOR_ON);
	}
	else
	{
		pContactUse->SetCaption(_T("OFF"));
		pContactUse->SetBackColor(GXCOLOR_OFF);
	}
}
//
CString m_str1Check = _T("T5_NG");
CString m_str2Check = _T("JIG_NG");
CString m_str3Check = _T("CONTACT_PIN_NG");
CString m_str4Check = _T("TEACHING_NG");
CString m_str5Check = _T("UNIT INTERLOCK");

void CFormOptions::ClickGxbtnOptionAskipOnoff()
{
	CString strValue;
	CGxButtonEx* pAskip = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_ASKIP_ONOFF);
	strValue = pAskip->GetCaption();
	if(strValue==_T("OFF"))
	{
		pAskip->SetCaption(m_str1Check);
		pAskip->SetBackColor(GXCOLOR_ON);
	}
	else if(strValue==m_str1Check)
	{
		pAskip->SetCaption(m_str2Check);
		pAskip->SetBackColor(GXCOLOR_ON);
	}
	else if(strValue==m_str2Check)
	{
		pAskip->SetCaption(m_str3Check);
		pAskip->SetBackColor(GXCOLOR_ON);
	}
	else if(strValue==m_str3Check)
	{
		pAskip->SetCaption(m_str4Check);
		pAskip->SetBackColor(GXCOLOR_ON);
	}
	else if(strValue==m_str4Check)
	{
		pAskip->SetCaption(m_str5Check);
		pAskip->SetBackColor(GXCOLOR_ON);
	}
	else if(strValue==m_str5Check)
	{
		pAskip->SetCaption(_T("OFF"));
		pAskip->SetBackColor(GXCOLOR_OFF);
	}
	else
	{
		pAskip->SetCaption(_T("OFF"));
		pAskip->SetBackColor(GXCOLOR_OFF);
	}
}


void CFormOptions::ClickGxbtnOptionBskipOnoff()
{
	CString strValue;
	CGxButtonEx* pBskip = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_BSKIP_ONOFF);
	strValue = pBskip->GetCaption();
	if(strValue==_T("OFF"))
	{
		pBskip->SetCaption(m_str1Check);
		pBskip->SetBackColor(GXCOLOR_ON);
	}
	else if(strValue==m_str1Check)
	{
		pBskip->SetCaption(m_str2Check);
		pBskip->SetBackColor(GXCOLOR_ON);
	}
	else if(strValue==m_str2Check)
	{
		pBskip->SetCaption(m_str3Check);
		pBskip->SetBackColor(GXCOLOR_ON);
	}
	else if(strValue==m_str3Check)
	{
		pBskip->SetCaption(m_str4Check);
		pBskip->SetBackColor(GXCOLOR_ON);
	}
	else if(strValue==m_str4Check)
	{
		pBskip->SetCaption(m_str5Check);
		pBskip->SetBackColor(GXCOLOR_ON);
	}
	else if(strValue==m_str5Check)
	{
		pBskip->SetCaption(_T("OFF"));
		pBskip->SetBackColor(GXCOLOR_OFF);
	}
	else
	{
		pBskip->SetCaption(_T("OFF"));
		pBskip->SetBackColor(GXCOLOR_OFF);
	}
}


void CFormOptions::ClickGxbtnOptionCskipOnoff()
{
	CString strValue;
	CGxButtonEx* pCskip = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_CSKIP_ONOFF);
	strValue = pCskip->GetCaption();
	if(strValue==_T("OFF"))
	{
		pCskip->SetCaption(m_str1Check);
		pCskip->SetBackColor(GXCOLOR_ON);
	}
	else if(strValue==m_str1Check)
	{
		pCskip->SetCaption(m_str2Check);
		pCskip->SetBackColor(GXCOLOR_ON);
	}
	else if(strValue==m_str2Check)
	{
		pCskip->SetCaption(m_str3Check);
		pCskip->SetBackColor(GXCOLOR_ON);
	}
	else if(strValue==m_str3Check)
	{
		pCskip->SetCaption(m_str4Check);
		pCskip->SetBackColor(GXCOLOR_ON);
	}
	else if(strValue==m_str4Check)
	{
		pCskip->SetCaption(m_str5Check);
		pCskip->SetBackColor(GXCOLOR_ON);
	}
	else if(strValue==m_str5Check)
	{
		pCskip->SetCaption(_T("OFF"));
		pCskip->SetBackColor(GXCOLOR_OFF);
	}
	else
	{
		pCskip->SetCaption(_T("OFF"));
		pCskip->SetBackColor(GXCOLOR_OFF);
	}
}


void CFormOptions::ClickGxbtnOptionDskipOnoff()
{
	CString strValue;
	CGxButtonEx* pDskip = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_DSKIP_ONOFF);
	strValue = pDskip->GetCaption();
	if(strValue==_T("OFF"))
	{
		pDskip->SetCaption(m_str1Check);
		pDskip->SetBackColor(GXCOLOR_ON);
	}
	else if(strValue==m_str1Check)
	{
		pDskip->SetCaption(m_str2Check);
		pDskip->SetBackColor(GXCOLOR_ON);
	}
	else if(strValue==m_str2Check)
	{
		pDskip->SetCaption(m_str3Check);
		pDskip->SetBackColor(GXCOLOR_ON);
	}
	else if(strValue==m_str3Check)
	{
		pDskip->SetCaption(m_str4Check);
		pDskip->SetBackColor(GXCOLOR_ON);
	}
	else if(strValue==m_str4Check)
	{
		pDskip->SetCaption(m_str5Check);
		pDskip->SetBackColor(GXCOLOR_ON);
	}
	else if(strValue==m_str5Check)
	{
		pDskip->SetCaption(_T("OFF"));
		pDskip->SetBackColor(GXCOLOR_OFF);
	}
	else
	{
		pDskip->SetCaption(_T("OFF"));
		pDskip->SetBackColor(GXCOLOR_OFF);
	}
}


void CFormOptions::ClickGxbtnOptionCellinfoskipOnoff()
{
	CString strValue;
	CGxButtonEx* pCellInfoSkip = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_CELLINFOSKIP_ONOFF);
	strValue = pCellInfoSkip->GetCaption();
	if(strValue==_T("OFF"))
	{
		pCellInfoSkip->SetCaption(_T("ON"));
		pCellInfoSkip->SetBackColor(GXCOLOR_ON);
	}
	else
	{
		pCellInfoSkip->SetCaption(_T("OFF"));
		pCellInfoSkip->SetBackColor(GXCOLOR_OFF);
	}
}


void CFormOptions::ClickGxbtnJobEndTime()
{
	CGxButtonEx* pButton = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_JOB_END_TIME);
	double dOldValue = _tstof(pButton->GetCaption());
	CGxNumPadEx	dlgNumPad;

	dlgNumPad.SetTitle(_T("Job End Time Change"));
	dlgNumPad.SetValue(0.0, 100.0, dOldValue);

	int nRet = dlgNumPad.DoModal();

	// 정상적인 입력을 하였다.
	if ( nRet == IDOK )
	{
		m_dJobEndTime = _tstof(dlgNumPad.GetValue());
		pButton->SetCaption(dlgNumPad.GetValue());
	}
}


void CFormOptions::ClickGxbtnUseJobEnd()
{
	CString strValue;
	CGxButtonEx* pUseJobEnd = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_USE_JOB_END);
	strValue = pUseJobEnd->GetCaption();
	if(strValue==_T("OFF"))
	{
		pUseJobEnd->SetCaption(_T("ON"));
		pUseJobEnd->SetBackColor(GXCOLOR_ON);
	}
	else
	{
		pUseJobEnd->SetCaption(_T("OFF"));
		pUseJobEnd->SetBackColor(GXCOLOR_OFF);
	}
}


void CFormOptions::ClickGxbtnOptionJudgeokUse()
{
	CString strValue;
	CGxButtonEx* pJudgeOKUse = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_JUDGEOK_USE);
	strValue = pJudgeOKUse->GetCaption();
	if(strValue==_T("OFF"))
	{
		pJudgeOKUse->SetCaption(_T("ON"));
		pJudgeOKUse->SetBackColor(GXCOLOR_ON);
	}
	else
	{
		pJudgeOKUse->SetCaption(_T("OFF"));
		pJudgeOKUse->SetBackColor(GXCOLOR_OFF);
	}
}


void CFormOptions::ClickGxbtnViGrabEndWaitTime()
{
	CGxButtonEx* pButton = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_VI_GRAB_END_WAIT_TIME);
	double dOldValue = _tstof(pButton->GetCaption());
	CGxNumPadEx	dlgNumPad;

	dlgNumPad.SetTitle(_T("VI Grab End Time Change"));
	dlgNumPad.SetValue(0.0, 100.0, dOldValue);

	int nRet = dlgNumPad.DoModal();

	// 정상적인 입력을 하였다.
	if ( nRet == IDOK )
	{
		m_dVIGrabEndWaitTime = _tstoi(dlgNumPad.GetValue());
		pButton->SetCaption(dlgNumPad.GetValue());
	}
}


void CFormOptions::ClickGxbtnViClassEndWaitTime()
{
	CGxButtonEx* pButton = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_VI_CLASS_END_WAIT_TIME);
	double dOldValue = _tstof(pButton->GetCaption());
	CGxNumPadEx	dlgNumPad;

	dlgNumPad.SetTitle(_T("VI Class End Time Change"));
	dlgNumPad.SetValue(0.0, 100.0, dOldValue);

	int nRet = dlgNumPad.DoModal();

	// 정상적인 입력을 하였다.
	if ( nRet == IDOK )
	{
		m_dVIClassEndWaitTime = _tstoi(dlgNumPad.GetValue());
		pButton->SetCaption(dlgNumPad.GetValue());
	}
}


void CFormOptions::ClickGxbtnGoodTrayMaxCount()
{
	CGxButtonEx* pButton = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_GOOD_TRAY_MAX_COUNT);
	double dOldValue = _tstof(pButton->GetCaption());
	CGxNumPadEx	dlgNumPad;

	dlgNumPad.SetTitle(_T("Good Tray Max Count Change"));
	dlgNumPad.SetValue(0.0, 100.0, dOldValue);

	int nRet = dlgNumPad.DoModal();

	// 정상적인 입력을 하였다.
	if ( nRet == IDOK )
	{
		m_nMaxGoodTrayCount = _tstoi(dlgNumPad.GetValue());
		pButton->SetCaption(dlgNumPad.GetValue());
	}
}


void CFormOptions::ClickGxbtnNgTrayMaxCount()
{
	CGxButtonEx* pButton = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_NG_TRAY_MAX_COUNT);
	double dOldValue = _tstof(pButton->GetCaption());
	CGxNumPadEx	dlgNumPad;

	dlgNumPad.SetTitle(_T("NG Tray Max Count Change"));
	dlgNumPad.SetValue(0.0, 100.0, dOldValue);

	int nRet = dlgNumPad.DoModal();

	// 정상적인 입력을 하였다.
	if ( nRet == IDOK )
	{
		m_nMaxNGTrayCount = _tstoi(dlgNumPad.GetValue());
		pButton->SetCaption(dlgNumPad.GetValue());
	}
}


void CFormOptions::ClickGxbtnOptionAskipch1Onoff()
{
	CString strValue;
	CGxButtonEx* pACh1skip = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_ASKIPCH1_ONOFF);
	strValue = pACh1skip->GetCaption();
	if(strValue==_T("OFF"))
	{
		pACh1skip->SetCaption(_T("ON"));
		pACh1skip->SetBackColor(GXCOLOR_ON);
	}
	else
	{
		pACh1skip->SetCaption(_T("OFF"));
		pACh1skip->SetBackColor(GXCOLOR_OFF);
	}
}


void CFormOptions::ClickGxbtnOptionAskipch2Onoff()
{
	CString strValue;
	CGxButtonEx* pACh2skip = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_ASKIPCH2_ONOFF);
	strValue = pACh2skip->GetCaption();
	if(strValue==_T("OFF"))
	{
		pACh2skip->SetCaption(_T("ON"));
		pACh2skip->SetBackColor(GXCOLOR_ON);
	}
	else
	{
		pACh2skip->SetCaption(_T("OFF"));
		pACh2skip->SetBackColor(GXCOLOR_OFF);
	}
}


void CFormOptions::ClickGxbtnOptionBskipch1Onoff()
{
	CString strValue;
	CGxButtonEx* pBCh1skip = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_BSKIPCH1_ONOFF);
	strValue = pBCh1skip->GetCaption();
	if(strValue==_T("OFF"))
	{
		pBCh1skip->SetCaption(_T("ON"));
		pBCh1skip->SetBackColor(GXCOLOR_ON);
	}
	else
	{
		pBCh1skip->SetCaption(_T("OFF"));
		pBCh1skip->SetBackColor(GXCOLOR_OFF);
	}
}


void CFormOptions::ClickGxbtnOptionBskipch2Onoff()
{
	CString strValue;
	CGxButtonEx* pBCh2skip = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_BSKIPCH2_ONOFF);
	strValue = pBCh2skip->GetCaption();
	if(strValue==_T("OFF"))
	{
		pBCh2skip->SetCaption(_T("ON"));
		pBCh2skip->SetBackColor(GXCOLOR_ON);
	}
	else
	{
		pBCh2skip->SetCaption(_T("OFF"));
		pBCh2skip->SetBackColor(GXCOLOR_OFF);
	}
}


void CFormOptions::ClickGxbtnOptionCskipch1Onoff()
{
	CString strValue;
	CGxButtonEx* pCCh1skip = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_CSKIPCH1_ONOFF);
	strValue = pCCh1skip->GetCaption();
	if(strValue==_T("OFF"))
	{
		pCCh1skip->SetCaption(_T("ON"));
		pCCh1skip->SetBackColor(GXCOLOR_ON);
	}
	else
	{
		pCCh1skip->SetCaption(_T("OFF"));
		pCCh1skip->SetBackColor(GXCOLOR_OFF);
	}
}


void CFormOptions::ClickGxbtnOptionCskipch2Onoff()
{
	CString strValue;
	CGxButtonEx* pCCh2skip = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_CSKIPCH2_ONOFF);
	strValue = pCCh2skip->GetCaption();
	if(strValue==_T("OFF"))
	{
		pCCh2skip->SetCaption(_T("ON"));
		pCCh2skip->SetBackColor(GXCOLOR_ON);
	}
	else
	{
		pCCh2skip->SetCaption(_T("OFF"));
		pCCh2skip->SetBackColor(GXCOLOR_OFF);
	}
}


void CFormOptions::ClickGxbtnOptionDskipch1Onoff()
{
	CString strValue;
	CGxButtonEx* pDCh1skip = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_DSKIPCH1_ONOFF);
	strValue = pDCh1skip->GetCaption();
	if(strValue==_T("OFF"))
	{
		pDCh1skip->SetCaption(_T("ON"));
		pDCh1skip->SetBackColor(GXCOLOR_ON);
	}
	else
	{
		pDCh1skip->SetCaption(_T("OFF"));
		pDCh1skip->SetBackColor(GXCOLOR_OFF);
	}
}


void CFormOptions::ClickGxbtnOptionDskipch2Onoff()
{
	CString strValue;
	CGxButtonEx* pDCh2skip = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_DSKIPCH2_ONOFF);
	strValue = pDCh2skip->GetCaption();
	if(strValue==_T("OFF"))
	{
		pDCh2skip->SetCaption(_T("ON"));
		pDCh2skip->SetBackColor(GXCOLOR_ON);
	}
	else
	{
		pDCh2skip->SetCaption(_T("OFF"));
		pDCh2skip->SetBackColor(GXCOLOR_OFF);
	}
}


void CFormOptions::ClickGxbtnOptionContactRetry()
{
	CString strValue;
	CGxButtonEx* pContactRetry = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_CONTACT_RETRY);
	strValue = pContactRetry->GetCaption();
	if(strValue==_T("OFF"))
	{
		pContactRetry->SetCaption(_T("ON"));
		pContactRetry->SetBackColor(GXCOLOR_ON);
	}
	else
	{
		pContactRetry->SetCaption(_T("OFF"));
		pContactRetry->SetBackColor(GXCOLOR_OFF);
	}
}


void CFormOptions::ClickGxbtnRunDownWaitTime()
{
	CGxButtonEx* pButton = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_RUN_DOWN_WAIT_TIME);
	double dOldValue = _tstof(pButton->GetCaption());
	CGxNumPadEx	dlgNumPad;

	dlgNumPad.SetTitle(_T("Run Down Wait Time Change"));
	dlgNumPad.SetValue(0.0, 3600.0, dOldValue);

	int nRet = dlgNumPad.DoModal();

	// 정상적인 입력을 하였다.
	if ( nRet == IDOK )
	{
		m_dRunDownWaitTime = _tstof(dlgNumPad.GetValue());
		pButton->SetCaption(dlgNumPad.GetValue());
	}
}

void CFormOptions::ClickGxbtnOptionCarrierReport()
{
	CString strValue;
	CGxButtonEx* pCarrierReportSkip = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_CARRIER_REPORT);
	strValue = pCarrierReportSkip->GetCaption();
	if(strValue==_T("OFF"))
	{
		pCarrierReportSkip->SetCaption(_T("ON"));
		pCarrierReportSkip->SetBackColor(GXCOLOR_ON);
	}
	else
	{
		pCarrierReportSkip->SetCaption(_T("OFF"));
		pCarrierReportSkip->SetBackColor(GXCOLOR_OFF);
	}
}

void CFormOptions::ClickGxbtnOptionRepeatInsp()
{
	CString strValue;
	CGxButtonEx* pRepeatInsp = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_REPEAT_INSP);
	strValue = pRepeatInsp->GetCaption();
	if(strValue==_T("OFF"))
	{
		pRepeatInsp->SetCaption(_T("ON"));
		pRepeatInsp->SetBackColor(GXCOLOR_ON);
	}
	else
	{
		pRepeatInsp->SetCaption(_T("OFF"));
		pRepeatInsp->SetBackColor(GXCOLOR_OFF);
	}
}


void CFormOptions::ClickGxbtnVisionContinuesNgCount()
{
	CGxButtonEx* pButton = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_VISION_CONTINUES_NG_COUNT);
	double dOldValue = _tstof(pButton->GetCaption());
	CGxNumPadEx	dlgNumPad;

	dlgNumPad.SetTitle(_T("Vision Continues NG Count Change"));
	dlgNumPad.SetValue(0.0, 100.0, dOldValue);

	int nRet = dlgNumPad.DoModal();

	// 정상적인 입력을 하였다.
	if ( nRet == IDOK )
	{
		m_nVisionContinuesNGCnt = _tstoi(dlgNumPad.GetValue());
		pButton->SetCaption(dlgNumPad.GetValue());
	}
}

void CFormOptions::ClickGxbtnMcrTime()
{
	CGxButtonEx* pButton = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_MCR_TIME);
	double dOldValue = _tstof(pButton->GetCaption());
	CGxNumPadEx	dlgNumPad;

	dlgNumPad.SetTitle(_T("MCR Delay Time Change"));
	dlgNumPad.SetValue(0.0, 1.0, dOldValue);

	int nRet = dlgNumPad.DoModal();

	// 정상적인 입력을 하였다.
	if ( nRet == IDOK )
	{
		m_dMCRDelayTime = _tstof(dlgNumPad.GetValue());
		pButton->SetCaption(dlgNumPad.GetValue());
	}
}


void CFormOptions::ClickGxbtnOptionConvPitchIn()
{
	CString strValue;
	CGxButtonEx* pConvPitchIn = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_CONV_PITCH_IN);
	strValue = pConvPitchIn->GetCaption();
	if(strValue==_T("OFF"))
	{
		pConvPitchIn->SetCaption(_T("ON"));
		pConvPitchIn->SetBackColor(GXCOLOR_ON);
	}
	else
	{
		pConvPitchIn->SetCaption(_T("OFF"));
		pConvPitchIn->SetBackColor(GXCOLOR_OFF);
	}
}


void CFormOptions::ClickGxbtnOptionMcrgrabUse()
{
	CString strValue;
	CGxButtonEx* pUseMCRGrab = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_MCRGRAB_USE);
	strValue = pUseMCRGrab->GetCaption();
	if(strValue==_T("OFF"))
	{
		pUseMCRGrab->SetCaption(_T("ON"));
		pUseMCRGrab->SetBackColor(GXCOLOR_ON);
	}
	else
	{
		pUseMCRGrab->SetCaption(_T("OFF"));
		pUseMCRGrab->SetBackColor(GXCOLOR_OFF);
	}
}


void CFormOptions::ClickGxbtnTrayidCount()
{
	CGxButtonEx* pButton = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_TRAYID_COUNT);
	int nOldValue = _tstoi(pButton->GetCaption());
	CGxNumPadEx	dlgNumPad;

	dlgNumPad.SetTitle(_T("TRAY ID Count Change"));
	dlgNumPad.SetValue(0, 100, nOldValue);

	int nRet = dlgNumPad.DoModal();

	// 정상적인 입력을 하였다.
	if ( nRet == IDOK )
	{
		m_nTrayIDCount = _tstoi(dlgNumPad.GetValue());
		pButton->SetCaption(dlgNumPad.GetValue());
	}
}


void CFormOptions::ClickGxbtnMcrCompchk()
{	
	//2017-10-13,SKCHO
	//theProcBank.m_bUpperMCRCompChk =FALSE;
	theProcBank.m_bUpperMCRCompChk = TRUE;
}


void CFormOptions::ClickGxbtnOptionDivision()
{
	CString strValue;
	CGxButtonEx* pUseDivision = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_DIVISION);
	strValue = pUseDivision->GetCaption();
	if(strValue==_T("OFF"))
	{
		pUseDivision->SetCaption(_T("ON"));
		pUseDivision->SetBackColor(GXCOLOR_ON);
	}
	else
	{
		pUseDivision->SetCaption(_T("OFF"));
		pUseDivision->SetBackColor(GXCOLOR_OFF);
	}
}

// 2017-08-14 KAKAKA
void CFormOptions::ClickGxbtnOptionNgin1Onoff()
{
	CString strValue;
	CGxButtonEx* pUseNGIN1 = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_NGIN1_ONOFF);
	strValue = pUseNGIN1->GetCaption();
	if(strValue==_T("OFF"))
	{
		pUseNGIN1->SetCaption(_T("ON"));
		pUseNGIN1->SetBackColor(GXCOLOR_ON);
		m_NGINLIFTSEN1 = TRUE;
	}
	else
	{
		pUseNGIN1->SetCaption(_T("OFF"));
		pUseNGIN1->SetBackColor(GXCOLOR_OFF);
		m_NGINLIFTSEN1 = FALSE;
	}
}


void CFormOptions::ClickGxbtnOptionNgin2Onoff()
{
	CString strValue;
	CGxButtonEx* pUseNGIN2 = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_NGIN2_ONOFF);
	strValue = pUseNGIN2->GetCaption();
	if(strValue==_T("OFF"))
	{
		pUseNGIN2->SetCaption(_T("ON"));
		pUseNGIN2->SetBackColor(GXCOLOR_ON);
		m_NGINLIFTSEN2 = TRUE;
	}
	else
	{
		pUseNGIN2->SetCaption(_T("OFF"));
		pUseNGIN2->SetBackColor(GXCOLOR_OFF);
		m_NGINLIFTSEN2 = FALSE;
	}
}


void CFormOptions::ClickGxbtnOptionNgout1Onoff()
{
	CString strValue;
	CGxButtonEx* pUseNGOUT1 = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_NGOUT1_ONOFF);
	strValue = pUseNGOUT1->GetCaption();
	if(strValue==_T("OFF"))
	{
		pUseNGOUT1->SetCaption(_T("ON"));
		pUseNGOUT1->SetBackColor(GXCOLOR_ON);
		m_NGOUTLIFTSEN1 = TRUE;
	}
	else
	{
		pUseNGOUT1->SetCaption(_T("OFF"));
		pUseNGOUT1->SetBackColor(GXCOLOR_OFF);
		m_NGOUTLIFTSEN1 = FALSE;
	}
}


void CFormOptions::ClickGxbtnOptionNgout2Onoff()
{
	CString strValue;
	CGxButtonEx* pUseNGOUT2 = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_NGOUT2_ONOFF);
	strValue = pUseNGOUT2->GetCaption();
	if(strValue==_T("OFF"))
	{
		pUseNGOUT2->SetCaption(_T("ON"));
		pUseNGOUT2->SetBackColor(GXCOLOR_ON);
		m_NGOUTLIFTSEN2 = TRUE;
	}
	else
	{
		pUseNGOUT2->SetCaption(_T("OFF"));
		pUseNGOUT2->SetBackColor(GXCOLOR_OFF);
		m_NGOUTLIFTSEN2 = FALSE;
	}
}

void CFormOptions::ClickGxbtnAgvDelaytime()
{
	CGxButtonEx* pButton = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_AGV_DELAYTIME);
	int nOldValue = _tstoi(pButton->GetCaption());
	CGxNumPadEx	dlgNumPad;

	dlgNumPad.SetTitle(_T("AGV Delay Time(Minute)"));
	dlgNumPad.SetValue(0, 30, nOldValue);

	int nRet = dlgNumPad.DoModal();

	// 정상적인 입력을 하였다.
	if ( nRet == IDOK )
	{
		m_nAGVDelayTime = _tstoi(dlgNumPad.GetValue());
		pButton->SetCaption(dlgNumPad.GetValue());
	}
}


void CFormOptions::ClickGxbtnOptionLdconvsen3skip()
{
	CString strValue;
	CGxButtonEx* pLDCONVSEN3 = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_LDCONVSEN3SKIP);
	strValue = pLDCONVSEN3->GetCaption();
	if(strValue==_T("OFF"))
	{
		pLDCONVSEN3->SetCaption(_T("ON"));
		pLDCONVSEN3->SetBackColor(GXCOLOR_ON);
	}
	else
	{
		pLDCONVSEN3->SetCaption(_T("OFF"));
		pLDCONVSEN3->SetBackColor(GXCOLOR_OFF);
	}
}


void CFormOptions::ClickGxbtnLdtrblowerWaitTime()
{
	CGxButtonEx* pButton = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LDTRBLOWER_WAIT_TIME);
	double nOldValue = _tstof(pButton->GetCaption());
	CGxNumPadEx	dlgNumPad;

	dlgNumPad.SetTitle(_T("LD TR BlowerOff Wait Time(sec)"));
	dlgNumPad.SetValue(0.0, 3.0, nOldValue);

	int nRet = dlgNumPad.DoModal();

	// 정상적인 입력을 하였다.
	if ( nRet == IDOK )
	{
		m_dLDTRBlowerOffTime = _tstof(dlgNumPad.GetValue());
		pButton->SetCaption(dlgNumPad.GetValue());
	}
}



void CFormOptions::ClickGxbtnOptionUldtrmode()
{
	CString strValue;
	CGxButtonEx* pUseULDTRMode = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_ULDTRMODE);
	strValue = pUseULDTRMode->GetCaption();
	if(strValue==_T("OFF"))
	{
		pUseULDTRMode->SetCaption(_T("ON"));
		pUseULDTRMode->SetBackColor(GXCOLOR_ON);	
	}
	else
	{
		pUseULDTRMode->SetCaption(_T("OFF"));
		pUseULDTRMode->SetBackColor(GXCOLOR_OFF);		
	}
}


void CFormOptions::ClickGxbtnT5RestartTime()
{
	CGxButtonEx* pButton = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_T5_RESTART_TIME);
	int nOldValue = _tstoi(pButton->GetCaption());
	CGxNumPadEx	dlgNumPad;

	dlgNumPad.SetTitle(_T("T5 Restart Time(Hour)"));
	dlgNumPad.SetValue(0, 100, nOldValue);

	int nRet = dlgNumPad.DoModal();

	// 정상적인 입력을 하였다.
	if ( nRet == IDOK )
	{
		m_nT5RestartTime = _tstoi(dlgNumPad.GetValue());
		pButton->SetCaption(dlgNumPad.GetValue());
	}
}


void CFormOptions::ClickGxbtnOptionPorttrmode()
{
	CGxButtonEx* pButton = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_GOODTRLENGTH);
	double dOldValue = _tstof(pButton->GetCaption());
	CGxNumPadEx	dlgNumPad;

	dlgNumPad.SetTitle(_T("Good TR Retry Length(mm)"));
	dlgNumPad.SetValue(0.0, 5.1, dOldValue);

	int nRet = dlgNumPad.DoModal();

	// 정상적인 입력을 하였다.
	if ( nRet == IDOK )
	{
		m_dGoodTRRetryLength = _tstof(dlgNumPad.GetValue());
		if(m_dGoodTRRetryLength == 0.0)
		{
			pButton->SetCaption(_T("0FF"));
			pButton->SetBackColor(GXCOLOR_OFF);
		}
		else
		{
			pButton->SetCaption(dlgNumPad.GetValue());
			pButton->SetBackColor(GXCOLOR_ON);
		}
	}
	
}


void CFormOptions::ClickGxbtnOptionNgtrlength()
{
	CGxButtonEx* pButton = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_NGTRLENGTH);
	double dOldValue = _tstof(pButton->GetCaption());
	CGxNumPadEx	dlgNumPad;

	dlgNumPad.SetTitle(_T("Ng TR Retry Length(mm)"));
	dlgNumPad.SetValue(0.0, 5.1, dOldValue);

	int nRet = dlgNumPad.DoModal();

	// 정상적인 입력을 하였다.
	if ( nRet == IDOK )
	{
		m_dNGTRRetryLength = _tstof(dlgNumPad.GetValue());
		if(m_dNGTRRetryLength == 0.0)
		{
			pButton->SetCaption(_T("0FF"));
			pButton->SetBackColor(GXCOLOR_OFF);
		}
		else
		{
			pButton->SetCaption(dlgNumPad.GetValue());
			pButton->SetBackColor(GXCOLOR_ON);
		}
	}
}


void CFormOptions::ClickGxbtnOptionLdrobotRetry()
{
	
	CString strValue;
	CGxButtonEx* pLDRobot = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_LDROBOT_RETRY);
	strValue = pLDRobot->GetCaption();
	if(strValue==_T("OFF"))
	{
		pLDRobot->SetCaption(_T("ON"));
		pLDRobot->SetBackColor(GXCOLOR_ON);	
	}
	else
	{
		pLDRobot->SetCaption(_T("OFF"));
		pLDRobot->SetBackColor(GXCOLOR_OFF);		
	}
}


void CFormOptions::ClickGxbtnOptionTrayvi()
{
	CString strValue;
	CGxButtonEx* pTrayVI = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_TRAYVI);
	strValue = pTrayVI->GetCaption();
	if(strValue==_T("OFF"))
	{
		pTrayVI->SetCaption(_T("ON"));
		pTrayVI->SetBackColor(GXCOLOR_ON);	
	}
	else
	{
		pTrayVI->SetCaption(_T("OFF"));
		pTrayVI->SetBackColor(GXCOLOR_OFF);		
	}
}


void CFormOptions::ClickGxbtnOptionPatrRetry()
{
	CString strValue;
	CGxButtonEx* pPATR = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_PATR_RETRY);
	strValue = pPATR->GetCaption();
	if(strValue==_T("OFF"))
	{
		pPATR->SetCaption(_T("ON"));
		pPATR->SetBackColor(GXCOLOR_ON);	
	}
	else
	{
		pPATR->SetCaption(_T("OFF"));
		pPATR->SetBackColor(GXCOLOR_OFF);		
	}
}


void CFormOptions::ClickGxbtnOptionJigblow()
{
	CString strValue;
	CGxButtonEx* pJigBlow = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_JIGBLOW);
	strValue = pJigBlow->GetCaption();
	if(strValue==_T("OFF"))
	{
		pJigBlow->SetCaption(_T("ON"));
		pJigBlow->SetBackColor(GXCOLOR_ON);	
	}
	else
	{
		pJigBlow->SetCaption(_T("OFF"));
		pJigBlow->SetBackColor(GXCOLOR_OFF);		
	}
}


void CFormOptions::ClickGxbtnPaOrgTime()
{
	
	CGxButtonEx* pButton = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_PA_ORG_TIME);
	int nOldValue = _tstoi(pButton->GetCaption());
	CGxNumPadEx	dlgNumPad;

	dlgNumPad.SetTitle(_T("PA ORG Interval Time(Hour)"));
	dlgNumPad.SetValue(0, 100, nOldValue);

	int nRet = dlgNumPad.DoModal();

	// 정상적인 입력을 하였다.
	if ( nRet == IDOK )
	{
		m_nPAOrgTime = _tstoi(dlgNumPad.GetValue());
		pButton->SetCaption(dlgNumPad.GetValue());
	}
}


//2018-01-20,GHLEE
void CFormOptions::ClickGxbtnOptionFPCBSen()
{
	CString strValue;
	CGxButtonEx* pFPCBSen = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_FPCBSEN);
	strValue = pFPCBSen->GetCaption();
	if(strValue==_T("OFF"))
	{
		pFPCBSen->SetCaption(_T("ON"));
		pFPCBSen->SetBackColor(GXCOLOR_ON);	
	}
	else
	{
		pFPCBSen->SetCaption(_T("OFF"));
		pFPCBSen->SetBackColor(GXCOLOR_OFF);		
	}
}


//2018-03-16,GHLEE, FINAL SENSOR OPTION 켤 때 DIVISION OPTION 켜져있는지 확인(베트남어는 아직)
void CFormOptions::ClickGxbtnOptionGoodInFinalSen()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(theConfigBank.m_Option.m_bDivision == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(TRUE, 
		_T("이 옵션을 사용하려면 DIVISION 옵션을 ON 시켜주세요"), 
		_T("If you want change this option, you should change DIVISION OPTION ON"),		
		_T("If you want change this option, you should change DIVISION OPTION ON") );

		int nRet = dlgMsgBox.DoModal();
		if(nRet != IDOK)
			return;
	}
	else
	{
		CString strValue;
		CGxButtonEx* pGoodInFinalSen = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_GOODINFINALSEN);
		strValue = pGoodInFinalSen->GetCaption();
		if(strValue==_T("OFF"))
		{
			pGoodInFinalSen->SetCaption(_T("ON"));
			pGoodInFinalSen->SetBackColor(GXCOLOR_ON);	
		}
		else
		{
			pGoodInFinalSen->SetCaption(_T("OFF"));
			pGoodInFinalSen->SetBackColor(GXCOLOR_OFF);		
		}
	}
}




void CFormOptions::ClickGxbtnContactDelayTime()
{
	CGxButtonEx* pContactButton = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_CONTACT_DELAY_TIME);
	int nOldValue = _tstoi(pContactButton->GetCaption());
	CGxNumPadEx	dlgNumPad;

	dlgNumPad.SetTitle(_T("CONTACT DELAY (Second)"));
	dlgNumPad.SetValue(1, 10, nOldValue);

	int nRet = dlgNumPad.DoModal();

	// 정상적인 입력을 하였다.
	if ( nRet == IDOK )
	{
		m_nContactDelay  = _tstoi(dlgNumPad.GetValue());
		pContactButton->SetCaption(dlgNumPad.GetValue());
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

//2018-06-29,NAM, Add Conveyor Mode 
void CFormOptions::ClickGxbtnOptionConveyormode()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString strValue;
	CGxButtonEx* pConveyorMode = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_OPTION_CONVEYORMODE);
	strValue = pConveyorMode->GetCaption();
	if(strValue==_T("OFF"))
	{
		pConveyorMode->SetCaption(_T("ON"));
		pConveyorMode->SetBackColor(GXCOLOR_ON);	
	}
	else
	{
		pConveyorMode->SetCaption(_T("OFF"));
		pConveyorMode->SetBackColor(GXCOLOR_OFF);		
	}
}

//2018-06-29,NAM, Add Conveyor Delay Time 
void CFormOptions::ClickGxbtnConveyorDelayTime()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CGxButtonEx* pConveyorDelay = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_CONVEYOR_DELAY_TIME);
	int nOldValue = _tstoi(pConveyorDelay->GetCaption());
	CGxNumPadEx	dlgNumPad;

	dlgNumPad.SetTitle(_T("CONVEYOR DELAY TIME (Second)"));
	dlgNumPad.SetValue(1, 600, nOldValue);

	int nRet = dlgNumPad.DoModal();

	// 정상적인 입력을 하였다.
	if ( nRet == IDOK )
	{
		m_nConveyorDelay  = _tstoi(dlgNumPad.GetValue());
		pConveyorDelay->SetCaption(dlgNumPad.GetValue());
	}
}

//2018wjp
void CFormOptions::ClickSetUseBuffer()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString strValue;
	CGxButtonEx* pUseBufferMode = (CGxButtonEx*)GetDlgItem(IDC_MODEL_BUFFER_USE);
	strValue = pUseBufferMode->GetCaption();
	if(strValue==_T("OFF"))
	{
		pUseBufferMode->SetCaption(_T("ON"));
		pUseBufferMode->SetBackColor(GXCOLOR_ON);	
	}
	else
	{
		pUseBufferMode->SetCaption(_T("OFF"));
		pUseBufferMode->SetBackColor(GXCOLOR_OFF);		
	}
}

void CFormOptions::ClickSetUseConveyorTestModel()
{
	CString strValue;
	CGxButtonEx* pUseBufferMode = (CGxButtonEx*)GetDlgItem(IDC_CONVEYOR_TEST_MODEL_SET);
	strValue = pUseBufferMode->GetCaption();
	if(strValue==_T("OFF"))
	{
		pUseBufferMode->SetCaption(_T("ON"));
		pUseBufferMode->SetBackColor(GXCOLOR_ON);	
	}
	else
	{
		pUseBufferMode->SetCaption(_T("OFF"));
		pUseBufferMode->SetBackColor(GXCOLOR_OFF);		
	}
}

void CFormOptions::ClickSetUseBufferWaitTime()
{
	CGxButtonEx* pContactButton = (CGxButtonEx*)GetDlgItem(IDC_MODEL_BUFFER_WAIT_TIME);
	int nOldValue = _tstoi(pContactButton->GetCaption());
	CGxNumPadEx	dlgNumPad;

	dlgNumPad.SetTitle(_T("Set Buffer Wait Time (Second)"));
	dlgNumPad.SetValue(30, 99999, nOldValue);

	int nRet = dlgNumPad.DoModal();

	// 정상적인 입력을 하였다.
	if ( nRet == IDOK )
	{
		m_nBufferUseWaitTime  = _tstoi(dlgNumPad.GetValue());
		pContactButton->SetCaption(dlgNumPad.GetValue());
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
void CFormOptions::ClickUseBufferTrayZUpDis()
{
	CGxButtonEx* pContactButton = (CGxButtonEx*)GetDlgItem(IDC_MODEL_BUFFER_Z_DIS);
	int nOldValue = _tstoi(pContactButton->GetCaption());
	CGxNumPadEx	dlgNumPad;

	dlgNumPad.SetTitle(_T("Set Buffer Z Axis Up Distance (mm)"));
	dlgNumPad.SetValue(0, 10, nOldValue);

	int nRet = dlgNumPad.DoModal();

	// 정상적인 입력을 하였다.
	if ( nRet == IDOK )
	{
		m_nBufferUseTrayDownZ  = _tstoi(dlgNumPad.GetValue());
		pContactButton->SetCaption(dlgNumPad.GetValue());
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
