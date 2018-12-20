#include "StdAfx.h"
#include "StateAMT.h"

#include "StdAfx.h"
#include "StateAMT.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW

#endif


int CStateAMTTable::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{
	case stepStart:	// ���̺� �̵� �� ���Ͷ� üũ �� �� �� SetZone �� Turn
		m_bRtn[0] = GetTurnTableMoveReadyAxisCheck();
		m_bRtn[1] = GetZoneEnd(ZONE_ID_A);
		m_bRtn[2] = GetZoneEnd(ZONE_ID_B);
		m_bRtn[3] = GetZoneEnd(ZONE_ID_C);
		m_bRtn[4] = GetZoneEnd(ZONE_ID_D);
		m_bRtn[5] = B_Zone_Cam_X_Ready_Chk();
		m_bRtn[6] = B_Zone_Cam_Y_Ready_Chk();
		m_bRtn[7] = MoveUnLoadTR_ZAxis_UpChk();

		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"),m_strStateName);
				
			if(MoveToNext(TRUE) == FALSE) // Table ȸ��
			{		
				SetError(ALARM_WORK_TABLE_NOT_MOVE);
				break;
			}

			// Turn ���� Ÿ�� ���.
			TableTurnTimeStart();
			// Wait Ÿ�� ����.
			WaitTimeEnd();

			
			//2018-03-15,MGYUN, �����̺� ��ġ ���� �Ἵ ���.
			theMonitorBank.WriteTurnTablePos();

			// Table Pos ����.
			theProcBank.m_nInspTablePos++;
			if(theProcBank.m_nInspTablePos >= CONST_TABLE_POS::POS_MAX)
				theProcBank.m_nInspTablePos = CONST_TABLE_POS::POS_1;

			// Cell Data �̵�.
			CellData_ToNextPos();
			
			//2018-03-15,MGYUN, �����̺� ��ġ ���� �Ἵ ���.
			theMonitorBank.WriteTurnTablePos();
			
			// ������ ��� ��� �� �� ���� Ŭ���� ���� �̻�� 2017/04/26 HSK.
			/*
			if (theConfigBank.m_Option.m_bUseRepeatInsp)
			{
				Zone_CellInfo_Clear(CELL_POS_AZONE_CH1);
				Zone_CellInfo_Clear(CELL_POS_AZONE_CH2);
			}
			*/
			// ���̺� ������ �̵� �� PG Set Zone. �̵����� �ϸ� �ϳ� ����...
			AMT_PGSetZone();
			// C Zone �˻縦 ���� Zone Switching.
			Zone_Switching();
			// Zone End ����.
			SetZoneEnd(ZONE_ID_MAX, FALSE);
			// Zone Time Start
			ZoneTimeStart();

			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)									SetError(ALARM_TT_POS_ALARM);
			else if (m_bRtn[1] == FALSE)							SetError(ALARM_AZONE_END_CHECK);
			else if (m_bRtn[2] == FALSE)							SetError(ALARM_BZONE_END_CHECK);
			else if (m_bRtn[3] == FALSE)							SetError(ALARM_CZONE_END_CHECK);
			else if (m_bRtn[4] == FALSE)							SetError(ALARM_DZONE_END_CHECK);
			else if (m_bRtn[5] == FALSE)							SetError(ALARM_BZONE_ALIGN_CAM_X_READY_POS);
			else if (m_bRtn[6] == FALSE)							SetError(ALARM_BZONE_ALIGN_CAM_Y_READY_POS);
			else if (m_bRtn[7] == FALSE)							SetError(ALARM_ULD_TR_Z_UP_POS);
		}
		break;
	case stepEnd:	// ���̺� Turn üũ
		if (theProcBank.m_nInspTablePos == GetEntryTablePos(0.02))
		{
			// Turn ���� Ÿ�� ���.
			TableTurnTimeEnd();
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"),m_strStateName);
			
			double dValue = 0;
			theDeviceMotion.GetPosition(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_TURN_TABLE_T, dValue);		///	������ġ ǥ��
			//2017-07-24,JSJUNG, DD���� �α� �߰�.
			theLog[LOG_TURN_TABLE].AddBuf( _T("TURN TABLE POS : %f"),dValue);
			nStep = stepIdle;
		}
		else if (m_Timer.Stop(FALSE) > 5.)
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd Table Move Chk Err"),m_strStateName);
			SetError(ALARM_TT_POS_ALARM);
		}
		break;
	}

	m_nStep = nStep;

	return nStep;
}

int CStateAZone::Run()
{
	CCellInfo *pCell;
	int nRequest = 0;
	int nRCMDResult = 0;

	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;
	
	switch(nStep)
	{
	case stepStart:	// Contact üũ �� A Zone���� ���ϴ� �˻� ����.
		// ü�� ��ŵ�� �˻� ������ ��� �ɼ� �ε� ���� ���� .2017/04/26 HSK.
		/*
		if (JigSkipCheck() || (theConfigBank.m_Option.m_bUseRepeatInsp == TRUE) || (ChSkipJigIDCheck() == TRUE))
		{
			SetZoneEnd(ZONE_ID_A,TRUE);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] JigSkip %d."),m_strStateName,AMT_GetAutoJigID(ZONE_ID_A));
			TactUpdate(AZONE_TACT);
			nStep = stepIdle;
			break;
		}
		*/
		//2017-11-20,SKCHO ��ü ���Ͷ� ���°� ���� ���¿��� JIG SKIP �����ϰ� ����
		if (JigSkipCheck() )
		{
			SetZoneEnd(ZONE_ID_A,TRUE);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] JigSkip %d."),m_strStateName,AMT_GetAutoJigID(ZONE_ID_A));
			TactUpdate(AZONE_TACT);
			nStep = stepIdle;
			break;
		}

		if(IsJobEnd())
		{
			if((CellTagExist(CELL_POS_AZONE_CH1) == TRUE || CellTagExist(CELL_POS_AZONE_CH2) == TRUE))
			{
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] A Zone Job End stepStart."),m_strStateName);
				
				//2017-12-06,SKCHO �赿ȣ ���� ��û���� LDTR BLOWER OFF�� �̵�
				//2017-09-12,SKCHO JIG �������ڸ��� ��Ũ������ ����(�赿ȣ ���� ��û)
				/*if (CellTagExist(CELL_POS_AZONE_CH1))
				{
					AMTVacOn(VAC_ON,AMT_GetAutoJigID(ZONE_ID_A),JIG_CH_1);
				}
				if (CellTagExist(CELL_POS_AZONE_CH2))
				{
					AMTVacOn(VAC_ON,AMT_GetAutoJigID(ZONE_ID_A),JIG_CH_2);
				}
				*/
				nStep++;
				break;
			}
			// JobEnd ������ �δ��� Cell�� ���� ��� A Zone End ���� �ʰ� �δ� �� ��ٸ���. 017/04/27 HSK.
			if(CellDataMachineChk(CELL_POS_BZONE_CH1,CELL_POS_CZONE_CH2) == TRUE && (CellDataMachineChk(CELL_POS_LD_CONV,CELL_POS_LD_TR_CH2) == FALSE)
				&& (CellTagExist(CELL_POS_AZONE_CH1) == FALSE && CellTagExist(CELL_POS_AZONE_CH2) == FALSE))
			{
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] A Zone Job End."),m_strStateName);

				SetZoneEnd(ZONE_ID_A,TRUE);
				TactUpdate(AZONE_TACT);
				nStep = stepIdle;
				break;
			}
			else
			{
				nStep = stepIdle;
				break;
			}
		}
		else
		{
			// Job End�� �ƴҶ��� ���� üũ �ϵ��� �Ѵ�. 17/04/26 HSK
			if((CellTagExist(CELL_POS_AZONE_CH1) == TRUE || CellTagExist(CELL_POS_AZONE_CH2) == TRUE))
			{
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart."),m_strStateName);
				
				//2017-09-12,SKCHO JIG �������ڸ��� ��Ũ������ ����(�赿ȣ ���� ��û)
				if (CellTagExist(CELL_POS_AZONE_CH1))
				{
					AMTVacOn(VAC_ON,AMT_GetAutoJigID(ZONE_ID_A),JIG_CH_1);
				}
				if (CellTagExist(CELL_POS_AZONE_CH2))
				{
					AMTVacOn(VAC_ON,AMT_GetAutoJigID(ZONE_ID_A),JIG_CH_2);
				}

				nStep++;
				break;
			}
			else
			{
				nStep = stepIdle;
				break;
			}
		}
		break;
	//2018-07-25,SKCHO,
	/*case stepCh1CellRequest:
			
		pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_AZONE_CH1));
		if(pCell)
		{
			if( pCell->m_strCellID != _T("FAIL") )
			//if( pCell->m_strCellID != _T("UV_FAIL") ) //2017-04-05,skcho, �ڱ״н����� FAIL �� �����͸� ��
			{				
				//1. CELL INFO REQUEST �ߺ� üũ
				// Cell Info �ߺ� ���� �ص� �̻� ���ٰ� �Ͽ� �ּ� 2017/04/27 HSK
//				if(CellData_CellInfoRequestChk((CELL_POS)(CELL_POS_AZONE_CH1)) == FALSE)
//				{
					//1. CELL INFO REQUEST
					theUnitFunc.CIM_CH1_CellInfoRequest();
					if(theProcBank.m_nCH1CellInfoRetryStep == 0)
					{
						theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh1CellInfoRequest"),m_strStateName);
					}
					else
					{
						theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh1CellInfoRequest - Retry"),m_strStateName);
					}
					pCell->bCellInfoRequest = TRUE;
					pCell->m_strCellInfoResult = _T("-1");//REQUEST �ʱⰪ
//				}				
			}
			else
			{
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] CH1 MCR_FAIL"),m_strStateName);
				theUnitFunc.Set_InspectSkip(CELL_POS_AZONE_CH1,TRUE);  //2017-04-08,skcho, MCR READING ���н� �˻� ��ŵ
				
			}
		}
		else
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] CH1 NONE CELL"),m_strStateName);
			m_Timer.Start();
			nStep = stepCh2CellRequest;
			break;
		}

		m_Timer.Start();
		nStep++;
		break;*/
    case stepCh1TrackIn:
		//1. Cell ID Infomation ��ٸ�
		pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_AZONE_CH1));
		if(pCell)
		{			
			if(pCell->bCellInfoRequest)
			{			
				nRequest = theUnitFunc.CIM_CellInfoRequestCheck(CELL_POS_AZONE_CH1);	

				//609���� ���ϱ� ���� �ʱ�ȭ
				//CIM_SetRCMD()���� ���� ����
//				pCell->m_bIsInspectionSkip = TRUE;
				if(nRequest == 1)  //�������� ������
				{
					//1. cell infomation true�̸� track_in
					// �ߺ� Track In �߻��� �����ϱ� ���� Track In ���� üũ �� Track In �Ѵ�.
					if(CellData_TrackInChk(CELL_POS_AZONE_CH1) == FALSE)
					{
						// Ʈ���� ������ RCMD CH ���� �÷��� �� 2017/06/08 HSK
						theProcBank.m_bRCMDCh1Chk = TRUE;
						theProcBank.m_strRCMDCellIDCh1 = _T("");
						//2017-05-15,skcho, Track-in log OK,NoneCell,Already �и�
						if(CIM_CellTrackIn(CELL_POS_AZONE_CH1))
						{
							//20170201
							//TRACK IN�� �����Ƿ� �ݵ��  TRACK OUT�� ���ش�
							pCell->bTrackOutChk = TRUE;	
							theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh1TrackIn - Ok"),m_strStateName);
						}
						else
						{
							theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh1TrackIn - None Cell"),m_strStateName);
						}
					}
					else
					{
						theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh1TrackIn - Already"),m_strStateName);
					}	
					//����
					//theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh1TrackIn "),m_strStateName);
					m_Timer.Start();
					nStep++;
				}
				else if( nRequest== 0)
				{
					//2018-07-25,SKCHO, CELL LOADING �� PRODUCT_ID ���� �ֱ� ���ؼ� CELL INFORMATION REQUEST�� MCR READING �������� �̵�
					//�ٷ� �˶� �߻�
					SetError(ALARM_CIM_CELL_INFO_CH1);
					/*
					//Ÿ�� �ƿ��̰� �����н�  false �̸� �˶�
					if(m_Timer.Stop(FALSE) > theConfigBank.m_Option.m_dCellInfoWaitTime && !theConfigBank.m_Option.m_bUseCh1ReqByPass)
					{
						//2017-09-24,SKCHO,Ÿ�Ӿƿ��� �ѹ��� ��û�ϰ� �ι�° Ÿ�Ӿƿ������� ������ �ִ��� Ȯ���Ѵ�.
						if(theProcBank.m_nCH1CellInfoRetryStep == 0)
						{
							theProcBank.m_nCH1CellInfoRetryStep = 1;
							nStep = stepCh1CellRequest;
							m_Timer.Start();
						}
						else if(theProcBank.m_nCH1CellInfoRetryStep == 1)
						{
							theProcBank.m_nCH1CellInfoRetryStep = 0;
							//2017-09-24,SKCHO,Ÿ�Ӿƿ��϶� ��� ������ ������ �˶�
							if(!theUnitFunc.ReadCellInformationResult(CELL_POS_AZONE_CH1))
							{								
								SetError(ALARM_CIM_CELL_INFO_CH1);
							}
						}
						
					}
					//�����н� true �̸� ����
					else if(theConfigBank.m_Option.m_bUseCh1ReqByPass)
					{			
						theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh1CellInfoRequest BY-PASS"),m_strStateName);
						m_Timer.Start();
						nStep++;	
					}*/
				}
				else
				{
					theLog[LOG_SEQUENCE].AddBuf( _T("[%s] CHANNEL1 TO NG"),m_strStateName);
					theUnitFunc.Set_InspectSkip(CELL_POS_AZONE_CH1,TRUE);
					m_Timer.Start();
					nStep++;
				}
			}
			else
			{
				//609���� ���ϱ� ���� �ʱ�ȭ
				//CIM_SetRCMD()���� ���� ����
//				pCell->m_bIsInspectionSkip = TRUE;
				theUnitFunc.Set_InspectSkip(CELL_POS_AZONE_CH1,TRUE);
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] CHANNEL1 TO NG"),m_strStateName);
				m_Timer.Start();
				nStep++;

			}
		}
		else
		{
			//2018-07-25,SKCHO,
			nStep = stepCh2TrackIn;
			//nStep = stepCh2CellRequest;
			m_Timer.Start();
		}
		break;
	case stepCh1TrackInChk:
		m_bRtn[0] = m_Timer.Stop(FALSE) > 0.1 ? TRUE : FALSE;  //2017-04-07,skcho, Ʈ���� �ϰ� RCMD ���� ���ð�, �������� ���ϴ°�찡 �־� INSPECTOIN FAIL ����� ����
		if(IsReturnOk())
		{
			pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_AZONE_CH1));
			if(pCell->bTrackInChk)
			{		
				//1. Ʈ���� �� jobprocess ��ٸ�			
				// ������ ���� TrackIn ���� Ȯ�� �� ��������.
				nRCMDResult = CIM_SetRCMD(CELL_POS_AZONE_CH1);
		
				if (nRCMDResult ==1)
				{
					theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh1TrackInChk"),m_strStateName);
					theProcBank.m_bShowCh1TrackInChk = TRUE;
					//2017-05-19,skcho
					Set_RCMDResult(CELL_POS_AZONE_CH1, TRUE);

					m_Timer.Start();
					nStep++;
				}
				else if(nRCMDResult == -1)
				{			
				  //Ÿ�� �ƿ��̰� �����н�  false �̸� �˶�
					if(m_Timer.Stop(FALSE) > theConfigBank.m_Option.m_dCellInfoWaitTime  && !theConfigBank.m_Option.m_bUseCh1RcmdByPass)
					{
						if(theProcBank.m_bRCMDCh1Chk)
						{
							if(theProcBank.m_nCH1JobProcessRetryStep == 0)
							{
								theProcBank.m_nCH1JobProcessRetryStep = 1;
								nStep = stepCh1TrackInChk;
								m_Timer.Start();
							}
							else if(theProcBank.m_nCH1JobProcessRetryStep == 1)
							{
								//2017-09-24,SKCHO,Ÿ�Ӿƿ��϶� ��� ������ ������ �˶�
								if(!theUnitFunc.ReadJobProcessResult(CELL_POS_AZONE_CH1))
								{
									theProcBank.m_nCH1JobProcessRetryStep = 0;
									theUnitFunc.Set_InspectSkip(CELL_POS_AZONE_CH1,TRUE);
									SetError(ALARM_CIM_GET_RCMD_CH1);
									theProcBank.m_bRCMDCh1Chk = FALSE;	
								}
								else
								{
									m_Timer.Start();
									nStep =stepCh1TrackInChk ;
								}
							}
						}
					}
					//�����н� true �̸� ����
					else if(theConfigBank.m_Option.m_bUseCh1RcmdByPass)
					{			
						theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh1RCMD BY-PASS"),m_strStateName);
						m_Timer.Start();
						nStep++;	
					}
				}
				else
				{
					 theLog[LOG_SEQUENCE].AddBuf( _T("[%s] CHANNEL1 - RCMD Fail"),m_strStateName);				
					 
					 //2017-05-19,skcho
					 Set_RCMDResult(CELL_POS_AZONE_CH1, FALSE);
					
					 m_Timer.Start();
					 nStep++;
				}
			}
			else
			{
				//NG ��Ʈ��
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] CHANNEL1 TO NG"),m_strStateName);
				m_Timer.Start();
				nStep++;
			}
		}
		break;
	/*case stepCh2CellRequest:
		pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_AZONE_CH2));
		if(pCell)
		{
			if( pCell->m_strCellID != _T("FAIL") )
			//if( pCell->m_strCellID != _T("UV_FAIL") ) //2017-04-05,skcho, �ڱ״н����� FAIL �� �����͸� ��
			{
				//CELL INFO �ߺ�üũ
				// Cell Info �ߺ� ���� �ص� �̻� ���ٰ� �Ͽ� �ּ� 2017/04/27 HSK
//				if(CellData_CellInfoRequestChk((CELL_POS)(CELL_POS_AZONE_CH2)) == FALSE)
//				{
					theUnitFunc.CIM_CH2_CellInfoRequest();	
					if(theProcBank.m_nCH2CellInfoRetryStep == 0)
					{
						theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh2CellInfoRequest"),m_strStateName);
					}
					else
					{
						theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh2CellInfoRequest - Retry"),m_strStateName);
					}
					pCell->bCellInfoRequest =TRUE;	
					pCell->m_strCellInfoResult = _T("-1");//REQUEST �ʱⰪ
//				}
			}
			else
			{
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] CH2 MCR_FAIL"),m_strStateName);
				theUnitFunc.Set_InspectSkip(CELL_POS_AZONE_CH2,TRUE);  //2017-04-08,skcho, MCR READING ���н� �˻� ��ŵ
			}
		}
		else
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] CH2 NONE CELL"),m_strStateName);
			m_Timer.Start();
			nStep = stepContactChk;
			break;
		}
	
		m_Timer.Start();
		nStep++;
		break;*/
	case stepCh2TrackIn:
		//1. Cell ID Infomation ��ٸ�
		pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_AZONE_CH2));
		if(pCell)
		{			
			if(pCell->bCellInfoRequest)
			{
				nRequest = theUnitFunc.CIM_CellInfoRequestCheck(CELL_POS_AZONE_CH2);
				//609���� ���ϱ� ���� �ʱ�ȭ
				//CIM_SetRCMD()���� ���� ����
//				pCell->m_bIsInspectionSkip = TRUE;
				if(nRequest == 1)  //�������� ������
				{
					//1. cell infomation true�̸� track_in
					// �ߺ� Track In �߻��� �����ϱ� ���� Track In ���� üũ �� Track In �Ѵ�.
					if(CellData_TrackInChk(CELL_POS_AZONE_CH2) == FALSE)
					{
						// Ʈ���� ������ RCMD CH ���� �÷��� �� 2017/06/08 HSK
						theProcBank.m_bRCMDCh2Chk = TRUE;
						theProcBank.m_strRCMDCellIDCh2 = _T("");
						//2017-05-15,skcho, Track-in log OK,NoneCell,Already �и�
						if(CIM_CellTrackIn(CELL_POS_AZONE_CH2))
						{
							//20170201
							//TRACK IN�� �����Ƿ� �ݵ��  TRACK OUT�� ���ش�
							pCell->bTrackOutChk = TRUE;
							theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh2TrackIn - Ok"),m_strStateName);
						}
						else
						{
							theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh2TrackIn - None Cell"),m_strStateName);
						}
						
					}
					else
					{
						theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh2TrackIn - Already"),m_strStateName);
					}
				
					m_Timer.Start();
					nStep++;
				}
				else if( nRequest== 0)
				{
				  	//2018-07-25,SKCHO,
					SetError(ALARM_CIM_CELL_INFO_CH2);
					/*
					//Ÿ�� �ƿ��̰� �����н�  false �̸� �˶�
					if(m_Timer.Stop(FALSE) > theConfigBank.m_Option.m_dCellInfoWaitTime && !theConfigBank.m_Option.m_bUseCh2ReqByPass)
					{
						//2017-09-19,SKCHO TIMEOUT �߻��� 1ȸ ��õ�
						if(theProcBank.m_nCH2CellInfoRetryStep == 0)
						{
							theProcBank.m_nCH2CellInfoRetryStep = 1;
							nStep = stepCh2CellRequest;
							m_Timer.Start();
						}
						else if(theProcBank.m_nCH2CellInfoRetryStep == 1)
						{
							theProcBank.m_nCH2CellInfoRetryStep = 0;
							//2017-09-24,SKCHO,Ÿ�Ӿƿ��϶� ��� ������ ������ �˶�
							if(!theUnitFunc.ReadCellInformationResult(CELL_POS_AZONE_CH2))
							{								
								SetError(ALARM_CIM_CELL_INFO_CH2);
							}						
						}
					}
					//�����н� true �̸� ����
					else if(theConfigBank.m_Option.m_bUseCh2ReqByPass)
					{			
						theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh2CellInfoRequest BY-PASS"),m_strStateName);
						m_Timer.Start();
						nStep++;	
					}*/
				}
				else
				{
					theLog[LOG_SEQUENCE].AddBuf( _T("[%s] CHANNEL2 TO NG"),m_strStateName);
					theUnitFunc.Set_InspectSkip(CELL_POS_AZONE_CH2,TRUE);
					m_Timer.Start();
					nStep++;
				}
			}
			else
			{	
				//609���� ���ϱ� ���� �ʱ�ȭ
				//CIM_SetRCMD()���� ���� ����
//				pCell->m_bIsInspectionSkip = TRUE;
				theUnitFunc.Set_InspectSkip(CELL_POS_AZONE_CH2,TRUE);
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] CHANNEL2 TO NG"),m_strStateName);
				m_Timer.Start();
				nStep++;	

			}
		}
		else
		{
			nStep = stepContactChk;
			m_Timer.Start();
		}
		break;
	case stepCh2TrackInChk:
		m_bRtn[0] = m_Timer.Stop(FALSE) > 0.1 ? TRUE : FALSE;  //2017-04-07,skcho, Ʈ���� �ϰ� RCMD ���� ���ð�, �������� ���ϴ°�찡 �־� INSPECTOIN FAIL ����� ����
		if(IsReturnOk())
		{
			pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_AZONE_CH2));
			if(pCell->bTrackInChk)
			{
				//1. Ʈ���� �� jobprocess ��ٸ�			
				// ������ ���� TrackIn ���� Ȯ�� �� ��������.
				nRCMDResult = CIM_SetRCMD(CELL_POS_AZONE_CH2);
				if (nRCMDResult ==1)
				{
					theProcBank.m_bShowCh2TrackInChk = TRUE;
					theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh2TrackInChk"),m_strStateName);
					
					//2017-05-19,skcho
					Set_RCMDResult(CELL_POS_AZONE_CH2, TRUE);
					
					m_Timer.Start();
					nStep++;
				}
				else if(nRCMDResult == -1)
				{			
				  //Ÿ�� �ƿ��̰� �����н�  false �̸� �˶�
					if(m_Timer.Stop(FALSE) > theConfigBank.m_Option.m_dCellInfoWaitTime && !theConfigBank.m_Option.m_bUseCh2RcmdByPass)
					{
						if(theProcBank.m_bRCMDCh2Chk)
						{
							if(theProcBank.m_nCH2JobProcessRetryStep == 0)
							{
								theProcBank.m_nCH2JobProcessRetryStep = 1;
								nStep = stepCh2TrackInChk;
								m_Timer.Start();
							}
							else if(theProcBank.m_nCH2JobProcessRetryStep == 1)
							{
								//2017-09-24,SKCHO,Ÿ�Ӿƿ��϶� ��� ������ ������ �˶�
								if(!theUnitFunc.ReadJobProcessResult(CELL_POS_AZONE_CH2))
								{
									theProcBank.m_nCH2JobProcessRetryStep = 0;
									theUnitFunc.Set_InspectSkip(CELL_POS_AZONE_CH2,TRUE);
									SetError(ALARM_CIM_GET_RCMD_CH2);
									theProcBank.m_bRCMDCh2Chk = FALSE;	
								}
								else
								{
									m_Timer.Start();
									nStep =stepCh2TrackInChk ;
								}
							}
						}
						
					}
					//�����н� true �̸� ����
					else if(theConfigBank.m_Option.m_bUseCh2RcmdByPass)
					{			
						theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh2RCMD BY-PASS"),m_strStateName);
						m_Timer.Start();
						nStep++;	
					}
				}
				else
				{
					 theLog[LOG_SEQUENCE].AddBuf( _T("[%s] CHANNEL2 - RCMD Fail"),m_strStateName);					
					 
					 //2017-05-19,skcho
					 Set_RCMDResult(CELL_POS_AZONE_CH2, FALSE);
					 
					 m_Timer.Start();
					 nStep++;
				}
			}
			else
			{
				//NG ��Ʈ��
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] CHANNEL2 TO NG"),m_strStateName);
				m_Timer.Start();
				nStep++;
			}
		}
		break;
	case stepContactChk: // CH1,CH2 Contact���� üũ.
		if(theConfigBank.m_Option.m_bUseContact == FALSE)
		{
			if (CellTagExist(CELL_POS_AZONE_CH1))
				AMTVacOn(VAC_ON,AMT_GetAutoJigID(ZONE_ID_A),JIG_CH_1);
			if (CellTagExist(CELL_POS_AZONE_CH2))
				AMTVacOn(VAC_ON,AMT_GetAutoJigID(ZONE_ID_A),JIG_CH_2);

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactChk Contact Skip"),m_strStateName);
			nStep = stepEnd;
			break;
		}

		//2017.10.16, JSJUNG. CONTACT CHECK�� BZONE ���� ��.START
		//nStep = stepEnd;
	//	break;
		//2017.10.16, JSJUNG. CONTACT CHECK�� BZONE ���� ��.END

		m_bRtn[0] = A_Zone_ContactChk(JIG_CH_1);
		m_bRtn[1] = A_Zone_ContactChk(JIG_CH_2);

		if(IsReturnOk())
		{	
			//2017-09-12,SKCHO JIG �������ڸ��� ��Ũ������ ����(�赿ȣ ���� ��û)
			/*if (CellTagExist(CELL_POS_AZONE_CH1))
			{
				AMTVacOn(VAC_ON,AMT_GetAutoJigID(ZONE_ID_A),JIG_CH_1);
			}
			if (CellTagExist(CELL_POS_AZONE_CH2))
			{
				AMTVacOn(VAC_ON,AMT_GetAutoJigID(ZONE_ID_A),JIG_CH_2);
			}*/


			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactChk"),m_strStateName);
			m_Timer.Start();
			nStep = stepEnd;	
		}	
		//2018-05-11,NAM
		//else if(m_Timer.Stop(FALSE) > 3.)  //5->3����
		else if(m_Timer.Stop(FALSE) >theConfigBank.m_Option.m_nContactDelay)  //5->3����
		{
			//2017-09-12,SKCHO JIG �������ڸ��� ��Ũ������ ����(�赿ȣ ���� ��û)
			/*if (CellTagExist(CELL_POS_AZONE_CH1))
			{
				AMTVacOn(VAC_ON,AMT_GetAutoJigID(ZONE_ID_A),JIG_CH_1);
			}
			if (CellTagExist(CELL_POS_AZONE_CH2))
			{
				AMTVacOn(VAC_ON,AMT_GetAutoJigID(ZONE_ID_A),JIG_CH_2);
			}
			*/
			if (theConfigBank.m_Option.m_bUseContactRetry == FALSE)
			{
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactChk Fail"),m_strStateName);
				m_Timer.Start();
				nStep = stepEnd;	
				break;
			}
			else
			{
				//A_Zone_SetContactCount(JIG_CH_1);
				//A_Zone_SetContactCount(JIG_CH_2);
			}

			//���� ����� �Ѵ� GOOD�� �ƴҰ�� 5�� ��ٸ���
			if((m_bRtn[0]==FALSE && m_bRtn[1]==FALSE))// && (m_bRtn[2]==FALSE || m_bRtn[3]==FALSE))
			{
				// ���� ����� �Ѵ� NG�϶�	
				//Reset(JIG_CH_1);
				//Reset(JIG_CH_2);
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactChk CH1,CH2 Fail"),m_strStateName);
				if (CellTagExist(CELL_POS_AZONE_CH1))
				{
					pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_AZONE_CH1));
					if(pCell->m_strContactResult_1st == _T("NG_CELL_LOADING"))
					{
						// ���� ����� �Ѵ� NG�϶�	
						Reset(JIG_CH_1);
						theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactChk CH1 Reset"),m_strStateName);
					}
				}
				if (CellTagExist(CELL_POS_AZONE_CH2))
				{
					pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_AZONE_CH2));
					if(pCell->m_strContactResult_1st == _T("NG_CELL_LOADING"))
					{
						// ���� ����� �Ѵ� NG�϶�	
						Reset(JIG_CH_2);	
						theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactChk CH2 Reset"),m_strStateName);
					}
				}
			}
			//else if((m_bRtn[0]==TRUE && m_bRtn[1]==FALSE))
			else if((m_bRtn[0]==FALSE && m_bRtn[1]==TRUE))// && (m_bRtn[2]==TRUE && m_bRtn[3]==TRUE))
			{
				// ���� ����� CH1�� NG�϶�	
				//Reset(JIG_CH_1);
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactChk CH1 Fail"),m_strStateName);
           	// ���� ����� CH1�� NG�϶�	
				if (CellTagExist(CELL_POS_AZONE_CH1))
				{
					pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_AZONE_CH1));
					if(pCell->m_strContactResult_1st == _T("NG_CELL_LOADING"))
					{
						// ���� ����� �Ѵ� NG�϶�	
						Reset(JIG_CH_1);	
						theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactChk CH1 Reset"),m_strStateName);
					}
				}	
			}
			//else if((m_bRtn[0]==FALSE && m_bRtn[1]==TRUE))
			else if((m_bRtn[0]==TRUE && m_bRtn[1]==FALSE))// && (m_bRtn[2]==FALSE || m_bRtn[3]==FALSE))
			{
				// ���� ����� CH2�� NG�϶�	
				//Reset(JIG_CH_2);
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactChk CH2 Fail"),m_strStateName);
				// ���� ����� CH2�� NG�϶�	
				if (CellTagExist(CELL_POS_AZONE_CH2))
				{
					pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_AZONE_CH2));
					if(pCell->m_strContactResult_1st == _T("NG_CELL_LOADING"))
					{
						// ���� ����� �Ѵ� NG�϶�	
						Reset(JIG_CH_2);	
						theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactChk CH2 Reset"),m_strStateName);
					}
				}	
			}

			m_Timer.Start();
	 	nStep = stepEnd;
		}
		break;
	case stepContactRetry: // RETRY Contact
		m_bRtn[0] = A_Zone_ContactChk(JIG_CH_1);
		m_bRtn[1] = A_Zone_ContactChk(JIG_CH_2);

		if (m_Timer.Stop(FALSE) > 3.)
		{
			// Cell�� ���� ���� ������ �ؼ� ����.17/04/25 HSK.
			if(m_bRtn[0] == FALSE)			
				ContactStart(JIG_CH_1);
			if(m_bRtn[1] == FALSE)			
				ContactStart(JIG_CH_2);

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactRetry"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		break;
	// ���� �����̰� �ʿ����. üũ �ϴ� �ð��� �����̴�. 17/04/25 HSK.
/*
	case stepContactDelay: // Contact DELAY
		if (m_Timer.Stop(FALSE) > 2.)  //2->5
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactDelay"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		break;
*/
	case stepEnd:	// �˻� ���� Check �� End.
		if (CellTagExist(CELL_POS_AZONE_CH1))
			m_bRtn[0] = AMTVacChk(AMT_GetAutoJigID(ZONE_ID_A),JIG_CH_1);
		else
			m_bRtn[0] = TRUE;

		if (CellTagExist(CELL_POS_AZONE_CH2))
			m_bRtn[1] = AMTVacChk(AMT_GetAutoJigID(ZONE_ID_A),JIG_CH_2);
		else
			m_bRtn[1] = TRUE;


		//2017.10.16, JSJUNG. CONTACT CHECK�� BZONE ���� ��.START
		if(IsReturnOk())
		{	
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"),m_strStateName);
			AZoneContactEndTime();
			SetZoneEnd(ZONE_ID_A, TRUE);
			TactUpdate(AZONE_TACT);

			theProcBank.m_bShowCh1TrackInChk = FALSE;
			theProcBank.m_bShowCh2TrackInChk = FALSE;
			//20170201 byskcho	
			theConfigBank.m_Option.m_bUseCh1ReqByPass = FALSE;
			theConfigBank.m_Option.m_bUseCh2ReqByPass = FALSE;
			theConfigBank.m_Option.m_bUseCh1RcmdByPass = FALSE;
			theConfigBank.m_Option.m_bUseCh2RcmdByPass = FALSE;

			theProcBank.m_nCH1CellInfoRetryStep = 0;
			theProcBank.m_nCH2CellInfoRetryStep = 0;
			theProcBank.m_nCH1JobProcessRetryStep = 0;
			theProcBank.m_nCH2JobProcessRetryStep = 0;

			//2017-09-26,SKCHO, ���������� ó���Ǹ� ������ CELL INFO, JOB PROCESS ������ �����.
			theUnitFunc.DeleteResult(CELL_POS_AZONE_CH1,CELL_POS_AZONE_CH2);

			if (CellTagExist(CELL_POS_AZONE_CH1))
			{
				A_Zone_SetClass(JIG_CH_1);
				AMTVacOn(VAC_ON,AMT_GetAutoJigID(ZONE_ID_A),JIG_CH_1);
			}
			if (CellTagExist(CELL_POS_AZONE_CH2))
			{
				A_Zone_SetClass(JIG_CH_2);
				AMTVacOn(VAC_ON,AMT_GetAutoJigID(ZONE_ID_A),JIG_CH_2);
			}

			nStep = stepIdle;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"),m_strStateName);
			AZoneContactEndTime();
			SetZoneEnd(ZONE_ID_A, TRUE);
			TactUpdate(AZONE_TACT);

			theProcBank.m_bShowCh1TrackInChk = FALSE;
			theProcBank.m_bShowCh2TrackInChk = FALSE;

			theProcBank.m_nCH1CellInfoRetryStep = 0;
			theProcBank.m_nCH2CellInfoRetryStep = 0;
			theProcBank.m_nCH1JobProcessRetryStep = 0;
			theProcBank.m_nCH2JobProcessRetryStep = 0;

			//20170201 byskcho	
			theConfigBank.m_Option.m_bUseCh1ReqByPass = FALSE;
			theConfigBank.m_Option.m_bUseCh2ReqByPass = FALSE;
			theConfigBank.m_Option.m_bUseCh1RcmdByPass = FALSE;
			theConfigBank.m_Option.m_bUseCh2RcmdByPass = FALSE;

			if (CellTagExist(CELL_POS_AZONE_CH1))
			{
				A_Zone_SetClass(JIG_CH_1);
				AMTVacOn(VAC_ON,AMT_GetAutoJigID(ZONE_ID_A),JIG_CH_1);
			}
			if (CellTagExist(CELL_POS_AZONE_CH2))
			{
				A_Zone_SetClass(JIG_CH_2);
				AMTVacOn(VAC_ON,AMT_GetAutoJigID(ZONE_ID_A),JIG_CH_2);
			}
			
			nStep = stepIdle;
		}
		break;
		//2017.10.16, JSJUNG. CONTACT CHECK�� BZONE ���� ��.END
		//2017.10.16, JSJUNG. �� �����δ� �� ������.
		/*
		// ��¥�� ���� üũ�� ���� �ؾ� �Ѵ�. ���ǹ� ���� 2017/04/25 HSK
//		if ((A_Zone_GetContactCount(JIG_CH_1) != 0) || (A_Zone_GetContactCount(JIG_CH_2) != 0))
//		{
		m_bRtn[2] = A_Zone_ContactChk(JIG_CH_1);
		m_bRtn[3] = A_Zone_ContactChk(JIG_CH_2);
//		}
		//20170730 JSJUNG, ���׺� ���� NG count/3ȸ ���� : JigContactNGFlag ���º��� �˶�
		m_bRtn[4] = !theProcBank.m_bJigContactNGFlag;

		//2017-08-01,JSJUNG, ���׺� ���� NG count/3ȸ ���� : JigContactNGFlag ���º��� �ʱ�ȭ
		theProcBank.m_bJigContactNGFlag = FALSE;
		

		if(IsReturnOk())
		{	
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"),m_strStateName);
			AZoneContactEndTime();
			SetZoneEnd(ZONE_ID_A, TRUE);
			TactUpdate(AZONE_TACT);

			theProcBank.m_bShowCh1TrackInChk = FALSE;
			theProcBank.m_bShowCh2TrackInChk = FALSE;
			//20170201 byskcho	
			theConfigBank.m_Option.m_bUseCh1ReqByPass = FALSE;
			theConfigBank.m_Option.m_bUseCh2ReqByPass = FALSE;
			theConfigBank.m_Option.m_bUseCh1RcmdByPass = FALSE;
			theConfigBank.m_Option.m_bUseCh2RcmdByPass = FALSE;

			theProcBank.m_nCH1CellInfoRetryStep = 0;
			theProcBank.m_nCH2CellInfoRetryStep = 0;
			theProcBank.m_nCH1JobProcessRetryStep = 0;
			theProcBank.m_nCH2JobProcessRetryStep = 0;

			//2017-09-26,SKCHO, ���������� ó���Ǹ� ������ CELL INFO, JOB PROCESS ������ �����.
			theUnitFunc.DeleteResult(CELL_POS_AZONE_CH1,CELL_POS_AZONE_CH2);

			if (CellTagExist(CELL_POS_AZONE_CH1))
			{
				A_Zone_SetClass(JIG_CH_1);
				AMTVacOn(VAC_ON,AMT_GetAutoJigID(ZONE_ID_A),JIG_CH_1);
			}
			if (CellTagExist(CELL_POS_AZONE_CH2))
			{
				A_Zone_SetClass(JIG_CH_2);
				AMTVacOn(VAC_ON,AMT_GetAutoJigID(ZONE_ID_A),JIG_CH_2);
			}

			//2017-04-08, �Լ��� ������ ��
			//2017-04-07, skcho, CONTACT NG�� ��� �˻� ���� ����
			//���� ���� �����Ͽ� ������ ��Ȯ���� �ʾ� ����
			if (CellTagExist(CELL_POS_AZONE_CH1))
			{
				pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_AZONE_CH1));
				if (pCell->m_strContactResult == _T("NG"))
				{
					Set_InspectSkip(CELL_POS_AZONE_CH1 , TRUE);
				}
			}
			if (CellTagExist(CELL_POS_AZONE_CH2))
			{
				pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_AZONE_CH2));
				if (pCell->m_strContactResult == _T("NG"))
				{
					Set_InspectSkip(CELL_POS_AZONE_CH2 , TRUE);
				}
			}
			nStep = stepIdle;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"),m_strStateName);
			AZoneContactEndTime();
			SetZoneEnd(ZONE_ID_A, TRUE);
			TactUpdate(AZONE_TACT);

			theProcBank.m_bShowCh1TrackInChk = FALSE;
			theProcBank.m_bShowCh2TrackInChk = FALSE;

			theProcBank.m_nCH1CellInfoRetryStep = 0;
			theProcBank.m_nCH2CellInfoRetryStep = 0;
			theProcBank.m_nCH1JobProcessRetryStep = 0;
			theProcBank.m_nCH2JobProcessRetryStep = 0;

			//20170201 byskcho	
			theConfigBank.m_Option.m_bUseCh1ReqByPass = FALSE;
			theConfigBank.m_Option.m_bUseCh2ReqByPass = FALSE;
			theConfigBank.m_Option.m_bUseCh1RcmdByPass = FALSE;
			theConfigBank.m_Option.m_bUseCh2RcmdByPass = FALSE;

			if (CellTagExist(CELL_POS_AZONE_CH1))
			{
				A_Zone_SetClass(JIG_CH_1);
				AMTVacOn(VAC_ON,AMT_GetAutoJigID(ZONE_ID_A),JIG_CH_1);
			}
			if (CellTagExist(CELL_POS_AZONE_CH2))
			{
				A_Zone_SetClass(JIG_CH_2);
				AMTVacOn(VAC_ON,AMT_GetAutoJigID(ZONE_ID_A),JIG_CH_2);
			}
			
			//2017-04-08, �Լ��� ������ ��
			//2017-04-07, skcho, CONTACT NG�� ��� �˻� ���� ����
			//���� ���� �����Ͽ� ������ ��Ȯ���� �ʾ� ����
			if (CellTagExist(CELL_POS_AZONE_CH1))
			{
				pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_AZONE_CH1));
				if (pCell->m_strContactResult == _T("NG"))
				{
					Set_InspectSkip(CELL_POS_AZONE_CH1 , TRUE);
				}
			}
			
			if (CellTagExist(CELL_POS_AZONE_CH2))
			{
				pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_AZONE_CH2));
				if (pCell->m_strContactResult == _T("NG"))
				{
					Set_InspectSkip(CELL_POS_AZONE_CH2 , TRUE);
				}
			}

			nStep = stepIdle;

			if (m_bRtn[0] == FALSE)			SetError(ALARM_AZONE_JIG_CH1_VAC);
			else if (m_bRtn[1] == FALSE)	SetError(ALARM_AZONE_JIG_CH2_VAC);
			else if (m_bRtn[4] == FALSE)
			{
				//20170730 JSJUNG, ���׺� ���� NG count/3ȸ ���� : JIG�� ä�ΰ� �����Ͽ� �˶�ǥ��
				if(theProcBank.m_JigContactNGChName == 0)
					SetError(ALARM_JIG_A_CH1_CONTACT_CONTINUE_NG);
				else if(theProcBank.m_JigContactNGChName == 1)
					SetError(ALARM_JIG_A_CH2_CONTACT_CONTINUE_NG);
				else if(theProcBank.m_JigContactNGChName == 2)
					SetError(ALARM_JIG_B_CH1_CONTACT_CONTINUE_NG);
				else if(theProcBank.m_JigContactNGChName == 3)
					SetError(ALARM_JIG_B_CH2_CONTACT_CONTINUE_NG);
				else if(theProcBank.m_JigContactNGChName == 4)
					SetError(ALARM_JIG_C_CH1_CONTACT_CONTINUE_NG);
				else if(theProcBank.m_JigContactNGChName == 5)
					SetError(ALARM_JIG_C_CH2_CONTACT_CONTINUE_NG);
				else if(theProcBank.m_JigContactNGChName == 6)
					SetError(ALARM_JIG_D_CH1_CONTACT_CONTINUE_NG);
				else if(theProcBank.m_JigContactNGChName == 7)
					SetError(ALARM_JIG_D_CH2_CONTACT_CONTINUE_NG);
			}
		}
		break;
		*/
	}

	m_nStep = nStep;

	return nStep;
}


int CStateBZone::Run()
{
	//2017.10.16, JSJUNG. BZone Contact / Recontact �߰� �� ����.
	CCellInfo *pCell;

	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{
	//2017.10.16, JSJUNG. BZone Contact / Recontact �߰� �� ����.
	case stepStart:	// BZone Contact Check
		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"),m_strStateName);
		BZoneAlignStartTime();

		if((CellTagExist(CELL_POS_BZONE_CH1) == FALSE && CellTagExist(CELL_POS_BZONE_CH2) == FALSE)
			|| (theConfigBank.m_Option.m_bUseVIAlign == FALSE))
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] B Zone Cell Not Exist."),m_strStateName);

			BZoneAlignEndTime();
			SetZoneEnd(ZONE_ID_B,TRUE);
			TactUpdate(BZONE_TACT);
			nStep = stepIdle;

			break;
		}
		m_Timer.Start();
		nStep++;
		
		break;
	
	//2017.10.16, JSJUNG. BZone Contact / Recontact �߰� �� ����.
	case stepContactChk:// BZone Contact Check
		m_bRtn[0] = B_Zone_ContactChk(JIG_CH_1);
		m_bRtn[1] = B_Zone_ContactChk(JIG_CH_2);

		if(IsReturnOk())
		{	
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactChk"),m_strStateName);
			m_Timer.Start();
			nStep = stepAlignMove;	
		}		
		//2018-05-11,NAM
		//else if(m_Timer.Stop(FALSE) > 3.)  //5->3����
		else if(m_Timer.Stop(FALSE) > theConfigBank.m_Option.m_nContactDelay)  //5->3����
		{
			if (theConfigBank.m_Option.m_bUseContactRetry == FALSE)
			{
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactChk Fail"),m_strStateName);
				m_Timer.Start();
				nStep = stepAlignMove;	
				break;
			}
			else
			{
				//2018-05-16,SKCHO,����
				//--B_Zone_SetContactCount(JIG_CH_1);
				//--B_Zone_SetContactCount(JIG_CH_2);
			}

			//���� ����� �Ѵ� GOOD�� �ƴҰ�� 5�� ��ٸ���
			if((m_bRtn[0]==FALSE && m_bRtn[1]==FALSE))// && (m_bRtn[2]==FALSE || m_bRtn[3]==FALSE))
			{
				// ���� ����� �Ѵ� NG�϶�	
				//Reset_BZone(JIG_CH_1);
			//	Reset_BZone(JIG_CH_2);
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactChk CH1,CH2 Fail"),m_strStateName);
				if (CellTagExist(CELL_POS_BZONE_CH1))
				{
					pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_BZONE_CH1));
					if(pCell->m_strContactResult_1st == _T("NG_CELL_LOADING"))
					{
						// ���� ����� �Ѵ� NG�϶�	
						Reset_BZone(JIG_CH_1);
						theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactChk CH1 Reset"),m_strStateName);
					}
				}
				if (CellTagExist(CELL_POS_BZONE_CH2))
				{
					pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_BZONE_CH2));
					if(pCell->m_strContactResult_1st == _T("NG_CELL_LOADING"))
					{
						// ���� ����� �Ѵ� NG�϶�	
						Reset_BZone(JIG_CH_2);	
						theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactChk CH2 Reset"),m_strStateName);
					}
				}
			}
			//else if((m_bRtn[0]==TRUE && m_bRtn[1]==FALSE))
			else if((m_bRtn[0]==FALSE && m_bRtn[1]==TRUE))// && (m_bRtn[2]==TRUE && m_bRtn[3]==TRUE))
			{
					theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactChk CH1 Fail"),m_strStateName);
				// ���� ����� CH1�� NG�϶�	
				if (CellTagExist(CELL_POS_BZONE_CH1))
				{
					pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_BZONE_CH1));
					if(pCell->m_strContactResult_1st == _T("NG_CELL_LOADING"))
					{
						// ���� ����� �Ѵ� NG�϶�	
						Reset_BZone(JIG_CH_1);	
						theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactChk CH1 Reset"),m_strStateName);
					}
				}				
				
			}
			//else if((m_bRtn[0]==FALSE && m_bRtn[1]==TRUE))
			else if((m_bRtn[0]==TRUE && m_bRtn[1]==FALSE))// && (m_bRtn[2]==FALSE || m_bRtn[3]==FALSE))
			{
				// ���� ����� CH2�� NG�϶�	
				//Reset_BZone(JIG_CH_2);
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactChk CH2 Fail"),m_strStateName);
				// ���� ����� CH2�� NG�϶�	
				if (CellTagExist(CELL_POS_BZONE_CH2))
				{
					pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_BZONE_CH2));
					if(pCell->m_strContactResult_1st == _T("NG_CELL_LOADING"))
					{
						// ���� ����� �Ѵ� NG�϶�	
						Reset_BZone(JIG_CH_2);	
						theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactChk CH2 Reset"),m_strStateName);
					}
				}			
			}

			m_Timer.Start();
			nStep++;
		}
		break;

	case stepContactRetry: // RETRY Contact
		m_bRtn[0] = B_Zone_ContactChk(JIG_CH_1);
		m_bRtn[1] = B_Zone_ContactChk(JIG_CH_2);

		if (m_Timer.Stop(FALSE) > 3.)
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactRetry"),m_strStateName);
			if(m_bRtn[0] == FALSE)
			{
				//DIC_CHECK FAIL �̸� RECONTACT ����
				if (CellTagExist(CELL_POS_BZONE_CH1))
				{
					pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_BZONE_CH1));
					if(pCell->m_strContactResult_1st == _T("NG_CELL_LOADING"))
					{				
						//2018-05-19,MGYUN,�� ���� �� �޼���4�� NONE_CELL�� �ʱ�ȭ �� CELL_LOADING ������
						B_Zone_SetContactCount(JIG_CH_1);
						ContactStart_BZone(JIG_CH_1);
						theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactRetry_CH1"),m_strStateName);
					}
					else
					{
						theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactRetry_CH1_Cancel"),m_strStateName);
					}
				}
			}
			if(m_bRtn[1] == FALSE)
			{
				//DIC_CHECK FAIL �̸� RECONTACT ����
				if (CellTagExist(CELL_POS_BZONE_CH2))
				{
					pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_BZONE_CH2));
					if(pCell->m_strContactResult_1st == _T("NG_CELL_LOADING"))
					{				
						//2018-05-19,�� ���� �� �޼���4�� NONE_CELL�� �ʱ�ȭ �� CELL_LOADING ������
						B_Zone_SetContactCount(JIG_CH_2);
						ContactStart_BZone(JIG_CH_2);
						theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactRetry_CH2"),m_strStateName);
					}
					else
					{
						theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactRetry_CH2_Cancel"),m_strStateName);
					}
				}
			}

			m_Timer.Start();
			nStep++;
		}
		break;

	//case stepContactDelay: // Contact DELAY
	//	if (m_Timer.Stop(FALSE) > 8.)  //2->5 ->8�ʷ� ���� 2018-05-17,SKCHO
	//	{
	//		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactDelay"),m_strStateName);
	//		m_Timer.Start();
	//		nStep++;
	//	}
	//	break;

	//2017.10.16, JSJUNG. BZone Contact / Recontact �߰� �� ����.
	case stepAlignMove:	//case stepStart(����) VT Align �ؾ��ϴ� Cell ���� Ȯ�� �� Align ���� Left ��ũ ��ġ�� �̵�
		
		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepAlignMove"),m_strStateName);
		//BZoneAlignStartTime();

		//if((CellTagExist(CELL_POS_BZONE_CH1) == FALSE && CellTagExist(CELL_POS_BZONE_CH2) == FALSE)
		//	|| (theConfigBank.m_Option.m_bUseVIAlign == FALSE))
		//{
		//	theLog[LOG_SEQUENCE].AddBuf( _T("[%s] B Zone Cell Not Exist."),m_strStateName);

		//	BZoneAlignEndTime();
		//	SetZoneEnd(ZONE_ID_B,TRUE);
		//	TactUpdate(BZONE_TACT);
		//	nStep = stepIdle;

		//	break;
		//}

		if(CellTagExist(CELL_POS_BZONE_CH1) == TRUE)
		{
			B_Zone_Cam_X(JIG_CH_1, CAMPOS_LEFT);
			B_Zone_Cam_Y(JIG_CH_1, CAMPOS_LEFT);

			m_Timer.Start();
			nStep++;
		}
		else if(CellTagExist(CELL_POS_BZONE_CH2) == TRUE)
		{
			B_Zone_Cam_X(JIG_CH_2, CAMPOS_LEFT);
			B_Zone_Cam_Y(JIG_CH_2, CAMPOS_LEFT);

			m_Timer.Start();
			nStep = stepCh2LeftPosChk;
		}
		else
		{
			B_Zone_Cam_X_Ready();
			B_Zone_Cam_Y_Ready();

			m_Timer.Start();
			nStep = stepEnd;
		}
		break;
	case stepCh1LeftPosChk: // ���� �̵� üũ �� Ch1 Left ���� ����� ��û.
		m_bRtn[0] = B_Zone_Cam_X_Check(JIG_CH_1, CAMPOS_LEFT);
		m_bRtn[1] = B_Zone_Cam_Y_Check(JIG_CH_1, CAMPOS_LEFT);
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh1LeftPosChk"),m_strStateName);

			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)		SetError(ALARM_BZONE_CH1_CAM_X_LEFT_POS);
			else if(m_bRtn[1] == FALSE)	SetError(ALARM_BZONE_CH1_CAM_Y_LEFT_POS);
		}
		break;
	case stepCh1LeftPosStable:
		if (m_Timer.Stop(FALSE) > 0.3)
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh1LeftPosStable"),m_strStateName);

			// B zone 1Ch Left ��ũ �׷�.
			B_Zone_Align_Grab(JIG_CH_1, TRUE);		//Align  request
			m_Timer.Start();
			nStep++;
		}
		break;
	case stepCh1LeftAlignChk:	 // ����� ���� ���� Ȯ�� �� Ch1 Right ��ũ �̵�.
		m_bRtn[0] = B_Zone_Align_Check();
		if(IsReturnOk())
		{
			if(theProcBank.m_nVI_MITAlignResult == ALIGN_RESULT_GOOD)
			{
				CellData_SetVTAlignScore(JIG_CH_1, E_MARK_LEFT, theProcBank.m_dVI_MITAlignScore);
			}
			else
			{
				if(theProcBank.m_nVI_MITAlignResult == ALIGN_RESULT_FAIL)
				{
					CellData_SetVTAlignScore(JIG_CH_1, E_MARK_LEFT, theProcBank.m_dVI_MITAlignScore);
					CellData_SetVTAlignth(JIG_CH_1, 0., theProcBank.m_nVI_MITAlignResult);
				}
				else
					theLog[LOG_SEQUENCE].AddBuf(_T("[%s] stepCh1LeftAlignChk Align result SKIP"),m_strStateName);
					
			}

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh1LeftAlignChk"),m_strStateName);

			B_Zone_Cam_X(JIG_CH_1, CAMPOS_RIGHT);
			B_Zone_Cam_Y(JIG_CH_1, CAMPOS_RIGHT);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)	SetError(ALARM_BZONE_CH1_LEFT_ALIGN_RESULT);
		}
		break;
	case stepCh1RightPosChk:	// ���� �̵� üũ �� Ch1 Right ���� ����� ��û
		m_bRtn[0] = B_Zone_Cam_X_Check(JIG_CH_1,CAMPOS_RIGHT);
		m_bRtn[1] = B_Zone_Cam_Y_Check(JIG_CH_1,CAMPOS_RIGHT);
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh1RightPosChk"),m_strStateName);

			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)		SetError(ALARM_BZONE_CH1_CAM_X_RIGHT_POS);
			else if(m_bRtn[1] == FALSE)	SetError(ALARM_BZONE_CH1_CAM_Y_RIGHT_POS);
		}
		break;
	case stepCh1RightPosStable:
		if (m_Timer.Stop(FALSE) > 0.3)
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh1RightPosStable"),m_strStateName);
			// B zone 1Ch Right ��ũ �׷�.
			B_Zone_Align_Grab(JIG_CH_1, FALSE);		//Align  request
			m_Timer.Start();
			nStep++;
		}
		break;
	case stepCh1RightAlignChk:	// ����� ���� ���� Ȯ�� �� Ch2 Left ���� ����� ��ġ�� �̵�
		m_bRtn[0] = B_Zone_Align_Check();
		if(IsReturnOk())
		{
			if(theProcBank.m_nVI_MITAlignResult == ALIGN_RESULT_GOOD)
			{
				// Right ��ũ �׷� �Ŀ� Th ����ġ�� ����´�. �ش� ����ġ Cell data�� ����.
				CellData_SetVTAlignScore(JIG_CH_1, E_MARK_RIGHT, theProcBank.m_dVI_MITAlignScore);
				// �����Ǵ� ������ �ݴ�� -1.0�����ش�.
				CellData_SetVTAlignth(JIG_CH_1, -1.0 * theProcBank.m_dVI_MITAlignTH, theProcBank.m_nVI_MITAlignResult);
			}
			else
			{
				if(theProcBank.m_nVI_MITAlignResult == ALIGN_RESULT_FAIL)
				{
					CellData_SetVTAlignScore(JIG_CH_1, E_MARK_RIGHT, theProcBank.m_dVI_MITAlignScore);
					CellData_SetVTAlignth(JIG_CH_1, 0., theProcBank.m_nVI_MITAlignResult);
				}
				else
					theLog[LOG_SEQUENCE].AddBuf(_T("[%s] stepCh1RightAlignChk Align result SKIP"),m_strStateName);

			}
			::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_CZONE_UPDATE, 0,1); //2017-03-03,skcho , ����ȭ�鿡 ����� ���� ����

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh1RightAlignChk TH:%f"),m_strStateName,theProcBank.m_dVI_MITAlignTH);

			if(CellTagExist(CELL_POS_BZONE_CH2) == TRUE)
			{
				B_Zone_Cam_X(JIG_CH_2, CAMPOS_LEFT);
				B_Zone_Cam_Y(JIG_CH_2, CAMPOS_LEFT);

				m_Timer.Start();
				nStep++;
			}
			else
			{
				B_Zone_Cam_X_Ready();
				B_Zone_Cam_Y_Ready();

				m_Timer.Start();
				nStep = stepEnd;
			}

		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)	SetError(ALARM_BZONE_CH1_RIGHT_ALIGN_RESULT);
		}
		break;
	case stepCh2LeftPosChk:	// ���� �̵� üũ �� Ch2 Left ���� ����� ��û.
		m_bRtn[0] = B_Zone_Cam_X_Check(JIG_CH_2, CAMPOS_LEFT);
		m_bRtn[1] = B_Zone_Cam_Y_Check(JIG_CH_2, CAMPOS_LEFT);
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh2LeftPosChk"),m_strStateName);
			//2017-07-04,SKCHO, �ι� �����Ͽ� ������
			//--B_Zone_Align_Grab(JIG_CH_2, TRUE);		//Align  request
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)		SetError(ALARM_BZONE_CH2_CAM_X_LEFT_POS);
			else if(m_bRtn[1] == FALSE)	SetError(ALARM_BZONE_CH1_CAM_Y_LEFT_POS);
		}
		break;
	case stepCh2LeftPosStable:
		if (m_Timer.Stop(FALSE) > 0.3)
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh2LeftPosStable"),m_strStateName);

			B_Zone_Align_Grab(JIG_CH_2, TRUE);		//Align  request
			m_Timer.Start();
			nStep++;
		}
		break;
	case stepCh2LeftAlignChk:	// ����� ���� ���� Ȯ�� �� Ch2 Right ��ũ �̵�.
		m_bRtn[0] = B_Zone_Align_Check();
		if(IsReturnOk())
		{
			if(theProcBank.m_nVI_MITAlignResult == ALIGN_RESULT_GOOD)
			{
				CellData_SetVTAlignScore(JIG_CH_2, E_MARK_LEFT, theProcBank.m_dVI_MITAlignScore);
			}
			else
			{
				if(theProcBank.m_nVI_MITAlignResult == ALIGN_RESULT_FAIL)
				{
					CellData_SetVTAlignScore(JIG_CH_2, E_MARK_LEFT, theProcBank.m_dVI_MITAlignScore);
					CellData_SetVTAlignth(JIG_CH_2, 0., theProcBank.m_nVI_MITAlignResult);
				}
				else
					theLog[LOG_SEQUENCE].AddBuf(_T("[%s] stepCh2LeftAlignChk Align result SKIP"),m_strStateName);

			}

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh2LeftAlignChk"),m_strStateName);

			B_Zone_Cam_X(JIG_CH_2, CAMPOS_RIGHT);
			B_Zone_Cam_Y(JIG_CH_2, CAMPOS_RIGHT);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)	SetError(ALARM_BZONE_CH2_LEFT_ALIGN_RESULT);
		}
		break;
	case stepCh2RightPosChk:	 // ���� �̵� üũ �� Ch2 Right ���� ����� ��û
		m_bRtn[0] = B_Zone_Cam_X_Check(JIG_CH_2, CAMPOS_RIGHT);
		m_bRtn[1] = B_Zone_Cam_Y_Check(JIG_CH_2, CAMPOS_RIGHT);
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh2RightPosChk"),m_strStateName);

			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)		SetError(ALARM_BZONE_CH2_CAM_X_RIGHT_POS);
			else if(m_bRtn[1] == FALSE)	SetError(ALARM_BZONE_CH2_CAM_Y_RIGHT_POS);
		}
		break;
	case stepCh2RightPosStable:
		if (m_Timer.Stop(FALSE) > 0.3)
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh2RightPosStable"),m_strStateName);

			B_Zone_Align_Grab(JIG_CH_2, FALSE);		//Align  request
			m_Timer.Start();
			nStep++;
		}
		break;
	case stepCh2RightAlignChk:	 // ����� ���� ���� Ȯ�� �� �����ġ �̵�.
		m_bRtn[0] = B_Zone_Align_Check();
		if(IsReturnOk())
		{
			if(theProcBank.m_nVI_MITAlignResult == ALIGN_RESULT_GOOD)
			{
				CellData_SetVTAlignScore(JIG_CH_2, E_MARK_RIGHT, theProcBank.m_dVI_MITAlignScore);
				CellData_SetVTAlignth(JIG_CH_2, theProcBank.m_dVI_MITAlignTH, theProcBank.m_nVI_MITAlignResult);
			}
			else
			{
				if(theProcBank.m_nVI_MITAlignResult == ALIGN_RESULT_FAIL)
				{
					CellData_SetVTAlignScore(JIG_CH_2, E_MARK_RIGHT, theProcBank.m_dVI_MITAlignScore);
					CellData_SetVTAlignth(JIG_CH_2, 0., theProcBank.m_nVI_MITAlignResult);
				}
				else
					theLog[LOG_SEQUENCE].AddBuf(_T("[%s] stepCh2RightAlignChk Align result SKIP"),m_strStateName);

			}

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh2RightAlignChk TH:%f"),m_strStateName,theProcBank.m_dVI_MITAlignTH);
			
			::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_CZONE_UPDATE, 0,2); //2017-03-03,skcho , ����ȭ�鿡 ����� ���� ����
		
			B_Zone_Cam_X_Ready();
			B_Zone_Cam_Y_Ready();
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)	SetError(ALARM_BZONE_CH2_RIGHT_ALIGN_RESULT);
		}
		break;

	//2017.10.16, JSJUNG. BZone Contact / Recontact �߰� �� ����.
	case stepContactEnd:	// �˻� ���� Check �� End.
		m_bRtn[0] = B_Zone_ContactChk(JIG_CH_1);
		m_bRtn[1] = B_Zone_ContactChk(JIG_CH_2);
		//20170730 JSJUNG, ���׺� ���� NG count/3ȸ ���� : JigContactNGFlag ���º��� �˶�
		m_bRtn[2] = !theProcBank.m_bJigContactNGFlag;

		//2017-08-01,JSJUNG, ���׺� ���� NG count/3ȸ ���� : JigContactNGFlag ���º��� �ʱ�ȭ
		theProcBank.m_bJigContactNGFlag = FALSE;

		if(IsReturnOk())
		{	
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactEnd"),m_strStateName);

			//2017.10.17, JSJUNG.ContactNG Count �и�
			NGCountChk(JIG_CH_1);
			NGCountChk(JIG_CH_2);
			
			//2017-04-07, skcho, CONTACT NG�� ��� �˻� ���� ����
			 if (CellTagExist(CELL_POS_BZONE_CH1))
			{
				pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_BZONE_CH1));
				//2018-08-03,NAM,when Contact Result is NONE_CELL
				//if (pCell->m_strContactResult == _T("NG_CELL_LOADING") || pCell->m_strContactResult == _T("NG_DIC_CHECK"))
				if (pCell->m_strContactResult == _T("NG_CELL_LOADING") || pCell->m_strContactResult == _T("NG_DIC_CHECK") 
					|| pCell->m_strContactResult == _T("NONE_CELL"))
				{
					Set_InspectSkip(CELL_POS_BZONE_CH1 , TRUE);
				}
				//2017-12-08,SKCHO, �߰�
				B_Zone_SetClass(JIG_CH_1);
			}
			if (CellTagExist(CELL_POS_BZONE_CH2))
			{
				pCell =theCellBank.GetCellInfo((CELL_POS)(CELL_POS_BZONE_CH2));
				//2018-08-03,NAM,when Contact Result is NONE_CELL
				//if (pCell->m_strContactResult == _T("NG_CELL_LOADING") || pCell->m_strContactResult == _T("NG_DIC_CHECK"))
				if (pCell->m_strContactResult == _T("NG_CELL_LOADING") || pCell->m_strContactResult == _T("NG_DIC_CHECK") 
					|| pCell->m_strContactResult == _T("NONE_CELL"))
				{
					Set_InspectSkip(CELL_POS_BZONE_CH2 , TRUE);
				}

				//2017-12-08,SKCHO, �߰�
				B_Zone_SetClass(JIG_CH_2);
			}
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > theConfigBank.m_Option.m_nContactDelay)
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactEnd"),m_strStateName);
			//2017.10.17, JSJUNG.ContactNG Count �и�
			NGCountChk(JIG_CH_1);
			NGCountChk(JIG_CH_2);
			//2017-04-07, skcho, CONTACT NG�� ��� �˻� ���� ����
			if (CellTagExist(CELL_POS_BZONE_CH1))
			{
				pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_BZONE_CH1));
				//2018-08-03,NAM,when Contact Result is NONE_CELL
				//if (pCell->m_strContactResult == _T("NG_CELL_LOADING") || pCell->m_strContactResult == _T("NG_DIC_CHECK"))
				if (pCell->m_strContactResult == _T("NG_CELL_LOADING") || pCell->m_strContactResult == _T("NG_DIC_CHECK") 
					|| pCell->m_strContactResult == _T("NONE_CELL"))
				{
					Set_InspectSkip(CELL_POS_BZONE_CH1 , TRUE);
				}
				//2017-12-08,SKCHO, �߰�
				B_Zone_SetClass(JIG_CH_1);

			}
			if (CellTagExist(CELL_POS_BZONE_CH2))
			{
				pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_BZONE_CH2));
				//2018-08-03,NAM,when Contact Result is NONE_CELL
				//if (pCell->m_strContactResult == _T("NG_CELL_LOADING") || pCell->m_strContactResult == _T("NG_DIC_CHECK"))
				if (pCell->m_strContactResult == _T("NG_CELL_LOADING") || pCell->m_strContactResult == _T("NG_DIC_CHECK") 
					|| pCell->m_strContactResult == _T("NONE_CELL"))
				{
					Set_InspectSkip(CELL_POS_BZONE_CH2 , TRUE);
				}
				//2017-12-08,SKCHO, �߰�
				B_Zone_SetClass(JIG_CH_2);
			}

			if (m_bRtn[2] == FALSE)
			{
				//20170730 JSJUNG, ���׺� ���� NG count/3ȸ ���� : JIG�� ä�ΰ� �����Ͽ� �˶�ǥ��
				if(theProcBank.m_JigContactNGChName == 0)
					SetError(ALARM_JIG_A_CH1_CONTACT_CONTINUE_NG);
				else if(theProcBank.m_JigContactNGChName == 1)
					SetError(ALARM_JIG_A_CH2_CONTACT_CONTINUE_NG);
				else if(theProcBank.m_JigContactNGChName == 2)
					SetError(ALARM_JIG_B_CH1_CONTACT_CONTINUE_NG);
				else if(theProcBank.m_JigContactNGChName == 3)
					SetError(ALARM_JIG_B_CH2_CONTACT_CONTINUE_NG);
				else if(theProcBank.m_JigContactNGChName == 4)
					SetError(ALARM_JIG_C_CH1_CONTACT_CONTINUE_NG);
				else if(theProcBank.m_JigContactNGChName == 5)
					SetError(ALARM_JIG_C_CH2_CONTACT_CONTINUE_NG);
				else if(theProcBank.m_JigContactNGChName == 6)
					SetError(ALARM_JIG_D_CH1_CONTACT_CONTINUE_NG);
				else if(theProcBank.m_JigContactNGChName == 7)
					SetError(ALARM_JIG_D_CH2_CONTACT_CONTINUE_NG);
			}
			m_Timer.Start();
			nStep++;
		}
		break;

	case stepEnd:	// ��� ��ġ �̵� Ȯ�� �� End.
		m_bRtn[0] = B_Zone_Cam_X_Ready_Chk();
		m_bRtn[1] = B_Zone_Cam_Y_Ready_Chk();
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"),m_strStateName);

			BZoneAlignEndTime();
			SetZoneEnd(ZONE_ID_B,TRUE);
			TactUpdate(BZONE_TACT);
			nStep = stepIdle;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)		SetError(ALARM_BZONE_ALIGN_CAM_X_READY_POS);
			else if(m_bRtn[1] == FALSE)	SetError(ALARM_BZONE_ALIGN_CAM_Y_READY_POS);
		}
		break;
	}

	m_nStep = nStep;

	return nStep;
}


int CStateCZone::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{
	case stepStart:	// ���� ���� �� �˻� ��ġ Ȯ�� �� ���� �� �̵�.
		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart"),m_strStateName);
		if((CellTagExist(CELL_POS_CZONE_CH1) == FALSE && CellTagExist(CELL_POS_CZONE_CH2) == FALSE))
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] C Zone Cell Not Exist."),m_strStateName);

			SetZoneEnd(ZONE_ID_C,TRUE);
			TactUpdate(CZONE_TACT);
			nStep = stepIdle;

			break;
		}
			
		//20170203 byskcho
		//FALSE  �̸� SKIP �ǹ̷� ���
		if (!theConfigBank.m_Option.m_bUseVIInsp)
		{
			SetCZoneResultSkip(JIG_CH_1);
			SetCZoneResultSkip(JIG_CH_2);
			SetZoneEnd(ZONE_ID_C,TRUE);
			TactUpdate(CZONE_TACT);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] VI Insp Skip."),m_strStateName);
			nStep = stepIdle;
			break;
		}

		C_Zone_CamX();

		C_Zone_LCam_Focus();
		C_Zone_LCam_Theta();
		C_Zone_LCamZ();
			
		C_Zone_RCam_Focus();
		C_Zone_RCam_Theta();
		C_Zone_RCamZ();	

		m_Timer.Start();
		nStep++;
		break;
	case stepPosChk:// ���� ���� �� �˻� ��ġ Ȯ�� �� Theta Offset �� �̵�.
		m_bRtn[0] = TheachMotionChk(m_nThreadID,TEACH_PARAM::TT_INSP_CAM_X);
		m_bRtn[1] = TheachMotionChk(m_nThreadID,TEACH_PARAM::TT_INSP_CH1_CAM_FOCUS_F);
		m_bRtn[2] = TheachMotionChk(m_nThreadID,TEACH_PARAM::TT_INSP_CH1_CAM_TURN_T);
		m_bRtn[3] = TheachMotionChk(m_nThreadID,TEACH_PARAM::TT_INSP_CH1_CAM_ZOOM_Z);
		m_bRtn[4] = TheachMotionChk(m_nThreadID,TEACH_PARAM::TT_INSP_CH2_CAM_FOCUS_F);
		m_bRtn[5] = TheachMotionChk(m_nThreadID,TEACH_PARAM::TT_INSP_CH2_CAM_TURN_T);
		m_bRtn[6] = TheachMotionChk(m_nThreadID,TEACH_PARAM::TT_INSP_CH2_CAM_ZOOM_Z);
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepPosChk"),m_strStateName);

			// B Zone���� ������ ����ġ��ŭ TH �̵�.
			C_Zone_LCam_Theta(CellData_GetVTAlignth(JIG_CH_1));
			C_Zone_RCam_Theta(CellData_GetVTAlignth(JIG_CH_2));

			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE)>5.)
		{
			if(m_bRtn[0] == FALSE)	SetError(ALARM_CZONE_INSP_CAM_X);
			else if(m_bRtn[1] == FALSE)	SetError(ALARM_CZONE_INSP_CH1_CAM_FOCUS);
			else if(m_bRtn[2] == FALSE)	SetError(ALARM_CZONE_INSP_CH1_CAM_THETA);
			else if(m_bRtn[3] == FALSE)	SetError(ALARM_CZONE_INSP_CH1_CAM_Z);
			else if(m_bRtn[4] == FALSE)	SetError(ALARM_CZONE_INSP_CH2_CAM_FOCUS);
			else if(m_bRtn[5] == FALSE)	SetError(ALARM_CZONE_INSP_CH2_CAM_THETA);
			else if(m_bRtn[6] == FALSE)	SetError(ALARM_CZONE_INSP_CH2_CAM_Z);
		}
		break;
	case stepOffsetPosChk: // B Zone���� ���� Offset �� �̵� Ȯ�� �� �׷� ��ŸƮ ����
		m_bRtn[0] = C_Zone_LCam_ThetaChk(CellData_GetVTAlignth(JIG_CH_1));
		m_bRtn[1] = C_Zone_RCam_ThetaChk(CellData_GetVTAlignth(JIG_CH_2));
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepOffsetPosChk"),m_strStateName);

			//20170210 byskcho
			//�˻絵�� �˶� �߻��� �� �˻� �����Ҷ� �˻����α׷� �״� ������ �����ϱ� ���ؼ�
			//�˻縦 ���ϰ� NG�� �ϰ� ��������
			// Grab Start
/*
			if(C_Zone_Inspection_Start(VI_A1, JIG_CH_1) == FALSE || C_Zone_Inspection_Start(VI_A2, JIG_CH_2) == FALSE)
			{
				C_Zone_CamX();

				C_Zone_LCam_Focus();
				C_Zone_LCam_Theta();
				C_Zone_LCamZ();

				C_Zone_RCam_Focus();
				C_Zone_RCam_Theta();
				C_Zone_RCamZ();

				m_Timer.Start();
				nStep = stepEnd;
				break;
			}
*/
			C_Zone_Inspection_Start(VI_A1, JIG_CH_1);
			C_Zone_Inspection_Start(VI_A2, JIG_CH_2);
			CZoneVIGrabStartTime();
			CZoneVIStartTime();	
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			if(m_bRtn[0] == FALSE)		SetError(ALARM_CZONE_INSP_CH1_CAM_T_OFFSET);
			else if(m_bRtn[1] == FALSE)	SetError(ALARM_CZONE_INSP_CH2_CAM_T_OFFSET);
		}
		break;
	case stepBeginPosMove:	 // Grab End üũ �� Turn ���� �⺻ ��ġ�� �̵�.
		m_bRtn[0] = C_Zone_Inspection_Check(JIG_CH_1);
		m_bRtn[1] = C_Zone_Inspection_Check(JIG_CH_2);
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepBeginPosMove"),m_strStateName);
			CZoneVIEndTime();
			CZoneVIGrabEndTime();
			CZoneVIClassStartTime();

			C_Zone_CamX();

			C_Zone_LCam_Focus();
			C_Zone_LCam_Theta();
			C_Zone_LCamZ();
			
			C_Zone_RCam_Focus();
			C_Zone_RCam_Theta();
			C_Zone_RCamZ();


			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > theConfigBank.m_Option.m_dGrabEndWaitTime)
		{				
			if(m_bRtn[0] == FALSE)
			{
				Set_InspectSkip(CELL_POS_CZONE_CH1 , TRUE);
				SetError(ALARM_CZONE_INSP_CH1_GRABEND);
			}
			else if(m_bRtn[1] == FALSE)
			{
				Set_InspectSkip(CELL_POS_CZONE_CH2 , TRUE);
				SetError(ALARM_CZONE_INSP_CH2_GRABEND);
			}
		}
		break;
	case stepEnd:	// Turn ���� �⺻��ġ �̵� üũ
		m_bRtn[0] = TheachMotionChk(m_nThreadID,TEACH_PARAM::TT_INSP_CAM_X);
		m_bRtn[1] = TheachMotionChk(m_nThreadID,TEACH_PARAM::TT_INSP_CH1_CAM_FOCUS_F);
		m_bRtn[2] = TheachMotionChk(m_nThreadID,TEACH_PARAM::TT_INSP_CH1_CAM_TURN_T);
		m_bRtn[3] = TheachMotionChk(m_nThreadID,TEACH_PARAM::TT_INSP_CH1_CAM_ZOOM_Z);
		m_bRtn[4] = TheachMotionChk(m_nThreadID,TEACH_PARAM::TT_INSP_CH2_CAM_FOCUS_F);
		m_bRtn[5] = TheachMotionChk(m_nThreadID,TEACH_PARAM::TT_INSP_CH2_CAM_TURN_T);
		m_bRtn[6] = TheachMotionChk(m_nThreadID,TEACH_PARAM::TT_INSP_CH2_CAM_ZOOM_Z);
		
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"),m_strStateName);
			SetZoneEnd(ZONE_ID_C,TRUE);
			TactUpdate(CZONE_TACT);
			nStep = stepIdle;
		}
		else if(m_Timer.Stop(FALSE)>5.)
		{	
			if(m_bRtn[0] == FALSE)	SetError(ALARM_CZONE_INSP_CAM_X);
			else if(m_bRtn[1] == FALSE)	SetError(ALARM_CZONE_INSP_CH1_CAM_FOCUS);
			else if(m_bRtn[2] == FALSE)	SetError(ALARM_CZONE_INSP_CH1_CAM_THETA);
			else if(m_bRtn[3] == FALSE)	SetError(ALARM_CZONE_INSP_CH1_CAM_Z);
			else if(m_bRtn[4] == FALSE)	SetError(ALARM_CZONE_INSP_CH2_CAM_FOCUS);
			else if(m_bRtn[5] == FALSE)	SetError(ALARM_CZONE_INSP_CH2_CAM_THETA);
			else if(m_bRtn[6] == FALSE)	SetError(ALARM_CZONE_INSP_CH2_CAM_Z);
		}
		break;
	}

	m_nStep = nStep;

	return nStep;
}



int CStateDZone::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{
	case stepStart:	// ���� Class End Check �� Contact Off �� ���ϴ� �˻� ���� (White Current?)
		theLog[LOG_SEQUENCE].AddBuf(_T("[%s] D_Zone Start"),m_strStateName);
		if(CellTagExist(CELL_POS_DZONE_CH1) == FALSE && CellTagExist(CELL_POS_DZONE_CH2) == FALSE)
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] D Zone Cell Not Exist."),m_strStateName);

			SetZoneEnd(ZONE_ID_D,TRUE);
			TactUpdate(DZONE_TACT);
			nStep = stepIdle;

			break;
		}

		// ������ ���� ���� Ǯ�� �ȵ� ���� �̻�� 2017/04/26 HSK.
//		if (theConfigBank.m_Option.m_bUseRepeatInsp == FALSE)
//		{
			TurnOff(JIG_CH_1);
			TurnOff(JIG_CH_2);
//		}

		m_Timer.Start();
		nStep++;
		break;
	case stepClassEndChk:	// Class End Chk
		m_bRtn[0] = D_Zone_VT_ClassEndChk(JIG_CH_1);
		m_bRtn[1] = D_Zone_VT_ClassEndChk(JIG_CH_2);

		if(IsReturnOk())
		{
			CZoneVIClassEndTime();
			theLog[LOG_SEQUENCE].AddBuf(_T("[%s] D_Zone stepClassEndChk"),m_strStateName);

			// Class End �� File Thr ����.
			if(theConfigBank.m_Option.m_bUseDryRun == FALSE && theConfigBank.m_Option.m_bUseVIInsp)
			 	theThreadInitializer.StartLoadVTFileThread();

			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE)>theConfigBank.m_Option.m_dClassEndWaitTime)
		{
			CZoneVIClassEndTime();
			theLog[LOG_SEQUENCE].AddBuf(_T("[%s] D_Zone stepClassEndChk NG"),m_strStateName);
			m_Timer.Start();
			nStep++;
			// ���⼭ �ν���� ��ŵ �ϸ� ���� ���ܿ��� ������ �ȴ�.���⼭ �˶� ��Ÿ� ���� ���ܿ����� �˶� �ߴ� ����� �켱 �Ѿ��. 2017/06/15 HSK.
			/*
			if(m_bRtn[0] == FALSE)
			{
				Set_InspectSkip(CELL_POS_DZONE_CH1 , TRUE);
				SetError(ALARM_DZONE_ACTIVE1_END_VT_CHECK);
			}
			else if(m_bRtn[1] == FALSE)	
			{
				Set_InspectSkip(CELL_POS_DZONE_CH2 , TRUE);
				SetError(ALARM_DZONE_ACTIVE2_END_VT_CHECK);
			}
			*/
		}
		break;
	case stepFileChk:
		m_bRtn[0] = TurnOffChk(JIG_CH_1);
		m_bRtn[1] = TurnOffChk(JIG_CH_2);
		m_bRtn[2] = D_Zone_VT_FileLoadChk(JIG_CH_1);
		m_bRtn[3] = D_Zone_VT_FileLoadChk(JIG_CH_2);

		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf(_T("[%s] stepFileChk "),m_strStateName);

			//20170209 byskcho
			JudgeFinalClass(JIG_CH_1);
			JudgeFinalClass(JIG_CH_2);

			//���� VISION �ҷ� �߻��� �˶� ����.2017/04/26 HSK.
			//���� �Ｚ �ʿ��� �ٽ� ��û�� �ȿ��� ������ ���߿� ���� 2017/04/26 HSK.
			/*
			if (GetVisionNG(theUnitFunc.AMT_GetAutoJigID(ZONE_ID_D),JIG_CH_1))
			{
				SetVisionNGCnt(theUnitFunc.AMT_GetAutoJigID(ZONE_ID_D),JIG_CH_1,TRUE);
				CIM_CellAllInspectionEnd();
				SetZoneEnd(ZONE_ID_D,TRUE);
				TactUpdate(DZONE_TACT);
				nStep = stepIdle;

				SetError(ALARM_DZONE_CH1_VISION_CONTINUE_NG);
				break;
			}
			if (GetVisionNG(theUnitFunc.AMT_GetAutoJigID(ZONE_ID_D),JIG_CH_2))
			{
				SetVisionNGCnt(theUnitFunc.AMT_GetAutoJigID(ZONE_ID_D),JIG_CH_2,TRUE);
				CIM_CellAllInspectionEnd();
				SetZoneEnd(ZONE_ID_D,TRUE);
				TactUpdate(DZONE_TACT);
				nStep = stepIdle;

				SetError(ALARM_DZONE_CH2_VISION_CONTINUE_NG);
				break;
			}
			*/
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE)>10.)
		{
			/*--if(m_bRtn[2] == FALSE || m_bRtn[3] == FALSE)
			{
				JudgeFinalClass(JIG_CH_1);
				JudgeFinalClass(JIG_CH_2);
			}

			CIM_CellAllInspectionEnd();
			SetZoneEnd(ZONE_ID_D,TRUE);
			TactUpdate(DZONE_TACT);
			--*/
			if(m_bRtn[0] == FALSE)	
			{
				theLog[LOG_SEQUENCE].AddBuf(_T("[%s]Ch1 Turn Off Chk Fail - Retry"),m_strStateName);
				//--SetError(ALARM_DZONE_CH1_TURNOFF);
				nStep = stepEnd;
				m_Timer.Start();
				TurnOff(JIG_CH_1);				   
			}
			else if(m_bRtn[1] == FALSE)
			{
				theLog[LOG_SEQUENCE].AddBuf(_T("[%s]Ch2 Turn Off Chk Fail - Retry"),m_strStateName);
				//SetError(ALARM_DZONE_CH2_TURNOFF);
				nStep = stepEnd;
				m_Timer.Start();
				TurnOff(JIG_CH_2);
			 }
			else if(m_bRtn[2] == FALSE || m_bRtn[3] == FALSE)
			{
				theLog[LOG_SEQUENCE].AddBuf(_T("[%s]Ch VT File Chk Fail - Retry"),m_strStateName);
				nStep = stepEnd;
				m_Timer.Start();
					// Class End �� File Thr ����.
				if(theConfigBank.m_Option.m_bUseDryRun == FALSE && theConfigBank.m_Option.m_bUseVIInsp)
			 		theThreadInitializer.StartLoadVTFileThread();
			}
			
			//--nStep = stepIdle;
		}
		break;
	case stepEnd:	// Contact Off Ȯ�� �� End ( Cell data ����� Unloader TR�� ���� ���� ���.)
		m_bRtn[0] = TurnOffChk(JIG_CH_1);
		m_bRtn[1] = TurnOffChk(JIG_CH_2);
		m_bRtn[2] = D_Zone_VT_FileLoadChk(JIG_CH_1);
		m_bRtn[3] = D_Zone_VT_FileLoadChk(JIG_CH_2);
		m_bRtn[4] = m_Timer.Stop(FALSE)> 0.5 ? TRUE : FALSE;
		//--if(m_Timer.Stop(FALSE)>0.5)
		if(IsReturnOk())
		{
			// ������ ���ÿ� D Zone �۾� ������ ���α� ��� ���� �̻�� 2017/04/26 HSK
			/*
			if (theConfigBank.m_Option.m_bUseRepeatInsp == TRUE)
			{
				WriteCellLog(CELL_POS_DZONE_CH1);
				WriteCellLog(CELL_POS_DZONE_CH2);
			}
			*/
			//20171127 byskcho RETRY ���� ���� ������ ó��
			JudgeFinalClass(JIG_CH_1);
			JudgeFinalClass(JIG_CH_2);

			theLog[LOG_SEQUENCE].AddBuf(_T("[%s] stepEnd"),m_strStateName);
			CIM_CellAllInspectionEnd();
			SetZoneEnd(ZONE_ID_D,TRUE);
			TactUpdate(DZONE_TACT); 
			nStep = stepIdle;
		}
		else if(m_Timer.Stop(FALSE) > 10.0)
		{
			//2018-03-09,SKCHO
			//���α׿� CONTACT NG�ε� �˻� ����� ������ ����
			//CONTACT NG���� �˻� ��ŵ�ϰ� DZONE���� TURN OFF�ΰ�� JUDGEFINALCLASS�Լ� ���� ����.
			//�Ʒ� IF ���� ����
			//if(m_bRtn[2] == FALSE || m_bRtn[3] == FALSE)
			//{
				JudgeFinalClass(JIG_CH_1);
				JudgeFinalClass(JIG_CH_2);
			//}

			CIM_CellAllInspectionEnd();
			SetZoneEnd(ZONE_ID_D,TRUE);
			TactUpdate(DZONE_TACT);

			if(m_bRtn[0] == FALSE)	
			{
				theLog[LOG_SEQUENCE].AddBuf(_T("[%s]Ch1 Turn Off Chk Fail"),m_strStateName);
				SetError(ALARM_DZONE_CH1_TURNOFF);
			}
			else if(m_bRtn[1] == FALSE)
			{
				theLog[LOG_SEQUENCE].AddBuf(_T("[%s]Ch2 Turn Off Chk Fail"),m_strStateName);
				SetError(ALARM_DZONE_CH2_TURNOFF);
			}
			else if(m_bRtn[2] == FALSE)
			{
				theLog[LOG_SEQUENCE].AddBuf(_T("[%s]Ch1 VT File Chk Fail"),m_strStateName);
				SetError(ALARM_DZONE_ACTIVE1_VT_FILE);
			}
			else if(m_bRtn[3] == FALSE)
			{
				theLog[LOG_SEQUENCE].AddBuf(_T("[%s]Ch2 VT File Chk Fail"),m_strStateName);
				SetError(ALARM_DZONE_ACTIVE2_VT_FILE);
			}
		}
		break;
	}

	m_nStep = nStep;

	return nStep;
}
/*int CStateCH1_T5ReStart::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{

	}

	m_nStep = nStep;

	return nStep;

}
*/