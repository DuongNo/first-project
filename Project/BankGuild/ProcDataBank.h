#pragma once
#include <vector>
using namespace std;

#include "TrayBank.h"
#include "Etc\StopWatch.h"

struct CONST_MACHINE_STATE
{
	CString strName;
	enum ID 
	{
		MACHINE_PAUSE	= 0,
		MACHINE_INIT	,
		MACHINE_CHECK,  //2018-04-20,SKCHO
		MACHINE_IDLE	,
		MACHINE_RUN		,
		MACHINE_FAULT	,
		MACHINE_INTERLOCK
		
	};
};
extern const CONST_MACHINE_STATE			ConstMachineState[];

struct CONST_CIM_STATE
{
	CString strName;
	enum ID 
	{
		CIM_OFFLINE		= 0,
		CIM_REMOTE		= 1,
		CIM_LOCAL		= 2
	};
};
extern const CONST_CIM_STATE			ConstCimState[];

struct CONST_EQUIP_PART
{
	CString strNameKor;
	CString strNameEng;
	CString strNameVnm;
	enum ID 
	{
		EQUIP_PART_LOAD	= 0,
		EQUIP_PART_INSP,
		EQUIP_PART_UNLOAD,
		EQUIP_PART_ETC,
	};
};
extern const CONST_EQUIP_PART			ConstEquipPart[];

struct CONST_EQUIP_UNIT
{
	CString strNameKor;
	CString strNameEng;
	CString strNameVnm;
	enum ID 
	{
		EQUIP_UNIT_LOAD_COMMON	= 0,
		EQUIP_UNIT_LOAD_CONV				,
		EQUIP_UNIT_LD_ROBOT			,
		EQUIP_UNIT_UPPERSHUTTLE				,
		EQUIP_UNIT_LOWERSHUTTLE				,
		EQUIP_UNIT_PREALIGNTR					,
		EQUIP_UNIT_PREALIGNSTG						,
		EQUIP_UNIT_PREALIGN			,
		EQUIP_UNIT_LDTR						,
		EQUIP_UNIT_TURNTABLE						,
		EQUIP_UNIT_AZONE						,
		EQUIP_UNIT_BZONE						,
		EQUIP_UNIT_CZONE						,
		EQUIP_UNIT_DZONE						,
		EQUIP_UNIT_ULSTG			,
		EQUIP_UNIT_ULDTR			,
		EQUIP_UNIT_ULD_ROBOT					,
		EQUIP_UNIT_GOOD_TRAY_TR,
		EQUIP_UNIT_NG_TRAY_TR	,
		EQUIP_UNIT_GOOD_IN_TRAY					,
		EQUIP_UNIT_GOOD_OUT_TRAY					,
		EQUIP_UNIT_NG_TRAY					,
		EQUIP_UNIT_AGV					,
		EQUIP_UNIT_MONITORING					,
	};
};
extern const CONST_EQUIP_UNIT			ConstEquipUnit[];


// ALARM 정보 [11/16/2016 WHLEE]
struct ALARM_INFOMATION
{
	ALARM_ID	iERR_ID;
	CString     szERR_ID;   // 2017-03-23,skcho 인터락 ID를 받기 위해서 문자열 추가
	CString		szERR_NAME;
	CTime		TimeOccur;
	CTime		TimeClear;
	CString 	szTimeTake;
	CString		szLevel;

	void SetTakeTime()
	{
		CTimeSpan Elapse = TimeClear - TimeOccur;
		szTimeTake.Format(_T("%02d:%02d:%02d"), Elapse.GetHours(), Elapse.GetMinutes(), Elapse.GetSeconds() );
	}

	void WriteErrorBuf()
	{
		CString szOccTime, szClearTime, szTakeTime;
		szOccTime.Format(_T("%04u-%02u-%02u %02d:%02d:%02d"),TimeOccur.GetYear(), TimeOccur.GetMonth(), TimeOccur.GetDay(), TimeOccur.GetHour(), TimeOccur.GetMinute(), TimeOccur.GetSecond() );
		szClearTime.Format(_T("%04u-%02u-%02u %02d:%02d:%02d"), TimeOccur.GetYear(), TimeOccur.GetMonth(), TimeOccur.GetDay(),TimeClear.GetHour(), TimeClear.GetMinute(), TimeClear.GetSecond() );
		szTakeTime = szTimeTake;

		CString szFileDate, szPath, szLine, szTitle;
		CTime timeCur = TimeClear;	

		if ( timeCur.GetHour() < 7 )
			timeCur += CTimeSpan(-1,0,0,0);

		szFileDate.Format(_T("%04u-%02u-%02u"),(WORD)timeCur.GetYear(), (WORD)timeCur.GetMonth(), (WORD)timeCur.GetDay());
		szPath.Format(_T("D:\\Cheetah\\Log\\Debug\\%s\\ErrorLog.csv"), szFileDate) ;
	

		CStdioFile file;
		CFileException e;

		//szTitle.Format(_T("ERR_ID, ERR_NAME, OCCUR_TIME, CLEAR_TIME, TAKE_TIME"));
		szTitle.Format(_T("START TIME, END TIME, SPAN, CODE, NAME, LEVEL"));
		if ( !file.Open(szPath.GetBuffer(), CFile::modeRead | CFile::modeWrite , &e))	
		{
			if ( file.Open(szPath.GetBuffer(), CFile::modeCreate | CFile::modeWrite , &e ) )
			{
				file.SeekToEnd();															
				file.WriteString(szTitle+_T("\n"));	
				file.Close();
			}
			else
			{
				return;
			}
		}
		else
		{
		  file.Close();
		}


		//szLine.Format(_T("%s,%s,%s,%s,%s"), iERR_ID, szERR_NAME, szOccTime, szClearTime, szTakeTime);
		//szLine.Format(_T("%s,%s,%s,%d,%s,%s"), szOccTime, szClearTime, szTakeTime, iERR_ID, szERR_NAME, szLevel);
		szLine.Format(_T("%s,%s,%s,%s,%s,%s"), szOccTime, szClearTime, szTakeTime, szERR_ID, szERR_NAME, szLevel);
		if ( file.Open(szPath.GetBuffer(), CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate, &e ) )
		{
			file.SeekToEnd();
			file.WriteString(szLine+_T("\n"));
			file.Close();
		}
	}
};

// IDLE 정보 [2017/7/20 JSJUNG]
struct IDLE_DELAY_INFOMATION
{
	CString		szIDLEName;
	CTime		TimeOccur;
	CTime		TimeClear;
	CString 	szTimeTake;

	void SetTakeTime()
	{
		CTimeSpan Elapse = TimeClear - TimeOccur;
		szTimeTake.Format(_T("%02d:%02d:%02d"), Elapse.GetHours(), Elapse.GetMinutes(), Elapse.GetSeconds() );
	}

	void WriteIDLEBuf()
	{
		CString szOccTime, szClearTime, szTakeTime;
		szOccTime.Format(_T("%04u-%02u-%02u %02d:%02d:%02d"),TimeOccur.GetYear(), TimeOccur.GetMonth(), TimeOccur.GetDay(), TimeOccur.GetHour(), TimeOccur.GetMinute(), TimeOccur.GetSecond() );
		szClearTime.Format(_T("%04u-%02u-%02u %02d:%02d:%02d"), TimeOccur.GetYear(), TimeOccur.GetMonth(), TimeOccur.GetDay(),TimeClear.GetHour(), TimeClear.GetMinute(), TimeClear.GetSecond() );
		szTakeTime = szTimeTake;

		CString szFileDate, szPath, szLine, szTitle;
		CTime timeCur = TimeClear;	

		if ( timeCur.GetHour() < 7 )
			timeCur += CTimeSpan(-1,0,0,0);

		szFileDate.Format(_T("%04u-%02u-%02u"),(WORD)timeCur.GetYear(), (WORD)timeCur.GetMonth(), (WORD)timeCur.GetDay());
		szPath.Format(_T("D:\\Cheetah\\Log\\Debug\\%s\\IDLELog.csv"), szFileDate) ;

		CStdioFile file;
		CFileException e;

		szTitle.Format(_T("IDLE NAME, START TIME, END TIME, SPAN"));
		
		if ( !file.Open(szPath.GetBuffer(), CFile::modeRead | CFile::modeWrite , &e))			
		{
			if ( file.Open(szPath.GetBuffer(), CFile::modeCreate | CFile::modeWrite , &e ) )
			{
				file.SeekToEnd();															
				file.WriteString(szTitle+_T("\n"));	
				file.Close();
			}
			else
			{
				return;
			}
		}
		else
		{
		  file.Close();
		}

		szLine.Format(_T("%s,%s,%s,%s"), szIDLEName, szOccTime, szClearTime, szTakeTime);
		if ( file.Open(szPath.GetBuffer(), CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate, &e ) )
		{
			file.SeekToEnd();
			file.WriteString(szLine+_T("\n"));
			file.Close();
		}
	}
};

#define MAX_STRUCT 50
struct structSUBCARRINFO
{
	CString     m_strSubCarrier_ID;    
	CString     m_strCellQty;

};

#define ALARM_MAX 20
#define INTERLOCKMAX 20
#define OPCALLMAX 20

#define MCR_FAIL_CNT 5

//2018wjp
#define CELL_ID_ROW_MAX 2
#define CELL_ID_COL_MAX 5


// 설비 구동중에 사용되는 휘발성 Data들 관리하는 Class
class CProcDataBank
{
public:
	CProcDataBank(void);
	~CProcDataBank(void);

	//////////////////////////////////////////////////////////////////////////
	// 설비 상태
	CString m_strOperationVersion;
	BOOL m_bProgramInitFinish;
	CONST_MACHINE_STATE::ID m_MachineState;
	CONST_MACHINE_STATE::ID m_OldMachineState;   //2018-04-20,SKCHO, 이전 장비 상태 저장 변수 추가
	void ChangeMachineState(CONST_MACHINE_STATE::ID newState);
	CONST_MACHINE_STATE::ID GetMachineState() { return m_MachineState;};
	//////////////////////////////////////////////////////////////////////////
	// CIM 관련
private:
	CONST_CIM_STATE::ID m_CimState;
public:
	void ChangeCimState(CONST_CIM_STATE::ID newState);
	CONST_CIM_STATE::ID GetCimState() { return m_CimState; }

	int		m_MsgOpCallCnt;
	int		m_MsgInterlockCnt;
	CString m_strOPCallID;
	CString m_strOldOPCallID;
	CString m_strOPCallMsg[10];

	CString m_strInterlockID;
	CString m_strOldInterlockID;
	CString m_strInterlockMsg[10];

	CString m_strTerminalMsg;

	//2017-11-01,SKCHO, INTERLOCK2 변수 추가
	CString m_strUnitInterlockID;
	CString m_strOldUnitInterlockID;
	CString m_strUnitInterlockMsg[10];
	int		m_MsgUnitInterlockCnt;
	CString m_strUnitModuleID;
	unsigned char     m_cUnitModuleNum;

	CString m_strDataPCVersion;

	CString m_strRCMD;
	// RCMD 아이디가 알람 발생 후에 트랙인 재시도를 안하고 RCMD 체크를 할 때 1번 체널에서 2번 ID를 체크하는 경우가 있어 분리. 2017/06/08 HSK.
	CString m_strRCMDCellIDCh1;
	CString m_strRCMDCellIDCh2;
	// RCMD 아이디가 현재 1번 체널인지 2번 체널인지 체크 하는 변수 2017/06/08 HSK.
	BOOL m_bRCMDCh1Chk;
	BOOL m_bRCMDCh2Chk;

	void SetOPCall();
	void SetInterlock();
	void SetInterlock2();   //2017-11-01,SKCHO 인터락 추가
	void SetTerminalMsg();	
	//////////////////////////////////////////////////////////////////////////
	// Alarm 관련
	ALARM_MSG m_AlarmMsg[ALM_MAX];
	void ReadAlarmFile();

	//2018-05-07
	int m_ChangeState1;
	ALARM_ID m_AlarmID;
	SYSTEMTIME m_AlarmTime;
	CONST_EQUIP_PART::ID m_AlarmPart;
	CONST_EQUIP_UNIT::ID m_AlarmUnit;
	CONST_EQUIP_PART::ID GetEqiupPart_to_AlarmID(ALARM_ID id);
	CONST_EQUIP_UNIT::ID GetEqiupUnit_to_AlarmID(ALARM_ID id);
	void AlarmMsgHappen(ALARM_ID id, CString strRemark = _T(""));
	void AlarmHappen(ALARM_ID id, CString strRemark = _T(""));
	void AlarmClear();
	ALARM_INFOMATION	alarm_info[ALARM_MAX];  //2017-04-15,skcho, 다중알람발생시 로그에 정확한 시간 기록하기 위해서 배열로 만듦

	ALARM_INFOMATION	alarm_info_op[OPCALLMAX];  
	ALARM_INFOMATION	alarm_info_inter[INTERLOCKMAX];  

	map<int, DEFECT_MES_CODE> iMapDefectList;


	ALARM_ID m_AlarmID_List[100];//2017-03-06,skcho, 발생한 알람을 리스트에 저장한다
	int m_nAlarmCnt;//2017-03-06,skcho, 발생한 알람을 리스트에 저장한다
	//////////////////////////////////////////////////////////////////////////
	// 그 외 Run시 휘발성으로 관리되는 변수들

	CTrayBank m_TrayBank;

	void ResetTemporarilyValue();		// Lot End 등 설비 內 Cell을 전부 제거하고 처음부터 다시 돌릴 때 이거로 값들을 다 리셋한다
	void ResetAlignData();	

	int m_nCellCntX_Tray[TRAY_PORT_MAX];			// 현재 Tray 안에 담겨있는 Cell 수량
	int m_nCellCntY_Tray[TRAY_PORT_MAX];			// 현재 Tray 안에 담겨있는 Cell 수량
	BOOL m_bExistTray_TrayTr[TRAY_PORT_MAX];		// Tray TR이 Tray를 집고 있는지...
	//////////////////////////////////////////////////////////////////////////
	///////INNER ID 발번 변수
	int m_nIDGenIndex;
	//////////////////////////////////////////////////////////////////////////
	//Inspection Station Part
	//////////////////////////////////////////////////////////////////////////
	BOOL m_bTableTurnStatus[ZONE_ID_MAX];   //각 존마다 step end 이면 플레그를 TRUE로 세우고 테이블 턴이 완료되면 FALSE로 변경
	int		m_nInspTablePos;
	BOOL	m_bInsptableTurn;

	AZONE_CELL_EXIST isAZoneCell;
	/////////////////// MIT Align
	ALIGN_RESULT		m_nPA_MITAlignResult[JIG_CH_MAX];
	double	m_dPA_MITAlignX[JIG_CH_MAX];	
	double	m_dPA_MITAlignY[JIG_CH_MAX];	
	double	m_dPA_MITAlignTH[JIG_CH_MAX];	
	double	m_dPA_MITAlignScore[JIG_CH_MAX][E_MARK_MAX];	

	ALIGN_RESULT		m_nVI_MITAlignResult;
	double				m_dVI_MITAlignScore;
	double				m_dVI_MITAlignTH;	

	BOOL		m_bMarkSaveSuccess;
	BOOL		m_bMarkSaveRecv;

	// MIT Edge Align
	CString m_strLARecipeName;
	CString m_strLA_AlignOK;// ALIGN_RES OK or NG
	CString m_strLA_TeachingOK;// ALIGN_TEACHING_RES OK or NG
	CString m_strLAResX;	// ALIGN_RES X축
	CString m_strLAResY;	// ALIGN_RES Y축
	CString m_strLAResT;	// ALIGN_RES Theta축

	//2017-05-23,skcho, mcr data 저장변수 채널별 선언
	CString m_strCh1LastCellID;
	CString m_strCh2LastCellID;
	int m_nMCRChannel;

	CString m_strTrayID;
	CString m_strCloseTrayID;	//덮는 뚜껑 ID  
	//2017-07-07,SKCHO
	CString m_strClose352TrayID;	//352보고용 트레이 ID   // OUTBUFFER 포트에 트레이가 있고, LOTEND했을 경우 버퍼포트에서 트레이 뺄경우 352,353 트레이 ID가 중복되는 버그
	CString m_strClose353TrayID;	//353보고용 트레이 ID  

	CString m_strTrayAlignRecipeName;
	CString m_strOKTray_AlignOK;// ALIGN_RES OK or NG
	CString m_strOKTray_TeachingOK;
	CString m_strNGTray_AlignOK;// ALIGN_RES OK or NG
	CString m_strNGTray_TeachingOK;

	// TurnTable
	BOOL m_bTurnStart;

	BOOL m_bOriginSuccess;

	int GOODTrayBitX;
	int GOODTrayBitY;
	int NGTrayBitX;
	int NGTrayBitY;


	// LossTime 관리
	struct LOSS_INFOMATION
	{
		BOOL bFirstClass;
		BOOL bSecondClass;
		BOOL bThirdClass;
		CString strFirstClass;
		CString strSecondClass;
		CString strThirdClass;
		CString strFirstGroup;
		CString strSecondGroup;
		CString strThirdGroup;
		CString strLossCode;
	};
	
	LOSS_INFOMATION	Loss_info;

	void ResetLossData();

	BOOL m_bUserStop;
	// 포트 정보.
	// Tray 요청 했는지 여부 플레그.
	BOOL m_bGoodTrayReq;
	BOOL m_bNGTrayReq;

	// Manual로 트레이 요청 했는지 여부 플레그
	BOOL m_bGoodTrayInSWReq;
	BOOL m_bGoodTrayOutSWReq;
	BOOL m_bNGTrayInSWReq;
	BOOL m_bNGTrayOutSWReq;

	BOOL m_bGoodTrayInMuteSWChk;
	BOOL m_bGoodTrayOutMuteSWChk;
	BOOL m_bNGTrayMuteSWChk;

	// 2017.2.3 bgkim
	BOOL m_bGoodInTrayMuteOn;
	BOOL m_bGoodOutTrayMuteOn;
	BOOL m_bNGTrayMuteOn;

	struct PORT_INFOMATION
	{
		CString GoodInPortData[8];
		CString GoodOutPortData[8];
		CString NGPortData[8];
	};

	PORT_INFOMATION	Port_info;

	void ResetPortData();

	// 이 변수로 Lower Shuttle에 Cell이 한개가 놓이고 Upper Shuttle이 못오도록 한다. Inner ID 꼬이지 않도록 하기 위해.
	BOOL m_bLowerPutCompInterlock;

	// 이 변수로 해당 Shuttle이 Load가 끝냈음을 확인 한다. Shuttle에 Cell이 한개만 있을 경우를 위한 추가변수. 
	BOOL m_bLowerPutComp;
	BOOL m_bUpperPutComp;

	// DCR 찍을지 말지 변수
	BOOL m_bDCRChk;
	// TRAY ALIGN 찍을지 말지 변수
	BOOL m_bGoodTrayAlignChk;
	BOOL m_bNGTrayAlignChk;

	CStopWatch	m_CellLoadTact;	
	CStopWatch	m_CellUnloadTact;
	BOOL		m_bFirstLoadTactStart;	
	BOOL		m_bFirstUnloadTactStart;

	CStopWatch	m_CellLoadChkTact;
	BOOL		m_CellLoadChk;
	BOOL		m_CellLoadRunChk;

	BOOL PRE_ALARM_ID[ALM_MAX];  // 2017.2.6 bgkim
	
	BOOL m_bOKTrayTRLotEnd; // 2017.2.6 bgkim
	BOOL m_bNGTrayTRLotEnd; // 2017.2.6 bgkim

	// 2017.2.8 bgkim
	// MUTE 스위치 누를 시 토글 방지용
	BOOL m_bLDInMuteSWIn;
	BOOL m_bLDOutMuteSWIn;
	BOOL m_bNGMuteSWIn;
	// Job End 기능용 타이머
	CStopWatch m_timerJobEnd;
	
	// D Zone TeactTime Test
	BOOL m_bDZoneTT;
	DWORD m_dwDZoneTT1;
	DWORD m_dwDZoneTT2;

	// Load Conv Timer
	CStopWatch m_timerLoadConv;
	
	// Upper MCR 수행 여부 판단 변수.
	BOOL m_bUpperMCRCompChk;

	CString m_strTrayQty;
	CString m_strCellQty;
	CString m_strReplyCode;

	int m_nGoodAlignCount;  //2017-03-21,skcho, GOOD ALIGN TRAY 카운터 변수,262보고 용
	int m_nSubGoodAlignCount; //2017-07-07,SKCHO, 이전 GOODALIGNCOUNT변수 대체로 사용

	int m_nTrayVICount; //2017.10.26 JSJUNG. 트레이 비전 Retry Count
	BOOL m_bTrayVINGRetry;
	
	BOOL m_bTrayVIResultRetry;  //2017-12-09,SKCHO, 트레이 결과 RETRY
	BOOL m_bTrayVICompRetry;  //2017-12-09,SKCHO, 트레이 comp RETRY

	BOOL m_bTrayVIToRBRetry;  //2018-01-22,JSJUNG, 비전프로그램 로봇 통신 끊길때 리트라이 추가


	int GetGoodAlignCount() { return m_nGoodAlignCount; }
	void ResetGoodAlignCount() {m_nGoodAlignCount = 0;}
	void MinusGoodAlignCount();

	int GetSubGoodAlignCount() { return m_nSubGoodAlignCount; }
	void ResetSubGoodAlignCount() {m_nSubGoodAlignCount = 0;}
	void MinusSubGoodAlignCount();
	void SetSubGoodAlignCount(int nCount) {m_nSubGoodAlignCount = nCount;}

	structSUBCARRINFO     m_structSubCarrierID[MAX_STRUCT];

	void ResetSubCarrierID();

	BOOL m_bCarrierAssign;
	BOOL m_bTrayCloseRelease;  //2017-03-25, skcho, 뚜껑덮기 보고용

	BOOL m_bCarrierReportWaitCheck;  //2017-03-25, skcho, lot end 에서 배출이 끝나면  열기 뚜껑보고하도록

	CString m_strS3F115_CarrierID;   //2017-04-14,skcho, CARRIER INFORMATION 응답 S3F115
	CString m_strS3F115_ReplyCode;   //2017-04-14,skcho, CARRIER INFORMATION 응답 S3F115 , PASS/ FIAL/TIMEOUT 10초
	CString m_strTrayOpenRelease;

	BOOL m_bTeachMoveComplete;
	AXIS_ID m_nPresentAxis;

	int m_nGoodOutLiftCount;   //2017-04-20, skcho, goodoutlift tray 개수를 카운트
	int GetGoodOutLiftCount() { return m_nGoodOutLiftCount; }
	void ResetGoodOutLiftCount(){m_nGoodOutLiftCount = 0;}

	int m_nPre_GoodOutLiftCount;   //2017-04-20, skcho, goodoutlift tray 개수를 카운트
	int GetPre_GoodOutLiftCount() { return m_nPre_GoodOutLiftCount; }
	void ResetPre_GoodOutLiftCount(){m_nPre_GoodOutLiftCount = 0;}

	BOOL m_bTrayCellFullChk;
	// MCR 재연결 체크 변수. 17/04/25 HSK
	BOOL ManualMCRReConnect;

	// Vision Ch 연속 불량 체크 변수.
	int m_nVisionNGCnt[JIG_ID_MAX][JIG_CH_MAX];
	BOOL m_bVisionNG[JIG_ID_MAX][JIG_CH_MAX];

	// Ch Skip 
	BOOL m_bChSkip[8];
	int m_nChSkipCount;
	JIG_CH m_nChSecond;
	JIG_ID m_nChSkipJigID;

	BOOL m_bLightAlarmChk;
	BOOL m_bReleaseTrayChk;

	int m_nLotCellCnt;

	BOOL m_bShowCh1TrackInChk;
	BOOL m_bShowCh2TrackInChk;

	// Setup 탭을 들어갔는지 확인 하는 변수 2017/06/07 HSK.
	BOOL m_bMotorSetupChk;

	// Setup 탭에서 모터 원점 잡는 도중에 모터 열림 체크 변수 2017/06/08 HSK.
	BOOL m_bOriginAxisChk;

	// 상위로 부터 받은 Recipe 이름;
	CString m_strMESRecipeName;

	SYSTEMTIME		m_CurDate;
	
	CString			m_szCurrent_time;
	//2018-03-14, NAM
	SYSTEMTIME      m_CurDate1;
	CString			m_szCurrent_time1;
	SYSTEMTIME      m_CurDate2;
	CString         m_szCurrent_time2;
	SYSTEMTIME      m_CurDate3;
	CString         m_szCurrent_time3;
	void InitTime();
	void InitTime1();
	void InitTime2();
	void InitTime3();



	//2017-07-04,SKCHO
	CString m_strLoadAGVTC;     //AGV 요청 상태 표시 문자열
	CString m_strUnLoadAGVTC;

	SYSTEMTIME	m_AGVLoadReqtime;
	SYSTEMTIME	m_AGVUnLoadReqtime;

	int m_nStatePortType;

	IDLE_DELAY_INFOMATION IDLE_Info;
	IDLE_DELAY_INFOMATION AGVINIDLE_Info;
	IDLE_DELAY_INFOMATION AGVOUTIDLE_Info;

	//20171108 JSJUNG, 트레이 IDLE TIME 시작 신호
	BOOL m_bTRAYInIDLE;
	BOOL m_bTRAYOutIDLE;

	BOOL m_bTRAYInChk;
	BOOL m_bTRAYOutChk;

	BOOL m_bOLBInterLockSkip;  //2017-07-20,SKCHO

	//20170730 JSJUNG, 지그별 컨택 NG count/3회 지정
	int m_JigContactNGcount[4][2];
	int m_JigContactNGcountMAX;
	int m_JigContactNGChName;
	BOOL m_bJigContactNGFlag;

	BOOL m_bTrayInStart;
	BOOL GoodTrayInLiftingFlag;

	BOOL m_bTrayOutStart;
	BOOL GoodTrayOutAGVFlag;

	//2017-08-22 KAKAKA
	int m_nAGVDelayStep;
	int m_nOutAGVDelayStep;

	//2017-09-12,SKCHO, Good port에 트레이 있는지 확인
	int m_nGoodInPortEmptyStep;
	//2017-09-12,SKCHO, NG port에 트레이 있는지 확인
	int m_nNGInPortEmptyStep;

	int m_nPACH1RetryStep;
	int m_nPACH2RetryStep;

	int m_nCH1LowerMCRFailCnt;
	int m_nCH2LowerMCRFailCnt;

	int m_nCH1UpperMCRFailCnt;
	int m_nCH2UpperMCRFailCnt;

	int GetMCRFail();

	//2017-09-19,SKCHO
	int m_nCH1CellInfoRetryStep;
	int m_nCH2CellInfoRetryStep;

	int m_nCH1JobProcessRetryStep;
	int m_nCH2JobProcessRetryStep;

	int m_nULDTRRetryStep;

	//2017-09-21,SKCHO  버큠 알람시 GET 위치값 더 내려 다시 집는다
	int m_nGoodTrayRetryStep;
	int m_nGoodTrayRetryStep2;
	int m_nNGTrayRetryStep;
	int m_nLDRobotRetryStep;
	CTime m_tmPGMStart;
	BOOL m_bPreAlignOrgStart;

	//2017-10-06,SKCHO
	BOOL m_bInPutIO[MAX_IN_ID];
	BOOL m_bOutPutIO[MAX_OUT_ID];

	void CheckUnitInterlock(ZONE_ID zone);
	void ChangeToJigNum(int nChannel);
	BOOL m_bUnitInterlock_LoadingStop;   //UNIT INTERLOCK LOADING STOP 상황이면 TRUE로 앞단 설비의 CELL을 받지 않음
	
	int m_nGoodInPortLAGuidRetryStep;
    int m_nPATRUpperGetRetryStep;
	int m_nPATRLowerGetRetryStep;

	void InitUnitInterlock();  //2017-12-05,SKCHO, 프로그램 시작시 인터락 초기화 메시지
	int m_nSkipChannel;  //514보고용
	BOOL m_bAllUnitInterlock_Stop;   //unit lnterlock으로 전체 skip일 경우 loader부 셀을 지그에 놓지 않고 설비 인터락 발생



	//2018-03-07,SKCHO ,추가
	CString m_strEQ_ID;	
	CString m_strProductID;
	CString m_strPPID;
	
	//2018-03-12,MGYUN, 상류설비 IN 딜레이, AGV IN,OUT 딜레이 시간 카운트 변수
	int m_InDelayTime;
	int nAGV_InDelayTime;
	int nAGV_OutDelayTime;

	//2018-03-13,MGYUN, 추가, CELL MELSEC 기록 시 TACT TIME 기록 위하여
	CString m_strLoadTack;
	CString m_strUnLoadTack;
	CString m_strAZoneTack;
	CString m_strBZoneTack;
	CString m_strCZoneTack;
	CString m_strDZoneTack;


    BOOL m_bFaultOfTray[4];  //2018-04-20,SKCHO TRAY로 인한 알람 발생시 TRUE해주고 TRAY가 들어오면 FALSE한다.
	void ResetTrayAlarm(int nIndex);
    //2018-06-09,SKCHO
	BOOL GetTrayAlarmChk();
	BOOL GetOtherAlarmChk();

	//2018-05-22,MGYUN,추가
	BOOL HybridDelayFlag;
	//2018-06-21,NAM
	BOOL m_RestartState;

	//2018-06-29,NAM
	BOOL m_newstateofHybrid;
	BOOL m_oldstateofHybrid;


	//20180wjp
	BOOL  m_bInPick2Model;
	BOOL m_bPickOUT2INTrayRequest;
	BOOL m_bStageSeneorSingle;
	int  i_ModelChange1To2;
	int  i_ModelChange2To1;
	BOOL m_bIsRestartDone;
	BOOL m_bTest;
	BOOL m_bInitModelChange;
	CString strCIMLoadCellID[CELL_ID_ROW_MAX][CELL_ID_COL_MAX];
	BOOL LoadCellIDFromLocal();
	BOOL m_bGetCellIDComplate;
	BOOL m_bModel2FullTray;
	//duong
	BOOL Test_duong;
};

