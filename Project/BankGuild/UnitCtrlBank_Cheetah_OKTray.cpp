#include "StdAfx.h"
#include "UnitCtrlBank.h"
#include "CheetahApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CUnitCtrlBank::TrayTrY_InPos()
{
	theDeviceMotion.TeachMove( m_nThreadID, TEACH_PARAM::OK_TRAY_TR_Y_TO_IN);
}

void CUnitCtrlBank::TrayTrY_OutPos()
{
	theDeviceMotion.TeachMove( m_nThreadID, TEACH_PARAM::OK_TRAY_TR_Y_TO_OUT);
}

BOOL CUnitCtrlBank::TrayTrY_InChk()
{
	return theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::OK_TRAY_TR_Y_TO_IN);
}

BOOL CUnitCtrlBank::TrayTrY_OutChk()
{
	return theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::OK_TRAY_TR_Y_TO_OUT);
}

void CUnitCtrlBank::TrayTrZ_UpPos(double dSpeed)
{
	theDeviceMotion.TeachMove( m_nThreadID, TEACH_PARAM::OK_TRAY_TR_Z_TO_UP, dSpeed);
}
//2017-09-21,SKCHO, UP에서 버큠 알람 발생시 OFFSET값을 주고 RETRY한다.
void CUnitCtrlBank::TrayTrZ_InPos(double dOffset)
{
	theDeviceMotion.TeachMove( m_nThreadID, TEACH_PARAM::OK_TRAY_TR_Z_TO_IN,dOffset);
}

void CUnitCtrlBank::TrayTrZ_OutPos()
{
	theDeviceMotion.TeachMove( m_nThreadID, TEACH_PARAM::OK_TRAY_TR_Z_TO_OUT);
}

BOOL CUnitCtrlBank::TrayTrZ_UpChk()
{
	return theDeviceMotion.CheckTeachPosition( m_nThreadID, TEACH_PARAM::OK_TRAY_TR_Z_TO_UP);
}

//2017-09-21,SKCHO, GOOD TRAY RETRY 기능 추가로 함수 수정
BOOL CUnitCtrlBank::TrayTrZ_InChk(double dOffset)
{
	return theDeviceMotion.CheckTeachPosition( m_nThreadID, TEACH_PARAM::OK_TRAY_TR_Z_TO_IN,0.1, POS_EQUAL,dOffset);
}

BOOL CUnitCtrlBank::TrayTrZ_OutChk()
{
	return theDeviceMotion.CheckTeachPosition( m_nThreadID, TEACH_PARAM::OK_TRAY_TR_Z_TO_OUT);
}

void CUnitCtrlBank::TrayTr_VacOnOff( VAC_STATE vac )
{
	int nNum[4];
	ONOFF valVac = (vac== VAC_ON) ? ON:OFF;
#if(AMT_TYPE == 1)
	nNum[0] = Y_GOOD_TRAY_TR_VAC_ON_SOL_1;		SetOutPutIO(nNum[0], valVac);
	nNum[1] = Y_GOOD_TRAY_TR_BLOW_ON_SOL_1;		SetOutPutIO(nNum[1], !valVac);
	nNum[2] = Y_GOOD_TRAY_TR_VAC_ON_SOL_2;		SetOutPutIO(nNum[2], valVac);
	nNum[3] = Y_GOOD_TRAY_TR_BLOW_ON_SOL_2;		SetOutPutIO(nNum[3], !valVac);
#else
	nNum[0] = Y_GOOD_TRAY_TR_VAC_ON_SOL_1;		SetOutPutIO(nNum[0], valVac);
	nNum[1] = Y_GOOD_TRAY_TR_BLOW_ON_SOL_1;		SetOutPutIO(nNum[1], !valVac);
#endif
}

void CUnitCtrlBank::TrayTr_BlowOff()
{
#if(AMT_TYPE == 1)
	SetOutPutIO(Y_GOOD_TRAY_TR_BLOW_ON_SOL_1, FALSE);
	SetOutPutIO(Y_GOOD_TRAY_TR_BLOW_ON_SOL_2, FALSE);
#else
	SetOutPutIO(Y_GOOD_TRAY_TR_BLOW_ON_SOL_1, FALSE);
#endif

}

BOOL CUnitCtrlBank::TrayTr_VacChk( VAC_STATE vac )
{
	if(theConfigBank.m_Option.m_bUseDryRun)
		return TRUE;

	INPUT_ID nNum1;
	ONOFF val = (vac== VAC_ON) ? ON:OFF;
	nNum1 = X_GOOD_TRAY_TR_VAC_SEN_1;
	return GetInPutIOCheck(nNum1, val);
}


void CUnitCtrlBank::InTrayLiftZ_UpPos()
{
	theDeviceMotion.TeachMove( m_nThreadID, TEACH_PARAM::OK_TRAY_IN_LIFT_Z_TO_UP );
}

BOOL CUnitCtrlBank::InTrayLiftZ_UpChk()
{
	return theDeviceMotion.CheckTeachPosition( m_nThreadID, TEACH_PARAM::OK_TRAY_IN_LIFT_Z_TO_UP );
}

void CUnitCtrlBank::InTrayLiftZ_DownPos()
{
	theDeviceMotion.TeachMove( m_nThreadID, TEACH_PARAM::OK_TRAY_IN_LIFT_Z_TO_DOWN );
}

BOOL CUnitCtrlBank::InTrayLiftZ_DownChk()
{
	return theDeviceMotion.CheckTeachPosition( m_nThreadID, TEACH_PARAM::OK_TRAY_IN_LIFT_Z_TO_DOWN );
}

void CUnitCtrlBank::InTrayLiftZ_AlignPos()
{
	theDeviceMotion.TeachMove( m_nThreadID, TEACH_PARAM::OK_TRAY_IN_LIFT_Z_TO_ALIGN );
}

BOOL CUnitCtrlBank::InTrayLiftZ_AlignPosChk()
{
	return theDeviceMotion.CheckTeachPosition( m_nThreadID, TEACH_PARAM::OK_TRAY_IN_LIFT_Z_TO_ALIGN );
}

void CUnitCtrlBank::InTrayLiftZ_TopPos()
{
	theDeviceMotion.TeachMove( m_nThreadID, TEACH_PARAM::OK_TRAY_IN_LIFT_Z_TO_TOP );
}

BOOL CUnitCtrlBank::InTrayLiftZ_TopPosChk()
{
	return theDeviceMotion.CheckTeachPosition( m_nThreadID, TEACH_PARAM::OK_TRAY_IN_LIFT_Z_TO_TOP );
}


void CUnitCtrlBank::InTrayLiftZ_PitchUp(double dPitch)
{
	// Down 위치에서 Tray 갯수만큼 올라온다
//	double dOffset = theRecipeBank.m_Tray.m_dTrayThickness * nPitch * -1.0;
//	theDeviceMotion.TeachMove( m_nThreadID, TEACH_PARAM::OK_TRAY_IN_LIFT_Z_TO_UP, dOffset );
#if(AMT_TYPE == 2)
	dPitch = dPitch/2;
#endif

	AxisMoveParam param;
	param.dAcc = ACC_300ms;
	param.dDec = ACC_300ms;
	param.dSpeed = theAxisBank.m_Axis[AXIS_SVO_GOOD_TRAY_IN_LIFT_Z].m_dDefaultMoveSpeed;
	param.idAxis = AXIS_SVO_GOOD_TRAY_IN_LIFT_Z;
	//2017-03-31,skcho, 1,2호기 와양산호기 구분하기 위해서, 리프트 리미트 센서가 위쪽에 있는상태
	#if(LIMIT_TYPE == 2)
		param.dTargetPosition = (dPitch * theRecipeBank.m_Tray.m_dTrayThickness);
	#else
	    param.dTargetPosition = -(dPitch * theRecipeBank.m_Tray.m_dTrayThickness);
	#endif
	theDeviceMotion.AxisMoveInc(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_GOOD_TRAY_IN_LIFT_Z, param);
}



////20180816wjp
void CUnitCtrlBank::OutTrayLiftZ_PitchUpRetry()
{
	AxisMoveParam param;
	param.dAcc = ACC_300ms;
	param.dDec = ACC_300ms;
	param.dSpeed = theAxisBank.m_Axis[AXIS_SVO_GOOD_TRAY_IN_LIFT_Z].m_dJogSpeedLow;
	param.idAxis = AXIS_SVO_GOOD_TRAY_OUT_LIFT_Z;
	#if(LIMIT_TYPE == 2)
		theDeviceMotion.AxisMoveJog(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_GOOD_TRAY_OUT_LIFT_Z, TRUE, param);
	#else
	    theDeviceMotion.AxisMoveJog(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_GOOD_TRAY_OUT_LIFT_Z, FALSE, param);
	#endif
}
//20180816wjp 
void CUnitCtrlBank::OutTrayLiftZ_PitchUpStop()
{
	theDeviceMotion.AxisStop(MP2100_THREAD_MAIN_PROCESS,AXIS_SVO_GOOD_TRAY_OUT_LIFT_Z);
}


void CUnitCtrlBank::InTrayLiftZ_PitchUpRetry()
{
	AxisMoveParam param;
	param.dAcc = ACC_300ms;
	param.dDec = ACC_300ms;
	param.dSpeed = theAxisBank.m_Axis[AXIS_SVO_GOOD_TRAY_IN_LIFT_Z].m_dJogSpeedLow;
	param.idAxis = AXIS_SVO_GOOD_TRAY_IN_LIFT_Z;
	#if(LIMIT_TYPE == 2)
		theDeviceMotion.AxisMoveJog(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_GOOD_TRAY_IN_LIFT_Z, TRUE, param);
	#else
	    theDeviceMotion.AxisMoveJog(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_GOOD_TRAY_IN_LIFT_Z, FALSE, param);
	#endif
}
void CUnitCtrlBank::InTrayLiftZ_PitchUpStop()
{
	theDeviceMotion.AxisStop(MP2100_THREAD_MAIN_PROCESS,AXIS_SVO_GOOD_TRAY_IN_LIFT_Z);
}
void CUnitCtrlBank::InTrayLiftZ_PitchDown(double dPitch)
{
	// Up 위치에서 Tray 갯수만큼 내려온다
//	double dOffset = theRecipeBank.m_Tray.m_dTrayThickness * nPitch;
//	theDeviceMotion.TeachMove( m_nThreadID, TEACH_PARAM::OK_TRAY_IN_LIFT_Z_TO_UP, dOffset );
#if(AMT_TYPE == 2)
	dPitch = dPitch/2;
#endif

	AxisMoveParam param;
	param.dAcc = ACC_300ms;
	param.dDec = ACC_300ms;
	param.dSpeed = theAxisBank.m_Axis[AXIS_SVO_GOOD_TRAY_IN_LIFT_Z].m_dDefaultMoveSpeed;
	param.idAxis = AXIS_SVO_GOOD_TRAY_IN_LIFT_Z;
	//2017-03-31,skcho, 1,2호기 와양산호기 구분하기 위해서, 리프트 리미트 센서가 위쪽에 있는상태
	#if(LIMIT_TYPE == 2)
		param.dTargetPosition = -(dPitch * theRecipeBank.m_Tray.m_dTrayThickness);
	#else
		param.dTargetPosition = (dPitch * theRecipeBank.m_Tray.m_dTrayThickness);
	#endif
	theDeviceMotion.AxisMoveInc(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_GOOD_TRAY_IN_LIFT_Z, param);
}

void CUnitCtrlBank::InTrayLiftZ_PitchDownRetry()
{
	AxisMoveParam param;
	param.dAcc = ACC_300ms;
	param.dDec = ACC_300ms;
	param.dSpeed = theAxisBank.m_Axis[AXIS_SVO_GOOD_TRAY_IN_LIFT_Z].m_dJogSpeedLow;
	param.idAxis = AXIS_SVO_GOOD_TRAY_IN_LIFT_Z;
	//2017-03-31,skcho, 1,2호기 와양산호기 구분하기 위해서, 리프트 리미트 센서가 위쪽에 있는상태
	#if(LIMIT_TYPE == 2)
		theDeviceMotion.AxisMoveJog(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_GOOD_TRAY_IN_LIFT_Z, TRUE, param);
	#else
		theDeviceMotion.AxisMoveJog(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_GOOD_TRAY_IN_LIFT_Z, FALSE, param);
	#endif
}

BOOL CUnitCtrlBank::InTrayLiftZ_StopChk()
{
	return theDeviceMotion.IsAxisReady( m_nThreadID, AXIS_SVO_GOOD_TRAY_IN_LIFT_Z, TRUE);
}

void CUnitCtrlBank::OutTrayLiftZ_UpPos()
{
	theDeviceMotion.TeachMove( m_nThreadID, TEACH_PARAM::OK_TRAY_OUT_LIFT_Z_TO_UP );
}

BOOL CUnitCtrlBank::OutTrayLiftZ_UpChk()
{
	return theDeviceMotion.CheckTeachPosition( m_nThreadID, TEACH_PARAM::OK_TRAY_OUT_LIFT_Z_TO_UP );
}




void CUnitCtrlBank::OutTrayLiftZ_PitchUp( double dPitch )
{
	// Down 위치에서 Tray 갯수만큼 올라온다
//	double dOffset = theRecipeBank.m_Tray.m_dTrayThickness * nPitch;
//	theDeviceMotion.TeachMove( m_nThreadID, TEACH_PARAM::OK_TRAY_OUT_LIFT_Z_TO_UP, -dOffset );
#if(AMT_TYPE == 2)
	dPitch = dPitch/2;
#endif

	AxisMoveParam param;
	param.dAcc = ACC_300ms;
	param.dDec = ACC_300ms;
	param.dSpeed = theAxisBank.m_Axis[AXIS_SVO_GOOD_TRAY_OUT_LIFT_Z].m_dDefaultMoveSpeed;
	param.idAxis = AXIS_SVO_GOOD_TRAY_OUT_LIFT_Z;
	//2017-03-31,skcho, 1,2호기 와양산호기 구분하기 위해서, 리프트 리미트 센서가 위쪽에 있는상태
	#if(LIMIT_TYPE == 2)
		param.dTargetPosition = (dPitch * theRecipeBank.m_Tray.m_dTrayThickness);
    #else
		param.dTargetPosition = -(dPitch * theRecipeBank.m_Tray.m_dTrayThickness);
    #endif
	theDeviceMotion.AxisMoveInc(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_GOOD_TRAY_OUT_LIFT_Z, param);
}

void CUnitCtrlBank::OutTrayLiftZ_PitchDown( double dPitch )
{
	// Up 위치에서 Tray 갯수만큼 내려온다
//	double dOffset = theRecipeBank.m_Tray.m_dTrayThickness * nPitch;
//	theDeviceMotion.TeachMove( m_nThreadID, TEACH_PARAM::OK_TRAY_OUT_LIFT_Z_TO_UP, dOffset );
#if(AMT_TYPE == 2)
	dPitch = dPitch/2;
#endif

	AxisMoveParam param;
	param.dAcc = ACC_300ms;
	param.dDec = ACC_300ms;
	param.dSpeed = theAxisBank.m_Axis[AXIS_SVO_GOOD_TRAY_OUT_LIFT_Z].m_dDefaultMoveSpeed;
	param.idAxis = AXIS_SVO_GOOD_TRAY_OUT_LIFT_Z;

	//2017-03-31,skcho, 1,2호기 와양산호기 구분하기 위해서, 리프트 리미트 센서가 위쪽에 있는상태
	#if(LIMIT_TYPE == 2)
		param.dTargetPosition = -(dPitch * theRecipeBank.m_Tray.m_dTrayThickness);
	#else 
		param.dTargetPosition = (dPitch * theRecipeBank.m_Tray.m_dTrayThickness);
	#endif
	theDeviceMotion.AxisMoveInc(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_GOOD_TRAY_OUT_LIFT_Z, param);
}

void CUnitCtrlBank::OutTrayLiftZ_DownPos()
{
	theDeviceMotion.TeachMove( m_nThreadID, TEACH_PARAM::OK_TRAY_OUT_LIFT_Z_TO_DOWN );
}

BOOL CUnitCtrlBank::OutTrayLiftZ_DownChk()
{
	return theDeviceMotion.CheckTeachPosition( m_nThreadID, TEACH_PARAM::OK_TRAY_OUT_LIFT_Z_TO_DOWN);
}

BOOL CUnitCtrlBank::OutTrayLiftZ_StopChk()
{
	return theDeviceMotion.IsAxisReady( m_nThreadID, AXIS_SVO_GOOD_TRAY_OUT_LIFT_Z, TRUE);
}
//2017-12-22,SKCHO,추가
void CUnitCtrlBank::OutTrayLiftZ_AlignPos()
{
	theDeviceMotion.TeachMove( m_nThreadID, TEACH_PARAM::OK_TRAY_OUT_LIFT_Z_TO_ALIGN );
}

BOOL CUnitCtrlBank::OutTrayLiftZ_AlignChk()
{
	return theDeviceMotion.CheckTeachPosition( m_nThreadID, TEACH_PARAM::OK_TRAY_OUT_LIFT_Z_TO_ALIGN );
}

BOOL CUnitCtrlBank::TrayLiftOVerChk(TRAY_PORT Port)
{
	if (Port == GOOD_IN_TRAY_PORT)
		return GetInPutIOCheck(X_GOOD_LD_TRAY_LIFT_LIMIT_SEN_1);
	else
		return GetInPutIOCheck(X_NG_TRAY_LIFT_LIMIT_SEN_1);
}

BOOL CUnitCtrlBank::InTrayAlignTrayChk( SENSOR_STATE sen )
{
	if(theConfigBank.m_Option.m_bUseDryRun)
		return TRUE;

	ONOFF val = (sen== SENSOR_ON) ? ON:OFF;
#if(AMT_TYPE == 1)
	INPUT_ID iId = X_GOOD_LD_TRAY_ORG_SENSOR;
#else
	INPUT_ID iId = X_GOOD_LD_TRAY_LIFT_LIMIT_SEN_2;
#endif
	if(GetInPutIOCheck(iId, val))
		return TRUE;
	else
		return FALSE;
}

BOOL CUnitCtrlBank::InTrayChk( SENSOR_STATE sen )
{
	if(theConfigBank.m_Option.m_bUseDryRun)
		return TRUE;
	ONOFF val = (sen== SENSOR_ON) ? ON:OFF;

	INPUT_ID iId = X_GOOD_LD_TRAY_LIFT_LIMIT_SEN_3;

	if(GetInPutIOCheck(iId, val))
		return TRUE;
	else
		return FALSE;
}


BOOL CUnitCtrlBank::LDTrayLiftTrayChk( SENSOR_STATE sen )
{
	if(theConfigBank.m_Option.m_bUseDryRun)
		return TRUE;

	ONOFF val = (sen== SENSOR_ON) ? ON:OFF;
	INPUT_ID iId = X_GOOD_LD_TRAY_LIFT_SEN;

	if(GetInPutIOCheck(iId, val))
		return TRUE;
	else
		return FALSE;
}


BOOL CUnitCtrlBank::ULDTrayLiftTrayChk( SENSOR_STATE sen )
{
	ONOFF val = (sen== SENSOR_ON) ? ON:OFF;
	INPUT_ID iId = X_GOOD_ULD_TRAY_LIFT_SEN;

	if(GetInPutIOCheck(iId, val))
		return TRUE;
	else
		return FALSE;
}

BOOL CUnitCtrlBank::InConv_TrayChk( E_CONV_SEN_INDEX index, SENSOR_STATE sen )
{
	if(theConfigBank.m_Option.m_bUseDryRun)
		return TRUE;
	ONOFF val = (sen== SENSOR_ON) ? ON:OFF;
	INPUT_ID iId;

#if(AMT_TYPE == 1)
	if (index == CONV_SEN_1)
		iId = X_GOOD_LD_CONV_TRAY_SEN_1;
	else if (index == CONV_SEN_2)
		iId = X_GOOD_LD_CONV_TRAY_SEN_2;
	else if (index == CONV_SEN_3)
		iId = X_GOOD_LD_CONV_TRAY_SEN_3;
	else if (index == CONV_SEN_4)
		iId = X_GOOD_LD_CONV_TRAY_SEN_4;
	else if (index == CONV_SEN_5)
		iId = X_GOOD_LD_CONV_TRAY_SEN_5;
#else
	if (index == CONV_SEN_1)
		iId = X_GOOD_LD_CONV_TRAY_SEN_1;
	else if (index == CONV_SEN_2)
		return TRUE;
	else if (index == CONV_SEN_3)
		return TRUE;
	else if (index == CONV_SEN_4)
		iId = X_GOOD_LD_CONV_TRAY_SEN_4;
	else if (index == CONV_SEN_5)
		iId = X_GOOD_LD_CONV_TRAY_SEN_5;
#endif
	if(GetInPutIOCheck(iId, val))
		return TRUE;
	else
		return FALSE;
}

BOOL CUnitCtrlBank::BuffConv_TrayChk( E_CONV_SEN_INDEX index, SENSOR_STATE sen )
{
	if(theConfigBank.m_Option.m_bUseDryRun)
		return TRUE;
	ONOFF val = (sen== SENSOR_ON) ? ON:OFF;
	INPUT_ID iId;

#if(AMT_TYPE == 1)
	if (index == CONV_SEN_1)
		iId = X_GOOD_LD_BUFFER_CONV_TRAY_SEN_1;
	else if (index == CONV_SEN_2)
		iId = X_GOOD_LD_BUFFER_CONV_TRAY_SEN_2;
	else if (index == CONV_SEN_3)
		iId = X_GOOD_LD_BUFFER_CONV_TRAY_SEN_3;
	else if (index == CONV_SEN_4)
		iId = X_GOOD_LD_BUFFER_CONV_TRAY_SEN_4;
	else if (index == CONV_SEN_5)
		iId = X_GOOD_LD_BUFFER_CONV_TRAY_SEN_5;
#else
	if (index == CONV_SEN_1)
		iId = X_GOOD_LD_BUFFER_CONV_TRAY_SEN_1;
	else if (index == CONV_SEN_2)
		return TRUE;
	else if (index == CONV_SEN_3)
		return TRUE;
	else if (index == CONV_SEN_4)
		iId = X_GOOD_LD_BUFFER_CONV_TRAY_SEN_4;
	else if (index == CONV_SEN_5)
		iId = X_GOOD_LD_BUFFER_CONV_TRAY_SEN_5;
#endif
	if(GetInPutIOCheck(iId, val))
		return TRUE;
	else
		return FALSE;
}

BOOL CUnitCtrlBank::OutConv_TrayChk( E_CONV_SEN_INDEX index, SENSOR_STATE sen )
{
	if(theConfigBank.m_Option.m_bUseDryRun)
		return TRUE;
	ONOFF val = (sen== SENSOR_ON) ? ON:OFF;
	INPUT_ID iId;
#if(AMT_TYPE == 1)
	if (index == CONV_SEN_1)
		iId = X_GOOD_ULD_CONV_TRAY_SEN_1;
	else if (index == CONV_SEN_2)
		iId = X_GOOD_ULD_CONV_TRAY_SEN_2;
	else if (index == CONV_SEN_3)
		iId = X_GOOD_ULD_CONV_TRAY_SEN_3;
	else if (index == CONV_SEN_4)
		iId = X_GOOD_ULD_CONV_TRAY_SEN_4;
	else if (index == CONV_SEN_5)
		iId = X_GOOD_ULD_CONV_TRAY_SEN_5;
#else
	if (index == CONV_SEN_1)
		iId = X_GOOD_ULD_CONV_TRAY_SEN_1;
	else if (index == CONV_SEN_2)
		return TRUE;
	else if (index == CONV_SEN_3)
		return TRUE;
	else if (index == CONV_SEN_4)
		iId = X_GOOD_ULD_CONV_TRAY_SEN_4;
	else if (index == CONV_SEN_5)
		iId = X_GOOD_ULD_CONV_TRAY_SEN_5;
#endif
	if(GetInPutIOCheck(iId, val))
		return TRUE;
	else
		return FALSE;
}

BOOL CUnitCtrlBank::BufConvSenChk(TRAY_PORT Port, SENSOR_STATE sen)
{
	if(theConfigBank.m_Option.m_bUseDryRun)
		return TRUE;

	ONOFF val = (sen== SENSOR_ON) ? ON:OFF;
#if(AMT_TYPE == 1)
	if (Port == GOOD_IN_TRAY_PORT)
	{
		return (GetInPutIOCheck(X_GOOD_LD_BUFFER_CONV_TRAY_SEN_2,val)
			&& GetInPutIOCheck(X_GOOD_LD_BUFFER_CONV_TRAY_SEN_3,val) && GetInPutIOCheck(X_GOOD_LD_BUFFER_CONV_TRAY_SEN_4,val));
	}
	else if (Port == GOOD_OUT_TRAY_PORT)
	{
		return (GetInPutIOCheck(X_GOOD_ULD_BUFFER_CONV_TRAY_SEN_2,val)
			&& GetInPutIOCheck(X_GOOD_ULD_BUFFER_CONV_TRAY_SEN_3,val) && GetInPutIOCheck(X_GOOD_ULD_BUFFER_CONV_TRAY_SEN_4,val));
	}
	else
	{
		return (GetInPutIOCheck(X_NG_BUFFER_CONV_TRAY_SEN_2,val)
			&& GetInPutIOCheck(X_NG_BUFFER_CONV_TRAY_SEN_3,val) && GetInPutIOCheck(X_NG_BUFFER_CONV_TRAY_SEN_4,val));
	}
#else
	if (Port == GOOD_IN_TRAY_PORT)
	{
		//2017-11-23,SKCHO,tray 투입시 컨베어 감속 시간이 있기때문에 4번에서 감지만 되도 감지되는걸로 수정
		//return (GetInPutIOCheck(X_GOOD_LD_BUFFER_CONV_TRAY_SEN_4,val) && GetInPutIOCheck(X_GOOD_LD_BUFFER_CONV_TRAY_SEN_5,val));
	    return (GetInPutIOCheck(X_GOOD_LD_BUFFER_CONV_TRAY_SEN_4,val) || GetInPutIOCheck(X_GOOD_LD_BUFFER_CONV_TRAY_SEN_5,val));
	}
	else if (Port == GOOD_OUT_TRAY_PORT)
	{
		return (GetInPutIOCheck(X_GOOD_ULD_BUFFER_CONV_TRAY_SEN_1,val) && GetInPutIOCheck(X_GOOD_ULD_BUFFER_CONV_TRAY_SEN_4,val));
	}
	else
	{
		//2017-04-23,skcho, NG 포트는 AGV 통신이 없으므로 3,4번 센서만 사용으로 변경
		//return (GetInPutIOCheck(X_NG_BUFFER_CONV_TRAY_SEN_2,val) && GetInPutIOCheck(X_NG_BUFFER_CONV_TRAY_SEN_3,val) && GetInPutIOCheck(X_NG_BUFFER_CONV_TRAY_SEN_4,val));
		return ( GetInPutIOCheck(X_NG_BUFFER_CONV_TRAY_SEN_3,val) && GetInPutIOCheck(X_NG_BUFFER_CONV_TRAY_SEN_4,val));
	}
#endif
}


void CUnitCtrlBank::ConvRun(TRAY_PORT Port,ONOFF bOn, CWCCW Cw)
{
	if (Port == GOOD_IN_TRAY_PORT)
	{
		if (bOn == TRUE)
		{
			SetOutPutIO(Y_GOOD_LD_CONV_TRAY_INVERTER_CW_RUN,Cw);
			SetOutPutIO(Y_GOOD_LD_CONV_TRAY_INVERTER_CCW_RUN,!Cw);
		}
		else
		{
			SetOutPutIO(Y_GOOD_LD_CONV_TRAY_INVERTER_CW_RUN,FALSE);
			SetOutPutIO(Y_GOOD_LD_CONV_TRAY_INVERTER_CCW_RUN,FALSE);
		}
	}
	else if (Port == GOOD_OUT_TRAY_PORT)
	{
		if (bOn == TRUE)
		{
			SetOutPutIO(Y_GOOD_ULD_CONV_TRAY_INVERTER_CW_RUN,Cw);
			SetOutPutIO(Y_GOOD_ULD_CONV_TRAY_INVERTER_CCW_RUN,!Cw);
		}
		else
		{
			SetOutPutIO(Y_GOOD_ULD_CONV_TRAY_INVERTER_CW_RUN,FALSE);
			SetOutPutIO(Y_GOOD_ULD_CONV_TRAY_INVERTER_CCW_RUN,FALSE);
		}
	}
	else
	{
		if (bOn == TRUE)
		{
			SetOutPutIO(Y_NG_CONV_TRAY_INVERTER_CW_RUN,Cw);
			SetOutPutIO(Y_NG_CONV_TRAY_INVERTER_CCW_RUN,!Cw);
		}
		else
		{
			SetOutPutIO(Y_NG_CONV_TRAY_INVERTER_CW_RUN,FALSE);
			SetOutPutIO(Y_NG_CONV_TRAY_INVERTER_CCW_RUN,FALSE);
		}
	}
}

void CUnitCtrlBank::BufConvRun(TRAY_PORT Port,ONOFF bOn, CWCCW Cw)
{
	if (Port == GOOD_IN_TRAY_PORT)
	{
		if (bOn == TRUE)
		{
			SetOutPutIO(Y_GOOD_LD_BUFFER_CONV_TRAY_INVERTER_CW_RUN,Cw);
			SetOutPutIO(Y_GOOD_LD_BUFFER_CONV_TRAY_INVERTER_CCW_RUN,!Cw);
		}
		else
		{
			SetOutPutIO(Y_GOOD_LD_BUFFER_CONV_TRAY_INVERTER_CW_RUN,FALSE);
			SetOutPutIO(Y_GOOD_LD_BUFFER_CONV_TRAY_INVERTER_CCW_RUN,FALSE);
		}
	}
	else if (Port == GOOD_OUT_TRAY_PORT)
	{
		if (bOn == TRUE)
		{
			SetOutPutIO(Y_GOOD_ULD_BUFFER_CONV_TRAY_INVERTER_CW_RUN,Cw);
			SetOutPutIO(Y_GOOD_ULD_BUFFER_CONV_TRAY_INVERTER_CCW_RUN,!Cw);
		}
		else
		{
			SetOutPutIO(Y_GOOD_ULD_BUFFER_CONV_TRAY_INVERTER_CW_RUN,FALSE);
			SetOutPutIO(Y_GOOD_ULD_BUFFER_CONV_TRAY_INVERTER_CCW_RUN,FALSE);
		}
	}
	else
	{
		if (bOn == TRUE)
		{
			SetOutPutIO(Y_NG_BUFFER_CONV_TRAY_INVERTER_CW_RUN,Cw);
			SetOutPutIO(Y_NG_BUFFER_CONV_TRAY_INVERTER_CCW_RUN,!Cw);
		}
		else
		{
			SetOutPutIO(Y_NG_BUFFER_CONV_TRAY_INVERTER_CW_RUN,FALSE);
			SetOutPutIO(Y_NG_BUFFER_CONV_TRAY_INVERTER_CCW_RUN,FALSE);
		}
	}
}

BOOL CUnitCtrlBank::TrayReqSwChk(TRAY_PORT Port)
{
	if (Port == GOOD_IN_TRAY_PORT)
		return GetInPutIOCheck(X_GOOD_LD_TRAY_IN_SW);
	else if (Port == GOOD_OUT_TRAY_PORT)
		return GetInPutIOCheck(X_GOOD_ULD_TRAY_IN_SW);
	else if (Port == NG_TRAY_PORT)
		return GetInPutIOCheck(X_NG_TRAY_IN_REQ_SW);
	else
		return GetInPutIOCheck(X_NG_TRAY_OUT_REQ_SW);
}

BOOL CUnitCtrlBank::TrayMuteSWChk(TRAY_PORT Port)
{
	BOOL bRet = FALSE;
	
	if (Port == GOOD_IN_TRAY_PORT)
	{
		bRet = GetInPutIOCheck(X_GOOD_LD_TRAY_MUTE_SW);
		if(bRet)
			theProcBank.m_bLDInMuteSWIn = TRUE;
		else
			theProcBank.m_bLDInMuteSWIn = FALSE;

		return bRet;
	}
	else if (Port == GOOD_OUT_TRAY_PORT)
	{
		bRet = GetInPutIOCheck(X_GOOD_ULD_TRAY_MUTE_SW);
		if(bRet)
			theProcBank.m_bLDOutMuteSWIn = TRUE;
		else
			theProcBank.m_bLDOutMuteSWIn = FALSE;

		return bRet;
	}
	else if (Port == NG_TRAY_PORT)
	{
		bRet = GetInPutIOCheck(X_NG_TRAY_MUTE_SW);
		if(bRet)
			theProcBank.m_bNGMuteSWIn = TRUE;
		else
			theProcBank.m_bNGMuteSWIn = FALSE;

		return bRet;
	}

	return bRet;

}

BOOL CUnitCtrlBank::TrayCompSwChk(TRAY_PORT Port)
{
	if (Port == GOOD_IN_TRAY_PORT)
		return GetInPutIOCheck(X_GOOD_LD_TRAY_OUT_SW);
	else if (Port == GOOD_OUT_TRAY_PORT)
		return GetInPutIOCheck(X_GOOD_ULD_TRAY_OUT_SW);
	else if (Port == NG_TRAY_PORT)
		return GetInPutIOCheck(X_NG_TRAY_IN_COMP_SW);
	else
		return GetInPutIOCheck(X_NG_TRAY_OUT_COMP_SW);
}

BOOL CUnitCtrlBank::TrayMuteChk(TRAY_PORT Port)
{
	if (Port == GOOD_IN_TRAY_PORT)
		return GetInPutIOCheck(X_GOOD_LD_TRAY_MUTE_SW);
	else if (Port == GOOD_OUT_TRAY_PORT)
		return GetInPutIOCheck(X_GOOD_ULD_TRAY_MUTE_SW);
	else
		return GetInPutIOCheck(X_NG_TRAY_MUTE_SW);
}

void CUnitCtrlBank::TrayMuteTimeDelay1(TRAY_PORT Port)
{
	if (Port == GOOD_IN_TRAY_PORT)
		m_GoodInMuteTimer1.Start();
	else if (Port == GOOD_OUT_TRAY_PORT)
		m_GoodOutMuteTimer1.Start();
	else
		m_NGMuteTimer1.Start();
}

void CUnitCtrlBank::TrayMuteTimeDelay2(TRAY_PORT Port)
{
	if (Port == GOOD_IN_TRAY_PORT)
		m_GoodInMuteTimer2.Start();
	else if (Port == GOOD_OUT_TRAY_PORT)
		m_GoodOutMuteTimer2.Start();
	else
		m_NGMuteTimer2.Start();
}

BOOL CUnitCtrlBank::TrayExistChk(TRAY_PORT Port)
{
#if(AMT_TYPE == 1)
	if (Port == GOOD_IN_TRAY_PORT)
	{
		return (GetInPutIOCheck(X_GOOD_LD_CONV_TRAY_SEN_1) || GetInPutIOCheck(X_GOOD_LD_CONV_TRAY_SEN_2) || GetInPutIOCheck(X_GOOD_LD_CONV_TRAY_SEN_3)
				|| GetInPutIOCheck(X_GOOD_LD_CONV_TRAY_SEN_4) || GetInPutIOCheck(X_GOOD_LD_CONV_TRAY_SEN_5)
				|| GetInPutIOCheck(X_GOOD_LD_BUFFER_CONV_TRAY_SEN_1) || GetInPutIOCheck(X_GOOD_LD_BUFFER_CONV_TRAY_SEN_2) || GetInPutIOCheck(X_GOOD_LD_BUFFER_CONV_TRAY_SEN_3)
				|| GetInPutIOCheck(X_GOOD_LD_BUFFER_CONV_TRAY_SEN_4) || GetInPutIOCheck(X_GOOD_LD_BUFFER_CONV_TRAY_SEN_5));	
	}
	if (Port == GOOD_OUT_TRAY_PORT)
	{
		return (GetInPutIOCheck(X_GOOD_ULD_CONV_TRAY_SEN_1) || GetInPutIOCheck(X_GOOD_ULD_CONV_TRAY_SEN_2) || GetInPutIOCheck(X_GOOD_ULD_CONV_TRAY_SEN_3)
			|| GetInPutIOCheck(X_GOOD_ULD_CONV_TRAY_SEN_4) || GetInPutIOCheck(X_GOOD_ULD_CONV_TRAY_SEN_5)
			|| GetInPutIOCheck(X_GOOD_ULD_BUFFER_CONV_TRAY_SEN_1) || GetInPutIOCheck(X_GOOD_ULD_BUFFER_CONV_TRAY_SEN_2) || GetInPutIOCheck(X_GOOD_ULD_BUFFER_CONV_TRAY_SEN_3)
			|| GetInPutIOCheck(X_GOOD_ULD_BUFFER_CONV_TRAY_SEN_4) || GetInPutIOCheck(X_GOOD_ULD_BUFFER_CONV_TRAY_SEN_5));	
	}
	if (Port == NG_TRAY_PORT)
	{
		return (GetInPutIOCheck(X_NG_CONV_TRAY_SEN_1) || GetInPutIOCheck(X_NG_CONV_TRAY_SEN_2) || GetInPutIOCheck(X_NG_CONV_TRAY_SEN_3)
			|| GetInPutIOCheck(X_NG_CONV_TRAY_SEN_4) || GetInPutIOCheck(X_NG_CONV_TRAY_SEN_5)
			|| GetInPutIOCheck(X_NG_BUFFER_CONV_TRAY_SEN_1) || GetInPutIOCheck(X_NG_BUFFER_CONV_TRAY_SEN_2) || GetInPutIOCheck(X_NG_BUFFER_CONV_TRAY_SEN_3)
			|| GetInPutIOCheck(X_NG_BUFFER_CONV_TRAY_SEN_4) || GetInPutIOCheck(X_NG_BUFFER_CONV_TRAY_SEN_5));	
	}
#else
	if (Port == GOOD_IN_TRAY_PORT)
	{
		return (GetInPutIOCheck(X_GOOD_LD_CONV_TRAY_SEN_1) 
				|| GetInPutIOCheck(X_GOOD_LD_CONV_TRAY_SEN_4) || GetInPutIOCheck(X_GOOD_LD_CONV_TRAY_SEN_5)
				|| GetInPutIOCheck(X_GOOD_LD_BUFFER_CONV_TRAY_SEN_1) 
				|| GetInPutIOCheck(X_GOOD_LD_BUFFER_CONV_TRAY_SEN_4) || GetInPutIOCheck(X_GOOD_LD_BUFFER_CONV_TRAY_SEN_5));	
	}
	if (Port == GOOD_OUT_TRAY_PORT)
	{
		return (GetInPutIOCheck(X_GOOD_ULD_CONV_TRAY_SEN_1) 
			|| GetInPutIOCheck(X_GOOD_ULD_CONV_TRAY_SEN_4) || GetInPutIOCheck(X_GOOD_ULD_CONV_TRAY_SEN_5)
			|| GetInPutIOCheck(X_GOOD_ULD_BUFFER_CONV_TRAY_SEN_1) 
			|| GetInPutIOCheck(X_GOOD_ULD_BUFFER_CONV_TRAY_SEN_4) || GetInPutIOCheck(X_GOOD_ULD_BUFFER_CONV_TRAY_SEN_5));	
	}
	if (Port == NG_TRAY_PORT)
	{
		return (GetInPutIOCheck(X_NG_CONV_TRAY_SEN_1) || GetInPutIOCheck(X_NG_CONV_TRAY_SEN_2) || GetInPutIOCheck(X_NG_CONV_TRAY_SEN_3)
			|| GetInPutIOCheck(X_NG_CONV_TRAY_SEN_4) || GetInPutIOCheck(X_NG_CONV_TRAY_SEN_5)
			|| GetInPutIOCheck(X_NG_BUFFER_CONV_TRAY_SEN_1) || GetInPutIOCheck(X_NG_BUFFER_CONV_TRAY_SEN_2) || GetInPutIOCheck(X_NG_BUFFER_CONV_TRAY_SEN_3)
			|| GetInPutIOCheck(X_NG_BUFFER_CONV_TRAY_SEN_4) || GetInPutIOCheck(X_NG_BUFFER_CONV_TRAY_SEN_5));	
	}
#endif
	return FALSE;
}

void CUnitCtrlBank::AlignCylFwdBwd(TRAY_PORT Port,CYL_STATE Cyl)
{
	if (Port == GOOD_IN_TRAY_PORT)
	{
		SetOutPutIO(Y_GOOD_LD_TRAY_UPPER_ALIGN_GUIDE_FORWARD_SOL_1,Cyl);
		SetOutPutIO(Y_GOOD_LD_TRAY_UPPER_ALIGN_GUIDE_BACKWARD_SOL_1,!Cyl);
		SetOutPutIO(Y_GOOD_LD_TRAY_UPPER_ALIGN_GUIDE_FORWARD_SOL_2,Cyl);
		SetOutPutIO(Y_GOOD_LD_TRAY_UPPER_ALIGN_GUIDE_BACKWARD_SOL_2,!Cyl);
	}
	else if (Port == NG_TRAY_PORT)
	{
		SetOutPutIO(Y_NG_TRAY_UPPER_ALIGN_GUIDE_FORWARD_SOL_1,Cyl);
		SetOutPutIO(Y_NG_TRAY_UPPER_ALIGN_GUIDE_BACKWARD_SOL_1,!Cyl);
		SetOutPutIO(Y_NG_TRAY_UPPER_ALIGN_GUIDE_FORWARD_SOL_2,Cyl);
		SetOutPutIO(Y_NG_TRAY_UPPER_ALIGN_GUIDE_BACKWARD_SOL_2,!Cyl);
	}
}

BOOL CUnitCtrlBank::AlignCylFwdBwdChk(TRAY_PORT Port,CYL_STATE Cyl)
{
	if (Port == GOOD_IN_TRAY_PORT)
	{
		if (Cyl == CYL_FWD)
		{
			return (GetInPutIOCheck(X_GOOD_LD_TRAY_UPPER_ALIGN_GUIDE_FORWARD_SEN_1) && GetInPutIOCheck(X_GOOD_LD_TRAY_UPPER_ALIGN_GUIDE_FORWARD_SEN_2)
				&& GetInPutIOCheck(X_GOOD_LD_TRAY_UPPER_ALIGN_GUIDE_FORWARD_SEN_3) && GetInPutIOCheck(X_GOOD_LD_TRAY_UPPER_ALIGN_GUIDE_FORWARD_SEN_4));
		}
		else
		{
			return (GetInPutIOCheck(X_GOOD_LD_TRAY_UPPER_ALIGN_GUIDE_BACKWARD_SEN_1) && GetInPutIOCheck(X_GOOD_LD_TRAY_UPPER_ALIGN_GUIDE_BACKWARD_SEN_2)
				&& GetInPutIOCheck(X_GOOD_LD_TRAY_UPPER_ALIGN_GUIDE_BACKWARD_SEN_3) && GetInPutIOCheck(X_GOOD_LD_TRAY_UPPER_ALIGN_GUIDE_BACKWARD_SEN_4));
		}
	}
	else 
	{
		if (Cyl == CYL_FWD)
		{
			return (GetInPutIOCheck(X_NG_TRAY_UPPER_ALIGN_GUIDE_FORWARD_SEN_1) && GetInPutIOCheck(X_NG_TRAY_UPPER_ALIGN_GUIDE_FORWARD_SEN_2)
				&& GetInPutIOCheck(X_NG_TRAY_UPPER_ALIGN_GUIDE_FORWARD_SEN_3) && GetInPutIOCheck(X_NG_TRAY_UPPER_ALIGN_GUIDE_FORWARD_SEN_4));
		}
		else
		{
			return (GetInPutIOCheck(X_NG_TRAY_UPPER_ALIGN_GUIDE_BACKWARD_SEN_1) && GetInPutIOCheck(X_NG_TRAY_UPPER_ALIGN_GUIDE_BACKWARD_SEN_2)
				&& GetInPutIOCheck(X_NG_TRAY_UPPER_ALIGN_GUIDE_BACKWARD_SEN_3) && GetInPutIOCheck(X_NG_TRAY_UPPER_ALIGN_GUIDE_BACKWARD_SEN_4));
		}
	}
}

void CUnitCtrlBank::DivisionCylFwdBwd(TRAY_PORT Port,CYL_STATE Cyl)
{
	if (Port == GOOD_IN_TRAY_PORT)
	{
		SetOutPutIO(Y_GOOD_LD_TRAY_UPPER_DIVISION_GUIDE_FORWARD_SOL_1,Cyl);
		SetOutPutIO(Y_GOOD_LD_TRAY_UPPER_DIVISION_GUIDE_BACKWARD_SOL_1,!Cyl);
		SetOutPutIO(Y_GOOD_LD_TRAY_UPPER_DIVISION_GUIDE_FORWARD_SOL_2,Cyl);
		SetOutPutIO(Y_GOOD_LD_TRAY_UPPER_DIVISION_GUIDE_BACKWARD_SOL_2,!Cyl);
	}
	else
	{
		SetOutPutIO(Y_NG_TRAY_UPPER_DIVISION_GUIDE_FORWARD_SOL_1,Cyl);
		SetOutPutIO(Y_NG_TRAY_UPPER_DIVISION_GUIDE_BACKWARD_SOL_1,!Cyl);
		SetOutPutIO(Y_NG_TRAY_UPPER_DIVISION_GUIDE_FORWARD_SOL_2,Cyl);
		SetOutPutIO(Y_NG_TRAY_UPPER_DIVISION_GUIDE_BACKWARD_SOL_2,!Cyl);
	}
}

BOOL CUnitCtrlBank::DivisionCylFwdBwdChk(TRAY_PORT Port,CYL_STATE Cyl)
{
	if (Port == GOOD_IN_TRAY_PORT)
	{
		if (Cyl == CYL_FWD)
		{
			return (GetInPutIOCheck(X_GOOD_LD_TRAY_UPPER_DIVISION_GUIDE_FORWARD_SEN_1) && GetInPutIOCheck(X_GOOD_LD_TRAY_UPPER_DIVISION_GUIDE_FORWARD_SEN_2));
		}
		else
		{
			return (GetInPutIOCheck(X_GOOD_LD_TRAY_UPPER_DIVISION_GUIDE_BACKWARD_SEN_1) && GetInPutIOCheck(X_GOOD_LD_TRAY_UPPER_DIVISION_GUIDE_BACKWARD_SEN_2));
		}
	}
	else 
	{
		if (Cyl == CYL_FWD)
		{
			return (GetInPutIOCheck(X_NG_TRAY_UPPER_DIVISION_GUIDE_FORWARD_SEN_1) && GetInPutIOCheck(X_NG_TRAY_UPPER_DIVISION_GUIDE_FORWARD_SEN_2));
		}
		else
		{
			return (GetInPutIOCheck(X_NG_TRAY_UPPER_DIVISION_GUIDE_BACKWARD_SEN_1) && GetInPutIOCheck(X_NG_TRAY_UPPER_DIVISION_GUIDE_BACKWARD_SEN_2));
		}
	}
}

void CUnitCtrlBank::BottomAlignCylFwdBwd(TRAY_PORT Port,CYL_STATE Cyl)
{
	if (Port == GOOD_IN_TRAY_PORT)
	{
		SetOutPutIO(Y_GOOD_LD_TRAY_LOWER_ALIGN_GUIDE_FORWARD_SOL,Cyl);
		SetOutPutIO(Y_GOOD_LD_TRAY_LOWER_ALIGN_GUIDE_BACKWARD_SOL,!Cyl);
	}
	else if(Port == GOOD_OUT_TRAY_PORT)
	{
		SetOutPutIO(Y_GOOD_ULD_TRAY_LOWER_ALIGN_GUIDE_FORWARD_SOL,Cyl);
		SetOutPutIO(Y_GOOD_ULD_TRAY_LOWER_ALIGN_GUIDE_BACKWARD_SOL,!Cyl);
	}
	else
	{
		SetOutPutIO(Y_NG_TRAY_LOWER_ALIGN_GUIDE_FORWARD_SOL_1,Cyl);
		SetOutPutIO(Y_NG_TRAY_LOWER_ALIGN_GUIDE_BACKWARD_SOL_1,!Cyl);
		SetOutPutIO(Y_NG_TRAY_LOWER_ALIGN_GUIDE_FORWARD_SOL_2,Cyl);
		SetOutPutIO(Y_NG_TRAY_LOWER_ALIGN_GUIDE_BACKWARD_SOL_2,!Cyl);
	}
}


BOOL CUnitCtrlBank::BottomAlignCylFwdBwdChk(TRAY_PORT Port,CYL_STATE Cyl)
{
	BOOL bRtn[2] = {0};
	if (Port == GOOD_IN_TRAY_PORT)
	{
		if (Cyl == CYL_FWD)
		{
			return (GetInPutIOCheck(X_GOOD_LD_TRAY_LOWER_ALIGN_GUIDE_FORWARD_SEN_1) && GetInPutIOCheck(X_GOOD_LD_TRAY_LOWER_ALIGN_GUIDE_FORWARD_SEN_2));
		}
		else
		{
			return (GetInPutIOCheck(X_GOOD_LD_TRAY_LOWER_ALIGN_GUIDE_BACKWARD_SEN_1) && GetInPutIOCheck(X_GOOD_LD_TRAY_LOWER_ALIGN_GUIDE_BACKWARD_SEN_2));
		}
	}
	else if (Port == GOOD_OUT_TRAY_PORT)
	{
		if (Cyl == CYL_FWD)
		{
			return (GetInPutIOCheck(X_GOOD_ULD_TRAY_LOWER_ALIGN_GUIDE_FORWARD_SEN_1) && GetInPutIOCheck(X_GOOD_ULD_TRAY_LOWER_ALIGN_GUIDE_FORWARD_SEN_2));
		}
		else
		{
			return (GetInPutIOCheck(X_GOOD_ULD_TRAY_LOWER_ALIGN_GUIDE_BACKWARD_SEN_1) && GetInPutIOCheck(X_GOOD_ULD_TRAY_LOWER_ALIGN_GUIDE_BACKWARD_SEN_2));
		}
	}
	else 
	{
		if (Cyl == CYL_FWD)
		{
			bRtn[0] = GetInPutIOCheck(X_NG_TRAY_LOWER_ALIGN_GUIDE_FORWARD_SEN_1);
			bRtn[1] = GetInPutIOCheck(X_NG_TRAY_LOWER_ALIGN_GUIDE_FORWARD_SEN_2);
			return (bRtn[0] && bRtn[1]);
		}
		else
		{
			bRtn[0] =GetInPutIOCheck(X_NG_TRAY_LOWER_ALIGN_GUIDE_BACKWARD_SEN_1) ;
			bRtn[1] =GetInPutIOCheck(X_NG_TRAY_LOWER_ALIGN_GUIDE_BACKWARD_SEN_2);
			return (bRtn[0]&& bRtn[1]);
		}
	}
}

void CUnitCtrlBank:: BottomStoperUpDown(TRAY_PORT Port,CYL_STATE Cyl,PORT_POS Pos)
{
	if (Port == GOOD_IN_TRAY_PORT)
	{
		if (Pos == TRAY_BUFFER)
		{
			#if(AMT_TYPE == 1)
				SetOutPutIO(Y_GOOD_LD_TRAY_LOWER_STOPPER_UP_SOL_2,Cyl);
				SetOutPutIO(Y_GOOD_LD_TRAY_LOWER_STOPPER_DOWN_SOL_2,!Cyl);					
			#else
				SetOutPutIO(Y_GOOD_LD_TRAY_LOWER_STOPPER_UP_SOL_1,Cyl);
				SetOutPutIO(Y_GOOD_LD_TRAY_LOWER_STOPPER_DOWN_SOL_1,!Cyl);
			#endif
		}
		else
		{
			#if(AMT_TYPE == 1)
				SetOutPutIO(Y_GOOD_LD_TRAY_LOWER_STOPPER_UP_SOL_1,Cyl);
				SetOutPutIO(Y_GOOD_LD_TRAY_LOWER_STOPPER_DOWN_SOL_1,!Cyl);
			#else
				SetOutPutIO(Y_GOOD_LD_TRAY_LOWER_STOPPER_UP_SOL_2,Cyl);
				SetOutPutIO(Y_GOOD_LD_TRAY_LOWER_STOPPER_DOWN_SOL_2,!Cyl);
			#endif
		}
	}
	else if(Port == GOOD_OUT_TRAY_PORT)
	{
		if (Pos == TRAY_BUFFER)
		{
			
			#if(AMT_TYPE == 1)
				SetOutPutIO(Y_GOOD_ULD_TRAY_LOWER_STOPPER_UP_SOL_2,Cyl);
				SetOutPutIO(Y_GOOD_ULD_TRAY_LOWER_STOPPER_DOWN_SOL_2,!Cyl);
			#else
				SetOutPutIO(Y_GOOD_ULD_TRAY_LOWER_STOPPER_UP_SOL_1,Cyl);
				SetOutPutIO(Y_GOOD_ULD_TRAY_LOWER_STOPPER_DOWN_SOL_1,!Cyl);
			#endif
		}
		else
		{
			#if(AMT_TYPE == 1)
				SetOutPutIO(Y_GOOD_ULD_TRAY_LOWER_STOPPER_UP_SOL_1,Cyl);
				SetOutPutIO(Y_GOOD_ULD_TRAY_LOWER_STOPPER_DOWN_SOL_1,!Cyl);
			#else
				SetOutPutIO(Y_GOOD_ULD_TRAY_LOWER_STOPPER_UP_SOL_2,Cyl);
				SetOutPutIO(Y_GOOD_ULD_TRAY_LOWER_STOPPER_DOWN_SOL_2,!Cyl);
			#endif
		}
	}
	else
	{
		if (Pos == TRAY_BUFFER)
		{
		
			#if(AMT_TYPE == 1)
				SetOutPutIO(Y_NG_TRAY_LOWER_STOPPER_UP_SOL_2,Cyl);
				SetOutPutIO(Y_NG_TRAY_LOWER_STOPPER_DOWN_SOL_2,!Cyl);
			#else
				SetOutPutIO(Y_NG_TRAY_LOWER_STOPPER_UP_SOL_1,Cyl);
				SetOutPutIO(Y_NG_TRAY_LOWER_STOPPER_DOWN_SOL_1,!Cyl);
			#endif
		}
		else
		{
			#if(AMT_TYPE == 1)
				SetOutPutIO(Y_NG_TRAY_LOWER_STOPPER_UP_SOL_1,Cyl);
				SetOutPutIO(Y_NG_TRAY_LOWER_STOPPER_DOWN_SOL_1,!Cyl);
			#else
				SetOutPutIO(Y_NG_TRAY_LOWER_STOPPER_UP_SOL_2,Cyl);
				SetOutPutIO(Y_NG_TRAY_LOWER_STOPPER_DOWN_SOL_2,!Cyl);
			#endif
		}
	}
}


BOOL CUnitCtrlBank::BottomStoperUpDownChk(TRAY_PORT Port,CYL_STATE Cyl,PORT_POS Pos)
{
	PORT_POS TrayPos;
	#if(AMT_TYPE == 1)
	TrayPos = TRAY_IN;
	#else
	TrayPos = TRAY_BUFFER;
	#endif

	if (Port == GOOD_IN_TRAY_PORT)
	{
//		if (Pos == TRAY_BUFFER)
		if (Pos == TrayPos)
		{
			if (Cyl == CYL_UP)
				return GetInPutIOCheck(X_GOOD_LD_TRAY_LOWER_STOPPER_UP_SEN_1);
			else
				return GetInPutIOCheck(X_GOOD_LD_TRAY_LOWER_STOPPER_DOWN_SEN_1);
		}
		else
		{
			if (Cyl == CYL_UP)
				return GetInPutIOCheck(X_GOOD_LD_TRAY_LOWER_STOPPER_UP_SEN_2);
			else
				return GetInPutIOCheck(X_GOOD_LD_TRAY_LOWER_STOPPER_DOWN_SEN_2);
		}
	}
	else if(Port == GOOD_OUT_TRAY_PORT)
	{
//		if (Pos == TRAY_BUFFER)
		if (Pos == TrayPos)
		{
			if (Cyl == CYL_UP)
				return GetInPutIOCheck(X_GOOD_ULD_TRAY_LOWER_STOPPER_UP_SEN_1);
			else
				return GetInPutIOCheck(X_GOOD_ULD_TRAY_LOWER_STOPPER_DOWN_SEN_1);
		}
		else
		{
			if (Cyl == CYL_UP)
				return GetInPutIOCheck(X_GOOD_ULD_TRAY_LOWER_STOPPER_UP_SEN_2);
			else
				return GetInPutIOCheck(X_GOOD_ULD_TRAY_LOWER_STOPPER_DOWN_SEN_2);
		}
	}
	else
	{
//		if (Pos == TRAY_BUFFER)
		if (Pos == TrayPos)
		{
			if (Cyl == CYL_UP)
				return GetInPutIOCheck(X_NG_TRAY_LOWER_STOPPER_UP_SEN_1);
			else
				return GetInPutIOCheck(X_NG_TRAY_LOWER_STOPPER_DOWN_SEN_1);
		}
		else
		{
			if (Cyl == CYL_UP)
				return GetInPutIOCheck(X_NG_TRAY_LOWER_STOPPER_UP_SEN_2);
			else
				return GetInPutIOCheck(X_NG_TRAY_LOWER_STOPPER_DOWN_SEN_2);
		}
	}
}

void CUnitCtrlBank::MuteOn(ONOFF bOn,TRAY_PORT Port)
{
	if (Port == GOOD_IN_TRAY_PORT)
		SetOutPutIO(Y_GOOD_LD_TRAY_MUTE_ON,bOn);
	else if (Port == GOOD_OUT_TRAY_PORT)
		SetOutPutIO(Y_GOOD_ULD_TRAY_MUTE_ON,bOn);
	else 
		SetOutPutIO(Y_NG_TRAY_MUTE_ON,bOn);
}

void CUnitCtrlBank::TrayAlignStart(CString strPort)
{
	if (theConfigBank.m_Option.m_bUseTrayAlign)
	{
		if (strPort == _T("OKTRAY"))
			theProcBank.m_strOKTray_AlignOK = _T("");
		else
			theProcBank.m_strNGTray_AlignOK = _T("");

		theSocketInterFace.SendMsgToTrayAlign(TRAY_ALIGN_REQ, strPort);
	}
}

BOOL CUnitCtrlBank::TrayAlignChk(CString strPort)
{
	if (theConfigBank.m_Option.m_bUseTrayAlign == FALSE)
		return TRUE;
	else
	{
		if (strPort == _T("OKTRAY"))
		{
			if (theProcBank.m_strOKTray_AlignOK == _T("OK"))
				return TRUE;
			else
				return FALSE;
		}
		else
		{
			if (theProcBank.m_strNGTray_AlignOK == _T("OK"))
				return TRUE;
			else
				return FALSE;
		}
	}
}

void CUnitCtrlBank::TrayAlignTeaching(CString strPort)
{
	theSocketInterFace.SendMsgToTrayAlign(TRAY_ALIGN_TEACHING, strPort);
}

void CUnitCtrlBank::DCRTrigger(BOOL On)
{
	//2017-07-06,SKCHO,  고객사 요청으로 DCR 옵션 기능 삭제
	/*if (theConfigBank.m_Option.m_bUseDCR)
	{
		if(On == ON)
			theProcBank.m_strTrayID.Empty();

		SetOutPutIO((OUTPUT_ID) Y_DCR_TRIGGER, On);	
	}*/

	if(On == ON)
	{
		theProcBank.m_strTrayID.Empty();
	}

	SetOutPutIO((OUTPUT_ID) Y_DCR_TRIGGER, On);	
}

BOOL CUnitCtrlBank::DCRChk()
{
	//2017-07-06,SKCHO,  고객사 요청으로 DCR 옵션 기능 삭제
	/*if (theConfigBank.m_Option.m_bUseDCR == FALSE)
		return TRUE;
	else
	{
		if (theProcBank.m_strTrayID != _T(""))
		{
			theLog[LOG_SEQUENCE_TRAY].AddBuf( _T("Tray ID : %s"), theProcBank.m_strTrayID);			
			return TRUE;
		}
		else
			return FALSE;
	}*/
	//2017-07-14,SKCHO, DRY RUN에서 동작되게 함
	if (theConfigBank.m_Option.m_bUseDryRun)
		return TRUE;

	if (theProcBank.m_strTrayID != _T(""))
	{
		theLog[LOG_SEQUENCE_TRAY].AddBuf( _T("Tray ID : %s"), theProcBank.m_strTrayID);			
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	
}
//2018-03-14,MGYUN, GOOD IN LIFT 마지막 트레이 감지 센서 체크 함수
BOOL CUnitCtrlBank::InTrayLiftZ_FinalSenChk(SENSOR_STATE sen)
{
	if(theConfigBank.m_Option.m_bUseDryRun)
	return TRUE;

	ONOFF val = (sen== SENSOR_ON) ? ON:OFF;
	
	INPUT_ID iId = X_GOOD_LD_TRAY_FINAL_SEN_1;
		
	if(GetInPutIOCheck(iId, val))
		return TRUE;
	else
		return FALSE;
	
}