////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// MCMain : 자동 제어에 중심이되는 Class로 각종 Station과 Service/System들을 선언하고 관리한다.
//
//
// #2014-05-27, jhLee : 원형 작성 시작
// #2014-11-19, jhLee : 안정화를 위한 최적화, 비가동log/Cell log 최적화 
// #2014-12-27, jhLee : _VER_MOTION_NMC2_ 조건 처리 시작, 기존 MP2100 및 NMC1과 새로운 NMC2와 배타적으로 사용
// #2015-07-26, jhLee : Gaus2 적용에따른 기존 모듈 변환, 미사용 코드 삭제작업
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once


#include "GxSingleton.h"
#include "GxCallback.h"
#include "GxGausFrame.h"
#include "File\GxLogData.h"			// Cell Log를 위한 class

#include "MCCode\MCDefine.h"
#include "CellInfo.h"			// Cell 정보를 담아두는 class
#include "JigInfo.h"			// Jig 및 Channel 정보를 관리하는 Class
#include "StatsBlock.h"			// 각종 통계 데이터 관리


#include "Motion\GxMotionCmdMgr.h"
#include "Motion\GxAxisMgr.h"
#include "Motion\GxRobotMgr.h"

#include "Comm\GxSocketMgr.h"

//d // for CIM
//d #include "..\Interface\CommonDefine.h"


// 설비 실행에 필요한 각종 데이터들을 관리한다. 
// Profile file에 저장/읽어오기
typedef struct _TProfileDatas {
	CString		sEQID;							// 설비 ID
	CString		sModelName;						// 생산 모델명
	CString		sVersion;						// Program version : Y_A_MT_14.05.17.01
	CString		sPPID;							// PPID

	// Cell-Log 관련
	CString		sCellLog_Path;					// Cell log를 남기는 기본 폴더,
	CString		sCellLog_LineID;				// MINS에서 사용되는 라인내 설비 ID, 3자리, Cell-Log 파일이름에 사용된다.
	UINT			nCellLog_Hour;					// Cell Log 파일 생성에 기준이되는 하루의 시작 시간 00 ~ 23
	UINT			nCellLog_Min;					// Cell Log 파일 생성에 기준이된느 하루의 사직 분 00 ~ 59

	// CIM 관련
	CString		sDataPCVersion;				// Data-PC Version, Cell-Log에 필요한것으로, Data-PC와 통신시 내려받는다. CDlgSock 에서 갱신
	int			nLastReportState;				// CIM으로 보고한 최종 상태, (중복된 상태는 보고를 하지 않기 위함, MainFrm에서 사용)
	CString		sLastReportState;				// -> 문자열 (MainFrm에서 갱신해준다.), AutoForm에서 화면에 표시해주기 위해 사용
	


	// 각 Jig의 채널의 상태 (Profile에서 관리)
	BOOL		bChannelNotUseFlag[eMAX_JigCount][eMAX_ChCount];		// 각 채널의 사용/미사용 설정 상태, Profile에서 관리
	BOOL		bPGContactFlag[eMAX_JigCount][eMAX_ChCount];				// 각 채널의 Contact 여부 상태, 실시간 관리


	// 정면 버튼들의 동작을 활성화/비활성화 제어한다.
	BOOL		bPanelButtonLock;

	// D-Zone 진입으로 작업자의 Defect 입력하는 모드이다.
	BOOL		bDZoneDefectInputFlag;		

	// A-Zone 초기 Loading 작업시 Cell 없음/미사용 설정용 Dialog 가능/불가능 설정
	BOOL		bAZoneCellInfoEnabled;

	// 초기화를 마친 후 최초로 START를 눌렀는가 ?
	BOOL		bInitialFisrtStart;

	//2014-12-18, jhLee, 상위로 설비상태 보고시 Interlock/Alarm을 해제하더라도 Auto로 전환되기 전까지는 현 상태 유지해야 함
	BOOL		bCIMInterlockFlag;		// TRUE, STOP 상태로 전화되더라도 상태보고하지 않음, FALSE : 그대로 상태보고


	//2014-09-16, jhLee, (백윤기S) TACT Time 표시 개선
	// TACT Time 측정시 STOP -> AUTO 전환 할 때 PG-OFF 시간을 현재 'START'한 시간으로 갱신할것인가 ?
	BOOL		bTACTStartTimeUpdateFlag;	// TACT 측정 START시간 갱신 가능한지 여부
	BOOL		bOperatorLoadingWorkFlag;	// 작업자 로딩 작업 진행중인지 여부, 이 때에만 STOP -> AUTO 변경시 현재 시각 적용이 가능하다.



	// A/D-Zone에서 작업자 Defect 입력시 상세 Defect list를 보여줄 때 어느 Zone에서 어느 Jig와 Channel에 해당되는 작업인지 지정한다.
	// CDlgDefectSelect 에서 화면 처리할 때 참조하여 동작한다.
	// 2015-02-28, jhLee, FormSetup7 설정 화면에서도 사용하도록 변경
	int			nDefectInputJig;
	int			nDefectInputCh;
	int			nDefectInputZone;				// Defect입력을 요청한 Zone
	int			nDefectInputOwner;				// 입력된 Defect을 필요로 하는 대상, Form Index
	CString		sDefectInputValue;				// 입력/선택된 Delect에 대한 정보


	// 각종 검사 항목 실행 여부 (option으로 처리)
	BOOL		bUseTest_VI;				// C-Zone에서 AV검사를 진행할 것인지 여부,
	BOOL		bUseTest_VIForce;			// A-Zone에서 Defect 발생시에도 C-Zone에서 AV검사를 진행할 것인지 여부,
	BOOL		bUseTest_Manual;			// D-Zone 작업자 검사를 진행할 것인가 ?
	BOOL		bUseTest_TSP;				// 2016-03-15, C-Zone에서 TSP 검사를 진행할 것인지 여부
	BOOL		bUseTest_IDCheck;			// (미사용) IDCHECK 검사를 진행할 것인가 ? (현재 미사용)
	BOOL		bUseTest_Sleep;				// (미사용) Sleep Current검사를 진행할 것인가 ?
	BOOL		bUseTest_White;				// (미사용) White Current 검사를 진행할 것인가 ?
	BOOL		bUseTest_BIN2TestMode;		// 2015-03-06, jhLee, 김상묵 사원요청, TEST Cell 작업 모드, 별도의 Cell log로 남기고 상위 보고를 하지 않는다.
//d	BOOL		bUseTest_MultiGrab;			// 2015-03-11, jhLee, 김상묵 사원요청, CH_1 Grab.OK 응답 받은 후 바로 CH_2의 Grab.Start를 보낼 것인가 ? 
	BOOL		bUseTest_OpenCellMode;		// 2015-03-12, jhLee, 김상묵 사원요청, Open Cell mode인지 여부, 기본은 FALSE(Normal Cell)
	BOOL		bUseTest_TECheck;			// 2015-03-28, jhLee, 김상묵 사원요청, C-Zone에서 TE_CHECK 를 진행한다.
	BOOL		bUseTest_ModelCheck;		// 2015-05-21, JH-PARK 검정유D, 김상묵S 요청 , Cell Model 투입시 현재 Model 구동여부를 판단한다;
	//2015-09-16, jhLee 미사용 삭제, BOOL		bUseTest_CheckTracking;		// Cell의 Tracking 정보를 확인하여 0 값이 아니면 투입을 막을 것인가 ? 0:무시, 1:투입방지, #2015-05-26, jhLee, 김영락 사원요청



	// 각종 실행 항목 Option
	BOOL		bUse_CircleDisp;			// D-Zone Test시 VI Defect을 원으로 그려서 보여줄 것인가 ?
	BOOL		bUse_LoopTest;				// 무한루프 테스트 일명 뺑뺑이
	BOOL		bUse_LabelPrinter;			// Label Printer를 사용할 것인가 ?
	BOOL		bUse_KeepVIResult;			// VI Result file을 유지할 것인가 ?
	BOOL		bUse_ContactCurrentCheck;	// 2014-11-25, jhLee, Contact Current NG를 체크하여 Loading을 Cancel시킬 것인가 ?
	BOOL		bUse_OPLosstimeAlert;		// 2014-12-10, jhLee, 작업자 작업시간초과 발생시 경보를 울릴것인가 ?
	BOOL		bUse_CellExcept;			// 2014-12-22, jhLee, 특정 Cell-ID는 검사를 수행하지 않고 제외 시키도록 한다.
	BOOL		bUse_SkipJigTilt;			// JIG를 Tilt 하지 않고 사용할 것인가 ? (TACT 감소를 위해) - 기본값 TRUE

	BOOL		bUse_SkipMCR;					// MCR을 읽지 않고 가상의 Cell ID를 생성하여 사용할 것인가 ? - 기본값 FALSE
	BOOL		bUse_SaveMCRImage;			// 정상적을 읽은 MCR 이미지도 저장할 것인가 ? NG 이미지는 반드시 저장, OK 이미지는 선택적
	BOOL		bUse_GMSCheck;					// GMS (접지모니터링) 기능을 사용할 것인가 ?
	BOOL		bUse_RTracking;				// LOSS도 R로 보고 할것인가 ?


	//d BOOL		bUse_SkipCIMSend;			// 상위(CIM) 보고 하지않음 (테스트 제품 검사시) - 기본값 FALSE
	BOOL		bUse_SkipEngineerTest;		// 2014-11-07, 엔지니어 로그인으로도 검사를 수행할 수 있도록 한다 - 기본값 FALSE.(김길성 책임 요청 회피용)
	BOOL		bUse_SkipCommFail;			// 2014-12-10, 통신 접속이 끊어져도 무시할 것인가 ? TRUE:무시, FALSE:해당 통신의 Alarm을 발생시켜준다.
	BOOL		bUse_AutoMoveRetry;			// 2015-03-10, Hide, 모션 이동관련 에러 발생시 자동으로 재시도를 수행하게 한다. 주로 NOT Inpos 알람에 대응하기 위함.
	BOOL		bUse_CellLoadingCmd;		// 2015-03-28, TACT을 줄이기위해 PowerOn 버튼으로 기존 POWER_ON을 대신하여 Cell Loading 명령을 사용할 것인가 ? TRUE:CellLoading명령 사용, FALSE:기존대로 POWER_ON 사용
	//d BOOL		bUse_CZoneReset;			// 2015-03-30, 2개 채널 제품 모두 양품이고, bUse_SkipDefectForm 옵션이 활성화 되었을 경우, A-Zone으로 회전전에 Contact을 풀고 회전하여 작업 시간을 단축시키도록 한다.
	BOOL		bUse_LoginServerRequest;	// 2015-04-16, 작업자 Login/out시 상위서버에게 인증을 요청하도록 할 것인가 ?
	BOOL		bUse_JudgeCellExcept;		// 2015-06-21 JH-PARk 판정 완료된 Cell에 대하여 투입을 진행할 것인가? 
	BOOL		bUse_TMDVersionCheck;		// 2015-07-16, jhLee, 각 채널별 TMD Version을 관리할것인가 ?
	BOOL		bUse_UpperBackLight;		// 2015-12-26, jhLee, 상부 백라이트를 이용하여 Cell MCR 리딩시 조명으로 사용한다.
	BOOL		bUse_TMDModelCheck;			// 2015-10-15, jhLee, 신호기 TMD모델명과 Product ID와 비교하여 Alarm 처리할 것인가 (SDC 백윤기사원 요청)
	BOOL		bUse_SkipAutoShutdown;		// 2015-10-20, jhLee, 신호기 TMD모델명과 Product ID가 일치하지 않을경우 설비를 Down시키는 것을 비활성화 시킬것인가 ?
	BOOL		bUse_SkipAutoTrackout;		// 2015-10-20, jhLee, 설비가 DOWN될 때 Track-in된 Cell을 자동으로 RETEST로 Track-Out하는 기능을 수행하지 않게할 것인가 ?

	BOOL		bUse_AutoContactOff;		// 2016-05-26, 이름변경 bUse_SkipDefectForm;		// 2014-11-17, 사귀진 사원요청, 이전 검사가 모두 양품일 경우 별도의 Defect 입력화면을 띄우지 않고 PG-OFF 시킨다.
	BOOL		bUse_BIN2PatternTest;		// 2016-05-26, 메뉴얼 검사에 C-Zone 패턴을 이용한다. (이정현 선임)

	BOOL		bUse_DefectPosition;			// 2016-11-20, jhLee, Defect이 발생한 위치를 입력받는 기능을 수행할 것인가 ?
	BOOL		bUse_GMSFileData;				// 2016-11-20, jhLee, GMS Data를 file을 통해 취득한다.
	BOOL		bUse_BoxFanAlarm;				// 2016-11-25, jhLee, 각종 BOX에 부착된 FAN의 동작상태 Alarm 기능을 이용한다.

	// 정수형 설정 항목
	// 시간
	int			nTIME_CylinderTimeout;		// Cylinder 동작 Timeout 밀리초
	int			nTIME_VacuumTimeout;		// Vacuum 동작 Timeout 밀리초
	int			nTIME_UnloadVACOff;			// Unload 동작시 진공 파기 지연 시간
	int			nTIME_RunDownTime;			// 입력이 없을 시 RunDown으로 전환시키는 시간, 초 (60초? -> 변경가능)
	int			nTIME_OPLossTime;			// 작업자 손실시간 (OP Loss) 판단 기준 시간, 초, (18초? -> 변경可能)
	int			nTIME_EQLossTime;			// 설비동작 손실시간 (EQ Loss) 판단 기준 시간, 초, (18초? -> 변경可)
	int			nTIME_MDChangeTime;			// MD/BM 자동전환 기준 시간, 초 (60초? -> 변경가능)
	int			nTIME_VTOffsetTime;			// Vistion 검사결과 파일 대기 시간에서 추가로 더 대기 할 추가시간 (초)
	int			nTIME_TECheckTime;			// 2015-03-30, TE Check Timeout 기준 시각을 지정 (초)
	int			nTIME_LoginServerTimeout;	// 2015-04-16, jhLee, 작업자 Login/out 요청에 대한 서버의 응답시간 제한 (초)
	int			nTIME_MotorMoveCheckTime;	// 2015-06-21 JH-PARk, Motor 이동 완료 Retry 진행시 Delay 시간
	int			nTIME_DispResultCell;		// 2015-07-01 JH-PARK, 판정완료된 Cell의 값을 표시해주는 시간 (초)
	int			nTIME_GMSInterval;			// 2015-07-20, jhLee, MGS와 통신을 하는 주기, 10초 ~ 
	int			nTIME_TSPTimeout;			// 2016-04-21, TSP Timeout 기준 시각을 지정 (초)
	int			nTIME_DeepCheck;			// 2016-11-13, 심층검사 시간을 지정 (초)

	int			nTIME_VI_GrabEnd;			// Vision Grab End 회신 Timeout
	int			nTIME_VI_ClassEnd;		// Vision Class End 회신 Timeout


	// 횟수
	int			nCNT_DispCircleCount;		// D-Zone에서 Cell에 표시할 Defect circle 수량 (개)

	// 모드
	int			nMode_AlignImageSave;		// Align 이미지 저장 모드 0:None, 1:OK, 2:NG, 3:All
	int			nMode_UVMCRImageSave;		// UVMCR 이미지 저장 모드 0:None, 1:OK, 2:NG, 3:All
	int			nMode_WorkShift;			// 작업자 근무 표시방법 0:3개조 , 1:2개조
	int			nMode_DispVIResult;			// 검사결과 표시 방법     0: Message , 1 : Pattern, 안보이는 옵션

	int			nVALUE_PrinterLeft;		// 왼쪽 여백


	// 2015-03-23, jhLee, 각종 경로명을 변경할 수 있도록 옵션으로 관리한다.
	CString		sDIR_LossTime;			// #define NAME_DIR_LOSSTIME	_T("D:\\SIRIUS\\LOG\\MAIN")				// 비가동 Log 폴더
	CString		sDIR_CellLog;			// #define NAME_DIR_CELLLOG		_T("D:\\AUTO_TEST\\LOG")				// Cell Log 폴더
	CString		sDIR_MachineData;				// DIR_MachineData	

	CString		sDIR_RuntimeImage;		// 실행시 보여줄 화면 이미지 보관 경로, "D:\\D-AMT\\Data\\Image\\Runtime"
	CString		sDIR_TACTLog;			// #define NAME_DIR_TACTLOG		_T("D:\\Machine_Data\\TactTime_Data")	// TACT Time Log 폴더, 2014-10-14, 백윤기S 지정
	CString		sDIR_AlarmLog;			// #define NAME_DIR_ALARMLOG	_T("D:\\Machine_Data\\Alarm_Data")		// Alarm 발생 Log 폴더, jhLee
	CString		sDIR_DataPC_Cell;		// #define NAME_DIR_DATAPC_CELL	_T("D:\\UB-AMT\\DATA_PC\\CELL")			// Data PC로 전송 할 Cell 임시 정보 (Tack-out 정보)
	CString		sDIR_DataPC_FDC;		// #define NAME_DIR_DATAPC_FDC	_T("D:\\UB-AMT\\DATA_PC\\FDC")			// Data PC로 전송 할 FDC 실시간 정보 
	CString		sDIR_GMSLog;			// GMS로 부터 수신된 데이터 기록용 
	CString		sDIR_GMSDataFile;		// 파일을 통한 GMS 데이터 취득용 경로 및 파일 이름
	CString		sDIR_MCRImage;			// MCR로 부터 수신된 이미지 기록용 

	// 기존 일반 멤버변수에서 Profile로 이동
	CString		sDIR_VIResult;					// 검사PC에서 테스트 결과를 가져올 경로
	CString		sDIR_VIRecipe;					// 검사PC에게 변경된 레서피를 전달해줄 폴더
	CString		sDIR_VIInform;					// 검사PC에서 해상도관련 스펙을 가져올 경로

	CString		sDIR_ProductLog;				// 생산율 정보를 기록을 경로

	

	// Setup화면에 안보이는 항목 
	int			nVALUE_TempPCRack;			// PC랙 온도 이상 판단 기준 온도 


	//// 각종 설정상태 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	// TCP/IP 통신 설정, 각 접속 통신마다 설정값을 가진다.
	CString		sTCPName[eMAX_COMMCount];			// 통신 이름
	CString		sTCPSymbol[eMAX_COMMCount];			// 통신 상징 ID
	CString		sTCPAddress[eMAX_COMMCount];		// MCDefine.h에 정의
	UINT		nTCPPort[eMAX_COMMCount];

	UINT		nPrinterPort;						// Label Printer의 COM Port
	UINT		nVACPort;							// 진공 A/D 보드의 COM Port
	UINT		nPCTempPort;						// PC-RACK 온도의 COM Port
	UINT		nAlignLightPort;					// Align Back Light 컨트롤 COM Port
	UINT		nPanasonicPort;						// Panasonic Servo pack 연결을 위한 COM Port
	UINT		nYaskawaPort;						// Yaskawa DD Motor Tourque 수집을 위한 COM Port
	UINT		nGMSPort;							// 접지 저항을 재기위한 GMS 통신 COM Port


	// 예외 처리를 위한 설정값들 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	// 사고LOT 발생 Cell 검사 제외를 위한 설정값 (2015-07-14, jhLee)
	CString		sTroubleLOT_Value;				// MES에서 넘겨주는 Code값, 41
	CString		sTroubleLOT_Code;				// 불량 코드, USN03
	CString		sTroubleLOT_Name;				// 불량명, FAB HOLD

	// POCH 예외처리 
	CString		sExceptCell_POCH_Value;			// POCH 불량 값 : POCH
	CString		sExceptCell_POCH_Name;			// POCH 불량명, POCH
	CString		sExceptCell_POCH_Code;			// POCH 불량 코드, USF09


} TProfileDatas;	



struct AlarmInfoCode
{
	CString	m_strCode;
	
};


// 실행중 사용되는 각종 데이터들
typedef struct _TRuntimeDatas
{
	int		nAbnormalTerminate;				// 프로그램을 정상적으로 종료를 하였는가 ? 0:정상 종료, 1:비정상 종료
	BOOL		bSystemAbortRequest;				// 시스템을 ABORT 시켜서 초기상태로 만들라는 요청이 있었는가 ? 중대 Alarm이 발생한 경우 지정 (CSysSafety)

	// 서버 로그인 인증 관련
	BOOL		bLoginUIRequest;					// 작업자가 Login창에서 Login/out을 서버에 요청한 상태인가 ? 
	CString	sLoginUserID;						// 응답받은 Login ID
	CString	sLoginResponse;					// Login/out 결과
	BOOL		bIsDoorOpenLogin;					// PM 화면에서 Door open을 위한 Login인가 ?

	// 마지막에 진행한 작업자 정보를 기억하고 사용한다. (저장)
	CString	sLastUserID;						// 최종 접속된 작업자 정보
	CString	sLastUserPassword;				// 최종 접속된 작업자 암호
	int		nLogoutRequestCount;				// 서버에 로그아웃을 요청을 시도한 횟수, 일정 횟수를 넘어서면 수행을 포기한다.

	CString sDataPCVersion;						// 가온누리 버전

	// 각종 설정상태 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	BOOL		bStatePrevMachine;				// 이전 버전의 설비인가 ?				0:최신버전, 1:이전버전

	// 통신 - TCP/IP
	BOOL		bStateUVMCR1;					// UV-MCR #1이 정상인가 ?				0 :NotConnect, 1:정상
	BOOL		bStateUVMCR2;					// UV-MCR #2이 정상인가 ?				0 :NotConnect, 1:정상
	BOOL		bStateAVCamera;					// AVCamera 검사PC연결이 정상인가 ?		0:NotConnect, 1:정상
	BOOL		bStatePGPC;						// 신호기PC연결이 정상인가 ?			0:NotConnect, 1:정상
	BOOL		bStateDataPC;					// DataPC 연결이 정상인가 ?				0:NotConnect, 1:정상
	
	// 통신 - Serial
	BOOL		bStateLightCtrl;				// Align용 조명 제어기 통신				0:NotConnect, 1:정상
	BOOL		bStateVACCtrl;					// 진공 계측용 통신						0:NotConnect, 1:정상
	BOOL		bStateTempCtrl;					// PC온도 측정정용 통신					0:NotConnect, 1:정상
	BOOL		bStateTorqueCtrl1;				// DD Motor Torque 측정용 통신			0:NotConnect, 1:정상
	BOOL		bStateTorqueCtrl2;				// Servo Motor Torque 측정용 통신		0:NotConnect, 1:정상
	BOOL		bStateGMSCtrl;					// GMS 접지저항 계측용 통신				0:NotConnect, 1:정상
	

	// 장치 - Device (0:Close, 1:정상)
	BOOL		bStateAlignVision;				// Align용 Vision이 정상적인가 ?		0:Close, 1:정상
	BOOL		bStateMotionAxis;				// 모션 제어 시스템이 정상적인가 ?
	BOOL		bStateMotionMP2100;				// 모션제어 MP2100이 정상인가 ?			0:Close, 1:정상
	BOOL		bStateMotionNMC;				// 모션제어 NMC가 정상인가 ?			0:Close, 1:정상
	BOOL		bStateMotionCtrl;				// 모션 컨트롤러가 정상적인가 ?
	BOOL		bStateDIODeviceNet;				// I/O DeviceNET master가 정상인가 ?	0:Close, 1:정상

	// 데이터 상태 (0:비정상, 1:정상)
	BOOL		bStateLoadRecipe;				// 레서피 파일을 정상적으로 읽어왔는가 ?
	BOOL		bStateAlignTemplate;			// Align용 Template파일들을 정상적으로 읽었는가 /
	BOOL		bStateDeepCheck;				// 현재 Deep check 동작 모드인가 ?


	// 상위 연결 상태
	int			nControlState;					// CIM에서 보내온 Control-State, E_CRST_OFFLINE = 0,	E_CRST_REMOTE,	E_CRST_LOCAL
	CString		sControlState;					// -> 문자열로 보관
//d	BOOL		bForceCommFlag;					// 초기 통신 연결 시작시 다른 어떤 설정과는 상관없이 Data PC로 정보를 전송할 수 있게 예외처리한다.

	// 각종 시각 설정, AUTO form에서 보여준다.
	SYSTEMTIME	tmUserLogin;					// 작업자 Log in/out 시각
	SYSTEMTIME	tmEQStart;						// 설비 동작 시작 시각
	SYSTEMTIME	tmEQStop;						// 설비 중지 시각

	CString		sUserLoginTime;					// 파일 저장용 변수
	CString		sEQStartTime;
	CString		sEQStopTime;

	// 매 초마다 시간을 check하여 작업조 혹은 날짜가 변경되었는지 확인한다.
	//d CString		sPrevDay;						
	CTime		timePrevDay;					// 하루 기준 시각이 변했는지 점검용 시작과 종료시 저장관리한다.
	int		nPrevShift;						// 바로 직전의 작업 조

	// Light Curtain 동작상태 지정
	BOOL		bLCRequestState;				// 자동 제어에서 요청한 Light Curtain 동작 상태, TRUE:감지, FALSE:Mutting

	// 간편 Teaching 작업을 위한 C-Zone VI 검사직전 일시정지 설정 여부
	BOOL		bQuickTeachingMode;				// 자동 검사 순서에서 Z-Zone VI Grab.Start 전송 직전 시스템을 STOP 상태로 만든다. (Align된 상태)


	// MCR을 Handy Scanner로 직접 입력을 받기위한 내용들
	int			nMCRInputJig;						// 작업 대상
	int			nMCRInputCh;
	int			nMCRInputState;					// 입력 과정
	int			nMCRInputZone;						// 입력을 요청한 Zone
	CString		sMCRInputValue;					// 입력을 받아들인 값



	_TRuntimeDatas()							// 생성자
	{
		nAbnormalTerminate	= 0;				// 프로그램을 정상적으로 종료를 하였는가 ? 0:정상 종료, 1:비정상 종료
		bSystemAbortRequest	= FALSE;			// 시스템을 ABORT 시켜서 초기상태로 만들라는 요청이 있었는가 ? 중대 Alarm이 발생한 경우 지정 (CSysSafety)

		bLoginUIRequest		= FALSE;			// 작업자가 Login창에서 Login/out을 서버에 요청한 상태는 아니다. 
		bIsDoorOpenLogin		= FALSE;			// Door Open을 위한 Login 요청인가 ?
		sLoginResponse		= _T("");			// Login이 실패한 사유
		sLastUserID			= _T("");			// 최종 접속된 작업자 정보
		sLastUserPassword	= _T("");			// 최종 접속된 작업자 암호
		nLogoutRequestCount	= 0;				// 서버에 로그아웃을 요청을 시도한 횟수, 일정 횟수를 넘어서면 수행을 포기한다.

		sDataPCVersion		= _T("");						// 가온누리 버전

		bStateUVMCR1		= FALSE;			// UV-MCR #1이 정상인가 ?				0 :NotConnect, 1:정상
		bStateUVMCR2		= FALSE;			// UV-MCR #2이 정상인가 ?				0 :NotConnect, 1:정상
		bStateAVCamera		= FALSE;			// AVCamera 검사PC연결이 정상인가 ?		0:NotConnect, 1:정상
		bStatePGPC			= FALSE;			// 신호기PC연결이 정상인가 ?			0:NotConnect, 1:정상
		bStateDataPC		= FALSE;			// DataPC 연결이 정상인가 ?				0:NotConnect, 1:정상

		//// 통신 - Serial
		bStateLightCtrl		= FALSE;			// Align용 조명 제어기 통신				0:NotConnect, 1:정상
		bStateVACCtrl		= FALSE;			// 진공 계측용 통신						0:NotConnect, 1:정상
		bStateTempCtrl		= FALSE;			// PC온도 측정정용 통신					0:NotConnect, 1:정상
		bStateTorqueCtrl1	= FALSE;			// DD Motor Torque 측정용 통신			0:NotConnect, 1:정상
		bStateTorqueCtrl2	= FALSE;			// Servo Motor Torque 측정용 통신		0:NotConnect, 1:정상
		bStateGMSCtrl		= FALSE;			// GMS 접지저항 계측용 통신				0:NotConnect, 1:정상


		//// 장치 - Device (0:Close, 1:정상)
		bStateAlignVision	= FALSE;			// Align용 Vision이 정상적인가 ?		0:Close, 1:정상
		bStateMotionAxis	= FALSE;			// 모션 제어 시스템이 정상적인가 ?
		bStateMotionMP2100	= FALSE;			// 모션제어 MP2100이 정상인가 ?			0:Close, 1:정상
		bStateMotionNMC		= FALSE;			// 모션제어 NMC가 정상인가 ?			0:Close, 1:정상
		bStateMotionCtrl	= FALSE;			// 모든 모션 컨트롤러가 정상적인가 ?
		bStateDIODeviceNet	= FALSE;			// I/O DeviceNET master가 정상인가 ?	0:Close, 1:정상

		//// 데이터 상태 (0:비정상, 1:정상)
		bStateLoadRecipe	= FALSE;			// 레서피 파일을 정상적으로 읽어왔는가 ?
		bStateAlignTemplate	= FALSE;			// Align용 Template파일들을 정상적으로 읽었는가 /


		::GetLocalTime( &(tmUserLogin) );					// 작업자 Log in/out 시각
		::GetLocalTime( &(tmEQStart) );						// 설비 동작 시작 시각
		::GetLocalTime( &(tmEQStop) );						// 설비 중지 시각


		nControlState			= 0;				// CIM에서 보내온 Control-State, E_CRST_OFFLINE = 0,	E_CRST_REMOTE,	E_CRST_LOCAL
		sControlState			= _T("OFFLINE");	// -> 문자열로 보관

		// 각종 시간 저장용 변수 초기화
		sUserLoginTime		= _T("");				
		sEQStartTime		= _T("");
		sEQStopTime			= _T("");

		timePrevDay			= CTime::GetCurrentTime();	// 현재 시각
		nPrevShift			= 0;			// 기본적인 작업 조

		//// Light Curtain 동작상태 지정
		bLCRequestState		= TRUE;				// 자동 제어에서 요청한 Light Curtain 동작 상태, TRUE:감지, FALSE:Mutting

		bQuickTeachingMode	= FALSE;			// 간편 Teaching mode는 off


		nMCRInputJig		= 0;						// 작업 대상
		nMCRInputCh			= 0;
		nMCRInputState		= 0;					// 입력 과정
		nMCRInputZone		= 0;						// 입력을 요청한 Zone
		sMCRInputValue		= _T("");					// 입력을 받아들인 값


	};

} TRuntimeDatas;



// 
class CMCMain : public CGxSingleton <CMCMain>
{

private:
	//d static CMCMain* theMCMain;					// MCMain 지칭

	HWND	m_hOwner;							// 이 컨트롤을 소유한 주인의 핸들,

	int		m_nLanguageType;					// 설정된 언어 종류, 1:한국어, 2:영어, 3:베트남어 ...
	CString	m_sLangSuffix;						// 언어와 관련된 파일들의 접미사, KOR:한국어, 2:영어, 3:베트남어

	BOOL		m_bEnabled;								// Machine System을 동작할 수 있는 상태이가 ? 모든 초기화를 마쳤는가 ?
	int		m_nInitialFlag;					// 설비의 Initial 관계
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


public:
	CMCMain(void);
	~CMCMain(void);
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -

	// 설정 변수들
	CString	m_sBasePath;						// 근간이되는 경로 (Data 관리)
	// CString	m_sExePath;							// 실행파일 경로 (기초 설정값 읽기용)

	CString	m_sPath_Data;						// 설정값을 저장하는 data 폴더
	CString	m_sPath_INI;						// INI 설정값을 저장하는 폴더
	CString	m_sPath_Log;						// Log를 기록하는 폴더
	CString	m_sPath_Recipe;					// 생산모델의 Recipe가 있는 폴더, 모델이 바뀌면 함께 바뀐다.
	CString	m_sPath_UnitImg;					// 각 Unit들의 이미지가 존재하는 경로
	CString	m_sPath_Runtime;					// 동작시 생성되는 이미지 경로(Align, MCR 이미지)

	BOOL		m_bIsSimulate;						// Simulation mode 인가 ?
	BOOL		m_bIsDryRun;						// DryRun mode인가 ?
	BOOL		m_bCycleStop;						// Cycle stop 여부
	BOOL		m_bAlarmStop;						// Alarm 발생시 모든 축 정지 여부


	// AutoForm에 보여줄 Align 및 MCR 실행 이미지 파일이름
	CString	m_sAutoViewImg[eMAX_ChCount][eMAX_ViewImgCount];

	// Axis Map
	int m_nAxesMap[eMAX_AXISCount];				// 내부적으로 사용할 축 index 보관배열
	int GetLocalAxis(int nIdx);					// 내부적으로 사용하는 축 Index 조회 
	CString CheckLocalAxis();					// 내부적으로 사용하는 축 Index를 토대로 축 지정이 정상적인지 판정한다.
	int GetMotionBoardCheck(CString& sMsg);		// 모션보드의 상태를 점검하고 ErrorCode를 되돌려받는다. return : 0 정상

	// TACT Time을 기록하는 루틴, Cell Info에서 지정한다.
	void SetTACTTime(int nJig, int nIdx);		// 지정 JIG의 모든 채널에 해당 순번의 TACT TIme을 현재의 시간으로 지정한다.

	// 2014-11-23, jhLee, Display circle to Cell
	int m_nPTN_JigNo;						// 표시되고 있는 Jig 번호
	int m_nPTN_CurrentPage;					// 현재 표시되고 있는 page 번호
	int m_nPTN_MaxPage;						// Control 할 최대 Page = 결과(1) + Circle 표시(2) + 신호기 기본 페이지(5)
	int m_nPTN_PGPage;						// 실제 신호기에 표시되는 원래 신호기 패터 Page 번호
	int m_nPTN_CirclePage;					// Circle을 그려 줄 Page 수 (최대 2 ?)
	void SetPreparePatternDisplay(int nJig);	// 지정 Jig의 패턴을 그려주기 전에 처리 할 내용들
	void SetNextPatternDisplay(int nJig);		// 작업자가 Next 버튼을 눌렀을 경우 처리
	void SetBackPatternDisplay(int nJig);		// 작업자가 Back 버튼을 눌렀을 경우 처리

	// 지정 패턴의 실제 색 값을 가져온다.
	//d void GetPatternColor(CString sPtnName, COLORREF &colorBack, COLORREF &colorFore);

public:
	// static CMCMain* GetMCMain(void);			// SingleTone, Main의 주소 조회
	// static void ReleaseMCMain(void);			// SingleTone, Main 객체 파괴

	void ReleaseProcess(void);					// 종료 처리
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	// Recipe / Profile file
	CGxIni				m_iniRecipe;			// 모델별 Recipe file
	CGxIni				m_iniRecipeBack;		// 모델별 Recipe file 값 변경 감지용
	CGxIni				m_iniProfile;			// 실행에 근간이 되는 설정값 File
	CGxIni				m_iniOptions;			// 각종 실행 옵션 값 관리
	CGxIni				m_iniProduct;			// 생산량 정보 기록 
	CGxIni				m_iniLosstime;			// 비가동 정보를 임시 기억한다.
	CGxIni				m_iniRuntime;			// 실행 상태를 저장한다.


	//각종 구조체 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// 실행 Data 
	TProfileDatas		m_rProfileData;			// 실행에 필요한 각종 설정값들, profile m_iniProfile 로 관리
	TRuntimeDatas		m_rRuntimeData;			// 실행시 참조되는 각종 변수들

	// Defect 관련 데이터
	TDefectList			m_rDefectList;			// Defect에 대한 데이터를 관리한는 구조체

	// 수동검사시 Cell에 불량명을 출력해주기위한 좌표계산용 Recipe 항목
	TCellDisp			m_rCellDisp;
	TCellDisp			m_rCellDispBack;

	// 생산량 정보를 기록하고 관리하는 구조체
	TProductData		m_rProductData;		// 생산량 정보 (AUTO Form에 표시)


	// Alarm 조치항목 관련 데이터 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	TRepairAlarmList	m_rRepairAlarmList;
	TAlarmRecord		m_rAlarmRecord;							// Alarm 발생 내역을 Log 기록하기위한 구조체



	// Defect name table 매칭용 데이터 파일 읽기 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	CArray < TDefectNameTable*, TDefectNameTable* > m_arrDefectTalbe;		// Defect 검색 테이블
	CMapStringToDefectName		m_mapDefectTable;				// 작업자 관리용 map


	// 비가동 항목 관련 데이터 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	TLosstimeList		m_rLosstimeList;						// 비가동 항목과련 데이터를 관리하는 구조체
	TLosstimeRecord	m_rLosstimeRecord;							// 비가동 Log를 남기기 위한 데이터 기록

	// 상위에 전송하는 코드및 영문 저장 하기 위한 변수 추가 시작 20141226-2 jhkim1
	CString				m_sTPSystemCode;						// 코드 저장 하기 위한 변수
	CString				m_sTPSystemName;						// 네임 저장 하기 위한 변수

	void SaveTPSyetem(/*CString sCode, CString sName*/);		// TPSystem 저장하기 위한 함수 20141226-2 jhkim1
	BOOL LoadTPSystem();										// 저장된 TPSystem 불러오기 위한 함수 생성 20141226-2 jhkim1
	// 상위에 전송하는 코드및 영문 저장 하기 위한 변수 추가 끝 20141226-2 jhkim1

	int  m_nRunDownTimer;										// RunDown으로 전환을 위한 카운터
	int  GetRunDownTimer();										// 현재의 RunDown 경과 Timer를 조회한다.
	void ClearRunDownTimer();									// RunDown 타이머를 초기화 해준다.
	BOOL CheckRunDownTimer();									// RunDown을 실행할 것인지 점검해준다.
	void ApplyRunDown();										// RunDown을 실행한다.
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 


	// 생산 TACT Time을 관리하는 구조체와 관련 함수들  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	void SetPGOffTime(int nJig);								// 지정 지그의 PG-Off 시각을 지정한다.
	void SetPrevJigPGOffTime(int nJig);							// 이전 지그의 PG-OFF 시각을 새로운 시각으로 갱신한다.
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


	// 작업자 계정 관리용  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	CMapStringToUserAccount		m_mapUsers;						// 작업자 관리용 map
	CUserAccount				m_rCurrentUser;					// 현재 접속된 작업자 계정 정보

	BOOL LoadUserAccount();										// File에서 작업자 계정 정보를 읽어온다.
	void ReleaseUserAccount();									// 종료시 계정 정보를 메모리 해제해 준다.

	BOOL UserFind(CString sID);									// 지정 작업자를 검색한다.

	int GetUserLevel(CString sID, CString sPW);					// 지정 ID에 대한 Level을 되돌려준다.
	void ChangeCurrentUser(CString sID, int nLevel, BOOL bFixForm=FALSE);			// 현재 사용자를 변경한다.
	void UserLogout(BOOL bHide);								// Logout 시킨다.

	int GetCurrentUserLevel();									// 현재 접속된 사용의 Level
	CString GetCurrentUserID();									// 현재 접속된 사용자 ID
	CString GetCurrentUserIDNo();								// 현재 접속된 사용자 사번 조회
	CString GetCurrentUserName();								// 현재 접속된 사용자 이름 조회

	CString GetUserName(CString sID, int nLang);				// 지정 ID에 대한 이름을 되돌려준다.
	CString GetUserIDNo(CString sID);							// 지정 ID에 대한 사원번호를 되돌려준다.
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	// Gaus Frame 1개에 모두 포함시켰다.
	// CGxGausFrame*		m_pGausFrame;				// Gaus Lib의 내용을 사용하는 Frame

	//int CreateSystem(CString sPath);			// 구동 시스템들을 생성한다.
	//int InitialSystem();						// 시스템을 초기화 시킨다.
	//int StartupSystem();						// 준비된 시스템들을 구동시켜준다.

	//int ShutdownSystem();					// 시스템들을 구동을 마치고 종료 준비를 시킨다.
	//int DeleteSystem();						// 생성된 구동 시스템들을 제거한다.

	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	// Gaus Library를 사용할 때 기본적으로 구현되어야 하는 부분 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	CGxGausFrame*	m_pGausFrame;							// Gaus Framework 객체
	CGxSocketMgr*	m_pSocketMgr;


	// Gaus Frame 기동 순서
	void SystemCreate(CWnd* pOwner, CString sBase);			// 1. Frame 객체를 생성한다.
	void SystemAdd();										// 2. 각종 System/Service/Station을 생성하여 추가한다.
	void SystemInitial();									// 3. 각종 제어기 장치를 초기화 시킨다.
	void SystemStartup();									// 4. 자동 제어를 시작한다.
	void ServiceStartup();									// 5. 각종 FDC 측정용 데이터 취득을 시작한다.

	// Gaus Frame 종료 순서
	void SystemStop();										// 1. 자동 수행을 중지 시킨다.
	void ServiceStop();										// 2. 각종 FDC 측정용 데이터 취득을 중지시킨다.
	void SystemShutdown();									// 3. 각종 장치 제어기의 장치를 Close 해준다.
	void SystemRelease();									// 4. 할당했던 객체들의 리소스를 반환한다.

	//of Gaus Library -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	// 정보 조회 함수
	BOOL GetEnabled();										// Machine System을 사용할 수 있는 상태인가 ? 모든 초기화를 마쳤는가 ?

	void InitialDataFiles();								// 각종 파일들의 초기화와 불러오기를 수행한다.
	void SetLogMgr();										// Log Manager에게 관리할 Log를 지정한다.


	// 각종 로그 기록용
	CGxLog			m_logTracking;						// MES로 Track In/Out을 전송하는 log
	CGxLog			m_logBlock[eMAX_LOGCount];		// Log 배열
	CGxLog*			m_pLogSys;						// System Log
	CGxLog*			m_pLogSeq;						// 순차처리 log
	CGxLog*			GetLogPointer(int nIdx);		// Log pointer 획득

	// 데이터 기록용 log file
//	CGxLogData			m_dlogAlarm;				// Alarm 기록용
//	CGxLogData			m_dlogTACTTime;				// TACT Time 로그 기록용
//	CGxLogData			m_dlogProductData;			// 생산율 기록용

	CGxLogData*			GetAlarmDataLog();			// Alarm data 기록용 Log pointer
	CGxLogData*			GetTACTTimeDataLog();		// TACT Time 로그 기록용 Log pointer
	CGxLog*			GetProductDataLog();		// 생산율 로그 기록용 Log pointer


//	CGxDataLog			m_dlogLosstime;				// 비가동 기록용
//	CGxDataLog*			GetLosstimeDataLog();		// 비가동 data 기록용 Log pointer
	
	CStatsBlock*		m_pStatsBlock;				// 통계정보

	// Cell data 및 TACT 관련 정보를 저장/관리하는 중요한 Class instance
	CCellInfo*			m_pCellInfo;			// Cell Data 저장 Class

	// Jig 및 Channel별 데이터를 관리할 class 객체
	CJigInfo*			m_pJigInfo;


	// 각종 파일 경로 관리용 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	//d void SetBasePath(CString sPath);			// 기본 경로를 지정시켜준다 
	CString GetBasePath(void);					// 기본 경로 조회
	CString GetDataPath(void);					// Data folder 경로 조회
	CString GetLogPath(void);					// Log folder 경로 조회
	CString GetUnitImgPath(void);				// Unit Image Path
	CString GetRuntimeImgPath(void);
	CString GetVIResultPath(void);			// 검사PC에서 테스트 결과를 가져올 경로
	CString GetVIRecipePath(void);			// 검사PC에게 변경된 레서피를 전달해줄 폴더
	CString GetVIInformPath(void);			// 검사PC에서 해상도관련 스펙을 가져올 경로

	void SetExePath(CString sPath);			// 실행파일 경로
	void SetLanguageType(int nType);			// 언어 설정
	int GetLanguageType();						// 설정된 언어 조회
	void SetOwnerHandle(HWND hHandle);		// 상위 소유자의 Handle을 넘겨준다.

	// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==

	// Label Printer用 RS232C Serial Class
	//CCommThread		m_CommPrinter;				// Label Printer용 Serial Class
	//BOOL IsPrinterOpen(void);					// Label Printer용 시리얼 통신이 Open되었나 ?
	//BOOL OpenLabelPrinter(int nPortNo);		// Label Printer용 시리얼 포트를 연다.
	//void CloseLabelPrinter(void);				// Label Printer용 시리얼 포트를 닫는다.
	//BOOL DefectPrintOut(int nJig, int nCh);	// 해당 채널의 Defect 내용을 출력한다.
	//DWORD LabelPrintOut(int nLn, CString sMsg);	// 지정 줄에 문자열을 출력해준다. (버퍼에 출력)
	//BOOL TestPrintOut();							// 시험 출력
	//BOOL TestBarcodePrint(CString sMsg);		// 2차원 바코드를 출력한다.

	// TCP/IP 통신 상태를 확인한다 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	BOOL GetCommConnected(int nIdx);			// 지정 TCP/IP 통신의 연결 상태를 조회한다.
	int GetCommConnectCheck();				// 통신 상태를 점검하여 오류내용을 되돌린다.

	// 통신 설정 조회
	CString GetTCPSymbol(int nIdx);			// 지정한 통신의 ID명을 획득한다. Index정의는 MCDefine.h
	CString GetTCPName(int nIdx);				// 지정한 통신의 이름을 획득
	CString GetTCPAddress(int nIdx);			// 지정한 통신의 주소를 획득한다.
	int GetTCPPort(int nIdx);					// 지정한 통신의 Port를 획득한다.

	// DATA-PC에게 Cell의 존재 여부를 전송한다.
	BOOL SendCellExistToDataPC();


	// AutoForm에 보여줄 Align 및 MCR 실행 이미지 파일이름을 지정하고 조회한다.
	void SetViewImgFilename(int nCh, int nIdx, CString sFile);			// 이미지 이름 지정
	CString GetViewImgFilename(int nCh, int nIdx);

	// TEST用 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// 메모리 사용량 기록
	MEMORYSTATUS m_memoryStatus;				// 메모리 사용량
	CString GetMemoryCheck();					// 메모리 사용량 기록용
	void WriteMemoryCheck();					// 메모리 사용량 기록
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	BOOL GetVIRetryDefect(CString sResult);	// 검사를 진행하지 못한 Defect이 발생하였다면,


	// 전면부 버튼의 활성화 여부
	BOOL GetPanelButtonLock();
	void SetPanelButtonLock(BOOL bFlag);	// TRUE:전면부 버튼 비활성화, FALSE:버튼 활성화


	////d 언어 설정
	//int GetCurrentLang();						// 현재 사용하고있는 언어 index

	// Defect List 관련
	BOOL LoadDefectList();						// Defect List 데이터를 파일에서 읽어들인다.
	BOOL SaveShortcutList();					// Shortcut list를 파일로 저장한다.
	int SetDefectShortcutAppend( TDefectItem* pDefect );	// Defcet Shortcut 추가


	//2014-12-22, jhLee 제품 투입시 특정 Cell ID는 투입시키지 않고 검사 제외를 시켜 다시 제거토록 한다.
	CMapStringToString m_mapExceptCellID;				// 제외시킬 Cell ID를 저장하고 검색할 Map
	int LoadExeptCellID();									// 제외시킬 Cell ID를 파일에서 읽어온다.
	BOOL GetIsExceptCellID(CString sCellID);			// 지정한 Cell ID가 검사를 제외 할 대상인가 ?

	//2015-06-21 JH-PARK 제품 투입시 판정 Cell인지 확인하여 투입 여부를 확인한다;
	CMapStringToString m_mapJudgeCellID;				// 제외시킬 Cell ID를 저장하고 검색할 Map
	int LoadJudgeCellID();								// 제외시킬 Cell ID를 파일에서 읽어온다.
	void ReleaseJudgeCellID();									// 종료시 계정 정보를 메모리 해제해 준다.

	BOOL GetIsJudgeCellID(CString sCellID);				// 지정한 Cell ID가 검사를 제외 할 대상인가 ?




	// 비가동 관련 데이터 읽어오기
	BOOL LoadLosstimeItemList();
	void AddCellLossTime(double dbTime);	// 지정 시간을 모든 JIG의 Cell들에게 적용한다. (기존 시간에 계속해서 누적시킨다.)

	// STOP->AUTO로 변환시 Contact Count 초기화
	void ClearContactCount();					// 지정 지그의 Cell 데이터들의 Contact Count 값을 초기화 시킨다 (작업자 양수 버튼이전의 경우)
	void UpdateLoadPGOffTime();					// STOP -> AUTO로 상태 변경시 Loading시의 PG-OFF 시간을 현재 시각에 맞춰서 변경한다.

	// Alarm 조치관련 데이터 읽어오기
	BOOL LoadRepairAlarmItemList();

	// 예외처리 할 설정값들을 읽어온다. 2015-07-14, jhLee
	void LoadExceptConfigFile();


	// 현2015-08-05, 재 모델의 Align Mark가 모두 존재하는가 ?
	BOOL CheckAlignMarkFile();

	// 각종 데이터 기록
	void WriteAlarmData();						// Alarm Data를 파일로 기록한다.
	CString GetAlarmLogFileName(CTime tmDay);						// 지정일의 Alarm Log 파일 이름을 조회한다.


	void WriteTACTTimeData(int nJig);			// 지정 JIG의 TACT Time을 파일로 기록한다.
	void WriteProdcutData(CTime time);			// 시간에 따른 생산율에 대한 정보를 기록한다

	// VI 검사 파일을 삭제한다.
	void DeleteVIResultFile(int nJig);	// VI 검사 PC의 결과 파일과, 내부 복사 사용 파일 삭제, 해당 Cell이 Unloading되면서 수행한다.
	void DeleteAllVIResultFile();			// VI 검사 PC의 결과 파일과, 내부 복사 사용 모든 파일 삭제, 설비 초기화시 실시

	// Data PC 전송용 Cell 임시파일 삭제
	void DeleteDataPCCellFile(int nJig);	// 지정 지그에서 사용했던 Data PC 전송용 Cell 임시파일 삭제
	void DeleteAllDataPCCellFile();			// Data PC로 전송하는 Cell 정보 임시 파일을 모두 일괄 삭제한다. 설비 초기화시 실시


	// GUI 관련
	// Sub-Dailog를 보여주거나 감추어준다.
	void ShowSubDialog(int nIdx, BOOL bShow);		// Dialog를 보여주거나 감추어준다.


	// 품질 경고 관련
	// 2015-02-23, jhLee
	void AddQualWarningDefect(int nJig);		// 지정 Jig의 결과를 조사하여 품질경고용 Defect 추가를 수행한다.


public:

	// 설비 실행에 필수적인 데이터 관리
	void InitProfile();							// Profile 설정
	BOOL LoadProfile();							// Profile 데이터를 읽어온다.
	void SaveProfile(CString sGroup);		// Profile 데이터를 값 그룹별로 저장한다.

	// Model change관련
	void InitRecipeFile();						// Recipe 파일 등록
	BOOL LoadRecipeFile(CString sModel);	// 지정한 이름의 Recipe 파일을 불러와서 적용시킨다.
	void SaveRecipeFile(CString sModel);	// 지정한 이름으로 현재의 데이터를 Recipe 파일로 저장시킨다.
	void SaveAsRecipeFile(CString sModel);	// 지정한 이름으로 현재의 Recipe을 저장한다.
	void SaveRecipeData(CString sSec);	   // Recipe의 내용중 지정된 Section name에 해당되는 데이터를 저장한다.
	void LoadRecipeData(CString sSec);		// 지정 Section의 값들을 불러온다.
	BOOL CheckRecipeChange();				// Recipe값 변경 감지

	// 생산량 정보를 기록하고 관리하는 파일 초기화
	void InitProductFile();
	void LoadProductFile();						// 파일 읽어오기
	void SaveProductFile(CString sGroup);	// 지정 그룹의 데이터 저장
	void AddTotalProductCount();				// 총 생산량 누적
	void AddProductCount(int nIdx, int nJudge);		// 판정별 생산량 누적
	void AddUVMCRCount(int nJudge);			// UV-MCR 관련 누적
	void AddAlignCount(int nJudge);			// Align 관련 누적
	void AddRuntimeCount(BOOL bRunFlag);	// 설비 가동에 대한 누적
	void SaveProductCount();					// 생산량 저장 및 화면 갱신
	void SaveUVMCRCount();						// UV-MCR 관련 누적 저장
	void SaveAlignCount();						// Align 관련 누적 저장
	void SaveRuntimeCount(BOOL bForce);		// 설비 가동에 대한 누적 저장

	void WriteContactData();									// 시간에 따른 생산율에 대한 정보를 기록한다
	void SaveTodayProductData(int nIdx, int nCnt = 0);	// Log폴더에 일일 생산수량을 ini파일로 저장해준다.;
	void LoadTodayProductData();				// 


	void SaveRuntimeData();						// 실행 데이터를 저장하고 불러온다.
	void LoadRuntimeData();


	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	// 각 Zone별 대표 불량을 판정한다.
	int GetDefectFromJudge_old( int nZone, TCellData* pCell, CString &strDefect, int &nClass, CString &sDefectCode );
	void ProcessFinalJudge(int nJig);							// 지정 Cell의 최종 판정을 처리한다. (D-Zone 검사완료 후 실시)
	BOOL GetIsInspProcess(int nInspIdx, int nJig, int nCh);		// 지정 지그를 해당 Test를 수행할 것인가 ?

//New
	BOOL GetPerformStep(int nJig, int nCh, int nStep);			// 지정 검사를 수행할 것인가 ? 이전 단계의 결과에 따라 수행여부 판단
	void SetZoneJudgement(int nZone, TCellData* pCell);			// 지정 Zone에서의 대표불량을 판정한다.
	BOOL GetVIRetryDefect(TCellData* pCell);					// 'RETRY'를 표시해야하는 VI 결과값인가 ?
	BOOL GetVIEQErrorDefect(CString sName);						// 설비성 비전 불량인가 ? 'RETRY'를 표시해야하는 VI 결과값인가 ?



	BOOL LoadDefectNameTable();								// Defect Name 매칭 테이블 데이터 읽기
	void ReleaseDefectNameTable();							//  메모리 해제
	BOOL GetDefectNameCode(CString sKey, CString &sMESName, CString &sMESCode);		// 지정한 defect 이름에 해당되는 MES 보고용 이름과 Code를 조회한다.
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// 신호기PC 관련
	
	void SetCellZoneChange(int nJig, int nZone);				// Cell Info data에 현재 위치 Zone을 지정 Zone으로 지정한다.
	BOOL GetPGTurnOnSendCheck(int nJig);						// 지정 Jig에서 신호기로 TurnOn 명령을 보낸 상태인가 ?
	BOOL PG_AZonePGControl(int nCmd);							// PG Control : Next/Back/On-Off 제어, 전면 버튼 사용
	BOOL GetRcvBackNext();										// Next / Back 버튼 명령에대한 응답이 수신되었나 ?

//
//	// 
//	void PG_DrawCellText(int nJig, int nCh, CString sMsg );		// Cell에 문자열을 표시한다.
//	void PG_DrawCellJudge(int nJig, int nCh);					// Cell에 판정결과를 표시한다.
//
//	void PG_DrawCellResult( TCellData* pCell );					// 검사결과를 Cell 화면에 표시
//	void PG_DrawCellDefectCircle( TCellData* pCell );			// Defect위치에 원을 그려준다.
//
//	void PG_SendContactCurrentResult(int nJig, int nCh);		// 지정 지그의 Cell로 Contact Current 결과를 출력한다.
//
//	void PG_SendZoneChange(CString sZone, int nJig);			// Zone Change 전송
//	void PG_SendZoneChange(CString sZone, int nJig, int nCh);	// 지정 채널을 직접 Zone 변경을 시켜준다.
//
//	void PG_SendTMDInfo(int nJig);								// 지정 지그의 TMD_INFO를 요청한다.
//	BOOL PG_SendTMDInfoRequest();								// 2015-07-16, jhLee, 신호기에게 모든 채널의 TMD 정보를 요청한다. (초기화시에 수행)
//	void PG_SendCellLoading(int nJig);							// Cell loading 전송
//	void PG_SendTurnOnOff(int nJig, BOOL bFlag);				// 지정 지그의 TURN_ON/OFF 전송
//	void PG_SendTurnOnOff(int nJig, int nCh, BOOL bFlag);		// 지정 지그-채널의 TURN_ON/OFF 전송
//	void PG_SendNext(int nJig);									// 지정 지그의 NEXT 전송
//	void PG_SendNext(int nJig, int nCh);						// 지정 지그의 CH에 NEXT 전송
//
//	void PG_SendBack(int nJig);									// 지정 지그의 Back 전송
//	void PG_SendBack(int nJig, int nCh);						// 지정 지그의 CH에 BACK 전송
//	void PG_SendReset(int nJig);								// 지정 지그의 Reset 전송
//
//
////?	void PG_SendResetForAutoContact(int nJig);					// 지정 지그의 C-Zone Contact off 기능 수행
////?	void PG_SendResetCheckAutoContact(int nJig);				// 지정 지그의 C-Zone Contact off 기능 수행했는지 조사한 후 필요시 Reset을 수행한다.
//
//	void PG_SendChReset(int nJig, int nCh);						// 지정 지그 / 채널에게 Reset 전송
//	void PG_SendTSPStart(int nJig, int nCh);					// STP 검사를 지령한다.
//	void PG_SendSleepCurrentStart(int nJig);					// SLEEP_CURRENT 측정 명령
//	void PG_SendIDCheckStart(int nJig);							// ID_CHECK ( White current ) 측정 명령
//	BOOL PG_SendTECheck(int nJig, int nCh);						// 2015-03-27, jhLee, C-Zone에서 TE_CHECK 기능을 추가한다.
//	BOOL PG_SendPatternSelect(int nJig, int nCh, int nIdx);		// 2015-06-02, jhLee, D-Zone에서 표시하는 패턴을 지정해준다.
//
	int GetPGErrorCheck(int nJig);								// 지정 지금의 신호기가 정상적인 상태인지 여부
	int GetPGContactCurrentCheck(int nJig);						// 지정 지그의 Contact Current 통과 여부
	int GetPGCellLoadingCheck(int nJig);						// 지정 지그의 Cell loading 완료 여부
	BOOL Is_PGCellLoadingCheck(int nJig);						// 지정 지그의 Cell loading 완료 여부
	BOOL Is_PGTSPEndCheck(int nJig);							// TSP 완료 여부
//	BOOL Is_PGSleepCurrentEndCheck(int nJig);					// Sleep Current 완료 여부
//	BOOL Is_PGSIDCheckEndCheck(int nJig);						// ID_CHECK (White current) 완료 여부
	void PG_CatchTMDInfo(int nJig);								// 지정 Jig의 각 채널에 TMD Info 정보를 지정한다.

	//void SetPGContactFlag(int nJig, int nCh, BOOL bFlag);		// 지정 채널의 신호기 Contact 변화 기록, 
	//BOOL GetPGContactFlag(int nJig, int nCh);					// 지정 채널의 신호기 Contact 상태
	//BOOL GetPGContactJigFlag(int nJig);							// 지정 지그의 contact 여부
	//void ClearPGContactFlag(int nJig);							// 지정 채널의 신호기 Contact 상태를 OFF로 Clear 해준다.
//


	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// CIM 관련 함수, 2014-06-26, jhLee, 기존 MIT-AFT에서 발췌

	//// CIM, Base:SONIC, 
	//void CIM_Ict_Info_InspectionEnd(int nJig);					// 새로운 Cell을 로딩하여 신호기 Contact가 되었다.
	//void CIM_ContactAlignReport(int nJig, int nCh);				// Align 결과를 전송한다.
	//void CIM_CellProcessStart(int nJig, int nCh);				// 새로운 Cell을 시작하였다. (Track-In)

	//void CIM_Pwr_InspectionEnd(int nJig, int nCh);				// 2014-12-23,jhLee,신호기 파워검사 종료
	//void CIM_TSP_InspEnd(int nJig, int nCh);					// TSP 검사를 종료하였다.
	//void CIM_VT_InspEnd( int nJig, int nCh );					// 검사 PC 화상 검사완료 알림
	//void CIM_All_InspectionEnd(int nJig);						// 모든 검사가 종료 되었다.

	//void CIM_CellProcessComplete(int nJig);						// Cell의 작업을 완료 하였다. (Track-Out)
	//BOOL CIM_GetOperationData(int nJig);						// 작업 완료 후 데이터 취합

	//// 추가한 내용
	//void CIM_AlarmSet(UINT nErrID, CString sErrName);
	//void CIM_AlarmClear(UINT nErrID, CString sErrName);	
	//void CIM_UserLogin(CString sID);							// 작업자 로그인 보고
	//void CIM_UserLogout();										// 작업자 로그아웃 보고

	//// 2015-04-15, jhLee, 작업자 인증 처리 추가
	//BOOL CIM_UserLoginRequest(CString sID, CString sPassword);	// 작업자 로그인 가능 여부를 상위 서버에게 요청 한다
	//BOOL CIM_UserLogoutRequest(CString sID, CString sPassword);	// 작업자 로그아웃 가능 여부를 상위 서버에게 요청 한다

	//// 2015-05-26, jhLee, Cell Tracking 정보 요청
	//BOOL CIM_CellTrackingInfoRequest(int nJig, int nCh);			// Cell Tracking 정보 요청

	//// 2016-01-04, Cell Lot Information 요청, POCH 처리용
	//BOOL CIM_CellLotInfoRequest(int nJig, int nCh);					// Cell Lot 정보 요청, POCH

	//// PPID 관련
	//BOOL CIM_PPIDReport(CString sPPID);
	//BOOL CIM_PPIDCreate(CString sPPID);
	//BOOL CIM_PPIDModify(CString sPPID);
	//BOOL CIM_PPIDDelete(CString sPPID);
	//BOOL CIM_PPIDChange(CString sOldID, CString sNewID);
	//
	//BOOL CIM_ReportTMDChange();									// 2015-07-16, jhLee, 신호기 TMD가 변경되었을 경우 처리


	//// 비가동 보고, TPSystem
	//void CIM_TPSCodeReport(CString sCode, CString sMsg);		// 2014-12-24, TPS Code 전송 추가


	// 2015-10-19, jhLee, Abort발생시 Track-In 보고를 한 Cell이 아직 설비내 Track-Out 보고를 하지 않았다면 'R'로 보고를 한다.
	void ProcessAbortReport();



	// 추후 구현예정
	// void	CIM_UnitTactReport(CMessage::E_UNIT eUnit, short sPoint, TAG_CELL tagCell);

	// 본 설비에서는 실시하지 않는 MTP 테스트 과련 내용은 사용하지 않는다. jhLee, 2014-06-26
	// void	CIM_Mtp_InspectionEnd(); //SONIC-2013.05.28.
	// void	CIM_Pwr_InspectionEnd(); //SONIC-2013.05.28.

	// 사용 안함 (단일 Unit 설비이므로)
	// void  CIM_UnitCellReport(CMessage::E_UNIT eUnit, short sPoint, TAG_CELL tagCell); //SONIC-2013.08.14.
	// void	CIM_CompInOutReport(CMessage::E_UNIT eFromUnit, short sFromPoint,  CMessage::E_UNIT eToUnit, short sToPoint, TAG_CELL tagCell);	//SONIC-2013.07.18.

	// CellData class 안에 기술
	//	BOOL	CellIdCheckup(CString strCellId);

	// B Zone에서 수행하는 Align 수행안함
	//	void	CIM_VisionAlignReport( JIG_CH ch );	//SONIC-2013.07.23.
	


	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	float CountTactTime(SYSTEMTIME StartTime, SYSTEMTIME EndTime);

	// Cell Log 및 비가동 log 관련
	// 2015-03-03, jhLee, Cell Log 발식 변경, 설비프로그램 기동중에도 데이터를 참조할 수 있도록 보조 파일을 생성한다.

	// 각종 데이터를 저장하기 위한 Log file -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	CGxLogData		m_logCellLog;						// Cell Log
	CGxLogData		m_logCellBIN2Log;					// BIN2Test 기록용 Cell Log
	CGxLogData		m_logLosstime;						// 비가동 로그

	// 데이터 기록용 log file
	CGxLogData		m_logAlarm;							// Alarm 기록용
	CGxLogData		m_logTACTTime;						// TACT Time 로그 기록용
	CGxLogData		m_logContactData;					// 생산율 / Conctact 정보 기록용
	CGxLog			m_logProductData;					// 생산 정보 기록 파일 이름 취득용, m_iniProduct ini file이 원본

	void SetUpdateLogToday(int nHour, int nMin);	// 각종 log들의 오늘 날짜 기준을 변경하여 준다.

	// Cell log
//	CGxLogData	m_logCellData;							// Cell Log를 기록한다. (새로운 방식)
	void SetCellLogPath(void* pValue);				// 새로운 Cell log 경로를 지정한다.
	BOOL SaveCellLogData(int nJig);					// 새로운 방식으로 지정 지금의 모든 Cell에 대하여 Cell Log를 File로 기록한다.
	void OpenCellLogFile();								// Cell log 파일을 열어 다른곳에서 접근을 막는다. (프로그램 시작시, 날짜 변경시)
	void CloseCellLogFile();							// Cell Log file 강제 닫기
	void SaveProductData(int nJig);					// 통계용 생산 정보를 저장한다.

	// 비가동 Log
//	CGxLogData	m_logLosstimeData;						// 비가동 Log를 기록한다. (새로운 방식)
	void SetLosstimeLogPath(void* pValue);			// 비가동 Log 파일이름 생성 callback 함수
	void WriteLossTimeRecord(TLosstimeRecord& rRec, BOOL bPrevDay);		//새로운 방식으로 지정한 비가동 레코드 데이터를 파일로 기록한다.
	void WriteAutoLossTimeRecord(TLosstimeRecord& rRec);

	void InitLosstimeInfo();					// 비가동 정보를 임시 기억하기위한 초기화
	void LoadLosstimeInfo();					// 파일에 저장된 비가동 정보를 읽어온다. (Program 실행시)
	void SaveLosstimeInfo();					// 현재 설정된 비가동 정보를 파일로 기록한다.
	void ClearLosstimeInfo();					// 비가동 관련 내용이 없다면 내용을 청소해준다.

	void WriteLossTimeByAlarm();				// Alarm에 의해 강제적으로 비가동 상태가 종료되었다고 저장한다.
	void WriteOPLossTime(int nJigNo);		// 작업자 시간초과 기록 (비가동 로그에 남긴다)
	void WriteEQLossTime(int nJigNo);		// 설비 시간초과 기록 (비가동 로그에 남긴다)


	// 비가동 로그의 일별, 조별 마무리를 수행해준다.
	void LosstimeDataCloseProcess(BOOL bPrevDay);
	void ShiftCloseProcess();

	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// 작업 진행 관련
	void SetDefectInputMode(BOOL bFlag);		// D-Zone 진입으로 작업자의 Defect 입력하는 모드이다.
	BOOL GetDefectInputMode();					// D-Zone 작업자 Defect 입력 모드인가 ?
	void SetCellInfoEnabled(BOOL bFlag);		// A-Zone 초기 Loading 작업시 Cell 없음/미사용 설정용 Dialog 가능/불가능 설정
	BOOL GetCellInfoEnabled();					// A-Zone 초기 Loading 작업시 Cell 없음/미사용 설정용 Dialog 가능/불가능 여부 조회
	

	// 초기화 여부
	void SetInitialFlag(int nFlag);			// 초기화 진행여부 설정
	int  GetInitialFlag(void);					// 초기화 진행여부 조회

	// 생산하는 제품이 Rigid인지 youm인지 조회한다.
	BOOL IsCellRigid();										// Rigid cell을 생산하는가 ?

	// Data 처리 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	CString GetRecipeModelName(void);		// 모델 이름 반환
	CString GetVersion(void);					// 프로그램 버전

	UINT	GetOptionTime(int nIdx);			// 지정 순번의 Time 설정값 조회


	CString GetCurrentPPID(void);				// 현재 PPID
	void	SetNewPPID(CString sData);			// 새로운 PPID 적용

	// 각 Jig의 채널 사용여부 지정/조회
	void	SetChannelNotUseMode(int nJig, int nCh, BOOL bFlag);		// 지정 채널의 미사용 모드 지정/해제
	BOOL	GetChannelNotUseMode(int nJig, int nCh);						// 지정 채널의  미사용 모드 조회

	// Folder 및 파일 이름생성
	CString MakeFolderNFilename(CString sFolder, CString sFile);		// 폴더를 만들어주고 파일 이름을 되돌려준다.
	CString MakeSubFolderNFilename(CString sFolder, CString sSub, CString sFile);		// 하위 폴더까지 만들어주고 파일 이름을 되돌려준다.

	// 지정된 하루 단위 시각에 맞춰서 기록을 담당할 '오늘' 날자값을 얻어온다.
	CTime	GetToday();				// 오늘
	BOOL GetIsPrevDay(CTime &tmDay);		// 지정한 날짜/시각이 기준시간상 전날인가 ?
	int GetShift(int nNewHour =0, BOOL bChange = FALSE);				// 지금 시각에 따른 작업 조
	
	int GetCurrentShift();				// 현재 시각에 따른 근무조 (Shift)를 되돌려준다.


	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// 검사PC에서 해상도 관련 데이터를 읽어온다.
	BOOL	LoadVIInformFile();	// 검사PC 설정값들, LoadVIInformFile() 파일을 통해서 읽어온다.


	//// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	////2014/09/25 JH-PARK 
	//BOOL m_bUsePM_Mode;					// PM mode 해지시 Login을 할 지 안할지 여부, TRUE : PMmode만 해제, FALSE : Login change, PM Mode 해지 안됨
	//void PM_ModeChange();


	////////////////////////////////////////////////////////////////////////////
	// Alarm 정보
	//2014/11/07 JH-PARK 
	CList<AlarmInfoCode*>					m_ListAlarmInfoCode; // 운영 로그;

	void LoadAlarmInfo();
	void RelaseAlarmInfo();

	// Display Language
	// void DisplayLanuage();

		////////////////////////////////////////////////////////////////////////////
	// 판정완료 Cell ID 기록 
	void WriteJudgeCell_ID(CString strID);	// 2015-06-18 JH-PARK 판정 완료(Retest제외) Cell ID를 기록한다 ( 최대 4개)


	
};

