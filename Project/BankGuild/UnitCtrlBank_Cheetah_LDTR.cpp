#include "StdAfx.h"
#include "UnitCtrlBank.h"
#include "CheetahApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CUnitCtrlBank::LDTRXGetPos()
{
	theDeviceMotion.TeachMove(m_nThreadID, TEACH_PARAM::LD_TR_X_GET);
}

BOOL CUnitCtrlBank::LDTRXGetPosChk()
{
	BOOL bRet = FALSE; 

	if(theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::LD_TR_X_GET) == TRUE)
		bRet = TRUE;

	return bRet;
}

void CUnitCtrlBank::LDTRXPutPos()
{
	theDeviceMotion.TeachMove(m_nThreadID, TEACH_PARAM::LD_TR_X_PUT);
}

BOOL CUnitCtrlBank::LDTRXPutPosChk()
{
	BOOL bRet = FALSE; 

	if(theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::LD_TR_X_PUT) == TRUE)
		bRet = TRUE;

	return bRet;
}

void CUnitCtrlBank::LDTR_X_LOWERMCRPos(JIG_CH nCh,double dSpeed)
{
	theDeviceMotion.TeachMove(m_nThreadID, (nCh == JIG_CH_1) ? TEACH_PARAM::LD_TR_X_CH1_LOWER_MCR : TEACH_PARAM::LD_TR_X_CH2_LOWER_MCR,dSpeed);
}

BOOL CUnitCtrlBank::LDTR_X_LOWERMCRPosChk(JIG_CH nCh)
{
	BOOL bRet = FALSE; 

	if(theDeviceMotion.CheckTeachPosition(m_nThreadID, (nCh == JIG_CH_1) ? TEACH_PARAM::LD_TR_X_CH1_LOWER_MCR : TEACH_PARAM::LD_TR_X_CH2_LOWER_MCR) == TRUE)
		bRet = TRUE;

	return bRet;
}

void CUnitCtrlBank::LDTR_X_UPPERMCRPos(JIG_CH nCh)
{
	theDeviceMotion.TeachMove(m_nThreadID, (nCh == JIG_CH_1) ? TEACH_PARAM::LD_TR_X_CH1_UPPER_MCR : TEACH_PARAM::LD_TR_X_CH2_UPPER_MCR);
}

BOOL CUnitCtrlBank::LDTR_X_UPPERMCRPosChk(JIG_CH nCh)
{
	BOOL bRet = FALSE; 

	if(theDeviceMotion.CheckTeachPosition(m_nThreadID, (nCh == JIG_CH_1) ? TEACH_PARAM::LD_TR_X_CH1_UPPER_MCR : TEACH_PARAM::LD_TR_X_CH2_UPPER_MCR) == TRUE)
		bRet = TRUE;

	return bRet;
}

void CUnitCtrlBank::LDTRZGetPos()
{
	theDeviceMotion.TeachMove(m_nThreadID, TEACH_PARAM::LD_TR_Z_GET);
}

BOOL CUnitCtrlBank::LDTRZGetPosChk()
{
	BOOL bRet = FALSE; 

	if(theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::LD_TR_Z_GET) == TRUE)
		bRet = TRUE;

	return bRet;
}

void CUnitCtrlBank::LDTRZUpPos(double dSpeed)
{
	theDeviceMotion.TeachMove(m_nThreadID, TEACH_PARAM::LD_TR_Z_UP, dSpeed);
}

BOOL CUnitCtrlBank::LDTRZUpPosChk()
{
	BOOL bRet = FALSE; 

	if(theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::LD_TR_Z_UP) == TRUE)
		bRet = TRUE;

	return bRet;
}

void CUnitCtrlBank::LDTRZPutPos(double Offset)
{
	theDeviceMotion.TeachMove(m_nThreadID, TEACH_PARAM::LD_TR_Z_PUT,Offset);
}

BOOL CUnitCtrlBank::LDTRZPutPosChk(double Offset)
{
	BOOL bRet = FALSE; 

	if(theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::LD_TR_Z_PUT,0.05,POS_EQUAL,Offset) == TRUE)
		bRet = TRUE;

	return bRet;
}

void CUnitCtrlBank::LDTR_Z_LOWERMCRPos()
{
	theDeviceMotion.TeachMove(m_nThreadID, TEACH_PARAM::LD_TR_Z_LOWER_MCR);
}

BOOL CUnitCtrlBank::LDTR_Z_LOWERMCRPosChk()
{
	BOOL bRet = FALSE; 

	if(theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::LD_TR_Z_LOWER_MCR) == TRUE)
		bRet = TRUE;

	return bRet;
}

void CUnitCtrlBank::LDTR_Z_UPPERMCRPos()
{
	theDeviceMotion.TeachMove(m_nThreadID, TEACH_PARAM::LD_TR_Z_UPPER_MCR);
}

BOOL CUnitCtrlBank::LDTR_Z_UPPERMCRPosChk()
{
	BOOL bRet = FALSE; 

	if(theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::LD_TR_Z_UPPER_MCR) == TRUE)
		bRet = TRUE;

	return bRet;
}

void CUnitCtrlBank::LDTRVacOn(BOOL bVacOn, JIG_CH Ch)
{
	if(Ch == JIG_CH_1)
	{
		SetOutPutIO(Y_LD_TR_CH1_VAC_ON_SOL, bVacOn);
		SetOutPutIO(Y_LD_TR_CH1_BLOW_ON_SOL, !bVacOn);
	}
	else if(Ch == JIG_CH_2)
	{
		SetOutPutIO(Y_LD_TR_CH2_VAC_ON_SOL, bVacOn);
		SetOutPutIO(Y_LD_TR_CH2_BLOW_ON_SOL, !bVacOn);
	}
	else
	{
		SetOutPutIO(Y_LD_TR_CH1_VAC_ON_SOL, bVacOn);
		SetOutPutIO(Y_LD_TR_CH1_BLOW_ON_SOL, !bVacOn);
		SetOutPutIO(Y_LD_TR_CH2_VAC_ON_SOL, bVacOn);
		SetOutPutIO(Y_LD_TR_CH2_BLOW_ON_SOL, !bVacOn);
	}
}

void CUnitCtrlBank::LDTR_BlowOn(JIG_CH Ch)
{
	if(Ch == JIG_CH_1)
	{
		SetOutPutIO(Y_LD_TR_CH1_BLOW_ON_SOL, TRUE);
	}
	else if(Ch == JIG_CH_2)
	{
		SetOutPutIO(Y_LD_TR_CH2_BLOW_ON_SOL, TRUE);
	}
	else
	{
		SetOutPutIO(Y_LD_TR_CH1_BLOW_ON_SOL, TRUE);
		SetOutPutIO(Y_LD_TR_CH2_BLOW_ON_SOL, TRUE);
	}
}
void CUnitCtrlBank::LDTR_BlowOff(JIG_CH Ch)
{
	if(Ch == JIG_CH_1)
	{
		SetOutPutIO(Y_LD_TR_CH1_BLOW_ON_SOL, FALSE);
	}
	else if(Ch == JIG_CH_2)
	{
		SetOutPutIO(Y_LD_TR_CH2_BLOW_ON_SOL, FALSE);
	}
	else
	{
		SetOutPutIO(Y_LD_TR_CH1_BLOW_ON_SOL, FALSE);
		SetOutPutIO(Y_LD_TR_CH2_BLOW_ON_SOL, FALSE);
	}
}

BOOL CUnitCtrlBank::LDTRCellVacChk(JIG_CH Ch,VAC_STATE Vac)
{
	if(theConfigBank.m_Option.m_bUseDryRun)
		return TRUE;
	if(Ch == JIG_CH_1)
	{
		if (Vac == VAC_ON)
			return GetInPutIOCheck(X_LD_TR_CH1_VAC_SEN);
		else
			return !GetInPutIOCheck(X_LD_TR_CH1_VAC_SEN);
	}
	else if(Ch == JIG_CH_2)
	{
		if (Vac == VAC_ON)
			return GetInPutIOCheck(X_LD_TR_CH2_VAC_SEN);
		else
			return !GetInPutIOCheck(X_LD_TR_CH2_VAC_SEN);
	}
	return TRUE;
}


void CUnitCtrlBank::LDTRFPCBBlowOn(BOOL bBlowOn , BOOL bCh1, BOOL bCh2)
{
#if(AMT_TYPE == 2)
	if (bCh1 == TRUE)
		SetOutPutIO(Y_LD_TR_CH1_FPCB_BLOW_SOL, bBlowOn);
	if (bCh2 == TRUE)
		SetOutPutIO(Y_LD_TR_CH2_FPCB_BLOW_SOL, bBlowOn);
#endif
}

void CUnitCtrlBank::Trigger(MCR_UNIT Unit)
{
	if(theProcBank.m_nMCRChannel == JIG_CH_1)
	{
		theProcBank.m_strCh1LastCellID.Empty();
	}
	else
	{
		theProcBank.m_strCh2LastCellID.Empty();
	}
			
	
	// ���� ���ִ� Ʈ���� �Լ� ��� �Ѵ�. 2017/04/25 HSK
	SetSoftTrigger(Unit);

}

void CUnitCtrlBank::Trigger(MCR_UNIT Unit,JIG_CH nCh)
{
	if(nCh == JIG_CH_1)
	{
		theProcBank.m_strCh1LastCellID.Empty();
		theProcBank.m_nMCRChannel = JIG_CH_1;
	}
	else
	{		
		theProcBank.m_strCh2LastCellID.Empty();
		theProcBank.m_nMCRChannel = JIG_CH_2;
	}
	// ���� ���ִ� Ʈ���� �Լ� ��� �Ѵ�. 2017/04/25 HSK
	SetSoftTrigger(Unit);

	//kjpark 20161027 IO Output check bug ����
/*
	if (Unit == LOWER_MCR)
	{
		SetOutPutIO((OUTPUT_ID) Y_MCR_LOWER_CAM_TRIGGER, On);						// UV-MCR #1 Read Trigger signal;
	}
	else
	{
		SetOutPutIO((OUTPUT_ID) Y_MCR_UPPER_CAM_TRIGGER, On);						// UV-MCR #1 Read Trigger signal;
	}
*/
}

CString	CUnitCtrlBank::GetSoftTriggerData(MCR_UNIT Unit)
{
	if ( GetMainHandler()->m_pIDReader[Unit] )
	{
		if ( GetMainHandler()->m_pIDReader[Unit]->IsConnect() )
		{
			return GetMainHandler()->m_pIDReader[Unit]->GetReadString();
		}
	}

	return _T("");
}

int	CUnitCtrlBank::GetSoftTriggerResult(MCR_UNIT Unit)
{
	if ( GetMainHandler()->m_pIDReader[Unit] )
	{
		if ( GetMainHandler()->m_pIDReader[Unit]->IsConnect() )
		{
			return GetMainHandler()->m_pIDReader[Unit]->GetReadResult();
		}
	}

	return 0;
}

CString CUnitCtrlBank::MCR_GetLastID(JIG_CH nCh)
{
	CString strResult;
	if(nCh == JIG_CH_1)
	{
		strResult = theProcBank.m_strCh1LastCellID;		
	}
	else
	{
		strResult = theProcBank.m_strCh2LastCellID;		
	}
	return strResult;
}

// Live ȭ���� �������ش�.
BOOL CUnitCtrlBank::SetLiveMode(BOOL bFlag,MCR_UNIT Unit)
{
	if ( GetMainHandler()->m_pIDReader[Unit] )
	{
		if ( GetMainHandler()->m_pIDReader[Unit]->IsConnect() )
		{
			if ( bFlag )			// Live On ��û
			{
				if ( GetMainHandler()->m_pIDReader[Unit]->IsLiveOn() == FALSE )
				{
					GetMainHandler()->m_pIDReader[Unit]->LiveOn();
				}

				return TRUE;
			}

			GetMainHandler()->m_pIDReader[Unit]->LiveOff();

			return TRUE;
		}
	}

	return FALSE;
}

// Live mode�ΰ� ?
BOOL CUnitCtrlBank::GetLiveMode(MCR_UNIT Unit)
{
	if ( GetMainHandler()->m_pIDReader[Unit] )
	{
		if ( GetMainHandler()->m_pIDReader[Unit]->IsConnect() )
		{
			return GetMainHandler()->m_pIDReader[Unit]->IsLiveOn();
		}

	}

	return FALSE;	
}

// ��ſ� ���� Trigger ��ȣ�� �����ش�.
BOOL CUnitCtrlBank::SetSoftTrigger(MCR_UNIT Unit)
{
	if ( GetMainHandler()->m_pIDReader[Unit] )
	{
		if ( GetMainHandler()->m_pIDReader[Unit]->IsConnect() )
		{
			if ( GetMainHandler()->m_pIDReader[Unit]->IsLiveOn() )
			{
				GetMainHandler()->m_pIDReader[Unit]->LiveOff();
				SleepEx(200, NULL);
			}

			GetMainHandler()->m_pIDReader[Unit]->Trigger();

			return TRUE;
		}
	}

	return FALSE;
}


// ��� ȭ���� �����Ѵ�.
BOOL CUnitCtrlBank::SaveResultImage(CString sFile,MCR_UNIT Unit)
{
	if ( GetMainHandler()->m_pIDReader[Unit] )
	{
		if ( GetMainHandler()->m_pIDReader[Unit]->IsConnect() )
		{
			GetMainHandler()->m_sLastSavedImage[Unit] = sFile;// = _T("O:\\123.jpg");					// �������� ����� �̹��� �����̸�
			GetMainHandler()->m_pIDReader[Unit]->SaveImage(sFile);

			return TRUE;
		}
	}

	return FALSE;
}

void CUnitCtrlBank::SaveMCRNGImage(MCR_UNIT Unit,JIG_CH nCh)
{
	CCellTag tagCell;
	CCellInfo* pCellInfo;
	CString FilePath;
	SYSTEMTIME	time;
	GetLocalTime(&time);
	
	if (Unit == LOWER_MCR)
	{
		if (nCh == JIG_CH_1)
		{
			tagCell = theCellBank.GetCellTag(CELL_POS_LD_TR_CH1);
			pCellInfo = theCellBank.GetCellInfo(CELL_POS_LD_TR_CH1);
		}
		else if (nCh == JIG_CH_2)
		{
			tagCell = theCellBank.GetCellTag(CELL_POS_LD_TR_CH2);
			pCellInfo = theCellBank.GetCellInfo(CELL_POS_LD_TR_CH2);
		}
	}
	else
	{
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
	}
	
	if(tagCell.IsExist() == FALSE) //2017-04-13,skcho, ��Ÿ�� ���� �߻��Ͽ� �����ľ��ϱ� ���ؼ� ����
	{
		theLog[LOG_UV_MCR].AddBuf(_T("UV_MCR CELL NOT EXIST"));
		return;
	}

	if ( GetMainHandler()->m_pIDReader[Unit] )
	{
		if ( GetMainHandler()->m_pIDReader[Unit]->IsConnect() )
		{
			if (Unit == LOWER_MCR)
				FilePath.Format(_T("D:\\Cheetah\\Image\\MCRNG\\Lower\\%04u-%02u-%02u"),time.wYear,time.wMonth,time.wDay);
			else
				FilePath.Format(_T("D:\\Cheetah\\Image\\MCRNG\\Upper\\%04u-%02u-%02u"),time.wYear,time.wMonth,time.wDay);
			
			if(CFileSupport::DirectoryCheck(FilePath) == FALSE)
			{
				if(CFileSupport::CreateDirectory(FilePath) == FALSE)
					return;
			}

			if (Unit == LOWER_MCR)
				FilePath.Format(_T("D:\\Cheetah\\Image\\MCRNG\\Lower\\%04u-%02u-%02u\\%s.jpg"),time.wYear,time.wMonth,time.wDay,pCellInfo->m_strInnerID);
			else
				FilePath.Format(_T("D:\\Cheetah\\Image\\MCRNG\\Upper\\%04u-%02u-%02u\\%s.jpg"),time.wYear,time.wMonth,time.wDay,pCellInfo->m_strInnerID);

			// �̹��� ���̺� �Լ��� ����ȣ��Ǽ� DEBUG �߻� �� ���� �ִٰ� �Ѵ�. �������� 17/04/25 HSK
			if(Unit == LOWER_MCR)
				CopyFile(LOWER_MCRPath, FilePath, FALSE);
			else
				CopyFile(UPPER_MCRPath, FilePath, FALSE);

//			�̹��� �����ϴ� �Լ��� ���� �־ �Լ��� ����. 17/04/22
//			GetMainHandler()->m_sLastSavedImage[Unit] = FilePath;
//			GetMainHandler()->m_pIDReader[Unit]->SaveImage(FilePath);
//			SaveResultImage(FilePath,Unit);

			// �α� ���� �κ��� Ŀ��Ʈ �Ȱ� üũ ��� ���� ���´�...Ŀ��Ʈ �ȉ��� �� FileFath�� ������ ���̶�....������...17/04/22
			theLog[LOG_UV_MCR].AddBuf(FilePath);
		}

	}
}

// �������� ����� �̹��� �����̸�
CString CUnitCtrlBank::GetLastSavedImage(MCR_UNIT Unit)
{
	return	GetMainHandler()->m_sLastSavedImage[Unit];					
}

void CUnitCtrlBank::CellData_SetUVResult(MCR_UNIT Pos,JIG_CH nCh,MCR_JUDGE Success)
{
	CCellTag tagCell;
	CCellInfo* pCellInfo;

	if (Pos == LOWER_MCR)
	{
		if (nCh == JIG_CH_1)
		{
			tagCell = theCellBank.GetCellTag(CELL_POS_LD_TR_CH1);
			pCellInfo = theCellBank.GetCellInfo(CELL_POS_LD_TR_CH1);
		}
		else if (nCh == JIG_CH_2)
		{
			tagCell = theCellBank.GetCellTag(CELL_POS_LD_TR_CH2);
			pCellInfo = theCellBank.GetCellInfo(CELL_POS_LD_TR_CH2);
		}
	}
	else
	{
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
	}

	if(tagCell.IsExist()==FALSE)
		return ;

	pCellInfo->bUVReadingSuccess = Success; 

	if (Success == MCR_RESULT_GOOD)
	{
		//pCellInfo->m_strCellID = MCR_GetLastID();
		pCellInfo->m_strCellID = MCR_GetLastID(nCh); //2017-05-23,skcho ,ä�κ� ������ ����
		//2017-09-17,SKCHO, ����� �����̸� ī��Ʈ �ʱ�ȭ
		if (Pos == LOWER_MCR)
		{
			if(nCh == JIG_CH_1)
			{
				theProcBank.m_nCH1LowerMCRFailCnt = 0;	
			}
			else
			{
				theProcBank.m_nCH2LowerMCRFailCnt = 0;
			}
		}
		else
		{
			if(nCh == JIG_CH_1)
			{
				theProcBank.m_nCH1UpperMCRFailCnt = 0;
			}
			else
			{
				theProcBank.m_nCH2UpperMCRFailCnt =0;
			}
		}
	}
	//2017-09-17,SKCHO
	else if(Success == MCR_RESULT_SKIP)
	{
		pCellInfo->m_strCellID = SKIP;
		//2017-09-17,SKCHO, ����� �����̸� ī��Ʈ �ʱ�ȭ
		if (Pos == LOWER_MCR)
		{
			theProcBank.m_nCH1LowerMCRFailCnt = 0;	
			theProcBank.m_nCH2LowerMCRFailCnt = 0;
		}
		else
		{		
			theProcBank.m_nCH1UpperMCRFailCnt = 0;
			theProcBank.m_nCH2UpperMCRFailCnt = 0;			
		}
	}
	else
	{
		//pCellInfo->m_strCellID = TEXT_UV_FAIL; //2017-04-05,skcho, �ڱ״н����� FAIL �� �����͸� ��
		pCellInfo->m_strCellID = FAIL;

		//2017-09-17,SKCHO, ����� FAIL�̸� ī��Ʈ ����
		if (Pos == LOWER_MCR)
		{
			if(nCh == JIG_CH_1)
			{
				theProcBank.m_nCH1LowerMCRFailCnt++;	
			}
			else
			{
				theProcBank.m_nCH2LowerMCRFailCnt++;
			}
		}
		else
		{
			if(nCh == JIG_CH_1)
			{
				theProcBank.m_nCH1UpperMCRFailCnt++;
			}
			else
			{
				theProcBank.m_nCH2UpperMCRFailCnt++;
			}
		}
	}

	if (Pos == LOWER_MCR)
	{
		pCellInfo->strReadID = _T("1");
		pCellInfo->m_strReadUnitMCR = _T("LOWER");
	}
	else
	{
		pCellInfo->strReadID = _T("2");
		pCellInfo->m_strReadUnitMCR = _T("UPPER");
	}

	CString strbuf;
	if (pCellInfo->bUVReadingSuccess == MCR_RESULT_NONE)
		strbuf = _T("MCR_NONE");
	else if (pCellInfo->bUVReadingSuccess == MCR_RESULT_GOOD)
		strbuf = _T("MCR_GOOD");
	else if (pCellInfo->bUVReadingSuccess == MCR_RESULT_SKIP)
		strbuf = _T("MCR_SKIP");
	else 
		strbuf = _T("MCR_FAIL");

	theLog[LOG_UV_MCR].AddBuf(_T("Reader Unit : %s, Ch : %d, Success : %s, Cell ID, %s"),pCellInfo->m_strReadUnitMCR,nCh,strbuf,pCellInfo->m_strCellID);

	//mcr ����� main���� ����
	::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_MCR_UPDATE, nCh,Pos);
	
	//2018-03-10,MGYUN, MCR FAIL or CELL ID MELSEC WRITE
	//2018-03-13,MGYUN, ����� ���, CH1 �ϳ��� ��� ����Ͽ� ����.
	//theMonitorBank.WriteMCRCellID(nCh,pCellInfo->m_strCellID);
	theMonitorBank.WriteMCRCellID(JIG_CH_1,pCellInfo->m_strCellID);
	theMonitorBank.WriteMCRCellID(JIG_CH_2,pCellInfo->m_strCellID);
}

MCR_JUDGE CUnitCtrlBank::CellData_UVReadingChk(MCR_UNIT Pos,JIG_CH nCh)
{
	CCellTag tagCell;
	CCellInfo* pCellInfo;
	if (Pos == LOWER_MCR)
	{
		if (nCh == JIG_CH_1)
		{
			tagCell = theCellBank.GetCellTag(CELL_POS_LD_TR_CH1);
			pCellInfo = theCellBank.GetCellInfo(CELL_POS_LD_TR_CH1);
		}
		else if (nCh == JIG_CH_2)
		{
			tagCell = theCellBank.GetCellTag(CELL_POS_LD_TR_CH2);
			pCellInfo = theCellBank.GetCellInfo(CELL_POS_LD_TR_CH2);
		}
	}
	else 
	{
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
	}

	if((tagCell.IsExist()==FALSE) || (theConfigBank.m_Option.m_bUseUpperMCRRead == FALSE))
		return MCR_RESULT_GOOD;

	return pCellInfo->bUVReadingSuccess;
}
