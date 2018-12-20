#pragma once

#include "ymcPCIAPI/ymcPCAPI.h"
#pragma comment(lib, "ymcPCAPI.lib")
#include "MotionModuleDefine.h"


// Board 1EA당 CMp2100객체 1EA가 생성되어야 한다
class CMp2100
{
public:
	CMp2100();
	~CMp2100();

	enum { MAX_AXIS_CNT_IN_BOARD = 32,	};

	//=================================================================================================
	//  MP2100 Servo Setting Parameter
	//
	enum SETTING_PARAM
	{
		RUN_COMMANDS				= 0x00,
		MOTION_COMMAND				= 0x08,
		MOTION_COMMAND_OPTIONS		= 0x09,
		SPEED_REFERENCE				= 0x10,
		SPEED_OVERRIDE				= 0x18,
		POSITION_REFERENCE_TYPE		= 0x1C,
		LINEAR_ACC_TIME				= 0x36,
		LINEAR_DEC_TIME				= 0x38,
		HOME_RETURN_TYPE			= 0x3C,
		HOME_WINDOW					= 0x3D,
		APPROACH_SPEED				= 0x3E,
		CREEP_SPEED					= 0x40,
		HOME_OFFSET_				= 0x42,// NMC와 충돌
	};
	//=================================================================================================
	//  MP2100 Servo Monitoring Parameter
	//
	enum MONITOR_PARAM
	{
		DRIVE_STATUS				= 0x00,
		WARNING						= 0x02,
		ALARM						= 0x04,
		SERVO_CMD_TYPE_RESPONSE		= 0x08,
		SERVO_MODULE_CMD_STATUS		= 0x09,
		POSITION_MANAGEMENT_STATUS	= 0x0C,
		CPOS						= 0x10,
		APOS						= 0x16,
		SERVO_ALARM_CODE			= 0x2D,
		NETWORK_SERVO_IO_MONITOR	= 0x28,		//Mechatrolink3 변경
//		NETWORK_SERVO_IO_MONITOR	= 0x2E,		//Mechatrolink2.
		TORQUE_MONITOR				= 0x42,		//SONIC - 2013.06.20.
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
	/*
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
	*/

	// Mechatrolink3
	enum
	{
		NETWORK_SERVO_IO_MONITOR_NULL,		// NULL
		NETWORK_SERVO_IO_MONITOR_DEC,
		NETWORK_SERVO_IO_MONITOR_POT,
		NETWORK_SERVO_IO_MONITOR_NOT,
		NETWORK_SERVO_IO_MONITOR_EXT1,
		NETWORK_SERVO_IO_MONITOR_EXT2,
		NETWORK_SERVO_IO_MONITOR_EXT3,
		NETWORK_SERVO_IO_MONITOR_ESTP,
		NETWORK_SERVO_IO_MONITOR_BRK,
		NETWORK_SERVO_IO_MONITOR_PSOT,
		NETWORK_SERVO_IO_MONITOR_NSOT,
		NETWORK_SERVO_IO_MONITOR_DEN,
		NETWORK_SERVO_IO_MONITOR_NEAR,
		NETWORK_SERVO_IO_MONITOR_PEST,
		NETWORK_SERVO_IO_MONITOR_ZPOINT,
	};

protected:
	int				m_nBoardID;
	HCONTROLLER		m_hController;
	HAXIS			m_hAxis[MAX_AXIS_CNT_IN_BOARD];
	HDEVICE			m_hDevice[MAX_AXIS_CNT_IN_BOARD];
	long			m_lAxisAddr[MAX_AXIS_CNT_IN_BOARD];
	//20161019 kw
	HAXIS			m_hAxisSync[1][2];

public:
	static DWORD	m_dwOpenState;
	DWORD	OpenController(const int nCpuNo);
	BOOL	CloseController();
	BOOL	SelectBoard();

	BOOL	SetSyncAxis(int *pAxisArray, const int nAxisCount, int nMasterAxisIndex);

	BOOL	ReadMonitorParam(int nAxisNo, int nOffset, DWORD& dwVal);
	BOOL	ReadSettingParam(int nAxisNo, int nOffset, DWORD& dwVal);
	BOOL	WriteSettingParam(int nAxisNo, int nOffset, DWORD dwVal);

	//--- Monitor Data
	BOOL	GetParam_DriveStatus(int nAxisNo, WORD& wVal);	// IWxx00
	BOOL	GetParam_Warning(int nAxisNo, LONG& lVal);		// ILxx02
	BOOL	GetParam_Alarm(int nAxisNo, LONG& lVal);		// ILxx04
	BOOL	GetParam_CommandType(int nAxisNo, WORD& wVal);	// IWxx08
	BOOL	GetParam_CommandStatus(int nAxisNo, WORD& wVal);// IWxx09
	BOOL	GetParam_PositionStatus(int nAxisNo, WORD& wVal);// IWxx0C
	BOOL	GetParam_TargetPos(int nAxisNo, double& dVal);	// ILxx10
	BOOL	GetParam_FeedbackPos(int nAxisNo, double& dVal);// ILxx16
	BOOL	GetParam_ServoAlarmCode(int nAxisNo, WORD& wVal);// IWxx2D
	BOOL	GetParam_ServoIO(int nAxisNo, WORD& wVal);		// IWxx2E
	BOOL	GetParam_TorqueChk(int nAxisNo, double& dVal);		//SONIC - 2013.06.20.

	//--- Monitor Bit
	BOOL	GetParamBit_ServoON(int nAxisNo);
	BOOL	GetParamBit_CommandReady(int nAxisNo);
	BOOL	GetParamBit_ZRNC(int nAxisNo);

	//--- Servo Control
	BOOL	ServoON(int nAxisNo);
	BOOL	ServoOFF(int nAxisNo);
	BOOL	AlarmClear(int nAxisNo);
	BOOL	BoardAlarmClear();
	BOOL	ZeroPointSet(int nAxisNo, double dPos);
	BOOL ZeroPointReturn(int nAxisNo, double dHomeSpeed, double dAccType = ACC_300ms, int nOriginMode = HMETHOD_HOMELS_ONLY);
	BOOL	AxisMove(int nAxisNo, double dPos, double dVel, SHORT sMoveType, double dAccType = ACC_300ms);
	BOOL	AxisJog(int nAxisNo, double dVel, BOOL bForward = TRUE, double dAccType = ACC_300ms);
	BOOL	AxisStop(int nAxisNo, LONG nDec);
	BOOL	AxisStopJog(int nAxisNo, double dVel);

	//--- Check Function
	BOOL	IsAxisReady(int nAxisNo);
	//BOOL	IsAxisMoveReady(int nAxisNo);
	BOOL	CheckTrouble(int nAxisNo);
	BOOL	CheckPLimit(int nAxisNo);
	BOOL	CheckNLimit(int nAxisNo);

	//20161024 kw 동기제어
	BOOL EnableGear(int wMasterAxisNo, int wSlaveAxisNo);
	BOOL DisableGear(int wMasterAxisNo, int wSlaveAxisNo);
	BOOL GetSyncAxisStatus();

protected:
};
