////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// MCDefine : 현재 시스템에 특화된 정의 파일들
//
//
// #2014-06-04, jhLee : 원형 작성 시작
//
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once


//#include "..\Lib\Align\MkNxCam.h"		// DPOINT 참조를 위한 include


//20170118 byskcho 리소스 스트링에서 관리하므로 삭제
//#define PROGRAM_VERSION				_T("Y_A_MT_16.12.09.01")		// Program Version

#define POS_UNKNOWN					(-1)							// 현재 위치를 알 수 없다.

// Defect Circle을 Cell에 표시하기위한 색 지정
#define COLOR_PTN_RED				RGB(255, 0, 0)
#define COLOR_PTN_GREEN				RGB(0, 255, 0)
#define COLOR_PTN_BLUE				RGB(0, 0, 255)
#define COLOR_PTN_WHITE				RGB(255, 255, 255)
#define COLOR_PTN_GRAY32			RGB(32, 32, 32)
#define COLOR_PTN_GRAY75			RGB(75, 75, 75)
#define COLOR_PTN_GRAY87			RGB(87, 87, 87)
#define COLOR_PTN_GRAY127			RGB(127, 127, 127)
#define COLOR_PTN_BLACK				RGB(0, 0, 0)



// 각종 Station의 이름을 지정해준다.
#define NAME_STATION_Main				_T("STNMainStation")			// MainStation name
#define NAME_STATION_PreAlign			_T("STNPreAlign")				// PreAlign station name
#define NAME_STATION_WorkTable			_T("STNWorkTable")			// WorkTable station name
#define NAME_STATION_AVCamera			_T("STNAVCamera")				// AVCamera station name
#define NAME_STATION_VIChannel1			_T("STNVIChannel1")			// VIChannel 1 Station name
#define NAME_STATION_VIChannel2			_T("STNVIChannel2")			// VIChannel 2 Station name

#define NAME_SYSTEM_Safety				_T("SYSSafety")				// Safety control system name
#define NAME_SYSTEM_Panel				_T("SYSPanel")					// Panel control system name
#define NAME_SYSTEM_Tower				_T("SYSTower")					// Signal Tower control system name
#define NAME_SYSTEM_Aligner				_T("SYSAligner")				// Aligner system name
#define NAME_SYSTEM_VIMonitor			_T("SYSVIMonitor")			// VI 결과 모니터링 system name
#define NAME_SYSTEM_Alert				_T("SYSAlert")					// 각종 경고 처리 system name
#define NAME_SYSTEM_TORQUEDD			_T("SYSTorqueDD")				// KM6015통신 : Yaskawa(舊 Yokokawa) DD Motor Torque 측정용 
#define NAME_SYSTEM_TORQUESERVO			_T("SYSTorqueServo")			// Panasonic통신 : Panasonic Servo Motor Torque 측정용 
#define NAME_SYSTEM_ACCURA				_T("SYSAccura")				// Accura 2350 통신 : 전력량계 전력/전압 측정용 
#define NAME_SYSTEM_QualWarning			_T("SYSQualWarning")			// 품질 경고 시스템 
#define NAME_SYSTEM_DataReport			_T("SYSDataReport")			// 상위 CIM Data 전송 담당 
#define NAME_SYSTEM_VacRead				_T("SYSVacRead")				// SMC A/D 통신 : 공압값을 읽어온다.
#define NAME_SYSTEM_GmsRead				_T("SYSGmsRead")				// GMS 계측기 통신 : 접지 저항값을 읽어온다.
#define NAME_SYSTEM_TempRead			_T("SYSTempRead")				// OMRON 온도조절기 통신 : PC-Rack 온도를 읽어온다.
#define NAME_SYSTEM_FDCWrite			_T("SYSFDCWrite")				// FDC 정보 저장 시스템
#define NAME_SYSTEM_RobotIF				_T("SYSRobotIF")				// 무인화 Robot Interface 시스템


// 기본 Base Model
#define NAME_MODEL_BASE					_T("BaseModel")					// 기본이 되는 Model

// 각종 파일이름
#define NAME_FILE_DEFECT_LIST			_T("DefectList.ini")				// Defect list (작업자 입력)
#define NAME_FILE_LOSSTIME_LIST			_T("LosstimeList.ini")			// 비가동 항목 list
#define NAME_FILE_LOSSTIME_DATA			_T("LosstimeData.ini")			// 비가동 현황 저장 file
#define NAME_FILE_USER_LIST				_T("UserList.ini")				// 작업자 계정관리 List
#define NAME_FILE_USER_LIST_BK			_T("UserList_BK.ini")			// 작업자 계정관리 List_BK
#define NAME_FILE_PRODUCT_DATA			_T("ProductData.ini")			// 생산량 저장 file
#define NAME_FILE_ALARM_LIST			_T("AlarmCode.ini")				// Alarm 내역 저장 File	//2014/09/24 JH-PARK 
#define NAME_FILE_REPAIR_ALARM			_T("RepairAlarm.ini")			// Alarm 조치이력 정보 File;
#define NAME_FILE_REPORT_TACTTIME		_T("TACTTimeData.ini")			// TACT Time 취합 데이터 저장 File
#define NAME_FILE_REPORT_PRODECT		_T("ProductData.ini")			// 생산량 취합 데이터 저장 File
#define NAME_FILE_RUNTIME_DATA			_T("RuntimeData.ini")			// 실행상태 저장 file
#define NAME_FILE_CIRCLE_DEFECT			_T("DispDefectList.ini")		// Rject Defect 중 Circle을 표시해 주 Defect list 저장 file
#define NAME_FILE_EXCEPT_DEFECT			_T("ExceptDefectList.ini")		// Circle을 표시해 주지 않을 Defect list 저장 file
#define NAME_FILE_TPSYSTEM				_T("TPSystem.ini")				// ? TPM Code를 임시 보관하는 파일 ??
#define NAME_FILE_EXCEPT_CELL			_T("CellIDError.ini")			// 실행시 제외시킬 Cell ID list file, \INI\ 하위에 존재
#define NAME_FILE_WARNING_LIST			_T("QualDefectList.ini")		// 경고 시스템에 적용 할 Defect list
#define NAME_FILE_WARNING_AREA			_T("QualAreaData.ini")			// 경고 시스템에 적용 할 영역 데이터 파일
#define NAME_FILE_WARNING_DEFECT		_T("QualDefectData.ini")		// 경고 시스템에 적용 할 Defect 발생 데이터 파일
#define NAME_FILE_JUDGE_CELL_ID			_T("JudgeCell_ID.ini")			// 판정 완료된 Cell ID를 기록하는 파일
#define NMAE_FILE_EXCEPT_CONFIG			_T("ExceptConfig.ini")			// 예외적인 처리를 위한 각종 설정값 파일, \INI\ 하위에 존재

#define NMAE_FILE_DEFECT_TABLE			_T("NgCodeList.csv")				// Defect Name 매칭을 위한 테이블 파일, \INI\ 하위에 존재, 확장자는 csv

// Align Templete 이미지 파일 이름
#define NAME_FILE_MARK1_CH1_L		_T("MARK1_CH1_L.png")			// Align Mark 채널#1 Left
#define NAME_FILE_MARK1_CH1_R		_T("MARK1_CH1_R.png")			// Align Mark 채널#1 Right
#define NAME_FILE_MARK1_CH2_L		_T("MARK1_CH2_L.png")			// Align Mark 채널#2 Left
#define NAME_FILE_MARK1_CH2_R		_T("MARK1_CH2_R.png")			// Align Mark 채널#2 Right
#define NAME_FILE_MARK2_CH1_L		_T("MARK2_CH1_L.png")			// Align Mark 채널#1 Left
#define NAME_FILE_MARK2_CH1_R		_T("MARK2_CH1_R.png")			// Align Mark 채널#1 Right
#define NAME_FILE_MARK2_CH2_L		_T("MARK2_CH2_L.png")			// Align Mark 채널#2 Left
#define NAME_FILE_MARK2_CH2_R		_T("MARK2_CH2_R.png")			// Align Mark 채널#2 Right


#define NAME_FILE_ALIGN_CH1_L		_T("CH1_L.jpg")					// Channel#1 왼쪽 Align 이미지
#define NAME_FILE_ALIGN_CH1_R		_T("CH1_R.jpg")					// Channel#1 오른쪽 Align 이미지
#define NAME_FILE_ALIGN_CH2_L		_T("CH2_L.jpg")					// Channel#2 왼쪽 Align 이미지
#define NAME_FILE_ALIGN_CH2_R		_T("CH2_R.jpg")					// Channel#2 오른쪽 Align 이미지
#define NAME_FILE_UVMCR_CH1			_T("MCR_1.jpg")					// Channel#1 UV MCR 이미지
#define NAME_FILE_UVMCR_CH2			_T("MCR_2.jpg")					// Channel#2 UV MCR 이미지

#define NAME_FILE_IMAGE_PREALIGN	_T("PreAlign.jpg")				// PreAlign Unit 이미지
#define NAME_FILE_IMAGE_WORKTABLE	_T("WorkTable.jpg")				// WorkTable Unit 이미지
#define NAME_FILE_IMAGE_AVCAMERA	_T("AVCamera.jpg")				// AVCamera Unit 이미지
#define NAME_FILE_IMAGE_DOORFRONT	_T("DoorFront.jpg")				// Door open용 설비 정면 이미지
#define NAME_FILE_IMAGE_DOORREAR	_T("DoorRear.jpg")				// Door open용 설비 후면 이미지

//#define NAME_FILE_IMAGE_WARNING		_T("PMWarning.jpg")			// PM 화면 경고 그림
#define NAME_FILE_IMAGE_WARNING		_T("PMWarning")					// PM 화면 경고 그림
#define NAME_FILE_IMAGE_RECIPE		_T("RecipeView.png")			// Recipe용 그림

#define NAME_FILE_FDC_DATA			_T("FDC_SV.par")				// Data PC에서 읽어갈 FDC값 저장 파일 이름, 2015-07-31


//20170118 byskcho
#define NAME_FILE_OPTION            _T("Option.ini")
#define NAEM_FILE_SYSTEM            _T("System.ini")


// Image log folder
#define NAME_DIR_IMAGE_ALIGN		_T("\\ImgLog\\Align")			// Align Image log를 남기기 위한 폴더
#define NAME_DIR_IMAGE_MCR			_T("\\ImgLog\\MCR")				// UV-MCR Image log를 남기기 위한 폴더


// 아래 내용은 MCMain의 Profile內 변수로 변경되었다.
// #define NAME_DIR_LOSSTIME		_T("D:\\SIRIUS\\LOG\\MAIN")				// 비가동 Log 폴더
// #define NAME_DIR_CELLLOG			_T("D:\\AUTO_TEST\\LOG")				// Cell Log 폴더
// #define NAME_DIR_TACTLOG			_T("D:\\Machine_Data\\TactTime_Data")	// TACT Time Log 폴더, 2014-10-14, 백윤기S 지정
// #define NAME_DIR_ALARMLOG		_T("D:\\Machine_Data\\Alarm_Data")		// Alarm 발생 Log 폴더, jhLee
// #define NAME_DIR_DATAPC_CELL		_T("D:\\UB-AMT\\DATA_PC\\CELL")			// Data PC로 전송 할 Cell 임시 정보 (Tack-out 정보)
// #define NAME_DIR_DATAPC_FDC		_T("D:\\UB-AMT\\DATA_PC\\FDC")			// Data PC로 전송 할 FDC 실시간 정보 


// Recipe key
#define NAME_KEY_ALIGN_MARK			_T("AlignMark")					// Align Mark 중심 좌표

#define NAME_VALUE_DUMMY_DATE		_T("0000-00-00 00:00:00:000")	// 비어있는 날짜형 값


//2014-12-29, Robot 및 축 이름, RobotInfo.ini 에서 정의한다.
#define NAME_ROBOT_PRE_ALIGN		_T("ROBOT_PRE_ALIGN")			// Pre-Align에서 사용 할 Robot의 이름
#define NAME_ROBOT_WORK_TABLE		_T("ROBOT_TURN_TABLE")			// Work-Table에서 사용 할 Robot의 이름
#define NAME_ROBOT_AV_CAMERA		_T("ROBOT_AV_CAMERA")			// AV-Camera에서 사용 할 Robot의 이름


// 각 축의 이름, AxisInfo.ini에서 각 축의 Section 이름으로 사용되는 값이다.
#define NAME_AXIS_WORK_TABLE_T		_T("WORK_TABLE_T_AXIS")
#define NAME_AXIS_AV_CAMERA_F		_T("AV_CAMERA_F_AXIS")
#define NAME_AXIS_PRE_ALIGN_X		_T("PRE_ALIGN_X_AXIS")
#define NAME_AXIS_AV_CAMERA_Z		_T("AV_CAMERA_Z_AXIS")
#define NAME_AXIS_AV_CAMERA_X		_T("AV_CAMERA_X_AXIS")
#define NAME_AXIS_AV_CAMERA_T		_T("AV_CAMERA_T_AXIS")


// 단위변환
#define _CNV_MSEC_TO_SEC(x)		(x * 0.001)							// 밀리초 -> 초
#define _CNV_SEC_TO_MSEC(x)		(x * 1000)							// 초 -> 밀리초
// 
// 현재 System의 각종 메세지 정의, User Defined Message ...
#define UDMSG_MESSAGE_BASE				(WM_USER + 5000)					// 현재 시스템에서 사용할 사용자정의 메세지 Base
#define UDMSG_STATE_CHANGE				(UDMSG_MESSAGE_BASE + 1)		// 시스템의 상태가 변경되었다.
#define UDMSG_MODEL_CHANGE				(UDMSG_MESSAGE_BASE + 2)		// Recipe model이 변경되었다.
#define UDMSG_USER_CHANGE				(UDMSG_MESSAGE_BASE + 3)		// 사용자가 변경되었다.
#define UDMSG_COMM_STATE				(UDMSG_MESSAGE_BASE + 4)		// 통신 연결 상태가 변경되었다.
#define UDMSG_DISPDATA_UPDATE			(UDMSG_MESSAGE_BASE + 5)		// 각종 정보가 Update 되었다.
#define UDMSG_TEACHKEY_ON				(UDMSG_MESSAGE_BASE + 6)		// TeachKey가 AUTO -> TEACH로 변경되었다.
#define UDMSG_DISPFROM_DEFECT			(UDMSG_MESSAGE_BASE + 7)		// Defect 입력창으로 전환한다.
#define UDMSG_DEFECT_CHANGE				(UDMSG_MESSAGE_BASE + 8)		// Manual 입력의 Defect이 변경되었다.
#define UDMSG_DISP_SUBDIALOG			(UDMSG_MESSAGE_BASE + 9)		// 하위 Dalog를 보여주거나 감춘다.
#define UDMSG_DATAPC_CHANGE				(UDMSG_MESSAGE_BASE + 10)		// DATA PC OnOffline 상태가 변경되었다.
#define UDMSG_OPTION_UPDATE				(UDMSG_MESSAGE_BASE + 11)		// 각종 Option 값들이 변경되었다. (화면 표시용)
#define UDMSG_APPLY_RUNDOWN				(UDMSG_MESSAGE_BASE + 12)		// RUN-Down을 실행시켜준다.
#define UDMSG_CONTACT_CURRENT			(UDMSG_MESSAGE_BASE + 13)		// Loading시 Contact Current에 대한 결과가 도착하였다.
#define UDMSG_TODAY_CHANGE				(UDMSG_MESSAGE_BASE + 14)		// Log 기록 기준 일자가 변하였다
#define UDMSG_SHIFT_CHANGE				(UDMSG_MESSAGE_BASE + 15)		// 작업조가 변경되었다.
#define UDMSG_TACT_UPDATE				(UDMSG_MESSAGE_BASE + 16)		// TACT Time 정보가 Update 되었다.


#define UDMSG_DISPDLG_OPCALL			(UDMSG_MESSAGE_BASE + 20)		// OPCall 요청이 내려왔다.
#define UDMSG_DISPDLG_TERMMSG			(UDMSG_MESSAGE_BASE + 21)		// Termainal Message 요청이 내려왔다.
#define UDMSG_DISPDLG_INTERLOCK			(UDMSG_MESSAGE_BASE + 22)		// Interlock 요청이 내려왔다.
#define UDMSG_DISPDLG_CELLINFO			(UDMSG_MESSAGE_BASE + 23)		// 작업자 셀 정보 입력창 띄우기
#define UDMSG_LOGIN_RESPONSE			(UDMSG_MESSAGE_BASE + 24)		// 작업자 로그인/아웃 인증 요청에대한 응답이 내려왔다.

#define UDMSG_UPDATE_TEMPDATA			(UDMSG_MESSAGE_BASE + 30)		// 온도값이 변화되었다.
#define UDMSG_UPDATE_ABSDATA			(UDMSG_MESSAGE_BASE + 31)		// 절대치 엔코더 값이 값이 변화되었다.

#define UDMSG_DISP_LAN_CHANGE			(UDMSG_MESSAGE_BASE + 40)		//  프로그램 표시언어가 변화되었다.
#define UDMSG_DISP_QUAL_WARNING			(UDMSG_MESSAGE_BASE + 41)		//  품질경고 화면을 보여주도록 한다.

#define UDMSG_COUNT_UPDATE				(UDMSG_MESSAGE_BASE + 44)		

#define UDMSG_TACT_CHANGE				(UDMSG_MESSAGE_BASE + 45)	

#define UDMSG_MCR_UPDATE                (UDMSG_MESSAGE_BASE + 46)	

#define UDMSG_UPPEREQP_COM_UPDATE      (UDMSG_MESSAGE_BASE + 47)

#define UDMSG_DCR_UPDATE                (UDMSG_MESSAGE_BASE + 48)	

#define UDMSG_CZONE_UPDATE				(UDMSG_MESSAGE_BASE + 49)	  //2017-03-03,skcho, CZONE정보를 보여주기 위해
 
#define UDMSG_TMDDOWNLOAD_UPDATE		(UDMSG_MESSAGE_BASE + 50)	 //2017-03-08, skcho

#define UDMSG_TMDDOWNLOAD_LOG           (UDMSG_MESSAGE_BASE + 51)

#define UDMSG_JOBCHANGE_POPUP           (UDMSG_MESSAGE_BASE + 52)

#define UDMSG_IDLECHAGNE_UPDATE			(UDMSG_MESSAGE_BASE + 53)	 

#define UDMSG_RECIPE_UPDATE				(UDMSG_MESSAGE_BASE + 54)	 

#define UDMSG_CONTACT_JIGCNT			(UDMSG_MESSAGE_BASE + 55)	

#define UDMSG_IDLECHAGNERUN_UPDATE		(UDMSG_MESSAGE_BASE + 56)

#define UDMSG_IDLEAGVINSTART_UPDATE		(UDMSG_MESSAGE_BASE + 57)

#define UDMSG_IDLEAGVINEND_UPDATE		(UDMSG_MESSAGE_BASE + 58)

#define UDMSG_IDLEAGVOUTSTART_UPDATE		(UDMSG_MESSAGE_BASE + 59)

#define UDMSG_IDLEAGVOUTEND_UPDATE		(UDMSG_MESSAGE_BASE + 60)

#define UDMSG_DISPDLG_AGV   (UDMSG_MESSAGE_BASE + 61)

//2018wjp
#define UDMSG_UPDATE_LOAD_CELL   (UDMSG_MESSAGE_BASE + 62)
#define UDMSG_UPDATE_ONE_CELL    (UDMSG_MESSAGE_BASE + 63)
#define UDMSG_REMOVE_ALL_CELL    (UDMSG_MESSAGE_BASE + 64)
#define UDMSG_REMOVE_ONE_CELL    (UDMSG_MESSAGE_BASE + 65)
#define UDMSG_DCR_FAIL           (UDMSG_MESSAGE_BASE + 66)	
#define UDMSG_CIM_BACK_FAIL      (UDMSG_MESSAGE_BASE + 67)	
// Defect 데이터를 저장하는 구조체, ini file로 된 defect list를 읽어와서 프로그램 내부에서 사용된다.
// FormDefect, DlgDefectSelect, DlgDefectEdit 참조.
#define DELIMITER_DEFECT				'>'								// Defect 및 LossTime 설정값 관련 문자열 분리 구분자

enum
{	// 언어 설정
	eLANG_KOREAN = 0,
	eLANG_ENGLISH,
	eLANG_VIETNAM,
	eMAX_Language,
};

//wjp2018
struct Cell_Coord
{
	int row;
	int col;
	CString strCode;
};

// D-Zone에서 작업자의 Defect 입력을 위한 구조체
typedef struct _TDefectData {
	// enum { eMAX_Language = 3 };		// 최대 지원 언어

	CString	sCode;						// Defect Code
	CString	sLine;						// 데이터 원본
	CString	sDefect[eMAX_Language];	// 분리된 Defect 이름

	_TDefectData() { 
		sCode			= _T("");
		sLine			= _T("");
		for (int i=0; i < eMAX_Language; i++) sDefect[i] = _T("");
	};

	// 대입연산자
	_TDefectData& operator = (const _TDefectData &p) {
		sCode = p.sCode;
		sLine	= p.sLine;
		for(int i=0; i<eMAX_Language; i++) sDefect[i] = p.sDefect[i];
		return *this;
	};

} TDefectData;


// Defect을 그룹별로 관리하기위한 구조체
typedef struct _TDefectList {
	enum {	eMAX_GoupCount			= 12,								// 최대 12개 Defect 그룹	
			eMAX_DefectCount		= 40,								// 그룹당 최대 40개의 defet 존재
			eMAX_ShortcutCount		= 8,								// 단축 defect 수량, 8개
	};		
	
	UINT			nShortcutCount;										// 단축 버튼 정의 수량
	TDefectData		rShortcut[ eMAX_ShortcutCount ];					// 단축 버튼 defect 저장

	UINT			nGroupCount;										// 그룹 수
	CString			sGroupName[eMAX_GoupCount][eMAX_Language];			// 각 그룹의 이름
	UINT			nDefectCount[eMAX_GoupCount];						// 각 그룹당 Defect  수량
	TDefectData		rDefectData[eMAX_GoupCount][eMAX_DefectCount];		// defect data가 보관되는 배열

	TDefectData*	GetDefectData(int nGroup, int nIdx) { return &(rDefectData[nGroup][nIdx]); };		// Defect Datd pointer 획득
	TDefectData*	GetShortcut(int nIdx) { return &(rShortcut[nIdx]); };		// Shortcut Defect Datd pointer 획득

	_TDefectList() {	nShortcutCount = 0; for (int i=0; i<eMAX_GoupCount; i++) nDefectCount[i] = 0;	};		// 초기화

} TDefectList;


// 비가동 로그에서 사용되는 비가동 사유 
// 최 하위 3rdClass 데이터
// PM>000>Shift점검>프로그램 점검>Check Program
typedef struct _TLosstimeData {
	// enum { eMAX_Language = 2 };		// 최대 지원 언어

	CString	sCode;						// 비가동 Code, 현재는 미사용, 향후 사용예정
	CString	sLine;						// 데이터 원본, 단축버튼에 활용
	CString	sReason[eMAX_Language];	// 분리된 비가동 사유 이름

	_TLosstimeData() { 
		sCode			= _T("");
		sLine			= _T("");
		for (int i=0; i < eMAX_Language; i++) sReason[i] = _T("");
	};

	// 대입연산자
	_TLosstimeData& operator = (const _TLosstimeData &p) {
		sCode = p.sCode;
		sLine	= p.sLine;
		for(int i=0; i<eMAX_Language; i++) sReason[i] = p.sReason[i];
		return *this;
	};

} TLosstimeData;


// 중간 2nd Class Data
typedef struct _TLoss2ndClass {
	enum {	//eMAX_Language = 2,			// 최대 지원 언어
				eMAX_3rdCount = 24,	};		// 하위 항목 수

	CString	sLine;						// 데이터 원본, 단축버튼에 활용
	CString	sName[eMAX_Language];	// 2nd Class name

	UINT		n3rdCount;								// 3rd class 수
	TLosstimeData r3rdClass[eMAX_3rdCount];	// 3rd Class data

	_TLoss2ndClass() {			// 초기화
		sLine			= _T("");
		n3rdCount	= 0;
		for (int i=0; i < eMAX_Language; i++) sName[i] = _T("");
	};

} TLoss2ndClass;


// 1st Class Data
typedef struct _TLoss1stClass {
	enum {	eMAX_2ndCount = 8,	};				// 하위 항목 수

	CString	sLine;									// 데이터 원본, 단축버튼에 활용
	CString	sName;									// 1st Class name

	UINT		n2ndCount;								// 2nd class 수
	TLoss2ndClass r2ndClass[eMAX_2ndCount];	// 2nd Class data

	_TLoss1stClass() {			// 초기화
		sLine			= _T("");
		sName			= _T("");
		n2ndCount	= 0;
	};

} TLoss1stClass;


// 비가동 사유를 그룹별로 관리하기위한 구조체
typedef struct _TLosstimeList {
	enum {	eMAX_1stCount			= 8,			// 최대 8개 1st class 그룹	
				eMAX_ShortcutCount	= 6,			// 단축 버튼 수량, 6개
	};		

	//2014-12-24, jhLee TPSystem Code 보고시 필요
	// 자동으로 처리해하는 3가지 예외 처리
	CString			sAutoRunDownCode;						// AUTO RUN DOWN 선택 항목
	CString			sOPLossCode;							// OP LOSS 선택 항목
	CString			sTactTimeOver;							// TATC TIME OVER 선택 항목


	UINT			nShortcutCount;						// 단축 버튼 정의 수량
	TLosstimeData	rShortcut[ eMAX_ShortcutCount ];	// 단축 버튼 defect 저장

	UINT			nCount_1st;								// 1st class 그룹 수
	TLoss1stClass	rLosstime[eMAX_1stCount];			// 1st Class의 Data

	_TLosstimeList() {	nShortcutCount = 0; nCount_1st = 0;	};		// 초기화
	TLosstimeData*	GetDefectData(int n1st, int n2nd, int n3rd) { return &(rLosstime[n1st].r2ndClass[n2nd].r3rdClass[n3rd]); };		// losstime Data pointer 획득
	TLosstimeData*	GetShortcut(int nIdx) { return &(rShortcut[nIdx]); };		// Shortcut Losstime Data pointer 획득

} TLosstimeList;


// 비가동 Log를 남기기 위한 데이터 기록
typedef struct _TLosstimeRecord {

	BOOL			bPrevDay;						// TRUE:강제로 이전 날짜로 기록한다, FALSE:오늘 날짜로 기록한다.
	int				nMode;							// 현재 비가동 기록 모드 0: STOP으로 전환, 1:Auto로 전환

	// 선택된 내용들
	CString			sReasonLine;					// 선택된 전체 Code 내용
	CString			s1stClass;						// 각 Class별 선택 내용
	CString			s2ndClass;
	CString			s3rdClass;
	CString			sMESCode;						// 상위 보고용 코드, 현재 미사용
	CString			sID;								// Stop 시킨 작업자 ID
	CString			sIDNo;							// 작업자 사원번호;
	CString			sAlarmType;						// Alarm 여부, 중/경 알람여부
	CString			sAlarmCode;						// Alarm Code
	CString			sRepairCode;					// MD/BM의 경우 조치내역
	CString			sUserMsg;						// 사용자 정의 입력
	float			fWaitTime;						// 알람발생 ~ 고장조치 시작까지
	float			fRepairTime;					// 실제 조치가 일어난 시간
	float			fDuration;						// 실제 설비가 멈춘 시간

	//2014-09-11 추가 (백윤기S 요청)
	CString			sTPSystemName;					// TP System loss name
	CString			sTPSystemCode;					// TP System Loss Code

	SYSTEMTIME		tmStartTime;					// 정지 시작 시각
	SYSTEMTIME		tmResetTime;					// 인지 시각
	SYSTEMTIME		tmEndTime;						// 정지 종료시각

	// ini file로 저장하기 위한 변수
	CString			sStartTime;						// 문자열로 변경된 시작 시간
	CString			sResetTime;						// 문자열로 변경된 인지 시간

	double			dbAddLossTime;					// 추가적인 미가동 시간


	void Clear()
	{
		bPrevDay		= FALSE;						//  FALSE:오늘 날짜로 기록한다.
		nMode			= 0;							// 현재 비가동 기록 모드 0: STOP으로 전환, 1:Auto로 전환
	
		sReasonLine		= _T("");					// 선택된 전체 Code 내용
		s1stClass		= _T("");					// 각 Class별 선택 내용
		s2ndClass		= _T("");
		s3rdClass		= _T("");
		sMESCode			= _T("");					// 상위 보고용 코드, 현재 미사용
		sID				= _T("");					// Stop 시킨 작업자 ID
		sIDNo				= _T("");					// 작업자 사원번호;
		sAlarmType		= _T("");					// Alarm 여부, 중/경 알람여부
		sAlarmCode		= _T("");					// Alarm Code
		sRepairCode		= _T("");					// MD/BM의 경우 조치내역
		sUserMsg			= _T("");					// 사용자 정의 입력
		fWaitTime		= 0.0;						// 알람발생 ~ 고장조치 시작까지
		fRepairTime		= 0.0;						// 실제 조치가 일어난 시간
		fDuration		= 0.0;						// 실제 설비가 멈춘 시간

		sTPSystemName	= _T("UNDEFINDED");					// TP System loss name
		sTPSystemCode	= _T("UNDEFINDED");					// TP System Loss Code

		::GetLocalTime( &tmStartTime );			// 정지 시작 시각
		::GetLocalTime( &tmResetTime );			// 인지 시각
		::GetLocalTime( &tmEndTime );				// 정지 종료시각

		sStartTime		= _T("");					// 문자열로 변경된 시작 시간
		sResetTime		= _T("");					// 문자열로 변경된 인지 시간

		dbAddLossTime	= 0.0;						// 추가적인 미가동 시간
	};

	_TLosstimeRecord () { Clear(); };				// 생성자

} TLosstimeRecord;





/*	----------------------------------------------- */
//2014/10/08 JH-PARK 

// 최 하위 3rdClass 데이터
typedef struct _TRepairAlarmData {
	// enum { eMAX_Language = 2 };		

	CString	sCode;						
	CString	sLine;						
	CString	sReason[eMAX_Language];	

	_TRepairAlarmData() { 
		sCode			= _T("");
		sLine			= _T("");
		for (int i=0; i < eMAX_Language; i++) sReason[i] = _T("");
	};

	// 대입연산자
	_TRepairAlarmData& operator = (const _TRepairAlarmData &p) {
		sCode = p.sCode;
		sLine	= p.sLine;
		for(int i=0; i<eMAX_Language; i++) sReason[i] = p.sReason[i];
		return *this;
	};
} TRepairAlarmData;


// 중간 2nd Class Data
typedef struct _TRepair2ndClass {
	enum {	// eMAX_Language = 2,			// 최대 지원 언어
		eMAX_3rdCount = 24,	};		// 하위 항목 수

	CString	sLine;						// 데이터 원본, 단축버튼에 활용
	CString	sName[eMAX_Language];	// 2nd Class name

	UINT		n3rdCount;								// 3rd class 수
	_TRepairAlarmData r3rdClass[eMAX_3rdCount];	// 3rd Class data

	_TRepair2ndClass() {			// 초기화
		sLine			= _T("");
		n3rdCount	= 0;
		for (int i=0; i < eMAX_Language; i++) sName[i] = _T("");
	};

} TRepair2ndClass;

// 1st Class Data
typedef struct _TRepair1stClass {
	enum {	eMAX_2ndCount = 8,	};				// 하위 항목 수

	CString	sLine;									// 데이터 원본, 단축버튼에 활용
	CString	sName;									// 1st Class name

	UINT		n2ndCount;								// 2nd class 수
	TRepair2ndClass r2ndClass[eMAX_2ndCount];	// 2nd Class data

	_TRepair1stClass() {			// 초기화
		sLine			= _T("");
		sName			= _T("");
		n2ndCount	= 0;
	};

} TRepair1stClass;

// Alarm 조치사유를 그룹별로 관리하기위한 구조체
typedef struct _TRepairAlarmList {
	enum {	eMAX_1stCount			= 8,			// 최대 8개 1st class 그룹	
				eMAX_ShortcutCount	= 6,			// 단축 버튼 수량, 6개
			};		

	UINT				nShortcutCount;						// 단축 버튼 정의 수량
	TRepairAlarmData	rShortcut[ eMAX_ShortcutCount ];	// 단축 버튼 defect 저장

	UINT				nCount_1st;								// 1st class 그룹 수
	TRepair1stClass	rRepairAlarm[eMAX_1stCount];			// 1st Class의 Data

	_TRepairAlarmList() {	nShortcutCount = 0; nCount_1st = 0;	};		// 초기화
	TRepairAlarmData*	GetDefectData(int n1st, int n2nd, int n3rd) { return &(rRepairAlarm[n1st].r2ndClass[n2nd].r3rdClass[n3rd]); };		// losstime Data pointer 획득
	TRepairAlarmData*	GetShortcut(int nIdx) { return &(rShortcut[nIdx]); };		// Shortcut Losstime Data pointer 획득

} TRepairAlarmList;




// Alarm 발생 내역을 Log 기록하기위한 구조체
typedef struct _AlarmRecord {

	CString			sCode;							// Alarm Code
	CString			sName;							// Alarm 내용
	CString			sParts;							// 발생 부분
	int				nLevel;							// Alarm Level

	SYSTEMTIME		tmStartTime;					// Alarm 발생 시각 시작 시각
	SYSTEMTIME		tmResetTime;					// Alarm 인지 시각
	SYSTEMTIME		tmLossStartTime;				// Loss time기록용 Alarm 발생 시각 
} TAlarmRecord;


// NG Code list와 Defect code와의 매칭을 위한 Table 구조체
typedef struct _TDefectNameTable
{
	CString		sKey;						// 검색할 때 사용되는 Key값, 공백제거, 대문자 변환

	CString		sName1;					// 파일에서 읽어들인 Defect 글자
	CString		sName2;					// 영문 및 한글 defect
	CString		sName3;					
	CString		sCode;					// MES 보고용 Defect Code

} TDefectNameTable;
 typedef CTypedPtrMap<CMapStringToPtr, CString, TDefectNameTable*> CMapStringToDefectName;


// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==

//
// SysWarning에서 사용하는 경보 Defect data
//
// 품질 경고용 개별 Defect 정보
typedef struct _DefectItem {
	CString		sName;								// Defect Name, 검사PC에서 보내온 
	CString		sCode;								// Defect Code, Defect List에서 검색을 통해 찾아올 수 있다.

	_DefectItem() { sName = _T(""); sCode = _T(""); };
} TDefectItem;


// 품질 경고용 자료구조

// 구간별 영역 자료구조
typedef struct _QualArea {

	int			nBoundX1;							// X축 경계 시작
	int			nBoundX2;							// X축 경계 끝
	int			nBoundY1;							// Y축 경계 시작
	int			nBoundY2;							// Y축 경계 끝

	CString		sLastCellID;						// 최근에 Defect이 발생한 Cell ID, 동일 영역에 대한 중복 누적 방지
	int			nErrorCount;						// 발생한 Defect 건수 보관

	int			nAlertLevel;						// 경고 발생 수위, 0:Normal, 1:주의, 2:경고, 3:위험
	SYSTEMTIME	tmEvent;				// 경보가 발생된 시각

	//d CList <TDefectItem, TDefectItem> lstDefectItems;		// 발생된 Defect 지정

	void Clear() { 
		sLastCellID = _T("");	
		nErrorCount = 0;
		nAlertLevel = 0;
	//d lstDefectItems.RemoveAll(); 
	};

	void ClearBound()			// 영역을 초기화 한다.
	{
		nBoundX1 = 0; 
		nBoundY1 = 0; 
		nBoundX2 = 0; 
		nBoundY2 = 0; 
	};

	_QualArea() 
	{  
		ClearBound();
		Clear(); 
	};

	~_QualArea() 
	{ 
		// lstDefectItems.RemoveAll(); 		
	};

} TQualArea;



// 품질 검사를 위한 지정 Defect 관련 데이터 관리 
typedef struct _QualItem {
	CString		sCode;					// Defect Code
	CString		sName[eMAX_Language];	// 언어별 Defect 이름
	CString		sStepName;				// Feedback해줄 공정명

	int			nMode;					// 동작 모드, 0:미사용, 1:발생횟수로 경보, 2:발생율 %로 경보
	int			nLimitCount;			// 경보를 울리기위한 제한 값, 발생횟수
	int			nLimitRatio;			// 경보를 울리기 위한 제한 값, 발생 비율 %

	// 실행중의 누적 데이터, 별도의 파일로 관리한다.
	int			nIndex;					// 파일 저장관리를 위한 Index

	int			nAlertLevel;			// 경고 발생 수위, 0:Normal, 1:주의, 2:경고, 3:위험
	int			nCount;					// 발생한 빈도수
	double		dbRatio;				// 계산된 발생률

	SYSTEMTIME	tmEvent;				// 경보가 발생된 시각

	// 발생 횟수를 초기화 한다.
	void ClearCount()					// 발생 빈도수 Clear
	{
		nCount		= 0;
		nAlertLevel = 0; 
	};

	// 사용여부를 초기화하여 해당 항목을 비운다.
	void ClearItem()
	{
		sCode.Empty();					// 내용 삭제
		sStepName.Empty();
		for (int i=0; i<eMAX_Language; i++) sName[i].Empty();

		nMode = 0;						// 미사용 설정
		nLimitCount = 3;
		nLimitRatio = 3;

		ClearCount();					// 발생 내역도 초기화 한다.
	};
} TQualItem;
typedef CArray<TQualItem, TQualItem&> TArrayQualItem;				// 점검대상 Defect 관리용 CArray


// 품질경보 메세지를 보관하기 위한 구조체
typedef struct _QualMsg {

	int		nMode;						// 어떠한 경보의 종류인가 ?
	int		nIndex1;					// 발생한 곳의 Index, Area인경우 X축 Index, Defect 인 경우 해당 Defect의 배열 Index
	int		nIndex2;					// Area의 Y축 Index

	CString	sName;						// Area 명 or Defect 명
	CString sStep;						// Step 명
	int		nCount;						// 발생한 수치 : 카운트 혹은 불랼율
	int		nReason;					// 원인, 0 : 카운트 초과 / 1 : 불량률 초과

	CString		sMsg;					// 경고 내용
	SYSTEMTIME	tmEvent;				// 경보가 발생된 시각

	_QualMsg () {
		nMode		= 0;
		nIndex1		= 0;
		nIndex2		= 0;

		sMsg.Empty();
	};

} TQualMsg;
typedef CArray<TQualMsg, TQualMsg&> TArrayQualMsg;					// 경고 메세지 관리용 CArray


// C-Zone에서 검출한 Defect 정보를 저장하기 위한 구조체, 
// CellInfo에서 선언한 뒤 SysVIMonitor에서 정보를 누적시킨다.
typedef struct _QualDefect 
{
	int x;
	int y;

	CString DefectName;				// 해당 Defect의 이름
} TQualDefect;
typedef CList<TQualDefect, TQualDefect&> TListQualDefect;


// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==




// 기존 GAON ConstCollection.h에서 발췌
// 판정에 대한 정의
struct CONST_JUDGE_NAME
{
	CString strName;

	enum
	{
		JUDGE_MCR = 0,
		JUDGE_CONTACT,
		JUDGE_CELL_LOADING,
		JUDGE_SLEEP,
		JUDGE_ID,
		JUDGE_WHITE,
		JUDGE_TSP,
		JUDGE_MTP,
		JUDGE_MANUAL_A,
		JUDGE_MANUAL_D,
		JUDGE_VISION,
		JUDGE_PG_ALARM,
		JUDGE_FINAL_CLASS,
		JUDGE_MAX,
	};
};

const CONST_JUDGE_NAME CONST_JUDGE_LIST[] =
{
	_T("MCR")			,
	_T("CONTACT")		,
	_T("CELL_LOADING"),
	_T("SLEEP")			,
	_T("ID_CHECK")		,
	_T("WHITE")			,
	_T("TSP")			,			
	_T("MTP")			,			
	_T("MANUAL_A")		,
	_T("MANUAL_D")		,
	_T("VISION")		,
	_T("PG ALARM")	,
	_T("FINAL CLASS")	
};

// D-Zone에서 수동 검사시 Cell에 문자를 표시하기위한 설정값들 MCMain에서 Recipe로 관리된다.
typedef struct 
{
	//double	m_dWindowWidth;
	//double	m_dWindowHeight;
	//double	m_dGlassWidth;
	//double	m_dGlassHeight;
	//double	m_dGlassToWindow;
	//double	m_dFpcb;
	//double	m_dAMarkX;
	//double	m_dAMarkY;

	CString	sModelID;					// VI PC에 설정된 Model ID, Recipe의 Model 이름과 동일해야 한다.

	int		nDotCountX;					// Recipe 편집 화면에서 입력한다.
	int		nDotCountY;
	int		nDotSizeX;					// VI PC에서 Inform.ini 파일에 의해 값을 읽어온다.
	int		nDotSizeY;
	int		nDotCount;
	int		nFontSize;					// 글씨 출력 크기

	int		nResolutionX;				// D-Zone 작업자 검사시 검사 결과를 출력하기 위한 해상도 데이터
	int		nResolutionY;
	int		nDZonePtnMax;				// D-Zone 작업자 검사시 Back/Next 제어용 패턴 수 
	double	dbMarkDistance;			// 두 Align Mark간 거리, Align 동작시 반드시 필요 !
	
	BOOL	bPOLRemoved;				// POL을 부착하지 않은 제품인가 ? 567 모델은 미부착으로 상부조명을 피신시켜야한다.

} TCellDisp;



// UB-AMT 시스템 내에서 사용하는 상수 정의
enum {	// 통신
	eCOMM_MyPC				= 0,			// 0. 운영PC, 내 PC이므로 신호기PC의 Server 설정할 때 사용한다.
	eCOMM_SignalPC,							// 2. 신호기 PC
	eCOMM_DataPC,							// 3. 데이터 PC 와의 통신
	//kjpark 20161006 MCR 상하 수정
	eCOMM_Lower_UVMCR,							// 4. UVMCR #1과의 통신
	eCOMM_Upper_UVMCR,
	eCOMM_AccuraGPS,						// 6, GPS 전력량계 Accura 1
	eCOMM_AccuraUPS,						// 7, UPS 전력량계 Accura 2
	eCOMM_AVCamera1,						// AVCamera PC 1 와의 통신
	eCOMM_AVCamera2,						// AVCamera PC 2 와의 통신
	eCOMM_MITAlign,							//  MITPreAlign
	eCOMM_MITEdgeAlign,						//  MITEdgeALign
	eCOMM_RobotAlign,						//  Robot
	eCOMM_LdRobot,						//  Robot
	eCOMM_UdRobot,						//  Robot
	eCOMM_DCR,
	eCOMM_Tray_Align,
	eCOMM_TMDDownLoad,                      //2017-03-08, skcho, tmd download할 파일이 있으면
	eMAX_COMMCount,							// 8. TCP/IP 통신 수량

};


enum {				// CIM 및 각종 Cell관련 정보를 처리하기 위한 상수 선언

	   //20170118 byskcho
	    eTIMER_Monitoring,						// 일정 시간마다 수행할 내용들
		// CellData.h를 포함한 CIM data 관련된곳에서 사용한다.
		// 이전과의 호환성을 위해 4개까지 선언을 한다.
		eIDX_JIG_1			= 0,
		eIDX_JIG_2,
		eMAX_JigCount,							// Jig수를 이전과의 호환을 위해 4개로 선언 ? -> 실제 사용하는 2개만 사용한다.
		// eIDX_JIG_3,
		// eIDX_JIG_4,

		eIDX_CH_1			= 0,				// 실제로 사용하는 Channel 수만 사용한다.
		eIDX_CH_2,
		eMAX_ChCount,							// 채널 수
		eIDX_CH_ALL			= eMAX_ChCount,		// 양족 채널 모두를 지칭 (JIG 진공 관련 I/O 명령에 사용)


		// 각 Cell data를 관리하는 배열 및 신호기에서의 일련 번호 index
		eIDX_JIG1_CH1		= 0,
		eIDX_JIG1_CH2,
		eIDX_JIG2_CH1,
		eIDX_JIG2_CH2,
		eMAX_JIG_CH_Count,



		// 2015-07-22, jhLee, GMS 기능 추가
		eIDX_GMS_CH1		= 0,				// Main 접지
		eIDX_GMS_CH2,							// A-Zone Jig (Jig #1)
		eIDX_GMS_CH3,							// C-Zone Jig (Jig #2)
		eIDX_GMS_CH4,							// Work Table
		eIDX_GMS_CH5,							// Worker Earth strap
		eMAX_GMS_ChCount,						// GMS 모니터링 채널 수



		// 2015-02-17, jhLee, 품질 경보시스템에 적용 할 구획 수 지정, 
		eMAX_WARNING_X		= 9,				// 가로 구획 수 , 1 ~ 9
		eMAX_WARNING_Y		= 15,				// 세로 구획 수, A ~ O (15)
		eMAX_WARNING_Defect	= 25,				// 경고 기능을 사용 할 Defect 종류

		eMODE_WARNING_None	= 0,				// 감시를 하지 않는다.
		eMODE_WARNING_Count	= 1,				// 발생 수량으로 경보를 울린다.
		eMODE_WARNING_Ratio = 2,				// 발생율로 경부를 울린다.

		eMODE_WARNING_Area	= 0,				// 영역별 Defect 발생 경고
		eMODE_WARNING_Defect= 1,				// Defect 별 발생 경고

		eIDX_WARNING_Normal = 0,				// 경고가 발생하지 않은 정상 상태
		eIDX_WARNING_Alert	= 1,				// 경보가 발생한 상태


		// 2015-04-17, jhLee, Login/out 서버 인증용
		eMODE_LOGINOUT_Logout	= 0,			// Log out에 대한 응답
		eMODE_LOGINOUT_Login	= 1,			// Log in에 대한 응답

		eSET_LOGINOUT_Fail		= 0,			// Log in/out 실패
		eSET_LOGINOUT_Pass		= 1,			// Log in/out 성공



		// 시퀀스 처리상 정의된 논리적인 Zone으로 A, C, D 존으로 나뉜다. 
		eIDX_Zone_A			= 0,				// Cell을 Loading하고 Aling/MCR 작업하는 곳
		eIDX_Zone_C,							// 화상 검사(VI)와 TSP 검사를 수행하는 곳
		eIDX_Zone_D,							// 작업자 육안검사와 Cell Unloading이 이루어지는 곳
		eMAX_ZoneCount,

		// 각종 검사 종류, 해당 검사를 수행할지 여부를 판단하기 위한 검사의 일련번호
		eIDX_INSP_AZoneAlign	= 0,			// A-Zone Align(UV-MCR) 동작 
		eIDX_INSP_CZoneVI,						// C-Zone Vision  검사
		eIDX_INSP_CZoneTECheck,					// C-Zone 신호기 TE_CHECK  검사
		eIDX_INSP_CZoneTSP,						// C-Zone TSP 검사
		eIDX_INSP_DZoneManual,					// D-Zone Manual Defect 검사
		eMAX_INSPCount,							// 검사 종류

		
		// 작업중인 Cell log data 처리용 Index, CMCMain 에서 정의하고 참조한다.
		// MCMain의 nCellDataIndex[] 배열에서 Index로 사용한다.
		eIDX_JOB_Load_CH1		= 0,			// A Zone에서 Loading 처리중인 CH#1 cell data
		eIDX_JOB_Load_CH2,
		eIDX_JOB_VI_GRAB,						// C Zone에서 VI 테스트중인 Cell, GRAB.OK응답을 기다리고 있다.
		eIDX_JOB_TSP_TEST,					// C Zone에서 신호기와 TSP 테스트 중인 Cell
		eIDX_JOB_VI_CLASS,					// C Zone 혹은 D Zone에서 VI 테스 결과를 기다리는  Cell
		eIDX_JOB_Defect_CH1,					// D Zone에서 사용자 Defect 입력 및 Unloading 처리하는 Cell
		eIDX_JOB_Defect_CH2,					
		eMAX_JOBCount,


		// UV MCR 순번
		eIDX_UVMCR_1		= 0,
		eIDX_UVMCR_2,
		eMAX_UVMCRCount,

		// Align mark 순번
		eIDX_MarkLeft		= 0,				// 왼쪽 Align Mark
		eIDX_MarkRight,						// 오른쪽 Align Mark
		eMAX_MarkCount,


		// Align용 Mark 순번 : MkNxCam lib내 Template index
		eIDX_MARK1_CH1_L	= 0,				// 채널#1 왼쪽 마크
		eIDX_MARK1_CH1_R,						// 채널#1 오른쪽 마크
		eIDX_MARK1_CH2_L,						// 채널#2 왼쪽 마크
		eIDX_MARK1_CH2_R,						// 채널#2 오른쪽 마크

		eIDX_MARK2_CH1_L,				// 채널#1 왼쪽 마크
		eIDX_MARK2_CH1_R,						// 채널#1 오른쪽 마크
		eIDX_MARK2_CH2_L,						// 채널#2 왼쪽 마크
		eIDX_MARK2_CH2_R,						// 채널#2 오른쪽 마크

		eMAX_MARKCount,						// 마크 수, 모두 8개

		// Align용 CAM No : Camera Index, MkNxCam Lib내 사용
		eIDX_ALIGN_CAM1	= 0,				// Align Camera #1
		eIDX_ALIGN_CAM2,						// Align Camera #2
		eMAX_ALIGN_CAMCount,					// Align에서 사용하는 Camera 수, 2개


		// Align용 조명 컨트롤러
		eIDX_LIGHT_CH1		= 0,			// 하단 CH#1 조명
		eIDX_LIGHT_CH2,						// 하단 CH#2 조명
		eIDX_LIGHT_UP,						// 상부 조명 CH#3 제어용 (3 채널)
		eIDX_LIGHT_ALL,						// 모든 채널 ( 1 ~ 4 ) 제어용
		eIDX_LIGHT_LOWER,					// 아래 조명 제어용 (1,2 채널)
		eMAX_LIGHTCount		= 4,			// 채널은 모두 4개이다.

		// Door Open/Close용 Index
		eIDX_DOOR_AZone	= 0,				// 정면 A-Zone door
		eIDX_DOOR_BZone,
		eIDX_DOOR_CZone1,
		eIDX_DOOR_CZone2,
		eIDX_DOOR_DZone,
		eMAX_DoorCount,						// Door 갯수


		// AutoForm에 보여줄 Align 및 MCR 이미지 순번, MCMain에 m_sAutoViewImg[]에 사용
		eIDX_ViewImg_AlignLeft	= 0,
		eIDX_ViewImg_AlignRight,
		eIDX_ViewImg_UVMCR,
		eMAX_ViewImgCount,


		eMODE_SaveNone				= 0,		// 이미지를 저장하지 않는다 (UV-MCR / Align)
		eMODE_SaveOKOnly,						// OK 영상만 저장한다.
		eMODE_SaveNGOnly,						// NG 영상만 저장한다.
		eMODE_SaveAll,							// 모두 저장한다.
		eMAX_ModeSave,


		// 판정에 대한 상수
		eJUDGE_NONE					= 0,		// 판정이 없다.
		eJUDGE_Good,							// 양품
		eJUDGE_NG,								// 불량 <- Align이나 UV-MCR에서 사용
		eJUDGE_BIN2,							// 재검
		eJUDGE_Reject,							// 불량
		eMAX_JUDGECount,


		// 통계 표시용 index
		eIDX_JudgeFinal			= 0,		// 최종 판정 (작업자 판정 포함)
		eIDX_JudgeAuto,						// 자동 (VI) 판정
		eMAX_JudgeMode,						// 통계용 판정 종류



		// 각종 log용 index, 실체는 CMCMain에 선언되어있고, 각자 필요한 곳에서 Pointer를 연결해 사용한다.
		eLOG_SYS				= 0,				// 시스템초기화/에러등 기초적인 LOG
		eLOG_SEQ,								// 자동 실행 시퀀스 기록용
		eLOG_COMM_TCP,							// TCP/IP 통신 로그 (공통)
		eLOG_COMM_VI,							// Vision 검사PC와의 통신 로그
		eLOG_COMM_PG,							// 신호기 PC와의 통신 로그
		eLOG_COMM_DATA,							// Data-PC와의 통신 로그
		eLOG_COMM_MCR,							// UV-MCR과의 통신 로그
		eLOG_COMM_TEMP,							// 온도 센서와의 통신 로그
		eLOG_COMM_VAC,							// 진공 센서와의 통신 로그
		eLOG_UI,								// UI 상에서 사용자의 입력에 대한 로그
		eLOG_CIM,								// Data-PC와의 통신관련 로그
		eLOG_Teach,								// Motor값 변경시 값 변경 내역 로그 ( Teach, Motor )
		eLOG_ETC,								// 기타 잡동사니
		eMAX_LOGCount,							// Log 갯수



		// 작업조 (Shift)
		eIDX_Shift_DAY			= 0,							// 오전 작업조 : 06 ~ 14
		eIDX_Shift_SW,											// 오후 작업조 : 14 ~ 22
		eIDX_Shift_GY,											// 야간 작업조 : 22 ~ 06
		eIDX_Shift_Today,										// 오늘 작업한 모든 내용
		eMAX_ShiftCount,

		// 2015-03-07, jhLee,	작업조 구분을 위한 상수 선언, 기존과 호환성을 가져야 한다.
		eIDX_Shift_DAY2			= eIDX_Shift_DAY,	// 주간 작업조
		eIDX_Shift_GY2			= eIDX_Shift_SW,	// 야간 작업조

		// 2015-03-06, jhPark, 작업조를 2개조로 할 것인지, 3개조로 할 것인지 구분한다.
		eMODE_WorkShift3				= 0,		// 작업조를 3개조로 표시한다
		eMODE_WorkShift2,							// 작업조를 2개조로 표시한다
		eMAX_ModeWork,


		// 적용 취소
		////2015-05-26, jhLee,  Cell Tacking 정보를 어떻게 활용할 것인지 지정한다.
		//eMODE_TrackingMode1				= 0,		// 무시
		//eMODE_TrackingMode2,						// BIN2처리
		//eMODE_TrackingMode3,						// 투입 금지
		//eMAX_MODETrackingMode,						// Tracking mode 사용 모드 수


		// 2015-05-16 JH-PARK	검사결과 방법에 대하여 Message 출력 , Pattern(White) 표시를 구분한다.
		eMODE_DispMessage	=0,								// 결과에 대하여 Message로 표시
		eMODE_Disp1stPattern,								// 결과에 대하여 1st Patten(White)를 표시
		//eMODE_DispNGPatten,									// 결과에 대하여 불량 Patten을 먼저 표시
		
		
		
		// TACT Time 통계 집계 기준
		eIDX_TACTData_Last	= 0,							// 마지막에 실행된 내용
		eIDX_TACTData_Min,									// 최소값의 (가장 빠른 실행) TACT Time을 갖는 데이터
		eIDX_TACTData_Max,									// 최대값의 (가장 느린 실행) TACT Time을 갖는 데이터
		eMAX_TACTDataCount,									// TACT Time 통계 Data 수

			
		// 2015-03-25, jhLee, 김상묵 사원과의 협의로 기존 항목외에 
		// 2014-10-15, jhLee, TACT Time에 대한 새로운 정의, 기존의 TACT과 이동 TACT을 동시에 아우른다.  -- -- -- -- -- -- -- --
		// 본 데이터는 Cell log 데이이터와 함께 움직인다.
		// 실제 TACT Time이 기록되는것은 아니고, SYSTEMTIME을 기록할 배열의 Index로 사용된다.
		eTACT_LoadPGOff				= 0,				// Loading시 사용하는 PG-OFF Time = 이전 Jig의 Unloading PG-OFF
		eTACT_TurnOn,									// 최초로 신호기로 TURNON을 전송한 시각, 작업자 검사 시작 (2015-07-08, jhLee, 추가)
		eTACT_PGOn,										// 최초 Contact 시각, 새로운 Cell Loading 작업 중
		eTACT_ReadyBtn,									// 양수버튼	Loading 및 A-Zone 검사 완료
		eTACT_TiltUpStart,								// JIG Tilt Up Start	지그 Tilt Up, 옵션에 따라서 생략 가능, 상부 조명 실린더 전진
		eTACT_TiltUpEnd,								// JIG Tilt Up End	지그 Tilt Up 완료, UV-MCR 읽기 시작
		eTACT_UVMCRRead,								// UV-MCR 리딩	UV-MCR 읽기 완료, Align Right Move Start
		eTACT_AlignRightMove,							// Align Right 이동	오른쪽 Align 마크 찾기 위치로 이동 완료
		eTACT_AlignRightMark,							// Align Right 마크 찾기 완료, 왼쪽 마크 위치로 이동 시작
		eTACT_AlignLeftMove,							// 왼쪽 Align 마크 찾기 위치로 이동
		eTACT_AlignLeftMark,							// 왼쪽 Align 마크 찾기 및 Align 계산 완료, Turn 대기 시작
		eTACT_PGCellLoading,							// 신호기로부터 Cell Loading 완료 신호를 받는 대기 시간,	#2015-05-26, jhLee 추가
		eTACT_TurnMoveStart,							// C-Zone으로의 회전 이동 시각, 회전 대기 완료
		eTACT_TurnMoveEnd,								// C-Zone으로의 회전 이동 완료
		eTACT_VI_CH1_AlignMove,							// 비전검사 채널#1 Align 동작
		eTACT_VI_CH1_GrabStart,							// 비전검사 채널#1 검사 시작 전송
		eTACT_VI_CH1_GrabEnd,							// 비전검사 채널#1 Grab 완료, CH#2 이동 시작
		eTACT_VI_CH1_ClassEnd,							// 비전검사 채널#1 검사 결과 완료
		eTACT_VI_CH2_AlignMove,							// 비전검사 채널#2 이동 및 Align 동작 완료
		eTACT_VI_CH2_GrabStart,							// 비전검사 채널#2 검사 시작 전송
		eTACT_VI_CH2_GrabEnd,							// 비전검사 채널#2 Grab 완료, Return Move 대기 시작
		eTACT_VI_CH2_ClassEnd,							// 비전검사 채널#2 검사 결과 완료
		eTACT_ReturnMoveStart,							// A-Zone으로의 회전 이동 시각, 회전 대기 완료
		eTACT_ReturnMoveEnd,							// A-Zone으로의 회전 이동 완료
		eTACT_TiltDownStart,							// 지그 Tilt down 사직
		eTACT_TiltDownEnd,								// 지그 Tilt down, 작업자 검사 시작
		eTACT_ManualStart,								// D-Zone 작업자 검사 시작
		eTACT_UnloadPGOff,								// 작업자 검사 완료

		// 2015-07-10, jhLee, 기존의 이름을 새로이 변경하였다.
		eTACT_JigTACTTime,								// 舊 eTACT_TACTTime, PG OFF ~ PG OFF 까지의 설비 TACT, JIG 단위 TACT Time
		eTACT_TotalTACTTime,							// 舊 eTACT_CycleTime,  Jig에 Cell이 투입 부터 검사 완료때까지의 전체 시간, TACTTime + Cell Load/Unload 시간

		eTACT_TACTTime,									// 신규, Cell을 처리하는데 소요되는 시간, 신호기 Power ON ~ 신호기 Power Off
		eTACT_EQTime,									// 설비 소요시간
		eTACT_OPTime,									// 작업자 소요시간
		eTACT_WaitTime,									// 대기 시간

		eTACT_EQLossTime,								// 기준 시간을 벗어난 설비 손실시간
		eTACT_OPLossTime,								// 기준 시간을 벗어난 작업자 손실시간
		eTACT_LossTime,									// 설비 비가동 시간, Cell 작업 중 Alarm/Stop요청에 의해 중지된 시간
		eTACT_LossInEQTime,								// 설비 구동중 중단된 시간
		eTACT_LossInOPTime,								// 작업자 동작중 중단된 시간

		eTACT_AlignTime,								// Align 동작에 소요된 시간 (MCR리딩 후 ~ Align 계산 완료까지)
		eTACT_MCRAlignTime,								// MCR 읽기와 Align을 마칠때까지의 시각, Ready Button ~ Align 종료때까지

		//2015-03-27, jhLee, TE_Check Time추가
		eTACT_PG_TECheckStart,							// TE Check 시작 시간
		eTACT_PG_TECheckEnd,							// TE Check 완료 시간

		//// 2016-04-21, jhLee, TSP time 추가
		//eTACT_PG_TSPtart,								// TSP 시작 시간
		//eTACT_PG_TSPEnd,								// TSP 완료 시간

		// 2015-07-10, jhLee, 각 Zone별 시각 및 TACT 추가
		// eTACT_CellLoadTime,								// 이전Cell 제거 및 신규 Cell 투입까지의 시간 : PG OFF ~ Power On 전송
		eTACT_AZoneTime,								// A Zone TACT Time : TURN ON ~ Cell Loading 수신까지
		eTACT_CZoneTime,								// C Zone TACT Time : 1CH Grab.Start 전송 ~ 2CH GRAB.END.OK 수신까지
		eTACT_DZoneTIme,								// D Zone TACT Time : C->A 회전 후 ~ PG OFF 까지


		eMAX_TACTTimeCount,								// Tact 시각 기록 수량
		//end of NEW TACT -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --



		// 하루동안 진행된 Cell 정보
		eIDX_Product_TodayRunningCell	=0,// 검사 진행된 Cell 수량
		eDIX_Prodcut_TodayAutoGood,		//	최종판정이 자동GOOD
		eDIX_Prodcut_TodayAutoBin2,		//	최종판정이 자동BIN2
		eDIX_Prodcut_TodayAutoReject,		//	최종판정이 자동REJECT
		eDIX_Prodcut_TodayManGood,			//	최종판정이 수동GOOD
		eDIX_Prodcut_TodayManBin2,			//	최종판정이 수동BIN2
		eDIX_Prodcut_TodayManReject,		//	최종판정이 수동REJECT
		eIDX_Product_TodayMcrRead,			// MCR 리딩 횟수
		eIDX_Product_TodayMcrNg,			// MCR 리딩 NG
		eIDX_Product_TodayAlignRead,		// ALIGN 리딩 횟수
		eIDX_Product_TodayAlignNg,			// ALIGN 리딩 NG
		eIDX_Product_TodayRunTime,			// 가동시간
		eIDX_Product_TodayOPLoss,			// OP-LOSS
		eIDX_Product_TodayContact,			// CONTACT 횟수

		eIDX_Product_TodayCH1_ContactGOOD,	// CH1의 Contact 수량
		eIDX_Product_TodayCH2_ContactGOOD,	// CH2의 Contact 수량
		eIDX_Product_TodayCH3_ContactGOOD,	// CH3의 Contact 수량
		eIDX_Product_TodayCH4_ContactGOOD,	// CH4의 Contact 수량
		eIDX_Product_TodayCH1_ContactNG,	// CH1의 Contact 수량
		eIDX_Product_TodayCH2_ContactNG,	// CH2의 Contact 수량
		eIDX_Product_TodayCH3_ContactNG,	// CH3의 Contact 수량
		eIDX_Product_TodayCH4_ContactNG,	// CH4의 Contact 수량

		eIDX_Product_TodayNoDisplay,
		eMAX_Product_Today




};




enum {			// UB-AMT에서 사용되는 각종 설정 값들

	eIDX_CTRL_NMC2		= 0,				// NMC2의 Control Index, CMCMain의 OpenDevice에서 사용된다.

	eSET_CPUNo			= 1,				// MP2100 CPU 번호
	eSET_NMCNo			= 1,				// NMC IP 번호

	// 온도센서 종류
	eIDX_TEMP_PCRACK	= 0,				// PC-RACK 온도 센서
	eIDX_TEMP_LOAD,
	eIDX_TEMP_UPS,
	eIDX_TEMP_UNLOAD,
	eIDX_TEMP_GPS,
	eIDX_TEMP_DRIVEBOX,
	eMAX_TEMPCount,						// 설치된 온도계 수량 (현재, 1개)


	// 사용자 로그인시 입력 글자 수 제한
	eMAX_UserIDLength	= 8,			// 사번 8자리 이내 입력


	// 신호기 Current 데이터 수, CellInfo에서 사용
	eMAX_CurrentCount	= 6,			// Contact current 데이터 수량
	eMAX_TSP_RESULT		= 12,			// TSP 결과는 최대 12까지 


	// 비가동 사유 입력시 STOP->AUTO 전환 도중인지, AUTO->STOP 전환 도중인지 구분해준다.
	eMODE_STATE_AutoToStop	= 0,		// AUTO -> STOP으로 전환
	eMODE_STATE_StopToAuto,				// STOP -> AUTO로 전환
	
	eSTATE_Judge_None	= 0,			// 아직 판정을 하지 않았다.
	eSTATE_Judge_Run,					// 판정 진행중이다.
	eSTATE_Judge_End,					// 판정을 마쳤다.
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --



	eUSER_NONE			= 0,				// Login을 하지 않았다.
	eUSER_Operator,						// Operator level
	eUSER_Engineer,						// Engineer level
	eUSER_SuperUser,						// 최상위 권한, Setup시 사용

	// 나중에 지우기;
	eSTATUS_NONE =0,					
	eSTATUS_DAY,						
	eSTATUS_SW,
	eSTATUS_GY,						
	

	// Initial 진행여부 flag값, 	CMCMain에서 선언 int m_nInitialFlag; // 설비의 Initial 관계
	eINIT_NONE			= 0,				// 아직 Initial 작업을 하지않음
	eINIT_Processing,						// Initial 동작 중
	eINIT_Complete,						// Initial 동작 


	// 전면 PG 제어용 버튼의 구분, SysPanel과 WorkTable에서 사용
	eBTN_PG_ONOFF		= 0,
	eBTN_PG_BACK,
	eBTN_PG_NEXT,


	// D-Zone Manual 검사시 Defect 위치에 Circle을 그리기 위한 상수들
	eMAX_CIRCLE_PATTERN		= 7,		// 7가지가 존재


	//// 진공센서 순번
	//eIDX_VAC_MAIN		= 0,			// Main공압
	//eIDX_VAC_JIG1_1,
	//eIDX_VAC_JIG1_2,
	//eIDX_VAC_JIG2_1,
	//eIDX_VAC_JIG2_2,
	//eMAX_VACCount,						// 읽어올 공압센서 수량


	// 공압 순서
	eIDX_VAC_JIG1_CH1	= 0,
	eIDX_VAC_JIG1_CH2,
	eIDX_VAC_JIG2_CH1,
	eIDX_VAC_JIG2_CH2,
	eIDX_VAC_MAIN,
	eMAX_VACCount,						// 읽어올 공압센서 수량


	// PG Test를 위한 신호기 선택용 (FormSetup4) - - - - -  - - - - - - - - - -
	eIDX_PG_A1					= 0,
	eIDX_PG_A2,
	eIDX_PG_C1,
	eIDX_PG_C2,
	eMAX_PGCount,							// 사용되는 PG 수량

	// PG Test용 Zone 선택
	eIDX_PG_ZONE_A				= 0,
	eIDX_PG_ZONE_B,
	eIDX_PG_ZONE_C,
	eIDX_PG_ZONE_D,
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


	// AxisInfo에서 사용하는 축의 순번
	eAXIS_1						= 0,
	eAXIS_2,
	eAXIS_3,
	eAXIS_4,
	eAXIS_5,
	eAXIS_6,
	eMAX_AXISCount,

	// STO기능 때문에 축 순번이 변경되었다 : 전장설계 김병수 대리
	eAXIS_PreAlignX			= eAXIS_3,		// 2, NMC2		// 1, MP2100
	eAXIS_WorkTableT		= eAXIS_1,		// 0			// 0
	eAXIS_AVCameraZ			= eAXIS_4,		// 3			// 2
	eAXIS_AVCameraT			= eAXIS_6,		// 5			// 3
	eAXIS_AVCameraX			= eAXIS_5,		// 4			// 4
	eAXIS_AVCameraF			= eAXIS_2,		// 1,			// 0, NMC


	// 위치 조회를 위한 상수 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	// PreAlign
	ePOS_PREALIGN_Align1		= 0,			// PreAlign X축, Align #1 위치
	ePOS_PREALIGN_Align2,					// PreAlign X축, Align #2 위치
	ePOS_PREALIGN_UVMCR,						// PreAlign UVMCR 취득 위치
	eMAX_POS_PREALIGNCount,


	// WorkTable
	ePOS_WORKTABLE_Return	= 0,			// WorkTable T축, 원점 위치 (Return)
	ePOS_WORKTABLE_Turn,						// WorkTable T축, 회전 위치 (Turn)
	eMAX_POS_WORKTABLECount,

	// AVCamera
	ePOS_AVCAM_JIG1_Ch1		= 0,		// Jig#1, Channel#1 위치
	ePOS_AVCAM_JIG1_Ch2,					// Jig#1, Channel#2 위치
	ePOS_AVCAM_JIG2_Ch1,					// Jig#2, Channel#1 위치
	ePOS_AVCAM_JIG2_Ch2,					// Jig#2, Channel#2 위치
	eMAX_POS_AVCAMCount,

	ePOS_AVCAMERA_Ch1			= 0,				// AV-Camera 모든축(특히 X축), 채널 #1 위치
	ePOS_AVCAMERA_Ch2,					// AV-Camera 모든축(특히 X축), 채널 #2 위치

	//// AVCamera에서만 사용하는 내부 축 순번
	//eIDX_AVCAM_AXIS_X			= 0,
	//eIDX_AVCAM_AXIS_Z,
	//eIDX_AVCAM_AXIS_T,
	//eIDX_AVCAM_AXIS_F,
	//eMAX_AVCAM_AXISCount,


	//end of 위치 조회를 위한 상수 -- -- -- -- -- --- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --



	// UDMSG_DISPDATA_UPDATE 메세지중 변경된 데이터 종류를 지정한다.
	eIDX_UPDATE_ALL			= 0,		// 모든 정보를 새로이 갱신한다.
	eIDX_UPDATE_MODEL,					// 모델 정보가 변경되었다.
	eIDX_UPDATE_STATE,					// 설비 상태가 변경되었다.
	eIDX_UPDATE_TACT1,					// JIG#1의 TACT 관련 내용이 변경되었다.
	eIDX_UPDATE_TACT2,					// JIG#2의 TACT 관련 내용이 변경되었다.
	eIDX_UPDATE_TACTAVG,					// JIG#의 TACT 평균 내용이 변경되었다.
	eIDX_UPDATE_PRODUCT,					// 생산량 관련 내용이 변경되었다.
	eIDX_UPDATE_ALIGN,					// Align 수행결과 반영
	eIDX_UPDATE_UVMCR,					// UV-MCR 리딩 결과 반영
	eIDX_UPDATE_CONTACT,					// PG Contact 관련 변경
	eIDX_UPDATE_PROCESS,					// Cell 처리과정이 변경되었다.
	eIDX_UPDATE_UI,						// UI (버튼 등...) 관련 처리 내용이 변경되었다.
	eIDX_UPDATE_RUNTIME,					// 실행 변수 내용이 변경되었다. (User Login / Start-Stop Time 등)

	// eIDX_UPDATE_PRODUCT, 생산량 데이터 분류
	eIDX_PRODUCT_All			= 0,		// 모든 데이터
	eIDX_PRODUCT_Product,				// 생산량
	eIDX_PRODUCT_UVMCR,					// UV-MCR 읽기
	eIDX_PRODUCT_Align,					// Align 수행
	eIDX_PRODUCT_Time,					// 시간데이터

	// eIDX_UPDATE_UI, Auto Form의 UI 변경용
	eIDX_AUTO_UI_All			= 0,		// 모든 내용 갱신
	eIDX_AUTO_UI_CellInfo,				// A-Zone에서의 Cell 정보 입력
	eIDX_AUTO_UI_Defect,					// D-Zone에서의 Defect 입력

	// eIDX_UPDATE_PROCESS, 현재 처리 상태를 화면에 표시해주기 위한 상태 index
	eIDX_PROCESS_NotUse		= 0,		// 미사용, Cell 없음.
	eIDX_PROCESS_Wait,					// 대기중
	eIDX_PROCESS_Working,				// 작업중
	eIDX_PROCESS_Finish,					// 작업완료
	eIDX_PROCESS_Error,					// 오류 발생

	// eIDX_UPDATE_RUNTIME, 실행 데이터 표시용
	eIDX_RUNTIME_Login		= 0,		// Login/out 정보
	eIDX_RUNTIME_EQState,				// 설비 Start/Stop 정보
	// ----------------------------- // UDMSG_DISPDATA_UPDATE


	// Label Printer Buffer 크기
	eMAX_PrinterBuffer		= 256,	// 256 bytes

};


enum {
	// 동작 명령 수행중 일정 시간이 지나면 동작에 이상이 있다고 감지하기위한 시간 설정값

	// TIME ============================================================================
	// I/O 동작
	eOPTION_TM_Cylinder			= 0,		// 실린더 동작 기본 Timeout
	eOPTION_TM_BackLight,					// PreAlign 상부 BackLight 실린더동작 Timeout
	eOPTION_TM_JigTilt,						// WorkTable Jig의 Tilt 동작 Timeout
	eOPTION_TM_JigVacOn,						// WorkTable Jig의 진공 형성 Timeout
	eOPTION_TM_JigVacOff,					// WorkTable Jig의 진공 파기 Timeout

	// Motion 동작 Timeout
	eOPTION_TM_MoveAlign,					// Align/MCR 읽는 위치로 이동 Timeout,
	eOPTION_TM_MoveTurn,						// WorkTable Trun/Return 이동 Timeout,
	eOPTION_TM_MoveCamera,					// AVCamera들의 위치로 이동 Timeout,

	// 각종 검사 시간 Timeout
	eOPTION_TM_Align,							// Align (Mark Search) 동작 Timeout
	eOPTION_TM_MCRRead,						// UV-MCR 읽기동작 Timeout
	eOPTION_TM_AVCamera,						// 검사PC에의한 자동검사 Timeout ?
	eOPTION_TM_PGTest,						// 신호기에의한 검사 Timeout ?


	// 가상모드로 동작시 해당 동작에 소요되는 가상의 시간을 설정한다.
	eOPTION_TACT_Cylinder		= 100,	// 실린더 동작 기본 Tact
	eOPTION_TACT_BackLight,					// PreAlign 상부 BackLight 실린더동작 Tact
	eOPTION_TACT_JigTilt,					// WorkTable Jig의 Tilt 동작 Tact
	eOPTION_TACT_JigVacOn,					// WorkTable Jig의 진공 형성 Tact
	eOPTION_TACT_JigVacOff,					// WorkTable Jig의 진공 파기 Tact

	// Motion 동작 가상 Tact
	eOPTION_TACT_MoveAlign,					// Align/MCR 읽는 위치로 이동 Tact,
	eOPTION_TACT_MoveTurn,					// WorkTable Trun/Return 이동 Tact,
	eOPTION_TACT_MoveCamera,				// AVCamera들의 위치로 이동 Tact,

	// 각종 검사 시간 Timeout
	eOPTION_TACT_Align,						// Align (Mark Search) 동작 Tact
	eOPTION_TACT_MCRRead,					// UV-MCR 읽기동작 Tact
	eOPTION_TACT_AVCamera,					// 검사PC에의한 자동검사 Tact
	eOPTION_TACT_PGTest,						// 신호기에의한 검사 Tact

};
enum
{
	eLEVEL_NONE =0,
	eLEVEL_WARNING,
	eLEVEL_ALARM,
	eLEVEL_ESTOP

};

#if 0
// 아래 부분은 사용할 것인가 ?
// 2015-04-02, 각종 상태를 정의한다.
eSTATE_Insp_None	= 0,			// 아직 검사를 진행하지 않았다.
	eSTATE_Insp_Run,					// 검사 수행중
	eSTATE_Insp_GrabEnd,				// Grab.End 수신
	eSTATE_Insp_ClassEnd,				// Class.End 수신
	eSTATE_Insp_End,					// 검사 종료
#else
enum INSP_STATE
{
	eSTATE_Insp_None = 0,
	eSTATE_Insp_Run,
#ifdef DISPLAY_INSPECT_CHECK
	eSTATE_Insp_GrabEnd,	// 화상검사일 때만 쓰임
	eSTATE_Insp_ClassEnd,	// 화상검사 전용. 검사결과파일 읽었는지..
#endif
	eSTATE_Insp_End,
	eSTATE_Insp_MAX
};	
#endif





// 작업자 계정관리용
class CUserAccount
{
public:
	CString		sID;						// 작업자 ID, 사번?
	CString		sPassword;				// 암호
	int			nLevel;					// 레벨
	CString		sKorName;				// 한글이름
	CString		sEngName;				// 영어이름
	CString		sIDNo;					//	사원번호;
	

	CUserAccount () {	
		Init();// 생성자
	};
	void Init()
	{
		sID			= _T("");
		sPassword	= _T("");
		nLevel		= 0;
		sKorName		= _T("");
		sEngName		= _T("");
		sIDNo			= _T("");
	}
};


typedef CTypedPtrMap<CMapStringToPtr, CString, CUserAccount*> CMapStringToUserAccount;



// Align 결과를 보관 할 저장소
//typedef struct _TAlignResult {
//
//	DPOINT	FindPos[eMAX_MarkCount];		// 패턴매칭을 통해 찾은 왼쪽 위치
//	double	dbScore[eMAX_MarkCount];		// 왼쪽 마크를 찾을때의 점수
//	BOOL	bResult[eMAX_MarkCount];		// 왼쪽 마크를 찾았는지 여부
//
//	double	dbDeltaX;						// 증감된 X 값
//	double	dbDeltaY;						// 증감된 Y 값
//	double	dbDeltaTheta;					// 증감된 T 값 -> 이 값으로 AVCamera의 T 축을 회전시킨다.
//
//	BOOL	bFinalResult;					// 최종 결과
//
//	_TAlignResult() { Clear(); };			// 생성자
//
//	// 내용을 초기화 한다.
//	void Clear() {
//		for (int i = 0; i < eMAX_MarkCount; i++)
//		{
//			FindPos[i].SetPoint(0.0, 0.0);
//			dbScore[i]	= 0.0;			
//			bResult[i]	= FALSE;			
//		}
//
//		dbDeltaX		= 0.0;			
//		dbDeltaY		= 0.0;			
//		dbDeltaTheta	= 0.0;		
//
//		bFinalResult	= FALSE;		
//	};
//
//
//	// 2015-03-27, jhLee, 구조체 복사용 치환 연산자 선언
//	void operator=(_TAlignResult &src) 	
//	{	
//		for (int i = 0; i < eMAX_MarkCount; i++)
//		{
//			FindPos[i]		= src.FindPos[i];		// 패턴매칭을 통해 찾은 왼쪽 위치
//			dbScore[i]		= src.dbScore[i];		// 왼쪽 마크를 찾을때의 점수
//			bResult[i]		= src.bResult[i];		// 왼쪽 마크를 찾았는지 여부
//		}
//		
//		dbDeltaX		= src.dbDeltaX;						// 증감된 X 값
//		dbDeltaY		= src.dbDeltaY;						// 증감된 Y 값
//		dbDeltaTheta	= src.dbDeltaTheta;					// 증감된 T 값 -> 이 값으로 AVCamera의 T 축을 회전시킨다.
//
//		bFinalResult	= src.bFinalResult;					// 최종 결과
//	};
//
//
//} TAlignResult;



// 생산량 정보를 기록하고 관리하는 구조체
//	// 통계 표시용 index
//		eIDX_JudgeFinal			= 0,		// 최종 판정 (작업자 판정 포함)
//		eIDX_JudgeAuto,						// 자동 (VI) 판정
//		eMAX_JudgeMode,						// 통계용 판정 종류
//
typedef struct _TProductData {
	UINT		nTotalCount;							// 전체 생산량

	UINT	nGoodCount[eMAX_JudgeMode];		// 양품 수량
	UINT	nBin2Count[eMAX_JudgeMode];		// BIN2, 재검 수량
	UINT	nRejectCount[eMAX_JudgeMode];	  	// 불량 수량
	double	dbGoodRatio[eMAX_JudgeMode];		// 양품 비율
	double	dbBin2Ratio[eMAX_JudgeMode];		// BIN2 비율
	double	dbRejectRatio[eMAX_JudgeMode];	// Reject 비율
	
	UINT	nMCRReadCount;							// UV-MCR Reading 횟수
	UINT	nMCRReadNG;								// UV-MCR 리딩 중 NG 횟수
	double	dbMCRReadRatio;						// UV-MCR 리딩 성공률

	UINT	nAlignCount;							// Align 읽기 횟수
	UINT	nAlignNG;								// Align 실패 횟수
	double	dbAlignReadRatio;						// Algin 읽기 성공률

	//

	// 가동시간 (초)
	UINT	nRunTime;								// 가동 시간
	UINT	nRunTimeSum;							// 지금까지의 총 가동시간 ?
	UINT	nLossTime;								// 비가동 시간
	UINT	nTimeSaveCnt;							// 시간관련 데이터 저장 주기 Count

	_TProductData() { ClearProduct(); ClearTime(); };

	void ClearProduct() {
		nTotalCount		= 0;							// 전체 생산량

		for(int i=0; i<eMAX_JudgeMode; i++) {
			nGoodCount[i]		= 0;					// 양품 수량
			nBin2Count[i]		= 0;					// BIN2, 재검 수량
			nRejectCount[i]	= 0;					// 불량 수량

			dbGoodRatio[i]		= 0.0;				// 양품 비율	
			dbBin2Ratio[i]		= 0.0;				// BIN2 비율
			dbRejectRatio[i]	= 0.0;				// Reject 비율
		}

		nMCRReadCount	= 0;							// UV-MCR Reading 횟수
		nMCRReadNG		= 0;							// UV-MCR 리딩 중 NG 횟수
		dbMCRReadRatio	= 0;							// UV-MCR 리딩 성공률

		nAlignCount		= 0;							// Align 읽기 횟수
		nAlignNG			= 0;							// Align 실패 횟수
		dbAlignReadRatio=0;							// Algin 읽기 성공률
	};

	void ClearTime() {
		nRunTime		= 0;								// 가동 시간
		// nRunTimeSum	= 0;								// 지금까지의 총 가동시간 ?
		nLossTime	= 0;								// 비가동 시간
		nTimeSaveCnt = 99;							// 시간관련 데이터 저장 주기 Count
	};

	void ClearDayTime() {
		nRunTimeSum	= 0;								// 지금까지의 총 하루기준 가동시간 ?
		nTimeSaveCnt = 99;							// 시간관련 데이터 저장 주기 Count
	};



	// 하루치 정보내용;
	UINT		nTodayRunningCell[24];		// Cell 진행수량
	UINT		nTodayAutoGoodCount[24];	// 자동검사 GOOD 수량	
	UINT		nTodayAutoBin2Count[24];	// 자동검사 BIN2 수량
	UINT		nTodayAutoRejectCount[24];	// 자동검사 LOSS 수량
	UINT		nTodayManGoodCount[24];		// 수동검사 GOOD 수량
	UINT		nTodayManBin2Count[24];		// 수동검사 BIN2 수량
	UINT		nTodayManRejectCount[24];	// 수동검사 LOSS 수량
	UINT		nTodayRunTime[24];			// 가동시간
	UINT		nTodayOPLoss[24];			// OP Loss시간
	UINT		nTodayContact[24];			// Contact 수량
	
	double	dTodayMcrReadCount[24];			// MCR Reading 수량
	double	dTodayMcrNgCount[24];			// MCR 판정 NG 수량
	double	dTodayAlignReadCount[24];		// ALIGN 진행 수량
	double	dTodayAlignNgCount[24];			// ALIGN 판정 NG 수량

	//2015/05/03 JH-PARk
	// Contact율에 대한 변수 추가 ( 김정유D 요청사항 )

	UINT		nCH1_GoodCount[24];		// CH1의 대한 GOOD 수량
	UINT		nCH2_GoodCount[24];		// CH2의 대한 GOOD 수량
	UINT		nCH3_GoodCount[24];		// CH3의 대한 GOOD 수량
	UINT		nCH4_GoodCount[24];		// CH4의 대한 GOOD 수량

	UINT		nCH1_NgCount[24];		// CH1의 대한 NG 수량
	UINT		nCH2_NgCount[24];		// CH2의 대한 NG 수량
	UINT		nCH3_NgCount[24];		// CH3의 대한 NG 수량
	UINT		nCH4_NgCount[24];		// CH4의 대한 NG 수량
	UINT		nNoDisplay[24];			// No Display 선택수량

	
	void ClearTodayProductData() 
	{
		for(int i=0; i<24; i++)
		{
			nTodayRunningCell[i]		= 0;
			nTodayAutoGoodCount[i]	= 0;
			nTodayAutoBin2Count[i]	= 0;
			nTodayAutoRejectCount[i]= 0;
			nTodayManGoodCount[i]	= 0;
			nTodayManBin2Count[i]	= 0;
			nTodayManRejectCount[i]	= 0;

			nTodayRunTime[i]		= 0;
			nTodayOPLoss[i]			= 0;
			nTodayContact[i]		= 0;

			dTodayMcrReadCount[i]	= 0;
			dTodayMcrNgCount[i]		= 0;
			dTodayAlignReadCount[i]	= 0;
			dTodayAlignNgCount[i]	= 0;

			nCH1_GoodCount[i]		= 0;	
			nCH2_GoodCount[i]		= 0;		
			nCH3_GoodCount[i]		= 0;		
			nCH4_GoodCount[i]		= 0;		
			nCH1_NgCount[i]			= 0;		
			nCH2_NgCount[i]			= 0;		
			nCH3_NgCount[i]			= 0;		
			nCH4_NgCount[i]			= 0;		
			nNoDisplay[i]			= 0;			

		}
	};


} TProductData;



typedef CTypedPtrMap<CMapStringToPtr, CString, CUserAccount*> CMapUserAccount;


// from GAON, MIT-AFT == == == == == == == == == == == == == == == == == == == == == == ==

//// Paramter Bin2 Pattern
//#define MAX_BIN2_PATTERN 10
//class CParamBin2Pattern
//{
//public:
//	CStringList m_DefectName;
//	COLORREF m_PatternRGB[MAX_BIN2_PATTERN];
//	CString m_PatternName[MAX_BIN2_PATTERN];
//	int m_PatternCnt;
//
//	BOOL ReadDefect(CString filename);
//	BOOL ReadPattern(CString filename);
//};
