#include "stdafx.h"
#include "MP2100.h"
#include "CheetahApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

DWORD CMp2100::m_dwOpenState = 0x00000000;
CMp2100::CMp2100()
{
	m_nBoardID = 0;

	m_hController = NULL;

	for(int ax=0; ax<MAX_AXIS_CNT_IN_BOARD; ax++)
	{
		m_hAxis[ax] = NULL;
		m_hDevice[ax] = NULL;
	}

	
/*
	double d = 3.234, d2;
	
	d *= 1000;
	LONG lVal = LONG(d);
	DWORD dwVal = -10023;

	lVal = LONG(dwVal);
	d = double(lVal/FLOAT_1000);
	d2 = double(lVal/INT_1000);

	TRACE(_T("%d\n", lVal);*/
}
CMp2100::~CMp2100()
{

}
//---------------------
// 주의 : 
//   1. CpuNumber는 1부터 시작 한다.
//
DWORD CMp2100::OpenController(const int nCpuNo)
{
	CString errMsg;

	DWORD					rc;
	COM_DEVICE	ComDevice;		// The ymcOpenController setting structure 

	m_nBoardID = nCpuNo;

	ComDevice.ComDeviceType = COMDEVICETYPE_PCI_MODE;
	ComDevice.PortNumber    = 1;
	ComDevice.CpuNumber     = m_nBoardID+1;// 범위 : 1~4
	ComDevice.NetworkNumber = NULL;
	ComDevice.StationNumber = NULL;
	ComDevice.UnitNumber    = NULL;
	ComDevice.IPAddress     = NULL;
	ComDevice.Timeout       = 5000;

	rc = ::ymcOpenController(&ComDevice, &m_hController);
	if(rc != MP_SUCCESS)
	{
		errMsg.Format(_T("ymcOpenController ErrorCode[%d: 0x%x ]"), m_nBoardID, rc);
		theLog[LOG_MP2100].AddBuf(errMsg);
// 		theErrorMgr.ErrorHappen(CONST_ERR::Mp2100_OpenController);
		return rc;
	}
	TRACE(_T("m_hController=%X\n"), m_hController);

	rc = ymcSetAPITimeoutValue(30000);
	if(rc != MP_SUCCESS)
	{
		errMsg.Format(_T("ymcSetAPITimeoutValue ErrorCode[%d: 0x%x ]"), m_nBoardID, rc);
 		theLog[LOG_MP2100].AddBuf(errMsg);
// 		theErrorMgr.ErrorHappen(CONST_ERR::Mp2100_OpenController);
		return rc;
	}

	rc = ::ymcSetController(m_hController);
	if(rc != MP_SUCCESS)
	{
		errMsg.Format(_T("ymcSetController ErrorCode[%d: 0x%x ]"), m_nBoardID, rc);
 		theLog[LOG_MP2100].AddBuf(errMsg);
// 		theErrorMgr.ErrorHappen(CONST_ERR::Mp2100_OpenController);
		return FALSE;
	}	
	
	rc	= ::ymcClearAllAxes();
	if(rc != MP_SUCCESS)
	{
		errMsg.Format(_T("ymcClearAllAxes ErrorCode[%d: 0x%x ]"), m_nBoardID, rc);
 		theLog[LOG_MP2100].AddBuf(errMsg);
// 		theErrorMgr.ErrorHappen(CONST_ERR::Mp2100_OpenController);
		return rc;
	}

	//rc = SelectBoard();
	//if(rc != MP_SUCCESS)	
	//	return rc;


	CString strAxisName;
	char cText[256];
	for(int ax=0; ax<MAX_AXIS_CNT_IN_BOARD; ax++)
	{
		strAxisName.Format(_T("Axis%d-%02d"), m_nBoardID+1, ax + 1);
		CEtc::ConvertMultibyte((wchar_t*)strAxisName.GetString(), cText, 256);
		rc	= ymcDeclareAxis( 1, 
			((ax / 16) % 2) == 0 ? 0 : 1, 
			((ax / 16) % 2) == 0 ? 3 : 1, 
			(ax - (ax / 16) * 16) + 1, 
			ax + 1, 
			REAL_AXIS, 
			(LPBYTE)cText,
			&m_hAxis[ax]); 
		if(rc != MP_SUCCESS)
		{
			errMsg.Format(_T("ymcDeclareAxis ErrorCode[%d: 0x%x ]"), m_nBoardID, rc);
			theLog[LOG_MP2100].AddBuf(errMsg);
			// 			theErrorMgr.ErrorHappen(CONST_ERR::Mp2100_OpenController);
			return rc;
		}
		rc	= ymcDeclareDevice(1, &m_hAxis[ax], &m_hDevice[ax]);
		if(rc != MP_SUCCESS)
		{
			errMsg.Format(_T("ymcDeclareDevice ErrorCode[%d: 0x%x ]"), m_nBoardID, rc);
 			theLog[LOG_MP2100].AddBuf(errMsg);
// 			theErrorMgr.ErrorHappen(CONST_ERR::Mp2100_OpenController);
			return rc;
		}
		TRACE(_T("m_hAxis=%X,%X \n"), m_hAxis[ax], m_hDevice[ax]);
	}

	for(int ax=0; ax<MAX_AXIS_CNT_IN_BOARD; ax++)
	{
		if(ax<MAX_AXIS_CNT_IN_BOARD/2)	m_lAxisAddr[ax] = 0x8000 + (ax * 0x80);
		else					m_lAxisAddr[ax] = 0x9000 + (ax * 0x80);
	}


	return MP_SUCCESS;
}
BOOL CMp2100::CloseController()
{
	if(m_dwOpenState==0)
		return TRUE;

	DWORD	rc;
	CString errMsg;

	if(!SelectBoard())
		return FALSE;

	for(int ax=0; ax<MAX_AXIS_CNT_IN_BOARD; ax++)
	{
		if(m_hDevice[ax]==NULL)
			continue;

		rc	= ::ymcClearDevice(m_hDevice[ax]);
		if(rc != MP_SUCCESS)
		{
			errMsg.Format(_T("ymcClearDevice ErrorCode[%d: 0x%x ]"), m_nBoardID, rc);
 			theLog[LOG_MP2100].AddBuf(errMsg);
// 			theErrorMgr.ErrorHappen(CONST_ERR::Mp2100_CloseController);
			return rc;
		}
		m_hDevice[ax] = NULL;
	}

	if(m_hController != NULL)
	{
		rc = ::ymcCloseController(m_hController);
		if(rc != MP_SUCCESS)
		{
			errMsg.Format(_T("ymcCloseController ErrorCode[%d: 0x%x ]"), m_nBoardID, rc);
 			theLog[LOG_MP2100].AddBuf(errMsg);
// 			theErrorMgr.ErrorHappen(CONST_ERR::Mp2100_CloseController);
			return rc;
		}
		m_hController = NULL;
	}

	return TRUE;
}

BOOL CMp2100::SelectBoard()
{
	if(CMp2100::m_dwOpenState == 0)
		return FALSE;


	CString errMsg;
	DWORD					rc;
	
	rc = ::ymcSetController(m_hController);
	if(rc != MP_SUCCESS)
	{
		errMsg.Format(_T("ymcSetController ErrorCode[%d: 0x%x ]"), m_nBoardID, rc);
 		theLog[LOG_MP2100].AddBuf(errMsg);
// 		theErrorMgr.ErrorHappen(CONST_ERR::Mp2100_OpenController);
		return FALSE;
	}
	
	return TRUE;
}

//==============================================================================================================
// Monitor Param Read
//
BOOL CMp2100::ReadMonitorParam(int nAxisNo, int nOffset, DWORD& dwVal)
{
	if(!SelectBoard())
		return FALSE;	

	if(MP_SUCCESS!=::ymcGetMotionParameterValue(m_hAxis[nAxisNo], MONITOR_PARAMETER, nOffset, &dwVal))
		return FALSE;
	return TRUE;
}
BOOL CMp2100::ReadSettingParam(int nAxisNo, int nOffset, DWORD& dwVal)
{
	if(!SelectBoard())
		return FALSE;	

	if(MP_SUCCESS!=::ymcGetMotionParameterValue(m_hAxis[nAxisNo], SETTING_PARAMETER, nOffset, &dwVal))
		return FALSE;
	return TRUE;
}
BOOL CMp2100::WriteSettingParam(int nAxisNo, int nOffset, DWORD dwVal)
{
	if(SelectBoard() == FALSE)
		return FALSE;	

	if(MP_SUCCESS!=::ymcSetMotionParameterValue(m_hAxis[nAxisNo], SETTING_PARAMETER, nOffset, dwVal))
		return FALSE;
	return TRUE;
}

BOOL CMp2100::GetParam_DriveStatus(int nAxisNo, WORD& wVal)
{
	if(!SelectBoard())
		return FALSE;	

	DWORD dwVal;
	DWORD dRtn = ::ymcGetMotionParameterValue(m_hAxis[nAxisNo], MONITOR_PARAMETER, DRIVE_STATUS, &dwVal);
	if(MP_SUCCESS!=dRtn)
		return FALSE;
	wVal = (WORD)(dwVal);
	return TRUE;
	//return ServoReadMoniW(m_hAxis[nAxisNo], DRIVE_STATUS, wVal);
}
BOOL CMp2100::GetParam_Warning(int nAxisNo, LONG& lVal)
{
	if(!SelectBoard())
		return FALSE;	

	DWORD dwVal;
	if(MP_SUCCESS!=::ymcGetMotionParameterValue(m_hAxis[nAxisNo], MONITOR_PARAMETER, WARNING, &dwVal))
		return FALSE;
	lVal = (LONG)dwVal;
	return TRUE;
}
BOOL CMp2100::GetParam_Alarm(int nAxisNo, LONG& lVal)
{
	if(!SelectBoard())
		return FALSE;	

	DWORD dwVal;

	DWORD dwRtnVal = ::ymcGetMotionParameterValue(m_hAxis[nAxisNo], MONITOR_PARAMETER, ALARM, &dwVal);
	if(MP_SUCCESS!=dwRtnVal)
		return FALSE;
	lVal = (LONG)dwVal;
	return TRUE;
}
BOOL CMp2100::GetParam_CommandType(int nAxisNo, WORD& wVal)
{
	if(!SelectBoard())
		return FALSE;	

	DWORD dwVal;
	if(MP_SUCCESS!=::ymcGetMotionParameterValue(m_hAxis[nAxisNo], MONITOR_PARAMETER, SERVO_CMD_TYPE_RESPONSE, &dwVal))
		return FALSE;
	wVal = (WORD)(dwVal);
	return TRUE;
}
BOOL CMp2100::GetParam_CommandStatus(int nAxisNo, WORD& wVal)
{
	if(!SelectBoard())
		return FALSE;	

	DWORD dwVal;
	if(MP_SUCCESS!=::ymcGetMotionParameterValue(m_hAxis[nAxisNo], MONITOR_PARAMETER, SERVO_MODULE_CMD_STATUS, &dwVal))
		return FALSE;
	wVal = (WORD)(dwVal);
	return TRUE;
}

BOOL CMp2100::GetParam_PositionStatus(int nAxisNo, WORD& wVal)
{
	if(!SelectBoard())
		return FALSE;	

	DWORD dwVal;
	if(MP_SUCCESS!=::ymcGetMotionParameterValue(m_hAxis[nAxisNo], MONITOR_PARAMETER, POSITION_MANAGEMENT_STATUS, &dwVal))
		return FALSE;
	wVal = (WORD)(dwVal);
	return TRUE;
}

BOOL CMp2100::GetParam_TargetPos(int nAxisNo, double& dVal)
{
	if(!SelectBoard())
		return FALSE;	

	DWORD dwVal;
	if(MP_SUCCESS!=::ymcGetMotionParameterValue(m_hAxis[nAxisNo], MONITOR_PARAMETER, CPOS, &dwVal))
		return FALSE;
	dVal = double(LONG(dwVal)/FLOAT_1000);
	return TRUE;
}

BOOL CMp2100::GetParam_FeedbackPos(int nAxisNo, double& dVal)
{
	if(!SelectBoard())
		return FALSE;	

	DWORD dwVal;
	if(MP_SUCCESS!=::ymcGetMotionParameterValue(m_hAxis[nAxisNo], MONITOR_PARAMETER, APOS, &dwVal))
		return FALSE;
	dVal = double(LONG(dwVal)/FLOAT_1000);
	return TRUE;
}

//SONIC - 2013.06.20.
BOOL CMp2100::GetParam_TorqueChk(int nAxisNo, double& dVal)
{
	if(!SelectBoard())
		return FALSE;	

	DWORD dwVal;
	if(MP_SUCCESS!=::ymcGetMotionParameterValue(m_hAxis[nAxisNo], MONITOR_PARAMETER, TORQUE_MONITOR, &dwVal))
		return FALSE;
	dVal = double(LONG(dwVal)/FLOAT_100);
	return TRUE;
}

BOOL CMp2100::GetParam_ServoAlarmCode(int nAxisNo, WORD& wVal)
{
	if(!SelectBoard())
		return FALSE;	

	DWORD dwVal;
	if(MP_SUCCESS!=::ymcGetMotionParameterValue(m_hAxis[nAxisNo], MONITOR_PARAMETER, SERVO_ALARM_CODE, &dwVal))
		return FALSE;
	wVal = (WORD)(dwVal);
	return TRUE;
}

BOOL CMp2100::GetParam_ServoIO(int nAxisNo, WORD& wVal)
{
	if(!SelectBoard())
		return FALSE;	

	DWORD dwVal;
	if(MP_SUCCESS!=::ymcGetMotionParameterValue(m_hAxis[nAxisNo], MONITOR_PARAMETER, NETWORK_SERVO_IO_MONITOR, &dwVal))
		return FALSE;
	wVal = (WORD)(dwVal);
	return TRUE;
}

BOOL CMp2100::GetParamBit_ServoON(int nAxisNo)
{
	WORD	wVal;
	if(!GetParam_DriveStatus(nAxisNo, wVal))
		return FALSE;

	return (wVal>>DRIVE_STATUS_RUNNING)&0x0001;
}

BOOL CMp2100::GetParamBit_CommandReady(int nAxisNo)
{
	WORD	wVal;
	if(!GetParam_CommandStatus(nAxisNo, wVal))
		return FALSE;

	return (~(wVal>>SERVO_MODULE_COMMAND_STATUS_COMMAND_EXECUTING))&0x0001;
}

BOOL CMp2100::GetParamBit_ZRNC(int nAxisNo)
{
	WORD	wVal;
	if(!GetParam_PositionStatus(nAxisNo, wVal))
		return FALSE;

	return (wVal>>POSITION_MANAGEMENT_STATUS_ZERO_POINT_RETURN_COMPLETED)&0x0001;
}



BOOL CMp2100::IsAxisReady(int nAxisNo)
{
	LONG lVal;
	WORD wVal;

	if(!GetParam_Alarm(nAxisNo, lVal) || lVal!=0)
		return FALSE;
	if(!GetParamBit_ServoON(nAxisNo))
		return FALSE;
	if(!GetParamBit_CommandReady(nAxisNo))
		return FALSE;
	if(!GetParam_CommandType(nAxisNo, wVal) || wVal!=0)
		return FALSE;

	return TRUE;
}
/*
BOOL CMp2100::IsAxisMoveReady(int nAxisNo)
{
	if(!IsAxisReady(nAxisNo))
		return FALSE;

	if(!GetParamBit_ZRNC(nAxisNo))
		return FALSE;

	return TRUE;
}*/
BOOL CMp2100::CheckTrouble(int nAxisNo)
{
	// Return Value
	// TRUE = Trouble
	// FALSE = Not Trouble

	WORD wVal;
	LONG lVal;

	GetParam_ServoIO(nAxisNo, wVal);
	if((wVal>>NETWORK_SERVO_IO_MONITOR_POT)&0x01)	return TRUE;
	if((wVal>>NETWORK_SERVO_IO_MONITOR_NOT)&0x01)	return TRUE;

	GetParam_PositionStatus(nAxisNo, wVal);
	if(((wVal>>POSITION_MANAGEMENT_STATUS_ZERO_POINT_RETURN_COMPLETED)&0x01)!=0x01)	return TRUE;

	GetParam_Alarm(nAxisNo, lVal);
	if(lVal!=0)	return TRUE;

	if(!GetParamBit_ServoON(nAxisNo))	return TRUE;

	return FALSE;
}

BOOL CMp2100::ServoON(int nAxisNo)
{
	if(!SelectBoard())
		return FALSE;

	if(MP_SUCCESS != ::ymcServoControl(m_hDevice[nAxisNo], SERVO_ON, 5000))
		return FALSE;

	return TRUE;
}
BOOL CMp2100::ServoOFF(int nAxisNo)
{
	if(!SelectBoard())
		return FALSE;

	if(MP_SUCCESS != ::ymcServoControl(m_hDevice[nAxisNo], SERVO_OFF, 5000))
		return FALSE;

	return TRUE;
}

BOOL CMp2100::AlarmClear(int nAxisNo)
{
	if(!SelectBoard())
		return FALSE;

	if(MP_SUCCESS != ::ymcClearServoAlarm(m_hAxis[nAxisNo]))
		return FALSE;

	return TRUE;
}

BOOL CMp2100::BoardAlarmClear()
{
	if(!SelectBoard())
		return FALSE;

	if(MP_SUCCESS != ::ymcClearAlarm(0))
		return FALSE;

	return TRUE;
}

BOOL CMp2100::ZeroPointSet(int nAxisNo, double dPos)
{
	if(!SelectBoard())
		return FALSE;

	if(MP_SUCCESS != ::ymcClearAlarm(0))
		return FALSE;

	POSITION_DATA   PositionData;
	PositionData.DataType		= DATATYPE_IMMEDIATE;
	PositionData.PositionData	= (LONG)(dPos * INT_1000);

	if(MP_SUCCESS != ::ymcDefinePosition(m_hDevice[nAxisNo], &PositionData))
		return FALSE;

	return TRUE;
}

BOOL CMp2100::ZeroPointReturn(int nAxisNo, double dHomeSpeed, double dAccType /*= ACC_300ms*/, int nOriginMode /*= HMETHOD_HOMELS_ONLY*/)
{
	if(!SelectBoard())
		return FALSE;	

	if(!IsAxisReady(nAxisNo))
		return FALSE;

	MOTION_DATA     MotionData;        
	POSITION_DATA   PositionData;               
	WORD            WaitForCompletion;	
	WORD			HomeMethod;
	WORD			Direction;

	HomeMethod					= nOriginMode;
	if(HomeMethod == HMETHOD_NOT_C)
	{
		MotionData.ApproachVelocity = (LONG)(dHomeSpeed * INT_1000 * -1);
	}

	Direction					= DIRECTION_NEGATIVE;

	MotionData.VelocityType		= VTYPE_UNIT_PAR;	
	MotionData.AccDecType		= ATYPE_UNIT_PAR;   
	MotionData.DataType			= 0;    

	MotionData.Acceleration		= (LONG)(dHomeSpeed * INT_1000 * LONG(dAccType));
	MotionData.Deceleration		= (LONG)(dHomeSpeed * INT_1000 * LONG(dAccType));	
	
	MotionData.Velocity			= (LONG)(dHomeSpeed * INT_1000);
	MotionData.CreepVelocity	= (LONG)(dHomeSpeed * INT_1000 * -1);

	PositionData.DataType		= DATATYPE_IMMEDIATE;
	PositionData.PositionData	= (LONG)(HOME_FINAL_MOVING * INT_1000);

	WaitForCompletion			= COMMAND_STARTED;

	if(MP_SUCCESS != ::ymcMoveHomePosition(m_hDevice[nAxisNo], &MotionData, &PositionData, &HomeMethod, &Direction, NULL, (LPBYTE)"Start", &WaitForCompletion, 0))
		return FALSE;
	return TRUE;
}
BOOL CMp2100::AxisMove( int nAxisNo, double dPos, double dVel, SHORT sMoveType, double dAccType /*= ACC_300ms*/ )
{
	if(!SelectBoard())
	{
		return FALSE;	
	}

	if(!IsAxisReady(nAxisNo))
	{
// 		GetLogCtrl()->AddBuf(LOG_MOTOR_ERROR, _T("MP2100 Board %d Axis %d is Not IsAxisReady"), m_nBoardID, nAxisNo);
		return FALSE;
	}

	MOTION_DATA     MotionData;        
	POSITION_DATA   PositionData;               
	WORD            WaitForCompletion;	

	PositionData.DataType		= DATATYPE_IMMEDIATE;
	PositionData.PositionData	= LONG(dPos * INT_1000);

	MotionData.CoordinateSystem	= WORK_SYSTEM;		
	MotionData.MoveType			= sMoveType;	
	MotionData.VelocityType		= VTYPE_UNIT_PAR;		
	MotionData.AccDecType		= ATYPE_UNIT_PAR;		
	MotionData.FilterType		= FTYPE_S_CURVE;		
	MotionData.DataType			= 0;					

	MotionData.Acceleration		= (LONG)(dVel * 1000. * dAccType);
	MotionData.Deceleration		= (LONG)(dVel * 1000. * dAccType);	

	MotionData.FilterTime		= 10;					
	MotionData.Velocity			= LONG(dVel * INT_1000);			

	WaitForCompletion			= COMMAND_STARTED;

// 	DWORD dwRtn = ::ymcChangeDynamics(m_hDevice[nAxisNo], &MotionData, &PositionData, SUBJECT_ACC|SUBJECT_DEC|SUBJECT_POS|SUBJECT_VEL, (LPBYTE)"Start", 0);
	DWORD dwRtn = ::ymcMoveDriverPositioning(m_hDevice[nAxisNo], &MotionData, &PositionData, NULL, (LPBYTE)"Start", &WaitForCompletion, 0);
	if(MP_SUCCESS != dwRtn)
	{
 		theLog[LOG_MP2100].AddBuf(_T("ymcMoveDriverPositioning ErrorCode[%d: 0x%x ]"), m_nBoardID, dwRtn);
		return FALSE;
	} 

	return TRUE;
}

BOOL CMp2100::AxisJog( int nAxisNo, double dVel, BOOL bForward /*= TRUE*/, double dAccType /*= ACC_300ms*/ )
{
	if(!SelectBoard())
	{
		return FALSE;	
	}

	if(!IsAxisReady(nAxisNo))
	{
// 		GetLogCtrl()->AddBuf(LOG_MOTOR_ERROR, _T("MP2100 Board %d Axis %d is Not IsAxisReady"), m_nBoardID, nAxisNo);
		return FALSE;
	}

	MOTION_DATA     MotionData;        
	WORD            Direction = bForward ? DIRECTION_POSITIVE:DIRECTION_NEGATIVE;	

	MotionData.CoordinateSystem	= WORK_SYSTEM;		
	MotionData.MoveType			= MTYPE_RELATIVE;	
	MotionData.VelocityType		= VTYPE_UNIT_PAR;		
	MotionData.AccDecType		= ATYPE_UNIT_PAR;		
	MotionData.FilterType		= FTYPE_S_CURVE;		
	MotionData.DataType			= 0;					

	MotionData.Acceleration		= (LONG)(dVel * 1000. * dAccType);
	MotionData.Deceleration		= (LONG)(dVel * 1000. * dAccType);	

	MotionData.FilterTime		= 10;					
	MotionData.Velocity			= LONG(dVel * INT_1000);			



	DWORD dwRtn = ::ymcMoveJOG(m_hDevice[nAxisNo], &MotionData, &Direction, NULL, 0, (LPBYTE)"Jog", 0);
	if(MP_SUCCESS != dwRtn)
	{
 		theLog[LOG_MP2100].AddBuf(_T("ymcMoveDriverPositioning ErrorCode[%d: 0x%x ]"), m_nBoardID, dwRtn);
		return FALSE;
	} 

	return TRUE;
}


BOOL CMp2100::AxisStop(int nAxisNo, LONG nDec)
{
	if(!SelectBoard())
		return FALSE;	

	MOTION_DATA     MotionData;        
	WORD            WaitForCompletion;	

	MotionData.AccDecType	= ATYPE_TIME;//ATYPE_UNIT_PAR;		
	MotionData.DataType		= 0;	
	//if(m_bInterlock == TRUE)
	//	MotionData.Deceleration = (LONG)(m_MotorParam[nAxisNo].fPositionSpeed * NUMBER_OF_DECIMAL * 8);
	//else

	MotionData.Deceleration = nDec;//LONG(dVel * INT_1000 * LONG(ACC_DEC_MULT));

// 	WaitForCompletion		= DISTRIBUTION_COMPLETED;//COMMAND_STARTED;
	WaitForCompletion		= COMMAND_STARTED;

	if(MP_SUCCESS!=::ymcStopMotion(m_hDevice[nAxisNo], &MotionData, NULL, &WaitForCompletion, 0))
	{
		//m_strErrMsg.Format("Error ymcStopMotion Board %d  \nErrorCode [ 0x%x ]", nCPUNo, rc);
		//::MessageBox(CSiriusApp::GetMaintenanceViewDlg(), m_strErrMsg, AfxGetApp()->m_pszAppName, MB_ICONERROR);
		return FALSE;
	} 
	return TRUE;
}

BOOL CMp2100::AxisStopJog( int nAxisNo, double dVel )
{
	if(!SelectBoard())
		return FALSE;	

	MOTION_DATA     MotionData;        
	WORD            WaitForCompletion;	

	MotionData.AccDecType	= ATYPE_TIME;//ATYPE_UNIT_PAR;		
	MotionData.DataType		= 0;	

	MotionData.Deceleration = 500;//LONG(dVel * INT_1000 * LONG(ACC_DEC_MULT));

	WaitForCompletion		= DISTRIBUTION_COMPLETED;//COMMAND_STARTED;

	if(MP_SUCCESS!=::ymcStopJOG(m_hDevice[nAxisNo], 0, (LPBYTE)"StopJog", &WaitForCompletion, 0))
	{
		return FALSE;
	} 
	return TRUE;
}
BOOL CMp2100::CheckPLimit( int nAxisNo )
{
	WORD	wVal;
	GetParam_ServoIO(nAxisNo, wVal);
	return (wVal >> NETWORK_SERVO_IO_MONITOR_POT & 0x01) ? TRUE:FALSE;
}

BOOL CMp2100::CheckNLimit( int nAxisNo )
{
	WORD	wVal;
	GetParam_ServoIO(nAxisNo, wVal);
	return (wVal >> NETWORK_SERVO_IO_MONITOR_NOT & 0x01) ? TRUE:FALSE;
}

BOOL CMp2100::SetSyncAxis( int *pAxisArray, const int nAxisCount, int nMasterAxisIndex )
{
	int axisarray[MAX_AXIS_CNT_IN_BOARD];
	memcpy(axisarray, pAxisArray, nAxisCount*sizeof(int));
	HAXIS haxisarray[MAX_AXIS_CNT_IN_BOARD];
	for(int i = 0; i < nAxisCount; i++)
	{
		haxisarray[i] = m_hAxis[axisarray[i]];
	}
	DWORD rc = ymcDeclareDevice(nAxisCount,haxisarray, &m_hDevice[axisarray[nMasterAxisIndex]]);
	return rc == MP_SUCCESS ? TRUE:FALSE;
}
BOOL CMp2100::EnableGear(int wMasterAxisNo, int wSlaveAxisNo)
{	
	DWORD	dwRC = 0;
	char    cRegisterName[10] = {NULL};
	WORD	wWaitForCompletion = 0;

	HAXIS	hAxis = NULL;
	HDEVICE	hDevice = NULL;
	GEAR_RATIO_DATA	GearRatioData; 
	SYNC_DISTANCE	SyncDistance;        

	hDevice = m_hDevice[wMasterAxisNo];
	GearRatioData.Master = 1;
	GearRatioData.Slave = 1;
	dwRC = ymcSetGearRatio(hDevice, &GearRatioData, 0);
	if(dwRC != MP_SUCCESS) return dwRC;

	SyncDistance.SyncType     = SYNCH_TIME;
	SyncDistance.DataType     = DTYPE_IMMEDIATE;
	SyncDistance.DistanceData = 0;

	wWaitForCompletion        = GEAR_COMMAND_STARTED;
	hAxis = m_hAxis[wMasterAxisNo];
	hDevice = m_hDevice[wSlaveAxisNo];

	sprintf_s(cRegisterName, (LPSTR)(LPCTSTR)"MW1000");
	HREGISTERDATA	hReg;
	dwRC = ymcGetRegisterDataHandle((LPBYTE)cRegisterName, &hReg);
	if(dwRC != MP_SUCCESS) return dwRC;

	dwRC = ymcEnableGear(hAxis, hDevice, MASTER_AXIS_FEEDBACK, &SyncDistance, &hReg, NULL, &wWaitForCompletion, 0 );
	//dwRC = ymcEnableGear(hAxis, hDevice, MASTER_AXIS_COMMAND, &SyncDistance, &hReg, NULL, &wWaitForCompletion, 0 );
	if(dwRC != MP_SUCCESS) return dwRC;

	return MP_SUCCESS;
}
BOOL CMp2100::DisableGear(int wMasterAxisNo, int wSlaveAxisNo)
{	
	DWORD	dwRC = 0;
	char    cRegisterName[10] = {NULL};
	WORD	wWaitForCompletion = 0;

	HAXIS	hAxis = NULL;
	HDEVICE	hDevice = NULL;
// 	GEAR_RATIO_DATA	GearRatioData; 
	SYNC_DISTANCE	SyncDistance;        

	hDevice = m_hDevice[wMasterAxisNo];

	SyncDistance.SyncType     = SYNCH_TIME;
	SyncDistance.DataType     = DTYPE_IMMEDIATE;
	SyncDistance.DistanceData = 0;

	wWaitForCompletion         = GEAR_COMMAND_STARTED;

	hAxis = m_hAxis[wMasterAxisNo];
	hDevice = m_hDevice[wSlaveAxisNo];

	dwRC = ymcDisableGear(hAxis, hDevice, MASTER_AXIS_FEEDBACK, &SyncDistance, NULL, &wWaitForCompletion, 0 );
	//dwRC = ymcDisableGear(hAxis, hDevice, MASTER_AXIS_COMMAND, &SyncDistance, NULL, &wWaitForCompletion, 0 );
	if(dwRC != MP_SUCCESS) return dwRC;

	return MP_SUCCESS;
}

BOOL CMp2100::GetSyncAxisStatus()
{
	HREGISTERDATA		hRegister_MW;                	// Register data handle for ML register
	//BYTE             	cRegisterName_MW[10] = "MW1001"; 	// MW register name storage variable
	DWORD            	RegisterDataNumber;          	// Number of read-in registers
	DWORD            	ReadDataNumber;              	// Number of obtained registers
	short             	Reg_ShortData[3];             	// W or B size register data storage variable
	DWORD            	dwRC;                          		// Motion API return value
	char				cRegisterName_MW[10] = {NULL};

	sprintf_s(cRegisterName_MW, (LPSTR)(LPCTSTR)"MW1001");
	dwRC = ymcGetRegisterDataHandle( (LPBYTE)cRegisterName_MW, &hRegister_MW);
	if(dwRC != MP_SUCCESS) return dwRC;
	RegisterDataNumber = 1;		
	dwRC = ymcGetRegisterData( hRegister_MW, RegisterDataNumber, Reg_ShortData, &ReadDataNumber );
	if(dwRC != MP_SUCCESS) return dwRC;
	short sReg = Reg_ShortData[0];
	if(sReg == 0) // 동기화 끊어짐
		return FALSE;
	else
		return TRUE;
}
