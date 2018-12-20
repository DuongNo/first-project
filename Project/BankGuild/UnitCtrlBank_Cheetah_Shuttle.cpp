#include "StdAfx.h"
#include "UnitCtrlBank.h"
#include "CheetahApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CUnitCtrlBank::Move_UpperShuttle_LoadPos()
{
	theDeviceMotion.TeachMove( m_nThreadID, TEACH_PARAM::LD_UPPERSHUTTLE_LOAD );
}

BOOL CUnitCtrlBank::Chk_UpperShuttle_LoadPos()
{
	return theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::LD_UPPERSHUTTLE_LOAD);
}

void CUnitCtrlBank::Move_UpperShuttle_UnLoadPos()
{
	theDeviceMotion.TeachMove( m_nThreadID, TEACH_PARAM::LD_UPPERSHUTTLE_UNLOAD );
}

BOOL CUnitCtrlBank::Chk_UpperShuttle_UnLoadPos()
{
	return theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::LD_UPPERSHUTTLE_UNLOAD);
}

void CUnitCtrlBank::Move_LowerShuttle_LoadPos()
{
	theDeviceMotion.TeachMove( m_nThreadID, TEACH_PARAM::LD_LOWERSHUTTLE_LOAD );
}

BOOL CUnitCtrlBank::Chk_LowerShuttle_LoadPos()
{
	return theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::LD_LOWERSHUTTLE_LOAD);
}

void CUnitCtrlBank::Move_LowerShuttle_UnLoadPos()
{
	theDeviceMotion.TeachMove( m_nThreadID, TEACH_PARAM::LD_LOWERSHUTTLE_UNLOAD );
}

BOOL CUnitCtrlBank::Chk_LowerShuttle_UnLoadPos()
{
	return theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::LD_LOWERSHUTTLE_UNLOAD);
}

void CUnitCtrlBank::UpperShuttle_VacuumOn( BOOL bVacOn, JIG_CH Ch )
{
	if (Ch == JIG_CH_1)
	{
		SetOutPutIO(Y_UPPER_SHUTTLE_CH1_VAC_ON_SOL,bVacOn);
		SetOutPutIO(Y_UPPER_SHUTTLE_CH1_BLOW_ON_SOL,!bVacOn);
		if (bVacOn)
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_XY_UPPERSTGVAC1ON);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_UPPERSTGVAC1OFF);
		}
		else
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_XY_UPPERSTGVAC1OFF);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_UPPERSTGVAC1ON);
	}
	}
	else if(Ch == JIG_CH_2)
	{
		SetOutPutIO(Y_UPPER_SHUTTLE_CH2_VAC_ON_SOL,bVacOn);
		SetOutPutIO(Y_UPPER_SHUTTLE_CH2_BLOW_ON_SOL,!bVacOn);
		if (bVacOn)
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_XY_UPPERSTGVAC2ON);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_UPPERSTGVAC2OFF);
		}
		else
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_XY_UPPERSTGVAC2OFF);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_UPPERSTGVAC2ON);
		}
	}
	else
	{
		SetOutPutIO(Y_UPPER_SHUTTLE_CH1_VAC_ON_SOL,bVacOn);
		SetOutPutIO(Y_UPPER_SHUTTLE_CH1_BLOW_ON_SOL,!bVacOn);
		SetOutPutIO(Y_UPPER_SHUTTLE_CH2_VAC_ON_SOL,bVacOn);
		SetOutPutIO(Y_UPPER_SHUTTLE_CH2_BLOW_ON_SOL,!bVacOn);
		if (bVacOn)
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_XY_UPPERSTGVAC1ON);
			theDeviceMelsec.RobotBitOn(LDROBOT_XY_UPPERSTGVAC2ON);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_UPPERSTGVAC1OFF);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_UPPERSTGVAC2OFF);
		}
		else
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_XY_UPPERSTGVAC1OFF);
			theDeviceMelsec.RobotBitOn(LDROBOT_XY_UPPERSTGVAC2OFF);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_UPPERSTGVAC1ON);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_UPPERSTGVAC2ON);
		}
	}
}

void CUnitCtrlBank::UpperShuttle_BlowOff( JIG_CH Ch )
{
	if (Ch == JIG_CH_1)
	{
		SetOutPutIO(Y_UPPER_SHUTTLE_CH1_BLOW_ON_SOL,FALSE);
	}
	else if(Ch == JIG_CH_2)
	{
		SetOutPutIO(Y_UPPER_SHUTTLE_CH2_BLOW_ON_SOL,FALSE);
	}
	else
	{
		SetOutPutIO(Y_UPPER_SHUTTLE_CH1_BLOW_ON_SOL,FALSE);
		SetOutPutIO(Y_UPPER_SHUTTLE_CH2_BLOW_ON_SOL,FALSE);
	}
}

void CUnitCtrlBank::UpperShuttle_BlowOn( JIG_CH Ch )
{
	if (Ch == JIG_CH_1)
	{
		SetOutPutIO(Y_UPPER_SHUTTLE_CH1_BLOW_ON_SOL,TRUE);
	}
	else if(Ch == JIG_CH_2)
	{
		SetOutPutIO(Y_UPPER_SHUTTLE_CH2_BLOW_ON_SOL,TRUE);
	}
	else
	{
		SetOutPutIO(Y_UPPER_SHUTTLE_CH1_BLOW_ON_SOL,TRUE);
		SetOutPutIO(Y_UPPER_SHUTTLE_CH2_BLOW_ON_SOL,TRUE);
	}
}

void CUnitCtrlBank::LowerShuttle_VacuumOn( BOOL bVacOn, JIG_CH Ch )
{
	if (Ch == JIG_CH_1)
	{
		SetOutPutIO(Y_LOWER_SHUTTLE_CH1_VAC_ON_SOL,bVacOn);
		SetOutPutIO(Y_LOWER_SHUTTLE_CH1_BLOW_ON_SOL,!bVacOn);
		if (bVacOn)
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_XY_LOWERSTGVAC1ON);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_LOWERSTGVAC1OFF);
		}
		else
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_XY_LOWERSTGVAC1OFF);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_LOWERSTGVAC1ON);
		}
	}
	else if(Ch == JIG_CH_2)
	{
		SetOutPutIO(Y_LOWER_SHUTTLE_CH2_VAC_ON_SOL,bVacOn);
		SetOutPutIO(Y_LOWER_SHUTTLE_CH2_BLOW_ON_SOL,!bVacOn);
		if (bVacOn)
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_XY_LOWERSTGVAC2ON);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_LOWERSTGVAC2OFF);
		}
		else
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_XY_LOWERSTGVAC2OFF);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_LOWERSTGVAC2ON);
		}
	}
	else
	{
		SetOutPutIO(Y_LOWER_SHUTTLE_CH1_VAC_ON_SOL,bVacOn);
		SetOutPutIO(Y_LOWER_SHUTTLE_CH1_BLOW_ON_SOL,!bVacOn);
		SetOutPutIO(Y_LOWER_SHUTTLE_CH2_VAC_ON_SOL,bVacOn);
		SetOutPutIO(Y_LOWER_SHUTTLE_CH2_BLOW_ON_SOL,!bVacOn);
		if (bVacOn)
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_XY_LOWERSTGVAC1ON);
			theDeviceMelsec.RobotBitOn(LDROBOT_XY_LOWERSTGVAC2ON);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_LOWERSTGVAC1OFF);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_LOWERSTGVAC2OFF);
		}
		else
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_XY_LOWERSTGVAC1OFF);
			theDeviceMelsec.RobotBitOn(LDROBOT_XY_LOWERSTGVAC2OFF);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_LOWERSTGVAC1ON);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_LOWERSTGVAC2ON);
		}
	}
}

void CUnitCtrlBank::LowerShuttle_BlowOff( JIG_CH Ch )
{
	if (Ch == JIG_CH_1)
	{
		SetOutPutIO(Y_LOWER_SHUTTLE_CH1_BLOW_ON_SOL,FALSE);
	}
	else if(Ch == JIG_CH_2)
	{
		SetOutPutIO(Y_LOWER_SHUTTLE_CH2_BLOW_ON_SOL,FALSE);
	}
	else
	{
		SetOutPutIO(Y_LOWER_SHUTTLE_CH1_BLOW_ON_SOL,FALSE);
		SetOutPutIO(Y_LOWER_SHUTTLE_CH2_BLOW_ON_SOL,FALSE);
	}
}
//2017-11-20, YMG BLOW ON ÇÔ¼ö
void CUnitCtrlBank::LowerShuttle_BlowOn( JIG_CH Ch )
{
	if (Ch == JIG_CH_1)
	{
		SetOutPutIO(Y_LOWER_SHUTTLE_CH1_BLOW_ON_SOL,TRUE);
	}
	else if(Ch == JIG_CH_2)
	{
		SetOutPutIO(Y_LOWER_SHUTTLE_CH2_BLOW_ON_SOL,TRUE);
	}
	else
	{
		SetOutPutIO(Y_LOWER_SHUTTLE_CH1_BLOW_ON_SOL,TRUE);
		SetOutPutIO(Y_LOWER_SHUTTLE_CH2_BLOW_ON_SOL,TRUE);
	}
}

BOOL CUnitCtrlBank::UpperShuttle_VacuumChk(JIG_CH Ch, VAC_STATE Vac)
{
	if(theConfigBank.m_Option.m_bUseDryRun)
		return TRUE;
	if (Ch == JIG_CH_1)
	{
		if (Vac == VAC_ON)
			return	GetInPutIOCheck(X_LD_UPPER_SHUTTLE_CH1_VAC_SEN);
		else
			return	!GetInPutIOCheck(X_LD_UPPER_SHUTTLE_CH1_VAC_SEN);

	}
	else
	{
		if (Vac == VAC_ON)
			return	GetInPutIOCheck(X_LD_UPPER_SHUTTLE_CH2_VAC_SEN);
		else
			return	!GetInPutIOCheck(X_LD_UPPER_SHUTTLE_CH2_VAC_SEN);
	}

	return TRUE;
}

BOOL CUnitCtrlBank::LowerShuttle_VacuumChk(JIG_CH Ch, VAC_STATE Vac)
{
	if(theConfigBank.m_Option.m_bUseDryRun)
		return TRUE;
	if (Ch == JIG_CH_1)
	{
		if (Vac == VAC_ON)
			return   GetInPutIOCheck(X_LD_LOWER_SHUTTLE_CH1_VAC_SEN);
		else
			return   !GetInPutIOCheck(X_LD_LOWER_SHUTTLE_CH1_VAC_SEN);

	}
	else
	{
		if (Vac == VAC_ON)
			return   GetInPutIOCheck(X_LD_LOWER_SHUTTLE_CH2_VAC_SEN);
		else
			return   !GetInPutIOCheck(X_LD_LOWER_SHUTTLE_CH2_VAC_SEN);
	}

	return TRUE;
}
