#pragma once

//////////////////////////////////////////////////////////////////////////
// CConfigOption
// 1. 기본적인 Option Parameter Class
// 2. 딱히 그룹화하기 애매한 것들은 다 여기 모아놓는다
#include <vector>
#include <map>
using namespace std;
class CConfigOption
{
public:
	CConfigOption(void);
	~CConfigOption(void);

	int	m_OffsetHour;
	int	m_AGVWaitTime;
	int m_UpperIFWaitTime;
	
	BOOL m_bUseDryRun;		// Panel 없이 돌려보는 Mode
	BOOL m_bUseLotEnd;
	BOOL m_bUseUpperMCRRead;		
	BOOL m_bUseLowerMCRRead;		
	BOOL m_bUseEdgeAlign;
	BOOL m_bUseVIAlign;
	BOOL m_bUsePreAlign;
	BOOL m_bUseLoginRequest;
	BOOL m_bUseManualReceive;
	BOOL m_bUseRCMD;
	BOOL m_bUseVIInsp;
	//2017-07-06,SKCHO,  고객사 요청으로 DCR 옵션 기능 삭제
	//BOOL m_bUseDCR;
	BOOL m_bUseTrayAlign;
	BOOL m_bUseContact;
	BOOL m_bUseJudgeOK;
	//20170210 byskcho
	BOOL m_bUseCellInfoSkip;
	BOOL m_bUseCh1ReqByPass; //상위 통신 오류 났을 경우 바이패스
	BOOL m_bUseCh2ReqByPass;

	BOOL m_bUseCh1RcmdByPass; //상위 통신 오류 났을 경우 바이패스
	BOOL m_bUseCh2RcmdByPass;

	int m_nMaxGoodTrayCount;	// 적재 가능한 Tray 수량. 이 수량만큼 차면 배출
	int m_nMaxNGTrayCount;	// 적재 가능한 Tray 수량. 이 수량만큼 차면 배출

	// 2017.2.8 bgkim
	double m_dJobEndTime;
	BOOL m_bUseJobEnd; // Use Job End 사용 유무

	double m_dGrabEndWaitTime;

	double m_dClassEndWaitTime;

	double m_dRunDownWaitTime; //2017-03-23,skcho , idle 상태로 변환하기 위해 기다리는 시간 

	double m_dMCRDelayTime;
	// 화상 검사 연속 NG 체크 카운트
	int m_nVisionContinuesNGCnt;
	// Zone , Ch Skip
	BOOL m_bUseASkip;
	BOOL m_bUseBSkip;
	BOOL m_bUseCSkip;
	BOOL m_bUseDSkip;

	//2017-11-01,SKCHO
	BOOL m_bOldUseASkip;
	BOOL m_bOldUseBSkip;
	BOOL m_bOldUseCSkip;
	BOOL m_bOldUseDSkip;
	
	//2017-09-19,SKCHO
	CString m_strASkipDescription;
	CString m_strBSkipDescription;
	CString m_strCSkipDescription;
	CString m_strDSkipDescription;

	BOOL m_bUseACh1Skip;
	BOOL m_bUseBCh1Skip;
	BOOL m_bUseCCh1Skip;
	BOOL m_bUseDCh1Skip;
	BOOL m_bUseACh2Skip;
	BOOL m_bUseBCh2Skip;
	BOOL m_bUseCCh2Skip;
	BOOL m_bUseDCh2Skip;

	BOOL m_bUseContactRetry;

	BOOL m_bUseCarrierReportSkip;

	BOOL m_bUseRepeatInsp;

	// Load Pitch In 이동 옵션으로 추가. 2017/06/09 HSK.
	BOOL m_bLoadPitchIn;

	// 상위로 보고 하는 불량 L Or R 2017/06/15 HSK.
	CString m_strRejectCode;

	// 그랩 모드 변경 옵션 추가 2017/06/15 HSK.
	BOOL m_bGrabMCRUse;

	//2017-07-20,SKCHO
	BOOL m_bDivision;
	//2017-07-10,SKCHO
	int m_nTrayIDCount;
	int m_nAGVDelayTime;
	double m_dLDTRBlowerOffTime;

	double m_dCellInfoWaitTime;
	double m_dCarrierInfoWaitTime;

	int m_nT5RestartTime;
	double m_dGoodTRRetryLength;
	double m_dNGTRRetryLength;

	//2017-08-14 KAKAKA
	BOOL m_NGINLIFTSEN1;
	BOOL m_NGINLIFTSEN2;
	BOOL m_NGOUTLIFTSEN1;
	BOOL m_NGOUTLIFTSEN2;

	//2017-09-01,SKCHO
	BOOL m_bSkipLDConvSen3;
	//2017--9-19,SKCHO
	BOOL m_bUseULDTRMode;

	BOOL m_bUseLDRobotRetry;  //LD ROBOT RETRY 사용여부

	//2017-10-26,JSJUNG
	BOOL m_bUseTrayVI;

	BOOL m_bUsePATRRetry;  //2017-11-17,SKCHO PATR VAC RETRY

	//2017-12-23,SKCHO
	BOOL m_bUseJigBlow; 
	int  m_nPaOrgTime;

	//2018-05-11,NAM
	int m_nContactDelay;

	//2018-06-29,NAM
	int m_nConveyorDelay;

	//2018-06-29,NAM
	BOOL m_bUseConveyorMode;

	//2018-01-20,GHLEE
	BOOL m_bUseFPCBSen;

	//2018-03-15,GHLEE
	BOOL m_bGoodInFinalSen;

	//2018wjp
	BOOL m_bUseBuffer;
	int  m_nBufferUseWaitTime;
	double  m_nBufferUseTrayDownZ;
	BOOL m_bUseConveyorTestModel;

	map<STATUS_LAMP_DESCRIPTION, LAMP_SETTING_LIST> iMapLampSettingList;

	void ReadOptionFile(CString strPath);
	void SaveOptionFile(CString strPath, CString strBackupPath);

		
	double m_dFDCRange[POWER_INDEX_MAX];

};

