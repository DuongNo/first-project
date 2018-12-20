#include "StdAfx.h"
#include "UnitCtrlBank.h"
#include "CheetahApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CUnitCtrlBank::AZoneStartTime()
{
	if(theCellBank.GetCellTag(CELL_POS_AZONE_CH1).IsExist()) 
	{
			GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_AZONE_CH1)->cellTime.systmA_StartTime);
	}
	if(theCellBank.GetCellTag(CELL_POS_AZONE_CH2).IsExist())
	{
			GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_AZONE_CH2)->cellTime.systmA_StartTime);
	}
}

void CUnitCtrlBank::AZoneEndTime()
{
	if(theCellBank.GetCellTag(CELL_POS_AZONE_CH1).IsExist()) GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_AZONE_CH1)->cellTime.systmA_EndTime);
	if(theCellBank.GetCellTag(CELL_POS_AZONE_CH2).IsExist()) GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_AZONE_CH2)->cellTime.systmA_EndTime);
}

void CUnitCtrlBank::AZoneWaitTime()
{
	if(theCellBank.GetCellTag(CELL_POS_AZONE_CH1).IsExist()) GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_AZONE_CH1)->cellTime.systmA_WaitTime);
	if(theCellBank.GetCellTag(CELL_POS_AZONE_CH2).IsExist()) GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_AZONE_CH2)->cellTime.systmA_WaitTime);
}

void CUnitCtrlBank::AZoneContactStartTime()
{
	if(theCellBank.GetCellTag(CELL_POS_AZONE_CH1).IsExist()) GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_AZONE_CH1)->cellTime.systmA_ContactStartTime);
	if(theCellBank.GetCellTag(CELL_POS_AZONE_CH2).IsExist()) GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_AZONE_CH2)->cellTime.systmA_ContactStartTime);
}

void CUnitCtrlBank::AZoneContactEndTime()
{
	if(theCellBank.GetCellTag(CELL_POS_AZONE_CH1).IsExist())
	{
		GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_AZONE_CH1)->cellTime.systmA_ContactEndTime);
	}
	if(theCellBank.GetCellTag(CELL_POS_AZONE_CH2).IsExist()) 
	{
		GetLocalTime(&theCellBank.GetCellInfo(CELL_POS_AZONE_CH2)->cellTime.systmA_ContactEndTime);
	}
}


int CUnitCtrlBank::A_Zone_GetContactCount(JIG_CH nCh)
{
	if(theConfigBank.m_Option.m_bUseContact == FALSE)
		return 0;
	if(theConfigBank.m_Option.m_bUseDryRun)
		return 0;

	CCellTag tagCell;
	CCellInfo* pCellInfo;

	if (nCh == JIG_CH_1)
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_AZONE_CH1);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_AZONE_CH1);
	}
	else if (nCh == JIG_CH_2)
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_AZONE_CH2);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_AZONE_CH2);
	}

	if(tagCell.IsExist()==FALSE)
	{
		return 0;
	}

	return pCellInfo->m_nContactCnt;	
}


void CUnitCtrlBank::A_Zone_SetContactCount(JIG_CH nCh)
{
	if(theConfigBank.m_Option.m_bUseContact == FALSE)
		return ;
	if(theConfigBank.m_Option.m_bUseDryRun)
		return ;

	CCellTag tagCell;
	CCellInfo* pCellInfo;

	if (nCh == JIG_CH_1)
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_AZONE_CH1);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_AZONE_CH1);
	}
	else if (nCh == JIG_CH_2)
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_AZONE_CH2);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_AZONE_CH2);
	}

	if(tagCell.IsExist()==FALSE)
	{
		return ;
	}

	pCellInfo->m_nContactCnt = 1;	
}

void CUnitCtrlBank::B_Zone_SetContactCount(JIG_CH nCh)
{
	if(theConfigBank.m_Option.m_bUseContact == FALSE)
		return ;
	if(theConfigBank.m_Option.m_bUseDryRun)
		return ;

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
	{
		return ;
	}
    //2018-05-19,MGYUN, RECONTACT RESET MESSAGE
	pCellInfo->m_Zone_A.CellLoadingClass = NONE_CELL; 
	pCellInfo->m_Zone_A.ContactCurrentClass = NONE_CELL;
	pCellInfo->m_Zone_A.RegPinClass = NONE_CELL;
	pCellInfo->m_Zone_A.DicCheckClass = NONE_CELL;

	pCellInfo->m_nContactCnt = 1;	
}



BOOL CUnitCtrlBank::A_Zone_ContactChk(JIG_CH nCh)
{
	if(theConfigBank.m_Option.m_bUseContact == FALSE)
		return TRUE;
	if(theConfigBank.m_Option.m_bUseDryRun)
		return TRUE;

	BOOL bResult = FALSE;

	CCellTag tagCell;
	CCellInfo* pCellInfo;

	if (nCh == JIG_CH_1)
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_AZONE_CH1);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_AZONE_CH1);
	}
	else if (nCh == JIG_CH_2)
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_AZONE_CH2);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_AZONE_CH2);
	}

	if(tagCell.IsExist()==FALSE)
	{
		return TRUE;
	}

	if(pCellInfo->m_nContactCnt == 0)
	{
		//첫번째 컨택에서 good이면 1차,2차 결과 모두 good
		if(pCellInfo->m_Zone_A.CellLoadingClass == GOOD_CELL 
			&& pCellInfo->m_Zone_A.ContactCurrentClass == GOOD_CELL
			&& pCellInfo->m_Zone_A.RegPinClass == GOOD_CELL
			&& pCellInfo->m_Zone_A.DicCheckClass == GOOD_CELL)
		{
			pCellInfo->m_strContactResult_1st = _T("GOOD");
			pCellInfo->m_strContactResult= _T("GOOD");
			bResult = TRUE;
		}
		else if((pCellInfo->m_Zone_A.CellLoadingClass != GOOD_CELL 
			|| pCellInfo->m_Zone_A.ContactCurrentClass != GOOD_CELL
			|| pCellInfo->m_Zone_A.RegPinClass != GOOD_CELL))
		{
			//2018-05-19,MGYUN, CONTACT_RETRY OFF시 1,2차 결과 모두 반영
			if (theConfigBank.m_Option.m_bUseContactRetry == FALSE)
			{
				pCellInfo->m_strContactResult_1st = _T("NG_CELL_LOADING");	
				pCellInfo->m_strContactResult= _T("NG_CELL_LOADING");
				bResult = FALSE;
			}
			else	//CONTACT_RETRY OPTION ON
			{
				pCellInfo->m_strContactResult_1st = _T("NG_CELL_LOADING");			
				bResult = FALSE;
			}
		}
		else if(pCellInfo->m_Zone_A.DicCheckClass != GOOD_CELL)
		{
			pCellInfo->m_strContactResult_1st = _T("NG_DIC_CHECK");	
			pCellInfo->m_strContactResult = _T("NG_DIC_CHECK");	
			bResult = FALSE;
		}
		else  //첫번째 컨택에서 ng이면 1차 결과 ng이고 다음 스텝으로 넘어감
		{
			pCellInfo->m_strContactResult_1st = _T("NG_CELL_LOADING");	
			bResult = FALSE;
		}		
	}
	else
	{
	if(pCellInfo->m_Zone_A.CellLoadingClass == GOOD_CELL 
			&& pCellInfo->m_Zone_A.ContactCurrentClass == GOOD_CELL
			&& pCellInfo->m_Zone_A.RegPinClass == GOOD_CELL
			&& pCellInfo->m_Zone_A.DicCheckClass == GOOD_CELL)
		{			
			pCellInfo->m_strContactResult= _T("GOOD");
			bResult = TRUE;
		}
		else if((pCellInfo->m_Zone_A.CellLoadingClass != GOOD_CELL 
			|| pCellInfo->m_Zone_A.ContactCurrentClass != GOOD_CELL
			|| pCellInfo->m_Zone_A.RegPinClass != GOOD_CELL))
		{
			pCellInfo->m_strContactResult = _T("NG_CELL_LOADING");			
			bResult = FALSE;
		}
		else if(pCellInfo->m_Zone_A.DicCheckClass != GOOD_CELL)
		{
			pCellInfo->m_strContactResult = _T("NG_DIC_CHECK");			
			bResult = FALSE;
		}
		else  //첫번째 컨택에서 ng이면 1차 결과 ng이고 다음 스텝으로 넘어감
		{
			pCellInfo->m_strContactResult = _T("NG_CELL_LOADING");
			bResult = FALSE;
		}	
	}
	
	return bResult;
}

BOOL CUnitCtrlBank::B_Zone_ContactChk(JIG_CH nCh)
{
	if(theConfigBank.m_Option.m_bUseContact == FALSE)
		return TRUE;
	if(theConfigBank.m_Option.m_bUseDryRun)
		return TRUE;

	BOOL bResult = FALSE;

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
	{
		return TRUE;
	}

	if(pCellInfo->m_nContactCnt == 0)
	{
		//첫번째 컨택에서 good이면 1차,2차 결과 모두 good
		if(pCellInfo->m_Zone_A.CellLoadingClass == GOOD_CELL 
			&& pCellInfo->m_Zone_A.ContactCurrentClass == GOOD_CELL
			&& pCellInfo->m_Zone_A.RegPinClass == GOOD_CELL
			&& pCellInfo->m_Zone_A.DicCheckClass == GOOD_CELL)
		{
			pCellInfo->m_strContactResult_1st = _T("GOOD");
			pCellInfo->m_strContactResult= _T("GOOD");
			bResult = TRUE;
		}
		else if((pCellInfo->m_Zone_A.CellLoadingClass != GOOD_CELL 
			|| pCellInfo->m_Zone_A.ContactCurrentClass != GOOD_CELL
			|| pCellInfo->m_Zone_A.RegPinClass != GOOD_CELL))
		{
			//2018-05-19,MGYUN, CONTACT_RETRY OFF시 1,2차 결과 모두 반영
			if (theConfigBank.m_Option.m_bUseContactRetry == FALSE)
			{
				pCellInfo->m_strContactResult_1st = _T("NG_CELL_LOADING");	
				pCellInfo->m_strContactResult= _T("NG_CELL_LOADING");
				bResult = FALSE;
			}
			else	//CONTACT_RETRY OPTION ON
			{
				pCellInfo->m_strContactResult_1st = _T("NG_CELL_LOADING");			
				bResult = FALSE;
			}
		}
		else if(pCellInfo->m_Zone_A.DicCheckClass != GOOD_CELL)
		{
			pCellInfo->m_strContactResult_1st = _T("NG_DIC_CHECK");	
			pCellInfo->m_strContactResult = _T("NG_DIC_CHECK");	
			bResult = FALSE;
		}
		else  //첫번째 컨택에서 ng이면 1차 결과 ng이고 다음 스텝으로 넘어감
		{
			pCellInfo->m_strContactResult_1st = _T("NG_CELL_LOADING");
			bResult = FALSE;
		}		
	}
	else
	{
		if(pCellInfo->m_Zone_A.CellLoadingClass == GOOD_CELL 
			&& pCellInfo->m_Zone_A.ContactCurrentClass == GOOD_CELL
			&& pCellInfo->m_Zone_A.RegPinClass == GOOD_CELL
			&& pCellInfo->m_Zone_A.DicCheckClass == GOOD_CELL)
		{			
			pCellInfo->m_strContactResult= _T("GOOD");
			bResult = TRUE;
		}
		else if((pCellInfo->m_Zone_A.CellLoadingClass != GOOD_CELL 
			|| pCellInfo->m_Zone_A.ContactCurrentClass != GOOD_CELL
			|| pCellInfo->m_Zone_A.RegPinClass != GOOD_CELL))
		{
			pCellInfo->m_strContactResult = _T("NG_CELL_LOADING");			
			bResult = FALSE;
		}
		else if(pCellInfo->m_Zone_A.DicCheckClass != GOOD_CELL)
		{
			pCellInfo->m_strContactResult = _T("NG_DIC_CHECK");			
			bResult = FALSE;
		}
		else  //첫번째 컨택에서 ng이면 1차 결과 ng이고 다음 스텝으로 넘어감
		{
			pCellInfo->m_strContactResult = _T("NG_CELL_LOADING");			
			bResult = FALSE;
		}		

		/*if(pCellInfo->m_Zone_A.CellLoadingClass == GOOD_CELL && pCellInfo->m_Zone_A.ContactCurrentClass == GOOD_CELL)
		{
			pCellInfo->m_strContactResult= _T("GOOD");
			bResult = TRUE;
		}
		else
		{			
			pCellInfo->m_strContactResult = _T("NG");			
			bResult = FALSE;
		}	
		*/
	}

	return bResult;
}

void CUnitCtrlBank::NGCountChk(JIG_CH nCh)
{
	if(theConfigBank.m_Option.m_bUseContact == FALSE)
		return;
	if(theConfigBank.m_Option.m_bUseDryRun)
		return;

	BOOL bResult = FALSE;

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
	{
		return;
	}

	if(pCellInfo->m_strContactResult == _T("NG"))
	{
		//20170730 JSJUNG, 지그별 컨택 NG count/3회 지정 : NG일때 count +1
		theProcBank.m_JigContactNGcount[pCellInfo->m_JigId][pCellInfo->m_JigCh]++;
		//20171017 JSJUNG, 지그별 컨택 NG count로그 저장
		theLog[LOG_SEQUENCE].AddBuf( _T("[ContactNGCount] Jig:%d, CH:%d, NGCount:%d"),pCellInfo->m_JigId,pCellInfo->m_JigCh,theProcBank.m_JigContactNGcount[pCellInfo->m_JigId][pCellInfo->m_JigCh]);
	}
	else
	{
		//20170730 JSJUNG, 지그별 컨택 NG count/3회 지정 : GOOD 일때 count 0
		theProcBank.m_JigContactNGcount[pCellInfo->m_JigId][pCellInfo->m_JigCh] = 0;
	}

	//20170730 JSJUNG, 지그별 컨택 NG count/3회 지정 : JigContactNGFlag 켜고 COUNT RESET
	if(theProcBank.m_JigContactNGcount[pCellInfo->m_JigId][pCellInfo->m_JigCh] >= theProcBank.m_JigContactNGcountMAX)
	{
		
		theProcBank.m_bJigContactNGFlag = TRUE;
		for (int i = JIG_ID_A; i < JIG_ID_MAX; i++)
		{
			for (int j = JIG_CH_1; j < JIG_CH_MAX; j++)
			{
				theProcBank.m_JigContactNGcount[i][j] = 0;
			}
		}
		//20170730 JSJUNG, 지그별 컨택 NG count/3회 지정 : JIG별 채널값 적용하여 알람표시

		if(pCellInfo->m_JigId == JIG_ID_A && pCellInfo->m_JigCh == JIG_CH_1)
			theProcBank.m_JigContactNGChName = 0;
		else if(pCellInfo->m_JigId == JIG_ID_A && pCellInfo->m_JigCh == JIG_CH_2)
			theProcBank.m_JigContactNGChName = 1;
		else if(pCellInfo->m_JigId == JIG_ID_B && pCellInfo->m_JigCh == JIG_CH_1)
			theProcBank.m_JigContactNGChName = 2;
		else if(pCellInfo->m_JigId == JIG_ID_B && pCellInfo->m_JigCh == JIG_CH_2)
			theProcBank.m_JigContactNGChName = 3;
		else if(pCellInfo->m_JigId == JIG_ID_C && pCellInfo->m_JigCh == JIG_CH_1)
			theProcBank.m_JigContactNGChName = 4;
		else if(pCellInfo->m_JigId == JIG_ID_C && pCellInfo->m_JigCh == JIG_CH_2)
			theProcBank.m_JigContactNGChName = 5;
		else if(pCellInfo->m_JigId == JIG_ID_D && pCellInfo->m_JigCh == JIG_CH_1)
			theProcBank.m_JigContactNGChName = 6;
		else if(pCellInfo->m_JigId == JIG_ID_D && pCellInfo->m_JigCh == JIG_CH_2)
			theProcBank.m_JigContactNGChName = 7;
	}

	return;
}

BOOL CUnitCtrlBank::IsJobEnd()
{
	BOOL bRet = (theProcBank.m_timerJobEnd.Stop(FALSE) > theConfigBank.m_Option.m_dJobEndTime) ? TRUE : FALSE;
	return (bRet && theConfigBank.m_Option.m_bUseJobEnd);
}

BOOL CUnitCtrlBank::IsCellLoadConv()
{
	BOOL bRet = (theProcBank.m_timerLoadConv.Stop(FALSE) > 10.) ? TRUE : FALSE;
	return bRet;
}

void CUnitCtrlBank::A_Zone_SetClass(JIG_CH nCh)
{
	CCellTag tagCell;
	CCellInfo* pCellInfo;

	if (nCh == JIG_CH_1)
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_AZONE_CH1);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_AZONE_CH1);
	}
	else if (nCh == JIG_CH_2)
	{
		tagCell = theCellBank.GetCellTag(CELL_POS_AZONE_CH2);
		pCellInfo = theCellBank.GetCellInfo(CELL_POS_AZONE_CH2);
	}

	if(tagCell.IsExist()==FALSE)
		return;

	if (pCellInfo->m_Zone_A.CellLoadingClass != GOOD_CELL
		|| pCellInfo->m_Zone_A.ContactCurrentClass != GOOD_CELL)
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

	////2018-05-17,SKCHO,수정
	////if (pCellInfo->m_Zone_A.CellLoadingClass != GOOD_CELL	|| pCellInfo->m_Zone_A.ContactCurrentClass != GOOD_CELL)
	//if (pCellInfo->m_Zone_A.CellLoadingClass != GOOD_CELL 
	//	|| pCellInfo->m_Zone_A.ContactCurrentClass != GOOD_CELL
	//	|| pCellInfo->m_Zone_A.RegPinClass != GOOD_CELL
	//	|| pCellInfo->m_Zone_A.DicCheckClass != GOOD_CELL)
	//{
	//	pCellInfo->m_Zone_A.AZoneClass = REJECT_CELL;
	//	pCellInfo->m_Zone_A.AZoneDefect = _T("NG");
	//	//pCellInfo->m_strContactResult = _T("NG");
	//}
	//else
	//{
	//	pCellInfo->m_Zone_A.AZoneClass = GOOD_CELL;
	//	pCellInfo->m_Zone_A.AZoneDefect = _T("GOOD");
	//	//pCellInfo->m_strContactResult = _T("GOOD");
	//}
}

void CUnitCtrlBank::A_Zone_TMDInfo(JIG_CH nCh)
{
	if(theConfigBank.m_Option.m_bUseDryRun)
		return;

	theUnitFunc.SendPGMessage(TMD_INFO, ZONE_ID_A, nCh);	
}
