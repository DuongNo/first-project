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


// ALARM ���� [11/16/2016 WHLEE]
struct ALARM_INFOMATION
{
	ALARM_ID	iERR_ID;
	CString     szERR_ID;   // 2017-03-23,skcho ���Ͷ� ID�� �ޱ� ���ؼ� ���ڿ� �߰�
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

// IDLE ���� [2017/7/20 JSJUNG]
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


// ���� �����߿� ���Ǵ� �ֹ߼� Data�� �����ϴ� Class
class CProcDataBank
{
public:
	CProcDataBank(void);
	~CProcDataBank(void);

	//////////////////////////////////////////////////////////////////////////
	// ���� ����
	CString m_strOperationVersion;
	BOOL m_bProgramInitFinish;
	CONST_MACHINE_STATE::ID m_MachineState;
	CONST_MACHINE_STATE::ID m_OldMachineState;   //2018-04-20,SKCHO, ���� ��� ���� ���� ���� �߰�
	void ChangeMachineState(CONST_MACHINE_STATE::ID newState);
	CONST_MACHINE_STATE::ID GetMachineState() { return m_MachineState;};
	//////////////////////////////////////////////////////////////////////////
	// CIM ����
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

	//2017-11-01,SKCHO, INTERLOCK2 ���� �߰�
	CString m_strUnitInterlockID;
	CString m_strOldUnitInterlockID;
	CString m_strUnitInterlockMsg[10];
	int		m_MsgUnitInterlockCnt;
	CString m_strUnitModuleID;
	unsigned char     m_cUnitModuleNum;

	CString m_strDataPCVersion;

	CString m_strRCMD;
	// RCMD ���̵� �˶� �߻� �Ŀ� Ʈ���� ��õ��� ���ϰ� RCMD üũ�� �� �� 1�� ü�ο��� 2�� ID�� üũ�ϴ� ��찡 �־� �и�. 2017/06/08 HSK.
	CString m_strRCMDCellIDCh1;
	CString m_strRCMDCellIDCh2;
	// RCMD ���̵� ���� 1�� ü������ 2�� ü������ üũ �ϴ� ���� 2017/06/08 HSK.
	BOOL m_bRCMDCh1Chk;
	BOOL m_bRCMDCh2Chk;

	void SetOPCall();
	void SetInterlock();
	void SetInterlock2();   //2017-11-01,SKCHO ���Ͷ� �߰�
	void SetTerminalMsg();	
	//////////////////////////////////////////////////////////////////////////
	// Alarm ����
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
	ALARM_INFOMATION	alarm_info[ALARM_MAX];  //2017-04-15,skcho, ���߾˶��߻��� �α׿� ��Ȯ�� �ð� ����ϱ� ���ؼ� �迭�� ����

	ALARM_INFOMATION	alarm_info_op[OPCALLMAX];  
	ALARM_INFOMATION	alarm_info_inter[INTERLOCKMAX];  

	map<int, DEFECT_MES_CODE> iMapDefectList;


	ALARM_ID m_AlarmID_List[100];//2017-03-06,skcho, �߻��� �˶��� ����Ʈ�� �����Ѵ�
	int m_nAlarmCnt;//2017-03-06,skcho, �߻��� �˶��� ����Ʈ�� �����Ѵ�
	//////////////////////////////////////////////////////////////////////////
	// �� �� Run�� �ֹ߼����� �����Ǵ� ������

	CTrayBank m_TrayBank;

	void ResetTemporarilyValue();		// Lot End �� ���� Ү Cell�� ���� �����ϰ� ó������ �ٽ� ���� �� �̰ŷ� ������ �� �����Ѵ�
	void ResetAlignData();	

	int m_nCellCntX_Tray[TRAY_PORT_MAX];			// ���� Tray �ȿ� ����ִ� Cell ����
	int m_nCellCntY_Tray[TRAY_PORT_MAX];			// ���� Tray �ȿ� ����ִ� Cell ����
	BOOL m_bExistTray_TrayTr[TRAY_PORT_MAX];		// Tray TR�� Tray�� ���� �ִ���...
	//////////////////////////////////////////////////////////////////////////
	///////INNER ID �߹� ����
	int m_nIDGenIndex;
	//////////////////////////////////////////////////////////////////////////
	//Inspection Station Part
	//////////////////////////////////////////////////////////////////////////
	BOOL m_bTableTurnStatus[ZONE_ID_MAX];   //�� ������ step end �̸� �÷��׸� TRUE�� ����� ���̺� ���� �Ϸ�Ǹ� FALSE�� ����
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
	CString m_strLAResX;	// ALIGN_RES X��
	CString m_strLAResY;	// ALIGN_RES Y��
	CString m_strLAResT;	// ALIGN_RES Theta��

	//2017-05-23,skcho, mcr data ���庯�� ä�κ� ����
	CString m_strCh1LastCellID;
	CString m_strCh2LastCellID;
	int m_nMCRChannel;

	CString m_strTrayID;
	CString m_strCloseTrayID;	//���� �Ѳ� ID  
	//2017-07-07,SKCHO
	CString m_strClose352TrayID;	//352����� Ʈ���� ID   // OUTBUFFER ��Ʈ�� Ʈ���̰� �ְ�, LOTEND���� ��� ������Ʈ���� Ʈ���� ����� 352,353 Ʈ���� ID�� �ߺ��Ǵ� ����
	CString m_strClose353TrayID;	//353����� Ʈ���� ID  

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


	// LossTime ����
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
	// ��Ʈ ����.
	// Tray ��û �ߴ��� ���� �÷���.
	BOOL m_bGoodTrayReq;
	BOOL m_bNGTrayReq;

	// Manual�� Ʈ���� ��û �ߴ��� ���� �÷���
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

	// �� ������ Lower Shuttle�� Cell�� �Ѱ��� ���̰� Upper Shuttle�� �������� �Ѵ�. Inner ID ������ �ʵ��� �ϱ� ����.
	BOOL m_bLowerPutCompInterlock;

	// �� ������ �ش� Shuttle�� Load�� �������� Ȯ�� �Ѵ�. Shuttle�� Cell�� �Ѱ��� ���� ��츦 ���� �߰�����. 
	BOOL m_bLowerPutComp;
	BOOL m_bUpperPutComp;

	// DCR ������ ���� ����
	BOOL m_bDCRChk;
	// TRAY ALIGN ������ ���� ����
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
	// MUTE ����ġ ���� �� ��� ������
	BOOL m_bLDInMuteSWIn;
	BOOL m_bLDOutMuteSWIn;
	BOOL m_bNGMuteSWIn;
	// Job End ��ɿ� Ÿ�̸�
	CStopWatch m_timerJobEnd;
	
	// D Zone TeactTime Test
	BOOL m_bDZoneTT;
	DWORD m_dwDZoneTT1;
	DWORD m_dwDZoneTT2;

	// Load Conv Timer
	CStopWatch m_timerLoadConv;
	
	// Upper MCR ���� ���� �Ǵ� ����.
	BOOL m_bUpperMCRCompChk;

	CString m_strTrayQty;
	CString m_strCellQty;
	CString m_strReplyCode;

	int m_nGoodAlignCount;  //2017-03-21,skcho, GOOD ALIGN TRAY ī���� ����,262���� ��
	int m_nSubGoodAlignCount; //2017-07-07,SKCHO, ���� GOODALIGNCOUNT���� ��ü�� ���

	int m_nTrayVICount; //2017.10.26 JSJUNG. Ʈ���� ���� Retry Count
	BOOL m_bTrayVINGRetry;
	
	BOOL m_bTrayVIResultRetry;  //2017-12-09,SKCHO, Ʈ���� ��� RETRY
	BOOL m_bTrayVICompRetry;  //2017-12-09,SKCHO, Ʈ���� comp RETRY

	BOOL m_bTrayVIToRBRetry;  //2018-01-22,JSJUNG, �������α׷� �κ� ��� ���涧 ��Ʈ���� �߰�


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
	BOOL m_bTrayCloseRelease;  //2017-03-25, skcho, �Ѳ����� �����

	BOOL m_bCarrierReportWaitCheck;  //2017-03-25, skcho, lot end ���� ������ ������  ���� �Ѳ������ϵ���

	CString m_strS3F115_CarrierID;   //2017-04-14,skcho, CARRIER INFORMATION ���� S3F115
	CString m_strS3F115_ReplyCode;   //2017-04-14,skcho, CARRIER INFORMATION ���� S3F115 , PASS/ FIAL/TIMEOUT 10��
	CString m_strTrayOpenRelease;

	BOOL m_bTeachMoveComplete;
	AXIS_ID m_nPresentAxis;

	int m_nGoodOutLiftCount;   //2017-04-20, skcho, goodoutlift tray ������ ī��Ʈ
	int GetGoodOutLiftCount() { return m_nGoodOutLiftCount; }
	void ResetGoodOutLiftCount(){m_nGoodOutLiftCount = 0;}

	int m_nPre_GoodOutLiftCount;   //2017-04-20, skcho, goodoutlift tray ������ ī��Ʈ
	int GetPre_GoodOutLiftCount() { return m_nPre_GoodOutLiftCount; }
	void ResetPre_GoodOutLiftCount(){m_nPre_GoodOutLiftCount = 0;}

	BOOL m_bTrayCellFullChk;
	// MCR �翬�� üũ ����. 17/04/25 HSK
	BOOL ManualMCRReConnect;

	// Vision Ch ���� �ҷ� üũ ����.
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

	// Setup ���� ������ Ȯ�� �ϴ� ���� 2017/06/07 HSK.
	BOOL m_bMotorSetupChk;

	// Setup �ǿ��� ���� ���� ��� ���߿� ���� ���� üũ ���� 2017/06/08 HSK.
	BOOL m_bOriginAxisChk;

	// ������ ���� ���� Recipe �̸�;
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
	CString m_strLoadAGVTC;     //AGV ��û ���� ǥ�� ���ڿ�
	CString m_strUnLoadAGVTC;

	SYSTEMTIME	m_AGVLoadReqtime;
	SYSTEMTIME	m_AGVUnLoadReqtime;

	int m_nStatePortType;

	IDLE_DELAY_INFOMATION IDLE_Info;
	IDLE_DELAY_INFOMATION AGVINIDLE_Info;
	IDLE_DELAY_INFOMATION AGVOUTIDLE_Info;

	//20171108 JSJUNG, Ʈ���� IDLE TIME ���� ��ȣ
	BOOL m_bTRAYInIDLE;
	BOOL m_bTRAYOutIDLE;

	BOOL m_bTRAYInChk;
	BOOL m_bTRAYOutChk;

	BOOL m_bOLBInterLockSkip;  //2017-07-20,SKCHO

	//20170730 JSJUNG, ���׺� ���� NG count/3ȸ ����
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

	//2017-09-12,SKCHO, Good port�� Ʈ���� �ִ��� Ȯ��
	int m_nGoodInPortEmptyStep;
	//2017-09-12,SKCHO, NG port�� Ʈ���� �ִ��� Ȯ��
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

	//2017-09-21,SKCHO  ��Ũ �˶��� GET ��ġ�� �� ���� �ٽ� ���´�
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
	BOOL m_bUnitInterlock_LoadingStop;   //UNIT INTERLOCK LOADING STOP ��Ȳ�̸� TRUE�� �մ� ������ CELL�� ���� ����
	
	int m_nGoodInPortLAGuidRetryStep;
    int m_nPATRUpperGetRetryStep;
	int m_nPATRLowerGetRetryStep;

	void InitUnitInterlock();  //2017-12-05,SKCHO, ���α׷� ���۽� ���Ͷ� �ʱ�ȭ �޽���
	int m_nSkipChannel;  //514�����
	BOOL m_bAllUnitInterlock_Stop;   //unit lnterlock���� ��ü skip�� ��� loader�� ���� ���׿� ���� �ʰ� ���� ���Ͷ� �߻�



	//2018-03-07,SKCHO ,�߰�
	CString m_strEQ_ID;	
	CString m_strProductID;
	CString m_strPPID;
	
	//2018-03-12,MGYUN, ������� IN ������, AGV IN,OUT ������ �ð� ī��Ʈ ����
	int m_InDelayTime;
	int nAGV_InDelayTime;
	int nAGV_OutDelayTime;

	//2018-03-13,MGYUN, �߰�, CELL MELSEC ��� �� TACT TIME ��� ���Ͽ�
	CString m_strLoadTack;
	CString m_strUnLoadTack;
	CString m_strAZoneTack;
	CString m_strBZoneTack;
	CString m_strCZoneTack;
	CString m_strDZoneTack;


    BOOL m_bFaultOfTray[4];  //2018-04-20,SKCHO TRAY�� ���� �˶� �߻��� TRUE���ְ� TRAY�� ������ FALSE�Ѵ�.
	void ResetTrayAlarm(int nIndex);
    //2018-06-09,SKCHO
	BOOL GetTrayAlarmChk();
	BOOL GetOtherAlarmChk();

	//2018-05-22,MGYUN,�߰�
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

