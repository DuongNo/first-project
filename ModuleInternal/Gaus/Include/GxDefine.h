//
// Gaus library에서 공통적으로 사용되는 상수 및 정의 모음
//
// 2014-03-18, jhLee (semsemi)
// 2016-05-31, jhLee, DLL Version 이동, Gx_Round ... 등의 매크로 정의 이름을 변경
//

#pragma once

#include <math.h>
#include <afxmt.h>												// CEvent 사용

#define _GAUS_DLL_VERSION_	_T("2.6.4")						// Gaus DLL Version

// 
// Gaus System의 각종 메세지 정의
#define GXMSG_MESSAGE_BASE			(WM_USER + 9000)				// Gaus에서 사용할 사용자정의 메세지 Base
#define GXMSG_STATE_CHANGE			(GXMSG_MESSAGE_BASE + 1)		// 상태가 변경되었을때 
#define GXMSG_SYSTEM_ABORT			(GXMSG_MESSAGE_BASE + 2)		// Abort가 진행되었다.
#define GXMSG_ALARM_SET				(GXMSG_MESSAGE_BASE + 3)		// Alarm이 발생하였다.
#define GXMSG_ALARM_CLEAR			(GXMSG_MESSAGE_BASE + 4)		// Alarm이 처리되었다.

#define GXMSG_REQUEST_START		(GXMSG_MESSAGE_BASE + 10)		// Start 요청 (외부 -> GxMaster)
#define GXMSG_REQUEST_STOP			(GXMSG_MESSAGE_BASE + 11)		// STOP 요청 (외부 -> GxMaster)
#define GXMSG_REQUEST_ABORT		(GXMSG_MESSAGE_BASE + 12)		// ABORT 요청 (외부 -> GxMaster)

#define GXMSG_MOTION_ALL_STOP		(GXMSG_MESSAGE_BASE + 20)		// 모든 축을 정지 시킨다.
#define GXMSG_MOTION_ROBOT_STOP	(GXMSG_MESSAGE_BASE + 21)		// 지정 Robot을 정지 시킨다.
#define GXMSG_MOTION_AXIS_STOP	(GXMSG_MESSAGE_BASE + 22)		// 지정 1개축을 정지 시킨다.
////////////////////////////////////////////////////////////////////////////////////////////////////



// try ~ catch() 에서 thraw하여 넘겨주는 예외 상황
#define EXP_THROW_ABORT			(-1)		// Abort 
#define EXP_THROW_TERMINATE	(-2)		// Thread 종료 처리

////////////////////////////////////////////////////////////////////////////////////////////////////

#define Gx_Round(x,dig)			(floor((x) * pow(10, dig) + 0.5) / pow(10, dig))
#define Gx_RoundDown(x,dig)	(floor((x) * pow(10, dig)) / pow(10, dig))
#define Gx_RoundUp(x,dig)		(ceil((x) * pow(10, dig)) / pow(10, dig))
#define Gx_Pow2(x)				(pow((x), 2))
#define Gx_Pi						(3.1415926535897932384626433832795028841971)
#define Gx_ToRad(x)				((x) * Gx_Pi / 180.0)
#define Gx_ToDeg(x)				((x) * 180.0 / Gx_Pi)
#define Gx_Sqrt(x)				(sqrt(x))


//
////-----------------------------------------------------------------------------
//// Define Color 
////-----------------------------------------------------------------------------
//#ifndef _GxCOLOR_
//	#define _GxCOLOR_
//
//	#define clrBlack			0x00000000	// 검은색
//	#define clrWhite			0x00FFFFFF	// 흰색
//	#define clrRed	 			0x000000FF	// 빨간색
//	#define clrGreen			0x0000FF00	// 초록색
//	#define clrBlue				0x00FF0000	// 파란색
//	#define clrMagenta			0x00FF00FF	// 자홍색
//	#define clrCyan				0x00FFFF00	// 청록색
//	#define clrYellow			0x0000FFFF	// 노란색
//
//	#define clrCrimson			RGB(255,	100,	100)			
//	#define	clrGreenForest		RGB(80,		240,	148)
//	#define	clrGreenLime		RGB(200,	250,	200)
//	#define	clrBlueD			RGB(102,	102,	153)
//	#define	ClrBlueRoyal		RGB(180,	180,	218)
//	#define ClrBlueWhite		RGB(118,	116,	177)
//	#define clrYellowWhite		RGB(255,	255,	181)	
//	#define clrOrange			RGB(255,	128,	0)
//	#define	clrOrangeD			RGB(255,	128,	64)
//	#define	ClrCoralL			RGB(255,	180,	180)
//
//	#define	clrGrayDim			RGB(128,	128,	128)
//	#define ClrIndigo			RGB(0x4b,	0x00,	0x82)
//	#define ClrRedIndian		RGB(0xcd,	0x5c,	0x5c)
//	#define clrGrayLSlate		RGB(0x77,	0x88,	0x99)
//	#define	clrnavajowhite		RGB(255,	222,	173)
//	#define clrburlywood		RGB(0xde,	0xb8,	0x87)
//
//
//	// Dk 값이 커질수록 색이 어두워짐 (%)
//	// Lt 값이 커질수록 명도가 밝아짐 (%)
//	#define clrRedP(Dk, Lt)		(0x000001*(0xFF*Dk/100) + 0x010100*(Dk*Lt/100))
//	#define clrGreenP(Dk, Lt)	(0x000100*(0xFF*Dk/100) + 0x010001*(Dk*Lt/100))
//	#define clrBlueP(Dk, Lt)	(0x010000*(0xFF*Dk/100) + 0x000101*(Dk*Lt/100))
//	#define clrMagentaP(Dk, Lt)	(0x010010*(0xFF*Dk/100) + 0x000100*(Dk*Lt/100))
//	#define clrCyanP(Dk, Lt)	(0x010100*(0xFF*Dk/100) + 0x000001*(Dk*Lt/100))
//	#define clrYellowP(Dk, Lt)	(0x000101*(0xFF*Dk/100) + 0x010000*(Dk*Lt/100))
//	#define clrGrayP(Lt)		(0x010101*(0xFF*Lt/100))
//
//	// Dk 값이 커질수록 색이 어두워짐 (0~255)
//	// Lt 값이 커질수록 명도가 밝아짐 (0~255)
//	#define clrRedV(Dk, Lt)		(0x000001*Dk + 0x010100*Lt)
//	#define clrGreenV(Dk, Lt)	(0x000100*Dk + 0x010001*Lt)
//	#define clrBlueV(Dk, Lt)	(0x010000*Dk + 0x000101*Lt)
//	#define clrMagentaV(Dk, Lt)	(0x010010*Dk + 0x000100*Lt)
//	#define clrCyanV(Dk, Lt)	(0x010100*Dk + 0x000001*Lt)
//	#define clrYellowV(Dk, Lt)	(0x000101*Dk + 0x010000*Lt)
//	#define clrGrayV(Lt)		(0x010101*Lt)
//#endif //of #ifndef _GxCOLOR_


////////////////////////////////////////////////////////////////////////////////////////////////////

enum {		// 2015-04-25, jhLee, CGxObject의 ObjType 선언
	eGxOBJ_None			= 0,		// 아무런 지정이 없는 기본 값
	eGxOBJ_Task,					// Thread 기능을 수행하는 기본적인 task 객체
	eGxOBJ_Station,					// 자동 Sequence 기능을 수행하는 Station 객체
	eGxOBJ_System,					// 상시 구동을 진행한는 System 객체
	eGxOBJ_Service,					// 주변 장치와의 연결을 지원하는 Service 객체

	eMAX_GxOBJCount,				// 최대 종류 수량

};


enum {		// 각 Class들의 Thread 기동 수량을 지정한다.
	eCNT_ThreadStation	= 2,	// Station의 Thread 수량은 2개, AutoRun, ManualRun
	eCNT_ThreadSystem		= 1,	// System의 Thread 수량은 1개, SystemRun
	eCNT_ThreadService	= 1,	// Service의 Thread 수량은 1개, ServiceRun

};



enum {	// 각종 상태 상태를 나타내는 State
		// 정지상태 
		eSTATE_IDLE			= 0,	// 아무런 상태도 아니다, 최초의 상태, ABORT 실행 후 상태, Initial을 수행해야한다.
		eSTATE_ABORT,				// 동작을 포기하고 상태 초기화 상태
		eSTATE_ALARM,				// Error 상태
		eSTATE_STOP,				// STOP 상태
		eSTATE_AUTO,				// 자동실행중인 상태
		eSTATE_MANUAL,				// 수동 동작중인 상태

		// AUTO 상태
		eSTATE_AUTOSTART,			// STOP -> AUTORUN으로 전환되는 과정
		eSTATE_AUTOSTOP,			// AUTORUN -> STOP으로 전환되는 과정

		// MANUAL 상태
		eSTATE_MANUALSTART,		// STOP -> MANUALRUN으로 전환되는 과정
		eSTATE_MANUALSTOP,		// MANUALRUN -> STOP으로 전환되는 과정

		eSTATE_ALARMSTOP,			// Alarm이 발생되어 중지 되는 과정

		// 기타
		eMAX_STATECount,

};


enum {	// Event 관련 Index
		eEVENT_ABORT		= 0,
		eEVENT_RESET,				// 최초 실생시 및 Abort 상태일때 다시 기동을 위한 초기화
		eEVENT_ALARM,				// Error 발생시
		eEVENT_STOP,
		eEVENT_START,				// STOP -> AUTO로 진행하기 위한 상태변경용 Event
		eEVENT_AUTO,
		eEVENT_MANUAL,				// STOP -> MANUAL로 전환하기 위한 상태변경용 Event
		eEVENT_AUTORUN,				// 각 Station 내에서 AutoRun()을 계속 실행할것인지 바로미터
		eEVENT_TIMEOUT,				// 시간 초과 발생 ?
		eEVENT_SHUTDOWN,			// 프로그램 종료를 위한 종료Event
		eMAX_EVENTCount,
};

enum {	// 값
		eOFF			= 0,		// OFF 값, DIO
		eON				= 1,		// ON 값, DIO

		eRESET			= 0,		// Reset 값, Event
		eSET			= 1,		// Set 값, Event

		eAUTO			= FALSE,	// 명령 지령시 오토상태에서 내린다.
		eMANUAL			= TRUE,		// 명령 지령시 메뉴얼 상태에서 내린다.

		eMODE_Auto		= 0,		// 명령 모드 (motion 제어시에 이용)
		eMODE_Manual	= 1,

		eCHECK			= TRUE,		// 명령 완료를 검사하도록 한다.
		eNOCHECK		= FALSE,	// 명령 완료를 검사하지 않는다.
		
		// Motion 이동명령등에서 다른 동작과 병행하기 위해 동작과 검사를 선택적으로 지시한다.
		eACT_Action		= 0x01,		// 동작만 수행한다. 완료검사는 하지 않는다.
		eACT_Check		= 0x02,		// 완료검사만 수행한다. 동작은 지령하지 않는다.
		eACT_All		= 0x03,		// 동작 지령 및 완료검사를 모두 수행한다, 보통의 명령


		// 응답코드 (Alarm 발생 후 사용자의 응답)
		eRETURN_OK		= 0,		// 응답으로 OK
		eRETURN_NO		= 1,		// NO 선택
		eRETURN_YES		= 2,		// YES 선택
		eRETURN_RETRY	= 3,		// ReTry 선택
		eRETURN_CANCEL	= 4,		// Cancel 선택
		eRETURN_SKIP	= 5,		// Skip 혹은 Bypass
		eRETURN_CUSTOM	= 6,		// 사용자가 지정한 항목 선택


		// Alarm 발생시 사용자에게 선택을 묻는다.
		eQUERY_NONE		= 0x000,		// 아무것도 묻지 않는다... ?
		eQUERY_RETRY	= 0x001,		// 다시 시도한다, 기본값
		eQUERY_CANCEL	= 0x002,		// 진행을 포기한다.
		eQUERY_SKIP		= 0x004,		// 무시하고 넘긴다. == IGNORE
		eQUERY_CUSTOM	= 0x008,		// 사용자가 지정한 문자열을 보여주고 선택하도록 한다. 
};


enum {		// 각종 지정 값
		eTIME_ServiceDelay	= 1000,			// Service 시작 지연시간, 실직적으로 서비스는 Thread동작을 이용하지 않는다.
		eTIME_SystemDelay		= 2000,			// System 시작한지 약 2초간 각종 System을 기동하지 않고 대기한다. -> 변경가능


};



enum {	//Max
		eMAX_WaitObjects	= MAXIMUM_WAIT_OBJECTS,	//  Maximum number of wait objects, old : 32,	// Wait 함수류의 최대 객체 수

};


enum {	// Auto Sequence내 Sub Index
	// 각 Station에서 m_nAutoSeqSub의 값으로 사용되어이진다. 상수값의 난립을 막기위함
	// 현재는 최대 20번까지 있지만 필요시에 더 추가하여 사용할 수 있다.
	eIDX_AUTO_SUBSTEP_00		= 0,
	eIDX_AUTO_SUBSTEP_01,
	eIDX_AUTO_SUBSTEP_02,
	eIDX_AUTO_SUBSTEP_03,
	eIDX_AUTO_SUBSTEP_04,
	eIDX_AUTO_SUBSTEP_05,
	eIDX_AUTO_SUBSTEP_06,
	eIDX_AUTO_SUBSTEP_07,
	eIDX_AUTO_SUBSTEP_08,
	eIDX_AUTO_SUBSTEP_09,
	eIDX_AUTO_SUBSTEP_10,
	eIDX_AUTO_SUBSTEP_11,
	eIDX_AUTO_SUBSTEP_12,
	eIDX_AUTO_SUBSTEP_13,
	eIDX_AUTO_SUBSTEP_14,
	eIDX_AUTO_SUBSTEP_15,
	eIDX_AUTO_SUBSTEP_16,
	eIDX_AUTO_SUBSTEP_17,
	eIDX_AUTO_SUBSTEP_18,
	eIDX_AUTO_SUBSTEP_19,
	eIDX_AUTO_SUBSTEP_20,

	eIDX_AUTO_SUBSTEP_99,			// 최종 단계
};



typedef CList<CString, CString&> CGxStringList;			// String을 지정하는 CList 
typedef CTypedPtrArray<CPtrArray, CSyncObject*> CSyncObjPtrArray;			// Sync 객체 array, WaitEvent류 함수에서 사용
