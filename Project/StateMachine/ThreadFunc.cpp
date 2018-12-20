#include "stdafx.h"
#include "CheetahApp.h"
#include "MainFrm.h"
#include "ThreadFunc.h"
#include "Device/DeviceMotion.h"

#include "StateMachine/StateRestart.h"
#include "StateMachine/StateLoader.h"
#include "StateMachine/StateAMT.h"
#include "StateMachine/StateUnloader.h"

#include "UI/Dialog/DlgTMDDownLoad.h"

//DUONG
#include "BankGuild/UnitCtrlBank.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
CStateManualReceive				StateManualReceive(STATE_MANUALRECEIVE, MP2100_THREAD_MAIN_SEQUENCE);
CStateConvMove					StateConvMove(STATE_CONVMOVE, MP2100_THREAD_MAIN_SEQUENCE);
CStateLDRobotGet				StateLDRobotGet(STATE_LDROBOTGET, MP2100_THREAD_MAIN_SEQUENCE);
CStateLDRobotUpperPut			stateLDRobotUpperPut(STATE_LDROBOTUPPERPUT, MP2100_THREAD_MAIN_SEQUENCE);
CStateLDRobotLowerPut			stateLDRobotLowerPut(STATE_LDROBOTLOWERPUT, MP2100_THREAD_MAIN_SEQUENCE);
CStateLowerShuttleGet			stateLowerShuttleGet(STATE_LOWERSHUTTLEGET, MP2100_THREAD_MAIN_SEQUENCE);
CStateLowerShuttlePut			stateLowerShuttlePut(STATE_LOWERSHUTTLEPUT, MP2100_THREAD_MAIN_SEQUENCE);
CStateUpperShuttleGet			stateUpperShuttleGet(STATE_UPPERSHUTTLEGET, MP2100_THREAD_MAIN_SEQUENCE);
CStateUpperShuttlePut			stateUpperShuttlePut(STATE_UPPERSHUTTLEPUT, MP2100_THREAD_MAIN_SEQUENCE);
CStatePATRUpperGet				statePATRUpperGet(STATE_PATRUPPERGET, MP2100_THREAD_MAIN_SEQUENCE);
CStatePATRLowerGet				statePATRLowerGet(STATE_PATRLOWERGET, MP2100_THREAD_MAIN_SEQUENCE);
CStatePATRPut					StatePATRPut(STATE_PATRPUT, MP2100_THREAD_MAIN_SEQUENCE);
CStatePAStageGet				StatePAStageGet(STATE_PASTAGEGET, MP2100_THREAD_MAIN_SEQUENCE);
CStatePAStage					StatePAStage(STATE_PASTAGE, MP2100_THREAD_MAIN_SEQUENCE);
CStatePAStagePut				StatePAStagePut(STATE_PASTAGEPUT, MP2100_THREAD_MAIN_SEQUENCE);
CStateLDTRGet					StateLDTRGet(STATE_LDTRGET, MP2100_THREAD_MAIN_SEQUENCE);
CStateLowerMCR					StateLowerMCR(STATE_LOWERMCR, MP2100_THREAD_MAIN_SEQUENCE);
CStateLDTRPut					StateLDTRPut(STATE_LDTRPUT, MP2100_THREAD_MAIN_SEQUENCE);
CStateUpperMCR					StateUpperMCR(STATE_UPPERMCR, MP2100_THREAD_MAIN_SEQUENCE);

CStateAMTTable					StateAMTTable(STATE_AMTTABLE, MP2100_THREAD_MAIN_SEQUENCE);
CStateAZone						StateAZone(STATE_AZONE, MP2100_THREAD_MAIN_SEQUENCE);
CStateBZone						StateBZone(STATE_BZONE, MP2100_THREAD_MAIN_SEQUENCE);
CStateCZone						StateCZone(STATE_CZONE, MP2100_THREAD_MAIN_SEQUENCE);
CStateDZone						StateDZone(STATE_DZONE, MP2100_THREAD_MAIN_SEQUENCE);

CStateUDTRGet					StateUDTRGet(STATE_UDTRGET, MP2100_THREAD_MAIN_SEQUENCE);
CStateUDTRPut					StateUDTRPut(STATE_UDTRPUT, MP2100_THREAD_MAIN_SEQUENCE);
CStateUDRobotGet				StateUDRobotGet(STATE_UDROBOTGET, MP2100_THREAD_MAIN_SEQUENCE);
CStateUDRobotGoodPut			RobotGoodPutstateUD(STATE_UDROBOTPUT1, MP2100_THREAD_MAIN_SEQUENCE);
CStateUDRobotNGPut				stateUDRobotNGPut(STATE_UDROBOTPUT2, MP2100_THREAD_MAIN_SEQUENCE);
//2018-05-14,SKCHO CONV �߰�
CStateUDRobotConvPut			stateUDRobotConvPut(STATE_UDROBOTPUT3, MP2100_THREAD_MAIN_SEQUENCE);


//20180801wz
CStateGoodTrayGetPut            StateGoodTrayGetPut(STATE_GOODTRAYGETPUT, MP2100_THREAD_MAIN_SEQUENCE);
CStateUDRobotPick2              StateUDRobotPick2(STATE_UDROBOTPICK2,MP2100_THREAD_MAIN_SEQUENCE);
///////////////////////////////////////////////////////////////////////////////////////////////////////////

CStateJobChange					StateJobChange(STATE_JOBCHANGE, MP2100_THREAD_MAIN_SEQUENCE);

CStateGoodTrayInLift			StateGoodTrayInLift(STATE_GOODTRAYINLIFT, MP2100_THREAD_MAIN_SEQUENCE);
CStateGoodTrayInLiftPitch		StateGoodTrayInLiftPitch(STATE_GOODTRAYINLIFTPITCH, MP2100_THREAD_MAIN_SEQUENCE);
CStateGoodTrayTRGet				StateGoodTrayTRGet(STATE_GOODTRAYTRGET, MP2100_THREAD_MAIN_SEQUENCE);
CStateGoodTrayTRPut				StateGoodTrayTRPut(STATE_GOODTRAYTRPUT, MP2100_THREAD_MAIN_SEQUENCE);
CStateGoodTrayDCR				StateGoodTrayDCR(STATE_GOODTRAYDCR, MP2100_THREAD_MAIN_SEQUENCE);
CStateGoodTrayAlign				StateGoodTrayAlign(STATE_GOODTRAYALIGN, MP2100_THREAD_MAIN_SEQUENCE);
CStateGoodTrayOutLift			StateGoodTrayOutLift(STATE_GOODTRAYOUTLIFT, MP2100_THREAD_MAIN_SEQUENCE);

CStateNGTrayIn					StateNGTrayIn(STATE_NGTRAYIN, MP2100_THREAD_MAIN_SEQUENCE);
CStateNGTrayInLiftUp			StateNGTrayInLiftUp(STATE_NGTRAYINLIFTUP, MP2100_THREAD_MAIN_SEQUENCE);
CStateNGTrayInLiftPitch			StateNGTrayInLiftPitch(STATE_NGTRAYINLIFTPITCH, MP2100_THREAD_MAIN_SEQUENCE);
CStateNGTrayTRGet				StateNGTrayTRGet(STATE_NGTRAYTRGET, MP2100_THREAD_MAIN_SEQUENCE);
CStateNGTrayTRPut				StateNGTrayTRPut(STATE_NGTRAYTRPUT, MP2100_THREAD_MAIN_SEQUENCE);
CStateNGTrayAlign				StateNGTrayAlign(STATE_NGTRAYALIGN, MP2100_THREAD_MAIN_SEQUENCE);
CStateNGTrayOutLift				StateNGTrayOutLift(STATE_NGTRAYOUTLIFT, MP2100_THREAD_MAIN_SEQUENCE);

CStateGoodTrayReceive			stateGoodTrayReceive(STATE_GOODTRAYRECEIVE, MP2100_THREAD_AGV_INTERFACE);
CStateNGTrayReceive				stateNGTrayReceive(STATE_NGTRAYRECEIVE, MP2100_THREAD_AGV_INTERFACE);
CStateGoodTraySend				StateGoodTraySend(STATE_GOODTRAYSEND, MP2100_THREAD_AGV_INTERFACE);
CStateNGTraySend				StateNGTraySend(STATE_NGTRAYSEND, MP2100_THREAD_AGV_INTERFACE);

CStateCellReceive				StateCellReceive(STATE_CELLRECEIVE, MP2100_THREAD_CELLRCV_INTERFACE);

CStateRestart					stateRestart(STATE_RESTART, MP2100_THREAD_MAIN_SEQUENCE);

//2017-09-17,SKCHO
CStateGoodTrayManualReceive		stateGoodTrayManualReceive(STATE_GOODTRAYMANUALRECEIVE, MP2100_THREAD_TRAY_MANUAL_RECEIVE);
/*CStateCH1_T5ReStart						CStateCH1_T5ReStart	(STATE_DZONE, MP2100_THREAD_MAIN_SEQUENCE);
CStateCH2_T5ReStart						CStateCH2_T5ReStart	(STATE_DZONE, MP2100_THREAD_MAIN_SEQUENCE);
CStateCH3_T5ReStart						CStateCH3_T5ReStart	(STATE_DZONE, MP2100_THREAD_MAIN_SEQUENCE);
CStateCH4_T5ReStart						CStateCH4_T5ReStart	(STATE_DZONE, MP2100_THREAD_MAIN_SEQUENCE);
CStateCH5_T5ReStart						CStateCH5_T5ReStart	(STATE_DZONE, MP2100_THREAD_MAIN_SEQUENCE);
CStateCH6_T5ReStart						CStateCH6_T5ReStart	(STATE_DZONE, MP2100_THREAD_MAIN_SEQUENCE);
CStateCH7_T5ReStart						CStateCH7_T5ReStart	(STATE_DZONE, MP2100_THREAD_MAIN_SEQUENCE);
CStateCH8_T5ReStart						CStateCH8_T5ReStart	(STATE_DZONE, MP2100_THREAD_MAIN_SEQUENCE);
*/
UINT MainSequenceThr(LPVOID lpParam)
{
	// MP2100 ����� ��� �� Thread���� Open
	theDeviceMotion.Open_Mp2100(MP2100_THREAD_MAIN_SEQUENCE);

	int nThreadIndex = (int)lpParam;

	// Thread������ _func�� ���� ���� �ȵ�!!! �Ʒ� _func�� ����
	CUnitCtrlFunc					_func(MP2100_THREAD_MAIN_SEQUENCE);	// �Ʒ� ��ü�� �̸��� �ʹ� �� �ϳ� �� ����


	CONST_MACHINE_STATE::ID _LastState = CONST_MACHINE_STATE::MACHINE_PAUSE;

	static int itest=0;

	//2018wjp
	BOOL bIncheck=FALSE;
	CStopWatch m_checkstage;
	while(theThreadInitializer.m_bThrExist[nThreadIndex] == FALSE)
	{
		//DUONG
		/*if(theProcBank.Test_duong==TRUE)
		{
		/CUnitCtrlBank check;
			check.CIM_CarrierAssign();
			m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_ID=_T("1234567");
			//WriteCarrierReqCellIDInfo(port,pos);
			theProcBank.Test_duong=FALSE;`

		}*/
		//2018wjp
		BOOL bTemp=theProcBank.m_bInPick2Model;
		if(theConfigBank.m_Option.m_bUseBuffer&&(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN||
			theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)&&!theConfigBank.m_Option.m_bUseDryRun
			&&theProcBank.m_bIsRestartDone)
		{
			BOOL bTemp=FALSE;
			if(theConfigBank.m_Option.m_bUseConveyorTestModel)
				bTemp=TRUE;
			else
			{
				if(_func.GetInPutIOState(X_HYBRID_CONV_READY) == TRUE&&(_func.GetInPutIOState(X_HYBRID_CONV_INTERFACE) == TRUE))
					bTemp=TRUE;
				else
					bTemp=FALSE;
			}
			bTemp=TRUE;
			//110
			//_func.GetInPutIOState(X_HYBRID_CONV_READY) == TRUE  //�����̾� ��尡 TRUE�϶��� 
				//&& (_func.GetInPutIOState(X_HYBRID_CONV_INTERFACE) == TRUE) //2018-05-15,NAM, Check Hybrid Conv Mode 
			if(bTemp&&theConfigBank.m_Option.m_bUseConveyorMode
				//&&theProcBank.HybridDelayFlag
				)
			{
				if(_func.CellTagExist(CELL_POS_UD_STAGE_CH1) == FALSE && _func.CellTagExist(CELL_POS_UD_STAGE_CH2)== FALSE)//!theProcBank.m_bTest
				{
					if(!bIncheck)
					{
						m_checkstage.Start();
						bIncheck=TRUE;
					}
					if(m_checkstage.Stop(FALSE)>theConfigBank.m_Option.m_nBufferUseWaitTime&&(
						_func.ULDTrayLiftTrayChk(SENSOR_ON)||theUnitFunc.TrayTr_VacChk(VAC_ON)||((!_func.ULDTrayLiftTrayChk(SENSOR_ON)&&
						(((theProcBank.GOODTrayBitX>=1||theProcBank.GOODTrayBitY>=1)))))))//&&!theUnitFunc.TrayTr_VacChk(VAC_ON)
					{
						theProcBank.m_bInPick2Model=TRUE;
					}
					else
					{
						m_checkstage.Stop(FALSE);
						theProcBank.m_bInPick2Model=FALSE;
						if(theProcBank.m_bPickOUT2INTrayRequest)
							theProcBank.m_bPickOUT2INTrayRequest=FALSE;
					}

				}
				else
				{
					bIncheck=FALSE;
					m_checkstage.Stop(FALSE);
					theProcBank.m_bInPick2Model=FALSE;
					if(theProcBank.m_bPickOUT2INTrayRequest)
						theProcBank.m_bPickOUT2INTrayRequest=FALSE;
				}
			}
			else
			{
				bIncheck=FALSE;
				theProcBank.m_bInPick2Model=FALSE;
				m_checkstage.Stop(FALSE);
				if(theProcBank.m_bPickOUT2INTrayRequest)
					theProcBank.m_bPickOUT2INTrayRequest=FALSE;
			}
		}
		else
		{
			theProcBank.m_bInPick2Model=FALSE;
			bIncheck=FALSE;
			m_checkstage.Stop(FALSE);
			if(theProcBank.m_bPickOUT2INTrayRequest)
				theProcBank.m_bPickOUT2INTrayRequest=FALSE;
		}
		//Model Changel
		if(bTemp!=theProcBank.m_bInPick2Model)
		{
			if(theProcBank.m_bInPick2Model)
			{
				if((theProcBank.i_ModelChange2To1!=1)||theProcBank.m_bInitModelChange)
				{
					theProcBank.i_ModelChange1To2=1;
					theProcBank.i_ModelChange2To1=0;	
					theProcBank.m_bInitModelChange=FALSE;
				}
				else
				{
					theProcBank.i_ModelChange1To2=0;
					theProcBank.i_ModelChange2To1=0;
				}
			}
			else
			{
				if(theProcBank.i_ModelChange1To2!=1)
				{
					theProcBank.i_ModelChange1To2=0;
					theProcBank.i_ModelChange2To1=1;	
				}
				else
				{
					theProcBank.i_ModelChange1To2=0;
					theProcBank.i_ModelChange2To1=0;
				}
			}
		}

		Sleep(5);
		//-------------------------------------------------------------------------------------------------------------------------------
		//--- EQ State Check
		//-------------------------------------------------------------------------------------------------------------------------------
		if(_LastState != theProcBank.GetMachineState())
		{
			//2018-04-20,SKCHO,MACHINE_CHECK�߰�, RESTART�� �˶� �߻����� ��� ���񿡼��� ó���ϱ� ���ؼ� �߰�
			//if((theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)
			if((theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_CHECK)
				&& (_LastState == CONST_MACHINE_STATE::MACHINE_INIT || _LastState == CONST_MACHINE_STATE::MACHINE_PAUSE
				|| _LastState == CONST_MACHINE_STATE::MACHINE_FAULT || _LastState == CONST_MACHINE_STATE::MACHINE_INTERLOCK))
			{				
				// ����� �����....
				theLog[LOG_SEQUENCE].AddBuf(_T("        \tThread Resume"));
				/*****************Init Buffer**************************/
				//if(theProcBank.m_bInPick2Model)

				/******************************************************/
				stateRestart.Start();
				StateLDRobotGet.Reset();
				stateUpperShuttleGet.Reset();
				stateLowerShuttleGet.Reset();
				stateLDRobotUpperPut.Reset();
				stateLDRobotLowerPut.Reset();
				stateUpperShuttlePut.Reset();
				stateLowerShuttlePut.Reset();
				statePATRUpperGet.Reset();
				statePATRLowerGet.Reset();
				StatePATRPut.Reset();
				StatePAStageGet.Reset();
				StatePAStage.Reset();
				StatePAStagePut.Reset();
				StateLDTRGet.Reset();
				StateLowerMCR.Reset();
				StateLDTRPut.Reset();
				StateUpperMCR.Reset();

				StateAMTTable.Reset();
				StateAZone.Reset();
				StateBZone.Reset();
				StateCZone.Reset();
				StateDZone.Reset();

				StateJobChange.Reset();

				StateUDTRGet.Reset();
				StateUDTRPut.Reset();
				StateUDRobotGet.Reset();
				stateUDRobotGoodPut.Reset();
				stateUDRobotNGPut.Reset();

				//2018-05-04,SKCHO, CONV�߰�
				stateUDRobotConvPut.Reset();

				StateGoodTrayInLift.Reset();
				StateGoodTrayInLiftPitch.Reset();
				StateGoodTrayTRGet.Reset();
				StateGoodTrayTRPut.Reset();
				StateGoodTrayDCR.Reset();
				StateGoodTrayAlign.Reset();
				StateGoodTrayOutLift.Reset();
				
				StateNGTrayIn.Reset();
				StateNGTrayInLiftUp.Reset();
				StateNGTrayInLiftPitch.Reset();
				StateNGTrayTRGet.Reset();
				StateNGTrayTRPut.Reset();
				StateNGTrayAlign.Reset();

				StateNGTrayOutLift.Reset(); //2017-04-18, skcho, �����Ǿ� �߰�
				StateUDRobotPick2.Reset();//2018wjp
				StateGoodTrayGetPut.Reset();//2018wjp

			}
			//2018-04-20,SKCHO,MACHINE_CHECK�߰�
			//else if((_LastState == CONST_MACHINE_STATE::MACHINE_RUN || _LastState == CONST_MACHINE_STATE::MACHINE_IDLE)
			else if((_LastState == CONST_MACHINE_STATE::MACHINE_RUN || _LastState == CONST_MACHINE_STATE::MACHINE_IDLE || _LastState == CONST_MACHINE_STATE::MACHINE_CHECK)
				&& (theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_INIT || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_PAUSE
				|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_FAULT || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_INTERLOCK))
			{
				// �����ϴ� �����...
				stateRestart.Reset();

				theLog[LOG_SEQUENCE].AddBuf(_T("        \tThread Pause"));
			}

			_LastState = theProcBank.GetMachineState();
		}
		//2018-05-07,NAM, Last state and current state is same (MACHINE_CHECK) 
		else 
		{
			if (theProcBank.m_ChangeState1==1)	//CHECK == CHECK SAME THE MACHINE_STATE
			{
				stateRestart.Start();			//STARTE THE RESTART
				//2018-05-07,NAM,reset the value
				theProcBank.m_ChangeState1=0;
			}
		}
		
		// 2018-06-21,NAM
		if(theProcBank.m_AlarmID != ALM_NONE)
		{
			if ((theProcBank.m_AlarmID == ALARM_NGTRAY_IN_PORT_ALIGN_EMPTY  // 2018-04-24,NAM 
				|| theProcBank.m_AlarmID == ALARM_OKTRAY_IN_PORT_ALIGN_EMPTY
				|| theProcBank.m_AlarmID == ALARM_OKTRAY_OUT_BUFF_PORT_FULL)
				&& theProcBank.m_RestartState==TRUE)
			{
				stateRestart.Start();
				theProcBank.m_RestartState=FALSE;
			}
		}

		// Restart�� �Ϸ�Ǿ�� �ٸ� State�� �����Ѵ�
		if( (stateRestart.IsHolded() == FALSE) || stateRestart.IsStoped() )
		{
			
			stateRestart.Run();
			continue;
		}

		// ���� Alarm ���¸� ���� ����
		if(theProcBank.m_AlarmID != ALM_NONE)
		{
			if(theProcBank.m_AlarmID != ALARM_CIM_ATTRIBUTE_FAIL
				&& theProcBank.m_AlarmID != ALARM_OKTRAY_ID_ASSIGN_REPORT_FAIL
				&& theProcBank.m_AlarmID != ALARM_OKTRAY_ID_ASSIGN_REPORT_TIMEOUT
				&& theProcBank.m_AlarmID != ALARM_OKTRAY_ID_RELEASE_REPORT_FAIL
				&& theProcBank.m_AlarmID != ALARM_OKTRAY_ID_RELEASE_REPORT_TIMEOUT
				//2018-06-09,SKCHO,AGV PORT FULL/EMPTY �˶� �߻��� ���� �ȵǰ� �ؾ��ϹǷ� ����
				/*&& theProcBank.m_AlarmID != ALARM_NGTRAY_IN_PORT_ALIGN_EMPTY  // 2018-04-24,NAM 
				&& theProcBank.m_AlarmID != ALARM_OKTRAY_IN_PORT_ALIGN_EMPTY
				&& theProcBank.m_AlarmID != ALARM_OKTRAY_OUT_BUFF_PORT_FULL*/
				) 
			{
				continue;
			}
		}
/*
		// ���� RUN ���°� �ƴϸ� ���� ����
		if(_LastState != CONST_MACHINE_STATE::MACHINE_RUN)
		{
			continue;
		}
*/
		if(_LastState == CONST_MACHINE_STATE::MACHINE_PAUSE && _LastState == CONST_MACHINE_STATE::MACHINE_INIT
			&& _LastState == CONST_MACHINE_STATE::MACHINE_FAULT && _LastState == CONST_MACHINE_STATE::MACHINE_INTERLOCK)   
		{
			continue;
		}
		//-------------------------------------------------------------------------------------------------------------------------------
		//--- Step, Wake up! 
		//-------------------------------------------------------------------------------------------------------------------------------
		// Restart�� �Ϸ�Ǿ�� �ٸ� State�� �����Ѵ�
		if( (stateRestart.IsHolded() == FALSE) || stateRestart.IsStoped() )
		{
			
			stateRestart.Run();
			continue;
		}
		//2018wjp
		theProcBank.m_bIsRestartDone=TRUE;
		//////////////////////////////////////////////////////////////////////////
		// �� State���� Start���� Ȯ�� �� Start ����

		//LD Conv�� �δ� �κ��� ���� �ִ��� �������� üũ.
		if ( StateLDRobotGet.IsStoped()
			&& stateLDRobotUpperPut.IsStoped()
			&& stateLDRobotLowerPut.IsStoped()
			&& (_func.CellTagExist(CELL_POS_LD_CONV) == TRUE)
			&& (_func.CellTagExist(CELL_POS_LDROBOT) == FALSE))
		{
			// �κ��� �� ������ ���� ������
			StateLDRobotGet.Start();
		}

		if (stateUpperShuttleGet.IsStoped()
			&& stateUpperShuttlePut.IsStoped()
			&& stateLDRobotUpperPut.IsStoped()
			&& stateLDRobotLowerPut.IsStoped()
			&& statePATRUpperGet.IsStoped()
			&& (_func.CellTagExist(CELL_POS_UPPER_SHUTTLE_CH1) == FALSE && _func.CellTagExist(CELL_POS_UPPER_SHUTTLE_CH2) == FALSE)
			&& _func.Chk_UpperShuttle_LoadPos() == FALSE && theProcBank.m_bLowerPutCompInterlock == FALSE)
		{
			// Upper Shuttle�� �ε� ��ġ�� �̵� �ϴ� ������
			stateUpperShuttleGet.Start();
		}

		//Lower Shuttler�� Cell�� ���� Lower Shuttle�� �ε� ��ġ�� �ƴҶ�
		if (stateLowerShuttleGet.IsStoped()
			&& stateLowerShuttlePut.IsStoped()
			&& stateLDRobotLowerPut.IsStoped()
			&& statePATRLowerGet.IsStoped()
			&& (_func.CellTagExist(CELL_POS_LOWER_SHUTTLE_CH1) == FALSE && _func.CellTagExist(CELL_POS_LOWER_SHUTTLE_CH2) == FALSE)
			&& _func.Chk_LowerShuttle_LoadPos() == FALSE)
		{
			// Lower Shuttle�� �ε� ��ġ�� �̵� �ϴ� ������
			stateLowerShuttleGet.Start();
		}
		//�κ��� Cell���ְ� Upper Shuttle�� Get Pos�� �ְ� Cell ���� ��.
		if (stateLDRobotUpperPut.IsStoped()
			&& stateLDRobotLowerPut.IsStoped()
			&& StateLDRobotGet.IsStoped()
			&& stateUpperShuttleGet.IsStoped()
			&& stateUpperShuttlePut.IsStoped()
			&& _func.CellTagExist(CELL_POS_LDROBOT) == TRUE && _func.Chk_UpperShuttle_LoadPos() == TRUE 
			&& (_func.CellTagExist(CELL_POS_UPPER_SHUTTLE_CH1) == FALSE || _func.CellTagExist(CELL_POS_UPPER_SHUTTLE_CH2) == FALSE))
		{
			// �δ� �κ��� UpperShuttler�� �� ��ε� �ϴ� ������
			stateLDRobotUpperPut.Start();
		}
		//�κ��� Cell���ְ� Lower Shuttle�� Get Pos�� �ְ� Cell�� ���� Upper Shuttler�� �ε� ��ġ�� �ƴ� ��.
		if (stateLDRobotUpperPut.IsStoped()
			&& stateLDRobotLowerPut.IsStoped()
			&& StateLDRobotGet.IsStoped()
			&& stateLowerShuttleGet.IsStoped()
			&& stateLowerShuttlePut.IsStoped()
			&& stateUpperShuttleGet.IsStoped()
			&& stateUpperShuttlePut.IsStoped()
			&& _func.CellTagExist(CELL_POS_LDROBOT) == TRUE && _func.Chk_LowerShuttle_LoadPos() == TRUE && _func.Chk_UpperShuttle_LoadPos() == FALSE
			&& (_func.CellTagExist(CELL_POS_LOWER_SHUTTLE_CH1) == FALSE || _func.CellTagExist(CELL_POS_LOWER_SHUTTLE_CH2) == FALSE))
		{
			// �δ� �κ��� Lower Shuttle�� �� ��ε� �ϴ� ������
			stateLDRobotLowerPut.Start();
		}

		if(_func.IsJobEnd())
		{
			if (stateUpperShuttlePut.IsStoped()
			&& stateUpperShuttleGet.IsStoped()
			&& statePATRUpperGet.IsStoped()
			&& statePATRLowerGet.IsStoped()
			&& stateLDRobotUpperPut.IsStoped()
			&& (_func.CellTagExist(CELL_POS_UPPER_SHUTTLE_CH1) == TRUE || _func.CellTagExist(CELL_POS_UPPER_SHUTTLE_CH2) == TRUE )
			&& _func.Chk_PATR_ZAxis_UpPos()
			&& _func.Chk_UpperShuttle_UnLoadPos() == FALSE)
			{
				//Upper Shuttle�� ��ε� ��ġ�� �̵� �ϴ� ������
				stateUpperShuttlePut.Start();
			}
		}
		else
		{
			if (stateUpperShuttlePut.IsStoped()
			&& stateUpperShuttleGet.IsStoped()
			&& statePATRUpperGet.IsStoped()
			&& statePATRLowerGet.IsStoped()
			&& stateLDRobotUpperPut.IsStoped()
			&& (_func.CellTagExist(CELL_POS_UPPER_SHUTTLE_CH1) == TRUE || _func.CellTagExist(CELL_POS_UPPER_SHUTTLE_CH2) == TRUE )
			&& _func.Chk_PATR_ZAxis_UpPos()
			&& _func.Chk_UpperShuttle_UnLoadPos() == FALSE
			&& theProcBank.m_bUpperPutComp == TRUE)
			{
				//Upper Shuttle�� ��ε� ��ġ�� �̵� �ϴ� ������
				stateUpperShuttlePut.Start();
			}
		}
	
		if(_func.IsJobEnd())
		{
			//Lower Shuttler�� Cell�� �ְ� Unload �������� �ƴϰ� PA TR Z���� Get Lower Shuttle�� �ƴ� ��
			if (stateLowerShuttlePut.IsStoped()
				&& stateLowerShuttleGet.IsStoped() 
				&& statePATRLowerGet.IsStoped()
				&& stateLDRobotLowerPut.IsStoped()
				&& _func.Chk_PATR_ZAxis_GetLowerShuttlePos() == FALSE
				&& (_func.CellTagExist(CELL_POS_LOWER_SHUTTLE_CH1) == TRUE || _func.CellTagExist(CELL_POS_LOWER_SHUTTLE_CH2) == TRUE)
				&& _func.Chk_LowerShuttle_UnLoadPos() == FALSE)
			{
				//Lower Shuttle�� ��ε� ��ġ�� �̵� �ϴ� ������
				stateLowerShuttlePut.Start();
			}
		}
		else
		{
			if (stateLowerShuttlePut.IsStoped()
				&& stateLowerShuttleGet.IsStoped() 
				&& statePATRLowerGet.IsStoped()
				&& stateLDRobotLowerPut.IsStoped()
				&& _func.Chk_PATR_ZAxis_GetLowerShuttlePos() == FALSE
				&& (_func.CellTagExist(CELL_POS_LOWER_SHUTTLE_CH1) == TRUE || _func.CellTagExist(CELL_POS_LOWER_SHUTTLE_CH2) == TRUE)
				&& _func.Chk_LowerShuttle_UnLoadPos() == FALSE
				&& theProcBank.m_bLowerPutComp == TRUE)
			{
				//Lower Shuttle�� ��ε� ��ġ�� �̵� �ϴ� ������
				stateLowerShuttlePut.Start();
			}
		}

		//PA TR�� Cell�� ���� Upper Shuttle�� Cell�� �ְ� Upper Shuttle�� Put��ġ �� ��
		if (statePATRUpperGet.IsStoped()
			&& statePATRLowerGet.IsStoped()
			&& StatePATRPut.IsStoped()
			&& stateUpperShuttleGet.IsStoped()
			&& stateUpperShuttlePut.IsStoped()
			&& StatePAStage.IsStoped()
			&& (_func.CellTagExist(CELL_POS_PA_TR_CH1) == FALSE && _func.CellTagExist(CELL_POS_PA_TR_CH2) == FALSE)
			&& (_func.CellTagExist(CELL_POS_UPPER_SHUTTLE_CH1) == TRUE || _func.CellTagExist(CELL_POS_UPPER_SHUTTLE_CH2) == TRUE) 
			&& _func.Chk_UpperShuttle_UnLoadPos() == TRUE)
		{
			//PA TR�� Upper Shuttle �� �ε� �ϴ� ������
			statePATRUpperGet.Start();
		}

		//PA TR�� Cell�� ���� Lower Shuttle�� Cell�� �ְ� Lower Shuttle�� Put��ġ �̰� Upper Shuttle�� �ε� ��ġ �� ��
		if (statePATRLowerGet.IsStoped()
			&& statePATRUpperGet.IsStoped()
			&& StatePATRPut.IsStoped()
			&& stateUpperShuttleGet.IsStoped()
			&& stateUpperShuttlePut.IsStoped()
			&& stateLowerShuttleGet.IsStoped()
			&& stateLowerShuttlePut.IsStoped()
			&& StatePAStage.IsStoped()
			&& (_func.CellTagExist(CELL_POS_PA_TR_CH1) == FALSE && _func.CellTagExist(CELL_POS_PA_TR_CH2) == FALSE)
			&& (_func.CellTagExist(CELL_POS_LOWER_SHUTTLE_CH1) == TRUE || _func.CellTagExist(CELL_POS_LOWER_SHUTTLE_CH2) == TRUE) 
			&& _func.Chk_UpperShuttle_LoadPos() == TRUE && _func.Chk_LowerShuttle_UnLoadPos() == TRUE )
		{
			// PA TR�� Lower Shuttle �� �ε� �ϴ� ������
			statePATRLowerGet.Start();
		}
		//PA Stage�� Cell�� ���� PA Stage�� �ε� �������� �ƴ� �� 
		if (StatePAStageGet.IsStoped()
			&& StatePAStagePut.IsStoped()
			&& StatePAStage.IsStoped()
			&& StateLDTRGet.IsStoped()
			&& _func.CellTagExist(CELL_POS_PA_STAGE_CH1) == FALSE && _func.CellTagExist(CELL_POS_PA_STAGE_CH2) == FALSE
			&& _func.PAStageLoadPosChk() == FALSE)
		{
			// PA Stage�� �ε� ��ġ�� �̵� �ϴ� ������
			StatePAStageGet.Start();
		}
		//PA TR�� Cell�� �ְ� PA Stage�� Cell�� ���� PA Stage�� Get ��ġ �϶�.
		if (StatePATRPut.IsStoped()
			&& statePATRUpperGet.IsStoped()
			&& statePATRLowerGet.IsStoped()
			&& StatePAStage.IsStoped()
			&& (_func.CellTagExist(CELL_POS_PA_TR_CH1) == TRUE || _func.CellTagExist(CELL_POS_PA_TR_CH2) == TRUE)
			//2017-11-15,SKCHO, PA STAGE�� CELL�� ��� �������� PUT������(1�������� PUT �����ؼ� CELL�� ��ħ �߻�)
			//&& (_func.CellTagExist(CELL_POS_PA_STAGE_CH1) == FALSE || _func.CellTagExist(CELL_POS_PA_STAGE_CH2) == FALSE)
			&& (_func.CellTagExist(CELL_POS_PA_STAGE_CH1) == FALSE && _func.CellTagExist(CELL_POS_PA_STAGE_CH2) == FALSE)
			&& _func.PAStageLoadPosChk() == TRUE)
		{
			// PA TR�� ��ε� �ϴ� ������
			StatePATRPut.Start();
		}
		// PA ��� �� ���.
		if (theConfigBank.m_Option.m_bUsePreAlign)
		{
			//PA Stage�� Cell�� �ְ� Align Data�� ������
			if (StatePAStage.IsStoped()
				&& StatePAStageGet.IsStoped()
				&& StatePAStagePut.IsStoped()
				&& statePATRLowerGet.IsStoped()
				&& statePATRUpperGet.IsStoped()
				&& StatePATRPut.IsStoped()
				&& StateLDTRGet.IsStoped()
				&& (_func.CellData_GetPreAlignState(JIG_CH_1) == ALIGN_RESULT_NONE || _func.CellData_GetPreAlignState(JIG_CH_2) == ALIGN_RESULT_NONE))
			{
				// PreAlign �ϴ� ������
				StatePAStage.Start();
			}
		}

		//PA Stage Cell�� �ְ� Align Data�� �ְ� PA Stage�� ��ε� �������� �ƴҶ�
		if (StatePAStagePut.IsStoped()
			&& StatePAStageGet.IsStoped()
			&& StatePAStage.IsStoped()
			&& StateLDTRGet.IsStoped()
			&& StatePATRPut.IsStoped()
			&& _func.PAStageUnloadPosChk() == FALSE
			&& (_func.CellTagExist(CELL_POS_PA_STAGE_CH1) == TRUE || _func.CellTagExist(CELL_POS_PA_STAGE_CH2) == TRUE)
			&& (_func.CellData_GetPreAlignState(JIG_CH_1) != ALIGN_RESULT_NONE || _func.CellData_GetPreAlignState(JIG_CH_2) != ALIGN_RESULT_NONE))
		{
			// PA Stage ��ε� ���������� �̵��ϴ� ������
			StatePAStagePut.Start();
		}

		//PA Stage�� Cell�� �ְ� PA Stage�� Put ��ġ�� LD TR�� Cell�� ���� ��
		if (StateLDTRGet.IsStoped()
			&& StateLowerMCR.IsStoped()
			&& StateLDTRPut.IsStoped()
			&& StateUpperMCR.IsStoped()
			&& StatePAStage.IsStoped()
			&& _func.PAStageUnloadPosChk() == TRUE
			&& (_func.CellTagExist(CELL_POS_LD_TR_CH1) == FALSE && _func.CellTagExist(CELL_POS_LD_TR_CH2) == FALSE)
			&& (_func.CellTagExist(CELL_POS_PA_STAGE_CH1) == TRUE || _func.CellTagExist(CELL_POS_PA_STAGE_CH2) == TRUE)
			&& theProcBank.m_bUpperMCRCompChk == TRUE)
		{
			// LD TR�� �ε� �ϴ� ������
			StateLDTRGet.Start();
		}	

		// Lower MCR �ɼ��� ��� �� ���
		//if (theConfigBank.m_Option.m_bUseLowerMCRRead)
		//{
			// PA Stage�� Get ��ġ�� �ְ� LDTR�� Cell�� �ְ� MCR ���� ������ ���� ��
			if (StateLowerMCR.IsStoped()
				&& StateLDTRGet.IsStoped()
				&& StateLDTRPut.IsStoped()
				&& StateUpperMCR.IsStoped()
				&& (_func.CellData_UVReadingChk(LOWER_MCR,JIG_CH_1) == MCR_RESULT_NONE || _func.CellData_UVReadingChk(LOWER_MCR,JIG_CH_2) == MCR_RESULT_NONE)
				&& (_func.CellTagExist(CELL_POS_LD_TR_CH1) == TRUE || _func.CellTagExist(CELL_POS_LD_TR_CH2) == TRUE))
			{
				// Lower MCR �д� ������.
				StateLowerMCR.Start();
			}
		//}
	
	
		// Table ���� ������ LD TR�� Cell�� �ְ� MCR Data�� �ְ� A Zone�� Cell�� ������
		if (StateLDTRPut.IsStoped()
			&& StateLowerMCR.IsStoped()
			&& StateLDTRGet.IsStoped()
			&& StateUpperMCR.IsStoped()
			&& StateAMTTable.IsStoped()
			&& StateAZone.IsStoped()
			&& (_func.CellTagExist(CELL_POS_LD_TR_CH1) == TRUE || _func.CellTagExist(CELL_POS_LD_TR_CH2) == TRUE)
			&& (_func.CellTagExist(CELL_POS_AZONE_CH1) == FALSE && _func.CellTagExist(CELL_POS_AZONE_CH2) == FALSE)
			&& _func.GetZoneEnd(ZONE_ID_A) == FALSE
			&& (_func.CellData_UVReadingChk(LOWER_MCR,JIG_CH_1) != MCR_RESULT_NONE && _func.CellData_UVReadingChk(LOWER_MCR,JIG_CH_2) != MCR_RESULT_NONE)
			&& (_func.JigSkipCheck() == FALSE))
			//&& (_func.ChSkipJigIDCheck() == FALSE)) // Jig ch Skip üũ ���ε� ���� ����. 2017/04/26 HSK
		{
			// LD TR ��ε� �ϴ� ������
			StateLDTRPut.Start();
		}

		// Upper MCR �ɼ��� ��� �� ���
		if (theConfigBank.m_Option.m_bUseUpperMCRRead)
		{
			// Lower MCR�� �������� �õ� �߰� �д°� �����ϰ� A Zone�� Cell�� ���� ���
			if (StateUpperMCR.IsStoped()
				&& StateLowerMCR.IsStoped()
				&& StateLDTRGet.IsStoped()
				&& StateLDTRPut.IsStoped()
				&& (_func.CellTagExist(CELL_POS_AZONE_CH1) == TRUE || _func.CellTagExist(CELL_POS_AZONE_CH2) == TRUE)
				&& (_func.CellData_UVReadingChk(UPPER_MCR,JIG_CH_1) != MCR_RESULT_GOOD || _func.CellData_UVReadingChk(UPPER_MCR,JIG_CH_2) != MCR_RESULT_GOOD)
				&& theProcBank.m_bUpperMCRCompChk == FALSE)
			{
				// Upper MCR �д� ������
				StateUpperMCR.Start();
			}
		}

		// ������ ���ÿ��� ������ üũ ���� ���� �̻�� 2017/04/26 HSK.
		/*
		if (theConfigBank.m_Option.m_bUseRepeatInsp)
		{
			if (StateAMTTable.IsStoped()
				&& StateLDTRPut.IsStoped()
				&& StateAZone.IsStoped()
				&& StateBZone.IsStoped()
				&& StateCZone.IsStoped()
				&& StateDZone.IsStoped()
				&& StateUpperMCR.IsStoped()
				&& _func.GetZoneEnd(ZONE_ID_A)
				&& _func.GetZoneEnd(ZONE_ID_B)
				&& _func.GetZoneEnd(ZONE_ID_C)
				&& _func.GetZoneEnd(ZONE_ID_D))
			{
				//AMT TABLE ȸ�� �ϴ� ������
				StateAMTTable.Start();
			}
		}
		else
		{
		*/
			if (StateAMTTable.IsStoped()
				&& StateLDTRPut.IsStoped()
				&& StateAZone.IsStoped()
				&& StateBZone.IsStoped()
				&& StateCZone.IsStoped()
				&& StateDZone.IsStoped()
				&& StateUpperMCR.IsStoped()
				&& _func.GetZoneEnd(ZONE_ID_A)
				&& _func.GetZoneEnd(ZONE_ID_B)
				&& _func.GetZoneEnd(ZONE_ID_C)
				&& _func.GetZoneEnd(ZONE_ID_D)
				&& (_func.CellTagExist(CELL_POS_DZONE_CH1) == FALSE && _func.CellTagExist(CELL_POS_DZONE_CH2) == FALSE)
				&& theProcBank.m_bAllUnitInterlock_Stop == FALSE)  //2017-12-05,SKCHO,��ü ���Ͷ��� �ƴҶ�  ����
			{
				//AMT TABLE ȸ�� �ϴ� ������
				StateAMTTable.Start();
			}
		//}

		// A Zone End ���� Ȯ�� �� A Zone Cell�� ���� ��
		if (_func.IsJobEnd())
		{
			if (StateAZone.IsStoped()
				&& StateLDTRPut.IsStoped()
				&& StateAMTTable.IsStoped()
				&& StateUpperMCR.IsStoped()
				&& _func.GetZoneEnd(ZONE_ID_A) == FALSE)
//				&& (_func.CellDataMachineChk(CELL_POS_LD_CONV,CELL_POS_LD_TR_CH2) == FALSE))
			{
				StateAZone.Start();
			}
		}
		else
		{
			if (StateAZone.IsStoped()
				&& StateLDTRPut.IsStoped()
				&& StateAMTTable.IsStoped()
				&& StateUpperMCR.IsStoped()
				&& _func.GetZoneEnd(ZONE_ID_A) == FALSE
				&& ((_func.CellTagExist(CELL_POS_AZONE_CH1) == TRUE) || (_func.CellTagExist(CELL_POS_AZONE_CH2) == TRUE)
				|| (_func.JigSkipCheck() == TRUE)))
				//|| (_func.ChSkipJigIDCheck() == TRUE)))// Jig ch Skip üũ ���ε� ���� ����. 2017/04/26 HSK
			{
				// A Zone ������ ����.
				StateAZone.Start();
			}
		}

		// B Zone End ���� Ȯ��.
		if (StateBZone.IsStoped()
			&& StateAMTTable.IsStoped()
			&& _func.GetZoneEnd(ZONE_ID_B) == FALSE)
		{
			// B Zone ������ ����.
			StateBZone.Start();
		}
		// C Zone End Ȯ��.
		if (StateCZone.IsStoped()
			&& StateAMTTable.IsStoped()
			&& _func.GetZoneEnd(ZONE_ID_C) == FALSE)
		{
			// C Zone ������ ����.
			StateCZone.Start();
		}
		// D Zone End Ȯ��.
		if (StateDZone.IsStoped()
			&& StateAMTTable.IsStoped()
			&& StateUDTRGet.IsStoped()
			&& _func.GetZoneEnd(ZONE_ID_D) == FALSE)
		{
			// D Zone ������ ����.
			StateDZone.Start();
		}

		// D Zone �� UD TR Cell ���� ���� Ȯ��.
		// ������ ���ÿ��� UD TR�� ���� ����� �ȵ� ���� �̻�� 2017/04/26 HSK.
		/*
		if (theConfigBank.m_Option.m_bUseRepeatInsp == FALSE)
		{
			if (StateUDTRGet.IsStoped()
				&& StateUDTRPut.IsStoped()
				&& StateAMTTable.IsStoped()
				&& StateDZone.IsStoped()
				&& (_func.CellTagExist(CELL_POS_DZONE_CH1) == TRUE || _func.CellTagExist(CELL_POS_DZONE_CH2) == TRUE)
				&& (_func.CellTagExist(CELL_POS_UD_TR_CH1) == FALSE && _func.CellTagExist(CELL_POS_UD_TR_CH2) == FALSE))
			{
				// UD TR Get ���� ������
				StateUDTRGet.Start();
			}
		}
		*/
		if (StateUDTRGet.IsStoped()
			&&StateUDTRPut.IsStoped()
			&& StateAMTTable.IsStoped()
			&& StateDZone.IsStoped()
			&& (_func.CellTagExist(CELL_POS_DZONE_CH1) == TRUE || _func.CellTagExist(CELL_POS_DZONE_CH2) == TRUE)
			&& (_func.CellTagExist(CELL_POS_UD_TR_CH1) == FALSE && _func.CellTagExist(CELL_POS_UD_TR_CH2) == FALSE))
		{
			// UD TR Get ���� ������
			StateUDTRGet.Start();
		}

		// UD TR �� UD STAGE Cell ���� Ȯ��.
		if (StateUDTRPut.IsStoped()
			&& StateUDTRGet.IsStoped()
			&& StateUDRobotGet.IsStoped()
			&& (_func.CellTagExist(CELL_POS_UD_TR_CH1) == TRUE || _func.CellTagExist(CELL_POS_UD_TR_CH2) == TRUE)
			&& (_func.CellTagExist(CELL_POS_UD_STAGE_CH1) == FALSE && _func.CellTagExist(CELL_POS_UD_STAGE_CH2) == FALSE))
		{
			// UD TR Put ���� ������
			StateUDTRPut.Start();
		}

		// UDRobot �� UD Stage Cell ���� Ȯ�� �� UD TR X Get ��ġ �϶�.
		if (StateGoodTrayGetPut.IsStoped()//2018wjp
			&&StateUDRobotPick2.IsStoped()//2018wjp
			&&StateUDRobotGet.IsStoped() && stateUDRobotGoodPut.IsStoped() && stateUDRobotNGPut.IsStoped() && StateUDTRPut.IsStoped()
			&& stateUDRobotConvPut.IsStoped() //2018-05-18,NAM, Robot Get when Robot Conv Put.
			&& _func.CellTagExist(CELL_POS_UD_ROBOT) == FALSE && _func.MoveUnloadTR_X_GetPosChk() == TRUE
			&& (_func.CellTagExist(CELL_POS_UD_STAGE_CH1) == TRUE || _func.CellTagExist(CELL_POS_UD_STAGE_CH2) == TRUE))
			
		{
			// UD Robot Get ���� ������
			StateUDRobotGet.Start();
		}

		if (theConfigBank.m_UnloadRunInfo.GetJobChangeType() == JOBCHANGE_CASE2	&& StateJobChange.IsStoped())
		{
			StateJobChange.Start();
		}

		// UD Robot�� Cell�� �ְ� Good In Align Tray Data�� �ְ� Cell�� ������ �ʾ����� Last Class�� Good Cell �̰� Good Tray TR Y�� Out ��ġ �� ��.
		if (StateUDRobotPick2.IsStoped()//2018wjp interlock
			&&StateGoodTrayGetPut.IsStoped()//2018wjp interlock
			&&StateUDRobotGet.IsStoped()
			&& stateUDRobotGoodPut.IsStoped()
			&& stateUDRobotNGPut.IsStoped() 
			&& StateGoodTrayTRGet.IsStoped() 
			&& StateGoodTrayTRPut.IsStoped()
			&& _func.CellTagExist(CELL_POS_UD_ROBOT)
			&& _func.TrayData_Exist(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_ALIGN)
			&& _func.CellData_GetLastClass(CELL_POS_UD_ROBOT) == GOOD_CELL
			&& _func.TrayTrY_OutChk() == TRUE && theProcBank.m_bDCRChk == TRUE
			&& _func.TrayData_CellIsFull(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_ALIGN) == FALSE
			&& theConfigBank.m_UnloadRunInfo.GetJobChangeType() == JOBCHANGE_NONE
			//&& (theProcBank.m_TrayBank.GetTrayInfo(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_OUT_LIFT)->GetTrayCount() < theConfigBank.m_UnloadRunInfo.GetTrayLotQtyInt())) //2017-03-21, skcho
		    && (theProcBank.GetGoodOutLiftCount() < theConfigBank.m_UnloadRunInfo.GetTrayLotQtyInt())//2017-04-20, skcho
			//2018-06-29,NAM, Choose the Option for Hybrid Conveyor MOde 
			//&& _func.GetInPutIOState(X_HYBRID_CONV_READY) == FALSE  //2018-05-14,SKCHO, �����̾� ��尡 FALSE�϶��� PUT
			//&& _func.GetInPutIOState(X_HYBRID_CONV_INTERFACE) ==TRUE  //2018-05-15,NAM,Check Hybrid Conv Mode 
			&& (( ((_func.GetInPutIOState(X_HYBRID_CONV_READY) == FALSE)  || (_func.GetInPutIOState(X_HYBRID_CONV_INTERFACE) == FALSE))   //2018-05-14,SKCHO, �����̾� ��尡 FALSE�϶��� PUT
			&& (theConfigBank.m_Option.m_bUseConveyorMode )&& theProcBank.HybridDelayFlag == TRUE) || (theConfigBank.m_Option.m_bUseConveyorMode==FALSE))
			&& stateUDRobotConvPut.IsStoped() //2018-05-22, NAM,Check UD ROBOT Conv Put Stop 
			&&!theProcBank.m_bInPick2Model) // Should move inside when Use Conveyor Mode  
		{
			if(theProcBank.i_ModelChange2To1==1)
			{
				if(theProcBank.m_bInitModelChange)
					theProcBank.m_bInitModelChange=FALSE;
				theProcBank.i_ModelChange2To1=0;
				if(theProcBank.GOODTrayBitX>=theRecipeBank.m_Tray.m_nCountX&&
					theProcBank.GOODTrayBitY>=theRecipeBank.m_Tray.m_nCountY)
				{
					//full
					theProcBank.GOODTrayBitX=theRecipeBank.m_Tray.m_nCountX;
					theProcBank.GOODTrayBitY=theRecipeBank.m_Tray.m_nCountY;
				}
				else
				{
					if(theProcBank.GOODTrayBitX==0&&theProcBank.GOODTrayBitY==0)
					{
						theProcBank.GOODTrayBitX=1;
						theProcBank.GOODTrayBitY=1;
					}
					else
					{
						if(theProcBank.GOODTrayBitX!=theRecipeBank.m_Tray.m_nCountX)
							theProcBank.GOODTrayBitX++;
						else
						{
							theProcBank.GOODTrayBitY++;
							theProcBank.GOODTrayBitX=1;
						}
					}
				}
			}
			stateUDRobotGoodPut.Start();
		}

		//2018-05-14,SKCHO, CONV�߰�
		// INTERFACE(���), READY ON�̰�, CONV RUN OFF�̰�, �������� �����ӵ� STOP �����̰� , 
		// NG TRAY Y OUT ��ġ �̰� Last Class�� Good Cell�϶�
		if (StateUDRobotPick2.IsStoped()//2018wjp interlock
			&&_func.GetInPutIOState(X_HYBRID_CONV_READY) == TRUE  //�����̾� ��尡 TRUE�϶��� 
			&& (_func.GetInPutIOState(X_HYBRID_CONV_INTERFACE) == TRUE) //2018-05-15,NAM, Check Hybrid Conv Mode 
			&& theConfigBank.m_Option.m_bUseConveyorMode  //2018-06-29,NAM, Choose the Option for Conveyor.
			&& stateUDRobotConvPut.IsStoped()
			&& StateUDRobotGet.IsStoped() 
			&& stateUDRobotGoodPut.IsStoped()
			&& stateUDRobotNGPut.IsStoped() 
			&& StateGoodTrayTRGet.IsStoped() 
			&& StateGoodTrayTRPut.IsStoped()
			&& StateNGTrayTRGet.IsStoped()
			&& StateNGTrayTRPut.IsStoped()
			&& _func.NGTrayTrX_OutChk() == TRUE
			&& _func.CellTagExist(CELL_POS_UD_ROBOT)
			//&& _func.CellData_GetLastClass(CELL_POS_UD_ROBOT) == GOOD_CELL		
			&& theConfigBank.m_UnloadRunInfo.GetJobChangeType() == JOBCHANGE_NONE )  //2017-03-10,skcho
		{
			// UD Robot CONV ���� ������
			stateUDRobotConvPut.Start();
		}
		

		// UD Robot�� Cell�� �ְ� NG Align Tray Data�� �ְ� Cell�� ������ �ʾ����� Last Class�� Good Cell�� �ƴϰ� NG Tray TR X�� Out ��ġ �� ��.
		if (
			StateUDRobotPick2.IsStoped()//2018wjp interlock
			//&&StateGoodTrayGetPut.IsStoped()//2018wjp interlockStateUDRobotGet.IsStoped()
			&& stateUDRobotGoodPut.IsStoped()
			&& stateUDRobotNGPut.IsStoped() 
			&& StateNGTrayTRGet.IsStoped()
			&& StateNGTrayTRPut.IsStoped()
			&& _func.NGTrayTrX_OutChk() == TRUE
			&& _func.CellTagExist(CELL_POS_UD_ROBOT)
			&& _func.CellData_GetLastClass(CELL_POS_UD_ROBOT) != GOOD_CELL
			&& _func.TrayData_Exist(NG_TRAY_PORT,TRAY_INFO_POS_NG_IN_ALIGN)
			&& _func.TrayData_CellIsFull(NG_TRAY_PORT,TRAY_INFO_POS_NG_IN_ALIGN) == FALSE
			&& theConfigBank.m_UnloadRunInfo.GetJobChangeType() == JOBCHANGE_NONE )  //2017-03-10,skcho
		{
			// UD Robot NG Put ���� ������
			stateUDRobotNGPut.Start();
		}


		// Good In Conv�� Ʈ���� ���� Good In Buf Conv�� Ʈ���� �ְ� �Ǹ��� ���� Ȯ��.
		if (StateGoodTrayGetPut.IsStoped()
			&&StateUDRobotPick2.IsStoped()//2018wjp interlock
			&&StateGoodTrayGetPut.IsStoped()//2018wjp interlock
			&&StateGoodTrayInLift.IsStoped()
			&& StateGoodTrayInLiftPitch.IsStoped()
			&& stateUDRobotGoodPut.IsStoped()
//			&& _func.BottomAlignCylFwdBwdChk(GOOD_IN_TRAY_PORT, CYL_BWD) // ������ �ȿ��� ���� ��Ű�� 2017/04/29 HSK
//			&& _func.BottomStoperUpDownChk(GOOD_IN_TRAY_PORT,CYL_DOWN,TRAY_IN)
// 			&& _func.InConv_TrayChk(CONV_SEN_2, SENSOR_OFF)
// 			&& _func.InConv_TrayChk(CONV_SEN_3, SENSOR_OFF)
// 			&& _func.InConv_TrayChk(CONV_SEN_4, SENSOR_OFF)
// 			&& _func.BufConvSenChk(GOOD_IN_TRAY_PORT,SENSOR_ON)
			&& _func.TrayData_Exist(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_BUFFER)
			&& (_func.TrayData_Exist(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_LIFT) == FALSE))
		{
			// Good Tray In �ϴ� ������.
			StateGoodTrayInLift.Start();
		}


		//lot end�� job change�϶��� ��ġ���ϰ� release ���� �Ҷ� m_bCarrierAssign= true�� ���� �ϹǷ� good align tray count �ʱ�ȭ�� ���� ���ؼ� �����̿Ϸ� �� �� ��ġ���ϰ�
		if(theConfigBank.m_Option.m_bUseLotEnd || theConfigBank.m_UnloadRunInfo.GetJobChangeType() == JOBCHANGE_CASE1) 
		{
			//Top Tray�� ���� Lift�� Tray�� �ְ� �Ǹ��� ���� Ȯ��.
			if (!theProcBank.m_bInPick2Model//2018wjp
				&&StateUDRobotPick2.IsStoped()//2018wjp interlock
			    &&StateGoodTrayGetPut.IsStoped()//2018wjp interlockStateGoodTrayInLiftPitch.IsStoped()
				&& StateGoodTrayInLift.IsStoped()
				// Lift Pitch �̵� �� �� TR�� ������ ���� ������ �־ �߰�. 2017/04/25 HSK
				&& StateGoodTrayTRGet.IsStoped()
				&& theProcBank.m_bCarrierReportWaitCheck == FALSE//������ �����⸦ ��ٸ���.			
				&& _func.LDTrayLiftTrayChk(SENSOR_ON)  // ���� üũ ������ ������ �� ��쿡 Ʈ���̰� ��� ��ġ ���ϴ� ��찡 ���� ���־ ���ܵ�. 2017/04/29 HSK
//				&& _func.AlignCylFwdBwdChk(GOOD_IN_TRAY_PORT, CYL_BWD)
//				&& _func.DivisionCylFwdBwdChk(GOOD_IN_TRAY_PORT, CYL_BWD) 
				&& (_func.TrayData_Exist(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_ALIGN) == FALSE)
				&& theProcBank.m_bDCRChk == TRUE  //2017-07-04,SKCHO, Ʈ���� ������ DCR���� �����ϹǷ� m_bDCRChk������ ���� ���� �߰�, ������ ALIGN�� ����ؼ� ��
				&& (_func.TrayData_Exist(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_LIFT) == TRUE))
			{
				// Good Tray Lift Pitch Up�ϴ� ������
				StateGoodTrayInLiftPitch.Start();
			}
		}
		else
		{
			//Top Tray�� ���� Lift�� Tray�� �ְ� �Ǹ��� ���� Ȯ��.
			if (!theProcBank.m_bInPick2Model//2018wjp
				&&StateUDRobotPick2.IsStoped()//2018wjp interlock
			    &&StateGoodTrayGetPut.IsStoped()//2018wjp interlock
				&&StateGoodTrayInLiftPitch.IsStoped()
				&& StateGoodTrayInLift.IsStoped()
				// Lift Pitch �̵� �� �� TR�� ������ ���� ������ �־ �߰�. 2017/04/25 HSK
				&& StateGoodTrayTRGet.IsStoped()
				&& _func.LDTrayLiftTrayChk(SENSOR_ON) 
				&& theProcBank.m_bCarrierReportWaitCheck == FALSE
//				&& _func.AlignCylFwdBwdChk(GOOD_IN_TRAY_PORT, CYL_BWD)
//				&& _func.DivisionCylFwdBwdChk(GOOD_IN_TRAY_PORT, CYL_BWD) 
				&& (_func.TrayData_Exist(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_ALIGN) == FALSE)  
				&& theProcBank.m_bDCRChk == TRUE   //2017-07-04,SKCHO, Ʈ���� ������ DCR���� �����ϹǷ� m_bDCRChk������ ���� ���� �߰�, ������ ALIGN�� ����ؼ� ��
				&& (_func.TrayData_Exist(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_LIFT) == TRUE))
			{
				// Good Tray Lift Pitch Up�ϴ� ������
				StateGoodTrayInLiftPitch.Start();
			}
		}

		//2017-07-06,SKCHO,  ���� ��û���� DCR �ɼ� ��� ����
		//if (theConfigBank.m_Option.m_bUseDCR)
		//{
			if (StateGoodTrayDCR.IsStoped() && StateGoodTrayInLiftPitch.IsStoped()
				//--&& _func.TrayData_Exist(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_IN_ALIGN) == TRUE  //2017-07-04,SKCHO,Ʈ���� ���� ���� ��ġ�� DCR �б� ���� �������� �����Ͽ� �������� ����
				&& theProcBank.m_bDCRChk == FALSE)
			{
				StateGoodTrayDCR.Start();
			}
		//}

		//Lot End�� �� A Zone ���� UD Robot���� Cell�� ���� �� Tray�� Cell�� �������� Ȯ�� ���� �ʴ´�.
			if(theConfigBank.m_Option.m_bUseLotEnd || theConfigBank.m_UnloadRunInfo.GetJobChangeType() == JOBCHANGE_CASE1) 
			{
				if (
					StateGoodTrayTRGet.IsStoped()
					&& StateGoodTrayTRPut.IsStoped()
					&& StateGoodTrayInLiftPitch.IsStoped()
					&& stateUDRobotGoodPut.IsStoped()
					&& (_func.TrayData_Exist(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_TR) == FALSE)
					&& ((_func.CellDataMachineChk(CELL_POS_LD_CONV,CELL_POS_UD_ROBOT) == FALSE
					||( _func.TrayData_CellIsFull(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_ALIGN)) || theConfigBank.m_UnloadRunInfo.GetJobChangeType() == JOBCHANGE_CASE1))
					//&& (_func.TrayData_ExistCell(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_IN_ALIGN) == FALSE)// TRUE)  //2017-03-02,skcho, ������ ����� �ϹǷ� Ʈ���̿� ���̾�� ��
					//&& (theProcBank.m_TrayBank.GetTrayInfo(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_OUT_LIFT)->GetTrayFull() == FALSE)  //2017-03-02,skcho, maxtray���� ������ ����´�.
					&& _func.TrayData_Exist(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_ALIGN)
					&& theProcBank.m_bCarrierAssign == FALSE
					&& stateUDRobotConvPut.IsStoped()
					&&StateUDRobotPick2.IsStoped()//2018wjp interlock
					&&StateGoodTrayGetPut.IsStoped()//2018wjp interlock
					&&!theProcBank.m_bInPick2Model)  //2018-05-14,SKCHO
				{
					//2017-07-06,SKCHO,  ���� ��û���� DCR �ɼ� ��� ����
					//if (theConfigBank.m_Option.m_bUseDCR)
					//{
					if (theProcBank.m_bDCRChk)
					{
						// Good Tray TR �ε� ������
						StateGoodTrayTRGet.Start();
					}
					//}
					//else
					//	StateGoodTrayTRGet.Start();
				} 
			}
		else
		{			//Tray TR�� Tray�� ���� In Top Tray�� cell �� �� á����
			if (StateGoodTrayTRGet.IsStoped()
				&& StateGoodTrayTRPut.IsStoped()
				&& StateGoodTrayInLiftPitch.IsStoped()
				&& stateUDRobotGoodPut.IsStoped()
				&& _func.TrayData_Exist(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_ALIGN)
				&& (((_func.TrayData_CellIsFull(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_ALIGN))) 
				//|| (theProcBank.m_TrayBank.GetTrayInfo(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_OUT_LIFT)->GetTrayCount() == theConfigBank.m_UnloadRunInfo.GetTrayLotQtyInt()))  //2017-03-21,skcho
				|| ((theProcBank.GetGoodOutLiftCount() == theConfigBank.m_UnloadRunInfo.GetTrayLotQtyInt())))  //2017-04-20,skcho
				&& (_func.TrayData_Exist(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_TR) == FALSE)
				&& stateUDRobotConvPut.IsStoped()//2018-05-14,SKCHO)
				&&StateUDRobotPick2.IsStoped()//2018wjp interlock
			    &&StateGoodTrayGetPut.IsStoped()//2018wjp interlock
				&&!theProcBank.m_bInPick2Model)//2018wjp interlock
			{
				//2017-07-06,SKCHO,  ���� ��û���� DCR �ɼ� ��� ����
				//if (theConfigBank.m_Option.m_bUseDCR)
				//{
				if (theProcBank.m_bDCRChk)
				{
					// Good Tray TR �ε� ������
					StateGoodTrayTRGet.Start();
				}

				//}
				//else
				//	StateGoodTrayTRGet.Start();
			}
		}

		if (theConfigBank.m_Option.m_bUseTrayAlign)
		{
			if (StateGoodTrayAlign.IsStoped() && StateGoodTrayOutLift.IsStoped()
				&& StateGoodTrayTRPut.IsStoped()
				&& theProcBank.m_bGoodTrayAlignChk == FALSE
				&& _func.TrayData_Exist(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_OUT_LIFT))
			{
				StateGoodTrayAlign.Start();
			}
		}

		//Good Tray TR�� Tray�� ���� ��
		if  (StateUDRobotPick2.IsStoped()//2018wjp interlock
			&&StateGoodTrayGetPut.IsStoped()//2018wjp interlock
			&&StateGoodTrayTRPut.IsStoped() && StateGoodTrayTRGet.IsStoped()
			&& _func.TrayTr_VacChk(VAC_ON) && StateGoodTrayAlign.IsStoped()
			&& StateGoodTrayOutLift.IsStoped()
			&& _func.TrayData_Exist(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_TR))
		{
			// Good Tray TR ��ε� ������.
			StateGoodTrayTRPut.Start();
		}

		// Lot End�̰� ���� Cell�� ���� �� Out Lift�� Ʈ���� �������� üũ ���� �ʴ´�.
		if(theConfigBank.m_Option.m_bUseLotEnd  || theConfigBank.m_UnloadRunInfo.GetJobChangeType() == JOBCHANGE_CASE1)  //PPID�� �ٸ���� ���� �˻���ǰ lot end ó��
		{
			if (StateGoodTrayGetPut.IsStoped()//2018wjp interlock
				&&StateGoodTrayOutLift.IsStoped()
				&& StateGoodTrayTRGet.IsStoped()
				&& StateGoodTrayTRPut.IsStoped()
				&& StateGoodTrayAlign.IsStoped()
				// Alarm �߻� �ÿ� Buf Conv�� Ʈ���̰� �ְ� ����Ÿ�� �ƿ� ����Ʈ�� ���� ��� ����۽� �ش� ������ Ż�� �ֵ��� ���� 2017/06/06 HSK
				//&& _func.BufConvSenChk(GOOD_OUT_TRAY_PORT,SENSOR_OFF)
				&& (theProcBank.m_bOKTrayTRLotEnd == TRUE || (_func.TrayData_ExistCell(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_IN_ALIGN) == FALSE)) // 2017.2.6 bgkim
				&& (_func.CellDataMachineChk(CELL_POS_LD_CONV,CELL_POS_UD_ROBOT) == FALSE || theConfigBank.m_UnloadRunInfo.GetJobChangeType() == JOBCHANGE_CASE1)
				&& (_func.TrayData_Exist(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_OUT_BUFFER) == FALSE)
				&& _func.TrayData_Exist(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_OUT_LIFT)
				&& _func.TrayData_TrayIsFull(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_OUT_LIFT))  //2017-04-12,skcho,ī��Ʈ ������ �Ѳ����� �Ǵ���,�� ������ ������ �Ѳ����� ������.
				//&& theProcBank.m_TrayBank.GetTrayInfo(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_OUT_LIFT)->GetTrayFull())  //2017-03-02,skcho, maxtray���� ũ�� ����Ʈ�� ������.)
			{
				StateGoodTrayOutLift.Start();
			}
		}
		else
		{
			// Out Lift�� Tray ���� á�� ���� Conv�� Tray ���� ��
			if (StateGoodTrayGetPut.IsStoped()//2018wjp interlock
				&&StateGoodTrayOutLift.IsStoped() && StateGoodTrayTRPut.IsStoped()
				&& StateGoodTrayAlign.IsStoped()
				// Alarm �߻� �ÿ� Buf Conv�� Ʈ���̰� �ְ� ����Ÿ�� �ƿ� ����Ʈ�� ���� ��� ����۽� �ش� ������ Ż�� �ֵ��� ���� 2017/06/06 HSK
				//&& _func.BufConvSenChk(GOOD_OUT_TRAY_PORT,SENSOR_OFF)
				&& _func.TrayData_TrayIsFull(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_OUT_LIFT)
				&& (_func.TrayData_Exist(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_OUT_BUFFER) == FALSE))
			{
				// Good Tray ���� ������.
				StateGoodTrayOutLift.Start();
			}
		}

		// NG Tray REQ Buffer�� Ʈ���̰� �ְ� NG Tray In Buffer�� Ʈ���̰� ���� IN Lift Arm�� Ʈ���̰� 10������ ���� ��
		if (StateNGTrayIn.IsStoped()
			// �����ϰ��� ���� �� �˶� �߻� �� ��쿡 �Ǹ��� ������ ���� �ȵǴ� ���� �־� ������ �������� ����.17/04/22
//			&& _func.NGTrayUpDownChk(NG_TRAY_PORT, CYL_DOWN)
//			&& _func.BottomStoperUpDownChk(NG_TRAY_PORT, CYL_DOWN, TRAY_IN)
			&& theProcBank.m_TrayBank.GetTrayInfo(NG_TRAY_PORT, TRAY_INFO_POS_NG_IN_LIFT_ARM)->GetTrayCount() < 10
// 			&& _func.NGConv_TrayChk(NG_TRAY_PORT, CONV_SEN_2, SENSOR_OFF)
// 			&& _func.NGConv_TrayChk(NG_TRAY_PORT, CONV_SEN_3, SENSOR_OFF)
// 			&& _func.NGConv_TrayChk(NG_TRAY_PORT, CONV_SEN_4, SENSOR_OFF)
// 			&& _func.NGConv_TrayChk(NG_BUFFER_TRAY_PORT, CONV_SEN_2, SENSOR_ON)
// 			&& _func.NGConv_TrayChk(NG_BUFFER_TRAY_PORT, CONV_SEN_3, SENSOR_ON)
// 			&& _func.NGConv_TrayChk(NG_BUFFER_TRAY_PORT, CONV_SEN_4, SENSOR_ON)
			&& _func.TrayData_Exist(NG_TRAY_PORT,TRAY_INFO_POS_NG_OUT_REQ_BUFFER)
			&& (_func.TrayData_Exist(NG_TRAY_PORT,TRAY_INFO_POS_NG_IN_BUFFER) == FALSE))
		{
			// NG Tray ���� ������.
			StateNGTrayIn.Start();
		}

		//NG Tray In Buffer Port�� Ʈ���̰� �ְ�, In Lift Arm�� Ʈ���̰� ���� ��
		if (StateNGTrayInLiftUp.IsStoped() && StateNGTrayIn.IsStoped()
			&& StateNGTrayInLiftPitch.IsStoped() && stateUDRobotNGPut.IsStoped()
			&& _func.TrayData_Exist(NG_TRAY_PORT,TRAY_INFO_POS_NG_IN_BUFFER)
			&& (_func.TrayData_Exist(NG_TRAY_PORT,TRAY_INFO_POS_NG_IN_LIFT_ARM) == FALSE))
		{
			// NG Tray Lift Arm Up ��ġ �̵� ������
			StateNGTrayInLiftUp.Start();
		}

		//�Ǹ��� ���� Ȯ�� �ϰ� , Align Tray Data ���� Lift Arm�� Tray Data ���� ��
		if (StateNGTrayInLiftPitch.IsStoped()
			&& StateNGTrayInLiftUp.IsStoped()
			&& StateNGTrayIn.IsStoped()
			&& StateNGTrayTRGet.IsStoped()
			&& stateUDRobotNGPut.IsStoped() 
//			&& _func.AlignCylFwdBwdChk(NG_TRAY_PORT, CYL_BWD) // ������ �ȿ��� ���� ��Ű�� 2017/04/29 HSK
//			&& _func.DivisionCylFwdBwdChk(NG_TRAY_PORT, CYL_BWD)
			&& _func.NGTrayLiftTrayChk(SENSOR_ON) // ���� üũ ������ ������ �� ��쿡 Ʈ���̰� ��� ��ġ ���ϴ� ��찡 ���� ���־ ���ܵ�. 2017/04/29 HSK
			&& (_func.TrayData_Exist(NG_TRAY_PORT,TRAY_INFO_POS_NG_IN_ALIGN) == FALSE)
			&& (_func.TrayData_Exist(NG_TRAY_PORT,TRAY_INFO_POS_NG_IN_LIFT_ARM) == TRUE))
		{
			// NG tray Lift Arm Pitch Up �̵� ������
			StateNGTrayInLiftPitch.Start();
		}

		// Lot End�϶��� Align Tray Cell ������ Ȯ�� ���� �ʰ� ���´�.
		if(theConfigBank.m_Option.m_bUseLotEnd || theConfigBank.m_UnloadRunInfo.GetJobChangeType() == JOBCHANGE_CASE1)//PPID�� �ٸ���� ���� �˻���ǰ lot end ó��
		{
			if (StateNGTrayTRGet.IsStoped()
				&& StateNGTrayTRPut.IsStoped()
				&& StateNGTrayInLiftPitch.IsStoped() 
				&& stateUDRobotNGPut.IsStoped()
				&& (_func.TrayData_Exist(NG_TRAY_PORT,TRAY_INFO_POS_NG_TR) == FALSE)
				&& (_func.CellDataMachineChk(CELL_POS_LD_CONV,CELL_POS_UD_ROBOT) == FALSE  || theConfigBank.m_UnloadRunInfo.GetJobChangeType() == JOBCHANGE_CASE1
						|| _func.TrayData_CellIsFull(NG_TRAY_PORT,TRAY_INFO_POS_NG_IN_ALIGN))
				&& (_func.TrayData_ExistCell(NG_TRAY_PORT, TRAY_INFO_POS_NG_IN_ALIGN) == TRUE)
				&& stateUDRobotConvPut.IsStoped())  //2018-05-14,SKCHO)
			{
				StateNGTrayTRGet.Start();
			}
		}
		else
		{
			//Align Tray Cell ������ Ȯ�� �� Tray TR Tray ���� Ȯ��.
			if (StateNGTrayTRGet.IsStoped()
				&& StateNGTrayTRPut.IsStoped()
				&& StateNGTrayInLiftPitch.IsStoped() 
				&& stateUDRobotNGPut.IsStoped()
				&& _func.TrayData_CellIsFull(NG_TRAY_PORT,TRAY_INFO_POS_NG_IN_ALIGN)
				&& (_func.TrayData_Exist(NG_TRAY_PORT,TRAY_INFO_POS_NG_TR) == FALSE)
				&& stateUDRobotConvPut.IsStoped())  //2018-05-14,SKCHO)
			{
				// Tray Tr Get ������
				StateNGTrayTRGet.Start();
			}
		}

		//Tray TR Tray ���� Ȯ��
		if (StateNGTrayTRPut.IsStoped()
			&& StateNGTrayTRGet.IsStoped()
			&& StateNGTrayOutLift.IsStoped()
			&& StateNGTrayAlign.IsStoped()
			&& _func.NGTrayTr_VacChk(VAC_ON)
			&& _func.TrayData_Exist(NG_TRAY_PORT,TRAY_INFO_POS_NG_TR)
			// NG OUT LIFT�� Ʈ���̰� ������ BUFF CONV�� Ʈ���� ���� �� ��� ����Ʈ�� Ʈ���̸� �״°� ����. 17/04/26 HSK
			&& _func.TrayData_TrayIsFull(NG_TRAY_PORT, TRAY_INFO_POS_NG_OUT_LIFT_ARM) == FALSE
			&& stateUDRobotConvPut.IsStoped())  //2018-05-14,SKCHO)
		{
			// Tray TR Put ������
			StateNGTrayTRPut.Start();
		}

		if (theConfigBank.m_Option.m_bUseTrayAlign)
		{
			if (theProcBank.m_bNGTrayAlignChk == FALSE
				&& _func.TrayData_Exist(NG_TRAY_PORT,TRAY_INFO_POS_NG_OUT_LIFT_ARM))
			{
				StateNGTrayAlign.Start();
			}
		}

		// LotEnd�� ��쿣 Out Lift Arm�� Ʈ���� �������� üũ ���� �ʴ´�.
		if(theConfigBank.m_Option.m_bUseLotEnd || theConfigBank.m_UnloadRunInfo.GetJobChangeType() == JOBCHANGE_CASE1)
		{
			if (StateNGTrayOutLift.IsStoped()
				&& StateNGTrayTRGet.IsStoped()
				&& StateNGTrayTRPut.IsStoped()
				&& (theProcBank.m_bNGTrayTRLotEnd == TRUE || _func.TrayData_ExistCell(NG_TRAY_PORT, TRAY_INFO_POS_NG_IN_ALIGN) == FALSE) // 2017.2.6 bgkim
				// Ʈ���� ����Ÿ�� ���� �Ͽ� ������ ���� ������ �ȿ��� ����Ÿ�� ���µ� ���� ���� �� ��� �˶�. 2017/04/29 HSK.
//				&& _func.NGConv_TrayChk(NG_BUFFER_TRAY_PORT, CONV_SEN_2, SENSOR_OFF)
//				&& _func.NGConv_TrayChk(NG_BUFFER_TRAY_PORT, CONV_SEN_3, SENSOR_OFF)
//				&& _func.NGConv_TrayChk(NG_BUFFER_TRAY_PORT, CONV_SEN_4, SENSOR_OFF)
				&& (_func.TrayData_Exist(NG_TRAY_PORT,TRAY_INFO_POS_NG_OUT_COMP_BUFFER) == FALSE)
				&& (_func.TrayData_Exist(NG_TRAY_PORT,TRAY_INFO_POS_NG_OUT_REQ_BUFFER) == FALSE)
				&& (_func.CellDataMachineChk(CELL_POS_LD_CONV,CELL_POS_UD_ROBOT) == FALSE  || theConfigBank.m_UnloadRunInfo.GetJobChangeType() == JOBCHANGE_CASE1)
				&& _func.TrayData_Exist(NG_TRAY_PORT, TRAY_INFO_POS_NG_OUT_LIFT_ARM))
			{
				StateNGTrayOutLift.Start();
			}
		}
		else
		{
			// NG Comp Buffer �� Req Buffer�� Tray�� ���� Out Lift Arm�� Ʈ���̰� ��á�� ��
			if (StateNGTrayOutLift.IsStoped() && StateNGTrayTRPut.IsStoped()
				// Ʈ���� ����Ÿ�� ���� �Ͽ� ������ ���� ������ �ȿ��� ����Ÿ�� ���µ� ���� ���� �� ��� �˶�. 2017/04/29 HSK.
//				&& _func.NGConv_TrayChk(NG_BUFFER_TRAY_PORT, CONV_SEN_2, SENSOR_OFF)
//				&& _func.NGConv_TrayChk(NG_BUFFER_TRAY_PORT, CONV_SEN_3, SENSOR_OFF)
//				&& _func.NGConv_TrayChk(NG_BUFFER_TRAY_PORT, CONV_SEN_4, SENSOR_OFF)
				&& (_func.TrayData_Exist(NG_TRAY_PORT,TRAY_INFO_POS_NG_OUT_COMP_BUFFER) == FALSE)
				&& (_func.TrayData_Exist(NG_TRAY_PORT,TRAY_INFO_POS_NG_OUT_REQ_BUFFER) == FALSE)
				&& _func.TrayData_TrayIsFull(NG_TRAY_PORT, TRAY_INFO_POS_NG_OUT_LIFT_ARM))  //2017-03-11,skcho, NG�� Ʈ���̰� �������� �����ϰ�
			{
				// Tay ���� ������
				StateNGTrayOutLift.Start();
			}
		}

		BOOL bTemp1=FALSE;
		if(theConfigBank.m_Option.m_bUseConveyorTestModel)
			bTemp1=TRUE;
		else
		{
			if(_func.GetInPutIOState(X_HYBRID_CONV_READY) == TRUE&&(_func.GetInPutIOState(X_HYBRID_CONV_INTERFACE) == TRUE))
				bTemp1=TRUE;
			else
				bTemp1=FALSE;
		}
		//2018wjp
		if (StateGoodTrayInLift.IsStoped()//2018wjp interlock
			&&StateGoodTrayOutLift.IsStoped()//2018wjp interlock
			&&StateGoodTrayInLiftPitch.IsStoped()//2018wjp interlock
			&&stateUDRobotGoodPut.IsStoped()
			&& stateUDRobotNGPut.IsStoped()
			&& stateUDRobotConvPut.IsStoped()
			&& StateUDRobotGet.IsStoped()
			&&StateGoodTrayGetPut.IsStoped()
			//&& StateGoodTrayTRPut.IsStoped()
			&& StateGoodTrayTRGet.IsStoped()
			&& StateGoodTrayGetPut.IsStoped()
			&& StateUDRobotPick2.IsStoped()
			//&& (_func.CellTagExist(CELL_POS_UD_TR_CH1) == FALSE && _func.CellTagExist(CELL_POS_UD_TR_CH2) == FALSE)
			//110
			//&& (_func.GetInPutIOState(X_HYBRID_CONV_READY) == TRUE  )
			//&& (_func.GetInPutIOState(X_HYBRID_CONV_INTERFACE) == TRUE) 
			&&bTemp1
			&&theProcBank.m_bInPick2Model
			&&theProcBank.m_bPickOUT2INTrayRequest)
		{
			// UD TR Get ���� ������
			if(theProcBank.m_bPickOUT2INTrayRequest)
				theProcBank.m_bPickOUT2INTrayRequest=FALSE;
			StateGoodTrayGetPut.Start();
		}

		BOOL bTemp2=FALSE;
		if(theConfigBank.m_Option.m_bUseConveyorTestModel)
			bTemp2=TRUE;
		else
		{
			if(_func.GetInPutIOState(X_HYBRID_CONV_READY) == TRUE&&(_func.GetInPutIOState(X_HYBRID_CONV_INTERFACE) == TRUE))
				bTemp2=TRUE;
			else
				bTemp2=FALSE;
		}

		/////////////2018wjp
		if (stateUDRobotGoodPut.IsStoped()
			&& stateUDRobotNGPut.IsStoped()
			&& stateUDRobotConvPut.IsStoped()
			&& StateUDRobotGet.IsStoped()
			&& StateGoodTrayGetPut.IsStoped()
			&& StateUDRobotPick2.IsStoped()
			&& StateGoodTrayInLiftPitch.IsStoped()
			//&& (_func.CellTagExist(CELL_POS_UD_TR_CH1) == FALSE && _func.CellTagExist(CELL_POS_UD_TR_CH2) == FALSE)
			//110
			//&& (_func.GetInPutIOState(X_HYBRID_CONV_READY) == TRUE)
			//&& (_func.GetInPutIOState(X_HYBRID_CONV_INTERFACE) == TRUE) 
			&&bTemp2
			&& !_func.CellTagExist(CELL_POS_UD_ROBOT)
			&&theProcBank.m_bInPick2Model)
		{
			// UD TR Get ���� ������	
			//2018wjp
			if(theProcBank.i_ModelChange1To2==1)
			{
				theProcBank.i_ModelChange1To2=0;
				if(theProcBank.GOODTrayBitX==theRecipeBank.m_Tray.m_nCountX
					&&theProcBank.GOODTrayBitY==theRecipeBank.m_Tray.m_nCountY)
				{
					theProcBank.GOODTrayBitX=theRecipeBank.m_Tray.m_nCountX-1;
					theProcBank.GOODTrayBitY=theRecipeBank.m_Tray.m_nCountY;
				}
				else
				{
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
				}
			}
			StateUDRobotPick2.Start();
		}


		/////////////////////

		//-------------------------------------------------------------------------------------------------------------------------------
		//--- Step, Running!
		//-------------------------------------------------------------------------------------------------------------------------------
		StateLDRobotGet.Run();
		stateUpperShuttleGet.Run();
		stateLowerShuttleGet.Run();
		stateLDRobotUpperPut.Run();
		stateLDRobotLowerPut.Run();
		stateUpperShuttlePut.Run();
		stateLowerShuttlePut.Run();
		statePATRUpperGet.Run();
		statePATRLowerGet.Run();
		StatePATRPut.Run();
		StatePAStageGet.Run();
		StatePAStage.Run();
		StatePAStagePut.Run();
		StateLDTRGet.Run();
		StateLowerMCR.Run();
		StateLDTRPut.Run();
		StateUpperMCR.Run();

		StateAMTTable.Run();
		StateAZone.Run();
		StateBZone.Run();
		StateCZone.Run();
		StateDZone.Run();

		StateUDTRGet.Run();
		StateJobChange.Run();
		StateUDTRPut.Run();

		// 2016.12.21
		StateUDRobotGet.Run();
		stateUDRobotGoodPut.Run();
		stateUDRobotNGPut.Run();
		
		//2018-05-14,SKCHO
		stateUDRobotConvPut.Run();

		StateGoodTrayInLift.Run();
		StateGoodTrayInLiftPitch.Run();
		StateGoodTrayTRGet.Run();
		StateGoodTrayTRPut.Run();
		StateGoodTrayDCR.Run();
		StateGoodTrayAlign.Run();
		StateGoodTrayOutLift.Run();

		StateNGTrayIn.Run();
		StateNGTrayInLiftUp.Run();
		StateNGTrayInLiftPitch.Run();
		StateNGTrayTRGet.Run();
		StateNGTrayTRPut.Run();
		StateNGTrayOutLift.Run();

		//2018wjp
		StateUDRobotPick2.Run();
        StateGoodTrayGetPut.Run();
	}

	TRACE("Thread end\n");
	return 0;
};

// ���� �����忡�� �˶� �ߴ� �� ��� ���� ���� �Ѵ�. 2017/05/02 HSK.
UINT CellRcvThr( LPVOID lpParam )
{
	theDeviceMotion.Open_Mp2100(MP2100_THREAD_CELLRCV_INTERFACE);

	int nThreadIndex = (int)lpParam;
	// Thread������ _func�� ���� ���� �ȵ�!!! �Ʒ� _func�� ����
	CUnitCtrlFunc					_func(MP2100_THREAD_CELLRCV_INTERFACE);	// �Ʒ� ��ü�� �̸��� �ʹ� �� �ϳ� �� ����
	
	CONST_MACHINE_STATE::ID _LastState = CONST_MACHINE_STATE::MACHINE_PAUSE;

	while(theThreadInitializer.m_bThrExist[nThreadIndex] == FALSE)
	{
		Sleep(50);

		//���α׷� ���� �ʱ�ȭ üũ ���ߴ��� �ʱ�ȭ �� ���� ���� üũ ���ϴ� �����忡�� �ʹ� ���� ��ȣ üũ �ؼ� ���α׷� �״´� 2017/06/09 HSK.
		if(theProcBank.m_bProgramInitFinish == FALSE)
			continue;

		if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE) // ����� ���� ��.
		{		
			if(StateCellReceive.GetError() > 0)
			{
				StateCellReceive.Reset();
			}	
			if(StateManualReceive.GetError() > 0)
			{
				StateCellReceive.Reset();
			}	
			if(StateConvMove.GetError() > 0)
			{
				StateCellReceive.Reset();
			}	
		}

		if(StateCellReceive.GetError() > 0 || StateManualReceive.GetError() > 0 || StateConvMove.GetError() > 0)
		{
			continue;
		}

		// ���� ���߸� �ش� �κп��� Continue �Ǽ� ���� ������ �ȵȴ� 2017/06/09  HSK.
//		if( (stateRestart.IsHolded() == FALSE) || stateRestart.IsStoped() )
//		{
//			continue;
//		}

//		if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)
//		{	
			if(theConfigBank.m_Option.m_bLoadPitchIn == FALSE)
			{
				// Lot End �ϰ�쿣 Cell�� ���� �ʴ´�.
				if(theConfigBank.m_Option.m_bUseLotEnd == FALSE)
				{
					// ���� �غ� ���ְ� Conv�� Cell�� ���� ��.
					if (StateManualReceive.IsStoped() && StateCellReceive.IsStoped()
						&& (_func.RcvEnableChk() == TRUE)
						&& (_func.LDConvCellChk(CONV_SEN_3) == FALSE) && (_func.LDConvCellChk(CONV_SEN_1) == FALSE)
						&& (_func.CellTagExist(CELL_POS_LD_CONV) == FALSE))
					{
						// �մ� ����κ��� Cell �޴� ������
						StateCellReceive.Start();
					}
					if(StateManualReceive.IsStoped() && StateCellReceive.IsStoped()
					&& (_func.LDConvCellChk(CONV_SEN_3) == FALSE) && (_func.LDConvCellChk(CONV_SEN_1) == TRUE)
					&& (_func.CellTagExist(CELL_POS_LD_CONV) == FALSE))
					{
						// �޴���� �� �޴� ������
						StateManualReceive.Start();
					}
				}
			}
			else
			{
				// Lot End �ϰ�쿣 Cell�� ���� �ʴ´�.
				if(theConfigBank.m_Option.m_bUseLotEnd == FALSE)// && theProcBank.m_bUnitInterlock_LoadingStop == FALSE)
				{
					// ���� �غ� ���ְ� Conv�� Cell�� ���� ��.
					if (StateCellReceive.IsStoped() && StateConvMove.IsStoped()
						&& (_func.RcvEnableChk() == TRUE)
						&& _func.LDConvCellChk(CONV_SEN_1) == FALSE && _func.LDConvCellChk(CONV_SEN_3) == FALSE
						&& _func.CellTagExist(CELL_POS_LD_CONV) == FALSE)
					{
						// �մ� ����κ��� Cell �޴� ������
						StateCellReceive.Start();
					}
					if(StateConvMove.IsStoped() && StateCellReceive.IsStoped()
					&& (_func.LDConvCellChk(CONV_SEN_3) == FALSE) && (_func.LDConvCellChk(CONV_SEN_1) == TRUE)
					&& (_func.CellTagExist(CELL_POS_LD_CONV) == FALSE))
					{
						StateConvMove.Start();
					}
				}
			}
//		}
		StateCellReceive.Run();
		StateManualReceive.Run();
		StateConvMove.Run();
	}
	return 0;
}
// ���� �����忡�� �˶� �ߴ� �� ��� ���� ���� �Ѵ�. 2017/05/02 HSK.
UINT AGVInterfaceThr( LPVOID lpParam )		
{
	theDeviceMotion.Open_Mp2100(MP2100_THREAD_AGV_INTERFACE);

	int nThreadIndex = (int)lpParam;

	// Thread������ _func�� ���� ���� �ȵ�!!! �Ʒ� _func�� ����
	CUnitCtrlFunc					_func(MP2100_THREAD_AGV_INTERFACE);	// �Ʒ� ��ü�� �̸��� �ʹ� �� �ϳ� �� ����

	CONST_MACHINE_STATE::ID _LastState = CONST_MACHINE_STATE::MACHINE_PAUSE;
	
	CStopWatch AgvTimer;
	CStopWatch OutAgvTimer;
	int nDelay= 0;

	while(theThreadInitializer.m_bThrExist[nThreadIndex] == FALSE)
	{
		Sleep(5);

		//���α׷� ���� �ʱ�ȭ üũ ���ߴ��� �ʱ�ȭ �� ���� ���� üũ ���ϴ� �����忡�� �ʹ� ���� ��ȣ üũ �ؼ� ���α׷� �״´� 2017/06/09 HSK.
		if(theProcBank.m_bProgramInitFinish == FALSE)
			continue;
	
		if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)
		{				
			if(stateGoodTrayReceive.GetError() > 0)
			{
				stateGoodTrayReceive.Reset();
			}
			if(StateGoodTraySend.GetError() > 0)
			{
				StateGoodTraySend.Reset();
			}
		}

		if(stateGoodTrayReceive.GetError() > 0 || StateGoodTraySend.GetError() > 0)
		{
			continue;
		}

		// ���� ���߸� �ش� �κп��� Continue �Ǽ� ���� ������ �ȵȴ� 2017/06/09  HSK.
//		if( (stateRestart.IsHolded() == FALSE) || stateRestart.IsStoped() )
//		{
//			continue;
//		}
	
		//2017-08-30,SKCHO, ���� �ð� ������ �˶� �︮�� ��
		if(theProcBank.m_nAGVDelayStep == 1)
		{
			AgvTimer.Start();
			nDelay = (int)(theConfigBank.m_Option.m_nAGVDelayTime * 60);
			theProcBank.m_nAGVDelayStep = 2;
			theLog[LOG_AGV].AddBuf(_T("IN PORT AGV ��û"));			
		}
		else if(AgvTimer.Stop(FALSE) > nDelay && theProcBank.m_nAGVDelayStep == 2)
		{
			
			theUnitFunc.LampChange((STATUS_LAMP_DESCRIPTION)LAMP_AGV,4);
			theProcBank.m_nAGVDelayStep = 3;
		}
		else if(theProcBank.m_nAGVDelayStep == 4)  //4�̸� ���� ���·� ������
		{
			//2017-09-12,SKCHO, LAMPCHANGE �Լ� ����
			int nLampState = theUnitFunc.GetMachinetoLampState();
			
			theUnitFunc.LampChange((STATUS_LAMP_DESCRIPTION)nLampState,0);

			//2017-11-17,SKCHO , TRAY ������
			//theProcBank.m_nAGVDelayStep = 0;
			theProcBank.m_nAGVDelayStep = 6;
		}

		//OUT PORT AGV �ð� üũ
		if(theProcBank.m_nOutAGVDelayStep == 1)
		{
			OutAgvTimer.Start();
			nDelay = (int)(theConfigBank.m_Option.m_nAGVDelayTime * 60);
			theProcBank.m_nOutAGVDelayStep = 2;
			theLog[LOG_AGV].AddBuf(_T("OUT PORT AGV ��û"));			
		}
		else if(OutAgvTimer.Stop(FALSE) > nDelay && theProcBank.m_nOutAGVDelayStep == 2)
		{
			
			theUnitFunc.LampChange((STATUS_LAMP_DESCRIPTION)LAMP_AGV,4);
			theProcBank.m_nOutAGVDelayStep = 3;
		}
		else if(theProcBank.m_nOutAGVDelayStep == 4)  //4�̸� ���� ���·� ������
		{

			//2017-09-12,SKCHO, LAMPCHANGE �Լ� ����
			int nLampState = theUnitFunc.GetMachinetoLampState();
		
			theUnitFunc.LampChange((STATUS_LAMP_DESCRIPTION)nLampState,0);
			
			//2017-11-17,SKCHO , TRAY �����ϸ�
			//theProcBank.m_nOutAGVDelayStep = 0;
			theProcBank.m_nOutAGVDelayStep = 6;
		}

	
		if (stateGoodTrayReceive.IsStoped() && StateGoodTraySend.IsStoped()
			//���� üũ ���� �ʴ´�. 2017/04/28 HSK.
			//&& _func.BufConvSenChk(GOOD_IN_TRAY_PORT,SENSOR_OFF)
			&& _func.AGVConnectionChk(GOOD_IN_TRAY_PORT)
			&& _func.TrayData_Exist(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_BUFFER) == FALSE)
		{
			// Tray �޴� ������
			stateGoodTrayReceive.Start();
		}
		if (StateGoodTraySend.IsStoped() && stateGoodTrayReceive.IsStoped()
			&& _func.AGVConnectionChk(GOOD_OUT_TRAY_PORT)
			&& _func.TrayData_Exist(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_OUT_BUFFER))
		{
			// Tray ������ ������
			StateGoodTraySend.Start();
		}

		stateGoodTrayReceive.Run();
		StateGoodTraySend.Run();
	}
	return 0;
}
// 2017-09-17,SKCHO, MANUAL ���Խ� ���� �����Ǹ� ������ �����ϰ�
UINT GoodTrayManualThr( LPVOID lpParam )		
{
	theDeviceMotion.Open_Mp2100(MP2100_THREAD_TRAY_MANUAL_RECEIVE);

	int nThreadIndex = (int)lpParam;

	// Thread������ _func�� ���� ���� �ȵ�!!! �Ʒ� _func�� ����
	CUnitCtrlFunc					_func(MP2100_THREAD_TRAY_MANUAL_RECEIVE);	// �Ʒ� ��ü�� �̸��� �ʹ� �� �ϳ� �� ����

	CONST_MACHINE_STATE::ID _LastState = CONST_MACHINE_STATE::MACHINE_PAUSE;
	
	CStopWatch AgvTimer;
	CStopWatch OutAgvTimer;
	int nDelay= 0;

	while(theThreadInitializer.m_bThrExist[nThreadIndex] == FALSE)
	{
		Sleep(5);

		//���α׷� ���� �ʱ�ȭ üũ ���ߴ��� �ʱ�ȭ �� ���� ���� üũ ���ϴ� �����忡�� �ʹ� ���� ��ȣ üũ �ؼ� ���α׷� �״´� 2017/06/09 HSK.
		if(theProcBank.m_bProgramInitFinish == FALSE)
			continue;
	
		if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)
		{				
			if(stateGoodTrayReceive.GetError() > 0)
			{
				stateGoodTrayManualReceive.Reset();
			}			
		}
	
		if(stateGoodTrayReceive.GetError() > 0 )
		{
			continue;
		}	
	
		if (stateGoodTrayReceive.IsStoped() && StateGoodTraySend.IsStoped()			
			&& _func.BufConvSenChk(GOOD_IN_TRAY_PORT,SENSOR_OFF)
			&& _func.AGVConnectionChk(GOOD_IN_TRAY_PORT) == FALSE
			&& _func.TrayData_Exist(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_BUFFER) == FALSE
			/*&& _func.BuffConv_TrayChk(CONV_SEN_1,SENSOR_ON)*/
			&& _func.TrayReqSwChk(GOOD_IN_TRAY_PORT))
		{
			// Tray �޴� ������
			stateGoodTrayManualReceive.Start();
		}

		stateGoodTrayManualReceive.Run();
		
	}
	return 0;
}
// AGV �� ��� ������. Main Thr���� �˶��� �߻� �ص� �޴� ������ ������ ���� �� �ֵ��� Thr ���� ����.
/*
UINT AGVInterfaceThr( LPVOID lpParam )		
{
	theDeviceMotion.Open_Mp2100(MP2100_THREAD_AGV_INTERFACE);

	int nThreadIndex = (int)lpParam;

	// Thread������ _func�� ���� ���� �ȵ�!!! �Ʒ� _func�� ����
	CUnitCtrlFunc					_func(MP2100_THREAD_AGV_INTERFACE);	// �Ʒ� ��ü�� �̸��� �ʹ� �� �ϳ� �� ����

	CONST_MACHINE_STATE::ID _LastState = CONST_MACHINE_STATE::MACHINE_PAUSE;

	while(theThreadInitializer.m_bThrExist[nThreadIndex] == FALSE)
	{
		Sleep(5);

		//-------------------------------------------------------------------------------------------------------------------------------
		//--- EQ State Check
		//-------------------------------------------------------------------------------------------------------------------------------
		if(_LastState != theProcBank.GetMachineState())
		{
			if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)
			{				
				if(stateGoodTrayReceive.GetError() > 0)
					stateGoodTrayReceive.Reset();
				if(stateNGTrayReceive.GetError() > 0)
					stateNGTrayReceive.Reset();
				if(StateGoodTraySend.GetError() > 0)
					StateGoodTraySend.Reset();
				if(StateNGTraySend.GetError() > 0)
					StateNGTraySend.Reset();
			}
			else if((_LastState == CONST_MACHINE_STATE::MACHINE_RUN || _LastState == CONST_MACHINE_STATE::MACHINE_IDLE)
				&& (theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_INIT || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_PAUSE
				|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_FAULT || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_INTERLOCK))
			{
					stateGoodTrayReceive.Reset();
					stateNGTrayReceive.Reset();
					StateGoodTraySend.Reset();
					StateNGTraySend.Reset();
			}
			_LastState = theProcBank.GetMachineState();
		}

		if(stateGoodTrayReceive.GetError() > 0 || stateNGTrayReceive.GetError() > 0 || StateGoodTraySend.GetError() > 0 || StateNGTraySend.GetError() > 0)
		{
			continue;
		}

		if( (stateRestart.IsHolded() == FALSE) || stateRestart.IsStoped() )
		{
			continue;
		}

		if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)
		{	
			//Good In Buf Conv�� Tray ���� ���� ��� ����
			if (stateGoodTrayReceive.IsStoped() && StateGoodTraySend.IsStoped()
				//���� üũ ���� �ʴ´�. 2017/04/28 HSK.
				//&& _func.BufConvSenChk(GOOD_IN_TRAY_PORT,SENSOR_OFF)
				&& _func.AGVConnectionChk(GOOD_IN_TRAY_PORT)
				&& _func.TrayData_Exist(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_BUFFER) == FALSE)
			{
				// Tray �޴� ������
				stateGoodTrayReceive.Start();
			}
			//NG In Buf Conv�� Tray ���� Receive Flag ��� ���� �׿��ִ� Tray�κ��� ���� ���� ��
			if (stateNGTrayReceive.IsStoped() && StateNGTraySend.IsStoped()
				&& _func.BufConvSenChk(NG_TRAY_PORT,SENSOR_OFF) && _func.AGVConnectionChk(NG_TRAY_PORT)
				&& _func.TrayData_Exist(NG_TRAY_PORT,TRAY_INFO_POS_NG_OUT_REQ_BUFFER) == FALSE && _func.TrayData_Exist(NG_TRAY_PORT,TRAY_INFO_POS_NG_OUT_COMP_BUFFER) == FALSE
				&& theProcBank.m_TrayBank.GetTrayInfo(NG_TRAY_PORT, TRAY_INFO_POS_NG_OUT_LIFT_ARM)->GetTrayCount() < 10)
			{
				// Tray �޴� ������
				//stateNGTrayReceive.Start();//ng��Ʈ�� agv���� ������� ����-�����
			}

			//Good Out Buff Conv�� Tray �ְ� Send Flag ����� ��
			if (StateGoodTraySend.IsStoped() && stateGoodTrayReceive.IsStoped()
				//���� üũ ���� �ʴ´�. 2017/04/28 HSK.
				//&& _func.BufConvSenChk(GOOD_OUT_TRAY_PORT,SENSOR_ON)
				&& _func.AGVConnectionChk(GOOD_OUT_TRAY_PORT)
				//20170212 byskcho
				//Ʈ���� ������ GOOD_IN_TRAY_PORT �ε����� ���
				&& _func.TrayData_Exist(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_OUT_BUFFER))
				//&& _func.TrayData_Exist(GOOD_OUT_TRAY_PORT,TRAY_INFO_POS_GOOD_OUT_BUFFER))
			{
				// Tray ������ ������
				StateGoodTraySend.Start();
			}

			//Buff Conv�� Tray �ְ� Send Flag ����� ��
			if (StateNGTraySend.IsStoped() && stateNGTrayReceive.IsStoped() 
				&& _func.BufConvSenChk(NG_TRAY_PORT,SENSOR_ON) && _func.AGVConnectionChk(NG_TRAY_PORT)
				&& _func.TrayData_Exist(NG_TRAY_PORT,TRAY_INFO_POS_NG_OUT_COMP_BUFFER))
			{
				// Tray ������ ������
				//StateNGTraySend.Start();  //ng��Ʈ�� agv���� ������� ����-�����
			}
		}

		stateGoodTrayReceive.Run();
		stateNGTrayReceive.Run();
		StateGoodTraySend.Run();
		StateNGTraySend.Run();

	}
	return 0;
}
*/

// ��� üũ �ؾ� �ϴ� I/O Thr
UINT IOCheckThr( LPVOID lpParam )		
{
	theDeviceMotion.Open_Mp2100(MP2100_THREAD_IO_CHECK);

	int nTimeCount = 0;
	int nAGVTimeCount = 0;
	int nThreadIndex = (int)lpParam;
	//2018-06-29,NAM
	int nTimeHybrid=0;

	CStopWatch m_timerTrayInBufin;
	CStopWatch m_timerTrayInBufOut;
	CStopWatch m_timerTrayOutBufOut;
	CStopWatch m_timerHybrid;
//2018wjp
	CStopWatch m_checkstage;
	CStopWatch m_checkstageSingle;
	BOOL bIncheck=FALSE;
	BOOL bstagecheck=FALSE;

	CONST_MACHINE_STATE::ID _LastState = CONST_MACHINE_STATE::MACHINE_PAUSE;

	// Thread������ _func�� ���� ���� �ȵ�!!! �Ʒ� _func�� ����
	CUnitCtrlFunc					_func(MP2100_THREAD_IO_CHECK);	// �Ʒ� ��ü�� �̸��� �ʹ� �� �ϳ� �� ����

	while(theThreadInitializer.m_bThrExist[nThreadIndex] == FALSE)
	{
		Sleep(1);
		nTimeCount++;

		//2018-05-16,MGYUN, AGV ȿ�� ���� ������ �߰�
		nAGVTimeCount++;

		if(nTimeCount < 100) continue;
		else nTimeCount = 0;

		if(GetMainHandler() == NULL)
			continue;

		//���α׷� ���� �ʱ�ȭ üũ ���ߴ��� �ʱ�ȭ �� ���� ���� üũ ���ϴ� �����忡�� �ʹ� ���� ��ȣ üũ �ؼ� ���α׷� �״´� 2017/06/09 HSK.
		if(theProcBank.m_bProgramInitFinish == FALSE)
			continue;

		if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)
		{	
			// ����ġ�� �ƴ� ���� �߿�.
			if(_func.TrayData_Exist(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_BUFFER) == FALSE
			&& theProcBank.m_bGoodTrayReq == FALSE)
			{
				//2018-05-16,MGYUN, AGV ȿ�� ���� ������ �߰�
				//if(nAGVTimeCount > 2000)	
				//if(nAGVTimeCount > 1000)	//2000���� �Ͽ����� 71ȣ�� �α� Ȯ�� ��� 2�ʰ� �ƴ� 5~6��
				if(nAGVTimeCount > 700)	//1000���� �Ϸ� ����͸� ��� 3-4��
				{
					_func.CIM_TRAYProcess(_T("350"),_T("1"));
					theProcBank.m_bGoodTrayReq = TRUE;
					// �ٽ� 0���� �ʱ�ȭ
					nAGVTimeCount = 0;
				}
			}
			else
			{
				nAGVTimeCount = 0;
			}

			if(_func.TrayData_Exist(NG_TRAY_PORT,TRAY_INFO_POS_NG_IN_BUFFER) == FALSE
				&& theProcBank.m_bNGTrayReq == FALSE)
			{
				//_func.CIM_TRAYProcess(_T("350"),_T("3")); //NG Ʈ���� �������. 20170510. JSJUNG
				theProcBank.m_bNGTrayReq = TRUE;
			}

/*			if(_func.TrayData_Exist(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_BUFFER) == FALSE
				&& _func.TrayReqSwChk(GOOD_IN_TRAY_PORT) )
			{
			//	if(_func.BufConvSenChk(GOOD_IN_TRAY_PORT,SENSOR_ON))  //2017-04-14,skcho, cim-qual -request������ ����
			//	{
					theProcBank.m_bGoodTrayInSWReq = TRUE;
					_func.CIM_TRAYProcess(_T("350"),_T("1"));
			//	}
			}
*/
			if(_func.TrayData_Exist(NG_TRAY_PORT,TRAY_INFO_POS_NG_IN_BUFFER) == FALSE
				&& _func.TrayReqSwChk(NG_TRAY_PORT))
			{
				if(_func.BufConvSenChk(NG_TRAY_PORT,SENSOR_ON))
				{
					theProcBank.m_bNGTrayInSWReq = TRUE;
					//_func.CIM_TRAYProcess(_T("350"),_T("3")); //NG Ʈ���� �������. 20170510. JSJUNG
				}
			}

			//2018-05-22,MGYUN,���̺긮��,�߰�
			//C/V PUT ��Ȳ�̳�, C/V�� NO RUN �����̿��� �ٷ� GOOD TRAY PUT �Ҽ� �ְ�
			//2018-06-29,NAM
			/*
			//if(_func.GetInPutIOState(X_HYBRID_CONV_READY) == FALSE &&
				if(_func.GetInPutIOState(X_HYBRID_CONV_INTERFACE) == TRUE &&
				_func.CellTagExist(CELL_POS_UD_ROBOT)
			      ) 
			{
				//m_timerHybrid.Start();
				//theProcBank.HybridDelayFlag = FALSE; // NG T.T
				theProcBank.HybridDelayFlag = TRUE;
			}
			// �����̾� READY OFF�̰� INTERFACE �� OFF �϶� -> GOOD TRAY PUT 
			//else if (_func.GetInPutIOState(X_HYBRID_CONV_READY) == FALSE &&
				else if (_func.GetInPutIOState(X_HYBRID_CONV_READY) == FALSE ||
				_func.GetInPutIOState(X_HYBRID_CONV_INTERFACE) == FALSE)
			{
				theProcBank.HybridDelayFlag = TRUE;
			}
			else // C/V PUT 
				theProcBank.HybridDelayFlag = FALSE;

				*/

			if (_func.CellTagExist(CELL_POS_UD_ROBOT)
			    && _func.CellData_GetLastClass(CELL_POS_UD_ROBOT) == GOOD_CELL)	
				{
					//2018-06-29,NAm, After 20 second -> ULD Robot Put in Tray. 
					/*if (m_timerHybrid.Stop(FALSE) > 25.)
					{
						theProcBank.HybridDelayFlag=TRUE;
					}*/
					if (nTimeHybrid >= theConfigBank.m_Option.m_nConveyorDelay*4)
					{
						theProcBank.HybridDelayFlag=TRUE;
						nTimeHybrid=0;
					}

					if ( _func.GetInPutIOState(X_HYBRID_CONV_READY)==TRUE &&
						_func.GetInPutIOState(X_HYBRID_CONV_INTERFACE) == TRUE)
					{
					   theProcBank.HybridDelayFlag = FALSE;
					   theProcBank.m_newstateofHybrid=TRUE;
					}
					else 
					   theProcBank.m_newstateofHybrid=FALSE;

					if (theProcBank.m_newstateofHybrid ==TRUE) 
					{
					theProcBank.m_oldstateofHybrid=TRUE;
					}
					else 
					{	
					  if (theProcBank.m_oldstateofHybrid != theProcBank.m_newstateofHybrid)
						{
						theProcBank.m_oldstateofHybrid=theProcBank.m_newstateofHybrid;
						//m_timerHybrid.Start();
						}
					  if ((theProcBank.m_oldstateofHybrid==FALSE) 
						  && (theProcBank.m_newstateofHybrid==FALSE) 
						  && (theProcBank.HybridDelayFlag==FALSE))
					  {
						  nTimeHybrid++;
					  }
					}

					/*if(m_timerHybrid.Stop(FALSE) > 5. && theProcBank.HybridDelayFlag == FALSE)
					{
						theProcBank.HybridDelayFlag = TRUE;
					}*/
					
			     }

			//2018wjp
			if(_func.CellTagExist(CELL_POS_UD_TR_CH1) == TRUE || _func.CellTagExist(CELL_POS_UD_TR_CH2)== TRUE)
			{
				if(!bstagecheck)
				{
					m_checkstageSingle.Start();
					bstagecheck=TRUE;
				}
				if(m_checkstageSingle.Stop(FALSE) > 1)
				{
					if(_func.CellTagExist(CELL_POS_UD_TR_CH1) == TRUE || _func.CellTagExist(CELL_POS_UD_TR_CH2)== TRUE)
						theProcBank.m_bStageSeneorSingle=TRUE;
				}
			}
			else
			{
				bstagecheck=FALSE;
				m_checkstageSingle.Stop(FALSE);
				theProcBank.m_bStageSeneorSingle=FALSE;
			}

			// 2017.2.11 bgkim
			// NG Lift Arm ������ �������� Ʈ���̷κ��� �������� �ʾ� ������ Ʈ���̸� ����� �ص�
			// ������ �������� �ʾ� Ʈ���� ������ ������ Ʈ���� ���� ����
//			if (_func.NGTrayLiftTrayChk(SENSOR_OFF)
//					&& theConfigBank.m_Option.m_bUseDryRun == FALSE
//					&& _func.TrayData_Exist(NG_TRAY_PORT,TRAY_INFO_POS_NG_IN_LIFT_ARM))
//			{
//				_func.TrayData_RemoveData(NG_TRAY_PORT,TRAY_INFO_POS_NG_IN_LIFT_ARM);
//			}

//			if (_func.LDTrayLiftTrayChk(SENSOR_OFF)
//					&& theConfigBank.m_Option.m_bUseDryRun == FALSE
//					&& _func.TrayData_Exist(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_LIFT))
//			{
//				_func.TrayData_RemoveData(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_LIFT);
//				_func.InTrayLiftZ_DownPos();
//				theLog[LOG_SEQUENCE].AddBuf(_T("LDTrayLiftSensor Check ����"));
//			}

			// IDLE ����Ǵ� ������ �δ� �κ� üũ �Ѵ� .2017/06/20 HSK.
			if (theProcBank.m_CellLoadChkTact.Stop(FALSE) > theConfigBank.m_Option.m_dRunDownWaitTime && theProcBank.m_CellLoadChk == FALSE
				&& _func.CellDataMachineChk(CELL_POS_LDROBOT,CELL_POS_UD_ROBOT) == FALSE)
			{
				theProcBank.ChangeMachineState(CONST_MACHINE_STATE::MACHINE_IDLE);
				::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_IDLECHAGNE_UPDATE, 0,0); 
				theLog[LOG_LOAD_DELAY_TACT].AddBuf(_T("���� IDLE ���� ����"));
				theLog[LOG_SYSTEM].AddBuf(_T("�ڵ� ���� IDLE ����"));
				theProcBank.m_CellLoadChk = TRUE;
				theProcBank.m_CellLoadRunChk = TRUE;
			}

			//2017-11-01,SKCHO, ��ü ��ŵ �ϰ�� ���� ULDTR ���� ULD ROBOT���� cell ���� �Ϸ� �� ���� ����
			if(theProcBank.m_bAllUnitInterlock_Stop == TRUE &&  _func.CellDataMachineChk(CELL_POS_UD_TR_CH1,CELL_POS_UD_ROBOT) == FALSE)
			{
				theProcBank.ChangeMachineState(CONST_MACHINE_STATE::MACHINE_INTERLOCK);
				theLog[LOG_SYSTEM].AddBuf(_T("Unit Interlock�� ���� ���� ����"));	
				theProcBank.m_bUnitInterlock_LoadingStop = TRUE;
				
			}

		}

		//���� �־��µ� �ڵ� RUN���� �ٲ��� �� ��찡 �ֳ� �𸣰ڴ�... �ּ� �Ѵ� 2017/06/17 HSK.
/*
		if ((theProcBank.m_CellLoadChk == FALSE) && (theProcBank.m_CellLoadRunChk == TRUE))
		{
			theProcBank.ChangeMachineState(CONST_MACHINE_STATE::MACHINE_RUN);
			theLog[LOG_SYSTEM].AddBuf(_T("�ڵ� ���� RUN ����"));
			theProcBank.m_CellLoadRunChk = FALSE;
		}
*/

		// ���߿� Ƽġ Ű ���信�� Ƽġ���� ����� �˶� �� ���� �߰�. 20170713 skcho
		if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)
		{
			if ( _func.GetInPutIOCheck(X_TEACH_KEY_SW) == OFF )
			{
				//2017-11-16,SKCHO, AUTO KEY ������ �˶���� ���� ������, MTBI ��û
				//_func.SetError(ALARM_TEACHKEY_SWITCH);
				theProcBank.ChangeMachineState(CONST_MACHINE_STATE::MACHINE_PAUSE);
			}
		}

		// �˶� �߻� �Ͽ� Ʈ���� Z���� ���� ����ϴ°� ������ ��.
		if(_func.TrayLiftOVerChk(NG_TRAY_PORT))
		{
			_func.InNGTrayLiftZ_PitchUpStop();
		}
		if(_func.TrayLiftOVerChk(GOOD_IN_TRAY_PORT))
		{
			_func.InTrayLiftZ_PitchUpStop();
		}
		
		//20170801,JSJUNG, preload abnormal case ���� 
		//Ʈ���� ������ �������� 15���� ���ø�Ʈ �� ��Ʈ ���� / �������� �� ��������
		//Ʈ���̸� ������ ���� ��쿡�� �ٷ� ���� ���� �� ��������
		//Ʈ���� ����Ʈ�� 262 ���� �������� 350 ���� ����
		// agv�� ���۽ô� �׽�Ʈ �ʿ���
		if( _func.GetInPutIOCheck(X_GOOD_LD_BUFFER_CONV_TRAY_SEN_1) ||
			_func.GetInPutIOCheck(X_GOOD_LD_BUFFER_CONV_TRAY_SEN_4) ||
			_func.GetInPutIOCheck(X_GOOD_LD_BUFFER_CONV_TRAY_SEN_5) )
		{
			if(theProcBank.m_bTrayInStart == FALSE && !_func.TrayData_Exist(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_BUFFER))
			{
				m_timerTrayInBufin.Start();
				theProcBank.m_bTrayInStart = TRUE;
			}

			if(m_timerTrayInBufin.Stop(FALSE) > 15. && theProcBank.GoodTrayInLiftingFlag == FALSE)
			{
				if(!_func.TrayData_Exist(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_BUFFER))
				{
					_func.SetOutPutIO(Y_GOOD_LD_TRAY_MUTING_LAMP_ON,OFF);
					_func.SetOutPutIO(Y_GOOD_LD_TRAY_REQ_SW,OFF);
					_func.SetOutPutIO(Y_GOOD_LD_TRAY_MUTE_ON,OFF);
					_func.SetOutPutIO(Y_GOOD_LD_TRAY_COMP_SW,ON);
					theProcBank.m_bGoodTrayInSWReq  = FALSE;
					theProcBank.m_bGoodInTrayMuteOn = FALSE; 
					_func.TrayData_Create(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_BUFFER);
					_func.TrayInBuzzer(3,OFF);
					_func.CIM_TRAYProcess(_T("351"),_T("4"));
				}
				theProcBank.m_bTrayInStart = FALSE;
			}
		}
		else if( !_func.GetInPutIOCheck(X_GOOD_LD_BUFFER_CONV_TRAY_SEN_1) &&
				 !_func.GetInPutIOCheck(X_GOOD_LD_BUFFER_CONV_TRAY_SEN_4) &&
				 !_func.GetInPutIOCheck(X_GOOD_LD_BUFFER_CONV_TRAY_SEN_5)	)
		{
			if(_func.TrayData_Exist(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_BUFFER) && theProcBank.GoodTrayInLiftingFlag == FALSE)
			{
				_func.TrayData_RemoveData(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_BUFFER);
				_func.CIM_TRAYProcess(_T("350"),_T("4"));
				//theProcBank.m_bTrayInStart = FALSE;
			}
			theProcBank.m_bTrayInStart = FALSE;
		}

		//20170802,JSJUNG, preload abnormal case ���� 
		//��� ���� ������ �������
		// GOOD IN BUFFER�� ��������� 350����
		// GOOD OUT BUFFER�� Ʈ���̰� �������� 352����
		if(_LastState != theProcBank.GetMachineState())
		{
			if((theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)
				&& (_LastState == CONST_MACHINE_STATE::MACHINE_INIT || _LastState == CONST_MACHINE_STATE::MACHINE_PAUSE
				|| _LastState == CONST_MACHINE_STATE::MACHINE_FAULT || _LastState == CONST_MACHINE_STATE::MACHINE_INTERLOCK))
			{				
				// ����� �����....
				if( !_func.GetInPutIOCheck(X_GOOD_LD_BUFFER_CONV_TRAY_SEN_1) &&
				 !_func.GetInPutIOCheck(X_GOOD_LD_BUFFER_CONV_TRAY_SEN_4) &&
				 !_func.GetInPutIOCheck(X_GOOD_LD_BUFFER_CONV_TRAY_SEN_5)	)
				{
					if(!_func.TrayData_Exist(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_BUFFER) && theProcBank.GoodTrayInLiftingFlag == FALSE)
					{
						_func.CIM_TRAYProcess(_T("350"),_T("4"));
					}
				}

				if(_func.TrayData_Exist(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_OUT_BUFFER))
				{
					if( _func.GetInPutIOCheck(X_GOOD_ULD_BUFFER_CONV_TRAY_SEN_1))
					{						
						_func.CIM_TRAYProcess(_T("352"),_T("2")); 
					}
				}
			}
			else if((_LastState == CONST_MACHINE_STATE::MACHINE_RUN || _LastState == CONST_MACHINE_STATE::MACHINE_IDLE)
				&& (theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_INIT || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_PAUSE
				|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_FAULT || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_INTERLOCK))
			{
				// �����ϴ� �����...
				if( !_func.GetInPutIOCheck(X_GOOD_LD_BUFFER_CONV_TRAY_SEN_1) &&
				 !_func.GetInPutIOCheck(X_GOOD_LD_BUFFER_CONV_TRAY_SEN_4) &&
				 !_func.GetInPutIOCheck(X_GOOD_LD_BUFFER_CONV_TRAY_SEN_5)	)
				{
					if(!_func.TrayData_Exist(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_BUFFER) && theProcBank.GoodTrayInLiftingFlag == FALSE)
					{
						_func.CIM_TRAYProcess(_T("350"),_T("4"));
					}
				}

				if(_func.TrayData_Exist(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_OUT_BUFFER))
				{
					if( _func.GetInPutIOCheck(X_GOOD_ULD_BUFFER_CONV_TRAY_SEN_1))
					{
						_func.CIM_TRAYProcess(_T("352"),_T("2")); 
					}
				}				
			}
			_LastState = theProcBank.GetMachineState();
		}

		//20170801,JSJUNG, preload abnormal case ���� 
		//Ʈ���� �ƿ� ���ۿ� ���� �����ǰ� ������ ���� �ô� �˶�.
		if( _func.GetInPutIOCheck(X_GOOD_ULD_BUFFER_CONV_TRAY_SEN_1) && theProcBank.GoodTrayOutAGVFlag == FALSE)
		{
			//2017-12-12,SKCHO, TRAY ID������ 352���� �ȵǰ� ����
			if(!_func.TrayData_Exist(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_OUT_BUFFER) && theProcBank.m_strClose352TrayID != _T(""))
			{
				//2017-10-09,SKCHO, �˶� ��� ���� �����ϰ� ��Ʈ ���� ����
				_func.TrayData_Create(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_OUT_BUFFER);				
				_func.CIM_TRAYProcess(_T("352"),_T("2"));
				//_func.SetError(ALARM_OKTRAY_OUT_BUF_CONV_TRAY_CHK);

				//2017-12-04,SKCHO, 352���� �� ���α׷� ����� �� ���� �������� �� 353���� ���� �������
				_func.ReadPortEvent353();
			}
		}

		//20170801,JSJUNG, preload abnormal case ���� 
		//Ʈ���� �ƿ� ���ۿ� Ʈ���̰� �ִٰ� ������Ʈ ������ Ʈ���̸� ������ 10���� �ڵ� ���ø�Ʈ
		//�ڵ� ���ø�Ʈ �Ҷ� 353����
		//Ʈ���� ����Ʈ���� �ƿ����� ���� ���� �Ҷ��� �۵�����.
		// agv�� ���۽ô� �׽�Ʈ �ʿ���.
		if(_func.TrayData_Exist(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_OUT_BUFFER))
		{
			//2017-12-26,SKCHO, SEN1�Ѱ�����  SEN1 AND SEN4 ���� ���� �ȵɶ��� ����
			//if( !_func.GetInPutIOCheck(X_GOOD_ULD_BUFFER_CONV_TRAY_SEN_1))
			if(_func.BufConvSenChk(GOOD_OUT_TRAY_PORT,SENSOR_ON) == FALSE )
			{
				if(theProcBank.m_bTrayOutStart == FALSE)
				{
					m_timerTrayOutBufOut.Start();
					theProcBank.m_bTrayOutStart = TRUE;
				}
				else
				{
					if(m_timerTrayOutBufOut.Stop(FALSE) > 10. && theProcBank.GoodTrayOutAGVFlag == FALSE)
					{
						_func.SetOutPutIO(Y_GOOD_ULD_TRAY_MUTING_LAMP_ON,OFF);
						_func.SetOutPutIO(Y_GOOD_ULD_TRAY_REQ_SW,OFF);
						_func.SetOutPutIO(Y_GOOD_ULD_TRAY_MUTE_ON,OFF);
						_func.SetOutPutIO(Y_GOOD_ULD_TRAY_COMP_SW,ON);
						theProcBank.m_bGoodTrayOutSWReq = FALSE;
						theProcBank.m_bGoodOutTrayMuteOn = FALSE; 
						_func.TrayData_RemoveData(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_OUT_BUFFER);
						theLog[LOG_AGV].AddBuf( _T("Tray Out BufConvSensor not Check in 10 Seconds"));
						_func.CIM_TRAYProcess(_T("353"),_T("2")); 
						theProcBank.m_bTrayOutStart = FALSE;
						
						//2018-06-11,NAM
						/*if (theProcBank.m_AlarmID== ALARM_OKTRAY_OUT_BUFF_PORT_FULL)
						{
							theProcBank.ResetTrayAlarm(2);  //2�� ALARM_OKTRAY_OUT_BUFF_PORT_FULL �˶� ����
							//theProcBank.AlarmClear();
						}*/
						theProcBank.ResetTrayAlarm(1);  //2�� ALARM_OKTRAY_OUT_BUFF_PORT_FULL �˶� ����

					}
				}
			}
			else
			{
				theProcBank.m_bTrayOutStart = FALSE;
			}
		}
		//2017-04-10,skcho, init / ORIGIN�϶��� ���� �ȵǰ� (ȯ�������û)
		// motor Setup ȭ�� �󿡼��� ���� ���� �ȵǰ�. 2017/06/07 HSK.
		if((theProcBank.m_bMotorSetupChk == FALSE) && (theProcBank.GetMachineState() != CONST_MACHINE_STATE::MACHINE_INIT))
		{
			// Ʈ���� �� ���� ���� ��ư ������ �� ����.
			if (_func.TrayReqSwChk(GOOD_IN_TRAY_PORT))
			{
				if(_func.BufConvSenChk(GOOD_IN_TRAY_PORT,SENSOR_ON) == FALSE)
				{
					_func.SetOutPutIO(Y_GOOD_LD_TRAY_MUTING_LAMP_ON,ON);
					_func.SetOutPutIO(Y_GOOD_LD_TRAY_REQ_SW,ON);
					_func.SetOutPutIO(Y_GOOD_LD_TRAY_MUTE_ON,ON);
					_func.SetOutPutIO(Y_GOOD_LD_TRAY_COMP_SW,OFF);
					theProcBank.m_bGoodInTrayMuteOn = TRUE; // 2017.2.3 bgkim
				}
			}
			if (_func.TrayCompSwChk(GOOD_IN_TRAY_PORT)
				&& theProcBank.GetMachineState() != CONST_MACHINE_STATE::MACHINE_INIT)  //2017-04-10,skcho, init�϶��� ���� �ȵǰ� (ȯ�������û)
			{
				if(_func.BufConvSenChk(GOOD_IN_TRAY_PORT,SENSOR_ON) && _func.TrayData_Exist(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_IN_BUFFER) == FALSE)
				{
					_func.SetOutPutIO(Y_GOOD_LD_TRAY_MUTING_LAMP_ON,OFF);
					_func.SetOutPutIO(Y_GOOD_LD_TRAY_REQ_SW,OFF);
					_func.SetOutPutIO(Y_GOOD_LD_TRAY_MUTE_ON,OFF);
					_func.SetOutPutIO(Y_GOOD_LD_TRAY_COMP_SW,ON);
					theProcBank.m_bGoodTrayInSWReq  = FALSE;
					theProcBank.m_bGoodInTrayMuteOn = FALSE; // 2017.2.3 bgkim
					_func.TrayData_Create(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_BUFFER);

					//2017-09-12,SKCHO, TRAY ������ ����
					//2017-05-23,skcho,BUFFER�� 
					//theUnitFunc.TrayInBuzzer(3,OFF);
					theProcBank.m_nGoodInPortEmptyStep = 3;

					//2017-07-17,SKCHO, BUFFER PORT�� Ʈ���̰� ������ Pre-Load����, CIM QUAL��û
					theUnitFunc.CIM_TRAYProcess(_T("351"),_T("4"));
				}
			}

			//2018-06-11,NAM, Reset Tray Alarm when Putting the NG TRAY IN 
			if(theProcBank.GetTrayAlarmChk())
			{
				if(theProcBank.m_bFaultOfTray[2] == TRUE)//ALARM_NGTRAY_IN_PORT_ALIGN_EMPTY)
				{
					if (_func.GetInPutIOCheck(X_NG_BUFFER_CONV_TRAY_SEN_3)==TRUE && _func.GetInPutIOCheck(X_NG_BUFFER_CONV_TRAY_SEN_4)==TRUE)

						theProcBank.ResetTrayAlarm(2);
				}
			}

			if (_func.TrayReqSwChk(GOOD_OUT_TRAY_PORT)
				&& theProcBank.GetMachineState() != CONST_MACHINE_STATE::MACHINE_INIT)  //2017-04-10,skcho, init�϶��� ���� �ȵǰ� (ȯ�������û)
			{
				if(_func.BufConvSenChk(GOOD_OUT_TRAY_PORT,SENSOR_ON))
				{
					_func.SetOutPutIO(Y_GOOD_ULD_TRAY_MUTING_LAMP_ON,ON);
					_func.SetOutPutIO(Y_GOOD_ULD_TRAY_REQ_SW,ON);
					_func.SetOutPutIO(Y_GOOD_ULD_TRAY_MUTE_ON,ON);
					_func.SetOutPutIO(Y_GOOD_ULD_TRAY_COMP_SW,OFF);
					_func.BottomStoperUpDown(GOOD_OUT_TRAY_PORT, CYL_DOWN); // 2017.2.7 bgkim
					theProcBank.m_bGoodTrayOutSWReq = TRUE;
					theProcBank.m_bGoodOutTrayMuteOn = TRUE; // 2017.2.3 bgkim
				}
			}
			if (_func.TrayCompSwChk(GOOD_OUT_TRAY_PORT)
				&& theProcBank.GetMachineState() != CONST_MACHINE_STATE::MACHINE_INIT)  //2017-04-10,skcho, init�϶��� ���� �ȵǰ� (ȯ�������û)
			{
				if(_func.BufConvSenChk(GOOD_OUT_TRAY_PORT,SENSOR_ON) == FALSE && _func.TrayData_Exist(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_OUT_BUFFER) == TRUE)
				{
					_func.SetOutPutIO(Y_GOOD_ULD_TRAY_MUTING_LAMP_ON,OFF);
					_func.SetOutPutIO(Y_GOOD_ULD_TRAY_REQ_SW,OFF);
					_func.SetOutPutIO(Y_GOOD_ULD_TRAY_MUTE_ON,OFF);
					_func.SetOutPutIO(Y_GOOD_ULD_TRAY_COMP_SW,ON);
					theProcBank.m_bGoodTrayOutSWReq = FALSE;
					theProcBank.m_bGoodOutTrayMuteOn = FALSE; // 2017.2.3 bgkim
					_func.TrayData_RemoveData(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_OUT_BUFFER);

					//2017-08-22 KAKAKA
					theUnitFunc.TrayInBuzzer(4,OFF);
					theLog[LOG_AGV].AddBuf( _T("Tray Out Complete Button"));
					_func.CIM_TRAYProcess(_T("353"),_T("2")); 
				
					//2018-04-24
					//2018-06-11,NAM
						/*if (theProcBank.m_AlarmID== ALARM_OKTRAY_OUT_BUFF_PORT_FULL)
							{
							theProcBank.ResetTrayAlarm(2);  //2�� ALARM_OKTRAY_OUT_BUFF_PORT_FULL �˶� ����
							//theProcBank.AlarmClear();
							}
							*/
					theProcBank.ResetTrayAlarm(1);  //2�� ALARM_OKTRAY_OUT_BUFF_PORT_FULL �˶� ����
					

				}

				// ��� Ʈ���� ���� �ϸ� ���� ���ش� 2017/04/30 HSK.
				if((_func.TrayData_Exist(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_OUT_BUFFER) == FALSE)
					&& (_func.TrayData_Exist(NG_TRAY_PORT, TRAY_INFO_POS_NG_OUT_COMP_BUFFER) == FALSE))
				{
					_func.TrayOutBuzzer(TRUE,OFF);
					_func.TrayOutBuzzer(FALSE,OFF);
				}
			}

			if (_func.TrayReqSwChk(NG_TRAY_PORT)
				&& theProcBank.GetMachineState() != CONST_MACHINE_STATE::MACHINE_INIT)  //2017-04-10,skcho, init�϶��� ���� �ȵǰ� (ȯ�������û)
			{
				if(_func.BufConvSenChk(NG_TRAY_PORT,SENSOR_ON) == FALSE)
				{
					_func.SetOutPutIO(Y_NG_TRAY_MUTING_LAMP_ON,ON);
					_func.SetOutPutIO(Y_NG_TRAY_IN_REQ_SW,ON);
					_func.SetOutPutIO(Y_NG_TRAY_MUTE_ON,ON);
					_func.SetOutPutIO(Y_NG_TRAY_IN_COMP_SW,OFF);
					theProcBank.m_bNGTrayMuteOn = TRUE; // 2017.2.3 bgkim
				}
			}
			if (_func.TrayCompSwChk(NG_TRAY_PORT)
				&& theProcBank.GetMachineState() != CONST_MACHINE_STATE::MACHINE_INIT)  //2017-04-10,skcho, init�϶��� ���� �ȵǰ� (ȯ�������û)
			{
				if(_func.BufConvSenChk(NG_TRAY_PORT,SENSOR_ON))
				{
					_func.SetOutPutIO(Y_NG_TRAY_MUTING_LAMP_ON,OFF);
					_func.SetOutPutIO(Y_NG_TRAY_IN_REQ_SW,OFF);
					_func.SetOutPutIO(Y_NG_TRAY_MUTE_ON,OFF);
					_func.SetOutPutIO(Y_NG_TRAY_IN_COMP_SW,ON);
					theProcBank.m_bNGTrayInSWReq = FALSE;
					theProcBank.m_bNGTrayMuteOn = FALSE; // 2017.2.3 bgkim
					_func.TrayData_Create(NG_TRAY_PORT,TRAY_INFO_POS_NG_OUT_REQ_BUFFER);

					//2017-09-12,SKCHO, Ʈ���� ������ ����
					//2017-05-23,skcho,BUFFER�� 
					//theUnitFunc.TrayInBuzzer(2,OFF);
					theProcBank.m_nNGInPortEmptyStep = 3;
				}

			}
		
			if (_func.TrayReqSwChk(NG_BUFFER_TRAY_PORT)
				&& theProcBank.GetMachineState() != CONST_MACHINE_STATE::MACHINE_INIT)  //2017-04-10,skcho, init�϶��� ���� �ȵǰ� (ȯ�������û)
			{
				if(_func.BufConvSenChk(NG_TRAY_PORT,SENSOR_ON))
				{
					_func.SetOutPutIO(Y_NG_TRAY_MUTING_LAMP_ON,ON);
					_func.SetOutPutIO(Y_NG_TRAY_OUT_REQ_SW,ON);
					_func.SetOutPutIO(Y_NG_TRAY_MUTE_ON,ON);
					_func.SetOutPutIO(Y_NG_TRAY_OUT_COMP_SW,OFF);
					//_func.BottomStoperUpDown(NG_TRAY_PORT, CYL_DOWN, TRAY_IN); // 2017.2.7 bgkim
					_func.BottomStoperUpDown(NG_TRAY_PORT, CYL_DOWN);  //2017-04-23,skcho, buffer complete ������ ������ �ٿ�
					theProcBank.m_bNGTrayOutSWReq = TRUE;
					theProcBank.m_bNGTrayMuteOn = TRUE; // 2017.2.3 bgkim
				}

			}
			if (_func.TrayCompSwChk(NG_BUFFER_TRAY_PORT)
				&& theProcBank.GetMachineState() != CONST_MACHINE_STATE::MACHINE_INIT)  //2017-04-10,skcho, init�϶��� ���� �ȵǰ� (ȯ�������û)
			{
				if(_func.BufConvSenChk(NG_TRAY_PORT,SENSOR_ON) == FALSE)
				{
					_func.SetOutPutIO(Y_NG_TRAY_MUTING_LAMP_ON,OFF);
					_func.SetOutPutIO(Y_NG_TRAY_OUT_REQ_SW,OFF);
					_func.SetOutPutIO(Y_NG_TRAY_MUTE_ON,OFF);
					_func.SetOutPutIO(Y_NG_TRAY_OUT_COMP_SW,ON);
					theProcBank.m_bNGTrayOutSWReq = FALSE;
					theProcBank.m_bNGTrayMuteOn = FALSE; // 2017.2.3 bgkim
					_func.TrayData_RemoveData(NG_TRAY_PORT,TRAY_INFO_POS_NG_OUT_COMP_BUFFER);
					//--_func.CIM_TRAYProcess(_T("353"),_T("3"));  //2017-05-13,skcho, NG ��Ʈ ���� ���ϹǷ� ����
				}
				// ��� Ʈ���� ���� �ϸ� ���� ���ش� 2017/04/30 HSK.
				if((_func.TrayData_Exist(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_OUT_BUFFER) == FALSE)
					&& (_func.TrayData_Exist(NG_TRAY_PORT, TRAY_INFO_POS_NG_OUT_COMP_BUFFER) == FALSE))
				{
					_func.TrayOutBuzzer(TRUE,OFF);
					_func.TrayOutBuzzer(FALSE,OFF);
				}
			}

			//ȯ����� �������� - ���� ���߿� ���� �ȵǰ�. 2017/05/04 HSK
			if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_INIT
				|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_PAUSE
				|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_FAULT
				|| theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_INTERLOCK)
			{			

				if (_func.TrayMuteChk(NG_TRAY_PORT) 
					&& theProcBank.m_bNGTrayMuteSWChk == FALSE 
					&& _func.m_NGMuteTimer1.Stop(FALSE) > 1.)
				{
					_func.SetOutPutIO(Y_NG_TRAY_MUTING_LAMP_ON,ON);
					_func.SetOutPutIO(Y_NG_TRAY_MUTE_ON,ON);
					theProcBank.m_bNGTrayMuteSWChk = TRUE;
					theProcBank.m_bNGTrayMuteOn = TRUE; // 2017.2.3 bgkim

					_func.TrayMuteTimeDelay2(NG_TRAY_PORT);
				}
				else if(_func.TrayMuteChk(NG_TRAY_PORT) && theProcBank.m_bNGTrayMuteSWChk == TRUE && _func.m_NGMuteTimer2.Stop(FALSE) > 1.)
				{
					_func.SetOutPutIO(Y_NG_TRAY_MUTING_LAMP_ON,OFF);
					_func.SetOutPutIO(Y_NG_TRAY_MUTE_ON,OFF);
					theProcBank.m_bNGTrayMuteSWChk = FALSE;
					theProcBank.m_bNGTrayMuteOn = FALSE; // 2017.2.3 bgkim
					_func.TrayMuteTimeDelay1(NG_TRAY_PORT);
				}

				if (_func.TrayMuteChk(GOOD_IN_TRAY_PORT)
					&& theProcBank.m_bGoodTrayInMuteSWChk == FALSE
					&& _func.m_GoodInMuteTimer1.Stop(FALSE) > 1.)
				{
					_func.SetOutPutIO(Y_GOOD_LD_TRAY_MUTING_LAMP_ON,ON);
					_func.SetOutPutIO(Y_GOOD_LD_TRAY_MUTE_ON,ON);
					theProcBank.m_bGoodTrayInMuteSWChk = TRUE;
					theProcBank.m_bGoodInTrayMuteOn = TRUE;
					_func.TrayMuteTimeDelay2(GOOD_IN_TRAY_PORT);
				}
				else if(_func.TrayMuteChk(GOOD_IN_TRAY_PORT)
					&& theProcBank.m_bGoodTrayInMuteSWChk == TRUE 
					&& _func.m_GoodInMuteTimer2.Stop(FALSE) > 1.)
				{
					_func.SetOutPutIO(Y_GOOD_LD_TRAY_MUTING_LAMP_ON,OFF);
					_func.SetOutPutIO(Y_GOOD_LD_TRAY_MUTE_ON,OFF);
					theProcBank.m_bGoodTrayInMuteSWChk = FALSE;
					theProcBank.m_bGoodInTrayMuteOn = FALSE;
					_func.TrayMuteTimeDelay1(GOOD_IN_TRAY_PORT);
				}

				if (_func.TrayMuteChk(GOOD_OUT_TRAY_PORT) 
					&& theProcBank.m_bGoodTrayOutMuteSWChk == FALSE
					&& _func.m_GoodOutMuteTimer1.Stop(FALSE) > 1)
				{
					_func.SetOutPutIO(Y_GOOD_ULD_TRAY_MUTING_LAMP_ON,ON);
					_func.SetOutPutIO(Y_GOOD_ULD_TRAY_MUTE_ON,ON);
					theProcBank.m_bGoodTrayOutMuteSWChk = TRUE;
					theProcBank.m_bGoodOutTrayMuteOn = TRUE;
					_func.TrayMuteTimeDelay2(GOOD_OUT_TRAY_PORT);
				}
				else if(_func.TrayMuteChk(GOOD_OUT_TRAY_PORT) && theProcBank.m_bGoodTrayOutMuteSWChk == TRUE && _func.m_GoodOutMuteTimer2.Stop(FALSE) > 1)
				{
					_func.SetOutPutIO(Y_GOOD_ULD_TRAY_MUTING_LAMP_ON,OFF);
					_func.SetOutPutIO(Y_GOOD_ULD_TRAY_MUTE_ON,OFF);
					theProcBank.m_bGoodTrayOutMuteSWChk = FALSE;
					theProcBank.m_bGoodOutTrayMuteOn = FALSE;
					_func.TrayMuteTimeDelay1(GOOD_OUT_TRAY_PORT);
				}
				if(_func.DoorInterLockChk()==FALSE)
				{
					//2017-04-18,skcho, ȯ����� �������� - ƼĪ��忡�� ���� �̵��� �� ������ ���� ����
					theDeviceMotion.AxisStop(MP2100_THREAD_IO_CHECK,theProcBank.m_nPresentAxis);
				}

				//2017-07-08,SKCHO, Unloader�� ���͸� ���߰� ����(agv ��ſ��� muting�� on �϶� �ٸ� ���͵��� �� ������ �˶� ó���� �ȵ�)
				if(theProcBank.m_nPresentAxis >= AXIS_SVO_GOOD_TRAY_OUT_LIFT_Z && theProcBank.m_nPresentAxis <= AXIS_SVO_NG_TRAY_TR_Z)
				{
					if(theProcBank.m_bNGTrayMuteOn == TRUE || theProcBank.m_bGoodInTrayMuteOn == TRUE || theProcBank.m_bGoodOutTrayMuteOn == TRUE)
					{
						//ȯ����� �������� - ���� ���� ���� �� ���� �̵��� �� ������ ���� ���� 2017/05/04 HSK
						theDeviceMotion.AxisStop(MP2100_THREAD_IO_CHECK,theProcBank.m_nPresentAxis);
					}
				}
			}
		
		}
		//2018-06-26,NAM 
		if (_func.CellTagExist(CELL_POS_UD_ROBOT) 
			&& (_func.CellData_GetLastClass(CELL_POS_UD_ROBOT) == GOOD_CELL)
			&& (theProcBank.GetOtherAlarmChk() == FALSE)
			&& theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN
			&& stateUDRobotGoodPut.IsStoped() && theConfigBank.m_Option.m_bUseConveyorMode  //2018-07-03,NAM, Check When ULD ROBOT PUT IN TRAY 
			&&_func.GetInPutIOState(X_HYBRID_CONV_READY) == TRUE) //2018-08-19,NAM,Checking if Robot Ready ON. 
			_func.SetOutPutIO(Y_HYBRID_ROBOT_READY,ON);
		else 
			_func.SetOutPutIO(Y_HYBRID_ROBOT_READY,OFF);

		//2017-10-09,SKCHO,AUTOTEACHKEY IO VALUE CHECK
		_func.AutoTeachKeyIOState();
		//2017-10-09,SKCHO, AGV INPUT IO VALUE CHECK
		_func.AGVReceiveIOState();
		//2017-10-07,SKCHO, AGV OUTPUT IO VALUE CHECK
		_func.AGVSendIOState();
		//2017-10-07,SKCHO, VACCUM IO CHECK
		_func.VacuumIOState();
		//2018-05-10,MGYUN, Hybird Conv IO CHECK
		_func.HybridConvIOState();

		if(_func.EMSChk() == FALSE)
			theProcBank.m_bOriginSuccess = FALSE;
		
		_func.LightCurtainOnCheck();

		_func.TempChk();
		_func.MainVacChk();
		_func.FANChk();
		}
	return 0;
}

// �˻� ������ �� �� VT File ���� �Ȱ� Ȯ�� �� Data �о� ���̴� Thr.
UINT LoadVTFileThr(LPVOID lpParam)
{
	CCellTag tagCell[JIG_CH_MAX];
	CCellInfo* pCell;

	int nThreadIndex = (int)lpParam;

	CString	szFileName,strFolderPath, strLocalFileName;
	CString strKey, strTmp,strBuf;
	int i = 0, nRejectIndex = 0,nCnt = 0;
	CTokenizer t;
	CIni ini;
	int nRejectCount[7];
	memset(nRejectCount, 0, sizeof(int) * 7);

	while(theThreadInitializer.m_bThrExist[nThreadIndex] == FALSE)
	{
		theThreadInitializer.WaitLoadVTFileThread();

		if(theThreadInitializer.m_bThrExist[nThreadIndex])
			break;

		theLog[LOG_VISION].AddBuf( _T("LoadVTFileThr Start"));

		tagCell[JIG_CH_1] = theCellBank.GetCellTag(CELL_POS_DZONE_CH1);
		tagCell[JIG_CH_2] = theCellBank.GetCellTag(CELL_POS_DZONE_CH2);

		for(int nCh = 0; nCh < JIG_CH_MAX; nCh++)
		{
			if(tagCell[nCh].IsExist())
			{
				if (nCh == JIG_CH_1)
				{
					pCell = theCellBank.GetCellInfo(CELL_POS_DZONE_CH1);
					if (theUnitFunc.Get_InspectSkip(CELL_POS_DZONE_CH1) == TRUE)
					{						
						continue; 
					}
				}
				else
				{
					pCell = theCellBank.GetCellInfo(CELL_POS_DZONE_CH2);
					if (theUnitFunc.Get_InspectSkip(CELL_POS_DZONE_CH2) == TRUE)
					{						
						continue; 
					}
				}


/*				if( pCell->m_Zone_D.InspVIState == eSTATE_Insp_End )
				{
					::DeleteFile(szFileName);
					theLog[LOG_VISION].AddBuf(_T("File Load Skip : %s Ch %d\n"), pCell->m_strInnerID, nCh+1);
					continue;
				}
*/
				if (nCh == JIG_CH_1)
					strFolderPath = theConfigBank.m_System.m_strActive1Path;
				else
					strFolderPath = theConfigBank.m_System.m_strActive2Path;

				szFileName.Format(_T("%s\\%s.txt") , strFolderPath, pCell->m_strInnerID);

				//2018-01-10, JSJUNG. 3������ 5�� �õ��� ����
				// �˻� ��� ���� ���� �õ�
				for(i = 0; i < 5 ; i++)
				{
					if(CFileSupport::DirectoryCheck(strFolderPath)) 
						break;
					::Sleep(100);
				}
				if(i >= 5)		
				{
					if (nCh == JIG_CH_1)
						theUnitFunc.SetError(ALARM_DZONE_ACTIVE1_VT_NETWORK);
					else
						theUnitFunc.SetError(ALARM_DZONE_ACTIVE2_VT_NETWORK);

					break;
				}

				//2018-01-10, JSJUNG. 3������ 5�� �õ��� ����
				// �˻� ��� ���� Ȯ��
				for(i = 0; i < 5 ; i++)
				{
					if(CFileSupport::FileCheck(szFileName)) 
						break;
					::Sleep(100);
				}
				if(i >= 5)		
				{
					if (nCh == JIG_CH_1)
						theUnitFunc.SetError(ALARM_DZONE_ACTIVE1_VT_FILE);
					else
						theUnitFunc.SetError(ALARM_DZONE_ACTIVE2_VT_FILE);

					pCell->m_Zone_C.InspVIState = eSTATE_Insp_End;
					break;
				}

				if(CFileSupport::DirectoryCheck(theConfigBank.m_System.m_strVTLocalPath) == FALSE)
				{
					if(CFileSupport::CreateDirectory(theConfigBank.m_System.m_strVTLocalPath) == FALSE)
					{
						break;
					}
				}
				strLocalFileName.Format(_T("%s\\%s.txt") , theConfigBank.m_System.m_strVTLocalPath, pCell->m_strInnerID);
				::CopyFile(szFileName, strLocalFileName, FALSE);

				theLog[LOG_VISION].AddBuf(_T("Cell ID : %s, File Load Start : %s"), pCell->m_strCellID , strLocalFileName);

				ini.SetIniFilename(strLocalFileName);
				pCell->m_Zone_C.VIDefect = ini.GetString(_T("ACTIVE RESULT"), _T("ACTIVE_REJECT_NAME"), _T("NO DATA"));
				pCell->m_Zone_C.CZoneDefect = pCell->m_Zone_C.VIDefect;
				theLog[LOG_VISION].AddBuf(_T("Main Defect : %s"), pCell->m_Zone_C.VIDefect);
				pCell->m_Zone_C.VIClassJudge = ini.GetLong(_T("ACTIVE RESULT"), _T("ACTIVE_REJECT_BIN_CLASS"), 2);
				theLog[LOG_VISION].AddBuf( _T("Class : %d"), pCell->m_Zone_C.VIClassJudge);

				switch(pCell->m_Zone_C.VIClassJudge)
				{
					case 0:		
						pCell->m_Zone_C.VIClass = GOOD_CELL;		
						break;
					case 1:		
						pCell->m_Zone_C.VIClass = REJECT_CELL;
						break;
					case 13:		
						pCell->m_Zone_C.VIClass = GOOD_CELL;
						break;
					default:	
						pCell->m_Zone_C.VIClass = BIN2_CELL;
						break;
				}
				pCell->m_Zone_C.CZoneClass = pCell->m_Zone_C.VIClass;

				pCell->m_Zone_C.GrabTime = ini.GetDouble(_T("ACTIVE RESULT"), _T("ACTIVE_REJECT_GRAB"));
				theLog[LOG_VISION].AddBuf(_T("Grab Time : %f"), pCell->m_Zone_C.GrabTime);
				pCell->m_Zone_C.ClassTime = ini.GetDouble(_T("ACTIVE RESULT"), _T("ACTIVE_REJECT_CLASS"));
				theLog[LOG_VISION].AddBuf(_T("Class Time : %f"), pCell->m_Zone_C.ClassTime);

				if (nCh == JIG_CH_1)
				{
					pCell->m_strMICRO_DLL_VERSION = ini.GetString(_T("FDC INFORMATION"),_T("FDC_MICRO_VERSION_AC1"),_T(""));
					theLog[LOG_VISION].AddBuf( _T("FDC_MICRO_VERSION_AC1 : %s"), pCell->m_strMICRO_DLL_VERSION);

					pCell->m_strMACRO_DLL_VERSION = ini.GetString(_T("FDC INFORMATION"),_T("FDC_MACRO_VERSION_AC1"),_T(""));
					theLog[LOG_VISION].AddBuf( _T("FDC_MACRO_VERSION_AC1 : %s"), pCell->m_strMACRO_DLL_VERSION);

					pCell->m_strSETBIN_VERSION = ini.GetString(_T("FDC INFORMATION"),_T("FDC_SETBIN_VERSION_AC1"),_T(""));
					theLog[LOG_VISION].AddBuf( _T("FDC_SETBIN_VERSION_AC1 : %s"), pCell->m_strSETBIN_VERSION);

					pCell->m_strUNIATS_VERSION = ini.GetString(_T("FDC INFORMATION"),_T("FDC_UNIATS_VERSION_AC1"),_T(""));
					theLog[LOG_VISION].AddBuf( _T("FDC_UNIATS_VERSION_AC1 : %s"), pCell->m_strUNIATS_VERSION);
				}
				else
				{
					pCell->m_strMICRO_DLL_VERSION = ini.GetString(_T("FDC INFORMATION"),_T("FDC_MICRO_VERSION_AC2"),_T(""));
					theLog[LOG_VISION].AddBuf( _T("FDC_MICRO_VERSION_AC2 : %s"), pCell->m_strMICRO_DLL_VERSION);

					pCell->m_strMACRO_DLL_VERSION = ini.GetString(_T("FDC INFORMATION"),_T("FDC_MACRO_VERSION_AC2"),_T(""));
					theLog[LOG_VISION].AddBuf( _T("FDC_MACRO_VERSION_AC2 : %s"), pCell->m_strMACRO_DLL_VERSION);

					pCell->m_strSETBIN_VERSION = ini.GetString(_T("FDC INFORMATION"),_T("FDC_SETBIN_VERSION_AC2"),_T(""));
					theLog[LOG_VISION].AddBuf( _T("FDC_SETBIN_VERSION_AC2 : %s"), pCell->m_strSETBIN_VERSION);

					pCell->m_strUNIATS_VERSION = ini.GetString(_T("FDC INFORMATION"),_T("FDC_UNIATS_VERSION_AC2"),_T(""));
					theLog[LOG_VISION].AddBuf( _T("FDC_UNIATS_VERSION_AC2 : %s"), pCell->m_strUNIATS_VERSION);
				}

/*[DETAIL RESULT]
DETAIL_REJECT_COUNT=1

DETAIL_REJECT_NAME_1=MASK_RIB
DETAIL_RESULT_DOT_SIZE_1=6
DETAIL_RESULT_DOT_WIDTH_1=1353
DETAIL_RESULT_DOT_HEIGHT_1=67
DETAIL_RESULT_DOT_PSMMAXVAL_1=342
DETAIL_RESULT_LINE_LENGTH_1=8
DETAIL_RESULT_LINE_BRIGHTNESS_1=342
DETAIL_RESULT_LINE_THICKNESS_1=6
DETAIL_REJECT_XY_1=1875,36
DETAIL_REJECT_IMG_XY_1=522,103
DETAIL_REJECT_CIRCLE_DRAW_1=0
DETAIL_REJECT_DEFECT_GROUP_1=0
DETAIL_REJECT_RGB_1=B
DETAIL_REJECT_PATTERN_NAME_1=WHITE
DETAIL_REJECT_PATTERN_NO_1=0
DETAIL_REJECT_COORD_XY_1=0,0
DETAIL_REJECT_VIS_1=103
DETAIL_REJECT_IMAGE_1=
*/   
//7���� �ҷ��� ���ؼ��� DV���� �Ѵ�.
/*	ETC HIGH
    HIGH CONTIGUITY
	HIGH DOT
	H-O CONTIGUITY
	HIGH DOT MULTITUDE
	HIGH ETC
	HIGH MULTITUDE
	*/
				CString strX,strY,strBuf1;
				nRejectIndex = 0;
				//2017-09-24,SKCHO
				strTmp= ini.GetString(_T("DETAIL RESULT"), _T("DETAIL_REJECT_COUNT"), _T(" "));
				nCnt = _ttoi(strTmp);
				//if(pCell->m_Zone_C.VIRejectCount >= MAX_REJECT_CNT )  pCell->m_Zone_C.VIRejectCount = MAX_REJECT_CNT;
				for(int i = 0; i < nCnt; i++)
				{
					strTmp.Format(_T("DETAIL_REJECT_NAME_%d"),i+1);
					strBuf = ini.GetString(_T("DETAIL RESULT"), strTmp, _T(" "));
					if(strBuf == _T("ETC HIGH") 
						|| strBuf == _T("HIGH CONTIGUITY") 
						|| strBuf == _T("HIGH DOT") 
						|| strBuf == _T("H-O CONTIGUITY") 
						|| strBuf == _T("HIGH DOT MULTITUDE") 
						|| strBuf == _T("HIGH ETC") 
						|| strBuf == _T("HIGH MULTITUDE"))
					{
						if(nRejectIndex >= 5) break;

						if(strBuf == _T("ETC HIGH"))
						{
							nRejectCount[ETC_HIGH]++;
							strBuf1.Format(_T("ETC_HIGH_%d"),nRejectCount[ETC_HIGH]);								
						}
						if(strBuf == _T("HIGH CONTIGUITY"))
						{
							nRejectCount[HIGH_CONTIGUITY]++;
							strBuf1.Format(_T("HIGH_CONTIGUITY_%d"),nRejectCount[HIGH_CONTIGUITY]);	
						}
						if(strBuf == _T("HIGH DOT"))
						{
							nRejectCount[HIGH_DOT]++;
							strBuf1.Format(_T("HIGH_DOT_%d"),nRejectCount[HIGH_DOT]);	
						}
						if(strBuf == _T("H-O CONTIGUITY"))
						{
							nRejectCount[HO_CONTIGUITY]++;
							strBuf1.Format(_T("H-O_CONTIGUITY_%d"),nRejectCount[HO_CONTIGUITY]);
						}
						if(strBuf == _T("HIGH DOT MULTITUDE"))
						{
							nRejectCount[HIGH_DOT_MULTITUDE]++;
							strBuf1.Format(_T("HIGH_DOT_MULTITUDE_%d"),nRejectCount[HIGH_DOT_MULTITUDE]);
						}
						if(strBuf == _T("HIGH ETC")) 
						{
							nRejectCount[HIGH_ETC]++;
							strBuf1.Format(_T("HIGH_ETC_%d"),nRejectCount[HIGH_ETC]);
						}
						if(strBuf == _T("HIGH MULTITUDE"))
						{
							nRejectCount[HIGH_MULTITUDE]++;
							strBuf1.Format(_T("HIGH_MULTITUDE_%d"),nRejectCount[HIGH_MULTITUDE]);
						}

						pCell->m_Zone_C.stRejct[nRejectIndex].strName= strBuf1;
						strTmp.Format(_T("DETAIL_REJECT_XY_%d"),i+1);
						strBuf= ini.GetString(_T("DETAIL RESULT"), strTmp, _T(" "));
						//��ǥ�� ��ǥ(,)�� (_)�κ��� �ڵ�ȭ ��û����
						strX = strBuf.Mid(0,strBuf.Find(_T(",")));
						strY = strBuf.Mid(strBuf.Find(_T(","))+1,strBuf.GetLength());
						strBuf1.Format(_T("%s_%s"),strX,strY);
						pCell->m_Zone_C.stRejct[nRejectIndex].strXY = strBuf1;
						nRejectIndex++;
					}
					
				}

				memset(nRejectCount, 0, sizeof(int) * 7);

				pCell->m_Zone_C.VIRejectCount = nRejectIndex;
		
				// Vision �˶� �߻� ī��Ʈ ���� ���� 2017/04/26 HSK.
				/*
				if (pCell->m_Zone_C.CZoneClass != GOOD_CELL)
					theUnitFunc.SetVisionNGCnt(theUnitFunc.AMT_GetAutoJigID(ZONE_ID_D),(JIG_CH)nCh,FALSE);
				else
					theUnitFunc.SetVisionNGCnt(theUnitFunc.AMT_GetAutoJigID(ZONE_ID_D),(JIG_CH)nCh,TRUE);
				*/

				pCell->m_Zone_C.InspVIState = eSTATE_Insp_End;

				::DeleteFile(strLocalFileName);
				
				theLog[LOG_VISION].AddBuf(_T("File Load End\n"));
			}
		}
		theLog[LOG_VISION].AddBuf( _T("LoadVTFileThr End\n--------------------------------------------------------\n"));
	}

	return 0;
};

/*UINT FDCMonitorThr( LPVOID lpParam )
{
	static UINT nLD_ADSlot_Index = 0;
	static UINT nUD_ADSlot_Index = 0;
	static UINT nINSP_ADSlot_Index = 0;

	static UINT nTempIndex = 0;
	static UINT nReadCnt = 0;


	int nThreadIndex = (int)lpParam;

	theDeviceMotion.Open_Mp2100(MP2100_THREAD_FDC_MONITOR);
	CUnitCtrlFunc					_func(MP2100_THREAD_FDC_MONITOR);

// 	return 0;

	//////////////////////////////////////////////////////////////////////////
	// GMS
	theSerialInterFace.SerialOpen(SERIAL_GMS);

	CDeviceSerialGMS* pDeviceGMS = theSerialInterFace.GetDeviceGMSHandler();
	int nGMSCh = 0;
	BOOL bGMSSended = FALSE;
	CStopWatch m_timerGMS;
	//////////////////////////////////////////////////////////////////////////
	// Accura
	CDeviceAccura DeviceAccura;
	DeviceAccura.InitAccura();
	CStopWatch m_timerAccura;
	//////////////////////////////////////////////////////////////////////////

	//20161229 byskcho
	//���� ������  LOAD�� ����Ǿ� �־� �������� ���� ����
	//20170129 byskcho
	//cim oss ���� �������� ���� ����
#if(AMT_TYPE == 1)
	theSerialInterFace.SerialOpen(SERIAL_VACUUM_LD);
	theSerialInterFace.GetVACUUMLDHandler()->SetValueOffset(6);
	CStopWatch m_timerVacLD;

	theSerialInterFace.SerialOpen(SERIAL_VACUUM_UD);
	theSerialInterFace.GetVACUUMUDHandler()->SetValueOffset(-1.);
	CStopWatch m_timerVacUD;

	theSerialInterFace.SerialOpen(SERIAL_VACUUM_INSP);
	theSerialInterFace.GetVACUUMINSPHandler()->SetValueOffset(-1.);
	CStopWatch m_timerVacINSP;	
#else
	_func.LoadADBoardOffset();

	theSerialInterFace.SerialOpen(SERIAL_VACUUM_LD);
	CStopWatch m_timerVacLD;

	theSerialInterFace.SerialOpen(SERIAL_VACUUM_UD);
	CStopWatch m_timerVacUD;

	theSerialInterFace.SerialOpen(SERIAL_VACUUM_INSP);
	CStopWatch m_timerVacINSP;	


#endif

	
	theSerialInterFace.SendSetVacRange();
	BOOL bVacLDSended = FALSE;
	BOOL bVacUDSended = FALSE;
	BOOL bVacINSPSended = FALSE;

	theSerialInterFace.SerialOpen(SERIAL_TEMPERATURE);
	CStopWatch m_timerTemperature;	
	BOOL bTempSended = FALSE;
	//////////////////////////////////////////////////////////////////////////
	AxisStatus axisstatus;
	//////////////////////////////////////////////////////////////////////////
	// FDC Write
	CStopWatch m_timerFDCWrite;
	BOOL bFDCStart = FALSE;
	//////////////////////////////////////////////////////////////////////////
	while(theThreadInitializer.m_bThrExist[nThreadIndex] == FALSE)
	{
		Sleep(100);
// 		continue;

		//////////////////////////////////////////////////////////////////////////
		// GMS (1�� �������� GMS, PGMS ������ Read) [12/7/2016 OSC]
		if(pDeviceGMS->IsOpen())
		{
			
			if(pDeviceGMS->ParsingReciveValue())
			{
				m_timerGMS.Start();
				bGMSSended = FALSE;
			}
			else if( (m_timerGMS.Stop(FALSE) > 1.) && (bGMSSended == FALSE) )
			{
				pDeviceGMS->SendCommand_Read((GMS_MODULE_CH)GMS_CH001_GMS);
		
				//nGMSCh++;
				//nGMSCh %= GMS_MODULE_CH_MAX;
				bGMSSended = TRUE;
				m_timerGMS.Start();
			}
			else if((m_timerGMS.Stop(FALSE) > 10.) && bGMSSended )
			{
				// 10�ʰ� �ѵ��� ������ �ȿ��� bSended�� �ʱ�ȭ�Ͽ� �ٽ� �������� �Ѵ�
				bGMSSended = FALSE;
			}
		}
		//////////////////////////////////////////////////////////////////////////
		// Accura ���·��� (1�� �������� Read) [12/7/2016 OSC]
		if(m_timerAccura.Stop(FALSE) > 1.)
		{
			for(int i = 0; i < ACCURA_CLIENT_MAX; i++)
			{
				DeviceAccura.ReadValue((ACCURA_CLIENT)i);
				theFDCBank.m_rDataBlock[i] = DeviceAccura.m_rDataBlock[i];
			}
			m_timerAccura.Start();
		}
		//////////////////////////////////////////////////////////////////////////
		// Torque
		for(int i = 0; i < AXIS_ID_MAX; i++)
		{
			theFDCBank.m_dTorque[i] = theDeviceMotion.GetTorque(MP2100_THREAD_FDC_MONITOR, (AXIS_ID)i);
		}
		//////////////////////////////////////////////////////////////////////////
	
		theFDCBank.m_nEFU_Alarm = _func.GetInPutIOCheck(X_03F, OFF);
		if(theFDCBank.m_nEFU_Alarm == 0)
		{
			theUnitFunc.SetError(ALARM_FFU_SEN);
		}

		//////////////////////////////////////////////////////////////////////////
		// Main Air
		if(theSerialInterFace.GetVACUUMLDHandler()->IsOpen())
		{
			if(theSerialInterFace.ParsingLDVACUUMValue())
			{

				theSerialInterFace.CopyAirValue_to_FDCBank((SERIAL_MODE)SERIAL_VACUUM_LD);
				m_timerVacLD.Start();
				bVacLDSended = FALSE;
			}
			else if( (m_timerVacLD.Stop(FALSE) > 1.) && (bVacLDSended == FALSE) )
			{
				theSerialInterFace.SendVacReadLD(nLD_ADSlot_Index);
				bVacLDSended = TRUE;
				m_timerVacLD.Start();
				nLD_ADSlot_Index++;
				if(nLD_ADSlot_Index >= LD_AD_USE_SLOT)
				{
					nLD_ADSlot_Index = 0;
				}
			}
			else if((m_timerVacLD.Stop(FALSE) > 10.) && bVacLDSended )
			{
				// 10�ʰ� �ѵ��� ������ �ȿ��� bSended�� �ʱ�ȭ�Ͽ� �ٽ� �������� �Ѵ�
				bVacLDSended = FALSE;
			}

			
		}
		//20161229 byskcho
		//���� ������  LOAD�� ����Ǿ� �־� �������� ���� ����
		//20170129 byskcho
	    //cim oss ���� �������� ���� ����
		if(theSerialInterFace.GetVACUUMINSPHandler()->IsOpen())
		{
			if(theSerialInterFace.ParsingINSPVACUUMValue())
			{
				theSerialInterFace.CopyAirValue_to_FDCBank((SERIAL_MODE)SERIAL_VACUUM_INSP);
				m_timerVacINSP.Start();
				bVacINSPSended = FALSE;
			}
			else if( (m_timerVacINSP.Stop(FALSE) > 1.) && (bVacINSPSended == FALSE) )
			{
				theSerialInterFace.SendVacReadINSP(nINSP_ADSlot_Index);
				bVacINSPSended = TRUE;
				m_timerVacINSP.Start();
				nINSP_ADSlot_Index++;
				if(nINSP_ADSlot_Index >= INSP_AD_USE_SLOT)
				{
					nINSP_ADSlot_Index = 0;
				}
			}
			else if((m_timerVacINSP.Stop(FALSE) > 10.) && bVacINSPSended )
			{
				// 10�ʰ� �ѵ��� ������ �ȿ��� bSended�� �ʱ�ȭ�Ͽ� �ٽ� �������� �Ѵ�
				bVacINSPSended = FALSE;
			}
		}
		if(theSerialInterFace.GetVACUUMUDHandler()->IsOpen())
		{
			if(theSerialInterFace.ParsingUDVACUUMValue())
			{
				theSerialInterFace.CopyAirValue_to_FDCBank((SERIAL_MODE)SERIAL_VACUUM_UD);
				m_timerVacUD.Start();
				bVacUDSended = FALSE;
			}
			else if( (m_timerVacUD.Stop(FALSE) > 1.) && (bVacUDSended == FALSE) )
			{
				theSerialInterFace.SendVacReadUD(nUD_ADSlot_Index);
				bVacUDSended = TRUE;
				m_timerVacUD.Start();
				nUD_ADSlot_Index++;
				if(nUD_ADSlot_Index >= UD_AD_USE_SLOT)
				{
					nUD_ADSlot_Index = 0;
				}
			}
			else if((m_timerVacUD.Stop(FALSE) > 10.) && bVacUDSended )
			{
				// 10�ʰ� �ѵ��� ������ �ȿ��� bSended�� �ʱ�ȭ�Ͽ� �ٽ� �������� �Ѵ�
				bVacUDSended = FALSE;
			}
		}
		//TEMPERATURE SENSOR
		if(theSerialInterFace.GetTemperatureHandler()->IsOpen())
		{
			nTempIndex = theSerialInterFace.GetDeviceAddr();
			if(theSerialInterFace.ParsingTemperatureValue())
			{
				
				if(nReadCnt > 3)
				{ 					
					theSerialInterFace.CopyTempValue_to_FDCBank(nTempIndex);
					theSerialInterFace.GetTemperatureHandler()->IncDeviceAddr();
					nReadCnt = 0;
				}
				nReadCnt++;
				m_timerTemperature.Start();
				bTempSended = FALSE;
			}
			else if( (m_timerTemperature.Stop(FALSE) > 1.) && (bTempSended == FALSE) )
			{
				theSerialInterFace.SendTempRead(nTempIndex);
				bTempSended = TRUE;
				m_timerTemperature.Start();				
			}
			else if((m_timerTemperature.Stop(FALSE) > 10.) && bTempSended )
			{
				// 10�ʰ� �ѵ��� ������ �ȿ��� bSended�� �ʱ�ȭ�Ͽ� �ٽ� �������� �Ѵ�
				bTempSended = FALSE;
				//2017-04-14,skcho,10�ʵ��� ���� ������ ���� ��巹���� �Ѿ
				m_timerTemperature.Start();
				theSerialInterFace.GetTemperatureHandler()->IncDeviceAddr();
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// FDC Write 1�ʸ��� ���
		// ó�� 10�ʴ� Data�� ���� ������ �� �� ���� ���� �����־ ��ٸ���
		if( (bFDCStart == FALSE) && (m_timerFDCWrite.Stop(FALSE) > 10.) )
		{
			theFDCBank.DeleteFDC_SV_par();
			bFDCStart = TRUE;
		}
		if( bFDCStart && (m_timerFDCWrite.Stop(FALSE) > 1.) )
		{
			theFDCBank.SaveFDC_SV_par();
			m_timerFDCWrite.Start();
		}
		//////////////////////////////////////////////////////////////////////////
	}

	theSerialInterFace.SerialClose(SERIAL_GMS);
	theSerialInterFace.SerialClose(SERIAL_VACUUM_LD);
	theSerialInterFace.SerialClose(SERIAL_TEMPERATURE);

	theSerialInterFace.SerialClose(SERIAL_VACUUM_UD);
	theSerialInterFace.SerialClose(SERIAL_VACUUM_INSP);
	return 0;
}
*/

UINT FDCMonitorThr( LPVOID lpParam )
{
	static UINT nUD_ADSlot_Index = 0;

	static UINT nTempIndex = 0;
	static UINT nReadCnt = 0;

	int nThreadIndex = (int)lpParam;

	//2018-01-10, JSJUNG. �ʿ� ���� �ǴܵǾ� ���� //MP2100�α������� ���� �ٽ� ȸ��. 
	theDeviceMotion.Open_Mp2100(MP2100_THREAD_FDC_MONITOR);
	CUnitCtrlFunc					_func(MP2100_THREAD_FDC_MONITOR);

// 	return 0;

	//////////////////////////////////////////////////////////////////////////
	// GMS
	theSerialInterFace.SerialOpen(SERIAL_GMS);

	CDeviceSerialGMS* pDeviceGMS = theSerialInterFace.GetDeviceGMSHandler();
	int nGMSCh = 0;
	BOOL bGMSSended = FALSE;
	CStopWatch m_timerGMS;


	_func.LoadADBoardOffset();

	theSerialInterFace.SerialOpen(SERIAL_VACUUM_UD);
	CStopWatch m_timerVacUD;

	theSerialInterFace.SendSetVacRange(2);  //UD- 2
	BOOL bVacUDSended = FALSE;


	//////////////////////////////////////////////////////////////////////////
	// Accura
	CDeviceAccura DeviceAccura;
	DeviceAccura.InitAccura();
	CStopWatch m_timerAccura;
	//////////////////////////////////////////////////////////////////////////


	theSerialInterFace.SerialOpen(SERIAL_TEMPERATURE);
	CStopWatch m_timerTemperature;	
	BOOL bTempSended = FALSE;
	//////////////////////////////////////////////////////////////////////////
	AxisStatus axisstatus;
	//////////////////////////////////////////////////////////////////////////
	// FDC Write
	//CStopWatch m_timerFDCWrite;
	//BOOL bFDCStart = FALSE;

	theLog[LOG_SEQUENCE].AddBuf(_T("Start FDC Thread\n"));
	//////////////////////////////////////////////////////////////////////////
	while(theThreadInitializer.m_bThrExist[nThreadIndex] == FALSE)
	{
		//2018-01-10, JSJUNG. Sleep(100)->Sleep(300); FDC���� ���� �κ� �״����� ���� ���� test
		//Sleep(100);
		Sleep(300);

		//���α׷� ���� �ʱ�ȭ üũ ���ߴ��� �ʱ�ȭ �� ���� ���� üũ ���ϴ� �����忡�� �ʹ� ���� ��ȣ üũ �ؼ� ���α׷� �״´� 2017/06/09 HSK.
		if(theProcBank.m_bProgramInitFinish == FALSE)
			continue;

		//////////////////////////////////////////////////////////////////////////
		// GMS (1�� �������� GMS, PGMS ������ Read) [12/7/2016 OSC]
		if(pDeviceGMS->IsOpen())
		{
			
			if(pDeviceGMS->ParsingReciveValue())
			{
				//2017-08-31,SKCHO ��հ� �߰�
				pDeviceGMS->AverageValue(5);

				m_timerGMS.Start();
				bGMSSended = FALSE;
			}
			else if( (m_timerGMS.Stop(FALSE) > 1.) && (bGMSSended == FALSE) )
			{
				pDeviceGMS->SendCommand_Read((GMS_MODULE_CH)GMS_CH001_GMS);
		
				//nGMSCh++;
				//nGMSCh %= GMS_MODULE_CH_MAX;
				bGMSSended = TRUE;
				m_timerGMS.Start();
			}
			else if((m_timerGMS.Stop(FALSE) > 10.) && bGMSSended )
			{
				// 10�ʰ� �ѵ��� ������ �ȿ��� bSended�� �ʱ�ȭ�Ͽ� �ٽ� �������� �Ѵ�
				bGMSSended = FALSE;
			}
		}
		//////////////////////////////////////////////////////////////////////////
		// Accura ���·��� (0.5�� �������� Read) [12/7/2016 OSC]
		if(m_timerAccura.Stop(FALSE) > 0.5)
		{
			for(int i = 0; i < ACCURA_CLIENT_MAX; i++)
			{
				DeviceAccura.ReadValue((ACCURA_CLIENT)i);
				//theFDCBank.m_rDataBlock[i] = DeviceAccura.m_rDataBlock[i];
				if((ACCURA_CLIENT)i == ACCURA_CLIENT_UPS)
				{
					DeviceAccura.UPSAverageValue(10);
				}
				else
				{
					DeviceAccura.GPSAverageValue(10);
				}
			}
			m_timerAccura.Start();
		}
		//////////////////////////////////////////////////////////////////////////
		// Torque
		for(int i = 0; i < AXIS_ID_MAX; i++)
		{
			theFDCBank.m_dTorque[i] = theDeviceMotion.GetTorque(MP2100_THREAD_FDC_MONITOR, (AXIS_ID)i);
			//2018-03-24
			if( i==4 || i==7 || i==13 || i==17 || i==21 )
			{
				if( theFDCBank.m_dTorque[i] > 60 || theFDCBank.m_dTorque[i] < -60 )
				{
					theFDCBank.m_dTorque[i] = 5.;
				}
			}
			if( i==14 || i==15 || i==18 || i==19 )
			{
				if( theFDCBank.m_dTorque[i] > 100 || theFDCBank.m_dTorque[i] < -40 )
				{
					theFDCBank.m_dTorque[i] = 5.;
				}
			}
			if( i==3 )
			{
				if( theFDCBank.m_dTorque[i] > 120 || theFDCBank.m_dTorque[i] < -120 )
				{
					theFDCBank.m_dTorque[i] = 5.;
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////
	
		theFDCBank.m_nEFU_Alarm = _func.GetInPutIOCheck(X_EFU_ALARM_SEN, OFF);
		if(theFDCBank.m_nEFU_Alarm == 0)
		{
			theUnitFunc.SetError(ALARM_FFU_SEN);
		}
	
		if(theSerialInterFace.GetVACUUMUDHandler()->IsOpen())
		{
			if(theSerialInterFace.ParsingUDVACUUMValue())
			{
				theSerialInterFace.CopyAirValue_to_FDCBank((SERIAL_MODE)SERIAL_VACUUM_UD);
				m_timerVacUD.Start();
				bVacUDSended = FALSE;
			}
			else if( (m_timerVacUD.Stop(FALSE) > 0.5) && (bVacUDSended == FALSE) )
			{
				theSerialInterFace.SendVacReadUD(nUD_ADSlot_Index);
				bVacUDSended = TRUE;
				m_timerVacUD.Start();
				nUD_ADSlot_Index++;
				if(nUD_ADSlot_Index >= UD_AD_USE_SLOT)
				{
					nUD_ADSlot_Index = 0;
				}
			}
			else if((m_timerVacUD.Stop(FALSE) > 10.) && bVacUDSended )
			{
				// 10�ʰ� �ѵ��� ������ �ȿ��� bSended�� �ʱ�ȭ�Ͽ� �ٽ� �������� �Ѵ�
				bVacUDSended = FALSE;
			}
		}	

		//TEMPERATURE SENSOR
		if(theSerialInterFace.GetTemperatureHandler()->IsOpen())
		{
			nTempIndex = theSerialInterFace.GetDeviceAddr();
			if(theSerialInterFace.ParsingTemperatureValue())
			{				
				if(nReadCnt > 1)
			    { 					
					theSerialInterFace.CopyTempValue_to_FDCBank(nTempIndex);
					theSerialInterFace.GetTemperatureHandler()->IncDeviceAddr();
					nReadCnt = 0;
				}
				nReadCnt++;
				m_timerTemperature.Start();
				bTempSended = FALSE;
			}
			else if( (m_timerTemperature.Stop(FALSE) > 1.0) && (bTempSended == FALSE) )  //0.2->1
			{
				theSerialInterFace.SendTempRead(nTempIndex);
				bTempSended = TRUE;
				m_timerTemperature.Start();				
			}
			else if((m_timerTemperature.Stop(FALSE) > 10.) && bTempSended )
			{
				// 10�ʰ� �ѵ��� ������ �ȿ��� bSended�� �ʱ�ȭ�Ͽ� �ٽ� �������� �Ѵ�
				bTempSended = FALSE;
				//2017-04-14,skcho,10�ʵ��� ���� ������ ���� ��巹���� �Ѿ
				m_timerTemperature.Start();
				theSerialInterFace.GetTemperatureHandler()->IncDeviceAddr();
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// FDC Write 1�ʸ��� ���
		// ó�� 10�ʴ� Data�� ���� ������ �� �� ���� ���� �����־ ��ٸ���
		/*if( (bFDCStart == FALSE) && (m_timerFDCWrite.Stop(FALSE) > 10.) )
		{
			theFDCBank.DeleteFDC_SV_par();
			bFDCStart = TRUE;
		}
		if( bFDCStart && (m_timerFDCWrite.Stop(FALSE) > 1.0) )
		{
			theFDCBank.SaveFDC_SV_par();
			m_timerFDCWrite.Start();
		}*/
		//////////////////////////////////////////////////////////////////////////
	}

	theSerialInterFace.SerialClose(SERIAL_GMS);
	theSerialInterFace.SerialClose(SERIAL_TEMPERATURE);

	theLog[LOG_SEQUENCE].AddBuf(_T("End FDC Thread\n"));
	return 0;
}
//�˻�� �з� ���� ������
UINT VAC_FDCMonitorThr( LPVOID lpParam )
{
	static UINT nLD_ADSlot_Index = 0;
	
	static UINT nINSP_ADSlot_Index = 0;

	int nThreadIndex = (int)lpParam;
	
	//2018-01-10, JSJUNG. �ʿ� ���� �ǴܵǾ� ���� //MP2100�α������� ���� �ٽ� ȸ��. 
	theDeviceMotion.Open_Mp2100(MP2100_THREAD_FDC_MONITOR_VACUUM);
	CUnitCtrlFunc					_func(MP2100_THREAD_FDC_MONITOR_VACUUM);

	// FDC Write
	CStopWatch m_timerFDCWrite;
	BOOL bFDCStart = FALSE;

	//20161229 byskcho
	//���� ������  LOAD�� ����Ǿ� �־� �������� ���� ����
	//20170129 byskcho
	//cim oss ���� �������� ���� ����
#if(AMT_TYPE == 1)
	theSerialInterFace.SerialOpen(SERIAL_VACUUM_LD);
	theSerialInterFace.GetVACUUMLDHandler()->SetValueOffset(6);
	CStopWatch m_timerVacLD;

	theSerialInterFace.SerialOpen(SERIAL_VACUUM_UD);
	theSerialInterFace.GetVACUUMUDHandler()->SetValueOffset(-1.);
	CStopWatch m_timerVacUD;

	theSerialInterFace.SerialOpen(SERIAL_VACUUM_INSP);
	theSerialInterFace.GetVACUUMINSPHandler()->SetValueOffset(-1.);
	CStopWatch m_timerVacINSP;	
#else
	_func.LoadADBoardOffset();

	theSerialInterFace.SerialOpen(SERIAL_VACUUM_INSP);
	CStopWatch m_timerVacINSP;	


#endif
	
	theSerialInterFace.SendSetVacRange(1);  //INSP -1
	BOOL bVacLDSended = FALSE;
	//BOOL bVacUDSended = FALSE;
	BOOL bVacINSPSended = FALSE;

	theFDCBank.m_bFDCCheckFlag = FALSE;

	theLog[LOG_SEQUENCE].AddBuf(_T("Start VACUUM FDC Thread\n"));
	//////////////////////////////////////////////////////////////////////////
	while(theThreadInitializer.m_bThrExist[nThreadIndex] == FALSE)
	{
		//2018-01-11, JSJUNG. TEST : Sleep(10)->Sleep(100)
		Sleep(100);
		if(theSerialInterFace.GetVACUUMINSPHandler()->IsOpen())  //������ �ޱ���� �� 170ms�ҿ�
		{
			if(theSerialInterFace.ParsingINSPVACUUMValue())
			{
				theSerialInterFace.CopyAirValue_to_FDCBank((SERIAL_MODE)SERIAL_VACUUM_INSP);
				m_timerVacINSP.Start();
				bVacINSPSended = FALSE;				
			}
			////2018-01-11, JSJUNG. TEST 
			//else if( (m_timerVacINSP.Stop(FALSE) > 0.05) && (bVacINSPSended == FALSE) )
			else if( (m_timerVacINSP.Stop(FALSE) > 0.2) && (bVacINSPSended == FALSE) ) //
			{
				theSerialInterFace.SendVacReadINSP(nINSP_ADSlot_Index);
				bVacINSPSended = TRUE;
				m_timerVacINSP.Start();
				nINSP_ADSlot_Index++;
				if(nINSP_ADSlot_Index >= INSP_AD_USE_SLOT)
				{
					nINSP_ADSlot_Index = 0;
				}				
			}
			else if((m_timerVacINSP.Stop(FALSE) > 2.) && bVacINSPSended )
			{
				// 2�ʰ� �ѵ��� ������ �ȿ��� bSended�� �ʱ�ȭ�Ͽ� �ٽ� �������� �Ѵ�
				bVacINSPSended = FALSE;
			}
		}
			
		// FDC Write 1�ʸ��� ���
		// ó�� 10�ʴ� Data�� ���� ������ �� �� ���� ���� �����־ ��ٸ���
		if( (bFDCStart == FALSE) && (m_timerFDCWrite.Stop(FALSE) > 10.) )
		{
			//2017-12-27,SKCHO, ���α׷� ���۽� ���� �����͸� ����ϰ� ���� ����°� ����
			//theFDCBank.DeleteFDC_SV_par();
			bFDCStart = TRUE;
		}
		if( bFDCStart && (m_timerFDCWrite.Stop(FALSE) > 1.0) )
		{
			//2018-04-01,GHLEE, FDC ������ ���� �׽�Ʈ
			for( int nPos = (int)CELL_POS_AZONE_CH1; nPos<=(int)CELL_POS_UD_STAGE_CH2; nPos++)
			{
				theUnitFunc.SetFDCCellInfo((CELL_POS) nPos);
			}
			theFDCBank.SaveFDC_SV_par();

			//2018-03-10, NAM, Write FDC to Device Monitoring
			theMonitorBank.WriteFDCLoader();	//OK
			theMonitorBank.WriteFDCZone();		//NO
			theMonitorBank.WriteFDCUld();		//OK
			theMonitorBank.WriteFDCUld_Stage();	//OK
			theMonitorBank.WriteFDCGoodTray();	//OK
			theMonitorBank.WriteFDCNGTray();	//OK
			theMonitorBank.WriteFDCUtil();		//GMS ? ??? ??? ??? ??? ??.
			theMonitorBank.WriteFDCSem();		//OK


			m_timerFDCWrite.Start();
			//2017-12-27,SKCHO FDC ���� ����
			if(theFDCBank.m_bFDCCheckFlag)
			{
				theFDCBank.m_bFDCCheckFlag = FALSE;
			}
			else
			{
				theFDCBank.m_bFDCCheckFlag = TRUE;
			}
		}
	}

	theSerialInterFace.SerialClose(SERIAL_VACUUM_INSP);

	theLog[LOG_SEQUENCE].AddBuf(_T(" End VACUUM FDC Thread\n"));
	return 0;
}
// TMD DOWNLOAD ������
UINT TMDDownLoadThr(LPVOID lpParam)
{
	int nThreadIndex = (int)lpParam;
	CStopWatch m_timerTMD;	//
	CString	szFileName,strFolderPath, strLocalFileName;
	CString strKey, strTmp;
	CString sAction ;				// ���� �������� �ܰ�
	CString sResult;	
	int i = 0;
	int nStep = 0;
	CTokenizer t;
	CIni ini;
	CString strMsg = _T("");
	CDlgTMDDownLoad CDigTMD;	

	theLog[LOG_SEQUENCE].AddBuf( _T("TMD DownLoad Threrad Start"));

	while(theThreadInitializer.m_bThrExist[nThreadIndex] == FALSE)
	{
		//--theThreadInitializer.WaitTMDDownLoadThread();

		if(theThreadInitializer.m_bThrExist[nThreadIndex])
			break;

		if(theConfigBank.m_System.m_nJobProcessStep > 0)
		{			
			nStep = theConfigBank.m_System.m_nJobProcessStep;
			
			switch(nStep)
			{
				case theConfigBank.m_System.eJOB_WaitUserConfirm: //1.READ FILE 
		 
		          	theConfigBank.m_System.ReadTMDJobFileInfo();	
					theLog[LOG_SEQUENCE].AddBuf( _T("ReadTMDJobFileInfo"));		
					GetMainHandler()->GetGUIFrame()->SendMessageDialog(eDLG_MsgTMDDownLoad, UDMSG_TMDDOWNLOAD_LOG, 0, 1);		// ���� ���¹ٿ� ���� ������ �����Ѵ�.
					
					m_timerTMD.Start();
					nStep++;
					break;
				case theConfigBank.m_System.eJOB_Route:  //2. ROUTE �������� download�� path
					if(m_timerTMD.Stop(FALSE) >5.0)
					{
						strTmp = theConfigBank.m_System.m_strServerPath;
						theSocketInterFace.PG_TMD_SendRoute(strTmp);
						theLog[LOG_SEQUENCE].AddBuf( _T("PG_TMD_SendRoute"));		
						GetMainHandler()->GetGUIFrame()->SendMessageDialog(eDLG_MsgTMDDownLoad, UDMSG_TMDDOWNLOAD_LOG, 0, 2);		// ���� ���¹ٿ� ���� ������ �����Ѵ�.
						m_timerTMD.Start();
						nStep++;
					}
					break;
				case theConfigBank.m_System.eJOB_TMDSend:   //3. TMD 
					if(theConfigBank.m_System.m_nJobProcessRcv == theConfigBank.m_System.eJOB_Route &&		// TxHost���� TMD_COPY_START�� �����Ͽ���.
						theConfigBank.m_System.m_nJobProcessState == theConfigBank.m_System.eSTATE_JOB_GOOD)
					{
						    strTmp = theConfigBank.m_System.GetTMDFileList();					// ������ �����ͷ� ��ȣ��� ���������� ���� ����Ʈ ���ڿ��� ������ش�.
							theSocketInterFace.PG_TMD_SendFileList(strTmp);
							theLog[LOG_SEQUENCE].AddBuf( _T("PG_TMD_SendFileList"));
							GetMainHandler()->GetGUIFrame()->SendMessageDialog(eDLG_MsgTMDDownLoad, UDMSG_TMDDOWNLOAD_LOG, 0, 3);		// ���� ���¹ٿ� ���� ������ �����Ѵ�.
							m_timerTMD.Start();
							nStep++;
					}
					else
					{
						if(m_timerTMD.Stop(FALSE) >5.)
						{
							nStep = 0;
							GetMainHandler()->GetGUIFrame()->SendMessageDialog(eDLG_MsgTMDDownLoad, UDMSG_TMDDOWNLOAD_LOG, 0, 4);		// ���� ���¹ٿ� ���� ������ �����Ѵ�.
							//m_timerTMD.Start();
							//nStep++;
						}
					}
					break;
				case theConfigBank.m_System.eJOB_CopyStart:  //4. TMD COPY START 
					if(theConfigBank.m_System.m_nJobProcessRcv == theConfigBank.m_System.eJOB_TMDSend &&		// TxHost���� TMD_COPY_START�� �����Ͽ���.
						theConfigBank.m_System.m_nJobProcessState == theConfigBank.m_System.eSTATE_JOB_GOOD)
					{
						theSocketInterFace.PG_TMD_SendCopyStart();	
						theLog[LOG_SEQUENCE].AddBuf( _T("PG_TMD_SendCopyStart"));
						GetMainHandler()->GetGUIFrame()->SendMessageDialog(eDLG_MsgTMDDownLoad, UDMSG_TMDDOWNLOAD_LOG, 0, 5);		// ���� ���¹ٿ� ���� ������ �����Ѵ�.
						m_timerTMD.Start();
						nStep++;
					}
					else
					{
						if(m_timerTMD.Stop(FALSE) >5.)
						{
							nStep = 0;
							GetMainHandler()->GetGUIFrame()->SendMessageDialog(eDLG_MsgTMDDownLoad, UDMSG_TMDDOWNLOAD_LOG, 0, 6);		// ���� ���¹ٿ� ���� ������ �����Ѵ�.
							//m_timerTMD.Start();
							//nStep++;
						}
					}
					break;
				case theConfigBank.m_System.eJOB_ChecksumStart: //5.TMD CHECKSUM START 
		 	    
					if(theConfigBank.m_System.m_nJobProcessRcv == theConfigBank.m_System.eJOB_CopyStart &&		// TxHost���� TMD_COPY_START�� �����Ͽ���.
						theConfigBank.m_System.m_nJobProcessState == theConfigBank.m_System.eSTATE_JOB_GOOD)
					{
						theSocketInterFace.PG_TMD_SendChecksumStart();
						theLog[LOG_SEQUENCE].AddBuf( _T("PG_TMD_SendChecksumStart"));
						GetMainHandler()->GetGUIFrame()->SendMessageDialog(eDLG_MsgTMDDownLoad, UDMSG_TMDDOWNLOAD_LOG, 0, 7);		// ���� ���¹ٿ� ���� ������ �����Ѵ�.
						nStep++;
						m_timerTMD.Start();
					}
					else 
					{
						if(m_timerTMD.Stop(FALSE) > 5.)
						{
							nStep = 0;
							GetMainHandler()->GetGUIFrame()->SendMessageDialog(eDLG_MsgTMDDownLoad, UDMSG_TMDDOWNLOAD_LOG, 0, 8);		// ���� ���¹ٿ� ���� ������ �����Ѵ�.
							//--theSocketInterFace.PG_TMD_SendChecksumStart();
							//m_timerTMD.Start();
							//nStep++;
						}
					}		
			
					break;
				case theConfigBank.m_System.eJOB_ClientChecksum:  //6.CLIENT CHECKSUM START
					if(theConfigBank.m_System.m_nJobProcessRcv	== theConfigBank.m_System.eJOB_ChecksumStart &&		// ȸ�Ź��� ����
						theConfigBank.m_System.m_nJobProcessState == theConfigBank.m_System.eSTATE_JOB_GOOD)
					{
						theSocketInterFace.PG_TMD_SendClientChecksum();	
						theLog[LOG_SEQUENCE].AddBuf( _T("PG_TMD_SendClientChecksum"));
						GetMainHandler()->GetGUIFrame()->SendMessageDialog(eDLG_MsgTMDDownLoad, UDMSG_TMDDOWNLOAD_LOG, 0, 9);		// ���� ���¹ٿ� ���� ������ �����Ѵ�.
						m_timerTMD.Start();
						nStep++;
					}
					else
					{
						if(m_timerTMD.Stop(FALSE) > 5)
						{
							nStep = 0;
							GetMainHandler()->GetGUIFrame()->SendMessageDialog(eDLG_MsgTMDDownLoad, UDMSG_TMDDOWNLOAD_LOG, 0, 10);		// ���� ���¹ٿ� ���� ������ �����Ѵ�.
							//--theSocketInterFace.PG_TMD_SendClientChecksum();	
							//m_timerTMD.Start();
							//nStep++;
						}
					}			
				
					break;
				case theConfigBank.m_System.eJOB_DownloadStart:  //7.TMD DOWNLOAD START 

					if(theConfigBank.m_System.m_nJobProcessRcv	== theConfigBank.m_System.eJOB_ClientChecksum &&	// CLIENT_CHECKSUM ����� NG�̸� �ٿ�ε� �Ѵ�.
						theConfigBank.m_System.m_nJobProcessState == theConfigBank.m_System.eSTATE_JOB_NG)
					{

						theSocketInterFace.PG_TMD_SendDownloadStart();
						theLog[LOG_SEQUENCE].AddBuf( _T("PG_TMD_SendDownloadStart"));
						GetMainHandler()->GetGUIFrame()->SendMessageDialog(eDLG_MsgTMDDownLoad, UDMSG_TMDDOWNLOAD_LOG, 0, 11);		// ���� ���¹ٿ� ���� ������ �����Ѵ�.
						m_timerTMD.Start();
						nStep++;
					}
					else
					{
						if(m_timerTMD.Stop(FALSE) > 5)
						{
							nStep = 0;
							GetMainHandler()->GetGUIFrame()->SendMessageDialog(eDLG_MsgTMDDownLoad, UDMSG_TMDDOWNLOAD_LOG, 0, 12);		// ���� ���¹ٿ� ���� ������ �����Ѵ�.
							//theSocketInterFace.PG_TMD_SendDownloadStart();
							//m_timerTMD.Start();
							//nStep++;
						}
					}			
			
					break;
				case theConfigBank.m_System.eJOB_Finish:  //8. EVENT REPORT
			
					if(theConfigBank.m_System.m_nJobProcessRcv	== theConfigBank.m_System.eJOB_DownloadStart &&		// TxHost���� TMD_DOWNLOAD_START�� �����Ͽ���.
						theConfigBank.m_System.m_nJobProcessState == theConfigBank.m_System.eSTATE_JOB_GOOD)
					{

						sAction = theConfigBank.m_System.m_strActionFlag;				// ���� �������� �ܰ�
						sResult	= _T("OK");		// ó�� ���
						theSocketInterFace.SendToTMDDownloadState(sAction, sResult);
						theLog[LOG_SEQUENCE].AddBuf( _T("SendToTMDDownloadState"));
						GetMainHandler()->GetGUIFrame()->SendMessageDialog(eDLG_MsgTMDDownLoad, UDMSG_TMDDOWNLOAD_LOG, 0, 13);		// ���� ���¹ٿ� ���� ������ �����Ѵ�.
						nStep++;

					}
					else
					{
						if(m_timerTMD.Stop(FALSE) > 300.)
						{
							GetMainHandler()->GetGUIFrame()->SendMessageDialog(eDLG_MsgTMDDownLoad, UDMSG_TMDDOWNLOAD_LOG, 0, 14);		// ���� ���¹ٿ� ���� ������ �����Ѵ�.
							nStep = 0;
						}
					}				
					break;	
			
				default:
					break;
			}
			theConfigBank.m_System.m_nJobProcessStep = nStep ;
		}
		Sleep(10);
	}
	theLog[LOG_SEQUENCE].AddBuf( _T("TMD DownLoad Threrad End"));
	return 0;
}
//2017-09-26,SKCHO, T5 RESTART ������
UINT T5ReStartThr(LPVOID lpParam)
{

	int nThreadIndex = (int)lpParam;
	CStopWatch m_timerJob;	//
	CString	szFileName,strFolderPath, strLocalFileName;
	CString strKey, strTmp;
	CString sAction ;				// ���� �������� �ܰ�
	CString sResult;	
	int i = 0;
	int nStep = 0;
	CTokenizer t;
	CIni ini;
	CString strMsg = _T("");


	theLog[LOG_SEQUENCE].AddBuf( _T("T5 ReStart Threrad Start"));

	while(theThreadInitializer.m_bThrExist[nThreadIndex] == FALSE)
	{		
		Sleep(100);

		if(theThreadInitializer.m_bThrExist[nThreadIndex])
			break;

	
		

	}

	return 0;

}

//2018-03-03,SKCHO, ���� ��û���� MELSEC�� CELL Ʈ��ŷ ������ �߰�
/*UINT MelSecMonitorThr(LPVOID lpParam)
{

	int nThreadIndex = (int)lpParam;
	int nChannel = 0;
	theDeviceMotion.Open_Mp2100(MP2100_THREAD_MELSEC_MONITOR);
	CUnitCtrlFunc					_func(MP2100_THREAD_MELSEC_MONITOR);

	CStopWatch m_timer100;	//
	CStopWatch m_timer200;	//
	CStopWatch m_timer1000;	//

	int nFirst = 99;
	CCellTag tagCell;
	CCellInfo *pCell;
	BOOL bFirstSet = FALSE;

	m_timer100.Start();
	m_timer200.Start();
	m_timer1000.Start();

	theLog[LOG_SEQUENCE].AddBuf( _T("Melsec  Threrad Start"));

	while(theThreadInitializer.m_bThrExist[nThreadIndex] == FALSE)
	{		
		Sleep(5);

		if(theThreadInitializer.m_bThrExist[nThreadIndex])
			break;

		if(m_timer1000.Stop(FALSE) >= 1.0)  //1������ �ѹ� ����
		{
			theMonitorBank.SetInitData();
			m_timer1000.Start();		
		}

		if(m_timer100.Stop(FALSE) >= 0.1)   //100ms
		{
			 for(int i = CELL_POS_LD_CONV; i < CELL_POS_AZONE_CH1 ; i++)
			 {
				tagCell = theCellBank.GetCellTag((CELL_POS)i);	
				if(tagCell.IsExist())
				{
					theDeviceMelsec.UpperBitOn(OPTICAL_LOADCONVYOR+i);
				}
				else
				{
					theDeviceMelsec.UpperBitOff(OPTICAL_LOADCONVYOR+i);
				}	
			 }
			 
			m_timer100.Start();
		}

		if(m_timer200.Stop(FALSE) >= 0.2)  //200ms
		{
			 nChannel = 0;	
		
			 for(int i = CELL_POS_AZONE_CH1; i <= CELL_POS_UD_ROBOT ; i++)
			 {
				tagCell = theCellBank.GetCellTag((CELL_POS)i);	
				if(tagCell.IsExist())
				{
					pCell = theCellBank.GetCellInfo((CELL_POS)i);					
					theDeviceMelsec.UpperWordSend(OPTICAL_JIG_A_CH1_CELLID+(nChannel*20),(LPVOID)pCell->m_strCellID.GetString(),20,FALSE);
				}
				else
				{
					theDeviceMelsec.UpperWordSend(OPTICAL_JIG_A_CH1_CELLID+(nChannel*20),_T(""),20,FALSE);
				}	
				nChannel++;
			 }
			 
			 m_timer200.Start();
			
		}
		
		

	}

	return 0;

}
*/
