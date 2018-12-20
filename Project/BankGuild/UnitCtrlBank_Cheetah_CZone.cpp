#include "StdAfx.h"
#include "UnitCtrlBank.h"
#include "CheetahApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CUnitCtrlBank::CZoneStartTime()
{
	if(theCellBank.GetCellTag(CELL_POS_CZONE_CH1).IsExist()) GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_CZONE_CH1)->cellTime.systmC_StartTime);
	if(theCellBank.GetCellTag(CELL_POS_CZONE_CH2).IsExist()) GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_CZONE_CH2)->cellTime.systmC_StartTime);
}

void CUnitCtrlBank::CZoneEndTime()
{
	if(theCellBank.GetCellTag(CELL_POS_CZONE_CH1).IsExist()) GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_CZONE_CH1)->cellTime.systmC_EndTime);
	if(theCellBank.GetCellTag(CELL_POS_CZONE_CH2).IsExist()) GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_CZONE_CH2)->cellTime.systmC_EndTime);
}

void CUnitCtrlBank::CZoneWaitTime()
{
	if(theCellBank.GetCellTag(CELL_POS_CZONE_CH1).IsExist()) GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_CZONE_CH1)->cellTime.systmC_WaitTime);
	if(theCellBank.GetCellTag(CELL_POS_CZONE_CH2).IsExist()) GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_CZONE_CH2)->cellTime.systmC_WaitTime);
}

void CUnitCtrlBank::CZoneVIStartTime()
{
	if(theCellBank.GetCellTag(CELL_POS_CZONE_CH1).IsExist()) GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_CZONE_CH1)->cellTime.systmC_VIStartTime);
	if(theCellBank.GetCellTag(CELL_POS_CZONE_CH2).IsExist()) GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_CZONE_CH2)->cellTime.systmC_VIStartTime);
}

void CUnitCtrlBank::CZoneVIEndTime()
{
	if(theCellBank.GetCellTag(CELL_POS_CZONE_CH1).IsExist()) GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_CZONE_CH1)->cellTime.systmC_VIEndTime);
	if(theCellBank.GetCellTag(CELL_POS_CZONE_CH2).IsExist()) GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_CZONE_CH2)->cellTime.systmC_VIEndTime);
}

void CUnitCtrlBank::CZoneVIGrabStartTime()
{
	if(theCellBank.GetCellTag(CELL_POS_CZONE_CH1).IsExist()) GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_CZONE_CH1)->cellTime.systmC_VIGrabStartTime);
	if(theCellBank.GetCellTag(CELL_POS_CZONE_CH2).IsExist()) GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_CZONE_CH2)->cellTime.systmC_VIGrabStartTime);
}

void CUnitCtrlBank::CZoneVIGrabEndTime()
{
	if(theCellBank.GetCellTag(CELL_POS_CZONE_CH1).IsExist()) GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_CZONE_CH1)->cellTime.systmC_VIGrabEndTime);
	if(theCellBank.GetCellTag(CELL_POS_CZONE_CH2).IsExist()) GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_CZONE_CH2)->cellTime.systmC_VIGrabEndTime);
}

void CUnitCtrlBank::CZoneVIClassStartTime()
{
	if(theCellBank.GetCellTag(CELL_POS_CZONE_CH1).IsExist()) GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_CZONE_CH1)->cellTime.systmC_VIClassStartTime);
	if(theCellBank.GetCellTag(CELL_POS_CZONE_CH2).IsExist()) GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_CZONE_CH2)->cellTime.systmC_VIClassStartTime);
}

void CUnitCtrlBank::CZoneVIClassEndTime()
{
	if(theCellBank.GetCellTag(CELL_POS_DZONE_CH1).IsExist()) GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_DZONE_CH1)->cellTime.systmC_VIClassEndTime);
	if(theCellBank.GetCellTag(CELL_POS_DZONE_CH2).IsExist()) GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_DZONE_CH2)->cellTime.systmC_VIClassEndTime);
}

void CUnitCtrlBank::C_Zone_CamX()
{
	theDeviceMotion.TeachMove(m_nThreadID,TEACH_PARAM::TT_INSP_CAM_X);

}

void CUnitCtrlBank::C_Zone_LCam_Focus()
{
	theDeviceMotion.TeachMove(m_nThreadID,TEACH_PARAM::TT_INSP_CH1_CAM_FOCUS_F);
}

void CUnitCtrlBank::C_Zone_LCam_Theta(double dbOffset)
{
	theDeviceMotion.TeachMove(m_nThreadID,TEACH_PARAM::TT_INSP_CH1_CAM_TURN_T, dbOffset);
}

void CUnitCtrlBank::C_Zone_LCamZ()
{
	theDeviceMotion.TeachMove(m_nThreadID,TEACH_PARAM::TT_INSP_CH1_CAM_ZOOM_Z);
}

void CUnitCtrlBank::C_Zone_RCam_Focus()
{
	theDeviceMotion.TeachMove(m_nThreadID,TEACH_PARAM::TT_INSP_CH2_CAM_FOCUS_F);
}

void CUnitCtrlBank::C_Zone_RCam_Theta(double dbOffset)
{
	theDeviceMotion.TeachMove(m_nThreadID,TEACH_PARAM::TT_INSP_CH2_CAM_TURN_T, dbOffset);
}

void CUnitCtrlBank::C_Zone_RCamZ()
{
	theDeviceMotion.TeachMove(m_nThreadID,TEACH_PARAM::TT_INSP_CH2_CAM_ZOOM_Z);
}

BOOL CUnitCtrlBank::C_Zone_Inspection_Start(VI_ACTIVE nActive,JIG_CH nCh)
{
	BOOL bResult = FALSE;
	// 2017.1.11 bgkim
	if(!theConfigBank.m_Option.m_bUseVIInsp)
		return FALSE ;

	CCellTag tagCell;
	CCellInfo* pCellInfo;

	if(nCh == JIG_CH_1)
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_CZONE_CH1);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_CZONE_CH1);
	
		if (tagCell.IsExist() == FALSE)
			return TRUE;

		if (Get_InspectSkip(CELL_POS_CZONE_CH1) == TRUE)
		{
			// 검사 안할 경우에 C Zone 결과 넣어 준다 2017/05/02 HSK.
			SetCZoneResultSkip(JIG_CH_1);
			return FALSE;
		}
	}
	else
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_CZONE_CH2);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_CZONE_CH2);

		if (tagCell.IsExist() == FALSE)
			return TRUE;

		if (Get_InspectSkip(CELL_POS_CZONE_CH2) == TRUE)
		{
			// 검사 안할 경우에 C Zone 결과 넣어 준다 2017/05/02 HSK.
			SetCZoneResultSkip(JIG_CH_2);
			return FALSE;
		}
	}

	//20170210 byskcho
	if(pCellInfo->m_Zone_C.InspVIState == eSTATE_Insp_None)
	{
//		theSocketInterFace.SendMsgToVt(nActive, CMD_GRAB_START, pCellInfo->m_strInnerID);
		// Cell ID 추가 해서 보내 달라는 요청 2017/06/13 박용학책임.
		theSocketInterFace.SendMsgToVt(nActive, CMD_GRAB_START, pCellInfo->m_strInnerID,pCellInfo->m_strCellID);

		bResult = TRUE;
	}
	else
	{
		//2017-04-03,skcho 검사 생략은 아니므로 삭제
		//pCellInfo->m_bIsInspectionSkip = FALSE;
		bResult = FALSE;
	}

	return bResult;
}

BOOL CUnitCtrlBank::C_Zone_Inspection_Check(JIG_CH ch)
{
	if(theConfigBank.m_Option.m_bUseDryRun)
		return TRUE;

	// 2017.1.11 bgkim
	if(!theConfigBank.m_Option.m_bUseVIInsp)
		return TRUE;

	CCellTag tagCell;
	CCellInfo* pCellInfo;

	if(ch == JIG_CH_1)
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_CZONE_CH1);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_CZONE_CH1);

		if (tagCell.IsExist() == FALSE)
			return TRUE;

		if (Get_InspectSkip(CELL_POS_CZONE_CH1) == TRUE)
			return TRUE;
	}
	else
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_CZONE_CH2);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_CZONE_CH2);

		if (tagCell.IsExist() == FALSE)
			return TRUE;

		if (Get_InspectSkip(CELL_POS_CZONE_CH2) == TRUE)
			return TRUE;
	}
	
	if(tagCell.IsExist())
	{
		return (pCellInfo->m_Zone_C.InspVIState < eSTATE_Insp_GrabEnd) ? FALSE : TRUE;
	}

	return TRUE;
}

BOOL CUnitCtrlBank::C_Zone_CamXChk()
{
	return TheachMotionChk(m_nThreadID,TEACH_PARAM::TT_INSP_CAM_X);
}

BOOL CUnitCtrlBank::C_Zone_LCamZChk()
{
	return TheachMotionChk(m_nThreadID,TEACH_PARAM::TT_INSP_CH1_CAM_ZOOM_Z);
}

BOOL CUnitCtrlBank::C_Zone_LCam_FocusChk()
{
	return TheachMotionChk(m_nThreadID,TEACH_PARAM::TT_INSP_CH1_CAM_FOCUS_F);
}

BOOL CUnitCtrlBank::C_Zone_LCam_ThetaChk(double dbOffset)
{
	return theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::TT_INSP_CH1_CAM_TURN_T,0.01, POS_EQUAL, dbOffset);
}

BOOL CUnitCtrlBank::C_Zone_RCamZChk()
{
	return TheachMotionChk(m_nThreadID,TEACH_PARAM::TT_INSP_CH2_CAM_ZOOM_Z);
}

BOOL CUnitCtrlBank::C_Zone_RCam_FocusChk()
{
	return TheachMotionChk(m_nThreadID,TEACH_PARAM::TT_INSP_CH2_CAM_FOCUS_F);
}

BOOL CUnitCtrlBank::C_Zone_RCam_ThetaChk(double dbOffset)
{
	return theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::TT_INSP_CH2_CAM_TURN_T,0.01, POS_EQUAL, dbOffset);
}
BOOL CUnitCtrlBank::C_Zone_VT_ClassEndChk(JIG_CH nCh)
{
	if(theConfigBank.m_Option.m_bUseDryRun)
		return TRUE;

	// 2017.1.11 bgkim
	if(!theConfigBank.m_Option.m_bUseVIInsp)
		return TRUE;

	CCellTag tagCell;
	CCellInfo* pCellInfo;

	if(nCh == JIG_CH_1)
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_CZONE_CH1);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_CZONE_CH1);

		if (Get_InspectSkip(CELL_POS_CZONE_CH1) == TRUE)
			return TRUE;
	}
	else
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_CZONE_CH2);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_CZONE_CH2);

		if (Get_InspectSkip(CELL_POS_CZONE_CH2) == TRUE)
			return TRUE;
	}
	
	if(tagCell.IsExist())
	{
		if( pCellInfo->m_Zone_C.InspVIState < eSTATE_Insp_ClassEnd )
			return FALSE;
	}

	return TRUE;
}

BOOL CUnitCtrlBank::C_Zone_VT_FileLoadChk(JIG_CH nCh)
{
	if(theConfigBank.m_Option.m_bUseDryRun)
		return TRUE;

	// 2017.1.11 bgkim
	if(!theConfigBank.m_Option.m_bUseVIInsp)
		return TRUE;

	CCellTag tagCell;
	CCellInfo* pCellInfo;

	if(nCh == JIG_CH_1)
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_CZONE_CH1);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_CZONE_CH1);

		if(Get_InspectSkip(CELL_POS_CZONE_CH1) == TRUE)
			return TRUE;
	}
	else
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_CZONE_CH2);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_CZONE_CH2);

		if(Get_InspectSkip(CELL_POS_CZONE_CH2) == TRUE)
			return TRUE;
	}
	
	if(tagCell.IsExist())
	{
		if( pCellInfo->m_Zone_C.InspVIState != eSTATE_Insp_End )
			return FALSE;
	}

	return TRUE;
}


void CUnitCtrlBank::C_ZONE_JudgeFinalClass(JIG_CH ch)
{
	CCellTag tagCell = theCellBank.GetCellTag(ZONE_ID_C, ch);

	if(tagCell.IsExist()==FALSE)
		return;

	CCellInfo *pCell = theCellBank.GetCellInfo(tagCell);

	CELL_POS Pos;
	if (ch == JIG_CH_1)
		Pos = CELL_POS_CZONE_CH1;
	else
		Pos = CELL_POS_CZONE_CH2;

	if (Get_InspectSkip(Pos) == TRUE)
	{
		pCell->m_LastClass = BIN2_CELL;
		pCell->m_LastResult = _T("RCMD_NG");
	}
	else if (pCell->m_Zone_A.AZoneClass != GOOD_CELL)
	{
		pCell->m_LastClass = pCell->m_Zone_A.AZoneClass;
		pCell->m_LastResult = pCell->m_Zone_A.AZoneDefect;
	}
	else
	{
		pCell->m_LastClass = pCell->m_Zone_C.CZoneClass;
		pCell->m_LastResult = pCell->m_Zone_C.CZoneDefect;
	}
	
	//2018-02-12 JSJUNG, GOOD_CELL이 아닌경우에만 MES코드저장
	//if(pCell->m_LastClass != GOOD_CELL)
	//{
		pCell->MesCode = GetMesCode(pCell->m_LastResult);
	//}

	theLog[LOG_SEQUENCE].AddBuf(_T("%s\t%s\tFinalJudge\t%s"), pCell->m_strCellID, pCell->m_strInnerID, CONST_JUDGE_LIST[pCell->FinalJudge].strName);
	theLog[LOG_SEQUENCE].AddBuf(_T("%s\t%s\tFinalClass\t%d"), pCell->m_strCellID, pCell->m_strInnerID, pCell->m_LastClass);
	theLog[LOG_SEQUENCE].AddBuf(_T("%s\t%s\tFinalDefect\t%s"), pCell->m_strCellID, pCell->m_strInnerID, pCell->m_LastResult);
	theLog[LOG_SEQUENCE].AddBuf(_T("%s\t%s\tMesCode\t%s"), pCell->m_strCellID, pCell->m_strInnerID, pCell->MesCode);
	theLog[LOG_SEQUENCE].AddBuf(_T("%s\t%s\tCellInfoResult\t"), pCell->m_strCellID, pCell->m_strInnerID);

	theLog[LOG_SEQUENCE].AddBuf(_T("C_Zone Final Class : %d , Final Result %s"),pCell->m_LastClass,pCell->m_LastResult);
}


INSP_STATE CUnitCtrlBank::C_Zone_Get_InspState(JIG_CH nCh)
{
	CCellTag tagCell;
	CCellInfo* pCellInfo;

	if(nCh == JIG_CH_1)
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_CZONE_CH1);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_CZONE_CH1);
	}
	else
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_CZONE_CH2);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_CZONE_CH2);
	}
	
	return pCellInfo->m_Zone_C.InspVIState;
}


void CUnitCtrlBank::SetVisionNGCnt(JIG_ID Jig, JIG_CH Ch, BOOL bGood)
{
	if (bGood == FALSE)
	{
		theProcBank.m_nVisionNGCnt[Jig][Ch]++;
		if (theProcBank.m_nVisionNGCnt[Jig][Ch] >= theConfigBank.m_Option.m_nVisionContinuesNGCnt)
		{
			theProcBank.m_bVisionNG[Jig][Ch] = TRUE;
		}
	}
	else
	{
		theProcBank.m_nVisionNGCnt[Jig][Ch] = 0;
		theProcBank.m_bVisionNG[Jig][Ch] = FALSE;
	}
}


BOOL CUnitCtrlBank::GetVisionNG(JIG_ID Jig, JIG_CH Ch)
{
	return	theProcBank.m_bVisionNG[Jig][Ch];
}