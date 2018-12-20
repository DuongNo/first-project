#include "stdafx.h"
#include "MotionNMC2.h"
#include "CheetahApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

DWORD CNmc2::m_dwOpenState	= 0x00000000;  
CNmc2::CNmc2()
{
}
CNmc2::~CNmc2()
{

}
BOOL CNmc2::OpenController(const int nCtrlID)
{

	short ret = 0;

	ret = nmc_OpenDevice(nCtrlID);
	if(ret != NMC_OK)
	{
		CString errMsg;
		errMsg.Format(_T("nmc_opendevice ErrorCode %d : %d"), ret,nCtrlID);
		theLog[LOG_NMC].AddBuf(errMsg);
		return FALSE;
	}
	
	nmc_OpenDeviceEx(nCtrlID, 60);
	// Turn Table 한바퀴마다 Position Reset [5/15/2013 OSC]
	if (nCtrlID == 1)
		nmc_SetRingCountMode(nCtrlID, 6, 1799999, 1799999, TRUE);

	m_nControllerID = nCtrlID;

	return TRUE;
}
BOOL CNmc2::CloseController()
{
	for(int i=0; i<NMC_CTRL_MAX; i++)
		nmc_CloseDevice(i+1);

	return TRUE;
}
BOOL CNmc2::GetAlarmSignal(const int nAxisNo, LONG& lVal)
{
	if(!CheckOpen())
		return FALSE;

	short sSig = 0;
	//20160919 kw
	short nRet = 0;
	NMCAXESEXPR nmcAxesExpr;
	if(nmc_GetAxesExpress(nAxisNo,&nmcAxesExpr) != NMC_OK)
		return FALSE;
	else
	{
		lVal = nmcAxesExpr.nAlarm[nAxisNo];
	}
	//if(::nmc2_alarm_signal(m_nControllerID, nAxisNo, &sSig) != NMC_OK)

	//if(::nmc_SetAlarmLogic(m_nControllerID, nAxisNo, &sSig) != NMC_OK)
	//	return FALSE;

	//lVal = LONG(sSig);

	return TRUE;
}
BOOL CNmc2::GetInPosSignal(const int nAxisNo, WORD& wVal)
{
	if(!CheckOpen())
		return FALSE;

	short sSig = 0;
	//20160919 kw
	short nRet = 0;
	NMCAXESEXPR nmcAxesExpr;
	if(nmc_GetAxesExpress(nAxisNo,&nmcAxesExpr) != NMC_OK)
		return FALSE;
	else
	{
		wVal = nmcAxesExpr.nInpo[nAxisNo];
	}

	//if(::nmc2_inpos_signal(m_nControllerID, nAxisNo, &sSig) != NMC_OK)
	//	return FALSE;

	//wVal = WORD(sSig);

	return TRUE;
}
BOOL CNmc2::GetCmdPos(const int nAxisNo, const double dGearRatio, double& dPos)
{
	if(!CheckOpen())
		return FALSE;

	//long lPulse;
	int nPulse;
	short ret = nmc_GetCmdPos(m_nControllerID, nAxisNo, &nPulse);
	if(ret != NMC_OK)
	{
		//GetLogCtrl()->AddBuf(LOG_NMC, _T("GetCmdPos\tnmc_GetCmdPos\tret=%d"), ret);
		theLog[LOG_NMC].AddBuf(_T("GetCmdPos\tnmc_GetCmdPos\tret=%d"), ret);
		return FALSE;
	}

	if(dGearRatio>0.f)
		dPos = PulseToPos(nPulse, dGearRatio);
	else
		dPos = double(nPulse);

	return TRUE;
}
BOOL CNmc2::GetEncPos(const int nAxisNo, const double dGearRatio, double& dPos)
{
	if(!CheckOpen())
		return FALSE;

	int nPulse;
	short ret = nmc_GetEncPos(m_nControllerID, nAxisNo, &nPulse);
	if(ret != NMC_OK)
	{
		//GetLogCtrl()->AddBuf(LOG_NMC, _T("GetEncPos\tnmc_GetEncPos\tret=%d"), ret);
		theLog[LOG_NMC].AddBuf(_T("GetEncPos\tnmc_GetEncPos\tret=%d"), ret);
		return FALSE;
	}

	if(dGearRatio>0.f)
		dPos = PulseToPos(nPulse, dGearRatio);
	else
		dPos = double(nPulse);

	return TRUE;
}
BOOL CNmc2::GetNmcData(NMC_AXES_EXPR& data)
{
	if(!CheckOpen())
		return FALSE;

	if(nmc_GetAxesExpress(m_nControllerID,&data) != NMC_OK)
		return FALSE;

	return TRUE;
}
BOOL CNmc2::ServoON( int nAxisNo )
{
	if(!CheckOpen())
		return FALSE;

	//::nmc2_servo_on_off(m_nControllerID, nAxisNo, 0, 1);
	if(nAxisNo == 6) // Turn Table NMC1 Inner ID 6
		nmc_SetServoOn(m_nControllerID, nAxisNo, 1);
	else
		nmc_SetServoOn(m_nControllerID, nAxisNo, 0);
	
	return TRUE;
}

BOOL CNmc2::ServoOFF( int nAxisNo )
{
	if(!CheckOpen())
		return FALSE;

	//::nmc2_servo_on_off(m_nControllerID, nAxisNo, 1, 1);
	if(nAxisNo == 6) // Turn Table NMC1 Inner ID 6
		nmc_SetServoOn(m_nControllerID, nAxisNo, 0);
	else
		nmc_SetServoOn(m_nControllerID, nAxisNo, 1);
	return TRUE;
}
BOOL CNmc2::AlarmClear(const int nAxisNo)
{
	if(!CheckOpen())
		return FALSE;

	nmc_SetAlarmResetOn(m_nControllerID, nAxisNo, 0);// None Blocking function
	Sleep(100);
	nmc_SetAlarmResetOn(m_nControllerID, nAxisNo, 1);// None Blocking function
	return TRUE;
}
BOOL CNmc2::ZeroPointSet(const int nAxisNo, double dPos, double dGearRatio)
{
	if(!CheckOpen())
		return FALSE;

	long lPulse = PosToPulse(dPos,dGearRatio);
	//if(::nmc2_set_enc(m_nControllerID, nAxisNo, lPulse) != NMC_OK)	return FALSE;
	if(nmc_SetEncPos(m_nControllerID, nAxisNo, lPulse) != NMC_OK)	return FALSE;
	//if(::nmc2_set_cmd(m_nControllerID, nAxisNo, lPulse) != NMC_OK)	return FALSE;
	if(nmc_SetCmdPos(m_nControllerID, nAxisNo, lPulse) != NMC_OK)	return FALSE;

	return TRUE;
}
BOOL CNmc2::ZeroPointReturn(const int nAxisNo, int nHomeMode, double dGearRatio, double dHomeSpeed, double dAccType /*= ACC_300ms*/, double dOffsetPos/*=0.*/)
{
	if(!CheckOpen())
		return FALSE;

	long lVelocity = PosToPulse(dHomeSpeed, dGearRatio);

	long lAcc;
	lAcc =  lVelocity * long(dAccType);

	long lDec = lAcc;
	long lOffsetPulse = PosToPulse(dOffsetPos, dGearRatio);

	// Limit 감지시 급정지로 변경
	if(nmc_SetLimitStopMode(m_nControllerID, nAxisNo, 0) != NMC_OK)
		return FALSE;

	//if(::nmc2_scurve_ext(m_nControllerID, nAxisNo, (long)(lVelocity /10.), lAcc, lDec, lVelocity , 0, 0) != NMC_OK)
	if(nmc_SetSCurveSpeed(m_nControllerID, nAxisNo, (long)(lVelocity /10.), lAcc, lDec, lVelocity ) != NMC_OK)
		return FALSE;
	if(nmc_SetHomeSpeed(m_nControllerID, nAxisNo, lVelocity, lVelocity/5, lVelocity/5 ) != NMC_OK)
		return FALSE;

	//if(::nmc2_home_start(m_nControllerID, nAxisNo, nHomeMode, 3, lOffsetPulse) != NMC_OK)	return FALSE;
	if(nmc_HomeMove(m_nControllerID, nAxisNo, nHomeMode, 3, lOffsetPulse,0) != NMC_OK)	return FALSE;
	return TRUE;
}
BOOL CNmc2::AxisStop( const int nAxisNo, BOOL bEms /*= FALSE*/ )
{
	short ret;
	if(bEms)
	{
		//ret = ::nmc2_stop_motor(m_nControllerID, nAxisNo);
		ret = nmc_SuddenStop(m_nControllerID, nAxisNo);
		if(ret != NMC_OK)
		{
			//GetLogCtrl()->AddBuf(LOG_NMC, _T("AxisStop\tnmc2_stop_motor\tret=%d"), ret);
			theLog[LOG_NMC].AddBuf(_T("AxisStop\tnmc_SuddenStop\tret=%d"), ret);
			return FALSE;
		}
	}
	else
	{
		//ret = ::nmc2_slow_stop_motor(m_nControllerID, nAxisNo);
		ret = nmc_DecStop(m_nControllerID, nAxisNo);
		if(ret != NMC_OK)
		{
			//GetLogCtrl()->AddBuf(LOG_NMC, _T("AxisStop\tnmc2_slow_stop_motor\tret=%d"), ret);
			theLog[LOG_NMC].AddBuf(_T("AxisStop\tnmc_DecStop\tret=%d"), ret);
			return FALSE;
		}
	}

	return TRUE;
}
BOOL CNmc2::AxisMove(const int nAxisNo, double dPos, double dSpeed, double dGearRatio, double dAccType /*= ACC_300ms*/ )
{
	if(!CheckOpen())
	{
		TRACE(_T("  Open실패\n"));
		return FALSE;
	}

	long lPulse = PosToPulse(dPos, dGearRatio);
	long lVelocity = PosToPulse(dSpeed, dGearRatio);

	long lAcc;
	lAcc =  (long)( (double)lVelocity * dAccType );// acc time 조절 가능

	long lDec = (long)( (double)lVelocity * dAccType );// dec time 조절 가능

	//long lJerk1, lJerk2;
	//double temp;

	long lStartVel;/*long(lVelocity /20.)*/;
		lStartVel = (lVelocity /20);

	short retSet = nmc_SetSCurveSpeed (m_nControllerID, nAxisNo, lStartVel, lAcc, lDec, lVelocity);
	if(retSet != NMC_OK)
	{
		theLog[LOG_NMC].AddBuf(_T("AxisMove\tnmc_SetSCurveSpeed\tretSet=%d"), retSet);
		return FALSE;
	}
	short nPositionMode;
	nPositionMode = 1; //상대위치 = 0, 절대위치 = 1
	short ret = nmc_VelMove(m_nControllerID, nAxisNo, lPulse, lVelocity, nPositionMode);
	if(ret != NMC_OK)
	{
		theLog[LOG_NMC].AddBuf(_T("AxisMove\tnmc_VelMove\tret=%d"), ret);
		return FALSE;
	}

	return TRUE;
}
void CNmc2::ChangeAxisSpeed( int nInnerAxisNo, double dSpeed, double dGearRatio )
{
	if(!CheckOpen())
	{
		TRACE(_T("  Open실패\n"));
		return;
	}

	long lVelocity = PosToPulse(dSpeed, dGearRatio);


	//long lRtn = ::nmc2_drive_pps(m_nControllerID, nInnerAxisNo, lVelocity);
	long lRtn = nmc_SetOverrideDriveSpeed(m_nControllerID, nInnerAxisNo, lVelocity);
	
	if( lRtn != NMC_OK)
	{
		TRACE(_T("  nmc2_abs_scurve %d \n"), lRtn);
		return;
	}
}
BOOL CNmc2::CheckOpen()
{
	return ((m_dwOpenState>>(m_nControllerID-1))&0x1 ? TRUE : FALSE);
}
BOOL CNmc2::CheckServo(const int nAxisNo)
{
	short nState =0;

	//nmc2_current_outstatus(m_nControllerID, &NmcOut);
	nmc_GetAxesMotionOut(m_nControllerID, &NmcOut);
	return (NmcOut.nServoOn[nAxisNo] &0x1 ? FALSE : TRUE);

}
BOOL CNmc2::CheckBusy(const int nAxisNo)
{
	if(!CheckOpen())
		return FALSE;

	short sSig;
	//if(::nmc2_busy_signal(m_nControllerID, nAxisNo, &sSig) != NMC_OK)
	if(nmc_GetBusyStatus(m_nControllerID, nAxisNo, &sSig) != NMC_OK)
		return FALSE;

	return sSig==1 ? TRUE : FALSE;
}
BOOL CNmc2::CheckZRNC(const int nAxisNo)
{
	if(!CheckOpen())
		return FALSE;

	NMC_HOME_FLAG	homeFlag;
	//if(::nmc2_get_homeflag_all(m_nControllerID, &homeFlag) != NMC_OK)
	if(::nmc_GetHomeStatus(m_nControllerID, &homeFlag) != NMC_OK)
	return FALSE;

	int m = sizeof(NMC_HOME_FLAG);

	//return (/*homeFlag.nSrchFlag[nAxisNo]==1 &&*/homeFlag.nStatusFlag[nAxisNo]==HOME_READY) ? TRUE:FALSE;
	return (/*homeFlag.nSrchFlag[nAxisNo]==1 &&*/homeFlag.nStatusFlag[nAxisNo]==0) ? TRUE:FALSE;
}
BOOL CNmc2::CheckAxisReady(const int nAxisNo, BOOL bIgnoreInposition)
{
	short ret;
	if(!CheckOpen())
		return FALSE;
	ret = nmc_GetAxesExpress(m_nControllerID,&m_nmcData);
	if( ret != NMC_OK)
	{
		//GetLogCtrl()->AddBuf(LOG_NMC, _T("CheckAxisReady\tnmc2_get_sensor_all\tret=%d"), ret);
		theLog[LOG_NMC].AddBuf(_T("CheckAxisReady\tnmc_GetAxesExpress\tret=%d"), ret);
		return FALSE;
	}

	//nError = Limit or Alarm or Emegency 가 온일대 On
	if(bIgnoreInposition)
	{
		if(m_nmcData.nAlarm[nAxisNo] || m_nmcData.nBusy[nAxisNo])
		{
			TRACE(_T("Control=%d,Axis=%d, %d,%d,%d\n"), m_nControllerID ,nAxisNo, m_nmcData.nAlarm[nAxisNo], m_nmcData.nBusy[nAxisNo] , m_nmcData.nInpo[nAxisNo]);
			return FALSE;
		}
	}
	else
	{
		if(m_nmcData.nAlarm[nAxisNo] || m_nmcData.nBusy[nAxisNo] || m_nmcData.nInpo[nAxisNo] == 0)
		{
			TRACE(_T("Control=%d,Axis=%d, %d,%d,%d\n"), m_nControllerID ,nAxisNo, m_nmcData.nAlarm[nAxisNo], m_nmcData.nBusy[nAxisNo] , m_nmcData.nInpo[nAxisNo]);
			return FALSE;
		}
	}
	return TRUE;
}
BOOL CNmc2::CheckTrouble(const int nAxisNo)
{
	// Return Value
	// TRUE = Trouble
	// FALSE = Not Trouble

	if(!CheckOpen())
		return TRUE;


	if(nmc_GetAxesExpress(m_nControllerID,&m_nmcData) != NMC_OK)
		return TRUE;

	//	if(data.nAlarm[nAxisNo] || data.nMLimit[nAxisNo] || data.nPLimit[nAxisNo])
	if(m_nmcData.nError[nAxisNo])// 위와 동일함.
		return TRUE;
	return FALSE;
}
BOOL CNmc2::CheckPLimit( const int nAxisNo )
{
	NMC_AXES_EXPR data;
	GetNmcData(data);
	return data.nPLimit[nAxisNo];
}

BOOL CNmc2::CheckNLimit( const int nAxisNo )
{
	NMC_AXES_EXPR data;
	GetNmcData(data);
	return data.nMLimit[nAxisNo];
}
long CNmc2::PosToPulse(double dPos, double dGearRatio)
{
	return long(CEtc::Round(dPos*dGearRatio));
}
double CNmc2::PulseToPos(int nPulse, double dGearRatio)
{
	return double(nPulse)/dGearRatio;
}
BOOL CNmc2::GetTargetPostion(short nAxisNo,double &pTargetPos)
{
	if(!CheckOpen())
		return FALSE;

	short ret = nmc_GetParaTargetPos(m_nControllerID, nAxisNo, &pTargetPos);
	if(ret != NMC_OK)
	{
		//GetLogCtrl()->AddBuf(LOG_NMC, _T("GetEncPos\tnmc_GetEncPos\tret=%d"), ret);
		theLog[LOG_NMC].AddBuf(_T("GetTargetPostion\tnmc_GetParaTargetPos\tret=%d"), ret);
		return FALSE;
	}
	return TRUE;
}