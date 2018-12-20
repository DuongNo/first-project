#include "StdAfx.h"
#include "DeviceMotion.h"
#include "CheetahApp.h"

#include "UI\GausGUI\GxMsgBox.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void AxisMoveParam::operator=( const TEACH_PARAM &srcParam )
{
	idAxis = srcParam.idAxis;
	dTargetPosition = srcParam.dPos;
	dSpeed = srcParam.dSpeed;
	dAcc = srcParam.dAcc;
	dDec = srcParam.dDec;
}

CDeviceMotion::CDeviceMotion(void)
{
	m_bTurnTableHomeFinish = TRUE;
}

CDeviceMotion::~CDeviceMotion(void)
{
}

BOOL CDeviceMotion::Open_Mp2100( MP2100_THREAD thr )
{
	for(int board=0; board<MP2100_BOARD_MAX; board++)
	{
		if(m_Mp2100[thr][board].OpenController(board)!=MP_SUCCESS)
			return FALSE;
		CMp2100::m_dwOpenState |= 0x1<<thr*board; 
	}

	return TRUE;
}

void CDeviceMotion::Close_Mp2100()
{
	if(CMp2100::m_dwOpenState!=0)
	{
		//for(int th=0; th<MP2100_THREAD_MAX; th++)
		for(int th=0; th<1; th++)// Sample ������ 1���� close ���ش�.
		{
			for(int board=0; board<MP2100_BOARD_MAX; board++)
			{
				if(!m_Mp2100[th][board].CloseController())
					return;
			}
		}
		CMp2100::m_dwOpenState = 0;
	}

}

BOOL CDeviceMotion::Open_NMC2()
{
	for(int nCtrl=0; nCtrl<NMC_CTRL_MAX; nCtrl++)
	{
		if(!m_Nmc2[nCtrl].OpenController(nCtrl+1))
			return FALSE;
		CNmc2::m_dwOpenState |= 0x1<<nCtrl; 
	}

	return TRUE;
}

void CDeviceMotion::Close_NMC2()
{
	if(CNmc2::m_dwOpenState!=0)
	{
		for(int nCtrl=0; nCtrl<NMC_CTRL_MAX; nCtrl++)
		{
			if(!m_Nmc2[nCtrl].CloseController())
				return;
		}
		CNmc2::m_dwOpenState = 0;
	}
}

BOOL CDeviceMotion::ServoOn( MP2100_THREAD thr, AXIS_ID axis )
{
	if(theAxisBank.m_Axis[axis].m_bUse == FALSE)
	{
		if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
			theLog[LOG_MP2100].AddBuf(_T("%s Axis is Used"), theAxisBank.m_Axis[axis].m_strAxisName);
		else
			theLog[LOG_NMC].AddBuf(_T("%s Axis is Used"), theAxisBank.m_Axis[axis].m_strAxisName);
		return TRUE;
	}
	int nDevice = theAxisBank.m_Axis[axis].m_nModuleNo;
	int nInnerAxis = theAxisBank.m_Axis[axis].m_nInnerNo;
	if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
	{
		return m_Mp2100[thr][nDevice].ServoON(nInnerAxis);
	}
	else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
	{
		return m_Nmc2[nDevice].ServoON(nInnerAxis);
	}

	return FALSE;
}
BOOL CDeviceMotion::ServoOff(MP2100_THREAD thr, AXIS_ID axis)
{
	if(theAxisBank.m_Axis[axis].m_bUse == FALSE)
		return TRUE;

	BOOL bRtn = FALSE;
	int nDevice = theAxisBank.m_Axis[axis].m_nModuleNo;
	int nInnerAxis = theAxisBank.m_Axis[axis].m_nInnerNo;
	if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
	{
// 		if(MP_SUCCESS != m_Mp2100[thr][MP2100_BOARD_1].DisableGear(theAxisBank.m_Axis[AXIS_INSPECTION_Y1].m_nInnerNo,theAxisBank.m_Axis[AXIS_INSPECTION_Y2].m_nInnerNo))
// 			return FALSE;
		bRtn = m_Mp2100[thr][nDevice].ServoOFF(nInnerAxis);
	}
	else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
	{
		bRtn  = m_Nmc2[nDevice].ServoOFF(nInnerAxis);
		bRtn  = m_Nmc2[nDevice].AxisStop(nInnerAxis);
	}
	return bRtn;
}
BOOL CDeviceMotion::ResetAlarm(MP2100_THREAD thr, AXIS_ID axis)
{
	if(theAxisBank.m_Axis[axis].m_bUse == FALSE)
		return FALSE;

	BOOL bRtn = FALSE;
	int nDevice = theAxisBank.m_Axis[axis].m_nModuleNo;
	int nInnerAxis = theAxisBank.m_Axis[axis].m_nInnerNo;
	if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
	{
		bRtn = m_Mp2100[thr][nDevice].BoardAlarmClear();
		bRtn = m_Mp2100[thr][nDevice].AlarmClear(nInnerAxis);
	}
	else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
	{
		//		bRtn = TRUE;
		bRtn = m_Nmc2[nDevice].AlarmClear(nInnerAxis);
	}

	return bRtn;
}
void CDeviceMotion::SetPosition(MP2100_THREAD thr, AXIS_ID axis, double dPosition)	// ������ ���� ��ġ���� �ٲ۴�
{
	if(theAxisBank.m_Axis[axis].m_bUse == FALSE)
	{
		if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
		{
			theLog[LOG_MP2100].AddBuf(_T("%s Axis Not Use"), theAxisBank.m_Axis[axis].m_strAxisName);
		}
		else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
		{
			if(axis != AXIS_STP_RESERVED)
				theLog[LOG_NMC].AddBuf(_T("%s Axis Not Use"), theAxisBank.m_Axis[axis].m_strAxisName);
		}
		return;
	}
	BOOL bRtn;
	int nDevice = theAxisBank.m_Axis[axis].m_nModuleNo;
	int nInnerAxis = theAxisBank.m_Axis[axis].m_nInnerNo;

	if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
	{
		bRtn = m_Mp2100[thr][nDevice].ZeroPointSet(nInnerAxis, dPosition);
	}
	else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
	{
		CAxisParam	nodeM;
		theAxisBank.GetParamNode(axis, nodeM);

		if (axis == AXIS_STP_PA_CH1_T || axis == AXIS_STP_PA_CH2_T)
			bRtn = m_Nmc2[nDevice].ZeroPointSet(nInnerAxis, ConvertPosition(DegToMM,dPosition,FALSE), nodeM.m_dGearRatio);
		else if(axis == AXIS_STP_INS_CH1_T || axis == AXIS_STP_INS_CH2_T)
			bRtn = m_Nmc2[nDevice].ZeroPointSet(nInnerAxis, ConvertPosition(DegToMM,dPosition,TRUE), nodeM.m_dGearRatio);
		else
			bRtn = m_Nmc2[nDevice].ZeroPointSet(nInnerAxis, dPosition, nodeM.m_dGearRatio);
	}
	if(!bRtn)
	{
		if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
		{
			theLog[LOG_MP2100].AddBuf(_T("%s failed SetPostion"), theAxisBank.m_Axis[axis].m_strAxisName);
		}
		else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
		{
			theLog[LOG_NMC].AddBuf(_T("%s failed SetPostion"), theAxisBank.m_Axis[axis].m_strAxisName);
		}
		return;
	}
	//return;
}
BOOL CDeviceMotion::AxisMove(MP2100_THREAD thr, AXIS_ID axis, AxisMoveParam param)
{
	//20160930 kw �ະ ����ó�� �ʿ�
	if(theAxisBank.m_Axis[axis].m_bUse == FALSE)
	{
		//theLog[LOG_NMC].AddBuf(LOG_MOTOR_ERROR,_T("%s Axis Not Use"), theAxisBank.m_Axis[axis].m_strAxisName);
		if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
		{
			theLog[LOG_MP2100].AddBuf(_T("%s Axis Not Use"), theAxisBank.m_Axis[axis].m_strAxisName);
		}
		else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
		{
			if(axis != AXIS_STP_RESERVED)
				theLog[LOG_NMC].AddBuf(_T("%s Axis Not Use"), theAxisBank.m_Axis[axis].m_strAxisName);
		}
		return FALSE;
	}

	m_dAcctype = param.dAcc;
	CAxisParam	nodeM;
	theAxisBank.GetParamNode(axis, nodeM);

	if(param.dSpeed == 0.f)
		param.dSpeed = nodeM.m_dDefaultMoveSpeed;
	//double dPosition	= dPos;	//bRelative ? (GetFeedbackPos(axis) + dPos) : dPos;

	if(param.dTargetPosition > nodeM.m_dPositiveLimit)
	{
		//theLog[LOG_NMC].AddBuf(LOG_MOTOR_ERROR, _T("nodeM.dPositiveLimit - %d, %lf,%lf"), axis, param.dTargetPosition , nodeM.m_dPositiveLimit);
		theLog[LOG_NMC].AddBuf(_T("nodeM.dPositiveLimit - %d, %lf,%lf"), axis, param.dTargetPosition , nodeM.m_dPositiveLimit);
		return FALSE;
	}
	if(param.dTargetPosition < nodeM.m_dNegativeLimit)
	{
		//theLog[LOG_NMC].AddBuf(LOG_MOTOR_ERROR, _T("nodeM.dNegativeLimit - %d, %lf,%lf"), axis, param.dTargetPosition , nodeM.m_dNegativeLimit);
		theLog[LOG_NMC].AddBuf(_T("nodeM.dNegativeLimit - %d, %lf,%lf"), axis, param.dTargetPosition , nodeM.m_dNegativeLimit);
		return FALSE;
	}


	// -999�̸� �̵� ���۸�ɶ� ��������, 0�� �ƴϸ� ��ġ üũ�ϴ� Time Over ��������... [10/12/2011 OSC]
	m_TargetPos[axis] = -999;

	BOOL bRtn;
	int nDevice = theAxisBank.m_Axis[axis].m_nModuleNo;
	int nInnerAxis = theAxisBank.m_Axis[axis].m_nInnerNo;
	if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
	{
		if(m_dAcctype == 0.)
		{
			m_dAcctype = ACC_300ms;
		}

		SHORT sMoveType; 

		sMoveType = MTYPE_ABSOLUTE;		

		bRtn = m_Mp2100[thr][nDevice].AxisMove(nInnerAxis, param.dTargetPosition, param.dSpeed, sMoveType, m_dAcctype);
	}
	else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
	{
		m_dAcctype = ACC_300ms;
		
		if (nDevice == 0 && (nInnerAxis == 4 || nInnerAxis == 5))
			bRtn = m_Nmc2[nDevice].AxisMove(nInnerAxis, ConvertPosition(DegToMM,param.dTargetPosition,FALSE), param.dSpeed, nodeM.m_dGearRatio, m_dAcctype);
		else if(nDevice == 1 && (nInnerAxis == 2 || nInnerAxis == 3))
			bRtn = m_Nmc2[nDevice].AxisMove(nInnerAxis, ConvertPosition(DegToMM,param.dTargetPosition,TRUE), param.dSpeed, nodeM.m_dGearRatio, m_dAcctype);
		else
			bRtn = m_Nmc2[nDevice].AxisMove(nInnerAxis, param.dTargetPosition, param.dSpeed, nodeM.m_dGearRatio, m_dAcctype);
	}
	if(bRtn == FALSE)
	{
		theLog[LOG_NMC].AddBuf(_T("%s AxisMove Fail. Pos = %f"), nodeM.m_strAxisName, param.dTargetPosition);
	}

	return bRtn;
	
}
BOOL CDeviceMotion::AxisMoveInc(MP2100_THREAD thr, AXIS_ID axis, AxisMoveParam param)
{
	//20160930 kw �ະ ����ó�� �ʿ�

	double dFeedPos = 0.;
	GetPosition(thr, axis, dFeedPos);
	param.dTargetPosition += dFeedPos;
	return AxisMove(thr, axis, param);	return FALSE;

}
BOOL CDeviceMotion::AxisMoveJog(MP2100_THREAD thr, AXIS_ID axis, BOOL bForward, AxisMoveParam param)
{
	if(theAxisBank.m_Axis[axis].m_bUse == FALSE)
	{
		if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
		{
			theLog[LOG_MP2100].AddBuf(_T("%s Axis Not Use"), theAxisBank.m_Axis[axis].m_strAxisName);
		}
		else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
		{
			if(axis != AXIS_STP_RESERVED)
				theLog[LOG_NMC].AddBuf(_T("%s Axis Not Use"), theAxisBank.m_Axis[axis].m_strAxisName);
		}
		return FALSE;
	}

	CAxisParam	nodeM;
	theAxisBank.GetParamNode(axis, nodeM);

	if(!IsAxisReady(thr, axis, TRUE))
		return FALSE;

	double dVelocity = 0.f;
	double dPosition = 0.f;

	if(param.dSpeed!=0.0)								dVelocity	= param.dSpeed;
	//else if(nJogSpeedType == JOG_SPEED_HIGH)	dVelocity	= nodeM.dJogHighSpeed;
	//else if(nJogSpeedType == JOG_SPEED_MID)		dVelocity	= nodeM.dJogMidSpeed;
	//else										dVelocity	= nodeM.dJogLowSpeed;

	//if(dVelocity == 0.f)
	//	dVelocity	= nodeM.dJogLowSpeed;

	if(bForward)	dPosition	= nodeM.m_dPositiveLimit;
	else			dPosition	= nodeM.m_dNegativeLimit;

	param.dTargetPosition = dPosition;
	return AxisMove(thr, axis, param);

}
BOOL CDeviceMotion::OriginRetrun(MP2100_THREAD thr, AXIS_ID axis)  // home search
{
	//20160930 kw �ະ ����ó�� �ʿ�

	if(theAxisBank.m_Axis[axis].m_bUse == FALSE)
	{
		if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
		{
			theLog[LOG_MP2100].AddBuf(_T("%s Axis Not Use"), theAxisBank.m_Axis[axis].m_strAxisName);
		}
		else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
		{
			if(axis != AXIS_STP_RESERVED)
				theLog[LOG_NMC].AddBuf(_T("%s Axis Not Use"), theAxisBank.m_Axis[axis].m_strAxisName);
		}
		return TRUE;
	}

	CAxisParam	nodeM;
	theAxisBank.GetParamNode(axis, nodeM);

	double dHomeSpeed = nodeM.m_dHomeSpeed;

	BOOL bRtn;
	int nDevice = theAxisBank.m_Axis[axis].m_nModuleNo;
	int nInnerAxis = theAxisBank.m_Axis[axis].m_nInnerNo;
	int nOriginMode;

	if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)	
	{
		switch (theAxisBank.m_Axis[axis].m_OriginType)
		{
		case CAxisParam::ORIGIN_TYPE_HOME:
			nOriginMode = HMETHOD_HOMELS_ONLY;
			break;
		case CAxisParam::ORIGIN_TYPE_LIMIT_N:
			nOriginMode = HMETHOD_NOT_C;
			break;
		case CAxisParam::ORIGIN_TYPE_LIMIT_P:
			nOriginMode = HMETHOD_POT_C;
			break;
		case CAxisParam::ORIGIN_TYPE_Z_PULSE:
			nOriginMode = HMETHOD_C_ONLY;
			break;
		default:
			nOriginMode = HMETHOD_HOMELS_ONLY;
			break;
		}
		bRtn = m_Mp2100[thr][nDevice].ZeroPointReturn(nInnerAxis, dHomeSpeed, ACC_300ms, nOriginMode);
	}
	else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
	{
		// 2 : Home Sensor�� ����
		// 1 : -Limit�� ����
		// 0 : +Limit�� ����
		double dOffsetPos = 0;
		switch (theAxisBank.m_Axis[axis].m_OriginType)
		{
		case CAxisParam::ORIGIN_TYPE_HOME:
			nOriginMode = 2;
			break;
		case CAxisParam::ORIGIN_TYPE_LIMIT_N:
			nOriginMode = 1;
			dOffsetPos = 1.;
			break;
		case CAxisParam::ORIGIN_TYPE_LIMIT_P:
			nOriginMode = 0;
			dOffsetPos = 1.;
			break;
		default:
			nOriginMode = 2;
			break;
		}

		if (nDevice == 1 && nInnerAxis == 2)
		{
			dOffsetPos = (theAxisBank.m_Axis[AXIS_STP_INS_CH1_T].m_dPositiveLimit);
			dOffsetPos = ConvertPosition(DegToMM,dOffsetPos,TRUE);
		}	
		else if (nDevice == 1 && nInnerAxis == 3)
		{
			dOffsetPos = (theAxisBank.m_Axis[AXIS_STP_INS_CH2_T].m_dPositiveLimit);
			dOffsetPos = ConvertPosition(DegToMM,dOffsetPos,TRUE);
		}
		else if (nDevice == 0 && nInnerAxis == 4)
		{
			dOffsetPos = (theAxisBank.m_Axis[AXIS_STP_PA_CH1_T].m_dPositiveLimit);
			dOffsetPos = ConvertPosition(DegToMM,dOffsetPos,FALSE);
		}
		else if (nDevice == 0 && nInnerAxis == 5)
		{
			dOffsetPos = (theAxisBank.m_Axis[AXIS_STP_PA_CH2_T].m_dPositiveLimit);
			dOffsetPos = ConvertPosition(DegToMM,dOffsetPos,FALSE);
		}
			
		bRtn = m_Nmc2[nDevice].ZeroPointReturn(nInnerAxis, nOriginMode, nodeM.m_dGearRatio, dHomeSpeed, ACC_300ms, dOffsetPos);
	}
	return bRtn;
}
void CDeviceMotion::AxisStop(MP2100_THREAD thr, AXIS_ID axis)			// ���� ���� �� ����
{
	//20160930 kw �ະ ����ó�� �ʿ�

	if(theAxisBank.m_Axis[axis].m_bUse == FALSE)
	{
		if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
		{
			theLog[LOG_MP2100].AddBuf(_T("%s Axis Not Use"), theAxisBank.m_Axis[axis].m_strAxisName);
		}
		else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
		{
			if(axis != AXIS_STP_RESERVED)
				theLog[LOG_NMC].AddBuf(_T("%s Axis Not Use"), theAxisBank.m_Axis[axis].m_strAxisName);
		}
		return;
	}
	BOOL bRtn = 0;
	int nDevice = theAxisBank.m_Axis[axis].m_nModuleNo;
	int nInnerAxis = theAxisBank.m_Axis[axis].m_nInnerNo;
	if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)	
	{
		CAxisParam	nodeM;
		theAxisBank.GetParamNode(axis, nodeM);
		if(!m_Mp2100[thr][nDevice].AxisStop(nInnerAxis, 500))
		{
			theLog[LOG_MP2100].AddBuf(_T("%s AxisStop failed."), theAxisBank.m_Axis[axis].m_strAxisName);

			return ;
		}
	}
	else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
	{
		bRtn = m_Nmc2[nDevice].AxisStop(nInnerAxis, 0);
		if(!bRtn)
		{
			theLog[LOG_NMC].AddBuf(_T("%s AxisStop failed."), theAxisBank.m_Axis[axis].m_strAxisName);

			return ;

		}
		//if((axis == CONST_AXIS::SVO_AMT_TURN_TABLE) && g_pMainFrm->m_hWnd )
		//	::PostMessage(g_pMainFrm->m_hWnd, WM_TURN_TABLE_HOME_STOP, NULL, NULL);
	}

}
void CDeviceMotion::AxisStopEMS(MP2100_THREAD thr, AXIS_ID axis)		// ������
{
	//20160930 kw �ະ ����ó�� �ʿ�

	if(theAxisBank.m_Axis[axis].m_bUse == FALSE)
	{
		if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
		{
			theLog[LOG_MP2100].AddBuf(_T("%s Axis Not Use"), theAxisBank.m_Axis[axis].m_strAxisName);
		}
		else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
		{
			if(axis != AXIS_STP_RESERVED)
				theLog[LOG_NMC].AddBuf(_T("%s Axis Not Use"), theAxisBank.m_Axis[axis].m_strAxisName);
		}
		return;
	}

	BOOL bRtn = 0;
	int nDevice = theAxisBank.m_Axis[axis].m_nModuleNo;
	int nInnerAxis = theAxisBank.m_Axis[axis].m_nInnerNo;
	if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)	
	{
		if(!m_Mp2100[thr][nDevice].AxisStop(nInnerAxis, 200))
		{
			theLog[LOG_MP2100].AddBuf(_T("%s AxisStop failed."), theAxisBank.m_Axis[axis].m_strAxisName);

			return ;
		}
	}
	else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
	{
		bRtn = m_Nmc2[nDevice].AxisStop(nInnerAxis, 1);
		if(!bRtn)
		{
			theLog[LOG_NMC].AddBuf(_T("%s AxisStop failed."), theAxisBank.m_Axis[axis].m_strAxisName);

			return ;

		}
	}

}
AxisStatus CDeviceMotion::GetAxisStatus(MP2100_THREAD thr, AXIS_ID axis)
{
	// �� ���¸� ��� AxisStatus�� �����Ͽ� ��ȯ�Ѵ�.
	AxisStatus status;

	if(theAxisBank.m_Axis[axis].m_bUse == FALSE)
	{
		if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
		{
			theLog[LOG_MP2100].AddBuf(_T("%s Axis Not Use"), theAxisBank.m_Axis[axis].m_strAxisName);
		}
		else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
		{
			if(axis != AXIS_STP_RESERVED)
				theLog[LOG_NMC].AddBuf(_T("%s Axis Not Use"), theAxisBank.m_Axis[axis].m_strAxisName);
		}
		return status;
	}

	int nDevice = theAxisBank.m_Axis[axis].m_nModuleNo;
	int nInnerAxis = theAxisBank.m_Axis[axis].m_nInnerNo;
	if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
	{
		WORD wVal;
		LONG lVal = 0;

		m_Mp2100[thr][nDevice].GetParam_DriveStatus(nInnerAxis, wVal);
// 		status.bBusy = ((wVal>>CMp2100::DRIVE_STATUS_MOTION_READY)&0x01) ? FALSE:TRUE;
		status.bBusy = IsAxisReady(thr, axis, TRUE) ? FALSE:TRUE;
		status.bServoOn = ((wVal>>CMp2100::DRIVE_STATUS_RUNNING)&0x01) ? TRUE:FALSE;

		m_Mp2100[thr][nDevice].GetParam_Alarm(nInnerAxis, lVal);
		status.bAlarmOn = lVal ? TRUE:FALSE;
		m_Mp2100[thr][nDevice].GetParam_Warning(nInnerAxis, lVal);
		status.bWarningOn = lVal ? TRUE:FALSE;

		m_Mp2100[thr][nDevice].GetParam_ServoIO(nInnerAxis, wVal);
		status.bHomeOn = (wVal >> CMp2100::NETWORK_SERVO_IO_MONITOR_EXT1 & 0x01) ? TRUE:FALSE;
		status.bPLimitOn = (wVal >> CMp2100::NETWORK_SERVO_IO_MONITOR_POT & 0x01) ? TRUE:FALSE;
		status.bNLimitOn = (wVal >> CMp2100::NETWORK_SERVO_IO_MONITOR_NOT & 0x01) ? TRUE:FALSE;
		status.bOriginComplete = m_Mp2100[thr][nDevice].GetParamBit_ZRNC(nInnerAxis);
	}
	else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
	{
		NMC_AXES_EXPR nmcdata;
		m_Nmc2[nDevice].GetNmcData(nmcdata);
		status.bServoOn = m_Nmc2[nDevice].CheckServo(nInnerAxis);
		status.bAlarmOn = nmcdata.nAlarm[nInnerAxis];
// 		status.bWarningOn = nmcdata.nAlarm[nInnerAxis];	// NMC�� Warning�� ����
		if(theAxisBank.m_Axis[axis].m_MotorType == CAxisParam::MOTOR_TYPE_STEP)
			status.bBusy = nmcdata.nBusy[nInnerAxis] ? TRUE:FALSE;
		else
			status.bBusy = ( nmcdata.nBusy[nInnerAxis] || (nmcdata.nInpo[nInnerAxis] == FALSE) ) ? TRUE:FALSE;

		status.bHomeOn = nmcdata.nNear[nInnerAxis];
		status.bPLimitOn = nmcdata.nPLimit[nInnerAxis];
		status.bNLimitOn = nmcdata.nMLimit[nInnerAxis];
		status.bOriginComplete = m_Nmc2[nDevice].CheckZRNC(nInnerAxis);
	}
	return status;
}
CString CDeviceMotion::GetAlarmCode(MP2100_THREAD thr, AXIS_ID axis)	// Ȥ�� ��ü���� 10����, 16���� �� ǥ�� ���̰� ������� ���ڿ��� ó���Ѵ�
{
	CString str = _T("");
	if(theAxisBank.m_Axis[axis].m_bUse == FALSE)
	{
		if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
		{
			theLog[LOG_MP2100].AddBuf(_T("%s Axis Not Use"), theAxisBank.m_Axis[axis].m_strAxisName);
		}
		else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
		{
			if(axis != AXIS_STP_RESERVED)
				theLog[LOG_NMC].AddBuf(_T("%s Axis Not Use"), theAxisBank.m_Axis[axis].m_strAxisName);
		}
		return str;
	}
	LONG lVal = 0;

	int nDevice = theAxisBank.m_Axis[axis].m_nModuleNo;
	int nInnerAxis = theAxisBank.m_Axis[axis].m_nInnerNo;
	if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
	{
		if(!m_Mp2100[thr][nDevice].GetParam_Alarm(nInnerAxis, lVal))
			return FALSE;
		str.Format(_T("%X0"),lVal);	// 3�ڸ� Alarm�ε� ���ڸ��ۿ� ǥ�� ���ؼ� �ڿ� 0�� ���δ�
	}
	// NMC�� ���������� Alarm �ڵ带 �޴� ����� ����
// 	else
// 	{
// 		nBoard			= theAxisBank.m_Axis[axis].m_nModuleNo;
// 		nInnerAxisNo	= theAxisBank.m_Axis[axis].m_nInnerNo;
// 		m_Nmc2[nBoard].GetAlarmSignal(nInnerAxisNo, lVal);
// 	}
	return str;
}
double CDeviceMotion::GetTorque(MP2100_THREAD thr, AXIS_ID axis)		// int? double?
{
	double dTorque = 0;
	if(theAxisBank.m_Axis[axis].m_bUse == FALSE)
	{
		if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
		{
			theLog[LOG_MP2100].AddBuf(_T("%s Axis Not Use"), theAxisBank.m_Axis[axis].m_strAxisName);
		}
		else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
		{
			if(axis != AXIS_STP_RESERVED)
				theLog[LOG_NMC].AddBuf(_T("%s Axis Not Use"), theAxisBank.m_Axis[axis].m_strAxisName);
		}
		return dTorque;
	}

	int nDevice = theAxisBank.m_Axis[axis].m_nModuleNo;
	int nInnerAxis = theAxisBank.m_Axis[axis].m_nInnerNo;
	if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
	{
		if(!m_Mp2100[thr][nDevice].GetParam_TorqueChk(nInnerAxis, dTorque))
			return dTorque;
	}
	else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
	{
		// NMC�� ��ũ�� �޾ƿ��� ����
		dTorque = 0.;
	}

	return dTorque;
}
BOOL CDeviceMotion::GetPosition(MP2100_THREAD thr, AXIS_ID axis, double &dPos)
{
	//20160930 kw �ະ ����ó�� �ʿ�

	if(theAxisBank.m_Axis[axis].m_bUse == FALSE)
	{
		if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
		{
			theLog[LOG_MP2100].AddBuf(_T("%s Axis Not Use"), theAxisBank.m_Axis[axis].m_strAxisName);
		}
		else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
		{
			if(axis != AXIS_STP_RESERVED)
				theLog[LOG_NMC].AddBuf(_T("%s Axis Not Use"), theAxisBank.m_Axis[axis].m_strAxisName);
		}
		return FALSE;
	}
	int nDevice = theAxisBank.m_Axis[axis].m_nModuleNo;
	int nInnerAxis = theAxisBank.m_Axis[axis].m_nInnerNo;
	if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
	{
		if(!m_Mp2100[thr][nDevice].GetParam_FeedbackPos(nInnerAxis, dPos))
			return FALSE;
	}
	else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
	{
		CAxisParam	nodeM;
		theAxisBank.GetParamNode(axis, nodeM);
		if(theAxisBank.m_Axis[axis].m_MotorType == CAxisParam::MOTOR_TYPE_STEP)
		{

			if(!m_Nmc2[nDevice].GetCmdPos(nInnerAxis, nodeM.m_dGearRatio, dPos))
				return FALSE;
		}
		else
		{
			if(!m_Nmc2[nDevice].GetEncPos(nInnerAxis, nodeM.m_dGearRatio, dPos))
				return FALSE;
		}

		if (nDevice == 0 && (nInnerAxis == 4 || nInnerAxis == 5))
			dPos = ConvertPosition(MMtoDeg,dPos,FALSE);
		else if((nDevice == 1 && (nInnerAxis == 2 || nInnerAxis == 3)))
			dPos = ConvertPosition(MMtoDeg,dPos,TRUE);
	}

	return TRUE;
}
BOOL CDeviceMotion::GetTargetPosition(MP2100_THREAD thr, AXIS_ID axis, double &dPos)	// �̵��� ��ǥ��ġ
{
	if(theAxisBank.m_Axis[axis].m_bUse == FALSE)
	{
		if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
		{
			theLog[LOG_MP2100].AddBuf(_T("%s Axis Not Use"), theAxisBank.m_Axis[axis].m_strAxisName);
		}
		else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
		{
			if(axis != AXIS_STP_RESERVED)
				theLog[LOG_NMC].AddBuf(_T("%s Axis Not Use"), theAxisBank.m_Axis[axis].m_strAxisName);
		}
		return FALSE;
	}
	int nDevice = theAxisBank.m_Axis[axis].m_nModuleNo;
	int nInnerAxis = theAxisBank.m_Axis[axis].m_nInnerNo;
	if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
	{
		if(!m_Mp2100[thr][nDevice].GetParam_TargetPos(nInnerAxis, dPos))
			return FALSE;
	}
	else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
	{	
		//CAxisParam	nodeM;
		//theAxisBank.GetParamNode(axis, nodeM);
		//if(!m_Nmc2[nDevice].GetCmdPos(nInnerAxis, nodeM.m_dGearRatio,dPos)) // Enc ���� ������ Command�� ��ü��.
		//	return FALSE;
		if(!m_Nmc2[nDevice].GetTargetPostion(/*nDevice,*/nInnerAxis,dPos))
			return FALSE;
	}

	return TRUE;

}

//bIgnoreOrigin:TRUE  ���� Ȯ�ξ���
BOOL CDeviceMotion::IsAxisReady(MP2100_THREAD thr, AXIS_ID axis, BOOL bIgnoreOrigin)	// �ش� ���� ���� ���� �� �ִ� �������� Ȯ��
{
	if(axis == AXIS_SVO_LOAD_CONV)
		return TRUE;

	if(theAxisBank.m_Axis[axis].m_bUse == FALSE)
	{
		if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
		{
			theLog[LOG_MP2100].AddBuf(_T("%s Axis Not Use"), theAxisBank.m_Axis[axis].m_strAxisName);
		}
		else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
		{
			if(axis != AXIS_STP_RESERVED)
				theLog[LOG_NMC].AddBuf(_T("%s Axis Not Use"), theAxisBank.m_Axis[axis].m_strAxisName);
		}
		return TRUE;
	}
	BOOL bRtn;
	BOOL bIgnore = TRUE;
	int nDevice = theAxisBank.m_Axis[axis].m_nModuleNo;
	int nInnerAxis = theAxisBank.m_Axis[axis].m_nInnerNo;
	if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
	{
		if(!bIgnoreOrigin)
			bIgnore = m_Mp2100[thr][nDevice].GetParamBit_ZRNC(nInnerAxis);
		bRtn = m_Mp2100[thr][nDevice].IsAxisReady(nInnerAxis);
	}
	else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
	{
		if(!bIgnoreOrigin)
		{
			bIgnore = m_Nmc2[nDevice].CheckZRNC(nInnerAxis);
		}
		if(theAxisBank.m_Axis[axis].m_MotorType == CAxisParam::MOTOR_TYPE_STEP)
			bRtn = m_Nmc2[nDevice].CheckAxisReady(nInnerAxis, TRUE);	
		else
			bRtn = m_Nmc2[nDevice].CheckAxisReady(nInnerAxis, FALSE);	
	}
	if(!bIgnore)
		return FALSE;
	return bRtn;
}
BOOL CDeviceMotion::IsTrouble(MP2100_THREAD thr, AXIS_ID axis)	// �࿡ ������ �ִ��� ������ Ȯ��(Busy�ʹ� ������� Alarm�̳� Limit ���� ������Ҹ� Ȯ��)
{
	// Return Value
	// TRUE = Trouble
	// FALSE = Not Trouble
	if(theAxisBank.m_Axis[axis].m_bUse == FALSE)
	{
		if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
		{
			theLog[LOG_MP2100].AddBuf(_T("%s Axis Not Use"), theAxisBank.m_Axis[axis].m_strAxisName);
		}
		else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
		{
			if(axis != AXIS_STP_RESERVED)
				theLog[LOG_NMC].AddBuf(_T("%s Axis Not Use"), theAxisBank.m_Axis[axis].m_strAxisName);
		}
		return TRUE;
	}
	BOOL bRtn;
	int nDevice = theAxisBank.m_Axis[axis].m_nModuleNo;
	int nInnerAxis = theAxisBank.m_Axis[axis].m_nInnerNo;
	if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
	{
		bRtn = m_Mp2100[thr][nDevice].CheckTrouble(nInnerAxis);
	}
	else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
	{
		bRtn = m_Nmc2[nDevice].CheckTrouble(nInnerAxis);
	}

	return bRtn;
}

BOOL CDeviceMotion::TeachMove( MP2100_THREAD thr, TEACH_PARAM::ID idTeach, double dOffset /*= 0.*/ )
{
	CUnitCtrlFunc UnitCtrl(thr);

	BOOL RunState = FALSE;
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN
		|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)
	{
		RunState = TRUE;
	}

	if(UnitCtrl.CheckTeachMoveInterlock(idTeach, dOffset,RunState) == FALSE)
	{
		theLog[LOG_MOTOR_ERROR].AddBuf(_T("%s Axis Interlock : %s"), 
			theAxisBank.m_Axis[theTeachBank.GetTeachParam( idTeach ).idAxis].m_strAxisName,
			UnitCtrl.m_strLastKorMsg);
		return FALSE;
	}

	double dOldSpped = 0;
	AxisMoveParam paramMove;
	paramMove = theTeachBank.GetTeachParam( idTeach );

	// Cell Get ���� Up�ϴ� ��� Z �� ���ǵ带 Put���� Up�ϴ� ���ǵ� ���� ������ �Ѵ�. 17/04/26 HSK.
	if(idTeach == TEACH_PARAM::ULD_TR_Z_UP || idTeach == TEACH_PARAM::PA_TR_Z_UP || idTeach == TEACH_PARAM::PA_TR_Z_UP
		|| idTeach == TEACH_PARAM::LD_TR_Z_UP || idTeach == TEACH_PARAM::OK_TRAY_TR_Z_TO_UP || idTeach == TEACH_PARAM::NG_TRAY_TR_Z_TO_UP
		|| idTeach == TEACH_PARAM::LD_TR_X_CH1_LOWER_MCR || idTeach == TEACH_PARAM::LD_TR_X_CH2_LOWER_MCR
		|| idTeach == TEACH_PARAM::LD_ROBOT_CONV)
	{
		if(dOffset != 0.0)
			paramMove.dSpeed = paramMove.dSpeed * dOffset;
	}
	else
	{
		//2017-09-20,SKCHO, ULD TR ������ ��� �߰�
		if(idTeach == TEACH_PARAM::ULD_TR_Z_GET &&  dOffset != 0.0)
		{
			dOldSpped = paramMove.dSpeed;
			paramMove.dSpeed /= 10.0;
		}
		paramMove.dTargetPosition += dOffset;
	}

	if(idTeach == TEACH_PARAM::LD_ROBOT_CONV)
	{
		if(theConfigBank.m_Option.m_bLoadPitchIn == FALSE)
			paramMove.dTargetPosition = paramMove.dTargetPosition * 3;
	}
	// Origin�� �Ǿ����� ������ ��ġ�̵� �Ұ� [11/1/2016 OSC]
	if(IsAxisReady(thr, paramMove.idAxis, FALSE) == FALSE)
	{
		theLog[LOG_MOTOR_ERROR].AddBuf(_T("%s Axis Not Ready"), 
				theAxisBank.m_Axis[theTeachBank.GetTeachParam( idTeach ).idAxis].m_strAxisName);
			
		//2017-09-20,SKCHO, ULD TR ������ ��� �߰�
		if(idTeach == TEACH_PARAM::ULD_TR_Z_GET &&  dOffset != 0.0)
		{
			 paramMove.dSpeed = dOldSpped;			
		}

		return FALSE;
	}

	BOOL bRet = FALSE;
	bRet = AxisMove(thr, paramMove.idAxis, paramMove);

	//2017-04-18,skcho, ƼĪ��� �̵� �� �� ���� ���̵�
	if(theUnitFunc.GetInPutIOCheck(X_TEACH_KEY_SW) == FALSE)
		theProcBank.m_bTeachMoveComplete = FALSE;

	theProcBank.m_nPresentAxis = (AXIS_ID)theTeachBank.GetTeachParam( idTeach ).idAxis;

	//2017-09-20,SKCHO, ULD TR ������ ��� �߰�
	if(idTeach == TEACH_PARAM::ULD_TR_Z_GET &&  dOffset != 0.0)
	{
			paramMove.dSpeed = dOldSpped;			
	}

	return bRet;
}

BOOL CDeviceMotion::CheckAxisPosition(MP2100_THREAD thr, AXIS_ID axis, double dCheckPos, double dRange /*= 0.05*/, POS_RANGE_COMPAIR rangeCompair /*= POS_EQUAL*/ )
{
	if(theAxisBank.m_Axis[axis].m_bUse == FALSE)
	{
		if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_MP2100)
		{
			theLog[LOG_MP2100].AddBuf(_T("%s Axis Not Use"), theAxisBank.m_Axis[axis].m_strAxisName);
		}
		else if(theAxisBank.m_Axis[axis].m_DeviceType == CAxisParam::DEVICE_TYPE_NMC)
		{
			if(axis != AXIS_STP_RESERVED)
				theLog[LOG_NMC].AddBuf(_T("%s Axis Not Use"), theAxisBank.m_Axis[axis].m_strAxisName);
		}
		return TRUE;
	}
	double dFeedPos, dTargetPos;
	if(!GetPosition(thr, axis, dFeedPos))	return FALSE;
	if(!GetTargetPosition(thr, axis, dTargetPos))	return FALSE;

	if(rangeCompair == POS_EQUAL)			// ���� ��ġ�� ����	(Motor ���� ���ε� ���� Ȯ���Ѵ�)
		return (IsAxisReady(thr, axis, TRUE) && dFeedPos > (dCheckPos - dRange) && dFeedPos < (dCheckPos + dRange));
	else if(rangeCompair == POS_LOWER)	// ���� ��ġ���� �۰ų� ���� (Motor ������ Ȯ�� ���Ѵ�)
		return ( (IsTrouble(thr, axis) == FALSE) && (dFeedPos < (dCheckPos + dRange)) && (dTargetPos < (dCheckPos + dRange)) );
	else if(rangeCompair == POS_UPPER)	// ���� ��ġ���� ���ų� ���� (Motor ������ Ȯ�� ���Ѵ�)
		return ( (IsTrouble(thr, axis) == FALSE) && (dFeedPos > (dCheckPos - dRange)) && (dTargetPos > (dCheckPos - dRange)) );

	return FALSE;
}

BOOL CDeviceMotion::CheckTeachPosition( MP2100_THREAD thr, TEACH_PARAM::ID idTeach, double dRange /*= 0.05*/, POS_RANGE_COMPAIR rangeCompair /*= POS_EQUAL*/, double dOffset /*= 0.*/ )
{
	TEACH_PARAM paramTeach = theTeachBank.GetTeachParam( idTeach );

	double dTeachPos = paramTeach.dPos + dOffset;
	BOOL bRet = CheckAxisPosition(thr, paramTeach.idAxis, dTeachPos, dRange, rangeCompair);

	//2017-04,18,skcho, ƼĪ �̵� �Ϸ��ϸ� �÷��� ��
	//ƼĪ ��� �� ���� �÷��� �� 2017/04/27 HSK
	if(theUnitFunc.GetInPutIOCheck(X_TEACH_KEY_SW) == FALSE)
	{
		if(bRet && theProcBank.m_nPresentAxis == paramTeach.idAxis)
		{
			theProcBank.m_bTeachMoveComplete = TRUE;
		}
	}
	return bRet;
}

//kjpark 20161030 ����ġ�� �ƴϸ� A���� A���� ������ ���̾�α�â ���
BOOL CDeviceMotion::CheckPosition( double dDefaultPos, double dNowPos, double dCheckLimit /*= 0.01*/ )	/* 0.01mm�̻� ���̳��� Ʋ���ٰ� ���� */
{
	if(max(dDefaultPos+dCheckLimit, dNowPos) == dNowPos)	return FALSE;
	if(min(dDefaultPos-dCheckLimit, dNowPos) == dNowPos)	return FALSE;

	return TRUE;
}


// Vision Turn ���� ��� ��ġ�� ���� ����.
// Ư���� ��� ��ġ ����� ������ ������ �̿��Ͽ� �����Ѵ�.
//
// [in] int nMode : ��ȯ ��������
//   eGxMMode_MachineToWorld  = 0,  // ��� ��ǥ�� -> ���� ���Ǵ� ��ǥ ��ġ�� ��ȯ��Ų��. ��) 5mm -> 2.0653��
//   eGxMMode_WorldToMachine,    // �� ��� ��ǥ ��ġ -> ��� ��ǥ ��ġ�� ��ȯ. ��) 2.0653 �� -> 5 mm
//
// [in] double dbPos : ��ȯ �ϰ����ϴ� ��
//
// [in] dobule dbOriginPos : ��ȯ ������ ��ġ��, JMR Turn ���� Ȱ��ȴ�.
//
//
// [return] double : ��ȯ�� ��ġ �� 

double CDeviceMotion::ConvertPosition(int nMode, double dbPos, BOOL bVision, double dbOriginPos)
{
	double dbNewPos = dbPos;
	double dPositionArg ;
	if(bVision)
		dPositionArg = 46;
	else
		dPositionArg = 97;
	// �� �������� ������ ��ȯ�ϰ��� �ϴ� Type�� eGxMPos_Normal �� ��� ��ȯ�� ��û�� �� �״�θ� ����Ѵ�.

	// JMR Turn �� �̼۷� ����� ���

	// ������ �ʿ� ������, dPositionArg1 : �߽� �ݰ� mm ���� ���� ��쿡�� ����� �� ����.
	if ( dPositionArg <= 0.0 ) return dbNewPos;

	if ( nMode == MMtoDeg )  // ���� ��ǥ mm -> ȸ�� ��ǥ '��' �� ��ȯ // ������ġ �о�ö�
	{
		// tan-1((��Y+��O)/R)-tan-1((��O)/R)
		// =((ATAN((O6+O8)/L6))*180/PI()) -((ATAN(O8/L6))*180/PI())

		//old dbNewPos = ((atan( dbPos + 0.0 ) / m_rAxisData.dPositionArg1 ) * 180.0 / 3.141592654) - (atan( 0.0 / m_rAxisData.dPositionArg1 ) * 180.0 / 3.141592654);
		dbNewPos = ((atan( (dbPos + dbOriginPos ) / dPositionArg) ) * 180.0 / 3.141592654) - (atan( dbOriginPos / dPositionArg ) * 180.0 / 3.141592654);

	}
	else  // ȸ�� ��ǥ �� -> ���� ��ǥ mm �� ��ȯ // �̵��� ��ġ
	{
		// R tan(���+��O)-R tan(��O)
		// =(L16*TAN(RADIANS(O16+O18)))-(L16*TAN(RADIANS(O18)))

		//old dbNewPos = ( m_rAxisData.dPositionArg1 * tan( (dbPos + 0.0 ) * (3.141592654 / 180.0))) - (m_rAxisData.dPositionArg1 * tan( 0.0 * (3.141592654 / 180.0)));
		dbNewPos = ( dPositionArg * tan( (dbPos + dbOriginPos ) * (3.141592654 / 180.0))) - (dPositionArg * tan( dbOriginPos * (3.141592654 / 180.0)));
	}

	return dbNewPos;
}
