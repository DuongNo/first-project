#include "StdAfx.h"
#include "StateAMT.h"

#include "StdAfx.h"
#include "StateUnloader.h"
#include "MainFrm.h"

//Duong
#include "BankGuild/TrayBank.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW

#endif

int CStateJobChange::Run()
{
	CCellInfo *pCellInfo = NULL;
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{
	case stepStart:
		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"), m_strStateName);	
		theUnitFunc.CIM_CellLot_Info_Request();  //S6F205
		m_Timer.Start();
		nStep++;
		break;
	case stepJob2InfoChk:  //s3f109
		m_bRtn[0] = (theProcBank.m_strProductID == _T("")) ? FALSE : TRUE;
		if (IsReturnOk())//product ID ����?
		{		

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] JobChangeCase2 Cell Lot Info Check"), m_strStateName);	

			theConfigBank.m_UnloadRunInfo.m_sCurrentProduct = theProcBank.m_strProductID ;//PRODUCT ID ����
			theConfigBank.m_UnloadRunInfo.m_sCurrentPPID = theProcBank.m_strPPID ; //PPID ����
			theConfigBank.m_UnloadRunInfo.SaveUnloadRuleFileInfo();	

			theProcBank.m_strProductID  = _T("");  //2017-03-11,skcho ���� jobchange�� ���ؼ� �ʱ�ȭ
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 10.)
		{		
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] JobChangeCase2 GO to NG"), m_strStateName);	
			//Ÿ�� �ƿ��� ��� NG ��Ʈ�� ��
			pCellInfo = theCellBank.GetCellInfo(CELL_POS_UD_ROBOT);
			if(pCellInfo)
			{
				pCellInfo->m_LastClass = NONE_CELL;
			}
			nStep = stepIdle;
		}		
		break;
	case stepJob2Attri:
		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] JobChangeCase2 Attribute Request"), m_strStateName);	
		theUnitFunc.CIM_Attribute_Request();
		m_Timer.Start();
		nStep++;
		break;
	case stepJob2AttriChk:
		if(m_Timer.Stop(FALSE) > 10.)
		{
			// FAIL �߰� �ؾ� ��. 2017/04/27 HSK
			//m_bRtn[0] = (theProcBank.m_strReplyCode == _T("PASS")) ? TRUE : FALSE;
			if (theProcBank.m_strReplyCode == _T("PASS"))//product ID ����?
			{		
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] JobChangeCase2 Attribute Check"), m_strStateName);	
				theConfigBank.m_UnloadRunInfo.m_sTrayQtyInLot = theProcBank.m_strTrayQty ;//PRODUCT ID ����
				theConfigBank.m_UnloadRunInfo.m_sCellQtyInTray = theProcBank.m_strCellQty ; //PPID ����
				theConfigBank.m_UnloadRunInfo.m_sNextStepID = NEXTSTEPID; 
				theConfigBank.m_UnloadRunInfo.SaveUnloadRuleFileInfo();
				theProcBank.m_strReplyCode = _T("-1");//2017-03-11,skcho ���� jobchange�� ���ؼ� �ʱ�ȭ
				nStep++;
			}	
			else if(theProcBank.m_strReplyCode == _T("FAIL"))
			{
				// Light �˶� �߻� �ÿ��� ������ ������ �ʰ� ����. 2017/05/01 HSK.
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] JobChangeCase2 Attribute FAIL"), m_strStateName);				
				theProcBank.m_bLightAlarmChk = TRUE;
				SetError(ALARM_CIM_ATTRIBUTE_FAIL);
				nStep++;
			}
			else if(m_Timer.Stop(FALSE) > 10.)
			{		
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] JobChangeCase2 Attribute TimeOut"), m_strStateName);				
				nStep++;
			}
		}
		break;
	case stepEnd:
		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"), m_strStateName);
		theConfigBank.m_UnloadRunInfo.SetJobChangeType(JOBCHANGE_NONE);
		nStep = stepIdle;
		break;
	}
	m_nStep = nStep;

	return nStep;
}

int CStateUDTRGet::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	m_bCH1_Cell = CellTagExist(CELL_POS_DZONE_CH1);
	m_bCH2_Cell = CellTagExist(CELL_POS_DZONE_CH2);

	switch(nStep)
	{
	case stepStart:		// D Zone�� Cell �� �ְ� Zone End ���� üũ �� UD TR X Get �̵�.
		m_bRtn[0] = m_bCH1_Cell || m_bCH2_Cell;
		m_bRtn[1] = GetZoneEnd(ZONE_ID_D);
		if(IsReturnOk())
		{
			if(theProcBank.m_nULDTRRetryStep == 0)
			{
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"), m_strStateName);
			}
			else
			{
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart - Retry"), m_strStateName);
			}

			MoveUnloadTR_X_GetPos();
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)			SetError(ALARM_DZONE_CELL_NOT_EXIST);
			else if(m_bRtn[1] == FALSE)		SetError(ALARM_DZONE_END_CHECK);
		}
		break;
	case stepZDown:		// UD TR X �̵� üũ ��  Z Down.
		m_bRtn[0] = MoveUnloadTR_X_GetPosChk();
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepZDown"), m_strStateName);

			MoveUnLoadTR_Z_GetPos();
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)		SetError(ALARM_ULD_TR_X_GET_POS);
		}
		break;
	case stepVacOn:		// Z Down Check �� UD TR Vac On, D Zone Vac Off.
		m_bRtn[0] = MoveUnLoadTR_Z_GetPosChk();
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepVacOn"), m_strStateName);

			// DZone Vacuum Off
			AMTVacOn(VAC_OFF, AMT_GetAutoJigID(ZONE_ID_D), JIG_CH_1);
			AMTVacOn(VAC_OFF, AMT_GetAutoJigID(ZONE_ID_D), JIG_CH_2);

			// ULD TR Vacuum On
			// �� ���� �� ��ο� �Ҹ� Vac�� ��ũ����ٰ� �Ѵ�...
			if(m_bCH1_Cell)
				UnloadTRVacOnOff(JIG_CH_1, m_bCH1_Cell);
			if(m_bCH2_Cell)
				UnloadTRVacOnOff(JIG_CH_2, m_bCH2_Cell);

			m_Timer.Start();
			//2017-09-19,SKCHO, ������ ��� ����̸� �������� �ƴϸ�
			if(theConfigBank.m_Option.m_bUseULDTRMode)
			{
				nStep++;
			}
			else
			{
				nStep = stepZUp;
			}
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)		SetError(ALARM_ULD_TR_Z_GET_POS);
		}
		break;
		// ���� ���� �����Ͽ� Z�� ��.
	case stepZUp1:		// UD TR Vac �� D Zone Vac Check �� Z up.
		// UD TR CH1, CH2 Vac On Check
		m_bRtn[0] = UnloadTRVacChk(VAC_ON, m_bCH1_Cell, m_bCH2_Cell);
		m_bRtn[1] = m_Timer.Stop(FALSE) > 0.1 ? TRUE :FALSE;

		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepZUp1"), m_strStateName);

			UnloadTR_BlowOff(JIG_CH_MAX);

			// Move ULD TR Slow Up
			MoveUnLoadTR_ZAxis_Up_Slow(-25.);

			//2017-12-22,SKCHO JIGBLOW �ɼ��߰�
			if(theConfigBank.m_Option.m_bUseJigBlow == TRUE)
			{
				//2017-11-17,SKCHO, DZONE BLOWER ON - �ⱸ�۾� �Ϸ�Ǹ� ����
				if(m_bCH1_Cell)
					AMTBlowerOnOff(BLOW_ON, AMT_GetAutoJigID(ZONE_ID_D), JIG_CH_1);
				if(m_bCH2_Cell)
					AMTBlowerOnOff(BLOW_ON, AMT_GetAutoJigID(ZONE_ID_D), JIG_CH_2);	
			}
			

			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)
			{
				SetError(ALARM_ULD_TR_VAC);				
			}
		}
		break;
	case stepZUp1Chk:
		// Move ULD TR Slow Up Check
		m_bRtn[0] = MoveUnLoadTR_ZAxis_Up_SlowChk(-25.);
		m_bRtn[1] = UnloadTRVacChk(VAC_ON, m_bCH1_Cell, m_bCH2_Cell);

		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepZUp1Chk"), m_strStateName);

			// ULD TR Fast Up 2017-09-19,SKCHO, ��ġ �̵�
			//MoveUnLoadTR_ZAxis_Up();

			//2017-12-22,SKCHO JIGBLOW �ɼ��߰�
			if(theConfigBank.m_Option.m_bUseJigBlow == TRUE)
			{
				//2017-11-17,SKCHO, DZONE BLOWER ON - �ⱸ�۾� �Ϸ�Ǹ� ����
				if(m_bCH1_Cell)
					AMTBlowerOnOff(BLOW_OFF, AMT_GetAutoJigID(ZONE_ID_D), JIG_CH_1);
				if(m_bCH2_Cell)
					AMTBlowerOnOff(BLOW_OFF, AMT_GetAutoJigID(ZONE_ID_D), JIG_CH_2);
			}
			
			//2017-12-01,SKCHO
			if(theProcBank.m_nULDTRRetryStep == 1)
			{
				theProductBank.m_ULDTR_RetryInfo.nSucess++;
			}

			theProcBank.m_nULDTRRetryStep = 0;

			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)		SetError(ALARM_ULD_TR_Z_UP_POS);
			else if(m_bRtn[1] == FALSE)	  //��Ũ�˶� �߻���
			{
				
				if(theProcBank.m_nULDTRRetryStep == 0)
				{
					theProcBank.m_nULDTRRetryStep =1;
					m_Timer.Start();   //2017-09-19,SKCHO,��Ũ�˶��� ������ x ������ üũ�ϰ� z get �������̵�
					nStep  =stepStart;

					//2017-12-01,SKCHO
					theProductBank.m_ULDTR_RetryInfo.nTotal++;

				}
				else if(theProcBank.m_nULDTRRetryStep == 1)
				{
					theProcBank.m_nULDTRRetryStep = 0;
					SetError(ALARM_ULD_TR_VAC);
				}
				
			}
		}
		break;
	case stepZUp:		// UD TR Vac �� D Zone Vac Check �� Z up.
		// UD TR CH1, CH2 Vac On Check
		m_bRtn[0] = UnloadTRVacChk(VAC_ON, m_bCH1_Cell, m_bCH2_Cell);

		// DZone Vacuum Off Check
		if(m_bCH1_Cell)
			m_bRtn[1] = AMTVacChk(AMT_GetAutoJigID(ZONE_ID_D), JIG_CH_1);
		else
			m_bRtn[1] = FALSE;

		if(m_bCH2_Cell)
			m_bRtn[2] = AMTVacChk(AMT_GetAutoJigID(ZONE_ID_D), JIG_CH_2);
		else
			m_bRtn[2] = FALSE;

		//20170201 byskcho
		if(theConfigBank.m_Option.m_bUseDryRun)
		{
			m_bRtn[1] = FALSE;
			m_bRtn[2] = FALSE;
		}
		 //2017-07-24,JSJUNG, ��ε� TR UP ������ ���� 1�� �߰�
		if(m_bRtn[0] == TRUE && m_bRtn[1] == FALSE && m_bRtn[2] == FALSE )
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepZUp"), m_strStateName);

			//2017-09-19,SKCHO, ������ ��� ����̸� fast ���̰� / �ƴϸ� 0.5 �ӵ� �ٿ��� �̵�
			if(theConfigBank.m_Option.m_bUseULDTRMode)
			{
				MoveUnLoadTR_ZAxis_Up();
			}
			else
			{
				MoveUnLoadTR_ZAxis_Up(0.5);
			}
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)			SetError(ALARM_ULD_TR_VAC);
			else if(m_bRtn[1] == TRUE)		SetError(ALARM_DZONE_JIG_CH1_VAC);  // Dry run
			else if(m_bRtn[2] == TRUE)		SetError(ALARM_DZONE_JIG_CH2_VAC);  // Dry run
		}
		break;
	case stepEnd:		// Z Up �̵� üũ �� End.
		m_bRtn[0] = MoveUnLoadTR_ZAxis_UpChk();
		m_bRtn[1] = UnloadTRVacChk(VAC_ON, m_bCH1_Cell, m_bCH2_Cell);
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"), m_strStateName);
			// 2017.2.5 bgkim UD TR�� Z Up �� ���� �˶� �� �����̺��� ���ư��� ���� �����ϱ� ���� ��ġ ����
			CellData_UDTRFromAMT();
			//TrackOut
			CIM_CellTrackOut();
			//2018-03-14,MGYUN, CELL LOG �׸� MELSEC ���( ����� �ʿ�)
			theMonitorBank.WriteCellItemCH1(CELL_POS_UD_TR_CH1);
			theMonitorBank.WriteCellItemCH2(CELL_POS_UD_TR_CH2);

			//2018-02-08 JSJUNG, TRACKOUT INFO ���� ����(������ å�� ��û)
			//theCellBank.WriteTrackOutInfo(CELL_POS_UD_TR_CH1);
			//theCellBank.WriteTrackOutInfo(CELL_POS_UD_TR_CH2);

			//���ϼ���� ������ ���� �Ǿ� Ʈ�� �ƿ� �Ŀ� �� ���� ���� �Ѵ�. 2017/06/21 HSK.
			//CIM_CellClear();
			// Cell Log ���.
//			WriteCellLog(CELL_POS_UD_TR_CH1);
//			WriteCellLog(CELL_POS_UD_TR_CH2);
			// Main UI�� ��迡 ����� Product Data ����.

			theProcBank.m_nULDTRRetryStep = 0;
			nStep = stepIdle;

			//2017-12-05,SKCHO ULD TRGET���� UNIT INTERLOCK ������
			theProcBank.CheckUnitInterlock(ZONE_ID_D);
			
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			theProcBank.m_nULDTRRetryStep = 0;

			if(m_bRtn[0] == FALSE)		SetError(ALARM_ULD_TR_Z_UP_POS);
			else						SetError(ALARM_ULD_TR_VAC);
		}
		break;
	}

	m_nStep = nStep;

	return nStep;
}

int CStateUDTRPut::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	m_bCH1_Cell = CellTagExist(CELL_POS_UD_TR_CH1);
	m_bCH2_Cell = CellTagExist(CELL_POS_UD_TR_CH2);

	switch(nStep)
	{
	case stepStart:			// UD Stage�� Cell�� ������, Robot�� Get��ġ�� �ƴ��� üũ �� UD TR X Put ��ġ �̵�
		m_bRtn[0] = !CellTagExist(CELL_POS_UD_STAGE_CH1);
		m_bRtn[1] = !CellTagExist(CELL_POS_UD_STAGE_CH2);
		m_bRtn[2] = UnloadTRVacChk(VAC_ON, m_bCH1_Cell, m_bCH2_Cell);
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"), m_strStateName);

			MoveUnloadTR_X_PutPos();
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE || m_bRtn[1] == FALSE)	SetError(ALARM_UD_STG_CELL_EXIST);
			else if(m_bRtn[2] == FALSE)	SetError(ALARM_ULD_TR_VAC);
		}
		break;
	case stepZDown:			// UD TR X Put��ġ üũ �� Z Down 
		m_bRtn[0] = MoveUnloadTR_X_PutPosChk();
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepZDown"), m_strStateName);

			MoveUnLoadTR_Z_PutPos();
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)  //5.0
		{
			if(m_bRtn[0] == FALSE)		SetError(ALARM_ULD_TR_X_PUT_POS);
		}
		break;
	case stepVacOff:		// Z Down Check �� UD TR Vac Off , Stage Vac On.
		m_bRtn[0] = MoveUnLoadTR_Z_PutPosChk();
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepVacOff"), m_strStateName);

			// ULD TR Vac Off
			UnloadTRVacOnOff(JIG_CH_MAX, VAC_OFF);

			// ULD Buff STG Vac On
			ULDBuffSTGVacOnOff(VAC_ON, m_bCH1_Cell, m_bCH2_Cell);			

			CellData_UDStageFromUDTR();
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)		SetError(ALARM_ULD_TR_Z_PUT_POS);
		}
		break;
	case stepZup:			// Vac Check�� Z Up.
		m_bRtn[0] = UnloadTRVacChk(VAC_OFF, m_bCH1_Cell, m_bCH2_Cell); // ULD TR Vac Off Check
		m_bRtn[1] = ULDBuffSTGVacChk(VAC_ON, m_bCH1_Cell, m_bCH2_Cell); // ULD Buff STG Vac On Check
		m_bRtn[2] = (m_Timer.Stop(FALSE) > 0.1) ? TRUE : FALSE; // 2017.2.4 bgkim ���� ������ ���� ������ �߰�
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepZup"), m_strStateName);
			UnloadTR_BlowOff(JIG_CH_MAX);

			MoveUnLoadTR_ZAxis_Up();
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			CellData_UDStageFromUDTR();
			if(m_bRtn[0] == FALSE)			SetError(ALARM_ULD_TR_VAC);
			else if(m_bRtn[1] == FALSE)		SetError(ALARM_UD_STG_VAC);
		}
		break;
	case stepGetMove:		// Z up Check �� UD TR X Get ��ġ�� ����.
		m_bRtn[0] = MoveUnLoadTR_ZAxis_UpChk();
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepGetMove"), m_strStateName);

			MoveUnloadTR_X_GetPos();
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)		SetError(ALARM_ULD_TR_Z_UP_POS);
		}
		break;
	case stepEnd:			// UD TR X GET ��ġ üũ �� End.
		m_bRtn[0] = MoveUnloadTR_X_GetPosChk();
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"), m_strStateName);

			nStep = stepIdle;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)		SetError(ALARM_ULD_TR_X_GET_POS);
		}
		break;
	}

	m_nStep = nStep;

	return nStep;
}


int CStateUDRobotGet::Run()
{
	CCellInfo *pCellInfo = NULL;
	CString strTemp;
	int nType = 0;
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;
	long write;
	CLASS_CELL Class;
// 	//�ٽ� ���� �Ұ�� JOBCHANGETYPE 2�̸� PPID�� ���� ��� �̸�
// 	if( theConfigBank.m_UnloadRunInfo.GetJobChangeType() == JOBCHANGE_CASE2 && nStep < stepJob2Start)
// 	{
// 		nStep = stepJob2Start;			
// 	}

	switch(nStep)
	{
	case stepStart:			// UD Stage�� Cell �� �ְ� UD TR X�� Get ��ġ���� Ȯ�� �� UD Robot Get��ġ�� �̵�.
		m_bRtn[0] = MoveUnloadTR_X_GetPosChk();
		m_bRtn[1] = CellTagExist(CELL_POS_UD_STAGE_CH1);
		m_bRtn[2] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_RUNNING_Y_ERRRESET);
		if (m_bRtn[0] == TRUE && m_bRtn[2] == TRUE)
		{
			if(m_bRtn[1])
			{
				m_bCH1_Cell = TRUE;
				//2017.11.09, JSJUNG. ��ε� �������� ä��1 �� ��ȣ 
				theDeviceMelsec.RobotBitOn(UDROBOT_X_STGCH1GET);
			}
			else
			{
				m_bCH1_Cell = FALSE;
				//2017.11.09, JSJUNG. ��ε� �������� ä��2 �� ��ȣ 
				theDeviceMelsec.RobotBitOn(UDROBOT_X_STGCH2GET);
			}
			write = 4; //GETOK1
			theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			if (m_bRtn[0] == FALSE)
				SetError(ALARM_ULD_TR_X_GET_POS);
			else
				SetError(ALARM_UD_ROBOT_RUNNINGACK);
		}
		break;
	case stepBusyChk:
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_BUSY_Y_PGMSEL6);
		if (m_bRtn[0] == FALSE)
		{
			theDeviceMelsec.RobotBitOn(UDROBOT_XY_GETSTAGE);

			// �κ��� �ش� Cell�� ��ǰ���� �ҷ����� �˷��ش�.
			if(m_bCH1_Cell)
			{
				Class = CellData_GetLastClass(CELL_POS_UD_STAGE_CH1);
				if(Class == GOOD_CELL)
					theDeviceMelsec.RobotBitOn(UDROBOT_X_STGCH2VACON_Y_STG1OK);
				else
					theDeviceMelsec.RobotBitOn(UDROBOT_X_STGCH2VACOFF_Y_STG1NG);
			}
			else
			{
				Class = CellData_GetLastClass(CELL_POS_UD_STAGE_CH2);
				if(Class == GOOD_CELL)
					theDeviceMelsec.RobotBitOn(UDROBOT_X_STG2GETERR_Y_STG2OK);
				else
					theDeviceMelsec.RobotBitOn(UDROBOT_X_STGGETCOMP_Y_STG2NG);

			}

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepBusyChk"), m_strStateName);
			m_Timer.Start();
			nStep++;

		}
		else if (m_Timer.Stop(FALSE) > 3.) 
		{
			UDRobotBitAllOff(E_CELL_LOADING,FALSE);
			SetError(ALARM_UD_ROBOT_BUSYACK);
		}
		break;
	case stepGetChk:
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_GETSTAGE);
		if (m_bRtn[0] == FALSE )
		{
			//2017.10.31, JSJUNG. �۹� �ñ׳� ����
			write = 0; 
			theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
			
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepGetChk"), m_strStateName);
			theDeviceMelsec.RobotBitOn(UDROBOT_Y_SQCOMPRTN);
			theDeviceMelsec.RobotBitOff(UDROBOT_Y_SQSKIP);

			//2017.11.09, JSJUNG. ��ε� �������� ä��1/2 �� ��ȣ ����
			theDeviceMelsec.RobotBitOff(0xBC);
			theDeviceMelsec.RobotBitOff(0xBD);

			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 5. && m_Timer.Stop(FALSE) < 5.5)
		{
			theDeviceMelsec.RobotBitOn(UDROBOT_Y_SQSKIP);
		}
		else if (m_Timer.Stop(FALSE) > 10.)
		{
			UDRobotBitAllOff(E_CELL_LOADING,FALSE);
			SetError(ALARM_UD_ROBOT_GETSTAGEINTERLOCKOFFACK);
		}
		break;
	case stepRobotVacOn:
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_VACON);
		if (IsReturnOk())
		{
			theDeviceMelsec.RobotBitOff(UDROBOT_Y_SQCOMPRTN);
			RobotCellVacOn(VAC_ON,E_UD_ROBOT);
			theDeviceMelsec.RobotBitOff(UDROBOT_Y_SQSKIP);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepRobotVacOn"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 3. && m_Timer.Stop(FALSE) < 3.5)
		{
			theDeviceMelsec.RobotBitOn(UDROBOT_Y_SQSKIP);
		}
		else if (m_Timer.Stop(FALSE) > 7.)
		{
			UDRobotBitAllOff(E_CELL_LOADING,FALSE);
			if (m_bRtn[0] == TRUE)					SetError(ALARM_UD_ROBOT_VACONACK);
			else									SetError(ALARM_UD_STG_VAC);
		}
		break;
	case stepRobotVacOnChk:
		m_bRtn[0] = RobotCellVacOnChk(E_UD_ROBOT,VAC_ON);

		if (IsReturnOk())
		{
			theDeviceMelsec.RobotBitOn(UDROBOT_XY_VACON);
			theDeviceMelsec.RobotBitOff(UDROBOT_XY_VACOFF);

			if(m_bCH1_Cell)
				CellData_UDRobotFromUDStage(CELL_POS_UD_STAGE_CH1);
			else
				CellData_UDRobotFromUDStage(CELL_POS_UD_STAGE_CH2);
	
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepRobotVacOnChk"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			UDRobotBitAllOff(E_CELL_LOADING,FALSE);
			SetError(ALARM_UD_ROBOT_VACON);
		}
		break;
	case stepVacOff:
		if(m_bCH1_Cell)
			m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_STGCH1VACOFF);
		else
			m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_STGCH2VACOFF_Y_STG1NG);

		if (IsReturnOk())
		{
			if(m_bCH1_Cell)
				ULDBuffSTGVacOnOff(VAC_OFF,TRUE,FALSE);
			else
				ULDBuffSTGVacOnOff(VAC_OFF,FALSE,TRUE);

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepVacOff"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			UDRobotBitAllOff(E_CELL_LOADING,FALSE);
			SetError(ALARM_UD_ROBOT_STAGECH1VACOFFACK);
		}
		break;
	case stepGetComp:
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_STGGETCOMP_Y_STG2NG);
		if(m_bCH1_Cell)
			m_bRtn[1] = ULDBuffSTGVacChk(VAC_OFF,TRUE,FALSE);
		else
			m_bRtn[1] = ULDBuffSTGVacChk(VAC_OFF,FALSE,TRUE);

		if (IsReturnOk())
		{
			//2018/01/26, JSJUNG, UD ROBOT ƼĪ��/���� ��ġ��(���念��/READ ��)
			long Data[8];

			theDeviceMelsec.RobotWordRecvBuf(UDROBOT_TEACH_MOVE_XYZT_VAL,sizeof(long)*8,Data);

			if(m_bCH1_Cell)
			{
				theLog[LOG_UDRB_TEACH_MOVE].AddBuf( _T("UD CH1 GET TEACH X[%f], Y[%f], Z[%f], T[%f]"),(float)Data[0]/100, (float)Data[1]/100, (float)Data[2]/100, (float)Data[3]/100);
				theLog[LOG_UDRB_TEACH_MOVE].AddBuf( _T("UD CH1 GET MOVE X[%f], Y[%f], Z[%f], T[%f]"), (float)Data[4]/100, (float)Data[5]/100, (float)Data[6]/100, (float)Data[7]/100);
				//2018-03-08,MGYUN, ULD ROBOT CH1 TEACHING MELSEC ���
				theMonitorBank.WriteULDRobotCH1POS( (double)Data[0]/100, (double)Data[1]/100, (double)Data[2]/100, (double)Data[3]/100 );
			}
			else
			{
				theLog[LOG_UDRB_TEACH_MOVE].AddBuf( _T("UD CH2 GET TEACH X[%f], Y[%f], Z[%f], T[%f]"),(float)Data[0]/100, (float)Data[1]/100, (float)Data[2]/100, (float)Data[3]/100);
				theLog[LOG_UDRB_TEACH_MOVE].AddBuf( _T("UD CH2 GET MOVE X[%f], Y[%f], Z[%f], T[%f]"), (float)Data[4]/100, (float)Data[5]/100, (float)Data[6]/100, (float)Data[7]/100);
				//2018-03-08,MGYUN, ULD ROBOT CH2 TEACHING MELSEC ���
				theMonitorBank.WriteULDRobotCH2POS( (double)Data[0]/100, (double)Data[1]/100, (double)Data[2]/100, (double)Data[3]/100 );
			}
			ULDBuffSTG_BlowOff(TRUE,TRUE);
			theDeviceMelsec.RobotBitOn(UDROBOT_Y_SQCOMPRTN);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepGetComp"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 3.)
		{
			UDRobotBitAllOff(E_CELL_LOADING,FALSE);
			SetError(ALARM_UD_ROBOT_GETCOMPACK);
		}
		break;
	case stepEnd:			// Wait ��ġ üũ �� End.
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_GETSTAGE);	
		m_bRtn[1] = RobotCellVacOnChk(E_UD_ROBOT,VAC_ON);

		if (IsReturnOk())
		{
			theDeviceMelsec.RobotBitOff(UDROBOT_Y_SQCOMPRTN);
			UDRobotBitAllOff(E_CELL_LOADING,FALSE);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"), m_strStateName);		

			//2017-03-09, skcho, jobchange ������ �Ǵ�
			strTemp = theConfigBank.m_UnloadRunInfo.GetUnloadingRuleInfoCurrentPPID();
			if(strTemp == _T("") || strTemp != theRecipeBank.m_strRecipeName )  //unloading rule info�� PPID�� ���� ��� �Ǵ� �ٸ����,//theRecipeBank.m_strRecipeName�� �������� ����, ���� ������� �����Ƿ� �Ǿ�� ��.
			{
				 nType = (strTemp == _T("")) ? JOBCHANGE_CASE2 : JOBCHANGE_CASE1;  //job change ������ �����Ѵ�. ppid�� ������ CASE1, �ٸ��� CASE2
				 theConfigBank.m_UnloadRunInfo.SetJobChangeType(nType);

				 theProcBank.ChangeMachineState(CONST_MACHINE_STATE::MACHINE_PAUSE);
				 GetMainHandler()->GetGUIFrame()->SendMessageDialog(eDLG_JobChange, UDMSG_JOBCHANGE_POPUP, 0, 1);		// ���� ���¹ٿ� ���� ������ �����Ѵ�.
			     if(nType == JOBCHANGE_CASE1)
				 {
					nStep = stepIdle;
				 }
			}		
			else
			{
				nStep = stepIdle;
			}
			
		}
		else if(m_Timer.Stop(FALSE) > 3.) //CHANGE FROM 3 TO 10
		{
			if(m_bRtn[0] == FALSE)	SetError(ALARM_UD_ROBOT_GETSTAGEINTERLOCKONACK);
			else					SetError(ALARM_UD_ROBOT_VACON);
		}
		break;
// 	case stepJob2Start:
// 7
// 		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] JobChangeCase2 Start"), m_strStateName);	
// 		nStep++;
// 		break;
// 
// 	case stepJob2Info:   //s6f205
// 
// 		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] JobChangeCase2 Cell Lot Info Request"), m_strStateName);	
// 		theUnitFunc.CIM_CellLot_Info_Request();
// 		m_Timer.Start();
// 		nStep++;
// 		break;
// 	case stepJob2InfoChk:  //s3f109
// 		m_bRtn[0] = (theProcBank.m_strProductID == _T("")) ? FALSE : TRUE;
// 		if (IsReturnOk())//product ID ����?
// 		{		
// 
// 			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] JobChangeCase2 Cell Lot Info Check"), m_strStateName);	
// 
// 			theConfigBank.m_UnloadRunInfo.m_sCurrentProduct = theProcBank.m_strProductID ;//PRODUCT ID ����
// 			theConfigBank.m_UnloadRunInfo.m_sCurrentPPID = theProcBank.m_strPPID ; //PPID ����
// 			theConfigBank.m_UnloadRunInfo.SaveUnloadRuleFileInfo();	
// 
// 			theProcBank.m_strProductID  = _T("");  //2017-03-11,skcho ���� jobchange�� ���ؼ� �ʱ�ȭ
// 			nStep++;
// 		}
// 		else if(m_Timer.Stop(FALSE) > 10.)
// 		{		
// 			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] JobChangeCase2 GO to NG"), m_strStateName);	
// 			//Ÿ�� �ƿ��� ��� NG ��Ʈ�� ��
// 			pCellInfo = theCellBank.GetCellInfo(CELL_POS_UD_ROBOT);
// 			if(pCellInfo)
// 			{
// 				pCellInfo->m_LastClass = NONE_CELL;
// 			}
// 			nStep = stepIdle;
// 		}		
// 
// 		break;
// 	case stepJob2Attri:
// 		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] JobChangeCase2 Attribute Request"), m_strStateName);	
// 		theUnitFunc.CIM_Attribute_Request();
// 		m_Timer.Start();
// 		nStep++;
// 		break;
// 	case stepJob2AttriChk:
// 		if(m_Timer.Stop(FALSE) > 10.)
// 		{
// 			m_bRtn[0] = (theProcBank.m_strReplyCode == _T("0")) ? TRUE : FALSE;
// 			if (IsReturnOk())//product ID ����?
// 			{		
// 				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] JobChangeCase2 Attribute Check"), m_strStateName);	
// 				theConfigBank.m_UnloadRunInfo.m_sTrayQtyInLot = theProcBank.m_strTrayQty ;//PRODUCT ID ����
// 				theConfigBank.m_UnloadRunInfo.m_sCellQtyInTray = theProcBank.m_strCellQty ; //PPID ����
// 				theConfigBank.m_UnloadRunInfo.m_sNextStepID = NEXTSTEPID; 
// 				theConfigBank.m_UnloadRunInfo.SaveUnloadRuleFileInfo();
// 				theProcBank.m_strReplyCode = _T("-1");//2017-03-11,skcho ���� jobchange�� ���ؼ� �ʱ�ȭ
// 				nStep++;
// 			}		
// 			else if(m_Timer.Stop(FALSE) > 10.)
// 			{		
// 				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] JobChangeCase2 Attribute TimeOut"), m_strStateName);				
// 				nStep++;
// 			}
// 		}
// 
// 		break;
// 	case stepJob2End:
// 		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] JobChangeCase2 End"), m_strStateName);
// 		theConfigBank.m_UnloadRunInfo.SetJobChangeType(JOBCHANGE_NONE);
// 		nStep = stepIdle;
// 		break;
	default:
		break;

	}

	m_nStep = nStep;

	return nStep;
}

//////////////////////20180816wjp
int CStateGoodTrayGetPut::ResetEx()
{
	m_nStep=stepInitCheck;
	return 0;
}

int CStateGoodTrayGetPut::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();
	int ic=0;
	int nStep = m_nStep;
	//theDeviceMelsec.RobotBitOn(UDROBOT_XY_VACOFF);
	//theDeviceMelsec.
	//RobotCellVacOn(VAC_OFF,E_UD_ROBOT);
	//if(bputget==0);
	switch(nStep)
	{
	case stepStart://initial
		if(theProcBank.m_bModel2FullTray)
			theProcBank.m_bModel2FullTray=FALSE;
		if(! ULDTrayLiftTrayChk(SENSOR_ON)&&!theUnitFunc.TrayTr_VacChk(VAC_ON))
		{
			nStep = m_nStep=-1;
			return -1;
		}
		//TrayTr_VacOnOff(VAC_OFF);
		TrayTrZ_UpPos();
		if(!TrayTrZ_UpChk()) 
			TrayTrZ_UpPos();
		if(!TrayTrY_OutChk()) 
			TrayTrY_OutPos();
		if(!TrayTrY_OutChk()) 
			TrayTrY_OutPos();
		///in lift up
		if(LDTrayLiftTrayChk(SENSOR_ON))
		{
			if(!InTrayAlignTrayChk(SENSOR_ON))//not tray in pos in lift
			{
				DivisionCylFwdBwd(GOOD_IN_TRAY_PORT, CYL_BWD);
				AlignCylFwdBwd(GOOD_IN_TRAY_PORT, CYL_BWD);
				/*while(TRUE)
				{
				if( AlignCylFwdBwdChk(GOOD_IN_TRAY_PORT, CYL_BWD)&&
				DivisionCylFwdBwdChk(GOOD_IN_TRAY_PORT, CYL_BWD))
				break;
				if(-1==nStep)
				return -1;
				Sleep(5);
				}*/
				InTrayLiftZ_PitchUpRetry();
			}
			m_Timer.Start();
			while(!InTrayAlignTrayChk(SENSOR_ON))
			{
				if(	InTrayLiftZ_StopChk())
				{InTrayLiftZ_PitchUpRetry();}
				if(-1==nStep)
				{
					InTrayLiftZ_PitchUpStop();
					return -1;
				}
				if(m_Timer.Stop(FALSE)>15)
				{
					m_Timer.Stop(TRUE);
					InTrayLiftZ_PitchUpStop();
					SetError(ALARM_OKTRAY_IN_LIFT_STOPPER_UP_CHK);
					return -1;
				}
				Sleep(1);	
			}
			InTrayLiftZ_PitchUpStop();
		}
		else
		{
			InTrayLiftZ_TopPos();
			m_Timer.Start();
			while(!InTrayLiftZ_TopPosChk())//==1  tray ok
			{	
				Sleep(1);
				if(-1==nStep)
				{
					OutTrayLiftZ_PitchUpStop();
					return -1;
				}
				if(m_Timer.Stop(FALSE)>15)
				{
					m_Timer.Stop(TRUE);
					OutTrayLiftZ_PitchUpStop();
					SetError(ALARM_OKTRAY_OUT_LIFT_STOPPER_UP_CHK);
					return -1;
				}
				Sleep(1);
			}

		}	

		//out lift up
		if(!theUnitFunc.GetInPutIOCheck(X_GOOD_LD_TRAY_LIFT_LIMIT_SEN_OUT2))//==1  tray ok
		{
			OutTrayLiftZ_PitchDown(5);
		}
		m_Timer.Start();
		while(!theUnitFunc.GetInPutIOCheck(X_GOOD_LD_TRAY_LIFT_LIMIT_SEN_OUT2))//==1  tray ok
		{	
			if(-1==nStep)
			{
				OutTrayLiftZ_PitchUpStop();
				nStep=-1;
				return -1;
			}
			if(m_Timer.Stop(FALSE)>30)
			{
				m_Timer.Stop(TRUE);
				OutTrayLiftZ_PitchUpStop();
				nStep=-1;
				SetError(ALARM_OKTRAY_OUT_LIFT_STOPPER_UP_CHK);
				return -1;
			}
			Sleep(1);
		}
		Sleep(100);

		if(theUnitFunc.GetInPutIOCheck(X_GOOD_LD_TRAY_LIFT_LIMIT_SEN_OUT2)&&!TrayTr_VacChk(VAC_ON))//==1  no tray 
		{
			OutTrayLiftZ_PitchUpRetry();
		}
		while(theUnitFunc.GetInPutIOCheck(X_GOOD_LD_TRAY_LIFT_LIMIT_SEN_OUT2)&&!TrayTr_VacChk(VAC_ON))//==1  tray ok
		{	
			if(-1==nStep)
			{
				OutTrayLiftZ_PitchUpStop();
				return -1;
			}
			if(m_Timer.Stop(FALSE)>30)
			{
				m_Timer.Stop(TRUE);
				OutTrayLiftZ_PitchUpStop();
				nStep=-1;
				SetError(ALARM_OKTRAY_OUT_LIFT_STOPPER_UP_CHK);
				return -1;
			}
			Sleep(1);
		}
		OutTrayLiftZ_PitchUpStop();
		m_bRtn[3]=InTrayAlignTrayChk(SENSOR_ON);
		ResetReturnValue();
		if(LDTrayLiftTrayChk(SENSOR_ON))
		{	
			AlignCylFwdBwd(GOOD_IN_TRAY_PORT, CYL_FWD);
			DivisionCylFwdBwd(GOOD_IN_TRAY_PORT, CYL_FWD);
		}
		//m_bRtn[0] = LDTrayLiftTrayChk(SENSOR_ON);
		m_bRtn[1] = ULDTrayLiftTrayChk(SENSOR_ON);
		m_bRtn[2] = TrayTrZ_UpChk();
		m_bRtn[3] = TrayTrY_OutChk();
		//m_bRtn[4] = InTrayAlignTrayChk(SENSOR_ON);
		//if( TrayTr_VacChk(VAC_ON))     nStep=8;
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] init Condition  stepStart"), m_strStateName);
			m_Timer.Start();
			nStep++;
			//if(theProcBank.m_bPick2PitchliftOK==TRUE) nStep=15;
			//if(theProcBank.m_bPick2PitchliftOK==TRUE&&theProcBank.m_bPick2ScanOK==FALSE) nStep=16;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(!m_bRtn[2])SetError(ALARM_OKTRAY_TR_Z_UP_POS);
			if(!m_bRtn[3])SetError(ALARM_OKTRAY_TR_Y_PUT_POS);
			TRACE(_T("put get sequence  check NG"));
		}
		break;
	case stepInLiftCylBFWD://start tray check then cylinder release
		//m_bRtn[0] = LDTrayLiftTrayChk(SENSOR_ON);
		m_bRtn[1] = ULDTrayLiftTrayChk(SENSOR_ON);
		if(!LDTrayLiftTrayChk(SENSOR_ON))
		{
			m_Timer.Start();
			nStep++;
		}
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] start tray check then cylinder release"), m_strStateName);
			DivisionCylFwdBwd(GOOD_IN_TRAY_PORT, CYL_BWD);
			AlignCylFwdBwd(GOOD_IN_TRAY_PORT, CYL_BWD);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			SetError(ALARM_OKTRAY_IN_LIFT_TRAY_CHK);
			TRACE(_T("put cylinder backforwad  check NG"));
		}
		break;

	case stepInLiftPitchDn:// in Tray picth down
		m_bRtn[0] = AlignCylFwdBwdChk(GOOD_IN_TRAY_PORT, CYL_BWD);
		m_bRtn[1] = DivisionCylFwdBwdChk(GOOD_IN_TRAY_PORT, CYL_BWD);
		m_bRtn[2]=  InTrayLiftZ_StopChk();
		if(!LDTrayLiftTrayChk(SENSOR_ON))
		{
	        ResetReturnValue();
		}
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] in Tray picth down"), m_strStateName);
		/*	if(InTrayAlignTrayChk(SENSOR_ON))
			{*/
				InTrayLiftZ_PitchDown(3);
			/*}*/
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			TRACE(_T("put cylinder backforwad  check NG"));	
			if(m_bRtn[0] == FALSE)			SetError(ALARM_OKTRAY_IN_ALIGN_BWD_CHK);
			else if(m_bRtn[1] == FALSE)		SetError(ALARM_OKTRAY_IN_DIVISION_BWD_CHK);
		}
		if(TrayTr_VacChk(VAC_ON))
		{
			nStep=stepTrayTRZUp;
			m_Timer.Start();
		}
		break;
	case stepTratTRZDn:// pitch out check then z down
		m_bRtn[0] = OutTrayLiftZ_StopChk();
		m_bRtn[1] = TrayTr_VacChk(VAC_OFF);
		m_bRtn[2] = TrayTrY_OutChk();
		m_bRtn[3]=InTrayLiftZ_StopChk();	
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s]   out lift pitchn check then z down"), m_strStateName);
			TrayTrZ_OutPos();
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == 0)	SetError(ALARM_OKTRAY_OUT_LIFT_STOPPER_UP_CHK);
			if(m_bRtn[3] == 0)	SetError(ALARM_OKTRAY_IN_LIFT_STOPPER_UP_CHK);

		}
		break;		
	case stepVacOn ://  out lift  z stop ok  then vac on
		m_bRtn[0] = TrayTrY_OutChk();
		m_bRtn[1]= TrayTrZ_OutChk();
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] out lift  z stop ok  then vac on"), m_strStateName);
			TrayTr_VacOnOff(VAC_ON);
			m_Timer.Start();
			RetryNum=0;
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)	SetError(ALARM_OKTRAY_TR_Y_PUT_POS);
			if(m_bRtn[1] == FALSE)	SetError(ALARM_OKTRAY_TR_Z_PUT_POS);
		}
		break;		

	case stepTrayTRZUp://check vac on then up
		m_bRtn[0] = TrayTr_VacChk(VAC_ON);
		if(!DivisionCylFwdBwdChk(GOOD_IN_TRAY_PORT, CYL_BWD)) 	
		{
			DivisionCylFwdBwd(GOOD_IN_TRAY_PORT, CYL_BWD);
		}
		if(!AlignCylFwdBwdChk(GOOD_IN_TRAY_PORT, CYL_BWD)) 
		{
			AlignCylFwdBwd(GOOD_IN_TRAY_PORT, CYL_BWD);
		}
		while(!TrayTr_VacChk(VAC_ON))
		{
			/*if(AlignCylFwdBwdChk(GOOD_IN_TRAY_PORT, CYL_BWD)&&
				DivisionCylFwdBwdChk(GOOD_IN_TRAY_PORT, CYL_BWD))
				break;*/
			if(-1==nStep)
				return -1;

			if(m_Timer.Stop(FALSE) >3)
			{
				if(!AlignCylFwdBwdChk(GOOD_IN_TRAY_PORT, CYL_BWD))
				{
					SetError(ALARM_OKTRAY_IN_ALIGN_BWD_CHK);
					break;
				}
				if(!DivisionCylFwdBwdChk(GOOD_IN_TRAY_PORT, CYL_BWD))
				{
					SetError(ALARM_OKTRAY_IN_DIVISION_BWD_CHK);
					break;
				}
				SetError(ALARM_OKTRAY_TR_VAC);
				nStep=-1;
			}
			Sleep(1);
		}

		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] check vac on then up"), m_strStateName);
			TrayTrZ_UpPos();

		/*	if(TrayData_CellIsEmpty(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_TR))
				theProcBank.m_bTrayCellFullChk = TRUE;
			else
				theProcBank.m_bTrayCellFullChk = FALSE;

			if(theProcBank.m_TrayBank.GetTrayInfo(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_OUT_LIFT)->IsExist())
			{
				TrayData_RemoveData(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_TR);
			}
			else
			{
				TrayData_from_to(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_TR,TRAY_INFO_POS_GOOD_OUT_LIFT);
			}		*/

			//theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepOutZDown"), m_strStateName);
		//	theProcBank.m_TrayBank.GetTrayInfo(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_OUT_LIFT)->m_nTrayCount--;

		//	theProcBank.m_nGoodOutLiftCount--;

	/*		if(theProcBank.m_bTrayCellFullChk)
			{
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] Tray Count : %d , Out Lift Full"), m_strStateName,theProcBank.GetGoodOutLiftCount());
				theProcBank.m_TrayBank.GetTrayInfo(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_OUT_LIFT)->SetTrayFull(TRUE);
				theProcBank.m_bTrayCellFullChk = FALSE;
			}*/

			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 3.)
		{
			if(m_bRtn[0] == FALSE)
			{
				RetryNum=0;
				this->OutTrayLiftZ_PitchUpStop();
				SetError(ALARM_OKTRAY_TR_VAC);
			}
		}
		break;
	case stepTrayTRYIn://up ok move to Y  In pos
		m_bRtn[0] = TrayTr_VacChk(VAC_ON);
		m_bRtn[1]=TrayTrZ_UpChk();
		m_bRtn[2]=TrayTrY_OutChk();
		
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] up ok move to Y  In pos"), m_strStateName);
			TrayTrY_InPos();
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)		SetError(ALARM_OKTRAY_TR_Z_UP_POS);
		}
		break;
	case stepTrayTRZIn://down in tray z in pos
	    m_bRtn[0] = InTrayLiftZ_StopChk();
		m_bRtn[1] = TrayTr_VacChk(VAC_ON);
		m_bRtn[2]=TrayTrZ_UpChk();
		m_bRtn[3]=TrayTrY_InChk();

		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepZDown"), m_strStateName);
			TrayTrZ_InPos();
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)	SetError(ALARM_OKTRAY_TR_Y_GET_POS);
		}
		break;	
	case stepVacOff:	//in z pos then vac off
		m_bRtn[0] = TrayTrZ_InChk();
		m_bRtn[1] = TrayTr_VacChk(VAC_ON);
		m_bRtn[2] = TrayTrY_InChk();

		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s]lift  in z pos then vac off"), m_strStateName);
			TrayTr_VacOnOff(VAC_OFF);
			//2018wjp
			theProcBank.m_bModel2FullTray=TRUE;
			theProcBank.m_TrayBank.GetTrayInfo(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_OUT_LIFT)->m_nTrayCount--;
			theProcBank.m_TrayBank.GetTrayInfo(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_IN_LIFT)->m_nTrayCount++;
			::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_REMOVE_ALL_CELL, 0,0); 
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)	SetError(ALARM_OKTRAY_TR_Z_GET_POS);
		}
		break;	
	case stepTrayTRZUpOut:	//vac off then z up os
		m_bRtn[0] = TrayTrZ_InChk();
		m_bRtn[1] = TrayTr_VacChk(VAC_OFF);
		m_bRtn[2] = TrayTrY_InChk();
		if(IsReturnOk())
		{		
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] vac off then z up os"), m_strStateName);
			TrayTrZ_UpPos();
			//theProcBank.m_bCheckDcrCheck=True;
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)	SetError(ALARM_OKTRAY_TR_VAC);
		}
		break;	
	case stepTrayTRYOut:	//vac off then z up os
		m_bRtn[0] = TrayTrZ_UpChk();
		m_bRtn[1] = TrayTr_VacChk(VAC_OFF);
		m_bRtn[2] = TrayTrY_InChk();

		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] vac off then z up os"), m_strStateName);
			TrayTrY_OutPos();
			OutTrayLiftZ_PitchDown(3);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)	SetError(ALARM_OKTRAY_TR_Z_UP_POS);
		}
		break;	
	case stepInLiftUp:// check the out pos  then lift up 
		m_bRtn[0] = TrayTrZ_UpChk();
		m_bRtn[1] = TrayTr_VacChk(VAC_OFF);
		m_bRtn[2] = TrayTrY_OutChk();
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] check the out pos  then lift up"), m_strStateName);
			DivisionCylFwdBwd(GOOD_IN_TRAY_PORT, CYL_BWD);
			AlignCylFwdBwd(GOOD_IN_TRAY_PORT, CYL_BWD);

			if(InTrayAlignTrayChk(SENSOR_ON))
			{ InTrayLiftZ_PitchDown(2);
			}

			InTrayLiftZ_PitchUpRetry(); //up to ceck limit
			while(1)
			{
				if(InTrayAlignTrayChk(SENSOR_ON))
					break;
				if(-1==nStep)
				{
					InTrayLiftZ_PitchUpStop();
					return -1;
				}
				Sleep(1);
				if(m_Timer.Stop(FALSE) > 15.)
				{

	             InTrayLiftZ_PitchUpStop();
				 nStep=-1;
			     SetError(ALARM_OKTRAY_IN_LIFT_STOPPER_UP_CHK);
				 return -1;
				}
				Sleep(1);
			}
			InTrayLiftZ_PitchUpStop();
			//theProcBank.m_bPick2RequestPitchlift=TRUE;
			m_Timer.Start();
			nStep++;
			//nStep=stepIdle;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)	SetError(ALARM_OKTRAY_TR_Y_PUT_POS);
		}
		break;
     case stepInLiftDiCylFWD:// check up stop cylinder Fwd
		m_bRtn[0]=InTrayAlignTrayChk(SENSOR_ON);
		m_bRtn[1]=InTrayLiftZ_StopChk();
		if(IsReturnOk())
		{
			DivisionCylFwdBwd(GOOD_IN_TRAY_PORT, CYL_FWD);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] check up stop cylinder Fwd"), m_strStateName);
			m_Timer.Start();
			nStep++;
			//theProcBank.m_bPick2RequestPitchlift=TRUE;
			//nStep=stepIdle;
		}
		else if(m_Timer.Stop(FALSE) > 15.)
		{
			if(m_bRtn[0] == FALSE)	SetError(ALARM_OKTRAY_IN_LIFT_STOPPER_UP_CHK);
		}
		break;
	 case stepInLiftAlCylFWD://check up stop cylinder2 Fwd
		 m_bRtn[0]=InTrayLiftZ_StopChk();
		 m_bRtn[1] = DivisionCylFwdBwdChk(GOOD_IN_TRAY_PORT, CYL_FWD);
		 if(IsReturnOk())
		 {
			 AlignCylFwdBwd(GOOD_IN_TRAY_PORT, CYL_FWD);
			 theLog[LOG_SEQUENCE].AddBuf( _T("[%s] check up stop cylinder2 Fwd"), m_strStateName);
			 m_Timer.Start();
			 nStep++;
			 //theProcBank.m_bPick2RequestPitchlift=TRUE;
			 //nStep=stepIdle;
		 }
		 else if(m_Timer.Stop(FALSE) > 5.)
		 {
			 if(m_bRtn[0] == FALSE)	SetError(ALARM_OKTRAY_IN_DIVISION_FWD_CHK);
		 }
		 break;
	 case stepDCR :
		 m_bRtn[0] = AlignCylFwdBwdChk(GOOD_IN_TRAY_PORT, CYL_FWD);
		 m_bRtn[1] = DivisionCylFwdBwdChk(GOOD_IN_TRAY_PORT, CYL_FWD);
		 if (IsReturnOk())
		 {
			 DCRTrigger(ON);	
			 Sleep(200);
			 theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepDCR "), m_strStateName);
			 m_Timer.Start();
			 nStep++;
		 }
		 else if(m_Timer.Stop(FALSE) > 3.)
		 {
			 if(m_bRtn[0] == FALSE)			SetError(ALARM_OKTRAY_IN_ALIGN_FWD_CHK);
			 else if(m_bRtn[1] == FALSE)		SetError(ALARM_OKTRAY_IN_DIVISION_FWD_CHK);
		 }
		 break;
	 case stepEnd:
		 m_bRtn[0] = DCRChk();
		 if(!m_bRtn[0])	
		 {  
			 nStep=stepDCR ;
			 DCRTrigger(OFF);
			 RetryDCR++;
			 if(RetryDCR>=3)
			 {
				 ::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_DCR_FAIL, 0,1); 
				 SetError(ALARM_OKTRAY_DCR_FAIL);
				 break;
			 }
			 break;
		 }
		 if (IsReturnOk())
		 {
			 DCRTrigger(OFF);
			 //2018wjp
			 CellData_Tray_ClearAll();
			 theProcBank.m_strTrayID.MakeUpper();
			 theUnitFunc.CIM_CELLID_Info_Request();		 
			 m_Timer.Start();
			 while(!theProcBank.m_bGetCellIDComplate)
			 {
				 if(m_Timer.Stop(FALSE) > 5)
				 {
					 nStep=-1;
					 ::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_CIM_BACK_FAIL, 0,2); 
					 SetError(ALARM_OKTRAY_DCR_FAIL);//ALARM_CIM_GET_CELL_ID_FAIL
				 }
				 if(-1==nStep)
					 return -1;
				 Sleep(1);
			 }
			 theProcBank.m_bGetCellIDComplate=FALSE;
			 CellData_Tray_CreateAll();
			 TrayData_Create(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_LIFT);
			 TrayData_from_toAlign(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_LIFT,TRAY_INFO_POS_GOOD_IN_ALIGN);
			 for(int i=0;i<theRecipeBank.m_Tray.m_nCountX;i++)
			 {
				 for(int j=0;j<theRecipeBank.m_Tray.m_nCountY;j++)
				 {
					 theProcBank.m_TrayBank.GetTrayInfo(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_ALIGN)->AddCell(CELL_POS_GOOD_TRAY,theProcBank.strCIMLoadCellID[i][j],i+1,j+1);
				 }
			 }
			 theProcBank.MinusSubGoodAlignCount();
			 theProcBank.MinusGoodAlignCount();
			 if(theProcBank.m_bModel2FullTray)
				 theProcBank.m_bModel2FullTray=FALSE;
			 //2018wjp	 
			 theProcBank.GOODTrayBitX=theRecipeBank.m_Tray.m_nCountX+1;
			 theProcBank.GOODTrayBitY=theRecipeBank.m_Tray.m_nCountY+1;
			 theLog[LOG_SEQUENCE_TRAY].AddBuf(_T("Good Align Count : %d"),theProcBank.m_nGoodAlignCount);
			 ::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_DCR_UPDATE, 0,0);  //2017-02-28, skcho, DCR ���� ���� ȭ�鿡 ǥ��
			 theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepIDReadCheck"), m_strStateName);
			 m_Timer.Start();
			 nStep=stepIdle;
		 }
		 else if(m_Timer.Stop(FALSE) > 5.)
		 {
			 ::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_DCR_UPDATE, 0,0);  //2017-02-28, skcho, DCR ���� ���� ȭ�鿡 ǥ��
			 SetError(ALARM_OKTRAY_DCR_FAIL);
			 DCRTrigger(OFF);
		 }
		 break;
	 case stepInitCheck:
		 if(!TrayTrZ_UpChk())
		 {
			 TrayTrZ_UpPos();
			 if(!TrayTrZ_UpChk()) 
				 TrayTrZ_UpPos();
		 }
		 if(!TrayTrY_OutChk())
		 {
			 TrayTrY_OutPos();
			 if(!TrayTrY_OutChk()) 
				 TrayTrY_OutPos();
		 }
		 if(TrayTr_VacChk(VAC_ON))
		 {
			 //
			 if(!ULDTrayLiftTrayChk(SENSOR_ON))
			 {
				 InTrayLiftZ_UpPos();
				 TrayTrZ_OutPos();
				 if(!TrayTrZ_OutChk()) 
					 TrayTrZ_OutPos();
				 TrayTr_VacOnOff(VAC_OFF);
				 TrayTrZ_UpPos();
				 if(!TrayTrZ_UpChk()) 
					 TrayTrZ_UpPos();
				 m_nStep=-1;
				 break;
			 }
			 //
			 if(ULDTrayLiftTrayChk(SENSOR_ON))
			 {
				 if(!theUnitFunc.GetInPutIOCheck(X_GOOD_LD_TRAY_LIFT_LIMIT_SEN_OUT1))//==1  tray ok
				 {
					 OutTrayLiftZ_PitchDown(10);
					 m_Timer.Start();
					 while(!theUnitFunc.GetInPutIOCheck(X_GOOD_LD_TRAY_LIFT_LIMIT_SEN_OUT1))
					 {	
						 Sleep(1);
						 if(-1==nStep)
						 {
							 OutTrayLiftZ_PitchUpStop();
							 m_nStep=-1;
							 return -1;
						 }
						 if(m_Timer.Stop(FALSE)>3000)
						 {
							 m_Timer.Stop(TRUE);
							 OutTrayLiftZ_PitchUpStop();
							 m_nStep=-1;
							 SetError(ALARM_OKTRAY_IN_LIFT_TRAY_CHK);
							 return -1;
						 }
					 }
					 OutTrayLiftZ_PitchUpStop();
					 InTrayLiftZ_UpPos();
					 TrayTrZ_OutPos();
					 if(!TrayTrZ_OutChk()) 
						 TrayTrZ_OutPos();
					 TrayTr_VacOnOff(VAC_OFF);
					 TrayTrZ_UpPos();
					 if(!TrayTrZ_UpChk()) 
						 TrayTrZ_UpPos();
					 m_nStep=-1;
					 break;

				 }
				 else
				 {
					 OutTrayLiftZ_PitchUpRetry();
					 m_Timer.Start();
					 while(theUnitFunc.GetInPutIOCheck(X_GOOD_LD_TRAY_LIFT_LIMIT_SEN_OUT1))
					 {	
						 Sleep(1);
						 if(-1==nStep)
						 {
							 OutTrayLiftZ_PitchUpStop();
							 m_nStep=-1;
							 return -1;
						 }
						 if(m_Timer.Stop(FALSE)>5000)
						 {
							 m_Timer.Stop(TRUE);
							 OutTrayLiftZ_PitchUpStop();
							 m_nStep=-1;
							 SetError(ALARM_OKTRAY_IN_LIFT_TRAY_CHK);
							 return -1;
						 }
					 }
					 OutTrayLiftZ_PitchUpStop();
					 OutTrayLiftZ_PitchUpStop();
					 InTrayLiftZ_UpPos();
					 TrayTrZ_OutPos();
					 if(!TrayTrZ_OutChk()) 
						 TrayTrZ_OutPos();
					 TrayTr_VacOnOff(VAC_OFF);
					 TrayTrZ_UpPos();
					 if(!TrayTrZ_UpChk()) 
						 TrayTrZ_UpPos();
					 m_nStep=-1;
					 break;
				 }
			 }
		 }
		 break;
	default:
		 break;
	}
	m_nStep = nStep;
	return nStep;
}


/////


//	UDROBOT_X_TEACHMODE_Y_EXTSTART					0x80   output1
//	UDROBOT_X_PLAYMODE_Y_EXTSTOP					0x81  
//	UDROBOT_X_RUNNING_Y_ERRRESET					0x82   
//	UDROBOT_X_STOPPING_Y_EXTMOTORON					0x83
//	UDROBOT_X_INTERLOCKALARM_Y_EXTMOTOROFF			0x84	
//	UDROBOT_X_MOTORON_Y_LOWSPEED					0x85
//	UDROBOT_X_PGMEND_Y_PAUSE						0x86
// UDROBOT_X_ALARM_Y_EXTRESET						0x87
//	UDROBOT_X_BATWARN_Y_ORG							0x88
//	UDROBOT_X_HOMEPOS1_Y_CYCLESTOP					0x89
//	UDROBOT_X_HOMEPOS2_Y_PGMSTROBE					0x8A
// UDROBOT_X_PGMACK_Y_PGMSEL1						0x8B
//	UDROBOT_X_EXTRESET_Y_PGMSEL2					0x8C
//	UDROBOT_X_STATEOUT1_Y_PGMSEL3					0x8D
//	UDROBOT_X_STATEOUT2_Y_PGMSEL4					0x8E
//	UDROBOT_X_UNITREADY_Y_PGMSEL5					0x8F   16
//	UDROBOT_X_BUSY_Y_PGMSEL6						0x90   17
//	UDROBOT_X_CYCLESTOP_Y_PGMSEL7					0x91
//	UDROBOT_X_ORGERR_Y_PGMSEL8						0x92
//	UDROBOT_X_DATAERR_Y_SPEEDIN1					0x93
//	UDROBOT_X_MEMORYERR_Y_SPEEDIN2					0x94
//	UDROBOT_Y_SPEEDIN3								0x95
//	UDROBOT_Y_SPEEDIN4								0x96
//	UDROBOT_Y_SPEEDIN5								0x97	
//	UDROBOT_Y_SPEEDIN6								0x98
//	UDROBOT_Y_SPEEDIN7								0x99
//	UDROBOT_Y_RETRY									0x9A
//	UDROBOT_X_MANUALIN_Y_MANUALSELECT				0x9B
//	UDROBOT_X_MANUALCOMP_Y_MANUALGO					0x9C    29 30 31 32
//�޴��� ���� Ȯ�� ��Ʈ �߰� 2017/06/20 HSK.
//	UDROBOT_X_MANUALMOVE_STANBY						0xA0   33

//	UDROBOT_XY_VACON								0xA2   35
//	UDROBOT_XY_VACOFF								0xA3   36
//	UDROBOT_X_VACERR								0xA4

//2017/11/08, JSJUNG, TRAY VISION ��Ʈ �߰� .
//	UDROBOT_X_TRAYVIREADY_Y_TRAYVIUSE				0xA7
//	UDROBOT_X_TRAYVIOK_Y_TRAYVISTART				0xA8
//	UDROBOT_X_TRAYVING_Y_OKNGRTN					0xA9
//	UDROBOT_X_RBTOVIRETRY_Y_TRAYVISTARTCOMP			0xAA	

//	UDROBOT_XY_GETSTAGE								0xB1
//	UDROBOT_XY_STGCH1VACON							0xB2
//	UDROBOT_XY_STGCH1VACOFF							0xB3
//	UDROBOT_X_STGCH2VACON_Y_STG1OK					0xB4
//	UDROBOT_X_STGCH2VACOFF_Y_STG1NG					0xB5
//	UDROBOT_Y_STG2VACON								0xB6
//	UDROBOT_X_STG1GETERR_Y_STG2VACOFF				0xB7
//	UDROBOT_X_STG2GETERR_Y_STG2OK					0xB8
//	UDROBOT_X_STGGETCOMP_Y_STG2NG					0xB9
//	UDROBOT_XY_NGTRAYPUT							0xBB
//2017/11/09, JSJUNG,  ��Ʈ �߰� 
//	UDROBOT_X_STGCH1GET								0xBC
//	UDROBOT_X_STGCH2GET								0xBD

//2018-05-14,NAM, Add IO for ULD ROBOT Put in Hybrid Conveyor

// UDROBOT_XY_REQUESTPUT_CONVEYORMODE			  0xBF  64

// UDROBOT_XY_PUTDOWNCMP_CONVEYORREADY           0xC0     65

//	UDROBOT_X_NGTRAYPUTERR							0xC3   66
//	UDROBOT_X_NGTRAYPUTCOMP							0xC4   67
//	UDROBOT_XY_OKTRAYPUT							0xC5   68
//	UDROBOT_X_OKTRAYPUTERR							0xCD   69
//	UDROBOT_X_OKTRAYPUTCOMP							0xCE   70

//2017/11/08, JSJUNG,  ��Ʈ �߰� 
//	UDROBOT_Y_SQCOMPRTN								0xE3
//	UDROBOT_Y_SQERRRTN								0xE4
//	UDROBOT_Y_SQSKIP								0xE5

//	UDROBOT_X_TRAYDATANG							0xE8
//	UDROBOT_X_TRAYDATAOK							0xE9
//	UDROBOT_XY_UDROBOTINTERLOCK1					0xEE
//	UDROBOT_XY_UDROBOTINTERLOCK2					0xEF

//	UDROBOT_PERMIT									0x10

//	UDROBOT_MANUAL_POS								0x17

//	UDROBOT_TRAYCOUNT								0x18

//duong
/*int CTest_1::Run()
{
	ResetReturnValue();
	int nStep = m_nStep;
	if(nStep==stepStart)
	{
		CTrayBank chek_1;
		check_1.WriteCarrierCloseInfo(port,pos);
	}
}*/

//=======================


///////////20180801wz
int CStateUDRobotPick2::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;
	ResetReturnValue();
	int write=0;
	int nStep = m_nStep;
	CCellInfo* pCell=NULL;
	CCellInfo *pCellInfo = NULL;
	CELL_POS pos=(CELL_POS)-1;
	switch(nStep)
	{
	case 0:// robot runing check start
		if((theProcBank.GOODTrayBitX==0&&theProcBank.GOODTrayBitY==0)||!LDTrayLiftTrayChk(SENSOR_ON))
		{
			if(!ULDTrayLiftTrayChk(SENSOR_ON)&&!theUnitFunc.TrayTr_VacChk(VAC_ON)) 
			{
				m_nStep=-1;
				return -1;
			}
			theProcBank.m_bPickOUT2INTrayRequest=TRUE;
			m_nStep=-1;
			return -1;
		}
		else
		{
			theProcBank.m_bPickOUT2INTrayRequest=FALSE;
		}
		if(theProcBank.GOODTrayBitX==1&&theProcBank.GOODTrayBitY==1)
			pos=CELL_POS_TRAY_11;
		else if(theProcBank.GOODTrayBitX==1&&theProcBank.GOODTrayBitY==2)
			pos=CELL_POS_TRAY_12;
		else if(theProcBank.GOODTrayBitX==1&&theProcBank.GOODTrayBitY==3)
			pos=CELL_POS_TRAY_13;
		else if(theProcBank.GOODTrayBitX==1&&theProcBank.GOODTrayBitY==4)
			pos=CELL_POS_TRAY_14;
		else if(theProcBank.GOODTrayBitX==1&&theProcBank.GOODTrayBitY==5)
			pos=CELL_POS_TRAY_15;
		else if(theProcBank.GOODTrayBitX==2&&theProcBank.GOODTrayBitY==1)
			pos=CELL_POS_TRAY_21;
		else if(theProcBank.GOODTrayBitX==2&&theProcBank.GOODTrayBitY==2)
			pos=CELL_POS_TRAY_22;
	    else if(theProcBank.GOODTrayBitX==2&&theProcBank.GOODTrayBitY==3)
			pos=CELL_POS_TRAY_23;
		else if(theProcBank.GOODTrayBitX==2&&theProcBank.GOODTrayBitY==4)
			pos=CELL_POS_TRAY_24;
		else if(theProcBank.GOODTrayBitX==2&&theProcBank.GOODTrayBitY==5)
			pos=CELL_POS_TRAY_25;
		else if((theRecipeBank.m_Tray.m_nCountX+1)==theProcBank.GOODTrayBitX&&(theRecipeBank.m_Tray.m_nCountY+1)==theProcBank.GOODTrayBitY)
		{
			theProcBank.GOODTrayBitX=theRecipeBank.m_Tray.m_nCountX;
			theProcBank.GOODTrayBitY=theRecipeBank.m_Tray.m_nCountY;
			if((theRecipeBank.m_Tray.m_nCountX*theRecipeBank.m_Tray.m_nCountY)==4)
				pos=CELL_POS_TRAY_22;
			else if((theRecipeBank.m_Tray.m_nCountX*theRecipeBank.m_Tray.m_nCountY)==6)
				pos=CELL_POS_TRAY_23;
			else if((theRecipeBank.m_Tray.m_nCountX*theRecipeBank.m_Tray.m_nCountY)==8)
				pos=CELL_POS_TRAY_24;
			else if((theRecipeBank.m_Tray.m_nCountX*theRecipeBank.m_Tray.m_nCountY)==10)
				pos=CELL_POS_TRAY_25;
		}

		pCellInfo = theCellBank.GetCellInfo(pos);
		if(NULL==pCellInfo)
		{
			//theProcBank.m_TrayBank.GetTrayInfo(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_ALIGN)->MinusCell();		
			if(theProcBank.GOODTrayBitX!=1)
				theProcBank.GOODTrayBitX--;
			else
			{
				if(theProcBank.GOODTrayBitY!=1)
				{
					theProcBank.GOODTrayBitY--;
					theProcBank.GOODTrayBitX=theRecipeBank.m_Tray.m_nCountX;
				}
				else
				{
					theProcBank.GOODTrayBitX=0;
					theProcBank.GOODTrayBitY=0;
				}
			}
			break;
		}
		RobotCellVacOn(VAC_OFF,E_UD_ROBOT);	
		RobotCellVacOnChk(E_UD_ROBOT,VAC_OFF);	
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_RUNNING_Y_ERRRESET);
		//theDeviceMelsec.RobotBitOff(UDROBOT_X_MOTORON_Y_LOWSPEED);
		m_bRtn[1] = RobotCellVacOnChk(E_UD_ROBOT,VAC_OFF);
		if(IsReturnOk())
		{
			write = 1024; //pick2
			theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
			theDeviceMelsec.RobotBitOn(UDROBOT_XY_OKTRAYPUT);
			if((theRecipeBank.m_Tray.m_nCountX+1)==theProcBank.GOODTrayBitX&&(theRecipeBank.m_Tray.m_nCountY+1)==theProcBank.GOODTrayBitY)
			{
				theProcBank.GOODTrayBitX=theRecipeBank.m_Tray.m_nCountX;
				theProcBank.GOODTrayBitY=theRecipeBank.m_Tray.m_nCountY;
				UDRobotTrayCountBitOn(theProcBank.GOODTrayBitX,theProcBank.GOODTrayBitY);
			}
			else
			{
				UDRobotTrayCountBitOn(theProcBank.GOODTrayBitX,theProcBank.GOODTrayBitY);				
			}

			//UDRobotTrayCountBitOn(1,1);
			theLog[LOG_SEQUENCE_TRAY].AddBuf(_T("[%s] stepStart X:%d Y:%d"), m_strStateName, theProcBank.GOODTrayBitX,theProcBank.GOODTrayBitY);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			if(m_bRtn[0] == FALSE)	SetError(ALARM_UD_ROBOT_RUNNINGACK);
			else					SetError(ALARM_UD_ROBOT_VACON);
		}
		break;
	case 1:
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_OKTRAYPUT);//interlock
		if (m_bRtn[0]==FALSE )
		{
			write = 0; 
			theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepPutChk"), m_strStateName);
			//	theDeviceMelsec.RobotBitOn(UDROBOT_Y_SQCOMPRTN);//out 100
			//	theDeviceMelsec.RobotBitOff(UDROBOT_Y_SQSKIP);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 50. && m_Timer.Stop(FALSE) < 55)
		{
			theDeviceMelsec.RobotBitOn(UDROBOT_Y_SQSKIP);
		}
		else if (m_Timer.Stop(FALSE) > 60.)
		{
			UDRobotBitAllOff(E_PUT_1,FALSE);
			SetError(ALARM_UD_ROBOT_OKPUTINTERLOCKOFFACK);
		}
		break;
	case 2://receive vac on
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_VACON);
		if (IsReturnOk())
		{
			theDeviceMelsec.RobotBitOff(UDROBOT_Y_SQCOMPRTN);
			RobotCellVacOn(VAC_ON,E_UD_ROBOT);
			Sleep(200);
			theDeviceMelsec.RobotBitOff(UDROBOT_Y_SQSKIP);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepRobotVacOn"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 3. && m_Timer.Stop(FALSE) < 3.5)
		{
			theDeviceMelsec.RobotBitOn(UDROBOT_Y_SQSKIP);
		}
		else if (m_Timer.Stop(FALSE) > 7.)
		{
			UDRobotBitAllOff(E_PUT_1,FALSE);
			if (m_bRtn[0] == TRUE)					SetError(ALARM_UD_ROBOT_VACONACK);
			else									SetError(ALARM_UD_STG_VAC);
		}
		break;
	case 3:// check vac on
		m_bRtn[0] = RobotCellVacOnChk(E_UD_ROBOT,VAC_ON);
		RobotCellVacOn(VAC_ON,E_UD_ROBOT);
		m_bRtn[0] = RobotCellVacOnChk(E_UD_ROBOT,VAC_ON);
		if (IsReturnOk())
		{
			theDeviceMelsec.RobotBitOn(UDROBOT_XY_VACON);
			theDeviceMelsec.RobotBitOff(UDROBOT_XY_VACOFF);
			if(theProcBank.m_bModel2FullTray)
				theProcBank.m_bModel2FullTray=FALSE;
			CellData_RobotFromTray(theProcBank.GOODTrayBitX,theProcBank.GOODTrayBitY);
			::SendMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd,UDMSG_REMOVE_ONE_CELL,theProcBank.GOODTrayBitX,theProcBank.GOODTrayBitY);
			theProcBank.m_TrayBank.GetTrayInfo(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_ALIGN)->MinusCell();		
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepRobotVacOnChk"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}	
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			UDRobotBitAllOff(E_PUT_1,FALSE);
			SetError(ALARM_UD_ROBOT_VACON);
		}
		break;
		//case 4:

		//m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_VACON);
		//if (IsReturnOk())
		//{
		//	//RobotCellVacOn(VAC_ON,E_UD_ROBOT);

		//	theDeviceMelsec.RobotBitOff(UDROBOT_Y_SQCOMPRTN);
		//	theDeviceMelsec.RobotBitOn(UDROBOT_XY_VACON);
		//	theDeviceMelsec.RobotBitOff(UDROBOT_XY_VACOFF);	
		///*	pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_GOOD_TRAY));
		//	if(pCell != NULL)
		//	{
		//		theConfigBank.m_UnloadRunInfo.m_sEndCellID = pCell->m_strCellID;	
		//	}
		//
		//       theProcBank.m_TrayBank.GetTrayInfo(NG_TRAY_PORT,TRAY_INFO_POS_NG_IN_ALIGN)->ReduceCell(CELL_POS_GOOD_TRAY);
		//		theProcBank.m_iPickTotalCellNow--;*/
		//		m_Timer.Start();
		//	nStep++;
		//}
		//else if (m_Timer.Stop(FALSE) > 300.)
		//{
		//	UDRobotBitAllOff(E_PUT_1,FALSE);
		//	SetError(ALARM_UD_ROBOT_OKPUTCOMP);
		//}
		//break;
	case 4:				
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_OKTRAYPUTCOMP);
		if (IsReturnOk())
		{
			/*if(theProcBank.GOODTrayBitX==1&&theProcBank.GOODTrayBitY==1)
			{
				if(!ULDTrayLiftTrayChk(SENSOR_ON)) 
				{
					m_nStep=-1;
					return -1;
				}
				theProcBank.m_bPickOUT2INTrayRequest=TRUE;
			}*/
			if(theProcBank.GOODTrayBitX==theRecipeBank.m_Tray.m_nCountX&&
				theProcBank.GOODTrayBitY==theRecipeBank.m_Tray.m_nCountY)
			{
				theProcBank.m_TrayBank.GetTrayInfo(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_ALIGN)->ReSetCellFull();
			}

			if(theProcBank.GOODTrayBitX!=1)
				theProcBank.GOODTrayBitX--;
			else
			{
				if(theProcBank.GOODTrayBitY!=1)
				{
					theProcBank.GOODTrayBitY--;
					theProcBank.GOODTrayBitX=theRecipeBank.m_Tray.m_nCountX;
				}
				else
				{
					theProcBank.GOODTrayBitX=0;
					theProcBank.GOODTrayBitY=0;
				}
			}

			theDeviceMelsec.RobotBitOn(UDROBOT_Y_SQCOMPRTN);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepPutComp"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			UDRobotBitAllOff(E_PUT_1,FALSE);
			SetError(ALARM_UD_ROBOT_OKPUTCOMP);
		}
		break;
	case 5:
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_OKTRAYPUT);
		if (IsReturnOk())
		{
			theDeviceMelsec.RobotBitOff(UDROBOT_Y_SQCOMPRTN);
			UDRobotBitAllOff(E_PUT_1,FALSE);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"), m_strStateName);
			theLog[LOG_UNLOADING].AddBuf( _T("Good Put"));
			m_Timer.Start();

			if(theConfigBank.m_Option.m_bUseConveyorTestModel)	
			{
				nStep=10;
			}
			else
			{
				nStep = stepIdle;
			}
		}
		else if (m_Timer.Stop(FALSE) > 100.&& m_Timer.Stop(FALSE) < 150)
		{

			//theDeviceMelsec.RobotBitOff(0x63);
		}

		else if (m_Timer.Stop(FALSE) > 30.)
		{
			UDRobotBitAllOff(E_PUT_1,FALSE);
			SetError(ALARM_UD_ROBOT_OKPUTINTERLOCKONACK);
		}
		break;
	////////////////////// put back
	case 10:	
		theDeviceMelsec.RobotBitOn(UDROBOT_XY_REQUESTPUT_CONVEYORMODE);
		theDeviceMelsec.RobotBitOn(UDROBOT_XY_PUTDOWNCMP_CONVEYORREADY);
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_RUNNING_Y_ERRRESET);
		if (IsReturnOk())
		{
			write = 256; 
			///write = 32;
			theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s]  con stepStart"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 10.)
		{
			if(m_bRtn[0] == FALSE)	SetError(ALARM_UD_ROBOT_RUNNINGACK);
			else					SetError(ALARM_UD_ROBOT_VACON);		
		}
		break;
	case 11:
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_REQUESTPUT_CONVEYORMODE);
		if ((m_bRtn[0] == FALSE) )
		{
			//2017.10.31, JSJUNG. �۹� �ñ׳� ����
			write = 0; 
			theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepPutChk"), m_strStateName);
			//2018-05-15,NAM, ON Conveyor Ready. 
			theDeviceMelsec.RobotBitOn(UDROBOT_XY_PUTDOWNCMP_CONVEYORREADY);
			theDeviceMelsec.RobotBitOn(UDROBOT_Y_SQCOMPRTN);
			theDeviceMelsec.RobotBitOff(UDROBOT_Y_SQSKIP);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 50. && m_Timer.Stop(FALSE) < 55)
		{
			theDeviceMelsec.RobotBitOn(UDROBOT_Y_SQSKIP);
		}
		else if (m_Timer.Stop(FALSE) > 100.)
		{
			UDRobotBitAllOff(E_PUT_3,FALSE);
			if(m_bRtn[0] == TRUE) SetError(ALARM_UD_ROBOT_CONVPUTINTERLOCKOFFACK);
			else if(m_bRtn[1] == TRUE) SetError(ALARM_UD_HYBRID_CONV_RUN);
			else if(m_bRtn[2] == TRUE) SetError(ALARM_UD_HYBRID_ROBOT_READY);
			else if(m_bRtn[3] == FALSE) SetError(ALARM_UD_HYBRID_ROBOT_RUN);
		}
		break;
	case 12:
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_VACOFF);
		if (IsReturnOk())
		{
			RobotCellVacOn(VAC_OFF,E_UD_ROBOT);
			theDeviceMelsec.RobotBitOff(UDROBOT_Y_SQCOMPRTN);
			theDeviceMelsec.RobotBitOn(UDROBOT_XY_VACOFF);
			theDeviceMelsec.RobotBitOff(UDROBOT_XY_VACON);

			// UnLoad Tact ����.
			TactUpdate(UNLOADER_TACT);
			SetProductData(CELL_POS_UD_ROBOT);

			//2018-05-23,NAM, Cell Load Check Tact Time Start. 
			theProcBank.m_CellLoadChkTact.Start();

			UnloadTactTime();
			// Ʈ�� �ƿ� ������ Ʈ���� �� �����̵� ����.
			// CIM_CellTrackOut();
			WriteCellLog(CELL_POS_UD_ROBOT);
			//2018-02-08 JSJUNG, TRACKOUT INFO ���� ����(������ å�� ��û)
			theCellBank.WriteTrackOutInfo(CELL_POS_UD_ROBOT);
			//2018-05-16,MGYUN, ������ �̵� ULD CONV��
			CellData_ULDConvFromUDRobot(CELL_POS_UD_ROBOT);	

			//2018-05-16,MGYUN, ������ �̵� �� ����
			CellData_ULDConvRemove();


			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepVacOff"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 100000000.) // Chaning from 3 second to 15second 
		{
			UDRobotBitAllOff(E_PUT_3,FALSE);
			SetError(ALARM_UD_ROBOT_VACOFFACK);
		}
		break;
	case 13:
		//m_bRtn[0] = RobotCellVacOnChk(E_UD_ROBOT,VAC_OFF);
		if (IsReturnOk())
		{
			RobotBlowOff(E_UD_ROBOT);	

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepVacOffChk"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 30.)
		{
			UDRobotBitAllOff(E_PUT_3,FALSE);
			SetError(ALARM_UD_ROBOT_VACON);
		}
		break;
	case 14:				
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_PUTDOWNCMP_CONVEYORREADY);		
		if (IsReturnOk())
		{
			theDeviceMelsec.RobotBitOn(UDROBOT_Y_SQCOMPRTN);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepPutComp"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 30.)
		{
			UDRobotBitAllOff(E_PUT_3,FALSE);
			SetError(ALARM_UD_ROBOT_CONVPUTCOMP);
		}
		break;
	case 15:				
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_REQUESTPUT_CONVEYORMODE);

		if (m_bRtn[0]==FALSE)
		{
			theDeviceMelsec.RobotBitOff(UDROBOT_Y_SQCOMPRTN);
			UDRobotBitAllOff(E_PUT_3,FALSE);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"), m_strStateName);
			theLog[LOG_UNLOADING].AddBuf( _T("Convayor Put"));
			theUnitFunc.SetOutPutIO(Y_HYBRID_SEND_COMPLETE,ON);
			theUnitFunc.SetOutPutIO(Y_HYBRID_ROBOT_RUN,OFF);
			Sleep(100);
			theUnitFunc.SetOutPutIO(Y_HYBRID_SEND_COMPLETE,OFF);
			nStep = stepIdle;
		}
		else if (m_Timer.Stop(FALSE) > 100.)
		{
			UDRobotBitAllOff(E_PUT_3,FALSE);
			SetError(ALARM_UD_ROBOT_CONVPUTINTERLOCKOFFACK);
		}
		break;
	default:
		break;
	}
	m_nStep = nStep;
	return nStep;
}





//////////////////////20180816wjp

int CStateUDRobotGoodPut::Run()
{
	CCellInfo *pCell = NULL;

	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;
	long write;
	switch(nStep)
	{
	case stepStart:				// �κ��� Cell �� �ְ� Tray ���� Ȯ�� �� �̵�.
		if(theProcBank.GOODTrayBitX>theRecipeBank.m_Tray.m_nCountX||theProcBank.GOODTrayBitY>theRecipeBank.m_Tray.m_nCountY)
		{
			nStep=-1;
			break;
		}
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_RUNNING_Y_ERRRESET);
		m_bRtn[1] = RobotCellVacOnChk(E_UD_ROBOT,VAC_ON);

		if (IsReturnOk())
		{
			write = 128; //PUTOK3
			theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
			// OK Cell Put
			theDeviceMelsec.RobotBitOn(UDROBOT_XY_OKTRAYPUT);
			// OK Cell ���ƾ��� ��ġ Bit On
			UDRobotTrayCountBitOn(theProcBank.GOODTrayBitX,theProcBank.GOODTrayBitY);

			theLog[LOG_SEQUENCE_TRAY].AddBuf(_T("[%s] stepStart X:%d Y:%d"), m_strStateName, theProcBank.GOODTrayBitX,theProcBank.GOODTrayBitY);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"), m_strStateName);

			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			if(m_bRtn[0] == FALSE)	SetError(ALARM_UD_ROBOT_RUNNINGACK);
			else					SetError(ALARM_UD_ROBOT_VACON);
		}
		break;
	case stepPutChk:
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_OKTRAYPUT);
		if (m_bRtn[0] == FALSE)
		{
			//2017.10.31, JSJUNG. �۹� �ñ׳� ����
			write = 0; 
			theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepPutChk"), m_strStateName);
			theDeviceMelsec.RobotBitOn(UDROBOT_Y_SQCOMPRTN);
			theDeviceMelsec.RobotBitOff(UDROBOT_Y_SQSKIP);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 5. && m_Timer.Stop(FALSE) < 5.5)
		{
			theDeviceMelsec.RobotBitOn(UDROBOT_Y_SQSKIP);
		}
		else if (m_Timer.Stop(FALSE) > 10.)
		{
			UDRobotBitAllOff(E_PUT_1,FALSE);
			SetError(ALARM_UD_ROBOT_OKPUTINTERLOCKOFFACK);
		}
		break;
	case stepVacOff:		
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_VACOFF);
		if (IsReturnOk())
		{
			RobotCellVacOn(VAC_OFF,E_UD_ROBOT);

			theDeviceMelsec.RobotBitOff(UDROBOT_Y_SQCOMPRTN);
			theDeviceMelsec.RobotBitOn(UDROBOT_XY_VACOFF);
			theDeviceMelsec.RobotBitOff(UDROBOT_XY_VACON);
	
			// UnLoad Tact ����.
			TactUpdate(UNLOADER_TACT);
			
			SetProductData(CELL_POS_UD_ROBOT);

			//2018-05-23,NAM, Cell Load Check Tact Time Start. 
			theProcBank.m_CellLoadChkTact.Start();


			UnloadTactTime();
//			Ʈ�� �ƿ� ������ Ʈ���� �� �����̵� ����.
//			CIM_CellTrackOut();
	
			WriteCellLog(CELL_POS_UD_ROBOT);
			//2018-02-08 JSJUNG, TRACKOUT INFO ���� ����(������ å�� ��û)
			theCellBank.WriteTrackOutInfo(CELL_POS_UD_ROBOT);

			//20170207 byskcho
			//ULD ROBOT���� Tray�� �� ���� �̵�

			//2018wjp
			CCellInfo *pCellInfo = NULL;
		    pCellInfo = theCellBank.GetCellInfo(CELL_POS_UD_ROBOT);
			CellData_TrayFromRobot(CELL_POS_UD_ROBOT,theProcBank.GOODTrayBitX,theProcBank.GOODTrayBitY);
			cell.row=theProcBank.GOODTrayBitX;
			cell.col=theProcBank.GOODTrayBitY;
			if(NULL!=pCellInfo)
				cell.strCode=pCellInfo->m_strCellID;
			else
				cell.strCode=_T("");
			::SendMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd,UDMSG_UPDATE_ONE_CELL,(WPARAM)(&cell),0);

			CellData_OKTrayFromUDRobot(CELL_POS_UD_ROBOT);

			//20170207 byskcho
			//�� ������  tray���� �����ϹǷ� ����
            //CellData_UDRobotRemove();
			//������ Ʈ���̿� ù���� ���� ������ 301 ���� 2017/05/04 HSK.
			if(theProcBank.m_bReleaseTrayChk)
			{
				if(theProcBank.GOODTrayBitX == 1 && theProcBank.GOODTrayBitY == 1)
				{
					theProcBank.m_bReleaseTrayChk = FALSE;
					theLog[LOG_SEQUENCE_TRAY].AddBuf(_T("[%s] CIM_CarrierCellLoadStart X:%d Y:%d"), m_strStateName, theProcBank.GOODTrayBitX,theProcBank.GOODTrayBitY);
					
					//2017-05-17,skcho, 301/302 ����� CELL ID ���� �߰� CIM QUAL ��û					
					pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_GOOD_TRAY));
					if(pCell != NULL)
					{
						theConfigBank.m_UnloadRunInfo.m_sStartCellID = pCell->m_strCellID;
					}

					CIM_CarrierCellLoadStart();
				}
			}
			
			//2017-05-17,skcho, 301/302 ����� CELL ID ���� �߰� CIM QUAL ��û
		    //2017-05-17,skcho, ���⼭ ������ �����ϰ� CStateGoodTrayTRGet���� ���� ���
			
			pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_GOOD_TRAY));
			if(pCell != NULL)
			{
				theConfigBank.m_UnloadRunInfo.m_sEndCellID = pCell->m_strCellID;	
			}

			//CIM_CarrierCellLoadEnd()���� �ʿ��� ���� Ʈ���� ��Ʈ�� �� ������ �����ش�. 2017/05/04 HSK.
			theProcBank.m_nLotCellCnt++;

			// Tray ������ �� Cell ���� ī��Ʈ.
			theProcBank.m_TrayBank.GetTrayInfo(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_ALIGN)->AddCell(CELL_POS_GOOD_TRAY);
			
			//20170207 byskcho
			//��������  AddCell()����   tray�� �����ϰ� ����
			CellData_OKTrayRemove();
			
			// Tray ���� Bit ī��Ʈ. Cell ���� ī��Ʈ�� ������ ���� ��� �Ϸ� ������ ��� ���� �߻� �Ͽ� ���� ����..
			//2017-03-05,skcho, cell ��ġ(strCell_Location)�� �°� �ϱ� ���ؼ� addcell�� �̵�
			/*theProcBank.GOODTrayBitX++;				
			if(theProcBank.GOODTrayBitX > theRecipeBank.m_Tray.m_nCountX)
			{
				theProcBank.GOODTrayBitX = 1;
				theProcBank.GOODTrayBitY++;
			}
			if(theProcBank.GOODTrayBitY > theRecipeBank.m_Tray.m_nCountY)
				theProcBank.GOODTrayBitY = 1;
			*/

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepVacOff"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			UDRobotBitAllOff(E_PUT_1,FALSE);
			SetError(ALARM_UD_ROBOT_VACOFFACK);
		}
		break;
	case stepVacOffChk:
		m_bRtn[0] = RobotCellVacOnChk(E_UD_ROBOT,VAC_OFF);
		if (IsReturnOk())
		{
			RobotBlowOff(E_UD_ROBOT);

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepVacOffChk"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			UDRobotBitAllOff(E_PUT_1,FALSE);
			SetError(ALARM_UD_ROBOT_VACON);
		}
		break;
	case stepPutComp:				//Wait ��ġ üũ �� End
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_OKTRAYPUTCOMP);
		if (IsReturnOk())
		{
			
			theDeviceMelsec.RobotBitOn(UDROBOT_Y_SQCOMPRTN);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepPutComp"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			UDRobotBitAllOff(E_PUT_1,FALSE);
			SetError(ALARM_UD_ROBOT_OKPUTCOMP);
		}
		break;
	case stepEnd:
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_OKTRAYPUT);
		if (IsReturnOk())
		{
			theDeviceMelsec.RobotBitOff(UDROBOT_Y_SQCOMPRTN);
			UDRobotBitAllOff(E_PUT_1,FALSE);
			//2018-06-29,NAM
			//2018-05-22,MGYUN, GOOD TRAY PUT �Ϸ� �� FALSE��
			//theProcBank.HybridDelayFlag = FALSE;

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"), m_strStateName);
			theLog[LOG_UNLOADING].AddBuf( _T("Good Put"));

			
			nStep = stepIdle;
		}
		else if (m_Timer.Stop(FALSE) > 1.&& m_Timer.Stop(FALSE) < 1.5)
		{

			//theDeviceMelsec.RobotBitOff(0x63);
		}

		else if (m_Timer.Stop(FALSE) > 3.)
		{
			UDRobotBitAllOff(E_PUT_1,FALSE);
			SetError(ALARM_UD_ROBOT_OKPUTINTERLOCKONACK);
		}
		break;
	}

	m_nStep = nStep;

	return nStep;
}


int CStateUDRobotNGPut::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;
	long write;
	switch(nStep)
	{
	case stepStart:				// �κ��� Cell �� �ְ� Tray ���� Ȯ�� �� �̵�.
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_RUNNING_Y_ERRRESET);
		m_bRtn[1] = RobotCellVacOnChk(E_UD_ROBOT,VAC_ON);
		if (IsReturnOk())
		{
			write = 32; //PUTOK2
			theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
			theDeviceMelsec.RobotBitOn(UDROBOT_XY_NGTRAYPUT);

			UDRobotTrayCountBitOn(theProcBank.NGTrayBitX,theProcBank.NGTrayBitY);

			/*
				NG TRAY PUT 6 PANEL 
				Add Source
				X : 
			*/
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"), m_strStateName);
			theLog[LOG_SEQUENCE_TRAY].AddBuf(_T("[%s] stepStart X:%d Y:%d"), m_strStateName, theProcBank.NGTrayBitX,theProcBank.NGTrayBitY);

			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			if(m_bRtn[0] == FALSE)	SetError(ALARM_UD_ROBOT_RUNNINGACK);
			else					SetError(ALARM_UD_ROBOT_VACON);		
		}
		break;
	case stepPutChk:
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_NGTRAYPUT);
		if (m_bRtn[0] == FALSE)
		{
			//2017.10.31, JSJUNG. �۹� �ñ׳� ����
			write = 0; 
			theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepPutChk"), m_strStateName);
			theDeviceMelsec.RobotBitOn(UDROBOT_Y_SQCOMPRTN);
			theDeviceMelsec.RobotBitOff(UDROBOT_Y_SQSKIP);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 5. && m_Timer.Stop(FALSE) < 5.5)
		{
			theDeviceMelsec.RobotBitOn(UDROBOT_Y_SQSKIP);
		}
		else if (m_Timer.Stop(FALSE) > 10.)
		{
			UDRobotBitAllOff(E_PUT_2,FALSE);
			SetError(ALARM_UD_ROBOT_NGPUTINTERLOCKOFFACK);
		}
		break;
	case stepVacOff:
		
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_VACOFF);
		if (IsReturnOk())
		{
			RobotCellVacOn(VAC_OFF,E_UD_ROBOT);
			theDeviceMelsec.RobotBitOff(UDROBOT_Y_SQCOMPRTN);
			theDeviceMelsec.RobotBitOn(UDROBOT_XY_VACOFF);
			theDeviceMelsec.RobotBitOff(UDROBOT_XY_VACON);

			// UnLoad Tact ����.
			TactUpdate(UNLOADER_TACT);

			SetProductData(CELL_POS_UD_ROBOT);

			UnloadTactTime();
//			Ʈ�� �ƿ� ������ Ʈ���� �� �����̵� ����.
//			CIM_CellTrackOut();

			//2018-05-23,NAM, Cell Load Check Tact Time Start. 
			theProcBank.m_CellLoadChkTact.Start();

			WriteCellLog(CELL_POS_UD_ROBOT);
			//2018-02-08 JSJUNG, TRACKOUT INFO ���� ����(������ å�� ��û)
			theCellBank.WriteTrackOutInfo(CELL_POS_UD_ROBOT);

			//2017-03-03, skcho ������ �̵� Ʈ���̷�
			CellData_NGTrayFromUDRobot(CELL_POS_UD_ROBOT);
			
			//2017-03-03, skcho //�� ������  tray���� �����ϹǷ� ����
			//CellData_UDRobotRemove();
			theProcBank.m_TrayBank.GetTrayInfo(NG_TRAY_PORT,TRAY_INFO_POS_NG_IN_ALIGN)->AddCell(CELL_POS_NG_TRAY);

			//2017-03-03, skcho
			CellData_NGTrayRemove();

			theProcBank.NGTrayBitX++;				
			if(theProcBank.NGTrayBitX > theRecipeBank.m_Tray.m_nCountX)
			{
				theProcBank.NGTrayBitX = 1;
				theProcBank.NGTrayBitY++;
			}
			if(theProcBank.NGTrayBitY > theRecipeBank.m_Tray.m_nCountY)
				theProcBank.NGTrayBitY = 1;

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepVacOff"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 3.) // FROM 3 TO 10. 
		{
			UDRobotBitAllOff(E_PUT_2,FALSE);
			SetError(ALARM_UD_ROBOT_VACOFFACK);
		}
		break;
	case stepVacOffChk:
		m_bRtn[0] = RobotCellVacOnChk(E_UD_ROBOT,VAC_OFF);
		if (IsReturnOk())
		{
			RobotBlowOff(E_UD_ROBOT);	

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepVacOffChk"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			UDRobotBitAllOff(E_PUT_2,FALSE);
			SetError(ALARM_UD_ROBOT_VACON);
		}
		break;
	case stepPutComp:				//Wait ��ġ üũ �� End
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_NGTRAYPUTCOMP);
		if (IsReturnOk())
		{
			theDeviceMelsec.RobotBitOn(UDROBOT_Y_SQCOMPRTN);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepPutComp"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			UDRobotBitAllOff(E_PUT_2,FALSE);
			SetError(ALARM_UD_ROBOT_NGPUTCOMP);
		}
		break;
	case stepEnd:				//Wait ��ġ üũ �� End
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_NGTRAYPUT);
		if (IsReturnOk())
		{
			theDeviceMelsec.RobotBitOff(UDROBOT_Y_SQCOMPRTN);
			UDRobotBitAllOff(E_PUT_2,FALSE);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"), m_strStateName);
			theLog[LOG_UNLOADING].AddBuf( _T("NG Put"));

			
			nStep = stepIdle;
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			UDRobotBitAllOff(E_PUT_2,FALSE);
			SetError(ALARM_UD_ROBOT_NGPUTINTERLOCKONACK);
		}
		break;
	}

	m_nStep = nStep;

	return nStep;
}


//2018-05-14.SKCHO CONVAYOR �߰�
int CStateUDRobotConvPut::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;
	long write;
	switch(nStep)
	{
	case stepStart:				// �κ��� Cell �� �ְ� Tray ���� Ȯ�� �� �̵�.
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_RUNNING_Y_ERRRESET);
		m_bRtn[1] = RobotCellVacOnChk(E_UD_ROBOT,VAC_ON);
		if (IsReturnOk())
		{
			//write = 32; //PUTOK3  �� �����ʿ�
			write = 256; 
			theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
			
			//2018-05-15,NAM, Send Signal for CONVEYOR 
			//theUnitFunc.SetOutPutIO(Y_HYBRID_ROBOT_READY,OFF);
			theUnitFunc.SetOutPutIO(Y_HYBRID_ROBOT_RUN,ON);

			//theDeviceMelsec.RobotBitOn(UDROBOT_XY_NGTRAYPUT);
			theDeviceMelsec.RobotBitOn(UDROBOT_XY_REQUESTPUT_CONVEYORMODE);

			//--UDRobotTrayCountBitOn(theProcBank.NGTrayBitX,theProcBank.NGTrayBitY);

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"), m_strStateName);
			//theLog[LOG_SEQUENCE_TRAY].AddBuf(_T("[%s] stepStart X:%d Y:%d"), m_strStateName, theProcBank.NGTrayBitX,theProcBank.NGTrayBitY);

			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			if(m_bRtn[0] == FALSE)	SetError(ALARM_UD_ROBOT_RUNNINGACK);
			else					SetError(ALARM_UD_ROBOT_VACON);		
		}
		break;
	case stepPutChk:
		//m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_NGTRAYPUT);
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_REQUESTPUT_CONVEYORMODE);
		// 2018-05-15,NAM, Check Hybrid Convey Run On or Off. 
		m_bRtn[1]= (BOOL)theUnitFunc.GetInPutIOCheck(X_HYBRID_CONV_RUN);
		//m_bRtn[2]= theUnitFunc.GetOutPutIOState(Y_HYBRID_ROBOT_READY);
		m_bRtn[3]= theUnitFunc.GetOutPutIOState(Y_HYBRID_ROBOT_RUN);
		//if ((m_bRtn[0] == FALSE) && (m_bRtn[1]==FALSE)&& (m_bRtn[2]==FALSE) && (m_bRtn[3]==TRUE))
		if ((m_bRtn[0] == FALSE) && (m_bRtn[1]==FALSE) && (m_bRtn[3]==TRUE))
		{
			//2017.10.31, JSJUNG. �۹� �ñ׳� ����
			write = 0; 
			theDeviceMelsec.RobotWordSend(UDROBOT_PERMIT,&write,sizeof(long), FALSE);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepPutChk"), m_strStateName);
			
			//2018-05-15,NAM, ON Conveyor Ready. 
			theDeviceMelsec.RobotBitOn(UDROBOT_XY_PUTDOWNCMP_CONVEYORREADY);

			theDeviceMelsec.RobotBitOn(UDROBOT_Y_SQCOMPRTN);
			theDeviceMelsec.RobotBitOff(UDROBOT_Y_SQSKIP);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 5. && m_Timer.Stop(FALSE) < 5.5)
		{
			theDeviceMelsec.RobotBitOn(UDROBOT_Y_SQSKIP);
		}
		else if (m_Timer.Stop(FALSE) > 10.)
		{
			UDRobotBitAllOff(E_PUT_3,FALSE);
			if(m_bRtn[0] == TRUE) SetError(ALARM_UD_ROBOT_CONVPUTINTERLOCKOFFACK);
			else if(m_bRtn[1] == TRUE) SetError(ALARM_UD_HYBRID_CONV_RUN);
			else if(m_bRtn[2] == TRUE) SetError(ALARM_UD_HYBRID_ROBOT_READY);
			else if(m_bRtn[3] == FALSE) SetError(ALARM_UD_HYBRID_ROBOT_RUN);
		}
		break;
	case stepVacOff:
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_VACOFF);
		if (IsReturnOk())
		{
			RobotCellVacOn(VAC_OFF,E_UD_ROBOT);
			theDeviceMelsec.RobotBitOff(UDROBOT_Y_SQCOMPRTN);
			theDeviceMelsec.RobotBitOn(UDROBOT_XY_VACOFF);
			theDeviceMelsec.RobotBitOff(UDROBOT_XY_VACON);

			// UnLoad Tact ����.
			TactUpdate(UNLOADER_TACT);
			SetProductData(CELL_POS_UD_ROBOT);

			//2018-05-23,NAM, Cell Load Check Tact Time Start. 
			theProcBank.m_CellLoadChkTact.Start();

			UnloadTactTime();
			// Ʈ�� �ƿ� ������ Ʈ���� �� �����̵� ����.
			// CIM_CellTrackOut();
			WriteCellLog(CELL_POS_UD_ROBOT);
			//2018-02-08 JSJUNG, TRACKOUT INFO ���� ����(������ å�� ��û)
			theCellBank.WriteTrackOutInfo(CELL_POS_UD_ROBOT);
			//2018-05-16,MGYUN, ������ �̵� ULD CONV��
			CellData_ULDConvFromUDRobot(CELL_POS_UD_ROBOT);	
			
			//2018-05-16,MGYUN, ������ �̵� �� ����
			CellData_ULDConvRemove();


			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepVacOff"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 10.) // Chaning from 3 second to 15second 
		{
			UDRobotBitAllOff(E_PUT_3,FALSE);
			SetError(ALARM_UD_ROBOT_VACOFFACK);
		}
		break;
	case stepVacOffChk:
		m_bRtn[0] = RobotCellVacOnChk(E_UD_ROBOT,VAC_OFF);
		if (IsReturnOk())
		{
			RobotBlowOff(E_UD_ROBOT);	

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepVacOffChk"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			UDRobotBitAllOff(E_PUT_3,FALSE);
			SetError(ALARM_UD_ROBOT_VACON);
		}
		break;
	case stepPutComp:				//Wait ��ġ üũ �� End
		//m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_NGTRAYPUTCOMP);
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_PUTDOWNCMP_CONVEYORREADY);		
		if (IsReturnOk())
		{
			theDeviceMelsec.RobotBitOn(UDROBOT_Y_SQCOMPRTN);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepPutComp"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 3.)
		{
			UDRobotBitAllOff(E_PUT_3,FALSE);
			SetError(ALARM_UD_ROBOT_CONVPUTCOMP);
		}
		break;
	case stepEnd:				//Wait ��ġ üũ �� End
		//m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_NGTRAYPUT);
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_REQUESTPUT_CONVEYORMODE);

		if (m_bRtn[0]==FALSE)
		{
			theDeviceMelsec.RobotBitOff(UDROBOT_Y_SQCOMPRTN);
			UDRobotBitAllOff(E_PUT_3,FALSE);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"), m_strStateName);
			theLog[LOG_UNLOADING].AddBuf( _T("Convayor Put"));
			
			
			//2018-05-15,NAM, Send Signal for CONVEYOR 
			theUnitFunc.SetOutPutIO(Y_HYBRID_SEND_COMPLETE,ON);
			theUnitFunc.SetOutPutIO(Y_HYBRID_ROBOT_RUN,OFF);
			//theUnitFunc.SetOutPutIO(Y_HYBRID_ROBOT_READY,ON);
			Sleep(100);
			theUnitFunc.SetOutPutIO(Y_HYBRID_SEND_COMPLETE,OFF);
			
			

			nStep = stepIdle;
		}
		else if (m_Timer.Stop(FALSE) > 10.)
		{
			UDRobotBitAllOff(E_PUT_3,FALSE);
			SetError(ALARM_UD_ROBOT_CONVPUTINTERLOCKOFFACK);
		}
		break;
	}

	m_nStep = nStep;

	return nStep;
}


//2017-08-30,SKCHO, AGV �ű� ������ �߰�
#define AGV_REQUEST_TIMEOUT 3.0  //agv request ��ȣ �̻� Ÿ�Ӿƿ�
#define AGV_CONV_TIMEOUT 20.0    //agv conv ���� �̻� Ÿ�Ӿƿ�/ tray ��� �� �ɸ� �߻�
#define AGV_RUN_TIMEOUT 3.0      //agv run ��ȣ �̻� Ÿ�Ӿƿ�

int CStateGoodTrayReceive::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

//	--OK LOAD
//x141 req  //y102 req
//x142 con	//y103 connection
//x143 run	//y104 run

	switch(nStep)
	{
	case stepStart:				// AGV Ready üũ �� �ε� ��û
		m_bRtn[0] = BufConvSenChk(GOOD_IN_TRAY_PORT,SENSOR_OFF);//����üũ�� ������ �ȿ��� �Ѵ�.2017/04/29 HSK.
		if (IsReturnOk())
		{
			theLog[LOG_AGV].AddBuf( _T("[%s] stepStart"), m_strStateName);	


			//2017-08-30,SKCHO,�˶� �߻� �� ABORT �ʱ�ȭ
			SendAGVAbort(GOOD_IN_TRAY_PORT,OFF);		

			m_Timer.Start();
			nStep++;
		}
		else
		{
			SetError(ALARM_OKTRAY_IN_BUFF_CONV_TRAY_CHK);
			theLog[LOG_AGV].AddBuf( _T("[%s] ALARM_OKTRAY_IN_BUFF_CONV_TRAY_CHK"), m_strStateName);	
		}
		break;
	case stepConnectChk:
		m_bRtn[0] =AGVConnectionChk(GOOD_IN_TRAY_PORT);// AGVRequestChk(GOOD_IN_TRAY_PORT);		//CONNECTION CHK(X_142). JSJUNG. 20170307
		if (IsReturnOk())
		{
			//�� ��ȣ ������ ���� ���δ� ����. ���� ���߰� �߰��� �ٲ����� �ִ�. 2017/05/02 HSK.
			SendAGVConnection(GOOD_IN_TRAY_PORT,ON);		
			SendAGVReq(GOOD_IN_TRAY_PORT,ON);		
			SendAGVStart(GOOD_IN_TRAY_PORT,OFF);	

			BottomStoperUpDown(GOOD_IN_TRAY_PORT, CYL_UP, TRAY_IN);  //IN PORT TRAY STOPPER �ø�

			theUnitFunc.SetOutPutIO(Y_GOOD_LD_TRAY_MUTING_LAMP_ON,ON);
			theUnitFunc.SetOutPutIO(Y_GOOD_LD_TRAY_REQ_SW,ON);
			theUnitFunc.SetOutPutIO(Y_GOOD_LD_TRAY_MUTE_ON,ON);
			theProcBank.m_bGoodInTrayMuteOn = TRUE; // 2017.2.3 bgkim			

			theLog[LOG_AGV].AddBuf( _T("[%s] stepConnectChk :Send Connetion On - Send Request On - Send Start Off"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > theConfigBank.m_Option.m_AGVWaitTime)
		{
			//�� ���� ���� ���� ��ȣ �ʱ�ȭ 2017/05/02 HSK.
			SendAGVConnection(GOOD_IN_TRAY_PORT,OFF);		
			SendAGVReq(GOOD_IN_TRAY_PORT,OFF);			
			SendAGVStart(GOOD_IN_TRAY_PORT,OFF);

			//2017-08-30,SKCHO,�˶� �߻� �� ABORT ON
			SendAGVAbort(GOOD_IN_TRAY_PORT,ON);

			SetError(ALARM_AGV_READY);
		}
		break;
	case stepTransferChk:		// �ε� ��û ���� Transfer Req Chk �� Conv ���� ���
		m_bRtn[0] = AGVRequestChk(GOOD_IN_TRAY_PORT);	//AGVConnectionChk(GOOD_IN_TRAY_PORT);		//TR-REQ CHK(X_141). JSJUNG. 20170307
		m_bRtn[1] = BottomStoperUpDownChk(GOOD_IN_TRAY_PORT, CYL_UP, TRAY_IN);
		if (IsReturnOk())
		{
			//MuteOn(ON,GOOD_IN_TRAY_PORT);

			SendAGVConnection(GOOD_IN_TRAY_PORT,ON);		
			SendAGVReq(GOOD_IN_TRAY_PORT,ON);		
			SendAGVStart(GOOD_IN_TRAY_PORT,ON);			
			BufConvRun(GOOD_IN_TRAY_PORT,ON);
			
			//2018-04-24
			/*if (theProcBank.m_AlarmID== ALARM_OKTRAY_IN_PORT_ALIGN_EMPTY)
				{
			    	//theProcBank.ResetTrayAlarm(1);  //1�� ALARM_OKTRAY_IN_PORT_ALIGN_EMPTY �˶� ����
					//theProcBank.AlarmClear();
				}
			*/
			//2018-06-11,NAM
			theProcBank.ResetTrayAlarm(0);  //1�� ALARM_OKTRAY_IN_PORT_ALIGN_EMPTY �˶� ����
			
			theProcBank.GoodTrayInLiftingFlag = TRUE; //20170801. JSJUNG

			theLog[LOG_AGV].AddBuf( _T("[%s] stepTransferChk :Send Connetion On - Send Request On - Send Start On"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > AGV_REQUEST_TIMEOUT)//theConfigBank.m_Option.m_AGVWaitTime)
		{
			SendAGVConnection(GOOD_IN_TRAY_PORT,OFF);		
			SendAGVReq(GOOD_IN_TRAY_PORT,OFF);			
			SendAGVStart(GOOD_IN_TRAY_PORT,OFF);

			//2017-08-30,SKCHO,�˶� �߻� �� ABORT ON
			SendAGVAbort(GOOD_IN_TRAY_PORT,ON);

			if(m_bRtn[0] == FALSE)			SetError(ALARM_AGV_TRANSFER_REQ);
			else if(m_bRtn[1] == FALSE)		SetError(ALARM_OKTRAY_IN_LIFT_STOPPER_UP_CHK);
		}
		break;
	case stepBusyChk:		// AGV Busy Chk �� Conv Sen Chk �� Comp On
		m_bRtn[0] = AGVBusyChk(GOOD_IN_TRAY_PORT);		//RUN CHK(X_143)
		if (IsReturnOk())
		{
			m_bRtn[0] = BufConvSenChk(GOOD_IN_TRAY_PORT,SENSOR_ON);
			if (IsReturnOk())
			{
				BufConvRun(GOOD_IN_TRAY_PORT,OFF);
				//Tray ���� ����
				//����� ���� �ϸ� Ʈ���� ������ ���� 2017/04/28 HSK.
				TrayData_Create(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_BUFFER);
				theLog[LOG_AGV].AddBuf( _T("[%s] stepBusyChk"), m_strStateName);

				//2017-07-04,SKCHO, AGV���� TRAY�� ���� �� ���ۿ� ��ġ�Ҷ� ���Դ�� ǥ��
				theProcBank.m_strLoadAGVTC = _T("���Դ��");
								
				//2017-07-17,SKCHO, BUFFER PORT�� Ʈ���̰� ������ Pre-Load����, CIM QUAL��û
				theUnitFunc.CIM_TRAYProcess(_T("351"),_T("4"));
			
				m_Timer.Start();
				nStep++;
			}
			else if (m_Timer.Stop(FALSE) > AGV_CONV_TIMEOUT)//20.)
			{
				SendAGVConnection(GOOD_IN_TRAY_PORT,OFF);		
				SendAGVReq(GOOD_IN_TRAY_PORT,OFF);				
				SendAGVStart(GOOD_IN_TRAY_PORT,OFF);

				//2017-08-30,SKCHO,�˶� �߻� �� ABORT ON
				SendAGVAbort(GOOD_IN_TRAY_PORT,ON);

				SetError(ALARM_AGV_CONV_MOVE);
			}
		}
		else if (m_Timer.Stop(FALSE) > AGV_RUN_TIMEOUT)//theConfigBank.m_Option.m_AGVWaitTime)
		{
			SendAGVConnection(GOOD_IN_TRAY_PORT,OFF);		
			SendAGVReq(GOOD_IN_TRAY_PORT,OFF);				
			SendAGVStart(GOOD_IN_TRAY_PORT,OFF);

			//2017-08-30,SKCHO,�˶� �߻� �� ABORT ON
			SendAGVAbort(GOOD_IN_TRAY_PORT,ON);

			//2018-01-11, JSJUNG, Ʈ���̰� ��� ���ٰ� �߶Ծ��� ����� ���� ��� ���� �����̾�
			//					1�� ������ ���� �ϰ� Ʈ���� ���� ���� �˶�
			if(BuffConv_TrayChk(CONV_SEN_1,SENSOR_ON))
			{
				SetError(ALARM_AGV_TO_CONV_MOVE_NG);
			}
			else
			{
				SetError(ALARM_AGV_BUSY);
			}
			
		}
		break;
	// Conv Stop�� ������ �ٿ� �����ϸ� ������ ���� ������ �� ������ ������ ���� ����. 2017/06/21 HSK.
	case stepStopperDown:
		if(m_Timer.Stop(FALSE) > 0.5)
		{
			BottomStoperUpDown(GOOD_IN_TRAY_PORT, CYL_DOWN,TRAY_IN);
			theLog[LOG_AGV].AddBuf( _T("[%s] stepStopperDown"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		break;
		// �ʿ� ���� Good In �� �ο� ���� ���� �������� ���� �Ѵ� 2017/05/02 HSK.
	case stepEnd:		// AGV Comp Ȯ�� �� AGV ��ȣ All Off
		m_bRtn[1] = BottomStoperUpDownChk(GOOD_IN_TRAY_PORT, CYL_DOWN, TRAY_IN);
		if (IsReturnOk())
		{
			SendAGVConnection(GOOD_IN_TRAY_PORT,OFF);		
			SendAGVReq(GOOD_IN_TRAY_PORT,OFF);				
			SendAGVStart(GOOD_IN_TRAY_PORT,OFF);			

			theUnitFunc.SetOutPutIO(Y_GOOD_LD_TRAY_MUTING_LAMP_ON,OFF);
			theUnitFunc.SetOutPutIO(Y_GOOD_LD_TRAY_REQ_SW,OFF);
			theUnitFunc.SetOutPutIO(Y_GOOD_LD_TRAY_MUTE_ON,OFF);
			theProcBank.m_bGoodInTrayMuteOn = FALSE; // 2017.2.3 bgkim

			// AGV�� Ʈ���� �ٶ��� ���� ���ش� 2017/06/15 HSK.
			TrayInBuzzer(3,OFF);  //GOOD IN

			theProcBank.GoodTrayInLiftingFlag = FALSE; //20170801. JSJUNG

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd :Send Connetion Off - Send Request Off - Send Start Off"), m_strStateName);
			nStep = stepIdle;
		}
		else if (m_Timer.Stop(FALSE) > theConfigBank.m_Option.m_AGVWaitTime)
		{
			SendAGVConnection(GOOD_IN_TRAY_PORT,OFF);		
			SendAGVReq(GOOD_IN_TRAY_PORT,OFF);			
			SendAGVStart(GOOD_IN_TRAY_PORT,OFF);
			SetError(ALARM_OKTRAY_IN_LIFT_STOPPER_CHK);
			nStep = stepIdle;
		}
		break;
	}

	m_nStep = nStep;

	return nStep;
}

int CStateGoodTrayInLift::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;
	BOOL StepChk = FALSE;

	switch(nStep)
	{
	case stepStart:				// Conv1�� Tray Ȯ�� �ϰ� Lift�� Tray���� �����ϰ� down Pos�� Lift Down, Conv Stopper Down.
//		m_bRtn[0] = GetInPutIOCheck(X_GOOD_LD_BUFFER_CONV_TRAY_SEN_4,ON);
//		m_bRtn[1] = GetInPutIOCheck(X_GOOD_LD_CONV_TRAY_SEN_4,OFF);
		
//		if(IsReturnOk())
//		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] step start"), m_strStateName);
			// ��������ǿ��� üũ ���� ���� ����ÿ� �׳� ���� ���� ������. 2017/04/29 HSK.
			BottomStoperUpDown(GOOD_IN_TRAY_PORT,CYL_DOWN,TRAY_IN);
			BottomAlignCylFwdBwd(GOOD_IN_TRAY_PORT,CYL_BWD);
			
			// TR Get���� ������ ����� �־�, �������� ���� ���� ���� �ȳ�����. 2017/06/06 HSK.
			if(GetAxisReadyCheck(AXIS_SVO_GOOD_TRAY_IN_LIFT_Z))
				InTrayLiftZ_DownPos();

			m_Timer.Start();
			nStep++;
			
//		}
//		else
//		{
//			if(m_bRtn[0] == FALSE)			SetError(ALARM_OKTRAY_IN_BUFF_CONV_TRAY_CHK);
//			else if(m_bRtn[1] == FALSE)		SetError(ALARM_OKTRAY_IN_CONV_TRAY_CHK4);
//		}
		break;
	case stepConvMove:				// Lower Align Guide Backward, Lift Down Pos, Stopper Down Ȯ�� �ϰ� Conv Move
		m_bRtn[0] = InTrayLiftZ_DownChk();
		m_bRtn[1] = BottomStoperUpDownChk(GOOD_IN_TRAY_PORT,CYL_DOWN,TRAY_IN);
		m_bRtn[2] = BottomAlignCylFwdBwdChk(GOOD_IN_TRAY_PORT,CYL_BWD);
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepConvMove"), m_strStateName);
			ConvRun(GOOD_IN_TRAY_PORT,ON,CW);
			BufConvRun(GOOD_IN_TRAY_PORT,ON,CW);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 10.)
		{
			if(m_bRtn[0] == FALSE)			SetError(ALARM_GOOD_TRAY_IN_LIFT_Z_DOWN_POS);
			else if(m_bRtn[1] == FALSE)		SetError(ALARM_OKTRAY_IN_LIFT_STOPPER_DOWN_CHK);
			else if(m_bRtn[2] == FALSE)		SetError(ALARM_OKTRAY_LD_LIFT_LOWERGUIDE_BWD_CHK);
		}
		break;
	case stepConvCheck: // Tray ��ġ Ȯ�� �� ������ ������ Ȯ�� �� Conveyor Stop.
		m_bRtn[0] = InConv_TrayChk(CONV_SEN_5,SENSOR_ON);
		m_bRtn[1] = InConv_TrayChk(CONV_SEN_1,SENSOR_ON);
		theProcBank.GoodTrayInLiftingFlag = TRUE;
		if (IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepConvCheck."), m_strStateName);
			BufConvRun(GOOD_IN_TRAY_PORT, OFF);
			ConvRun(GOOD_IN_TRAY_PORT,OFF);

			m_Timer.Start();
			nStep++;
		}
		//2017-11-21,SKCHO, SENSOR5 �̰����� �ϴ� ������� ���ش�.		
		else if(m_Timer.Stop(FALSE) > 15.)
		{
			if(m_bRtn[0] == FALSE)		SetError(ALARM_OKTRAY_IN_CONV_TRAY_CHK5);
			else if(m_bRtn[1] == FALSE)		SetError(ALARM_OKTRAY_IN_CONV_TRAY_CHK1);
		}
		else if(m_Timer.Stop(FALSE) > 6.) 
		{
			BottomAlignCylFwdBwd(GOOD_IN_TRAY_PORT, CYL_BWD);			
		}
		else if(m_Timer.Stop(FALSE) > 5.)  
		{
			BottomAlignCylFwdBwd(GOOD_IN_TRAY_PORT, CYL_FWD);			
		}
		break;

	case stepAlignPos:
		//2017-12-22, step �߰�
		m_bRtn[0] = (m_Timer.Stop(FALSE) > 1.0);
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepInTrayLiftAlignPos"), m_strStateName);
			InTrayLiftZ_AlignPos();
			m_Timer.Start();
			nStep++;
		}		
		break;
	case stepGuideFwd:	
	
		//2017-11-15,SKCHO, Retry ��� �߰�
		if(theProcBank.m_nGoodInPortLAGuidRetryStep == 0)
		{			
			m_bRtn[0] = TRUE;
		}
		else
		{		
			m_bRtn[0] = (m_Timer.Stop(FALSE) > 2.0) ? TRUE : FALSE;  //retry�������� backward ���� �Ϸ� ��� �ð��� ��
		}
		//2017-11-20,SKCHO, PRE BOTTOMALIGNBACKWARD ���� �� BACKWARD SENSOR CHECK�ϰ� ���� ���� �Ѿ
		//m_bRtn[1] = BottomAlignCylFwdBwdChk(GOOD_IN_TRAY_PORT, CYL_BWD);
		//2017-12-21,SKCHO
		m_bRtn[1] = InTrayLiftZ_AlignPosChk();

		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepGuideFwd"), m_strStateName);
			BottomAlignCylFwdBwd(GOOD_IN_TRAY_PORT, CYL_FWD);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[1] == FALSE)		SetError(ALARM_GOOD_TRAY_IN_LIFT_Z_ALIGN_POS);
		}

		break;
	case stepGuideBwd: 
		m_bRtn[0] = BottomAlignCylFwdBwdChk(GOOD_IN_TRAY_PORT, CYL_FWD);
		m_bRtn[1] = (m_Timer.Stop(FALSE) > 2.0) ? TRUE : FALSE;   //2017-11-15,SKCHO, TRAY 30�� -> 40������ ����Ǿ� 1�ʿ��� 2�ʷ� ����
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepGuideBwd"), m_strStateName);
			BottomAlignCylFwdBwd(GOOD_IN_TRAY_PORT, CYL_BWD);
			m_Timer.Start();
			nStep++;

			theProcBank.m_nGoodInPortLAGuidRetryStep = 0;
		}
		else if(m_Timer.Stop(FALSE) > 10.)
		{
		    //2017-11-15,SKCHO, Retry ��� �߰�
			if(theProcBank.m_nGoodInPortLAGuidRetryStep == 0)
			{
				m_Timer.Start();
				//--nStep = stepGuideFwd;
				nStep = stepAlignPos;
				theProcBank.m_nGoodInPortLAGuidRetryStep = 1;
				BottomAlignCylFwdBwd(GOOD_IN_TRAY_PORT, CYL_BWD);
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepGuideFwd - Retry"), m_strStateName);
			}
			else if(theProcBank.m_nGoodInPortLAGuidRetryStep == 1)
			{
				if(m_bRtn[0] == FALSE)		SetError(ALARM_OKTRAY_LD_LIFT_LOWERGUIDE_FWD_CHK);
				theProcBank.m_nGoodInPortLAGuidRetryStep = 0;
			}
			
		}
		break;
	case stepLiftUp:
		m_bRtn[0] = BottomAlignCylFwdBwdChk(GOOD_IN_TRAY_PORT, CYL_BWD);
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepLiftUp"), m_strStateName);
			InTrayLiftZ_UpPos();

			//20171108,JSJUNG, AGV IN IDLE TIME ����
			if(theProcBank.m_bTRAYInIDLE == TRUE)
			{
				::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_IDLEAGVINEND_UPDATE, 0,0); 
				theLog[LOG_LOAD_DELAY_TACT].AddBuf(_T("AGV IN IDLE TIME ����"));
			}

			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)		SetError(ALARM_OKTRAY_LD_LIFT_LOWERGUIDE_BWD_CHK);
		}
		break;
	case stepEnd:	
		m_bRtn[0] = InTrayLiftZ_UpChk();
		if(IsReturnOk())
		{
			TrayData_from_to(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_BUFFER,TRAY_INFO_POS_GOOD_IN_LIFT);
			
			//2017-05-13, JSJUNG, DCR�а� �����ϴ� ��ġ�� ����, CIM QUAL ��û
			//CIM_TRAYProcess(_T("351"),_T("1"));
			//theProcBank.m_bGoodTrayReq = FALSE;
			theProcBank.GoodTrayInLiftingFlag = FALSE;
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"), m_strStateName);
			nStep = stepIdle;			
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			SetError(ALARM_GOOD_TRAY_IN_LIFT_Z_UP_POS);			
		}

		break;
	}

	m_nStep = nStep;

	return nStep;
}


// ��ü������ ��ġ�� �������� ���� �Ͽ� ���� �Ͽ��� 2017/04/29 HSK
int CStateGoodTrayInLiftPitch::Run()
{
	static int nAlignTrayCount = 0;
	int nBuff = 0;
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{
	case stepStart:				// Top Tray ���� Sensor Ȯ�� �� Tray ���� Ȯ�� �ϰ� Division Guide , Align Guide BackWard.
		m_bRtn[0] = LDTrayLiftTrayChk(SENSOR_ON);

		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"), m_strStateName);

			DivisionCylFwdBwd(GOOD_IN_TRAY_PORT, CYL_BWD);
			AlignCylFwdBwd(GOOD_IN_TRAY_PORT, CYL_BWD);
			m_Timer.Start();
			nStep++;
		}
		else
		{
			SetError(ALARM_OKTRAY_IN_LIFT_TRAY_CHK);
		}
		break;
	case stepPitchUp: // Align, Division BWD Ȯ�� �ϰ� Lift Pitch Up.
		m_bRtn[0] = AlignCylFwdBwdChk(GOOD_IN_TRAY_PORT, CYL_BWD);
		m_bRtn[1] = DivisionCylFwdBwdChk(GOOD_IN_TRAY_PORT, CYL_BWD);
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepPitchUp"), m_strStateName);
			InTrayLiftZ_PitchUpRetry();
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)			SetError(ALARM_OKTRAY_IN_ALIGN_BWD_CHK);
			else if(m_bRtn[1] == FALSE)		SetError(ALARM_OKTRAY_IN_DIVISION_BWD_CHK);
		}
		break;
	case stepPitchUpChk:
		m_bRtn[0] = InTrayAlignTrayChk(SENSOR_ON);
		if(IsReturnOk())
		{
			InTrayLiftZ_PitchUpStop();
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepPitchUpChk"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 60.)
		{
			SetError(ALARM_OKTRAY_IN_TOP_TRAY_CHK);
		}
		break;
	case stepDivision:				// Top Tray Sensor ���� �ϸ� Division Guide Forward
		m_bRtn[0] = InTrayAlignTrayChk(SENSOR_ON);
		m_bRtn[1] = InTrayLiftZ_StopChk();

		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepDivision"), m_strStateName);
			DivisionCylFwdBwd(GOOD_IN_TRAY_PORT, CYL_FWD);
			//2018wjp
			if(!theProcBank.m_bModel2FullTray)
			{
				theProcBank.GOODTrayBitX = 1; //2017-03-05,skcho lotend ������ ���ο� Ʈ���̰� ���� �� ��ġ �ʱ�ȭ
				theProcBank.GOODTrayBitY =1;
			}
			else
			{
				TrayData_CreateEx(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_LIFT);
			}
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 3.)
		{
			if(m_bRtn[0] == FALSE)			SetError(ALARM_OKTRAY_IN_TOP_TRAY_CHK);
			else if(m_bRtn[1] == FALSE)		SetError(ALARM_GOOD_TRAY_IN_LIFT_Z);
		}
		break;

	case stepAlignForward:				// Guide Forward Ȯ���ϰ� Lift Pitch Down
		m_bRtn[0] = DivisionCylFwdBwdChk(GOOD_IN_TRAY_PORT, CYL_FWD);
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepAlignForward"), m_strStateName);
			AlignCylFwdBwd(GOOD_IN_TRAY_PORT, CYL_FWD);
			m_Timer.Start();
			nStep++;
		}
		//2017-11-22,SKCHO, 3�� ���� ���� �ȵǸ� BACKWARD�� 2�� ���� �� FORWARD ��Ʈ����
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			DivisionCylFwdBwd(GOOD_IN_TRAY_PORT, CYL_BWD);
		}
		else if(m_Timer.Stop(FALSE) > 7.)
		{
			DivisionCylFwdBwd(GOOD_IN_TRAY_PORT, CYL_FWD);
		}
		else if(m_Timer.Stop(FALSE) > 10.)
		{
			if(m_bRtn[0] == FALSE)		SetError(ALARM_OKTRAY_IN_DIVISION_FWD_CHK);
		}
		break;
	case stepEnd:				// Align Guide Forward�� Tray Sensor Ȯ�� �� End.
		//2017-07-29,JSJUNG, ����� �� ���� ���� OKTRAY_IN_TOP_TRAY_CHK ���� ���̳��� ����
		//m_bRtn[0] = InTrayAlignTrayChk(SENSOR_ON);
		m_bRtn[0] = AlignCylFwdBwdChk(GOOD_IN_TRAY_PORT, CYL_FWD);
	
		if(IsReturnOk())
		{		
			
			//2017-07-04,skcho, DCR �д� ��ġ�� ����,DCR�бⰡ �����ؾ� �� ������ �����ϰ� ��
			//2017-03-21,skcho, align������ Ʈ���� ī��Ʈ ��(Ʈ���� ���̵� ���� ������ ����)
			//TrayData_from_toAlign(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_LIFT,TRAY_INFO_POS_GOOD_IN_ALIGN);
			//theProcBank.m_nGoodAlignCount++;
			//theLog[LOG_SEQUENCE_TRAY].AddBuf(_T("Good Align Count : %d"),theProcBank.m_nGoodAlignCount);
			
			//2017-07-06,SKCHO,  ���� ��û���� DCR �ɼ� ��� ����
			//if (theConfigBank.m_Option.m_bUseDCR)
			//{				
				theProcBank.m_bDCRChk = FALSE;				
			//}

			// 2017.2.11 bgkim
			// ����Ʈ�� Ʈ���̿� �����Ǿ� �־� ��� �Ұ�
			//if (LDTrayLiftTrayChk(SENSOR_OFF) && theConfigBank.m_Option.m_bUseDryRun == FALSE)
			//{
			//	TrayData_RemoveData(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_LIFT);
			//}

			//2018-04-20,SKCHO, ALARM_OKTRAY_IN_PORT_ALIGN_EMPTY�˶� ����
			//2018-06-11,NAM
				//theProcBank.ResetTrayAlarm(1);  //1�� ALARM_OKTRAY_IN_PORT_ALIGN_EMPTY �˶� ����
			theProcBank.ResetTrayAlarm(0);  //1�� ALARM_OKTRAY_IN_PORT_ALIGN_EMPTY �˶� ����

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"), m_strStateName);
			nStep = stepIdle;

			//2018wjp
			if(theProcBank.i_ModelChange2To1==1||theProcBank.m_bInPick2Model)
				theProcBank.m_bInitModelChange=TRUE;

		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			//2017-07-29,JSJUNG, ����� �� ���� ���� OKTRAY_IN_TOP_TRAY_CHK ���� ���̳��� ����
			//if(m_bRtn[0] == FALSE)			SetError(ALARM_OKTRAY_IN_TOP_TRAY_CHK);
			//else if(m_bRtn[1] == FALSE)		SetError(ALARM_OKTRAY_IN_ALIGN_FWD_CHK);
			if(m_bRtn[0] == FALSE)		SetError(ALARM_OKTRAY_IN_ALIGN_FWD_CHK);
		}
		break;
	}

	m_nStep = nStep;

	return nStep;
}



int CStateGoodTrayTRGet::Run()
{

	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{
	case stepStart:				// In tray�� Cell ������ Ȯ�� �� Robot ���� Ȯ�� �ϰ� Tray TR Y Get Pos�� �̵�.
		//2017-09-21,SKCHO, Retry ��� �߰�
		if(theProcBank.m_nGoodTrayRetryStep == 0)
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"), m_strStateName);
		}
		else if(theProcBank.m_nGoodTrayRetryStep == 1)
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart - Retry"), m_strStateName);
		}
		//2018-03-16,GHLEE, RetryStep2 �߰�
		if(theProcBank.m_nGoodTrayRetryStep2 == 0)
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"), m_strStateName);
		}
		else if(theProcBank.m_nGoodTrayRetryStep2 == 1)
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart - Retry2"), m_strStateName);
		}

		theProcBank.m_bOKTrayTRLotEnd = FALSE;

		// Good Out Tray�� Tray�� ���� �� �Ѳ� ���� ���� ���� ���� 17/04/24 HSK
		if(TrayData_CellIsEmpty(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_ALIGN) && TrayData_Exist(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_OUT_LIFT) == FALSE)
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] LotEnd , Good Out Tray Not Exist. Not Assign"), m_strStateName);
			theProcBank.m_bCarrierAssign = TRUE;
			nStep = stepIdle;
			break;
		}
		TrayTrY_InPos();
		m_Timer.Start();
		nStep++;
		break;
	case stepZDown:				// Tray TR Y Get Pos Ȯ�� �� Vac On �� Z Down.
		m_bRtn[0] = TrayTrY_InChk();
		if(IsReturnOk())
		{
			//2017-12-19,KHLEE, �˶� �߻� �� ����Ÿ�� TR�� VAC ON�̸� Z�� �ȳ����� �ٷ� �̵�
			if(TrayTr_VacChk(VAC_ON) == TRUE)
			{
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepZDown - Move to stepZupChk"), m_strStateName);
				m_Timer.Start();
				nStep = stepZUpChk;
			}
			else
			{
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepZDown"), m_strStateName);
				//2017-09-21,SKCHO, Retry ��� �߰�
				/*if(theProcBank.m_nGoodTrayRetryStep == 0)
				{
					TrayTrZ_InPos(0.0);
				}
				//else if (theProcBank.m_nGoodTrayRetryStep == 1)
				else
				{
					TrayTrZ_InPos(theConfigBank.m_Option.m_dGoodTRRetryLength);
				}
				//2018-03-16,GHLEE, RetryStep2 �߰�
				if(theProcBank.m_nGoodTrayRetryStep2 == 0)
				{
					TrayTrZ_InPos(0.0);
				}
				else
				{
					TrayTrZ_InPos(theConfigBank.m_Option.m_dGoodTRRetryLength);
				}*/	
if(theProcBank.m_nGoodTrayRetryStep == 0    && theProcBank.m_nGoodTrayRetryStep2 == 0)
				{
					TrayTrZ_InPos(0.0);
				}
				else if (theProcBank.m_nGoodTrayRetryStep == 1   || theProcBank.m_nGoodTrayRetryStep2 == 1)
				{
					TrayTrZ_InPos(theConfigBank.m_Option.m_dGoodTRRetryLength);
				}
				TrayTr_VacOnOff(VAC_ON);
				m_Timer.Start();
				nStep++;
			}
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)		SetError(ALARM_OKTRAY_TR_Y_GET_POS);
		}
		break;
	case stepGuideBack:			// Vac On �� Z Down Ȯ�� �� Align , Division Guide Back Ward.
		m_bRtn[0] = TrayTr_VacChk(VAC_ON);
		//2017-09-21,SKCHO, Retry ��� �߰�
		if(theProcBank.m_nGoodTrayRetryStep == 0
			&& theProcBank.m_nGoodTrayRetryStep2 == 0)
		{
			m_bRtn[1] = TrayTrZ_InChk(0.0);
		}
		else if (theProcBank.m_nGoodTrayRetryStep == 1
					|| theProcBank.m_nGoodTrayRetryStep2 == 1)
		{
			m_bRtn[1] = TrayTrZ_InChk(theConfigBank.m_Option.m_dGoodTRRetryLength);
		}
		m_bRtn[2] = (m_Timer.Stop(FALSE) > 0.5 ) ? TRUE : FALSE;
		if(IsReturnOk())
		{
			if (theConfigBank.m_Option.m_bUseTrayAlign)
			{
				if (TrayData_Exist(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_OUT_LIFT))
					theProcBank.m_bGoodTrayAlignChk = FALSE;
			}

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepGuideBack"), m_strStateName);
			AlignCylFwdBwd(GOOD_IN_TRAY_PORT, CYL_BWD);
			
			//2017-07-20,SKCHO,����� �ɼ� ��� ����
			if(!theConfigBank.m_Option.m_bDivision)
			{
				DivisionCylFwdBwd(GOOD_IN_TRAY_PORT, CYL_BWD);
			}

			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			//vac ���� ��
			if(m_bRtn[0] == FALSE)			
			{
				//2018-03-16,GHLEE, �߰�
				//m_dGoodTRRetryLength �ɼ� 0�� ���� �˶� �߻�
				if(theConfigBank.m_Option.m_dGoodTRRetryLength == 0)
				{
					SetError(ALARM_OKTRAY_TR_VAC);
				}
				//m_dGoodTRRetryLength �ɼ� �������� ��
				else
				{
					//vac ���� �� RetryStep �Ѵ� 0 �� �� RetryStep2�� 1�� �ٽ� ����
					if(theProcBank.m_nGoodTrayRetryStep == 0 
						&& theProcBank.m_nGoodTrayRetryStep2 == 0)
					{
						m_Timer.Start();
						nStep = stepStart;
						theProcBank.m_nGoodTrayRetryStep2 = 1;

						//2017-12-01,SKCHO
						theProductBank.m_OKTR_RetryInfo.nTotal++;
					}
					//RetryStep�� 1�ӿ��� vac�� ������ �� �˶� �߻� �� 0���� �ʱ�ȭ
					else if(theProcBank.m_nGoodTrayRetryStep2 == 1)
					{
						SetError(ALARM_OKTRAY_TR_VAC);
						theProcBank.m_nGoodTrayRetryStep2 = 0;
					}
				}
				
			}
			else if(m_bRtn[1] == FALSE)		SetError(ALARM_OKTRAY_TR_Z_GET_POS);
		}
		break;
	case stepGuideBackChk:
		m_bRtn[0] = AlignCylFwdBwdChk(GOOD_IN_TRAY_PORT, CYL_BWD);
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepGuideBackChk"), m_strStateName);
			TrayTrZ_UpPos(0.5);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			SetError(ALARM_OKTRAY_IN_ALIGN_BWD_CHK);
		}
		break;
	case stepZUpChk:				// Guide Backward Ȯ�� �� Z Up.
		m_bRtn[0] = TrayTrZ_UpChk();
		m_bRtn[1] = TrayTr_VacChk(VAC_ON);
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepZUpChk"), m_strStateName);
			
			theProcBank.m_bCarrierAssign = TrayData_CellIsEmpty(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_ALIGN);
					
			if(theProcBank.m_bCarrierAssign == TRUE)
			{	
				theProcBank.m_bCarrierReportWaitCheck = TRUE;  //������ �Ϸ� �Ǳ�����   GoodAlignCount = 0 �̵Ǿ� release���� �Ǵ� ���� ����
			}	

			//2018-03-14,GHLEE
		    if(theConfigBank.m_Option.m_bGoodInFinalSen == FALSE)
			{			
				//2017-07-17,SKCHO, ����Ʈ�� Ʈ���̰� �������� 302���� �Ѵ�.
				if (LDTrayLiftTrayChk(SENSOR_OFF))
				{
					theLog[LOG_SEQUENCE_TRAY].AddBuf(_T("[%s] CIM_CarrierCellLoadEnd Cell Cnt : %d"), m_strStateName,theProcBank.m_nLotCellCnt );
					theUnitFunc.CIM_CarrierCellLoadEnd();				
				}
			}
			else
			{
				//last sensor ����
				if (InTrayLiftZ_FinalSenChk(SENSOR_OFF))
				{
					theLog[LOG_SEQUENCE_TRAY].AddBuf(_T("[%s] CIM_CarrierCellLoadEnd Cell Cnt : %d"), m_strStateName,theProcBank.m_nLotCellCnt );
					theUnitFunc.CIM_CarrierCellLoadEnd();				
				}

			}

			theProcBank.m_strS3F115_ReplyCode = _T("");

			// Assign�� �׳� ������ ���ش�. 2017/05/04 HSK.
			//2018wjp
			if(!theProcBank.m_bModel2FullTray)
				CIM_CarrierAssign();

			TrayData_from_to(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_ALIGN,TRAY_INFO_POS_GOOD_TR);
			//2018wjp
			CellData_Tray_ClearAll();
			::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_REMOVE_ALL_CELL, 0,0); 
			//20171108,JSJUNG, AGV OUT IDLE TIME ����
			//2018-01-11, JSJUNG, GOOD_OUT_TRAY_PORT ������ GOOD_IN_TRAY_PORT�� ����
			if(theProcBank.m_bTRAYOutIDLE == FALSE
				&& theProcBank.m_bCarrierAssign == TRUE
				&& TrayData_Exist(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_OUT_BUFFER) == TRUE
				&& TrayData_Exist(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_OUT_LIFT) == TRUE)
			{
				::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_IDLEAGVOUTSTART_UPDATE, 0,0); 
				theLog[LOG_LOAD_DELAY_TACT].AddBuf(_T("AGV OUT IDLE TIME ����"));


				//2018-04-20,SKCHO, AGV OUT IDEL TIME�����Ҷ� �˶� �߻�
				theUnitFunc.SetError(ALARM_OKTRAY_OUT_BUFF_PORT_FULL);
			}

			//����� �ɼ� ��� 2017-07-20,SKCHO
			if(theConfigBank.m_Option.m_bDivision)
			{
				DivisionCylFwdBwd(GOOD_IN_TRAY_PORT, CYL_BWD);
			}

			// IOCheckTHR���� ���� �ϴ� �� �ȱ�. 2017/05/02 HSK
			////2018-03-14,GHLEE
		    if(theConfigBank.m_Option.m_bGoodInFinalSen == FALSE)
			{
				if (LDTrayLiftTrayChk(SENSOR_OFF)
						&& theConfigBank.m_Option.m_bUseDryRun == FALSE
						&& TrayData_Exist(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_LIFT))			
				{
					TrayData_RemoveData(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_LIFT);
					InTrayLiftZ_DownPos();		
					//2018-03-10,MGYUN, LIFT DOWN�� PORT2 EMPTY, NONE  
					theMonitorBank.WritePortStatus(2,0,0);
				}
			}
			
			//2017-12-01,SKCHO
			if(theProcBank.m_nGoodTrayRetryStep == 1)
			{
				theProductBank.m_OKTR_RetryInfo.nSucess++;
			}
			theProcBank.m_nGoodTrayRetryStep  = 0;

			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)				SetError(ALARM_OKTRAY_TR_Z_UP_POS);
			else if(m_bRtn[1] == FALSE)	
			{
				//2017-09-21,SKCHO, LENGTH= 0�̸� ��ɻ�����
				if(theConfigBank.m_Option.m_dGoodTRRetryLength == 0)
				{
					SetError(ALARM_OKTRAY_TR_VAC);
				}
				else
				{
					//2017-09-21,SKCHO, Retry ��� �߰�
					if(theProcBank.m_nGoodTrayRetryStep == 0)
					{
						m_Timer.Start();
						nStep = stepStart;
						theProcBank.m_nGoodTrayRetryStep = 1;
						DivisionCylFwdBwd(GOOD_IN_TRAY_PORT, CYL_FWD);

						//2017-12-01,SKCHO
						theProductBank.m_OKTR_RetryInfo.nTotal++;

					}
					else if(theProcBank.m_nGoodTrayRetryStep == 1)
					{
						SetError(ALARM_OKTRAY_TR_VAC);
						theProcBank.m_nGoodTrayRetryStep = 0;

					}
				}
			}
		
		}
		break;
	case stepEnd:
		// Assign ���� ���� ���� �߰� 2017/04/27 HSK.
		if(theConfigBank.m_Option.m_bUseCarrierReportSkip == FALSE)
			m_bRtn[0] = CIM_CarrierReportCheck();
		else
			m_bRtn[0] = TRUE;

		if(TrayData_Exist(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_LIFT) == FALSE)
		{
			m_bRtn[1] = InTrayLiftZ_DownChk();
			if(m_bRtn[1] == TRUE)
			{
				//2017-07-07,SKCHO, GOODLIFT ��Ʈ�� TRAY�� ���� ��� 262���� ���ؼ� ī��Ʈ �ʱ�ȭ
				theProcBank.ResetGoodAlignCount();
				
			}
		}
		else
		{
			m_bRtn[1] = TRUE;
		}

		if(IsReturnOk())
		{
			//2017-05-15,skcho, ��������� pass�̳� ��ŵ�ɼ��̸� ���
			if(theProcBank.m_strS3F115_ReplyCode == _T("PASS") || theConfigBank.m_Option.m_bUseCarrierReportSkip == TRUE)
			{		
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"), m_strStateName);			
			}
			else
			{	
				//��������� pass�� �ƴϸ� ��˶�
				theProcBank.m_bLightAlarmChk = TRUE;
				//Carrier Report �˶� ����ȭ 2017/06/22 HSK.
				SetError(ALARM_OKTRAY_ID_ASSIGN_REPORT_FAIL);
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] step Carrier Report Check Fail"), m_strStateName);
			}

			//2017-05-23,skcho, in/buffer�� Ʈ���̰� ���� �˶� �︲
			if(TrayData_Exist(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_IN_BUFFER) == FALSE
				&& TrayData_Exist(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_LIFT) == FALSE)
			{
				//2017-09-12, SKCHO,main timer���� �ϹǷ� ����
				//TrayInBuzzer(2,ON);  //GOOD IN		
				theProcBank.m_nGoodInPortEmptyStep  =1;  //2017-09-12,SKCHO, GOOD PORT �� �˸�
			}	

			//2017-08-30,SKCHO, TR GET��  TRAY OUT PORT���¸� üũ�ϴ°� �̻��ؼ� ����
			//2017-08-22 KAKAKA OUT/buffer�� Ʈ���� �����غ� �˶� �︲
			/*if(TrayData_Exist(GOOD_OUT_TRAY_PORT, TRAY_INFO_POS_GOOD_OUT_BUFFER) == TRUE
				&& TrayData_Exist(GOOD_OUT_TRAY_PORT,TRAY_INFO_POS_GOOD_OUT_LIFT) == TRUE)
			{
				TrayInDelayBuzzer(4,ON);  //GOOD OUT				
			}*/	
			
			//2017-10-13,SKCHO ���� �ʱ�ȭ
			theProcBank.m_nGoodTrayRetryStep = 0;
			theProcBank.m_nGoodTrayRetryStep2 = 0;
			nStep = stepIdle;
		}
		else if(m_Timer.Stop(FALSE) > theConfigBank.m_Option.m_dCarrierInfoWaitTime)
		{
			if(m_bRtn[0] == FALSE)
			{
				//2017-10-09,SKCHO, CARRIER RESULT FILE READ
				if(theUnitFunc.ReadCarrierResult())
				{
					nStep = stepEnd;
					m_Timer.Start();
				}
				else
				{
					theProcBank.m_bLightAlarmChk = TRUE;
					SetError(ALARM_OKTRAY_ID_ASSIGN_REPORT_TIMEOUT);
					theLog[LOG_SEQUENCE].AddBuf( _T("[%s] step Carrier Assign Report Check TimeOut"), m_strStateName);
					//����� ������ ���� ��˶��̸� ����� ����. 2017/06/17 HSK.
					nStep = stepIdle;
				}
			}
			else	
			{
				SetError(ALARM_GOOD_TRAY_IN_LIFT_Z_DOWN_POS);
			}
		}
		break;
	}

	m_nStep = nStep;

	return nStep;
}


int CStateGoodTrayTRPut::Run()
{
	
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{
	case stepStart:				  // TR�� Tray ���� Ȯ�� �� Y �� Out Port �̵� Ȯ�� �� �ƴ� ��� �̵�.
		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"), m_strStateName);
		TrayTrY_OutPos();
		m_Timer.Start();
		nStep++;
		break;
	case stepZDown:     // Y�� Out Ȯ�� �� Z Down.
		m_bRtn[0] = TrayTrY_OutChk();
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepZDown"), m_strStateName);
			TrayTrZ_OutPos();
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)	SetError(ALARM_OKTRAY_TR_Y_PUT_POS);
		}
		break;		
	case stepVacOff:				// Z down Ȯ�� ��  Vac Off
		m_bRtn[0] = TrayTrZ_OutChk();
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepVacOff"), m_strStateName);
			TrayTr_VacOnOff(VAC_OFF);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)		SetError(ALARM_OKTRAY_TR_Z_PUT_POS);
		}
		break;
	case stepZUp:					// Vac Off Ȯ�� �� Z Up.
		m_bRtn[0] = TrayTr_VacChk(VAC_OFF);
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepZUp"), m_strStateName);
			TrayTrZ_UpPos();
			//TrayTr_BlowOff();

			//Ʈ���� TR�� ���� ���� Ʈ���̸� ���� �ִ� ���� �Ѳ��ۿ� ����. 2017/04/30 HSK.
			if(TrayData_CellIsEmpty(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_TR))
				theProcBank.m_bTrayCellFullChk = TRUE;
			else
				theProcBank.m_bTrayCellFullChk = FALSE;

			if(theProcBank.m_TrayBank.GetTrayInfo(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_OUT_LIFT)->IsExist())
			{
				//--theProcBank.m_TrayBank.Copy_CarrierInfo(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_TR,TRAY_INFO_POS_GOOD_OUT_LIFT);
				TrayData_RemoveData(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_TR);
			}
			else
			{
				//2017-05-15,skcho , �ʿ���� ���� cim qual
				//--CIM_TRAYProcess(_T("351"),_T("2"));
				TrayData_from_to(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_TR,TRAY_INFO_POS_GOOD_OUT_LIFT);
			}		

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepOutZDown"), m_strStateName);
			theProcBank.m_TrayBank.GetTrayInfo(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_OUT_LIFT)->m_nTrayCount++;

			theProcBank.m_nGoodOutLiftCount++;


			//TR�� ���� �ִ� Ʈ���̰� ����ִ� ��쿡 ������ �������� ����. 2017/05/04 HSK.
			if(theProcBank.m_bTrayCellFullChk)
			{
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] Tray Count : %d , Out Lift Full"), m_strStateName,theProcBank.GetGoodOutLiftCount());
				theProcBank.m_TrayBank.GetTrayInfo(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_OUT_LIFT)->SetTrayFull(TRUE);
				theProcBank.m_bTrayCellFullChk = FALSE;
			}

			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)		SetError(ALARM_OKTRAY_TR_VAC);
		}
		break;
	case stepOutZDown:
		m_bRtn[0] = TrayTrZ_UpChk();
		if (IsReturnOk())
		{
			
			// ��ο� ���� ������ TR�� �ö�°� üũ�ϵ��� ���� 2017/04/27 HSK
			TrayTr_BlowOff();
			OutTrayLiftZ_PitchDown(1); //1->1.5�κ���
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)		SetError(ALARM_OKTRAY_TR_Z_UP_POS);
		}
		break;
	case stepEnd:
		m_bRtn[0] = OutTrayLiftZ_StopChk();
		m_bRtn[1] = TrayTr_VacChk(VAC_OFF); // ��ο츦 �Ҿ Ƽ�˿� Ʈ���̰� �������� ������ �־ �������� �ѹ��� üũ 2017/04/27 HSK
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"), m_strStateName);
			nStep = stepIdle;

			if(theConfigBank.m_Option.m_bUseLotEnd
				&& TrayData_CellIsFull(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_ALIGN) == FALSE)
				theProcBank.m_bOKTrayTRLotEnd = TRUE; // 2017.2.6 bgkim


		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)			SetError(ALARM_GOOD_TRAY_OUT_LIFT_Z_PITCH_DOWN_POS);
			else if(m_bRtn[1] == FALSE)		SetError(ALARM_OKTRAY_TR_VAC);
		}
		break;
	}

	m_nStep = nStep;

	return nStep;
}

int CStateGoodTrayDCR::Run()
{
	int nBuff = 0;

	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{
	case stepStart:
		// DCR FAIL�ÿ� �������� Ʈ���� ���� �� ��� Ʈ���� �ø��� �������߰�. 17/04/24 HSK
		m_bRtn[0] = InTrayAlignTrayChk(SENSOR_ON);
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"), m_strStateName);		
			nStep = stepDCRTriggerON;
		}
		else
		{
			AlignCylFwdBwd(GOOD_IN_TRAY_PORT, CYL_BWD);
			DivisionCylFwdBwd(GOOD_IN_TRAY_PORT, CYL_BWD);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart -> Not Sensing Tray"), m_strStateName);
			nStep++;
		}
		m_Timer.Start();
		break;

	case stepLiftUp:
		m_bRtn[0] = AlignCylFwdBwdChk(GOOD_IN_TRAY_PORT, CYL_BWD);
		m_bRtn[1] = DivisionCylFwdBwdChk(GOOD_IN_TRAY_PORT, CYL_BWD);

		if(IsReturnOk())
		{
			InTrayLiftZ_PitchUpRetry();
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepLiftUp"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			///2017-11-22,YMG, �˶����� NG�� �߸� �Ǿ� �־� ����.			
			if(m_bRtn[0] == FALSE)		SetError(ALARM_OKTRAY_IN_ALIGN_BWD_CHK); //SetError(ALARM_NGTRAY_IN_ALIGN_BWD_CHK);
			else						SetError(ALARM_OKTRAY_IN_DIVISION_BWD_CHK); //SetError(ALARM_NGTRAY_IN_DIVISION_BWD_CHK);
		}
		break;
	case stepLiftStop:
		m_bRtn[0] = InTrayAlignTrayChk(SENSOR_ON);
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepLiftStop"), m_strStateName);
			InTrayLiftZ_PitchUpStop();
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 10.)
		{
			SetError(ALARM_OKTRAY_IN_TOP_TRAY_CHK);
		}
		break;
	case stepCylFwd:
		m_bRtn[0] = InTrayLiftZ_StopChk();
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCylFwd"), m_strStateName);
			AlignCylFwdBwd(GOOD_IN_TRAY_PORT, CYL_FWD);
			DivisionCylFwdBwd(GOOD_IN_TRAY_PORT, CYL_FWD);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 10.)
		{
			SetError(ALARM_GOOD_TRAY_IN_LIFT_Z);
		}
		break;
	case stepDCRTriggerON:
		m_bRtn[0] = AlignCylFwdBwdChk(GOOD_IN_TRAY_PORT, CYL_FWD);
		m_bRtn[1] = DivisionCylFwdBwdChk(GOOD_IN_TRAY_PORT, CYL_FWD);
		if(IsReturnOk() && m_Timer.Stop(FALSE) > 0.5) //2017.10.14, JSJUNG, ����� ���̵� FWD�� DCR �������� 0.5�� ������
		{
			DCRTrigger(ON);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepDCRTriggerON"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 3.)
		{
			//2017-11-22,YMG, �˶����� NG�� �߸� �Ǿ� �־� ����.							
			if(m_bRtn[0] == FALSE)		SetError(ALARM_OKTRAY_IN_ALIGN_FWD_CHK);   //SetError(ALARM_NGTRAY_IN_ALIGN_FWD_CHK);
			else						SetError(ALARM_OKTRAY_IN_DIVISION_FWD_CHK); //SetError(ALARM_NGTRAY_IN_DIVISION_FWD_CHK);
		}
		break;
	case stepIDReadCheck:
		m_bRtn[0] = DCRChk();
		if (IsReturnOk())
		{
			DCRTrigger(OFF);
			//2017-07-04,SKCHO, DCR �б� ���� �� Ʈ���� ���� ����, ī��Ʈ ����
			//2017-03-21,skcho, align������ Ʈ���� ī��Ʈ ��(Ʈ���� ���̵� ���� ������ ����)
			TrayData_from_toAlign(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_LIFT,TRAY_INFO_POS_GOOD_IN_ALIGN);
			if(!theProcBank.m_bModel2FullTray)
			{
				theProcBank.m_nGoodAlignCount++;
				//2017-07-07,SKCHO
				theProcBank.m_nSubGoodAlignCount++;
			}
			else
				theProcBank.m_bModel2FullTray=FALSE;
			theLog[LOG_SEQUENCE_TRAY].AddBuf(_T("Good Align Count : %d"),theProcBank.m_nGoodAlignCount);

			::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_DCR_UPDATE, 0,0);  //2017-02-28, skcho, DCR ���� ���� ȭ�鿡 ǥ��

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepIDReadCheck"), m_strStateName);
			m_Timer.Start();

			//2018-03-14,GHLEE, LastTraySen�� ���� �������� LastTraySen �ɼǰ� Division �ɼ��� �����־�� �ȴ�.
			if(InTrayLiftZ_FinalSenChk(SENSOR_OFF)
				&& TrayData_Exist(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_LIFT)
				&& theConfigBank.m_Option.m_bGoodInFinalSen == TRUE)
			{
				TrayData_RemoveData(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_LIFT);
				InTrayLiftZ_DownPos();
				//2018-03-10,MGYUN, LIFT DOWN�� PORT2 EMPTY, NONE  
				theMonitorBank.WritePortStatus(2,0,0);
			}
			
			//2017.10.26 JSJUNG. Ʈ���� ���� ��� ���� �����̵�
			if(theConfigBank.m_Option.m_bUseTrayVI)
			{
				theDeviceMelsec.RobotBitOff(UDROBOT_X_TRAYVING_Y_OKNGRTN);
				theProcBank.m_nTrayVICount = 0;

				theProcBank.m_bTrayVIResultRetry = FALSE;//2017-12-09,SKCHO
				theProcBank.m_bTrayVICompRetry = FALSE;

				nStep++;
			}
			else
			{
				nStep = stepIDReport;
			}
			//nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 3.)
		{
			::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_DCR_UPDATE, 0,0);  //2017-02-28, skcho, DCR ���� ���� ȭ�鿡 ǥ��
			SetError(ALARM_OKTRAY_DCR_FAIL);
			DCRTrigger(OFF);
		}
		break;

	
		
	//2017.10.26 JSJUNG. Ʈ���� ���� �߰�
	case stepTrayVIReadyChk:
		//2017.10.26 JSJUNG. �κ�/���� ���� ��ȣ Ȯ��
		if(!theConfigBank.m_Option.m_bUseTrayVI)
		{
			nStep++;
			break;
		}

		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_TRAYVIREADY_Y_TRAYVIUSE);
		if (IsReturnOk())
		{
			theProcBank.m_nTrayVICount++;
			//2017.10.26 JSJUNG. �Ի��ȣ ��
			theDeviceMelsec.RobotBitOn(UDROBOT_X_TRAYVIOK_Y_TRAYVISTART);

			//2018-01-22,JSJUNG, �κ� TO VISION ��� ��Ʈ���� ��ȣ OFF
			theDeviceMelsec.RobotBitOff(UDROBOT_X_RBTOVIRETRY_Y_TRAYVISTARTCOMP);

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepTrayVIReadyChk"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		//2018-01-22,JSJUNG, �������α׷� �κ� ��� ���涧 ��Ʈ���� �߰��Ǿ� ��� 10�ʷ� ����(����5��)
		else if(m_Timer.Stop(FALSE) > 10.)
		{
			//2018-01-22,JSJUNG, �κ� TO VISION ��� ��Ʈ���� ��ȣ OFF
			theDeviceMelsec.RobotBitOff(UDROBOT_X_RBTOVIRETRY_Y_TRAYVISTARTCOMP);

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepTrayVIReadyChk-Robot Not Ready"), m_strStateName);			
			SetError(ALARM_OKTRAY_VI_NOT_READY);
		}
		break;

	case stepTrayVIStartComp:
		if(!theConfigBank.m_Option.m_bUseTrayVI)
		{
			nStep++;
			break;
		}

		theProcBank.m_bTrayVINGRetry = FALSE;

		//2017-12-09,SKCHO else if �������� �̵�
		//theDeviceMelsec.RobotBitOn(UDROBOT_X_TRAYVIOK_Y_TRAYVISTART);
		
		//2017.10.26 JSJUNG. �κ�/���� �Ի� �Ϸ� ��ȣ Ȯ��
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_RBTOVIRETRY_Y_TRAYVISTARTCOMP);
		if (IsReturnOk() && m_Timer.Stop(FALSE) > 0.1)
		{
			//2017.10.26 JSJUNG. �Ի��ȣ ����
			theDeviceMelsec.RobotBitOff(UDROBOT_X_TRAYVIOK_Y_TRAYVISTART);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepTrayVIStartComp"), m_strStateName);
			theProcBank.m_bTrayVICompRetry = FALSE;
			m_Timer.Start();
			nStep++;
		}		
		else if(m_Timer.Stop(FALSE) > 10.)
		{
			//2017-12-09,SKCHL �߰�
			//--theDeviceMelsec.RobotBitOff(UDROBOT_X_TRAYVING_Y_OKNGRTN);

			theDeviceMelsec.RobotBitOff(UDROBOT_X_TRAYVIOK_Y_TRAYVISTART);
			//2017-12-09,SKCHO, VI COMP RETRY
			/*if(theProcBank.m_bTrayVICompRetry == FALSE)
			{
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepTrayVIStartComp-Retry"), m_strStateName);	
				theProcBank.m_bTrayVICompRetry = TRUE;
				nStep = stepTrayVIReadyChk;
				m_Timer.Start();
			}
			else
			{
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepTrayVIStartComp-Fail"), m_strStateName);
				theProcBank.m_bTrayVICompRetry = FALSE;
				SetError(ALARM_OKTRAY_VI_NOT_COMP);
			}
			*/
			SetError(ALARM_OKTRAY_VI_NOT_COMP);
		}
		//2017-12-09,SKCHO, 1�ʾȿ� ���޾��� ��� �ٽ� ��Ʈ��
		else if(m_Timer.Stop(FALSE) > 0.3)
		{
			theDeviceMelsec.RobotBitOn(UDROBOT_X_TRAYVIOK_Y_TRAYVISTART);
		}
		
		
		break;

	case stepTrayVIResult:
		if(!theConfigBank.m_Option.m_bUseTrayVI)
		{
			nStep++;
			break;
		}
		//2017-12-09,SKCHO else if �������� �̵�
		//theDeviceMelsec.RobotBitOff(UDROBOT_X_TRAYVIOK_Y_TRAYVISTART);

		////2017.10.26 JSJUNG. �κ�/���� �Ի� ��� ��ȣ Ȯ��
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_TRAYVIOK_Y_TRAYVISTART);
		m_bRtn[1] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_TRAYVING_Y_OKNGRTN);
		if (m_bRtn[0]== TRUE && m_bRtn[1] == FALSE && m_Timer.Stop(FALSE) > 0.1)
		{
			//2017.10.26 JSJUNG. ��� ��ȣ Return ��
			theDeviceMelsec.RobotBitOn(UDROBOT_X_TRAYVING_Y_OKNGRTN);
			theProcBank.m_nTrayVICount = 0;
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepTrayVIResult-OK"), m_strStateName);

			theProcBank.m_bTrayVIResultRetry = FALSE;
			m_Timer.Start();
			nStep++;
		}
		else if (m_bRtn[0]== FALSE && m_bRtn[1] == TRUE && m_Timer.Stop(FALSE) > 0.1)
		{
			//2017.10.26 JSJUNG. ��� ��ȣ Return ��
			theDeviceMelsec.RobotBitOn(UDROBOT_X_TRAYVING_Y_OKNGRTN);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepTrayVIResult-NG"), m_strStateName);
			
			if(theProcBank.m_nTrayVICount > 5)
			{
				theProcBank.m_nTrayVICount = 0;
				
				//2017.10.26 JSJUNG. ��� ��ȣ Return ����
				//theDeviceMelsec.RobotBitOff(UDROBOT_X_TRAYVING_Y_OKNGRTN);
				SetError(ALARM_OKTRAY_VI_5COUNT_NG);
			}
			else
			{
				//2017.10.26 JSJUNG. ��� ��ȣ Return ����
				//theDeviceMelsec.RobotBitOff(UDROBOT_X_TRAYVING_Y_OKNGRTN);
				theProcBank.m_bTrayVINGRetry = TRUE;
				
				theProcBank.m_bTrayVIResultRetry = FALSE;

				m_Timer.Start();
				nStep++;
			}
		}
		//2018-01-22,JSJUNG, �������α׷� �κ� ��� ���涧 ��Ʈ���� �߰� �κ��� ��ȣ�̻� �ñ׳� ���
		else if(m_Timer.Stop(FALSE) > 3. && theProcBank.m_bTrayVIToRBRetry == FALSE)
		{
			theDeviceMelsec.RobotBitOn(UDROBOT_X_RBTOVIRETRY_Y_TRAYVISTARTCOMP);
			theProcBank.m_bTrayVIToRBRetry = TRUE;
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepTrayVIResult-Robot to Tray VI RETRY"), m_strStateName);
			m_Timer.Start();
			nStep = stepTrayVIReadyChk;

		}
		else if(m_Timer.Stop(FALSE) > 10.)
		{				
			
			theDeviceMelsec.RobotBitOff(UDROBOT_X_TRAYVING_Y_OKNGRTN);

			//2018-01-22,JSJUNG, �κ� TO VISION ��� ��Ʈ���� ��ȣ OFF
			theDeviceMelsec.RobotBitOff(UDROBOT_X_RBTOVIRETRY_Y_TRAYVISTARTCOMP);
			//2017-12-09,SKCHO, VIRESULT RETRY
			/*if(theProcBank.m_bTrayVIResultRetry == FALSE)
			{
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepTrayVIResult-Retry"), m_strStateName);	
				theProcBank.m_bTrayVIResultRetry = TRUE;
				nStep = stepTrayVIReadyChk;
				m_Timer.Start();
			}
			else
			{
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepTrayVIResult-Fail"), m_strStateName);	
				SetError(ALARM_OKTRAY_VI_NOT_RESULT);
				theProcBank.m_bTrayVIResultRetry = FALSE;
			}*/
			SetError(ALARM_OKTRAY_VI_NOT_RESULT);
		}
		//2017-12-09,SKCHO, 1�ʾȿ� ���޾��� ��� �ٽ� ��Ʈoff
		else if(m_Timer.Stop(FALSE) > 0.3)
		{
			theDeviceMelsec.RobotBitOff(UDROBOT_X_TRAYVIOK_Y_TRAYVISTART);
		}
		
		break;

	case stepTrayVIResultOK:
		if(!theConfigBank.m_Option.m_bUseTrayVI)
		{
			nStep++;
			break;
		}
		
		//2017-12-09,SKCHO else if �������� �̵�
		//theDeviceMelsec.RobotBitOn(UDROBOT_X_TRAYVING_Y_OKNGRTN);
		m_bRtn[0] = !(BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_TRAYVIOK_Y_TRAYVISTART);
		m_bRtn[1] = !(BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_TRAYVING_Y_OKNGRTN);

		if (m_bRtn[0]== TRUE && m_bRtn[1] == TRUE && m_Timer.Stop(FALSE) > 0.3)
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepTrayVIResultOK"), m_strStateName);
			theDeviceMelsec.RobotBitOff(UDROBOT_X_TRAYVING_Y_OKNGRTN);
			m_Timer.Start();
			if(theProcBank.m_bTrayVINGRetry == TRUE)
			{
				theProcBank.m_bTrayVINGRetry = FALSE;
				nStep = stepTrayVIReadyChk;
			}
			else
			{
				nStep++;
			}
			
		}		
		else if(m_Timer.Stop(FALSE) > 10.)
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepTrayVIResult-Fail"), m_strStateName);
			
			SetError(ALARM_OKTRAY_VI_NOT_RESULT);
		}
		//2017-12-09,SKCHO, 1�ʾȿ� ���޾��� ��� �ٽ� ��Ʈoff
		else if(m_Timer.Stop(FALSE) > 0.3)
		{
			theDeviceMelsec.RobotBitOn(UDROBOT_X_TRAYVING_Y_OKNGRTN);
		}
		
		break;

	case stepIDReport:

		//theDeviceMelsec.RobotBitOn(UDROBOT_X_TRAYVING_Y_OKNGRTN);
		// ReplyCode�� Fail�� ��� ��� �Ͽ� �Ʒ��� �̵� 17/04/24 HSK
//		theProcBank.m_bDCRChk = TRUE;
		theProcBank.m_strS3F115_ReplyCode = _T("");
		//2017-03-21,skcho, DCR�а� ���� ���� ���� ó�� �Ѳ��� ��츸 ������.
		if(CIM_CarrierRelease() == FALSE)
		{
			theProcBank.m_bDCRChk = TRUE;
			//2018-01-22,JSJUNG, �κ� TO VISION ��� ��Ʈ���� FLAG �ʱ�ȭ
			theProcBank.m_bTrayVIToRBRetry = FALSE;
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"), m_strStateName);
			nStep = stepIdle;
		}
		else
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] step Carrier Report"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		break;		
		
	case stepEnd:				// DCR Reading Ȯ���� End.	

		//2017-12-09,SKCHO else if �������� �̵�
		//theDeviceMelsec.RobotBitOff(UDROBOT_X_TRAYVING_Y_OKNGRTN);
		if(theConfigBank.m_Option.m_bUseCarrierReportSkip == FALSE)
			m_bRtn[0] = CIM_CarrierReportCheck();
		else
			m_bRtn[0] = TRUE;

		// ������ ���� �� ��쿡�� ���� ��˶��� �ƴ� �߾˶����� ����. 2017/06/22 HSK.
		if(IsReturnOk()&& m_Timer.Stop(FALSE) > 0.1)
		{
			//2017.10.26 JSJUNG. ��� ��ȣ Return ����
			theDeviceMelsec.RobotBitOff(UDROBOT_X_TRAYVING_Y_OKNGRTN);

			//2017-05-15,sckho ,���� ��� pass�� fail ����
			if(theProcBank.m_strS3F115_ReplyCode == _T("PASS") || theConfigBank.m_Option.m_bUseCarrierReportSkip == TRUE)
			{		
				theProcBank.m_bDCRChk = TRUE;
				theProcBank.m_bGoodTrayReq = FALSE;		//351������ Release PASS ������ 350����� ����. 20170512, JSJUNG
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"), m_strStateName);				
				nStep = stepIdle;
			}
			else
			{	
				//DCR ��� ���� �����ص� ��˶� �� �׳� ���� �Ϸ��� TRUE 2017/06/21 HSK.
//				theProcBank.m_bDCRChk = TRUE;
//				theProcBank.m_bLightAlarmChk = TRUE;
				//������Ʈ FALSE����� 350���� �߻��Ѵ� 2017/06/21 HSK.
//				theProcBank.m_bGoodTrayReq = FALSE;
				SetError(ALARM_OKTRAY_ID_RELEASE_REPORT_FAIL);
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] step Carrier Release Report Check Fail"), m_strStateName);
				
				//2017-07-11, SKCHO,262�������� ���н� Ʈ���� ���� �� ���� ���� ���ϰ� �������(�۾��� �Ǽ���) ��� ī��Ʈ�� 2���Ǿ� 262���� �ȵ�
				theProcBank.MinusSubGoodAlignCount();
				theProcBank.MinusGoodAlignCount();
			}
			
		}		
		else if(m_Timer.Stop(FALSE) > theConfigBank.m_Option.m_dCarrierInfoWaitTime)
		{
			// Light �˶� �߻� �ÿ��� ������ ������ �ʰ� ����. 2017/05/01 HSK.
			// DCR ��� ���� �����ص� ��˶� �� �׳� ���� �Ϸ��� TRUE 2017/06/21 HSK.
//			theProcBank.m_bDCRChk = TRUE;
//			theProcBank.m_bLightAlarmChk = TRUE;
//			theProcBank.m_bGoodTrayReq = FALSE;

			if(theUnitFunc.ReadCarrierResult())
			{
				nStep = stepEnd;
				m_Timer.Start();
			}
			else
			{
				SetError(ALARM_OKTRAY_ID_RELEASE_REPORT_TIMEOUT);
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] step Carrier Release Report Check Time Out"), m_strStateName);
	//			nStep = stepIdle;
				//2017-07-11, SKCHO,262�������� ���н� Ʈ���� ���� �� ���� ���� ���ϰ� �������(�۾��� �Ǽ���) ��� ī��Ʈ�� 2���Ǿ� 262���� �ȵ�
				theProcBank.MinusSubGoodAlignCount();
				theProcBank.MinusGoodAlignCount();
			}
		}
		//2017-12-09,SKCHO, 1�ʾȿ� ���޾��� ��� �ٽ� ��Ʈoff
		else if(m_Timer.Stop(FALSE) > 0.3)
		{
			theDeviceMelsec.RobotBitOff(UDROBOT_X_TRAYVING_Y_OKNGRTN);
		}
		
		break;
	}

	m_nStep = nStep;

	return nStep;
}

int CStateGoodTrayAlign::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{
	case stepStart:	
		TrayAlignStart(_T("OKTRAY"));
		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"), m_strStateName);
		m_Timer.Start();
		nStep++;
		break;
	case stepEnd:
		m_bRtn[0] = TrayAlignChk(_T("OKTRAY"));
		if (IsReturnOk())
		{
			theProcBank.m_bGoodTrayAlignChk = TRUE;
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"), m_strStateName);
			nStep = stepIdle;
		}
		else if (m_Timer.Stop(FALSE) > 5.)
		{
			// �˶��� �߻� �ؼ� ��ġ �� �� �ֵ��� �ϰ� ��õ� ���ϵ��� �Ѵ�.
			theProcBank.m_bGoodTrayAlignChk = TRUE;
			SetError(ALARM_OKTRAY_ALIGN_FAIL);
			// ������ ���� �����༭ ������ �־��� ����. 2017/06/21 HSK.
			nStep = stepIdle;
		}
		break;
	}

	m_nStep = nStep;

	return nStep;
}

int CStateGoodTrayOutLift::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{
	case stepStart:				// ������ Tray �ܼ��� �׿��� Ȯ�� �ϰ� Lift Down Pos�� �̵� �� Stopper Up.
		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"), m_strStateName);
		BottomAlignCylFwdBwd(GOOD_OUT_TRAY_PORT,CYL_BWD);
		m_Timer.Start();
		nStep++;
		break;
	case stepZAlignDown:
		m_bRtn[0] = BottomAlignCylFwdBwdChk(GOOD_OUT_TRAY_PORT,CYL_BWD);
		if (IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepZAlign"), m_strStateName);
//			�ߺ� ���� �Ͽ� �ϳ� ���� 2017/05/03 HSK.
//			CIM_TRAYProcess(_T("352"),_T("2"));
			//--OutTrayLiftZ_DownPos();

			if(OutTrayLiftZ_DownChk() == FALSE)   //2017-12-22,SKCHO, �˶� �߻��Ͽ� �ٽ� �����Ҷ� �ٿ� ��ġ�� ������ ����� ��ġ�� �ȿø��� ���̵� ���� �� �������� ���� �̵�
			{
				OutTrayLiftZ_AlignPos();
				nStep++;
			}
			else
			{
				BottomAlignCylFwdBwd(GOOD_OUT_TRAY_PORT, CYL_FWD);
				nStep = stepGuideBwd;
			}

			//20171108,JSJUNG, AGV OUT IDLE TIME ����
			if(theProcBank.m_bTRAYOutIDLE == TRUE)
			{
				::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_IDLEAGVOUTEND_UPDATE, 0,0); 
				theLog[LOG_LOAD_DELAY_TACT].AddBuf(_T("AGV OUT IDLE TIME ����"));
			}

			m_Timer.Start();
			
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			SetError(ALARM_OKTRAY_OUT_LIFT_LOWERGUIDE_BWD_CHK);
		}
		break;
	case stepGuideFwd:			// Conv2�� Tray �Ȱ��� Ȯ�� �ϰ� Conv Stop, Stopper Down �� Lift Up Pos�� �̵�.
		m_bRtn[0] = OutTrayLiftZ_AlignChk();
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepGuideFwd"), m_strStateName);
			BottomAlignCylFwdBwd(GOOD_OUT_TRAY_PORT, CYL_FWD);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)			SetError(ALARM_GOOD_TRAY_OUT_LIFT_Z_ALIGN_POS);
		}
		break;
	case stepGuideBwd: 
		m_bRtn[0] = BottomAlignCylFwdBwdChk(GOOD_OUT_TRAY_PORT, CYL_FWD);
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepGuideBwd"), m_strStateName);
			BottomAlignCylFwdBwd(GOOD_OUT_TRAY_PORT, CYL_BWD);
			BottomStoperUpDown(GOOD_OUT_TRAY_PORT, CYL_UP);		

			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)		SetError(ALARM_OKTRAY_OUT_LIFT_LOWERGUIDE_FWD_CHK);
		}
		break;
	case stepZDown: 
		m_bRtn[0] = m_Timer.Stop(FALSE) > 0.5 ? TRUE:FALSE;
		if(IsReturnOk())
		{
			//2017-12-22,SKCHO
			OutTrayLiftZ_DownPos();
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepZDown"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		break;
	case stepConvMove:			// Lift Down Pos Ȯ�� �ϰ� Conv1 , Conv2 Move
		m_bRtn[0] = BottomAlignCylFwdBwdChk(GOOD_OUT_TRAY_PORT, CYL_BWD);
		m_bRtn[1] = BottomStoperUpDownChk(GOOD_OUT_TRAY_PORT, CYL_UP);
		m_bRtn[2] = OutTrayLiftZ_DownChk();   //2017-12-22,SKCHO
		theProcBank.GoodTrayOutAGVFlag = TRUE;
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepConvMove"), m_strStateName);
			ConvRun(GOOD_OUT_TRAY_PORT,ON,CCW);
			BufConvRun(GOOD_OUT_TRAY_PORT,ON,CCW);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			//2017-11-22,YMG, �˶��� ����.		 
			if(m_bRtn[0] == FALSE)			SetError(ALARM_OKTRAY_OUT_LIFT_LOWERGUIDE_BWD_CHK); //SetError(ALARM_OKTRAY_LD_LIFT_LOWERGUIDE_BWD_CHK);
			else if(m_bRtn[1] == FALSE)		SetError(ALARM_OKTRAY_OUT_LIFT_STOPPER_UP_CHK);
			else if(m_bRtn[2] == FALSE)		SetError(ALARM_GOOD_TRAY_OUT_LIFT_Z_DOWN_POS);
		}
		break;

	case stepLiftUp:			// Conv2�� Tray �Ȱ��� Ȯ�� �ϰ� Conv Stop, Stopper Down �� Lift Up Pos�� �̵�.
		m_bRtn[0] = BufConvSenChk(GOOD_OUT_TRAY_PORT,SENSOR_ON);
		//2017-09-25,SKCHO, OUTLIFT SENSOR�� Ʈ���̰� ������ LIFT UP���� (OUT BUFFER�� Ʈ���̰� ������� Ʈ���̰� �ٽÿö�� �浹�� ����)
		m_bRtn[1] = ULDTrayLiftTrayChk(SENSOR_OFF);
		//2017-12-26,SKCHO OUT CONV SENSOR1,4 ���� �߰� �Ͽ� Ʈ���̰� ������ �ö󰡴� ��
		m_bRtn[2] = (OutConv_TrayChk(CONV_SEN_4, SENSOR_OFF) && OutConv_TrayChk(CONV_SEN_1, SENSOR_OFF));
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepLiftUp"), m_strStateName);

			//2017-07-15,SKCHO, ��ġ �̵�
			//2017-04-03., skcho, �޴��� ���ۿ��� �ʿ��ϹǷ� ���2ȣ��� �����ϰ� �ӽ� ����
			/*theUnitFunc.SetOutPutIO(Y_GOOD_ULD_TRAY_MUTING_LAMP_ON,ON);
			theUnitFunc.SetOutPutIO(Y_GOOD_ULD_TRAY_REQ_SW,ON);
			theUnitFunc.SetOutPutIO(Y_GOOD_ULD_TRAY_MUTE_ON,ON);
			theUnitFunc.SetOutPutIO(Y_GOOD_ULD_TRAY_COMP_SW,OFF);

			theProcBank.m_bGoodTrayOutSWReq = TRUE;
			theProcBank.m_bGoodOutTrayMuteOn = TRUE; // 2017.2.3 bgkim
			*/
			BottomAlignCylFwdBwd(GOOD_OUT_TRAY_PORT, CYL_BWD);
			ConvRun(GOOD_OUT_TRAY_PORT,OFF);
			BufConvRun(GOOD_OUT_TRAY_PORT,OFF);
			// ������� ���� ���� �ϴ� Ʈ���̰� �з��� ������ ������ ����Ʈ Ŀư ���� �Ѵ�. 2017/06/15 HSK.
//			BottomStoperUpDown(GOOD_OUT_TRAY_PORT, CYL_DOWN);
			OutTrayLiftZ_UpPos();
			m_Timer.Start();
			nStep++;
		}
		//2017-11-22,SKCHO OUTLIFT�� �߰�		
		else if(m_Timer.Stop(FALSE) > 15.)
		{
			if(m_bRtn[0] == FALSE)			SetError(ALARM_OKTRAY_OUT_BUF_CONV_TRAY_CHK);
		}
		else if(m_Timer.Stop(FALSE) > 6.)  
		{
			BottomAlignCylFwdBwd(GOOD_OUT_TRAY_PORT, CYL_BWD);			
		}
		else if(m_Timer.Stop(FALSE) > 5. ) 
		{
			BottomAlignCylFwdBwd(GOOD_OUT_TRAY_PORT, CYL_FWD);			
		}
		
		break;
	case stepLiftUpChk:   //2017-07-15,SKCHO, ȯ����� �߰� ��û(����Ʈ�� �ö󰣵� ���÷��� ������)
		m_bRtn[0] = OutTrayLiftZ_UpChk();
		if(IsReturnOk())
		{
			//2017-07-15,SKCHO, ȯ����� ��û���� �̵�
			theUnitFunc.SetOutPutIO(Y_GOOD_ULD_TRAY_MUTING_LAMP_ON,ON);
			theUnitFunc.SetOutPutIO(Y_GOOD_ULD_TRAY_REQ_SW,ON);
			theUnitFunc.SetOutPutIO(Y_GOOD_ULD_TRAY_MUTE_ON,ON);
			theUnitFunc.SetOutPutIO(Y_GOOD_ULD_TRAY_COMP_SW,OFF);

			theProcBank.m_bGoodTrayOutSWReq = TRUE;
			theProcBank.m_bGoodOutTrayMuteOn = TRUE; 

			m_Timer.Start();
            nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 15.)
		{
			if(m_bRtn[0] == FALSE)			SetError(ALARM_GOOD_TRAY_OUT_LIFT_Z_UP_POS);
		}
		break;
	case stepStopperDown:
		if(m_Timer.Stop(FALSE) > 1.)
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStopperDown"), m_strStateName);
			BottomStoperUpDown(GOOD_OUT_TRAY_PORT, CYL_DOWN);
			m_Timer.Start();
			nStep++;
		}
		break;
	case stepEnd:				// LiftUp Ȯ�� �ϰ� End.
		//m_bRtn[0] = OutTrayLiftZ_UpChk();  //2017-07-15,SKCHO,ȯ����� ��û���� ����
		m_bRtn[0] = BottomStoperUpDownChk(GOOD_OUT_TRAY_PORT, CYL_DOWN);
		if(IsReturnOk())
		{
			TrayData_from_to(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_OUT_LIFT,TRAY_INFO_POS_GOOD_OUT_BUFFER);
			CIM_TRAYProcess(_T("352"),_T("2"));
			if(theConfigBank.m_Option.m_bUseDryRun)
				TrayData_RemoveData(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_OUT_BUFFER);

			if (theConfigBank.m_Option.m_bUseLotEnd)
			{
//				theProcBank.m_bCarrierAssign = FALSE;

				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd -> Lot End Success"), m_strStateName);
				if(TrayData_Exist(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_OUT_LIFT) == FALSE
					&& TrayData_Exist(NG_TRAY_PORT, TRAY_INFO_POS_NG_OUT_LIFT_ARM) == FALSE)
				{
					theConfigBank.m_Option.m_bUseLotEnd = FALSE;
					if(TrayData_Exist(NG_TRAY_PORT, TRAY_INFO_POS_NG_OUT_LIFT_ARM) == FALSE)
						theConfigBank.SaveOptionFile();

					TrayOutBuzzer(TRUE,ON);
				}
			}
			else
				TrayOutBuzzer(FALSE,ON);		

			//2017-03-09,skcho, JOBCHANGE CASE1�� ������ JOBCHANGE_CASE2���� �ʱ�ȭ
			if(theConfigBank.m_UnloadRunInfo.GetJobChangeType() == JOBCHANGE_CASE1)
			{
				theConfigBank.m_UnloadRunInfo.SetJobChangeType(JOBCHANGE_CASE2); 
			}

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"), m_strStateName);
			nStep = stepIdle;

			theProcBank.m_bOKTrayTRLotEnd = FALSE; // 2017.2.6 bgkim

			theLog[LOG_SEQUENCE_TRAY].AddBuf( _T("LotCellCnt : %d"), theProcBank.m_nLotCellCnt);
			theProcBank.m_nLotCellCnt = 0;

			//2017-07-07,SKCHO, 262�߰����� ���ϱ� ���ؼ� �ű� ���� �߰�
			//--theProcBank.ResetGoodAlignCount();	 //�ʱ�ȭ GoodTrayTrGet���� �̵�	    
			
			//2017-07-20,SKCHO, GOODTRPUT END �������� �̵�
			//Ʈ���� ������ ���� ������ �ʱ�ȭ
			theProcBank.m_bCarrierReportWaitCheck = FALSE;
			theProcBank.ResetSubGoodAlignCount();
			
			theProcBank.ResetGoodOutLiftCount();// 2017-04-20,skcho, ����� �����Ѵ�.
		    theProcBank.ResetPre_GoodOutLiftCount();  //2017-04-23,skcho, ����� �������� �����Ѵ�.
			theProcBank.GoodTrayOutAGVFlag = FALSE;


			//2018-04-20,SKCHO, ALARM_OKTRAY_OUT_BUFF_PORT_FULL�˶� ����
			//theProcBank.ResetTrayAlarm(2);  //1�� ALARM_OKTRAY_OUT_BUFF_PORT_FULL �˶� ����
			//2018-06-11,NAM
			theProcBank.ResetTrayAlarm(1);  //1�� ALARM_OKTRAY_OUT_BUFF_PORT_FULL �˶� ����
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			 //2017-07-15,SKCHO,ȯ����� ��û���� ����
			//--if(m_bRtn[0] == FALSE)			SetError(ALARM_GOOD_TRAY_OUT_LIFT_Z_UP_POS);
			if(m_bRtn[0] == FALSE)	SetError(ALARM_OKTRAY_OUT_LIFT_STOPPER_DOWN_CHK);

			//2017-05-15,skcho, ���� �������� �˶� �߻��ص� good lift pitch�� �Ҽ� �ְ�(���ѷ����߻����ɼ�����)
			theProcBank.m_bCarrierReportWaitCheck = FALSE;
		}
		break;
	}

	m_nStep = nStep;

	return nStep;
}


//2017-08-30,SKCHO, AGV �ű� ������ �߰�
#define EQ_REQUEST_TIMEOUT 3.0  //agv request ��ȣ �̻� Ÿ�Ӿƿ�
#define EQ_CONV_TIMEOUT 20.0    //agv conv ���� �̻� Ÿ�Ӿƿ�/ tray ��� �� �ɸ� �߻�
#define EQ_RUN_TIMEOUT 3.0      //agv run ��ȣ �̻� Ÿ�Ӿƿ�

int CStateGoodTraySend::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;
	
	//OK UNLOAD
	//x14A REQ   //Y10C REQ
	//X14B CON	 //Y10D CON
	//X14C RUN	 //Y10E RUN
	switch(nStep)
	{
	case stepStart:				// AGV Ready üũ �� �ε� ��û
		m_bRtn[0] = GetInPutIOCheck(X_GOOD_ULD_BUFFER_CONV_TRAY_SEN_4,ON); // ������ �ȿ��� Ʈ���� �ִ��� Ȯ��. �� �ۿ� �ִ� ������ �켱 Ȯ�� 2017/04/29 HSK.
		if (IsReturnOk())
		{
			
		        //2018-06-11,NAM
				/*if (theProcBank.m_AlarmID== ALARM_OKTRAY_OUT_BUFF_PORT_FULL)
				{
					theProcBank.ResetTrayAlarm(2);  //2�� ALARM_OKTRAY_OUT_BUFF_PORT_FULL �˶� ����
					//theProcBank.AlarmClear();
				}*/
				theProcBank.ResetTrayAlarm(1);  //2�� ALARM_OKTRAY_OUT_BUFF_PORT_FULL �˶� ����
			
			theLog[LOG_AGV].AddBuf( _T("[%s] stepStart"), m_strStateName);			

			//2017-08-30,SKCHO,�˶� �߻� �� ABORT �ʱ�ȭ
			SendAGVAbort(GOOD_OUT_TRAY_PORT,OFF);			

			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > theConfigBank.m_Option.m_AGVWaitTime)
		{
			SetError(ALARM_OKTRAY_OUT_BUF_CONV_TRAY_CHK);
		}
		break;
	case stepConnectChk:
		m_bRtn[0] = AGVConnectionChk(GOOD_OUT_TRAY_PORT);//AGVRequestChk(GOOD_OUT_TRAY_PORT);  //CONNECTION CHK(X_14B). JSJUNG. 20170307
		if (IsReturnOk())
		{
			//2017-03-03,skcho �ڵ������ ����ƮĿư �����ϱ� ���ؼ�
			theUnitFunc.SetOutPutIO(Y_GOOD_ULD_TRAY_MUTING_LAMP_ON,ON);
			theUnitFunc.SetOutPutIO(Y_GOOD_ULD_TRAY_REQ_SW,ON);
			theUnitFunc.SetOutPutIO(Y_GOOD_ULD_TRAY_MUTE_ON,ON);

			SendAGVConnection(GOOD_OUT_TRAY_PORT,ON);		
			SendAGVReq(GOOD_OUT_TRAY_PORT,OFF);			
			SendAGVStart(GOOD_OUT_TRAY_PORT,OFF);			

			theLog[LOG_AGV].AddBuf( _T("[%s] stepConnectChk :Send Connetion On - Send Request Off - Send Start Off"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > theConfigBank.m_Option.m_AGVWaitTime)
		{
			SendAGVConnection(GOOD_OUT_TRAY_PORT,OFF);		
			SendAGVReq(GOOD_OUT_TRAY_PORT,OFF);			
			SendAGVStart(GOOD_OUT_TRAY_PORT,OFF);

			//2017-08-30,SKCHO,�˶� �߻� �� ABORT ON
			SendAGVAbort(GOOD_OUT_TRAY_PORT,ON);

			SetError(ALARM_AGV_READY);
		}
		break;
	case stepTransferChk:		// �ε� ��û ���� Transfer Req Chk �� 
		m_bRtn[0] = AGVRequestChk(GOOD_OUT_TRAY_PORT);;//AGVConnectionChk(GOOD_OUT_TRAY_PORT);		//TR-REQ CHK(X_14A). JSJUNG. 20170307
		if (IsReturnOk())
		{
			//MuteOn(ON,GOOD_OUT_TRAY_PORT);
			
			theUnitFunc.SetOutPutIO(Y_GOOD_ULD_TRAY_MUTING_LAMP_ON,ON);
			theUnitFunc.SetOutPutIO(Y_GOOD_ULD_TRAY_REQ_SW,ON);
			theUnitFunc.SetOutPutIO(Y_GOOD_ULD_TRAY_MUTE_ON,ON);
			theProcBank.m_bGoodOutTrayMuteOn = TRUE; // 2017.2.3 bgkim
			
			BottomStoperUpDown(GOOD_OUT_TRAY_PORT, CYL_DOWN, TRAY_BUFFER); 
			
			SendAGVConnection(GOOD_OUT_TRAY_PORT,ON);		
			SendAGVReq(GOOD_OUT_TRAY_PORT,ON);			
			SendAGVStart(GOOD_OUT_TRAY_PORT,OFF);
			//BufConvRun(GOOD_OUT_TRAY_PORT,ON,CCW);        //Conv ���� ��ɸ���..
			theLog[LOG_AGV].AddBuf( _T("[%s] stepTransferChk :Send Connetion On - Send Request On - Send Start Off"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > EQ_REQUEST_TIMEOUT)//theConfigBank.m_Option.m_AGVWaitTime)
		{
			SendAGVConnection(GOOD_OUT_TRAY_PORT,OFF);		
			SendAGVReq(GOOD_OUT_TRAY_PORT,OFF);			
			SendAGVStart(GOOD_OUT_TRAY_PORT,OFF);

			//2017-08-30,SKCHO,�˶� �߻� �� ABORT ON
			SendAGVAbort(GOOD_OUT_TRAY_PORT,ON);

			SetError(ALARM_AGV_TRANSFER_REQ);
		}
		break;
	case stepBusyChk:		// AGV Busy Chk �� Conv Sen Chk �� Comp On
		m_bRtn[0] = AGVBusyChk(GOOD_OUT_TRAY_PORT);			
		m_bRtn[1] = BottomStoperUpDownChk(GOOD_OUT_TRAY_PORT, CYL_DOWN, TRAY_BUFFER); 
		if (IsReturnOk())
		{
			//20170801, JSJUNG, preload abnormal case ���� flag
			theProcBank.GoodTrayOutAGVFlag = TRUE;

			SendAGVConnection(GOOD_OUT_TRAY_PORT,ON);		
			SendAGVReq(GOOD_OUT_TRAY_PORT,ON);			
			SendAGVStart(GOOD_OUT_TRAY_PORT,ON);		
			BufConvRun(GOOD_OUT_TRAY_PORT,ON,CCW);        //Conv ���� 
			theLog[LOG_AGV].AddBuf( _T("[%s] stepConvRun :Send Connetion On - Send Request On - Send Start On"), m_strStateName);
			m_Timer.Start();
			nStep++;
			//m_bRtn[0] = BufConvSenChk(GOOD_OUT_TRAY_PORT,SENSOR_ON);
			//if (IsReturnOk())
			//{
			//	SendAGVComp(GOOD_OUT_TRAY_PORT,ON);
			//	BufConvRun(GOOD_OUT_TRAY_PORT,OFF);
			//	theLog[LOG_AGV].AddBuf( _T("[%s] stepBusyChk"), m_strStateName);
			//	m_Timer.Start();
			//	nStep++;
			//}
			//else if (m_Timer.Stop(FALSE) > 20.)
			//{
			//	SetError(ALARM_AGV_CONV_MOVE);
			//}
		}
		else if (m_Timer.Stop(FALSE) > EQ_RUN_TIMEOUT)//theConfigBank.m_Option.m_AGVWaitTime)
		{
			SendAGVConnection(GOOD_OUT_TRAY_PORT,OFF);		
			SendAGVReq(GOOD_OUT_TRAY_PORT,OFF);			
			SendAGVStart(GOOD_OUT_TRAY_PORT,OFF);

			//2017-08-30,SKCHO,�˶� �߻� �� ABORT ON
			SendAGVAbort(GOOD_OUT_TRAY_PORT,ON);

			if(m_bRtn[0] == FALSE)	 SetError(ALARM_AGV_BUSY);
			else					 SetError(ALARM_OKTRAY_OUT_LIFT_STOPPER_DOWN_CHK);
		}
		break;
	case stepEnd:		// AGV Comp Ȯ�� �� AGV ��ȣ All Off
		// Run�� �����µ� �̶� �츮 ��ȣ�� ������. 2017/04/27 HSK
		m_bRtn[0] = !AGVBusyChk(GOOD_OUT_TRAY_PORT);			//RUN CHK(X_14C). JSJUNG. 20170307
		if (IsReturnOk())
		{

			SendAGVStart(GOOD_OUT_TRAY_PORT,OFF);			
			SendAGVReq(GOOD_OUT_TRAY_PORT,OFF);		
			SendAGVConnection(GOOD_OUT_TRAY_PORT,OFF);	
			//MuteOn(OFF,GOOD_OUT_TRAY_PORT);
			BufConvRun(GOOD_OUT_TRAY_PORT,OFF);

			TrayData_RemoveData(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_OUT_BUFFER);
			CIM_TRAYProcess(_T("353"),_T("2"));	

			theUnitFunc.SetOutPutIO(Y_GOOD_ULD_TRAY_MUTING_LAMP_ON,OFF);
			theUnitFunc.SetOutPutIO(Y_GOOD_ULD_TRAY_REQ_SW,OFF);
			theUnitFunc.SetOutPutIO(Y_GOOD_ULD_TRAY_MUTE_ON,OFF);
			theProcBank.m_bGoodOutTrayMuteOn = FALSE; // 2017.2.3 bgkim

			//20170801, JSJUNG, preload abnormal case ���� flag
			theProcBank.GoodTrayOutAGVFlag = FALSE;

			// AGV�� �޾� ������ ������ ���ش� 2017/06/15 HSK.
			TrayOutBuzzer(TRUE,OFF);
			TrayOutBuzzer(FALSE,OFF);
			theLog[LOG_AGV].AddBuf( _T("[%s] stepAllOff :Send Connetion Off - Send Request Off - Send Start Off"), m_strStateName);
			nStep = stepIdle;
		}
		else if (m_Timer.Stop(FALSE) > EQ_CONV_TIMEOUT)//RUN OFF�� �ȵȰ��� CONV �������� ���� Ÿ�Ӿƿ� ����
		{
			SendAGVConnection(GOOD_OUT_TRAY_PORT,OFF);		
			SendAGVReq(GOOD_OUT_TRAY_PORT,OFF);				
			SendAGVStart(GOOD_OUT_TRAY_PORT,OFF);

			//2017-08-30,SKCHO,�˶� �߻� �� ABORT ON
			SendAGVAbort(GOOD_OUT_TRAY_PORT,ON);

			SetError(ALARM_AGV_COMP);
			theLog[LOG_AGV].AddBuf( _T("[%s] stepAllOff - Error"), m_strStateName);
		}
		break;
	}

	m_nStep = nStep;

	return nStep;
}


int CStateNGTrayReceive::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{
	case stepStart:				// AGV Ready üũ �� �ε� ��û, Buffer Stopper Down, In Stopper UP.
		m_bRtn[0] = AGVConnectionChk(NG_TRAY_PORT);
		if (IsReturnOk())
		{
			SendAGVReq(NG_TRAY_PORT,ON);
			BottomStoperUpDown(NG_TRAY_PORT, CYL_DOWN, TRAY_BUFFER);
			BottomStoperUpDown(NG_TRAY_PORT, CYL_UP, TRAY_IN);
			theLog[LOG_AGV].AddBuf( _T("[%s] stepStart"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > theConfigBank.m_Option.m_AGVWaitTime)
		{
			SetError(ALARM_AGV_READY);
		}
		break;
	case stepTransferChk:		// �ε� ��û ���� Transfer Req Chk �� Buffer Stopper Down, In Stopper UP Ȯ��, Conv ���� ���
		m_bRtn[0] = AGVRequestChk(NG_TRAY_PORT);
		m_bRtn[1] = BottomStoperUpDownChk(NG_TRAY_PORT, CYL_DOWN, TRAY_BUFFER);
		m_bRtn[2] = BottomStoperUpDownChk(NG_TRAY_PORT, CYL_UP, TRAY_IN);
		if (IsReturnOk())
		{
			MuteOn(ON,NG_TRAY_PORT);
			SendAGVStart(NG_TRAY_PORT,ON);
			BufConvRun(NG_TRAY_PORT,ON);
			theLog[LOG_AGV].AddBuf( _T("[%s] stepTransferChk"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > theConfigBank.m_Option.m_AGVWaitTime)
		{
			if(m_bRtn[0] == FALSE)			SetError(ALARM_AGV_TRANSFER_REQ);
			else if(m_bRtn[1] == FALSE)	SetError(ALARM_NGTRAY_OUT_LIFT_STOPPER_DOWN_CHK);
			else if(m_bRtn[2] == FALSE)	SetError(ALARM_NGTRAY_IN_LIFT_STOPPER_UP_CHK);
		}
		break;
	case stepBusyChk:		// AGV Busy Chk �� Conv Sen Chk �� Comp On, Stopper Down
		m_bRtn[0] = AGVBusyChk(NG_TRAY_PORT);
		if (IsReturnOk())
		{
			m_bRtn[0] = BufConvSenChk(NG_TRAY_PORT,SENSOR_ON);
			if (IsReturnOk())
			{
				SendAGVComp(NG_TRAY_PORT,ON);
				BufConvRun(NG_TRAY_PORT,OFF);
				BottomStoperUpDown(NG_TRAY_PORT, CYL_DOWN , TRAY_IN);
				theLog[LOG_AGV].AddBuf( _T("[%s] stepBusyChk"), m_strStateName);
				m_Timer.Start();
				nStep++;
			}
			else if (m_Timer.Stop(FALSE) > 20.)
			{
				SetError(ALARM_AGV_CONV_MOVE);
			}
		}
		else if (m_Timer.Stop(FALSE) > theConfigBank.m_Option.m_AGVWaitTime)
		{
			SetError(ALARM_AGV_BUSY);
		}
		break;
	case stepEnd:		// AGV Comp Ȯ�� �� AGV ��ȣ All Off
		m_bRtn[0] = AGVCompChk(NG_TRAY_PORT);
		m_bRtn[1] = BottomStoperUpDownChk(NG_TRAY_PORT, CYL_DOWN, TRAY_IN);
		if (IsReturnOk())
		{
			MuteOn(OFF,NG_TRAY_PORT);
			SendAGVReq(NG_TRAY_PORT,OFF);
			SendAGVStart(NG_TRAY_PORT,OFF);
			SendAGVComp(NG_TRAY_PORT,OFF);
			TrayData_Create(NG_TRAY_PORT,TRAY_INFO_POS_NG_OUT_REQ_BUFFER);
			theLog[LOG_AGV].AddBuf( _T("[%s] stepAllOff"), m_strStateName);
			nStep = stepIdle;
		}
		else if (m_Timer.Stop(FALSE) > theConfigBank.m_Option.m_AGVWaitTime)
		{
			if(m_bRtn[0] == FALSE)			SetError(ALARM_AGV_COMP);
			else if(m_bRtn[1] == FALSE)	SetError(ALARM_NGTRAY_IN_LIFT_STOPPER_DOWN_CHK);
		}
		break;
	}

	m_nStep = nStep;

	return nStep;
}

int CStateNGTrayManualReceive::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{
	case stepStart:				// AGV Ready üũ �� �ε� ��û
		break;
	case stepEnd:		// AGV Comp Ȯ�� �� AGV ��ȣ All Off
		break;
	}

	m_nStep = nStep;

	return nStep;
}

int CStateNGTrayIn::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{
	case stepStart:				// Conv1�� Tray Ȯ�� �ϰ� Lift Teaching Pos�� Tray �����ص� �Ǵ� �����̸� Conv 1 , Conv 2 ����.
//		m_bRtn[0] = NGConv_TrayChk(NG_TRAY_PORT, CONV_SEN_3, SENSOR_OFF);
//		m_bRtn[1] = NGConv_TrayChk(NG_BUFFER_TRAY_PORT, CONV_SEN_4, SENSOR_ON);
///		if(IsReturnOk())
//		{
		   //2018-06-11,NAM
			/* 
		    if (theProcBank.m_AlarmID== ALARM_NGTRAY_IN_PORT_ALIGN_EMPTY)
				  {
					theProcBank.ResetTrayAlarm(3);  //2�� ALARM_OKTRAY_OUT_BUFF_PORT_FULL �˶� ����
					//theProcBank.AlarmClear();
				  }
             */
	     	theProcBank.ResetTrayAlarm(2);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"), m_strStateName);
			// �����ϰ��� ���� �� �˶� �߻� �� ��쿡 �Ǹ��� ������ ���� �ȵǴ� ���� �־� ������ �������� ����.17/04/22
			NGTrayUpDown(NG_TRAY_PORT, CYL_DOWN);
			BottomStoperUpDown(NG_TRAY_PORT, CYL_DOWN, TRAY_IN);
			m_Timer.Start();
			nStep++;
//		}
//		else
//		{
//			if(m_bRtn[1] == FALSE)		SetError(ALARM_NGTRAY_IN_CONV_TRAY_CHK3);
//			else if(m_bRtn[1] == FALSE)	SetError(ALARM_NGTRAY_OUT_CONV_TRAY_CHK4);
//		}
		break;
	case stepCylDownChk:
		m_bRtn[0] = NGTrayUpDownChk(NG_TRAY_PORT, CYL_DOWN);
		m_bRtn[1] = BottomStoperUpDownChk(NG_TRAY_PORT, CYL_DOWN, TRAY_IN);

		if(IsReturnOk())
		{
			ConvRun(NG_TRAY_PORT,ON,CW);
			BufConvRun(NG_TRAY_PORT,ON,CW);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)	SetError(ALARM_NGTRAY_IN_LIFT_TRAY_DOWN_CHK);
			else if(m_bRtn[1] == FALSE)	SetError(ALARM_NGTRAY_IN_LIFT_STOPPER_DOWN_CHK);
		}
		break;
	case stepBTMTrayUP:
		m_bRtn[0] = NGConv_TrayChk(NG_TRAY_PORT, CONV_SEN_3, SENSOR_ON);
		m_bRtn[1] = NGConv_TrayChk(NG_TRAY_PORT, CONV_SEN_4, SENSOR_ON);
		m_bRtn[2] = NGConv_TrayChk(NG_TRAY_PORT, CONV_SEN_5, SENSOR_ON);
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepBTMTrayUP"), m_strStateName);
			ConvRun(NG_TRAY_PORT,OFF);
			BufConvRun(NG_TRAY_PORT,OFF);
			NGTrayUpDown(NG_TRAY_PORT, CYL_UP);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 20.)
		{
			ConvRun(NG_TRAY_PORT,OFF);
			BufConvRun(NG_TRAY_PORT,OFF);
			if(m_bRtn[0] == FALSE)	SetError(ALARM_NGTRAY_IN_CONV_TRAY_CHK3);
			else if(m_bRtn[1] == FALSE)	SetError(ALARM_NGTRAY_IN_CONV_TRAY_CHK4);
			else if(m_bRtn[2] == FALSE)	SetError(ALARM_NGTRAY_IN_CONV_TRAY_CHK5);
		}
		break;	
	case stepBTMFwd:
		m_bRtn[0] = NGTrayUpDownChk(NG_TRAY_PORT, CYL_UP);
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepBTMFwd"), m_strStateName);
			BottomAlignCylFwdBwd(NG_TRAY_PORT, CYL_FWD);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			SetError(ALARM_NGTRAY_IN_LIFT_TRAY_UP_CHK);
		}
		break;
	case stepBTMBwd:
		m_bRtn[0] = BottomAlignCylFwdBwdChk(NG_TRAY_PORT, CYL_FWD);
		m_bRtn[1] = (m_Timer.Stop(FALSE) > 1.) ? TRUE : FALSE;
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepBTMBwd"), m_strStateName);
			BottomAlignCylFwdBwd(NG_TRAY_PORT, CYL_BWD);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			SetError(ALARM_NGTRAY_IN_LIFT_LOWERGUIDE_FWD_CHK);	
		}
		break;	
	case stepEnd:				// Lift ������ Tray ���� ���� �ϰ� Conv 1,2 Stop.
		m_bRtn[0] = BottomAlignCylFwdBwdChk(NG_TRAY_PORT, CYL_BWD);
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"), m_strStateName);
			TrayData_from_to(NG_TRAY_PORT,TRAY_INFO_POS_NG_OUT_REQ_BUFFER,TRAY_INFO_POS_NG_IN_BUFFER);
			nStep = stepIdle;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			SetError(ALARM_NGTRAY_IN_LIFT_LOWERGUIDE_BWD_CHK);	
		}
		break;	
	}

	m_nStep = nStep;

	return nStep;
}


int CStateNGTrayInLiftUp::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{
	case stepStart:	// Lift Arm Sensor�� Tray ���� ���� �ϰ� , Tray�� In Conv ������ ���� �ִ��� Ȯ�� �ϰ� Lift, Guide BackWard. Tray ������� Step Idle. 
		if(InNGTrayLiftZ_UpChk() && NGTrayLiftTrayChk(SENSOR_ON))
		{
			nStep = stepEnd;
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart -> stepEnd"), m_strStateName);
			break;
		}
		// Down Pos�� �̵� �� �˶� �߻� �� ����ŸƮ�� Ʈ���� ġ�� �ö󰬴ٰ� �������� ���� ����.
		if(InNGTrayLiftZ_DownChk() && NGTrayLiftTrayChk(SENSOR_OFF))
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart -> stepGuideForward"), m_strStateName);
			nStep = stepGuideForward;
		}

		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"), m_strStateName);
		NGLiftArmFwdBwd(NG_TRAY_PORT, CYL_FWD);
		m_Timer.Start();
		nStep++;
		break;
	case stepLiftMid:
		m_bRtn[0] = NGLiftArmFwdBwdChk(NG_TRAY_PORT, CYL_FWD);
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepLiftMid"), m_strStateName);
			InNGTrayLiftZ_MidPos();
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			//2017-11-22,YMG, �˶��� ����.
			SetError(ALARM_NGTRAY_IN_LIFT_ARM_FWD_CHK);  //SetError(ALARM_NGTRAY_IN_LIFT_LOWERGUIDE_FWD_CHK);
		}
		break;
	case stepLiftMidChk:
		m_bRtn[0] = InNGTrayLiftZ_MidPosChk();
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepLiftMidChk"), m_strStateName);
			NGLiftArmFwdBwd(NG_TRAY_PORT, CYL_BWD);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			SetError(ALARM_NG_TRAY_IN_LIFT_Z_MID_POS);
		}
		break;
	case stepLiftDown:				// Lift, Guide Backward Ȯ�� �ϰ� Lift �ٿ�, Tray UP.
		m_bRtn[0] = NGLiftArmFwdBwdChk(NG_TRAY_PORT, CYL_BWD);
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepLiftDown"), m_strStateName);
			InNGTrayLiftZ_DownPos();
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			SetError(ALARM_NGTRAY_IN_LIFT_ARM_BWD_CHK);
		}
		break;
	case stepGuideForward:				// �ٿ� Pos, Tray Up Ȯ�� �ϰ� Lift Arm Forward�ؼ� Ʈ���� �����.
		m_bRtn[0] = InNGTrayLiftZ_DownChk();
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepGuideForward"), m_strStateName);
			NGLiftArmFwdBwd(NG_TRAY_PORT, CYL_FWD);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 10.)
		{
			if(m_bRtn[0] == FALSE)			SetError(ALARM_NG_TRAY_IN_LIFT_Z_DOWN_POS);
		}
		break;
	case stepLiftUp:				 // Lift Arm Forward Ȯ�� �� Lift Up, Tray Down.
		m_bRtn[0] = NGLiftArmFwdBwdChk(NG_TRAY_PORT, CYL_FWD);
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepLiftUp"), m_strStateName);
			InNGTrayLiftZ_UpPos();
			NGTrayUpDown(NG_TRAY_PORT, CYL_DOWN);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)			SetError(ALARM_NGTRAY_IN_LIFT_ARM_FWD_CHK);
		}
		break;
	case stepEnd:    // ƼĪ ��ġ Up Ȯ�� �� Lower Guide Forward.
		m_bRtn[0] = InNGTrayLiftZ_UpChk();
		m_bRtn[1] = NGTrayUpDownChk(NG_TRAY_PORT, CYL_DOWN);
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"), m_strStateName);
			TrayData_from_to(NG_TRAY_PORT,TRAY_INFO_POS_NG_IN_BUFFER,TRAY_INFO_POS_NG_IN_LIFT_ARM);
			
			

			//2017-05-17,skcho ,NG TRAY ���� ����
			//CIM_TRAYProcess(_T("351"),_T("3"));
			
			theProcBank.m_bNGTrayReq = FALSE;
			nStep = stepIdle;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)			SetError(ALARM_NG_TRAY_IN_LIFT_Z_UP_POS);
			else if(m_bRtn[1] == FALSE)		SetError(ALARM_NGTRAY_IN_LIFT_TRAY_DOWN_CHK);
		}
		break;
	}

	m_nStep = nStep;

	return nStep;
}

// ��ü������ ��ġ�� �������� ���� �Ͽ� ���� �Ͽ��� 2017/04/29 HSK
int CStateNGTrayInLiftPitch::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{
	case stepStart:				 //Top Tray ���� �� Top Tray Guide �� Division Back Ȯ�� �ϰ� Lift Pitch Up.
		m_bRtn[0] = NGTrayLiftTrayChk(SENSOR_ON);
	
		if(IsReturnOk())
		{	//��ġ������ �� �� �ణ�� ������ �����..17/04/21
			AlignCylFwdBwd(NG_TRAY_PORT, CYL_BWD);
			DivisionCylFwdBwd(NG_TRAY_PORT, CYL_BWD);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 20.)
		{
			if(m_bRtn[0] == FALSE)		SetError(ALARM_NGTRAY_IN_LIFT_TRAY_CHK);
		}
		break;	
	case stepPitchUp:
		InNGTrayLiftZ_PitchUpRetry();
		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepPitchUp"), m_strStateName);
		m_Timer.Start();
		nStep++;
		break;
	case stepPitchUpChk:
		m_bRtn[0] = NGTrayInAlignTrayChk(SENSOR_ON);
		if(IsReturnOk())
		{
			InNGTrayLiftZ_PitchUpStop();
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepPitchUpChk"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 30.)
		{
			SetError(ALARM_NGTRAY_IN_TOP_TRAY_CHK);
		}
		break;
	case stepLiftStop:			// Top Tray �����ϰ� Lift Stop Ȯ�� , Division Forward.
		m_bRtn[0] = NGTrayInAlignTrayChk(SENSOR_ON);
		m_bRtn[1] = InNGTrayLiftZ_StopChk();
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepLiftStop"), m_strStateName);
			DivisionCylFwdBwd(NG_TRAY_PORT, CYL_FWD);

			theProcBank.NGTrayBitX = 1;  //2017-03-05,skcho ng tray cell ��ġ �ʱ�ȭ
			theProcBank.NGTrayBitY = 1;

			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 3.)
		{
			if(m_bRtn[0] == FALSE)			SetError(ALARM_NGTRAY_IN_TOP_TRAY_CHK);
			else if(m_bRtn[1] == FALSE)		SetError(ALARM_NG_TRAY_IN_LIFT_Z_PITCH_UP_POS);
		}
		break;
	case stepGuideForward:		// Division Forward Ȯ�� �ϰ� Pitch Down.
		m_bRtn[0] = DivisionCylFwdBwdChk(NG_TRAY_PORT, CYL_FWD);
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepGuideForward"), m_strStateName);
			AlignCylFwdBwd(NG_TRAY_PORT, CYL_FWD);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)			SetError(ALARM_NGTRAY_IN_DIVISION_FWD_CHK);
		}
		break;	
	case stepEnd:				// Guide Forward Ȯ�� �ϰ� End.
		m_bRtn[0] = AlignCylFwdBwdChk(NG_TRAY_PORT, CYL_FWD);
		m_bRtn[1] = NGTrayInAlignTrayChk(SENSOR_ON);
		if(IsReturnOk())
		{
			TrayData_from_toAlign(NG_TRAY_PORT,TRAY_INFO_POS_NG_IN_LIFT_ARM,TRAY_INFO_POS_NG_IN_ALIGN);
			
			//2018-03-10,MGYUN, ����� ���� �� PORT6 / LOADED , READY TO PROCESS
			theMonitorBank.WritePortStatus(6,2,1);
//			
			if(theConfigBank.m_Option.m_bUseDryRun == FALSE)
//			{
				// 2017.2.12 bgkim
				// NG Lift Arm ������ �������� Ʈ���̷κ��� �������� �ʾ� ������ Ʈ���̸� ����� �ص�
				// ������ �������� �ʾ� IO �����忡�� ó��
	//			if (NGTrayLiftTrayChk(SENSOR_OFF))
//				if(TrayData_Exist(NG_TRAY_PORT, TRAY_INFO_POS_GOOD_OUT_LIFT))
//				{
//					TrayData_RemoveData(NG_TRAY_PORT,TRAY_INFO_POS_NG_IN_LIFT_ARM);
//				}
//			}
			//2018-04-20,SKCHO, ALARM_NGTRAY_IN_PORT_ALIGN_EMPTY �˶� ����
			//theProcBank.ResetTrayAlarm(3);
			//2018-06-11,NAM
			theProcBank.ResetTrayAlarm(2);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"), m_strStateName);
			nStep = stepIdle;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)			SetError(ALARM_NGTRAY_IN_ALIGN_FWD_CHK);
			else							SetError(ALARM_NGTRAY_IN_TOP_TRAY_CHK);
		}
		break;	
	}

	m_nStep = nStep;

	return nStep;
}

int CStateNGTrayTRGet::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{
	case stepStart:				 //Top Tray Cell ������ Ȯ�� �ϰ� X Get Pos�� �̵�.
		//2017-09-21,SKCHO, Retry ��� �߰�
		if(theProcBank.m_nNGTrayRetryStep == 0)
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"), m_strStateName);
		}
		else if(theProcBank.m_nNGTrayRetryStep == 1)
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart - Retry"), m_strStateName);
		}		

		theProcBank.m_bNGTrayTRLotEnd = FALSE;

		NGTrayTrX_InPos();
		m_Timer.Start();
		nStep++;
		break;
	case stepZDown:			// X Get Pos Ȯ�� �� Z Down and Vac On.
		m_bRtn[0] = NGTrayTrX_InChk();
		if(IsReturnOk())
		{
			if (theConfigBank.m_Option.m_bUseTrayAlign)
			{
				if (TrayData_Exist(NG_TRAY_PORT,TRAY_INFO_POS_NG_OUT_LIFT_ARM))
					theProcBank.m_bNGTrayAlignChk = FALSE;
			}

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepZDown"), m_strStateName);
			//2017-09-21,SKCHO, Retry ��� �߰�
			if(theProcBank.m_nNGTrayRetryStep == 0)
			{
				NGTrayTrZ_InPos(0.0);
			}
			else if(theProcBank.m_nNGTrayRetryStep == 1)
			{
				NGTrayTrZ_InPos(theConfigBank.m_Option.m_dNGTRRetryLength);
			}

			NGTrayTr_VacOnOff(VAC_ON);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)		SetError(ALARM_NGTRAY_TR_X_GET_POS);
		}
		break;
	case stepGuideBackward:		// Z Down �� Vac On Ȯ�� �� Division, Tray Guide Back.
		//2017-09-21,SKCHO, Retry ��� �߰�
		if(theProcBank.m_nNGTrayRetryStep == 0)
		{
			m_bRtn[0] = NGTrayTrZ_InChk(0.0);
		}
		else if(theProcBank.m_nNGTrayRetryStep == 1)
		{
			m_bRtn[0] = NGTrayTrZ_InChk(theConfigBank.m_Option.m_dNGTRRetryLength);
		}
		
		m_bRtn[1] = NGTrayTr_VacChk(VAC_ON);
		m_bRtn[2] = (m_Timer.Stop(FALSE) > 0.5 ) ? TRUE : FALSE;
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepGuideBackward"), m_strStateName);
			AlignCylFwdBwd(NG_TRAY_PORT, CYL_BWD);
			
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)			SetError(ALARM_NGTRAY_TR_Z_GET_POS);
			else if(m_bRtn[1] == FALSE)		SetError(ALARM_NGTRAY_TR_VAC); 
		}
		break;
	case stepZUp:		 // Guide Back Ȯ�� �� Z Up.
		m_bRtn[0] = AlignCylFwdBwdChk(NG_TRAY_PORT, CYL_BWD);
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepZUp"), m_strStateName);
			NGTrayTrZ_UpPos(0.5);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)			SetError(ALARM_NGTRAY_IN_ALIGN_BWD_CHK);
		}
		break;
	case stepZUpCheck:
		m_bRtn[0] = NGTrayTrZ_UpChk();
		m_bRtn[1] = NGTrayTr_VacChk(VAC_ON);
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepZUpCheck"), m_strStateName);
			TrayData_from_to(NG_TRAY_PORT,TRAY_INFO_POS_NG_IN_ALIGN,TRAY_INFO_POS_NG_TR);
			DivisionCylFwdBwd(NG_TRAY_PORT, CYL_BWD);

			// IOCheckTHR���� ���� �ϴ� �� �ȱ�. 2017/05/02 HSK
			if (NGTrayLiftTrayChk(SENSOR_OFF)
					&& theConfigBank.m_Option.m_bUseDryRun == FALSE
					&& TrayData_Exist(NG_TRAY_PORT,TRAY_INFO_POS_NG_IN_LIFT_ARM))
			{
				TrayData_RemoveData(NG_TRAY_PORT,TRAY_INFO_POS_NG_IN_LIFT_ARM);
				
			}

			//2017-05-23,skcho, in/buffer�� Ʈ���̰� ���� �˶� �︲
			if (NGTrayLiftTrayChk(SENSOR_OFF)
					&& theConfigBank.m_Option.m_bUseDryRun == FALSE
					&& TrayData_Exist(NG_TRAY_PORT,TRAY_INFO_POS_NG_IN_LIFT_ARM) == FALSE)
			{	
				//2018-07-25,NAM, Checking the EXISTENCE OF  NG TRAY IN BUFFER 
				if( (TrayData_Exist(NG_TRAY_PORT, TRAY_INFO_POS_NG_OUT_REQ_BUFFER) == FALSE)
					&&(TrayData_Exist(NG_TRAY_PORT, TRAY_INFO_POS_NG_IN_BUFFER)==FALSE)) 
				
				{
					//2017-09-12, SKCHO,main timer���� �ϹǷ� ����
					//TrayInBuzzer(2,ON);  //GOOD IN
					theProcBank.m_nNGInPortEmptyStep  =1;  //2017-09-12,SKCHO, NG PORT �� �˸�
				}
			}

			//2017-12-01,SKCHO
			if(theProcBank.m_nNGTrayRetryStep == 1)
			{
				theProductBank.m_NGTR_RetryInfo.nSucess++;
			}
			theProcBank.m_nNGTrayRetryStep = 0;

			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)		SetError(ALARM_NGTRAY_TR_Z_UP_POS);
			else if(m_bRtn[1] == FALSE)	
			{
				//2017-09-21,SKCHO, LENGTH= 0�̸� ��ɻ�����
				if(theConfigBank.m_Option.m_dNGTRRetryLength == 0)
				{
						SetError(ALARM_NGTRAY_TR_VAC); 
				}
				else
				{
					//2017-09-21,SKCHO, Retry ��� �߰�
					if(theProcBank.m_nNGTrayRetryStep == 0)
					{
						m_Timer.Start();
						nStep = stepStart;
						theProcBank.m_nNGTrayRetryStep = 1;
						DivisionCylFwdBwd(NG_TRAY_PORT, CYL_FWD);

						//2017-12-01,SKCHO
						theProductBank.m_NGTR_RetryInfo.nTotal++;

					}
					else if(theProcBank.m_nNGTrayRetryStep == 1)
					{
						SetError(ALARM_NGTRAY_TR_VAC); 
						theProcBank.m_nNGTrayRetryStep = 0;
					}
				}
				
			}
		}
		break;
	case stepXMove:		 // Z Up Ȯ�� �� X Put Pos �̵�
		m_bRtn[0] = DivisionCylFwdBwdChk(NG_TRAY_PORT, CYL_BWD);
		m_bRtn[1] = theProcBank.m_bNGTrayAlignChk;

		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepXMove"), m_strStateName);
			NGTrayTrX_OutPos();
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)	SetError(ALARM_NGTRAY_IN_DIVISION_BWD_CHK);
			else					SetError(ALARM_NGTRAY_ALIGN_FAIL);
		}
		break;
	case stepEnd:				// X Put Pos Ȯ�� �� End
		m_bRtn[0] = NGTrayTrX_OutChk();
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"), m_strStateName);
			//2017-10-013,SKCHO ���� �ʱ�ȭ
			theProcBank.m_nNGTrayRetryStep =0;

			nStep = stepIdle;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)		SetError(ALARM_NGTRAY_TR_X_PUT_POS);
		}
		break;
	}

	m_nStep = nStep;

	return nStep;
}

int CStateNGTrayTRPut::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{
	case stepStart:				//Tray TR�� Tray �ִ��� X Put Pos���� Ȯ�� �� Put Pos�� �ƴ� ��� Put Pos�� �̵�. 
		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"), m_strStateName);
		NGTrayTrX_OutPos();
		m_Timer.Start();
		nStep++;
		break;
	case stepZDown:			 //X Put Pos Ȯ�� �� Z Down.
		m_bRtn[0] = NGTrayTrX_OutChk();
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepZDown"), m_strStateName);

			//2017-12-01,SKCHO, ���� ������ ���� ARM  ������ ���� ���н� TR PUT ��ġ���� �ѹ� �� üũ
			if (NGTrayLiftTrayChk(SENSOR_OFF)
					&& theConfigBank.m_Option.m_bUseDryRun == FALSE
					&& TrayData_Exist(NG_TRAY_PORT,TRAY_INFO_POS_NG_IN_LIFT_ARM))
			{
				TrayData_RemoveData(NG_TRAY_PORT,TRAY_INFO_POS_NG_IN_LIFT_ARM);
				
			}

			NGTrayTrZ_OutPos();
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)		SetError(ALARM_NGTRAY_TR_X_PUT_POS);
		}
		break;
	case stepVacOff:		// Z Down Ȯ�� �� Vac Off.
		m_bRtn[0] = NGTrayTrZ_OutChk();
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepVacOff"), m_strStateName);

			NGTrayTr_VacOnOff(VAC_OFF);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)		SetError(ALARM_NGTRAY_TR_Z_PUT_POS);
		}
		break;
	case stepZUp:		 // Vac Off Ȯ�� �� Z Up and OutLift Pitch Down.
		m_bRtn[0] = NGTrayTr_VacChk(VAC_OFF);
		if(IsReturnOk())
		{
//			NGTrayTr_BlowOff();
			NGTrayTrZ_UpPos();

			if(theProcBank.m_TrayBank.GetTrayInfo(NG_TRAY_PORT, TRAY_INFO_POS_NG_OUT_LIFT_ARM)->IsExist())
				TrayData_RemoveData(NG_TRAY_PORT,TRAY_INFO_POS_NG_TR);
			else
				TrayData_from_to(NG_TRAY_PORT,TRAY_INFO_POS_NG_TR,TRAY_INFO_POS_NG_OUT_LIFT_ARM);

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepZUp"), m_strStateName);
			theProcBank.m_TrayBank.GetTrayInfo(NG_TRAY_PORT, TRAY_INFO_POS_NG_OUT_LIFT_ARM)->m_nTrayCount++;
			if (theProcBank.m_TrayBank.GetTrayInfo(NG_TRAY_PORT, TRAY_INFO_POS_NG_OUT_LIFT_ARM)->GetTrayCount() >= theConfigBank.m_Option.m_nMaxNGTrayCount)
			{
				theProcBank.m_TrayBank.GetTrayInfo(NG_TRAY_PORT, TRAY_INFO_POS_NG_OUT_LIFT_ARM)->SetTrayFull(TRUE);
			}
			OutNGTrayLiftZ_PitchDown(1.5);

			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)		SetError(ALARM_NGTRAY_TR_VAC);
		}
		break;
	case stepEnd:				// Z Up Ȯ�� �� Pitch down Ȯ�� �� End.
		m_bRtn[0] = NGTrayTrZ_UpChk();
		m_bRtn[1] = OutNGTrayLiftZ_StopChk();
		m_bRtn[2] = NGTrayTr_VacChk(VAC_OFF);
		if(IsReturnOk())
		{
			// TR�� ��ο츦 �Ҿ������� ���� �ö���� ��찡 �־� ��ο� ���� ������ TR�� �ö���� �� �ķ� ���� 2017/04/27 HSK.
			NGTrayTr_BlowOff();
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"), m_strStateName);
			nStep = stepIdle;

			if(theConfigBank.m_Option.m_bUseLotEnd
				&& TrayData_CellIsFull(NG_TRAY_PORT,TRAY_INFO_POS_NG_IN_ALIGN) == FALSE )
				theProcBank.m_bNGTrayTRLotEnd = TRUE; // 2017.2.6 bgkim
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)			SetError(ALARM_NGTRAY_TR_Z_UP_POS);
			else if(m_bRtn[1] == FALSE)		SetError(ALARM_NG_TRAY_OUT_LIFT_Z_PITCH_DOWN_POS);
			else if(m_bRtn[2] == FALSE)		SetError(ALARM_NGTRAY_TR_VAC);
		}
		break;
	}

	m_nStep = nStep;

	return nStep;
}

int CStateNGTrayAlign::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{
	case stepStart:	
		TrayAlignStart(_T("NGTRAY"));
		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"), m_strStateName);
		m_Timer.Start();
		nStep++;
		break;
	case stepEnd:
		m_bRtn[0] = TrayAlignChk(_T("NGTRAY"));
		if (IsReturnOk())
		{
			theProcBank.m_bNGTrayAlignChk = TRUE;
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"), m_strStateName);
			nStep = stepIdle;
		}
		else if (m_Timer.Stop(FALSE) > 5.)
		{
			SetError(ALARM_NGTRAY_ALIGN_FAIL);
			//2017-06-22 JSJUNG. Tray Align NG�� ��� Ȯ�θ��ϰ� ���� ����
			nStep = stepIdle;
		}
		break;
	}

	m_nStep = nStep;

	return nStep;
}

int CStateNGTrayOutLift::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{
	case stepStart:				// ������ Tray �ܼ��� �׿��� Ȯ�� �ϰ� Lift Down Pos�� �̵�.
		// Tray Data ���ٰ� �Ͽ� ������ ���µ� ���� ���� �Ǿ� ������ �˶�. 2017/04/29 HSK.
		// Sensor üũ�� �ϴ� ������ �۾��� �Ǽ��� Ʈ���̵����� �ȸ���� Ʈ���� ������� ��� ���.
		m_bRtn[0] = BufConvSenChk(NG_TRAY_PORT,SENSOR_OFF);
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"), m_strStateName);
			OutNGTrayLiftZ_DownPos();
//			�ߺ� ���� �Ͽ� �ϳ� ���� 2017/05/03 HSK.
//			CIM_TRAYProcess(_T("352"),_T("3"));

			NGTrayUpDown(NG_BUFFER_TRAY_PORT,CYL_UP);
			m_Timer.Start();
			nStep++;
		}
		//�˶� �߻� �ؼ� �ٿ� ��ġ�� ���� ���� ����Ͽ� �߰� 2017/04/29 HSK.
		//�ٿ� ��ġ�� ���� �� ���� ������ ����� �־� �� ������ ��Ż ��� �ش� �������� ����.
		else if(OutNGTrayLiftZ_DownChk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] Already Down Position stepStart"), m_strStateName);
			NGTrayUpDown(NG_BUFFER_TRAY_PORT,CYL_UP);
			m_Timer.Start();
			nStep++;
		}
		else
		{
			SetError(ALARM_NGTRAY_OUT_CONV_TRAY_CHK3);
		}
		break;
	case stepLiftDownChk:		// Lift Down Pos Ȯ�� �� Lift arm Bwd. 
		m_bRtn[0] = OutNGTrayLiftZ_DownChk();
		m_bRtn[1] = NGTrayUpDownChk(NG_BUFFER_TRAY_PORT, CYL_UP);
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepLiftDownChk"), m_strStateName);
			NGLiftArmFwdBwd(NG_BUFFER_TRAY_PORT,CYL_BWD);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)			SetError(ALARM_NG_TRAY_OUT_LIFT_Z);
			else							SetError(ALARM_NGTRAY_OUT_LIFT_TRAY_UP_CHK); //2017-11-27,YMG, SetError(ALARM_NGTRAY_IN_LIFT_TRAY_DOWN_CHK);
		}
		break;
	case stepLiftUp:
		m_bRtn[0] = NGLiftArmFwdBwdChk(NG_BUFFER_TRAY_PORT,CYL_BWD);
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepLiftUp"), m_strStateName);
			OutNGTrayLiftZ_UpPos();
			NGTrayUpDown(NG_BUFFER_TRAY_PORT,CYL_DOWN);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			SetError(ALARM_NGTRAY_OUT_LIFT_ARM_BWD_CHK);
		}
		break;
	case stepUpChk:
		m_bRtn[0] = OutNGTrayLiftZ_UpChk();
		m_bRtn[1] = NGTrayUpDownChk(NG_BUFFER_TRAY_PORT,CYL_DOWN);
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepUpChk"), m_strStateName);
			NGLiftArmFwdBwd(NG_BUFFER_TRAY_PORT,CYL_FWD);
			//�̰� ���� �� �ִ���... �÷ȴ� ������ �κе� ��� �ּ� 17/04/22
//			BottomStoperUpDown(NG_TRAY_PORT, CYL_UP);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)	SetError(ALARM_NG_TRAY_OUT_LIFT_Z_UP_POS);
			else					SetError(ALARM_NGTRAY_OUT_LIFT_TRAY_DOWN_CHK);  //2017-11-27,YMG, SetError(ALARM_NGTRAY_IN_LIFT_TRAY_DOWN_CHK);
		}
		break;
	case stepEnd:		// Lift Arm Bwd, Conv Tray Sensor Ȯ�� �ϰ� End.
		m_bRtn[0] = NGLiftArmFwdBwdChk(NG_BUFFER_TRAY_PORT,CYL_FWD);
//		m_bRtn[1] = BottomStoperUpDownChk(NG_TRAY_PORT,CYL_UP);
		if(IsReturnOk())
		{
			TrayData_from_to(NG_TRAY_PORT,TRAY_INFO_POS_NG_OUT_LIFT_ARM,TRAY_INFO_POS_NG_OUT_COMP_BUFFER);
			//2017-05-17,skcho, NG��Ʈ ���� ���� - CIM QUAL
			//CIM_TRAYProcess(_T("352"),_T("3"));
			
			if(theConfigBank.m_Option.m_bUseDryRun)
				TrayData_RemoveData(NG_TRAY_PORT,TRAY_INFO_POS_NG_OUT_COMP_BUFFER);

			if (theConfigBank.m_Option.m_bUseLotEnd)
			{
				// GOOD�ʿ� ���� Ʈ���̰� ���� �� LOT END ������ �ʴ� ���� ���� ���� 17/04/24 HSK
				if(TrayData_Exist(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_OUT_LIFT) == FALSE
					&& TrayData_Exist(NG_TRAY_PORT, TRAY_INFO_POS_NG_OUT_LIFT_ARM) == FALSE)
				{
					theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd -> Lot End Success"), m_strStateName);
					theConfigBank.m_Option.m_bUseLotEnd = FALSE;
					if(TrayData_Exist(NG_TRAY_PORT, TRAY_INFO_POS_NG_OUT_LIFT_ARM) == FALSE)
						theConfigBank.SaveOptionFile();

					TrayOutBuzzer(TRUE,ON);
				}
			}
			else
				TrayOutBuzzer(FALSE,ON);
			//2017-04-23,skcho, good tray ���� ���� �������� good tray lot end ������ ���߰� ��.good tray������lot end flag�� ������.
			///if (theConfigBank.m_Option.m_bUseLotEnd)
			//{
				//theProcBank.m_bCarrierAssign = FALSE;				
				//theConfigBank.m_Option.m_bUseLotEnd = FALSE;
				//theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd -> Lot End Success"), m_strStateName);
				//if(TrayData_Exist(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_OUT_LIFT) == FALSE)
				//{
				//	theConfigBank.SaveOptionFile();
				//}
			//}
			//2017-04-03., skcho, �޴��� ���ۿ��� �ʿ��ϹǷ� ���2ȣ��� �����ϰ� �ӽ� ����
			theUnitFunc.SetOutPutIO(Y_NG_TRAY_MUTING_LAMP_ON,ON);
			theUnitFunc.SetOutPutIO(Y_NG_TRAY_OUT_REQ_SW,ON);
			theUnitFunc.SetOutPutIO(Y_NG_TRAY_MUTE_ON,ON);
			theUnitFunc.SetOutPutIO(Y_NG_TRAY_OUT_COMP_SW,OFF);

			theProcBank.m_bNGTrayOutSWReq = TRUE;
			theProcBank.m_bNGTrayMuteOn = TRUE; // 2017.2.3 bgkim

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"), m_strStateName);
			nStep = stepIdle;

			theProcBank.m_bNGTrayTRLotEnd = FALSE; // 2017.2.6 bgkim
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)		SetError(ALARM_NGTRAY_OUT_LIFT_ARM_FWD_CHK);
			else if(m_bRtn[1] == FALSE)	SetError(ALARM_NGTRAY_OUT_LIFT_STOPPER_UP_CHK);
		}
		break;
	}

	m_nStep = nStep;

	return nStep;
}



int CStateNGTraySend::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{
	case stepStart:				// AGV Ready üũ �� Stopper Down, �ε� ��û
		m_bRtn[0] = AGVConnectionChk(NG_TRAY_PORT);
		if (IsReturnOk())
		{
			SendAGVConnection(NG_TRAY_PORT,ON);
			BottomStoperUpDown(NG_TRAY_PORT, CYL_DOWN, TRAY_BUFFER);
			theLog[LOG_AGV].AddBuf( _T("[%s] stepStart"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > theConfigBank.m_Option.m_AGVWaitTime)
		{
			SetError(ALARM_AGV_READY);
		}
		break;
	case stepTransferChk:		// �ε� ��û ���� Transfer Req Chk �� Conv ���� ���
		m_bRtn[0] = AGVRequestChk(NG_TRAY_PORT);
		m_bRtn[1] = BottomStoperUpDownChk(NG_TRAY_PORT, CYL_DOWN, TRAY_BUFFER);
		if (IsReturnOk())
		{
			MuteOn(ON,NG_TRAY_PORT);
			SendAGVStart(NG_TRAY_PORT,ON);
			BufConvRun(NG_TRAY_PORT,ON,CCW);
			theLog[LOG_AGV].AddBuf( _T("[%s] stepTransferChk"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > theConfigBank.m_Option.m_AGVWaitTime)
		{
			if(m_bRtn[0] == FALSE)			SetError(ALARM_AGV_TRANSFER_REQ);
			else if(m_bRtn[1] == FALSE)	SetError(ALARM_NGTRAY_OUT_LIFT_STOPPER_DOWN_CHK);
		}
		break;
	case stepBusyChk:		// AGV Busy Chk �� Conv Sen Chk �� Comp On, In Stopper Down.
		m_bRtn[0] = AGVBusyChk(NG_TRAY_PORT);
		if (IsReturnOk())
		{
			m_bRtn[0] = BufConvSenChk(NG_TRAY_PORT,SENSOR_OFF);
			if (IsReturnOk())
			{
				SendAGVComp(NG_TRAY_PORT,ON);
				BufConvRun(NG_TRAY_PORT,OFF);
				OutNGTrayLiftZ_UpPos();
				theLog[LOG_AGV].AddBuf( _T("[%s] stepBusyChk"), m_strStateName);
				m_Timer.Start();
				nStep++;
			}
			else if (m_Timer.Stop(FALSE) > 20.)
			{
				SetError(ALARM_AGV_CONV_MOVE);
			}
		}
		else if (m_Timer.Stop(FALSE) > theConfigBank.m_Option.m_AGVWaitTime)
		{
			SetError(ALARM_AGV_BUSY);
		}
		break;
	case stepEnd:		// AGV Comp Ȯ�� �� AGV ��ȣ All Off
		m_bRtn[0] = AGVCompChk(NG_TRAY_PORT);
		m_bRtn[1] = OutNGTrayLiftZ_UpChk();
		if (IsReturnOk())
		{
			MuteOn(OFF,NG_TRAY_PORT);
			SendAGVReq(NG_TRAY_PORT,OFF);
			SendAGVStart(NG_TRAY_PORT,OFF);
			SendAGVComp(NG_TRAY_PORT,OFF);
			TrayData_RemoveData(NG_TRAY_PORT,TRAY_INFO_POS_NG_OUT_COMP_BUFFER);
			
			//2017-05-17,skcho, NG ��Ʈ ���� ����
			//CIM_TRAYProcess(_T("353"),_T("3"));

			theLog[LOG_AGV].AddBuf( _T("[%s] stepAllOff"), m_strStateName);
			nStep = stepIdle;
		}
		else if (m_Timer.Stop(FALSE) > theConfigBank.m_Option.m_AGVWaitTime)
		{
			if(m_bRtn[0] == FALSE)			SetError(ALARM_AGV_COMP);
			else if(m_bRtn[1] == FALSE)		SetError(ALARM_NG_TRAY_OUT_LIFT_Z_UP_POS);
		}
		break;
	}

	m_nStep = nStep;

	return nStep;
}

int CStateGoodTrayManualReceive::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{

	case stepStart:			
		m_Timer.Start();
		theLog[LOG_AGV].AddBuf( _T("[%s] stepStart"), m_strStateName);
		nStep++;
		break;
	case stepBuffSenChk:			// AGV Ready üũ �� �ε� ��û
		m_bRtn[0] = BuffConv_TrayChk(CONV_SEN_1,SENSOR_ON);//����üũ�� ������ �ȿ��� �Ѵ�.2017/04/29 HSK.
		if (IsReturnOk())
		{
			theLog[LOG_AGV].AddBuf( _T("[%s] stepTBuffSenChk"), m_strStateName);	
			BottomStoperUpDown(GOOD_IN_TRAY_PORT, CYL_UP,TRAY_IN);
			BufConvRun(GOOD_IN_TRAY_PORT,ON);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 10.0)
		{
			nStep = stepIdle;
		}
		break;
	case stepBuffSenChk1:
		m_bRtn[0] = BufConvSenChk(GOOD_IN_TRAY_PORT,SENSOR_ON);
		if (IsReturnOk())
		{			
			theLog[LOG_AGV].AddBuf( _T("[%s] stepTBuffSenChk1"), m_strStateName);
			BufConvRun(GOOD_IN_TRAY_PORT,OFF);
			BottomStoperUpDown(GOOD_IN_TRAY_PORT, CYL_DOWN,TRAY_IN);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 10.0)
		{
			nStep = stepIdle;
			BufConvRun(GOOD_IN_TRAY_PORT,OFF);
			BottomStoperUpDown(GOOD_IN_TRAY_PORT, CYL_DOWN,TRAY_IN);
		}
		break;	
		
	case stepEnd:		
		m_bRtn[1] = BottomStoperUpDownChk(GOOD_IN_TRAY_PORT, CYL_DOWN, TRAY_IN);
		if (IsReturnOk())
		{	
			 //2018-04-24,
				/*if (theProcBank.m_AlarmID== ALARM_OKTRAY_IN_PORT_ALIGN_EMPTY)
				{
					theProcBank.ResetTrayAlarm(1);  //1�� ALARM_OKTRAY_IN_PORT_ALIGN_EMPTY �˶� ����
					//theProcBank.AlarmClear();

				}*/
			//2018-06-11,NAM
			theProcBank.ResetTrayAlarm(0);  //1�� ALARM_OKTRAY_IN_PORT_ALIGN_EMPTY �˶� ����

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd "), m_strStateName);
			nStep = stepIdle;
		}
		else if (m_Timer.Stop(FALSE) >3.0)
		{			
			nStep = stepIdle;
		}
		break;
	}

	m_nStep = nStep;

	return nStep;
}