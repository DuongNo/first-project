#include "StdAfx.h"
#include "UnitCtrlBank.h"
#include "CheetahApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CUnitCtrlBank::NGTrayTrX_InPos()
{
	theDeviceMotion.TeachMove( m_nThreadID, TEACH_PARAM::NG_TRAY_TR_X_TO_IN);
}

void CUnitCtrlBank::NGTrayTrX_OutPos()
{
	theDeviceMotion.TeachMove( m_nThreadID, TEACH_PARAM::NG_TRAY_TR_X_TO_OUT);
}

BOOL CUnitCtrlBank::NGTrayTrX_InChk()
{
	return theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::NG_TRAY_TR_X_TO_IN);
}

BOOL CUnitCtrlBank::NGTrayTrX_OutChk()
{
	return theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::NG_TRAY_TR_X_TO_OUT);
}

void CUnitCtrlBank::NGTrayTrZ_UpPos(double dSpeed)
{
	theDeviceMotion.TeachMove( m_nThreadID, TEACH_PARAM::NG_TRAY_TR_Z_TO_UP, dSpeed);
}

void CUnitCtrlBank::NGTrayTrZ_InPos(double dOffset)
{
	theDeviceMotion.TeachMove( m_nThreadID, TEACH_PARAM::NG_TRAY_TR_Z_TO_IN,dOffset);
}

void CUnitCtrlBank::NGTrayTrZ_OutPos()
{
	theDeviceMotion.TeachMove( m_nThreadID, TEACH_PARAM::NG_TRAY_TR_Z_TO_OUT);
}

BOOL CUnitCtrlBank::NGTrayTrZ_UpChk()
{
	return theDeviceMotion.CheckTeachPosition( m_nThreadID, TEACH_PARAM::NG_TRAY_TR_Z_TO_UP);
}
//2017-09-21,SKCHO, GOOD TRAY RETRY 기능 추가로 함수 수정
BOOL CUnitCtrlBank::NGTrayTrZ_InChk(double dOffset)
{
	return theDeviceMotion.CheckTeachPosition( m_nThreadID, TEACH_PARAM::NG_TRAY_TR_Z_TO_IN,0.1, POS_EQUAL,dOffset);
}

BOOL CUnitCtrlBank::NGTrayTrZ_OutChk()
{
	return theDeviceMotion.CheckTeachPosition( m_nThreadID, TEACH_PARAM::NG_TRAY_TR_Z_TO_OUT);
}

void CUnitCtrlBank::NGTrayTr_VacOnOff( VAC_STATE vac )
{
	int nNum;
	ONOFF valVac = (vac== VAC_ON) ? ON:OFF;
#if(AMT_TYPE == 1)
	nNum = Y_NG_TRAY_TR_VAC_ON_SOL_1;		SetOutPutIO(nNum, valVac);
	nNum = Y_NG_TRAY_TR_BLOW_ON_SOL_1;		SetOutPutIO(nNum, !valVac);
	nNum = Y_NG_TRAY_TR_VAC_ON_SOL_2;		SetOutPutIO(nNum, valVac);
	nNum = Y_NG_TRAY_TR_BLOW_ON_SOL_2;		SetOutPutIO(nNum, !valVac);
#else
	nNum = Y_NG_TRAY_TR_VAC_ON_SOL_1;		SetOutPutIO(nNum, valVac);
	nNum = Y_NG_TRAY_TR_BLOW_ON_SOL_1;		SetOutPutIO(nNum, !valVac);
#endif
}

void CUnitCtrlBank::NGTrayTr_BlowOff()
{
#if (AMT_TYPE == 1)
	SetOutPutIO(Y_NG_TRAY_TR_BLOW_ON_SOL_1, FALSE);
	SetOutPutIO(Y_NG_TRAY_TR_BLOW_ON_SOL_2, FALSE);
#else
	SetOutPutIO(Y_NG_TRAY_TR_BLOW_ON_SOL_1, FALSE);
#endif
}

BOOL CUnitCtrlBank::NGTrayTr_VacChk( VAC_STATE vac )
{
	if(theConfigBank.m_Option.m_bUseDryRun)
		return TRUE;
	INPUT_ID nNum1;
	ONOFF val = (vac== VAC_ON) ? ON:OFF;
	nNum1 = X_NG_TRAY_TR_VAC_SEN_1;
	return GetInPutIOCheck(nNum1, val);
}


void CUnitCtrlBank::InNGTrayLiftZ_UpPos()
{
	theDeviceMotion.TeachMove( m_nThreadID, TEACH_PARAM::NG_TRAY_IN_LIFT_Z_TO_UP );
}

BOOL CUnitCtrlBank::InNGTrayLiftZ_UpChk()
{
	return theDeviceMotion.CheckTeachPosition( m_nThreadID, TEACH_PARAM::NG_TRAY_IN_LIFT_Z_TO_UP );
}

void CUnitCtrlBank::InNGTrayLiftZ_DownPos()
{
	theDeviceMotion.TeachMove( m_nThreadID, TEACH_PARAM::NG_TRAY_IN_LIFT_Z_TO_DOWN );
}

BOOL CUnitCtrlBank::InNGTrayLiftZ_DownChk()
{
	return theDeviceMotion.CheckTeachPosition( m_nThreadID, TEACH_PARAM::NG_TRAY_IN_LIFT_Z_TO_DOWN );
}

void CUnitCtrlBank::InNGTrayLiftZ_MidPos()
{
	theDeviceMotion.TeachMove( m_nThreadID, TEACH_PARAM::NG_TRAY_IN_LIFT_Z_TO_MID );
}

BOOL CUnitCtrlBank::InNGTrayLiftZ_MidPosChk()
{
	return theDeviceMotion.CheckTeachPosition( m_nThreadID, TEACH_PARAM::NG_TRAY_IN_LIFT_Z_TO_MID );
}

BOOL CUnitCtrlBank::InNGTrayLiftZ_MidPosLowChk()
{
	return theDeviceMotion.CheckTeachPosition( m_nThreadID, TEACH_PARAM::NG_TRAY_IN_LIFT_Z_TO_MID ,0.05,POS_LOWER,0.0);
}

void CUnitCtrlBank::InNGTrayLiftZ_PitchUp( double dPitch )
{
	// Up 위치에서 Tray 갯수만큼 내려온다
//	double dOffset = theRecipeBank.m_Tray.m_dTrayThickness * (double)nPitch;
//	theDeviceMotion.TeachMove( m_nThreadID, TEACH_PARAM::NG_TRAY_IN_LIFT_Z_TO_UP, -dOffset );
#if (AMT_TYPE == 2)
	dPitch = dPitch/2;
#endif
	AxisMoveParam param;
	param.dAcc = ACC_300ms;
	param.dDec = ACC_300ms;
	param.dSpeed = theAxisBank.m_Axis[AXIS_SVO_NG_TRAY_IN_LIFT_Z].m_dDefaultMoveSpeed;
	param.idAxis = AXIS_SVO_NG_TRAY_IN_LIFT_Z;
	param.dTargetPosition = -(dPitch * theRecipeBank.m_Tray.m_dTrayThickness);
	theDeviceMotion.AxisMoveInc(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_NG_TRAY_IN_LIFT_Z, param);
}

void CUnitCtrlBank::InNGTrayLiftZ_PitchDown( double dPitch )
{
	// Up 위치에서 Tray 갯수만큼 내려온다
//	double dOffset = theRecipeBank.m_Tray.m_dTrayThickness * (double)nPitch;
//	theDeviceMotion.TeachMove( m_nThreadID, TEACH_PARAM::NG_TRAY_IN_LIFT_Z_TO_UP, dOffset );
#if (AMT_TYPE == 2)
	dPitch = dPitch/2;
#endif
	AxisMoveParam param;
	param.dAcc = ACC_300ms;
	param.dDec = ACC_300ms;
	param.dSpeed = theAxisBank.m_Axis[AXIS_SVO_NG_TRAY_IN_LIFT_Z].m_dDefaultMoveSpeed;
	param.idAxis = AXIS_SVO_NG_TRAY_IN_LIFT_Z;
	param.dTargetPosition = (dPitch * theRecipeBank.m_Tray.m_dTrayThickness);
	theDeviceMotion.AxisMoveInc(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_NG_TRAY_IN_LIFT_Z, param);
}

void CUnitCtrlBank::InNGTrayLiftZ_PitchDownRetry()
{
	AxisMoveParam param;
	param.dAcc = ACC_300ms;
	param.dDec = ACC_300ms;
	param.dSpeed = theAxisBank.m_Axis[AXIS_SVO_NG_TRAY_IN_LIFT_Z].m_dJogSpeedLow;
	param.idAxis = AXIS_SVO_NG_TRAY_IN_LIFT_Z;
	theDeviceMotion.AxisMoveJog(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_NG_TRAY_IN_LIFT_Z, TRUE, param);
}

void CUnitCtrlBank::InNGTrayLiftZ_PitchUpRetry()
{
	AxisMoveParam param;
	param.dAcc = ACC_300ms;
	param.dDec = ACC_300ms;
	param.dSpeed = theAxisBank.m_Axis[AXIS_SVO_NG_TRAY_IN_LIFT_Z].m_dJogSpeedLow;
	param.idAxis = AXIS_SVO_NG_TRAY_IN_LIFT_Z;
	theDeviceMotion.AxisMoveJog(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_NG_TRAY_IN_LIFT_Z, FALSE, param);
}

void CUnitCtrlBank::InNGTrayLiftZ_PitchUpStop()
{
	theDeviceMotion.AxisStop(MP2100_THREAD_MAIN_PROCESS,AXIS_SVO_NG_TRAY_IN_LIFT_Z);
}

BOOL CUnitCtrlBank::InNGTrayLiftZ_StopChk()
{
	return theDeviceMotion.IsAxisReady( m_nThreadID, AXIS_SVO_NG_TRAY_IN_LIFT_Z, TRUE);
}

void CUnitCtrlBank::OutNGTrayLiftZ_UpPos()
{
	theDeviceMotion.TeachMove( m_nThreadID, TEACH_PARAM::NG_TRAY_OUT_LIFT_Z_TO_UP );
}

BOOL CUnitCtrlBank::OutNGTrayLiftZ_UpChk()
{
	return theDeviceMotion.CheckTeachPosition( m_nThreadID, TEACH_PARAM::NG_TRAY_OUT_LIFT_Z_TO_UP );
}

void CUnitCtrlBank::OutNGTrayLiftZ_PitchUp( double dPitch )
{
	// Up 위치에서 Tray 갯수만큼 내려온다
//	double dOffset = theRecipeBank.m_Tray.m_dTrayThickness * (double)nPitch;
//	theDeviceMotion.TeachMove( m_nThreadID, TEACH_PARAM::NG_TRAY_OUT_LIFT_Z_TO_UP, -dOffset );
#if(AMT_TYPE == 2)
	dPitch = dPitch/2;
#endif

	AxisMoveParam param;
	param.dAcc = ACC_300ms;
	param.dDec = ACC_300ms;
	param.dSpeed = theAxisBank.m_Axis[AXIS_SVO_NG_TRAY_OUT_LIFT_Z].m_dDefaultMoveSpeed;
	param.idAxis = AXIS_SVO_NG_TRAY_OUT_LIFT_Z;
	param.dTargetPosition = -(dPitch * theRecipeBank.m_Tray.m_dTrayThickness);
	theDeviceMotion.AxisMoveInc(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_NG_TRAY_OUT_LIFT_Z, param);
}

void CUnitCtrlBank::OutNGTrayLiftZ_PitchDown( double dPitch )
{
	// Up 위치에서 Tray 갯수만큼 내려온다
//	double dOffset = theRecipeBank.m_Tray.m_dTrayThickness * (double)nPitch;
//	theDeviceMotion.TeachMove( m_nThreadID, TEACH_PARAM::NG_TRAY_OUT_LIFT_Z_TO_UP, dOffset );
#if(AMT_TYPE == 2)
	dPitch = dPitch/2;
#endif

	AxisMoveParam param;
	param.dAcc = ACC_300ms;
	param.dDec = ACC_300ms;
	param.dSpeed = theAxisBank.m_Axis[AXIS_SVO_NG_TRAY_OUT_LIFT_Z].m_dDefaultMoveSpeed;
	param.idAxis = AXIS_SVO_NG_TRAY_OUT_LIFT_Z;
	param.dTargetPosition = (dPitch * theRecipeBank.m_Tray.m_dTrayThickness);
	theDeviceMotion.AxisMoveInc(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_NG_TRAY_OUT_LIFT_Z, param);
}


void CUnitCtrlBank::OutNGTrayLiftZ_DownPos()
{
	theDeviceMotion.TeachMove( m_nThreadID, TEACH_PARAM::NG_TRAY_OUT_LIFT_Z_TO_DOWN );
}

BOOL CUnitCtrlBank::OutNGTrayLiftZ_DownChk()
{
	return theDeviceMotion.CheckTeachPosition( m_nThreadID, TEACH_PARAM::NG_TRAY_OUT_LIFT_Z_TO_DOWN);
}

BOOL CUnitCtrlBank::OutNGTrayLiftZ_StopChk()
{
	return theDeviceMotion.IsAxisReady( m_nThreadID, AXIS_SVO_NG_TRAY_OUT_LIFT_Z, TRUE);
}

BOOL CUnitCtrlBank::NGConv_TrayChk( TRAY_PORT Port, E_CONV_SEN_INDEX index, SENSOR_STATE sen )
{
	if(theConfigBank.m_Option.m_bUseDryRun)
		return TRUE;
	if (Port == NG_TRAY_PORT)
	{
		ONOFF val = (sen== SENSOR_ON) ? ON:OFF;
		INPUT_ID iId;

		if (index == CONV_SEN_1)
			iId = X_NG_CONV_TRAY_SEN_1;
		else if (index == CONV_SEN_2)
			iId = X_NG_CONV_TRAY_SEN_2;
		else if (index == CONV_SEN_3)
			iId = X_NG_CONV_TRAY_SEN_3;
		else if (index == CONV_SEN_4)
			iId = X_NG_CONV_TRAY_SEN_4;
		else if (index == CONV_SEN_5)
			iId = X_NG_CONV_TRAY_SEN_5;

		if(GetInPutIOCheck(iId, val))
			return TRUE;
		else
			return FALSE;
	}
	if (Port == NG_BUFFER_TRAY_PORT)
	{
		ONOFF val = (sen== SENSOR_ON) ? ON:OFF;
		INPUT_ID iId;

		if (index == CONV_SEN_1)
			iId = X_NG_BUFFER_CONV_TRAY_SEN_1;
		else if (index == CONV_SEN_2)
			iId = X_NG_BUFFER_CONV_TRAY_SEN_2;
		else if (index == CONV_SEN_3)
			iId = X_NG_BUFFER_CONV_TRAY_SEN_3;
		else if (index == CONV_SEN_4)
			iId = X_NG_BUFFER_CONV_TRAY_SEN_4;
		else if (index == CONV_SEN_5)
			iId = X_NG_BUFFER_CONV_TRAY_SEN_5;

		if(GetInPutIOCheck(iId, val))
			return TRUE;
		else
			return FALSE;
	}

	return TRUE;
}


void CUnitCtrlBank::NGLiftArmFwdBwd(TRAY_PORT Port,CYL_STATE Cyl)
{
	if (Port == NG_TRAY_PORT)
	{
		SetOutPutIO(Y_NG_TRAY_ALIGN_ARM_IN_SOL,Cyl);
		SetOutPutIO(Y_NG_TRAY_ALIGN_ARM_OUT_SOL,!Cyl);
	}
	else
	{
		SetOutPutIO(Y_NG_BUFFER_TRAY_ALIGN_ARM_IN_SOL,Cyl);
		SetOutPutIO(Y_NG_BUFFER_TRAY_ALIGN_ARM_OUT_SOL,!Cyl);
	}
}

BOOL CUnitCtrlBank::NGLiftArmFwdBwdChk(TRAY_PORT Port,CYL_STATE Cyl)
{
	if (Port == NG_TRAY_PORT)
	{
		if (Cyl == CYL_FWD)
			return (GetInPutIOCheck(X_NG_TRAY_ARM_FORWARD_SEN_1) && GetInPutIOCheck(X_NG_TRAY_ARM_FORWARD_SEN_2));
		else
			return (GetInPutIOCheck(X_NG_TRAY_ARM_BACKWARD_SEN_1) && GetInPutIOCheck(X_NG_TRAY_ARM_BACKWARD_SEN_2));
	}
	else
	{
		if (Cyl == CYL_FWD)
			return (GetInPutIOCheck(X_NG_BUFFER_TRAY_ARM_FORWARD_SEN_1) && GetInPutIOCheck(X_NG_BUFFER_TRAY_ARM_FORWARD_SEN_2));
		else
			return (GetInPutIOCheck(X_NG_BUFFER_TRAY_ARM_BACKWARD_SEN_1) && GetInPutIOCheck(X_NG_BUFFER_TRAY_ARM_BACKWARD_SEN_2));
	}
}

void CUnitCtrlBank::NGTrayUpDown(TRAY_PORT Port,CYL_STATE Cyl)
{
	if (Port == NG_TRAY_PORT)
	{
		SetOutPutIO(Y_NG_TRAY_TRAY_UP_SOL,Cyl);
		SetOutPutIO(Y_NG_TRAY_TRAY_DOWN_SOL,!Cyl);
	}
	else
	{
		SetOutPutIO(Y_NG_BUFFER_TRAY_TRAY_UP_SOL,Cyl);
		SetOutPutIO(Y_NG_BUFFER_TRAY_TRAY_DOWN_SOL,!Cyl);
	}
}


BOOL CUnitCtrlBank::NGTrayUpDownChk(TRAY_PORT Port,CYL_STATE Cyl)
{
	if (Port == NG_TRAY_PORT)
	{
		if (Cyl == CYL_UP)
			return GetInPutIOCheck(X_NG_TRAY_TRAY_UP_SEN);
		else
			return GetInPutIOCheck(X_NG_TRAY_TRAY_DOWN_SEN);
	}
	else
	{
		if (Cyl == CYL_UP)
			return GetInPutIOCheck(X_NG_BUFFER_TRAY_TRAY_UP_SEN);
		else
			return GetInPutIOCheck(X_NG_BUFFER_TRAY_TRAY_DOWN_SEN);
	}
}

BOOL CUnitCtrlBank::NGTrayInAlignTrayChk( SENSOR_STATE sen )
{
	if(theConfigBank.m_Option.m_bUseDryRun)
		return TRUE;

	ONOFF val = (sen== SENSOR_ON) ? ON:OFF;
#if(AMT_TYPE == 1)
	INPUT_ID iId = X_NG_TRAY_ORG_SENSOR;
#else
	INPUT_ID iId = X_NG_TRAY_LIFT_LIMIT_SEN_2;
#endif

	if(GetInPutIOCheck(iId, val))
		return TRUE;
	else
		return FALSE;
}

BOOL CUnitCtrlBank::NGInTrayChk( SENSOR_STATE sen )
{
	if(theConfigBank.m_Option.m_bUseDryRun)
		return TRUE;

	ONOFF val = (sen== SENSOR_ON) ? ON:OFF;

	INPUT_ID iId = X_NG_TRAY_LIFT_LIMIT_SEN_3;

	if(GetInPutIOCheck(iId, val))
		return TRUE;
	else
		return FALSE;
}

BOOL CUnitCtrlBank::NGTrayLiftTrayChk( SENSOR_STATE sen )
{
	if(theConfigBank.m_Option.m_bUseDryRun)
		return TRUE;

	ONOFF val = (sen== SENSOR_ON) ? ON:OFF;
	// 차후 Lift Arm Sensor IO 추가되면 교체.

	INPUT_ID iId = X_NG_TRAY_LIFT_ARM_TRAY_SEN_1;
	INPUT_ID iId2 = X_NG_TRAY_LIFT_ARM_TRAY_SEN_2;

	//2017-08-14 KAKAKA
	if ((theConfigBank.m_Option.m_NGINLIFTSEN1 == TRUE) && (theConfigBank.m_Option.m_NGINLIFTSEN2 == TRUE))
	{
		if(GetInPutIOCheck(iId, val) && GetInPutIOCheck(iId2, val))
			return TRUE;
		else
			return FALSE;
	}
	else if ((theConfigBank.m_Option.m_NGINLIFTSEN1 == TRUE) && (theConfigBank.m_Option.m_NGINLIFTSEN2 == FALSE))
	{
		if(GetInPutIOCheck(iId, val))
			return TRUE;
		else
			return FALSE;
	}
	else if ((theConfigBank.m_Option.m_NGINLIFTSEN1 == FALSE) && (theConfigBank.m_Option.m_NGINLIFTSEN2 == TRUE))
	{
		if(GetInPutIOCheck(iId2, val))
			return TRUE;
		else
			return FALSE;
	}

	return FALSE;
}

BOOL CUnitCtrlBank::NGTrayOutLiftTrayChk( SENSOR_STATE sen )
{
	ONOFF val = (sen== SENSOR_ON) ? ON:OFF;
	// 차후 Lift Arm Sensor IO 추가되면 교체.
	INPUT_ID iId = X_NG_BUFFER_TRAY_LIFT_ARM_TRAY_SEN_1;
	INPUT_ID iId2 = X_NG_BUFFER_TRAY_LIFT_ARM_TRAY_SEN_2;

	//2017-08-14 KAKAKA
	if ((theConfigBank.m_Option.m_NGOUTLIFTSEN1 == TRUE) && (theConfigBank.m_Option.m_NGOUTLIFTSEN2 == TRUE))
	{
		if(GetInPutIOCheck(iId, val) && GetInPutIOCheck(iId2, val))
			return TRUE;
		else
			return FALSE;
	}
	else if ((theConfigBank.m_Option.m_NGOUTLIFTSEN1 == TRUE) && (theConfigBank.m_Option.m_NGOUTLIFTSEN2 == FALSE))
	{
		if(GetInPutIOCheck(iId, val))
			return TRUE;
		else
			return FALSE;
	}
	else if ((theConfigBank.m_Option.m_NGOUTLIFTSEN1 == FALSE) && (theConfigBank.m_Option.m_NGOUTLIFTSEN2 == TRUE))
	{
		if(GetInPutIOCheck(iId2, val))
			return TRUE;
		else
			return FALSE;
	}

	return FALSE;
}