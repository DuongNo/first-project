#include "StdAfx.h"
#include "UnitCtrlBank.h"
#include "CheetahApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//앞단 설비에 셀 받을 준비가 되어 있다고 알려줌
// Recv Able : 0x504
void CUnitCtrlBank::SendRcvEnable(ONOFF bOn)
{
	SetOutPutIO(Y_HOTLINE_INTERFACE_1,bOn);
	if(bOn)
	{
		theDeviceMelsec.UpperBitOn(OPTICAL_XY_SEND_ABLE);
	}
	else
	{
		theDeviceMelsec.UpperBitOff(OPTICAL_XY_SEND_ABLE);
	}

	::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_UPPEREQP_COM_UPDATE, bOn,OPTICAL_XY_SEND_ABLE);
}
//앞단 설비에 셀을 주라고 알려줌
// Recv Able : 0x505
void CUnitCtrlBank::SendRcvStart(ONOFF bOn)
{
	SetOutPutIO(Y_HOTLINE_INTERFACE_2,bOn);
	if(bOn)
	{
		theDeviceMelsec.UpperBitOn(OPTICAL_XY_SEND_START);
	}
	else
	{
		theDeviceMelsec.UpperBitOff(OPTICAL_XY_SEND_START);
	}

	::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_UPPEREQP_COM_UPDATE, bOn,OPTICAL_XY_SEND_START);
}

//앞단 설비의 셀을 하나 받았다고 알려줌
// Recv Able : 0x506
void CUnitCtrlBank::SendRcvComp(ONOFF bOn)
{
	SetOutPutIO(Y_HOTLINE_INTERFACE_3,bOn);
	if(bOn)
	{
		theDeviceMelsec.UpperBitOn(OPTICAL_XY_SEND_COMP);
	}
	else
	{
		theDeviceMelsec.UpperBitOff(OPTICAL_XY_SEND_COMP);
	}

	::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_UPPEREQP_COM_UPDATE, bOn,OPTICAL_XY_SEND_COMP);
}
//앞단 설비에 멈췄다고 알려줌
// Recv Able : 0x507
void CUnitCtrlBank::SendRcvPause(ONOFF bOn)
{
//	SetOutPutIO(Y_HOTLINE_INTERFACE_3,bOn);
	if(bOn)
	{
		theDeviceMelsec.UpperBitOn(OPTICAL_XY_SEND_IMM_PAUSE);
	}
	else
	{
		theDeviceMelsec.UpperBitOff(OPTICAL_XY_SEND_IMM_PAUSE);
	}

	::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_UPPEREQP_COM_UPDATE, bOn,OPTICAL_XY_SEND_IMM_PAUSE);
}
//앞단설비에서 준비 수신 체크
BOOL CUnitCtrlBank::RcvEnableChk()
{
	BOOL bResult = FALSE;
	BOOL bBuff[2] = {FALSE};
	bBuff[0] = 1;//GetInPutIOCheck(X_HOTLINE_INTERFACE_1) ;
    bBuff[1] = (BOOL)theDeviceMelsec.UpperBitRecv(OPTICAL_XY_RECEIVE_ABLE);
	if(bBuff[0] &&  bBuff[1])
	{
		bResult = TRUE;
	}	
	::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_UPPEREQP_COM_UPDATE, bBuff[1],OPTICAL_XY_RECEIVE_ABLE);
	//20170205 byskcho
	//둘중 하나만 되면 되므로 || 로 수정
	return bResult;
	//return (GetInPutIOCheck(X_HOTLINE_INTERFACE_1) && (BOOL)theDeviceMelsec.UpperBitRecv(OPTICAL_XY_RECEIVE_ABLE));
}
//앞단설비에서 시작 수신 체크
BOOL CUnitCtrlBank::RcvStartChk()
{
	BOOL bResult = FALSE;
	BOOL bBuff[2] = {FALSE};
	bBuff[0] = 1;//GetInPutIOCheck(X_HOTLINE_INTERFACE_2) ;
    bBuff[1] = (BOOL)theDeviceMelsec.UpperBitRecv(OPTICAL_XY_RECEIVE_START);
	if(bBuff[0] &&  bBuff[1])
	{
		bResult = TRUE;
	}
	::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_UPPEREQP_COM_UPDATE, bBuff[1],OPTICAL_XY_RECEIVE_START);
	//20170205 byskcho
	//둘중 하나만 되면 되므로 || 로 수정
	return bResult;
	//return (GetInPutIOCheck(X_HOTLINE_INTERFACE_2) && (BOOL)theDeviceMelsec.UpperBitRecv(OPTICAL_XY_RECEIVE_START));
}
//앞단설비에서 완료 수신 체크
BOOL CUnitCtrlBank::RcvCompChk()
{
	BOOL bResult = FALSE;
	BOOL bBuff[2] = {FALSE};
	bBuff[0] = 1;//GetInPutIOCheck(X_HOTLINE_INTERFACE_3) ;
    bBuff[1] = (BOOL)theDeviceMelsec.UpperBitRecv(OPTICAL_XY_RECEIVE_COMP);
	if(bBuff[0] &&  bBuff[1])
	{
		bResult = TRUE;
	}
	::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_UPPEREQP_COM_UPDATE, bBuff[1],OPTICAL_XY_RECEIVE_COMP);
	//20170205 byskcho
	//둘중 하나만 되면 되므로 || 로 수정
	return bResult;
	//return (GetInPutIOCheck(X_HOTLINE_INTERFACE_3) && (BOOL)theDeviceMelsec.UpperBitRecv(OPTICAL_XY_RECEIVE_COMP));
}

BOOL CUnitCtrlBank::LDConvCellChk(E_CONV_SEN_INDEX Sen)
{
	if (Sen == CONV_SEN_1)
		return GetInPutIOCheck(X_LD_CONV_CELL_SEN_1);
	else if (Sen == CONV_SEN_2)
		return GetInPutIOCheck(X_LD_CONV_CELL_SEN_2);
	else
		return GetInPutIOCheck(X_LD_CONV_CELL_SEN_3);
}

void CUnitCtrlBank::LDConvZeroPointSet()
{
	theDeviceMotion.SetPosition(m_nThreadID, AXIS_SVO_LOAD_CONV, 0.);
}

void CUnitCtrlBank::LDConvMove(double Speed)
{
	theDeviceMotion.TeachMove(m_nThreadID,TEACH_PARAM::LD_ROBOT_CONV,Speed);
}

void CUnitCtrlBank::LDConvStop()
{
	theDeviceMotion.AxisStop(m_nThreadID,AXIS_SVO_LOAD_CONV);
	LDConvZeroPointSet();
}
BOOL CUnitCtrlBank::LDConvStopChk()
{
	return theDeviceMotion.IsAxisReady( m_nThreadID, AXIS_SVO_LOAD_CONV, TRUE);
}
void CUnitCtrlBank::LoaderAlignStart()
{
	CCellInfo* pCellInfo;
	CCellTag tagCell;
	pCellInfo = theCellBank.GetCellInfo(CELL_POS_LD_CONV);
	tagCell = theCellBank.GetCellTag(CELL_POS_LD_CONV);
	if(tagCell.IsExist())
	{
		pCellInfo->LoaderAlignResult = ALIGN_RESULT_NONE;
		theSocketInterFace.SendMsgToMITEdgeAlign(LA_ALIGN_REQ, _T("MANUAL"), pCellInfo->m_strInnerID);
	}
}

BOOL CUnitCtrlBank::LoaderAlignEndChk()
{
	if(theConfigBank.m_Option.m_bUseDryRun || (theConfigBank.m_Option.m_bUseEdgeAlign == FALSE))
		return TRUE;
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
	BOOL bResult = FALSE;

	if(theConfigBank.m_Option.m_bUseDryRun || (theConfigBank.m_Option.m_bUseEdgeAlign == FALSE))
		return TRUE;
	CCellInfo* pCellInfo;
	CCellTag tagCell;
	pCellInfo = theCellBank.GetCellInfo(CELL_POS_LD_CONV);
	tagCell = theCellBank.GetCellTag(CELL_POS_LD_CONV);
	if(tagCell.IsExist())
	{
		//20170206 byskcho
		//에지 얼라인 결과 셀로그 저장
		if(pCellInfo->LoaderAlignResult == ALIGN_RESULT_GOOD)
		{
			bResult = TRUE;
			pCellInfo->m_strEdgeResult = _T("GOOD");
		}
		else
		{
			pCellInfo->m_strEdgeResult = _T("NG");
			bResult = FALSE;
		}
	}
	return bResult;
}

double CUnitCtrlBank::LoaderAlignResultX()
{
	CCellInfo* pCellInfo;
	CCellTag tagCell;
	pCellInfo = theCellBank.GetCellInfo(CELL_POS_LD_CONV);
	tagCell = theCellBank.GetCellTag(CELL_POS_LD_CONV);
	if(tagCell.IsExist())
	{
		return pCellInfo->LoaderAlignDataX;
	}
	return 0.0;
}

double CUnitCtrlBank::LoaderAlignResultY()
{
	CCellInfo* pCellInfo;
	CCellTag tagCell;
	pCellInfo = theCellBank.GetCellInfo(CELL_POS_LD_CONV);
	tagCell = theCellBank.GetCellTag(CELL_POS_LD_CONV);
	if(tagCell.IsExist())
	{
		return pCellInfo->LoaderAlignDataY;
	}
	return 0.0;
}

double CUnitCtrlBank::LoaderAlignResultT()
{
	CCellInfo* pCellInfo;
	CCellTag tagCell;
	pCellInfo = theCellBank.GetCellInfo(CELL_POS_LD_CONV);
	tagCell = theCellBank.GetCellTag(CELL_POS_LD_CONV);
	if(tagCell.IsExist())
	{
		return pCellInfo->LoaderAlignDataT;
	}
	return 0.0;
}

void CUnitCtrlBank::LDRobotSendAlignData( double dPosX , double dPosY , double dPosT)
{
	// edge align에서 X,Y와 로봇에서 X,Y가 반대라서 반대로 준다... 로봇으로 보내는 Y값도 방향이 반대라서 -1.0 곱해준다...
	theSocketInterFace.SendMsgToRobotAlign( dPosX, dPosY, dPosT);
}

void CUnitCtrlBank::RobotCellVacOn(const BOOL bVacOn,E_ROBOT Robot)
{
	if (Robot == E_LD_ROBOT)
	{
		SetOutPutIO(Y_LD_ROBOT_BLOW_ON_SOL,!bVacOn);
		SetOutPutIO(Y_LD_ROBOT_VAC_ON_SOL,bVacOn);
	}
	else
	{
		SetOutPutIO(Y_ULD_ROBOT_CELL_BLOW_ON_SOL,!bVacOn);
		SetOutPutIO(Y_ULD_ROBOT_CELL_VAC_ON_SOL,bVacOn);
	}
}

void CUnitCtrlBank::RobotBlowOff(E_ROBOT Robot)
{
	if (Robot == E_LD_ROBOT)
		SetOutPutIO(Y_LD_ROBOT_BLOW_ON_SOL,FALSE);
	else
		SetOutPutIO(Y_ULD_ROBOT_CELL_BLOW_ON_SOL,FALSE);
}

BOOL CUnitCtrlBank::RobotCellVacOnChk(E_ROBOT Robot,VAC_STATE Vac)
{
	if(theConfigBank.m_Option.m_bUseDryRun)
		return TRUE;
	if (Robot == E_LD_ROBOT)
	{
		if (Vac == VAC_ON)
			return GetInPutIOCheck(X_LD_ROBOT_VAC_SEN);
		else
			return !GetInPutIOCheck(X_LD_ROBOT_VAC_SEN);

	}
	else
	{
		if (Vac == VAC_ON)
			return GetInPutIOCheck(X_ULD_ROBOT_VAC_SEN);
		else
			return !GetInPutIOCheck(X_ULD_ROBOT_VAC_SEN);
	}

	return TRUE;
}

void CUnitCtrlBank::LDRobotBitAllOff(E_ROBOT_POS Pos,BOOL bManual)
{
	long write;
	if (Pos == E_ORG)
	{
		if(bManual)
		{
			theDeviceMelsec.RobotBitOff(LDROBOT_X_TEACHMODE_Y_EXTSTART);
			theDeviceMelsec.RobotBitOff(LDROBOT_X_STOPPING_Y_EXTMOTORON);
			theDeviceMelsec.RobotBitOff(LDROBOT_X_ALARM_Y_EXTRESET);
			theDeviceMelsec.RobotBitOff(LDROBOT_X_HOMEPOS2_Y_PGMSTROBE);
			theDeviceMelsec.RobotBitOff(LDROBOT_X_BATWARN_Y_ORG);
			//메뉴얼 모드 오프 해준다. 2017/06/20 HSK.
			theDeviceMelsec.RobotBitOff(LDROBOT_X_MANUALIN_Y_MANUALSELECT);
			theDeviceMelsec.RobotBitOff(LDROBOT_Y_SQSKIP);
		}
		else
		{
		}
	}
	else if (Pos == E_CELL_LOADING)
	{
		if(bManual)
		{
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_CVGET);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_VICOMP);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_VACON);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_VACOFF);
			theDeviceMelsec.RobotBitOff(LDROBOT_X_VACERR);
			theDeviceMelsec.RobotBitOff(LDROBOT_Y_VIERR);
			theDeviceMelsec.RobotBitOff(LDROBOT_X_CVGETERR);
			theDeviceMelsec.RobotBitOff(LDROBOT_X_CVGETCOMP);
			write = 0;
			theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
		}
		else
		{
			//write = 0;
			//theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_CVGET);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_VICOMP);
			theDeviceMelsec.RobotBitOff(LDROBOT_X_VACERR);
			theDeviceMelsec.RobotBitOff(LDROBOT_Y_VIERR);
			theDeviceMelsec.RobotBitOff(LDROBOT_X_CVGETERR);
			theDeviceMelsec.RobotBitOff(LDROBOT_X_CVGETCOMP);
			//2017.11.09, JSJUNG. 
			theDeviceMelsec.RobotBitOff(LDROBOT_Y_SQCOMPRTN);
			theDeviceMelsec.RobotBitOff(LDROBOT_Y_SQERRRTN);
			theDeviceMelsec.RobotBitOff(LDROBOT_Y_SQSKIP);
		}
	}
	else if(Pos == E_PUT_1)
	{
		if(bManual)
		{
			write = 0;
			theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_UPPERSTGPUT);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_VACON);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_VACOFF);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_UPPERSTGVAC1ON);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_UPPERSTGVAC1OFF);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_UPPERSTGVAC2ON);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_UPPERSTGVAC2OFF);
			theDeviceMelsec.RobotBitOff(LDROBOT_X_UPPERSTGVAC1ERR);
			theDeviceMelsec.RobotBitOff(LDROBOT_X_UPPERSTGVAC2ERR);
			theDeviceMelsec.RobotBitOff(LDROBOT_X_UPPERSTGPUTCOMP);
		}
		else
		{
			//write = 0;
			//theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_UPPERSTGPUT);
			theDeviceMelsec.RobotBitOff(LDROBOT_X_UPPERSTGVAC1ERR);
			theDeviceMelsec.RobotBitOff(LDROBOT_X_UPPERSTGVAC2ERR);
			theDeviceMelsec.RobotBitOff(LDROBOT_X_UPPERSTGPUTCOMP);
			//2017.11.09, JSJUNG. 
			theDeviceMelsec.RobotBitOff(LDROBOT_Y_SQCOMPRTN);
			theDeviceMelsec.RobotBitOff(LDROBOT_Y_SQERRRTN);
			theDeviceMelsec.RobotBitOff(LDROBOT_Y_SQSKIP);
			//2017.11.09, JSJUNG. 어퍼스테이지 채널1/2 풋 신호 오프
			theDeviceMelsec.RobotBitOff(LDROBOT_Y_UPPERSTGCH1PUT);
			theDeviceMelsec.RobotBitOff(LDROBOT_Y_UPPERSTGCH2PUT);
		
		}
	}
	else
	{
		if(bManual)
		{
			write = 0;
			theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_LOWERSTGPUT);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_VACON);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_VACOFF);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_LOWERSTGVAC1ON);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_LOWERSTGVAC1OFF);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_LOWERSTGVAC2ON);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_LOWERSTGVAC2OFF);
			theDeviceMelsec.RobotBitOff(LDROBOT_X_LOWERSTGVAC1ERR);
			theDeviceMelsec.RobotBitOff(LDROBOT_X_LOWERSTGVAC2ERR);
			theDeviceMelsec.RobotBitOff(LDROBOT_X_LOWERSTGPUTCOMP);

		}
		else
		{
			write = 0;
			theDeviceMelsec.RobotWordSend(LDROBOT_PERMIT,&write,sizeof(long), FALSE);
			theDeviceMelsec.RobotBitOff(LDROBOT_XY_LOWERSTGPUT);
			theDeviceMelsec.RobotBitOff(LDROBOT_X_LOWERSTGVAC1ERR);
			theDeviceMelsec.RobotBitOff(LDROBOT_X_LOWERSTGVAC2ERR);
			theDeviceMelsec.RobotBitOff(LDROBOT_X_LOWERSTGPUTCOMP);
			//2017.11.09, JSJUNG. 
			theDeviceMelsec.RobotBitOff(LDROBOT_Y_SQCOMPRTN);
			theDeviceMelsec.RobotBitOff(LDROBOT_Y_SQERRRTN);
			theDeviceMelsec.RobotBitOff(LDROBOT_Y_SQSKIP);
			//2017.11.09, JSJUNG. 로우스테이지 채널1/2 풋 신호 오프
			theDeviceMelsec.RobotBitOff(LDROBOT_Y_LOWERSTGCH1PUT);
			theDeviceMelsec.RobotBitOff(LDROBOT_Y_LOWERSTGCH2PUT);

		}
	}
}


void CUnitCtrlBank::UDRobotBitAllOff(E_ROBOT_POS Pos,BOOL bManual)
{
	long write;
	if (Pos == E_ORG)
	{
		theDeviceMelsec.RobotBitOff(UDROBOT_X_TEACHMODE_Y_EXTSTART);
		theDeviceMelsec.RobotBitOff(UDROBOT_X_STOPPING_Y_EXTMOTORON);
		theDeviceMelsec.RobotBitOff(UDROBOT_X_ALARM_Y_EXTRESET);
		theDeviceMelsec.RobotBitOff(UDROBOT_X_HOMEPOS2_Y_PGMSTROBE);
		theDeviceMelsec.RobotBitOff(UDROBOT_X_BATWARN_Y_ORG);
		//메뉴얼 모드 오프 해준다. 2017/06/20 HSK.
		theDeviceMelsec.RobotBitOff(UDROBOT_X_MANUALIN_Y_MANUALSELECT);
		theDeviceMelsec.RobotBitOff(LDROBOT_Y_SQSKIP);
	}
	else if (Pos == E_CELL_LOADING)
	{
		if(bManual)
		{
			write = 0;
			theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
			theDeviceMelsec.RobotBitOff(UDROBOT_XY_VACON);
			theDeviceMelsec.RobotBitOff(UDROBOT_XY_VACOFF);
			theDeviceMelsec.RobotBitOff(UDROBOT_XY_GETSTAGE);
			theDeviceMelsec.RobotBitOff(UDROBOT_XY_STGCH1VACON);
			theDeviceMelsec.RobotBitOff(UDROBOT_XY_STGCH1VACOFF);
			theDeviceMelsec.RobotBitOff(UDROBOT_X_STGCH2VACON_Y_STG1OK);
			theDeviceMelsec.RobotBitOff(UDROBOT_X_STGCH2VACOFF_Y_STG1NG);
			theDeviceMelsec.RobotBitOff(UDROBOT_Y_STG2VACON);
			theDeviceMelsec.RobotBitOff(UDROBOT_X_STG1GETERR_Y_STG2VACOFF);
			theDeviceMelsec.RobotBitOff(UDROBOT_X_STG2GETERR_Y_STG2OK);
			theDeviceMelsec.RobotBitOff(UDROBOT_X_STGGETCOMP_Y_STG2NG);
		}
		else
		{
			//write = 0;
			//theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
			theDeviceMelsec.RobotBitOff(UDROBOT_XY_GETSTAGE);
			theDeviceMelsec.RobotBitOff(UDROBOT_X_STGCH2VACON_Y_STG1OK);
			theDeviceMelsec.RobotBitOff(UDROBOT_X_STGCH2VACOFF_Y_STG1NG);
			theDeviceMelsec.RobotBitOff(UDROBOT_X_STG2GETERR_Y_STG2OK);
			theDeviceMelsec.RobotBitOff(UDROBOT_X_STGGETCOMP_Y_STG2NG);

			theDeviceMelsec.RobotBitOff(UDROBOT_Y_SQCOMPRTN);
			theDeviceMelsec.RobotBitOff(UDROBOT_Y_SQERRRTN);
			theDeviceMelsec.RobotBitOff(UDROBOT_Y_SQSKIP);
			theDeviceMelsec.RobotBitOff(0xBC);
			theDeviceMelsec.RobotBitOff(0xBD);

		}
	}
	else if(Pos == E_PUT_1)
	{
		if(bManual)
		{
			write = 0;
			theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
			theDeviceMelsec.RobotBitOff(UDROBOT_XY_VACON);
			theDeviceMelsec.RobotBitOff(UDROBOT_XY_VACOFF);
			theDeviceMelsec.RobotBitOff(UDROBOT_XY_OKTRAYPUT);
			theDeviceMelsec.RobotBitOff(UDROBOT_X_OKTRAYPUTERR);
			theDeviceMelsec.RobotBitOff(UDROBOT_X_OKTRAYPUTCOMP);
		}
		else
		{
			//write = 0;
			//theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
			theDeviceMelsec.RobotBitOff(UDROBOT_XY_OKTRAYPUT);
			theDeviceMelsec.RobotBitOff(UDROBOT_X_OKTRAYPUTERR);
			theDeviceMelsec.RobotBitOff(UDROBOT_X_OKTRAYPUTCOMP);

			theDeviceMelsec.RobotBitOff(UDROBOT_Y_SQCOMPRTN);
			theDeviceMelsec.RobotBitOff(UDROBOT_Y_SQERRRTN);
			theDeviceMelsec.RobotBitOff(UDROBOT_Y_SQSKIP);
			theDeviceMelsec.RobotBitOff(0xBC);
			theDeviceMelsec.RobotBitOff(0xBD);
		}
	}
	else if(Pos == E_PUT_2)
	{
		if(bManual)
		{
			write = 0;
			theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
			theDeviceMelsec.RobotBitOff(UDROBOT_XY_VACON);
			theDeviceMelsec.RobotBitOff(UDROBOT_XY_VACOFF);
			theDeviceMelsec.RobotBitOff(UDROBOT_XY_NGTRAYPUT);
			theDeviceMelsec.RobotBitOff(UDROBOT_X_NGTRAYPUTERR);
			theDeviceMelsec.RobotBitOff(UDROBOT_X_NGTRAYPUTCOMP);
		}
		else
		{
			//write = 0;
			//theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
			theDeviceMelsec.RobotBitOff(UDROBOT_XY_NGTRAYPUT);
			theDeviceMelsec.RobotBitOff(UDROBOT_X_NGTRAYPUTERR);
			theDeviceMelsec.RobotBitOff(UDROBOT_X_NGTRAYPUTCOMP);

			theDeviceMelsec.RobotBitOff(UDROBOT_Y_SQCOMPRTN);
			theDeviceMelsec.RobotBitOff(UDROBOT_Y_SQERRRTN);
			theDeviceMelsec.RobotBitOff(UDROBOT_Y_SQSKIP);
			theDeviceMelsec.RobotBitOff(0xBC);
			theDeviceMelsec.RobotBitOff(0xBD);
		}
	}
	//2018-05-14,SKCHO,추가
	else if(Pos == E_PUT_3)
	{
		if(bManual)
		{
			write = 0;
			theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
			theDeviceMelsec.RobotBitOff(UDROBOT_XY_VACON);
			theDeviceMelsec.RobotBitOff(UDROBOT_XY_VACOFF);
			theDeviceMelsec.RobotBitOff(UDROBOT_XY_REQUESTPUT_CONVEYORMODE);
			//theDeviceMelsec.RobotBitOff(UDROBOT_X_NGTRAYPUTERR);
			theDeviceMelsec.RobotBitOff(UDROBOT_XY_PUTDOWNCMP_CONVEYORREADY);
		}
		else
		{
			//write = 0;
			//theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
			theDeviceMelsec.RobotBitOff(UDROBOT_XY_REQUESTPUT_CONVEYORMODE);
			//theDeviceMelsec.RobotBitOff(UDROBOT_X_NGTRAYPUTERR);
			theDeviceMelsec.RobotBitOff(UDROBOT_XY_PUTDOWNCMP_CONVEYORREADY);

			theDeviceMelsec.RobotBitOff(UDROBOT_Y_SQCOMPRTN);
			theDeviceMelsec.RobotBitOff(UDROBOT_Y_SQERRRTN);
			theDeviceMelsec.RobotBitOff(UDROBOT_Y_SQSKIP);
			theDeviceMelsec.RobotBitOff(0xBC);
			theDeviceMelsec.RobotBitOff(0xBD);
		}
	}
}

BOOL CUnitCtrlBank::LDConvFPCBSenChk( SENSOR_STATE sen )
{
	if(theConfigBank.m_Option.m_bUseDryRun)
		return TRUE;

	ONOFF val = (sen== SENSOR_ON) ? ON:OFF;
#if(AMT_TYPE == 1)
	INPUT_ID iId = X_LD_CONV_FPCB_SEN;
#else
	INPUT_ID iId = X_LD_CONV_FPCB_SEN;
#endif

	if(GetInPutIOCheck(iId, val))
		return TRUE;
	else
		return FALSE;
}

//2018-05-10,MGYUN, 하이브리드 컨베이어 관련 작업
// 뒷단에 Robot Ready 신호 전송
void CUnitCtrlBank::SendHybRobotReady(ONOFF bOn)
{
	if(ON)
	{
		SetOutPutIO(Y_HYBRID_ROBOT_READY,ON);
	}
	else
	{
		SetOutPutIO(Y_HYBRID_ROBOT_READY,OFF);
	}
	
}

// 뒷단에 Robot Run 신호 전송
void CUnitCtrlBank::SendHybRobotRun(ONOFF bOn)
{
	if(ON)
	{
		SetOutPutIO(Y_HYBRID_ROBOT_RUN,ON);
	}
	else
	{
		SetOutPutIO(Y_HYBRID_ROBOT_RUN,OFF);
	}
}

// 뒷단에 ROBOT CELL 완료 신호 전송
void CUnitCtrlBank::SendHybSendComplete(ONOFF bOn)
{
	if(ON)
	{
		SetOutPutIO(Y_HYBRID_SEND_COMPLETE,ON);
	}
	else
	{
		SetOutPutIO(Y_HYBRID_SEND_COMPLETE,OFF);
	}
}

// 뒷단으로 부터 컨베이어 사용 준비 신호 받음
BOOL CUnitCtrlBank::RcvHybConvReadyChk()
{
	BOOL bResult = FALSE;
	BOOL bBuff[1] = {FALSE};
	bBuff[0] = GetInPutIOCheck(X_HYBRID_CONV_READY);
	if(bBuff[0])
	{
		bResult = TRUE;
	}
	return bResult;
}

// 뒷단으로 부터 컨베이어 사용 해도 된다는 신호를 받음
BOOL CUnitCtrlBank::RcvHybConvRunChk()
{
	BOOL bResult = FALSE;
	BOOL bBuff[1] = {FALSE};
	bBuff[0] = GetInPutIOCheck(X_HYBRID_CONV_RUN);
	if(bBuff[0])
	{
		bResult = TRUE;
	}
	return bResult;
}

// 뒷단으로 부터 컨베이어 사용 유무 신호를 받음 1 : CONV MODE , 0 : TRAY MODE
BOOL CUnitCtrlBank::RcvHybConvInterfaceChk()
{
	BOOL bResult = FALSE;
	BOOL bBuff[1] = {FALSE};
	bBuff[0] = GetInPutIOCheck(X_HYBRID_CONV_INTERFACE);
	if(bBuff[0])
	{
		bResult = TRUE;
	}
	return bResult;
}