//==================================================================================================================
//
// 화면 표시 처리를 하면서 정의하는 각종 상수 
//
//==================================================================================================================


#pragma once


// 폼/다이얼로그 화면 전환에 필요한 공통 사용 일련 번호 선언
enum {

	// CFormView, 서브 화면 폼 
	eFORM_Default			= 0,			// 프로그램 시작시 기본적으로 가지고 있는  CFormView
	eFORM_Main,								// Main 화면
	eFORM_Alarm,							// Alarm 표시 화면
	eFORM_PM,								// PM / 경고 화면	
	eFORM_OLBDOWN,
	eFORM_Recipe,							// [W] Recipe 화면
	eFORM_Report1,							// 각종 Report 표시 화면
	eFORM_Report2,							
	eFORM_Report3,							
	eFORM_Report4,
	eFORM_Report5,

	eFORM_Teach1_Loader,							// 모듈별 티칭 화면
	eFORM_Teach1_PreAlign,
	eFORM_Teach1_LoaderTR,
	eFORM_Teach1_TurnTable,
	eFORM_Teach1_UnloaderTR,
	eFORM_Teach1_GoodTray,
	eFORM_Teach1_NGTray,
	eFORM_Teach1_LDRobot,
	eFORM_Teach1_UDRobot,

	eFORM_Setup1,							// 디바이스 설정 화면
	eFORM_Setup2,
	eFORM_Setup3,
	eFORM_Setup4,
	eFORM_Config1,							//	파라미터 설정 화면
	eFORM_Config2,				//Options
	eFORM_Config3,			
	eMAX_FORMCount,


	// CDialogBar, 화면
	eDLGBAR_None			= 0,			// 지정 안함.
	eDLGBAR_Top,							// 상위 정보표시 
	eDLGBAR_Bottom,						// 하위 메인 메뉴 표시
	eDLGBAR_SubReport,						// Report 하위 메뉴 선택
	eDLGBAR_SubTeach,						// Teach 하위 메뉴 선택
	eDLGBAR_SubSetup,						// Setup 하위 메뉴 선택
	eDLGBAR_SubConfig,						// Config 하위 메뉴 선택
	eDLGBAR_Jog,							// Motion Jog 동작 
	eMAX_DLGBARCount,
	

	// CDialog, 대화상자 
	eDLG_None				= 0,			// 지정 안함. (고정)
	eDLG_Confirm,							// 사용자의 확인을 받는 화면
	eDLG_Inform,							// 각종 정보 표시 화면
	eDLG_Initial,							// 초기화 진행 상황 표시 화면
	eDLG_MsgInterlock,					// 상위 Interlock 메세지 표시 화면
	eDLG_MsgOPCall,						// 상위 Operator Call 메세지 표시 화면
	eDLG_MsgTerminal,						// 상위 Terminal 메세지 표시 화면
		
	eDLG_Losstime,						// 비가동 입력화면  20170118 byskcho

	eDLG_JobChange,                     //Trayid 잡 체인지 메세지

	eDLG_MsgTMDDownLoad,                 //TMD Download  메시지
	//eDLG_Door,								// Door open / Safety 관련 화면

	eDLG_Statement,                     //통계화면   
	eDLG_VersionHistory,
	eDLG_AGVMsg,
	eDLG_PortEmptyMsg,
	eDLG_RETRYINFO,
	eMAX_DLGCount,

};