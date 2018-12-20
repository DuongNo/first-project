#include "StdAfx.h"
#include "UnitCtrlBank.h"
#include "CheetahApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CUnitCtrlBank::ULDBuffStageRobotPos(int nPos)
{
	theSocketInterFace.SendMsgToRobot(E_UD_ROBOT, (nPos == ULD_BUFF_STG_CH1) ? ROBOT_MOVE_LTLD : ROBOT_MOVE_RTLD);
}

BOOL CUnitCtrlBank::ULDBuffStageRobotPosChk(int nPos)
{
	if(RobotAlarmChk(E_UD_ROBOT))
		return FALSE;

	return   GetInPutIOCheck(X_ULD_ROBOT_CH1_LOADING_POS_CHK);
}
