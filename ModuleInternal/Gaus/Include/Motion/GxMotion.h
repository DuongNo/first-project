//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// CGxMotion.h : 각종 motion 관련 상수 선언
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "GxDefine.h"		// for common constance

// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==

//=================================================================================================
//  MP2100 Servo Setting Parameter
//
enum SETTING_PARAM
{
	RUN_COMMANDS					= 0x00,
	MOTION_COMMAND					= 0x08,
	MOTION_COMMAND_OPTIONS		= 0x09,
	SPEED_REFERENCE				= 0x10,
	SPEED_OVERRIDE					= 0x18,
	POSITION_REFERENCE_TYPE		= 0x1C,
	LINEAR_ACC_TIME				= 0x36,
	LINEAR_DEC_TIME				= 0x38,
	HOME_RETURN_TYPE				= 0x3C,
	HOME_WINDOW						= 0x3D,
	APPROACH_SPEED					= 0x3E,
	CREEP_SPEED						= 0x40,
	HOME_OFFSET_					= 0x42,// NMC와 충돌
};
//=================================================================================================
//  MP2100 Servo Monitoring Parameter
//
enum MONITOR_PARAM
{
	DRIVE_STATUS					= 0x00,
	WARNING							= 0x02,
	ALARM								= 0x04,
	SERVO_CMD_TYPE_RESPONSE		= 0x08,
	SERVO_MODULE_CMD_STATUS		= 0x09,
	POSITION_MANAGEMENT_STATUS	= 0x0C,
	CPOS								= 0x10,		// Target Position
	APOS								= 0x16,		// Feedback Position
	SERVO_ALARM_CODE				= 0x2D,
	NETWORK_SERVO_IO_MONITOR	= 0x2E,
	TORQUE_MONITOR					= 0x42,		//SONIC - 2013.06.20.
};
//=================================================================================================
//  MP2100 Servo Monitoring Parameter Bit
//
enum // MONITOR - IWxx00
{
	DRIVE_STATUS_MOTION_READY	= 0,
	DRIVE_STATUS_RUNNING,
	DRIVE_STATUS_SYSTEM_BUSY,
	DRIVE_STATUS_SERVO_READY,
};
enum // MONITOR - ILxx02
{
	WARNING_EXCESSIVELY_FOLLOWING_ERROR=0,
	WARNING_SETTING_PARAMETER_ERROR,
	WARNING_FIXED_PARAMETER_ERROR,
	WARNING_SERVO_DRIVER_ERROR,
	WARNING_MOTION_COMMAND_SETTING_ERROR,
	WARNING_BIT_5,
	WARNING_POSITIVE_OVERTRAVEL,
	WARNING_NEGATIVE_OVERTRAVEL,
	WARNING_SERVO_NOT_ON,
	WARNING_SERVO_DRIVER_COMMUNICATION_WARNING
};
enum // MONITOR - IWxx09
{
	SERVO_MODULE_COMMAND_STATUS_COMMAND_EXECUTING=0,
	SERVO_MODULE_COMMAND_STATUS_COMMAND_HOLD_COMPLETED,
	SERVO_MODULE_COMMAND_STATUS_BIT_2,
	SERVO_MODULE_COMMAND_STATUS_COMMAND_ERROR_OCCURRENCE,
	SERVO_MODULE_COMMAND_STATUS_BIT_4,
	SERVO_MODULE_COMMAND_STATUS_BIT_5,
	SERVO_MODULE_COMMAND_STATUS_BIT_6,
	SERVO_MODULE_COMMAND_STATUS_RESET_ABSOLUTE_ENCODER_COMPLETED,
	SERVO_MODULE_COMMAND_STATUS_COMMAND_EXECUTION_COMPLETED
};
enum // MONITOR - IWxx0C
{
	POSITION_MANAGEMENT_STATUS_DISTRIBUTION_COMPLETED,
	POSITION_MANAGEMENT_STATUS_POSITIONING_COMPLETED,
	POSITION_MANAGEMENT_STATUS_LATCH_COMPLETED,
	POSITION_MANAGEMENT_STATUS_POSITION_PROXIMITY,
	POSITION_MANAGEMENT_STATUS_ZERO_POINT_POSITION,
	POSITION_MANAGEMENT_STATUS_ZERO_POINT_RETURN_COMPLETED,
	POSITION_MANAGEMENT_STATUS_MACHINE_LOCK_ON,
	POSITION_MANAGEMENT_STATUS_BIT_7,
	POSITION_MANAGEMENT_STATUS_ABS_SYSTEM_INFINITE_LENGTH_POSITION_CONTROL_INFORMATION_LOAD_COMPLETED,
	POSITION_MANAGEMENT_STATUS_POSMAX_TURN_NUMBER_PRESETTING_COMPLETED
};
// Network Servo I/O Monitor
enum
{
	NETWORK_SERVO_IO_MONITOR_POT,
	NETWORK_SERVO_IO_MONITOR_NOT,
	NETWORK_SERVO_IO_MONITOR_DEC,
	NETWORK_SERVO_IO_MONITOR_PA,
	NETWORK_SERVO_IO_MONITOR_PB,
	NETWORK_SERVO_IO_MONITOR_PC,
	NETWORK_SERVO_IO_MONITOR_EXT1,
	NETWORK_SERVO_IO_MONITOR_EXT2,
	NETWORK_SERVO_IO_MONITOR_EXT3,
	NETWORK_SERVO_IO_MONITOR_BRK,
	NETWORK_SERVO_IO_MONITOR_IO12,
	NETWORK_SERVO_IO_MONITOR_IO13,
	NETWORK_SERVO_IO_MONITOR_IO14,
	NETWORK_SERVO_IO_MONITOR_IO15
};



// 각종 상수 선언
enum {

	//eGxMOTION_UnitTimeMiliSec	= 1000,	// 시간 단위가 밀리초 일경우 적용되는 값
	//eGXMOTION_UnitTimeSec		= 1,	// 시간 단위가 초 일경우 적용되는 값
	//eGXMOTION_UnitTime			= eGxMOTION_UnitTimeMiliSec,			// 현재는 밀리초 단위로 사용된다.


	// 최대값
	eGxMOTION_MaxAxis			= 32,		// 각 모션 컨트롤러에서 지원 가능한 최대 축 수, 현재는 32축이지만, 보드별로 달라질 수 있다.
	eGxMOTION_MaxPosition	= 8,		// 모션에서 사용하는 각 위치 데이터의 최대 수량, TGxPosition에서 사용
	eGxMOTION_MaxNMCAxis		= 8,		// NMC 모션 제어기에서 사용하는 제어기당 최대 제어 축 수 
	eGxMOTION_MaxYMCAxis		= 32,		// YMC (MP2100) 모션 제어기에서 사용하는 제어기당 최대 제어 축 수 
	eGxMOTION_MaxAutoRetry	= 3,		// 이동명령에 대한 자동 재시도 횟수 지정
	eGXMOTION_MaxSpeedCount	= 10,		// 이동 속도 지정가능 수량, 최대 10개 까지 지정가능


	// 지원가능한 Motion 제어기 종류,
	// Paix社 NMC1 NMC2, Yaskawa社 MP2100 지원이 가능하다.
	eGxMOTION_CmdNMC2		= 0,			// PAIX NMC2 제어기 (주로 사용하는 제어기)
	eGxMOTION_CmdNMC1,					// PAIX NMC1 제어기 (구형으로 사용 빈도가 떨어져서 NMC2보다 후순위이다)
	eGxMOTION_CmdYMC,						// Yaskawa MP2100 (과거 사용했던 제어어기로 다축을 사용하는 장비에서 사용한다.)


	// Robot 종류를 구분
	eGXMOTION_RobotGeneral	= 0,		// 일반적인 Robot
	eGXMOTION_Robot_______,				// 특수한 목적의 Robot (현재는 미정의)



	// Jog 속도 단계별 지정
	eGxMOTION_JogSpeedLow	= 0,		// Jog 저속	
	eGxMOTION_JogSpeedMid,				// Jog 중속	
	eGxMOTION_JogSpeedHigh,				// Jog 고속	

	// 회전 방향 (JOG Move時)
	eGxMOTION_DirCW			= 0,		// CW 방향
	eGxMOTION_DirCCW,						// CCW 방향


	// 이동 명령에 대한 반환 상태값
	eGxMResult_Done		= 0,			// 정상적으로 이동을 마쳤다.
	eGxMResult_Busy,						// 아직 이동 중이다.
	eGxMResult_Pause,						// 요청에 의해 이동이 중지되었다.
	eGxMResult_Error,						// 이동중 오류가 발생하였다.


	// 기본값
	eGxMOTION_HomeTimeout		= 15000,		// 원점 찾기 기본 설정시간 : 15초
	eGxMOTION_MoveTimeout		= 5000,		// 이동 기본 설정시간 : 5초
	eGxMOTION_SettlingTime		= 25,			// 이동 완료 후 시본 안정화 시간, 0.025초
	eGxMOTION_AlarmResetTime	= 300,		// Alarm Reset 출력 대기 시간
	eGxMOTION_CheckInterval		= 25,			// 이동 완료 여부를 체크하는 간격, 0.025초


	// 각종 Alarm 내용 조사시 어떠한 상태일때 오류로 인식할것인지 모드 
	eGxMMode_CheckNone			= 0,		// 오류 내용을 조사하지 않는다. 단순 조회용
	eGxMMode_CheckOn,						// On이 되었을 때 오류가 발생하였다고 처리한다 (예: Alarm On 등)
	eGxMMode_CheckOff,						// Off가 되었을 때 오류가 발생하였다고 처리한다. (예: Servo On, Servo Ready 등)


	// 특수한 위치 계산을 위한 설정값들
	// CGxAxis::ConvertPosition() 에서 사용된다.
	eGxMPos_Normal					= 0,		// 일반적인 계산식 사용
	eGxMPos_JMRTurn,							// JMR Turn 축 이송량 산출식 사용
	// ... 필요시 추가 

	eGxMMode_MachineToWorld		= 0,		// 기계 좌표계 -> 실제 사용되는 좌표 위치로 변환시킨다. 예) 5mm -> 2.0653도
	eGxMMode_WorldToMachine,				// 실 사용 좌표 위치 -> 기계 좌표 위치로 변환. 예) 2.0653 도 -> 5 mm

};


// 기본적인 모션관련 오류 내용
enum _GxMotionError {
	eGxMError_None					= 0,	// 오류가 없다.

	eGxMError_Moving,						// 아직 이동중이다. 오류는 아님

	// 모션 및 시퀀스 동작중 자신의 문제가 아닌, 요청에 의한 중지에 대해서 이유를 지정한다.
	eGxMError_StopBreak,				// 중지 요청(STOP)에 의해 중지 되었다. (오류가 아님, 다시 시도)
	eGxMError_StopAlarm,				// 다른곳에서 발생한 알람에 의해 중지 되었다. (오류가 아님, 다시 시도)
	eGxMError_StopEMG,				// 중대한 원인으로 긴급 정지를 해야한다. (오류아님, 다시 시도)
	eGxMError_StopNormal,			// 오류에 의한 Stop이 아닌 요청에 의한 정지


	// 이하는 실제적으로 오류가 발생한 경우이다.
	// 초기 설정 문제
	eGxMError_ErrorState			= 10,	// 이하는 이동에 대한 오류가 발생하였다.
	eGxMError_OpenFail,				// 지정 모션보드 장치 오픈 실패
	eGxMError_AxisInitFail,			// 지정축의 초기화 실패
	eGxMError_DataMiss,				// 지정 데이터가 잘못되었다.
	eGxMError_AxisNoMiss,			// 축번호 지정 오류
	eGxMError_AxisEmpty,				// 지정된 축이 존재하지 않는다. 

	// 각종 포인터 지정 문제
	eGxMError_CmdAssign,			// 모션 보드 지정 오류 (모션 보드 지정을 하지 않음)
	eGxMError_AxisAssign,			// 축 객체 지정 오류 (축 지정을 하지 않음)
	eGxMError_RobotAssign,			// Robot 객체 지정 오류 (Robot을 지정하지 않음)
	eGxMError_TaskAssign,			// Task (Statin/System/Service) 미지정

	// 실행시 안전 조건 문제
	eGxMError_Interlock,				// 안전관련 인터락 문제 발생

	// 실행 상태 문제
	eGxMError_ServoOff,				// Servo가 OFF 되었다.
	eGxMError_ServoOn,				// Servo가 On 상태이다.
	eGxMError_ServoReadyOff,		// Servo Ready Off 상태이다.
	eGxMError_ServoReadyOn,			// Servo Ready가 ON 상태이다.
	eGxMError_ServoAlarmOn,			// Servo Alarm On 상태이다.
	eGxMError_ServoAlarmOff,		// Servo Alarm Off 상태이다.
	eGxMError_MoveReadyOn,			// 이동이 가능한 상태이다.
	eGxMError_MoveReadyOff,			// 이동이 불가능한 상태이다.
	eGxMError_InPosOn,				// 정지 상태이다.
	eGxMError_InPosOff,				// 정지 상태가 아니다
	eGxMError_BusyOn,					// 이송 명령 수행중이다.
	eGxMError_BusyOff,				// 이송 명령 수행중이 아니다.
	eGxMError_PosLimitOn,			// 양의 방향 리미트 감지
	eGxMError_PosLimitOff,			// 양의 방향 리미트 미감지
	eGxMError_NegLimitOn,			// 음의 방향 리미트 감지
	eGxMError_NegLimitOff,			// 음의 방향 리미트 미감지
	eGxMError_HomeSearchOn,			// 원점 찾기 완료 상태
	eGxMError_HomeSearchOff,		// 원점 찾기 미완료 상태

	eGxMError_InvalidPos,			// 이동 가능 범위를 벗어난 좌표 지정
	eGxMError_PositionRead,			// 현재 위치를 읽지 못함
	eGxMError_InPosition,			// 지정한 위치가 아니다.

	eGxMError_SetServoOn,			// Servo On을 수행하지 못하였다.
	eGxMError_SetServoOff,			// Servo Off를 수행하지 못하였다.
	eGxMError_SetServoReset,		// Servo Reset을 수행하지 못하였다.
	eGxMError_NotHomeSearch,		// 원점찾기를 수행하지 않았다.
	eGxMError_HomeSearchStart,		// 원점찾기 이동 명령 지령 실패
	eGxMError_HomeSearchDone,		// 원점찾기 동작이 정상적으로 완료되지 못 함
	eGxMError_MoveStart,				// 이동명령 지령 실패
	eGxMError_Timeout,				// 시간초과 오류
};



// 축 정보를 저장하는 구조체
typedef struct _GAUS_LIB _TGxAxisData {
	CString		sKeyName;			// Data file내 Section 이름
	CString		sName;				// 축 이름
	UINT			nCmdType;			// Control 보드 타입, 0: MP2100, 1: NMC
	UINT			nCmdIndex;			//2014-12-24, jhLee, 이 축이 속해있는 모션 보드의 GxMotionCmdMgr에서의 순번(Index)
											//				GxMotionCmdMgr->GetMotionCmd( nCmdIndex ) 에서 사용되어 Cmd를 불러온다.
	UINT			nAxisNo;				// 축 번호, 지정 모션 보드에서 사용되는 축 순서 번호, MP2100: 0 ~ 4, NMC: 0 ~ 8
	UINT			nAmpNo;				//2015-01-01, jhLee, Servo Amp 순번, Panasonic Amp와 직접 통신시 사용

	BOOL			bSimulate;			// 가상의 축인가 ?

	// - - - - - - - - -- - - - - - 
	UINT			nMoveTimeout;		// 이동 완료 제한시간, msec
	UINT			nHomeTimeout;		// 원점찾기 완료 제한시간, msec
	UINT			nAlarmResetDelay;	// Servo Alarm Reset 신호 유지 시간 msec


	// 각종 상태 기억
	double		dRelDestPos;		// 상대이동 변위값
	double		dAbsDestPos;		// 절대이동 변위값


	double		dRunSpeed;			// 이동 속도
	double		dJogSpeedLow;		// 저속 Jog 속도
	double		dJogSpeedMid;		// 중속 Jog 속도
	double		dJogSpeedHi;		// 고속 Jog 속도
	UINT			nModeJogSpeed;		// 선택된 조그 속도 index

	// 2016-06-22, jhLee, 속도값 다수 지정 가능
	double		dRunSpeeds[eGXMOTION_MaxSpeedCount];		// 이동 속도 지정가능 수량, 최대 10개 까지 지정가능
	UINT			nIdxRunSpeed;		// 선택된 이동 속도 Index

	double		dMinAccel;		 // only check range
	double		dMaxAccel;		 // only check range
	double		dRunAccel;		 // 가속도
	
	double 		dMinDecel;		 // only check range
	double		dMaxDecel;		 // only check range
	double		dRunDecel;		 // 감속도
	double		dEStopDecel;	 

	double		dMinJerk;
	double		dMaxJerk;
	double		dRunJerk;

	double		dMinSpeed;			// mm/s		// only check range
	double		dMaxSpeed;			// mm/s		// only check range
	double		dFastHomeSpeed;		// mm/s
	double		dSlowHomeSpeed;		// mm/s

	double		dNegSwLimit;		// mm
	double		dPosSwLimit;		// mm


	UINT			nHomeMode;
	UINT			nHomeSeq;
	UINT			nMoveDir;			// 원점 방향
	double		dHomeOffset;		// 원점 찾기 후 어느정도 이동하여 0 으로 설정하는가 ?


	BOOL			bHomeLevel;			// 각종 Sensor Level
	BOOL			bPosHwLevel;
	BOOL			bNegHwLevel;
	BOOL			bAmpFaultLevel;

	BOOL			bIsEncoder;			// TRUE(Servo Motor), FALSE(Step Motor)
	BOOL			bUseBrake;

	BOOL			bIsABSEncoder;				// 0 : Incremental, 1 : Absolute encoder
	double		dScale;						// count/mm : 기어비
	double		dPulsePerRevolution;		// 1회전에 필요한 Pluse 수
	double		dUnitPerRevolution;		// 1회전당 이동 거리 mm 단위
	double		dInposition;				// Count, 정지 상태임을 인지하는 범위

	//-------------------------------------------------------------------------
	// Internal Variable
	//-------------------------------------------------------------------------
	BOOL		bHomeChecked;			// 원점 찾기를 수행완료 하였는지 표시하는 내부 함수
	BOOL		bMasterEmgStop;			// Master 정지시 긴급정지를 할 것인가?
	BOOL		bResetRequest;			// Servo Off/Alarm등의 이유로 Servo를 Reset할 필요가 있는가 ?

	// Inposition 이후 완료 검사시 정지상태 확인용
	UINT		nSettlingTime;			// Motion-Done, Inposition 신호 입력 후 얼마간의 지연 후 실제 Inposition을 내보낼 것인가 ?
	UINT		nDoneCheckCnt;			// Motion 완료 점검시 몇번이나 완료로 인지되었는가 ?
	UINT		nDoneCheckCount;		// Motion-Done이 몇번이상 들어왔을 때 실제 Inpos로 인지할 것인가 ? 0 -> 실행안함
	UINT		nDoneCheckInterval;		// Motion-Done 검사 간격 msec -> 0 실행안함
	
	// 가상 동작시 상태저장
	BOOL		bSimul_ServoOn;		// 테스트용, 서보On/Off
	double	dSimul_CurrentPos;	// 테스트용, 현재 좌표

	// 현재 상태 
	BOOL		bSTOPRequest;			// 정지 요청이 들어왔는가 ? TRUE:정지 요청에 의한 중지, FALSE 정지 요청이 없었다.
	BOOL		bSafetyInterrupt;		// STO 등의 정지 요청에 의해 정지되었는지 여부 Flag, STO 발동 뒤 정지되었는지 기존 bSTOPRequest와는 용도가 다르다.

	BOOL		bIsMoveDone;			// 지령한 이동을 정상적으로 종료하였는가 ? (정지(InPos) 및 위치 확인 완료)
	BOOL		bIsMoving;				// 이동 동작중인가 ?
	double	dDestPos;				// 이동하고자 하는 거리, mm
	double	dPosErr;					// Target 위치와 현재 위치의 차이, 완료 검사시에 갱신된다.
	double	dFeedbackPos;			// 현재 위치
	double	dEncoderPos;			// Encoder로 부터 '직접'읽은 위치 값, 주로 Absolute encoder의 값을 읽어와서 보관하는데 사용된다.
	BOOL		bIsAbsRead;				// Abs 인코더 값을 읽었는가 ?

	//--------------------------------------------------------------------------
	// CIM & FDC 관련 데이터
	//--------------------------------------------------------------------------

	double	dMoveStartPos;			// 이동 동작 시작 위치 (mm)
	double	dMoveDestPos;			// 이동 동작 완료 위치 (mm)
	double	dMoveTorque;			// 이동시 측정된 현재 Torque값
	double	dMoveTorqueMax;		// 이동시 측정된 최대 Torque값


	// 추가 기능 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// 2016-05-31, jhLee, 좌표 계산시에 특수한 처리를 위한 처리 옵션 지정

	UINT		nPositionType;			// 위치 계산 처리방법 0:아무처리도 없다. 1:JMR Turn축 계산식 사용, 2: 미정...
	double	dPositionArg1;			// 위치 계산시 사용되는 특정한 값, nPositionType 설정이 JMR Turn축 Type인 경우는 중김반경 R mm

	// 2016-07-12, jhLee, NMC2에서 원점 센서 인식 시간을 설정한다. 기본은 300msec로 지정되어 원점 속도가 높으면 센서를 지나치게된다.




	// 생성자
	_TGxAxisData()
	{
		sKeyName			= _T("");			// Data file내 Section 이름
		sName				= _T("");				// 축 이름
		nCmdType			= 0;			// Control 보드 타입, 0: MP2100, 1: NMC
		nCmdIndex		= 0;			//2014-12-24, jhLee, 이 축이 속해있는 모션 보드의 GxMotionCmdMgr에서의 순번(Index)
								//				GxMotionCmdMgr->GetMotionCmd( nCmdIndex ) 에서 사용되어 Cmd를 불러온다.
		nAxisNo			= 0;				// 축 번호, 지정 모션 보드에서 사용되는 축 순서 번호, MP2100: 0 ~ 4, NMC: 0 ~ 8
		nAmpNo			= 0;				//2015-01-01, jhLee, Servo Amp 순번, Panasonic Amp와 직접 통신시 사용

		bSimulate		= TRUE;			// 가상의 축인가 ?

		// - - - - - - - - -- - - - - - 
		nMoveTimeout		= 0;		// 이동 완료 제한시간, msec
		nHomeTimeout		= 0;		// 원점찾기 완료 제한시간, msec
		nAlarmResetDelay	= 0;	// Servo Alarm Reset 신호 유지 시간 msec


		// 각종 상태 기억
		dRelDestPos			= 0.0;		// 상대이동 변위값
		dAbsDestPos			= 0.0;		// 절대이동 변위값


		dRunSpeed			= 0.1;			// 이동 속도
		dJogSpeedLow		= 0.1;		// 저속 Jog 속도
		dJogSpeedMid		= 0.1;		// 중속 Jog 속도
		dJogSpeedHi			= 0.1;		// 고속 Jog 속도
		nModeJogSpeed		= 0;			// 선택된 조그 속도 index

		// 2016-06-22, jhLee, 속도값 다수 지정 가능
		nIdxRunSpeed	= 0;								// 선택된 이동 속도 Index
		for (int i = 0; i < eGXMOTION_MaxSpeedCount; i++)
		{
			dRunSpeeds[i]	= 0.0;						// 이동 속도 지정가능 수량, 최대 10개 까지 지정가능
		}

		dMinAccel			= 1.0;		// only check range
		dMaxAccel			= 1.0;		// only check range
		dRunAccel			= 1.0;		// 가속도
	
		dMinDecel			= 1.0;		 // only check range
		dMaxDecel			= 1.0;		 // only check range
		dRunDecel			= 1.0;		 // 감속도
		dEStopDecel			= 1.0;			 

		dMinJerk				= 1.0;
		dMaxJerk				= 1.0;
		dRunJerk				= 1.0;

		dMinSpeed			= 1.0;			// mm/s		// only check range
		dMaxSpeed			= 1.0;			// mm/s		// only check range
		dFastHomeSpeed		= 1.0;		// mm/s
		dSlowHomeSpeed		= 1.0;		// mm/s

		dNegSwLimit			= 1.0;		// mm
		dPosSwLimit			= 1.0;		// mm

		nHomeMode			= 0;
		nHomeSeq				= 0;
		nMoveDir				= 0;				// 원점 방향
		dHomeOffset			= 0.0;			// 원점 찾기 후 어느정도 이동하여 0 으로 설정하는가 ?

		//	bHomeLevel			= FALSE;		// 각종 Sensor Level
		//	bPosHwLevel			= FALSE;
		//	bNegHwLevel			= FALSE;
		//	bAmpFaultLevel		= FALSE;

		bIsEncoder			= FALSE;			// TRUE(Servo Motor), FALSE(Step Motor)
		bUseBrake			= FALSE;
		bIsABSEncoder		= FALSE;			// 0 : Incremental, 1 : Absolute encoder
		dScale				= 1.0;			// count/mm : 기어비
		dInposition			= 1.0;			// Count, 정지 상태임을 인지하는 범위
		dPulsePerRevolution	= 1.0;		// 1회전에 필요한 Pluse 수
		dUnitPerRevolution	= 1.0;		// 1회전당 이동 거리 mm 단위

		//-------------------------------------------------------------------------
		// Internal Variable
		//-------------------------------------------------------------------------
		bHomeChecked		= FALSE;			// 원점 찾기를 수행완료 하였는지 표시하는 내부 함수
		bMasterEmgStop		= FALSE;			// Master 정지시 긴급정지를 할 것인가?
		bResetRequest		= FALSE;			// Servo Off/Alarm등의 이유로 Servo를 Reset할 필요가 있는가 ?

		// Inposition 이후 완료 검사시 정지상태 확인용
		nSettlingTime			= 0;			// Motion-Done, Inposition 신호 입력 후 얼마간의 지연 후 실제 Inposition을 내보낼 것인가 ?
		nDoneCheckCnt			= 0;			// Motion 완료 점검시 몇번이나 완료로 인지되었는가 ?
		nDoneCheckCount		= 0;			// Motion-Done이 몇번이상 들어왔을 때 실제 Inpos로 인지할 것인가 ? 0 -> 실행안함
		nDoneCheckInterval	= 0;			// Motion-Done 검사 간격 msec -> 0 실행안함
	
		// 가상 동작시 상태저장
		bSimul_ServoOn			= FALSE;		// 테스트용, 서보On/Off
		dSimul_CurrentPos		= 0.0;		// 테스트용, 현재 좌표

		// 현재 상태 
		bSTOPRequest			= FALSE;		// 정지 요청이 들어왔는가 ? TRUE:정지 요청에 의한 중지, FALSE 정지 요청이 없었다.
		bSafetyInterrupt		= FALSE;		// STO 등의 정지 요청에 의해 정지되었는지 여부 Flag, STO 발동 뒤 정지되었는지 기존 bSTOPRequest와는 용도가 다르다.

		bIsMoveDone				= FALSE;		// 지령한 이동을 정상적으로 종료하였는가 ? (정지(InPos) 및 위치 확인 완료)
		bIsMoving				= FALSE;		// 이동 동작중인가 ?
		dDestPos					= 0.0;		// 이동하고자 하는 거리, mm
		dPosErr					= 0.0;		// Target 위치와 현재 위치의 차이, 완료 검사시에 갱신된다.
		dFeedbackPos			= 0.0;		// 현재 위치
		dEncoderPos				= 0.0;		// Encoder로 부터 '직접'읽은 위치 값, 주로 Absolute encoder의 값을 읽어와서 보관하는데 사용된다.
		bIsAbsRead				= FALSE;		// Abs 인코더 값을 읽었는가 ?

		//--------------------------------------------------------------------------
		// CIM & FDC 관련 데이터
		//--------------------------------------------------------------------------

		dMoveStartPos			= 0.0;		// 이동 동작 시작 위치 (mm)
		dMoveDestPos			= 0.0;		// 이동 동작 완료 위치 (mm)
		dMoveTorque				= 0.0;		// 이동시 측정된 현재 Torque값
		dMoveTorqueMax			= 0.0;		// 이동시 측정된 최대 Torque값


		// 추가 기능 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
		// 2016-05-31, jhLee, 좌표 계산시에 특수한 처리를 위한 처리 옵션 지정

		nPositionType			= eGxMPos_Normal;	// = 0,		// 일반적인 계산식 사용	// 위치 계산 처리방법 0:아무처리도 없다. 1:JMR Turn축 계산식 사용, 2: 미정...
		dPositionArg1			= 0.0;		// 위치 계산시 사용되는 특정한 값, nPositionType 설정이 JMR Turn축 Type인 경우는 중김반경 R mm
	};

} TGxAxisData;
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --




// 이동시 사용하는 좌표 포인터 자료형
//
// 본 구조체의 사용 용도 by jhLee
// Gaus2 부터는 Library 안에서 Alarm을 발생시키고 사용자의 선택을 받아 재시도를 실행시키게되었다.
// 이때 Alarm 발생 후 작업자가 티칭을 다시하여 목표 위치가 변경되면 이미 재시도중인 좌표는 실제 티칭 좌표와는 괴리가 발생한다.
// 이를 방지하기 위해 자동 실행일경우에는 위치 티칭값을 보관하고있는 주소를 대입시켜줘서 알람발생 후 재시도시에 지정 주소의 값을 참조하여
// 실제 티칭값과 라이브러리 안에서 사용중인 값을 항상 일치시켜주도록 한다.
// 다만, 메뉴얼 수행일경우에는 단발성으로 끝나기 때문에 실제 값에 위해 수행을 시키고, 자동 실행일 경우에만 주소에 의한 실행을 수행한다.
// 베트남에서....
//
typedef struct _GAUS_LIB _TGxRefPosition
{
	// 현재는 최대 8축 까지만 지원, 확대 가능, eGxMOTION_MaxPosition값 참조
	double*	pPosAddr[eGxMOTION_MaxPosition];		// 각 축의 Target Position을 담고있는 변수 포인터
	double*	pSpeedAddr[eGxMOTION_MaxPosition];		// 각 축의 Speed를 담고있는 변수 포인터

	_TGxRefPosition() { Clear(); };								// 초기화

	void Clear()		// 초기화
	{	
		for (int i = 0; i < eGxMOTION_MaxPosition; i++)	{ pPosAddr[i] = NULL;	pSpeedAddr[i] = NULL; }
	};

	void Set(const _TGxRefPosition &SrcPos)			// 주소 치환
	{ 
		for (int i = 0; i < eGxMOTION_MaxPosition; i++) 
		{
			pPosAddr[i] = SrcPos.pPosAddr[i]; 
			pSpeedAddr[i] = SrcPos.pSpeedAddr[i]; 
		}
	};

	void operator= (const _TGxRefPosition& SrcPos)			// 대입 연산자 정의
	{ 
		for (int i = 0; i < eGxMOTION_MaxPosition; i++) 
		{
			pPosAddr[i] = SrcPos.pPosAddr[i]; 
			pSpeedAddr[i] = SrcPos.pSpeedAddr[i];
		}
	};

	void SetPositionAddr(int nIdx, double& SrcPos)				// 위치 값 주소를 대입
	{ if ((nIdx >= 0) && (nIdx < eGxMOTION_MaxPosition)) pPosAddr[nIdx] = &SrcPos; };		// 원본 변수의 주소를 빼서 대입시킨다. 

	void SetSpeedAddr(int nIdx, double& SrcSpeed)					// 속도 값 주소를 대입
	{ if ((nIdx >= 0) && (nIdx < eGxMOTION_MaxPosition)) pSpeedAddr[nIdx] = &SrcSpeed; };	// 원본 변수의 주소를 빼서 대입시킨다. 

	//void GetPositionAddr(int nIdx, double* pPos)							// 위치 값 조회
	//{ if ((nIdx >= 0) && (nIdx < eGxMOTION_MaxPosition)) pPos = pPosAddr[nIdx]; };			// 위치 주소 값 조회

	//void GetPositionValue(int nIdx, double& dbPos)							// 위치 값 조회
	//{ if ((nIdx >= 0) && (nIdx < eGxMOTION_MaxPosition)) dbPos = ( (pPosAddr[nIdx] != NULL) ? *pPosAddr[nIdx] : 0.0);  };			// 위치 주소 값 조회

	double* GetPositionAddr(int nIdx)							// 위치 값 조회
	{ if ((nIdx >= 0) && (nIdx < eGxMOTION_MaxPosition)) return pPosAddr[nIdx]; return NULL; };			// 위치 주소 값 조회

	double* GetSpeedAddr(int nIdx)							// 속도 값 조회
	{ if ((nIdx >= 0) && (nIdx < eGxMOTION_MaxPosition)) return pSpeedAddr[nIdx]; return NULL; };			// 위치 주소 값 조회


	double GetPositionValue(int nIdx)							// 위치 값 조회
	{ if ((nIdx >= 0) && (nIdx < eGxMOTION_MaxPosition)) return ( (pPosAddr[nIdx] != NULL) ? *pPosAddr[nIdx] : 0.0); return 0.0; };			// 위치 값 조회

	double GetSpeedValue(int nIdx)							// 속도 값 조회
	{ if ((nIdx >= 0) && (nIdx < eGxMOTION_MaxPosition)) return ( (pSpeedAddr[nIdx] != NULL) ? *pSpeedAddr[nIdx] : 0.0); return 0.0; };		// 속도 값 조회
} TGxRefPosition, TGxRefMPos;
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


// 이동시 사용하는 좌표 자료형
typedef struct _GAUS_LIB _TGxPosition
{
	// 현재는 최대 8축 까지만 지원, 확대 가능, eGxMOTION_MaxPosition값 참조

	int		nAxisCount;							// 축의 수량
	double	dbPos[eGxMOTION_MaxPosition];		// 각 축의 Target Position 
	double	dbSpeed[eGxMOTION_MaxPosition];		// 각 축의 이동 속도 

	_TGxPosition() { Clear(); };								// 초기화

	void Clear()		// 초기화
	{	nAxisCount = 0;
		for (int i = 0; i < eGxMOTION_MaxPosition; i++)	{ dbPos[i] = 0.0; }
	};

	void Set(const _TGxPosition &Pos)
	{ 
		nAxisCount = Pos.nAxisCount; 
		for (int i = 0; i < eGxMOTION_MaxPosition; i++) 
		{
			dbPos[i]	= Pos.dbPos[i]; 
			dbSpeed[i]	= Pos.dbSpeed[i]; 
		}
	};

	void Set(const _TGxRefPosition& RefPos)			// 주소 위치 구조체에 대한 대입 연산자 정의
	{ 
		for (int i = 0; i < eGxMOTION_MaxPosition; i++)
		{
			if ( RefPos.pPosAddr[i] != NULL)				// 주소가 올바르다면
			{
				dbPos[i] = *RefPos.pPosAddr[i];				// 해당 주소의 값을 대입시키고
				dbSpeed[i] = *RefPos.pSpeedAddr[i];			// 속도값 치환
			}
			else
			{
				dbPos[i] = 0.0;								// 주소가 없다면 0.0 값으로 치환 시킨다.
				dbSpeed[i] = 1.0;
			}
		}
	};

	void operator= (const _TGxPosition& Pos)			// 대입 연산자 정의
	{ 
		nAxisCount = Pos.nAxisCount; 
		for (int i = 0; i < eGxMOTION_MaxPosition; i++) 
		{
			dbPos[i] = Pos.dbPos[i]; 
			dbSpeed[i] = Pos.dbSpeed[i]; 
		}
	};

	void operator= (const _TGxRefPosition& RefPos)			// 주소 위치 구조체에 대한 대입 연산자 정의
	{ 
		for (int i = 0; i < eGxMOTION_MaxPosition; i++)
			if ( RefPos.pPosAddr[i] != NULL)				// 주소가 올바르다면
			{
				dbPos[i] = *RefPos.pPosAddr[i];				// 해당 주소의 값을 대입시키고
				dbSpeed[i] = *RefPos.pSpeedAddr[i];			
			}
			else
			{
				dbPos[i] = 0.0;								// 주소가 없다면 0.0 값으로 치환 시킨다.
				dbSpeed[i] = 1.0;
			}
	};

	void AddPosition(const _TGxPosition& Pos)				// 위치 증감 누적 
	{ 
		for (int i = 0; i < eGxMOTION_MaxPosition; i++) 
			dbPos[i] = dbPos[i] + Pos.dbPos[i]; 
	};

	void SetPosition(int nIdx, double dbNew)				// 위치 값 대입
	{ if ((nIdx >= 0) && (nIdx < eGxMOTION_MaxPosition)) dbPos[nIdx] = dbNew; };		// 위치값 지정

	void SetSpeed(int nIdx, double dbNew)				// 속도 값 대입
	{ if ((nIdx >= 0) && (nIdx < eGxMOTION_MaxPosition)) dbSpeed[nIdx] = dbNew; };		//속도값 지정

//	void GetPosition(int nIdx, double& dbNew)							// 위치 값 조회
//	{ if ((nIdx >= 0) && (nIdx < eGxMOTION_MaxPosition)) dbNew = dbPos[nIdx]; };			// 위치값 조회

	double GetPosition(int nIdx)							// 위치 값 조회
	{ if ((nIdx >= 0) && (nIdx < eGxMOTION_MaxPosition)) return dbPos[nIdx]; return 0.0; };			// 위치값 조회

	double GetSpeed(int nIdx)							// 위치 값 조회
	{ if ((nIdx >= 0) && (nIdx < eGxMOTION_MaxPosition)) return dbSpeed[nIdx]; return 1.0; };		// 속도값 조회

	// 절대위치 이동을 이용한 상대 이동을 할 경우, 특정 축만 이동을 할 경우 활용
	double AddOffset(int nIdx, double dbValue)			// 지정된 거리만큼 현재 값에서 더하고 그 결과를 되돌려 받는다.
	{ if ((nIdx >= 0) && (nIdx < eGxMOTION_MaxPosition)) return dbPos[nIdx] = dbPos[nIdx] + dbValue; return dbPos[nIdx]; };	

} TGxPosition, TGxMPos;
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


// Motion 동작시 사용되는 최상위 파라메터 구조체
typedef struct _GAUS_LIB _TGxMotionParam
{
	BOOL	bIsManual;				// Manual Mode에서의 지령인가 ?, TRUE:Alarm을 일으키지 않고 결과만 되돌린다., FALSE:자동실행용 rRefMPos를 사용하고, 알람을 지원한다.
	BOOL	bIsCommand;				// 지령 명령을 보낼것인가 ?
	BOOL	bIsWaitDone;			// 자동 실행 (Thread) 환경에서 완료를 기다릴 것인지 여부, TRUE:완료대기, FALSE:바로 리턴
	BOOL	bIsCheckHome;			// 원점 복귀를 했는지 검사할 것인지 여부
	BOOL	bIsRefPosition;			// 좌표값을 주소로 전달할 것인가 ?
	BOOL	bIsAutoRetry;			// 이동 명령 지령 및 완료시에 자동으로 재시도를 수행할 것인가 ?
	BOOL	bIsUseInterlockCode;	// Interlock 오류 발생시 해당 Interlock의 Error Code를 ALarm으로 사용 할 것인가 ?
	BOOL	bIsUseStopEvent;		// 이동 중 정지 신호에 의한 정지 기능을 수행 할 것인가 ? 지정된 위치가 아니더라도 오류가 발생되지 않는다.

	int	nReAction;				// Alarm 발생시 어떠한 응답을 기다릴 것인가 ? (Retry, Cancle, Ignore등)
	int	nInterlockMode;		// Interlock 수행 모드 설정, 0:검사 안함, 1 ~ 어떠한 인터락을 조사할 것인가 ?
	int	nErrCode;				// 대표 Alarm Code, 최상위 Alarm 지정, 본 code로 Alarm을 일으킨다.
	int	nTimeout;				// 이동시 제한할 완료 시간, 밀리초단위, INFINITE는 무제한, 0은 완료검사 안함
	int	nIdxSpeed;				// 이동 속도 지정 Index, 0 : 기본 설정 속도, ~ eGXMOTION_MaxSpeedCount

	TGxMPos		rMPos;				// Position Data, 각 축의 Target Position, 현재는 최대 8축까지 
	TGxRefMPos	rRefMPos;			// 목표위치의 주소를 보관하는 구조체, 자동 시퀀스시에 즉각적인 변동값을 수렴하기 위해 사용된다. 

	_TGxMotionParam()					// 기본적인 값 설정 
	{
		bIsManual		= FALSE;		// Manual 명령은 아니다.
		bIsCommand		= TRUE;		// 지령 명령을 보낸다.
		bIsWaitDone		= FALSE;		// 완료 대기
		bIsCheckHome	= TRUE;		// 원점 복귀를 했는지 검사한다
		bIsRefPosition	= FALSE;		// 좌표값을 주소로 전달할 것인가 ?
		bIsAutoRetry	= TRUE;		// 이동 명령 지령 및 완료시에 자동으로 재시도를 수행할 것인가 ?
		bIsUseInterlockCode	= FALSE;// Interlock 오류 발생시 해당 Interlock의 Error Code를 ALarm으로 사용 할 것인가 ?
		bIsUseStopEvent	= FALSE;		// 이동 중 정지 신호에 의한 정지 기능을 수행 할 것인가 ?

		nReAction		= 0;		// eQUERY_RETRY;			// 기본적으로 재시도를 한다고 설정
		nInterlockMode	= 0;		// Interlock 검사 안함.

		nErrCode			= 0;		// Alarm 발생 안함
		nTimeout			= 0;		// 완료를 기다리지 않음

		nIdxSpeed		= -1;		// 이동 속도 지정 Index, -1 : 기본 설정 속도 사용

		rMPos.Clear();				// 좌표 초기화
		rRefMPos.Clear();			// 주소 좌표 초기화
	};


	void SetManual(int nTime)		// Manual 동작 수행용 설정
	{
		bIsManual		= TRUE;						// Manual 명령이다.
		nErrCode		= 0;						// Alarm Code
		bIsCommand		= TRUE;						// 지령 명령을 보낸다.
		bIsWaitDone		= TRUE;						// 완료 대기
		bIsCheckHome	= TRUE;						// 원점 복귀를 했는지 검사한다
		nTimeout			= nTime;					// 완료 대기 제한 시간 지정
		nReAction		= eQUERY_CANCEL;			// 알람 발생시 실행을 포기한다.
	};


	// 자동 실행시 사용하는 설정, 완료 대기 및 재시도를 선택
	void SetRetry(int nECode, int nTime)		
	{
		bIsManual		= FALSE;					// Auto Sequence 명령이다.
		nErrCode			= nECode;				// Alarm Code
		bIsCommand		= TRUE;					// 지령 명령을 보낸다.
		bIsWaitDone		= TRUE;					// 완료 대기
		bIsCheckHome	= TRUE;					// 원점 복귀를 했는지 검사한다
		nTimeout			= nTime;					// 완료 대기 제한 시간 지정
		nReAction		= eQUERY_RETRY;		// 알람 발생시 재시도 선택
	};

	void SetRetryCancel(int nECode, int nTime)		// 재시도 및 취소를 응답받기위한 설정
	{
		SetRetry( nECode, nTime );					// 나머지는 재시도 설정과 동일,
		nReAction = nReAction | eQUERY_CANCEL;		// 응답 선택시 취소도 포함시킨다.
	};

	// 이동 지령만 지시하고 완료 검사는 하지 않는다.
	void SetCommandOnly(int nECode)		
	{
		nErrCode			= nECode;				// Alarm Code
		bIsCommand		= TRUE;					// 지령 명령을 보낸다.
		bIsWaitDone		= FALSE;					// 완료 대기
		bIsCheckHome	= TRUE;					// 원점 복귀를 했는지 검사한다
		nTimeout			= 0;						// 완료 대기 제한 시간 지정
		nReAction		= eQUERY_RETRY;		// 알람 발생시 재시도 선택
	};


	// Interlock Alarm Code를 사용하도록 지정한다.
	void SetUseInterlockCode(BOOL bFlag)
	{
		bIsUseInterlockCode = bFlag;	// Interlock 오류 발생시 해당 Interlock의 Error Code를 ALarm으로 사용 할 것인가 ?
	};


	// 좌표를 지정한다.
	void SetDestPosition(int nIdx, double& dbDest)	// 좌표지정
	{
		rMPos.SetPosition(nIdx, dbDest);					// 위치값을 지정한다.
		rRefMPos.SetPositionAddr(nIdx, dbDest);		// 위치값을 담고있는 변수의 주소를 치환한다.
	};

	// 지정 축의 이동 속도를 직접 별도로 지정한다.
	void SetMoveSpeed(int nIdx, double& dbSpeed)		// 속도지정
	{
		rMPos.SetSpeed(nIdx, dbSpeed);					// 속도값을 지정한다.
		rRefMPos.SetSpeedAddr(nIdx, dbSpeed);			// 속도값을 담고있는 변수의 주소를 치환한다.
	};

	// 설정된 속도를 사용하도록 사용 속도의 Index 지정, 기본값은 -1로 지정되어있다.
	BOOL SetMoveSpeedIndex(int nIdx)
	{
		if ( (nIdx >= 0) && (nIdx < eGXMOTION_MaxSpeedCount) )
		{
			nIdxSpeed		= nIdx;							// 이동 속도 지정 Index, 0 : 기본 설정 속도 사용
			return TRUE;
		}

		return FALSE;
	};

	// 좌표값을 주소로 참조할 것인가 ?
	void SetUseRefPosition(BOOL bFlag)				
	{
		bIsRefPosition	= FALSE;						// 좌표값을 주소로 전달할 것인가 ?
	};

	// 이동 중 정지 신호에 의한 정지 기능을 수행 할 것인지 지정한다.
	void SetUseStopEvent(BOOL bFlag)
	{
		bIsUseStopEvent = bFlag;					// 지정 Event가 Set 되면 정지하고, 지정된 위치가 아니더라도 오류가 발생되지 않는다.
	}

} TGxMotionParam, TGxMParam;						// 모션동작 파라메터



// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

