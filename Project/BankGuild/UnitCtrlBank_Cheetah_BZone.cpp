#include "StdAfx.h"
#include "UnitCtrlBank.h"
#include "CheetahApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CUnitCtrlBank::BZoneStartTime()
{
	if(theCellBank.GetCellTag(CELL_POS_BZONE_CH1).IsExist()) GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_BZONE_CH1)->cellTime.systmB_StartTime);
	if(theCellBank.GetCellTag(CELL_POS_BZONE_CH2).IsExist()) GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_BZONE_CH2)->cellTime.systmB_StartTime);
}

void CUnitCtrlBank::BZoneEndTime()
{
	if(theCellBank.GetCellTag(CELL_POS_BZONE_CH1).IsExist()) GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_BZONE_CH1)->cellTime.systmB_EndTime);
	if(theCellBank.GetCellTag(CELL_POS_BZONE_CH2).IsExist()) GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_BZONE_CH2)->cellTime.systmB_EndTime);
}

void CUnitCtrlBank::BZoneWaitTime()
{
	if(theCellBank.GetCellTag(CELL_POS_BZONE_CH1).IsExist()) GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_BZONE_CH1)->cellTime.systmB_WaitTime);
	if(theCellBank.GetCellTag(CELL_POS_BZONE_CH2).IsExist()) GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_BZONE_CH2)->cellTime.systmB_WaitTime);
}

void CUnitCtrlBank::BZoneAlignStartTime()
{
	if(theCellBank.GetCellTag(CELL_POS_BZONE_CH1).IsExist()) GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_BZONE_CH1)->cellTime.systmB_Align_Start_Time);
	if(theCellBank.GetCellTag(CELL_POS_BZONE_CH2).IsExist()) GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_BZONE_CH2)->cellTime.systmB_Align_Start_Time);
}

void CUnitCtrlBank::BZoneAlignEndTime()
{
	if(theCellBank.GetCellTag(CELL_POS_BZONE_CH1).IsExist()) GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_BZONE_CH1)->cellTime.systmB_Align_End_Time);
	if(theCellBank.GetCellTag(CELL_POS_BZONE_CH2).IsExist()) GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_BZONE_CH2)->cellTime.systmB_Align_End_Time);
}

void CUnitCtrlBank::B_Zone_Cam_X(JIG_CH Ch,int pos)
{
	switch(Ch)
	{
	case JIG_CH_1:
		if(pos == CAMPOS_LEFT)
			theDeviceMotion.TeachMove(m_nThreadID,TEACH_PARAM::TT_VALIGN_CAM_CH1_X1);
		else
			theDeviceMotion.TeachMove(m_nThreadID,TEACH_PARAM::TT_VALIGN_CAM_CH1_X2);
		break;
	case JIG_CH_2:
		if(pos == CAMPOS_LEFT)
			theDeviceMotion.TeachMove(m_nThreadID,TEACH_PARAM::TT_VALIGN_CAM_CH2_X1);
		else
			theDeviceMotion.TeachMove(m_nThreadID,TEACH_PARAM::TT_VALIGN_CAM_CH2_X2);

		break;
	}
}

void CUnitCtrlBank::B_Zone_Cam_Y(JIG_CH Ch,int pos)
{
	switch(Ch)
	{
	case JIG_CH_1:
		if(pos== CAMPOS_LEFT){
			theDeviceMotion.TeachMove(m_nThreadID,TEACH_PARAM::TT_VALIGN_CAM_CH1_Y1);
		}else{
			theDeviceMotion.TeachMove(m_nThreadID,TEACH_PARAM::TT_VALIGN_CAM_CH1_Y2);
		}
		break;
	case JIG_CH_2:
		if(pos== CAMPOS_LEFT){
			theDeviceMotion.TeachMove(m_nThreadID,TEACH_PARAM::TT_VALIGN_CAM_CH2_Y1);
		}else{
			theDeviceMotion.TeachMove(m_nThreadID,TEACH_PARAM::TT_VALIGN_CAM_CH2_Y2);
		}
		break;
	}
}

BOOL CUnitCtrlBank::B_Zone_Cam_X_Check(JIG_CH Ch, int Pos)
{
	if(Ch == JIG_CH_1){
		if(Pos == CAMPOS_LEFT)
			return theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::TT_VALIGN_CAM_CH1_X1);
		else
			return theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::TT_VALIGN_CAM_CH1_X2);
	}else{
		if(Pos == CAMPOS_LEFT)
			return theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::TT_VALIGN_CAM_CH2_X1);
		else
			return theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::TT_VALIGN_CAM_CH2_X2);
	}
}

BOOL CUnitCtrlBank::B_Zone_Cam_Y_Check(JIG_CH Ch, int Pos)
{
	if(Ch == JIG_CH_1){
		if(Pos == CAMPOS_LEFT)
			return theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::TT_VALIGN_CAM_CH1_Y1);
		else
			return theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::TT_VALIGN_CAM_CH1_Y2);
	}else{
		if(Pos == CAMPOS_LEFT)
			return theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::TT_VALIGN_CAM_CH2_Y1);
		else
			return theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::TT_VALIGN_CAM_CH2_Y2);
	}
}

void CUnitCtrlBank::B_Zone_Cam_Y_Ready()
{
	theDeviceMotion.TeachMove(m_nThreadID,TEACH_PARAM::TT_VALIGN_CAM_Y_READY);
}


BOOL CUnitCtrlBank::B_Zone_Cam_Y_Ready_Chk()
{
	return theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::TT_VALIGN_CAM_Y_READY);
}


void CUnitCtrlBank::B_Zone_Cam_X_Ready()
{
	theDeviceMotion.TeachMove(m_nThreadID,TEACH_PARAM::TT_VALIGN_CAM_X_READY);
}


BOOL CUnitCtrlBank::B_Zone_Cam_X_Ready_Chk()
{
	return theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::TT_VALIGN_CAM_X_READY);
}

BOOL CUnitCtrlBank::B_Zone_Align_Check()
{
	if(theConfigBank.m_Option.m_bUseDryRun)
		return TRUE;

	if(theProcBank.m_nVI_MITAlignResult != ALIGN_RESULT_NONE)
		return TRUE;
	else 
		return FALSE;
}

void CUnitCtrlBank::B_Zone_Align_Grab(JIG_CH Ch , BOOL bFirst, BOOL bManual)
{
	if (bManual == TRUE)
	{
		theSocketInterFace.SendMsgToVisionAlign(Ch, bFirst, _T("1234567"), _T("MANUAL") );
		return;
	}
	CCellTag tagCell;
	CCellInfo* pCellInfo;

	if (Ch == JIG_CH_1)
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_BZONE_CH1);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_BZONE_CH1);
	}
	else if (Ch == JIG_CH_2)
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_BZONE_CH2);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_BZONE_CH2);
	}
	if(tagCell.IsExist()==FALSE)
	{
		theProcBank.m_nVI_MITAlignResult = ALIGN_RESULT_SKIP;
		return;
	}

	if(pCellInfo->m_Zone_B.m_nVI_AlignResult != ALIGN_RESULT_NONE)
	{
		theProcBank.m_nVI_MITAlignResult = ALIGN_RESULT_SKIP;
		return;
	}
	theProcBank.m_nVI_MITAlignResult = ALIGN_RESULT_NONE;

	theSocketInterFace.SendMsgToVisionAlign(Ch, bFirst, pCellInfo->m_strInnerID, pCellInfo->m_strCellID );
}

void CUnitCtrlBank::CellData_SetVTAlignScore(JIG_CH ch, E_MARK index, double score)
{
	CCellTag tagCell;
	CCellInfo* pCellInfo;

	if (ch == JIG_CH_1)
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_BZONE_CH1);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_BZONE_CH1);
	}
	else
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_BZONE_CH2);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_BZONE_CH2);
	}

	if(tagCell.IsExist()==FALSE)
		return ;

	pCellInfo->m_Zone_B.m_dVI_AlignScore[index] = score;
}

void CUnitCtrlBank::CellData_SetVTAlignth(JIG_CH ch, double th, ALIGN_RESULT result)
{
	CCellTag tagCell;
	CCellInfo* pCellInfo;

	if (ch == JIG_CH_1)
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_BZONE_CH1);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_BZONE_CH1);
	}
	else
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_BZONE_CH2);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_BZONE_CH2);
	}

	if(tagCell.IsExist()==FALSE)
		return ;

	pCellInfo->m_Zone_B.m_dVI_AlignTH = th;
	pCellInfo->m_Zone_B.m_nVI_AlignResult = result;

	if(result == ALIGN_RESULT_GOOD)
	{
		pCellInfo->m_Zone_B.BZoneClass = GOOD_CELL;
		pCellInfo->m_Zone_B.BZoneDefect = _T("GOOD");
	}
	else
	{
		pCellInfo->m_Zone_B.BZoneClass = REJECT_CELL;
		pCellInfo->m_Zone_B.BZoneDefect = _T("NG");
	}
}
// B Zone에서 VT Align Th 필요한 경우 있어 수정 17/04/24 HSK
double CUnitCtrlBank::CellData_GetVTAlignth( JIG_CH ch , ZONE_ID Zone)
{
	if(theConfigBank.m_Option.m_bUseDryRun)
		return 0.;

	CCellTag tagCell;
	CCellInfo* pCellInfo;

	if (ch == JIG_CH_1)
	{
		if(Zone == ZONE_ID_B)
		{
			tagCell = theCellBank.GetCellTag(CELL_POS_BZONE_CH1);   //2017-04-08, skcho,BZONE ALIGN값을 못받아 CZONE으로 수정 CELL_POS_BZONE_CH1
			pCellInfo = theCellBank.GetCellInfo(CELL_POS_BZONE_CH1);//2017-04-08, skcho,BZONE ALIGN값을 못받아 CZONE으로 수정CELL_POS_BZONE_CH1
		}
		else
		{
			tagCell = theCellBank.GetCellTag(CELL_POS_CZONE_CH1);   //2017-04-08, skcho,BZONE ALIGN값을 못받아 CZONE으로 수정 CELL_POS_BZONE_CH1
			pCellInfo = theCellBank.GetCellInfo(CELL_POS_CZONE_CH1);//2017-04-08, skcho,BZONE ALIGN값을 못받아 CZONE으로 수정CELL_POS_BZONE_CH1
		}
	}
	else
	{
		if(Zone == ZONE_ID_B)
		{
			tagCell = theCellBank.GetCellTag(CELL_POS_BZONE_CH2);   //2017-04-08, skcho,BZONE ALIGN값을 못받아 CZONE으로 수정 CELL_POS_BZONE_CH1
			pCellInfo = theCellBank.GetCellInfo(CELL_POS_BZONE_CH2);//2017-04-08, skcho,BZONE ALIGN값을 못받아 CZONE으로 수정CELL_POS_BZONE_CH1
		}
		else
		{
			tagCell = theCellBank.GetCellTag(CELL_POS_CZONE_CH2);   //2017-04-08, skcho,BZONE ALIGN값을 못받아 CZONE으로 수정 CELL_POS_BZONE_CH1
			pCellInfo = theCellBank.GetCellInfo(CELL_POS_CZONE_CH2);//2017-04-08, skcho,BZONE ALIGN값을 못받아 CZONE으로 수정CELL_POS_BZONE_CH1
		}
	}

	if(tagCell.IsExist()==FALSE)
		return 0.;

	return pCellInfo->m_Zone_B.m_dVI_AlignTH;
}

ALIGN_RESULT CUnitCtrlBank::CellData_GetVTAlignResult( JIG_CH ch )
{
	CCellTag tagCell;
	CCellInfo* pCellInfo;

	if (ch == JIG_CH_1)
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_CZONE_CH1);  //2017-04-08, skcho,BZONE ALIGN값을 못받아 CZONE으로 수정CELL_POS_BZONE_CH1
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_CZONE_CH1);//2017-04-08, skcho,BZONE ALIGN값을 못받아 CZONE으로 수정CELL_POS_BZONE_CH1
	}
	else
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_CZONE_CH2);//2017-04-08, skcho,BZONE ALIGN값을 못받아 CZONE으로 수정CELL_POS_CZONE_CH2
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_CZONE_CH2);//2017-04-08, skcho,BZONE ALIGN값을 못받아 CZONE으로 수정CELL_POS_CZONE_CH2
	}

	if(tagCell.IsExist()==FALSE)
		return ALIGN_RESULT_GOOD;

	return pCellInfo->m_Zone_B.m_nVI_AlignResult;
}

//2017-12-08,SKCHO,BZONE에서 리컨택을 하기때문에 리컨택 후 결과를 넣는다
void CUnitCtrlBank::B_Zone_SetClass(JIG_CH nCh)
{
	CCellTag tagCell;
	CCellInfo* pCellInfo;

	if (nCh == JIG_CH_1)
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_BZONE_CH1);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_BZONE_CH1);
	}
	else if (nCh == JIG_CH_2)
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_BZONE_CH2);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_BZONE_CH2);
	}

	if(tagCell.IsExist()==FALSE)
		return;

	//2018-05-17,SKCHO,수정
	//if (pCellInfo->m_Zone_A.CellLoadingClass != GOOD_CELL	|| pCellInfo->m_Zone_A.ContactCurrentClass != GOOD_CELL)
	if (pCellInfo->m_Zone_A.CellLoadingClass != GOOD_CELL 
		|| pCellInfo->m_Zone_A.ContactCurrentClass != GOOD_CELL
		|| pCellInfo->m_Zone_A.RegPinClass != GOOD_CELL
		|| pCellInfo->m_Zone_A.DicCheckClass != GOOD_CELL)
	{
		pCellInfo->m_Zone_A.AZoneClass = REJECT_CELL;
		pCellInfo->m_Zone_A.AZoneDefect = _T("NG");
//		pCellInfo->m_strContactResult = _T("NG");
	}
	else
	{
		pCellInfo->m_Zone_A.AZoneClass = GOOD_CELL;
		pCellInfo->m_Zone_A.AZoneDefect = _T("GOOD");
//		pCellInfo->m_strContactResult = _T("GOOD");
	}
}