#pragma once

#include "NMC2/NMC2.h"
#pragma comment(lib, "NMC2.lib")
#include "MotionModuleDefine.h"

class CNmc2
{
public:
	CNmc2();
	~CNmc2();
	enum
	{
		MAX_AXIS_CNT = 8,
	};
public:
	int		m_nControllerID;
	static DWORD m_dwOpenState;

	NMC_AXES_EXPR	m_nmcData;
	NMC_AXES_MOTION_OUT	NmcOut;	
public:
	BOOL	OpenController(const int nCtrlID);
	BOOL	CloseController();

	BOOL	GetAlarmSignal(const int nAxisNo, LONG& lVal);
	BOOL	GetInPosSignal(const int nAxisNo, WORD& wVal);
	BOOL	GetCmdPos(const int nAxisNo, const double dGearRatio, double& dPos);
	BOOL	GetEncPos(const int nAxisNo, const double dGearRatio, double& dPos);
	BOOL	GetNmcData(NMC_AXES_EXPR& data);
	BOOL	GetTargetPostion(short nAxisNo,double& pTargetPos);

	//--- Step Control
	BOOL	ServoON(int nAxisNo);		// Servo Motor 전용
	BOOL	ServoOFF(int nAxisNo);		// Servo Motor 전용
	BOOL	AlarmClear(const int nAxisNo);// Servo Motor 전용
	BOOL	ZeroPointSet(const int nAxisNo, double lPos, double dGearRatio);
	BOOL	ZeroPointReturn(const int nAxisNo, int nHomeMode, double dGearRatio, double dHomeSpeed, double dAccType = ACC_300ms, double dOffsetPos=0.);
	BOOL	AxisStop(const int nAxisNo, BOOL bEms = FALSE);

	BOOL 	AxisMove(const int nAxisNo, double dPos, double dSpeed, double dGearRatio, double dAccType = ACC_300ms);// 2011.09.30 탈조 방지용

	void	ChangeAxisSpeed(int nInnerAxisNo, double dSpeed, double dGearRatio);

	//--- Check Function
	BOOL	CheckOpen();
	BOOL	CheckServo(const int nAxisNo);	//JHPARK - 2013/05/23
	BOOL	CheckBusy(const int nAxisNo);
	BOOL	CheckZRNC(const int nAxisNo);
	BOOL 	CheckAxisReady(const int nAxisNo, BOOL bIgnoreInposition);
	BOOL	CheckTrouble(const int nAxisNo);
	BOOL	CheckPLimit(const int nAxisNo);
	BOOL	CheckNLimit(const int nAxisNo);

	long PosToPulse(double dPos, double dGearRatio);
	double PulseToPos(int nPulse, double dGearRatio);
};