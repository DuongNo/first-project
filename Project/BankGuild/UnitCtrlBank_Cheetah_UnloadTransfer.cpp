#include "StdAfx.h"
#include "UnitCtrlBank.h"
#include "CheetahApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int CUnitCtrlBank::UnloadTRVacChk(BOOL bVacOn, BOOL bCH1, BOOL bCH2)
{
	if(theConfigBank.m_Option.m_bUseDryRun)
		return TRUE;

	if(bCH1 && bCH2)
	{
		if(bVacOn)
		{
			if(GetInPutIOCheck(X_ULD_TR_CH1_VAC_SEN) && GetInPutIOCheck(X_ULD_TR_CH2_VAC_SEN))
				return TRUE;
			else
				return FALSE;
		}
		else
		{
			if( !GetInPutIOCheck(X_ULD_TR_CH1_VAC_SEN) && !GetInPutIOCheck(X_ULD_TR_CH2_VAC_SEN) )
				return TRUE;
			else
				return FALSE;
		}
	}
	else if(bCH1)
	{
		if(bVacOn)
		{
			if(GetInPutIOCheck(X_ULD_TR_CH1_VAC_SEN))
				return TRUE;
			else
				return FALSE;
		}
		else
		{
			if( !GetInPutIOCheck(X_ULD_TR_CH1_VAC_SEN) )
				return TRUE;
			else
				return FALSE;
		}
	}
	else if(bCH2)
	{
		if(bVacOn)
		{
			if(GetInPutIOCheck(X_ULD_TR_CH2_VAC_SEN))
				return TRUE;
			else
				return FALSE;
		}
		else
		{
			if( !GetInPutIOCheck(X_ULD_TR_CH2_VAC_SEN) )
				return TRUE;
			else
				return FALSE;
		}
	}

	return TRUE;
}

void CUnitCtrlBank::UnloadTRVacOnOff(JIG_CH nCh ,BOOL bon)
{
	if(nCh == JIG_CH_MAX)
	{
		SetOutPutIO(Y_ULD_TR_CH1_CELL_VAC_ON_SOL,bon);
		SetOutPutIO(Y_ULD_TR_CH1_CELL_BLOW_ON_SOL,!bon);
		SetOutPutIO(Y_ULD_TR_CH2_CELL_VAC_ON_SOL,bon);
		SetOutPutIO(Y_ULD_TR_CH2_CELL_BLOW_ON_SOL,!bon);
	}
	else if(nCh == JIG_CH_1)
	{
		SetOutPutIO(Y_ULD_TR_CH1_CELL_VAC_ON_SOL,bon);
		SetOutPutIO(Y_ULD_TR_CH1_CELL_BLOW_ON_SOL,!bon);		
	}
	else
	{	
		SetOutPutIO(Y_ULD_TR_CH2_CELL_VAC_ON_SOL,bon);
		SetOutPutIO(Y_ULD_TR_CH2_CELL_BLOW_ON_SOL,!bon);		
	}
}

void CUnitCtrlBank::UnloadTR_BlowOff(JIG_CH nCh)
{
	if(nCh == JIG_CH_MAX)
	{
		SetOutPutIO(Y_ULD_TR_CH1_CELL_BLOW_ON_SOL,FALSE);
		SetOutPutIO(Y_ULD_TR_CH2_CELL_BLOW_ON_SOL,FALSE);
	}
	else if(nCh == JIG_CH_1)
	{
		SetOutPutIO(Y_ULD_TR_CH1_CELL_BLOW_ON_SOL,FALSE);
	}
	else
	{
		SetOutPutIO(Y_ULD_TR_CH2_CELL_BLOW_ON_SOL,FALSE);
	}
}

void CUnitCtrlBank::MoveUnloadTR_X_GetPos()
{
	theDeviceMotion.TeachMove(m_nThreadID, TEACH_PARAM::ULD_TR_X_GET);
}

void CUnitCtrlBank::MoveUnloadTR_X_PutPos()
{
	theDeviceMotion.TeachMove(m_nThreadID, TEACH_PARAM::ULD_TR_X_PUT);
}

void CUnitCtrlBank::MoveUnLoadTR_Z_GetPos()
{
	theDeviceMotion.TeachMove(m_nThreadID, TEACH_PARAM::ULD_TR_Z_GET);
}

void CUnitCtrlBank::MoveUnLoadTR_Z_PutPos()
{
		theDeviceMotion.TeachMove(m_nThreadID, TEACH_PARAM::ULD_TR_Z_PUT);
}

void CUnitCtrlBank::MoveUnLoadTR_ZAxis_Up(double dSpeed)
{
	theDeviceMotion.TeachMove(m_nThreadID, TEACH_PARAM::ULD_TR_Z_UP, dSpeed);
}

BOOL CUnitCtrlBank::MoveUnloadTR_X_GetPosChk()
{
	BOOL bRet = FALSE; 

	if(theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::ULD_TR_X_GET) == TRUE)
		bRet = TRUE;

	return bRet;
}

BOOL CUnitCtrlBank::MoveUnloadTR_X_PutPosChk()
{
	BOOL bRet = FALSE; 

	if(theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::ULD_TR_X_PUT) == TRUE)
		bRet = TRUE;

	return bRet;
}

BOOL CUnitCtrlBank::MoveUnLoadTR_Z_GetPosChk()
{
	BOOL bRet = FALSE; 

	if(theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::ULD_TR_Z_GET) == TRUE)
		bRet = TRUE;

	return bRet;
}

BOOL CUnitCtrlBank::MoveUnLoadTR_Z_PutPosChk()
{
	BOOL bRet = FALSE; 

	if(theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::ULD_TR_Z_PUT) == TRUE)
		bRet = TRUE;

	return bRet;
}

BOOL CUnitCtrlBank::MoveUnLoadTR_ZAxis_UpChk()
{
	BOOL bRet = FALSE; 

	if(theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::ULD_TR_Z_UP) == TRUE)
		bRet = TRUE;

	return bRet;
}

void CUnitCtrlBank::ULDBuffSTGVacOnOff(BOOL bVacOn, BOOL bCH1, BOOL bCH2)
{
	if(bCH1)
	{
		SetOutPutIO(Y_ULD_STAGE_CH1_VAC_ON_SOL, bVacOn);
		SetOutPutIO(Y_ULD_STAGE_CH1_BLOW_ON_SOL, !bVacOn);

		if (bVacOn)
		{
			theDeviceMelsec.RobotBitOn(UDROBOT_XY_STGCH1VACON);
			theDeviceMelsec.RobotBitOff(UDROBOT_XY_STGCH1VACOFF);
		}
		else
		{
			theDeviceMelsec.RobotBitOn(UDROBOT_XY_STGCH1VACOFF);
			theDeviceMelsec.RobotBitOff(UDROBOT_XY_STGCH1VACON);
		}
	}

	if(bCH2)
	{
		SetOutPutIO(Y_ULD_STAGE_CH2_VAC_ON_SOL, bVacOn);
		SetOutPutIO(Y_ULD_STAGE_CH2_BLOW_ON_SOL, !bVacOn);

		if (bVacOn)
		{
			theDeviceMelsec.RobotBitOn(UDROBOT_Y_STG2VACON);
			theDeviceMelsec.RobotBitOff(UDROBOT_X_STG1GETERR_Y_STG2VACOFF);
		}
		else
		{
			theDeviceMelsec.RobotBitOn(UDROBOT_X_STG1GETERR_Y_STG2VACOFF);
			theDeviceMelsec.RobotBitOff(UDROBOT_Y_STG2VACON);
		}
	}
}

void CUnitCtrlBank::ULDBuffSTG_BlowOff(BOOL bCH1, BOOL bCH2)
{
	if(bCH1)
	{
		SetOutPutIO(Y_ULD_STAGE_CH1_BLOW_ON_SOL, FALSE);
	}

	if(bCH2)
	{
		SetOutPutIO(Y_ULD_STAGE_CH2_BLOW_ON_SOL, FALSE);
	}
}

BOOL CUnitCtrlBank::ULDBuffSTGVacChk(BOOL bVacOn, BOOL bCH1, BOOL bCH2)
{
	if(theConfigBank.m_Option.m_bUseDryRun)
		return TRUE;

	if(bCH1 && bCH2)
	{
		if(bVacOn)
		{
			if(GetInPutIOCheck(X_ULD_STAGE_CH1_VAC_SEN) && GetInPutIOCheck(X_ULD_STAGE_CH2_VAC_SEN))
				return TRUE;
			else
				return FALSE;
		}
		else
		{
			if( !GetInPutIOCheck(X_ULD_STAGE_CH1_VAC_SEN) && !GetInPutIOCheck(X_ULD_STAGE_CH2_VAC_SEN) )
				return TRUE;
			else
				return FALSE;
		}
	}
	else if(bCH1)
	{
		if(bVacOn)
		{
			if(GetInPutIOCheck(X_ULD_STAGE_CH1_VAC_SEN))
				return TRUE;
			else
				return FALSE;
		}
		else
		{
			if( !GetInPutIOCheck(X_ULD_STAGE_CH1_VAC_SEN) )
				return TRUE;
			else
				return FALSE;
		}
	}
	else if(bCH2)
	{
		if(bVacOn)
		{
			if(GetInPutIOCheck(X_ULD_STAGE_CH2_VAC_SEN))
				return TRUE;
			else
				return FALSE;
		}
		else
		{
			if( !GetInPutIOCheck(X_ULD_STAGE_CH2_VAC_SEN) )
				return TRUE;
			else
				return FALSE;
		}
	}

	return TRUE;
}

void CUnitCtrlBank::UDRobotTrayCountBitOn(int x , int y)
{
	long write; 
	if (x == 1 && y == 1)
	{
		write = 17;
		theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
	}
	else if (x == 1 && y == 2)
	{
		write = 33;
		theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
	}
	else if (x == 1 && y == 3)
	{
		write = 49;
		theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
	}
	else if (x == 1 && y == 4)
	{
		write = 65;
		theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
	}
	else if (x == 2 && y == 1)
	{
		write = 18;
		theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
	}
	else if (x == 2 && y == 2)
	{
		write = 34;
		theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
	}
	else if (x == 2 && y == 3)
	{
		write = 50;
		theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
	}
	else if (x == 2 && y == 4)
	{
		write = 66;
		theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
	}
	else if (x == 3 && y == 1)
	{
		write = 19;
		theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
	}
	else if (x == 3 && y == 2)
	{
		write = 35;
		theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
	}
	else if (x == 3 && y == 3)
	{
		write = 51;
		theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
	}
	else if (x == 3 && y == 4)
	{
		write = 67;
		theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
	}
	else if (x == 4 && y == 1)
	{
		write = 20;
		theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
	}
	else if (x == 4 && y == 2)
	{
		write = 36;
		theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
	}
	else if (x == 4 && y == 3)
	{
		write = 52;
		theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
	}
	else if (x == 4 && y == 4)
	{
		write = 68;
		theDeviceMelsec.RobotWordSend(UDROBOT_TRAYCOUNT,&write,sizeof(long), FALSE);
	}
}


void CUnitCtrlBank::UnloadTactTime()
{
	if(theCellBank.GetCellTag(CELL_POS_UD_ROBOT).IsExist()) GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_UD_ROBOT)->cellTime.systmUnload_Out_Tack_Time);
}


void CUnitCtrlBank::MoveUnLoadTR_ZAxis_Up_Slow(double dbOffset)
{
	theDeviceMotion.TeachMove(m_nThreadID, TEACH_PARAM::ULD_TR_Z_GET, dbOffset);
}

BOOL CUnitCtrlBank::MoveUnLoadTR_ZAxis_Up_SlowChk(double dbOffset)
{
	BOOL bRet = FALSE; 

	if(theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::ULD_TR_Z_GET,0.1, POS_EQUAL, dbOffset) == TRUE)
		bRet = TRUE;

	return bRet;
}