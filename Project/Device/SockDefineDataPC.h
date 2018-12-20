#pragma once

//////////////////////////////////////////////////////////////////////////
// DATA PC 통신 전용 Define [11/13/2016 OSC]
//////////////////////////////////////////////////////////////////////////

#define MAX_MES_COUNT					12
#define MAX_MES_SIZE					100  //40 2017-04-26,skcho, 가오누리에서 40->100으로변경
#define MAX_CELL_ID						150		// 2016-08-26, jhLee, 최대 Cell ID 크기는 150자까지 가능하다 (SDC 안도현 사원)


#define CRST_OFF						_T("0")
#define CRST_REMOTE						_T("1")
#define CRST_LOCAL						_T("2")

#define EQST_RUN_IS_CELL				_T("2")
#define EQST_RUN_NOT_CELL				_T("1")

#define ALST_SET						_T("1")
#define ALST_CLEAR                      _T("2")

#define ALCD_LIGHT						_T("1")
#define ALCD_SERIOUS					_T("2")


enum E_MESSAGE_CLASS
{
	CLASS_NONE=0,
	CLASS_CONTROL,
	CLASS_MCC,
};

enum E_MESSAGE_TYPE
{
	TYPE_SET_NONE=0,
	//TO DATA PC
	EQUIP_SET_CONTROL,
	EQUIP_SET_DATAPC_VERSION,
	EQUIP_SET_ALARM,
	EQUIP_SET_ERMS,
	EQUIP_SET_CELL_PROCESS_START_LOAD,
	EQUIP_SET_CELL_PROCESS_COMPLETE_UNLOAD,
	EQUIP_SET_CELL_ENDINS,
	EQUIP_SET_EQUIPMENT_STATUS_CHANGE,
	EQUIP_SET_UNIT_STATUS_CHANGE_LOAD,
	EQUIP_SET_UNIT_STATUS_CHANGE_INSPECTION,
	EQUIP_SET_UNIT_STATUS_CHANGE_UNLOAD,
	EQUIP_SET_PPID_CREATE,
	EQUIP_SET_PPID_DELETE,
	EQUIP_SET_PPID_MODIFY,
	EQUIP_SET_PPID_CHANGE,
	EQUIP_SET_PARAMETER_CHANGE,
	EQUIP_SET_PPID_OPERATION,
	EQUIP_SET_MATERIAL_CHANGE,
	EQUIP_SET_OPCALL_EVENT,
	EQUIP_SET_INTERLOCK_EVENT,	
	EQUIP_SET_UNIT_INTERLOCK_EVENT,
	EQUIP_SET_CELL_INFORMATION_REQUEST,
	EQUIP_SET_CODE_INFORMATION_REQUEST,
	EQUIP_SET_CONTROL_STATE_CHANGE_ONESELF_ACK,
	EQUIP_SET_CELL_LOADER_IN,
	EQUIP_SET_CELL_TESTER_OUT,
	EQUIP_SET_CELL_UNLOADER_OUT,

	//부분 검사 완료
	EQUIP_SET_CELL_ICT_ENDINS,
	EQUIP_SET_CELL_INFO_ENDINS,
	EQUIP_SET_CELL_PWR_ENDINS,
	EQUIP_SET_CELL_MTPW_ENDINS,
	EQUIP_SET_CELL_TSP_ENDINS,
	EQUIP_SET_CELL_VT_ENDINS,
	EQUIP_SET_CELL_MTPR_ENDINS,

	//TACT & CREATE & DELETE
	EQUIP_SET_CELL_BEGIN,						// 미사용
	EQUIP_SET_CELL_END,							// 미사용
	EQUIP_SET_CELL_TACT,							// 미사용

	//USER LOGIN & OUT
	EQUIP_SET_CTL_USER_LOGIN,
	EQUIP_SET_INS1_USER_LOGIN,
	EQUIP_SET_INS2_USER_LOGIN,
	EQUIP_SET_CTL_USER_LOGOUT,
	EQUIP_SET_INS1_USER_LOGOUT,
	EQUIP_SET_INS2_USER_LOGOUT,

	//UV MCR READING
	EQUIP_SET_UV_READING_MCR,	//SONIC - 2013.07.22.
	//VISION ALIGN
	EQUIP_SET_ALIGN_MARK_FIND,	//SONIC - 2013.07.22.
	EQUIP_SET_LINE_NO,
	EQUIP_SET_CELL_EXIST,

	//LossCode
	EQUIP_SET_LOSS_CODE,		//20141223 sms

	//TO AFT
	HOST_SET_CONTROL,
	HOST_SET_MODULE,
	HOST_SET_PROC,
	HOST_SET_DATAPC_VERSION,
	HOST_SET_CELL_INFORMATION,		//SONIC - 2014.02.09. Add, Cell Tracking Information

	HOST_SET_OPCALL,
	HOST_SET_INTERLOCK,
	HOST_SET_UNIT_INTERLOCK,			 //2017-11-01,SKCHO , 인터락 종류 추가
	HOST_SET_TERMINAL,
	HOST_SET_CONTROL_STATE_CHANGE,
	HOST_SET_LINE_NO,

	EQUIP_SET_USER_LOGIN_REQUEST,   // 58, 작업자의 Login 검증을 상위 서버에 요청한다.
	EQUIP_SET_USER_LOGOUT_REQUEST,	// 59, 작업자의 Login 검증을 상위 서버에 요청한다.
	HOST_SET_USER_LOGOUT_REQUEST,   // 60, 서버에서 설비를 세우고 작업자를 강제로 Logout 시키라는 지령 메세지

	// 추가된 Message
	EQUIP_SET_CELL_LOT_INFORMATION_REQUEST,		//2017-03-09, skcho, UNLOADER TRAY ID SEQUENCE에서 사용 // 2016-01-04, jhLee, S3F109, POCH 처리를 위한 Cell Lot Information 이슈 발생
	HOST_SET_CELL_LOT_INFORMATION,				//2017-03-09, skcho, UNLOADER TRAY ID SEQUENCE에서 사용 // 2016-01-04, jhLee, S3F109, POCH 처리를 위한 Cell Lot Information 응답

	// 2016-01-20, 지정 Cell Data의 내용을 Clear 하는 요청
	EQUIP_SET_CELL_DATA_CLEAR_REQUEST,			// 지정 Cell Data의 내용을 Clear

	// 2016-02-13, jhLee, 점등 불량 LOSS 보고를 위한 Track In/Out 통합 보고 지령
	EQUIP_SET_LOSS_TRACK_REPORT,				// 특정 불량 Track In/Out 처리

	EQUIP_SET_INSPECTION_RESULT_REPORT,				// 2016-11-19, jhLee, S6F11, CEID 609, 주요 검사 완료 시점에 이벤트를 보내도록 한다.

	HOST_SET_CELL_JOB_PROCESS,

	EQUIP_SET_PORT_PARAMETER_MANAGEMENT,

	HOST_SET_PORT_PARAMETER_REPORT,

	EQUIP_SET_PORT_PARAMETER_REPORT,

	
	//20170206 byskcho s6F11 -   CARRIER ASSIGN REQUEST
	
	EQUIP_SET_CARRIER_RELEASE_REQUEST,  // 열기
	EQUIP_SET_CARRIER_CLOSE_REQUEST,    // 덮기
    EQUIP_SET_CARRIER_ASSIGN_REQUEST,   //
	EQUIP_GET_CARRIER_CELLID_REQUEST,   //2018wjp

	//Carrier Cell Load 20160503 HSK
	EQUIP_SET_CARRIER_CELL_LOAD_START,
	EQUIP_SET_CARRIER_CELL_LOAD_END,

	// 2017-02-27, jhLee, TMD 자동 Download Triger
	HOST_SET_TMD_DOWNLOAD,						// TMD 자동 Download 요청
	EQUIP_SET_TMD_DOWNLOAD,						// TMD 자동 Download 진행 과정 Report

	HOST_SET_RECEPIE_CREATE,   //2017-03-06,skcho recepie 생성 cim 검수 요청(이현식 선임)

	EQUIP_SET_ATTRIBUTEDATA_REQUEST,//2017-03-09, TRAY ID 시퀀스에 필요한 추가 명령
	HOST_SET_ATTRIBUTEDATA_RECEIVE,


	HOST_SET_CARRIER_INFORMATION,   //2017-04-14, skcho, CIMQUAL 추가 사항 Carrier Information

	EQUIP_SET_PORTSTATECHANGE_REQUEST, //2017-07-11, SKCHO, PORT STATE CHANGE 추가
};

enum E_EQST_MATCH
{
	E_EQST_MATCH_NONE=0,
	E_EQST_MATCH_IDLE,
	E_EQST_MATCH_PAUSE,
	E_EQST_MATCH_EXECUTING,
	E_EQST_MATCH_FAULT,
	E_EQST_MATCH_INTERLOCK,
	E_EQST_MATCH_CELL_EXIST,
	E_EQST_MATCH_CELL_NOT_EXIST,	
	E_EQST_MATCH_MAX,
};

//2014-11-03, jhLee, Data-PC와의 Structre size를 통일하기 위해 Struct Pack align을 8 Bytes로 변경한 뒤 원복 시켜준다.
#pragma pack(push, 1)
struct tag_Message
{
	short sMsgClass;	
	short sMsgType;
	short sEqst;	
	short sUnit;
	short sCellPos;
	short sTactPoint;
	char cMsgData[MAX_MES_COUNT][MAX_MES_SIZE];
	//
	// 2016-08-26, jhLee, Cell ID가 150자까지 늘어났으므로 별도의 데이터로 처리한다.
	char cCellID[MAX_CELL_ID+1];

	tag_Message Clear()
	{
		sMsgClass = CLASS_NONE;
		sMsgType = TYPE_SET_NONE;
		sEqst = E_EQST_MATCH_NONE;
		sUnit = 0;
		sCellPos = 0;
		sTactPoint = 0;
		memset(cMsgData, 0x00, MAX_MES_COUNT*MAX_MES_SIZE*sizeof(char));
		memset(cCellID, 0x00, sizeof(cCellID));
	}
};
#pragma pack(pop)

class tag_Message_Unicode
{
public:
	tag_Message_Unicode(void) { Clear(); }
	~tag_Message_Unicode(void) {}

	short sMsgClass;	
	short sMsgType;
	short sEqst;	
	short sUnit;
	short sCellPos;
	short sTactPoint;
	CString strMsgData[MAX_MES_COUNT];
	CString strCellID;

	void Clear();
	void operator=(const tag_Message &src);
	void Convert_tag_Message(tag_Message *pMsg);

};
