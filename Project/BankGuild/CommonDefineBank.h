
#pragma once

//////////////////////////////////////////////////////////////////////////
// 이곳 저곳 범용적으로 쓸 Define 모아두는 곳
//////////////////////////////////////////////////////////////////////////
//Define 부분
//////////////////////////////////////////////////////////////////////////
#define	WM_SERIAL									WM_USER+0x9000
#define	WM_UPDATE_LOWER_MCRCONNECT					WM_USER+0x9001
#define	WM_UPDATE_LOWER_MCRDATA						WM_USER+0x9002
#define	WM_UPDATE_LOWER_MCRLIVE						WM_USER+0x9003
#define	WM_UPDATE_UPPER_MCRCONNECT					WM_USER+0x9004
#define	WM_UPDATE_UPPER_MCRDATA						WM_USER+0x9005
#define	WM_UPDATE_UPPER_MCRLIVE						WM_USER+0x9006

#define TRAY_PITCH_MAX	30	// Tray 최대 쌓을 수 있는 수량
#define MAX_TSP_RESULT	12

#define STX		0x02
#define ETX		0x03
#define ENQ		0x05
#define ACK		0x06
#define CR		0x0D
#define LF		0x0A
#define DEL		0x10
#define CAN		0x18
#define ESC		0x1B
#define NAK		0x15

//////////////////////////////////////////////////////////////////////////
//String 부분
//////////////////////////////////////////////////////////////////////////
#define TEXT_CONTACT_CURRENT_NG	_T("CONTACT_CURRENT_NG")
#define TEXT_CONTACT_NG			_T("CONTACT_NG")
#define TEXT_UV_FAIL			_T("UV_FAIL")
#define TEXT_MCR_READING_FAIL	_T("MCR_READING_FAIL")
#define TEXT_UNDEFINED			_T("UNDEFINED")
#define TEXT_DIC_CHECK_NG       _T("DIC_CHECK_NG")

//#define DRY_RUN			_T("DRY_RUN")
#define UV_FAIL			_T("UV_FAIL")
#define GOOD				_T("GOOD")
#define REJECT				_T("REJECT")
#define OK					_T("OK")
#define NG					_T("NG")
#define FAIL				_T("FAIL")
#define EXT				_T("EXT")
#define SKIP				_T("SKIP")

#define LOWER_MCRPath _T("D:/Cheetah/Image/LOWERMCR_Image.jpg")
#define UPPER_MCRPath _T("D:/Cheetah/Image/UPPERMCR_Image.jpg")

//2017-11-15,SKCHO CIM 버전 같이 표시
#define CIM_VERSION _T("CIM18.07.25.01")

#define DISPVACTIER 1999
#define DISPSTATETIME 1998	
//2017-12-18,SKCHO 추가
#define DRYRUN_TIMER  1997
#define LDROBOTORG_TIMER 1996


#define AGV_IN_DELAY_TIMER 2000
#define AGV_OUT_DELAY_TIMER 2001
//////////////////////////////////////////////////////////////////////////
//enum 부분
//////////////////////////////////////////////////////////////////////////
enum JIG_ID
{
	JIG_ID_A =0,
	JIG_ID_B,
	JIG_ID_C,
	JIG_ID_D,
	JIG_ID_MAX
};

enum ZONE_ID
{
	ZONE_ID_A,
	ZONE_ID_B,
	ZONE_ID_C,
	ZONE_ID_D,
	ZONE_ID_MAX
};

enum JIG_CH
{
	JIG_CH_1 =0,
	JIG_CH_2,
	JIG_CH_MAX
};

enum CIM_JIG_CH
{
	CIM_JIG_CH_1,
	CIM_JIG_CH_2,
	CIM_JIG_CH_3,
	CIM_JIG_CH_4,
	CIM_JIG_CH_5,
	CIM_JIG_CH_MAX
};

enum LANGUAGE
{
	LAN_KOR,
	LAN_ENG,
	LAN_VNM,
	LAN_MAX
};

enum TRAY_PORT
{
	GOOD_IN_TRAY_PORT,
	GOOD_OUT_TRAY_PORT,
	NG_TRAY_PORT,
	NG_BUFFER_TRAY_PORT,
	TRAY_PORT_MAX
};

enum PORT_POS
{
	TRAY_BUFFER,
	TRAY_IN,
	TRAY_POS_MAX
};

enum VAC_STATE
{
	VAC_OFF = 0,
	VAC_ON = 1,
	VAC_STATE_MAX
};

enum BLOW_STATE
{
	BLOW_OFF = 0,
	BLOW_ON = 1,
	BLOW_STATE_MAX
};

enum TILT_STATE
{
	TILT_DOWN = 0,
	TILT_UP = 1,
	TILT_STATE_MAX
};

enum SENSOR_STATE
{
	SENSOR_OFF = 0,
	SENSOR_ON = 1,
	SENSOR_STATE_MAX
};

enum CYL_STATE
{
	CYL_BWD = 0,
	CYL_FWD = 1,
	CYL_UP = 1,
	CYL_DOWN = 0,
	CYL_STATE_MAX
};

enum CLASS_CELL
{
	NONE_CELL = 0,
	GOOD_CELL,
	REJECT_CELL,
	BIN2_CELL,		// SKIP도 BIN2로 치자.... [6/24/2013 OSC]
	CLASS_CELL_MAX
};

enum INSP_POS
{
	CAMPOS_LEFT = 0,
	CAMPOS_RIGHT,
	CAMPOS_IDLE
};

//kjpark 20161017 CellInfo 항목 추가(Cell Log 기준)
enum CLASS_CELL_LOG
{
	CELL_LOG_OK = 0,
	CELL_LOG_NG,
	CELL_LOG_BIN2,
	CELL_LOG_MAX
};

//kjpark 20161017 CellInfo 항목 추가(Cell Log 기준)
enum MCR_READ_SELECT
{
	MCR_NONE = 0,
	MCR_AUTO,
	MCR_MANUAL,
	MCR_READ_FAIL
};

//kjpark 20161017 CellInfo 항목 추가(Cell Log 기준)
enum CLASS_RESULT
{
	RESULT_NONE = -1,	// 검사하기 전 상태 [11/5/2016 OSC]
	RESULT_NG = 0,
	RESULT_GOOD
};

//kjpark 20161017 CellInfo 항목 추가(Cell Log 기준)
enum CLASS_DECISION
{
	DECISION_FAIL = 0,
	DECISION_PASS		
};

enum ONOFF
{
	OFF = 0,
	ON,
	ONOFFMAX,
};

enum CWCCW
{
	CCW = 0,
	CW,
	CWCCWMAX,
};

enum CHECK_OPTION
{
	CHECK_AND,
	CHECK_OR
};

enum TMD_INFO_STATUS
{
	TMD_INFO_NOT_RECEIVE = 0,
	TMD_INFO_GOOD,
	TMD_INFO_DOWN,
	TMD_INFO_MAX,
};

enum SERIAL_MODE
{
	SERIAL_GMS = 0,
#ifdef OPERATOR_PC	
	SERIAL_LABELPRINT,
#endif
	SERIAL_TEMPERATURE,
	SERIAL_VACUUM_LD,
	SERIAL_VACUUM_INSP,
	SERIAL_VACUUM_UD,
	SERIAL_LABELPRINT,
	SERIAL_SERVOTORQUE,
	SERIAL_MAX
};

//kjpark 20161018 A Zone Cell exist Check
// enum AZONE_CELL_EXIST
// {
// 	CELL_NONE,
// 	CELL_FULL,	
// };

enum LD_SHUTTLE
{
	SHUTTLE_1,
	SHUTTLE_2,
	SHUTTLE_MAX
};

enum VI_ACTIVE
{
	VI_A1 = 0,
	VI_A2,
	VI_MAX
};

enum ALIGN_RESULT
{
	ALIGN_RESULT_NONE=0,
	ALIGN_RESULT_GOOD,
	ALIGN_RESULT_FAIL,
	ALIGN_RESULT_SKIP
};

enum E_PA_MIT
{
	E_P1=0,
	E_P2,
	E_PA_MAX
};

enum E_MARK
{
	E_MARK_LEFT=0,
	E_MARK_RIGHT,
	E_MARK_MAX
};

enum E_ROBOT
{
	E_LD_ROBOT=0,
	E_UD_ROBOT,
	E_ROBOT_MAX
};

enum MCR_UNIT
{
	LOWER_MCR = 0,
	UPPER_MCR,
	MCR_UNIT_MAX
};

enum MCR_JUDGE
{
	MCR_RESULT_NONE = 0,
	MCR_RESULT_GOOD,
	MCR_RESULT_FAIL,
	MCR_RESULT_SKIP
};

enum E_ROBOT_POS
{
	E_ORG=0,
	E_CELL_LOADING,
	E_PUT_1,
	E_PUT_2,
	E_PUT_3,  //2018-05-14,SKCHO
	E_ROBOT_POS_MAX
};

enum E_CONV_SEN_INDEX
{
	CONV_SEN_1,
	CONV_SEN_2,
	CONV_SEN_3,
	CONV_SEN_4,
	CONV_SEN_5,
	CONV_SEN_MAX
};

enum CELL_POS
{
	CELL_POS_LD_CONV = 0,

	CELL_POS_LDROBOT,

	CELL_POS_UPPER_SHUTTLE_CH1,
	CELL_POS_UPPER_SHUTTLE_CH2,

	CELL_POS_LOWER_SHUTTLE_CH1,
	CELL_POS_LOWER_SHUTTLE_CH2,

	CELL_POS_PA_TR_CH1,
	CELL_POS_PA_TR_CH2,

	CELL_POS_PA_STAGE_CH1,
	CELL_POS_PA_STAGE_CH2,

	CELL_POS_LD_TR_CH1,
	CELL_POS_LD_TR_CH2,

	CELL_POS_AZONE_CH1,
	CELL_POS_AZONE_CH2,

	CELL_POS_BZONE_CH1,
	CELL_POS_BZONE_CH2,

	CELL_POS_CZONE_CH1,
	CELL_POS_CZONE_CH2,

	CELL_POS_DZONE_CH1,
	CELL_POS_DZONE_CH2,

	CELL_POS_UD_TR_CH1,
	CELL_POS_UD_TR_CH2,

	CELL_POS_UD_STAGE_CH1,
	CELL_POS_UD_STAGE_CH2,

	CELL_POS_UD_ROBOT,

	CELL_POS_GOOD_TRAY,

	CELL_POS_NG_TRAY,

	CELL_POS_UD_CONV,	//2018-05-16,MGYUN,추가

	//2018wjp
	CELL_POS_TRAY_11,

	CELL_POS_TRAY_12,

	CELL_POS_TRAY_13,

	CELL_POS_TRAY_14,

	CELL_POS_TRAY_15,

	CELL_POS_TRAY_21,

	CELL_POS_TRAY_22,

	CELL_POS_TRAY_23,

	CELL_POS_TRAY_24,

	CELL_POS_TRAY_25,

	CELL_POS_TRAY_TO_ROBOT,

	CELL_POS_MAX
};

//kjpark 20161019 MainFrame timer 추가 - 인터락, 타워램프 컨트롤
enum TMID
{
	TMID_CHECK = 0,
	TMID_TEST,
	TMID_TEST_BUFFER,
	TMID_50,
	TMID_100,
	TMID_200,
	TMID_500,
	TMID_1000,
	TMID_60000,
	TMID_PEELER_ION,
	TMID_RECONNECT,
	TMID_AUTO_DELETE_FILE,
	TMID_ERROR_HAPPEN,
	TMID_DOOR_INTERLOCK, 
	TMID_FLICK_LAMP,
	TMID_TURN_TABLE_HOME,
	TMID_TURN_TABLE_HOME_FINISH,
	TMID_NG_MARK_START_DELAY,
	TMID_ROBOT_INTERLOCK,
};

//kjpark 20161019 MainFrame timer 추가 - 인터락, 타워램프 컨트롤
enum LAMP_STATE
{
	LAMP_ON = 0,
	LAMP_FLICKER,
	LAMP_OFF,
	LAMP_MAX,
};

//kjpark 20161022 설정->램프 설정 구현
enum STATUS_LAMP
{
	STATUS_LAMP_RED,
	STATUS_LAMP_YELLOW,
	STATUS_LAMP_GREEN,	
};
//////////////////////////////////////////////////////////////////////////
//Struct 부분
//////////////////////////////////////////////////////////////////////////
typedef struct _SERIALMSG {

	_SERIALMSG ()
	{
		message = 0;
		nParam1 = 0;
		nParam2 = 0;
		nParam3 = 0;
		pParam	= NULL;
		pDest	= NULL;
		pParent = NULL;
	}
	UINT	message;
	int		nParam1;
	int		nParam2;
	int		nParam3;
	LPARAM	pParam;
	LPARAM	pDest;	
	LPARAM	pParent; 
} SERIALEvent;



//kjpark 20161017 CellInfo 항목 추가(Cell Log 기준)
struct TACK_TIME_CELL
{
	SYSTEMTIME
		////////////////////////////////////////////////////////////
		//확정 load
		////////////////////////////////////////////////////////////
		systm_Cell_Load_Tack_Start_Time,//CELL LOAD TACT START TIME:Loader Picker Vacuum ON 시점
		systm_Cell_Load_Tack_End_Time;//CELL LOAD TACT END TIME:Loader Picker Vacuum OFF 시점

		////////////////////////////////////////////////////////////
	SYSTEMTIME
		////////////////////////////////////////////////////////////
		//확정 턴테이블
		////////////////////////////////////////////////////////////
		systmTableTurn_StartTime,
		systmTableTurn_EndTime;
		////////////////////////////////////////////////////////////

	//A Zone
	SYSTEMTIME
		////////////////////////////////////////////////////////////
		//확정 A
		////////////////////////////////////////////////////////////
		systmA_StartTime,//이전 Unloader Picker Vacuum OFF 시점
		systmA_EndTime,//Loader Picker 상승 완료 시점(Table Turn)
		systmA_WaitTime,//Loader Picker 상승 완료(Table Turn) ~ ZONE_B 송신		
		systmA_ContactStartTime,//최초 CELL_LOADING 송신 시각
		systmA_ContactEndTime;//최종 CELL_LOADING,GOOD/NG 수신 시각
		////////////////////////////////////////////////////////////

	//B Zone
	SYSTEMTIME
		////////////////////////////////////////////////////////////
		//확정 B
		////////////////////////////////////////////////////////////
		systmB_StartTime,//ZONE_B 송신 시각
		systmB_EndTime,//B Zone 양수버튼 클릭 시각
		systmB_WaitTime,//B Zone 양수버튼 클릭 ~ ZONE_C 송신
		systmB_Align_Start_Time,
		systmB_Align_End_Time;
		////////////////////////////////////////////////////////////


	//B Zone
	SYSTEMTIME
		////////////////////////////////////////////////////////////
		//확정 C
		////////////////////////////////////////////////////////////
		systmC_StartTime,
		systmC_EndTime,
		systmC_WaitTime,
		systmC_VIStartTime,
		systmC_VIEndTime,
		systmC_VIGrabStartTime,
		systmC_VIGrabEndTime,
		systmC_VIClassStartTime,
		systmC_VIClassEndTime;

	//D Zone
	SYSTEMTIME
		////////////////////////////////////////////////////////////
		//확정 D
		////////////////////////////////////////////////////////////
		systmD_StartTime,//ZONE_D 송신 시각
		systmD_EndTime,//D Zone 양수버튼 클릭 시각
		systmD_WaitTime;//D Zone 양수버튼 클릭 시각 ~ ZONE_A 송신

	SYSTEMTIME
		////////////////////////////////////////////////////////////
		//확정 UnLoad
		////////////////////////////////////////////////////////////
		systmU_TackStartTime,
		systmU_TackEndTime,
		systmUnload_Out_Tack_Time,//Unload Picker Vacuum OFF 시각 기록
		systmUnload_Tack_Start_Time,//ZONE_A 송신시점
		systmUnload_Tack_End_Time;//Unloader Picker Vacuum OFF 시점
		////////////////////////////////////////////////////////////

	SYSTEMTIME
		////////////////////////////////////////////////////////////
		//확정 Common
		////////////////////////////////////////////////////////////
		systmTotal_Tack_Start_Time,//TACT TIME:이전 Unloader Picker Vacuum OFF 시점~Unloader Picker Vacuum OFF 시점
		systmTotal_Tack_End_Time;//
		////////////////////////////////////////////////////////////

	//kjpark 20161017 CellInfo 항목 추가(Cell Log 기준)
	TACK_TIME_CELL& operator= (TACK_TIME_CELL &rb)
	{
		if (this == &rb)
			return *this;

		////////////////////////////////////////////////////////////
		//확정 load
		////////////////////////////////////////////////////////////
		systm_Cell_Load_Tack_Start_Time = rb.systm_Cell_Load_Tack_Start_Time;
		systm_Cell_Load_Tack_End_Time	= rb.systm_Cell_Load_Tack_End_Time;
		////////////////////////////////////////////////////////////



		////////////////////////////////////////////////////////////
		//확정 턴테이블
		////////////////////////////////////////////////////////////
		systmTableTurn_StartTime		= rb.systmTableTurn_StartTime;
		systmTableTurn_EndTime			= rb.systmTableTurn_EndTime;
		////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////
		//확정 A
		////////////////////////////////////////////////////////////
		systmA_StartTime				= rb.systmA_StartTime;
		systmA_EndTime					= rb.systmA_EndTime;
		systmA_WaitTime					= rb.systmA_WaitTime;		
		systmA_ContactStartTime			= rb.systmA_ContactStartTime;
		systmA_ContactEndTime			= rb.systmA_ContactEndTime;
		////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////
		//확정 B
		////////////////////////////////////////////////////////////
		systmB_StartTime				= rb.systmB_StartTime;
		systmB_EndTime					= rb.systmB_EndTime;
		systmB_WaitTime					= rb.systmB_WaitTime;
		systmB_Align_Start_Time			= rb.systmB_Align_Start_Time;
		systmB_Align_End_Time			= rb.systmB_Align_End_Time;
		////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////
		//확정 C
		////////////////////////////////////////////////////////////
		systmC_StartTime				= rb.systmC_StartTime;
		systmC_EndTime					= rb.systmC_EndTime;
		systmC_WaitTime					= rb.systmC_WaitTime;
		systmC_VIStartTime				= rb.systmC_VIStartTime;
		systmC_VIEndTime				= rb.systmC_VIEndTime;
		systmC_VIGrabStartTime			= rb.systmC_VIGrabStartTime;
		systmC_VIGrabEndTime			= rb.systmC_VIGrabEndTime;
		systmC_VIClassStartTime			= rb.systmC_VIClassStartTime;
		systmC_VIClassEndTime			= rb.systmC_VIClassEndTime;

		////////////////////////////////////////////////////////////
		//확정 D
		////////////////////////////////////////////////////////////
		systmD_StartTime				= rb.systmD_StartTime;
		systmD_EndTime					= rb.systmD_EndTime;
		systmD_WaitTime					= rb.systmD_WaitTime;		

		////////////////////////////////////////////////////////////
		//확정 UnLoad
		////////////////////////////////////////////////////////////
		systmU_TackStartTime			= rb.systmU_TackStartTime;
		systmU_TackEndTime				= rb.systmU_TackEndTime	;
		systmUnload_Out_Tack_Time		= rb.systmUnload_Out_Tack_Time;
		systmUnload_Tack_Start_Time		= rb.systmUnload_Tack_Start_Time;
		systmUnload_Tack_End_Time		= rb.systmUnload_Tack_End_Time;
		////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////
		//확정 Common
		////////////////////////////////////////////////////////////
		systmTotal_Tack_Start_Time			= rb.systmTotal_Tack_Start_Time;
		systmTotal_Tack_End_Time			= rb.systmTotal_Tack_End_Time;
		////////////////////////////////////////////////////////////
		return *this;

	}

	TACK_TIME_CELL()
	{
		Clear();
	};

	//kjpark 20161017 CellInfo 항목 추가(Cell Log 기준)
	void Clear()
	{
		////////////////////////////////////////////////////////////
		//확정 load
		////////////////////////////////////////////////////////////
		systm_Cell_Load_Tack_Start_Time	= SYSTEMTIME();
		systm_Cell_Load_Tack_End_Time	= SYSTEMTIME();
		////////////////////////////////////////////////////////////




		////////////////////////////////////////////////////////////
		//확정 턴테이블
		////////////////////////////////////////////////////////////
		systmTableTurn_StartTime		= SYSTEMTIME();
		systmTableTurn_EndTime			= SYSTEMTIME();
		////////////////////////////////////////////////////////////




		////////////////////////////////////////////////////////////
		//확정 A
		////////////////////////////////////////////////////////////
		systmA_StartTime				= SYSTEMTIME();
		systmA_EndTime					= SYSTEMTIME();
		systmA_WaitTime					= SYSTEMTIME();
		systmA_ContactStartTime			= SYSTEMTIME();
		systmA_ContactEndTime			= SYSTEMTIME();
		////////////////////////////////////////////////////////////




		////////////////////////////////////////////////////////////
		//확정 B
		////////////////////////////////////////////////////////////
		systmB_StartTime				= SYSTEMTIME();
		systmB_EndTime					= SYSTEMTIME();
		systmB_WaitTime					= SYSTEMTIME();
		systmB_Align_Start_Time			= SYSTEMTIME();
		systmB_Align_End_Time			= SYSTEMTIME();
		////////////////////////////////////////////////////////////
		
		

		////////////////////////////////////////////////////////////
		//확정 C
		////////////////////////////////////////////////////////////
		systmC_StartTime				= SYSTEMTIME();
		systmC_EndTime					= SYSTEMTIME();
		systmC_WaitTime					= SYSTEMTIME();
		systmC_VIStartTime				= SYSTEMTIME();
		systmC_VIEndTime				= SYSTEMTIME();
		systmC_VIGrabStartTime			= SYSTEMTIME();
		systmC_VIGrabEndTime			= SYSTEMTIME();
		systmC_VIClassStartTime			= SYSTEMTIME();
		systmC_VIClassEndTime			= SYSTEMTIME();







		////////////////////////////////////////////////////////////
		//확정 D
		////////////////////////////////////////////////////////////
		systmD_StartTime				= SYSTEMTIME();
		systmD_EndTime					= SYSTEMTIME();
		systmD_WaitTime					= SYSTEMTIME();

		////////////////////////////////////////////////////////////
		//확정 UnLoad
		////////////////////////////////////////////////////////////
		systmU_TackStartTime			= SYSTEMTIME();
		systmU_TackEndTime				= SYSTEMTIME();
		systmUnload_Out_Tack_Time		= SYSTEMTIME();
		systmUnload_Tack_Start_Time		= SYSTEMTIME();
		systmUnload_Tack_End_Time		= SYSTEMTIME();
		////////////////////////////////////////////////////////////


		
		
		////////////////////////////////////////////////////////////
		//확정 Common
		////////////////////////////////////////////////////////////
		systmTotal_Tack_Start_Time			= SYSTEMTIME();
		systmTotal_Tack_End_Time			= SYSTEMTIME();
		////////////////////////////////////////////////////////////
	}
};

struct CONST_TABLE_POS
{
	CString strName;
	enum
	{
		POS_1=0,
		POS_2,
		POS_3,
		POS_4,
		POS_MAX,
	};
};

//kjpark 20161017 CellInfo 항목 추가(Cell Log 기준)
struct structMCR_READ
{
	CString		strMCRRead_Description;	
};


//kjpark 20161017 CellInfo 항목 추가(Cell Log 기준)
const structMCR_READ MCRReadDefect[] =
{
	_T("NONE")			,
	_T("AUTO")		,
	_T("MANUAL"),
	_T("READ_FAIL")			,
};

//kjpark 20161017 CellInfo 항목 추가(Cell Log 기준)
struct ClassResult
{
	CString		strResultClass;
	
};

//kjpark 20161017 CellInfo 항목 추가(Cell Log 기준)
const ClassResult ClassResultStringArr[] =
{	
	_T("NG")		,
	_T("GOOD")			,
};

//kjpark 20161017 CellInfo 항목 추가(Cell Log 기준)
struct ClassDecision
{
	CString		strResultClass;

};

//kjpark 20161017 CellInfo 항목 추가(Cell Log 기준)
const ClassDecision ClassDecisionStringArr[] =
{	
	_T("FAIL")		,
	_T("PASS")			,
};




//kjpark 20161022 설정->램프 설정 구현
enum STATUS_LAMP_DESCRIPTION
{
	LAMP_INIT,
	LAMP_IDLE,
	LAMP_STOP,
	LAMP_RUN,
	LAMP_ALARM,
	LAMP_PM,
	LAMP_OP_CALL,
	LAMP_AGV,
	LAMP_CHECK,
};


//kjpark 20161022 설정->램프 설정 구현
struct LAMP_SETTING_LIST_BUTTON
{
	CString strColor;
	UINT nResourceID;
	LAMP_STATE lamp_state;
};

//kjpark 20161022 설정->램프 설정 구현
struct LAMP_SETTING_LIST
{
	CString strList;
	BOOL bSelect;
	UINT nResourceID;
	LAMP_SETTING_LIST_BUTTON Red;
	LAMP_SETTING_LIST_BUTTON Yellow;
	LAMP_SETTING_LIST_BUTTON Green;
};

enum AZONE_CELL_EXIST
{
	//kjpark 20161105 Sequence 동작 수정
	CELL_IDLE,
	CELL_NONE,
	CELL_FULL,	
	CELL_LEFT,
	CELL_RIGHT,
};

struct DEFECT_MES_CODE
{
	CString strPopup;
	CString strPopup_KOR;
	CString strPopup_ENG;
	CString strMES_CODE;
};

enum LOG_INDEX
{
	LOG_SYSTEM,				// Program 내부 구동 관련 로그
	LOG_SYSTEM_ERROR,		// 프로그램적으로 분석 필요한 것만 모아놓은 로그
	LOG_SEQUENCE,				// Seq 변경 현황 기록 로그
	LOG_SEQUENCE_TRAY,		// Tray Part Seq 변경 현황 기록 로그
	LOG_SEQUENCE_UNLOADER,	// Unloader Part Seq 변경 현황 기록 로그
	LOG_SOCKET,				// Socket 통신 관련 기록 로그
	LOG_CIM,					// CIM 통신 관련 기록 로그
	LOG_MITALIGN,			// Socket MIT ALIGN 관련 로그
	LOG_MITEDGEALIGN,		// Socket MIT EDGE ALIGN 관련 로그
	LOG_NACHIROBOT,				// Socket 나치 로봇 로그
	LOG_VISION,				// Socket VISION 로그
	LOG_AGV,				// AGV 로그
	LOG_SERIAL,					// Serial 통신 관련 기록 로그
	LOG_SERIAL_GMS,				// GMS Serial 통신 관련 기록 로그
	LOG_OPERATION,			// 유저 버튼 조작 기록 로그
	LOG_ERROR,					// 에러명 기록 로그
	LOG_MOTOR_ERROR,
	LOG_MP2100,		
	LOG_SOCKET_PG,
	LOG_OP2PG_PACKET,
	LOG_NMC,		
	LOG_CELL_FLOW,		// Cell 흐름 추적 위해 만듬
	LOG_UNLOADING,
	LOG_TRAY_READER,			
	LOG_TRAY_ALIGN,	
	LOG_UV_MCR,
	LOG_LOAD_DELAY_TACT,					// 앞단 설비 Load 지연 TACT 기록
	LOG_TURN_TABLE,					// TURN_TABLE 위치값 기록
	LOG_TEACH_MOVE,					// LDTR 티칭한 값으로 이동하는지 위치값 기록하여 확인
	LOG_LDRB_TEACH_MOVE,			// LD ROBOT 티칭한 값으로 이동하는지 위치값 기록하여 확인
	LOG_UDRB_TEACH_MOVE,			// UD ROBOT티칭한 값으로 이동하는지 위치값 기록하여 확인

	LOG_MAX					// Log 폴더 총 갯수
};


enum MAIN_TACK
{
	LOADER_TACT =0,
	UNLOADER_TACT,
	AZONE_TACT,
	BZONE_TACT,
	CZONE_TACT,
	DZONE_TACT,
};

//20170129 by skcho
enum VACCUM_INDEX
{
	MAIN_AIR_1 = 0,
	MAIN_AIR_2,
	MAIN_VAC_1,
	MAIN_VAC_2,
	A_ZONE_CH1,
	A_ZONE_CH2,
	B_ZONE_CH1,
	B_ZONE_CH2,
	C_ZONE_CH1,
	C_ZONE_CH2,
	D_ZONE_CH1,
	D_ZONE_CH2,
	ULD_TR_CH1,
	ULD_TR_CH2,
	ULD_STAGE_CH1,
	ULD_STAGE_CH2,
	VACCUM_MAX,
};

//2017-03-06, skcho,  포트 번호 재정의 1,2,3 ->UB01,UI01,UO01
#define NG_PORT _T("UB01")
#define GOOD_IN_PORT _T("UI01")
#define GOOD_OUT_PORT _T("UO01")

enum JOBCHANGE_INDEX
{
	JOBCHANGE_NONE = 0,
	JOBCHANGE_CASE1 =1,
	JOBCHANGE_CASE2,
};

#define NEXTSTEPID  _T("AMT")

//2017-05-19, skcho, 타임아웃 디파인
//#define  CARRIER_REPORT_TIMEOUT 10.
//#define  CELL_REPORT_TIMEOUT 10.

enum POWER_INDEX
{
	UPS_VOLT_MAX = 0,
	UPS_VOLT_MIN,
	UPS_CURRENT_MAX,
	UPS_CURRENT_MIN,
	UPS_POWER_MAX,
	UPS_POWER_MIN,
	GPS_VOLT_MAX,
	GPS_VOLT_MIN,
	GPS_CURRENT_MAX,
	GPS_CURRENT_MIN,
	GPS_POWER_MAX,
	GPS_POWER_MIN,
	GMS_MAX,
	GMS_MIN,
	AIR_MAX,
	AIR_MIN,
	VAC_MAX,
	VAC_MIN,
	POWER_INDEX_MAX,
};


//2017-10-09,SKCHO
enum REJECT_NAME
{
	ETC_HIGH = 0,
    HIGH_CONTIGUITY,
	HIGH_DOT,
	HO_CONTIGUITY,
	HIGH_DOT_MULTITUDE,
	HIGH_ETC,
	HIGH_MULTITUDE,
};

//2017-09-24,SKCHO  , 우선 5개까지 
#define MAX_REJECT_CNT 5