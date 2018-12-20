#include "StdAfx.h"
#include "UnitCtrlBank.h"
#include "CheetahApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CUnitCtrlBank::DZoneStartTime()
{
	if(theCellBank.GetCellTag(CELL_POS_DZONE_CH1).IsExist()) GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_DZONE_CH1)->cellTime.systmD_StartTime);
	if(theCellBank.GetCellTag(CELL_POS_DZONE_CH2).IsExist()) GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_DZONE_CH2)->cellTime.systmD_StartTime);
}

void CUnitCtrlBank::DZoneEndTime()
{
	if(theCellBank.GetCellTag(CELL_POS_DZONE_CH1).IsExist()) GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_DZONE_CH1)->cellTime.systmD_EndTime);
	if(theCellBank.GetCellTag(CELL_POS_DZONE_CH2).IsExist()) GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_DZONE_CH2)->cellTime.systmD_EndTime);
}

void CUnitCtrlBank::DZoneWaitTime()
{
	if(theCellBank.GetCellTag(CELL_POS_DZONE_CH1).IsExist()) GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_DZONE_CH1)->cellTime.systmD_WaitTime);
	if(theCellBank.GetCellTag(CELL_POS_DZONE_CH2).IsExist()) GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_DZONE_CH2)->cellTime.systmD_WaitTime);
}

BOOL CUnitCtrlBank::D_Zone_VT_ClassEndChk(JIG_CH nCh)
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
		tagCell = theCellBank.GetCellTag(CELL_POS_DZONE_CH1);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_DZONE_CH1);

		if (tagCell.IsExist() == FALSE)
			return TRUE;

		if (Get_InspectSkip(CELL_POS_DZONE_CH1) == TRUE)
			return TRUE;
	}
	else
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_DZONE_CH2);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_DZONE_CH2);

		if (tagCell.IsExist() == FALSE)
			return TRUE;

		if (Get_InspectSkip(CELL_POS_DZONE_CH2) == TRUE)
			return TRUE;
	}
	
	if(tagCell.IsExist())
	{
		if( pCellInfo->m_Zone_C.InspVIState < eSTATE_Insp_ClassEnd )
			return FALSE;
	}

	return TRUE;
}

BOOL CUnitCtrlBank::D_Zone_VT_FileLoadChk(JIG_CH nCh)
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
		tagCell = theCellBank.GetCellTag(CELL_POS_DZONE_CH1);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_DZONE_CH1);

		if (tagCell.IsExist() == FALSE)
			return TRUE;

		if(Get_InspectSkip(CELL_POS_DZONE_CH1) == TRUE)
			return TRUE;
	}
	else
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_DZONE_CH2);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_DZONE_CH2);

		if (tagCell.IsExist() == FALSE)
			return TRUE;

		if(Get_InspectSkip(CELL_POS_DZONE_CH2) == TRUE)
			return TRUE;
	}
	
	if(tagCell.IsExist())
	{
		if( pCellInfo->m_Zone_C.InspVIState != eSTATE_Insp_End )
			return FALSE;
	}
	else
		return TRUE;

	return TRUE;
}


void CUnitCtrlBank::JudgeFinalClass(JIG_CH ch)
{
	CCellTag tagCell = theCellBank.GetCellTag(ZONE_ID_D, ch);

	if(tagCell.IsExist()==FALSE)
		return;

	CCellInfo *pCell = theCellBank.GetCellInfo(tagCell);

	CELL_POS Pos;
	if (ch == JIG_CH_1)
		Pos = CELL_POS_DZONE_CH1;
	else
		Pos = CELL_POS_DZONE_CH2;

	//2017-04-05, skcho, 공정검사 권승엽씨 요청
	//MCR 읽기가 실패하면 검사를 하지 않기 때문에 MCR READING FAIL로 표시한다.
	if (Get_InspectSkip(Pos) == TRUE)
	{
		if(pCell->m_strCellID ==_T("FAIL") || pCell->m_strCellID ==_T(""))
		//if(pCell->m_strCellID ==_T("UV_FAIL") || pCell->m_strCellID ==_T(""))	//2017-04-05,skcho, 코그닉스에서 FAIL 로 데이터를 줌
		{
			pCell->m_LastClass = BIN2_CELL;
		    pCell->m_LastResult = TEXT_MCR_READING_FAIL;  //MCR_READING_FAIL로 표시
		}
		//CONTACT은 CELL LOADING과  CURRENT CHECK로 나누기때문에 최종값인 CURRNT CLASS로 판정하고 NG코드는 CONTACT NG로 한다.
		//2018-08-02,SKCHO, CONTACT_NG 판단 조건 변경, DIC_CHECK과 구별하기 위해서
		else if (pCell->m_Zone_A.CellLoadingClass != GOOD_CELL || pCell->m_Zone_A.ContactCurrentClass != GOOD_CELL)
		//else if (pCell->m_Zone_A.AZoneClass != GOOD_CELL || pCell->m_Zone_A.ContactCurrentClass != GOOD_CELL)
		{			
			pCell->m_LastClass = REJECT_CELL;//pCell->m_Zone_A.AZoneClass;
			pCell->m_LastResult = TEXT_CONTACT_NG;   //CONTACT NG로 표시
		}
		//2018-05-17,SKCHO
		else if (pCell->m_Zone_A.DicCheckClass != GOOD_CELL)	
		{
			pCell->m_LastClass = REJECT_CELL;//pCell->m_Zone_A.AZoneClass;
			pCell->m_LastResult = TEXT_DIC_CHECK_NG;   //DIC_CHECK NG로 표시
		}
		//2017-05-19,skcho, RCMD 에러이면 'O'보고, 컨택NG보다 우선권,MCR FAIL이 아닌 경우
		if (Get_RCMDResult(Pos) == FALSE && pCell->m_strCellID !=_T("FAIL"))
		{
			pCell->m_LastClass = BIN2_CELL;
		    pCell->m_LastResult = _T("INSPECTION FAIL"); //RCMD오류로 인한 결과 표시
		}		
	}
	//CONTACT NG일 경우 검사 동작이 불필요 하므로 INSPCETION_SIKP 구문에 통합  2017-04-07,skcho, 공정 심선임 요청
	//CONTACT은 CELL LOADING과  CURRENT CHECK로 나누기때문에 최종값인 CURRNT CLASS로 판정하고 NG코드는 CONTACT NG로 한다.
	/*else if (pCell->m_Zone_A.AZoneClass != GOOD_CELL || pCell->m_Zone_A.ContactCurrentClass != GOOD_CELL)
	{
		//pCell->m_LastClass = pCell->m_Zone_A.AZoneClass;
		//pCell->m_LastResult = pCell->m_Zone_A.AZoneDefect;   //CONTACT NG로 표시
		pCell->m_LastClass = REJECT_CELL;//pCell->m_Zone_A.AZoneClass;
		pCell->m_LastResult = TEXT_CONTACT_NG;   //CONTACT NG로 표시
	}
	*/
	else
	{
		pCell->m_LastClass = pCell->m_Zone_C.CZoneClass;
		pCell->m_LastResult = pCell->m_Zone_C.CZoneDefect;
	}

	// Judge OK 판정 옵션시에 실제로 판정을 GOOD으로 해준다. 17/04/22 HSK.
	if(theConfigBank.m_Option.m_bUseJudgeOK)
	{
		pCell->m_LastClass = GOOD_CELL;
		//2017-08-01,JSJUNG, 강제OK 했을경우 검사결과도 GOOD으로 변경
		pCell->m_LastResult =_T("GOOD");
	}

	//2018-02-12 JSJUNG, GOOD_CELL이 아닌경우에만 MES코드저장
	//if(pCell->m_LastClass != GOOD_CELL)
	//{
		pCell->MesCode = GetMesCode(pCell->m_LastResult);
	//}

	theLog[LOG_SEQUENCE].AddBuf(_T("%s\t%s\tFinalJudge\t%s"), pCell->m_strCellID, pCell->m_strInnerID, CONST_JUDGE_LIST[pCell->FinalJudge].strName);
	theLog[LOG_SEQUENCE].AddBuf(_T("%s\t%s\tFinalClass\t%d"), pCell->m_strCellID, pCell->m_strInnerID, pCell->m_LastClass);
	theLog[LOG_SEQUENCE].AddBuf(_T("%s\t%s\tFinalDefect\t%s"), pCell->m_strCellID, pCell->m_strInnerID, pCell->m_LastResult);
	theLog[LOG_SEQUENCE].AddBuf(_T("%s\t%s\tMESCode\t%s"), pCell->m_strCellID, pCell->m_strInnerID, pCell->MesCode);
	theLog[LOG_SEQUENCE].AddBuf(_T("%s\t%s\tCellInfoResult\t"), pCell->m_strCellID, pCell->m_strInnerID);

	theLog[LOG_SEQUENCE].AddBuf(_T("D_Zone Final Class : %d , Final Result %s"),pCell->m_LastClass,pCell->m_LastResult);
}

CString CUnitCtrlBank::GetMesCode(CString strDefectName)
{
	//kjpark 20161121 MES code list read
	CString strCode, strUndefinedCode;
	map<int, DEFECT_MES_CODE>::iterator itmap = theProcBank.iMapDefectList.begin();
	for(; itmap != theProcBank.iMapDefectList.end(); ++ itmap)
	{		
		//if(pCell->m_LastResult.Find(itmap->second.strPopup) == 0) //문자 포함할 경우 사용
		if(strDefectName == itmap->second.strPopup)
		{
			return itmap->second.strMES_CODE;
		}
		else if(TEXT_UNDEFINED == itmap->second.strPopup)
		{
			strUndefinedCode = itmap->second.strMES_CODE;
		
			//2018-02-12 JSJUNG, MESCODE매칭되는것이 없을경우에는 GDCAMTE 저장
			//strUndefinedCode.Format(_T("GDCAMTE"));
		}
	}
	// 못찾을 경우 기본값 UNDEFINED용 불량코드 [11/28/2016 OSC]
	return strUndefinedCode;
}


void CUnitCtrlBank::WriteCellLog( CELL_POS pos )
{
	theCellBank.WriteCellLog(pos);
}

BOOL CUnitCtrlBank::TurnOffChk(JIG_CH ch)
{
//  뺑뺑이 모드 추가 체크 2017/04/26 HSK 아직 안함.
//	if((theConfigBank.m_Option.m_bUseContact == FALSE) || (theConfigBank.m_Option.m_bUseRepeatInsp == TRUE))
//		return TRUE;
	if(theConfigBank.m_Option.m_bUseContact == FALSE)
		return TRUE;

	CCellTag tagCell;
	CCellInfo* pCellInfo;

	if(ch == JIG_CH_1)
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_DZONE_CH1);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_DZONE_CH1);
	}
	else
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_DZONE_CH2);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_DZONE_CH2);
	}
	
	if(tagCell.IsExist())
	{
		if( pCellInfo->m_Zone_D.TurnOffChk)
			return TRUE;
	}
	else
		return TRUE;

	return FALSE;
}