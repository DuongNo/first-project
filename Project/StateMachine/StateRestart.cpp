#include "StdAfx.h"
#include "StateRestart.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int CStateRestart::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;
	ResetReturnValue();
	int nStep = m_nStep;
	switch(nStep)
	{
	case stepStart: // 재시작 할 경우 Cell Data Chk 하여 Vac On 및 Cyl 동작 해준다.
		if(m_Timer.Stop(FALSE) > 2.)
		{
			theLog[LOG_SEQUENCE].AddBuf(_T("[%s] stepStart"), m_strStateName);
			m_LastAlarm = ALM_NONE;
			Restart_VacuumOn();
			for(int i = 0 ; i < ZONE_ID_MAX ; i++)
			{
				//kjpark 20161105 Sequence 동작 수정
				theProcBank.m_bTableTurnStatus[i] = TRUE;
			}
			Restart_InitTrayCylinder();
			nStep++;
			m_Timer.Start();
		}
		break;
	case step1stVacChk:	// Vacuum 및 Cyl 체크
		m_bRtn[0] = Restart_VacuumChk();
		m_bRtn[1] = Restart_InitTrayCylinderChk();
		m_bRtn[2] = m_Timer.Stop(FALSE) > 1.;
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf(_T("[%s] step1stVacChk"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 2.)
		{
			SetError(m_LastAlarm);
		}
		break;
	case stepBitAllOff:	// Robot의 Vac 신호를 뺀 모든 동작 비트를 꺼주도록 한다.(Vac 신호를 임의로 건드릴 경우 Robot이 동작 하지 않는다고 한다......)
		LDRobotBitAllOff(E_ORG);
		UDRobotBitAllOff(E_ORG);
		LDRobotBitAllOff(E_CELL_LOADING,FALSE);
		UDRobotBitAllOff(E_CELL_LOADING,FALSE);
		LDRobotBitAllOff(E_PUT_1,FALSE);
		UDRobotBitAllOff(E_PUT_1,FALSE);
		LDRobotBitAllOff(E_PUT_2,FALSE);
		UDRobotBitAllOff(E_PUT_2,FALSE);
		//2018-05-16,MGYUN,추가
		UDRobotBitAllOff(E_PUT_3,FALSE);
		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepBitAllOff"),m_strStateName);
		m_Timer.Start();
		nStep++;
		break;
	case stepRobotRunningChk: // 여기서 부터 Robot ORG 시퀀스.
		if((BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_RUNNING_Y_ERRRESET))
		{
			theDeviceMelsec.RobotBitOff(LDROBOT_X_PLAYMODE_Y_EXTSTOP);
		}
		if((BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_RUNNING_Y_ERRRESET))
		{
			theDeviceMelsec.RobotBitOff(UDROBOT_X_PLAYMODE_Y_EXTSTOP);
		}
		theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepRobotRunningChk"),m_strStateName);
		m_Timer.Start();
		nStep++;
		break;
	case stepRobotPauseOn:
		if (m_Timer.Stop(FALSE) > 0.2)
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_X_PGMEND_Y_PAUSE);
			theDeviceMelsec.RobotBitOn(UDROBOT_X_PGMEND_Y_PAUSE);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepRobotPauseOn"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		break;
	case seqRobotExtStop:
		if (m_Timer.Stop(FALSE) > 0.2)
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_X_PLAYMODE_Y_EXTSTOP);
			theDeviceMelsec.RobotBitOn(UDROBOT_X_PLAYMODE_Y_EXTSTOP);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] seqRobotExtStop"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		break;
	case seqRobotMotorOn:
		if (m_Timer.Stop(FALSE) > 0.2)
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_X_STOPPING_Y_EXTMOTORON);
			theDeviceMelsec.RobotBitOn(UDROBOT_X_STOPPING_Y_EXTMOTORON);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] seqRobotMotorOn"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		break;
	case stepRobotOnChk:	
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_MOTORON_Y_LOWSPEED);
		m_bRtn[1] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_MOTORON_Y_LOWSPEED);
		if (IsReturnOk())
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_X_ALARM_Y_EXTRESET);
			theDeviceMelsec.RobotBitOn(UDROBOT_X_ALARM_Y_EXTRESET);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepRobotOnChk"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 1.)
		{
			if (m_bRtn[0] == FALSE)		SetError(ALARM_LD_ROBOT_MOTOR_ENERGIZE_ONACK);
			else						SetError(ALARM_UD_ROBOT_MOTOR_ENERGIZE_ONACK);
		}
		break;
	case stepPGMSel:
		// 외부 리셋 신호가 너무 잠깐 켜졌다 꺼졌다 하여 감지 될때가 있고 안될때가 있어서 우선 주석.
		/*
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_EXTRESET_Y_PGMSEL2);
		m_bRtn[1] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_EXTRESET_Y_PGMSEL2);
		if (IsReturnOk())
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_X_PGMACK_Y_PGMSEL1);
			theDeviceMelsec.RobotBitOn(UDROBOT_X_PGMACK_Y_PGMSEL1);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepPGMSel"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 5.)
		{
			if (m_bRtn[0] == FALSE)		SetError(ALARM_LD_ROBOT_RESET_ACK);
			else						SetError(ALARM_UD_ROBOT_RESET_ACK);
		}
		*/
		if (m_Timer.Stop(FALSE) > 0.2)
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_X_PGMACK_Y_PGMSEL1);
			theDeviceMelsec.RobotBitOn(UDROBOT_X_PGMACK_Y_PGMSEL1);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepPGMSel"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		break;
	case stepPGMStrobe:
		if (m_Timer.Stop(FALSE) > 0.2)
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_X_HOMEPOS2_Y_PGMSTROBE);
			theDeviceMelsec.RobotBitOn(UDROBOT_X_HOMEPOS2_Y_PGMSTROBE);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepPGMStrobe"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		break;
	case stepExtStart:
		if (m_Timer.Stop(FALSE) > 0.2)
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_X_TEACHMODE_Y_EXTSTART);
			theDeviceMelsec.RobotBitOn(UDROBOT_X_TEACHMODE_Y_EXTSTART);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepExtStart"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		break;
	case stepRunningChk:
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_RUNNING_Y_ERRRESET);
		m_bRtn[1] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_RUNNING_Y_ERRRESET);
		if (IsReturnOk())
		{
			theDeviceMelsec.RobotBitOn(LDROBOT_X_BATWARN_Y_ORG);
			theDeviceMelsec.RobotBitOn(UDROBOT_X_BATWARN_Y_ORG);
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepRunningChk"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 10.)
		{
			if (m_bRtn[0] == FALSE)		SetError(ALARM_LD_ROBOT_RUNNINGACK);
			else						SetError(ALARM_UD_ROBOT_RUNNINGACK);
		}
		break;
	case stepOrgOff:
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_X_HOMEPOS1_Y_CYCLESTOP);
		m_bRtn[1] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_X_HOMEPOS1_Y_CYCLESTOP);
		if (IsReturnOk())
		{
			theDeviceMelsec.RobotBitOff(LDROBOT_X_BATWARN_Y_ORG);
			theDeviceMelsec.RobotBitOff(UDROBOT_X_BATWARN_Y_ORG);	

			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepOrgOff"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 30.)
		{
			if (m_bRtn[0] == FALSE)		SetError(ALARM_LD_ROBOT_HOMEPOSACK);
			else						SetError(ALARM_UD_ROBOT_HOMEPOSACK);
		}
		break;
	case stepInterlockChk: // Robot이 ORG가 끝난 후 로봇 Interlock을 체크 한다. Interlock신호가 꺼져 있는 경우엔 Org이 제대로 되지 않은 경우다. 그리고 Z Up.
		m_bRtn[0] = (BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_CVGET);
		m_bRtn[1] = (BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_LOWERSTGPUT);
		m_bRtn[2] = (BOOL)theDeviceMelsec.RobotBitRecv(LDROBOT_XY_UPPERSTGPUT);
		m_bRtn[3] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_GETSTAGE);
		m_bRtn[4] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_OKTRAYPUT);
		m_bRtn[5] = (BOOL)theDeviceMelsec.RobotBitRecv(UDROBOT_XY_NGTRAYPUT);
		if (IsReturnOk())
		{
			Restart_ZUp();
			theLog[LOG_SEQUENCE].AddBuf( _T("[%s] stepInterlockChk"),m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if (m_Timer.Stop(FALSE) > 30.)
		{
			if (m_bRtn[0] == FALSE)	
			{
				SetError(ALARM_LD_ROBOT_CVGETINTERLOCKONACK);				
			}
			else if(m_bRtn[1] == FALSE)		SetError(ALARM_LD_ROBOT_UPPERSTGPUTINTERLOCKONACK);
			else if(m_bRtn[2] == FALSE)		SetError(ALARM_LD_ROBOT_LOWERSTGPUTINTERLOCKONACK);
			else if(m_bRtn[3] == FALSE)		SetError(ALARM_UD_ROBOT_GETCOMPACK);
			else if(m_bRtn[4] == FALSE)		SetError(ALARM_UD_ROBOT_OKPUTINTERLOCKONACK);
			else if(m_bRtn[5] == FALSE)		SetError(ALARM_UD_ROBOT_NGPUTINTERLOCKONACK);
		}
		break;
	case stepZUpChk:		// 모든 Z축 Up Chk후 XYT 이동 한다.
		m_bRtn[0] = Restart_ZUpChk();
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf(_T("[%s] stepZUpChk"), m_strStateName);
			
			if(theProcBank.m_bPreAlignOrgStart == TRUE)
			{
				theLog[LOG_SEQUENCE].AddBuf(_T("[%s] stepPaOrg"), m_strStateName);

				//2017-12-25,SKCHO, 셀이 있으면 다시 얼라인 하기 위해서
				if(CellTagExist(CELL_POS_PA_STAGE_CH1) || CellTagExist(CELL_POS_PA_STAGE_CH2))
				{
					CellData_SetPreAlignState(JIG_CH_1,ALIGN_RESULT_NONE);
					CellData_SetPreAlignState(JIG_CH_2,ALIGN_RESULT_NONE);
				}
				Restart_PAOrgMove();
				nStep++;
			}
			else
			{
				Restart_XYTMove();
				nStep = stepReadyPosChk;

			}
			m_Timer.Start();
			
		}
		else if(m_Timer.Stop(FALSE) > 20.)
		{
			SetError(m_LastAlarm);
		}
		break;
	case stepPaOrgChk:
		/*m_bRtn[0] = Restart_PAOrgMoveChk();
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf(_T("[%s] stepPaOrgChk"), m_strStateName);
			theProcBank.m_bPreAlignOrgStart = FALSE;
			Restart_XYTMove();
			m_Timer.Start();
			nStep++;

		}
		else if(m_Timer.Stop(FALSE) > 30.)
		{
			SetError(m_LastAlarm);
		}*/
		if(Restart_PAOrgMoveChk())
		{
			theLog[LOG_SEQUENCE].AddBuf(_T("[%s] stepPaOrgChk"), m_strStateName);
			theProcBank.m_bPreAlignOrgStart = FALSE;
			Restart_XYTMove();
			m_Timer.Start();
			nStep++;
		}
		else if( (m_Timer.Stop(FALSE) > 120.) || CheckAxisOriginFail() )
		{			
			SetError(m_LastAlarm);
		}
		break;
	case stepReadyPosChk:	// 대기위치 Chk
		m_bRtn[0] = Restart_XYTMoveChk();
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf(_T("[%s] stepReadyPosChk"), m_strStateName);
			// 설비 멈추고 중간에 PG 점검 후에 진행 할 때 셋존이 다를 수 있어 리스타트 할때 다시 해준다.2017/04/30 HSK.
			// SetZone A를 다시 보내 주면 컨택이 풀린다 ..2017/05/03 HSK.
			//AMT_PGSetZone();
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 20.)
		{
			SetError(m_LastAlarm);
		}
		break;
	case stepTrayDataDelay: //UD Robot 소켓 열린지 확인.
		m_bRtn[0] = theSocketInterFace.m_pSocket_Robot[E_UD_ROBOT].IsConnected();
		if(IsReturnOk())
		{
			theLog[LOG_SEQUENCE].AddBuf(_T("[%s] stepTrayDataSend"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 20.)
		{
			SetError(ALARM_COMM_UDROBOT);
		}
		break;
	case stepTrayDataSend: // 소켓열린거 확인 후 500ms 있다가 Tray Pitch 보낸다.
		if(m_Timer.Stop(FALSE) > 1.)
		{
			theSocketInterFace.SendMsgToRobot(E_UD_ROBOT);

			// 피치 이동 시킨 이후에 이동 감지 못하고 알람 발생 하였을 시 데이터가 없을 경우 대비 하여 넣는다 2017/06/07 HSK.
			if(theConfigBank.m_Option.m_bLoadPitchIn)
			{
				if(LDConvCellChk(CONV_SEN_3) && (CellTagExist(CELL_POS_LD_CONV) == FALSE))
				{
					CellData_LDConvIn();
					theProductBank.UpdateMainInputCount();
				}
			}
			//2017-09-13,SKCHO
			//if(CellTagExist(CELL_POS_LD_CONV))
			//	LoaderAlignStart();
			//재시작시 컨베어에 셀 정보와 셀이 있어야 얼라인 함 -> 엣지얼라인 알람 방지
			if(CellTagExist(CELL_POS_LD_CONV) && LDConvCellChk(CONV_SEN_3))
			{
				LoaderAlignStart();
				theLog[LOG_SEQUENCE].AddBuf(_T("[%s] LD Align Start"), m_strStateName);
			}
			else if(LDConvCellChk(CONV_SEN_3) == FALSE && CellTagExist(CELL_POS_LD_CONV))
			{
				theCellBank.RemoveCellTagCellInfo(CELL_POS_LD_CONV);
				theProductBank.UpdateMainInputCountMinus();
				theLog[LOG_SEQUENCE].AddBuf(_T("[%s] LD Conv Cell Delete"), m_strStateName);
			}

			theLog[LOG_SEQUENCE].AddBuf(_T("[%s] stepTrayDataSend"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		break;
	case stepEnd:	// stepHold에서 대기. Tray Pitch 받은 후 UD Robot이 3초정도 Delay가 필요하다고 한다....왜그런진 모르겠다.
		m_bRtn[0] = InConv_TrayChk(CONV_SEN_1,SENSOR_ON);
		m_bRtn[1] = InConv_TrayChk(CONV_SEN_4,SENSOR_ON);
		m_bRtn[2] = InConv_TrayChk(CONV_SEN_5,SENSOR_ON);
		if(m_Timer.Stop(FALSE) > 3.)
		{
			// 만약 리스타트 할때 굿인 리프트에 트레이 있으면 트레이 데이터 생성 2017/04/30 HSK.
			if(IsReturnOk())
			{
				TrayData_Create(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_LIFT);
			}

			//2018-04-20,SKCHO, RESTART이후에 RUN으로 상태 보고
			if(theUnitFunc.CellDataMachineChk(CELL_POS_LDROBOT,CELL_POS_UD_ROBOT))
			{
				//2017-07-20, jsjung, 설비 IDLE->RUN 변경시 IDLE TIME 종료
				if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)
				{
					::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_IDLECHAGNERUN_UPDATE, 0,0); 
					theLog[LOG_LOAD_DELAY_TACT].AddBuf(_T("설비 IDLE->RUN 변경"));
				}
				theProcBank.ChangeMachineState(CONST_MACHINE_STATE::MACHINE_RUN);
			}
			else
			{
				//110
				//theProcBank.ChangeMachineState(CONST_MACHINE_STATE::MACHINE_IDLE);
				theProcBank.ChangeMachineState(CONST_MACHINE_STATE::MACHINE_RUN);
				::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_IDLECHAGNE_UPDATE, 0,0); 
				theLog[LOG_LOAD_DELAY_TACT].AddBuf(_T("설비 IDLE 변경 시작"));
			}
			//2018-06-21,GHLEE, 홍영기 과장님 요청으로 RUN 상태 CHANGE 후 TRAY ALARM 다시 확인하고 FAULT 보고
			if(theProcBank.GetTrayAlarmChk())
			{
				if(theProcBank.m_bFaultOfTray[0] == TRUE 
					|| theProcBank.m_bFaultOfTray[1] == TRUE
					|| theProcBank.m_bFaultOfTray[2] == TRUE)
				{
					theSocketInterFace.SendCmdStateToDataPC(EQUIP_SET_EQUIPMENT_STATUS_CHANGE, E_EQST_MATCH_FAULT);
				}
			}

			theLog[LOG_SEQUENCE].AddBuf(_T("[%s] stepEnd"), m_strStateName);
			nStep = stepHold;		// Restart는 완료상태로 있어야 하기 때문에 stepIdle이 아니라 stepHold로 간다
		}
		break;
	}

	m_nStep = nStep;

	return nStep;
}


void CStateRestart::Restart_VacuumOn()
{
	// 셀 있을 때
	if (CellTagExist(CELL_POS_LDROBOT))
	{
		RobotCellVacOn(VAC_ON,E_LD_ROBOT);
		theDeviceMelsec.RobotBitOn(LDROBOT_XY_VACON);
		theDeviceMelsec.RobotBitOff(LDROBOT_XY_VACOFF);
	}

	if (CellTagExist(CELL_POS_UPPER_SHUTTLE_CH1))
	{
		UpperShuttle_VacuumOn(VAC_ON,JIG_CH_1);
		theDeviceMelsec.RobotBitOn(LDROBOT_XY_UPPERSTGVAC1ON);
		theDeviceMelsec.RobotBitOff(LDROBOT_XY_UPPERSTGVAC1OFF);
	}

	if (CellTagExist(CELL_POS_UPPER_SHUTTLE_CH2))
	{
		UpperShuttle_VacuumOn(VAC_ON,JIG_CH_2);
		theDeviceMelsec.RobotBitOn(LDROBOT_XY_UPPERSTGVAC2ON);
		theDeviceMelsec.RobotBitOff(LDROBOT_XY_UPPERSTGVAC2OFF);
	}

	if (CellTagExist(CELL_POS_LOWER_SHUTTLE_CH1))
	{
		LowerShuttle_VacuumOn(VAC_ON,JIG_CH_1);
		theDeviceMelsec.RobotBitOn(LDROBOT_XY_LOWERSTGVAC1ON);
		theDeviceMelsec.RobotBitOff(LDROBOT_XY_LOWERSTGVAC1OFF);
	}

	if (CellTagExist(CELL_POS_LOWER_SHUTTLE_CH2))
	{
		LowerShuttle_VacuumOn(VAC_ON,JIG_CH_2);
		theDeviceMelsec.RobotBitOn(LDROBOT_XY_LOWERSTGVAC2ON);
		theDeviceMelsec.RobotBitOff(LDROBOT_XY_LOWERSTGVAC2OFF);
	}

	if (CellTagExist(CELL_POS_PA_TR_CH1))
		PATR_Vacuum_On(VAC_ON,JIG_CH_1);

	if (CellTagExist(CELL_POS_PA_TR_CH2))
		PATR_Vacuum_On(VAC_ON,JIG_CH_2);

	if (CellTagExist(CELL_POS_PA_STAGE_CH1))
		PAStageVac(VAC_ON,JIG_CH_1);

	if (CellTagExist(CELL_POS_PA_STAGE_CH2))
		PAStageVac(VAC_ON,JIG_CH_2);

	if (CellTagExist(CELL_POS_LD_TR_CH1))
		LDTRVacOn(VAC_ON,JIG_CH_1);

	if (CellTagExist(CELL_POS_LD_TR_CH2))
		LDTRVacOn(VAC_ON,JIG_CH_2);

	if (CellTagExist(CELL_POS_AZONE_CH1))
		AMTVacOn(VAC_ON,AMT_GetAutoJigID(ZONE_ID_A),JIG_CH_1);
	else
		AMTVacOn(VAC_OFF,AMT_GetAutoJigID(ZONE_ID_A),JIG_CH_1);

	if (CellTagExist(CELL_POS_AZONE_CH2))
		AMTVacOn(VAC_ON,AMT_GetAutoJigID(ZONE_ID_A),JIG_CH_2);
	else
		AMTVacOn(VAC_OFF,AMT_GetAutoJigID(ZONE_ID_A),JIG_CH_2);

	if (CellTagExist(CELL_POS_BZONE_CH1))
		AMTVacOn(VAC_ON,AMT_GetAutoJigID(ZONE_ID_B),JIG_CH_1);
	else
		AMTVacOn(VAC_OFF,AMT_GetAutoJigID(ZONE_ID_B),JIG_CH_1);

	if (CellTagExist(CELL_POS_BZONE_CH2))
		AMTVacOn(VAC_ON,AMT_GetAutoJigID(ZONE_ID_B),JIG_CH_2);
	else
		AMTVacOn(VAC_OFF,AMT_GetAutoJigID(ZONE_ID_B),JIG_CH_2);

	if (CellTagExist(CELL_POS_CZONE_CH1))
		AMTVacOn(VAC_ON,AMT_GetAutoJigID(ZONE_ID_C),JIG_CH_1);
	else
		AMTVacOn(VAC_OFF,AMT_GetAutoJigID(ZONE_ID_C),JIG_CH_1);

	if (CellTagExist(CELL_POS_CZONE_CH2))
		AMTVacOn(VAC_ON,AMT_GetAutoJigID(ZONE_ID_C),JIG_CH_2);
	else
		AMTVacOn(VAC_OFF,AMT_GetAutoJigID(ZONE_ID_C),JIG_CH_2);

	if (CellTagExist(CELL_POS_DZONE_CH1))
		AMTVacOn(VAC_ON,AMT_GetAutoJigID(ZONE_ID_D),JIG_CH_1);
	else
		AMTVacOn(VAC_OFF,AMT_GetAutoJigID(ZONE_ID_D),JIG_CH_1);

	if (CellTagExist(CELL_POS_DZONE_CH2))
		AMTVacOn(VAC_ON,AMT_GetAutoJigID(ZONE_ID_D),JIG_CH_2);
	else
		AMTVacOn(VAC_OFF,AMT_GetAutoJigID(ZONE_ID_D),JIG_CH_2);

	if (CellTagExist(CELL_POS_UD_TR_CH1))
		UnloadTRVacOnOff(JIG_CH_1,VAC_ON);

	if (CellTagExist(CELL_POS_UD_TR_CH2))
		UnloadTRVacOnOff(JIG_CH_2,VAC_ON);
	
	if (CellTagExist(CELL_POS_UD_STAGE_CH1))
	{
		ULDBuffSTGVacOnOff(VAC_ON,TRUE,FALSE);
		theDeviceMelsec.RobotBitOn(UDROBOT_XY_STGCH1VACON);
		theDeviceMelsec.RobotBitOff(UDROBOT_XY_STGCH1VACOFF);
	}

	if (CellTagExist(CELL_POS_UD_STAGE_CH2))
	{
		ULDBuffSTGVacOnOff(VAC_ON,FALSE,TRUE);
		theDeviceMelsec.RobotBitOn(UDROBOT_Y_STG2VACON);
		theDeviceMelsec.RobotBitOff(UDROBOT_X_STG1GETERR_Y_STG2VACOFF);
	}

	if (CellTagExist(CELL_POS_UD_ROBOT))
	{
		RobotCellVacOn(VAC_ON,E_UD_ROBOT);
		theDeviceMelsec.RobotBitOn(UDROBOT_XY_VACON);
		theDeviceMelsec.RobotBitOff(UDROBOT_XY_VACOFF);
	}

	if(TrayData_Exist(NG_TRAY_PORT, TRAY_INFO_POS_NG_TR))
		NGTrayTr_VacOnOff(VAC_ON);

	// Cell 없을 때 
	if (CellTagExist(CELL_POS_LDROBOT) == FALSE)
	{
		RobotCellVacOn(VAC_OFF,E_LD_ROBOT);
		theDeviceMelsec.RobotBitOn(LDROBOT_XY_VACOFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_XY_VACON);
	}
	if (CellTagExist(CELL_POS_UPPER_SHUTTLE_CH1) == FALSE)
	{
		UpperShuttle_VacuumOn(VAC_OFF,JIG_CH_1);
		theDeviceMelsec.RobotBitOn(LDROBOT_XY_UPPERSTGVAC1OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_XY_UPPERSTGVAC1ON);
	}
	if (CellTagExist(CELL_POS_UPPER_SHUTTLE_CH2) == FALSE)
	{
		UpperShuttle_VacuumOn(VAC_OFF,JIG_CH_2);
		theDeviceMelsec.RobotBitOn(LDROBOT_XY_UPPERSTGVAC2OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_XY_UPPERSTGVAC2ON);
	}
	if (CellTagExist(CELL_POS_LOWER_SHUTTLE_CH1) == FALSE)
	{
		LowerShuttle_VacuumOn(VAC_OFF,JIG_CH_1);
		theDeviceMelsec.RobotBitOn(LDROBOT_XY_LOWERSTGVAC1OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_XY_LOWERSTGVAC1ON);
	}
	if (CellTagExist(CELL_POS_LOWER_SHUTTLE_CH2) == FALSE)
	{
		LowerShuttle_VacuumOn(VAC_OFF,JIG_CH_2);
		theDeviceMelsec.RobotBitOn(LDROBOT_XY_LOWERSTGVAC2OFF);
		theDeviceMelsec.RobotBitOff(LDROBOT_XY_LOWERSTGVAC2ON);
	}
	// 체널이 두개인 경우 한쪽 체널에서 블로우 불면 한쪽 체널 셀이 떨어질수 있다...17/04/21
/*
	if (CellTagExist(CELL_POS_PA_TR_CH1) == FALSE)
		PATR_Vacuum_On(VAC_OFF,JIG_CH_1);

	if (CellTagExist(CELL_POS_PA_TR_CH2) == FALSE)
		PATR_Vacuum_On(VAC_OFF,JIG_CH_2);

	if (CellTagExist(CELL_POS_PA_STAGE_CH1) == FALSE)
		PAStageVac(VAC_OFF,JIG_CH_1);

	if (CellTagExist(CELL_POS_PA_STAGE_CH2) == FALSE)
		PAStageVac(VAC_OFF,JIG_CH_2);

	if (CellTagExist(CELL_POS_LD_TR_CH1) == FALSE)
		LDTRVacOn(VAC_OFF,JIG_CH_1);

	if (CellTagExist(CELL_POS_LD_TR_CH2) == FALSE)
		LDTRVacOn(VAC_OFF,JIG_CH_2);

	if (CellTagExist(CELL_POS_UD_TR_CH1) == FALSE)
		UnloadTRVacOnOff(JIG_CH_1,VAC_OFF);

	if (CellTagExist(CELL_POS_UD_TR_CH2) == FALSE)
		UnloadTRVacOnOff(JIG_CH_2,VAC_OFF);
*/
	if (CellTagExist(CELL_POS_UD_STAGE_CH1) == FALSE)
	{
		ULDBuffSTGVacOnOff(VAC_OFF,TRUE,FALSE);
		theDeviceMelsec.RobotBitOn(UDROBOT_XY_STGCH1VACOFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_XY_STGCH1VACON);
	}

	if (CellTagExist(CELL_POS_UD_STAGE_CH2) == FALSE)
	{
		ULDBuffSTGVacOnOff(VAC_OFF,FALSE,TRUE);
		theDeviceMelsec.RobotBitOn(UDROBOT_X_STG1GETERR_Y_STG2VACOFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_Y_STG2VACON);
	}

	if (CellTagExist(CELL_POS_UD_ROBOT) == FALSE)
	{
		RobotCellVacOn(VAC_OFF,E_UD_ROBOT);
		theDeviceMelsec.RobotBitOn(UDROBOT_XY_VACOFF);
		theDeviceMelsec.RobotBitOff(UDROBOT_XY_VACON);
	}

	//2017-12-04,SKCHO, 버큠 체크해서 없으면 오프
	if(TrayData_Exist(NG_TRAY_PORT, TRAY_INFO_POS_NG_TR) == FALSE)
	{
		if(NGTrayTr_VacChk(VAC_ON) == FALSE)
		{
			NGTrayTr_VacOnOff(VAC_OFF);
		}
	}
	//2017-12-04,SKCHO, 버큠 체크해서 없으면 오프
	if(TrayData_Exist(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_TR) == FALSE)
	{
		if(TrayTr_VacChk(VAC_ON) == FALSE)
		{
			TrayTr_VacOnOff(VAC_OFF);
		}
	}

	Sleep(100);

	if (CellTagExist(CELL_POS_LDROBOT) == FALSE)
		RobotBlowOff(E_LD_ROBOT);

	if (CellTagExist(CELL_POS_UPPER_SHUTTLE_CH1) == FALSE)
		UpperShuttle_BlowOff(JIG_CH_1);

	if (CellTagExist(CELL_POS_UPPER_SHUTTLE_CH2) == FALSE)
		UpperShuttle_BlowOff(JIG_CH_2);

	if (CellTagExist(CELL_POS_LOWER_SHUTTLE_CH1) == FALSE)
		LowerShuttle_BlowOff(JIG_CH_1);

	if (CellTagExist(CELL_POS_LOWER_SHUTTLE_CH2) == FALSE)
		LowerShuttle_BlowOff(JIG_CH_2);

	if (CellTagExist(CELL_POS_PA_TR_CH1) == FALSE)
		PATR_BlowOff(JIG_CH_1);

	if (CellTagExist(CELL_POS_PA_TR_CH2) == FALSE)
		PATR_BlowOff(JIG_CH_2);

	if (CellTagExist(CELL_POS_PA_STAGE_CH1) == FALSE)
		PAStage_BlowOff(JIG_CH_1);

	if (CellTagExist(CELL_POS_PA_STAGE_CH2) == FALSE)
		PAStage_BlowOff(JIG_CH_2);

	if (CellTagExist(CELL_POS_LD_TR_CH1) == FALSE)
		LDTR_BlowOff(JIG_CH_1);

	if (CellTagExist(CELL_POS_LD_TR_CH2) == FALSE)
		LDTR_BlowOff(JIG_CH_2);

	if (CellTagExist(CELL_POS_UD_TR_CH1) == FALSE)
		UnloadTR_BlowOff(JIG_CH_1);

	if (CellTagExist(CELL_POS_UD_TR_CH2) == FALSE)
		UnloadTR_BlowOff(JIG_CH_2);

	if (CellTagExist(CELL_POS_UD_STAGE_CH1) == FALSE)
		ULDBuffSTG_BlowOff(TRUE,FALSE);

	if (CellTagExist(CELL_POS_UD_STAGE_CH2) == FALSE)
		ULDBuffSTG_BlowOff(FALSE,TRUE);

	if (CellTagExist(CELL_POS_UD_ROBOT) == FALSE)
		RobotBlowOff(E_UD_ROBOT);


	//2017-12-04,SKCHO, 버큠 체크해서 없으면 오프
	if(TrayData_Exist(NG_TRAY_PORT, TRAY_INFO_POS_NG_TR) == FALSE)
	{ 
		if(NGTrayTr_VacChk(VAC_ON) == FALSE)
		{
			NGTrayTr_BlowOff();
		}
	}
	//2017-12-04,SKCHO, 버큠 체크해서 없으면 오프
	if(TrayData_Exist(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_TR) == FALSE)
	{
		if(TrayTr_VacChk(VAC_ON) == FALSE)
		{
			TrayTr_BlowOff();
		}
	}

	LDTRFPCBBlowOn(BLOW_OFF,TRUE,TRUE);
}


BOOL CStateRestart::Restart_VacuumChk()
{
	if(theConfigBank.m_Option.m_bUseDryRun)	return TRUE;

	if (CellTagExist(CELL_POS_LDROBOT))
	{
		if(RobotCellVacOnChk(E_LD_ROBOT,VAC_ON) == FALSE)
		{
			m_LastAlarm = ALARM_LD_ROBOT_VACON;
			return FALSE;
		}
	}

	if (CellTagExist(CELL_POS_UPPER_SHUTTLE_CH1))
	{
		if(UpperShuttle_VacuumChk(JIG_CH_1,VAC_ON) == FALSE)
		{
			m_LastAlarm = ALARM_UPPER_CH1_CELL_VAC;
			return FALSE;
		}
	}

	if (CellTagExist(CELL_POS_UPPER_SHUTTLE_CH2))
	{
		if(UpperShuttle_VacuumChk(JIG_CH_2,VAC_ON) == FALSE)
		{
			m_LastAlarm = ALARM_UPPER_CH2_CELL_VAC;
			return FALSE;
		}
	}

	if (CellTagExist(CELL_POS_LOWER_SHUTTLE_CH1))
	{
		if(LowerShuttle_VacuumChk(JIG_CH_1,VAC_ON) == FALSE)
		{
			m_LastAlarm = ALARM_LOWER_CH1_CELL_VAC;
			return FALSE;
		}
	}

	if (CellTagExist(CELL_POS_LOWER_SHUTTLE_CH2))
	{
		if(LowerShuttle_VacuumChk(JIG_CH_2,VAC_ON) == FALSE)
		{
			m_LastAlarm = ALARM_LOWER_CH2_CELL_VAC;
			return FALSE;
		}
	}

	if (CellTagExist(CELL_POS_PA_TR_CH1))
	{
		if(PATR_Vacuum_Chk(JIG_CH_1,VAC_ON) == FALSE)
		{
			m_LastAlarm = ALARM_PATR_CH1_VAC;
			return FALSE;
		}
	}

	if (CellTagExist(CELL_POS_PA_TR_CH2))
	{
		if(PATR_Vacuum_Chk(JIG_CH_2,VAC_ON) == FALSE)
		{
			m_LastAlarm = ALARM_PATR_CH2_VAC;
			return FALSE;
		}
	}

	if (CellTagExist(CELL_POS_PA_STAGE_CH1))
	{
		if(PAStageVacChk(JIG_CH_1,VAC_ON) == FALSE)
		{
			m_LastAlarm = ALARM_PA_STG_CH1_VAC;
			return FALSE;
		}
	}

	if (CellTagExist(CELL_POS_PA_STAGE_CH2))
	{
		if(PAStageVacChk(JIG_CH_2,VAC_ON) == FALSE)
		{
			m_LastAlarm = ALARM_PA_STG_CH2_VAC;
			return FALSE;
		}
	}

	if (CellTagExist(CELL_POS_LD_TR_CH1))
	{
		if(LDTRCellVacChk(JIG_CH_1,VAC_ON) == FALSE)
		{
			m_LastAlarm = ALARM_LD_TR_CH1_VAC;
			return FALSE;
		}
	}

	if (CellTagExist(CELL_POS_LD_TR_CH2))
	{
		if(LDTRCellVacChk(JIG_CH_2,VAC_ON) == FALSE)
		{
			m_LastAlarm = ALARM_LD_TR_CH2_VAC;
			return FALSE;
		}
	}
// 국내 셋업중인 셀은 많이 휘어져 있어서 진공알람이 너무 많이 떠서 스킵

	if (CellTagExist(CELL_POS_AZONE_CH1))
	{
		if(AMTVacChk(AMT_GetAutoJigID(ZONE_ID_A),JIG_CH_1) == FALSE)
		{
			m_LastAlarm = ALARM_AZONE_JIG_CH1_VAC;
			return FALSE;
		}
	}

	if (CellTagExist(CELL_POS_AZONE_CH2))
	{
		if(AMTVacChk(AMT_GetAutoJigID(ZONE_ID_A),JIG_CH_2) == FALSE)
		{
			m_LastAlarm = ALARM_AZONE_JIG_CH2_VAC;
			return FALSE;
		}
	}

	if (CellTagExist(CELL_POS_BZONE_CH1))
	{
		if(AMTVacChk(AMT_GetAutoJigID(ZONE_ID_B),JIG_CH_1) == FALSE)
		{
			m_LastAlarm = ALARM_BZONE_JIG_CH1_VAC;
			return FALSE;
		}
	}

	if (CellTagExist(CELL_POS_BZONE_CH2))
	{
		if(AMTVacChk(AMT_GetAutoJigID(ZONE_ID_B),JIG_CH_2) == FALSE)
		{
			m_LastAlarm = ALARM_BZONE_JIG_CH2_VAC;
			return FALSE;
		}
	}

	if (CellTagExist(CELL_POS_CZONE_CH1))
	{
		if(AMTVacChk(AMT_GetAutoJigID(ZONE_ID_C),JIG_CH_1) == FALSE)
		{
			m_LastAlarm = ALARM_CZONE_JIG_CH1_VAC;
			return FALSE;
		}
	}

	if (CellTagExist(CELL_POS_CZONE_CH2))
	{
		if(AMTVacChk(AMT_GetAutoJigID(ZONE_ID_C),JIG_CH_2) == FALSE)
		{
			m_LastAlarm = ALARM_CZONE_JIG_CH2_VAC;
			return FALSE;
		}
	}

	if (CellTagExist(CELL_POS_DZONE_CH1))
	{
		if(AMTVacChk(AMT_GetAutoJigID(ZONE_ID_D),JIG_CH_1) == FALSE)
		{
			m_LastAlarm = ALARM_DZONE_JIG_CH1_VAC;
			return FALSE;
		}
	}

	if (CellTagExist(CELL_POS_DZONE_CH2))
	{
		if(AMTVacChk(AMT_GetAutoJigID(ZONE_ID_D),JIG_CH_2) == FALSE)
		{
			m_LastAlarm = ALARM_DZONE_JIG_CH2_VAC;
			return FALSE;
		}
	}

	if (CellTagExist(CELL_POS_UD_TR_CH1))
	{
		if(UnloadTRVacChk(VAC_ON,TRUE,FALSE) == FALSE)
		{
			m_LastAlarm = ALARM_ULD_TR_VAC;
			return FALSE;
		}
	}

	if (CellTagExist(CELL_POS_UD_TR_CH2))
	{
		if(UnloadTRVacChk(VAC_ON,FALSE,TRUE) == FALSE)
		{
			m_LastAlarm = ALARM_ULD_TR_VAC;
			return FALSE;
		}
	}

	if (CellTagExist(CELL_POS_UD_STAGE_CH1))
	{
		if(ULDBuffSTGVacChk(VAC_ON,TRUE,FALSE) == FALSE)
		{
			m_LastAlarm = ALARM_UD_STG_VAC;
			return FALSE;
		}
	}

	if (CellTagExist(CELL_POS_UD_STAGE_CH2))
	{
		if(ULDBuffSTGVacChk(VAC_ON,FALSE,TRUE) == FALSE)
		{
			m_LastAlarm = ALARM_UD_STG_VAC;
			return FALSE;
		}
	}

	if (CellTagExist(CELL_POS_UD_ROBOT))
	{
		if(RobotCellVacOnChk(E_UD_ROBOT,VAC_ON) == FALSE)
		{
			m_LastAlarm = ALARM_UD_ROBOT_VACON;
			return FALSE;
		}
	}

	return TRUE;
}


void CStateRestart::Restart_ZUp()
{
	Move_PATR_ZAxis_UpPos();
	LDTRZUpPos();
	MoveUnLoadTR_ZAxis_Up();
	TrayTrZ_UpPos();
	NGTrayTrZ_UpPos();
	B_Zone_Cam_X_Ready();
	B_Zone_Cam_Y_Ready();

	if((TrayData_Exist(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_ALIGN) == FALSE)
		&& InTrayAlignTrayChk(SENSOR_ON))
		InTrayLiftZ_PitchDown(3.);
	if((TrayData_Exist(NG_TRAY_PORT,TRAY_INFO_POS_NG_IN_ALIGN) == FALSE)
		&& NGTrayInAlignTrayChk(SENSOR_ON))
		InNGTrayLiftZ_PitchDown(3.);
}

BOOL CStateRestart::Restart_ZUpChk()
{
	if(Chk_PATR_ZAxis_UpPos() == FALSE)
	{
		m_LastAlarm = ALARM_PATR_Z_UP_POS;
		return FALSE;
	}

	if(LDTRZUpPosChk() == FALSE)
	{
		m_LastAlarm = ALARM_LD_TR_Z_UP_POS;
		return FALSE;
	}

	if(MoveUnLoadTR_ZAxis_UpChk() == FALSE)
	{
		m_LastAlarm = ALARM_ULD_TR_Z_UP_POS;
		return FALSE;
	}

	if(TrayTrZ_UpChk() == FALSE)
	{
		m_LastAlarm = ALARM_OKTRAY_TR_Z_UP_POS;
		return FALSE;
	}

	if(NGTrayTrZ_UpChk() == FALSE)
	{
		m_LastAlarm = ALARM_NGTRAY_TR_Z_UP_POS;
		return FALSE;
	}

	if (B_Zone_Cam_X_Ready_Chk() == FALSE)
	{
		m_LastAlarm = ALARM_BZONE_ALIGN_CAM_X_READY_POS;
		return FALSE;
	}

	if (B_Zone_Cam_Y_Ready_Chk() == FALSE)
	{
		m_LastAlarm = ALARM_BZONE_ALIGN_CAM_Y_READY_POS;
		return FALSE;
	}

	return TRUE;
}


void CStateRestart::Restart_XYTMove()
{		
	if(IsJobEnd())
	{
		// CELL이 한개 또는 두개 담길 수 있으므로 시퀀스에서 처리할 수 있도록 한다. 17/04/24 HSK
/*
		if (CellTagExist(CELL_POS_UPPER_SHUTTLE_CH1) || CellTagExist(CELL_POS_UPPER_SHUTTLE_CH2))
			Move_UpperShuttle_UnLoadPos();
		else if(theProcBank.m_bLowerPutCompInterlock == FALSE)
			Move_UpperShuttle_LoadPos();

		if (CellTagExist(CELL_POS_LOWER_SHUTTLE_CH1) || CellTagExist(CELL_POS_LOWER_SHUTTLE_CH2))
			Move_LowerShuttle_UnLoadPos();
		else
			Move_LowerShuttle_LoadPos();
*/
		if(CellTagExist(CELL_POS_PA_STAGE_CH1) || CellTagExist(CELL_POS_PA_STAGE_CH2)
			&& (CellData_GetPreAlignState(JIG_CH_1) == ALIGN_RESULT_NONE || CellData_GetPreAlignState(JIG_CH_2) == ALIGN_RESULT_NONE))
			Move_PATR_YAxis_WaitPos();
		else if (CellTagExist(CELL_POS_PA_TR_CH1) || CellTagExist(CELL_POS_PA_TR_CH2))
			Move_PATR_YAxis_PutPos();
		else
			Move_PATR_YAxis_GetPos();

/*
		if ((CellTagExist(CELL_POS_PA_STAGE_CH1) == FALSE || CellTagExist(CELL_POS_PA_STAGE_CH2) == FALSE)
			&& (CellData_GetPreAlignState(JIG_CH_1) == ALIGN_RESULT_NONE || CellData_GetPreAlignState(JIG_CH_2) == ALIGN_RESULT_NONE))
		{
			PAXPos(JIG_CH_1);
			PAXPos(JIG_CH_2);
			PAYPos(JIG_CH_1);
			PAYPos(JIG_CH_2);
			PATPos(JIG_CH_1);
			PATPos(JIG_CH_2);
			PAStageLoadPos();
		}
		else
			PAStageUnloadPos();
*/
		if((CellTagExist(CELL_POS_PA_STAGE_CH1) == TRUE || CellTagExist(CELL_POS_PA_STAGE_CH2) == TRUE)
			&& (CellData_GetPreAlignState(JIG_CH_1) != ALIGN_RESULT_NONE && CellData_GetPreAlignState(JIG_CH_2) != ALIGN_RESULT_NONE))
		{
			PAStageUnloadPos();
		}
		else
		{
			PAXPos(JIG_CH_1);
			PAXPos(JIG_CH_2);
			PAYPos(JIG_CH_1);
			PAYPos(JIG_CH_2);
			PATPos(JIG_CH_1);
			PATPos(JIG_CH_2);
			PAStageLoadPos();
		}

		//2017-04-01, skcho, 가운데 && 연산자 구분하기 위해서 괄호 추가
		//UD_ST와 UD_TR이 셀이 있고 UD_TR이 UD_STAGE위에 있을때 로봇이 움직이지 않음 버그 수정
		if ((CellTagExist(CELL_POS_UD_TR_CH1) || CellTagExist(CELL_POS_UD_TR_CH2))
			&& ((CellTagExist(CELL_POS_UD_STAGE_CH1) == FALSE && CellTagExist(CELL_POS_UD_STAGE_CH2) == FALSE)))
			MoveUnloadTR_X_PutPos();
		else
			MoveUnloadTR_X_GetPos();
	}
	else
	{
		
		// CELL이 한개 또는 두개 담길 수 있으므로 시퀀스에서 처리할 수 있도록 한다. 17/04/24 HSK
/*	
		if (theProcBank.m_bUpperPutComp)
			Move_UpperShuttle_UnLoadPos();
		else if(theProcBank.m_bLowerPutCompInterlock == FALSE)
			Move_UpperShuttle_LoadPos();
	
		if (theProcBank.m_bLowerPutComp)
			Move_LowerShuttle_UnLoadPos();
		else
			Move_LowerShuttle_LoadPos();
*/
		if(CellTagExist(CELL_POS_PA_STAGE_CH1) && CellTagExist(CELL_POS_PA_STAGE_CH2)
			&& (CellData_GetPreAlignState(JIG_CH_1) == ALIGN_RESULT_NONE || CellData_GetPreAlignState(JIG_CH_2) == ALIGN_RESULT_NONE))
			Move_PATR_YAxis_WaitPos();
		else if (CellTagExist(CELL_POS_PA_TR_CH1) && CellTagExist(CELL_POS_PA_TR_CH2))
			Move_PATR_YAxis_PutPos();
		else
			Move_PATR_YAxis_GetPos();

/*		if ((CellTagExist(CELL_POS_PA_STAGE_CH1) == FALSE && CellTagExist(CELL_POS_PA_STAGE_CH2) == FALSE)
			&& (CellData_GetPreAlignState(JIG_CH_1) == ALIGN_RESULT_NONE || CellData_GetPreAlignState(JIG_CH_2) == ALIGN_RESULT_NONE))
		{
			PAXPos(JIG_CH_1);
			PAXPos(JIG_CH_2);
			PAYPos(JIG_CH_1);
			PAYPos(JIG_CH_2);
			PATPos(JIG_CH_1);
			PATPos(JIG_CH_2);
			PAStageLoadPos();
		}
		else
			PAStageUnloadPos();
*/
		if((CellTagExist(CELL_POS_PA_STAGE_CH1) == TRUE && CellTagExist(CELL_POS_PA_STAGE_CH2) == TRUE)
			&& (CellData_GetPreAlignState(JIG_CH_1) != ALIGN_RESULT_NONE && CellData_GetPreAlignState(JIG_CH_2) != ALIGN_RESULT_NONE))
		{
			PAStageUnloadPos();
		}
		else
		{
			PAXPos(JIG_CH_1);
			PAXPos(JIG_CH_2);
			PAYPos(JIG_CH_1);
			PAYPos(JIG_CH_2);
			PATPos(JIG_CH_1);
			PATPos(JIG_CH_2);
			PAStageLoadPos();
		}

		if (CellTagExist(CELL_POS_UD_TR_CH1) && CellTagExist(CELL_POS_UD_TR_CH2)
			&& (CellTagExist(CELL_POS_UD_STAGE_CH1) == FALSE && CellTagExist(CELL_POS_UD_STAGE_CH2) == FALSE))
			MoveUnloadTR_X_PutPos();
		else
			MoveUnloadTR_X_GetPos();
	}




//	충돌 위치가 없으므로 그냥 놔둔다. 시퀀스 시작할 때 가도록.
/*	if (CellTagExist(CELL_POS_LD_TR_CH1) == FALSE && CellTagExist(CELL_POS_LD_TR_CH2) == FALSE)
		LDTRXGetPos();
	else
		LDTRXPutPos();
*/
	if(GetEntryTablePos() != theProcBank.m_nInspTablePos)
		MoveToTblPos(theProcBank.m_nInspTablePos, 0.5);

	Zone_Switching();

	C_Zone_CamX();
	C_Zone_LCamZ();
	C_Zone_LCam_Focus();
	C_Zone_LCam_Theta();

	C_Zone_RCamZ();
	C_Zone_RCam_Focus();
	C_Zone_RCam_Theta();

	if (theConfigBank.m_Option.m_bUseTrayAlign)
	{
		if(theProcBank.m_bGoodTrayAlignChk == FALSE)
			TrayTrY_InPos();
		else
			TrayTrY_OutPos();
	}
	else
	{
		TrayTrY_OutPos();
	}
	NGTrayTrX_OutPos();

	if(TrayData_Exist(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_IN_LIFT) == FALSE)
		InTrayLiftZ_DownPos();

	if(TrayData_Exist(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_OUT_LIFT) == FALSE)
	{
		OutTrayLiftZ_UpPos();
	}

	if(TrayData_Exist(NG_TRAY_PORT, TRAY_INFO_POS_NG_OUT_LIFT_ARM) == FALSE)
	{
		NGLiftArmFwdBwd(NG_BUFFER_TRAY_PORT, CYL_FWD);
		OutNGTrayLiftZ_UpPos();
	}
}

BOOL CStateRestart::Restart_XYTMoveChk()
{
	if(theConfigBank.m_Option.m_bUseLotEnd|| IsJobEnd())
	{
		// CELL이 한개 또는 두개 담길 수 있으므로 시퀀스에서 처리할 수 있도록 한다. 17/04/24 HSK
/*
		if (CellTagExist(CELL_POS_UPPER_SHUTTLE_CH1) || CellTagExist(CELL_POS_UPPER_SHUTTLE_CH2))
		{
			if(Chk_UpperShuttle_UnLoadPos() == FALSE)
			{
				m_LastAlarm = ALARM_UPPER_SHUTTLE_UNLOAD_POS;
				return FALSE;
			}
		}
		else if(theProcBank.m_bLowerPutCompInterlock == FALSE)
		{
			if(Chk_UpperShuttle_LoadPos() == FALSE)
			{
				m_LastAlarm = ALARM_UPPER_SHUTTLE_LOAD_POS;
				return FALSE;
			}
		}

		if (CellTagExist(CELL_POS_LOWER_SHUTTLE_CH1) || CellTagExist(CELL_POS_LOWER_SHUTTLE_CH2))
		{
			if(Chk_LowerShuttle_UnLoadPos() == FALSE)
			{
				m_LastAlarm = ALARM_LOWER_SHUTTLE_UNLOAD_POS;
				return FALSE;
			}
		}
		else
		{
			if(Chk_LowerShuttle_LoadPos() == FALSE)
			{
				m_LastAlarm = ALARM_LOWER_SHUTTLE_LOAD_POS;
				return FALSE;
			}
		}
*/

		if(CellTagExist(CELL_POS_PA_STAGE_CH1) || CellTagExist(CELL_POS_PA_STAGE_CH2)
			&& (CellData_GetPreAlignState(JIG_CH_1) == ALIGN_RESULT_NONE || CellData_GetPreAlignState(JIG_CH_2) == ALIGN_RESULT_NONE))
		{
			if(Chk_PATR_YAxis_WaitPos() == FALSE)
			{
				m_LastAlarm = ALARM_PATR_Y_WAIT_POS;
				return FALSE;
			}
		}
		else if (CellTagExist(CELL_POS_PA_TR_CH1) || CellTagExist(CELL_POS_PA_TR_CH2))
		{
			if(Chk_PATR_YAxis_PutPos() == FALSE)
			{
				m_LastAlarm = ALARM_PATR_Y_PUT_POS;
				return FALSE;
			}
		}
		else
		{
			if(Chk_PATR_YAxis_GetPos() == FALSE)
			{
				m_LastAlarm = ALARM_PATR_Y_GET_POS;
				return FALSE;
			}
		}

/*		if ((CellTagExist(CELL_POS_PA_STAGE_CH1) == FALSE || CellTagExist(CELL_POS_PA_STAGE_CH2) == FALSE)
			&& (CellData_GetPreAlignState(JIG_CH_1) == ALIGN_RESULT_NONE || CellData_GetPreAlignState(JIG_CH_2) == ALIGN_RESULT_NONE))
		{
			if(PAStageLoadPosChk() == FALSE)
			{
				m_LastAlarm = ALARM_PASTAGE_LOAD_POS;
				return FALSE;
			}
			if (PAXPosChk(JIG_CH_1) == FALSE)
			{
				m_LastAlarm = ALARM_PA_CH1_X_LOAD_POS;
				return FALSE;
			}
			if (PAXPosChk(JIG_CH_2) == FALSE)
			{
				m_LastAlarm = ALARM_PA_CH2_X_LOAD_POS;
				return FALSE;
			}
			if (PAYPosChk(JIG_CH_1) == FALSE)
			{
				m_LastAlarm = ALARM_PA_CH1_Y_LOAD_POS;
				return FALSE;
			}
			if (PAYPosChk(JIG_CH_2) == FALSE)
			{
				m_LastAlarm = ALARM_PA_CH2_Y_LOAD_POS;
				return FALSE;
			}
			if (PATPosChk(JIG_CH_1) == FALSE)
			{
				m_LastAlarm = ALARM_PA_CH1_T_LOAD_POS;
				return FALSE;
			}
			if (PATPosChk(JIG_CH_2) == FALSE)
			{
				m_LastAlarm = ALARM_PA_CH2_T_LOAD_POS;
				return FALSE;
			}
		}
		else
		{
			if(PAStageUnloadPosChk() == FALSE)
			{
				m_LastAlarm = ALARM_PASTAGE_UNLOAD_POS;
				return FALSE;
			}
		}
*/
		if((CellTagExist(CELL_POS_PA_STAGE_CH1) == TRUE || CellTagExist(CELL_POS_PA_STAGE_CH2) == TRUE)
			&& (CellData_GetPreAlignState(JIG_CH_1) != ALIGN_RESULT_NONE && CellData_GetPreAlignState(JIG_CH_2) != ALIGN_RESULT_NONE))
		{
			if(PAStageUnloadPosChk() == FALSE)
			{
				m_LastAlarm = ALARM_PASTAGE_UNLOAD_POS;
				return FALSE;
			}
		}
		else
		{
			if(PAStageLoadPosChk() == FALSE)
			{
				m_LastAlarm = ALARM_PASTAGE_LOAD_POS;
				return FALSE;
			}
			if (PAXPosChk(JIG_CH_1) == FALSE)
			{
				m_LastAlarm = ALARM_PA_CH1_X_LOAD_POS;
				return FALSE;
			}
			if (PAXPosChk(JIG_CH_2) == FALSE)
			{
				m_LastAlarm = ALARM_PA_CH2_X_LOAD_POS;
				return FALSE;
			}
			if (PAYPosChk(JIG_CH_1) == FALSE)
			{
				m_LastAlarm = ALARM_PA_CH1_Y_LOAD_POS;
				return FALSE;
			}
			if (PAYPosChk(JIG_CH_2) == FALSE)
			{
				m_LastAlarm = ALARM_PA_CH2_Y_LOAD_POS;
				return FALSE;
			}
			if (PATPosChk(JIG_CH_1) == FALSE)
			{
				m_LastAlarm = ALARM_PA_CH1_T_LOAD_POS;
				return FALSE;
			}
			if (PATPosChk(JIG_CH_2) == FALSE)
			{
				m_LastAlarm = ALARM_PA_CH2_T_LOAD_POS;
				return FALSE;
			}
		}

		//2017-04-01, skcho, 가운데 && 연산자 구분하기 위해서 괄호 추가
		//UD_ST와 UD_TR이 셀이 있고 UD_TR이 UD_STAGE위에 있을때 로봇이 움직이지 않음 버그 수정
		//if (CellTagExist(CELL_POS_UD_TR_CH1) || CellTagExist(CELL_POS_UD_TR_CH2)
		//	&& (CellTagExist(CELL_POS_UD_STAGE_CH1) == FALSE && CellTagExist(CELL_POS_UD_STAGE_CH2) == FALSE))
		if ((CellTagExist(CELL_POS_UD_TR_CH1) || CellTagExist(CELL_POS_UD_TR_CH2))
			&& ((CellTagExist(CELL_POS_UD_STAGE_CH1) == FALSE && CellTagExist(CELL_POS_UD_STAGE_CH2) == FALSE)))
		{
			if(MoveUnloadTR_X_PutPosChk() == FALSE)
			{
				m_LastAlarm = ALARM_ULD_TR_X_PUT_POS;
				return FALSE;
			}
		}
		else
		{
			if(MoveUnloadTR_X_GetPosChk() == FALSE)
			{
				m_LastAlarm = ALARM_ULD_TR_X_GET_POS;
				return FALSE;
			}
		}
	}
	else
	{
// CELL이 한개 또는 두개 담길 수 있으므로 시퀀스에서 처리할 수 있도록 한다. 17/04/24 HSK
/*
		if (theProcBank.m_bUpperPutComp)
		{
			if(Chk_UpperShuttle_UnLoadPos() == FALSE)
			{
				m_LastAlarm = ALARM_UPPER_SHUTTLE_UNLOAD_POS;
				return FALSE;
			}
		}
		else if(theProcBank.m_bLowerPutCompInterlock == FALSE)
		{
			if(Chk_UpperShuttle_LoadPos() == FALSE)
			{
				m_LastAlarm = ALARM_UPPER_SHUTTLE_LOAD_POS;
				return FALSE;
			}
		}

		if (theProcBank.m_bLowerPutComp)
		{
			if(Chk_LowerShuttle_UnLoadPos() == FALSE)
			{
				m_LastAlarm = ALARM_LOWER_SHUTTLE_UNLOAD_POS;
				return FALSE;
			}
		}
		else
		{
			if(Chk_LowerShuttle_LoadPos() == FALSE)
			{
				m_LastAlarm = ALARM_LOWER_SHUTTLE_LOAD_POS;
				return FALSE;
			}
		}
*/

		if(CellTagExist(CELL_POS_PA_STAGE_CH1) && CellTagExist(CELL_POS_PA_STAGE_CH2)
			&& (CellData_GetPreAlignState(JIG_CH_1) == ALIGN_RESULT_NONE || CellData_GetPreAlignState(JIG_CH_2) == ALIGN_RESULT_NONE))
		{
			if(Chk_PATR_YAxis_WaitPos() == FALSE)
			{
				m_LastAlarm = ALARM_PATR_Y_WAIT_POS;
				return FALSE;
			}
		}
		else if (CellTagExist(CELL_POS_PA_TR_CH1) && CellTagExist(CELL_POS_PA_TR_CH2))
		{
			if(Chk_PATR_YAxis_PutPos() == FALSE)
			{
				m_LastAlarm = ALARM_PATR_Y_PUT_POS;
				return FALSE;
			}
		}
		else
		{
			if(Chk_PATR_YAxis_GetPos() == FALSE)
			{
				m_LastAlarm = ALARM_PATR_Y_GET_POS;
				return FALSE;
			}
		}
		
/*
		if ((CellTagExist(CELL_POS_PA_STAGE_CH1) == FALSE && CellTagExist(CELL_POS_PA_STAGE_CH2) == FALSE)
			&& (CellData_GetPreAlignState(JIG_CH_1) == ALIGN_RESULT_NONE || CellData_GetPreAlignState(JIG_CH_2) == ALIGN_RESULT_NONE))
		{
			if(PAStageLoadPosChk() == FALSE)
			{
				m_LastAlarm = ALARM_PASTAGE_LOAD_POS;
				return FALSE;
			}
			if (PAXPosChk(JIG_CH_1) == FALSE)
			{
				m_LastAlarm = ALARM_PA_CH1_X_LOAD_POS;
				return FALSE;
			}
			if (PAXPosChk(JIG_CH_2) == FALSE)
			{
				m_LastAlarm = ALARM_PA_CH2_X_LOAD_POS;
				return FALSE;
			}
			if (PAYPosChk(JIG_CH_1) == FALSE)
			{
				m_LastAlarm = ALARM_PA_CH1_Y_LOAD_POS;
				return FALSE;
			}
			if (PAYPosChk(JIG_CH_2) == FALSE)
			{
				m_LastAlarm = ALARM_PA_CH2_Y_LOAD_POS;
				return FALSE;
			}
			if (PATPosChk(JIG_CH_1) == FALSE)
			{
				m_LastAlarm = ALARM_PA_CH1_T_LOAD_POS;
				return FALSE;
			}
			if (PATPosChk(JIG_CH_2) == FALSE)
			{
				m_LastAlarm = ALARM_PA_CH2_T_LOAD_POS;
				return FALSE;
			}
		}
		else
		{
			if(PAStageUnloadPosChk() == FALSE)
			{
				m_LastAlarm = ALARM_PASTAGE_UNLOAD_POS;
				return FALSE;
			}
		}
*/
		if((CellTagExist(CELL_POS_PA_STAGE_CH1) == TRUE && CellTagExist(CELL_POS_PA_STAGE_CH2) == TRUE)
			&& (CellData_GetPreAlignState(JIG_CH_1) != ALIGN_RESULT_NONE && CellData_GetPreAlignState(JIG_CH_2) != ALIGN_RESULT_NONE))
		{
			if(PAStageUnloadPosChk() == FALSE)
			{
				m_LastAlarm = ALARM_PASTAGE_UNLOAD_POS;
				return FALSE;
			}
		}
		else
		{
			if(PAStageLoadPosChk() == FALSE)
			{
				m_LastAlarm = ALARM_PASTAGE_LOAD_POS;
				return FALSE;
			}
			if (PAXPosChk(JIG_CH_1) == FALSE)
			{
				m_LastAlarm = ALARM_PA_CH1_X_LOAD_POS;
				return FALSE;
			}
			if (PAXPosChk(JIG_CH_2) == FALSE)
			{
				m_LastAlarm = ALARM_PA_CH2_X_LOAD_POS;
				return FALSE;
			}
			if (PAYPosChk(JIG_CH_1) == FALSE)
			{
				m_LastAlarm = ALARM_PA_CH1_Y_LOAD_POS;
				return FALSE;
			}
			if (PAYPosChk(JIG_CH_2) == FALSE)
			{
				m_LastAlarm = ALARM_PA_CH2_Y_LOAD_POS;
				return FALSE;
			}
			if (PATPosChk(JIG_CH_1) == FALSE)
			{
				m_LastAlarm = ALARM_PA_CH1_T_LOAD_POS;
				return FALSE;
			}
			if (PATPosChk(JIG_CH_2) == FALSE)
			{
				m_LastAlarm = ALARM_PA_CH2_T_LOAD_POS;
				return FALSE;
			}
		}

		if (CellTagExist(CELL_POS_UD_TR_CH1) && CellTagExist(CELL_POS_UD_TR_CH2)
			&& (CellTagExist(CELL_POS_UD_STAGE_CH1) == FALSE && CellTagExist(CELL_POS_UD_STAGE_CH2) == FALSE))
		{
			if(MoveUnloadTR_X_PutPosChk() == FALSE)
			{
				m_LastAlarm = ALARM_ULD_TR_X_PUT_POS;
				return FALSE;
			}
		}
		else
		{
			if(MoveUnloadTR_X_GetPosChk() == FALSE)
			{
				m_LastAlarm = ALARM_ULD_TR_X_GET_POS;
				return FALSE;
			}
		}
	}

/*
	if (CellTagExist(CELL_POS_LD_TR_CH1) == FALSE && CellTagExist(CELL_POS_LD_TR_CH2) == FALSE)
	{
		if(LDTRXGetPosChk() == FALSE)
		{
			m_LastAlarm = ALARM_LD_TR_X_GET_POS;
			return FALSE;
		}
	}
	else 
	{
		if(LDTRXPutPosChk() == FALSE)
		{
			m_LastAlarm = ALARM_LD_TR_X_PUT_POS;
			return FALSE;
		}
	}
*/
	if(GetTablelPos(ZONE_ID_A) != theProcBank.m_nInspTablePos)
	{
		m_LastAlarm = ALARM_WORK_TABLE_NOT_MOVE;
		return FALSE;
	}

	if(C_Zone_CamXChk() == FALSE)
	{
		m_LastAlarm = ALARM_CZONE_INSP_CAM_X;
		return FALSE;
	}
	if(C_Zone_LCamZChk() == FALSE)
	{
		m_LastAlarm = ALARM_CZONE_INSP_CH1_CAM_Z;
		return FALSE;
	}
	if(C_Zone_LCam_FocusChk() == FALSE)
	{
		m_LastAlarm = ALARM_CZONE_INSP_CH1_CAM_FOCUS;
		return FALSE;
	}
	if(C_Zone_LCam_ThetaChk() == FALSE)
	{
		m_LastAlarm = ALARM_CZONE_INSP_CH1_CAM_THETA;
		return FALSE;
	}
	if(C_Zone_RCamZChk() == FALSE)
	{
		m_LastAlarm = ALARM_CZONE_INSP_CH2_CAM_Z;
		return FALSE;
	}
	if(C_Zone_RCam_FocusChk() == FALSE)
	{
		m_LastAlarm = ALARM_CZONE_INSP_CH2_CAM_FOCUS;
		return FALSE;
	}
	if(C_Zone_RCam_ThetaChk() == FALSE)
	{
		m_LastAlarm = ALARM_CZONE_INSP_CH2_CAM_THETA;
		return FALSE;
	}

	if (theConfigBank.m_Option.m_bUseTrayAlign)
	{
		if(theProcBank.m_bGoodTrayAlignChk == FALSE)
		{
			if(TrayTrY_InChk() == FALSE)
			{
				m_LastAlarm = ALARM_OKTRAY_TR_Y_GET_POS;
				return FALSE;
			}
		}
		else
		{
			if(TrayTrY_OutChk() == FALSE)
			{
				m_LastAlarm = ALARM_OKTRAY_TR_Y_PUT_POS;
				return FALSE;
			}
		}
	}
	else
	{
		if(TrayTrY_OutChk() == FALSE)
		{
			m_LastAlarm = ALARM_OKTRAY_TR_Y_PUT_POS;
			return FALSE;
		}
	}

	if(NGTrayTrX_OutChk() == FALSE)
	{
		m_LastAlarm = ALARM_NGTRAY_TR_X_PUT_POS;
		return FALSE;
	}

	//110
	if(!theConfigBank.m_Option.m_bUseBuffer)
	{
		if(TrayData_Exist(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_IN_LIFT) == FALSE)
		{
			if (InTrayLiftZ_DownChk() == FALSE)
			{
				m_LastAlarm = ALARM_GOOD_TRAY_IN_LIFT_Z_DOWN_POS;
				return FALSE;
			}
		}

		if(TrayData_Exist(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_OUT_LIFT) == FALSE)
		{
			if (OutTrayLiftZ_UpChk() == FALSE)
			{
				m_LastAlarm = ALARM_GOOD_TRAY_OUT_LIFT_Z_UP_POS;
				return FALSE;
			}
		}
	}

	if(TrayData_Exist(NG_TRAY_PORT, TRAY_INFO_POS_NG_OUT_LIFT_ARM) == FALSE)
	{
		if (OutNGTrayLiftZ_UpChk() == FALSE)
		{
			m_LastAlarm = ALARM_NG_TRAY_OUT_LIFT_Z_UP_POS;
			return FALSE;
		}

		if(NGLiftArmFwdBwdChk(NG_BUFFER_TRAY_PORT, CYL_FWD) == FALSE)
		{
			m_LastAlarm = ALARM_NGTRAY_OUT_LIFT_ARM_FWD_CHK;
			return FALSE;
		}
	}

	return TRUE;
}


void CStateRestart::Restart_InitTrayCylinder()
{
	if(TrayData_Exist(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_IN_ALIGN))
	{
		AlignCylFwdBwd(GOOD_IN_TRAY_PORT, CYL_FWD);
		DivisionCylFwdBwd(GOOD_IN_TRAY_PORT, CYL_FWD);
	}
	else
	{
		AlignCylFwdBwd(GOOD_IN_TRAY_PORT, CYL_BWD);
		DivisionCylFwdBwd(GOOD_IN_TRAY_PORT, CYL_BWD);
	}

	if(TrayData_Exist(NG_TRAY_PORT, TRAY_INFO_POS_NG_IN_ALIGN))
	{
		AlignCylFwdBwd(NG_TRAY_PORT, CYL_FWD);
		DivisionCylFwdBwd(NG_TRAY_PORT, CYL_FWD);
	}
	else
	{
		AlignCylFwdBwd(NG_TRAY_PORT, CYL_BWD);
		DivisionCylFwdBwd(NG_TRAY_PORT, CYL_BWD);
	}

	// NG IN LIFT BUFFER에 트레이 있을 경우엔 트레이 UP CYL 동작 2017/05/01 HSK.
	if(TrayData_Exist(NG_TRAY_PORT, TRAY_INFO_POS_NG_IN_BUFFER))
		NGTrayUpDown(NG_TRAY_PORT, CYL_UP);
}

BOOL CStateRestart::Restart_InitTrayCylinderChk()
{
	if(TrayData_Exist(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_IN_ALIGN))
	{
		if (AlignCylFwdBwdChk(GOOD_IN_TRAY_PORT, CYL_FWD) == FALSE)
		{
			m_LastAlarm = ALARM_OKTRAY_IN_ALIGN_FWD_CHK;
			return FALSE;
		}
		if (DivisionCylFwdBwdChk(GOOD_IN_TRAY_PORT, CYL_FWD) == FALSE)
		{
			m_LastAlarm = ALARM_OKTRAY_IN_DIVISION_FWD_CHK;
			return FALSE;
		}
	}
	else
	{
		if (AlignCylFwdBwdChk(GOOD_IN_TRAY_PORT, CYL_BWD) == FALSE)
		{
			m_LastAlarm = ALARM_OKTRAY_IN_ALIGN_BWD_CHK;
			return FALSE;
		}
		if (DivisionCylFwdBwdChk(GOOD_IN_TRAY_PORT, CYL_BWD) == FALSE)
		{
			m_LastAlarm = ALARM_OKTRAY_IN_DIVISION_BWD_CHK;
			return FALSE;
		}
	}

	if(TrayData_Exist(NG_TRAY_PORT, TRAY_INFO_POS_NG_IN_ALIGN))
	{
		if (AlignCylFwdBwdChk(NG_TRAY_PORT, CYL_FWD) == FALSE)
		{
			m_LastAlarm = ALARM_NGTRAY_IN_ALIGN_FWD_CHK;
			return FALSE;
		}
		if (DivisionCylFwdBwdChk(NG_TRAY_PORT, CYL_FWD) == FALSE)
		{
			m_LastAlarm = ALARM_NGTRAY_IN_DIVISION_FWD_CHK;
			return FALSE;
		}
	}
	else
	{
		if (AlignCylFwdBwdChk(NG_TRAY_PORT, CYL_BWD) == FALSE)
		{
			m_LastAlarm = ALARM_NGTRAY_IN_ALIGN_BWD_CHK;
			return FALSE;
		}
		if (DivisionCylFwdBwdChk(NG_TRAY_PORT, CYL_BWD) == FALSE)
		{
			m_LastAlarm = ALARM_NGTRAY_IN_DIVISION_BWD_CHK;
			return FALSE;
		}
	}

	if(TrayData_Exist(NG_TRAY_PORT, TRAY_INFO_POS_NG_IN_BUFFER))
	{
		if(NGTrayUpDownChk(NG_TRAY_PORT, CYL_UP) == FALSE)
			m_LastAlarm = ALARM_NGTRAY_IN_LIFT_TRAY_UP_CHK;
	}

	return TRUE;
}
void CStateRestart::Restart_PAOrgMove()
{
  /*
	AXIS_STP_PA_CH1_X,
	AXIS_STP_PA_CH2_X,
	AXIS_STP_PA_CH1_Y,
	AXIS_STP_PA_CH2_Y,
	AXIS_STP_PA_CH1_T,
	AXIS_STP_PA_CH2_T,
	*/
	int axis;
	int nIndex = 0;
	for(axis =(int)AXIS_STP_PA_CH1_X; axis <= (int)AXIS_STP_PA_CH2_T; axis++)
	{	
		nIndex = axis - (int)AXIS_STP_PA_CH1_X;
		if(theDeviceMotion.OriginRetrun(MP2100_THREAD_MAIN_PROCESS, (AXIS_ID)axis))
		{
			m_PAOriginStatus[nIndex] = ORIGIN_START;
		}
		else
		{
			m_PAOriginStatus[nIndex] = ORIGIN_FAIL;		
		}
	}		

}
BOOL CStateRestart::Restart_PAOrgMoveChk()
{
	BOOL bReturn = TRUE;
	int axis;
	int nIndex = 0;
	for(axis =(int)AXIS_STP_PA_CH1_X; axis <= (int)AXIS_STP_PA_CH2_T; axis++)
	{
		nIndex = axis - (int)AXIS_STP_PA_CH1_X;
		if(theDeviceMotion.IsAxisReady(MP2100_THREAD_MAIN_PROCESS, (AXIS_ID)axis, FALSE))
		{
			m_PAOriginStatus[nIndex] = ORIGIN_FINISH;
		}
		else if(theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, (AXIS_ID)axis).bAlarmOn)
		{
			m_PAOriginStatus[nIndex] = ORIGIN_FAIL;
		}
		else
		{
			bReturn = FALSE;
		}		
	}
	return bReturn;
}
BOOL CStateRestart::CheckAxisOriginFail()
{
	for(int i = 0; i < 6; i++)
	{
		if(m_PAOriginStatus[i] == ORIGIN_FAIL)
		{
			m_LastAlarm = ALARM_PASTAGE_ORIGIN_POS;
			return TRUE;
		}
	}
	return FALSE;
}