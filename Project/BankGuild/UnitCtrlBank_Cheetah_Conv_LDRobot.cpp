#include "StdAfx.h"
#include "UnitCtrlBank.h"
#include "CheetahApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//void CUnitCtrlBank::CONV_Start()
//{
//	const OUTPUT_ID id = Y_CONV;
//	SetOutPutIO(id, ON);
//}
//
//void CUnitCtrlBank::CONV_Stop()
//{
//	const OUTPUT_ID id = Y_CONV;
//	SetOutPutIO(id, ON);
//}
//
//void CUnitCtrlBank::Move_LDRobot_ConvPos()
//{
//	const OUTPUT_ID id = Y_LDROBOT_MOVE_CONV;
//	SetOutPutIO(id, ON);
//}
//
//void CUnitCtrlBank::Move_LDRobot_Shuttle1_LeftPos()
//{
//	const OUTPUT_ID id = Y_LDROBOT_MOVE_SHUTTLE1_LEFT;
//	SetOutPutIO(id, ON);
//}
//
//void CUnitCtrlBank::Move_LDRobot_Shuttle1_RightPos()
//{
//	const OUTPUT_ID id = Y_LDROBOT_MOVE_SHUTTLE1_RIGHT;
//	SetOutPutIO(id, ON);
//}
//
//void CUnitCtrlBank::Move_LDRobot_Shuttle2_LeftPos()
//{
//	const OUTPUT_ID id = Y_LDROBOT_MOVE_SHUTTLE2_LEFT;
//	SetOutPutIO(id, ON);
//}
//
//void CUnitCtrlBank::Move_LDRobot_Shuttle2_RightPos()
//{
//	const OUTPUT_ID id = Y_LDROBOT_MOVE_SHUTTLE2_RIGHT;
//	SetOutPutIO(id, ON);
//}
//
//void CUnitCtrlBank::OnOff_LDRobot_Vacuum(ONOFF vac)
//{
//	const OUTPUT_ID id = Y_LDROBOT_VAC;
//	SetOutPutIO(id, vac);
//}
//
//BOOL CUnitCtrlBank::Chk_LDRobot_Vacuum()
//{
//	const INPUT_ID id = X_CONV_CELLSTATE;
//	return GetInPutIOCheck(id, ON);
//}

BOOL CUnitCtrlBank::LoaderAlignEndChk()
{
	CCellInfo* pCellInfo;
	CCellTag tagCell;
	pCellInfo = theCellBank.GetCellInfo(CELL_POS_LD_CONV);
	tagCell = theCellBank.GetCellTag(CELL_POS_LD_CONV);
	if(tagCell.IsExist())
	{
		return (pCellInfo->LoaderAlignResult != ALIGN_RESULT_NONE) ? TRUE:FALSE;
	}
	return FALSE;
}


BOOL CUnitCtrlBank::LoaderAlignResultChk()
{
	CCellInfo* pCellInfo;
	CCellTag tagCell;
	pCellInfo = theCellBank.GetCellInfo(CELL_POS_LD_CONV);
	tagCell = theCellBank.GetCellTag(CELL_POS_LD_CONV);
	if(tagCell.IsExist())
	{
		return (pCellInfo->LoaderAlignResult == ALIGN_RESULT_GOOD) ? TRUE:FALSE;
	}
	return FALSE;
}

void CUnitCtrlBank::LDRobotLoadingPosMove( double dPosX , double dPosY , double dPosT)
{
	theSocketInterFace.SendMsgToRobot(E_LD_ROBOT,ROBOT_MOVE_LOAD, dPosX, dPosY, dPosT);
}

//void CUnitCtrlBank::LDRobotUpperLeftPosMove()
//{
//	theSocketInterFace.SendMsgToRobot(E_LD_ROBOT,ROBOT_MOVE_UDUL);
//}
//
//void CUnitCtrlBank::LDRobotUpperRightPosMove()
//{
//	theSocketInterFace.SendMsgToRobot(E_LD_ROBOT,ROBOT_MOVE_UDUR);
//}
//
//void CUnitCtrlBank::LDRobotLowerLeftPosMove()
//{
//	theSocketInterFace.SendMsgToRobot(E_LD_ROBOT,ROBOT_MOVE_UDLL);
//}
//
//void CUnitCtrlBank::LDRobotLowerRightPosMove()
//{
//	theSocketInterFace.SendMsgToRobot(E_LD_ROBOT,ROBOT_MOVE_UDLR);
//}

void CUnitCtrlBank::LDRobotWaitPosMove()
{
	theSocketInterFace.SendMsgToRobot(E_LD_ROBOT,ROBOT_MOVE_WAIT);
}

BOOL CUnitCtrlBank::LDRobotLoadingPosChk()
{
	if(RobotAlarmChk(E_LD_ROBOT))
		return FALSE;
	return theUnitFunc.GetInPutIOCheck(X_LD_ROBOT_LOADING_POS_CHK);
}

//BOOL CUnitCtrlBank::LDRobotUpperLeftUnloadingPosChk()
//{
//	if(RobotAlarmChk(E_LD_ROBOT))
//		return FALSE;
//	return   theUnitFunc.GetInPutIOCheck(X_LD_ROBOT_UPPER_LEFT_UNLOADING_POS_CHK);
//}
//
//BOOL CUnitCtrlBank::LDRobotUpperRightUnloadingPosChk()
//{
//	if(RobotAlarmChk(E_LD_ROBOT))
//		return FALSE;
//	return   theUnitFunc.GetInPutIOCheck(X_LD_ROBOT_UPPER_RIGHT_UNLOADING_POS_CHK);
//}
//
//BOOL CUnitCtrlBank::LDRobotLowerLeftUnloadingPosChk()
//{
//	if(RobotAlarmChk(E_LD_ROBOT))
//		return FALSE;
//	return   theUnitFunc.GetInPutIOCheck(X_LD_ROBOT_LOWER_LEFT_UNLOADING_POS_CHK);
//}
//
//BOOL CUnitCtrlBank::LDRobotLowerRightUnloadingPosChk()
//{
//	if(RobotAlarmChk(E_LD_ROBOT))
//		return FALSE;
//	return   theUnitFunc.GetInPutIOCheck(X_LD_ROBOT_LOWER_RIGHT_UNLOADING_POS_CHK);
//}

BOOL CUnitCtrlBank::LDRobotWaitPosChk()
{
	if(RobotAlarmChk(E_LD_ROBOT))
		return FALSE;
	return   theUnitFunc.GetInPutIOCheck(X_LD_ROBOT_WAIT_POS_CHECK);
}

//void CUnitCtrlBank::UDRobotLeftLoadingPosMove()
//{
//	theSocketInterFace.SendMsgToRobot(E_UD_ROBOT,ROBOT_MOVE_LTLD);
//}
//
//void CUnitCtrlBank::UDRobotRightLoadingPosMove()
//{
//	theSocketInterFace.SendMsgToRobot(E_UD_ROBOT,ROBOT_MOVE_RTLD);
//}

void CUnitCtrlBank::UDRobotGoodUnloadingPosMove()
{
	theSocketInterFace.SendMsgToRobot(E_UD_ROBOT,ROBOT_MOVE_GDUD);
}

void CUnitCtrlBank::UDRobotNGUnloadingPosMove()
{
	theSocketInterFace.SendMsgToRobot(E_UD_ROBOT,ROBOT_MOVE_NGUD);
}

void CUnitCtrlBank::UDRobotWaitPosMove()
{
	theSocketInterFace.SendMsgToRobot(E_UD_ROBOT,ROBOT_MOVE_WAIT);
}

//BOOL CUnitCtrlBank::UDRobotLeftLoadingPosChk()
//{
//	if(RobotAlarmChk(E_UD_ROBOT))
//		return FALSE;
//	return   theUnitFunc.GetInPutIOCheck(X_ULD_ROBOT_LEFT_LOADING_POS_CHK);
//}
//
//BOOL CUnitCtrlBank::UDRobotRightLoadingPosChk()
//{
//	if(RobotAlarmChk(E_UD_ROBOT))
//		return FALSE;
//	return   theUnitFunc.GetInPutIOCheck(X_ULD_ROBOT_RIGHT_LOADING_POS_CHK);
//}

BOOL CUnitCtrlBank::UDRobotGoodUnloadingPosChk()
{
	if(RobotAlarmChk(E_UD_ROBOT))
		return FALSE;
	return   theUnitFunc.GetInPutIOCheck(X_ULD_ROBOT_GOOD_UNOADING_POS_CHK);
}

BOOL CUnitCtrlBank::UDRobotNGUnloadingPosChk()
{
	if(RobotAlarmChk(E_UD_ROBOT))
		return FALSE;
	return   theUnitFunc.GetInPutIOCheck(X_ULD_ROBOT_NG_UNOADING_POS_CHK);
}

BOOL CUnitCtrlBank::ULDRobotWaitPosChk()
{
	if(RobotAlarmChk(E_UD_ROBOT))
		return FALSE;
	return   theUnitFunc.GetInPutIOCheck(X_ULD_ROBOT_WAIT_POS_CHECK);
}

BOOL CUnitCtrlBank::RobotAlarmChk(E_ROBOT Robot)
{
	if (Robot == E_LD_ROBOT)
		return   theUnitFunc.GetInPutIOCheck(X_LD_ROBOT_ALARM_CHK);
	else
		return   theUnitFunc.GetInPutIOCheck(X_ULD_ROBOT_ALARM_CHK);
}

BOOL CUnitCtrlBank::RobotReadyChk(E_ROBOT Robot)
{
	if (Robot == E_LD_ROBOT)
		return   theUnitFunc.GetInPutIOCheck(X_LD_ROBOT_READY_CHK);
	else
		return   theUnitFunc.GetInPutIOCheck(X_ULD_ROBOT_READY_CHK);
}

BOOL CUnitCtrlBank::RobotRunChk(E_ROBOT Robot)
{
	if (Robot == E_LD_ROBOT)
		return   theUnitFunc.GetInPutIOCheck(X_LD_ROBOT_RUN_CHK);
	else
		return   theUnitFunc.GetInPutIOCheck(X_ULD_ROBOT_RUN_CHK);
}

BOOL CUnitCtrlBank::RobotInterlockAreaChk(E_ROBOT Robot)
{
	if (Robot == E_LD_ROBOT)
		return   theUnitFunc.GetInPutIOCheck(X_LD_ROBOT_INTERLOCK_AREA_CHK);
	else
		return   theUnitFunc.GetInPutIOCheck(X_ULD_ROBOT_INTERLOCK_AREA_CHK);
}

BOOL CUnitCtrlBank::RobotOriginPosChk(E_ROBOT Robot)
{
	if (Robot == E_LD_ROBOT)
		return   theUnitFunc.GetInPutIOCheck(X_LD_ROBOT_ORIGIN_POS_CHK);
	else
		return   theUnitFunc.GetInPutIOCheck(X_ULD_ROBOT_ORIGIN_POS_CHK);
}

BOOL CUnitCtrlBank::RobotStopChk(E_ROBOT Robot)
{
	if (Robot == E_LD_ROBOT)
		return   theUnitFunc.GetInPutIOCheck(X_LD_ROBOT_STOP_CHK);
	else
		return   theUnitFunc.GetInPutIOCheck(X_ULD_ROBOT_STOP_CHK);
}

BOOL CUnitCtrlBank::RobotEStopChk(E_ROBOT Robot)
{
	if (Robot == E_LD_ROBOT)
		return   theUnitFunc.GetInPutIOCheck(X_LD_ROBOT_ESTOP_CHK);
	else
		return   theUnitFunc.GetInPutIOCheck(X_ULD_ROBOT_ESTOP_CHK);
}

BOOL CUnitCtrlBank::RobotHeavyAlarmChk(E_ROBOT Robot)
{
	if (Robot == E_LD_ROBOT)
		return   theUnitFunc.GetInPutIOCheck(X_LD_ROBOT_HEAVY_ALARM_CHK);
	else
		return   theUnitFunc.GetInPutIOCheck(X_ULD_ROBOT_HEAVY_ALARM_CHK);
}

void CUnitCtrlBank::RobotSetOnline( const BOOL bOn ,E_ROBOT Robot)
{
	if (Robot == E_LD_ROBOT)
		theUnitFunc.SetOutPutIO(Y_LD_ROBOT_ONLINE,bOn);
	else
		theUnitFunc.SetOutPutIO(Y_ULD_ROBOT_ONLINE,bOn);
}

void CUnitCtrlBank::RobotSetServoOn( const BOOL bSet ,E_ROBOT Robot)
{
	if (Robot == E_LD_ROBOT)
		theUnitFunc.SetOutPutIO(Y_LD_ROBOT_SERVO_ON,bSet);
	else
		theUnitFunc.SetOutPutIO(Y_ULD_ROBOT_SERVO_ON,bSet);

	if(bSet) RobotSetRunable(TRUE,Robot);
}

void CUnitCtrlBank::RobotSetServoOff( const BOOL bSet ,E_ROBOT Robot)
{
	if (Robot == E_LD_ROBOT)
		theUnitFunc.SetOutPutIO(Y_LD_ROBOT_SERVO_OFF,bSet);
	else
		theUnitFunc.SetOutPutIO(Y_LD_ROBOT_SERVO_OFF,bSet);

	if(bSet) RobotSetRunable(TRUE,Robot);
}

void CUnitCtrlBank::RobotSetOrigin(const BOOL bOn,E_ROBOT Robot)
{
	// 원점복귀(초기위치 이동). 재생 신호 OFF되기 전에 ON 되었다가 OFF되어야 함
	if (Robot == E_LD_ROBOT)
		theUnitFunc.SetOutPutIO(Y_LD_ROBOT_ORIGIN,bOn);
	else
		theUnitFunc.SetOutPutIO(Y_ULD_ROBOT_ORIGIN,bOn);
}

void CUnitCtrlBank::RobotSetRunable(const BOOL bOn,E_ROBOT Robot)
{
	if (Robot == E_LD_ROBOT)
		theUnitFunc.SetOutPutIO(Y_LD_ROBOT_RUNABLE,bOn);
	else
		theUnitFunc.SetOutPutIO(Y_ULD_ROBOT_RUNABLE,bOn);
}

void CUnitCtrlBank::RobotSeqReset(const BOOL bOn,E_ROBOT Robot)
{
	// 외부 리셋. Robot EMS 정지 후 Robot Sequence Reset
	if (Robot == E_LD_ROBOT)
		theUnitFunc.SetOutPutIO(Y_LD_ROBOT_SEQ_RESET,bOn);
	else
		theUnitFunc.SetOutPutIO(Y_ULD_ROBOT_SEQ_RESET,bOn);
}

void CUnitCtrlBank::RobotSetSlowSpeed(const BOOL bOn,E_ROBOT Robot)
{
	// 고속/저속 전환. ON : 저속(30%). OFF : 고속(100%)
	if (Robot == E_LD_ROBOT)
		theUnitFunc.SetOutPutIO(Y_LD_ROBOT_SLOW_SPEED,bOn);
	else
		theUnitFunc.SetOutPutIO(Y_ULD_ROBOT_SLOW_SPEED,bOn);
}

void CUnitCtrlBank::RobotCellVacOn(const BOOL bVacOn,E_ROBOT Robot)
{
	if (Robot == E_LD_ROBOT)
	{
		theUnitFunc.SetOutPutIO(Y_LD_ROBOT_CELL_VAC_SOL,bVacOn);
//		theUnitFunc.SetOutPutIO(Y_LD_ROBOT_CELL_BLOW_SOL,bBlowOn);
	}
	else
	{
		theUnitFunc.SetOutPutIO(Y_ULD_ROBOT_CELL_VAC_SOL,bVacOn);
//		theUnitFunc.SetOutPutIO(Y_ULD_ROBOT_CELL_BLOW_SOL,bBlowOn);
	}
}

//void CUnitCtrlBank::RobotFpcbVacOn(const BOOL bVacOn, const BOOL bBlowOn,E_ROBOT Robot)
//{
//	if (Robot == E_LD_ROBOT)
//	{
//		theUnitFunc.SetOutPutIO(Y_LD_ROBOT_FPCB_VAC_SOL,bVacOn);
//		theUnitFunc.SetOutPutIO(Y_LD_ROBOT_FPCB_BLOW_SOL,bBlowOn);
//	}
//	else
//	{
//		theUnitFunc.SetOutPutIO(Y_ULD_ROBOT_FPCB_VAC_SOL,bVacOn);
//		theUnitFunc.SetOutPutIO(Y_ULD_ROBOT_FPCB_BLOW_SOL,bBlowOn);
//	}
//}

BOOL CUnitCtrlBank::RobotCellVacOnChk(E_ROBOT Robot)
{
	if (Robot == E_LD_ROBOT)
		return   theUnitFunc.GetInPutIOCheck(X_LD_ROBOT_CELL_VAC_SEN);
	else
		return   theUnitFunc.GetInPutIOCheck(X_ULD_ROBOT_CELL_VAC_SEN);
}

//BOOL CUnitCtrlBank::RobotFpcbVacOnChk(E_ROBOT Robot)
//{
//	if (Robot == E_LD_ROBOT)
//		return   theUnitFunc.GetInPutIOCheck(X_LD_ROBOT_FPCB_VAC_SEN);
//	else
//		return   theUnitFunc.GetInPutIOCheck(X_ULD_ROBOT_FPCB_VAC_SEN);
//}