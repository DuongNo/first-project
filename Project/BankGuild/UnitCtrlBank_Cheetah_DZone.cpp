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

	//2017-04-05, skcho, �����˻� �ǽ¿��� ��û
	//MCR �бⰡ �����ϸ� �˻縦 ���� �ʱ� ������ MCR READING FAIL�� ǥ���Ѵ�.
	if (Get_InspectSkip(Pos) == TRUE)
	{
		if(pCell->m_strCellID ==_T("FAIL") || pCell->m_strCellID ==_T(""))
		//if(pCell->m_strCellID ==_T("UV_FAIL") || pCell->m_strCellID ==_T(""))	//2017-04-05,skcho, �ڱ״н����� FAIL �� �����͸� ��
		{
			pCell->m_LastClass = BIN2_CELL;
		    pCell->m_LastResult = TEXT_MCR_READING_FAIL;  //MCR_READING_FAIL�� ǥ��
		}
		//CONTACT�� CELL LOADING��  CURRENT CHECK�� �����⶧���� �������� CURRNT CLASS�� �����ϰ� NG�ڵ�� CONTACT NG�� �Ѵ�.
		//2018-08-02,SKCHO, CONTACT_NG �Ǵ� ���� ����, DIC_CHECK�� �����ϱ� ���ؼ�
		else if (pCell->m_Zone_A.CellLoadingClass != GOOD_CELL || pCell->m_Zone_A.ContactCurrentClass != GOOD_CELL)
		//else if (pCell->m_Zone_A.AZoneClass != GOOD_CELL || pCell->m_Zone_A.ContactCurrentClass != GOOD_CELL)
		{			
			pCell->m_LastClass = REJECT_CELL;//pCell->m_Zone_A.AZoneClass;
			pCell->m_LastResult = TEXT_CONTACT_NG;   //CONTACT NG�� ǥ��
		}
		//2018-05-17,SKCHO
		else if (pCell->m_Zone_A.DicCheckClass != GOOD_CELL)	
		{
			pCell->m_LastClass = REJECT_CELL;//pCell->m_Zone_A.AZoneClass;
			pCell->m_LastResult = TEXT_DIC_CHECK_NG;   //DIC_CHECK NG�� ǥ��
		}
		//2017-05-19,skcho, RCMD �����̸� 'O'����, ����NG���� �켱��,MCR FAIL�� �ƴ� ���
		if (Get_RCMDResult(Pos) == FALSE && pCell->m_strCellID !=_T("FAIL"))
		{
			pCell->m_LastClass = BIN2_CELL;
		    pCell->m_LastResult = _T("INSPECTION FAIL"); //RCMD������ ���� ��� ǥ��
		}		
	}
	//CONTACT NG�� ��� �˻� ������ ���ʿ� �ϹǷ� INSPCETION_SIKP ������ ����  2017-04-07,skcho, ���� �ɼ��� ��û
	//CONTACT�� CELL LOADING��  CURRENT CHECK�� �����⶧���� �������� CURRNT CLASS�� �����ϰ� NG�ڵ�� CONTACT NG�� �Ѵ�.
	/*else if (pCell->m_Zone_A.AZoneClass != GOOD_CELL || pCell->m_Zone_A.ContactCurrentClass != GOOD_CELL)
	{
		//pCell->m_LastClass = pCell->m_Zone_A.AZoneClass;
		//pCell->m_LastResult = pCell->m_Zone_A.AZoneDefect;   //CONTACT NG�� ǥ��
		pCell->m_LastClass = REJECT_CELL;//pCell->m_Zone_A.AZoneClass;
		pCell->m_LastResult = TEXT_CONTACT_NG;   //CONTACT NG�� ǥ��
	}
	*/
	else
	{
		pCell->m_LastClass = pCell->m_Zone_C.CZoneClass;
		pCell->m_LastResult = pCell->m_Zone_C.CZoneDefect;
	}

	// Judge OK ���� �ɼǽÿ� ������ ������ GOOD���� ���ش�. 17/04/22 HSK.
	if(theConfigBank.m_Option.m_bUseJudgeOK)
	{
		pCell->m_LastClass = GOOD_CELL;
		//2017-08-01,JSJUNG, ����OK ������� �˻����� GOOD���� ����
		pCell->m_LastResult =_T("GOOD");
	}

	//2018-02-12 JSJUNG, GOOD_CELL�� �ƴѰ�쿡�� MES�ڵ�����
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
		//if(pCell->m_LastResult.Find(itmap->second.strPopup) == 0) //���� ������ ��� ���
		if(strDefectName == itmap->second.strPopup)
		{
			return itmap->second.strMES_CODE;
		}
		else if(TEXT_UNDEFINED == itmap->second.strPopup)
		{
			strUndefinedCode = itmap->second.strMES_CODE;
		
			//2018-02-12 JSJUNG, MESCODE��Ī�Ǵ°��� ������쿡�� GDCAMTE ����
			//strUndefinedCode.Format(_T("GDCAMTE"));
		}
	}
	// ��ã�� ��� �⺻�� UNDEFINED�� �ҷ��ڵ� [11/28/2016 OSC]
	return strUndefinedCode;
}


void CUnitCtrlBank::WriteCellLog( CELL_POS pos )
{
	theCellBank.WriteCellLog(pos);
}

BOOL CUnitCtrlBank::TurnOffChk(JIG_CH ch)
{
//  ������ ��� �߰� üũ 2017/04/26 HSK ���� ����.
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