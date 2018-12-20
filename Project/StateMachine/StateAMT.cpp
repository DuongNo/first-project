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
	case stepStart:	// 테이블 이동 전 인터락 체크 및 각 존 SetZone 후 Turn
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
				
			if(MoveToNext(TRUE) == FALSE) // Table 회전
			{		
				SetError(ALARM_WORK_TABLE_NOT_MOVE);
				break;
			}

			// Turn 시작 타임 기록.
			TableTurnTimeStart();
			// Wait 타임 갱신.
			WaitTimeEnd();

			
			//2018-03-15,MGYUN, 턴테이블 위치 정보 멜섹 기록.
			theMonitorBank.WriteTurnTablePos();

			// Table Pos 갱신.
			theProcBank.m_nInspTablePos++;
			if(theProcBank.m_nInspTablePos >= CONST_TABLE_POS::POS_MAX)
				theProcBank.m_nInspTablePos = CONST_TABLE_POS::POS_1;

			// Cell Data 이동.
			CellData_ToNextPos();
			
			//2018-03-15,MGYUN, 턴테이블 위치 정보 멜섹 기록.
			theMonitorBank.WriteTurnTablePos();
			
			// 뺑뺑이 모드 사용 시 셀 정보 클리어 아직 미사용 2017/04/26 HSK.
			/*
			if (theConfigBank.m_Option.m_bUseRepeatInsp)
			{
				Zone_CellInfo_Clear(CELL_POS_AZONE_CH1);
				Zone_CellInfo_Clear(CELL_POS_AZONE_CH2);
			}
			*/
			// 테이블 포지션 이동 후 PG Set Zone. 이동전에 하면 하나 마나...
			AMT_PGSetZone();
			// C Zone 검사를 위한 Zone Switching.
			Zone_Switching();
			// Zone End 리셋.
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
	case stepEnd:	// 테이블 Turn 체크
		if (theProcBank.m_nInspTablePos == GetEntryTablePos(0.02))
		{
			// Turn 엔드 타임 기록.
			TableTurnTimeEnd();
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepEnd"),m_strStateName);
			
			double dValue = 0;
			theDeviceMotion.GetPosition(MP2100_THREAD_MAIN_PROCESS, AXIS_SVO_TURN_TABLE_T, dValue);		///	현재위치 표시
			//2017-07-24,JSJUNG, DD모터 로그 추가.
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
	case stepStart:	// Contact 체크 및 A Zone에서 원하는 검사 실행.
		// 체널 스킵과 검사 뺑뺑이 모드 옵션 인데 아직 안함 .2017/04/26 HSK.
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
		//2017-11-20,SKCHO 전체 인터락 상태가 아인 상태에서 JIG SKIP 가능하게 수정
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
				
				//2017-12-06,SKCHO 김동호 프로 요청으로 LDTR BLOWER OFF로 이동
				//2017-09-12,SKCHO JIG 안착되자마자 버큠온으로 변경(김동호 프로 요청)
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
			// JobEnd 일지라도 로더에 Cell이 있을 경우 A Zone End 하지 않고 로더 셀 기다린다. 017/04/27 HSK.
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
			// Job End가 아닐때도 셀을 체크 하도록 한다. 17/04/26 HSK
			if((CellTagExist(CELL_POS_AZONE_CH1) == TRUE || CellTagExist(CELL_POS_AZONE_CH2) == TRUE))
			{
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepStart."),m_strStateName);
				
				//2017-09-12,SKCHO JIG 안착되자마자 버큠온으로 변경(김동호 프로 요청)
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
			//if( pCell->m_strCellID != _T("UV_FAIL") ) //2017-04-05,skcho, 코그닉스에서 FAIL 로 데이터를 줌
			{				
				//1. CELL INFO REQUEST 중복 체크
				// Cell Info 중복 보고 해도 이상 없다고 하여 주석 2017/04/27 HSK
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
					pCell->m_strCellInfoResult = _T("-1");//REQUEST 초기값
//				}				
			}
			else
			{
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] CH1 MCR_FAIL"),m_strStateName);
				theUnitFunc.Set_InspectSkip(CELL_POS_AZONE_CH1,TRUE);  //2017-04-08,skcho, MCR READING 실패시 검사 스킵
				
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
		//1. Cell ID Infomation 기다림
		pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_AZONE_CH1));
		if(pCell)
		{			
			if(pCell->bCellInfoRequest)
			{			
				nRequest = theUnitFunc.CIM_CellInfoRequestCheck(CELL_POS_AZONE_CH1);	

				//609보고를 안하기 위한 초기화
				//CIM_SetRCMD()에서 값이 변함
//				pCell->m_bIsInspectionSkip = TRUE;
				if(nRequest == 1)  //정상적인 데이터
				{
					//1. cell infomation true이면 track_in
					// 중복 Track In 발생을 방지하기 위해 Track In 여부 체크 후 Track In 한다.
					if(CellData_TrackInChk(CELL_POS_AZONE_CH1) == FALSE)
					{
						// 트랙인 시점에 RCMD CH 구별 플래그 온 2017/06/08 HSK
						theProcBank.m_bRCMDCh1Chk = TRUE;
						theProcBank.m_strRCMDCellIDCh1 = _T("");
						//2017-05-15,skcho, Track-in log OK,NoneCell,Already 분리
						if(CIM_CellTrackIn(CELL_POS_AZONE_CH1))
						{
							//20170201
							//TRACK IN을 했으므로 반드시  TRACK OUT을 해준다
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
					//삭제
					//theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh1TrackIn "),m_strStateName);
					m_Timer.Start();
					nStep++;
				}
				else if( nRequest== 0)
				{
					//2018-07-25,SKCHO, CELL LOADING 시 PRODUCT_ID 정보 주기 위해서 CELL INFORMATION REQUEST를 MCR READING 시퀀스로 이동
					//바로 알람 발생
					SetError(ALARM_CIM_CELL_INFO_CH1);
					/*
					//타임 아웃이고 바이패스  false 이면 알람
					if(m_Timer.Stop(FALSE) > theConfigBank.m_Option.m_dCellInfoWaitTime && !theConfigBank.m_Option.m_bUseCh1ReqByPass)
					{
						//2017-09-24,SKCHO,타임아웃시 한번더 요청하고 두번째 타임아웃에서는 파일이 있는지 확인한다.
						if(theProcBank.m_nCH1CellInfoRetryStep == 0)
						{
							theProcBank.m_nCH1CellInfoRetryStep = 1;
							nStep = stepCh1CellRequest;
							m_Timer.Start();
						}
						else if(theProcBank.m_nCH1CellInfoRetryStep == 1)
						{
							theProcBank.m_nCH1CellInfoRetryStep = 0;
							//2017-09-24,SKCHO,타임아웃일때 결과 파일이 없으면 알람
							if(!theUnitFunc.ReadCellInformationResult(CELL_POS_AZONE_CH1))
							{								
								SetError(ALARM_CIM_CELL_INFO_CH1);
							}
						}
						
					}
					//바이패스 true 이면 진행
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
				//609보고를 안하기 위한 초기화
				//CIM_SetRCMD()에서 값이 변함
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
		m_bRtn[0] = m_Timer.Stop(FALSE) > 0.1 ? TRUE : FALSE;  //2017-04-07,skcho, 트랙인 하고 RCMD 수신 대기시간, 수신하지 못하는경우가 있어 INSPECTOIN FAIL 결과가 나옴
		if(IsReturnOk())
		{
			pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_AZONE_CH1));
			if(pCell->bTrackInChk)
			{		
				//1. 트랙인 후 jobprocess 기다림			
				// 상위로 부터 TrackIn 정보 확인 후 다음으로.
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
				  //타임 아웃이고 바이패스  false 이면 알람
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
								//2017-09-24,SKCHO,타임아웃일때 결과 파일이 없으면 알람
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
					//바이패스 true 이면 진행
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
				//NG 포트로
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
			//if( pCell->m_strCellID != _T("UV_FAIL") ) //2017-04-05,skcho, 코그닉스에서 FAIL 로 데이터를 줌
			{
				//CELL INFO 중복체크
				// Cell Info 중복 보고 해도 이상 없다고 하여 주석 2017/04/27 HSK
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
					pCell->m_strCellInfoResult = _T("-1");//REQUEST 초기값
//				}
			}
			else
			{
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] CH2 MCR_FAIL"),m_strStateName);
				theUnitFunc.Set_InspectSkip(CELL_POS_AZONE_CH2,TRUE);  //2017-04-08,skcho, MCR READING 실패시 검사 스킵
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
		//1. Cell ID Infomation 기다림
		pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_AZONE_CH2));
		if(pCell)
		{			
			if(pCell->bCellInfoRequest)
			{
				nRequest = theUnitFunc.CIM_CellInfoRequestCheck(CELL_POS_AZONE_CH2);
				//609보고를 안하기 위한 초기화
				//CIM_SetRCMD()에서 값이 변함
//				pCell->m_bIsInspectionSkip = TRUE;
				if(nRequest == 1)  //정상적인 데이터
				{
					//1. cell infomation true이면 track_in
					// 중복 Track In 발생을 방지하기 위해 Track In 여부 체크 후 Track In 한다.
					if(CellData_TrackInChk(CELL_POS_AZONE_CH2) == FALSE)
					{
						// 트랙인 시점에 RCMD CH 구별 플래그 온 2017/06/08 HSK
						theProcBank.m_bRCMDCh2Chk = TRUE;
						theProcBank.m_strRCMDCellIDCh2 = _T("");
						//2017-05-15,skcho, Track-in log OK,NoneCell,Already 분리
						if(CIM_CellTrackIn(CELL_POS_AZONE_CH2))
						{
							//20170201
							//TRACK IN을 했으므로 반드시  TRACK OUT을 해준다
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
					//타임 아웃이고 바이패스  false 이면 알람
					if(m_Timer.Stop(FALSE) > theConfigBank.m_Option.m_dCellInfoWaitTime && !theConfigBank.m_Option.m_bUseCh2ReqByPass)
					{
						//2017-09-19,SKCHO TIMEOUT 발생시 1회 재시도
						if(theProcBank.m_nCH2CellInfoRetryStep == 0)
						{
							theProcBank.m_nCH2CellInfoRetryStep = 1;
							nStep = stepCh2CellRequest;
							m_Timer.Start();
						}
						else if(theProcBank.m_nCH2CellInfoRetryStep == 1)
						{
							theProcBank.m_nCH2CellInfoRetryStep = 0;
							//2017-09-24,SKCHO,타임아웃일때 결과 파일이 없으면 알람
							if(!theUnitFunc.ReadCellInformationResult(CELL_POS_AZONE_CH2))
							{								
								SetError(ALARM_CIM_CELL_INFO_CH2);
							}						
						}
					}
					//바이패스 true 이면 진행
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
				//609보고를 안하기 위한 초기화
				//CIM_SetRCMD()에서 값이 변함
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
		m_bRtn[0] = m_Timer.Stop(FALSE) > 0.1 ? TRUE : FALSE;  //2017-04-07,skcho, 트랙인 하고 RCMD 수신 대기시간, 수신하지 못하는경우가 있어 INSPECTOIN FAIL 결과가 나옴
		if(IsReturnOk())
		{
			pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_AZONE_CH2));
			if(pCell->bTrackInChk)
			{
				//1. 트랙인 후 jobprocess 기다림			
				// 상위로 부터 TrackIn 정보 확인 후 다음으로.
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
				  //타임 아웃이고 바이패스  false 이면 알람
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
								//2017-09-24,SKCHO,타임아웃일때 결과 파일이 없으면 알람
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
					//바이패스 true 이면 진행
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
				//NG 포트로
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] CHANNEL2 TO NG"),m_strStateName);
				m_Timer.Start();
				nStep++;
			}
		}
		break;
	case stepContactChk: // CH1,CH2 Contact여부 체크.
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

		//2017.10.16, JSJUNG. CONTACT CHECK는 BZONE 에서 함.START
		//nStep = stepEnd;
	//	break;
		//2017.10.16, JSJUNG. CONTACT CHECK는 BZONE 에서 함.END

		m_bRtn[0] = A_Zone_ContactChk(JIG_CH_1);
		m_bRtn[1] = A_Zone_ContactChk(JIG_CH_2);

		if(IsReturnOk())
		{	
			//2017-09-12,SKCHO JIG 안착되자마자 버큠온으로 변경(김동호 프로 요청)
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
		//else if(m_Timer.Stop(FALSE) > 3.)  //5->3변경
		else if(m_Timer.Stop(FALSE) >theConfigBank.m_Option.m_nContactDelay)  //5->3변경
		{
			//2017-09-12,SKCHO JIG 안착되자마자 버큠온으로 변경(김동호 프로 요청)
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

			//컨택 결과가 둘다 GOOD이 아닐경우 5초 기다린후
			if((m_bRtn[0]==FALSE && m_bRtn[1]==FALSE))// && (m_bRtn[2]==FALSE || m_bRtn[3]==FALSE))
			{
				// 컨택 결과가 둘다 NG일때	
				//Reset(JIG_CH_1);
				//Reset(JIG_CH_2);
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactChk CH1,CH2 Fail"),m_strStateName);
				if (CellTagExist(CELL_POS_AZONE_CH1))
				{
					pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_AZONE_CH1));
					if(pCell->m_strContactResult_1st == _T("NG_CELL_LOADING"))
					{
						// 컨택 결과가 둘다 NG일때	
						Reset(JIG_CH_1);
						theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactChk CH1 Reset"),m_strStateName);
					}
				}
				if (CellTagExist(CELL_POS_AZONE_CH2))
				{
					pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_AZONE_CH2));
					if(pCell->m_strContactResult_1st == _T("NG_CELL_LOADING"))
					{
						// 컨택 결과가 둘다 NG일때	
						Reset(JIG_CH_2);	
						theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactChk CH2 Reset"),m_strStateName);
					}
				}
			}
			//else if((m_bRtn[0]==TRUE && m_bRtn[1]==FALSE))
			else if((m_bRtn[0]==FALSE && m_bRtn[1]==TRUE))// && (m_bRtn[2]==TRUE && m_bRtn[3]==TRUE))
			{
				// 컨택 결과가 CH1만 NG일때	
				//Reset(JIG_CH_1);
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactChk CH1 Fail"),m_strStateName);
           	// 컨택 결과가 CH1만 NG일때	
				if (CellTagExist(CELL_POS_AZONE_CH1))
				{
					pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_AZONE_CH1));
					if(pCell->m_strContactResult_1st == _T("NG_CELL_LOADING"))
					{
						// 컨택 결과가 둘다 NG일때	
						Reset(JIG_CH_1);	
						theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactChk CH1 Reset"),m_strStateName);
					}
				}	
			}
			//else if((m_bRtn[0]==FALSE && m_bRtn[1]==TRUE))
			else if((m_bRtn[0]==TRUE && m_bRtn[1]==FALSE))// && (m_bRtn[2]==FALSE || m_bRtn[3]==FALSE))
			{
				// 컨택 결과가 CH2만 NG일때	
				//Reset(JIG_CH_2);
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactChk CH2 Fail"),m_strStateName);
				// 컨택 결과가 CH2만 NG일때	
				if (CellTagExist(CELL_POS_AZONE_CH2))
				{
					pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_AZONE_CH2));
					if(pCell->m_strContactResult_1st == _T("NG_CELL_LOADING"))
					{
						// 컨택 결과가 둘다 NG일때	
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
			// Cell이 없을 때도 컨택을 해서 수정.17/04/25 HSK.
			if(m_bRtn[0] == FALSE)			
				ContactStart(JIG_CH_1);
			if(m_bRtn[1] == FALSE)			
				ContactStart(JIG_CH_2);

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactRetry"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		break;
	// 따로 딜레이가 필요없다. 체크 하는 시간이 딜레이다. 17/04/25 HSK.
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
	case stepEnd:	// 검사 실행 Check 후 End.
		if (CellTagExist(CELL_POS_AZONE_CH1))
			m_bRtn[0] = AMTVacChk(AMT_GetAutoJigID(ZONE_ID_A),JIG_CH_1);
		else
			m_bRtn[0] = TRUE;

		if (CellTagExist(CELL_POS_AZONE_CH2))
			m_bRtn[1] = AMTVacChk(AMT_GetAutoJigID(ZONE_ID_A),JIG_CH_2);
		else
			m_bRtn[1] = TRUE;


		//2017.10.16, JSJUNG. CONTACT CHECK는 BZONE 에서 함.START
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

			//2017-09-26,SKCHO, 정상적으로 처리되면 생성된 CELL INFO, JOB PROCESS 파일은 지운다.
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
		//2017.10.16, JSJUNG. CONTACT CHECK는 BZONE 에서 함.END
		//2017.10.16, JSJUNG. 이 밑으로는 안 사용안함.
		/*
		// 어짜피 컨택 체크는 전부 해야 한다. 조건문 없앰 2017/04/25 HSK
//		if ((A_Zone_GetContactCount(JIG_CH_1) != 0) || (A_Zone_GetContactCount(JIG_CH_2) != 0))
//		{
		m_bRtn[2] = A_Zone_ContactChk(JIG_CH_1);
		m_bRtn[3] = A_Zone_ContactChk(JIG_CH_2);
//		}
		//20170730 JSJUNG, 지그별 컨택 NG count/3회 지정 : JigContactNGFlag 상태보고 알람
		m_bRtn[4] = !theProcBank.m_bJigContactNGFlag;

		//2017-08-01,JSJUNG, 지그별 컨택 NG count/3회 지정 : JigContactNGFlag 상태보고 초기화
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

			//2017-09-26,SKCHO, 정상적으로 처리되면 생성된 CELL INFO, JOB PROCESS 파일은 지운다.
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

			//2017-04-08, 함수로 만들어야 함
			//2017-04-07, skcho, CONTACT NG일 경우 검사 공정 생략
			//상위 보고 관련하여 포맷이 정확하지 않아 보류
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
			
			//2017-04-08, 함수로 만들어야 함
			//2017-04-07, skcho, CONTACT NG일 경우 검사 공정 생략
			//상위 보고 관련하여 포맷이 정확하지 않아 보류
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
				//20170730 JSJUNG, 지그별 컨택 NG count/3회 지정 : JIG별 채널값 적용하여 알람표시
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
	//2017.10.16, JSJUNG. BZone Contact / Recontact 추가 및 변경.
	CCellInfo *pCell;

	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{
	//2017.10.16, JSJUNG. BZone Contact / Recontact 추가 및 변경.
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
	
	//2017.10.16, JSJUNG. BZone Contact / Recontact 추가 및 변경.
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
		//else if(m_Timer.Stop(FALSE) > 3.)  //5->3변경
		else if(m_Timer.Stop(FALSE) > theConfigBank.m_Option.m_nContactDelay)  //5->3변경
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
				//2018-05-16,SKCHO,삭제
				//--B_Zone_SetContactCount(JIG_CH_1);
				//--B_Zone_SetContactCount(JIG_CH_2);
			}

			//컨택 결과가 둘다 GOOD이 아닐경우 5초 기다린후
			if((m_bRtn[0]==FALSE && m_bRtn[1]==FALSE))// && (m_bRtn[2]==FALSE || m_bRtn[3]==FALSE))
			{
				// 컨택 결과가 둘다 NG일때	
				//Reset_BZone(JIG_CH_1);
			//	Reset_BZone(JIG_CH_2);
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactChk CH1,CH2 Fail"),m_strStateName);
				if (CellTagExist(CELL_POS_BZONE_CH1))
				{
					pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_BZONE_CH1));
					if(pCell->m_strContactResult_1st == _T("NG_CELL_LOADING"))
					{
						// 컨택 결과가 둘다 NG일때	
						Reset_BZone(JIG_CH_1);
						theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactChk CH1 Reset"),m_strStateName);
					}
				}
				if (CellTagExist(CELL_POS_BZONE_CH2))
				{
					pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_BZONE_CH2));
					if(pCell->m_strContactResult_1st == _T("NG_CELL_LOADING"))
					{
						// 컨택 결과가 둘다 NG일때	
						Reset_BZone(JIG_CH_2);	
						theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactChk CH2 Reset"),m_strStateName);
					}
				}
			}
			//else if((m_bRtn[0]==TRUE && m_bRtn[1]==FALSE))
			else if((m_bRtn[0]==FALSE && m_bRtn[1]==TRUE))// && (m_bRtn[2]==TRUE && m_bRtn[3]==TRUE))
			{
					theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactChk CH1 Fail"),m_strStateName);
				// 컨택 결과가 CH1만 NG일때	
				if (CellTagExist(CELL_POS_BZONE_CH1))
				{
					pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_BZONE_CH1));
					if(pCell->m_strContactResult_1st == _T("NG_CELL_LOADING"))
					{
						// 컨택 결과가 둘다 NG일때	
						Reset_BZone(JIG_CH_1);	
						theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactChk CH1 Reset"),m_strStateName);
					}
				}				
				
			}
			//else if((m_bRtn[0]==FALSE && m_bRtn[1]==TRUE))
			else if((m_bRtn[0]==TRUE && m_bRtn[1]==FALSE))// && (m_bRtn[2]==FALSE || m_bRtn[3]==FALSE))
			{
				// 컨택 결과가 CH2만 NG일때	
				//Reset_BZone(JIG_CH_2);
				theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactChk CH2 Fail"),m_strStateName);
				// 컨택 결과가 CH2만 NG일때	
				if (CellTagExist(CELL_POS_BZONE_CH2))
				{
					pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_BZONE_CH2));
					if(pCell->m_strContactResult_1st == _T("NG_CELL_LOADING"))
					{
						// 컨택 결과가 둘다 NG일때	
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
				//DIC_CHECK FAIL 이면 RECONTACT 안함
				if (CellTagExist(CELL_POS_BZONE_CH1))
				{
					pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_BZONE_CH1));
					if(pCell->m_strContactResult_1st == _T("NG_CELL_LOADING"))
					{				
						//2018-05-19,MGYUN,재 컨택 시 메세지4개 NONE_CELL로 초기화 후 CELL_LOADING 날리게
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
				//DIC_CHECK FAIL 이면 RECONTACT 안함
				if (CellTagExist(CELL_POS_BZONE_CH2))
				{
					pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_BZONE_CH2));
					if(pCell->m_strContactResult_1st == _T("NG_CELL_LOADING"))
					{				
						//2018-05-19,재 컨택 시 메세지4개 NONE_CELL로 초기화 후 CELL_LOADING 날리게
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
	//	if (m_Timer.Stop(FALSE) > 8.)  //2->5 ->8초로 변경 2018-05-17,SKCHO
	//	{
	//		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactDelay"),m_strStateName);
	//		m_Timer.Start();
	//		nStep++;
	//	}
	//	break;

	//2017.10.16, JSJUNG. BZone Contact / Recontact 추가 및 변경.
	case stepAlignMove:	//case stepStart(기존) VT Align 해야하는 Cell 인지 확인 후 Align 모터 Left 마크 위치로 이동
		
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
	case stepCh1LeftPosChk: // 모터 이동 체크 후 Ch1 Left 비젼 얼라인 요청.
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

			// B zone 1Ch Left 마크 그랩.
			B_Zone_Align_Grab(JIG_CH_1, TRUE);		//Align  request
			m_Timer.Start();
			nStep++;
		}
		break;
	case stepCh1LeftAlignChk:	 // 얼라인 성공 여부 확인 후 Ch1 Right 마크 이동.
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
	case stepCh1RightPosChk:	// 모터 이동 체크 후 Ch1 Right 비젼 얼라인 요청
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
			// B zone 1Ch Right 마크 그랩.
			B_Zone_Align_Grab(JIG_CH_1, FALSE);		//Align  request
			m_Timer.Start();
			nStep++;
		}
		break;
	case stepCh1RightAlignChk:	// 얼라인 성공 여부 확인 후 Ch2 Left 비젼 얼라인 위치로 이동
		m_bRtn[0] = B_Zone_Align_Check();
		if(IsReturnOk())
		{
			if(theProcBank.m_nVI_MITAlignResult == ALIGN_RESULT_GOOD)
			{
				// Right 마크 그랩 후에 Th 보정치가 날라온다. 해당 보정치 Cell data에 삽입.
				CellData_SetVTAlignScore(JIG_CH_1, E_MARK_RIGHT, theProcBank.m_dVI_MITAlignScore);
				// 보정되는 방향이 반대라서 -1.0곱해준다.
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
			::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_CZONE_UPDATE, 0,1); //2017-03-03,skcho , 메인화면에 얼라인 각도 전송

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
	case stepCh2LeftPosChk:	// 모터 이동 체크 후 Ch2 Left 비젼 얼라인 요청.
		m_bRtn[0] = B_Zone_Cam_X_Check(JIG_CH_2, CAMPOS_LEFT);
		m_bRtn[1] = B_Zone_Cam_Y_Check(JIG_CH_2, CAMPOS_LEFT);
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepCh2LeftPosChk"),m_strStateName);
			//2017-07-04,SKCHO, 두번 전송하여 삭제함
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
	case stepCh2LeftAlignChk:	// 얼라인 성공 여부 확인 후 Ch2 Right 마크 이동.
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
	case stepCh2RightPosChk:	 // 모터 이동 체크 후 Ch2 Right 비젼 얼라인 요청
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
	case stepCh2RightAlignChk:	 // 얼라인 성공 여부 확인 후 대기위치 이동.
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
			
			::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_CZONE_UPDATE, 0,2); //2017-03-03,skcho , 메인화면에 얼라인 각도 전송
		
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

	//2017.10.16, JSJUNG. BZone Contact / Recontact 추가 및 변경.
	case stepContactEnd:	// 검사 실행 Check 후 End.
		m_bRtn[0] = B_Zone_ContactChk(JIG_CH_1);
		m_bRtn[1] = B_Zone_ContactChk(JIG_CH_2);
		//20170730 JSJUNG, 지그별 컨택 NG count/3회 지정 : JigContactNGFlag 상태보고 알람
		m_bRtn[2] = !theProcBank.m_bJigContactNGFlag;

		//2017-08-01,JSJUNG, 지그별 컨택 NG count/3회 지정 : JigContactNGFlag 상태보고 초기화
		theProcBank.m_bJigContactNGFlag = FALSE;

		if(IsReturnOk())
		{	
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactEnd"),m_strStateName);

			//2017.10.17, JSJUNG.ContactNG Count 분리
			NGCountChk(JIG_CH_1);
			NGCountChk(JIG_CH_2);
			
			//2017-04-07, skcho, CONTACT NG일 경우 검사 공정 생략
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
				//2017-12-08,SKCHO, 추가
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

				//2017-12-08,SKCHO, 추가
				B_Zone_SetClass(JIG_CH_2);
			}
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > theConfigBank.m_Option.m_nContactDelay)
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepContactEnd"),m_strStateName);
			//2017.10.17, JSJUNG.ContactNG Count 분리
			NGCountChk(JIG_CH_1);
			NGCountChk(JIG_CH_2);
			//2017-04-07, skcho, CONTACT NG일 경우 검사 공정 생략
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
				//2017-12-08,SKCHO, 추가
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
				//2017-12-08,SKCHO, 추가
				B_Zone_SetClass(JIG_CH_2);
			}

			if (m_bRtn[2] == FALSE)
			{
				//20170730 JSJUNG, 지그별 컨택 NG count/3회 지정 : JIG별 채널값 적용하여 알람표시
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

	case stepEnd:	// 대기 위치 이동 확인 후 End.
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
	case stepStart:	// 비젼 모터 축 검사 위치 확인 및 모터 축 이동.
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
		//FALSE  이면 SKIP 의미로 사용
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
	case stepPosChk:// 비젼 모터 축 검사 위치 확인 후 Theta Offset 값 이동.
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

			// B Zone에서 측정한 보정치만큼 TH 이동.
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
	case stepOffsetPosChk: // B Zone에서 받은 Offset 값 이동 확인 후 그랩 스타트 전송
		m_bRtn[0] = C_Zone_LCam_ThetaChk(CellData_GetVTAlignth(JIG_CH_1));
		m_bRtn[1] = C_Zone_RCam_ThetaChk(CellData_GetVTAlignth(JIG_CH_2));
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepOffsetPosChk"),m_strStateName);

			//20170210 byskcho
			//검사도중 알람 발생시 재 검사 동작할때 검사프로그램 죽는 현상을 방지하기 위해서
			//검사를 안하고 NG로 하고 빠져나감
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
	case stepBeginPosMove:	 // Grab End 체크 후 Turn 모터 기본 위치로 이동.
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
	case stepEnd:	// Turn 모터 기본위치 이동 체크
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
	case stepStart:	// 비전 Class End Check 및 Contact Off 및 원하는 검사 실행 (White Current?)
		theLog[LOG_SEQUENCE].AddBuf(_T("[%s] D_Zone Start"),m_strStateName);
		if(CellTagExist(CELL_POS_DZONE_CH1) == FALSE && CellTagExist(CELL_POS_DZONE_CH2) == FALSE)
		{
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] D Zone Cell Not Exist."),m_strStateName);

			SetZoneEnd(ZONE_ID_D,TRUE);
			TactUpdate(DZONE_TACT);
			nStep = stepIdle;

			break;
		}

		// 뺑뺑이 모드시 컨텍 풀면 안됨 아직 미사용 2017/04/26 HSK.
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

			// Class End 후 File Thr 시작.
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
			// 여기서 인스펙션 스킵 하면 다음 스텝에서 문제가 된다.여기서 알람 뜰거면 다음 스텝에서도 알람 뜨니 여기는 우선 넘어가자. 2017/06/15 HSK.
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

			//연속 VISION 불량 발생시 알람 띄우기.2017/04/26 HSK.
			//아직 삼성 쪽에서 다시 요청이 안오니 검증은 나중에 하자 2017/04/26 HSK.
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
					// Class End 후 File Thr 시작.
				if(theConfigBank.m_Option.m_bUseDryRun == FALSE && theConfigBank.m_Option.m_bUseVIInsp)
			 		theThreadInitializer.StartLoadVTFileThread();
			}
			
			//--nStep = stepIdle;
		}
		break;
	case stepEnd:	// Contact Off 확인 후 End ( Cell data 기록은 Unloader TR이 집어 갈때 기록.)
		m_bRtn[0] = TurnOffChk(JIG_CH_1);
		m_bRtn[1] = TurnOffChk(JIG_CH_2);
		m_bRtn[2] = D_Zone_VT_FileLoadChk(JIG_CH_1);
		m_bRtn[3] = D_Zone_VT_FileLoadChk(JIG_CH_2);
		m_bRtn[4] = m_Timer.Stop(FALSE)> 0.5 ? TRUE : FALSE;
		//--if(m_Timer.Stop(FALSE)>0.5)
		if(IsReturnOk())
		{
			// 뺑뺑이 모드시에 D Zone 작업 끝날때 셀로그 기록 아직 미사용 2017/04/26 HSK
			/*
			if (theConfigBank.m_Option.m_bUseRepeatInsp == TRUE)
			{
				WriteCellLog(CELL_POS_DZONE_CH1);
				WriteCellLog(CELL_POS_DZONE_CH2);
			}
			*/
			//20171127 byskcho RETRY 에서 받은 데이터 처리
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
			//셀로그에 CONTACT NG인데 검사 결과가 공백인 버그
			//CONTACT NG에서 검사 스킵하고 DZONE에서 TURN OFF인경우 JUDGEFINALCLASS함수 수행 안함.
			//아래 IF 조건 삭제
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