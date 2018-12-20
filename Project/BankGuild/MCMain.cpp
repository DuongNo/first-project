
#include "StdAfx.h"
#include "CheetahApp.h"			// for theDataPCMessage;
#include "MCMain.h"

#include "MainFrm.h"


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// 사용자 정의 Station 및 Service/System

#include "MCDefine.h"


//GxGUIFrame 사용
#include "GausGUI\GxGUIFrame.h"
#include "GausGUI\GxUICtrl.h"
#include "GausGUI\GxButtonEx.h"
#include "GausGUI\GxStaticEx.h"
#include "GausGUI\GxMsgBox.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//
//// Singleton 구현
//CMCMain* CMCMain::theMCMain = NULL;				// static 초기화, MCMain의 주소
//
//CMCMain* CMCMain::GetInstance()
//{
//	if( theMCMain == NULL ) theMCMain = new CMCMain();
//	return theMCMain;
//}
//
//// SingleTone, Master 객체 파괴
//void CMCMain::ReleaseMCMain(void)
//{
//	if ( theMCMain != NULL ) 
//	{
//		theMCMain->ReleaseProcess();
//		delete theMCMain;
//		theMCMain = NULL;
//	}
//}
////////////////////////////////////////////////////////////////////////////////////////////////////

// 생성자
CMCMain::CMCMain(void)
{
	m_sBasePath		= _T("D:\\AMT-Pilot");			// 근간이되는 경로
	m_iniProfile.m_sIniFile.Format(_T("%s\\Data\\Profile\\MCMain.ini"), m_sBasePath);

	m_hOwner			= NULL;
	m_pGausFrame	= NULL;		// 2015-07-24, GausFrame으로 통합
	m_pSocketMgr	= NULL;

	//m_pMaster		= NULL;
	//m_pServiceHub	= NULL;
	//m_pSystemHub	= NULL;
	//m_pStationHub	= NULL;

	m_pCellInfo		= NULL;			// Cell Data 저장 Class
	m_pJigInfo		= NULL;
	m_pLogSeq		= NULL;

	m_sBasePath		= _T("");				// 기본 경로
	m_bEnabled		= FALSE;					// Machine System을 동작할 수 있는 상태이가 ? 모든 초기화를 마쳤는가 ?

	m_nInitialFlag	= eINIT_NONE;				// 아직 Initial 작업을 하지않음
	m_nLanguageType	= eLANG_KOREAN;			// 한글이 기본

	m_bIsSimulate	= FALSE;						// Simulation mode 인가 ?
	m_bIsDryRun		= FALSE;						// DryRun mode인가 ?
	m_bCycleStop	= TRUE;						// Cycle stop 여부
	m_bAlarmStop	= TRUE;						// Alarm 발생시 모든 축 정지 여부
//	m_bDisp1stPattern = FALSE;					// 1st Pattern 강제 출력 여부




//d	m_nUserLevel	= eUSER_NONE;						// 현재 접속은 사용자 보안단계
//d	m_rProfileData.sUserID	= _T("");

	m_rProfileData.sVersion					= PROGRAM_VERSION;	// program version

	// CIM 관련
	m_rProfileData.sDataPCVersion			= _T("");		// Data-PC Version, Cell-Log에 필요한것으로, Data-PC와 통신시 내려받는다. CDlgSock 에서 갱신

	m_rProfileData.nLastReportState			= 0;				// E_EQST_MATCH_NONE, CIM으로 보고한 최종 상태, (중복된 상태는 보고를 하지 않기 위함, MainFrm에서 사용)
	m_rProfileData.sLastReportState			= _T("NONE");	// -> 문자열 (MainFrm에서 갱신해준다.), AutoForm에서 화면에 표시해주기 위해 사용


	m_rProfileData.bDZoneDefectInputFlag	= FALSE;		// 사용자 Defect 입력을 받는가 ?	
	m_rProfileData.bInitialFisrtStart		= FALSE;		// 초기화를 마친 후 최초로 수행한 SART이면 비가동 로그를 실시하지 않는다.
	m_rProfileData.bTACTStartTimeUpdateFlag	= TRUE;		// TACT 측정 START시간을 AUTO 전환시각으로 갱신 여부
	m_rProfileData.bOperatorLoadingWorkFlag	= FALSE;	// 작업자 로딩 작업 진행중인지 여부, 이 때에만 STOP -> AUTO 변경시 현재 시각 적용이 가능하다.

	m_rProfileData.bCIMInterlockFlag		= FALSE;		// 상태 변경 보고를 잠근 상태가 아니다.

	// 각종 검사 항목 실행 여부 (option으로 처리)
	m_rProfileData.bUseTest_VI				= TRUE;		// C-Zone에서 AV검사를 진행할 것인지 여부,
	m_rProfileData.bUseTest_Manual			= TRUE;		// D-Zone 작업자 검사를 진행할 것인가 ?
	m_rProfileData.bUseTest_TSP				= FALSE;		// C-Zone에서 TSP 검사를 진행할 것인지 여부
	m_rProfileData.bUseTest_IDCheck			= FALSE;		// IDCHECK 검사를 진행할 것인가 ?
	m_rProfileData.bUseTest_Sleep			= FALSE;		// Sleep Current검사를 진행할 것인가 ?
	m_rProfileData.bUseTest_White			= FALSE;		// White Current 검사를 진행할 것인가 ?
	m_rProfileData.bUseTest_BIN2TestMode		= FALSE;		// 2015-03-06, jhLee, 김상묵 사원요청, TEST Cell 작업 모드, 별도의 Cell log로 남기고 상위 보고를 하지 않는다.
//d	m_rProfileData.bUseTest_MultiGrab		= FALSE;		// 2015-03-11, jhLee, 김상묵 사원요청, CH_1 Grab.OK 응답 받은 후 바로 CH_2의 Grab.Start를 보낼 것인가 ? 
	m_rProfileData.bUseTest_ModelCheck		= FALSE;		// Cell Model 투입시 현재 Model 구동여부를 판단한다;

	


	// 각종 실행 항목 Option
	m_rProfileData.bUse_CircleDisp			= TRUE;		// D-Zone Test시 VI Defect을 원으로 그려서 보여줄 것인가 ?
	m_rProfileData.bUse_LoopTest			= FALSE;	// 무한루프 테스트 일명 뺑뺑이

	m_rProfileData.bUse_ContactCurrentCheck	= FALSE;	// 2014-11-25, jhLee, Contact Current NG를 체크하여 Loading을 Cancel시킬 것인가 ?
	m_rProfileData.bUse_CellExcept			= FALSE;	// 2014-12-22, jhLee, 특정 Cell-ID는 검사를 수행하지 않고 제외 시키도록 한다.


	m_rProfileData.bUse_SkipJigTilt			= FALSE;	// JIG를 Tilt 하지 않고 사용할 것인가 ? (TACT 감소를 위해)
	m_rProfileData.bUse_SkipMCR			= FALSE;	// UV-MCR을 읽지 않고 가상의 Cell ID를 생성하여 사용할 것인가 ?
	//d m_rProfileData.bUse_SkipCIMSend			= FALSE;	// 상위(CIM) 보고 하지않음 (테스트 제품 검사시)
	m_rProfileData.bUse_SkipEngineerTest	= FALSE;	// 엔지니어 로그인으로도 검사를 수행할 수 있도록 한다 - 기본값 FALSE
	m_rProfileData.bUse_AutoContactOff		= FALSE;	// 이전 검사가 모두 양품일 경우 별도의 Defect 입력화면을 띄우지 않고 PG-OFF 시킨다.
	m_rProfileData.bUse_AutoMoveRetry		= TRUE;		// 2015-03-10, 모션 이동관련 에러 발생시 자동으로 재시도를 수행하게 한다. 주로 NOT Inpos 알람에 대응하기 위함.

	m_rProfileData.bUse_CellLoadingCmd		= FALSE;	//미사용, 2015-03-28, TACT을 줄이기위해 PowerOn 버튼으로 기존 POWER_ON을 대신하여 Cell Loading 명령을 사용할 것인가 ? TRUE:CellLoading명령 사용, FALSE:기존대로 POWER_ON 사용
	//d m_rProfileData.bUse_CZoneReset			= FALSE;	//미사용, 2015-03-30, 2개 채널 제품 모두 양품이고, bUse_SkipDefectForm 옵션이 활성화 되었을 경우, A-Zone으로 회전전에 Contact을 풀고 회전하여 작업 시간을 단축시키도록 한다.
	m_rProfileData.bUse_LoginServerRequest	= FALSE;	// 2015-04-16, 작업자 Login/out시 상위서버에게 인증을 요청하도록 할 것인가 ?
	m_rProfileData.bUse_JudgeCellExcept		= FALSE;	// 2015-06-21 JH-PARk 판정 완료된 Cell에 대하여 투입을 진행할 것인가? 
	m_rProfileData.bUse_TMDVersionCheck		= FALSE;	// 2015-07-16, jhLee, 각 채널별 TMD Version을 관리할것인가 ?
	m_rProfileData.bUse_UpperBackLight		= TRUE;		// 2015-09-16, jhLee, 상부 백라이트 사용을 옵션으로 처리한다. SDC 사귀진 사원요청(이상욱D)
	m_rProfileData.bUse_TMDModelCheck		= FALSE;	// 2015-10-15, jhLee, 신호기 TMD모델명과 Product ID와 비교하여 Alarm 처리할 것인가 (SDC 백윤기사원 요청)
	m_rProfileData.bUse_SkipAutoShutdown	= FALSE;	// 2015-10-20, jhLee, 신호기 TMD모델명과 Product ID가 일치하지 않을경우 설비를 Down시키는 것을 비활성화 시킬것인가 ?
	m_rProfileData.bUse_SkipAutoTrackout	= FALSE;	// 2015-10-20, jhLee, 설비가 DOWN될 때 Track-in된 Cell을 자동으로 RETEST로 Track-Out하는 기능을 수행하지 않게할 것인가 ?


	// 정수형 옵션 항목
	m_rProfileData.nTIME_CylinderTimeout	= 2000;		// Cylinder 동작 Timeout 밀리초
	m_rProfileData.nTIME_VacuumTimeout		= 2000;		// Vacuum 동작 Timeout 밀리초
	m_rProfileData.nTIME_UnloadVACOff		= 100;		// Unload 동작시 진공 파기 지연 시간
	m_rProfileData.nTIME_RunDownTime		= 60;			// 입력이 없을 시 RunDown으로 전환시키는 시간, 초
	m_rProfileData.nTIME_OPLossTime			= 18;			// 작업자 손실시간 (OP Loss) 판단 기준 시간, 초
	m_rProfileData.nTIME_EQLossTime			= 18;			// 설비동작 손실시간 (EQ Loss) 판단 기준 시간, 초
	m_rProfileData.nTIME_MDChangeTime		= 60;			// MD/BM 자동전환 기준 시간, 초 (60초?)
	m_rProfileData.nTIME_VTOffsetTime		= 10;			// 검사 파일을 추가로 기다려볼 시간 초
	m_rProfileData.nTIME_TECheckTime		= 4;			// TE Check Timeout 기준 시각을 지정 (초), 4초 이후에는 NG라고 판정한다.
	m_rProfileData.nCNT_DispCircleCount		= 5;			// D-Zone에서 Cell에 표시할 Defect circle 수량 (개)
	m_rProfileData.nTIME_LoginServerTimeout	= 3;			// 작업자 Login/out 요청에 대한 서버의 응답시간 제한, 초
	m_rProfileData.nTIME_MotorMoveCheckTime	= 50;			// 2015-06-21 JH-PARk, Motor 이동 완료 Retry 진행시 Delay 시간 밀리초
	m_rProfileData.nTIME_DispResultCell		= 3;			// 2015-07-01 JH-PARK, 판정완료된 Cell의 값을 표시해주는 시간 (초)
	m_rProfileData.nTIME_TSPTimeout			= 10;			// 2016-04-21, TSP Timeout

	m_rProfileData.nMode_AlignImageSave	= eMODE_SaveAll;		// Align 이미지 저장 모드 0:None, 1:OK, 2:NG, 3:All
	m_rProfileData.nMode_UVMCRImageSave	= eMODE_SaveAll;		// UVMCR 이미지 저장 모드 0:None, 1:OK, 2:NG, 3:All
	m_rProfileData.nMode_WorkShift		= eMODE_WorkShift3;		// 작업자 근무 방식 모드 0:3개조 , 1:2개조 
	m_rProfileData.nMode_DispVIResult	= eMODE_DispMessage;	// 검사결과 표시 방법     0: Message , 1 : Pattern
	



	// 실행 변수 초기화
	// 모든 날짜형은 현재 시각을 기준으로 지정하고 나중에 파일에서 읽어와서 다시 갱신한다.
	::GetLocalTime( &(m_rLosstimeRecord.tmStartTime) );		// 현재 시각으로 치환한다.
	::GetLocalTime( &(m_rLosstimeRecord.tmResetTime) );	
	::GetLocalTime( &(m_rLosstimeRecord.tmEndTime) );

	::GetLocalTime( &(m_rRuntimeData.tmUserLogin) );					// 작업자 Log in/out 시각
	::GetLocalTime( &(m_rRuntimeData.tmEQStart) );						// 설비 동작 시작 시각
	::GetLocalTime( &(m_rRuntimeData.tmEQStop) );						// 설비 중지 시각



	// 각종 시간 저장용 변수 초기화
	m_rRuntimeData.sUserLoginTime	= CGxUtility::SystemTimeToString(m_rRuntimeData.tmUserLogin);				
	m_rRuntimeData.sEQStartTime		= CGxUtility::SystemTimeToString(m_rRuntimeData.tmEQStart);
	m_rRuntimeData.sEQStopTime		= CGxUtility::SystemTimeToString(m_rRuntimeData.tmEQStop);

	m_rRuntimeData.timePrevDay		= CTime::GetCurrentTime();	// 현재 시각
	m_rRuntimeData.nPrevShift		= eIDX_Shift_DAY;			// 기본적인 작업 조


	//d	m_rRuntimeData.bForceCommFlag	= FALSE;					// 초기 통신 연결 시작시 다른 어떤 설정과는 상관없이 Data PC로 정보를 전송할 수 있게 예외처리한다.


	// 상위에 전송하는 코드및 영문 저장 하기 위한 변수 초기화 추가 시작 20141226-2 jhkim1
	m_sTPSystemCode = _T("");
	m_sTPSystemName = _T("");
	// 상위에 전송하는 코드및 영문 저장 하기 위한 변수 초기화 추가 끝 20141226-2 jhkim1


}


// 파괴자
CMCMain::~CMCMain(void)
{

	if ( m_pGausFrame )	m_pGausFrame->ReleaseInstance();
}



// 시스템 종료시 종료 처리
void CMCMain::ReleaseProcess(void)
{
	// Cell Log 및 비가동 Log file Close
	CloseCellLogFile();

}

//
// 각종 log들의 오늘 날짜 기준을 변경하여 준다.
//
void CMCMain::SetUpdateLogToday(int nHour, int nMin)
{
	// 각종 데이터로그의 날짜 지정
	CloseCellLogFile();												// 기존의 파일들은 닫아주고

	m_logCellLog.SetToday( nHour, nMin );
	m_logCellBIN2Log.SetToday( nHour, nMin );
	m_logAlarm.SetToday( nHour, nMin );						// 하루 기준 대입
	m_logTACTTime.SetToday( nHour, nMin );
	m_logContactData.SetToday( nHour, nMin );
	m_logProductData.SetToday( nHour, nMin );

	OpenCellLogFile();												// 새로 적용한 파일을 열어준다.
}

//
// Cell log 파일을 열어 다른곳에서 접근을 막는다. (프로그램 시작시, 날짜 변경시)
//
void CMCMain::OpenCellLogFile()
{
	m_logCellLog.OpenFile( 0 );			// Cell log 기록 파일
//	m_logCellBIN2Log.OpenFile( 0 );		// BIN2Test 기록용 Cell Log
//	m_logLosstime.OpenFile( 0 );			// 비가동 log 기록 파일
}

//
// 열려진 Cell Log file을 닫아준다. 
// 하루 기준 설정값이 변경되거나 날짜가 변경되었을 때 기존에 열린 파일을 닫아준다.
//
void CMCMain::CloseCellLogFile()
{
	m_logCellLog.CloseFile();			// Cell log 기록 파일
	m_logCellBIN2Log.CloseFile();		// BIN2 Test 용 기록 파일
	m_logLosstime.CloseFile();			// 비가동 log 기록 파일
}



// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


//
//// 기본 경로를 지정시켜준다 
//void CMCMain::SetBasePath(CString sPath)
//{
//	m_sBasePath	= sPath;			// 근간이되는 경로
//
//	// 구동 설정값들이 저장/관리되는 프로파일 데이터 설정
//	m_iniProfile.m_sIniFile.Format(_T("%s\\Data\\Profile\\MCMain.ini"), m_sBasePath);
//
//
//	if ( m_pMaster != NULL )			m_pMaster->SetBasePath( sPath );
//	if ( m_pServiceHub != NULL )		m_pServiceHub->SetBasePath( sPath );
//	if ( m_pSystemHub != NULL )			m_pSystemHub->SetBasePath( sPath );
//	if ( m_pStationHub != NULL )		m_pStationHub->SetBasePath( sPath );
//}


// Log pointer 획득
CGxLog* CMCMain::GetLogPointer(int nIdx)
{
	if ( (nIdx >= 0) && (nIdx < eMAX_LOGCount) )
	{
		return &( m_logBlock[nIdx] );			// 원하는 log control의 pointer 를 넘겨준다.
	}

	// index 오류시
	return &( m_logBlock[eLOG_ETC] );		// 기타 잡동사니
}


// Alarm data 기록용 Log pointer
CGxLogData*	CMCMain::GetAlarmDataLog()
{
	return &m_logAlarm;			// Alarm 기록용
}

// TACT Time 로그 기록용 Log pointer
CGxLogData*	CMCMain::GetTACTTimeDataLog()
{
	return &m_logTACTTime;		// TACT Time 로그 기록용
}

// 생산율 로그 기록용 Log pointer
CGxLog*	CMCMain::GetProductDataLog()
{
	return &m_logProductData;		// 생산율 로그 기록용
}


// 기본 경로 조회
CString CMCMain::GetBasePath(void)
{
	return m_sBasePath;
}

// Data folder 경로 조회
CString CMCMain::GetDataPath(void)
{
	return m_sPath_Data;
}

// Log folder 경로 조회
CString CMCMain::GetLogPath(void)
{
	return m_sPath_Log;
}

// Unit Image Path
CString  CMCMain::GetUnitImgPath(void)
{
	return m_sPath_UnitImg;					// 각 Unit들의 이미지가 존재하는 경로
}

CString  CMCMain::GetRuntimeImgPath(void)
{
	return m_sPath_Runtime;					// 동작시 생성되는 이미지 경로(Align, MCR 이미지)
}

// 검사PC에서 테스트 결과를 가져올 경로
// SysVIMonitor에서 사용한다.
CString CMCMain::GetVIResultPath(void)
{
	return m_rProfileData.sDIR_VIResult;
}

// 검사PC에게 변경된 레서피를 전달해줄 폴더
CString CMCMain::GetVIRecipePath(void)
{
	return m_rProfileData.sDIR_VIRecipe;					
}

// 검사PC에서 해상도관련 스펙을 가져올 경로
CString CMCMain::GetVIInformPath(void)
{
	return m_rProfileData.sDIR_VIInform;					
}

//// 검사PC에서 검사결과를 복사해서 내부에서 사용할 폴더
//CString CMCMain::GetVILocalPath(void)
//{
//	return m_sPath_VILocal;					
//}
//

//// 실행파일 경로 지정
//void CMCMain::SetExePath(CString sPath)
//{
//	m_sExePath = sPath;
//}


// 언어 설정
void CMCMain::SetLanguageType(int nType)
{
	m_nLanguageType = nType;					// 언어를 지정해주고,

	// MainFram에게 전송한다.
	::PostMessage( ( (CMainFrame*)AfxGetMainWnd() )->m_hWnd, UDMSG_DISP_LAN_CHANGE, (WPARAM)m_nLanguageType + 1, (LPARAM)0);
}


// 설정된 언어 조회
int CMCMain::GetLanguageType()
{
	return m_nLanguageType;
}



// 상위 소유자의 Handle을 넘겨준다.
void CMCMain::SetOwnerHandle(HWND hHandle)
{
	m_hOwner	= hHandle;								// 이 컨트롤을 소유한 주인의 핸들,

}



// 2015-08-05, jhLee, Axis Map 처리

// int m_nAxesMap[eMAX_AXISCount];				// 내부적으로 사용할 축 index 보관배열

// 내부적으로 사용하는 축 Index 조회 
int CMCMain::GetLocalAxis(int nIdx)
{
	if ( (nIdx < 0) || (nIdx >= eMAX_AXISCount) ) return -1;		// 잘못된 index 지정

	return m_nAxesMap[ nIdx ];										// 지정한 Index의 축 Index 리턴
}

// 내부적으로 사용하는 축 Index를 토대로 축 지정이 정상적인지 판정한다.
CString CMCMain::CheckLocalAxis()
{
	CString sResult = _T("");

	for (int i = 0 ; i < eMAX_AXISCount; i++)
	{
		if ( m_nAxesMap[i] < 0 )									// 0보다 작다면 잘못된 순번이다. 즉 해당 순번의 축은 존재하지 않는다.
		{
			if ( sResult.IsEmpty() )
			{
				sResult.Format(_T("* Axis Not Found : %d "), i);	// 비정상적인 축 순번을 표시한다.
			}
			else
				sResult.AppendFormat(_T(", %d "), i);
		}
	}//off for i

	return sResult;
}



// 모션보드의 상태를 점검하고 ErrorCode를 되돌려받는다. return : 0 정상
int CMCMain::GetMotionBoardCheck(CString& sMsg)
{
	CGxMotionCmdMgr* pMotionMgr = CGxMotionCmdMgr::GetInstance();			// 모션보드 관리자

	int nErrCode = ERR_NONE;
	int nSubIdx = 0;
	int nCmdType = pMotionMgr->GetCheckErrorCmdType( nSubIdx );		// 문제가 있는 보드를 찾는다.

	sMsg = _T("");

	// 문제가 있는 보드가 발견되면
	if ( nCmdType >= 0 )
	{
		switch ( nCmdType )
		{
			case eGxMOTION_CmdNMC2	:		// = 0,		// PAIX NMC2 제어기
				sMsg.Format(_T("[NMC2] Motion Controller [%d] Board Error"), nSubIdx); 
				nErrCode = ERR_COMM_NMC;					// =	1306	,	//	NMC 제어기와의 통신 실패
				break;

			case eGxMOTION_CmdNMC1 :		// PAIX NMC1
				sMsg.Format(_T("[NMC1] Motion Controller [%d] Board Error"), nSubIdx); 
				nErrCode = ERR_COMM_NMC;					// =	1306	,	//	NMC 제어기와의 통신 실패
				break;


			case eGxMOTION_CmdYMC :			// Yaskawa MP2100
				sMsg.Format(_T("[MP2100] Motion Controller [%d] Board Error"), nSubIdx); 
				nErrCode = ERR_COMM_MP2100;					// =	1308	,	//	MP2100과의 통신 실패
				break;
	
			default :
				sMsg.Format(_T("[UNKNOWN] Motion Controller [%d] Board Error"), nSubIdx); 
				nErrCode = ERR_COMM_NMC;					// =	1306	,	//	NMC 제어기와의 통신 실패
				break;
		}//of switch
	}

	return nErrCode;			// 지정된 오류코드를 되돌린다.
}





//
// 2014-10-15, jhLee, 새로운 TACT Time을 기록하는 루틴, Cell Info에서 지정한다.
//
// 지정 JIG의 모든 채널에 해당 순번의 TACT TIme을 현재의 시간으로 지정한다.
void CMCMain::SetTACTTime(int nJig, int nIdx)
{
	if ( (nJig >= 0) && (nJig < eMAX_JigCount) )
	{
		TCellData* pCell;
		double dbTime = 0.0;

		for (int i = 0; i < eMAX_ChCount; i++) 
		{
			pCell = m_pCellInfo->GetCellData(nJig, i);					// 지정 Jig의 각 채널의 포인트를 얻어온다.
			pCell->SetTACTTime( nIdx );									// 지정한 시각으로 지정한다.


			if (nIdx == eTACT_UnloadPGOff)			// 검사 완료 처리
			{
				dbTime = pCell->dbMoveTACT[eTACT_OPTime] - m_rProfileData.nTIME_OPLossTime;		// 작업자 손실시간

				if ( dbTime < 0.0 ) dbTime = 0.0;
				pCell->dbMoveTACT[eTACT_OPLossTime]	= dbTime;									// 기준 시간을 벗어난 설비 손실시간

				dbTime = pCell->dbMoveTACT[eTACT_EQTime] - m_rProfileData.nTIME_EQLossTime;		// 설비 손실시간

				if ( dbTime < 0.0 ) dbTime = 0.0;
				pCell->dbMoveTACT[eTACT_EQLossTime] = dbTime;									// 기준 시간을 벗어난 설비 손실시간
			}

			// 제품이 투입되는 시점을 기준으로 작업 조를 나눈다.
			if ( (nIdx == eTACT_PGOn) || (nIdx == eTACT_ReadyBtn) )			// 최초의 동작인 PG_On이 들어오면 , 혹은 양수버튼(만약을 대비)
			{
				pCell->SetShift( GetShift() );								// 현재의 작업조를 대입시켜준다.
			}
		}//of for i

		// 화면 변경용 메시지를 전송한다. 소유자인 MainFrame에게 TACT 변경을 알린다.
		// ::PostMessage(m_hOwner, UDMSG_TACT_UPDATE, nJig, nIdx);			// TACT Time이 변경되었다.
		::PostMessage( ( (CMainFrame*)AfxGetMainWnd() )->m_hWnd, UDMSG_TACT_UPDATE, (WPARAM)nJig, (LPARAM)nIdx);


		if (nIdx == eTACT_UnloadPGOff)			// 검사 완료 처리라면,
		{
			// ::PostMessage(m_hOwner, UDMSG_DISPDATA_UPDATE, eIDX_UPDATE_TACTAVG, nJig);		// 통합 TACT 정보를 갱신한다.
			::PostMessage( ( (CMainFrame*)AfxGetMainWnd() )->m_hWnd, UDMSG_DISPDATA_UPDATE, (WPARAM)eIDX_UPDATE_TACTAVG, (LPARAM)nJig);
		}
	}
}




// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==
// 2014-11-23, jhLee, Display circle to Cell

	//int		m_nPTN_JigNo;						// 표시되고 있는 Jig 번호
	//int		m_nPTN_CurrentPage;				// 현재 표시되고 있는 page 번호
	//int		m_nPTN_MaxPage;					// Control 할 최대 Page = 결과(1) + Circle 표시(2) + 신호기 기본 페이지(5)
	//int		m_nPTN_PGPage;						// 실제 신호기에 표시되는 원래 신호기 패터 Page 번호
	//int		m_nPTN_CirclePage;				// Circle을 그려 줄 Page 수 (최대 2 ?)

// 지정 Jig의 패턴을 그려주기 전에 처리 할 내용들
void CMCMain::SetPreparePatternDisplay(int nJig)
{
	//int i;
	//TCellData* pCell;

	//m_nPTN_JigNo			= nJig;				// 표시되고 있는 Jig 번호
	//m_nPTN_CurrentPage	= 0;					// 현재 표시되고 있는 page 번호
	//m_nPTN_PGPage			= 0;					// 실제 신호기에 표시되는 원래 신호기 패터 Page 번호, 0 : 아직 표시하지 않는다.

	//m_nPTN_CirclePage		= 0;					// Circle을 그려 줄 Page 수 (최대 1?)

	//for (i = 0; i < eMAX_ChCount; i++)
	//{
	//	pCell =  m_pCellInfo->GetCellData(nJig, i);					// 지정 Jig의 각 채널의 포인트를 얻어온다.
	//	if ( (pCell->bIsExist == TRUE) && (pCell->Bin2Defect == TRUE) && ( m_rProfileData.bUse_CircleDisp == TRUE ) )	// circle 출력을 해야 한다면,
	//	{
	//		if ( pCell->DefectCount > 0 )		// Cell중 한 군데에서라도 표시할 Defect이 존재한다면,
	//		{
	//			m_nPTN_CirclePage = 1;			// 최대 패턴 수량을 1개로 지정한다. (2014-11-27, 고정값)
	//			break;							// 나머지는 볼 것도 없다 !
	//		}
	//	}
	//}//of for i

	////// 최대로 표시 할 Circle pattern 수량을 제한한다.
	////if ( m_nPTN_CirclePage > 2 )				// 기본적으로 2페이지로 한정한다.
	////{
	////	m_nPTN_CirclePage = 2;
	////}

	//m_nPTN_MaxPage			= m_nPTN_CirclePage + m_rCellDisp.nDZonePtnMax;	// Control 할 최대 Page = 결과(1) + Circle 표시(2) + 신호기 기본 페이지(5)
}


// 작업자가 Next 버튼을 눌렀을 경우 처리
void CMCMain::SetNextPatternDisplay(int nJig)
{
	// 2016-05-23, jhLee
	//
	// 1. 만약 양품이거나 Disply 불량성 판정이 발생했다면 일반적인 Next/Back 신호를 보낸다.
	// 2. C-Zone BIN2, REJECT (CLASS 1, 2) 판정 발생시 Defect이 발생한 패턴만 표시해준다.
	// 3. POL 불량 (CLASS 13) 발생시 NEXT/BACK을 막는다.
	
	// nBIN2TestState;			// BIN2 패턴 검사 모든 : 0 : 일반적인 Next/Back, 1 : BIN2 패턴 표시, 2 : Next/Back 금지 모드

	CSvcTcpPG* pSvcTcpPG	= (CSvcTcpPG*)( (CGxServiceHub::GetInstance())->GetService(NAME_SERVICE_TcpPG));						// 신호기 통신


	int i;
	int nNextIdx = 0;
	int nPtnNo = 0;
	TCellData* pCell = NULL;

	for (i = 0; i < eMAX_ChCount; i++ )
	{
		pCell = m_pCellInfo->GetCellData(nJig, i); 
		if ( pCell )
		{
			if ( pCell->bIsExist )						// Cell이 존재하는 조건으로,
			{
				if ( pCell->nBIN2TestState == 0 )		// 일반적인 Next/Back 적용 모드
				{
					pSvcTcpPG->PG_SendNext(nJig, i);				// 지정 지그의 CH에 NEXT 전송
				}
				else if ( pCell->nBIN2TestState == 1 )	// BIN2 패턴 표시 모드
				{
					nNextIdx = pCell->nBIN2PtnIdx;												// 현재 선택된 패턴 순번 

					if ( (nNextIdx >= 0) && (nNextIdx < pCell->arrPtnIndex.GetCount()) )		// 합당한 범위인지 점검
					{
						nPtnNo = pCell->arrPtnIndex[ nNextIdx ];								// 패턴 Index를 추출한다.
						
						pSvcTcpPG->PG_SendPatternSelect(nJig, i, nPtnNo + 1);								// BIN2 패턴을 지정해준다.

						pCell->nBIN2PtnIdx = nNextIdx + 1;										// 다음 순번을 지정한다.
					}
				}

				// 그밖에는 반응을 하지 않는다.
			}
		}//of if pCell
	}//of for i


}


// 작업자가 Back 버튼을 눌렀을 경우 처리
void CMCMain::SetBackPatternDisplay(int nJig)
{
	// nBIN2TestState;			// BIN2 패턴 검사 모든 : 0 : 일반적인 Next/Back, 1 : BIN2 패턴 표시, 2 : Next/Back 금지 모드
	CSvcTcpPG* pSvcTcpPG	= (CSvcTcpPG*)( (CGxServiceHub::GetInstance())->GetService(NAME_SERVICE_TcpPG));						// 신호기 통신

	int i;
	int nNextIdx = 0;
	int nPtnNo = 0;
	TCellData* pCell = NULL;

	for (i = 0; i < eMAX_ChCount; i++ )
	{
		pCell = m_pCellInfo->GetCellData(nJig, i); 
		if ( pCell )
		{
			if ( pCell->bIsExist )						// Cell이 존재하는 조건으로,
			{
				if ( pCell->nBIN2TestState == 0 )		// 일반적인 Next/Back 적용 모드
				{
					pSvcTcpPG->PG_SendBack(nJig, i);				// 지정 지그의 CH에 BACK 전송
				}
				else if ( pCell->nBIN2TestState == 1 )	// BIN2 패턴 표시 모드
				{
					nNextIdx =pCell->nBIN2PtnIdx - 1;													// 현재 선택된 패턴 이전 순번 

					if ( (nNextIdx >= 0) && (nNextIdx < pCell->arrPtnIndex.GetCount()) )	// 합당한 범위인지 점검
					{
						nPtnNo = pCell->arrPtnIndex[ nNextIdx ];										// 패턴 Index를 추출한다.
						
						pSvcTcpPG->PG_SendPatternSelect(nJig, i, nPtnNo + 1);					// BIN2 패턴을 지정해준다.

						pCell->nBIN2PtnIdx = nNextIdx;													// 다음 순번을 지정한다.
					}
				}

				// 그밖에는 반응을 하지 않는다.
			}
		}//of if pCell
	}//of for i

}

//
//// 지정 패턴의 실제 색 값을 가져온다.
//void CMCMain::GetPatternColor(CString sPtnName, COLORREF &colorBack, COLORREF &colorFore)
//{
//	// GRAY81, GRAY29 추가 필요, 2015-02-25, 김상묵 사원요청
//	if ( sPtnName == _T("GRAY127") )
//	{
//		colorBack = COLOR_PTN_GRAY127;			// 배경색
//		colorFore = COLOR_PTN_WHITE;				// 원 색상
//	}
//	else if ( sPtnName == _T("GRAY87") )
//	{
//		colorBack = COLOR_PTN_GRAY87;				// 배경색
//		colorFore = COLOR_PTN_WHITE;				// 원 색상
//	}
//	else if ( sPtnName == _T("GRAY32") )
//	{
//		colorBack = COLOR_PTN_GRAY32;				// 배경색
//		colorFore = COLOR_PTN_WHITE;				// 원 색상
//	}
//	else if ( sPtnName == _T("WHITE") )
//	{
//		colorBack = COLOR_PTN_WHITE;				// 배경색
//		colorFore = COLOR_PTN_BLACK;				// 원 색상
//	}
//	else if ( sPtnName == _T("RED") )
//	{
//		colorBack = COLOR_PTN_RED;				// 배경색
//		colorFore = COLOR_PTN_BLACK;				// 원 색상
//	}
//	else if ( sPtnName == _T("GREEN") )
//	{
//		colorBack = COLOR_PTN_GREEN;				// 배경색
//		colorFore = COLOR_PTN_BLACK;				// 원 색상
//	}
//	else if ( sPtnName == _T("BLUE") )
//	{
//		colorBack = COLOR_PTN_BLUE;				// 배경색
//		colorFore = COLOR_PTN_BLACK;				// 원 색상
//	}
//	else
//	{
//		colorBack = COLOR_PTN_WHITE;				// 배경색
//		colorFore = COLOR_PTN_BLACK;				// 원 색상
//	}
//
//}



// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==


//
// 지정 시간을 모든 JIG의 Cell들에게 적용한다. (기존 시간에 계속해서 누적시킨다.)
//
// 본 함수는 CMainFrame::OnChangeState()에서 설비 상태가 바뀌면 호출되어진다.
//
void CMCMain::AddCellLossTime(double dbTime)
{
	TCellData* pCell;

	// 모든 Cell에 대하여
	for (int i = 0; i < eMAX_JigCount; i++)
	{
		for (int j = 0; j < eMAX_ChCount; j++)
		{
			pCell = m_pCellInfo->GetCellData(i, j);					// 지정 Jig의 각 채널의 포인트를 얻어온다.

			// 회전 동작 전 대기상태일 경우 WaitTime과 LossTime이 중복되어 계산되므로 LossTime은 추가하지 않는다.
			if ( pCell->bWaitStep == FALSE )			// Turn 동작 전 대기 시간인가 ? FALSE: 대기상태 아님
			{
				// 지정한 시간을 비가동 손실 시간에 누적시킨다.
				pCell->dbMoveTACT[eTACT_LossTime] = pCell->dbMoveTACT[eTACT_LossTime] + dbTime;

				// 현재 작업자 동작 상태인지, 설비 동작 상태인지 판단하여 해당 작업의 비가동 시각을 누적시킨다.
				// 설비 동작 모드인가 ? FALSE: OP 동작상태, TRUE : EQ 동작 상태, 지금부터는 OP 동작상태로 간주
				if ( pCell->bEQWorkMode == TRUE )
				{
					pCell->dbMoveTACT[eTACT_LossInEQTime] = pCell->dbMoveTACT[eTACT_LossInEQTime] + dbTime;
				}
				else
					pCell->dbMoveTACT[eTACT_LossInOPTime] = pCell->dbMoveTACT[eTACT_LossInOPTime] + dbTime;
			}//of if ( pCell->bWaitStep == FALSE )
		}//of for j
	}

}


// 2015-04-18, jhLee, 현재 미사용, 실제 컨텍한 수량을 그대로 가지고 있도록한다. (노민종 사원) 
//  CMainFrame::OnChangeState(WPARAM wParam, LPARAM lParam) 에서 사용됨
// 지정 지그의 Cell 데이터들의 Contact Count 값을 초기화 시킨다 (작업자 양수 버튼이전의 경우)
//
void CMCMain::ClearContactCount()
{
	CSTNWorkTable* pWorkTable	= (CSTNWorkTable *)((CGxStationHub::GetInstance())->GetStation( NAME_STATION_WorkTable ));	// WorkTable Station pointer
	int nJig = pWorkTable->GetAZoneJig();						// 현재 A-Zone에 있는 Jig
	
	TCellData* pCell;

	for (int j = 0; j < eMAX_ChCount; j++)
	{
		pCell = m_pCellInfo->GetCellData(nJig, j);					// 지정 Jig의 각 채널의 포인트를 얻어온다.

		// A-Zone에서 아직 양수 버튼이 눌리기 전에 설비가 STOP -> AUTO로 전환되었다면, 
		if ( (pCell->bEQWorkMode != TRUE) && (pCell->nCurrentZone == eIDX_PG_ZONE_A)  )
		{
			if ( pCell->ContactCount > 1 ) pCell->ContactCount = 1;		// 다량의 Contact이 있었어도 1회로 초기화 한다.
		}
	}
}


// STOP -> AUTO로 상태 변경시 Loading시의 PG-OFF 시간을 현재 시각에 맞춰서 변경한다.
void CMCMain::UpdateLoadPGOffTime()
{
	CSTNWorkTable* pWorkTable	= (CSTNWorkTable *)((CGxStationHub::GetInstance())->GetStation( NAME_STATION_WorkTable ));	// WorkTable Station pointer
	int nJig = pWorkTable->GetAZoneJig();						// 현재 A-Zone에 있는 Jig
	
	TCellData* pCell;

	for (int j = 0; j < eMAX_ChCount; j++)
	{
		pCell = m_pCellInfo->GetCellData(nJig, j);					// 지정 Jig의 각 채널의 포인트를 얻어온다.

		if ( pCell != NULL )
		{
			// 아직 신호기로 Turn On 명령을 전송한 이력이 없고 현재 A-Zone 상태라면
			if ( (pCell->TurnOnStartFlag == FALSE) && (pCell->nCurrentZone == eIDX_PG_ZONE_A)  )
			{
				pCell->SetTACTTime( eTACT_LoadPGOff );						// 현재 시각을 Loading PG-OFF 시각 지정
			}

		}
	}//of for i
}





////////////////////////////////////////////////////////////////////////////////////////////////////
// 설비 운영에 기본이 되는 값들을 관리한다.
//
// 본 함수를 호출하기 전에 반드시 모든 동적 생성 객체들이 미리 할당되어있어야 정상적인 동작을 보장한다. ( CellInfo , JigInfo, Station 등)
void CMCMain::InitProfile()
{
	CSTNWorkTable* pWorkTable	= (CSTNWorkTable *)((CGxStationHub::GetInstance())->GetStation( NAME_STATION_WorkTable ));	// WorkTable Station pointer
	CSTNAVCamera*	pAVCamera	= (CSTNAVCamera*)((CGxStationHub::GetInstance())->GetStation( NAME_STATION_AVCamera ));
	CSTNPreAlign*	pPreAlign	= (CSTNPreAlign*)((CGxStationHub::GetInstance())->GetStation( NAME_STATION_PreAlign ));


	CString sKey;
	int i, j;

	// 각종 경로 지정
	m_sPath_Data		= m_sBasePath + _T("\\Data\\");										// 설정값을 저장하는 data 폴더
	m_sPath_INI			= m_sBasePath + _T("\\INI\\");										// INI 설정값을 저장하는 폴더
	m_sPath_Log			= m_sBasePath + _T("\\Log\\");										// Log를 기록하는 폴더
	m_sPath_UnitImg	= m_sBasePath + _T("\\Data\\Image\\Unit\\");;					// 각 Unit들의 이미지가 존재하는 경로
	m_sPath_Runtime	= m_sBasePath + _T("\\Data\\Image\\Runtime\\");					// 동작시 생성되는 이미지 경로(Align, MCR 이미지)


	// profile ini file 설정 
	m_iniProfile.m_sIniFile.Format(_T("%s\\Data\\Profile\\MCMain.ini"), m_sBasePath);		// Profile 파일 경로 지정


	// 2015-03-23, jhLee, 각종 경로명을 변경할 수 있도록 옵션으로 관리한다.
	m_iniProfile.AddStr(_T("Path:DIR_LossTime"),		m_rProfileData.sDIR_LossTime);		// #define NAME_DIR_LOSSTIME	_T("D:\\SIRIUS\\LOG\\MAIN")				// 비가동 Log 폴더
	m_iniProfile.AddStr(_T("Path:DIR_CellLog"),		m_rProfileData.sDIR_CellLog);			// #define NAME_DIR_CELLLOG		_T("D:\\AUTO_TEST\\LOG")				// Cell Log 폴더
	m_iniProfile.AddStr(_T("Path:DIR_MachineData"),	m_rProfileData.sDIR_MachineData);		// D:\Machine_data 폴더

	m_iniProfile.AddStr(_T("Path:DIR_TACTLog"),		m_rProfileData.sDIR_TACTLog);			// #define NAME_DIR_TACTLOG		_T("D:\\Machine_Data\\TactTime_Data")	// TACT Time Log 폴더, 2014-10-14, 백윤기S 지정
	m_iniProfile.AddStr(_T("Path:DIR_AlarmLog"),		m_rProfileData.sDIR_AlarmLog);		// #define NAME_DIR_ALARMLOG	_T("D:\\Machine_Data\\Alarm_Data")		// Alarm 발생 Log 폴더, jhLee
	m_iniProfile.AddStr(_T("Path:DIR_DataPC_Cell"),	m_rProfileData.sDIR_DataPC_Cell);		// #define NAME_DIR_DATAPC_CELL	_T("D:\\UB-AMT\\DATA_PC\\CELL")			// Data PC로 전송 할 Cell 임시 정보 (Tack-out 정보)
	m_iniProfile.AddStr(_T("Path:DIR_DataPC_FDC"),	m_rProfileData.sDIR_DataPC_FDC);		// #define NAME_DIR_DATAPC_FDC	_T("D:\\UB-AMT\\DATA_PC\\FDC")			// Data PC로 전송 할 FDC 실시간 정보 
	m_iniProfile.AddStr(_T("Path:DIR_GMSLog"),		m_rProfileData.sDIR_GMSLog);			// GMS로 부터 수신된 데이터 기록용
	m_iniProfile.AddStr(_T("Path:DIR_MCRImage"),		m_rProfileData.sDIR_MCRImage);		// MCR Live 및 이미지 파일을 저장할 경로
	m_iniProfile.AddStr(_T("Path:DIR_GMSDataFile"),		m_rProfileData.sDIR_GMSDataFile);	// 파일을 통한 GMS 데이터 취득용 경로 및 파일이름


	// 검사PC 대상 경로
	m_iniProfile.AddStr(_T("Path:DIR_VIResult"),	m_rProfileData.sDIR_VIResult);					// 검사PC에서 테스트 결과를 가져올 경로
	m_iniProfile.AddStr(_T("Path:DIR_VIRecipe"),	m_rProfileData.sDIR_VIRecipe);					// 검사PC에게 변경된 레서피를 전달해줄 폴더
	m_iniProfile.AddStr(_T("Path:DIR_VIInform"),	m_rProfileData.sDIR_VIInform);					// 검사PC에서 해상도관련 스펙을 가져올 경로

	//Contact율 파일 경로
	m_iniProfile.AddStr(_T("Path:DIR_ProductLog"),	m_rProfileData.sDIR_ProductLog);				// 생산율 관련 정보를 기록할 경로 ( 김정유D 요청)



	// 각종 설비 설정값 관리
	//Hedaer file에서 정의된 값 사용 m_iniProfile.AddStr(_T("Setting:Version"),		m_rProfileData.sVersion);		// Program version : Y_A_MT_14.05.17.01
	m_iniProfile.AddStr(_T("Setting:EQID"),			m_rProfileData.sEQID);			// 설비 ID
	m_iniProfile.AddStr(_T("Setting:ModelName"),	m_rProfileData.sModelName);	// 생산 모델명
	m_iniProfile.AddStr(_T("Setting:PPID"),			m_rProfileData.sPPID);			// PPID

	m_iniProfile.AddInt(_T("Setting:Language"),		m_nLanguageType);				// 언어 설정


	// Cell log 관련
	m_iniProfile.AddStr(_T("CellLog:CellLogPath"),	m_rProfileData.sCellLog_Path);	// Cell log를 남기는 기본 폴더,
	m_iniProfile.AddStr(_T("CellLog:LineID"),		m_rProfileData.sCellLog_LineID);	// MINS에서 사용되는 라인내 설비 ID, 3자리, Cell-Log 파일이름에 사용된다.
	m_iniProfile.AddUint(_T("CellLog:DayHour"),		m_rProfileData.nCellLog_Hour);	// Cell Log 파일 생성에 기준이되는 하루의 시작 시간 00 ~ 23
	m_iniProfile.AddUint(_T("CellLog:DayMin"),		m_rProfileData.nCellLog_Min);		// Cell Log 파일 생성에 기준이된느 하루의 사직 분 00 ~ 59


	m_iniProfile.AddBool(_T("Setting:bIsSimulate"),	m_bIsSimulate);					// Simulation mode 인가 ?
	m_iniProfile.AddBool(_T("Setting:bIsDryRun"),	m_bIsDryRun);						// DryRun mode인가 ?

	m_iniProfile.AddBool(_T("Setting:CycleStop"),	m_bCycleStop);						// Cycle stop 여부
	m_iniProfile.AddBool(_T("Setting:AlarmStop"),	m_bAlarmStop);						// Alarm 발생시 모든 축 정지 여부


	// 두 Mark간 거리는 Recipe에서 기록하고 관리한다.
	m_iniProfile.AddDouble(_T("Align:ScoreLimit"),	pPreAlign->m_dbAlignScoreLimit);	// Find Mark 합격/불합격 판단 기준값, 통상 60 정도로 한다. - Profile로 관리한다.
	m_iniProfile.AddDouble(_T("Align:ThetaLimit"),	pPreAlign->m_dbAlignThetaLimit);	// Align 결과 회전각도 제한 합격/불합격 판단 기준값, 통상 +/- 6 정도로 한다. : (Profile로 관리)
	m_iniProfile.AddUint(_T("Align:RetryCount"),	pPreAlign->m_nAlign_ReTryCount);	// Align 동작을 몇회까지 자동으로 재시도 할 것인가 ? (Profile로 관리)

	// PreAlign의 조명 밝기값
	m_iniProfile.AddUint(_T("LightDim:Ch1"),		pPreAlign->m_nLightDim[eIDX_LIGHT_CH1]);	// Ch1 조명 Align 밝기값		
	m_iniProfile.AddUint(_T("LightDim:Ch2"),		pPreAlign->m_nLightDim[eIDX_LIGHT_CH2]);	// Ch2 조명 Align 밝기값		
	m_iniProfile.AddUint(_T("LightDim:Up"),			pPreAlign->m_nLightDim[eIDX_LIGHT_UP]);		// 상부 조명 Align 밝기값		

	// Cell MCR을 읽기위한 PreAlign의 조명 밝기값
	m_iniProfile.AddUint(_T("LightDim_MCR:Ch1"),	pPreAlign->m_nLightDim_MCR[eIDX_LIGHT_CH1]);	// Ch1 조명 Align 밝기값		
	m_iniProfile.AddUint(_T("LightDim_MCR:Ch2"),	pPreAlign->m_nLightDim_MCR[eIDX_LIGHT_CH2]);	// Ch2 조명 Align 밝기값		
	m_iniProfile.AddUint(_T("LightDim_MCR:Up"),		pPreAlign->m_nLightDim_MCR[eIDX_LIGHT_UP]);		// 상부 조명 Align 밝기값		

	// UV-MCR 관련
	m_iniProfile.AddUint(_T("UVMCR:MCRTimeout"),		pPreAlign->m_nMCRTimeout);			// MCR 리딩 대기 시간, (Profile로 관리)
	m_iniProfile.AddUint(_T("UVMCR:MCRTriggerDelay"),	pPreAlign->m_nMCRTriggerDelay);	// MCR 리딩 트리거 신호 유지 시간, (Profile로 관리)
	m_iniProfile.AddUint(_T("UVMCR:RetryCount"),		pPreAlign->m_nMCR_ReTryCount);	// MCR을 몇회까지 자동으로 리딩 시도를 할 것인가 ? (Profile로 관리)
	m_iniProfile.AddUint(_T("UVMCR:RetryDelay"),		pPreAlign->m_nMCRRetryDelay);		// 2014-09-09, jhLee, 고민오S 요청, 자동 Retry 간격 시간을 지정한다. (Profile로 관리)


	// 신호기 관련
	m_iniProfile.AddUint(_T("PG:CellLoadTimeout"),	pPreAlign->m_nPG_CellLoadTimeout);		// CELL_LOAD 시간 초과 감지
	m_iniProfile.AddUint(_T("PG:TSPTimeout"),		pPreAlign->m_nPG_TSPTimeout);				// TSP 결과 시간초과
	m_iniProfile.AddUint(_T("PG:CurrentTimeout"),	pPreAlign->m_nPG_CurrentTimeout);		// 각종 전류 측정 시간초과

	// 검사PC 관련
	m_iniProfile.AddUint(_T("VISetting:GrabTimeout"),	pAVCamera->m_nVIGrabTimeout);			// 검사PC GRAB 명령에 대한 Timeout 설정 (Profile로 관리)
	m_iniProfile.AddUint(_T("VISetting:ResultTimeout"),	pAVCamera->m_nVIResultTimeout);		// 검사PC 최종 판결 명령에 대한 Timeout 설정 (Profile로 관리)


	// WorkTable T축 위치 
	m_iniProfile.AddDouble(_T("WorkTable:Return"),	pWorkTable->m_dbPosData[ePOS_WORKTABLE_Return]);	// WorkTable T축, 원점 위치 (Return)
	m_iniProfile.AddDouble(_T("WorkTable:Turn"),	pWorkTable->m_dbPosData[ePOS_WORKTABLE_Turn]);		// WorkTable T축, 회전 위치 (Turn)

	// 각 채널의 사용/미사용 설정 상태
	for (i = 0; i < eMAX_JigCount; i++)
		for (j = 0; j < eMAX_ChCount; j ++)
		{
			sKey.Format(_T("ChannelNotUse:JIG%d_CH%d"), i+1, j+1);						// 지정 지그-채널
			m_iniProfile.AddBool(sKey,	m_rProfileData.bChannelNotUseFlag[i][j]);		// 채널 미사용 설정 여부, 0:사용, 1:미사용
		}


	// TCP/IP 통신 설정, 각 접속 통신마다 설정값을 가진다.
	for ( i = 0; i < eMAX_COMMCount; i++ )
	{
		sKey.Format(_T("TCPComm_%d:Name"), i);
		m_iniProfile.AddStr( sKey,			m_rProfileData.sTCPName[i]);					// 통신주소

		sKey.Format(_T("TCPComm_%d:Symbol"), i);
		m_iniProfile.AddStr( sKey,			m_rProfileData.sTCPSymbol[i]);				// ID

		sKey.Format(_T("TCPComm_%d:IPAddress"), i);
		m_iniProfile.AddStr( sKey,			m_rProfileData.sTCPAddress[i]);				// IP주소

		sKey.Format(_T("TCPComm_%d:Port"), i);
		m_iniProfile.AddUint( sKey,			m_rProfileData.nTCPPort[i]);				// Port번호
	}

	m_iniProfile.AddUint( _T("SerialComm:PrinterPort"),		m_rProfileData.nPrinterPort);			// Label Printer의 COM Port
	m_iniProfile.AddUint( _T("SerialComm:VACPort"),				m_rProfileData.nVACPort);				// 진공 A/D 보드의 COM Port
	m_iniProfile.AddUint( _T("SerialComm:PCTempPort"),			m_rProfileData.nPCTempPort);			// PC-RACK 온도의 COM Port
	m_iniProfile.AddUint( _T("SerialComm:AlignLightPort"),	m_rProfileData.nAlignLightPort);		// Align 조명 컨트롤러 COM Port
	m_iniProfile.AddUint( _T("SerialComm:PanasonicPort"),		m_rProfileData.nPanasonicPort);			// Panasonic Servo pack 연결을 위한 COM Port
	m_iniProfile.AddUint( _T("SerialComm:GMSPort"),				m_rProfileData.nGMSPort);				// 접지 저항을 재기위한 GMS 통신 COM Port
	m_iniProfile.AddUint( _T("SerialComm:YaskawaPort"),		m_rProfileData.nYaskawaPort);			// Yaskawa DD Motor Tourque 수집을 위한 COM Port


	
	// Option 항목
	// 각종 검사 항목 실행 여부 (option으로 처리)
	m_iniProfile.AddBool(_T("Option_Use:bUseTest_VI"),				m_rProfileData.bUseTest_VI);			// C-Zone에서 AV검사를 진행할 것인지 여부,
	m_iniProfile.AddBool(_T("Option_Use:bUseTest_VIForce"),			m_rProfileData.bUseTest_VIForce);		// A-Zone에서 Defect 발생시에도 C-Zone에서 AV검사를 진행할 것인지 여부,
	m_iniProfile.AddBool(_T("Option_Use:bUseTest_Manual"),			m_rProfileData.bUseTest_Manual);		// D-Zone 작업자 검사를 진행할 것인가 ?
	m_iniProfile.AddBool(_T("Option_Use:bUseTest_TSP"),				m_rProfileData.bUseTest_TSP);			// C-Zone에서 TSP 검사를 진행할 것인지 여부
	m_iniProfile.AddBool(_T("Option_Use:bUseTest_IDCheck"),			m_rProfileData.bUseTest_IDCheck);		// IDCHECK 검사를 진행할 것인가 ?
	m_iniProfile.AddBool(_T("Option_Use:bUseTest_Sleep"),			m_rProfileData.bUseTest_Sleep);			// Sleep Current검사를 진행할 것인가 ?
	m_iniProfile.AddBool(_T("Option_Use:bUseTest_White"),			m_rProfileData.bUseTest_White);			// White Current 검사를 진행할 것인가 ?
	m_iniProfile.AddBool(_T("Option_Use:bUseTest_BIN2TestMode"),		m_rProfileData.bUseTest_BIN2TestMode);	// 2015-03-06, jhLee, 김상묵 사원요청, TEST Cell 작업 모드, 별도의 Cell log로 남기고 상위 보고를 하지 않는다.
	m_iniProfile.AddBool(_T("Option_Use:bUseTest_OpenCellMode"),	m_rProfileData.bUseTest_OpenCellMode);	// 2015-03-12, jhLee, 김상묵 사원요청, Open Cell mode인지 여부, 기본은 FALSE(Normal Cell)
	m_iniProfile.AddBool(_T("Option_Use:bUseTest_TECheck"),			m_rProfileData.bUseTest_TECheck);		// 2015-03-28, jhLee, 김상묵 사원요청, C-Zone에서 TE_CHECK 를 진행한다.
	m_iniProfile.AddBool(_T("Option_Use:bUseTest_ModelCheck"),		m_rProfileData.bUseTest_ModelCheck);	// 2015-05-21, JH-PARK 검정유D, 김상묵S 요청 , Cell Model 투입시 현재 Model 구동여부를 판단한다;


	m_iniProfile.AddBool(_T("Option_Use:bUse_CircleDisp"),			m_rProfileData.bUse_CircleDisp);		// D-Zone Test시 VI Defect을 원으로 그려서 보여줄 것인가 ?
	m_iniProfile.AddBool(_T("Option_Use:bUse_LabelPrinter"),		m_rProfileData.bUse_LabelPrinter);		// Label Printer를 이용하여 출력을 할 것인가 ?
	m_iniProfile.AddBool(_T("Option_Use:bUse_KeepVIResult"),		m_rProfileData.bUse_KeepVIResult);		// VI Result file을 유지할 것인가 ?
	m_iniProfile.AddBool(_T("Option_Use:bUse_ContactCurrentCheck"),	m_rProfileData.bUse_ContactCurrentCheck);	// 2014-11-25, jhLee, Contact Current NG를 체크하여 Loading을 Cancel시킬 것인가 ?
	m_iniProfile.AddBool(_T("Option_Use:bUse_OPLosstimeAlert"),		m_rProfileData.bUse_OPLosstimeAlert);	// 2014-12-10, jhLee, 작업자 작업시간초과 발생시 경보를 울릴것인가 ?
	m_iniProfile.AddBool(_T("Option_Use:bUse_CellExcept"),			m_rProfileData.bUse_CellExcept);		// 2014-12-22, jhLee, 특정 Cell-ID는 검사를 수행하지 않고 제외 시키도록 한다.

	m_iniProfile.AddBool(_T("Option_Use:bUse_SkipJigTilt"),			m_rProfileData.bUse_SkipJigTilt);		// JIG를 Tilt 하지 않고 사용할 것인가 ? (TACT 감소를 위해)
	m_iniProfile.AddBool(_T("Option_Use:bUse_RTracking"),			m_rProfileData.bUse_RTracking);			// LOSS도 R로 보고 할것인가 ?
	m_iniProfile.AddBool(_T("Option_Use:bUse_SkipMCR"),				m_rProfileData.bUse_SkipMCR);			// UV-MCR을 읽지 않고 가상의 Cell ID를 생성하여 사용할 것인가 ?

	m_iniProfile.AddBool(_T("Option_Use:bUse_SkipEngineerTest"),	m_rProfileData.bUse_SkipEngineerTest);	// 2014-11-07, 엔지니어 로그인으로도 검사를 수행할 수 있도록 한다 - 기본값 FALSE.(김길성 책임 요청 회피용)
	m_iniProfile.AddBool(_T("Option_Use:bUse_AutoContactOff"),		m_rProfileData.bUse_AutoContactOff);	// 이전 검사가 모두 양품일 경우 별도의 Defect 입력화면을 띄우지 않고 PG-OFF 시킨다.
	m_iniProfile.AddBool(_T("Option_Use:bUse_SkipCommFail"),		m_rProfileData.bUse_SkipCommFail);		// 2014-12-10, 통신 접속이 끊어져도 무시할 것인가 ? TRUE:무시, FALSE:해당 통신의 Alarm을 발생시켜준다.

	// 보이지 않는 옵션
	m_iniProfile.AddBool(_T("Option_Use:bUse_AutoMoveRetry"),		m_rProfileData.bUse_AutoMoveRetry);		// 2015-03-10, 모션 이동관련 에러 발생시 자동으로 재시도를 수행하게 한다. 주로 NOT Inpos 알람에 대응하기 위함.
	m_iniProfile.AddBool(_T("Option_Use:bUse_CellLoadingCmd"),		m_rProfileData.bUse_CellLoadingCmd);	// 2015-03-28, TACT을 줄이기위해 PowerOn 버튼으로 기존 POWER_ON을 대신하여 Cell Loading 명령을 사용할 것인가 ? TRUE:CellLoading명령 사용, FALSE:기존대로 POWER_ON 사용
	m_iniProfile.AddBool(_T("Option_Use:bUse_LoginServerRequest"),	m_rProfileData.bUse_LoginServerRequest);// 2015-04-16, 작업자 Login/out시 상위서버에게 인증을 요청하도록 할 것인가 ?
	m_iniProfile.AddBool(_T("Option_Use:bUse_JudgeCellExcept"),		m_rProfileData.bUse_JudgeCellExcept);	// 2015-06-21 JH-PARk 판정 완료된 Cell에 대하여 투입을 진행할 것인가? 
	m_iniProfile.AddBool(_T("Option_Use:bUse_TMDVersionCheck"),		m_rProfileData.bUse_TMDVersionCheck);	// 2015-07-16, jhLee, 각 채널별 TMD Version을 관리할것인가 ?
	m_iniProfile.AddBool(_T("Option_Use:bUse_UpperBackLight"),		m_rProfileData.bUse_UpperBackLight);	// 2015-12-26, jhLee, 상부 백라이트를 이용하여 Cell MCR 리딩시 조명으로 사용한다.

	// 2015-10-15, TMD Model명과 Product ID 비교 기능 추가건
	m_iniProfile.AddBool(_T("Option_Use:bUse_TMDModelCheck"),		m_rProfileData.bUse_TMDModelCheck);		// 2015-10-15, jhLee, 신호기 TMD모델명과 Product ID와 비교하여 Alarm 처리할 것인가 (SDC 백윤기사원 요청)
	m_iniProfile.AddBool(_T("Option_Use:bUse_SkipAutoShutdown"),	m_rProfileData.bUse_SkipAutoShutdown);	// 2015-10-20, jhLee, 신호기 TMD모델명과 Product ID가 일치하지 않을경우 설비를 Down시키는 것을 비활성화 시킬것인가 ?
	m_iniProfile.AddBool(_T("Option_Use:bUse_SkipAutoTrackout"),	m_rProfileData.bUse_SkipAutoTrackout);	// 2015-10-20, jhLee, 설비가 DOWN될 때 Track-in된 Cell을 자동으로 RETEST로 Track-Out하는 기능을 수행하지 않게할 것인가 ?
	m_iniProfile.AddBool(_T("Option_Use:bUse_BIN2PatternTest"),		m_rProfileData.bUse_BIN2PatternTest);	// 2016-05-26, 메뉴얼 검사에 C-Zone 패턴을 이용한다

	m_iniProfile.AddBool(_T("Option_Use:bUse_DefectPosition"),		m_rProfileData.bUse_DefectPosition);	// 2016-11-20, jhLee, Defect이 발생한 위치를 입력받는 기능을 수행할 것인가 ?
	m_iniProfile.AddBool(_T("Option_Use:bUse_GMSFileData"),			m_rProfileData.bUse_GMSFileData);			// 2016-11-20, jhLee, GMS Data를 file을 통해 취득한다.
	m_iniProfile.AddBool(_T("Option_Use:bUse_BoxFanAlarm"),			m_rProfileData.bUse_BoxFanAlarm);			// 2016-11-25, jhLee, 각종 BOX에 부착된 FAN의 동작상태 Alarm 기능을 이용한다.


	m_iniProfile.AddInt(_T("Option_Value:nTIME_CylinderTimeout"),	m_rProfileData.nTIME_CylinderTimeout);	// Cylinder 동작 Timeout 밀리초
	m_iniProfile.AddInt(_T("Option_Value:nTIME_VacuumTimeout"),		m_rProfileData.nTIME_VacuumTimeout);	// Vacuum 동작 Timeout 밀리초
	m_iniProfile.AddInt(_T("Option_Value:nTIME_UnloadVACOff"),		m_rProfileData.nTIME_UnloadVACOff);		// Unload 동작시 진공 파기 지연 시간
	m_iniProfile.AddInt(_T("Option_Value:nTIME_RunDownTime"),		m_rProfileData.nTIME_RunDownTime);		// 입력이 없을 시 RunDown으로 전환시키는 시간, 초
	m_iniProfile.AddInt(_T("Option_Value:nTIME_OPLossTime"),			m_rProfileData.nTIME_OPLossTime);		// 작업자 손실시간 (OP Loss) 판단 기준 시간, 초;
	m_iniProfile.AddInt(_T("Option_Value:nTIME_EQLossTime"),			m_rProfileData.nTIME_EQLossTime);		// 설비동작 손실시간 (EQ Loss) 판단 기준 시간, 초
	m_iniProfile.AddInt(_T("Option_Value:nTIME_MDChangeTime"),		m_rProfileData.nTIME_MDChangeTime);		// MD/BM 자동전환 기준 시간, 초 (60초?)
	m_iniProfile.AddInt(_T("Option_Value:nTIME_VTOffsetTime"),		m_rProfileData.nTIME_VTOffsetTime);		// Vistion 검사결과 파일 대기 시간에서 추가로 더 대기 할 추가시간 (초)
	m_iniProfile.AddInt(_T("Option_Value:nTIME_TECheckTime"),		m_rProfileData.nTIME_TECheckTime);		// TE Check Timeout 기준 시각을 지정 (초)
	m_iniProfile.AddInt(_T("Option_Value:nTIME_LoginServerTimeout"),m_rProfileData.nTIME_LoginServerTimeout);	// 2015-04-16, jhLee, 작업자 Login/out 요청에 대한 서버의 응답시간 제한 (초)
	m_iniProfile.AddInt(_T("Option_Value:nTIME_MotorMoveCheckTime"),m_rProfileData.nTIME_MotorMoveCheckTime);	// 2016-06-21 JH-PARk, Motor 이동 완료 Retry 진행시 Delay 시간 (밀리초)
	m_iniProfile.AddInt(_T("Option_Value:nTIME_DispResultCell"),	m_rProfileData.nTIME_DispResultCell);	// 2015-07-01 JH-PARK, 판정완료된 Cell의 값을 표시해주는 시간 (초)
	m_iniProfile.AddInt(_T("Option_Value:nTIME_TSPTimeout"),			m_rProfileData.nTIME_TSPTimeout);		// 2016-04-21, TSP Timeout 기준 시각을 지정 (초)
	m_iniProfile.AddInt(_T("Option_Value:nTIME_DeepCheck"),			m_rProfileData.nTIME_DeepCheck);			// 2016-11-13, 심층검사 시간을 지정 (초)
	
	m_iniProfile.AddInt(_T("Option_Value:nTIME_VI_GrabEnd"),			m_rProfileData.nTIME_VI_GrabEnd);		// Vision Grab End 회신 Timeout
	m_iniProfile.AddInt(_T("Option_Value:nTIME_VI_ClassEnd"),		m_rProfileData.nTIME_VI_ClassEnd);	// Vision Class End 회신 Timeout


	m_iniProfile.AddInt(_T("Option_Value:nCNT_DispCircleCount"),	m_rProfileData.nCNT_DispCircleCount);	// D-Zone에서 Cell에 표시할 Defect circle 수량 (개)

	m_iniProfile.AddInt(_T("Option_Mode:nMode_AlignImageSave"),		m_rProfileData.nMode_AlignImageSave);	// Align 이미지 저장 모드 0:None, 1:OK, 2:NG, 3:All
	m_iniProfile.AddInt(_T("Option_Mode:nMode_UVMCRImageSave"),		m_rProfileData.nMode_UVMCRImageSave);	// UVMCR 이미지 저장 모드 0:None, 1:OK, 2:NG, 3:All
	m_iniProfile.AddInt(_T("Option_Mode:nMode_WorkShift"),			m_rProfileData.nMode_WorkShift);		// 작업자 근무표시 저장 모드 0:3개조 , 1:2개조
	m_iniProfile.AddInt(_T("Option_Mode:nMode_DispVIResult"),		m_rProfileData.nMode_DispVIResult);		// 검사결과 표시 방법     0: Message , 1 : Pattern

	// 직접 ini 파일을 편집해주는 설정값
	m_iniProfile.AddInt(_T("Option_Value:nVALUE_PrinterLeft"),		m_rProfileData.nVALUE_PrinterLeft);		// Label Printer 왼쪽 여백
	m_iniProfile.AddInt(_T("Option_Value:nVALUE_TempPCRack"),		m_rProfileData.nVALUE_TempPCRack);		// PC랙 온도 이상 판단 기준 온도 


	//
	// 실행 내역 저장 file 설정 
	m_iniRuntime.m_sIniFile.Format(_T("%s\\Data\\RunTime\\%s"),	m_sBasePath, NAME_FILE_RUNTIME_DATA);			// data 파일 경로 지정
	m_iniRuntime.AddStr(_T("TimeData:sLoginTime"),		m_rRuntimeData.sUserLoginTime );					// 로그인 시각
	m_iniRuntime.AddStr(_T("TimeData:sEQStart"),		m_rRuntimeData.sEQStartTime );					// 설비 시작 시각
	m_iniRuntime.AddStr(_T("TimeData:sEQStop"),			m_rRuntimeData.sEQStopTime );						// 설비 중지 시각

	// 종료당시의 상태 저장
	m_iniRuntime.AddInt(_T("EQState:nAbnormalTerminate"),	m_rRuntimeData.nAbnormalTerminate);		// 프로그램이 비정상적으로 종료를 하였는가 ?
	m_iniRuntime.AddStr(_T("EQState:sLastUserID"),			m_rRuntimeData.sLastUserID);			// 최종 접속된 작업자 정보
	m_iniRuntime.AddStr(_T("EQState:sLastUserPassword"),	m_rRuntimeData.sLastUserPassword);		// 최종 접속된 작업자 암호

}



// Profile 데이터를 읽어온다.
BOOL CMCMain::LoadProfile()
{
	// file이 존재하는지 점검한다.
	if ( -1 != GetFileAttributes( m_iniProfile.m_sIniFile ) ) 
	{
		m_iniProfile.Load();
		return TRUE;
	}

	return FALSE;				// 파일이 존재하지 않는다.
}


// Profile 데이터를 값 그룹별로 저장한다.
void CMCMain::SaveProfile(CString sGroup)
{
	if ( !sGroup.IsEmpty() )		// 저장하고픈 Section 그룹이 지정되었다면,
	{
		m_iniProfile.Save( sGroup );
		return;
	}

	// 모든 내용을 저장한다.
	m_iniProfile.Save();	
}


// 현재 설정된 비가동 정보를 파일로 기록한다.
void CMCMain::SaveRuntimeData()
{
	// ini file로 저장하기 위한 변수
	m_rRuntimeData.sUserLoginTime	= CGxUtility::SystemTimeToString( m_rRuntimeData.tmUserLogin );	
	m_rRuntimeData.sEQStartTime		= CGxUtility::SystemTimeToString( m_rRuntimeData.tmEQStart );	
	m_rRuntimeData.sEQStopTime		= CGxUtility::SystemTimeToString( m_rRuntimeData.tmEQStop );

	//d m_rRuntimeData.sPrevDay			= CGxUtility::SystemTimeToString( m_rRuntimeData.tmPrevDay );

	m_iniRuntime.Save();
}


// 저장되었던 실행 데이터를 불러온다.
void CMCMain::LoadRuntimeData()
{
	m_iniRuntime.Load();

	// 문자열 -> 날짜형으로 변형시켜준다.
	CGxUtility::StringToSystemTime( m_rRuntimeData.sUserLoginTime,	m_rRuntimeData.tmUserLogin );
	CGxUtility::StringToSystemTime( m_rRuntimeData.sEQStartTime,	m_rRuntimeData.tmEQStart );
	CGxUtility::StringToSystemTime( m_rRuntimeData.sEQStopTime,		m_rRuntimeData.tmEQStop );

}




// 비가동 정보를 파일에서 읽어온다.
BOOL CMCMain::LoadLosstimeItemList()
{
	CGxIniFile iniFile;
	CString sFilename;
	CString sGroup, sKey, sData, sLine, sTok;
	int i, j, k, n, n1stCount, n2ndCount, n3rdCount;
	int nLangIdx = CGxGUIFrame::GetInstance()->GetDispLanguage();			// 현재 표시되고 있는 언어 index

	sFilename.Format(_T("%s%s"), m_sPath_INI, NAME_FILE_LOSSTIME_LIST);

	// 파일이 존재하는지 점검
	if ( CGxUtility::IsExistFile( sFilename ) == FALSE ) 
	{
		CString sMsg;
		sMsg.Format(_T("* File not found : %s"), sFilename);
		
		CGxMsgBox	dlgMsgBox;
		dlgMsgBox.SetTitle(_T("알림"), _T("Notifications"), _T("thông báo"));
		dlgMsgBox.SetMessage(FALSE, sMsg, sMsg, sMsg, nLangIdx);
		dlgMsgBox.DoModal();


		//::AfxMessageBox( sMsg );																// 오류 내용을 보여준다.
		return FALSE;
	}

	// 파일이 존재하면 파일에서 데이터를 읽어온다.
	iniFile.SetIniFilename( sFilename );			// file이름을 지정

	// AUto Run-Down 항목을 읽어온다.
	m_rLosstimeList.sAutoRunDownCode	= iniFile.GetString(_T("Setting"), _T("AutoRunDown"),		_T("RD>15190>AUTO RUN DOWN>AUTO RUN DOWN>AUTO RUN DOWN"));		// AUTO RUN DOWN
	m_rLosstimeList.sOPLossCode		= iniFile.GetString(_T("Setting"), _T("OPLoss"),			_T("RD>0110>OP LOSS>OP LOSS>OP LOSS"));								// OP LOSS 선택 항목
	m_rLosstimeList.sTactTimeOver		= iniFile.GetString(_T("Setting"), _T("TactTimeOver"),	_T("RD>01200>TACT TIME OVER>TACT TIME OVER>TACT TIME OVER"));	// TATC TIME OVER 선택 항목


	// 전체 First Class 그룹 수량
	n1stCount = iniFile.GetInt( _T("GROUP"), _T("COUNT"),	0);

	// 1st class 최대 수량을 제한한다.
	if ( n1stCount > TLosstimeList::eMAX_1stCount ) n1stCount = TLosstimeList::eMAX_1stCount;

	m_rLosstimeList.nCount_1st	= n1stCount;				// 1st class 수

	for (i = 0; i < n1stCount; i++)
	{
		sGroup.Format( _T("GROUP_%02d"), i );			// 그룹이름, 00 ~ 07

		m_rLosstimeList.rLosstime[i].sName = iniFile.GetString( sGroup, _T("NAME"), _T("") );		// 1st class 이름
		n2ndCount = iniFile.GetInt( sGroup, _T("COUNT"),	0);												// 해당 1st Class의 2nd class 수

		if ( n2ndCount > TLoss1stClass::eMAX_2ndCount ) n2ndCount = TLoss1stClass::eMAX_2ndCount;	// 2nd class수 제한

		m_rLosstimeList.rLosstime[i].n2ndCount = n2ndCount;			// 해당 1stClass내 존재하는 2nd class 갯수

		// 2nd class 해당되는  항목을 읽어온다.
		for (j = 0; j < n2ndCount; j++ )
		{
			sGroup.Format( _T("GROUP_%02d"), i );			// 그룹이름, 00 ~ 07
			sKey.Format(_T("SUB_%02d"), j);				// Key
			m_rLosstimeList.rLosstime[i].r2ndClass[j].sLine	= iniFile.GetString( sGroup, sKey, _T("") );		// 2st class 이름
			
			// 각 언어별로 2개의 2nd class Name이 존재한다.
			sLine = m_rLosstimeList.rLosstime[i].r2ndClass[j].sLine;
			for (n = 0; n < eMAX_Language; n++)
			{
				// 각 언어별 2nd Class 이름을 '>' 기호로 분리하지 못한 경우에는 바로 끝낸다.
				if ( AfxExtractSubString( m_rLosstimeList.rLosstime[i].r2ndClass[j].sName[n], sLine, n, DELIMITER_DEFECT) == FALSE )	break;
			}//of for n				

			// 3rd Class data 읽기
			sGroup.Format(_T("GROUP_%02d_SUB_%02d"), i, j);				// 3rd Class를 읽기위한 Group 이름 지정
			n3rdCount = iniFile.GetInt( sGroup, _T("COUNT"), 0 );		// 3rd class 항목 수

			if ( n3rdCount > TLoss2ndClass::eMAX_3rdCount ) n3rdCount = TLoss2ndClass::eMAX_3rdCount;	// 3rd class수 제한

			m_rLosstimeList.rLosstime[i].r2ndClass[j].n3rdCount = n3rdCount;			// 해당 2ndClass내 존재하는 3rd class 갯수
			
			// 최종적인 3rd Class 항목을 읽어온다.
			for (k = 0; k < n3rdCount; k++)
			{
				sKey.Format(_T("ITEM_%02d"), k);		// Key
				m_rLosstimeList.rLosstime[i].r2ndClass[j].r3rdClass[k].sLine	= iniFile.GetString( sGroup, sKey, _T("") );		// 3rd class Data

				sLine = m_rLosstimeList.rLosstime[i].r2ndClass[j].r3rdClass[k].sLine;

				//예: ITEM_00   = PM>000>Shift점검>프로그램 점검>Check Program

				// Code 발췌 (현재는 미사용)
				AfxExtractSubString( m_rLosstimeList.rLosstime[i].r2ndClass[j].r3rdClass[k].sCode, sLine, 1, DELIMITER_DEFECT);	// 첫번째 항목인 Defect Code를 읽어낸다.

				for (n = 0; n < eMAX_Language; n++)
				{
					// 각 언어별 3rd Class 이름을 '>' 기호로 분리하지 못한 경우에는 바로 끝낸다.
					if ( AfxExtractSubString(m_rLosstimeList.rLosstime[i].r2ndClass[j].r3rdClass[k].sReason[n], sLine, (3 + n), DELIMITER_DEFECT) == FALSE )	break;
				}//of for n				
			}//of for k
		}//of for j
	}//of for i
	
	return TRUE;
}



// 2014-10-08 JH-PARK 
// Alarm 조치내용항목 파일에서 읽어온다.
BOOL CMCMain::LoadRepairAlarmItemList()
{
	CGxIniFile iniFile;
	CString sFilename;
	CString sGroup, sKey, sData, sLine, sTok;
	int i, j, k, n, n1stCount, n2ndCount, n3rdCount;
	int nLangIdx = CGxGUIFrame::GetInstance()->GetDispLanguage();			// 현재 표시되고 있는 언어 index

	sFilename.Format(_T("%s\\INI\\%s"), m_sBasePath, NAME_FILE_REPAIR_ALARM);

	// 파일이 존재하는지 점검
	if ( CGxUtility::IsExistFile( sFilename ) == FALSE ) 
	{
		CString sMsg;
		sMsg.Format(_T("* File not found : %s"), sFilename);
		
		CGxMsgBox	dlgMsgBox;
		dlgMsgBox.SetTitle(_T("알림"), _T("Notifications"), _T("thông báo"));
		dlgMsgBox.SetMessage(FALSE, sMsg, sMsg, sMsg, nLangIdx);
		dlgMsgBox.DoModal();

	}

	// 파일이 존재하면 파일에서 데이터를 읽어온다.
	iniFile.SetIniFilename( sFilename );			// file이름을 지정

	// 전체 First Class 그룹 수량
	n1stCount = iniFile.GetInt( _T("GROUP"), _T("COUNT"),	0);

	// 1st class 최대 수량을 제한한다.
	if ( n1stCount > TRepairAlarmList::eMAX_1stCount ) n1stCount = TRepairAlarmList::eMAX_1stCount;

	m_rRepairAlarmList.nCount_1st	= n1stCount;				// 1st class 수

	for (i = 0; i < n1stCount; i++)
	{
		sGroup.Format( _T("GROUP_%02d"), i );			// 그룹이름, 00 ~ 07

		m_rRepairAlarmList.rRepairAlarm[i].sName = iniFile.GetString( sGroup, _T("NAME"), _T("") );		// 1st class 이름
		n2ndCount = iniFile.GetInt( sGroup, _T("COUNT"),	0);												// 해당 1st Class의 2nd class 수

		if ( n2ndCount > TRepair1stClass::eMAX_2ndCount ) n2ndCount = TRepair1stClass::eMAX_2ndCount;	// 2nd class수 제한

		m_rRepairAlarmList.rRepairAlarm[i].n2ndCount = n2ndCount;			// 해당 1stClass내 존재하는 2nd class 갯수

		// 2nd class 해당되는  항목을 읽어온다.
		for (j = 0; j < n2ndCount; j++ )
		{
			sGroup.Format( _T("GROUP_%02d"), i );			// 그룹이름, 00 ~ 07
			sKey.Format(_T("SUB_%02d"), j);				// Key
			m_rRepairAlarmList.rRepairAlarm[i].r2ndClass[j].sLine	= iniFile.GetString( sGroup, sKey, _T("") );		// 2st class 이름

			// 각 언어별로 2개의 2nd class Name이 존재한다.
			sLine = m_rRepairAlarmList.rRepairAlarm[i].r2ndClass[j].sLine;
			for (n = 0; n < eMAX_Language; n++)
			{
				// 각 언어별 2nd Class 이름을 '>' 기호로 분리하지 못한 경우에는 바로 끝낸다.
				if ( AfxExtractSubString( m_rRepairAlarmList.rRepairAlarm[i].r2ndClass[j].sName[n], sLine, n, DELIMITER_DEFECT) == FALSE )	break;
			}//of for n				

			// 3rd Class data 읽기
			sGroup.Format(_T("GROUP_%02d_SUB_%02d"), i, j);				// 3rd Class를 읽기위한 Group 이름 지정
			n3rdCount = iniFile.GetInt( sGroup, _T("COUNT"), 0 );		// 3rd class 항목 수

			if ( n3rdCount > TRepair2ndClass::eMAX_3rdCount ) n3rdCount = TRepair2ndClass::eMAX_3rdCount;	// 3rd class수 제한

			m_rRepairAlarmList.rRepairAlarm[i].r2ndClass[j].n3rdCount = n3rdCount;			// 해당 2ndClass내 존재하는 3rd class 갯수

			// 최종적인 3rd Class 항목을 읽어온다.
			for (k = 0; k < n3rdCount; k++)
			{
				sKey.Format(_T("ITEM_%02d"), k);		// Key
				m_rRepairAlarmList.rRepairAlarm[i].r2ndClass[j].r3rdClass[k].sLine	= iniFile.GetString( sGroup, sKey, _T("") );		// 3rd class Data

				sLine = m_rRepairAlarmList.rRepairAlarm[i].r2ndClass[j].r3rdClass[k].sLine;

				//예: ITEM_00   = PM>000>Shift점검>프로그램 점검>Check Program

				// Code 발췌 (현재는 미사용)
				AfxExtractSubString( m_rRepairAlarmList.rRepairAlarm[i].r2ndClass[j].r3rdClass[k].sCode, sLine, 1, DELIMITER_DEFECT);	// 첫번째 항목인 Defect Code를 읽어낸다.

				for (n = 0; n < eMAX_Language; n++)
				{
					// 각 언어별 3rd Class 이름을 '>' 기호로 분리하지 못한 경우에는 바로 끝낸다.
					if ( AfxExtractSubString(m_rRepairAlarmList.rRepairAlarm[i].r2ndClass[j].r3rdClass[k].sReason[n], sLine, (3 + n), DELIMITER_DEFECT) == FALSE )	break;
				}//of for n				
			}//of for k
		}//of for j
	}//of for i

	return TRUE;
}



//
// 예외처리할 각종 설정값들을 읽어온다. 2015-07-14, jhLee,
// 1. 사고LOT 처리 Cell 설정값
//
void CMCMain::LoadExceptConfigFile()
{
	CGxIniFile iniFile;
	CString sFilename, sSection, sKey;

	sFilename.Format(_T("%s\\INI\\%s"), GetBasePath(), NMAE_FILE_EXCEPT_CONFIG);

	// 파일이 존재하는지 점검
	if ( CGxUtility::IsExistFile( sFilename ) == FALSE )		// 파일이 존재하지 않는다면
	{
		CGxLog* pLog = GetLogPointer( eLOG_SYS );
		pLog->Write(_T("* Error, Except process config file not found : %s"), sFilename);
	}


	// 파일이 존재하면 파일에서 데이터를 읽어온다.
	iniFile.SetIniFilename( sFilename );			// file이름을 지정

	//1. 사고LOT 처리
	sSection = _T("TroubleLOT");
	m_rProfileData.sTroubleLOT_Value	= iniFile.GetString( sSection, _T("ResultValue"), _T("41"));		// MES에서 넘겨주는 Code값, 41
	m_rProfileData.sTroubleLOT_Code		= iniFile.GetString( sSection, _T("DefectCode"), _T("USN03"));		// 불량 코드, USN03
	m_rProfileData.sTroubleLOT_Name		= iniFile.GetString( sSection, _T("DefectName"), _T("FAB HOLD"));	// 불량명, FAB HOLD


	//2. POCH 예외처리 
	sSection = _T("POCH_LOT");
	m_rProfileData.sExceptCell_POCH_Value	= iniFile.GetString( sSection, _T("ResultValue"), _T("POCH"));		// POCH 불량 값 : POCH
	m_rProfileData.sExceptCell_POCH_Name	= iniFile.GetString( sSection, _T("DefectCode"), _T("USF09"));		// POCH 불량명, POCH
	m_rProfileData.sExceptCell_POCH_Code	= iniFile.GetString( sSection, _T("DefectName"), _T("POCH"));		// POCH 불량 코드, USF09

}



// 변경된 Shortcut list를 파일로 저장한다.
BOOL CMCMain::SaveShortcutList()
{
	CGxIniFile iniFile;
	CString sFilename;
	CString sGroup, sKey;
	int i;

	sFilename.Format(_T("%s\\INI\\%s"), m_sBasePath, NAME_FILE_DEFECT_LIST);

	iniFile.SetIniFilename( sFilename );			// file이름을 지정

	sGroup = _T("SHORTCUT");
	iniFile.WriteInt( sGroup, _T("SHORTCUT_COUNT"),	m_rDefectList.nShortcutCount);			// 단축 버튼 수량 기록

	for (i = 0; i < TDefectList::eMAX_ShortcutCount; i++ )
	{
		sKey.Format( _T("SHORTCUT_%02d"), i );					// Shortcut 항목, 00 ~ 09

		if ( i <	(int)m_rDefectList.nShortcutCount )				// 설정된 단축버튼 수량인 경우
		{
			iniFile.WriteString( sGroup, sKey,	m_rDefectList.rShortcut[i].sLine);				// 단축 버튼 정보 기록
		}
		else
			iniFile.WriteString( sGroup, sKey,	_T("") );		// 공백 기록

	}//of for i

	return TRUE;
}


// Defcet Shortcut 추가
int CMCMain::SetDefectShortcutAppend( TDefectItem* pDefect )
{
	int i;
	int nNewIdx;

	if ( pDefect == NULL ) return 0;


	// 중복되는지 조사한다.
	for (i = 0; i < (int)m_rDefectList.nShortcutCount; i++)
	{
		// 동일한 defect이 이미 등록되어있는가 ?
		if ( m_rDefectList.rShortcut[i].sCode == pDefect->sCode ) return (i + 1);				// 발견된 순번을 되돌린다.
	}

	// 등록되어있지 않다면, 추가를 한다.
	// 이미 할당된 수량을 넘어섰다면, 첫번째부터 밀어낸다.
	if ( m_rDefectList.nShortcutCount >= TDefectList::eMAX_ShortcutCount )
	{
		for (i = 0; i < (TDefectList::eMAX_ShortcutCount - 1); i++)
		{
			m_rDefectList.rShortcut[i] = m_rDefectList.rShortcut[i + 1];		// 한 칸씩 앞으로 당긴다.
		}//of for i

		nNewIdx = (TDefectList::eMAX_ShortcutCount - 1);			// 가장 마지막에 추가하도록 한다. 전체 수량에는 변화가 없다.
	}
	else
		{
			nNewIdx = (int)m_rDefectList.nShortcutCount;				// 마지막에 추가를 한다.
			m_rDefectList.nShortcutCount++;								// 수량 증가
		}		


	m_rDefectList.rShortcut[ nNewIdx ] = *pDefect;		// 새로이 추가한다. 

	SaveShortcutList();		// File로 저장한다.

	return nNewIdx;			// 추가된 Index를 넘긴다.
}



//2014-12-22, jhLee 제품 투입시 특정 Cell ID는 투입시키지 않고 검사 제외를 시켜 다시 제거토록 한다.
// CMapStringToString m_mapExceptCellID;				// 제외시킬 Cell ID를 저장하고 검색할 Map

// 제외시킬 Cell ID를 파일에서 읽어온다.
int CMCMain::LoadExeptCellID()
{
	CString sFilename;

	// 일단 고정값
	// sFilename = _T("D:\\UB-AMT\\INI\\CellIDError.ini");
	sFilename.Format(_T("%s\\INI\\%s"), GetBasePath(), NAME_FILE_EXCEPT_CELL);		// 제외시킬 Cell ID List file

	m_mapExceptCellID.RemoveAll();

	// 파일이 존재하는지 점검
	if ( PathFileExists( sFilename ) == FALSE )		//  파일 존재여부
	{
		CGxLog* pLog = GetLogPointer( eLOG_SYS );

		pLog->Write(_T("* Error, Except Cell-ID List File not found \n : %s"), sFilename);
		return 0;
	}


	//파일을 한줄씩 읽어가면서 넣어준다.;
	// Text 파일 UniCode 방식으로 설정하지 않는다, Cell ID는 영문으로 되어있다.
	FILE *fStream;
	CString sLine;

	errno_t e = _tfopen_s(&fStream, sFilename.GetString(), _T("r"));	// ,ccs=UNICODE"));
	if (e != 0) return 0;

	CStdioFile		file(fStream);

	while( file.ReadString( sLine ) )
	{
		if ( (sLine.Left(1) == _T("!")) || (sLine.Left(1) == _T("#")) || (sLine.Left(1) == _T(";") ))	continue;	// 처음 글자가 " ! "  혹은 " # " 일경우 다음 문장으로 넘어간다.;

		m_mapExceptCellID.SetAt(sLine, sLine);		// Map에 추가한다.
	}

	file.Close();
	fclose(fStream);

	return m_mapExceptCellID.GetCount();			// 읽어들인 수량을 되돌려준다.
}



// 지정한 Cell ID가 검사를 제외 할 대상인가 ?
BOOL CMCMain::GetIsExceptCellID(CString sCellID)
{
	CString sValue;

	// 등록된 정보가 있으면 검색을 한다.
	if ( m_mapExceptCellID.GetCount() > 0 )
	{
		// 지정한 Key (Defect name)이 존재한다면 TRUE를 Return
		if ( m_mapExceptCellID.Lookup(sCellID, sValue) == TRUE )
		{
			return TRUE;
		}
	}

	return FALSE;		// 속하지 않는다면 FALSE
}

// 2015-06-21 JH-PARK 판정 Cell에 대한 Data Mapping 진행

int CMCMain::LoadJudgeCellID()
{
	CString sFilename;

	// sFilename.Format(_T("D:\\UB-AMT\\Data\\%s"), NAME_FILE_JUDGE_CELL_ID);	// 해당 ini파일 경로 지정
	sFilename.Format(_T("%s\\INI\\%s"), GetBasePath(), NAME_FILE_JUDGE_CELL_ID);		// 제외시킬 Cell ID List file

	m_mapJudgeCellID.RemoveAll();

	// 파일이 존재하는지 점검
	if ( PathFileExists( sFilename ) == FALSE )		//  파일 존재여부
	{
		CGxLog* pLog = GetLogPointer( eLOG_SYS );

		pLog->Write(_T("* Error, Judge Cell-ID List File not found \n : %s"), sFilename);
		return 0;
	}


	// ini파일 Data 가져와서 Mapping 진행
	CGxIniFile iniFile;
	iniFile.SetIniFilename( sFilename );			// file이름을 지정

	CString sSection = _T("JUDGE_CELL_ID");

	CString sName;
	CString sID;

	for(int i=0; i<4; i++)
	{
		sName.Format(_T("sID_%d"),i+1);	

		sID = iniFile.GetString( sSection, sName,	_T("")); // ID 검색 

		if(sID == _T("")) // ID가 공백일경우 다음으로 넘어간다
			continue;
		
		m_mapJudgeCellID.SetAt(sID, sID);		// Map에 추가한다.
	}
	return m_mapJudgeCellID.GetCount();			// 읽어들인 수량을 되돌려준다.
}



// 지정한 Cell ID가 검사를 제외 할 대상인가 ?
BOOL CMCMain::GetIsJudgeCellID(CString sCellID)
{
	if(sCellID == _T(""))
		return FALSE;

	CString sValue;

	// 등록된 정보가 있으면 검색을 한다.
	if ( m_mapJudgeCellID.GetCount() > 0 )
	{
		// 지정한 Key (Defect name)이 존재한다면 TRUE를 Return
		if ( m_mapJudgeCellID.Lookup(sCellID, sValue) == TRUE )
		{
			return TRUE;
		}
	}
	return FALSE;		// 속하지 않는다면 FALSE
}


// 종료시 계정 정보를 메모리 해제해 준다.
void CMCMain::ReleaseJudgeCellID()
{
	m_mapUsers.RemoveAll();										// 등록되어있는 모든 키를 삭제해 준다.
}







// Defect List 데이터를 파일에서 읽어들인다.
BOOL CMCMain::LoadDefectList()
{
	CGxIniFile iniFile;
	CString sFilename;
	CString sGroup, sKey, sData, sLine, sTok;
	int i, j, k, nCount, nDefectCount;
	// int nLangIdx = CGxGUIFrame::GetInstance()->GetDispLanguage();			// 현재 표시되고 있는 언어 index

	// Rigid용 Defect list인지, Youm인지 구분하여 파일을 읽어온다.
	sFilename.Format(_T("%s%s_%s.ini"), m_sPath_INI, NAME_FILE_DEFECT_LIST, IsCellRigid() ? TEXT_NameRigid : TEXT_NameYoum);

	// 파일이 존재하는지 점검
	if ( CGxUtility::IsExistFile( sFilename ) == FALSE ) 
	{
		CString sMsg;
		sMsg.Format(_T("* File not found : %s"), sFilename);
		

		CGxMsgBox dlgMsg;
		dlgMsg.SetTitle(_T("알림"), _T("Notifications"), _T("thông báo"));
		dlgMsg.SetMessage(FALSE, sMsg, sMsg, sMsg, 0);
		dlgMsg.DoModal();
	}

	// 이전의 내용을 지워준다.
	for (i = 0; i < eMAX_DefaultDefectCount; i++) 
	{
		m_rDefectList.rDefault[i].Clear();
	}

	// 파일이 존재하면 파일에서 데이터를 읽어온다.
	iniFile.SetIniFilename( sFilename );			// file이름을 지정
	nCount = iniFile.GetInt( _T("GROUP"), _T("GROUP_COUNT"),	0);

	// Group 최대 수량을 제한한다.
	if ( nCount > TDefectList::eMAX_GoupCount ) nCount = TDefectList::eMAX_GoupCount;

	m_rDefectList.nGroupCount	= nCount;				// 그룹 수

	for (i = 0; i < nCount; i++)
	{
		sGroup.Format( _T("GROUP_%02d"), i );			// 그룹이름, 00 ~ 11

		//old m_rDefectList.sGroupName[i]	= iniFile.GetString( sGroup, _T("GROUP_NAME"), _T("") );			// 그룹 이름
		sLine = iniFile.GetString( sGroup, _T("GROUP_NAME"), _T("") );										// 그룹 이름
		// 각 언어별로 3개의 Group Name이 존재한다.
		for (k = 0; k < eMAX_Language; k++)
		{
			// 각 언어별 Defect 명칭을 읽어온다. '>' 기호로 분리하지 못한 경우에는 바로 끝낸다.
			if ( AfxExtractSubString(m_rDefectList.sGroupName[i][k], sLine, k, DELIMITER_DEFECT) == FALSE )	break;
		}// for k

		nDefectCount = iniFile.GetInt( sGroup, _T("DEFECT_COUNT"),	0);										// 해당 그룹의 Defect 수

		if ( nDefectCount > TDefectList::eMAX_DefectCount ) nDefectCount = TDefectList::eMAX_DefectCount;	// Defect수 제한

		m_rDefectList.nDefectCount[i] = nDefectCount;			// 그룹내 defect 갯수

		// 그룹에 해당되는 Defect 항목을 읽어온다.
		for (j = 0; j < nDefectCount; j++ )
		{
			sKey.Format(_T("DEF_%02d"), j);				// Key
			m_rDefectList.rDefectData[i][j].sLine	= iniFile.GetString( sGroup, sKey, _T("") );				// Defect 이름 정보를 가져온다.

			// DEF_05=USA06>C-Panel들뜸>C-Panel Come Off>C-Panel翘起
			sLine = m_rDefectList.rDefectData[i][j].sLine;

			if ( sLine.IsEmpty() == FALSE )			// 내용이 있는 경우에 실행
			{
				// Code를 가져온다.
				AfxExtractSubString(m_rDefectList.rDefectData[i][j].sCode, sLine, 0, DELIMITER_DEFECT);	// 첫번째 항목인 Defect Code를 읽어낸다.

				// 각 언어별로 3개의 Defect name이 존재한다.
				for (k = 0; k < eMAX_Language; k++)
				{
					// 각 언어별 Defect 명칭을 읽어온다. '>' 기호로 분리하지 못한 경우에는 바로 끝낸다.
					if ( AfxExtractSubString(m_rDefectList.rDefectData[i][j].sDefect[k], sLine, (k + 1), DELIMITER_DEFECT) == FALSE )	break;
				}// for k
			}//of if not emtpy
		}//of for j
	}//of for i


	// 단축 defect을 읽어온다.
	sGroup = _T("SHORTCUT");
	nCount = iniFile.GetInt( sGroup, _T("SHORTCUT_COUNT"),	0);

	// Shortcut 최대 수량을 제한한다.eMAX_ShortcutCount	= 10,			// 단축 defect 수량, 8개
	if ( nCount > TDefectList::eMAX_ShortcutCount ) nCount = TDefectList::eMAX_ShortcutCount;
	m_rDefectList.nShortcutCount = nCount;

	for (i = 0; i < nCount; i++)
	{
		sKey.Format( _T("SHORTCUT_%02d"), i );			// Shortcut 항목, 00 ~ 07

		m_rDefectList.rShortcut[i].sLine	= iniFile.GetString( sGroup, sKey, _T("") );				// Shortcut Defect 이름 정보를 가져온다.

		// SHORTCUT_05=USA06>C-Panel들뜸>C-Panel Come Off>C-Panel翘起
		sLine = m_rDefectList.rShortcut[i].sLine;

		if ( sLine.IsEmpty() == FALSE )			// 내용이 있는 경우에 실행
		{
			// Code를 가져온다.
			AfxExtractSubString(m_rDefectList.rShortcut[i].sCode, sLine, 0, DELIMITER_DEFECT);	// 첫번째 항목인 Defect Code를 읽어낸다.

			// 각 언어별로 3개의 Defect name이 존재한다.
			for (j = 0; j < eMAX_Language; j++)
			{
				// 각 언어별 Defect 명칭을 읽어온다. '>' 기호로 분리하지 못한 경우에는 바로 끝낸다.
				if ( AfxExtractSubString(m_rDefectList.rShortcut[i].sDefect[j], sLine, (j + 1), DELIMITER_DEFECT) == FALSE )	break;
			}//of for j
		}//of if not empty
	}//of for i



	// Default Defect 
	sGroup = _T("DEFAULT_DEFECT");
	for (i = 0; i < eMAX_DefaultDefectCount; i++)
	{
		sKey.Format( _T("DEFAULT_%02d"), i );			// Default defect 항목, 1 ~ 4

		m_rDefectList.rDefault[i].sLine	= iniFile.GetString( sGroup, sKey, _T("") );				// Default Defect 정보를 가져온다.

		// DEFAULT_00=USA06>C-Panel들뜸>C-Panel Come Off>C-Panel翘起
		sLine = m_rDefectList.rDefault[i].sLine;

		if ( sLine.IsEmpty() == FALSE )			// 내용이 있는 경우에 실행
		{
			// Code를 가져온다.
			AfxExtractSubString(m_rDefectList.rDefault[i].sCode, sLine, 0, DELIMITER_DEFECT);		// 첫번째 항목인 Defect Code를 읽어낸다.

			// 각 언어별로 3개의 Defect name이 존재한다.
			for (j = 0; j < eMAX_Language; j++)
			{
				// 각 언어별 Defect 명칭을 읽어온다. '>' 기호로 분리하지 못한 경우에는 바로 끝낸다.
				if ( AfxExtractSubString(m_rDefectList.rDefault[i].sDefect[j], sLine, (j + 1), DELIMITER_DEFECT) == FALSE )	break;
			}//of for j
		}//of if not empty
	}//of for i


	return TRUE;
}

//
// 생산량 정보를 기록하고 관리하는 파일 초기화
void CMCMain::InitProductFile()
{
	int i;
	CString sKey;

	// 생산량 정보 기록 
	// profile ini file 설정 
	m_iniProduct.m_sIniFile.Format(_T("%s\\Data\\Runtime\\%s"), m_sBasePath, NAME_FILE_PRODUCT_DATA);			// data 파일 경로 지정

	m_iniProduct.AddUint(_T("Product:nTotalCount"),		m_rProductData.nTotalCount);						// 전체 생산량

	// 각 판정 기준별 분류
	for (i = 0; i < eMAX_JudgeMode; i++)
	{
		// 생산량
		sKey.Format(_T("Product:nGoodCount_%d"), i);
		m_iniProduct.AddUint(sKey,		m_rProductData.nGoodCount[i]);											// 양품 수량

		sKey.Format(_T("Product:nBin2Count_%d"), i);
		m_iniProduct.AddUint(sKey,		m_rProductData.nBin2Count[i]);											// BIN2, 재검 수량

		sKey.Format(_T("Product:nRejectCount_%d"), i);
		m_iniProduct.AddUint(sKey,		m_rProductData.nRejectCount[i]);											// 불량 수량

		// 비율
		sKey.Format(_T("Product:nGoodRatio_%d"), i);
		m_iniProduct.AddDouble(sKey,		m_rProductData.dbGoodRatio[i]);										// 양품 비율

		sKey.Format(_T("Product:nBin2Ratio_%d"), i);
		m_iniProduct.AddDouble(sKey,		m_rProductData.dbBin2Ratio[i]);										// Bin2 비율

		sKey.Format(_T("Product:nRejectRatio_%d"), i);
		m_iniProduct.AddDouble(sKey,		m_rProductData.dbRejectRatio[i]);									// 불량 비율

	}

	m_iniProduct.AddUint(_T("UVMCR:nMCRReadCount"),		m_rProductData.nMCRReadCount);					// UV-MCR Reading 횟수
	m_iniProduct.AddUint(_T("UVMCR:nMCRReadNG"),			m_rProductData.nMCRReadNG);						// UV-MCR 리딩 중 NG 횟수
	m_iniProduct.AddDouble(_T("UVMCR:dbMCRReadRatio"),	m_rProductData.dbMCRReadRatio);					// UV-MCR 리딩 성공률

	m_iniProduct.AddUint(_T("Align:nAlignCount"),		m_rProductData.nAlignCount);						// Align 읽기 횟수
	m_iniProduct.AddUint(_T("Align:nAlignNG"),			m_rProductData.nAlignNG);							// Align 실패 횟수
	m_iniProduct.AddDouble(_T("Align:dbAlignReadRatio"),m_rProductData.dbAlignReadRatio);				// Algin 읽기 성공률

	m_iniProduct.AddUint(_T("Time:nRunTime"),				m_rProductData.nRunTime);							// 가동 시간
	m_iniProduct.AddUint(_T("Time:nRunTimeSum"),			m_rProductData.nRunTimeSum);						// 지금까지의 총 가동시간 ?
	m_iniProduct.AddUint(_T("Time:nLossTime"),			m_rProductData.nLossTime);							// 비가동 시간
}


// 파일 읽어오기
void CMCMain::LoadProductFile()
{
	m_iniProduct.Load();


}




// 지정된 생산데이터를 저장한다.
void CMCMain::SaveProductFile(CString sGroup)
{
	m_iniProduct.Save( sGroup );
}

// 총 생산량 증가
void CMCMain::AddTotalProductCount()
{
	m_rProductData.nTotalCount++;					// 전체 생산량 증가

	//SaveTodayProductData(eIDX_Product_TodayRunningCell);

	// 총 생산량에 변화가 있다면,
	if ( m_rProductData.nTotalCount != 0 ) 
	{
		SaveTodayProductData(eIDX_Product_TodayRunningCell); // 시간별 진행수량 등록;
		for (int i = 0; i < eMAX_JudgeMode; i++)
		{
			// 다시 비율을 계산해 준다.
			m_rProductData.dbGoodRatio[i] = (m_rProductData.nGoodCount[i] / (m_rProductData.nTotalCount * 1.0) ) * 100;
			m_rProductData.dbBin2Ratio[i] = (m_rProductData.nBin2Count[i] / (m_rProductData.nTotalCount * 1.0) ) * 100;
			m_rProductData.dbRejectRatio[i] = (m_rProductData.nRejectCount[i] / (m_rProductData.nTotalCount * 1.0) ) * 100;
		}//of for i

	}
}

// 생산량 증가
void CMCMain::AddProductCount(int nIdx, int nJudge)
{
	if ( (nIdx < 0) || (nIdx >= eMAX_JudgeMode) ) return;				// Index 부적합

	switch ( nJudge )
	{
		case eJUDGE_Good :									// 양품
 			m_rProductData.nGoodCount[nIdx]++;			// 양품 수량

			if(nIdx == eIDX_JudgeFinal)				SaveTodayProductData(eDIX_Prodcut_TodayManGood);
			else	if(nIdx == eIDX_JudgeAuto)		SaveTodayProductData(eDIX_Prodcut_TodayAutoGood);

			// 지정 수치의 비율만 다시 계산한다.
			// 양품 비율
			if ( m_rProductData.nTotalCount != 0 ) m_rProductData.dbGoodRatio[nIdx] = (m_rProductData.nGoodCount[nIdx] / (m_rProductData.nTotalCount * 1.0) ) * 100;
			break;

		case eJUDGE_BIN2 :		// 재검
			m_rProductData.nBin2Count[nIdx]++;			// BIN2, 재검 수량

			if(nIdx == eIDX_JudgeFinal)				SaveTodayProductData(eDIX_Prodcut_TodayManBin2);
			else	if(nIdx == eIDX_JudgeAuto)		SaveTodayProductData(eDIX_Prodcut_TodayAutoBin2);

			if ( m_rProductData.nTotalCount != 0 ) m_rProductData.dbBin2Ratio[nIdx] = (m_rProductData.nBin2Count[nIdx] / (m_rProductData.nTotalCount * 1.0) ) * 100;
			break;

		case eJUDGE_Reject :									// 불량
			m_rProductData.nRejectCount[nIdx]++;		// 불량 수량


			if(nIdx == eIDX_JudgeFinal)				SaveTodayProductData(eDIX_Prodcut_TodayManReject);
			else	if(nIdx == eIDX_JudgeAuto)		SaveTodayProductData(eDIX_Prodcut_TodayAutoReject);
			
			if ( m_rProductData.nTotalCount != 0 ) m_rProductData.dbRejectRatio[nIdx] = (m_rProductData.nRejectCount[nIdx] / (m_rProductData.nTotalCount * 1.0) ) * 100;
			break;
	}
}


// UV-MCR 관련 누적
void CMCMain::AddUVMCRCount(int nJudge)
{
	if ( nJudge == eJUDGE_Good )
	{
		m_rProductData.nMCRReadCount++;							// UV-MCR Reading 횟수	
		SaveTodayProductData(eIDX_Product_TodayMcrRead);
	}
	else if ( nJudge == eJUDGE_NG )
		{
			m_rProductData.nMCRReadNG++;							// UV-MCR 리딩 중 NG 횟수
			m_rProductData.nMCRReadCount++;						// UV-MCR Reading 횟수	

			SaveTodayProductData(eIDX_Product_TodayMcrRead);
			SaveTodayProductData(eIDX_Product_TodayMcrNg);
		}

	// 0으로 나누는것 방지
	if ( m_rProductData.nMCRReadCount > 0 )
		m_rProductData.dbMCRReadRatio = (m_rProductData.nMCRReadCount - m_rProductData.nMCRReadNG) / ( m_rProductData.nMCRReadCount * 1.0) * 100;
	else
		m_rProductData.dbMCRReadRatio = 0.0;
}

// Align 관련 누적
void CMCMain::AddAlignCount(int nJudge)
{
	if ( nJudge == eJUDGE_Good )
	{
		m_rProductData.nAlignCount++;							// UV-MCR Reading 횟수	
		SaveTodayProductData(eIDX_Product_TodayAlignRead);
	}
	else if ( nJudge == eJUDGE_NG )
		{
			m_rProductData.nAlignNG++;							// UV-MCR 리딩 중 NG 횟수
			m_rProductData.nAlignCount++;						// UV-MCR Reading 횟수	

			SaveTodayProductData(eIDX_Product_TodayAlignRead);
			SaveTodayProductData(eIDX_Product_TodayAlignNg);
		}

	// 0으로 나누는것 방지
	if ( m_rProductData.nAlignCount > 0 )
		m_rProductData.dbAlignReadRatio = (m_rProductData.nAlignCount - m_rProductData.nAlignNG)  / ( m_rProductData.nAlignCount * 1.0) * 100;
	else
		m_rProductData.dbAlignReadRatio = 0.0;
}


// 설비 가동에 대한 누적
// BOOL bRunFlag : TRUE : Auto-Run 상태, FALSE : 정지 상태
void CMCMain::AddRuntimeCount(BOOL bRunFlag)
{
	if ( bRunFlag == TRUE )									// 가동시각
	{
		m_rProductData.nRunTime++;							// 가동 시간
		m_rProductData.nRunTimeSum++;						// 지금까지의 총 가동시간 ?

		SaveTodayProductData(eIDX_Product_TodayRunTime);	// 가동시간 시간별 누적;
	}
	else
		m_rProductData.nLossTime++;							// 비가동 시간
}


//eIDX_PRODUCT_Product,				// 생산량
//eIDX_PRODUCT_UVMCR,				// UV-MCR 읽기
//eIDX_PRODUCT_Align,				// Align 수행
//eIDX_PRODUCT_Time,					// 시간데이터

// 생산량 저장 및 화면 갱신
void CMCMain::SaveProductCount()
{
	m_iniProduct.Save(_T("Product"));

	// AutoForm 화면에 변경된 내용을 표시한다.
	::PostMessage(m_hOwner, UDMSG_DISPDATA_UPDATE, eIDX_UPDATE_PRODUCT, eIDX_PRODUCT_Product);				// 생산량
}

// UV-MCR 관련 누적 저장
void CMCMain::SaveUVMCRCount()
{
	m_iniProduct.Save(_T("UVMCR"));
	::PostMessage(m_hOwner, UDMSG_DISPDATA_UPDATE, eIDX_UPDATE_PRODUCT, eIDX_PRODUCT_UVMCR);					// UV-MCR 읽기
}

// Align 관련 누적 저장
void CMCMain::SaveAlignCount()
{
	m_iniProduct.Save(_T("Align"));
	::PostMessage(m_hOwner, UDMSG_DISPDATA_UPDATE, eIDX_UPDATE_PRODUCT, eIDX_PRODUCT_Align);					// Align 수행
}

// 설비 가동에 대한 누적 저장
// 본 함수는 거의 1초마다 호출될것인데, 그 때마다 저장하면 부하가 발생할듯하여 
// 10초 주기마다 저장하도록 하였다.
void CMCMain::SaveRuntimeCount(BOOL bForce)
{
	// 시간관련 데이터 저장 주기 Count
	m_rProductData.nTimeSaveCnt++;

	if ( (bForce == TRUE) || (m_rProductData.nTimeSaveCnt >= 10) )		// 10초에 한 번씩 저장하거나 강제로 저장ㅎ나다.
	{
		m_iniProduct.Save(_T("Time"));
		m_rProductData.nTimeSaveCnt = 0;
	}

	::PostMessage(m_hOwner, UDMSG_DISPDATA_UPDATE, eIDX_UPDATE_PRODUCT, eIDX_PRODUCT_Time);					// 시간데이터
}

//
//
//void CMCMain::LoadTodayProductData()
//{
//	CTime time = CTime::GetCurrentTime();
//
//	// 하루 나누는 시간으로 날짜를 갱신한다.
//	// 기준시간이 06시 일경우 
//	// 15일 05시까지는 time이 14일자로 고정된다.
//	// 15일 05시부터 tiem이 15일로 변경된다.
//
//	//CEtc::ApplyTimeOffset(time, nTimeOffet, m_rProfileData.nCellLog_Min);		// By 오시창K
//	time = GetToday();																			// By 이재현C
//
//	CString strFolderPath;
//	strFolderPath.Format(_T("D:\\UB-AMT\\Log\\%04d%02d\\Product"),time.GetYear(),time.GetMonth());	// 폴더경로;
//
//	if(CFileSupport::DirectoryCheck(strFolderPath) != TRUE)
//	{
//		// 초기값 설정; (초기값 설정이 안들어가서 쓰레기값이 표시됨)
//		m_rProductData.ClearTodayProductData(); 
//
//		return;
//	}
//
//	CString strPath;
//	strPath.Format(_T("%s\\%s_%04d%02d%02d.ini"), strFolderPath, _T("ProductDB"), time.GetYear(), time.GetMonth(), time.GetDay());
//
//	CString strSection = _T("");
//	CGxIniFile ini;
//
//	ini.SetIniFilename(strPath);	// ini지정;
//
//	for (int i = 0; i < 24; i++)
//	{
//		strSection.Format(_T("%02d"), i);
//		m_rProductData.nTodayRunningCell[i]			= ini.GetInt(strSection, _T("TOTAL"),0);
//		m_rProductData.nTodayAutoGoodCount[i]		= ini.GetInt(strSection, _T("AUTO_GOOD"),0);
//		m_rProductData.nTodayAutoBin2Count[i]		= ini.GetInt(strSection, _T("AUTO_BIN2"),0);
//		m_rProductData.nTodayAutoRejectCount[i]		= ini.GetInt(strSection, _T("AUTO_REJECT"),0);
//		m_rProductData.nTodayManGoodCount[i]		= ini.GetInt(strSection, _T("MANUAL_GOOD"),0);
//		m_rProductData.nTodayManBin2Count[i]		= ini.GetInt(strSection, _T("MANUAL_BIN2"),0);
//		m_rProductData.nTodayManRejectCount[i]		= ini.GetInt(strSection, _T("MANUAL_REJECT"),0);
//
//		m_rProductData.nTodayRunTime[i]				= ini.GetInt(strSection, _T("RUN_TIME"),0);
//		m_rProductData.nTodayOPLoss[i]				= ini.GetInt(strSection, _T("OP_LOSS"),0);
//		m_rProductData.nTodayContact[i]				= ini.GetInt(strSection, _T("CONTACT"),0);
//
//		m_rProductData.dTodayMcrReadCount[i]		= ini.GetDouble(strSection, _T("MCR_REAIDNG"),0);
//		m_rProductData.dTodayMcrNgCount[i]			= ini.GetDouble(strSection, _T("MCR_NG"),0);
//		m_rProductData.dTodayAlignReadCount[i]		= ini.GetDouble(strSection, _T("ALIGN_READING"),0);
//		m_rProductData.dTodayAlignNgCount[i]		= ini.GetDouble(strSection, _T("ALIGN_NG"),0);
//
//		m_rProductData.nCH1_GoodCount[i]			= ini.GetInt(strSection, _T("CH1_GOOD"),0);
//		m_rProductData.nCH2_GoodCount[i]			= ini.GetInt(strSection, _T("CH2_GOOD"),0);
//		m_rProductData.nCH3_GoodCount[i]			= ini.GetInt(strSection, _T("CH3_GOOD"),0);
//		m_rProductData.nCH4_GoodCount[i]			= ini.GetInt(strSection, _T("CH4_GOOD"),0);
//		m_rProductData.nCH1_NgCount[i]				= ini.GetInt(strSection, _T("CH1_NG"),0);
//		m_rProductData.nCH2_NgCount[i]				= ini.GetInt(strSection, _T("CH2_NG"),0);
//		m_rProductData.nCH3_NgCount[i]				= ini.GetInt(strSection, _T("CH3_NG"),0);
//		m_rProductData.nCH4_NgCount[i]				= ini.GetInt(strSection, _T("CH4_NG"),0);
//		m_rProductData.nNoDisplay[i]				= ini.GetInt(strSection, _T("NO_DISP"),0);
//
//	}
//}


void CMCMain::LoadTodayProductData()
{
	CTime time = CTime::GetCurrentTime();

	// 하루 나누는 시간으로 날짜를 갱신한다.
	// 기준시간이 06시 일경우 
	// 15일 05시까지는 time이 14일자로 고정된다.
	// 15일 05시부터 tiem이 15일로 변경된다.

	//CEtc::ApplyTimeOffset(time, nTimeOffet, m_rProfileData.nCellLog_Min);		// By 오시창K
	time = GetToday();																			// By 이재현C

	CString strFolderPath = m_logProductData.GetFilePath( time );
	// strFolderPath.Format(_T("%s%04d%02d\\Product"), m_sPath_Log, time.GetYear(),time.GetMonth());	// 폴더경로;

	if( CGxUtility::IsExistFile( strFolderPath ) != TRUE)			// 파일이 존하지 않는다면,
	{
		// 초기값 설정; (초기값 설정이 안들어가서 쓰레기값이 표시됨)
		m_rProductData.ClearTodayProductData(); 

		return;
	}

	//CString strPath;
	//strPath.Format(_T("%s\\%s_%04d%02d%02d.ini"), strFolderPath, _T("ProductDB"), time.GetYear(), time.GetMonth(), time.GetDay());

	CString strSection = _T("");
	CGxIniFile ini;

	ini.SetIniFilename( strFolderPath );	// ini지정;

	for (int i = 0; i < 24; i++)
	{
		strSection.Format(_T("%02d"), i);
		m_rProductData.nTodayRunningCell[i]			= ini.GetInt(strSection, _T("TOTAL"),0);
		m_rProductData.nTodayAutoGoodCount[i]		= ini.GetInt(strSection, _T("AUTO_GOOD"),0);
		m_rProductData.nTodayAutoBin2Count[i]		= ini.GetInt(strSection, _T("AUTO_BIN2"),0);
		m_rProductData.nTodayAutoRejectCount[i]		= ini.GetInt(strSection, _T("AUTO_REJECT"),0);
		m_rProductData.nTodayManGoodCount[i]		= ini.GetInt(strSection, _T("MANUAL_GOOD"),0);
		m_rProductData.nTodayManBin2Count[i]		= ini.GetInt(strSection, _T("MANUAL_BIN2"),0);
		m_rProductData.nTodayManRejectCount[i]		= ini.GetInt(strSection, _T("MANUAL_REJECT"),0);

		m_rProductData.nTodayRunTime[i]				= ini.GetInt(strSection, _T("RUN_TIME"),0);
		m_rProductData.nTodayOPLoss[i]				= ini.GetInt(strSection, _T("OP_LOSS"),0);
		m_rProductData.nTodayContact[i]				= ini.GetInt(strSection, _T("CONTACT"),0);

		m_rProductData.dTodayMcrReadCount[i]		= ini.GetDouble(strSection, _T("MCR_REAIDNG"),0);
		m_rProductData.dTodayMcrNgCount[i]			= ini.GetDouble(strSection, _T("MCR_NG"),0);
		m_rProductData.dTodayAlignReadCount[i]		= ini.GetDouble(strSection, _T("ALIGN_READING"),0);
		m_rProductData.dTodayAlignNgCount[i]		= ini.GetDouble(strSection, _T("ALIGN_NG"),0);

		m_rProductData.nCH1_GoodCount[i]			= ini.GetInt(strSection, _T("CH1_GOOD"),0);
		m_rProductData.nCH2_GoodCount[i]			= ini.GetInt(strSection, _T("CH2_GOOD"),0);
		m_rProductData.nCH3_GoodCount[i]			= ini.GetInt(strSection, _T("CH3_GOOD"),0);
		m_rProductData.nCH4_GoodCount[i]			= ini.GetInt(strSection, _T("CH4_GOOD"),0);
		m_rProductData.nCH1_NgCount[i]				= ini.GetInt(strSection, _T("CH1_NG"),0);
		m_rProductData.nCH2_NgCount[i]				= ini.GetInt(strSection, _T("CH2_NG"),0);
		m_rProductData.nCH3_NgCount[i]				= ini.GetInt(strSection, _T("CH3_NG"),0);
		m_rProductData.nCH4_NgCount[i]				= ini.GetInt(strSection, _T("CH4_NG"),0);
		m_rProductData.nNoDisplay[i]				= ini.GetInt(strSection, _T("NO_DISP"),0);

	}
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 모델별 Recipe file 초기화
//
// file에 각종 모델별 설정값을 저장/관리한다.
//
void CMCMain::InitRecipeFile()
{
	CSTNPreAlign*	pPreAlign = (CSTNPreAlign *)((CGxStationHub::GetInstance())->GetStation( NAME_STATION_PreAlign ));	// PreAlign Station pointer
	CSTNAVCamera*	pAVCamera = (CSTNAVCamera *)((CGxStationHub::GetInstance())->GetStation( NAME_STATION_AVCamera ));
	CJigInfo*		pJigInfo = CJigInfo::GetInstance();			// Jig 정보 관리자

	// Recipe 모델은 기본적으로 현재 적용중인  m_rRunData.sModelName에서 지정 ( iniProfile에서 저장/관리)

	// 여기부터 새로운 Recipe 관리 방법에 의한 데이터 관리
	// 각 좌표를 항목별로 나누어 관리한다.

	// 2016-11-09, jhLee, Key 값 앞에 언더바 _ 를 더하면 RMS에 보고가 되지 않는 항목이 된다.
	m_iniRecipe.AddInt(_T("CellDisp:_nDotCountX"),			m_rCellDisp.nDotCountX);
	m_iniRecipe.AddInt(_T("CellDisp:_nDotCountY"),			m_rCellDisp.nDotCountY);
	m_iniRecipe.AddInt(_T("CellDisp:_nFontSize"),				m_rCellDisp.nFontSize);

	m_iniRecipe.AddInt(_T("CellDisp:_nDotSizeX"),				m_rCellDisp.nDotSizeX);
	m_iniRecipe.AddInt(_T("CellDisp:_nDotSizeY"),				m_rCellDisp.nDotSizeY);
	m_iniRecipe.AddInt(_T("CellDisp:_nDotCount"),				m_rCellDisp.nDotCount);
	m_iniRecipe.AddInt(_T("CellDisp:_nResolutionX"),			m_rCellDisp.nResolutionX);
	m_iniRecipe.AddInt(_T("CellDisp:_nResolutionY"),			m_rCellDisp.nResolutionY);
	m_iniRecipe.AddInt(_T("CellDisp:_nDZonePtnMax"),			m_rCellDisp.nDZonePtnMax);	
	m_iniRecipe.AddBool(_T("CellDisp:_bRigidModel"),			m_rCellDisp.bRigidModel);			// 리지드 모델 여부, FALSE:UB, TRUE:Rigid model

	//! 중요, 반드시 설정되어야만 Align을 수행한다 !! 
	m_iniRecipe.AddDouble(_T("CellDisp:dMarkDistance"),	m_rCellDisp.dbMarkDistance);		// 두 Align mark간 거리 



	// Recipe 변경 감지용 Recipe 보관 
	m_iniRecipeBack.AddInt(_T("CellDisp:_nDotCountX"),			m_rCellDispBack.nDotCountX);
	m_iniRecipeBack.AddInt(_T("CellDisp:_nDotCountY"),			m_rCellDispBack.nDotCountY);
	m_iniRecipeBack.AddInt(_T("CellDisp:_nFontSize"),			m_rCellDispBack.nFontSize);

	m_iniRecipeBack.AddInt(_T("CellDisp:_nDotSizeX"),			m_rCellDispBack.nDotSizeX);
	m_iniRecipeBack.AddInt(_T("CellDisp:_nDotSizeY"),			m_rCellDispBack.nDotSizeY);
	m_iniRecipeBack.AddInt(_T("CellDisp:_nDotCount"),			m_rCellDispBack.nDotCount);
	m_iniRecipeBack.AddInt(_T("CellDisp:_nResolutionX"),		m_rCellDispBack.nResolutionX);
	m_iniRecipeBack.AddInt(_T("CellDisp:_nResolutionY"),		m_rCellDispBack.nResolutionY);
	m_iniRecipeBack.AddInt(_T("CellDisp:_nDZonePtnMax"),		m_rCellDispBack.nDZonePtnMax);	
	m_iniRecipeBack.AddBool(_T("CellDisp:_bRigidModel"),		m_rCellDispBack.bRigidModel);		// 리지드 모델 여부, FALSE:UB, TRUE:Rigid model
	m_iniRecipeBack.AddDouble(_T("CellDisp:dMarkDistance"),	m_rCellDispBack.dbMarkDistance);		// 두 Align mark간 거리 
	
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


	// PreAlign
	m_iniRecipe.AddDouble(_T("PreAlign:Align1_X"),		pPreAlign->m_dbPosData[CSTNPreAlign::eIDX_MY_AXIS_X][ePOS_PREALIGN_Align1]);	// 왼쪽 얼라인마크 X 위치
	m_iniRecipe.AddDouble(_T("PreAlign:Align1_Y"),		pPreAlign->m_dbPosData[CSTNPreAlign::eIDX_MY_AXIS_Y][ePOS_PREALIGN_Align1]);	// 왼쪽 얼라인마크 Y 위치
	m_iniRecipe.AddDouble(_T("PreAlign:Align2_X"),		pPreAlign->m_dbPosData[CSTNPreAlign::eIDX_MY_AXIS_X][ePOS_PREALIGN_Align2]);	// 오른쪽 얼라인마크 X 위치
	m_iniRecipe.AddDouble(_T("PreAlign:Align2_Y"),		pPreAlign->m_dbPosData[CSTNPreAlign::eIDX_MY_AXIS_Y][ePOS_PREALIGN_Align2]);	// 오른쪽 얼라인마크 Y 위치
	m_iniRecipe.AddDouble(_T("PreAlign:MCR1_X"),			pPreAlign->m_dbPosData[CSTNPreAlign::eIDX_MY_AXIS_X][ePOS_PREALIGN_MCR1]);		// MCR 1 X 위치
	m_iniRecipe.AddDouble(_T("PreAlign:MCR1_Y"),			pPreAlign->m_dbPosData[CSTNPreAlign::eIDX_MY_AXIS_Y][ePOS_PREALIGN_MCR1]);		// MCR 1 Y 위치
	m_iniRecipe.AddDouble(_T("PreAlign:MCR2_X"),			pPreAlign->m_dbPosData[CSTNPreAlign::eIDX_MY_AXIS_X][ePOS_PREALIGN_MCR2]);		// MCR 2 X 위치
	m_iniRecipe.AddDouble(_T("PreAlign:MCR2_Y"),			pPreAlign->m_dbPosData[CSTNPreAlign::eIDX_MY_AXIS_Y][ePOS_PREALIGN_MCR2]);		// MCR 2 Y 위치

	// Template Center 위치 저장
	m_iniRecipe.AddDouble(_T("AlignMark:_MARK1_CH1_L_X"),	pPreAlign->m_dpMarkCenter[eIDX_MARK1_CH1_L].x);	
	m_iniRecipe.AddDouble(_T("AlignMark:_MARK1_CH1_L_Y"),	pPreAlign->m_dpMarkCenter[eIDX_MARK1_CH1_L].y);	
	m_iniRecipe.AddDouble(_T("AlignMark:_MARK1_CH1_R_X"),	pPreAlign->m_dpMarkCenter[eIDX_MARK1_CH1_R].x);	
	m_iniRecipe.AddDouble(_T("AlignMark:_MARK1_CH1_R_Y"),	pPreAlign->m_dpMarkCenter[eIDX_MARK1_CH1_R].y);	
	m_iniRecipe.AddDouble(_T("AlignMark:_MARK1_CH2_L_X"),	pPreAlign->m_dpMarkCenter[eIDX_MARK1_CH2_L].x);	
	m_iniRecipe.AddDouble(_T("AlignMark:_MARK1_CH2_L_Y"),	pPreAlign->m_dpMarkCenter[eIDX_MARK1_CH2_L].y);	
	m_iniRecipe.AddDouble(_T("AlignMark:_MARK1_CH2_R_X"),	pPreAlign->m_dpMarkCenter[eIDX_MARK1_CH2_R].x);	
	m_iniRecipe.AddDouble(_T("AlignMark:_MARK1_CH2_R_Y"),	pPreAlign->m_dpMarkCenter[eIDX_MARK1_CH2_R].y);	

	m_iniRecipe.AddDouble(_T("AlignMark:_MARK2_CH1_L_X"),	pPreAlign->m_dpMarkCenter[eIDX_MARK2_CH1_L].x);	
	m_iniRecipe.AddDouble(_T("AlignMark:_MARK2_CH1_L_Y"),	pPreAlign->m_dpMarkCenter[eIDX_MARK2_CH1_L].y);	
	m_iniRecipe.AddDouble(_T("AlignMark:_MARK2_CH1_R_X"),	pPreAlign->m_dpMarkCenter[eIDX_MARK2_CH1_R].x);	
	m_iniRecipe.AddDouble(_T("AlignMark:_MARK2_CH1_R_Y"),	pPreAlign->m_dpMarkCenter[eIDX_MARK2_CH1_R].y);	
	m_iniRecipe.AddDouble(_T("AlignMark:_MARK2_CH2_L_X"),	pPreAlign->m_dpMarkCenter[eIDX_MARK2_CH2_L].x);	
	m_iniRecipe.AddDouble(_T("AlignMark:_MARK2_CH2_L_Y"),	pPreAlign->m_dpMarkCenter[eIDX_MARK2_CH2_L].y);	
	m_iniRecipe.AddDouble(_T("AlignMark:_MARK2_CH2_R_X"),	pPreAlign->m_dpMarkCenter[eIDX_MARK2_CH2_R].x);	
	m_iniRecipe.AddDouble(_T("AlignMark:_MARK2_CH2_R_Y"),	pPreAlign->m_dpMarkCenter[eIDX_MARK2_CH2_R].y);	


	// [ AVCamera ]
	m_iniRecipe.AddDouble(_T("AVCamera:JIG1_CH1_X"),	pAVCamera->m_dbPosData[CSTNAVCamera::eIDX_MY_AXIS_X][ePOS_AVCAM_JIG1_Ch1]);
	m_iniRecipe.AddDouble(_T("AVCamera:JIG1_CH2_X"),	pAVCamera->m_dbPosData[CSTNAVCamera::eIDX_MY_AXIS_X][ePOS_AVCAM_JIG1_Ch2]);
	m_iniRecipe.AddDouble(_T("AVCamera:JIG2_CH1_X"),	pAVCamera->m_dbPosData[CSTNAVCamera::eIDX_MY_AXIS_X][ePOS_AVCAM_JIG2_Ch1]);
	m_iniRecipe.AddDouble(_T("AVCamera:JIG2_CH2_X"),	pAVCamera->m_dbPosData[CSTNAVCamera::eIDX_MY_AXIS_X][ePOS_AVCAM_JIG2_Ch2]);

	m_iniRecipe.AddDouble(_T("AVCamera:JIG1_CH1_Z"),	pAVCamera->m_dbPosData[CSTNAVCamera::eIDX_MY_AXIS_Z][ePOS_AVCAM_JIG1_Ch1]);
	m_iniRecipe.AddDouble(_T("AVCamera:JIG1_CH2_Z"),	pAVCamera->m_dbPosData[CSTNAVCamera::eIDX_MY_AXIS_Z][ePOS_AVCAM_JIG1_Ch2]);
	m_iniRecipe.AddDouble(_T("AVCamera:JIG2_CH1_Z"),	pAVCamera->m_dbPosData[CSTNAVCamera::eIDX_MY_AXIS_Z][ePOS_AVCAM_JIG2_Ch1]);
	m_iniRecipe.AddDouble(_T("AVCamera:JIG2_CH2_Z"),	pAVCamera->m_dbPosData[CSTNAVCamera::eIDX_MY_AXIS_Z][ePOS_AVCAM_JIG2_Ch2]);

	m_iniRecipe.AddDouble(_T("AVCamera:JIG1_CH1_T"),	pAVCamera->m_dbPosData[CSTNAVCamera::eIDX_MY_AXIS_T][ePOS_AVCAM_JIG1_Ch1]);
	m_iniRecipe.AddDouble(_T("AVCamera:JIG1_CH2_T"),	pAVCamera->m_dbPosData[CSTNAVCamera::eIDX_MY_AXIS_T][ePOS_AVCAM_JIG1_Ch2]);
	m_iniRecipe.AddDouble(_T("AVCamera:JIG2_CH1_T"),	pAVCamera->m_dbPosData[CSTNAVCamera::eIDX_MY_AXIS_T][ePOS_AVCAM_JIG2_Ch1]);
	m_iniRecipe.AddDouble(_T("AVCamera:JIG2_CH2_T"),	pAVCamera->m_dbPosData[CSTNAVCamera::eIDX_MY_AXIS_T][ePOS_AVCAM_JIG2_Ch2]);

	m_iniRecipe.AddDouble(_T("AVCamera:JIG1_CH1_F"),	pAVCamera->m_dbPosData[CSTNAVCamera::eIDX_MY_AXIS_F][ePOS_AVCAM_JIG1_Ch1]);
	m_iniRecipe.AddDouble(_T("AVCamera:JIG1_CH2_F"),	pAVCamera->m_dbPosData[CSTNAVCamera::eIDX_MY_AXIS_F][ePOS_AVCAM_JIG1_Ch2]);
	m_iniRecipe.AddDouble(_T("AVCamera:JIG2_CH1_F"),	pAVCamera->m_dbPosData[CSTNAVCamera::eIDX_MY_AXIS_F][ePOS_AVCAM_JIG2_Ch1]);
	m_iniRecipe.AddDouble(_T("AVCamera:JIG2_CH2_F"),	pAVCamera->m_dbPosData[CSTNAVCamera::eIDX_MY_AXIS_F][ePOS_AVCAM_JIG2_Ch2]);

	m_iniRecipe.AddDouble(_T("AVCamera:JIG1_CH1_P"),	pAVCamera->m_dbPosData[CSTNAVCamera::eIDX_MY_AXIS_P][ePOS_AVCAM_JIG1_Ch1]);
	m_iniRecipe.AddDouble(_T("AVCamera:JIG1_CH2_P"),	pAVCamera->m_dbPosData[CSTNAVCamera::eIDX_MY_AXIS_P][ePOS_AVCAM_JIG1_Ch2]);
	m_iniRecipe.AddDouble(_T("AVCamera:JIG2_CH1_P"),	pAVCamera->m_dbPosData[CSTNAVCamera::eIDX_MY_AXIS_P][ePOS_AVCAM_JIG2_Ch1]);
	m_iniRecipe.AddDouble(_T("AVCamera:JIG2_CH2_P"),	pAVCamera->m_dbPosData[CSTNAVCamera::eIDX_MY_AXIS_P][ePOS_AVCAM_JIG2_Ch2]);

	m_iniRecipe.AddDouble(_T("AVCamera:_Safety_P"),	pAVCamera->m_dbSafetyPos);	// 평행광 Z 축 이동 안전 위치


	// 2015-07-16, jhLee, 신호기 TMD 정보를 RMS Recipe 항목으로 관리한다.
	if ( m_rProfileData.bUse_TMDVersionCheck == TRUE )	// 각 채널별 TMD Version을 관리할것인가 ?
	{
		m_iniRecipe.AddStr(_T("TMD_VERSION_CHECK:_JIG1_1"),	pJigInfo->m_ChData[eIDX_JIG_1][eIDX_CH_1].sPG_TMDModelSaved );
		m_iniRecipe.AddStr(_T("TMD_VERSION_CHECK:_JIG1_2"),	pJigInfo->m_ChData[eIDX_JIG_1][eIDX_CH_2].sPG_TMDModelSaved );
		m_iniRecipe.AddStr(_T("TMD_VERSION_CHECK:_JIG2_1"),	pJigInfo->m_ChData[eIDX_JIG_2][eIDX_CH_1].sPG_TMDModelSaved );
		m_iniRecipe.AddStr(_T("TMD_VERSION_CHECK:_JIG2_2"),	pJigInfo->m_ChData[eIDX_JIG_2][eIDX_CH_2].sPG_TMDModelSaved );
	}


	// 그 밖에 모델별로 필요한 내용들을 설정한다.

}


// 현재 모델의 Align Mark가 모두 존재하는가 ?
BOOL CMCMain::CheckAlignMarkFile()
{
	BOOL bResult = TRUE;

	// Mark Template 파일 이름
	CString sMarkFiles[eMAX_MARKCount] = {	NAME_FILE_MARK1_CH1_L, NAME_FILE_MARK1_CH1_R, 
											NAME_FILE_MARK1_CH2_L, NAME_FILE_MARK1_CH2_R,
											NAME_FILE_MARK2_CH1_L, NAME_FILE_MARK2_CH1_R, 
											NAME_FILE_MARK2_CH2_L, NAME_FILE_MARK2_CH2_R };

	//CSTNPreAlign*	pPreAlign	= (CSTNPreAlign *)((CGxStationHub::GetInstance())->GetStation( NAME_STATION_PreAlign ));	// PreAlign Station pointer
	//CMkNxCam*		pCamCtrl	= pPreAlign->GetGrabber();					// Align 작업을 위한 Grabber pointer
	CString sTempFile;
	CString sModelPath;
	int i;

	sModelPath.Format(_T("%s\\Model\\%s\\"), m_sBasePath, m_rProfileData.sModelName);		// Recipe 경로 지정

	// Align용 Templte 파일을 읽어온다.

	for (i = 0; i < eMAX_MARKCount; i++ )				// Template 수량만큼
	{
		sTempFile.Format(_T("%s%s"), sModelPath, sMarkFiles[i]);		// 파일 이름 지정

		if ( -1 == GetFileAttributes( sTempFile ) )		// 파일이 존재하지 않는다면,
		{
			bResult = FALSE;	// Align용 Template파일들을 정상적으로 읽지 못했다.
			break;
		}
		
	}

	return bResult;
}


// 지정한 이름의 Recipe 파일을 불러와서 적용시킨다.
BOOL CMCMain::LoadRecipeFile(CString sModel)
{
	// 생산 모델의 Recipe 정보
	if ( sModel.IsEmpty() == TRUE )						// 모델명이 정상이 아니라면,
	{
		return FALSE;
	}


	// 경로명 : C:\UB-AMT\Model\A12345\A12345.ini
	CString sFullPath;
	sFullPath.Format(_T("%s\\Model\\%s\\%s.ini"), m_sBasePath, sModel, sModel);			// 파일 전체 경로 완성

	// file이 존재하는지 점검한다.
	if ( PathFileExists( sFullPath ) == FALSE )		// 파일이 존재하지 않는다면,
	{
		CString sMsg1,sMsg2,sMsg3;

		sMsg1.Format(_T("지정한 [%s] 레서피가 존재하지 않습니다."), sModel);
		sMsg2.Format(_T("Recipe File [%s] Not Exist"), sModel);
		sMsg3.Format(_T("Recipe [%s] Không tồn tại."), sModel);
		
		
		//::AfxMessageBox( sMsg );
		return FALSE;
	}
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	// Mark Template 파일 이름
	CString sMarkFiles[eMAX_MARKCount] = {	NAME_FILE_MARK1_CH1_L, NAME_FILE_MARK1_CH1_R, 
											NAME_FILE_MARK1_CH2_L, NAME_FILE_MARK1_CH2_R,
											NAME_FILE_MARK2_CH1_L, NAME_FILE_MARK2_CH1_R, 
											NAME_FILE_MARK2_CH2_L, NAME_FILE_MARK2_CH2_R };

	CSTNPreAlign*	pPreAlign	= (CSTNPreAlign *)((CGxStationHub::GetInstance())->GetStation( NAME_STATION_PreAlign ));	// PreAlign Station pointer
	CMkNxCam*		pCamCtrl	= pPreAlign->GetGrabber();					// Align 작업을 위한 Grabber pointer
	CString sTempFile;
	int i;


	m_rProfileData.sModelName = sModel;		// 모델 지정
	m_sPath_Recipe.Format(_T("%s\\Model\\%s\\"), m_sBasePath, m_rProfileData.sModelName);		// Recipe 경로 지정

	m_iniRecipe.m_sIniFile	= sFullPath;				// 읽어올 파일 이름 지정 
	m_iniRecipe.Load();										// 기본적인 설정값 Loading

	// 변화 감지용 데이터 저장
	m_iniRecipeBack.m_sIniFile	= sFullPath;				// 읽어올 파일 이름 지정 
	m_iniRecipeBack.Load();									// 기본적인 설정값 Loading

	// Align용 Templte 파일을 읽어온다.
	m_rRuntimeData.bStateAlignTemplate = TRUE;		// Align용 Template파일들을 정상적으로 읽었는가 /

	for (i = 0; i < eMAX_MARKCount; i++ )				// Template 수량만큼
	{
		sTempFile.Format(_T("%s%s"), m_sPath_Recipe, sMarkFiles[i]);		// 파일 이름 지정

		if ( -1 != GetFileAttributes( sTempFile ) )		// 파일이 존재한다면,
		{
			pCamCtrl->PcvLoadTemplate(i, sTempFile);		//	OPENCV :Template 파일을 읽어와 적용시킨다.
		}
		else
			m_rRuntimeData.bStateAlignTemplate = FALSE;	// Align용 Template파일들을 정상적으로 읽지 못했다.
	}

	// 값을 점검한다.
	if ( m_rCellDisp.nFontSize <= 0 ) m_rCellDisp.nFontSize = 100;							// 글씨 기본크기
	if ( m_rCellDisp.nDotCountX <= 0) m_rCellDisp.nDotCountX = 1;
	if ( m_rCellDisp.nDotCountY <= 0) m_rCellDisp.nDotCountY = 1;
	if ( m_rCellDisp.nDZonePtnMax <= 0) m_rCellDisp.nDotCountX = 7;



	m_rRuntimeData.bStateLoadRecipe = m_rRuntimeData.bStateAlignTemplate;				// 레서피 파일을 정상적으로 읽어왔는가 ?

	// return m_rProfileData.bStateLoadRecipe;				// 최종 결과

	// 여기까지 수행하면 일단 모델 변경은 성공한 것이다.
	return TRUE;
}



// 지정한 이름으로 현재의 데이터를 Recipe 파일로 저장시킨다.
void CMCMain::SaveRecipeFile(CString sModel)
{
	CString sFullPath;

	if ( !sModel.IsEmpty() )		// 내용이 있는 모델명이라면,
	{
		// 경로명 : C:\UB-AMT\Model\A12345\A12345.ini
		sFullPath.Format(_T("%s\\Model\\%s"), m_sBasePath , sModel);		// 지정 모델에 해당되는 폴더 생성
		m_iniRecipe.MakeFoloer( sFullPath );

		// 모델 파일 전체 경로를 지정한다.
		m_iniRecipe.m_sIniFile.Format(_T("%s\\Model\\%s\\%s.ini"), m_sBasePath, sModel, sModel);
	}
	
	// 내용 저장
	m_iniRecipe.Save();
}


// 지정한 이름으로 현재의 Recipe을 저장한다.
void CMCMain::SaveAsRecipeFile(CString sModel)
{
	CString sFullPath;
	CString sOldFile = m_iniRecipe.m_sIniFile;			// 현재의 파일 경로를 보관한다.

	if ( !sModel.IsEmpty() )		// 내용이 있는 모델명이라면,
	{
		// 경로명 : C:\UB-AMT\Model\A12345\A12345.ini
		sFullPath.Format(_T("%s\\Model\\%s"), m_sBasePath , sModel);		// 지정 모델에 해당되는 폴더 생성
		m_iniRecipe.MakeFoloer( sFullPath );

		// 모델 파일 전체 경로를 지정한다.
		m_iniRecipe.m_sIniFile.Format(_T("%s\\Model\\%s\\%s.ini"), m_sBasePath, sModel, sModel);

		// 내용 저장
		m_iniRecipe.Save();
		m_iniRecipe.m_sIniFile = sOldFile;		// 보관된 파일경로를 복원한다.
	}
}


// Recipe값 변경 감지
BOOL CMCMain::CheckRecipeChange()
{
	m_iniRecipeBack.Load();						// 현재 저장된 기본적인 설정값 Loading

	// 파일의 값과 변경되 내용이 있는가 ?
	if ( m_rCellDisp.nDotCountX != m_rCellDispBack.nDotCountX ) return TRUE;
	if ( m_rCellDisp.nDotCountY != m_rCellDispBack.nDotCountY)  return TRUE;
	if ( m_rCellDisp.nFontSize !=  m_rCellDispBack.nFontSize) return TRUE;

	if ( m_rCellDisp.nDotSizeX != m_rCellDispBack.nDotSizeX) return TRUE;
	if ( m_rCellDisp.nDotSizeY != m_rCellDispBack.nDotSizeY) return TRUE;
	if ( m_rCellDisp.nDotCount != m_rCellDispBack.nDotCount) return TRUE;
	if ( m_rCellDisp.nResolutionX != m_rCellDispBack.nResolutionX) return TRUE;
	if ( m_rCellDisp.nResolutionY != m_rCellDispBack.nResolutionY) return TRUE;
	if ( m_rCellDisp.nDZonePtnMax != m_rCellDispBack.nDZonePtnMax) return TRUE;	
	
	if ( m_rCellDisp.dbMarkDistance != m_rCellDispBack.dbMarkDistance)  return TRUE;		// 두 Align mark간 거리 
	if ( m_rCellDisp.bRigidModel !=  m_rCellDispBack.bRigidModel) return TRUE;

	return FALSE;
}


// Recipe의 내용중 지정된 Section name에 해당되는 데이터를 저장한다.
void CMCMain::SaveRecipeData(CString sSec)
{
	m_iniRecipe.Save( sSec );		// Section 지정 저장
}


// 지정 Section의 값들을 불러온다.
void CMCMain::LoadRecipeData(CString sSec)
{
	m_iniRecipe.Load( sSec );		// Section 지정 불러오기
}



// 현재 PPID 조회
CString CMCMain::GetCurrentPPID(void)
{
	return m_rProfileData.sPPID;
}

// 새로운 PPID 적용
void	CMCMain::SetNewPPID(CString sData)
{
	m_rProfileData.sPPID = sData;
	m_iniRecipe.Save( _T("Setting") );			// 저장
}


// 각 Jig의 채널 사용여부 지정/조회
// 지정 채널의 미사용 모드 지정/해제
void	CMCMain::SetChannelNotUseMode(int nJig, int nCh, BOOL bFlag)
{
	if ( (nJig >= 0) && (nJig < eMAX_JigCount) && (nCh >= 0) && (nCh < eMAX_ChCount) )
	{
		m_rProfileData.bChannelNotUseFlag[nJig][nCh] = bFlag;		// 채널 미사용 설정 여부, 0:사용, 1:미사용
	}
}

// 지정 채널의  미사용 모드 조회
BOOL	CMCMain::GetChannelNotUseMode(int nJig, int nCh)
{
	if ( (nJig >= 0) && (nJig < eMAX_JigCount) && (nCh >= 0) && (nCh < eMAX_ChCount) )
	{
		return m_rProfileData.bChannelNotUseFlag[nJig][nCh];		// 채널 미사용 설정 여부, 0:사용, 1:미사용
	}

	return FALSE;
}




// 폴더를 만들어주고 파일 이름을 되돌려준다.
// D:\UB-AMT   \ImgLog\Align   \2014-07\2014-07-26    \1234567_CH1L_   224525  _OK.BMP
// Basepath			sFolder					자동생성				sFile				자동		나중에 추가
CString CMCMain::MakeFolderNFilename(CString sFolder, CString sFile)
{
	CString sFullPath;
	CString sFilePath;

	SYSTEMTIME	tmNow;
	CTime			tmToday;

	::GetLocalTime( &tmNow );			// 현재 시각을 취득
	tmToday = GetToday();				// Log를 남길 날짜를 취득한다.

	// 폴더명 : D:\UB-AMT   \ImgLog\Align   \2014-07\2014-07-26
	//old sFullPath.Format(_T("%s\\%s\\%04u-%02u\\%04u-%02u-%02u"), m_sBasePath, sFolder, tmNow.wYear, tmNow.wMonth, tmNow.wYear, tmNow.wMonth, tmNow.wDay);
	sFullPath.Format(_T("%s\\%s\\%04u-%02u\\%04u-%02u-%02u"), m_sBasePath, sFolder,tmToday.GetYear(), tmToday.GetMonth(), 
							tmToday.GetYear(), tmToday.GetMonth(), tmToday.GetDay() );
	
	// 지정 폴더가 존재하지 않는다면,
	if ( PathIsDirectory( sFullPath ) == FALSE )
	{
		BOOL bRet;
		//char szFolder[250];		// MAX length is 248
		//memset(szFolder, 0, sizeof(szFolder));
		//	::WideCharToMultiByte(CP_ACP, 0, sFullPath, -1, szFolder, 250, 0, 0);		// CString -> char*로 변형시켜준다.

		bRet = (ERROR_SUCCESS == SHCreateDirectoryEx(NULL, sFullPath, NULL));			// 폴더를 생성해준다.
	}
		
	//old sFilePath.Format(_T("%s\\%s_%02u%02u%02u"), sFullPath, sFile, tmNow.wHour, tmNow.wMinute, tmNow.wSecond );		// 파일 이름을 생성해준다.
	sFilePath.Format(_T("%s\\%s_%02u%02u%02u"), sFullPath, sFile, tmToday.GetHour(), tmToday.GetMinute(), tmToday.GetSecond() );		// 파일 이름을 생성해준다.

	return sFilePath;
}


// 하위 폴더까지 만들어주고 파일 이름을 되돌려준다.
// D:\UB-AMT   \ImgLog\Align   \2014-07\2014-07-26  \Img_OK  \1234567_CH1L_   224525				_OK.BMP
// Basepath			sFolder					자동생성			sSub		sFile				자동(시분초)		나중에 추가
CString CMCMain::MakeSubFolderNFilename(CString sFolder, CString sSub, CString sFile)
{
	CString sFullPath;
	CString sFilePath;

	SYSTEMTIME	tmNow;
	CTime		tmToday;

	::GetLocalTime( &tmNow );			// 현재 시각을 취득
	tmToday = GetToday();				// Log를 남길 날짜를 취득한다.

	// 폴더명 : D:\UB-AMT   \ImgLog\Align   \2014-07\2014-07-26
	if ( sSub == _T("") )			// 하위 폴더명이 지정되어있지 않다면,
	{
		// sFullPath.Format(_T("%s\\%s\\%04u-%02u\\%04u-%02u-%02u"), m_sBasePath, sFolder, tmNow.wYear, tmNow.wMonth, tmNow.wYear, tmNow.wMonth, tmNow.wDay);
		// 2014-09-25, Cell log와 동일한 날자 체계로 적용한다. 지정된 시각 이전에는 이전 날짜를 사용한다.
		sFullPath.Format(_T("%s\\%s\\%04u-%02u\\%04u-%02u-%02u"), m_sBasePath, sFolder, tmToday.GetYear(), tmToday.GetMonth(), 
							tmToday.GetYear(), tmToday.GetMonth(), tmToday.GetDay() );
	}
	else
		// sFullPath.Format(_T("%s\\%s\\%04u-%02u\\%04u-%02u-%02u\\%s"), m_sBasePath, sFolder, tmNow.wYear, tmNow.wMonth, tmNow.wYear, tmNow.wMonth, tmNow.wDay, sSub);
		sFullPath.Format(_T("%s\\%s\\%04u-%02u\\%04u-%02u-%02u\\%s"), m_sBasePath, sFolder, tmToday.GetYear(), tmToday.GetMonth(), 
							tmToday.GetYear(), tmToday.GetMonth(), tmToday.GetDay(), sSub);
	
	// 지정 폴더가 존재하지 않는다면,
	if ( PathIsDirectory( sFullPath ) == FALSE )
	{
		BOOL bRet;
		//char szFolder[250];		// MAX length is 248
		//memset(szFolder, 0, sizeof(szFolder));
		//	::WideCharToMultiByte(CP_ACP, 0, sFullPath, -1, szFolder, 250, 0, 0);		// CString -> char*로 변형시켜준다.

		bRet = (ERROR_SUCCESS == SHCreateDirectoryEx(NULL, sFullPath, NULL));			// 폴더를 생성해준다.
	}
		
	// sFilePath.Format(_T("%s\\%s_%02u%02u%02u"), sFullPath, sFile, tmNow.wHour, tmNow.wMinute, tmNow.wSecond );		// 시분초를 더하여 파일 이름을 생성해준다.
	sFilePath.Format(_T("%s\\%s_%02u%02u%02u"), sFullPath, sFile, tmToday.GetHour(), tmToday.GetMinute(), tmToday.GetSecond() );

	return sFilePath;
}


// 지정된 하루 단위 시각에 맞춰서 기록을 담당할 '오늘' 날자값을 얻어온다.
// 각종 Log의 파일 이름에 사용된다.
//
CTime	CMCMain::GetToday()
{
	CTime time = CTime::GetCurrentTime();

	// 하루는 나누는 시각과 비교하여 시/분으로 기준을 나누어준다.
	if (time.GetHour() == (int)m_rProfileData.nCellLog_Hour)	// Cell Log 파일 생성에 기준이되는 하루의 시작 시간 00 ~ 23
	{
		// 기준으로 정해놓은 '시'가 같은경우
		if (time.GetMinute() < (int)m_rProfileData.nCellLog_Min)		// Cell Log 파일 생성에 기준이된는 하루의 시작 분 00 ~ 59
		{
			// 지정한 '분'을 아직 넘기지 못했다면, 어제 날짜로 사용한다.
			CTimeSpan timeOffset(1, 0, 0, 0);				// 하루
			time -= timeOffset;								// 오늘 날짜에서 하루를 빼준다.
		}
	}
	else if(time.GetHour() <  (int)m_rProfileData.nCellLog_Hour)		// 아직 기준 시간에 도달하지 못했다 : 어제날짜
	{
		CTimeSpan timeOffset(1, 0, 0, 0);
		time -= timeOffset; 
	}

	return time;
}


// 지정한 날짜/시각이 기준시간상 전날인가 /
BOOL CMCMain::GetIsPrevDay(CTime &tmDay)
{
	// 하루는 나누는 시각과 비교하여 시/분으로 기준을 나누어준다.
	if (tmDay.GetHour() == (int)m_rProfileData.nCellLog_Hour)			// Cell Log 파일 생성에 기준이되는 하루의 시작 시간 00 ~ 23
	{
		// 기준으로 정해놓은 '시'가 아직 지정 분 전일경우
		if(tmDay.GetMinute() < (int)m_rProfileData.nCellLog_Min)		// Cell Log 파일 생성에 기준이된는 하루의 시작 분 00 ~ 59
		{
			return TRUE;			// 아직 전날이다.
		}
	}
	else if(tmDay.GetHour() <  (int)m_rProfileData.nCellLog_Hour)		// 아직 기준 시간에 도달하지 못했다 : 어제날짜
	{
		return TRUE;			// 아직 전날이다.
	}

	return FALSE;
}


// 현재 시각에 따른 Shift를 되돌려준다.
//int CMCMain::GetShift()
int CMCMain::GetShift(int nNewHour /* =0 */, BOOL bChange /* = FALSE */)
{
	CTime tmNow = CTime::GetCurrentTime();
	int nOptHour = m_rProfileData.nCellLog_Hour;		// 하루의 기준을 나누는 시간

	int nHour =0;

	// bChange가 TRUE일경우 nNewHour에 대한 Shift을 리턴해준다. (Product Data 표시해줄때 사용하기 위해서)
	if ( bChange == TRUE )
		nHour = nNewHour;
	else
		nHour = tmNow.GetHour();

	// 2015/03/06 JH-PARK
	// 근무조 방식 3개조,2개조로 선택가능하도록 , 김길성K 요청

	// Tip
	// 3개조로 표기할경우 DAY,SW,GY를 모두 사용
	// 2개조로 표기할경우 DAY,SW만 사용
	// DAY -> D2로 대체
	// SW -> G2로 대체

	// 시간대별로 작업조를 나누어준다.

	///////////////////////////////////////////////////
	// 2개조 방식으로 SHIFT 구별
	if ( m_rProfileData.nMode_WorkShift == eMODE_WorkShift2) 
	{
		//? if ( (nHour >= nOptHour) && (nHour < nOptHour+12) )							// Day2
		if ( (nHour >= nOptHour) && (nHour < (nOptHour + 12)) )							
		{
			return eIDX_Shift_DAY2;
		}
		//? else if ( (nHour >= nOptHour+12 && nHour < 24) || (nHour < nOptHour) )			// GY2
		else if ( ( (nHour >= (nOptHour + 12)) && (nHour < 24)) || (nHour < nOptHour) )		
		{
			return eIDX_Shift_GY2;
		}
			
		return eIDX_Shift_DAY2;										// 아니라면(?) 기본값으로 주간근무조


	} // end of if (eMODE_Work2nd)
	else 
	{
	///////////////////////////////////////////////////
	// 3개조 방식으로 SHIFT 구별
		//? if ( (nHour >= nOptHour) && (nHour < nOptHour+8) )					// DAY
		if ( (nHour >= nOptHour) && (nHour < (nOptHour + 8)) )					// DAY
		{
			return eIDX_Shift_DAY;
		}
		//? else if ( (nHour >= nOptHour+8) && (nHour < nOptHour+16) )			// Swing
		else if ( (nHour >= (nOptHour + 8)) && (nHour < (nOptHour + 16)) )			// Swing
		{
			return eIDX_Shift_SW;
		}
		//? else if ( (nHour >= nOptHour+16 && nHour < 24) || (nHour < nOptHour) )			// GY
		else if ( ( (nHour >= (nOptHour + 16)) && (nHour < 24) ) || (nHour < nOptHour) )			// GY
		{
			return eIDX_Shift_GY;
		}
			
		return eIDX_Shift_DAY;										// 아니라면(?) 기본값으로 DAY
	} // end of else ( eMODE_Work3rd)



	//if ( (nHour >= nOptHour) && (nHour < nOptHour+8) )					// DAY
	//{
	//	return eIDX_Shift_DAY;
	//}
	//else if ( (nHour >= nOptHour+8) && (nHour < nOptHour+16) )			// Swing
	//{
	//	return eIDX_Shift_SW;
	//}
	//else if ( (nHour >= nOptHour+16 && nHour < 24) || (nHour < nOptHour) )			// GY
	//{
	//	return eIDX_Shift_GY;
	//}

	return eIDX_Shift_DAY;										// 아니라면(?) 기본값으로 DAY
}


//
// 현재 시각에 따른 근무조 (Shift)를 되돌려준다.
// 
int CMCMain::GetCurrentShift()
{
	CTime tmNow = CTime::GetCurrentTime();				// 현재 시각
	int nOptHour = m_rProfileData.nCellLog_Hour;		// 하루의 기준을 나누는 시간

	int nHour = tmNow.GetHour();						// 현재 몇시인가 ?



	// 2015/03/06 JH-PARK
	// 근무조 방식 3개조,2개조로 선택가능하도록 , 김길성K 요청

	// Tip
	// 3개조로 표기할경우 DAY,SW,GY를 모두 사용
	// 2개조로 표기할경우 DAY,SW만 사용
	// DAY -> Day2로 대체
	// SW -> GY2로 대체

	// 시간대별로 작업조를 나누어준다.

	///////////////////////////////////////////////////
	// 2개조 방식으로 SHIFT 구별 (SDBN)
	if ( m_rProfileData.nMode_WorkShift == eMODE_WorkShift2) 
	{
		// DAY2 낮 근무조인가 ? 통상 8시 ~ 20시
		if ( (nHour >= nOptHour) && (nHour < (nOptHour + 12)) )							
		{
			return eIDX_Shift_DAY2;
		}
		else if ( ( (nHour >= (nOptHour + 12)) && (nHour <= 23)) || (nHour < nOptHour) )	// 야간 조 인가 ?		
		{
			return eIDX_Shift_GY2;
		}
			
		return eIDX_Shift_DAY2;										// 아니라면(?) 기본값으로 주간근무조

	} // end of if (eMODE_Work2nd)
	///////////////////////////////////////////////////
	// 3개조 방식으로 SHIFT 구별
	else 
	{
		if ( (nHour >= nOptHour) && (nHour < (nOptHour + 8)) )								// DAY
		{
			return eIDX_Shift_DAY;
		}
		else if ( (nHour >= (nOptHour + 8)) && (nHour < (nOptHour + 16)) )					// Swing
		{
			return eIDX_Shift_SW;
		}
		else if ( ( (nHour >= (nOptHour + 16)) && (nHour < 24) ) || (nHour < nOptHour) )	// GY
		{
			return eIDX_Shift_GY;
		}
			
		return eIDX_Shift_DAY;										// 아니라면(?) 기본값으로 DAY
	} // end of else ( eMODE_Work3rd)

	
	return eIDX_Shift_DAY;										// 아니라면(?) 기본값으로 DAY
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Gaus 시작 순서  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

// 1. Frame 객체를 생성한다.
//
// [in] CWnd* pOwner : CMainFrame의 this pointer를 넘겨준다. 각종 Framework의 Message를 전달 받는다. STOP/START/ALARM ...
// [in] CString sBase : 프로그램 실행의 중심이 되는 Base path를 지정한다. 이 경로 하위 폴더인 \Data\ 폴더에서 각종 설정값을 읽어온다.
//
void CMCMain::SystemCreate(CWnd* pOwner, CString sBase)
{
	// 아직 생성이 되어있지 않다면
	if ( !m_pGausFrame )
	{
		m_sBasePath = sBase;											// 기본 경로 대입
		SetOwnerHandle(pOwner->m_hWnd);							// 소유자의 핸들을 대입시킨다.

		m_pCellInfo		= CCellInfo::GetInstance();			// Cell Data 저장 Class
		m_pJigInfo		= CJigInfo::GetInstance();
		m_pStatsBlock	= CStatsBlock::GetStatsBlock();		// 통계정보
		m_pStatsBlock->Init( m_sBasePath );						// 통계정보를 기록 할 객체

		// Gaus Frame
		m_pGausFrame = CGxGausFrame::GetInstance();
		m_pGausFrame->SetOwnerHandle( pOwner->m_hWnd );		// MCMain 할당시에 ((CMainFrame*)AfxGetMainWnd())->m_hWnd );			// CMainFrame의 핸들을 넘겨준다.
		m_pGausFrame->CreateSystem( sBase );

		// Socket 관리자에게 통신 연결 상태 변경을 받아들일 Message를 알려준다.
		m_pSocketMgr = CGxSocketMgr::GetInstance();
		m_pSocketMgr->SetRetryInterval( 5 );						// 자동 재시도 간격을 초단위로 설정한다.
		m_pSocketMgr->SetEventHandle(pOwner, UDMSG_COMM_STATE, NULL);
		m_pSocketMgr->GetLogPtr()->SetFilePath(sBase + _T("\\Log"), _T("Comm"), _T("SockMgr"));

	}
}



// 2. 각종 System/Service/Station을 생성하여 추가한다.
//
// 
void CMCMain::SystemAdd()
{
	if ( m_pGausFrame )
	{
		// Service 추가
		m_pGausFrame->AddService( new CSvcAccura() );			// Accura 전력량계 통신 처리 시스템
		m_pGausFrame->AddService( new CSvcGmsRead() );			// GMS 통신 시스템
		m_pGausFrame->AddService( new CSvcVacRead() );			// 공압 통신 시스템
		m_pGausFrame->AddService( new CSvcTempRead() );			// 통도 통신 시스템
		m_pGausFrame->AddService( new CSvcTorquePanasonic() );		// Panasonic Servo motor Torque 통신 처리 시스템
		m_pGausFrame->AddService( new CSvcAlignLight() );			// 조명 컨트롤
		m_pGausFrame->AddService( new CSvcLabelPrinter() );		// Label 프린터

		m_pGausFrame->AddService( new CSvcTcpCIM() );							// Data-PC CIM 통신 
		m_pGausFrame->AddService( new CSvcTcpPG() );								// 신호기 통신
		m_pGausFrame->AddService( new CSvcTcpVI() );								// 검사기 PC 1 통신
		m_pGausFrame->AddService( new CSvcTcpMCR(0) );							// Cognex MCR Reader 통신
		//미사용 m_pGausFrame->AddService( new CSvcTorqueAnalog() );			// DD motor Torque KM6015 통신 처리 시스템
		//미사용 m_pGausFrame->AddService( new CSvcTorqueSankyo() );			// Sankyo motor Torque 통신 처리 시스템


		// System 추가
		m_pGausFrame->AddSystem( new CSysPanel() );				// 전면 버튼 감지 system을 생성하여 등록한다. 
		m_pGausFrame->AddSystem( new CSysTower() );				// 경광등 관리 System을 생성하여 등록한다. 
		m_pGausFrame->AddSystem( new CSysSafety() );			// 안전관련 Interlock 관리 System을 생성하여 등록한다. 
//d		m_pGausFrame->AddSystem( new CSysVIMonitor() );			// VI 검사 결과파일 모니터링 시스템
		m_pGausFrame->AddSystem( new CSysFDCWrite() );			// FDC 항목 저장 시스템


		// Station 추가
		m_pGausFrame->AddStation( new CSTNMainStation() );		// MainStation 생성
		m_pGausFrame->AddStation( new CSTNPreAlign() );			// Pre-Align Station 생성
		m_pGausFrame->AddStation( new CSTNWorkTable() );			// Work-Table Station 생성
		m_pGausFrame->AddStation( new CSTNAVCamera() );				// AVCamera Station 생성
		m_pGausFrame->AddStation( new CSTNVIChannel(eIDX_CH_1) );	// 화상 검사 처리용 Station 생성, 채널#1 용
		m_pGausFrame->AddStation( new CSTNVIChannel(eIDX_CH_2) );	// 화상 검사 처리용 Station 생성, 채널#2 용

	}
}



// 각종 파일들의 초기화와 불러오기를 수행한다.
void CMCMain::InitialDataFiles()
{
	// 새로운 방식의 Cell Log 파일을 기록한다.
	CString sHeader;
	CString sFile;

		sHeader  = _T("INNER ID,MODEL,CELL ID,READ UNIT,EQP ID,JIG NAME,CH NAME,CONTACT COUNT,MC VER,PRODUCT ID,U VER,D VER,UNIATS VERSION,MICRO DLL VERSION,MACRO DLL VERSION,SETBIN VERSION,UNLOADER OUT TACT TIME,BIN2 CELL INFO,PG_UI,PROGRAM NAME,CH Card MAC,JIG NO,CIM_LOG,CIM_MTP,CIM_TSP,TACT TIME,TABLE TURN TIME,CELL LOAD TACT,CELL UNLOAD TACT START TIME,CELL UNLOAD TACT,CELL UNLOAD TACT END TIME,LAST CLASS,LAST RESULT,PRINT DEFECT,PRINT CODE, MES CODE"); 
		sHeader += _T(",AZONE START TIME,AZONE END TIME,AZONE TACT TIME,AZONE WAITING,AZONE CLASS,AZONE DEFECT,AZONE TSP PIN DEFECT,AZONE REG DEFECT,AZONE CONTACT START TIME,AZONE CONTACT END TIME,AZONE CONTACT TACT TIME,AZONE CONTACT CLASS,AZONE Contact IBAT,AZONE Contact IVSS,AZONE Contact IVDD,AZONE Contact IVCI,AZONE Contact IBAT2,AZONE Contact IDD2,AZONE CURRENT START TIME,AZONE CURRENT END TIME,AZONE CURRENT TACT TIME,AZONE CURRENT CLASS,AZONE IBAT,AZONE IVSS,AZONE IVDD,AZONE IVCI,AZONE IBAT2,AZONE IDD2,AZONE ALIGN START TIME,AZONE ALIGN END TIME,AZONE ALIGN TACT TIME,AZONE ALIGN DEGREE,AZONE ALIGN SUCCESS,N/A,N/A");
		sHeader += _T(",CZONE START TIME,CZONE END TIME,CZONE TACT TIME,CZONE WAITING,CZONE CLASS,CZONE DEFECT,CZONE VISION START TIME,CZONE VISION END TIME,CZONE VISION TACT TIME,CZONE VISION GRAB TIME,CZONE VISION CLASS TIME,CZONE VISION CLASS,CZONE VISION DEFECT,CZONE TECHECK START TIME,CZONE TECHECK END TIME,CZONE TECHECK TACT TIME,CZONE TECHECK RESULT,CZONE TECHECK EXECUTE,CZONE TSP START TIME,CZONE TSP END TIME,CZONE TSP TACT TIME,CZONE TSP RESULT,CZONE TSP EXECUTE,CZONE WHITE CURRENT START TIME,CZONE WHITE CURRENT END TIME,CZONE WHITE CURRENT TACT TIME,CZONE WHITE CURRENT CLASS,CZONE WHITE IBAT,CZONE WHITE IVSS,CZONE WHITE IVDD,CZONE WHITE IVCI,CZONE WHITE IBAT2,CZONE WHITE IDD2,N/A,N/A");
		sHeader += _T(",DZONE START TIME,DZONE END TIME,DZONE TACT TIME,DZONE WAITING TIME,DZONE CLASS,DZONE DEFECT,DZONE MANUAL2 START TIME,DZONE MANUAL2 END TIME,DZONE MANUAL2 TACT TIME,DZONE MANUAL2 CLASS,DZONE MANUAL2 DEFECT,DZONE CURRENT START TIME,DZONE CURRENT END TIME,DZONE CURRENT TACT TIME,DZONE CURRENT CLASS,DZONE IBAT,DZONE IVSS,DZONE IVDD,DZONE IVCI,DZONE IBAT2,DZONE IDD2,SLEEP CURRENT START TIME,SLEEP CURRENT END TIME,SLEEP CURRENT TACT TIME,SLEEP CURRENT CLASS,SLEEP IBAT,SLEEP IVSS,SLEEP IVDD,SLEEP IVCI,SLEEP IBAT2,SLEEP IDD2,N/A,N/A");
		sHeader += _T(",TIME PG OFF1,TIME PG ON,TIME START BUTTON,TIME MCR ALIGN,TIME CELL LOADING,TIME WAITING1,TIME TABLE TURN1,TIME VISION 1CH,TIME CAMERA 2CH MOVE,TIME VISION 2CH,TIME WAITING2,TIME TABLE TURN2,TIME PG OFF2,EMPTY,N/A,N/A");
		sHeader +=_T(",ID,CYCLE TIME,EQ TIME,OP TIME,WAIT TIME,LOSS TIME,NO INPUT TIME,OPEN CELL,N/A,N/A");
		sHeader += _T(",TACT LOAD UNLOAD,TACT A ZONE INSP,TACT A ZONE MCR ALIGN,TACT A ZONE,TACT WAITING1,TACT TABLE TURN1,TACT C ZONE VISION 1CH,TACT C ZONE CAMERA MOVE,TACT C ZONE VISION 2CH,TACT C ZONE,TACT WAITING2,TACT TABLE TURN2,TACT D ZONE,TACT TIME,TOTAL TACT,N/A,N/A,POC INFO,DEEP CHECK,DEFECT POS");

	m_logCellLog.SetHeader( sHeader );						// Cell log 본체
	m_logCellBIN2Log.SetHeader( sHeader );					// BIN2Test 기록용 Cell Log

	// Cell log 본체
	sFile.Format(_T("%s"), m_rProfileData.sEQID );			// , m_rProfileData.sCellLog_LineID);
	m_logCellLog.SetFilePath(m_rProfileData.sDIR_CellLog, _T("CELL_DATA"), sFile);				// Data Log
	m_logCellLog.m_sExtention		= _T("csv");																// 확장자
	m_logCellLog.m_nDirNameMode	= CGxLog::eNAME_MonthOnly;												// 월별로 생성한다. (년은 생략),		\LOG\09\test.txt
	m_logCellLog.m_nDirDatePos		= CGxLog::ePOS_DateFirst;												// 경로의 중간 구분자 앞에 날짜를 넣는다.	D:\AUTO_TEST\LOG\05\CELL_DATA\20150512_CELL_Y01.csv
	m_logCellLog.m_nFileDatePos	= CGxLog::ePOS_DateFirst;												// 파일 이름 앞에 날짜를 넣는다.	D:\AUTO_TEST\LOG\05\CELL_DATA\20150512_CELL_Y01.csv
	m_logCellLog.m_bIsHeaderPrint = TRUE;																		// Header 출력
	m_logCellLog.m_bIsUnicode		= FALSE;																		// ANSI (MBCS)로 file에 기록한다.
	m_logCellLog.m_bIsTimePrint		= FALSE;																		// 데이터 앞에 자동으로 추가돼ㅣ는 시간을 기록하지 않는다.
	m_logCellLog.m_nKeepDay			= 90;																			// 90일치 데이터만 보존한다.
	m_logCellLog.m_bIsAutoDelete	= TRUE;																		// 보존 기간을 경과한 이전 log를 자동 삭제한다.
	m_logCellLog.m_bKeepOpen		= TRUE;																		// 파일을 Opne한 채로 진행한다.
	m_logCellLog.m_bMirrorCreate	= TRUE;																		// 미러링 파일을 생성한다.
	m_logCellLog.m_bIsFolderCreate	= TRUE;
	m_logCellLog.m_sMirrorSuffix	= _T("Temp");																// 미러링 파일의 접두사 지정
	m_logCellLog.m_sDeliMiliSec		= _T(":");															// 밀리초는 : 으로 구분해준다.

	// BIN2 Test 시 기록할 log
	sFile.Format(_T("%s_BIN2"), m_rProfileData.sEQID );		// , m_rProfileData.sCellLog_LineID);
	m_logCellBIN2Log.SetFilePath(m_rProfileData.sDIR_CellLog, _T("CELL_DATA_BIN2"), sFile);	// Data Log
	m_logCellBIN2Log.m_sExtention		= _T("csv");															// 확장자
	m_logCellBIN2Log.m_nDirNameMode	= CGxLog::eNAME_MonthOnly;											// 월별로 생성한다. (년은 생략),		\LOG\09\test.txt
	m_logCellBIN2Log.m_nDirDatePos	= CGxLog::ePOS_DateFirst;											// 경로의 중간 구분자 앞에 날짜를 넣는다.	D:\AUTO_TEST\LOG\05\CELL_DATA\20150512_CELL_Y01.csv
	m_logCellBIN2Log.m_nFileDatePos	= CGxLog::ePOS_DateFirst;											// 파일 이름 앞에 날짜를 넣는다.	D:\AUTO_TEST\LOG\05\CELL_DATA\20150512_CELL_Y01.csv
	m_logCellBIN2Log.m_bIsHeaderPrint = TRUE;																	// Header 출력
	m_logCellBIN2Log.m_bIsUnicode		= FALSE;																	// ANSI (MBCS)로 file에 기록한다.
	m_logCellBIN2Log.m_bIsTimePrint	= FALSE;																	// 데이터 앞에 자동으로 추가돼ㅣ는 시간을 기록하지 않는다.
	m_logCellBIN2Log.m_nKeepDay		= 90;																		// 90일치 데이터만 보존한다.
	m_logCellBIN2Log.m_bIsAutoDelete	= TRUE;																	// 보존 기간을 경과한 이전 log를 자동 삭제한다.
	m_logCellBIN2Log.m_bKeepOpen		= TRUE;																	// 파일을 Opne한 채로 진행한다.
	m_logCellBIN2Log.m_bMirrorCreate	= TRUE;																	// 미러링 파일을 생성한다.
	m_logCellBIN2Log.m_bIsFolderCreate	= TRUE;
	m_logCellBIN2Log.m_sMirrorSuffix	= _T("Temp");															// 미러링 파일의 접두사 지정
	m_logCellBIN2Log.m_sDeliMiliSec		= _T(":");															// 밀리초는 : 으로 구분해준다.



	// 비가동 Log에 대한 파일 설정을 한다.
	sHeader = _T("EQP ID,MODEL,ID,ALARM TYPE,START TIME,RESET TIME,END TIME,WAIT TIME,REPAIR TIME,DURATION,FIRST CLASS,SECOND CLASS,THIRD CLASS,TP SYSTEM LOSS NAME,TP SYSTEM LOSS CODE,ALARM CODE,REPAIR CODE");
	m_logLosstime.SetHeader( sHeader );

	sFile.Format(_T("Losstime_%s"), m_rProfileData.sCellLog_LineID);						// 예: 20150413_LOSSTIME_Y01.csv
	m_logLosstime.SetFilePath(m_rProfileData.sDIR_LossTime, _T("MAIN"), sFile);				// Data Log
	m_logLosstime.m_bIsUnicode		= FALSE;				// MBCS로 저장한다.
	m_logLosstime.m_bKeepOpen		= TRUE;					// 파일을 열은 채로 처리하여 다른곳에서의 접근을 막는다.
	m_logLosstime.m_bMirrorCreate	= TRUE; 				// 미러 파일을 만든다.
	m_logLosstime.m_bIsHeaderPrint	= TRUE;																		// Header 출력
	m_logLosstime.m_bIsFolderCreate	= TRUE;
	m_logLosstime.m_sExtention		= _T("csv");			// 확장자 지정
	m_logLosstime.m_sMirrorSuffix	= _T("Temp");			// 미러링 파일의 접두사 지정
	m_logLosstime.m_sDeliMiliSec	= _T(":");				// 밀리초는 : 으로 구분해준다.
	// 6, 2, 1 : D:\UB-AMT\Log\Sample\201509\20150904_LogTest.txt	-> Loss Time (비가동 로그) 기록용 설정값
	m_logLosstime.m_nDirNameMode	= CGxLog::eNAME_YearMonth;
	m_logLosstime.m_nDirDatePos		= CGxLog::ePOS_DateLast;
	m_logLosstime.m_nFileDatePos	= CGxLog::ePOS_DateFirst;



	// Alarm
	m_logAlarm.SetHeader(_T("START TIME,END TIME,SPAN,CODE,NAME,DESCRIPT"));			// Heaer 지정
	sFile.Format(_T("Alarm_%s"), m_rProfileData.sCellLog_LineID);						// 예: 20150413_Alarm_Y01.csv
	m_logAlarm.SetFilePath(m_rProfileData.sDIR_MachineData,	_T("Alarm"), sFile );		// Alarm 기록용, m_rProfileData.sDIR_AlarmLog
	m_logAlarm.m_bIsHeaderPrint = TRUE;													// Header 출력
	m_logAlarm.m_bIsFolderCreate	= TRUE;
	m_logAlarm.m_sExtention = _T("csv");												// File 확장자
	m_logAlarm.m_sDeliMiliSec	= _T(":");												// 밀리초는 : 으로 구분해준다.
	m_logAlarm.m_nDirNameMode	= CGxLog::eNAME_YearMonth;
	m_logAlarm.m_nDirDatePos	= CGxLog::ePOS_DateLast;
	m_logAlarm.m_nFileDatePos	= CGxLog::ePOS_DateFirst;
	m_logAlarm.m_bIsThreadLock = FALSE;

	// TACT Time
	sHeader = _T("In Time,Out Time,Ch No,Cell ID,Total TACT,Machine TACT,User TACT,TACT Time,Jig No,User Name,PG ON,Ready Btn,Tilt Up,UV MCR Read,Align Move1,Align Mark1,Align Move2,Align Mark2,PG Cell Loading,Turn Wait,Turn Move,VT CH1 Align,VT CH1 Grab,VT CH2 Align,VT CH2 Grab,Return Wait,Return Move,Tilt Down,Manual");
	m_logTACTTime.SetHeader( sHeader );
	sFile.Format(_T("TACT_%s"), m_rProfileData.sCellLog_LineID);						// 예: 20150413_TACT_Y01.csv
	m_logTACTTime.SetFilePath(m_rProfileData.sDIR_MachineData,	_T("TACT"), sFile );	// TACT Time 기록용 파일 경로 지정
	m_logTACTTime.m_bIsHeaderPrint = TRUE;												// Header 출력
	m_logTACTTime.m_bIsFolderCreate	= TRUE;
	m_logTACTTime.m_sExtention = _T("csv");												// File 확장자
	m_logTACTTime.m_bIsDatePrint = FALSE;												// 날짜 및 시각을 넣지 않는다.
	m_logTACTTime.m_bIsTimePrint = FALSE;												//
	m_logTACTTime.m_sDeliMiliSec	= _T(":");											// 밀리초는 : 으로 구분해준다.
	m_logTACTTime.m_nDirNameMode	= CGxLog::eNAME_YearMonth;
	m_logTACTTime.m_nDirDatePos	= CGxLog::ePOS_DateLast;
	m_logTACTTime.m_nFileDatePos	= CGxLog::ePOS_DateFirst;


	// Contact율 data
	// m_dlogProductData.SetHeader(_T("Line,Time,Total_Cnt,Contact_Cnt,Contact_OK,Contact_NG,Contact_Ratio,NoDisplay,Contact_CH1_Ratio,Contact_CH2_Ratio,Contact_CH3_Ratio,Contact_CH4_Ratio"));	// Heaer 지정

	m_logContactData.SetHeader(_T("Line,Time,Total_Cnt,Contact_Cnt,Contact_OK,Contact_NG,Contact_Ratio,NoDisplay,Contact_CH1_Ratio,Contact_CH2_Ratio,Contact_CH3_Ratio,Contact_CH4_Ratio"));	// Heaer 지정
	sFile.Format(_T("Contact_%s"), m_rProfileData.sCellLog_LineID);						// 예: 20150413_TACT_Y01.csv
	m_logContactData.SetFilePath(m_rProfileData.sDIR_MachineData,	_T("Contact"), sFile );	// Contact률 기록용 m_rProfileData.sDIR_ProductLog,	
	m_logContactData.m_bIsHeaderPrint = TRUE;																		// Header 출력
	m_logContactData.m_bIsFolderCreate	= TRUE;
	m_logContactData.m_sExtention = _T("csv");													// File 확장자
	m_logContactData.m_nDirNameMode	= CGxLog::eNAME_YearMonth;
	m_logContactData.m_nDirDatePos	= CGxLog::ePOS_DateLast;
	m_logContactData.m_nFileDatePos	=CGxLog:: ePOS_DateFirst;


	// 생산 데이터, 이름을 취득하기 위해 사용되고 실제적인 데이터는 ini 파일 형태로 갱신한다.
	sFile.Format(_T("Product_%s"), m_rProfileData.sCellLog_LineID);						// 예: 20150413_TACT_Y01.csv
	m_logProductData.SetFilePath(m_rProfileData.sDIR_MachineData,	_T("Product"), sFile );	// 생산 정보 기록 파일 이름 취득용
	m_logProductData.m_bIsFolderCreate	= TRUE;
	m_logProductData.m_sExtention = _T("txt");													// File 확장자
	m_logProductData.m_nDirNameMode	= CGxLog::eNAME_YearMonth;
	m_logProductData.m_nDirDatePos	= CGxLog::ePOS_DateLast;
	m_logProductData.m_nFileDatePos	= CGxLog::ePOS_DateFirst;

	// 각종 설정 데이터 Loading

	InitRecipeFile();									// Recipe파일과 변수들을 결합

	// 생산량 정보를 기록하고 관리하는 파일 초기화
	InitProductFile();
	LoadProductFile();		// 데이터 읽어오기


	LoadLosstimeItemList();							// 비가동 정보를 파일에서 읽어온다.
	LoadRepairAlarmItemList();						// Alarm 조치항목 정보를 파일에서 읽어온다.

	// 비가동 정보를 임시 기억하기위한 초기화
	InitLosstimeInfo();
	LoadLosstimeInfo();

	
	LoadTodayProductData();							// 금일 진행된 생산량을 가져온다.;
	LoadRuntimeData();								// 동작 현황 데이터를 불러온다.
	LoadExceptConfigFile();							// 예외 처리에 대한 설정값들을 읽어온다.

	LoadAlarmInfo();									// Alarm Code를 읽어와서 CList에 추가, Setup6에서 사용,
	LoadUserAccount();								// File에서 작업자 계정 정보를 읽어온다.
	LoadJudgeCellID();								// 제외시킬 Cell ID를 파일에서 읽어온다.


	// 각종 데이터로그의 날짜 지정
	SetUpdateLogToday(m_rProfileData.nCellLog_Hour, m_rProfileData.nCellLog_Min);			// 하루 기준 대입
}


// 3. 각종 제어기 장치를 초기화 시킨다.
void CMCMain::SystemInitial()
{
	// Gaus Frame의 System 초기화를 실시한다.
	if ( m_pGausFrame )
	{
		InitProfile();												// 각종 정보 저장관리 지정
		LoadProfile();												// Profile 데이터를 읽어온다.
		InitialDataFiles();										// 각종 파일들의 초기화와 불러오기를 수행한다.

		// Defect 정보 읽기
		LoadDefectList();									// Defect List 데이터를 파일에서 읽어들인다. 
		LoadDefectNameTable();							// Defect Name 매칭 테이블 데이터 읽기

		SetLogMgr();												// Log Manager에게 관리할 Log를 지정한다.

		m_pGausFrame->InitialSystem();
	}
}


// 4. 자동 제어를 시작한다.
void CMCMain::SystemStartup()
{
	if ( m_pGausFrame )
	{
		CGxAlarmCtrl *pAlarmCtrl = CGxAlarmCtrl::GetInstance();
		pAlarmCtrl->SetCycleStop( m_bCycleStop );  // Alarm 발생시에 Cycle Stop을 할것인지 여부

		// 자동 시스템 시작
		m_pGausFrame->StartupSystem();
		// m_pLogMgr->Startup( 10 );								// Log 관리자 동작 시작, 10분에 한 번씩 오래된 로그 삭제하도록 지정

		// Axis Map
		// 내부적으로 사용 할 축의 Index를 구한다.
		CGxAxisMgr*	pAxisMgr = CGxAxisMgr::GetInstance();	
		m_nAxesMap[eAXIS_WorkTableT]	= pAxisMgr->GetAxisIndex( NAME_AXIS_WORK_TABLE_T );		// Work Table축을 얻어온다.
		m_nAxesMap[eAXIS_AVCameraF]		= pAxisMgr->GetAxisIndex( NAME_AXIS_AV_CAMERA_F );		// AV Camera F 축
		m_nAxesMap[eAXIS_PreAlignX]		= pAxisMgr->GetAxisIndex( NAME_AXIS_PRE_ALIGN_X );		// Pre Align X 축
		m_nAxesMap[eAXIS_PreAlignY]		= pAxisMgr->GetAxisIndex( NAME_AXIS_PRE_ALIGN_Y );		// Pre Align Y 축
		m_nAxesMap[eAXIS_AVCameraX]		= pAxisMgr->GetAxisIndex( NAME_AXIS_AV_CAMERA_X );		// AV Camera X 축
		m_nAxesMap[eAXIS_AVCameraZ]		= pAxisMgr->GetAxisIndex( NAME_AXIS_AV_CAMERA_Z );		// AV Camera Z 축
		m_nAxesMap[eAXIS_AVCameraT]		= pAxisMgr->GetAxisIndex( NAME_AXIS_AV_CAMERA_T );		// AV Camera T 축
		m_nAxesMap[eAXIS_AVCameraP]		= pAxisMgr->GetAxisIndex( NAME_AXIS_AV_CAMERA_P );		// AV Camera P 축

		m_bEnabled = TRUE;										// 이제부터 Machine System을 동작할 수 있는 상태가 되었다.
	}
}



// 5. 각종 FDC 측정용 데이터 취득을 시작한다.
void CMCMain::ServiceStartup()
{
	if ( !m_pGausFrame ) return;

	CGxServiceHub*	pSvcHub = CGxServiceHub::GetInstance();
	CGxSystemHub*	pSysHub = CGxSystemHub::GetInstance();

	// 전력량을 측정하기 위한 Accura 2300과의 통신을 시작한다.
	CSvcAccura* pAccura	= (CSvcAccura*)( pSvcHub->GetService(NAME_SERVICE_Accura) );
	pAccura->SetProcess( TRUE );


	// 공압 읽기용 Service를 기동한다.
	CSvcVacRead* pSvcVacRead	= (CSvcVacRead*)( pSvcHub->GetService(NAME_SERVICE_VacRead) );
	pSvcVacRead->SetPort( m_rProfileData.nVACPort );							// 진공 A/D 보드의 COM Port
	pSvcVacRead->SetProcess( TRUE );

	// PC-Rack 온도 읽기
	CSvcTempRead* pSvcTempRead	= (CSvcTempRead*)( pSvcHub->GetService(NAME_SERVICE_TempRead) );
	pSvcTempRead->SetPort( m_rProfileData.nPCTempPort );						// PC-RACK 온도의 COM Port
	pSvcTempRead->SetProcess( TRUE );

	// GMS 통신을 시작한다.
	CSvcGmsRead* pSvcGmsRead	= (CSvcGmsRead*)( pSvcHub->GetService(NAME_SERVICE_GmsRead) );
	pSvcGmsRead->SetPort( m_rProfileData.nGMSPort );		// GMS 통신용 COM Port
	pSvcGmsRead->SetProcess( TRUE );


	// Align 조명 컨트롤러와 통신을 열어준다.
	CSvcAlignLight* pSvcAlignLight	= (CSvcAlignLight*)( pSvcHub->GetService(NAME_SERVICE_AlignLight) );
	pSvcAlignLight->SetPort( m_rProfileData.nAlignLightPort );		// Align 조명 컨트롤러 COM Port
	pSvcAlignLight->SetProcess( TRUE );

	// Label Printer와 통신을 열어준다.
	CSvcLabelPrinter* pSvcLabelPrinter	= (CSvcLabelPrinter*)( pSvcHub->GetService(NAME_SERVICE_LabelPrinter) );
	pSvcLabelPrinter->SetPort( m_rProfileData.nPrinterPort );		// Label Printer의 COM Port
	pSvcLabelPrinter->SetProcess( TRUE );

	// Panasonic Servo AMP와 통신을 열어준다.
	CSvcTorquePanasonic* pTorqueServo	= (CSvcTorquePanasonic*)( (CGxSystemHub*)(CGxServiceHub::GetInstance())->GetService(NAME_SERVICE_TorquePanasonic) );
	pTorqueServo->SetPort( m_rProfileData.nPanasonicPort );		// COM Port 지정
	pTorqueServo->SetProcess( TRUE );			// 명령 처리가 가능하도록 Event를 지정한다.

	// FDC 기록용 system 동작
	CSysFDCWrite* pSysFDCWrite = (CSysFDCWrite*)( pSysHub->GetSystem(NAME_SYSTEM_FDCWrite) );
	pSysFDCWrite->SetProcess(TRUE);

}







// Gaus Frame 종료 순서 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


// 1. 자동 수행을 중지 시킨다.
void CMCMain::SystemStop()
{
	CGxMaster* pMaster = CGxMaster::GetInstance();
	pMaster->SetAbort();				// 자동 시스템을 정지 시킨다.

	//? SleepEx(100, NULL);
}


// 2. 각종 FDC 측정용 데이터 취득을 중지 시킨다.
void CMCMain::ServiceStop()
{
	if ( !m_pGausFrame ) return;

	 CGxServiceHub*	pSvcHub = CGxServiceHub::GetInstance();
	CGxSystemHub*	pSysHub = CGxSystemHub::GetInstance();

	// FDC 기록용 system 동작
	CSysFDCWrite* pSysFDCWrite = (CSysFDCWrite*)(pSysHub->GetSystem(NAME_SYSTEM_FDCWrite) );
	pSysFDCWrite->SetProcess(FALSE);

	// Panasonic Servo AMP와 통신을 열어준다.
	CSvcTorquePanasonic* pTorqueServo	= (CSvcTorquePanasonic*)( (CGxSystemHub*)(CGxServiceHub::GetInstance())->GetService(NAME_SERVICE_TorquePanasonic) );
	pTorqueServo->SetProcess( FALSE );			// 명령 처리가 가능하도록 Event를 지정한다.

	// 전력량을 측정하기 위한 Accura 2300과의 통신을 중지한다.
	CSvcAccura* pAccura	= (CSvcAccura*)( pSvcHub->GetService(NAME_SERVICE_Accura) );
	pAccura->SetProcess( FALSE );

	// 공압 읽기용 Service를 중지한다.
	CSvcVacRead* pSvcVacRead	= (CSvcVacRead*)( pSvcHub->GetService(NAME_SERVICE_VacRead) );
	pSvcVacRead->SetProcess( FALSE );

	// PC-Rack 온도 읽기
	CSvcTempRead* pSvcTempRead	= (CSvcTempRead*)( pSvcHub->GetService(NAME_SERVICE_TempRead) );
	pSvcTempRead->SetProcess( FALSE );

	// GMS 통신을 중지한다.
	CSvcGmsRead* pSvcGmsRead	= (CSvcGmsRead*)( pSvcHub->GetService(NAME_SERVICE_GmsRead) );
	pSvcGmsRead->SetProcess( FALSE );

	// Align 조명 컨트롤러와 통신을 닫아준다.
	CSvcAlignLight* pSvcAlignLight	= (CSvcAlignLight*)( pSvcHub->GetService(NAME_SERVICE_AlignLight) );
	pSvcAlignLight->SetProcess( FALSE );

	// Label Printer와 통신을 닫아준다.
	CSvcLabelPrinter* pSvcLabelPrinter	= (CSvcLabelPrinter*)( pSvcHub->GetService(NAME_SERVICE_LabelPrinter) );
	pSvcLabelPrinter->SetProcess( FALSE );
}


// 3. 각종 장치 제어기의 장치를 Close 해준다.
void CMCMain::SystemShutdown()
{
	if ( m_pGausFrame )
	{
		m_bEnabled = FALSE;							// 이제부터 Machine System을 동작할 수 없는 상태가 되었다.
		CloseCellLogFile();							// 열려진 Cell Log file을 닫아준다.

		m_pSocketMgr->ClientClose();					// 통신 단절

		m_pGausFrame->ShutdownSystem();				// 제어기 Close
	}
}



// 4. 할당했던 객체들의 리소스를 반환한다.
void CMCMain::SystemRelease()
{
	if ( m_pGausFrame )
	{

		// 실행 데이터 관리용 객체 해제
		CCellInfo::ReleaseInstance();				// Cell 데이터 class 삭제
		CJigInfo::ReleaseInstance();				// Jig 및 Channel 정보관리 Class instance 삭제
		CStatsBlock::ReleaseStatsBlock();			// 통계정보 삭제

		RelaseAlarmInfo();
		ReleaseUserAccount();						// 종료시 계정 정보를 메모리 해제해 준다.
		ReleaseJudgeCellID();
		ReleaseDefectNameTable();					//  메모리 해제


		if ( m_pSocketMgr ) 
		{
			m_pSocketMgr->ReleaseInstance();
		}

		// 할당한 객체 파괴
		m_pGausFrame->ReleaseSystem();
		m_pGausFrame->ReleaseInstance();
		m_pGausFrame = NULL;
	}
}
//end of  Gaus Frame 종료 순서 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --




// Machine System을 사용할 수 있는 상태인가 ? 모든 초기화를 마쳤는가 ?
//
// 초기화를 온전히 마치지 못한 상태에서는 각종 메뉴의 사용에 제약을 받는다.
//
BOOL CMCMain::GetEnabled()
{
	return m_bEnabled;				// 사용 가능 여부
}

// Log Manager에게 관리할 Log를 지정한다.
void CMCMain::SetLogMgr()
{
	if ( !m_pGausFrame ) return;			// 수행할 수 없다.
	
	int i;
	CGxTask* pTask = NULL;
	CGxStationHub*	pStnHub = CGxStationHub::GetInstance();
	CGxSystemHub*	pSysHub = CGxSystemHub::GetInstance();
	CGxServiceHub*	pSvcHub = CGxServiceHub::GetInstance();
	CGxLogMgr*	pLogMgr = CGxLogMgr::GetInstance();

	pLogMgr->AddByName(_T("GausFrame"), m_pGausFrame->m_pDebug);						// Gaus Frame의 Debug log			

	// Station Log 취합
	for (i = 0; i < pStnHub->GetCount(); i++)
	{
		pTask = (CGxTask*)(pStnHub->GetStation(i));				// 등록된 Station을 취득한다.
		if ( pTask )
		{
			pLogMgr->AddByName( pTask->GetName(), pTask->GetDebugPtr() );			// Debug용 log를 취득하여 관리자 에게 등록한다.
		}
	}

	// System Log 취합
	for (i = 0; i < pSysHub->GetCount(); i++)
	{
		pTask = (CGxTask*)(pSysHub->GetSystem(i));					// 등록된 System을 취득한다.
		if ( pTask )
		{
			pLogMgr->AddByName( pTask->GetName(), pTask->GetDebugPtr() );			// Debug용 log를 취득하여 관리자 에게 등록한다.
		}
	}

	// Service Log 취합
	for (i = 0; i < pSvcHub->GetCount(); i++)
	{
		pTask = (CGxTask*)(pSvcHub->GetService(i));				// 등록된 Service를 취득한다.
		if ( pTask )
		{
			pLogMgr->AddByName( pTask->GetName(), pTask->GetDebugPtr() );			// Debug용 log를 취득하여 관리자 에게 등록한다.
		}
	}


	// 기타 별도로 정의된 log 포함
		m_logTracking.SetFilePath(m_rProfileData.sDIR_MachineData ,	_T("Tracking"),	_T("AMT_Tracking") );	// MES로 Track In/Out을 전송하는 log


		// 임시 !!
		// 각종 Log 경로 설정
		CString sLogPath;

		sLogPath.Format(_T("%s\\Log\\"), m_sBasePath);		// Log base path
		m_logBlock[eLOG_SYS].SetFilePath(sLogPath ,			_T("System"),	_T("System") );		// 시스템초기화/에러등 기초적인 LOG
		m_logBlock[eLOG_SEQ].SetFilePath(sLogPath ,			_T("System"),	_T("RunSeq") );		// 자동 실행 시퀀스 기록용
		m_logBlock[eLOG_COMM_TCP].SetFilePath(sLogPath ,	_T("Comm"),		_T("CommTCP") );	// TCP/IP 통신 로그 (공통)
		m_logBlock[eLOG_COMM_VI].SetFilePath(sLogPath ,		_T("Comm"),		_T("CommVI") );		// Vision 검사PC와의 통신 로그
		m_logBlock[eLOG_COMM_PG].SetFilePath(sLogPath ,		_T("Comm"),		_T("CommPG") );		// 신호기 PC와의 통신 로그
		m_logBlock[eLOG_COMM_DATA].SetFilePath(sLogPath ,	_T("Comm"),		_T("CommData") );	// Data-PC와의 통신 로그
		m_logBlock[eLOG_COMM_MCR].SetFilePath(sLogPath ,	_T("Comm"),		_T("CommMCR") );	// UV-MCR과의 통신 로그
		m_logBlock[eLOG_COMM_TEMP].SetFilePath(sLogPath ,	_T("Comm"),		_T("CommTemp") );	// 온도 센서와의 통신 로그
		m_logBlock[eLOG_COMM_VAC].SetFilePath(sLogPath ,	_T("Comm"),		_T("CommVAC") );	// 진공 센서와의 통신 로그

		m_logBlock[eLOG_UI].SetFilePath(sLogPath,			_T("System"),	_T("GUI") );		// UI 상에서 사용자의 입력에 대한 로그
		m_logBlock[eLOG_CIM].SetFilePath(sLogPath,		_T("System"),	_T("CIM") );		// CIM 관련 로그 SONIC - 2014.07.22.
		m_logBlock[eLOG_Teach].SetFilePath(sLogPath,		_T("System"),	_T("Teach") );		// Motor값 변경시 값 변경 내역 로그 ( Teach, Motor )

		m_logBlock[eLOG_ETC].SetFilePath(sLogPath,		_T("System"),	_T("Etc") );		// 기타 로그

		// 대표적인 기본 log의 경로 
		m_pLogSys = &(m_logBlock[eLOG_SYS]);						// System Log
		m_pLogSeq = &(m_logBlock[eLOG_SEQ]);

}

//
////------------------------------ OLD Version
//
//// 구동 시스템들을 생성한다.
//int CMCMain::CreateSystem(CString sPath)
//{
//	// 각종 Log 파일에 대한 초기화를 수행한다.
//	CString sLogPath;
//
//
//	// 기본 경로를 지정한다.
//	m_sBasePath = sPath;
//
//	// 구동 설정값들이 저장/관리되는 프로파일 데이터 설정
//	m_iniProfile.m_sIniFile.Format(_T("%s\\Data\\Profile\\MCMain.ini"), m_sBasePath);
//
//	// 각종 Log 경로 설정
//	sLogPath.Format(_T("%s\\Log\\"), m_sBasePath);		// Log base path
//	m_logBlock[eLOG_SYS].SetFilePath(sLogPath ,			_T("System"),	_T("System") );		// 시스템초기화/에러등 기초적인 LOG
//	m_logBlock[eLOG_SEQ].SetFilePath(sLogPath ,			_T("System"),	_T("RunSeq") );		// 자동 실행 시퀀스 기록용
//	m_logBlock[eLOG_COMM_TCP].SetFilePath(sLogPath ,	_T("Comm"),		_T("CommTCP") );	// TCP/IP 통신 로그 (공통)
//	m_logBlock[eLOG_COMM_VI].SetFilePath(sLogPath ,		_T("Comm"),		_T("CommVI") );		// Vision 검사PC와의 통신 로그
//	m_logBlock[eLOG_COMM_PG].SetFilePath(sLogPath ,		_T("Comm"),		_T("CommPG") );		// 신호기 PC와의 통신 로그
//	m_logBlock[eLOG_COMM_DATA].SetFilePath(sLogPath ,	_T("Comm"),		_T("CommData") );	// Data-PC와의 통신 로그
//	m_logBlock[eLOG_COMM_MCR].SetFilePath(sLogPath ,	_T("Comm"),		_T("CommMCR") );	// UV-MCR과의 통신 로그
//	m_logBlock[eLOG_COMM_TEMP].SetFilePath(sLogPath ,	_T("Comm"),		_T("CommTemp") );	// 온도 센서와의 통신 로그
//	m_logBlock[eLOG_COMM_VAC].SetFilePath(sLogPath ,	_T("Comm"),		_T("CommVAC") );	// 진공 센서와의 통신 로그
//
//	m_logBlock[eLOG_UI].SetFilePath(sLogPath,			_T("System"),	_T("GUI") );		// UI 상에서 사용자의 입력에 대한 로그
//	m_logBlock[eLOG_CIM].SetFilePath(sLogPath,		_T("System"),	_T("CIM") );		// CIM 관련 로그 SONIC - 2014.07.22.
//	m_logBlock[eLOG_Teach].SetFilePath(sLogPath,		_T("System"),	_T("Teach") );		// Motor값 변경시 값 변경 내역 로그 ( Teach, Motor )
//
//	m_logBlock[eLOG_ETC].SetFilePath(sLogPath,		_T("System"),	_T("Etc") );		// 기타 로그
//
//	// 대표적인 기본 log의 경로 
//	m_pLogSys = &(m_logBlock[eLOG_SYS]);						// System Log
//	m_pLogSeq = &(m_logBlock[eLOG_SEQ]);
//
//
//
//	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//	// 본 설비에서 사용할 고유 데이터 관리자를 생성한다.
//	m_pCellInfo		= CCellInfo::GetInstance();				// Cell Data 저장 Class
//	m_pJigInfo		= CJigInfo::GetInstance();				// Jig 및 Channel 정보 관리 class
//	m_pStatsBlock	= CStatsBlock::GetStatsBlock();			// 통계정보
//	m_pStatsBlock->Init( m_sBasePath );						// 통계정보를 기록 할 객체
//
//
//	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
//	// 2015-07-24, jhLee, 새로운 GuasFrame 생성
//	m_pGausFrame	= CGxGausFrame::GetInstance();
//	m_pGausFrame->SetOwnerHandle( m_hOwner );					// MCMain 할당시에 ((CMainFrame*)AfxGetMainWnd())->m_hWnd );			// CMainFrame의 핸들을 넘겨준다.
//
//	// 기본 경로를 지정하여 Frame을 생성해준다.
//	//예 : m_pGausFrame->CreateSystem( _T("D:\\Work\\LIB\\Gaus\\TEST_DATA\\GAUS") );
//	 m_pGausFrame->CreateSystem( m_sBasePath );					// 지정 기본 경로로 시스템을 생성해준다.
//
//
//	// 본 설비에서 자동 처리를 담당할 각종 Station과 System, Service를 생성하여 등록시킨다.
//	//  System들을 생성한다.
//	m_pGausFrame->AddSystem( new CSysPanel() );				// 전면 버튼 감지 system을 생성하여 등록한다. 
//	m_pGausFrame->AddSystem( new CSysTower() );				// 경광등 관리 System을 생성하여 등록한다. 
//	m_pGausFrame->AddSystem( new CSysSafety() );			// 안전관련 Interlock 관리 System을 생성하여 등록한다. 
//	m_pGausFrame->AddSystem( new CSysVIMonitor() );			// VI 검사 결과파일 모니터링 시스템
//	m_pGausFrame->AddSystem( new CSvcAccura() );			// Accura 전력량계 통신 처리 시스템
//	m_pGausFrame->AddSystem( new CSvcTorqueAnalog() );			// DD motor Torque KM6015 통신 처리 시스템
//	m_pGausFrame->AddSystem( new CSvcTorquePanasonic() );		// Panasonic Servo motor Torque KM6015 통신 처리 시스템
//	m_pGausFrame->AddSystem( new CSysGmsRead() );			// GMS 통신 시스템
//	m_pGausFrame->AddSystem( new CSysVacRead() );			// 공압 통신 시스템
//	m_pGausFrame->AddSystem( new CSysTempRead() );			// 통도 통신 시스템
//	m_pGausFrame->AddSystem( new CSysFDCWrite() );			// FDC 항목 저장 시스템
//
//	// 2015-08-14, jhLee, 현재 C-Zone에서의 자동 풀림 기능은 사용하지 않는다.
//	// m_pGausFrame->AddSystem( new CSysPreJudgment() );		// 사전 판정 시스템, 미사용
//
//
//	// Station들을 생성한다.
//	m_pGausFrame->AddStation( new CSTNMainStation() );		// MainStation 생성
//	m_pGausFrame->AddStation( new CSTNPreAlign() );			// Pre-Align Station 생성
//	m_pGausFrame->AddStation( new CSTNWorkTable() );			// Work-Table Station 생성
//	m_pGausFrame->AddStation( new CSTNAVCamera() );				// AVCamera Station 생성
//	m_pGausFrame->AddStation( new CSTNVIChannel(eIDX_CH_1) );	// 화상 검사 처리용 Station 생성, 채널#1 용
//	m_pGausFrame->AddStation( new CSTNVIChannel(eIDX_CH_2) );	// 화상 검사 처리용 Station 생성, 채널#2 용
//
//
//	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//	// 기본 값들을 가지고 있는 프로파일을 설정하고 읽어온다.
//	// 내부적으로 다른 Station들을 참조하기에 모든 Station들이 생성된 뒤에 호출해야 한다.
//	InitProfile();			// Profile파일과 변수들을 결합
//	LoadProfile();			// 파일에서 불러온다.
//
//
//	//OLD m_dlogAlarm.SetFilePath(NAME_DIR_ALARMLOG,	_T("Alarm") );			// Alarm 기록용
//	//m_dlogAlarm.SetFilePath(m_rProfileData.sDIR_AlarmLog,	_T("Alarm") );			// Alarm 기록용
//	//m_dlogAlarm.SetFileExtention( _T("csv") );					// File 확장자
//	//m_dlogAlarm.SetHeader(_T("START TIME,END TIME,PARTS,CODE,NAME,LEVEL"));	// Heaer 지정
//
//	//OLD m_dlogTACTTime.SetFilePath(NAME_DIR_TACTLOG,	_T("TACTTimeData") );	// TACT Time 기록용 파일 경로 지정
//	//m_dlogTACTTime.SetFilePath(m_rProfileData.sDIR_TACTLog,	_T("TACTTimeData") );	// TACT Time 기록용 파일 경로 지정
//	//m_dlogTACTTime.SetFileExtention( _T("csv") );				// File 확장자
//	//m_dlogTACTTime.SetLogDate(FALSE);								// 시각을 넣지 않는다.
//	//m_dlogTACTTime.SetHeader(			// OLD _T("StartTime,EndTime,Parts,Code,Name,Reason"));	// Heaer 지정
//	//	_T("In Time,Out Time,Ch No,Cell ID,Total TACT,Machine TACT,User TACT,TACT Time,Jig No,User Name,PG ON,Ready Btn,Tilt Up,UV MCR Read,Align Move1,Align Mark1,Align Move2,Align Mark2,PG Cell Loading,Turn Wait,Turn Move,VT CH1 Align,VT CH1 Grab,VT CH2 Align,VT CH2 Grab,Return Wait,Return Move,Tilt Down,Manual"));
//
//	// 2015/05/03 JH-PARK
//	// 생산율 data
//	//m_dlogProductData.SetFilePath(m_rProfileData.sDIR_ProductLog,	_T("Product") );			// Alarm 기록용
//	//m_dlogProductData.SetFileExtention( _T("csv") );					// File 확장자
//	//m_dlogProductData.SetHeader(_T("Line,Time,Total_Cnt,Contact_Cnt,Contact_OK,Contact_NG,Contact_Ratio,NoDisplay,Contact_CH1_Ratio,Contact_CH2_Ratio,Contact_CH3_Ratio,Contact_CH4_Ratio"));	// Heaer 지정
//
//
//
//	InitRecipeFile();									// Recipe파일과 변수들을 결합
//
//	// 생산량 정보를 기록하고 관리하는 파일 초기화
//	InitProductFile();
//	LoadProductFile();		// 데이터 읽어오기
//
//
//	// Defect List 데이터를 파일에서 읽어들인다.
//	LoadDefectList();
//	LoadDefectNameTable();							// Defect Name 매칭 테이블 데이터 읽기
//
//
//	// 비가동 정보를 파일에서 읽어온다.
//	LoadLosstimeItemList();
//
//	// Alarm 조치항목 정보를 파일에서 읽어온다.
//	LoadRepairAlarmItemList();
//
//	// File에서 작업자 계정 정보를 읽어온다.
//	LoadUserAccount();						
//
//	// 비가동 정보를 임시 기억하기위한 초기화
//	InitLosstimeInfo();
//	LoadLosstimeInfo();
//
//	// 금일 진행된 생산량을 가져온다.;
//	LoadTodayProductData();
//
//	// 동작 현황 데이터를 불러온다.
//	LoadRuntimeData();
//
//	// Alarm Code를 읽어와서 CList에 추가, Setup6에서 사용,
//	LoadAlarmInfo();
//
//
//	// 예외 처리에 대한 설정값들을 읽어온다.
//	LoadExceptConfigFile();
//
//
//	// 현재 표시언어가 어떤건지 확인하여 알려준다;
//	DisplayLanuage();
//
//	//! 2014-06-30, jhLee,
//	// 나중에 초기화를 수행하는 Vision 처리 Class의 marking 데이터 로딩을 위해 
//	// 레서피 파일의 로딩은 가장 늦게 실시한다. Startup Timer로 이동
//	//	LoadRecipeFile( m_rProfileData.sModelName );	// 마지막에서 사용되던 모델 Recipe를 불러온다.
//
//	// 각종 데이터로그의 날짜 지정
//	SetUpdateLogToday(m_rProfileData.nCellLog_Hour, m_rProfileData.nCellLog_Min);
//
//	//m_dlogAlarm.SetToday(m_rProfileData.nCellLog_Hour, m_rProfileData.nCellLog_Min);			// 하루 기준 대입
//	//m_dlogTACTTime.SetToday(m_rProfileData.nCellLog_Hour, m_rProfileData.nCellLog_Min); 
//	//m_dlogProductData.SetToday(m_rProfileData.nCellLog_Hour, m_rProfileData.nCellLog_Min); 
//
//
//
//	m_pLogSys->Write(_T("===== Create System ====="));
//	return 0;
//}
//
//
//
//// 시스템을 초기화 시킨다.
//int CMCMain::InitialSystem()
//{
//	// Gaus Frame의 System 초기화를 실시한다.
//	m_pGausFrame->InitialSystem();
//
//
//	return 0;
//}
//
//
//// 준비된 시스템들을 구동시켜준다.
//int CMCMain::StartupSystem()
//{
//	// 자동 시스템 시작
//	// 2015-07-24, jhLee, 통합 Frame
//	m_pGausFrame->StartupSystem();
//
//
//	// Axis Map
//	// 내부적으로 사용 할 축의 Index를 구한다.
//	CGxAxisMgr*	pAxisMgr = CGxAxisMgr::GetInstance();	
//	m_nAxesMap[eAXIS_WorkTableT]	= pAxisMgr->GetAxisIndex( NAME_AXIS_WORK_TABLE_T );		// Work Table축을 얻어온다.
//	m_nAxesMap[eAXIS_AVCameraF]		= pAxisMgr->GetAxisIndex( NAME_AXIS_AV_CAMERA_F );		// AV Camera F 축
//	m_nAxesMap[eAXIS_PreAlignX]		= pAxisMgr->GetAxisIndex( NAME_AXIS_PRE_ALIGN_X );		// Pre Align X 축
//	m_nAxesMap[eAXIS_AVCameraX]		= pAxisMgr->GetAxisIndex( NAME_AXIS_AV_CAMERA_X );		// AV Camera X 축
//	m_nAxesMap[eAXIS_AVCameraZ]		= pAxisMgr->GetAxisIndex( NAME_AXIS_AV_CAMERA_Z );		// AV Camera Z 축
//	m_nAxesMap[eAXIS_AVCameraT]		= pAxisMgr->GetAxisIndex( NAME_AXIS_AV_CAMERA_T );		// AV Camera T 축
//
//
//	//TEST 파일 생성용
//	//TEST Recipe file 저장 SaveRecipeFile(GetRecipeModelName());
//	//TEST SaveProfile(_T(""));
//
//	// Cycle Stop 여부를 지정한다.
//	CGxAlarmCtrl* pAlarmCtrl = CGxAlarmCtrl::GetInstance();
//	pAlarmCtrl->SetCycleStop( m_bCycleStop );							// Cycle Stop 지정
//
//
//	// 2015-08-14, jhLee, 현재 C-Zone에서의 자동 풀림 기능은 사용하지 않는다.
//	// 2015-04-03, jhLee, C-Zone PreJudgment startup
//	//CSysPreJudgment*	pPreJedgment = (CSysPreJudgment*)( (CGxSystemHub*)(CGxSystemHub::GetInstance())->GetSystem(NAME_SYSTEM_PreJudgment) );			// 경보관련 System
//	//pPreJedgment->SetRunOption( m_rProfileData.bUse_CZoneReset == TRUE );	// 감시모드 자체를 사용하지 말것인지 여부, 바로 적용해 준다.
//
//	
//	m_pLogSys->Write(_T("===== Startup System"));
//	return 0;
//}
//
//
//// 시스템들의 구동을 마치고 종료 준비를 시킨다.
//int CMCMain::ShutdownSystem(void)
//{
//	//! 자동 저장은 곤란! 명시적으로 저장시키도록 한다. SaveProfile(_T(""));						// 사용하던 Profile내용을 저장한다.
//
//	// 2015-07-24, jhLee, GausFrmae으로 통합관리
//	m_pGausFrame->ShutdownSystem();
//
//	m_pLogSys->Write(_T("===== Shutdown System"));
//	return 0;
//}
//
//
//// 생성된 구동 시스템들을 제거한다.
//int CMCMain::DeleteSystem(void)
//{
//	// 생성되었던 System들을 삭제해준다.
//
//
//	// 2015-07-24, jhLee
//	// GausFrame으로 통합 관리
//	if ( m_pGausFrame != NULL )
//	{
//
//		m_pGausFrame->ReleaseSystem();
//		m_pGausFrame->ReleaseInstance();
//
//		m_pGausFrame = NULL;
//	}
//
//	// 실행 데이터 관리용 객체 해제
//	CCellInfo::ReleaseCellInfo();				// Cell 데이터 class 삭제
//	CJigInfo::ReleaseInstance();				// Jig 및 Channel 정보관리 Class instance 삭제
//	CStatsBlock::ReleaseStatsBlock();			// 통계정보 삭제
//
//
//	ReleaseUserAccount();						// 종료시 계정 정보를 메모리 해제해 준다.
//	RelaseAlarmInfo();
//	ReleaseJudgeCellID();
//	ReleaseDefectNameTable();					//  NG List Table 메모리 해제
//
//	m_pCellInfo		= NULL;
//	m_pStatsBlock	= NULL;
//
//	return 0;
//}
//

////////////////////////////////////////////////////////////////////////////////////////////////////
// 각종 참조 함수들


// 통신 설정 조회
// 지정한 통신의 ID명을 획득한다. Index정의는 MCDefine.h
CString  CMCMain::GetTCPSymbol(int nIdx)
{
	if ( (nIdx >= 0) && (nIdx < eMAX_COMMCount) )	return m_rProfileData.sTCPSymbol[nIdx];

	return _T("");
}

CString  CMCMain::GetTCPName(int nIdx)
{
	if ( (nIdx >= 0) && (nIdx < eMAX_COMMCount) )	return m_rProfileData.sTCPName[nIdx];

	return _T("");
}

// 지정한 통신의 주소를 획득한다.
CString CMCMain::GetTCPAddress(int nIdx)
{
	if ( (nIdx >= 0) && (nIdx < eMAX_COMMCount) )	return m_rProfileData.sTCPAddress[nIdx];

	return _T("");
}

// 지정한 통신의 Port를 획득한다.
int  CMCMain::GetTCPPort(int nIdx)
{
	if ( (nIdx >= 0) && (nIdx < eMAX_COMMCount) )	return m_rProfileData.nTCPPort[nIdx];

	return 0;
}

// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==

// DATA-PC에게 Cell의 존재 여부를 전송한다.
BOOL CMCMain::SendCellExistToDataPC()
{
	CGxServiceHub* pSvcHub = CGxServiceHub::GetInstance();
	CSvcTcpCIM*	pSvcTcpCIM = (CSvcTcpCIM*)( pSvcHub->GetService(NAME_SERVICE_TcpCIM));

	TCellData* pCell;
	BOOL bExist = FALSE;
	BOOL bResult = TRUE;

	for (int i = 0; i < eMAX_JigCount; i++)
	{
		for (int j = 0; j < eMAX_ChCount; j++) 
		{
			pCell = m_pCellInfo->GetCellData(i, j);			// 지정 Jig의 각 채널의 포인트를 얻어온다.

			if ( (pCell->bIsCellDetect == TRUE) && (pCell->bIsExist == TRUE) )
			{
				bExist = TRUE;
				break;
			}
		}//of for j
	}//of for i

	if ( bExist == TRUE )
	{
		bResult = pSvcTcpCIM->CmdSetCellExist( _T(EQST_RUN_IS_CELL) );			// Cell이 존재한다.
	}
	else
		bResult = pSvcTcpCIM->CmdSetCellExist( _T(EQST_RUN_NOT_CELL) );			// Cell이 존재하지 않는다.

	return bResult;
}



// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==


// 초기화 진행여부 설정
//
//	eINIT_NONE			= 0,				// 아직 Initial 작업을 하지않음
//	eINIT_Processing,						// Initial 동작 중
//	eINIT_Complete,						// Initial 동작 
void CMCMain::SetInitialFlag(int nFlag)
{
	m_nInitialFlag = nFlag;					// 설비의 Initial 관계
}


// 초기화 진행여부 조회
int  CMCMain::GetInitialFlag(void)
{
	return m_nInitialFlag;					// 설비의 Initial 관계
}


// 생산하는 제품이 Rigid인지 youm인지 조회한다.
	// Rigid cell을 생산하는가 ?
BOOL CMCMain::IsCellRigid()
{	
	return m_rCellDisp.bRigidModel;			// 설정된 Recipe값을 반환한다.
}


// 모델 이름 반환
CString CMCMain::GetRecipeModelName(void)
{
	// Recipe 파일에서 모델이름을 획득하여 반환해준다.
	return m_rProfileData.sModelName;			// _T("A2AMS568AT02");

}

// 프로그램 버전
CString CMCMain::GetVersion(void)
{
	return m_rProfileData.sVersion;
}



// 지정 Option 설정값 조회
UINT CMCMain::GetOptionTime(int nIdx)
{
	// 일단 임의의 값을 적용한다. 
	switch ( nIdx )
	{
		case eOPTION_TM_Cylinder	:		// 실린더 동작 기본 Timeout
		case eOPTION_TM_BackLight :					// PreAlign 상부 BackLight 실린더동작 Timeout
		case eOPTION_TM_JigTilt :						// WorkTable Jig의 Tilt 동작 Timeout
			return m_rProfileData.nTIME_CylinderTimeout;				// Cylinder 동작 Timeout 밀리초

		case eOPTION_TM_JigVacOn :						// WorkTable Jig의 진공 형성 Timeout
		case eOPTION_TM_JigVacOff :					// WorkTable Jig의 진공 파기 Timeout
			return m_rProfileData.nTIME_VacuumTimeout;			// Vacuum 동작 Timeout 밀리초

		//// Motion 동작 Timeout
		//case eOPTION_TM_MoveAlign :					// Align/MCR 읽는 위치로 이동 Timeout :
		//case eOPTION_TM_MoveTurn :						// WorkTable Trun/Return 이동 Timeout :
		//case eOPTION_TM_MoveCamera :					// AVCamera들의 위치로 이동 Timeout :
		//	return 5000;

		//// 각종 검사 시간 Timeout
		//case eOPTION_TM_Align :							// Align (Mark Search) 동작 Timeout
		//case eOPTION_TM_MCRRead :						// UV-MCR 읽기동작 Timeout
		//		return 
		//case eOPTION_TM_AVCamera :						// 검사PC에의한 자동검사 Timeout ?
		//case eOPTION_TM_PGTest :						// 신호기에의한 검사 Timeout ?
		//	return 

	}//of switch

	return 3000;
}


// 지정 지그의 PG-Off 시각을 지정한다.
void CMCMain::SetPGOffTime(int nJig)
{
	if ( (nJig >= 0) && (nJig < eMAX_JigCount) )
	{
		//for old
		//m_rTactTime[nJig].SetPGOffTime();

		// 2014-10-16, jhLee, New
		// 내가사용할 PG-OFF 시각 = 상대편 JIG의 PG-OFF 시각
		int nNewJig = (nJig == eIDX_JIG_1) ? eIDX_JIG_2 : eIDX_JIG_1;			// 상대편 JIG index를 취득한다.
		TCellData* pCell;

		for (int i = 0; i < eMAX_ChCount; i++) 
		{
			pCell = m_pCellInfo->GetCellData(nNewJig, i);			// 지정 Jig의 각 채널의 포인트를 얻어온다.

			if ( pCell != NULL ) pCell->SetPrevPGOffTime( );		// 현재 시각으로 지정한다.
		}//of for i
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	CMapStringToUserAccount		m_mapUsers;	// 작업자 관리용 map

// UserInfo.ini를 가져와서 Map에 넣어준다.;
BOOL CMCMain::LoadUserAccount()
{
	CString strFileName;
	strFileName.Format(_T("%s\\INI\\%s") , m_sBasePath, NAME_FILE_USER_LIST);			// 읽어 올 파일

	// 파일이 존재하는지 점검
	if ( PathFileExists( strFileName ) == FALSE )		//  파일 존재여부
	{
		m_pLogSeq->Write(_T("[MAIN] User Account file read ERROR"));
		return FALSE;
	}

	CUserAccount*	pData;

	CString strUserID = _T("");		// ID
	CString strKorName = _T("");		// 한글이름
	CString strEngName = _T("");		// 영어이름
	CString strPw = _T("");				// 비밀번호
	CString strUserIDNo = _T("");		// 사원번호
	CString strUserLevel = _T("");	// 권한 ( 오퍼레이터:1 엔지니어:2 관리자:3)


	//파일을 한줄씩 읽어가면서 넣어준다.;
	// Text 파일 UniCode 방식으로 설정 변경 ( 베트남어 읽고 쓰기 가능)
	FILE *fStream;
	errno_t e = _tfopen_s(&fStream, strFileName.GetString(), _T("r,ccs=UTF-8"));	// UNICODE"));
	if (e != 0) return FALSE;
	CStdioFile		file(fStream);

	while(file.ReadString(strFileName))
	{
		if(strFileName.Left(1) == _T("!"))	continue;	// 처음 글자가 " ! " 일경우 다음 문장으로 넘어간다.;

		::AfxExtractSubString( strUserID,		strFileName	, 0, '\t');			
		::AfxExtractSubString( strKorName,		strFileName	, 2, '\t');			
		::AfxExtractSubString( strEngName,		strFileName	, 4, '\t');			
		::AfxExtractSubString( strUserIDNo,		strFileName	, 6, '\t');			
		::AfxExtractSubString( strPw,				strFileName	, 8, '\t');			
		::AfxExtractSubString( strUserLevel,	strFileName	, 10,'\t');			

		pData = new CUserAccount();			// 새로운 Data 생성

		// Data 할당
		pData->sID			= strUserID;
		pData->sKorName	= strKorName;
		pData->sEngName	= strEngName;
		pData->sIDNo		= strUserIDNo;	// _wtoi(strUserIDNo);		// 사원번호는 절대 숫자가 아니고, 문자로 취급해야 한다.
		pData->sPassword	= strPw;
		pData->nLevel		= _wtoi(strUserLevel);

		if( strUserID == _T("") || strUserID == _T("NONE")) // ID값이 이상하면 지운다.
		{
			delete pData;
		}
		else	// 정상적인 ID라면
		{
			m_mapUsers.SetAt(pData->sID, pData);		// Map에 추가한다.
		}
	}

	file.Close();
	fclose(fStream);

	return TRUE;
}

// 이름 가져오기.;
CString CMCMain::GetUserName(CString sID, int nLang)
{
	CString strName = _T("");

	CUserAccount*	pData = NULL;

	if ( m_mapUsers.Lookup(sID, pData) == TRUE )
	{
		if( pData != NULL )
		{
			// 반환시킬 언어를 가져온다;
			if(nLang == eLANG_KOREAN)
				strName = pData->sKorName;
			else if(nLang == eLANG_VIETNAM)
				strName = pData->sEngName;		// 베트남어도 지원해달라고 하면 그때 적용하도록!
			else
				strName = pData->sEngName;		// 기본값은 영어로;
		}
	}
	return strName;
}

// Level 가져오기
int CMCMain::GetUserLevel(CString sID, CString sPW)
{
	int nLevel = 0;
	CUserAccount*	pData = NULL;

	// 지정된 ID로 찾아본다
	if ( m_mapUsers.Lookup(sID, pData) == TRUE )
	{
		if ( pData != NULL )			// 정상적으로 사용자 계정을 찾았다.
		{
			if ( pData->sPassword == sPW )		// 제시한 암호가 올바르다면
			{
				nLevel = pData->nLevel;				// 지정된 Level를 취득한다.
			}
		}
	}

	return nLevel;
}

// 사원번호 가져오기
CString CMCMain::GetUserIDNo(CString sID)
{
	CString sIDNo = _T("");
	CUserAccount*	pData = NULL;

	if ( m_mapUsers.Lookup(sID, pData) == TRUE )
	{
		if ( pData != NULL )
		{
			sIDNo = pData->sIDNo;
		}
	}

	return sIDNo;
}


// 작업자가 있는지 확인!
BOOL CMCMain::UserFind(CString sID)
{
	if ( sID.IsEmpty() == TRUE ) return FALSE;

	CUserAccount*	pData = NULL;

	if ( m_mapUsers.Lookup(sID, pData) == TRUE )
	{
		return (pData != NULL);			// 찾았다.
	}

	return FALSE;	// 찾지 못했다.
}




// 종료시 계정 정보를 메모리 해제해 준다.
void CMCMain::ReleaseUserAccount()
{
	POSITION pos = NULL;
	CString key;
	CUserAccount*	pData = NULL;

	pos = m_mapUsers.GetStartPosition();
	while( pos != NULL ) 
	{
		pData = NULL;

		m_mapUsers.GetNextAssoc(pos, key, pData);			// Data를 인출한다.

		if ( pData != NULL )	
		{
			delete pData;
			pData = NULL;
		}
	}

	m_mapUsers.RemoveAll();										// 등록되어있는 모든 키를 삭제해 준다.
}



// 알람 정보를 가지고 있는다.
void CMCMain::LoadAlarmInfo()
{
	CString strPath;
	strPath.Format(_T("%s\\Data\\Alarm\\AlarmCode.ini"), m_sBasePath);
	
	if ( CGxUtility::IsExistFile( strPath ) == FALSE ) 
	{
		return;
	}

	AlarmInfoCode* pData =NULL;
	
	FILE *fStream;
	errno_t e = _tfopen_s(&fStream, strPath.GetString(), _T("r,ccs=UTF-8"));// UNICODE"));
	if (e != 0) return;
	CStdioFile		file(fStream);

	int nIdx1 =0;
	int nIdx2 =0;
	
	while( file.ReadString(strPath) )
	{
		nIdx1 = strPath.Find(_T("["));
		nIdx2 = strPath.Find(_T("]"));

		if(nIdx1 < 0 || nIdx2 < 0)
			continue;

		pData = new AlarmInfoCode;
		
		pData->m_strCode = strPath.Mid(nIdx1+1,nIdx2-1);
		
		m_ListAlarmInfoCode.AddTail(pData);		
				
	}//end of while

	file.Close();
	fclose(fStream);


}


// 알람 정보를 해지한다.
void CMCMain::RelaseAlarmInfo()
{
	POSITION pos = NULL;
	CString key;

	AlarmInfoCode*	pData = NULL;

	pos = m_ListAlarmInfoCode.GetHeadPosition();

	while( pos != NULL ) 
	{
		pData = NULL;
		pData = m_ListAlarmInfoCode.GetNext(pos);

		if ( pData != NULL )	
		{
			delete pData;
			pData = NULL;
		}
	}
	m_ListAlarmInfoCode.RemoveAll();										// 등록되어있는 모든 키를 삭제해 준다.
}



// User login -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
//
// 현재 사용자를 변경한다.
void CMCMain::ChangeCurrentUser(CString sID, int nLevel, BOOL bFixForm)
{
	CSvcTcpCIM*	pSvcTcpCIM = (CSvcTcpCIM*)(CGxServiceHub::GetInstance()->GetService( NAME_SERVICE_TcpCIM ));		// Data PC CIM 연결을 위한 TCP/IP 통신
	CGxLogMgr*	pLogMgr = CGxLogMgr::GetInstance();
	CGxLog*	pLogUI = pLogMgr->GetLogByName( NAME_LOG_UIOperate );									// 화면에서 발생하는 주요 Event 기록

	// 현재 접속된 작업자가 존재하면,
	if ( !m_rCurrentUser.sID.IsEmpty() )
	{
		pSvcTcpCIM->CIM_UserLogout();									// 작업자 로그아웃 보고

		pLogUI->Write(_T("User Logout [%s, %s]"), m_rCurrentUser.sID, m_rCurrentUser.sIDNo);
	}

	// 2015-04-22, 닉네임 = 사번 으로 통일되었다.
	m_rCurrentUser.sID		= sID;
	m_rCurrentUser.nLevel	= nLevel;
	m_rCurrentUser.sIDNo	= sID;				// GetUserIDNo( sID );

	//2015/04/16 JH-PARK
	//로그인 권한에 대한 상위요청이 승인될경우 이름을 ID로 표기한다
	if ( m_rProfileData.bUse_LoginServerRequest == TRUE ) 
	{
		m_rCurrentUser.sKorName	= sID;
		m_rCurrentUser.sEngName	= sID;
	}
	else
	{
		m_rCurrentUser.sKorName	= GetUserName( sID, eLANG_KOREAN);
		m_rCurrentUser.sEngName	= GetUserName( sID, eLANG_ENGLISH);
	}


	::GetLocalTime( &( m_rRuntimeData.tmUserLogin ) );					// Event가 발생한 시각

	//2014-10-13, jhLee, 기존 작업자 ID -> 작업자 사번 으로 변경, 백윤기S 요청
	pSvcTcpCIM->CIM_UserLogin( m_rCurrentUser.sID );						// 작업자 로그인 보고, m_rCurrentUser.sIDNo

	pLogUI->Write(_T("User Login [%s, %s, %s]"), m_rCurrentUser.sID, m_rCurrentUser.sIDNo, m_rCurrentUser.sKorName );	// 사번으로 변경 GetUserName(sID, eLANG_KOREAN));


	// 화면에 적용한다.
	// MainFrame에게 사용자가 변경되었음을 통보한다.
	::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, UDMSG_USER_CHANGE, nLevel, bFixForm);

	SaveRuntimeData();						// 실행 데이터를 저장
}


// Logout 시킨다.
//
// [in] BOOL bHide : 화면변경없이 데이터 처리만 할 것인가 ?
//		TRUE : 화면을 변경하지 않고 Logout 처리
//		FALSE : 화면을 Main 화면으로 강제 변경한다.
void CMCMain::UserLogout(BOOL bHide)
{
	CSvcTcpCIM*	pSvcTcpCIM = (CSvcTcpCIM*)(m_pGausFrame->GetService( NAME_SERVICE_TcpCIM ));		// Data PC CIM 연결을 위한 TCP/IP 통신
	CGxLogMgr*	pLogMgr = CGxLogMgr::GetInstance();
	CGxLog*	pLogUI = pLogMgr->GetLogByName( NAME_LOG_UIOperate );									// 화면에서 발생하는 주요 Event 기록

	pSvcTcpCIM->CIM_UserLogout();									// 작업자 로그아웃 보고
	pLogUI->Write(_T("User Logout [%s, %s]"), m_rCurrentUser.sID, m_rCurrentUser.sIDNo);


	// 초기화
	m_rCurrentUser.nLevel = eUSER_NONE;
	m_rCurrentUser.sID.Empty();					// 작업자가 없다.
	m_rCurrentUser.sIDNo.Empty();
	m_rCurrentUser.sKorName.Empty();
	m_rCurrentUser.sEngName.Empty();

	::GetLocalTime( &( m_rRuntimeData.tmUserLogin ) );					// Event가 발생한 시각

	// 화면에 적용한다.
	::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, UDMSG_USER_CHANGE, eUSER_NONE, bHide);

	SaveRuntimeData();						// 실행 데이터를 저장
}




// 현재 로그인된 작업자 레벨
int CMCMain::GetCurrentUserLevel()
{
	return m_rCurrentUser.nLevel;				// 현재 접속된 작업자 레벨 정보
}

// 현재 로그인된 작업자 ID
CString CMCMain::GetCurrentUserID()
{
	return m_rCurrentUser.sID;
}


// 현재 접속된 사용자 사번 조회
CString CMCMain::GetCurrentUserIDNo()
{
	return m_rCurrentUser.sIDNo;
}

// 현재 접속된 사용자 이름 조회
CString CMCMain::GetCurrentUserName()
{
	// 설정된 언어마다 다른 이름 리턴 ?

	return m_rCurrentUser.sKorName;
}


// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==

// AutoForm에 보여줄 Align 및 MCR 실행 이미지 파일이름을 지정하고 조회한다.

// 이미지 이름 지정
void CMCMain::SetViewImgFilename(int nCh, int nIdx, CString sFile)
{
	if ( (nCh >= 0) && (nCh < eMAX_ChCount) && (nIdx >= 0) && (nIdx < eMAX_ViewImgCount) )
	{
		m_sAutoViewImg[nCh][nIdx] = sFile;
	}
}

// 이미지 이름 조회
CString CMCMain::GetViewImgFilename(int nCh, int nIdx)
{
	if ( (nCh >= 0) && (nCh < eMAX_ChCount) && (nIdx >= 0) && (nIdx < eMAX_ViewImgCount) )
	{
		return m_sAutoViewImg[nCh][nIdx];
	}

	return _T("");
}


// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==

//	MEMORYSTATUS m_memoryStatus;				// 메모리 사용량
// 메모리 사용량 기록
CString CMCMain::GetMemoryCheck()
{

	// void WINAPI GlobalMemoryStatus(LPMEMORYSTATUS lpBuffer);
	// MEMORYSTATUS는 아래와 같은 구조를 갖게 됩니다.
	
	//DWORD
	// dwLength
	// 32 고정
 //
	//DWORD
	// dwMemoryLoad
	// 사용할수 있는 메모리비율
 //
	//DWORD
	// dwTotalPhys
	// 물리적 메모리양
 //
	//DWORD
	// dwAvailPhys
	// 물리적 사용가능한 메모리양
 //
	//DWORD
	// dwTotalPageFile
	// 가상 메모리양
 //
	//DWORD
	// dwAvailPageFile
	// 가상 사용가능한 메모리양
 //
	//DWORD
	// dwTotalVirtual
	// 전체(물리,가상) 메모리양
 //
	//DWORD
	// dwAvailVirtual
	// 전체(물리,가상) 사용가능한 메모리양
 

	::GlobalMemoryStatus( &m_memoryStatus);

	CString sMemoryInfo;
	sMemoryInfo.Format( _T("[사용가능 메모리 : %u Bytes (%u %%)]"), m_memoryStatus.dwAvailVirtual, m_memoryStatus.dwMemoryLoad );

	return sMemoryInfo;
}


// 메모리 사용량 기록
void CMCMain::WriteMemoryCheck()
{
//	CString sMsg;
	::GlobalMemoryStatus( &m_memoryStatus);

//	sMsg.Format(_T("[MEM] AvlLoad:%u%%, AvlPhys:%u, AvlPage:%u, TotalAvl:%u"), m_memoryStatus.dwMemoryLoad, m_memoryStatus.dwAvailPhys, m_memoryStatus.dwAvailPageFile,
//		m_memoryStatus.dwAvailVirtual);

	m_pLogSys->Write(_T("[MEM] AvlLoad:%u%%, AvlPhys:%u, AvlPage:%u, TotalAvl:%u"), m_memoryStatus.dwMemoryLoad, m_memoryStatus.dwAvailPhys, m_memoryStatus.dwAvailPageFile,
						m_memoryStatus.dwAvailVirtual);
	
}


//
// 검사를 진행하지 못한 Defect이 발생하였다면,
// 'RETRY'를 표시해야하는 VI 결과값인가 ?
BOOL CMCMain::GetVIRetryDefect(CString sResult)
{
	return CGxUtility::IsIn_s( 9,							// 비교 수량은 8개
							sResult,					// 불량명으로 비교		
							_T("Inspection Fail"), 
							_T("Display불량"),	
							_T("No Panel"), 
							_T("Block Defect"),
							_T("불량 다발"),			// 2016-05-26, SDC 이정현 선임 요청으로 추가

							_T("NO FOLDER"),				// Vision 검사 결과 파일을 취득하지 못한 경우에도 재검을 실시한다.
							_T("NO FILE1"),
							_T("NO FILE2"),
							_T("NO DATA") );
}


// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==

// 정면 버튼들의 동작을 활성화/비활성화 제어한다.
BOOL CMCMain::GetPanelButtonLock()
{
	return m_rProfileData.bPanelButtonLock;
}


// TRUE:전면부 버튼 비활성화, FALSE:버튼 활성화
void CMCMain::SetPanelButtonLock(BOOL bFlag)
{
	if ( m_rProfileData.bPanelButtonLock != bFlag )
	{
		m_rProfileData.bPanelButtonLock = bFlag;

		if ( bFlag == TRUE )
		{
			m_pLogSeq->Write( _T("[PA] Panel Button Lock") );	
		}
		else
			m_pLogSeq->Write( _T("[PA] Panel Button Free") );	
	}
}


// 작업 진행 관련
// D-Zone 진입으로 작업자의 Defect 입력하는 모드이다.
void CMCMain::SetDefectInputMode(BOOL bFlag)
{
	m_rProfileData.bDZoneDefectInputFlag = bFlag;		

	// AutoForm 화면에 변경된 정보를 갱신요청한다.
	::PostMessage(m_hOwner, UDMSG_DISPDATA_UPDATE, eIDX_UPDATE_UI, eIDX_AUTO_UI_Defect);	// 상태 변경
}

// D-Zone 진입으로 작업자의 Defect 입력하는 모드인지 조회한다.
BOOL CMCMain::GetDefectInputMode()
{
	return m_rProfileData.bDZoneDefectInputFlag;
}


// A-Zone 초기 Loading 작업시 Cell 없음/미사용 설정용 Dialog 가능/불가능 설정
void CMCMain::SetCellInfoEnabled(BOOL bFlag)
{
	m_rProfileData.bAZoneCellInfoEnabled = bFlag;

	// AutoForm 화면에 변경된 정보를 갱신요청한다.
	::PostMessage(m_hOwner, UDMSG_DISPDATA_UPDATE, eIDX_UPDATE_UI, eIDX_AUTO_UI_CellInfo);	// 상태 변경
}

// A-Zone 초기 Loading 작업시 Cell 없음/미사용 설정용 Dialog 가능/불가능 여부 조회
BOOL CMCMain::GetCellInfoEnabled()
{
	return m_rProfileData.bAZoneCellInfoEnabled;
}



// 삭제
////2015-03-30, jhLee, C-Zone 자동 Contact Off 처리 조건
////
//// 지정 JIG의 제품들을 C-Zone에서 미리 Contact을 OFF 해도 되는 조건인가 ?
//// return	TRUE : 자동 Contact Off 가능
////			FALSE : 불가능
////
//BOOL CMCMain::GetIsCZoneAutoContactOff(int nJigNo)
//{
//	if ( (nJigNo < 0) || (nJigNo >= eMAX_JigCount) ) return FALSE;			// 인수 점검
//
//	// Cell이 하나도 존재하지 않는다면, FALSE
//	if ( m_pCellInfo->GetCellExist(nJigNo) < 0 ) return FALSE;				// Cell 존재 여부 점검
//
//	int i;
//	TCellData* pCell = NULL;
//	BOOL bAutoJudge = TRUE;				// 초기값은 자동으로 Contact이 풀릴 수 있다고 가정
//
//	int nClass;
//	CString sDefect, sDefectCode;
//
//
//	for (i = 0; i < eMAX_ChCount; i++)									// 모든 채널에 대하여
//	{
//		pCell	= m_pCellInfo->GetCellData(nJigNo, i);					// 지정 채널의 데이터 포인터 획득
//
//		if ( pCell == NULL ) return FALSE;								// 데이터 획득 실패시 
//
//		if ( pCell->bIsExist == TRUE )									// 제품이 존재하고
//		{
//			// C-Zone Vision 검사를 수행하지 않거나, 수행했더라도 아직 결과를 기다리는 중이다.
//			if ( (pCell->bVTProcessFlag == FALSE) || (pCell->bWaitVTResult == TRUE) )							
//			{
//				bAutoJudge = FALSE;										// 자동 판정은 하지 못한다.
//				break;
//			}
//
//			// 판정을 살펴본다.
//			// 만약 지금까지도 D-Zone에서 판정이 되지 않았다면 Good으로 지정한다.
//			//if ( (pCell->DZoneManualClass == NONE_CELL) && (pCell->DZoneManualInspState == INSP_NONE) )
//			//{
//			//	pCell->DZoneManualClass = GOOD_CELL;
//			//	pCell->DZoneManualState = INSP_END;
//			//	pCell->DZoneManualDefect = TEXT_GOOD;
//			//	pCell->DZoneManualCode	= _T("");
//			//}
//
//			GetDefectFromJudge(ZONE_A, pCell, pCell->AZoneDefect, pCell->AZoneClass, sDefectCode);
//			GetDefectFromJudge(ZONE_B, pCell, pCell->BZoneDefect, pCell->BZoneClass, sDefectCode);		// B-Zone은 존재하지 않으나 실시,
//			GetDefectFromJudge(ZONE_C, pCell, pCell->CZoneDefect, pCell->CZoneClass, sDefectCode);
//
////			GetDefectFromJudge(ZONE_D, pCell, pCell->DZoneDefect, pCell->DZoneClass, sDefectCode);
//			// D-Zone을 제외한 최종 판정
//			pCell->FinalJudge	= GetDefectFromJudge(ZONE_MAX, pCell, sDefect, nClass, sDefectCode);
//			pCell->FinalDefect	= sDefect;
//			pCell->MesCode		= sDefectCode;			// CIM 보고용 코드
//
//
//			// 만약 지금까지도 D-Zone에서 판정이 되지 않았다면 Good으로 지정한다.
//			if ( (pCell->DZoneManualClass == NONE_CELL) && (pCell->DZoneManualState == INSP_NONE) )
//			{
//				pCell->DZoneManualClass = GOOD_CELL;
//				pCell->DZoneManualState = INSP_END;
//				pCell->DZoneManualDefect = TEXT_GOOD;
//				pCell->DZoneManualCode	= _T("");
//			}
//
//	
//			// nJudge	= GetDefectFromJudge(ZONE_MAX, pCell, sDefect, nClass, sDefectCode);
//
//			if ( pCell->FinalJudge != CELL_LOG_OK )								// 현재까지의 최종 판정이 양품이 아니다.
//			{
//				bAutoJudge = FALSE;										// 자동 판정은 하지 못한다.
//				break;													// 더이상의 비교는 무의미하다.
//			}
//		}//of if Cell Exist
//	} //of for i
//
//
//	return bAutoJudge;
//}
//



//////////////////////////////////////////////////////////////////////////////////////////////////////////////


//
// 검사PC에서 해상도 관련 데이터를 읽어온다.
BOOL CMCMain::LoadVIInformFile()
{
	CString strFileName;
	strFileName.Format(_T("%s\\Inform.ini") , m_rProfileData.sDIR_VIInform);			// 검사PC에서 읽어 올 파일

	// 파일이 존재하는지 점검
	if ( CGxUtility::IsExistFile( strFileName ) == FALSE ) 
	{
		m_pLogSeq->Write(_T("[MAIN] VI Inform file read ERROR"));
		return FALSE;
	}

	CGxIniFile iniInform(strFileName);

	m_rCellDisp.sModelID		= iniInform.GetString(_T("MODEL INFO"), _T("MODEL ID"), _T(""));
	m_rCellDisp.nResolutionX	= iniInform.GetLong(_T("MODEL INFO"), _T("X Resolution"), 1);
	m_rCellDisp.nResolutionY	= iniInform.GetLong(_T("MODEL INFO"), _T("Y Resolution"), 1);
	m_rCellDisp.nDotCount		= iniInform.GetLong(_T("MODEL INFO"), _T("ETC::Dot Count"), 11);

	// 2014-11-27, jhLee, Vision PC에서 보내온 정보로 3으로 나누면 정확하 위치가 나오지 않아서 2로 나누어 사용한다.
	//  Teaching1 form에서 설정값으로 DotCountX와 Y를 지정하도록 변경
	//
	//	m_rCellDisp.nDotCountX = m_rCellDisp.nDotCount / 10;
	//	m_rCellDisp.nDotCountY = m_rCellDisp.nDotCount % 10;

	if (m_rCellDisp.nDotCount == 42)	// 펜타일 타입		(42)
	{
		m_rCellDisp.nDotSizeX = 2;
		m_rCellDisp.nDotSizeY = 2;
	}
	else			// 스트라이프 타입	(31)
	{
		m_rCellDisp.nDotSizeX = 1;
		m_rCellDisp.nDotSizeY = 1;
	}

	// 해당 데이터를 읽었다면, Recipe 파일을 저장한다.
	SaveRecipeData(_T("CellDisp"));								// 현재 Recipe 파일중 현재 편집중인 항목의 데이터를 저장한다.

	m_pLogSeq->Write(_T("[MAIN] VI Inform file read ok, [%s] x:%d, y:%d, Dot:%d"), m_rCellDisp.sModelID, m_rCellDisp.nResolutionX, m_rCellDisp.nResolutionY, m_rCellDisp.nDotCount);
	return TRUE;

// 	m_nDotSizeX = theParamMgr.m_Rcp.CELL.m_nResolutionX/nResolutionX;
// 	m_nDotSizeY = theParamMgr.m_Rcp.CELL.m_nResolutionY/nResolutionY;

/*
  Inform.ini 예

[MODEL INFO]
MODEL ID=A2-AMB184BW01
X Resolution=128
Y Resolution=432
Horizontal Size=13.248
Vertical Size=44.712
ETC::Dot Count=31
CF Array=Stripe
Cell Origin=3
[SPEC Validation]
HIGH_A_R_DOT=0
HIGH_A_G_DOT=0
HIGH_A_B_DOT=0
HIGH_A_T_DOT=0
HIGH_B_R_DOT=0
HIGH_B_G_DOT=0
HIGH_B_B_DOT=0
HIGH_B_T_DOT=0
OFF_A_R_DOT=0
OFF_A_G_DOT=0
OFF_A_B_DOT=0
OFF_A_T_DOT=0
OFF_B_R_DOT=0
OFF_B_G_DOT=0
OFF_B_B_DOT=0
OFF_B_T_DOT=0
*/

}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 판정 관련 각 Zone별 대표 불량을 판정하여 획득한다.
//
// [ 각 Zone별 판정 우선순위 정의 ]
// 일시 : 2015-03-10, 17:30
// 장소 : SDBN 모듈동 3층 사무실
// 참석자 : 김상묵 사원, 백윤기 사원, 이재현 차장
//
//1. 최종 Last class 판정 우선순위는 A Zone -> D Zone -> C Zone -> 신호기Error 순으로 결정한다.
//
//2. A-Zone에서 신호기 불량 (Contact Current NG, Cell Loading NG, SYSTEM ERROR(신호기 알람)) 이 발생하면 
//  2-1. 최종 결과는 "RETEST"로 결정된다. (A-Zone이 우선순위로 지정되기 때문에, 만약 D-Zone에서 다른 Defect을 입력하더라도 무시된다)
//  2-2. C-Zone 검사는 "SKIP" 으로 정한다 (검사 진행을 하지 않는다)
//
//3. A-Zone에서 OI로 작업자가 Defect을 입력하였더라도, C-Zone 검사를 진행하는 조건
//  3-1. 신호기불량(Contact Current NG, Cell Loading NG, SYSTEM ERROR(신호기 알람))이 발생하지 않았다.
//  3-2. Option에서 C-Zone 강제 검사 모드가 ON 으로 설정되었다
//  3-3. 이때 진행하는 C-Zone 검사는 해당 Cell의 영상을 남기기 위한 행위이며, 
//       이미 A-Zone에서 Defect이 입력되었으므로 이 검사의 결과는 우선순위에서 밀려 무시된다. 
//
//4. A-Zone에서 Defect을 입력한 후에는 D-Zone에서 어떠한 판정을 내리더라도 우선순위에서 밀려 무시된다.
// 
int CMCMain::GetDefectFromJudge_old( int nZone, TCellData* pCell, CString &strDefect, int &nClass, CString &sDefectCode )
{
	int nJudgeIndex = CONST_JUDGE_NAME::JUDGE_MCR;
	sDefectCode = _T("");

	CString sKey, sMESCode, sMESName;		// MES 보고용 Defect code와 명칭

	switch ( nZone)
	{
		case eIDX_PG_ZONE_A:	// A-Zone의 대표 불량

			// 작업자 OI 입력이 최우선이다
			if ( pCell->AZoneManualDefect != _T("") )		// 수동검사가 판정한 불량명이 존재한다면,
			{
				strDefect	= pCell->AZoneManualDefect;
				nClass		= pCell->AZoneManualClass;
				nJudgeIndex = CONST_JUDGE_NAME::JUDGE_MANUAL_A;
				sDefectCode	= pCell->AZoneManualCode;	
			}
			else if ( pCell->PG_DownZone == eIDX_PG_ZONE_A )			// 신호기 Down여부
			{
				if ( pCell->PG_VoltageAlarm == TRUE )
				{
					strDefect = _T("PG VOLTAGE");	//TEXT_VOLTAGE_ALRAM;
				}
				else if ( pCell->PG_CurrentAlarm == TRUE )
				{
					strDefect = _T("PG CURRENT");	// CURRENT_ALRAM;
				}
				else if ( pCell->PG_SystemError == TRUE )
				{
					strDefect = _T("PG SYSTEM");	// PG_SYSTEM_ERROR;
				}
				else
				{
					strDefect = TEXT_CHANNEL_DOWN;
				}
			
				nClass		= eBIN2_CELL;						// old CELL_LOG_NG;
				nJudgeIndex = CONST_JUDGE_NAME::JUDGE_PG_ALARM;
				sDefectCode	= _T("");								// 보고용 Defect Code
			}
			else if ( pCell->ContactCurrentClass != eGOOD_CELL )			// Contact Current NG
			{
				strDefect	= TEXT_CONTACT_CURRENT_NG;				// Defect명은 Contact Current NG
				nClass		= eBIN2_CELL;	// CELL_LOG_BIN2;						// old (pCell->ContactCurrentClass == GOOD_CELL) ? CELL_LOG_OK:CELL_LOG_NG;
				nJudgeIndex = CONST_JUDGE_NAME::JUDGE_CONTACT;
				sDefectCode	= _T("");								// 보고용 Defect Code
			}
			else if ( pCell->CellLoadingClass != eGOOD_CELL )		// Cell Loading NG
			{
				strDefect	= TEXT_CELL_LOADING_NG;					// Defect명은 Cell Loading NG 
				nClass		= eBIN2_CELL;	// CELL_LOG_BIN2;						// old (pCell->CellLoadingClass == GOOD_CELL) ? CELL_LOG_OK:CELL_LOG_NG;
				nJudgeIndex = CONST_JUDGE_NAME::JUDGE_CELL_LOADING;
				sDefectCode	= _T("");								// 보고용 Defect Code
			}
			else // 나머지는 양품 
			{
				strDefect	= TEXT_GOOD;
				nClass		= eGOOD_CELL;	// CELL_LOG_OK;
				nJudgeIndex = CONST_JUDGE_NAME::JUDGE_MCR;
				sDefectCode	= _T("");								// 보고용 Defect Code
			}
			break;

	
		// 본 설비에서는 ZONE_B이 존재하지 않는다.
		case eIDX_PG_ZONE_B:		// 무조건 양품
				strDefect	= TEXT_GOOD;
				nClass		= eGOOD_CELL;	// CELL_LOG_OK;
				nJudgeIndex = CONST_JUDGE_NAME::JUDGE_MCR;
				sDefectCode	= _T("");			// 보고용 Defect Code
			break;



		case eIDX_PG_ZONE_C: 
			// 
			//old 우선순위 TSP > VT [7/29/2013 OSC]
			//old 우선순위 ID_CHECK > White > Manual [7/29/2013 OSC]

			// 2016-04-16, jhLee, TSP 추가로 우선순위 변경
			// 2015-03-31, jhLee, TE_CHECK가 C-Zone에 추가되면서 우선순위에도 변화가 발생


			// TE_CHECK 불량 발생점검, Option 지정 및 검사 진행과 결과 확인
			if ( m_rProfileData.bUseTest_TECheck && (pCell->TE_CheckInspState == eINSP_END) && (pCell->TE_CheckClass == eLOSS_CELL) )		
			{
				strDefect	= TEXT_TE_CHECK_NG;					// TE_CHECK_NG
				nClass		= eLOSS_CELL;							// CELL_LOG_NG;
				nJudgeIndex = CONST_JUDGE_NAME::JUDGE_VISION;

				// NG Mapping Table에서 대표 코드를 찾는다.
				if ( GetDefectNameCode( strDefect, sMESName, sMESCode ) )
				{
					sDefectCode		= sMESCode;						// 수동검사 Defect Code, CIM 보고용
					strDefect		= sMESName;						// 라벨 출력용 디펙명
				}
				else	// Default Code로 대입한다.
				{
					sDefectCode	= _T("UST14");							// 보고용 Defect Code
				}
			}
			// TSP 검사 결과
			else if( m_rProfileData.bUseTest_TSP && (pCell->TspInspState == eINSP_END) && (pCell->TspClass != eGOOD_CELL) )	
			{
				strDefect	= pCell->TspNGMainName;
				nClass		= eLOSS_CELL;							// CELL_LOG_NG;
				nJudgeIndex = CONST_JUDGE_NAME::JUDGE_TSP;

				// NG Mapping Table에서 대표 코드를 찾는다.
				if ( GetDefectNameCode( strDefect, sMESName, sMESCode ) )
				{
					sDefectCode		= sMESCode;						// 수동검사 Defect Code, CIM 보고용
					strDefect		= sMESName;						// 라벨 출력용 디펙명
				}
				else	// Default Code로 대입한다.
				{
					sDefectCode	= _T("UST14");							// 보고용 Defect Code
				}
			}
			// Vision 검사결과 불량 판정
			else if( (pCell->VTClass == eLOSS_CELL) )	// && (m_rProfileData.bUseTest_VI == TRUE))		// C-Zone에서 Vistion 검사를 진행할 것인지 여부
			{
				strDefect	= pCell->VTResult;
				nClass		= eLOSS_CELL;								// CELL_LOG_NG;
				nJudgeIndex = CONST_JUDGE_NAME::JUDGE_VISION;
				sDefectCode	= pCell->VTCode;								// 보고용 Defect Code
			}
			// Vision 검사결과 재검발생
			else if( (pCell->VTClass == eBIN2_CELL) )	// && (m_rProfileData.bUseTest_VI == TRUE))		// C-Zone에서 Vistion 검사를 진행할 것인지 여부
			{
				strDefect	= pCell->VTResult;
				nClass		= eBIN2_CELL;									// CELL_LOG_BIN2;
				nJudgeIndex = CONST_JUDGE_NAME::JUDGE_VISION;
				sDefectCode	= pCell->VTCode;			// 보고용 Defect Code
			}
			// 우선순위에 의해 신호기는 가장 마지막으로 위치변경하였다.
			else if (pCell->PG_DownZone == eIDX_PG_ZONE_C)
			{
				if ( pCell->PG_VoltageAlarm == TRUE )
				{
					strDefect = _T("PG VOLTAGE");	//TEXT_VOLTAGE_ALRAM;
				}
				else if ( pCell->PG_CurrentAlarm == TRUE )
				{
					strDefect = _T("PG CURRENT");	// CURRENT_ALRAM;
				}
				else if ( pCell->PG_SystemError == TRUE )
				{
					strDefect = _T("PG SYSTEM");	// PG_SYSTEM_ERROR;
				}
				else
				{
					strDefect = TEXT_CHANNEL_DOWN;
				}
			
				nClass		= eBIN2_CELL;										// CELL_LOG_BIN2;						// old CELL_LOG_NG;
				nJudgeIndex = CONST_JUDGE_NAME::JUDGE_PG_ALARM;
				sDefectCode	= _T("");			// 보고용 Defect Code
			}
			else	// 별다른 Defect이 없다면 양품
			{
				strDefect	= TEXT_GOOD;
				nClass		= eGOOD_CELL;										// CELL_LOG_OK;
				nJudgeIndex = CONST_JUDGE_NAME::JUDGE_MCR;
				sDefectCode	= _T("");			// 보고용 Defect Code
			}

			break;


		case eIDX_PG_ZONE_D: 
			// OLD
			// 우선순위 Sleep > Manual [7/29/2013 OSC]
			//else if( (pCell->SleepCurrentClass == LOSS_CELL) && (m_rProfileData.bUseTest_Sleep == TRUE))		// D-Zone에서 Sleep Current 검사를 진행할 것인지 여부
			//{
			//	strDefect	= TEXT_SLEEP_NG;
			//	nClass		= CELL_LOG_NG;
			//	nJudgeIndex = CONST_JUDGE_NAME::JUDGE_SLEEP;
			//}
			//

			// 작업자가 재검을 선택하였다면 최우선으로 지정한다.
			if( m_rProfileData.bUseTest_Manual && (pCell->DZoneManualInspState == eINSP_END) && (pCell->DZoneManualClass == eBIN2_CELL)  )		
			{
				strDefect	= TEXT_RETEST;				// RESET
				nClass		= eBIN2_CELL;
				nJudgeIndex = CONST_JUDGE_NAME::JUDGE_MANUAL_D;
				sDefectCode	=_T("");
			}
			// D-Zone에서 불량을  선택하였다.
			else if( m_rProfileData.bUseTest_Manual && (pCell->DZoneManualInspState == eINSP_END) && (pCell->DZoneManualClass == eLOSS_CELL) )		// D-Zone에서 작업자 수동 검사를 진행할 것인지 여부
			{
				strDefect	= pCell->DZoneManualDefect;
				nClass		= eLOSS_CELL;									// CELL_LOG_NG;
				nJudgeIndex = CONST_JUDGE_NAME::JUDGE_MANUAL_D;
				sDefectCode	= pCell->DZoneManualCode;					// 지정한 Defect Code 사용
			}
			// D-Zone에서 작업자가 재검 ('RETEST')를 선택하였나 ?
		// 신호기 관련 Error 우선 순위에서 가장 마지막으로 위치 이동 시켰다.
			else if( pCell->PG_DownZone == eIDX_PG_ZONE_D )			
			{
				if ( pCell->PG_VoltageAlarm == TRUE )
				{
					strDefect = _T("PG VOLTAGE");	//TEXT_VOLTAGE_ALRAM;
				}
				else if ( pCell->PG_CurrentAlarm == TRUE )
				{
					strDefect = _T("PG CURRENT");	// CURRENT_ALRAM;
				}
				else if ( pCell->PG_SystemError == TRUE )
				{
					strDefect = _T("PG SYSTEM");	// PG_SYSTEM_ERROR;
				}
				else
				{
					strDefect = TEXT_CHANNEL_DOWN;
				}
			
				nClass		= eBIN2_CELL;											// CELL_LOG_BIN2;						// old CELL_LOG_NG;
				nJudgeIndex = CONST_JUDGE_NAME::JUDGE_PG_ALARM;
				sDefectCode	= _T("");
			}
			else	// 그 밖은 양품을 선택하였다.
			{
				strDefect	= TEXT_GOOD;
				nClass		= eGOOD_CELL;
				nJudgeIndex = CONST_JUDGE_NAME::JUDGE_MCR;
				sDefectCode	= _T("");
			}
			break;



		// 최종 판정
		case eIDX_PG_ZONE_Final: 

			// 2016-05-12, jhLee, TSP 검사를 추가하면서 판정 우선순위가 변경되었다.

			// 아래내용은 이전 버전, 참고용으로만 사용
			// 먼저 검사한 순서대로 우선순위 MCR > Contact > TSP > ID_CHECK > White > Manual C > Sleep > Manual D > VT [7/29/2013 OSC]
			// D-Zone Defect이 없는 경우 : PG-Down > Contact > A-Zone Manual > VT
			// D-Zone Defect이 있는 경우 : PG-Down > Contact > D-Zone Manual
			// D-Zone Defect이 GOOD인 경우 : PG-Down > Contact > GOOD
			// 
			// Zone별 우선순위 : A Zone -> D Zone -> C Zone 순으로 판정한다. : 김상묵 사원 (2015-03-10 베트남에서)
			// 1. A-Zone Manual 검사가 Defect을 강제 지정하였다.
			// if( (pCell->AZoneManualClass == LOSS_CELL) )	


			// 1. D-Zone Manual 검사가 BIN2 (RETEST)를 강제 지정하였다.
			if( m_rProfileData.bUseTest_Manual && (pCell->DZoneManualInspState == eINSP_END) && (pCell->DZoneManualClass == eBIN2_CELL) )
			{
				strDefect	= TEXT_RETEST;			
				nClass		= eBIN2_CELL;
				nJudgeIndex = CONST_JUDGE_NAME::JUDGE_MANUAL_D;
				sDefectCode	= _T("");					
			}
			// 2. A-Zone에서 작업자가 Defect을 입력하였다.
			else if( (pCell->AZoneClass == eLOSS_CELL) )	
			{
				strDefect	= pCell->AZoneDefect;					//old ResultAZoneManualName;
				nClass		= eLOSS_CELL;
				nJudgeIndex = CONST_JUDGE_NAME::JUDGE_MANUAL_A;
				sDefectCode	= pCell->AZoneCode;						
			}
			// 3. D-Zone Manual 검사가 Defect을 강제 지정하였다.
			else if( m_rProfileData.bUseTest_Manual && (pCell->DZoneManualInspState == eINSP_END) && (pCell->DZoneManualClass == eLOSS_CELL) )
			{
				strDefect	= pCell->DZoneManualDefect;
				nClass		= eLOSS_CELL;
				nJudgeIndex = CONST_JUDGE_NAME::JUDGE_MANUAL_D;
				sDefectCode	= pCell->DZoneManualCode;						// CIM 보고용 Defect Code
			}
			// 4. TE_CHECK 불량 발생점검, Option 지정 및 검사 진행과 결과 확인
			else if ( m_rProfileData.bUseTest_TECheck && (pCell->TE_CheckInspState == eINSP_END) && (pCell->TE_CheckClass == eLOSS_CELL) )		
			{
				strDefect	= TEXT_TE_CHECK_NG;					// TE_CHECK_NG
				nClass		= eLOSS_CELL;
				nJudgeIndex = CONST_JUDGE_NAME::JUDGE_VISION;

				// NG Mapping Table에서 대표 코드를 찾는다.
				if ( GetDefectNameCode( strDefect, sMESName, sMESCode ) )
				{
					sDefectCode		= sMESCode;						// 수동검사 Defect Code, CIM 보고용
					strDefect		= sMESName;						// 라벨 출력용 디펙명
				}
				else	// Default Code로 대입한다.
				{
					sDefectCode	= _T("UST14");							// 보고용 Defect Code
				}
			}
			// 5. TSP 검사 결과
			else if( m_rProfileData.bUseTest_TSP && (pCell->TspInspState == eINSP_END) && (pCell->TspClass != eGOOD_CELL) )	
			{
				strDefect	= pCell->TspNGMainName;
				nClass		= eLOSS_CELL;
				nJudgeIndex = CONST_JUDGE_NAME::JUDGE_TSP;

				// NG Mapping Table에서 대표 코드를 찾는다.
				if ( GetDefectNameCode( strDefect, sMESName, sMESCode ) )
				{
					sDefectCode		= sMESCode;						// 수동검사 Defect Code, CIM 보고용
					strDefect		= sMESName;						// 라벨 출력용 디펙명
				}
				else	// Default Code로 대입한다.
				{
					sDefectCode	= _T("UST14");							// 보고용 Defect Code
				}
			}
			// 6. D-Zone Manual 검사가 양품을 강제 지정하였다.
			else if( m_rProfileData.bUseTest_Manual && (pCell->DZoneManualInspState == eINSP_END) && (pCell->DZoneManualClass == eGOOD_CELL) )
			{
				// 강제 양품 처리
				strDefect	= TEXT_GOOD;
				nClass		= eGOOD_CELL;
				nJudgeIndex = CONST_JUDGE_NAME::JUDGE_MCR;
				sDefectCode	= _T("");
			}
			//// 6. C-Zone VI 검사 결과
			// 2014-10-22, jhLee, C-Zone VT 검사가 양품이 아니라면 해당 불량명을 되돌려준다.
			else if( (pCell->VTClass != eGOOD_CELL) )	// && (pCell->Bin2Defect == FALSE) )	// && (m_rProfileData.bUseTest_VI == TRUE))		// C-Zone에서 Vistion 검사를 진행할 것인지 여부
			{
				strDefect	= pCell->VTResult;
				nClass		= pCell->VTClass;
				nJudgeIndex = CONST_JUDGE_NAME::JUDGE_VISION;
				sDefectCode	= pCell->VTCode;						
			}
			//
			// 이후로는 신호기 Error 관련 내용이다.
			else if( pCell->PG_DownError == TRUE )			// 신호기 System error
			{
				strDefect	= TEXT_RETEST;								// PG error 관련 메세지 대신에 최종결과에는 RETEST는 넣는다., 김상묵사원, 2015-03-11, TEXT_CHANNEL_DOWN;
				nClass		= eBIN2_CELL;
				nJudgeIndex = CONST_JUDGE_NAME::JUDGE_PG_ALARM;
				sDefectCode	= _T("");
			}
			else if ( pCell->PG_SystemError == TRUE )		
			{
				strDefect	= TEXT_RETEST;								// PG_SYSTEM_ERROR;
				nClass		= eBIN2_CELL;
				nJudgeIndex = CONST_JUDGE_NAME::JUDGE_PG_ALARM;
				sDefectCode	= _T("");
			}
			else if ( pCell->PG_CurrentAlarm == TRUE )
			{
				strDefect	= TEXT_RETEST;								// CURRENT_ALRAM;
				nClass		= eBIN2_CELL;
				nJudgeIndex = CONST_JUDGE_NAME::JUDGE_PG_ALARM;
				sDefectCode	= _T("");
			}
			else if ( pCell->PG_VoltageAlarm == TRUE )
			{
				strDefect	= TEXT_RETEST;								// VOLTAGE_ALRAM;
				nClass		= eBIN2_CELL;
				nJudgeIndex = CONST_JUDGE_NAME::JUDGE_PG_ALARM;
				sDefectCode	= _T("");
			}
			// 2015-03-09, jhLee, TACT Time 향상을 위해 A-Zone에서 이루어지던 Contact Current NG 인터락을 해제하여 사용하게 됨으로 아래 판정도 되살린다.
			// 2015-03-06, jhLee, Contact current OK 및 Cell loading이 완료되지 않으면 더이상의 검사가 진행되지 않으므로 아래는 무의미하다.
			else if ( pCell->ContactCurrentClass != eGOOD_CELL )								// A-Zone 에서 Contact Class NG ->
			{
				strDefect	= TEXT_RETEST;			// TEXT_CONTACT_CURRENT_NG;									// Defect명은 Contact Current NG
				//old nClass		= (pCell->ContactCurrentClass == GOOD_CELL) ? CELL_LOG_OK:CELL_LOG_NG;
				nClass		= eBIN2_CELL;											// Retest를 수행햐야 한다. BIN2로 판정,
				nJudgeIndex = CONST_JUDGE_NAME::JUDGE_CONTACT;
				sDefectCode	= _T("");														// 보고용 Defect Code
			}
			else if ( pCell->CellLoadingClass != eGOOD_CELL )						// A-Zone에서 Cell loading시 오류가 발생했는가 ?
			{
				strDefect	= TEXT_RETEST;			// TEXT_CELL_LOADING_NG;										// Defect명은 Cell Loading NG 
				//old nClass		= (pCell->CellLoadingClass == GOOD_CELL) ? CELL_LOG_OK:CELL_LOG_NG;
				nClass		= eBIN2_CELL;											// Retest를 수행햐야 한다. BIN2로 판정,
				nJudgeIndex = CONST_JUDGE_NAME::JUDGE_CELL_LOADING;
				sDefectCode	= _T("");														// 보고용 Defect Code
			}
			else		// 그 외 별다른 불량이 없다면, 양품처리
			{
				strDefect	= TEXT_GOOD;
				nClass		= eGOOD_CELL;
				nJudgeIndex = CONST_JUDGE_NAME::JUDGE_MCR;
				sDefectCode	= _T("");	
			}
			break;
	}

	return nJudgeIndex;
}


// 지정 Cell의 최종 판정을 처리한다. (D-Zone 검사완료 후 실시)
//
//2015-02-23, jhLee, 품질경고 시스템에 Defect 정보를 추가한다.
//
void CMCMain::ProcessFinalJudge(int nJig)
{
	int i;
	TCellData* pCell;
	CString sDefect, sDefectCode;
	// int nClass;


	for (i = 0; i < eMAX_ChCount; i++) 
	{
		pCell = m_pCellInfo->GetCellData(nJig, i);		// 지정 Jig의 Cell Data 획득

		if ( pCell->bIsExist == TRUE)							// Cell이 존재하는 경우에만 실시
		{
			// 전체 생산 수량을 증가시킨다.
			AddTotalProductCount();				// 총 생산량 누적

			// 2015-04-03, jhLee, 사전에 C-Zone에서 이미 Contact OFF 되면서 최종 판정을 하였다면, 이곳에서 다시 처리할 필요는 없다.
			//  ==> 다시 처리하도록 한다. 현재 통계데이터 처리와 연관이 있어서 다시 처리.


			//? if ( m_pCellInfo->GetPreJudgmentFlag( nJig ) == FALSE )
			//? {
				// 만약 지금까지도 D-Zone에서 판정이 되지 않았다면 Good으로 지정한다.
				if ( (pCell->DZoneManualClass == eNONE_CELL) && (pCell->DZoneManualInspState == eINSP_NONE) )
				{
					pCell->DZoneManualClass			= eGOOD_CELL;
					pCell->DZoneManualInspState	= eINSP_END;
					pCell->DZoneManualDefect		= TEXT_GOOD;
					pCell->DZoneManualCode			= _T("");
				}

				// 각 Zone의 대표 불량을 지정한다.
				// 이동 GetDefectFromJudge(ZONE_A, pCell, pCell->AZoneDefect, pCell->AZoneClass, sDefectCode);
				// GetDefectFromJudge(eIDX_PG_ZONE_B, pCell, pCell->BZoneDefect, pCell->BZoneClass, sDefectCode);		// B-Zone은 존재하지 않으나 실시,
				// GetDefectFromJudge(eIDX_PG_ZONE_C, pCell, pCell->CZoneDefect, pCell->CZoneClass, sDefectCode);
				// GetDefectFromJudge(eIDX_PG_ZONE_D, pCell, pCell->DZoneDefect, pCell->DZoneClass, sDefectCode);

				SetZoneJudgement(eIDX_PG_ZONE_B, pCell);			
				SetZoneJudgement(eIDX_PG_ZONE_C, pCell);			
				SetZoneJudgement(eIDX_PG_ZONE_D, pCell);			
				SetZoneJudgement(eIDX_PG_ZONE_Final, pCell);			


				//// 만약 지금까지도 D-Zone에서 판정이 되지 않았다면 Good으로 지정한다.
				//if ( (pCell->DZoneManualClass == NONE_CELL) && (pCell->DZoneManualInspState == INSP_NONE) )
				//{
				//	pCell->DZoneManualClass = GOOD_CELL;
				//	pCell->DZoneManualInspState = INSP_END;
				//	pCell->DZoneManualDefect = TEXT_GOOD;
				//	pCell->DZoneManualCode	= _T("");
				//}

				//// 최종 판정
				//GetDefectFromJudge(eIDX_PG_ZONE_Final, pCell, sDefect, nClass, sDefectCode);
				//pCell->FinalDefect	= sDefect;
				//pCell->MesCode			= sDefectCode;			// CIM 보고용 코드
				//pCell->FinalLabel		= sDefect;				// PrintName	= sDefect;


			//? } //ofif ( m_pCellInfo->GetPreJudgmentFlag( nJig ) == FALSE )


			// 자동 검사에 의한 판정 집계
			if ( pCell->VTClass == eGOOD_CELL )		// 양품 판정
			{
				AddProductCount( eIDX_JudgeAuto, eJUDGE_Good );
			}
			else 	if ( pCell->VTClass == eLOSS_CELL )		// 불량 판정
			{
				AddProductCount( eIDX_JudgeAuto, eJUDGE_Reject );
			}
			else //	if ( pCell->VTClass == BIN2_CELL )		// 재검 판정
			{
				AddProductCount( eIDX_JudgeAuto, eJUDGE_BIN2 );
			}


			// 최종 판정에 의한 집계	
			if (pCell->FinalClass == eGOOD_CELL)
			{
				pCell->FinalClass = eGOOD_CELL;
				AddProductCount( eIDX_JudgeFinal, eJUDGE_Good );
			}
			else if (pCell->FinalClass == eLOSS_CELL)
			{
				pCell->FinalClass = eLOSS_CELL;
				AddProductCount( eIDX_JudgeFinal, eJUDGE_Reject );

				if ( m_rProfileData.bUse_LabelPrinter == TRUE) 
				{	
					// Defect 정보를 출력한다.
					CSvcLabelPrinter* pLabelPrinter = (CSvcLabelPrinter*)( (CGxServiceHub::GetInstance())->GetService(NAME_SERVICE_LabelPrinter) );
					pLabelPrinter->DefectPrintOut(pCell, m_rProfileData.sEQID);
				}

			}
			else
				{ // 재검
					pCell->FinalClass = eBIN2_CELL;
					AddProductCount( eIDX_JudgeFinal, eJUDGE_BIN2 );
				}

		}//of if cell exist ?

	}//of for i

	SaveProductCount();					// 생산량 저장 및 화면 갱신

}



// 지정 지그를 해당 Test를 수행할 것인가 ?
BOOL CMCMain::GetIsInspProcess(int nInspIdx, int nJig, int nCh)
{
	TCellData* pCell;
	CString sCode;

	pCell = m_pCellInfo->GetCellData(nJig, nCh);		// 지정 Jig의 Cell Data 획득

	if ( pCell != NULL)
	{
		// 가장 우선, 해당 채널에 Cell이 존재하지 않는다면 검사할 것도 없다.
		if ( pCell->bIsExist != TRUE )					// Cell이 존재하는 경우에만 실시
		{
			return FALSE;
		}

		// 신호기가 Down되었나 ?
		if (	( pCell->PG_VoltageAlarm == TRUE ) ||
				( pCell->PG_CurrentAlarm == TRUE ) ||
				( pCell->PG_SystemError == TRUE ) ||
				( pCell->PG_DownError == TRUE ) )
		{
			return FALSE;
		}


		switch ( nInspIdx )
		{
			// case eIDX_INSP_AZoneAlign :			// A-Zone Align(UV-MCR) 동작 
			//	
			//	break;

			case eIDX_INSP_CZoneVI		:	// C-Zone Vision  검사

				// Contact Error이면 검사를 하지 못하므로 검사를 Skip한다
				if ( pCell->ContactCurrentClass != eGOOD_CELL )
				{
					return FALSE;
				}

				// Cell Loading Error이면 검사를 하지 못하므로 검사를 Skip한다
				if ( pCell->CellLoadingClass != eGOOD_CELL )
				{
					return FALSE;
				}


				// Option에서 Vision 검사하라고 지정되었나 ?
				if ( m_rProfileData.bUseTest_VI == TRUE )
				{
					// A-Zone Defect이 있는가 ? Contact 불량 / 작업자 OI 검사
					//old if ( pCell->AZoneDefect != TEXT_GOOD ) // _T("") )			// Defect이 존재한다면, VI 검사를 하지 않는다.
					if ( pCell->AZoneManualClass != eGOOD_CELL )							// A존 수동검사 판정이 Reject이라면,
					{
						//비록 A-Zone에서 Defect이 발생하였다하더라도 강제 검사가 지정되어있다면,
						if ( m_rProfileData.bUseTest_VIForce == TRUE )	
						{
							return TRUE;				// VI 검사를 강행한다.
						}
		
						return FALSE;					// 그렇지 않다면 Vison 검사를 수행하지 않는다.
					}
				}//of if VI TRUE
				else
					return FALSE;						// Vision 검사를 하지 않는다고 하였다면 검사 미실행

				break;


			case eIDX_INSP_CZoneTECheck :					// C-Zone 신호기 TE_CHECK  검사 여부

				// Option에서 TE_CHECK 검사하라고 지정되있지 않다면 
				if ( m_rProfileData.bUseTest_TECheck == FALSE )
				{
					return FALSE;				// TE_CHECK 검사를 진행 한다.
				}

				// Contact Error이면 검사를 하지 못하므로 검사를 Skip한다
				if ( pCell->ContactCurrentClass != eGOOD_CELL )
				{
					return FALSE;
				}

				// Cell Loading Error이면 검사를 하지 못하므로 검사를 Skip한다
				if ( pCell->CellLoadingClass != eGOOD_CELL )
				{
					return FALSE;
				}


				// A-Zone Defect이 있는가 ? Contact 불량 / 작업자 OI 검사 / MES Cell 정보
				if ( pCell->AZoneManualClass != eGOOD_CELL )							// A존 수동검사 판정이 Reject이라면,
				{
					//비록 A-Zone에서 Defect이 발생하였다하더라도 강제 검사가 지정되어있다면,
					if ( m_rProfileData.bUseTest_VIForce == TRUE )	
					{
						return TRUE;				// VI 검사 및 TE Check를 강행한다.
					}

					return FALSE;					// A-Zone Defect이 있다면 TE Check 생략
				}
		
				// 그 외는 TE_CHECK를 수행한다.
				break;



			case eIDX_INSP_CZoneTSP		:	// C-Zone TSP 검사
				// Option에 검사하라고 지정되었나 ?
				if ( m_rProfileData.bUseTest_TSP == FALSE )
				{
					return FALSE;
				}

				// Contact Error이면 검사를 하지 못하므로 검사를 Skip한다
				if ( pCell->ContactCurrentClass != eGOOD_CELL )
				{
					return FALSE;
				}

				// Cell Loading Error이면 검사를 하지 못하므로 검사를 Skip한다
				if ( pCell->CellLoadingClass != eGOOD_CELL )
				{
					return FALSE;
				}


				// A-Zone Defect이 있는가 ? Contact 불량 / 작업자 OI 검사 / MES Cell 정보
				if ( pCell->AZoneManualClass != eGOOD_CELL )							// A존 수동검사 판정이 Reject이라면,
				{
					//비록 A-Zone에서 Defect이 발생하였다하더라도 강제 검사가 지정되어있다면,
					if ( m_rProfileData.bUseTest_VIForce == TRUE )	
					{
						return TRUE;				// VI 검사 및 TE Check를 강행한다.
					}

					return FALSE;					// A-Zone Defect이 있다면 TE Check 생략
				}

				break;


			case eIDX_INSP_DZoneManual :	// D-Zone Manual Defect 검사
				// Option에 검사하라고 지정되었나 ?
				if ( m_rProfileData.bUseTest_Manual == FALSE )
				{
					return FALSE;
				}
				break;
		}//of switch
	}//of pCell 

	return TRUE;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//
// 검사를 진행하지 못한 Defect이 발생하였다면,
// 'RETRY'를 표시해야하는 VI 결과값인가 ?
BOOL CMCMain::GetVIRetryDefect(TCellData* pCell)
{
	if ( !pCell ) return FALSE;
	if ( pCell->bIsExist == FALSE ) return FALSE;			// Cell이 존재해야지 판정이 가능하다.

	return GetVIRetryDefect( pCell->BZoneDefect );

	//return CGxUtility::IsIn_s( 8,							// 비교 수량은 4개
	//						pCell->BZoneDefect,				// 불량명으로 비교		
	//						_T("Inspection Fail"), 
	//						_T("Display불량"),	
	//						_T("No Panel"), 
	//						_T("Block Defect"),

	//						_T("NO FOLDER"),				// Vision 검사 결과 파일을 취득하지 못한 경우에도 재검을 실시한다.
	//						_T("NO FILE1"),
	//						_T("NO FILE2"),
	//						_T("NO DATA") );
}

// 설비성 비전 불량인가 ? 'RETRY'를 표시해야하는 VI 결과값인가 ?
BOOL CMCMain::GetVIEQErrorDefect(CString sName)
{
	if ( sName.IsEmpty() ) return FALSE;

	return CGxUtility::IsIn_s( 8,							// 비교 수량은 4개
							sName,								// 주어진 불량명으로 비교		
							_T("Inspection Fail"), 
							_T("Display불량"),	
							_T("No Panel"), 
							_T("Block Defect"),

							_T("NO FOLDER"),				// Vision 검사 결과 파일을 취득하지 못한 경우에도 재검을 실시한다.
							_T("NO FILE1"),
							_T("NO FILE2"),
							_T("NO DATA") );
}





// 지정 검사를 수행할 것인가 ? 이전 단계의 결과에 따라 수행여부 판단
BOOL CMCMain::GetPerformStep(int nJig, int nCh, int nStep)
{
	TCellData* pCell = m_pCellInfo->GetCellData(nJig, nCh);

	if ( !pCell ) return FALSE;				// 데이터를 찾지 못했다면 수행 불가
	if ( !pCell->bIsExist ) return FALSE;	// Cell이 존재하지 않는다면 수행 불가

	BOOL bPerform = TRUE;						// 수행여부 초기화

	// 조회를 원하는 단계별로 조건을 검사한다.
	switch ( nStep )
	{
		// 미사용
		// case eIDX_StepClear :			// 기존 결과를 모두 지우고 Clear 한다.
		// case eIDX_StepReadyButton :	// 양수버튼 눌렸는지 여부

		// 테스트 순서대로 조사한다.
		case eIDX_StepContact :			// Contact current
			if ( pCell->AZoneManualClass == eLOSS_CELL ) bPerform = FALSE;		// A Zone 메뉴얼 검사에서 불량 코드를 입력하였다면 수행 불가
			break;

		case eIDX_StepCellLoading :	// Cell Loading
			if ( pCell->AZoneManualClass == eLOSS_CELL ) bPerform = FALSE;		// A Zone 메뉴얼 검사에서 불량 코드를 입력하였다면 수행 불가
			break;

		case eIDX_StepIDCheck :			// MTP ID Check
			if (	(pCell->AZoneManualClass == eLOSS_CELL) ||							// A Zone 메뉴얼 검사에서 불량 코드를 입력하였다면 수행불가
					(pCell->ContactCurrentClass == eLOSS_CELL) ||				// A Zone Contact Current 불량이 발생하였다면 수행불가
					(pCell->CellLoadingClass == eLOSS_CELL) )					// A Zone Cell Loading 결과가 불량이라면 수행불가
			{
				 bPerform = FALSE;		
			}
			break;

		case eIDX_StepSleepCurrent :	// Sleep Current 
			if (	(pCell->AZoneManualClass == eLOSS_CELL) ||							// A Zone 메뉴얼 검사에서 불량 코드를 입력하였다면 수행불가
					(pCell->ContactCurrentClass == eLOSS_CELL) ||				// A Zone Contact Current 불량이 발생하였다면 수행불가
					(pCell->CellLoadingClass == eLOSS_CELL) ||					// A Zone Cell Loading 결과가 불량이라면 수행불가
					(pCell->IDCheckClass == eLOSS_CELL) )						// A Zone ID Check 결과가 불량이라면 수행불가
			{
				 bPerform = FALSE;		
			}
			break;


		case eIDX_StepVisionInsp :		// Vision 검사 수행
			if (	(pCell->AZoneManualClass == eLOSS_CELL) ||							// A Zone 메뉴얼 검사에서 불량 코드를 입력하였다면 수행불가
					(pCell->ContactCurrentClass == eLOSS_CELL) ||				// A Zone Contact Current 불량이 발생하였다면 수행불가
					(pCell->CellLoadingClass == eLOSS_CELL) ||					// A Zone Cell Loading 결과가 불량이라면 수행불가
					(pCell->IDCheckClass == eLOSS_CELL) || 						// A Zone ID Check 결과가 불량이라면 수행불가
					(pCell->SleepCurrentClass == eLOSS_CELL) )					// A Zone Sleep Current 결과가 불량이라면 수행불가
			{
				 bPerform = FALSE;		
			}
			break;

		case eIDX_StepTSPEnd :			// TSP 
			if (	(pCell->AZoneManualClass == eLOSS_CELL) ||					// A Zone 메뉴얼 검사에서 불량 코드를 입력하였다면 수행불가
					(pCell->ContactCurrentClass == eLOSS_CELL) ||				// A Zone Contact Current 불량이 발생하였다면 수행불가
					(pCell->CellLoadingClass == eLOSS_CELL) ||					// A Zone Cell Loading 결과가 불량이라면 수행불가
					(pCell->IDCheckClass == eLOSS_CELL) || 						// A Zone ID Check 결과가 불량이라면 수행불가
					(pCell->SleepCurrentClass == eLOSS_CELL) ||					// A Zone Sleep Current 결과가 불량이라면 수행불가
					(pCell->VTClass == eLOSS_CELL ) )								// B Zone Vision 검사 결과가 불량인 경우
			{
				 bPerform = FALSE;		
			}
			break;

		case eIDX_StepWhiteCurrent :	// White Current , 자동으로 실행되는 것이라 본 case문은 사용되지 않는다.
			if (	(pCell->AZoneManualClass == eLOSS_CELL) ||					// A Zone 메뉴얼 검사에서 불량 코드를 입력하였다면 수행불가
					(pCell->ContactCurrentClass == eLOSS_CELL) ||				// A Zone Contact Current 불량이 발생하였다면 수행불가
					(pCell->CellLoadingClass == eLOSS_CELL) ||					// A Zone Cell Loading 결과가 불량이라면 수행불가
					(pCell->IDCheckClass == eLOSS_CELL) || 						// A Zone ID Check 결과가 불량이라면 수행불가
					(pCell->SleepCurrentClass == eLOSS_CELL) ||					// A Zone Sleep Current 결과가 불량이라면 수행불가
					(pCell->VTClass == eLOSS_CELL ) ||								// B Zone Vision 검사 결과가 불량인 경우
					(pCell->TspClass == eLOSS_CELL ) )								// C Zone TSP 검사 결과가 불량인 경우
			{
				 bPerform = FALSE;		
			}
			break;

		case eIDX_StepManualInsp :		// C-Zone Manual 검사 
			if (	(pCell->AZoneManualClass == eLOSS_CELL) ||					// A Zone 메뉴얼 검사에서 불량 코드를 입력하였다면 수행불가
					(pCell->ContactCurrentClass == eLOSS_CELL) ||				// A Zone Contact Current 불량이 발생하였다면 수행불가
					(pCell->CellLoadingClass == eLOSS_CELL) ||					// A Zone Cell Loading 결과가 불량이라면 수행불가
					(pCell->IDCheckClass == eLOSS_CELL) || 						// A Zone ID Check 결과가 불량이라면 수행불가
					(pCell->SleepCurrentClass == eLOSS_CELL) ||					// A Zone Sleep Current 결과가 불량이라면 수행불가
					(pCell->VTClass == eLOSS_CELL ) ||								// B Zone Vision 검사 결과가 불량인 경우
					(pCell->TspClass == eLOSS_CELL ) ||								// C Zone TSP 검사 결과가 불량인 경우
					(pCell->WhiteCurrentClass == eLOSS_CELL ) )					// C Zone White Current 결과가 불량인 경우
			{
				 bPerform = FALSE;		
			}
			break;

		default:
			bPerform = FALSE;		
			break;
	}//of switch ()

	return bPerform;
}



// 지정 Zone에서의 대표불량을 판정한다.
//
// [ 각 Zone별 판정 우선순위 정의 ]
// 
void CMCMain::SetZoneJudgement(int nZone, TCellData* pCell)
{
	if ( !pCell ) return ;
	if ( pCell->bIsExist == FALSE ) return;				// Cell이 존재해야지 판정이 가능하다.


	CString sKey, sMESCode, sMESName;		// MES 보고용 Defect code와 명칭
	TDefectItem* pDefaultDefect = NULL;
	

	switch ( nZone )
	{
		case eIDX_PG_ZONE_A:	// A-Zone의 대표 불량

				// 수동검사가 판정한 불량이 존재한다면,
				if ( pCell->AZoneManualClass == eLOSS_CELL )		
				{
					pCell->AZoneClass		= eLOSS_CELL; 
					pCell->AZoneDefect	= pCell->AZoneManualDefect;			// 수동입력 Defect 채용
					pCell->AZoneCode		= pCell->AZoneManualCode;			// 수동검사 Defect Code, CIM 보고용
					pCell->AZoneLabel		= pCell->AZoneManualLabel;			// 라벨 출력할 내용
				}
				else if ( (pCell->ContactCurrentClass != eGOOD_CELL) )		//d && (m_rProfileData.bUseTest_Skip_Loading == FALSE) )		// Contact Current NG
				{
					// Mapping Table에서 대표 코드를 찾는다.
					if ( GetDefectNameCode( pCell->ContactCurrentResult, sMESName, sMESCode ) )
					{
						pCell->AZoneCode		= sMESCode;						// 수동검사 Defect Code, CIM 보고용
						pCell->AZoneLabel		= sMESName;						// 라벨 출력용 디펙명
					}
					else
					{
						pDefaultDefect = m_rDefectList.GetDefault( eDEFAULT_DefectContact );		// Contact Current 실패에 대한 Defaut defect 조회
						if ( pDefaultDefect->sCode.IsEmpty() == FALSE )		// 기본 Defect이 존재한다면,
						{
							// pCell->AZoneDefect		= _T("FT)No Display"); 
							pCell->AZoneCode		= pDefaultDefect->sCode;
							pCell->AZoneLabel		= pDefaultDefect->sDefect[eLANG_ENGLISH];		// 라벨 및 Cell에 출력할 내용
						}
						else
						{
							// 지정된 Default 불량명이 없다면, 최초로 지정한 값을 사용한다.
							pCell->AZoneCode		= m_rCellDisp.bRigidModel ? _T("SF02") : _T("USF44");
							pCell->AZoneLabel		= m_rCellDisp.bRigidModel ? _T("FT)No Display") : _T("FT)No Display");			
						}
					}

					pCell->AZoneDefect	= pCell->ContactCurrentResult;	// Contact current 측정 결과 메세지 그대로 채용
					pCell->AZoneClass		= eLOSS_CELL;							// 불량 발생
				}
				else if ( (pCell->CellLoadingClass != eGOOD_CELL) )		//d && (m_rProfileData.bUseTest_Skip_Loading == FALSE) )			// Cell Loading NG
				{
					if ( GetDefectNameCode( pCell->CellLoadingResult, sMESName, sMESCode ) )
					{
						pCell->AZoneCode		= sMESCode;						// 수동검사 Defect Code, CIM 보고용
						pCell->AZoneLabel		= sMESName;						// 라벨 출력용 디펙명
					}
					else
					{
						// Mapping Table에서 대표 코드를 찾는다.
						if ( GetDefectNameCode( pCell->CellLoadingResult, sMESName, sMESCode ) )
						{
							pCell->AZoneCode		= sMESCode;						// 수동검사 Defect Code, CIM 보고용
							pCell->AZoneLabel		= sMESName;						// 라벨 출력용 디펙명
						}
						else
						{
							pDefaultDefect = m_rDefectList.GetDefault( eDEFAULT_DefectContact );		// Contact Current 실패에 대한 Defaut defect 조회
							if ( pDefaultDefect->sCode.IsEmpty() == FALSE )		// 기본 Defect이 존재한다면,
							{
								pCell->AZoneCode		= pDefaultDefect->sCode;
								pCell->AZoneLabel		= pDefaultDefect->sDefect[eLANG_ENGLISH];		// 라벨 및 Cell에 출력할 내용
							}
							else
							{
								// 지정된 Default 불량명이 없다면, 최초로 지정한 값을 사용한다.
								pCell->AZoneCode		= m_rCellDisp.bRigidModel ? _T("SF02") : _T("USF44");
								pCell->AZoneLabel		= m_rCellDisp.bRigidModel ? _T("FT)No Display") : _T("FT)No Display");			
							}
						}
					}

					pCell->AZoneDefect	= pCell->CellLoadingResult;			// 결과 메세지 그대로 채용
					pCell->AZoneClass		= eLOSS_CELL;						// 불량 발생
				}
				// ID-Check 미사용
				//else if ( (pCell->IDCheckClass == eLOSS_CELL) && (m_rProfileData.bUseTest_Skip_IDCheck == FALSE) )	// MTP ID Check를 통과하지 못했다.
				//{
				//	// Mapping Table에서 대표 코드를 찾는다.
				//	if ( GetDefectNameCode( pCell->IDCheckResult, sMESName, sMESCode ) )
				//	{
				//		pCell->AZoneCode		= sMESCode;						// 수동검사 Defect Code, CIM 보고용
				//		pCell->AZoneLabel		= sMESName;						// 라벨 출력용 디펙명
				//	}
				//	else
				//	{
				//		pDefaultDefect = m_rDefectList.GetDefault( eDEFAULT_DefectIDCheck );		// ID CHeck 실패에 대한 Defaut defect 조회
				//		if ( pDefaultDefect->sCode.IsEmpty() == FALSE )		// 기본 Defect이 존재한다면,
				//		{
				//			pCell->AZoneCode		= pDefaultDefect->sCode;
				//			pCell->AZoneLabel		= pDefaultDefect->sDefect[eLANG_ENGLISH];		// 라벨 및 Cell에 출력할 내용
				//		}
				//		else
				//		{
				//			//pCell->AZoneDefect		= _T("SLEEP CURRENT NG");			
				//			//old pCell->AZoneCode		= _T("USF17");							
				//			//old pCell->AZoneLabel		= _T("SLEEP CURRENT NG");

				//			// 지정된 Default 불량명이 없다면, 최초로 지정한 값을 사용한다.
				//			pCell->AZoneCode		= m_rCellDisp.bRigidModel ? _T("SF62") : _T("USF07");
				//			pCell->AZoneLabel		= m_rCellDisp.bRigidModel ? _T("ID_CHECK_NG") : _T("ID_CHECK_NG");			
				//		}
				//	}

				//	pCell->AZoneDefect	= pCell->AZoneLabel; // IDCheckResult;				// 결과 메세지 그대로 채용
				//	pCell->AZoneClass	= LOSS_CELL;
				//}
				// Sleep Current 미사용
				//else if ( (pCell->SleepCurrentClass	!= GOOD_CELL) && (m_rProfileData.bUseTest_Skip_Sleep == FALSE) )	// Sleep Current를 통과하지 못했다.
				//{
				//	if ( GetDefectNameCode( pCell->SleepCurrentResult, sMESName, sMESCode ) )
				//	{
				//		pCell->AZoneCode		= sMESCode;						// 수동검사 Defect Code, CIM 보고용
				//		pCell->AZoneLabel		= sMESName;						// 라벨 출력용 디펙명
				//	}
				//	else
				//	{
				//		pDefaultDefect = m_rDefectList.GetDefault( eDEFAULT_DefectSleep );		// Sleep Current 실패에 대한 Defaut defect 조회
				//		if ( pDefaultDefect->sCode.IsEmpty() == FALSE )		// 기본 Defect이 존재한다면,
				//		{
				//			pCell->AZoneCode		= pDefaultDefect->sCode;
				//			pCell->AZoneLabel		= pDefaultDefect->sDefect[eLANG_ENGLISH];		// 라벨 및 Cell에 출력할 내용
				//		}
				//		else
				//		{
				//			//pCell->AZoneDefect		= _T("SLEEP CURRENT NG");			
				//			//old pCell->AZoneCode		= _T("USF17");							
				//			//old pCell->AZoneLabel		= _T("SLEEP CURRENT NG");

				//			// 지정된 Default 불량명이 없다면, 최초로 지정한 값을 사용한다.
				//			pCell->AZoneCode		= m_rCellDisp.bRigidModel ? _T("SF09") : _T("USF17");
				//			pCell->AZoneLabel		= m_rCellDisp.bRigidModel ? _T("Sleep Current NG") : _T("Sleep Current NG");			
				//		}
				//	}

				//	pCell->AZoneDefect	= pCell->SleepCurrentResult;			// 결과 메세지 그대로 채용
				//	pCell->AZoneClass		= LOSS_CELL;
				//}
				else // 나머지는 양품 
				{
					pCell->AZoneClass		= eGOOD_CELL;
					pCell->AZoneDefect	= TEXT_Good;
					pCell->AZoneCode		= TEXT_Null;
					pCell->AZoneLabel		= TEXT_Null;
				}

				// 아직까지 A Zone Manual 판정이 없다면 GOOD으로 지정
				if ( pCell->AZoneManualClass == eNONE_CELL)
				{
					pCell->AZoneManualClass		= eGOOD_CELL;
					pCell->AZoneManualDefect	= TEXT_Good;
				}

				// 사용여부 협의要
				//// A-Zone에서 LOSS가 판정되었다면 D-Zone에서의 Back/Next 버튼을 막도록 한다.
				//if ( pCell->AZoneClass == eLOSS_CELL )
				//{
				//	pCell->bNextBackLock	= TRUE;			// C-Zone에서 작업자 패턴 테스트시 이미 LOSS가 발생한 셀의 경우 Next/Back 버튼을 막도록 한다.
				//}

				// 해당 Cell의 판정이 변경되었다.
				//! ::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, UDMSG_DEFECT_CHANGE, (WPARAM)pCell->JigNo, (LPARAM)pCell->ChNo);
				break; // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


		case eIDX_PG_ZONE_B:	// B-Zone의 대표 불량

				// 2015-12-06, jhLee, 현재까지 TE_CHECK는 미확정, 일단 배제하고 추후에 추가하도록 한다.
				// TE_CHECK 불량 발생

				//// Vision 검사결과 불량 판정
				//if( (pCell->VTClass != GOOD_CELL) && (m_rProfileData.bUseTest_Skip_VI == FALSE) )		// C-Zone에서 Vistion 검사를 진행할 것인지 여부
				//{
				//	pCell->BZoneClass		= BIN2_CELL;
				//	pCell->BZoneDefect		= pCell->ResultVTName;
				//	pCell->BZoneCode		= pCell->VTDefectCode;			// 보고용 Defect Code
				//}
				//else	// 별다른 Defect이 없다면 양품
				{
					pCell->BZoneClass		= eGOOD_CELL;
					pCell->BZoneDefect	= TEXT_Good;
					pCell->BZoneCode		= TEXT_Null;
				}

				// 해당 Cell의 판정이 변경되었다.
				//! ::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, UDMSG_DEFECT_CHANGE, (WPARAM)pCell->JigNo, (LPARAM)pCell->ChNo);
				break; // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


		case eIDX_PG_ZONE_C:	// C-Zone의 대표 불량

				// TSP 관련 불량 발생 
				if( ( pCell->TspClass == eLOSS_CELL) && (m_rProfileData.bUseTest_TSP == TRUE) )		
				{
					if ( GetDefectNameCode( pCell->TspNGMainName, sMESName, sMESCode ) )
					{
						pCell->CZoneCode		= sMESCode;							// 수동검사 Defect Code, CIM 보고용
						pCell->CZoneLabel		= sMESName;							// 라벨 출력용 디펙명
					}
					else
					{
						pDefaultDefect = m_rDefectList.GetDefault( eDEFAULT_DefectTSP );		// TSP  실패에 대한 Defaut defect 조회
						if ( pDefaultDefect->sCode.IsEmpty() == FALSE )		// 기본 Defect이 존재한다면,
						{
							pCell->CZoneCode		= pDefaultDefect->sCode;
							pCell->CZoneLabel		= pDefaultDefect->sDefect[eLANG_ENGLISH];		// 라벨 및 Cell에 출력할 내용
						}
						else
						{
							pCell->CZoneCode		= m_rCellDisp.bRigidModel ? _T("ST14") : _T("UST14");
							pCell->CZoneLabel		= m_rCellDisp.bRigidModel ? _T("TSP IC") : _T("TSP IC");			

						}
					}

					pCell->CZoneDefect	= pCell->TspNGMainName;				// 수신 내용 그대로 채용
					pCell->CZoneClass		= eLOSS_CELL;
				}	// end of TSP - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -	
				else if( ( pCell->TE_CheckClass == eLOSS_CELL) && (m_rProfileData.bUseTest_TECheck == TRUE) )		
				{
					if ( GetDefectNameCode( pCell->TE_CheckName, sMESName, sMESCode ) )
					{
						pCell->CZoneCode		= sMESCode;							// 수동검사 Defect Code, CIM 보고용
						pCell->CZoneLabel		= sMESName;							// 라벨 출력용 디펙명
					}
					else
					{
						pDefaultDefect = m_rDefectList.GetDefault( eDEFAULT_DefectTECheck );		// TE_Check 실패에 대한 Defaut defect 조회
						if ( pDefaultDefect->sCode.IsEmpty() == FALSE )		// 기본 Defect이 존재한다면,
						{
							pCell->CZoneCode		= pDefaultDefect->sCode;
							pCell->CZoneLabel		= pDefaultDefect->sDefect[eLANG_ENGLISH];		// 라벨 및 Cell에 출력할 내용
						}
						else
						{
							pCell->CZoneCode		= m_rCellDisp.bRigidModel ? _T("ST14") : _T("UST14");
							pCell->CZoneLabel		= m_rCellDisp.bRigidModel ? _T("TSP IC") : _T("TSP IC");			
						}
					}

					pCell->CZoneDefect	= pCell->TE_CheckName;				// 수신 내용 그대로 채용
					pCell->CZoneClass		= eLOSS_CELL;
				}	// end of TSP - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -	
				// Vision 검사결과 불량 판정
				else if( (pCell->VTClass != eGOOD_CELL) )						//d && (m_rProfileData.bUseTest_Skip_VI == FALSE) )		// C-Zone에서 Vistion 검사를 진행할 것인지 여부
				{
					pCell->CZoneClass		= eBIN2_CELL;
					pCell->CZoneDefect	= pCell->VTResult;
					pCell->CZoneCode		= pCell->VTCode;					// 보고용 Defect Code
				}

				else	// 별다른 Defect이 없다면 양품
				{
					pCell->CZoneClass		= eGOOD_CELL;
					pCell->CZoneDefect	= TEXT_Good;
					pCell->CZoneCode		= TEXT_Null;
				}

				// 해당 Cell의 판정이 변경되었다.
				//! ::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, UDMSG_DEFECT_CHANGE, (WPARAM)pCell->JigNo, (LPARAM)pCell->ChNo);
				break; // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


		case eIDX_PG_ZONE_D:	// D-Zone의 대표 불량

				// D-Zone에서 작업자가 재검 ('RETEST')를 선택하였나 ?
				if( (pCell->DZoneManualClass == eBIN2_CELL) && (pCell->DZoneManualInspState == eINSP_END) )		//d && (m_rProfileData.bUseTest_SKip_Manual == FALSE))		
				{
					pCell->DZoneClass			= eBIN2_CELL;
					pCell->DZoneDefect		= TEXT_Retest;
					pCell->DZoneCode			= TEXT_Null;
					pCell->DZoneLabel			= TEXT_Null;
				}
				// 작업자 수동 검사에서 불량을  선택하였다.
				else if( (pCell->DZoneManualClass == eLOSS_CELL) && (pCell->DZoneManualInspState == eINSP_END) && (m_rProfileData.bUseTest_Manual == TRUE))		// D-Zone에서 작업자 수동 검사를 진행할 것인지 여부
				{
					pCell->DZoneClass			= eLOSS_CELL;
					pCell->DZoneDefect		= pCell->DZoneManualDefect;
					pCell->DZoneCode			= pCell->DZoneManualCode;
					pCell->DZoneLabel			= pCell->DZoneManualLabel;
				}
				else	// 별다른 Defect이 없다면 양품
				{
					pCell->DZoneClass			= eGOOD_CELL;
					pCell->DZoneDefect		= TEXT_Good;
					pCell->DZoneCode			= TEXT_Null;
					pCell->DZoneLabel			= TEXT_Null;
				}


				// 아직까지 D Zone Manual 판정이 없다면 GOOD으로 지정
				if ( pCell->DZoneManualClass == eNONE_CELL)
				{
					pCell->DZoneManualClass		= eGOOD_CELL;
					pCell->DZoneManualDefect	= TEXT_Good;
				}

				//// C-Zone에서 LOSS가 판정되었다면 Back/Next 버튼을 막도록 한다.
				//if ( pCell->CZoneClass == LOSS_CELL )
				//{
				//	pCell->bNextBackLock	= TRUE;			// C-Zone에서 작업자 패턴 테스트시 이미 LOSS가 발생한 셀의 경우 Next/Back 버튼을 막도록 한다.
				//}

				// 해당 Cell의 판정이 변경되었다.
				// ::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, UDMSG_DEFECT_CHANGE, (WPARAM)pCell->JigNo, (LPARAM)pCell->ChNo);
				break; // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


		// 최종 판정
		case eIDX_PG_ZONE_Final: 
				// Zone별 우선순위 : A Zone -> B Zone -> C Zone 순으로 판정 

				//// 우선순위 0. 투입되지 말아야 할 Cell인 경우 R 처리한다.
				//if ( m_rProfileData.bUse_DispStepNG && (pCell->CellValidateResult > eValidate_GOOD) )	// STEP-NG R 처리 Option 및 예외 발생여부
				//{
				//	if ( m_rProfileData.sExcept_StepNG_Judge == _T("L") )			// Loss 처리를 하라고 지정되었다면
				//	{
				//		pCell->FinalClass			= LOSS_CELL;				// Loss로 지정
				//		pCell->MesReport			= _T("L");					// L 로 지정하여 전송한다.
				//	}
				//	else // RETEST로 전송
				//	{
				//		pCell->FinalClass			= BIN2_CELL;										// Retest로 지정
				//		pCell->MesReport			= _T("R");					// R 로 고정하여 전송한다.
				//	}

				//	pCell->FinalDefect			= m_rProfileData.sExcept_StepNG_Name;				// STEP NG 메세지
				//	pCell->FinalCode			= m_rProfileData.sExcept_StepNG_Code;				// Step NG Code
				//	pCell->FinalLabel.Format(_T("%s %d"), TEXT_StepNG, pCell->CellValidateResult);		// STEP NG 뿐만 아니라 발생원인도 함께 출력
				//}
				// else
				// 우선순위 1. A-Zone  
				if ( pCell->AZoneClass != eGOOD_CELL )		
				{	
					// Mapping Table에서 대표 코드를 찾는다.
					if ( GetDefectNameCode( pCell->AZoneDefect, sMESName, sMESCode ) )
					{
						pCell->FinalDefect	= sMESName;					// 검색된  Defect 채용
						pCell->FinalCode		= sMESCode;					// 수동검사 Defect Code, CIM 보고용
						pCell->FinalLabel		= sMESName;					// 라벨 출력용 디펙명
					}
					else		// 찾지 못했을 경우는 기존에 지정된 내용을 적는다.
					{
						pCell->FinalDefect	= pCell->AZoneDefect;		// 수동입력 Defect 채용
						pCell->FinalCode		= pCell->AZoneCode;			// 수동검사 Defect Code, CIM 보고용
						pCell->FinalLabel		= pCell->AZoneLabel;		// 라벨 출력용 디펙명
					}
					
					pCell->FinalClass		= pCell->AZoneClass;					// A-Zone에서의 판정을 사용
					pCell->MESReport		= pCell->FinalClass == eLOSS_CELL ? _T("L") : _T("R");		// 최종결과 G:Good, L:Loss, R:Retest
				}
				// 우선순위 2. C-Zone 
				else if( pCell->CZoneClass == eLOSS_CELL )					// C-Zone에서 불량판정 사용
				{
					// Mapping Table에서 대표 코드를 찾는다.
					if ( GetDefectNameCode( pCell->CZoneDefect, sMESName, sMESCode ) )
					{
						pCell->FinalDefect	= sMESName;							// 검색된  Defect 채용
						pCell->FinalCode		= sMESCode;							// 수동검사 Defect Code, CIM 보고용
						pCell->FinalLabel		= sMESName;							// 라벨 출력용 디펙명
					}
					else		// 찾지 못했을 경우는 기존에 지정된 내용을 적는다.
					{
						pCell->FinalDefect	= pCell->CZoneDefect;				// 
						pCell->FinalCode		= pCell->CZoneCode;					// 
						pCell->FinalLabel		= pCell->CZoneLabel;					// 라벨 출력용 디펙명 
					}

					pCell->FinalClass		= pCell->CZoneClass;					// 
					pCell->MESReport		= pCell->FinalClass == eLOSS_CELL ? _T("L") : _T("R");		// 최종결과 G:Good, L:Loss, R:Retest
				}
				// 우선순위 3. C-Zone에서 검사 미실시에대한 판정결과라면 RETRY 고정, LOSS 판정은 없다.
				else if (  (pCell->CZoneClass != eGOOD_CELL) &&						// B-Zone에서 양품이 아니고,				
								( (pCell->VTInspState == eINSP_NONE) ||					// 검사를 진행하지 않았다. 검사를 하지 않았다고 지정한 불량 명이 존재
									GetVIRetryDefect( pCell ) ) )

									// CGxUtility::IsIn( pCell->BZoneDefect,				// 검사를 진행하지 못한 Defect이 발생하였다면,	
									// _T("Inspection Fail"), _T("Display불량"),	
									// _T("No Panel"), _T("Block Defect"), _T("")) ) )
				{
						pCell->FinalClass		= eBIN2_CELL;						// 반드시 재검을 해야한다.
						pCell->FinalDefect	= TEXT_Retest;						// Retry 표시
						pCell->FinalCode		= TEXT_Null;						// 없다
						pCell->FinalLabel		= TEXT_Null;						// 없다.
						pCell->MESReport		= _T("R");							// 최종결과 G:Good, L:Loss, R:Retest
				}
				// 우선순위 4. D-Zone 
				else if( pCell->DZoneClass != eGOOD_CELL )					// D-Zone에서 판정 사용
				{
					// Mapping Table에서 대표 코드를 찾는다.
					if ( GetDefectNameCode( pCell->DZoneDefect, sMESName, sMESCode ) )
					{
						pCell->FinalDefect	= sMESName;							// 검색된  Defect 채용
						pCell->FinalCode		= sMESCode;							// 수동검사 Defect Code, CIM 보고용
						pCell->FinalLabel		= sMESName;							// 라벨 출력용 디펙명
					}
					else		// 찾지 못했을 경우는 기존에 지정된 내용을 적는다.
					{
						pCell->FinalDefect	= pCell->DZoneDefect;				// 
						pCell->FinalCode		= pCell->DZoneCode;					// 
						pCell->FinalLabel		= pCell->DZoneLabel;					// 라벨 출력용 디펙명 
					}

					pCell->FinalClass		= pCell->DZoneClass;					// 
					pCell->MESReport		= pCell->FinalClass == eLOSS_CELL ? _T("L") : _T("R");		// 최종결과 G:Good, L:Loss, R:Retest
				}
				//// 2016-05-20, jhLee, POC 대상품 자동 판정 기능 추가
				//else if ( (pCell->POCInfo.IsEmpty() == FALSE) && GetDefectNameCode( pCell->POCInfo, sMESName, sMESCode ) )	// POC 대상품 인가 ?
				//{
				//	pCell->FinalClass		= eLOSS_CELL;						// 
				//	pCell->FinalDefect	= sMESName;							// 검색된  Defect 채용
				//	pCell->FinalCode		= sMESCode;							// 수동검사 Defect Code, CIM 보고용
				//	pCell->FinalLabel		= sMESName;							// 라벨 출력용 디펙명
				//	pCell->MESReport		= _T("L");							// 최종결과 G:Good, L:Lot Loss, R:Retest
				//}
				else
				{
					pCell->FinalClass		= eGOOD_CELL;							// 최종 양품
					pCell->FinalDefect	= TEXT_Good;							// 
					pCell->FinalCode		= TEXT_Null;							// 
					pCell->FinalLabel		= TEXT_Null;;							// 
					pCell->MESReport		= _T("G");								// 최종결과 G:Good, L:Loss, R:Retest
				}

				// 해당 Cell의 판정이 변경되었다.
				//! ::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, UDMSG_DEFECT_CHANGE, (WPARAM)pCell->JigNo, (LPARAM)pCell->ChNo);
				break; // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	} //of switch
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




// 지정한 defect 이름에 해당되는 MES 보고용 이름과 Code를 조회한다.
BOOL CMCMain::GetDefectNameCode(CString sKey, CString &sMESName, CString &sMESCode)
{
	if ( sKey.IsEmpty() ) return FALSE;		

		sKey.Trim();								// 좌우 공백제거
		sKey.Replace(_T(" "), _T(""));				// 글자내 공백제거
		sKey.MakeUpper();							// 모두 대문자로 바꾼다.

	TDefectNameTable*	pData = NULL;

	// 검색 성공
	if ( m_mapDefectTable.Lookup( sKey, pData) == TRUE )
	{
		if ( pData != NULL )
		{
			sMESName = pData->sName3;				// MES 보고용 이름
			sMESCode = pData->sCode;				// MES  보고용 코드

			return TRUE;							// 검색 성공
		}
	}

	return FALSE;			// 찾지 못함
}


//
// Defect name table 매칭용 데이터 파일 읽기
//
BOOL CMCMain::LoadDefectNameTable()
{
	CString strFileName;

		// Rigid용 Defect list인지, Youm인지 구분하여 파일을 읽어온다.
	strFileName.Format(_T("%s%s_%s.csv") , m_sPath_INI, NMAE_FILE_DEFECT_TABLE, IsCellRigid() ? TEXT_NameRigid : TEXT_NameYoum);			// 읽어 올 파일

	// 파일이 존재하는지 점검
	if ( CGxUtility::IsExistFile( strFileName ) == FALSE )		//  파일 존재여부
	{
		CString sMsg;
		sMsg.Format(_T("NG Code List File not found : %s"), strFileName);
		
		CGxMsgBox dlgMsg;
		dlgMsg.SetTitle(_T("알림"), _T("Notifications"), _T("thông báo"));
		dlgMsg.SetMessage(FALSE, sMsg, sMsg, sMsg, 0);
		dlgMsg.DoModal();

		// m_pLogSeq->Log(_T("[MAIN] User Account file read ERROR"));
		return FALSE;
	}

	ReleaseDefectNameTable();			// 이전에 읽었던 내용은 삭제한다.
	// .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. ..

	CString sLine;						// 파일에서 읽어온 데이터
	TDefectNameTable*	pData;

	CString		sKey;					// 검색할 때 사용되는 Key값, 공백제거, 대문자 변환
	CString		sName1;					// 파일에서 읽어들인 Defect 글자
	CString		sName2;					// 영문 및 한글 defect
	CString		sName3;					
	CString		sCode;					// MES 보고용 Defect Code

	//파일을 한줄씩 읽어가면서 넣어준다.;
	// Text 파일 UniCode 방식으로 설정 변경 ( 베트남어 읽고 쓰기 가능)
	FILE *fStream;
	errno_t e = _tfopen_s(&fStream, strFileName.GetString(), _T("r,ccs=UTF-8"));	// UNICODE"));
	if (e != 0) return FALSE;

	CStdioFile		file(fStream);

	// 파일에서 1줄씩 읽어와서 처리한다.
	while ( file.ReadString(sLine) )
	{
		if (sLine.IsEmpty() )	continue;			// 공백일 경우 무시
		if (sLine.Left(1) == _T("#"))	continue;	// 처음 글자가 " # " 일경우 다음 문장으로 넘어간다.;
		if (sLine.Left(1) == _T("?"))	continue;	// 처음 글자가 " ? " 일경우 다음 문장으로 넘어간다.;
		if (sLine.Left(1) == _T("/"))	continue;	// 처음 글자가 " / " 일경우 다음 문장으로 넘어간다.;
		if (sLine.Left(1) == _T("!"))	continue;	// 처음 글자가 " ! " 일경우 다음 문장으로 넘어간다.;
		if (sLine.Left(1) == _T(";"))	continue;	// 처음 글자가 " ; " 일경우 다음 문장으로 넘어간다.;

		::AfxExtractSubString( sName1,	sLine	, 0, ',');			
		::AfxExtractSubString( sName2,	sLine	, 1, ',');			
		::AfxExtractSubString( sName3,	sLine	, 2, ',');			
		::AfxExtractSubString( sCode,		sLine	, 3, ',');			

		if ( sName1.IsEmpty() || sCode.IsEmpty() ) continue;			// 공백이라면 무시

		// 검색할 때 사용 할 키를 만든다.
		sKey = sName1;
		sKey.Trim();							// 좌우 공백제거
		sKey.Replace(_T(" "), _T(""));			// 글자내 공백제거
		sKey.MakeUpper();						// 모두 대문자로 바꾼다.


		// 새로운 데이터 동적생성
		pData = new TDefectNameTable();

		if ( pData )
		{
			pData->sKey			= sKey;

			pData->sName1		= sName1;
			pData->sName2		= sName2;
			pData->sName3		= sName3;
			pData->sCode		= sCode;

			m_arrDefectTalbe.Add( pData );				// Defect 검색 테이블 실제 저장소
			m_mapDefectTable.SetAt(sKey, pData);		// 검색용 Map에 추가한다.
		}
	}//of while

	file.Close();
	fclose(fStream);

	return TRUE;
}

//  메모리 해제
void CMCMain::ReleaseDefectNameTable()
{
	m_mapDefectTable.RemoveAll();			// 맵 해제

	// 실제 데이터가 들어가 있는 동적 배열의 내용을 정리한다.
	for (int i = 0; i < m_arrDefectTalbe.GetCount(); i++)
	{
		delete m_arrDefectTalbe[i];
	}

	m_arrDefectTalbe.RemoveAll();							// 실제 저장소 해제
}






//////////////////////////////////////////////////////////////////////////////////////////////////////////////


//PG begin
/*
// 제품 화면에 특정 메세지를 표시한다.
void CMCMain::PG_DrawCellText(int nJig, int nCh, CString sMsg )
{
	return;


	COLORREF colorMsg = RGB(0xFF, 0, 0);
	CString strDefect, sCode;

	theSockMgr.AddPGMsg( sMsg );
	colorMsg = RGB(0xFF, 0, 0);

	// 글자 크기는 고정비율로 계산... [8/27/2013 OSC]
	double dSize = m_rCellDisp.nFontSize;	// (m_rCellDisp.nResolutionX * 1.0) * MSG_DISPLAY_RATE;
	theSockMgr.SendMsgToPGMsg( nJig, nCh, RGB(0, 0, 0), colorMsg, 10, 10, (int)dSize);
	theSockMgr.ClearPGMsg();
}


// Cell에 판정결과를 표시한다. (D-Zone 작업자 Defect 검사)
void CMCMain::PG_DrawCellJudge(int nJig, int nCh)
{
	return;

	if ( (nJig < 0) || (nJig >= eMAX_JigCount) || (nCh < 0) || (nCh >= eMAX_ChCount) ) return;		// Index 범위 check

	CString strDefect, sCode;
	int nSize			= 120;
	int nClass			= CELL_LOG_OK;
	TCellData* pCell	= m_pCellInfo->GetCellData(nJig, nCh);								// Cell Data Pointer 취득
	int nFinalJudge	= GetDefectFromJudge(eIDX_PG_ZONE_Final, pCell, strDefect, nClass, sCode);		// 지금까지의 최종 판정을 조회
	double dSize = m_rCellDisp.nFontSize;													// Font Size 설정


	// 2015-06-03 JH-PARK
	// 검사결과에 대하여 Cell에 표시하는 방법을 총 3가지 방법으로 지정한다. ( 1:1st Patten우선출력, 2: NG Pattern우선 출력 ,  3:검사결과 Message출력  )
	
	// 1. 1st Patten 출력
	// [검사결과 : 양품] : White 배경 출력 
	// [검사결과 : NG ]  : White 배경에 Circle 표시
	// * A존에 위치한 Prev/Next Button을 누를시 2nd Patten 부터 표시한다.

	if( m_rProfileData.nMode_DispVIResult == eMODE_Disp1stPattern )
	{
		PG_SendNext(nJig,nCh);					// 해당 CH에 PG_NEXT를 지정한다 ( 즉 White Patten으로 넘겨준다)

		if(nCh == eIDX_CH_2)
			m_nPTN_CurrentPage++;				// CH2일때 해당 JIG에 "SET NEXT" 버튼을 누른효과를 지정한다 ( Manual로 "NEXT/PREV" 버튼시 정상적으로 기능수행하기 위하여")

		// [검사결과 : GOOD]
		if( nClass == CELL_LOG_OK)
		{
			// 2015-05-16 기준으로 검사결과가 GOOD일경우 아무작업도 하지 않는다 ( 상위 단계에서 PG_NEXT 명령을 기준으로 White을 표시)
			// 추후 추가사항 요청이 있을경우 해당 부문에 소스 코딩 작업 필요

			// here //
		}
		// [검사결과 : NG ]
		else
		{
			if ( m_rProfileData.bUse_CircleDisp)		// Circle 구현 여부 판다.
			{
				SleepEx(50,FALSE);						// 상위 단계에서 이미 PG NEXT를 보낸시점이라 약간의 Delay를 지정후 Circle 구현 명령을 한다

				PG_DrawCellDefectCircle( pCell );		// 기록된 Circle을 구현한다.
			}

		}
	}//end of Disp : 1st Patten
	// 1. NG Patten 출력
	// [검사결과 : 양품] : White 배경 출력 \
	// [검사결과 : NG ]  : 검사PC가 지정한 Patten ( DETAIL_REJECT_PATTEN_NO)을 신호기에 날려준다.
	// * A존에 위치한 Prev/Next Button을 누를시 첫번째 Patten을 우선적으로 표시한다. (별도의 예외처리를 하기에는 꼬일 가능성이 많다)

	//else if(m_rProfileData.nMode_DispVIResult == eMODE_DispNGPatten)
	//{
	//	m_bDisp1stPattern = TRUE;									// Next /Prev 버튼으로 Pattern 변경시 강제 1st 패턴 출력

	//	// [검사결과 : GOOD]
	//	if( nClass == CELL_LOG_OK)
	//	{
	//		theSockMgr.AddPGMsg( _T("") );							// 공백으로 글자를 지정 ( 단순하게 White 색상을 표시 )
	//		COLORREF Textcolor = RGB(0xFF , 0xFF, 0xFF);			//	글자 색상 (White)
	//		COLORREF BackGroudColor = RGB(0xFF , 0xFF, 0xFF);		//  배경 색상 (White)

	//		theSockMgr.SendMsgToPGMsg(nJig, nCh, BackGroudColor, Textcolor, 10, 10, (int)dSize);	// 신호기에 명령한다.
	//		theSockMgr.ClearPGMsg();																// 초기화 진행
	//	}

	//	// [검사결과 : NG ]
	//	else
	//	{
	//		int nPattenNo = pCell->nResultVTPatternNo;				// Cell에 기록되어 있는 PattenNo를 가져온다. ( 검사PC에서 InnerID.txt을 가져올때 별도로 기록한다 ) 
	//		PG_SendPatternSelect(nJig,nCh,nPattenNo);				// 신호기에 명령한다.
	//		theSockMgr.ClearPGMsg();								// 초기화 진행
	//	}
	//}//end of Disp : NG Patten


	// 3. Message 출력
	// [검사결과 : 양품] : Black 배경  , Green 색상  "OK" 표시
	// [검사결과 : NG ]  : Black 배경  , Red 색상  불량명 표시
	// * A존에 위치한 Prev/Next Button을 누를시 첫번째 Patten을 우선적으로 표시한다. (별도의 예외처리를 하기에는 꼬일 가능성이 많다)
	else
	{
		COLORREF colorMsg;
		
		// [검사결과 : GOOD]
		if ( nClass == CELL_LOG_OK )
		{
			theSockMgr.AddPGMsg( TEXT_OK );				// "OK"
			colorMsg = RGB(0 , 0xFF, 0);				// 글자색상 Green 지정
		}

		// [검사결과 : NG ]
		else 
		{	
			theSockMgr.AddPGMsg( strDefect );			// "불량명"
			colorMsg = RGB(0xFF, 0, 0);					// 글자색상 Red 지정
		}//end of else
				

		theSockMgr.SendMsgToPGMsg(nJig, nCh, RGB(0, 0, 0), colorMsg, 10, 10, (int)dSize);	// 신호기에 명령한다.
		theSockMgr.ClearPGMsg();															// 초기화 진행
	} //end of Disp : Message 
}


// 미사용
// D-Zone 검사시, 제품 화면에 검사 결과를 표시한다.
void CMCMain::PG_DrawCellResult(TCellData* pCell )
{
	return;

	if ( pCell == NULL ) return;

	COLORREF colorMsg;
	CString strDefect, sCode;
	int nClass			= CELL_LOG_NG;
	int nFinalJudge	= GetDefectFromJudge(eIDX_PG_ZONE_Final, pCell, strDefect, nClass, sCode);		// 지금까지의 최종 판정을 조회

	// 양품이면 녹색으로 'OK' 표시
	if ( nClass == CELL_LOG_OK )
	{
		theSockMgr.AddPGMsg(TEXT_OK);
		colorMsg = RGB(0 , 0xFF, 0);
	}
	else // 그 외는 빨간색으로 불량이 발생한 검사명을 출력한다.
	{
		theSockMgr.AddPGMsg( strDefect );	// CONST_JUDGE_LIST[nFinalJudge].strName);
		colorMsg = RGB(0xFF, 0, 0);
	}

	// 글자 크기는 고정비율로 계산... [8/27/2013 OSC]
	double dSize = (m_rCellDisp.nResolutionX * 1.0) * MSG_DISPLAY_RATE;
	theSockMgr.SendMsgToPGMsg(pCell->JigNo, pCell->ChNo, RGB(0, 0, 0), colorMsg, 10, 10, (int)dSize);
	theSockMgr.ClearPGMsg();
}


// D-Zone 검사시, VI테스트시  발견된 Defect위치에 원을 표시해준다.
void CMCMain::PG_DrawCellDefectCircle( TCellData* pCell )	
{
	return;

	if (pCell == NULL) return;
	if (pCell->Bin2Defect == FALSE) return;

	int nCount = 0;
	COLORREF colorBack	= RGB(255, 255, 255);		// 흰 색 바탕
	COLORREF colorFore	= RGB(0, 0, 0);				// 검은색 원

	// 2014-11-27, jhLee, Defect Circle의 배경색은 흰색, Circle color는 검은색으로 지정한다. 김상묵 사원
	// 단 1개의 패턴만 보여준다
	//
	//	// 주어진 패턴에 대한 생삭을 얻어온다.
	//	GetPatternColor(pCell->sCirclePtnName[nPtnNo], colorBack, colorFore);


	CIRCLE_DATA data;
	POSITION pos = pCell->PatternCircle.GetHeadPosition();
	while (pos)
	{
		data = pCell->PatternCircle.GetNext(pos);						// 좌표 데이터를 조회한다.
		
		//2015-05-16 JH-PARK 
		//Circl 구현 관련 예외처리 추가 ( 신호기 Error )

		// x,y의 좌표가 신호기에서 허용할수 값이 나올경우 해당 신호기CH에서 Pixel Position Error가 발생하면서 해당 CH에 대한 신호기 제어가 순간적으로 제어 불가능 하게 된다 
		// Piexel Error에 대한 처리과정을 진행하는동안 신호기 응답 및 제어가 불가능

		// Circle을 구현할수 최대 범위 ( X : 1440 , Y : 2560 )

		if(data.x > 1440 || data.y > 2560) 
			continue;						// 해당 좌표에 대해서는 Circle을 구현하지 않는다. 
		
		theSockMgr.AddPGCircle(colorFore, data.x, data.y);
				
		// 정해진 수량만큼만 Circle을 그린다.
		if ( ++nCount >= m_rProfileData.nCNT_DispCircleCount ) break;

	}

	// PGPC에 전송
	// theSockMgr.SetPGCircleBackColor( RGB( 255, 255, 255 ) );			// 일단은 흰색 바탕에 그리도록 한다.

	theSockMgr.SetPGCircleBackColor( colorBack );							// 지정된 색상으로 배경을 지정한다.
	theSockMgr.SendMsgToPGCircle(pCell->JigNo, pCell->ChNo);
	theSockMgr.ClearPGCircle();
}

*/
//PG end


// TCP/IP 통신 상태를 확인한다 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

// 지정 TCP/IP 통신의 연결 상태를 조회한다.
BOOL CMCMain::GetCommConnected(int nIdx)
{
	return TRUE;	//COMM theSockMgr.GetCommConnected(nIdx);				// 연결 상태를 조회한다.
}


// 통신 상태를 점검하여 오류내용을 되돌린다.
int CMCMain::GetCommConnectCheck()
{
	CMainFrame* pMainFrm = (CMainFrame*)::AfxGetMainWnd();
	::PostMessage(pMainFrm->m_hWnd, UDMSG_COMM_STATE, 0, 0);		// 연결 상태 Update 요청

	// 가상 실행 모드일 경우 이상이 없다고 지정한다.
	if ( (m_bIsSimulate == TRUE) || (m_bIsDryRun == TRUE) )
	{
		return ERR_NONE;
	}

	// 각 통신 상태에 따른 오류값을 조회한다.

//COMM begin
	//// 1. Vision PC
	//if ( theSockMgr.GetCommConnected( eCOMM_AVCamera ) != TRUE )
	//{
	//	return ERR_COMM_VISIONPC;
	//}

	//// 2. Data-PC
	//if ( theSockMgr.GetCommConnected( eCOMM_DataPC ) != TRUE )
	//{
	//	// 상위 통신을 하지 않는다고 지정되어있지 않다면 오류 발생
	//	//d if ( (m_rProfileData.bUse_SkipCIMSend != TRUE) || (m_rProfileData.bUseTest_BIN2TestMode != TRUE) )
	//	if ( m_rProfileData.bUseTest_BIN2TestMode != TRUE )
	//	{
	//		return ERR_COMM_DATAPC;
	//	}
	//}

	//// 3. 신호기 PC
	//if ( theSockMgr.GetCommConnected( eCOMM_SignalPC ) != TRUE )
	//{
	//	return ERR_COMM_PGPC;
	//}

	//// 4. UV-MCR 1
	//if ( theSockMgr.GetCommConnected( eCOMM_UVMCR1 ) != TRUE )
	//{
	//	return ERR_COMM_UVMCR1;
	//}

	//// 5. UV-MCR 2
	//if ( theSockMgr.GetCommConnected( eCOMM_UVMCR2 ) != TRUE )
	//{
	//	return ERR_COMM_UVMCR2;
	//}
//COMM  end

	return ERR_NONE;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
// 신호기PC 관련

// 지정 Jig에서 신호기로 TurnOn 명령을 보낸 상태인가 /
BOOL CMCMain::GetPGTurnOnSendCheck(int nJig)
{
	if ( (nJig < 0) || (nJig >= eMAX_JigCount) )	return FALSE;		// Jig 번호 오류

	BOOL bResult = TRUE;			// Cell 투입없이 양수 버튼을 눌렀을때에도 정상적으로 다음 스텝으로 넘어가기위한 초기값
	BOOL bFlag = FALSE;
	TCellData *pCell = NULL;

	for (int i = 0; i < eMAX_ChCount; i++)
	{
		pCell = m_pCellInfo->GetCellData(nJig, i);
		if ( pCell == NULL ) continue;

		if ( pCell->bIsExist == TRUE )	// Cell이 있는 경우에만 조사. 즉, 양쪽 채널 모두 Cell이 존재하지 않는다면 TRUE를 되돌려준다.
		{
			// 신호기로 Turn On 명령을 전송한 이력이 없거나 현재 TURN ON 명령을 보낸 상태가 아니라면
			if ( (pCell->TurnOnStartFlag == FALSE) || ( m_rProfileData.bPGContactFlag[ nJig ][ i ] == FALSE ) )
			{
				bResult = FALSE;		// 정상적으로 TURN ON 명령 전송 상태가 아니다.
			}
		}
	}

	return bResult;
}


// PG Control : Next/Back/On-Off 제어
BOOL CMCMain::PG_AZonePGControl(int nCmd)
{
	if ( GetPanelButtonLock() == TRUE ) return FALSE;		// 버튼이 잠겨있으면 무시
	CSvcTcpPG* pSvcTcpPG	= (CSvcTcpPG*)( (CGxServiceHub::GetInstance())->GetService(NAME_SERVICE_TcpPG));						// 신호기 통신

	CSTNWorkTable* pWorkTable = (CSTNWorkTable *)((CGxStationHub::GetInstance())->GetStation( NAME_STATION_WorkTable ));			// WorkTable 포인터 조회
	if ( pWorkTable == NULL ) return FALSE;

	int nJig = pWorkTable->GetAZoneJig();						// 현재 A-Zone에 있는 Jig
	BOOL bFlag;

	switch ( nCmd )
	{
		case eBTN_PG_ONOFF	:	
			bFlag = pSvcTcpPG->GetPGContactJigFlag( nJig );				// 현재 지그의 Contact 현황

			// 현재 출력의 반대로 내보내 준다.
			if ( bFlag == FALSE )		// 현재 OFF 상태라면,
			{
				bFlag = TRUE;				// ON 시키도록 반전시켜준다.
			}
			else
				bFlag = FALSE;				// OFF 시키도록 한다. 

			pSvcTcpPG->PG_SendTurnOnOff(nJig, bFlag);					// 지정 지그의 TURN_ON/OFF 전송
			return TRUE;

		case eBTN_PG_BACK		:
			//
			// 무한 순환 반복을 방지한다.
			//
			// PG_SendBack( nJig );
			SetBackPatternDisplay( nJig );

			return TRUE;


		case eBTN_PG_NEXT		:
			//
			// 무한 순환 반복을 방지한다.
			//
			// PG_SendNext( nJig );									// 지정 지그의 NEXT 전송
			SetNextPatternDisplay( nJig );

			return TRUE;
	}//of switch

	return FALSE;
}


// Next / Back 버튼 명령에대한 응답이 수신되었나 ?
BOOL CMCMain::GetRcvBackNext()
{
	CSTNWorkTable* pWorkTable = (CSTNWorkTable *)((CGxStationHub::GetInstance())->GetStation( NAME_STATION_WorkTable ));			// WorkTable 포인터 조회
	if ( pWorkTable == NULL ) return FALSE;

	int i;
	int nJig = pWorkTable->GetAZoneJig();						// 현재 A-Zone에 있는 Jig
	TCellData *pCell;

	BOOL bResult = TRUE;												// 초기값은 응답 수신이 되었다고 간주

	for (i = 0; i < eMAX_ChCount; i++)
	{
		pCell = m_pCellInfo->GetCellData(nJig, i);

		if ( pCell == NULL ) continue;
		if ( pCell->bIsExist == FALSE ) continue;				// Cell이 없으면 통과

		if ( pCell->bRcvBackNextCmd != TRUE )					// 1개라도 수신이 안되었다면 
		{
			bResult = FALSE;									// 응답 미수신으로 판정한다.
			break;
		}
	}

	return bResult;
}




// Cell Info data에 현재 위치 Zone을 지정 Zone으로 지정한다.
//
// int nJig : 0 ~ 1,
// int nZone : INSP_ZONE		nCurrentZone;		// 현재 어느 Zone에 와 있는가 ?
void CMCMain::SetCellZoneChange(int nJig, int nZone)
{
	int i;
	TCellData *pCell;

	for (i = 0; i < eMAX_ChCount; i++)
	{
		pCell = m_pCellInfo->GetCellData(nJig, i);
		if ( pCell == NULL ) continue;

		//if ( pCell->bIsExist == TRUE )					// Cell이 있는 경우에만 실행
		//{

			pCell->nCurrentZone = nZone;

		//}
	}
}

//
////
//// 지정 JIG의 Cell로 Contact Current 결과를 출력한다.
////
//void CMCMain::PG_SendContactCurrentResult(int nJig, int nCh)
//{
//	TCellData *pCell = m_pCellInfo->GetCellData(nJig, nCh);
//	if ( pCell == NULL ) return;
//	if ( pCell->nCurrentZone != ZONE_A ) return;		// A-Zone이 아니라면 실행하지 말아야 한다.
//
//	if ( pCell->bIsExist == TRUE )					// Cell이 있는 경우에만 실행
//	{
//		// Contact Current 결과에 따라서 출력 내용을 달리한다.
//		if ( pCell->ContactCurrentClass == eGOOD_CELL )
//		{
//			theSockMgr.SendMsgToPG(PG_CONTACT_OK_MSG, nJig, nCh);		// 통과
//		}
//		else
//			if ( pCell->ContactCurrentClass == LOSS_CELL )		// Contact Current 결과 NG
//			{
//				PG_DrawCellText(nJig, nCh, _T("Contact Current NG"));
//			}
//
//		// 아직 결과가 도착하지 않았다면 표시하지 않는다.
//	}//of if Cell Exist
//}
//
//
//
//
//// 지정 Zone으로 지정한다.
////
//// CString sZone : SET_ZONE_A / SET_ZONE_C /SET_ZONE_D
//// int nJig : 0 ~ 1,
//void CMCMain::PG_SendZoneChange(CString sZone, int nJig)
//{
//	int i;
//	TCellData *pCell;
//
//	for (i = 0; i < eMAX_ChCount; i++)
//	{
//		pCell = m_pCellInfo->GetCellData(nJig, i);
//		if ( pCell == NULL ) continue;
//
//		if ( pCell->bIsExist == TRUE )					// Cell이 있는 경우에만 실행
//		{
//
//			theSockMgr.SendMsgToPG(sZone, nJig, (JIG_CH)i);
//		}
//	}
//}
//
//// 지정 채널의 Zone을 직접 지정한다.
//void CMCMain::PG_SendZoneChange(CString sZone, int nJig, int nCh)
//{
//	theSockMgr.SendMsgToPG(sZone, nJig, nCh);
//}
//
//
//// 지정 Jig의 TMD_INFO를 요청한다.
////
//// int nJig : 0 ~ 1,
//void CMCMain::PG_SendTMDInfo(int nJig)
//{
//	int i;
//	TCellData *pCell;
//
//	for (i = 0; i < eMAX_ChCount; i++)
//	{
//		pCell = m_pCellInfo->GetCellData(nJig, i);
//		if ( pCell == NULL ) continue;
//
//		if ( pCell->bIsExist == TRUE )					// Cell이 있는 경우에만 실행
//		{
//			theSockMgr.SendMsgToPG(TMD_INFO, nJig, (JIG_CH)i);
//		}
//	}
//}
//
//
//
//// 2015-07-16, jhLee, 신호기에게 모든 채널의 TMD 정보를 요청한다. (초기화시에 수행)
//BOOL CMCMain::PG_SendTMDInfoRequest()
//{
//	int i, j;
//	BOOL bResult = TRUE;
//
//	for (i = 0; i < eMAX_JigCount; i++)
//	{
//		for (j = 0; j < eMAX_ChCount; j++)
//		{
//			// 한 채널이라도 명령 전송에 실패하면 
//			if ( theSockMgr.SendMsgToPG(TMD_INFO, i, j) == FALSE )
//			{
//				bResult = FALSE;
//			}
//
//			SleepEx(50, NULL);			// 약간의 시간을 지연해가며 전송한다.
//		}
//	}
//
//	return bResult;
//}
//
//
//// 지정 지그의 CELL_LOADING 명령을 전송한다.
////
//void CMCMain::PG_SendCellLoading(int nJig)
//{
//	int i;
//	TCellData *pCell;
//
//	for (i = 0; i < eMAX_ChCount; i++)
//	{
//		pCell = m_pCellInfo->GetCellData(nJig, i);
//		if ( pCell == NULL ) continue;
//
//		if ( pCell->bIsExist == TRUE )					// Cell이 있는 경우에만 실행
//		{
//			if ( (m_bIsSimulate == TRUE) || (m_bIsDryRun == TRUE) )
//			{
//				pCell->RecvCellLoading	= TRUE;
//				pCell->ContactCurrentClass		= eGOOD_CELL;
//				pCell->CellLoadingClass = eGOOD_CELL;
//
//				::GetLocalTime( &pCell->cellTime.systmA_ContactStartTime );
//				::GetLocalTime( &pCell->cellTime.systmA_ContactEndTime );
//			}
//			else
//			{	// else if(pCell->RecvCellLoading == FALSE)
//				// 초기화를 수행하고 요청을 보낸다.
//				pCell->RecvCellLoading	= FALSE;
//				pCell->CellLoadingClass = NONE_CELL;
//
//				theSockMgr.SendMsgToPG(CELL_LOADING, nJig, (JIG_CH)i, pCell->CellID);
//			}
//		}//of if Exist
//	}//of for i
//}
//
//
//
//
//
//// 지정 지그의 TURN_ON / OFF 전송
//void CMCMain::PG_SendTurnOnOff(int nJig, BOOL bFlag)
//{
//	int i;
//	TCellData *pCell;
//	BOOL bFirstFlag = FALSE;							// 최초의 TurnOn 신호 전송인가 ?
//
//	for (i = 0; i < eMAX_ChCount; i++)
//	{
//		pCell = m_pCellInfo->GetCellData(nJig, i);
//		if ( pCell == NULL ) continue;
//
//		if ( pCell->bIsExist == TRUE )					// Cell이 있는 경우에만 실행
//		{
//			pCell->bRcvPowerOnOffCmd = FALSE;			// ACK 응답 수신 초기화
//
//			if ( bFlag == TRUE )						// Turn On 명령
//			{
//				// 2015-07-10, jhLee, 최초의 신호기 Turn On 명령인가 ?
//				if ( pCell->TurnOnStartFlag == FALSE )	// 아직 신호기 power on 요청을 하지 않은 상태라면,
//				{
//					 pCell->TurnOnStartFlag = TRUE;		// 이제 신호를 보냈다.
//					 bFirstFlag = TRUE;					// 최초 Turn on TACT Time 처리
//				}
//			
//
//				//2015-03-28, jhLee, TACT을 앞당기기 위해 PowerON 명령을 대신하여 미리 Cell Loading 명령을 지령한다. -> 아직 미적용(2015-07-10)
//				// 옵션이 지정되었다면, 
//				if ( m_rProfileData.bUse_CellLoadingCmd == TRUE )
//				{
//					// 새로운 방식인 CELL LOADING 명령을 전송한다.
//					pCell->RecvCellLoading	= FALSE;
//					pCell->CellLoadingClass = NONE_CELL;
//
//					theSockMgr.SendMsgToPG(CELL_LOADING, nJig, (JIG_CH)i, pCell->InnerID);		// 아직 UV-MCR을 읽지 못하였기 때문에 Cell ID 대신에 InnerID를 사용한다.
//				}
//				else	// 기존방식 사용
//				{
//					theSockMgr.SendMsgToPG(TURN_ON, nJig, JIG_CH(i));		// 이전 방식으로 POWER ON 명령을 보낸다.
//				}
//
//			}
//			else
//				theSockMgr.SendMsgToPG(TURN_OFF, nJig, JIG_CH(i));		// Turn off 명령
//
//
//			// 해당 채널의 Contact 여부를 지정하는 Flag에 반영
//			// nIdx = (nJig * eMAX_JigCount) + i;
//			SetPGContactFlag(nJig, i, bFlag);
//		}
//
//	}
//
//	// 2015-07-08, jhLee, TACT Time 항목중에 신호기 TurnOn 항목이 새로이 추가되었다.
//	// 최초의 TurnOn 신호 전송인가 ?
//	if ( bFirstFlag == TRUE ) 
//	{
//		// 최초로 TurnOn을 지령하는 시각을 저장한다.
//		SetTACTTime(nJig, eTACT_TurnOn );		// 최초로 신호기로 TURNON을 전송한 시각, 작업자 검사 시작  );
//	}
//}
//
//
//
//// 지정 지그 채널에 TURN_ON / OFF 전송
//void CMCMain::PG_SendTurnOnOff(int nJig, int nCh, BOOL bFlag)
//{
//
//	if ( bFlag )
//	{
//		theSockMgr.SendMsgToPG(TURN_ON, nJig, nCh);			// POWER ON 명령을 보낸다.
//	}
//	else
//		theSockMgr.SendMsgToPG(TURN_OFF, nJig, nCh);		// Turn off 명령
//
//
//	// 해당 채널의 Contact 여부를 지정하는 Flag에 반영
//	// nIdx = (nJig * eMAX_JigCount) + i;
//	SetPGContactFlag(nJig, nCh, bFlag);
//}
//
//
//
//// 2015/05/15 JH-PARK
//// 지정 지그의 CH에 NEXT 전송
//void CMCMain::PG_SendNext(int nJig, int nCh)
//{
//	TCellData *pCell;
//	
//	pCell = m_pCellInfo->GetCellData(nJig, nCh);
//
//	if ( pCell == NULL ) return;
//
//	if ( pCell->bIsExist == TRUE )					// Cell이 있는 경우에만 실행
//	{
//		pCell->bRcvBackNextCmd = FALSE;				// ACK 응답 수신 초기화
//
//		theSockMgr.SendMsgToPG(NEXT, nJig, JIG_CH(nCh));
//	}
//
//}
//
//
//// 지정 지그의 NEXT 전송
//void CMCMain::PG_SendNext(int nJig)
//{
//	int i;
//	TCellData *pCell;
//
//	for (i = 0; i < eMAX_ChCount; i++)
//	{
//		pCell = m_pCellInfo->GetCellData(nJig, i);
//		if ( pCell == NULL ) continue;
//
//		if ( pCell->bIsExist == TRUE )					// Cell이 있는 경우에만 실행
//		{
//			pCell->bRcvBackNextCmd = FALSE;				// ACK 응답 수신 초기화
//
//			theSockMgr.SendMsgToPG(NEXT, nJig, JIG_CH(i));
//		}
//	}
//}
//
//// 지정 지그의 Back 전송
//void CMCMain::PG_SendBack(int nJig)
//{
//	int i;
//	TCellData *pCell;
//
//	for (i = 0; i < eMAX_ChCount; i++)
//	{
//		pCell = m_pCellInfo->GetCellData(nJig, i);
//		if ( pCell == NULL ) continue;
//
//		if ( pCell->bIsExist == TRUE )					// Cell이 있는 경우에만 실행
//		{
//			pCell->bRcvBackNextCmd = FALSE;				// ACK 응답 수신 초기화
//
//			theSockMgr.SendMsgToPG(BACK, nJig, JIG_CH(i));
//		}
//	}
//}
//
//// 2016-05-23, jhLee, 
//// 개별 채널의 BACK 버튼
//void CMCMain::PG_SendBack(int nJig, int nCh)
//{
//	TCellData *pCell;
//	
//	pCell = m_pCellInfo->GetCellData(nJig, nCh);
//
//	if ( pCell == NULL ) return;
//
//	if ( pCell->bIsExist == TRUE )					// Cell이 있는 경우에만 실행
//	{
//		pCell->bRcvBackNextCmd = FALSE;				// ACK 응답 수신 초기화
//
//		theSockMgr.SendMsgToPG(BACK, nJig, JIG_CH(nCh));
//	}
//
//}
//
//// 지정 지그의 Reset 전송
//// Reset은 셀의 유무와 상관없이 진행한다.
//void CMCMain::PG_SendReset(int nJig)
//{
//	int i;
//
//	for (i = 0; i < eMAX_ChCount; i++)
//	{
//		theSockMgr.SendMsgToPG(RESET, nJig, JIG_CH(i));
//
//		SetPGContactFlag(nJig, i, FALSE);			// 현재 지그의 Contact이 OFF 되었다.
//	}
//}


//? 적용 ?
//
	//// 지정 지그의 Reset 전송 2
	//// C-Zone에서 미리 Contact을 풀어서 넘기기 위한 함수 추가로, Reset은 셀의 유무와 상관없이 진행한다.
	//void CMCMain::PG_SendResetForAutoContact(int nJig)
	//{
	//	int i;
	//	TCellData *pCell;

	//	for (i = 0; i < eMAX_ChCount; i++)
	//	{
	//		pCell = m_pCellInfo->GetCellData(nJig, i);
	//		if ( pCell == NULL ) continue;

	//		theSockMgr.SendMsgToPG(RESET, nJig, i);		// 지정 채널에 Reset을 전송하고 
	//		SetPGContactFlag(nJig, i, FALSE);			// 현재 지그의 Contact이 OFF 되었다.

	//		pCell->bIsAutoContactOff	= TRUE;			// C-Zone 자동 Contact off 처리 표시
	//	}
	//}

	////
	//// 지정 지그의 C-Zone Contact off 기능 수행했는지 조사한 후 필요시 Reset을 수행한다.
	//void CMCMain::PG_SendResetCheckAutoContact(int nJig)
	//{
	//	int i;
	//	TCellData *pCell;

	//	for (i = 0; i < eMAX_ChCount; i++)
	//	{
	//		pCell = m_pCellInfo->GetCellData(nJig, i);
	//		if ( pCell == NULL ) continue;

	//		// C-Zone에서 미리 Contact을 풀어준 경우가 아니라면, Reset을 전송한다.
	//		if ( pCell->bIsAutoContactOff == FALSE )
	//		{
	//			theSockMgr.SendMsgToPG(RESET, nJig, i);		// 지정 채널에 Reset을 전송하고 
	//			SetPGContactFlag(nJig, i, FALSE);			// 현재 지그의 Contact이 OFF 되었다.
	//		}
	//	}


	//}

//
//
//// 지정 지그 / 채널에게 Reset 전송
//// Reset은 셀의 유무와 상관없이 진행한다.
//void CMCMain::PG_SendChReset(int nJig, int nCh)
//{
//	theSockMgr.SendMsgToPG(RESET, nJig, JIG_CH(nCh));
//	SetPGContactFlag(nJig, nCh, FALSE);				// 현재 지그의 Contact이 OFF 되었다.
//
//	// 가종 PG-ERROR에 대한 Clear 동작은 
//	// 신호기로부터 REST에 대한 완료 동작을 받은 후 수행하게 된다.
//
//
//	//TCellData *pCell = m_pCellInfo->GetCellData(nJig, nCh);
//	//if ( pCell != NULL )
//	//{
//	//	pCell->PG_SystemError = TRUE;				// System Error 해소
//
//	//}
//
//}
//
//
//// 2016-04-21, jhLee, 기능 추가
//// TSP 검사를 지령한다.
//void CMCMain::PG_SendTSPStart(int nJig, int nCh)
//{
//	TCellData *pCell;
//
//	pCell = m_pCellInfo->GetCellData(nJig, nCh);
//	if ( pCell == NULL ) return;
//
//	if ( pCell->bIsExist == TRUE )					// Cell이 있는 경우에만 실행
//	{
//		// 검사 요청시간 설정
//		::GetLocalTime(&pCell->cellTime.systmC_TspStartTime);
//
//		// Test mode
//		if ( (m_bIsSimulate == TRUE) || (m_bIsDryRun == TRUE) )
//		{
//			pCell->InspTspState = INSP_END;
//			pCell->TspClass = eGOOD_CELL;
//			pCell->TspNGMainName = TEXT_GOOD;
//			pCell->evtTSPEnd.Set();											// 종료 지정
//
//			::GetLocalTime(&pCell->cellTime.systmC_TspEndTime);
//		}
//		else
//			if ( pCell->IsPGAbnormal() == TRUE )	// 신호기에 이상이 있는지 여부
//			{	
//				// 검사를 Skip 시킨다.
//				pCell->InspTspState = INSP_NONE;
//				pCell->TspClass		= BIN2_CELL;
//				pCell->TspNGMainName = TEXT_SKIP;
//				pCell->evtTSPEnd.Set();											// 종료 지정
//
//				::GetLocalTime(&pCell->cellTime.systmC_TspEndTime);
//			}
//			else
//				theSockMgr.SendMsgToPG(TSP_START, nJig, nCh);
//	}//of if Exist
//	else
//		pCell->evtTSPEnd.Set();												// 종료 지정
//
//}
//
//
////  현재 미사용, 지정 지그의 Sleep Current start 명령을 전송한다.
////
//void CMCMain::PG_SendSleepCurrentStart(int nJig)
//{
//	int i;
//	TCellData *pCell;
//
//	for (i = 0; i < eMAX_ChCount; i++)
//	{
//		pCell = m_pCellInfo->GetCellData(nJig, i);
//		if ( pCell == NULL ) continue;
//
//		if ( pCell->bIsExist == TRUE )					// Cell이 있는 경우에만 실행
//		{
//			::GetLocalTime(&pCell->cellTime.systm_SleepCurrentStartTime);
//
//			if ( (m_bIsSimulate == TRUE) || (m_bIsDryRun == TRUE) )
//			{
//				pCell->RecvSleepCurrnet = TRUE;
//				pCell->SleepCurrentClass = LOSS_CELL;
//				::GetLocalTime(&pCell->cellTime.systm_SleepCurrentEndTime);
//			}
//			else
//				if ( pCell->IsPGAbnormal() == TRUE )	// 신호기에 이상이 있는지 여부
//				{	
//					// 검사를 Skip 시킨다.
//					pCell->RecvSleepCurrnet = TRUE;
//					pCell->SleepCurrentClass = BIN2_CELL;
//					::GetLocalTime( &pCell->cellTime.systm_SleepCurrentEndTime );
//				}
//				else
//					theSockMgr.SendMsgToPG(SLEEP_CURRENT_SND, nJig, (JIG_CH)i);
//
//		}//of if Exist
//	}//of for i
//}
//
//
//// ID_CHECK ( White current ) 측정 명령
//void CMCMain::PG_SendIDCheckStart(int nJig)
//{	
//	int i;
//	TCellData *pCell;
//
//	for (i = 0; i < eMAX_ChCount; i++)
//	{
//		pCell = m_pCellInfo->GetCellData(nJig, i);
//		if ( pCell == NULL ) continue;
//
//		if ( pCell->bIsExist == TRUE )					// Cell이 있는 경우에만 실행
//		{
//			::GetLocalTime(&pCell->cellTime.systmC_WhiteCurrentStartTime);
//
//			if ( (m_bIsSimulate == TRUE) || (m_bIsDryRun == TRUE) )
//			{
//				pCell->RecvWhiteCurrnet = TRUE;
//				pCell->WhiteCurrentClass = LOSS_CELL;
//				::GetLocalTime(&pCell->cellTime.systmC_WhiteCurrentEndTime);
//			}
//			else
//				if ( pCell->IsPGAbnormal() == TRUE )	// 신호기에 이상이 있는지 여부
//				{	
//					// 검사를 Skip 시킨다.
//					pCell->RecvWhiteCurrnet = TRUE;
//					pCell->WhiteCurrentClass = BIN2_CELL;
//					::GetLocalTime( &pCell->cellTime.systmC_WhiteCurrentEndTime );
//				}
//				else
//					theSockMgr.SendMsgToPG(ID_CHECK, nJig, (JIG_CH)i);
//
//		}//of if Exist
//	}//of for i
//}
//
//
//
////
//// 2015-03-27, jhLee, C-Zone에서 TE_CHECK 기능을 추가한다.
////
//BOOL CMCMain::PG_SendTECheck(int nJig, int nCh)
//{
//	TCellData *pCell;
//
//	pCell = m_pCellInfo->GetCellData(nJig, nCh);
//	if ( pCell == NULL ) return FALSE;
//
//
//	if ( pCell->bIsExist == TRUE )					// Cell이 있는 경우에만 실행
//	{
//		// 검사 요청시간 설정
//		pCell->SetTACTTime( eTACT_PG_TECheckStart );
//
//		// Test mode
//		if ( (m_bIsSimulate == TRUE) || (m_bIsDryRun == TRUE) )
//		{
//			pCell->TE_CheckJudge	= TRUE;										// 합격
//			pCell->TE_CheckState	= INSP_END;									// 검사 진행 완료
//			pCell->evtTECheckEnd.Set();											// 종료 지정
//
//			// 검사 종료 시각
//			pCell->SetTACTTime( eTACT_PG_TECheckEnd );
//		}
//		else
//			if ( pCell->IsPGAbnormal() == TRUE )								// 신호기에 이상이 있는지 여부, 이상이 있다.
//			{	
//				// 검사를 Skip 시킨다.
//				pCell->TE_CheckJudge	= FALSE;								// 불합격
//				pCell->TE_CheckState	= INSP_NONE;							// 검사를 진행하지 않는다.
//				pCell->evtTECheckEnd.Set();											// 종료 지정
//	
//				// 검사 종료 시각 지정
//				pCell->SetTACTTime( eTACT_PG_TECheckEnd );
//			}
//			else
//				return theSockMgr.SendMsgToPG(TE_CHECK, nJig, nCh);				// TE_CHECK 수행 명령을 전송한다.
//
//	}//of if Exist
//	else
//		pCell->evtTECheckEnd.Set();												// 종료 지정
//
//
//
//	return TRUE;
//}
//
//
//// 2015-06-02, jhLee, D-Zone에서 표시하는 패턴을 지정해준다.
//BOOL CMCMain::PG_SendPatternSelect(int nJig, int nCh, int nIdx)
//{
//	TCellData *pCell;
//
//	pCell = m_pCellInfo->GetCellData(nJig, nCh);
//	if ( pCell == NULL ) return FALSE;
//
//	if ( pCell->bIsExist == TRUE )					// Cell이 있는 경우에만 실행
//	{
//		CString sData;
//		sData.Format(_T("%d"), nIdx);				// 지정 Index를 문자열로 변환한다.
//
//		return theSockMgr.SendMsgToPG(PATTERN_SELECT, nJig, nCh, sData);				// TE_CHECK 수행 명령을 전송한다.
//	}
//
//	return FALSE;
//}
//
//


// 완료 여부 check 함수

// 지정 지그의 신호기 상태가 정상적인지 여부
int CMCMain::GetPGErrorCheck(int nJig)
{
	int i;
	TCellData *pCell;

	for (i = 0; i < eMAX_ChCount; i++)
	{
		pCell = m_pCellInfo->GetCellData(nJig, i);

		if ( pCell->bIsExist == TRUE )					// Cell이 있는 경우에만 실행
		{
			// 1개라도 신호기에 이상이 있다면,
			if (pCell->IsPGAbnormal() == TRUE )
			{
				return ERR_PALIGN_PG_ERROR1 + i;			// Error Code를 되돌린다.
			}
		}
	}//of for i

	return ERR_NONE;
}


// 지정 지그의 Contact Current 완료 여부
int CMCMain::GetPGContactCurrentCheck(int nJig)
{
	int i;
	TCellData *pCell;

	for (i = 0; i < eMAX_ChCount; i++)
	{
		pCell = m_pCellInfo->GetCellData(nJig, i);

		if ( pCell->bIsExist == TRUE )					// Cell이 있는 경우에만 실행
		{
			// 1개라도 수신받지 못한 channel이 있다면 아직 수신완료하지 못함
			if (pCell->ContactCurrentClass != eGOOD_CELL )
			{
				// 2015-03-04, jhLee, 김상묵 사원 요청, A-Zone에서 작업자가 Defect을 입력하였다면 Contact current를 통화하지 못하였더라도 다음으로 통과 시킨다. 
				//						(전산 처리를 위해)
				if ( (pCell->AZoneManualDefect != _T("")) || (pCell->AZoneManualClass == eLOSS_CELL) )							// A-Zone Defect이 입력되었나
				{
					//A-Zone에서 불량 입력된 Cell 이다, 
					// 마치 Contact Current가 통과된 것처럼 진행한다.

					//pCell->ResultAZoneManualName	= pDefect->sDefect[eLANG_KOREAN];				// 첫번째 오는 Defect 이름을 지정한다.
					//pCell->PrintName				= pDefect->sDefect[eLANG_ENGLISH];				// Label Printer는 영어로 출력한다.
					//pCell->AZoneManualClass		= LOSS_CELL;									// Recject 발생
					//pCell->AZoneDefectCode		= pDefect->sCode;								// 수동검사 Defect Code, CIM 보고용
					//pCell->MesCode				= pDefect->sCode;								// 수동검사 Defect Code, CIM 보고용, jhLee 추가, (2014-08-6)
				}
				else
					return ERR_PALIGN_PG_CONTACT_CURRENT1 + i;			// Error Code를 되돌린다.
			}
		}
	}//of for i

	return ERR_NONE;
}


// 지정 지그의 Cell loading 완료 여부
int CMCMain::GetPGCellLoadingCheck(int nJig)
{
	int i;
	TCellData *pCell;

	for (i = 0; i < eMAX_ChCount; i++)
	{
		pCell = m_pCellInfo->GetCellData(nJig, i);

		if ( pCell->bIsExist == TRUE )					// Cell이 있는 경우에만 실행
		{
			// 1개라도 수신받지 못한 channel이 있다면 아직 수신완료하지 못함
			if  (pCell->CellLoadingClass != eGOOD_CELL )
			{
				return ERR_PALIGN_PG_CELL_LOADING1 + i;			// Error Code를 되돌린다.
			}
		}
	}//of for i

	return ERR_NONE;
}


// 지정 지그의 Cell loading 완료 여부
BOOL CMCMain::Is_PGCellLoadingCheck(int nJig)
{
	int i;
	TCellData *pCell;

	for (i = 0; i < eMAX_ChCount; i++)
	{
		pCell = m_pCellInfo->GetCellData(nJig, i);
		if ( pCell == NULL ) continue;

		if ( pCell->bIsExist == TRUE )					// Cell이 있는 경우에만 실행
		{
			// 1개라도 수신받지 못한 channel이 있다면 아직 수신완료하지 못함
			if(pCell->CellLoadingClass != eGOOD_CELL)
			{
				return FALSE;
			}
		}
	}//of for i

	return TRUE;
}


// TSP 완료 여부
BOOL CMCMain::Is_PGTSPEndCheck(int nJig)
{
	int i;
	TCellData *pCell;

	for (i = 0; i < eMAX_ChCount; i++)
	{
		pCell = m_pCellInfo->GetCellData(nJig, i);
		if ( pCell == NULL ) continue;

		if ( pCell->bIsExist == TRUE )					// Cell이 있는 경우에만 실행
		{
			// 1개라도 수신받지 못한 channel이 있다면 아직 수신완료하지 못함
			if(pCell->TspInspState != eINSP_END)
			{
				return FALSE;
			}
		}
	}//of for i

	return TRUE;
}

//
//// Sleep Current 완료 여부
//BOOL CMCMain::Is_PGSleepCurrentEndCheck(int nJig)
//{
//	int i;
//	TCellData *pCell;
//
//	for (i = 0; i < eMAX_ChCount; i++)
//	{
//		pCell = m_pCellInfo->GetCellData(nJig, i);
//		if ( pCell == NULL ) continue;
//
//		if ( pCell->bIsExist == TRUE )					// Cell이 있는 경우에만 실행
//		{
//			// PG 이상발생 처리
//			if( (pCell->PG_DownZone != eIDX_PG_ZONE_Final) && (pCell->RecvSleepCurrnet == FALSE) )
//			{
//				pCell->RecvSleepCurrnet = TRUE;
//				pCell->SleepCurrentClass = LOSS_CELL;
//				::GetLocalTime( &pCell->cellTime.systm_SleepCurrentEndTime );
//			}
//
//			// 1개라도 수신받지 못한 channel이 있다면 아직 수신완료하지 못함
//			if(pCell->RecvSleepCurrnet == FALSE)
//			{
//				return FALSE;
//			}
//		}
//	}//of for i
//
//	return TRUE;
//}
//
//
//// ID_CHECK (White current) 완료 여부
//BOOL CMCMain::Is_PGSIDCheckEndCheck(int nJig)
//{
//	int i;
//	TCellData *pCell;
//
//	for (i = 0; i < eMAX_ChCount; i++)
//	{
//		pCell = m_pCellInfo->GetCellData(nJig, i);
//		if ( pCell == NULL ) continue;
//
//		if ( pCell->bIsExist == TRUE )					// Cell이 있는 경우에만 실행
//		{
//			// PG 이상발생 처리
//			if( (pCell->PG_DownZone != eIDX_PG_ZONE_Final) && (pCell->RecvWhiteCurrnet == FALSE) )
//			{
//				pCell->RecvWhiteCurrnet = TRUE;
//				pCell->WhiteCurrentClass = LOSS_CELL;
//				::GetLocalTime( &pCell->cellTime.systmC_WhiteCurrentEndTime );
//			}
//
//			// 1개라도 수신받지 못한 channel이 있다면 아직 수신완료하지 못함
//			if(pCell->RecvWhiteCurrnet == FALSE)
//			{
//				return FALSE;
//			}
//		}
//	}//of for i
//
//	return TRUE;
//}

 //! 2015-07-16, jhLee, CJigInfo의 사용으로 더이상 사용하지 않게되었다.

// 지정 Jig의 각 채널에 TMD Info 정보를 지정한다.
void CMCMain::PG_CatchTMDInfo(int nJig)
{
	int i;
	TCellData *pCell;
	CJigInfo* pJigInfo = CJigInfo::GetInstance();
	CChannelData* pChData = NULL;

	for (i = 0; i < eMAX_ChCount; i++)
	{
		pCell = m_pCellInfo->GetCellData(nJig, i);
		if ( pCell == NULL ) continue;

		pChData = pJigInfo->GetChData(nJig, i); 
		if ( pChData == NULL ) continue;

		pCell->PG_TMDModel	= pChData->sPG_TMDModel;	// theSockMgr.m_strTMDInfo[ nIdx ];
		pCell->PG_Version		= pChData->sPG_Version;	// theSockMgr.m_strPGVer[ nIdx ];
		pCell->PG_MACAddress	= pChData->sPG_MACAddress;	// theSockMgr.CH_Card_Mac[ nIdx ];
	}
}

//
//// 지정 채널의 신호기 Contact 변화 기록, 
//void CMCMain::SetPGContactFlag(int nJig, int nCh, BOOL bFlag)
//{
//	if ( (nJig >= 0) && (nJig < eMAX_JigCount) && (nCh >= 0) && (nCh < eMAX_ChCount) )
//	{
//		m_rProfileData.bPGContactFlag[ nJig ][ nCh ] = bFlag;
//	}
//}
//
//// 지정 채널의 신호기 Contact 상태
//BOOL CMCMain::GetPGContactFlag(int nJig, int nCh)
//{
//	if ( (nJig >= 0) && (nJig < eMAX_JigCount) && (nCh >= 0) && (nCh < eMAX_ChCount) )
//	{
//		return m_rProfileData.bPGContactFlag[ nJig ][ nCh ];
//	}
//
//	return FALSE;
//}
//
//
//// 지정 지그의 신호기 Contact 상태
//// 1개의 채널이라도 Contact 되었다면 TRUE이다.
//BOOL CMCMain::GetPGContactJigFlag(int nJig)
//{
//	BOOL bFlag = FALSE;
//	int i;
//
//	if ( (nJig >= 0) && (nJig < eMAX_JigCount) )
//	{
//		for (i = 0; i < eMAX_ChCount; i++)
//		{
//			//nCh = (nIdx * eMAX_ChCount) + i;
//
//			// 1개라도 켜져 있으면 ON 상태이다.
//			if ( m_rProfileData.bPGContactFlag[ nJig ][ i ] == TRUE )	bFlag = TRUE;
//		}
//	}
//
//	return bFlag;
//}
//
//
//// 지정 채널의 신호기 Contact 상태를 OFF로 Clear 해준다.
//void CMCMain::ClearPGContactFlag(int nJig)
//{
//	if ( (nJig >= 0) && (nJig < eMAX_JigCount) )
//	{
//		for (int i = 0; i < eMAX_ChCount; i++)
//		{
//			m_rProfileData.bPGContactFlag[ nJig ][ i ]	= FALSE;
//		}
//	}
//}
//

//////////////////////////////////////////////////////////////////////////////////////////////////////////////


//CIM begin
/*

//SONIC-2013.05.28.
// 검사가 끝났음을 알려줌

// 현재 A-Zone에 있는 Jig를 대상으로 한다. 
// 지정 Jig의 모든 채널에 대하여 동작 실행
void CMCMain::CIM_All_InspectionEnd(int nJig)
{
	if ( m_rProfileData.bUseTest_BIN2TestMode ) return;	// BIN2 RUN mdoe, Data-PC로 전송하지 않는다.

	TCellData*	pCell; 

	TCHAR cInnerID[MAX_MES_SIZE];
	TCHAR cCellID[MAX_CELL_ID];
	TCHAR cDefect[MAX_MES_SIZE];

	for (int i = 0; i < eMAX_ChCount; i++)
	{
		pCell = m_pCellInfo->GetCellData( nJig, i );									// 해당 채널의 Cell data를 취득

		if ( pCell != NULL )
		{
			// 셀이 존재하고 DryRun이 아닐때
			if( (pCell->bIsExist == TRUE) && (m_pCellInfo->bIsDryRun == FALSE) )		// m_bIsDryRun은 MCMain (여기)에 선언되어있다.
			{
				_tcscpy_s(cInnerID,	_countof(cInnerID),	pCell->InnerID);	// 내부 CellID
				_tcscpy_s(cCellID,	_countof(cCellID),	pCell->CellID);	// MCR ID
				cDefect[0] = 0x00;

				theDataPCMessage.CmdCellInspectionEnd(CMessage::E_INS_END, (CMessage::E_JIG_X)nJig, (CMessage::E_CELL_POS)i, pCell->bIsExist, cInnerID, cCellID, cDefect);
			}
		}//of if NOT Null
	}
}


//SONIC-2013.05.29.
// jhLee, 2014-06-26
//
// 처리(작업)가 최종적으로 끝났음을 알려준다.				// XXXX 끝나 최종 판정 불량명 전송
// 전송시점 : A존에서 작업자 Unloading이 되면 전송
//
// 현재 A-Zone에 있는 Jig를 대상으로 한다. 
// 지정 Jig의 모든 채널에 대하여 동작 실행
void CMCMain::CIM_CellProcessComplete(int nJig)
{
	if ( m_rProfileData.bUseTest_BIN2TestMode ) return;	// BIN2 RUN mdoe, Data-PC로 전송하지 않는다.

	BOOL bRet;

	TCellData*	pCell; 

	TCHAR cInnerID[MAX_MES_SIZE];
	TCHAR cCellID[MAX_CELL_ID];

	// previous inform...
	//
	// 양산Cell일 땐 양품만 Track Out
	// 양품이 아닌것은 이후 따로 Manual검사 후 Track Out한다고 함
	// Rework Cell일 때는 모두 Track Out
	// Test Cell 및 Job Change일 경우 Track Out 보고 안함
	// 모든 Cell이 GOOD
	// --> 변경됨, 원칙대로 L, R, G 모두 각각 보고하도록 하였다.

	for (int i = 0; i < eMAX_ChCount; i++)
	{
		pCell = m_pCellInfo->GetCellData( nJig, i );								// 해당 채널의 Cell data를 취득

		if ( pCell != NULL )
		{
			if ( pCell->bIsExist == TRUE  )											// 셀이 존재할때
			{
				_tcscpy_s(cInnerID,	_countof(cInnerID),	pCell->InnerID);			// 내부 CellID
				_tcscpy_s(cCellID,	_countof(cCellID),	pCell->CellID);				// MCR ID

				// 통신 전송이 실패하면 3회까지 재시도를 수행
				//for (int j = 0; j < 3; j++)
				//{
					bRet = theDataPCMessage.CmdCellProcessComplete((CMessage::E_JIG_X)nJig, (CMessage::E_CELL_POS)i, pCell->CellIDCheckup(), cInnerID, cCellID);

				//	if ( bRet == TRUE ) break;

				//	Sleep(10);
				//}

				pCell->bTrackInReport = FALSE;										// Track out을 보고하여 이전의 Track-In 보고 표식은 지운다.

				// 2016-09-08, jhLee, 별도의 Track-In Log를 남긴다.
				m_logTracking.Write(_T("%Track-Out,%s,%s,%d,%s"), pCell->InnerID, pCell->CellID, pCell->FinalClass, pCell->FinalCode);

			}
		}//of if NOT NULL
	}//of for i
}





//
//2014-12-23, jhLee, 
//SONIC-2013.05.28.
//
void CMCMain::CIM_Pwr_InspectionEnd(int nJig, int nCh)
{
	if ( m_rProfileData.bUseTest_BIN2TestMode ) return;	// BIN2 RUN mdoe, Data-PC로 전송하지 않는다.

	TCellData* pCell;

//	JIG_ID jig;
//	jig = AMT_GetAutoJigID(ZONE_C);

	TCHAR cInnerID[MAX_MES_SIZE];
	TCHAR cCellID[MAX_CELL_ID];
	TCHAR cDefect[MAX_MES_SIZE];

	for (int i = 0; i < JIG_CH_MAX; i++)
	{
		//tagCell = theProcDataMgr.m_ProcData.m_AmtModule.GetCell((JIG_CH)i, ZONE_C);
		pCell = m_pCellInfo->GetCellData( nJig, nCh );								// 해당 채널의 Cell data를 취득

		if( (pCell->bIsExist == TRUE) && (m_pCellInfo->bIsDryRun == FALSE) )		
		{
			// pCell = &theProcDataMgr.m_ProcData.m_DataPool.m_RuningCell[tagCell.inxCell];
			_tcscpy_s(cInnerID, _countof(cInnerID), pCell->InnerID);
			_tcscpy_s(cCellID, _countof(cCellID), pCell->CellID);
			cDefect[0] = 0x00;
			theDataPCMessage.CmdCellInspectionEnd(CMessage::E_PWR, (CMessage::E_JIG_X)nJig, (CMessage::E_CELL_POS)i, pCell->bIsExist, cInnerID, cCellID, cDefect);
		}
	}
}


//SONIC - 2014.04.13.
// jhLee, 2014-06-26
// 지정 Jig의 Channel에서 TSP 검사가 종료되었다.
//
void CMCMain::CIM_TSP_InspEnd(int nJig, int nCh)		//  JIG_CH ch )
{
	if ( m_rProfileData.bUseTest_BIN2TestMode ) return;	// BIN2 RUN mdoe, Data-PC로 전송하지 않는다.

	TCellData*	pCell; 

	TCHAR cInnerID[MAX_MES_SIZE];
	TCHAR cCellID[MAX_CELL_ID];
	TCHAR cTspMainDefect[MAX_MES_SIZE];

	pCell = m_pCellInfo->GetCellData( nJig, nCh );								// 해당 채널의 Cell data를 취득

	if ( pCell != NULL )
	{
		// 셀이 존재하고 DryRun이 아닐때
		if( (pCell->bIsExist == TRUE) && (m_pCellInfo->bIsDryRun == FALSE) )		
		{
			_tcscpy_s(cInnerID,	_countof(cInnerID),	pCell->InnerID);	// 내부 CellID
			_tcscpy_s(cCellID,	_countof(cCellID),	pCell->CellID);	// MCR ID
			_tcscpy_s(cTspMainDefect, _countof(cTspMainDefect), m_pCellInfo->MakeString( pCell->TspNGMainName, MAX_MES_SIZE) );

			theDataPCMessage.CmdCellInspectionEnd(CMessage::E_TSP, (CMessage::E_JIG_X)nJig, (CMessage::E_CELL_POS)nCh, pCell->bIsExist, cInnerID, cCellID, cTspMainDefect);
		}
	}//of if NOT Null

}


//SONIC-2013.05.28.
//
// Track-In, MCR 읽었을때 (최초로 전송)
//
void CMCMain::CIM_CellProcessStart(int nJig, int nCh)
{
	if ( m_rProfileData.bUseTest_BIN2TestMode ) return;	// BIN2 RUN mdoe, Data-PC로 전송하지 않는다.

	TCellData*	pCell; 
	BOOL bIsRead;				// UV-MCR을 읽었는가 ?

	TCHAR cInnerID[MAX_MES_SIZE];
	TCHAR cCellID[MAX_CELL_ID];

	pCell = m_pCellInfo->GetCellData( nJig, nCh );								// 해당 채널의 Cell data를 취득

	if ( pCell != NULL )
	{
		if( pCell->bIsExist == TRUE )		
		{
			_tcscpy_s(cInnerID,	_countof(cInnerID),	pCell->InnerID);			// 내부 CellID
			_tcscpy_s(cCellID,	_countof(cCellID),	pCell->CellID);				// MCR ID


			// UV-MCR을 읽지 않도록 지정되어있다면,
			if ( m_rProfileData.bUse_SkipMCR == TRUE ) {				
				bIsRead = FALSE;												// Cell ID는 읽지 않았다고 지정한다.
			}
			else
				bIsRead =  pCell->CellIDCheckup();								// 정상적인 Cell ID여부

			// Data PC에게 전송
			theDataPCMessage.CmdCellProcessStart((CMessage::E_JIG_X)nJig, (CMessage::E_CELL_POS)nCh, pCell->CellIDCheckup(), cInnerID, cCellID);

			pCell->bTrackInReport = TRUE;										// Track in 보고 


			// 2016-09-08, jhLee, 별도의 Track-In Log를 남긴다.
			m_logTracking.Write(_T("%Track-In,%s,%s"), pCell->InnerID, pCell->CellID);
		}
	}//of if NOT Null
}




//SONIC-2013.05.28.
// jhLee, 2014-06-26
// 새로운 Cell을 로딩하여 신호기 Contact가 되었다.
//
// A Zone에서 작업자가 Cell을 로딩 후 양수 버튼을 눌러 Loaing이 완료되면 신호기에서 이미 ICT 테스트를 진행하였고,
// 신호기에서  Contact 후 결과 log 생성되는 시간을 감안하여,
// Tilt 후 보내도록 한다.
//
// 기존 참고 : //AMT_AZone_ContactEndChk() 다음에 바로 진행.
void CMCMain::CIM_Ict_Info_InspectionEnd(int nJig)	
{
	if ( m_rProfileData.bUseTest_BIN2TestMode ) return;	// BIN2 RUN mdoe, Data-PC로 전송하지 않는다.

	TCellData*	pCell; 

	TCHAR cInnerID[MAX_MES_SIZE];
	TCHAR cCellID[MAX_CELL_ID];
	TCHAR cDefect[MAX_MES_SIZE];


	for (int i = 0; i < eMAX_ChCount; i++)
	{
		pCell = m_pCellInfo->GetCellData( nJig, i );									// 해당 채널의 Cell data를 취득

		if ( pCell != NULL )
		{
			// 셀이 존재하고 DryRun이 아닐때
			if( (pCell->bIsExist == TRUE) && (m_pCellInfo->bIsDryRun == FALSE) )	
			{
				_tcscpy_s(cInnerID,	_countof(cInnerID),	pCell->InnerID);	// 내부 CellID
				_tcscpy_s(cCellID,	_countof(cCellID),	pCell->CellID);	// MCR ID
				cDefect[0] = 0x00;

				theDataPCMessage.CmdCellInspectionEnd(CMessage::E_ICT, (CMessage::E_JIG_X)nJig, (CMessage::E_CELL_POS)i, pCell->bIsExist, cInnerID, cCellID, cDefect);
				theDataPCMessage.CmdCellInspectionEnd(CMessage::E_INFO, (CMessage::E_JIG_X)nJig, (CMessage::E_CELL_POS)i, pCell->bIsExist, cInnerID, cCellID, cDefect);
			}
		}//of if NOT Null
	}
}



//SONIC-2013.05.28.
// jhLee, 2014-06-26
//
// 화상검사(검사PC) 검사 완료 전송
//
// 검사PC결과는 필요없으므로 Event만 전송, (검사 결과는 TSP,MTP만 필요, MTP도 현재는 안씀)
void CMCMain::CIM_VT_InspEnd(int nJig, int nCh )
{
	if ( m_rProfileData.bUseTest_BIN2TestMode ) return;	// BIN2 RUN mdoe, Data-PC로 전송하지 않는다.

	TCellData*	pCell; 

	TCHAR cInnerID[MAX_MES_SIZE];
	TCHAR cCellID[MAX_CELL_ID];
	TCHAR cDefect[MAX_MES_SIZE];

	pCell = m_pCellInfo->GetCellData( nJig, nCh );									// 해당 채널의 Cell data를 취득

	if ( pCell != NULL )
	{
		// 셀이 존재하고 DryRun이 아닐때
		if( (pCell->bIsExist == TRUE) && (m_pCellInfo->bIsDryRun == FALSE) )
		{
			_tcscpy_s(cInnerID,	_countof(cInnerID),	pCell->InnerID);	// 내부 CellID
			_tcscpy_s(cCellID,	_countof(cCellID),	pCell->CellID);		// MCR ID
			cDefect[0] = 0x00;

			theDataPCMessage.CmdCellInspectionEnd(CMessage::E_VT1, (CMessage::E_JIG_X)nJig, (CMessage::E_CELL_POS)nCh, pCell->bIsExist, cInnerID, cCellID, cDefect);
		}
	}
}





//SONIC-2013.09.27. Modify
// Align 스코어값, Align 패스여부
//

// Channel #1 인경우 -> E_ALIGN_LEFT_CAM 으로 간주
// Channel #2 인경우 -> E_ALIGN_LEFT_CAM 으로 간주
void CMCMain::CIM_ContactAlignReport(int nJig, int nCh)	
{
	if ( m_rProfileData.bUseTest_BIN2TestMode ) return;	// BIN2 RUN mdoe, Data-PC로 전송하지 않는다.

	TCellData*	pCell;

//	CCell* pCell;
	//JIG_ID jig;

	TCHAR cAlignCAM[MAX_MES_SIZE];
	TCHAR cCellID[MAX_CELL_ID];
	TCHAR cMarkLeftScore[MAX_MES_SIZE];
	TCHAR cMarkRightScore[MAX_MES_SIZE];
	BOOL bRead = FALSE;

	memset(cMarkLeftScore, 0x00, sizeof(cMarkLeftScore));
	memset(cMarkRightScore, 0x00, sizeof(cMarkRightScore));

	pCell = m_pCellInfo->GetCellData( nJig, nCh );									// 해당 채널의 Cell data를 취득 

	if ( pCell != NULL )
	{
		// 셀이 존재하고 DryRun이 아닐때
		if( (pCell->bIsExist == TRUE) && (m_pCellInfo->bIsDryRun == FALSE) )
		{
			// 채널 #1 이면, 왼쪽 카메라임을, 아니면 오른쪽 카메라라고 간준
			if ( nCh == eIDX_CH_1 )
				_tcscpy_s(cAlignCAM, _countof(cAlignCAM), _T(STR_ALIGN_LEFT_CAM));
			else
				_tcscpy_s(cAlignCAM, _countof(cAlignCAM), _T(STR_ALIGN_RIGHT_CAM));

			// Align 결과,
			if ( pCell->VT_AlignResult == eAlignResult_GOOD )
				bRead = TRUE;
			else if ( pCell->VT_AlignResult == eAlignResult_FAIL )
				bRead = FALSE;
			else
				return;

			_tcscpy_s(cCellID, _countof(cCellID), pCell->CellID);
			_stprintf_s(cMarkLeftScore, _countof(cMarkLeftScore), _T("%.3f"),		pCell->VT_Align_Score[eIDX_MarkLeft]);
			_stprintf_s(cMarkRightScore, _countof(cMarkRightScore), _T("%.3f"),	pCell->VT_Align_Score[eIDX_MarkRight]);

			theDataPCMessage.CmdFindingMarkReport(cAlignCAM, bRead, cCellID, cMarkLeftScore, cMarkRightScore);
		}

	}//of if NOT Null

}


// 2015-04-28, jhLee, QMS 전송에 맞도록 변경 : 1차는 천안에 적용한 방식으로 
// 날짜형을 기존 "2014-04-28 09:30:25:120" -> "20140428093025" 형식으로 변경 
// 
//SONIC - 2013.07.22. Modify
// 
// 모든 검사가 끝난 후(Unloading후) 취합하여 전송
//
BOOL CMCMain::CIM_GetOperationData(int nJig)
{
	if ( m_rProfileData.bUseTest_BIN2TestMode ) return TRUE;	// BIN2 RUN mdoe, Data-PC로 전송하지 않는다.

	int i;
	CString strCell;
 	char szBuf[256];
 	float fTacktime;
	CString strTemp;
	CString strRCode;
	CString strDefect;
	TCHAR cDefName[256] = {0};			//SONIC - 2013.11.05.
	TCHAR cExcepDefName[256] = {0};		//SONIC - 2013.11.05.

	TCellData*	pCell;				// 동작 데이터
	tag_CellData*	pCellData;			// 전송용 데이터

	for ( i = 0; i < eMAX_ChCount; i++)
	{
		pCell = m_pCellInfo->GetCellData( nJig, i );					// 해당 채널의 Cell data를 취득

		if ( pCell->bIsExist == FALSE )	continue;				// Cell이 존재 하지 않는다면 수행하지 않는다.

		// 전송용 데이터 저장소 획득
		pCellData = NULL;
		pCellData = &theDataPCData.m_tCellData[nJig][i];

		/////////////////////////////// CELL DATA ///////////////////////////////////
		//SONIC - 2013.07.31. Modify

		// 현재 시스템에서는 D존 작업자 판정이 별도로 없다.
		// 두 값들은 OK 만 넣어줘서 NULL 방지하여 전송
		_tcscpy_s(pCellData->m_cJudgeOper1, _countof(pCellData->m_cJudgeOper1), _T("OK"));
		_tcscpy_s(pCellData->m_cJudgeOper2, _countof(pCellData->m_cJudgeOper2), _T("OK"));


		// 여기서부터 필요, 작업자 판정
		switch (pCell->FinalClass)
		{
			case eGOOD_CELL:	
				_tcscpy_s(pCellData->m_cJudge, _countof(pCellData->m_cJudge), _T("G"));
				_tcscpy_s(pCellData->m_cReasonCode, _countof(pCellData->m_cReasonCode), _T(""));
				//_tcscpy_s(pCellData->m_cDescription, _countof(pCellData->m_cDescription), _T("GODD"));	// 2015/05/07 JH-PARK , 상위보고시 해당 Defect명도 같이 보내준다 ( 김영락S 요청)
				break;

			// 2015-01-17, jhLee, SDC 김상묵 사원 요청으로 BIN2와 Reject 모두 'Repair'로 올리던 것을 원래대로 Reject은 'Loss'로 올린다.
			//  이 부분은 본 프로그램을 만들면서 CIM 담당 이충현 과장의 조언대로 검사시 Reject이 발생하면 그대로 Loss로 보고하지 않고
			// 일단 BIN2 (Repair)로 보고하고 추후에 재검등의 처리를 통해 확실해지면 별도로 Loss로 등록한다고 하여 당시 코드를 그대로 채용한 부분이다.
			// 하지만 현재까지(2015-01-16) Reject이 발생한 제품들이 전산에 등록이 되지 않게되므로 여사원들이 Reject 제품을 가지고 일일히 수동 입력해 주고있음.
			// 이에, 고객사 요청으로 BIN2와 Reject을 분리하여 보고하도록 한다.
			case BIN2_CELL:
			// case LOSS_CELL:	
				_tcscpy_s(pCellData->m_cJudge, _countof(pCellData->m_cJudge), _T("R"));
				// strRCode = pCell->MesCode;
 				_tcscpy_s(pCellData->m_cReasonCode, _countof(pCellData->m_cReasonCode), _T("") );	// Retest는 불량 코드가 없다. strRCode);
				//_tcscpy_s(pCellData->m_cDescription, _countof(pCellData->m_cDescription), _T("RETEST"));	// 2015/05/07 JH-PARK , 상위보고시 해당 Defect명도 같이 보내준다 ( 김영락S 요청)
				break;


			case LOSS_CELL:		// LOSS로 보고
				_tcscpy_s(pCellData->m_cJudge, _countof(pCellData->m_cJudge), _T("L"));
				strRCode = pCell->MesCode;
 				_tcscpy_s(pCellData->m_cReasonCode, _countof(pCellData->m_cReasonCode), strRCode);
				strDefect = pCell->PrintName;	// 영문 Defect명 (Print 표기) 을 취득한다
				//_tcscpy_s(pCellData->m_cDescription, _countof(pCellData->m_cDescription), strDefect);	// 2015/05/07 JH-PARK , 상위보고시 해당 Defect명도 같이 보내준다 ( 김영락S 요청)
				break;
		}


		//YMS 수정.2014.03.29[JYC]
		/////////////////////////////CELL LOG DATA /////////////////////////////////

		theDataPCData.m_tCellLogData.ValueClear(i);

		// INNER ID;
		strCell.Format(_T("%s")									,pCell->InnerID);
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);

		// MODEL;
		strCell.Format(_T(",%s")								, GetRecipeModelName());		// theParamMgr.m_Rcp.HEADER.strName);
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);

		// CELL ID;
		strCell.Format(_T(",%s")								,pCell->CellID == TEXT_UV_FAIL ? pCell->InnerID:pCell->CellID);
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);

		// JIG NO;
		// strCell.Format(_T(",%s")								,pCell->JigRFID);				//  Jig의 순번 혹은 Jig 고유번호? (현재 고유번호는 관리하지 않는다)
		strCell.Format(_T(",%d")								,pCell->JigNo+1);
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);

	
		// TACT TIME; 투입부터 현재 까지의 시간, 
		fTacktime = m_pCellInfo->CountTactTime(pCell->cellTime.systmA_StartTime, pCell->cellTime.systmD_EndTime);
		strCell.Format(_T(",%.2f")								,fTacktime);
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);
	

		// OLD
			//// AZONE START TIME;
			//strCell.Format(_T(",%04u-%02u-%02u %02u:%02u:%02u:%03u"),pCell->cellTime.systmA_StartTime.wYear
			//	,pCell->cellTime.systmA_StartTime.wMonth
			//	,pCell->cellTime.systmA_StartTime.wDay
			//	,pCell->cellTime.systmA_StartTime.wHour
			//	,pCell->cellTime.systmA_StartTime.wMinute
			//	,pCell->cellTime.systmA_StartTime.wSecond
			//	,pCell->cellTime.systmA_StartTime.wMilliseconds);
			//CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
			//theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);

			//// AZONE END TIME;
			//strCell.Format(_T(",%04u-%02u-%02u %02u:%02u:%02u:%03u"),pCell->cellTime.systmA_EndTime.wYear
			//	,pCell->cellTime.systmA_EndTime.wMonth
			//	,pCell->cellTime.systmA_EndTime.wDay
			//	,pCell->cellTime.systmA_EndTime.wHour
			//	,pCell->cellTime.systmA_EndTime.wMinute
			//	,pCell->cellTime.systmA_EndTime.wSecond
			//	,pCell->cellTime.systmA_EndTime.wMilliseconds);
			//CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
			//theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);

		// 2015-04-28, jhLee, QMS用 날짜형으로 변경
		// AZONE START TIME;
		strCell.Format(_T(",%04u%02u%02u%02u%02u%02u")		// pCell->cellTime.systmA_StartTime
			,pCell->cellTime.systmA_StartTime.wYear
			,pCell->cellTime.systmA_StartTime.wMonth
			,pCell->cellTime.systmA_StartTime.wDay
			,pCell->cellTime.systmA_StartTime.wHour
			,pCell->cellTime.systmA_StartTime.wMinute
			,pCell->cellTime.systmA_StartTime.wSecond);
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);

		// AZONE END TIME;
		strCell.Format(_T(",%04u%02u%02u%02u%02u%02u")		// pCell->cellTime.systmA_EndTime
			,pCell->cellTime.systmA_EndTime.wYear
			,pCell->cellTime.systmA_EndTime.wMonth
			,pCell->cellTime.systmA_EndTime.wDay
			,pCell->cellTime.systmA_EndTime.wHour
			,pCell->cellTime.systmA_EndTime.wMinute
			,pCell->cellTime.systmA_EndTime.wSecond );
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);



		// AZONE TACK TIME;  A존을 끝낸 시간
		fTacktime = m_pCellInfo->CountTactTime(pCell->cellTime.systmA_StartTime, pCell->cellTime.systmA_EndTime);
		strCell.Format(_T(",%.2f")								,fTacktime);
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);

		// AZONE CLASS;  A존에서 구동 테스트 결과
		strCell.Format(_T(",%d")								, pCell->AZoneClass);
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);
	
		// AZONE CONTACT CLASS;   Z 존 컨택후 구동 결과.... 위와 같은 값 ?
		strCell.Format(_T(",%d")								,pCell->ContactCurrentClass == eGOOD_CELL ? CELL_LOG_OK:CELL_LOG_NG);
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);

		// AZONE DEFECT; 
		strCell.Format(_T(",%s")								,pCell->AZoneDefect);
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);

		// N/A
		strCell.Format(_T(",%s")								,_T(""));
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);

		// N/A
		strCell.Format(_T(",%s")								,_T(""));
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);

		// 양식 맞추어서 빈공백

		// BZONE START TIME
		strCell.Format(_T(",%04u%02u%02u%02u%02u%02u")			// pCell->cellTime.systmB_StartTime
			,0
			,0
			,0
			,0
			,0
			,0);

		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);

		// BZONE END TIME
		strCell.Format(_T(",%04u%02u%02u%02u%02u%02u")			// pCell->cellTime.systmB_EndTime
			,0
			,0
			,0
			,0
			,0
			,0);
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);


		// BZONE TACK TIME
		// fTacktime = m_pCellInfo->CountTactTime(pCell->cellTime.systmB_StartTime, pCell->cellTime.systmB_EndTime);
		fTacktime = 0.0;
		strCell.Format(_T(",%.2f")								,fTacktime);
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);

	// 생략
		//MTP  START TIME
		strCell.Format(_T(",%04u%02u%02u%02u%02u%02u")			// pCell->cellTime.systmC_MtpStartTime
			,0
			,0
			,0
			,0
			,0
			,0);
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);

		//MTP  END TIME
		strCell.Format(_T(",%04u%02u%02u%02u%02u%02u")			// pCell->cellTime.systmC_MtpEndTime
			,0
			,0
			,0
			,0
			,0
			,0);
		strCell = _T(",");
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);

		//MTP  TACT TIME
		// fTacktime = m_pCellInfo->CountTactTime(pCell->cellTime.systmC_MtpStartTime, pCell->cellTime.systmC_MtpEndTime);
		fTacktime = 0.0;
		strCell.Format(_T(",%.2f")								,fTacktime);
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);

		//MCR START TIME
		// 기존 B-Zone -> A-Zone으로 이동되었다.
		strCell.Format(_T(",%04u%02u%02u%02u%02u%02u")
			,pCell->cellTime.systmA_McrStartTime.wYear
		 	,pCell->cellTime.systmA_McrStartTime.wMonth
		 	,pCell->cellTime.systmA_McrStartTime.wDay
		 	,pCell->cellTime.systmA_McrStartTime.wHour
		 	,pCell->cellTime.systmA_McrStartTime.wMinute
		 	,pCell->cellTime.systmA_McrStartTime.wSecond );
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);


		//MCR END TIME
		strCell.Format(_T(",%04u%02u%02u%02u%02u%02u")
			,pCell->cellTime.systmA_McrENDTime.wYear
		 	,pCell->cellTime.systmA_McrENDTime.wMonth
		 	,pCell->cellTime.systmA_McrENDTime.wDay
		 	,pCell->cellTime.systmA_McrENDTime.wHour
		 	,pCell->cellTime.systmA_McrENDTime.wMinute
		 	,pCell->cellTime.systmA_McrENDTime.wSecond );
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);

		//MCR TACT TIME
		fTacktime = m_pCellInfo->CountTactTime(pCell->cellTime.systmA_McrStartTime, pCell->cellTime.systmA_McrENDTime);
		strCell.Format(_T(",%.2f")								,fTacktime);
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);

		//MTP DEFECT
		strCell.Format(_T(",%s")								,pCell->ResultMtpName);
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);
	

		//ALIGN DEGREE
		strCell.Format(_T(",%.6f")								,pCell->VT_Align_Data_th);
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);

		// N/A
		strCell.Format(_T(",%s")								,_T(""));
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);

		// N/A
		strCell.Format(_T(",%s")								,_T(""));
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);


	// C존 전체 시작

		// CZONE START TIME
		strCell.Format(_T(",%04u%02u%02u%02u%02u%02u")
			,pCell->cellTime.systmC_StartTime.wYear
			,pCell->cellTime.systmC_StartTime.wMonth
			,pCell->cellTime.systmC_StartTime.wDay
			,pCell->cellTime.systmC_StartTime.wHour
			,pCell->cellTime.systmC_StartTime.wMinute
			,pCell->cellTime.systmC_StartTime.wSecond );
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);

		// CZONE END TIME
		strCell.Format(_T(",%04u%02u%02u%02u%02u%02u")
			,pCell->cellTime.systmC_EndTime.wYear
			,pCell->cellTime.systmC_EndTime.wMonth
			,pCell->cellTime.systmC_EndTime.wDay
			,pCell->cellTime.systmC_EndTime.wHour
			,pCell->cellTime.systmC_EndTime.wMinute
			,pCell->cellTime.systmC_EndTime.wSecond );
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);

		// CZONE TACT TIME
		fTacktime = m_pCellInfo->CountTactTime(pCell->cellTime.systmC_StartTime, pCell->cellTime.systmC_EndTime);
		strCell.Format(_T(",%.2f")								,fTacktime);
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);



		//! 화상검사 시작 시간
		// AUTO START TIME
		strCell.Format(_T(",%04u%02u%02u%02u%02u%02u")
			,pCell->cellTime.systmC_VisionStartTime.wYear
			,pCell->cellTime.systmC_VisionStartTime.wMonth
			,pCell->cellTime.systmC_VisionStartTime.wDay
			,pCell->cellTime.systmC_VisionStartTime.wHour
			,pCell->cellTime.systmC_VisionStartTime.wMinute
			,pCell->cellTime.systmC_VisionStartTime.wSecond );
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);


		// AUTO END TIME
		strCell.Format(_T(",%04u%02u%02u%02u%02u%02u")
			,pCell->cellTime.systmC_VisionEndTime.wYear
			,pCell->cellTime.systmC_VisionEndTime.wMonth
			,pCell->cellTime.systmC_VisionEndTime.wDay
			,pCell->cellTime.systmC_VisionEndTime.wHour
			,pCell->cellTime.systmC_VisionEndTime.wMinute
			,pCell->cellTime.systmC_VisionEndTime.wSecond );
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);

		// AUTO TACK TIME
		fTacktime = m_pCellInfo->CountTactTime(pCell->cellTime.systmC_VisionStartTime, pCell->cellTime.systmC_VisionEndTime);
		strCell.Format(_T(",%.2f")								,fTacktime);
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);

		// 신호기
		// TSP START TIME
		strCell.Format(_T(",%04u%02u%02u%02u%02u%02u")		// pCell->cellTime.systmC_TspStartTime
			,0
			,0
			,0
			,0
			,0
			,0);
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);


		// TSP END TIME
		strCell.Format(_T(",%04u%02u%02u%02u%02u%02u")		// pCell->cellTime.systmC_TspEndTime
			,0
			,0
			,0
			,0
			,0
			,0);
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);

		// TSP TACT TIME
		// fTacktime = m_pCellInfo->CountTactTime(pCell->cellTime.systmC_TspStartTime, pCell->cellTime.systmC_TspEndTime);
		fTacktime = 0.0;
		strCell.Format(_T(",%.2f")								,fTacktime);
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);

	// 대표 불량명
		// CZONE CLASS
		strCell.Format(_T(",%d")								,pCell->CZoneClass);
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);
				
	// 자동검사 판정
		// AUTO  CLASS
		// QMS 변경 포멧 방식 자동검사 판정을 BIN1, BIN2, REJECT으로 남김 20150410 jhkim1
		//OLD strCell.Format(_T(",%d")								,pCell->VTClassJudge);
		if (pCell->VTClassJudge == 0)													
		{
			strCell.Format(_T(",BIN1"));
		}
		else if (pCell->VTClassJudge == 1)
		{
			strCell.Format(_T(",REJECT"));
		}
		else
		{
			strCell.Format(_T(",BIN2"));
		}
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);

	// 자동검사 불량명
		// AUTO  DEFECT
		strCell.Format(_T(",%s")								, pCell->ResultVTName);
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);

		//요놈들 모르는놈들임....
		// REF DEFECT
		strCell.Format(_T(",%s")								,_T(""));
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);

		// DEL DEFECT
		strCell.Format(_T(",%s")								,_T(""));
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);


		//BLACK IDDI
		strCell.Format(_T(",%s")								,_T(""));
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);

		//BLACK IVCI
		strCell.Format(_T(",%s")								,_T(""));
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);

		//BLACK IBAT
		strCell.Format(_T(",%s")								,_T(""));
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);
	
		//1BY1 2BY2 IBAT
		strCell.Format(_T(",%s")								,_T(""));
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);
	
		//1BY1 2BY2 IVCI
		strCell.Format(_T(",%s")								,_T(""));
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);
	
		//1BY1 2BY2 IBAT
		strCell.Format(_T(",%s")								,_T(""));
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);


		// N/A
		strCell.Format(_T(",%s")								,_T(""));
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);

		// N/A
		strCell.Format(_T(",%s")								,_T(""));
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);

	// 리턴되어 A존으로 되돌아 왔을때 부터 
		// DZONE START TIME
		strCell.Format(_T(",%04u%02u%02u%02u%02u%02u")
			,pCell->cellTime.systmD_StartTime.wYear
			,pCell->cellTime.systmD_StartTime.wMonth
			,pCell->cellTime.systmD_StartTime.wDay
			,pCell->cellTime.systmD_StartTime.wHour
			,pCell->cellTime.systmD_StartTime.wMinute
			,pCell->cellTime.systmD_StartTime.wSecond );
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);

		// DZONE END TIME
		strCell.Format(_T(",%04u%02u%02u%02u%02u%02u")
			,pCell->cellTime.systmD_EndTime.wYear
			,pCell->cellTime.systmD_EndTime.wMonth
			,pCell->cellTime.systmD_EndTime.wDay
			,pCell->cellTime.systmD_EndTime.wHour
			,pCell->cellTime.systmD_EndTime.wMinute
			,pCell->cellTime.systmD_EndTime.wSecond );
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);


		// DZONE TACK TIME
		fTacktime = m_pCellInfo->CountTactTime(pCell->cellTime.systmD_StartTime, pCell->cellTime.systmD_EndTime);
		strCell.Format(_T(",%.2f")								,fTacktime);
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);

		// DZONE CLASS
		strCell.Format(_T(",%d")								,pCell->DZoneClass);
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);

		// DZONE DEFECT
		strCell.Format(_T(",%s")								,pCell->DZoneDefect);
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);


		// DZONE DEFECT POSITION
		strCell.Format(_T(",%s")								,_T(""));
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);
						
		// N/A
		strCell.Format(_T(",%s")								,_T(""));
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);

		// N/A
		strCell.Format(_T(",%s\n")								,_T(""));
		CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		theDataPCData.m_tCellLogData.AddCellLogData(i, strCell);

		CString strID;

		if ( pCell->CellIDCheckup() == TRUE )	//	CellIdCheckup(pCell->CellID))
		{
			strID = pCell->CellID;
		}
		else
		{
			strID = pCell->InnerID;
		}

		theDataPCData.m_tCellLogData.TranferCellLogData(i, strID);	//SONIC-2013.06.18.

	} //of for i

	return TRUE;
}



//
// Alalrm 발생
void CMCMain::CIM_AlarmSet(UINT nErrID, CString sErrName)
{
	CString sMsg = sErrName.Left(MAX_MES_SIZE - 1);

	//SONIC-2013.05.28.
	TCHAR cALST[MAX_MES_SIZE];
	TCHAR cALCD[MAX_MES_SIZE];
	TCHAR cALID[MAX_MES_SIZE];
	TCHAR cALTX[MAX_MES_SIZE];

	if ( nErrID != ERR_NONE )
	{
		_tcscpy_s(cALST, _countof(cALST), _T(ALST_SET));
		_tcscpy_s(cALCD, _countof(cALCD), _T(ALCD_SERIOUS));
		_stprintf_s(cALID, _countof(cALID), _T("%d"), nErrID);
	//	_tcscpy_s(cALTX, _countof(cALTX), sErrName);
		_tcscpy_s(cALTX, _countof(cALTX), sMsg);
		theDataPCMessage.CmdAlarmSetClear(cALST, cALCD, cALID, cALTX);
	}
}

// Alarm Clear
void CMCMain::CIM_AlarmClear(UINT nErrID, CString sErrName)	
{
	CString sMsg = sErrName.Left(MAX_MES_SIZE - 1);

	//SONIC-2013.05.28.
	TCHAR cALST[MAX_MES_SIZE];
	TCHAR cALCD[MAX_MES_SIZE];
	TCHAR cALID[MAX_MES_SIZE];
	TCHAR cALTX[MAX_MES_SIZE];

	if ( nErrID != ERR_NONE )
	{
		_tcscpy_s(cALST, _countof(cALST), _T(ALST_CLEAR));
		_tcscpy_s(cALCD, _countof(cALCD), _T(ALCD_SERIOUS));
		_stprintf_s(cALID, _countof(cALID), _T("%d"), nErrID);
		// _tcscpy_s(cALTX, _countof(cALTX), sErrName);
		_tcscpy_s(cALTX, _countof(cALTX), sMsg);
		theDataPCMessage.CmdAlarmSetClear(cALST, cALCD, cALID, cALTX);
	}
}


// 작업자 로그인 보고
// 본 설비에서는 C Zone 작업자가 없기 때문에 D Zone으로 고정한다.
// -> 상위 전산에는 D존이 없고 C존만 있기 때문에 C존으로 고정하도록 변경한다.
void CMCMain::CIM_UserLogin(CString sID)					
{
	TCHAR cUserID[MAX_MES_SIZE];
	_tcscpy_s(cUserID, _countof(cUserID),	sID);

	// 2014-10-29, jhLee, 이충현K의 수정요청 -> 기존 E_D_ZONE -> E_C_ZONE으로 변경,
	//  현재 D Zone은 DataPC내에서 사용되지 않는다.
	theDataPCMessage.CmdUserLogin(CMessage::E_C_ZONE , cUserID);		
}

// 작업자 로그아웃 보고
// 본 설비에서는 C Zone 작업자가 없기 때문에 D Zone으로 고정한다.
// -> 상위 전산에는 D존이 없고 C존만 있기 때문에 C존으로 고정하도록 변경한다.
void CMCMain::CIM_UserLogout()					
{
	// 2014-10-29, jhLee, Logout시 무조건 Event 보내는것을 변경, 이충현K의 요청
	// 로그아웃 할 ID가 존재하는 경우에만 Logout 전송
	//if ( (sID.IsEmpty() == FALSE) && (sID != _T("NONE") ) )
	//{

		theDataPCMessage.CmdUserLogout(CMessage::E_C_ZONE);

	//}
}



// 2015-04-15, jhLee, 작업자 인증 처리 추가
// 작업자 로그인 가능 여부를 상위 서버에게 요청 한다
BOOL CMCMain::CIM_UserLoginRequest(CString sID, CString sPassword)
{

	TCHAR cUserID[MAX_MES_SIZE];
	TCHAR cPassword[MAX_MES_SIZE];

	// 작업자 사번(ID)와 암호를 전송하여 인증을 요청한다.
	_tcscpy_s(cUserID, _countof(cUserID),		sID);
	_tcscpy_s(cPassword, _countof(cPassword),	sPassword);

	return theDataPCMessage.CmdUserLoginRequest(CMessage::E_C_ZONE , cUserID, cPassword);		
}


// 작업자 로그아웃 가능 여부를 상위 서버에게 요청 한다
BOOL CMCMain::CIM_UserLogoutRequest(CString sID, CString sPassword)
{
	TCHAR cUserID[MAX_MES_SIZE];
	TCHAR cPassword[MAX_MES_SIZE];

	// 작업자 사번(ID)와 암호를 전송하여 인증을 요청한다.
	_tcscpy_s(cUserID, _countof(cUserID),		sID);
	_tcscpy_s(cPassword, _countof(cPassword),	sPassword);

	return theDataPCMessage.CmdUserLogoutRequest(CMessage::E_C_ZONE , cUserID, cPassword);		
}


// 2015-05-26, jhLee, Cell Tracking 정보 요청
BOOL CMCMain::CIM_CellTrackingInfoRequest(int nJig, int nCh)
{
	if ( m_rProfileData.bUseTest_BIN2TestMode ) return TRUE;	// BIN2 RUN mdoe, Data-PC로 전송하지 않는다.

	TCellData*	pCell; 

	// TCHAR cInnerID[MAX_MES_SIZE];
	TCHAR cCellID[MAX_CELL_ID];

	pCell = m_pCellInfo->GetCellData( nJig, nCh );								// 해당 채널의 Cell data를 취득

	if ( pCell != NULL )
	{
		if( pCell->bIsExist == TRUE )		
		{
			// _tcscpy_s(cInnerID,	_countof(cInnerID),	pCell->InnerID);	// 내부 CellID
			_tcscpy_s(cCellID,	_countof(cCellID),	pCell->CellID);	// MCR ID

			//// UV-MCR을 읽지 않도록 지정되어있다면,
			//if ( m_rProfileData.bUse_SkipMCR == TRUE ) {				
			//	bIsRead = FALSE;														// Cell ID는 읽지 않았다고 지정한다.
			//}
			//else
			//	bIsRead =  pCell->CellIDCheckup();								// 정상적인 Cell ID여부

			return theDataPCMessage.CmdCellInformationRequest( nJig, nCh, cCellID);				// 2015-05-26, jhLee, Cell Tracking 정보를 요청한다.
		}
	}//of if NOT Null

	return FALSE;
}


// 2016-01-04, Cell Lot Information 요청, POCH 처리용
BOOL CMCMain::CIM_CellLotInfoRequest(int nJig, int nCh)
{
	if ( m_rProfileData.bUseTest_BIN2TestMode ) return TRUE;	// BIN2 RUN mdoe, Data-PC로 전송하지 않는다.

	TCellData*	pCell; 

	// TCHAR cInnerID[MAX_MES_SIZE];
	TCHAR cCellID[MAX_CELL_ID];

	pCell = m_pCellInfo->GetCellData( nJig, nCh );								// 해당 채널의 Cell data를 취득

	if ( pCell != NULL )
	{
		if( pCell->bIsExist == TRUE )		
		{
			// _tcscpy_s(cInnerID,	_countof(cInnerID),	pCell->InnerID);	// 내부 CellID
			_tcscpy_s(cCellID,	_countof(cCellID),	pCell->CellID);	// MCR ID

			return theDataPCMessage.CmdCellLotInformationRequest( nJig, nCh, cCellID);				// 2015-05-26, jhLee, Cell Tracking 정보를 요청한다.
		}
	}//of if NOT Null

	return FALSE;
}


// PPID 관련
BOOL CMCMain::CIM_PPIDReport(CString sPPID)
{
	TCHAR cPPID[MAX_MES_SIZE];

	_tcscpy_s(cPPID, _countof(cPPID), sPPID);

	return theDataPCMessage.CmdCurPPIDReport(cPPID);
}

BOOL CMCMain::CIM_PPIDCreate(CString sPPID)
{
	TCHAR cPPID[MAX_MES_SIZE];

	_tcscpy_s(cPPID, _countof(cPPID), sPPID);

	return theDataPCMessage.CmdPPIDCreate(cPPID);
}

BOOL CMCMain::CIM_PPIDModify(CString sPPID)
{
	TCHAR cPPID[MAX_MES_SIZE];

	_tcscpy_s(cPPID, _countof(cPPID), sPPID);

	return theDataPCMessage.CmdPPIDModify(cPPID);
}

BOOL CMCMain::CIM_PPIDDelete(CString sPPID)
{
	TCHAR cPPID[MAX_MES_SIZE];

	_tcscpy_s(cPPID, _countof(cPPID), sPPID);

	return theDataPCMessage.CmdPPIDDelete(cPPID);
}

BOOL CMCMain::CIM_PPIDChange(CString sOldID, CString sNewID)
{
	TCHAR cOldPPID[MAX_MES_SIZE];
	TCHAR cNewPPID[MAX_MES_SIZE];

	_tcscpy_s(cOldPPID, _countof(cOldPPID), sOldID);
	_tcscpy_s(cNewPPID, _countof(cNewPPID), sNewID);

	return theDataPCMessage.CmdPPIDChange(cOldPPID, cNewPPID);
}



// 신호기 TMD가 변경되었을 경우 처리
BOOL CMCMain::CIM_ReportTMDChange()
{
	// 1. 변경된 TMD 관련 내용을 저장하고
	SaveRecipeData( _T("TMD_VERSION_CHECK") );	   // Recipe의 내용중 TMD 버전관리 항목들만 저장한다.

	// 2. 상위에 PPID Modify를 전송한다. 상위에서 요청이 들어오면 GaonNuri에서 Recipe 파일을 읽어 상위에 보고한다.
	return CIM_PPIDModify( GetRecipeModelName() );	// 상위에 Recipe 내용이 변경되었다고 보고한다.
}



//
// 2014-12-24, jhLee, 비가동 사유가 발생하였을 때 상위에 TP Code를 전송한다.
//
void CMCMain::CIM_TPSCodeReport(CString sCode, CString sMsg)
{
	TCHAR cTPCode[MAX_MES_SIZE];
	TCHAR cTPMsg[MAX_MES_SIZE];


	_tcscpy_s(cTPCode, _countof(cTPCode), sCode);
	_tcscpy_s(cTPMsg, _countof(cTPMsg), sMsg.Left(MAX_MES_SIZE-1));

//	theDataPCMessage.CmdPPIDChange(cTPCode, cTPMsg);	// PPID를 보내는게 아니라 LOSSCODE를 보내는 거니 주석 20141226-1 jhkim1
	theDataPCMessage.CmdSetLossCode(cTPCode, cTPMsg);	// PPID를 보내는게 아니라 LOSSCODE를 보내는 거니 추가 20141226-1 jhkim1
}

*/
//CIM end


// 2015-10-19, jhLee, 설비 DOWN시 처리하던 Cell 상위 보고
// PreAlign Station에서 Abort() 함수에서 처리한다.
// Abort발생시 Track-In 보고를 한 Cell이 아직 설비내 Track-Out 보고를 하지 않았다면 'R'로 보고를 한다.
//
void CMCMain::ProcessAbortReport()
{

	CGxLog* pLog = GetLogPointer( eLOG_SYS );

	// BIN 2 Test Mode일 경우에는 상위 보고에 대한 처리를 하지 않는다. 
	if ( m_rProfileData.bUseTest_BIN2TestMode == TRUE ) 
	{
		pLog->Write(_T("Skip Auto RETEST Track-OUT, BIN2-Test Mode"));			// 기능 skip 기록
		return;
	}

	// 2015-10-20, jhLee, 설비가 DOWN될 때 Track-in된 Cell을 자동으로 RETEST로 Track-Out하는 기능을 수행하지 않게할 것인가 ?
	if ( m_rProfileData.bUse_SkipAutoTrackout == TRUE ) 
	{
		pLog->Write(_T("Skip Auto RETEST Track-OUT, Option Off (bUse_SkipAutoTrackout)"));	// 기능 skip 기록
		return; 
	}


	CSvcTcpCIM* pSvcTcpCIM	= (CSvcTcpCIM*)( (CGxServiceHub::GetInstance())->GetService(NAME_SERVICE_TcpCIM));						// 가온누리 통신
	int i, j;
	BOOL bProcessFlag = FALSE;
	TCellData* pCell = NULL;

	for (i = 0; i < eMAX_JigCount; i++)
	{
		bProcessFlag = FALSE;									// 처리할 것이 없다고 초기화

		for (j = 0; j < eMAX_ChCount; j++)
		{
			pCell = m_pCellInfo->GetCellData(i, j);				// 지정 채널의 데이터 포인터 획득

			if ( !pCell ) continue;								// 포인터 획득 실패시 포기

			// Cell이 존재하고, Track-In 보고를 한 경우에만 처리
			if ( (pCell->bIsExist == TRUE) && (pCell->bTrackInReport == TRUE) )
			{
				// 작업중인 Cell을 RETEST로 처리한다.
				pCell->DZoneManualDefect		= _T("RETEST");							// 재검이름을 이름을 지정한다.
				pCell->DZoneManualClass			= eBIN2_CELL;							// Retest
				pCell->DZoneManualInspState	= eINSP_END;								// 검사 종료 지정
				pCell->DZoneManualCode			= _T("");								// 수동검사 Defect Code, CIM 보고용, jhLee 추가, (2014-07-15)
				//d pCell->DZoneOP_ID				= GetCurrentUserIDNo();					// 현재 검사 작업자 ID

				// 최종 판정을 BIN2로 지정
				pCell->FinalClass					= eBIN2_CELL;
				pCell->FinalDefect				= TEXT_RETEST;
				pCell->MesCode						= _T("");								// CIM 보고용 코드

				bProcessFlag = TRUE;													// 현재 JIG는 상위 보고를 할 필요가 있다.

				pLog->Write(_T("ABORT Auto RETEST Track-OUT, Jig:%d, Ch:%d, CellID:%s"), i, j, pCell->CellID);
			}
		} //of for j

		// 상위에 보고할 필요가 있다면
		if ( bProcessFlag == TRUE )
		{
			pSvcTcpCIM->CIM_GetOperationData( pCell );				// Data PC가 참조하는 결과 파일을 기록한다.
			SaveCellLogData( i );											// Cell log 기록

			// 2016-11-20, jhLee, Inspection Result Report를 전송한다.
			pSvcTcpCIM->CmdInspectionResultReport(pCell->JigNo, pCell->ChNo, _T("SKIP"));

			SleepEx(25, NULL);
			pSvcTcpCIM->CIM_CellProcessComplete( pCell );			// Track-Out을 보고한다.
		}

	}//of for i

}


// 사용 보류
////SONIC-2013.05.28.
//// 현재는x, 나중에 처리하기로 함
//void CMCMain::CIM_UnitTactReport(CMessage::E_UNIT eUnit, short sPoint, TAG_CELL tagCell)	//SONIC-2013.05.28.
//{
//	return;
//
//	if(tagCell.IsExist() == FALSE)
//		return;
//	TCHAR cInnerID[MAX_MES_SIZE];
//	TCHAR cCellID[MAX_MES_SIZE];
//	_tcscpy_s(cInnerID, _countof(cInnerID), theProcDataMgr.m_ProcData.m_DataPool.m_RuningCell[tagCell.inxCell].InnerID);
//	_tcscpy_s(cCellID, _countof(cCellID), theProcDataMgr.m_ProcData.m_DataPool.m_RuningCell[tagCell.inxCell].CellID);
//
//	if (eUnit==CMessage::E_UNIT_LOAD)
//	{
//		theDataPCMessage.CmdTactReport((CMessage::E_TACT_POINT)sPoint, cInnerID, cCellID);
//	}
//	else if (eUnit==CMessage::E_UNIT_INSPECTION)
//	{
//		sPoint = sPoint + CMessage::A_ZONE_1;
//		theDataPCMessage.CmdTactReport((CMessage::E_TACT_POINT)sPoint, cInnerID, cCellID);
//	}
//	else if (eUnit==CMessage::E_UNIT_UNLOAD)
//	{
//		sPoint = sPoint + CMessage::TR_2_1;
//		theDataPCMessage.CmdTactReport((CMessage::E_TACT_POINT)sPoint, cInnerID, cCellID);
//	}
//
//	_tcscpy_s(theDataPCData.m_cFdcUnitCellId[sPoint], cCellID);		//SONIC-2013.08.14.
//	theDataPCData.m_bFdcUnitCell[sPoint] = TRUE;
//
//}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// Cell Log
//


// 두 시간 사이의 차를 구해준다.
float CMCMain::CountTactTime(SYSTEMTIME StartTime, SYSTEMTIME EndTime)
{
	FILETIME fTm1,fTm2;
	ULONGLONG *ullVal1,*ullVal2;
	ULONGLONG ullDif;
	double dbTactTime;

	SystemTimeToFileTime(&StartTime,&fTm1);
	SystemTimeToFileTime(&EndTime,&fTm2);
	ullVal1 = (ULONGLONG *)&fTm1;
	ullVal2 = (ULONGLONG *)&fTm2;
	ullDif = *ullVal2 - * ullVal1;

	dbTactTime = double(ullDif/10000)/1000;

	// 1시간 limit
	if(dbTactTime > 3600 || dbTactTime < 0)	dbTactTime = 0;

	return (float)dbTactTime;
}



// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==



// 수신 처리 함수 치환
// 새로운 Cell log 경로를 지정한다.
//
// void* pValue -> CString pointer로 경로명을 되돌려준다.
void CMCMain::SetCellLogPath(void* pValue)
{
	CString* pFilename = (CString *)pValue;			// CString 형으로 변환하여 사용한ㄷ.

	CTime time = GetToday();						// '오늘'의 기준 날짜를 얻는다.

	CString sFullPath;

	if ( m_rProfileData.bUseTest_BIN2TestMode == TRUE )		// TEST Run (BIN2 Run) 동작 모드인경우 Cell Log를 다른곳에 기록을 남겨준다.
	{
		//OLD sFullPath.Format(_T("%s\\%02d\\CELL_DATA_BIN2\\%04d%02d%02d_CELL_%s_BIN2.csv"), NAME_DIR_CELLLOG, time.GetMonth(), time.GetYear(), time.GetMonth(), time.GetDay(), m_rProfileData.sCellLog_LineID );	// MINS에서 사용되는 라인내 설비 ID, 3자리 
		sFullPath.Format(_T("%s\\%02d\\CELL_DATA_BIN2\\%04d%02d%02d_CELL_%s_BIN2.csv"), m_rProfileData.sDIR_CellLog, time.GetMonth(), time.GetYear(), time.GetMonth(), time.GetDay(), m_rProfileData.sCellLog_LineID );	// MINS에서 사용되는 라인내 설비 ID, 3자리 
		
	}
	else
	{
		//OLD sFullPath.Format(_T("%s\\%02d\\CELL_DATA\\%04d%02d%02d_CELL_%s.csv"), NAME_DIR_CELLLOG, time.GetMonth(), time.GetYear(), time.GetMonth(), time.GetDay(), m_rProfileData.sCellLog_LineID );	// MINS에서 사용되는 라인내 설비 ID, 3자리 
		sFullPath.Format(_T("%s\\%02d\\CELL_DATA\\%04d%02d%02d_CELL_%s.csv"), m_rProfileData.sDIR_CellLog, time.GetMonth(), time.GetYear(), time.GetMonth(), time.GetDay(), m_rProfileData.sCellLog_LineID );	// MINS에서 사용되는 라인내 설비 ID, 3자리 
	}

	*pFilename = sFullPath;		// 만들어진 경로명을 지정
}


// 통계용 생산 정보를 저장한다.
// 기존에 SaveCellLogData() 함수내에 구현하였던 내용을 기능 별로 분리하여 별도의 파일로 이동시킴
//
void CMCMain::SaveProductData(int nJig)
{

	int i;
	TCellData* pCell = NULL;	// [eMAX_ChCount];		


	// 지정 지그의 모든 채널을 대상으로 한다.
	for( i = 0; i< eMAX_ChCount; i++)
	{
		pCell	= m_pCellInfo->GetCellData(nJig, i);

		if ( (pCell->bIsExist == FALSE) || (pCell->bIsCellDetect == FALSE) )	continue;	// 해당 셀에 존재하지 않으면 다음 셀로 넘기자;
	
		// 2015/03/29 JH-PARk

		// 해당 채널에 대한 CONTACT횟수를 가져온다 , 삼성 김길성K 요청사항
		SaveTodayProductData(eIDX_Product_TodayContact, pCell->ContactCount);

		//2015/05/03 JH-PARK
		// CH별 contact 횟수를 따로 관리한다
		if ( pCell->JigNo == eIDX_JIG_1 )
		{
			SaveTodayProductData(eIDX_Product_TodayCH1_ContactGOOD + pCell->ChNo ,pCell->ContactGoodCount);
			SaveTodayProductData(eIDX_Product_TodayCH1_ContactNG + pCell->ChNo ,pCell->ContactNGCount);
		}
		else
		{
			SaveTodayProductData(eIDX_Product_TodayCH3_ContactGOOD + pCell->ChNo ,pCell->ContactGoodCount);
			SaveTodayProductData(eIDX_Product_TodayCH3_ContactNG + pCell->ChNo ,pCell->ContactNGCount);
		}
		
		// 2015/05/03 JH-PARk
		// No Display 선택여부를 통계로 확인하고 싶다. ( 김정유D)
		if ( pCell->MesCode == _T("USF02") )
		{
			SaveTodayProductData(eIDX_Product_TodayNoDisplay);
		}

	}//of for i
		
}


//
// 새로운 방식으로 지정 지금의 모든 Cell에 대하여 Cell Log를 File로 기록한다.
//
// 2015-03-30, jhLee, TG_CHECK 항목을 추가하였다.
//
BOOL CMCMain::SaveCellLogData(int nJig)
{
	CJigInfo* pJigInfo = CJigInfo::GetInstance();

	int i;
	double dbTacktime;	// Tack Time 선언(Start - End);

	TCellData* pCell = NULL;	// [eMAX_ChCount];		
	// CChannelData* pChData = NULL;	// pJigInfo->GetChData(nJigNo, ch);	// 지정 지그 및 채널 데이터


	// 지정 지그의 Cell 정보 포인터를 가져온다.
	// pCell[eIDX_CH_2]	= m_pCellInfo->GetCellData(nJig, eIDX_CH_2);

	// 지정 지그의 모든 채널을 대상으로 한다.
	for( i = 0; i< eMAX_ChCount; i++)
	{
		pCell	= m_pCellInfo->GetCellData(nJig, i);

		if ( (pCell->bIsExist == FALSE) || (pCell->bIsCellDetect == FALSE) )		continue;	// 해당 셀에 존재하지 않으면 다음 셀로 넘기자;
		
		// 일반 모드인지 BIN2 테스트 모드인지 구분하여 각기 다른 파일에 리록을 한다.
		CGxLogData* pCellLog = ( m_rProfileData.bUseTest_BIN2TestMode ) ? &m_logCellBIN2Log : &m_logCellLog;


		// INNER ID;
		pCellLog->AddItem( pCell->InnerID );
		// MODEL;
		pCellLog->AddItem(m_rProfileData.sModelName);
	
		// CELL ID;	
		// 본설비에서는 반드시 UV-MCR을 읽어야 진행이 되므로 Cell-ID는 반드시 존재한다.
		pCellLog->AddItem( pCell->CellID );		//  == TEXT_UV_FAIL ? pCell->InnerID:pCell->CellID);
	
		// READ UNIT;
		if ( pCell->nMCRReadUnit == eMCR_UNIT_Auto )
		{
			pCellLog->AddItem( _T("AUTO") );			// MCR을 읽어들인 장치, UV:UV-MCR, CELL:Cell MCR
		}
		else if ( pCell->nMCRReadUnit == eMCR_UNIT_Manual )
		{
			pCellLog->AddItem( _T("MANUAL") );
		}
		else 
		{
			pCellLog->AddItem( _T("NONE") );
		}

		// EQP ID : 2015-09-30, jhLee, 설비 ID를 기입하도록 변경
		// L_ALIGN SUCESS;
		// VT Align으로 대체
		// pCellLog->AddItem(_T(""));	// (pCell->PreAlignResultXY == eAlignResult_GOOD) ? TEXT_GOOD:TEXT_NG);
		pCellLog->AddItem( m_rProfileData.sEQID );			// EQP ID를 기록 남긴다.
	
		// JIG NAME;
		pCellLog->AddItem_Int( pCell->JigNo + 1 );

		// CH NAME;
		pCellLog->AddItem_Int( pCell->ChNo + 1 );

		// Contact Count
		pCellLog->AddItem_Int( pCell->ContactCount );


		// MC_Ver;
		pCellLog->AddItem( m_rProfileData.sVersion );


		// 2015-10-19, jhLee, Product ID
		// L_Ver // 미사용
		pCellLog->AddItem( pCell->MesProductID );			// Cell Tracking Information 응답의 Product ID를 출력
	
		// U_Ver
		// 미사용
		pCellLog->AddItem( _T("") );

		// D_Ver
		// Data-PC 버전
		//d strTemp = theDataPCData.m_tEqpData.m_cVersionUseCellLog;
		pCellLog->AddItem( m_rProfileData.sDataPCVersion );

		// UNIATS VERSION
		pCellLog->AddItem( _T("") );

		// MICRO DLL VERSION
		pCellLog->AddItem( _T("") );

		// MACRO DLL VERSION
		pCellLog->AddItem( _T("" ));

		// SETBIN VERSION
		pCellLog->AddItem( _T("") );

		// UNLOADER OUT TACK TIME
		pCellLog->AddItem( _T("") );


		// BIN2 CELL INFO
		//! 임의로 G 지정
		if ( !m_rProfileData.bUseTest_BIN2TestMode )
		{
			pCellLog->AddItem( _T("G") );					// 정상적인 실행 모드
		}
		else
			pCellLog->AddItem( _T("T") );					// BIN2 RUN TEST 모드

		//? 문의

		//2014-07-08, OSC
		// 수율 실적 관리를 위해 본설비에서 너무 많은 불량을 양산하여 수율하락의 주원인으로 지목되는 
		// 경우를 방지하기 위해 예외처리를 한다. (고객사 요청)
		// 1. GOOD 모드 : 양품만 보고,
		// 2. NG 모드 : 양품/BIN2/Reject 모두 보고 -> 정상적인 경우
		// 3. TEST mode : 모두 보고를 하지 않고, 비가동 로그상 가동내내 'Down'으로 간주한다.


		//if(theParamMgr.m_Opt.MODE.modeCellInput == INPUT_MODE_GOOD)
		//{
		//	strCell.Format(_T(",%s")								,_T("G"));
		//	CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		//	m_fileCellData.Write(szBuf, strlen(szBuf));
		//}
		//else if(theParamMgr.m_Opt.MODE.modeCellInput == INPUT_MODE_NG)
		//{
		//	strCell.Format(_T(",%s")								,_T("N"));
		//	CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		//	m_fileCellData.Write(szBuf, strlen(szBuf));
		//}
		//else if( (theParamMgr.m_Opt.MODE.modeCellInput == INPUT_MODE_TEST) || (theParamMgr.m_Opt.MODE.modeCellInput == INPUT_MODE_JOB_CHANGE) )
		//{
		//	strCell.Format(_T(",%s")								,_T("T"));
		//	CEtc::ConvertMultibyte((wchar_t*)strCell.GetString()	, szBuf, 256);
		//	m_fileCellData.Write(szBuf, strlen(szBuf));
		//}


		// 2015-07-15, jhLee, 신호기 TMD 정보를 새로운 방식으로 남기도록 한다.
		// PG_UI;
		//old pCellLog->AddItem( pCell->PGVer);
		pCellLog->AddItem( pCell->PG_Version );			// pChData->sPG_Version );

		// PROGRAM NAME;
		//old pCellLog->AddItem( pCell->TMD_Info);
		pCellLog->AddItem( pCell->PG_TMDModel );			// pChData->sPG_TMDModel );

		// CH Card MAC;
		//old pCellLog->AddItem( pCell->CH_Card_Mac);
		pCellLog->AddItem( pCell->PG_MACAddress );		// pChData->sPG_MACAddress );


		// JIG NO;
		pCellLog->AddItem_Int( pCell->JigNo);

		// CIM LOG;
		pCellLog->AddItem( pCell->MesCellInfoResult);		// Cell Tracking 정보

		// CIM MTP;
		pCellLog->AddItem( _T("0"));

		// CIM TSP;
		pCellLog->AddItem( _T("0"));


		// TACT TIME;
		// 투입부터 배출까지의 시간
		pCellLog->AddItem_Double( CountTactTime(pCell->cellTime.systmA_StartTime, pCell->cellTime.systmD_EndTime), 2 );

		// TABLE TURN TIME;
		pCellLog->AddItem_Double( CountTactTime(pCell->cellTime.systm_TableTurn_StartTime, pCell->cellTime.systm_TableTurn_EndTime), 2);

		// CELL LOAD TACT
		// 본 설비에는 Loader가 없기에 항상 0이 출력된다.
		pCellLog->AddItem_Double( 0.0, 2 );	// pCell->LoadTact, 2);
		

		//OLD CELL UNLOAD TACT START TIME
		// 이전 JIG의 PG-OFF 시각을 표시한다.
		pCellLog->AddItem_DateTime( pCell->tmPrevPGOffTime, TRUE );				// 이전 JIG의 PG Off 시각을 출력한다.
	
		//old CELL UNLOAD TACT
		// PG-OFF 시각을 기준으로 TACT을 계산하여 되돌려 받는다.
		pCellLog->AddItem_Double( pCell->dbMoveTACT[eTACT_JigTACTTime], 2);
		//OLD CELL UNLOAD TACT END TIME

		//2014-08-28, 김상묵S 요청,
		// 이번 Cell의 PG-OFF 시각 
		pCellLog->AddItem_DateTime( pCell->tmMoveTACT[eTACT_UnloadPGOff], TRUE );	// GetPGOffTimeString(nJig)); // PG-OFF 시각

		// LAST CLASS;
		pCellLog->AddItem_Int( pCell->FinalClass);
		//old pCellLog->AddItem_Int( pCell->FinalClass == eGOOD_CELL ? CELL_LOG_OK:CELL_LOG_NG);
	


		// LAST RESULT;
		pCellLog->AddItem( pCell->FinalDefect);
	
		// PRINT DEFECT
		if ( pCell->FinalClass == eLOSS_CELL )		// 라벨 프린터르 출력하는 조건일 경우에만 기록에 남긴다.
		{
			pCellLog->AddItem( pCell->FinalLabel);
		}
		else
			pCellLog->AddItem( _T("") );
	
		// PRINT CODE
		pCellLog->AddItem( pCell->FinalCode );		// PrintCode );

		// MES CODE
		pCellLog->AddItem( pCell->MesCode);


		// AZONE START TIME;
		pCellLog->AddItem_DateTime( pCell->cellTime.systmA_StartTime, TRUE );
		
		// AZONE END TIME;
		pCellLog->AddItem_DateTime( pCell->cellTime.systmA_EndTime, TRUE);

		// AZONE TACK TIME;
		pCellLog->AddItem_Double( CountTactTime(pCell->cellTime.systmA_StartTime, pCell->cellTime.systmA_EndTime), 2);

		// AZONE WAITING;
		pCellLog->AddItem_Double( CountTactTime(pCell->cellTime.systmA_EndTime, pCell->cellTime.systmA_WaitTime), 2);

		// AZONE CLASS;
		pCellLog->AddItem_Int( pCell->AZoneClass);

		// AZONE DEFECT;
		pCellLog->AddItem( pCell->AZoneDefect);

		// AZONE TSP DEFECT;
		pCellLog->AddItem( _T("")	);	// pCell->TspPin);

		// AZONE REG DEFECT;
		pCellLog->AddItem( _T("")	);	// pCell->RegPin);

		// AZONE CONTACT START TIME;
		pCellLog->AddItem_DateTime( pCell->cellTime.systmA_ContactStartTime, TRUE);

		// AZONE CONTACT END TIME;
		pCellLog->AddItem_DateTime( pCell->cellTime.systmA_ContactEndTime, TRUE);

		// AZONE CONTACT TACK TIME;
		pCellLog->AddItem_Double( CountTactTime(pCell->cellTime.systmA_ContactStartTime, pCell->cellTime.systmA_ContactEndTime), 2);

		// AZONE CONTACT CLASS;
		pCellLog->AddItem_Int( pCell->ContactCurrentClass );	//old  == eGOOD_CELL ? CELL_LOG_OK:CELL_LOG_NG);

		// AZONE Contact IBAT
		pCellLog->AddItem_Double( pCell->ContactCurrent[0], 2);

		// AZONE Contact IVSS
		pCellLog->AddItem_Double( pCell->ContactCurrent[1], 2);

		// AZONE Contact IVDD
		pCellLog->AddItem_Double( pCell->ContactCurrent[2], 2);

		// AZONE Contact IVCI
		pCellLog->AddItem_Double( pCell->ContactCurrent[3], 2);

		// AZONE Contact IBAT2
		pCellLog->AddItem_Double( pCell->ContactCurrent[4], 2);

		// AZONE Contact IDD2
		pCellLog->AddItem_Double( pCell->ContactCurrent[5], 2);


		// AZONE CURRENT START TIME
		pCellLog->AddItem( NAME_VALUE_DUMMY_DATE );

		// AZONE CURRENT END TIME
		pCellLog->AddItem( NAME_VALUE_DUMMY_DATE );

		// AZONE CURRENT TACK TIME
		// 		fTacktime = CountTactTime(pCell->cellTime.systmA_CurrentStartTime, pCell->cellTime.systmA_CurrentEndTime);
		pCellLog->AddItem_Double( 0.0, 2);

		// AZONE CURRENT CLASS
		pCellLog->AddItem_Int( 0 );

		// AZONE IBAT
		pCellLog->AddItem_Double( 0.0, 2);

		// AZONE IVSS
		pCellLog->AddItem_Double( 0.0, 2);

		// AZONE IVDD
		pCellLog->AddItem_Double( 0.0, 2);

		// AZONE IVCI
		pCellLog->AddItem_Double( 0.0, 2);

		// AZONE IBAT2
		pCellLog->AddItem_Double( 0.0, 2);

		// AZONE IDD2
		pCellLog->AddItem_Double( 0.0, 2);

		// AZONE ALIGN START TIME
		pCellLog->AddItem_DateTime( pCell->cellTime.systmA_AlignStartTime, TRUE);

		// AZONE ALIGN END TIME
		pCellLog->AddItem_DateTime( pCell->cellTime.systmA_AlignEndTime, TRUE);

		// AZONE ALIGN TACK TIME
		pCellLog->AddItem_Double( CountTactTime(pCell->cellTime.systmA_AlignStartTime, pCell->cellTime.systmA_AlignEndTime), 2);

		// AZONE ALIGN DEGREE
		pCellLog->AddItem_Double( pCell->VT_Align_Data_th, 3);

		// AZONE ALIGN SUCCESS
		pCellLog->AddItem( pCell->VT_AlignResult == eGOOD_CELL ? TEXT_GOOD:TEXT_FAIL );
	
		// N/A
		pCellLog->AddItem( _T("") );

		// N/A
		pCellLog->AddItem( _T("") );


		// Start of B-Zone - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		// B-Zone 삭제?? -> 일단 보류 (사귀진S), jhLee, 2014-07-20
		// 2014-08-11, B-Zone 삭제

		// end of B-Zone - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


		// CZONE START TIME
		pCellLog->AddItem_DateTime( pCell->cellTime.systmC_StartTime, TRUE);

		// CZONE END TIME
		pCellLog->AddItem_DateTime( pCell->cellTime.systmC_EndTime, TRUE);

		// CZONE TACK TIME
		pCellLog->AddItem_Double( CountTactTime(pCell->cellTime.systmC_StartTime, pCell->cellTime.systmC_EndTime), 2);

		// CZONE WAITING
		pCellLog->AddItem_Double( CountTactTime(pCell->cellTime.systmC_EndTime, pCell->cellTime.systmC_WaitTime), 2);

		// CZONE CLASS
		pCellLog->AddItem_Int( pCell->CZoneClass);

		// CZONE DEFECT
		pCellLog->AddItem( pCell->CZoneDefect);


		// 이동 ! 기존 B-Zone -> C-Zone으로
		//CZONE VISION START TIME
		pCellLog->AddItem_DateTime( pCell->cellTime.systmC_VisionStartTime, TRUE );
	
		// CZONE VISION END TIME
		pCellLog->AddItem_DateTime( pCell->cellTime.systmC_VisionEndTime, TRUE);

		// CZONE VISION TACK TIME
		pCellLog->AddItem_Double( CountTactTime(pCell->cellTime.systmC_VisionStartTime, pCell->cellTime.systmC_VisionEndTime), 2);

		// BZONE VISION GRAB TIME
		pCellLog->AddItem_Double( pCell->VTGrabTime, 2);			// 검사 PC에서 보내온 Grab 소요시간 기록

		// BZONE VISION CLASS TIME
		pCellLog->AddItem_Double( pCell->VTClassTime, 2);			// 검사 PC에서 보내온 Class End 소요시간 기록

		// BZONE VISION CLASS
		pCellLog->AddItem_Int( pCell->VTClassJudge);

		// BZONE VISION DEFECT
		pCellLog->AddItem( pCell->VTResult );

		//end of C Zone Vision -------

		// 2015-03-30, 사용되지 않는 C-Zone Current 항목을 새로이 추가되는 TE_CHECK 항목으로 용도 변경하여 사용한다.
		// TE CHECK 시작 시각 : 명령 전송 시각
		// CZONE CURRENT START TIME -> TE CHECK START TIME
		//OLD pCellLog->AddItem( NAME_VALUE_DUMMY_DATE );
		pCellLog->AddItem_DateTime( pCell->tmMoveTACT[eTACT_PG_TECheckStart], TRUE );

		// TE CHECK 종료 시각 : 결과 수신 시각 
		// CZONE CURRENT END TIME -> TE CHECK END TIME
		//OLD pCellLog->AddItem( NAME_VALUE_DUMMY_DATE );
		pCellLog->AddItem_DateTime( pCell->tmMoveTACT[eTACT_PG_TECheckEnd], TRUE  );

		// TE CHECK 소요 시간 : 명령 전송 ~ 결과 수신까지의 시간
		// CZONE CURRENT TACK TIME -> TE CHECK TACK TIME
		//OLD pCellLog->AddItem_Double( 0.0, 2);
		pCellLog->AddItem_Double( pCell->dbMoveTACT[eTACT_PG_TECheckEnd], 2);

		// TE CHECK 결과
		// CZONE CURRENT CLASS -> TE CHECK RESULT
		//OLD pCellLog->AddItem( _T("") );
		pCellLog->AddItem( (pCell->TE_CheckClass == eGOOD_CELL) ? TEXT_GOOD : TEXT_NG  );			// 판정 결과를 기록한다.

		// TE CHECK 수행 여부
		// C ZONE CURRENT IBAT -> TE CHECK EXECUTE
		//OLD pCellLog->AddItem( _T("") );
		pCellLog->AddItem( (pCell->TE_CheckProcessFlag == TRUE) ? TEXT_YES : TEXT_NO  );


		// 2016-03-18, jhLee, TSP 테스트 추가
		// CZONE TSP START TIME,	//old CZONE IVSS
		pCellLog->AddItem_DateTime( pCell->cellTime.systmC_TspStartTime, TRUE);

		// CZONE TSP END TIME,		//old CZONE IVDD
		pCellLog->AddItem_DateTime( pCell->cellTime.systmC_TspEndTime, TRUE);

		// CZONE TSP TACT TIME,		//old CZONE IVCI
		pCellLog->AddItem_Double(  CountTactTime(pCell->cellTime.systmC_TspStartTime, pCell->cellTime.systmC_TspEndTime), 2);

		// CZONE TSP RESULT,		//old CZONE IBAT2
		pCellLog->AddItem( pCell->TspNGMainName );		// TSP 대표 불량명 기록

		// CZONE TSP EXECUTE,		//old CZONE IDD2
		pCellLog->AddItem( (pCell->TspInspState == eINSP_NONE) ? _T("0") : _T("1") );		// TSP 테스트 진행여부


		// White Current
		// CZONE WHITE CURRENT START TIME
		pCellLog->AddItem( NAME_VALUE_DUMMY_DATE );

		// CZONE WHITE CURRENT END TIME
		pCellLog->AddItem( NAME_VALUE_DUMMY_DATE );


		// CZONE WHITE CURRENT TACK TIME
		// CountTactTime(pCell->cellTime.systmC_WhiteCurrentStartTime, pCell->cellTime.systmC_WhiteCurrentEndTime);
		pCellLog->AddItem_Double( 0.0, 2);

		// CZONE WHITE CURRENT CLASS
		pCellLog->AddItem_Int( pCell->WhiteCurrentClass );	//  == eGOOD_CELL ? CELL_LOG_OK:CELL_LOG_NG);

		// CZONE WHITE IBAT
		pCellLog->AddItem_Double( pCell->WhiteCurrent[0], 2);

		// CZONE WHITE IVSS
		pCellLog->AddItem_Double( pCell->WhiteCurrent[1], 2);

		// CZONE WHITE IVDD
		pCellLog->AddItem_Double( pCell->WhiteCurrent[2], 2);

		// CZONE WHITE IVCI
		pCellLog->AddItem_Double( pCell->WhiteCurrent[3], 2);

		// CZONE WHITE IBAT2
		pCellLog->AddItem_Double( pCell->WhiteCurrent[4], 2);

		// CZONE WHITE IDD2
		pCellLog->AddItem_Double( pCell->WhiteCurrent[5], 2);

		// N/A
		pCellLog->AddItem( _T(""));

		// N/A
		pCellLog->AddItem( _T(""));


		// D-Zone --

		// DZONE START TIME
		pCellLog->AddItem_DateTime( pCell->cellTime.systmD_StartTime, TRUE);

		// DZONE END TIME
		pCellLog->AddItem_DateTime( pCell->cellTime.systmD_EndTime, TRUE);


		// DZONE TACK TIME
		pCellLog->AddItem_Double(  CountTactTime(pCell->cellTime.systmD_StartTime, pCell->cellTime.systmD_EndTime), 2);


		// DZONE WAITING TIME
		pCellLog->AddItem_Double(  CountTactTime(pCell->cellTime.systmD_EndTime, pCell->cellTime.systmD_WaitTime), 2);

		// DZONE CLASS
		pCellLog->AddItem_Int( pCell->DZoneClass);

		// DZONE DEFECT
		pCellLog->AddItem( pCell->DZoneDefect);

		// DZONE MANUAL2 START TIME
		pCellLog->AddItem_DateTime( pCell->cellTime.systmD_ManualStartTime, TRUE);
		

		// DZONE MANUAL2 END TIME
		pCellLog->AddItem_DateTime( pCell->cellTime.systmD_ManualEndTime, TRUE);


		// DZONE MANUAL2 TACK TIME
		pCellLog->AddItem_Double( CountTactTime(pCell->cellTime.systmD_ManualStartTime, pCell->cellTime.systmD_ManualEndTime), 2);

		// DZONE MANUAL2 CLASS
		pCellLog->AddItem_Int( pCell->DZoneManualClass );	//  == eGOOD_CELL ? CELL_LOG_OK:CELL_LOG_NG);
	
		// DZONE MANUAL2 DEFECT
		pCellLog->AddItem( pCell->DZoneManualDefect);


		// DZONE CURRENT START TIME
		pCellLog->AddItem( NAME_VALUE_DUMMY_DATE );

		// DZONE CURRENT END TIME
		pCellLog->AddItem( NAME_VALUE_DUMMY_DATE );


		// DZONE CURRENT TACK TIME
		// CountTactTime(pCell->cellTime.systmD_CurrentStartTime, pCell->cellTime.systmD_CurrentEndTime);
		pCellLog->AddItem_Double( 0.0, 2);


		//// CZONE CURRENT CLASS
		//pCellLog->AddItem( _T("") );


		// DZONE CURRENT CLASS
		pCellLog->AddItem( _T("") );

		// DZONE IBAT
		pCellLog->AddItem( _T("") );

		// DZONE IVSS
		pCellLog->AddItem( _T("") );

		// DZONE IVDD
		pCellLog->AddItem( _T("") );

		// DZONE IVCI
		pCellLog->AddItem( _T("") );

		// DZONE IBAT2
		pCellLog->AddItem( _T("") );

		// DZONE IDD2
		pCellLog->AddItem( _T("") );

		// SLEEP CURRENT START TIME
		pCellLog->AddItem( NAME_VALUE_DUMMY_DATE );

		// SLEEP CURRENT END TIME
		pCellLog->AddItem( NAME_VALUE_DUMMY_DATE );


		// SLEEP CURRENT TACK TIME
		// CountTactTime(pCell->cellTime.systm_SleepCurrentStartTime, pCell->cellTime.systm_SleepCurrentEndTime);
		pCellLog->AddItem_Double( 0.0, 2);

		// SLEEP CURRENT CLASS
		pCellLog->AddItem_Int( pCell->SleepCurrentClass );	// == eGOOD_CELL ? CELL_LOG_OK:CELL_LOG_NG);

		// SLEEP IBAT
		pCellLog->AddItem_Double( pCell->SleepCurrent[0], 2);

		// SLEEP IVSS
		pCellLog->AddItem_Double( pCell->SleepCurrent[1], 2);

		// SLEEP IVDD
		pCellLog->AddItem_Double( pCell->SleepCurrent[2], 2);

		// SLEEP IVCI
		pCellLog->AddItem_Double( pCell->SleepCurrent[3], 2);

		// SLEEP IBAT2
		pCellLog->AddItem_Double( pCell->SleepCurrent[4], 2);

		// SLEEP IDD2
		pCellLog->AddItem_Double( pCell->SleepCurrent[5], 2);

		// N/A
		pCellLog->AddItem( _T(""));

		// N/A
		// strCell.Format(_T(",%s\n")								,_T(""));
		pCellLog->AddItem( _T(""));


		// NEW Tact Time - - - - - - - - - - 
		//OLD Header strTemp += _T(",PG OFF TIME,PG ON TIME,TACT CONTACT,READY BTN TIME,TACT A ZONE INSP,TURN START TIME,TACT ALIGN,VI CH1 TIME,TACT VI CH1,VI CH2 TIME,TACT VI CH2,D ZONE PG OFF TIME,TACT MANUAL INSP,TACT TOTAL,N/A,N/A\n");

		// NEW
		// sHeader += _T(",TIME PG OFF1,TIME PG ON,TIME START BUTTON,TIME MCR ALIGN,TIME CELL LOADING,TIME WAITING1,TIME TABLE TURN1,TIME VISION 1CH,TIME CAMERA 2CH MOVE,TIME VISION 2CH,TIME WAITING2,TIME TABLE TURN2,TIME PG OFF2,EMPTY,N/A,N/A");
		// sHeader +=_T(",ID,CYCLE TIME,EQ TIME,OP TIME,WAIT TIME,LOSS TIME,NO INPUT TIME,OPEN CELL,N/A,N/A");
		// sHeader += _T(",TACT LOAD UNLOAD,TACT A ZONE INSP,TACT A ZONE MCR ALIGN,TACT A ZONE,TACT WAITING1,TACT TABLE TURN1,TACT C ZONE VISION 1CH,TACT C ZONE CAMERA MOVE,TACT C ZONE VISION 2CH,TACT C ZONE,TACT WAITING2,TACT TABLE TURN2,TACT D ZONE,TACT TIME,TOTAL TACT,N/A,N/A");


		// 1. TIME PG OFF1, // Loading시 사용하는 PG-OFF Time = 이전 Jig의 Unloading PG-OFF
		pCellLog->AddItem_DateTime(  pCell->tmMoveTACT[eTACT_LoadPGOff], TRUE );

		// 2. TIME PG ON // 최초로 신호기로 TURNON을 전송한 시각, 작업자 검사 시작 (2015-07-08, jhLee, 추가)
		pCellLog->AddItem_DateTime(  pCell->tmMoveTACT[eTACT_TurnOn], TRUE );

		// 3. TIME START BUTTON 	// 양수버튼	Loading 및 A-Zone 검사 완료
		pCellLog->AddItem_DateTime(  pCell->tmMoveTACT[eTACT_ReadyBtn], TRUE );

		// 4. TIME MCR ALIGN // 왼쪽 Align 마크 찾기 및 Align 계산 완료, Turn 대기 시작
		pCellLog->AddItem_DateTime(  pCell->tmMoveTACT[eTACT_AlignLeftMark], TRUE );

		// 5. TIME CELL LOADING // 신호기로부터 Cell Loading 완료 신호를 받는 대기 시간,	#2015-05-26, jhLee 추가
		pCellLog->AddItem_DateTime(  pCell->tmMoveTACT[eTACT_PGCellLoading], TRUE );
		
		// 6. TIME WAITING1 	// C-Zone으로의 회전 이동 시각, 회전 대기 완료
		pCellLog->AddItem_DateTime(  pCell->tmMoveTACT[eTACT_TurnMoveStart], TRUE );

		// 7. TIME TABLE TURN1 // C-Zone으로의 회전 이동 완료
		pCellLog->AddItem_DateTime(  pCell->tmMoveTACT[eTACT_TurnMoveEnd], TRUE );

		// 8. TIME VISION 1CH	// 비전검사 채널#1 Grab 완료, CH#2 이동 시작
		pCellLog->AddItem_DateTime(  pCell->tmMoveTACT[eTACT_VI_CH1_GrabEnd], TRUE );

		// 9. TIME CAMERA 2CH MOVE // 비전검사 채널#2 이동 및 Align 동작 완료, Vision 2Ch Start
		pCellLog->AddItem_DateTime(  pCell->tmMoveTACT[eTACT_VI_CH2_AlignMove], TRUE );

		// 10. TIME VISION 2CH // 비전검사 채널#2 Grab 완료, Return Move 대기 시작
		pCellLog->AddItem_DateTime(  pCell->tmMoveTACT[eTACT_VI_CH2_GrabEnd], TRUE );

		// 11. TIME WAITING2, // A-Zone으로의 회전 이동 시각, 회전 대기 완료
		pCellLog->AddItem_DateTime(  pCell->tmMoveTACT[eTACT_ReturnMoveStart], TRUE );

		// 12. TIME TABLE TURN2  // A-Zone으로의 회전 이동 완료
		pCellLog->AddItem_DateTime(  pCell->tmMoveTACT[eTACT_ReturnMoveEnd], TRUE );

		// 13. TIME PG OFF2 // 작업자 검사 완료
		pCellLog->AddItem_DateTime(  pCell->tmMoveTACT[eTACT_UnloadPGOff], TRUE );
	
		// 14. EMPTY
		pCellLog->AddItem( _T(""));

		// N/A 2개
		pCellLog->AddItem( _T(""));
		pCellLog->AddItem( _T(""));


		// 2014-10-13, jhLee, 백윤기S 요청, 가장 마지막에 작업자 사원번호를 삽입한다.
		pCellLog->AddItem( GetCurrentUserIDNo()  );		
	
		// 2014-10-16, jhLee, 설비 및 OP TACT 추가
		// 실제로는 TACT Time이지만, 김길성 책임의 요청으로 'Cycle Time'이란 명칭으로 사용된다.

		//2014-10-16, jhLee, 60초를 넘기는 TACT Time은 공백으로 처리한다. (백윤기S)
		//2014-11-18, jhLee, 60초를 Option으로 처리하고, 관련 모든 TACT을 공백으로 처리한다.
		dbTacktime = pCell->dbMoveTACT[eTACT_JigTACTTime];			

		// 기준 시간이내에 진입한다면
		if ( dbTacktime < (double)(m_rProfileData.nTIME_RunDownTime) ) {	

			// 정상적으로 해당 시간을 기록한다.
			pCellLog->AddItem_Double(  pCell->dbMoveTACT[eTACT_JigTACTTime], 3);							//  Jig별 생산 TACT Time

			pCellLog->AddItem_Double(  pCell->dbMoveTACT[eTACT_EQTime], 3);								// 설비 Time

			pCellLog->AddItem_Double(  pCell->dbMoveTACT[eTACT_OPTime], 3);								// 작업자 Time
		}
		else		// 기준 시간을 넘어선다면, 각 TACT time 값을 공백으로 대체한다.
		{
			pCellLog->AddItem( _T("") );						//  Jig별 생산 TACT Time

			pCellLog->AddItem( _T("") );						// 설비 Time

			pCellLog->AddItem( _T("") );						// 작업자 Time
		}

		//2014-11-07, jhlee, 대기 시간을 추가한다. (김길성 책임 요청)
		pCellLog->AddItem_Double( pCell->dbMoveTACT[eTACT_WaitTime], 3);							// 작업을 마쳤으나 이전 JIG의 작업이 지연이되어 발생하는 대기 시간

		// 비가동 시각 (Alarm/Stop ...)
		pCellLog->AddItem_Double( pCell->dbMoveTACT[eTACT_LossTime], 3);							// 비가동 Time, 설비가 멈춰진 시간 

		// 2014-11-18, jhLee, TACTTime 공백처리 기준시간 표시
		pCellLog->AddItem_Int(  m_rProfileData.nTIME_RunDownTime);								// TACTTime 공백처리 기준시간

		//// OPEN CELL
		pCellLog->AddItem_Int( (int)m_rProfileData.bUseTest_OpenCellMode );		// 2015-03-12, jhLee, 김상묵 사원요청, Open Cell mode인지 여부, 기본은 FALSE(Normal Cell)

		// N/A
		pCellLog->AddItem( _T("") );
		// N/A
		pCellLog->AddItem( _T("") );



		// sHeader += _T(",TACT LOAD UNLOAD,TACT A ZONE INSP,TACT A ZONE MCR ALIGN,TACT A ZONE,TACT WAITING1,TACT TABLE TURN1,TACT C ZONE VISION 1CH,TACT C ZONE CAMERA MOVE,TACT C ZONE VISION 2CH,TACT C ZONE,TACT WAITING2,TACT TABLE TURN2,TACT D ZONE,TACT TIME,TOTAL TACT,N/A,N/A");


		// 1. TACT LOAD UNLOAD 	// 이전Cell 제거 및 신규 Cell 투입까지의 시간 : PG OFF ~ Power On 전송
		pCellLog->AddItem_Double(  pCell->dbMoveTACT[eTACT_TurnOn], 3 );

		// 2. TACT A ZONE INSP, A-Zone 목시 검사 후 양수 버튼까지 시간
		pCellLog->AddItem_Double(  pCell->dbMoveTACT[eTACT_ReadyBtn], 3 );

		// 3. TACT A ZONE MCR ALIGN, MCR 읽기와 ALign 동작시간, 양수 버튼 ~ Align 완료
		pCellLog->AddItem_Double(  pCell->dbMoveTACT[eTACT_MCRAlignTime], 3 );

		// 4. TACT A ZONE, A-Zone에서의 작업 시간, TURN ON 부터 Cell Loading 수신 혹은 Align 종료 시 까지
		pCellLog->AddItem_Double(  pCell->dbMoveTACT[eTACT_AZoneTime], 3 );


		// 5. TACT WAITING1 	// C-Zone으로의 회전 이동 시각, 회전 대기 완료
		pCellLog->AddItem_Double(  pCell->dbMoveTACT[eTACT_TurnMoveStart], 3 );
		
		// 6. TACT TABLE TURN1, // C-Zone으로의 회전 이동 완료
		pCellLog->AddItem_Double(  pCell->dbMoveTACT[eTACT_TurnMoveEnd], 3 );


		// 7. TACT C ZONE VISION 1CH	// 비전검사 채널#1 Grab 완료, CH#2 이동 시작
		pCellLog->AddItem_Double(  pCell->dbMoveTACT[eTACT_VI_CH1_GrabEnd], 3 );

		// 8. TACT C ZONE CAMERA MOVE // 비전검사 채널#2 이동 및 Align 동작 완료, Vision 2Ch Start
		pCellLog->AddItem_Double(  pCell->dbMoveTACT[eTACT_VI_CH2_AlignMove], 3 );

		// 9. TACT C ZONE VISION 2CH // 비전검사 채널#2 Grab 완료, Return Move 대기 시작
		pCellLog->AddItem_Double(  pCell->dbMoveTACT[eTACT_VI_CH2_GrabEnd], 3 );

		// 10. TACT C ZONE,  // C Zone TACT Time : 1CH Grab.Start 전송 ~ 2CH GRAB.END.OK 수신까지
		pCellLog->AddItem_Double(  pCell->dbMoveTACT[eTACT_CZoneTime], 3 );


		// 11. TTACT WAITING2  , A-Zone으로의 회전 대기시간
		pCellLog->AddItem_Double(  pCell->dbMoveTACT[eTACT_ReturnMoveStart], 3 );

		// 12. TACT TABLE TURN2, A-Zone으로의 회전 이동 시간
		pCellLog->AddItem_Double(  pCell->dbMoveTACT[eTACT_ReturnMoveEnd], 3 );


		// 13. TACT D ZONE // 작업자 검사 완료로 D-Zone 작업 종료 시간 , C->A 회전 후 ~ PG OFF 까지
		pCellLog->AddItem_Double(  pCell->dbMoveTACT[eTACT_DZoneTIme], 3 );
	

		// 14. TACT TIME,  Cell을 처리하는데 소요되는 시간, 신호기 Power ON ~ 신호기 Power Off
		pCellLog->AddItem_Double( pCell->dbMoveTACT[eTACT_TACTTime], 3 );

		// 15. TOTAL TACT
		pCellLog->AddItem_Double( pCell->dbMoveTACT[eTACT_TotalTACTTime], 3 );

		// N/A 2개
		pCellLog->AddItem( _T(""));
		pCellLog->AddItem( _T(""));

		// POC INFO : POCH 대상인가 ? 1:대상, 공백:미대상
		pCellLog->AddItem( pCell->POCInfo );


		// 2016-11-20, 추가된 항목
		// DEEP CHECK Cell Information Result 42의 결과로 집중검사를 수행하였나 ?
		pCellLog->AddItem( pCell->bDeepCheck ? _T("Y") : _T("N") );
			
		// DEFECT POS, 작업자가 선택한 Defect 위치, 최대 문자열길이는 39자까지이다 (13개 포인트 지정가능)
		CString sTemp;
		sTemp.Format(_T("\"%s\""), pCell->sDefectPos.Left(39));			// 복수개의 좌표는 콤마로 분리되어 함께 존재하므로 이중따옴표로 하나의 문장으로 만들어준다.
		pCellLog->AddItem( sTemp );



		// 누적한 데이터들을 파일에 기록한다.
		pCellLog->WriteItem();
	}

	
	return TRUE;
}





// 비가동 log 기록용 파일 경로를 설정한다.
// void* pValue -> CString pointer로 경로명을 되돌려준다.
void CMCMain::SetLosstimeLogPath(void* pValue)
{
	CString* pFilename = (CString *)pValue;			// CString 형으로 변환하여 사용한ㄷ.

	CTime time;										// '오늘'의 기준 날짜를 얻는다.

	if ( *pFilename == _T("PREV") )					// 전날의 날짜로 강제 맞추라고 하였다면,
	{
		time = CTime::GetCurrentTime();				// 현재 시간

		CTimeSpan timeOffset(1, 0, 0, 0);			// 강제로 1일을 빼준다.
		time -= timeOffset; 
	}
	else
		time = GetToday();							// 시간대에 따른 날짜 지정
		

	CString sFullPath;
	//OLD sFullPath.Format(_T("%s\\%04d%02d%02d\\%04d%02d%02d_LOSSTIME_%s.csv"), NAME_DIR_LOSSTIME, time.GetYear(), time.GetMonth(), time.GetDay(),
	sFullPath.Format(_T("%s\\%04d%02d%02d\\%04d%02d%02d_LOSSTIME_%s.csv"), m_rProfileData.sDIR_LossTime, time.GetYear(), time.GetMonth(), time.GetDay(),
																time.GetYear(), time.GetMonth(), time.GetDay(), m_rProfileData.sCellLog_LineID );	// MINS에서 사용되는 라인내 설비 ID, 3자리

	*pFilename = sFullPath;							// 만들어진 경로명을 지정
}




// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==


// Alarm Data를 파일로 기록한다.
void CMCMain::WriteAlarmData()
{
	//CString sMsg;
	//sMsg.Format(_T("%s,%s,%s,%s,%s,%d"), CGxUtility::SystemTimeToString(m_rAlarmRecord.tmStartTime), CGxUtility::SystemTimeToString(m_rAlarmRecord.tmResetTime), 
	//		m_rAlarmRecord.sParts, m_rAlarmRecord.sCode, m_rAlarmRecord.sName, m_rAlarmRecord.nLevel);			// 내용 조립
	//m_dlogAlarm.Write( sMsg );			// file에 기록한다.

	m_logAlarm.AddItem_DateTime( m_rAlarmRecord.tmStartTime, TRUE );
	m_logAlarm.AddItem_DateTime( m_rAlarmRecord.tmResetTime, TRUE );
	m_logAlarm.AddItem_Double( CGxUtility::DiffSeconds(m_rAlarmRecord.tmStartTime, m_rAlarmRecord.tmResetTime), 3 );
	m_logAlarm.AddItem( m_rAlarmRecord.sCode );
	m_logAlarm.AddItem( m_rAlarmRecord.sName );
	m_logAlarm.AddItem( m_rAlarmRecord.sDescript );
	
	m_logAlarm.WriteItem();

}


// 지정일의 Alarm Log 파일 이름을 조회한다.
CString CMCMain::GetAlarmLogFileName(CTime tmDay)
{
	return m_logAlarm.GetFilePath( tmDay );
}



// 지정 JIG의 TACT Time을 파일로 기록한다.
void CMCMain::WriteTACTTimeData(int nJig)
{
	if ( (nJig < 0) || (nJig >= eMAX_JigCount) ) return;

	int i;
	TCellData* pCell;

	// 2014-10-16, jhLee, New
	for (i = 0; i < eMAX_ChCount; i++) 
	{
		pCell = m_pCellInfo->GetCellData(nJig, i);				// 지정 Jig의 각 채널의 포인트를 얻어온다.

		if ( !pCell ) return;
		if ( !pCell->bIsExist ) continue;

			m_logTACTTime.AddItem_DateTime( 	pCell->tmMoveTACT[eTACT_LoadPGOff], TRUE );		// 새로운 제품이 들어온 시각
			m_logTACTTime.AddItem_DateTime( 	pCell->tmMoveTACT[eTACT_UnloadPGOff], TRUE );	// 제품 검사를 마친 시각
			m_logTACTTime.AddItem_Int( pCell->ChNo + 1 );												// 채널 번호
			m_logTACTTime.AddItem( pCell->CellID );														// cell id
			m_logTACTTime.AddItem_Double(pCell->dbMoveTACT[eTACT_TotalTACTTime],	2);			// 1 cycle tact time
			m_logTACTTime.AddItem_Double(pCell->dbMoveTACT[eTACT_EQTime], 2);						// 설비 시간
			m_logTACTTime.AddItem_Double(pCell->dbMoveTACT[eTACT_OPTime], 2);						// 작업자 시간
			m_logTACTTime.AddItem_Double(pCell->dbMoveTACT[eTACT_JigTACTTime], 2);				// JIG단위 TACT Time
			m_logTACTTime.AddItem_Int( pCell->JigNo + 1 );												// JIG 번호
			m_logTACTTime.AddItem(GetUserName( GetCurrentUserID(), eLANG_KOREAN ));				// 작업자 이름

			m_logTACTTime.AddItem_Double(pCell->dbMoveTACT[eTACT_PGOn],	 2);						// Contact 시간
			m_logTACTTime.AddItem_Double(pCell->dbMoveTACT[eTACT_ReadyBtn], 2);					// 양수버튼	Loading 및 A-Zone 검사 완료
			m_logTACTTime.AddItem_Double(pCell->dbMoveTACT[eTACT_TiltUpEnd], 2);					// JIG Tilt Up End	지그 Tilt Up 완료, UV-MCR 읽기 시작
			m_logTACTTime.AddItem_Double(pCell->dbMoveTACT[eTACT_UVMCRRead],	 2);				// UV-MCR 리딩	UV-MCR 읽기 완료, Align Right Move Start
			m_logTACTTime.AddItem_Double(pCell->dbMoveTACT[eTACT_AlignRightMove], 2);			// Align Right 이동	오른쪽 Align 마크 찾기 위치로 이동 완료
			m_logTACTTime.AddItem_Double(pCell->dbMoveTACT[eTACT_AlignRightMark], 2);			// Align Right 마크 찾기 완료, 왼쪽 마크 위치로 이동 시작
			m_logTACTTime.AddItem_Double(pCell->dbMoveTACT[eTACT_AlignLeftMove],	 2);			// 왼쪽 Align 마크 찾기 위치로 이동
			m_logTACTTime.AddItem_Double(pCell->dbMoveTACT[eTACT_AlignLeftMark],	 2);			// 왼쪽 Align 마크 찾기 및 Align 계산 완료, Turn 대기 시작
			m_logTACTTime.AddItem_Double(pCell->dbMoveTACT[eTACT_PGCellLoading],	 2);			// 신호기로부터 Cell Loading 완료 신호를 받는 대기 시간,	#2015-05-26, jhLee 추가
			m_logTACTTime.AddItem_Double(pCell->dbMoveTACT[eTACT_TurnMoveStart],	 2);			// C-Zone으로의 회전 이동 시각, 회전 대기 완료

			m_logTACTTime.AddItem_Double(pCell->dbMoveTACT[eTACT_TurnMoveEnd], 2);				// C-Zone으로의 회전 이동 완료
			m_logTACTTime.AddItem_Double(pCell->dbMoveTACT[eTACT_VI_CH1_AlignMove], 2);		// 비전검사 채널#1 Align 동작
			m_logTACTTime.AddItem_Double(pCell->dbMoveTACT[eTACT_VI_CH1_GrabEnd],	 2);		// 비전검사 채널#1 Grab 완료, CH#2 이동 시작
			m_logTACTTime.AddItem_Double(pCell->dbMoveTACT[eTACT_VI_CH2_AlignMove],	 2);		// 비전검사 채널#2 이동 및 Align 동작 완료
			m_logTACTTime.AddItem_Double(pCell->dbMoveTACT[eTACT_VI_CH2_GrabEnd],	 2);		// 비전검사 채널#2 Grab 완료, Return Move 대기 시작
			m_logTACTTime.AddItem_Double(pCell->dbMoveTACT[eTACT_ReturnMoveStart], 2);			// A-Zone으로의 회전 이동 시각, 회전 대기 완료
			m_logTACTTime.AddItem_Double(pCell->dbMoveTACT[eTACT_ReturnMoveEnd],	 2);			// A-Zone으로의 회전 이동 완료

			m_logTACTTime.AddItem_Double(pCell->dbMoveTACT[eTACT_TiltDownEnd], 2);				// 지그 Tilt down, 작업자 검사 시작
			m_logTACTTime.AddItem_Double(pCell->dbMoveTACT[eTACT_UnloadPGOff] , 2);				// 작업자 검사 완료

			// 데이터 기록
			m_logTACTTime.WriteItem();
	}//of for i

}


// 이름 변경,
//
//// 생산율 정보를 파일로 기록한다.
//void CMCMain::WriteProdcutData(CTime time)
//{
//	CString sMsg;
//			
//	CTimeSpan tmOffset(0,1,0,0);
//	time -= tmOffset;	// 취득한 시간에서 -1 시간을 한다 (실제 기록할 생산율 정보)
//	
//
//	// 데이타를 가져올 경로를 설정
//	CString strFolderPath;
//	strFolderPath.Format(_T("D:\\UB-AMT\\Log\\%04d%02d\\Product"),time.GetYear(),time.GetMonth());	// 폴더경로;
//
//	if(CFileSupport::DirectoryCheck(strFolderPath) != TRUE)
//	{
//		return;
//	}
//	CString strPath;
//	strPath.Format(_T("%s\\%s_%04d%02d%02d.ini"), strFolderPath, _T("ProductDB"), time.GetYear(), time.GetMonth(), time.GetDay());
//
//	CString strSection = _T("");
//	CGxIniFile ini;
//	ini.SetIniFilename(strPath);	// ini지정;
//	strSection.Format(_T("%02d"), time.GetHour());
//
//
//	// 데이타 지정;
//	// 시간
//	int nHour = time.GetHour();						
//
//	// 생산량
//	int nTotal = ini.GetInt(strSection, _T("TOTAL"), 0);	
//
//	// Contact수량
//	int nContact = ini.GetInt(strSection, _T("CONTACT"),0);	
//
//	// Contact OK 수량
//	int nContactOK = ini.GetInt(strSection, _T("CH1_GOOD"),0) + ini.GetInt(strSection, _T("CH2_GOOD"),0) + ini.GetInt(strSection, _T("CH3_GOOD"),0) + ini.GetInt(strSection, _T("CH4_GOOD"),0);
//
//	// Contact NG 수량
//	int nContactNG = ini.GetInt(strSection, _T("CH1_NG"),0) + ini.GetInt(strSection, _T("CH2_NG"),0) + ini.GetInt(strSection, _T("CH3_NG"),0) + ini.GetInt(strSection, _T("CH4_NG"),0);
//
//	// Contact (%)
//	double dContactRatio = 0;
//	if(nContact == 0)
//	{
//		dContactRatio =0;
//	}
//	else
//	{
//		dContactRatio = ( (double)(nContactOK) / (double)(nContact) ) * 100;
//	}
//	
//	// Contact CH1 (%)
//	int nContact1_OK = ini.GetInt(strSection, _T("CH1_GOOD"),0);
//	int nContact1_NG = ini.GetInt(strSection, _T("CH1_NG"),0);
//
//	double dContactCH1_Ratio = 0;
//	if(nContact1_OK + nContact1_NG == 0)
//	{
//		dContactCH1_Ratio =0;
//	}
//	else
//	{
//		dContactCH1_Ratio = ( (double)(nContact1_OK) / (double)(nContact1_OK + nContact1_NG) ) * 100;
//	}
//
//
//	// Contact CH2 (%)
//	int nContact2_OK = ini.GetInt(strSection, _T("CH2_GOOD"),0);
//	int nContact2_NG = ini.GetInt(strSection, _T("CH2_NG"),0);
//
//	double dContactCH2_Ratio = 0;
//	if(nContact2_OK + nContact2_NG == 0)
//	{
//		dContactCH2_Ratio =0;
//	}
//	else
//	{
//		dContactCH2_Ratio = ( (double)(nContact2_OK) / (double)(nContact2_OK + nContact2_NG) ) * 100;
//	}
//
//	// Contact CH3 (%)
//	int nContact3_OK = ini.GetInt(strSection, _T("CH3_GOOD"),0);
//	int nContact3_NG = ini.GetInt(strSection, _T("CH3_NG"),0);
//
//	double dContactCH3_Ratio = 0;
//	if(nContact3_OK + nContact3_NG == 0)
//	{
//		dContactCH3_Ratio =0;
//	}
//	else
//	{
//		dContactCH3_Ratio = ( (double)(nContact3_OK) / (double)(nContact3_OK + nContact3_NG) ) * 100;
//	}
//
//	// Contact CH4 (%)
//	int nContact4_OK = ini.GetInt(strSection, _T("CH4_GOOD"),0);
//	int nContact4_NG = ini.GetInt(strSection, _T("CH4_NG"),0);
//
//	double dContactCH4_Ratio = 0;
//	if(nContact4_OK + nContact4_NG == 0)
//	{
//		dContactCH4_Ratio =0;
//	}
//	else
//	{
//		dContactCH4_Ratio = ( (double)(nContact4_OK) / (double)(nContact4_OK + nContact4_NG) ) * 100;
//	}
//	
//	int nNoDisplay	=	ini.GetInt(strSection, _T("NO_DISP"),0);
//
//
//
//	
//	sMsg.Format(_T("%s,%d,%d,%d,%d,%d,%.1f,%d,%.1f,%.1f,%.1f,%.1f")	,	m_rProfileData.sEQID
//																	,	nHour
//																	,	nTotal
//																	,	nContact
//																	,	nContactOK
//																	,	nContactNG
//																	,	dContactRatio
//																	,	nNoDisplay
//																	,	dContactCH1_Ratio
//																	,	dContactCH2_Ratio
//																	,	dContactCH3_Ratio
//																	,	dContactCH4_Ratio);
//
//
////		m_dlogProductData.Write( sMsg );			// file에 기록한다.
//		m_dlogProductData.Write_ProductData( sMsg );			// file에 기록한다.
//}
//


// 생산율 정보를 파일로 기록한다.
void CMCMain::WriteContactData()
{
	CString sMsg;
			

	CTime tmToday = m_logProductData.GetToday( 0 );			// 오늘자 기준 시각

	CTimeSpan tmOffset(0,1,0,0);
	tmToday -= tmOffset;									// 취득한 시간에서 -1 시간을 한다 (실제 기록할 생산율 정보)
	

	// 데이타를 가져올 경로를 설정
	CString sFilePath = m_logProductData.GetFilePath( tmToday );			// 생산 정보 기록 파일 이름 취득용, m_iniProduct ini file이 원본
	BOOL bFolerCheck = m_logProductData.CheckFolder( sFilePath, TRUE );		// 지정한 경로의 폴더가 존재하는지 점검한다.


	CString strSection = _T("");
	CString sKey;

	CGxIniFile ini;
	ini.SetIniFilename(sFilePath);	// ini지정;
	strSection.Format(_T("%02d"), tmToday.GetHour());

	int nCount = 0;
	int nCntGood = 0;
	int nCntNG = 0;

	int nValue = 0;
	double dbValue = 0.0;


	// 데이타 지정;
	m_logContactData.AddItem( m_rProfileData.sEQID );

	m_logContactData.AddItem_Int( tmToday.GetHour() );												// 시간
	m_logContactData.AddItem_Int( ini.GetInt(strSection, _T("TOTAL"), 0) );						// 총 생산량

	nCount = ini.GetInt(strSection, _T("CONTACT"),0);
	m_logContactData.AddItem_Int( nCount  );													// Contact 수량


	// Contact OK 수량
	nCntGood = ini.GetInt(strSection, _T("CH1_GOOD"),0) + ini.GetInt(strSection, _T("CH2_GOOD"),0) + ini.GetInt(strSection, _T("CH3_GOOD"),0) + ini.GetInt(strSection, _T("CH4_GOOD"),0);
	m_logContactData.AddItem_Int( nValue );
	
	// Contact NG 수량
	nCntNG = ini.GetInt(strSection, _T("CH1_NG"),0) + ini.GetInt(strSection, _T("CH2_NG"),0) + ini.GetInt(strSection, _T("CH3_NG"),0) + ini.GetInt(strSection, _T("CH4_NG"),0);
	m_logContactData.AddItem_Int( nValue );

	// Contact 성공률 (%)
	dbValue = (nCount > 0) ? (nCntGood / (nCount * 1.0) ) * 100 : 0.0;			// COntact 성공률을 구한다.
	m_logContactData.AddItem_Double( dbValue, 1 );

	// No Display 발생 횟수
	m_logContactData.AddItem_Int( ini.GetInt(strSection, _T("NO_DISP"),0) );


	// Channel 1 Contact 성공률

	for (int i = 0; i < eMAX_JIG_CH_Count; i++)
	{
		sKey.Format(_T("CH%d_GOOD"), i+1);
		nCntGood	= ini.GetInt(strSection, sKey, 0);

		sKey.Format(_T("CH%d_NG"), i+1);
		nCntNG	= ini.GetInt(strSection, sKey, 0);

		nCount		= nCntGood + nCntNG;
		dbValue		= (nCount > 0) ? (nCntGood / (nCount * 1.0) ) * 100 : 0.0;			// COntact 성공률을 구한다.

		m_logContactData.AddItem_Double( dbValue, 1 );		// CH1 ~ CH4 까지의 Contact 성공률을 기록한다.
	}


	m_logContactData.WriteItem();							// 파일에 기록을 한다.		
}




// 생산량 파일을 시간 기준으로 기록해준다.;
void CMCMain::SaveTodayProductData(int nIdx, int nCnt /*= 0*/)
{
	CTime secTime = CTime::GetCurrentTime(); // Section 지정용 현재 시각
	
	// 하루 나누는 시간으로 날짜를 갱신한다.
	// 기준시간이 06시 일경우 
	// 15일 05시까지는 time이 14일자로 고정된다.
	// 15일 05시부터 tiem이 15일로 변경된다.

	CString sFilePath = m_logProductData.GetFilePath( GetToday() );					// 생산 정보 기록 파일 이름 취득용, m_iniProduct ini file이 원본
	

	// 새로운 파일이라면, 변수 초기화
	if(CGxUtility::IsExistFile( sFilePath ) == FALSE)	
	{
		// 새로운 파일을 만들어야한다면.. (=즉 다음날로 변경될경우) 변수들 전부 초기화;
		m_rProductData.ClearTodayProductData();
	}

	CString strSection = _T("");
	CGxIniFile ini;

	int nCurHour = secTime.GetHour();
	ini.SetIniFilename( sFilePath );	// ini지정;
	strSection.Format(_T("%02d"), nCurHour); // Section 지정;


	switch(nIdx)
	{
	case eIDX_Product_TodayRunningCell:
			ini.WriteInt(strSection, _T("TOTAL")		, ++m_rProductData.nTodayRunningCell[nCurHour]);
		break;

	case eDIX_Prodcut_TodayAutoGood:
		ini.WriteInt(strSection, _T("AUTO_GOOD")		, ++m_rProductData.nTodayAutoGoodCount[nCurHour]);
		break;

	case eDIX_Prodcut_TodayAutoBin2:
		ini.WriteInt(strSection, _T("AUTO_BIN2")		, ++m_rProductData.nTodayAutoBin2Count[nCurHour]);
		break;

	case eDIX_Prodcut_TodayAutoReject:
		ini.WriteInt(strSection, _T("AUTO_REJECT")	, ++m_rProductData.nTodayAutoRejectCount[nCurHour]);
		break;

	case eDIX_Prodcut_TodayManGood:
		ini.WriteInt(strSection, _T("MANUAL_GOOD")	, ++m_rProductData.nTodayManGoodCount[nCurHour]);
		break;

	case eDIX_Prodcut_TodayManBin2:
		ini.WriteInt(strSection, _T("MANUAL_BIN2")	, ++m_rProductData.nTodayManBin2Count[nCurHour]);
		break;

	case eDIX_Prodcut_TodayManReject:
		ini.WriteInt(strSection, _T("MANUAL_REJECT")	, ++m_rProductData.nTodayManRejectCount[nCurHour]);
		break;

	case eIDX_Product_TodayMcrRead:
		ini.WriteDouble(strSection, _T("MCR_REAIDNG")	, ++m_rProductData.dTodayMcrReadCount[nCurHour]);
		break;

	case eIDX_Product_TodayMcrNg:
		ini.WriteDouble(strSection, _T("MCR_NG")			, ++m_rProductData.dTodayMcrNgCount[nCurHour]);
		break;

	case eIDX_Product_TodayAlignRead:
		ini.WriteDouble(strSection, _T("ALIGN_READING")	, ++m_rProductData.dTodayAlignReadCount[nCurHour]);
		break;

	case eIDX_Product_TodayAlignNg:
		ini.WriteDouble(strSection, _T("ALIGN_NG")		, ++m_rProductData.dTodayAlignNgCount[nCurHour]);
		break;

	case eIDX_Product_TodayRunTime:
		ini.WriteInt(strSection, _T("RUN_TIME")		, ++m_rProductData.nTodayRunTime[nCurHour]);
		break;

	case eIDX_Product_TodayOPLoss:
		m_rProductData.nTodayOPLoss[nCurHour] += nCnt; // Count 개념이 아니라 특정 값이 날라와 별도의 계산식이 필요하다
		ini.WriteInt(strSection, _T("OP_LOSS")		, m_rProductData.nTodayOPLoss[nCurHour]);
		break;

	case eIDX_Product_TodayContact:
		m_rProductData.nTodayContact[nCurHour] += nCnt; // Count 개념이 아니라 특정 값이 날라와 별도의 계산식이 필요하다
		ini.WriteInt(strSection, _T("CONTACT")		, m_rProductData.nTodayContact[nCurHour]);
		break;
		
	case eIDX_Product_TodayCH1_ContactGOOD:
		m_rProductData.nCH1_GoodCount[nCurHour] += nCnt;
		ini.WriteInt(strSection, _T("CH1_GOOD")		, m_rProductData.nCH1_GoodCount[nCurHour]);
		break;

	case eIDX_Product_TodayCH2_ContactGOOD:
		m_rProductData.nCH2_GoodCount[nCurHour] += nCnt;
		ini.WriteInt(strSection, _T("CH2_GOOD")		, m_rProductData.nCH2_GoodCount[nCurHour]);
		break;

	case eIDX_Product_TodayCH3_ContactGOOD:
		m_rProductData.nCH3_GoodCount[nCurHour] += nCnt;
		ini.WriteInt(strSection, _T("CH3_GOOD")		, m_rProductData.nCH3_GoodCount[nCurHour]);
		break;

	case eIDX_Product_TodayCH4_ContactGOOD:
		m_rProductData.nCH4_GoodCount[nCurHour] += nCnt;
		ini.WriteInt(strSection, _T("CH4_GOOD")		, m_rProductData.nCH4_GoodCount[nCurHour]);
		break;

	case eIDX_Product_TodayCH1_ContactNG:
		m_rProductData.nCH1_NgCount[nCurHour] += nCnt;
		ini.WriteInt(strSection, _T("CH1_NG")		, m_rProductData.nCH1_NgCount[nCurHour]);
		break;

	case eIDX_Product_TodayCH2_ContactNG:
		m_rProductData.nCH2_NgCount[nCurHour] += nCnt;
		ini.WriteInt(strSection, _T("CH2_NG")		, m_rProductData.nCH2_NgCount[nCurHour]);
		break;

	case eIDX_Product_TodayCH3_ContactNG:
		m_rProductData.nCH3_NgCount[nCurHour] += nCnt;
		ini.WriteInt(strSection, _T("CH3_NG")		, m_rProductData.nCH3_NgCount[nCurHour]);
		break;

	case eIDX_Product_TodayCH4_ContactNG:
		m_rProductData.nCH4_NgCount[nCurHour] += nCnt;
		ini.WriteInt(strSection, _T("CH4_NG")		, m_rProductData.nCH4_NgCount[nCurHour]);
		break;

	case eIDX_Product_TodayNoDisplay:
		ini.WriteInt(strSection, _T("NO_DISP")		, ++m_rProductData.nNoDisplay[nCurHour]);
		break;


	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 비가동 로그 관련
//
// 비가동 정보를 임시 기억하기위한 초기화
void CMCMain::InitLosstimeInfo()
{
	// ini file 설정 
	m_iniLosstime.m_sIniFile.Format(_T("%s\\INI\\%s"), m_sBasePath, NAME_FILE_LOSSTIME_DATA);	// data 파일 경로 지정

	m_iniLosstime.AddStr(_T("LastInfo:sReasonLine"),	m_rLosstimeRecord.sReasonLine);				// 선택된 전체 Code 내용
	m_iniLosstime.AddStr(_T("LastInfo:sUserID"),		m_rLosstimeRecord.sID);							// Stop 시킨 작업자 ID
	m_iniLosstime.AddStr(_T("LastInfo:sUserIDNo"),		m_rLosstimeRecord.sIDNo);						// 작업자 사원번호;
	m_iniLosstime.AddStr(_T("LastInfo:sAlarmType"),		m_rLosstimeRecord.sAlarmType);				// Alarm 여부, 중/경 알람여부
	m_iniLosstime.AddStr(_T("LastInfo:sAlarmCode"),		m_rLosstimeRecord.sAlarmCode);				// Alarm Code
	m_iniLosstime.AddStr(_T("LastInfo:sUserMsg"),		m_rLosstimeRecord.sUserMsg);					// 사용자 입력 내용

	m_iniLosstime.AddStr(_T("LastInfo:sStartTime"),		m_rLosstimeRecord.sStartTime);		// 정지 시작 시각
	m_iniLosstime.AddStr(_T("LastInfo:sResetTime"),		m_rLosstimeRecord.sResetTime);		// 인지 시각

	m_iniLosstime.AddStr(_T("LastInfo:sTPSystemName"),	m_rLosstimeRecord.sTPSystemName);		// TP System loss name
	m_iniLosstime.AddStr(_T("LastInfo:sTPSystemCode"),	m_rLosstimeRecord.sTPSystemCode);		// TP System loss code

}

// 파일에 저장된 비가동 정보를 읽어온다. (Program 실행시)
void CMCMain::LoadLosstimeInfo()
{
	m_iniLosstime.Load();

	// 문자열 -> 날짜형으로 변형시켜준다.
	CGxUtility::StringToSystemTime( m_rLosstimeRecord.sStartTime,	m_rLosstimeRecord.tmStartTime );
	CGxUtility::StringToSystemTime( m_rLosstimeRecord.sResetTime,	m_rLosstimeRecord.tmResetTime );

	// 각 항목에 맞도록 분류해서 치환한다.
	if ( m_rLosstimeRecord.sReasonLine != _T("") )
	{
		::AfxExtractSubString( m_rLosstimeRecord.s1stClass,	m_rLosstimeRecord.sReasonLine, 0, DELIMITER_DEFECT);			// 1st class
		::AfxExtractSubString( m_rLosstimeRecord.sMESCode,	m_rLosstimeRecord.sReasonLine, 1, DELIMITER_DEFECT);			// MESCode
		::AfxExtractSubString( m_rLosstimeRecord.s2ndClass,	m_rLosstimeRecord.sReasonLine, 2, DELIMITER_DEFECT);			// 2nd class
		::AfxExtractSubString( m_rLosstimeRecord.s3rdClass,	m_rLosstimeRecord.sReasonLine, 3, DELIMITER_DEFECT);			// 3rd class
	}

}


// 현재 설정된 비가동 정보를 파일로 기록한다.
void CMCMain::SaveLosstimeInfo()
{
	// ini file로 저장하기 위한 변수
	m_rLosstimeRecord.sStartTime = CGxUtility::SystemTimeToString( m_rLosstimeRecord.tmStartTime );						// 문자열로 변경된 시작 시간
	m_rLosstimeRecord.sResetTime = CGxUtility::SystemTimeToString( m_rLosstimeRecord.tmResetTime );						// 문자열로 변경된 인지 시간

	m_iniLosstime.Save();
}

// 비가동 관련 내용이 없다면 내용을 청소해준다.
void CMCMain::ClearLosstimeInfo()
{
	//// ini file 설정 
	//m_rLosstimeRecord.sReasonLine		= _T("");					// 선택된 전체 Code 내용
	//m_rLosstimeRecord.sUserMsg			= _T("");
	//m_rLosstimeRecord.dbAddLossTime	= 0.0;

	//::GetLocalTime( &(m_rLosstimeRecord.tmStartTime) );		// 처리 종료하고 다시 시작 시간
	//::GetLocalTime( &(m_rLosstimeRecord.tmResetTime) );		// 처리 종료하고 다시 시작 시간
	//::GetLocalTime( &(m_rLosstimeRecord.tmEndTime) );			// 처리 종료하고 다시 시작 시간

	////2014-09-11, jhLee, 일단 현재는 정의된 내용이 없다.
	//m_rLosstimeRecord.sTPSystemName	= _T("UNDEFINDED");					// TP System loss name
	//m_rLosstimeRecord.sTPSystemCode	= _T("UNDEFINDED");					// TP System Loss Code

	m_rLosstimeRecord.Clear();										// 내용을 CLear한 후 저장한다.

	SaveLosstimeInfo();
}

//
// AUTO RUN DOWN 처리에 대한 루틴
//
//	int					m_nRunDownTimer;		// RunDown으로 전환을 위한 카운터
// 현재의 RunDown 경과 Timer를 조회한다.
int CMCMain::GetRunDownTimer()
{
	return m_nRunDownTimer;
}


// RunDown 타이머를 초기화 해준다.
void CMCMain::ClearRunDownTimer()
{
	m_nRunDownTimer = 0;
}

// RunDown을 실행할 것인지 점검해준다.
BOOL CMCMain::CheckRunDownTimer()
{
	m_nRunDownTimer++;							// 시간을 증가시킨다.

	return (m_nRunDownTimer > m_rProfileData.nTIME_RunDownTime);			// 지정된 시간을 넘어섰다면, TRUE
}

// RunDown을 실행한다.
void CMCMain::ApplyRunDown()
{
	ClearRunDownTimer();					// 타이머를 초기화 해주고,

	::PostMessage( m_hOwner, UDMSG_APPLY_RUNDOWN, (WPARAM)0, (LPARAM)0);			// Run Down을 실행한다.
}
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 


// 지정한 비가동 레코드 데이터를 파일로 기록한다.
void CMCMain::WriteLossTimeRecord(TLosstimeRecord& rRec, BOOL bPrevDay)
{
	CString sData;

	// Alarm에 의해 중지가 발생 한 경우 
	if ( (rRec.sAlarmType == _T("A")) && (rRec.s1stClass == _T("MD")) )
	{
		// 60초 이상이면 BM으로 전환한다.
		float fFlag = CGxUtility::DiffSeconds(rRec.tmStartTime, rRec.tmEndTime);		// Duration time

		//2014-11-19, jhLee, 김길성책임 요청, MD/BM 자동 전환시 지정된 시간을 사용한다.
		//OLDif ( fFlag >= 60.0 ) // 지정한 60초를 넘겼다면,
		if ( fFlag >= (float)(m_rProfileData.nTIME_MDChangeTime) ) // 지정한 시간을 넘겼다면,
		{
			rRec.s1stClass = _T("BM");			// MD -> BM으로 자동 전환된다.
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Data 기록

	// EQID;
	m_logLosstime.AddItem( m_rProfileData.sEQID );
	m_logLosstime.AddItem( m_rProfileData.sPPID );
	m_logLosstime.AddItem( GetCurrentUserIDNo() );				// 현재 작업자의 사원번호
	m_logLosstime.AddItem( rRec.sAlarmType );						// Alarm여부


	// 지정한 시간 보정값이 존재한다면,
	if ( rRec.dbAddLossTime > 0.0 )
	{
		//보류 CGxUtility::PrevSecondTime(rRec.dbAddLossTime, rRec.tmStartTime);		// 지정한 시간만큼 시작 시간을 당겨준다.
		m_logLosstime.AddItem_DateTime( rRec.tmStartTime , TRUE );		// 시작 시간
		m_logLosstime.AddItem_DateTime( rRec.tmResetTime, TRUE );		// 조치 시간
		m_logLosstime.AddItem_DateTime( rRec.tmEndTime, TRUE );		// 종료 시각
		m_logLosstime.AddItem_Double( CGxUtility::DiffSeconds(rRec.tmStartTime, rRec.tmResetTime), 1);			// Wait time
		m_logLosstime.AddItem_Double( CGxUtility::DiffSeconds(rRec.tmResetTime, rRec.tmEndTime) + rRec.dbAddLossTime , 1);	// Repair time
		m_logLosstime.AddItem_Double( CGxUtility::DiffSeconds(rRec.tmStartTime, rRec.tmEndTime) + rRec.dbAddLossTime , 1);	// Duration time
	}
	else
	{
		m_logLosstime.AddItem_DateTime( rRec.tmStartTime , TRUE );		// 시작 시간
		m_logLosstime.AddItem_DateTime( rRec.tmResetTime , TRUE );		// 조치 시간
		m_logLosstime.AddItem_DateTime( rRec.tmEndTime, TRUE );		// 종료 시각
		m_logLosstime.AddItem_Double( CGxUtility::DiffSeconds(rRec.tmStartTime, rRec.tmResetTime), 1 );	// Wait time
		m_logLosstime.AddItem_Double( CGxUtility::DiffSeconds(rRec.tmResetTime, rRec.tmEndTime), 1 );		// Repair time
		m_logLosstime.AddItem_Double( CGxUtility::DiffSeconds(rRec.tmStartTime, rRec.tmEndTime), 1 );		// Duration time
	}

	m_logLosstime.AddItem( rRec.s1stClass);
	m_logLosstime.AddItem( rRec.s2ndClass);

	// 최종 사유가 사용자 입력이고, 사용자가 입력한 내용이 있으면 해당 내용으로 기록한다.
	sData = rRec.s3rdClass;

	if ( (rRec.s3rdClass == _T("입력")) || (rRec.s3rdClass == ("Input") ) )
	{
		if ( rRec.sUserMsg != _T("") ) 
		{
			sData.Format(_T("%s")	, rRec.sUserMsg);
		}
	}
	m_logLosstime.AddItem( sData );

	m_logLosstime.AddItem( rRec.sTPSystemName);
	m_logLosstime.AddItem( rRec.sTPSystemCode);
	m_logLosstime.AddItem( rRec.sAlarmCode);
	m_logLosstime.AddItem( rRec.sRepairCode);


	// 만약 강제로 어제 날짜로 저장하라고 설정되었다면, 적용한다.
	if ( bPrevDay == TRUE ) 
	{
		// 어제 날짜로 저장
		//!! m_logLosstime.Sa .SetFilenameOption(_T("PREV"));	// 옵션 적용
		m_logLosstime.WriteItem( -1 );			// 어제 날짜로 파일에 기록한다.
	}
	else
		m_logLosstime.WriteItem();			// 파일에 기록한다.

}



// 자동으로 생성되는 OP/EQ Loss time 전용 기록 함수.
void CMCMain::WriteAutoLossTimeRecord(TLosstimeRecord& rRec)
{
	// EQID;
	m_logLosstime.AddItem( m_rProfileData.sEQID);
	m_logLosstime.AddItem( m_rProfileData.sModelName);
	m_logLosstime.AddItem( GetCurrentUserIDNo() );				// 현재 작업자의 사원번호
	m_logLosstime.AddItem( rRec.sAlarmType );						// Alarm여부


	// 시간 데이터는 계산을 통하지 않고 그대로 기록한다.
	m_logLosstime.AddItem_DateTime( rRec.tmStartTime , TRUE );		// 시작 시간
	m_logLosstime.AddItem_DateTime( rRec.tmResetTime , TRUE );		// 조치 시간
	m_logLosstime.AddItem_DateTime( rRec.tmEndTime, TRUE );			// 종료 시각
	m_logLosstime.AddItem_Double( rRec.fWaitTime, 1 );				// Wait time
	m_logLosstime.AddItem_Double( rRec.fRepairTime, 1 );			// Repair time
	m_logLosstime.AddItem_Double( rRec.fDuration, 1 );				// Duration time, 경과시간은 소숫점 1자리까지만 표기한다.

	m_logLosstime.AddItem( rRec.s1stClass);
	m_logLosstime.AddItem( rRec.s2ndClass);
	m_logLosstime.AddItem( rRec.s3rdClass );


	m_logLosstime.AddItem( rRec.sTPSystemName);
	m_logLosstime.AddItem( rRec.sTPSystemCode);
	m_logLosstime.AddItem( rRec.sAlarmCode);
	m_logLosstime.AddItem( rRec.sRepairCode);


	m_logLosstime.WriteItem();			// 파일에 기록한다.
}



//
// Alarm에 의해 강제적으로 비가동 상태가 종료되었다고 저장한다.
// 2015-03-09, jhLee, 바가동 상태로 설비가 멈춘 상태로 라이트커튼등의 감지로 알람이 발생하면 기존 비가동과 알람에 의한 비가동이 섞이게 된다.
// 이를 방지하기 위해 비가동 중지일때 알람이 발생하면, 기존의 비가동은 완료 시켜주고 알람으로 비가동 사유를 대체한다.
//
void CMCMain::WriteLossTimeByAlarm()
{
	if ( m_rLosstimeRecord.sReasonLine == _T("") )				// 비가동 사유가 없다면 저장할 것도 없다.
	{
		return;
	}

	if ( m_rLosstimeRecord.sAlarmType == _T("A") )				// Alarm 발생 상태에서 다시 Alarm이 발생하였다 ? 논리상 오류로 이전에 발생한 Alarm 정보는 기록하지 않는다.
	{
		return;
	}


	::GetLocalTime( &(m_rLosstimeRecord.tmResetTime) );			// Reset 시간을 현재 시각으로 맞춘다.
	::GetLocalTime( &(m_rLosstimeRecord.tmEndTime) );			// 처리 종료 시각

	// Login과는 다른 사용자 입력이다.
	m_rLosstimeRecord.sID	= GetCurrentUserID();				// 현재 작업자 ID로 
	m_rLosstimeRecord.sIDNo	= GetCurrentUserIDNo();

	//
	// 중지 사유에 따른 처리
	// Auto RunDown에 의해 중지 되었다면,
	if ( (m_rLosstimeRecord.s2ndClass == _T("AUTO RUN DOWN")) || (m_rLosstimeRecord.s2ndClass == _T("AUTO RUNDOWN") ) )
	{
		// 비가동 Log 저장시 AUTO RUn down 기준 시간을 더하여 기록한다.
		m_rLosstimeRecord.dbAddLossTime =  m_rProfileData.nTIME_RunDownTime;
	}
	else
		m_rLosstimeRecord.dbAddLossTime = 0.0;

	// 파일로 기록을 한다.
	WriteLossTimeRecord( m_rLosstimeRecord, FALSE );			 

	ClearLosstimeInfo();														// 비가동 임시기억 초기화

}






// 작업자 시간초과 기록 (비가동 로그에 남긴다)
// 작업자 작업 시간이 정해진 시간을 초과하게되면 기록을 한다.
//
void CMCMain::WriteOPLossTime(int nJigNo)
{
	if ( (nJigNo < 0) || (nJigNo >= eMAX_JigCount) )  return;		// index 

	TCellData* pCell = m_pCellInfo->GetCellData(nJigNo, eIDX_CH_1 );		// Cell Data 1개를 가져온다. Cell 존재여부와 상관없이 시각은 기록되므로 사용이 가능하다.

	// OP 작업시간이 기준보다 초과되었나 ?
	if ( (pCell->dbMoveTACT[eTACT_OPLossTime] < 1.0 )			// 1초 미만이라면 기록하지 않는다. 기록은 1초 단위의 정수형이므로 0초가 기록되는 경우가 많다.
		|| (pCell->dbMoveTACT[eTACT_OPLossTime] > 120.0 ) )		// 2015-04-10, jhLee, 120 초, 즉 60초를 초과하는 값은 무시하도록 한다.
	{
		// m_pLogSeq->Write(_T("[MAIN] Write OP Loss time Skip : %.3f"), dbOPLoss);			
		return;		// 초과된 시간이 없으므로 기록을 할 필요가 없다.
	}

	TLosstimeRecord	rLossRec;	// OP Loss Log를 남기기 위한 데이터 기록
	float fLossTime = (float)( pCell->dbMoveTACT[eTACT_OPLossTime] / 2.0 );	// 2014-11-27, 김길성 책임 요청, 기준 시간을 더하는것을 취소, + m_rProfileData.nTIME_OPLossTime);	// 작업자 손실 시간

	//2015/03/29 JH-PARK
	//시간별 OPLOSS를 기록한다
	SaveTodayProductData(eIDX_Product_TodayOPLoss, int(fLossTime));
				
	rLossRec.sReasonLine	= _T("");					// 선택된 전체 Code 내용
	rLossRec.s1stClass		= _T("RD");					// 각 Class별 선택 내용
	rLossRec.s2ndClass		= _T("OP LOSS");
	rLossRec.s3rdClass		= _T("OP LOSS");
	rLossRec.sMESCode		= _T("000");				// 상위 보고용 코드, 현재 미사용
	rLossRec.sID			= m_rCurrentUser.sID;		// Stop 시킨 작업자 ID
	rLossRec.sIDNo			= m_rCurrentUser.sIDNo;		// 작업자 사원번호;
	rLossRec.sAlarmType		= _T("-");					// Alarm 여부, 중/경 알람여부
	rLossRec.sAlarmCode		= _T("");					// Alarm Code
	rLossRec.sRepairCode	= _T("");					// MD/BM의 경우 조치내역
	rLossRec.sUserMsg		= _T("");					// 사용자 정의 입력
	rLossRec.fWaitTime		= 0.0;						// 알람발생 ~ 고장조치 시작까지
	rLossRec.fRepairTime	= 0.0;						// 실제 조치가 일어난 시간
	rLossRec.fDuration		= fLossTime;				// 작업자 손실 시간
	rLossRec.dbAddLossTime	= 0.0;						// 추가적인 더하는 시각

	//2014-12-24, jhLee, TPSystem code 지정
	rLossRec.sTPSystemName	= _T("OP LOSS");			// TP System loss name
	rLossRec.sTPSystemCode	= _T("01100");				// TP System Loss Code

	// ::GetLocalTime( &(rLossRec.tmStartTime) );			// 정지 종료시각

	//OLD CGxUtility::PrevSecondTime( (int)(pCell->dbMoveTACT[eTACT_OPLossTime]),  rLossRec.tmStartTime );	// 정지 시작 시각
	//2015-03-12, 현재시각으로 모두 통일 CGxUtility::PrevSecondTime( (int)fLossTime,  rLossRec.tmStartTime );	// 정지 시작 시각

	::GetLocalTime( &(rLossRec.tmStartTime) );			// 시각 시각
	::GetLocalTime( &(rLossRec.tmResetTime) );			// 인지 시각
	::GetLocalTime( &(rLossRec.tmEndTime) );			// 정지 종료시각

	//old WriteLossTimeRecord( rLossRec, FALSE );
	// WriteLossTimeRecord_new( rLossRec, FALSE );
	WriteAutoLossTimeRecord( rLossRec );
}


// 설비 시간초과 기록 (비가동 로그에 남긴다)
// 설비 작업 시간이 정해진 시간을 초과하게되면 기록을 한다.
//
void CMCMain::WriteEQLossTime(int nJigNo)
{
	if ( (nJigNo < 0) || (nJigNo >= eMAX_JigCount) )  return;		// index 

	TCellData* pCell = m_pCellInfo->GetCellData(nJigNo, eIDX_CH_1 );		// Cell Data 1개를 가져온다. Cell 존재여부와 상관없이 시각은 기록되므로 사용이 가능하다.

	// EQ 작업시간이 기준보다 초과되었나 ?
	if ( pCell->dbMoveTACT[eTACT_EQLossTime] < 1.0 )	
	{
		// m_pLogSeq->Write(_T("[MAIN] Write OP Loss time Skip : %.3f"), dbOPLoss);			
		return;		// 초과된 시간이 없으므로 기록을 할 필요가 없다.
	}

	TLosstimeRecord	rLossRec;	// OP Loss Log를 남기기 위한 데이터 기록
	float fLossTime = (float)( pCell->dbMoveTACT[eTACT_EQLossTime] / 2.0 );	// + m_rProfileData.nTIME_EQLossTime);	// 설비 손실 시간

	rLossRec.sReasonLine	= _T("");					// 선택된 전체 Code 내용
	rLossRec.s1stClass		= _T("RD");					// 각 Class별 선택 내용
	rLossRec.s2ndClass		= _T("TACT TIME OVER");
	rLossRec.s3rdClass		= _T("TACT TIME OVER");
	rLossRec.sMESCode		= _T("000");				// 상위 보고용 코드, 현재 미사용
	rLossRec.sID			= m_rCurrentUser.sID;		// Stop 시킨 작업자 ID
	rLossRec.sIDNo			= m_rCurrentUser.sIDNo;		// 작업자 사원번호;
	rLossRec.sAlarmType		= _T("-");					// Alarm 여부, 중/경 알람여부
	rLossRec.sAlarmCode		= _T("");					// Alarm Code
	rLossRec.sRepairCode	= _T("");					// MD/BM의 경우 조치내역
	rLossRec.sUserMsg		= _T("");					// 사용자 정의 입력
	rLossRec.fWaitTime		= 0.0;						// 알람발생 ~ 고장조치 시작까지
	rLossRec.fRepairTime	= 0.0;						// 실제 조치가 일어난 시간
	rLossRec.fDuration		= fLossTime;				// 설비 손실 시간
	rLossRec.dbAddLossTime	= 0.0;						// 추가적인 더하는 시각

	//2014-12-24, jhLee, TPSystem code 지정
	rLossRec.sTPSystemName	= _T("TACT TIME OVER");		// TP System loss name
	rLossRec.sTPSystemCode	= _T("01200");					// TP System Loss Code


	//OLD CGxUtility::PrevSecondTime( (int)(pCell->dbMoveTACT[eTACT_EQLossTime]),  rLossRec.tmStartTime );	// 정지 시작 시각
	// ::GetLocalTime( &(rLossRec.tmStartTime) );			// 정지 시작시각
	//2015-03-12, 현재시각으로 모두 통일, CGxUtility::PrevSecondTime( (int)fLossTime,  rLossRec.tmStartTime );		// 정지 시작 시각

	::GetLocalTime( &(rLossRec.tmStartTime) );			// 시각 시각
	::GetLocalTime( &(rLossRec.tmResetTime) );			// 인지 시각
	::GetLocalTime( &(rLossRec.tmEndTime) );				// 정지 종료시각

	//old WriteLossTimeRecord( rLossRec, FALSE );
	//old WriteLossTimeRecord_new( rLossRec, FALSE );
	WriteAutoLossTimeRecord( rLossRec );
}




//
// 비가동 로그를 일일 기준으로 마무리를 수행한다.
//
// 호출하는 축에서 이미 설비 State가 AUTO 상태가 아님을 확인하고 본 함수를 호출한다.
//
// CTime timePrev : 이전 날짜
//
void CMCMain::LosstimeDataCloseProcess(BOOL bPrevDay)
{

	// 대분류가 존재하는가 ? 이미 비가동 사유가 보관되고 있다 -> 처리 가능 !
	if ( m_rLosstimeRecord.s1stClass != _T("") )
	{
		CTime tmToday		= CTime::GetCurrentTime();		// 실제 현재 시각
		CTimeSpan timeOffset(0, 0, 1, 0);					// 1분전
		tmToday -= timeOffset;								// 1분전 시각을 구한다. 

		//d CTime tmToday	= GetToday();						// 오늘 기준일자

		// 종료 시각을 이전 조가 끝나는 시각으로 지정한다.
		::GetLocalTime( &(m_rLosstimeRecord.tmEndTime) );	// 오늘 기준, 현재 시각 (기본값 채우기)

		// 바로 직전 날짜로 만들어준다.
		m_rLosstimeRecord.tmEndTime.wYear			= (WORD)tmToday.GetYear();
		m_rLosstimeRecord.tmEndTime.wMonth			= (WORD)tmToday.GetMonth();
		m_rLosstimeRecord.tmEndTime.wDay			= (WORD)tmToday.GetDay();

		m_rLosstimeRecord.tmEndTime.wHour			= (WORD)tmToday.GetHour();	// 바로 직전시간	
		m_rLosstimeRecord.tmEndTime.wMinute			= 59;		// 59분
		m_rLosstimeRecord.tmEndTime.wSecond			= 59;		// 59초
		m_rLosstimeRecord.tmEndTime.wMilliseconds	= 999;		// 0.999초

		m_rLosstimeRecord.tmResetTime = m_rLosstimeRecord.tmEndTime;
		m_rLosstimeRecord.dbAddLossTime = 0.0;

		// 기준 날짜가 변경되었기에 이전 날짜의 파일에 기록하도록 한다. 
		WriteLossTimeRecord( m_rLosstimeRecord, bPrevDay );			// 이전 날짜로 기록할것인지 구분하여 기록한다.

		// 새로운 시작시간을 현재의 시각으로 만든다.
		::GetLocalTime( &(m_rLosstimeRecord.tmStartTime) );		// 오늘 기준, 현재 시각으로 만들어준다. (기본값 채우기)

		m_rLosstimeRecord.tmStartTime.wMinute		= 0;		// 0분
		m_rLosstimeRecord.tmStartTime.wSecond		= 0;		// 0초
		m_rLosstimeRecord.tmStartTime.wMilliseconds	= 0;		// 0.000초

		// 일단 동일한 시간으로 통일한다.
		m_rLosstimeRecord.tmResetTime	= m_rLosstimeRecord.tmStartTime;
		m_rLosstimeRecord.tmEndTime		= m_rLosstimeRecord.tmStartTime;


		// Alarm이 발생했던것이라면,
		// 자동으로 작업조별로 구간을 나눌때 Alarm 시작 시간을 새롭게 나뉘어진 시간으로 대입시켜준다.
		// 만약 이 루틴을 타지 않는다면, 최초에 Alarm이 발생했던 시간을 간직하고 있을것이다.
		// 본 값은 DlgRepairAlarm에서 비가동 로그를 기록할 때 사용되어진다.
		if ( m_rLosstimeRecord.sAlarmType == _T("A") )
		{
			// Alarm 발생 시각 자체도 현재시각으로 만들어준다.
			CGxAlarmCtrl* pAlarmCtrl = CGxAlarmCtrl::GetInstance();
			CGxAlarmData* pAlarmData = pAlarmCtrl->GetCurrentAlarmData();		

			pAlarmData->tmLossStart	= m_rLosstimeRecord.tmStartTime;				// 새롭게 계산되어진 시작 시간을 대입한다.
		}

		SaveLosstimeInfo();	// 현재 설정된 비가동 정보를 파일로 기록한다.
		
	}//of if 대분류 존재

}





// 2015-03-12, 검증 필요, 오작동으로 혼란 야기 中
// 비가동 로그의 조별 마무리를 수행해준다.
void CMCMain::ShiftCloseProcess()
{
	int nState		= (CGxMaster::GetInstance())->GetState();
	int nNewShift	= GetShift();						// 현재의 작업조를 조회한다.


	// 작업조가 변경되었는가 ?
	if ( nNewShift != m_rRuntimeData.nPrevShift )
	{

		// 2015-04-18, 일단 작업조가 제대로 변경됨을 확인한다.
		CGxLog* pLog = GetLogPointer( eLOG_SYS );
		pLog->Write(_T("Shift change : %d -> %d"), m_rRuntimeData.nPrevShift, nNewShift);		// 변경된 작업조 Log 기록

		// 가동중이 아니라면, 비가동 로그를 작업조별로 분리해서 정리해준다.
		if ( nState != eSTATE_AUTO )
		{
			LosstimeDataCloseProcess( FALSE );			// 같은 기준일내의 작업조가 변경되었으므로 이전 날짜로 기록하지 않는다.
		}




		//if ( nState != eSTATE_AUTO )			// 자동 실행중이 아니라면, 처리한다.
		//{
		//	// 대분류가 존재하는가 ? 이미 비가동 사유가 보관되고 있다 -> 처리 가능 !
		//	if ( m_rLosstimeRecord.s1stClass != _T("") )
		//	{
		//		// CTime tmNow		= CTime::GetCurrentTime();		// 실제 현재 시각
		//		CTime tmToday	= GetToday();						// 오늘 기준일자
		//		CTime tmPrev	= CTime::GetCurrentTime();			// 이전 시각

		//		CTimeSpan timeOffset(0, 0, 1, 0);					// 1분전
		//		tmPrev -= timeOffset;								// 1분전 시각을 구한다. 


		//		// 종료 시각을 이전 조가 끝나는 시각으로 지정한다.
		//		::GetLocalTime( &(m_rLosstimeRecord.tmEndTime) );	// 오늘 기준, 현재 시각

		//		// 바로 직전 날짜로 만들어준다.
		//		m_rLosstimeRecord.tmEndTime.wYear			= (WORD)tmPrev.GetYear();
		//		m_rLosstimeRecord.tmEndTime.wMonth			= (WORD)tmPrev.GetMonth();
		//		m_rLosstimeRecord.tmEndTime.wDay			= (WORD)tmPrev.GetDay();

		//		m_rLosstimeRecord.tmEndTime.wHour			= (WORD)tmPrev.GetHour();	// 바로 직전시간	
		//		m_rLosstimeRecord.tmEndTime.wMinute			= 59;		// 59분
		//		m_rLosstimeRecord.tmEndTime.wSecond			= 59;		// 59초
		//		m_rLosstimeRecord.tmEndTime.wMilliseconds	= 999;		// 0.999초

		//		m_rLosstimeRecord.tmResetTime = m_rLosstimeRecord.tmEndTime;

		//		// 기준 날짜가 변경되었다면, 
		//		// if ( tmToday.GetDay() != tmPrev.GetDay() )
		//		if ( GetIsPrevDay( tmPrev ) == TRUE )						// 조를 마감하는 직전 시각의 기준 날짜가 날짜라면,
		//		{
		//			WriteLossTimeRecord_new( m_rLosstimeRecord, TRUE );		// 직전 날짜로 기록한다.
		//		}
		//		else
		//			WriteLossTimeRecord_new( m_rLosstimeRecord, FALSE );	// 오늘 기준으로 직전 시간으로 기록한다.



		//		// 새로운 시작시간을 현재의 시각으로 만든다.
		//		::GetLocalTime( &(m_rLosstimeRecord.tmStartTime) );				// 오늘 기준, 현재 시각으로 만들어준다.
		//		// m_rLosstimeRecord.tmStartTime.wHour			= (WORD)tmToday.GetHour();		// 현재 시각의 시간으로

		//		m_rLosstimeRecord.tmStartTime.wMinute		= 0;		// 0분
		//		m_rLosstimeRecord.tmStartTime.wSecond		= 0;		// 0초
		//		m_rLosstimeRecord.tmStartTime.wMilliseconds	= 0;		// 0.000초

		//		// 일단 동일한 시간으로 통일한다.
		//		m_rLosstimeRecord.tmResetTime	= m_rLosstimeRecord.tmStartTime;
		//		m_rLosstimeRecord.tmEndTime		= m_rLosstimeRecord.tmStartTime;


		//		// Alarm이 발생했던것이라면,
		//		// 자동으로 작업조별로 구간을 나눌때 Alarm 시작 시간을 새롭게 나뉘어진 시간으로 대입시켜준다.
		//		// 만약 이 루틴을 타지 않는다면, 최초에 Alarm이 발생했던 시간을 간직하고 있을것이다.
		//		// 본 값은 DlgRepairAlarm에서 비가동 로그를 기록할 때 사용되어진다.
		//		if ( m_rLosstimeRecord.sAlarmType == _T("A") )
		//		{
		//			// Alarm 발생 시각 자체도 현재시각으로 만들어준다.
		//			CGxAlarmCtrl* pAlarmCtrl = CGxAlarmCtrl::GetInstance();
		//			CGxAlarmData* pAlarmData = pAlarmCtrl->GetCurrentAlarmData();		

		//			pAlarmData->tmLossStart	= m_rLosstimeRecord.tmStartTime;				// 새롭게 계산되어진 시작 시간을 대입한다.
		//		}

		//		SaveLosstimeInfo();	// 현재 설정된 비가동 정보를 파일로 기록한다.
		//
		//		}//of if 대분류 존재
		// }//of if Not Auto


		
		m_rRuntimeData.nPrevShift = nNewShift;			// 변경된 작업조 적용

		SaveRuntimeData();
	}//of if NewShift

}



//
// VI 검사 PC의 결과 파일과, 내부 복사 사용 파일 삭제, 해당 Cell이 Unloading되면서 수행한다.
//
void CMCMain::DeleteVIResultFile(int nJig)
{
	TCellData* pCell;
	CString sTargetFile;
	CString sResultFile;

	for (int i = 0; i < eMAX_ChCount; i++ )
	{
		pCell = m_pCellInfo->GetCellData(nJig, i);		// 지정 Jig의 Cell Data 획득

		if ( pCell != NULL)
		{
			// 가장 우선, 해당 채널에 Cell이 존재하지 않는다면 삭제할 파일도 없다.
			if ( pCell->bIsExist == TRUE )					// Cell이 존재하는 경우에만 실시
			{
				// 찾을 파일 이름 및 경로
				sTargetFile.Format(_T("%s\\%s.txt") , GetVIResultPath(), pCell->InnerID);				// 검사PC측 파일
				sResultFile.Format(_T("%s\\VIResult\\%s.txt"), GetBasePath(), pCell->InnerID);		// 내부에서 사용할 파일

				::DeleteFile(sTargetFile);				// 검사 PC의 결과 삭제
				::DeleteFile(sResultFile);				// 내부로 복사한 결과 삭제

				// 2014-11-11, jhLee
				// 만약 지워지지 않았다면 한 번 더 삭제를 시도한다.
				if ( PathFileExists( sTargetFile ) != FALSE )
				{
					::DeleteFile(sTargetFile);				// 검사 PC의 결과 삭제
				}

				if ( PathFileExists( sResultFile ) != FALSE )
				{
					::DeleteFile(sResultFile);				// 내부로 복사한 결과 삭제
				}
			}//of if ( pCell->bIsExist == TRUE )
		}//of  if ( pCell != NULL)
	}//of for i

}


//
// VI 검사 PC의 결과 파일과, 내부 복사 사용 모든 파일 삭제, 
//
void CMCMain::DeleteAllVIResultFile()
{
	CString sTargetFile;
	CString sResultFile;

	CFile File;
	CString strPath, strFile;
	CFileFind finder;
	BOOL bWorking;
	CTime timeCreate;
	CTime timeCurrent;
	CTimeSpan timeElapsed;
	int nCnt=0;

		// 찾을 파일 이름 및 경로
		sTargetFile.Format(_T("%s\\*.txt") , GetVIResultPath() );				// 검사PC측 파일
		sResultFile.Format(_T("%s\\VIResult\\*.txt"), GetBasePath());			// 내부에서 사용할 파일

		// 검사 PC 파일 삭제
		bWorking = finder.FindFile(sTargetFile);

		while (bWorking)
		{
			bWorking = finder.FindNextFile();
			if (finder.IsDots())											// 폴더 제외
				continue;

			if (finder.IsDirectory())									// 폴더 제외
				continue;

			finder.GetCreationTime(timeCreate);
			//timeCurrent = CTime::GetCurrentTime();
			//timeElapsed = (timeCurrent - timeCreate);
			strFile = finder.GetFilePath();
			//if ( timeElapsed.GetTotalMinutes() < 10 )			// 10분 이전에 생성된 파일이라면 삭제한다.
			//{
				DeleteFile(strFile);
			}
		//}
		finder.Close();


		// 내부 파일 삭제
		bWorking = finder.FindFile(sResultFile);

		while (bWorking)
		{
			bWorking = finder.FindNextFile();
			if (finder.IsDots())											// 폴더 제외
				continue;

			if (finder.IsDirectory())									// 폴더 제외
				continue;

			//			finder.GetCreationTime(timeCreate);
			//			timeCurrent = CTime::GetCurrentTime();
			//			timeElapsed = (timeCurrent - timeCreate);
			strFile = finder.GetFilePath();
			//			if ( timeElapsed.GetTotalMinutes() < 10 )			// 10분 이전에 생성된 파일이라면 삭제한다.
			//			{
				DeleteFile(strFile);
			//			}
		}
		finder.Close();
		
}


//
// Data PC로 전송하는 Cell 정보 임시 파일을 삭제한다.
//
void CMCMain::DeleteDataPCCellFile(int nJig)
{
	// CSvcTcpCIM* pSvcTcpCIM	= (CSvcTcpCIM*)( (CGxServiceHub::GetInstance())->GetService(NAME_SERVICE_TcpCIM));						// 가온누리 통신
	TCellData* pCell;
	CString sResultFile;

	for (int i = 0; i < eMAX_ChCount; i++ )
	{
		pCell = m_pCellInfo->GetCellData(nJig, i);		// 지정 Jig의 Cell Data 획득

		if ( pCell != NULL)
		{
			if ( pCell->sPrevCellID != _T("") )
			{

				//d pSvcTcpCIM->DeleteCellLogData( pCell->sPrevCellID, TRUE );

				// 삭제할 파일 이름 및 경로
				//OLD sResultFile.Format(_T("%s\\%s.csv"), NAME_DIR_DATAPC_CELL, pCell->sPrevCellID);	
				sResultFile.Format(_T("%s\\%s.csv"), m_rProfileData.sDIR_DataPC_Cell, pCell->sPrevCellID);	

				// 만약 지워지지 않았다면 한 번 더 삭제를 시도한다.
				if ( PathFileExists( sResultFile ) != FALSE )
				{
					::DeleteFile(sResultFile);			// 내부로 복사한 결과 삭제
				}

			}//of if  ( pCell->sPrevCellID != _T("") )

			pCell->sPrevCellID = pCell->CellID;			// 현재의 Cell ID를 대입한다.
		}//of  if ( pCell != NULL)
	}//of for i

}


//
// Data PC로 전송하는 Cell 정보 임시 파일을 모두 일괄 삭제한다.
//
void CMCMain::DeleteAllDataPCCellFile()
{
	// 모든 파일 삭제하는 함수 호출
	//구현	theDataPCData.m_tCellLogData.DeleteCellLogData( _T(""), FALSE );

}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GUI 관련


// Sub-Dailog를 보여주거나 감추어준다.
void CMCMain::ShowSubDialog(int nIdx, BOOL bShow)
{

	::PostMessage( m_hOwner, UDMSG_DISP_SUBDIALOG, (WPARAM)nIdx, (LPARAM)bShow);

	//d CFrameWnd * pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);			// MainFrame 획득
	//d m_listDialogData[nIdx].pDialog->SetWindowPos(&(pFrame->wndTopMost),0,0,0,0,SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);		// 최상위로 만든다.
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RS232C를 이용한 Label Printer 제어

//	CCommThread		m_CommPrinter;				// Label Printer용 Serial Class

//PRINT
/*
// Label Printer용 시리얼 통신이 Open되었나 ?
BOOL CMCMain::IsPrinterOpen(void)
{
	return m_CommPrinter.m_bConnected;				// 연결 여부
}


// Label Printer용 시리얼 포트를 연다.
BOOL CMCMain::OpenLabelPrinter(int nPortNo)
{
	// 이미 열려있다면, 닫는다.
	if ( m_CommPrinter.m_bConnected )
	{
		m_CommPrinter.ClosePort();
	}

	// 지정된 조건으로 통신 포트를 열어준다.
	m_CommPrinter.InitCommPort( nPortNo, CBR_115200, 8, ONESTOPBIT, NOPARITY);

	return m_CommPrinter.m_bConnected;

}

// Label Printer용 시리얼 포트를 닫는다.
void CMCMain::CloseLabelPrinter(void)
{
	if ( m_CommPrinter.m_bConnected )
	{
		m_CommPrinter.ClosePort();
	}
}

// Test출력을 한다.
BOOL CMCMain::TestPrintOut()
{
	CString sMsg;
	CString sDefect = _T("Test Print"); 

	sMsg.Format(_T("SM%d,%d"), 130, 40);
	LabelPrintOut(-1, sMsg);						// 상/하 마진 설정

	LabelPrintOut(0, sDefect);						// Cell ID 출력
	LabelPrintOut(1, _T("U123456")) ;				// 불량 코드
	LabelPrintOut(2, _T("Label Test Print") );		// 불량명

	CTime dateTm = CTime::GetCurrentTime();
	CString strLineMsg;

	strLineMsg.Format(_T("%04d%02d%02d_%s"),dateTm.GetYear(), dateTm.GetMonth(),dateTm.GetDay()
										   ,m_rProfileData.sCellLog_LineID);						// 시간 + LIne명;
	
	LabelPrintOut(3, strLineMsg );		// 시간+Line명

	TestBarcodePrint(_T("A2EG1S53DOKAD024"));			// 2차원 바코드 출력


	return LabelPrintOut(-1, _T("P1") ) > 0;					// 실제 출력 명령
}

// 해당 채널의 내용을 출력한다.
BOOL CMCMain::DefectPrintOut(int nJig, int nCh)
{
	// 포트가 열린 경우에만 출력한다.
	if ( m_CommPrinter.m_bConnected != TRUE ) return FALSE;


	if ( (nJig >= 0) && (nJig < eMAX_JigCount) && (nCh >= 0) && (nCh < eMAX_ChCount) )
	{
		TCellData*	pCell = m_pCellInfo->GetCellData(nJig, nCh);

		if ( pCell == NULL ) return FALSE;				// Cell data 오류

		// 2015/04/13 JH-PARK
		// Label Print 시 해당시간과 Line명을 출력한다. 가충현K 요청

		// 지정한 데이터들을 출력시킨다.
		CString sMsg;

		//sMsg.Format(_T("SM%d,%d"), 130, 40);
		sMsg.Format(_T("SM%d,%d"), 120, 40);		// Center Line 변경  //2015/05/03 JH-PARK
		LabelPrintOut(-1, sMsg);					// 상/하 마진 설정

		LabelPrintOut(0, pCell->CellID);			// Cell ID 출력
		LabelPrintOut(1, pCell->MesCode);			// 불량 코드
		LabelPrintOut(2, pCell->PrintName);			// 불량명, 영문

		// CTime dateTm = CTime::GetCurrentTime();
		SYSTEMTIME tmNow;
		CString strLineMsg;

		::GetLocalTime( &tmNow );

		// 검사 시간 + Line명 출력
		strLineMsg.Format(_T("%04d%02d%02d_%02d%02d%02d_%s"), 
					tmNow.wYear, tmNow.wMonth, tmNow.wDay, tmNow.wHour, tmNow.wMinute, tmNow.wSecond, m_rProfileData.sCellLog_LineID);		

		LabelPrintOut(3, strLineMsg );				// 시간+Line명


		return LabelPrintOut(-1, _T("P1") ) > 0;					// 실제 출력 명령
	}

	return FALSE;
}


// 지정 줄에 문자열을 출력해준다. (버퍼에 출력)
DWORD CMCMain::LabelPrintOut(int nLn, CString sMsg)
{
	// 포트가 열린 경우에만 출력한다.
	if ( m_CommPrinter.m_bConnected != TRUE ) return 0;

	CString sSendMsg;
	TCHAR cTCCmd[eMAX_PrinterBuffer];
	char	cSendData[eMAX_PrinterBuffer];


	int nLen = 0;
	int nHeight = 0;
	int nGap = 0;

	if ( nLn < 0 )			// 줄 번호가 음수라면, 특수 지정 명령이다.
	{
		sSendMsg.Format( _T("%s%c%c"), sMsg, 0x0d, 0x0a);				// 뒤에 CR/LF를 붙여준다.
	}
	else
		{
			switch ( nLn )
			{
				case 0 : nHeight = 0;		break;
				case 1 : nHeight = 20;		break;
				case 2 : nHeight = 40;		break;
				case 3 : nHeight = 60;		break;
				case 4 : nHeight = 80;		break;
				default :
					nHeight = nLn * 20;
			}//of switch

			// Label Printer에 맞는 명령문을 만들어 준다.
			sSendMsg.Format( _T("T%d,%d,c,1,1,0,0,N,N,\'%s\'%c%c"), nGap, nHeight, sMsg, 0x0d, 0x0a);		
		}//of else


	nLen = sSendMsg.GetLength();				// 전송할 글자 수

	_tcscpy_s(cTCCmd,	_countof(cTCCmd),	sSendMsg);	// CString -> TCHAR로변환
	::WideCharToMultiByte(CP_ACP, NULL, cTCCmd, -1, cSendData, eMAX_PrinterBuffer, NULL, FALSE);		// TCHAR -> char로 변환

	return  m_CommPrinter.WriteComm( (BYTE*)cSendData, strlen( cSendData ) );// nLen );
}


// 2차원 바코드를 출력한다.
BOOL CMCMain::TestBarcodePrint(CString sMsg)
{
	// 포트가 열린 경우에만 출력한다.
	if ( m_CommPrinter.m_bConnected != TRUE ) return 0;

	CString sSendMsg;
	TCHAR cTCCmd[eMAX_PrinterBuffer];
	char	cSendData[eMAX_PrinterBuffer];


	int nLen = 0;
	int nSize = 3;
	int nPosX = 140;
	int nPosY = 0;

	// Label Printer에 맞는 명령문을 만들어 준다.
	sSendMsg.Format( _T("B2%d,%d,D,%d,N,\'%s\'%c%c"), nPosX, nPosY, nSize, sMsg, 0x0d, 0x0a);		

	nLen = sSendMsg.GetLength();				// 전송할 글자 수

	_tcscpy_s(cTCCmd,	_countof(cTCCmd),	sSendMsg);	// CString -> TCHAR로변환
	::WideCharToMultiByte(CP_ACP, NULL, cTCCmd, -1, cSendData, eMAX_PrinterBuffer, NULL, FALSE);		// TCHAR -> char로 변환

	return  m_CommPrinter.WriteComm( (BYTE*)cSendData, strlen( cSendData ) );// nLen );
}
*/
//PRINT end


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//void CMCMain::PM_ModeChange()
//{
//	::SendMessage( ( (CMainFrame*)AfxGetMainWnd() )->m_hWnd, UM_UI_MENULOCK, (WPARAM)FALSE, (LPARAM)0);
//	::PostMessage( ( (CMainFrame*)AfxGetMainWnd() )->m_hWnd, UM_MAINMENU_CHANGE_REQ, (WPARAM)eFORM_Auto, (LPARAM)TRUE);
//}

// TPSystem 저장하기 위한 함수 추가 시작 20141226-2 jhkim1
void CMCMain::SaveTPSyetem(/*CString sCode, CString sName*/)
{
// 	m_sTPSystemCode = m_rLosstimeRecord.sTPSystemCode;	// TPSystem CODE 상위에 보고되는 변수에 저장 20141226-2 jhkim1
// 	m_sTPSystemName = m_rLosstimeRecord.sTPSystemName;	// TPSystem NAME 상위에 보고되는 변수에 저장 20141226-2 jhkim1

	CString sFilename;
	sFilename.Format(_T("%s\\INI\\%s"), m_sBasePath, NAME_FILE_TPSYSTEM);	// 해당 ini파일 경로 지정
//	sFilename = _T("D:\\UB-AMT\\Data\\Profile\\TPSystem.ini");	// 해당 ini파일 경로 지정

	WritePrivateProfileString(_T("TPSYSTEM"), _T("CODE"), m_sTPSystemCode , sFilename);	// code 저장
	WritePrivateProfileString(_T("TPSYSTEM"), _T("NAME"), m_sTPSystemName , sFilename);	// name 저장
}
// TPSystem 저장하기 위한 함수 추가 끝 20141226-2 jhkim1


// 저장된 TPSystem 불러오는 함수 20141226-2 jhkim1
BOOL CMCMain::LoadTPSystem()
{
	CGxIniFile iniFile;
	CString sFilename;
	int nLangIdx = CGxGUIFrame::GetInstance()->GetDispLanguage();			// 현재 표시되고 있는 언어 index

	sFilename.Format(_T("%s\\INI\\%s"), m_sBasePath, NAME_FILE_TPSYSTEM);	// 해당 ini파일 경로 지정
//	sFilename.Format(_T("D:\\UB-AMT\\Data\\Profile\\%s"), NAME_FILE_TPSYSTEM);	// 해당 ini파일 경로 지정
//	sFilename = _T("D:\\UB-AMT\\Data\\Profile\\TPSystem.ini");	// 해당 ini파일 경로 지정

	// 파일이 존재하는지 점검
	if ( CGxUtility::IsExistFile( sFilename ) == FALSE ) 
	{
		CString sMsg;
		sMsg.Format(_T("* File not found : %s"), sFilename);
		
		CGxMsgBox	dlgMsgBox;
		dlgMsgBox.SetTitle(_T("알림"), _T("Notifications"), _T("thông báo"));
		dlgMsgBox.SetMessage(FALSE, sMsg, sMsg, sMsg, nLangIdx);
		dlgMsgBox.DoModal();
	}

	// 파일이 존재하면 파일에서 데이터를 읽어온다.
	iniFile.SetIniFilename( sFilename );			// file이름을 지정

	m_sTPSystemCode = iniFile.GetString( _T("TPSYSTEM"), _T("CODE"),	_T(""));
	m_sTPSystemName = iniFile.GetString( _T("TPSYSTEM"), _T("NAME"),	_T(""));

	return TRUE;
}

//// 설정되어 있는 언어를 가져와서 Bottom Menu에 알려준다;
//void CMCMain::DisplayLanuage()
//{
//	::PostMessage( ( (CMainFrame*)AfxGetMainWnd() )->m_hWnd, UDMSG_DISP_LAN_CHANGE, (WPARAM)GetLanguageType() + 1, (LPARAM)0);
//}


//판정 완료(Retest제외) Cell ID를 기록한다 ( 최대 4개)
void CMCMain::WriteJudgeCell_ID(CString strID)
{

	if(strID == _T("")) // Cell ID가 없을경우 기록하질 않는다;
		return;

	CGxIniFile iniFile;
	CString sFilename;

//	sFilename.Format(_T("D:\\UB-AMT\\Data\\%s"), NAME_FILE_JUDGE_CELL_ID);	// 해당 ini파일 경로 지정
	sFilename.Format(_T("%s\\INI\\%s"), GetBasePath(), NAME_FILE_JUDGE_CELL_ID);		// 제외시킬 Cell ID List file

	iniFile.SetIniFilename( sFilename );			// file이름을 지정
	CString sSection = _T("JUDGE_CELL_ID");
	
	//해당 파일에 기록되어 있는 Cell ID를 가져온다;
	CString sID[4];

	sID[0] = iniFile.GetString( sSection, _T("sID_1"),	_T(""));
	sID[1] = iniFile.GetString( sSection, _T("sID_2"),	_T(""));
	sID[2] = iniFile.GetString( sSection, _T("sID_3"),	_T(""));
	sID[3] = iniFile.GetString( sSection, _T("sID_4"),	_T(""));

	
	//새로 입력해야할 ID를 파일로 기록한다
	//이때 이전에 기록된 ID는 처음에 기록된 ID기준으로 한개씩 삭제후 새로 작성한다 ( 최대4개)

	//4개의 Cell_ID가 모두 기록되어있읅겨우 한칸씩 위로 올려준다
	if(sID[0] != _T("") && sID[1] != _T("") && sID[2] != _T("") && sID[3] != _T("") )
	{
		iniFile.WriteString(sSection,_T("sID_1"), sID[1]);
		iniFile.WriteString(sSection,_T("sID_2"), sID[2]);
		iniFile.WriteString(sSection,_T("sID_3"), sID[3]);
		iniFile.WriteString(sSection,_T("sID_4"), strID);
	}//end of if

	//새로 등록해야야한다면
	else
	{
		// 새로 ID를 등록해야할경우
		CString sName;
		for(int i=0; i<4; i++)
		{
			sName.Format(_T("sID_%d"),i+1);
			if(sID[i] == _T(""))	// 기록해야할 ID값이 존재하지 않으면
			{
				iniFile.WriteString(sSection,sName, strID);
				break;	//파일 기록하면 빠져나온다
			}
		}//end of for
	}//end of else

	//최종 Data를 Mapping 해준다;
	m_mapJudgeCellID.RemoveAll();	// Mapping 초기화


	// INI파일 일거온다`	
	sID[0] = iniFile.GetString( sSection, _T("sID_1"),	_T(""));
	sID[1] = iniFile.GetString( sSection, _T("sID_2"),	_T(""));
	sID[2] = iniFile.GetString( sSection, _T("sID_3"),	_T(""));
	sID[3] = iniFile.GetString( sSection, _T("sID_4"),	_T(""));

	for(int i=0; i<4; i++)
	{
		if(sID[i] != _T(""))
			m_mapJudgeCellID.SetAt(sID[i], sID[i]);		// Map에 추가한다.
	}
}

