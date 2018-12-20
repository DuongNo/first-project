#pragma once
#include "BankGuild/UnitCtrlBank.h"


class CStateRestart : 	public CUnitCtrlBank
{
public:
	CStateRestart(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
	
	};
	~CStateRestart(void) {};

	enum
	{
		stepStart,
		step1stVacChk,
		stepBitAllOff,
		stepRobotRunningChk,
		stepRobotPauseOn,
		seqRobotExtStop,
		seqRobotMotorOn,
		stepRobotOnChk,
		stepPGMSel,
		stepPGMStrobe,
		stepExtStart,
		stepRunningChk,
		stepOrgOff,
		stepInterlockChk,
		stepZUpChk,
		stepPaOrgChk,   //2017-12-23,SKCHO
		stepReadyPosChk,
		stepTrayDataDelay,
		stepTrayDataSend,
		stepEnd,
	};

	enum ORIGIN_STATUS
	{
		ORIGIN_NONE,
		ORIGIN_START,
		ORIGIN_FINISH,
		ORIGIN_FAIL,
		ORIGIN_STATUS_MAX
	};
	ALARM_ID m_LastAlarm;

	int Run();

	void Restart_VacuumOn();
	BOOL Restart_VacuumChk();
	void Restart_ZUp();
	BOOL Restart_ZUpChk();
	void Restart_XYTMove();
	BOOL Restart_XYTMoveChk();
	void Restart_InitTrayCylinder();
	BOOL Restart_InitTrayCylinderChk();
	void Restart_PAOrgMove();
	BOOL Restart_PAOrgMoveChk();
	BOOL CheckAxisOriginFail();

	ORIGIN_STATUS m_PAOriginStatus[6];
};

