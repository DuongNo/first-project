#include "StdAfx.h"
#include "UnitCtrlBank.h"
#include "CheetahApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CUnitCtrlBank::Move_PATR_YAxis_GetPos()
{
	theDeviceMotion.TeachMove( m_nThreadID, TEACH_PARAM::PA_TR_Y_GET );
}

BOOL CUnitCtrlBank::Chk_PATR_YAxis_GetPos()
{
	return theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::PA_TR_Y_GET);
}

void CUnitCtrlBank::Move_PATR_YAxis_PutPos()
{
	theDeviceMotion.TeachMove( m_nThreadID, TEACH_PARAM::PA_TR_Y_PUT );
}

BOOL CUnitCtrlBank::Chk_PATR_YAxis_PutPos()
{
	return theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::PA_TR_Y_PUT);
}

void CUnitCtrlBank::Move_PATR_YAxis_WaitPos()
{
	theDeviceMotion.TeachMove( m_nThreadID, TEACH_PARAM::PA_TR_Y_WAIT );
}

BOOL CUnitCtrlBank::Chk_PATR_YAxis_WaitPos()
{
	return theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::PA_TR_Y_WAIT);
}

void CUnitCtrlBank::Move_PATR_ZAxis_GetUpperShuttlePos()
{
	theDeviceMotion.TeachMove( m_nThreadID, TEACH_PARAM::PA_TR_Z_UPPER_GET );
}

BOOL CUnitCtrlBank::Chk_PATR_ZAxis_GetUpperShuttlePos()
{
	return theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::PA_TR_Z_UPPER_GET);
}

void CUnitCtrlBank::Move_PATR_ZAxis_GetLowerShuttlePos()
{
	theDeviceMotion.TeachMove( m_nThreadID, TEACH_PARAM::PA_TR_Z_LOWER_GET );
}

BOOL CUnitCtrlBank::Chk_PATR_ZAxis_GetLowerShuttlePos()
{
	return theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::PA_TR_Z_LOWER_GET);
}

void CUnitCtrlBank::Move_PATR_ZAxis_UpPos(double dSpeed)
{
	theDeviceMotion.TeachMove( m_nThreadID, TEACH_PARAM::PA_TR_Z_UP, dSpeed);
}

BOOL CUnitCtrlBank::Chk_PATR_ZAxis_UpPos()
{
	return theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::PA_TR_Z_UP);
}
void CUnitCtrlBank::Move_PATR_ZAxis_PutPos()
{
	theDeviceMotion.TeachMove( m_nThreadID, TEACH_PARAM::PA_TR_Z_PUT );
}

BOOL CUnitCtrlBank::Chk_PATR_ZAxis_PutPos()
{
	return theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::PA_TR_Z_PUT);
}

void CUnitCtrlBank::PATR_Vacuum_On(const BOOL bVacOn,JIG_CH Ch)
{
	if (Ch == JIG_CH_1)	
	{
		SetOutPutIO(Y_PA_TR_CH1_VAC_ON_SOL, bVacOn);
		SetOutPutIO(Y_PA_TR_CH1_BLOW_ON_SOL, !bVacOn);
	}
	else if (Ch == JIG_CH_2)
	{
		SetOutPutIO(Y_PA_TR_CH2_VAC_ON_SOL, bVacOn);
		SetOutPutIO(Y_PA_TR_CH2_BLOW_ON_SOL, !bVacOn);
	}
	else
	{
		SetOutPutIO(Y_PA_TR_CH1_VAC_ON_SOL, bVacOn);
		SetOutPutIO(Y_PA_TR_CH1_BLOW_ON_SOL, !bVacOn);
		SetOutPutIO(Y_PA_TR_CH2_VAC_ON_SOL, bVacOn);
		SetOutPutIO(Y_PA_TR_CH2_BLOW_ON_SOL, !bVacOn);
	}
}
void CUnitCtrlBank::PATR_BlowOn(JIG_CH Ch)
{
	if (Ch == JIG_CH_1)	
	{
		SetOutPutIO(Y_PA_TR_CH1_BLOW_ON_SOL, TRUE);
	}
	else if (Ch == JIG_CH_2)
	{
		SetOutPutIO(Y_PA_TR_CH2_BLOW_ON_SOL, TRUE);
	}
	else
	{
		SetOutPutIO(Y_PA_TR_CH1_BLOW_ON_SOL, TRUE);
		SetOutPutIO(Y_PA_TR_CH2_BLOW_ON_SOL, TRUE);
	}
}
void CUnitCtrlBank::PATR_BlowOff(JIG_CH Ch)
{
	if (Ch == JIG_CH_1)	
	{
		SetOutPutIO(Y_PA_TR_CH1_BLOW_ON_SOL, FALSE);
	}
	else if (Ch == JIG_CH_2)
	{
		SetOutPutIO(Y_PA_TR_CH2_BLOW_ON_SOL, FALSE);
	}
	else
	{
		SetOutPutIO(Y_PA_TR_CH1_BLOW_ON_SOL, FALSE);
		SetOutPutIO(Y_PA_TR_CH2_BLOW_ON_SOL, FALSE);
	}
}

BOOL CUnitCtrlBank::PATR_Vacuum_Chk(JIG_CH Ch,VAC_STATE Vac)
{
	if(theConfigBank.m_Option.m_bUseDryRun)
		return TRUE;
	if (Ch == JIG_CH_1)
	{
		if (Vac == VAC_ON)
			return   GetInPutIOCheck(X_PA_TR_CH1_VAC_SEN);
		else
			return   !GetInPutIOCheck(X_PA_TR_CH1_VAC_SEN);

	}
	else if(Ch == JIG_CH_2)
	{
		if (Vac == VAC_ON)
			return   GetInPutIOCheck(X_PA_TR_CH2_VAC_SEN);
		else
			return   !GetInPutIOCheck(X_PA_TR_CH2_VAC_SEN);

	}

	return TRUE;
}
