#include "StdAfx.h"
#include "UnitCtrlBank.h"
#include "CheetahApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CUnitCtrlBank::PAXPos(JIG_CH nCh, double dbOffset)
{
	theDeviceMotion.TeachMove(m_nThreadID, (nCh == JIG_CH_1) ? TEACH_PARAM::PA_CH1_X : TEACH_PARAM::PA_CH2_X,dbOffset);
}

BOOL CUnitCtrlBank::PAXPosChk(JIG_CH nCh, double dbOffset)
{
	BOOL bRet = FALSE; 

	if(theDeviceMotion.CheckTeachPosition(m_nThreadID, (nCh == JIG_CH_1) ? TEACH_PARAM::PA_CH1_X : TEACH_PARAM::PA_CH2_X,0.05,POS_EQUAL, dbOffset) == TRUE)
		bRet = TRUE;

	return bRet;
}

void CUnitCtrlBank::PAYPos(JIG_CH nCh, double dbOffset)
{
	theDeviceMotion.TeachMove(m_nThreadID, (nCh == JIG_CH_1) ? TEACH_PARAM::PA_CH1_Y : TEACH_PARAM::PA_CH2_Y,dbOffset);
}

BOOL CUnitCtrlBank::PAYPosChk(JIG_CH nCh, double dbOffset)
{
	BOOL bRet = FALSE; 

	if(theDeviceMotion.CheckTeachPosition(m_nThreadID, (nCh == JIG_CH_1) ? TEACH_PARAM::PA_CH1_Y : TEACH_PARAM::PA_CH2_Y,0.05,POS_EQUAL, dbOffset) == TRUE)
		bRet = TRUE;

	return bRet;
}

void CUnitCtrlBank::PATPos(JIG_CH nCh, double dbOffset)
{
	theDeviceMotion.TeachMove(m_nThreadID, (nCh == JIG_CH_1) ? TEACH_PARAM::PA_CH1_T : TEACH_PARAM::PA_CH2_T,dbOffset);
}

BOOL CUnitCtrlBank::PATPosChk(JIG_CH nCh, double dbOffset)
{
	BOOL bRet = FALSE; 

	if(theDeviceMotion.CheckTeachPosition(m_nThreadID, (nCh == JIG_CH_1) ? TEACH_PARAM::PA_CH1_T : TEACH_PARAM::PA_CH2_T,0.01,POS_EQUAL, dbOffset) == TRUE)
		bRet = TRUE;

	return bRet;
}

void CUnitCtrlBank::PAStageLoadPos()
{
	theDeviceMotion.TeachMove(m_nThreadID, TEACH_PARAM::PA_STAGE_LOAD_POS);
}

BOOL CUnitCtrlBank::PAStageLoadPosChk()
{
	BOOL bRet = FALSE; 

	if(theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::PA_STAGE_LOAD_POS) == TRUE)
		bRet = TRUE;

	return bRet;
}


void CUnitCtrlBank::PAStageAlignPos()
{
	theDeviceMotion.TeachMove(m_nThreadID, TEACH_PARAM::PA_STAGE_ALIGN_POS);
}

BOOL CUnitCtrlBank::PAStageAlignPosChk()
{
	BOOL bRet = FALSE; 

	if(theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::PA_STAGE_ALIGN_POS) == TRUE)
		bRet = TRUE;

	return bRet;
}

void CUnitCtrlBank::PAStageUnloadPos()
{
	theDeviceMotion.TeachMove(m_nThreadID, TEACH_PARAM::PA_STAGE_UNLOAD_POS);
}

BOOL CUnitCtrlBank::PAStageUnloadPosChk()
{
	BOOL bRet = FALSE; 

	if(theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::PA_STAGE_UNLOAD_POS) == TRUE)
		bRet = TRUE;

	return bRet;
}

void CUnitCtrlBank::PAStageVac(BOOL bVacOn, JIG_CH Ch)
{
	if(Ch == JIG_CH_1)
	{
		SetOutPutIO(Y_PA_STAGE_CH1_VAC_ON_SOL, bVacOn);
		SetOutPutIO(Y_PA_STAGE_CH1_BLOW_ON_SOL, !bVacOn);
	}
	else if (Ch == JIG_CH_2)
	{
		SetOutPutIO(Y_PA_STAGE_CH2_VAC_ON_SOL, bVacOn);
		SetOutPutIO(Y_PA_STAGE_CH2_BLOW_ON_SOL, !bVacOn);
	}
	else
	{
		SetOutPutIO(Y_PA_STAGE_CH1_VAC_ON_SOL, bVacOn);
		SetOutPutIO(Y_PA_STAGE_CH1_BLOW_ON_SOL, !bVacOn);
		SetOutPutIO(Y_PA_STAGE_CH2_VAC_ON_SOL, bVacOn);
		SetOutPutIO(Y_PA_STAGE_CH2_BLOW_ON_SOL, !bVacOn);
	}
}
//2017-11-20, YMG
void CUnitCtrlBank::PAStage_BlowOn(JIG_CH Ch)
{
	if(Ch == JIG_CH_1)
	{
		SetOutPutIO(Y_PA_STAGE_CH1_BLOW_ON_SOL, TRUE);
	}
	else if (Ch == JIG_CH_2)
	{
		SetOutPutIO(Y_PA_STAGE_CH2_BLOW_ON_SOL, TRUE);
	}
	else
	{
		SetOutPutIO(Y_PA_STAGE_CH1_BLOW_ON_SOL, TRUE);
		SetOutPutIO(Y_PA_STAGE_CH2_BLOW_ON_SOL, TRUE);
	}
}
void CUnitCtrlBank::PAStage_BlowOff(JIG_CH Ch)
{
	if(Ch == JIG_CH_1)
	{
		SetOutPutIO(Y_PA_STAGE_CH1_BLOW_ON_SOL, FALSE);
	}
	else if (Ch == JIG_CH_2)
	{
		SetOutPutIO(Y_PA_STAGE_CH2_BLOW_ON_SOL, FALSE);
	}
	else
	{
		SetOutPutIO(Y_PA_STAGE_CH1_BLOW_ON_SOL, FALSE);
		SetOutPutIO(Y_PA_STAGE_CH2_BLOW_ON_SOL, FALSE);
	}
}

BOOL CUnitCtrlBank::PAStageVacChk(JIG_CH Ch,VAC_STATE Vac)
{
	if(theConfigBank.m_Option.m_bUseDryRun)
		return TRUE;
	if(Ch == JIG_CH_1)
	{
		if (Vac == VAC_ON)
			return GetInPutIOCheck(X_PA_STG_CH1_VAC_SEN);
		else
			return !GetInPutIOCheck(X_PA_STG_CH1_VAC_SEN);

	}		
	else if(Ch == JIG_CH_2)
	{
		if (Vac == VAC_ON)
			return GetInPutIOCheck(X_PA_STG_CH2_VAC_SEN);
		else
			return !GetInPutIOCheck(X_PA_STG_CH2_VAC_SEN);

	}	
	return TRUE;
}

void CUnitCtrlBank::PA_SendAlignStart( BOOL bTheta )
{
	CString strInnerID1, strInnerID2, strCellID1, strCellID2;
	strInnerID1 = _T("");
	strCellID1 = _T("");
	strInnerID2 = _T("");
	strCellID2 = _T("");

//	theProcBank.m_nPA_MITAlignResult[JIG_CH_1] = ALIGN_RESULT_SKIP;
//	theProcBank.m_nPA_MITAlignResult[JIG_CH_2] = ALIGN_RESULT_SKIP;

	CCellTag tagCell;
	CCellInfo* pCellInfo;

	tagCell = theCellBank.GetCellTag(CELL_POS_PA_STAGE_CH1);
	pCellInfo = theCellBank.GetCellInfo(CELL_POS_PA_STAGE_CH1);

	if(tagCell.IsExist())
	{
		//2017-09-25,SKCHO, 재시도시 조건 초기화
		if(theProcBank.m_nPACH1RetryStep == 1)
		{
			pCellInfo->Pre_AlignResultT = ALIGN_RESULT_NONE;
			pCellInfo->Pre_AlignResultXY = ALIGN_RESULT_NONE;
		}

		if(bTheta)
		{
			if(pCellInfo->Pre_AlignResultT == ALIGN_RESULT_NONE)
			{
				theProcBank.m_nPA_MITAlignResult[JIG_CH_1] = ALIGN_RESULT_NONE;	
				strInnerID1 = pCellInfo->m_strInnerID;
				strCellID1 = pCellInfo->m_strCellID;
			}
		}
		else
		{
			if(pCellInfo->Pre_AlignResultXY == ALIGN_RESULT_NONE)
			{
				theProcBank.m_nPA_MITAlignResult[JIG_CH_1] = ALIGN_RESULT_NONE;	
				strInnerID1 = pCellInfo->m_strInnerID;
				strCellID1 = pCellInfo->m_strCellID;
			}
		}
	}

	tagCell = theCellBank.GetCellTag(CELL_POS_PA_STAGE_CH2);
	pCellInfo = theCellBank.GetCellInfo(CELL_POS_PA_STAGE_CH2);
	if(tagCell.IsExist())
	{

		//2017-09-25,SKCHO, 재시도시 조건 초기화
		if(theProcBank.m_nPACH2RetryStep == 1)
		{
			pCellInfo->Pre_AlignResultT = ALIGN_RESULT_NONE;
			pCellInfo->Pre_AlignResultXY = ALIGN_RESULT_NONE;
		}

		if(bTheta)
		{
			if(pCellInfo->Pre_AlignResultT == ALIGN_RESULT_NONE)
			{
				theProcBank.m_nPA_MITAlignResult[JIG_CH_2] = ALIGN_RESULT_NONE;	
				strInnerID2 = pCellInfo->m_strInnerID;
				strCellID2 = pCellInfo->m_strCellID;
			}
		}
		else
		{
			if(pCellInfo->Pre_AlignResultXY == ALIGN_RESULT_NONE)
			{
				theProcBank.m_nPA_MITAlignResult[JIG_CH_2] = ALIGN_RESULT_NONE;	
				strInnerID2 = pCellInfo->m_strInnerID;
				strCellID2 = pCellInfo->m_strCellID;
			}
		}
	}

	JIG_ID jig;
	JIG_CH	ch;

	ch = JIG_CH_1;

	// Contact Align 요청 시에 무조건 현재 위치하는 지그를 보내면 안된다.... 2017/04/26 HSK
	if((CellDataMachineChk(CELL_POS_AZONE_CH1,CELL_POS_AZONE_CH2) == FALSE)
		&& (CellDataMachineChk(CELL_POS_LD_TR_CH1,CELL_POS_LD_TR_CH2) == FALSE))
	{
		jig = AMT_GetAutoJigID(ZONE_ID_A);
	}
	else if((CellDataMachineChk(CELL_POS_AZONE_CH1,CELL_POS_AZONE_CH2) == TRUE)
		&& (CellDataMachineChk(CELL_POS_LD_TR_CH1,CELL_POS_LD_TR_CH2) == TRUE))
	{
		jig = AMT_GetAutoJigID(ZONE_ID_A);
		if(jig == JIG_ID_A)
			jig = JIG_ID_C;
		else if(jig == JIG_ID_B)
			jig = JIG_ID_D;
		else if(jig == JIG_ID_C)
			jig = JIG_ID_A;
		else if(jig == JIG_ID_D)
			jig = JIG_ID_B;
	}
	else 
	{
		jig = AMT_GetAutoJigID(ZONE_ID_A);
		if(jig == JIG_ID_A)
			jig = JIG_ID_D;
		else if(jig == JIG_ID_B)
			jig = JIG_ID_A;
		else if(jig == JIG_ID_C)
			jig = JIG_ID_B;
		else if(jig == JIG_ID_D)
			jig = JIG_ID_C;
	}

	theSocketInterFace.SendMsgToContactAlign(jig, ch, strInnerID1, strInnerID2, strCellID1, strCellID2);
}


ALIGN_RESULT CUnitCtrlBank::CellData_GetPreAlignState(JIG_CH nCh)
{
	CCellTag tagCell;
	CCellInfo* pCellInfo;

	if (nCh == JIG_CH_1)
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_PA_STAGE_CH1);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_PA_STAGE_CH1);
	}
	else if (nCh == JIG_CH_2)
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_PA_STAGE_CH2);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_PA_STAGE_CH2);
	}

	if((tagCell.IsExist()==FALSE) || (theConfigBank.m_Option.m_bUsePreAlign == FALSE))
		return ALIGN_RESULT_SKIP;

	return pCellInfo->Pre_AlignResultXY;


	// X, Y를 나중에 하니까 X, Y만 확인하면 된다 [8/16/2013 OSC]
}

void CUnitCtrlBank::CellData_SetPreAlignState(JIG_CH nCh , ALIGN_RESULT Result)
{
	CCellTag tagCell;
	CCellInfo* pCellInfo;

	if (nCh == JIG_CH_1)
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_PA_STAGE_CH1);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_PA_STAGE_CH1);
	}
	else if (nCh == JIG_CH_2)
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_PA_STAGE_CH2);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_PA_STAGE_CH2);
	}

	if((tagCell.IsExist()==FALSE) || (theConfigBank.m_Option.m_bUsePreAlign == FALSE))
		return;
	
	//2017-12-25,SKCHO, 상태 저장
	pCellInfo->Pre_AlignResultT = Result;
	pCellInfo->Pre_AlignResultXY = Result;

	//pCellInfo->Pre_AlignResultXY = ALIGN_RESULT_SKIP;
}

void CUnitCtrlBank::CellData_SetPreAlignStateXY( JIG_CH nCh, ALIGN_RESULT nState, double dX, double dY, double dScoreL, double dScoreR )
{
	CCellTag tagCell;
	CCellInfo* pCellInfo;

	if (nCh == JIG_CH_1)
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_PA_STAGE_CH1);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_PA_STAGE_CH1);
	}
	else if (nCh == JIG_CH_2)
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_PA_STAGE_CH2);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_PA_STAGE_CH2);
	}

	if(tagCell.IsExist()==FALSE)
		return ;

	pCellInfo->Pre_Align_Data_x = dX;
	pCellInfo->Pre_Align_Data_y = dY;
	pCellInfo->Pre_AlignScore[E_MARK_LEFT] = dScoreL;
	pCellInfo->Pre_AlignScore[E_MARK_RIGHT] = dScoreR;
	if(pCellInfo->Pre_AlignResultXY != ALIGN_RESULT_SKIP)
		pCellInfo->Pre_AlignResultXY = nState;
}
void CUnitCtrlBank::CellData_SetPreAlignStateT( JIG_CH nCh, ALIGN_RESULT nState, double dTh )
{
	CCellTag tagCell;
	CCellInfo* pCellInfo;

	if (nCh == JIG_CH_1)
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_PA_STAGE_CH1);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_PA_STAGE_CH1);
	}
	else if (nCh == JIG_CH_2)
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_PA_STAGE_CH2);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_PA_STAGE_CH2);
	}

	if(tagCell.IsExist()==FALSE)
		return ;

	pCellInfo->Pre_Align_Data_th = dTh;
	if(pCellInfo->Pre_AlignResultT != ALIGN_RESULT_SKIP)
		pCellInfo->Pre_AlignResultT = nState;
}
double CUnitCtrlBank::CellData_GetPreAlginX(JIG_CH nCh)
{
	CCellTag tagCell;
	CCellInfo* pCellInfo;

	if (nCh == JIG_CH_1)
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_PA_STAGE_CH1);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_PA_STAGE_CH1);
	}
	else if (nCh == JIG_CH_2)
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_PA_STAGE_CH2);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_PA_STAGE_CH2);
	}

	if(tagCell.IsExist()==FALSE)
		return 0.;

	if(pCellInfo->Pre_AlignResultXY != ALIGN_RESULT_GOOD)
		return 0.;

	return pCellInfo->Pre_Align_Data_x;
}
double CUnitCtrlBank::CellData_GetPreAlginY(JIG_CH nCh)
{
	CCellTag tagCell;
	CCellInfo* pCellInfo;

	if (nCh == JIG_CH_1)
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_PA_STAGE_CH1);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_PA_STAGE_CH1);
	}
	else if (nCh == JIG_CH_2)
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_PA_STAGE_CH2);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_PA_STAGE_CH2);
	}

	if(tagCell.IsExist()==FALSE)
		return 0.;

	if(pCellInfo->Pre_AlignResultXY != ALIGN_RESULT_GOOD)
		return 0.;

	return pCellInfo->Pre_Align_Data_y;
}
double CUnitCtrlBank::CellData_GetPreAlginT(JIG_CH nCh)
{
	CCellTag tagCell;
	CCellInfo* pCellInfo;

	if (nCh == JIG_CH_1)
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_PA_STAGE_CH1);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_PA_STAGE_CH1);
	}
	else if (nCh == JIG_CH_2)
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_PA_STAGE_CH2);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_PA_STAGE_CH2);
	}

	if(tagCell.IsExist()==FALSE)
		return 0.;

	if(pCellInfo->Pre_AlignResultT != ALIGN_RESULT_GOOD)
		return 0.;

	return pCellInfo->Pre_Align_Data_th;
}

//2018-01-22,GHLEE, PA STAGE FPCB SENSOR 설치 대비
BOOL CUnitCtrlBank::PAStageCH1FPCBSenChk( SENSOR_STATE sen )
{
	if(theConfigBank.m_Option.m_bUseDryRun)
		return TRUE;

	ONOFF val = (sen== SENSOR_ON) ? ON:OFF;
#if(AMT_TYPE == 1)
	INPUT_ID iId = X_PA_STAGE_CH1_FPCB_SEN;
#else
	INPUT_ID iId = X_PA_STAGE_CH1_FPCB_SEN;
#endif

	if(GetInPutIOCheck(iId, val))
		return TRUE;
	else
		return FALSE;
}

BOOL CUnitCtrlBank::PAStageCH2FPCBSenChk( SENSOR_STATE sen )
{
	if(theConfigBank.m_Option.m_bUseDryRun)
		return TRUE;

	ONOFF val = (sen== SENSOR_ON) ? ON:OFF;
#if(AMT_TYPE == 1)
	INPUT_ID iId = X_PA_STAGE_CH2_FPCB_SEN;
#else
	INPUT_ID iId = X_PA_STAGE_CH2_FPCB_SEN;
#endif

	if(GetInPutIOCheck(iId, val))
		return TRUE;
	else
		return FALSE;
}