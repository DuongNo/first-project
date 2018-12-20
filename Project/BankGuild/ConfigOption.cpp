#include "StdAfx.h"
#include "ConfigOption.h"
#include "Etc/Ini.h"
#include "Etc/FileSupport.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CConfigOption::CConfigOption(void)
{
	m_bUseCh1ReqByPass = FALSE;
	m_bUseCh2ReqByPass = FALSE;
	m_bUseCh1RcmdByPass = FALSE;
	m_bUseCh2RcmdByPass = FALSE;

//2017-11-01,SKCHO
	m_bOldUseASkip = FALSE;
	m_bOldUseBSkip = FALSE;
	m_bOldUseCSkip = FALSE;
	m_bOldUseDSkip = FALSE;
	m_bUseConveyorTestModel=FALSE;
}


CConfigOption::~CConfigOption(void)
{
}

void CConfigOption::ReadOptionFile( CString strPath )
{
	CIni ini(strPath);

	m_OffsetHour				= ini.GetLong(_T("OPTION")		, _T("m_OffsetHour")		, 7);
	m_AGVWaitTime				= ini.GetLong(_T("OPTION")		, _T("m_AGVWaitTime")		, 1800);
	m_UpperIFWaitTime			= ini.GetLong(_T("OPTION")		, _T("m_UpperIFWaitTime")	, 60);

	m_bUseDryRun				= ini.GetBoolean(_T("OPTION")	, _T("m_bUseDryRun")		, FALSE);
	m_bUseLotEnd				= ini.GetBoolean(_T("OPTION")	, _T("m_bUseLotEnd")		, FALSE);
	m_bUseUpperMCRRead			= ini.GetBoolean(_T("OPTION")	, _T("m_bUseUpperMCRRead")		, FALSE);
	m_bUseLowerMCRRead			= ini.GetBoolean(_T("OPTION")	, _T("m_bUseLowerMCRRead")		, FALSE);
	m_bUseEdgeAlign				= ini.GetBoolean(_T("OPTION")	, _T("m_bUseEdgeAlign")		, FALSE);
	m_bUseVIAlign				= ini.GetBoolean(_T("OPTION")	, _T("m_bUseVIAlign")		, FALSE);
	m_bUsePreAlign				= ini.GetBoolean(_T("OPTION")	, _T("m_bUsePreAlign")		, FALSE);
	m_bUseLoginRequest			= ini.GetBoolean(_T("OPTION")	, _T("m_bUseLoginRequest")	, FALSE);
	m_bUseManualReceive			= ini.GetBoolean(_T("OPTION")	, _T("m_bUseManualReceive")	, FALSE);
	m_bUseRCMD					= ini.GetBoolean(_T("OPTION")	, _T("m_bUseRCMD")	, FALSE);
	m_bUseVIInsp				= ini.GetBoolean(_T("OPTION")	, _T("m_bUseVIInsp")	, FALSE);
	//2017-07-06,SKCHO,  고객사 요청으로 DCR 옵션 기능 삭제
	//m_bUseDCR					= ini.GetBoolean(_T("OPTION")	, _T("m_bUseDCR")	, FALSE);
	m_bUseTrayAlign				= ini.GetBoolean(_T("OPTION")	, _T("m_bUseTrayAlign")	, FALSE);
	m_bUseContact				= ini.GetBoolean(_T("OPTION")	, _T("m_bUseContact")	, FALSE);
	m_bUseJudgeOK				= ini.GetBoolean(_T("OPTION")	, _T("m_bUseJudgeOK")	, FALSE);
	m_bUseCellInfoSkip			= ini.GetBoolean(_T("OPTION")	, _T("m_bUseCellInfoSkip")	, FALSE);
	m_bUseJobEnd				= ini.GetBoolean(_T("OPTION")	, _T("m_bUseJobEnd")	, FALSE);

	m_dJobEndTime  				= ini.GetDouble(_T("OPTION")	, _T("m_dJobEndTime")			, 20.);
	m_dGrabEndWaitTime			= ini.GetDouble(_T("OPTION")	, _T("m_dGrabEndWaitTime")		, 30.);
	m_dClassEndWaitTime			= ini.GetDouble(_T("OPTION")	, _T("m_dClassEndWaitTime")		, 30.);
	m_nMaxGoodTrayCount			= ini.GetLong(_T("OPTION")		, _T("m_nMaxGoodTrayCount")		, 30);
	m_nMaxNGTrayCount			= ini.GetLong(_T("OPTION")		, _T("m_nMaxNGTrayCount")		, 30);

	m_nVisionContinuesNGCnt		= ini.GetLong(_T("OPTION")		, _T("m_nVisionContinuesNGCnt")		, 5);

	m_dRunDownWaitTime          = ini.GetDouble(_T("OPTION")		, _T("m_dRunDownWaitTime")		, 180.);  //2017-03-23,skcho, idle 상태변환 대기 시간

	m_dMCRDelayTime          = ini.GetDouble(_T("OPTION")		, _T("m_dMCRDelayTime")		, 0.2); 
	
	//2017-07-10,SKCHO
	m_nTrayIDCount          = ini.GetLong(_T("OPTION")		, _T("m_nTrayIDCount")		, 30); 
	m_nAGVDelayTime         = ini.GetLong(_T("OPTION")		, _T("m_nAGVDelayTime")		, 5);
	
	//2017-09-21,SKCHO, T5 전원 리셋 시간
	m_nT5RestartTime         = ini.GetLong(_T("OPTION")		, _T("pT5RestartTime")		, 24);
	m_dGoodTRRetryLength  = ini.GetDouble(_T("OPTION")		, _T("m_dGoodTRRetryLength")		, 1.0);
	m_dNGTRRetryLength  = ini.GetDouble(_T("OPTION")		, _T("m_dNGTRRetryLength")		, 1.0);

	
	// Zone , Ch Skip
	m_bUseASkip				= ini.GetBoolean(_T("OPTION")	, _T("m_bUseASkip")	, FALSE);
	m_strASkipDescription   = ini.GetString(_T("OPTION")	, _T("ASkip_Description"), _T("OFF"));
	
	m_bUseBSkip				= ini.GetBoolean(_T("OPTION")	, _T("m_bUseBSkip")	, FALSE);
	m_strBSkipDescription   = ini.GetString(_T("OPTION")	, _T("BSkip_Description"), _T("OFF"));

	m_bUseCSkip				= ini.GetBoolean(_T("OPTION")	, _T("m_bUseCSkip")	, FALSE);
	m_strCSkipDescription   = ini.GetString(_T("OPTION")	, _T("CSkip_Description"), _T("OFF"));

	m_bUseDSkip				= ini.GetBoolean(_T("OPTION")	, _T("m_bUseDSkip")	, FALSE);
	m_strDSkipDescription   = ini.GetString(_T("OPTION")	, _T("DSkip_Description"), _T("OFF"));

	//2017-11-01,SKCHO
	m_bOldUseASkip = m_bUseASkip;
	m_bOldUseBSkip = m_bUseBSkip;
	m_bOldUseCSkip = m_bUseCSkip;
	m_bOldUseDSkip = m_bUseDSkip;

	m_bUseACh1Skip				= ini.GetBoolean(_T("OPTION")	, _T("m_bUseACh1Skip")	, FALSE);
	m_bUseBCh1Skip				= ini.GetBoolean(_T("OPTION")	, _T("m_bUseBCh1Skip")	, FALSE);
	m_bUseCCh1Skip				= ini.GetBoolean(_T("OPTION")	, _T("m_bUseCCh1Skip")	, FALSE);
	m_bUseDCh1Skip				= ini.GetBoolean(_T("OPTION")	, _T("m_bUseDCh1Skip")	, FALSE);
	m_bUseACh2Skip				= ini.GetBoolean(_T("OPTION")	, _T("m_bUseACh2Skip")	, FALSE);
	m_bUseBCh2Skip				= ini.GetBoolean(_T("OPTION")	, _T("m_bUseBCh2Skip")	, FALSE);
	m_bUseCCh2Skip				= ini.GetBoolean(_T("OPTION")	, _T("m_bUseCCh2Skip")	, FALSE);
	m_bUseDCh2Skip				= ini.GetBoolean(_T("OPTION")	, _T("m_bUseDCh2Skip")	, FALSE);

	m_bUseContactRetry			= ini.GetBoolean(_T("OPTION")	, _T("m_bUseContactRetry")	, FALSE);
	m_bUseCarrierReportSkip         = ini.GetBoolean(_T("OPTION")	, _T("m_bUseCarrierReportSkip")	, FALSE);
	m_bUseRepeatInsp			= ini.GetBoolean(_T("OPTION")	, _T("m_bUseRepeatInsp")	, FALSE);

	m_bLoadPitchIn			= ini.GetBoolean(_T("OPTION")	, _T("m_bLoadPitchIn")	, FALSE);

	m_strRejectCode			= ini.GetString(_T("OPTION")	, _T("m_strRejectCode")	, _T("L"));

	m_bGrabMCRUse			= ini.GetBoolean(_T("OPTION")	, _T("m_bGrabMCRUse")	, FALSE);
	
	//2017-07-20,SKCHO
	m_bDivision			= ini.GetBoolean(_T("OPTION")	, _T("m_bDivision")	, FALSE);

	m_dLDTRBlowerOffTime   = ini.GetDouble(_T("OPTION")		, _T("m_dLDTRBlowerOffTime"),0.5);
	//2017-09-19,SKCHO
	m_dCellInfoWaitTime   = ini.GetDouble(_T("OPTION")		, _T("m_dCellInfoWaitTime"),20);
	m_dCarrierInfoWaitTime   = ini.GetDouble(_T("OPTION")		, _T("m_dCarrierInfoWaitTime"),20);

	m_bSkipLDConvSen3			= ini.GetBoolean(_T("OPTION")	, _T("m_bSkipLDConvSen3")	, FALSE);
	//2017-09-19,SKCHO
	m_bUseULDTRMode			= ini.GetBoolean(_T("OPTION")	, _T("m_bUseULDTRMode")	, FALSE);
	m_bUseLDRobotRetry			= ini.GetBoolean(_T("OPTION")	, _T("m_bUseLDRobotRetry")	, FALSE);
	
	m_bUseTrayVI			= ini.GetBoolean(_T("OPTION")	, _T("m_bUseTrayVI")	, FALSE);

	//2017-11-17,SKCHO
	m_bUsePATRRetry = ini.GetBoolean(_T("OPTION")	, _T("m_bUsePATRRetry")	, FALSE);

	//2017-12-22,SKCHO
	m_bUseJigBlow = ini.GetBoolean(_T("OPTION")	, _T("m_bUseJigBlow")	, FALSE);
	m_nPaOrgTime        = ini.GetLong(_T("m_nPaOrgTime")		, _T("m_nPaOrgTime")		, 24);

	//2018-05-11,NAM
	m_nContactDelay         = ini.GetLong(_T("OPTION")		, _T("m_nContactDelay")		, 3);

	//2018-06-29,NAM
	m_nConveyorDelay        = ini.GetLong(_T("OPTION")		, _T("m_nConveyorDelay")		, 3);

	//2018-06-29,NAM
	m_bUseConveyorMode				= ini.GetBoolean(_T("OPTION")	, _T("m_bUseConveyorMode")	, FALSE);

	//2018-01-20,GHLEE
	m_bUseFPCBSen = ini.GetBoolean(_T("OPTION")	, _T("m_bUseFPCBSen")	, FALSE);

	//2018-03-15,GHLEE
	m_bGoodInFinalSen = ini.GetBoolean(_T("OPTION")	, _T("m_bGoodInFinalSen")	, FALSE);


		//2017-08-14 KAKAKA
	m_NGINLIFTSEN1		= ini.GetBoolean(_T("OPTION")	, _T("m_NGINLIFTSEN1")	, TRUE);
	m_NGINLIFTSEN2		= ini.GetBoolean(_T("OPTION")	, _T("m_NGINLIFTSEN2")	, TRUE);
	m_NGOUTLIFTSEN1		= ini.GetBoolean(_T("OPTION")	, _T("m_NGOUTLIFTSEN1")	, TRUE);
	m_NGOUTLIFTSEN2		= ini.GetBoolean(_T("OPTION")	, _T("m_NGOUTLIFTSEN2")	, TRUE);

	//2017-08-31,SKCHO FDC 상/하한값 추가
	m_dFDCRange[UPS_VOLT_MAX] = ini.GetDouble(_T("FDC RANGE"),_T("UPS_VOLT_MAX"),242.0);
	m_dFDCRange[UPS_VOLT_MIN] = ini.GetDouble(_T("FDC RANGE"),_T("UPS_VOLT_MIN"),198.0);

	m_dFDCRange[UPS_CURRENT_MAX] = ini.GetDouble(_T("FDC RANGE"),_T("UPS_CURRENT_MAX"),15.0);
	m_dFDCRange[UPS_CURRENT_MIN] = ini.GetDouble(_T("FDC RANGE"),_T("UPS_CURRENT_MIN"),0.0);

	m_dFDCRange[UPS_POWER_MAX] = ini.GetDouble(_T("FDC RANGE"),_T("UPS_POWER_MAX"),3.0);
	m_dFDCRange[UPS_POWER_MIN] = ini.GetDouble(_T("FDC RANGE"),_T("UPS_POWER_MIN"),0.0);

	m_dFDCRange[GPS_VOLT_MAX] = ini.GetDouble(_T("FDC RANGE"),_T("GPS_VOLT_MAX"),242.0);
	m_dFDCRange[GPS_VOLT_MIN] = ini.GetDouble(_T("FDC RANGE"),_T("GPS_VOLT_MIN"),198.0);

	m_dFDCRange[GPS_CURRENT_MAX] = ini.GetDouble(_T("FDC RANGE"),_T("GPS_CURRENT_MAX"),15.0);
	m_dFDCRange[GPS_CURRENT_MIN] = ini.GetDouble(_T("FDC RANGE"),_T("GPS_CURRENT_MIN"),0.0);
	
	m_dFDCRange[GPS_POWER_MAX] = ini.GetDouble(_T("FDC RANGE"),_T("GPS_POWER_MAX"),3.0);
	m_dFDCRange[GPS_POWER_MIN] =  ini.GetDouble(_T("FDC RANGE"),_T("GPS_POWER_MIN"),0.0);

	m_dFDCRange[GMS_MAX] = ini.GetDouble(_T("FDC RANGE"),_T("GMS_MAX"),10.0);
	m_dFDCRange[GMS_MIN] =  ini.GetDouble(_T("FDC RANGE"),_T("GMS_MIN"),0.0);

	m_dFDCRange[AIR_MAX] = ini.GetDouble(_T("FDC RANGE"),_T("AIR_MAX"),70);
	m_dFDCRange[AIR_MIN] =  ini.GetDouble(_T("FDC RANGE"),_T("AIR_MIN"),50);

	m_dFDCRange[VAC_MAX] = ini.GetDouble(_T("FDC RANGE"),_T("VAC_MAX"),-40);
	m_dFDCRange[VAC_MIN] =  ini.GetDouble(_T("FDC RANGE"),_T("VAC_MIN"),-80);

	//2018wjp
	m_bUseBuffer = ini.GetBoolean(_T("OPTION")	, _T("m_bUseBuffer")	, FALSE);
	m_nBufferUseWaitTime = ini.GetLong(_T("OPTION")	, _T("m_nBufferUseWaitTime")	,300);
	m_nBufferUseTrayDownZ = ini.GetDouble(_T("OPTION")	, _T("m_nBufferUseTrayDownZ")	, 0.5);
	m_bUseConveyorTestModel = ini.GetBoolean(_T("OPTION")	, _T("m_bUseConveyorTestModel")	, FALSE);
}

void CConfigOption::SaveOptionFile( CString strPath, CString strBackupPath )
{
	if(CFileSupport::FileCheck(strPath))
	{
		// 기존 파일 백업
		::CopyFile(strPath, strBackupPath, FALSE);
		// File 내용을 깔끔하게 하기 위해 기존껀 지우고 새로 적는다
		::DeleteFile(strPath);
	}

	CIni ini(strPath);

	ini.WriteLong(_T("OPTION")		, _T("m_OffsetHour")			, m_OffsetHour				);
	ini.WriteLong(_T("OPTION")		, _T("m_AGVWaitTime")			, m_AGVWaitTime				);
	ini.WriteLong(_T("OPTION")		, _T("m_UpperIFWaitTime")		, m_UpperIFWaitTime			);

	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseDryRun")			, m_bUseDryRun				);
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseLotEnd")			, m_bUseLotEnd				);
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseUpperMCRRead")		, m_bUseUpperMCRRead		);
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseLowerMCRRead")		, m_bUseLowerMCRRead		);
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseEdgeAlign")			, m_bUseEdgeAlign			);
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseVIAlign")			, m_bUseVIAlign				);
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUsePreAlign")			, m_bUsePreAlign			);
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseLoginRequest")		, m_bUseLoginRequest		);
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseManualReceive")		, m_bUseManualReceive		);
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseRCMD")				, m_bUseRCMD		);
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseVIInsp")			, m_bUseVIInsp		);
	//2017-07-06,SKCHO,  고객사 요청으로 DCR 옵션 기능 삭제
	//ini.WriteBoolean(_T("OPTION")	, _T("m_bUseDCR")				, m_bUseDCR		);
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseTrayAlign")			, m_bUseTrayAlign		);
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseContact")			, m_bUseContact		);
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseJudgeOK")			, m_bUseJudgeOK		);
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseCellInfoSkip")			, m_bUseCellInfoSkip		);
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseJobEnd")				, m_bUseJobEnd		);

	ini.WriteDouble(_T("OPTION")	,_T("m_dJobEndTime")				, m_dJobEndTime);
	ini.WriteDouble(_T("OPTION")	, _T("m_dGrabEndWaitTime")			, m_dGrabEndWaitTime			);
	ini.WriteDouble(_T("OPTION")	, _T("m_dClassEndWaitTime")			, m_dClassEndWaitTime			);
	ini.WriteLong(_T("OPTION")		, _T("m_nMaxGoodTrayCount")			, m_nMaxGoodTrayCount			);
	ini.WriteLong(_T("OPTION")		, _T("m_nMaxNGTrayCount")			, m_nMaxNGTrayCount				);

	ini.WriteLong(_T("OPTION")		, _T("m_nVisionContinuesNGCnt")			, m_nVisionContinuesNGCnt				);

	ini.WriteDouble(_T("OPTION")	, _T("m_dRunDownWaitTime")			, m_dRunDownWaitTime			);  //2017-03-23,skcho
	
	ini.WriteDouble(_T("OPTION")	, _T("m_dMCRDelayTime")			, m_dMCRDelayTime			);  //2017-03-23,skcho
	
	//2017-07-10,SKCHO
	ini.WriteLong(_T("OPTION")		, _T("m_nTrayIDCount")			, m_nTrayIDCount			);
	ini.WriteLong(_T("OPTION")		, _T("m_nAGVDelayTime")			, m_nAGVDelayTime			);
	
	ini.WriteDouble(_T("OPTION")		, _T("m_dLDTRBlowerOffTime")			, m_dLDTRBlowerOffTime			);
		//2017-09-19,SKCHO
	ini.WriteDouble(_T("OPTION")		, _T("m_dCellInfoWaitTime")			, m_dCellInfoWaitTime			);
	ini.WriteDouble(_T("OPTION")		, _T("m_dCarrierInfoWaitTime")			, m_dCarrierInfoWaitTime			);

	ini.WriteLong(_T("OPTION")		, _T("m_nT5RestartTime")			, m_nT5RestartTime			);
	ini.WriteDouble(_T("OPTION")		, _T("m_dGoodTRRetryLength")			, m_dGoodTRRetryLength			);
	ini.WriteDouble(_T("OPTION")		, _T("m_dNGTRRetryLength")			, m_dNGTRRetryLength			);

	
	// Zone , Ch Skip
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseASkip")				,m_bUseASkip		);
	ini.WriteString(_T("OPTION")	, _T("ASkip_Description")		,m_strASkipDescription );

	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseBSkip")				,m_bUseBSkip		);
	ini.WriteString(_T("OPTION")	, _T("BSkip_Description")		,m_strBSkipDescription );
	
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseCSkip")				,m_bUseCSkip		);
	ini.WriteString(_T("OPTION")	, _T("CSkip_Description")		,m_strCSkipDescription );
	
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseDSkip")				,m_bUseDSkip		);
	ini.WriteString(_T("OPTION")	, _T("DSkip_Description")		,m_strDSkipDescription );
	
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseACh1Skip")				, m_bUseACh1Skip		);
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseBCh1Skip")				, m_bUseBCh1Skip		);
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseCCh1Skip")				, m_bUseCCh1Skip		);
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseDCh1Skip")				, m_bUseDCh1Skip		);
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseACh2Skip")				, m_bUseACh2Skip		);
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseBCh2Skip")				, m_bUseBCh2Skip		);
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseCCh2Skip")				, m_bUseCCh2Skip		);
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseDCh2Skip")				, m_bUseDCh2Skip		);

	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseContactRetry")			, m_bUseContactRetry		);

	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseCarrierReportSkip")		, m_bUseCarrierReportSkip		);

	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseRepeatInsp")			, m_bUseRepeatInsp		);

	ini.WriteBoolean(_T("OPTION")	, _T("m_bLoadPitchIn")				, m_bLoadPitchIn		);

	ini.WriteString(_T("OPTION")	, _T("m_strRejectCode")				, m_strRejectCode		);

	ini.WriteBoolean(_T("OPTION")	, _T("m_bGrabMCRUse")				, m_bGrabMCRUse		);
	//2017-07-20,SKCHO
	ini.WriteBoolean(_T("OPTION")	, _T("m_bDivision")				, m_bDivision		);

	ini.WriteBoolean(_T("OPTION")	, _T("m_bSkipLDConvSen3")				, m_bSkipLDConvSen3		);
	
	//2017-09-19,SKCHO
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseULDTRMode")				, m_bUseULDTRMode		);
	
	//2017-09-19,SKCHO
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseLDRobotRetry")				, m_bUseLDRobotRetry		);
	    
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseTrayVI")				, m_bUseTrayVI		);
	
	//2017-11-17,SKCHO PATR RETRY
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUsePATRRetry")				, m_bUsePATRRetry		);

	//2017-12-22,SKCHO JigBlow
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseJigBlow")				, m_bUseJigBlow		);
	ini.WriteLong(_T("OPTION")		, _T("m_nPaOrgTime")			, m_nPaOrgTime			);

	//2018-05-11,NAM
	ini.WriteLong(_T("OPTION")		, _T("m_nContactDelay ")			, m_nContactDelay 	);

	//2018-06-29,NAM
	ini.WriteLong(_T("OPTION")		, _T("m_nConveyorDelay ")			, m_nConveyorDelay 	);

	//2018-06-29,NAM
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseConveyorMode")				, m_bUseConveyorMode);

	//2018-01-20,GHLEE FPCBSen
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseFPCBSen")				, m_bUseFPCBSen		);

	//201-03-15,GHLEE LastTraySen
	ini.WriteBoolean(_T("OPTION")	, _T("m_bGoodInFinalSen")				, m_bGoodInFinalSen		);


	//2017-08-14,KAKAKA
	ini.WriteBoolean(_T("OPTION")	, _T("m_NGINLIFTSEN1")				, m_NGINLIFTSEN1		);
	ini.WriteBoolean(_T("OPTION")	, _T("m_NGINLIFTSEN2")				, m_NGINLIFTSEN2		);
	ini.WriteBoolean(_T("OPTION")	, _T("m_NGOUTLIFTSEN1")				, m_NGOUTLIFTSEN1		);
	ini.WriteBoolean(_T("OPTION")	, _T("m_NGOUTLIFTSEN2")				, m_NGOUTLIFTSEN2		);


	ini.WriteDouble(_T("FDC RANGE")	, _T("UPS_VOLT_MAX")			, m_dFDCRange[UPS_VOLT_MAX]	);
	ini.WriteDouble(_T("FDC RANGE")	, _T("UPS_VOLT_MIN")			, m_dFDCRange[UPS_VOLT_MIN]	);

	ini.WriteDouble(_T("FDC RANGE")	, _T("UPS_CURRENT_MAX")			, m_dFDCRange[UPS_CURRENT_MAX]	);
	ini.WriteDouble(_T("FDC RANGE")	, _T("UPS_CURRENT_MIN")			, m_dFDCRange[UPS_CURRENT_MIN]	);

	ini.WriteDouble(_T("FDC RANGE")	, _T("GPS_VOLT_MAX")			, m_dFDCRange[GPS_VOLT_MAX]	);
	ini.WriteDouble(_T("FDC RANGE")	, _T("GPS_VOLT_MIN")			, m_dFDCRange[GPS_VOLT_MIN]	);

	ini.WriteDouble(_T("FDC RANGE")	, _T("GPS_CURRENT_MAX")			, m_dFDCRange[GPS_CURRENT_MAX]	);
	ini.WriteDouble(_T("FDC RANGE")	, _T("GPS_CURRENT_MIN")			, m_dFDCRange[GPS_CURRENT_MIN]	);

	
	ini.WriteDouble(_T("FDC RANGE")	, _T("GPS_POWER_MAX")			, m_dFDCRange[GPS_POWER_MAX]	);
	ini.WriteDouble(_T("FDC RANGE")	, _T("GPS_POWER_MIN")			, m_dFDCRange[GPS_POWER_MIN]	);

	
	ini.WriteDouble(_T("FDC RANGE")	, _T("GMS_MAX")			, m_dFDCRange[GMS_MAX]	);
	ini.WriteDouble(_T("FDC RANGE")	, _T("GMS_MIN")			, m_dFDCRange[GMS_MIN]	);

	ini.WriteDouble(_T("FDC RANGE")	, _T("AIR_MAX")			, m_dFDCRange[AIR_MAX]	);
	ini.WriteDouble(_T("FDC RANGE")	, _T("AIR_MIN")			, m_dFDCRange[AIR_MIN]	);

	ini.WriteDouble(_T("FDC RANGE")	, _T("VAC_MAX")			, m_dFDCRange[VAC_MAX]	);
	ini.WriteDouble(_T("FDC RANGE")	, _T("VAC_MIN")			, m_dFDCRange[VAC_MIN]	);

	//2018wjp
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseBuffer")	, m_bUseBuffer);
	ini.WriteLong(_T("OPTION")	, _T("m_nBufferUseWaitTime")	, m_nBufferUseWaitTime);
	ini.WriteDouble(_T("OPTION")	, _T("m_nBufferUseTrayDownZ")	,m_nBufferUseTrayDownZ);
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseConveyorTestModel")	,m_bUseConveyorTestModel);
}
