#include "StdAfx.h"
#include "UnitCtrlBank.h"
#include "CheetahApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

BOOL CUnitCtrlBank::m_bZoneEnd[ZONE_ID_MAX] = {FALSE, FALSE, FALSE, FALSE};

void CUnitCtrlBank::TurnTable_Move()
{
	theDeviceMotion.TeachMove(m_nThreadID,TEACH_PARAM::WORK_TABLE_T_TO_WORK);
}

BOOL CUnitCtrlBank::TurnTable_MoveChk()
{
	return TRUE;
}

JIG_ID CUnitCtrlBank::AMT_GetAutoJigID( ZONE_ID zone )
{
	JIG_ID jig = JIG_ID_MAX;

	switch(theProcBank.m_nInspTablePos)
	{
	case CONST_TABLE_POS::POS_1:
		switch(zone)
		{
		case ZONE_ID_A:
			jig = JIG_ID_A;
			break;
		case ZONE_ID_B:
			jig = JIG_ID_B;
			break;
		case ZONE_ID_C:
			jig = JIG_ID_C;
			break;
		case ZONE_ID_D:
			jig = JIG_ID_D;
			break;
		}
		break;
	case CONST_TABLE_POS::POS_2:
		switch(zone)
		{
		case ZONE_ID_A:
			jig = JIG_ID_D;
			break;
		case ZONE_ID_B:
			jig = JIG_ID_A;
			break;
		case ZONE_ID_C:
			jig = JIG_ID_B;
			break;
		case ZONE_ID_D:
			jig = JIG_ID_C;
			break;
		}
		break;
	case CONST_TABLE_POS::POS_3:
		switch(zone)
		{
		case ZONE_ID_A:
			jig = JIG_ID_C;
			break;
		case ZONE_ID_B:
			jig = JIG_ID_D;
			break;
		case ZONE_ID_C:
			jig = JIG_ID_A;
			break;
		case ZONE_ID_D:
			jig = JIG_ID_B;
			break;
		}
		break;
	case CONST_TABLE_POS::POS_4:
		switch(zone)
		{
		case ZONE_ID_A:
			jig = JIG_ID_B;
			break;
		case ZONE_ID_B:
			jig = JIG_ID_C;
			break;
		case ZONE_ID_C:
			jig = JIG_ID_D;
			break;
		case ZONE_ID_D:
			jig = JIG_ID_A;
			break;
		}
		break;
	}
	return jig;
}

int CUnitCtrlBank::TableParsing(int nTblPos )
{
	nTblPos = (nTblPos < CONST_TABLE_POS::POS_MAX ? nTblPos + CONST_TABLE_POS::POS_MAX : nTblPos);
	nTblPos = (nTblPos >= CONST_TABLE_POS::POS_MAX ? nTblPos - CONST_TABLE_POS::POS_MAX : nTblPos);
	return nTblPos;
}

int CUnitCtrlBank::GetTablelPos(ZONE_ID cpos)
{
	int nTblPos = GetEntryTablePos();
	if(nTblPos == CONST_TABLE_POS::POS_MAX)
		return CONST_TABLE_POS::POS_MAX;

	nTblPos -= cpos;

	return TableParsing(nTblPos);
}

int CUnitCtrlBank::AMT_GetAutoTblPos(ZONE_ID cpos)
{

	int nTblPos = theProcBank.m_nInspTablePos;
	if(nTblPos == CONST_TABLE_POS::POS_MAX)
		return CONST_TABLE_POS::POS_MAX;

	nTblPos -= cpos;

	return TableParsing(nTblPos);
}


int CUnitCtrlBank::GetEntryTablePos( double dRange /*= 0.02*/ )
{
	if(theDeviceMotion.IsAxisReady(m_nThreadID, AXIS_SVO_TURN_TABLE_T, FALSE) == FALSE)
	{
		// �����̺� �̵� üũ�� ���� �־ ����ÿ� �˶� �߻� 2017/06/07 HSK.
		theProcBank.m_bTeachMoveComplete = FALSE;
		return CONST_TABLE_POS::POS_MAX;
	}
	TEACH_PARAM paramTeach = theTeachBank.GetTeachParam( TEACH_PARAM::WORK_TABLE_T_TO_WORK);
	
	double dDefPos = paramTeach.dPos;
	double dPos;
	double dMotorPos;

	if(theDeviceMotion.GetPosition(m_nThreadID, AXIS_SVO_TURN_TABLE_T, dMotorPos) == FALSE)
		return CONST_TABLE_POS::POS_MAX;

	int nInx;

	for (nInx = 0; nInx < CONST_TABLE_POS::POS_MAX; nInx++)
	{
		dPos = dDefPos + nInx*90;	
		if(dPos >= 360.)
			dPos -=  360.;

		if(theDeviceMotion.CheckAxisPosition(m_nThreadID, AXIS_SVO_TURN_TABLE_T, dPos))
			break;
		
	}
	//2017-07-10,SKCHO, ���m_bTeachMoveComplete = TRUE���Ǿ� �ٸ� ���� ���۽� ���� ��� ������ ����
	if(theProcBank.m_nPresentAxis == AXIS_SVO_TURN_TABLE_T)
	{
		// �����̺� �̵� üũ�� ���� �־ ����ÿ� �˶� �߻� 2017/06/07 HSK.
		theProcBank.m_bTeachMoveComplete = TRUE;
	}
	return nInx;
}

BOOL CUnitCtrlBank::MoveToTblPos(int nTblPos, double dSpeedRatio)	// ���� : Avt Entry ��ġ�� ����.
{
	// ���� : Amt Entry ��ġ�� ����.
	if(nTblPos < CONST_TABLE_POS::POS_1 || nTblPos > CONST_TABLE_POS::POS_4)
		return FALSE;
	AxisMoveParam param;
	TEACH_PARAM	paramT;

	paramT = theTeachBank.GetTeachParam(TEACH_PARAM::WORK_TABLE_T_TO_WORK);

	double dCurPos;
	theDeviceMotion.GetPosition(m_nThreadID, AXIS_SVO_TURN_TABLE_T, dCurPos);
	paramT.dPos += 90.*(double)nTblPos;
	paramT.dPos =  (double)((long)(paramT.dPos*1000.)%360000)/1000.;
	if(dCurPos > paramT.dPos)
		paramT.dPos += 360.;

	paramT.dSpeed *= dSpeedRatio;

	param.idAxis = AXIS_SVO_TURN_TABLE_T;
	param.dAcc = paramT.dAcc;
	param.dDec = paramT.dDec;
	param.dSpeed = paramT.dSpeed;
	param.dTargetPosition = paramT.dPos;


	return theDeviceMotion.AxisMove(m_nThreadID, AXIS_SVO_TURN_TABLE_T, param);
}

BOOL CUnitCtrlBank::MoveToNext(BOOL bAuto,double dSpeedRatio)
{
	if(CheckTeachMoveInterlock(TEACH_PARAM::WORK_TABLE_T_TO_WORK,0.0,bAuto) == FALSE)
		return FALSE;
	int nTblPos = GetEntryTablePos();

	if(nTblPos == CONST_TABLE_POS::POS_MAX)
		return FALSE;

	if(nTblPos >= CONST_TABLE_POS::POS_4)
		nTblPos = CONST_TABLE_POS::POS_1;
	else	
		nTblPos++;

	if(bAuto == FALSE)
	{
		if(nTblPos == CONST_TABLE_POS::POS_1)
		{
			theUnitFunc.SetOutPutIO(Y_SWITCH_1,TRUE);
			theUnitFunc.SetOutPutIO(Y_SWITCH_2,TRUE);
		}
		else if(nTblPos == CONST_TABLE_POS::POS_2)
		{
			theUnitFunc.SetOutPutIO(Y_SWITCH_1,FALSE);
			theUnitFunc.SetOutPutIO(Y_SWITCH_2,TRUE);
		}
		else if(nTblPos == CONST_TABLE_POS::POS_3)
		{
			theUnitFunc.SetOutPutIO(Y_SWITCH_1,TRUE);
			theUnitFunc.SetOutPutIO(Y_SWITCH_2,FALSE);
		}
		else
		{
			theUnitFunc.SetOutPutIO(Y_SWITCH_1,FALSE);
			theUnitFunc.SetOutPutIO(Y_SWITCH_2,FALSE);
		}
	}
	return MoveToTblPos(nTblPos, dSpeedRatio);
}

//2017-11-17,SKCHO, DZONE BLOWER ���(FPCB �ּ� �ʿ�) - �ⱸ�۾� �Ϸ�Ǹ� ����
void CUnitCtrlBank::AMTVacOn(BOOL bVacOn, JIG_ID Jig, JIG_CH Ch)
{
	if (Jig == JIG_ID_A)
	{
		SetOutPutIO(Y_AMT_AJIG_MAIN_VAC_SOL, VAC_ON);

		if(Ch == JIG_CH_1)
		{
			SetOutPutIO(Y_AMT_AJIG_CH1_CELL_VAC_SOL, bVacOn);	
			//2017-12-22,SKCHO JIGBLOW �ɼ��߰�
			if(theConfigBank.m_Option.m_bUseJigBlow == FALSE)
			{
			   SetOutPutIO(Y_AMT_AJIG_CH1_FPCB_VAC_SOL, bVacOn);
			}
		}
		else if(Ch == JIG_CH_2)
		{
			SetOutPutIO(Y_AMT_AJIG_CH2_CELL_VAC_SOL, bVacOn);
			//2017-12-22,SKCHO JIGBLOW �ɼ��߰�
			if(theConfigBank.m_Option.m_bUseJigBlow == FALSE)
			{
			   SetOutPutIO(Y_AMT_AJIG_CH2_FPCB_VAC_SOL, bVacOn);
			}
		}
		else
		{
			SetOutPutIO(Y_AMT_AJIG_CH1_CELL_VAC_SOL, bVacOn);			
			SetOutPutIO(Y_AMT_AJIG_CH2_CELL_VAC_SOL, bVacOn);
			//2017-12-22,SKCHO JIGBLOW �ɼ��߰�
			if(theConfigBank.m_Option.m_bUseJigBlow == FALSE)
			{
				SetOutPutIO(Y_AMT_AJIG_CH1_FPCB_VAC_SOL, bVacOn);
			    SetOutPutIO(Y_AMT_AJIG_CH2_FPCB_VAC_SOL, bVacOn);
			}
		}
	}
	else if (Jig == JIG_ID_B)
	{
		SetOutPutIO(Y_AMT_BJIG_MAIN_VAC_SOL, VAC_ON);

		if(Ch == JIG_CH_1)
		{
			SetOutPutIO(Y_AMT_BJIG_CH1_CELL_VAC_SOL, bVacOn);
			//2017-12-22,SKCHO JIGBLOW �ɼ��߰�
			if(theConfigBank.m_Option.m_bUseJigBlow == FALSE)
			{
				SetOutPutIO(Y_AMT_BJIG_CH1_FPCB_VAC_SOL, bVacOn);
			}
		}
		else if(Ch == JIG_CH_2)
		{
			SetOutPutIO(Y_AMT_BJIG_CH2_CELL_VAC_SOL, bVacOn);
			{
				SetOutPutIO(Y_AMT_BJIG_CH2_FPCB_VAC_SOL, bVacOn);
			}
		}
		else
		{
			SetOutPutIO(Y_AMT_BJIG_CH1_CELL_VAC_SOL, bVacOn);
			SetOutPutIO(Y_AMT_BJIG_CH2_CELL_VAC_SOL, bVacOn);
			//2017-12-22,SKCHO JIGBLOW �ɼ��߰�
			if(theConfigBank.m_Option.m_bUseJigBlow == FALSE)
			{
				SetOutPutIO(Y_AMT_BJIG_CH1_FPCB_VAC_SOL, bVacOn);
				SetOutPutIO(Y_AMT_BJIG_CH2_FPCB_VAC_SOL, bVacOn);
			}		
			
		}
	}
	else if (Jig == JIG_ID_C)
	{
		SetOutPutIO(Y_AMT_CJIG_MAIN_VAC_SOL, VAC_ON);

		if(Ch == JIG_CH_1)
		{
			SetOutPutIO(Y_AMT_CJIG_CH1_CELL_VAC_SOL, bVacOn);
			//2017-12-22,SKCHO JIGBLOW �ɼ��߰�
			if(theConfigBank.m_Option.m_bUseJigBlow == FALSE)
			{
			   SetOutPutIO(Y_AMT_CJIG_CH1_FPCB_VAC_SOL, bVacOn);
			}
		}
		else if(Ch == JIG_CH_2)
		{
			SetOutPutIO(Y_AMT_CJIG_CH2_CELL_VAC_SOL, bVacOn);
			//2017-12-22,SKCHO JIGBLOW �ɼ��߰�
			if(theConfigBank.m_Option.m_bUseJigBlow == FALSE)
			{
			   SetOutPutIO(Y_AMT_CJIG_CH2_FPCB_VAC_SOL, bVacOn);
			}
		}
		else
		{
			SetOutPutIO(Y_AMT_CJIG_CH1_CELL_VAC_SOL, bVacOn);
			SetOutPutIO(Y_AMT_CJIG_CH2_CELL_VAC_SOL, bVacOn);
			//2017-12-22,SKCHO JIGBLOW �ɼ��߰�
			if(theConfigBank.m_Option.m_bUseJigBlow == FALSE)
			{
				SetOutPutIO(Y_AMT_CJIG_CH1_FPCB_VAC_SOL, bVacOn);
				SetOutPutIO(Y_AMT_CJIG_CH2_FPCB_VAC_SOL, bVacOn);
			}
			
			
		}
	}
	else if (Jig == JIG_ID_D)
	{
		SetOutPutIO(Y_AMT_DJIG_MAIN_VAC_SOL, VAC_ON);

		if(Ch == JIG_CH_1)
		{
			SetOutPutIO(Y_AMT_DJIG_CH1_CELL_VAC_SOL, bVacOn);
			//2017-12-22,SKCHO JIGBLOW �ɼ��߰�
			if(theConfigBank.m_Option.m_bUseJigBlow == FALSE)
			{
			  SetOutPutIO(Y_AMT_DJIG_CH1_FPCB_VAC_SOL, bVacOn);
			}
		}
		else if(Ch == JIG_CH_2)
		{
			SetOutPutIO(Y_AMT_DJIG_CH2_CELL_VAC_SOL, bVacOn);
			//2017-12-22,SKCHO JIGBLOW �ɼ��߰�
			if(theConfigBank.m_Option.m_bUseJigBlow == FALSE)
			{
			  SetOutPutIO(Y_AMT_DJIG_CH2_FPCB_VAC_SOL, bVacOn);
			}
		}
		else
		{
			SetOutPutIO(Y_AMT_DJIG_CH1_CELL_VAC_SOL, bVacOn);
			SetOutPutIO(Y_AMT_DJIG_CH2_CELL_VAC_SOL, bVacOn);
			//2017-12-22,SKCHO JIGBLOW �ɼ��߰�
			if(theConfigBank.m_Option.m_bUseJigBlow == FALSE)
			{
				SetOutPutIO(Y_AMT_DJIG_CH1_FPCB_VAC_SOL, bVacOn);
				SetOutPutIO(Y_AMT_DJIG_CH2_FPCB_VAC_SOL, bVacOn);
			}
			
			
		}
	}
}


BOOL CUnitCtrlBank::AMTVacChk(JIG_ID Jig,JIG_CH Ch)
{
	if(theConfigBank.m_Option.m_bUseDryRun)
		return TRUE;

	if (Jig == JIG_ID_A)
	{
		if(Ch == JIG_CH_1)
			return GetInPutIOCheck(X_A_JIG_CH1_VAC_SEN);
		else if(Ch == JIG_CH_2)
			return GetInPutIOCheck(X_A_JIG_CH2_VAC_SEN);
	}
	else if (Jig == JIG_ID_B)
	{
		if(Ch == JIG_CH_1)
			return GetInPutIOCheck(X_B_JIG_CH1_VAC_SEN);
		else if(Ch == JIG_CH_2)
			return GetInPutIOCheck(X_B_JIG_CH2_VAC_SEN);
	}
	else if (Jig == JIG_ID_C)
	{
		if(Ch == JIG_CH_1)
			return GetInPutIOCheck(X_C_JIG_CH1_VAC_SEN);
		else if(Ch == JIG_CH_2)
			return GetInPutIOCheck(X_C_JIG_CH2_VAC_SEN);
	}
	else if (Jig == JIG_ID_D)
	{
		if(Ch == JIG_CH_1)
			return GetInPutIOCheck(X_D_JIG_CH1_VAC_SEN);
		else if(Ch == JIG_CH_2)
			return GetInPutIOCheck(X_D_JIG_CH2_VAC_SEN);
	}

	return TRUE;
}

void CUnitCtrlBank::CellData_SetJIGID(JIG_ID Jig)
{
	CCellTag tagCell;
	CCellInfo* pCellInfo;

	tagCell = theCellBank.GetCellTag(CELL_POS_AZONE_CH1);
	pCellInfo = theCellBank.GetCellInfo(CELL_POS_AZONE_CH1);

	if (tagCell.IsExist())
		pCellInfo->m_JigId = Jig;

	tagCell = theCellBank.GetCellTag(CELL_POS_AZONE_CH2);
	pCellInfo = theCellBank.GetCellInfo(CELL_POS_AZONE_CH2);

	if (tagCell.IsExist())
		pCellInfo->m_JigId = Jig;
}

BOOL CUnitCtrlBank::OriginRetrun(AXIS_ID IdAxis)
{
	return theDeviceMotion.OriginRetrun(m_nThreadID, IdAxis);
}

BOOL CUnitCtrlBank::IsFinishTurnTable()
{
	if(theProcBank.m_bTableTurnStatus[ZONE_ID_A]
	&& theProcBank.m_bTableTurnStatus[ZONE_ID_B]
	&& theProcBank.m_bTableTurnStatus[ZONE_ID_C]
	&&theProcBank.m_bTableTurnStatus[ZONE_ID_D])
		return TRUE;

	return FALSE;
}


void CUnitCtrlBank::TableTurnTimeStart()
{
	CCellInfo* pCell;
	CCellTag tagCell;

	for (int i = 0; i < JIG_CH_MAX; i++)
	{
		// Start�� A���� ������ ȸ���ϸ� B�̹Ƿ� End�� B���� ���´� [6/11/2013 OSC]
		tagCell = theCellBank.GetCellTag(ZONE_ID_A, (JIG_CH)i);
		if(tagCell.IsExist())
		{
			pCell = theCellBank.GetCellInfo(tagCell);
			GetLocalTime(&pCell->cellTime.systmTableTurn_StartTime);
		}
	}
}

void CUnitCtrlBank::TableTurnTimeEnd()
{
	CCellInfo* pCell;
	CCellTag tagCell;

	for (int i = 0; i < JIG_CH_MAX; i++)
	{
		// Start�� A���� ������ ȸ���ϸ� B�̹Ƿ� End�� B���� ���´� [6/11/2013 OSC]
		tagCell = theCellBank.GetCellTag(ZONE_ID_B, (JIG_CH)i);
		if(tagCell.IsExist())
		{
			pCell = theCellBank.GetCellInfo(tagCell);
			GetLocalTime(&pCell->cellTime.systmTableTurn_EndTime);
		}
	}
}

void CUnitCtrlBank::AMT_PGSetZone()
{
	JIG_ID jigId[ZONE_ID_MAX] = {
		AMT_GetAutoJigID(ZONE_ID_A), AMT_GetAutoJigID(ZONE_ID_B), 
		AMT_GetAutoJigID(ZONE_ID_C), AMT_GetAutoJigID(ZONE_ID_D)
	};

	// ���� ������ Set �Ѵ� [7/1/2013 OSC]
	// ������ ��� ���� ��ŵ ���� �̻�� 2017/04/26 HSK.
	/*
	if (theConfigBank.m_Option.m_bUseRepeatInsp == FALSE)
	{
		for (int j = 0; j < JIG_CH_MAX; j++)
			theSocketInterFace.SendMassageToPatternGenerator(SET_ZONE_A, jigId[JIG_ID_A], (JIG_CH)j);
	}
	*/
	for (int j = 0; j < JIG_CH_MAX; j++)
		theSocketInterFace.SendMassageToPatternGenerator(SET_ZONE_A, jigId[JIG_ID_A], (JIG_CH)j);

	for (int j = 0; j < JIG_CH_MAX; j++)
		theSocketInterFace.SendMassageToPatternGenerator(SET_ZONE_B, jigId[JIG_ID_B], (JIG_CH)j);

	for (int j = 0; j < JIG_CH_MAX; j++)
		theSocketInterFace.SendMassageToPatternGenerator(SET_ZONE_C, jigId[JIG_ID_C], (JIG_CH)j);

	for (int j = 0; j < JIG_CH_MAX; j++)
		theSocketInterFace.SendMassageToPatternGenerator(SET_ZONE_D, jigId[JIG_ID_D], (JIG_CH)j);
}

void CUnitCtrlBank::ContactStart(JIG_CH Ch)
{
	if(theConfigBank.m_Option.m_bUseContact)
		SendPGMessage(CELL_LOADING, ZONE_ID_A, Ch);	
}

void CUnitCtrlBank::ContactStart_BZone(JIG_CH Ch)
{
	if(theConfigBank.m_Option.m_bUseContact)
		SendPGMessage(CELL_LOADING, ZONE_ID_B, Ch);	
}

void CUnitCtrlBank::TurnOff(JIG_CH Ch)
{
	SendPGMessage(TURN_OFF, ZONE_ID_D, Ch);	
}

void CUnitCtrlBank::DZoneOff(JIG_CH Ch)
{
	SendPGMessage(DZONE_OFF, ZONE_ID_D, Ch);	
}

void CUnitCtrlBank::Reset(JIG_CH Ch)
{
	SendPGMessage(RESET, ZONE_ID_A, Ch);	
}
void CUnitCtrlBank::Reset_BZone(JIG_CH Ch)
{
	SendPGMessage(RESET, ZONE_ID_B, Ch);	
}


CLASS_CELL CUnitCtrlBank::CellData_GetLastClass(CELL_POS pos)
{
	// 2017.1.11 bgkim
	CCellTag tagCell;
	CCellInfo* pCellInfo;

	tagCell = theCellBank.GetCellTag(pos);
	pCellInfo = theCellBank.GetCellInfo(pos);

	if(tagCell.IsExist()==FALSE)
		return NONE_CELL;

	// CELL�� UNLOADING �ϴ� �͸� �̶� �ּ�. �����ϴ� �κп� ���� �߰�. 17/04/24 HSK
//	if(theConfigBank.m_Option.m_bUseJudgeOK)
//		return GOOD_CELL;
	
	return pCellInfo->m_LastClass;
}

//2018-07-25,SKCHO, �ű��Լ� ����
void CUnitCtrlBank::CIM_CellInfoRequest(CELL_POS pCellPos)
{
	CCellInfo *pCell;
	pCell = theCellBank.GetCellInfo((CELL_POS)(pCellPos));
	if(pCell)
	{
		if(pCellPos == CELL_POS_LD_TR_CH1 || pCellPos == CELL_POS_LD_TR_CH2)
		{
			pCell->m_JigId = (JIG_ID)4;
		}
		theSocketInterFace.SendCmdCellToDataPC(EQUIP_SET_CELL_INFORMATION_REQUEST, pCell);
	}	

}

//2018wjp
void CUnitCtrlBank::CIM_CellInfoRequestEx(CString strCellID)
{
	if(!strCellID.IsEmpty())
	{
		CCellInfo *pCell;
		pCell=new CCellInfo();
		pCell->m_JigId=(JIG_ID)0;
		pCell->m_JigCh=(JIG_CH)0;
		pCell->m_strInnerID = "";
		pCell->m_strCellID=strCellID;
		theSocketInterFace.SendCmdCellToDataPC(EQUIP_SET_CELL_INFORMATION_REQUEST, pCell);
		delete pCell;
	}	
}

void CUnitCtrlBank::CIM_CH1_CellInfoRequest()
{
	CCellInfo *pCell;
	pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_AZONE_CH1));
	if(pCell)
	{
		theSocketInterFace.SendCmdCellToDataPC(EQUIP_SET_CELL_INFORMATION_REQUEST, pCell);
	}	
}

void CUnitCtrlBank::CIM_CH2_CellInfoRequest()
{
	CCellInfo *pCell;
	pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_AZONE_CH2));
	if(pCell)
	{
		theSocketInterFace.SendCmdCellToDataPC(EQUIP_SET_CELL_INFORMATION_REQUEST, pCell);
	}	
}

BOOL CUnitCtrlBank::CIM_CellTrackIn(CELL_POS Pos)
{
	CCellInfo *pCell;

	pCell = theCellBank.GetCellInfo((CELL_POS)Pos);
	if(pCell)
	{
		if( pCell->m_strCellID != _T("FAIL") )
		//if( pCell->m_strCellID != _T("UV_FAIL") )  //2017-04-05,skcho, �ڱ״н����� FAIL �� �����͸� ��
		{
			pCell->bTrackInChk = TRUE;
			theSocketInterFace.SendCmdCellToDataPC(EQUIP_SET_CELL_PROCESS_START_LOAD, pCell);		
			return TRUE;
		}
		else
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[AZONE] CIM_TrackIn - NONE CELL ID"));
		}
	}
	else
	{
		theLog[LOG_SEQUENCE].AddBuf( _T("[AZONE] CIM_TrackIn - NONE CELL POSITION"));
	}
	return FALSE;
}


BOOL CUnitCtrlBank::CellData_TrackInChk(CELL_POS Pos)
{
	if ((theConfigBank.m_Option.m_bUseDryRun == TRUE))
		return TRUE;

	CCellInfo *pCell;

	pCell = theCellBank.GetCellInfo((CELL_POS)Pos);
	if(pCell)
		return	pCell->bTrackInChk;

	return FALSE;
}

BOOL CUnitCtrlBank::CellData_CellInfoRequestChk(CELL_POS Pos)
{
	if ((theConfigBank.m_Option.m_bUseDryRun == TRUE))
		return TRUE;

	CCellInfo *pCell;

	pCell = theCellBank.GetCellInfo((CELL_POS)Pos);
	if(pCell)
	{
		return	pCell->bCellInfoRequest;
	}

	return FALSE;
}
void CUnitCtrlBank::CIM_CellAllInspectionEnd()
{
	CCellInfo *pCell;
	for (int i = 0; i < JIG_CH_MAX; i++)
	{
		pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_DZONE_CH1 + i));
		if(pCell)
		{
			//theSocketInterFace.SendCmdCellToDataPC(EQUIP_SET_CELL_ENDINS, pCell);
			//20170201 byskcho
			//CIM QUAL RCMD���� 21�� �ƴϸ� 609���� ���ϳ� DATA_PC�� �����ʹ� �����ϰ� 609 ��ƾ���� �������� ����
			if( pCell->m_strCellID != _T("FAIL")  && pCell->m_strCellID != _T("") )
			//if( pCell->m_strCellID != _T("UV_FAIL")  && pCell->m_strCellID != _T("") ) //2017-04-05,skcho, �ڱ״н����� FAIL �� �����͸� ��
			{
				// 609�� ���� �޶�� �Ѵ�.
				theSocketInterFace.SendCmdCellToDataPC(EQUIP_SET_INSPECTION_RESULT_REPORT, pCell);
			}
			
		}
	}
}

void CUnitCtrlBank::CIM_CellTrackOut()
{
	CCellInfo *pCell;
	for(int i = CELL_POS_UD_TR_CH1; i < CELL_POS_UD_STAGE_CH1; i++)
	{
		pCell = theCellBank.GetCellInfo((CELL_POS)i);
		if(pCell)
		{
			if( pCell->m_strCellID != _T("FAIL")  && pCell->bTrackOutChk)
			//if( pCell->m_strCellID != _T("UV_FAIL")  && pCell->bTrackOutChk) //2017-04-05,skcho, �ڱ״н����� FAIL �� �����͸� ��
			{
				theSocketInterFace.SendCmdCellToDataPC(EQUIP_SET_CELL_PROCESS_COMPLETE_UNLOAD, pCell);
			}
		}
	}
}


void CUnitCtrlBank::CIM_CellClear()
{
	CCellInfo *pCell;
	for(int i = CELL_POS_UD_TR_CH1; i < CELL_POS_UD_STAGE_CH1; i++)
	{
		pCell = theCellBank.GetCellInfo((CELL_POS)i);
		if(pCell)
		{
			theSocketInterFace.SendCmdCellToDataPC(EQUIP_SET_CELL_DATA_CLEAR_REQUEST, pCell);
		}
	}
}
//2017-03-02, skcho, ���� ���� ���� ���� �Լ�
//���� ȭ�鿡�� zone ��ư�� ������ ��
void CUnitCtrlBank::CIM_CellLossReport(CELL_POS Pos)
{
	CCellInfo *pCell;
	
	pCell = theCellBank.GetCellInfo((CELL_POS)Pos);
	if(pCell)
	{		
		pCell->sAbstract = _T("1");  
		if( pCell->m_strCellID != _T("FAIL")  && pCell->bTrackOutChk)
		//if( pCell->m_strCellID != _T("UV_FAIL")  && pCell->bTrackOutChk)
		{
			theSocketInterFace.SendCmdCellToDataPC(EQUIP_SET_CELL_PROCESS_COMPLETE_UNLOAD, pCell);
		}
	}
	
}
void CUnitCtrlBank::CIM_CellExistInMachine()
{
	CCellTag tag;
	BOOL bExist = FALSE;

	for (int i = CELL_POS_LD_CONV; i < CELL_POS_MAX; i++)
	{
		tag = theCellBank.GetCellTag((CELL_POS)i);
		if(tag.IsExist())
		{
			bExist = TRUE;
			break;
		}
	}

	theSocketInterFace.SendCmdToDataPC(EQUIP_SET_CELL_EXIST, bExist ? EQST_RUN_IS_CELL:EQST_RUN_NOT_CELL);
}
int CUnitCtrlBank::CIM_CellInfoRequestCheck(CELL_POS Pos)
{
	int nResult = 0;
	if (theConfigBank.m_Option.m_bUseCellInfoSkip == TRUE || theConfigBank.m_Option.m_bUseDryRun == TRUE)
	{
		return 1;
	}

	CCellTag tagCell;
	CCellInfo* pCellInfo;

	tagCell = theCellBank.GetCellTag(Pos);

	//2018-07-30,SKCHO, CELL������ ������ 0�� ����
	if(tagCell.IsExist()==FALSE)
	{	
		return 0;
	}

	pCellInfo = theCellBank.GetCellInfo(Pos);
	//CELL INFO ���� 0,41,42
	if(pCellInfo->m_strCellInfoResult == _T("-1"))
	{
		nResult =  0;
	}
	else if (pCellInfo->m_strCellInfoResult == _T("0") || pCellInfo->m_strCellInfoResult == _T("41") ||pCellInfo->m_strCellInfoResult == _T("42") )
	{
		nResult =  1;
	}
	else
	{
		nResult =  2;
	}

	return nResult;
}

int CUnitCtrlBank::CIM_SetRCMD(CELL_POS Pos)
{
	int nResult = -1;
	

	CCellTag tagCell;
	CCellInfo* pCellInfo;

	tagCell = theCellBank.GetCellTag(Pos);
	pCellInfo = theCellBank.GetCellInfo(Pos);

	//ȭ��˻縦 ������.609 ����
	if ((theConfigBank.m_Option.m_bUseRCMD == FALSE) || (theConfigBank.m_Option.m_bUseDryRun == TRUE))
	{
		//ȭ��˻� ���� �÷���
		pCellInfo->m_bIsInspectionSkip = FALSE;
		nResult= 1;
		return nResult;
	}

	if(Pos == CELL_POS_AZONE_CH1)
	{
		//--theLog[LOG_SEQUENCE].AddBuf( _T("[RCMD_CHECK] RCMD %s, CELL ID %s - RCMD CELL ID CH1 %s"),theProcBank.m_strRCMD,pCellInfo->m_strCellID,theProcBank.m_strRCMDCellIDCh1);
		if(theProcBank.m_strRCMDCellIDCh1.GetLength() > 0)
		{
			if (theProcBank.m_strRCMDCellIDCh1 != pCellInfo->m_strCellID )
			{
				//2017-05-22,skcho track in ������ ��ID�� �ٸ� ��� �˻� skip
				pCellInfo->m_bIsInspectionSkip = TRUE;  //609 ���� ����
				nResult = 0;
			}
			else
			{
				if (theProcBank.m_strRCMD == _T("21"))// �˻� OK
				{
					pCellInfo->m_bIsInspectionSkip = FALSE;
					nResult = 1;
				}
				else
				{
					pCellInfo->m_bIsInspectionSkip = TRUE;  //609 ���� ����
					nResult = 2;
				}
			}
		}
	}
	else
	{
		//--theLog[LOG_SEQUENCE].AddBuf( _T("[RCMD_CHECK] RCMD %s, CELL ID %s - RCMD CELL ID CH2 %s"),theProcBank.m_strRCMD,pCellInfo->m_strCellID,theProcBank.m_strRCMDCellIDCh2);
		if(theProcBank.m_strRCMDCellIDCh2.GetLength() > 0)
		{
			if (theProcBank.m_strRCMDCellIDCh2 != pCellInfo->m_strCellID )
			{
				//2017-05-22,skcho track in ������ ��ID�� �ٸ� ��� �˻� skip
				pCellInfo->m_bIsInspectionSkip = TRUE;  //609 ���� ����
				nResult = 0;
			}
			else
			{
				if (theProcBank.m_strRCMD == _T("21"))// �˻� OK
				{
					pCellInfo->m_bIsInspectionSkip = FALSE;
					nResult = 1;
				}
				else
				{
					pCellInfo->m_bIsInspectionSkip = TRUE;  //609 ���� ����
					nResult = 2;
				}
			}
		}
	}
	/*else if(theProcBank.m_strRCMD == _T("22"))// �˻� NG
	{
		pCellInfo->m_bIsInspectionSkip = TRUE;
		return TRUE;
	}
	else if((theProcBank.m_strRCMD == _T("23")) || (theProcBank.m_strRCMD == _T("24")))
	{
		pCellInfo->m_bIsInspectionSkip = TRUE;
		return TRUE;
	}*/

	return nResult;
}

BOOL CUnitCtrlBank::Get_InspectSkip(CELL_POS Pos)
{
	CCellTag tagCell;
	CCellInfo* pCellInfo;

	tagCell = theCellBank.GetCellTag(Pos);
	pCellInfo = theCellBank.GetCellInfo(Pos);

	if (tagCell.IsExist() == FALSE)
		return TRUE;

	return pCellInfo->m_bIsInspectionSkip;
}

void CUnitCtrlBank::Set_InspectSkip(CELL_POS Pos, BOOL bSkip)
{
	CCellTag tagCell;
	CCellInfo* pCellInfo;

	tagCell = theCellBank.GetCellTag(Pos);
	pCellInfo = theCellBank.GetCellInfo(Pos);

	if (tagCell.IsExist() == FALSE)
		return;

	pCellInfo->m_bIsInspectionSkip = bSkip;
}

BOOL CUnitCtrlBank::Get_RCMDResult(CELL_POS Pos)
{
	CCellTag tagCell;
	CCellInfo* pCellInfo;

	tagCell = theCellBank.GetCellTag(Pos);
	pCellInfo = theCellBank.GetCellInfo(Pos);

	if (tagCell.IsExist() == FALSE)
		return TRUE;

	return pCellInfo->m_bRCMDResult;
}

void CUnitCtrlBank::Set_RCMDResult(CELL_POS Pos, BOOL bSkip)
{
	CCellTag tagCell;
	CCellInfo* pCellInfo;

	tagCell = theCellBank.GetCellTag(Pos);
	pCellInfo = theCellBank.GetCellInfo(Pos);

	if (tagCell.IsExist() == FALSE)
		return;

	pCellInfo->m_bRCMDResult = bSkip;
}

void CUnitCtrlBank::CIM_TRAYProcess(CString strCEID,CString strPort)
{
	CString strBuf1,strBuf2;
	/*if(strPort == _T("1"))
		strBuf1 = _T("GOOD IN PORT");
	else if(strPort == _T("2"))
		strBuf1 = _T("GOOD OUT PORT");*/

	if(strCEID == _T("350"))
	{
		strBuf2 = _T("���Կ�û(350)");	
		::GetLocalTime(&theProcBank.m_AGVLoadReqtime);
		
		//2017-08-30,.SKCHO, ��û�� �ð� üũ ����
		theProcBank.m_nAGVDelayStep =1;
	}
	else if(strCEID == _T("351"))
	{
		strBuf2 = _T("���ԿϷ�(351)");

		//2017-08-30,.SKCHO, �Ϸ�� �������·� ����
		theProcBank.m_nAGVDelayStep =4;
	}
	else if(strCEID == _T("352"))
	{
		//2017-12-11,SKCHO ����
		//2017-11-27,SKCHO, TRAY ID�� ������ ���� ����.
		//--if(theProcBank.m_strCloseTrayID == _T(""))
		//if(theProcBank.m_strClose352TrayID == _T(""))
		//{
		//	return;
		//}


		strBuf2 = _T("������(352)");
		::GetLocalTime(&theProcBank.m_AGVUnLoadReqtime);

		//2017-08-30,.SKCHO, ��û�� �ð� üũ ����
		theProcBank.m_nOutAGVDelayStep =1;
	}
	else if(strCEID == _T("353"))
	{
		//2017-11-27,SKCHO, TRAY ID�� ������ ���� ����.
		if(theProcBank.m_strClose353TrayID == _T(""))
		{
			return;
		}

		strBuf2 = _T("EMPTY,����Ϸ�(353)");

		//2017-08-30,.SKCHO, �Ϸ�� �������·� ����
		theProcBank.m_nOutAGVDelayStep =4;
	}

	//2017-07-04,SKCHO, AGV ���� ǥ��
	if(strPort == _T("1") || strPort == _T("4"))  //2017-07-17,SKCHO,OK BUFFER PORT�� Ʈ���̰� ���� 4�� ����
	{
		strBuf1 = _T("GOOD IN PORT");
		theProcBank.m_strLoadAGVTC = strBuf2;		
	}
	else if(strPort == _T("2"))
	{
		strBuf1 = _T("GOOD OUT PORT");
		theProcBank.m_strUnLoadAGVTC = strBuf2;
	}
	
	theLog[LOG_AGV].AddBuf( _T("%s %s"), strBuf1 , strBuf2);
	theSocketInterFace.SendCmdTrayToDataPC(EQUIP_SET_PORT_PARAMETER_MANAGEMENT, strCEID , strPort);
}


void CUnitCtrlBank::CIM_TRAYProcessReport()
{
	theSocketInterFace.SendCmdTrayToDataPC(EQUIP_SET_PORT_PARAMETER_REPORT , _T(""), _T("1"));
	theSocketInterFace.SendCmdTrayToDataPC(EQUIP_SET_PORT_PARAMETER_REPORT , _T(""), _T("2"));
	theSocketInterFace.SendCmdTrayToDataPC(EQUIP_SET_PORT_PARAMETER_REPORT , _T(""), _T("3"));
}

void CUnitCtrlBank::Zone_Switching()
{
	if(theProcBank.m_nInspTablePos == CONST_TABLE_POS::POS_1)
	{
		theUnitFunc.SetOutPutIO(Y_SWITCH_1,TRUE);
		theUnitFunc.SetOutPutIO(Y_SWITCH_2,TRUE);
	}
	else if(theProcBank.m_nInspTablePos == CONST_TABLE_POS::POS_2)
	{
		theUnitFunc.SetOutPutIO(Y_SWITCH_1,FALSE);
		theUnitFunc.SetOutPutIO(Y_SWITCH_2,TRUE);
	}
	else if(theProcBank.m_nInspTablePos == CONST_TABLE_POS::POS_3)
	{
		theUnitFunc.SetOutPutIO(Y_SWITCH_1,TRUE);
		theUnitFunc.SetOutPutIO(Y_SWITCH_2,FALSE);
	}
	else
	{
		theUnitFunc.SetOutPutIO(Y_SWITCH_1,FALSE);
		theUnitFunc.SetOutPutIO(Y_SWITCH_2,FALSE);
	}
}


void CUnitCtrlBank::SetCZoneResultSkip(JIG_CH ch)
{
	CCellTag tagCell;
	CCellInfo* pCellInfo;

	if (ch == JIG_CH_1)
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_CZONE_CH1);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_CZONE_CH1);
	}
	else
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_CZONE_CH2);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_CZONE_CH2);
	}

	if(!tagCell.IsExist())  //2017-04-10,skcho, ������ ���� null�̾ ����
	{
		return;
	}
	// ��� �ϴ� ��쿡�� ��ŵ�� ��찡 �ִ� 2017/05/02 HSK.
//	if (!theConfigBank.m_Option.m_bUseVIInsp)
//	{
		pCellInfo->m_Zone_C.CZoneDefect = _T("SKIP");
		pCellInfo->m_Zone_C.CZoneClass = BIN2_CELL;
		pCellInfo->m_Zone_C.VIClass = BIN2_CELL;
		pCellInfo->m_Zone_C.VIDefect = _T("SKIP");
		pCellInfo->m_Zone_C.InspVIState = eSTATE_Insp_End;

		//2017-0-20,skcho, �˻� ��ŵ �ɼ��̸�
		pCellInfo->m_bIsInspectionSkip = TRUE;
//	}
}

BOOL CUnitCtrlBank::JigSkipCheck()
{
	if (theConfigBank.m_Option.m_bUseASkip == FALSE
		&& theConfigBank.m_Option.m_bUseBSkip == FALSE
		&& theConfigBank.m_Option.m_bUseCSkip == FALSE
		&& theConfigBank.m_Option.m_bUseDSkip == FALSE)
	{
		return FALSE;
	}

	JIG_ID JigID;
	JigID = AMT_GetAutoJigID(ZONE_ID_A);	

	if (theConfigBank.m_Option.m_bUseASkip)
	{
		if (JigID == JIG_ID_A)
			return TRUE;
	}
	if (theConfigBank.m_Option.m_bUseBSkip)
	{
		if (JigID == JIG_ID_B)
			return TRUE;
	}
	if (theConfigBank.m_Option.m_bUseCSkip)
	{
		if (JigID == JIG_ID_C)
			return TRUE;
	}
	if (theConfigBank.m_Option.m_bUseDSkip)
	{
		if (JigID == JIG_ID_D)
			return TRUE;
	}

	return FALSE;
}

//2017-02-06, byskcho , Ʈ���� �Ѳ� ����/�ݱ� ����
/*void CUnitCtrlBank::CIM_CarrierRelease()  //CEID 256 ����
{ 
	int nResult[2] = {0};
	//ó�� Ʈ���̿� ������ Ʈ���̿����� ����
	nResult[0] = theProcBank.m_TrayBank.GetTrayInfo(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_OUT_LIFT)->GetTrayCount();
	nResult[1] = (int)theProcBank.m_TrayBank.GetTrayInfo(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_OUT_LIFT)->GetTrayFull();
	if( nResult[0] == 1 || nResult[1] == 1)
	{
		theProcBank.m_TrayBank.SetCarrierInfo(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_OUT_LIFT,TRAY_RELEASE);
		theSocketInterFace.SendCmdCarrierAssignToDataPC(EQUIP_SET_CARRIER_RELEASE_REQUEST,NULL);
	}
}

//2017-02-06, byskcho , Ʈ���� ����� ����
void CUnitCtrlBank::CIM_CarrierAssign()//CEID 260 ����
{
	int nResult[2] = {0};
	//ó�� Ʈ���̿� ������ Ʈ���̸� ������ ������ ����
	nResult[0] = theProcBank.m_TrayBank.GetTrayInfo(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_OUT_LIFT)->GetTrayCount();
	nResult[1] = (int)theProcBank.m_TrayBank.GetTrayInfo(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_OUT_LIFT)->GetTrayFull();
	//ó�� Ʈ���̴� ������ ���� �ϰ� ����� ���� ���� �Ѵ�. DCR ��ġ�� OUT_LIFT�� �����Ƿ�
	//Ʈ���̰� FULL�� �Ǳ� ������ ����� ����
	if( nResult[0] == 1 || nResult[1] != 1)
	{
	   theProcBank.m_TrayBank.SetCarrierInfo(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_OUT_LIFT,TRAY_ASSIGN);
	   theSocketInterFace.SendCmdCarrierAssignToDataPC(EQUIP_SET_CARRIER_ASSIGN_REQUEST,NULL);
	}
}*/
/*
//2017-02-06, byskcho , Ʈ���� �Ѳ� ����/�ݱ� ����
void CUnitCtrlBank::CIM_CarrierRelease()  //CEID 262 ����
{ 
	static int nTrayAlignCnt = 0;
	int nResult[2] = {0};
	int nBuff = 0;
	
	//ó�� Ʈ���̿� ������ Ʈ���̿����� ����
	//nResult[0] = theProcBank.m_TrayBank.GetTrayInfo(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_IN_ALIGN)->GetTrayCount();
	//nResult[1] = (int)theProcBank.m_TrayBank.GetTrayInfo(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_IN_ALIGN)->GetTrayFull();
	
	// nResult[0] = ���� Good In Align�� ���� Ʈ���� ����.
	// nResult[1] = Ʈ���� TR�� ���� ���� �Ѳ� Ʈ���̸� ������ ��� �÷��� ����.
	nBuff =  theProcBank.GetGoodAlignCount();
	nResult[0] = nBuff;
	if(theConfigBank.m_Option.m_bUseLotEnd  || theConfigBank.m_UnloadRunInfo.GetJobChangeType() == JOBCHANGE_CASE1)  //2017-03-25,skcho,PPID�� �ٸ���� ���� �˻���ǰ lot end ó��
	{
		if(theProcBank.m_bCarrierAssign)
		{
			nResult[1] = TRUE;	
		}
		else
		{
			nResult[1] = FALSE;	
		}
	}
	else
	{
		nResult[1] = (nBuff > theConfigBank.m_UnloadRunInfo.GetTrayLotQtyInt() ) ? TRUE : FALSE ;	
	}		
	// ����ο� ���� Ʈ���� ������ 1�� ��� ���� �Ѳ� ���� �Ѵ�.
	// Ʈ���� TR�� ���� Ʈ���̿� ���� �ϳ��� �ȴ�� ���� ��쳪 
	// ����� ī��Ʈ�� �ƽ� Ʈ���� ���� ���� Ŭ ��� ���� �Ѳ� ���� �Ѵ�.
	if( nResult[0] == 1 || nResult[1] == TRUE)
	{
		theLog[LOG_SEQUENCE_TRAY].AddBuf(_T("Release Good Align Count : %d , Tray Max Over : %d, Tray ID : %s"), nResult[0],nResult[1],theProcBank.m_strTrayID);

		theProcBank.m_TrayBank.SetCarrierInfo(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_IN_ALIGN,TRAY_RELEASE);
		theSocketInterFace.SendCmdCarrierAssignToDataPC(EQUIP_SET_CARRIER_RELEASE_REQUEST,NULL);

		if(nResult[1] == TRUE)
		{
			theProcBank.ResetGoodAlignCount();
			theProcBank.ResetSubCarrierID();  //�Ѳ� ���� �� ĳ���� ���̵� �ʱ�ȭ
			theLog[LOG_SEQUENCE_TRAY].AddBuf(_T("Release Good Align Count Reset : %d"),  theProcBank.GetGoodAlignCount());
		}
	}
}
*/
//Ʈ���� �Ѳ� ���� ���� 2017/05/03 HSK.
BOOL CUnitCtrlBank::CIM_CarrierRelease()  //CEID 262 ����
{ 
	BOOL bResult = FALSE;
	int nResult = 0;
	
	nResult = theProcBank.GetGoodAlignCount();
		
	if( nResult == 1)
	{
		//2017-07-04, 351���� TRAY ID�� �ѱ�, SetCarrierInfo()�Լ����� ��ġ ����
		theProcBank.m_strTrayOpenRelease = theProcBank.m_strTrayID;
		
		CIM_TRAYProcess(_T("351"),_T("1"));		// 351����(Ʈ����ID �������� ����)�� Release������. 2017-05-13, JSJUNG

		//���� �Ѳ����� ������ ��Ÿ���� ����.2017/05/04 HSK.
		theProcBank.m_bReleaseTrayChk = TRUE;

		theLog[LOG_SEQUENCE_TRAY].AddBuf(_T("Release Report Tray ID : %s"), theProcBank.m_strTrayID);

		theProcBank.m_TrayBank.SetCarrierInfo(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_IN_ALIGN,TRAY_RELEASE);
	
	    bResult = TRUE;
	}
	else
	{
		bResult = FALSE;
	}

	return bResult;
}
/*
//2017-02-06, byskcho , Ʈ���� ����� ����
void CUnitCtrlBank::CIM_CarrierAssign()//CEID 260 ����
{
	int nResult[2] = {0};
	int nBuff = 0;
	//ó�� Ʈ���̿� ������ Ʈ���̸� ������ ������ ����
	//nResult[0] = theProcBank.m_TrayBank.GetTrayInfo(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_IN_ALIGN)->GetTrayCount();
	//nResult[1] = (int)theProcBank.m_TrayBank.GetTrayInfo(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_IN_ALIGN)->GetTrayFull();
	//ó�� Ʈ���̴� ������ ���� �ϰ� ����� ���� ���� �Ѵ�. DCR ��ġ�� OUT_LIFT�� �����Ƿ�
	//Ʈ���̰� FULL�� �Ǳ� ������ ����� ����

	// nResult[0] = ���� Good In Align�� ���� Ʈ���� ����.
	// nResult[1] = Ʈ���� TR�� ���� ���� �Ѳ� Ʈ���̸� ������ ��� �÷��� ����.
	
	nBuff =  theProcBank.GetGoodAlignCount();
	nResult[0] = nBuff;
	
	if(theConfigBank.m_Option.m_bUseLotEnd || theConfigBank.m_UnloadRunInfo.GetJobChangeType() == JOBCHANGE_CASE1)  //2017-03-25,skcho,PPID�� �ٸ���� ���� �˻���ǰ lot end ó��)
	{
		if(theProcBank.m_bCarrierAssign)
			nResult[1] = TRUE;
		else
			nResult[1] = (nBuff > theConfigBank.m_UnloadRunInfo.GetTrayLotQtyInt() ) ? TRUE : FALSE ;
	}
	else
		nResult[1] = (nBuff > theConfigBank.m_UnloadRunInfo.GetTrayLotQtyInt() ) ? TRUE : FALSE;

	// Align�� Ʈ���̰� ������ �Ѳ��� �ƴ� ��� ����� ���� �Ѵ�.
	if( nResult[0] > 0 && nResult[1] != TRUE)
	{
	   theLog[LOG_SEQUENCE_TRAY].AddBuf(_T("Assign Good Align Count : %d , Tray Max Over : %d, Tray ID : %s"), nResult[0],nResult[1],theProcBank.m_strTrayID);
	   theProcBank.m_TrayBank.SetCarrierInfo(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_IN_ALIGN,TRAY_ASSIGN);
	   theSocketInterFace.SendCmdCarrierAssignToDataPC(EQUIP_SET_CARRIER_ASSIGN_REQUEST,NULL);
	}
}
*/
// Assign ���� ���� �Ѳ��� Assign 2017/05/03 HSK. 
void CUnitCtrlBank::CIM_CarrierAssign()//CEID 260 ����
{
	theLog[LOG_SEQUENCE_TRAY].AddBuf(_T("Assign Report Tray ID : %s"), theProcBank.m_strTrayID);
	theProcBank.m_TrayBank.SetCarrierInfo(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_IN_ALIGN,TRAY_ASSIGN);
	
}

void CUnitCtrlBank::CIM_CellLot_Info_Request()
{
	CCellInfo *pCell;
	pCell = theCellBank.GetCellInfo((CELL_POS)CELL_POS_UD_ROBOT);
	if(pCell)
	{
		theSocketInterFace.SendCmdCellToDataPC(EQUIP_SET_CELL_LOT_INFORMATION_REQUEST, pCell);
	}	
}
void CUnitCtrlBank::CIM_Attribute_Request()
{	
	theSocketInterFace.SendCmdCellToDataPC(EQUIP_SET_ATTRIBUTEDATA_REQUEST, NULL);	
}

//2018wjp
void CUnitCtrlBank::CIM_CELLID_Info_Request()
{
	//Tray Id
	theProcBank.m_strTrayOpenRelease = theProcBank.m_strTrayID;
	
	theLog[LOG_SEQUENCE_TRAY].AddBuf(_T("Get Cell ID From Tray ID : %s"), theProcBank.m_strTrayID);

	theProcBank.m_TrayBank.SetCarrierInfo(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_IN_ALIGN,TRAY_CELLID);
}

BOOL CUnitCtrlBank::CIM_CarrierReportCheck()
{
	BOOL bResult = FALSE;
	//2017-05-15,skcho,timeout�� ��������� �����ϱ� ���ؼ� ����- cim qual
	//if(theProcBank.m_strS3F115_ReplyCode == _T("PASS") )
	if(theProcBank.m_strS3F115_ReplyCode != _T("") )
	{
		bResult = TRUE;
	}
	return bResult;
}

void CUnitCtrlBank::SetChSkip()
{	
	CCellInfo *pCell;
	pCell = theCellBank.GetCellInfo(CELL_POS_LDROBOT);

	if (theConfigBank.m_Option.m_bUseASkip)
	{
		theProcBank.m_bChSkip[0] = TRUE;
		theProcBank.m_bChSkip[1] = TRUE;
	}
	else
	{
		theProcBank.m_bChSkip[0] = theConfigBank.m_Option.m_bUseACh1Skip;
		theProcBank.m_bChSkip[1] = theConfigBank.m_Option.m_bUseACh2Skip;
	}
	if (theConfigBank.m_Option.m_bUseBSkip)
	{
		theProcBank.m_bChSkip[2] = TRUE;
		theProcBank.m_bChSkip[3] = TRUE;
	}
	else
	{
		theProcBank.m_bChSkip[2] = theConfigBank.m_Option.m_bUseBCh1Skip;
		theProcBank.m_bChSkip[3] = theConfigBank.m_Option.m_bUseBCh2Skip;
	}
	if (theConfigBank.m_Option.m_bUseCSkip)
	{
		theProcBank.m_bChSkip[4] = TRUE;
		theProcBank.m_bChSkip[5] = TRUE;
	}
	else
	{
		theProcBank.m_bChSkip[4] = theConfigBank.m_Option.m_bUseCCh1Skip;
		theProcBank.m_bChSkip[5] = theConfigBank.m_Option.m_bUseCCh2Skip;
	}
	if (theConfigBank.m_Option.m_bUseDSkip)
	{
		theProcBank.m_bChSkip[6] = TRUE;
		theProcBank.m_bChSkip[7] = TRUE;
	}
	else
	{
		theProcBank.m_bChSkip[6] = theConfigBank.m_Option.m_bUseDCh1Skip;
		theProcBank.m_bChSkip[7] = theConfigBank.m_Option.m_bUseDCh2Skip;
	}
	for (int i = theProcBank.m_nChSkipCount; i < 8; i++)
	{
		if (theProcBank.m_bChSkip[i] == FALSE)
		{
			if (theProcBank.m_nChSkipCount == 0 || theProcBank.m_nChSkipCount == 2
				|| theProcBank.m_nChSkipCount == 4 || theProcBank.m_nChSkipCount == 6)
				theProcBank.m_nChSecond = JIG_CH_1;
			else
				theProcBank.m_nChSecond = JIG_CH_2;

			theProcBank.m_nChSkipCount++;

			if (theProcBank.m_nChSkipCount >= 8)
				theProcBank.m_nChSkipCount = 0;

			if (theProcBank.m_nChSkipCount == 1 || theProcBank.m_nChSkipCount == 2)
				pCell->m_nChSkipInfoJigID = JIG_ID_A;
			else if (theProcBank.m_nChSkipCount == 3 || theProcBank.m_nChSkipCount == 4)
				pCell->m_nChSkipInfoJigID = JIG_ID_B;
			else if (theProcBank.m_nChSkipCount == 5 || theProcBank.m_nChSkipCount == 6)
				pCell->m_nChSkipInfoJigID = JIG_ID_C;
			else if (theProcBank.m_nChSkipCount == 7 || theProcBank.m_nChSkipCount == 0)
				pCell->m_nChSkipInfoJigID = JIG_ID_D;
			return;
		}
		else if (i == 7 && theProcBank.m_bChSkip[i] == TRUE)
		{
			theProcBank.m_nChSkipCount++;

			if (theProcBank.m_nChSkipCount >= 8)
				theProcBank.m_nChSkipCount = 0;

			for (int i = theProcBank.m_nChSkipCount; i < 8; i++)
			{
				if (theProcBank.m_bChSkip[i] == FALSE)
				{
					if (theProcBank.m_nChSkipCount == 0 || theProcBank.m_nChSkipCount == 2
						|| theProcBank.m_nChSkipCount == 4 || theProcBank.m_nChSkipCount == 6)
						theProcBank.m_nChSecond = JIG_CH_1;
					else
						theProcBank.m_nChSecond = JIG_CH_2;

					theProcBank.m_nChSkipCount++;

					if (theProcBank.m_nChSkipCount >= 8)
						theProcBank.m_nChSkipCount = 0;

					if (theProcBank.m_nChSkipCount == 1 || theProcBank.m_nChSkipCount == 2)
						pCell->m_nChSkipInfoJigID = JIG_ID_A;
					else if (theProcBank.m_nChSkipCount == 3 || theProcBank.m_nChSkipCount == 4)
						pCell->m_nChSkipInfoJigID = JIG_ID_B;
					else if (theProcBank.m_nChSkipCount == 5 || theProcBank.m_nChSkipCount == 6)
						pCell->m_nChSkipInfoJigID = JIG_ID_C;
					else if (theProcBank.m_nChSkipCount == 7 || theProcBank.m_nChSkipCount == 0)
						pCell->m_nChSkipInfoJigID = JIG_ID_D;

					return;
				}
				else if (i == 7 && theProcBank.m_bChSkip[i] == TRUE)
				{
					theProcBank.m_nChSecond = JIG_CH_1;
					theProcBank.m_nChSkipCount++;
				}
				theProcBank.m_nChSkipCount++;
				if (theProcBank.m_nChSkipCount >= 8)
					theProcBank.m_nChSkipCount = 0;
			}
		}
		theProcBank.m_nChSkipCount++;
		if (theProcBank.m_nChSkipCount >= 8)
			theProcBank.m_nChSkipCount = 0;

		if (theProcBank.m_nChSkipCount == 1 || theProcBank.m_nChSkipCount == 2)
			pCell->m_nChSkipInfoJigID = JIG_ID_A;
		else if (theProcBank.m_nChSkipCount == 3 || theProcBank.m_nChSkipCount == 4)
			pCell->m_nChSkipInfoJigID = JIG_ID_B;
		else if (theProcBank.m_nChSkipCount == 5 || theProcBank.m_nChSkipCount == 6)
			pCell->m_nChSkipInfoJigID = JIG_ID_C;
		else if (theProcBank.m_nChSkipCount == 7 || theProcBank.m_nChSkipCount == 0)
			pCell->m_nChSkipInfoJigID = JIG_ID_D;
	}
}

JIG_CH CUnitCtrlBank::GetChSkip()
{	
	if (theConfigBank.m_Option.m_bUseACh1Skip || theConfigBank.m_Option.m_bUseACh2Skip
		|| theConfigBank.m_Option.m_bUseBCh1Skip || theConfigBank.m_Option.m_bUseBCh2Skip
		|| theConfigBank.m_Option.m_bUseCCh1Skip || theConfigBank.m_Option.m_bUseCCh2Skip
		|| theConfigBank.m_Option.m_bUseDCh1Skip || theConfigBank.m_Option.m_bUseDCh2Skip)
	{
		return theProcBank.m_nChSecond;
	}
	else
		return JIG_CH_MAX;
}


BOOL CUnitCtrlBank::ChSkipJigIDCheck()
{
	if (theConfigBank.m_Option.m_bUseACh1Skip == FALSE && theConfigBank.m_Option.m_bUseACh2Skip == FALSE
		&& theConfigBank.m_Option.m_bUseBCh1Skip == FALSE && theConfigBank.m_Option.m_bUseBCh2Skip == FALSE
		&& theConfigBank.m_Option.m_bUseCCh1Skip == FALSE && theConfigBank.m_Option.m_bUseCCh2Skip == FALSE
		&& theConfigBank.m_Option.m_bUseDCh1Skip == FALSE && theConfigBank.m_Option.m_bUseDCh2Skip == FALSE)
	{
		return FALSE;
	}

	JIG_ID JigID;
	CCellInfo *pCell;
	CCellTag tagCell;
	JigID = AMT_GetAutoJigID(ZONE_ID_A);	

	tagCell = theCellBank.GetCellTag(CELL_POS_LD_TR_CH1);
	pCell = theCellBank.GetCellInfo(CELL_POS_LD_TR_CH1);

	if(tagCell.IsExist())
	{
		if (pCell->m_nChSkipInfoJigID == JigID)
			return FALSE;
		else
			return TRUE;
	}

	tagCell = theCellBank.GetCellTag(CELL_POS_LD_TR_CH2);
	pCell = theCellBank.GetCellInfo(CELL_POS_LD_TR_CH2);

	if(tagCell.IsExist())
	{
		if (pCell->m_nChSkipInfoJigID == JigID)
			return FALSE;
		else
			return TRUE;
	}
	return TRUE;
}

void CUnitCtrlBank::CIM_CarrierCellLoadStart()
{
	theSocketInterFace.SendCmdCarrierAssignToDataPC(EQUIP_SET_CARRIER_CELL_LOAD_START,NULL);		
}


void CUnitCtrlBank::CIM_CarrierCellLoadEnd()
{
	theSocketInterFace.SendCmdCarrierAssignToDataPC(EQUIP_SET_CARRIER_CELL_LOAD_END,NULL);		
}

//2017-09-24,SKCHO �������� ������ Job PROCESS ����� �о�´�.
BOOL CUnitCtrlBank::ReadJobProcessResult(CELL_POS Pos)
{
	CString sSection;
	CString sKey;
	CString strPath;
	CString strRCMD;
	CString strCELLID;
	CString strTemp;
	CCellTag tagCell;
	CCellInfo* pCellInfo;

	tagCell = theCellBank.GetCellTag(Pos);
	pCellInfo = theCellBank.GetCellInfo(Pos);

	if(!tagCell.IsExist())
	{
		return FALSE;
	}
	strPath.Format(_T("D:\\Cheetah\\Runtime\\JOBPROCESS\\%s_Result.ini"),pCellInfo->m_strCellID );	
	
	//�����̾����� ����
	if(!CFileSupport::FileCheck(strPath)) 
	{
		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] Read JobProcess File Fail"),m_strStateName);
		return FALSE;
	}

	CIni iniFile(strPath);	

	sSection = "JOBProcess";
	
	// ���� ���� ���� ������
	strRCMD = iniFile.GetString(sSection,	_T("RCMD"),			_T(""));		
	strTemp = iniFile.GetString(sSection,	_T("JOBID"),		_T(""));		
	strCELLID = iniFile.GetString(sSection,	_T("CELLID"),		_T(""));	
	strTemp = iniFile.GetString(sSection,	_T("PRODUCTID"),	_T(""));		
	strTemp = iniFile.GetString(sSection,	_T("STEPID"),		_T(""));				
	strTemp = iniFile.GetString(sSection,	_T("ACTIONTYPE"),	_T(""));	

	theProcBank.m_strRCMD.Format(_T("%s"),strRCMD);
	if(theProcBank.m_bRCMDCh1Chk)
	{
		theProcBank.m_strRCMDCellIDCh1.Format(_T("%s"), strCELLID);
		// Ʈ���� RMCDüũ �Ϸ� �Ŀ� RCMD CH ���� �÷��� ���� 2017/06/12 HSK
		theProcBank.m_bRCMDCh1Chk = FALSE;
	}
	if(theProcBank.m_bRCMDCh2Chk)
	{
		theProcBank.m_strRCMDCellIDCh2.Format(_T("%s"),strCELLID);
		// Ʈ���� RMCDüũ �Ϸ� �Ŀ� RCMD CH ���� �÷��� ���� 2017/06/12 HSK
		theProcBank.m_bRCMDCh2Chk = FALSE;
	}	


	theLog[LOG_SEQUENCE].AddBuf( _T("[%s] Read JobProcess File Ok"),m_strStateName);
	::DeleteFile(strPath);
	

	return TRUE;	
}

//2017-09-24,SKCHO �������� ������ Job PROCESS ����� �о�´�.
BOOL CUnitCtrlBank::ReadCellInformationResult(CELL_POS Pos)
{
	CString sSection;
	CString sKey;
	CString strPath;
	CString strRCMD;
	CString strCELLID;
	CString strResult;
	CString strTemp;
	CCellTag tagCell;
	CCellInfo* pCellInfo;

	tagCell = theCellBank.GetCellTag(Pos);
	pCellInfo = theCellBank.GetCellInfo(Pos);

	if(!tagCell.IsExist())
	{
		return FALSE;
	}

	strPath.Format(_T("D:\\Cheetah\\Runtime\\CELLINFORMATION\\%s_Result.ini"),pCellInfo->m_strCellID );	
	
	//�����̾����� ����
	if(!CFileSupport::FileCheck(strPath)) 
	{
		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] Read CellInformation File Fail"),m_strStateName);
		return FALSE;
	}

	CIni iniFile(strPath);	
	sSection = _T("CellInformaion");
		
	// ���� ���� ���� ������
	strCELLID = iniFile.GetString(sSection,	_T("CELLID"),			_T(""));		
	strTemp = iniFile.GetString(sSection,	_T("PRODUCTID"),		_T(""));		
	strResult = iniFile.GetString(sSection,	_T("RESULT"),		_T(""));	
	
	pCellInfo->m_strProductID.Format(_T("%s"), strTemp);
	pCellInfo->m_strCellInfoResult.Format(_T("%s"),strResult);	

	theLog[LOG_SEQUENCE].AddBuf( _T("[%s] Read CellInformation File Ok"),m_strStateName);
	::DeleteFile(strPath);
	

	return TRUE;	
}
//2017-09-24,SKCHO ����� ��� ������ �����Ѵ�.
BOOL CUnitCtrlBank::DeleteResult(CELL_POS CH1,CELL_POS CH2)
{

	CString strPath;
	CCellTag tagCell;
	CCellInfo* pCellInfo;

	tagCell = theCellBank.GetCellTag(CH1);
	pCellInfo = theCellBank.GetCellInfo(CH1);
	if(tagCell.IsExist())
	{
    	strPath.Format(_T("D:\\Cheetah\\Runtime\\JOBPROCESS\\%s_Result.ini"),pCellInfo->m_strCellID );	
		//���������� ����
		if(CFileSupport::FileCheck(strPath)) 
		{
			::DeleteFile(strPath);
		}

		strPath.Format(_T("D:\\Cheetah\\Runtime\\CELLINFORMATION\\%s_Result.ini"),pCellInfo->m_strCellID );	
		//���������� ����
		if(CFileSupport::FileCheck(strPath)) 
		{
			::DeleteFile(strPath);
		}
	}
	tagCell = theCellBank.GetCellTag(CH2);
	pCellInfo = theCellBank.GetCellInfo(CH2);
	if(tagCell.IsExist())
	{
    	strPath.Format(_T("D:\\Cheetah\\Runtime\\JOBPROCESS\\%s_Result.ini"),pCellInfo->m_strCellID );	
		//���������� ����
		if(CFileSupport::FileCheck(strPath)) 
		{
			::DeleteFile(strPath);
		}

		strPath.Format(_T("D:\\Cheetah\\Runtime\\CELLINFORMATION\\%s_Result.ini"),pCellInfo->m_strCellID );	
		//���������� ����
		if(CFileSupport::FileCheck(strPath)) 
		{
			::DeleteFile(strPath);
		}
	}

	return TRUE;	
}

//2017-10-09,SKCHO �������� ������ Job PROCESS ����� �о�´�.
BOOL CUnitCtrlBank::ReadCarrierResult()
{
	CString sSection;
	CString sKey;
	CString strPath;
	CString strResult;
	
	strPath.Format(_T("D:\\Cheetah\\Runtime\\CARRIERRESULT\\Carrier_Result.ini"));	
	
	//�����̾����� ����
	if(!CFileSupport::FileCheck(strPath)) 
	{
		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] Read Carrier Result File Fail"),m_strStateName);
		return FALSE;
	}

	CIni iniFile(strPath);	
	sSection = _T("Carrier Result");	

	strResult = iniFile.GetString(sSection,	_T("RESULT"),		_T(""));	
	
	theProcBank.m_strS3F115_ReplyCode.Format(_T("%s"),strResult); 

	theLog[LOG_SEQUENCE].AddBuf( _T("[%s] Read Carrier Result File Ok"),m_strStateName);
	::DeleteFile(strPath);
	

	return TRUE;	
}


void CUnitCtrlBank::AMTBlowerOnOff(BOOL bBlowerOnOff, JIG_ID Jig, JIG_CH Ch)
{
	if (Jig == JIG_ID_A)
	{		

		if(Ch == JIG_CH_1)
		{			
			SetOutPutIO(Y_AMT_AJIG_CH1_FPCB_VAC_SOL, bBlowerOnOff);
		}
		else if(Ch == JIG_CH_2)
		{		
			SetOutPutIO(Y_AMT_AJIG_CH2_FPCB_VAC_SOL, bBlowerOnOff);
		}
		else
		{			
			SetOutPutIO(Y_AMT_AJIG_CH1_FPCB_VAC_SOL, bBlowerOnOff);			
			SetOutPutIO(Y_AMT_AJIG_CH2_FPCB_VAC_SOL, bBlowerOnOff);
		}
	}
	else if (Jig == JIG_ID_B)
	{		

		if(Ch == JIG_CH_1)
		{		
			SetOutPutIO(Y_AMT_BJIG_CH1_FPCB_VAC_SOL, bBlowerOnOff);
		}
		else if(Ch == JIG_CH_2)
		{		
			SetOutPutIO(Y_AMT_BJIG_CH2_FPCB_VAC_SOL, bBlowerOnOff);
		}
		else
		{		
			SetOutPutIO(Y_AMT_BJIG_CH1_FPCB_VAC_SOL, bBlowerOnOff);	
			SetOutPutIO(Y_AMT_BJIG_CH2_FPCB_VAC_SOL, bBlowerOnOff);
		}
	}
	else if (Jig == JIG_ID_C)
	{
		if(Ch == JIG_CH_1)
		{		
			SetOutPutIO(Y_AMT_CJIG_CH1_FPCB_VAC_SOL, bBlowerOnOff);
		}
		else if(Ch == JIG_CH_2)
		{		
			SetOutPutIO(Y_AMT_CJIG_CH2_FPCB_VAC_SOL, bBlowerOnOff);
		}
		else
		{		
			SetOutPutIO(Y_AMT_CJIG_CH1_FPCB_VAC_SOL, bBlowerOnOff);		
			SetOutPutIO(Y_AMT_CJIG_CH2_FPCB_VAC_SOL, bBlowerOnOff);
		}
	}
	else if (Jig == JIG_ID_D)
	{	

		if(Ch == JIG_CH_1)
		{		
			SetOutPutIO(Y_AMT_DJIG_CH1_FPCB_VAC_SOL, bBlowerOnOff);
		}
		else if(Ch == JIG_CH_2)
		{		
			SetOutPutIO(Y_AMT_DJIG_CH2_FPCB_VAC_SOL, bBlowerOnOff);
		}
		else
		{			
			SetOutPutIO(Y_AMT_DJIG_CH1_FPCB_VAC_SOL, bBlowerOnOff);		
			SetOutPutIO(Y_AMT_DJIG_CH2_FPCB_VAC_SOL, bBlowerOnOff);
		}
	}
}
//2017-12-04,SKCHO
BOOL CUnitCtrlBank::ReadPortEvent353()
{
	CString sSection;
	CString sKey;
	CString strPath;
	CString strResult;
	
	strPath.Format(_T("D:\\Cheetah\\Runtime\\PORTEVENT\\PortEvent353.ini"));	
	
	//�����̾����� ����
	if(!CFileSupport::FileCheck(strPath)) 
	{
		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] Read PortEvent353 File Fail"),m_strStateName);
		return FALSE;
	}

	CIni iniFile(strPath);	
	sSection = _T("EVENT353");	

	strResult = iniFile.GetString(sSection,	_T("TRAYID"), _T(""));	
	//2017-12-11,SKCHO �߰�
	if(strResult == _T(""))
	{
		return FALSE;
	}
	theProcBank.m_strClose353TrayID.Format(_T("%s"),strResult); 

	theLog[LOG_SEQUENCE].AddBuf( _T("[%s] Read PortEvent353 File Ok"),m_strStateName);	

	return TRUE;	
}


BOOL CUnitCtrlBank::SavePortEvent353(CString strID)
{
	CString sSection;
	CString sKey;
	CString strPath;
	CString strResult;
	
	strPath.Format(_T("D:\\Cheetah\\Runtime\\PORTEVENT\\PortEvent353.ini"));	
	
	//�����̾����� ����
	if(CFileSupport::DirectoryCheck(_T("D:\\Cheetah\\Runtime\\PORTEVENT")) == FALSE)
	{
		if(CFileSupport::CreateDirectory(_T("D:\\Cheetah\\Runtime\\PORTEVENT")) == FALSE)
			return FALSE;
	}

	CIni iniFile(strPath);	
	sSection = _T("EVENT353");	
	
	iniFile.WriteString(sSection, _T("TRAYID")	, strID);

	return TRUE;

}

void CUnitCtrlBank::DeletePortEvent353()
{
	CString strPath;	
	strPath.Format(_T("D:\\Cheetah\\Runtime\\PORTEVENT\\PortEvent353.ini"));	//���������� ����
	if(CFileSupport::FileCheck(strPath)) 
	{
		::DeleteFile(strPath);
	}
}